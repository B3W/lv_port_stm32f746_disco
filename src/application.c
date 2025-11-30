/*
 * @file application.c
 */

/*********************
 *      INCLUDES
 *********************/

#include "lvgl/lvgl.h"
#include "lvgl/examples/widgets/lv_example_widgets.h"
#include "lvgl/examples/porting/osal/lv_example_osal.h"
#include "lvgl/src/osal/lv_os_private.h"

#include "hal_stm_lvgl/tft/tft.h"
#include "hal_stm_lvgl/touchpad/touchpad.h"

#include "tx_api.h"
#ifdef TX_INCLUDE_USER_DEFINE_FILE
#include "tx_user.h"
#endif
#include <stdint.h>

/*********************
 *      DEFINES
 *********************/

#define UI_TASK_NAME        "UITask"
#define UI_TASK_PRIORITY    LV_THREAD_PRIO_HIGH
#define UI_TASK_STACK_SIZE  4096
#define UI_REFRESH_DELAY_MS 5U

/*********************
 *      TYPEDEFS
 *********************/

/*********************
 *  STATIC PROTOTYPES
 *********************/

static int32_t CreateUITask(void);
static void ExecuteUITask(void* argument);
static uint32_t GetTickCallback(void);

/*********************
 *  STATIC VARIABLES
 *********************/

static lv_thread_t ui_thread;

/*********************
 *      MACROS
 *********************/

/*********************
 *  GLOBAL FUNCTIONS
 *********************/

void CreateApplication(void)
{
    lv_init();

    // Create the main UI thread
    int32_t error = CreateUITask();

    if (error != 0) {
        while (1)
        {
            // Spin forever
        }
    }
}

/*********************
 *  STATIC FUNCTIONS
 *********************/

int32_t CreateUITask(void)
{
    lv_result_t result = lv_thread_init(&ui_thread,
                                        UI_TASK_NAME,
                                        UI_TASK_PRIORITY,
                                        ExecuteUITask,
                                        UI_TASK_STACK_SIZE,
                                        NULL);
    if (result == LV_RESULT_OK) {
        return 0;
    }
    else {
        return -1;
    }
}

void ExecuteUITask(void* argument)
{
    const uint32_t UI_REFRESH_DELAY_TICKS = (uint32_t)((UI_REFRESH_DELAY_MS * TX_TIMER_TICKS_PER_SECOND) / 1000U);

    lv_tick_set_cb(GetTickCallback);

    lv_stm32f746_display_init();
    touchpad_init();

    // Create separate thread that interacts with UI
    //lv_example_osal();
    lv_example_label_1();

    /* Infinite loop */
    for (;;) {
        lv_timer_handler();
        tx_thread_sleep(UI_REFRESH_DELAY_TICKS);
    }
}

uint32_t GetTickCallback(void)
{
    return (uint32_t)(((uint64_t)tx_time_get() * 1000U) / TX_TIMER_TICKS_PER_SECOND);
}

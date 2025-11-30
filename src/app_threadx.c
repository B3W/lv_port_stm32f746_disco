/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_threadx.c
  * @author  MCD Application Team
  * @brief   ThreadX applicative file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "app_threadx.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

// Define the size of static ThreadX memory byte pool
#define TX_APP_MEM_POOL_SIZE             1024

// Byte Pool=4096 bytes, Byte Pool overhead=8 bytes, Thread overhead=8 bytes
#define STARTUP_THREAD_STACK_SIZE        (1008)
#define STARTUP_THREAD_PRIORITY          (8)
#define STARTUP_THREAD_PREEMPT_THRESHOLD (8)

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

#if defined ( __ICCARM__ )
#pragma data_alignment=4
#endif
static UCHAR tx_byte_pool_buffer[TX_APP_MEM_POOL_SIZE];
static TX_BYTE_POOL tx_app_byte_pool;

static TX_THREAD startup_thread;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

extern void CreateApplication(void);

static void StartupThreadEntry(ULONG thread_input);

/* USER CODE END PFP */

/**
  * @brief  Application ThreadX Initialization.
  * @param memory_ptr: memory pointer
  * @retval int
  */
UINT App_ThreadX_Init(VOID *memory_ptr)
{
  UINT ret = TX_SUCCESS;
  TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;

  /* USER CODE BEGIN App_ThreadX_MEM_POOL */

  CHAR* stack_pointer = 0;

  /* Allocate stack for Startup Thread from byte pool. */
  if (tx_byte_allocate(byte_pool,
                       (VOID **) &stack_pointer,
                       STARTUP_THREAD_STACK_SIZE,
                       TX_NO_WAIT) != TX_SUCCESS)
  {
    ret = TX_POOL_ERROR;
  }

  /* USER CODE END App_ThreadX_MEM_POOL */

  /* USER CODE BEGIN App_ThreadX_Init */

  if (ret == TX_SUCCESS)
  {
    ret = tx_thread_create(&startup_thread,
                           (CHAR *)"StartupThread",
                           StartupThreadEntry,
                           0,
                           stack_pointer,
                           STARTUP_THREAD_STACK_SIZE,
                           STARTUP_THREAD_PRIORITY,
                           STARTUP_THREAD_PREEMPT_THRESHOLD,
                           TX_NO_TIME_SLICE,
                           TX_AUTO_START);
  }

  /* USER CODE END App_ThreadX_Init */

  return ret;
}

/**
  * @brief  MX_ThreadX_Init
  * @param  None
  * @retval None
  */
void MX_ThreadX_Init(void)
{
  /* USER CODE BEGIN  Before_Kernel_Start */

  /* USER CODE END  Before_Kernel_Start */

  tx_kernel_enter();

  /* USER CODE BEGIN  Kernel_Start_Error */

  /* USER CODE END  Kernel_Start_Error */
}

/* USER CODE BEGIN 1 */

/**
  * @brief  Define the initial system.
  * @param  first_unused_memory : Pointer to the first unused memory
  * @retval None
  */
VOID tx_application_define(VOID *first_unused_memory)
{
  VOID *memory_ptr;

  if (tx_byte_pool_create(&tx_app_byte_pool, "Tx App memory pool", tx_byte_pool_buffer, TX_APP_MEM_POOL_SIZE) != TX_SUCCESS)
  {
      // TX_Byte_Pool Error
      // TODO Handle error
  }
  else
  {
    // TX_Byte_Pool Success
    memory_ptr = (VOID *)&tx_app_byte_pool;

    if (App_ThreadX_Init(memory_ptr) != TX_SUCCESS)
    {
        // App_ThreadX_Init Error
        // TODO Handle error
    }

    // App_ThreadX_Init Success
  }
}

void StartupThreadEntry(ULONG thread_input)
{
   // Do our task work in Application
   CreateApplication();
}

/* USER CODE END 1 */

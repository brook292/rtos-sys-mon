/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "task_utils.h"
#include "cli.h"
#include "usart.h"
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define CLI_FLAG_LINE_READY (1U << 0)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
volatile uint8_t rx_data;
osEventFlagsId_t cliFlags;
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t HeartBeatTaskHandle;
osThreadId_t CliTaskHandle;
const osThreadAttr_t HeartBeatTask_attributes = {
  .name = "HeartBeatTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};

const osThreadAttr_t CliTask_attributes = {
  .name = "CliTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void HeartBeatTask(void *argument);
void CliTask(void *argument);
/* USER CODE END FunctionPrototypes */



void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
  CLI_Init();
  HAL_UART_Receive_IT(&huart2, (uint8_t *)&rx_data, sizeof(rx_data));
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
	TaskCreateSafe(HeartBeatTask, NULL, &HeartBeatTask_attributes, &HeartBeatTaskHandle);
	TaskCreateSafe(CliTask, NULL, &CliTask_attributes, &CliTaskHandle);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
   cliFlags = osEventFlagsNew(NULL);
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void HeartBeatTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
	HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
    osDelay(1000);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

void CliTask(void *argument)
{
   uint8_t cmd_buf[CLI_LINE_BUF_SIZE];
  for(;;)
  {
    osEventFlagsWait(cliFlags, CLI_FLAG_LINE_READY, osFlagsWaitAny,osWaitForever);
    while (CLI_GetLine(cmd_buf, sizeof(cmd_buf)))
    	 {
    	   // TODO: process command in future
    	 }
  }
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART2)
	{
	  CLI_PutChar(rx_data);
	  if(rx_data == '\n' || rx_data == '\r' )
	  {
       //set event flag
		osEventFlagsSet(cliFlags, CLI_FLAG_LINE_READY);
	  }
	  HAL_UART_Receive_IT(&huart2, (uint8_t *)&rx_data, sizeof(rx_data));
	}
}
/* USER CODE END Application */


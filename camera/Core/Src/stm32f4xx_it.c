/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "main.h"
#include "stm32f4xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "camera.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_usart2_tx;
extern UART_HandleTypeDef huart2;
extern TIM_HandleTypeDef htim1;

/* USER CODE BEGIN EV */
extern uint8_t ucExitCritical;
extern TaskHandle_t xCameraTaskCriticalHandle;

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles DMA1 stream6 global interrupt.
  */
void DMA1_Stream6_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream6_IRQn 0 */

  /* USER CODE END DMA1_Stream6_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart2_tx);
  /* USER CODE BEGIN DMA1_Stream6_IRQn 1 */

  /* USER CODE END DMA1_Stream6_IRQn 1 */
}

/**
  * @brief This function handles TIM1 update interrupt and TIM10 global interrupt.
  */
void TIM1_UP_TIM10_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_UP_TIM10_IRQn 0 */

  /* USER CODE END TIM1_UP_TIM10_IRQn 0 */
  HAL_TIM_IRQHandler(&htim1);
  /* USER CODE BEGIN TIM1_UP_TIM10_IRQn 1 */

  /* USER CODE END TIM1_UP_TIM10_IRQn 1 */
}

/**
  * @brief This function handles USART2 global interrupt.
  */
void USART2_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */

  /* USER CODE END USART2_IRQn 0 */
  HAL_UART_IRQHandler(&huart2);
  /* USER CODE BEGIN USART2_IRQn 1 */

  /* USER CODE END USART2_IRQn 1 */
}

/* USER CODE BEGIN 1 */

// Message with the camera info.
static msg_t xMsg;


/*
 * @brief: PCLK interrupt handler. The bytes read are stored in a global buffer.
 */
void EXTI0_IRQHandler(void) {
	if(__HAL_GPIO_EXTI_GET_IT(PCLK_Pin) != RESET)
	{
		__HAL_GPIO_EXTI_CLEAR_IT(PCLK_Pin);
		xMsg.pucBuffer[xMsg.usBytes++] = cameraGPIO_IDR >> 5;
	}
}


/*
 * @brief	VSYNC IRQ handler. When a rising edge is detected, a debounce
 * 			routine is started.
 */
void EXTI1_IRQHandler(void) {
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	if(__HAL_GPIO_EXTI_GET_IT(VSYNC_Pin) != RESET) {
		__HAL_GPIO_EXTI_CLEAR_IT(VSYNC_Pin);

		vDelayUs(10);
		if (RISING_EDGE_VSYNC()) {
			vCameraEnableHREFIRQ();
			xMsg.usRow = 0;
		}

		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}
}


/*
 * @brief: 	HREF interrupt handler. On rising edge, the PCLK interrupt will be
 * 			enabled and the camera bytes read. On falling edge, all the camera information
 * 			will be copied to the Stream Buffer.
 */
void EXTI15_10_IRQHandler(void) {
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	if(__HAL_GPIO_EXTI_GET_IT(HREF_Pin) != RESET) {
		__HAL_GPIO_EXTI_CLEAR_IT(HREF_Pin);

		// Start of new pixel row.
		if (RISING_EDGE_HREF()) {
			vCameraEnablePCLKIRQ();

			// Resume the critical task.
			xHigherPriorityTaskWoken = xTaskResumeFromISR(xCameraTaskCriticalHandle);

		}

		// End of row.
		else {
			vCameraDisablePCLKIRQ();
			xQueueSendToBackFromISR(xCameraImageQueue, &xMsg, &xHigherPriorityTaskWoken);
			xMsg.usRow++;

			// The first 2 pixels of the camera will be ignored. This interruption handling and the
			// scheduler context switch are too slow to read the first bytes.
			xMsg.usBytes = 4;

			// End of critical task section.
			ucExitCritical = 1;
		}
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}
}


/*
 * 	@brief  ISR called when the DMA transfer was completed. It wakes up the
 * 			task to start sending more data (if there is any left).
 */
void HAL_UART_TxCpltCallback (UART_HandleTypeDef * huart) {
	BaseType_t xHigherPriorityTaskWoken;
	xSemaphoreGiveFromISR(xCameraSemUart, &xHigherPriorityTaskWoken);
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

/*
 * @brief:	ISR called when an order from the computer (UART2)
 * 			is received. It sends a notification to the task to
 * 			process it.
 */
void HAL_UART_RxCpltCallback (UART_HandleTypeDef * huart) {
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	vTaskNotifyGiveFromISR(xCameraTaskUartHandle, &xHigherPriorityTaskWoken);
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

/* USER CODE END 1 */

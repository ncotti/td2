/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#define mainDEBUG

#include "cmsis_os.h"
#include "stream_buffer.h"
#include "event_groups.h"

#ifdef mainDEBUG
#include <stdio.h>
extern void initialise_monitor_handles(void);
#endif

//TODO circular include
#include "camera.h"


/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
extern UART_HandleTypeDef huart2;

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */


/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */


#ifdef mainDEBUG
void vDebugErrorRoutine(const char * pcErrorString);
void vDebugSuccessRoutine(const char * pcSuccessString);
#endif

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define PCLK_Pin GPIO_PIN_0
#define PCLK_GPIO_Port GPIOC
#define PCLK_EXTI_IRQn EXTI0_IRQn
#define VSYNC_Pin GPIO_PIN_1
#define VSYNC_GPIO_Port GPIOC
#define VSYNC_EXTI_IRQn EXTI1_IRQn
#define D0_Pin GPIO_PIN_5
#define D0_GPIO_Port GPIOC
#define STEPPER_1_Pin GPIO_PIN_1
#define STEPPER_1_GPIO_Port GPIOB
#define STEPPER_4_Pin GPIO_PIN_13
#define STEPPER_4_GPIO_Port GPIOB
#define STEPPER_3_Pin GPIO_PIN_14
#define STEPPER_3_GPIO_Port GPIOB
#define STEPPER_2_Pin GPIO_PIN_15
#define STEPPER_2_GPIO_Port GPIOB
#define D1_Pin GPIO_PIN_6
#define D1_GPIO_Port GPIOC
#define D2_Pin GPIO_PIN_7
#define D2_GPIO_Port GPIOC
#define D3_Pin GPIO_PIN_8
#define D3_GPIO_Port GPIOC
#define D4_Pin GPIO_PIN_9
#define D4_GPIO_Port GPIOC
#define HREF_Pin GPIO_PIN_12
#define HREF_GPIO_Port GPIOA
#define HREF_EXTI_IRQn EXTI15_10_IRQn
#define D5_Pin GPIO_PIN_10
#define D5_GPIO_Port GPIOC
#define D6_Pin GPIO_PIN_11
#define D6_GPIO_Port GPIOC
#define D7_Pin GPIO_PIN_12
#define D7_GPIO_Port GPIOC
#define SCLK_Pin GPIO_PIN_6
#define SCLK_GPIO_Port GPIOB
#define SDA_Pin GPIO_PIN_7
#define SDA_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

#define ULONG_MAX 0xFFFFFFFF

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

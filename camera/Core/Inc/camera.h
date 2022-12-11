/*
 * camera.h
 *
 *  Created on: 19 jul. 2022
 *      Author: cotti
 */

#ifndef INC_CAMERA_H_
#define INC_CAMERA_H_

/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include "main.h"
#include "camera_reg.h"
#include "camera_api.h"
#include "stepper.h"

extern TaskHandle_t xCameraTaskControllerHandle;

extern TaskHandle_t xCameraTaskUartHandle;

extern StreamBufferHandle_t xCameraStreamBufferUart;

extern SemaphoreHandle_t xCameraSemUart;

extern QueueHandle_t xCameraImageQueue;

/******************************************************************************
 * DEFINES
 *****************************************************************************/
#define cameraTASK_CONTROLLER_PRIORITY 2
#define cameraTASK_CONTROLLER_STACK_SIZE 3000

#define cameraTASK_UART_PRIORITY 2
#define cameraTASK_UART_STACK_SIZE 2000
#define cameraTASK_CRITICAL_PRIORITY configMAX_PRIORITIES

#define cameraGPIO_IDR GPIOC->IDR
#define cameraGPIO_OFFSET 5

#define cameraUART_TIMEOUT 100

#define cameraBYTES_PER_PIXEL 2
#define cameraH 120
#define cameraW 156

#define cameraSOF_HEADER "STR__\n"
#define cameraEOR_FOOTER "ROW__\n"
#define cameraEOF_FOOTER "END__\n"

#define cameraSTART_OF_ORDER 	0xFA
#define cameraEND_OF_ORDER		0xFE

#define cameraQUEUE_LENGHT (cameraH)
#define cameraQUEUE_ITEM_SIZE (cameraW*cameraBYTES_PER_PIXEL)
#define cameraQUEUE_READ_TIMEOUT 100

typedef struct {
	uint8_t pucBuffer [cameraQUEUE_ITEM_SIZE + 10];
	uint16_t usRow;
	uint16_t usBytes;
} msg_t;

extern uint8_t ucExitCritical;
extern TaskHandle_t xCameraTaskCriticalHandle;


/******************************************************************************
 * MACROS
 *****************************************************************************/
#define vCameraEnableVSYNCIRQ() {__HAL_GPIO_EXTI_CLEAR_IT(VSYNC_Pin); \
	HAL_NVIC_EnableIRQ(EXTI1_IRQn);}
#define vCameraEnableHREFIRQ() {__HAL_GPIO_EXTI_CLEAR_IT(HREF_Pin); \
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);}
#define vCameraEnablePCLKIRQ() {HAL_NVIC_EnableIRQ(EXTI0_IRQn);}
#define vCameraDisableVSYNCIRQ() (HAL_NVIC_DisableIRQ(EXTI1_IRQn))
#define vCameraDisableHREFIRQ() (HAL_NVIC_DisableIRQ(EXTI15_10_IRQn))
#define vCameraDisablePCLKIRQ() (HAL_NVIC_DisableIRQ(EXTI0_IRQn))

#define RISING_EDGE_VSYNC() (HAL_GPIO_ReadPin(VSYNC_GPIO_Port, VSYNC_Pin) == GPIO_PIN_SET)
#define RISING_EDGE_HREF() (HAL_GPIO_ReadPin(HREF_GPIO_Port, HREF_Pin) == GPIO_PIN_SET)

/******************************************************************************
 * FUNCTION DEFINITIONS
 *****************************************************************************/
void vCameraInit(void);

#endif /* INC_CAMERA_H_ */

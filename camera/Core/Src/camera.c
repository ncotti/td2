/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include "camera.h"

/******************************************************************************
 * STATIC FUNCITONS PROTOTYPES
 *****************************************************************************/
static void prvCameraTaskController(void* pvParameters);
static void prvCameraTaskUart (void* pvParameters);
static void prvCriticalTask(void* pvParameters);

/******************************************************************************
 * GLOBAL VARIABLES
 *****************************************************************************/
// Reference to the camera controller task.
TaskHandle_t xCameraTaskControllerHandle;

// Reference to the UART camera communication task.
TaskHandle_t xCameraTaskUartHandle;

TaskHandle_t xCameraTaskCriticalHandle;

QueueHandle_t xCameraImageQueue;

SemaphoreHandle_t xCameraSemUart;

uint8_t ucExitCritical = 0;

/******************************************************************************
 * FUNCTIONS
 *****************************************************************************/
/*
 * @brief: Resource initialization for the camera operation.
 */
void vCameraInit(void) {

	vCameraDisablePCLKIRQ();
	vCameraDisableHREFIRQ();
	vCameraDisableVSYNCIRQ();

	if ((xCameraSemUart = xSemaphoreCreateBinary()) == NULL) {
		#ifdef mainDEBUG
		vDebugErrorRoutine("Failed to create sem for uart.");
		#endif
		vToolsErrorRoutine();
	}

	if ( xTaskCreate(
			prvCameraTaskController,
			"camera_control",
			cameraTASK_CONTROLLER_STACK_SIZE,
			NULL,
			cameraTASK_CONTROLLER_PRIORITY,
			&xCameraTaskControllerHandle ) != pdPASS) {

		#ifdef mainDEBUG
		vDebugErrorRoutine("Failed to create task.");
		#endif
		vToolsErrorRoutine();

	}

	if ((xCameraImageQueue = xQueueCreate(cameraQUEUE_LENGHT, sizeof(msg_t))) == NULL) {
		#ifdef mainDEBUG
		vDebugErrorRoutine("Failed to create image queue.");
		#endif
		vToolsErrorRoutine();
	}

	if ( xTaskCreate(prvCameraTaskUart,
			"camera_uart",
			cameraTASK_UART_STACK_SIZE,
			NULL,
			cameraTASK_UART_PRIORITY,
			&xCameraTaskUartHandle) != pdPASS ) {

		#ifdef mainDEBUG
		vDebugErrorRoutine("Failed to create task.");
		#endif
		vToolsErrorRoutine();
	}

	if ( xTaskCreate(prvCriticalTask,
			"critical_task",
			configMINIMAL_STACK_SIZE,
			NULL,
			cameraTASK_CRITICAL_PRIORITY,
			&xCameraTaskCriticalHandle) != pdPASS ) {

		#ifdef mainDEBUG
		vDebugErrorRoutine("Failed to create task.");
		#endif
		vToolsErrorRoutine();
	}

	vCameraRegInit();
}


/******************************************************************************
 * RTOS TASKS
 *****************************************************************************/
/*
 * 	@brief	RTOS task that handles the UART communication for the camera pixels.
 */
static void prvCameraTaskUart (void* pvParameters) {

	uint8_t pucUartRx [4];

	void (*pfOptions[])(void) = {
			vCameraTestPatternOn, vCameraTestPatternOff,
			vCameraColorBarOn, vCameraColorBarOff,
			vCameraHMirrorOn, vCameraHMirrorOff,
			vCameraVFlipOn, vCameraVFlipOff,
			vCameraFreezeAGCAEC, vCameraUnfreezeAGCAEC,
			vCameraRGB565, vCameraYUV,
			vCameraAWBR1, vCameraAWBR125, vCameraAWBR15, vCameraAWBR175,
			vCameraAWBB1, vCameraAWBB125, vCameraAWBB15, vCameraAWBB175,
			vCameraAWBGB1, vCameraAWBGB125, vCameraAWBGB15, vCameraAWBGB175,
			vCameraAWBGR1, vCameraAWBGR125, vCameraAWBGR15, vCameraAWBGR175,
			vCameraReset,
			vStepperStart, vStepperStop, vStepperChangeDirection,
			vCameraManualAGCAEC, vCameraAutoAGCAEC
	};

	while (1) {
		do{
			HAL_UART_Receive_IT(&huart2, pucUartRx, 4);
			ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		}while(pucUartRx[0] != cameraSTART_OF_ORDER || pucUartRx[3] != cameraEND_OF_ORDER);

		if (pucUartRx[1] == 0xFF) {
			if (pucUartRx[2] < sizeof(pfOptions) / sizeof(pfOptions[0])) {
				pfOptions[pucUartRx[2]]();
			}
		}

		else if (pucUartRx[1] == 0xFE) {
			vStepperChangeVelocity((uint16_t) pucUartRx[2]);
		}

		else {
			vCameraSetReg(pucUartRx[1], pucUartRx[2]);
		}
	}
}


/*
 * 	@brief: RTOS task that handles photo petitions.
 */
static void prvCameraTaskController(void* pvParameters) {

	msg_t xMsg;
	uint16_t i;

	if (xCameraProbe() == pdFAIL) {
		#ifdef mainDEBUG
		vDebugErrorRoutine("Failed connection to camera.");
		#endif
		vToolsErrorRoutine();
	}

	else {
		#ifdef mainDEBUG
		vDebugSuccessRoutine("Camera Working.");
		#endif
	}

	vCameraReset();

	xSemaphoreGive(xCameraSemUart);

	while(1) {

		if (xQueueReceive(xCameraImageQueue, &xMsg, pdMS_TO_TICKS(cameraQUEUE_READ_TIMEOUT)) == pdTRUE) {

			if (xMsg.usRow == 0) {
				xSemaphoreTake(xCameraSemUart, portMAX_DELAY);
				HAL_UART_Transmit_DMA(&huart2, (uint8_t *) cameraSOF_HEADER, sizeof(cameraSOF_HEADER) - 1);
			}

			xSemaphoreTake(xCameraSemUart, portMAX_DELAY);

			// The first 4 bytes weren't read, fill with trash
			for (i = 0; i < 4; i++){
				xMsg.pucBuffer[i] = 0xFF;
			}

			// Take as many bytes as were read, and complete the rest of the image with the garbage ones.
			xMsg.usBytes -= 4;
			i = cameraQUEUE_ITEM_SIZE - xMsg.usBytes;

			HAL_UART_Transmit_DMA(&huart2, &xMsg.pucBuffer[i], cameraQUEUE_ITEM_SIZE );

			xSemaphoreTake(xCameraSemUart, portMAX_DELAY);
			HAL_UART_Transmit_DMA(&huart2, (uint8_t *) cameraEOR_FOOTER, sizeof(cameraEOR_FOOTER) - 1);


			if (xMsg.usRow == cameraH - 1) {
				xSemaphoreTake(xCameraSemUart, portMAX_DELAY);
				HAL_UART_Transmit_DMA(&huart2, (uint8_t *) cameraEOF_FOOTER, sizeof(cameraEOF_FOOTER) - 1);
			}
		}
	}
}

/*
 * @brief: 	Critical section. This task is called when an HREF is detected, and exits when the
 * 			HREF stops. During photo's byte transfers, the RTOS scheduler context switches
 * 			may cause a byte to be missed, and corrupt the whole image. Therefore, the RTOS
 * 			scheduler is suspended to ensure picture integrity. Besides, the sole act of pausing
 * 			the scheduler may produce a missing byte in one of the first 4 bytes.
 */
static void prvCriticalTask(void* pvParameters) {
	while(1) {
		vTaskSuspend(NULL);
		vTaskSuspendAll();
		ucExitCritical = 0;
		while( ! ucExitCritical) {
			// delay while pixels are read.
		}
		xTaskResumeAll();
	}
}




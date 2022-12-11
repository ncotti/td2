/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include "stepper.h"

/******************************************************************************
 * STATIC PROTOTYPES
 *****************************************************************************/
static void prvStepperTask (void* pvParameters);

/******************************************************************************
 * GLOBAL STATIC VARIABLES
 *****************************************************************************/
static TaskHandle_t xStepperTaskHandle;

/******************************************************************************
 * FUNCTIONS
 *****************************************************************************/
/*
 *	@brief: Resource initialization for the stepper motor action. To be called
 *		before initializing the RTOS scheduler.
 */
void vStepperInit (void) {
	xTaskCreate(prvStepperTask, "stepper_task", configMINIMAL_STACK_SIZE, NULL, stepperTASK_PRIORITY, &xStepperTaskHandle);
}

/*
 *	@brief: Starts stepper motor operation. Not ISR safe.
 */
void vStepperStart(void) {
	xTaskNotify(xStepperTaskHandle, stepperSTART_NOTIFICATION, eSetBits);
}

/*
 *	@brief: Starts stepper motor operation from an ISR.
 */
void vStepperStartFromISR(BaseType_t* pxTaskWoken) {
	xTaskNotifyFromISR(xStepperTaskHandle, stepperSTART_NOTIFICATION, eSetBits, pxTaskWoken);
}

/*
 *	@brief: Stops stepper motor operation. Not ISR safe.
 */
void vStepperStop(void) {
	xTaskNotify(xStepperTaskHandle, stepperSTOP_NOTIFICATION, eSetBits);
}

/*
 *	@brief: Stops stepper motor operation from an ISR.
 */
void vStepperStopFromISR(BaseType_t* pxTaskWoken) {
	xTaskNotifyFromISR(xStepperTaskHandle, stepperSTOP_NOTIFICATION, eSetBits, pxTaskWoken);
}

/*
 *	@brief: Changes the direction of the motor. Not ISR safe.
 */
void vStepperChangeDirection(void) {
	xTaskNotify(xStepperTaskHandle, stepperDIRECTION_NOTIFICATION, eSetBits);
}

/*
 *	@brief: Changes the direction of the motor from an ISR.
 */
void vStepperChangeDirectionFromISR(BaseType_t* pxTaskWoken) {
	xTaskNotifyFromISR(xStepperTaskHandle, stepperDIRECTION_NOTIFICATION, eSetBits, pxTaskWoken);
}

/*
 *	@brief: Changes the rotational speed of the motor. The velocity value
 *		represent milisenconds per step. Not ISR safe.
 *
 *	@param:	usVelocity, rotational speed. Lower values (starting from 5),
 *		mean higher speeds.
 */
void vStepperChangeVelocity(uint16_t usVelocity) {
	if (usVelocity < 5) {
		usVelocity = 5;
	}
	xTaskNotify(xStepperTaskHandle, stepperVELOCITY_NOTIFICATION | (usVelocity << stepperVELOCITY_INDEX), eSetBits);
}

/*
 *	@brief: same as vStepperChangeVelocity(), but ISR safe.
 *
 *	@param:	usVelocity, rotational speed. Lower values (starting from 5),
 *		mean higher speeds.
 */
void vStepperChangeVelocityFromISR(uint16_t usVelocity, BaseType_t* pxTaskWoken) {
	if (usVelocity < 5) {
		usVelocity = 5;
	}
	xTaskNotifyFromISR(xStepperTaskHandle, stepperVELOCITY_NOTIFICATION & (usVelocity << stepperVELOCITY_INDEX), eSetBits, pxTaskWoken);

}


/******************************************************************************
 * STATIC FUNCTIONS
 *****************************************************************************/
/*
 *	@brief: Manages the motor's step sequence. It waits for a notification,
 *		which could be either start the motor, stop the motor, change direction
 *		or change velocity. If the last one is true, then a Queue is read for
 *		the velocity change.
 */
static void prvStepperTask (void* pvParameters) {
	uint8_t ucWorking = stepperON;
	uint8_t ucDirection = stepperFORWARD;
	uint16_t usVelocity = stepperDEFAULT_VELOCITY;
	uint32_t ulNotification;

	while(1) {

		if (xTaskNotifyWait(0, ULONG_MAX, &ulNotification, 0) == pdTRUE) {
			if (ulNotification & stepperSTART_NOTIFICATION) {
				ucWorking = stepperON;
			}

			if (ulNotification & stepperSTOP_NOTIFICATION) {
				ucWorking = stepperOFF;
			}

			if (ulNotification & stepperDIRECTION_NOTIFICATION) {
				ucDirection = !ucDirection;
			}

			if (ulNotification & stepperVELOCITY_NOTIFICATION) {
				usVelocity = (uint16_t) (ulNotification >> stepperVELOCITY_INDEX);
			}
		}

		if (ucWorking == stepperON) {
			if (ucDirection) {

				STEPPER_3_LOW();
				STEPPER_1_HIGH();

				vTaskDelay(pdMS_TO_TICKS(usVelocity));

				STEPPER_4_LOW();
				STEPPER_2_HIGH();

				vTaskDelay(pdMS_TO_TICKS(usVelocity));

				STEPPER_1_LOW();
				STEPPER_3_HIGH();

				vTaskDelay(pdMS_TO_TICKS(usVelocity));

				STEPPER_2_LOW();
				STEPPER_4_HIGH();

				vTaskDelay(pdMS_TO_TICKS(usVelocity));

			}

			else {
				STEPPER_2_LOW();
				STEPPER_4_HIGH();

				vTaskDelay(pdMS_TO_TICKS(usVelocity));

				STEPPER_1_LOW();
				STEPPER_3_HIGH();

				vTaskDelay(pdMS_TO_TICKS(usVelocity));

				STEPPER_4_LOW();
				STEPPER_2_HIGH();

				vTaskDelay(pdMS_TO_TICKS(usVelocity));

				STEPPER_3_LOW();
				STEPPER_1_HIGH();

				vTaskDelay(pdMS_TO_TICKS(usVelocity));

			}

		}

	}
}






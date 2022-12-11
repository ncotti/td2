#ifndef INC_STEPPER_H_
#define INC_STEPPER_H_

/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include "cmsis_os.h"
#include "main.h"

/******************************************************************************
 * DEFINES
 *****************************************************************************/
#define stepperSTART_NOTIFICATION 0x01
#define stepperSTOP_NOTIFICATION 0x02
#define stepperDIRECTION_NOTIFICATION 0x04
#define stepperVELOCITY_NOTIFICATION 0x08
#define stepperVELOCITY_INDEX 16

#define stepperTASK_PRIORITY 1

#define stepperOFF 0
#define stepperON 1
#define stepperFORWARD 0
#define stepperBACKWARD 1
#define stepperDEFAULT_VELOCITY 40 //Note: MAX_SPEED ~ 5, lower missbehaves.

/******************************************************************************
 * MACROS
 *****************************************************************************/
#define STEPPER_1_HIGH() 	(HAL_GPIO_WritePin(STEPPER_1_GPIO_Port, STEPPER_1_Pin, GPIO_PIN_SET))
#define STEPPER_1_LOW() 	(HAL_GPIO_WritePin(STEPPER_1_GPIO_Port, STEPPER_1_Pin, GPIO_PIN_RESET))

#define STEPPER_2_HIGH()	(HAL_GPIO_WritePin(STEPPER_2_GPIO_Port, STEPPER_2_Pin, GPIO_PIN_SET))
#define STEPPER_2_LOW()		(HAL_GPIO_WritePin(STEPPER_2_GPIO_Port, STEPPER_2_Pin, GPIO_PIN_RESET))

#define STEPPER_3_HIGH()	(HAL_GPIO_WritePin(STEPPER_3_GPIO_Port, STEPPER_3_Pin, GPIO_PIN_SET))
#define STEPPER_3_LOW()		(HAL_GPIO_WritePin(STEPPER_3_GPIO_Port, STEPPER_3_Pin, GPIO_PIN_RESET))

#define STEPPER_4_HIGH()	(HAL_GPIO_WritePin(STEPPER_4_GPIO_Port, STEPPER_4_Pin, GPIO_PIN_SET))
#define STEPPER_4_LOW()		(HAL_GPIO_WritePin(STEPPER_4_GPIO_Port, STEPPER_4_Pin, GPIO_PIN_RESET))


/******************************************************************************
 * FUNCTION DEFINITIONS
 *****************************************************************************/
void vStepperInit (void);
void vStepperStart(void);
void vStepperStartFromISR(BaseType_t* pxTaskWoken);
void vStepperStop(void);
void vStepperStopFromISR(BaseType_t* pxTaskWoken);
void vStepperChangeDirection(void);
void vStepperChangeDirectionFromISR(BaseType_t* pxTaskWoken);
void vStepperChangeVelocity(uint16_t usVelocity);
void vStepperChangeVelocityFromISR(uint16_t usVelocity, BaseType_t* pxTaskWoken);


#endif /* INC_STEPPER_H_ */

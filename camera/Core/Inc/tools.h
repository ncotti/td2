/*
 * tools.h
 *
 *  Created on: Aug 6, 2022
 *      Author: cotti
 */

#ifndef INC_TOOLS_H_
#define INC_TOOLS_H_

/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include "main.h"

/******************************************************************************
 * DEFINES
 *****************************************************************************/


/******************************************************************************
 * MACROS
 *****************************************************************************/
/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/
void vDelayUs(uint16_t n);
void vToolsErrorRoutine(void);
void vToolsDebounceTask(void* pvParameters);
void vToolsInit(void);
uint8_t ucToolsHex(uint8_t* pucHex);

inline uint8_t ucToolsCheckNoti(uint32_t ulVariable, uint32_t ulNoti) {
	return (ulVariable & ulNoti);
}

extern EventGroupHandle_t xDebounceEvent;

#endif /* INC_TOOLS_H_ */

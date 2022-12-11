/*
 * tools.c
 *
 *  Created on: Aug 6, 2022
 *      Author: cotti
 */
#include "tools.h"

EventGroupHandle_t xDebounceEvent;

void vDelayUs(uint16_t n) {
	TIM2->CNT = 0;
	while (TIM2->CNT < n);
}


void vToolsErrorRoutine(void) {
	while(1) {}
}

void vToolsInit(void) {
	//xDebounceEvent = xEventGroupCreate();
	//xTaskCreate (vToolsDebounceTask, "debounce_task", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
}

void vToolsDebounceTask(void* pvParameters) {
	while (1) {
		xEventGroupWaitBits (xDebounceEvent, 1 << 0, pdTRUE, pdFALSE, portMAX_DELAY);

		vDelayUs(10);
		if (RISING_EDGE_VSYNC()) {
			vCameraEnableHREFIRQ();
			xEventGroupSetBits(xDebounceEvent, 1 << 1);
		}
	}
}

uint8_t ucToolsHex(uint8_t* pucHex) {
	uint8_t i;

	for (i = 0; i < 2; i++) {
		if (pucHex[i] > '9') {
			pucHex[i] -= 'A';
		}

		else {
			pucHex[i] -= '0';
		}
	}
	return pucHex[0]*16 + pucHex[1];
}



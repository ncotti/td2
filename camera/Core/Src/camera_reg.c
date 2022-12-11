/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include "camera_reg.h"

/******************************************************************************
 * GLOBAL VARIABLES
 *****************************************************************************/
static SemaphoreHandle_t xCameraMutexSCCB; // Mutex to block I2C operation.

/******************************************************************************
 * STATIC FUNCITONS PROTOTYPES
 *****************************************************************************/
static void prvCameraWriteReg (uint8_t ucReg, uint8_t ucData);
static uint8_t prucCameraReadReg (uint8_t ucReg);

#ifdef cameraBIT_BANG
static void prvSDAOutput(void);
static void prvSDAInput(void);
static void prvWrite(uint8_t byte);
static void prvStart(void);
static void prvStop(void);
static uint8_t prucRead(void);
#endif


/******************************************************************************
 * REGISTER OPERATIONS
 *****************************************************************************/
void vCameraChangeRegBit(uint8_t ucReg, uint8_t ucPos, uint8_t ucState) {
	uint8_t ucRegValue;
	xSemaphoreTake(xCameraMutexSCCB, portMAX_DELAY);
	ucRegValue = prucCameraReadReg(ucReg);
	if (ucState == cameraON) {
		prvCameraWriteReg(ucReg, ucRegValue | 1 << ucPos);
	}

	else {
		prvCameraWriteReg(ucReg, ucRegValue & ~(1 << ucPos));
	}

	xSemaphoreGive(xCameraMutexSCCB);
}

/*
 * @brief: 	Sets a register to a certain value, overwriting the value before.
 * @param:	ucReg, register address.
 * 			ucValue, register value.
 */
void vCameraSetReg(uint8_t ucReg, uint8_t ucValue) {
	xSemaphoreTake(xCameraMutexSCCB, portMAX_DELAY);
	prvCameraWriteReg(ucReg, ucValue);
	xSemaphoreGive(xCameraMutexSCCB);
}

/******************************************************************************
 * CAMERA CONFIGURATION
 *****************************************************************************/
/*
 * @brief: 	Resets camera settings to fabric / default, and later
 * 			configures the QQVGA YUV mode.
 */
void vCameraReset(void) {
	xSemaphoreTake(xCameraMutexSCCB, portMAX_DELAY);
	vCameraDisablePCLKIRQ();
	vCameraDisableHREFIRQ();
	vCameraDisableVSYNCIRQ();
	prvCameraWriteReg(COM7, 1 << 7);
	// This delay is proposed by the OV7670's manufacturer (max 1ms).
	vTaskDelay(pdMS_TO_TICKS(1));
	xSemaphoreGive(xCameraMutexSCCB);
	vCameraQQVGA();
	vCameraEnableVSYNCIRQ();
}

/*
 * 	@brief	Checks if the SCCB connection is working by reading the
 * 			manufacturer's ID registers.
 * 	@retval pdPASS if the connection is established, pdFAIL in case of error.
 */
BaseType_t xCameraProbe(void) {
	xSemaphoreTake(xCameraMutexSCCB, portMAX_DELAY);
	if (prucCameraReadReg(MIDH) != 0x7F || prucCameraReadReg(MIDL) != 0xA2) {
		xSemaphoreGive(xCameraMutexSCCB);
		return pdFAIL;
	}

	else {
		xSemaphoreGive(xCameraMutexSCCB);
		return pdPASS;
	}
}



void vCameraQQVGA (void) {
	xSemaphoreTake(xCameraMutexSCCB, portMAX_DELAY);
	prvCameraWriteReg(CLKRC, 0x07);
	prvCameraWriteReg(COM7, 0x00);
	prvCameraWriteReg(COM3, 0x04);
	prvCameraWriteReg(COM14, 0x1A);
	prvCameraWriteReg(SCALING_XSC, 0x3A);
	prvCameraWriteReg(SCALING_YSC, 0x35);
	prvCameraWriteReg(SCALING_DCWCTR, 0x22);
	prvCameraWriteReg(SCALING_PCLK_DIV, 0xF2);
	prvCameraWriteReg(SCALING_PCLK_DELAY, 0x02);
	prvCameraWriteReg(0xb0, 0x84);
	xSemaphoreGive(xCameraMutexSCCB);
}

void vCameraQVGA(void) {
	xSemaphoreTake(xCameraMutexSCCB, portMAX_DELAY);
	prvCameraWriteReg(CLKRC, 0x0F);
	prvCameraWriteReg(COM7, 0x00);
	prvCameraWriteReg(COM3, 0x04);
	prvCameraWriteReg(COM14, 0x19);
	prvCameraWriteReg(SCALING_XSC, 0x3A);
	prvCameraWriteReg(SCALING_YSC, 0x35);
	prvCameraWriteReg(SCALING_DCWCTR, 0x11);
	prvCameraWriteReg(SCALING_PCLK_DIV, 0xF1);
	prvCameraWriteReg(SCALING_PCLK_DELAY, 0x02);
	prvCameraWriteReg(0xb0, 0x84);
	xSemaphoreGive(xCameraMutexSCCB);
}

/*
 * @brief: Select the output format for the image pixels.
 */
void vCameraFormat (uint8_t ucFormat) {
	uint8_t ucCom7, ucCom15;
	xSemaphoreTake(xCameraMutexSCCB, portMAX_DELAY);

	ucCom7 = prucCameraReadReg(COM7);
	ucCom7 &= 0xF8;
	ucCom15 = prucCameraReadReg(COM15);

	switch (ucFormat) {
		case cameraRAW_BAYER: {
			prvCameraWriteReg(COM7, ucCom7 | 0x01);
			prvCameraWriteReg(COM15, 0xC0 & ~(1 << 4) );
			break;
		}

		case cameraPROCESSED_BAYER: {
			prvCameraWriteReg(COM7, ucCom7 | 0x05);
			prvCameraWriteReg(COM15, 0xC0 & ~(1 << 4));
			break;
		}

		case cameraYUV: {
			prvCameraWriteReg(COM7, ucCom7 | 0x00);
			prvCameraWriteReg(COM15, 0xC0 & ~(1 << 4));
			break;
		}

		case cameraGRB: {
			prvCameraWriteReg(COM7, ucCom7 | 0x04);
			prvCameraWriteReg(COM15, 0xC0 & ~(1 << 4));
			break;
		}

		case cameraRGB565: {
			prvCameraWriteReg(COM7, ucCom7 | 0x04);
			prvCameraWriteReg(COM15, (ucCom15 & ~(1 << 5)) | (1 << 4) );
			break;
		}

		case cameraRGB555: {
			prvCameraWriteReg(COM7, ucCom7 | 0x04);
			prvCameraWriteReg(COM15, 0xC0 | (0x03 << 5));
			break;
		}
	}
	prvCameraWriteReg(SCALING_XSC, 0x3A);
	prvCameraWriteReg(SCALING_YSC, 0x35);

	xSemaphoreGive(xCameraMutexSCCB);
}

/******************************************************************************
 * RTOS TASK CONTROL
 *****************************************************************************/
/*
 * @brief	Initialization for the SCCB camera communication.
 */
void vCameraRegInit(void) {
	xCameraMutexSCCB = xSemaphoreCreateMutex();

	if (xCameraMutexSCCB == NULL) {
		#ifdef mainDEBUG
		vDebugErrorRoutine("SCCB Mutex Creation failed.");
		#endif
		vToolsErrorRoutine();
	}

	xSemaphoreGive(xCameraMutexSCCB);
}


/******************************************************************************
 * SCCB BIT BANGING
 *****************************************************************************/
#ifdef cameraBIT_BANG

/*
 * @brief	Changes the SDA GPIO to Output.
 */
static void prvSDAOutput(void) {
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = SDA_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/*
 * @brief	Changes the SDA GPIO to input.
 */
static void prvSDAInput(void) {
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = SDA_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(SDA_GPIO_Port, &GPIO_InitStruct);
}

/*
 * @brief: 	SCCB bit bang write operation. It writes 8 bits, and then ignores
 * 			the ninth one.
 */
static void prvWrite(uint8_t byte) {
	uint8_t i;
	prvSDAOutput();
	vDelayUs(10);
	for (i=0; i < 8; i++) {
		HAL_GPIO_WritePin(GPIOB, SCLK_Pin, GPIO_PIN_RESET);
		vDelayUs(20);
		HAL_GPIO_WritePin(GPIOB, SDA_Pin, byte & (0x01 << (7 - i)));
		vDelayUs(20);
		HAL_GPIO_WritePin(GPIOB, SCLK_Pin, GPIO_PIN_SET);
		vDelayUs(20);
	}

	//ACK ignore
	HAL_GPIO_WritePin(GPIOB, SCLK_Pin, GPIO_PIN_RESET);
	vDelayUs(10);
	HAL_GPIO_WritePin(GPIOB, SDA_Pin, GPIO_PIN_SET);
	vDelayUs(10);
	HAL_GPIO_WritePin(GPIOB, SCLK_Pin, GPIO_PIN_SET);
	vDelayUs(20);
	HAL_GPIO_WritePin(GPIOB, SCLK_Pin, GPIO_PIN_RESET);
	vDelayUs(20);
}

/*
 * @brief	SCCB bit bang start condition. SCLK and SDA both high, then
 * 			SDA goes LOW, and then SCLK goes LOW.
 */
static void prvStart(void) {
	prvSDAOutput();

	HAL_GPIO_WritePin(GPIOB, SCLK_Pin, GPIO_PIN_SET);

	vDelayUs(10);

	HAL_GPIO_WritePin(GPIOB, SDA_Pin, GPIO_PIN_RESET);

	vDelayUs(10);

	HAL_GPIO_WritePin(GPIOB, SCLK_Pin, GPIO_PIN_RESET);
}

/*
 * @brief	SCCB bit bang Stop condition. SCLK and SDA both low. Then, SCLK
 * 			raises to HIGH, followed by SDA going HIGH.
 */
static void prvStop(void) {
	prvSDAOutput();
	HAL_GPIO_WritePin(GPIOB, SDA_Pin, GPIO_PIN_RESET);
	vDelayUs(20);

	HAL_GPIO_WritePin(GPIOB, SCLK_Pin, GPIO_PIN_SET);

	vDelayUs(20);

	HAL_GPIO_WritePin(GPIOB, SDA_Pin, GPIO_PIN_SET);
}

/*
 * @brief	SCCB bit bang Read operation. 8 Bits are read, followed by a
 * 			NACK response in the ninth bit.
 */
static uint8_t prucRead(void) {
	uint8_t ucByte = 0x00;
	uint8_t i;
	prvSDAInput();
	vDelayUs(10);

	for (i=0; i < 8; i++) {
		HAL_GPIO_WritePin(GPIOB, SCLK_Pin, GPIO_PIN_RESET);
		vDelayUs(20);
		HAL_GPIO_WritePin(GPIOB, SCLK_Pin, GPIO_PIN_SET);
		vDelayUs(10);
		ucByte |= HAL_GPIO_ReadPin(GPIOB, SDA_Pin) << (7 - i);
		vDelayUs(10);
	}

	HAL_GPIO_WritePin(GPIOB, SCLK_Pin, GPIO_PIN_RESET);
	prvSDAOutput();
	vDelayUs(10);
	HAL_GPIO_WritePin(GPIOB, SDA_Pin, GPIO_PIN_SET);
	vDelayUs(10);
	HAL_GPIO_WritePin(GPIOB, SCLK_Pin, GPIO_PIN_SET);
	vDelayUs(20);
	HAL_GPIO_WritePin(GPIOB, SCLK_Pin, GPIO_PIN_RESET);
	vDelayUs(20);

	return ucByte;
}
#endif

/******************************************************************************
 * READING AND WIRTING REGISTERS
 *****************************************************************************/
#ifdef cameraBIT_BANG
/*
 * @brief	Write a register in the OV7670 camera. It's a 3 part process.
 * 			1. Write the address of the device.
 * 			2. Write the address of the register.
 * 			3. Write the value of the register.
 * 	@param	ucReg, register address.
 * 	@param	ucData, value of the register.
 */
static void prvCameraWriteReg (uint8_t ucReg, uint8_t ucData) {
	prvStart();
	prvWrite(cameraWRITE_ADDRESS);
	prvWrite(ucReg);
	prvWrite(ucData);
	prvStop();
}

/*
 * 	@brief	Reads a register from the camera. It's a 5 part process:
 * 			1. Write the address of the camera.
 * 			2. Write the address of the register.
 * 			3. Restart communication.
 * 			4. Write the address of the camera for reading.
 * 			5. Read the register's value.
 * 	@param	ucReg, register to be read from.
 * 	@retval Register's value.
 */
static uint8_t prucCameraReadReg (uint8_t ucReg) {
	uint8_t ucData = 0x00;

	prvStart();
	prvWrite(cameraWRITE_ADDRESS);
	prvWrite(ucReg);
	prvStop();
	prvStart();
	prvWrite(cameraREAD_ADDRESS);;
	ucData = prucRead();
	prvStop();
	return ucData;
}

#else
static void prvCameraWriteReg (uint8_t ucReg, uint8_t ucData) {
	LL_I2C_GenerateStartCondition(I2C1);

	while(!LL_I2C_IsActiveFlag_SB(I2C1));

	LL_I2C_TransmitData8(I2C1, cameraWRITE_ADDRESS);

	while(!LL_I2C_IsActiveFlag_ADDR(I2C1));
	LL_I2C_ClearFlag_ADDR(I2C1);
	LL_I2C_TransmitData8(I2C1, ucReg);

	while(!LL_I2C_IsActiveFlag_AF(I2C1) && !LL_I2C_IsActiveFlag_TXE(I2C1));
	LL_I2C_ClearFlag_AF(I2C1);
	LL_I2C_TransmitData8(I2C1, ucData);

	while(!LL_I2C_IsActiveFlag_AF(I2C1) && !LL_I2C_IsActiveFlag_TXE(I2C1) );
	LL_I2C_ClearFlag_AF(I2C1);
	LL_I2C_GenerateStopCondition(I2C1);
}

static uint8_t prucCameraReadReg (uint8_t ucReg) {

	uint8_t ucData = 0x00;

	LL_I2C_GenerateStartCondition(I2C1);

	while(!LL_I2C_IsActiveFlag_SB(I2C1));

	LL_I2C_TransmitData8(I2C1, cameraWRITE_ADDRESS);

	while(!LL_I2C_IsActiveFlag_ADDR(I2C1) && !LL_I2C_IsActiveFlag_AF(I2C1));
	LL_I2C_ClearFlag_ADDR(I2C1);
	LL_I2C_ClearFlag_AF(I2C1);
	LL_I2C_TransmitData8(I2C1, ucReg);

	while(!LL_I2C_IsActiveFlag_AF(I2C1) && !LL_I2C_IsActiveFlag_TXE(I2C1));
	LL_I2C_ClearFlag_AF(I2C1);
	LL_I2C_GenerateStopCondition(I2C1);

	LL_I2C_GenerateStartCondition(I2C1);

	while(!LL_I2C_IsActiveFlag_SB(I2C1));
	LL_I2C_TransmitData8(I2C1, cameraREAD_ADDRESS);
	LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_NACK);

	while(!LL_I2C_IsActiveFlag_ADDR(I2C1) && !LL_I2C_IsActiveFlag_AF(I2C1));
	LL_I2C_ClearFlag_ADDR(I2C1);
	LL_I2C_ClearFlag_AF(I2C1);

	while(!LL_I2C_IsActiveFlag_RXNE(I2C1));

	ucData = LL_I2C_ReceiveData8(I2C1);
	LL_I2C_GenerateStopCondition(I2C1);

	return ucData;
}
#endif

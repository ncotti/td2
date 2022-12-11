#include "camera_api.h"

/******************************************************************************
 * TESTING
 *****************************************************************************/
void vCameraTestPatternOn(void) {vCameraChangeRegBit(SCALING_YSC, 7, cameraON);}

void vCameraTestPatternOff(void) {vCameraChangeRegBit(SCALING_YSC, 7, cameraOFF);}

void vCameraColorBarOn(void) {vCameraChangeRegBit(COM7, 1, cameraON);}

void vCameraColorBarOff(void) {vCameraChangeRegBit(COM7, 1, cameraOFF);}


/******************************************************************************
 * CONTROL
 *****************************************************************************/
void vCameraHMirrorOn(void) {vCameraChangeRegBit(MVFP, 5, cameraON);}
void vCameraHMirrorOff(void) {vCameraChangeRegBit(MVFP, 5, cameraOFF);}

void vCameraVFlipOn(void) {vCameraChangeRegBit(MVFP, 4, cameraON);}
void vCameraVFlipOff(void) {vCameraChangeRegBit(MVFP, 4, cameraOFF);}

void vCameraFreezeAGCAEC(void) {vCameraChangeRegBit(COM9, 0, cameraON);}
void vCameraUnfreezeAGCAEC(void) {vCameraChangeRegBit(COM9, 0, cameraOFF);}

void vCameraManualAGCAEC(void) {
	vCameraChangeRegBit(COM8, 0, cameraOFF);
	vCameraChangeRegBit(COM8, 2, cameraOFF);
}

void vCameraAutoAGCAEC(void) {
	vCameraChangeRegBit(COM8, 0, cameraON);
	vCameraChangeRegBit(COM8, 2, cameraON);
}

void vCameraRGB565(void) {
	vCameraChangeRegBit(COM7, 0, cameraOFF);
	vCameraChangeRegBit(COM7, 2, cameraON);
	vCameraChangeRegBit(COM15, 4, cameraON);
	vCameraChangeRegBit(COM15, 5, cameraOFF);
}

void vCameraYUV(void) {
	vCameraChangeRegBit(COM7, 0, cameraOFF);
	vCameraChangeRegBit(COM7, 2, cameraOFF);
	vCameraChangeRegBit(COM15, 4, cameraOFF);
}


void vCameraStandByOn(void) {vCameraChangeRegBit(COM2, 4, cameraON);}
void vCameraStandByOff(void) {vCameraChangeRegBit(COM2, 4, cameraOFF);}


/******************************************************************************
 * AWB (Automatic White Balance)
 *****************************************************************************/
void vCameraAWBB1(void) {
	vCameraChangeRegBit(GFIX, 0, cameraOFF);
	vCameraChangeRegBit(GFIX, 1, cameraOFF);
}

void vCameraAWBB125(void) {
	vCameraChangeRegBit(GFIX, 0, cameraON);
	vCameraChangeRegBit(GFIX, 1, cameraOFF);
}

void vCameraAWBB15(void) {
	vCameraChangeRegBit(GFIX, 0, cameraOFF);
	vCameraChangeRegBit(GFIX, 1, cameraON);
}

void vCameraAWBB175(void) {
	vCameraChangeRegBit(GFIX, 0, cameraON);
	vCameraChangeRegBit(GFIX, 1, cameraON);
}

void vCameraAWBR1(void) {
	vCameraChangeRegBit(GFIX, 2, cameraOFF);
	vCameraChangeRegBit(GFIX, 3, cameraOFF);
}

void vCameraAWBR125(void) {
	vCameraChangeRegBit(GFIX, 2, cameraON);
	vCameraChangeRegBit(GFIX, 3, cameraOFF);
}

void vCameraAWBR15(void) {
	vCameraChangeRegBit(GFIX, 2, cameraOFF);
	vCameraChangeRegBit(GFIX, 3, cameraON);
}

void vCameraAWBR175(void) {
	vCameraChangeRegBit(GFIX, 2, cameraON);
	vCameraChangeRegBit(GFIX, 3, cameraON);
}

void vCameraAWBGB1(void) {
	vCameraChangeRegBit(GFIX, 4, cameraOFF);
	vCameraChangeRegBit(GFIX, 5, cameraOFF);
}

void vCameraAWBGB125(void) {
	vCameraChangeRegBit(GFIX, 4, cameraON);
	vCameraChangeRegBit(GFIX, 5, cameraOFF);
}

void vCameraAWBGB15(void) {
	vCameraChangeRegBit(GFIX, 4, cameraOFF);
	vCameraChangeRegBit(GFIX, 5, cameraON);
}

void vCameraAWBGB175(void) {
	vCameraChangeRegBit(GFIX, 4, cameraON);
	vCameraChangeRegBit(GFIX, 5, cameraON);
}

void vCameraAWBGR1(void) {
	vCameraChangeRegBit(GFIX, 6, cameraOFF);
	vCameraChangeRegBit(GFIX, 7, cameraOFF);
}

void vCameraAWBGR125(void) {
	vCameraChangeRegBit(GFIX, 6, cameraON);
	vCameraChangeRegBit(GFIX, 7, cameraOFF);
}

void vCameraAWBGR15(void) {
	vCameraChangeRegBit(GFIX, 6, cameraOFF);
	vCameraChangeRegBit(GFIX, 7, cameraON);
}

void vCameraAWBGR175(void) {
	vCameraChangeRegBit(GFIX, 6, cameraON);
	vCameraChangeRegBit(GFIX, 7, cameraON);
}


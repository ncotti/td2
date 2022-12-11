#ifndef INC_CAMERA_API_H_
#define INC_CAMERA_API_H_

#include "camera_reg.h"

/******************************************************************************
 * TESTING
 *****************************************************************************/
void vCameraTestPatternOn(void);
void vCameraTestPatternOff(void);
void vCameraColorBarOn(void);
void vCameraColorBarOff(void);

/******************************************************************************
 * CONTROL
 *****************************************************************************/
void vCameraHMirrorOn(void);
void vCameraHMirrorOff(void);

void vCameraVFlipOn(void);
void vCameraVFlipOff(void);

void vCameraFreezeAGCAEC(void);
void vCameraUnfreezeAGCAEC(void);

void vCameraManualAGCAEC(void);
void vCameraAutoAGCAEC(void);

void vCameraRGB565(void);
void vCameraYUV(void);

void vCameraStandByOn(void);
void vCameraStandByOff(void);

/******************************************************************************
 * AWB (Automatic White Balance)
 *****************************************************************************/
void vCameraAWBR1(void);
void vCameraAWBR125(void);
void vCameraAWBR15(void);
void vCameraAWBR175(void);

void vCameraAWBB1(void);
void vCameraAWBB125(void);
void vCameraAWBB15(void);
void vCameraAWBB175(void);

void vCameraAWBGB1(void);
void vCameraAWBGB125(void);
void vCameraAWBGB15(void);
void vCameraAWBGB175(void);

void vCameraAWBGR1(void);
void vCameraAWBGR125(void);
void vCameraAWBGR15(void);
void vCameraAWBGR175(void);




#endif /* INC_CAMERA_API_H_ */

#ifndef INC_CAMERA_REG_H_
#define INC_CAMERA_REG_H_

/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include "main.h"
#include "tools.h"

/******************************************************************************
 * DEFINES
 *****************************************************************************/
#define cameraWRITE_ADDRESS 0x42	//I2C Slave Address for writing (includes 8th Write bit)
#define cameraREAD_ADDRESS 	0x43	//I2C Slave Address for reading (includes 8th Read bit)

// Camera modes
#define cameraRAW_BAYER 0
#define cameraPROCESSED_BAYER 1
#define cameraYUV 2
#define cameraGRB 3
#define cameraRGB565 4
#define cameraRGB555 5

// Comment to use Hardware I2C (must be configured for I2C1, with LL library).
#define cameraBIT_BANG
#define cameraON 1
#define cameraOFF 0

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/
void vCameraRegInit(void);
void vCameraTaskSCCB(void* pvParameters);
void vCameraReset(void);
BaseType_t xCameraProbe(void);
void vCameraChangeRegBit(uint8_t ucReg, uint8_t ucPos, uint8_t ucState);
void vCameraSetReg(uint8_t ucReg, uint8_t ucValue);
void vCameraQQVGA(void);
void vCameraQVGA(void);



/******************************************************************************
 * CAMERA REGISTERS
 *****************************************************************************/
#define GAIN 				0x00
#define BLUE 				0x01
#define RED 				0x02
#define VREF 				0x03
#define COM1				0x04
#define BAVE 				0x05
#define GbAVE 				0x06
#define AECHH 				0x07
#define RAVE 				0x08
#define COM2 				0x09
#define PID 				0x0A
#define VER 				0x0B
#define COM3 				0x0C
#define COM4 				0x0D
#define COM6 				0x0F
#define AECH 				0x10
#define CLKRC 				0x11
#define COM7 				0x12
#define COM8 				0x13
#define COM9 				0x14
#define COM10 				0x15
#define HSTART 				0x17
#define HSTOP 				0x18
#define VSTRT 				0x19
#define VSTOP 				0x1A
#define PSHFT 				0x1B
#define MIDH 				0x1C
#define MIDL 				0x1D
#define MVFP 				0x1E
#define ADCCTR0 			0x20
#define AEW 				0x24
#define AEB 				0x25
#define VPT 				0x26
#define BBIAS 				0x27
#define GbBIAS 				0x28
#define RSVD 				0x29
#define EXHCH 				0x2A
#define EXHCL 				0x2B
#define RBIAS 				0x2C
#define ADVFL 				0x2D
#define ADVFH 				0x2E
#define YAVE 				0x2F
#define HSYST 				0x30
#define HSYEN 				0x31
#define HREF 				0x32
#define TSLB 				0x3A
#define COM11 				0x3B
#define COM12 				0x3C
#define COM13 				0x3D
#define COM14 				0x3E
#define EDGE 				0x3F
#define COM15 				0x40
#define COM16 				0x41
#define COM17 				0x42
#define REG4B 				0x4B
#define DNSTH 				0x4C
#define MTX1 				0x4F
#define MTX2 				0x50
#define MTX3 				0x51
#define MTX4 				0x52
#define MTX5 				0x53
#define MTX6 				0x54
#define BRIGHT 				0x55
#define CONTRAS 			0x56
#define CONTRAS_CENTER 		0x57
#define MTXS 				0x58
#define LCC1 				0x62
#define LCC2 				0x63
#define LCC3 				0x64
#define LCC4 				0x65
#define LCC5 				0x66
#define MANU 				0x67
#define MANV 				0x68
#define GFIX 				0x69
#define GGAIN 				0x6A
#define DBLV 				0x6B
#define AWBCTR3 			0x6C
#define AWBCTR2 			0x6D
#define AWBCTR1 			0x6E
#define AWBCTR0 			0x6F
#define SCALING_XSC 		0x70
#define SCALING_YSC 		0x71
#define SCALING_DCWCTR 		0x72
#define SCALING_PCLK_DIV 	0x73
#define REG74 				0x74
#define REG75				0x75
#define REG76 				0x76
#define REG77 				0x77
#define SLOP 				0x7A
#define GAM1			 	0x7B
#define GAM2 				0x7C
#define GAM3 				0x7D
#define GAM4 				0x7E
#define GAM5 				0x7F
#define GAM6 				0x80
#define GAM7 				0x81
#define GAM8 				0x82
#define GAM9 				0x83
#define GAM10 				0x84
#define GAM11 				0x85
#define GAM12 				0x86
#define GAM13 				0x87
#define GAM14 				0x88
#define GAM15 				0x89
#define RGB444 				0x8C
#define DM_LNL 				0x92
#define DM_LNH 				0x93
#define LCC6 				0x94
#define LCC7 				0x95
#define BD50ST 				0x9D
#define BD60ST 				0x9E
#define HAECC1 				0x9F
#define HAECC2 				0xA0
#define SCALING_PCLK_DELAY 	0xA2
#define NT_CTRL 			0xA4
#define BD50MAX 			0xA5
#define HAECC3 				0xA6
#define HAECC4 				0xA7
#define HAECC5 				0xA8
#define HAECC6 				0xA9
#define HAECC7 				0xAA
#define BD60MAX 			0xAB
#define STR_OPT 			0xAC
#define STR_R 				0xAD
#define STR_G 				0xAE
#define STR_B 				0xAF
#define ABLC1 				0xB1
#define THL_ST 				0xB3
#define THL_DLT 			0xB5
#define AD_CHB 				0xBE
#define AD_CHR 				0xBF
#define AD_CHGb 			0xC0
#define AD_CHGr 			0xC
#define SATCTR 				0xC9



#endif /* INC_CAMERA_REG_H_ */

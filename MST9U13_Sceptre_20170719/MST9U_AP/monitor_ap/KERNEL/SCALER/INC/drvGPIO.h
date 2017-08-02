///////////////////////////////////////////////////////////////////////////////
/// @file drvScalerGPIO.h
/// @brief Scaler GPIO/PWM access
/// @author MStarSemi Inc.
///
/// Driver for Scaler GPIO/PWM access.
///
/// Features
///  -
///  -
///////////////////////////////////////////////////////////////////////////////

#ifndef _DRVGPIO_H
#define _DRVGPIO_H

#include "GPIO_DEF.h"
#define GPIO_DEBUG    1

/**************************************************
* Modified for CHIP_FAMILY_TSUMC PMW driver
*
* Josh 2012/07/11
**************************************************/
typedef enum
{
    _PWM0_,
    _PWM1_,
    _PWM2_,
    _PWM3_,
    _PWM4_,
    _PWM5_,
    _PWM6_,
    _PWM7_,
    _PWM8_, /* PWM8 of CHIP_FAMILY_TSUMC - Josh 2012/07/11 */
    _PWM9_, /* PWM9 of CHIP_FAMILY_TSUMC - Josh 2012/07/11 */
    _DISP_PWM0_,
    _DISP_PWM1_,
    _DISP_PWM2_,
    _DISP_PWM3_,
    _DISP_PWM4_,
    _DISP_PWM5_,
    _NOUSE_,
} PWMNoType;

#define _NO_USE_  0
#define _PWM0_GP0_  BIT0
#define _PWM1_GP0_  BIT1
#define _PWM2_GP0_  BIT2
#define _PWM3_GP0_  BIT3
#define _PWM4_GP0_  BIT4
#define _PWM5_GP0_  BIT5
#define _PWM6_GP0_  BIT6
#define _PWM7_GP0_  BIT7
#define _PWM8_GP0_  BIT8
#define _PWM9_GP0_  BIT9
#define _PWM0_GP1_  BIT10
#define _PWM1_GP1_  BIT11
#define _PWM2_GP1_  BIT12
#define _PWM3_GP1_  BIT13
#define _PWM4_GP1_  BIT14
#define _PWM5_GP1_  BIT15
#define _PWM6_GP1_  BIT16
#define _PWM7_GP1_  BIT17
#define _PWM8_GP1_  BIT18
#define _PWM9_GP1_  BIT19
#define _DISP_PWM0_GP0_  BIT20
#define _DISP_PWM1_GP0_  BIT21
#define _DISP_PWM2_GP0_  BIT22
#define _DISP_PWM3_GP0_  BIT23
#define _DISP_PWM4_GP0_  BIT24
#define _DISP_PWM5_GP0_  BIT25
#define _DISP_PWM0_GP1_  BIT26
#define _DISP_PWM1_GP1_  BIT27
#define _DISP_PWM2_GP1_  BIT28
#define _DISP_PWM3_GP1_  BIT29
#define _DISP_PWM4_GP1_  BIT30
#define _DISP_PWM5_GP1_  BIT31
//PWM Config
#define _PWM_DB_EN_ BIT0
#define _PWM_DB_VSYNC_MODE_ BIT1//0:vsync double buffer,1:double buffer
#define _PWM_VSYNC_ALIGN_EN_ BIT2
#define _PWM_HSYNC_ALIGN_EN_ BIT3
#define _PWM_POLARITY_EN_ BIT4
#define _PWM_OD_EN_ BIT5

typedef enum //Mike 110318 //Sky110406
{
    _LED_PUSH_PULL_,
    _LED_OPEN_DRAIN_,
} PWMPadControlType;

#if 0
#define LINEAR_MAPPING_VALUE(orgNow,orgMin,orgMax,newMin,newMax) \
    ((((DWORD)((orgNow)-(orgMin)))*((newMax)-(newMin))+(((orgMax)-(orgMin))>>1))/((orgMax)-(orgMin))+(newMin))
#endif

#ifdef _MSOSD_C
#define INTERFACE
#else
#define INTERFACE   extern
#endif

#if 0//(USE_SCALER_GPIO)
INTERFACE void drvGPIO_SetScalerGPIO_OEnable(BYTE u8GPIO,Bool Enable);
INTERFACE void drvGPIO_SetScalerGPIO(BYTE u8GPIO);
INTERFACE void drvGPIO_ClrScalerGPIO(BYTE u8GPIO);
INTERFACE BYTE drvGPIO_GetScalerGPIO(BYTE u8GPIO);
#endif
INTERFACE void msDrvPWMAlignSync(DWORD u32PWMCh,BYTE u8Config, BYTE u8Counter);
INTERFACE void msDrvPWMFreq(DWORD u32PWMCh, DWORD u32PWMFreq);
INTERFACE void msDrvPWMPolarity(DWORD u32PWMCh, BYTE u8Config);
INTERFACE void msDrvPWMShift(DWORD u32PWMCh, DWORD u32PWMShift);
//INTERFACE void drvGPIO_SetPWMDuty(BYTE u8PWMCh,DWORD u32Duty);
INTERFACE void msAPIPWMDuty(DWORD u32PWMCh,WORD u8ByteDuty);
INTERFACE void msAPIPWMConfig(DWORD u32PWMCh, DWORD u32Freq, WORD u16Duty, BYTE u8Config, BYTE u8Counter);
INTERFACE void msDrvPWMEnableSwitch(DWORD u32PWMCh, Bool bEnable);

#undef INTERFACE
#endif //_DRVGPIO_H

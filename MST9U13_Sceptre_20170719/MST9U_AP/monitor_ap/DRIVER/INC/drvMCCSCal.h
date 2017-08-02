#ifndef _DRVMCCSCAL_H
#define _DRVMCCSCAL_H

#include "Global.h"

//==========DELL MCCS COMMAND====================================
#define MCCS_COLORAP_COMMAND_READ       0xF1
#define MCCS_COLORAP_COMMAND_WRITE      0xF0
#define MCCS_COLORAP_SaveAdjustment     0x02
#define MCCS_COLORAP_RestoreFactory     0x04
#define MCCS_COLORAP_Bypass             0x08
#define MCCS_COLORAP_Brightness         0x10
#define MCCS_COLORAP_Contrast           0x12
#define MCCS_COLORAP_Color_Preset       0x14
#define MCCS_COLORAP_RGB_Gain           0x16
#define MCCS_COLORAP_RGB_Offset         0x17
#define MCCS_COLORAP_Color_Matrix       0x18
#define MCCS_COLORAP_Post_Gamma         0x30
#define MCCS_COLORAP_Fix_Gamma          0x31
#define MCCS_COLORAP_Pre_Gamma          0x32
#define MCCS_COLORAP_Gamma_CheckSum     0x34
#define MCCS_COLORAP_ResetPostGamma     0x35
#define MCCS_COLORAP_ResetPreGamma      0x36
#define MCCS_COLORAP_ResetFixGamma      0x37
#define MCCS_COLORAP_Saturation         0x8A
#define MCCS_COLORAP_Hue                0x90
#define MCCS_COLORAP_6axis_Saturation   0x9A
#define MCCS_COLORAP_6axis_Hue          0x9B
#define MCCS_COLORAP_6axis_Brightness   0x9C
#define MCCS_COLORAP_DisableOSD         0xB0
#define MCCS_COLORAP_EnableOSD          0xB1
#define MCCS_COLORAP_ToggleInternalPattern   0xB2
#define MCCS_COLORAP_InternalPattern         0xB3
#define MCCS_COLORAP_ToggleXVYCCPATTERN      0xB4
#define MCCS_COLORAP_XVYCCPATTERN            0xB5


static const int _maxEntrieBytes= 2048;
#define NumOf3rdPartyColorEngine    13
#define NUM_OF_USER_CALIBRATION_BLOCK   2

struct MCCS_PARAMETER{
    BYTE  bPostGammaModified;
    BYTE  bPreGammaModified;
    BYTE  bFixGammaModified;
    BYTE  bSixAxisHueModified;
    BYTE  bSixAxisSatModified;
    BYTE  bSixAxisBriModified;
    BYTE  bRGBGainModified;
    BYTE  bRGBOffsetModified;
    BYTE  bColorMatrixModified;
    BYTE  bBrightnessModified;
    BYTE  bContrastModified;
    BYTE  bSaturationModified;
    BYTE  bHueModified;
    BYTE  u8ModifiedFlagCheckSum;

    BYTE  u8PreGamma[3][2048];
    BYTE  u8FixGamma[3][512];
    BYTE  u8PostGamma[3][2048];
    BYTE  u8SixAxisHue[6];
    BYTE  u8SixAxisSat[6];
    BYTE  u8SixAxisBri[6];
    BYTE  u8RGBGain[3];
    BYTE  u8RGBOffset[3];
    short  ColorMatrix[9];
    BYTE  u8Brightness;
    BYTE  u8Contrast;
    BYTE  u8Saturation;
    BYTE  u8Hue;
    BYTE  u8PresetNumber;
};

extern void msBypassMCCSColorEngine(void);
extern BYTE MCCSCalibrationHandler(BYTE CommandCode);

#if ENABLE_SaveMCCSData
#define sizeOfMCCSDataBlock sizeof(struct MCCS_PARAMETER)*NUM_OF_USER_CALIBRATION_BLOCK
extern BOOL msLoadMCCSColorData(BYTE CSIndex);
extern void  msSaveMCCSColorData(BYTE CSIndex);
extern void  msDiscardMCCSColorData(BYTE CSIndex);
extern void msApplyMCCSColorData(void);
#endif
#endif

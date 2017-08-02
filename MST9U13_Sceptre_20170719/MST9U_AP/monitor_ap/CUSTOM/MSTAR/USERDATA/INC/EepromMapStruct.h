///////////////////////////////////////////////////////////////////////////////
/// @file DemoMap.h
/// @brief Head file to define the structure of user preference data.
/// @author MStarSemi Inc.
///
///
///
/// Features
///  -
///  -
///  -
///  -
///////////////////////////////////////////////////////////////////////////////

#ifndef _EepromMapStruct_H_
#define _EepromMapStruct_H_
#include "msScaler.h"
#include "EepromMap.h"
//#include "Mode.h"


// eEEP_FactoryBlock
typedef struct
{
    BYTE u8FAC_EepromChksum;
	
    BYTE u8FAC_SrgbDefault;

    WORD u16FAC_AdcGainR;
    WORD u16FAC_AdcGainG;
    WORD u16FAC_AdcGainB;

    BYTE u8FAC_AdcPgaGain;

    BYTE u8FAC_AdcOffsetR;
    BYTE u8FAC_AdcOffsetG;
    BYTE u8FAC_AdcOffsetB;

    BYTE u8FAC_ColorSrgb_R;
    BYTE u8FAC_ColorSrgb_G;
    BYTE u8FAC_ColorSrgb_B;
    BYTE u8FAC_ColorSrgb_Bri;
    BYTE u8FAC_ColorSrgb_Con;

    BYTE u8FAC_5000K_R;
    BYTE u8FAC_5000K_G;
    BYTE u8FAC_5000K_B;
    BYTE u8FAC_5000K_Bri;
    BYTE u8FAC_5000K_Con;

    BYTE u8FAC_5700K_R;
    BYTE u8FAC_5700K_G;
    BYTE u8FAC_5700K_B;
    BYTE u8FAC_5700K_Bri;
    BYTE u8FAC_5700K_Con;

    BYTE u8FAC_6500K_R;
    BYTE u8FAC_6500K_G;
    BYTE u8FAC_6500K_B;
    BYTE u8FAC_6500K_Bri;
    BYTE u8FAC_6500K_Con;

    BYTE u8FAC_7500K_R;
    BYTE u8FAC_7500K_G;
    BYTE u8FAC_7500K_B;
    BYTE u8FAC_7500K_Bri;
    BYTE u8FAC_7500K_Con;

    BYTE u8FAC_9300K_R;
    BYTE u8FAC_9300K_G;
    BYTE u8FAC_9300K_B;
    BYTE u8FAC_9300K_Bri;
    BYTE u8FAC_9300K_Con;

    BYTE u8FAC_10000K_R;
    BYTE u8FAC_10000K_G;
    BYTE u8FAC_10000K_B;
    BYTE u8FAC_10000K_Bri;
    BYTE u8FAC_10000K_Con;

    BYTE u8FAC_FacBrightness;
    BYTE u8FAC_FacContrast;

    BYTE u8FAC_SSC_Frequency;
    BYTE u8FAC_SSC_Permillage;
	
    BYTE u8FAC_DDR_SSC_Frequency;
    BYTE u8FAC_DDR_SSC_Permillage;


    BYTE u8FAC_Padding[3];
}__attribute__((packed)) ST_EEP_FACTORY;

typedef struct
{
    WORD u16X;
    WORD u16Y;
}__attribute__((packed)) ST_EEP_POS_XY;

// eEEP_SystemBlock
typedef struct
{
    WORD u16SYS_SystemFlag;
    WORD u16SYS_Misc;
    WORD u16SYS_DDCFlag;
    BYTE u8SYS_CtrlHPD;
    BYTE u8SYS_AutoDetect;
    BYTE u8SYS_WideMode[PORT_CNT];         //Support max 8 input port
    BYTE u8SYS_DLC;
#if ENABLE_MULTI_WINDOW_SETTINGS_MENU	
    BYTE u8SYS_MltWinOnOff;
#endif
    BYTE u8SYS_MltWin_SEL;
    BYTE u8SYS_MltWinSRC[WIND_CNT];        //Support max 4 output display windows
#if ENABLE_MULTI_WINDOW_SETTINGS_MENU	
    BYTE u8SYS_MltWinSize[WIND_CNT];       //Support max 4 output display windows
    BYTE u8SYS_MltWinBorderColor;
    BYTE u8SYS_PIP_Possition;
	
    ST_EEP_POS_XY MltWinPos[WIND_CNT];     //Support max 4 output display windows
    BYTE u8SYS_MltWinRotate[WIND_CNT];     //Support max 4 output display windows
#endif    
    BYTE u8SYS_Language;
    BYTE u8SYS_MenuTranspareny;
    BYTE u8SYS_DPConfigType;
#if HDMI_VerSion_Enable	
    BYTE u8SYS_HDMIConfigType;
#endif
#if Enable_Menu_Panel_Rotate_180
    BYTE u8SYS_RotateIndex;
#endif
    BYTE u8SYS_OsdRotation;
    BYTE u8SYS_DDCCI;
    BYTE u8SYS_OSDHPos;
    BYTE u8SYS_OSDVPos;
    BYTE u8SYS_OSDTime;
    BYTE u8SYS_OSDLock;
    BYTE u8SYS_OSDPivot;
    BYTE u8SYS_SleepMode;
    BYTE u8SYS_LosMode;
    BYTE u8SYS_LedMode;
    BYTE u8SYS_LEDColorfulMode;
    BYTE u8SYS_Hot_PICTURE_INDEX;
	
    BYTE u8SYS_LedStrength;
    BYTE u8SYS_LedModeType;

    BYTE u8SYS_LedModeIndex;

    BYTE u8SYS_PictureIndex;
		
    BYTE u8SYS_Volume;
    BYTE u8SYS_AudioSRC[PORT_CNT];
//    BYTE u8SYS_AudioMuteStatus;
    BYTE u8SYS_AudioConfig;
    BYTE u8SYS_AudioChannelOut;
#if ENABLE_ADJ_AUDIO_OUTPUT	
    BYTE u8SYS_AudioOutputType;
#endif	
    BYTE u8SYS_OffTimer;
    BYTE u8SYS_OverScan[WIND_CNT];        //Support max 4 output display windows
    BYTE u8SYS_ResponseTime;
    DWORD u32SYS_BacklightTime;
	
    BYTE u8SYS_rgbBrightness;
    BYTE u8SYS_BackLight;      //Support max 8 input port
    BYTE u8SYS_BackLight_User;      //Support max 8 input port
    
    BYTE u8SYS_DeMuraMeasureCommand;
    BYTE u8SYS_Preset_Mode_Eeprom_Index;
    BYTE u8SYS_User_Mode_Eeprom_Index;
    BYTE u8SYS_Padding[WIND_CNT];
}__attribute__((packed)) ST_EEP_SYSTEM;

// eEEP_ColorParameterBlock
typedef struct
{
    BYTE u8COLOR_Contrast;
    BYTE u8COLOR_Contrast_User;
    BYTE u8COLOR_InputColorFormat;
    BYTE u8COLOR_InputColorRange;
    BYTE u8COLOR_Gamma;

    BYTE u8COLOR_Saturature;
    BYTE u8COLOR_Hue;
    BYTE u8COLOR_ColorTemp;
    BYTE u8COLOR_PictureMode;
	
    BYTE u8COLOR_Sharpness;
    BYTE u8COLOR_LowBlueLight;

    BYTE u8COLOR_RedColor;
    BYTE u8COLOR_GreenColor;
    BYTE u8COLOR_BlueColor;
	
    BYTE u8COLOR_RedBlackLevel;
    BYTE u8COLOR_BlueBlackLevel;
    BYTE u8COLOR_GreenBlackLevel;

    BYTE u8COLOR_R_Hue;
    BYTE u8COLOR_G_Hue;
    BYTE u8COLOR_B_Hue;
    BYTE u8COLOR_C_Hue;
    BYTE u8COLOR_M_Hue;
    BYTE u8COLOR_Y_Hue;
    BYTE u8COLOR_R_Saturation;
    BYTE u8COLOR_G_Saturation;
    BYTE u8COLOR_B_Saturation;
    BYTE u8COLOR_C_Saturation;
    BYTE u8COLOR_M_Saturation;
    BYTE u8COLOR_Y_Saturation;
	
    BYTE u8COLOR_R;
    BYTE u8COLOR_G;
    BYTE u8COLOR_B;
    BYTE u8COLOR_C;
    BYTE u8COLOR_M;
    BYTE u8COLOR_Y;

    BYTE u8COLOR_SuperResolution;
    BYTE u8COLOR_NoiseReduction;
}__attribute__((packed)) ST_EEP_COLOR;

// eEEP_ModeBlock
typedef struct
{
    WORD u16MODE_HTotal;
    WORD u16MODE_HStart;

    WORD u16MODE_VStart;
    WORD u16MODE_Auto_HStart;

    WORD u16MODE_Auto_VStart;
    BYTE u8MODE_Phase;
    BYTE u8MODE_AutoTime;

    BYTE u8MODE_ModeIndex;
    BYTE u8MODE_CheckSum;
    BYTE u8MODE_UserModeIndex;
    BYTE u8MODE_Reserve0;
}__attribute__((packed)) ST_EEP_MODE;

// eEEP_HDCPBlock
#if 1
typedef struct
{
    #if ENABLE_HDCP22
    BYTE   u8HDCP22_Data[HDCP22_KEY_ROMSIZE];//1040+2
    WORD u8HDCP22_CRC;//304+2	
    #endif
    BYTE   u8HDCP_Data[HDCP_KEY_ROMSIZE];//304+2
    WORD u8HDCP_CRC;//304+2
}__attribute__((packed)) ST_EEP_HDCP;

#else
typedef struct
{
    BYTE u8HDCP_Data[0x121];
    BYTE u8HDCP_Padding[3];
}__attribute__((packed)) ST_EEP_HDCP;
#endif
// eEEP_ODBlock
typedef struct
{
    BYTE u8OD_Data[0x420];
}__attribute__((packed)) ST_EEP_OD;

// eEEP User Data
typedef struct
{
    ST_EEP_FACTORY  sUdFactory;
    ST_EEP_SYSTEM   sUdSystem;
    ST_EEP_COLOR    sUdColor[PORT_CNT][WIND_CNT];     // [PORT: max 8 input port][WIN: max 4 windows]
    ST_EEP_MODE     sUdMode[CNT_MODE_SETS];
    ST_EEP_HDCP     sUdHDCP;
    ST_EEP_OD       sUdOverDrive;
}__attribute__((packed)) ST_EEP_USER_DATA;


#endif

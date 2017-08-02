
#include "drvDDC2Bi.h"
#include "DDCColorHandler.h"

//==========COMMAND TYPE=======================================
#define GETVCPFeature               0x01
#define SETVCPFeature               0x03
#define GetTimingRequest            0x07
#define SaveCurrentSetting          0x0C
#define GETVCPPage                  0xC4
#define GETPagedVCP                 0xC6
#define SETPagedVCP                 0xC8

//==========VCP CODE=======================================
#if ENABLE_HDMI_DRR_MCCS
#define HDMI_FREESYNC_VCP			0xED//0xE6
#endif
#define CapabilitiesRequest         0xF3
#define EnableApp                   0xF5
#define AppTest                     0xB1
#define FlatPanellayout             0xB2
#define DisplayUsageTime            0xC0
#define FWLevel                     0xC9
#define OSDEnable                   0xCA

#define Geometry_Reset              0x06
#define ColorTempInc                0x0B
#define ColorTempRequest            0x0C
#define ActiveControl               0x52

#define ADJ_Red_Blacklevel          0x6C
#define ADJ_Green_Blacklevel        0x6E
#define ADJ_Blue_Blacklevel         0x70

#define ADJ_Clock                   0x0E

#define ADJ_Red_Gain                0x16
#define ADJ_Green_Gain              0x18
#define ADJ_Blue_Gain               0x1A

#define ADJ_HorizontalPos           0x20
#define ADJ_VerticalPos             0x30

#define Contrast_Auto               0x37
#define ADJ_OSD_HorizontalPos       0x38
#define ADJ_OSD_VerticalPos         0x39
#define DefaultColorChange          0x3B

#define ADJ_Phase                   0x3E

#define ADJ_Language                0x68
#define SyncType                    0xA8
#define DDCCI_Settings              0xB0
#define RGBOrdering                 0xB4

//0xE0~0xFF Manufacturer Specific
#define ADJ_ControlLock             0xE3
#define HoursRunning                0xFA
#define DisplayTime                 0xFF

#define TEST_SaveCurrentSetting     0xF4
#define TEST_GETHTotal              0xF3
#define TEST_GETVTotal              0xF2
#define TEST_GETSrcMode             0xF1

//#define TEST_GETRegE1               0xE1
//#define TEST_GETHStart              0xF1
//#define TEST_GETVStart              0xF0
#define FactoryCommand              0xC0
#define ModeChangeRequest           0x11
#define EEPROMRead                  0x09
#define EEPROMWrite                 0x02
#define CommonDataEntry             0x05
#define TimingDataEntry             0x06
#define AssetRead                   0x0B
#define AssetWrite                  0x0E
#define WhiteBalanceRead            0x23
#define WhiteBalanceWrite           0x24
#define MPUReset                    0x00

//*************************************************
#define Degauss                     0x01
#define NewControl                  0x02
#define ALL_Reset                   0x04
#define RestoryFactory              0x05
#define Color_Reset                 0x08
#define ADJ_Brightness              0x10
#define ADJ_Contrast                0x12
#define Select_Color_Preset         0x14
#define Auto_Setup                  0x1E
#define Select_InputSource          0x60
#if ENABLE_OSD_ROTATION
#define ScreenOrient                0xAA
#endif
#define H_Frequency                 0xAC
#define V_Frequency                 0xAE
#define MonitorType                 0xB6
#define APEnableKey                 0xC6
#define DisplayControlType          0xC8
#define PowerMode                   0xD6
#define PowerMode_On            0x01
#define PowerMode_Standby       0x02
#define PowerMode_Suspend       0x03
#define PowerMode_Off           0x04
#define PowerMode_SwitchOff     0x05
#define DisplayApplication          0xDC
#define VCPVersion                  0xDF
#define ManufacturerID              0xFD

//==========DPS COMMAND====================================
#if 0//ENABLE_DPS
#define MS_DPS_COMMAND              0xCC
#define MS_SET_EnableFrameColor_DPS 0xB0
#define MS_SET_FrameColor_DPS       0xB1
#define MS_SET_AdjustPWM_DPS        0xB2
#define MS_SET_Adjust_REAL_PWM_DPS  0xB3
#endif

#if HKC_PCToolCommand
#define HKC_COMMUNICATION_COMMAND_CHECK  0x13
#define ADJ_ECOmode       		0x22
#define ADJ_AspectRatiomode       0x24
#define ADJ_Gammamode       0x26
#define ADJ_PIP_PBP_Mode       0x27
#define ADJ_PIP_Source       	0x28
#define ADJ_OverDrive		0x29
#define ADJ_LedMode			0x25
#define ADJ_StandbyTime		0x31
#define ADJ_AccSetupWinEnabled 0x50
#define ADJ_SubBrightness 	0x51
#define ADJ_SubContrast	 	0x55
#define ADJ_WindowRect		0x53
#define GetTimingHDisplay	0x68
#define GetTimingVDisplay	0x69
#define DCRMode               		0xE1
#define ADJ_AUDIO_VOLUME    0x62  
#define ADJ_AUDIO_MUTE        0x8D  
#define ADJ_PIP_Win_Size        0x35
#define ADJ_GameCross_Mode    0x34 

#if 1//Enable_AdjLowbluray
#define ADJ_LowBluRay       		0x33
#endif
#if LED_HOT_Menu_Func
#define ADJ_LedColor       		0x36
#define ADJ_LedLedStrength       		0x37
#define ADJ_LedModeType       		0x38
#define ADJ_LedOffType       		0x39

#endif

#endif
//=========================================================



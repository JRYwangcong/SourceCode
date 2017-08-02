///////////////////////////////////////////////////////////////////////////////
/// @file UserPrefDef.h
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

#include <stddef.h>
#include "Board.h"
//#include "Global.h"
//#include "Mode.h"
//#include "menudef.h"
#include "UserPrefAPI.h"
#ifndef _UserPrefDef_H_
#define _UserPrefDef_H_
#include "EepromMap.h"
#include "EepromMapStruct.h"
//#include "OsdDefault.h"

#if 1//EnableDdcLed
#define OffLedMode			0x02
#define GreenLedMode		0x04
#define RedLedMode			0x06
#endif

//==========================================================================
typedef enum
{
    MODE1_DLC,
    OFF_DLC,
    MAX_DLC,

    MODE2_DLC,
    MODE3_DLC,
} DLCMode;

typedef enum
{
    INPUT_COLOR_RGB,
    INPUT_COLOR_YUV422,
    INPUT_COLOR_YUV444,
    INPUT_COLOR_YUV420,
    INPUT_COLOR_FORMAT_AUTO,
    INPUT_COLOR_MAX,

    INPUT_COLOR_YPBPR,
} InputColorType;


typedef enum
{
    eColorimetry_NoData,
    eColorimetry_ITU601,
    eColorimetry_ITU709,
    eColorimetry_ITU2020,
    eColorimetry_Extend,
    eColorimetry_Max
}InputColorimetry;

typedef enum
{
    OSD_INPUT_COLOR_RANGE_FULL,
    OSD_INPUT_COLOR_RANGE_LIMITED,
    OSD_INPUT_COLOR_RANGE_AUTO,
    OSD_INPUT_COLOR_RANGE_MAX,
} OSD_InputColorRangeType;




typedef enum
{
    eGAMMA_OFF,
    eGAMMA_18,
    eGAMMA_20,
    eGAMMA_22,
    eGAMMA_24,
    eGAMMA_MAX,
    eGAMMA_26,
    
}GammaIndexType;




typedef enum
{
    CT_STANDARD,
    CT_USER,
    CT_MOVIE,
    CT_VIVID,
    CT_FPS,
    CT_RTS,
    CT_NUMS,
    CT_GAME,

    CT_MULTIMEDIA,
} ColorTempType;


#if Enable_Lowbluray_FY
typedef enum
{
    LOW_BLUE_LIGHT_OFF,
    LOW_BLUE_LIGHT_1,
    LOW_BLUE_LIGHT_2,
    LOW_BLUE_LIGHT_3,
    LOW_BLUE_LIGHT_MAX,
}LowBlugLightType;
#endif

typedef enum
{
#if Enable_ColorTemp_5700K
    PCT_5700K,
#endif    
    PCT_6500K,
    PCT_9300K,
    PCT_USER,
//    PCT_Advanced,
    
    PCT_NUMS,

    PCT_5000K,
#if !Enable_ColorTemp_5700K
    PCT_5700K,
#endif        
    PCT_7500K,
    PCT_10000K,
} PresetColorTempType;


typedef enum
{
    AUDIO_IDLE,
    AUDIO_MUTE,
    AUDIO_UNMUTE,
    AUDIO_NUMS,
}AUDIOTypes;


typedef enum
{
    GAIN_R,
    GAIN_G,
    GAIN_B,
    GAIN_NUMS,
}GainTypes;

typedef enum
{
    IC_R,
    IC_G,
    IC_B,
    IC_C,
    IC_M,
    IC_Y,
    IC_NUMS,
}IndependentColorTypes;

#if 1//ENABLE_SUPER_RESOLUTION
typedef enum
{
	SUPER_RESOLUTION_OFF,
	SUPER_RESOLUTION_LOW,
	SUPER_RESOLUTION_MIDDLE,
	SUPER_RESOLUTION_HIGH,
	SUPER_RESOLUTION_HIGHEST,
    SUPER_RESOLUTION_NUMS
} SuperResolutionTupe;
#endif


typedef enum
{
    DP11,
    DP12,
    #if 0//ENABLE_DAISY_CHAIN
    DP12MST,
    #endif
    DPconfigtype_NUMS
} DPConfigType;

#if HDMI_VerSion_Enable
typedef enum
{
    HDMI20,
    HDMI14,
    HDMIconfigtype_NUMS
} HDMIConfigType;
#endif

typedef enum
{
    Rotate_Normal,
    Rotate_90,
    Rotate_180,
    Rotate_270,
    RotateIndex_NUMS
} RotateIndexType;

typedef enum
{
    Osd_Rotation_Normal,
    Osd_Rotation_90,
    Osd_Rotation_180,
    Osd_Rotation_270,
    Osd_RotationIndex_NUMS
} OsdRotationIndexType;



#if ENABLE_AUDIO_SETTINGS_MENU || ENABLE_DAC
typedef enum
{
    //LOS_AUTO,
    LOS_HDMI,
    LOS_PC_AUDIO,
    LOS_MAX
} LineOutSrcType;
#endif

#if 1//ENABLE_NOISE_REDUCTION
typedef enum
{
    NR_OFF,
    NR_HIGH,
    NR_MEDIUM,
    NR_LOW,
    NR_MAX,
}NoiseReductionTypes;
#endif

#if USB_Updata_FW_Enable
typedef enum
{
    USB_Idle,
    USB_Writting,
    USB_Fail,
    USB_Pass,
    USB_MAX,
}USBTypes;

#endif

#if New_Cross_Enable
typedef enum
{
  Cross_Large_Y,
  Cross_Middle_y,
  Cross_Small_Y,
  Cross_Large_R,
  Cross_Middle_R,
  Cross_Small_R,
  Cross_OFF,
  
  Cross_Max,
} Cross_Type;
#endif

#if 1//Enable_Corss_FY
typedef enum
{
  Cross_Mode_1,
  Cross_Mode_2,
  Cross_Mode_3,
  Cross_Mode_Max,
} Cross_Mode_Type;

typedef enum
{
  Cross_Color_Y,
  Cross_Color_R,
  Cross_Color_OFF,
  Cross_Color_Max,
} Cross_Color_Type;

#endif

#if Enable_Corss_LHCX
typedef enum
{
  Cross_Mode1_G,
  Cross_Mode1_R,
  Cross_Mode2_G,
  Cross_Mode2_R,
  
  Cross_Max,
} Cross_Type;
#endif

#if Enable_Corss_RunFeng
typedef enum
{
  Cross_Mode1,
  Cross_Mode2,
  Cross_Mode3,
  
  Cross_Mode_MaxNumber,
} Cross_Mode_RF_Type;
#endif


#if 1//LED_HOT_Menu_Func

typedef enum
{
LedRed,//ºì
LedYellow,//»Æ
LedGreen,//ÂÌ
LedCyan,//Çà
LedBlue,//À¶
LedPurple,//×Ï
//LedWhite,//°×
//LedColorful,//Æß²Ê

LedColor_max
}LedColortype;

/*
//Lit Led Color
typedef enum
{
LedRed,//ºì
LedYellow,//»Æ
LedGreen,//ÂÌ
LedCyan,//Çà
LedBlue,//À¶
LedPurple,//×Ï
//LedWhite,//°×
LedColorful,//Æß²Ê

LedColor_max
}LedColortype;
*/

//Lit Led Mode
typedef enum
{
LedNormal,	//³£ÁÁ
LedFlicker,	//±¬ÉÁ
LedBreath,	//ºôÎü
//LedSport,	//Ñ­É¨

LedMode_Max
}LedLitModetype;

//Lit Led Intensity  //Ç¿¶È
typedef enum
{
LedMin,
LedMid,
LedMax,

LedStrength_max
}LedStrengthType;



typedef enum
{
LedModeIndex_Off,
LedModeIndex_User,	
LedModeIndex_Breath,	

LedModeIndex_Max
}LedModeIndextype;


#endif

#if ENABLE_RTE 
typedef enum
{
    RT_OFF,
    RT_ON,
    RT_MAX,
}ResponseTimeTypes;
#endif

typedef enum
{

    SleepMode_Off,
    SleepMode_30Min,
    SleepMode_60Min,
    SleepMode_120Min,


    SleepMode_Max,
    
} SleepModeType;

typedef enum
{
AIDIOSOURCE_LINE_IN=0,
#if ENABLE_HDMI_INPUT	
AIDIOSOURCE_HDMI1,
#endif
#if ENABLE_HDMI2ND_INPUT
AIDIOSOURCE_HDMI2,
#endif
#if ENABLE_HDMI3ND_INPUT
AIDIOSOURCE_HDMI3,
#endif
#if ENABLE_DP_INPUT
AIDIOSOURCE_DP1,
#endif
#if ENABLE_DP2ND_INPUT
AIDIOSOURCE_DP2,
#endif
AIDIOSOURCE_TYPE_MAX,

#if !ENABLE_HDMI_INPUT	
AIDIOSOURCE_HDMI1,
#endif
#if !ENABLE_HDMI2ND_INPUT
AIDIOSOURCE_HDMI2,
#endif
#if !ENABLE_HDMI3ND_INPUT
AIDIOSOURCE_HDMI3,
#endif
#if !ENABLE_DP_INPUT
AIDIOSOURCE_DP1,
#endif
#if !ENABLE_DP2ND_INPUT
AIDIOSOURCE_DP2,
#endif

} AudioSourceItemIndexType;




typedef enum
{
DCR_IDLE = 0,                   // 0
DCR_START_WAIT_BL_READY,    // 1  //DCR on, count 10 mins to disable backlight
DCR_WAIT_BL_READY,          // 2  // wait 0~10 mins to warm set
DCR_BL_READY,               // 3  // set is ready almost 10 mins, ready to count 2 min to disable Backlight
DCR_START_WAIT_BL_OFF,      // 4  // set is waiting for 2 mins , then go to next step to disable Backlight
DCR_WAIT_BL_OFF,            // 5
DCR_BL_OFF,                 // 6  // backlight is disable and set Highcontrast counter to 5 min.
DCR_WAIT_EXIT_HIGH_CONT,    // 7  // disable backlight till Highcontrast counter is reached 5min.
DCR_EXIT_HIGH_CONT,         // 8
DCR_WAIT_HISTOGRAM_CHANGE,         // 9
} DCRStatus_Mode;



typedef enum
{
    MIA_NOTHING,
    MIA_GOTO_NEXT,            // goto next
    MIA_GOTO_PREV,            // goto prev
    MIA_GOTO_NEXT1,            
    MIA_GOTO_PREV1,            

    
    MIA_PREV_ITEM,
    MIA_NEXT_ITEM,
    MIA_GOTO_ADJUST_ITEM,
    MIA_RETURN_SELECT_ITEM,   //Chenge setting and return
    MIA_UNDO_RETURN_SELECT_ITEM,
    MIA_UNDO_GOTO_PREV,       //Undo and goto prev menu
    MIA_EXEC_GOTO,            // goto next or prev according to function result
    MIA_EXEC_AND_GOTO_NEXT,   // goto Next Page
    MIA_EXEC_AND_GOTO_PREV,   // goto Prev Page
    MIA_EXEC_GOTO_NEXT_RETURN,// go next or return by func result
    MIA_EXEC_FUNC,
    MIA_DEC_VALUE,            //increase value
    MIA_INC_VALUE,            //decrease value
    MIA_AUTO,

    MIA_SET_VALUE,
	
    MIA_INPUT_TYPE,
    MIA_SWITCH_SOURCE,
    MIA_INV_INC_VALUE,
    MIA_INV_DEC_VALUE,
    
    MIA_REDRAW_MENU,
    MIA_SHOW_POWER_SAVE,

    MIA_FACTORY_EXIT,


    MIA_HotMulti_PXP,
    MIA_HotMultiView_PIP_Swap,
    MIA_HotMulti_PIP_AudioSwap,
    MIA_HotMulti_PIP_Position,
    
    MIA_HotMulti_PIP_Size,

    MIA_HotMulti_PIP,
    MIA_HotMulti_PBP_OFF,
    MIA_HotMulti_PBP_2WIN,
    MIA_HotMulti_PBP_3WIN,
    MIA_HotMulti_PBP_4WIN,

    MIA_HotInput_DP,
    MIA_HotInput_HDMI1,
    MIA_HotInput_HDMI2,
    MIA_HotInput_HDMI3,

    MIA_HotPictureFormat,//3// 4: 3
    
    MIA_HotPictureMode,

    MIA_HotBrightness,
    MIA_HotContrast,
    
    MIA_HotVolume,
    MIA_HotInput,
    MIA_HotMuteOnOff,
    MIA_HotDCR,
    MIA_HOTCROSS,	

    MIA_HotSleepMode,
    MIA_HotLedMode,
    
    MIA_EXIT,

  #if Enable_Corss_LHCX_New
    MIA_HotGameMode,
    //MIA_HotGameClokShow,
  #endif

#if Enable_HotAdjLowbluray
    MIA_Lowbluray,
#endif

    MIA_HotRGBRange,

    MIA_RESET_BACKLIGHT,
#if (ODM_NAME==ODM_INL)
    MIA_RESET_VERSION,
#endif
#if F_ResetItem
     MIA_RESET_Factory,
#endif
#if LHCX_IR_Function_Enable
     MIA_GOTO_MainMenu, 
#endif
    MIA_HOTKEY1,
    MIA_HOTKEY2,
    MIA_POWER // power control
} MenuItemActionType;



//==========================================================================
#define PROPFONTTBL_INDEX	PropFontIndex
#define MAX_LANG_ITEM		(MAX_SUBMENU_INDEX)
    typedef enum
    {
#if Enable_Engliish
    LANG_English,
#endif    
#if Enable_Spanish
    LANG_Spanish,
#endif    
#if Enable_French
    LANG_French,
#endif    
#if Enable_TChinese
    LANG_TChinese,
#endif    

#if Enable_SChinese
    LANG_SChinese,
#endif    
#if Enable_Portuguese
    LANG_Portuguese,
#endif    
#if Enable_Italian
    LANG_Italian,
#endif    
#if Enable_German
    LANG_German,
#endif    
#if Enable_Korean
    LANG_Korean,
#endif    
#if Enable_Japanese
    LANG_Japanese,
#endif    

#if Enable_Russian
    LANG_Russian,
#endif    
#if Enable_Greek
    LANG_Greek,
#endif    
#if Enable_Hungarian
    LANG_Hungarian,
#endif    
#if Enable_Dutch
    LANG_Dutch,
#endif    
#if Enable_BrazilPortuguese
    LANG_BrazilPortuguese,
#endif
#if Enable_Polish
    LANG_Polish,
#endif    
#if Enable_Swedish
    LANG_Swedish,
#endif    
#if Enable_Finnish
    LANG_Finnish,
#endif    
#if Enable_Turkish
    LANG_Turkish,
#endif    
#if Enable_Czech
    LANG_Czech,
#endif    
#if Enable_Ukrainian
    LANG_Ukrainian,
#endif    
    LANG_Nums,
    
#if !Enable_Engliish
    LANG_English,
#endif   
#if !Enable_Spanish
    LANG_Spanish,
#endif    
#if !Enable_French
    LANG_French,
#endif    
#if !Enable_TChinese
    LANG_TChinese,
#endif    


#if !Enable_SChinese
    LANG_SChinese,
#endif    
#if !Enable_Korean
    LANG_Korean,
#endif    
#if !Enable_Japanese
    LANG_Japanese,
#endif    
#if !Enable_Italian
    LANG_Italian,
#endif    
#if !Enable_German
    LANG_German,
#endif    
#if !Enable_Portuguese
    LANG_Portuguese,
#endif    

#if !Enable_Russian
    LANG_Russian,
#endif    
#if !Enable_Greek
    LANG_Greek,
#endif    
#if !Enable_Hungarian
    LANG_Hungarian,
#endif    
#if !Enable_Dutch
    LANG_Dutch,
#endif    
#if !Enable_BrazilPortuguese
    LANG_BrazilPortuguese,
#endif
#if !Enable_Polish
    LANG_Polish,
#endif    
#if !Enable_Swedish
    LANG_Swedish,
#endif    
#if !Enable_Finnish
    LANG_Finnish,
#endif    
#if !Enable_Turkish
    LANG_Turkish,
#endif    
#if !Enable_Czech
    LANG_Czech,
#endif    
#if !Enable_Ukrainian
    LANG_Ukrainian,
#endif    

    
    LANG_INDEX01 = LANG_English,
    LANG_INDEX02 = LANG_German,
    LANG_INDEX03 = LANG_Spanish,
    LANG_INDEX04 = LANG_Greek,
    LANG_INDEX05 = LANG_French,
    LANG_INDEX06 = LANG_Italian,
    LANG_INDEX07 = LANG_Hungarian,
    LANG_INDEX08 = LANG_Dutch,
    LANG_INDEX09 = LANG_Portuguese,
    LANG_INDEX10 = LANG_BrazilPortuguese,
    LANG_INDEX11 = LANG_Polish,
    LANG_INDEX12 = LANG_Russian,
    LANG_INDEX13 = LANG_Swedish,
    LANG_INDEX14 = LANG_Finnish,
    LANG_INDEX15 = LANG_Turkish,
    LANG_INDEX16 = LANG_Czech,
    LANG_INDEX17 = LANG_Ukrainian,
    LANG_INDEX18 = LANG_SChinese,
    LANG_INDEX19 = LANG_TChinese,
    LANG_INDEX20 = LANG_Japanese,
    LANG_INDEX21 = LANG_Korean,


    LANG_INDEX_MAX = (LANG_Nums)
    
    } LanguageType;
    
    
typedef enum
{
PROPINDEX_English = 0,
PROPINDEX_Spanish,
PROPINDEX_French,
PROPINDEX_TChinese,
	
PROPINDEX_SChinese,
PROPINDEX_Korean,

PROPINDEX_Japanese,
PROPINDEX_Italian,
PROPINDEX_German,
PROPINDEX_Portuguese,

	
PROPINDEX_Turkish,
PROPINDEX_Russian,

PROPINDEX_BrazilPortuguese,
PROPINDEX_Dutch,
PROPINDEX_Swedish,
PROPINDEX_Finnish,
PROPINDEX_Polish,
PROPINDEX_Czech,
PROPINDEX_Hungarian,
PROPINDEX_Greek,
PROPINDEX_Ukrainian,

PROPINDEX_Nums
} PROPINDEX_Type;




//==========================================================================

#define ModeSettingType         ST_EEP_MODE
//==========================================================================

//==========================================================================
// eEEP_FactoryBlock
//==========================================================================

#define USER_PREF_EEPROM_CHKSUM                 (pUserPref->sUdFactory.u8FAC_EepromChksum)

#define USER_PREF_SRGB_DEFAULT                  (pUserPref->sUdFactory.u8FAC_SrgbDefault)
#define USER_PREF_ADC_PGA_GAIN                  (pUserPref->sUdFactory.u8FAC_AdcPgaGain)

#define USER_PREF_ADC_RED_GAIN                  (pUserPref->sUdFactory.u16FAC_AdcGainR)
#define USER_PREF_ADC_GREEN_GAIN                (pUserPref->sUdFactory.u16FAC_AdcGainG)
#define USER_PREF_ADC_BLUE_GAIN                 (pUserPref->sUdFactory.u16FAC_AdcGainB)

#define USER_PREF_ADC_RED_OFFSET                (pUserPref->sUdFactory.u8FAC_AdcOffsetR)
#define USER_PREF_ADC_GREEN_OFFSET              (pUserPref->sUdFactory.u8FAC_AdcOffsetG)
#define USER_PREF_ADC_BLUE_OFFSET               (pUserPref->sUdFactory.u8FAC_AdcOffsetB)

#define USER_PREF_RED_COLOR_SRGB                (pUserPref->sUdFactory.u8FAC_ColorSrgb_R)
#define USER_PREF_GREEN_COLOR_SRGB              (pUserPref->sUdFactory.u8FAC_ColorSrgb_G)
#define USER_PREF_BLUE_COLOR_SRGB               (pUserPref->sUdFactory.u8FAC_ColorSrgb_B)
#define USER_PREF_BRIGHTNESS_SRGB              	(pUserPref->sUdFactory.u8FAC_ColorSrgb_Bri)
#define USER_PREF_CONTRAST_SRGB               	(pUserPref->sUdFactory.u8FAC_ColorSrgb_Con)

#define USER_PREF_RED_COLOR_5000K           	(pUserPref->sUdFactory.u8FAC_5000K_R)
#define USER_PREF_GREEN_COLOR_5000K         	(pUserPref->sUdFactory.u8FAC_5000K_G)
#define USER_PREF_BLUE_COLOR_5000K          	(pUserPref->sUdFactory.u8FAC_5000K_B)
#define USER_PREF_BRIGHTNESS_5000K         		(pUserPref->sUdFactory.u8FAC_5000K_Bri)
#define USER_PREF_CONTRAST_5000K          		(pUserPref->sUdFactory.u8FAC_5000K_Con)

#define USER_PREF_RED_COLOR_5700K               (pUserPref->sUdFactory.u8FAC_5700K_R)
#define USER_PREF_GREEN_COLOR_5700K             (pUserPref->sUdFactory.u8FAC_5700K_G)
#define USER_PREF_BLUE_COLOR_5700K              (pUserPref->sUdFactory.u8FAC_5700K_B)
#define USER_PREF_BRIGHTNESS_5700K             	(pUserPref->sUdFactory.u8FAC_5700K_Bri)
#define USER_PREF_CONTRAST_5700K              	(pUserPref->sUdFactory.u8FAC_5700K_Con)

#define USER_PREF_RED_COLOR_6500K               (pUserPref->sUdFactory.u8FAC_6500K_R)
#define USER_PREF_GREEN_COLOR_6500K             (pUserPref->sUdFactory.u8FAC_6500K_G)
#define USER_PREF_BLUE_COLOR_6500K              (pUserPref->sUdFactory.u8FAC_6500K_B)
#define USER_PREF_BRIGHTNESS_6500K             	(pUserPref->sUdFactory.u8FAC_6500K_Bri)
#define USER_PREF_CONTRAST_6500K              	(pUserPref->sUdFactory.u8FAC_6500K_Con)

#define USER_PREF_RED_COLOR_7500K           	(pUserPref->sUdFactory.u8FAC_7500K_R)
#define USER_PREF_GREEN_COLOR_7500K         	(pUserPref->sUdFactory.u8FAC_7500K_G)
#define USER_PREF_BLUE_COLOR_7500K          	(pUserPref->sUdFactory.u8FAC_7500K_B)
#define USER_PREF_BRIGHTNESS_7500K         		(pUserPref->sUdFactory.u8FAC_7500K_Bri)
#define USER_PREF_CONTRAST_7500K          		(pUserPref->sUdFactory.u8FAC_7500K_Con)

#define USER_PREF_RED_COLOR_9300K               (pUserPref->sUdFactory.u8FAC_9300K_R)
#define USER_PREF_GREEN_COLOR_9300K             (pUserPref->sUdFactory.u8FAC_9300K_G)
#define USER_PREF_BLUE_COLOR_9300K              (pUserPref->sUdFactory.u8FAC_9300K_B)
#define USER_PREF_BRIGHTNESS_9300K             	(pUserPref->sUdFactory.u8FAC_9300K_Bri)
#define USER_PREF_CONTRAST_9300K              	(pUserPref->sUdFactory.u8FAC_9300K_Con)

#define USER_PREF_RED_COLOR_10000K          	(pUserPref->sUdFactory.u8FAC_10000K_R)
#define USER_PREF_GREEN_COLOR_10000K        	(pUserPref->sUdFactory.u8FAC_10000K_G)
#define USER_PREF_BLUE_COLOR_10000K        		(pUserPref->sUdFactory.u8FAC_10000K_B)
#define USER_PREF_BRIGHTNESS_10000K        		(pUserPref->sUdFactory.u8FAC_10000K_Bri)
#define USER_PREF_CONTRAST_10000K         		(pUserPref->sUdFactory.u8FAC_10000K_Con)

#define USER_PREF_DEF_SUBBRIGHTNESS             (pUserPref->sUdFactory.u8FAC_FacBrightness)
#define USER_PREF_DEF_SUBCONTRAST               (pUserPref->sUdFactory.u8FAC_FacContrast)


#define USER_PREF_SSC_FREQUENCY             (pUserPref->sUdFactory.u8FAC_SSC_Frequency)
#define USER_PREF_SSC_PERMILLAGE_ADJUST          (pUserPref->sUdFactory.u8FAC_SSC_Permillage)

#define USER_PREF_DDR_SSC_FREQ_MODULATION             (pUserPref->sUdFactory.u8FAC_DDR_SSC_Frequency)
#define USER_PREF_DDR_SSC_PERMILLAGE_ADJUST          (pUserPref->sUdFactory.u8FAC_DDR_SSC_Permillage)



#define USER_PREF_FAC_PADDING0                  (pUserPref->sUdFactory.u8FAC_Padding[0])
#define USER_PREF_FAC_PADDING1                  (pUserPref->sUdFactory.u8FAC_Padding[1])
#define USER_PREF_FAC_PADDING2                  (pUserPref->sUdFactory.u8FAC_Padding[2])

//==========================================================================
// eEEP_SystemBlock
//==========================================================================

#define MONITOR_FLAGS                       	(pUserPref->sUdSystem.u16SYS_SystemFlag)
#define MISC_FLAGS                          	(pUserPref->sUdSystem.u16SYS_Misc)
#define DDCCI_FUN_FLAGS                     	(pUserPref->sUdSystem.u16SYS_DDCFlag)

#define USER_PREF_CTRL_HPD           			(pUserPref->sUdSystem.u8SYS_CtrlHPD)
#define USER_PREF_INPUT_PRIORITY            	(pUserPref->sUdSystem.u8SYS_AutoDetect)
#define USER_PREF_WIDE_MODE(src)                (pUserPref->sUdSystem.u8SYS_WideMode[src])

#define USER_PREF_DLC_MODE                  	(pUserPref->sUdSystem.u8SYS_DLC)

#if ENABLE_MULTI_WINDOW_SETTINGS_MENU
#define USER_PREF_MULTI_WIN_MODE          		(pUserPref->sUdSystem.u8SYS_MltWinOnOff)
#define USER_PREF_MULTI_WIN_SIZE(win)        	(pUserPref->sUdSystem.u8SYS_MltWinSize[win])
#define USER_PREF_MULTI_WIN_BOARDER_COLOR   	(pUserPref->sUdSystem.u8SYS_MltWinBorderColor)

#define USER_PREF_PIP_POSITION                 		(pUserPref->sUdSystem.u8SYS_PIP_Possition)

#endif
#define USER_PREF_WIN_SEL                   	(pUserPref->sUdSystem.u8SYS_MltWin_SEL)
#define USER_PREF_WIN_SRC(win)              	(pUserPref->sUdSystem.u8SYS_MltWinSRC[win])

#if ENABLE_MULTI_WINDOW_SETTINGS_MENU
#define USER_PREF_MULTI_WIN_H_POS(win)     		(pUserPref->sUdSystem.MltWinPos[win].u16X)
#define USER_PREF_MULTI_WIN_V_POS(win)     		(pUserPref->sUdSystem.MltWinPos[win].u16Y)
#define USER_PREF_MULTI_WIN_ROTATE(win)      	(pUserPref->sUdSystem.u8SYS_MltWinRotate[win])
#endif

#define USER_PREF_LANGUAGE                  	(pUserPref->sUdSystem.u8SYS_Language)
#define USER_PREF_TRANSPARENCY              	(pUserPref->sUdSystem.u8SYS_MenuTranspareny)

#define USER_PREF_DPConfigType                  (pUserPref->sUdSystem.u8SYS_DPConfigType)

#if HDMI_VerSion_Enable
#define USER_PREF_HDMIConfigType                  (pUserPref->sUdSystem.u8SYS_HDMIConfigType)
#endif

#if Enable_Menu_Panel_Rotate_180
#define USER_PREF_ROTATE_INDEX                  (pUserPref->sUdSystem.u8SYS_RotateIndex)
#endif

#define USER_PREF_OSD_ROTATION                 (pUserPref->sUdSystem.u8SYS_OsdRotation)


#define USER_PREF_DDCCI  	                  	(pUserPref->sUdSystem.u8SYS_DDCCI)
#define USER_PREF_OSD_H_POSITION                (pUserPref->sUdSystem.u8SYS_OSDHPos)
#define USER_PREF_OSD_V_POSITION                (pUserPref->sUdSystem.u8SYS_OSDVPos)
#define USER_PREF_OSD_TIMEOUT                   (pUserPref->sUdSystem.u8SYS_OSDTime)
#define USER_PREF_OSD_LOCK                  	(pUserPref->sUdSystem.u8SYS_OSDLock)
#define USER_PREF_OSD_PIVOT    					(pUserPref->sUdSystem.u8SYS_OSDPivot)

#if AudioFunc/* || ENABLE_DAC*/
#define USER_PREF_VOLUME                    			(pUserPref->sUdSystem.u8SYS_Volume)
#define USER_PREF_AUDIO_SRC(Src)                 			(pUserPref->sUdSystem.u8SYS_AudioSRC[Src])
#define USER_PREF_AUDIO_SOURCE                 			(pUserPref->sUdSystem.u8SYS_AudioSRC[SrcInputPortM])
//#define USER_PREF_AUDIO_MUTE             			(pUserPref->sUdSystem.u8SYS_AudioMuteStatus)
#define USER_PREF_AUDIO_CONFIG                 		(pUserPref->sUdSystem.u8SYS_AudioConfig)
#define USER_PREF_AUDIO_CHANNEL_OUT             	(pUserPref->sUdSystem.u8SYS_AudioChannelOut)
#endif
#if ENABLE_ADJ_AUDIO_OUTPUT
#define USER_PREF_AUDIO_OUTPUTTYPE                    		(pUserPref->sUdSystem.u8SYS_AudioOutputType)
#endif

#define USER_PREF_OSD_TIME                  	(pUserPref->sUdSystem.u8SYS_OffTimer)
#define USER_PREF_MULTI_WINDOW_OVERSCAN(win)    (pUserPref->sUdSystem.u8SYS_OverScan[win])
#define USER_PREF_RESPONSE_TIME            		(pUserPref->sUdSystem.u8SYS_ResponseTime)
#define USER_PREF_BACKLIGHT_TIME            	(pUserPref->sUdSystem.u32SYS_BacklightTime)
#define USER_PREF_BRIGHTNESS               (pUserPref->sUdSystem.u8SYS_BackLight)
#define USER_PREF_BRIGHTNESS_USER               (pUserPref->sUdSystem.u8SYS_BackLight_User)

#define USER_PREF_rgbBRIGHTNESS               (pUserPref->sUdSystem.u8SYS_rgbBrightness)

#if 1//ENABLE_DEMURA_FUNCTION
#define USER_PREF_DEMURA_MEASURE_COMMAND        (pUserPref->sUdSystem.u8SYS_DeMuraMeasureCommand)
#endif
#define USER_PREF_PRESET_MODE_EEPROM_INDEX      (pUserPref->sUdSystem.u8SYS_Preset_Mode_Eeprom_Index)
#define USER_PREF_USER_MODE_EEPROM_INDEX        (pUserPref->sUdSystem.u8SYS_User_Mode_Eeprom_Index)
#define USER_PREF_SYS_PADDING0                  (pUserPref->sUdSystem.u8SYS_Padding[0])

#define USER_PREF_SLEEPMODE                  	(pUserPref->sUdSystem.u8SYS_SleepMode)

#define USER_PREF_LOSMODE 				(pUserPref->sUdSystem.u8SYS_LosMode)

#define USER_PREF_LEDMODE                       (pUserPref->sUdSystem.u8SYS_LedMode)

#define USER_PREF_LEDColorful_MODE                       (pUserPref->sUdSystem.u8SYS_LEDColorfulMode)

#define USER_PREF_HOT_PICTURE_INDEX                       (pUserPref->sUdSystem.u8SYS_Hot_PICTURE_INDEX)

#define USER_PREF_LedStrength                       (pUserPref->sUdSystem.u8SYS_LedStrength)

#define USER_PREF_LedModeType                       (pUserPref->sUdSystem.u8SYS_LedModeType)

#define USER_PREF_LEDMODE_INDEX                       (pUserPref->sUdSystem.u8SYS_LedModeIndex)

//==========================================================================
// ST_EEP_HDCPBlock
//==========================================================================

#define USER_HDCP_DATA(u16Index)                    			(pUserPref->sUdHDCP.u8HDCP_Data[u16Index])
#define USER_HDCP_CRC                    			(pUserPref->sUdHDCP.u8HDCP_CRC)

#if ENABLE_HDCP22
#define USER_HDCP22_DATA(u16Index)              (pUserPref->sUdHDCP.u8HDCP22_Data[u16Index])
#define USER_HDCP22_CRC                    			(pUserPref->sUdHDCP.u8HDCP22_CRC)
#endif

//==========================================================================
// eEEP_ColorParameterBlock
//==========================================================================

#define USER_PREF_CONTRAST(win)                 (pUserPref->sUdColor[USER_PREF_WIN_SRC(win)][win].u8COLOR_Contrast)
#define USER_PREF_CONTRAST_USER(win)                 (pUserPref->sUdColor[USER_PREF_WIN_SRC(win)][win].u8COLOR_Contrast_User)
#define USER_PREF_INPUT_COLOR_FORMAT(win)   	(pUserPref->sUdColor[USER_PREF_WIN_SRC(win)][win].u8COLOR_InputColorFormat)
#define USER_PREF_INPUT_COLOR_RANGE(win)   	    (pUserPref->sUdColor[USER_PREF_WIN_SRC(win)][win].u8COLOR_InputColorRange)
#define USER_PREF_GAMMA(win)                	(pUserPref->sUdColor[USER_PREF_WIN_SRC(win)][win].u8COLOR_Gamma)

#define USER_PREF_SATURATION(win)            	(pUserPref->sUdColor[USER_PREF_WIN_SRC(win)][win].u8COLOR_Saturature)
#define USER_PREF_HUE(win)                   	(pUserPref->sUdColor[USER_PREF_WIN_SRC(win)][win].u8COLOR_Hue)
#define USER_PREF_COLOR_TEMP(win)            	(pUserPref->sUdColor[USER_PREF_WIN_SRC(win)][win].u8COLOR_ColorTemp)

#define USER_PREF_PICTURE_MODE(win)            	(pUserPref->sUdColor[USER_PREF_WIN_SRC(win)][win].u8COLOR_PictureMode)

#define USER_PREF_SHARPNESS(win)             	(pUserPref->sUdColor[USER_PREF_WIN_SRC(win)][win].u8COLOR_Sharpness)
#define USER_PREF_RED_COLOR(win)             	(pUserPref->sUdColor[USER_PREF_WIN_SRC(win)][win].u8COLOR_RedColor)
#define USER_PREF_GREEN_COLOR(win)           	(pUserPref->sUdColor[USER_PREF_WIN_SRC(win)][win].u8COLOR_GreenColor)
#define USER_PREF_BLUE_COLOR(win)            	(pUserPref->sUdColor[USER_PREF_WIN_SRC(win)][win].u8COLOR_BlueColor)
//#define USER_PREF_RED_OFFSET(win)            (pUserPref->sUdColor[USER_PREF_WIN_SRC(win)][win].u8COLOR_RedOffset)
//#define USER_PREF_GREEN_OFFSET(win)          (pUserPref->sUdColor[USER_PREF_WIN_SRC(win)][win].u8COLOR_GreenOffset)
//#define USER_PREF_BLUE_OFFSET(win)           (pUserPref->sUdColor[USER_PREF_WIN_SRC(win)][win].u8COLOR_BlueOffset)

#define USER_PREF_R_HUE(win)                 	(pUserPref->sUdColor[USER_PREF_WIN_SRC(win)][win].u8COLOR_R_Hue)
#define USER_PREF_G_HUE(win)                 	(pUserPref->sUdColor[USER_PREF_WIN_SRC(win)][win].u8COLOR_G_Hue)
#define USER_PREF_B_HUE(win)                 	(pUserPref->sUdColor[USER_PREF_WIN_SRC(win)][win].u8COLOR_B_Hue)
#define USER_PREF_C_HUE(win)                 	(pUserPref->sUdColor[USER_PREF_WIN_SRC(win)][win].u8COLOR_C_Hue)
#define USER_PREF_M_HUE(win)                 	(pUserPref->sUdColor[USER_PREF_WIN_SRC(win)][win].u8COLOR_M_Hue)
#define USER_PREF_Y_HUE(win)                 	(pUserPref->sUdColor[USER_PREF_WIN_SRC(win)][win].u8COLOR_Y_Hue)

#define USER_PREF_R_SATURATION(win)          	(pUserPref->sUdColor[USER_PREF_WIN_SRC(win)][win].u8COLOR_R_Saturation)
#define USER_PREF_G_SATURATION(win)          	(pUserPref->sUdColor[USER_PREF_WIN_SRC(win)][win].u8COLOR_G_Saturation)
#define USER_PREF_B_SATURATION(win)          	(pUserPref->sUdColor[USER_PREF_WIN_SRC(win)][win].u8COLOR_B_Saturation)
#define USER_PREF_C_SATURATION(win)          	(pUserPref->sUdColor[USER_PREF_WIN_SRC(win)][win].u8COLOR_C_Saturation)
#define USER_PREF_M_SATURATION(win)          	(pUserPref->sUdColor[USER_PREF_WIN_SRC(win)][win].u8COLOR_M_Saturation)
#define USER_PREF_Y_SATURATION(win)          	(pUserPref->sUdColor[USER_PREF_WIN_SRC(win)][win].u8COLOR_Y_Saturation)

#define USER_PREF_Color_R(win)          	(pUserPref->sUdColor[USER_PREF_WIN_SRC(win)][win].u8COLOR_R)
#define USER_PREF_Color_G(win)          (pUserPref->sUdColor[USER_PREF_WIN_SRC(win)][win].u8COLOR_G)
#define USER_PREF_Color_B(win)          	(pUserPref->sUdColor[USER_PREF_WIN_SRC(win)][win].u8COLOR_B)
#define USER_PREF_Color_C(win)          	(pUserPref->sUdColor[USER_PREF_WIN_SRC(win)][win].u8COLOR_C)
#define USER_PREF_Color_M(win)          (pUserPref->sUdColor[USER_PREF_WIN_SRC(win)][win].u8COLOR_M)
#define USER_PREF_Color_Y(win)          	(pUserPref->sUdColor[USER_PREF_WIN_SRC(win)][win].u8COLOR_Y)


#define USER_PREF_RED_BLACK_LEVEL(win)       	(pUserPref->sUdColor[USER_PREF_WIN_SRC(win)][win].u8COLOR_RedBlackLevel)
#define USER_PREF_GREEN_BLACK_LEVEL(win)     	(pUserPref->sUdColor[USER_PREF_WIN_SRC(win)][win].u8COLOR_GreenBlackLevel)
#define USER_PREF_BLUE_BLACK_LEVEL(win)      	(pUserPref->sUdColor[USER_PREF_WIN_SRC(win)][win].u8COLOR_BlueBlackLevel)

#define USER_PREF_SUPER_RESOLUTION(win)      	(pUserPref->sUdColor[USER_PREF_WIN_SRC(win)][win].u8COLOR_SuperResolution)
#define USER_PREF_NOISE_REDUCTION(win)          (pUserPref->sUdColor[USER_PREF_WIN_SRC(win)][win].u8COLOR_NoiseReduction)


#define USER_PREF_LOWBLUELIGHT(win)             	(pUserPref->sUdColor[USER_PREF_WIN_SRC(win)][win].u8COLOR_LowBlueLight)

//==========================================================================
// eEEP_ModeBlock
//==========================================================================

#define USER_PREF_H_TOTAL(port)               	(pUserPref->sUdMode[MODE_OFFSET].u16MODE_HTotal)
#define USER_PREF_H_START(port)               	(pUserPref->sUdMode[MODE_OFFSET].u16MODE_HStart)
#define USER_PREF_V_START(port)               	(pUserPref->sUdMode[MODE_OFFSET].u16MODE_VStart)
#define USER_PREF_AUTO_HSTART(port)         	(pUserPref->sUdMode[MODE_OFFSET].u16MODE_Auto_HStart)
#define USER_PREF_AUTO_VSTART(port)         	(pUserPref->sUdMode[MODE_OFFSET].u16MODE_Auto_VStart)

#define USER_PREF_PHASE(port)                   (pUserPref->sUdMode[MODE_OFFSET].u8MODE_Phase)
#define USER_PREF_AUTOTIME(port)                (pUserPref->sUdMode[MODE_OFFSET].u8MODE_AutoTime)

#define USER_PREF_MODE_INDEX(port)              (pUserPref->sUdMode[MODE_OFFSET].u8MODE_ModeIndex)
#define USER_PREF_MODE_CHECKSUM(port)           (pUserPref->sUdMode[MODE_OFFSET].u8MODE_CheckSum)
#define USER_PREF_USER_MODE_INDEX(port)         (pUserPref->sUdMode[MODE_OFFSET].u8MODE_UserModeIndex)
#define USER_PREF_RESERVE0(port)                (pUserPref->sUdMode[MODE_OFFSET].u8MODE_Reserve0)


//==========================================================================
////////////////////////////////////////////////////

#define CURRENT_CONTRAST              USER_PREF_CONTRAST(USER_PREF_WIN_SEL)
#define CURRENT_CONTRAST_USER              USER_PREF_CONTRAST_USER(USER_PREF_WIN_SEL)
#define CURRENT_INPUT_COLOR_FORMAT    USER_PREF_INPUT_COLOR_FORMAT(USER_PREF_WIN_SEL)
#define CURRENT_INPUT_COLOR_RANGE     USER_PREF_INPUT_COLOR_RANGE(USER_PREF_WIN_SEL)
#define CURRENT_GAMMA                 USER_PREF_GAMMA(USER_PREF_WIN_SEL)

#define CURRENT_SATURATION            USER_PREF_SATURATION(USER_PREF_WIN_SEL)
#define CURRENT_HUE                   USER_PREF_HUE(USER_PREF_WIN_SEL)
#define CURRENT_COLOR_TEMP            USER_PREF_COLOR_TEMP(USER_PREF_WIN_SEL)

#define CURRENT_PICTURE_MODE            USER_PREF_PICTURE_MODE(USER_PREF_WIN_SEL)

#define CURRENT_SHARPNESS             USER_PREF_SHARPNESS(USER_PREF_WIN_SEL)
#define CURRENT_RED_COLOR             USER_PREF_RED_COLOR(USER_PREF_WIN_SEL)
#define CURRENT_GREEN_COLOR           USER_PREF_GREEN_COLOR(USER_PREF_WIN_SEL)
#define CURRENT_BLUE_COLOR            USER_PREF_BLUE_COLOR(USER_PREF_WIN_SEL)
//#define CURRENT_RED_OFFSET            USER_PREF_RED_OFFSET(USER_PREF_WIN_SEL)
//#define CURRENT_GREEN_OFFSET          USER_PREF_GREEN_OFFSET(USER_PREF_WIN_SEL)
//#define CURRENT_BLUE_OFFSET           USER_PREF_BLUE_OFFSET(USER_PREF_WIN_SEL)

#define CURRENT_R_HUE                 USER_PREF_R_HUE(USER_PREF_WIN_SEL)
#define CURRENT_G_HUE                 USER_PREF_G_HUE(USER_PREF_WIN_SEL)
#define CURRENT_B_HUE                 USER_PREF_B_HUE(USER_PREF_WIN_SEL)
#define CURRENT_C_HUE                 USER_PREF_C_HUE(USER_PREF_WIN_SEL)
#define CURRENT_M_HUE                 USER_PREF_M_HUE(USER_PREF_WIN_SEL)
#define CURRENT_Y_HUE                 USER_PREF_Y_HUE(USER_PREF_WIN_SEL)

#define CURRENT_R_SATURATION          USER_PREF_R_SATURATION(USER_PREF_WIN_SEL)
#define CURRENT_G_SATURATION          USER_PREF_G_SATURATION(USER_PREF_WIN_SEL)
#define CURRENT_B_SATURATION          USER_PREF_B_SATURATION(USER_PREF_WIN_SEL)
#define CURRENT_C_SATURATION          USER_PREF_C_SATURATION(USER_PREF_WIN_SEL)
#define CURRENT_M_SATURATION          USER_PREF_M_SATURATION(USER_PREF_WIN_SEL)
#define CURRENT_Y_SATURATION          USER_PREF_Y_SATURATION(USER_PREF_WIN_SEL)

#define CURRENT_Color_R                      USER_PREF_Color_R(USER_PREF_WIN_SEL)
#define CURRENT_Color_G                      USER_PREF_Color_G(USER_PREF_WIN_SEL)
#define CURRENT_Color_B                      USER_PREF_Color_B(USER_PREF_WIN_SEL)
#define CURRENT_Color_C                      USER_PREF_Color_C(USER_PREF_WIN_SEL)
#define CURRENT_Color_M                      USER_PREF_Color_M(USER_PREF_WIN_SEL)
#define CURRENT_Color_Y                      USER_PREF_Color_Y(USER_PREF_WIN_SEL)

#define CURRENT_RED_BLACK_LEVEL       USER_PREF_RED_BLACK_LEVEL(USER_PREF_WIN_SEL)
#define CURRENT_GREEN_BLACK_LEVEL     USER_PREF_GREEN_BLACK_LEVEL(USER_PREF_WIN_SEL)
#define CURRENT_BLUE_BLACK_LEVEL      USER_PREF_BLUE_BLACK_LEVEL(USER_PREF_WIN_SEL)

#define CURRENT_SUPER_RESOLUTION      USER_PREF_SUPER_RESOLUTION(USER_PREF_WIN_SEL)
#define CURRENT_NOISE_REDUCTION       USER_PREF_NOISE_REDUCTION(USER_PREF_WIN_SEL)

#if ENABLE_MULTI_WINDOW_SETTINGS_MENU
#define CURRENT_MULTI_WINDOW_MODE     USER_PREF_MULTI_WIN_MODE
#endif

#define GET_USER_PREF_CONTRAST(win)   USER_PREF_CONTRAST(win)
#define GET_USER_PREF_CONTRAST_USER(win)   USER_PREF_CONTRAST_USER(win)

#define USER_PREF_INPUT_TYPE          USER_PREF_WIN_SRC(DISPLAY_WIN1)
#define USER_PREF_INPUT_TYPE_SUBWIN1               USER_PREF_WIN_SRC(DISPLAY_WIN2)
#define USER_PREF_INPUT_TYPE_SUBWIN2               USER_PREF_WIN_SRC(DISPLAY_WIN3)
#define USER_PREF_INPUT_TYPE_SUBWIN3               USER_PREF_WIN_SRC(DISPLAY_WIN4)

#define USERPREFLOWBLURAY               USER_PREF_LOWBLUELIGHT(USER_PREF_WIN_SEL)

//==========================================================================
#define USER_PREF_HUE_MOVIE                 //??
#define USER_PREF_SATURATION_MOVIE          //??

#define USER_PREF_HUE_VIDEO_GAME            //??
#define USER_PREF_SATURATION_VIDEO_GAME     //??


//==================================================================================
#define AUTO_TIMES_BIT      BIT0

#define CHECKSUM_VALUE      0x34
#define MODE_CHECKSUM_VALUE     	 0x55
#define BOARD_CHECKSUM                 0x39//0x59//(CNT_TOTAL&0xFF)
#define BOARD_CHECKSUM2                 (BOARD_CHECKSUM+1)

#define NUMBER_OF_MODE          15 //25 //50

#define SAVE_NONE 0
#define SAVE_MONITOR_SETTING    BIT0
#define SAVE_MODE_SETTING       BIT1
#define SAVE_FACTORY_SETTING    BIT2

//============================================================================================

// Monitor status flags for storing into NVRAM

#define POWER_ON_BIT                    BIT0    // system power on/off status
#define POWER_ON_FLAG                   (MONITOR_FLAGS&POWER_ON_BIT)
#define SET_POWER_ON_FLAG()             (MONITOR_FLAGS|=POWER_ON_BIT)
#define CLR_POWER_ON_FLAG()             (MONITOR_FLAGS&=~POWER_ON_BIT)

#define FACTORY_MODE_BIT                BIT1    // factory mode
#define FACTORY_MODE_FLAG               (MONITOR_FLAGS&FACTORY_MODE_BIT)
#define SET_FACTORY_MODE_FLAG()         (MONITOR_FLAGS|=FACTORY_MODE_BIT)
#define CLR_FACTORY_MODE_FLAG()         (MONITOR_FLAGS&=~FACTORY_MODE_BIT)

/// Set/clear this flag according by Burnin item in factory Menu.
#define BURNIN_MODE_BIT                 BIT2    // burnin mode
#define BURNIN_MODE_FLAG                (MONITOR_FLAGS&BURNIN_MODE_BIT)
#define SET_BURNIN_MODE_FLAG()          (MONITOR_FLAGS|=BURNIN_MODE_BIT)
#define CLR_BURNIN_MODE_FLAG()          (MONITOR_FLAGS&=~BURNIN_MODE_BIT)


#define OSD_LOCK_BIT                    BIT3
#define OSD_LOCK_FLAG                   (MONITOR_FLAGS&OSD_LOCK_BIT)
#define SET_OSD_LOCK_FLAG()             (MONITOR_FLAGS|=OSD_LOCK_BIT)
#define CLR_OSD_LOCK_FLAG()             (MONITOR_FLAGS&=~OSD_LOCK_BIT)

#define HOT_OSD_LOCK_BIT                BIT5
#define HOT_OSD_LOCK_FLAG               (MONITOR_FLAGS&HOT_OSD_LOCK_BIT)
#define SET_HOT_OSD_LOCK_FLAG()         (MONITOR_FLAGS|=HOT_OSD_LOCK_BIT)
#define CLR_HOT_OSD_LOCK_FLAG()         (MONITOR_FLAGS&=~HOT_OSD_LOCK_BIT)

#define DDCCI_ENABLE_BIT                BIT6
#define DDCCI_ENABLE_FLAG               (MONITOR_FLAGS&DDCCI_ENABLE_BIT)
#define SET_DDCCI_ENABLE_FLAG()         (MONITOR_FLAGS|=DDCCI_ENABLE_BIT)
#define CLR_DDCCI_ENABLE_FLAG()         (MONITOR_FLAGS&=~DDCCI_ENABLE_BIT)

#if ENABLE_DCR
#define DCR_CONTROL_BIT               BIT8
#define DCR_ENABLE_FLAG               (MONITOR_FLAGS&DCR_CONTROL_BIT)
#define SET_DCR_ENABLE_FLAG()         (MONITOR_FLAGS|=DCR_CONTROL_BIT)
#define CLR_DCR_ENABLE_FLAG()         (MONITOR_FLAGS&=~DCR_CONTROL_BIT)
#endif

#if 0//LED_HOT_Menu_Func
#define LED_FUNC_ON_BIT               BIT9
#define LED_FUNC_ON_FLAG               (MONITOR_FLAGS&LED_FUNC_ON_BIT)
#define SET_LED_FUNC_ON_FLAG()         (MONITOR_FLAGS|=LED_FUNC_ON_BIT)
#define CLR_LED_FUNC_ON_FLAG()         (MONITOR_FLAGS&=~LED_FUNC_ON_BIT)
#endif


#define DCR_FLAG_BIT               BIT10
#define DCR_FLAG_ENABLE               (MONITOR_FLAGS&DCR_FLAG_BIT)
#define SET_DCR_FLAG_ENABLE()         (MONITOR_FLAGS|=DCR_FLAG_BIT)
#define CLR_DCR_FLAG_ENABLE()         (MONITOR_FLAGS&=~DCR_FLAG_BIT)


#define AUDIO_MUTE_BIT               BIT11
#define AUDIO_MUTE_FLAG              (MONITOR_FLAGS&AUDIO_MUTE_BIT)
#define SET_AUDIO_MUTE_FLAG()        (MONITOR_FLAGS|=AUDIO_MUTE_BIT)
#define CLR_AUDIO_MUTE_FLAG()        (MONITOR_FLAGS&=~AUDIO_MUTE_BIT)

#if HDMI_HPD_Enable
#define MENU_HDMI_HPD_BIT               BIT12
#define MENU_HDMI_HPD_FLAG              (MONITOR_FLAGS&MENU_HDMI_HPD_BIT)
#define SET_MENU_HDMI_HPD_FLAG()        (MONITOR_FLAGS|=MENU_HDMI_HPD_BIT)
#define CLR_MENU_HDMI_HPD_FLAG()        (MONITOR_FLAGS&=~MENU_HDMI_HPD_BIT)
#endif

#define PIP_MAIN_AUDIO_BIT                                        	 BIT13
#define PIP_MAIN_AUDIO_FLAG                                  	 (MONITOR_FLAGS&PIP_MAIN_AUDIO_BIT)
#define SET_PIP_MAIN_AUDIO_FLAG()                        	 (MONITOR_FLAGS|=PIP_MAIN_AUDIO_BIT)
#define CLR_PIP_MAIN_AUDIO_FLAG()                        	 (MONITOR_FLAGS&=~PIP_MAIN_AUDIO_BIT)



//Steven101019, about ES logo on/off
#define OSD_LOGO_BIT                                 BIT14
#define OSD_LOGO_FLAG                             (MONITOR_FLAGS&OSD_LOGO_BIT)
#define SET_OSD_LOGO_FLAG()                   (MONITOR_FLAGS|=OSD_LOGO_BIT)
#define CLR_OSD_LOGO_FLAG()                   (MONITOR_FLAGS&=~OSD_LOGO_BIT)

#define bFreeSyncBit				               BIT15
#define FreeSyncFlag			                     (MONITOR_FLAGS&bFreeSyncBit )
#define Set_FreeSyncFlag()		              (MONITOR_FLAGS|=bFreeSyncBit)
#define Clr_FreeSyncFlag()		              (MONITOR_FLAGS&=~bFreeSyncBit)

//===================================================================================
///For DDCCI Function Flag
#define DDCCI_AUTO_COLOR_BIT                BIT0
#define DDCCI_AUTO_COLOR_FLAG               (DDCCI_FUN_FLAGS&DDCCI_AUTO_COLOR_BIT)
#define SET_DDCCI_AUTO_COLOR_FLAG()         (DDCCI_FUN_FLAGS|=DDCCI_AUTO_COLOR_BIT)
#define CLR_DDCCI_AUTO_COLOR_FLAG()         (DDCCI_FUN_FLAGS&=~DDCCI_AUTO_COLOR_BIT)

#define DDCCI_AUTO_GEOMTRY_BIT              BIT1
#define DDCCI_AUTO_GEOMTRY_FLAG             (DDCCI_FUN_FLAGS&DDCCI_AUTO_GEOMTRY_BIT)
#define SET_DDCCI_AUTO_GEOMTRY_FLAG()       (DDCCI_FUN_FLAGS|=DDCCI_AUTO_GEOMTRY_BIT)
#define CLR_DDCCI_AUTO_GEOMTRY_FLAG()       (DDCCI_FUN_FLAGS&=~DDCCI_AUTO_GEOMTRY_BIT)

#define DDCCI_AUTO_ADJUST_BIT               BIT2
#define DDCCI_AUTO_ADJUST_FLAG              (DDCCI_FUN_FLAGS&DDCCI_AUTO_ADJUST_BIT)
#define SET_DDCCI_AUTO_ADJUST_FLAG()        (DDCCI_FUN_FLAGS|=DDCCI_AUTO_ADJUST_BIT)
#define CLR_DDCCI_AUTO_ADJUST_FLAG()        (DDCCI_FUN_FLAGS&=~DDCCI_AUTO_ADJUST_BIT)

#if 0
#define DDCCI_SAVE_BIT                      BIT3
#define DDCCI_SAVE_FLAG                     (DDCCI_FUN_FLAGS&DDCCI_SAVE_BIT)
#define SET_DDCCI_SAVE_FLAG()               (DDCCI_FUN_FLAGS|=DDCCI_SAVE_BIT)
#define CLR_DDCCI_SAVE_FLAG()               (DDCCI_FUN_FLAGS&=~DDCCI_SAVE_BIT)
#endif

#define DDCCI_FORCE_POWER_SAVE_BIT          BIT4
#define DDCCI_FORCE_POWER_SAVE_FLAG         (DDCCI_FUN_FLAGS&DDCCI_FORCE_POWER_SAVE_BIT)
#define SET_DDCCI_FORCE_POWER_SAVE_FLAG()   (DDCCI_FUN_FLAGS|=DDCCI_FORCE_POWER_SAVE_BIT)
#define CLR_DDCCI_FORCE_POWER_SAVE_FLAG()   (DDCCI_FUN_FLAGS&=~DDCCI_FORCE_POWER_SAVE_BIT)

#define DDCCI_MCCS_CMD_BIT                  BIT5
#define DDCCI_MCCS_CMD_FLAG                 (DDCCI_FUN_FLAGS&DDCCI_MCCS_CMD_BIT)
#define SET_DDCCI_MCCS_CMD_FLAG()           (DDCCI_FUN_FLAGS|=DDCCI_MCCS_CMD_BIT)
#define CLR_DDCCI_MCCS_CMD_FLAG()           (DDCCI_FUN_FLAGS&=~DDCCI_MCCS_CMD_BIT)

#if ENABLE_VGA_INPUT
#define bLoadVGAEdidBit				               BIT6
#define LoadVGAEdidFlag			                     (DDCCI_FUN_FLAGS&bLoadVGAEdidBit )
#define Set_LoadVGAEdidFlag()		              (DDCCI_FUN_FLAGS|=bLoadVGAEdidBit)
#define Clr_LoadVGAEdidFlag()		              (DDCCI_FUN_FLAGS&=~bLoadVGAEdidBit)
#endif

#define bLoadDigital1EdidBit				               BIT7
#define LoadDigital1EdidFlag			                     (DDCCI_FUN_FLAGS&bLoadDigital1EdidBit )
#define Set_LoadDigital1EdidFlag()		              (DDCCI_FUN_FLAGS|=bLoadDigital1EdidBit)
#define Clr_LoadDigital1EdidFlag()		                     (DDCCI_FUN_FLAGS&=~bLoadDigital1EdidBit)

#define bLoadDigital2EdidBit				               BIT8
#define LoadDigital2EdidFlag			                     (DDCCI_FUN_FLAGS&bLoadDigital2EdidBit )
#define Set_LoadDigital2EdidFlag()		              (DDCCI_FUN_FLAGS|=bLoadDigital2EdidBit)
#define Clr_LoadDigital2EdidFlag()		                     (DDCCI_FUN_FLAGS&=~bLoadDigital2EdidBit)

#define bLoadDigital3EdidBit				               BIT9
#define LoadDigital3EdidFlag			                     (DDCCI_FUN_FLAGS&bLoadDigital3EdidBit )
#define Set_LoadDigital3EdidFlag()		              (DDCCI_FUN_FLAGS|=bLoadDigital3EdidBit)
#define Clr_LoadDigital3EdidFlag()		                     (DDCCI_FUN_FLAGS&=~bLoadDigital3EdidBit)


//===================================================================================
///Misc Flags
#if (ENABLE_MWE)
#define MWE_FUNC_ENABLE_BIT         BIT0
#define MWE_FUNC_ENABLE_FLAG        (MISC_FLAGS&MWE_FUNC_ENABLE_BIT)
#define SET_MWE_FUNC_ENABLE_FLAG()  (MISC_FLAGS|=MWE_FUNC_ENABLE_BIT)
#define CLR_MWE_FUNC_ENABLE_FLAG()  (MISC_FLAGS&=~MWE_FUNC_ENABLE_BIT)

#define MWE_WIN_CONTROL_BIT         BIT1
#define MWE_WIN_CONTROL_FLAG        (MISC_FLAGS&MWE_WIN_CONTROL_BIT)
#define SET_MWE_WIN_CONTROL_FLAG()  (MISC_FLAGS|=MWE_WIN_CONTROL_BIT)
#define CLR_MWE_WIN_CONTROL_FLAG()  (MISC_FLAGS&=~MWE_WIN_CONTROL_BIT)
#endif

/*
#define CS_BYPASS_BIT                       BIT2
#define CS_BYPASS_FLAG                      (MISC_FLAGS&CS_BYPASS_BIT)
#define SET_CS_BYPASS_FLAG()                (MISC_FLAGS|=CS_BYPASS_BIT)
#define CLR_CS_BYPASS_FLAG()                (MISC_FLAGS&=~CS_BYPASS_BIT)
#define PQ_BYPASS_BIT                       BIT3
#define PQ_BYPASS_FLAG                      (MISC_FLAGS&PQ_BYPASS_BIT)
#define SET_PQ_BYPASS_FLAG()                (MISC_FLAGS|=PQ_BYPASS_BIT)
#define CLR_PQ_BYPASS_FLAG()                (MISC_FLAGS&=~PQ_BYPASS_BIT)
*/
#if ENABLE_DEINTERLACE
#define DEINTERLACE_BIT                     BIT5
#define DEINTERLACE_FLAG                    (MISC_FLAGS&DEINTERLACE_BIT)
#define SET_DEINTERLACE_FLAG()              (MISC_FLAGS|=DEINTERLACE_BIT)
#define CLR_DEINTERLACE_FLAG()              (MISC_FLAGS&=~DEINTERLACE_BIT)
#endif

#if ENABLE_DEMURA_FUNCTION
#define UNIFORMITY_COMPENSATION_BIT         BIT6
#define UNIFORMITY_COMPENSATION_FLAG        (MISC_FLAGS&UNIFORMITY_COMPENSATION_BIT)
#define SET_UNIFORMITY_COMPENSATION_FLAG()  (MISC_FLAGS|=UNIFORMITY_COMPENSATION_BIT)
#define CLR_UNIFORMITY_COMPENSATION_FLAG()  (MISC_FLAGS&=~UNIFORMITY_COMPENSATION_BIT)

#define DEMURA_MEASURE_STANDBY_BIT          BIT7
#define DEMURA_MEASURE_STANDBY_FLAG         (MISC_FLAGS&DEMURA_MEASURE_STANDBY_BIT)
#define SET_DEMURA_MEASURE_STANDBY_FLAG()   (MISC_FLAGS|=DEMURA_MEASURE_STANDBY_BIT)
#define CLR_DEMURA_MEASURE_STANDBY_FLAG()   (MISC_FLAGS&=~DEMURA_MEASURE_STANDBY_BIT)
#endif

#if ENABLE_SUPER_RESOLUTION
#define SUPER_RESOLUTION_BIT                BIT8
#define SUPER_RESOLUTION_FLAG               (MISC_FLAGS&SUPER_RESOLUTION_BIT)
#define SET_SUPER_RESOLUTION_FLAG()         (MISC_FLAGS|=SUPER_RESOLUTION_BIT)
#define CLR_SUPER_RESOLUTION_FLAG()         (MISC_FLAGS&=~SUPER_RESOLUTION_BIT)
#endif

#define YUV_COLOR_DOMAIN_BIT                BIT9
#define YUV_COLOR_DOMAIN_FLAG               (MISC_FLAGS&YUV_COLOR_DOMAIN_BIT)
#define SET_YUV_COLOR_DOMAIN_FLAG()         (MISC_FLAGS|=YUV_COLOR_DOMAIN_BIT)
#define CLR_YUV_COLOR_DOMAIN_FLAG()         (MISC_FLAGS&=~YUV_COLOR_DOMAIN_BIT)



#endif



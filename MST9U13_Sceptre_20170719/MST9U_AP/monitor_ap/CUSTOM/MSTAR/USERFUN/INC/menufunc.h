///////////////////////////////////////////////////////////////////////////////
/// @file menufunc.h
/// @brief Head file of Menufunc.c
/// @author MStarSemi Inc.
///
/// Functions response to OSD action.
///
/// Features
///  -
///
///////////////////////////////////////////////////////////////////////////////
#ifndef _MENUFUNC_H
#define _MENUFUNC_H

#include "Adjust.h"

#if (ZUI)
#define MENU_ITEM_ACTION  Bool
#else
#define MENU_ITEM_ACTION  MenuItemActionType
#endif

enum
{
    MENU_ITEM_ON = 0,
    MENU_ITEM_OFF
};

#if ENABLE_JUMP
enum
{
    LAN0    =0, // CLK
    LAN1    =1, // RX0
    LAN2    =2, // RX1
    LAN3    =3, // RX2
};
enum
{
    GPIO_OUT = 0,
    GPIO_IN  = 1,
};
enum
{
    RX_P = 0,
    RX_N = 1,
};

enum
{
    TMDS = 0,
    DVID = 1,
    DP = 2,
};

#define RESISTOR_MODE       0
#define CAPACITANCE_MODE    1

#define OPEN_SHORT_MODE     RESISTOR_MODE
#define CAP_MODE_RTY_CNT    100
#endif
extern Bool MWE_InitColorSetting(void);
extern void GetColorTempRGB(ColorType * pstColor, BYTE u8ColorTemp);

extern void SetIsrTimingChangeStatus(BYTE ucInputPort);
#if 1//SleepMode_Enable
extern Bool SetSleepMode(void);
extern Bool AdjustSleepModeValue(MENU_ITEM_ACTION enAction);
extern WORD GetSleepModeValue(void);
#endif
extern void Menu_InitAction(void);
extern Bool Is_MultiWin_SyncLoss(void);
extern Bool Is_DisplayOk_In_MultiWin(void);
extern  void Menu_SaveInputStatus(void);
extern WORD Get_MultiWin_Number(void );
extern void Set_MultiWin_InputTimingChangeFlag(void);

extern void SwitchPort( BYTE ucWinIndex, BYTE ucInputPort );
extern BYTE Get_AutoSource_NextPort(BYTE InputType);

#if Enable_AutoInput_Menu
extern BYTE Get_SourceSwitchType(void);
#endif

extern WORD GetAudioSourceValue(void);
extern BYTE GetOSDAudioInputType(BYTE InputPort);
extern Bool OSD_SetAudioSource( void );
extern void SetAudioSource( BYTE ucInputPort);
extern Bool AdjustAudioSourceValue(MENU_ITEM_ACTION enAction);
extern WORD DecIncValueCirculate(MENU_ITEM_ACTION enAction, WORD u16Value, WORD u16MinValue, WORD u16MaxValue, BYTE u8Step);
extern void SetModeChangeAudioMuteOnOff( void );

extern Bool SetGotoFactoryMenu(void);

extern Bool EnterStandbyOnMenu(void);
extern Bool EnterCheckCableMenu(void);
extern Bool EnterInputMenu(void);
extern Bool EnterSuspendOnMenu(void);

extern Bool PowerOffSystem(void);
extern Bool PowerOnSystem(void);

extern Bool EnterDisplayLogo(void);
//extern Bool EnterISPMode(void);
//extern BYTE GetSubWndRealContrast(BYTE ucContrast);

#if ADJ_BackLight_Enable
extern Bool AdjustrgbBrightness(MENU_ITEM_ACTION enAction);
extern WORD GetrgbBrightnessValue(void);
#endif

extern Bool AdjustContrast(MENU_ITEM_ACTION enAction);
extern WORD GetContrastValue(void);
extern Bool AdjustBrightness(MENU_ITEM_ACTION enAction);
extern WORD GetBrightnessValue(void);

extern Bool AdjustPhase(MENU_ITEM_ACTION enAction);
extern WORD GetPhaseValue(void);
extern Bool AdjustClock(MENU_ITEM_ACTION enAction);
extern WORD GetClockValue(void);
extern Bool AdjustHPosition(MENU_ITEM_ACTION enAction);
extern WORD GetHPositionValue(void);
extern Bool AdjustVPosition(MENU_ITEM_ACTION enAction);
extern WORD GetVPositionValue(void);

extern BYTE GetImageMode(BYTE u8ColorPresetMode);
extern WORD GetColorPresetValue(void);
//extern void msSwitchPressMode(BYTE u8CurrentImageMode );
extern Bool AdjustColorPresetMode(MENU_ITEM_ACTION enAction);
extern Bool SetColorPresetMode(void);

extern void mStar_AdjustACE_RGB(BYTE u8Red,BYTE u8Green,BYTE u8Blue);
extern Bool AdjustUserBlueColor(MENU_ITEM_ACTION enAction);
extern Bool AdjustUserGreenColor(MENU_ITEM_ACTION enAction);
extern Bool AdjustUserRedColor(MENU_ITEM_ACTION enAction);
extern WORD GetUserRedColorValue(void);
extern WORD GetUserGreenColorValue(void);
extern WORD GetUserBlueColorValue(void);

extern WORD GetHueValue(void);
extern Bool AdjustVideoHue(MENU_ITEM_ACTION enAction);
extern WORD GetSaturationValue(void);
extern Bool AdjustVideoSaturation(MENU_ITEM_ACTION enAction);

extern Bool AdjustOSDTimer(MENU_ITEM_ACTION enAction);
extern WORD GetOSDTimerValue(void);
extern WORD GetOSDTimerValue100(void);

//extern Bool ResetDisplaySettings(void);
extern Bool ResetPositionSetting(void);
extern Bool ResetColorSettingGraphics(void);
extern Bool ResetColorSettingVideo(void);

#if Enable_Lowbluray_FY
extern Bool SetLowbluray(void);
extern Bool AdjustLowbluray(MENU_ITEM_ACTION enAction);
extern WORD GetLowblurayValue(void);
#else
extern WORD GetLowblurayValue(void);
extern Bool AdjustLowbluray(MENU_ITEM_ACTION enAction);
#endif

extern WORD GetLowblurayValue_100(void);
extern WORD GetLowblurayValue_Number(void);

//extern Bool ResetColorSettingCustom(void);
extern Bool ResetColorSettingAll(void);
extern Bool ResetAllSetting(void);
extern Bool AutoConfig(void);
//extern Bool SwitchSource(void);
extern void SaveUserPref(void);

extern WORD GetLanguageValue(void);
extern Bool SetLanguage(void);
extern Bool AdjustLanguageValue(MENU_ITEM_ACTION enAction);
#if ENABLE_DP_INPUT
extern WORD GetDPConfigTypeValue(void);
extern void SetDPConfigType(void);
extern Bool OSD_SetDPConfigType(void);
extern Bool AdjustDPConfigType(MENU_ITEM_ACTION enAction);
#endif
extern Bool AdjustMenuTempValueFlipFlop(MENU_ITEM_ACTION enAction);
extern Bool AdjustInputValue(MENU_ITEM_ACTION enAction);
extern Bool SetColorDomain(void);
extern WORD GetColorDomainRealValue(void);

//extern WORD GetColorPresetModeValue(void);
extern Bool AdjustColorFormatValue(MENU_ITEM_ACTION enAction);
extern Bool SetInputColorFormatValue(void);
extern WORD GetInputColorFormatRealValue(void);

extern Bool SetGamma(void);
extern WORD GetGammaValue(void);
extern Bool AdjustGammaValue(MENU_ITEM_ACTION enAction);

extern void LoadGammaFunc(BYTE u8DispWin);

extern Bool SelectGain(void);
extern WORD GetSelectGainValue(void);
extern Bool AdjustGainVale(MENU_ITEM_ACTION enAction);

extern Bool SelectIndependentColor(void);
extern WORD GetSelectIndependentColorValue(void);
extern Bool AdjustIndependentColorVale(MENU_ITEM_ACTION enAction);

Bool SelectHueSaturation(void);
WORD GetSelectHueSaturationValue(void);

extern Bool SetColorTemperature(void);
extern WORD GetColorTemperatureValue(void);
extern Bool AdjustColorTemperature(MENU_ITEM_ACTION enAction);

//extern WORD GetColorSettingsModeValue(void);
//extern Bool SetColorSettingsMode(void);
extern Bool Menu_AdjustColorSetting(void);
//extern Bool SetColorPresetMode(void);

#if HDMI_VerSion_Enable
extern void SetHDMIConfigType(void);
extern Bool OSD_SetHDMIConfigType(void);
extern WORD GetHDMIConfigTypeValue(void);
extern Bool AdjustHDMIConfigType(MENU_ITEM_ACTION enAction);
#endif


extern WORD GetInputValue(void);

#if ENABLE_NOISE_REDUCTION
extern void SetNoiseReduction(void);
extern Bool Menu_SetNoiseReduction(void);
extern WORD GetNoiseReductionValue(void);
extern Bool AdjustNoiseReductionValue(MENU_ITEM_ACTION enAction);
#endif
#if ENABLE_DEINTERLACE
extern Bool SetDeinterlace(void);
extern WORD GetDeinterlaceValue(void);
#endif
#if ENABLE_DEMURA_FUNCTION
extern Bool SetUniformityCompensation(void);
extern WORD GetUniformityCompensationValue(void);
#endif
#if ENABLE_SUPER_RESOLUTION
extern WORD GetSuperResolutionValue(void);
extern Bool SetSuperResolutionFunction(void);
extern Bool AdjustSuperResolutionValue(MENU_ITEM_ACTION enAction);
extern Bool AdjustSuperResolution(void);
#endif
#if ENABLE_DLC
extern Bool SetDLCFunction(void);
extern WORD GetDLCValue(void);
#endif

extern Bool SetHotDCRFunction(void);

#if ENABLE_WIDE_MODE
extern void SetWideMode(void);
extern Bool Menu_SetWideMode(void);
extern WORD GetWideModeValue(void);
extern Bool AdjustWideModeValue(MENU_ITEM_ACTION enAction);
#endif
extern void SetPropFontIndex(void);

extern Bool AdjustResetValue(MENU_ITEM_ACTION enAction);

extern Bool Osd_Set_PiP_Position(MENU_ITEM_ACTION enAction);
extern Bool SetPIPPosition(void);
extern WORD GetPIPPositionValue(void);
extern void Set_PIP_Border(void);

#if ENABLE_MULTI_WINDOW_SETTINGS_MENU
extern Bool Menu_SetMultiWindowModes(void);
extern WORD GetMultiWindowModesValue(void);
extern Bool AdjustMultiWindowModesValue(MENU_ITEM_ACTION enAction);
extern Bool Menu_SetMultiWindowSelect(void);
extern WORD GetWindowSelectValue(void);
extern Bool AdjustMultiWindowSelectValue(MENU_ITEM_ACTION enAction);

extern Bool Menu_SetMultiWindow2Input(void);
extern WORD GetWindow2InputValue(void);
extern Bool AdjustMultiWindow2InputValue(MENU_ITEM_ACTION enAction);

extern Bool Menu_SetMultiWindow3Input(void);
extern WORD GetWindow3InputValue(void);
extern Bool AdjustMultiWindow3InputValue(MENU_ITEM_ACTION enAction);


extern Bool Menu_SetMultiWindow4Input(void);
extern WORD GetWindow4InputValue(void);
extern Bool AdjustMultiWindow4InputValue(MENU_ITEM_ACTION enAction);


extern Bool Menu_SetPIPSize(void);
extern WORD GetPIPSizeValue(void);
extern Bool AdjustPIPSizeValue(MENU_ITEM_ACTION enAction);
extern Bool AdjustPIPHPosition(MENU_ITEM_ACTION enAction);
extern WORD GetPIPHPositionValue(void);
extern Bool AdjustPIPVPosition(MENU_ITEM_ACTION enAction);
extern WORD GetPIPVPositionValue(void);
#endif //ENABLE_MULTI_WINDOW_SETTINGS_MENU


#if ENABLE_AUDIO_SETTINGS_MENU
extern Bool ResetAudioSettings(void);
extern Bool AdjustVolume(MENU_ITEM_ACTION enAction);
extern WORD GetVolumeValue(void);
#endif
#if AudioFunc
extern Bool OSD_SetAudioMuteOnOff(MENU_ITEM_ACTION enAction);
extern WORD GetMuteOnOffValue(void);
#endif
#if ENABLE_ADJ_AUDIO_OUTPUT
extern WORD GetAudioOutputValue(void);
extern Bool OSD_SetAudioOutput( void );
extern Bool AdjustAudioOutputValue(MENU_ITEM_ACTION enAction);
#endif


#if Free_Sync_Enable
extern Bool OSD_SetFreeSyncOnOff( void );
extern WORD GetFreeSyncOnOffValue(void);
extern Bool AdjustFreeSync(MENU_ITEM_ACTION enAction);

#endif

#if HDMI_HPD_Enable
extern WORD GetHDMIHPDRealValue(void);
extern Bool SetHDMIHPDFunction(void);
#endif

#if !Enable_AutoInput_Menu	
extern void Set_PMModeFlag( BYTE ePMMode );
#endif
extern Bool Is_PM_Mode_State(void);

#if USB_Updata_FW_Enable
extern WORD GetUSBUpdataFWStatus(void);
extern Bool SetUSBUpdataFW(void);
#endif


//extern WORD GetLCDConditionValue(void);
#if ENABLE_MMD_DCR
extern  void MF_DCRHandler(void);
extern  void SmartConntrastTimerHandler(void);
extern  void HighSmartContrastHandler(BYTE u8WinIndex);
#endif

extern Bool SetPIPPositionParameter(void);
extern void Set_PIP_Border(void);
extern void SetAudioMuteOnOff( void );

extern Bool Osd_Set_PiP_PbP_Swap( void );

#if ENABLE_DCR
extern void MF_DCRCheckBrightness(void);
extern void MF_DCRAdjustBrightness(BYTE u8Brightness);
//extern Bool SetDCRDisable(void);
extern WORD GetDCREnableRealValue(void);
//extern Bool AdjustDCREnableTempYesNo(MENU_ITEM_ACTION enAction);
extern Bool SetDCRFunction(void);
extern Bool SetDCRFunctionConfirm(void);
#endif

#if ENABLE_MULTI_INPUT
extern Bool SetInputSelect(void);
#endif
#if ENABLE_RTE
extern void mStar_SetRTE(void);
extern Bool SetRTEFunction(void);
extern WORD GetResponseTimeValue(void);
extern Bool AdjustResponseTimeValue(MENU_ITEM_ACTION enAction);

#endif
extern void SetSharpness(void);
extern Bool AdjustSharpness(MENU_ITEM_ACTION enAction);
extern WORD GetSharpnessValue(void);
extern Bool AdjustTransparency(MENU_ITEM_ACTION enAction);
extern WORD GetTransparencyValue(void);
extern WORD GetTransparencyGauge(void);

extern Bool AdjustOSDHPosition(MENU_ITEM_ACTION enAction);
extern WORD GetOSDHPositionValue(void);
extern Bool AdjustOSDVPosition(MENU_ITEM_ACTION enAction);
extern WORD GetOSDVPositionValue(void);


extern WORD InitOne(void);
#if ENABLE_OSD_ROTATION
extern WORD GetMenuRotationValue(void);
extern Bool SetMenuRotationValue(void);
extern Bool AdjustMenuRotationValue(MENU_ITEM_ACTION enAction);
#endif

#if Enable_AutoDownloadGamma
extern void TestLoadGamma(BYTE u8DispWin, BYTE GamaIdx);
#endif
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

extern Bool ClearHoldMenuKeyFlag(void);
extern Bool EnterDisplayInputHotMenu(void);
#if 1//INPUT_SAME_OUT
extern DWORD Get_TMDS_ClockRateType( BYTE InputPort );
extern Bool CheckPanelNormalTiming(void);
#endif

extern DWORD GetFreesyncFixClock(BYTE InputPort);
extern DWORD GetFreesyncHTotal(BYTE InputPort);

extern BYTE GetVfreqType( void );

#if LED_HOT_Menu_Func
extern WORD GetLedColorValue(void);
extern Bool SetLedColor(void);
extern Bool AdjustLedColorValue(MENU_ITEM_ACTION enAction);
extern Bool SetLedStrength(void);
extern Bool AdjustLedStrengthValue(MENU_ITEM_ACTION enAction);
extern Bool SetLedMode(void);
extern Bool AdjustLedModeValue(MENU_ITEM_ACTION enAction);
extern Bool SetLedModeConfirm(void);
extern Bool SetLedModeCancel(void);
extern void SetLedFunc(BYTE LedNumber, BYTE LedColor,BYTE LedStrength,BYTE LedColorMode);
extern Bool AdjustLedModeIndexValue(MENU_ITEM_ACTION enAction);

#endif

extern Bool SetHotInputSelect(void);

///
///<
//extern WORD GetMinOSDHPosition();
//extern WORD GetMaxOSDHPosition();
//extern WORD GetMinOSDVPosition();
//extern WORD GetMaxOSDVPosition();
///==========================================================


extern WORD GetScale100Value(WORD u16Value, WORD u16MinValue, WORD u16MaxValue);

extern void msAdjustHDMIVolume(BYTE volume);
//extern Bool SetGradualColor(void);
///-======================================

extern BYTE GetMainWndRealContrast( BYTE u8DispWin , BYTE u8Contrast,BYTE u8Color);
#define HUE_ANGLE   240

// Convert factory ADC digital gain to real reg value(0x80) is default
#if 1//(ODM_NAME==ODM_INL)
#define GET_REAL_GAIN(gain) (((WORD)(gain)*128+(MAX_COLOR_VALUE>>1))/MAX_COLOR_VALUE) //(((DWORD)(gain)*75*219+16384)>>15) //Jison simplify formula,result are the same
#else
#define GET_REAL_GAIN(gain) (((WORD)(gain)*75+64)>>7)
#endif

//IHC========================================================
extern WORD GetCustomHueValue_R(void);
extern WORD GetCustomHueValue_G(void);
extern WORD GetCustomHueValue_B(void);
extern WORD GetCustomHueValue_C(void);
extern WORD GetCustomHueValue_M(void);
extern WORD GetCustomHueValue_Y(void);

extern Bool AdjustCustomHue_R(MENU_ITEM_ACTION enAction);
extern Bool AdjustCustomHue_G(MENU_ITEM_ACTION enAction);
extern Bool AdjustCustomHue_B(MENU_ITEM_ACTION enAction);
extern Bool AdjustCustomHue_C(MENU_ITEM_ACTION enAction);
extern Bool AdjustCustomHue_M(MENU_ITEM_ACTION enAction);
extern Bool AdjustCustomHue_Y(MENU_ITEM_ACTION enAction);
// ICC=================================================
extern WORD GetCustomSATValue_R(void);
extern WORD GetCustomSATValue_G(void);
extern WORD GetCustomSATValue_B(void);
extern WORD GetCustomSATValue_C(void);
extern WORD GetCustomSATValue_M(void);
extern WORD GetCustomSATValue_Y(void);

extern Bool AdjustCustomSaturation_R(MENU_ITEM_ACTION enAction);
extern Bool AdjustCustomSaturation_G(MENU_ITEM_ACTION enAction);
extern Bool AdjustCustomSaturation_B(MENU_ITEM_ACTION enAction);
extern Bool AdjustCustomSaturation_C(MENU_ITEM_ACTION enAction);
extern Bool AdjustCustomSaturation_M(MENU_ITEM_ACTION enAction);
extern Bool AdjustCustomSaturation_Y(MENU_ITEM_ACTION enAction);



extern WORD GetCustomColorValue_R(void);
extern Bool AdjustCustomColor_R(MENU_ITEM_ACTION enAction);
extern WORD GetCustomColorValue_G(void);
extern Bool AdjustCustomColor_G(MENU_ITEM_ACTION enAction);
extern WORD GetCustomColorValue_B(void);
extern Bool AdjustCustomColor_B(MENU_ITEM_ACTION enAction);
extern WORD GetCustomColorValue_C(void);
extern Bool AdjustCustomColor_C(MENU_ITEM_ACTION enAction);
extern WORD GetCustomColorValue_M(void);
extern Bool AdjustCustomColor_M(MENU_ITEM_ACTION enAction);
extern WORD GetCustomColorValue_Y(void);
extern Bool AdjustCustomColor_Y(MENU_ITEM_ACTION enAction);


#if ENABLE_DEMURA_FUNCTION
extern void EnterDeMuraMeasureMode(void);
extern void CloseDeMuraMeasureMode(void);
#endif

#if ENABLE_JUMP
extern void msInputOpenShortTest(void);
extern void msLVDSOpenShortTest(void);
#endif

#if Enable_Corss_FY
extern WORD GetCrossModeValue(void);
extern Bool SetCrossMode(void);
extern Bool AdjustCrossModeValue(MENU_ITEM_ACTION enAction);
#endif

extern Bool Set_RotateType(void);
extern WORD GetRotateTypeValue(void);
extern Bool AdjustRotateType(MENU_ITEM_ACTION enAction);



#if ENABLE_DP_INPUT

extern void ApplyDaisyChainType(void);
#endif
void SetFreeSyncOnOffFunction(BYTE ucWinIndex, Bool bEnable, WORD uwDurationMs);

extern Bool Is_All_VGAPort_DisplayOk_In_MultiWin(void);

extern BYTE GetRealIhcValue(BYTE u8HueValue);
extern WORD GetProperHue(BYTE ucHue);
extern BYTE GetRealICCValue(BYTE u8SaturationValue);

enum
{
    eCT_SRGB,
    eCT_5000K,
    eCT_5700K,
    eCT_6500K,
    eCT_7500K,
    eCT_9300K,
    eCT_USER,    
    eCT_10000K,
    eCT_DEFAULT,
};


enum
{
    eIHC_R,
    eIHC_G,
    eIHC_B,
    eIHC_C,
    eIHC_M,
    eIHC_Y,
};

enum
{
#if 1
    eICC_R,
    eICC_G,
    eICC_B,
    eICC_C,
    eICC_M,
    eICC_Y,
#else
    eICC_R = 1,
    eICC_G = 2,
    eICC_B = 3,
    eICC_C = 4,
    eICC_M = 5,
    eICC_Y = 6,
#endif

};



#endif   ///_MENUFUNC_H

///////////////////////////////////////////////////////////////////////////////
/// @file MenuPropStr.h
/// @brief Head file of MenuPropStr.c
/// @author MStarSemi Inc.
///
/// Functions to get the (start address,length) of prop. strings.
///
/// Features
///  -
///////////////////////////////////////////////////////////////////////////////

#ifndef _MENUPROPSTR_H
#define _MENUPROPSTR_H

#include "menudef.h"
#include "msOSDDrawText.h"

#ifdef _MENUPROPSTR_C
#define INTERFACE
#else
#define INTERFACE   extern
#endif

///==============================================================

//INTERFACE BYTE* PowerSaveText(void);

INTERFACE BYTE* TxtAutoAdjustInProgress(void);


///------------------------------------------------
//09 OSD
//Common usage
INTERFACE BYTE* Txt_tPropText0(void);
//M0 group

INTERFACE BYTE* TxtColorFormatSettings(void);


//M2
INTERFACE BYTE* TxtMNoignal_01(void);
INTERFACE BYTE* TxtMNoignal_02(void);
//M3
INTERFACE BYTE* TxtM3PowerSave1(void);

//M4
INTERFACE BYTE* TxtM4CableNotConnected(void);
//INTERFACE BYTE* TxtNoCableMsg(void);
//INTERFACE BYTE* TxtTimer(void);
//INTERFACE BYTE GetTxtNoCableMsgLen(void);

//M5
INTERFACE BYTE* TxtM5OutOfRange1(void);
INTERFACE BYTE* TxtM5OutOfRange2(void);
INTERFACE BYTE* TxtM5OutOfRange3(void);

INTERFACE BYTE* TxtM5OutOfRangeSource(void);

INTERFACE BYTE  GetTxtM5OutOfRange2Len(void);

INTERFACE BYTE* TxtOOR_Resolution(void);

INTERFACE BYTE* MainFactoryText(void);




#if Enable_ECOGameMode
INTERFACE BYTE* TxtCM_FPS(void);
INTERFACE BYTE* TxtCM_RTS(void);
#endif





INTERFACE BYTE* TxtSleepMode(void);
INTERFACE BYTE* TxtSleepModeIndx(void);
INTERFACE BYTE* TxtSleepModeOFF(void); 
INTERFACE BYTE* TxtSleepMode10min(void) ;
INTERFACE BYTE* TxtSleepMode20min(void) ;
INTERFACE BYTE* TxtSleepMode30min(void) ;
INTERFACE BYTE* TxtSleepMode40min(void) ;
INTERFACE BYTE* TxtSleepMode50min(void) ;
INTERFACE BYTE* TxtSleepMode60min(void) ;
INTERFACE BYTE* TxtSleepMode120min(void); 

INTERFACE BYTE* TxtPowerOffMsg(void);
INTERFACE BYTE* TxtPowerOffS(void);



#if Enable_HotAdjLowbluray
INTERFACE BYTE* TxtLowBluRay(void);
#endif


INTERFACE BYTE* TxtMenuTimerOut(void);

#if HKC_RGB_Range_Func
INTERFACE BYTE* TxtUserPrefRGBRange(void);
#endif

//M108

#if USB_Updata_FW_Enable
INTERFACE BYTE* TxtUSBUpdataFW(void);
INTERFACE BYTE* TxtUSBUpdataFWStatus(void);
#endif


INTERFACE BYTE* AudioSourceText(void);



#if 0//LED_HOT_Menu_Func
INTERFACE BYTE* TxtLEDModeTitle(void);
INTERFACE BYTE* TxtLedColor(void);
INTERFACE BYTE* TxtLedColorMode(void);
INTERFACE BYTE* TxtLedStrength(void);
INTERFACE BYTE* TxtLedStrengthType(void);
INTERFACE BYTE* TxtLedMode(void);
INTERFACE BYTE* TxtLedModeType(void);
INTERFACE BYTE* TxtLedModeConfirm(void);
INTERFACE BYTE* TxtLedModeCancel(void);
#endif

#if Enable_Corss_LHCX_New
extern BYTE* HotKeyGameModetext(void);
extern BYTE* HotKeyGameSelectModetext(void);
extern BYTE* HotKeyGameClokModetext(void);
extern BYTE* HotKeyGameClokPositionModetext(void);

extern BYTE* HotKeyGameModeIcon(void);
extern BYTE* HotKeyGameClokIcon(void);

extern BYTE* HotKeyGameMode1Icon(void);
extern BYTE* HotKeyGameMode2Icon(void);
extern BYTE* HotKeyGameMode1SubIcon(void);
extern BYTE* HotKeyGameMode2SubIcon(void);

extern BYTE* HotKeyGameClok_30M_Icon(void);
extern BYTE* HotKeyGameClok_40M_Icon(void);
extern BYTE* HotKeyGameClok_50M_Icon(void);
extern BYTE* HotKeyGameClok_60M_Icon(void);
extern BYTE* HotKeyGameClok_90M_Icon(void);

extern BYTE* HotKeyGameClokPosition_UpIcon(void);
extern BYTE* HotKeyGameClokPosition_CenterIcon(void);
extern BYTE* HotKeyGameClokPosition_DownIcon(void);

extern BYTE* HotKeyGameClokShowIcon(BYTE number);
#endif

extern BYTE* TxtMainQSDescription_1(void);
extern BYTE* TxtMainQSDescription_2(void);
extern BYTE* TxtMainQSDescription_3(void);
extern BYTE* TxtMainQSDescription_4(void);

extern BYTE* TxtMainPicDescription_1(void);
extern BYTE* TxtMainPicDescription_2(void);
extern BYTE* TxtMainPicDescription_3(void);
extern BYTE* TxtMainPicDescription_4(void);

extern BYTE* TxtMainColDescription_1(void);
extern BYTE* TxtMainColDescription_2(void);
extern BYTE* TxtMainColDescription_3(void);
extern BYTE* TxtMainColDescription_4(void);

extern BYTE* TxtMainSysDescription_1(void);
extern BYTE* TxtMainSysDescription_2(void);
extern BYTE* TxtMainSysDescription_3(void);
extern BYTE* TxtMainSysDescription_4(void);



extern BYTE* TxtDescription_1(void);
extern BYTE* TxtDescription_2(void);
extern BYTE* TxtDescription_3(void);
extern BYTE* TxtMenu(void);
extern BYTE* TxtUp(void);
extern BYTE* TxtDown(void);
extern BYTE* TxtExit(void);
extern BYTE* TxtPower(void);

extern BYTE* TxtQuickBackLight(void);
extern BYTE* TxtQuickBrightness(void);
extern BYTE* TxtQuickContrast(void);
extern BYTE* TxtQuickPresetMode(void);
extern BYTE* TxtQuickAspectRatio(void);
extern BYTE* TxtQuickSource(void);

extern BYTE* TxtQuickBlueLight(void);
extern BYTE* TxtQuickFreeSync(void);
extern BYTE* TxtQuickFreeSyncOnOff(void);
extern BYTE* TxtQuickAspectRatioStatus(void);
extern BYTE* TxtQuickPresetModeStatus(void);
extern BYTE* TxtQuickBlueLightStatus(void);
extern BYTE* TxtQuickSourceStatus(void);

extern BYTE* TxtBackLightDes01(void);
extern BYTE* TxtBackLightDes02(void);
extern BYTE* TxtBrightnessDes01(void);
extern BYTE* TxtBrightnessDes02(void);
extern BYTE* TxtContrastDes01(void);
extern BYTE* TxtContrastDes02(void);
extern BYTE* TxtPresetDes01(void);
extern BYTE* TxtPresetDes02(void);
extern BYTE* TxtAspectDes01(void);
extern BYTE* TxtAspectDes02(void);
extern BYTE* TxtBlueLightDes01(void);
extern BYTE* TxtBlueLightDes02(void);
extern BYTE* TxtFreeSyncDes01(void);
extern BYTE* TxtFreeSyncDes02(void);

extern BYTE* TxtDCRDes01(void);
extern BYTE* TxtDCRDes02(void);
extern BYTE* TxtSharpnessDes01(void);
extern BYTE* TxtSharpnessDes02(void);
extern BYTE* TxtColorTempDes01(void);
extern BYTE* TxtColorTempDes02(void);
extern BYTE* TxtGammaDes01(void);
extern BYTE* TxtGammaDes02(void);
extern BYTE* TxtTintDes01(void);
extern BYTE* TxtTintDes02(void);
extern BYTE* TxtSaturationDes01(void);
extern BYTE* TxtSaturationDes02(void);
extern BYTE* TxtODDes01(void);
extern BYTE* TxtODDes02(void);
extern BYTE* TxtSleepModeDes01(void);
extern BYTE* TxtSleepModeDes02(void);
extern BYTE* TxtLanguageDes01(void);
extern BYTE* TxtLanguageDes02(void);
extern BYTE* TxtOSDHDes01(void);
extern BYTE* TxtOSDHDes02(void);
extern BYTE* TxtOSDVDes01(void);
extern BYTE* TxtOSDVDes02(void);
extern BYTE* TxtOSDTimerDes01(void);
extern BYTE* TxtOSDTimerDes02(void);
extern BYTE* TxtOSDTransDes01(void);
extern BYTE* TxtOSDTransDes02(void);
extern BYTE* TxVolumeDes01(void);
extern BYTE* TxVolumeDes02(void);
extern BYTE* TxResetDes01(void);
extern BYTE* TxResetDes02(void);

extern BYTE* TxLedModeAllDes01(void);
extern BYTE* TxLedModeAllDes02(void);
extern BYTE* TxLedModeDes01(void);
extern BYTE* TxLedModeDes02(void);
extern BYTE* TxLedColorDes01(void);
extern BYTE* TxLedColorDes02(void);
extern BYTE* TxLedBrightnessDes01(void);
extern BYTE* TxLedBrightnessDes02(void);


extern BYTE* TxtColorTempAdvancedDes01(void);
extern BYTE* TxtColorTempAdvancedDes02(void);

extern BYTE* TxtSourceDes01(void);
extern BYTE* TxtSourceDes02(void);
#if LED_HOT_Menu_Func
extern BYTE* TxtSystemLedModeIndexStatus(void);
#endif
///////


extern BYTE* TxtPictureBackLight(void);
extern BYTE* TxtPictureContrast(void);
extern BYTE* TxtPictureBrightness(void);
extern BYTE* TxtPicturePresetMode(void);
extern BYTE* TxtPictureDCR(void);
extern BYTE* TxtPictureAspectRatio(void);
extern BYTE* TxtPictureSharpness(void);
extern BYTE* TxtPicturePresetModeStatus(void);
extern BYTE* TxtPictureDCRStatus(void);
extern BYTE* TxtPictureAspectRatioStatus(void);
extern BYTE* TxtAuto_Adjust(void);
extern BYTE* TxtAuto_Color(void);
extern BYTE* TxtH_Position(void);
extern BYTE* TxtV_Position(void);
extern BYTE* TxtClock(void);
extern BYTE* TxtPhase(void);



extern BYTE* TxtColorTemperature(void);
extern BYTE* TxtColorGamma(void);
extern BYTE* TxtColorTint(void);
extern BYTE* TxtColorSaturation(void);
extern BYTE* TxtColorBlueLight(void);
extern BYTE* TxtColorTemperatureStatus(void);
extern BYTE* TxtColorRed(void);
extern BYTE* TxtColorGreen(void);
extern BYTE* TxtColorBlue(void);
extern BYTE* TxtColorCyan(void);
extern BYTE* TxtColorMagenta(void);
extern BYTE* TxtColorYellow(void);
extern BYTE* TxtColorGammaStatus(void);
extern BYTE* TxtColorBlueLightStatus(void);

extern BYTE* TxtAdvanced(void);


extern BYTE* TxtSystemOverDrive(void);
extern BYTE* TxtSystemFreeSync(void);
extern BYTE* TxtSystemSleepMode(void);
extern BYTE* TxtSystemLedMode(void);

extern BYTE* TxtSystemLanguage(void);
extern BYTE* TxtSystemOSDHPosition(void);
extern BYTE* TxtSystemOSDVPosition(void);
extern BYTE* TxtSystemOSDTimer(void);
extern BYTE* TxtSystemOSDTransparency(void);
extern BYTE* TxtSystemVolume(void);
extern BYTE* TxtSystemReset(void);
extern BYTE* TxtSystemConfirm(void);
extern BYTE* TxtSystemCancel(void);
extern BYTE* TxtSystemLanguageStatus(void);
extern BYTE* TxtSystemOverDriveStatus(void);
extern BYTE* TxtSystemFreeSyncStatus(void);
extern BYTE* TxtSystemSleepModeStatus(void);

extern BYTE* TxtSystemResetStatus(void);
extern BYTE* TxtSystemTransStatus(void);

extern BYTE* TxtSystemLedModeText(void);
extern BYTE* TxtSystemLedModeIcon(void);

#if LED_HOT_Menu_Func
extern BYTE* TxtSystemLedColor(void);
extern BYTE* TxtSystemLedBrightness(void);
extern BYTE* TxtSystemLedMode(void);
extern BYTE* TxtSystemLedColorStatus(void);
extern BYTE* TxtSystemLedBrightnessStatus(void);
extern BYTE* TxtSystemLedModeStatus(void);
#endif

#undef INTERFACE
#endif   ///_MENUPROPSTR_H




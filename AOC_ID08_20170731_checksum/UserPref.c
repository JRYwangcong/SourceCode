#include <math.h>
#include "types.h"
#include "board.h"
#include "global.h"
#include "nvram.h"
#include "debug.h"
#include "menudef.h"
#include "menufunc.h"
#include "mcu.h"
#if MWEFunction
    #include "MsDLC.h"
    #include "MsACE.h"
    #include "UserPref.h"
#endif 
#if 1//USEFLASH
	#include "msflash.h"
	//Bool IsFlashSettingAreaEmpty(BYTE EmptyValue);//edit by lizzie.ding ,for check flash empty
#endif 

#if EnablePanelServiceMenu
#include "panel.h"
#include "ms_reg.h"
#endif
#ifdef TSUMXXT    //110401 Modify
#include "MsADC.h"
#endif

#define HFreq_Tol   16 // 1.6 KHz
#define VFreq_Tol   8  // 0.8 Hz

#if USEFLASH
BYTE xdata ucFlashSaveState;	// Save user Setting flag
//WORD xdata ucFlashSaveCount;	// Flash Save counter
#endif

#if !USEFLASH
void CheckBoard(void);
void ReadBlacklitTime(void);
#endif
Bool IsEEPRomEmpty(BYTE EmptyValue);

//Benz 2007.4.24   23:25:55 void SaveBlacklitTime( void );
#if EnablePanelServiceMenu
void Init_PanelSetting(void)
{
    //PanelDither=_PanelDither;
    PanelSettingFlag=_PanelColorDepth|_ChangeModePanelVddOff;

    PanelOnTiming1=_PanelOnTiming1;
    PanelOnTiming2=_PanelOnTiming2;
    PanelOffTiming1=_PanelOffTiming1;
    PanelOffTiming2=_PanelOffTiming2;
    
    PanelHSyncWidth=_PanelHSyncWidth;
    PanelHSyncBackPorch=_PanelHSyncBackPorch;
    PanelVSyncWidth=_PanelVSyncWidth;
    PanelVSyncBackPorch=_PanelVSyncBackPorch;
    PanelWidth=_PanelWidth;
    PanelHeight=_PanelHeight;
    PanelHTotal=_PanelHTotal;
    PanelVTotal=_PanelVTotal;
    PanelMaxHTotal=_PanelMaxHTotal;
    PanelMinHTotal=_PanelMinHTotal;
   /* PanelMaxVTotal	=_PanelMaxVTotal;
    PanelMinVTotal=_PanelMinVTotal;*/
    
    PanelDCLK=_PanelDCLK;
    PanelMaxDCLK=_PanelMaxDCLK;
    PanelMinDCLK=_PanelMinDCLK;
    PanelLVDSSwing=_PanelLVDSSwing;

    PanelOutputControl1=_PanelOutputControl1;	
    PanelOutputControl2=_PanelOutputControl2;	
    /*PanelOSContol=_PanelOSContol;		
    PanelODRriving=_PanelODRriving;*/		

    PanelBurstModeFreq=_DefBurstModeFreq;		
    PanelMinDuty=_DefMinDutyValue;		
    PanelMaxDuty=_DefMaxDutyValue;		
    PanelDefDuty=((DWORD)9 *(_DefMaxDutyValue - _DefMinDutyValue)+5) / 10+ _DefMinDutyValue;//Set default duty to 90%		
    
    PanelSSCSTEPL=_STEPL;
    PanelSSCSTEPH=_STEPH;
    PanelSSCSPANL=_SPANL;
    PanelSSCSPANH	=_SPANH;
    #if PanelSettingInFlash
    PanelSetting.PanelTypeNum=UseDefaultPanelValue;
    #endif
    PanelSetting.CheckSum=CalculateSettingCheckSum((BYTE *)(&PanelSetting)+PanelSettingChecksumSize, PanelSettingSize-PanelSettingChecksumSize);//FLASH_CHECKSUM; PanelSetting.CheckSum=FLASH_CHECKSUM;PanelSetting.Ident=FLASH_IDENTDATA;
	
  
}
WORD CalculateSettingCheckSum(BYTE* Array, BYTE Len)
{
	BYTE i;
	WORD checksum=0;
	for(i=0;i<Len;i++)
		checksum+=Array[i];
	return checksum;
}
#if EnablePanelServiceMenu
#if PanelSettingInFlash
void SavePanelSetting(void)
{ 
	#if 0//Enable_Cache
	MCU_DisableCache();
	#endif
	{
    //PanelSetting.Ident=FLASH_IDENTDATA;
    //PanelSetting.CheckSum=FLASH_CHECKSUM;
    	PanelSetting.CheckSum=CalculateSettingCheckSum((BYTE *)(&PanelSetting)+PanelSettingChecksumSize, PanelSettingSize-PanelSettingChecksumSize);
	Flash_Write_Factory_KeySet(FLASH_KEY_PANEL, 0x00, PanelSettingSize, (BYTE*)&PanelSetting, PanelSettingSize);
	}
	//Flash_WritePanelSet((BYTE*)&PanelSetting, PanelSettingSize);
	#if 0//Enable_Cache
	MCU_EnableCache();
	#endif
	#if 0//DEBUG_ENABLE
	    printMsg("Save Panel Setting");
	#endif
}
#else
void SavePanelSetting(void)
{ 
    NVRam_WriteTbl(PanelSettingAddr, (BYTE*) &PanelSetting, PanelSettingSize);
    #if 0//DEBUG_ENABLE
        printMsg("Save Panel Setting");
    #endif
}
#endif
Bool CheckPanelSettingRange(void)
{
    if(PanelOnTiming1>MaxPanelOnTiming1||PanelOffTiming2>MaxPanelOffTiming2\
		||PanelOnTiming2>MaxPanelOnTiming2||PanelOffTiming1>MaxPanelOffTiming1)
        return TRUE;

    if(PanelMinDCLK<20||PanelMaxDCLK>200||PanelMinDCLK>PanelMaxDCLK)
        return TRUE;

    if(PanelBurstModeFreq<MinBurstModeFreqValue||PanelBurstModeFreq>MaxBurstModeFreqValue)
        return TRUE;

    if(PanelMinDuty>PanelDefDuty||PanelMinDuty>PanelMaxDuty||PanelDefDuty>PanelMaxDuty)
        return TRUE;
	
    if(PanelSSCSTEPH>0x07||PanelSSCSPANH>0x07)
        return TRUE;
	
    return FALSE;
}

#endif

#endif

Bool CheckFactorySettingOutOfRange(void)
{
	#ifdef TSUMXXT//(ChipID == ChipTSUMT)  	//110401 Modify
    if (UserPrefAdcPgaGain > 0x2A)
        return TRUE;
    if(
        UserPrefAdcRedGain >= 0x600 ||
        UserPrefAdcGreenGain >= 0x600 ||
        UserPrefAdcBlueGain >= 0x600 ||
        UserPrefAdcRedGain <= 0x200 ||
        UserPrefAdcGreenGain <= 0x200 ||
        UserPrefAdcBlueGain <= 0x200 
        )
        return TRUE;
    if( (UserPrefAdcRedOffset != 0x00 && UserPrefAdcRedOffset != 0x800-OffsetValue)||
        (UserPrefAdcBlueOffset != 0x00 && UserPrefAdcBlueOffset != 0x800-OffsetValue)||
        (UserPrefAdcGreenOffset != 0x00 && UserPrefAdcGreenOffset != 0x800-OffsetValue)
      	)
        return TRUE;
	#else
    if(
        UserPrefAdcRedGain <= 50||
        UserPrefAdcGreenGain <= 50||
        UserPrefAdcBlueGain <= 50||
        UserPrefAdcRedOffset <= 50||
        UserPrefAdcGreenOffset <= 50||
        UserPrefAdcBlueOffset <= 50
        )
        return TRUE;
    if(
        UserPrefAdcRedGain >= 200||
        UserPrefAdcGreenGain >= 200||
        UserPrefAdcBlueGain >= 200||
        UserPrefAdcRedOffset >= 200||
        UserPrefAdcGreenOffset >=200||
        UserPrefAdcBlueOffset >= 200
        )
        return TRUE;
	#endif
/*	
    if(
	 FUserPrefBrightnessWarm1 != FUserPrefBrightnessCool1||
        FUserPrefBrightnessCool1 != FUserPrefBrightnessNormal||
        FUserPrefBrightnessNormal != 
        #if USEFLASH
        FUserPrefBrightnessSRGB||
        FUserPrefBrightnessSRGB != 
        #else
        UserPrefBrightnessSRGB||
        UserPrefBrightnessSRGB != 
        #endif
        FUserPrefBrightnessWarm1
       )
        return TRUE;
    if(FUserPrefContrastWarm1 != FUserPrefContrastCool1||
		FUserPrefContrastCool1 != FUserPrefContrastNormal||
		FUserPrefContrastNormal != 
		#if USEFLASH
		FUserPrefContrastSRGB||
		FUserPrefContrastSRGB != 
	        #else
		UserPrefContrastSRGB||
		UserPrefContrastSRGB != 
		#endif
		FUserPrefContrastWarm1
	)
        return TRUE;
		*/
    if(
        UserPrefRedColorCool1 <= 50||
        UserPrefGreenColorCool1 <= 50||
        UserPrefBlueColorCool1 <= 50||
        UserPrefRedColorNormal <= 50||
        UserPrefGreenColorNormal <= 50||
        UserPrefBlueColorNormal <= 50||
        UserPrefRedColorWarm1 <= 50||
        UserPrefGreenColorWarm1 <= 50||
        UserPrefBlueColorWarm1 <= 50||
        UserPrefRedColorSRGB <= 50||
        UserPrefGreenColorSRGB <= 50||
        UserPrefBlueColorSRGB <= 50
        )
        return TRUE;
    if(
	#if PANEL_VCOM_ADJUST		//121031 Modify
        FactorySetting.VcomValue>MAX_VCOM_VALUE||
        FactorySetting.VcomValue<MIN_VCOM_VALUE||
	#endif
        abs(UserPrefRedColorCool1-UserPrefRedColorNormal) >= 100||
        abs(UserPrefRedColorNormal-UserPrefBlueColorCool1)  >= 100||
        abs(UserPrefRedColorCool1-UserPrefBlueColorCool1)  >= 100||

        abs(UserPrefRedColorNormal-UserPrefGreenColorNormal) >= 100||
        abs(UserPrefGreenColorNormal-UserPrefBlueColorNormal)  >= 100||
        abs(UserPrefRedColorNormal-UserPrefBlueColorNormal)  >= 100||

        abs(UserPrefRedColorWarm1-UserPrefGreenColorWarm1) >= 100||
        abs(UserPrefGreenColorWarm1-UserPrefBlueColorWarm1)  >= 100||
        abs(UserPrefRedColorWarm1-UserPrefBlueColorWarm1)  >= 100||

        abs(UserPrefRedColorSRGB-UserPrefGreenColorSRGB) >= 100||
        abs(UserPrefGreenColorSRGB-UserPrefBlueColorSRGB)  >= 100||
        abs(UserPrefRedColorSRGB-UserPrefBlueColorSRGB)  >= 100
        )
        return TRUE;
	
    #if USEFLASH
        if(FactoryCheckSum!=FLASH_CHECKSUM)
        return TRUE;
    #endif
	return FALSE;
}

#if USEFLASH
void Init_FactorySetting(void)
{
	#ifdef TSUMXXT//#if (ChipID == ChipTSUMT)	    //110401 Modify
    	UserPrefAdcPgaGain = 0x00;  	//110401 Modify
	UserPrefAdcRedGain=0x400;		//110314 Modify
	UserPrefAdcGreenGain=0x400;		//110314 Modify
	UserPrefAdcBlueGain=0x400;		//110314 Modify

	UserPrefAdcRedOffset=0x00;  	//110401 Modify
	UserPrefAdcGreenOffset=0x00;  	//110401 Modify
	UserPrefAdcBlueOffset=0x00;  	//110401 Modify
	#else
    UserPrefAdcRedGain = DefRedGain;
    UserPrefAdcGreenGain = DefGreenGain;
    UserPrefAdcBlueGain = DefBlueGain;
    UserPrefAdcRedOffset = DefRedOffset;
    UserPrefAdcGreenOffset = DefGreenOffset;
    UserPrefAdcBlueOffset = DefBlueOffset;
	#endif
    FUserPrefBrightnessWarm1 = DefBrightness;
    FUserPrefContrastWarm1 = DefContrast;
    UserPrefRedColorWarm1 = DefWarm_RedColor;
    UserPrefGreenColorWarm1 = DefWarm_GreenColor;
    UserPrefBlueColorWarm1 = DefWarm_BlueColor;
    FUserPrefBrightnessCool1 = DefBrightness;
    FUserPrefContrastCool1 = DefContrast;
    UserPrefRedColorCool1 = DefCool_RedColor;
    UserPrefGreenColorCool1 = DefCool_GreenColor;
    UserPrefBlueColorCool1 = DefCool_BlueColor;
    FUserPrefBrightnessNormal = DefBrightness;
    FUserPrefContrastNormal = DefContrast;
    UserPrefRedColorNormal = DefNormal_RedColor;
    UserPrefGreenColorNormal = DefNormal_GreenColor;
    UserPrefBlueColorNormal = DefNormal_BlueColor;
    UserPrefRedColorSRGB = DefsRGB_RedColor;
    UserPrefGreenColorSRGB = DefsRGB_GreenColor;
    UserPrefBlueColorSRGB = DefsRGB_BlueColor;
    FUserPrefBrightnessSRGB = DefBrightness;
    FUserPrefContrastSRGB = DefContrast;
    FactoryCheckSum=FLASH_CHECKSUM;
    FactoryProductModeValue=ProductModeOffValue;
    #if PANEL_VCOM_ADJUST		//121031 Modify
    UserPrefVcomValue = DEF_VCOM_VALUE;
	#elif EnableVcomAdjustItem//def Panel_RSDS
    UserPrefVcomValue = DefVcomValue;
    #endif
    LogoON=1;

	#if PANELSSCSTEPLFUN
	PanelSSCSTEPL=_STEPL;
	PanelSSCSTEPH=_STEPH;
	PanelSSCSPANL=_SPANL;
	PanelSSCSPANH=_SPANH;
	#endif

	
    //#ifdef SETTING_IN_FLASH_12K
    FactorySetting.Ident=FLASH_IDENTDATA;
    FactorySetting.CheckSum=FLASH_CHECKSUM;
    //#endif
}

void Init_MonitorSetting(void)
{
#if DEBUG_ENABLE
printMsg("Init_MonitorSetting");
#endif
    UserPrefBrightness = DefBrightness;
    UserPrefContrast = DefContrast;
    UserPrefRedColor = DefRedColor;
    UserPrefGreenColor = DefGreenColor;
    UserPrefBlueColor = DefBlueColor;
    UserPrefBrightnessUser = DefBrightness;
    UserPrefContrastUser = DefContrast;
    UserPrefRedColorUser = DefRedColor;
    UserPrefGreenColorUser = DefGreenColor;
    UserPrefBlueColorUser = DefBlueColor;
    #if Enable_YMC
    UserPrefYColor = DefRedColor;
    UserPrefCColor = DefGreenColor;
    UserPrefMColor = DefBlueColor;
    #endif
    UserPrefBrightnessWarm1 = DefBrightness;
    UserPrefContrastWarm1 = DefContrast;
    UserPrefBrightnessCool1 = DefBrightness;
    UserPrefContrastCool1 = DefContrast;
    UserPrefBrightnessNormal = DefBrightness;
    UserPrefContrastNormal = DefContrast;
    UserPrefBrightnessSRGB = DefBrightness;
    UserPrefContrastSRGB = DefContrast;
    UserPrefECOMode = 0;
    UserPrefColorTemp = DEF_CTEMP;
    // 2006/10/26 6:49PM by Emily     UserPrefSharpness = 2;

    UserPrefLanguage = DEF_OSD_LANG;
    UserPrefVolume = 50;
    UserPrefOsdHStart = 50;
    UserPrefOsdVStart = 50;
    UserPrefOsdTime = 10;
	UserPrefOsdTrans= 0;
	#ifdef OffPower      //090331 xiandi.yu Auto Off Power
    UserPrefAutoPowerOff=0;	
	#endif
    UserPrefDcrMode = 0;
    UserPrefGamaMode = 0;
    
    #if DefaultIconExit//AOC_37_Series || ModelName==ENVISION_P2071l
    UserPrefLastMenuIndex = 8;
    #else
    UserPrefLastMenuIndex = 0;
    #endif
    
    UserPrefInputType = Input_Analog1;
    #if INPUT_TYPE == INPUT_1A
    UserPrefInputSelectType = InputSelectType_Analog; 		
    #else
    UserPrefInputSelectType = InputSelectType_Auto; // 071221 modified for initial UserPrefInputSelectType
    #endif	
	UserPrefSavedModeIndex = NumberOfMode;
    #if DDCCI_ENABLE
        UserprefRedBlackLevel = 50;
        UserprefGreenBlackLevel = 50;
        UserprefBlueBlackLevel = 50;
    #endif 
    #if Enable_Expansion
        UserprefExpansionMode = DefExpansion;
    #endif 
    #if MWEFunction
        //UserPrefMWE=OFF_W;
        UserPrefHue = DefHue;
        UserPrefSaturation = DefSaturation;
        UserPrefSubContrast = DefSubContrast;
        UserPrefSubBrightness = DefSubBrightness;
        // 2006/10/26 6:47PM by Emily     UserPrefSubSharpness = 11;
        //UserPrefSkinTone=OFF_SKIN;
        UserPrefColorFlags = 0;
        // 2006/10/26 6:48PM by Emily     UserPrefDLCMode = OFF_DLC;
        // UserPrefBFPosition =0;               
        UserPrefBFSize = 1;
        UserPrefBFVSize = 0;
        UserPrefBFHStart = 0;
        UserPrefBFVStart = 0;
    #endif 
    //=======for AutoBacklight=====================
    UserprefHistogram1 = 0x40;
    UserprefHistogram2 = 0xB0;
    UserprefALha = 50;
    UserprefBata = 50;
    // 2006/11/6 5:34PM by Emily      UserprefAutoModel=1;
    // 2006/11/6 5:33PM by Emily      UserprefCompareRange=8;
    // 2006/11/6 5:33PM by Emily      UserprefDiffHvalue=0x80;
    #if MS_OVD
	    MonitorFlags = bPowerOnBit | bBurninModeBit | bDCCCIONOBit |bOverDriveOnBit; //Benz 2007.4.23   20:32:00]
    #else
	    MonitorFlags = bPowerOnBit | bBurninModeBit |bFactoryModeBit| bDCCCIONOBit; //Benz 2007.4.23   20:32:00
    #endif
    #if USEFLASH
    MonitorBackLightTime= 0;
    #endif

    #if Enable_Lightsensor
    UserPrefiCareMode = LightSensorOff;
    #endif


#if VGA_EDID_Write_To_24C02
	Clr_VGA_EDID_Flag();
	//Set_VGA_EDID_Flag();
#endif

	

    #ifdef EnableLightBar
    UserPrefLightBar = DefLightBarValue;
    #endif
    
    MonitorSetting.checksum = FLASH_CHECKSUM;
}
void Init_ModeSetting(void)
{
    UserPrefHTotal = StandardModeHTotal;
    UserPrefHStart = StandardModeHStart;
    UserPrefVStart = StandardModeVStart;
    UserPrefAutoHStart = UserPrefHStart;
    UserPrefAutoVStart = StandardModeVStart; //2004/01/15 num11
    UserPrefPhase = 18;
    UserPrefAutoTimes = 0;
#if USEFLASH
        ModeSetting.Index=SaveIndex;
        ModeSetting.Ident=FLASH_IDENTDATA;
        ModeSetting.checksum=FLASH_CHECKSUM;
	 ModeSetting.HerFreq=0xFFFF;
	 ModeSetting.VerFreq=0xFFFF;
	 ModeSetting.VTotal=0xFFFF;
	 ModeSetting.Flags=0xFF;
#else
        ModeSetting.CheckSum=FLASH_CHECKSUM;
#endif
}
void CheckColorValueRange(void)
{
    if (UserPrefContrast > MaxContrastValue || UserPrefContrast < MinContrastValue)
    {
        UserPrefContrast = DefContrast;
    }
    #if 1
        if (UserPrefRedColor > MaxColorValue)
            UserPrefRedColor = DefRedColor;
        if (UserPrefGreenColor > MaxColorValue)
            UserPrefGreenColor = DefGreenColor;
        if (UserPrefBlueColor > MaxColorValue)
            UserPrefBlueColor = DefBlueColor;
    #endif 
}
Bool CheckMonitorSettingOutOfRange(void)
{	MSBankNo%=3;
    if(
        UserPrefBrightness > MaxBrightnessValue||
        UserPrefContrast > MaxContrastValue ||
        UserPrefContrast < MinContrastValue ||
        UserPrefECOMode  >= ECO_Nums||
        UserPrefColorTemp >= CTEMP_Nums||
        #if AudioFunc
        UserPrefVolume  > 100||
        #endif
        UserPrefLanguage >= LANG_Nums ||
        UserPrefOsdHStart > 100||
        UserPrefOsdVStart > 100||
        UserPrefOsdTime > 120||
        UserPrefOsdTrans >4 ||
        UserPrefBFSize > 7||
        UserPrefSubBrightness<MinSubBrightness||
        UserPrefSubBrightness>MaxSubBrightness||
        UserPrefSubContrast < MinSubContrast||
        UserPrefSubContrast > MaxSubContrast||
        UserPrefDcrMode > 2 ||
        UserPrefGamaMode >= GAMA_Nums|| 
        UserPrefInputType>=Input_Nums ||
        UserPrefHue > 100 ||
        UserPrefBFSize > 7||
        MSBankNo> 2
    #if Enable_Lightsensor
        || UserPrefiCareMode > LightSensorOn
    #endif
        )
        return TRUE;
    return FALSE;
}

#if 0  //0729
    void CheckModeSettingRange(void)
    {
        if (abs(UserPrefHTotal - StandardModeHTotal) > ClockAdjRange)
            UserPrefHTotal = StandardModeHTotal;
        if (UserPrefHStart > 2 *StandardModeHStart)
            UserPrefHStart = StandardModeHStart;
        if (UserPrefVStart > 2 *StandardModeVStart)
            UserPrefVStart = StandardModeVStart;
        UserPrefPhase &= 0x3F;
        if (UserPrefAutoTimes == 0 || abs(UserPrefHStart - UserPrefAutoHStart) > 50)
            UserPrefAutoHStart = UserPrefHStart;
    }
#endif 
void CheckModeSettingRange(void)
{
    if (abs(UserPrefHTotal - StandardModeHTotal) > 100)
    {
        UserPrefHTotal = StandardModeHTotal;
    }
    //  if (UserPrefHStart>2*StandardModeHStart)
    //    UserPrefHStart=StandardModeHStart;
    //  if (UserPrefVStart>2*StandardModeVStart)
    //    UserPrefVStart=StandardModeVStart;
    if (abs(UserPrefHStart - UserPrefAutoHStart) > 50)
    //2004/02/10 num1
    {
        UserPrefHStart = UserPrefAutoHStart;
    } //2004/02/10 num1
    if (UserPrefVStart > 2 *UserPrefAutoVStart)
    //2004/02/10 num1
    {
        UserPrefVStart = UserPrefAutoVStart;
    } //2004/02/10 num1
    #ifdef TSUMXXT		//110413 Modify
    UserPrefPhase &= 0x7F;
	#else
    UserPrefPhase &= 0x3F;
	#endif
    if (/*UserPrefAutoTimes == 0 ||*/ abs(UserPrefHStart - UserPrefAutoHStart) > 50)
    {
        UserPrefAutoHStart = UserPrefHStart;
    }
}

void UserPref_EnableFlashSaveBit(BYTE ucFlag)
{
    ucFlashSaveState |= ucFlag;
}
void UserPref_FlashSaveFlagCheck(void)
{
    if(FlashSaveMonitorFlag)
    {
        Clr_FlashSaveMonitorFlag();
        Set_FlashForceSaveMonitorFlag();
    }
    if(FlashSaveModeFlag)
    {
        Clr_FlashSaveModeFlag();
        Set_FlashForceSaveModeFlag();
    }
    if(FlashSaveFactoryFlag)
    {
        Clr_FlashSaveFactoryFlag();
        Set_FlashForceSaveFactoryFlag();
    }
    if(FlashSaveMonitor2Flag)
    {
        Clr_FlashSaveMonitor2Flag();
        Set_FlashForceSaveMonitor2Flag();
    }
}

void UserPref_FlashDataSaving(void)
{
    if(FlashForceSaveMonitorFlag)
    {
        //USERP_printMsg("--UserPref_FlashDataSaving SaveMonitorSetting");
        Clr_FlashForceSaveMonitorFlag();
        SaveMonitorSetting();
    }
    if(FlashForceSaveModeFlag)
    {
        //USERP_printMsg("--UserPref_FlashDataSaving SaveModeSetting");
        Clr_FlashForceSaveModeFlag();
        SaveModeSetting();
    }
    if(FlashForceSaveFactoryFlag)
    {
        //USERP_printMsg("--UserPref_FlashDataSaving SaveFactorySetting");
        Clr_FlashForceSaveFactoryFlag();
        SaveFactorySetting();
    }
    if(FlashForceSaveMonitor2Flag)
    {
        //USERP_printMsg("--UserPref_FlashDataSaving SaveMonitorSetting2");
        Clr_FlashForceSaveMonitor2Flag();
        SaveMonitorSetting2();
    }
}
void Init_MonitorSetting2(void)
{
    MonitorSetting2.Ident = FLASH_IDENTDATA;
    MonitorSetting2.Backlighttime = 0;
   #if  BlacklitTimeOnShow
   MonitorSetting2.BlacklitTimeOn_H=0;
   MonitorSetting2.BlacklitTimeOn_M=0;
   MonitorSetting2.BlacklitTimeOn_S=0;
   //MonitorSetting2.BlacklitTimeOn_HH=0;
   #endif
    MonitorSetting2.CheckSum = FLASH_CHECKSUM;
}
void SaveMonitorSetting(void)
{
	MonitorSetting.Ident=FLASH_IDENTDATA;
	MonitorSetting.checksum=FLASH_CHECKSUM;
	#if 0//Enable_Cache
	MCU_DisableCache();
	#endif

	Flash_WriteMonitorSet((BYTE*)&MonitorSetting, MonitorSettingSize);
	#if 0//Enable_Cache
	MCU_EnableCache();
	#endif

}
void SaveMonitorSetting2(void)
{
    MonitorSetting2.Ident = FLASH_IDENTDATA;
    MonitorSetting2.CheckSum = FLASH_CHECKSUM;
	#if 0//Enable_Cache
	MCU_DisableCache();
	#endif

	Flash_WriteMonitorSet2((BYTE*)&MonitorSetting2, MonitorSetting2Size);
	#if 0//Enable_Cache
	MCU_EnableCache();
	#endif


}
void SaveFactorySetting(void)
{
	FactorySetting.Ident=FLASH_IDENTDATA;
	FactorySetting.CheckSum=FLASH_CHECKSUM;
	#if 0//Enable_Cache
	MCU_DisableCache();
	#endif
	Flash_WriteFactorySet((BYTE*)&FactorySetting, FactorySettingSize);
	#if 0//Enable_Cache
	MCU_EnableCache();
	#endif

}

void SaveModeSetting(void)
{
	if (SrcInputType>=Input_Digital || SyncLossState())
		return;

   // USERP_printData("UserModeFlag %x",UserModeFlag);
	//USERP_printData("SaveIndex %x",SaveIndex);
	#if 0//Enable_Cache
	MCU_DisableCache();
	#endif

	if(UserModeFlag)
	{	ModeSetting.Index = SaveIndex|BIT7;//|= 0x80;
		ModeSetting.HerFreq=HFreq(SrcHPeriod);//HFreq(tempHperiod);
		ModeSetting.VerFreq=VFreq(HFreq(SrcHPeriod), SrcVTotal);
		ModeSetting.Flags=(SrcFlags&0x13);

		//USERP_printData("ModeSetting.Index %x",ModeSetting.Index);

	}
	else
	{	ModeSetting.Index=SaveIndex=SrcModeIndex;
		ModeSetting.HerFreq=0xFFFF;
		ModeSetting.VerFreq=0xFFFF;
		ModeSetting.Flags=0xFF;
	}

	ModeSetting.Ident=FLASH_IDENTDATA;
	ModeSetting.checksum=FLASH_CHECKSUM;

	if(UserModeFlag)
		Flash_WriteModeSet((SaveIndex|BIT7), (BYTE*)&ModeSetting);
	else
		Flash_WriteModeSet(SaveIndex, (BYTE*)&ModeSetting);
	ModeSetting.Index &= 0x7F;
	#if 0//Enable_Cache
	MCU_EnableCache();
	#endif

}

Bool IsFlashSettingAreaEmpty(BYTE EmptyValue)  //add  by lizzie.ding ,for check flashsetting empty
{
    BYTE i,j;
    for(i=0;i<FDATA_DYNAMIC_SECTOR_NUM;i++)
    {
            for(j=0;j<10;j++)
            {
                if(FlashReadByte(FDATA_FACTORY_ADDR+FDATA_SECTOR_SIZE*i+j)!=EmptyValue)
                    return FALSE;
            }
    }
    return TRUE;
}

void ReadMonitorSetting(void)
{
	BYTE xdata FlashEmptyFlag=0; 
	
#if DISPLAY_CHECK_VOL
	if (FlashReadByte(PROTECT_START_ADDR+4))//20100911,checksum
	{
		xdata BYTE  temp[5];
		Flash_ReadTbl(0xFFFC,(BYTE*)&temp,4);
		temp[4]=0x00;
		FlashWriteCheckSumTbl(TRUE,PROTECT_START_ADDR, (BYTE*) &temp, 5);
              Code_checksun=((temp[2])<<8)|(temp[3]);    
	}
	else
	{
		Code_checksun=(FlashReadByte(PROTECT_START_ADDR+2));
		Code_checksun=((Code_checksun<<8)|(FlashReadByte(PROTECT_START_ADDR+3)));
	}
	//printData("Code_checksun   ====   %d", Code_checksun);
#endif

	Flash_GetBufferAddr();
		Flash_AllocateBufferAddr();
		if(IsFlashSettingAreaEmpty(0x00)||IsFlashSettingAreaEmpty(0xFF))//edit by lizzie.ding ,for make sure flashsetting empty
      	 {
		FlashEmptyFlag=1;
      	 }
		Flash_CheckBufferType();
		
		if(!Flash_ReadMonitorSet((BYTE*)&MonitorSetting, MonitorSettingSize))
		{
			#if DEBUG_ENABLE
			printMsg("Flash_ReadMonitorSet");
			#endif
			//USERP_printMsg(" RMS MonitorSetting initial");
			Init_MonitorSetting();
			if(FlashEmptyFlag)
				{
              			#if Enable_Lightsensor
                                   UserPrefiCareMode = LightSensorOn;//PE Request
              			#endif
              			UserPrefVolume=90;//PE Request petit 20130102
				}
			SaveMonitorSetting();
		}
		if (CheckMonitorSettingOutOfRange())
		{
			#if DEBUG_ENABLE
			printMsg("CheckMonitorSettingOutOfRange");
			#endif
			//USERP_printMsg(" CheckMonitorSettingOutOfRange initial");
			Init_MonitorSetting();
			SaveMonitorSetting();
		}
	
		if(!Flash_ReadFactorySet((BYTE*)&FactorySetting, FactorySettingSize))
		{
			//USERP_printMsg(" RMS FactorySetting initial");
			Init_FactorySetting();
			if(FlashEmptyFlag)
			FactoryProductModeValue=ProductModeOnValue;
			SaveFactorySetting();
		}
	
		if (CheckFactorySettingOutOfRange())
		{
		#if DEBUG_ENABLE
			printMsg("CheckFactorySettingOutOfRange");
		#endif
			//USERP_printMsg(" CheckFactorySettingOutOfRange initial");
			Init_FactorySetting();
			SaveFactorySetting();
		}
	
    #if EnablePanelServiceMenu
		if(!Flash_Read_Factory_KeySet(FLASH_KEY_PANEL, TRUE, 0x00, PanelSettingSize, (BYTE*)&PanelSetting, PanelSettingSize))	//110311 Modify
		{
			//USERP_printMsg(" RMS PanelSetting initial");
			Init_PanelSetting();
			SavePanelSetting();
		}
		if(CheckPanelSettingRange())
		{
			Init_PanelSetting();
			SavePanelSetting();
		}
    #endif
	
		if(!Flash_ReadMonitorSet2((BYTE*)&MonitorSetting2, MonitorSetting2Size))
		{
			//USERP_printMsg(" RMS MonitorSetting2 initial");
			Init_MonitorSetting2();
			SaveMonitorSetting2();
		}
		//USERP_printData("	Backlighttime[%d]",MonitorSetting2.Backlighttime);
		
		SrcInputType = UserPrefInputType;//0909003

//-----------------------------------------------------------------
    {
    
        if(FactoryProductModeValue==ProductModeOnValue)
            Set_ProductModeFlag();
        else
            Clr_ProductModeFlag();
    }
//-----------------------------------------------------------------
}

Bool ReadUserModeSetting(void)
{
	BYTE bFound=FALSE;
	WORD hFreq, vFreq;
	BYTE i;

	hFreq=HFreq(SrcHPeriod);//HFreq(tempHperiod);
	vFreq=VFreq(hFreq, SrcVTotal);

	#if DISPLAY_CHECK_VOL
	/*
	if (FlashReadByte(PROTECT_START_ADDR+4))
		{
		xdata BYTE  temp[5];
		Flash_ReadTb(0x3FFFc,(BYTE*)&temp,4);
		temp[4]=0x00;
		Flash_WriteTb(PROTECT_START_ADDR, (BYTE*) &temp, 5);
              Code_checksun=((temp[2])<<8)|(temp[3]);    //20100911,checksum
	}
	else
		//*/
		{
		Code_checksun=(FlashReadByte(PROTECT_START_ADDR+2));
		Code_checksun=((Code_checksun<<8)|(FlashReadByte(PROTECT_START_ADDR+3)));
		}
	#endif

	

	// 20070426Albert.Search each user mode
	for (i=0; i<UserModeNumber; i++)
    {
    	Flash_ReadModeSet((i|BIT7), (BYTE*)&ModeSetting);	// BIT7 == (UserModeFlag<<1)
		if(ModeSetting.Ident==FLASH_IDENTDATA && ModeSetting.checksum==FLASH_CHECKSUM)
		{	if (abs(hFreq-ModeSetting.HerFreq)<HFreq_Tol &&    abs(vFreq-ModeSetting.VerFreq)<VFreq_Tol &&
				(SrcFlags&0x13)==ModeSetting.Flags)
				break;
		}
    }

	// 20070426Albert,Init new Mode info
    if(i<UserModeNumber)
		bFound = TRUE;
    else
    {
        UserPrefSavedModeIndex=(UserPrefSavedModeIndex+1)%UserModeNumber;
        SaveIndex=UserPrefSavedModeIndex;
        SaveMonitorSetting();
    }
	return bFound;
}

void ReadModeSetting(void)
{
	Bool bFound=FALSE;

	if (UserModeFlag)
		bFound = ReadUserModeSetting();
	else
	{
		SaveIndex=SrcModeIndex;
		bFound = Flash_ReadModeSet(SrcModeIndex, (BYTE*)&ModeSetting);
	}

	if (bFound)
	{
		//USERP_printMsg("mode setting found");
		//USERP_printData("Index=%d",ModeSetting.Index);
		//USERP_printData("AutoTimes=%d",ModeSetting.AutoTimes);
	}
	else
	{
		//USERP_printMsg("mode setting found fail");
		Init_ModeSetting();
		SaveModeSetting();
	}
	CheckModeSettingRange();
}




#else
void Init_MonitorSetting(void)
{
    UserPrefAdcRedGain = DefRedGain;
    UserPrefAdcGreenGain = DefGreenGain;
    UserPrefAdcBlueGain = DefBlueGain;
    UserPrefAdcRedOffset = DefRedOffset;
    UserPrefAdcGreenOffset = DefGreenOffset;
    UserPrefAdcBlueOffset = DefBlueOffset;
    UserPrefBrightness = DefBrightness;
    UserPrefContrast = DefContrast;
    UserPrefRedColor = DefRedColor;
    UserPrefGreenColor = DefGreenColor;
    UserPrefBlueColor = DefBlueColor;
    UserPrefBrightnessUser = DefBrightness;
    UserPrefContrastUser = DefContrast;
    UserPrefRedColorUser = DefRedColor;
    UserPrefGreenColorUser = DefGreenColor;
    UserPrefBlueColorUser = DefBlueColor;
    #if Enable_YMC
    UserPrefYColor = DefRedColor;
    UserPrefCColor = DefGreenColor;
    UserPrefMColor = DefBlueColor;
    #endif
    UserPrefBrightnessWarm1 = DefBrightness;
    UserPrefContrastWarm1 = DefContrast;
    FUserPrefBrightnessWarm1 = DefBrightness;
    FUserPrefContrastWarm1 = DefContrast;
    UserPrefRedColorWarm1 = DefWarm_RedColor;
    UserPrefGreenColorWarm1 = DefWarm_GreenColor;
    UserPrefBlueColorWarm1 = DefWarm_BlueColor;
    UserPrefBrightnessCool1 = DefBrightness;
    UserPrefContrastCool1 = DefContrast;
    FUserPrefBrightnessCool1 = DefBrightness;
    FUserPrefContrastCool1 = DefContrast;
    UserPrefRedColorCool1 = DefCool_RedColor;
    UserPrefGreenColorCool1 = DefCool_GreenColor;
    UserPrefBlueColorCool1 = DefCool_BlueColor;
    UserPrefBrightnessNormal = DefBrightness;
    UserPrefContrastNormal = DefContrast;
    FUserPrefBrightnessNormal = DefBrightness;
    FUserPrefContrastNormal = DefContrast;
    UserPrefRedColorNormal = DefNormal_RedColor;
    UserPrefGreenColorNormal = DefNormal_GreenColor;
    UserPrefBlueColorNormal = DefNormal_BlueColor;
    UserPrefBrightnessSRGB = DefBrightness;
    UserPrefContrastSRGB = DefContrast;
    UserPrefRedColorSRGB = DefsRGB_RedColor;
    UserPrefGreenColorSRGB = DefsRGB_GreenColor;
    UserPrefBlueColorSRGB = DefsRGB_BlueColor;
    UserPrefECOMode = 0;


	UserPrefColorTemp = CTEMP_Warm1;
    // 2006/10/26 6:49PM by Emily     UserPrefSharpness = 2;
    UserPrefLanguage = LANG_English;
    UserPrefVolume = 50;
    //UserPrefLanguage = 0; cancel for zhifeng
    UserPrefOsdHStart = 50;
    UserPrefOsdVStart = 50;
    UserPrefOsdTime = 10;
    UserPrefOsdTrans = 0;
    UserPrefDcrMode = 0;
    UserPrefGamaMode = 0;
    
    #if DefaultIconExit//AOC_37_Series || ModelName==ENVISION_P2071l
    UserPrefLastMenuIndex = 8;
    #else
    UserPrefLastMenuIndex = 0;
    #endif
    
    UserPrefInputType = Input_Analog1;
    #if INPUT_TYPE == INPUT_1A
    UserPrefInputSelectType = InputSelectType_Analog; 		
    #else
    UserPrefInputSelectType = InputSelectType_Auto; // 071221 modified for initial UserPrefInputSelectType
    #endif
    UserPrefSavedModeIndex = NumberOfMode;
    #if DDCCI_ENABLE
        UserprefRedBlackLevel = 50;
        UserprefGreenBlackLevel = 50;
        UserprefBlueBlackLevel = 50;
    #endif 
    #if Enable_Expansion
        UserprefExpansionMode = DefExpansion;
    #endif 
    #if MWEFunction
        //UserPrefMWE=OFF_W;
        UserPrefHue = DefHue;
        UserPrefSaturation = DefSaturation;
        UserPrefSubContrast = DefSubContrast;
        UserPrefSubBrightness = DefSubBrightness;
        // 2006/10/26 6:47PM by Emily     UserPrefSubSharpness = 11;
        //UserPrefSkinTone=OFF_SKIN;
        UserPrefColorFlags = 0;
        // 2006/10/26 6:48PM by Emily     UserPrefDLCMode = OFF_DLC;
        // UserPrefBFPosition =0;               
        UserPrefBFSize = 1;
        UserPrefBFVSize = 0;
        UserPrefBFHStart = 0;
        UserPrefBFVStart = 0;
    #endif 
    //=======for AutoBacklight=====================
    UserprefHistogram1 = 0x40;
    UserprefHistogram2 = 0xB0;
    UserprefALha = 50;
    UserprefBata = 50;
	#if PANEL_VCOM_ADJUST		//121031 Modify
    UserPrefVcomValue = DEF_VCOM_VALUE;
    #elif EnableVcomAdjustItem//def Panel_RSDS
    UserPrefVcomValue = DefVcomValue;
    #endif
    // 2006/11/6 5:34PM by Emily      UserprefAutoModel=1;
    // 2006/11/6 5:33PM by Emily      UserprefCompareRange=8;
    // 2006/11/6 5:33PM by Emily      UserprefDiffHvalue=0x80;
    #if MS_OVD
	    MonitorFlags = bPowerOnBit | bBurninModeBit | bDCCCIONOBit |bOverDriveOnBit; //Benz 2007.4.23   20:32:00]
    #else
	    MonitorFlags = bPowerOnBit | bBurninModeBit | bDCCCIONOBit; //Benz 2007.4.23   20:32:00
    #endif

    #if Enable_Lightsensor
    UserPrefiCareMode = LightSensorOff;
    #endif

    #ifdef EnableLightBar
    UserPrefLightBar = DefLightBarValue;
    #endif
        LogoON=1;
    MonitorSetting.checksum = ChecksumValue;
}
void Init_ModeSetting(void)
{
    UserPrefHTotal = StandardModeHTotal;
    UserPrefHStart = StandardModeHStart;
    UserPrefVStart = StandardModeVStart;
    UserPrefAutoHStart = UserPrefHStart;
    UserPrefAutoVStart = StandardModeVStart; //2004/01/15 num11
    UserPrefPhase = 18;
    UserPrefAutoTimes = 0;
    ModeSetting.checksum = ChecksumValue;
}
void CheckColorValueRange(void)
{
    if (UserPrefContrast > MaxContrastValue || UserPrefContrast < MinContrastValue)
    {
        UserPrefContrast = DefContrast;
    }
    #if 1
        if (UserPrefRedColor > MaxColorValue)
            UserPrefRedColor = DefRedColor;
        if (UserPrefGreenColor > MaxColorValue)
            UserPrefGreenColor = DefGreenColor;
        if (UserPrefBlueColor > MaxColorValue)
            UserPrefBlueColor = DefBlueColor;
    #endif 
}
void CheckMonitorSettingRange(void)
{
    CheckColorValueRange();
    MSBankNo %= 3;
    UserPrefBrightnessUser %= 101;
    UserPrefContrastUser %= 256;
    UserPrefBrightness %= 101;
    UserPrefColorTemp %= CTEMP_Nums;
    // 2006/10/26 6:49PM by Emily     UserPrefSharpness %= 4;
    UserPrefLanguage %= LANG_Nums;
    UserPrefInputType %= Input_Nums;
    UserPrefECOMode %= ECO_Nums; //Benz 2007.4.23   16:12:21
    UserPrefHue %= (MAX_VIDEO_HUE + 1);
    UserPrefSaturation %= (MAX_VIDEO_SATURATION + 1);
    UserPrefSubContrast %= 256;
    UserPrefSubBrightness %= 256;
    // 2006/10/26 6:47PM by Emily     UserPrefSubSharpness %= 16;
    UserPrefBFSize %= 8; // 2006/9/18 1:59PM by Emily need check again
    #if Enable_Lightsensor
    UserPrefiCareMode %= 2;
    #endif
}
#if 0  //0729
    void CheckModeSettingRange(void)
    {
        if (abs(UserPrefHTotal - StandardModeHTotal) > ClockAdjRange)
            UserPrefHTotal = StandardModeHTotal;
        if (UserPrefHStart > 2 *StandardModeHStart)
            UserPrefHStart = StandardModeHStart;
        if (UserPrefVStart > 2 *StandardModeVStart)
            UserPrefVStart = StandardModeVStart;
        UserPrefPhase &= 0x3F;
        if (UserPrefAutoTimes == 0 || abs(UserPrefHStart - UserPrefAutoHStart) > 50)
            UserPrefAutoHStart = UserPrefHStart;
    }
#endif 
void CheckModeSettingRange(void)
{
    if (abs(UserPrefHTotal - StandardModeHTotal) > 100)
    {
        UserPrefHTotal = StandardModeHTotal;
    }
    //  if (UserPrefHStart>2*StandardModeHStart)
    //    UserPrefHStart=StandardModeHStart;
    //  if (UserPrefVStart>2*StandardModeVStart)
    //    UserPrefVStart=StandardModeVStart;
    if (abs(UserPrefHStart - UserPrefAutoHStart) > 50)
    //2004/02/10 num1
    {
        UserPrefHStart = UserPrefAutoHStart;
    } //2004/02/10 num1
    if (UserPrefVStart > 2 *UserPrefAutoVStart)
    //2004/02/10 num1
    {
        UserPrefVStart = UserPrefAutoVStart;
    } //2004/02/10 num1
	#ifdef TSUMXXT		//110413 Modify
    UserPrefPhase &= 0x7F;
	#else
    UserPrefPhase &= 0x3F;
	#endif
    if (UserPrefAutoTimes == 0 || abs(UserPrefHStart - UserPrefAutoHStart) > 50)
    {
        UserPrefAutoHStart = UserPrefHStart;
    }
}
void SaveMonitorSetting(void)
{
    NVRam_WriteTbl(MonitorSettingAddr, (BYTE*) &MonitorSetting, MonitorSettingSize);
}

void SaveModeSetting(void)
{
    //NVRam_WriteTbl(nvrModeBaseAddr(SrcModeIndex), (BYTE*)&ModeSetting, ModeSettingSize);
    NVRam_WriteTbl(nvrModeBaseAddr(SaveIndex), (BYTE*) &ModeSetting, ModeSettingSize);
}
/*******
void ReadMonitorSetting(void)
{ BYTE tempValue;
CheckBoard();
NVRam_ReadByte(nvrMonitorAddr(checksum), &tempValue);
//printData("monitor size %d", MonitorSettingSize);
//printData("contrast %x", nvrMonitorAddr(Contrast));
if (tempValue==ChecksumValue)
{ NVRam_ReadTbl(MonitorSettingAddr, (BYTE*)&MonitorSetting, MonitorSettingSize);
printMsg("monitor setting found");
// printData("monitorflag %d",MonitorFlags);
}
else
{ Init_MonitorSetting();
SaveMonitorSetting();
}
#if 0 //0517   
UserPrefRedColor=UserPrefRedColor9300K;
UserPrefGreenColor=UserPrefGreenColor9300K;
UserPrefBlueColor=UserPrefBlueColor9300K;
UserPrefColorTemp=CTEMP_9300K;
Read9300KSetting();
#endif  
CheckMonitorSettingRange();
SrcInputType=UserPrefInputType;
}
 ***************/
void ReadMonitorSetting(void)
{
    BYTE tempValue,i;
    #if Backupdata
        BYTE xdata tempchecksum, NVRAMData, checksum1;
        WORD xdata totalchecksum;
        WORD j;
        BYTE InitialFiniah = 0;
    #endif 
      	 if(IsEEPRomEmpty(0xFF)||IsEEPRomEmpty(0x00))
      	 	{
                     //WORD temp=ProductModeOnValue;
                     //NVRam_WriteTbl(ProductModeAddr, (BYTE*)(&temp), 2);
			NVRam_WriteWord(ProductModeAddr, ProductModeOnValue);
			#if EnablePanelServiceMenu&&!PanelSettingInFlash
			//NVRam_WriteWord(PanelTypeNumAddr, UseDefaultPanelValue);
			Init_PanelSetting();
			SavePanelSetting();
			#endif
			#if Enable_Lightsensor
                   UserPrefiCareMode = LightSensorOn;
			#endif
			#if AudioFunc
			UserPrefVolume=90;//petit 20130102
			#endif
			SaveMonitorSetting();
      	 	}
    CheckBoard(); //Benz 2007.4.24   23:28:02 Why?
    NVRam_ReadByte(nvrMonitorAddr(checksum), &tempValue);
    if (tempValue == ChecksumValue)
    {
        NVRam_ReadTbl(MonitorSettingAddr, (BYTE*) &MonitorSetting, MonitorSettingSize);
    }
    else
    {
        #if Backupdata
            InitialFiniah = 1;
        #endif 
        for(i=0;i<10;i++)
        {       
     	     NVRam_ReadByte(nvrMonitorAddr(checksum), &tempValue);
     	     if (tempValue==ChecksumValue)   // double check
     	     {
                NVRam_ReadTbl(MonitorSettingAddr, (BYTE*)&MonitorSetting, MonitorSettingSize);
                break;
     	     }
        }
        if(i==10)
      	 {
	     Init_MonitorSetting();
            SaveMonitorSetting();
        }
        //Init_MonitorSetting();
        //SaveMonitorSetting();
    }
      if (CheckFactorySettingOutOfRange())
      	 {
	     Init_MonitorSetting();
            SaveMonitorSetting();
        }
	
    //************************************************************
	#if EnablePanelServiceMenu
	#if PanelSettingInFlash
	{
		BYTE retry;
		   #if Enable_Cache    // jeff 12 22 for cache add it 
	        retry =50;
		   #else
		retry = 3;
		   #endif
		while(FlashReadByte(PANEL_START)!=FLASH_IDENTDATA)

		{
			retry--;
			if(retry==0)
				break;
		}
		if(retry==0)//CheckPanelSettingRange()
		{
			Init_PanelSetting();
			SavePanelSetting();
		}
		else
		{
			//Flash_ReadPanelSet(0, (BYTE*)&PanelSetting, PanelSettingSize);
			Flash_Read_Factory_KeySet(FLASH_KEY_PANEL, TRUE, 0, PANEL_SIZE, (BYTE*)&PanelSetting, PANEL_SIZE);	//110311 Modify
		}
		if(CheckPanelSettingRange())
		{
			Init_PanelSetting();	
			SavePanelSetting();
		}
		
		if(PanelPanelTypeNum==UseDefaultPanelValue)
	            PanelSelect=0;
		else
	            PanelSelect=1;
	}
	#else
    {
        WORD temp;
        NVRam_ReadTbl(PanelTypeNumAddr, (BYTE*)(&temp), 2);
        if(temp==UseUserPanelValue)
        {
            PanelSelect=1;
            NVRam_ReadByte(nvrPanelAddr(CheckSum), &tempValue);
            if (tempValue == ChecksumValue)
            {
                NVRam_ReadTbl(PanelSettingAddr, (BYTE*) &PanelSetting, PanelSettingSize);
                if(CheckPanelSettingRange())
                {
                    Init_PanelSetting();
                    SavePanelSetting();
                }
            }
            else
            {
                for(i=0;i<10;i++)
                {       
             	     NVRam_ReadByte(nvrPanelAddr(CheckSum), &tempValue);
             	     if (tempValue==ChecksumValue)   // double check
             	     {
                        NVRam_ReadTbl(PanelSettingAddr, (BYTE*) &PanelSetting, PanelSettingSize);
                        break;
             	     }
                }
                if(i==10)
              	 {
        	     Init_PanelSetting();
                    SavePanelSetting();
                	}
            }
        }
	else
	{
            Init_PanelSetting();
            PanelSelect=0;
	}
    }
    #endif
    #endif
    //************************************************************
	
    CheckMonitorSettingRange();
    SrcInputType = UserPrefInputType;
//-----------------------------------------------------------------
    {
        WORD temp;
        NVRam_ReadTbl(ProductModeAddr, (BYTE*)(&temp), 2);
        if(temp==ProductModeOnValue)
            Set_ProductModeFlag();
        else
            Clr_ProductModeFlag();
    }
//-----------------------------------------------------------------
    #if Backupdata// check backup data checksum
        if (!InitialFiniah)
        {
            NVRam_ReadByte(Monitorchecksum, &tempchecksum);
            totalchecksum = 0;
            for (j = 0; j < MonitorSettingSize; j++)
            {
                NVRam_ReadByte(j + MonitorSettingAddr, &NVRAMData);
                totalchecksum = totalchecksum + NVRAMData;
            }
            checksum1 = totalchecksum % 256; // data check sum 	 
            if (checksum1 != tempchecksum)
            {
                 /*totalchecksum=0;
                for(i=BackupdataAddress;i<(BackupdataAddress+MonitorSettingSize);i++)
                {NVRam_ReadByte(i, &NVRAMData);	
                totalchecksum=totalchecksum+NVRAMData;
                }
                checksum1=totalchecksum%256; // data check sum */
                //if(checksum1==tempchecksum)		 
                NVRam_ReadTbl(BackupdataAddress, (BYTE*) &MonitorSetting, MonitorSettingSize);
                //else
                // Init_MonitorSetting();
                SaveMonitorSetting();
            }
            else
            {
                NVRam_WriteTbl(BackupdataAddress, (BYTE*) &MonitorSetting, MonitorSettingSize);
            }
        }
    #endif 
}
void GetModeSaveIndex(void)
{
    ModeInfoType modeInfo;
    BYTE i;
    WORD hFreq, vFreq;
    hFreq = HFreq(SrcHPeriod);
    vFreq = VFreq(hFreq, SrcVTotal);
    for (i = 0; i < NumberOfMode; i++)
    {
        NVRam_ReadTbl(nvrModeInfoAddr(i), (BYTE*) &modeInfo, ModeInfoSize);
        if ( abs( hFreq - modeInfo.HerFreq ) < HFreq_Tol && abs( vFreq - modeInfo.VerFreq ) < VFreq_Tol 
		&&abs(SrcVTotal-modeInfo.VTotal)<3&& ( SrcFlags & 0x13 ) == modeInfo.Flags )
        {
            #if DEBUG_SAVEMODE
                printData("found saveindex",1);
                printData("SaveIndex=%d", i);
           // printData("End2FontAddr=%x", End2FontAddr);
           // printData("End4FontAddr=%x", End4FontAddr);
           // printData("End8FontAddr=%x", End8FontAddr);
            #endif 
            break;
        }
    }
    if (i >= NumberOfMode)
    {
        UserPrefSavedModeIndex = (UserPrefSavedModeIndex + 1) % NumberOfMode;
        SaveIndex = UserPrefSavedModeIndex;
        modeInfo.HerFreq = hFreq;
        modeInfo.VerFreq = vFreq;
        modeInfo.VTotal = SrcVTotal;
        modeInfo.Flags = SrcFlags &0x13;
        NVRam_WriteTbl(nvrModeInfoAddr(SaveIndex), (BYTE*) &modeInfo, ModeInfoSize);
        NVRam_WriteByte(nvrModeAddr(checksum), 0);
        NVRam_WriteByte(nvrMonitorAddr(SavedModeIndex), UserPrefSavedModeIndex);
    }
    else
    {
        SaveIndex = i;
    }
}
void ReadModeSetting(void)
{
    BYTE tempValue;
    if ((SrcInputType != Input_Digital) && (SrcInputType != Input_Digital2))//yuanfa
    {
        GetModeSaveIndex();
        NVRam_ReadByte(nvrModeAddr(checksum), &tempValue);
        if (tempValue == ChecksumValue)
        {
            #if DEBUG_MSG
                printMsg("mode setting found");
            #endif 
            //NVRam_ReadTbl(nvrModeBaseAddr(SrcModeIndex), (BYTE*)&ModeSetting, ModeSettingSize);
            NVRam_ReadTbl(nvrModeBaseAddr(SaveIndex), (BYTE*) &ModeSetting, ModeSettingSize);
        }
        else
        {
            #if DEBUG_MSG
                printMsg("mode setting not found");
            #endif 
            Init_ModeSetting();
            SaveModeSetting();
        }
        CheckModeSettingRange();
    }
    else
    	{
            Init_ModeSetting();
    	}
}
Bool IsEEPRomEmpty(BYTE EmptyValue)
{
    BYTE tempValue,i;
    for ( i = 0; i < ModeSettingAddr-1; i++ )
    	{
            NVRam_ReadByte(i, &tempValue);
            if(tempValue==EmptyValue)
                continue;
            else
                return FALSE;
    	}
    return TRUE;
}

void EraseEEPROM(void)
{
    NVRam_WriteByte(nvrMonitorAddr(checksum), 0);
    for (SaveIndex = 0; SaveIndex < NumberOfMode; SaveIndex++)
    {
        NVRam_WriteByte(nvrModeAddr(checksum), 0);
    }
//    SrcFlags |= bUserMode;
//    for (SaveIndex = 0; SaveIndex < NumberOfMode; SaveIndex++)
//    {
//        NVRam_WriteByte(nvrModeInfoAddr(checksum), 0);
//    }
    SrcFlags &= ~bUserMode;
    //  NVRam_WriteByte(Factory9300KBrightnessAddr, DefBrightness);
    //  NVRam_WriteByte(Factory6500KBrightnessAddr, DefBrightness);
    //  NVRam_WriteByte(Factory9300KContrastAddr, DefContrast);
    // NVRam_WriteByte(Factory6500KContrastAddr, DefContrast);
}
/**********
void CheckBoard(void)
{ BYTE tempValue;
NVRam_ReadByte(CheckBoardAddr, &tempValue);
if (tempValue!=BoardChecksum)
{ EraseEEPROM();
BlacklitTime=0;
SaveBlacklitTime();
NVRam_WriteByte(CheckBoardAddr, BoardChecksum);
}
else
ReadBlacklitTime();
}
 ************/
void CheckBoard(void)
{
    BYTE tempValue;
    BYTE i;
    NVRam_ReadByte( CheckBoardAddr, &tempValue );
    if ( tempValue != BoardChecksum )
    {
        for ( i = 0; i < 10; i++ )
        {
            NVRam_ReadByte( CheckBoardAddr, &tempValue );
            if ( tempValue == BoardChecksum )   // double check
            {
            ReadBlacklitTime();
            break;
            }
        }
        if ( i == 10 )
        {
            EraseEEPROM();
            BlacklitTime = 0;
            SaveBlacklitTime();
            NVRam_WriteByte( CheckBoardAddr, BoardChecksum );    
        }
    }
    else
    {
        ReadBlacklitTime();
    }
}
void ReadBlacklitTime(void)
{
    NVRam_ReadTbl(BlacklitTimeAddr, (BYTE*)(&BlacklitTime), 4);
}

void SaveBlacklitTime(void)
{
    NVRam_WriteTbl(BlacklitTimeAddr, (BYTE*)(&BlacklitTime), 4);
}
#endif

#if 0//USEFLASH
void UserPref_EnableFlashSaveBit(BYTE ucFlag)
{
    ucFlashSaveState |= ucFlag;
    if(FactoryModeFlag || ProductModeFlag)
        ucFlashSaveCount = 200;
    else
        ucFlashSaveCount = 1000;
}
void UserPref_FlashSaveFlagCheck(void)
{
    if(FlashSaveMonitorFlag)
    {
        Clr_FlashSaveMonitorFlag();
        Set_FlashForceSaveMonitorFlag();
    }
    if(FlashSaveModeFlag)
    {
        Clr_FlashSaveModeFlag();
        Set_FlashForceSaveModeFlag();
    }
    if(FlashSaveFactoryFlag)
    {
        Clr_FlashSaveFactoryFlag();
        Set_FlashForceSaveFactoryFlag();
    }
}

void UserPref_FlashDataSaving(void)
{
    if(FlashForceSaveMonitorFlag)
    {
        Clr_FlashForceSaveMonitorFlag();
        SaveMonitorSetting();
    }
    if(FlashForceSaveModeFlag)
    {
        Clr_FlashForceSaveModeFlag();
        SaveModeSetting();
    }
    if(FlashForceSaveFactoryFlag)
    {
        Clr_FlashForceSaveFactoryFlag();
        SaveFactorySetting();
    }
    /*
    if(SaveBackLightTimeFlag)
    {
        Clr_SaveBackLightTimeFlag();
#if UserData_InFlash
        UserPrefBacklightTime = BlacklitTime;
#endif
        SaveBlacklitTime();
    }
    */
}
#endif

extern Bool ResetOsdTimer(void);
extern Bool EnablePowerDownCounter(void);
extern Bool PowerOffSystem(void);
extern Bool PowerOnSystem(void);
extern Bool EnterRootMenu(void);
extern Bool OsdCountDown(void);
extern void SetDefaultSetting(BYTE ucDef);
//==============ready================
extern Bool AdjustContrast(MenuItemActionType action);
extern WORD GetContrastValue(void);
extern Bool AdjustVolume(MenuItemActionType action);
extern WORD GetVolumeValue(void);
extern Bool AdjustBrightness(MenuItemActionType action);
extern WORD GetBrightnessValue(void);
extern Bool AdjustFocus(MenuItemActionType action);
extern WORD GetFocusValue(void);
extern Bool AdjustClock(MenuItemActionType action);
extern WORD GetClockValue(void);
extern Bool AdjustHPosition(MenuItemActionType action);
extern WORD GetHPositionValue(void);
extern Bool AdjustVPosition(MenuItemActionType action);
extern WORD GetVPositionValue(void);
extern WORD GetSourceValue(void);
extern Bool AdjustSource(MenuItemActionType action);
extern WORD GetLanguageValue(void);
extern Bool AdjustLanguage(MenuItemActionType action);
extern void RunLanguage(void);  //edit by lizzie.ding 100601, for new define language,don't forget define language address
#ifdef EnableLightBar
extern Bool AdjustLightBar(MenuItemActionType action);
extern WORD GetLightBarValue(void);
#endif
extern Bool AdjustBlueColor(MenuItemActionType action);
extern WORD GetBlueColorValue(void);
extern Bool AdjustGreenColor(MenuItemActionType action);
extern WORD GetGreenColorValue(void);
extern Bool AdjustRedColor(MenuItemActionType action);
extern WORD GetRedColorValue(void);
extern WORD GetSharpnessValue(void);
extern WORD GetSharpnessRealValue(void);
extern Bool AdjustSharpness(MenuItemActionType action);
extern Bool ResetAllSetting(void);
extern Bool AutoConfig(void);
extern Bool SwitchSource(void);
extern void SaveUserPref(void);
extern Bool ChangeSourceToDigital(void);
#if Dual
extern Bool ChangeSourceToAnalog(void);
#endif
extern Bool ChangeSource(void);
#if HotInputSelect

extern Bool SetInputToAnalog(void);
#if INPUT_TYPE==INPUT_1A2D||INPUT_TYPE==INPUT_1A1D
extern Bool SetInputToDigital(void);
#endif
extern Bool EnterHotInputMenu( void );
#if INPUT_TYPE==INPUT_1A2D||INPUT_TYPE==INPUT_1A1H
extern Bool SetInputToHDMI(void);
#endif

#endif
#if AudioFunc
    extern Bool SetMute(void);
#endif 
#if ECO
    extern Bool AdjustECOMode(MenuItemActionType action);
#endif 
extern Bool AdjustGamaMode(MenuItemActionType action);
extern Bool AdjustDcrMode(MenuItemActionType action);
#if Enable_Lightsensor
Bool AdjustiCareMode(MenuItemActionType action);
#endif
extern Bool AdjustColorTempMode(MenuItemActionType action);
extern Bool AdjustColorBoostMode(MenuItemActionType action);
extern Bool AdjustSkinProtectMode(MenuItemActionType action);
extern Bool AdjustGreenEnhanceMode(MenuItemActionType action);
extern Bool AdjustBlueEnhanceMode(MenuItemActionType action);
extern Bool AdjustAutoDetectMode(MenuItemActionType action);
extern Bool AdjustDemoMode(MenuItemActionType action);
#if Dual
extern Bool AdjustSource(MenuItemActionType action);
#endif
//Bool AdjustBrightFrameMode(MenuItemActionType action);
//============ADD ON 06/05/2003========================================
//extern void MoveColorTemp(MenuItemActionType menuAction);
//extern Bool SetColorTemp(void); //1014
//extern void MoveLanguage(MenuItemActionType menuAction);
//=====================================================================
extern Bool SetToEnglish(void);
//=====================================================================
//=====================================================================
#if 1
    extern Bool AutoColor(void);
    extern Bool AdjustBlueGain(MenuItemActionType action);
    extern WORD GetBlueGainValue(void);
    extern Bool AdjustGreenGain(MenuItemActionType action);
    extern WORD GetGreenGainValue(void);
    extern Bool AdjustRedGain(MenuItemActionType action);
    extern WORD GetRedGainValue(void);
    extern Bool AdjustAdcBlueOffset(MenuItemActionType action);
    extern WORD GetAdcBlueOffsetValue(void);
    extern Bool AdjustAdcGreenOffset(MenuItemActionType action);
    extern WORD GetAdcGreenOffsetValue(void);
    extern Bool AdjustAdcRedOffset(MenuItemActionType action);
    extern WORD GetAdcRedOffsetValue(void);
    extern Bool SetFactoryColorTempCool1(void);
    extern Bool SetFactoryColorTempCool2(void);
    extern Bool SetFactoryColorTempWarm1(void);
    extern Bool SetFactoryColorTempWarm2(void);
    extern Bool SetFactoryColorTempNormal(void);
    extern Bool SetFactoryColorTempSRGB(void);
    extern Bool AdjustRedColorWarm1(MenuItemActionType action);
    extern Bool AdjustGreenColorWarm1(MenuItemActionType action);
    extern Bool AdjustBlueColorWarm1(MenuItemActionType action);
    extern Bool AdjustBrightnessWarm1(MenuItemActionType action);
    extern Bool AdjustContrastWarm1(MenuItemActionType action);
    extern WORD GetBColorWarm1Value(void);
    extern WORD GetGColorWarm1Value(void);
    extern WORD GetRColorWarm1Value(void);
    extern WORD GetBrightnessWarm1Value(void);
    extern WORD GetContrastWarm1Value(void);
    extern Bool AdjustRedColorWarm2(MenuItemActionType action);
    extern Bool AdjustGreenColorWarm2(MenuItemActionType action);
    extern Bool AdjustBlueColorWarm2(MenuItemActionType action);
    extern Bool AdjustBrightnessWarm2(MenuItemActionType action);
    extern Bool AdjustContrastWarm2(MenuItemActionType action);
    extern WORD GetBColorWarm2Value(void);
    extern WORD GetGColorWarm2Value(void);
    extern WORD GetRColorWarm2Value(void);
    extern WORD GetBrightnessWarm2Value(void);
    extern WORD GetContrastWarm2Value(void);
    extern Bool AdjustRedColorCool1(MenuItemActionType action);
    extern Bool AdjustGreenColorCool1(MenuItemActionType action);
    extern Bool AdjustBlueColorCool1(MenuItemActionType action);
    extern Bool AdjustBrightnessCool1(MenuItemActionType action);
    extern Bool AdjustContrastCool1(MenuItemActionType action);
    extern WORD GetBColorCool1Value(void);
    extern WORD GetGColorCool1Value(void);
    extern WORD GetRColorCool1Value(void);
    extern WORD GetBrightnessCool1Value(void);
    extern WORD GetContrastCool1Value(void);
    extern Bool AdjustRedColorCool2(MenuItemActionType action);
    extern Bool AdjustGreenColorCool2(MenuItemActionType action);
    extern Bool AdjustBlueColorCool2(MenuItemActionType action);
    extern Bool AdjustBrightnessCool2(MenuItemActionType action);
    extern Bool AdjustContrastCool2(MenuItemActionType action);
    extern WORD GetBColorCool2Value(void);
    extern WORD GetGColorCool2Value(void);
    extern WORD GetRColorCool2Value(void);
    extern WORD GetBrightnessCool2Value(void);
    extern WORD GetContrastCool2Value(void);
    extern Bool AdjustRedColorNormal(MenuItemActionType action);
    extern Bool AdjustGreenColorNormal(MenuItemActionType action);
    extern Bool AdjustBlueColorNormal(MenuItemActionType action);
    extern Bool AdjustBrightnessNormal(MenuItemActionType action);
    extern Bool AdjustContrastNormal(MenuItemActionType action);
    extern WORD GetBColorNormalValue(void);
    extern WORD GetGColorNormalValue(void);
    extern WORD GetRColorNormalValue(void);
    extern WORD GetBrightnessNormalValue(void);
    extern WORD GetContrastNormalValue(void);
    extern Bool AdjustRedColorSRGB(MenuItemActionType action);
    extern Bool AdjustGreenColorSRGB(MenuItemActionType action);
    extern Bool AdjustBlueColorSRGB(MenuItemActionType action);
    extern Bool AdjustBrightnessSRGB(MenuItemActionType action);
    extern Bool AdjustContrastSRGB(MenuItemActionType action);
    extern WORD GetBColorSRGBValue(void);
    extern WORD GetGColorSRGBValue(void);
    extern WORD GetRColorSRGBValue(void);
    extern WORD GetBrightnessSRGBValue(void);
    extern WORD GetContrastSRGBValue(void);
    extern Bool AdjustYColor(MenuItemActionType action);
    extern Bool AdjustCColor(MenuItemActionType action);
    extern Bool AdjustMColor(MenuItemActionType action);
    extern WORD GetYColorValue(void);
    extern WORD GetCColorValue(void);
    extern WORD GetMColorValue(void);
    extern Bool EnableBurninMode(void);
    extern WORD GetBurninValue(void);
    extern Bool AdjustBurnin(MenuItemActionType action);

#if PANELSSCSTEPLFUN
extern        Bool AdjustSSCSTEPL(MenuItemActionType action);
extern        WORD GetSSCSTEPL(void);
extern        Bool AdjustSSCSTEPH(MenuItemActionType action);
extern        WORD GetSSCSTEPH(void);
extern        Bool AdjustSSCSPANL(MenuItemActionType action);
extern        WORD GetSSCSPANL(void);
extern        Bool AdjustSSCSPANH(MenuItemActionType action);
extern        WORD GetSSCSPANH(void);
#endif

	
#if EnablePanelServiceMenu
    extern Bool AdjustPanelTpye(MenuItemActionType action);
#endif
#if Enable_ProductModeAdjust
    extern WORD GetProductModeValue(void);
    extern Bool AdjustProductMode(MenuItemActionType action);
#endif
    extern Bool AdjustBankNo(MenuItemActionType action);
    extern Bool AdjustAddr(MenuItemActionType action);
    extern WORD GetAddrValue(void);
    extern Bool AdjustRegValue(MenuItemActionType action);
    extern WORD GetAddrRegValue(void);
    extern WORD GetRegValue(void);
    extern Bool WriteRegValue(void);
    //extern WORD GetOSDTime100Value(void);
    extern Bool SetColorTemp(void);
    extern void ReadColorTempSetting(void);
#if !USEFLASH
    extern void SaveFactorySetting(BYTE itemIndex);
#endif
    extern void SaveUserPrefSetting(BYTE menuPageIndex);
    #if ECO
        extern void SetECO(void);
    #endif 
    extern Bool EnterISPMode(void);
#endif 
extern Bool AdjustOSDHPosition(MenuItemActionType action);
extern WORD GetOSDHPositionValue(void);
extern Bool AdjustOSDVPosition(MenuItemActionType action);
extern WORD GetOSDVPositionValue(void);
extern Bool AdjustOSDTime(MenuItemActionType action);
extern WORD GetOSDTimeValue(void);
extern WORD GetOSDTime100Value(void);
#ifdef OffPower        //090331 xiandi.yu Auto Off Power
extern Bool AdjustAutoPowerOff(MenuItemActionType action);
extern WORD GetAutoPowerOffValue(void);
#endif
//extern Bool DrawMenuTitle(void);
extern Bool DrawTimingInfo(void);
extern Bool EnterDisplayLogo(void);
#if DDCCI_ENABLE
    extern Bool AdjustDDCCI(MenuItemActionType action);
    extern Bool ExecDDC2Bi(void);
    extern Bool ResetGeometry(void);
    extern Bool ResetColorTemp(void);
    extern void AdjustTempColorTemp(BYTE ctemp);
#endif 
#if Enable_Expansion
    extern Bool AdjustExpansionMode(void);
    extern void SetExpansionMode(void);
#endif 
extern Bool AdjustBlue(MenuItemActionType action);
extern Bool AdjustGreen(MenuItemActionType action);
extern Bool AdjustMWE(MenuItemActionType action);
extern Bool AdjustHue(MenuItemActionType action);
extern Bool AdjustSaturation(MenuItemActionType action);
extern Bool AdjustSubContrast(MenuItemActionType action);
extern Bool AdjustSubBrightness(MenuItemActionType action);
extern Bool AdjustSkinTone(MenuItemActionType action);
//extern Bool AdjustMWEMode(MenuItemActionType action);
extern void SetColorEnhance(bit enable);
extern Bool AdjustBFSize(MenuItemActionType action);
extern WORD GetBFSizeValue(void);
//extern Bool AdjustBFHSize(MenuItemActionType action);
//extern WORD GetBFHSizeValue(void);
//extern Bool AdjustBFVSize(MenuItemActionType action);
//extern WORD GetBFVSizeValue(void);
extern Bool AdjustBFHstart(MenuItemActionType action);
extern WORD GetBFHstartValue(void);
extern Bool AdjustBFVstart(MenuItemActionType action);
extern WORD GetBFVstartValue(void);
extern Bool AdjustBF_Switch(MenuItemActionType action);
extern WORD GetBF_SwitchValue(void);
extern WORD GetHue100Value(void);
extern WORD GetScale100Value(WORD value, WORD minValue, WORD maxValue);
extern WORD GetSaturation100Value(void);
extern WORD GetSubContrast100Value(void);
extern WORD GetSubBrightness100Value(void);
extern Bool AdjustInputSource(MenuItemActionType action);
//extern Bool SetBFSize(void);
extern Bool SetBFSize(BYTE ucSize);
Bool AdjustBrightFrameSize(BYTE ucSize);

extern WORD GetPanelOnTimeValue(void);
extern Bool ResetGeometry(void);

extern Bool AdjustOSDTransparence(MenuItemActionType action);
extern WORD GetOSDTransparence(void);
extern Bool AdjustAutoConfig(void);
extern Bool DoAutoConfig(void);
extern Bool AdjustResetSwitch(void);
extern Bool DoResetConfig(void);

#if MS_OVD
extern  WORD GetOverDriveValue(void);
extern  Bool AdjustOverDrive(MenuItemActionType action);
#endif

#if 0//EnablePanelServiceMenu
extern void SavePanelSettingByItem(BYTE itemIndex);
extern Bool Test(void);
extern    Bool AdjustPanelDither(MenuItemActionType action);
//extern    WORD GetPanelDither(void);
extern    Bool AdjustPanelOnTiming1(MenuItemActionType action);
extern    WORD GetPanelOnTiming1(void);
extern    Bool AdjustPanelOnTiming2(MenuItemActionType action);
extern    WORD GetPanelOnTiming2(void);
extern    Bool AdjustPanelOffTiming1(MenuItemActionType action);
extern    WORD GetPanelOffTiming1(void);
extern    Bool AdjustPanelOffTiming2(MenuItemActionType action);
extern    WORD GetPanelOffTiming2(void);

extern    Bool AdjustPanelHSyncWidth(MenuItemActionType action);
extern    WORD GetPanelHSyncWidth(void);
extern    Bool AdjustPanelHSyncBackPorch(MenuItemActionType action);
extern    WORD GetPanelHSyncBackPorch(void);
extern    Bool AdjustPanelVSyncWidth(MenuItemActionType action);
extern    WORD GetPanelVSyncWidth(void);
extern    Bool AdjustPanelVSyncBackPorch(MenuItemActionType action);
extern    WORD GetPanelVSyncBackPorch(void);
//extern    Bool AdjustPanelWidth(MenuItemActionType action);
extern    WORD GetPanelWidth(void);
//extern    Bool AdjustPanelHeight(MenuItemActionType action);
extern    WORD GetPanelHeight(void);
extern    Bool AdjustPanelHTotal(MenuItemActionType action);
extern    WORD GetPanelHTotal(void);
extern    Bool AdjustPanelVTotal(MenuItemActionType action);
extern    WORD GetPanelVTotal(void);
extern    Bool AdjustPanelMaxHTotal(MenuItemActionType action);
extern    WORD GetPanelMaxHTotal(void);
extern    Bool AdjustPanelMinHTotal(MenuItemActionType action);
extern    WORD GetPanelMinHTotal(void);
/*extern    Bool AdjustPanelMaxVTotal(MenuItemActionType action);
extern    WORD GetPanelMaxVTotal(void);
extern    Bool AdjustPanelMinVTotal(MenuItemActionType action);
extern    WORD GetPanelMinVTotal(void);*/
extern    Bool AdjustPanelDCLK(MenuItemActionType action);
extern    WORD GetPanelDCLK(void);
extern    Bool AdjustPanelMaxDCLK(MenuItemActionType action);
extern    WORD GetPanelMaxDCLK(void);
extern    Bool AdjustPanelMinDCLK(MenuItemActionType action);
extern    WORD GetPanelMinDCLK(void);
extern        Bool AdjustSwing(MenuItemActionType action);
extern        WORD GetSwingValue(void);
#if 0
extern    Bool AdjustScaler0x42(MenuItemActionType action);
extern    WORD GetScaler0x42(void);
extern    Bool AdjustScaler0x43(MenuItemActionType action);
extern    WORD GetScaler0x43(void);
extern    Bool AdjustScaler0xF2(MenuItemActionType action);
extern    WORD GetScaler0xF2(void);
extern    Bool AdjustScaler0xF5(MenuItemActionType action);
extern    WORD GetScaler0xF5(void);
#endif

extern        Bool AdjustABPortSwap(MenuItemActionType action);
extern        Bool AdjustDualPixelOutput(MenuItemActionType action);

extern        Bool AdjustPNSwap(MenuItemActionType action);
extern       Bool AdjustChannelSwap(MenuItemActionType action);
extern        Bool AdjustMSBLSBExchange(MenuItemActionType action);
extern        Bool AdjustTIMode(MenuItemActionType action);
extern        Bool AdjustOddMSBLSBExchange(MenuItemActionType action);
extern        Bool AdjustEvenMSBLSBExchange(MenuItemActionType action);
extern        Bool AdjustOddRBExchange(MenuItemActionType action);
extern        Bool AdjustEvenRBExchange(MenuItemActionType action);
extern        Bool AdjustDECurrent(MenuItemActionType action);
extern        Bool AdjustClockCurrent(MenuItemActionType action);
extern        Bool AdjustOddCurrent(MenuItemActionType action);
extern        Bool AdjustEvenCurrent(MenuItemActionType action);
extern        Bool AdjustBurstModeFreq(MenuItemActionType action);
extern        WORD GetBurstModeFreq(void);
extern        Bool AdjustBurstModeFreq(MenuItemActionType action);
extern        WORD GetBurstModeFreq(void);
extern        Bool AdjustDefDuty(MenuItemActionType action);
extern        WORD GetDefDuty(void);
extern        Bool AdjustMinDuty(MenuItemActionType action);
extern        WORD GetMinDuty(void);
extern        Bool AdjustMaxDuty(MenuItemActionType action);
extern        WORD GetMaxDuty(void);
extern	Bool AdjustChangeModePanelVddOnOff(MenuItemActionType action);
extern        Bool AdjustSSCSTEPL(MenuItemActionType action);
extern        WORD GetSSCSTEPL(void);
extern        Bool AdjustSSCSTEPH(MenuItemActionType action);
extern        WORD GetSSCSTEPH(void);
extern        Bool AdjustSSCSPANL(MenuItemActionType action);
extern        WORD GetSSCSPANL(void);
extern        Bool AdjustSSCSPANH(MenuItemActionType action);
extern        WORD GetSSCSPANH(void);
extern Bool InitialPanelSetting(void);
extern Bool SaveCurrentPanelSetting(void);

#endif

#if EnableVcomAdjustItem||PANEL_VCOM_ADJUST		//121031 Modify
extern WORD GetVCOMValue(void);
extern Bool AdjustVCOMValue(MenuItemActionType action);
#endif




extern BYTE *Main_FText(void);
extern BYTE *MainMonoText(void);


extern BYTE *VersionDateText(void);
extern BYTE *ChangeDateText(void);
extern BYTE *ModelNameText(void);
extern BYTE *DateText(void);
extern BYTE *HourText(void);
extern BYTE *PotText(void);

#if PANELSSCSTEPLFUN
extern    BYTE *SSCSTEPLText(void);
extern    BYTE *SSCSTEPHText(void);
extern    BYTE *SSCSPANLText(void);
extern    BYTE *SSCSPANHText(void);
#endif


#if DISPLAY_CHECK_VOL
extern BYTE *ChecksumText(void);
//extern BYTE *Checksum(void);
#endif

extern BYTE *LuminanceText(void);
extern BYTE *ContrastText(void);
extern BYTE *BrightnessText(void);
extern BYTE *ECOModeText(void);
extern BYTE *ECOModeValue(void);
extern BYTE *GAMAText(void);
extern BYTE *GamaModeValue(void);

extern BYTE *DCRText(void);
extern BYTE *DcrValueText(void);

#if Enable_Lightsensor
extern BYTE *iCareText(void);
extern BYTE *iCareValueText(void);
#endif

extern BYTE *ImageSetupText(void);
extern BYTE *ClockText(void);
extern BYTE *FocusText(void);
extern BYTE *HPositionText(void);
extern BYTE *VPositionText(void);
extern BYTE *ColorTempText(void);
extern BYTE *ColorTempValueText(void);
extern BYTE *UserColorRText(void);
extern BYTE *UserColorGText(void);
extern BYTE *UserColorBText(void);
extern BYTE *UserColorYText(void);
extern BYTE *UserColorCText(void);
extern BYTE *UserColorMText(void);
#if AUTO_COLOR_ITEM_EN
extern BYTE *AutoColorText1(void);
#endif
extern BYTE *ColorBoostText(void);
extern BYTE *FullText(void);
extern BYTE *NatureSkinText(void);
extern BYTE *GreenFieldText(void);
extern BYTE *SkyBlueText(void);
extern BYTE *AutoDetectText(void);
extern BYTE *DemoText(void);
extern BYTE *FreeColorBarText(void);
extern BYTE *FreeColorFullModeText(void);
extern BYTE *FreeColorSkinProtectModeText(void);
extern BYTE *FreeColorGreenEnhanceModeText(void);
extern BYTE *FreeColorBlueEnhanceModeText(void);
extern BYTE *FreeColorAutoDetectModeText(void);
extern BYTE *FreeColorDemoModeText(void);
extern BYTE *BFHPositionText(void);
extern BYTE *BFVPositionText(void);
extern BYTE *BFPositionText(void);
extern BYTE *PictureBoostText(void);
extern BYTE *BF_FrameSizeText(void);
extern BYTE *BF_BrightnessText(void);
extern BYTE *BF_ContrastText(void);
extern BYTE *BF_HueText(void);
extern BYTE *BF_SaturationText(void);
extern BYTE *BF_PositionText(void);
extern BYTE *BF_RightArrowText(void);
extern BYTE *BF_SwitchText(void);
extern BYTE *BF_SwitchValueText(void);
extern BYTE *OsdSetUpText(void);
extern BYTE *OsdHPositionText(void);
extern BYTE *OsdVPositionText(void);
extern BYTE *OsdTransparenceText(void);
extern BYTE *OsdTimeOutText(void);
extern BYTE *LanguageText(void);
extern BYTE *LanguageValueText(void);
extern BYTE *LanguageValue1Text(void);
extern BYTE *Language1Text(void);
#ifdef EnableLightBar
extern BYTE *LightBarText(void);
#endif
extern BYTE *ExtraText(void);
#if 1//Dual
#if (!Youweipaike)
extern BYTE *InputSelectText(void);
#endif
#endif
#if HotInputSelect
#if (INPUT_TYPE == INPUT_1A1D || INPUT_TYPE == INPUT_1A2D)
extern BYTE *InputDigitalText(void);
#endif
extern BYTE *InputAnalogText(void);
#if (INPUT_TYPE == INPUT_1A2D || INPUT_TYPE == INPUT_1A1H)
extern BYTE *InputHDMIText(void);
#endif
#endif
extern BYTE *AutoConfigText(void);
extern BYTE *ResetText(void);
extern BYTE *InformationText(void);
#if(!Youweipaike)
extern BYTE *InputSourceText(void);
#endif
extern BYTE *DigitalText(void);
extern BYTE *YesText(void);
extern BYTE *NoText(void);
extern BYTE *ExtraBarText(void);
extern BYTE *InformationArrowText(void);
// 071218 reserved for extra page fw version
//extern BYTE *VersionText(void);
//extern BYTE *FW_VerText(void);
#if DDCCI_ENABLE
    extern BYTE *DDCCIText(void);
    extern BYTE *DDCCIValueText(void);
#endif 
#ifdef OffPower      //090331 xiandi.yu Auto Off Power
    extern BYTE *OffTimerText(void);
	extern BYTE *MonitorOffText_1(void);
	extern BYTE *MonitorOffText_2(void);
#endif 
#if Enable_Expansion
    extern BYTE *ExpansionText(void);
    extern BYTE *ExpansionValueText(void);
#endif 
extern BYTE *InformationExtraText(void);
extern BYTE *InputText(void);
extern BYTE *InputConnectorText(void);
extern BYTE *ResolutionText(void);
extern BYTE *HResText(void);
extern BYTE *VResText(void);
extern BYTE *X_Text(void);
extern BYTE *HFrequenceText(void);
extern BYTE *VFrequenceText(void);
extern BYTE *KHzText(void);
extern BYTE *HzText(void);
extern BYTE *AutoColorMenuText(void);
extern BYTE *AutoColorPassText(void);
extern BYTE *AutoColorFailText(void);
extern BYTE *NoSyncText(void);
/*
extern BYTE* CableNotConnectText(void);
 */
extern BYTE *OutOfRangeText(void);
extern BYTE *ResetText_1(void);
extern BYTE *ResetText_2(void);
extern BYTE *AutoText_1(void);
extern BYTE *AutoText_2(void);
extern BYTE *OsdLockText(void);
extern BYTE *COOL1Text(void);
extern BYTE *COOL2Text(void);
extern BYTE *WARM1Text(void);
extern BYTE *WARM2Text(void);
extern BYTE *sRGBText(void);
extern BYTE *NORMALText(void);
extern BYTE *EnterBurninText(void);
extern BYTE *ExitBurninText(void);
extern BYTE *DigitalText(void);
extern BYTE *AnalogText(void);
extern BYTE *InputTypeSelectText(void);
extern BYTE *VolumeText(void);
extern BYTE *MuteText(void);
//====================================
//extern BYTE* NoSyncText(void);
extern BYTE *CableNotConnected1Text(void);
extern BYTE *CableNotConnected2Text(void);
extern BYTE *OutOfRange1Text(void);
extern BYTE *AutoColorText(void);
extern BYTE *PassText(void);
extern BYTE *FailText(void);
extern BYTE *AdcGainText(void);
extern BYTE *AdcOffsetText(void);
extern BYTE *sRGBText(void);
extern BYTE *BankText(void);
extern BYTE *AddressText(void);
extern BYTE *ValueText(void);
extern BYTE *FactoryRedText(void);
extern BYTE *FactoryGreenText(void);
extern BYTE *FactoryBlueText(void);
extern BYTE *FactoryBrightnessText(void);
extern BYTE *FactoryContrastText(void);
extern BYTE *BurninText(void);
#if Enable_ProductModeAdjust
extern BYTE *ProductModeText(void);
extern BYTE *ProductModeOnOffText(void);
#endif
extern BYTE *PotText(void);
extern BYTE *HrsText(void);
extern BYTE *PanelText(void);
extern BYTE *PanelOnText(void);
extern BYTE *BankNameText(void);
extern BYTE *BurninOnOffText(void);
extern BYTE *FactoryIcon(void);
//===================================================================


extern BYTE *AutoSwitchValueText(void);
extern BYTE *ResetValueText(void);
extern BYTE *Input2Text(void);

#if INPUT_TYPE==INPUT_1A
extern BYTE * CableNotConnectText( void );
#endif

#if EnablePanelServiceMenu
extern    BYTE *PanelSelectText(void);
extern    BYTE *PanelTpyeText(void);
#endif

#if MS_OVD
extern   BYTE *OverDriveText(void);
extern BYTE *OverDriveOnOffText(void);
#endif

extern BYTE *IcModelText(void);


#if EnablePanelServiceMenu
extern BYTE *AdjustPanelParaText(void);
extern   BYTE *PanelDitherText(void);
extern    BYTE *ColorDepthText(void);
extern    BYTE *PanelOnTiming1Text(void);
extern    BYTE *PanelOnTiming2Text(void);
extern    BYTE *PanelOffTiming1Text(void);
extern    BYTE *PanelOffTiming2Text(void);
extern    BYTE *PanelHSyncWidthText(void);
extern    BYTE *PanelHSyncBackPorchText(void);
extern    BYTE *PanelVSyncWidthText(void);
extern    BYTE *PanelVSyncBackPorchText(void);
extern    BYTE *PanelWidthText(void);
extern    BYTE *PanelHeightText(void);
extern    BYTE *PanelHTotalText(void);
extern    BYTE *PanelVTotalText(void);
extern    BYTE *PanelMaxHTotalText(void);
extern    BYTE *PanelMinHTotalText(void);
/*extern    BYTE *PanelMaxVTotalText(void);
extern    BYTE *PanelMinVTotalText(void);*/
extern    BYTE *PanelDCLKText(void);
extern    BYTE *PanelMaxDCLKText(void);
extern    BYTE *PanelMinDCLKText(void);
extern    BYTE *SwingText(void);

extern    BYTE *ABPortSwapText(void);
extern    BYTE *DualPixelOutputText(void);
extern    BYTE *ABPortSwapOnOffText(void);
extern    BYTE *DualPixelOutputOnOffText(void);


extern    BYTE *PNSwapText(void);
extern    BYTE *ChannelSwapText(void);
extern    BYTE *MSBLSBExchangeText(void);
extern    BYTE *TIModeText(void);
extern    BYTE *OddMSBLSBExchangeText(void);
extern   BYTE *EvenMSBLSBExchangeText(void);
extern    BYTE *OddRBExchangeText(void);
extern    BYTE *EvenRBExchangeText(void);
/*extern    BYTE *DECurrentText(void);
extern    BYTE *ClockCurrentText(void);
extern    BYTE *OddCurrentText(void);
extern    BYTE *EvenCurrentText(void);
extern    BYTE *DECurrentValueText(void);
extern    BYTE *ClockCurrentValueText(void);
extern    BYTE *OddCurrentValueText(void);
extern    BYTE *EvenCurrentValueText(void);*/



extern    BYTE *PNSwapOnOffText(void);
extern    BYTE *ChannelSwapOnOffText(void);
extern    BYTE *MSBLSBExchangeOnOffText(void);
extern    BYTE *TIModeOnOffText(void);
extern    BYTE *OddMSBLSBExchangeOnOffText(void);
extern    BYTE *EvenMSBLSBExchangeOnOffText(void);
extern    BYTE *OddRBExchangeOnOffText(void);
extern    BYTE *EvenRBExchangeOnOffText(void);
extern    BYTE *BurstModeFreqText(void);
extern    BYTE *DefDutyText(void);
extern    BYTE *MinDutyText(void);
extern    BYTE *MaxDutyText(void);
extern    BYTE *ChangeModePanelVddOnOffText(void);
extern    BYTE *ChangeModePanelVddText(void);
extern    BYTE *SSCSTEPLText(void);
extern    BYTE *SSCSTEPHText(void);
extern    BYTE *SSCSPANLText(void);
extern    BYTE *SSCSPANHText(void);
extern    BYTE *InitialSettingText(void);
extern    BYTE *SaveSettingText(void);
#endif
#if EnableVcomAdjustItem||PANEL_VCOM_ADJUST		//121031 Modify
extern BYTE *VCOMText(void);
#endif


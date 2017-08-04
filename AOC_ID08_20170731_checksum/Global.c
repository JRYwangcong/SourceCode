#include "types.h"
#include "board.h"
#include "global.h"
#include "panel.h"
#include "Debug.h"
#if LANGUAGE_TYPE==ASIA || LANGUAGE_TYPE==All13Language || LANGUAGE_TYPE==All14Language_AddJap
    #include "menudef.h"
#endif
#if MWEFunction
    #include "MsDLC.h"
    #include "MsACE.h"
    #include "Userpref.h"
    /*
    BYTE  UserPrefSubRedGain=0;
    BYTE  UserPrefSubGreenGain=0;
    BYTE  UserPrefSubBlueGain=0;
     */
#endif
extern void RunLanguage(void);
#if ScanKey
#include "ScanKey.h"
BYTE TPDebunceCounter=0;
bit StartScanKeyFlag =0;
bit KeyReadyFlag = 0;
BYTE xdata KeyLedCounter=0;
    #if Scan_Source == Cypress
    BYTE xdata SKReadCaseCnt = 0;
    #ifdef LEDBrightCtrlByScalerPWM
    BYTE xdata SKLEDStatus = 0;
    bit bPowerLEDBrightActStepFlag = 0;
    #endif
    BYTE xdata SKKeyErrorCnt = 0;
    bit bKeyInitialFlag = 0; // indicate enlarge waiting ack time for scan key IIC bus // 081215
    #else // Scan_Source == Synaptics
    bit keyLEDTurnOnFlag=0;
    bit KeyLEDTurnOffFlag=0;
    bit KeyLEDStatus=0;
    #endif

    #ifdef LEDFlash
    BYTE xdata LEDFlashStep = 0;
    BYTE xdata LEDFlashCounter = 0;
    bit LEDFlashChangeFlag = 0;
    //bit LEDFlashActionFlag = 0
    #endif

    #ifdef EnableSlider
    BYTE xdata PreKeyStatus;
    BYTE xdata PreSliderStatus;
    BYTE xdata SliderIncDecStep;
    WORD xdata FingerOnCounter;
    #endif

    #ifdef PowerSavingAmberLEDFlash
    BYTE xdata LEDFlashCount = 0;
    bit LEDFlash100mSFlag = 0;	//120813 Modify
    //BYTE xdata LEDFlash100mSCount = 0;
    #endif

	#ifdef PowerSavingAmberLEDFlashing
	BYTE xdata LEDFlashCount = 0;
    #if PSAmberLEDFlashingType == Type_Breathing
		bit bPSLEDIncToLightFlag = 0;
		bit bLEDFlashingActFlag = 0;
    #else
		bit LEDFlash100mSFlag = 0;	//120813 Modify
    #endif
	#endif

	
#endif
#ifdef OffPower
	bit OffPower1sFlag = 0;
	bit ShowOffPowerWarningFlag = 0;
#endif
#if MainBoardType==MainBoard_715G3598_M0A // edit by lizzie. for MainBoard_715G3598_M0A  when power saving,system runs at 14.318MHz,and the 1s counter will be half.
bit SingalXTALFlag=0;
#endif
WORD ms_Counter = SystemTick; // for System timer
BYTE xdata ms10msCounter=10;
BYTE ModeDetectCounter = 20;
//BYTE Second=0;
WORD DelayCounter=0;
bit bDelayFlag = 0;
WORD xdata PanelOffOnDelayCntr = 0;
bit bPanelDelayFlag = 0;

// 110826 coding addition for recording back light active status
BYTE xdata BackLightActiveFlag;

#if ESaverPowerDownFunc
WORD ESaverPowerDownCounter=0;
#endif
BYTE  OsdCounter = 0;
BYTE xdata HotKeyCounter = 0;
BYTE xdata PowerDownCounter = 0;
BYTE xdata SourceKeyCounter =0;
BYTE xdata InputTimingStableCounter = 0;
BYTE xdata TimingChangeDebunce = 0;
BYTE xdata SwitchPortCntr = 0; // 071222
#ifdef OffPower          //090331 xiandi.yu Auto Off Power
BYTE xdata AutoPowerOffSTime=0;
BYTE xdata AutoPowerOffMTime=0;
BYTE xdata AutoPowerOffHTime=0;
#endif
//WORD SystemFlags=0;    // for system status flags
//BYTE System2Flags=0;
//BYTE PatternNo=0;
#if 0 // 2006/11/9 11:41AM by Emily  temp
    DWORD xdata BlackoffTime = 0;
#endif
DWORD xdata BlacklitTime = 0;
//BYTE LanguageState=0;
//BYTE CurrentLanguage=0;   //wumaozhong add 20050615
//BYTE ShortcutLuminance=0;
//BYTE ChipVer=0;
//BYTE SaveIndex=0; // saveIndex
WORD xdata SystemFlags = 0; // for system status flags
WORD xdata System2Flags = 0; // for system status flags
BYTE xdata System3Flags=0;
BYTE xdata PowerKeyCounter = 0;
//BYTE xdata KeypadFlags=0;
#if AllInOneKey
BYTE xdata SourceKeyCnter = 0;
#endif
BYTE xdata ChipVer = 0;
BYTE xdata SaveIndex = 0; // saveIndex
BYTE xdata PatternNo = 0;
//BYTE xdata BrightnessTemp = 0;
//BYTE xdata PrevHistoValue = 0;
WORD  xdata VScaleDecValue; // 2006/7/5 11:44PM by Emily for dec output dot clock
bit gBoolVisualKey = 0;
BYTE gByteVisualKey = 0xff;
BYTE LanguageIndex;
BYTE SecondTblAddr;

BYTE  MenuPageIndex = 0;
BYTE  MenuItemIndex = 0;

BYTE xdata BoostTempValue=0;

BYTE  xdata TermBuffer = 0;
#if ENABLE_HDCP
    //BYTE xdata HPD_Counter = 0;
    //BYTE xdata HDCPFlag = 0;
    // 2006/11/29 10:35AM by KK WORD xdata g_wHDCP_KeyChkSum;
    // 2006/11/29 10:35AM by KK WORD xdata g_HDCP_KeyCounter;
    //XDATA g_bHPD_Status;

    //bit HpdReadyFlag=0;
    bit HdcpNVRamFlag=0;

#endif

#if ENABLE_HDMI
BYTE xdata PreHDMIColor=0;
#endif


#if ENABLE_HDCP
    #if HDCPKEY_IN_Flash
    bit LoadHDCPKeyFlag=0;
    bit WriteHDCPcodeFlag=0;
        BYTE xdata HDCPDataBuffer[6]; //[6];
        WORD xdata TCRCvalue = 0;
        #ifdef HDCP_IN_FLASH_NEW_WAY
        BYTE xdata HDCPKeySaveIndex=0;//Zhifeng.wu
        #endif
    #endif
#endif

#if 0//ENABLE_HDMI
bit bDigitalGraphicReset=0;
#endif
#if USEFLASH
FactorySettingType xdata  FactorySetting;
#endif

#ifdef  UseVGACableReadWriteAllPortsEDID
	BYTE xdata SIORxBuffer[MaxBufLen];
#else
    #ifdef DATA_LENGTH_20_BYTE
    BYTE xdata SIORxBuffer[23];
    #else
    BYTE xdata SIORxBuffer[8];
    #endif
#endif

#ifdef UseVGACableReadWriteAllPortsEDID
    #ifdef ReduceDDC
    	bit LoadEDIDFlag=0;
	#else
		BYTE xdata LoadEDIDSelectPort = 0xFF ;
    #endif
    bit WriteDeviceFlag=0;
#endif
#if 1 //ReduceDDC
 BYTE xdata DDC1[128];
#endif
#ifdef ReduceDDC
bit EnCheckVccFlag = 0;
BYTE xdata SystemVccDropDownCnt = 0;
#endif
#if WaitingForDrawOSDSUB
bit WaitingForOSDFlsh=0; // 081125
#endif




 #if AutoProcess
//zjb20071218
WORD xdata wAutoProcessCount=0;
WORD xdata wAutoProcess0=0;
WORD xdata wAutoProcess1=0;
#endif
#if Backupdata
    //BYTE BackFlags;
    BYTE DDCCIAdjCounter = 0;

    bit GetChecksumFlag=0;
    bit DDCCIAadjustFlag=0;
    bit DonotchangeBackFlag=0;
    bit ACONFlag=0;

#endif
BYTE xdata FlashStatus=0;

//bit SaveFactorySettingFlag=0;
#if EnablePanelServiceMenu
BYTE xdata PanelSelect=0;
bit SavePanelSettingFlag=0;
bit TestMinDutyFlag=0;
PanelSettingType xdata PanelSetting;
#endif
#if PANEL_VCOM_ADJUST		//121031 Modify
BYTE xdata g_bACPowerOn_CheckVCOM = FALSE ;
#endif


InputTimingType idata InputTiming;
MonitorSettingType xdata MonitorSetting; //Benz 2007.4.15   19:44:07idata - >xdata
ModeSettingType xdata ModeSetting;
#if USEFLASH
MonitorSettingType2 xdata  MonitorSetting2;
#endif

#if FDDCCI_WriteEDID
BYTE  xdata ReloadedidCounter=0;
#endif

BYTE xdata LastMenuPage=0;
//==========================for AutoBacklight=====
BYTE xdata HistogramNow = 0;
BYTE xdata HistogramPrev = 0;
BYTE xdata BacklightNow = 0;
BYTE xdata BacklightPrev = 0;
BYTE xdata HistogramFlag = 1;
//xdata BYTE xfr_regs[256] _at_ 0xC000;
xdata BYTE g_FlashID = 0;
xdata BYTE EnterKeyCouter= 0;


//bit InputTimingChangeFlag=0;
//bit PowerSavingFlag=0;
//bit ForcePowerSavingFlag=0;
//bit InputTimingStableFlag=0;
//bit DoModeSettingFlag=0;
//bit CableNotConnectedFlag=0;
//bit FreeRunModeFlag=0;
bit RxBusyFlag=0;
//bit OsdTimeoutFlag=0;
//bit ChangePatternFlag=0;
//bit SaveSettingFlag=0;
bit ms500Flag=0;
//bit ReverseXFlag=0;
//bit ReverseYFlag=0;
//bit ISPFlag=0;

#if FDDCCI_WriteEDID
bit ubAutoAlign=0;
#endif
bit AC_PowerFlag=0;

//bit DisplayLogoFlag=0;
bit FlashFlag=0;
//bit ReadyForChangeFlag=0;
//bit LoadOSDDataFlag=0;
//bit LoadFontFlag=0;
//bit AutoColorFlag=0;


bit DLKOverFlag=0;

//bit ShowInputInfoFlag=0;
//bit JustShowValueFlag=0;
bit DecVScaleFlag	=0;
//bit PanelOnFlag=0;

//bit PressExitFlag=0;
bit EnableShowDDCFlag=0;
//bit PressMenuFlag=0;
//bit MuteFlag=0;
#if Enable_Expansion
//bit ExpansionFlag=0;
WORD xdata DispalyWidth = PANEL_WIDTH;
WORD xdata HStartOffset;
#endif

//bit SecondFlag=0;
//bit DebugOnlyFlag=0;
bit DcrOpenScikeFlag=0;



bit MainOSDFlag=0;
bit SubOSDFlag=0;
bit PowerKeyFlag=0;
bit PowerOffFlag=0;
bit DoAutoConfigFlag=0;
bit DoResetFlag=0;
bit DoKeyCountFlag=0;

//bit ProductModeFlag=0;
#if AllInOneKey
#if !ScanKey
bit FactoryPowerOnFlag=0;
#endif
#endif

//#if (ModelName == ENVISION_P2271w || ModelName == ENVISION_P2471w||ModelName==ENVISION_P971wl)//|| ModelName==ENVISION_P2071l)
#ifdef GreenLEDDutyCtrl
bit DuringResetFlag=0;
#endif

//bit PowerGoodExistFlag = 0;
#if DisplayEnergyStar
bit ShowEpaAfterLogoFlag=0;		//071228
#endif
#if OSD_PIVOT_EN//1226 Changed
bit OSDRotateFlag =0;
BYTE xdata PivotStatusDebunce=0;
#endif
#if Enable_Lightsensor
bit bLightSensorActStepFlag = 0; // unit: set the flag per LightSensorStepCnt 
BYTE xdata TempBrightness= 0;
bit DetectBriFlag = 0;	//120813 Modify
BYTE xdata SetBriValue = 0;
WORD xdata LightSensorAdcOld = LIGHT_SENSOR_ADC_MAX, LightSensorAdcNew = LIGHT_SENSOR_ADC_MAX;	//fido 20080526* <==	WORD xdata LightSensorAdcOld, LightSensorAdcNew;	//fido 20070328+
    #if LightSensor_Source == EverLight
    BYTE xdata SensorErrorCnt = 0;
    #else
    #if TSL2561
    BYTE xdata SensorErrorCnt = 0;
    #endif
    #endif
#endif
#if !ENABLE_HDMI
extern BYTE code nullData[1];
#endif


#if DISPLAY_CHECK_VOL
WORD xdata  Code_checksun;
#endif

#if BlacklitTimeOnShow
BYTE 	xdata BlacklitTimeOn_S=0;
BYTE 	xdata BlacklitTimeOn_M=0;
WORD 	xdata BlacklitTimeOn_H=0;
//BYTE 	xdata BlacklitTimeOn_HH=0;
//BYTE 	xdata BlacklitTimeOn_SShift=0;
#endif

void Init_GlobalVariables(void)
{
    #if Backupdata
        //BackFlags = 0;
    #endif
    #if !ENABLE_HDMI
    // prevent complier warning.
    BYTE temp = nullData[0];
    #endif    
    #if ScanKey
    Set_StartScanKeyFlag();
    
    #if Scan_Source == Synaptics
    Clr_keyLEDTurnOnFlag();
    Clr_keyLEDTurnOffFlag();
	//#if (ModelName == ENVISION_P2271w || ModelName == ENVISION_P2471w||ModelName==ENVISION_P971wl)//|| ModelName==ENVISION_P2071l)
	#ifdef GreenLEDDutyCtrl
	Clr_DuringResetFlag();
	#endif
    KeyLEDStatus = KeyLEDStatus_Off;
    #endif
    
    #endif
    InputTimingStableCounter = 0;
    SwitchPortCntr = 0;//071226
    //Second=0;
    OsdCounter = 0;
    HotKeyCounter = 0;
    PowerKeyCounter = 0;
    PowerDownCounter = 0;
#if AllInOneKey
    SourceKeyCnter = 0;	
#endif
    #if USEFLASH
	BlacklitTime=MonitorBackLightTime;
    #endif
	
    #if ESaverPowerDownFunc
    ESaverPowerDownCounter=0;
    #endif

    #if Enable_Lightsensor
    // 091002 coding, initial variable of light sensor
    SetBriValue = TempBrightness = DefBrightness; 
    #endif



   #if  BlacklitTimeOnShow
   BlacklitTimeOn_M=MonitorBlacklitTimeOn_M;
   BlacklitTimeOn_S=MonitorBlacklitTimeOn_S;
   BlacklitTimeOn_H=MonitorBlacklitTimeOn_H;
   //BlacklitTimeOn_HH=MonitorBlacklitTimeOn_HH;
   #endif
		

	#if HKC_ColorTemp_Adjust||FDDCCI_WriteEDID
	Clr_ATEModeFlag();
	Clr_DDCCIResetPowerOffFlag();
	#endif
	
    #if 0//ENABLE_HDMI
    bDigitalGraphicReset=1;
    #endif
    SrcFlags = SyncLoss;
    if (BurninModeFlag)
    {
        Set_DoBurninModeFlag();
    }
    else
    {
        Clr_DoBurninModeFlag();
    }
    // printData("Monitor flag before init %d",MonitorFlags);
    if (PowerOnFlag)
    {
        //SystemFlags |= bInputTimingChangeBit;
        Set_InputTimingChangeFlag();
        #if DisplayPowerOnLogo
	
            if (!(ProductModeFlag||FactoryModeFlag))
            //eshin
                Set_DisplayLogoFlag();
        #endif
        //printMsg("Power On--->from monitorflag");
       #if DisplayEnergyStar
        Set_ShowEpaAfterLogoFlag();// 1228
        #endif
    }
    else
    {
        Set_ForcePowerSavingFlag(); // 080917 addition for dc off
        Clr_DisplayLogoFlag();
        //printMsg("Power Off-->force power saving");
    }

	RunLanguage();//edit by lizzie.ding 100601, for new define language,don't forget define language address
	
    #if ENABLE_HDCP
        //Clr_HpdReadyFlag();
        //HPD_Counter = 2;
        #if 0//ENABLE_NEW_HDCP_RULE
            g_bHPD_Status = 1;
        #endif
    #endif
	
    #if MWEFunction
        // disable sub win
        //UserPrefMWE=OFF_W;
        // disable DLC function
        // 2006/10/26 6:47PM by Emily     UserPrefDLCMode = OFF_DLC;
        /*
        UserPrefSubRedGain=128;
        UserPrefSubGreenGain=128;
        UserPrefSubBlueGain=128;
         */
        //
        // UserPrefSkinTone=OFF_SKIN;
        //Benz 2007.4.24   9:48:33 why? Clr_BlueFlag();
        //Benz 2007.4.24   9:48:40 why? Clr_GreenFlag();
        //   Clr_ACE2Flag();
        Clr_MWEModeFlag();
        // UserPrefMWEMode=OFF_MWEMODE;
    #endif
	#if Dual
       if (!(ProductModeFlag||FactoryModeFlag))
	Set_ShowInputInfoFlag();
	#endif
	
	#ifdef OffPower
	OffPower1sFlag = 0;
	ShowOffPowerWarningFlag = 0;
	#endif
	
}

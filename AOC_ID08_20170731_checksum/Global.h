#ifndef __GLOBAL_H__
    #define __GLOBAL_H__
    #include "mode.h"
	#include "panel.h"
    #include "userprefdef.h"
    #include "Board.h"
   #define   PrintAutoMsgPeriod	20//23//20//25
   #define   WaitForDataBlanking     1   //zjb20071219

   #define MaxBufLen 50       //peter.yu 20120627

#if ScanKey

extern BYTE TPDebunceCounter;
extern bit StartScanKeyFlag;
extern bit KeyReadyFlag;
extern BYTE xdata KeyLedCounter;
    #if Scan_Source == Cypress
    extern BYTE xdata SKReadCaseCnt;
    #ifdef LEDBrightCtrlByScalerPWM
    extern BYTE xdata SKLEDStatus;
    extern bit bPowerLEDBrightActStepFlag;
    #define PowerLEDBrightStepCnt    20 // unit: mS
    #define SKLEDTurnDarkTime   5 // unit: second
    #define LEDSetting_Off   		0xff
    // 091001 coding, LED bright define
    #define LEDSetting_Dark 		0xff//for SVT keyboard //091023  only for SIT TEST 0xff , by lizzie// 091012 GPM request//0xf0//0xE0//0xb0
    #define LEDSetting_Light    		0x00//0xB0//0x00
    #if MainBoardType == MainBoard_715G3603_M0A
    #define LEDPWM      			PWM1C // PWM1
    #elif MainBoardType == MainBoard_715G3598_M0A
	#ifdef TSUMXXNX
	#define LEDPWM      			PWM2C // PWM1
	#else
	#define LEDPWM      			PWM0C // PWM1
	#endif
	#elif MainBoardType == MainBoard_715G3598_1PLL
	#define LEDPWM					PWM2C 
    #endif
    #endif
    extern BYTE xdata SKKeyErrorCnt;
    extern bit bKeyInitialFlag; // indicate enlarge waiting ack time for scan key IIC bus
    #else // Scan_Source == Synaptics
    extern bit keyLEDTurnOnFlag;
    extern bit KeyLEDTurnOffFlag;
    extern bit KeyLEDStatus;
    #define Set_keyLEDTurnOnFlag()		(keyLEDTurnOnFlag=1)
    #define Clr_keyLEDTurnOnFlag()  		(keyLEDTurnOnFlag=0)
    #define Set_keyLEDTurnOffFlag()		(KeyLEDTurnOffFlag=1)
    #define Clr_keyLEDTurnOffFlag()  		(KeyLEDTurnOffFlag=0)

    #define KeyLEDStatus_Off  0
    #define KeyLEDStatus_On   1
    #endif

#define DefTPDebunceCounter		30
#define Set_StartScanKeyFlag()	     (StartScanKeyFlag=1)
#define Clr_StartScanKeyFlag()	     (StartScanKeyFlag=0)
#define Set_KeyReadyFlag()	     (KeyReadyFlag=1)
#define Clr_KeyReadyFlag()	     (KeyReadyFlag=0)

    #ifdef LEDFlash
    extern BYTE xdata LEDFlashStep;
    extern BYTE xdata LEDFlashCounter;
    extern bit LEDFlashChangeFlag;
    //extern bit LEDFlashActionFlag;
    #define Set_LEDFlashChangeFlag()    (LEDFlashChangeFlag=1)
    #define Clr_LEDFlashChangeFlag()    (LEDFlashChangeFlag=0)
    //#define Set_LEDFlashActionFlag()    (LEDFlashActionFlag=1)
    //#define Clr_LEDFlashActionFlag()    (LEDFlashActionFlag=0)
    #define LEDFlashChangeInterval  100
    #endif

    #ifdef EnableSlider
    extern BYTE xdata PreKeyStatus;
    extern BYTE xdata PreSliderStatus;
    extern BYTE xdata SliderIncDecStep;
    extern WORD xdata FingerOnCounter;
    #endif

    #ifdef PowerSavingAmberLEDFlash
    extern BYTE xdata LEDFlashCount;
    extern bit LEDFlash100mSFlag;	//120813 Modify
    //extern BYTE xdata LEDFlash100mSCount;
    #endif

	#ifdef PowerSavingAmberLEDFlashing
	extern BYTE xdata LEDFlashCount;
    #if PSAmberLEDFlashingType == Type_Breathing
		extern bit bPSLEDIncToLightFlag;
		extern bit bLEDFlashingActFlag;
    #else
		extern bit LEDFlash100mSFlag;	//120813 Modify
    #endif
	#endif

#endif
     #ifdef OffPower
    extern bit OffPower1sFlag;
	extern bit ShowOffPowerWarningFlag;
     #endif
#if MainBoardType==MainBoard_715G3598_M0A // edit by lizzie. for MainBoard_715G3598_M0A  when power saving,system runs at 14.318MHz,and the 1s counter will be half.
     extern bit SingalXTALFlag;
#endif

#if FDDCCI_WriteEDID
extern BYTE  xdata ReloadedidCounter;
#endif

    extern WORD ms_Counter; // for System timer
    extern BYTE xdata ms10msCounter;
    extern BYTE ModeDetectCounter;
    extern WORD DelayCounter;
    extern bit bDelayFlag;
    extern BYTE  MenuPageIndex;
    extern BYTE  MenuItemIndex ;
    extern BYTE  xdata TermBuffer ;
    extern WORD xdata PanelOffOnDelayCntr;
    extern bit bPanelDelayFlag;
    extern BYTE xdata BackLightActiveFlag;
	
#if ESaverPowerDownFunc
	extern WORD ESaverPowerDownCounter;
#endif

 #if AutoProcess
    //zjb20071218
    extern  WORD xdata wAutoProcessCount;
    extern  WORD xdata wAutoProcess0;
    extern  WORD xdata wAutoProcess1;
#endif
    extern BYTE xdata BoostTempValue;

extern BYTE xdata FlashStatus;
    //extern BYTE Second;
    extern WORD xdata SystemFlags;	 // for system status flags
    extern WORD xdata System2Flags;
	 extern BYTE xdata System3Flags;
	 
    extern BYTE xdata InputTimingStableCounter;
    //extern WORD xdata SystemFlags; // for system status flags
    //extern WORD  System2Flags;
	//extern BYTE xdata KeypadFlags;
    extern BYTE xdata PowerKeyCounter;
    extern BYTE xdata SourceKeyCounter;
#if AllInOneKey
    extern BYTE xdata SourceKeyCnter;
#endif
    extern BYTE  OsdCounter;
    extern BYTE xdata HotKeyCounter;
    extern BYTE xdata PowerDownCounter;
    extern BYTE xdata TimingChangeDebunce;
    extern BYTE xdata SwitchPortCntr;//071226
    //extern BYTE PatternNo;
    #ifdef OffPower            //090331 xiandi.yu Auto Off Power
    extern BYTE xdata AutoPowerOffSTime;
    extern BYTE xdata AutoPowerOffHTime;  
    extern BYTE xdata AutoPowerOffMTime;
	#endif
    extern DWORD xdata BlacklitTime;
    extern DWORD xdata BlackoffTime;
    extern bit gBoolVisualKey;
    extern BYTE gByteVisualKey;
    //extern BYTE ChipVer;
    //extern BYTE SaveIndex;
    extern BYTE xdata ChipVer;
    extern BYTE xdata SaveIndex;
    extern BYTE xdata PatternNo;

extern xdata BYTE EnterKeyCouter;
    /*
    extern BYTE  UserPrefSubRedGain;
    extern BYTE  UserPrefSubGreenGain;
    extern BYTE  UserPrefSubBlueGain;
     */
    //extern BYTE xdata BrightnessTemp;
    //extern BYTE xdata PrevHistoValue;
    extern WORD  xdata VScaleDecValue; // 2006/7/5 11:45PM by Emily
    #if PanelType==PanelCLAA156WB11A||PanelType==PanelCMON156B6L0B||PanelType==PanelBOEHT156WXB || PanelType == PanelTPM220Z1_ZGEL01
    #define VSCALE_DECVALUE	20
    #elif((PanelType == PanelHB156WX1_100) || (PanelType == PanelLTN156AT24_P))
    #define VSCALE_DECVALUE	28
    #else
    #define VSCALE_DECVALUE	50   // 2006/7/6 0:2AM by Emily
    #endif
    #if ENABLE_HDCP
        //extern xdata BYTE HPD_Counter;
        //extern xdata BYTE HDCPFlag;
        // 2006/11/29 10:36AM by KK extern WORD xdata g_wHDCP_KeyChkSum;
        // 2006/11/29 10:36AM by KK extern WORD xdata g_HDCP_KeyCounter;
        #if 0//ENABLE_NEW_HDCP_RULE
            extern XDATA g_bHPD_Status;
        #endif
    #endif

#if ENABLE_HDMI
extern BYTE xdata PreHDMIColor;
#endif

#if WaitingForDrawOSDSUB
extern bit WaitingForOSDFlsh; // 081125
#endif

#if 0//ENABLE_HDMI
extern bit bDigitalGraphicReset;
#endif

#ifdef	UseVGACableReadWriteAllPortsEDID
#if !defined(ReduceDDC)
extern BYTE xdata LoadEDIDSelectPort;
#endif
typedef enum
{
	VGA1,// 0
	DVI1,// 1
	HDMI1,// 2
	DP1,// 3
	VGA2,// 4
	DVI2,// 5
	HDMI2,// 6
	DP2,// 7
	EEPROM=0x10,
	HDCP1,
	HDCP2,//for DP
	OSDEDIDSN,//OSD EDID Serial Number(SN)  but AOC not use
	OSDEDIDModelName,//OSD EDID Model Name but AOC not use
	PanelSettingDevice//Panel Setting
}DeviceID;
typedef enum
{
	VGA_Port1=BIT7,
	VGA_Port2=BIT6,
	DVI_Port1=BIT5,
	DVI_Port2=BIT4,
	HDMI_Port1=BIT3,
	HDMI_Port2=BIT2,
	DP_Port1=BIT1,
	DP_Port2=BIT0,
}InputPortID;
#endif

#ifdef  UseVGACableReadWriteAllPortsEDID
	extern BYTE xdata SIORxBuffer[MaxBufLen];
#else
    #ifdef DATA_LENGTH_20_BYTE
    extern BYTE xdata SIORxBuffer[23];
    #else
    extern BYTE xdata SIORxBuffer[8];
    #endif
#endif
   #if 1// ReduceDDC
    extern BYTE xdata DDC1[128];
   #endif
#ifdef ReduceDDC
extern bit EnCheckVccFlag;
extern BYTE xdata SystemVccDropDownCnt;
#endif

    //extern xdata BYTE xfr_regs[256];
    #define xfr_regs ((BYTE xdata*)0xC000)

    extern xdata BYTE g_FlashID;
    extern BYTE LanguageIndex;
    extern BYTE SecondTblAddr;
    extern BYTE xdata HistogramNow;
    extern BYTE xdata HistogramPrev;
    extern BYTE xdata HistogramFlag;
    extern BYTE xdata BacklightNow;
    extern BYTE xdata BacklightPrev;
    #if CompressPropFont
        typedef WORD PropFontType;
    #else
        typedef struct
        {
            BYTE SpaceWidth;
            WORD LineData[18];
        } PropFontType;
    #endif
    #if ENABLE_HDCP
        #if HDCPKEY_IN_Flash
            extern BYTE xdata HDCPDataBuffer[6];
            extern WORD xdata TCRCvalue;
            #ifdef HDCP_IN_FLASH_NEW_WAY
            extern BYTE xdata HDCPKeySaveIndex;//Zhifeng.wu
            #endif
        #endif
    #endif
    #if Backupdata
        //extern BYTE BackFlags;
        extern BYTE DDCCIAdjCounter;
    #endif

#if DISPLAY_CHECK_VOL
extern WORD xdata  Code_checksun;			//Added by dragonheb for checksum
#endif
	

    #if USEFLASH
       extern FactorySettingType xdata FactorySetting;
    #endif
    extern bit ProductModeFlag;
	
    #define BKSVDataAddr  0
    #define HDCPKetAddr     5
    extern InputModeType code StandardMode[];
    extern InputResolutionType code StandardModeResolution[];
    extern InputTimingType idata InputTiming;
    extern MonitorSettingType xdata MonitorSetting;
	#if USEFLASH
	extern MonitorSettingType2 xdata  MonitorSetting2;
	#endif

#if EnablePanelServiceMenu
    extern PanelSettingType xdata PanelSetting;
    extern BYTE xdata PanelSelect;
#endif	
    extern ModeSettingType xdata ModeSetting;
    extern BYTE xdata LastMenuPage;

#if PANEL_VCOM_ADJUST		//121031 Modify
	extern BYTE xdata g_bACPowerOn_CheckVCOM;
#endif



    #if EnableVcomAdjustItem//def Panel_RSDS
        #if MainBoardType==MainBoard_3289_A2
        #define DefVcomValue       0x6A
        #elif MainBoardType==MainBoard_715G3289_C2
		#if PanelType==PanelTPM215HW01H1PS1
        	#define DefVcomValue       0xAD//0x70
		#else
       	#define DefVcomValue       0x70
              #endif
        #else
        #define DefVcomValue       0x80
        #endif
    #endif

#if ModelName==ENVISION_JRYL1PTR_EV1
    #define DEF_OSD_LANG        LANG_English
#else
    #define DEF_OSD_LANG        LANG_SChina
#endif
	#define DEF_CTEMP		   CTEMP_Warm1

#if (PanelType==PanelM185WHB_N10||PanelType==PanelLM185TT3A)
    #define DefBrightness	 80
#else
    #define DefBrightness	 90
#endif

#if PanelType==PanelLGLP156WHB_DP
    #define DefContrast	(120+10)//128//106//96 // 92 // 0x5c // 0x60 //32~96~160 171(0xAB)
#else 
    #define DefContrast	180//160+50//175//160//120//128//106//96 // 92 // 0x5c // 0x60 //32~96~160 171(0xAB)
#endif
    #define DefRedColor	0x81//0xAB // 96(0x60)
    #define DefGreenColor	0x81//0xAB //
    #define DefBlueColor	0x81//0xAB
    #define DefColor		0x81//0cxAB for YCM use

    #define DefHue					50
    #define DefSaturation			0x80
	
    #define MinSubContrast			55
    #define MaxSubContrast		255
	
    #define MinSubBrightness		55
    #define MaxSubBrightness		255
	
    #define DefSubContrast			((MaxSubContrast-MinSubContrast)/2+MinSubContrast)
    #define DefSubBrightness		((MaxSubBrightness-MinSubBrightness)/2+MinSubBrightness)
	
    #if Enable_Expansion
	  #if WidePanel
        #define DefExpansion	Expansion_Full
	  #else
	    #define DefExpansion	Expansion_Aspect  //071222
	  #endif	
    #endif


//For User Brightness
#define RealMaxBrightnessValue	0xFF
#define RealMinBrightnessValue	0x7F

//For Min DCR Brightness
#if (MainBoardType==MainBoard_2186_1)
#undef RealMinBrightnessValue
#define RealMinBrightnessValue	0x1F
#define RealAbsoluteMinBrightnessValue	0x00
#elif MainBoardType==MainBoard_715G3603_M0A
#define RealAbsoluteMinBrightnessValue	0x00
#elif MainBoardType==MainBoard_715G3598_M0A || MainBoardType==MainBoard_715G3598_1PLL
   #if(ModelName==AOC_2237Fw)
      #define RealAbsoluteMinBrightnessValue	0x0A
   #else
      #define RealAbsoluteMinBrightnessValue	0x10
   #endif
#elif(MainBoardType==MainBoard_2186_2A)
#define RealAbsoluteMinBrightnessValue	0x47
#elif(MainBoardType==MainBoard_2698_2_X_X_8)
#define RealAbsoluteMinBrightnessValue	0x5F
#elif(MainBoardType==MainBoard_2810_B)
#define RealAbsoluteMinBrightnessValue	0x6E
#elif(MainBoardType==MainBoard_2698_2_X_X_2)
#define RealAbsoluteMinBrightnessValue	0x43//0x6F
#elif(MainBoardType==MainBoard_715G2575)
#define RealAbsoluteMinBrightnessValue	0x54
#elif (MainBoardType==MainBoard_715G2808 || MainBoardType==MainBoard_715G2988)
#define RealAbsoluteMinBrightnessValue	0x4C
#elif (MainBoardType==MainBoard_715G2904_B||MainBoardType==MainBoard_715G3529||MainBoardType==MainBoard_715G3529_T)
	#if(ModelName==AOC_Q156W)
	#define RealAbsoluteMinBrightnessValue	0x60
	#elif(ModelName==AOC_Q17W)
	#define RealAbsoluteMinBrightnessValue	0x60
	#elif(ModelName==AOC_917Sw)
	#define RealAbsoluteMinBrightnessValue	0x39
	#elif(ModelName==AOC_831S_Plus)
		#if PanelType == PanelM185B1L01
			#define RealAbsoluteMinBrightnessValue	0x3F
		#else
			#define RealAbsoluteMinBrightnessValue	0x3F
		#endif
	#else
	#define RealAbsoluteMinBrightnessValue	0x4F
	#endif
#elif(MainBoardType==MainBoard_715G4734)
#define RealAbsoluteMinBrightnessValue	0x4F
#elif (MainBoardType==MainBoard_2883_B_X_X_1)
	#if(ModelName==AOC_919Vwa_Asia||ModelName==AOC_919Vwa_Europe)
	#undef RealMaxBrightnessValue
	#define RealMaxBrightnessValue		0xD6//0xDB
	#define RealAbsoluteMinBrightnessValue	0x2F//0x2C
	#elif(ModelName==AOC_F19)
	#define RealAbsoluteMinBrightnessValue	0x46
	#else
	#define RealAbsoluteMinBrightnessValue	0x30  
	#endif
#elif (MainBoardType==MainBoard_2883_B_X_X_2)
	#if (ModelName==AOC_930FWz)
	#define RealAbsoluteMinBrightnessValue	0x66//0x5F
	#elif (ModelName==AOC_931Fwz)
	#undef RealMaxBrightnessValue
	#define RealMaxBrightnessValue		0xF8
	#define RealAbsoluteMinBrightnessValue	0x4A
	#elif (ModelName==AOC_931Fwz)
	#define RealAbsoluteMinBrightnessValue	0x4A
	#elif (ModelName==AOC_919Va2)
	#define RealAbsoluteMinBrightnessValue	0x38//(22)//0x35(21)//0x4C(30)//0x3A(23)
	#elif (ModelName==ENVISION_G2460)
	#define RealAbsoluteMinBrightnessValue	0x20//(22)//0x35(21)//0x4C(30)//0x3A(23)
	#elif (ModelName==ENVISION_G2219_LM220WE1)
	#define RealAbsoluteMinBrightnessValue	0x10
	#else
	#define RealAbsoluteMinBrightnessValue	0x30
	#endif
#elif(MainBoardType==MainBoard_2670_B||MainBoardType==RDG2670_C_4_X_1_080611)
#define RealAbsoluteMinBrightnessValue	0x4F//0x66
#elif(MainBoardType==MainBoard_2883_1B||MainBoardType==MainBoard_715G3329_A2||\
         MainBoardType==MainBoard_715G3598_M0A||MainBoardType==MainBoard_712G3244_2||\
         MainBoardType==MainBoard_715G3244_TSUMU18NR || MainBoardType==MainBoard_715G3598_1PLL||\
         MainBoardType==MainBoard_715G3329_58N||MainBoardType==MainBoard_715G3329_18N)
#if ModelName==AOC_2219Vg||ModelName==AOC_2330V
#define RealAbsoluteMinBrightnessValue	0x23
#elif ModelName==ENVISION_P2151
#define RealAbsoluteMinBrightnessValue	0x39//0x3D
#elif ModelName==AOC_2430V
#define RealAbsoluteMinBrightnessValue	0x1B
#elif ModelName==ENVISION_P2471w ||ModelName==ENVISION_P2271w||ModelName==ENVISION_P971wl|| ModelName==ENVISION_P2071l||ModelName == ENVISION_P2271wl||\
	ModelName==ENVISION_P971l||ModelName==ENVISION_P2471||ModelName==ENVISION_P2371l||ModelName==ENVISION_P971wl1||ModelName==ENVISION_P2371l2||ModelName==ENVISION_P971waL2
#define RealAbsoluteMinBrightnessValue	0x08//#define RealAbsoluteMinBrightnessValue	0x10
#elif ModelName==AOC_2330V_PLUS||ModelName==AOC_2330V_14L
#define RealAbsoluteMinBrightnessValue	0x08
#else
#define RealAbsoluteMinBrightnessValue	0x30
#endif
#elif(MainBoardType==MainBoard_715G3289_C2)
#define RealAbsoluteMinBrightnessValue	0x13
#elif(MainBoardType==MainBoard_715G4175_M0A)
#define RealAbsoluteMinBrightnessValue	0x00
#elif(MainBoardType==MainBoard_715G3329_1_2)
#define RealAbsoluteMinBrightnessValue	0x0A
#elif(MainBoardType==MainBoard_4432)
#define RealAbsoluteMinBrightnessValue	0x00
#elif(MainBoardType==MainBoard_715G3959_T)		//110617 Modify
#define RealAbsoluteMinBrightnessValue	0x30
#elif(MainBoardType==MainBoard_715G3834)		//hong add 20120201
#define RealAbsoluteMinBrightnessValue	0x30
#elif(MainBoardType==MainBoard_715G5265_T)//petit 20121030
#define RealAbsoluteMinBrightnessValue	0x00
#elif (MainBoardType==MainBoard_3834_18T)//petit 20121228
#define RealAbsoluteMinBrightnessValue	0x30
#elif(MainBoardType==MainBoard_715G6952_M0B)//Gino 20140522
#define RealAbsoluteMinBrightnessValue	0x30
#elif(MainBoardType==MainBoard_715G6912_M1A)//Gino 20140522
#define RealAbsoluteMinBrightnessValue	0x30
#elif (MainBoardType==MainBoard_SLB1PTR||MainBoardType == MainBoard_JRYL1PTR_LF)	//lixw 20151120
//#undef RealMaxBrightnessValue
//#define RealMaxBrightnessValue		0xE5
#undef RealMinBrightnessValue
#define RealMinBrightnessValue	0x75//0x96
#define RealAbsoluteMinBrightnessValue	0x1F
#else
#define RealAbsoluteMinBrightnessValue	0x11
#endif

#define LowBoundDCRRealBriRatio          30     // CCFL: 30%, LED:50%
#define RealMinDcrBrightnessValue    ((((RealMaxBrightnessValue - RealMinBrightnessValue) * LowBoundDCRRealBriRatio) / 100) + RealMinBrightnessValue)

	
           #define DefRedGain				99
	    #define DefGreenGain				103
	    #define DefBlueGain				101
		
	    #define DefRedOffset				141
	    #define DefGreenOffset				129
	    #define DefBlueOffset				122

#if PanelType==PanelHSM215EU_HTN01
	    #define DefCool_RedColor			100//113
	    #define DefCool_GreenColor   		111//126
	    #define DefCool_BlueColor   		135//134
		
	    #define DefNormal_RedColor   		108//135
	    #define DefNormal_GreenColor		117//126
	    #define DefNormal_BlueColor  		135//128
		
	    #define DefWarm_RedColor			125//139
	    #define DefWarm_GreenColor		128//125
	    #define DefWarm_BlueColor			136//115
#elif PanelType==PanelLGLP156WHB_DP
	    #define DefCool_RedColor			122
	    #define DefCool_GreenColor   		122
	    #define DefCool_BlueColor   		127
		
	    #define DefNormal_RedColor   		125
	    #define DefNormal_GreenColor		126
	    #define DefNormal_BlueColor  		116
		
	    #define DefWarm_RedColor			125
	    #define DefWarm_GreenColor		126
	    #define DefWarm_BlueColor			103
#elif PanelType==PanelB156XTN07
	    #define DefCool_RedColor			95//114
	    #define DefCool_GreenColor   		81//117
	    #define DefCool_BlueColor   		100//127
		
	    #define DefNormal_RedColor   		135//128
	    #define DefNormal_GreenColor		126//127
	    #define DefNormal_BlueColor  		128//121

	#if (Panel_N156BGE&&(PanelType!=PanelM185BGE_L23))
		#if Panel_N156BGE_NEW
			//  #define DefWarm_RedColor			112//139//128  
			// #define DefWarm_GreenColor		90//125//128
			// #define DefWarm_BlueColor			101//115//128
			#define DefWarm_RedColor			86//139//128  
			#define DefWarm_GreenColor		80//125//128
			#define DefWarm_BlueColor			81//115//128
		#else
			#define DefWarm_RedColor			108//139//128  
			#define DefWarm_GreenColor		104//125//128
			#define DefWarm_BlueColor			115//115//128
		#endif
	#elif  Panel_B156xtno7
		#define DefWarm_RedColor			104//139//128  
		#define DefWarm_GreenColor		107//125//128
		#define DefWarm_BlueColor			93//115//128
	#endif
#elif PanelType==PanelNT156WHM_N12
	    #define DefCool_RedColor			122
	    #define DefCool_GreenColor   		122
	    #define DefCool_BlueColor   		127
		
	    #define DefNormal_RedColor   		125
	    #define DefNormal_GreenColor		126
	    #define DefNormal_BlueColor  		116
		
	    #define DefWarm_RedColor			99//125
	    #define DefWarm_GreenColor		91//126
	    #define DefWarm_BlueColor			110//103
#elif PanelType==PanelM185BGE_L23
	    #define DefCool_RedColor			122
	    #define DefCool_GreenColor   		122
	    #define DefCool_BlueColor   		127
		
	    #define DefNormal_RedColor   		125
	    #define DefNormal_GreenColor		126
	    #define DefNormal_BlueColor  		116
		
	    #define DefWarm_RedColor			98//99//125
	    #define DefWarm_GreenColor		100//91//126
	    #define DefWarm_BlueColor			100//110//103
#elif PanelType==PanelM185WHB_N10
	    #define DefCool_RedColor			83//122//122
	    #define DefCool_GreenColor   		86//81//122
	    #define DefCool_BlueColor   		93//127//127
		
	    #define DefNormal_RedColor   		92//94//125
	    #define DefNormal_GreenColor		87//75//126
	    #define DefNormal_BlueColor  		72//73//116
		
	    #define DefWarm_RedColor			94//98//99//125
	    #define DefWarm_GreenColor		94//77//100//91//126
	    #define DefWarm_BlueColor			89//80//100//110//103
#elif PanelType==PanelLM185TT3A

#if LM185TT3A_240MA
	    #define DefCool_RedColor			79//83//122//122
	    #define DefCool_GreenColor   		80//86//81//122
	    #define DefCool_BlueColor   		96//93//127//127

	    //5400K
	    #define DefNormal_RedColor   		96//92//94//125
	    #define DefNormal_GreenColor		90//87//75//126
	    #define DefNormal_BlueColor  		88//72//73//116
		
	    #define DefWarm_RedColor			96//94//98//99//125
	    #define DefWarm_GreenColor		90//94//77//100//91//126
	    #define DefWarm_BlueColor			95//89//80//100//110//103
#else

	    #define DefCool_RedColor			70//79//83//122//122
	    #define DefCool_GreenColor   		72//80//86//81//122
	    #define DefCool_BlueColor   		110//96//93//127//127

	    //5400K
	    #define DefNormal_RedColor   		100//96//92//94//125
	    #define DefNormal_GreenColor		93//90//87//75//126
	    #define DefNormal_BlueColor  		94//88//72//73//116
		
	    #define DefWarm_RedColor			93//96//94//98//99//125
	    #define DefWarm_GreenColor		89//90//94//77//100//91//126
	    #define DefWarm_BlueColor			105//95//89//80//100//110//103


#endif

#else//PanelHSM195EU_RTN01
	    #define DefCool_RedColor			113//114
	    #define DefCool_GreenColor   		126//117
	    #define DefCool_BlueColor   		134//127
		
	    #define DefNormal_RedColor   		135//128
	    #define DefNormal_GreenColor		126//127
	    #define DefNormal_BlueColor  		128//121

	    #if Panel_N156BGE
	    #define DefWarm_RedColor			108//139//128  
	    #define DefWarm_GreenColor		104//125//128
	    #define DefWarm_BlueColor			115//115//128
	    #elif  Panel_B156xtno7
	    #define DefWarm_RedColor			104//139//128  
	    #define DefWarm_GreenColor		107//125//128
	    #define DefWarm_BlueColor			93//115//128
	    #else
	    #define DefWarm_RedColor			104//139//128  
	    #define DefWarm_GreenColor		107//125//128
	    #define DefWarm_BlueColor			93//115//128
	    #endif
	    
#endif
    //=======================================================================================//
    // for srgb
    //=======================================================================================//
#define DefsRGB_RedColor  		DefWarm_RedColor
#define DefsRGB_GreenColor  	DefWarm_GreenColor
#define DefsRGB_BlueColor 		DefWarm_BlueColor
	
#define  EDID_215W                  (PanelWidth==1920&&PanelHeight==1080)
#define  EDID_195W                  (PanelWidth==1600&&PanelHeight==900)
#define  EDID_185W                  ((PanelWidth==1366&&PanelHeight==768)||PanelType==PanelLGLP156WHB_DP)

#if PanelType==PanelLGLP156WHB_DP
    #define MaxContrastValue  (170+10)//178//156 // 152 // 160//0x80 wmz 20051017
    #define MinContrastValue   (70+10)//78//56//36 // 32 //0x80    wmz 20051017
#elif PanelType==PanelNT156WHM_N12
    #define MaxContrastValue  200//185//170//178//156 // 152 // 160//0x80 wmz 20051017
    #define MinContrastValue   100//85//70//78//56//36 // 32 //0x80    wmz 20051017
#elif PanelType==PanelM185BGE_L23
    #define MaxContrastValue  200//185//170//178//156 // 152 // 160//0x80 wmz 20051017
    #define MinContrastValue   100//85//70//78//56//36 // 32 //0x80    wmz 20051017
#else
    #define MaxContrastValue  180+50//185//170//178//156 // 152 // 160//0x80 wmz 20051017
    #define MinContrastValue   80+50//85//70//78//56//36 // 32 //0x80    wmz 20051017
#endif
    #define MaxColorValue	(0xFF-28)//(0x81+49)
    #define MinColorValue	(0x00+28)//(0x81-49)//0x00
    #define MaxBrightnessValue 100 // 160//0x80 wmz 20051017
    #define MinBrightnessValue  0 //0x80    wmz 20051017
    #define ClockAdjRange	100
    #define DefRGBgainBase	20
    #define DefContrastBase   26//48
    #define MAX_VIDEO_SATURATION    0xFF
    #define MIN_VIDEO_SATURATION    0
    #define MAX_VIDEO_HUE           100
    #define MIN_VIDEO_HUE           0
    #define MaxVolume         100//74 // 79
    #define MinVolume          00//24 // 26 // 27
	#define MaxHStart  	((UserPrefAutoHStart>50)?(UserPrefAutoHStart+50):(2*UserPrefAutoHStart-1))  	// 110527 Modify
	#define MinHStart	((UserPrefAutoHStart>50)?(UserPrefAutoHStart-50):1)	
	// 110527 Modify
    #define MaxVStart 	(2*UserPrefAutoVStart)
    #define MinVStart 	0// 2006/10/26 4:18PM by Emily  0
    
    #define MaxClock    (StandardModeHTotal+50)
    #define MinClock    (StandardModeHTotal-50)
	#ifdef OffPower           //090331 xiandi.yu Auto Off Power
	#define MaxAutoPowerOffTime		24
    #define MinAutoPowerOffTime		0
	#endif
    
    #if Enable_Lightsensor
    #define LIGHT_SENSOR_ADC_MAX			428// 1200 090417	//fido 20070710* <==	1400	//fido 20070521* <==	2000	//fido 20070317+ <==	4095
    #define LIGHT_SENSOR_ADC_MIN			28//0 090417		//fido 20070317+
    #define LIGHT_SENSOR_Threshold       3
    	#if LightSensor_Source == EverLight
	#define LIGHT_SENSOR_ERROR_RANGE		(MaxLux*2/10)	// 090610
	#else
	#define LIGHT_SENSOR_ERROR_RANGE		150 //fido 20070525* <==	100		//fido 20070317+
	#endif
    #define LightSensorOn   1
    #define LightSensorOff  0
    #endif

    #ifdef EnableLightBar
    #define DefLightBarValue    50  //80
    #endif
    //============================================================================================
    #define MAIN_FACTORY_FONT_START		0x1D
#if Enable_OsdEngine1
    #define PROP_FONT_START					0x30
    #define MAIN_MENU_FONT_START		  	0x03//0x1E//OSD E1
    #define GuageFontStartAddr	                     0x1C
    #define Arrow_Left				                     0x2D
    #define Arrow_Right			                     0x2E// 2 color end
    #define NUM_START						0x23
#else
    #define PROP_FONT_START					0x63
    #define MAIN_MENU_FONT_START		  	0x21
    #define GuageFontStartAddr	                     0x50
    #define Arrow_Left				                     0x61
    #define Arrow_Right			                     0x62
    #define NUM_START						0x57
#endif

    #define FACTORY_NUM_START				0x03



    #define UpLeft_Corner		0x2
    #define UpRight_Corner		UpLeft_Corner+1
    #define UPMiddle			UpLeft_Corner+2
    #define DownLeft_Corner	UpLeft_Corner+3
    #define DownRight_Corner	UpLeft_Corner+4
    #define DownMiddle		UpLeft_Corner+5


    #define E1_FRAME_01		0x08
    #define E1_FRAME_02		E1_FRAME_01+1
    #define E1_FRAME_03		E1_FRAME_02+1
    #define E1_FRAME_04		E1_FRAME_03+1
    #define E1_FRAME_05		E1_FRAME_04+1
    #define E1_FRAME_06		E1_FRAME_05+1
    #define E1_FRAME_07		E1_FRAME_01//E1_FRAME_06+1//E1_FRAME_06+1//E1_FRAME_01//
    #define E1_FRAME_08		E1_FRAME_06+1//E1_FRAME_07+1//
    #define E1_FRAME_09		E1_FRAME_08+1
    #define E1_FRAME_10		E1_FRAME_09+1
    #define E1_FRAME_11		E1_FRAME_10+1
    #define E1_FRAME_12		E1_FRAME_11+1
    #define E1_FRAME_13		E1_FRAME_12+1
    #define E1_FRAME_14		E1_FRAME_13+1
    #define E1_FRAME_15		E1_FRAME_14+1





    #define iconx1_xpos   4// //4
    #define iconx2_xpos   12// 13
    #define iconx3_xpos    20 //22

    #define icon1y_ypos    4//5
    #define icon2y_ypos    9//10
    #define icon3y_ypos    14//15


    //============================================================================================
    // for FactoryMenu str
    //============================================================================================
      #define _SPACE            0x01
      #define _DASH             0x02//

      #define	_0			0x03
      #define	_1			_0+1
      #define	_2			_0+2
      #define	_3			_0+3
      #define	_4			_0+4
      #define	_5			_0+5
      #define	_6			_0+6
      #define	_7			_0+7
      #define	_8			_0+8
      #define	_9			_0+9

      #define	_A			0x0D
      #define	_B			_A+1
      #define	_C			_B+1
      #define	_D			_C+1
      #define	_E			_D+1
      #define	_F			_E+1
      #define	_G			_F+1
      #define	_H			_G+1
      #define	_I			_H+1
      #define	_J			_I+1
      #define	_K			_J+1
      #define	_L			_K+1
      #define	_M			_L+1
      #define	_N			_M+1
      #define	_O			_N+1
      #define	_P			_O+1
      #define	_Q			_P+1
      #define	_R			_Q+1
      #define	_S			_R+1
      #define	_T			_S+1
      #define	_U			_T+1
      #define	_V			_U+1
      #define	_W			_V+1
      #define	_X			_W+1
      #define	_Y			_X+1
      #define	_Z			_Y+1

      #define	_a			_Z+1
      #define	_b			_a+1
      #define	_c			_b+1
      #define	_d			_c+1
      #define	_e			_d+1
      #define	_f			_e+1
      #define	_g			_f+1
      #define	_h			_g+1
      #define	_i			_h+1
      #define	_j			_i+1
      #define	_k			_j+1
      #define	_l			_k+1
      #define	_m			_l+1
      #define	_n			_m+1
      #define	_o			_n+1
      #define	_p			_o+1
      #define	_q			_p+1
      #define	_r			_q+1
      #define	_s			_r+1
      #define	_t			_s+1
      #define	_u			_t+1
      #define	_v			_u+1
      #define	_w			_v+1
      #define	_x			_w+1
      #define	_y			_x+1
      #define	_z			_y+1

      #define _DOT
      #define   _Plus 	  	_z+2	// 111114 Modify
    //============================================================================================
    // for MainMenu str
    //============================================================================================

	#define      English_Luminace                      10,0x8A,0xFF//10, 10,0xFF  //strlen, str , strend
	#define      English_Imagesetup                  11,0x8B,0xFF//11, 3,0x82,4,0x82,0xFF
	#define      English_Colortemp                     10,0x8B,0xFF//10, 9,0x81,0xFF    //071225
	#define      English_Colorboost                    11,0x8B,0xFF//11, 11,0xFF
	#define      English_Pictureboost                 12,0x8C,0xFF//  12, 12,0xFF
	#define      English_Osdsetup                       10,0x8A,0xFF//10, 8,0x81,1,0xFF
	#define      English_Extra                             5, 0x85,0xFF//5,  5,0xFF
	#define      English_Reset                             5,0x85,0xFF//5,  5,0xFF
	#define      English_Exit                                4,0x84,0xFF//4,  4,0xFF
#if CenterTitleText
	#define      Spanish_Luminace                    12,0x8C,0xFF
	#define      Spanish_Imagesetup                14,0x8E,0xFF
	#define      Spanish_Colortemp                   16,0x80+16,0xFF
	#define      Spanish_Colorboost                  14,0x80+14,0xFF
	#define      Spanish_Pictureboost               15,0x80+15,0xFF
	#define      Spanish_Osdsetup                    11,0x8B,0xFF
	#define      Spanish_Extra                          5, 0x85,0xFF
	#define      Spanish_Reset                          8,0x88,0xFF
	#define      Spanish_Exit                             4,0x84,0xFF
	#define      France_Luminace                     10,0x8A,0xFF
	#define      France_Imagesetup                 17,0x80+17,0xFF
	#define      France_Colortemp                    7,0x87,0xFF
	#define      France_Colorboost                   13,0x8D,0xFF
	#define      France_Pictureboost                11,0x8B,0xFF
	#define      France_Osdsetup                     12,0x8C,0xFF
	#define      France_Extra                            5, 0x85,0xFF
	#define      France_Reset                           11,0x8B,0xFF
	#define      France_Exit                              6,0x86,0xFF
	#define      Portugues_Luminace                 17,0x80+17,0xFF
	#define      Portugues_Imagesetup              15,0x80+15,0xFF
	#define      Portugues_Colortemp                14,0x80+14,0xFF
	#define      Portugues_Colorboost                18,0x80+18,0xFF
	#define      Portugues_Pictureboost             20,0x80+20,0xFF
	#define      Portugues_Osdsetup                  18,0x80+18,0xFF
	#define      Portugues_Extra                     15, 0x80+15,0xFF
	#define      Portugues_Reset                     10,0x80+10,0xFF
	#define      Portugues_Exit                       4,0x84,0xFF
#ifdef DEBUG_EN                                      
	#define      Russia_Luminace                       10,0x8A,0xFF
	#define      Russia_Imagesetup                  11,0x8B,0xFF
	#define      Russia_Colortemp                      10,0x8A,0xFF
	#define      Russia_Colorboost                      11,0x8B,0xFF
	#define      Russia_Pictureboost               12,0x8C,0xFF
	#define      Russia_Osdsetup                       10,0x8A,0xFF
	#define      Russia_Extra                            5,0x85,0xFF
	#define      Russia_Reset                            5,0x85,0xFF
	#define      Russia_Exit                             4,0x84,0xFF
#else		//110617 Modify                                              
	#define      Russia_Luminace                      9,0x80+ 9,0xFF
	#define      Russia_Imagesetup                   17,0x80+17,0xFF
	#define      Russia_Colortemp                    11,0x80+11,0xFF// 9,0x80+9,0xFF
	#define      Russia_Colorboost                   11,0x80+11,0xFF
	#define      Russia_Pictureboost                 17,0x80+17,0xFF
	#define      Russia_Osdsetup                     13,0x80+13,0xFF
	#define      Russia_Extra                        14,0x80+14,0xFF
	#define      Russia_Reset                        16,0x80+16,0xFF
	#define      Russia_Exit                         7,0x87,0xFF
#endif                                               
	#define      SChina_Luminace                       5,  0x85,0xFF// 3,  0x83,0xFF
	#define      SChina_Imagesetup                  6,  0x86,0xFF
	#define      SChina_Colortemp                       3,  0x83,0xFF
	#define      SChina_Colorboost                      6,  0x86,0xFF
	#define      SChina_Pictureboost                6,  0x86,0xFF
	#define      SChina_Osdsetup                        6,  0x86,0xFF
	#define      SChina_Extra                           3,  0x83,0xFF
	#define      SChina_Reset                           5,  0x85,0xFF
	#define      SChina_Exit                            3,  0x83,0xFF
	#define      TChina_Luminace                        3,  0x83,0xFF // 4,  0x84,0xFF 
	#define      TChina_Imagesetup                  6,  0x86,0xFF
	#define      TChina_Colortemp                       3,  0x83,0xFF
	#define      TChina_Colorboost                      6,  0x86,0xFF
	#define      TChina_Pictureboost                6,  0x86,0xFF
	#define      TChina_Osdsetup                        6,  0x86,0xFF
	#define      TChina_Extra                           3,  0x83,0xFF
	#define      TChina_Reset                           5,  0x85,0xFF
	#define      TChina_Exit                            3,  0x83,0xFF
	#define      Korea_Luminace                         3,  0x83,0xFF
	#define      Korea_Imagesetup                   8,  0x88,0xFF
	#define      Korea_Colortemp                       5,  0x85,0xFF// 6,  0x86,0xFF
	#define      Korea_Colorboost                       6,  0x86,0xFF// 5,  0x85,0xFF
	#define      Korea_Pictureboost                 6,  0x86,0xFF
	#define      Korea_Osdsetup                         6,  0x86,0xFF
	#if 0///yuanzhi20100810_3
	#define      Korea_Extra                            3,  0x83,0xFF
	#else
	#define      Korea_Extra                            6,  0x86,0xFF
	#endif
	#define      Korea_Reset                            5,  0x85,0xFF// 6,  0x86,0xFF
	#define      Korea_Exit                             5,  0x85,0xFF
	#define      Japan_Luminace                         4,  0x84,0xFF
	#define      Japan_Imagesetup                   	7,  0x87,0xFF
	#define      Japan_Colortemp                        5,  0x85,0xFF//   6,  6,0xFF
	#define      Japan_Colorboost                       8,  0x88,0xFF//   5,  5,0xFF		//110617 Modify
	#define      Japan_Pictureboost                 	10, 0x8A,0xFF
	#define      Japan_Osdsetup                         7,  0x87,0xFF
	#define      Japan_Extra                            5,  0x85,0xFF
	#define      Japan_Reset                            5,  0x85,0xFF//   6,  6,0xFF
	#define      Japan_Exit                             3,  0x83,0xFF
	#define      German_Luminace                     10,0x8A,0xFF
	#define      German_Imagesetup                   14,0x80+14,0xFF
	#define      German_Colortemp                    9,0x89,0xFF
	#define      German_Colorboost                   12,0x8C,0xFF
	#define      German_Pictureboost                 11,0x8B,0xFF
	#define      German_Osdsetup                     16,0x90,0xFF
	#define      German_Extra                           5, 0x85,0xFF
	#define      German_Reset                        13,0x80+13,0xFF
	#define      German_Exit                         9, 0x89,0xFF
	#define      Italian_Luminace                    10,0x8A,0xFF
	#define      Italian_Imagesetup                 18,0x80+18,0xFF//  13,0x8D,0xFF
	#define      Italian_Colortemp                   6,0x86,0xFF
	#define      Italian_Colorboost                 15,0x80+15,0xFF
	#define      Italian_Pictureboost              18,0x80+18,0xFF// 16,0x90,0xFF
	#define      Italian_Osdsetup                     13,0x80+13,0xFF
	#define      Italian_Extra                       5, 0x85,0xFF
	#define      Italian_Reset                       9,0x89,0xFF
	#define      Italian_Exit                         4,0x84,0xFF
	#define      Swedish_Luminace                   9,0x89,0xFF
	#define      Swedish_Imagesetup                15,0x80+14,0xFF//    7,0x87,0xFF
	#define      Swedish_Colortemp                   5,0x85,0xFF
	#define      Swedish_Colorboost                  13,0x8D,0xFF
	#define      Swedish_Pictureboost                13,0x8D,0xFF
	#define      Swedish_Osdsetup                    14,0x8E,0xFF
	#define      Swedish_Extra                      5, 0x85,0xFF
	#define      Swedish_Reset                       8,0x88,0xFF
	#define      Swedish_Exit                        7,0x87,0xFF
	#define      Polish_Luminace                     11,0x8B,0xFF
	#define      Polish_Imagesetup                   17,0x80+17,0xFF
	#define      Polish_Colortemp                    18,0x80+18,0xFF
	#define      Polish_Colorboost                   18,0x80+18,0xFF
	#define      Polish_Pictureboost                 17,0x80+17,0xFF
	#define      Polish_Osdsetup                     11,0x8B,0xFF
	#define      Polish_Extra                        6,0x86,0xFF
	#define      Polish_Reset                         5,0x85,0xFF
	#define      Polish_Exit                         8,0x88,0xFF
	#define      Czech_Luminace                      10,0x8A,0xFF
	#define      Czech_Imagesetup                   15,0x80+15,0xFF
	#define      Czech_Colortemp                     11,0x8B,0xFF
	#define      Czech_Colorboost                    12,0x8C,0xFF
	#define      Czech_Pictureboost                 13,0x8D,0xFF
	#define      Czech_Osdsetup                      13,0x8D,0xFF
	#define      Czech_Extra                        5,0x85,0xFF
	#define      Czech_Reset                          9,0x89,0xFF
	#define      Czech_Exit                         6,0x86,0xFF

#else
	#define      Spanish_Luminace                   12, 12,0xFF
	#define      Spanish_Imagesetup                 14, 4,0x82,3,0x82,3,0xFF
	#define      Spanish_Colortemp                  16, 4,0x81,11,0xFF
	#define      Spanish_Colorboost                 14, 14,0xFF
	#define      Spanish_Pictureboost               15, 11,0x82,2,0xFF
	#define      Spanish_Osdsetup                       11, 4,0x82,5,0xFF
	#define      Spanish_Extra                          5, 5,0xFF,//5,  5,0xFF
	#define      Spanish_Reset                          8,  8,0xFF
	#define      Spanish_Exit                           4,  4,0xFF
	#define      France_Luminace                        10, 10,0xFF
	#define      France_Imagesetup                  14, 5,0x82,4,0x82,1,0xFF
	#define      France_Colortemp                   7,  7,0xFF
	#define      France_Colorboost                      13, 13,0xFF
	#define      France_Pictureboost                11, 8,0x82,1,0xFF
	#define      France_Osdsetup                        12, 2,0x84,6,0xFF
	#define      France_Extra                           5, 5,0xFF,//5,  5,0xFF
	#define      France_Reset                           11, 11,0xFF
	#define      France_Exit                            6,  6,0xFF
	#define      Portugues_Luminace                 15,1,0x82,9,0x82,2,0xFF//16,1,0x82,9,0x82,0xFF
	#define      Portugues_Imagesetup               12, 5,0x81,4,0x82,2,0xFF
	#define      Portugues_Colortemp                12, 2,0x81,10,0xFF
	#define      Portugues_Colorboost                17, 17,0xFF
	#define      Portugues_Pictureboost             18,18,0xFF
	#define      Portugues_Osdsetup                 17, 5,0x81,11,0xFF
	#define      Portugues_Extra                        14, 15,0xFF,//5,  5,0xFF
	#define      Portugues_Reset                        9, 9,0xFF
	#define      Portugues_Exit                         4,  4,0xFF
#ifdef DEBUG_EN
	#define      Russia_Luminace                       10,0x8A,0xFF
	#define      Russia_Imagesetup                  11,0x8B,0xFF
	#define      Russia_Colortemp                      10,0x8A,0xFF
	#define      Russia_Colorboost                      11,0x8B,0xFF
	#define      Russia_Pictureboost               12,0x8C,0xFF
	#define      Russia_Osdsetup                       10,0x8A,0xFF
	#define      Russia_Extra                            5,0x85,0xFF
	#define      Russia_Reset                            5,0x85,0xFF
	#define      Russia_Exit                             4,0x84,0xFF
#else
	#define      Russia_Luminace                        13, 13,0xFF
	#define      Russia_Imagesetup                  17,5,0x81,6,0x81,4,0xff
	#define      Russia_Colortemp                   10,1,0x81,8,0xff
	#define      Russia_Colorboost                      17, 17,0xFF
	#define      Russia_Pictureboost                17,3,0x81,9,0x81,3,0xff
	#define      Russia_Osdsetup                       13,5,0x81,7,0xff
	#define      Russia_Extra                       8,0,0x82,6,0xff
	#define      Russia_Reset                        7,3,0x81,3,0xff
	#define      Russia_Exit                         7,5,0x82,0xff
#endif
	#define      SChina_Luminace                        3,  5,0xFF//3,  3,0xFF
	#define      SChina_Imagesetup                  6,  6,0xFF
	#define      SChina_Colortemp                       3,  3,0xFF
	#define      SChina_Colorboost                      6,  6,0xFF
	#define      SChina_Pictureboost                6,  6,0xFF
	#define      SChina_Osdsetup                        6,  6,0xFF
	#define      SChina_Extra                           3,  3,0xFF
	#define      SChina_Reset                           3,  3,0xFF
	#define      SChina_Exit                            3,  3,0xFF
	#define      TChina_Luminace                      3,  3,0xFF//  4,  4,0xFF 
	#define      TChina_Imagesetup                  6,  6,0xFF
	#define      TChina_Colortemp                       3,  3,0xFF
	#define      TChina_Colorboost                      6,  6,0xFF
	#define      TChina_Pictureboost                6,  6,0xFF
	#define      TChina_Osdsetup                        6,  6,0xFF
	#define      TChina_Extra                           3,  3,0xFF
	#define      TChina_Reset                           5,  5,0xFF
	#define      TChina_Exit                            3,  3,0xFF
	#define      Korea_Luminace                         3,  3,0xFF
	#define      Korea_Imagesetup                   8,  8,0xFF
	#define      Korea_Colortemp                       5,  5,0xFF//   6,  6,0xFF
	#define      Korea_Colorboost                      6,  6,0xFF//   5,  5,0xFF
	#define      Korea_Pictureboost                 6,  6,0xFF
	#define      Korea_Osdsetup                         7,  7,0xFF
	#define      Korea_Extra                            3,  3,0xFF
	#define      Korea_Reset                          5,  5,0xFF//   6,  6,0xFF
	#define      Korea_Exit                             5,  5,0xFF
	#define      Japan_Luminace                         4,  0x84,0xFF
	#define      Japan_Imagesetup                   	11,  0x8B,0xFF
	#define      Japan_Colortemp                        5,  0x85,0xFF//   6,  6,0xFF
	#define      Japan_Colorboost                       9,  0x89,0xFF//   5,  5,0xFF
	#define      Japan_Pictureboost                 	9,  0x89,0xFF
	#define      Japan_Osdsetup                         11,  0x8B,0xFF
	#define      Japan_Extra                            3,  0x83,0xFF
	#define      Japan_Reset                            5,  0x85,0xFF//   6,  6,0xFF
	#define      Japan_Exit                             3,  0x83,0xFF
	#define      German_Luminace                         10,  10,0xFF
	#define      German_Imagesetup                   14,  12,0x82,0xFF
	#define      German_Colortemp                        9, 7,0x81,1,0xFF
	#define      German_Colorboost                       12,  12,0xFF
	#define      German_Pictureboost                 11,  11,0xFF
	#define      German_Osdsetup                         16,  15,0x81,0xFF// 17,  13,0x82,2,0xFF
	#define      German_Extra                            5, 5,0xFF,//5,  5,0xFF
	#define      German_Reset                            13,  13,0xFF
	#define      German_Exit                             9,  9,0xFF
	#define      Italian_Luminace                         10,  10,0xFF
	#define      Italian_Imagesetup                   18, 2,0x82,9,0x82,3 ,0xFF// 13, 2,0x81,6,0x82,2 ,0xFF
	#define      Italian_Colortemp                        6,  6,0xFF
	#define      Italian_Colorboost                      15,2,0x82,11,0xff
	#define      Italian_Pictureboost                  18,2,0x82,9,0x82,3,0xff//  16, 1, 0x82, 8, 0x82, 3, 0xFF
	#define      Italian_Osdsetup                     13,2,0x82,9,0xff
	#define      Italian_Extra                            5,  5,0xFF
	#define      Italian_Reset                            9,1, 0x82,  6,0xFF
	#define      Italian_Exit                             4,  4,0xFF
	#define      Swedish_Luminace                         9,  9,0xFF
	#define      Swedish_Imagesetup                  14,  12,0x82,0xFF//  7,  7,0xFF
	#define      Swedish_Colortemp                     5,3,0x82,0xFF //   9, 2, 0x82,3, 0x81,1, 0xFF
	#define      Swedish_Colorboost                       13, 2, 0x82,8,0x81,0xFF
	#define      Swedish_Pictureboost                 13,11,0x82,0xFF
	#define      Swedish_Osdsetup                         14,14,0xFF
	#define      Swedish_Extra                            5,  5,0xFF
	#define      Swedish_Reset                            8,0,0x82, 6,0xFF
	#define      Swedish_Exit                             7,  7,0xFF
	#define      Polish_Luminace                         11 , 9 , 0x81,1,0xFF
	#define      Polish_Imagesetup                   17,  17,0xFF
	#define      Polish_Colortemp                       18,4,0x81,13,0xff
	#define      Polish_Colorboost                       18,18,0xFF
	#define      Polish_Pictureboost                 17,17,0xFF
	#define      Polish_Osdsetup                         11,11,0xFF
	#define      Polish_Extra                            6,6,0xFF
	#define      Polish_Reset                            5,5,0xFF
	#define      Polish_Exit                             8,8,0xFF
	#define      Czech_Luminace                         10,10,0xFF
	#define      Czech_Imagesetup                   15,15,0xFF
	#define      Czech_Colortemp                       11,3,0x81,7,0xff
	#define      Czech_Colorboost                       12,12,0xFF
	#define      Czech_Pictureboost                 13,13,0xFF
	#define      Czech_Osdsetup                         13,13,0xFF
	#define      Czech_Extra                            5,5,0xFF
	#define      Czech_Reset                            9,9,0xFF
	#define      Czech_Exit                                 6,6,0xFF
#endif
    //============================================================================================

    // current input timing information
    #define SrcHPeriod		InputTiming.HPeiod
    #define SrcVTotal		InputTiming.VTotal
    #define SrcModeIndex		InputTiming.ModeIndex
    #define SrcFlags		InputTiming.fFlags
    #define SrcInputType		InputTiming.InputType
	#define SrcVsyncwidth   InputTiming.Vsyncwidth 
    //============================================================================================
    // VESA Standard mode default settings
    #define StandardModeWidth	       GetStandardModeWidth(StandardModeGroup)//StandardModeResolution[StandardModeGroup].DispWidth
    #define StandardModeHeight	GetStandardModeHeight(StandardModeGroup)//StandardModeResolution[StandardModeGroup].DispHeight
    #define StandardModeGroup	GetStandardModeGroup(SrcModeIndex)//StandardMode[SrcModeIndex].ResIndex
    #define StandardModeHStart	GetStandardModeHStart(SrcModeIndex)//StandardMode[SrcModeIndex].HStart
    #define StandardModeVStart	GetStandardModeVStart(SrcModeIndex)//StandardMode[SrcModeIndex].VStart
    #define StandardModeHTotal	GetStandardModeHTotal(SrcModeIndex)//StandardMode[SrcModeIndex].HTotal
    #define StandardModeHFreq	StandardMode[SrcModeIndex].HFreq
    #define StandardModeVFreq		GetStandardModeVFreq(SrcModeIndex)//StandardMode[SrcModeIndex].VFreq
    //============================================================================================

#if  BlacklitTimeOnShow
extern BYTE 	xdata BlacklitTimeOn_S;
extern BYTE 	xdata BlacklitTimeOn_M;
extern WORD 	xdata BlacklitTimeOn_H;
//extern BYTE 	xdata BlacklitTimeOn_HH;


#define MonitorBlacklitTimeOn_S				MonitorSetting2.BlacklitTimeOn_S
#define MonitorBlacklitTimeOn_M                       MonitorSetting2.BlacklitTimeOn_M
#define MonitorBlacklitTimeOn_H				MonitorSetting2.BlacklitTimeOn_H
//#define MonitorBlacklitTimeOn_HH				MonitorSetting2.BlacklitTimeOn_HH

//extern BYTE 	xdata BlacklitTimeOn_SShift;
#endif


#if PANELSSCSTEPLFUN

#define PanelSSCSTEPL		(FactorySetting.SSCSTEPL)
#define PanelSSCSTEPH		(FactorySetting.SSCSTEPH)
#define PanelSSCSPANL		(FactorySetting.SSCSPANL)
#define PanelSSCSPANH		(FactorySetting.SSCSPANH)

#endif


#if EnablePanelServiceMenu

    #define bPanelColorDepth					BIT0//0->6bit+FRC						1->8Bit
    #define bChangeModePanelVddOff			BIT1//0->Change Mode Panel Vdd On		1->Change Mode Panel Vdd Off

    //#define PanelDither   			(PanelSetting.DitherTpye)
    #define PanelSettingFlag   		(PanelSetting.SettingFlag)

    #define PanelOnTiming1		(PanelSetting.OnTiming1)
    #define PanelOnTiming2		(PanelSetting.OnTiming2)
    #define PanelOffTiming1		(PanelSetting.OffTiming1)
    #define PanelOffTiming2		(PanelSetting.OffTiming2)


	
    #define PanelHSyncWidth		(PanelSetting.HsyncWidth)
    #define PanelHSyncBackPorch	(PanelSetting.HSyncBackPorch)
    #define PanelVSyncWidth		(PanelSetting.VSyncWidth)
    #define PanelVSyncBackPorch	(PanelSetting.VsyncBackPorch)
    #define PanelWidth			(PanelSetting.Width)
    #define PanelHeight		(PanelSetting.Height)
    #define PanelHTotal		(PanelSetting.HTotal)
    #define PanelVTotal		(PanelSetting.VTotal)
    #define PanelMaxHTotal	(PanelSetting.MaxHTotal)
    #define PanelMinHTotal		(PanelSetting.MinHTotal)
    //#define PanelMaxVTotal		(PanelSetting.MaxVTotal)
    //#define PanelMinVTotal		(PanelSetting.MinVTotal)

	
    #define PanelDCLK			(PanelSetting.TypicalDCLK)
    #define PanelMaxDCLK		(PanelSetting.MaxDCLK)
    #define PanelMinDCLK		(PanelSetting.MinDCLK)
    #define PanelLVDSSwing	(PanelSetting.Swing)

    #define PanelOutputControl1	(PanelSetting.OutputControl1)
    #define PanelOutputControl2	(PanelSetting.OutputControl2)
    //#define PanelOSContol		(PanelSetting.OSContol)
    //#define PanelODRriving		(PanelSetting.ODRriving)

    #define PanelBurstModeFreq		(PanelSetting.BurstModeFreq)
    #define PanelDefDuty		(PanelSetting.DefDuty)
    #define PanelMinDuty		(PanelSetting.MinDuty)
    #define PanelMaxDuty		(PanelSetting.MaxDuty)

   // #define PanelSSCSTEPL		(PanelSetting.SSCSTEPL)
   // #define PanelSSCSTEPH		(PanelSetting.SSCSTEPH)
   // #define PanelSSCSPANL		(PanelSetting.SSCSPANL)
   // #define PanelSSCSPANH		(PanelSetting.SSCSPANH)

    #define PanelPanelTypeNum	(PanelSetting.PanelTypeNum)


    #define PanelHStart		(PanelHSyncWidth+PanelHSyncBackPorch)
    #define PanelVStart		(PanelVSyncWidth+PanelVSyncBackPorch)

    #define MaxPanelOnTiming1		100
    #define MaxPanelOnTiming2		1000
    #define MaxPanelOffTiming1	1000
    #define MaxPanelOffTiming2	100	

    #define MaxPanelHSyncWidth		255
    #define MaxPanelHSyncBackPorch	255
    #define MaxPanelVSyncWidth		255
    #define MaxPanelVSyncBackPorch	255

    #define MinBurstModeFreqValue			219//100
    #define MaxBurstModeFreqValue		600
	
    #define MinDutyValue			0
    #define MaxDutyValue			255

    #define MinSwingValue			0
    #define MaxSwingValue			0x1F
    #define MinSwingAutoTuneValue			0
    #define MaxSwingAutoTuneValue		0x3		

#endif


#if USEFLASH
   #if 1
			   // Monitor Setting
        #define MonitorFlags			          MonitorSetting.MonitorFlag
        #define UserPrefColorTemp		    MonitorSetting.ColorTemp
        #define UserPrefBrightness 	 	   MonitorSetting.Brightness
        #define UserPrefContrast 		         MonitorSetting.Contrast
        #define UserPrefBrightnessWarm1	  MonitorSetting.BrightnessWarm1
        #define UserPrefContrastWarm1	  MonitorSetting.ContrastWarm1
        #define UserPrefBrightnessNormal	  MonitorSetting.BrightnessNormal
        #define UserPrefContrastNormal	  MonitorSetting.ContrastNormal
        #define UserPrefBrightnessCool1	  MonitorSetting.BrightnessCool1
        #define UserPrefContrastCool1	        MonitorSetting.ContrastCool1
        #define UserPrefBrightnessSRGB	MonitorSetting.BrightnesssRGB
        #define UserPrefContrastSRGB		MonitorSetting.ContrastsRGB
        #define UserPrefRedColor 		         MonitorSetting.RedColor
        #define UserPrefGreenColor 		  MonitorSetting.GreenColor
        #define UserPrefBlueColor 		        MonitorSetting.BlueColor
        #define UserPrefYColor		        MonitorSetting.YColor
        #define UserPrefCColor		 MonitorSetting.CColor
        #define UserPrefMColor		       MonitorSetting.MColor
        #define UserPrefBrightnessUser	 	   MonitorSetting.BrightnessUser
        #define UserPrefContrastUser		         MonitorSetting.ContrastUser
        #define UserPrefRedColorUser		         MonitorSetting.RedColorUser
        #define UserPrefGreenColorUser		  MonitorSetting.GreenColorUser
        #define UserPrefBlueColorUser		        MonitorSetting.BlueColorUser
        #define UserPrefGamaMode   	    MonitorSetting.GamaMode
        #define UserPrefVolume		   MonitorSetting.Volume
        #define UserPrefECOMode		  MonitorSetting.ECOMode
        #define UserPrefLanguage		  MonitorSetting.Language
        #define UserPrefOsdHStart	  MonitorSetting.OsdHStart
        #define UserPrefOsdVStart		  MonitorSetting.OsdVStart
        #define UserPrefOsdTime		  MonitorSetting.OsdTime
		#define UserPrefOsdTrans         MonitorSetting.OsdTrans
		#ifdef OffPower         //090331 xiandi.yu Auto Off Power
		#define UserPrefAutoPowerOff    MonitorSetting.AutoPowerOff
		#endif
        #define UserPrefInputType		   MonitorSetting.InputType
        #define UserPrefInputSelectType		   MonitorSetting.InputSelectType
        //#define UserPrefLastMenuIndex		   MonitorSetting.LastMenuPage
        #define UserPrefSavedModeIndex	MonitorSetting.SavedModeIndex // current mode index can be used
        #define UserprefRedBlackLevel			MonitorSetting.RedBlackLevel
        #define UserprefGreenBlackLevel				MonitorSetting.GreenBlackLevel
        #define UserprefBlueBlackLevel				MonitorSetting.BlueBlackLevel
        #define UserPrefDcrMode   	    MonitorSetting.DcrMode
		#define UserPrefColorFlags		  	MonitorSetting.ColorFlag
        #if MWEFunction
            #define UserPrefHue					MonitorSetting.Hue
            #define UserPrefSaturation		       MonitorSetting.Saturation
            #define UserPrefSubContrast		       MonitorSetting.SubContrast
            #define UserPrefSubBrightness	              MonitorSetting.SubBrightness

            #define UserPrefBFSize 				MonitorSetting.BF_Size
            #define UserPrefBFHSize 				MonitorSetting.BF_HSize
            #define UserPrefBFVSize 				MonitorSetting.BF_VSize
            #define UserPrefBFHStart 				MonitorSetting.BF_Hstart
            #define UserPrefBFVStart 				MonitorSetting.BF_Vstart
        #endif
        #if Enable_Expansion
            #define UserprefExpansionMode        MonitorSetting.ExpansionMode
        #endif
        #define UserprefHistogram1              MonitorSetting.Histogram1
        #define UserprefHistogram2              MonitorSetting.Histogram2
        #define UserprefALha              MonitorSetting.ALhaA
        #define UserprefBata              MonitorSetting.BataA
        #define MSBankNo			MonitorSetting.BankNo
        #define MSAddr				MonitorSetting.Addr
        #define MSValue				MonitorSetting.Value
        #if USEFLASH
            //#define MonitorBackLightTime				MonitorSetting.BackLightTime
            #define MonitorBackLightTime  MonitorSetting2.Backlighttime
        #endif
    #endif

		#ifdef TSUMXXT    //110401 Modify
		#define UserPrefAdcPgaGain              FactorySetting.AdcPgaGain
		#endif
        #define UserPrefAdcRedGain		FactorySetting.AdcRedGain
        #define UserPrefAdcGreenGain		FactorySetting.AdcGreenGain
        #define UserPrefAdcBlueGain		FactorySetting.AdcBlueGain
        #define UserPrefAdcRedOffset		FactorySetting.AdcRedOffset
        #define UserPrefAdcGreenOffset	FactorySetting.AdcGreenOffset
        #define UserPrefAdcBlueOffset		FactorySetting.AdcBlueOffset
        #define UserPrefRedColorWarm1	  FactorySetting.RedColorWarm1
        #define UserPrefGreenColorWarm1	  FactorySetting.GreenColorWarm1
        #define UserPrefBlueColorWarm1	 FactorySetting.BlueColorWarm1
        #define UserPrefRedColorNormal	  FactorySetting.RedColorNormal
        #define UserPrefGreenColorNormal   FactorySetting.GreenColorNormal
        #define UserPrefBlueColorNormal	  FactorySetting.BlueColorNormal
        #define UserPrefRedColorCool1		 FactorySetting.RedColorCool1
        #define UserPrefGreenColorCool1	 FactorySetting.GreenColorCool1
        #define UserPrefBlueColorCool1	 FactorySetting.BlueColorCool1
        #define UserPrefRedColorSRGB		FactorySetting.RedColorsRGB
        #define UserPrefGreenColorSRGB    FactorySetting.GreenColorsRGB
        #define UserPrefBlueColorSRGB      FactorySetting.BlueColorsRGB
        #define FUserPrefBrightnessWarm1	  FactorySetting.FBrightnessWarm1
        #define FUserPrefContrastWarm1	  FactorySetting.FContrastWarm1
        #define FUserPrefBrightnessNormal	  FactorySetting.FBrightnessNormal
        #define FUserPrefContrastNormal	  FactorySetting.FContrastNormal
        #define FUserPrefBrightnessCool1	  FactorySetting.FBrightnessCool1
        #define FUserPrefContrastCool1	        FactorySetting.FContrastCool1
        #define FUserPrefBrightnessSRGB	        FactorySetting.FBrightnesssRGB
        #define FUserPrefContrastSRGB	        FactorySetting.FContrastsRGB
        #if EnableVcomAdjustItem || PANEL_VCOM_ADJUST		//121031 Modify
        #define UserPrefVcomValue       FactorySetting.VcomValue
        #endif
	 #define LogoON  FactorySetting.LogoON
        #if USEFLASH
            #define FactoryProductModeValue	        FactorySetting.ProductModeValue
        #endif
        #define FactoryCheckSum	        FactorySetting.CheckSum




   #else



	#if 1
        // Monitor Setting
        #define MonitorFlags			          MonitorSetting.MonitorFlag
        #define UserPrefAdcRedGain		MonitorSetting.AdcRedGain
        #define UserPrefAdcGreenGain		MonitorSetting.AdcGreenGain
        #define UserPrefAdcBlueGain		MonitorSetting.AdcBlueGain
        #define UserPrefAdcRedOffset		MonitorSetting.AdcRedOffset
        #define UserPrefAdcGreenOffset	MonitorSetting.AdcGreenOffset
        #define UserPrefAdcBlueOffset		MonitorSetting.AdcBlueOffset
	#define LogoON	                       	MonitorSetting.LogoON//2011.8.3  
        #define UserPrefColorTemp		    MonitorSetting.ColorTemp
        #define UserPrefBrightness 	 	   MonitorSetting.Brightness
        #define UserPrefContrast 		         MonitorSetting.Contrast
        #define UserPrefRedColor 		         MonitorSetting.RedColor
        #define UserPrefGreenColor 		  MonitorSetting.GreenColor
        #define UserPrefBlueColor 		        MonitorSetting.BlueColor
        #define UserPrefYColor		        MonitorSetting.YColor
        #define UserPrefCColor		 MonitorSetting.CColor
        #define UserPrefMColor		       MonitorSetting.MColor
        #define UserPrefBrightnessUser	 	   MonitorSetting.BrightnessUser
        #define UserPrefContrastUser		         MonitorSetting.ContrastUser
        #define UserPrefRedColorUser		         MonitorSetting.RedColorUser
        #define UserPrefGreenColorUser		  MonitorSetting.GreenColorUser
        #define UserPrefBlueColorUser		        MonitorSetting.BlueColorUser
        #define UserPrefBrightnessWarm1	  MonitorSetting.BrightnessWarm1
        #define UserPrefContrastWarm1	  MonitorSetting.ContrastWarm1
        #define UserPrefRedColorWarm1	  MonitorSetting.RedColorWarm1
        #define UserPrefGreenColorWarm1	  MonitorSetting.GreenColorWarm1
        #define UserPrefBlueColorWarm1	 MonitorSetting.BlueColorWarm1
        #define UserPrefBrightnessNormal	  MonitorSetting.BrightnessNormal
        #define UserPrefContrastNormal	  MonitorSetting.ContrastNormal
        #define UserPrefRedColorNormal	  MonitorSetting.RedColorNormal
        #define UserPrefGreenColorNormal   MonitorSetting.GreenColorNormal
        #define UserPrefBlueColorNormal	  MonitorSetting.BlueColorNormal
        #define UserPrefBrightnessCool1	  MonitorSetting.BrightnessCool1
        #define UserPrefContrastCool1	        MonitorSetting.ContrastCool1
        #define UserPrefRedColorCool1		 MonitorSetting.RedColorCool1
        #define UserPrefGreenColorCool1	 MonitorSetting.GreenColorCool1
        #define UserPrefBlueColorCool1	 MonitorSetting.BlueColorCool1
        #define UserPrefBrightnessSRGB	MonitorSetting.BrightnesssRGB
        #define UserPrefContrastSRGB		MonitorSetting.ContrastsRGB
        #define UserPrefRedColorSRGB		MonitorSetting.RedColorsRGB
        #define UserPrefGreenColorSRGB    MonitorSetting.GreenColorsRGB
        #define UserPrefBlueColorSRGB      MonitorSetting.BlueColorsRGB
        #define FUserPrefBrightnessWarm1	  MonitorSetting.FBrightnessWarm1
        #define FUserPrefContrastWarm1	  MonitorSetting.FContrastWarm1
        #define FUserPrefBrightnessNormal	  MonitorSetting.FBrightnessNormal
        #define FUserPrefContrastNormal	  MonitorSetting.FContrastNormal
        #define FUserPrefBrightnessCool1	  MonitorSetting.FBrightnessCool1
        #define FUserPrefContrastCool1	        MonitorSetting.FContrastCool1
        #define UserPrefGamaMode   	    MonitorSetting.GamaMode
        // 2006/10/26 6:49PM by Emily #define UserPrefSharpness	   MonitorSetting.Sharpness
        #define UserPrefVolume		   MonitorSetting.Volume
        #define UserPrefECOMode		  MonitorSetting.ECOMode
        #define UserPrefLanguage		  MonitorSetting.Language
        #define UserPrefOsdHStart	  MonitorSetting.OsdHStart
        #define UserPrefOsdVStart		  MonitorSetting.OsdVStart
        #define UserPrefOsdTrans         MonitorSetting.OsdTrans
        #define UserPrefOsdTime		  MonitorSetting.OsdTime
        #define UserPrefInputType		   MonitorSetting.InputType
        #define UserPrefInputSelectType		   MonitorSetting.InputSelectType
        //#define UserPrefLastMenuIndex		   MonitorSetting.LastMenuPage
        #define UserPrefSavedModeIndex	MonitorSetting.SavedModeIndex // current mode index can be used
        #define UserprefRedBlackLevel			MonitorSetting.RedBlackLevel
        #define UserprefGreenBlackLevel				MonitorSetting.GreenBlackLevel
        #define UserprefBlueBlackLevel				MonitorSetting.BlueBlackLevel
        #define UserPrefDcrMode   	    MonitorSetting.DcrMode
        #if MWEFunction
            //#define UserPrefMWE					MonitorSetting.MWE
            #define UserPrefHue					MonitorSetting.Hue
            #define UserPrefSaturation		       MonitorSetting.Saturation
            #define UserPrefSubContrast		       MonitorSetting.SubContrast
            #define UserPrefSubBrightness	              MonitorSetting.SubBrightness
            // 2006/10/26 6:46PM by Emily #define UserPrefSubSharpness	              MonitorSetting.SubSharpness
            //#define UserPrefSkinTone			        MonitorSetting.SkinTone
            #define UserPrefColorFlags		  	MonitorSetting.ColorFlag
            // 2006/10/26 6:47PM by Emily #define	UserPrefDLCMode		MonitorSetting.DLCModeType
            //#define	UserPrefMWEMode		MonitorSetting.MWEModeType
            //#define UserPrefBFPosition 				MonitorSetting.BF_Position
            //#define UserPrefBFVstart 				MonitorSetting.BF_Vstart
            #define UserPrefBFSize 				MonitorSetting.BF_Size
            #define UserPrefBFHSize 				MonitorSetting.BF_HSize
            #define UserPrefBFVSize 				MonitorSetting.BF_VSize
            #define UserPrefBFHStart 				MonitorSetting.BF_Hstart
            #define UserPrefBFVStart 				MonitorSetting.BF_Vstart
        #endif
	#ifdef OffPower         //090331 xiandi.yu Auto Off Power
	#define UserPrefAutoPowerOff    MonitorSetting.AutoPowerOff
	#endif
        #if Enable_Expansion
            #define UserprefExpansionMode        MonitorSetting.ExpansionMode
        #endif
        #define UserprefHistogram1              MonitorSetting.Histogram1
        #define UserprefHistogram2              MonitorSetting.Histogram2
        #define UserprefALha              MonitorSetting.ALhaA
        #define UserprefBata              MonitorSetting.BataA
        // 2006/11/6 5:34PM by Emily #define UserprefAutoModel     MonitorSetting.AutoModel
        // 2006/11/6 5:33PM by Emily #define UserprefCompareRange    MonitorSetting.CompareRange
        // 2006/11/6 5:33PM by Emily #define UserprefDiffHvalue                MonitorSetting.DiffHvalue
        #define MSBankNo			MonitorSetting.BankNo
        #define MSAddr				MonitorSetting.Addr
        #define MSValue				MonitorSetting.Value
    #endif
	#endif

        #if Enable_Lightsensor
        #define UserPrefiCareMode     MonitorSetting.iCareMode
        extern bit bLightSensorActStepFlag; // unit: set the flag per LightSensorStepCnt 
        #define LightSensorStepCnt          20 // unit: mS
        extern BYTE xdata TempBrightness;
        extern bit DetectBriFlag;	//120813 Modify
        extern BYTE xdata SetBriValue;
        extern WORD xdata LightSensorAdcOld, LightSensorAdcNew;

        #define Set_DetectBriFlag()     (DetectBriFlag = 1)	//120813 Modify
        #define Clr_DetectBriFlag()     (DetectBriFlag = 0)	//120813 Modify
        #endif

        #ifdef EnableLightBar
        #define UserPrefLightBar        MonitorSetting.LightBar
        #endif


        #define UserPrefLastMenuIndex		   LastMenuPage
	
    #if MWEFunction
        // Color Flags
	 #define bFullEnhanceBit		BIT0
        #define bSkinBit				BIT1
        #define bBlueBit				BIT2
        #define bGreenBit			BIT3
        #define bColorAutoDetectBit 	BIT4
        #define bDemoBit			BIT5
	 
        #define bPictureBoostBit		BIT6
        #define bMWEModeBit		BIT7
        #define MWEModeFlag				(UserPrefColorFlags&bMWEModeBit)
        #define Set_MWEModeFlag()			(UserPrefColorFlags|=bMWEModeBit)
        #define Clr_MWEModeFlag()			(UserPrefColorFlags&=~bMWEModeBit)
        #define ColorAutoDetectFlag				(UserPrefColorFlags&bColorAutoDetectBit)
        #define Set_ColorAutoDetectFlag()			(UserPrefColorFlags|=bColorAutoDetectBit )
        #define Clr_ColorAutoDetectFlag()			(UserPrefColorFlags&=~bColorAutoDetectBit )
        #define SkinFlag					(UserPrefColorFlags&bSkinBit)
        #define Set_SkinFlag()			(UserPrefColorFlags|=bSkinBit)
        #define Clr_SkinFlag()			      (UserPrefColorFlags&=~bSkinBit)
        #define BlueFlag					(UserPrefColorFlags&bBlueBit)
        #define Set_BlueFlag()			(UserPrefColorFlags|=bBlueBit)
        #define Clr_BlueFlag()			      (UserPrefColorFlags&=~bBlueBit)
        #define GreenFlag			 	(UserPrefColorFlags&bGreenBit)
        #define Set_GreenFlag()			(UserPrefColorFlags|=bGreenBit)
        #define Clr_GreenFlag()			(UserPrefColorFlags&=~bGreenBit)
        #define DemoFlag			 	(UserPrefColorFlags&bDemoBit	)
        #define Set_DemoFlag()			(UserPrefColorFlags|=bDemoBit	)
        #define Clr_DemoFlag()			(UserPrefColorFlags&=~bDemoBit	)
        #define FullEnhanceFlag			 	      (UserPrefColorFlags&bFullEnhanceBit	)
        #define Set_FullEnhanceFlag()			      (UserPrefColorFlags|=bFullEnhanceBit	)
        #define Clr_FullEnhanceFlag()			      (UserPrefColorFlags&=~bFullEnhanceBit	)
        #define PictureBoostFlag			 	      (UserPrefColorFlags&bPictureBoostBit	)
        #define Set_PictureBoostFlag()			      (UserPrefColorFlags|=bPictureBoostBit	)
        #define Clr_PictureBoostFlag()			      (UserPrefColorFlags&=~bPictureBoostBit	)
    #endif
    //============================================================================================
    // current input timing setting
    #define UserPrefHTotal			ModeSetting.HTotal
    #define UserPrefHStart			ModeSetting.HStart
    #define UserPrefVStart			ModeSetting.VStart
    #define UserPrefPhase			ModeSetting.Phase
    #define UserPrefAutoHStart		ModeSetting.AutoHStart
    #define UserPrefAutoVStart		ModeSetting.AutoVStart	//2004/01/15 num11
    #define UserPrefAutoTimes		ModeSetting.AutoTimes
    //======================================================================================
    #define SyncLoss			(bHSyncLoss|bVSyncLoss)
    #define SyncPolarity(status)		(status&0x03)
    #define GetSyncPolarity(status)		(1<<(status&0x03))
    //=================================================================================
    // Input Timing information flags
    #define SyncLossState()			(SrcFlags&SyncLoss)
    #define UserModeFlag			(SrcFlags&bUserMode)
    #define UnsupportedModeFlag		(SrcFlags&bUnsupportMode)
    #define NativeModeFlag			(SrcFlags&bNativeMode)
    #define ShrinkModeFlag			(SrcFlags&bShrinkMode)
    //==================================================================================
    // Monitor status flags for storing into NVRAM
    #define bPowerOnBit			BIT0	// system power on/off status
    #define bFactoryModeBit		BIT1	// factory mode
    #define bBurninModeBit			BIT2	// burnin mode
    #define bDoBurninModeBit		BIT3
    #define bOsdLockModeBit 		BIT4
    #define bShowOsdLockBit 		BIT5
    //#define bMuteOnBit				BIT6
    #define bOverDriveOnBit		BIT6	
    #define bDCCCIONOBit			BIT7
    //========= flags status
    #define PowerOnFlag			(MonitorFlags&bPowerOnBit)
    #define FactoryModeFlag		(MonitorFlags&bFactoryModeBit)
    #define BurninModeFlag			(MonitorFlags&bBurninModeBit)
    #define DoBurninModeFlag		(MonitorFlags&bDoBurninModeBit)
    #define OsdLockModeFlag 		(MonitorFlags&bOsdLockModeBit)
    #define ShowOsdLockFlag 		(MonitorFlags&bShowOsdLockBit)
    //#define MuteOnFlag			    (MonitorFlags&bMuteOnBit)
    #define OverDriveOnFlag 			(MonitorFlags&bOverDriveOnBit)
    #define DDCciFlag            	(MonitorFlags&bDCCCIONOBit)
    //--------- flags setting
    #define Set_PowerOnFlag()		  (MonitorFlags|=bPowerOnBit)
    #define Set_FactoryModeFlag()	  (MonitorFlags|=bFactoryModeBit)
    #define Set_BurninModeFlag()	  (MonitorFlags|=bBurninModeBit)
    #define Set_DoBurninModeFlag()  (MonitorFlags|=bDoBurninModeBit)
    #define Set_OsdLockModeFlag()	  (MonitorFlags|=bOsdLockModeBit)
    #define Set_ShowOsdLockFlag()	  (MonitorFlags|=bShowOsdLockBit)
    //#define Set_MuteOnFlag()		  (MonitorFlags|=bMuteOnBit)
    #define Set_OverDriveOnFlag() 	(MonitorFlags|=bOverDriveOnBit)
    #define Set_DDCciFlag()		(MonitorFlags|=bDCCCIONOBit)
    #define Clr_PowerOnFlag()		  (MonitorFlags&=~bPowerOnBit)
    #define Clr_FactoryModeFlag()	  (MonitorFlags&=~bFactoryModeBit)
    #define Clr_BurninModeFlag()	  (MonitorFlags&=~bBurninModeBit)
    #define Clr_DoBurninModeFlag() (MonitorFlags&=~bDoBurninModeBit)
    #define Clr_OsdLockModeFlag()	  (MonitorFlags&=~bOsdLockModeBit)
    #define Clr_ShowOsdLockFlag()	  (MonitorFlags&=~bShowOsdLockBit)
    //#define Clr_MuteOnFlag()		  (MonitorFlags&=~bMuteOnBit)
    #define Clr_OverDriveOnFlag() 	(MonitorFlags&=~bOverDriveOnBit)
    #define Clr_DDCciFlag()		(MonitorFlags&=~bDCCCIONOBit)
    //==================================================================================

#if VGA_EDID_Write_To_24C02
#define bVGA_EDID_Bit                       BIT8
#define VGA_EDID_Flag            	(MonitorFlags&bVGA_EDID_Bit)
#define Set_VGA_EDID_Flag()		   (MonitorFlags|=bVGA_EDID_Bit)
#define Clr_VGA_EDID_Flag()		   (MonitorFlags&=~bVGA_EDID_Bit)
#endif


#if 1

    //========== system flags status
   // extern bit InputTimingChangeFlag;
    //extern bit PowerSavingFlag;
   // extern bit ForcePowerSavingFlag;
  //  extern bit InputTimingStableFlag;
  //  extern bit DoModeSettingFlag;
   // extern bit CableNotConnectedFlag;
   // extern bit FreeRunModeFlag;
    extern bit RxBusyFlag;
    //extern bit OsdTimeoutFlag;
   // extern bit ChangePatternFlag;
   // extern bit SaveSettingFlag;
    extern bit ms500Flag;
   // extern bit ReverseXFlag;
   // extern bit ReverseYFlag;
    //extern bit ISPFlag;

    extern bit AC_PowerFlag;

   // extern bit DisplayLogoFlag;
    extern bit FlashFlag;
  //extern bit ReadyForChangeFlag;
  //  extern bit LoadOSDDataFlag;
  //  extern bit LoadFontFlag;
   // extern bit AutoColorFlag;
    extern bit DLKOverFlag;
  //  extern bit ShowInputInfoFlag;
  //  extern bit JustShowValueFlag;
    extern bit DecVScaleFlag	;
  //  extern bit PanelOnFlag;

//   extern bit PressExitFlag;
    extern bit EnableShowDDCFlag;
   // extern bit PressMenuFlag;
   // extern bit MuteFlag;
    //extern bit SecondFlag;
   // extern bit DebugOnlyFlag;

   // extern bit ProductModeFlag;
   // extern bit PowerGoodExistFlag;
#if DisplayEnergyStar
	extern bit ShowEpaAfterLogoFlag;		//071228
#endif



#if FDDCCI_WriteEDID
extern bit ubAutoAlign; 
#endif


    #define InputTimingChangeBit                        BIT0
    #define InputTimingChangeFlag	  	             (SystemFlags&InputTimingChangeBit)
    #define Set_InputTimingChangeFlag()	  	(SystemFlags|=InputTimingChangeBit)
    #define Clr_InputTimingChangeFlag()	  	(SystemFlags&=~InputTimingChangeBit)

 //   #define Set_InputTimingChangeFlag()	        	(InputTimingChangeFlag=1)
 //   #define Clr_InputTimingChangeFlag()	        	(InputTimingChangeFlag=0)
	
	#define PowerSavingBit                          BIT1
	#define PowerSavingFlag                      (SystemFlags&PowerSavingBit) 
      #define Set_PowerSavingFlag()		  	(SystemFlags|=PowerSavingBit)
      #define Clr_PowerSavingFlag()		  	(SystemFlags&=~PowerSavingBit)

	
	#define ForcePowerSavingBit                          BIT2
	#define ForcePowerSavingFlag                      (SystemFlags&ForcePowerSavingBit) 
      #define Set_ForcePowerSavingFlag()		  (SystemFlags|=ForcePowerSavingBit)
      #define Clr_ForcePowerSavingFlag()		  (SystemFlags&=~ForcePowerSavingBit)
	  
	
    #define InputTimingStableBit                    BIT4
    #define InputTimingStableFlag                (SystemFlags&InputTimingStableBit)
    #define Set_InputTimingStableFlag()	   	(SystemFlags|=InputTimingStableBit)
    #define Clr_InputTimingStableFlag()	   	(SystemFlags&=~InputTimingStableBit)
	
	
    #define   DoModeSettingBit                         BIT5
    #define   DoModeSettingFlag                    (SystemFlags&DoModeSettingBit)
    #define   Set_DoModeSettingFlag()		 (SystemFlags|=DoModeSettingBit)
    #define   Clr_DoModeSettingFlag()		 (SystemFlags&=~DoModeSettingBit)
	
    #define Set_RxBusyFlag()		               		 (RxBusyFlag=1)
    #define Clr_RxBusyFlag()		               		 (RxBusyFlag=0)
	

	
	#define OsdTimeoutBit                         BIT6
	#define OsdTimeoutFlag                      (SystemFlags&OsdTimeoutBit) 
     #define Set_OsdTimeoutFlag()			(SystemFlags|=OsdTimeoutBit)
    #define Clr_OsdTimeoutFlag()		  	(SystemFlags&=~OsdTimeoutBit)

	
	#define CableNotConnectedBit                	BIT7
	#define CableNotConnectedFlag            	(SystemFlags&CableNotConnectedBit)
     #define Set_CableNotConnectedFlag()	  	(SystemFlags|=CableNotConnectedBit)
    #define Clr_CableNotConnectedFlag()	  	(SystemFlags&=~CableNotConnectedBit)
	
	#define FreeRunModeBit                        BIT8
	#define FreeRunModeFlag                   (SystemFlags&FreeRunModeBit)
     #define  Set_FreeRunModeFlag()		  	(SystemFlags|=FreeRunModeBit)
    #define  Clr_FreeRunModeFlag()		  	(SystemFlags&=~FreeRunModeBit)	
	
	#define ChangePatternBit                             BIT9
	#define ChangePatternFlag                        (SystemFlags&ChangePatternBit)
    #define Set_ChangePatternFlag()			(SystemFlags|=ChangePatternBit)
    #define Clr_ChangePatternFlag()			(SystemFlags&=~ChangePatternBit)
	
	#define SaveSettingBit                        BIT10
	#define SaveSettingFlag                  	(SystemFlags&SaveSettingBit)
	#define Set_SaveSettingFlag()		 	(SystemFlags|=SaveSettingBit)
    #define Clr_SaveSettingFlag()			(SystemFlags&=~SaveSettingBit)
	
    #define Set_ms500Flag()			    		        (ms500Flag=1)
    #define Clr_ms500Flag()			    		        (ms500Flag=0)
	
	#define ReverseXBit                             BIT11
	#define ReverseXFlag                       	(SystemFlags&ReverseXBit)
    #define Set_ReverseXFlag()		 		(SystemFlags|=ReverseXBit)
    #define Clr_ReverseXFlag()		    		(SystemFlags&=~ReverseXBit)	
	
	#define ReverseYBit                       BIT12
	#define ReverseYFlag                     (SystemFlags&ReverseYBit)
    #define Set_ReverseYFlag()		    	(SystemFlags|=ReverseYBit)
    #define Clr_ReverseYFlag()		       (SystemFlags&=~ReverseYBit)
    //#define Set_ISPFlag()			                  		(ISPFlag=1)
    //#define Clr_ISPFlag()			                  		(ISPFlag=0)
    #define Set_AC_PowerFlag()			            	(AC_PowerFlag=1)
    #define Clr_AC_PowerFlag()			            		(AC_PowerFlag=0)

	#define DisplayLogoBit                    BIT13
	#define DisplayLogoFlag                 (SystemFlags&DisplayLogoBit)
    #define Set_DisplayLogoFlag()		(SystemFlags|=DisplayLogoBit)
    #define Clr_DisplayLogoFlag()	      	(SystemFlags&=~DisplayLogoBit)
	
    #define Set_FlashFlag() 			                     (FlashFlag=1)
    #define Clr_FlashFlag() 			                            (FlashFlag=0)
	

	
	#define LoadOSDDataBit                 	 BIT14
	#define LoadOSDDataFlag             		(SystemFlags&LoadOSDDataBit)
    	#define Set_LoadOSDDataFlag()		 (SystemFlags|=LoadOSDDataBit)
    	#define Clr_LoadOSDDataFlag()		 (SystemFlags&=~LoadOSDDataBit)

	#define  ReadyForChangeBit                   	BIT0
	#define  ReadyForChangeFlag            		(System2Flags&ReadyForChangeBit)
	#define Set_ReadyForChangeFlag()      		(System2Flags|=ReadyForChangeBit)
       #define Clr_ReadyForChangeFlag()     		(System2Flags&=~ReadyForChangeBit)
		
	#define LoadFontBit                        	BIT1
	#define LoadFontFlag                   		(System2Flags&LoadFontBit)
    	 #define Set_LoadFontFlag()		     	(System2Flags|=LoadFontBit)
   	 #define Clr_LoadFontFlag()		     	(System2Flags&=~LoadFontBit) 
	
	 #define AutoColorBit                  BIT2
	 #define AutoColorFlag			(System2Flags&AutoColorBit)
    	#define Set_AutoColorFlag() 	     	(System2Flags|=AutoColorBit)
    	#define Clr_AutoColorFlag() 	      	(System2Flags&=~AutoColorBit)
	
    #define Set_DLKOverFlag() 						(DLKOverFlag=1)
    #define Clr_DLKOverFlag()  						(DLKOverFlag=0)
	
	#define  ShowInputInfoBit                           BIT3
	#define ShowInputInfoFlag           			(System2Flags&ShowInputInfoBit)
  	#define Set_ShowInputInfoFlag()   		(System2Flags|=ShowInputInfoBit)
    	#define Clr_ShowInputInfoFlag()    		(System2Flags&=~ShowInputInfoBit)
	
    #define JustShowValueBit                     BIT4
    #define JustShowValueFlag                  (System2Flags&JustShowValueBit)   
    #define Set_JustShowValueFlag()    		(System2Flags|=JustShowValueBit)
    #define Clr_JustShowValueFlag()    		(System2Flags&=~JustShowValueBit)
	
	#define MuteBit                                       BIT5
	#define MuteFlag                      		(System2Flags&MuteBit)
	#define Set_MuteFlag()   				(System2Flags|=MuteBit)
	#define Clr_MuteFlag()   				(System2Flags&=~MuteBit)

	    #define PressMenuBit                             BIT6
    #define PressMenuFlag                           (System2Flags&PressMenuBit)
    #define Set_PressMenuFlag()   			(System2Flags|=PressMenuBit)
    #define Clr_PressMenuFlag()   			(System2Flags&=~PressMenuBit)	
	
     #define PanelOnBit                        BIT7
    #define PanelOnFlag                     (System2Flags&PanelOnBit)
    #define Set_PanelOnFlag()		     	(System2Flags|=PanelOnBit)
    #define Clr_PanelOnFlag()		     	(System2Flags&=~PanelOnBit)

      #define PressExitBit                             BIT8
    #define  PressExitFlag                           (System2Flags&PressMenuBit)
    #define Set_PressExitFlag()   			(System2Flags|=PressMenuBit)
    #define Clr_PressExitFlag()   			(System2Flags&=~PressMenuBit)
	
    #define Set_DecVScaleFlag()	       			(DecVScaleFlag=1)
    #define Clr_DecVScaleFlag()	      					(DecVScaleFlag=0)

    #define Set_EnableShowDDCFlag()    				(EnableShowDDCFlag=1)
    #define Clr_EnableShowDDCFlag()    				(EnableShowDDCFlag=0)
	
    #if Enable_Expansion
    // extern bit ExpansionFlag;
	#define ExpansionBit                                        BIT9
	#define ExpansionFlag                                         (System2Flags&ExpansionBit) 
     #define Set_ExpansionFlag()					(System2Flags|=ExpansionBit)
     #define Clr_ExpansionFlag()					(System2Flags&=~ExpansionBit)
	 
	 extern WORD xdata DispalyWidth;
	 extern WORD xdata HStartOffset;
    #endif
	
	#define SecondBit 					BIT10
	#define SecondFlag                       		(System2Flags&SecondBit)
    	#define Set_SecondFlag()			     	(System2Flags|=SecondBit)
   	 #define Clr_SecondFlag()			       (System2Flags&=~SecondBit)

	#define DebugOnlyBit 						BIT11
	#define DebugOnlyFlag                       		(System2Flags&DebugOnlyBit)
    	#define Set_DebugOnlyFlag()			     	(System2Flags|=DebugOnlyBit)
   	 #define Clr_DebugOnlyFlag()			       (System2Flags&=~DebugOnlyBit)

	#define ProductModeBit 						BIT12
	#define ProductModeFlag                       		(System2Flags&ProductModeBit)
    	#define Set_ProductModeFlag()			     	(System2Flags|=ProductModeBit)
   	 #define Clr_ProductModeFlag()			       (System2Flags&=~ProductModeBit) 
	
	#define PowerGoodExistBit 						BIT13
	#define PowerGoodExistFlag                       		(System2Flags&PowerGoodExistBit)
    	#define Set_PowerGoodExistFlag()			     	(System2Flags|=PowerGoodExistBit)
   	 #define Clr_PowerGoodExistFlag()			       (System2Flags&=~PowerGoodExistBit) 
    
#if HKC_ColorTemp_Adjust||FDDCCI_WriteEDID

#define ATEModeBit                                   BIT14
#define ATEModeFlag 						(System2Flags&ATEModeBit)
#define Set_ATEModeFlag()				(System2Flags|=ATEModeBit)
#define Clr_ATEModeFlag()				(System2Flags&=~ATEModeBit)

#define bDDCCIResetPowerOffBit					 BIT15
#define DDCCIResetPowerOffFlag				      (System2Flags&bDDCCIResetPowerOffBit)
#define Set_DDCCIResetPowerOffFlag()			(System2Flags|=bDDCCIResetPowerOffBit)
#define Clr_DDCCIResetPowerOffFlag()			(System2Flags&=~bDDCCIResetPowerOffBit)

#endif

#if IT6251
#define bCheckITE6251bit                  BIT0
#define CheckITE6251Flag                 (System3Flags&bCheckITE6251bit)
#define Set_CheckITE6251Flag()        (System3Flags|=bCheckITE6251bit)
#define Clr_CheckITE6251Flag()         (System3Flags&=~bCheckITE6251bit)
#endif

	//#if (ModelName == ENVISION_P2271w || ModelName == ENVISION_P2471w||ModelName==ENVISION_P971wl)//|| ModelName==ENVISION_P2071l)
	#ifdef GreenLEDDutyCtrl
	extern bit DuringResetFlag;
	#define Set_DuringResetFlag()					(DuringResetFlag=1)
	#define Clr_DuringResetFlag()					(DuringResetFlag=0)
	#endif


	#if Enable_Expansion	//120204 Modify
	#define DISPLAY_WIDTH	DispalyWidth
	#define HSTART_OFFSET	HStartOffset 
	#else
	#define DISPLAY_WIDTH	PanelWidth
	#define HSTART_OFFSET	0
	#endif


#if OSD_PIVOT_EN  //071226

    extern bit OSDRotateFlag;
    extern BYTE xdata PivotStatusDebunce;
    #define Set_OSDRotateFlag()			 (OSDRotateFlag=1)
    #define Clr_OSDRotateFlag()		        (OSDRotateFlag=0)
#endif
#if ENABLE_HDCP


        //extern bit HpdReadyFlag;
        extern bit HdcpNVRamFlag;

        //#define Set_HpdReadyFlag()			(HpdReadyFlag=1)
        //#define Clr_HpdReadyFlag()			(HpdReadyFlag=0)

        #define Set_HdcpNVRamFlag()			(HdcpNVRamFlag=1)
        #define Clr_HdcpNVRamFlag()			(HdcpNVRamFlag=0)
        #if HDCPKEY_IN_Flash
        extern bit LoadHDCPKeyFlag;
        extern bit WriteHDCPcodeFlag;

        #define Set_LoadHDCPKeyFlag()    (LoadHDCPKeyFlag=1)
        #define Clr_LoadHDCPKeyFlag()     (LoadHDCPKeyFlag=0)

        #define Set_WriteHDCPcodeFlag()	      (WriteHDCPcodeFlag=1)
        #define Clr_WriteHDCPcodeFlag()    	(WriteHDCPcodeFlag=0)
        #endif


#endif
#ifdef UseVGACableReadWriteAllPortsEDID
    #ifdef ReduceDDC
    extern bit LoadEDIDFlag;
    #define Set_LoadEDIDFlag()			 	(LoadEDIDFlag=1)
    #define Clr_LoadEDIDFlag()		        	(LoadEDIDFlag=0)
    #endif
    extern bit WriteDeviceFlag;
    #define Set_WriteDeviceFlag()			 (WriteDeviceFlag=1)
    #define Clr_WriteDeviceFlag()		        (WriteDeviceFlag=0)
#endif


    #if Backupdata


        extern bit GetChecksumFlag;
        extern bit DDCCIAadjustFlag;
        extern bit DonotchangeBackFlag;
        extern bit ACONFlag;

        #define Set_GetChecksumFlag()      (GetChecksumFlag=1)
        #define Clr_GetChecksumFlag()      (GetChecksumFlag=0)

        #define Set_DDCCIAadjustFlag()      (DDCCIAadjustFlag=1)
        #define Clr_DDCCIAadjustFlag()      (DDCCIAadjustFlag=0)

        #define Set_DonotchangeBackFlag()      (DonotchangeBackFlag=1)
        #define Clr_DonotchangeBackFlag()      (DonotchangeBackFlag=0)

        #define Set_ACONFlag()      (ACONFlag=1)
        #define Clr_ACONFlag()      (ACONFlag=0)
    #endif




        extern bit MainOSDFlag;
        extern bit SubOSDFlag;
	 extern bit PowerKeyFlag;
	 extern bit PowerOffFlag;
	 extern bit DoAutoConfigFlag;
	 extern bit DoResetFlag;
	 extern bit DoKeyCountFlag;
	 #if AllInOneKey
           #if !ScanKey
	 extern bit FactoryPowerOnFlag;
	    #endif
	#endif
	extern bit DcrOpenScikeFlag;

	 #define Set_MainOSDFlag()      (MainOSDFlag=1)
	 #define Clr_MainOSDFlag()      (MainOSDFlag=0)
	 #define Set_SubOSDFlag()      (SubOSDFlag=1)
	 #define Clr_SubOSDFlag()      (SubOSDFlag=0)
	 #define Set_PowerKeyFlag()      (PowerKeyFlag=1)
	 #define Clr_PowerKeyFlag()      (PowerKeyFlag=0)
	 #define Set_PowerOffFlag()      (PowerOffFlag=1)
	 #define Clr_PowerOffFlag()      (PowerOffFlag=0)
	 #if AllInOneKey
             #if !ScanKey
	 #define Set_FactoryPowerOnFlag()  (FactoryPowerOnFlag=1)
	 #define Clr_FactoryPowerOnFlag()  (FactoryPowerOnFlag=0)
	      #endif
	 #endif
	 #define Set_DoAutoConfigFlag()      (DoAutoConfigFlag=1)
	 #define Clr_DoAutoConfigFlag()      (DoAutoConfigFlag=0)
	 #define Set_DoResetFlag()      (DoResetFlag=1)
	 #define Clr_DoResetFlag()      (DoResetFlag=0)
	 #define Set_DoKeyCountFlag()      (DoKeyCountFlag=1)
	 #define Clr_DoKeyCountFlag()      (DoKeyCountFlag=0)
#if DisplayEnergyStar
     #define Set_ShowEpaAfterLogoFlag()			(ShowEpaAfterLogoFlag=1)
	 #define Clr_ShowEpaAfterLogoFlag()			(ShowEpaAfterLogoFlag=0)//1228
#endif

	 #define Set_DcrOpenScikeFlag()      (DcrOpenScikeFlag=1)
	 #define Clr_DcrOpenScikeFlag()      (DcrOpenScikeFlag=0)
//extern bit SaveFactorySettingFlag;
//    #define Set_SaveFactorySettingFlag()			 (SaveFactorySettingFlag=1)
//    #define Clr_SaveFactorySettingFlag()		        (SaveFactorySettingFlag=0)
    #if MWEFunction
        // Color Flags
	 #define bDrawFullEnhanceBit		BIT0
        #define bDrawSkinBit				BIT1
        #define bDrawBlueBit				BIT2
        #define bDrawGreenBit			BIT3
        #define bDrawColorAutoDetectBit 	BIT4
        #define DrawColorAutoDetectFlag				(BoostTempValue&bDrawColorAutoDetectBit)
        #define Set_DrawColorAutoDetectFlag()			(BoostTempValue|=bDrawColorAutoDetectBit )
        #define Clr_DrawColorAutoDetectFlag()			(BoostTempValue&=~bDrawColorAutoDetectBit )
        #define DrawSkinFlag					(BoostTempValue&bDrawSkinBit)
        #define Set_DrawSkinFlag()			(BoostTempValue|=bDrawSkinBit)
        #define Clr_DrawSkinFlag()			      (BoostTempValue&=~bDrawSkinBit)
        #define DrawBlueFlag					(BoostTempValue&bDrawBlueBit)
        #define Set_DrawBlueFlag()			(BoostTempValue|=bDrawBlueBit)
        #define Clr_DrawBlueFlag()			      (BoostTempValue&=~bDrawBlueBit)
        #define DrawGreenFlag			 	(BoostTempValue&bDrawGreenBit)
        #define Set_DrawGreenFlag()			(BoostTempValue|=bDrawGreenBit)
        #define Clr_DrawGreenFlag()			(BoostTempValue&=~bDrawGreenBit)
        #define DrawFullEnhanceFlag			 	      (BoostTempValue&bDrawFullEnhanceBit)
        #define Set_DrawFullEnhanceFlag()			      (BoostTempValue|=bDrawFullEnhanceBit)
        #define Clr_DrawFullEnhanceFlag()			      (BoostTempValue&=~bDrawFullEnhanceBit)
		
	 #define bDrawItemRadioGroupScikeBit 	BIT5
        #define DrawItemRadioGroupScikeFlag			 	      (BoostTempValue&bDrawItemRadioGroupScikeBit)
        #define Set_DrawItemRadioGroupScikeFlag()			      (BoostTempValue|=bDrawItemRadioGroupScikeBit)
        #define Clr_DrawItemRadioGroupScikeFlag()			      (BoostTempValue&=~bDrawItemRadioGroupScikeBit)
    #endif

#if EnablePanelServiceMenu

	
    extern bit SavePanelSettingFlag;
    #define Set_SavePanelSettingFlag()			 (SavePanelSettingFlag=1)
    #define Clr_SavePanelSettingFlag()		        (SavePanelSettingFlag=0)
    extern bit TestMinDutyFlag;
    #define Set_TestMinDutyFlag()			 (TestMinDutyFlag=1)
    #define Clr_TestMinDutyFlag()		        (TestMinDutyFlag=0)
#endif
	 

#else
    // System status flags
    #define bInputTimingChangeBit		BIT0
    #define bPowerSavingBit			BIT1	// system power saving status
    #define bForcePowerSavingBit		BIT2	// indicated system must enter power down
    #define bInputTimingStableBit		BIT3	// input timing stable
    #define bDoModeSettingBit		    BIT4	//
    #define bCableNotConnectedBit		BIT5
    #define bFreeRunModeBit			BIT6
    #define bOsdTimeoutBit			    BIT7	// osd timeout
    #define bSaveSettingBit			BIT8	// flag for storing settings into NVRam
    #define bChangePatternBit		    BIT9	// switch input port
    #define bRxBusyBit			        BIT10	// debug is busy
    #define bms500Bit			        BIT11
    #define bReverseXBit			    BIT12
    #define bReverseYBit			    BIT13
    #define bISPBit				        BIT14
    //#define bAutoColorBit			    BIT15
    #define bAC_PowerBit			    BIT15
    //=========System2 Flag Bit================================================================
    #define bDisplayLogoBit 			BIT0
    #define bFlashBit			        BIT1
    #define bReadyForChangeBit		BIT2
    #define bLoadOSDDataBit 			BIT3
    #define bLoadFontBit				BIT4
    #define bAutoColorBit 			    BIT5
    #define bDLKOverBit					BIT6
    //#define bDisableLuminanceBit		BIT6
    //#define bSpeedupIncBrighnessBit     BIT8
    #define bShowInputInfoBit			BIT10
    #define bJustShowValueBit			BIT11
    #define bDecVScaleBit				BIT12
    #define bPanelOnBit			BIT13
    #define bPressExitKeyBit			BIT7
    #define bEnableShowDDCBit		BIT8
    #define bPressMenuKeyBit			BIT14
    #define bMuteBit			BIT15
    //========== system flags status
    #define InputTimingChangeFlag		(SystemFlags&bInputTimingChangeBit)
    #define PowerSavingFlag			(SystemFlags&bPowerSavingBit)
    #define ForcePowerSavingFlag		(SystemFlags&bForcePowerSavingBit)
    #define InputTimingStableFlag		(SystemFlags&bInputTimingStableBit)
    #define DoModeSettingFlag		    (SystemFlags&bDoModeSettingBit)
    #define CableNotConnectedFlag		(SystemFlags&bCableNotConnectedBit)
    #define FreeRunModeFlag			(SystemFlags&bFreeRunModeBit)
    #define RxBusyFlag			        (SystemFlags&bRxBusyBit)
    #define OsdTimeoutFlag			    (SystemFlags&bOsdTimeoutBit)
    #define ChangePatternFlag		    (SystemFlags&bChangePatternBit)
    #define SaveSettingFlag			(SystemFlags&bSaveSettingBit)
    #define ms500Flag			        (SystemFlags&bms500Bit)
    #define ReverseXFlag			    (SystemFlags&bReverseXBit)
    #define ReverseYFlag			    (SystemFlags&bReverseYBit)
    #define ISPFlag				        (SystemFlags&bISPBit)
    //#define AutoColorFlag 			(SystemFlags&bAutoColorBit)
    #define AC_PowerFlag				(SystemFlags&bAC_PowerBit)
    //--------- System2 Flag status------
    #define DisplayLogoFlag 	        (System2Flags&bDisplayLogoBit)
    #define FlashFlag		        (System2Flags&bFlashBit)
    #define ReadyForChangeFlag	        (System2Flags&bReadyForChangeBit)
    #define LoadOSDDataFlag 	        (System2Flags&bLoadOSDDataBit)
    #define LoadFontFlag		        (System2Flags&bLoadFontBit)
    #define AutoColorFlag				(System2Flags&bAutoColorBit)
    //#define DisableLuminanceFlag 		(System2Flags&bDisableLuminanceBit)
    #define DLKOverFlag 				(System2Flags&bDLKOverBit)
    //#define SpeedupIncBrighnessFlag			(System2Flags&bSpeedupIncBrighnessBit)
    #define ShowInputInfoFlag			(System2Flags&bShowInputInfoBit)
    #define JustShowValueFlag			(System2Flags&bJustShowValueBit)
    #define DecVScaleFlag				(System2Flags&bDecVScaleBit)
    #define PanelOnFlag				(System2Flags&bPanelOnBit)
    #if Enable_Expansion
        #define bExpansionBit               BIT9
        #define ExpansionFlag             (System2Flags&bExpansionBit)
        #define Set_ExpansionFlag()	(System2Flags|=bExpansionBit)
        #define Clr_ExpansionFlag()	(System2Flags&=~bExpansionBit)
    #endif
    //=======================================================================================//
    // for hot key control
    //=======================================================================================//
    #define PressExitFlag   			(System2Flags&bPressExitKeyBit)
    #define EnableShowDDCFlag   		(System2Flags&bEnableShowDDCBit)
    #define PressMenuFlag   			(System2Flags&bPressMenuKeyBit)
    #define MuteFlag   		(System2Flags&bMuteBit)
    //--------- system flags setting
    #define Set_InputTimingChangeFlag()	(SystemFlags|=bInputTimingChangeBit)
    #define Clr_InputTimingChangeFlag()	(SystemFlags&=~bInputTimingChangeBit)
    #define Set_PowerSavingFlag()		    (SystemFlags|=bPowerSavingBit)
    #define Clr_PowerSavingFlag()		    (SystemFlags&=~bPowerSavingBit)
    #define Set_ForcePowerSavingFlag()	(SystemFlags|=bForcePowerSavingBit)
    #define Clr_ForcePowerSavingFlag()	(SystemFlags&=~bForcePowerSavingBit)
    #define Set_InputTimingStableFlag()	(SystemFlags|=bInputTimingStableBit)
    #define Clr_InputTimingStableFlag()	(SystemFlags&=~bInputTimingStableBit)
    #define Set_RxBusyFlag()		        (SystemFlags|=bRxBusyBit)
    #define Clr_RxBusyFlag()		        (SystemFlags&=~bRxBusyBit)
    #define Set_DoModeSettingFlag()		(SystemFlags|=bDoModeSettingBit)
    #define Clr_DoModeSettingFlag()		(SystemFlags&=~bDoModeSettingBit)
    #define Set_OsdTimeoutFlag()		    (SystemFlags|=bOsdTimeoutBit)
    #define Clr_OsdTimeoutFlag()		    (SystemFlags&=~bOsdTimeoutBit)
    #define Set_CableNotConnectedFlag()	(SystemFlags|=bCableNotConnectedBit)
    #define Clr_CableNotConnectedFlag()	(SystemFlags&=~bCableNotConnectedBit)
    #define Set_FreeRunModeFlag()		    (SystemFlags|=bFreeRunModeBit)
    #define Clr_FreeRunModeFlag()		    (SystemFlags&=~bFreeRunModeBit)
    #define Set_ChangePatternFlag()		(SystemFlags|=bChangePatternBit)
    #define Clr_ChangePatternFlag()		(SystemFlags&=~bChangePatternBit)
    #define Set_SaveSettingFlag()		    (SystemFlags|=bSaveSettingBit)
    #define Clr_SaveSettingFlag()		    (SystemFlags&=~bSaveSettingBit)
    #define Set_ms500Flag()			    (SystemFlags|=bms500Bit)
    #define Clr_ms500Flag()			    (SystemFlags&=~bms500Bit)
    #define Set_ReverseXFlag()		    (SystemFlags|=bReverseXBit)
    #define Clr_ReverseXFlag()		    (SystemFlags&=~bReverseXBit)
    #define Set_ReverseYFlag()		    (SystemFlags|=bReverseYBit)
    #define Clr_ReverseYFlag()		    (SystemFlags&=~bReverseYBit)
    #define Set_ISPFlag()			        (SystemFlags|=bISPBit)
    #define Clr_ISPFlag()			        (SystemFlags&=~bISPBit)
    #define Set_AC_PowerFlag()			(SystemFlags|=bAC_PowerBit)
    #define Clr_AC_PowerFlag()			(SystemFlags&=~bAC_PowerBit)
    //=============================================================
    //Set AOC LOGO STATUS
    #define Set_DisplayLogoFlag()		   (System2Flags|=bDisplayLogoBit)
    #define Clr_DisplayLogoFlag()	   	   (System2Flags&=~bDisplayLogoBit)
    #define Set_FlashFlag() 			   (System2Flags|=bFlashBit)
    #define Clr_FlashFlag() 			   (System2Flags&=~bFlashBit)
    #define Set_ReadyForChangeFlag()	   (System2Flags|=bReadyForChangeBit)
    #define Clr_ReadyForChangeFlag()	   (System2Flags&=~bReadyForChangeBit)
    #define Set_LoadOSDDataFlag()		   (System2Flags|=bLoadOSDDataBit)
    #define Clr_LoadOSDDataFlag()		   (System2Flags&=~bLoadOSDDataBit)
    #define Set_LoadFontFlag()		   (System2Flags|=bLoadFontBit)
    #define Clr_LoadFontFlag()		   (System2Flags&=~bLoadFontBit)
    #define Set_AutoColorFlag() 	       (System2Flags|=bAutoColorBit)
    #define Clr_AutoColorFlag() 	       (System2Flags&=~bAutoColorBit)
    //#define Set_DisableLuminanceFlag()  (System2Flags|=bDisableLuminanceBit)
    //#define Clr_DisableLuminanceFlag()  (System2Flags&=~bDisableLuminanceBit)
    #define Set_DLKOverFlag()  (System2Flags|=bDLKOverBit)
    #define Clr_DLKOverFlag()  (System2Flags&=~bDLKOverBit)
    //#define Set_SpeedupIncBrighnessFlag()  (System2Flags|=bSpeedupIncBrighnessBit)
    //#define Clr_SpeedupIncBrighnessFlag()  (System2Flags&=~bSpeedupIncBrighnessBit)
    #define Set_ShowInputInfoFlag()  (System2Flags|=bShowInputInfoBit)
    #define Clr_ShowInputInfoFlag()  (System2Flags&=~bShowInputInfoBit)
    #define Set_JustShowValueFlag()  (System2Flags|=bJustShowValueBit)
    #define Clr_JustShowValueFlag()  (System2Flags&=~bJustShowValueBit)
    #define Set_DecVScaleFlag()		(System2Flags|=bDecVScaleBit)
    #define Clr_DecVScaleFlag()		(System2Flags&=~bDecVScaleBit)
    #define Set_PanelOnFlag()			(System2Flags|=bPanelOnBit)
    #define Clr_PanelOnFlag()			(System2Flags&=~bPanelOnBit)
    #define Set_PressExitFlag()   		(System2Flags|=bPressExitKeyBit)
    #define Clr_PressExitFlag()   		(System2Flags&=~bPressExitKeyBit)
    #define Set_EnableShowDDCFlag()   (System2Flags|=bEnableShowDDCBit)
    #define Clr_EnableShowDDCFlag()   (System2Flags&=~bEnableShowDDCBit)
    #define Set_PressMenuFlag()   	(System2Flags|=bPressMenuKeyBit)
    #define Clr_PressMenuFlag()   		(System2Flags&=~bPressMenuKeyBit)
    #define Set_MuteFlag()   			(System2Flags|=bMuteBit)
    #define Clr_MuteFlag()   			(System2Flags&=~bMuteBit)







    #if  ENABLE_HDCP
        //=======================================================================================//
        // for
        //=======================================================================================//
        #define bHpdReadyBit				BIT0
        #define bHdcpNVRamBit			BIT1
        #define bWriteHDCPcodeBit			BIT2
        #define bLoadHDCPKeyBit   			BIT3
        #define LoadHDCPKeyFlag   (HDCPFlag&bLoadHDCPKeyBit)
        #define Set_LoadHDCPKeyFlag()      (HDCPFlag|=bLoadHDCPKeyBit)
        #define Clr_LoadHDCPKeyFlag()      (HDCPFlag&=~bLoadHDCPKeyBit)
        #define HpdReadyFlag			(HDCPFlag&bHpdReadyBit)
        #define HdcpNVRamFlag			(HDCPFlag&bHdcpNVRamBit)
        #define Set_HpdReadyFlag()			(HDCPFlag|=bHpdReadyBit)
        #define Clr_HpdReadyFlag()			(HDCPFlag&=~bHpdReadyBit)
        #define Set_HdcpNVRamFlag()			(HDCPFlag|=bHdcpNVRamBit)
        #define Clr_HdcpNVRamFlag()			(HDCPFlag&=~bHdcpNVRamBit)
        #define WriteHDCPcodeFlag                  (HDCPFlag&bWriteHDCPcodeBit)
        #define Set_WriteHDCPcodeFlag()	      (HDCPFlag|=bWriteHDCPcodeBit)
        #define Clr_WriteHDCPcodeFlag()    	(HDCPFlag&=~bWriteHDCPcodeBit)
    #endif


    #if Backupdata
        #define bGetChecksumBit    BIT0
        #define bDDCCIAadjustBit   BIT1
        #define bDonotchangeBackBit  BIT2
        #define bACONBit     BIT3
        #define GetChecksumFlag     ( BackFlags&bGetChecksumBit)
        #define DDCCIAadjustFlag     ( BackFlags&bDDCCIAadjustBit)
        #define DonotchangeBackFlag     ( BackFlags&bDonotchangeBackBit)
        #define ACONFlag     ( BackFlags&bACONBit)
        #define Set_GetChecksumFlag()      (BackFlags|=bGetChecksumBit)
        #define Clr_GetChecksumFlag()      (BackFlags&=~bGetChecksumBit)
        #define Set_DDCCIAadjustFlag()      (BackFlags|=bDDCCIAadjustBit)
        #define Clr_DDCCIAadjustFlag()      (BackFlags&=~bDDCCIAadjustBit)
        #define Set_DonotchangeBackFlag()      (BackFlags|=bDonotchangeBackBit)
        #define Clr_DonotchangeBackFlag()      (BackFlags&=~bDonotchangeBackBit)
        #define Set_ACONFlag()      (BackFlags|=bACONBit)
        #define Clr_ACONFlag()      (BackFlags&=~bACONBit)
    #endif



    	#define bPressMainOSDBit	BIT0
        #define bPressSubOSDBit		BIT1
        #define bPressPowerBit		BIT2
		#define bPowerOffBit        BIT3
		#define bAutoConfigBit              BIT4
		#define bResetBit        	        BIT5
		#define bKeyCountBit                   BIT6

        #define MainOSDFlag	  ( KeypadFlags & bPressMainOSDBit)
		#define SubOSDFlag   ( KeypadFlags & bPressSubOSDBit)
		#define PowerKeyFlag    ( KeypadFlags & bPressPowerBit)
		#define PowerOffFlag    ( KeypadFlags & bPowerOffBit)
		#define DoAutoConfigFlag    ( KeypadFlags & bAutoConfigBit)
		#define DoResetFlag    ( KeypadFlags & bResetBit)
		#define DoKeyCountFlag    ( KeypadFlags & bKeyCountBit)

		#define Set_MainOSDFlag()      (KeypadFlags|=bPressMainOSDBit)
		#define Clr_MainOSDFlag()      (KeypadFlags&=~bPressMainOSDBit)
		#define Set_SubOSDFlag()      (KeypadFlags|=bPressSubOSDBit)
		#define Clr_SubOSDFlag()      (KeypadFlags&=~bPressSubOSDBit)
		#define Set_PowerKeyFlag()      (KeypadFlags|=bPressPowerBit)
		#define Clr_PowerKeyFlag()      (KeypadFlags&=~bPressPowerBit)
		#define Set_PowerOffFlag()      (KeypadFlags|=bPowerOffBit)
		#define Clr_PowerOffFlag()      (KeypadFlags&=~bPowerOffBit)
		#define Set_DoAutoConfigFlag()      (KeypadFlags|=bAutoConfigBit)
		#define Clr_DoAutoConfigFlag()      (KeypadFlags&=~bAutoConfigBit)
		#define Set_DoResetFlag()      (KeypadFlags|=bResetBit)
		#define Clr_DoResetFlag()      (KeypadFlags&=~bResetBit)
		#define Set_DoKeyCountFlag()      (KeypadFlags|=bKeyCountBit)
		#define Clr_DoKeyCountFlag()      (KeypadFlags&=~bKeyCountBit)

 #endif
    //=================================================================





#endif

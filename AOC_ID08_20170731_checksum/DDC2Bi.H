extern void DDC2Bi_Init(void);
extern void DDC2Bi_CommandHandler(void);

/*extern void DoDDCCI_A(void);
extern void DoDDCCI_D(void);*/
//==========COMMAND TYPE=======================================

#if HKC_ColorTemp_Adjust

  #define GET_VCP_FEATURE         0x01
#define SET_VCP_FEATURE         0x03
#define GET_TIMING_REQUEST      0x07
#define SAVE_CURRENT_SETTING    0x0C

#define GET_VCP_PAGE            0xC4
#define GET_PAGED_VCP           0xC6
#define SET_PAGED_VCP           0xC8
#define CAPABILITIES_REQUEST    0xF3
#define ENABLE_APP              0xF5
#define APP_TEST                0xB1

#define NEW_CONTROL             0x02
#define ACTIVE_CONTROL          0x52
#define VCP_APPLICATION_ENABLE  0xC6 
#define ALL_RESET               0x04
#define RESTORY_FACTORY         0x05    
#define GEOMETRY_RESET          0x06 
#define COLOR_RESET             0x08
#define COLOR_TEMP_INC          0x0B
#define COLOR_TEMP_REQUEST      0x0C
#define ADJ_CLOCK               0x0E
#define ADJ_BRIGHTNESS          0x10
#define ADJ_CONTRAST            0x12
#define SELECT_COLOR_PRESET     0x14
#define ADJ_RED_GAIN            0x16
#define ADJ_GREEN_GAIN          0x18
#define ADJ_BLUE_GAIN           0x1A
#define AUTO_SETUP              0x1E
#define AUTO_LEVEL              0x1F    //TPV
#define ADJ_H_POSITION          0x20
#define ADJ_V_POSITION          0x30
#define CONTRAST_AUTO           0x37
#define ADJ_OSD_HOR_POS         0x38
#define ADJ_OSD_VER_POS         0x39
#define ADJ_PHASE               0x3E
#define COLOR_TEMP              0x54
#define INPUT_SOURCE            0x60
#define ADJ_AUDIO_VOLUME        0x62
#define ADJ_LANGUAGE            0xCC // Sanger 0908 //0x68
#define AUDIO_MUTE              0x8D
//#define ADJ_DispayScaling       0x86  //<<NO USE
#define SYNC_TYPE               0xA8
#define H_FREQUENCY             0xAC
#define V_FREQUENCY             0xAE
#define FLAT_PANEL_TYPE         0xB2
#define DDCCI_SETTINGS          0xB0
#define RGB_ORDERING            0xB4
#define MONITOR_TYPE            0xB6
#define DISPLAY_USAGE_TIME      0xC0
#define DISPLAY_CONTROL_TYPE    0xC8
#define FW_LEVEL                0xC9
#define OSD_ENABLE              0xCA
#define POWER_MODE              0xD6
#define POWER_MODE_ON         0x01
#define POWER_MODE_STANDBY    0x02
#define POWER_MODE_SUSPEND    0x03
#define POWER_MODE_OFF        0x04
#define POWER_MODE_SWITCH_OFF 0x05
#define ADJ_CONTROL_LOCK        0xE3
#define HOURS_RUNNING           0xFA
#define ADJ_HOT_KEY             0xFB
#define ADJ_OSD_TURN_OFF        0xFC
#define DISPLAY_TIME            0xFF
//#define DefaultColorChange      0x3B  //<<NO USE    
#define ADJ_RED_BLACK_LEVEL     0x6C
#define ADJ_GREEN_BLACK_LEVEL   0x6E
#define ADJ_BLUE_BLACK_LEVEL    0x70
#define VCP_VERSION         0xDF //zg 1215
//#define TEST_SaveCurrentSetting 0xF4  //<<NO USE
//#define TEST_GETHTotal          0xF3  //<<NO USE
//#define TEST_GETVTotal          0xF2  //<<NO USE  
//#define TEST_GETSrcMode         0xF1  //<<NO USE
#define FACTORY_CMD             0xC0
#define MODE_CHANGE_REQUEST     0x11
//#define GetTimingMode           0x14  //<<NO USE
#define EEPROM_READ             0x09
#define EEPROM_WRITE            0x02

#if ENABLE_HDCP
#define _FLASH_HDCP    0xC2
#endif



#endif


#define GETVCPFeature			0x01
#define SETVCPFeature			0x03
#define GetTimingRequest			0x07
#define SaveCurrentSetting		0x0C
#define GETVCPPage				0xC4
#define GETPagedVCP			    	0xC6
#define SETPagedVCP			    	0xC8
#define CapabilitiesRequest	    	0xF3
#define EnableApp				0xF5
#define AppTest					0xB1
#define NewControl           			0x02
#define ALL_Reset			    	0x04
#define BriCon_Reset           		0x05
#define Geometry_Reset			0x06
#define Color_Reset				0x08
#define ColorTempInc      			0x0B
#define ColorTempRequest 		0x0C
#define ADJ_Clock			    	0x0E
#define ADJ_Brightness			0x10
#define ADJ_Contrast				0x12
#define Select_Color_Preset		0x14
#define ADJ_Red_Gain			0x16
#define ADJ_Green_Gain			0x18
#define ADJ_Blue_Gain			0x1A
#define Auto_Setup			    	0x1E
#define Auto_Color			       0x1F
#define ADJ_HorizontalPos		0x20
#define ADJ_VerticalPos			0x30
#define ADJ_Phase				0x3E
#define ActiveControl            		0x52
#define Color_Temp				0x54
#define Select_InputSource		0x60
#define ADJ_SpeakerVolume 		0x62
#define ADJ_Language			0xCC//0x68
#define ADJ_Red_Blacklevel        	0x6C
#define ADJ_Green_Blacklevel     	0x6E
#define ADJ_Blue_Blacklevel      	0x70
#define AudioMute				0x8D
#define SyncType					0xA8
#define DDCCI_Settings			0xB0
#define FlatPanellayout               	0xB2
#define RGBOrdering				0xB4
#define MonitorType				0xB6
#define H_Frequency           		0xAC
#define V_Frequency           		0xAE
#define DisplayUsageTime        	0xC0
#define APEnableKey				0xC6
#define DisplayControlType   		0xC8
#define FWLevel               			0xC9
#define OSDEnable             		0xCA
#define PowerMode				0xD6
#define VCPVersion                 		0xDF
#define ADJ_ControlLock			0xE3
#define HoursRunning				0xFA
#define ADJ_HotKey              		0xFB
#define ADJ_OSD_TurnOff        	 	0xFC
#define DisplayTime				0xFF
#define Contrast_Auto			0x37
#define ADJ_OSD_HorizontalPos	0x38
#define ADJ_OSD_VerticalPos		0x39
#define DefaultColorChange		0x3B
#define TEST_SaveCurrentSetting	0xF4
#define TEST_GETHTotal			0xF3
#define TEST_GETVTotal			0xF2
#define TEST_GETSrcMode		0xF1
//#define TEST_GETRegE1			0xE1
//#define TEST_GETHStart		0xF1
//#define TEST_GETVStart		0xF0
#define FactoryCommand			0xC0
#define ModeChangeRequest		0x11
#define GetTimingMode			0x14
#define EEPROMRead				0x09
#define EEPROMWrite				0x02
#define CommonDataEntry		0x05
#define TimingDataEntry			0x06
#define AssetRead				0x0B
#define AssetWrite				0x0E
#define WhiteBalanceRead		0x23
#define WhiteBalanceWrite		0x24
#define MPUReset					0x00


//Zhifeng.Wu Define For I-Menu
#define TPV_App_Command		0xF8
#define IMenu_Application		0xE1

#define ECO_Mode				0x11
#define Gamma					0x12
#define DCR						0x13

#define Full_Enhance				0x21
#define Nature_Skin				0x22
#define Sky_Blue					0x23
#define Green_Field				0x24
#define Auto_Detect				0x25
#define Demo					0x26


#define FrameSize				0x31
#define PB_Brightness			0x32
#define PB_Contrast				0x33
#define PB_Hue					0x34
#define PB_Saturation			0x35
#define PB_Hpos					0x36
#define PB_Vpos					0x37
#define PB_BrightFrame			0x38
#define PB_AutoPowerDown        0x41

#define GetUserTime                       0x7E//JRY COLOR TOOL
#define CleanUsedTime                   0x7D


//=========================================================
//#define ClockAdjRange		200
//#define AdjustClockRange		100
//#define MaxHStart			UserPrefAutoHStart+50
//#define MinHStart  			UserPrefAutoHStart-50
//#define MaxVStart			(2*StandardModeVStart)
//#define MinVStart	        		0
//#define MaxClock				(StandardModeHTotal+AdjustClockRange)  
//#define MinClock				(StandardModeHTotal-AdjustClockRange)  
#define MaxClockValue		(StandardModeHTotal+ClockAdjRange)
#define MinClockValue		(StandardModeHTotal-ClockAdjRange)
//#define MaxHStartValue	    (UserPrefHTotal-mStar_ReadWord(SPRHDC_H))
//#define MinHStartValue	    0x40
#define MaxHStartValue		(UserPrefAutoHStart+50)
#define MinHStartValue		(UserPrefAutoHStart-50)
//#define MaxVStartValue	    ((SrcVTotal-mStar_ReadWord(SPRVDC_H)))
//#define MinVStartValue	    (0x02)
//#define MaxVStartValue	    (2*StandardModeVStart)
//#define MinVStartValue	    (0x00)
#define MaxOsdHStartValue	100
#define MinOsdHStartValue	0
#define MaxOsdVStartValue	100
#define MinOsdVStartValue	0

//==============NewFactoryCommand=================

#define TPVFactoryCode		0xFE
#if NewFactoryCommand

#define SetOn				1
#define SetOff				0

//W & R================
#define TPV_AdjBrightness	0x10
#define TPV_AdjContrast		0x12
#define TPV_AdjRedGain		0x16
#define TPV_AdjGreenGain	0x18
#define TPV_AdjBlueGain		0x1A

#define TPV_SetDefLanguage			0xCC
	#define LanTChineseCode			0x01
	#define LanEnglistCode 			0x02
	#define LanFrenchCode			0x03
	#define LanGermanCode			0x04
	#define LanItalianCode			0x05
	#define LanJapaneseCode			0x06
	#define LanKoreanCode			0x07
	#define LanPortugueseCode_Portugal		0x08
	#define LanRussianCode			0x09
	#define LanSpanishCode			0x0A
	#define LanSwedishCode			0x0B
	#define LanTurkishCode			0x0C
	#define LanSChineseCode			0x0D
	#define LanPortugueseCode_Brazil               0x0E
	#define LanCzechCode			0x12
	#define LanPolishCode			0x1E
	#define LanNorskCode			0x1D
	#define LanFinnishCode			0x16
	#define LanDanskCode			0x13
	#define LanDutchCode			0x14
	

#define TPV_SelectInput				0x60
#define TPV_SelectGamma			0x72
#define TPV_HDCPReadWriteStart		0xEA

//Write Only=============
#define TPV_RestorFactory		0x04
#define TPV_AutoConfig			0x1E
#define TPV_AutoColor			0x1F
#define TPV_SaveOrRecallColTmp	0x14
	#define ColorTempSRGB		0x01
	#define ColorTempNative		0x02
	#define ColorTemp4000K		0x03
	#define ColorTemp5000K		0x04
	#define ColorTemp6500K		0x05
	#define ColorTemp7500K		0x06/*Or 7800K*/
	#define ColorTemp8200K		0x07/*Or 8500K*/
	#define ColorTemp9300K		0x08
	#define ColorTemp10000K		0x09
	#define ColorTemp11500K		0x0A
	#define ColorTempUser		0x0B
	#define SaveAllColorTemp		0x00


#define TPV_FactoryAutoAlignment 0xE1
	#define SetFactoryMode		0xA0
	#define NVMInit				0xA1
	#define SetBurnMode			0xA2
	#define ClearBacklitTime	0xA3
	#define SetProduceMode		0xA5
	#define LogoONOFF		        0xA7
	#define AdjPanelVCOM		0xA9		//121031 Modify
	#define HDCPReadWriteStart	0xE0

	

#endif 


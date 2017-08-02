#include "datatype.h"
#include "Board.h"
#include "Global.h"
#include "MDebug.h"
#include "Common.h"
#if 1//(LANGUAGE_TYPE==ASIA)||(LANGUAGE_TYPE==SIXTEEN)
#include "menudef.h"
#endif
#include "MsDLC.h"
#include "msACE.h"
//#include "Userpref.h"
#include "Menu.h"
#include "msEread.h"
#if ENABLE_DPS
#include "msDPS_Setting.h"
#endif

#include "Keypaddef.h"

#include "msEread.h"
#include "sysinfo2_MST9U.h"
#if PANEL_VBY1()
WORD u8Vby1CheckLockCounter = 0;
bit bVby1CheckLockFlag = 0;
#endif

bit bHDMI108oiTimingFlag=0; //20160309

#if DP_HPD_Test
BYTE TimingUnstableCNT = 0;
#endif

BYTE g_u8Test = 0;
#if ENABLE_DEMURA_FUNCTION
BYTE g_MEASURE_SIZE_W = 5;
BYTE g_MEASURE_SIZE_H = 3;
#endif

#if ENABLE_TIME_MEASUREMENT
WORD volatile g_u16TimeMeasurementCounter = 0;
bit bTimeMeasurementFlag=0;
WORD g_u16Time1;
WORD g_u16Time2;
#endif
#if (ZUI)
U16 g_u16ZUIDebounceTime = 0;
U16 g_u16DTotalTime = ZUI_DEBOUNCE_TIME;
#endif

#if (IR_REMOTE_SEL!=NO_REMOTE)
BYTE xdata IRkeyDelayCounter=0;
#endif

//==================================================
//EEPROM MAP
//==================================================
BYTE u8NVRAM_MAP[NVRAM_SIZE] __attribute__ ((aligned (16)));
BYTE u8NVRAM_MAP_BK[NVRAM_SIZE] __attribute__ ((aligned (16)));
Bool g_bEEP_Dirty       = FALSE;
WORD g_u16EEP_SaveAddr  = 0xFFFF;
WORD g_u16EEP_SaveCnt   = 0xFFFF;
Bool g_BAutoSave        = FALSE;
Bool g_bBLTimeSaveFlag  = FALSE;

//==================================================
//Scaler Mask Settings
//==================================================
WORD g_wOrgScMaskOut = 0;
Bool g_bZonalEnable = FALSE;
BYTE g_u8ZonalColorMode = 0;

#ifdef LoadDefaultEDIDTo24C02
BYTE I2C_TYPE=S_EDID_Digital1;
#endif

//==================================================
//Window Settings
//==================================================
BYTE g_u8WinIndex = 0;

//MAILBOX
BYTE *u8MBX = (BYTE *)MBX_XD2MIU_ADR; // size = 0x1000 for MBX
#if ENABLE_MBX_SAR
BYTE u8MailButton = 0;
#endif

//#if ENABLE_DPS
BOOL idata bRunToolFlag = FALSE;
//#endif
BYTE g_ucTimer0_TH0;
BYTE g_ucTimer0_TL0;
BYTE xdata g_u8SystemSpeedMode=0;

volatile bit g_bMcuPMClock = 0;
bit ms10Flag = 0;
bit ms20Flag = 0;
bit ms50Flag = 0;
bit ms500Flag = 0; // 120815 coding, set to bit data type for isr usage

#if ENABLE_DP_INPUT
bit g_bDoDPInit;
#endif
bit g_bAPPWindow_Init=0;

#if ENABLE_MMD_DCR
BYTE xdata DCRBacklight=0;
BYTE xdata DCRDarkBacklight=0;
BYTE xdata DCRMinBacklight=0;
BYTE xdata DCRMaxBacklight=0;
BYTE xdata  DCR_TargetValue=0;

WORD xdata HighContCounter=0;
BYTE xdata BacklightOffCounter=0;
WORD xdata DCRWaitBLReadyCounter=0;
BYTE xdata DCRWaitBLOffCounter=0;
BYTE xdata DCR_Status=DCR_IDLE;
#endif

WORD xdata OrigAdjPipHpos=0;
WORD xdata OrigAdjPipVpos=0;

WORD xdata InputWidth=0;
WORD xdata InputHeight=0;
WORD xdata InputVfreq=0;

BYTE xdata LEDModeCounter=0;

#if 0//LED_HOT_Menu_Func
BYTE xdata UserprefLEDColorful=DEF_LED_MODE;
BYTE xdata UserprefLedStrength=DEF_LED_STRENGTH;
BYTE xdata UserprefLedModeType=DEF_LED_MODE_TYPE;
#endif

#if 1//SleepMode_Enable
WORD xdata SleepModeCounter=0;
#endif

#if SET_BLIT_WITH_OSDFreeSyncSwitchAction  //  160729_03
bit bOSDFreeSyncOnOffFlag=FALSE;
#endif
#if ENABLE_TIMINGCHANGE_CLRBL_ONLY
bit bOnlyClrBL=0;
#elif ENABLE_TIMINGCHANGE_TURNOFFPANEL
bit bTurnOffPanelWhenTimingChange=FALSE;
#endif
#if LOGOTimeOutDelayForVBY1PanelLock  //  160729_02
WORD xdata g_u8OsdCountOffset=0;
#endif

bit msFreeSyncTimerFlag = 0;
Bool g_NotSettingLPLL=TRUE;  //  160714_11

#if ENABLE_HDCP
bit LoadHDCPKeyFlag;
BYTE xdata Flash_HDCPcont=0;
BYTE xdata DDC1[289];//289
#endif


#if New_Cross_Enable
BYTE xdata  UserPrefCrossType=Cross_Large_Y;
#elif Enable_Corss_FY
BYTE xdata  UserPrefCrossType=Cross_Color_Y;
#elif Enable_Corss_LHCX
BYTE xdata  UserPrefCrossType=Cross_Mode1_G;
#elif Enable_Corss_RunFeng
BYTE xdata  UserPrefCrossType=Cross_Mode1;
#endif

BYTE xdata UserPrefBrightness_Temp=0;
BYTE xdata UserPrefContrast_Temp=0;
BYTE xdata UserPrefNoiseReduction=0;

#if Audio_Source_Enable
BYTE xdata gAudioSourceSelectableIndex = 0;
#endif

#if Enable_Corss_LHCX_New
BYTE xdata GameModeTime_S=0;
BYTE xdata GameModeTime_M=0;
BYTE xdata GameModeTime_Position=HotKeyGameClokPosition_Close;
#endif

bit SecondFlag = 0;
BYTE xdata g_u8Second = 0;
WORD xdata g_u16OsdCounter = 0;
BYTE xdata g_u8SwitchPortCounter;
BYTE xdata g_u8PowerDownCounter = 0;
volatile bit bPanelOffOnFlag = 0;
bit bPanelOnFlag = 0; // 100811 coding addition to indicate panel power status
BYTE xdata TPDebunceCounter = 0;

WORD xdata SystemFlags = 0; // for system status flags
WORD xdata SystemFlags2 = 0; // for system status flags
WORD xdata SystemFlags3 = 0; // for system status flags
WORD xdata SystemFlags4 = 0; // for system status flags

BYTE xdata PropFontIndex = 0;

BYTE xdata g_u8SyncLossOsdCounter = 0;

BYTE xdata ePMWakeupStatus=0;

BYTE xdata g_u8DDCCIAControl;

eBOOT_STATUS_TYPE g_eBootStatus = eBOOT_STATUS_NONE;

bit g_bInputSOGFlag = 0;
BYTE xdata g_u8HoldKeyCounter=0;

volatile WORD ms_Counter = SystemTick; // for System timer

volatile WORD PanelOffOnCounter = 0;

#if (VBY1_PowerSequence)
WORD xdata PowerOnBackLightCounter=0;
#endif
BYTE xdata VKeyPad=VK_NOTHING;

xdata BYTE KeypadButton = 0;

BYTE xdata g_u8PositionNo=0;
BYTE xdata g_u8PatternNo = 0;
BYTE xdata g_u8SaveUserData=0;
#if PANEL_VBY1()
BYTE xdata g_u8VBY1CheckFlag=0;
#endif
#if ENABLE_DAC
#if EnableDACDePop
volatile WORD u16AudioDePopCounter = 0;
bit bDAC_DePopCountFlag = 0;
volatile bit bAudioDePopFlag = 0;
#if (CHIP_ID == MST9U3) || (CHIP_ID == MST9U4)
volatile bit bAudioWakeUpDePopFlag = 0;
#endif
#endif
volatile BYTE xdata audio_EnableAudioAfterSignalLock_Flag = AUDIO_MUTE;
#endif

BYTE xdata g_u8SaveIndex = 0; // saveIndex

volatile WORD DelayCounter = 0;
volatile bit bDelayFlag = 0;
volatile WORD xdata TimeOutCounter = 0; // 120510 coding, addition for checking register status

#if ENABLE_DAISY_CHAIN
BYTE xdata g_u8DCMSTFlag = 0; // 1:fake sleep
BYTE xdata g_u8DCMSTCheckFlag = 0;
#endif

BYTE xdata DDCCIFuncCounter=0;
BYTE xdata DDCCIFuncCommand=0;

xdata WORD CFontData[18];
WORD xdata font_shift;
WORD xdata font_offset;
WORD xdata TotalFontWidth;
BYTE xdata PropFontRamWidth;
BYTE xdata FontUsedPixel;
BYTE xdata font_Norm_Up_Down; // 0: norm  , 1: half of up, 2:half of down.

#if USB_Updata_FW_Enable
BYTE xdata USBUpdataFwStatus=USB_Idle;
#endif

WORD xdata BlackScreenCounter=0;

BYTE xdata OSDActiveCounter=0;


BYTE xdata LanguageIndex;
BYTE xdata SecondTblAddr;
extern BYTE g_bPQ_Adaptive_On[DISPLAY_MAX_NUMS];
DWORD XDATA g_u16Total_Weight[DISPLAY_MAX_NUMS], g_u16Total_Count[DISPLAY_MAX_NUMS];
BYTE XDATA g_u8Ymax[DISPLAY_MAX_NUMS], g_u8Ymin[DISPLAY_MAX_NUMS];
#if ENABLE_DPS
xdata WORD  u16DPSCurYgain = 0x400;
xdata BYTE  u8DPSCurBacklight = 0xFF;
#endif
//BYTE xdata u8BacklightNow = 0;
//BYTE xdata u8BacklightPrev = 0;
BYTE g_u8NoUse;
#if 0//ENABLE_TOUCH_KEY
#if TOUCH_KEY_POWER_KEY_DEBOUNCE
bit bPowerKeyPressed = 0;
BYTE xdata PowerKeyDebounceCntr = 0;
#endif
BYTE xdata TouchKeyRestortCounter; // unit: 50mS
    #if ENABLE_LOW_CONTACT
    BYTE xdata TouchKeyLowContactDelay; // unit: 50mS
    BYTE xdata TouchKeyLowContactDetectCntr; // unit: 1S, use to detect low contact peroid
    #endif
    #if 0//TOUCH_KEY_CTRL_LED
    BYTE xdata TouchKeySetLEDStatus;
    #endif
#endif
#if ENABLE_MENULOAD
BYTE u8MenuLoadFlag = 0;
#endif

#if ENABLE_SW_SCALER_COMMON_WRITE
WORD g_u16CommonWriteSCMask = 0;
#endif
//PanelSettingType xdata PanelSetting;
//FactorySettingType xdata    stFactorySetting;
//MonitorSettingType xdata    stMonitorSetting;
/////////////////////////////////////////////////////////////////////////////////////////////
//BYTE xdata DDCWPActiveFlag; //cc 2011.09.09 15:50
//bit TI0_FLAG; // 120815 coding, set to bit data type for isr usage
//BYTE g_u8Second=0;
//BYTE xdata HotKeyCounter = 0;
//DWORD xdata BlacklitTime = 0;
//bit SaveBlacklitTimeFlag = 0;
//BYTE LanguageState=0;
//BYTE CurrentLanguage=0;   //wumaozhong add 20050615
//BYTE ShortcutLuminance=0;
//BYTE xdata ChipVer = 0;
//BYTE xdata BrightnessTemp = 0;
//BYTE xdata PrevHistoValue = 0;
//bit SaveFactorySettingFlag = 0;
//bit g_bServiceEDIDUnLock = 0;
//xdata BYTE xfr_regs[256] _at_ 0xC000;
//xdata BYTE g_FlashID = 0;
//bit PowerGoodExistFlag = 0;
//bit g_bBacklightOn = 0;
//BYTE code UserDataCode[];// =//Set UserDataCode To Bank3 Addr:A000

#if (ENABLE_CHECK_RANGE_COLORIMETRY_CHANGE)
ST_INPUT_PARAM g_InputPort[Input_Nums] =
{
#if (INPUT_TYPE&INPUT_1A)
    {PORT_FLOW_0, PORT_TYPE_0, IP_STATUS_IDLE, SC_MAX_NUMS, 0, 0, 0, 0, 0, bInputTimingChangeBit, {SyncLoss, 0, 0, 0, 0xFF, 0, 0}, {0, 0, 0, 0}, INPUTCOLOR_RGB, INPUT_RANGE_FULL, INPUT_COLORIMETRY_ITU709, 0, 0, {0, 0, 0}},
#endif
#if (INPUT_TYPE >= INPUT_1C)
    {PORT_FLOW_1, PORT_TYPE_1, IP_STATUS_IDLE, SC_MAX_NUMS, 0, 0, 0, 0, 0, bInputTimingChangeBit, {SyncLoss, 0, 0, 0, 0xFF, 0, 0}, {0, 0, 0, 0}, INPUTCOLOR_RGB, INPUT_RANGE_FULL, INPUT_COLORIMETRY_ITU709, 0, 0, {0, 0, 0}},
#endif
#if (INPUT_TYPE >= INPUT_2C)
    {PORT_FLOW_2, PORT_TYPE_2, IP_STATUS_IDLE, SC_MAX_NUMS, 0, 0, 0, 0, 0, bInputTimingChangeBit, {SyncLoss, 0, 0, 0, 0xFF, 0, 0}, {0, 0, 0, 0}, INPUTCOLOR_RGB, INPUT_RANGE_FULL, INPUT_COLORIMETRY_ITU709, 0, 0, {0, 0, 0}},
#endif
#if (INPUT_TYPE >= INPUT_3C)
    {PORT_FLOW_3, PORT_TYPE_3, IP_STATUS_IDLE, SC_MAX_NUMS, 0, 0, 0, 0, 0, bInputTimingChangeBit, {SyncLoss, 0, 0, 0, 0xFF, 0, 0}, {0, 0, 0, 0}, INPUTCOLOR_RGB, INPUT_RANGE_FULL, INPUT_COLORIMETRY_ITU709, 0, 0, {0, 0, 0}},
#endif
#if (INPUT_TYPE >= INPUT_4C)
    {PORT_FLOW_4, PORT_TYPE_4, IP_STATUS_IDLE, SC_MAX_NUMS, 0, 0, 0, 0, 0, bInputTimingChangeBit, {SyncLoss, 0, 0, 0, 0xFF, 0, 0}, {0, 0, 0, 0}, INPUTCOLOR_RGB, INPUT_RANGE_FULL, INPUT_COLORIMETRY_ITU709, 0, 0, {0, 0, 0}},
#endif
#if (INPUT_TYPE >= INPUT_5C)
    {PORT_FLOW_5, PORT_TYPE_5, IP_STATUS_IDLE, SC_MAX_NUMS, 0, 0, 0, 0, 0, bInputTimingChangeBit, {SyncLoss, 0, 0, 0, 0xFF, 0, 0}, {0, 0, 0, 0}, INPUTCOLOR_RGB, INPUT_RANGE_FULL, INPUT_COLORIMETRY_ITU709, 0, 0, {0, 0, 0}},
#endif
#if (INPUT_TYPE >= INPUT_6C)
    {PORT_FLOW_6, PORT_TYPE_6, IP_STATUS_IDLE, SC_MAX_NUMS, 0, 0, 0, 0, 0, bInputTimingChangeBit, {SyncLoss, 0, 0, 0, 0xFF, 0, 0}, {0, 0, 0, 0}, INPUTCOLOR_RGB, INPUT_RANGE_FULL, INPUT_COLORIMETRY_ITU709, 0, 0, {0, 0, 0}},
#endif
#if (INPUT_TYPE >= INPUT_7C)
    {PORT_FLOW_7, PORT_TYPE_7, IP_STATUS_IDLE, SC_MAX_NUMS, 0, 0, 0, 0, 0, bInputTimingChangeBit, {SyncLoss, 0, 0, 0, 0xFF, 0, 0}, {0, 0, 0, 0}, INPUTCOLOR_RGB, INPUT_RANGE_FULL, INPUT_COLORIMETRY_ITU709, 0, 0, {0, 0, 0}},
#endif
#if (INPUT_TYPE >= INPUT_8C)
    {PORT_FLOW_8, PORT_TYPE_8, IP_STATUS_IDLE, SC_MAX_NUMS, 0, 0, 0, 0, 0, bInputTimingChangeBit, {SyncLoss, 0, 0, 0, 0xFF, 0, 0}, {0, 0, 0, 0}, INPUTCOLOR_RGB, INPUT_RANGE_FULL, INPUT_COLORIMETRY_ITU709, 0, 0, {0, 0, 0}},
#endif

};
#else
ST_INPUT_PARAM g_InputPort[Input_Nums] =
{
#if (INPUT_TYPE&INPUT_1A)
    {PORT_FLOW_0, PORT_TYPE_0, IP_STATUS_IDLE, SC_MAX_NUMS, 0, 0, 0, 0, 0, bInputTimingChangeBit, {SyncLoss, 0, 0, 0, 0xFF, 0, 0}, {0, 0, 0, 0}, INPUTCOLOR_RGB, 0, 0, {0, 0, 0}},
#endif
#if (INPUT_TYPE >= INPUT_1C)
    {PORT_FLOW_1, PORT_TYPE_1, IP_STATUS_IDLE, SC_MAX_NUMS, 0, 0, 0, 0, 0, bInputTimingChangeBit, {SyncLoss, 0, 0, 0, 0xFF, 0, 0}, {0, 0, 0, 0}, INPUTCOLOR_RGB, 0, 0, {0, 0, 0}},
#endif
#if (INPUT_TYPE >= INPUT_2C)
    {PORT_FLOW_2, PORT_TYPE_2, IP_STATUS_IDLE, SC_MAX_NUMS, 0, 0, 0, 0, 0, bInputTimingChangeBit, {SyncLoss, 0, 0, 0, 0xFF, 0, 0}, {0, 0, 0, 0}, INPUTCOLOR_RGB, 0, 0, {0, 0, 0}},
#endif
#if (INPUT_TYPE >= INPUT_3C)
    {PORT_FLOW_3, PORT_TYPE_3, IP_STATUS_IDLE, SC_MAX_NUMS, 0, 0, 0, 0, 0, bInputTimingChangeBit, {SyncLoss, 0, 0, 0, 0xFF, 0, 0}, {0, 0, 0, 0}, INPUTCOLOR_RGB, 0, 0, {0, 0, 0}},
#endif
#if (INPUT_TYPE >= INPUT_4C)
    {PORT_FLOW_4, PORT_TYPE_4, IP_STATUS_IDLE, SC_MAX_NUMS, 0, 0, 0, 0, 0, bInputTimingChangeBit, {SyncLoss, 0, 0, 0, 0xFF, 0, 0}, {0, 0, 0, 0}, INPUTCOLOR_RGB, 0, 0, {0, 0, 0}},
#endif
#if (INPUT_TYPE >= INPUT_5C)
    {PORT_FLOW_5, PORT_TYPE_5, IP_STATUS_IDLE, SC_MAX_NUMS, 0, 0, 0, 0, 0, bInputTimingChangeBit, {SyncLoss, 0, 0, 0, 0xFF, 0, 0}, {0, 0, 0, 0}, INPUTCOLOR_RGB, 0, 0, {0, 0, 0}},
#endif
#if (INPUT_TYPE >= INPUT_6C)
    {PORT_FLOW_6, PORT_TYPE_6, IP_STATUS_IDLE, SC_MAX_NUMS, 0, 0, 0, 0, 0, bInputTimingChangeBit, {SyncLoss, 0, 0, 0, 0xFF, 0, 0}, {0, 0, 0, 0}, INPUTCOLOR_RGB, 0, 0, {0, 0, 0}},
#endif
#if (INPUT_TYPE >= INPUT_7C)
    {PORT_FLOW_7, PORT_TYPE_7, IP_STATUS_IDLE, SC_MAX_NUMS, 0, 0, 0, 0, 0, bInputTimingChangeBit, {SyncLoss, 0, 0, 0, 0xFF, 0, 0}, {0, 0, 0, 0}, INPUTCOLOR_RGB, 0, 0, {0, 0, 0}},
#endif
#if (INPUT_TYPE >= INPUT_8C)
    {PORT_FLOW_8, PORT_TYPE_8, IP_STATUS_IDLE, SC_MAX_NUMS, 0, 0, 0, 0, 0, bInputTimingChangeBit, {SyncLoss, 0, 0, 0, 0xFF, 0, 0}, {0, 0, 0, 0}, INPUTCOLOR_RGB, 0, 0, {0, 0, 0}},
#endif
};
#endif

extern void UserDataSpaceDummy(void);

void DummyCall(void)
{
    BYTE i = 0;

    if(i)
    {
        i = (BYTE)GetStandardModeVTotal(SrcInputPortM/*//tmp*/);
        UserDataSpaceDummy();

        GetPow2(0);
        GetLog(0);
        GetLsbBitPos(0);

    #ifdef MST9U_FPGA
        {
        BYTE bHDCPKeyInEfuse,ucValue,u8ADCBandgapTrimVal;
        bHDCPKeyInEfuse = msEread_IsHDCPKeyInEfuse();
        ucValue = msEread_GetHDCPKeyFromEfuse(0);
        msEread_CompareHDCPChecksumInEfuse(0);
        u8ADCBandgapTrimVal = msEread_GetADCBandgapTrimValFromEfuse(EFUSE_0);
        msEread_SetComboInitialValue();
        msEread_SetHDMIInitialValue();
        msEread_SetMHLInitialValue();
        }
    #endif
    }

}

void Init_GlobalVariables(void)
{
    BYTE i;
/////////////////////////////////////////////////
// add for prevent complier warning
    DummyCall();
//////////////////////////////////////////////////

    //g_u8SwitchInputPortCount=0;
    g_u8Second = 0;
    g_u16OsdCounter = 0;
    g_u8SwitchPortCounter = 0;
    g_u8PowerDownCounter = 0;

    bPanelOffOnFlag = 0;
    bPanelOnFlag = 0;

    TPDebunceCounter = SKPollingInterval;

#if 0//USEFLASH
    BlacklitTime = UserprefBacklighttime;
#endif

#if 0//ENABLE_TOUCH_KEY
    TouchKeyRestortCounter = 0;
    #if ENABLE_LOW_CONTACT
    TouchKeyLowContactDetectCntr = TOUCH_KEY_LOW_CONTACT_DETECT_PEROID;
    TouchKeyLowContactDelay = 0;
    #endif
    #if 0//TOUCH_KEY_CTRL_LED
    TouchKeySetLEDStatus = TouchKeyLEDStatus_OFF;
    #endif
#endif
    #if LOGOTimeOutDelayForVBY1PanelLock //  160729_02
     g_u8OsdCountOffset=0;
    #endif
    #if ENABLE_TIMINGCHANGE_CLRBL_ONLY
    bOnlyClrBL=FALSE;
    #elif ENABLE_TIMINGCHANGE_TURNOFFPANEL
    bTurnOffPanelWhenTimingChange=FALSE;
    #endif

    #if SET_BLIT_WITH_OSDFreeSyncSwitchAction
     bOSDFreeSyncOnOffFlag=FALSE;
    #endif
    g_NotSettingLPLL=TRUE; //  160714_11

    if(POWER_ON_FLAG)
    {
        //moved to g_ScalerInfo SystemFlags|=bInputTimingChangeBit;
    }
    else
    {
        SystemFlags|=bForcePowerSavingBit;
    }

     LEDModeCounter=0;

#if 1//ENABLE_AUTO_SELECT_SOURCE
    //USER_PREF_INPUT_PRIORITY=INPUT_PRIORITY_AUTO;
#endif

#if ENABLE_DCR //init with USER_PREF_BRIGHTNESS
    u8BacklightNow   = USER_PREF_BRIGHTNESS;
    u8BacklightPrev = USER_PREF_BRIGHTNESS;
#endif

#if ENABLE_DPS
  #if DPS_UserBrightControl_EN
    REG_DPS_UserBrightContlGain = USER_PREF_BRIGHTNESS;//(float)USER_PREF_BRIGHTNESS * 256 / 100;
  #else
    REG_DPS_UserBrightContlGain = 100;
  #endif
#endif
    for(i = 0 ; i < DISPLAY_MAX_NUMS ; i++)
    {
        g_u16Total_Weight[i] = 0;
        g_u16Total_Count[i] = 0;
        g_u8Ymax[i] = 0;
        g_u8Ymin[i] = 0xFF;
        g_bPQ_Adaptive_On[i] = 0;
    }
	
#if ENABLE_MMD_DCR
	DCR_Status=DCR_IDLE;
	DCRMinBacklight = BRIGHTNESS_DCR_MIN;
	DCRMaxBacklight = BRIGHTNESS_DCR_MAX;
	DCRDarkBacklight = BRIGHTNESS_DCR_MIN;//MIN_DCR_BRI_PWM_VALUE;
	DCRBacklight = ( ( WORD ) (USER_PREF_BRIGHTNESS)* (DCRMaxBacklight-DCRMinBacklight) ) / 100 + DCRMinBacklight;	
#endif

	UserPrefBrightness_Temp = USER_PREF_BRIGHTNESS;
	UserPrefContrast_Temp = CURRENT_CONTRAST;
  	
  #if 0//LED_HOT_Menu_Func
      UserprefLEDColorful=USER_PREF_LEDColorful_MODE;
      UserprefLedStrength=USER_PREF_LedStrength;
      UserprefLedModeType=USER_PREF_LedModeType;
  #endif

     #if ENABLE_MULTI_INPUT
      g_stMenuFlag.bDisplayInputMsg[USER_PREF_WIN_SEL]=1;
      g_stMenuFlag.bDisplayInputNoSignalMsg[USER_PREF_WIN_SEL]=1;
    #endif

    DDCCIFuncCounter=0;
    DDCCIFuncCommand=0;
    BlackScreenCounter=0;	
	
    Clr_VBY1TrainingPassFlag();	

#ifdef PanelI2CCommandSetting
    Clr_VBY1_Check_LockN_Flag();
#endif

    Clr_BackLightOnFlag();
    Clr_DDCCIActiveFlag();
    Clr_HotCrossMenuFlag();
    Clr_OSDFreeSyncActionFlag();
	
    Clr_HotGameTimeUpdateFlag();

#if ((Free_Sync_Enable)&&(ENABLE_MULTI_WINDOW_SETTINGS_MENU))
	if((USER_PREF_MULTI_WIN_MODE != eMULTIWIN_OFF))
	{
		Clr_FreeSyncFlag();
	}
#endif

#if Disable_4K60HZ_Timing
     if(USER_PREF_MULTI_WIN_MODE!=eMULTIWIN_OFF)
    	{
	    Clr_HDMI_EDID_1920x1080_Flag();	   
	    Set_HDMI_EDID_1920x2160_Flag();	  	
	    Clr_HDMI_EDID_3840x2160_Flag();	
    	}
     else
     	{
	 Set_HDMI_EDID_3840x2160_Flag();		
	 Clr_HDMI_EDID_1920x1080_Flag();	  
	 Clr_HDMI_EDID_1920x2160_Flag();	  
     	}
#else
#if DVI_PBP_RELOAD_EDID
     if((USER_PREF_MULTI_WIN_MODE>=eMULTIWIN_2_POP1)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_DVI)))
     	{

	if((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_4)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_DVI)))
	{
	   Set_DVI_EDID_1920x1080_Flag();	  
	   Clr_DVI_EDID_1920x2160_Flag();	  
	}
	else if((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE!=INPUT_DVI)&&(USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_DVI||USER_PREF_INPUT_TYPE_SUBWIN2==INPUT_DVI))
	{
	   Set_DVI_EDID_1920x1080_Flag();	  
	   Clr_DVI_EDID_1920x2160_Flag();	  
	}
	else
	{
	    Clr_DVI_EDID_1920x1080_Flag();	   
	    Set_DVI_EDID_1920x2160_Flag();	  	
	}
		
	 Clr_DVI_EDID_3840x2160_Flag();	
     	}
	else
     	{
	 Set_DVI_EDID_3840x2160_Flag();		
	 Clr_DVI_EDID_1920x1080_Flag();	  
	 Clr_DVI_EDID_1920x2160_Flag();	  
     	}
#endif
#if HDMI_PBP_RELOAD_EDID
     if((USER_PREF_MULTI_WIN_MODE>=eMULTIWIN_2_POP1)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_HDMI)))
     	{

	if((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_4)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_HDMI)))
	{
	   Set_HDMI_EDID_1920x1080_Flag();	  
	   Clr_HDMI_EDID_1920x2160_Flag();	  
	}
	else if((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE!=INPUT_HDMI)&&(USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_HDMI||USER_PREF_INPUT_TYPE_SUBWIN2==INPUT_HDMI))
	{
	   Set_HDMI_EDID_1920x1080_Flag();	  
	   Clr_HDMI_EDID_1920x2160_Flag();	  
	}
	else
	{
	    Clr_HDMI_EDID_1920x1080_Flag();	   
	    Set_HDMI_EDID_1920x2160_Flag();	  	
	}
		
	 Clr_HDMI_EDID_3840x2160_Flag();	
     	}
	else
     	{
	 Set_HDMI_EDID_3840x2160_Flag();		
	 Clr_HDMI_EDID_1920x1080_Flag();	  
	 Clr_HDMI_EDID_1920x2160_Flag();	  
     	}
#endif		
	 
#endif


#if 1//SleepMode_Enable
   if(USER_PREF_SLEEPMODE==SleepMode_30Min)
	SleepModeCounter=30*60;
   else if(USER_PREF_SLEEPMODE==SleepMode_60Min)
	SleepModeCounter=60*60;
   else if(USER_PREF_SLEEPMODE==SleepMode_120Min)
	SleepModeCounter=120*60;
   else
	SleepModeCounter=0;
     Clr_ForceEnterPowerOffFlag();
     Clr_ForceEnterPowerOffMsgFlag();	   
#endif	

           if(hw_HP_DET_Pin)
           {
                CLR_EARPHONE_EN_FLAG();
           }  
	    else
                SET_EARPHONE_EN_FLAG();

   if(FACTORY_MODE_FLAG||BURNIN_MODE_FLAG)		
        hw_ClrDDC_WP();
  else
        hw_SetDDC_WP();


   if(FACTORY_MODE_FLAG||BURNIN_MODE_FLAG)		
   {
      #if LOGO_ENABLE			
      #if (!ZUI)
      g_stMenuFlag.bDisplayLogo=0;
      #else
      g_stZuiMenuFlag.bDisplayLogo=0;
      #endif
      #endif	
      g_stMenuFlag.bShowInputIcon=0;	  
   }
   else if((POWER_ON_FLAG&&OSD_LOGO_FLAG)&&(ePMWakeupStatus == ePMSTS_POWERGPIO_ACT||ePMWakeupStatus == ePMSTS_NON
     ||ePMWakeupStatus == ePMSTS_SAR_ACT
   ))
   {
      #if LOGO_ENABLE			
      #if (!ZUI)
      g_stMenuFlag.bDisplayLogo=1;
      #else
      g_stZuiMenuFlag.bDisplayLogo=1;
      #endif
      #endif	
       g_stMenuFlag.bShowInputIcon=0;	  
  }
   else if((POWER_ON_FLAG)&&(ePMWakeupStatus==ePMSTS_KEYWAKEUP_ACT))
   {
      //g_stMenuFlag.bShowInputIcon=1;
      g_stMenuFlag.bShowInputIcon=0;
   }
   
}
	

BYTE GetPow2(BYTE expvalue)
{
    if( expvalue == 0 )
        return 1;
    else
        return (0x02<<(expvalue-1));
}

BYTE GetLog(DWORD value)
{
    BYTE i = 0;

    if( value == 0 )
        return 0;

    while(value)
    {
        value >>= 1;
        i++;
    }

    return (i-1);
}

BYTE GetLsbBitPos(DWORD value) // 0:0, 0:BIT0, 1:BIT1, 2:BIT2, ...
{
    BYTE i = 0;

    if( value == 0 )
        return 0; // care it's same as BIT0

    while((value&BIT0)==0)
    {
        value >>= 1;
        i++;
    }

    return i;
}
BYTE TransNumToBit(BYTE value)
{
    BYTE i=1;
    if(value == 0)
        return 1;

    while(value)
    {
        i <<= 1;
        value--;
    }
    return i;
}

Bool  INPUT_IS_DP_DRR(BYTE InputPort)
{
    Bool ucflag= FALSE;

    if(CURRENT_INPUT_IS_DISPLAYPORT())
    {
        if(!msAPI_combo_IPGetCableDetect((EN_COMBO_IP_SELECT)g_InputPort[InputPort].eIPMux)  && !msAPI_combo_IPCheckSourceAlive((EN_COMBO_IP_SELECT)g_InputPort[InputPort].eIPMux))
        {
            ucflag =FALSE;
        }
        else if(msAPI_combo_IPGetDPDRRFlag((EN_COMBO_IP_SELECT)g_InputPort[InputPort].eIPMux))
        {
            ucflag =TRUE;
        }
    }
    return ucflag;
}



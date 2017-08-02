#define _MSTAR_C_
#include <math.h>
#include "datatype.h"
#include "Board.h"
#include "ms_reg.h"
#include "Global.h"
#include "Mcu.h"
//#include "Panel.h"
#include "Adjust.h"
//#include "msADC.h"
#include "Ms_rwreg.h"
#include "MDebug.h"
#include "Common.h"
#include "misc.h"
#include "DDC.h"
#include "Power.h"
#include "Detect.h"
#include "msOSD.h"
//#include "extlib.h"
#include "Menu.h"
#include "menudef.h"
#include "menufunc.h"
#include "msflash.h"
//#include "msid_v1.h"
#include "drvGPIO.h"
//#include "drv_adctbl.h"
#include "drvADC.h"
#if ENABLE_RTE
#include "msOVD.h"
#endif

#include "Ms_PM.h"
#include "MsDLC.h"
#include "msACE.h"
//#include "Userpref.h"
#include "Panel.h"
#if ENABLE_DAC
#include "drvAudio.h"
#endif
#if ENABLE_3DLUT
#include "drv3DLUT.h"
#endif
#if ENABLE_MENULOAD
#include "apiXC_SC_MenuLoad.h"
#include "sysinfo.h"
#endif
#include "AutoFunc.h"
///////////////////////////////////////////
//#include "drvMcu.h"
#include "drvmStar.h"
//#include "appmStar.h"
//#include "halRwreg.h"
//////////////////////////////////////////
#if ENABLE_DP_INPUT
#include "drvDPRxApp.h"
#endif

#if ENABLE_OSDC_OSDD
#include "drvOSDblend.h"
#endif
#if ENABLE_DELTAE_FUNCTION
#include "drvDeltaE.h"
#endif
#if ENABLE_DEMURA_FUNCTION
#include "drvDemura.h"
#endif
#if ENABLE_SUPER_RESOLUTION
#include "msPQ.h"
#endif
// INTERNAL
#include "mStar.h"

#define MSTAR_DEBUG    1
#if ENABLE_MSTV_UART_DEBUG && MSTAR_DEBUG
#define MSTAR_PRINT(format, ...)     printf(format, ##__VA_ARGS__)
#else
#define MSTAR_PRINT(format, ...)
#endif

//*******************************************************************
//
// Constant Definition
//
//*******************************************************************
#define ADC2Sets    0//0// 1 for chip with 2 ADC(9x5x & 9x4x)
#define OutDClk1    108
#define OutDClk2    135
#if PanelLVDS
//14.318 * 15 * 8 / 3.5 = 490.9, 490.9<<19=257374477.16,   490.9<<3=8=3927
#define DClkFactor  257374477ul
//#define DClkFactor      257650102// use 215 as base
#else
//14.318 * 15 * 8 / 4 = 429.54, 429.54<<19=225202667.52,   429.54<<3=8=3436.32
#define DClkFactor  225202667ul
//#define DClkFactor      257650102// use 215 as base
#endif
#define FreeRunDClk (DWORD)DClkFactor/PanelDCLK
// 2006/8/25 11:43AM by Emily BYTE InputSclk;  //2006-07-07 Andy
//*******************************************************************
//
// extern declaration
//
//*******************************************************************
//*******************************************************************
//
// local declaration
//
//*******************************************************************
//void mStar_InitADC( void );
#if !ENABLE_LED_CONTROLLER
void SetPWMFreq( WORD freq );
#endif

void msDrvPWMInit(void)
{
    extern WORD msDrvPWMMaskTransfer(DWORD u32Mask);
    DWORD u32PWMMask = BACKLIGHT_PWM;
#if AudioFunc
    u32PWMMask |=VOLUME_PWM;
    msAPIPWMConfig(VOLUME_PWM, VOLUME_PWM_FREQ, VOLUME_PWM_DUTY, VOLUME_PWM_CONFIG, VOLUME_PWM_VS_ALG_NUM);
#endif
    msAPIPWMConfig(BACKLIGHT_PWM, BACKLIGHT_PWM_FREQ, BACKLIGHT_PWM_DUTY, BACKLIGHT_PWM_CONFIG, BACKLIGHT_PWM_VS_ALG_NUM);
    msDrvPWMEnableSwitch(u32PWMMask, TRUE);
}

//*******************************************************************
// Function Name: mStar_Init(void)
//
// Description: Initialize mStar chip while 1st power on system
//
// Caller: mStar_InitADC(), mStar_InitTCON, mStar_SetupFreeRunMode()
//         _msWriteByte(), mStar_WriteWord() in ms_rwreg.c
//         mStar_WriteDDC1(), mStar_WriteDDC2() in ddc.c
//         mStar_InitGamma() in gamma.c
//         Osd_InitOSD() in osd.c
// Callee: Init_Device() in main.c
//*******************************************************************
void mStar_Init( void )
{
    BYTE NOUSE = 0;
    BYTE ucIdxWin = 0;

    msDrvmStarInit();
    msDrvPWMInit();
    msDrvMIUInit();
    msDrvPowerModCtrl(_DISABLE); //Jison 110421 disable panel data out

    // should included in AP func, takes win as param ??
    mStar_SetPanelTiming();
    mStar_SetupFreeRunMode();
    mStar_SetPanelSSCEnable(ENABLE_PANEL_SSC);
#if ENABLE_FACTORY_SSCADJ
    mStar_SetDDRSSCFactor(USER_PREF_DDR_SSC_FREQ_MODULATION,USER_PREF_DDR_SSC_PERMILLAGE_ADJUST);
 #else
    mStar_SetDDRSSCFactor(DDR_SSC_MODULATION_DEF, DDR_SSC_PERCENTAGE_DEF);
#endif 
    mStar_SetDDRSSCEnable(ENABLE_DDR_SSC);

#if ENABLE_DAC
    msAPI_AudioInit();  // Audio initialize
#endif
#if ENABLE_RTE
    msAPI_OverDriveInit();
#if ENABLE_OD_MTB
    msAPI_OverDriveMultiTable(0x00);
#else
    msAPI_OverDriveTable(tOverDrive);
#endif
    mStar_SetRTE();
#endif

#if ENABLE_SUPER_RESOLUTION
    msPQ_InitSuperResolution();
#endif

#if (!ZUI)
    #if (CHIP_ID == MST9U3)
    #if ENABLE_OSD_LR
    OSD_COMMON_WRITE_ONOFF(TRUE);   // common write Bank 101C -> 101A, 101D -> 101B
    #endif
    #endif
    Menu_InitOSD();
#endif
    iGenTuningFinished = 0;

    InitACEVar(); // Initialize the variable at ACE.c
#if ENABLE_PREFER_POST_CONTRAST
    msUsePostContrast(USER_PREF_WIN_SEL, true);
#endif
    InitDLCVar(TRUE, PanelWidth, PanelHeight);
    if(NOUSE)
    {
        msAdjustVideoContrast( USER_PREF_WIN_SEL, 0x80 );
        msAdjustVideoHue( USER_PREF_WIN_SEL, 0x32 );
        msAdjustVideoSaturation( USER_PREF_WIN_SEL, 0x80 );
    #if ENABLE_TNR
        AdjustTNR(0);
    #endif
        ucIdxWin=ucIdxWin;
    }

#if ENABLE_3DLUT
    for( ucIdxWin = DISPLAY_WIN1 ; ucIdxWin < DISPLAY_MAX_NUMS ; ucIdxWin++ )
        mStar_Setup3DLUT( ucIdxWin,0, DISABLE);
#endif

#if ENABLE_DeBlocking
    msInitDeBlocking( );
#endif
 //   if(g_bAPPWindow_Init&&CB_FuncList[eCB_SetupUserPref]!=NULL)
 //   ((fpSetupUserPref)CB_FuncList[eCB_SetupUserPref])(DISPLAY_WIN1);

#if (ENABLE_MENULOAD&&(MENULOAD_BUFFER_LEN != 0))
    {
        MApi_XC_MLoad_Init(MENULOAD_BUFFER_ADR+MIU_INTERVAL, MENULOAD_BUFFER_LEN);
        MApi_XC_MLoad_Enable(TRUE);
    }
#endif

#if(!ZUI)
#if ENABLE_OSDC_OSDD
    msDrvOSDBlend_Init(SOSD_SOURCE);
#else
    #if (CHIP_ID == MST9U3)
    #else
    msWriteByteMask(SCL0_00_81, BIT4, BIT5|BIT4); // sram OSD
    msWrite2ByteMask(REG_101C02, 0x00, BIT12|BIT2);
    #endif
    msWriteByteMask(REG_120F11, 0x00, BIT3|BIT2); // SOSD select odclk
#endif // #if ENABLE_OSDC_OSDD
#endif // #if(!ZUI)
    //msAPI_LoadICCInitialTable();

#if ENABLE_DEMURA_FUNCTION
    drvLoadCompressDemuraTable();
    drvSetDemuraTable();
    drvInitialDemura(PanelWidth);
#endif

#if ENABLE_DP_OUTPUT
#if eDPTXHBR2PortEn
    DPTxInit();
    DPTxSetMSA(0);
#endif
#endif

    //DLC Histogram enable
    for( ucIdxWin = DISPLAY_WIN1 ; ucIdxWin < DISPLAY_MAX_NUMS ; ucIdxWin++ )
        msDrvHistogramRangeEnable(ucIdxWin, TRUE);
}


//[7:6] : vtotal change
//[9:8] : vsync loss
//[19:18]: htotal change
//[21:20]: hsync loss
#if UseINT //Enable mode change INT
void mStar_EnableModeChangeINT(Bool u8Enable, WORD u16SCMask)
{
    DWORD u32SCRegBase;
    BYTE u8ScIndex = 0;

    while(u16SCMask!=0)
    {
        if(u16SCMask & BIT0)
        {
            DWORD u32SCINTMain = SC_INT_MUTE_MAIN, u32SCINTSub = SC_INT_MUTE_SUB;

            u32SCRegBase = SCRegBase(u8ScIndex);
            if(u8ScIndex & BIT0) // Main
            {
                if(INPUT_IS_DP_DRR(MapScaler2Port(u8ScIndex)))
                    u32SCINTMain = u32SCINTMain & (~BIT20);
                if(INPUT_IS_HDMI_DRR(MapScaler2Port(u8ScIndex)))
                    u32SCINTMain = u32SCINTMain & (~BIT6);

                msWrite4ByteMask(SC00_24+u32SCRegBase, SC_INT_MUTE_MAIN, SC_INT_MUTE_MAIN); // int clr
                msWrite4ByteMask(SC00_28+u32SCRegBase, u8Enable?0:u32SCINTMain, u32SCINTMain); // int mask
            }
            else  // Sub
            {
                if(INPUT_IS_DP_DRR(MapScaler2Port(u8ScIndex)))
                    u32SCINTSub = u32SCINTSub & (~BIT21);
                if(INPUT_IS_HDMI_DRR(MapScaler2Port(u8ScIndex)))
                    u32SCINTSub = u32SCINTSub & (~BIT7);

                msWrite4ByteMask(SC00_24+u32SCRegBase, SC_INT_MUTE_SUB, SC_INT_MUTE_SUB); // int clr
                msWrite4ByteMask(SC00_28+u32SCRegBase, u8Enable?0:u32SCINTSub, u32SCINTSub); // int mask
            }
        }

        u16SCMask >>=1;
        u8ScIndex ++;
    }
}
#endif

void mStar_SetPanelTiming()
{
    BYTE ucSCIndex;

    for( ucSCIndex = SCL0_SUB ; ucSCIndex < SC_MAX_NUMS-1 ; ucSCIndex++ )
        appmStar_SetPanelTiming(ucSCIndex);

    msDrvMFTSetup();
    msDrvSetDClkPLL((DWORD)PanelHTotal*PanelVTotal*GetVfreq()/1000);
#if ENABLE_FACTORY_SSCADJ 
    mStar_SetPanelSSCFactor(USER_PREF_SSC_FREQUENCY,USER_PREF_SSC_PERMILLAGE_ADJUST);
#else
    mStar_SetPanelSSCFactor(PANEL_SSC_MODULATION_DEF, PANEL_SSC_PERCENTAGE_DEF);
#endif
}

//*******************************************************************
// Function Name: mStar_SetupMode
//
// Decscription: setup registers for input timing,
// return      : TRUE,
// caller: mStar_SetupADC(), mStar_SetupCaptureWindow(),
//         mStar_SetScalingFactor(), mStar_SetPanelTiming(),
//         msAPI_SetUserPref() in mstar.c
//         mStar_FineTuneDVIPhase(), mStar_ValidTimingDetect() in detect.c
//         mSar_WriteByte(), _msReadByte() in ms_rwreg.c
// callee: mStar_ModeHandler() in detect.c
//*******************************************************************
Bool msDrvSetupMode(BYTE SCDetWin)
{
    BYTE u8WinIndex = MapScaler2Win(SCDetWin);
    FSyncMode eFrameSyncType = FLM_NO_LOCK;
    BYTE InputPort = MapScaler2Port(SCDetWin);
#if ((PanelType==PanelLTM340YP01)&&(defined(LianHeChuangXin_Project)))	
    BYTE InputVFreq=(V_FREQ_IN(MapScaler2Port(SCL0_MAIN)));
#endif

    if( UnsupportedModeFlag(MapScaler2Port(SCDetWin)) ) //prevent from unexpected behavior (e.g. /0) in msDrvScSetupPath()
        return FALSE;

    #if ENABLE_DVI_INPUT
    if(g_InputPort[InputPort].eIPType == TYPE_DVI_DUAL)
    {
        if(msAPI_combo_IPGetTmdsClkStable((EN_COMBO_IP_SELECT)g_InputPort[InputPort].eIPMux))
            msAPI_combo_tmds_DualLink_Align((EN_COMBO_IP_SELECT)g_InputPort[InputPort].eIPMux);
    }
    #endif

    Clr_FreeRunModeFlag(SCDetWin);

    msDrvScSetupPath(SCDetWin);

    //msDrvHistogramSetRange(SCDetWin);

    if( IsWindowDirectDisplay(u8WinIndex) )
    {
        eFrameSyncType = FLM_NO_LOCK;
    }
    else if( g_ScalerInfo[SCDetWin].u8SiFbMode )
    {
        if((INPUT_IS_DRR(MapScaler2Port(SCDetWin)))
#if ((PanelType==PanelLTM340YP01)&&(defined(LianHeChuangXin_Project)))	
      &&(InputVFreq<110)
#endif      
 	)
    	{
        	if (msDrvCheckMultiWin()) 
		{
            	eFrameSyncType = FLM_NO_LOCK;
    		}
		else
		{
	#if (PanelType==PanelLM270WR2)	
          if((abs(InputWidth-800)<10)&&(abs(InputHeight-600)<10))
             eFrameSyncType = FLM_FB_FAST_LOCK;
	   else	  		
	#endif   	
	#if (PanelType==PanelLSM315HP01)	
          if((abs(InputWidth-640)<10)&&(abs(InputHeight-480)<10))
             eFrameSyncType = FLM_FB_NEW_LOCK;//FLM_FB_FAST_LOCK;
	   else	  		
	#endif   	
	       eFrameSyncType = FLM_FB_FLOCK; //By Pass Input to Output
		}  			
	}
        else if( g_DisplayWindow[u8WinIndex].bDwReadCurFrame )
    	{
            eFrameSyncType = FLM_FB_NEW_LOCK;
			
    	}
        else
    	{
            eFrameSyncType = FLM_FB_FAST_LOCK;
    	}
    }
    else
    {
        eFrameSyncType = FLM_FBL_FLOCK;
    }
	       printf(" @@@@@@@@@@ FLM_FB_FLOCK===============>>eFrameSyncType====%x \r\n",eFrameSyncType);
    msDrvSetFrameSyncMode(eFrameSyncType, SCDetWin);
    msDrvSetTimingGen(SCDetWin);
    // enable double buffer
    mStar_ScalerDoubleBuffer(TRUE);

    // use interrupt to speedup mode changing while input timing is changing
#if UseINT
    if(SCDetWin == MapPort2DetScIndex(MapScaler2Port(SCDetWin)))
    	{
        mStar_EnableModeChangeINT(TRUE, BIT0<<SCDetWin);
    	}
#endif

    #if ENABLE_RTE
     mStar_SetRTE();
    #endif

#if ENABLE_TNR
    if( TNROnFlag )
        msTNROnOff( TRUE );
    else
        msTNROnOff( FALSE );
#endif

   if(!BacklightFlag)
    Clr_VBY1TrainingPassFlag();

    return TRUE;
}

//*******************************************************************
// Function Name: mStar_SetupFreeRunMode
//
// Decscription: setup registers for free run mode without any input timing,
//
// caller: mStar_ResetDClkPLL() in mstar.c
//         mSar_WriteByte(), _msReadByte(), mStar_WriteWord() in ms_rwreg.c
// callee: mStar_ModeHandler() in detect.c
//*******************************************************************

//tmp should consider output display or no display to set freerun!!!
void mStar_SetupFreeRunMode(void)
{
    BYTE ucSCIndex;

    for( ucSCIndex=SCL0_SUB ; ucSCIndex < SC_MAX_NUMS-1 ; ucSCIndex++ )
    {
        drvmStar_SetupFreeRunMode(ucSCIndex);
    }

#if BrightFreqByVfreq
    SetPWMFreq( 240 );
#endif
}
//*******************************************************************
// Function Name: mStar_SetAnalogInputPort
//
// Decscription: setup registers for Separate Sync/Composite Sync/SOG,
//
// caller: mSar_WriteByte(), _msReadByte() in ms_rwreg.c
//
// callee: mStar_MonitorInputTiming() in detect.c
//*******************************************************************
void mStar_SetAnalogInputPort( BYTE InputPort, Bool ToSOGPort )
{

#if 0 // dump, wait correct setting is confirmed

    Bool bTure;

    bTure = ToSOGPort;

#else
    BYTE u8SCDetIndex = MapPort2DetScIndex(InputPort);
    WORD u16ScMask    = g_InputPort[InputPort].u16IPScMask;
    BYTE regValue     = msDrvIP1GetInputSyncType(u8SCDetIndex);

#if MS_VGA_SOG_EN
   // if( regValue & BIT4 )
   if(!ToSOGPort)
    {
        regValue = regValue & 0x8F;
        g_bInputSOGFlag=0;
        //if(bInputVGAisYUV)
        //    drvADC_SetADCSource(ADC_TABLE_SOURCE_YUV_HV);  // input is YUV
        //else
            drvADC_SetADCSource(ADC_TABLE_SOURCE_RGB);   // input is RGB

        msDrvIP1AdcSetCoast(u16ScMask, 0x01, 0x03, 0x01);//_msWriteByte(SC0_ED, 0x01);   // enable ADC coast
        msDrvIP1AdcGlitchRemoval(u16ScMask, 0);//_msWriteByte(SC0_F3, 0x00 ); //RD suggest 20081008
        msDrvIP1SetInputVSyncDelay(u16ScMask, TRUE);
        ADC_PLL_LOCKING_EDGE(0);//_msWriteByteMask(REG_ADC_DTOP_07_L,0,BIT5); // 0:Hsync leading edge; 1: Hsync trailing edge
        MSTAR_PRINT("InputPort_____VGA\n");
    }
    else
    {
        regValue = regValue | 0x70;
        g_bInputSOGFlag=1;
        //if(bInputVGAisYUV)
            drvADC_SetADCSource(ADC_TABLE_SOURCE_SOG);  // input is YUV
        //else
        //    drvADC_SetADCSource(ADC_TABLE_SOURCE_RGB);   // input is RGB

        msDrvIP1AdcSetCoast(u16ScMask, 0x21, 0x08, 0x08);//_msWriteByte(SC0_ED, 0x21);   // enable ADC coast
        msDrvIP1AdcGlitchRemoval(u16ScMask, 0);//_msWriteByte( SC0_F3,  0x00 );
        msDrvIP1SetInputVSyncDelay(u16ScMask, FALSE);
        ADC_PLL_LOCKING_EDGE(1);//_msWriteByteMask(REG_ADC_DTOP_07_L,BIT5,BIT5); // 0:Hsync leading edge; 1: Hsync trailing edge
        MSTAR_PRINT("InputPort_____SOG\n");
    }
#else
    ToSOGPort=ToSOGPort;
    regValue = regValue & 0x8F;
    g_bInputSOGFlag = 0;
    drvADC_SetADCSource(ADC_TABLE_SOURCE_RGB);
#endif

    msDrvIP1SetInputSyncType(u16ScMask, regValue);//_msWriteByte(SC0_02, regValue);

    //msDrvSoftwareReset(RST_ADC_DIG_A, 0);//temp
#endif
}

//*******************************************************************
// Function Name: mStar_ReadAutoWindow
//
// Decscription: read auto window
//
// caller:
///
// callee:
//*******************************************************************
ST_WINDOW_INFO mStar_ReadAutoWindow( BYTE InputPort )
{
    ST_WINDOW_INFO AutoWin;
    BYTE SCDetWin = MapPort2DetScIndex(InputPort);

#if ENABLE_VGA_INPUT
    if (INPUT_IS_VGA(InputPort))
    {
        AutoWin.x       = USER_PREF_H_START(InputPort);
        AutoWin.y       = USER_PREF_V_START(InputPort);
    }
    else
#endif
    {
        AutoWin.x       = msDrvIP1AutoPositionReadHstart(SCDetWin, InputPort);
        AutoWin.y       = msDrvIP1AutoPositionReadVstart(SCDetWin, InputPort);
    }
#if Enable_Auto_Switch_DE_Mode
   if(!FreeSyncFlag)
    	AutoWin.width   = msDrvIP1AutoPositionReadHend(SCDetWin, InputPort) - AutoWin.x + 1;
  else
   {
	if(INPUT_IS_HDMI_2_0(InputPort)&& HBR2Flag(InputPort))
        AutoWin.width = msDrvIP1ReadHtotal(SCDetWin)-(msDrvIP1AutoPositionReadHstart(SCDetWin, InputPort)-msDrvIP1AutoPositionReadHend(SCDetWin, InputPort))+2;
	else
        AutoWin.width = msDrvIP1AutoPositionReadHend(SCDetWin, InputPort) - AutoWin.x + 1;
   }

  if(!FreeSyncFlag)
    	AutoWin.height  = msDrvIP1AutoPositionReadVend(SCDetWin, InputPort) - AutoWin.y + 1;
   else
   {
	if(INPUT_IS_HDMI_2_0(InputPort)&& HBR2Flag(InputPort))
        AutoWin.height  = msDrvIP1AutoPositionReadVend(SCDetWin, InputPort) - AutoWin.y ;
	else
        AutoWin.height  = msDrvIP1AutoPositionReadVend(SCDetWin, InputPort) - AutoWin.y + 1;
   }
#else
    #if ENABLE_DEONLY_MODE
    	AutoWin.width   = msDrvIP1AutoPositionReadHend(SCDetWin, InputPort) - AutoWin.x + 1;
    #else
	if(INPUT_IS_HDMI_2_0(InputPort)&& HBR2Flag(InputPort))
        AutoWin.width = msDrvIP1ReadHtotal(SCDetWin)-(msDrvIP1AutoPositionReadHstart(SCDetWin, InputPort)-msDrvIP1AutoPositionReadHend(SCDetWin, InputPort))+2;
	else
        AutoWin.width = msDrvIP1AutoPositionReadHend(SCDetWin, InputPort) - AutoWin.x + 1;

//printData("xxxxxxxxxx AutoWin.width==%d",AutoWin.width);
    if((INPUT_IS_HDMI(InputPort))&&(!INPUT_IS_HDMI_DRR(InputPort)))
    {
      if((abs(AutoWin.width-720)<2)&&(AutoWin.width!=720))
      	{
            AutoWin.x  = AutoWin.x-1;
	     AutoWin.width=720;		
      	}
	else if((abs(AutoWin.width-1280)<2)&&(AutoWin.width!=1280))   
      	{
            AutoWin.x  = AutoWin.x-1;
	     AutoWin.width=1280;		
      	}
	else if((abs(AutoWin.width-1920)<2)&&(AutoWin.width!=1920))   
      	{
            AutoWin.x  = AutoWin.x-1;
	     AutoWin.width=1920;		
      	}
	else if((abs(AutoWin.width-2560)<2)&&(AutoWin.width!=2560))   
      	{
            AutoWin.x  = AutoWin.x-1;
	     AutoWin.width=2560;		
      	}
	else if((abs(AutoWin.width-3840)<2)&&(AutoWin.width!=3840))   
      	{
            AutoWin.x  = AutoWin.x-1;
	     AutoWin.width=3840;		
      	}
	else if((abs(AutoWin.width-4096)<2)&&(AutoWin.width!=4096))   
      	{
            AutoWin.x  = AutoWin.x-1;
	     AutoWin.width=4096;		
      	}

    }
	
    #endif

    #if ENABLE_DEONLY_MODE
    	AutoWin.height  = msDrvIP1AutoPositionReadVend(SCDetWin, InputPort) - AutoWin.y + 1;
    #else
	if(INPUT_IS_HDMI_2_0(InputPort)&& HBR2Flag(InputPort))
        AutoWin.height  = msDrvIP1AutoPositionReadVend(SCDetWin, InputPort) - AutoWin.y ;
	else
        AutoWin.height  = msDrvIP1AutoPositionReadVend(SCDetWin, InputPort) - AutoWin.y + 1;
    #endif
#endif

    if( INPUT_IS_TMDS(InputPort)
#if ENABLE_DP_INPUT
        || INPUT_IS_DISPLAYPORT(InputPort)
#endif
        || INPUT_IS_VD(InputPort)
    )
    {
        //USER_PREF_H_START(InputPort) = AutoWin.x; // get hsync DE start
        //USER_PREF_V_START(InputPort) = AutoWin.y; // get vsync DE start
        SrcAutoHstart(InputPort) = AutoWin.x;
        SrcAutoVstart(InputPort) = AutoWin.y;

        if(InterlaceModeFlag(InputPort))
        {
            if(msDrvIP1IsDEOnlyMode(SCDetWin) == TRUE)//(_scReadByte(SC0_04)&BIT6) //DE only mode
            {
                AutoWin.y &= (~BIT0);
                if(msDrvIP1IsVideoFieldInvert(SCDetWin)== FALSE)//((_scReadByte(SC0_E9)&BIT3)==0) //video field invert
                    AutoWin.height++;  //height=(Vend ¡V Vstart + 2);
                else
                    AutoWin.height-=3; //height=(Vend ¡V Vstart - 2);
            }
            else
            {
                AutoWin.y |= (BIT0);
                AutoWin.height--; //height=(Vend ¡V Vstart)
            }
        }
    }
    else
    {
        // input 1360x768 display on 1366 panel,
        // don't do sacle and get 1366 for hori. data
        if( PanelWidth == 1366 )
        {
            if( StandardModeGroup(InputPort) == RES_1360x768 )
                AutoWin.width = 1366;
            else
                AutoWin.width = StandardModeWidth(InputPort);
        }
        else
        {
            AutoWin.width = StandardModeWidth(InputPort);
        }

        if( StandardModeGroup(InputPort) == RES_1152x864 )
        {
            AutoWin.height = 864;
        }
        #if 0
        else if((( StandardModeGroup(InputPort) == RES_720x480I ) && ( abs( StandardModeVFreq(InputPort) - 599 ) < 10 ) && ( abs( StandardModeHFreq(InputPort) - 157 ) < 10 ) )
                || (( StandardModeGroup(InputPort) == RES_720x576I ) && ( abs( StandardModeVFreq(InputPort) - 500 ) < 10 ) && ( abs( StandardModeHFreq(InputPort) - 156 ) < 10 ) )
                || (( StandardModeGroup(InputPort) == RES_1920x1080_60 ) && ( abs( StandardModeVFreq(InputPort) - 600 ) < 10 ) && ( abs( StandardModeHFreq(InputPort) - 337 ) < 10 ) )
                || (( StandardModeGroup(InputPort) == RES_1920x1080_50 ) && ( abs( StandardModeVFreq(InputPort) - 500 ) < 10 ) && ( abs( StandardModeHFreq(InputPort) - 281 ) < 10 ) ) ) // for VGA InterlaceMode
        {
            AutoWin.height = StandardModeHeight(InputPort) + 4;
        }
        #endif
        else
        {
            AutoWin.height = StandardModeHeight(InputPort);
        }
    }

#if 0
    // HDMI YUV 422, VActive - 1
    AutoWin.width = (AutoWin.width + 1) & 0xFFFE;
    if( msAPISrcCheckHDMI422(InputPort) )
        AutoWin.x &= 0xFFFE;
#endif

    return AutoWin;
}
//*******************************************************************
// Function Name: mStar_SetupClampingByMode
//
// Decscription: When input timing is reduced blanking, we have to reduce the
//   duration of ADC calibration to avoid dark display.
//    The amount of calibration duration must be smaller than back-porch.
//    Currently, we set the calibration duration to H_Back_Porch/2.
// caller: mSar_WriteByte(), _msReadByte() in ms_rwreg.c
///
// callee: mStar_SetupMode() in mstar.c
//*******************************************************************
#if ENABLE_VGA_INPUT
void mStar_SetupClampingByMode(void)
{
    BYTE SCDetWin, InputPort;

    InputPort = Input_Analog1;
    SCDetWin = MapPort2DetScIndex(InputPort);

    if (INPUT_IS_VGA(MapScaler2Port(SCDetWin)))//(MapScaler2Port< Input_Digital)
    {
        if(g_bInputSOGFlag)  //Steven101221
        {
            switch(SrcModeIndex(InputPort))
            {
                case MD_640X480_60:             //Back-porch=25 pixel
                case MD_640X350_70:             //Back-porch=48 pixel
                    drvADC_AdjustCalDuring(7);
                    break;
                case MD_720X480I_60:            //Back-porch=57 pixel
                case MD_720X400_70:             //Back-porch=48 pixel
                    drvADC_AdjustCalDuring(15);
                    break;
                case MD_1280X768_60R:
                case MD_1280X800_60R:
                case MD_1440X900_60R:
                case MD_1600X900_60R:
                case MD_1600X1000_60R:
                case MD_1680X1050_60R:
                case MD_1920X1200_60R:
                case MD_1920X1200_50R:
                case MD_2048X1152_60R:
                case MD_1920X1080_60R:          //Back-porch=80 pixel
                    drvADC_AdjustCalDuring(31);
                    break;
            }
        }
        else
        {
            switch(SrcModeIndex(InputPort))
            {
                case MD_1920X1440_60R: //CVT1960R
                case MD_2048X1280_60:  //CVR2059D
                    drvADC_AdjustCalDuring(60);  // unit: pixel, 63 is ok, use 60
                    break;
                case MD_2048X1152_60_1:  //for DMR2060H
                    drvADC_AdjustCalDuring(0x20);
                    break;
                case MD_2048X1152_60R:  //for bubble screen saver crosstolk issue
                    drvADC_AdjustCalDuring( 0x18 );
                    break;

                case MD_2560x1080_60:            
                case MD_2560x1440_60:      
		  case MD_2560x1600_60:			

			drvADC_AdjustCalDelay(0x08);
                    break;

          }
        }
    }
}
#endif
//*******************************************************************
// Function Name: mStar_SetupADC
//
// Decscription: setup ADC bandwidth/filter, clock, phase for sampling input data
//               and R/G/B gains, offsets
// caller: mSar_WriteByte(), _msReadByte() in ms_rwreg.c
///
// callee: mStar_SetupMode() in mstar.c
//*******************************************************************
#if ENABLE_VGA_INPUT
void mStar_SetupADC(BYTE InputPort)
{
    WORD tempValue;

    if(!INPUT_IS_VGA(InputPort))//(MapScaler2Port > Input_Analog1)
        return;

    tempValue = HFREQ( SrcHPeriod(InputPort) );//(( DWORD )MST_CLOCK_MHZ * 10 + SrcHPeriod / 2 ) / SrcHPeriod; //calculate hfreq: round 5
    tempValue = (( DWORD )tempValue * USER_PREF_H_TOTAL(InputPort) + 5000 ) / 10000; //dclk= hfreq * htotal

#if DADCPLLPROTECTION
    drvADC_SetModewithPLLProtection(g_bInputSOGFlag ? ADC_INPUTSOURCE_YPBPR : ADC_INPUTSOURCE_RGB, tempValue, USER_PREF_H_TOTAL(InputPort));
#else
    drvADC_AdjustHTotal(USER_PREF_H_TOTAL(InputPort));

    drvADC_SetADCModeSetting((g_bInputSOGFlag ? ADC_INPUTSOURCE_YPBPR : ADC_INPUTSOURCE_RGB), tempValue);
#endif
    //drvADC_SetPhaseCode((UPPHASE_GET_REAL_VALUE()+1)%MAX_PHASE_VALUE); //Jison 100818 patch for phase state machine reset
    drvADC_SetPhaseCode(USER_PREF_PHASE(InputPort)); //Jison 100818
    drvADC_SetupHsyncPolarity((BOOL)(SrcFlags(InputPort) & bHSyncNegative)); //Jison 100818

    // wait for checking
    drvADC_SetRGBGainCode(USER_PREF_ADC_RED_GAIN, USER_PREF_ADC_GREEN_GAIN, USER_PREF_ADC_BLUE_GAIN);
    drvADC_SetRGBOffsetCode(USER_PREF_ADC_RED_OFFSET, USER_PREF_ADC_GREEN_OFFSET, USER_PREF_ADC_BLUE_OFFSET);
    //drvADC_AdjustCalDuring(7);

    mStar_SetupClampingByMode(); //Jison 110106
    //msADC_AdjustCalDuring( 80/2 );  // unit: pixel // coding temp setting
}
#endif

#if 0
//*******************************************************************
// Function Name: mStar_InitADC
//
// Decscription: Initialize ADC bank registers
//
// callee: _msWriteByte() in ms_rwreg.c
//
// caller: mStar_Init() in mStar.c
//*******************************************************************
void mStar_InitADC( void )
{

    drvADC_init(FALSE);
#if CHIP_FAMILY_TYPE == CHIP_FAMILY_MST9U
    drvADC_ADCLinearityOffsetGainMismatchCal();
#else
    drvADC_ADCOffsetGainMismatchCal();
#endif
#if 0 //def _DGAIN_CAL_WITHOUT_INPUT_
    drvADC_ADCAutoGainCalwithoutInput();
#endif
#if ENABLE_ADC_DITHERING
    msWriteByteMask(REG_ADC_DTOPB_00_L, 0x00, 0x03); // [1:0]: enable ADCA interleve H/V dither in the normal display
#else
    msWriteByteMask(REG_ADC_DTOPB_00_L, 0x03, 0x03); // [1:0]: disable ADCA interleve H/V dither in the normal display
#endif
}
#endif

#if ((!ENABLE_LED_CONTROLLER)&&BrightFreqByVfreq)
void SetPWMFreq(WORD freq)
{
    msDrvPWMFreq(BACKLIGHT_PWM, freq);
}
#endif
#if BrightFreqByVfreq
void SetFourtimesPWMFreq( BYTE InputPort )
{
    WORD vfreq, hfreq;
    hfreq = HFREQ( SrcHPeriod(InputPort) );
    vfreq = VFREQ( hfreq, SrcVTotal(InputPort) );
   
 	if( abs( vfreq - 500 ) < 10 ) //when Input Vfreq=50Hz,Set Brightness freq to 250Hz
    	SetPWMFreq( 240 );
	else
    	SetPWMFreq(( vfreq * 4 ) / 10 );
}
#endif

// BlackWhite control for All scaler
void mStar_BlackWhiteScreenCtrl(BYTE u8Ctrl)
{
    msDrvSCCommonWrite(SCALER_MASK_MAIN, ENABLE);
    if (u8Ctrl==BW_SCREEN_WHITE)
        SC0_WHITE_SCREEN_ENABLE();//_msWriteByteMask(SC0_43,BIT5,(BIT4|BIT5));
    else if (u8Ctrl==BW_SCREEN_BLACK)
    {
        printMsg("000000 BW_SCREEN_BLACK");
        Set_BlackScreenFlag();
        SC0_BLACK_SCREEN_ENABLE();//_msWriteByteMask(SC0_43,BIT4,(BIT4|BIT5));
    }
    else
    {
        printMsg("1111111 CLR_BW_SCREEN_BLACK");
        Clr_BlackScreenFlag();
        SC0_BLACK_WHITE_SCREEN_DISABLE();//_msWriteByteMask(SC0_43,0,(BIT4|BIT5));
    }
    msDrvSCCommonWrite(SCALER_MASK_MAIN, DISABLE);
}

//#if ENABLE_HDMI_INPUT
void  SettingInputColorimetry(BYTE ucWinIndex)
{
    BYTE InputPort = MapWin2Port(ucWinIndex);
#if EABLE_Y_EXTENSION
    EN_INPUT_COLOR eInputColor;
    EN_COLOR_RANGE eInputRange;
    msAPIWinGetColorSpaceInput(ucWinIndex, &eInputColor, &eInputRange);
#endif

#if ENABLE_HDMI_INPUT||ENABLE_DP_INPUT
    if((INPUT_IS_TMDS(InputPort)||INPUT_IS_DISPLAYPORT(InputPort))&&(msAPISrcGetPktYuvColorimetry(ucWinIndex)!=eINPUT_YUV_COLORIMETRY_ITU601))
    {
        #if EABLE_Y_EXTENSION
        if(eInputColor == eINPUT_COLOR_RGB)
        {
            if(eInputRange == eCOLOR_RANGE_LIMIT)
            {
                msACESelectYUVtoRGBMatrix(ucWinIndex, ACE_YUV_TO_RGB_MATRIX_HDTV, 0);
            }
            else
            {
                msACESelectYUVtoRGBMatrix(ucWinIndex, ACE_YUV_TO_RGB_MATRIX_HDTV_LIMIT, 0);
            }
        }
        else
        #endif
        {
            if(eInputRange == eCOLOR_RANGE_FULL)
            {
                msACESelectYUVtoRGBMatrix(ucWinIndex, ACE_YUV_TO_RGB_MATRIX_HDTV_LIMIT, 0);
            }
            else
            {
                msACESelectYUVtoRGBMatrix(ucWinIndex, ACE_YUV_TO_RGB_MATRIX_HDTV, 0);
            }
        }
    }
    else
#endif
    {
        #if EABLE_Y_EXTENSION
        if(eInputColor == eINPUT_COLOR_RGB)
        {
            if(eInputRange == eCOLOR_RANGE_LIMIT)
            {
                msACESelectYUVtoRGBMatrix(ucWinIndex, ACE_YUV_TO_RGB_MATRIX_SDTV, 0);
            }
            else
            {
                msACESelectYUVtoRGBMatrix(ucWinIndex, ACE_YUV_TO_RGB_MATRIX_SDTV_LIMIT, 0);
            }
        }
        else
#endif
        {
            if(eInputRange == eCOLOR_RANGE_FULL)
            {
                msACESelectYUVtoRGBMatrix(ucWinIndex, ACE_YUV_TO_RGB_MATRIX_SDTV_LIMIT, 0);
            }
            else
            {
                msACESelectYUVtoRGBMatrix(ucWinIndex, ACE_YUV_TO_RGB_MATRIX_SDTV, 0);
            }
        }
    }
}

//#endif


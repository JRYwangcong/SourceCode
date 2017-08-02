#include <math.h>
#include "datatype.h"
#include "Board.h"
#include "Global.h"
#include "ms_reg.h"
#include "Ms_rwreg.h"
#include "Power.h"
#include "Common.h"
#include "misc.h"
#include "UserPrefDef.h"
#include "UserPrefAPI.h"
#include "msOSD.h"
#include "mStar.h"
//#include "Panel.h"
#include "Adjust.h"
#include "Mode.h"
#include "GPIO_DEF.h"
#include "drvOSD.h"
#include "drvADC.h"
#include "AutoFunc.h"
#include "Detect.h"
#ifdef UseInternalDDCRam
#include "DDC.h"
#endif
#include "menufunc.h"
#include "Menu.h"
#if (ZUI)
#include "MApp_ZUI_ACTglobal.h"
#endif
#include "msACE.h"
#include "MsDLC.h"
 #if ENABLE_DP_INPUT
#include "drvDPRxApp.h"
#endif
#if ENABLE_DAC
#include "drvAudio.h"
#endif
#include "msScaler.h"
#include "appInput.h"
#include "Factorymenu.h"

/////////////////////////////////////
#include "drvmStar.h"
//#include "halRwreg.h"
////////////////////////////////////
#if ENABLE_HDMI_DRR
#include "Mcu.h"
#endif

#define DETECT_DEBUG    1
#if ENABLE_MSTV_UART_DEBUG && DETECT_DEBUG
#define DETECT_PRINT(format, ...)     printf(format, ##__VA_ARGS__)
#else
#define DETECT_PRINT(format, ...)
#endif

////////////////////////////////////////////////////////////////////////

#define HVStartTorlance     2
#define HVDETorlance        5

//#define HFreq_Torlance     12 // 15 // 1.5 KHz
//#define VFreq_Torlance     12 // 15 // 1.5 Hz

//OTS_UU215DGLW spec 100812

#if  ENABLE_DP_INPUT|| ENABLE_HDMI_INPUT
#define MaxInputHFreq      ((CURRENT_INPUT_IS_DISPLAYPORT()||CURRENT_INPUT_IS_HDMI())? 1000:800)// 800  //FOR 3D
#define MinInputHFreq       ((CURRENT_INPUT_IS_DISPLAYPORT()||CURRENT_INPUT_IS_HDMI())? 100:100)  //for Dp interlace timing  //plz  HY conf spec
#else
#define MaxInputHFreq      800
#define MinInputHFreq       100//300 100831 code test
#endif
#define MaxInputVFreq      800//750
#define MinInputVFreq       490//560//560  100831 code test
extern InputModeType code StandardMode[];

Bool _mdrv_mhl_GetDisplayReadyFlag(BYTE ucPortSelect);
void _mhal_mhl_SetVbusCharge(BYTE ucPortSelect, Bool bFlag);


#if ENABLE_HDMI_INPUT
#define HDMI_STAY_1S_COUNT      50 // unit: 20mS
#define HDMI_SPEEDUP_SWITCH_PORT(InputPort)    ( !msAPISrcIsCableConnected(InputPort) && DVI_CLOCK_IS_MISSING() && (InputTimingStableCounter(InputPort) >= HDMI_STAY_1S_COUNT))
#endif
static void mStar_MonitorHBR2Status( BYTE InputPort )
{
    Bool bHBR2Temp = msAPI_combo_IPGetSplitFlag((EN_COMBO_IP_SELECT)g_InputPort[InputPort].eIPMux);

    if(INPUT_IS_DISPLAYPORT(InputPort)||INPUT_IS_HDMI_2_0(InputPort))
    {
        if(HBR2Flag(InputPort) != bHBR2Temp)
        {
            HBR2Flag(InputPort) = bHBR2Temp;
            if(HBR2Flag(InputPort))
                msDrvSrcSetHBR2FlagMux(InputPort, TRUE,(EN_INPUTPORT_TYPE)0);
            else
                msDrvSrcSetHBR2FlagMux(InputPort, FALSE,(EN_INPUTPORT_TYPE)0);
			
	     printData("INPUT_IS_HDMI_2_0(InputPort)==%d",INPUT_IS_HDMI_2_0(InputPort));
	     printData("bHBR2Temp==%d", bHBR2Temp);

	     printData("INPUT_IS_DISPLAYPORT(InputPort)==%d",INPUT_IS_DISPLAYPORT(InputPort));
	     printData("bHBR2Temp==%d", bHBR2Temp);
		 
            Set_InputTimingChangeFlag(InputPort);

        }
    }
}

BYTE GetMulWindowInputPortNums(void)
{
#if ENABLE_MULTI_WINDOW_SETTINGS_MENU
  #if Enable_AutoInput_Menu
     if(USER_PREF_INPUT_PRIORITY == INPUT_PRIORITY_AUTO)
     {
       if((USER_PREF_MULTI_WIN_MODE != eMULTIWIN_OFF)&&(g_ScalerInfo[DISPLAY_WIN1].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN2].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN3].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN4].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK))
       {
     	 return 1;//USER_PREF_MULTI_WIN_MODE;
       }
       else 
       return Input_Nums;    
    }
     else 
       return 1;
   	
  #else
    if((USER_PREF_MULTI_WIN_MODE != eMULTIWIN_OFF)&&(g_ScalerInfo[DISPLAY_WIN1].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN2].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN3].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN4].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK))
    {
      #if (defined(LianHeChuangXin_Project))
	 if(USER_PREF_MULTI_WIN_MODE > eMULTIWIN_2_POP1) 
  	  return 3;//USER_PREF_MULTI_WIN_MODE;
  	 else
      #endif	  
  	  return 2;//USER_PREF_MULTI_WIN_MODE;
    }
    else 
       return Input_Nums;
  #endif

#elif SwitchPortByDP_Detect
     return (Input_Nums-1);
#else
     return Input_Nums;
#endif
}

//*******************************************************************
// Function Name: mStar_MonitorInputTiming
//
// Decscription: Detect input timing
//
// callee: mStar_SyncLossStateDetect(), mStar_ValidTimingDetect(), mStar_AutoDVI() in detect.c
//         mStar_SetInputPort(), mStar_SetAnalogInputPort() in mstar.c
//
// caller: main() in main.c
//*******************************************************************
void mStar_MonitorInputTiming( BYTE InputPort )
{
    if(!POWER_ON_FLAG)
        return ;

    if (g_stMenuFlag.bShowDiagPattern)
        return;
    mStar_MonitorHBR2Status(InputPort);
    // detect whether input signal is changing
    if( !InputTimingChangeFlag(InputPort) )
    {
        if( SyncLossState(InputPort) )
        {

            if( !FreeRunModeFlag(MapPort2DetScIndex(InputPort)) )
            {
                Set_InputTimingChangeFlag(InputPort);
            }
            else if( mStar_SyncLossStateDetect(InputPort) ) // detect mode while no Sync/suspend/standby/disconnect
            {
                DETECT_PRINT( "mStar_SyncLossStateDetect\n" ); 
                Set_InputTimingChangeFlag(InputPort);
            }
            else if( InputTimingStableCounter(InputPort) 
            #if AutoSwitchPort_Enable
              &&(g_u8SwitchPortCounter>=GetMulWindowInputPortNums())
	     #endif	
            #if Enable_AutoInput_Menu
              &&(USER_PREF_INPUT_PRIORITY == INPUT_PRIORITY_AUTO)
            #endif
             #if Disable_MultiWindow_AutoSwitch
             &&(USER_PREF_MULTI_WIN_MODE <= eMULTIWIN_2_POP1)
             #endif
	      )
            {
                if(g_bAPPWindow_Init&&CB_FuncList[eCB_SourceSwitch]!=NULL)
                {
                    ((fpSourceSwitch)CB_FuncList[eCB_SourceSwitch])(InputPort, &g_u8SwitchPortCounter, &InputTimingStableCounter(InputPort), SyncLossThd(InputPort) );
                     APPInput_DetectSyncStableCntInit();
                }
	     }
        }
        else            // valid timing
        {
            // check if input timing is changing
            if( mStar_ValidTimingDetect(InputPort) )
            {
		  DETECT_PRINT( "mStar_ValidTimingDetect\n" ); 
   #if (ENABLE_MULTI_WINDOW_SETTINGS_MENU&&(defined(LianHeChuangXin_Project)))     
       if((USER_PREF_MULTI_WIN_MODE != eMULTIWIN_OFF)&&(g_ScalerInfo[DISPLAY_WIN1].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN2].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN3].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN4].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK))
        ;
	else   
   #endif		
   #if 0//Free_Sync_Enable     
       if((OSDFreeSyncActionFlag)&&(OSDShowFlag))
        ;
	else   
   #endif		
   	  {
             if((!g_stMenuFlag.bShowInputIcon)
           #if LOGO_ENABLE
               &&(!g_stMenuFlag.bDisplayLogo)
           #endif	
		  )
           	{
                hw_ClrBlacklit();
		  Menu_Hide();		
	      	}		
   	   }
		#if (VBY1_PowerSequence)		
		  PowerOnBackLightCounter=0;
		#endif
		   
                SrcFlags(InputPort) |= SyncLoss;
                Set_InputTimingChangeFlag(InputPort);
            }
        }
    }
    //=============================================================================
    //=============================================================================
    if( InputTimingChangeFlag(InputPort) )        // input timing is unstable
    {
        msAPISrcStatusUpdate(InputPort, IP_STATUS_WAITSYNC);

 #if AudioFunc
     if(g_bAPPWindow_Init&&CB_FuncList[eCB_CustomerSetModeChangeAudioMuteOnOff]!=NULL)
   ((fpCustomerSetModeChangeAudioMuteOnOff)CB_FuncList[eCB_CustomerSetModeChangeAudioMuteOnOff])();
     else
     {

       if (audio_EnableAudioAfterSignalLock_Flag!=AUDIO_MUTE)
        {
         audio_EnableAudioAfterSignalLock_Flag=AUDIO_MUTE;
	  hw_SetMute();	
	  hw_Clr_SPIFGPIO_EN_Pin(); 
 	#if ENABLE_DAC
         msAPI_AudioSetDpgaMute(AUDIO_DPGA_ALL, TRUE);
         msAPI_AudioSetEarphoneMute(TRUE);
         msAPI_AudioSetMute( AUDIO_SOFTWARE_MUTE, E_SW_TIMING_CHANGE_MUTE_ON);
	#endif
	#if ENABLE_DAC
         msDrvAudio_SetDPGAMuteSrc(E_DPGA_SOURCE_CHANGE_MUTE_ON);
	#endif
        }
      }
 #endif
        Clr_InputTimingChangeFlag(InputPort);
        Clr_DoModeSettingFlag(InputPort);
        Clr_ReadyForChangeFlag(InputPort);
	 Clr_InputTimingStable_NormalFlag();	

        DETECT_PRINT(" PrepareForTimingChange:%d\n", InputPort);

        #if DP_HPD_Test
		if(InputPort==INPUT_DISPLAYPORT)
		{
			TimingUnstableCNT++;
			DETECT_PRINT("222222222222222222222222222TimingUnstableCNT=%d\n",TimingUnstableCNT);
			if(TimingUnstableCNT>=10)
			{
			
			DETECT_PRINT("HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH\n");
				TimingUnstableCNT=0;
				CLR_DP_HPD_PORT4();
				MsOS_DelayTask(100);
				SET_DP_HPD_PORT4();
			}
		}
	#endif	
        mStar_PrepareForTimingChange(InputPort);
    }
    else        // input timing is stable
    {
        InputTimingStableCounter(InputPort)++; // counter timing stable times

        if( InputTimingStableCounter(InputPort) == 0 )
            InputTimingStableCounter(InputPort) = 1;

        if( !InputTimingStableFlag(InputPort) )
        {
            #if ENABLE_HDMI_INPUT
            if(HDMI_SPEEDUP_SWITCH_PORT(InputPort))
            {
                InputTimingStableCounter(InputPort) = SyncLossThd(InputPort);
            }
            #endif
              //printData("InputTimingStableCounter(InputPort)==%d",InputTimingStableCounter(InputPort));
              //printData("g_u8SwitchPortCounter==%d",g_u8SwitchPortCounter);
            if( SyncLossState(InputPort) && (InputTimingStableCounter(InputPort) >= ((g_u8SwitchPortCounter>=GetMulWindowInputPortNums())?2:SyncLossThd(InputPort))) )
            {
                Set_InputTimingStableFlag(InputPort); // set flag to search for input mode
				
          #if (AutoSwitchPort_Enable)
               if(SyncLossState(InputPort) && (g_u8SwitchPortCounter < GetMulWindowInputPortNums())
               #if Enable_AutoInput_Menu
               &&(USER_PREF_INPUT_PRIORITY == INPUT_PRIORITY_AUTO)
               #endif
               #if Disable_MultiWindow_AutoSwitch
               &&(USER_PREF_MULTI_WIN_MODE <= eMULTIWIN_2_POP1)
               #endif
		 )
               {		
   	         if(g_bAPPWindow_Init&&CB_FuncList[eCB_SourceSwitch]!=NULL)
  	         {
                   ((fpSourceSwitch)CB_FuncList[eCB_SourceSwitch])(InputPort, &g_u8SwitchPortCounter, &InputTimingStableCounter(InputPort), SyncLossThd(InputPort) );
   	         }
                  APPInput_DetectSyncStableCntInit();
		    Clr_InputTimingStableFlag(InputPort);	
		    Clr_OSDFreeSyncActionFlag();	
               }
               else
	   #endif	
	   	{
	   	  Set_InputTimingStable_NormalFlag();	
                msAPISrcStatusUpdate(InputPort, IP_STATUS_NOSYNC);
                Set_DoModeSettingFlag(InputPort);
				
	   	}
            }

            if( !SyncLossState(InputPort) && InputTimingStableCounter(InputPort) >= SyncValidThd(InputPort) )
            {

                msAPISrcStatusUpdate(InputPort, IP_STATUS_FINDMODE);
                Set_InputTimingStable_NormalFlag();	
                Set_InputTimingStableFlag(InputPort); // set flag to search for input mode
                Set_DoModeSettingFlag(InputPort);
	   #if ENABLE_MULTI_WINDOW_SETTINGS_MENU			
               if(USER_PREF_MULTI_WIN_MODE == eMULTIWIN_OFF)
	  #endif		   	
                g_u8SwitchPortCounter = 0; // sync locked, counter reset to 0

                
            }

        }
    }
}
//*******************************************************************
// Function Name: mStar_ModeHandler
//
// Decscription: Programming scaler while input timing is changing
//
// callee: mStar_SyncLossStateDetect(), mStar_ValidTimingDetect(), mStar_AutoDVI() in detect.c
//         mStar_SetInputPort(), mStar_SetAnalogInputPort() in mstar.c
//
// caller: main() in main.c
//*******************************************************************
#if AudioFunc
void mStar_SettingAudioFucntion( BYTE InputPort, BYTE SCDetWin)
{
    SCDetWin = SCDetWin;
   #if 0//ENABLE_DAC
    if (audio_EnableAudioAfterSignalLock_Flag!=AUDIO_IDLE)
        return;
   #endif
        if ( !SyncLossState(InputPort)&&!UnsupportedModeFlag(InputPort))
        {
        #if ENABLE_DAC

            if (audio_EnableAudioAfterSignalLock_Flag==AUDIO_UNMUTE)
                return;
		
                audio_EnableAudioAfterSignalLock_Flag=AUDIO_UNMUTE;
#if ((defined(LeXingEnMa_Project))&&ENABLE_MULTI_WINDOW_SETTINGS_MENU)
          if(USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP||USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_POP1)
          {
             if(PIP_MAIN_AUDIO_FLAG)
             	{
               msAPI_AudioSourceSel(USER_PREF_INPUT_TYPE);
		 gAudioSourceSelectableIndex=GetCurAudioSourceIndex(USER_PREF_INPUT_TYPE);	   
             	}
             else
             	{
               msAPI_AudioSourceSel(USER_PREF_INPUT_TYPE_SUBWIN1);
		 gAudioSourceSelectableIndex=GetCurAudioSourceIndex(USER_PREF_INPUT_TYPE_SUBWIN1);	   
             	}
          }
          else
#endif		  
             {

         #if Auto_Switch_AudioSource
              USER_PREF_AUDIO_SOURCE=GetCurAudioSourceIndex(InputPort);//auto switch audio
         #endif
		#if Audio_Source_Enable
		  SetAudioSource(InputPort);
		#else
                msAPI_AudioSourceSel(InputPort);
		#endif
              }
        #endif
		#if ENABLE_DAC
                msAPI_AudioSetEarphoneMute(TRUE);
                if(AUDIO_MUTE_FLAG)
                {
                msAPI_AudioSetMute( AUDIO_SOFTWARE_MUTE, E_SW_TIMING_CHANGE_MUTE_ON);
		  msDrvAudio_SetDPGAMuteSrc(E_DPGA_SOURCE_CHANGE_MUTE_ON);			
                }
		  else		
		  {
                msAPI_AudioSetMute( AUDIO_SOFTWARE_MUTE, E_SW_TIMING_CHANGE_MUTE_OFF);
		  msDrvAudio_SetDPGAMuteSrc(E_DPGA_SOURCE_CHANGE_MUTE_OFF);		
		  }
		#endif
		
                SetAudioMuteOnOff();
		
                msAPI_AdjustVolume(USER_PREF_VOLUME);
		
        }
        else
        {
             if(g_bAPPWindow_Init&&CB_FuncList[eCB_CustomerSetModeChangeAudioMuteOnOff]!=NULL)
	       ((fpCustomerSetModeChangeAudioMuteOnOff)CB_FuncList[eCB_CustomerSetModeChangeAudioMuteOnOff])();
             else
             {
            if (audio_EnableAudioAfterSignalLock_Flag==AUDIO_MUTE)
                return;
                 printMsg("33333333333333333333333333");
		   audio_EnableAudioAfterSignalLock_Flag=AUDIO_MUTE;		
		   hw_SetMute();		
		   hw_Clr_SPIFGPIO_EN_Pin();
         	#if ENABLE_DAC
                 msAPI_AudioSetDpgaMute(AUDIO_DPGA_ALL, TRUE);
                 msAPI_AudioSetEarphoneMute(TRUE);
                 msAPI_AudioSetMute( AUDIO_SOFTWARE_MUTE, E_SW_TIMING_CHANGE_MUTE_ON);
 		#endif
 		#if ENABLE_DAC
                 msDrvAudio_SetDPGAMuteSrc(E_DPGA_SOURCE_CHANGE_MUTE_ON);
 		#endif
              }
         
         }
		
}
#endif // end of #if AudioFunc

void mStar_ModeHandler(BYTE SCDetWin)
{
    BYTE    InputPort = MapScaler2Port(SCDetWin);

#if LOGO_ENABLE
#if (!ZUI)
    if (g_stMenuFlag.bDisplayLogo)
        return;
#else
    if (g_stZuiMenuFlag.bDisplayLogo)
        return;
#endif
#endif


    if( !SyncLossState(InputPort) ) // has sync
    {
        SettingInputColorimetry(MapScaler2Win(SCDetWin));

        if( !UnsupportedModeFlag(InputPort) ) //supported mode
        {
#if BrightFreqByVfreq
            SetFourtimesPWMFreq(InputPort);          //111115 modified Set PWMFreq before Setupmode - A073, A074
#endif

            WAIT_V_OUTPUT_BLANKING_START();
            MENU_LOAD_START();
            if( msDrvSetupMode(SCDetWin) == FALSE ) // setup scaler, FALSE: timing change
            {
                Set_InputTimingChangeFlag(InputPort); // setup mode failed
                drvmStar_SetupFreeRunMode(SCDetWin); // setup freerun mode
                return;
            }
            MENU_LOAD_END();
	#if (defined(KTC_Project))
	  if(BURNIN_MODE_FLAG)
	  {
	    CLR_BURNIN_MODE_FLAG();  
           SaveMonitorSetting();
	  }
	#endif		
        }
    }

    if( SyncLossState(InputPort) || UnsupportedModeFlag(InputPort) )
    {
        drvmStar_SetupFreeRunMode(SCDetWin); // setup freerun mode
    }
    else
    {
        // set status to SETUPOK if input mode is supported
        g_ScalerInfo[SCDetWin].eSiDispStatus = SC_STATUS_OUTPUT_SETUPOK;

    }

    // Vbus charge
   if(INPUT_IS_MHL(InputPort))
   {
    if((!SyncLossState(InputPort))&&(!UnsupportedModeFlag(InputPort)) )
      {
           if( _mdrv_mhl_GetDisplayReadyFlag(InputPort))
           {
               _mhal_mhl_SetVbusCharge(InputPort, TRUE);
           }
           else
           {
               _mhal_mhl_SetVbusCharge(InputPort, FALSE);
           }
      }

   }



    if(UnsupportedModeFlag(InputPort) )
        g_ScalerInfo[SCDetWin].eSiDispStatus = SC_STATUS_OUTPUT_SETUPOK;

    if(/* !CableNotConnectedFlag(InputPort) ||*/ !SyncLossState(InputPort) )
    {
        if( DoBurninModeFlag )
        {
            Clr_DoBurninModeFlag();
        #if ENABLE_MULTI_WINDOW_SETTINGS_MENU
            if (USER_PREF_MULTI_WIN_MODE != eMULTIWIN_OFF)
            {
                BYTE u8win;
                for(u8win=0; u8win<DISPLAY_MAX_NUMS; u8win++)
                {
                    msAPIWinEnableFrameColor(u8win, FALSE);
		      msAPI_AdjustBackgoundColor(u8win, ePATTERN_COLOR_BLACK);			
                }
		 
            }
            else
        #endif
            {
                msAPIWinEnableFrameColor(DISPLAY_WIN1, FALSE);
		  msAPI_AdjustBackgoundColor(DISPLAY_WIN1, ePATTERN_COLOR_BLACK);		
            }
        }
    }
    Clr_DoModeSettingFlag(InputPort);
	

    if (!SyncLossState(InputPort) && !UnsupportedModeFlag(InputPort))
    {
#if 0//ENABLE_MULTI_INPUT
        if (InputPort!=USER_PREF_INPUT_TYPE)
            g_stMenuFlag.bDisplayInputMsg=1;
#endif
#if ENABLE_MULTI_INPUT
        if (InputPort !=USER_PREF_WIN_SRC(MapScaler2Win(SCDetWin)))//USER_PREF_INPUT_TYPE)
        {
        	g_stMenuFlag.bDisplayInputMsg[MapScaler2Win(SCDetWin)]=1;
        }	
#endif
        	g_stMenuFlag.bDisplayInputNoSignalMsg[MapScaler2Win(SCDetWin)]=0;
     }
    else
     {
            g_stMenuFlag.bDisplayInputNoSignalMsg[MapScaler2Win(SCDetWin)]=1;
     }

#if 0//!PowerSequence_New	
#if AudioFunc
    //tmp need update !! audio out decided by AP
    mStar_SettingAudioFucntion( InputPort, SCDetWin );
#endif
#endif
	
    //tmp need update!!! decided by SCL0_M?? or 1st sc
if((SyncLossState(InputPort) || UnsupportedModeFlag(InputPort))&&(!InputTimingChangeFlag(InputPort)))
{

#if AIO_Board
 if(SyncLossState(InputPort)
#if SwitchPortByDP_Detect
 &&(USER_PREF_INPUT_TYPE==INPUT_HDMI)
#endif
 )
   {
    if(BURNIN_MODE_FLAG)
    {
      if(!bPanelOnFlag)
      {
        Power_TurnOnPanel(); // turn on panel
      }
      else      
     #if (VBY1_PowerSequence)
      if(!BackLightOnFlag)
      {
        printMsg("Set_BackLightOnFlag");
        Set_BackLightOnFlag();
        g_stMenuFlag.bAnyWinDisplayChange=1;
      }
     #else
       if((!BacklightFlag))
       {
          printMsg("BACKLIGHTT_FLAG");
          hw_SetBlacklit();	
          g_stMenuFlag.bAnyWinDisplayChange=1;
       }
     #endif
    }
       #if (!ZUI)
	 if((g_u8MenuPageIndex != BURNIN_MENU)&&(g_u8MenuPageIndex != MENU_STANDBY)&&(g_u8MenuPageIndex != MENU_ENTERPOWERSAVING))
         Menu_InitAction(); // initial menu osd state
      #endif
   }
 else
#endif
  {
#if ENABLE_DAISY_CHAIN
    if(CURRENT_INPUT_IS_DC_RX())
    {
        if(!PowerSavingFlag && (!g_u8DCMSTFlag || (!SyncLossState(InputPort) && InputTimingStableFlag(InputPort))))
        {
            Power_TurnOnPanel(); // turn on panel
            g_u8DCMSTFlag = 0;
        }
    }
    else
    {
        Power_TurnOnPanel(); // turn on panel
    }
#else

  if(!bPanelOnFlag)
  {
    #if ENABLE_MULTI_WINDOW_SETTINGS_MENU
        if (USER_PREF_MULTI_WIN_MODE != eMULTIWIN_OFF)
        {
            BYTE u8win;
            for(u8win=0; u8win<DISPLAY_MAX_NUMS; u8win++)
            {
                msAPIWinEnableFrameColor(u8win, FALSE);
	      msAPI_AdjustBackgoundColor(u8win, ePATTERN_COLOR_BLACK);			
            }
	 
        }
        else
    #endif
        {
            msAPIWinEnableFrameColor(DISPLAY_WIN1, FALSE);
	  msAPI_AdjustBackgoundColor(DISPLAY_WIN1, ePATTERN_COLOR_BLACK);		
        }  
    Power_TurnOnPanel(); // turn on panel
  }
  else   
#endif
   #if (VBY1_PowerSequence)
    if(!BackLightOnFlag)
    {
    #if ENABLE_MULTI_WINDOW_SETTINGS_MENU
        if (USER_PREF_MULTI_WIN_MODE != eMULTIWIN_OFF)
        {
            BYTE u8win;
            for(u8win=0; u8win<DISPLAY_MAX_NUMS; u8win++)
            {
                msAPIWinEnableFrameColor(u8win, FALSE);
	      msAPI_AdjustBackgoundColor(u8win, ePATTERN_COLOR_BLACK);			
            }
	 
        }
        else
    #endif
        {
            msAPIWinEnableFrameColor(DISPLAY_WIN1, FALSE);
	  msAPI_AdjustBackgoundColor(DISPLAY_WIN1, ePATTERN_COLOR_BLACK);		
        }  
    
      printMsg("Set_BackLightOnFlag");
      Set_BackLightOnFlag();
      g_stMenuFlag.bAnyWinDisplayChange=1;
    }
   #else
     if((!BacklightFlag))
     {
    #if ENABLE_MULTI_WINDOW_SETTINGS_MENU
        if (USER_PREF_MULTI_WIN_MODE != eMULTIWIN_OFF)
        {
            BYTE u8win;
            for(u8win=0; u8win<DISPLAY_MAX_NUMS; u8win++)
            {
                msAPIWinEnableFrameColor(u8win, FALSE);
	      msAPI_AdjustBackgoundColor(u8win, ePATTERN_COLOR_BLACK);			
            }
	 
        }
        else
    #endif
        {
            msAPIWinEnableFrameColor(DISPLAY_WIN1, FALSE);
	  msAPI_AdjustBackgoundColor(DISPLAY_WIN1, ePATTERN_COLOR_BLACK);		
        }  
     
        printMsg("BACKLIGHTT_FLAG");
        hw_SetBlacklit();	
        g_stMenuFlag.bAnyWinDisplayChange=1;
     }
   #endif
  }   
}

   

}


void mStar_PrepareForTimingChange( BYTE InputPort)
{
    DETECT_PRINT( "PrepareForTimingChange SrcInputPort=%d\n", InputPort );

    Clr_ForcePowerSavingFlag();
    g_u8PowerDownCounter = 0;
    InputColorFormat(InputPort) = INPUTCOLOR_RGB;
    if(g_u8MenuPageIndex==FACTORY_MENU)
         g_u8MenuPageIndex=MENU_ROOT;

    if((g_u8MenuPageIndex!=MENU_HOTINPUT)&&(!OSDFreeSyncActionFlag))	
         g_u8MenuItemIndex=0;
	
#if (!ZUI)
    if((!g_stMenuFlag.bShowInputIcon)
#if LOGO_ENABLE
    &&(!g_stMenuFlag.bDisplayLogo)
#endif		
      )
    {
        Menu_ClearOsdCounterAndFlag();
        if ((!PowerSavingFlag))
        {
   #if ENABLE_MULTI_WINDOW_SETTINGS_MENU     
       if((USER_PREF_MULTI_WIN_MODE != eMULTIWIN_OFF)&&(g_ScalerInfo[DISPLAY_WIN1].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN2].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN3].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN4].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK))
        ;
	else   
   #endif		
   #if 0//Free_Sync_Enable     
       if((OSDFreeSyncActionFlag)&&(OSDShowFlag))
        ;
	else   
   #endif		
	{
         #if ((PowerSequence_New&&(!defined(PanelI2CCommandSetting)))||(PanelType==PanelM320QAN01)||(PanelType==PanelLSM270HP06))
	    #if (PanelType==PanelLSM315HP01)	 
	   if(FreeSyncFlag)	 
	   {
           if(IS_BackLight_On())
		  hw_ClrBlacklit();
	   }
	   else
	   #endif	
	   	Power_TurnOffPanel();
	  #else		
           if(IS_BackLight_On())
		  hw_ClrBlacklit();
         #endif

		#if (VBY1_PowerSequence)		
		  PowerOnBackLightCounter=0;
		#endif
       }	

      #if 0//Free_Sync_Enable     
       if((OSDFreeSyncActionFlag)&&(OSDShowFlag))
        ;
	else   
   #endif		
            Menu_Hide();
   
            msAPISrcFreerunFuncUpdate(InputPort);  // input port freerun ==> all scaler on it freerun
            	g_NotSettingLPLL=TRUE;  //  160714_11	

        }
    }
#else
 #if LOGO_ENABLE
    if(!g_stZuiMenuFlag.bDisplayLogo)
 #endif		
    {
        if (!PowerSavingFlag)
        {
            #if 0
            Power_TurnOffPanel();
            #endif
            msAPISrcFreerunFuncUpdate(InputPort);  // input port freerun ==> all scaler on it freerun
        }
    }
#endif

#if UseINT
    mStar_EnableModeChangeINT(FALSE, BIT0<<MapPort2DetScIndex(InputPort));
#endif

    if( InputTimingStableFlag(InputPort) )
    {
        if( MapPort2DetScIndex(InputPort) & BIT0)//main window
            msDrvSoftwareReset( RST_IP_MAIN,  MapPort2DetScIndex(InputPort));
        else
            msDrvSoftwareReset( RST_IP_SUB, MapPort2DetScIndex(InputPort));
        Delay1ms(80);
    }
    #if ENABLE_DP_INPUT
    if( g_InputPort[InputPort].eIPType & TYPE_DP )
    msAPI_combo_IPDPClrMSAChg((EN_COMBO_IP_SELECT)g_InputPort[InputPort].eIPMux);
    #endif
    msDrvIP1SetImageVStart( 0, MapPort2DetScIndex(InputPort));

    Clr_InputTimingStableFlag(InputPort); // reset input timing stable and wait for timing stable
    InputTimingStableCounter(InputPort) = 0;
    SrcFlags(InputPort) &= ~( bUnsupportMode | bUserMode | bNativeMode );
    CLR_DISPLAY_UNSUPPORT_MODE_FLAG();
    Clr_FreeSyncMsgFlag();
	
#if ENABLE_MULTI_WINDOW_SETTINGS_MENU
   if(USER_PREF_MULTI_WIN_MODE == eMULTIWIN_OFF)
#endif
    g_u8SwitchPortCounter = 0; // sync locked, counter reset to 0

    //_msWriteByte(SC0_01, 0x00);        // disable double bufer.
    mStar_ScalerDoubleBuffer(FALSE);
    //mStar_WriteByte(REGBK, REGBANKADC);
    //_msWriteByte(SC0_01, 0x00);        // enable ADC's double bufer.
    //mStar_WriteByte(REGBK, REGBANKSCALER);

    // clear ScMaskIn ONLY scaler. e.g. when timing change in <HBR2 output window L/R only case>.
    msDrvSrcInOnlySCUpdate(InputPort, DISPLAY_MAX_NUMS);

    Clr_OSDActionFlag();
    BlackScreenCounter=0;	

//    g_stMenuFlag.bDisplayInputMsg[DISPLAY_WIN1]=1;

#if ENABLE_HDMI_DRR
    if(INPUT_IS_HDMI(InputPort))
    {
        BYTE u8SCMask = MapPort2ScMask(InputPort);
        BYTE u8SCIdx = 0;

        while(u8SCMask!=0)
        {
            if(u8SCMask & BIT0)
                g_ScalerInfo[u8SCIdx].eSiIPHDMIDRRChk = TRUE;

            u8SCMask >>=1;
            u8SCIdx++;
        }

        msDrvIPVsyncINTEnable(ENABLE);
    }
#endif
}
//==========================================================

#define DISABLE_CABLEDETECT  ENABLE_MULTI_INPUT//101215 Modify
Bool mStar_SyncLossStateDetect( BYTE InputPort)
{
    BYTE fStatus;
    Bool result = FALSE;

    fStatus = mStar_GetInputStatus( InputPort );
    //printData("status %xH", fStatus);//_msReadByte(INSTA));
    //printData("stable counter %d", InputTimingStableCounter);
    if( fStatus & SyncLoss )
    {
    #if ENABLE_VGA_INPUT
        if(INPUT_IS_VGA(InputPort))//( MapScaler2Port < Input_Digital )
        {
            if( InputTimingStableCounter(InputPort) % SyncValidThd(InputPort) == 0 )
                mStar_SetAnalogInputPort(InputPort, (g_bInputSOGFlag?FALSE:TRUE)); //tmp need update??

            fStatus = mStar_GetInputStatus( InputPort );
            if( !( fStatus & SyncLoss ) )
                goto SyncActive;
        }
    #endif


	
        if( CableNotConnectedFlag(InputPort) )  // no Cable
        {
            if( msAPISrcIsCableConnected(InputPort) )
            {
                Clr_CableNotConnectedFlag(InputPort);
            #if DISABLE_CABLEDETECT
            #else
                g_u8SwitchPortCounter = 0;
                result = TRUE;
            #endif
            }
        }
        else if( !msAPISrcIsCableConnected(InputPort) ) // no Sync
        {
            Set_CableNotConnectedFlag(InputPort);
        #if DISABLE_CABLEDETECT
        #else
            Power_TurnOnGreenLed();
            result = TRUE;
        #endif
        }


		
    }
    else
    {
#if ENABLE_VGA_INPUT
    SyncActive:
#endif
        result = TRUE;
    }
    if( result )
    {
        SrcFlags(InputPort) = ( SrcFlags(InputPort) & 0xF0 ) | fStatus;
    } // clear others flags, except sync informat (polarity & w/o)
    return result;
}

Bool mStar_ValidTimingDetect( BYTE InputPort )
{
    BYTE fStatus;
    BYTE SCDetWin = MapPort2DetScIndex(InputPort);
    ST_WINDOW_INFO  stAutoWin;

    fStatus = mStar_GetInputStatus( InputPort );
    if( fStatus & SyncLoss )        // no sync
    {
        SrcFlags(InputPort) = ( SrcFlags(InputPort) & 0xF0 ) | fStatus; // clear others flags, except sync informat (polarity & w/o)

        DETECT_PRINT( "Valid Timing : Sync Loss\n" );

        return TRUE;
    }
    else
    {
        if( SyncPolarity( SrcFlags(InputPort) ) != SyncPolarity( fStatus ) )            // Sync polarity changed
        {
            DETECT_PRINT( "Valid Timing : Sync polarity changed: %x, %x\n", fStatus, SrcFlags(InputPort) );
            SrcFlags(InputPort) = ( SrcFlags(InputPort) & 0xF0 ) | fStatus; // clear others flags, except sync informat (polarity & w/o)
            return TRUE;
        }
        else
        {
            WORD tempPeriod;
            SrcFlags(InputPort) = ( SrcFlags(InputPort) & 0xF0 ) | fStatus; // clear others flags, except sync informat (polarity & w/o)
            tempPeriod = msDrvIP1ReadHperiod(SCDetWin);//_msRead2Byte(SC0_E4) & MASK_13BIT;
            if( InputTimingStableCounter(InputPort) == 0 )
            {
                SrcHPeriod(InputPort) = tempPeriod;
            }
            // HPeriod changed

            if(!INPUT_IS_DP_DRR(InputPort))
            {
                if( labs( (DWORD)tempPeriod - SrcHPeriod(InputPort) ) > ((INPUT_IS_DISPLAYPORT(InputPort))?HPeriod_Torlance*8:HPeriod_Torlance) )
                {
                    tempPeriod = msDrvIP1ReadHperiod(SCDetWin);//_msRead2Byte(SC0_E4) & MASK_13BIT;

                    if( labs( (DWORD)tempPeriod - SrcHPeriod(InputPort) ) > ((INPUT_IS_DISPLAYPORT(InputPort))?HPeriod_Torlance*8:HPeriod_Torlance) )
                    {
                        DETECT_PRINT( "HPeriod changed : %x, %x\n",tempPeriod, SrcHPeriod(InputPort) );
                        return TRUE;
                    }
                }
            }

                tempPeriod = msDrvIP1ReadVtotal(SCDetWin);//_msRead2Byte(SC0_E2) & MASK_11BIT;
                if( InputTimingStableCounter(InputPort) == 0 )
                {
                    // 100902 coding addition
                    //Delay1ms( 100 );
                    tempPeriod = msDrvIP1ReadVtotal(SCDetWin);//_msRead2Byte(SC0_E2) & MASK_11BIT;
                    SrcVTotal(InputPort) = tempPeriod;
                }
            if(!INPUT_IS_HDMI_DRR(InputPort)) 
            {
            // vtotal changed
                if( abs( tempPeriod - SrcVTotal(InputPort) ) > VTotal_Torlance )
                {
#if ENABLE_HDMI_INPUT // 100902 coding addition
                    Delay1ms( 10 );
                    tempPeriod = msDrvIP1ReadVtotal(SCDetWin);//_msRead2Byte(SC0_E2) & MASK_11BIT;
                    if( labs( (DWORD)tempPeriod - SrcVTotal(InputPort) ) > VTotal_Torlance ) // vtotal changed
#endif
                    {
                        DETECT_PRINT( "Vtotal changed : %x, %x\n",tempPeriod, SrcVTotal(InputPort) );
                        return TRUE;
                    }
                }
            }

#if ENABLE_DP_INPUT
     if( g_InputPort[InputPort].eIPType & TYPE_DP )
     {
              if(msAPI_combo_IPDPCheckMSAChg((EN_COMBO_IP_SELECT)g_InputPort[InputPort].eIPMux))
                return TRUE;
     }
#endif

             mStar_NewModeInterlacedDetect(InputPort);

            // interlace field setting, not for timing change polling
            tempPeriod = msDrvSrcIsInterlaceMode(InputPort);
            if( ( InputTimingStableCounter(InputPort) == 0 )
              ||( tempPeriod && !InterlaceModeFlag(InputPort) )
              ||( !tempPeriod && InterlaceModeFlag(InputPort) ))
            {
                DETECT_PRINT( "Interlace Flag Changed/Initialized : %x, %x, Init:%x\n",tempPeriod, InterlaceModeFlag(InputPort), (InputTimingStableCounter(InputPort)==0) );

                if( tempPeriod )
                    SrcFlags(InputPort) |= bInterlaceMode;
                else
                    SrcFlags(InputPort) &= ~bInterlaceMode;

                msDrvScSetFieldDetect(SCDetWin);
            }

            // TMDS H/V start,  H/V DE(de only mode)
            if(( INPUT_IS_TMDS(InputPort) ) && !FreeRunModeFlag(SCDetWin) &&!INPUT_IS_HDMI_DRR(InputPort) )
            {
                tempPeriod = msDrvIP1ReadAutoHStart(SCDetWin);//_msRead2Byte( SC0_80 ) & MASK_12BIT;
                if( labs( (DWORD)tempPeriod - SrcAutoHstart(InputPort) ) > HVStartTorlance )
                {
                    DETECT_PRINT( "Hstart changed : %x, %x\n",tempPeriod, SrcAutoHstart(InputPort) );
                    return TRUE;
                }
                tempPeriod = msDrvIP1ReadAutoVStart(SCDetWin);//_msRead2Byte( SC0_7E ) & MASK_11BIT;
                if( labs( (DWORD)tempPeriod - SrcAutoVstart(InputPort) ) > HVStartTorlance )
                {
                    DETECT_PRINT( "Vstart changed : %x, %x\n",tempPeriod, SrcAutoVstart(InputPort) );
                    return TRUE;
                }

                if(msDrvIP1IsDEOnlyMode(SCDetWin) == TRUE) // TMDS DE only mode
                {
                    stAutoWin = mStar_ReadAutoWindow(InputPort);
                    if( labs( (DWORD)stAutoWin.width - g_InputPort[InputPort].sIPAutoWin.width) > HVDETorlance )
                    {
                        DETECT_PRINT( "HDE changed : %x, %x\n",stAutoWin.width, g_InputPort[InputPort].sIPAutoWin.width );
                        return TRUE;
                    }
                    if( labs( (DWORD)stAutoWin.height- g_InputPort[InputPort].sIPAutoWin.height) > HVDETorlance )
                    {
                        DETECT_PRINT( "VDE changed : %x, %x\n",stAutoWin.height, g_InputPort[InputPort].sIPAutoWin.height );
                        return TRUE;
                    }
                }
            }
        }
    }
    return FALSE;
}

#if  MS_VGA_SOG_EN
BYTE GetVSyncWidth( BYTE InputPort)
{
    BYTE u8VSyncWidth;
    BYTE u8DetSCIndex = MapPort2DetScIndex(InputPort);

    u8VSyncWidth=msDrvIP1ReadVSyncWidth(u8DetSCIndex);//_msReadByte(SC0_E2);
    //PRINT_DATA("u8VSyncWidth=%d",u8VSyncWidth);
    return u8VSyncWidth;
}
#endif

BYTE mStar_GetInputStatus( BYTE InputPort )
{
    BYTE fStatus = 0;
    WORD inputValue = 0;
    BYTE status;
    BYTE u8DetSCIndex = MapPort2DetScIndex(InputPort);

    if(!INPUT_IS_DP_DRR(InputPort))
    {
        inputValue = msDrvIP1ReadHperiod(u8DetSCIndex);
        if( inputValue == SC_MASK_IP1_HPD|| inputValue < 10 )
        {
            fStatus |= bHSyncLoss;
        }
    }

    inputValue = msDrvIP1ReadVtotal ( u8DetSCIndex );

    if(!INPUT_IS_HDMI_DRR(InputPort))
    {
        if( inputValue == SC_MASK_IP1_HVTT || inputValue < 200 )
        {
            fStatus |= bVSyncLoss;
        }
    }
	else
	{
	    if (inputValue < 200 )
	    {
	        fStatus |= bVSyncLoss;
	    }
	}

    status = msDrvIP1ReadSyncStatus(u8DetSCIndex);//_msReadByte(SC0_E1);

    fStatus |= ( status & 0x03 ); // Get input timing polarity

#if 0
    //tmp need update
    if(INPUT_IS_TMDS(InputPort))// input timing is valid while current state is no sync
    {
        if(DVI_DE_STABLE() == FALSE)
        {
            if(DVI_DE_STABLE() == FALSE)
            {
                fStatus |= SyncLoss;
                #if DETECT_DEBUG
                DETECT_PRINT("DVI DE Unstable\n", 1);
                #endif
            }
        }
    }
    else
#endif
    if( SyncLossState(InputPort) && !( fStatus & SyncLoss ) )        // input timing is valid while current state is no sync
    {
        Delay1ms( 20 );
        if( InputTimingChangeFlag(InputPort) )
        {
            return fStatus;
        }
        status = status & msDrvIP1ReadSyncStatus(u8DetSCIndex);//_msReadByte(SC0_E1);
        if(( status & 3 ) != ( fStatus & 3 ) )
        // polarity is not stable
        {
            fStatus |= SyncLoss;
        }
        else if( status & 0x30 )
            // SOG or CSync input
        {
            Delay1ms( 20 );
            if( InputTimingChangeFlag(InputPort) )
            {
                return fStatus;
            }
            status = msDrvIP1ReadSyncStatus(u8DetSCIndex);//_msReadByte(SC0_E1);           // Check if SOG/CSYNC is valid
            if( labs( (DWORD)msDrvIP1ReadVtotal(u8DetSCIndex) - inputValue ) > V_IntTorlance )//( abs( (_msRead2Byte( SC0_E2 )&MASK_11BIT) - inputValue ) > 2 )
            {
                fStatus |= SyncLoss;
            }
#if MS_VGA_SOG_EN
            else if (status&SOGD_B)
            {
                #if 1
                if(!SyncLossState(InputPort))
                 {
                    //BYTE i;
                    //for (i=0;i<10;i++)
                    {
                        inputValue=GetVSyncWidth(InputPort);
                        if (inputValue>15||inputValue==0)
                        {
                            //printMsg("SOG Det Err");
                            fStatus |= SyncLoss;
                            //break;
                        }
                        //ForceDelay1ms(2);
                    }
                }
                #else
                {
                    BYTE u8Tmp0=SC0_READ_POWER_DOWN_STATUS();//_msReadByte(SC0_F0);
                    BYTE u8Tmp1=SC0_READ_ADC_COAST_ENABLE_VALUE();//_msReadByte(SC0_ED);
                    BYTE u8Tmp2=SC0_READ_ADC_COAST_START_VALUE();//_msReadByte(SC0_EE);
                    BYTE u8Tmp3=SC0_READ_ADC_COAST_END_VALUE();//_msReadByte(SC0_EF);
                    BYTE i;
                    //for get stable VSyncWidth
                    SC0_SCALER_POWER_DOWN(0, BIT0|BIT1); //to get vsyncWidth
                    SC0_ADC_COAST_ENABLE(0x21);//_msWriteByte(SC0_ED, 0x21); //_NEW_SOG_WAKEUP_DET_
                    SC0_ADC_COAST_START(0x08);//_msWriteByte(SC0_EE, 0x08);
                    SC0_ADC_COAST_END(0x05);//_msWriteByte(SC0_EF, 0x05);
                    Delay1ms(20);
                    inputValue=0;
                    for (i=0;i<10;i++)
                    {
                        inputValue=GetVSyncWidth();
                        if (inputValue>15||inputValue=0)
                        {
                      //printMsg("SOG Det Err");
                            fStatus |= SyncLoss;
                            break;
                        }
                        ForceDelay1ms(2);
                    }
                    SC0_ADC_COAST_ENABLE(u8Tmp1);//_msWriteByte(SC0_ED, u8Tmp1);
                    SC0_ADC_COAST_START(u8Tmp2);//_msWriteByte(SC0_EE, u8Tmp2);
                    SC0_ADC_COAST_END(u8Tmp3);//_msWriteByte(SC0_EF, u8Tmp3);
                    SC0_SCALER_POWER_DOWN(u8Tmp0, 0xFF);
                }
                #endif
            }

#endif
        }
    }
    return fStatus;
}


void mStar_SetHVSyncStatusCheck(MS_BOOL bEnable, WORD u16SCMask)
{
    msDrvIP1SetHVSyncCheck(bEnable, u16SCMask);
}

void mStar_SetNewModeInterlacedDetect(MS_BOOL bEnable, MS_U8 u8Source, MS_BOOL bVttByPixelClk, MS_BOOL bVInvert, MS_BOOL bHInvert, WORD u16SCMask)
{

    msDrvIP1SyncSourceSelect(u8Source, u16SCMask);
    msDrvIP1SetVttCountbyPixel(bVttByPixelClk, u16SCMask);
    msDrvIP1SetVsyncInvert(bVInvert, u16SCMask);
    msDrvIP1SetHsyncInvert(bHInvert, u16SCMask);
    msDrvIP1SetNewInterlaceDetect(bEnable, u16SCMask);
}
////////////////////////////////////////////////////////////
//  //108oi   SrcHPeriod = 384 SrcVTotal = 1250           //
////////////////////////////////////////////////////////////
void mStar_NewModeInterlacedDetect(BYTE InputPort)
{
    static BYTE _NewModeInterlacedDetect = 0;  //one protindex by one bit
    WORD u8SCDetWin = MapPort2DetScIndex(InputPort);
    WORD u16SCMask = MapPort2ScMask(InputPort);
    // enable H/V sync status check


    if(INPUT_IS_HDMI(InputPort) || INPUT_IS_DVI(InputPort))
    {
        mStar_SetHVSyncStatusCheck(ENABLE, u16SCMask);
        WORD temp_width = msDrvIP1ReadAutoWidth(u8SCDetWin);
        WORD temp_height = msDrvIP1ReadAutoHeight(u8SCDetWin);

        if((temp_width > 1915) && (temp_width < 1925)
            && (temp_height > 535) && (temp_height < 545)
            && ( SrcHPeriod(InputPort) > 374) && ( SrcHPeriod(InputPort) < 394)//&& ( SrcHPeriod(InputPort) > 305) && ( SrcHPeriod(InputPort) < 315)
            && (SrcVfreq(InputPort) > 498) && (SrcVfreq(InputPort) < 502)
            && (SrcVTotal(InputPort) > 620) && (SrcVTotal(InputPort) < 630)
            && (!InterlaceModeFlag(InputPort)))
        {
            // enable new mode interlaced detect
            bHDMI108oiTimingFlag=1; //20160309
            mStar_SetNewModeInterlacedDetect(ENABLE, 2, ENABLE, TRUE, FALSE, u16SCMask);
            _NewModeInterlacedDetect |=(BYTE)(_BIT0<<InputPort); //_bNewModeInterlacedDetect = TRUE;
            Delay1ms(50);
        }
        else if((_NewModeInterlacedDetect & (_BIT0<<InputPort))
            && (!((SrcHPeriod(InputPort) > 374) && (SrcHPeriod(InputPort) < 394)//&& (!((SrcHPeriod(InputPort) > 305) && (SrcHPeriod(InputPort) < 315)
            && (SrcVfreq(InputPort) > 498) && (SrcVfreq(InputPort) < 502)
            && (SrcVTotal(InputPort) > 1245) && (SrcVTotal(InputPort) < 1255)
            && (InterlaceModeFlag(InputPort)))))
        {
            // disable new mode interlaced detect
            bHDMI108oiTimingFlag=0; //20160309
            mStar_SetNewModeInterlacedDetect(DISABLE, 0, DISABLE, FALSE, FALSE, u16SCMask);
            _NewModeInterlacedDetect &=(BYTE)(~(_BIT0<<InputPort));//_bNewModeInterlacedDetect = FALSE;
        }
    }
    else
    {
        mStar_SetNewModeInterlacedDetect(DISABLE, 0, DISABLE, FALSE, FALSE, u16SCMask);
    }
}

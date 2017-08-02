
#define _APPSYSTEM_C_
#include <string.h>
#include "Global.h"
#include "appSystem.h"
#include "Power.h"
#include "UserPref.h"
#include "NVRam.h"
#if (!ZUI)
#include "Menu.h"
#else
#include "MApp_ZUI_ACTglobal.h"
#endif
#if ENABLE_DAISY_CHAIN
#include "msDaisyChain.h"
#endif
#include "MenuFunc.h"
#include "Menu.h"
#include "drvAudio.h"

#define APPSYSTEM_DEBUG    1
#if ENABLE_MSTV_UART_DEBUG && APPSYSTEM_DEBUG
#define APPSYSTEM_PRINT(format, ...)     printf(format, ##__VA_ARGS__)
#else
#define APPSYSTEM_PRINT(format, ...)
#endif

////////////////////////////////////////////////////////////////////////
#if ENABLE_DAISY_CHAIN
void APPSystem_DCWakeup(void)
{	
	extern BYTE tCOMBO_HDCP_KEY_TABLE[], tCOMBO_HDCP_BKSV[];
    BYTE u8Retry=10, u8SendData=0, u8ReplyData=0;

    while(!(msDCSetCmdWakeup(u8SendData, &u8ReplyData)) && --u8Retry)
    {
        APPSYSTEM_PRINT("[appSystem] DC Setup Retry %d\n",u8Retry);
    }
    if( (u8Retry==0) || (u8ReplyData&BIT0)==0x00 )
    {
        APPSYSTEM_PRINT("[appSystem] DC Setup NG\n");
    }
    else
    {
        APPSYSTEM_PRINT("[appSystem] DC Setup OK\n");
    }

	if (msDCCheckRxHDCP() == FALSE)
		msDCLoadHDCP(tCOMBO_HDCP_KEY_TABLE, tCOMBO_HDCP_BKSV);
}
#endif
/////////////////////////////////////////////////////////////////////////
/////////////////////////////  Enter PM DPMS  /////////////////////////////
/////////////////////////////////////////////////////////////////////////
void APPSystem_CheckDPMS( void )
{
#if ENABLE_DAISY_CHAIN
    BYTE u8Retry=10, u8SendData=0, u8ReplyData=0;
#endif

    if( ForcePowerSavingFlag )
    {
       #if Enable_AutoInput_Menu
        #if 0//(ENABLE_DP_INPUT && (!ENABLE_DAISY_CHAIN))
        if((USER_PREF_INPUT_PRIORITY != INPUT_PRIORITY_AUTO)&&CURRENT_INPUT_IS_DISPLAYPORT()) 
        {
            if(msAPI_combo_IPDPCheckIgnoreDPMS((EN_COMBO_IP_SELECT)g_InputPort[SrcInputPortM].eIPMux)) // Do not into power saving when CTS
            {
                Clr_ForcePowerSavingFlag();
                return;
            }
        }
        #endif
       #endif		

#if ENABLE_DAISY_CHAIN  // if Daisy Chain is in MST mode and wait 5 sec then check again.
        if(CURRENT_INPUT_IS_DC_RX())
        {
            if(g_u8DCMSTCheckFlag)
            {
                g_u8DCMSTCheckFlag = 0;
                while(!(msDCSetCmdGetSleepSel(u8SendData, &u8ReplyData)) && --u8Retry)
                {
                    APPSYSTEM_PRINT("[appSystem] DC GetSleepSel Retry %d\n",u8Retry);
                }                
            }
            else
            {
                return ;
            }

            // check Daisy Chain is in MST mode or not
            if(IS_MST_MODE(u8ReplyData)) // fake sleep
            {
                g_u8DCMSTFlag = 1;
                Power_TurnOffPanel();
                APPSYSTEM_PRINT("[appSystem] DC fake sleep flag: %d\n",g_u8DCMSTFlag);
                return ;
            }
        }
#endif

        Clr_ForcePowerSavingFlag();
        Clr_OSDFreeSyncActionFlag();
        if( !PowerSavingFlag )
        {
            if( POWER_ON_FLAG )
            {
             #if LED_HOT_Menu_Func
             if(IS_LED_Power())
               SetLedFunc(0xFF, 0xFF, 0xFF, 0xFF);
               Clr_LED_Power();
             #endif
            
                Set_PowerSavingFlag();
		  Power_TurnOffPanel();
		  
	         ForceDelay1ms(500);
#if Enable_AutoInput_Menu
                if(USER_PREF_INPUT_PRIORITY!=INPUT_PRIORITY_AUTO)
                {
                   g_sMailBoxR2.u16PMPortSkipIndex = (0xFFFF^(1<<(Get_SourceSwitchType())));
                   //g_sMailBoxR2.u16PMPortSkipIndex = (0xFFFF^(1<<(USER_PREF_INPUT_PRIORITY-ENABLE_AUTO_SELECT_SOURCE)));
                }
#endif    
                 #if EnableDdcLed 
                     g_sMailBoxR2.u16PMFlag=g_sMailBoxR2.u16PMFlag|((USER_PREF_LEDMODE==OffLedMode)?0:BIT0);
                 #else
                     g_sMailBoxR2.u16PMFlag= 0x0000;
                 #endif	  
		 if(USER_PREF_SLEEPMODE==SleepMode_Off)		 
                     g_sMailBoxR2.u16PMCounter= 0x0000;
		 else
                     g_sMailBoxR2.u16PMCounter= SleepModeCounter;
		 	

                g_sMailBoxR2.u16PMPortIndex = (WORD)SrcInputPortM;
                msAPI_PMSetMode(ePM_POWER_DPMS);
            }
        }
    }
}
/////////////////////////////////////////////////////////////////////////
////////////////////////// Check Power Status  /////////////////////////////
/////////////////////////////////////////////////////////////////////////
void APPSystem_CheckPowerStatus( void )
{
    //ePM_WakeupStatus ePMWakeupStatus = msAPI_PMGetWakeupStatus();
    ePMWakeupStatus = msAPI_PMGetWakeupStatus();

   printData("ePMWakeupStatus==%d",ePMWakeupStatus);
    if( ePMWakeupStatus == ePMSTS_NON ) // AC ON (no PM wakeup status)
    {
/*
     //======================================
     // AC ON (no PM wakeup status) --Start
     //======================================
           if(USER_PREF_FORCE_POWERSTATE==FORCEPOWERSTATE_OFF)
           {
              CLR_POWER_ON_FLAG();
	       NVRam_WriteWord(EEP_SYSTEM_MEMBER_ADDR(u16SYS_SystemFlag), MONITOR_FLAGS);
           }
	    else if(USER_PREF_FORCE_POWERSTATE==FORCEPOWERSTATE_ON)
	    {
              SET_POWER_ON_FLAG();
	       NVRam_WriteWord(EEP_SYSTEM_MEMBER_ADDR(u16SYS_SystemFlag), MONITOR_FLAGS);
	    }
*/	
    #if (!AIO_Board)         
        if( POWER_ON_FLAG ) // power ON
    #endif    
        {
            msAPI_PMSetMode(ePM_POWER_ON);
	     #if(CHIP_ID == MST9U4)	
	      SET_DP_SYSYEM_RUN_ENABLE();	  	
            #endif
            if ((!FACTORY_MODE_FLAG)&&(!BURNIN_MODE_FLAG))
            {
            }
            APPSYSTEM_PRINT("[appSystem] AC ON \n");
            g_eBootStatus = eBOOT_STATUS_ACON;
		  g_stMenuFlag.bDisplayInputMsg[DISPLAY_WIN1]=1;
        }
    #if (!AIO_Board)         
        else // power OFF
        {
            msAPI_PMSetMode(ePM_POWER_DCOFF);
            APPSYSTEM_PRINT("[appSystem] DC OFF \n");
            g_eBootStatus = eBOOT_STATUS_DCOFF;
        }
     #endif		
    }
    else // PM wakeup
    {
        if( ePMWakeupStatus == ePMSTS_POWERGPIO_ACT || ePMWakeupStatus == ePMSTS_MCCS01_ACT || ePMWakeupStatus == ePMSTS_FACTORY_ACT
     ||ePMWakeupStatus == ePMSTS_SAR_ACT
     ||ePMWakeupStatus==ePMSTS_KEYWAKEUP_ACT
	) // DC ON
        {
#if (!AIO_Board)         
          if((POWER_ON_FLAG)&&(ePMWakeupStatus == ePMSTS_POWERGPIO_ACT
     ||ePMWakeupStatus == ePMSTS_SAR_ACT
          ))
          {
              CLR_POWER_ON_FLAG();
              CLR_FACTORY_MODE_FLAG();
              CLR_BURNIN_MODE_FLAG();

              //g_stMenuFlag.bShowPMWakeupInputPort=0;
              //g_stMenuFlag.bShowPMWakeupInputPortActive=0;	
              msAPI_PMSetMode(ePM_POWER_DCOFF);
              APPSYSTEM_PRINT("[appSystem] Power saving to DC OFF \n");
              g_eBootStatus = eBOOT_STATUS_DCOFF;
              SaveNVRAMBlock(EEP_SYSTEM_MEMBER_ADDR(u16SYS_SystemFlag), sizeof(MONITOR_FLAGS));
          }
          else
#endif		  	
          {
              SET_POWER_ON_FLAG();
        #if (!LHCX_EnterFactoryMode)
              if(ePMWakeupStatus == ePMSTS_FACTORY_ACT)
              {
                 SET_FACTORY_MODE_FLAG();
		   SET_BURNIN_MODE_FLAG();		 
                 NVRam_WriteWord(EEP_SYSTEM_MEMBER_ADDR(u16SYS_SystemFlag), MONITOR_FLAGS);
              }
	 #endif		  
            
              
              APPSYSTEM_PRINT("[appSystem] DC ON \n");
              g_eBootStatus = eBOOT_STATUS_DCON;
            #if ENABLE_DAISY_CHAIN
              APPSystem_DCWakeup();
            #endif
              msAPI_PMSetMode(ePM_POWER_ON);
		SaveNVRAMBlock(EEP_SYSTEM_MEMBER_ADDR(u16SYS_SystemFlag), sizeof(MONITOR_FLAGS));	  
              //g_stMenuFlag.bAnyWinDisplayChange=1;	
              g_stMenuFlag.bDisplayInputMsg[DISPLAY_WIN1]=1;
              g_stMenuFlag.bDisplayInputNoSignalMsg[DISPLAY_WIN1]=1;
          }
        }
        else
        {
            // do something according to wakeup condition by CUS spec
            APPSYSTEM_PRINT("[appSystem] DPMS?? ON \n");
            g_eBootStatus = eBOOT_STATUS_DPMSON;
         #if ENABLE_DAISY_CHAIN
           APPSystem_DCWakeup();
         #endif
           msAPI_PMSetMode(ePM_POWER_ON);
   			
           //g_stMenuFlag.bAnyWinDisplayChange=1;	
           g_stMenuFlag.bDisplayInputMsg[DISPLAY_WIN1]=1;
           g_stMenuFlag.bDisplayInputNoSignalMsg[DISPLAY_WIN1]=1;
        }
    }

{
	BYTE uc;
	for(uc = 0;uc<4;uc++)
	{
		g_stMenuFlag.bDisplayInputMsg[uc] = 1;
 		g_stMenuFlag.bDisplayInputNoSignalMsg[uc] = 1;
	}
}

    if(g_eBootStatus == eBOOT_STATUS_DCOFF)
    {
          Power_TurnOffLed();
	   APPSYSTEM_PRINT("[appSystem] Power_TurnOffLed \n");
    }
    else
    {
          Power_TurnOnGreenLed();
#if LED_HOT_Menu_Func		
  if(FACTORY_MODE_FLAG||BURNIN_MODE_FLAG)
   {
          Set_LED_Power();

          ForceDelay1ms(53);//(49);

          Clr_LEDActiveOnFlag();		  
		
          SetLedFunc(LED_Number, LedColorful, LedMax, LedBreath);
   }
  else
   {
     if(USER_PREF_LEDMODE_INDEX!=LedModeIndex_Off)
     	{
          Set_LED_Power();
       if(USER_PREF_LEDMODE_INDEX==LedModeIndex_Breath)
          ForceDelay1ms(53);//(49);
       else//LedModeIndex_User
       {
	if((USER_PREF_LedModeType==LedBreath)	)  
          ForceDelay1ms(53);
       else    
          ForceDelay1ms(55);//55//53
       }

          Clr_LEDActiveOnFlag();		  
		
	 if(FACTORY_MODE_FLAG||BURNIN_MODE_FLAG||(USER_PREF_LEDMODE_INDEX==LedModeIndex_Breath))	
          SetLedFunc(LED_Number, LedColorful, LedMax, LedBreath);
	 else
	 {
          SetLedFunc(LED_Number, USER_PREF_LEDColorful_MODE, USER_PREF_LedStrength, USER_PREF_LedModeType);
          Set_LEDActiveOnFlag();		  
	  }
	  if((USER_PREF_LEDMODE_INDEX==LedModeIndex_Breath)&&(!FACTORY_MODE_FLAG)&&(!BURNIN_MODE_FLAG))
          Set_LEDActiveOnFlag();		  
	  	
	 }
     else
     	{
     	 if(IS_LED_Power())
          SetLedFunc(0xFF, 0xFF, 0xFF, 0xFF);
          ForceDelay1ms(55);//55//53
          Clr_LED_Power();
	   Clr_LEDActiveOnFlag();	  
      }
    }
#endif

	   APPSYSTEM_PRINT("[appSystem] Power_TurnOnGreenLed \n");
    }


	
}

/////////////////////////////////////////////////////////////////////////
///////////////////////////////  AP init  ///////////////////////////////////
/////////////////////////////////////////////////////////////////////////
void APPSystem_Handler( void )
{
    APPSystem_CheckDPMS();
}

/////////////////////////////////////////////////////////////////////////
///////////////////////////////  AP init  ///////////////////////////////////
/////////////////////////////////////////////////////////////////////////
void APPSystem_Init( void )
{
    APPSystem_CheckPowerStatus();

    APPSYSTEM_PRINT("[appSystem] APPSystem_Init \n");
}




/////////////////////////////////////////////////////////////////////////
/////////////////////////////  Enter PM DPMS  /////////////////////////////
/////////////////////////////////////////////////////////////////////////
void APPSystem_ActivePort_For_PowerOn( void )
{

	Set_HDMI1HPDFlag();   
	Set_HDMI2HPDFlag();   
	Set_HDMI3HPDFlag();   

#if ENABLE_MULTI_WINDOW_SETTINGS_MENU	   
      if(USER_PREF_MULTI_WIN_MODE == eMULTIWIN_OFF)
      {  
#if Enable_AutoInput_Menu
          SwitchPort(DISPLAY_WIN1, Get_SourceSwitchType());
	   Set_InputTimingChangeFlag(Get_SourceSwitchType());
#else     
          SwitchPort(DISPLAY_WIN1, USER_PREF_INPUT_TYPE);
	   Set_InputTimingChangeFlag(USER_PREF_INPUT_TYPE);
#endif	   
      }
      else if (USER_PREF_MULTI_WIN_MODE == eMULTIWIN_2_PIP||USER_PREF_MULTI_WIN_MODE == eMULTIWIN_2_POP1)
      {  
#if Enable_AutoInput_Menu
          SwitchPort(DISPLAY_WIN1, Get_SourceSwitchType());
	   Set_InputTimingChangeFlag(Get_SourceSwitchType());
#else     
          SwitchPort(DISPLAY_WIN1, USER_PREF_INPUT_TYPE);
	   Set_InputTimingChangeFlag(USER_PREF_INPUT_TYPE);
#endif
          SwitchPort(DISPLAY_WIN2, USER_PREF_INPUT_TYPE_SUBWIN1);
	   Set_InputTimingChangeFlag(USER_PREF_INPUT_TYPE_SUBWIN1);
      }
      else if (USER_PREF_MULTI_WIN_MODE == eMULTIWIN_3)
     {   
#if Enable_AutoInput_Menu
          SwitchPort(DISPLAY_WIN1, Get_SourceSwitchType());
	   Set_InputTimingChangeFlag(Get_SourceSwitchType());
#else     
          SwitchPort(DISPLAY_WIN1, USER_PREF_INPUT_TYPE);
	   Set_InputTimingChangeFlag(USER_PREF_INPUT_TYPE);
#endif

          SwitchPort(DISPLAY_WIN2, USER_PREF_INPUT_TYPE_SUBWIN1);
          SwitchPort(DISPLAY_WIN3, USER_PREF_INPUT_TYPE_SUBWIN2);		

	   Set_InputTimingChangeFlag(USER_PREF_INPUT_TYPE_SUBWIN1);	
	   Set_InputTimingChangeFlag(USER_PREF_INPUT_TYPE_SUBWIN2);	
      }
      else if (USER_PREF_MULTI_WIN_MODE == eMULTIWIN_4)
      {   
#if Enable_AutoInput_Menu
          SwitchPort(DISPLAY_WIN1, Get_SourceSwitchType());
	   Set_InputTimingChangeFlag(Get_SourceSwitchType());
#else     
          SwitchPort(DISPLAY_WIN1, USER_PREF_INPUT_TYPE);
	   Set_InputTimingChangeFlag(USER_PREF_INPUT_TYPE);
#endif
          SwitchPort(DISPLAY_WIN2, USER_PREF_INPUT_TYPE_SUBWIN1);
          SwitchPort(DISPLAY_WIN3, USER_PREF_INPUT_TYPE_SUBWIN2);		
          SwitchPort(DISPLAY_WIN4, USER_PREF_INPUT_TYPE_SUBWIN3);		
	   Set_InputTimingChangeFlag(USER_PREF_INPUT_TYPE_SUBWIN1);	
	   Set_InputTimingChangeFlag(USER_PREF_INPUT_TYPE_SUBWIN2);		
	   Set_InputTimingChangeFlag(USER_PREF_INPUT_TYPE_SUBWIN3);		
      }	
#else

#if Enable_AutoInput_Menu
          SwitchPort(DISPLAY_WIN1, Get_SourceSwitchType());
	   Set_InputTimingChangeFlag(Get_SourceSwitchType());
#else     
          SwitchPort(DISPLAY_WIN1, USER_PREF_INPUT_TYPE);
	   Set_InputTimingChangeFlag(USER_PREF_INPUT_TYPE);
#endif	   

#endif	  
}
void APPSystem_ActivePort_For_PowerSaving( void )
{
       //BYTE u8InputPort =USER_PREF_INPUT_TYPE;
	WORD ePMWakeupActivePort = msAPI_PMGetWakeupActivePort();
	g_stMenuFlag.bDisplayInputMsg[DISPLAY_WIN1]=0;
	g_stMenuFlag.bDisplayInputNoSignalMsg[DISPLAY_WIN1]=1;

	printData("1414_USER_PREF_INPUT_TYPE==%x",USER_PREF_INPUT_TYPE);
	printData("ePMWakeupActivePort==%x",ePMWakeupActivePort);

   #if SwitchPortByDP_Detect
     if(USER_PREF_INPUT_TYPE==INPUT_HDMI)
     {
       if(!hw_DP_DET_Pin)
       {
          USER_PREF_INPUT_TYPE=INPUT_DISPLAYPORT;
	   ePMWakeupActivePort	 =USER_PREF_INPUT_TYPE;
	   g_u8SwitchPortCounter = 0; // sync locked, counter reset to 0		   
       }
	else
	{
           ;
	}
     }
    else	
    {
      if(hw_DP_DET_Pin)
      {
          USER_PREF_INPUT_TYPE=INPUT_HDMI;
	   ePMWakeupActivePort	 =USER_PREF_INPUT_TYPE;
	   g_u8SwitchPortCounter = 0; // sync locked, counter reset to 0		   
      }
    }
   #endif

	Set_HDMI1HPDFlag();   
	Set_HDMI2HPDFlag();   
	Set_HDMI3HPDFlag();   
#if ENABLE_MULTI_WINDOW_SETTINGS_MENU	
      if(USER_PREF_MULTI_WIN_MODE == eMULTIWIN_OFF)
      {  
#if 0//Enable_AutoInput_Menu
          SwitchPort(DISPLAY_WIN1, Get_SourceSwitchType());
#else     
          SwitchPort(USER_PREF_WIN_SEL, ePMWakeupActivePort);
#endif
          //SwitchPort(USER_PREF_WIN_SEL, ePMWakeupActivePort);
      }
      else if (USER_PREF_MULTI_WIN_MODE == eMULTIWIN_2_PIP||USER_PREF_MULTI_WIN_MODE == eMULTIWIN_2_POP1)
      {  
          if(USER_PREF_INPUT_TYPE==ePMWakeupActivePort)
          SwitchPort(DISPLAY_WIN1, ePMWakeupActivePort);
          if(USER_PREF_INPUT_TYPE_SUBWIN1==ePMWakeupActivePort)
          SwitchPort(DISPLAY_WIN2, ePMWakeupActivePort);
      }
      else if (USER_PREF_MULTI_WIN_MODE == eMULTIWIN_3)
     {   
          if(USER_PREF_INPUT_TYPE==ePMWakeupActivePort)
          SwitchPort(DISPLAY_WIN1, ePMWakeupActivePort);
          if(USER_PREF_INPUT_TYPE_SUBWIN1==ePMWakeupActivePort)
          SwitchPort(DISPLAY_WIN2, ePMWakeupActivePort);
          if(USER_PREF_INPUT_TYPE_SUBWIN2==ePMWakeupActivePort)
          SwitchPort(DISPLAY_WIN3, ePMWakeupActivePort);
		  
      }
      else if (USER_PREF_MULTI_WIN_MODE == eMULTIWIN_4)
      {   
          if(USER_PREF_INPUT_TYPE==ePMWakeupActivePort)
          SwitchPort(DISPLAY_WIN1, ePMWakeupActivePort);
          if(USER_PREF_INPUT_TYPE_SUBWIN1==ePMWakeupActivePort)
          SwitchPort(DISPLAY_WIN2, ePMWakeupActivePort);
          if(USER_PREF_INPUT_TYPE_SUBWIN2==ePMWakeupActivePort)
          SwitchPort(DISPLAY_WIN3, ePMWakeupActivePort);
          if(USER_PREF_INPUT_TYPE_SUBWIN3==ePMWakeupActivePort)
          SwitchPort(DISPLAY_WIN4, ePMWakeupActivePort);
		  	
      }	
#else
     SwitchPort(USER_PREF_WIN_SEL, ePMWakeupActivePort);
#endif

      if(USER_PREF_INPUT_TYPE!=ePMWakeupActivePort)
	 g_stMenuFlag.bDisplayInputMsg[DISPLAY_WIN1]=1;

}



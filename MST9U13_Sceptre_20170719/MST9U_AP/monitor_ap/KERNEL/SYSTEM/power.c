#include "datatype.h"
#include "Board.h"
#include "Global.h"
#include "ms_reg.h"
//#include "Panel.h"
#include "misc.h"
#include "Ms_rwreg.h"
#include "mStar.h"
#include "Common.h"
#include "msOSD.h"
#include "Adjust.h"
//#include "msADC.h"
//#include "drvADC.h"
#include "drvmsOVD.h"
#include "GPIO_DEF.h"
#include "Menu.h"
#if (ZUI)
#include "MApp_ZUI_ACTglobal.h"
#endif
//#if ENABLE_LED_CONTROLLER
//#include "LedControl.h"
//#endif
#include "Ms_PM.h"
#include "appPanel.h"
////////////////////////////////////////////////////
//#include "drvPower.h"
//#include "drvmStar.h"
///////////////////////////////////////////////////
#if ENABLE_DPS
#include "msDPS_Setting.h"
#endif

#define POWER_DEBUG    1
#if ENABLE_MSTV_UART_DEBUG&&POWER_DEBUG
    #define POWER_printData(str, value)   printData(str, value)
    #define POWER_printMsg(str)           printMsg(str)
#else
    #define POWER_printData(str, value)
    #define POWER_printMsg(str)
#endif
#define TurnOffOSD      1
void Power_TurnOffLed( void );
void Power_TurnOffPanel( void );
void Power_TurnOnAmberLed( void );
//=============================================
void Power_TurnOffLed( void )
{
#if ((defined(TaiBei_Project))&&(PanelType==PanelT500QVN02))
    hw_SetAmberLed();
#else
#if ENABLE_TOUCH_KEY
    #if TOUCH_KEY_CTRL_LED
    //TouchKeySetLEDStatus = TouchKeyLEDStatus_OFF;
    TouchKey_CtrlLED(TouchKeyLEDStatus_OFF);
    #endif
#else
    hw_ClrGreenLed();
    hw_ClrAmberLed();
#endif
#endif
}
void Power_TurnOnGreenLed( void )
{
#if ENABLE_TOUCH_KEY
    #if TOUCH_KEY_CTRL_LED
    //TouchKeySetLEDStatus = TouchKeyLEDStatus_GREEN_ON;
    TouchKey_CtrlLED(TouchKeyLEDStatus_GREEN_ON);
    #endif
#else
 #if EnableDdcLed
	if(USER_PREF_LEDMODE==OffLedMode)
	{
		hw_ClrGreenLed(); 
	}
	else
	{
		hw_SetGreenLed(); 
	}
  #else
        hw_SetGreenLed();
  #endif	
    hw_ClrAmberLed();
#endif
}

void Power_TurnOnAmberLed( void )
{
#if ((defined(TaiBei_Project))&&(PanelType==PanelT500QVN02))
    hw_SetGreenLed();
    hw_SetAmberLed();
#else
#if ENABLE_TOUCH_KEY
    #if TOUCH_KEY_CTRL_LED
    //TouchKeySetLEDStatus = TouchKeyLEDStatus_AMBER_ON;
    TouchKey_CtrlLED(TouchKeyLEDStatus_AMBER_ON);
    #endif
#else
    hw_ClrGreenLed();
 #if EnableDdcLed
	if(USER_PREF_LEDMODE==OffLedMode)
	{
		hw_ClrAmberLed(); 
	}
	else
	{
		hw_SetAmberLed(); 
	}
  #else
         hw_SetAmberLed();
  #endif
#endif
#endif
}
void Power_TurnOffAmberLed(void)
{
    hw_ClrAmberLed();
}

void Power_TurnOnPanel( void )
{
#if ENABLE_DP_OUTPUT
BYTE HPD_COUNT=0;
#endif

    if( bPanelOnFlag ) // prevent multiple call of Power_TurnOnPanel() for multi scaler case
        return;

    	#if ENABLE_TIMINGCHANGE_CLRBL_ONLY  //  160714_14
	if((bOnlyClrBL==TRUE)&&( bPanelOnFlag==0 ))
	{
		bPanelOnFlag=1;
		bOnlyClrBL=FALSE;
		hw_SetBlacklit();
		return;
	}
	#endif

    bPanelOnFlag = 1;
    mStar_BlackWhiteScreenCtrl(BW_SCREEN_BLACK);
 #if (PanelType==PanelCMI28UltraHD)	
    ForceDelay1ms( 300 );
#endif
    hw_SetPanel();

    ForceDelay1ms( PanelOnTiming1 );

    msDrvPowerModCtrl(_ENABLE);

    if( !iGenTuningFinished )
    {
        msDrvIGenTuning();
    }


#if ENABLE_DP_OUTPUT

#if eDPTXDETECTHPD
for(HPD_COUNT=0;HPD_COUNT<20;HPD_COUNT++)
{
if(DPTxCheckHPD(0))
{
    ForceDelay1ms(2); // clear HPDIRQ after HPD IRQ delay 1.5ms     
#endif
    DPTxHPDIRQRst(); // clear HPDIRQ after HPD IRQ delay 1.5ms 

#if eDPTXHBR2PortEn
    DPTxTraining(0,eDPTXLinkRate,eDPTXLaneCount);
#else
// ##################### MOD ###################
#if ((CHIP_ID==MST9U) || (CHIP_ID==MST9U2))
    msWrite2ByteMask(REG_MOD1_60, BIT15, BIT15);// enable check MODbuf  rw point status
#elif (CHIP_ID==MST9U3)
    msWriteByteMask(REG_MOD1_F3, BIT7, BIT7);    // sw mode enable
#endif
#if eDPTXMODOnePort
	if(DPTxTraining(0,eDPTXLinkRate,eDPTXLaneCount)==FALSE)
	{
		ForceDelay1ms(10);
    	DPTxTraining(0,eDPTXLinkRate,eDPTXLaneCount);
	}
#else
#if eDPTXPortSWAP	
    DPTxTraining(1,eDPTXLinkRate,eDPTXLaneCount);
    DPTxTraining(0,eDPTXLinkRate,eDPTXLaneCount);
#else
    DPTxTraining(0,eDPTXLinkRate,eDPTXLaneCount);
    DPTxTraining(1,eDPTXLinkRate,eDPTXLaneCount);
#endif
#endif
#if ((CHIP_ID==MST9U) || (CHIP_ID==MST9U2))
    msWrite2ByteMask(REG_MOD1_60,0, BIT15);// disable check MODbuf  rw point status
#endif
// ##############################################

#endif

#if eDPTXDETECTHPD
    		break;	
	}
	else
	{
		ForceDelay1ms(10); 
		POWER_printData("check HPD Low [%d]\r\n", HPD_COUNT);
	} 
}
#endif

#endif


#if PANEL_VBY1()
#if 0//def PanelI2CCommandSetting
   if(!VBY1_Check_LockN_Flag)
   {
      printMsg("222222222222222222222");
      Set_VBY1_Check_LockN_Flag();
      msWriteByteMask(REG_140600, 0x00, 0x01); // force at 1st time, especially for HW mode that might show lock(but display error) before we doing the training.
      msDrvCheckVBY1LockN(0);
      
      ForceDelay1ms(30);
      
      if(g_bAPPWindow_Init&&CB_FuncList[eCB_CustomerSetPanelI2CCommand]!=NULL)
      ((fpCustomerSetPanelI2CCommand)CB_FuncList[eCB_CustomerSetPanelI2CCommand])();
   }
#endif
   ForceDelay1ms( 50 );
  #if 1//ndef PanelI2CCommandSetting
    msWriteByteMask(REG_140600, 0x00, 0x01); // force at 1st time, especially for HW mode that might show lock(but display error) before we doing the training.
  #endif
    msDrvCheckVBY1LockN(1);
	
#endif

    ForceDelay1ms( PanelOnTiming2 );

#if (PanelType==PanelV390DK1_LS1)
    SwitchTCONDisplay();
    ForceDelay1ms( 250 );
#elif ((defined(W3K_Project)||defined(W1B_Project))&&(PanelType==PanelT430QVN01))
    ForceDelay1ms( 200 );
    msDrvCheckVBY1LockN(1);
    ForceDelay1ms( 400 );
#elif (PanelType== PanelLSF490FJ02)
    ForceDelay1ms( 250 );
#endif



#if (PanelType!= PanelCMI28UltraHD)
 #if FreeSync_Not_Support_Info
   if(!FreeSyncMsgFlag)
 #endif
    mStar_BlackWhiteScreenCtrl(BW_SCREEN_OFF);
#endif

    if(InputTimingChangeFlag(SrcInputPortM) // for mode change
        &&(!g_stMenuFlag.bShowInputIcon)
#if LOGO_ENABLE    
#if (!ZUI)
        && !g_stMenuFlag.bDisplayLogo
#else
        && !g_stZuiMenuFlag.bDisplayLogo
#endif
#endif
        )
        Power_TurnOffPanel();
    else
    {


 		#if (PanelType == PanelLSM315HP01)//(MS_PANEL_TYPE==PanelCMI40UltraHD||MS_PANEL_TYPE==PanelTPVLVM400K1QS1AD82P0X ||MS_PANEL_TYPE==PanelTPT315B5HP01)
              #if ( CHIP_ID == MST9U3)
              #define VX1_HPDN_STATUS     (msReadByte(REG_140619)&BIT7)
              #define VX1_LOCKN_STATUS    (msReadByte(REG_140619)&BIT6)
              #else
               #define VX1_HPDN_STATUS     (msReadByte(REG_1407D1)&BIT7)
               #define VX1_LOCKN_STATUS    (msReadByte(REG_1407D1)&BIT6)
              #endif
          if((VX1_LOCKN_STATUS||VX1_HPDN_STATUS)
             #if SET_BLIT_WITH_OSDFreeSyncSwitchAction
                &&(bOSDFreeSyncOnOffFlag==FALSE)
             #endif
             )
            {
              POWER_printData(" VB1_LOCK_NG",0);
            }
	     else
 	#endif
             {
             #if SET_BLIT_WITH_OSDFreeSyncSwitchAction
                #if (PanelType == PanelLSM315HP01)
        			if(VX1_LOCKN_STATUS||VX1_HPDN_STATUS)
        			{

        			}
        			else
        			{
        				bOSDFreeSyncOnOffFlag=FALSE;
        			}
		      #endif
             #endif

	
  #if (VBY1_PowerSequence)
     Set_BackLightOnFlag();
  #else
    #if (ENABLE_LED_CONTROLLER)
        msLED_On();
    #else
        hw_SetBlacklit();
    #endif
#endif	
    	}
    }

#if (PanelType== PanelCMI28UltraHD)
    ForceDelay1ms( 300 );
    mStar_BlackWhiteScreenCtrl(BW_SCREEN_OFF);
#endif
	
    g_stMenuFlag.bAnyWinDisplayChange=1;
    POWER_printData("  Power_TurnOnPanel", 1);
}

void Power_TurnOffPanel( void )
{
    if( !bPanelOnFlag )
        return;

    bPanelOnFlag = 0;

#if TurnOffOSD
    Osd_DoubleBuffer(FALSE);
    Menu_Hide();
    mStar_ScalerDoubleBuffer(FALSE);
    Delay4us();
#endif

  #if(ENABLE_LED_CONTROLLER)
    msLED_Off();
  #else
    hw_ClrBlacklit();
#endif

#if (VBY1_PowerSequence)
    PowerOnBackLightCounter=0;
#endif

    ForceDelay1ms( PanelOffTiming1 );

#if ENABLE_DP_OUTPUT
#if eDPTXHBR2PortEn
    DPTxMLSignalEn(0,FALSE);
#elif eDPTXMODOnePort
    DPTxMLSignalEn(0,FALSE);
#else
    DPTxMLSignalEn(0,FALSE);
    DPTxMLSignalEn(1,FALSE);
#endif
#endif

#if 0//ndef _PANEL_NORMALLY_BLACK_
    mStar_BlackWhiteScreenCtrl(BW_SCREEN_WHITE);
#else
    mStar_BlackWhiteScreenCtrl(BW_SCREEN_BLACK);
#endif
    ForceDelay1ms( 80 );
    mStar_BlackWhiteScreenCtrl(BW_SCREEN_OFF);

    msDrvPowerModCtrl(_DISABLE);

    ForceDelay1ms( PanelOffTiming2 );

    hw_ClrPanel();

#ifdef PanelI2CCommandSetting
   Clr_VBY1_Check_LockN_Flag();
#endif

#if TurnOffOSD
    Osd_DoubleBuffer(TRUE);
#endif

    PanelOffOnCounter = 1200;
    bPanelOffOnFlag = 1;
   #if ENABLE_TIMINGCHANGE_CLRBL_ONLY
    bOnlyClrBL=FALSE;
   #endif

    POWER_printData("  Power_TurnOffPanel", 1);
}



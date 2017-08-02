///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    Ms_PM.c
/// @author MStar Semiconductor Inc.
/// @brief  PM Function
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#define _MS_PM_C_
#include "Board.h"
#include <math.h>
#include <string.h>
#include "Types.h"
//#include "mode.h"
#include "Common.h"
//#include "UserPrefDef.h"
#include "Global.h"
#include "Misc.h"
#include "Ms_reg.h"
#include "Ms_rwreg.h"
//#include "MDebug.h"
//#include "menu.h"
//#include "userpref.h"
//#include "Power.h"
//#include "DDC2Bi.h"
#include "drvPM.h"
//#include "MenuDef.h"
//#include "MenuFunc.h"
#include "Mcu.h"
//#include "Mstar.h"
//#include "Detect.h"
#include "Keypad.h"     // Provides: Key_GetKeypadStatus()
#include "KeypadDef.h"  // Provides: KeypadMask
#include "GPIO_DEF.h"
#include "inputport.h"
#include "mapi_mhl.h"
#include "mdrv_mhl.h"
#include "halRwreg.h"
#include "mhal_tmds.h"
#include "drvmstar.h"
#include "mdrv_adctbl.h"
#include "drvAdc.h"
#include "mapi_combo.h"
#if(IR_REMOTE_SEL != NO_REMOTE)
#include "MsIR.h"
#include "IrFunc.h"
#endif
#if ENABLE_DC_SDM_FORWARD
#include "msAPI_DaisyChain.h"
#endif
//#include "msHDCP.h"
#if ENABLE_DP_INPUT
//#include "drvDPRxApp.h"
#endif
//#include "drvmStar.h"
//#include "UserPrefAPI.h"
//#include "MST9U_DEMO_PMSET.h"

extern void Main_SlowTimerHandler(void);
extern BYTE Key_GetKeypadStatus( void );

#if  ENABLE_DP_INPUT
#if  0 //DPENABLEMCCS
extern void DDC2BI_DP(void);
#if ((CHIP_FAMILY_TYPE == CHIP_FAMILY_TSUMC)||(CHIP_FAMILY_TYPE == CHIP_FAMILY_MST9U)||(CHIP_FAMILY_TYPE==CHIP_FAMILY_TSUMJ))
#if FPGA_PM
#if D2B_XShared_DDCBuffer
extern BYTE *DDCBuffer;
#else
#endif
#endif
extern BYTE xdata DDCBuffer[DDC_BUFFER_LENGTH];
//#endif
#else
extern BYTE xdata DDCBuffer[DDC_BUFFER_LENGTH];
#endif
#endif
#endif
//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define PM_DEBUG    1
#if ENABLE_DEBUG&&PM_DEBUG
    #define PM_printData(str, value)   printData(str, value)
    #define PM_printMsg(str)           printMsg(str)
#else
    #define PM_printData(str, value)
    #define PM_printMsg(str)
#endif

#define XBYTE             ((unsigned char volatile xdata *) 0)


//--------PM Option----------------
/*Choice MCU clock when enter into PM_MODE*/
//extern BYTE xdata MenuPageIndex ;

//extern BYTE xdata MenuItemIndex ;


//---------------------------------
//#define VCP_SET             0x03
//#define VCP_DPMS            0xD6
//#define VCP_DPMS_ON         0x01
//-------------------------------------------------------------------------------------------------
//#define IS_SOURCE_AUTOSWITCH() (TRUE)
//#define IS_SOURCE_VGA()        (FALSE) //(UserPreference.InputSync == ANALOG)
//#define IS_SOURCE_DVI0()       (FALSE) //(UserPreference.InputSync == DIGITAL)
//#define IS_SOURCE_DVI1()       (FALSE) //(UserPreference.InputSync == DIGITAL2)

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------




XDATA sPM_Info  sPMInfo;
XDATA ePM_WakeupStatus ucWakeupStatus;
XDATA EN_INPUTPORT_MUX u8WakeupInputSrc;
XDATA BYTE u8WakeupSyncStatus;
XDATA Bool bPMPortSkipIndex[Input_Nums];
XDATA Bool bForceWakeup = 0;
XDATA BYTE u8WakeuCounter;
XDATA sPM_SARSingleMode sPMSARKey;
//-------------------------------------------------------------------------------------------------
//  External Variables
//-------------------------------------------------------------------------------------------------
#if !ENABLE_DEBUG
extern BYTE rxIndex;
#if CHIP_FAMILY_TYPE==CHIP_FAMILY_TSUMC||CHIP_FAMILY_TYPE == CHIP_FAMILY_MST9U||CHIP_FAMILY_TYPE==CHIP_FAMILY_TSUMJ
#if 0//D2B_XShared_DDCBuffer
extern BYTE *DDCBuffer;
#else
extern BYTE xdata DDCBuffer[];
#endif
#else
extern BYTE xdata DDCBuffer[];
#endif
#endif

#ifndef PM_SUPPORT_ADC_TIME_SHARE
#define PM_SUPPORT_ADC_TIME_SHARE  0
#endif
code BYTE g_DVIClk_DetectMask[(MUX_VD-MUX_ADC+1)] =
{
    0x00,                   //MUX_ADC
    DVI_CLK_DET_0,  //MUX_COMBO0
    DVI_CLK_DET_1,  //MUX_COMBO1
    DVI_CLK_DET_2,  //MUX_COMBO2
    DVI_CLK_DET_3,  //MUX_COMBO3
    DVI_CLK_DET_4, //MUX_COMBO4
    DVI_CLK_DET_5, //MUX_COMBO5
    0x00,                  //MUX_VD
};

code ST_INPUTPORT_PARAM g_InputPort[Input_Nums] =
{
#if (INPUT_TYPE&INPUT_1A)
    {PORT_FLOW_0, PORT_TYPE_0, IP_STATUS_IDLE, SC_MAX_NUMS, 0, 0, 0, 0, 0, bInputTimingChangeBit, {SyncLoss, 0, 0, 0xFF, 0, 0}, INPUTCOLOR_RGB, 0, {0, 0, 0}},
#endif
#if (INPUT_TYPE >= INPUT_1C)
    {PORT_FLOW_1, PORT_TYPE_1, IP_STATUS_IDLE, SC_MAX_NUMS, 0, 0, 0, 0, 0, bInputTimingChangeBit, {SyncLoss, 0, 0, 0xFF, 0, 0}, INPUTCOLOR_RGB, 0, {0, 0, 0}},
#endif
#if (INPUT_TYPE >= INPUT_2C)
    {PORT_FLOW_2, PORT_TYPE_2, IP_STATUS_IDLE, SC_MAX_NUMS, 0, 0, 0, 0, 0, bInputTimingChangeBit, {SyncLoss, 0, 0, 0xFF, 0, 0}, INPUTCOLOR_RGB, 0, {0, 0, 0}},
#endif
#if (INPUT_TYPE >= INPUT_3C)
    {PORT_FLOW_3, PORT_TYPE_3, IP_STATUS_IDLE, SC_MAX_NUMS, 0, 0, 0, 0, 0, bInputTimingChangeBit, {SyncLoss, 0, 0, 0xFF, 0, 0}, INPUTCOLOR_RGB, 0, {0, 0, 0}},
#endif
#if (INPUT_TYPE >= INPUT_4C)
    {PORT_FLOW_4, PORT_TYPE_4, IP_STATUS_IDLE, SC_MAX_NUMS, 0, 0, 0, 0, 0, bInputTimingChangeBit, {SyncLoss, 0, 0, 0xFF, 0, 0}, INPUTCOLOR_RGB, 0, {0, 0, 0}},
#endif
#if (INPUT_TYPE >= INPUT_5C)
    {PORT_FLOW_5, PORT_TYPE_5, IP_STATUS_IDLE, SC_MAX_NUMS, 0, 0, 0, 0, 0, bInputTimingChangeBit, {SyncLoss, 0, 0, 0xFF, 0, 0}, INPUTCOLOR_RGB, 0, {0, 0, 0}},
#endif
#if (INPUT_TYPE >= INPUT_6C)
    {PORT_FLOW_6, PORT_TYPE_6, IP_STATUS_IDLE, SC_MAX_NUMS, 0, 0, 0, 0, 0, bInputTimingChangeBit, {SyncLoss, 0, 0, 0xFF, 0, 0}, INPUTCOLOR_RGB, 0, {0, 0, 0}},
#endif
#if (INPUT_TYPE >= INPUT_7C)
    {PORT_FLOW_7, PORT_TYPE_7, IP_STATUS_IDLE, SC_MAX_NUMS, 0, 0, 0, 0, 0, bInputTimingChangeBit, {SyncLoss, 0, 0, 0xFF, 0, 0}, INPUTCOLOR_RGB, 0, {0, 0, 0}},
#endif
};


Bool msPM_IsState_IDLE(void)
{
    if(sPMInfo.ePMState == ePM_IDLE)
        return TRUE;
    return FALSE;
}

//**************************************************************************
//  [Function Name]:
//                  msPM_Init()
//  [Description]
//                  msPM_Init
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void msPM_Init(void)
{
    //g_bMcuPMClock = 0; //20101104

    sPMInfo.sPMConfig.bHVSync_enable =0;

#if( PM_SUPPORT_WAKEUP_CEC )
	sPMInfo.sPMConfig.bCEC_enable=0;
#endif

#if( PM_SUPPORT_WAKEUP_DVI )
    sPMInfo.sPMConfig.bDVI_enable =0;
#endif  // end of #if( PM_SUPPORT_WAKEUP_DVI )
    sPMInfo.sPMConfig.bDP_enable=0;
    sPMInfo.sPMConfig.bSOG_enable =0;
    sPMInfo.sPMConfig.bSAR_enable =0;
    sPMInfo.sPMConfig.bGPIO_enable =0;
    sPMInfo.sPMConfig.bMCCS_enable =0;
#if( PM_SUPPORT_AC2DC )
    sPMInfo.sPMConfig.bACtoDC_enable=0;
#endif  // end of #if( PM_SUPPORT_AC2DC )
    sPMInfo.sPMConfig.bMCUSleep=0;

    sPMInfo.ucPMMode = ePM_POWER_ON;
    sPMInfo.ePMState = ePM_IDLE;

    //sPMInfo.sPMWakeUpCheck.wPMGPIOWakeupSta = _msRead2Byte(REG_0164)&EN_GPIO_DET_MASK;

}
//**************************************************************************
//  [Function Name]:
//                  msPM_SetPMSandby()
//  [Description]
//                  msPM_SetPMSandby_ForceDVI0
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void msPM_SetFlagDPMS(void)
{
    PM_printMsg(" msPM_SetFlagDPMS");
#if FPGA_HW_PMMODE//FPGA
    sPMInfo.sPMConfig.bHVSync_enable =1;
#if( PM_SUPPORT_WAKEUP_DVI )
    sPMInfo.sPMConfig.bDVI_enable =0;
#endif

    sPMInfo.sPMConfig.bDP_enable=0;//PM_SUPPORT_WAKEUP_DP;//PM_SUPPORT_WAKEUP_DP;
    sPMInfo.sPMConfig.bSOG_enable =0;//PM_POWERSAVING_WAKEUP_SOG;
    sPMInfo.sPMConfig.bSAR_enable =0;//PM_POWERSAVING_WAKEUP_SAR;
    sPMInfo.sPMConfig.bGPIO_enable =1;//PM_POWERSAVING_WAKEUP_GPIO;
    sPMInfo.sPMConfig.bMCCS_enable =0;//PM_POWERSAVING_WAKEUP_MCCS;
#if( PM_SUPPORT_AC2DC )
    sPMInfo.sPMConfig.bACtoDC_enable=0;
#endif
    sPMInfo.sPMConfig.bMCUSleep=1;  //Set 1 to test HW mode, set 0 to test FW mode
    sPMInfo.sPMConfig.bEDID_enable = 0;
    sPMInfo.sPMConfig.ePMDVImode = ePMDVI_DVI0;
    sPMInfo.sPMConfig.ePMSARmode = ePMSAR_SAR1;
    sPMInfo.ucPMMode = ePM_POWER_DPMS;
    sPMInfo.ePMState = ePM_ENTER_PM;
#else
    sPMInfo.sPMConfig.bHVSync_enable =1;
#if( PM_SUPPORT_WAKEUP_DVI )
    sPMInfo.sPMConfig.bDVI_enable =1;
#endif

	sPMInfo.sPMConfig.bCEC_enable = PM_SUPPORT_WAKEUP_CEC;

    sPMInfo.sPMConfig.bDP_enable=PM_SUPPORT_WAKEUP_DP;//PM_SUPPORT_WAKEUP_DP;
    sPMInfo.sPMConfig.bSOG_enable =PM_POWERSAVING_WAKEUP_SOG;
    sPMInfo.sPMConfig.bSAR_enable =PM_POWERSAVING_WAKEUP_SAR;
    sPMInfo.sPMConfig.bGPIO_enable =PM_POWERSAVING_WAKEUP_GPIO;
    sPMInfo.sPMConfig.bMCCS_enable =PM_POWERSAVING_WAKEUP_MCCS;
#if( PM_SUPPORT_AC2DC )
    sPMInfo.sPMConfig.bACtoDC_enable=0;
#endif
    sPMInfo.sPMConfig.bMCUSleep=0;
    sPMInfo.sPMConfig.bEDID_enable = 0;
    sPMInfo.sPMConfig.ePMDVImode = ePMDVI_DVI0;
    sPMInfo.sPMConfig.ePMSARmode = ePMSAR_SAR1;
    sPMInfo.ucPMMode = ePM_POWER_DPMS;
    sPMInfo.ePMState = ePM_ENTER_PM;
#endif
}


//**************************************************************************
//  [Function Name]:
//                  msPM_SetPMDCoff()
//  [Description]
//                  msPM_SetPMDCoff
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void msPM_SetFlagDCoff(void)
{
    PM_printMsg(" msPM_SetFlagDCoff");
    sPMInfo.sPMConfig.bHVSync_enable =0;
#if( PM_SUPPORT_WAKEUP_DVI )
    sPMInfo.sPMConfig.bDVI_enable =0;
#endif

#if( PM_SUPPORT_WAKEUP_CEC)
		sPMInfo.sPMConfig.bCEC_enable = 0;
#endif

    sPMInfo.sPMConfig.bDP_enable=0;
    sPMInfo.sPMConfig.bSOG_enable =0;
    sPMInfo.sPMConfig.bSAR_enable =0;
    sPMInfo.sPMConfig.bGPIO_enable =PM_POWEROFF_WAKEUP_GPIO;
    sPMInfo.sPMConfig.bMCCS_enable =PM_POWEROFF_WAKEUP_MCCS;
#if( PM_SUPPORT_AC2DC )
    sPMInfo.sPMConfig.bACtoDC_enable=0;
#endif
    sPMInfo.sPMConfig.bMCUSleep=0;
    sPMInfo.sPMConfig.bEDID_enable = 0;

    sPMInfo.sPMConfig.ePMDVImode = ePMDVI_DVI0;
    sPMInfo.sPMConfig.ePMSARmode = PM_POWEROFF_SARmode;
    sPMInfo.ucPMMode = ePM_POWER_DCOFF;
    sPMInfo.ePMState = ePM_ENTER_PM;
}

void msPM_SetPortSkipIndex(WORD u16PortIndex)
{
       BYTE i;
       for(i = 0; i< Input_Nums; i++)
            bPMPortSkipIndex[i] = FALSE;

       for(i = 0; i< Input_Nums; i++)
      {
             if((u16PortIndex >> i)&0x01)
             {
                    bPMPortSkipIndex[i] = TRUE;
             }

      }
}
//**************************************************************************
//  [Function Name]:
//                  msPM_EnableHVSyncDetect(BOOL bEnable)
//  [Description]
//                  msPM_EnableSyncDetect
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void msPM_EnableHVSyncDetect(BOOL bEnable)
{
    BYTE i;
#if PM_SUPPORT_ADC_TIME_SHARE
    msPM_EnableDVIClockAmp( TRUE );       // enable DVI clock amplifier control, because SoG time sharing requires DVI clock amplifier control.
#else

#endif

    for(i =0; i< Input_Nums; i++)
    {
        if(INPUT_IS_VGA(i) && !bPMPortSkipIndex[i])
                break;
    }


    if(bEnable && (i<Input_Nums))
    {
        msWriteByteMask(REG_002508, 0, (_BIT1|_BIT0)); // power on ADC BGAP and VREF
        msWriteByteMask(REG_0025B4, 0, (_BIT6|_BIT5)); // power on ADC BGAP and VREF
        msWriteByteMask(REG_SYNC_DET, EN_SYNC_DET_SET, EN_SYNC_DET_MASK);       // Sync clock not gating
    }
    else
    {
        msWriteByteMask(REG_002508, (_BIT1|_BIT0), (_BIT1|_BIT0)); // power down ADC BGAP and VREF
        msWriteByteMask(REG_0025B4, (_BIT6|_BIT5), (_BIT6|_BIT5)); // power on ADC BGAP and VREF
        msWriteByteMask(REG_SYNC_DET, 0, EN_SYNC_DET_MASK);     // Sync clock gating
    }
}


//**************************************************************************
//  [Function Name]:
//                  msPM_EnableSOGDetect(BOOL bEnable)
//  [Description]
//                  Enable SOG Detect: need power ADC VREF & BANGAP
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void msPM_EnableSOGDetect(BOOL bEnable)
{
    if(bEnable)
    {
#if(PM_SUPPORT_SOG_TIME_SHARE )
        msWriteBit(REG_0003FF, TRUE, _BIT2);
        msWriteBit(REG_0003E9, TRUE, _BIT7);   // enable SoG time sharing option of VREF/BGAP to save power of SoG wakeup
        msPM_EnableDVIClockAmp( TRUE );       // enable DVI clock amplifier control, because SoG time sharing requires DVI clock amplifier control.
#else
        msWriteBit(REG_0003E9, FALSE, _BIT7);   // disable SoG time sharing option of VREF/BGAP to save power of SoG wakeup
#endif
        msWriteByteMask(REG_002509, 0, (_BIT2|_BIT1)); // power on online SOG DAC and main
        msWriteByteMask(REG_002508, 0x00,BIT1);
        msWriteByteMask(REG_002508, 0x00,BIT2);

        msWriteByteMask(REG_002560, 0, (_BIT0)); // disable sog mux
        msWriteByteMask(REG_0025F0, 0x82, 0xFF); // sog high num/rst_cnt_limit
        msWriteByteMask(REG_0025F1, 0x92, 0xFF); //
        msWriteByteMask(REG_0025F2, 0xC8, 0xFF); // sog rst_high_ num
        msWriteByteMask(REG_0025F3, 0x10, 0xFF); // cnt_limit
        #if _NEW_SOG_WAKEUP_DET_
        msWriteByteMask(REG_0025F4, _BIT2, _BIT2); // sog high num
        NEW_SOG_WAKEUP_FLUSH();
        #endif
        msWriteByteMask(REG_SYNC_DET, EN_SOG_DET, EN_SOG_DET);
    }
    else
    {
        msWriteBit(REG_0003E9, FALSE, _BIT7);   // disable SoG time sharing option of VREF/BGAP to save power of SoG wakeup
        msWriteByteMask(REG_002509, (_BIT2|_BIT1), (_BIT2|_BIT1)); // power down online SOG DAC and main
        msWriteByteMask(REG_002508, BIT1,BIT1);
        msWriteByteMask(REG_002508, BIT2,BIT2);
        msWriteByteMask(REG_002560, (_BIT0), (_BIT0)); // disable sog mux
        msWriteByteMask(REG_0025F0, 0, 0xFF); // sog high num/rst_cnt_limit
        msWriteByteMask(REG_0025F1, 0, 0xFF); //
        msWriteByteMask(REG_0025F2, 0, 0xFF); // sog rst_high_ num
        msWriteByteMask(REG_0025F3, 0, 0xFF); // cnt_limit
        msWriteByteMask(REG_0025F4, 0, _BIT2); // sog high num
        msWriteByteMask(REG_SYNC_DET, 0, EN_SOG_DET);
    }
}


//**************************************************************************
//  [Function Name]:
//                  msPM_PassWord(BOOL bEnable)
//  [Description]
//                  input  the password for entering pm or turn off XTAL
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void msPM_PassWord(BOOL bEnable)
{
    if(bEnable)
    {
        msWriteByte(REG_000387,0x55);
        msWriteByte(REG_000388,0xAA);
    }
    else
    {
        msWriteByte(REG_000387,0x00);
        msWriteByte(REG_000388,0x00);
    }
}
#if( PM_SUPPORT_WAKEUP_DVI )

//**************************************************************************
//  [Function Name]:
//                  msPM_EnableDVIClockAmp(BOOL bEnable)
//  [Description]
//                  Enable DVI clock amplifier control
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void msPM_EnableDVIClockAmp(Bool bEnable)
{
    if(bEnable)
    {
        msWrite2ByteMask(REG_DVI_CHEN, EN_DVI_CTRL_PERIOD|EN_DVI_CHEN_AUTO, EN_DVI_CTRL_MASK ); // enable DVI clock amplifier
    }
    else
    {
        msWrite2ByteMask(REG_DVI_CHEN, EN_DVI_ALWAYS_ON, EN_DVI_CTRL_MASK ); // disable DVI clock amplifier time sharing
    }
}
#if PM_SUPPORT_DVI5V_TurnonRterm
void msPM_EnableDVI5VDetect(Bool bEnable)
{
   BYTE XDATA i;

    if(bEnable)
    {
         msWriteByteMask(REG_00038E, 0x00, 0xFF);

         for(i=0; i<Input_Nums; i++)
         {
                if(INPUT_IS_TMDS(i))
                {
                    if(g_InputPort[i].eIPMux == MUX_COMBO0)
                    {
                        msWrite2ByteMask(REG_DVI5V_DETECT_01, DVI5V_DETECT_0, DVI5V_DETECT_0 ); // enable DVI 5V detect
                        msWriteByte(REG_0011AE+COMBO_OFFSET(MUX_COMBO0), 0x00);
                        msWrite2ByteMask(REG_0011A8+COMBO_OFFSET(MUX_COMBO0), BIT13|BIT1, _BIT13|_BIT1);

                    }
                    else if(g_InputPort[i].eIPMux == MUX_COMBO1)
                    {
                        msWrite2ByteMask(REG_DVI5V_DETECT_01, DVI5V_DETECT_1, DVI5V_DETECT_1 ); // enable DVI 5V detect
                        msWriteByte(REG_0011AE+COMBO_OFFSET(MUX_COMBO1), 0x00);
                        msWrite2ByteMask(REG_0011A8+COMBO_OFFSET(MUX_COMBO1), BIT13|BIT1, _BIT13|_BIT1);

                    }
                    else if(g_InputPort[i].eIPMux == MUX_COMBO2)
                    {
                        msWrite2ByteMask(REG_DVI5V_DETECT_2, DVI5V_DETECT_2, DVI5V_DETECT_2 ); // enable DVI 5V detect
                        msWriteByte(REG_0011AE+COMBO_OFFSET(MUX_COMBO2), 0x00);
                        msWrite2ByteMask(REG_0011A8+COMBO_OFFSET(MUX_COMBO2), BIT13|BIT1, _BIT13|_BIT1);
                    }
                   else if(g_InputPort[i].eIPMux == MUX_COMBO3)
                   {
                        msWrite2ByteMask(REG_DVI5V_DETECT_345, DVI5V_DETECT_3, DVI5V_DETECT_3 ); // enable DVI 5V detect
                        msWriteByte(REG_0011AE+COMBO_OFFSET(MUX_COMBO3), 0x00);
                        msWrite2ByteMask(REG_0011A8+COMBO_OFFSET(MUX_COMBO3), BIT13|BIT1, _BIT13|_BIT1);

                   }
                   else if(g_InputPort[i].eIPMux == MUX_COMBO4)
                   {
                        msWrite2ByteMask(REG_DVI5V_DETECT_345, DVI5V_DETECT_4, DVI5V_DETECT_4 ); // enable DVI 5V detect
                        msWriteByte(REG_0011AE+COMBO_OFFSET(MUX_COMBO4), 0x00);
                        msWrite2ByteMask(REG_0011A8+COMBO_OFFSET(MUX_COMBO4), BIT13|BIT1, _BIT13|_BIT1);

                   }
                   else if(g_InputPort[i].eIPMux == MUX_COMBO5)
                   {
                        msWrite2ByteMask(REG_DVI5V_DETECT_345, DVI5V_DETECT_5, DVI5V_DETECT_5 ); // enable DVI 5V detect
                        msWriteByte(REG_0011AE+COMBO_OFFSET(MUX_COMBO5), 0x00);
                        msWrite2ByteMask(REG_0011A8+COMBO_OFFSET(MUX_COMBO5), BIT13|BIT1, _BIT13|_BIT1);

                   }
             }
         }

    }
    else
    {
            msWrite2ByteMask(REG_DVI5V_DETECT_01, 0x00, DVI5V_DETECT_1|DVI5V_DETECT_0 ); // enable DVI 5V detect
            msWrite2ByteMask(REG_DVI5V_DETECT_2, 0x00, DVI5V_DETECT_2 ); // enable DVI 5V detect
            msWrite2ByteMask(REG_DVI5V_DETECT_345, 0x00, DVI5V_DETECT_5|DVI5V_DETECT_4|DVI5V_DETECT_3 ); // enable DVI 5V detect
    }
}
#endif

//**************************************************************************
//  [Function Name]:
void msPM_EnableClockRterm(BOOL bEnable)
{
      BYTE XDATA i;

    if(bEnable)
    {
         for(i=0; i<Input_Nums; i++)
         {
             if(INPUT_IS_TMDS(i))
             {
                   if(g_InputPort[i].eIPMux == MUX_COMBO0)
                            msWriteByteMask(REG_001781, (!bPMPortSkipIndex[i] ?0 :(BIT0)), _BIT0);   //
                   else if(g_InputPort[i].eIPMux == MUX_COMBO1)
                           msWriteByteMask(REG_001781, (!bPMPortSkipIndex[i] ?0 :(BIT1)), _BIT1);   //
                   else if(g_InputPort[i].eIPMux == MUX_COMBO2)
                           msWriteByteMask(REG_001781, (!bPMPortSkipIndex[i] ?0 :(BIT2)), _BIT2);   //
                   else if(g_InputPort[i].eIPMux == MUX_COMBO3)
                           msWriteByteMask(REG_001781, (!bPMPortSkipIndex[i] ?0 :(BIT3)), _BIT3);   //
                   else if(g_InputPort[i].eIPMux == MUX_COMBO4)
                           msWriteByteMask(REG_001781, (!bPMPortSkipIndex[i] ?0 :(BIT4)), _BIT4);   //
                   else if(g_InputPort[i].eIPMux == MUX_COMBO5)
                          msWriteByteMask(REG_001781, (!bPMPortSkipIndex[i] ?0 :(BIT5)), _BIT5);   //
             }

         }
    }
    else
    {
                  msWriteByteMask(REG_001781, 0x3F, 0x3F);   //
    }

}


void msPM_EnableDataRterm(BOOL bEnable)
{
      BYTE XDATA i;

    if(bEnable)
    {
         for(i=0; i<Input_Nums; i++)
         {
             if(INPUT_IS_TMDS(i))
             {
                   if(g_InputPort[i].eIPMux == MUX_COMBO0)
                            msWriteByteMask(REG_0017C4, 0x00, 0x0F);   //
                   else if(g_InputPort[i].eIPMux == MUX_COMBO1)
                           msWriteByteMask(REG_0017C4, 0x00, 0xF0);   //
                   else if(g_InputPort[i].eIPMux == MUX_COMBO2)
                           msWriteByteMask(REG_0017C5, 0x00, 0x0F);   //
                   else if(g_InputPort[i].eIPMux == MUX_COMBO3)
                           msWriteByteMask(REG_0017A2, 0x00, 0x0F);   //
                   else if(g_InputPort[i].eIPMux == MUX_COMBO4)
                           msWriteByteMask(REG_0017A2, 0x00, 0xF0);   //
                   else if(g_InputPort[i].eIPMux == MUX_COMBO5)
                          msWriteByteMask(REG_0017A3, 0x00, 0x0F);   //
             }

         }
    }
    else
    {
                  msWriteByteMask(REG_0017C4, 0x0F, 0x0F);   //
                  msWriteByteMask(REG_0017C4, 0xF0, 0xF0);   //
                  msWriteByteMask(REG_0017C5, 0x0F, 0x0F);   //
                  msWriteByteMask(REG_0017A2, 0x0F, 0x0F);   //
                  msWriteByteMask(REG_0017A2, 0xF0, 0xF0);   //
                  msWriteByteMask(REG_0017A3, 0x0F, 0x0F);   //
    }

}

void msPM_SetDVIRawClk(void)
{

      msWriteByteMask(REG_00038E, 0x00, 0xFF);
      msWriteByteMask(REG_DVIRAW_0, 0x00, 0x0F);   // DVI RAW clock (A/B)not gating
      msWriteByteMask(REG_DVIRAW_12, 0x00, 0xFF);   // DVI RAW clock (A/B)not gating
      msWriteByteMask(REG_DVIRAW_34, 0x00, 0xFF);   // DVI RAW clock (A/B)not gating
      msWriteByteMask(REG_DVIRAW_5, 0x00, 0x0F);   // DVI RAW clock (A/B)not gating

}
//                  msPM_EnableDVIDetect(BOOL bEnable)
//  [Description]
//                  Enable DVI Clock Detect -using DVI control clock
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void msPM_EnableDVIDetect(BOOL bEnable)
{
    BYTE i,bWriteBit;

    for(i =0; i< Input_Nums; i++)
    {
        if(INPUT_IS_TMDS(i) && !bPMPortSkipIndex[i])
                break;
    }



    if( bEnable && (i<Input_Nums))
    {

#if PM_SUPPORT_DVI_TIME_SHARE
        msPM_EnableDVIClockAmp(TRUE);
#endif

#if PM_SUPPORT_DVI5V_TurnonRterm
        msPM_EnableDVI5VDetect(TRUE);
#endif
        msPM_SetDVIRawClk();
        msPM_EnableDataRterm(FALSE);
        msPM_EnableClockRterm(TRUE);

        msWrite2ByteMask(REG_DVI_DET, 0, EN_DVI_DET_MASK );
        for(i=0; i<Input_Nums; i++) // Detection channel selection
        {
           bWriteBit = INPUT_IS_TMDS(i) && (!bPMPortSkipIndex[i]) && bEnable;
             //if(INPUT_IS_TMDS(i) && (!bPMPortSkipIndex[i]) && bEnable)
             switch(g_InputPort[i].eIPMux )
             {
                 case MUX_COMBO0:
                     msWrite2ByteMask(REG_DVI_DET, bWriteBit?EN_DVI_DET_0: 0, EN_DVI_DET_0 );
                     break;
                 case MUX_COMBO1:
                     msWrite2ByteMask(REG_DVI_DET, bWriteBit?EN_DVI_DET_1: 0, EN_DVI_DET_1 );
                     break;
                 case MUX_COMBO2:
                     msWrite2ByteMask(REG_DVI_DET, bWriteBit?EN_DVI_DET_2: 0, EN_DVI_DET_2 );
                     break;
                 case MUX_COMBO3:
                     msWrite2ByteMask(REG_DVI_DET, bWriteBit?EN_DVI_DET_3: 0, EN_DVI_DET_3 );
                     break;
                 case MUX_COMBO4:
                    #if CHIP_ID == MST9U2
                     msWrite2ByteMask(REG_DVI_DET, bWriteBit?EN_DVI_DET_5: 0, EN_DVI_DET_5 );
                    #else
                     msWrite2ByteMask(REG_DVI_DET, bWriteBit?EN_DVI_DET_4: 0, EN_DVI_DET_4 );
                    #endif
                     break;
                 case MUX_COMBO5:
                     msWrite2ByteMask(REG_DVI_DET, bWriteBit?EN_DVI_DET_5: 0, EN_DVI_DET_5 );
                     break;
                 default:
                     break;
             }
        }



#if 0//!ENABLE_MHL
        hw_Set_HdcpHpd();//PortA
        hw_Set_HdcpHpd2();//PortB
        hw_Set_HdcpHpd3();//PortC
#endif
    }
    else
    {
        BYTE XDATA u8RegOffset;

        msWrite2ByteMask(REG_DVI_DET, 0, EN_DVI_DET_MASK);
        msWriteByte(REG_0017A7, 0x3F); //power down clock amp
       // msWriteByte(REG_001781, 0x3F); //power down DVI_A RCK
        for(u8RegOffset=MUX_COMBO0; u8RegOffset<MUX_COMBO5; u8RegOffset++)
        {
               msWriteByteMask(REG_1120D6+COMBO_OFFSET(u8RegOffset), _BIT3, _BIT3);   // DVI RAW clock (A/B)not gating
               msWriteByteMask(REG_1120D7+COMBO_OFFSET(u8RegOffset), _BIT0, _BIT0);   // DVI RAW clock (A/B)not gating
               msWriteByteMask(REG_1120B6+COMBO_OFFSET(u8RegOffset), 0xF, 0x0F);   // DVI RAW clock (A/B)not gating
        }


        msPM_EnableDataRterm(FALSE);
        msPM_EnableClockRterm(FALSE);
#if 0//!ENABLE_MHL
        hw_Clr_HdcpHpd();//PortA
        hw_Clr_HdcpHpd2();//PortB
        hw_Clr_HdcpHpd3();//PortC
#endif
    }
}
#endif  // end of #if( PM_SUPPORT_WAKEUP_DVI )

#if ( PM_SUPPORT_WAKEUP_CEC == 1)
//**************************************************************************
//  [Function Name]:
//                  msPM_GetCECRegOffset(BOOL bEnable)
//  [Description]
//                  msPM_GetCECRegOffset
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE msPM_GetCECRegOffset(BYTE ucIdx)
{
	WORD wOffset = 0;

	switch (ucIdx)
	{
		case 0:
            wOffset = 0;
        break;
        case 1:
            wOffset = 0x80;
        break;
        case 2:
            wOffset = 0x100;
        break;
        case 3:
            wOffset = 0x180;
        break;
		default:
			wOffset = 0x00;
		break;
	}
	return wOffset;
}

//**************************************************************************
//  [Function Name]:
//                  msPM_EnableCECDetect(BOOL bEnable)
//  [Description]
//                  msPM_EnableCECDetect
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void msPM_EnableCECDetect(BOOL bEnable)
{
	BYTE i = 0;
	WORD clkCnt = 0;

	if (bEnable == TRUE)
	{
		WORD wOffset = 0;

		for ( i = 0; i < CEC_SET_NUM; i++ )
		{
			wOffset = msPM_GetCECRegOffset(i);
			//CEC settings
			msWriteByteMask(REG_000601 + wOffset, CEC_RETRY_CNT, 0x07); //retry cnt
			msWriteByte(REG_000603 + wOffset, ((CEC_BUS_FREE_TIME << 4) | CEC_RESEND_INTERVAL)); //[3:0]: cnt1, [7:4]: cnt2
		    msWriteByte(REG_000604 + wOffset,  ((CEC_LA_TV << 4) | CEC_FRAME_INTERVAL)); //[3:0]: cnt3, [7:4]: logical address: TV

		    //Number of counts to achieve 10us
		    clkCnt = (CRYSTAL_CLOCK % 100000L)*0.00016 + 0.5;
		    msWriteByte(REG_000605 + wOffset, CRYSTAL_CLOCK/100000L); // integer part
		    msWriteByteMask(REG_000606 + wOffset, clkCnt, 0x0F); // CEC time unit by Xtal(fractional)

			//select sw CEC ontroller
			msWriteByteMask(REG_000628 + wOffset, 0x01, 0x01); //[1]: 0: clock source from xtal

			//[1:0]: tx rising shift 0us, [3:2]: tx falling shift 0us, [4]: cancel current tx = false, [5]: CEC main clock no gating,
		    //[6]: rx test mode:normal, [7]: enable PCM CEC controller
		    msWriteByte(REG_000602 + wOffset, 0x80);

			//setting wakeup event
			msWriteByte(REG_00060F + wOffset, 0x24); //enable wake up for OP2 operand, and OP4 length = 2
			msWriteByte(REG_000610 + wOffset, 0x04); // OPCODE0: Image View On
			msWriteByte(REG_000611 + wOffset, 0x0D); // OPCODE1: Text View ON

			msWriteByte(REG_000612 + wOffset, 0x44); // OPCODE2: E_MSG_UI_PRESS
			msWriteByte(REG_000617 + wOffset, 0x40); // OPCODE2 operand: power
			msWriteByte(REG_000618 + wOffset, 0x6D); // OPCODE2 operand: power on

			msWriteByte(REG_000614 + wOffset, 0x82); // OPCODE4: Active Source

			msWriteByte(REG_00061B + wOffset, (0x80 | HDMI_CEC_VER)); //CEC version, boardcasting command

			//enable OP code wakeup
			msWriteByte(REG_00060E + wOffset, 0x37); //enable OPCODE 0, 1, 2, 4 wakeup

			//enable CEC hw mode
			msWriteByteMask(REG_000607 + wOffset, 0x10, 0x10); //[4]: select CEC controller, 0:software / 1: PM HW
		}

	}

}

#endif

//**************************************************************************
//  [Function Name]:
//                  msPM_EnableSARDetect(BOOL bEnable)
//  [Description]
//                  msPM_EnableSARDetect
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void msPM_EnableSARDetect(BOOL bEnable)
{
    #if ENABLE_SW_SARMODE
    sPMSARKey.u8SAR_KeyMask = 0x00;
    sPMSARKey.u8SAR_CmpLvl = 0x00;
    #endif

    if (bEnable)
    {
        switch(sPMInfo.sPMConfig.ePMSARmode)
        {
            case ePMSAR_SAR0:
                #if ENABLE_SW_SARMODE //replace sw mode to avoid affect the MHL cable detection
                sPMSARKey.u8SAR_KeyMask = BIT7|BIT0;
                sPMSARKey.u8SAR_CmpLvl = EN_SAR_14V;
                sPMInfo.sPMConfig.ePMSARmode = ePMSAR_ALIVE;
                #else
                msWriteByteMask(REG_SINGLE_KEY, EN_SAR_SINGLE_SET, EN_SAR_SINGLE_MASK);
                msWriteByteMask(REG_SAR_CMP, EN_SAR_14V, EN_SAR_CMP_MASK);
                msWriteByteMask(REG_SAR_ANYKEY, 0, EN_SAR_ANYKEY_MASK);
                #endif
                break;
            case ePMSAR_SAR1:
                #if ENABLE_SW_SARMODE //replace sw mode to avoid affect the MHL cable detection
                sPMSARKey.u8SAR_KeyMask = BIT7|BIT1;
                sPMSARKey.u8SAR_CmpLvl = EN_SAR_05V;
                sPMInfo.sPMConfig.ePMSARmode = ePMSAR_ALIVE;
                #else
                msWriteByteMask(REG_SINGLE_KEY, 0x11, EN_SAR_SINGLE_MASK);
                msWriteByteMask(REG_SAR_CMP, EN_SAR_05V, EN_SAR_CMP_MASK);
                msWriteByteMask(REG_SAR_ANYKEY, 0, EN_SAR_ANYKEY_MASK);
                #endif
                break;
            case ePMSAR_SAR2:
                #if ENABLE_SW_SARMODE //replace sw mode to avoid affect the MHL cable detection
                sPMSARKey.u8SAR_KeyMask = BIT7|BIT2;
                sPMSARKey.u8SAR_CmpLvl = EN_SAR_05V;
                sPMInfo.sPMConfig.ePMSARmode = ePMSAR_ALIVE;
                #else
                msWriteByteMask(REG_SINGLE_KEY, 0x12, EN_SAR_SINGLE_MASK);
                msWriteByteMask(REG_SAR_CMP, EN_SAR_05V, EN_SAR_CMP_MASK);
                msWriteByteMask(REG_SAR_ANYKEY, 0, EN_SAR_ANYKEY_MASK);
                #endif
                break;
            case ePMSAR_SAR3:
                #if ENABLE_SW_SARMODE //replace sw mode to avoid affect the MHL cable detection
                sPMSARKey.u8SAR_KeyMask = BIT7|BIT3;
                sPMSARKey.u8SAR_CmpLvl = EN_SAR_05V;
                sPMInfo.sPMConfig.ePMSARmode = ePMSAR_ALIVE;
                #else
                msWriteByteMask(REG_SINGLE_KEY, 0x13, EN_SAR_SINGLE_MASK);
                msWriteByteMask(REG_SAR_CMP, EN_SAR_05V, EN_SAR_CMP_MASK);
                msWriteByteMask(REG_SAR_ANYKEY, 0, EN_SAR_ANYKEY_MASK);
                #endif
                break;
            case ePMSAR_SAR12:
				#if ENABLE_SW_SARMODE //replace sw mode to avoid affect the MHL cable detection
                sPMSARKey.u8SAR_KeyMask = BIT7|BIT1|BIT2;
                sPMSARKey.u8SAR_CmpLvl = EN_SAR_05V;
                sPMInfo.sPMConfig.ePMSARmode = ePMSAR_ALIVE;				
				#else
                msWriteByteMask(REG_SINGLE_KEY, 0, EN_SAR_SINGLE_MASK);
                msWriteByteMask(REG_SAR_CMP, EN_SAR_14V, EN_SAR_CMP_MASK);
                msWriteByteMask(REG_SAR_ANYKEY, EN_SAR_2CH_SET, EN_SAR_ANYKEY_MASK);
                msWriteByteMask(REG_003A64,0,BIT2);
				#endif
                break;
            case ePMSAR_SAR123:
				#if ENABLE_SW_SARMODE //replace sw mode to avoid affect the MHL cable detection
                sPMSARKey.u8SAR_KeyMask = BIT7|BIT1|BIT2|BIT3;
                sPMSARKey.u8SAR_CmpLvl = EN_SAR_05V;
                sPMInfo.sPMConfig.ePMSARmode = ePMSAR_ALIVE;				
				#else
                msWriteByteMask(REG_SINGLE_KEY, 0, EN_SAR_SINGLE_MASK);
                msWriteByteMask(REG_SAR_CMP, EN_SAR_14V, EN_SAR_CMP_MASK);
                msWriteByteMask(REG_SAR_ANYKEY, EN_SAR_3CH_SET, EN_SAR_ANYKEY_MASK);
                msWriteByteMask(REG_003A64,0,BIT2);
				#endif
                break;
            case ePMSAR_SAR01:
				#if ENABLE_SW_SARMODE
                sPMSARKey.u8SAR_KeyMask = BIT7|BIT0|BIT1;
                sPMSARKey.u8SAR_CmpLvl = EN_SAR_05V;
                sPMInfo.sPMConfig.ePMSARmode = ePMSAR_ALIVE;				
				#else
                msWriteByteMask(REG_SINGLE_KEY, 0, EN_SAR_SINGLE_MASK);
                msWriteByteMask(REG_SAR_CMP, EN_SAR_05V, EN_SAR_CMP_MASK);
                msWriteByteMask(REG_SAR_ANYKEY, EN_SAR_2CH_SET, EN_SAR_ANYKEY_MASK);
                msWriteByteMask(REG_003A64,BIT2,BIT2);
				#endif
                break;
            case ePMSAR_SAR012:
				#if ENABLE_SW_SARMODE
                sPMSARKey.u8SAR_KeyMask = BIT7|BIT0|BIT1|BIT2;
                sPMSARKey.u8SAR_CmpLvl = EN_SAR_05V;
                sPMInfo.sPMConfig.ePMSARmode = ePMSAR_ALIVE;				
				#else
                msWriteByteMask(REG_SINGLE_KEY, 0, EN_SAR_SINGLE_MASK);
                msWriteByteMask(REG_SAR_CMP, EN_SAR_05V, EN_SAR_CMP_MASK);
                msWriteByteMask(REG_SAR_ANYKEY, EN_SAR_3CH_SET, EN_SAR_ANYKEY_MASK);
                msWriteByteMask(REG_003A64,BIT2,BIT2);
				#endif
                break;
            default:
                break;
        }

        if(sPMInfo.sPMConfig.ePMSARmode == ePMSAR_ALIVE)
        {
           msWriteByteMask(REG_SAR_GPIO, 0, EN_SAR_DET_MASK);
           #if ENABLE_SW_SARMODE
           if(_bit7_(sPMSARKey.u8SAR_KeyMask)) //set sw mode
		   {
		      msWriteByteMask(REG_003A62,(sPMSARKey.u8SAR_KeyMask&0x0F),0x0F);
		   }
           #endif
        }
        else
        {
         msWriteByteMask(REG_003A62,sPMInfo.sPMConfig.ePMSARmode,0x0F);
         msWriteByteMask(REG_SAR_GPIO, EN_SAR_DET_SET, EN_SAR_DET_MASK);
        }
    }
    else
    {

        msWriteByteMask(REG_SAR_GPIO, 0, EN_SAR_DET_MASK);
        msWriteByteMask(REG_SAR_ANYKEY, 0, EN_SAR_ANYKEY_MASK);
        msWriteByteMask(REG_SINGLE_KEY, PM_SARKEY_GETVALUE? 0: PD_SAR, EN_SAR_SINGLE_MASK);

    }
    #if  ENABLE_DC_SDM_FORWARD
    msWriteByteMask(REG_003A66,DC_SDM_DET_SAR_EN,DC_SDM_DET_SAR_EN);
    #endif
}

#define SARTHR_25V      ((255ul*250)/330)
#define SARTHR_14V      ((255ul*140)/330)
#define SARTHR_05V      ((255ul*50)/330)
BOOL msPM_CompareSARChannel(void)
{
     static BYTE u8CmpThreshold = 0;
     BOOL bSAR_Result = FALSE;

     if(u8CmpThreshold == 0)
     {
        if(sPMSARKey.u8SAR_CmpLvl == EN_SAR_25V)
            u8CmpThreshold = SARTHR_25V;
        else if(sPMSARKey.u8SAR_CmpLvl == EN_SAR_14V)
            u8CmpThreshold = SARTHR_14V;
        else if(sPMSARKey.u8SAR_CmpLvl == EN_SAR_05V)
            u8CmpThreshold = SARTHR_05V;

     }
     if(_bit7_(sPMSARKey.u8SAR_KeyMask))
	 {
		if((_bit0_(sPMSARKey.u8SAR_KeyMask)) && (KEYPAD_SAR00 <= u8CmpThreshold))
            bSAR_Result = TRUE;
		else if((_bit1_(sPMSARKey.u8SAR_KeyMask)) && (KEYPAD_SAR01 <= u8CmpThreshold))
            bSAR_Result = TRUE;
		else if((_bit2_(sPMSARKey.u8SAR_KeyMask)) && (KEYPAD_SAR02 <= u8CmpThreshold))
            bSAR_Result = TRUE;
		else if((_bit3_(sPMSARKey.u8SAR_KeyMask)) && (KEYPAD_SAR03 <= u8CmpThreshold))
            bSAR_Result = TRUE;
	 }
     return bSAR_Result;
}
#if PM_CABLEDETECT_USE_GPIO
//**************************************************************************
//  [Function Name]:
//                  msPM_EnableGPIODetect(BOOL bEnable)
//  [Description]
//                  msPM_EnableGPIODetect
//  [Arguments]:
//
//  [Return]:

//**************************************************************************
void msPM_CableDetectStates()
{
           WORD INV_GPIO_POL_SET_Temp=INV_GPIO_POL_SET;
#if FPGA_PM
#if !(CABLE_DETECT_VGA_USE_SAR)
           if(hwDSUBCable_Pin)

                     INV_GPIO_POL_SET_Temp=(INV_GPIO_POL_SET_Temp)&~PM_VGACBL_DET;
#endif
#if !(CABLE_DETECT_VGA_USE_SAR)
           if(hwDVICable_Pin)

                     INV_GPIO_POL_SET_Temp=(INV_GPIO_POL_SET_Temp)&~PM_DVICBL_DET;
    #endif
#endif
#if ENABLE_DP_INPUT
    //if(!(DP_CABLE_NODET))
    if(!hwDPCable_Pin)
        INV_GPIO_POL_SET_Temp=(INV_GPIO_POL_SET_Temp)&~PM_DPCBL_DET;
#endif
#if 0
           if(DET_HDMIA)

                     INV_GPIO_POL_SET_Temp=(INV_GPIO_POL_SET_Temp)&~PM_HDMIACBL_DET;

           if(DET_HDMIB)

                     INV_GPIO_POL_SET_Temp=(INV_GPIO_POL_SET_Temp)&~PM_HDMIBCBL_DET;

           if(DET_HDMIC)

                     INV_GPIO_POL_SET_Temp=(INV_GPIO_POL_SET_Temp)&~PM_HDMICCBL_DET;
#endif


           msWrite2ByteMask(REG_000362,INV_GPIO_POL_SET_Temp, INV_GPIO_POL_MASK);

}
#endif

void msPM_EnableGPIODetect(BOOL bEnable)
{
	if(bEnable)
	{
#if  ENABLE_DAISY_CHAIN
		BYTE i;
		for(i =0; i< Input_Nums; i++)
		{
			if(INPUT_IS_DC_RX(i))
			{
				if (!bPMPortSkipIndex[i])
				{
				    msWrite2ByteMask(REG_000360, EN_GPIO_DET_SET, EN_GPIO_DET_MASK);
				}
				else
				{
				    msWrite2ByteMask(REG_000360, (EN_GPIO_DET_SET&(~PM_GPIO_DET_DC)), EN_GPIO_DET_MASK);
				}
				break;
			}
			else
				continue;
		}
#else
		msWrite2ByteMask(REG_000360, EN_GPIO_DET_SET , EN_GPIO_DET_MASK);
#endif
		msWrite2ByteMask(REG_000362, INV_GPIO_POL_SET, INV_GPIO_POL_MASK);
#if  PM_CABLEDETECT_USE_GPIO
		msPM_CableDetectStates();
#endif
	}
	else
	{
		msWrite2ByteMask(REG_000360, 0x00, EN_GPIO_DET_MASK);
		msWrite2ByteMask(REG_000362, 0x00, INV_GPIO_POL_MASK);
	}
}
void msPM_EnableIRDetect(BOOL bEnable)
{
   BYTE u8IrType = sPMInfo.sPMConfig.sPMIRmode.IrType;
   if(bEnable)
   {
        switch(u8IrType)
        {
           case IR_NEC:
           case IR_RC5:
           case IR_RC6:
           default:
                       break;
        }
   }
   else
   {
        msWriteByteMask(REG_003D01, 0, _BIT0); //disable RC receiver
        msWriteByteMask(REG_003D14, 0, _BIT1|_BIT0); //disable addrcmp and anykey
        msWriteByteMask(REG_003D80, 0, _BIT0); //disable decode for full/raw mode
        msWriteByteMask(REG_003DB4, 0, _BIT7|_BIT6); //disable decode for full/raw mode

   }



}

//**************************************************************************
//  [Function Name]:
//                  msPM_EnableMCCSDetect(BOOL bEnable)
//  [Description]
//                  msPM_EnableMCCSDetect
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void msPM_EnableMCCSDetect(BOOL bEnable)
{
    if(bEnable)
    {
        msWriteByte(REG_003E0A, 0xB7);  // enable DDC2Bi for A0
        msWriteByte(REG_003E0C, 0xB7);  // enable DDC2Bi for D0
        msWriteByte(REG_003E0D, 0xB7);  // enable DDC2Bi for D1
        msWriteByte(REG_003E52, 0xB7);  // enable DDC2Bi for D2
        msWriteByte(REG_003F0C, 0xB7);  // enable DDC2Bi for D3
        msWriteByte(REG_003F0D, 0xB7);  // enable DDC2Bi for D4
        msWriteByte(REG_003F52, 0xB7);  // enable DDC2Bi for D5
        msWriteByteMask(REG_003E09, 0, (BIT1|BIT0)); //disable No ACK ,Hold CLK
        msWriteByteMask(REG_003F09, 0, (BIT1|BIT0)); //disable No ACK ,Hold CLK
        msWriteByteMask(REG_003EC1, BIT7|BIT6, BIT7|BIT6); // enable A0, D0
        msWriteByteMask(REG_003EEF, BIT7|BIT6, BIT7|BIT6); // enable D1, D2
        msWriteByteMask(REG_003FC1, BIT7, BIT7);           // enable D3
        msWriteByteMask(REG_003FEF, BIT7|BIT6, BIT7|BIT6); // enable D4, D5
        if(PM_CLOCK == RCOSC)
             msWriteByteMask(REG_000390, BIT4, BIT4|BIT5);//12M
        else
             msWriteByteMask(REG_000390, 0x00, BIT4|BIT5); //Xtal

    }
    else
    {
        msWriteBit(REG_003E0A, FALSE, BIT7);  // disable DDC2Bi for A0
        msWriteBit(REG_003E0C, FALSE, BIT7);  // disable DDC2Bi for D0
        msWriteBit(REG_003E0D, FALSE, BIT7);  // disable DDC2Bi for D1
        msWriteBit(REG_003E52, FALSE, BIT7);  // disable DDC2Bi for D2
        msWriteBit(REG_003F0C, FALSE, BIT7);  // disable DDC2Bi for D3
        msWriteBit(REG_003F0D, FALSE, BIT7);  // disable DDC2Bi for D4
        msWriteBit(REG_003F52, FALSE, BIT7);  // disable DDC2Bi for D5
        msWriteByteMask(REG_003EC1, 0, BIT7|BIT6); // disable A0, D0
        msWriteByteMask(REG_003EEF, 0, BIT7|BIT6); // disable D1, D2
        msWriteByteMask(REG_003FC1, 0, BIT7);      // disable D3
        msWriteByteMask(REG_003FEF, 0, BIT7|BIT6); // disable D4, D5
    }
}


//**************************************************************************
//  [Function Name]:
//                  msPM_EDID_READ()
//  [Description]
//                  msPM_EDID_READ
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void msPM_Enable_EDID_READ(BOOL bEnable)
{
    if(bEnable)
    {
        msWriteByteMask(REG_003E45, BIT7, BIT7);  // enable DDC function for DVI_0
        msWriteByteMask(REG_003E4D, BIT7, BIT7);  // enable DDC function for DVI_1
        msWriteByteMask(REG_003E49, BIT7, BIT7);  // enable DDC function for ADC_0
        msWriteByteMask(REG_003E58, BIT7, BIT7);  // enable DDC function for DVI_2
        msWriteByteMask(REG_003F45, BIT7, BIT7);  // enable DDC function for DVI_3
        msWriteByteMask(REG_003F4D, BIT7, BIT7);  // enable DDC function for DVI_4
        msWriteByteMask(REG_003F58, BIT7, BIT7);  // enable DDC function for DVI_5
    }
    else
    {
        msWriteByteMask(REG_003E45, 0x00, BIT7);  // disable DDC function for DVI_0
        msWriteByteMask(REG_003E4D, 0x00, BIT7);  // disable DDC function for DVI_1
        msWriteByteMask(REG_003E49, 0x00, BIT7);  // disable DDC function for ADC_0
        msWriteByteMask(REG_003E58, 0x00, BIT7);  // disable DDC function for DVI_2
        msWriteByteMask(REG_003F45, 0x00, BIT7);  // disable DDC function for DVI_3
        msWriteByteMask(REG_003F4D, 0x00, BIT7);  // disable DDC function for DVI_4
        msWriteByteMask(REG_003F58, 0x00, BIT7);  // disable DDC function for DVI_5
    }
}




//**************************************************************************
//  [Function Name]:
//                  msPM_OutputTriState()
//  [Description]
//                  msPM_OutputTriState
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void msPM_OutputTriState(void)
{
        msWriteByteMask(REG_000406, 0x00, 0xEF); //disable SPDIF
        msWriteByteMask(REG_000407, 0x00, 0xF7); //disable i2s
        msWriteByte(REG_000416, 0xFF); //disable SPDIF

        msWrite3Byte(REG_14064A, 0x000000);
        msWriteByteMask(REG_14064E, 0x00, 0x1F);
        //config TTL mode
        msWrite2Byte(REG_140620, 0x0000);
        msWrite2Byte(REG_140622, 0x0000);
        msWrite2Byte(REG_140624, 0x0000);
        msWrite2Byte(REG_140782, 0x0000);
        msWrite2Byte(REG_140784, 0x0000);
        msWrite2Byte(REG_140786, 0x0000);
       //set TTL  open drain
        msWrite2Byte(REG_1407D2, 0xFFFF);
        msWriteByte(REG_1407D4, 0xFF);
        msWrite2Byte(REG_1407C6, 0xFFFF);
        msWrite2Byte(REG_1407C8, 0xFFFF);
        msWrite2Byte(REG_1407CA, 0xFFFF);



	 //DP   Cable 5V off.   Set Output low
	msWriteByteMask(_REG_GPIO_X0_OUT, FALSE, BIT1); // COMBO0
	msWriteByteMask(_REG_GPIO_X0_OEZ, FALSE, BIT1); //

	msWriteByteMask(_REG_GPIO_X1_OUT, FALSE, BIT1); // COMBO1
	msWriteByteMask(_REG_GPIO_X1_OEZ, FALSE, BIT1); //



}


//**************************************************************************
//  [Function Name]:
//                  msPM_PowerDownMacro()
//  [Description]
//                  msPM_PowerDownMacro
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void msPM_PowerDownMacro(void)
{
    // printData("\r\n PM_DISABLE_MOD ", 0);
    PM_DISABLE_MOD();
    //printData("\r\n PM_DISABLE_AUDIO ", 0);
    PM_DISABLE_AUDIO();
    //printData("\r\n PM_DISABLE_CHIPTOP ", 0);
    PM_DISABLE_CHIPTOP();
    //  printData("\r\n PM_DISABLE_LPLL ", 0);
    PM_DISABLE_LPLL();
    // to avoid the aux status error.
    PM_DISABLE_COMBOAUXSTATUS();
    PM_printMsg("\r\n PM_DISABLE_MACRO ");
    msPM_InterruptEnable(FALSE);
}
//**************************************************************************
//  [Function Name]:
//                  msPM_PowerUpMacro()
//  [Description]
//                  msPM_PowerUpMacro
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void msPM_PowerUpMacro(void)
{
#if ENABLE_DRAM_SELFREFRESH
    Init_Dram();
#endif
	// sar
	msWriteByte(REG_003A24,0x00);
}
//**************************************************************************
//  [Function Name]:
//                  msPM_MCCSReset()
//  [Description]
//                  msPM_MCCSReset
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void msPM_MCCSReset(void)
{
    msWriteByte(REG_003EC0, 0xFF); // clear status of A0, D0
    msWriteByte(REG_003EC0, 0x00);
    msWriteByte(REG_003EEE, 0xFF); // clear status of D1, D2
    msWriteByte(REG_003EEE, 0x00);

    msWriteByte(REG_003FC0, 0xFF); // clear status of A0, D3
    msWriteByte(REG_003FC0, 0x00);
    msWriteByte(REG_003FEE, 0xFF); // clear status of D4, D5
    msWriteByte(REG_003FEE, 0x00);
}

#if( PM_SUPPORT_AC2DC )
//**************************************************************************
//  [Function Name]:
//                  msPM_EnableAC2DC()
//  [Description]
//                  msPM_EnableAC2DC
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void msPM_EnableAC2DC(Bool bEnable)
{
    if(bEnable)   /*Turn On PM*/
    {
        _msWriteByteMask(REG_PM_A8, 0x00, 0xF0);       /*AC2DC controler clock bypass*/
        _msWriteByte(REG_PM_C0, AC2DC_MODE);     //AC2DC controler enable
        _msWriteByte(REG_PM_C2, OFF_PERIOD);     //AC2DC Off period
        _msWriteByte(REG_PM_C6, 0x88);       //AC detect voltage
        _msWriteByte(REG_PM_C4, AC2DC_PAD);          //AC2DC pad function enable(GPIO2)
    }
    else /*Turn Off PM*/
    {
        _msWriteByteMask(REG_PM_A8, 0xB0, 0xF0);       /*AC2DC controler clock bypass*/
        _msWriteByte(REG_PM_C4, 0x00);           //AC2DC pad function disable(GPIO2)
        _msWriteByte(REG_PM_C0, OFF_MODE);       //AC2DC controler disable
        _msWriteByte(REG_PM_C2, 0x00);       /*Off period*/
        _msWriteByte(REG_PM_C6, 0x00);
    }
}
#endif  // end of #if( PM_SUPPORT_AC2DC )

BYTE msPM_GetActiveDVIPort(BYTE u8DVI86_Status, BYTE u8DVI8B_Status)
{
     BYTE i;

     // for DVI 0 clcok detect
     for(i = 0; i< Input_Nums; i++)
     {

            if(INPUT_IS_TMDS(i)
                 && (g_InputPort[i].eIPMux== MUX_COMBO0)
                 && IsDetDVIClk(i, u8DVI86_Status)
                 && !bPMPortSkipIndex[i] )
                     break;
     }

     if(i < Input_Nums)
         return i;



    // for DVI 1~5 clock detect
     for(i = 0; i< Input_Nums; i++)
     {

            if(INPUT_IS_TMDS(i)
                 && (g_InputPort[i].eIPMux>= MUX_COMBO1 && g_InputPort[i].eIPMux<= MUX_COMBO5)
                 && IsDetDVIClk(i, u8DVI8B_Status)
                 && !bPMPortSkipIndex[i] )
                     break;
     }

     if(i < Input_Nums)
         return i;
     else
         return (Input_Nums-1);
}



BYTE msPM_GetActiveDPPort(BYTE u8InputPort, Bool bScanAll)
{
    if(bScanAll)
    {
        BYTE i;
        for(i=0; i<Input_Nums; i++)
        {

            if(INPUT_IS_DISPLAYPORT(i) &&
            (g_InputPort[i].eIPMux>= MUX_COMBO0 && g_InputPort[i].eIPMux<= MUX_COMBO5))
            {

#if ENABLE_DAISY_CHAIN
                if(INPUT_IS_DC_RX(i))
                {
                    continue;
                }
#endif

                if(msPM_GetDPSquelch(g_InputPort[i].eIPMux, 1) && (!bPMPortSkipIndex[i]))
                {
                    return i;
                }
                if(((msReadByte(REG_0011E3+COMBO_OFFSET(g_InputPort[i].eIPMux))&(BIT0|BIT1)) == 2) && (!bPMPortSkipIndex[i]))   // DVI RAW clock (A/B)not gating
                {
                    return i;
                }

            }
        }
    }
    else
    {
#if ENABLE_DAISY_CHAIN
        if (INPUT_IS_DC_RX(u8InputPort))
        {
            return 0xFF;
        }
        else
#endif
        if(g_InputPort[u8InputPort].eIPMux>= MUX_COMBO0 && g_InputPort[u8InputPort].eIPMux<= MUX_COMBO5)
        {
            if(msPM_GetDPSquelch(g_InputPort[u8InputPort].eIPMux, 0x1FFF)&&(!bPMPortSkipIndex[u8InputPort]))
                return u8InputPort;
            if(((msReadByte(REG_0011E3+COMBO_OFFSET(g_InputPort[u8InputPort].eIPMux))&(BIT0|BIT1)) == 2)&&(!bPMPortSkipIndex[u8InputPort]))   //
                return u8InputPort;

        }
     }

     return 0xFF;
}


BYTE msPM_GetActiveMHLPort(Bool bPowerSaving)
{
    Bool bWakeUp = FALSE;
    BYTE ucComboPort = 0;

    for(ucComboPort = 0; ucComboPort < Input_Nums; ucComboPort++)
    {
        if(INPUT_IS_MHL(ucComboPort) && (g_InputPort[ucComboPort].eIPMux >= MUX_COMBO0 && g_InputPort[ucComboPort].eIPMux <= MUX_COMBO5))
        {
            bWakeUp = mdrv_mhl_CbusWakeupDetect(g_InputPort[ucComboPort].eIPMux -1);

            if(bWakeUp && bPowerSaving && !bPMPortSkipIndex[ucComboPort])
            {
                return ucComboPort;
            }
        }
    }

    return 0xFF;
}

//**************************************************************************
//  [Function Name]:
//                  msPM_GetPMStatus()
//  [Description]
//                  msPM_GetPMStatus
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool msPM_GetPMStatus(void)
{
    BOOL bResault = FALSE;
    BYTE XDATA u8ActivePort = 0xFF;

    BYTE XDATA ucStatus_85;
    BYTE XDATA ucStatus_86;
    BYTE XDATA ucStatus_8B;
    BYTE XDATA ucStatus_A2;
    BYTE XDATA ucStatus_A3;

    volatile WORD XDATA ucStatus_GPIO;

    ucWakeupStatus = ePMSTS_NON;

    ucStatus_85   = msReadByte(REG_EVENT1);
    ucStatus_86   = msReadByte(REG_EVENT2);
    ucStatus_8B   = msReadByte(REG_EVENT3);
    ucStatus_A2   = msReadByte(REG_0003A2);
    ucStatus_A3   = msReadByte(REG_EVENT4);
    ucStatus_GPIO = msRead2Byte( REG_000364 );

#if PM_POWERkEY_GETVALUE
    if(HW_POWER_KEY==0)
    {
        KeypadButton = BTN_NOTHING;                //110914 Rick add for enable OSDLock function while PM On - A023
        Key_ScanKeypad();
        ucWakeupStatus =ePMSTS_POWERGPIO_ACT;
        bResault = TRUE;
    }
    else
#endif
    #if PM_CABLE_DETECT_USE_SAR
    if((abs(sPMInfo.bCABLE_SAR_VALUE-CABLE_DET_SAR)>5)
         &&(sPMInfo.ucPMMode == ePM_POWER_DPMS))
        {
              ucWakeupStatus =ePMSTS_CABLESAR_ACT;
        bResault = TRUE;
    }
    else
    #endif

    if((HVSYNC_ACT(ucStatus_A3) && HVSYNC_CHK(ucStatus_85)) || (SOGDet_ACT(ucStatus_A3) && SOG_CHK(ucStatus_85))||(CSYNC_ACT(ucStatus_A2) &&CSYNC_CHK(ucStatus_86 )))
    //if(HVSYNC_ACT(ucStatus_A3) && (HVSYNC_CHK(ucStatus_85) || CSYNC_CHK(ucStatus_86) || SOG_CHK(ucStatus_85)))
    {
        ucWakeupStatus = ePMSTS_VGA_ACT;
        u8WakeupInputSrc = Input_Analog1;
        if(SOG_CHK(ucStatus_85))
            u8WakeupSyncStatus = ePM_VGA_SOGSYNC;
        else if(CSYNC_CHK(ucStatus_86))
            u8WakeupSyncStatus = ePM_VGA_CSYNC;
        else
             u8WakeupSyncStatus = ePM_VGA_HVSYNC;

        #if FPGA_PM
        SrcInputType = Input_Analog1;
        #endif
             bResault = TRUE;
    }
    else if( ucStatus_GPIO&EN_GPIO_DET_MASK)
    {
        ucWakeupStatus = ePMSTS_GPIO_ACT;
        bResault = TRUE;
    }
    else
    {

        if( ucStatus_86 & CEC_WAKEUP )
        {
        	#if (PM_SUPPORT_WAKEUP_CEC == 1)
	            ucWakeupStatus = ePMSTS_CEC_ACT;
	            bResault = TRUE;
			#endif
        }
        else if( ucStatus_86 & D2B_WAKEUP )
        {
//#define D2B_PWR_CMD_WKUP_STATUS()   (_msReadByte( REG_3EC1 ))
          #if CHIP_FAMILY_TYPE == CHIP_FAMILY_TSUMC||CHIP_FAMILY_TYPE == CHIP_FAMILY_MST9U|| CHIP_FAMILY_TYPE == CHIP_FAMILY_TSUMJ
            volatile BYTE mccsWakeUpStatus;
            if(msReadByte(REG_003EC2)&BIT2) // A0
                mccsWakeUpStatus = msReadByte( REG_003EC4 );
            else if(msReadByte(REG_003EC2)&BIT3) // D0
                mccsWakeUpStatus = msReadByte( REG_003EC5 );
            else if(msReadByte(REG_003EF0)&BIT2) // D1
                mccsWakeUpStatus = msReadByte( REG_003EF2 );
            else if(msReadByte(REG_003EF0)&BIT3) // D2
                mccsWakeUpStatus = msReadByte( REG_003EF3 );
          #if CHIP_ID == MST9U3
            else if(msReadByte(REG_003EB6)&BIT6) // D3
                mccsWakeUpStatus = msReadByte( REG_003EB9 );
          #else
            else if(msReadByte(REG_003FC2)&BIT3) // D3
                mccsWakeUpStatus = msReadByte( REG_003FC5 );
            else if(msReadByte(REG_003FF0)&BIT2) // D4
                mccsWakeUpStatus = msReadByte( REG_003FF2 );
            else if(msReadByte(REG_003FF0)&BIT3) // D5
                mccsWakeUpStatus = msReadByte( REG_003FF3 );
          #endif

            if( mccsWakeUpStatus == 0x05 )
                ucWakeupStatus = ePMSTS_MCCS05_ACT;
            else if( mccsWakeUpStatus == 0x04 )
                ucWakeupStatus = ePMSTS_MCCS04_ACT;
            else
                ucWakeupStatus = ePMSTS_MCCS01_ACT;
          #else
            volatile BYTE mccsWakeUpStatus = msReadByte( REG_003EC1);
            if( mccsWakeUpStatus & BIT7 )
                ucWakeupStatus = ePMSTS_MCCS05_ACT;
            else if( mccsWakeUpStatus & BIT6 )
                ucWakeupStatus = ePMSTS_MCCS04_ACT;
            else
                ucWakeupStatus = ePMSTS_MCCS01_ACT;
          #endif
            bResault = TRUE;
        }

        else if( ucStatus_86 & SAR_IN_DET )
        {
              ucWakeupStatus = ePMSTS_SAR_ACT;
               bResault = TRUE;
        }
   #if ENABLE_SW_SARMODE
        else if( msPM_CompareSARChannel())
        {
              ucWakeupStatus = ePMSTS_SAR_ACT;
               bResault = TRUE;
        }

   #endif
        //================================================================================
        //  Move DP wake up checking first before DVI, because DVI clock detected bit would report 1 when DP plug in.
        //================================================================================
   #if ((CHIP_FAMILY_TYPE == CHIP_FAMILY_TSUMC)||(CHIP_FAMILY_TYPE == CHIP_FAMILY_MST9U))
   #if DISABLE_AUTO_SWITCH
         else if(sPMInfo.sPMConfig.bDP_enable &&
                     ((u8ActivePort = msPM_GetActiveDPPort(FIXED_DP_PORT, FALSE)) != 0xFF))
   #else
         else if(sPMInfo.sPMConfig.bDP_enable &&
                      ((u8ActivePort =msPM_GetActiveDPPort(0xFF, TRUE)) != 0xFF))
   #endif
        {
                   u8WakeupInputSrc = u8ActivePort;
                   ucWakeupStatus = ePMSTS_DP_ACT;
                   bResault = TRUE;
        }
    #endif

#if( PM_SUPPORT_WAKEUP_DVI )
       else if( ucStatus_86 & DVI_CLK_DET_0 )
       {
           u8WakeupInputSrc = msPM_GetActiveDVIPort(ucStatus_86, ucStatus_8B);;
           ucWakeupStatus = ePMSTS_DVI_ACT;
        //SrcInputType = Input_Digital; // 120117 coding reserved
           PM_printData("\r\n DVI_CLK_DET_0 ", 0);
        bResault = TRUE;

        }
        else if( ucStatus_8B & (DVI_CLK_DET_1|DVI_CLK_DET_2 |DVI_CLK_DET_3|DVI_CLK_DET_4|DVI_CLK_DET_5))
        {
            u8WakeupInputSrc = msPM_GetActiveDVIPort(ucStatus_86, ucStatus_8B);
            ucWakeupStatus = ePMSTS_DVI_ACT;//ePMSTS_DVI_1_ACT;
            PM_printData("\r\n DVI_CLK_DET_others ", 0);

            //SrcInputType = Input_Digital2; // 120117 coding reserved
            bResault = TRUE;
        }
#endif  // end of #if( PM_SUPPORT_WAKEUP_DVI )



#if CHIP_FAMILY_TYPE == CHIP_FAMILY_TSUMU
#if( PM_SUPPORT_WAKEUP_DP &&ENABLE_DP_INPUT)
        else if( (g_bDPAUXVALID && sPMInfo.sPMConfig.bDP_enable ))//||(!(_msRegs[REG_1FA5]&_BIT3)))
        {
            ucWakeupStatus = ePMSTS_DP_ACT;
            SrcInputType = Input_Displayport;
            bResault = TRUE;
        }
        if((!g_bDPAUXVALID) && sPMInfo.sPMConfig.bDP_enable && (_msReadByte( REG_1FE0)&BIT4))
        {
            _msWriteByteMask(REG_1FE0,0,BIT4);
        }
#endif
#endif

#if CHIP_FAMILY_TYPE == CHIP_FAMILY_TSUMC || CHIP_FAMILY_TYPE == CHIP_FAMILY_MST9U|| CHIP_FAMILY_TYPE == CHIP_FAMILY_TSUMJ
#if ENABLE_MHL
        if(sPMInfo.ucPMMode != ePM_POWER_DCOFF)
        {
            if((u8ActivePort = msPM_GetActiveMHLPort(TRUE)) != 0xFF)
            {
                u8WakeupInputSrc = u8ActivePort;
                ucWakeupStatus = ePMSTS_MHL_ACT;
                bResault = TRUE;
            }
        }
        else
        {
            msPM_GetActiveMHLPort(FALSE);
        }
#endif
#endif
#if(IR_REMOTE_SEL != NO_REMOTE)
    #if (IR_MODE_SEL == IR_SOFTWARE_MODE)
        irPollingHandler();
        irDecodeCommand(); // ir decode command
        irDecodeNumKey(); // ir decode number key

        if(g_ucIrCode == IRKEY_POWER)
        {
            ucWakeupStatus =ePMSTS_POWERGPIO_ACT;
            bResault = TRUE;
        }
    #endif
#endif
    }
    if(sPMInfo.ucPMMode == ePM_POWER_DCOFF)
    {
        if(g_u8KeypadPressData == (KEY_POWER|KEY_PLUS|KEY_MINUS))
        {
            ucWakeupStatus = ePMSTS_FACTORY_ACT;
            bResault = TRUE;
        }
    }
    if(bForceWakeup )
    {
        ucWakeupStatus = ePMSTS_GPIO_ACT;
        bResault = TRUE;
    }
    return  bResault;
}

//**************************************************************************
//  [Function Name]:
//                  msPM_SetPMClockLive()
//  [Description]
//                  msPM_SetPMClockLive
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void msPM_SetPMClockLive(BYTE clk_sel)
{
    if (clk_sel == CLK_LIVE_RCOSC_4M)
    {
        msWriteByteMask(REG_0003BC, BIT6, BIT7|BIT6); // RCOSC_4M
#if( ENABLE_WATCH_DOG )
        SetWDTClk(CLK_LIVE_RCOSC_4M);
#endif
    }
    else
    {
#if( ENABLE_WATCH_DOG )
        SetWDTClk(CLK_LIVE_XTAL);
#endif
        msWriteByteMask(REG_0003BC, 0x00, BIT7|BIT6); // XTAL
    }
}
//**************************************************************************
//  [Function Name]:
//                  msPM_SetPMClock()
//  [Description]
//                  msPM_SetPMClock
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void msPM_SetPMClock(BYTE clk_sel)
{
    if (clk_sel == RCOSC)
    {
        PM_printMsg(" \r\n***PM Clock =>RCOSC ");


#if (CHIP_FAMILY_TYPE == CHIP_FAMILY_TSUMC||CHIP_FAMILY_TYPE == CHIP_FAMILY_MST9U|| CHIP_FAMILY_TYPE == CHIP_FAMILY_TSUMJ)
        mcuSetSystemSpeed(SPEED_12MHZ_MODE);
#else
        mcuSetSystemSpeed(SPEED_4MHZ_MODE);
#endif

        msPM_SetPMClockLive(CLK_LIVE_RCOSC_4M); // mcu clk Live select ROSC

        msWriteByteMask(REG_000390, BIT0, _BIT0|_BIT1); //select PWM clk DFT_CLK
        msWriteByteMask(REG_000390, 0, _BIT2|_BIT3); //select SAR clk DFT_CLK
        msWriteByteMask(REG_000390, _BIT4, _BIT4|_BIT5);  //select DDC clk 12M
        msWriteByteMask(REG_000390, _BIT6, _BIT6|_BIT7); //select IR clk 12M

        g_bMcuPMClock = 1;
    }
    else
    {

        PM_printMsg(" \r\n***PM Clock =>XTAL ");
#if 1
        mcuSetSystemSpeed(SPEED_XTAL_MODE);
#else
        mcuSetSpiSpeed( IDX_SPI_CLK_XTAL );
        mcuSetMcuSpeed(IDX_MCU_CLK_XTAL) ;              // see function body for detail description
#endif

        msWriteByteMask(REG_000390, 0x00, _BIT0|_BIT1); //select PWM clk XTAL
        msWriteByteMask(REG_000390, 0x00, _BIT2|_BIT3); //select SAR clk XTAL
        msWriteByteMask(REG_000390, 0x00, _BIT4|_BIT5);  //select DDC clk XTAL
        msWriteByteMask(REG_000390, 0x00, _BIT6|_BIT7); //select IR clk XTAL

        msPM_SetPMClockLive(CLK_LIVE_XTAL); // mcu clk Live select XTAL
        g_bMcuPMClock = 0;
    }
    //Delay1ms(2);
    //msWriteByteMask(REG_101ED1,_BIT4|_BIT0 , _BIT4|_BIT0);    // power down LPLL and MPLL

    #if  PM_XTAL_AAC
           msWriteByteMask(REG_0003B8, 0, _BIT7); //disable turbo, select aac .
           msWriteByteMask(REG_0003B8, 0, _BIT1|_BIT0); //select weak swing
    #endif
    #if FPGA_PM
    _msWriteByteMask(SC0_F0, 0, BIT1|BIT0);
    _msWriteByte(SC0_F1, 0x01);
    _msWriteByte(SC0_F1, 0x00);
    _msWriteByteMask(SC0_F0, BIT4|BIT1|BIT0, BIT4|BIT1|BIT0);
    #endif

}


//**************************************************************************
//  [Function Name]:
//                  msPM_ClearStatus()
//  [Description]
//                  msPM_ClearStatus
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void msPM_ClearStatus(Bool bResetPM)
{
    //msWriteBit(REG_000381, FALSE   , BIT7);
     //*********************************************************
    //Software reset PM //Sky110719                       _                             //
    ////                                                                 | |                           //
    //Some wake up event just report a pulse    ____| |___,                    //
    //if wake up event keep happen after Wake up status Clear,              //
    //wake up report register will not update status.                               //
    //software reset can solve it                                                           //
    //*********************************************************

    if(bResetPM)
        msWriteBit(REG_000383, TRUE, BIT1);  //Software Reset for PM
    msWriteBit(REG_000383, FALSE, BIT1);

    msWriteBit(REG_00038E, TRUE, BIT4);     //Clear the DVI clock detection status
    msWriteBit(REG_00038E, FALSE, BIT4);

    msWriteBit(REG_000381, TRUE, BIT7);

}

//**************************************************************************
//  [Function Name]:
//                  msPM_EnableDPDetect(BOOL bEnable)
//  [Description]
//                  Enable DP detect
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************

void msPM_EnableDPDetect(BOOL bEnable)//if not enable can reduce 10mA
{
     BYTE i;
     BYTE j=0;

    for(i =0; i< Input_Nums; i++)
    {
        if(INPUT_IS_DISPLAYPORT(i) && !bPMPortSkipIndex[i])
                break;
    }



#if (CHIP_FAMILY_TYPE == CHIP_FAMILY_TSUMC || CHIP_FAMILY_TYPE == CHIP_FAMILY_MST9U)

	msWriteByteMask(REG_002B29,BIT5,BIT5);
       msWriteByte(REG_0011C2,0x0C);
       msWriteByte(REG_0011C3,0x78);
	msWriteByte(REG_00117D,0x00);
	msWriteByte(REG_001191,0xF0);
	msWriteByte(REG_00127D,0x00);
	msWriteByte(REG_001291,0xF0);
       for(j =0;j<=6;j++)
	{
		msWriteByte(REG_0011E0+j,0x00);
		msWriteByte(REG_0012E0+j,0x00);
		msWriteByte(REG_0013E0+j,0x00);
		msWriteByte(REG_0014E0+j,0x00);
		msWriteByte(REG_0015E0+j,0x00);
		msWriteByte(REG_0016E0+j,0x00);
	}
	#if 0
	msWriteByte(REG_0011E3,0x00);
	msWriteByte(REG_0012E3,0x00);
	msWriteByte(REG_0013E3,0x00);
	msWriteByte(REG_0014E3,0x00);
	msWriteByte(REG_0015E3,0x00);
	msWriteByte(REG_0016E3,0x00);
 	#endif
     

	msWrite2Byte(REG_0017A8,0x4444);
	msWriteByte(REG_0017AA,0x44);

    if(bEnable && (i<Input_Nums))
    {
        // _msWriteByteMask(REG_06AE,(bEnable)?0x07:0x00,0x07);
        // _msWriteByteMask(REG_07AE,(bEnable)?0x07:0x00,0x07);
    }
    else
    {

            msWriteByteMask(REG_0017A6, 0x00, 0x3F); //disable all sqh from no-die
            //msWrite2ByteMask(REG_0011A8+COMBO_OFFSET(MUX_COMBO0), _BIT13|_BIT1 , _BIT13|_BIT1); //
            //msWrite2ByteMask(REG_0011A8+COMBO_OFFSET(MUX_COMBO1), _BIT13|_BIT1, _BIT13|_BIT1); //
            //msWrite2ByteMask(REG_0011A8+COMBO_OFFSET(MUX_COMBO2), _BIT13|_BIT1, _BIT13|_BIT1); //
            //msWrite2ByteMask(REG_0011A8+COMBO_OFFSET(MUX_COMBO3), _BIT13|_BIT1, _BIT13|_BIT1); //
            //msWrite2ByteMask(REG_0011A8+COMBO_OFFSET(MUX_COMBO4), _BIT13|_BIT1, _BIT13|_BIT1); //
            //msWrite2ByteMask(REG_0011A8+COMBO_OFFSET(MUX_COMBO5), _BIT13|_BIT1, _BIT13|_BIT1); //

    }
#else
    if(bEnable)
    {
         _msWriteByteMask(REG_356D, 0, BIT1);//Disable Power down CDR center bias gen
    }
    else
    {
         _msWriteByteMask(REG_356D, BIT1, BIT1);//Power down CDR center bias gen
    }
#endif
}


//**************************************************************************
//  [Function Name]:
//                  msPM_SetPMMode()
//  [Description]
//                  msPM_SetPMMode
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void msPM_SetPMMode(void)
{


    #if !PM_SUPPORT_ADC_TIME_SHARE && !PM_SUPPORT_DVI_TIME_SHARE && !PM_SUPPORT_SOG_TIME_SHARE
        msPM_EnableDVIClockAmp(FALSE);
    #endif

    if (sPMInfo.sPMConfig.bHVSync_enable)
    {
        msPM_EnableHVSyncDetect(TRUE);
    }
    else
    {
        msPM_EnableHVSyncDetect(FALSE);
    }

    if (sPMInfo.sPMConfig.bSOG_enable)
    {
        msPM_EnableSOGDetect(TRUE);
    }
    else
    {
        msPM_EnableSOGDetect(FALSE);
    }

    if (sPMInfo.sPMConfig.bDP_enable)
        msPM_EnableDPDetect(TRUE);
    else
        msPM_EnableDPDetect(FALSE);

#if( PM_SUPPORT_WAKEUP_DVI )
    if (sPMInfo.sPMConfig.bDVI_enable)
    {
        msPM_EnableDVIDetect(TRUE);
    }
    else
    {
        msPM_EnableDVIDetect(FALSE);
    }
#endif  // end of #if( PM_SUPPORT_WAKEUP_DVI )

#if( PM_SUPPORT_WAKEUP_CEC == 1)
		if (sPMInfo.sPMConfig.bCEC_enable)
		{
			msPM_EnableCECDetect(TRUE);
		}
		else
		{
			msPM_EnableCECDetect(FALSE);
		}
#endif


    if (sPMInfo.ucPMMode == ePM_POWER_DCOFF)
        while (((Key_GetKeypadStatus()^KEYPAD_MASK)&KEYPAD_MASK)==KEY_POWER); //Wait power key released,avoid power off and the power on again

    if (sPMInfo.sPMConfig.bGPIO_enable)
    {
        msPM_EnableGPIODetect(TRUE);
    }
    else
    {
        msPM_EnableGPIODetect(FALSE);
    }

    if (sPMInfo.sPMConfig.bSAR_enable)
    {
        msPM_EnableSARDetect(TRUE);
    }
    else
    {
        msPM_EnableSARDetect(FALSE);
    }

    if (sPMInfo.sPMConfig.bMCCS_enable)
    {
        msPM_EnableMCCSDetect(TRUE);
    }
    else
    {
        msPM_EnableMCCSDetect(FALSE);
    }

    if(sPMInfo.sPMConfig.bIR_enable)
    {
        msPM_EnableIRDetect(TRUE);
    }
    else
    {
        msPM_EnableIRDetect(FALSE);
    }


    msPM_MCCSReset();

#if( PM_SUPPORT_AC2DC )
    if (sPMInfo.sPMConfig.bACtoDC_enable)
    {
        msPM_EnableAC2DC(TRUE);
    }
    else
    {
        msPM_EnableAC2DC(FALSE);
    }
#endif  // end of #if( PM_SUPPORT_AC2DC )

    if (sPMInfo.sPMConfig.bEDID_enable)
    {
        msPM_Enable_EDID_READ(TRUE);
    }
    else
    {
        msPM_Enable_EDID_READ(FALSE);
    }

    if (sPMInfo.ucPMMode == ePM_POWER_ON)
        return;
    //------------------------------------------------------

#if CHIP_FAMILY_TYPE == CHIP_FAMILY_TSUMC||CHIP_FAMILY_TYPE == CHIP_FAMILY_MST9U||CHIP_FAMILY_TYPE == CHIP_FAMILY_TSUMJ
#if SPI_SSC_EN
    mcuSetSpiSpeed(IDX_SPI_CLK_XTAL);// change to other clock source before  DDR clk pd
#endif
#endif
    msPM_PassWord(TRUE);
    msPM_PowerDownMacro();
    msPM_OutputTriState();
    msPM_ClearStatus(FALSE);
#if ENABLE_DP_INPUT
   // DPRXPMForceEnter();
#endif

#if 0//ENABLE_DP_INPUT
    DPRxClearAUXVliadStatus();
#endif

#if 0//DEBUG_EN
    msPM_UART();
#endif
   // g_bMcuPMClock = 1;
//--------------- Sacaler Reset Start--------------------------------

//--------------- Sacaler Reset End--------------------------------

    if (sPMInfo.sPMConfig.bMCUSleep)
    {
        msWriteByteMask(REG_0003A6, 0, _BIT0);
        msPM_SetPMClock(RCOSC);
        msWriteByteMask(REG_0003AA, _BIT1, _BIT1);
        msWriteByteMask(REG_0003A6, _BIT7, _BIT7|_BIT6);  // Enable HW PM mode (Enable mcu gating)

        Delay1ms(2);
#if PM_DISABLE_R2_COREPOWER
       DISABLE_R2CORE_POWER();
#endif
        //Set MCU sleep mode
        PM_printMsg("Enter HW PM Mode");
        //CACHE_DISABLE();
        msWriteByte(REG_000380, 0xA5);
        msWriteByte(REG_000380, 0x56);

    }
    else
    {
        //Set MCU no die mode
        msPM_SetPMClock(PM_CLOCK);
        if(PM_CLOCK == XTAL)
            msWriteByteMask(REG_0003A6, BIT2, _BIT2|BIT3);
        else if((PM_CLOCK == RCOSC))
#if 0//   ENABLE_DP_INPUT     //111110 modified VS230 PM mode Clock speed   //20130118 need to disable XTAL to turn off in PM mode,otherwise the current of LLPL will increase 1.5mA
        {
            _msWriteByteMask(REG_PM_A6, BIT2, _BIT2|BIT3);
        }
#else
        {
            //_msWriteByteMask(REG_PM_A6, 0, _BIT2|BIT3);
            msWriteByteMask(REG_0003A6, _BIT2, _BIT2);
#if (CHIP_ID!=MST9U) || (ENABLE_WATCH_DOG==0)
            msWriteByteMask(REG_0003A6, 0, _BIT2);
#endif
            msWriteByteMask(REG_0003A6, 0, BIT3);
        }
#endif
       // printData("\r\n PM_DISABLE_MPLL ", 0);
       // PM_DISABLE_MPLL();

        msWriteByteMask(REG_0003A6, 0, _BIT0);
        msWriteByteMask(REG_0003A6, _BIT0, _BIT0); //Software power down mode
        sPMInfo.ePMState = ePM_WAIT_EVENT;

        Delay1ms(2);
#if PM_DISABLE_R2_COREPOWER
       DISABLE_R2CORE_POWER();
#endif
        PM_printMsg("Enter SW PM Mode");
    }

#if PM_CABLE_DETECT_USE_SAR
    sPMInfo.bCABLE_SAR_VALUE=CABLE_DET_SAR;
#endif

#if ENABLE_POWER_BOARD_CONTROL
    if(!PowerOnFlag)
        Set_PowerBoardSaving_Pin();
#endif

}
void msPM_ResetChip(void)
{
    BYTE ucBackup = msReadByte(REG_0003A6);
    // Only work in <PM mode>
    msWriteByteMask(REG_0003A6, _BIT0, _BIT0); // + password on

    msWriteByteMask(REG_0003A6, BIT4, BIT4);
    Delay1ms(2);
    msWriteByteMask(REG_0003A6, 0, BIT4);

    // No function in MST9U
    //msWriteByteMask(REG_000383, BIT0, BIT0);
    //msWriteByteMask(REG_000383, 0, BIT0);

    msWriteByteMask(REG_0003A6, ucBackup&_BIT0, _BIT0);
}

void msPM_SetStatus2Spare(void)
{
      if(sPMInfo.ucPMMode == ePM_POWER_DPMS)
           msWriteByteMask(REG_0003AA, BIT2, BIT2|BIT3);
      else if(sPMInfo.ucPMMode == ePM_POWER_DCOFF)
           msWriteByteMask(REG_0003AA, BIT2|BIT3, BIT2|BIT3);
      else
           msWriteByteMask(REG_0003AA, 0x00, BIT2|BIT3);

}
//**************************************************************************
//  [Function Name]:
//                  msPM_Reset()
//  [Description]
//                  msPM_Reset
//  [Arguments]:
//
//  [Return]:
//      PM status
//
//**************************************************************************
BYTE msPM_Reset(void)
{
    BYTE ucStatus = ePMSTS_INVAID;

    //msPM_EnableAC2DC(FALSE);

   // _msWriteByte(SC0_F1, 0);
  //  _msWriteByte(REG_1ED1, 0x4);    //MPLL function enable
   // _msWriteByte(SC0_F0, 0);

   //  _msWriteByteMask(REG_PM_A6, _BIT3|_BIT2|BIT1, _BIT3|_BIT2|BIT1);       //SW XTAL on
   //  _msWriteByteMask(REG_01BC, 0, _BIT6);           //Live cLK select XTAL

   // mcuSetSystemSpeed(SPEED_NORMAL_MODE);
    //mcuSetMcuSpeed(MCU_SPEED_INDEX) ; // see function body for detail description
    //mcuSetSpiSpeed(SPI_SPEED_INDEX) ; // see function body for detail description

  //  _msWriteByteMask(REG_PM_A6, 0, _BIT7|_BIT0);  // PM SW power down mode  (MCU no die mode)
  //  _msWriteByte(SC0_06, 0x00);                  //power up idclk and odclk
   msWriteByteMask(REG_0003A6, _BIT3|_BIT2|_BIT1, _BIT3|_BIT2|_BIT1);       //SW XTAL on
   msPM_SetPMClockLive(CLK_LIVE_XTAL); //Live cLK select XTAL
   msWriteByteMask(REG_0003A6, 0, _BIT7|_BIT0); // exit PM SW power down mode  (MCU no die mode)
    msPM_PassWord(FALSE);
    msPM_Init();
    //msPM_SetPMMode();

    msPM_ClearStatus(TRUE);

    msPM_MCCSReset();
    PM_printData("Enter PM ePM_PowerON mode!",0);
    return ucStatus;
}



#if 1
void msPM_InterruptEnable(Bool benable)
{
    INT_IRQ_DISP_ENABLE(benable);
    INT_IRQ_DVI_ENABLE(benable);

}
#endif

#define RECHECK_COUNT     5

Bool IsHVSyncActive(BYTE count)
{
    Bool XDATA u8Rlt=FALSE;
    WORD XDATA u16InputValue;
    BYTE XDATA check_cnt;

    for(check_cnt=0; check_cnt<count; check_cnt++)
    {
        ForceDelay1ms(20);
        u16InputValue = 0;

#if FPGA_PM
        u16InputValue = _msRead2Byte(SC0_E4) & 0x1FFF;
        if (u16InputValue == 0x1FFF || u16InputValue < 20)
           continue;
        if(labs( (DWORD)u16InputValue-(_msRead2Byte(SC0_E4) & 0x1FFF))>4)
            continue;

        u16InputValue = _msRead2Byte(SC0_E2) & 0x7FF;
        if (u16InputValue == 0x7FF || u16InputValue < 200)
            continue;

        u8Rlt=TRUE;
#endif

        break;
    }

    return u8Rlt;
}

#define FAKE_VWIDTH   16
#if ENABLE_VGA_INPUT
Bool msPM_CheckAnalogSyncActive(void)
{
    Bool XDATA u8Rlt=TRUE;
#if ENABLE_SYNC_CHECK_AGAIN

    WORD XDATA u16Hperiod, u16Vtotal;
    //BYTE XDATA ucStatus;
    BYTE XDATA u8VSyncWidth;
    //WORD XDATA u16HSyncWidth;
    BYTE regValue;

#if PM_DISABLE_R2_COREPOWER
    ENABLE_R2CORE_POWER();
    ForceDelay1ms(30);
    PM_ENABLE_MPLL();
#endif
    drvADC_init(FALSE);
    msDrvSetOfflineIPMux(u8WakeupInputSrc, 0);

    regValue = msDrvIP1OfflineGetInputSyncType(0);

    if(u8WakeupSyncStatus == ePM_VGA_SOGSYNC)
    {
      regValue = regValue | 0x70;
      drvADC_SetADCSource(ADC_TABLE_SOURCE_SOG);
      msDrvIP1OfflineAdcSetCoast(0, 0x21, 0x08, 0x08);
      msDrvIP1OfflineAdcGlitchRemoval(0, 0);
      ADC_PLL_LOCKING_EDGE(0);
    }
    else
    {
      regValue = regValue & 0x8F;
      drvADC_SetADCSource(ADC_TABLE_SOURCE_RGB);
      msDrvIP1OfflineAdcSetCoast(0, 0x01, 0x03, 0x01);
      msDrvIP1OfflineAdcGlitchRemoval(0, 0);
      ADC_PLL_LOCKING_EDGE(1);
    }

    msDrvIP1OfflineSetInputSyncType(0, regValue);//_msWriteByte(SC0_02, regValue);


    Delay1ms(80);

    if(msDrvIP1OfflineReadSyncStatus(0)&BIT4) //detect SOG sync
    {
       u8VSyncWidth = msDrvIP1OfflineReadVSyncWidth(0);
       if((u8VSyncWidth >= FAKE_VWIDTH) || (u8VSyncWidth == 0))
       {
           u8Rlt = FALSE;
       }
    }
    u16Hperiod = msDrvIP1OfflineReadHperiod(0);
    u16Vtotal = msDrvIP1OfflineReadVtotal(0);

    if( u16Hperiod == MASK_14BIT|| u16Hperiod < 10 )
    {
       u8Rlt = FALSE;
    }
    if( u16Vtotal == MASK_13BIT || u16Vtotal < 200 )
    {

       u8Rlt = FALSE;
    }
    PM_printData("u16Hperiod = %d",u16Hperiod);
    PM_printData("u16Vtotal = %d",u16Vtotal);
#endif

     return u8Rlt;
}
#endif

Bool msPM_CheckDVISyncActive(void)
{
	WORD count = 700;
    //Bool u8Rlt=TRUE;
#if ENABLE_SYNC_CHECK_AGAIN

    WORD XDATA u16Hperiod, u16Vtotal;

#if PM_DISABLE_R2_COREPOWER
    ENABLE_R2CORE_POWER();
    ForceDelay1ms(30);
#endif

    mhal_tmds_Initial(g_InputPort[u8WakeupInputSrc].eIPMux, INPUT_IS_DVI_D(u8WakeupInputSrc));
    mapi_combo_LoadHDCPKey();
    msDrvSetOfflineIPMux(u8WakeupInputSrc, 0);
#if 0
    Delay1ms(150);

    u16Hperiod = msDrvIP1OfflineReadHperiod(0);
    u16Vtotal = msDrvIP1OfflineReadVtotal(0);

    if( u16Hperiod == MASK_14BIT || u16Hperiod < 10 )
    {
       u8Rlt = FALSE;
    }
    if( u16Vtotal == MASK_13BIT || u16Vtotal < 200 )
    {
       u8Rlt = FALSE;
    }
#endif
    do
    {
	    u16Hperiod = msDrvIP1OfflineReadHperiod(0);
	    u16Vtotal = msDrvIP1OfflineReadVtotal(0);

	    if( u16Hperiod != MASK_14BIT && u16Hperiod > 10 )
	    {
	        if( u16Vtotal != MASK_13BIT && u16Vtotal > 200 )
	        {
				//PM_printData("*****************count = %d",count);
				return TRUE;
	        }
	    }
	    ForceDelay1ms(1);
		count--;
    }while(count>0);

    PM_printData("u16Hperiod = %d",u16Hperiod);
    PM_printData("u16Vtotal = %d",u16Vtotal);
#endif

    return FALSE;

}
#if   0//  sahdow_Need test
Bool msPM_CheckDPMCCSActive(void)
{
    Bool bExitPM = TRUE;
     if(_msRegs[REG_1FE6]&_BIT7)   //MCCS Interrupt
        {
                //DDC2BI_DP() ;
            if(DDCBuffer[2]==PowerMode)
            {
                if((DDCBuffer[4] == 0x01)
                &&(sPMInfo.ucPMMode == ePM_POWEROFF || sPMInfo.ucPMMode == ePM_STANDBY))
                    ;
                else  if((DDCBuffer[4] == 0x05) && (sPMInfo.ucPMMode == ePM_STANDBY))
                {
                    msPM_Reset();
                    PowerOffSystem();
                    bExitPM=FALSE;
                }
                else  if((DDCBuffer[4] == 0x05) && (sPMInfo.ucPMMode == ePM_POWEROFF))
                {

                }
                else
                    bExitPM=FALSE;
            }

        }

 return bExitPM;


}
#endif
Bool msPM_CheckMCCSActive(void)
{
    Bool bExitPM = TRUE;
    if( (ucWakeupStatus == ePMSTS_MCCS01_ACT)
      &&(sPMInfo.ucPMMode == ePM_POWER_DCOFF || sPMInfo.ucPMMode == ePM_POWER_DPMS))
    {
    	bExitPM = TRUE;
    }
    else if((ucWakeupStatus == ePMSTS_MCCS05_ACT) && (sPMInfo.ucPMMode == ePM_POWER_DPMS))
    {
        msPM_SetFlagDCoff();
        msPM_Reset();
        bExitPM=FALSE;
    }
    else  if((ucWakeupStatus == ePMSTS_MCCS05_ACT) && (sPMInfo.ucPMMode == ePM_POWER_DCOFF))
    {
    }
    else
    {
        bExitPM=FALSE;
    }

    return bExitPM;
}
Bool msPM_CheckPowerKeyActive(void)
{
    Bool bExitPM = FALSE;

#if  PM_POWERkEY_GETVALUE
    {
            if(sPMInfo.ucPMMode == ePM_POWER_DPMS)
            {
                msPM_Reset();
                msPM_SetFlagDCoff();

				#if ENABLE_DAISY_CHAIN
				{
					extern void msPM_DCCmdInit();
					extern BOOL msPM_DCSetCmdDcOff(BYTE ,BYTE *);
					BYTE u8Retry=10, u8SendData=0, u8ReplyData=0;
					
					msPM_DCCmdInit();
					while(!(msPM_DCSetCmdDcOff(u8SendData, &u8ReplyData)) && --u8Retry)
	        		{
	            		PM_printData("[PM] Daisy Chain DC OFF Retry %d\n",u8Retry);
	        		}
				}
				#endif
				
                #if FPGA_PM
                PowerOffSystem();
                #endif
            }
            else if(sPMInfo.ucPMMode == ePM_POWER_DCOFF)
            {
                bExitPM = TRUE;
            }
        }
#else

    WORD ucStatus_GPIO;
    ucStatus_GPIO = msRead2Byte( REG_PMGPIO_STS );

    if ( ucStatus_GPIO & (PM_POWERKEY_INT)  || bForceWakeup)//power key
    {
        if(sPMInfo.ucPMMode == ePM_POWER_DPMS)
        {
            //msPM_Reset();
            msPM_SetFlagDCoff();
            #if FPGA_PM
            PowerOffSystem();
            #endif
        }
        else if(sPMInfo.ucPMMode == ePM_POWER_DCOFF)
            bExitPM = TRUE;
    }
#endif
    return bExitPM;
}


BOOL msPM_GetDPSquelch(BYTE u8ComboPort, WORD u16Times)
{
        Bool bResult = FALSE;
   while( u16Times-- )
   {
        bResult = FALSE;
        switch(u8ComboPort)
        {
            case MUX_COMBO0:
                     if(msReadByte(REG_0017B0)&_BIT0)
                         bResult = TRUE;
                     break;
            case MUX_COMBO1:
                     if(msReadByte(REG_0017B0)&_BIT1)
                         bResult = TRUE;
                     break;
            case MUX_COMBO2:
                     if(msReadByte(REG_0017B0)&_BIT2)
                         bResult = TRUE;
                     break;
            case MUX_COMBO3:
                     if(msReadByte(REG_0017B0)&_BIT3)
                         bResult = TRUE;
                     break;
            case MUX_COMBO4:
                     if(msReadByte(REG_0017B0)&_BIT4)
                         bResult = TRUE;
                     break;
            case MUX_COMBO5:
                     if(msReadByte(REG_0017B0)&_BIT5)
                         bResult = TRUE;
                     break;
            default:
                    break;
        }
                     if(!bResult)
                        break;
    }

             return bResult;

}

BOOL msPM_Checkagain(void)
{
    Bool bExitPM = TRUE;
   //volatile WORD ucStatus_GPIO;

    msWriteByteMask(REG_0003A6, _BIT3|_BIT2|BIT1, (_BIT3|_BIT2|BIT1));       //SW XTAL on
    Delay1ms(10);
    msPM_SetPMClockLive(CLK_LIVE_XTAL); //Live cLK select XTAL
    msWriteByteMask(REG_0003A6, 0, (_BIT7|_BIT0));  // PM SW power down mode  (MCU no die mode)
    msWriteByteMask(REG_0003BC, 0, _BIT0);           //Live cLK select XTAL
    msPM_SetPMClock(XTAL); // change refer clock to XTAL
 #if ENABLE_VGA_INPUT   
    if(ucWakeupStatus == ePMSTS_VGA_ACT)
    {
        //msWriteByteMask(REG_101ED1, 0, BIT0);
        if(msPM_CheckAnalogSyncActive())
            bExitPM = TRUE;//wakeup
        else
            bExitPM = FALSE;
        //msWriteByteMask(REG_101ED1, BIT0, BIT0);
    }
 #else
 if(0);
 #endif
#if( PM_SUPPORT_WAKEUP_DVI )
    else if(ucWakeupStatus == ePMSTS_DVI_ACT)
    {

        if(msPM_CheckDVISyncActive())
            bExitPM = TRUE;//wakeup
        else
            bExitPM = FALSE;
    }
#endif
    else if(ucWakeupStatus == ePMSTS_GPIO_ACT)
    {
#if !(PM_POWERkEY_GETVALUE)
    if(msPM_CheckPowerKeyActive())
        {
        bExitPM = TRUE;//wakeup
        }
        else
#endif
#if     PM_CABLEDETECT_USE_GPIO&&ENABLE_DP_INPUT
        if (!(DP_CABLE_NODET))
        {
              SrcInputType=Input_Displayport;
               mStar_SetupInputPort();
            bExitPM = TRUE;
        }
    else
#endif

#if ENABLE_DAISY_CHAIN
        bExitPM = TRUE;
#else
        bExitPM = FALSE;
#endif

    }
    else if(ucWakeupStatus == ePMSTS_SAR_ACT)
    {

        bExitPM = TRUE;
    }
    else if(ucWakeupStatus >= ePMSTS_MCCS04_ACT && ucWakeupStatus <= ePMSTS_MCCS01_ACT)//20100419
    {
       if(msPM_CheckMCCSActive())
            bExitPM = TRUE;//wakeup
        else
            bExitPM = FALSE;
    }
#if ENABLE_DP_INPUT&&PM_SUPPORT_WAKEUP_DP
    else if(ucWakeupStatus == ePMSTS_DP_ACT)
   {
     //mStar_SetupInputPort();   // For HDCP wake up issue. Aksv not ready in Rx
    #if  0 //  sahdow_Need test
    if(msPM_CheckDPMCCSActive())
        bExitPM = TRUE;
    else
        bExitPM = FALSE;
    #else
    bExitPM = TRUE;
    #endif
   }
#endif
#if (PM_POWERkEY_GETVALUE)
    else if(ucWakeupStatus == ePMSTS_POWERGPIO_ACT)
   {
        if(msPM_CheckPowerKeyActive())
        {
            bExitPM = TRUE;//wakeup
            KeypadButton=BTN_REPEAT;      //110914 Rick modified
        }
        else
            bExitPM = FALSE;
   }

#endif
#if (PM_CABLE_DETECT_USE_SAR)
    else if(ucWakeupStatus ==ePMSTS_CABLESAR_ACT)
    {
        if(CABLE_DET_SAR<sPMInfo.bCABLE_SAR_VALUE)
            bExitPM = TRUE;
        else
            bExitPM = FALSE;
    }
#endif
#if ENABLE_MHL
    else if(ucWakeupStatus == ePMSTS_MHL_ACT)
    {
        bExitPM = TRUE;
        PM_printData("ePMSTS_MHL_ACT:%d", ePMSTS_MHL_ACT);
    }
#endif
#if (PM_SUPPORT_WAKEUP_CEC == 1)
	else if (ucWakeupStatus == ePMSTS_CEC_ACT)
	{
		bExitPM = TRUE;
		PM_printData("ePMSTS_CEC_ACT:%d", ePMSTS_CEC_ACT);
	}
#endif
	else if (ucWakeupStatus == ePMSTS_FACTORY_ACT)
	{
		bExitPM = TRUE;
		PM_printData("ePMSTS_FACTORY_ACT:%d", ePMSTS_FACTORY_ACT);
	}
    else
        bExitPM = FALSE;

        //    bExitPM = TRUE;//wakeup

    return bExitPM;
}

void msPM_ScanADCCh(void)
{
    BYTE channel;

    if( !ModeDetectCounter )
    {
        channel = msReadByte(REG_0003FF)&0x03;
        channel = (++channel) >= 3 ? 0 : channel;
        msWriteByteMask(REG_0003FF,channel,0x03);
        ModeDetectCounter = 120;
    }
}

//**************************************************************************
//  [Function Name]:
//                  msPM_WaitingEvent()
//  [Description]
//                  msPM_WaitingEvent
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void msPM_WaitingEvent(void)
{
    //msPM_ClearStatus(TRUE); // 120209 coding reserved. PM sw mode, after disable xtal, it should not reset PM
    PM_printMsg("msPM_WaitingEvent.....");

    while(1)
    {
        Main_SlowTimerHandler();
#if( ENABLE_WATCH_DOG )
        ClearWDT();
#endif

#if ENABLE_DC_SDM_FORWARD
		msPM_DCSdmForward();
#endif

#if 0
#ifdef UseVGACableReadWriteAllPortsEDID
        //if ((!g_bServiceEDIDUnLock && !WriteDeviceFlag) && (!ProductModeFlag))         //110916 Rick add
#endif
        {
            if( SyncLossState() && IsCableNotConnected() )
            {
                hw_ClrDDC_WP();
            }
            else
            {
                hw_SetDDC_WP();
            }
        }
#endif

#if UART1
        UART1_Handler();
#endif
#if ENABLE_DEBUG
        DebugHandler();
        if ( DebugOnlyFlag )
            continue;
#endif

         #if PM_SUPPORT_ADC_TIME_SHARE

         #else
          msPM_ScanADCCh();
         #endif
         #if 0
         if(u8WakeuCounter)
         {        u8WakeuCounter--;
                if(!u8WakeuCounter)
                   bForceWakeup = TRUE;
         }
         #endif
         if(msPM_GetPMStatus())
            break;
        #if 0//!DEBUG_EN
        if(DDCCI_CheckDMPSON())
            break;
        #endif

    }
	#if ENABLE_DC_SDM_FORWARD
	msPM_DCSdmForward();
	#endif
    PM_printData("\r\nPM Wakeup Event1 (%d) !", ucWakeupStatus);
}


//**************************************************************************
//  [Function Name]:
//                  msPM_Handler()
//  [Description]
//                  msPM_Handler
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void msPM_Handler(void)
{
    static ePM_State ucPrevStatus = ePM_EXIT_PM;

    // print state for debug
    if(sPMInfo.ePMState != ucPrevStatus)
    {
        ucPrevStatus = sPMInfo.ePMState;
        PM_printData("[msPM_Handler] PM state: %d ", sPMInfo.ePMState);
    }

    switch(sPMInfo.ePMState)
    {
        case ePM_ENTER_PM:
#if ENABLE_DRAM_SELFREFRESH
            DeInit_Dram();
#endif
            msDrvMcu51ResetR2();
            msPM_SetPMMode();
            PM_printMsg("ePM_ENTER_PM, 51 Reset R2.");
            sPMInfo.ePMState = ePM_WAIT_EVENT;
#if(IR_REMOTE_SEL != NO_REMOTE)
            irInitialize(); // initialize IR
#endif
#if ENABLE_DC_SDM_FORWARD
            msPM_DCCmdInit();
            g_SDMCommandFlagForce = 1;
#endif
            break;

        case ePM_WAIT_EVENT:
            msPM_WaitingEvent();
            if(msPM_Checkagain())
            {
                PM_printData("\r\n ucWakeupStatus = %x", ucWakeupStatus);
                sPMInfo.ePMState = ePM_EXIT_PM;
               // if(sPMInfo.ePMState == ePM_EXIT_PM) while(1);

            }
            else
            {
                PM_printData("\r\n ePM_ENTER_PM = %x", ePM_ENTER_PM);
                sPMInfo.ePMState = ePM_ENTER_PM;
            }
            break;

        case ePM_EXIT_PM:
            sPMInfo.ePMState = ePM_IDLE;
            PM_printMsg("PMMode:ePM_POWER_ON ==> Exit PM");
#if ENABLE_DAISY_CHAIN
            msWriteBit(REG_000405, TRUE, BIT6); // restore hw i2c
#endif
#if PM_DISABLE_R2_COREPOWER
            ENABLE_R2CORE_POWER();
            ForceDelay1ms(30);
#endif
            msPM_PowerUpMacro();
            msPM_SetStatus2Spare();
#if WAKEUP_RESET_CHIP
            msPM_ResetChip(); // Should be called before exit PM.
#endif
            msPM_Reset();

            g_sMailBox51.u16PMWakeupStatus = (WORD)ucWakeupStatus;
            g_sMailBox51.u8PMActivePort= u8WakeupInputSrc;
            msDrvMcuMailBoxWrite();
            msDrvMcu51ToR2();
            sPMInfo.ucPMMode = ePM_POWER_ON;
            g_sMailBoxR2.u16PMMode = ePM_POWER_ON;

            mcuSetSystemSpeed(SPEED_NORMAL_MODE);
            break;

        case ePM_IDLE:
            if(g_sMailBoxR2.u16PMMode == (WORD)ePM_POWER_DPMS)
            {
                PM_printData("\r\n -----------------u16PMPortSkipIndex = %d", g_sMailBoxR2.u16PMPortSkipIndex);
                msPM_SetPortSkipIndex(g_sMailBoxR2.u16PMPortSkipIndex);
                msPM_SetFlagDPMS();
                mdrv_mhl_SetPowerStatus(MHL_POWER_STATUS_SAVING);
            }
            else if(g_sMailBoxR2.u16PMMode == (WORD)ePM_POWER_DCOFF)
            {
                msPM_SetFlagDCoff();
                mdrv_mhl_SetPowerStatus(MHL_POWER_STATUS_DOWN);
            }
            else
            {
                msDrvMcuMailBoxRead(); // polling R2 PM status
            }
            break;

        default:
            break;
    }

}
void PMDummy(void)
{
    BYTE temp;
    msPM_EnableDVIClockAmp(0);
    msPM_Checkagain();
    msPM_WaitingEvent();
    temp = IsHVSyncActive(temp);

    msPM_PowerUpMacro();
    msPM_Reset();
    msDrvMcuMailBoxWrite();
    msDrvMcu51ToR2();
    msPM_SetPMMode();
}




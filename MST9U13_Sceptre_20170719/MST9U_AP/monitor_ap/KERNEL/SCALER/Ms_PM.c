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
#include "Global.h"
#include "Ms_PM.h"
#include "appSystem.h"
#include "msAPI_Timer.h"
#if ENABLE_DAISY_CHAIN
#include "msDaisyChain.h"
#endif
//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define MSPM_DEBUG    1
#if ENABLE_MSTV_UART_DEBUG && MSPM_DEBUG
#define MSPM_PRINT(format, ...)     printf(format, ##__VA_ARGS__)
#else
#define MSPM_PRINT(format, ...)
#endif
void SetLedFunc(BYTE LedNumber, BYTE LedColor,BYTE LedStrength,BYTE LedColorMode);

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
ePM_Mode    g_ePMMode = ePM_POWER_ON;
//-------------------------------------------------------------------------------------------------
//  External Variables
//-------------------------------------------------------------------------------------------------

// Daisy Chain PM Setup
#if ENABLE_DAISY_CHAIN
void msPM_DCSetup(void)
{
    BYTE u8Retry=10, u8SendData=0, u8ReplyData=0;

    if(g_ePMMode == ePM_POWER_DCOFF)
    {
        while(!(msDCSetCmdDcOff(u8SendData, &u8ReplyData)) && --u8Retry)
        {
            MSPM_PRINT("[PM] DC Setup Retry %d\n",u8Retry);
        }
    }
    else
    {
        while(!(msDCSetCmdDPMS(u8SendData, &u8ReplyData)) && --u8Retry)
        {
            MSPM_PRINT("[PM] DC Setup Retry %d\n",u8Retry);
        }
    }

    if( (u8Retry==0) || (u8ReplyData&BIT0)==0x00 )
    {
        MSPM_PRINT("[PM] DC Setup NG\n");
    }
    else
    {
        MSPM_PRINT("[PM] DC Setup OK\n");
    }
}
#endif

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
void  msPM_Handler(void)
{
    if( (g_ePMMode == ePM_POWER_DCOFF) || (g_ePMMode == ePM_POWER_DPMS) )
    {
        MSPM_PRINT("[PM] Enter PM mode:%d\n",g_ePMMode);
        g_sMailBoxR2.u16PMMode = (WORD)g_ePMMode;

        if(g_ePMMode == ePM_POWER_DCOFF)
        {
            msAPI_combo_IPPowerDown();
        }
        else
        {
            msAPI_combo_IPPowerSaving();
        }

        #if ENABLE_DAISY_CHAIN
        msPM_DCSetup();
        #endif
#if (defined(FengYuan_Project))
       msAPISrcInitTmdsHPD(TRUE); 
#else
     if(POWER_ON_FLAG)
        msAPISrcInitTmdsHPD(TRUE); // Set TMDS HPD to High Before Entering PM
     else
        msAPISrcInitTmdsHPD(FALSE); 
#endif		

        msDrvMcuR2To51();
    }
}

//**************************************************************************
//  [Function Name]:
//                  msPM_StartRCOSCCal()
//  [Description]
//                  msPM_StartRCOSCCal
//  [Arguments]:
//
//  [Return]:
//  RCOSC = XTAL * Counter / 513 => Counter = RCOSC *513/XTAL = 12M*513/12M =>0x201
//  Leona has high resolution calibration function, so the formula will be changed as below
//  RCOSC = XTAL * Counter / 2049 => Counter = RCOSC *2049/XTAL = 12M*513/12M =>0x801
//  CHIP_FAMILY_MST9U uses FRO_12M to calibration ==> Counter=0x201
//**************************************************************************
Bool msPM_StartRCOSCCal(void)
{
//#define RCOSC_CAL_RANGE         64
#define WRITE_CAL_VALUE(A, B)    (msWriteByteMask(REG_000382, B,0x07));\
                                                (msWriteByteMask(REG_00038A, A,0x3F));

#if (CHIP_ID == MST9U3)
#define RCOSC_HIGH_RESOLUTION_EN  BIT3
#define RCOSC_COUNTER_MASK  0xFFF
#else
#define RCOSC_HIGH_RESOLUTION_EN  0
#define RCOSC_COUNTER_MASK  0x3FF
#endif
    WORD ucCounter;
    BYTE ucTemp,i;

    msWriteByte(REG_0003A6, 0x0E);  //reg_sw_pm_mode  //reg_sw_power_fence  //reg_sw_xtal_on
    msWriteByteMask(REG_0003BC, 0, _BIT6);  //Clkgen source select for "clk_live" gen  ([6] 0: select XTAL as clock source)
    ucTemp = msReadByte(REG_000382)&0x07;
    WRITE_CAL_VALUE(0x00,ucTemp);
    msWriteByte(REG_103A80, 0x80|RCOSC_HIGH_RESOLUTION_EN);//osc soft reset
    msWriteByte(REG_103A80, 0x03|RCOSC_HIGH_RESOLUTION_EN);//RCOSC calculate & counter one time mode enable

    while( !( msReadByte(REG_103A83) & _BIT4 ) ); // one time counter flag
    ucCounter = msRead2Byte(REG_103A82)&RCOSC_COUNTER_MASK; // one time counter report

    if( ucCounter < RCOSC_TARGET )
    {
        i = 0;
        do
        {
            WRITE_CAL_VALUE(++i,ucTemp);
            msWriteByte(REG_103A80, 0x80|RCOSC_HIGH_RESOLUTION_EN);//osc soft reset
            msWriteByte(REG_103A80, 0x03|RCOSC_HIGH_RESOLUTION_EN);//RCOSC calculate & counter one time mode enable
            while( !( msReadByte(REG_103A83) & _BIT4 ) );

            ucCounter = msRead2Byte(REG_103A82)&RCOSC_COUNTER_MASK;
        } while( (ucCounter < RCOSC_TARGET) && (i < 0x1F) );

        return (BOOL)(ucCounter >= RCOSC_TARGET);
    }
    else if( ucCounter > RCOSC_TARGET )
    {
        i = 0x40;
        do
        {
            WRITE_CAL_VALUE(--i,ucTemp);
            msWriteByte(REG_103A80, 0x80|RCOSC_HIGH_RESOLUTION_EN);//osc soft reset
            msWriteByte(REG_103A80, 0x03|RCOSC_HIGH_RESOLUTION_EN);//RCOSC calculate & counter one time mode enable
            while( !( msReadByte(REG_103A83) & _BIT4 ) );
            ucCounter = msRead2Byte(REG_103A82)&RCOSC_COUNTER_MASK;

        } while ( (ucCounter > RCOSC_TARGET) && (i > 0x20) );


        return (BOOL)(ucCounter <= RCOSC_TARGET);
    }

    return TRUE;

//#undef RCOSC_CAL_RANGE
#undef WRITE_CAL_VALUE
#undef RCOSC_HIGH_RESOLUTION_EN
#undef RCOSC_COUNTER_MASK
}

//**************************************************************************
//  [Function Name]:
//                  msAPI_PMSetMode()
//  [Description]
//                  API set pm mode
//  [Arguments]:
//                  ePM_Mode
//  [Return]:
//
//**************************************************************************
void msAPI_PMSetMode( ePM_Mode ePMMode )
{
    g_ePMMode = ePMMode;

#ifdef PanelI2CCommandSetting
   if(ePMMode==ePM_POWER_ON)
   {
         if(!bPanelOnFlag)
         {         
            hw_SetPanel();
	     Clr_VBY1_Check_LockN_Flag();		
	     printMsg("000000000000000000000000");
         }
   }
#endif

#if !Enable_AutoInput_Menu	
    if(g_bAPPWindow_Init&&CB_FuncList[eCB_CustomerSetPMModeFlag]!=NULL)
	((fpCustomerSetPMModeFlag)CB_FuncList[eCB_CustomerSetPMModeFlag])(ePMMode);
#endif

}

//**************************************************************************
//  [Function Name]:
//                  msAPI_PMGetWakeupStatus()
//  [Description]
//                  API get wakeup status
//  [Arguments]:
//
//  [Return]:
//                  ePM_WakeupStatus
//**************************************************************************
ePM_WakeupStatus msAPI_PMGetWakeupStatus( void )
{
    return (ePM_WakeupStatus)g_sMailBox51.u16PMWakeupStatus;
}
WORD msAPI_PMGetWakeupActivePort( void )
{
    return g_sMailBox51.u8PMActivePort;
}


#include "types.h"
#include "board.h"
#include "global.h"
#include "mcu.h"
#include "ms_reg.h"
#include "debug.h"
#include "Common.h"
#include "ms_rwreg.h"
#include "misc.h"
#include "gpio_def.h"
#include "Keypaddef.h"
#include "Keypad.h"
#include "msAPI_MailBox.h"
#include "Buzzer.h"
#include "mhal_tmds.h"
#include "mdrv_mhl.h"
#include "drvmStar.h"
#include "drvADC.h"
#include "Mapi_combo.h"
#include "mcu.h"
#if ENABLE_DAISY_CHAIN
#include "msAPI_DaisyChain.h"
#endif
#if(IR_REMOTE_SEL != NO_REMOTE)
#include "MsIr.h"
#include "IrFunc.h"
#endif
#include "Mapi_mhl.h"

////////////////////////////////////
extern void i2C_Intial(void);
///////////////////////////////////
void Main_SlowTimerHandler(void);
bit g_buncall = 0;
extern void msDrvPWMEnableSwitch(DWORD u32PWMCh, Bool bEnable);
#if !ENABLE_VGA_INPUT
void DrvADCNoUse(void);
#endif
void uncall_function(void)
{

    mhal_tmds_Initial(0, 0);
    msDrvIP1OfflineGetInputSyncType(0);
    msDrvIP1OfflineAdcGlitchRemoval(0, 0);
    msDrvIP1OfflineAdcSetCoast( 0,  0,  0,  0);
    msDrvSetOfflineIPMux(0,0);
    msDrvIP1OfflineReadVSyncWidth(0);
    msDrvIP1OfflineReadHperiod(0);
    msDrvIP1OfflineSetInputSyncType(0,0 );
    msDrvIP1OfflineReadSyncStatus(0);
    msDrvIP1OfflineReadVtotal(0);
    msDrvPWMEnableSwitch(0,0 );
#if ENABLE_VGA_INPUT	
    drvADC_init(0);
    drvADC_SetADCSource(0);
#else
    DrvADCNoUse();
#endif
    drvADC_PowerCtrl(0);
    mapi_combo_GetComboMask(0);
    mapi_combo_GetComboType(0);
    mapi_combo_GetComboInputPort(0);
    mcuDMACRC32(0,0,0,0,0,0);
    mcuDMADataTransfer(0,0,0,0,0,0);
    mcuDMAMemoryFill(0,0,0,0,0);
    mcuDMAPatternSearch(0,0,0,0,0);

#if ENABLE_CABLE_DET
    msAPISrcIsCableConnected(0);
#endif
#if(IR_REMOTE_SEL != NO_REMOTE)
    irDetectTimer0();
#endif
    i2C_Intial();
    ClearWDT();
    SetWDTClk(0);
    TriggerWDT(CLK_LIVE_XTAL, 0);
#if ENABLE_DAISY_CHAIN
	msPM_DCCmdInit();
	msPM_DCSetCmdDcOff(0, 0);
#endif

#if ENABLE_SW_DOUBLE_BUFFER
    DB_Mode(0);
    DB_WB(0,0);
    DB_W2B(0,0);
    DB_W3B(0,0);
    DB_WBMask(0,0,0);
    DB_W2BMask(0,0,0);
    DB_WBit(0,0,0);
#endif

    SetSPI_Quad_En(0);
}

////////////////////////////////////
///////////////////////////////////
void main( void )
{		
    Init_MCU();

#if( ENABLE_WATCH_DOG )
    ClearWDT();
#endif

    Init_GlobalVariables();

#if ENABLE_DEBUG
    printData("  g_u8SystemSpeedMode:%d", g_u8SystemSpeedMode);
#endif

#if !USEFLASH
   // i2C_Intial();
#endif

    msPM_Init();
    msWriteByteMask(REG_003A60, BIT5, (BIT6|BIT5));
    msWriteByte(REG_003A62, SARKEY_EN);

#if(IR_REMOTE_SEL != NO_REMOTE)
    irInitialize(); // initialize IR
#endif
#if ENABLE_MBX
    MApi_MBX_Init();
    MApi_MBX_Enable(TRUE);
    #if ENABLE_MBX_SAR
    MApi_MBX_RegisterMSG(E_MBX_CLASS_SAR, 32);
    #endif
#endif

#if ENABLE_BUZZER
    BuzzerInitial();
#endif

    if(g_buncall)
       uncall_function();

    while( 1 )
    {
		
        msPM_Handler();

#if( ENABLE_WATCH_DOG )
        ClearWDT();
#endif

#if 1 //(!R2_51_DUAL_MODE) // ???
        if(!msPM_IsState_IDLE())
            continue;
#endif

        Main_SlowTimerHandler();
        DebugHandler();
#if UART1
        UART1_Handler();
#endif

#if ENABLE_DEBUG
        if( DebugOnlyFlag )
        {
            continue;
        }
#endif

#if(IR_REMOTE_SEL != NO_REMOTE)
        irPollingHandler();
        irDecodeCommand(); // ir decode command
        irDecodeNumKey(); // ir decode number key
#endif

        Key_ScanKeypad();

#if ENABLE_MBX
        if(bMailNT)
        {
            bMailNT = 0;
            _MDrv_MBX_MsgRecvCb(E_INT_FIQ_R2_TO_8051);
        }
#endif
    }
}

void Main_SlowTimerHandler(void)
{
    if(ms10Flag)
    {
        Clr_ms10Flag();
    if(CheckPMcounter)
        CheckPMcounter++;

        mdrv_mhl_TimerHandler();
    }

    if(ms50Flag)
    {
        Clr_ms50Flag();
    }
#if ENABLE_BUZZER
    if(bBuzzer)
    {
        bBuzzer = 0;
        BuzzerMute();
    }
#endif
    if(SecondFlag)
    {
        Second++;

        #if EXT_TIMER0_1MS||FPGA_HW_PMMODE
            printData("Second: %d", Second);
        #endif
        #if ENABLE_R2_INT_M51_Test
        if(bR2_int_M51_flag == 1)
        {
            bR2_int_M51_flag=0x00;
            TestCommand_FIQ_INT(0xFF);
            #if ENABLE_DEBUG
            printData(" nonPM_FIQ interrupt......................................................",0);
            #endif
        }
    #endif

if(sPMInfo.ucPMMode == ePM_POWER_DPMS)
{

#if LED_FLICKER	
     #if (defined(LianHeChuangXin_Project)/*||defined(Sceptre_Project)*/)
     if(!(g_sMailBoxR2.u16PMFlag&BIT0))
     {
    	hw_ClrGreenLed();
    	hw_ClrAmberLed();
     }
     else
     #endif	
     {
    #if defined(ZhouYang_Project)
            if(hw_IsAmberLedOn)
            {
                hw_ClrAmberLed();
            }
            else
            {
                hw_SetAmberLed();
            }
    #else
     {
            if(hw_IsGreenLedOn)
            {
                hw_ClrGreenLed();
            }
            else
            {
                hw_SetGreenLed();
            }
     }
    #endif
     }
#endif

  if(sPMInfo.ucPMMode == ePM_POWER_DPMS)
  {
     if(PM_SleepModeCounter)
     	{
	       if(--PM_SleepModeCounter==0)
	       {
	          Set_SleepMode_Flag();
	       }
     	}
  }
 
}

        Clr_SecondFlag();
    }
}


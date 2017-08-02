#include "types.h"
#include "board.h"
#include "global.h"
#include "debug.h"
#include "debugDef.h"
#include "Common.h"
#include "ms_reg.h"
#include "Ms_rwreg.h"
#include "gpio_def.h"
#include "mcu.h"
#include "drvPM.h"
#include "halRWreg.h"
#include "drvMBX.h"
#include "Buzzer.h"
//#include "halRwreg.h"




//#define ReSyncTick          255//54 // 0.2 Second for resynchorize dubug process
#if EXT_TIMER0_1MS
#define EnableExt0Interrupt     0
#define EnableExt1Interrupt     1
#define EnableTime0Interrupt        0
#else
#define EnableExt0Interrupt     1
#define EnableTime0Interrupt        (!EXT_TIMER0_1MS)
#endif

#if (UseINT || ENABLE_SW_DOUBLE_BUFFER)
#define EnableExt1Interrupt     1
#define EnableSerialPortInterrupt   1 // 0
#else
#define EnableExt1Interrupt     1 //0
#define EnableSerialPortInterrupt   1
#endif

//#define EnableTime1Interrupt        1
//#define EnableTime2Interrupt        0

#if EXT_TIMER0_1MS
static void ms1MsHandler(void); // Jonson 20110713
#endif
#if ENABLE_SW_DOUBLE_BUFFER
extern XDATA StuSWDBCtrl SWDBCtrl;

void IsrML(void)
{
    SWDBCtrl.ucSWDB_Counter = 0;
    SWDBCtrl.ucSWDB_OnFire = 0;
    SWDBCtrl.ucSWDB_Num = 1;
    msRegs[REG_003C25] &=0x3F;
}
#endif

#if(IR_REMOTE_SEL != NO_REMOTE)
extern void irInterruptProcess(void);
#endif

#if UseINT
void MuteVideoAndAudio(void)
{

#if ENABLE_LED_CONTROLLER
    MEM_MSWRITE_BYTE(REG_0A80,0x00);
    MEM_MSWRITE_BYTE(REG_0A81,0x00);
#else
    hw_ClrBlacklit();
#endif
}

void IsrModeChangeCheck(void)
{
    MEM_SCWRITE_BYTE(0, 0); //bk0
    if (INT_STATUS_A_ISR(BIT4|BIT5|BIT6|BIT7) || INT_STATUS_B_ISR(BIT2))//((MEM_SCREAD_BYTE(SC0_CC)&(BIT4|BIT5|BIT6|BIT7)) || (MEM_SCREAD_BYTE(SC0_CD)&BIT2))
    {
        INT_SCALER_A_ISR(0);//MEM_SCWRITE_BYTE(SC0_CE, 0); //disable INT
        INT_SCALER_B_ISR(0);//MEM_SCWRITE_BYTE(SC0_CF, 0); //disable INT
        INT_STATUS_CLEAR_ISR();
//        if (!SyncLossState())
        {
            MuteVideoAndAudio();
        }
    }

}
#endif

#if EnableExt0Interrupt
void EXT0Interrupt( void )interrupt 0
{
    BYTE ucStatus;
    BYTE ucP2BK;
#if ENABLE_MBX
    BYTE u8MailStatus;
#endif
    ucP2BK = P2;

    ucStatus = MEM_MSREAD_BYTE_EXT0(REG_002B18);
    MEM_MSWRITE_BYTE(REG_002B18,ucStatus);

  #if 0//EXT_TIMER0_1MS
    if( ucStatus & _BIT0 )
        ms1MsHandler();
  #endif

#if ENABLE_R2_INT_M51_Test
	u8nonPMFIQStatus=0x00;
	if (u8HostS2D<=0x02)
	    u8nonPMFIQStatus = MEM_MSREAD_BYTE_EXT0(REG_10191E);
	else if (u8HostS2D>=0x03 && u8HostS2D<=0x08 )
	    u8nonPMFIQStatus = MEM_MSREAD_BYTE_EXT0(REG_10191D);
	else if (u8HostS2D>=0x09 && u8HostS2D<=11 )
	    u8nonPMFIQStatus = MEM_MSREAD_BYTE_EXT0(REG_10191C);
	if(u8nonPMFIQStatus)
	{
		if(bR2_int_M51_flag == 0)
			bR2_int_M51_flag = 1;
	}
	MEM_MSWRITE_BYTE(REG_10190C,0xFF);
	MEM_MSWRITE_BYTE(REG_10190D,0xFF);
	MEM_MSWRITE_BYTE(REG_10190E,0xFF);

	MEM_MSWRITE_BYTE(REG_10054C,0x00);
	MEM_MSWRITE_BYTE(REG_100548,0x00);
	MEM_MSWRITE_BYTE(REG_100544,0x00);
	MEM_MSWRITE_BYTE(REG_100540,0x00);

#endif
#if ENABLE_MBX
    u8MailStatus = MEM_MSREAD_BYTE_EXT0(REG_10191D);
    MEM_MSWRITE_BIT_ISR(_EXT0, REG_10191D, 1, BIT2);
    if(u8MailStatus & BIT2)
    {
        if(bMailNT == 0)
            bMailNT = 1;
    }
#endif

    P2 = ucP2BK;

    IE0 = 0;
}
#endif
extern BYTE xdata g_u8KeyDebounceCounter;
extern WORD xdata g_u32KeyCounter;

#if EnableTime0Interrupt
//extern WORD ms_Counter;
//extern BYTE ReSyncCounter;
//extern BYTE Second;
//extern BYTE xdata OsdCounter;
//extern BYTE xdata PowerDownCounter;
extern XDATA ePM_WakeupStatus ucWakeupStatus;

void Time0Interrupt( void )interrupt 1
#else
static void ms1MsHandler(void)
#endif
{
    TH0 = g_ucTimer0_TH0;
    TL0 = g_ucTimer0_TL0;

    ms_Counter--;

#if ENABLE_DEBUG
    if( ReSyncCounter )
    {
        ReSyncCounter--;
        if( ReSyncCounter == 0)
        {
            //Clr_RxBusyFlag();
            SIORxIndex = 0;
        }
    }
#endif
#if UART1
    if( ReSyncCounter1)
    {
        ReSyncCounter1--;
        if( ReSyncCounter1 == 0 )
        {
            SIORxIndex1 = 0;
        }
    }
#endif
#if ENABLE_BUZZER
    if(g_ucPWMTimer)
    {
        g_ucPWMTimer--;
        if(g_ucPWMTimer==0)
            bBuzzer = 1;
    }
#endif

    if( bDelayFlag )
        if( --DelayCounter == 0 )
            bDelayFlag = 0;

    if(TimeOutCounter)
        TimeOutCounter--;

    if(ModeDetectCounter)
         ModeDetectCounter--;


    if(g_u8KeyDebounceCounter)
        g_u8KeyDebounceCounter--;
    if(g_u32KeyCounter)
        g_u32KeyCounter--;

    if(g_u32MailCounter)
        g_u32MailCounter++;

    if(!g_bMcuPMClock) // system runs at normal speed
    {
    }

    if( ms_Counter == 0 )        //
    {
        Set_SecondFlag();
        ms_Counter = SystemTick;
    }

    if(( ms_Counter % 10 ) == 0 )
    {
        Set_ms10Flag();
        #if (IR_REMOTE_SEL!=NO_REMOTE)
          if( IRkeyDelayCounter)
            IRkeyDelayCounter--;
        #endif

        if(( ms_Counter % 50 ) == 0 )
        {
            Set_ms50Flag();

            if(( ms_Counter % 500 ) == 0 )
            {
                Set_ms500Flag();
            }
        }

        if(( ms_Counter % 200 ) == 0 )
        {
         #if _NEW_SOG_WAKEUP_DET_
                if(ucWakeupStatus == ePMSTS_NON)
                       NEW_SOG_WAKEUP_FLUSH_ISR(_TIME0);
        #endif
        }
    }

    if(TF0)
    {
        TH0 = g_ucTimer0_TH0;
        TL0 = g_ucTimer0_TL0;
    }

    TF0 = 0; // timer 1 flag

}
//#endif
#if EnableExt1Interrupt
void EXT1Interrupt( void )interrupt 2
{
    BYTE ucStatus_002B38, ucStatus_002B18;
    BYTE ucP2BK;

    ucP2BK = P2;

    //MEM_MSWRITE_BYTE(REG_002B00,1);
    ucStatus_002B18 = MEM_MSREAD_BYTE_EXT1(REG_002B18);
    ucStatus_002B38 = MEM_MSREAD_BYTE_EXT1(REG_002B38);

    //  EX1=0;

// BYTE ucBank = MEM_SCREAD_BYTE(0x00);       //110926 Rick add to store the current bank - Verify A036, A039

if( _BIT1 & ucStatus_002B38) // PM_IRQ
{
#if ENABLE_WATCH_DOG_INT
    if( _BIT1 & ucStatus_002B18 ) // WDT int
    {
	    u8WDT_Status=0x33;
    }
#endif
#if EXT_TIMER0_1MS
    if( _BIT0 & ucStatus_002B18)    //Ext Timer
    {
        ms1MsHandler();
    }
#endif
#if  ENABLE_SW_DOUBLE_BUFFER
    if(_bit4_(ucStatus_002B18))
        IsrML();
#endif
}
#if(IR_REMOTE_SEL != NO_REMOTE)
    if(_bit3_(ucStatus_002B18))
    {
        irInterruptProcess();
    }
#endif

#if UseINT
    if(INT_STATUS_DISP_ISR())
    {
        IsrModeChangeCheck();
    }
#endif
    MEM_MSWRITE_BYTE(REG_002B18,ucStatus_002B18);
   // MEM_MSWRITE_BYTE(REG_002B38,ucStatus_002B38);     Do not clear IRQ status

//    MEM_SCWRITE_BYTE(0x00, ucBank);
    IE1 = 0;
    //  EX1=1;
    P2 = ucP2BK;
}

#endif

#if EnableTime1Interrupt
void Time1Interrupt( void )interrupt 3
{
    {
        TH1=g_ucTimer0_TH0;
        TL1=g_ucTimer0_TL0; // timer 1 counter
    }


    g_u16MsCounter1--;
    if (g_u16MsCounter1==0) //
    {
        bSecondTimer1 = TRUE;
        g_u16MsCounter1=1000;
    }
}
#endif
#if EnableSerialPortInterrupt
void SerialPortInterrupt( void )interrupt 4
{
    BYTE sioData;

    if( RI )
    {
        RI = 0;
        sioData = SBUF;
        if( SIORxIndex < _UART_BUFFER_LENGTH_ )
        {
            SIORxBuffer[SIORxIndex++] = sioData;
            if(SIORxIndex < UART_CMD_LENGTH)
                ReSyncCounter = ReSyncTick;
            else
                ReSyncCounter = 0xFF;
        }
    }
    if( TI )
    {
        TI = 0;
        TI0_FLAG = 1;
    }
}
#endif
#if EnableTime2Interrupt
void Time2Interrupt( void )interrupt 5
{
    TF2 = 0;
}
#endif
#if UART1
void SerialPort1Interrupt (void)interrupt 16
{
    BYTE sioData;

    if(S1CON & RI1)
    {
        S1CON &= ~RI1;
        sioData = S1BUF;
        if( SIORxIndex1 < _UART_BUFFER_LENGTH_ )
        {
            //ReSyncCounter = ReSyncTick;
            SIORxBuffer1[SIORxIndex1++] = sioData;
            if(SIORxIndex1 < UART_CMD_LENGTH)
                ReSyncCounter1 = ReSyncTick;
            else
                ReSyncCounter1 = 0xFF;

        }
    }

    if(S1CON & TI1)
    {
        S1CON &= ~TI1;
        TI1_FLAG = 1;
    }
}
#endif


void EXT3Interrupt( void )interrupt 10
{

}

void EXT2Interrupt( void )interrupt 9
{

}


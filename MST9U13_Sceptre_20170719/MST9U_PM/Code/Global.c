#include "types.h"
#include "board.h"
#include "global.h"
#include "Debug.h"
#include "Common.h"
#include "Keypad.h"
#include "drvPM.h"
#include "msAPI_MailBox.h"
#include "drvGPIO.h"
#include "Keypaddef.h"

#if ENABLE_WATCH_DOG_INT
BYTE xdata u8WDT_Status=0;
#endif

#if EnableTime1Interrupt
WORD XDATA g_u16MsCounter1 = 1000; // for System timer
BOOL XDATA bSecondTimer1 = FALSE;
BYTE idata u8SecondTIMER1 = 0;
#endif

BYTE g_ucTimer0_TH0;
BYTE g_ucTimer0_TL0;
BYTE xdata g_u8SystemSpeedMode=0;
bit g_bMcuPMClock = 0;

BYTE xdata VKeyPad=VK_NOTHING;

WORD xdata SystemFlags = 0; // for system status flags

bit SecondFlag = 0;
bit ms10Flag = 0;
bit ms50Flag = 0;
bit ms500Flag = 0; // 120815 coding, set to bit data type for isr usage
WORD ms_Counter = SystemTick; // for System timer
BYTE xdata Second;
BYTE xdata KeypadButton = 0;
BYTE xdata ModeDetectCounter = 20;
WORD xdata g_u32MailCounter = 1;
WORD xdata CheckPMcounter =0;

#if (IR_REMOTE_SEL!=NO_REMOTE)
BYTE xdata IRkeyDelayCounter=0;
#endif

bit TI0_FLAG; // 120815 coding, set to bit data type for isr usage
BYTE xdata SIORxBuffer[_UART_BUFFER_LENGTH_];
BYTE xdata SIORxIndex;
BYTE ReSyncCounter;

#if UART1
bit TI1_FLAG;
BYTE xdata SIORxBuffer1[_UART_BUFFER_LENGTH_];
BYTE xdata SIORxIndex1;
BYTE ReSyncCounter1;
#endif

WORD xdata PM_SleepModeCounter = 0;


BYTE XDATA MailBox[0x1000] _at_ 0xC000;
DWORD XDATA XDATA_TO_DRAM_ADDR _at_ XDATA_MBX_XD2MIU_ADDR;
#if ENABLE_MBX
bit bMailNT =0;
#endif
#if ENABLE_R2_INT_M51_Test
bit bR2_int_M51_flag=0;
BYTE u8nonPMFIQStatus=0;
BYTE u8HostS2D=0;
#endif
bit DebugOnlyFlag = 0;
WORD DelayCounter = 0;
bit bDelayFlag = 0;


BYTE xdata TimeOutCounter = 0; // 120510 coding, addition for checking register status
BYTE xdata g_ucPWMTimer = 0;
bit bBuzzer = 0;
bit g_bCorePowerStatus=1;
bit g_bR2Status=1;             // 1 : alive , 0 : power down
#if ENABLE_DRAM_SELFREFRESH
bit g_bSTRStatus=0;            // 1 : dram in STR , 0 : dram not in STR
#endif
extern void msDebugDummy(void);

void DummyCall(void)
{
    BYTE i = 0;

    if(i)
    {
#if !ENABLE_DEBUG
        msDebugDummy();
#endif
        // Uncalled warnings
        msRead2Byte(0);
        msWrite3Byte(0,0);
        msWrite4Byte(0,0);
        msWrite2ByteMask(0,0,0);
        Delay1ms(0);
        Delay4us();

        mcuDMADownloadCode(0,0,0,0,0);
        mcuDMACRC32(0,0,0,0,0,0);
        mcuDMAMemoryFill(0,0,0,0,0);
        mcuDMAPatternSearch(0,0,0,0,0);
        mcuArrangeCodeAddr(0,0,0);
        mcuXdataMapToSRAM(0,0);
        codeMoveToXdata(0,0,0);
        mcuInitXdataMapToDRAM();
        mcuXdataMapToDRAM(0);
        PMDummy();
        Key_ScanKeypad();
        msDrvMcuMailBoxRead();
        msPM_IsState_IDLE();
        #if ENABLE_MBX
        Mailbox_dummy();
        #endif
        drvGPIO_dummy();
        MsOS_DeleteMutex(0);
    }
}

void Init_GlobalVariables( void )
{
/////////////////////////////////////////////////
// add for prevent complier warning
    DummyCall();
//////////////////////////////////////////////////
    Second = 0;

    PM_SleepModeCounter=g_sMailBoxR2.u16PMCounter;

    Clr_SleepMode_Flag();	

}

Bool MsOS_DeleteMutex (Slong s32MutexId)
{
    s32MutexId = s32MutexId;
    return TRUE;
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


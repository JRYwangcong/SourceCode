
///////////////////////////////////////
// DRVMCU_V VERSION: V02
////////////////////////////////////////
#define _DRVMCU_C_
#include "Global.h"
#include "appSystem.h"
#include "SysInit.h"

#define DRCMCU_DEBUG    1
#if ENABLE_MSTV_UART_DEBUG && DRCMCU_DEBUG
#define DRVMCU_PRINT(format, ...)     printf(format, ##__VA_ARGS__)
#else
#define DRVMCU_PRINT(format, ...)
#endif

void msAPI_InitWDT( BYTE bEnable )
{
    if( bEnable )
    {
        // initialize the watchdog timer reset interval
        MDrv_Sys_SetWatchDogTimer(WATCH_DOG_TIME_RESET);
        MDrv_Sys_EnableWatchDog();
    }
    else
    {
        MDrv_Sys_DisableWatchDog();
    }
}
void msAPI_ClearWDT( void )
{
#if R2_51_DUAL_MODE
    if(IsWDTAck())
    {
        MDrv_Sys_ClearWatchDog();
        SetWDTAck();
    }
#else
    MDrv_Sys_ClearWatchDog();
#endif
}

void msAPI_TriggerWDT( void )
{
    MDrv_Sys_SetWatchDogTimer(1);
    MDrv_Sys_EnableWatchDog();
    while(1);
}

void msDrvMcuR2Reset51(void)
{
    msWriteByte(REG_000393, 0x5F);
    msWriteByteMask(REG_00038A, BIT7, BIT7|BIT6);
}

void msDrvMcuR2To51(void)
{
    DRVMCU_PRINT("[MCU] R2 -> 51\n");

    MsOS_CPU_DisableInterrupt();
    MsOS_DisableAllInterrupts();

    //currently, MsOS_Dcache_Flush() will flush entire dcache, so, size is not matter
    MsOS_Dcache_Flush(0, 0x2000);
    MsOS_FlushMemory();

    INT_MASK_ALL();
    msDrvMcuMailBoxWrite();

#if R2_51_DUAL_MODE
    if(!Is51Active())
#endif
    {
        msDrvMcuWakeUp51();
    }

    while(1);
}

void msDrvMcuWakeUp51(void)
{
   msWriteByte(REG_0010F4, 1);    //spi offset enable
   //move to sboot since AP doesn't know the PM.bin address if size of sboot.bin is not fixed
   //msWrite2Byte(REG_0010F6, (WORD)PM_SPI_START_ADDR);
   //msWrite2Byte(REG_0010F8, (PM_SPI_START_ADDR >>16));
   msWriteByteMask(REG_00039D, 0, BIT6);
   msWrite2Byte(REG_000398, 0x829F);   //reset 51 pwd
   msWriteByteMask(REG_00039D, BIT6, BIT6);  //fire
   msWriteByteMask(REG_00038A, BIT6, BIT6);  //sw reset
}

void msDrvMcuMailBoxRead(void)
{
    BYTE i;
    BYTE *pu8Tmp    = (BYTE *)(&g_sMailBox51);
    BYTE u8MBSize   = sizeof(sMAILBOX_51);

    if(IsMailBoxValid())
    {
        for( i=0 ; i<u8MBSize ; i++ )
        {
            *(pu8Tmp+i) = msReadByte(REG_103380+i);
        }
    }
    else
    {
        msDrvMcuMailBoxClear();
    }
}

void msDrvMcuMailBoxWrite(void)
{
    BYTE i;
    BYTE *pu8Tmp    = (BYTE *)(&g_sMailBoxR2);
    BYTE u8MBSize   = sizeof(sMAILBOX_R2);

    msDrvMcuMailBoxClear();

    for( i=0 ; i<u8MBSize ; i++ )
    {
        msWriteByte(REG_103380+i, *(pu8Tmp+i));
    }

    SetMailBoxValid();
}

void msDrvMcuMailBoxClear(void)
{
    BYTE i;

    for( i=0 ; i<NUM_MAILBOX ; i++ )
    {
        msWriteByte(REG_103380+i, 0x00);
    }
}

void msDrvMcuCheckSRBoot(void)
{
    if(IsSelfRefreshBoot())
    {
        Set_SelfRefreshBootFlag();
    }
    else
    {
        Clr_SelfRefreshBootFlag();
    }
}

BOOL msDrvMcuCheck51Active(void)
{
    BYTE u8Rty=10;
    BYTE u8Rtn=FALSE;

    while(u8Rty--)
    {
        if(msRead3Byte(REG_0010FC)!=0x000000)
        {
            u8Rtn = TRUE;
            break;
        }
    }

    return u8Rtn;
}

void msDrvMcuXD2MIUAddrSet(DWORD dwAddr)
{
    R2_XSRAM_WRITE(XDATA_MBX_XD2MIU_ADDR+3,(dwAddr)&0xFF);
    R2_XSRAM_WRITE(XDATA_MBX_XD2MIU_ADDR+2,(dwAddr>>8)&0xFF);
    R2_XSRAM_WRITE(XDATA_MBX_XD2MIU_ADDR+1,(dwAddr>>16)&0xFF);
    R2_XSRAM_WRITE(XDATA_MBX_XD2MIU_ADDR,  (dwAddr>>24)&0xFF);
}
/*
enum // UART GPIO
{
    UART_GPIO02_03,
    UART_GPIO40_41,  // VGA ?
    UART_GPIO50_51,
    UART_GPIOX03_04, // combo 0
    UART_GPIOX13_14, // combo 1
    UART_GPIOX23_24, // combo 2
    UART_GPIOX33_34, // combo 3
    UART_GPIOX43_44, // combo 4
    UART_GPIOX53_54, // combo 5
};
enum // UART Engine
{
    51_UART0,
    51_UART1,
    R2_HK,
    R2_NONHK,
    DW_uart,
    RESERVED,
};
*/
#if ENABLE_MSTV_UART_DEBUG
void mcuSetUartMux( UART_ENGINE ucUartEngine, UART_GPIO ucUartGPIO )
{
    switch(ucUartGPIO)
    {
        case UART_GPIO02_03:
            msWriteByteMask(REG_000414, ucUartEngine, 0x07);
            break;
        case UART_GPIO40_41:
            msWriteByteMask(REG_000415, ucUartEngine, 0x07);
            break;
        case UART_GPIO50_51:
            msWriteByteMask(REG_000414, ucUartEngine<<4, 0x70);
            break;
        case UART_GPIOX03_04:
            msWriteByteMask(REG_000410, ucUartEngine, 0x07);
            break;
        case UART_GPIOX13_14:
            msWriteByteMask(REG_000410, ucUartEngine<<4, 0x70);
            break;
        case UART_GPIOX23_24:
            msWriteByteMask(REG_000411, ucUartEngine, 0x07);
            break;
        case UART_GPIOX33_34:
            msWriteByteMask(REG_000411, ucUartEngine<<4, 0x70);
            break;
        case UART_GPIOX43_44:
            msWriteByteMask(REG_000412, ucUartEngine, 0x07);
            break;
        case UART_GPIOX53_54:
            msWriteByteMask(REG_000412, ucUartEngine<<4, 0x70);
            break;
        default:
            DRVMCU_PRINT("******UART MUX ERROR*****\n");
            break;
    }
}
#endif

// u16Addr: MUST start from 0x4000
BYTE R2_XSRAM_READ(WORD u16Addr)
{
    WORD u16TimeOut=0xFFFF;

    u16Addr -= XDATA_ADDR_START;

    msWrite2Byte(REG_003C02, u16Addr); // address
    msWriteBit(REG_003C06, TRUE, BIT1); // read request

    while((msReadByte(REG_003C06)&BIT5) && (--u16TimeOut)); // wait busy flag

    return msReadByte(REG_003C01);
}

// u16Addr: MUST start from 0x4000
void R2_XSRAM_WRITE(WORD u16Addr, BYTE u8Data)
{
    WORD u16TimeOut=0xFFFF;

    u16Addr -= XDATA_ADDR_START;

    msWrite2Byte(REG_003C02, u16Addr); // address
    msWriteByte(REG_003C00, u8Data); // data
    msWriteBit(REG_003C06, TRUE, BIT0); // write request

    while((msReadByte(REG_003C06)&BIT5) && (--u16TimeOut)); // wait busy flag
}


// Timer0 32-bit counter
#define US_UNIT     (CRYSTAL_CLOCK/1000000)
void DelayUs(BYTE u8Delayus)
{
#if 1
    MsOS_DelayTaskUs(u8Delayus);
#else
    msWrite4Byte(REG_002C24, (DWORD)u8Delayus*US_UNIT);
    msWriteByte(REG_002C20, 0x02);
    while(!(msReadByte(REG_002C22) & _BIT0))
    {}
    msWriteByte(REG_002C20, 0x00);
#endif
}


void msDrvMcuR2Miu1(void)
{
    msWriteByteMask(REG_100403, _BIT7|_BIT1, _BIT7|_BIT3|_BIT2|_BIT1);
}


#define _MCU_C_
#include "types.h"
#include "board.h"
#include "misc.h"
#include "debug.h"
#include "Common.h"
#include "ms_rwreg.h"
#include "Reg52.h"
#include "global.h"
#include "ms_reg.h"
#include "mcu.h"
#include "gpio_def.h"

#define MCU_DEBUG    1
#if ENABLE_DEBUG&&MCU_DEBUG
    #define MCU_printData(str, value)   printData(str, value)
    #define MCU_printMsg(str)           printMsg(str)
#else
    #define MCU_printData(str, value)
    #define MCU_printMsg(str)
#endif


//////////////////////////////////
/////////////////////////////////
void Init_WDT( BYTE bEnable )
{
    if( bEnable )
    {
#if( ENABLE_WATCH_DOG )
        // initialize the watchdog timer reset interval
        SetWDTClk(CLK_LIVE_XTAL);

      #if ENABLE_WATCH_DOG_INT
        msWriteBit(REG_002B08, 0, BIT1);
        msWriteBit(REG_002B28, 0, BIT1);
      #endif
#endif  // end of #if( ENABLE_WATCH_DOG )
    }
    else
    {
        msWrite2Byte( REG_002C08, 0x0000 );
        msWrite2Byte( REG_002C0A, 0x0000 );
        msWriteBit(REG_002B08, 1, BIT1);
    }
}
void SetWDTClk(BYTE clk_live_sel)
{
    ClearWDT();
    if (clk_live_sel == CLK_LIVE_RCOSC_4M)
    {
        msWrite4Byte( REG_002C08, WATCH_DOG_TIME_RESET * CLOCK_4MHZ); // need set from H-byte to L-byte if default is not maximum
#if ENABLE_WATCH_DOG_INT
        msWrite2Byte( REG_002C06, (WATCH_DOG_TIME_Interrupt * CLOCK_4MHZ)>>16 );
#endif
    }
    else // CLK_LIVE_XTAL
    {
        msWrite4Byte( REG_002C08, WATCH_DOG_TIME_RESET * CRYSTAL_CLOCK ); // need set from H-byte to L-byte if default is not maximum
#if ENABLE_WATCH_DOG_INT
        msWrite2Byte( REG_002C06, (WATCH_DOG_TIME_Interrupt * CRYSTAL_CLOCK)>>16 );
#endif
    }
}
void ClearWDT( void )
{
#if R2_51_DUAL_MODE
    if(IsWDTAck() || !msPM_IsState_IDLE())
    {
        WDT_CLEAR();
        SetWDTAck();
    }
#else
    WDT_CLEAR();
#endif
}

void TriggerWDT( BYTE clk_live_sel, BYTE second )
{
    ClearWDT();
    if (clk_live_sel == CLK_LIVE_RCOSC_4M)
    {
        msWrite4Byte( REG_002C08, second * CLOCK_4MHZ); // need set from H-byte to L-byte if default is not maximum
    }
    else // CLK_LIVE_XTAL
    {
        msWrite4Byte( REG_002C08, second * CRYSTAL_CLOCK ); // need set from H-byte to L-byte if default is not maximum
    }
    while(1);
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
#if (ENABLE_DEBUG || UART1) //ENABLE_DEBUG:UART 0, UART1: UART 1
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
            MCU_printMsg("******UART MUX ERROR*****");
            break;
    }
}
#endif

void SetSPI_Quad_En(BYTE ucEnable)
{
    //    WREG(0x09C0) = 0x06;   //SPI Flash WREN Command
    //    WREG(0x09C1) = 0x01;   //SPI Flash Write Command
    msWrite2Byte(REG_0009C0, 0x0106);
    //    WREG(0x09C2) = 0x00;   //ADR(23:16)
    //    WREG(0x09C3) = 0x02;  //ADR(15:8)
    if (ucEnable==TRUE)
        msWrite2Byte(REG_0009C2, 0x0200);
    else
        msWrite2Byte(REG_0009C2, 0x0000);
    //    WREG(0x09C4) = 0x05;   //SPI Flash RDSR Command
    msWrite2Byte(REG_0009C4, 0x0005);
    //    WREG(0x09D4) = 0x31;   //Write length: 1 Bytes / 8 Bytes / 1 Bytes
    //    WREG(0x09D5) = 0x01;
    msWrite2Byte(REG_0009D4, 0x0131);

    //    WREG(0x09D6) = 0x00;   //Read length: 0 Bytes / 0 Bytes / 1 Bytes
    //    WREG(0x09D7) = 0x01;
    msWrite2Byte(REG_0009D6, 0x0100);

    //    WREG(0x09D9) = 0xf0;   //enable second/third commands and auto check status
    //    WREG(0x09D8) = 0x07;  //fsp enable and interrupt enable
    msWrite2Byte(REG_0009D8, 0xf007);

    //    WREG(0x09DA) = 0x01;        //Trigger
    msWrite2Byte(REG_0009DA, 0x0001);

    //Done = RREG(0x09DC, Bit(0))  //Check FSP done flag
    //WREG(0x09DE, Bit(0)) = 1     //Clear FSP done flag
    //    while(!(WREG(0x09DC)&BIT0))
    DelayCounter = 10;
    bDelayFlag = 1;
    while( (!(msReadByte(REG_0009DC)&BIT0)) && bDelayFlag );
#if ENABLE_DEBUG
    if( !bDelayFlag )
        printMsg("SetSPI_Quad_En Timeout !!!");
#endif

    msWrite2Byte(REG_0009DE, (msReadByte(REG_0009DE)|BIT0));     //Clear FSP done flag

}

//-------------------------------------------------------------------------------------------------
// spi clock
// SPI clock setting
//
// reg_reserved0[5:0]
//
// h0027    h0026   31  0   reg_reserved0   "Reserved.reg_reserved0[5:0]: for spi clock selectionreg_reserved0[8]: for spi new cycle"
//
//
// reg_ckg_spi[6]
//  0: Crystal clock
//  1: PLL clock
//
// reg_ckg_spi[4:2], clock selection
//
// ucIndex ( PLL clock selection)
void mcuSetSpiSpeed( BYTE ucIndex )
{
    if( ucIndex == IDX_SPI_CLK_XTAL)
    {
        msWriteByteMask( REG_000AE0, 0, _BIT2 );
    }
    else
    {
        msWriteByteMask( REG_000AE0, ucIndex << 3, _BIT5 | _BIT4 | _BIT3);
        msWriteByteMask( REG_000AE0, _BIT2, _BIT2 );
    }
}

/*
SPI model select.
0x0: Normal mode, (SPI command is 0x03)
0x1: Enable fast read mode, (SPI command is 0x0B)
0x2: Enable address single & data dual mode, (SPI command is 0x3B)
0x3: Enable address dual & data dual mode, (SPI command is 0xBB)
0xa: Enable address single & data quad mode, (SPI command is 0x6B) (Reserved)
0xb: Enable address quad & data quad mode, (SPI command is 0xEB)
(Reserved
*/
void mcuSetSpiMode( BYTE ucMode )
{
    BYTE XDATA ucValue;

    switch( ucMode )
    {
        case SPI_MODE_FR:
            ucValue = 0x01;
            break;

        case SPI_MODE_SADD:
            ucValue = 0x02;
            break;

        case SPI_MODE_DADD:
            ucValue = 0x03;
            break;

        case SPI_MODE_SAQD:
            ucValue = 0x0A;
            break;

        case SPI_MODE_QAQD:
            ucValue = 0x0B;
            break;

        case SPI_MODE_NORMAL:
        default:
            ucValue = 0x00;
    }

    //printf(" SPI_MODE=0x%x\r\n", MDrv_Read2Byte(REG_SEL_MODE));
    if ((ucMode == SPI_MODE_SAQD)||(ucMode == SPI_MODE_QAQD))
    {
#if 0   // no semaphore protect, may conflict w/ R2 fsp code and hang
        msWriteByte( REG_000AE4, SPI_MODE_FR ); // For FSP in SetSPI_Quad_En()
        SetSPI_Quad_En(TRUE);
#endif
        msWriteByteMask( REG_000405, _BIT3, _BIT3 ); //spi_quad_en
        //printData( "Enable QUAL Mode = %d ", ucMode );
    }
    else
    {
#if 0   // no semaphore protect, may conflict w/ R2 fsp code and hang
        msWriteByte( REG_000AE4, SPI_MODE_FR ); // For FSP in SetSPI_Quad_En()
        SetSPI_Quad_En(FALSE);
#endif
        msWriteByteMask( REG_000405, 0x00, _BIT3 ); //spi_quad_en
        //printData( "Disable QUAL Mode = %d ", ucMode );
    }
    msWriteByte( REG_000AE4, ucValue ); // SPI model select

#if ENABLE_DEBUG
    printData( " SPI Mode = %d ", ucMode );
#endif
}


DWORD code g_mcuPLLFreqTable[] =
{
    CLOCK_4MHZ,            // 0
    CLOCK_12MHZ,           // 1
    CLOCK_216MHZ,          // 2
    CLOCK_MPLL_MHZ,        // 3,
    CLOCK_108MHZ,          // 4,
    CLOCK_86MHZ,           // 5,
    CLOCK_54MHZ,           // 6,
    CLOCK_0MHZ,            // 7
    CLOCK_XTAL,            // 8
};


void mcuSetMcuSpeed( BYTE ucSpeedIdx )
{
    DWORD XDATA u32Freq;
    WORD XDATA u16Divider0;
    WORD XDATA u16Divider1;

    u32Freq = g_mcuPLLFreqTable[ucSpeedIdx];
    u16Divider0 = 1024 - (( _SMOD * u32Freq + u32Freq ) / SERIAL_BAUD_RATE ) / 64;
    u16Divider1 = 1024 - (( u32Freq ) / SERIAL_BAUD_RATE ) / 32;
    if ( ucSpeedIdx == IDX_MCU_CLK_XTAL )
        msWriteByteMask( REG_0003BC, 0, _BIT0 );
    else
    {
        msWriteByteMask( REG_0003BB, ucSpeedIdx, ( _BIT2 | _BIT1 | _BIT0 ) );
        msWriteByteMask( REG_0003BC, _BIT0, _BIT0 );
    }

    // Scaler WDT
    //MST9U ?? msWriteByte(SC0_00, 0x00);
    //MST9U ?? msWriteByte(SC0_B2, (u32Freq*4)/CRYSTAL_CLOCK);

#if ENABLE_DEBUG
    ES = 0;
    S0RELH = HIBYTE( u16Divider0 );
    S0RELL = LOBYTE( u16Divider0 );
    ES = 1;
    printData( "MCU freq = %d MHz ", u32Freq / 1000 / 1000 );
#endif

#if UART1
    IEN2 &= ~ES1;
    S1RELH = HIBYTE( u16Divider1 );
    S1RELL = LOBYTE( u16Divider1 );
    IEN2 |= ES1;
#endif

#if !EXT_TIMER0_1MS
    // timer
    u32Freq = u32Freq / 1000;
    u16Divider0 = ( 65536 - ( u32Freq * INT_PERIOD + 6 ) / 12 );
    EA = 0;
    TR0 = 0;
    ET0 = 0;
    TH0 = g_ucTimer0_TH0 = HIBYTE( u16Divider0 );
    TL0 = g_ucTimer0_TL0 = LOBYTE( u16Divider0 );
    TR0 = 1;
    ET0 = 1;
    EA = 1;
#endif
}
//=========================================================
void mcuSetSystemSpeed(BYTE u8Mode)
{
#if 0//def FPGA
    u8Mode = SPEED_XTAL_MODE;
#endif

    if (g_u8SystemSpeedMode!=u8Mode)
    { //MCU speed >= SPI speed
        switch(u8Mode)
        {
            case SPEED_4MHZ_MODE:
                mcuSetSpiSpeed(IDX_SPI_CLK_4MHZ); //spi speed down 1st
                mcuSetSpiMode( SPI_MODE_FR );
                mcuSetMcuSpeed(IDX_MCU_CLK_4MHZ);
                g_bMcuPMClock = 1; // 120925 coding addition
                break;
            case SPEED_12MHZ_MODE:
                if (g_u8SystemSpeedMode>SPEED_12MHZ_MODE)
                {
                    mcuSetSpiSpeed( IDX_SPI_CLK_12MHZ );
                    mcuSetSpiMode( SPI_MODE_NORMAL );
                    mcuSetMcuSpeed( IDX_MCU_CLK_12MHZ);
                }
                else
                {
                    mcuSetMcuSpeed( IDX_MCU_CLK_12MHZ );
                    mcuSetSpiSpeed( IDX_SPI_CLK_12MHZ );
                }
                g_bMcuPMClock = 1; // 120925 coding addition
                break;
            case SPEED_XTAL_MODE:
                mcuSetSpiMode( SPI_MODE_FR );
                if (g_u8SystemSpeedMode>SPEED_XTAL_MODE)
                {
                    mcuSetSpiSpeed( IDX_SPI_CLK_XTAL );
                    mcuSetMcuSpeed( IDX_MCU_CLK_XTAL );
                }
                else
                {
                    mcuSetMcuSpeed( IDX_MCU_CLK_XTAL );
                    mcuSetSpiSpeed( IDX_SPI_CLK_XTAL );
                }
                g_bMcuPMClock = 1; // 120925 coding addition
                break;
            default: //normal
                mcuSetSpiMode( SPI_MODE );
                msWriteByte( REG_101EDC, 0x08 );
                msWriteByte( REG_101ED0, 0x02 );
                msWriteByte( REG_101ED1, 0x04 );
                ForceDelay1ms(10);
                mcuSetMcuSpeed( MCU_SPEED_INDEX );
                mcuSetSpiSpeed( SPI_SPEED_INDEX );
                g_bMcuPMClock = 0; // 120925 coding addition
              break;
        }
        g_u8SystemSpeedMode=u8Mode;
        //SetForceDelayLoop();
    }
}

void Init_MCU( void )
{
    EA=0;

#if Enable_Cache
    CACHE_ENABLE();
#else
    CACHE_DISABLE();
#endif

#if( ENABLE_WATCH_DOG )
    Init_WDT( _ENABLE );
#else
    Init_WDT( _DISABLE );
#endif  // end of #if( ENABLE_WATCH_DOG )

    Init_IOPorts();
    Init_ExternalInterrupt();
    Init_Timer();
    Init_SerialPort();
#if (!R2_51_DUAL_MODE)
    msDrvMcu51ResetR2();
#endif
    msDrvMcuMailBoxRead();

#if ENABLE_SW_DOUBLE_BUFFER
    msSWDBInit();
#endif

    EA=1;

#if defined(FPGA)
    mcuSetUartMux((UART1)?(UART_ENGINE_51_UART1):(UART_ENGINE_51_UART0) , UART_GPIO40_41);
    mcuSetSystemSpeed(SPEED_XTAL_MODE);
#else
  #if ENABLE_DEBUG
    mcuSetUartMux(UART_ENGINE_51_UART0, DFT_51_UART0_GPIO);
  #endif
  #if UART1
    mcuSetUartMux(UART_ENGINE_51_UART1, DFT_51_UART1_GPIO);
  #endif

    mcuSetSystemSpeed(SPEED_NORMAL_MODE);
#endif

    mcuInitXdataMapToDRAM();
    mcuXdataMapToDRAM(XDATA_TO_DRAM_ADDR);
}


// Initialize I/O setting
void Init_IOPorts( void )
{
#if 0//tmp
    hw_SetDDC_WP();
    hw_SetFlashWP();
    #if !ENABLE_LED_CONTROLLER
    #endif
    hw_ClrPanel();
    hw_ClrGreenLed();
    hw_ClrAmberLed();

    Init_hwDDC_WP_Pin();
    Init_hwFlash_WP_Pin();
    Init_hwBlacklit_Pin();
    Init_hwPanel_Pin();
    Init_hwGreenLed_Pin();
    Init_hwAmberLed_Pin();

    Init_PowerKey();
    Init_hwDSUBCable_Pin();
#endif

    //MEM_MSWRITE_BIT(_REG_GPIO1_OEZ, 0, BIT1);

    //hw_SetFlashWP();
    //Init_hwFlash_WP_Pin();
    Init_hwBlacklit_Pin();
    Init_hwGreenLed_Pin();

    INIT_HW_POWER_KEY();
    Init_hwEEPROM_WP();

#if SwitchPortByDP_Detect_PM
    Init_DP_DET_Pin();
#endif

}
/* initialize 8051 CPU timer & interrupt routine */
/* TCON.7(   TF1): Timer 1 overflow flag */
/* TCON.6(   TR1): Timer 1 stop/start bit */
/* TCON.5(   TF0): Timer 0 overflow flag */
/* TCON.4(   TR0): Timer 0 stop/start bit */
/* TCON.3(   IE1): Timer 1 INT1 interrupt flag */
/* TCON.2(   IT1): Timer 1 INT1 interrupt style setup*/
/* TCON.1(   IE0): Timer 0 /INT0 interrupt flag */
/* TCON.0(   IT0): Timer 0 /INT0 interrupt style setup */
void Init_ExternalInterrupt( void )
{
    //INT0
#if EXT_TIMER0_1MS || ENABLE_MBX
    IT0 = 0; // 0:Level  1: falling edge trigger
    IE0 = 0;
    EX0 = 1; // enable external interrupt 0
#else
    IT0 = 0;
    IE0 = 0;
#if ENABLE_R2_INT_M51_Test
    EX0 = 1;  // enable external interrupt 0
#else
    EX0 = 0;  // disable external interrupt 0
#endif
#endif

    //INT1

#if EXT_TIMER0_1MS
    INT_FIQ_TIMER0_ENABLE(TRUE);
    INT_IRQ_TIMER0_ENABLE(TRUE);

    IT1 = 0;
    IE1 = 0;
    EX1 = 1;
#endif

#if UseINT
    INT_SYNC_CHANGE_TRIGGER_TYPE();//msWriteByteMask( SC0_CA, _BIT1 | _BIT0, _BIT1 | _BIT0); // BIT0 for level trigger, BIT1 for active high

    INT_IRQ_DISP_ENABLE(TRUE); // disp_int

    IT1 = 0;//1; // set external interrupt 0 & 1 as falling edge trigger
    IE1 = 0;  // interrupt flag
    EX1 = 1;  // enable external interrupt 1
#elif ENABLE_SW_DOUBLE_BUFFER
    IT1 = 0;  //1; // set external interrupt 0 & 1 as falling edge trigger
    IE1 = 0;  // interrupt flag
    EX1 = 1;  // enable external interrupt 1
#else
    IT1 = 0;
    IE1 = 0;
    EX1 = 1;//0;
#endif

    PX0=1;  //
    PX1=1;  //

#if ENABLE_DEBUG
    PS=1;
#endif

#if (ENABLE_DEBUG)
    IP0 = _BIT0 | _BIT4;
    IP1 = _BIT0 | _BIT4;
#else
    IP0 = _BIT2;
    IP1 = _BIT0 | _BIT2 | _BIT4;
#endif
}

void Init_Timer( void )
{
    WORD XDATA clock_period = ClockPeriod;

  /* -------------initialize Timer 0 -----------------------------*/
  g_ucTimer0_TH0 = clock_period >> 8;
  g_ucTimer0_TL0 = clock_period & 0xFF;
  ET0=1;  // enable timer 0 interrupt
  TH0 = g_ucTimer0_TH0;
  TL0 = g_ucTimer0_TL0; // timer 0 counter
  TF0=0; // timer 1 flag

 /* -------------initialize Timer 0 -----------------------------*/
  ET1=0;  // disable timer 1 interrupt
  TH1=0;
  TL1=0; // timer 1 counter
  TF1=0; // timer 1 flag

  // setup Timer mode
  // TMOD=0x11;  // set timer 1 as timer(1) , timer 0 as counter(5)
  TMOD=0x21;
  // enable/disable timer
  TR0=1;  // disable timer 0
  TR1=0;  // disable timer 1

#if EXT_TIMER0_1MS
    TR0 = 0;
    msWrite2Byte(REG_002C20, 0x0101);
    msWriteByte(REG_002C24, 0xE0);
    msWriteByte(REG_002C25, 0x2E);
    msWriteByte(REG_002C26, 0x00);
    msWriteByte(REG_002C27, 0x00);
#endif

#if EnableTime1Interrupt
    TR1 = 0;
    ET1 = 0;
    TH1 = g_ucTimer0_TH0;
    TL1 = g_ucTimer0_TL0;
    TMOD=0x11;
    TR1 = 1;
    ET1 = 1;
#endif

}
/*----------Timer 2 -------------------*/
/* T2CON.7(   TF2): overflow flag */
/* T2CON.6(  EXF2): extern enable flag */
/* T2CON.5(  RCLK): receive clock */
/* T2CON.4(  TCLK): transfer clock */
/* T2CON.3( EXEN2): extern enable flag */
/* T2CON.2(   TR2): stop/start timer 2 */
/* T2CON.1(  C_T2): intern clock(0)/extern counter(1) switch */
/* T2CON.0(CP_RL2): capture flag */
void Init_SerialPort( void )
{
#if ENABLE_DEBUG // Uart 0
    ADCON |= _BIT7;             // use S0RELH, S0RELL as baudrate generator
    SCON = 0x50;                // mode 1, 8-bit UART, enable receive
    PCON |= _BIT7;

    // default baudrate-xtal
    S0RELH = HIBYTE( S0REL );
    S0RELL = LOBYTE( S0REL );

    TI = 0;           // clear transfer flag
    RI = 0;
    TI0_FLAG = 0;
    ReSyncCounter = 0;
    SIORxIndex = 0;

    ES = 1;                     // enable uart interrupt
#else
    ES = 0;                     // disable uart interrupt
#endif


#if UART1
    S1CON = (_BIT7 | _BIT4); // Uart 1

    // baudrate
    S1RELH = HIBYTE( S1REL );
    S1RELL = LOBYTE( S1REL );

    S1CON &= ~TI1;    // Uart 1, clear TI1
    S1CON &= ~RI1;    // Uart 1, clear RI1

    TI1_FLAG = 0;
    ReSyncCounter1 = 0;
    SIORxIndex1 = 0;

    IEN2 |= ES1;
#else
    IEN2 &= ~ES1;
#endif
/*
    g_UartCommand.Index = 0;
    g_bUartDetected=0;

    g_bDebugASCIICommandFlag = _DISABLE;
    CLR_DEBUG_ONLY_FLAG();
*/
}

#if 1//ENABLE_HK_DATA_ON_DRAM
void mcuInitXdataMapToDRAM(void)
{
    /*
    Initial XDATA on DRAM.
    Win0: MCU view 0x5000~0xEFFF(40KB)
    Win1: MCU view 0xF000~0xFFFF(4KB)
    */
    msWriteByte(REG_002BC6, WIN0_ADDR_START); // unit is K Byte
    msWriteByte(REG_002BC7, WIN0_ADDR_END);

    msWriteByte(REG_002BCA, WIN1_ADDR_START);
    msWriteByte(REG_002BCB, WIN1_ADDR_END);

    msWriteBit(REG_002BC4, _ENABLE, _BIT2);   //  enable
}

void mcu4kXdataMapToDRAM(DWORD dw4k)
{
    //printf("\r\nWin1: Access DRAM %dth 4KB", w4k);
    msWrite2Byte(REG_002BCC, dw4k);
    msWrite2Byte(REG_002BCE, (dw4k >> 16));
}

void mcu40kXdataMapToDRAM(WORD w64k)
{
    /*
    The low byte address to access xdata from MIU.
    The granularity is 64k bytes.
    The actual address[26:0] to miu would be
    {reg_sdr_xd_map[10:8],reg_sdr_xd_map[7:0],xdata_addr[15:0]},
    where xdata_addr[15:0] is mcu xdata address of 64k bytes.
    reg_sdr_xd_map[10:8]
    */
    // printf("\r\nWin0: Access DRAM %dth 64KB", w64k);
    msWrite2Byte(REG_002BC8, w64k);
}

void mcuXdataMapToDRAM(DWORD dwADDR)
{
    idata WORD Nth64k;
    idata DWORD Nth4k;

    Nth64k = dwADDR >> 16; //unit is 64K Byte
    Nth4k  = (dwADDR + ((DWORD)WIN1_ADDR_START<<10)) >> 12;

    mcu40kXdataMapToDRAM(Nth64k);
    mcu4kXdataMapToDRAM(Nth4k);  // let win0 & win1 start from the same address
}
#endif

//there is a offset add to dwDestiAddr.
void mcuDMADownloadCode(BYTE ch,BYTE DestinaType,DWORD dwSourceAddr, DWORD dwDestinAddr, DWORD dwByteCount)
{
    //BDMA_Operation(ch,0x40|SOURCE_SPI,0x40|DestinaType,dwSourceAddr,dwDestinAddr,dwByteCount,0,0);
    if(DestinaType == DEST_MIU0)
    {
        msWrite2Byte(REG_002B80, ((dwDestinAddr)>>16));
        dwDestinAddr = (dwDestinAddr&0xFFFF0000)+(dwSourceAddr&0xFFFF);
    }

    mcuDMADataTransfer(ch,  SOURCE_SPI, DestinaType, dwSourceAddr, dwDestinAddr, dwByteCount);

}
//------------------------------------------------------------------------------------------------------
// Function Name:  mcuDMADataTransfer
//
//  Description: BDMA do Move Data from source to destination
//
//  Parameter:
//          channel:  can be set to CHANNEL_AUTO / CHANNEL_0 / CHANNEL_1.We can assign BDMA channel by using  CHANNEL_0 and CHANNEL_1
//                           if we set to CHANNEL_AUTO, BDMA_Operation function will auto switch to chnnel1 if channel0 is busy.
//          SourceType: you can choose device source, please refer to BDMA_SOURCE_TYPE structure
//          DestinaType: you can choose destination, please refer to BDMA_DESTINATION_TYPE structure
//          dwSourceAddr: set BDMA  Source Start Address
//          dwDestinAddr: set BDMA destination Address
//          dwByteCount:    set Data size
//
//------------------------------------------------------------------------------------------------------
void mcuDMADataTransfer(BYTE ch,BYTE SourceType,BYTE DestinaType,DWORD dwSourceAddr, DWORD dwDestinAddr, DWORD dwByteCount)
{
    XDATA BYTE width;
    if((SourceType != SOURCE_MIU0) && (SourceType != SOURCE_MIU1) && (SourceType != SOURCE_SPI))
        return;
    if((DestinaType == DEST_MIU0) || (DestinaType == DEST_MIU1))
    {
        width = 0x40;
        if((SourceType == DestinaType) && (dwSourceAddr+dwByteCount > dwDestinAddr))
            msWriteByteMask(REG_100906,_BIT0,_BIT0); //decrease
    }
    else if(DestinaType == DEST_HKMCU)
        width = 0x00;
    else
        return;

    BDMA_Operation(ch,  0x40|SourceType, width|DestinaType, dwSourceAddr, dwDestinAddr, dwByteCount,0 ,0);
}
//------------------------------------------------------------------------------------------------------
//  Function Name:  mcuDMAPatternSearch
//
//  Description: BDMA do Pattern Search
//
//  Parameter:
//          channel:  can be set to CHANNEL_AUTO / CHANNEL_0 / CHANNEL_1.We can assign BDMA channel by using  CHANNEL_0 and CHANNEL_1
//                           if we set to CHANNEL_AUTO, BDMA_Operation function will auto switch to chnnel1 if channel0 is busy.
//          SourceType: you can choose device source, please refer to BDMA_SOURCE_TYPE structure
//          dwSourceAddr:   set Pattern Search Start Address
//          dwByteCount:    set Search size
//          dwPattern:  Enter the pattern we want to search.
//                              ex:  pattern = "0x0000BBCC"
//  Return: address if hit, or 0 if not found or BDMA busy
//
//------------------------------------------------------------------------------------------------------

DWORD mcuDMAPatternSearch(BYTE channel, BYTE SourceType,DWORD dwSourceAddr, DWORD dwByteCount,DWORD dwPattern)
{
    XDATA DWORD dwExternPattern;
    XDATA BYTE index;
    //xdata BYTE uctemp;

    dwExternPattern = 0;
    index = 4;



    while(index)
    {
        index--;
        //MCU_printData("%d",index);
        if(dwPattern>>(index*8))
            break;
        dwExternPattern += ((DWORD)0xFF<<(index*8));

    }

    dwSourceAddr = BDMA_Operation(channel, 0x40|SourceType, DEST_PATTERN_SEARCH, dwSourceAddr, dwSourceAddr, dwByteCount, dwPattern, dwExternPattern);
    dwSourceAddr = dwSourceAddr-index+2;

    return dwSourceAddr;

}
//------------------------------------------------------------------------------------------------------
// Function Name:  mcuDMAMemoryFill
//
//  Description: BDMA do Memory fill
//
// Parameter:
//          channel:  can be set to CHANNEL_AUTO / CHANNEL_0 / CHANNEL_1.We can assign BDMA channel by using  CHANNEL_0 and CHANNEL_1
//                           if we set to CHANNEL_AUTO, BDMA_Operation function will auto switch to chnnel1 if channel0 is busy.
//          DestinaType: you can choose destination device, please refer to BDMA_DESTINATION_TYPE structure
//          dwSourceAddr:   set Memory Fill Start Address
//          dwByteCount:    set Fill pattern size
//          dwPattern:  Filled pattern
//------------------------------------------------------------------------------------------------------
void mcuDMAMemoryFill(BYTE channel,BYTE DestinaType,DWORD dwSourceAddr, DWORD dwByteCount, DWORD dwPattern)
{
    BDMA_Operation(channel, 0x20|SOURCE_MEMORY_FILL, 0x40|DestinaType, 0, dwSourceAddr, dwByteCount, dwPattern, 0);
}
//------------------------------------------------------------------------------------------------------
// Function Name:  mcuDMACRC32
//
//  Description: BDMA do check CRC value
//
// Parameter:
//          channel:  can be set to CHANNEL_AUTO / CHANNEL_0 / CHANNEL_1.We can assign BDMA channel by using  CHANNEL_0 and CHANNEL_1
//                           if we set to CHANNEL_AUTO, BDMA_Operation function will auto switch to chnnel1 if channel0 is busy.
//          SourceType: you can choose device source, please refer to BDMA_SOURCE_TYPE structure
//          dwSourceAddr:   set Check CRC Start Address
//          dwByteCount:    set Check CRC  size
//          dwPolynimial: set Polynomial
//          dwSeed: set seed
//          dwPattern:  return 4 byte value
//
//------------------------------------------------------------------------------------------------------
DWORD mcuDMACRC32(BYTE channel,BYTE SourceType,DWORD dwSourceAddr, DWORD dwByteCount, DWORD dwPolynimial, DWORD dwSeed)
{
    return BDMA_Operation(channel, 0x40|SourceType, DEST_CRC32, dwSourceAddr, dwSourceAddr, dwByteCount,dwPolynimial, dwSeed);
}

DWORD BDMA_Operation(BYTE ch, BYTE SourceId, BYTE DestinId,DWORD dwSourceAddr, DWORD dwDestinAddr, DWORD dwByteCount, DWORD dwPattern, DWORD dwExternPattern)
{
    xdata BYTE uctemp;
    if(ch == CHANNEL_AUTO)
    {
        if(!(msReadByte(REG_100902)&_BIT1))
            uctemp = 0; // channel 0
        else if(!(msReadByte(REG_100902+0x20)&_BIT1))
            uctemp = 0x20; // channel 1
        else
    {
            MCU_printMsg("Both Ch0 and Ch1 are busy");
            return 0; // both busy
        }
    }
    else if(ch ==CHANNEL_0)
    {
        if(msReadByte(REG_100902)&_BIT1)
    {
            MCU_printMsg("Ch0 is busy");
            return 0;
    }
        uctemp = 0;
    }
    else
    {
        if(msReadByte(REG_100902+0x20)&_BIT1)
    {
            MCU_printMsg("Ch1 is busy");
            return 0;
        }
        uctemp = 0x20;
    }

    msWriteByte(REG_100904+uctemp,SourceId);
    msWriteByte(REG_100905+uctemp,DestinId);

    msWriteByteMask(REG_100902+uctemp,_BIT4|_BIT3,_BIT4|_BIT3); // clear  bdma_done and pattern search result flag

    msWrite4Byte(REG_100908+uctemp, dwSourceAddr);
    msWrite4Byte(REG_10090C+uctemp, dwDestinAddr);
    msWrite4Byte(REG_100910+uctemp, dwByteCount);

    msWriteByte(REG_100914+uctemp,(BYTE)(dwPattern>>24));
    msWriteByte(REG_100915+uctemp,(BYTE)(dwPattern>>16));
    msWriteByte(REG_100916+uctemp,(BYTE)(dwPattern>>8));
    msWriteByte(REG_100917+uctemp,(BYTE)(dwPattern));

    msWriteByte(REG_100918+uctemp,(BYTE)(dwExternPattern>>24));
    msWriteByte(REG_100919+uctemp,(BYTE)(dwExternPattern>>16));
    msWriteByte(REG_10091A+uctemp,(BYTE)(dwExternPattern>>8));
    msWriteByte(REG_10091B+uctemp,(BYTE)(dwExternPattern));

    msWriteBit(REG_100900+uctemp, 1, _BIT0);    //trigger

    if((DestinId&0x0F) == DEST_PATTERN_SEARCH) // pattern search
    {
        TimeOutCounter=10;
        while( TimeOutCounter && !( msReadByte(REG_100902+uctemp)&_BIT3 ) && !( msReadByte(REG_100902+uctemp)&_BIT4) );
        if(TimeOutCounter == 0) MCU_printMsg("TIMEOUT!!");

        if(!( msReadByte(REG_100902+uctemp)&_BIT4))
        {
            MCU_printMsg("Can't find the specific pattern in this range!!!");
            return 0;
        }

        dwDestinAddr = 0;
        dwDestinAddr = (DWORD)msReadByte(REG_100908+uctemp);
        dwDestinAddr |= (DWORD)(msReadByte(REG_100909+uctemp))<<8;
        dwDestinAddr |= (DWORD)(msReadByte(REG_10090A+uctemp))<<16;
        dwDestinAddr |= ((DWORD)msReadByte(REG_10090B+uctemp))<<24;

        MCU_printData("BDMA_OP_dwDestinAddr_H: 0x%x",dwDestinAddr>>16);
        MCU_printData("BDMA_OP_dwDestinAddr_L: 0x%x",dwDestinAddr);
        return dwDestinAddr;
    }
    else if((DestinId&0x0F) == DEST_CRC32)
    {
        TimeOutCounter=10;
        while( TimeOutCounter && !( msReadByte(REG_100902+uctemp)&_BIT3 ) );
        if(TimeOutCounter == 0) MCU_printMsg("TIMEOUT!!");
        dwDestinAddr = 0;
        dwDestinAddr = (DWORD)msReadByte(REG_100918+uctemp);
        dwDestinAddr |= (DWORD)(msReadByte(REG_100919+uctemp))<<8;
        dwDestinAddr |= (DWORD)(msReadByte(REG_10091A+uctemp))<<16;
        dwDestinAddr |= ((DWORD)msReadByte(REG_10091B+uctemp))<<24;

        MCU_printData("BDMA_OP_dwDestinAddr_H: 0x%x",dwDestinAddr>>16);
        MCU_printData("BDMA_OP_dwDestinAddr_L: 0x%x",dwDestinAddr);
        return dwDestinAddr;

    }
    else // pattern search and  memory fill
    {
        TimeOutCounter=10;
        while( TimeOutCounter && !( msReadByte(REG_100902+uctemp)&_BIT3 ) );
        if(TimeOutCounter == 0) MCU_printMsg("TIMEOUT!!");
    }
    return 0;


}
void mcuArrangeCodeAddr(BYTE ucType, DWORD dwAddrStart, DWORD dwAddrEnd)
{
    XDATA BYTE ucTemp;

    if (ucType ==_SPI)
        ucTemp = 16;
    else if(ucType == _DRAM)
        ucTemp = 8;
    else if(ucType == _PSRAM)
        ucTemp = 0;

    msWriteByte(0x001004+ucTemp, (BYTE)(dwAddrStart)); // 0x1014
    msWriteByte(0x001005+ucTemp, (BYTE)(dwAddrStart>>8)); // 0x1015
    msWriteByte(0x001000+ucTemp, (BYTE)(dwAddrStart>>16)); // 0x1010
    msWriteByte(0x001001+ucTemp, (BYTE)(dwAddrStart>>24)); // 0x1011

    msWriteByte(0x001006+ucTemp, (BYTE)(dwAddrEnd)); // 0x1016
    msWriteByte(0x001007+ucTemp, (BYTE)(dwAddrEnd>>8)); // 0x1017
    msWriteByte(0x001002+ucTemp, (BYTE)(dwAddrEnd>>16)); // 0x1012
    msWriteByte(0x001003+ucTemp, (BYTE)(dwAddrEnd>>24)); // 0x1013


    if (ucType ==_SPI)
        ucTemp = _BIT1;
    else if(ucType == _DRAM)
        ucTemp = _BIT2;
    else if(ucType == _PSRAM)
        ucTemp = _BIT0;

    if (dwAddrStart < dwAddrEnd)
        msWriteBit(REG_001018, _ENABLE, ucTemp);
    else
        msWriteBit(REG_001018, _DISABLE, ucTemp);
}

void mcuXdataMapToSRAM(BYTE lbAddr, BYTE hbAddr)
{
    msWriteByte(REG_0010E0, hbAddr);
    msWriteByte(REG_0010E1, lbAddr);
    msWriteByte(REG_0010E4, lbAddr);

    msWriteBit(REG_0010E6, _ENABLE, _BIT3);
    msWriteBit(REG_0010E6, _ENABLE, _BIT4);
}
void codeMoveToXdata(DWORD FStartAddr, DWORD XStartAddr, DWORD XCOUNT)
{
    DWORD idata i;
    P2 = 0;
    for(i=0; i<XCOUNT; i++)
    {
        msRegs[XStartAddr+i]=msCodeArea[FStartAddr+i];
    }
    for(i=0; i<XCOUNT; i++)
    {
        if(msCodeArea[FStartAddr+i] != msRegs[XStartAddr+i])
        {
            MCU_printData("FLASH to XDATA wrong address i = %x",i);
        }
    }
}

#if ENABLE_DRAM_SELFREFRESH
void MIU_EnterSelfRefresh(void)
{
    msWrite2Byte(0x1012E0, 0x0000);

//*************************************
#if CHIP_ID == MST9U3
    //Enter_self_refresh.txt
    ForceDelay1ms(1);
	msWrite2ByteMask(0x101206, BIT12, BIT12);
    msWrite2Byte(0x101246, 0xfffe); //mask other request => reg_rq0_mask(save path "0" for miu_cmd)
    msWrite2Byte(0x101266, 0xffff); //mask other request => reg_rq1_mask
    msWrite2Byte(0x101286, 0xffff); //mask other request => reg_rq2_mask
    msWrite2Byte(0x1012a6, 0xffff); //mask other request => reg_rq3_mask
    ForceDelay1ms(1);
    msWrite2Byte(0x101218, 0x0400);
    msWrite2Byte(0x101200, 0x002f);
    msWrite2Byte(0x101200, 0x052f);
    msWrite2Byte(0x101200, 0x002f);
    msWrite2Byte(0x101200, 0x032f);
    msWrite2Byte(0x101200, 0x002f);
    ForceDelay1ms(1);
    msWrite2Byte(0x101246, 0xffff);
    msWrite2Byte(0x101200, 0x202f);
    ForceDelay1ms(1);
    //msWriteByteMask(0x101203, 0xF0, 0xF0);

    //AN_PowerDown.txt
    msWriteByteMask(0x101203, 0xF0, 0xF0);
    msWriteByteMask(0x101100, 0x08, 0x08);
    msWriteByteMask(0x101180, 0x08, 0x08);
    msWriteByteMask(0x101100, 0x18, 0x18);
    msWriteByteMask(0x101180, 0x18, 0x18);
    msWrite2Byte(0x101154, 0xC070);
    msWrite2Byte(0x1011D4, 0xC070);
    msWrite2Byte(0x101108, 0x0000);
    msWrite2Byte(0x101188, 0x0000);
#else
//*************************************
//*Enter SELF REFRESH
//*Set cko_oenz,adr_oenz,dq_oenz,cke_oenz as 1'b1
//*Store RX dqs phase value
//*Set gpio_oenz as 1'b1
//*Set gpio_mode as 1'b1
//*Set gpio_pad_mode as 1'b1
//*PM_power_down2standby
//*
//*(Set gpio_pad_mode as 1'b1§ï¦¨DDR_TEST_A[7:3] as 1'b1)
//*
//**************************************
    msWrite2Byte(0x101246, 0xfffe); //mask other request => reg_rq0_mask(save path "0" for miu_cmd)
    msWrite2Byte(0x101266, 0xffff); //mask other request => reg_rq1_mask
    msWrite2Byte(0x101286, 0xffff); //mask other request => reg_rq2_mask
    msWrite2Byte(0x1012a6, 0xffff); //mask other request => reg_rq3_mask
    //wait 1 //delay 1ms
    ForceDelay1ms(1);

    msWrite2Byte(0x101218, 0x0400); //tie A10=1 to enter precharge all
                           //reg_mrx = address in miu_cmd
    msWrite2Byte(0x101200, 0x002f); //turn off auto refresh
                           //reg_auto_ref_off                      1 : [5]
    msWrite2Byte(0x101200, 0x052f); //trigger precharge all
                           //single cmd [9:7] = {wez, casz, rasz} = {0, 1, 0}
                           //reg_single_cmd_en                     1 : [8]
    msWrite2Byte(0x101200, 0x002f);
    msWrite2Byte(0x101200, 0x032f); //trigger refresh
                           //single cmd [9:7] = {wez, casz, rasz} = {0, 0, 1}
                           //reg_single_cmd_en                     1 : [8]
    msWrite2Byte(0x101200, 0x002f);
    msWrite2ByteMask(0x101206, BIT12, BIT12); //csz cke always on
                           //reg_csz_always_on                     1 : [11] (ECO method)
                           //reg_cke_always_on                     1 : [12]
    //wait 1 //delay 1ms
    ForceDelay1ms(1);

    msWrite2Byte(0x101246, 0xffff); //turn off command request
    msWrite2Byte(0x101200, 0x202f); //Enter self refresh mode
                           //reg_self_refresh                      1 : [13]
//********************
//Enter Self-Refresh!!!
//********************
//In self-refresh mode, reg_cke_oenz, reg_cs_oenz should be "0"
//for DDR3, RESET OENZ and cke OENZ are controlled by cke_oenz
    msWrite2ByteMask(0x101202, BIT15|BIT14|BIT13|BIT12, BIT15|BIT14|BIT13|BIT12); //turn on oenz

                           //reg_cke_oenz                          1 : [12]
                           //reg_dq_oenz                           1 : [13]
                           //reg_adr_oenz                          1 : [14]
                           //reg_cko_oenz                          1 : [15]
    msWrite2ByteMask(0x101204, BIT11|BIT10, BIT11|BIT10); //reg_cs_oenz                           1 : [10]
                           //reg_cs1_oenz                          1 : [11]


    //Store RX dqs phase value
    //Saved_RX_DQS[0] = msRead2Byte(0x10116c);
    //Saved_RX_DQS[1] = msRead2Byte(0x10116e);



    msWrite2ByteMask(0x101100, BIT3, BIT3); //[ 0: 0] reg_pkg_sel
                           //[ 3: 3] reg_gpio_oenz                 1
                           //[ 4: 4] reg_gpio_mode                 0
                           //[ 5: 5] reg_mclk_pd(this bit no use)
    //wait 1 //delay 1ms
    ForceDelay1ms(1);

    msWrite2ByteMask(0x101100, BIT4, BIT4); //[ 0: 0] reg_pkg_sel

                           //[ 3: 3] reg_gpio_oenz                 1
                           //[ 4: 4] reg_gpio_mode                 1
                           //[ 5: 5] reg_mclk_pd(this bit no use)

    msWrite2Byte(0x10117e, 0x00fc); //[15: 0] reg_ddr_test                  1: [7:3] (cortrol gpio mode), [2] for MST9U2, no loading in MST9U1
    //wait 1 //delay 1ms
    ForceDelay1ms(1);
#endif
}

Bool MIU_SelfRefreshGuard( void )
{
#define BIST_SIZE       0x200000
#define BIST_BASE_ADDR  0x800000

#if (CHIP_ID == MST9U3)
#define BIST_BASE_UNIT      0x4000
#define MIU_UNIT            0x20
#define MIU_GROUP0_MASK     0xFFFE
#define MIU_GROUP2_MASK     0x7FFF
#else
#define BIST_BASE_UNIT      0x2000
#define MIU_UNIT            0x10
#define MIU_GROUP0_MASK     0x7FFE
#define MIU_GROUP2_MASK     0xFFFF
#endif

    BYTE u8LoopMode  = 0;
    BYTE u8ReadOnly  = 0;
    BYTE u8WriteOnly = 1;

    WORD i, bist_try;
    WORD u16TestMode, u16ExtraMode;
    Bool bReturn = FALSE;


    bist_try = 5;

    u16TestMode  = (BIT2|BIT1); //BIST test mode [2:1]
    u16ExtraMode = u16TestMode;

    if (u8LoopMode)
        u16ExtraMode |= BIT4;
    if (u8ReadOnly)
        u16ExtraMode |= BIT8;
    if (u8WriteOnly)
        u16ExtraMode |= BIT9;

    msWrite2Byte(0x101246, MIU_GROUP0_MASK); //mask other request
    msWrite2Byte(0x101266, 0xffff); //mask other request
    msWrite2Byte(0x101286, MIU_GROUP2_MASK); //mask other request
    msWrite2Byte(0x1012a6, 0xffff); //mask other request

    //DQS reset
    msWrite2Byte(0x10110e, msRead2Byte(0x10110e)|BIT6);
    msWrite2Byte(0x10110e, msRead2Byte(0x10110e)&~BIT6);
//-------------
//miu self test
//-------------
    msWrite2Byte(0x1012e0, 0x0000);
#if (CHIP_ID == MST9U3)
    msWriteByteMask(0x101203, 0x00, 0x08);
#else
    msWrite2Byte(0x1012F0, 0x0000);
#endif

    msWrite2Byte(0x1012e2, BIST_BASE_ADDR/BIST_BASE_UNIT); //test base address
    msWrite4Byte(0x1012e4, BIST_SIZE/MIU_UNIT);          //test length
    msWrite2Byte(0x1012e8, 0xff00);                        //test pattern

    msWrite2Byte(0x1012e0, u16ExtraMode);
	ForceDelay1ms(2);
    msWrite2Byte(0x1012e0, u16ExtraMode|BIT0);
    for (i=0; i<bist_try; i++)
    {
        while(!(msRead2Byte(0x1012e0)&BIT15));

        if (!(msRead2Byte(0x1012e0)&0x6000))
        {
            bReturn = TRUE;
            break;
        }
        msWrite2Byte(0x1012e0, u16ExtraMode);
		ForceDelay1ms(2);
        msWrite2Byte(0x1012e0, u16ExtraMode|BIT0);
    }
    if (i >= bist_try)
        bReturn = FALSE;
#if 0 // avoid the guard data overwriten by other clients
    msWrite2Byte(0x101246, 0x0000); //unmask other request
    msWrite2Byte(0x101266, 0x0000); //unmask other request
    msWrite2Byte(0x101286, 0x0000); //unmask other request
    msWrite2Byte(0x1012a6, 0x0000); //unmask other request
#endif
    return bReturn;
}

void DeInit_Dram(void)
{
    if(g_bSTRStatus == 0)
    {
        MCU_printMsg("DeInit Dram SR (MIU_EnterSelfRefresh)!");
        MIU_SelfRefreshGuard();
        MIU_EnterSelfRefresh();
        MCU_printMsg("MIU_EnterSelfRefresh2!");
        g_bSTRStatus = 1;
    }
}

void Init_Dram(void)
{
    g_bSTRStatus = 0;
#if 1
    // for ddr self-refresh init sequence.
    MCU_printMsg("Init Dram SR (TBD)!");
#else
    msWrite2Byte(0x101202, 0x0818);
    msWrite2Byte(0x101204, 0x01c5);
    msWrite2Byte(0x101206, 0x0120);
    msWrite2Byte(0x101208, 0x0833);
    msWrite2Byte(0x10120a, 0x0c33);
    msWrite2Byte(0x10120c, 0x7111);
    msWrite2Byte(0x10120e, 0x100e);
    msWrite2Byte(0x101210, 0x0031);
    msWrite2Byte(0x10121e, 0x1a08);

    msWrite2Byte(0x10122a, 0x400f);

    msWrite2Byte(0x101200, 0x0000);
    msWrite2Byte(0x101200, 0x0008);
    msWrite2Byte(0x101200, 0x000c);
    msWrite2Byte(0x101200, 0x000e);
    msWrite2Byte(0x101200, 0x001f);
// ===== miu self test =====
    msWrite2Byte(0x1012E2, 0x0000);
    msWrite2Byte(0x1012E4, 0x0021);
    msWrite2Byte(0x1012E6, 0x0000);
    msWrite2Byte(0x1012E8, 0x5aa5);
    msWrite2Byte(0x1012E0, 0x0001);
    MCU_printMsg("Init Dram ! ");
#endif
}
#endif

void msDrvMcu51ResetR2(void)
{
    msWriteByteMask(REG_00039D, 0, BIT6);
    msWriteByte(REG_000393, 0x3F);
    msWriteByteMask(REG_00038A, BIT6, BIT7|BIT6);
    g_bR2Status = 0;
}

void msDrvMcu51ToR2(void)
{
    MCU_printMsg("51 -> R2");

    #if (!R2_51_DUAL_MODE)
    CACHE_DISABLE();
    EA = 0;
    #endif

    msWrite2Byte(REG_100FB4, 0x00);  //reset base
    msWrite2Byte(REG_100F80, 0x00);
    msWrite2Byte(REG_100F80, 0x1F); //SPI boot

    msWriteByte(REG_000393, 0x5F); // to do, check flow if RST pin using 5V standby

    //CPU1 SW reset control  (CPU1: R2, CPU0: 51)
    msWriteByteMask(REG_00038A, BIT7, BIT7);    //sw reset

    g_bR2Status = 1;

    #if (!R2_51_DUAL_MODE)
    EA = 0;
    while(1);
    #endif
}

void msDrvMcuMailBoxRead(void)
{
    BYTE i;
    BYTE *pu8Tmp    = (BYTE *)(&g_sMailBoxR2);
    BYTE u8MBSize   = sizeof(sMAILBOX_R2);

    if(IsMailBoxValid())
    {
        for( i=0 ; i<u8MBSize ; i+=2 )
        {
            *(pu8Tmp+i+1) = msReadByte(REG_103380+i);
            *(pu8Tmp+i)   = msReadByte(REG_103380+i+1);
        }
    }

}

void msDrvMcuMailBoxWrite(void)
{
    BYTE i;
    BYTE *pu8Tmp    = (BYTE *)(&g_sMailBox51);
    BYTE u8MBSize   = sizeof(sMAILBOX_51);

    msDrvMcuMailBoxClear();

    for( i=0 ; i<u8MBSize ; i+=2 )
    {
        msWriteByte(REG_103380+i+1, *(pu8Tmp+i));
        msWriteByte(REG_103380+i  , *(pu8Tmp+i+1));
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

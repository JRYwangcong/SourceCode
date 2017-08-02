///////////////////////////////////////
// DRVDDC2BI_MST9U VERSION: V02
////////////////////////////////////////
#include "Global.h"
#include "UserPref.h"
#include "AutoFunc.h"

#if ENABLE_DP_INPUT
#include "drvDPRxApp.h"
#endif
#if ENABLE_DAISY_CHAIN
#include "msDaisyChain.h"
#endif

#define DRVDDC2BI_DEBUG    1
#if ENABLE_MSTV_UART_DEBUG && DRVDDC2BI_DEBUG
#define DRVDDC2BI_PRINT(format, ...)     printf(format, ##__VA_ARGS__)
#else
#define DRVDDC2BI_PRINT(format, ...)
#endif

// Swap D2 & D3 in MST9U3 due to the design order is different from MST9U1/2
#if CHIP_ID == MST9U3
#if (TMDS_Port_D2) && (!TMDS_Port_D3)
  #undef  TMDS_Port_D2
  #undef  TMDS_Port_D3
  #define TMDS_Port_D2  0
  #define TMDS_Port_D3  1
#elif (TMDS_Port_D3) && (!TMDS_Port_D2)
  #undef  TMDS_Port_D2
  #undef  TMDS_Port_D3
  #define TMDS_Port_D2  1
  #define TMDS_Port_D3  0
#endif
#endif

#if ENABLE_DP_INPUT
#if DPENABLEMCCS
BYTE idata ucfinal_status,g_ucDDC_TxRecord ;  // use idata to speedup
WORD idata DDC2BI_INT_STS, DDC2BI_R_BUF, DDC2BI_W_BUF, DDC2BI_INT_CLR;
code BYTE dpNullMsg[3]={0x6e, 0x80, 0xbe};
//code BYTE dpNullMsg[7]={0x77, 0x66, 0x55,0x44,0x33,0x22,0x11};
 BYTE u8DDC_RxRecord = 0;
 BYTE AuxIndex=0;

#endif
#endif


volatile BYTE idata rxInputPort; // Annie 2006.7.5
volatile BYTE idata rxComboPort; // Combo port index
volatile BYTE rxIndex = 0;
volatile BYTE rxStatus;
volatile BYTE rxLength = 0;
#if !D2B_HW_CheckSum
volatile BYTE rxChkSum = 0;
#endif
volatile BYTE txStatus;
volatile BYTE *txBufferPtr;
volatile BYTE txLength;
//BYTE sendData;
//=====================================
volatile BYTE xdata u8ButtonFIFO[FIFO_SIZE];
volatile BYTE xdata u8ButtonFIFOIndex;
volatile BYTE xdata g_u8DDCCISaveCounter=0;
volatile Bool xdata MccsEnable;
//=====================================
code BYTE nullMsg1[3] =
{
    0x6e, 0x80, 0xbe
};
code BYTE nullMsg[3] =
{
    0x51, 0x80, 0xbe
};

#if D2B_XShared_DDCBuffer
volatile BYTE *DDCBuffer;
#else
volatile BYTE xdata DDCBuffer[DDC_BUFFER_LENGTH];
#endif

#if D2B_FIFO_Mode
volatile WORD idata txIndex;
volatile WORD idata Previous_DDC2MCU_CUR_ADR;
volatile WORD idata Present_DDC2MCU_CUR_ADR;
volatile WORD idata Present_DDC2MCU_START_ADR;
volatile BYTE idata D2BErrorFlag;
#if D2B_XDATA_DEBUG
#if !D2B_R2 // 51
BYTE xdata XSRAMBuffer[D2B_FIFO_XdataSize+256] _at_ XDATA_DDC_ADDR_START;
#endif
volatile BYTE xdata IntCount;
volatile BYTE xdata IntCountTmp;
#else
#if !D2B_R2 // 51
BYTE xdata XSRAMBuffer[D2B_FIFO_XdataSize] _at_ XDATA_DDC_ADDR_START;
#endif
#endif
#endif // end of #if D2B_FIFO_Mode

extern void Osd_DrawNum(BYTE u8XPos, BYTE u8YPos, int s16Value);
extern void Osd_Draw5Num(BYTE u8XPos, BYTE u8YPos, WORD u16Value);
extern void Menu_Show(void);
#if HKC_ColorTemp_Adjust
extern BYTE HKCAlignControl(void);
extern bit ubAutoAlign;
#endif


BYTE D2B_FIFOMODE_DDCPORT2INPUT(BYTE ComboPort)
{
    BYTE rtn=0;
#if ENABLE_VGA_INPUT
    if(ComboPort==InputCombo_A0)
    {
        rtn = Input_Analog1;
    }
    else
#endif
    {
#if ENABLE_VGA_INPUT
        for(rtn=1;rtn<Input_Nums;rtn++)
#else
        for(rtn=0;rtn<Input_Nums;rtn++)
#endif
        {
		#if CHIP_ID==MST9U3
            if( (ComboPort==InputCombo_D2)  && (BYTE)g_InputPort[rtn].eIPMux == (BYTE)MUX_COMBO3)  //0x03
            {
                break;
            }
            else if((ComboPort==InputCombo_D3) && (BYTE)g_InputPort[rtn].eIPMux == (BYTE)MUX_COMBO2)  //0x02
            {
                break;
            }
            //else if((BYTE)g_InputPort[rtn].eIPMux == (ComboPort+1))
            else if(((BYTE)g_InputPort[rtn].eIPMux == (ComboPort+1)) && (ComboPort!=InputCombo_D2)&&(ComboPort!=InputCombo_D3))
            {
                break;
            }
		#else
            if((BYTE)g_InputPort[rtn].eIPMux == (ComboPort+1))
            {
                break;
            }
		#endif
        }
    }
    return rtn;
}
#if D2B_FIFO_Mode
BYTE DDC2Bi_FIFOMODE_READ_CUR(BYTE InputPort)
{
    BYTE CurValue=0;

    if(InputPort==InputCombo_A0) // A0
        CurValue = msRegs(REG_003EE4);
#if TMDS_Port_D0
    else if(InputPort==InputCombo_D0) // D0
        CurValue = msRegs(REG_003EC6);
#endif
#if TMDS_Port_D1
    else if(InputPort==InputCombo_D1) // D1
        CurValue = msRegs(REG_003ECC);
#endif
#if TMDS_Port_D2
    else if(InputPort==InputCombo_D2) // D2
        CurValue = msRegs(REG_003ED2);
#endif
#if TMDS_Port_D3
    else if(InputPort==InputCombo_D3) // D3
#if (CHIP_ID==MST9U3)
        CurValue = msRegs(REG_003EBA);
#else
        CurValue = msRegs(REG_003FC6);
#endif
#endif
#if TMDS_Port_D4
    else if(InputPort==InputCombo_D4) // D4
        CurValue = msRegs(REG_003FCC);
#endif
#if TMDS_Port_D5
    else //if(InputPort==InputCombo_D5) // D5
        CurValue = msRegs(REG_003FD2);
#endif
    return CurValue;
}
#if 1// prevent from R2 accrss current twice, causes DDC2Bi internal condition checking error
WORD DDC2Bi_FIFOMODE_READ_CUR_START(BYTE InputPort)
{
    WORD CurStartValue=0;

    if(InputPort==InputCombo_A0) // A0
        CurStartValue = ((unsigned short volatile *) RIU_MAP)[REG_003EE4];
#if TMDS_Port_D0
    else if(InputPort==InputCombo_D0) // D0
        CurStartValue = ((unsigned short volatile *) RIU_MAP)[REG_003EC6];
#endif
#if TMDS_Port_D1
    else if(InputPort==InputCombo_D1) // D1
        CurStartValue = ((unsigned short volatile *) RIU_MAP)[REG_003ECC];
#endif
#if TMDS_Port_D2
    else if(InputPort==InputCombo_D2) // D2
        CurStartValue = ((unsigned short volatile *) RIU_MAP)[REG_003ED2];
#endif
#if TMDS_Port_D3
    else if(InputPort==InputCombo_D3) // D3
#if (CHIP_ID==MST9U3)
        CurStartValue = ((unsigned short volatile *) RIU_MAP)[REG_003EBA];
#else
        CurStartValue = ((unsigned short volatile *) RIU_MAP)[REG_003FC6];
#endif
#endif
#if TMDS_Port_D4
    else if(InputPort==InputCombo_D4) // D4
        CurStartValue = ((unsigned short volatile *) RIU_MAP)[REG_003FCC];
#endif
#if TMDS_Port_D5
    else //if(InputPort==InputCombo_D5) // D5
        CurStartValue = ((unsigned short volatile *) RIU_MAP)[REG_003FD2];
#endif
    return CurStartValue;
}
#else
BYTE DDC2Bi_FIFOMODE_READ_START(BYTE InputComboPort)
{
    BYTE StartValue=0;

    if(InputComboPort==InputCombo_A0) // A0
        StartValue = msRegs(REG_003EE5);
#if TMDS_Port_D0
    else if(InputComboPort==InputCombo_D0) // D0
        StartValue = msRegs(REG_003EC7);
#endif
#if TMDS_Port_D1
    else if(InputComboPort==InputCombo_D1) // D1
        StartValue = msRegs(REG_003ECD);
#endif
#if TMDS_Port_D2
    else if(InputComboPort==InputCombo_D2)// D2
        StartValue = msRegs(REG_003ED3);
#endif
#if TMDS_Port_D3
    else if(InputComboPort==InputCombo_D3) // D3
#if D2B_DOUBLE_FIFO_Mode
        StartValue = msRegs(REG_003EBB);
#else
        StartValue = msRegs(REG_003FC7);
#endif
#endif
#if TMDS_Port_D4
    else if(InputComboPort==InputCombo_D4) // D4
        StartValue = msRegs(REG_003FCD);
#endif
#if TMDS_Port_D5
    else //if(InputComboPort==InputCombo_D5)// D5
        StartValue = msRegs(REG_003FD3);
#endif
    return StartValue;
}
#endif
WORD DDC2Bi_FIFOMODE_XDATA_ADDR_OFFSET(BYTE InputComboPort, BYTE u8TXBuffer)
{
    WORD XdataAddrStart=0;

#if !D2B_DOUBLE_FIFO_Mode
    u8TXBuffer = 0;
#endif

    if(InputComboPort==InputCombo_A0) // A0
        XdataAddrStart = u8TXBuffer?D2B_FIFO_XdataWriteAddressStart_A0:D2B_FIFO_XdataAddressStart_A0;
#if TMDS_Port_D0
    else if(InputComboPort==InputCombo_D0) // D0
        XdataAddrStart = u8TXBuffer?D2B_FIFO_XdataWriteAddressStart_D0:D2B_FIFO_XdataAddressStart_D0;
#endif
#if TMDS_Port_D1
    else if(InputComboPort==InputCombo_D1) // D1
        XdataAddrStart = u8TXBuffer?D2B_FIFO_XdataWriteAddressStart_D1:D2B_FIFO_XdataAddressStart_D1;
#endif
#if TMDS_Port_D2
    else if(InputComboPort==InputCombo_D2) // D2
        XdataAddrStart = u8TXBuffer?D2B_FIFO_XdataWriteAddressStart_D2:D2B_FIFO_XdataAddressStart_D2;
#endif
#if TMDS_Port_D3
    else if(InputComboPort==InputCombo_D3) // D3
        XdataAddrStart = u8TXBuffer?D2B_FIFO_XdataWriteAddressStart_D3:D2B_FIFO_XdataAddressStart_D3;
#endif
#if TMDS_Port_D4
    else if(InputComboPort==InputCombo_D4) // D4
        XdataAddrStart = D2B_FIFO_XdataAddressStart_D4;
#endif
#if TMDS_Port_D5
    else //if(InputComboPort==InputCombo_D5) // D5
        XdataAddrStart = D2B_FIFO_XdataAddressStart_D5;
#endif
    return XdataAddrStart-D2B_FIFO_XdataAddressStart;
}

void DDC2Bi_FIFOMODE_SET_RW_DONE(BYTE InputComboPort)
{
    if(InputComboPort==InputCombo_A0) // A0
        msRegs(REG_003EE6) |= (BIT3);
#if TMDS_Port_D0
    else if(InputComboPort==InputCombo_D0) // D0
        msRegs(REG_003EC8) |= (BIT3);
#endif
#if TMDS_Port_D1
    else if(InputComboPort==InputCombo_D1) // D1
        msRegs(REG_003ECE) |= (BIT3);
#endif
#if TMDS_Port_D2
    else if(InputComboPort==InputCombo_D2) // D2
        msRegs(REG_003EE0) |= (BIT3);
#endif
#if TMDS_Port_D3
    else if(InputComboPort==InputCombo_D3) // D3
#if (CHIP_ID==MST9U3)
        msRegs(REG_003EBC) |= (BIT3);
#else
        msRegs(REG_003FC8) |= (BIT3);
#endif
#endif
#if TMDS_Port_D4
    else if(InputComboPort==InputCombo_D4) // D4
        msRegs(REG_003FCE) |= (BIT3);
#endif
#if TMDS_Port_D5
    else //if(InputComboPort==InputCombo_D5) // D5
        msRegs(REG_003FE0) |= (BIT3);
#endif
}

void DDC2Bi_FIFOMODE_CLR_EMPTYFULL_FLAG(BYTE InputComboPort)
{
    if(InputComboPort==InputCombo_A0) // A0
        msRegs(REG_003EE6) |= (BIT5|BIT4);
#if TMDS_Port_D0
    else if(InputComboPort==InputCombo_D0) // D0
        msRegs(REG_003EC8) |= (BIT5|BIT4);
#endif
#if TMDS_Port_D1
    else if(InputComboPort==InputCombo_D1) // D1
        msRegs(REG_003ECE) |= (BIT5|BIT4);
#endif
#if TMDS_Port_D2
    else if(InputComboPort==InputCombo_D2) // D2
        msRegs(REG_003EE0) |= (BIT5|BIT4);
#endif
#if TMDS_Port_D3
    else if(InputComboPort==InputCombo_D3) // D3
#if (CHIP_ID==MST9U3)
        msRegs(REG_003EBC) |= (BIT5|BIT4);
#else
        msRegs(REG_003FC8) |= (BIT5|BIT4);
#endif
#endif
#if TMDS_Port_D4
    else if(InputComboPort==InputCombo_D4) // D4
        msRegs(REG_003FCE) |= (BIT5|BIT4);
#endif
#if TMDS_Port_D5
    else //if(InputComboPort==InputCombo_D5) // D5
        msRegs(REG_003FE0) |= (BIT5|BIT4);
#endif
}

BYTE DDC2Bi_FIFOMODE_READ_FULL_FLAG(BYTE InputComboPort)
{
    BYTE FlagValue=0;

    if(InputComboPort==InputCombo_A0) // A0
        FlagValue = msRegs(REG_003EE6);
#if TMDS_Port_D0
    else if(InputComboPort==InputCombo_D0) // D0
        FlagValue = msRegs(REG_003EC8);
#endif
#if TMDS_Port_D1
    else if(InputComboPort==InputCombo_D1) // D1
        FlagValue = msRegs(REG_003ECE);
#endif
#if TMDS_Port_D2
    else if(InputComboPort==InputCombo_D2) // D2
        FlagValue = msRegs(REG_003EE0);
#endif
#if TMDS_Port_D3
    else if(InputComboPort==InputCombo_D3) // D3
#if (CHIP_ID==MST9U3)
        FlagValue = msRegs(REG_003EBC);
#else
        FlagValue = msRegs(REG_003FC8);
#endif

#endif
#if TMDS_Port_D4
    else if(InputComboPort==InputCombo_D4) // D4
        FlagValue = msRegs(REG_003FCE);
#endif
#if TMDS_Port_D5
    else //if(InputComboPort==InputCombo_D5)// D5
        FlagValue = msRegs(REG_003FE0);
#endif
    return FlagValue&BIT6;
}

#if !D2B_XShared_DDCBuffer
BYTE DDC2Bi_FIFOMODE_READ_EMPTY_FLAG(BYTE InputComboPort)
{
    BYTE FlagValue=0;

    if(InputComboPort==InputCombo_A0) // A0
        FlagValue = msRegs(REG_003EE6);
#if TMDS_Port_D0
    else if(InputComboPort==InputCombo_D0) // D0
        FlagValue = msRegs(REG_003EC8);
#endif
#if TMDS_Port_D1
    else if(InputComboPort==InputCombo_D1) // D1
        FlagValue = msRegs(REG_003ECE);
#endif
#if TMDS_Port_D2
    else if(InputComboPort==InputCombo_D2) // D2
        FlagValue = msRegs(REG_003EE0);
#endif
#if TMDS_Port_D3
    else if(InputComboPort==InputCombo_D3) // D3
#if (CHIP_ID==MST9U3)
        FlagValue = msRegs(REG_003EBC);
#else
        FlagValue = msRegs(REG_003FC8);
#endif
#endif
#if TMDS_Port_D4
    else if(InputComboPort==InputCombo_D4) // D4
        FlagValue = msRegs(REG_003FCE);
#endif
#if TMDS_Port_D5
    else //if(InputComboPort==InputCombo_D5)// D5
        FlagValue = msRegs(REG_003FE0);
#endif
    return FlagValue&BIT7;
}
#endif

BYTE DDC2Bi_FIFOMODE_READ_HW_CHKSUM_FLAG(BYTE InputComboPort)
{
    BYTE HWCSValue=0;

    if(InputComboPort==InputCombo_A0) // A0
        HWCSValue = msRegs(REG_003EC2)&BIT0;
#if TMDS_Port_D0
    else if(InputComboPort==InputCombo_D0) // D0
        HWCSValue = msRegs(REG_003EC2)&BIT1;
#endif
#if TMDS_Port_D1
    else if(InputComboPort==InputCombo_D1) // D1
        HWCSValue = msRegs(REG_003EF0)&BIT0;
#endif
#if TMDS_Port_D2
    else if(InputComboPort==InputCombo_D2) // D2
        HWCSValue = msRegs(REG_003EF0)&BIT1;
#endif
#if TMDS_Port_D3
    else if(InputComboPort==InputCombo_D3) // D3
#if (CHIP_ID==MST9U3)
        HWCSValue = msRegs(REG_003EB6)&BIT5;
#else
        HWCSValue = msRegs(REG_003FC2)&BIT1;
#endif
#endif
#if TMDS_Port_D4
    else if(InputComboPort==InputCombo_D4) // D4
        HWCSValue = msRegs(REG_003FF0)&BIT0;
#endif
#if TMDS_Port_D5
    else //if(InputComboPort==InputCombo_D5)// D5
        HWCSValue = msRegs(REG_003FF0)&BIT1;
#endif
    return HWCSValue;
}

void DDC2Bi_CLR_INT(BYTE InputComboPort, BYTE ClrBits)
{
    if(InputComboPort==InputCombo_A0) // A0
        DDCADC_INT_CLR |= ClrBits;
#if TMDS_Port_D0
    else if(InputComboPort==InputCombo_D0) // D0
        DDCDVI0_INT_CLR |= ClrBits;
#endif
#if TMDS_Port_D1
    else if(InputComboPort==InputCombo_D1) // D1
        DDCDVI1_INT_CLR |= ClrBits;
#endif
#if TMDS_Port_D2
    else if(InputComboPort==InputCombo_D2) // D2
        DDCDVI2_INT_CLR |= ClrBits;
#endif
#if TMDS_Port_D3
    else if(InputComboPort==InputCombo_D3) // D3
        DDCDVI3_INT_CLR |= ClrBits;
#endif
#if TMDS_Port_D4
    else if(InputComboPort==InputCombo_D4) // D4
        DDCDVI4_INT_CLR |= ClrBits;
#endif
#if TMDS_Port_D5
    else //if(InputComboPort==InputCombo_D5)// D5
        DDCDVI5_INT_CLR |= ClrBits;
#endif
}
#endif // end of #if D2B_FIFO_Mode

void DDC2Bi_SET_RBUF(BYTE InputComboPort, BYTE RBufData)
{
    if(InputComboPort==InputCombo_A0) // A0
        ADC_RBUF_WDP = RBufData;
#if TMDS_Port_D0
    else if(InputComboPort==InputCombo_D0) // D0
        DDCDVI0_RBUF_WDP = RBufData;
#endif
#if TMDS_Port_D1
    else if(InputComboPort==InputCombo_D1) // D1
        DDCDVI1_RBUF_WDP = RBufData;
#endif
#if TMDS_Port_D2
    else if(InputComboPort==InputCombo_D2) // D2
        DDCDVI2_RBUF_WDP = RBufData;
#endif
#if TMDS_Port_D3
    else if(InputComboPort==InputCombo_D3) // D3
        DDCDVI3_RBUF_WDP = RBufData;
#endif
#if TMDS_Port_D4
    else if(InputComboPort==InputCombo_D4) // D4
        DDCDVI4_RBUF_WDP = RBufData;
#endif
#if TMDS_Port_D5
    else //if(InputComboPort==InputCombo_D5) // D5
        DDCDVI5_RBUF_WDP = RBufData;
#endif
}

void DDC2Bi_SET_WP_READY(BYTE InputComboPort)
{
    if(InputComboPort==InputCombo_A0) // A0
        DDC_ADC_WP_READY();
#if TMDS_Port_D0
    else if(InputComboPort==InputCombo_D0) // D0
        DDC_DVI0_WP_READY();
#endif
#if TMDS_Port_D1
    else if(InputComboPort==InputCombo_D1) // D1
        DDC_DVI1_WP_READY();
#endif
#if TMDS_Port_D2
    else if(InputComboPort==InputCombo_D2) // D2
        DDC_DVI2_WP_READY();
#endif
#if TMDS_Port_D3
    else if(InputComboPort==InputCombo_D3) // D3
        DDC_DVI3_WP_READY();
#endif
#if TMDS_Port_D4
    else if(InputComboPort==InputCombo_D4) // D4
        DDC_DVI4_WP_READY();
#endif
#if TMDS_Port_D5
    else //if(InputComboPort==InputCombo_D5) // D5
        DDC_DVI5_WP_READY();
#endif
}

BYTE DDC2Bi_Read_INT(BYTE InputComboPort)
{
    BYTE IntData=0;

    if(InputComboPort==InputCombo_A0) // A0
        IntData = DDCADC_INT_FLAG;
#if TMDS_Port_D0
    else if(InputComboPort==InputCombo_D0) // D0
        IntData = DDCDVI0_INT_FLAG;
#endif
#if TMDS_Port_D1
    else if(InputComboPort==InputCombo_D1) // D1
        IntData = DDCDVI1_INT_FLAG;
#endif
#if TMDS_Port_D2
    else if(InputComboPort==InputCombo_D2) // D2
        IntData = DDCDVI2_INT_FLAG;
#endif
#if TMDS_Port_D3
    else if(InputComboPort==InputCombo_D3) // D3
        IntData = DDCDVI3_INT_FLAG;
#endif
#if TMDS_Port_D4
    else if(InputComboPort==InputCombo_D4) // D4
        IntData = DDCDVI4_INT_FLAG;
#endif
#if TMDS_Port_D5
    else //if(InputComboPort==InputCombo_D5)// D5
        IntData = DDCDVI5_INT_FLAG;
#endif
    return IntData;
}

BYTE DDC2Bi_Read_WBUF(BYTE InputComboPort)
{
    BYTE WbufData=0;

    if(InputComboPort==InputCombo_A0) // A0
        WbufData = ADC_WBUF_RDP;
#if TMDS_Port_D0
    else if(InputComboPort==InputCombo_D0) // D0
        WbufData = DDCDVI0_WBUF_RDP;
#endif
#if TMDS_Port_D1
    else if(InputComboPort==InputCombo_D1) // D1
        WbufData = DDCDVI1_WBUF_RDP;
#endif
#if TMDS_Port_D2
    else if(InputComboPort==InputCombo_D2) // D2
        WbufData = DDCDVI2_WBUF_RDP;
#endif
#if TMDS_Port_D3
    else if(InputComboPort==InputCombo_D3) // D3
        WbufData = DDCDVI3_WBUF_RDP;
#endif
#if TMDS_Port_D4
    else if(InputComboPort==InputCombo_D4) // D4
        WbufData = DDCDVI4_WBUF_RDP;
#endif
#if TMDS_Port_D5
    else //if(InputComboPort==InputCombo_D5)// D5
        WbufData = DDCDVI5_WBUF_RDP;
#endif
    return WbufData;
}

void Clear_Botton_FIFO(void)
{
    BYTE  i;
    for (i=0;i<FIFO_SIZE;i++)
        u8ButtonFIFO[i]=0;
    u8ButtonFIFOIndex=0;
    g_u8DDCCIAControl=0;
    MccsEnable=FALSE;
}
void DDC2Bi_InitRx( void )
{
    // clear the rxState
    rxStatus = DDC2B_CLEAR;
    // the current buffer for a new message
    rxIndex = 0;
}

void DDC2Bi_Init( void )
{
    // mask all ddc interrupt
    D2B_INT_MASK_ALL(); // drvDDC2Bi_Init();

    DDC2Bi_InitRx();
    txStatus = DDC2B_CLEAR;

    txBufferPtr = &nullMsg1[0];
    txLength = sizeof( nullMsg1 );

    D2B_SET_ID_VGA(DDCCI_ENABLE_FLAG);
    D2B_INT_MASK_VGA();
    CLR_ADC_INT_FLAG();
    DDCADC_RBUF_WDP = *txBufferPtr++;
    DDC_ADC_WP_READY();

#if TMDS_Port_D0
    D2B_SET_ID_DVI0(DDCCI_ENABLE_FLAG);
    D2B_INT_MASK_DVI0();
    CLR_DVI0_INT_FLAG();
    DDCDVI0_RBUF_WDP = *txBufferPtr++;
    DDC_DVI0_WP_READY();
#endif
#if TMDS_Port_D1
    D2B_SET_ID_DVI1(DDCCI_ENABLE_FLAG);
    D2B_INT_MASK_DVI1();
    CLR_DVI1_INT_FLAG();
    DDCDVI1_RBUF_WDP = *txBufferPtr++;
    DDC_DVI1_WP_READY();
#endif
#if TMDS_Port_D2
    D2B_SET_ID_DVI2(DDCCI_ENABLE_FLAG);
    D2B_INT_MASK_DVI2();
    CLR_DVI2_INT_FLAG();
    DDCDVI2_RBUF_WDP = *txBufferPtr++;
    DDC_DVI2_WP_READY();
#endif
#if TMDS_Port_D3
    D2B_SET_ID_DVI3(DDCCI_ENABLE_FLAG);
    D2B_INT_MASK_DVI3();
    CLR_DVI3_INT_FLAG();
    DDCDVI3_RBUF_WDP = *txBufferPtr++;
    DDC_DVI3_WP_READY();
#endif
#if TMDS_Port_D4
    D2B_SET_ID_DVI4(DDCCI_ENABLE_FLAG);
    D2B_INT_MASK_DVI4();
    CLR_DVI4_INT_FLAG();
    DDCDVI4_RBUF_WDP = *txBufferPtr++;
    DDC_DVI4_WP_READY();
#endif
#if TMDS_Port_D5
    D2B_SET_ID_DVI5(DDCCI_ENABLE_FLAG);
    D2B_INT_MASK_DVI5();
    CLR_DVI5_INT_FLAG();
    DDCDVI5_RBUF_WDP = *txBufferPtr++;
    DDC_DVI5_WP_READY();
#endif
#if ENABLE_DP_INPUT
    D2B_SET_ID_DP(DDCCI_ENABLE_FLAG);//DDCDP_DDCCI_ID=0xB7;
#endif

#if 0//ENABLE_3D_FUNCTION && GLASSES_TYPE==GLASSES_NVIDIA
    DDCDVI_NV3D_DDCCI_ID = 0xFD;
    DDCDVI_NV3D_INTMASK_EN = BIT6 | BIT5 | BIT2 | BIT1;
    CLR_DDC_DVI_NV3D_INT_FLAG();
#endif

    Clear_Botton_FIFO();

#if D2B_FIFO_Mode
  #if D2B_XShared_DDCBuffer
    DDCBuffer = &D2B_XSRAM_READ(0);
  #endif
    //TMDS_Port_A0
    D2B_SET_FIFOMODE_VGA(TRUE);
    D2B_SET_FIFOMODE_ENHANCE_VGA(D2B_FIFO_Enhance);
    D2B_SET_FIFOMODE_SIZE_VGA(D2B_FIFOSize_INDEX);
    D2B_SET_FIFOMODE_XDATAMAP_VGA(D2B_FIFO_XdataAddressStart_A0);
    D2B_SET_FIFOMODE_SIMPLIFIED_VGA(D2B_SIMPLIFIED_MODE);
#if D2B_DOUBLE_FIFO_Mode

    D2B_SET_DOUBLE_FIFOMODE_VGA(TRUE);
    D2B_SET_FIFOMODE_TX_SIZE_VGA(D2B_FIFOSize_INDEX);
    D2B_SET_FIFOMODE_TX_XDATAMAP_VGA(D2B_FIFO_XdataWriteAddressStart_A0);
#endif
  #if TMDS_Port_D0
    D2B_SET_FIFOMODE_D0(TRUE);
    D2B_SET_FIFOMODE_ENHANCE_D0(D2B_FIFO_Enhance);
    D2B_SET_FIFOMODE_SIZE_D0(D2B_FIFOSize_INDEX);
    D2B_SET_FIFOMODE_XDATAMAP_D0(D2B_FIFO_XdataAddressStart_D0);
    D2B_SET_FIFOMODE_SIMPLIFIED_D0(D2B_SIMPLIFIED_MODE);
#if D2B_DOUBLE_FIFO_Mode
    D2B_SET_DOUBLE_FIFOMODE_D0(TRUE);
    D2B_SET_FIFOMODE_TX_SIZE_D0(D2B_FIFOSize_INDEX);
    D2B_SET_FIFOMODE_TX_XDATAMAP_D0(D2B_FIFO_XdataWriteAddressStart_D0);
#endif
  #endif
  #if TMDS_Port_D1
    D2B_SET_FIFOMODE_D1(TRUE);
    D2B_SET_FIFOMODE_ENHANCE_D1(D2B_FIFO_Enhance);
    D2B_SET_FIFOMODE_SIZE_D1(D2B_FIFOSize_INDEX);
    D2B_SET_FIFOMODE_XDATAMAP_D1(D2B_FIFO_XdataAddressStart_D1);
    D2B_SET_FIFOMODE_SIMPLIFIED_D1(D2B_SIMPLIFIED_MODE);
#if D2B_DOUBLE_FIFO_Mode
    D2B_SET_DOUBLE_FIFOMODE_D1(TRUE);
    D2B_SET_FIFOMODE_TX_SIZE_D1(D2B_FIFOSize_INDEX);
    D2B_SET_FIFOMODE_TX_XDATAMAP_D1(D2B_FIFO_XdataWriteAddressStart_D1);
#endif
  #endif
  #if TMDS_Port_D2
    D2B_SET_FIFOMODE_D2(TRUE);
    D2B_SET_FIFOMODE_ENHANCE_D2(D2B_FIFO_Enhance);
    D2B_SET_FIFOMODE_SIZE_D2(D2B_FIFOSize_INDEX);
    D2B_SET_FIFOMODE_XDATAMAP_D2(D2B_FIFO_XdataAddressStart_D2);
    D2B_SET_FIFOMODE_SIMPLIFIED_D2(D2B_SIMPLIFIED_MODE);
#if D2B_DOUBLE_FIFO_Mode
    D2B_SET_DOUBLE_FIFOMODE_D2(TRUE);
    D2B_SET_FIFOMODE_TX_SIZE_D2(D2B_FIFOSize_INDEX);
    D2B_SET_FIFOMODE_TX_XDATAMAP_D2(D2B_FIFO_XdataWriteAddressStart_D2);
#endif
  #endif
  #if TMDS_Port_D3
    D2B_SET_FIFOMODE_D3(TRUE);
    D2B_SET_FIFOMODE_ENHANCE_D3(D2B_FIFO_Enhance);
    D2B_SET_FIFOMODE_SIZE_D3(D2B_FIFOSize_INDEX);
    D2B_SET_FIFOMODE_XDATAMAP_D3(D2B_FIFO_XdataAddressStart_D3);
    D2B_SET_FIFOMODE_SIMPLIFIED_D3(D2B_SIMPLIFIED_MODE);
#if D2B_DOUBLE_FIFO_Mode
    D2B_SET_DOUBLE_FIFOMODE_D3(TRUE);
    D2B_SET_FIFOMODE_TX_SIZE_D3(D2B_FIFOSize_INDEX);
    D2B_SET_FIFOMODE_TX_XDATAMAP_D3(D2B_FIFO_XdataWriteAddressStart_D3);
#endif
  #endif
  #if TMDS_Port_D4
    D2B_SET_FIFOMODE_D4(TRUE);
    D2B_SET_FIFOMODE_ENHANCE_D4(D2B_FIFO_Enhance);
    D2B_SET_FIFOMODE_SIZE_D4(D2B_FIFOSize_INDEX);
    D2B_SET_FIFOMODE_XDATAMAP_D4(D2B_FIFO_XdataAddressStart_D4);
    D2B_SET_FIFOMODE_SIMPLIFIED_D4(D2B_SIMPLIFIED_MODE);
  #endif
  #if TMDS_Port_D5
    D2B_SET_FIFOMODE_D5(TRUE);
    D2B_SET_FIFOMODE_ENHANCE_D5(D2B_FIFO_Enhance);
    D2B_SET_FIFOMODE_SIZE_D5(D2B_FIFOSize_INDEX);
    D2B_SET_FIFOMODE_XDATAMAP_D5(D2B_FIFO_XdataAddressStart_D5);
    D2B_SET_FIFOMODE_SIMPLIFIED_D5(D2B_SIMPLIFIED_MODE);
  #endif
#else
    D2B_SET_FIFOMODE_VGA(FALSE);
  #if TMDS_Port_D0
    D2B_SET_FIFOMODE_D0(FALSE);
  #endif
  #if TMDS_Port_D1
    D2B_SET_FIFOMODE_D1(FALSE);
  #endif
  #if TMDS_Port_D2
    D2B_SET_FIFOMODE_D2(FALSE);
  #endif
  #if TMDS_Port_D3
    D2B_SET_FIFOMODE_D3(FALSE);
  #endif
  #if TMDS_Port_D4
    D2B_SET_FIFOMODE_D4(FALSE);
  #endif
  #if TMDS_Port_D5
    D2B_SET_FIFOMODE_D5(FALSE);
  #endif
#endif
#if D2B_HW_CheckSum
  #if (CHIP_ID==MST9U3)
    D2B_SET_D2B_HW_CheckSum(_ENABLE, TMDS_Port_D3, TMDS_Port_D2, TMDS_Port_D1, TMDS_Port_D0);
  #else
    D2B_SET_D2B_HW_CheckSum(_ENABLE, TMDS_Port_D5, TMDS_Port_D4, TMDS_Port_D3, TMDS_Port_D2, TMDS_Port_D1, TMDS_Port_D0);
  #endif
#endif

#ifdef MST9U_FPGA
    SET_DDCCI_ENABLE_FLAG();
#endif
    INT_IRQ_D2B_ENABLE(TRUE); //enable d2b int
}

void DDC2Bi_GetTxBuffer( void )
{
    BYTE i;
    for( i = MAX_TXWAIT_DELAY; i > 0; i-- )
        if( txStatus == DDC2B_MSGON )
        {
            Delay1ms( 1 );
        }
    txStatus = DDC2B_CLEAR;
}

BYTE DDC2Bi_ComputeChecksum( BYTE count )
{
    BYTE CheckSum = INITTxCK;
    BYTE i = 0;

    for( ; count > 0; count-- )
        CheckSum ^= DDCBuffer[i++];
    return CheckSum;
}

void DDC2Bi_MessageReady( void )
{
#if HKC_ColorTemp_Adjust
if((ATEModeFlag)
#ifdef Adj_DefBri_PWM
 && (DefBriPwmFlag==0)
 #endif
 )
{
		if(ubAutoAlign==0)
			Clr_ATEModeFlag();

  DDCBuffer[3]= 0x50^0x6E^DDCBuffer[0]^DDCBuffer[1]^DDCBuffer[2];
 DDCBuffer[4]= 0;
 DDCBuffer[5]= 0;

}
else
#endif	
{
    DDCBuffer[0] |= DDC2Bi_CONTROL_STATUS_FLAG;
    DDCBuffer[( DDCBuffer[0] &~DDC2Bi_CONTROL_STATUS_FLAG ) + 1] = DDC2Bi_ComputeChecksum(( DDCBuffer[0] &~DDC2Bi_CONTROL_STATUS_FLAG ) + 1 );
    DDC2Bi_GetTxBuffer();
}
    txBufferPtr = &DDCBuffer[0];
#if 0 //for debug/test
    Osd_Show();
    Osd_DrawNum( 1, 0, DDCBuffer[0] );
    Osd_DrawNum( 1, 1, DDCBuffer[1] );
    Osd_DrawNum( 1, 2, DDCBuffer[2] );
    Osd_DrawNum( 1, 3, DDCBuffer[3] );
    Osd_DrawNum( 1, 4, DDCBuffer[4] );
    Osd_DrawNum( 1, 5, DDCBuffer[5] );
    Osd_DrawNum( 1, 6, DDCBuffer[6] );
    Osd_DrawNum( 1, 7, DDCBuffer[7] );
    Osd_DrawNum( 1, 8, DDCBuffer[8] );
    Osd_DrawNum( 1, 9, DDCBuffer[9] );
    Delay1ms( 1000 );
#endif
    txLength = ( DDCBuffer[0] & ( ~DDC2Bi_CONTROL_STATUS_FLAG ) ) + 2;

#if D2B_FIFO_Mode
    if((!INPUT_IS_DISPLAYPORT(rxInputPort)) && txLength > 0)    //20121030
    {
    #if !D2B_XShared_DDCBuffer
        WORD i=0;
    #endif
        BYTE WriteXdataST_Add;

    #if D2B_FIFO_Enhance
        WriteXdataST_Add = DDC2Bi_FIFOMODE_READ_CUR(rxComboPort) + 1;
    #else
        WriteXdataST_Add= 0;
    #endif

        D2B_XSRAM_WRITE(DDC2Bi_FIFOMODE_XDATA_ADDR_OFFSET(rxComboPort,1)+(WriteXdataST_Add%D2B_FIFO_Size), 0x6E);
    #if !D2B_XShared_DDCBuffer
        for( i = 0; i < txLength; i++ )
        {
            D2B_XSRAM_WRITE(DDC2Bi_FIFOMODE_XDATA_ADDR_OFFSET(rxComboPort,1)+(( WriteXdataST_Add+i+1 )%D2B_FIFO_Size), DDCBuffer[i]);

            if( i==(D2B_FIFO_Size-2) )
            {
                txIndex = D2B_FIFO_Size-1;
                break;
            }
        }
    #endif
        DDC2Bi_SET_WP_READY(rxComboPort);
    }
#endif
}

void DDC2Bi_ParseCommand( void )
{
    BYTE length; //, command;

    length = DDCBuffer[LENGTH] & 0x7F;
    if( rxIndex <= 0 )
        return ;
    if( length <= 0 )
    {
        txBufferPtr = &nullMsg1[1];
        txLength = sizeof( nullMsg1 );
        DDC2Bi_SET_RBUF(rxComboPort, DDC2B_DEST_ADDRESS);
    }
#if D2B_XDATA_DEBUG
    {
        BYTE ii;
        for (ii=0;ii</*16*/24;ii++)
            D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+24/*32*/+ii, DDCBuffer[ii]);
    }
#endif
    SET_DDCCI_MCCS_CMD_FLAG();
    length = AlignControl();
    CLR_DDCCI_MCCS_CMD_FLAG();
    if(length == 0xFE || length == 0xFD)
    {
        DDCBuffer[0] = 0x80;
    }
    else if(length <= 0)
    {
        DDCBuffer[0] = 0x80;
        //return ;
    }

    DDC2Bi_MessageReady();
    DDC2Bi_SET_RBUF(rxComboPort, DDC2B_DEST_ADDRESS);
}

#if ENABLE_DAISY_CHAIN
void DDC2Bi_DCSetAck(void)
{
    BYTE u8Retry=10, u8SendData=0, u8ReplyData=0;

    if(INPUT_IS_DC_RX(rxInputPort))
    {
        while(!(msDCSetCmdMccsAckReady(u8SendData, &u8ReplyData)) && --u8Retry)
        {
            DRVDDC2BI_PRINT("[drvDDC2Bi] DC Setup Retry %d\n",u8Retry);
        }

        if( (u8Retry==0) || (u8ReplyData&BIT0)==0x00 )
        {
            DRVDDC2BI_PRINT("[drvDDC2Bi] DC Setup NG\n");
        }
        else
        {
            //DRVDDC2BI_PRINT("[drvDDC2Bi] DC Setup OK\n");`
        }
    }
}
#endif

void DDC2Bi_CommandHandler( void )
{
    //BYTE SCDetWin=GetSCOfInputPort(rxInputPort);

    if (MccsEnable)
    {
        if ((g_u8DDCCIAControl != 0) && u8ButtonFIFOIndex < FIFO_SIZE
            && (u8ButtonFIFO[u8ButtonFIFOIndex] == 0))
        {
            if ((u8ButtonFIFOIndex==0) || (u8ButtonFIFO[u8ButtonFIFOIndex-1]!=g_u8DDCCIAControl) )
            {
                u8ButtonFIFO[u8ButtonFIFOIndex]=g_u8DDCCIAControl;
                u8ButtonFIFOIndex++;
                g_u8DDCCIAControl=0;
            }
        }
    }
    if( rxStatus == DDC2B_COMPLETED )
    {
#if 0 //for debug/test
    Menu_Show();
    Osd_DrawNum( 17, 0, DDCBuffer[0] );
    Osd_DrawNum( 17, 1, DDCBuffer[1] );
    Osd_DrawNum( 17, 2, DDCBuffer[2] );
    Osd_DrawNum( 17, 3, DDCBuffer[3] );
    Osd_DrawNum( 17, 4, DDCBuffer[4] );
    Osd_DrawNum( 17, 5, DDCBuffer[5] );
    Osd_DrawNum( 17, 6, DDCBuffer[6] );
    Osd_DrawNum( 17, 7, DDCBuffer[7] );
    Osd_DrawNum( 17, 8, DDCBuffer[8] );
    Osd_DrawNum( 17, 9, DDCBuffer[9] );
   // Delay1ms( 1000 );
#endif

	
        DDC2Bi_ParseCommand();
        Clr_RxBusyFlag();
        DDC2Bi_InitRx();

    #if ENABLE_DAISY_CHAIN
        DDC2Bi_DCSetAck();
    #endif


	
    }
    if (DDCCI_AUTO_COLOR_FLAG)
    {
        CLR_DDCCI_AUTO_COLOR_FLAG();
    #if ENABLE_VGA_INPUT
        if (mStar_AutoColor(rxInputPort))
        {
            SaveFactorySetting();
        }
    #endif
    }

    if (DDCCI_AUTO_GEOMTRY_FLAG)
    {
        CLR_DDCCI_AUTO_GEOMTRY_FLAG();
    #if ENABLE_VGA_INPUT
        if (mStar_AutoGeomtry(USER_PREF_WIN_SEL))
        {
            USER_PREF_AUTOTIME(rxInputPort) = AUTO_TIMES_BIT;
            SaveModeSetting();
        }
    #endif
    }



	
}

#if D2B_FIFO_Mode
void DoDDCCI( BYTE Source )
{
    BYTE intFlag;
    WORD u8AddPoint;
    WORD u16StartCurrent=0;

    intFlag = DDC2Bi_Read_INT(Source);

    if( intFlag )
    {
        if(DDC_RW_SRAM_FLAG) // DDC access xdata fail
        {
            D2BErrorFlag=1;
            DDC2Bi_InitRx();

#if D2B_XDATA_DEBUG
            D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+64+(IntCount%16)+(IntCount/16)*80, 0x33);
            D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+80+(IntCount%16)+(IntCount/16)*80, 0x33);
            D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+96+(IntCount%16)+(IntCount/16)*80, msRegs(REG_003EEA));
            D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+112+(IntCount%16)+(IntCount/16)*80, msRegs(REG_003EE6));
            IntCount++;
#endif
            DDC_RW_SRAM_CLR;
            DDC2Bi_FIFOMODE_SET_RW_DONE(Source);
            DDC2Bi_FIFOMODE_CLR_EMPTYFULL_FLAG(Source);
            if( _bit7_( DDC2Bi_Read_INT(Source)) )
                DDC2Bi_CLR_INT(Source, BIT7);
            return;
        }

        switch( intFlag &( TXBI | RCBI ) )
        {
            // TX int
            case TXBI:
                //txStatus = DDC2B_MSGON;                // no characters left
                Previous_DDC2MCU_CUR_ADR = Present_DDC2MCU_CUR_ADR;
#if 1 // prevent from R2 accrss current twice, causes DDC2Bi internal condition checking error
                u16StartCurrent = DDC2Bi_FIFOMODE_READ_CUR_START(Source);
                Present_DDC2MCU_CUR_ADR = u16StartCurrent & 0xFF;
                Present_DDC2MCU_START_ADR = (u16StartCurrent & 0xFF00)>>8;
#else
                Present_DDC2MCU_CUR_ADR = DDC2Bi_FIFOMODE_READ_CUR(Source); // MUST
#endif
#if D2B_XDATA_DEBUG
                if(IntCount>=32)
                {
                    IntCount=31;
                    D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+48+(IntCount%16)+(IntCount/16)*80, IntCount|0xC0);
                }else
                    D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+48+(IntCount%16)+(IntCount/16)*80, IntCount|0x80);

                D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+64+(IntCount%16)+(IntCount/16)*80, Present_DDC2MCU_CUR_ADR);
                D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+80+(IntCount%16)+(IntCount/16)*80, 0x77);
                D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+96+(IntCount%16)+(IntCount/16)*80, txIndex);
                D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+112+(IntCount%16)+(IntCount/16)*80, msRegs(REG_003EE6));

                D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+9, D2BErrorFlag);
                D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+10, msRegs(REG_003EE6));
#endif

                if(txIndex) // Remaining Tx
                {
#if !D2B_XShared_DDCBuffer
                    if( (DDC2Bi_FIFOMODE_READ_EMPTY_FLAG(Source)) || (D2BErrorFlag==1) ) // Buffer Read Empty
                    {
                        D2BErrorFlag=1;
                        //u8TxStatus=DDC2B_CLEAR;
#if D2B_XDATA_DEBUG
                        D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+64+(IntCount%16)+(IntCount/16)*80, 0x77);
                        D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+80+(IntCount%16)+(IntCount/16)*80, 0x77);
                        D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+96+(IntCount%16)+(IntCount/16)*80, 0x77);
                        D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+112+(IntCount%16)+(IntCount/16)*80, msRegs(REG_003EE6));
                        IntCount++;
#endif
                        DDC2Bi_FIFOMODE_SET_RW_DONE(Source);
                        DDC2Bi_FIFOMODE_CLR_EMPTYFULL_FLAG(Source);
                        return;
                    }
#if 0  // prevent from R2 accrss current twice, causes DDC2Bi internal condition checking error
                    Present_DDC2MCU_START_ADR = DDC2Bi_FIFOMODE_READ_START(Source);
#endif

                    if( _bit7_( DDC2Bi_Read_INT(Source)) )
                    {
                        if( Present_DDC2MCU_CUR_ADR < Present_DDC2MCU_START_ADR )
                        {
                            Present_DDC2MCU_CUR_ADR += ( BIT0 << ( D2B_FIFOSize_INDEX + 3 ) );
                        }
#if D2B_XDATA_DEBUG
                        D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+64+(IntCount%16)+(IntCount/16)*80, Present_DDC2MCU_START_ADR|0x80);
                        D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+80+(IntCount%16)+(IntCount/16)*80, Present_DDC2MCU_CUR_ADR|0x80);
                        D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+96+(IntCount%16)+(IntCount/16)*80, Previous_DDC2MCU_CUR_ADR|0x80);
                        D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+112+(IntCount%16)+(IntCount/16)*80, msRegs(REG_003EE6));
#endif
                        for( u8AddPoint = Present_DDC2MCU_START_ADR; u8AddPoint <= Present_DDC2MCU_CUR_ADR; u8AddPoint++ )
                        {
                            D2B_XSRAM_WRITE(DDC2Bi_FIFOMODE_XDATA_ADDR_OFFSET(Source,1)+u8AddPoint%D2B_FIFO_Size, DDCBuffer[txIndex++]) ;
                            if( txIndex >= txLength )
                            {
                                txIndex=0;
                                Present_DDC2MCU_CUR_ADR &= (D2B_FIFO_Size-1);
                                break;
                            }
                        }
                    }
                    else
                    {
                        while( Present_DDC2MCU_CUR_ADR<Previous_DDC2MCU_CUR_ADR )
                        {
                            Present_DDC2MCU_CUR_ADR += ( BIT0 << ( D2B_FIFOSize_INDEX + 3 ) );
                        }
#if D2B_XDATA_DEBUG
                        D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+64+(IntCount%16)+(IntCount/16)*80, Present_DDC2MCU_START_ADR);
                        D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+80+(IntCount%16)+(IntCount/16)*80, Present_DDC2MCU_CUR_ADR);
                        D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+96+(IntCount%16)+(IntCount/16)*80, Previous_DDC2MCU_CUR_ADR);
                        D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+112+(IntCount%16)+(IntCount/16)*80, msRegs(REG_003EE6));
#endif
                        for( u8AddPoint = Previous_DDC2MCU_CUR_ADR + 1; u8AddPoint <= Present_DDC2MCU_CUR_ADR; u8AddPoint++ )
                        {
                            D2B_XSRAM_WRITE(DDC2Bi_FIFOMODE_XDATA_ADDR_OFFSET(Source,1)+u8AddPoint%D2B_FIFO_Size, DDCBuffer[txIndex++]) ;
                            if( txIndex >= txLength )
                            {
                                txIndex=0;
                                Present_DDC2MCU_CUR_ADR &= (D2B_FIFO_Size-1);
                                break;
                            }
                        }
                    }

                    if(DDC2Bi_FIFOMODE_READ_EMPTY_FLAG(Source)) // Buffer Read Empty at Race condition
                    {
                        D2BErrorFlag=1;
                        //u8TxStatus=DDC2B_CLEAR;
#if D2B_XDATA_DEBUG
                        D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+64+(IntCount%16)+(IntCount/16)*80, Present_DDC2MCU_START_ADR);
                        D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+80+(IntCount%16)+(IntCount/16)*80, Present_DDC2MCU_CUR_ADR);
                        D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+96+(IntCount%16)+(IntCount/16)*80, 0x66);
                        D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+112+(IntCount%16)+(IntCount/16)*80, msRegs(REG_003EE6));
#endif
                        DDC2Bi_FIFOMODE_CLR_EMPTYFULL_FLAG(Source); // clear
                    }
#endif // #if !D2B_XShared_DDCBuffer
                }
                else
                {
                   DDC2Bi_FIFOMODE_CLR_EMPTYFULL_FLAG(Source); // clear
                }

                DDC2Bi_FIFOMODE_SET_RW_DONE(Source); // To prevent Full_Error_flag
                if( _bit7_( DDC2Bi_Read_INT(Source)) )
                    DDC2Bi_CLR_INT(Source, BIT7);

                break;

            // RX int
            case RCBI:
                Previous_DDC2MCU_CUR_ADR = Present_DDC2MCU_CUR_ADR;
#if 1 // prevent from R2 accrss current twice, causes DDC2Bi internal condition checking error
                u16StartCurrent = DDC2Bi_FIFOMODE_READ_CUR_START(Source);
                Present_DDC2MCU_CUR_ADR = u16StartCurrent & 0xFF;
                Present_DDC2MCU_START_ADR = (u16StartCurrent & 0xFF00)>>8;
#else
                Present_DDC2MCU_CUR_ADR = DDC2Bi_FIFOMODE_READ_CUR(Source);
                Present_DDC2MCU_START_ADR = DDC2Bi_FIFOMODE_READ_START(Source);
#endif

#if D2B_XDATA_DEBUG
                if( _bit7_( DDC2Bi_Read_INT(Source)) )//( _bit7_( _msRegs(REG_3E10] ) == 1 )
                    IntCount=0;
                D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+11, D2BErrorFlag);
                D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+12, (msRegs(REG_003EE6)));
                D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+48+(IntCount%16)+(IntCount/16)*80, IntCount);
#endif

                if(DDC2Bi_FIFOMODE_READ_FULL_FLAG(Source)) // Buffer Write Overflow
                {
                    D2BErrorFlag=1;
                    DDC2Bi_InitRx();

#if D2B_XDATA_DEBUG
                    D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+64+(IntCount%16)+(IntCount/16)*80, 0xFF);
                    D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+80+(IntCount%16)+(IntCount/16)*80, 0xFF);
                    D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+96+(IntCount%16)+(IntCount/16)*80, 0xFF);
                    D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+112+(IntCount%16)+(IntCount/16)*80, msRegs(REG_003EE6));
                    IntCount++;
#endif
                    DDC2Bi_FIFOMODE_SET_RW_DONE(Source);
                    DDC2Bi_FIFOMODE_CLR_EMPTYFULL_FLAG(Source);
                    if( _bit7_( DDC2Bi_Read_INT(Source)) )
                        DDC2Bi_CLR_INT(Source, BIT7);

                    return;
                }
                else if( (D2BErrorFlag==1) && ( _bit7_( DDC2Bi_Read_INT(Source))==0 ) ) // Not New Command
                {
                    DDC2Bi_InitRx();

#if D2B_XDATA_DEBUG
                    D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+64+(IntCount%16)+(IntCount/16)*80, 0x88);
                    D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+80+(IntCount%16)+(IntCount/16)*80, 0x88);
                    D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+96+(IntCount%16)+(IntCount/16)*80, 0xFF);
                    D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+112+(IntCount%16)+(IntCount/16)*80, msRegs(REG_003EE6));
                    IntCount++;
#endif
                    DDC2Bi_FIFOMODE_SET_RW_DONE(Source);

                    return;
                }
                else
                {
                    D2BErrorFlag=0;
                }

#if D2B_XDATA_DEBUG
                D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+13, Previous_DDC2MCU_CUR_ADR);
                D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+14, Present_DDC2MCU_CUR_ADR);
                D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+15, Present_DDC2MCU_START_ADR);
#endif

                if( _bit7_( DDC2Bi_Read_INT(Source)) )
                {
                    rxIndex = 0;
                    rxLength = 0;
#if !D2B_HW_CheckSum
                    rxChkSum = INITRxCK;
#endif
#if !D2B_XShared_DDCBuffer
                    if( Present_DDC2MCU_CUR_ADR < Present_DDC2MCU_START_ADR )
                    {
                        Present_DDC2MCU_CUR_ADR += ( BIT0 << ( D2B_FIFOSize_INDEX + 3 ) );
                    }
#endif

#if D2B_XDATA_DEBUG
                    {
                        BYTE xdata ii;
                        for (ii=0;ii</*16*/24;ii++)
                            D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+24/*32*/+ii, 0);

                        for (ii=0;ii<32;ii++)
                        {
                            D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+64+(ii%16)+(ii/16)*80, 0);
                            D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+80+(ii%16)+(ii/16)*80, 0);
                            D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+96+(ii%16)+(ii/16)*80, 0);
                            D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+112+(ii%16)+(ii/16)*80, 0);
                        }
                    }
#endif


#if D2B_XDATA_DEBUG
                    D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+64+(IntCount%16)+(IntCount/16)*80, Present_DDC2MCU_START_ADR|0x80);
                    D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+80+(IntCount%16)+(IntCount/16)*80, Present_DDC2MCU_CUR_ADR|0x80);
                    D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+96+(IntCount%16)+(IntCount/16)*80, Previous_DDC2MCU_CUR_ADR|0x80);
                    D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+112+(IntCount%16)+(IntCount/16)*80, msRegs(REG_003EE6));
#endif
#if D2B_XShared_DDCBuffer
                    DDCBuffer = &D2B_XSRAM_READ(DDC2Bi_FIFOMODE_XDATA_ADDR_OFFSET(Source,0)+1);
#endif
                    for( u8AddPoint = Present_DDC2MCU_START_ADR + 1; u8AddPoint <= Present_DDC2MCU_CUR_ADR; u8AddPoint++ )
                    {
#if !D2B_XShared_DDCBuffer
                        DDCBuffer[rxIndex++] = D2B_XSRAM_READ(DDC2Bi_FIFOMODE_XDATA_ADDR_OFFSET(Source,0)+u8AddPoint%D2B_FIFO_Size);
#else
                        rxIndex++;
#endif
#if !D2B_HW_CheckSum
                        rxChkSum ^= D2B_XSRAM_READ(DDC2Bi_FIFOMODE_XDATA_ADDR_OFFSET(Source,0)+u8AddPoint%D2B_FIFO_Size);
#endif
                    }
                }
                else
                {
#if D2B_XDATA_DEBUG
                    D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+64+(IntCount%16)+(IntCount/16)*80, Present_DDC2MCU_START_ADR);
                    D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+80+(IntCount%16)+(IntCount/16)*80, Present_DDC2MCU_CUR_ADR);
                    D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+96+(IntCount%16)+(IntCount/16)*80, Previous_DDC2MCU_CUR_ADR);
                    D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+112+(IntCount%16)+(IntCount/16)*80, msRegs(REG_003EE6));
#endif
#if !D2B_XShared_DDCBuffer
                    while( Present_DDC2MCU_CUR_ADR<=Previous_DDC2MCU_CUR_ADR )
                    {
                        Present_DDC2MCU_CUR_ADR += ( BIT0 << ( D2B_FIFOSize_INDEX + 3 ) );
                    }
#endif

                    for( u8AddPoint = Previous_DDC2MCU_CUR_ADR + 1; u8AddPoint <= Present_DDC2MCU_CUR_ADR; u8AddPoint++ )
                    {
#if !D2B_XShared_DDCBuffer
                        DDCBuffer[rxIndex++] = D2B_XSRAM_READ(DDC2Bi_FIFOMODE_XDATA_ADDR_OFFSET(Source,0)+u8AddPoint%D2B_FIFO_Size);
#else
                        rxIndex++;
#endif
#if !D2B_HW_CheckSum
                        rxChkSum ^= D2B_XSRAM_READ(DDC2Bi_FIFOMODE_XDATA_ADDR_OFFSET(Source,0)+u8AddPoint%D2B_FIFO_Size);
#endif
                    }
                }

                // CheckSum
                if(rxIndex>0 && rxLength==0)
                    rxLength = DDCBuffer[0] & ~DDC2Bi_CONTROL_STATUS_FLAG;

#if D2B_HW_CheckSum
                if( ( (rxLength+ 2) == rxIndex ) && ( rxLength != 0 ) && ( DDC2Bi_FIFOMODE_READ_HW_CHKSUM_FLAG(Source)==0 ) )
#else
                if(( rxChkSum == 0x00 && rxIndex>0 ) )
#endif
                {
                    Set_RxBusyFlag();
                    rxStatus = DDC2B_COMPLETED;
#if !D2B_XShared_DDCBuffer
                    Present_DDC2MCU_CUR_ADR &= (D2B_FIFO_Size-1);
#endif
                    rxInputPort = D2B_FIFOMODE_DDCPORT2INPUT(Source);
                    rxComboPort = Source;
                }
                else
                {
                    rxStatus = DDC2B_CLEAR;
                    Clr_RxBusyFlag();
#if D2B_XDATA_DEBUG
                D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+48+(IntCount%16)+(IntCount/16)*80, D2B_XSRAM_READ(FIFODBGOFFSET+D2B_FIFO_Size+48+(IntCount%16)+(IntCount/16)*80)|0x40);
#endif
                }


                if(DDC2Bi_FIFOMODE_READ_FULL_FLAG(Source)) // Buffer Write Overflow at Race condition
                {
                    D2BErrorFlag=1;
                    DDC2Bi_InitRx();

#if D2B_XDATA_DEBUG
                    D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+64+(IntCount%16)+(IntCount/16)*80, Present_DDC2MCU_START_ADR);
                    D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+80+(IntCount%16)+(IntCount/16)*80, Present_DDC2MCU_CUR_ADR);
                    D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+96+(IntCount%16)+(IntCount/16)*80, 0x55);
                    D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+112+(IntCount%16)+(IntCount/16)*80, msRegs(REG_003EE6));
#endif

                    DDC2Bi_FIFOMODE_CLR_EMPTYFULL_FLAG(Source); // clear
                }

                DDC2Bi_FIFOMODE_SET_RW_DONE(Source);
                if( _bit7_( DDC2Bi_Read_INT(Source)) )
                    DDC2Bi_CLR_INT(Source, BIT7);

                break;

            // Default
            default:
#if D2B_XDATA_DEBUG
                D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+3, 0x44);
                D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+64+(IntCount%16)+(IntCount/16)*80, 0x44);
                D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+80+(IntCount%16)+(IntCount/16)*80, 0x44);
                D2B_XSRAM_WRITE(FIFODBGOFFSET+D2B_FIFO_Size+96+(IntCount%16)+(IntCount/16)*80, intFlag);
#endif
                DDC2Bi_SET_RBUF(Source, DDC2Bi_Read_WBUF(Source));
                txStatus = DDC2B_CLEAR;
                DDC2Bi_InitRx();
                break;

        }
    }

#if D2B_XDATA_DEBUG
    IntCount++;
    if(rxStatus == DDC2B_COMPLETED)
        IntCountTmp = IntCount;
#endif
}
#else
void DoDDCCI( BYTE Source )
{
    BYTE xdata intFlag, rxByte;

    intFlag = DDC2Bi_Read_INT(Source);

    if( intFlag )
    {
        switch( intFlag &( TXBI | RCBI ) )
        {
            case TXBI:                //
                txStatus = DDC2B_MSGON;                // no characters left
                if( !( txLength ) )
                {
                    // clear the transmit status
                    txStatus = DDC2B_CLEAR;

                    DDC2Bi_SET_RBUF(Source, 0);
                    DDC2Bi_SET_WP_READY(Source);

                    return ;
                }
                // send out the current byte
                DDC2Bi_SET_RBUF(Source, *txBufferPtr++);
                txLength--;
                break;

            case RCBI:
                // read the received byte
                rxByte = DDC2Bi_Read_WBUF(Source);

                // depending of the message status
                switch( rxStatus )
                {
                        // in case there is nothing received yet
                    case DDC2B_CLEAR:
                        if( rxByte == DDC2B_SRC_ADDRESS )
                        {
                            rxStatus++; // = DDC2B_SRCADDRESS;
                            rxChkSum = INITRxCK;
                        }
                        else
                        {
                            DDC2Bi_InitRx();
                        }
                        break;
                        //
                    case DDC2B_SRCADDRESS:
                        // get the length
                        rxLength = rxByte &~DDC2Bi_CONTROL_STATUS_FLAG;
                        // put the received byte in DDCBuffer
                        DDCBuffer[rxIndex++] = rxByte;
                        rxChkSum ^= rxByte;
                        // set the receive body state
                        rxStatus++; // = DDC2B_COMMAND;
                        //if it is a NULL message
                        if (rxLength == 0)
                        {
                            rxStatus= DDC2B_COMPLETED;
                            Set_RxBusyFlag();
                        }
                        else if (rxLength >= DDC_BUFFER_LENGTH)
                        {
                            DDC2Bi_InitRx();
                        }
                        break;
                        // get the command
                    case DDC2B_COMMAND:
                        // save the commandbyte
                        rxStatus++; // = DDC2B_RECBODY;
                        // get the message body
                    case DDC2B_RECBODY:
                        DDCBuffer[rxIndex++] = rxByte;
                        rxChkSum ^= rxByte;
                        rxLength--;
                        // the last byte in the message body
                        if( rxLength == 0 )
                        {
                            rxStatus++; // = DDC2B_WAITFORCK;
                        }
                        break;
                        // ...here we are waiting for CheckSum...
                    case DDC2B_WAITFORCK:
                        // if CheckSum match
                        if( rxChkSum == rxByte )
                        {
                            rxInputPort = D2B_FIFOMODE_DDCPORT2INPUT(Source);
                            rxComboPort = Source;
                            rxStatus = DDC2B_COMPLETED;
                            Set_RxBusyFlag();
                        }
                        // elsechecksum error
                        else
                        {
                            // if CheckSum error re-initialize the receive buffer
                            DDC2Bi_InitRx();
                        }
                        break;
                    default:
                        // clear the rxState and the current buffer for a new message
                        if( !RxBusyFlag )
                            DDC2Bi_InitRx();
                        break;
                }
                break;

            default:
                DDC2Bi_SET_RBUF(Source, DDC2Bi_Read_WBUF(Source));
                txStatus = DDC2B_CLEAR;
                DDC2Bi_InitRx();
                break;

        }
    }
}
#endif

void ISR_DDC2Bi(void)
{
    if( ADC_INT_FLAG &( BIT3 | BIT4 ) )
    {
        DoDDCCI( InputCombo_A0 );
        CLR_ADC_INT_FLAG();
    }
  #if TMDS_Port_D0
    else if( DDCDVI0_INT_FLAG &( BIT3 | BIT4 ) )
    {
        DoDDCCI( InputCombo_D0 );
        CLR_DVI0_INT_FLAG();
    }
  #endif
  #if TMDS_Port_D1
    else if( DDCDVI1_INT_FLAG &( BIT3 | BIT4 ) )
    {
        DoDDCCI( InputCombo_D1 );
        CLR_DVI1_INT_FLAG();
    }
  #endif
  #if TMDS_Port_D2
    else if( DDCDVI2_INT_FLAG &( BIT3 | BIT4 ) )
    {
        DoDDCCI( InputCombo_D2 );
        CLR_DVI2_INT_FLAG();
    }
  #endif
  #if TMDS_Port_D3
    else if( DDCDVI3_INT_FLAG &( BIT3 | BIT4 ) )
    {
        DoDDCCI( InputCombo_D3 );
        CLR_DVI3_INT_FLAG();
    }
  #endif
  #if TMDS_Port_D4
    else if( DDCDVI4_INT_FLAG &( BIT3 | BIT4 ) )
    {
        DoDDCCI( InputCombo_D4 );
        CLR_DVI4_INT_FLAG();
    }
  #endif
  #if TMDS_Port_D5
    else if( DDCDVI5_INT_FLAG &( BIT3 | BIT4 ) )
    {
        DoDDCCI( InputCombo_D5 );
        CLR_DVI5_INT_FLAG();
    }
  #endif
}


#if ENABLE_DP_INPUT
Bool mdrv_DDC2BI_GetDPSendFlag(BYTE ucAuxLength, BYTE ucDDCIndex, BYTE *ucDDCBuffer)
{
    Bool bSendFlag = FALSE;
    BYTE uctemp = 0;

    if(rxStatus == DDC2B_CLEAR && (txLength > 0))
    {
        bSendFlag = TRUE;

	   if(ucDDCIndex == 0)
        {
	   	   ucDDCBuffer[0] = DDC2B_DEST_ADDRESS;
		   for(uctemp = 0; uctemp < (ucAuxLength - 1); uctemp++)
		   ucDDCBuffer[uctemp+1] = DDCBuffer[uctemp + ucDDCIndex];

        }
	   else
            {
		   for(uctemp = 0; uctemp < (ucAuxLength); uctemp++)
	          ucDDCBuffer[uctemp] = DDCBuffer[uctemp + ucDDCIndex-1];
            }

    }
    return bSendFlag;
}

void mdrv_DDC2BI_SetDPReceiveStatus(BYTE ucPortSelect, BYTE ucDDCLength, BYTE *ucDDCBuffer)
{
    BYTE uctemp = 0;

    UNUSED(ucPortSelect);
    rxStatus = DDC2B_COMPLETED;
    txLength = 0;
    rxInputPort = D2B_FIFOMODE_DDCPORT2INPUT(4);
    rxIndex = ucDDCLength;

    for(uctemp = 0; uctemp < ucDDCLength; uctemp++)
    {
        DDCBuffer[uctemp] = ucDDCBuffer[uctemp];
    }
}

#if DPENABLEMCCS
//BYTE TX_Index=0;
//================= DP =====================

void DDC2BI_DP(void)
{
    BYTE AuxData;
    BYTE u8AUXCH_RxCmd;
    BYTE u8DDC_Length;

    u8AUXCH_RxCmd = msRegs(REG_001138 + Aux)&0x0F;
    u8DDC_Length = msRegs(REG_00113A+ Aux);
    //===============//<write>PC Read Command============================
    if(u8AUXCH_RxCmd & _BIT0)    //DP Tx
    {
        if (u8DDC_Length)
        {
            if(rxStatus == DDC2B_CLEAR && txLength)
            {
                while(u8DDC_Length-- && txLength)
                {
                    BYTE  temp3=100;
                    while(temp3--)
                    {
                        _nop_();
                    };
                    if(g_ucDDC_TxRecord)
                    {
                        msRegs(REG_0011D3 + Aux)=DDCBuffer[g_ucDDC_TxRecord-1];
                        //msRegs(REG_0011D3 + Aux)=GUIDData[g_ucDDC_TxRecord-1];
                        txLength--;
                    }
                    else
                        msRegs(REG_0011D3 + Aux)=DDC2B_DEST_ADDRESS;
                    g_ucDDC_TxRecord++;
                }
            }
            else
            {
                while(u8DDC_Length--)
                {
                    msRegs(REG_0011D3 + Aux)= dpNullMsg[g_ucDDC_TxRecord++];
                }
            }
        }
    }
    //===============//<write>PC Write Command============================
#if 1
    else if(u8AUXCH_RxCmd==0x00 || u8AUXCH_RxCmd==0x04)    //DP Rx
    {
#if 1
        if (u8DDC_Length)
        {
            if(u8DDC_RxRecord==0)
            {
                u8DDC_RxRecord=TRUE;
                msRegs(REG_0011FC + Aux)= msRegs(REG_0011FC + Aux)|_BIT2;
                //_nop_();
                AuxData= msRegs(REG_001175 + Aux);
                rxIndex=0;
                u8DDC_Length--;
            }
            while(u8DDC_Length--)
            {
                BYTE  temp3=100;
                msRegs(REG_0011FC + Aux)= msRegs(REG_0011FC + Aux)|_BIT2;
                while(temp3--)
                {
                    _nop_();
                };
                DDCBuffer[rxIndex++] = msRegs(REG_001175 + Aux);
                //DDCData[rxIndex++] = msRegs(REG_001175 + Aux);
            }
        }

        if (u8AUXCH_RxCmd==0x00)
        {
            u8DDC_RxRecord=FALSE;
            g_ucDDC_TxRecord=0;
            rxStatus = DDC2B_COMPLETED;
            txLength=0;
            //rxInputPort = (Aux + 1);					// Update
            rxInputPort = D2B_FIFOMODE_DDCPORT2INPUT(Aux/COMBO_PHY_OFFSET);
            msRegs(REG_113331)=rxInputPort;
        }

    }
#else

    if(u8DDC_RxRecord==0)
    {
        u8DDC_RxRecord=TRUE;
        msRegs(REG_0011FC + Aux)= msRegs(REG_0011FC + Aux)|_BIT2;
        AuxData= msRegs(REG_001175 + Aux);
        AuxIndex=0;
        u8DDC_Length = u8DDC_Length+1;
        while(u8DDC_Length--)
        {
            BYTE  temp3=255;
            msRegs(REG_0011FC + Aux)= msRegs(REG_0011FC + Aux)|_BIT2;
            while(temp3--){};
            DDCData[AuxIndex1++] = msRegs(REG_001175 + Aux);
        }
    }
    if (u8AUXCH_RxCmd==0x00)
    {
        u8DDC_RxRecord=FALSE;
        g_ucDDC_TxRecord=0;
        rxStatus = DDC2B_COMPLETED;
        txLength=0;
        rxInputPort = (Aux + 1);							// Update
        // rxInputPort = D2B_FIFOMODE_DDCPORT2INPUT(Source);
    }
    }

#endif

    /*
    msRegs(REG_0011FC + Aux)= msRegs(REG_0011FC + Aux)|_BIT2;
    AuxData= msRegs(REG_001175 + Aux);
    AuxIndex=0;
    u8DDC_Length = u8DDC_Length+1;
    while(u8DDC_Length--)
    {
        BYTE  temp3=255;
        msRegs(REG_0011FC + Aux)= msRegs(REG_0011FC + Aux)|_BIT2;
        while(temp3--){};
        DDCData[AuxIndex++] = msRegs(REG_001175 + Aux);
    }
    */
#endif
    msRegs(REG_001179 + Aux)= msRegs(REG_001179 + Aux)|_BIT7;//Clear Interrupt
    msRegs(REG_001179 + Aux)= 0x00;


}

void msDP_DDC2BI(void)
{

#if DPENABLEMCCS
    DDC2BI_DP() ;
#endif

}

#endif
#endif

void msDrvWriteInternalEDID(BYTE u8Block, BYTE u8Addr, BYTE u8Data)
{
    #if (CHIP_ID==MST9U3) || (CHIP_ID==MST9U4)
    msWriteByteMask( REG_003EEB, u8Block, 0x1F);    // Select sram base address for cpu read/write
    #else
    msWriteByteMask( REG_003EEB, u8Block, 0xF0);    // Select sram base address for cpu read/write
    #endif
    msWriteByte( REG_003E4B, u8Addr );    // DDC address port for CPU read/write
    msWriteByte( REG_003E4C, u8Data);  // DDC Data Port for cpu write
    msWriteByteMask( REG_003E43, BIT5, BIT5);    //ADC sram write data pulse gen when cpu write
    TimeOutCounter = 10;
    while((msReadByte(REG_003E7B) & BIT5)&&( TimeOutCounter != 0));
}

BYTE msDrvReadInternalEDID(BYTE u8Block, BYTE u8Addr)
{
    BYTE ucValue = 0;
    #if (CHIP_ID==MST9U3) || (CHIP_ID==MST9U4)
    msWriteByteMask( REG_003EEB, u8Block, 0x1F);    // Select sram base address for cpu read/write
    #else
    msWriteByteMask( REG_003EEB, u8Block, 0xF0);    // Select sram base address for cpu read/write
    #endif
    msWriteByte( REG_003E4B, u8Addr );    // DDC address port for CPU read/write
    msWriteByteMask( REG_003E43, BIT4, BIT4);    //ADC sram read data pulse gen when cpu read
    TimeOutCounter = 10;
    while((msReadByte(REG_003E7B) & BIT4)&&( TimeOutCounter != 0));

    ucValue = msReadByte( REG_003E0E );  // DDC Data Port for cpu read
    return ucValue;
}

unsigned char xdata ddcciTbl1[16]=
{
100,2,3,15,
5,233,7,8,
9,90,11,12,
13,14,4,200
};
unsigned char xdata ddcciTbl2[16]=
{
19,122,21,16,
25,26,207,28,
29,158,20,22,
13,14,24,189
};
unsigned int DdcciCheck(unsigned int index)
{
 index=index&0x00FF;
 return (((*(ddcciTbl1+(index>>4)))<<8)|(*(ddcciTbl2+(index&0x0F))));
}


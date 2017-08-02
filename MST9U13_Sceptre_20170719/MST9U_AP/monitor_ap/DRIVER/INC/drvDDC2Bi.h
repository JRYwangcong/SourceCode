
#ifndef _DRVDDC2BI_H_
#define _DRVDDC2BI_H_

#include "DDC2Bi.H"
#include "Global.h"
#if ENABLE_DP_INPUT
#include "drvDPRxApp.h"
#endif
///////////////////////////////////////
// DRVDDC2BI VERSION: V01
////////////////////////////////////////
// Enum of DDC mode provided for selection
#define D2BMode_SingleBuffer                    0 // Old DDC mode with tx/rx each with one byte buffer
#define D2BMode_FIFO_Normal                     1 // New DDC normal mode using xdata as tx/rx buffer
#define D2BMode_FIFO_Normal_XSharedDDCBuffer    2 // New DDC normal mode using xdata as tx/rx buffer, and xdata shared with DDCBuffer
#define D2BMode_FIFO_Normal_XSharedAll          3 // New DDC normal mode using xdata as tx/rx buffer, and xdata shared with DDCBuffer & All Ports
#define D2BMode_FIFO_Normal_XSharedInputPors    4 // New DDC normal mode using xdata as tx/rx buffer, and xdata shared with All Ports

// DDC mode selection (xshared DDCBuffer mode only support in 51)
#define D2BModeSel  D2BMode_FIFO_Normal_XSharedInputPors // Be aware of buffer overflow problem, MUST reserved enough buffer size!!!!!


// DDC2BI xdata access mode
#define D2B_R2              1 // 1: R2 access xdata(riu), 0: 51 access xdata


// Enum of DDC FIFO length
#define D2B_FIFO_8Level     0
#define D2B_FIFO_16Level    1
#define D2B_FIFO_32Level    2
#define D2B_FIFO_64Level    3
#define D2B_FIFO_128Level   4
#define D2B_FIFO_256Level   5

// DDC mode configuration

#define D2B_DOUBLE_FIFO_Mode   (1&(CHIP_ID>=MST9U3))
#if D2BModeSel==D2BMode_SingleBuffer
#define D2B_FIFO_Mode           0
#define D2B_FIFO_Enhance        (0&D2B_FIFO_Mode&(!D2B_DOUBLE_FIFO_Mode))
#define D2B_XShared_DDCBuffer   (0&(!D2B_FIFO_Enhance)&(D2B_FIFO_Mode)) // DDCBuffer need change to circular case in enhancemode(loop)
#define D2B_XShared_InputPors   (0&D2B_FIFO_Mode)
#define D2B_HW_CheckSum         0
#define D2B_FIFOSize_INDEX      D2B_FIFO_64Level
#define D2B_SIMPLIFIED_MODE     0
#define DDC_BUFFER_LENGTH       64//MaxBufLen 50 // ddc buffer length
#elif D2BModeSel==D2BMode_FIFO_Normal
#define D2B_FIFO_Mode           1
#define D2B_FIFO_Enhance        (0&D2B_FIFO_Mode&(!D2B_DOUBLE_FIFO_Mode))
#define D2B_XShared_DDCBuffer   (0&(!D2B_FIFO_Enhance)&(D2B_FIFO_Mode)) // DDCBuffer need change to circular case in enhancemode(loop)
#define D2B_XShared_InputPors   (0&D2B_FIFO_Mode)
#define D2B_HW_CheckSum         1
#define D2B_FIFOSize_INDEX      D2B_FIFO_32Level
#define D2B_SIMPLIFIED_MODE     1
#define DDC_BUFFER_LENGTH       64// ddc buffer length
#elif D2BModeSel==D2BMode_FIFO_Normal_XSharedDDCBuffer
#define D2B_FIFO_Mode           1
#define D2B_FIFO_Enhance        (0&D2B_FIFO_Mode&(!D2B_DOUBLE_FIFO_Mode))
#define D2B_XShared_DDCBuffer   (1&(!D2B_FIFO_Enhance)&(D2B_FIFO_Mode)) // DDCBuffer need change to circular case in enhancemode(loop)
#define D2B_XShared_InputPors   (0&D2B_FIFO_Mode)
#define D2B_HW_CheckSum         1
#define D2B_FIFOSize_INDEX      D2B_FIFO_64Level
#define D2B_SIMPLIFIED_MODE     1
#define DDC_BUFFER_LENGTH       64 // Same as D2B_FIFO_Size
#elif D2BModeSel==D2BMode_FIFO_Normal_XSharedAll
#define D2B_FIFO_Mode           1
#define D2B_FIFO_Enhance        (0&D2B_FIFO_Mode&(!D2B_DOUBLE_FIFO_Mode))
#define D2B_XShared_DDCBuffer   (1&(!D2B_FIFO_Enhance)&(D2B_FIFO_Mode)) // DDCBuffer need change to circular case in enhancemode(loop)
#define D2B_XShared_InputPors   (1&D2B_FIFO_Mode)
#define D2B_HW_CheckSum         1
#define D2B_FIFOSize_INDEX      D2B_FIFO_128Level
#define D2B_SIMPLIFIED_MODE     1
#define DDC_BUFFER_LENGTH       128 // Same as D2B_FIFO_Size
#elif D2BModeSel==D2BMode_FIFO_Normal_XSharedInputPors
#define D2B_FIFO_Mode           1
#define D2B_FIFO_Enhance        (0&D2B_FIFO_Mode&(!D2B_DOUBLE_FIFO_Mode))
#define D2B_XShared_DDCBuffer   (0&(!D2B_FIFO_Enhance)&(D2B_FIFO_Mode)) // DDCBuffer need change to circular case in enhancemode(loop)
#define D2B_XShared_InputPors   (1&D2B_FIFO_Mode)
#define D2B_HW_CheckSum         1
#define D2B_FIFOSize_INDEX      D2B_FIFO_64Level
#define D2B_SIMPLIFIED_MODE     1
#define DDC_BUFFER_LENGTH       64// ddc buffer length
#else
#warning "please implement drvDDC2Bi for new applications"
#endif

// DDC debug using xdata to store info, temply keeped for fpga/asic verify
#define D2B_XDATA_DEBUG         0&D2B_FIFO_Mode
// align dbg msg in xdata
#if D2B_XDATA_DEBUG
#if D2B_FIFOSize_INDEX==D2B_FIFO_8Level
#define FIFODBGOFFSET 8
#else
#define FIFODBGOFFSET 0
#endif
#endif

// DDC FIFO Index mapping to Size
#if D2B_FIFOSize_INDEX==D2B_FIFO_8Level
#define D2B_FIFO_Size   8
#elif D2B_FIFOSize_INDEX==D2B_FIFO_16Level
#define D2B_FIFO_Size   16
#elif D2B_FIFOSize_INDEX==D2B_FIFO_32Level
#define D2B_FIFO_Size   32
#elif D2B_FIFOSize_INDEX==D2B_FIFO_64Level
#define D2B_FIFO_Size   64
#elif D2B_FIFOSize_INDEX==D2B_FIFO_128Level
#define D2B_FIFO_Size   128
#elif D2B_FIFOSize_INDEX==D2B_FIFO_256Level
#define D2B_FIFO_Size   256
#endif



// DDC FIFO mode xdata mapping address
#if D2B_XShared_InputPors
#define D2B_FIFO_XdataAddressStart      (XDATA_DDC_ADDR_START)
#define D2B_FIFO_XdataAddressStart_A0   (D2B_FIFO_XdataAddressStart)
#define D2B_FIFO_XdataAddressStart_D0   (D2B_FIFO_XdataAddressStart)
#define D2B_FIFO_XdataAddressStart_D1   (D2B_FIFO_XdataAddressStart)
#define D2B_FIFO_XdataAddressStart_D2   (D2B_FIFO_XdataAddressStart)
#define D2B_FIFO_XdataAddressStart_D3   (D2B_FIFO_XdataAddressStart)
#define D2B_FIFO_XdataAddressStart_D4   (D2B_FIFO_XdataAddressStart)
#define D2B_FIFO_XdataAddressStart_D5   (D2B_FIFO_XdataAddressStart)
#define D2B_FIFO_XdataWriteAddressStart      (XDATA_DDC_TX_ADDR_START)
#define D2B_FIFO_XdataWriteAddressStart_A0   (D2B_FIFO_XdataWriteAddressStart)
#define D2B_FIFO_XdataWriteAddressStart_D0   (D2B_FIFO_XdataWriteAddressStart)
#define D2B_FIFO_XdataWriteAddressStart_D1   (D2B_FIFO_XdataWriteAddressStart)
#define D2B_FIFO_XdataWriteAddressStart_D2   (D2B_FIFO_XdataWriteAddressStart)
#define D2B_FIFO_XdataWriteAddressStart_D3   (D2B_FIFO_XdataWriteAddressStart)
#define D2B_FIFO_XdataWriteAddressStart_D4   (D2B_FIFO_XdataWriteAddressStart)
#define D2B_FIFO_XdataWriteAddressStart_D5   (D2B_FIFO_XdataWriteAddressStart)
#define D2B_FIFO_XdataSize              (D2B_FIFO_Size*1)
#else
#define D2B_FIFO_XdataAddressStart      (XDATA_DDC_ADDR_START)
#define D2B_FIFO_XdataAddressStart_A0   (D2B_FIFO_XdataAddressStart)
#define D2B_FIFO_XdataAddressStart_D0   (D2B_FIFO_XdataAddressStart_A0+D2B_FIFO_Size)
#define D2B_FIFO_XdataAddressStart_D1   (TMDS_Port_D0?(D2B_FIFO_XdataAddressStart_D0+D2B_FIFO_Size):D2B_FIFO_XdataAddressStart_D0)
#define D2B_FIFO_XdataAddressStart_D2   (TMDS_Port_D1?(D2B_FIFO_XdataAddressStart_D1+D2B_FIFO_Size):D2B_FIFO_XdataAddressStart_D1)
// Swap D2 & D3 in MST9U3 due to the design order is different from MST9U1/2
#if CHIP_ID == MST9U3
#define D2B_FIFO_XdataAddressStart_D3   (TMDS_Port_D3?(D2B_FIFO_XdataAddressStart_D2+D2B_FIFO_Size):D2B_FIFO_XdataAddressStart_D2)
#define D2B_FIFO_XdataAddressStart_D4   (TMDS_Port_D2?(D2B_FIFO_XdataAddressStart_D3+D2B_FIFO_Size):D2B_FIFO_XdataAddressStart_D3)
#else
#define D2B_FIFO_XdataAddressStart_D3   (TMDS_Port_D2?(D2B_FIFO_XdataAddressStart_D2+D2B_FIFO_Size):D2B_FIFO_XdataAddressStart_D2)
#define D2B_FIFO_XdataAddressStart_D4   (TMDS_Port_D3?(D2B_FIFO_XdataAddressStart_D3+D2B_FIFO_Size):D2B_FIFO_XdataAddressStart_D3)
#endif
#define D2B_FIFO_XdataAddressStart_D5   (TMDS_Port_D4?(D2B_FIFO_XdataAddressStart_D4+D2B_FIFO_Size):D2B_FIFO_XdataAddressStart_D4)
#define D2B_FIFO_XdataSize              (D2B_FIFO_Size*(1+TMDS_Port_D0+TMDS_Port_D1+TMDS_Port_D2+TMDS_Port_D3+TMDS_Port_D4+TMDS_Port_D5))
#define D2B_FIFO_XdataWriteAddressStart      (XDATA_DDC_TX_ADDR_START)
#define D2B_FIFO_XdataWriteAddressStart_A0   (D2B_FIFO_XdataWriteAddressStart)
#define D2B_FIFO_XdataWriteAddressStart_D0   (D2B_FIFO_XdataWriteAddressStart_A0+D2B_FIFO_Size)
#define D2B_FIFO_XdataWriteAddressStart_D1   (TMDS_Port_D0?(D2B_FIFO_XdataWriteAddressStart_D0+D2B_FIFO_Size):D2B_FIFO_XdataWriteAddressStart_D0)
#define D2B_FIFO_XdataWriteAddressStart_D2   (TMDS_Port_D1?(D2B_FIFO_XdataWriteAddressStart_D1+D2B_FIFO_Size):D2B_FIFO_XdataWriteAddressStart_D1)
// Swap D2 & D3 in MST9U3 due to the design order is different from MST9U1/2
#if CHIP_ID == MST9U3
#define D2B_FIFO_XdataWriteAddressStart_D3   (TMDS_Port_D3?(D2B_FIFO_XdataWriteAddressStart_D2+D2B_FIFO_Size):D2B_FIFO_XdataWriteAddressStart_D2)
#define D2B_FIFO_XdataWriteAddressStart_D4   (TMDS_Port_D2?(D2B_FIFO_XdataWriteAddressStart_D3+D2B_FIFO_Size):D2B_FIFO_XdataWriteAddressStart_D3)
#else
#define D2B_FIFO_XdataWriteAddressStart_D3   (TMDS_Port_D2?(D2B_FIFO_XdataWriteAddressStart_D2+D2B_FIFO_Size):D2B_FIFO_XdataWriteAddressStart_D2)
#define D2B_FIFO_XdataWriteAddressStart_D4   (TMDS_Port_D3?(D2B_FIFO_XdataWriteAddressStart_D3+D2B_FIFO_Size):D2B_FIFO_XdataWriteAddressStart_D3)
#endif
#define D2B_FIFO_XdataWriteAddressStart_D5   (TMDS_Port_D4?(D2B_FIFO_XdataWriteAddressStart_D4+D2B_FIFO_Size):D2B_FIFO_XdataWriteAddressStart_D4)
#endif

enum
{
    InputCombo_D0,
    InputCombo_D1,
    InputCombo_D2,
    InputCombo_D3,
    InputCombo_D4,
    InputCombo_D5,
    InputCombo_A0,
};

#define FIFO_SIZE               5
// receive/transmit buffer syntax defines
#define LENGTH                          0
#define COMMAND                         1
#define DATA1                           2
#define DATA2                           3
#define CHKSUM                          4
// DDC2Bi receive/transmit status bytes definitions
#define DDC2B_CLEAR                     0
#define DDC2B_SRCADDRESS                1
#define DDC2B_COMMAND                   2
#define DDC2B_RECBODY                   3
#define DDC2B_WAITFORCK                 4
#define DDC2B_COMPLETED                 5
#define DDC2B_MSGON                     6
#define DDC2B_OVER                      7
// DDC2Bi standard definitions
#define DDC2B_DEST_ADDRESS              0x6e
#define DDC2B_SRC_ADDRESS               0x51
#define DDC2Bi_CONTROL_STATUS_FLAG      0x80
// CheckSum start values
#define INITRxCK        (DDC2B_SRC_ADDRESS ^ DDC2B_DEST_ADDRESS)
#define INITTxCK        (0x50 ^ DDC2B_DEST_ADDRESS)
#define INITNMVTxCK     (0x50^DDC2B_SRC_ADDRESS)
//interrupt flag
#define TXBI     BIT4
#define RCBI     BIT3
#define SlvBMI   BIT5
// maximum wait time (msec) for getting the transmit buffer
#define MAX_TXWAIT_DELAY    20


extern volatile BYTE idata rxInputPort; // Annie 2006.7.5
extern volatile BYTE idata rxComboPort;
extern volatile BYTE rxIndex;
extern volatile BYTE rxStatus;
extern volatile BYTE rxLength;
#if !D2B_HW_CheckSum
extern volatile BYTE rxChkSum;
#endif
extern volatile BYTE txStatus;
extern volatile BYTE *txBufferPtr;
extern volatile BYTE txLength;
//extern BYTE sendData;
//=====================================
extern volatile BYTE xdata u8ButtonFIFO[FIFO_SIZE];
extern volatile BYTE xdata u8ButtonFIFOIndex;
extern volatile BYTE xdata g_u8DDCCISaveCounter;
extern volatile Bool xdata MccsEnable;
//=====================================
extern code BYTE nullMsg1[3];
extern code BYTE nullMsg[3];


#if D2B_XShared_DDCBuffer
extern BYTE *DDCBuffer;
#else
extern volatile BYTE xdata DDCBuffer[DDC_BUFFER_LENGTH];
#endif

#if D2B_FIFO_Mode
extern volatile WORD idata txIndex;
extern volatile WORD idata Previous_DDC2MCU_CUR_ADR;
extern volatile WORD idata Present_DDC2MCU_CUR_ADR;
extern volatile WORD idata Present_DDC2MCU_START_ADR;
extern volatile BYTE idata D2BErrorFlag;
#if D2B_XDATA_DEBUG
#if !D2B_R2 // 51
extern BYTE xdata XSRAMBuffer[D2B_FIFO_XdataSize+256];
#endif
extern volatile BYTE xdata IntCount;
extern volatile BYTE xdata IntCountTmp;
#else
#if !D2B_R2 // 51
extern BYTE xdata XSRAMBuffer[D2B_FIFO_XdataSize];
#endif
#endif

#if D2B_R2
#define D2B_XSRAM_READ(u16Addr)             (R2_XSRAM_READ((WORD)(u16Addr)+XDATA_DDC_ADDR_START))
#define D2B_XSRAM_WRITE(u16Addr, u8Data)    (R2_XSRAM_WRITE((WORD)(u16Addr)+XDATA_DDC_ADDR_START,(BYTE)(u8Data)))
#else // 51
#define D2B_XSRAM_READ(u16Addr)             (XSRAMBuffer[u16Addr])
#define D2B_XSRAM_WRITE(u16Addr, u8Data)    (XSRAMBuffer[u16Addr] = u8Data)
#endif

#endif // end of #if D2B_FIFO_Mode

//==========Register=======================================
#define INTMASK_EN_ADC      msRegs(REG_003E14)
#define ADC_INT_FLAG        msRegs(REG_003E10)
#define ADC_WBUF_RDP        msRegs(REG_003E00)
#define ADC_RBUF_WDP        msRegs(REG_003E01)
#define ADC_DDCCI_ID        msRegs(REG_003E0A)
#define ADC_INT_CLR         msRegs(REG_003E1C)

#define INTMASK_EN_DVI0     msRegs(REG_003E16)
#define DVI0_INT_FLAG       msRegs(REG_003E12)
#define DVI0_WBUF_RDP       msRegs(REG_003E04)
#define DVI0_RBUF_WDP       msRegs(REG_003E05)
#define DVI0_DDCCI_ID       msRegs(REG_003E0C)
#define DVI0_INT_CLR        msRegs(REG_003E1E)

#define INTMASK_EN_DVI1     msRegs(REG_003E17)
#define DVI1_INT_FLAG       msRegs(REG_003E13)
#define DVI1_WBUF_RDP       msRegs(REG_003E06)
#define DVI1_RBUF_WDP       msRegs(REG_003E07)
#define DVI1_DDCCI_ID       msRegs(REG_003E0D)
#define DVI1_INT_CLR        msRegs(REG_003E1F)

#define INTMASK_EN_DVI2     msRegs(REG_003E55)
#define DVI2_INT_FLAG       msRegs(REG_003E54)
#define DVI2_WBUF_RDP       msRegs(REG_003E50)
#define DVI2_RBUF_WDP       msRegs(REG_003E51)
#define DVI2_DDCCI_ID       msRegs(REG_003E52)
#define DVI2_INT_CLR        msRegs(REG_003E57)
#if (CHIP_ID==MST9U3) || (CHIP_ID==MST9U4)
#define INTMASK_EN_DVI3     msRegs(REG_003EA5)
#define DVI3_INT_FLAG       msRegs(REG_003EA4)
#define DVI3_WBUF_RDP       msRegs(REG_003EA0)
#define DVI3_RBUF_WDP       msRegs(REG_003EA1)
#define DVI3_DDCCI_ID       msRegs(REG_003EA2)
#define DVI3_INT_CLR        msRegs(REG_003EA7)
#else
#define INTMASK_EN_DVI3     msRegs(REG_003F16)
#define DVI3_INT_FLAG       msRegs(REG_003F12)
#define DVI3_WBUF_RDP       msRegs(REG_003F04)
#define DVI3_RBUF_WDP       msRegs(REG_003F05)
#define DVI3_DDCCI_ID       msRegs(REG_003F0C)
#define DVI3_INT_CLR        msRegs(REG_003F1E)
#endif
#define INTMASK_EN_DVI4     msRegs(REG_003F17)
#define DVI4_INT_FLAG       msRegs(REG_003F13)
#define DVI4_WBUF_RDP       msRegs(REG_003F06)
#define DVI4_RBUF_WDP       msRegs(REG_003F07)
#define DVI4_DDCCI_ID       msRegs(REG_003F0D)
#define DVI4_INT_CLR        msRegs(REG_003F1F)

#define INTMASK_EN_DVI5     msRegs(REG_003F55)
#define DVI5_INT_FLAG       msRegs(REG_003F54)
#define DVI5_WBUF_RDP       msRegs(REG_003F50)
#define DVI5_RBUF_WDP       msRegs(REG_003F51)
#define DVI5_DDCCI_ID       msRegs(REG_003F52)
#define DVI5_INT_CLR        msRegs(REG_003F57)

#define DP_DDCCI_ID         _msRegs[REG_1FF7] // Not Confirmed yet in CHIP_FAMILY_TSUMC!!!!
#define DDC2BI_CTRL         msRegs(REG_003E09)


#define DDC_RW_SRAM_FLAG   (msRegs(REG_003EEA)&(BIT3|BIT2))
#define DDC_RW_SRAM_CLR    (msRegs(REG_003EEA) |= (BIT1|BIT0))

#define DDCADC_INTMASK_EN           INTMASK_EN_ADC
#define DDCADC_INT_FLAG             ADC_INT_FLAG
#define DDCADC_WBUF_RDP             ADC_WBUF_RDP
#define DDCADC_RBUF_WDP             ADC_RBUF_WDP
#define DDCADC_DDCCI_ID             ADC_DDCCI_ID
#define DDCADC_INT_CLR              ADC_INT_CLR
#define CLR_ADC_INT_FLAG()          ADC_INT_CLR = 0xFF

#define DDCDVI0_INTMASK_EN           INTMASK_EN_DVI0
#define DDCDVI0_INT_FLAG             DVI0_INT_FLAG
#define DDCDVI0_WBUF_RDP             DVI0_WBUF_RDP
#define DDCDVI0_RBUF_WDP             DVI0_RBUF_WDP
#define DDCDVI0_DDCCI_ID             DVI0_DDCCI_ID
#define DDCDVI0_INT_CLR              DVI0_INT_CLR
#define CLR_DVI0_INT_FLAG()          DVI0_INT_CLR = 0xFF

#define DDCDVI1_INTMASK_EN           INTMASK_EN_DVI1
#define DDCDVI1_INT_FLAG             DVI1_INT_FLAG
#define DDCDVI1_WBUF_RDP             DVI1_WBUF_RDP
#define DDCDVI1_RBUF_WDP             DVI1_RBUF_WDP
#define DDCDVI1_DDCCI_ID             DVI1_DDCCI_ID
#define DDCDVI1_INT_CLR              DVI1_INT_CLR
#define CLR_DVI1_INT_FLAG()          DVI1_INT_CLR = 0xFF

#define DDCDVI2_INTMASK_EN           INTMASK_EN_DVI2
#define DDCDVI2_INT_FLAG             DVI2_INT_FLAG
#define DDCDVI2_WBUF_RDP             DVI2_WBUF_RDP
#define DDCDVI2_RBUF_WDP             DVI2_RBUF_WDP
#define DDCDVI2_DDCCI_ID             DVI2_DDCCI_ID
#define DDCDVI2_INT_CLR              DVI2_INT_CLR
#define CLR_DVI2_INT_FLAG()          DVI2_INT_CLR = 0xFF

#define DDCDVI3_INTMASK_EN           INTMASK_EN_DVI3
#define DDCDVI3_INT_FLAG             DVI3_INT_FLAG
#define DDCDVI3_WBUF_RDP             DVI3_WBUF_RDP
#define DDCDVI3_RBUF_WDP             DVI3_RBUF_WDP
#define DDCDVI3_DDCCI_ID             DVI3_DDCCI_ID
#define DDCDVI3_INT_CLR              DVI3_INT_CLR
#define CLR_DVI3_INT_FLAG()          DVI3_INT_CLR = 0xFF

#define DDCDVI4_INTMASK_EN           INTMASK_EN_DVI4
#define DDCDVI4_INT_FLAG             DVI4_INT_FLAG
#define DDCDVI4_WBUF_RDP             DVI4_WBUF_RDP
#define DDCDVI4_RBUF_WDP             DVI4_RBUF_WDP
#define DDCDVI4_DDCCI_ID             DVI4_DDCCI_ID
#define DDCDVI4_INT_CLR              DVI4_INT_CLR
#define CLR_DVI4_INT_FLAG()          DVI4_INT_CLR = 0xFF

#define DDCDVI5_INTMASK_EN           INTMASK_EN_DVI5
#define DDCDVI5_INT_FLAG             DVI5_INT_FLAG
#define DDCDVI5_WBUF_RDP             DVI5_WBUF_RDP
#define DDCDVI5_RBUF_WDP             DVI5_RBUF_WDP
#define DDCDVI5_DDCCI_ID             DVI5_DDCCI_ID
#define DDCDVI5_INT_CLR              DVI5_INT_CLR
#define CLR_DVI5_INT_FLAG()          DVI5_INT_CLR = 0xFF

#if D2B_FIFO_Mode
#define DDC_ADC_WP_READY()          (msRegs(REG_003EE6) |= BIT2)
#define DDC_DVI0_WP_READY()         (msRegs(REG_003EC8) |= BIT2)
#define DDC_DVI1_WP_READY()         (msRegs(REG_003ECE) |= BIT2)
#define DDC_DVI2_WP_READY()         (msRegs(REG_003EE0) |= BIT2)
#if (CHIP_ID==MST9U3)
#define DDC_DVI3_WP_READY()         (msRegs(REG_003EBC) |= BIT2)
#else
#define DDC_DVI3_WP_READY()         (msRegs(REG_003FC8) |= BIT2)
#endif
#define DDC_DVI4_WP_READY()         (msRegs(REG_003FCE) |= BIT2)
#define DDC_DVI5_WP_READY()         (msRegs(REG_003FE0) |= BIT2)
#else
#define DDC_ADC_WP_READY()          (g_u8NoUse = 0)//(_msRegs[REG_3E08] |= BIT7)
#define DDC_DVI0_WP_READY()         (g_u8NoUse = 0)//(_msRegs[REG_3E08] |= BIT5)
#define DDC_DVI1_WP_READY()         (g_u8NoUse = 0) //(_msRegs[REG_3E08] |= BIT4)
#define DDC_DVI2_WP_READY()         (g_u8NoUse = 0) //(_msRegs[REG_3E08] |= BIT3)
#define DDC_DVI3_WP_READY()         (g_u8NoUse = 0)//(msRegs[REG_003FC8] |= BIT2)
#define DDC_DVI4_WP_READY()         (g_u8NoUse = 0)//(msRegs[REG_003FCE] |= BIT2)
#define DDC_DVI5_WP_READY()         (g_u8NoUse = 0)//(msRegs[REG_003FE0] |= BIT2)
#endif

#if ENABLE_DP_INPUT
#define DDCDP_DDCCI_ID    DP_DDCCI_ID
#endif


// DDC2Bi
#define D2B_SET_ID_VGA(Enable)      (DDCADC_DDCCI_ID = (Enable)?(0xB7):(0))
#define D2B_INT_MASK_VGA()          (DDCADC_INTMASK_EN = BIT6 | BIT5 | BIT2 | BIT1 | BIT0)
#define D2B_SET_ID_DVI0(Enable)     (DDCDVI0_DDCCI_ID = (Enable)?(0xB7):(0))
#define D2B_INT_MASK_DVI0()         (DDCDVI0_INTMASK_EN = BIT6 | BIT5 | BIT2 | BIT1 | BIT0)
#define D2B_SET_ID_DVI1(Enable)     (DDCDVI1_DDCCI_ID = (Enable)?(0xB7):(0))
#define D2B_INT_MASK_DVI1()         (DDCDVI1_INTMASK_EN = BIT6 | BIT5 | BIT2 | BIT1 | BIT0)
#define D2B_SET_ID_DVI2(Enable)     (DDCDVI2_DDCCI_ID = (Enable)?(0xB7):(0))
#define D2B_INT_MASK_DVI2()         (DDCDVI2_INTMASK_EN = BIT6 | BIT5 | BIT2 | BIT1 | BIT0)
#define D2B_SET_ID_DVI3(Enable)     (DDCDVI3_DDCCI_ID = (Enable)?(0xB7):(0))
#define D2B_INT_MASK_DVI3()         (DDCDVI3_INTMASK_EN = BIT6 | BIT5 | BIT2 | BIT1 | BIT0)
#define D2B_SET_ID_DVI4(Enable)     (DDCDVI4_DDCCI_ID = (Enable)?(0xB7):(0))
#define D2B_INT_MASK_DVI4()         (DDCDVI4_INTMASK_EN = BIT6 | BIT5 | BIT2 | BIT1 | BIT0)
#define D2B_SET_ID_DVI5(Enable)     (DDCDVI5_DDCCI_ID = (Enable)?(0xB7):(0))
#define D2B_INT_MASK_DVI5()         (DDCDVI5_INTMASK_EN = BIT6 | BIT5 | BIT2 | BIT1 | BIT0)
#define D2B_INT_MASK_ALL()          (INTMASK_EN_ADC  = 0xFF, INTMASK_EN_DVI0 = 0xFF, INTMASK_EN_DVI1 = 0xFF, INTMASK_EN_DVI2 = 0xFF, INTMASK_EN_DVI3 = 0xFF, INTMASK_EN_DVI4 = 0xFF, INTMASK_EN_DVI5 = 0xFF)
#define D2B_SET_ID_DP(Enable)       (DDCDP_DDCCI_ID = (Enable)?(0xB7):(0))
//A0
#define D2B_SET_FIFOMODE_VGA(Enable)                        (msWriteByteMask( REG_003EE6, Enable, BIT0 )) //Enable XDATA FIFO to replace D2B Write/Read Buffer
#define D2B_SET_FIFOMODE_ENHANCE_VGA(EnhanceMode)           (msWriteByteMask( REG_003EE6, (EnhanceMode)<<1, BIT1 )) //0:Normal mode (default mode) 1:Enhance mode
#define D2B_SET_FIFOMODE_SIZE_VGA(FIFOSize_INDEX)           (msWriteByteMask( REG_003EE7, FIFOSize_INDEX, BIT2 | BIT1 | BIT0 )) //FiFo Size selection
#define D2B_SET_FIFOMODE_XDATAMAP_VGA(XdataAddressStart)    (msWrite2ByteMask(REG_003EE8, XdataAddressStart-XDATA_ADDR_START, 0x1FFF)) // DDC_xdata mapping address
#define D2B_SET_FIFOMODE_SIMPLIFIED_VGA(Enable)             (msWriteByteMask( REG_003EE7, Enable?0xF0:0x00, 0xF0 )) //FiFo Simplified mode
#if D2B_DOUBLE_FIFO_Mode
#define D2B_SET_DOUBLE_FIFOMODE_VGA(Enable)                 (msWriteByteMask( REG_003E93, Enable<<7, BIT7 ))
#define D2B_SET_FIFOMODE_TX_SIZE_VGA(FIFOSize_INDEX)        (msWriteByteMask( REG_003E93, FIFOSize_INDEX<<4, BIT6 | BIT5 | BIT4 )) //FiFo Size selection
#define D2B_SET_FIFOMODE_TX_XDATAMAP_VGA(XdataAddressStart) (msWrite2ByteMask(REG_003E96, XdataAddressStart-XDATA_ADDR_START, 0x1FFF)) // DDC_xdata mapping address
#endif
//D0
#define D2B_SET_FIFOMODE_D0(Enable)                         (msWriteByteMask( REG_003EC8, Enable, BIT0 )) //Enable XDATA FIFO to replace D2B Write/Read Buffer
#define D2B_SET_FIFOMODE_ENHANCE_D0(EnhanceMode)            (msWriteByteMask( REG_003EC8, (EnhanceMode)<<1, BIT1 )) //0:Normal mode (default mode) 1:Enhance mode
#define D2B_SET_FIFOMODE_SIZE_D0(FIFOSize_INDEX)            (msWriteByteMask( REG_003EC9, FIFOSize_INDEX, BIT2 | BIT1 | BIT0 )) //FiFo Size selection
#define D2B_SET_FIFOMODE_XDATAMAP_D0(XdataAddressStart)     (msWrite2ByteMask(REG_003ECA, XdataAddressStart-XDATA_ADDR_START, 0x1FFF)) // DDC_xdata mapping address
#define D2B_SET_FIFOMODE_SIMPLIFIED_D0(Enable)              (msWriteByteMask( REG_003EC9, Enable?0xF0:0x00, 0xF0 )) //FiFo Simplified mode
#if D2B_DOUBLE_FIFO_Mode
#define D2B_SET_DOUBLE_FIFOMODE_D0(Enable)                  (msWriteByteMask( REG_003E94, Enable<<3, BIT3 ))
#define D2B_SET_FIFOMODE_TX_SIZE_D0(FIFOSize_INDEX)         (msWriteByteMask( REG_003E94, FIFOSize_INDEX, BIT2 | BIT1 | BIT0 )) //FiFo Size selection
#define D2B_SET_FIFOMODE_TX_XDATAMAP_D0(XdataAddressStart)  (msWrite2ByteMask(REG_003E98, XdataAddressStart-XDATA_ADDR_START, 0x1FFF)) // DDC_xdata mapping address
#endif
//D1
#define D2B_SET_FIFOMODE_D1(Enable)                         (msWriteByteMask( REG_003ECE, Enable, BIT0 )) //Enable XDATA FIFO to replace D2B Write/Read Buffer
#define D2B_SET_FIFOMODE_ENHANCE_D1(EnhanceMode)            (msWriteByteMask( REG_003ECE, (EnhanceMode)<<1, BIT1 )) //0:Normal mode (default mode) 1:Enhance mode
#define D2B_SET_FIFOMODE_SIZE_D1(FIFOSize_INDEX)            (msWriteByteMask( REG_003ECF, FIFOSize_INDEX, BIT2 | BIT1 | BIT0 )) //FiFo Size selection
#define D2B_SET_FIFOMODE_XDATAMAP_D1(XdataAddressStart)     (msWrite2ByteMask(REG_003ED0, XdataAddressStart-XDATA_ADDR_START, 0x1FFF)) // DDC_xdata mapping address
#define D2B_SET_FIFOMODE_SIMPLIFIED_D1(Enable)              (msWriteByteMask( REG_003ECF, Enable?0xF0:0x00, 0xF0 )) //FiFo Simplified mode
#if D2B_DOUBLE_FIFO_Mode
#define D2B_SET_DOUBLE_FIFOMODE_D1(Enable)                  (msWriteByteMask( REG_003E94, Enable<<7, BIT7 ))
#define D2B_SET_FIFOMODE_TX_SIZE_D1(FIFOSize_INDEX)         (msWriteByteMask( REG_003E94, FIFOSize_INDEX<<4, BIT6 | BIT5 | BIT4 )) //FiFo Size selection
#define D2B_SET_FIFOMODE_TX_XDATAMAP_D1(XdataAddressStart)  (msWrite2ByteMask(REG_003E9A, XdataAddressStart-XDATA_ADDR_START, 0x1FFF)) // DDC_xdata mapping address
#endif
//D2
#define D2B_SET_FIFOMODE_D2(Enable)                         (msWriteByteMask( REG_003EE0, Enable, BIT0 )) //Enable XDATA FIFO to replace D2B Write/Read Buffer
#define D2B_SET_FIFOMODE_ENHANCE_D2(EnhanceMode)            (msWriteByteMask( REG_003EE0, (EnhanceMode)<<1, BIT1 )) //0:Normal mode (default mode) 1:Enhance mode
#define D2B_SET_FIFOMODE_SIZE_D2(FIFOSize_INDEX)            (msWriteByteMask( REG_003EE1, FIFOSize_INDEX, BIT2 | BIT1 | BIT0 )) //FiFo Size selection
#define D2B_SET_FIFOMODE_XDATAMAP_D2(XdataAddressStart)     (msWrite2ByteMask(REG_003EE2, XdataAddressStart-XDATA_ADDR_START, 0x1FFF)) // DDC_xdata mapping address
#define D2B_SET_FIFOMODE_SIMPLIFIED_D2(Enable)              (msWriteByteMask( REG_003EE1, Enable?0xF0:0x00, 0xF0 )) //FiFo Simplified mode
#if D2B_DOUBLE_FIFO_Mode
#define D2B_SET_DOUBLE_FIFOMODE_D2(Enable)                  (msWriteByteMask( REG_003E95, Enable<<3, BIT3 ))
#define D2B_SET_FIFOMODE_TX_SIZE_D2(FIFOSize_INDEX)         (msWriteByteMask( REG_003E95, FIFOSize_INDEX, BIT2 | BIT1 | BIT0 )) //FiFo Size selection
#define D2B_SET_FIFOMODE_TX_XDATAMAP_D2(XdataAddressStart)  (msWrite2ByteMask(REG_003E9C, XdataAddressStart-XDATA_ADDR_START, 0x1FFF)) // DDC_xdata mapping address
#endif
//D3
#if (CHIP_ID==MST9U3)
#define D2B_SET_FIFOMODE_D3(Enable)                         (msWriteByteMask( REG_003EBC, Enable, BIT0 )) //Enable XDATA FIFO to replace D2B Write/Read Buffer
#define D2B_SET_FIFOMODE_ENHANCE_D3(EnhanceMode)            (msWriteByteMask( REG_003EBC, (EnhanceMode)<<1, BIT1 )) //0:Normal mode (default mode) 1:Enhance mode
#define D2B_SET_FIFOMODE_SIZE_D3(FIFOSize_INDEX)            (msWriteByteMask( REG_003EBD, FIFOSize_INDEX, BIT2 | BIT1 | BIT0 )) //FiFo Size selection
#define D2B_SET_FIFOMODE_XDATAMAP_D3(XdataAddressStart)     (msWrite2ByteMask(REG_003EBE, XdataAddressStart-XDATA_ADDR_START, 0x1FFF)) // DDC_xdata mapping address
#define D2B_SET_FIFOMODE_SIMPLIFIED_D3(Enable)              (msWriteByteMask( REG_003EBD, Enable?0xF0:0x00, 0xF0 )) //FiFo Simplified mode
#else
#define D2B_SET_FIFOMODE_D3(Enable)                         (msWriteByteMask( REG_003FC8, Enable, BIT0 )) //Enable XDATA FIFO to replace D2B Write/Read Buffer
#define D2B_SET_FIFOMODE_ENHANCE_D3(EnhanceMode)            (msWriteByteMask( REG_003FC8, (EnhanceMode)<<1, BIT1 )) //0:Normal mode (default mode) 1:Enhance mode
#define D2B_SET_FIFOMODE_SIZE_D3(FIFOSize_INDEX)            (msWriteByteMask( REG_003FC9, FIFOSize_INDEX, BIT2 | BIT1 | BIT0 )) //FiFo Size selection
#define D2B_SET_FIFOMODE_XDATAMAP_D3(XdataAddressStart)     (msWrite2ByteMask(REG_003FCA, XdataAddressStart-XDATA_ADDR_START, 0x1FFF)) // DDC_xdata mapping address
#define D2B_SET_FIFOMODE_SIMPLIFIED_D3(Enable)              (msWriteByteMask( REG_003FC9, Enable?0xF0:0x00, 0xF0 )) //FiFo Simplified mode
#endif
#if D2B_DOUBLE_FIFO_Mode
#define D2B_SET_DOUBLE_FIFOMODE_D3(Enable)                  (msWriteByteMask( REG_003E95, Enable<<7, BIT7 ))
#define D2B_SET_FIFOMODE_TX_SIZE_D3(FIFOSize_INDEX)         (msWriteByteMask( REG_003E95, FIFOSize_INDEX<<4, BIT6 | BIT5 | BIT4 )) //FiFo Size selection
#define D2B_SET_FIFOMODE_TX_XDATAMAP_D3(XdataAddressStart)  (msWrite2ByteMask(REG_003E9E, XdataAddressStart-XDATA_ADDR_START, 0x1FFF)) // DDC_xdata mapping address
#endif
//D4
#define D2B_SET_FIFOMODE_D4(Enable)                         (msWriteByteMask( REG_003FCE, Enable, BIT0 )) //Enable XDATA FIFO to replace D2B Write/Read Buffer
#define D2B_SET_FIFOMODE_ENHANCE_D4(EnhanceMode)            (msWriteByteMask( REG_003FCE, (EnhanceMode)<<1, BIT1 )) //0:Normal mode (default mode) 1:Enhance mode
#define D2B_SET_FIFOMODE_SIZE_D4(FIFOSize_INDEX)            (msWriteByteMask( REG_003FCF, FIFOSize_INDEX, BIT2 | BIT1 | BIT0 )) //FiFo Size selection
#define D2B_SET_FIFOMODE_XDATAMAP_D4(XdataAddressStart)     (msWrite2ByteMask(REG_003FD0, XdataAddressStart-XDATA_ADDR_START, 0x1FFF)) // DDC_xdata mapping address
#define D2B_SET_FIFOMODE_SIMPLIFIED_D4(Enable)              (msWriteByteMask( REG_003FCF, Enable?0xF0:0x00, 0xF0 )) //FiFo Simplified mode

//D5
#define D2B_SET_FIFOMODE_D5(Enable)                         (msWriteByteMask( REG_003FE0, Enable, BIT0 )) //Enable XDATA FIFO to replace D2B Write/Read Buffer
#define D2B_SET_FIFOMODE_ENHANCE_D5(EnhanceMode)            (msWriteByteMask( REG_003FE0, (EnhanceMode)<<1, BIT1 )) //0:Normal mode (default mode) 1:Enhance mode
#define D2B_SET_FIFOMODE_SIZE_D5(FIFOSize_INDEX)            (msWriteByteMask( REG_003FE1, FIFOSize_INDEX, BIT2 | BIT1 | BIT0 )) //FiFo Size selection
#define D2B_SET_FIFOMODE_XDATAMAP_D5(XdataAddressStart)     (msWrite2ByteMask(REG_003FE2, XdataAddressStart-XDATA_ADDR_START, 0x1FFF)) // DDC_xdata mapping address
#define D2B_SET_FIFOMODE_SIMPLIFIED_D5(Enable)              (msWriteByteMask( REG_003FE1, Enable?0xF0:0x00, 0xF0 )) //FiFo Simplified mode

#if (CHIP_ID==MST9U3)
#define D2B_SET_D2B_HW_CheckSum(EnVGA, EnD3, EnD2, EnD1, EnD0)    (msWriteByteMask( REG_003EF4, (EnD3<<4)|(EnVGA<<3)|(EnD2<<2)|(EnD1<<1)|(EnD0), BIT4|BIT3|BIT2|BIT1|BIT0 ))
#else
#define D2B_SET_D2B_HW_CheckSum(EnVGA, EnD5, EnD4, EnD3, EnD2, EnD1, EnD0)    (msWriteByteMask( REG_003EF4, (EnVGA<<3)|(EnD2<<2)|(EnD1<<1)|(EnD0), BIT3|BIT2|BIT1|BIT0 ),msWriteByteMask( REG_003FF4, (EnD5<<2)|(EnD4<<1)|(EnD3), BIT2|BIT1|BIT0 ))
#endif

//#define D2B_FIFOMODE_INPUT2DDCPORT(InputPort)               (((g_InputPort[InputPort].eIPMux & MUX_ADC)?InputCombo_A0:(g_InputPort[InputPort].eIPMux-1)))
//#define D2B_FIFOMODE_DDCPORT2INPUT(ComboPort)               (ComboPort==InputCombo_A0?(Input_VGA):(ComboPort==InputCombo_D0?(Input_DVI^Input_HDMI^Input_Nothing):(ComboPort==InputCombo_D1?(Input_DVI2^Input_HDMI2^Input_Nothing):(Input_DVI3^Input_HDMI3^Input_Nothing))))

extern void DDC2Bi_InitRx( void );
extern void DDC2Bi_GetTxBuffer( void );
extern BYTE DDC2Bi_ComputeChecksum( BYTE count );
extern void DDC2Bi_MessageReady( void );
extern void DDC2Bi_Init( void );
extern void DDC2Bi_CommandHandler( void );
extern void DoDDCCI( BYTE Source );
extern void msDP_DDC2BI(void);
extern void ISR_DDC2Bi(void);
extern BYTE AlignControl( void );
extern Bool mdrv_DDC2BI_GetDPSendFlag(BYTE ucAuxLength, BYTE ucDDCIndex, BYTE *ucDDCBuffer);
extern void mdrv_DDC2BI_SetDPReceiveStatus(BYTE ucPortSelect, BYTE ucDDCLength, BYTE *ucDDCBuffer);
extern BYTE D2B_FIFOMODE_DDCPORT2INPUT(BYTE ComboPort);
#endif


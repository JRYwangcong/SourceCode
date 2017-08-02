///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   Mdrv_mhl.h
/// @author MStar Semiconductor Inc.
/// @brief  MHL driver Function
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "msEread.h"

#ifndef _MMHL_MHL_H_
#define _MMHL_MHL_H_

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define _BIT(a)                             (BIT0 << (a))
#define MASKBIT(a)                          (_BIT((1?a) +1) -_BIT((0?a)))
#define MASKSET(a, b, c)                    ((a &~c) | (b &c))
#define ABS_MINUS(a, b)                     ((a > b)? (a -b): (b -a))

#define MHL_SAR_VALTAGE_MIN_TARGET      20
#define MHL_SAR_VALTAGE_MAX_TARGET     31
#define MHL_CABLE_DETECT_LEVEL(a)           (a *255 /33)

#define CBUS_CDR_CODE_CALCULATE(a)          (a *64 /14.318)

#define CBUS_CDR_CLOCK_TARGET               20
#define CBUS_CLOCK_DETECT_LEVEL             (CBUS_CDR_CODE_CALCULATE(CBUS_CDR_CLOCK_TARGET))

#define CBUS_CDR_OFFSET_TARGET              5
#define CBUS_CLOCK_DETECT_OFFSET            (CBUS_CDR_CODE_CALCULATE(CBUS_CDR_OFFSET_TARGET) /10)

#define CBUS_CDR_CLOCK_CALCULATE(a)         (a *14.318 /64)

#define MHL_24BIT_VCO_LOOP_DIV2             48
#define MHL_PPMODE_VCO_LOOP_DIV2            36
#define MHL_PPMODE_VCO_LOOP_DIV4            36

#define MHL_VCO_CODE_CALCULATE(a)           (a *64 /14.318)

#define MHL_24BIT_VCO_THRESHOLD1            0x01E0
#define MHL_PPMODE_VCO_THRESHOLD1           0x0168
#define MHL_PPMODE_VCO_THRESHOLD2           0x02D0

#define MHL_CDR_STABLE_COUNTER              200
#define MHL_CDR_FORCE_COUNTER               20
#define MHL_CDR_STABLE_OFFSET               1
#define MHL_CDR_STABLE_THRESHOLD            (MHL_CDR_STABLE_COUNTER +MHL_CDR_STABLE_OFFSET)
#define MHL_CDR_FORCE_THRESHOLD             (MHL_CDR_STABLE_THRESHOLD +MHL_CDR_FORCE_COUNTER)

#define MHL_RX_XRAM_SIZE_HALF               128
#define MHL_RX_XRAM_SIZE_TOTAL              (MHL_RX_XRAM_SIZE_HALF *2)

#define MHL_RX_XRAM_ADDR_FIRST_PORT0        MHL_CBUS_SRAM_ADDRESS_PORT0
#define MHL_RX_XRAM_ADDR_SECOND_PORT0       (MHL_RX_XRAM_ADDR_FIRST_PORT0 +MHL_RX_XRAM_SIZE_HALF)

#define MHL_RX_XRAM_ADDR_FIRST_PORT1        MHL_CBUS_SRAM_ADDRESS_PORT1
#define MHL_RX_XRAM_ADDR_SECOND_PORT1       (MHL_RX_XRAM_ADDR_FIRST_PORT1 +MHL_RX_XRAM_SIZE_HALF)

#define MHL_RX_XRAM_ADDR_FIRST_PORT2        MHL_CBUS_SRAM_ADDRESS_PORT2
#define MHL_RX_XRAM_ADDR_SECOND_PORT2       (MHL_RX_XRAM_ADDR_FIRST_PORT2 +MHL_RX_XRAM_SIZE_HALF)

#define MHL_RX_XRAM_ADDR_FIRST_PORT3        MHL_CBUS_SRAM_ADDRESS_PORT3
#define MHL_RX_XRAM_ADDR_SECOND_PORT3       (MHL_RX_XRAM_ADDR_FIRST_PORT3 +MHL_RX_XRAM_SIZE_HALF)

#define MHL_RX_XRAM_ADDR_FIRST_PORT4        MHL_CBUS_SRAM_ADDRESS_PORT4
#define MHL_RX_XRAM_ADDR_SECOND_PORT4       (MHL_RX_XRAM_ADDR_FIRST_PORT4 +MHL_RX_XRAM_SIZE_HALF)

#define MHL_RX_XRAM_ADDR_FIRST_PORT5        MHL_CBUS_SRAM_ADDRESS_PORT5
#define MHL_RX_XRAM_ADDR_SECOND_PORT5       (MHL_RX_XRAM_ADDR_FIRST_PORT5 +MHL_RX_XRAM_SIZE_HALF)

#define MHL_RTERM_CONTROL_VALUE             (4 << 12)

#define MHL_B_CHANNEL_EQ_VALUE            \
	( (msEread_GetDataFromEfuse(EFUSE_0, 0x13C) & 0xC0) ? (msEread_GetDataFromEfuse(EFUSE_0, 0x13C) & 0x1F) : 11)

#define MHL_I_CONTROL_PD_VALUE              16
#define MHL_I_CONTROL_PFD_VALUE             2

#define MHL_RTERM_CONTROL_VALUE0           (MHL_RCONTROL_OVERWRITE_PORT0 << 12)
#define MHL_RTERM_CONTROL_VALUE1           (MHL_RCONTROL_OVERWRITE_PORT1 << 12)
#define MHL_RTERM_CONTROL_VALUE2           (MHL_RCONTROL_OVERWRITE_PORT2 << 12)
#define MHL_RTERM_CONTROL_VALUE3           (MHL_RCONTROL_OVERWRITE_PORT3 << 12)
#define MHL_RTERM_CONTROL_VALUE4           (MHL_RCONTROL_OVERWRITE_PORT4 << 12)
#define MHL_RTERM_CONTROL_VALUE5           (MHL_RCONTROL_OVERWRITE_PORT5 << 12)

#define MHL_CBUS_EFUSE_ADDRESS              0x138

#define MHL_CBUS_PD_1K_LOW_FLAG             BIT0
#define MHL_CBUS_PD_1K_HIGH_FLAG            BIT1
#define MHL_CBUS_PD_100K_LOW_FLAG           BIT1
#define MHL_CBUS_PD_100K_HIGH_FLAG          BIT2

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef enum
{
    RX_HDMI_RTERM = 0,    // HDMI mode
    RX_MHL_RTERM,    // MHL mode
    RX_RTERM_OFF,    // R-term off
} RXRtermControl_T;

enum MHL_CABLE_DETECT_FLAG_TYPE
{
    MHL_CABLE_DETECT_ENABLE0_FLAG = BIT0,
    MHL_CABLE_DETECT_ENABLE1_FLAG = BIT1,
    MHL_CABLE_DETECT_ENABLE2_FLAG = BIT2,
    MHL_CABLE_DETECT_ENABLE3_FLAG = BIT3,
    MHL_CABLE_DETECT_ENABLE4_FLAG = BIT4,
    MHL_CABLE_DETECT_ENABLE5_FLAG = BIT5,
};

enum MHL_CBUS_EFUSE_BIT_TYPE
{
    MHL_CBUS_EFUSE_PD_1K_HIGH = BIT0,
    MHL_CBUS_EFUSE_PD_1K_LOW = BIT1,
    MHL_CBUS_EFUSE_PD_100K_HIGH = BIT2,
    MHL_CBUS_EFUSE_PD_100K_LOW = BIT3,
    MHL_CBUS_EFUSE_TRIMMING = BIT7,
};

#define GET_MHL_CABLE_DETECT_ENABLE0()      (Bool)((ucCableDetectEnable & MHL_CABLE_DETECT_ENABLE0_FLAG) ?TRUE :FALSE)
#define SET_MHL_CABLE_DETECT_ENABLE0()      (ucCableDetectEnable |= MHL_CABLE_DETECT_ENABLE0_FLAG)
#define CLR_MHL_CABLE_DETECT_ENABLE0()      (ucCableDetectEnable &= ~MHL_CABLE_DETECT_ENABLE0_FLAG)

#define GET_MHL_CABLE_DETECT_ENABLE1()      (Bool)((ucCableDetectEnable & MHL_CABLE_DETECT_ENABLE1_FLAG) ?TRUE :FALSE)
#define SET_MHL_CABLE_DETECT_ENABLE1()      (ucCableDetectEnable |= MHL_CABLE_DETECT_ENABLE1_FLAG)
#define CLR_MHL_CABLE_DETECT_ENABLE1()      (ucCableDetectEnable &= ~MHL_CABLE_DETECT_ENABLE1_FLAG)

#define GET_MHL_CABLE_DETECT_ENABLE2()      (Bool)((ucCableDetectEnable & MHL_CABLE_DETECT_ENABLE2_FLAG) ?TRUE :FALSE)
#define SET_MHL_CABLE_DETECT_ENABLE2()      (ucCableDetectEnable |= MHL_CABLE_DETECT_ENABLE2_FLAG)
#define CLR_MHL_CABLE_DETECT_ENABLE2()      (ucCableDetectEnable &= ~MHL_CABLE_DETECT_ENABLE2_FLAG)

#define GET_MHL_CABLE_DETECT_ENABLE3()      (Bool)((ucCableDetectEnable & MHL_CABLE_DETECT_ENABLE3_FLAG) ?TRUE :FALSE)
#define SET_MHL_CABLE_DETECT_ENABLE3()      (ucCableDetectEnable |= MHL_CABLE_DETECT_ENABLE3_FLAG)
#define CLR_MHL_CABLE_DETECT_ENABLE3()      (ucCableDetectEnable &= ~MHL_CABLE_DETECT_ENABLE3_FLAG)

#define GET_MHL_CABLE_DETECT_ENABLE4()      (Bool)((ucCableDetectEnable & MHL_CABLE_DETECT_ENABLE4_FLAG) ?TRUE :FALSE)
#define SET_MHL_CABLE_DETECT_ENABLE4()      (ucCableDetectEnable |= MHL_CABLE_DETECT_ENABLE4_FLAG)
#define CLR_MHL_CABLE_DETECT_ENABLE4()      (ucCableDetectEnable &= ~MHL_CABLE_DETECT_ENABLE4_FLAG)

#define GET_MHL_CABLE_DETECT_ENABLE5()      (Bool)((ucCableDetectEnable & MHL_CABLE_DETECT_ENABLE5_FLAG) ?TRUE :FALSE)
#define SET_MHL_CABLE_DETECT_ENABLE5()      (ucCableDetectEnable |= MHL_CABLE_DETECT_ENABLE5_FLAG)
#define CLR_MHL_CABLE_DETECT_ENABLE5()      (ucCableDetectEnable &= ~MHL_CABLE_DETECT_ENABLE5_FLAG)

#define GET_MHL_CABLE_DETECT_ENABLE(a)      (Bool)((ucCableDetectEnable & _BIT(a)) ?TRUE :FALSE)
#define SET_MHL_CABLE_DETECT_ENABLE(a)      (ucCableDetectEnable |= _BIT(a))
#define CLR_MHL_CABLE_DETECT_ENABLE(a)      (ucCableDetectEnable &= ~(_BIT(a)))


extern BYTE ucCableDetectEnable;

//-------------------------------------------------------------------------------------------------
//  Function Prototype
//-------------------------------------------------------------------------------------------------
void mhal_mhl_EnableCbusInterrupt(Bool bEnableInterrupt);
Bool mhal_mhl_GetCbusInterruptFlag(void);
void mhal_mhl_LoadEDID(BYTE ucPortSelect, BYTE *pComboEDID);
void mhal_mhl_Initial(BYTE ucPortSelect, Bool bPowerIgnore, BYTE *pComboEDID, BYTE *pDevcap, BYTE ucVenderID);
void mhal_mhl_SetPullDownResistor(BYTE ucEfuseIndex);
Bool mhal_mhl_SAR_Detect(BYTE ucPortSelect);
Bool mhal_mhl_CableDetect(BYTE ucPortSelect);
Bool mhal_mhl_CbusConnectStatus(BYTE ucPortSelect);
Bool mhal_mhl_GetCbusBusyFlag(BYTE ucPortSelect);
void mhal_mhl_ComboRtermControl(BYTE ucPortSelect, RXRtermControl_T ucControl);
void mhal_mhl_RtermControlHWMode(BYTE ucPortSelect, Bool bRtermHW);
void mhal_mhl_CDRStableMonitor(BYTE ucPortSelect, Bool bDisplayReady);
Bool mhal_mhl_CbusWriteCommand(BYTE ucPortSelect, mhalCbusFifo_S *pdatabuf);
void mhal_mhl_CbusSetPathEnable(BYTE ucPortSelect, Bool bSetPathEn);
void mhal_mhl_SetRxXdataAddress(BYTE ucPortSelect, Bool bFirstBlock);
BYTE mhal_mhl_GetRxXdataValue(BYTE ucPortSelect, BYTE ucCommandAddress);
Bool mhal_mhl_GetCbusReceiveFlag(BYTE ucPortSelect);
Bool mhal_mhl_GetStucktoLowFlag(BYTE ucPortSelect);
Bool mhal_mhl_GetECbusStateChangeFlag(BYTE ucPortSelect);
Bool mhal_mhl_GetEMSCReceiveFlag(BYTE ucPortSelect);
BYTE mhal_mhl_GetEMSCSendStatus(BYTE ucPortSelect);
void mhal_mhl_CbusForceToStandby(BYTE ucPortSelect);
void mhal_mhl_ClockModeSwitchProc(BYTE ucPortSelect, Bool bPacketPixelMode);
void mhal_mhl_CablePlugProc(BYTE ucPortSelect);
void mhal_mhl_CableRemoveProc(BYTE ucPortSelect);
void mhal_mhl_CbusConnectProc(BYTE ucPortSelect);
void mhal_mhl_CbusStucktoLowProc(BYTE ucPortSelect);
void mhal_mhl_SourceChangeProc(BYTE ucDisplayIndex);
void mhal_mhl_PowerDownProc(BYTE ucPortSelect);
void mhal_mhl_PMStatePolling(BYTE ucPortSelect);
void mhal_mhl_CableDetectEnableControl(BYTE ucPortSelect, Bool bCableDetectEnable);
Bool mhal_mhl_CheckSignalStable(BYTE ucPortSelect);
BYTE mhal_mhl_GetECbusStatusFlag(BYTE ucPortSelect);
void mhal_mhl_ECbusStateChangeProc(BYTE ucPortSelect);
void mhal_mhl_SetMainLinkRate(BYTE ucPortSelect, BYTE ucLinkRate);
void mhal_mhl_ECbusEventProc(BYTE ucPortSelect, BYTE ucECbusEvent);
void mhal_mhl_GetEMSCReceiveData(BYTE ucPortSelect, BYTE *bReveiceEMSC);
void mhal_mhl_InsertEMSCSendData(BYTE ucPortSelect, BYTE ucLength, BYTE *bSendEMSC);
void mhal_mhl_CablePlugProcByInit(BYTE ucInputPort);
#endif //_MHAL_MHL_H_


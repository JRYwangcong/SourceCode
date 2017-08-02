#ifndef _MS_DAISY_CHAIN_H_
#define _MS_DAISY_CHAIN_H_

#ifdef _MS_DAISY_CHAIN_C_
#define INTERFACE
#else
#define INTERFACE extern
#endif

typedef struct
{
    U8  u8CorePowerPin;
    U8  u8ChipID;
    U8  u8StreamMode;
	U8  u8FreeSyncMode;
}ST_DC_HOST_FEATURE;

typedef enum
{
    CORE_POWER_PIN_LOW_ACTIVE  = 0x00,  // 1.2V core power pin set Low as ON.
    CORE_POWER_PIN_HIGH_ACTIVE = 0x01,  // 1.2V core power pin set High as ON.
}EN_DC_CORE_POWER_PIN;

typedef enum
{
    DP1_1_SST = 0x01,
    DP1_2_SST = 0x02,
    DP1_2_MST = 0x03,
}EN_DC_STREAM_MODE;

typedef enum
{
    SDM_PIN_GPIO06 = 0x01,
    SDM_PIN_GPIO04 = 0x02,
}EN_DC_SDM_PIN;

typedef enum
{
    SDM_AUTO_CTL = 0x01,
    SDM_MANUAL_CTL = 0x02,
}EN_DC_SDM_CTL;

typedef enum
{
    DRR_DISABLE = 0x01,
    DRR_ENABLE = 0x02,
}EN_DRR_CTL;

typedef enum
{
    OUTPUT_LOW = 0x01,
    OUTPUT_HIGH = 0x02,
}EN_DC_RX_HPD;

enum
{
    CORE_POWER_PIN_FIELD_FIRST_BIT = 0,
    CHIP_FIELD_FIRST_BIT = 4,           // 4~7
    STREAM_MODE_FIELD_FIRST_BIT = 8,    // 8~11
    FREESYNC_MODE_FIRST_BIT = 12,
};

#define DAISY_CHAIN_MST_CHECK_TIME  5 //unit: second
#define IS_MST_MODE(a)        ((a&BIT1) ? TRUE :((a&BIT0) ? FALSE : TRUE))

#define IsDC_SDMConnected()		((g_DCHostFeature.u8ChipID == MST9U3)?(!hwSDMDP4Pin_Pin):(!hwSDMDP1Pin_Pin))

#define DAISY_CHAIN_SDM_DECOUNCE_TIMES           0x3

INTERFACE BOOL g_SDMCommandFlagDetect;
INTERFACE BOOL g_SDMCommandFlagForce;

INTERFACE BOOL msDCWriteBit( WORD u16Reg, Bool bBit, BYTE u8BitPos );
INTERFACE BOOL msDCReadByte( WORD u16Reg, BYTE* pu8Data );
INTERFACE BOOL msDCWriteByte( WORD u16Reg, BYTE u8Val );
INTERFACE BOOL msDCWriteByteMask( WORD u16Reg, BYTE u8Value, BYTE u8Mask );
INTERFACE BOOL msDCRead2Byte( WORD u16RegLoAddr, WORD* pu16Data );
INTERFACE BOOL msDCWrite2Byte( WORD u16RegLoAddr, WORD u16Value );
INTERFACE BOOL msDCWrite2ByteMask( WORD u16RegLoAddr, WORD wValue , WORD ucMask );
INTERFACE BOOL msDCRead3Byte( WORD u16RegLoAddr, DWORD* pu32Data );
INTERFACE BOOL msDCWrite3Byte( WORD u16RegLoAddr, DWORD u32Value );
INTERFACE BOOL msDCWrite3ByteMask( WORD u16RegLoAddr, DWORD u32Value , DWORD u32Mask );
INTERFACE BOOL msDCRead4Byte( WORD u16RegLoAddr, DWORD* pu32Data );
INTERFACE BOOL msDCWrite4Byte( WORD u16RegLoAddr, DWORD u32Value );
INTERFACE BOOL msDCWrite4ByteMask( WORD u16RegLoAddr, DWORD u32Value , DWORD u32Mask );

INTERFACE void msDCInit( BOOL bFullInit, BYTE* pu8HDCP_KEY, BYTE* pu8HDCP_BKSV, BYTE *pu8EDID, WORD u16EDIDSize, ST_DC_HOST_FEATURE stDCHostFeature);
INTERFACE BOOL msDCSetCmdDcOff( BYTE u8SendData, BYTE* pu8ReplyData );
INTERFACE BOOL msDCSetCmdDPMS( BYTE u8SendData, BYTE* pu8ReplyData );
INTERFACE BOOL msDCSetCmdWakeup( BYTE u8SendData, BYTE* pu8ReplyData );
INTERFACE void SetDCSdmForward( void );
INTERFACE void msDCTimerHandler(void);
INTERFACE BOOL msDCSetCmdPatternGenOn( BYTE u8SendData, BYTE* pu8ReplyData );
INTERFACE BOOL msDCSetCmdPatternGenOff( BYTE u8SendData, BYTE* pu8ReplyData );
INTERFACE BOOL msDCSetCmdLongHPD( BYTE u8SendData, BYTE* pu8ReplyData );
INTERFACE BOOL msDCSetCmdForceReTrain( BYTE u8SendData, BYTE* pu8ReplyData );
INTERFACE BOOL msDCSetCmdSetSysMode( BYTE u8SendData, BYTE* pu8ReplyData );
INTERFACE BOOL msDCSetCmdCheckMstBusy( BYTE u8SendData, BYTE* pu8ReplyData );
INTERFACE BOOL msDCSetCmdGetSleepSel( BYTE u8SendData, BYTE* pu8ReplyData );
INTERFACE BOOL msDCSetCmdSetRxHPD( BYTE u8SendData, BYTE* pu8ReplyData );
INTERFACE BOOL msDCSetCmdMccsAckReady( BYTE u8SendData, BYTE* pu8ReplyData );
INTERFACE BOOL msDCSetCmdSetSdmWorkPin( BYTE u8SendData, BYTE* pu8ReplyData );
INTERFACE BOOL msDCSetCmdAutoSdmCtl( BYTE u8SendData, BYTE* pu8ReplyData );
INTERFACE BOOL msDCLoadEDID( BYTE* pu8EDID, WORD u16EDIDSize );
INTERFACE BOOL msDCLoadHDCP( BYTE* pu8Key, BYTE* pu8BKSV );
INTERFACE BOOL msDCCheckRxHDCP( void );
INTERFACE BOOL msDCSetDRRDPCD( BYTE u8SendData, BYTE* pu8ReplyData );
INTERFACE BOOL msDCSetCmdSwitchDPINOUT( BYTE u8SendData, BYTE* pu8ReplyData );
INTERFACE void msDCInit_DPRxHPD(void);
INTERFACE void msDCForceTx1Retrain(void);
INTERFACE BOOL msDCForwardGNDSDM(BYTE u8SendData, BYTE* pu8ReplyData );
INTERFACE void msDC_ForwardGNDSDM(void);

#undef INTERFACE

#endif


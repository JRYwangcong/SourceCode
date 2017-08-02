///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   Mhal_DPRx.h
/// @author MStar Semiconductor Inc.
/// @brief  DP Rx driver Function
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MHAL_DPRX_H_
#define _MHAL_DPRX_H_

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define _BIT(a)                             (BIT0 << (a))
#define MASKBIT(a)                          (_BIT((1?a) +1) -_BIT((0?a)))
#define MASKSET(a, b, c)                    ((a &~c) | (b &c))
#define ABS_MINUS(a, b)                     ((a > b)? (a -b): (b -a))

#define DP_HBR2_REG_OFFSET(a)               (a *0x600)
#define DP_PHY_REG_OFFSET(a)                (a *0x100)
#define DP_LINK_REG_OFFSET(a)               (a *0x600)
#define DP_HBR2_AUDIO_OFFSET(a)             (a +6)

#define DP_AUX_USE_XTAL                     1 // (1 : XTAL  , 0 : OSC)
#define DP_SUPPORT_HBR2                     1
#define DP_AUTO_EQ_ENABLE                   1
#define DP_DYNAMIC_PREEMPHASIS              1 // 1 == on / 0 == off
#define DP_ERROR_COUNT_FUNCTION             0 // 1 == on / 0 == off
#define DP_EQ_OFFSET_FUNCTION               1 // 1 == on / 0 == off
#define DP_EQ_OFFSET_VALUE                  2
#define DP_EQ_THRESHOLD                     25 // EQ threshold for DYNAMIC_PREEMPHASIS function
#define DP_SSC_ENABLE                       1
#define DP_FT_EQ_OFFSET_FUNCTION            0 // 1 == on / 0 == off
#define DP_FT_EQ_OFFSET_VALUE               2
#define DP_ENABLE_STABLE_COUNT              0 // 1 == on / 0 == off
#define DP_ERROR_COUNT_THRESHOLD            50 // Error count threshold for ENABLE_STABLE_COUNT function

#define DP_CHECK_SQUELCH_COUNTER            1

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
enum DP_PROGRAM_DPCD_TYPE
{
    DP_PROGRAM_DPCD_0 = 0,
    DP_PROGRAM_DPCD_1,
    DP_PROGRAM_DPCD_2,
    DP_PROGRAM_DPCD_3,
    DP_PROGRAM_DPCD_MASK,
};

enum DP_SPLIT_MODE_TYPE
{
    DP_SPLIT_MODE_1P = 0,
    DP_SPLIT_MODE_2P,
    DP_SPLIT_MODE_MASK,
};

enum DP_LANE_COUNT_TYPE
{
    DP_ONE_LANE = 1,
    DP_TWO_LANE = 2,
    DP_FOUR_LANE = 4,
};

typedef struct
{
    BYTE ucKmValue;
    BYTE ucKpValue;
    WORD usDivValue;
    DWORD ulMCodeValue;
    DWORD ulNCodeValue;
}stDPAudioInfo;

//-------------------------------------------------------------------------------------------------
//  Function Prototype
//-------------------------------------------------------------------------------------------------
void mhal_DP_AuxCommonSetting(BYTE ucDPEnableIndex);
void mhal_DP_SetInterruptMask(BYTE ucPortSelect);
void mhal_DP_Initial(BYTE ucPortSelect, BYTE ucHBR2Select);
void mhal_DP_ClockEnable(void);
void mhal_DP_EnableHPD(BYTE ucPortSelect);
void mhal_DP_HPDControl(BYTE ucPortSelect, Bool bSetHPD);
Bool mhal_DP_CableDetect(BYTE ucPortSelect);
void mhal_DP_EnableAuxInterrupt(Bool bEnableInterrupt);
Bool mhal_DP_GetAuxInterruptFlag(void);
Bool mhal_DP_CheckSquelch(BYTE ucPortSelect);
void mhal_DP_InsertGUIDValue(BYTE ucPortSelect, BYTE *ucGUIDQueue);
Bool mhal_DP_CheckCDRLock(BYTE ucPortSelect, BYTE ucHBR2Select);
void mhal_DP_SetFTSyntheizer(BYTE ucPortSelect, BYTE ucHBR2Select, BYTE ucRateType);
void mhal_DP_SetFTLaneCount(BYTE ucPortSelect, BYTE ucHBR2Select, BYTE ucLaneType);
void mhal_DP_FTScrambleEnable(BYTE ucPortSelect, BYTE ucHBR2Select, Bool bEnable);
void mhal_DP_FastTrainingEnable(BYTE ucPortSelect, BYTE ucHBR2Select, Bool bEnable);
void mhal_DP_FTCheckTimingPrepare(BYTE ucPortSelect, BYTE ucHBR2Select);
void mhal_DP_AutoEQEnable(BYTE ucPortSelect, BYTE ucHBR2Select, Bool bEnable);
Bool mhal_DP_FTAutoEQEnable(BYTE ucPortSelect, BYTE ucHBR2Select, Bool bEnable);
void mhal_DP_FTAutoEQCheckDown(BYTE ucPortSelect, BYTE ucHBR2Select);
Bool mhal_DP_SplitSwitch(BYTE ucHBR2Select, Bool bEnable);
void mhal_DP_UpdateMVIDValue(BYTE ucPortSelect, BYTE ucHBR2Select);
WORD mhal_DP_GetTimingPixelClock(BYTE ucPortSelect, BYTE ucHBR2Select);
WORD mhal_DP_GetTimingPixelClock_10KHz(BYTE ucPortSelect, BYTE ucHBR2Select);
Bool mhal_DP_GetTimingInformation(BYTE ucPortSelect, BYTE ucHBR2Select, WORD *usTimingInfo);
void mhal_DP_RegenTimingInformation(BYTE ucPortSelect, BYTE ucHBR2Select, WORD *usTimingInfo);
WORD mhal_DP_GetPacketStatus(BYTE ucPortSelect, BYTE ucHBR2Select);
BYTE mhal_DP_GetColorSpace(BYTE ucPortSelect, BYTE ucHBR2Select, WORD *usMISCInfo);
Bool mhal_DP_GetTrainingPatternFlag(BYTE ucPortSelect, BYTE ucHBR2Select);
Bool mhal_DP_CheckReceiveIdlePattern(BYTE ucPortSelect, BYTE ucHBR2Select);
Bool mhal_DP_CheckDPCDPowerState(BYTE ucPortSelect);
Bool mhal_DP_GetHDCPFailFlag(BYTE ucPortSelect, BYTE ucHBR2Select);
Bool mhal_DP_CheckLossCDRStatus(BYTE ucPortSelect, BYTE ucHBR2Select);
Bool mhal_DP_CheckInterlaneSkewStatus(BYTE ucPortSelect, BYTE ucHBR2Select);
Bool mhal_DP_GetMCCSReceiveFlag(BYTE ucPortSelect);
Bool mhal_DP_GetProgramDPCD0Flag(BYTE ucPortSelect);
Bool mhal_DP_GetProgramDPCD1Flag(BYTE ucPortSelect);
Bool mhal_DP_GetAuxReceiveFlag(BYTE ucPortSelect);
Bool mhal_DP_ProgramDPCD0Proc(BYTE ucPortSelect, BYTE *ucDataQueue);
Bool mhal_DP_ProgramDPCD1Proc(BYTE ucPortSelect, BYTE *ucDataQueue);

Bool mhal_DP_GetAUPLLBigChangeFlag(BYTE ucPortSelect, BYTE ucHBR2Select);
Bool mhal_DP_GetInterlaneSkewFlag(BYTE ucPortSelect, BYTE ucHBR2Select);
Bool mhal_DP_GetCDRLossLockFlag(BYTE ucPortSelect, BYTE ucHBR2Select);
Bool mhal_DP_GetVPLLBigChangeFlag(BYTE ucPortSelect, BYTE ucHBR2Select);
Bool mhal_DP_GetTrainingPattern1Flag(BYTE ucPortSelect, BYTE ucHBR2Select);
Bool mhal_DP_GetTrainingPattern2Flag(BYTE ucPortSelect, BYTE ucHBR2Select);
void mhal_DP_HDCPReset(BYTE ucPortSelect, BYTE ucHBR2Select);
void mhal_DP_SetGuardBand(BYTE ucPortSelect, BYTE ucHBR2Select, WORD usGuardBandValue);
void mhal_DP_AudioInitial(void);
void mhal_DP_AUPLLSetting(void);
Bool mhal_DP_CheckAudioAbsent(BYTE ucPortSelect, BYTE ucHBR2Select);
void mhal_DP_CheckAudioMuteEvent(BYTE ucPortSelect, BYTE ucHBR2Select);
Bool mhal_DP_CheckLossAlignment(BYTE ucPortSelect, BYTE ucHBR2Select);
Bool mhal_DP_CheckAUPLLBigChange(BYTE ucPortSelect, BYTE ucHBR2Select);
void mhal_DP_UpdateAudioMNCode(BYTE ucPortSelect, BYTE ucHBR2Select, DWORD ulAudioMCode, DWORD ulAudioNCode);
void mhal_DP_AUPLLBigChangeProc(BYTE ucPortSelect, BYTE ucHBR2Select);
void mhal_DP_ErrorCountEnable(BYTE ucPortSelect, BYTE ucHBR2Select, Bool bEnable);
Bool mhal_DP_GetErrorCountFlag(BYTE ucPortSelect, BYTE ucHBR2Select);
void mhal_DP_OverwriteInterlaceMode(BYTE ucPortSelect, BYTE ucHBR2Select, Bool bEnable);

#if(DP_HDCP2_FUNCTION_SUPPORT)
Bool mhal_DP_GetHDCP2AKEInitFlag(BYTE ucPortSelect);
Bool mhal_DP_GetHDCP2NoStoredKmFlag(BYTE ucPortSelect);
Bool mhal_DP_GetHDCP2StoredKmFlag(BYTE ucPortSelect);
Bool mhal_DP_GetHDCP2LCInitFlag(BYTE ucPortSelect);
Bool mhal_DP_GetHDCP2SKESendFlag(BYTE ucPortSelect);
Bool mhal_DP_GetHDCP2LinkIntegrityFailure(BYTE ucPortSelect);
Bool mhal_DP_HDCP2GetRxData(BYTE ucPortSelect, BYTE ucDataInfo, BYTE *pHDCPData);
Bool mhal_DP_HDCP2SetTxData(BYTE ucPortSelect, BYTE ucDataInfo, BYTE *pHDCPData);
void mhal_DP_HDCP2SetRxStatus(BYTE ucPortSelect, BYTE ucMaskIndex, Bool bEnable);

#endif

#endif //_MHAL_DPRX_H_


///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   Mhal_DPRx.h
/// @author MStar Semiconductor Inc.
/// @brief  DP Rx driver Function
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MHAL_DPRX_H_
#define _MHAL_DPRX_H_


#define GET_DP_RX_SYSYEM_STATE(a)         			((msReadByte(REG_DPRX_AUX_62_L+DP_PHY_REG_OFFSET(a))&BIT2) ? 1 : 0)


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define _BIT(a)                             (BIT0 << (a))
#define MASKBIT(a)                          (_BIT((1?a) +1) -_BIT((0?a)))
#define MASKSET(a, b, c)                    ((a &~c) | (b &c))
#define ABS_MINUS(a, b)                     ((a > b)? (a -b): (b -a))

#define DP_HBR2_REG_OFFSET(a)               (a *0x600)
#define DP_PHY_REG_OFFSET(a)                (a *0x100)
#define DPRX_PHY_REG_OFFSET(a)  	     (a *0x100)
#define DP_LINK_REG_OFFSET(a)               (a *0x600)
#define DP_HBR2_AUDIO_OFFSET(a)             (a +6)

#define DP_AUX_USE_XTAL                     1 // (1 : XTAL  , 0 : OSC)
#define DP_SUPPORT_HBR2                     1
#if(COMBO_DP_DAISY_CHAIN_SUPPORT)
#define DP_AUTO_EQ_ENABLE                   0
#else
#define DP_AUTO_EQ_ENABLE                   1
#endif
#define DP_AUTO_CR_ENABLE                       0
#define DP_AUTO_EQ_NEW_MODE                 1

#if(DP_AUTO_EQ_ENABLE)
#define DP_DYNAMIC_PREEMPHASIS              0 // 1 == on / 0 == off
#define DP_EQ_THRESHOLD                     25 // EQ threshold for DYNAMIC_PREEMPHASIS function

#define DP_ERROR_COUNT_FUNCTION             0 // 1 == on / 0 == off

#define DP_EQ_OFFSET_FUNCTION               0 // 1 == on / 0 == off
#define DP_EQ_OFFSET_VALUE                  2

#define DP_ENABLE_STABLE_COUNT              0 // 1 == on / 0 == off
#define DP_ERROR_COUNT_THRESHOLD            50 // Error count threshold for ENABLE_STABLE_COUNT function
#else
#define DP_DYNAMIC_PREEMPHASIS              0 // must be 0
#define DP_EQ_THRESHOLD                     25 // EQ threshold for DYNAMIC_PREEMPHASIS function

#define DP_ERROR_COUNT_FUNCTION             0 // must be 0

#define DP_EQ_OFFSET_FUNCTION               0 // must be 0
#define DP_EQ_OFFSET_VALUE                  2

#define DP_ENABLE_STABLE_COUNT              0 // must be 0
#define DP_ERROR_COUNT_THRESHOLD            50 // Error count threshold for ENABLE_STABLE_COUNT function
#endif

#define DP_FT_EQ_OFFSET_FUNCTION            0 // 1 == on / 0 == off
#define DP_FT_EQ_OFFSET_VALUE               2

#define DP_HW_ERROR_COUNT_FUNCTION          1 // 1 == on / 0 == off


#define DP_CHECK_SQUELCH_COUNTER            1

#define DP_AUX_CLOCK_ROSC_8M                0
#define DP_AUX_CLOCK_XTAL                   1

#define DP_AUX_CLOCK_SELECT                 DP_AUX_CLOCK_XTAL

#define DP_PROGRAM_DPCD0_ENABLE             1
#define DP_PROGRAM_DPCD1_ENABLE             0
#define DP_PROGRAM_DPCD2_ENABLE             0
#define DP_PROGRAM_DPCD3_ENABLE             0

#define DP_PROGRAM_DPCD0_ADDERSS            0x00030
#define DP_PROGRAM_DPCD1_ADDERSS            0x00030
#define DP_PROGRAM_DPCD2_ADDERSS            0x00030
#define DP_PROGRAM_DPCD3_ADDERSS            0x00030
//====================================================================
//		Sink DEVICE SPecific Field
//====================================================================
#define IEEE_OUI_FIRST            0x00
#define IEEE_OUI_SECOND         0x00
#define IEEE_OUI_THIRD            0x00

#define DEVICE_STRING_0          0x00
#define DEVICE_STRING_1 	   0x00	
#define DEVICE_STRING_2		   0x00	 
#define DEVICE_STRING_3 	   0x00	
#define DEVICE_STRING_4          0x00
#define DEVICE_STRING_5          0x00

#define HARDWARE_VERSION      0x00

#define FIRMWARE_MAJOR_VERSION      0x00
#define FIRMWARE_MINOR_VERSION      0x00


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

enum DP_Version
{
	E_DP_V11 = 0,
	E_DP_V12,
	E_DP_RESERVED
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
//void mhal_DP_AuxCommonSetting(BYTE ucDPEnableIndex);
//void mhal_DP_SetInterruptMask(BYTE ucPortSelect);
BYTE mhal_DP_GetDebugRegisterValue(DPRx_ID dprx_id, BYTE ucRegSelect);
//void mhal_DP_Initial(BYTE ucPortSelect, BYTE ucHBR2Select);
//void mhal_DP_ClockEnable(void);
void mhal_DP_HPDControl(DPRx_ID dprx_id, Bool bSetHPD);
Bool mhal_DP_CableDetect(DPRx_ID dprx_id);
//void mhal_DP_EnableAuxInterrupt(Bool bEnableInterrupt);
Bool mhal_DP_GetAuxInterruptFlag(void);
Bool mhal_DP_CheckSquelch(DPRx_ID dprx_id);
//void mhal_DP_InsertGUIDValue(BYTE ucPortSelect, BYTE *ucGUIDQueue);
Bool mhal_DP_CheckCDRLock(DPRx_ID dprx_id, BYTE ucHBR2Select);
//void mhal_DP_SetFTSyntheizer(BYTE ucPortSelect, BYTE ucHBR2Select, BYTE ucRateType);
void mhal_DP_SetFTLaneCount(DPRx_ID dprx_id, BYTE ucHBR2Select, BYTE ucLaneType);
void mhal_DP_FTScrambleEnable(DPRx_ID dprx_id, BYTE ucHBR2Select, Bool bEnable);
void mhal_DP_FastTrainingEnable(DPRx_ID dprx_id, BYTE ucHBR2Select, Bool bEnable);
void mhal_DP_FTCheckTimingPrepare(DPRx_ID dprx_id, BYTE ucHBR2Select);
void mhal_DP_AutoEQEnable(DPRx_ID dprx_id, BYTE ucHBR2Select, Bool bEnable);
Bool mhal_DP_FTAutoEQEnable(DPRx_ID dprx_id, BYTE ucHBR2Select, Bool bEnable);
void mhal_DP_FTAutoEQCheckDown(DPRx_ID dprx_id, BYTE ucHBR2Select);
Bool mhal_DP_SplitSwitch(BYTE ucHBR2Select, Bool bEnable);
//void mhal_DP_UpdateMVIDValue(BYTE ucPortSelect, BYTE ucHBR2Select);
WORD mhal_DP_GetTimingPixelClock(DPRx_ID dprx_id, BYTE ucHBR2Select);
WORD mhal_DP_GetTimingPixelClock_10KHz(DPRx_ID dprx_id, BYTE ucHBR2Select);
//Bool mhal_DP_GetTimingInformation(BYTE ucPortSelect, BYTE ucHBR2Select, WORD *usTimingInfo);
void mhal_DP_RegenTimingInformation(DPRx_ID dprx_id, BYTE ucHBR2Select, WORD *usTimingInfo);
WORD mhal_DP_GetPacketStatus(DPRx_ID dprx_id, BYTE ucHBR2Select);
BYTE mhal_DP_GetColorSpace(DPRx_ID dprx_id, BYTE ucHBR2Select, WORD *usMISCInfo);
void mhal_DP_GetMISC01(DPRx_ID dprx_id, BYTE ucHBR2Select, WORD *usMISCInfo);
//Bool mhal_DP_GetTrainingPatternFlag(BYTE ucPortSelect, BYTE ucHBR2Select);
Bool mhal_DP_CheckReceiveIdlePattern(DPRx_ID dprx_id, BYTE ucHBR2Select);
Bool mhal_DP_CheckDPCDPowerState(DPRx_ID dprx_id);
//Bool mhal_DP_GetHDCPFailFlag(BYTE ucPortSelect, BYTE ucHBR2Select);
//Bool mhal_DP_CheckLossCDRStatus(BYTE ucPortSelect, BYTE ucHBR2Select);
//Bool mhal_DP_CheckInterlaneSkewStatus(BYTE ucPortSelect, BYTE ucHBR2Select);
Bool mhal_DP_GetMCCSReceiveFlag(DPRx_ID dprx_id);
Bool mhal_DP_GetAuxReceiveFlag(DPRx_ID dprx_id);
Bool mhal_DP_ProgramDPCD0Proc(DPRx_ID dprx_id, BYTE *ucDataQueue);
Bool mhal_DP_ProgramDPCD1Proc(DPRx_ID dprx_id, BYTE *ucDataQueue);

Bool mhal_DP_GetAUPLLBigChangeFlag(DPRx_ID dprx_id, BYTE ucHBR2Select);
Bool mhal_DP_GetInterlaneSkewFlag(DPRx_ID dprx_id, BYTE ucHBR2Select);
Bool mhal_DP_GetCDRLossLockFlag(DPRx_ID dprx_id, BYTE ucHBR2Select);
Bool mhal_DP_GetVPLLBigChangeFlag(DPRx_ID dprx_id, BYTE ucHBR2Select);
Bool mhal_DP_GetTrainingPattern1Flag(DPRx_ID dprx_id, BYTE ucHBR2Select);
Bool mhal_DP_GetTrainingPattern2Flag(DPRx_ID dprx_id, BYTE ucHBR2Select);
//void mhal_DP_HDCPReset(BYTE ucPortSelect, BYTE ucHBR2Select);
void mhal_DP_SetGuardBand(DPRx_ID dprx_id, BYTE ucHBR2Select, WORD usGuardBandValue);
//void mhal_DP_AudioInitial(void);
extern void mhal_DP_AUPLLSetting(DPRx_ID dprx_id);
extern void mhal_DP_AudioSSCInit(void);
Bool mhal_DP_CheckAudioAbsent(DPRx_ID dprx_id, BYTE ucHBR2Select, WORD usPacketStatus);
void mhal_DP_CheckAudioMuteEvent(DPRx_ID dprx_id, BYTE ucHBR2Select);
Bool mhal_DP_CheckLossAlignment(DPRx_ID dprx_id, BYTE ucHBR2Select);
Bool mhal_DP_CheckAUPLLBigChange(DPRx_ID dprx_id, BYTE ucHBR2Select);
void mhal_DP_UpdateAudioMNCode(DPRx_ID dprx_id, BYTE ucHBR2Select, DWORD ulAudioMCode, DWORD ulAudioNCode);
void mhal_DP_AUPLLBigChangeProc(DPRx_ID dprx_id, BYTE ucHBR2Select);
//void mhal_DP_ErrorCountEnable(BYTE ucPortSelect, BYTE ucHBR2Select, Bool bEnable);
//Bool mhal_DP_GetErrorCountFlag(BYTE ucPortSelect, BYTE ucHBR2Select);
void mhal_DP_OverwriteInterlaceMode(DPRx_ID dprx_id, BYTE ucHBR2Select, Bool bEnable);
BYTE mhal_DP_CalAudioFreq(DPRx_ID dprx_id, BYTE ucHBR2Select);  
void mhal_DP_VersionSetting(DPRx_ID dprx_id, BYTE ucHBR2Select, BYTE ucVersion );
Bool mhal_DP_GetDRRFlag(DPRx_ID dprx_id, BYTE ucHBR2Select);
void mhal_DP_EnableDRRFunction(DPRx_ID dprx_id, BYTE ucHBR2Select, Bool bEnable);
Bool mhal_DP_GetMSAChgFlag(DPRx_ID dprx_id,BYTE ucHBR2Select);
void mhal_DP_ClrMSAChgFlag(DPRx_ID dprx_id,BYTE ucHBR2Select);
Bool mhal_DP_GetInterlaceFlag(DPRx_ID dprx_id, BYTE ucHBR2Select);
void mhal_DP_SetInterlaceInverse(DPRx_ID dprx_id, BYTE ucHBR2Select, Bool bEnable);
Bool mhal_DP_GetFieldFlag(DPRx_ID dprx_id, BYTE ucHBR2Select);
DWORD mhal_DP_GetFieldCount(DPRx_ID dprx_id, BYTE ucHBR2Select, Bool bFieldID);
//void mhal_DP_RegenTimingProtect(BYTE ucPortSelect, BYTE ucHBR2Select);


Bool mhal_DP_GetTimingInformation(DPRx_ID dprx_id, BYTE ucHBR2Select, WORD *usTimingInfo);
void mhal_DP_AudioMute(Bool bEnable);
//void mhal_DP_SetLearnMode(BYTE ucPortSelect, BYTE ucHBR2Select);
void mhal_DP_SetMSAMode(DPRx_ID dprx_id, BYTE ucHBR2Select); 
MSCombo_DP_AUDIO_FORMAT mhal_DP_IsAudioFmtPCM(DPRx_ID dprx_id,BYTE ucHBR2Select);
void mhal_DP_AudioInfo(DPRx_ID dprx_id, BYTE ucHBR2Select, BYTE *pPacketData);
Bool mhal_DP_GetTimingInformation(DPRx_ID dprx_id,BYTE ucHBR2Select, WORD *usTimingInfo);
void mhal_DP_GetVscSdp(DPRx_ID dprx_id, BYTE ucHBR2Select, WORD *usMISCInfo);

void  mhal_DPRx_MCUWritePMDPCD(DPRx_ID dprx_id,DWORD ulDPCDAddress , BYTE  ucDATA);
BYTE  mhal_DPRx_MCUReadPMDPCD(DPRx_ID dprx_id,DWORD ulDPCDAddress);
void  mhal_DPRx_MCUWriteNonPMDPCD(DPRx_ID dprx_id,DWORD ulDPCDAddress , BYTE  ucDATA);
BYTE  mhal_DPRx_MCUReadNonPMDPCD(DPRx_ID dprx_id,DWORD ulDPCDAddress);
DPRx_MODE  mhal_DPRx_GetOuputMode(DPRx_ID dprx_id);
#endif //_MHAL_DPRX_H_


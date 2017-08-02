///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   Mdrv_DPRx.h
/// @author MStar Semiconductor Inc.
/// @brief  DP Rx driver Function
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MDRV_DPRX_H_
#define _MDRV_DPRX_H_







//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
enum DP_FUNCTION_ENABLE_TYPE
{
    DP_FUNCTION_ENABLE_PORT0_FLAG = BIT0,
    DP_FUNCTION_ENABLE_PORT1_FLAG = BIT1,
    DP_FUNCTION_ENABLE_PORT2_FLAG = BIT2,
    DP_FUNCTION_ENABLE_PORT3_FLAG = BIT3,
    DP_FUNCTION_ENABLE_PORT4_FLAG = BIT4,
    DP_FUNCTION_ENABLE_PORT5_FLAG = BIT5,
    DP_FUNCTION_ENABLE_VGA_FLAG = BIT6,
};

enum DP_INPUT_SELECT_TYPE
{
    DP_INPUT_SELECT_PORT0 = 0,
#if(DP_IP_SUPPORT_TYPE == DP_IP_SUPPORT_1PORT)
    DP_INPUT_SELECT_PORT_END,
#endif
    DP_INPUT_SELECT_PORT1,
#if(DP_IP_SUPPORT_TYPE == DP_IP_SUPPORT_2PORT)
    DP_INPUT_SELECT_PORT_END,
#endif
    DP_INPUT_SELECT_PORT2,
#if(DP_IP_SUPPORT_TYPE == DP_IP_SUPPORT_3PORT)
    DP_INPUT_SELECT_PORT_END,
#endif
    DP_INPUT_SELECT_PORT3,
#if(DP_IP_SUPPORT_TYPE == DP_IP_SUPPORT_4PORT)
    DP_INPUT_SELECT_PORT_END,
#endif
    DP_INPUT_SELECT_PORT4,
#if(DP_IP_SUPPORT_TYPE == DP_IP_SUPPORT_5PORT)
    DP_INPUT_SELECT_PORT_END,
#endif
    DP_INPUT_SELECT_PORT5,
#if(DP_IP_SUPPORT_TYPE == DP_IP_SUPPORT_6PORT)
    DP_INPUT_SELECT_PORT_END,
#endif
    DP_INPUT_SELECT_MASK = DP_INPUT_SELECT_PORT_END,
    DP_INPUT_SELECT_NONE = DP_INPUT_SELECT_MASK,
};


typedef enum _DPRx_ID
{
	DPRx_ID_0 = 0x0,
	DPRx_ID_1 = 0x1,
	DPRx_ID_2 = 0x2,
	DPRx_ID_MAX,
}DPRx_ID;



enum DP_POWER_STATUS_TYPE
{
    DP_POWER_STATUS_NONE = 0,
    DP_POWER_STATUS_AC_ON,
    DP_POWER_STATUS_DC_ON,
    DP_POWER_STATUS_PS_ON,
    DP_POWER_STATUS_DC_OFF,
    DP_POWER_STATUS_POWER_SAVING,
};

enum DP_HBR2_ENGINE_TYPE
{
    DP_HBR2_ENGINE_0 = 0,
    DP_HBR2_ENGINE_1,
    DP_HBR2_ENGINE_2,
    DP_HBR2_ENGINE_3,
    DP_HBR2_ENGINE_NONE,
};

enum DP_CLEAR_EVENT_FLAG_TYPE
{
    DP_CLEAR_INITIAL_FLAG = 0,
    DP_CLEAR_CABLE_LOSS_FLAG,
};

enum DP_HANDLER_STATE_TYPE
{
    DP_STATE_INITIAL = 0,     // 0
    DP_STATE_RETRAIN =1,     // 1
    DP_STATE_STARTUP=2,     // 2
    DP_STATE_CKECKLOCK=3,   // 3
    DP_STATE_MEASURE=4,     // 4
    DP_STATE_NORMAL=5,      // 5
    DP_STATE_POWERSAVE=6,   // 6
    DP_STATE_DPIDLE=7,       // 7
};

enum DP_COLOR_FORMAT_TYPE
{
    DP_COLOR_FORMAT_RGB         = 0,     ///< HDMI RGB 444 Color Format
    DP_COLOR_FORMAT_YUV_422     = 1,     ///< HDMI YUV 422 Color Format
    DP_COLOR_FORMAT_YUV_444     = 2,     ///< HDMI YUV 444 Color Format
    DP_COLOR_FORMAT_YUV_420     = 3,     ///< HDMI YUV 420 Color Format
	DP_COLOR_FORMAT_YONLY         = 4,     ///< DP Y Only Color Format
	DP_COLOR_FORMAT_RAW            = 5,     ///< DP RAW Color Format
	DP_COLOR_FORMAT_RESERVED    = 6,     ///< Reserve
    DP_COLOR_FORMAT_DEFAULT     = DP_COLOR_FORMAT_RGB,///< Default setting
    DP_COLOR_FORMAT_UNKNOWN     = 15,    ///< Unknow Color Format
};

enum DP_COLOR_RANGE_TYPE
{
    DP_COLOR_RANGE_DEFAULT    = 0, //depend on video format;
    DP_COLOR_RANGE_LIMIT      = 1,
    DP_COLOR_RANGE_FULL       = 2,
    DP_COLOR_RANGE_RESERVED   = 3,
};

enum DP_COLORIMETRY_TYPE
{
    DP_COLORIMETRY_xvYCC601    = 0,    ///< HDMI xvYCC601 Colorimetry
    DP_COLORIMETRY_xvYCC709    = 1,    ///< HDMI xvYCC709 Colorimetry
    DP_COLORIMETRY_sYCC601     = 2,    ///< HDMI sYCC601 colorimetry
    DP_COLORIMETRY_ADOBEYCC601 = 3,    ///< HDMI Adobe YCC 601 colorimetry
    DP_COLORIMETRY_ADOBERBG    = 4,    ///< HDMI Adobe RGB colorimetry
    DP_COLORIMETRY_NONE        = 15,
};

enum DP_FAST_TRAINING_RATE_TYPE
{
    DP_FAST_TRAINING_RATE_HBR2 = 0,
    DP_FAST_TRAINING_RATE_HBR,
    DP_FAST_TRAINING_RATE_LBR,
    DP_FAST_TRAINING_RATE_MASK,
};

enum DP_FAST_TRAINING_LANE_TYPE
{
    DP_FAST_TRAINING_4LANE = 0,
    DP_FAST_TRAINING_2LANE,
    DP_FAST_TRAINING_1LANE,
    DP_FAST_TRAINING_LANE_MASK,
};

enum DP_SYSTEM_FLAG_TYPE
{
    DP_SYSTEM_CONFIG_FINISH_FLAG = BIT0,
    DP_SYSTEM_POWER_IGNORE_FLAG = BIT1,
};

enum DP_HDCP2_MESSAGE_ID_TYPE
{
    DP_HDCP2_MSG_ID_NONE = 0,
    DP_HDCP2_MSG_ID_AKE_INIT = 2,
    DP_HDCP2_MSG_ID_AKE_SEND_CERT = 3,
    DP_HDCP2_MSG_ID_AKE_NO_STORED_KM = 4,
    DP_HDCP2_MSG_ID_AKE_STORED_KM = 5,
    DP_HDCP2_MSG_ID_AKE_SEND_H_PRIME = 7,
    DP_HDCP2_MSG_ID_AKE_SEND_PAIRING_INFO = 8,
    DP_HDCP2_MSG_ID_LC_INIT = 9,
    DP_HDCP2_MSG_ID_LC_SEND_L_PRIME = 10,
    DP_HDCP2_MSG_ID_SKE_SEND_EKS = 11,
};

enum DP_HDCP2_INFORMATION_TYPE
{
    DP_HDCP2_INFO_NONE = 0,
    DP_HDCP2_INFO_AKE_RTX,
    DP_HDCP2_INFO_AKE_TXCAPS,
    DP_HDCP2_INFO_AKE_CERTRX,
    DP_HDCP2_INFO_AKE_RRX,
    DP_HDCP2_INFO_AKE_RXCAPS,
    DP_HDCP2_INFO_AKE_EKPUB_KM,
    DP_HDCP2_INFO_AKE_EKH_KM_WR,
    DP_HDCP2_INFO_AKE_M,
    DP_HDCP2_INFO_AKE_H_PRIME,
    DP_HDCP2_INFO_AKE_EKH_KM_RD,
    DP_HDCP2_INFO_LC_RN,
    DP_HDCP2_INFO_LC_L_PRIME,
    DP_HDCP2_INFO_SKE_EDKEY_KS,
    DP_HDCP2_INFO_SKE_RIV,
};

enum DP_HDCP2_RXSTATUS_TYPE
{
    DP_HDCP2_RXSTATUS_READY = BIT0,
    DP_HDCP2_RXSTATUS_H_PRIME_AVAILABLE = BIT1,
    DP_HDCP2_RXSTATUS_PAIRING_AVAILABLE = BIT2,
    DP_HDCP2_RXSTATUS_REAUTH_REQ = BIT3,
    DP_HDCP2_RXSTATUS_LINK_INTEGRITY_FAILURE = BIT4,
};


typedef enum _DPRx_MODE
{
	DPRX_BYPASS = 0x0,
	DPRX_2PMODE  = 0x1,
	DPRX_1PMODE = 0x2,
	DPRx_MODE_MAX,
	
}DPRx_MODE;
#define DP_SQUELCH_STABLE_TIME              1500 // ms
#define DP_FAST_TRAINING_CHECK_TIME         150 // ms
#define DP_FAST_TRAINING_LOCK_COUNT         5
#define DP_MEASURE_TIMING_COUNTER           5
#define DP_CHECK_TIMING_COUNTER             2
#define DP_CHECK_TIMING_QUEUE_SIZE          8
#define DP_STABLE_POLLING_QUEUE_SIZE        4

#define DP_AUX_COMMAND_MAX_LENGTH           16
#define DP_AUX_COMMAND_DDC_LENGTH           3

#define DP_HDCP2_AKE_RTX_SIZE               8
#define DP_HDCP2_AKE_TXCAPS_SIZE            3
#define DP_HDCP2_AKE_CERTRX_SIZE            522
#define DP_HDCP2_AKE_RRX_SIZE               8
#define DP_HDCP2_AKE_RXCAPS_SIZE            3
#define DP_HDCP2_AKE_EKPUB_KM_SIZE          128
#define DP_HDCP2_AKE_EKH_KM_WR_SIZE         16
#define DP_HDCP2_AKE_M_SIZE                 16
#define DP_HDCP2_AKE_H_PRIME_SIZE           32
#define DP_HDCP2_AKE_EKH_KM_RD_SIZE         16
#define DP_HDCP2_LC_RN_SIZE                 8
#define DP_HDCP2_LC_L_PRIME_SIZE            32
#define DP_HDCP2_SKE_EDKEY_KS_SIZE          16
#define DP_HDCP2_SKE_RIV_SIZE               8

#define DP_HDCP2_RX_QUEUE_SIZE              129
#define DP_HDCP2_TX_QUEUE_SIZE              32

#define GET_DP_MST_FUNCTION_ENABLE_PORT0()      (Bool)((ucDPMSTEnableIndex & DP_FUNCTION_ENABLE_PORT0_FLAG) ?TRUE :FALSE)
#define GET_DP_MST_FUNCTION_ENABLE_PORT1()      (Bool)((ucDPMSTEnableIndex & DP_FUNCTION_ENABLE_PORT1_FLAG) ?TRUE :FALSE)
#define GET_DP_MST_FUNCTION_ENABLE_PORT2()      (Bool)((ucDPMSTEnableIndex & DP_FUNCTION_ENABLE_PORT2_FLAG) ?TRUE :FALSE)
#define GET_DP_MST_FUNCTION_ENABLE_PORT3()      (Bool)((ucDPMSTEnableIndex & DP_FUNCTION_ENABLE_PORT3_FLAG) ?TRUE :FALSE)
#define GET_DP_MST_FUNCTION_ENABLE_PORT4()      (Bool)((ucDPMSTEnableIndex & DP_FUNCTION_ENABLE_PORT4_FLAG) ?TRUE :FALSE)
#define GET_DP_MST_FUNCTION_ENABLE_PORT5()      (Bool)((ucDPMSTEnableIndex & DP_FUNCTION_ENABLE_PORT5_FLAG) ?TRUE :FALSE)

#define GET_DP_MST_FUNCTION_ENABLE_PORT(a)      (Bool)((ucDPMSTEnableIndex & _BIT(a)) ?TRUE :FALSE)
#define GET_DP_MST_FUNCTION_ENABLE_INDEX()      (ucDPMSTEnableIndex)

#define GET_DP_FUNCTION_ENABLE_PORT0()      (Bool)((ucDPFunctionEnableIndex & DP_FUNCTION_ENABLE_PORT0_FLAG) ?TRUE :FALSE)
#define GET_DP_FUNCTION_ENABLE_PORT1()      (Bool)((ucDPFunctionEnableIndex & DP_FUNCTION_ENABLE_PORT1_FLAG) ?TRUE :FALSE)
#define GET_DP_FUNCTION_ENABLE_PORT2()      (Bool)((ucDPFunctionEnableIndex & DP_FUNCTION_ENABLE_PORT2_FLAG) ?TRUE :FALSE)
#define GET_DP_FUNCTION_ENABLE_PORT3()      (Bool)((ucDPFunctionEnableIndex & DP_FUNCTION_ENABLE_PORT3_FLAG) ?TRUE :FALSE)
#define GET_DP_FUNCTION_ENABLE_PORT4()      (Bool)((ucDPFunctionEnableIndex & DP_FUNCTION_ENABLE_PORT4_FLAG) ?TRUE :FALSE)
#define GET_DP_FUNCTION_ENABLE_PORT5()      (Bool)((ucDPFunctionEnableIndex & DP_FUNCTION_ENABLE_PORT5_FLAG) ?TRUE :FALSE)

#define GET_DP_FUNCTION_ENABLE_PORT(a)      (Bool)((ucDPFunctionEnableIndex & _BIT(a)) ?TRUE :FALSE)
#define GET_DP_FUNCTION_ENABLE_INDEX()      (ucDPFunctionEnableIndex)

#define GET_DP_DISPLAY_ENABLE_PORT0()       (Bool)((ucDPDisplayEnableIndex & DP_FUNCTION_ENABLE_PORT0_FLAG) ?TRUE :FALSE)
#define GET_DP_DISPLAY_ENABLE_PORT1()       (Bool)((ucDPDisplayEnableIndex & DP_FUNCTION_ENABLE_PORT1_FLAG) ?TRUE :FALSE)
#define GET_DP_DISPLAY_ENABLE_PORT2()       (Bool)((ucDPDisplayEnableIndex & DP_FUNCTION_ENABLE_PORT2_FLAG) ?TRUE :FALSE)
#define GET_DP_DISPLAY_ENABLE_PORT3()       (Bool)((ucDPDisplayEnableIndex & DP_FUNCTION_ENABLE_PORT3_FLAG) ?TRUE :FALSE)
#define GET_DP_DISPLAY_ENABLE_PORT4()       (Bool)((ucDPDisplayEnableIndex & DP_FUNCTION_ENABLE_PORT4_FLAG) ?TRUE :FALSE)
#define GET_DP_DISPLAY_ENABLE_PORT5()       (Bool)((ucDPDisplayEnableIndex & DP_FUNCTION_ENABLE_PORT5_FLAG) ?TRUE :FALSE)

#define GET_DP_DISPLAY_ENABLE_PORT(a)       (Bool)((ucDPDisplayEnableIndex & _BIT(a)) ?TRUE :FALSE)

#define GET_DP_SYSTEM_CONFIG_FINISH()       (Bool)((ucDPSystemIndex & DP_SYSTEM_CONFIG_FINISH_FLAG) ?TRUE :FALSE)
#define SET_DP_SYSTEM_CONFIG_FINISH()       (ucDPSystemIndex |= DP_SYSTEM_CONFIG_FINISH_FLAG)
#define CLR_DP_SYSTEM_CONFIG_FINISH()       (ucDPSystemIndex &= ~DP_SYSTEM_CONFIG_FINISH_FLAG)

#define GET_DP_SYSTEM_POWER_IGNORE()        (Bool)((ucDPSystemIndex & DP_SYSTEM_POWER_IGNORE_FLAG) ?TRUE :FALSE)
#define SET_DP_SYSTEM_POWER_IGNORE()        (ucDPSystemIndex |= DP_SYSTEM_POWER_IGNORE_FLAG)
#define CLR_DP_SYSTEM_POWER_IGNORE()        (ucDPSystemIndex &= ~DP_SYSTEM_POWER_IGNORE_FLAG)

#if 0
#define GET_DP_HDCP2_RX_MSG_ID_PORT(a)      (ucHDCP2RxMessageID[a])
#define SET_DP_HDCP2_RX_MSG_ID_PORT(a,b)    (ucHDCP2RxMessageID[a] = b)
#define CLR_DP_HDCP2_RX_MSG_ID_PORT(a)      (ucHDCP2RxMessageID[a] = 0)

#define GET_DP_HDCP2_TX_MSG_ID_PORT(a)      (ucHDCP2TxMessageID[a])
#define SET_DP_HDCP2_TX_MSG_ID_PORT(a,b)    (ucHDCP2TxMessageID[a] = b)
#define CLR_DP_HDCP2_TX_MSG_ID_PORT(a)      (ucHDCP2TxMessageID[a] = 0)

#define GET_DP_HDCP2_TX_QUEUE_ADDRESS()     (ucHDCP2TxQueue)
#define SET_DP_HDCP2_TX_QUEUE_ADDRESS(a)    (ucHDCP2TxQueue = a)
#endif
extern BYTE ucDPFunctionEnableIndex;
extern BYTE ucDPDisplayEnableIndex;
extern BYTE ucDPSystemIndex;
extern BYTE ucDPMSTEnableIndex;
#if 0
extern BYTE ucHDCP2RxMessageID[];
extern BYTE ucHDCP2TxMessageID[];
extern BYTE *ucHDCP2TxQueue;
#endif
//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef enum _DPAudioFreqType   
{
    DPAUD_FREQ_32K=32,
    DPAUD_FREQ_44K=44,
    DPAUD_FREQ_48K=48,
    DPAUD_FREQ_88K=88,
    DPAUD_FREQ_96K=96,
    DPAUD_FREQ_176K=176,
    DPAUD_FREQ_192K=192,
    DPAUD_FREQ_ERROR=0,
} DPAudioFreqType;

typedef enum
{
    //range [0000 ~ 1111] reserved for DP 3D spec
    E_DP_3D_INPUT_NO_STEREO         = 0x00, //0000
    E_DP_3D_INPUT_FRAME_SEQUENTIAL  = 0x01, //0001    /* page flipping */
    E_DP_3D_INPUT_STACK_FRAME       = 0x02, //0002    //Frame Packing
    E_DP_3D_INPUT_PIXEL_INTERLEAVED = 0x03, //003     /* line interleave */
    E_DP_3D_INPUT_SIDE_BY_SIDE_FULL = 0x04, //004     /* side by side full */
    E_DP_3D_RESERVED                = 0x05, //005, 101 ~ 1111
}E_DP_3D_INPUT_MODE;


typedef enum
{
    E_DP_PIXELINTERLEAVE_LLRR   = 0x00, //0000
    E_DP_PIXELINTERLEAVE_RRLL   = 0x01, //0001
    E_DP_PIXELINTERLEAVE_LRRL   = 0x02, //0002
    E_DP_PIXELINTERLEAVE_LRLR   = 0x03, //0003
    E_DP_PIXELINTERLEAVE_RLRL   = 0x04, //0004
    E_DP_SIDEBYSIDE_LR          = 0x05, //0005
    E_DP_SIDEBYSIDE_RL          = 0x06, //0005
    E_DP_3D_DATA_RESERVED
}E_DP_3D_DATA_MODE;


typedef struct
{
    BYTE CTSLevel;
    Bool bDPVPLLBIGChange;
    Bool bDPAUPLLBIGChange;
    WORD uwFastTrainingCntDown;
    WORD uwDPRxStableTimeout;
    Bool bDPAUXVALID;
    Bool bDPPowerDown;
    Bool bDPLoseCDRLockIRQ;
    Bool bDPInterlaneSkewIRQ;
    WORD uwSkewHPDCnt;
    WORD uwCDRHPDCnt;
    BYTE NoVscPktCnt;
    BYTE ucState;
    Bool bDPCDRLOCK;
    Bool bDPLANELOCK;
    BYTE TrainingP1Cnt;
    BYTE TrainingP0Cnt;
    BYTE bDPTrainingP1T;
    BYTE bDPTrainingP0T;
    WORD LockFailCnt;
    BYTE DPCheckModeCnt;
    WORD PowerSavingCnt;
    WORD uwDPHtotal;
    WORD uwDPVtotal;
    WORD uwDPHWidth;
    WORD uwDPVWidth;
    WORD uwDPHPWS;
    WORD uwDPVPWS;
    WORD uwDPHStart;
    WORD uwDPVStart;
    WORD uwDPPixel;
    Bool bDPHPol;
    Bool bDPVPol;
    BYTE ucDPColorFormat;
    BYTE ucDPRangeFormat;
    BYTE ucDPColorDepth;
    BYTE ucColorType;
    Bool bDPLock;
    Bool bDPInterlace;
    Bool bDPNoVideoStrm;
    Bool bDPNoAudioStrm;
    Bool bDPPowerSaving;
    //Bool    bAudioMute;
    Bool bDPSave;
    Bool bAupllRun;
    Bool bSquelchWaitStable;
    Bool bSquelchStable;
    E_DP_3D_INPUT_MODE DP_3DMode;
    E_DP_3D_DATA_MODE PAYLOAD;
    Bool bDPSplitEn;
    Bool bDPSplitEn_temp;
    BYTE bDPSplitEn_Cnt_Up;
    BYTE bDPSplitEn_Cnt_Down;
    Bool bDPSplitOnOff;
    BYTE ucPreState;
    DWORD DPAUM_OLD;
    DWORD DPAUN_OLD;
    BYTE DPMISC0;
    BYTE DPMISC1;
	Bool bDPGUID;
    WORD uwHDCPCnt;
    Bool bDPHDCPIRQ;
    Bool bAutoEQReset;
    WORD InputPacketStatus;
    BYTE bPacketTimer;
    Bool bAudioEnable;
//    Bool bDPCheckFlag;
//    WORD uwDPStableCount;
    BYTE DPAudioAdjustCnt;
    BYTE DPAudioFreq;
	Bool bCheckInterlaceToggle;	
	Bool bFieldToggle;
    Bool bInterlaceCheck;
    BYTE ucInterlaceCount;	
    Bool bDPTrainingFlag;
    #if  DP_NoInput_Audio	
    Bool bNoInputFlag;
    #endif
 }DP_INFO,*PDP_INFO;

//-------------------------------------------------------------------------------------------------
//  Function Prototype
//-------------------------------------------------------------------------------------------------
void mdrv_DP_SetConfiguration(BYTE ucHBR20EnableIndex, BYTE ucHBR21EnableIndex, BYTE ucDPEnableIndex, BYTE ucDisplayEnableIndex);
BYTE mdrv_DP_GetHBR2PortSelect(BYTE ucInputPort);
void mdrv_DP_Initial(void);
Bool mdrv_DP_GetAuxInterruptFlag(void);
void mdrv_DP_IsrAuxHandler(void);
void mdrv_DP_IsrHandler(void);
void mdrv_DP_Handler(void);
//void mdrv_DP_TimerHandler(void);
BYTE mdrv_DP_GetColorSpace(BYTE ucInputPort);
BYTE mdrv_DP_GetColorRange(BYTE ucInputPort);
BYTE mdrv_DP_GetColorimetry(BYTE ucInputPort);
BYTE mdrv_DP_GetColorDepthInfo(BYTE ucInputPort);
void mdrv_DP_Get_Misc(BYTE ucInputPort, BYTE *pPacketData);
Bool mdrv_DP_GetCableDetectPort(BYTE ucInputPort);
void mdrv_DP_ConfigAudioPort(BYTE ucInputPort, Bool bEnable);
Bool mdrv_DP_CheckAudioStatus(BYTE ucInputPort);
void mdrv_DP_SetGuardBand(BYTE ucInputPort, WORD usGuardBandValue);
void mdrv_DP_GetHVInformation(BYTE ucInputPort, WORD *usHTotalValue, WORD *usVTotalValue);
BYTE mdrv_DP_GetInterlaceFlag(BYTE ucInputPort);
Bool mdrv_DP_CheckIgnoreDPMS(BYTE ucInputPort);
Bool mdrv_DP_GetSplitFlag(BYTE ucInputPort);
Bool mdrv_DP_GetAstorPTGInterlacePatch(BYTE ucInputPort);
Bool mdrv_DP_GetAstorPTGInterlaceFieldPatch(BYTE ucInputPort);
Bool mdrv_DP_CheckSourceLock(BYTE ucInputPort);
void mdrv_DP_Set_HPD(BYTE ucInputPort, Bool bSetHPD);
void mdrv_DP_Set_Version(BYTE ucInputPort, BYTE ucVersion);
Bool mdrv_DP_GetDRRFlag(BYTE ucInputPort);
void mdrv_DP_EnableDRRFunction(BYTE ucInputPort, Bool bEnable);
WORD mdrv_DP_GetPixelClock(BYTE ucInputPort);
WORD mdrv_DP_GetPixelClock_10KHz(BYTE ucInputPort);
Bool mdrv_DP_GetMSAChgFlag(BYTE ucInputPort);
void mdrv_DP_ClrMSAChgFlag(BYTE ucInputPort);
Bool mdrv_DP_CheckDPLock(BYTE ucInputPort);
BYTE mdrv_DP_GetAudioFreq(BYTE ucInputPort);
#if  DP_NoInput_Audio	
void mdrv_DP_DPRxAudioFreeRun(BYTE ucInputPort);
#endif
#if(DP_HDCP2_FUNCTION_SUPPORT)
void mdrv_DP_SetHDCP2CallBackFunction(DP_HDCP2_CALL_BACK_FUNC pHDCP2RxFunc);
void mdrv_DP_HDCP2TxEventProc(BYTE ucPortSelect, BYTE ucMessageID, BYTE *pMessageData);
void mdrv_DP_HDCP2SetCertRx(BYTE *pHDCP2CertRx);
void mdrv_DP_HDCP2IsrHandler(void);
void mdrv_DP_HDCP2Handler(BYTE ucPortSelect);

#endif
MSCombo_DP_AUDIO_FORMAT  mdrv_DP_IsAudioFmtPCM(BYTE ucInputPort);
void  mdrv_DP_AudioInfo(BYTE ucInputPort, BYTE *pPacketData);


#endif //_MDRV_DPRX_H_


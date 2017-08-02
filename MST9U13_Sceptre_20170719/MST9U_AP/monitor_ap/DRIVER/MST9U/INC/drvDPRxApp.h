
#ifndef _DPRxApp_H_
#define _DPRxApp_H_


#if ENABLE_DP_INPUT
#include "combo_config.h"
#include "mapi_combo.h"

#ifndef _drvDPRXAPP_7_H_
#define _drvDPRXAPP_7_H_

#ifdef _DPRXAPP_C_
#define _DPRXAPP_DEC_
#else
#define _DPRXAPP_DEC_ extern
#endif

#define _DPDATATYPE_            xdata
#define _DPDATATYPEEDID_        xdata


#ifndef UNUSED //to avoid compile warnings...
    #define UNUSED(var) (void)((var) = (var))
#endif


// ***************    For Display Port CTS  *******************
#define DPCTS                   1
#define DPENABLEINTERLANESKEW   DPCTS
#define DPENABLELOSECDRLOCK     DPCTS
#define DPENABLEMCCS            1

#define  FullRate_Mode          0
#define  DP_NUM                 (DP_Port_Num)
#define  DP_COMBO 		6
#define  MSACnt                 2
#define  HBR2_OFFSET            0x600
#define  COMBO_PHY_OFFSET       0x100
#define  COMBO_LINK_OFFET       0x600


#define  DP_HDCP_Reset				    0	
#define DP_MCCS_NEW_MODE                    0

#define  EQ_OFFSET_FUNCTION     1 // 1 == on / 0 == off
#define  EQ_OFFSET_VALUE        2

#define  DYNAMIC_PREEMPHASIS    1 // 1 == on / 0 == off
#define  EQ_THRESHOLD           25 // EQ threshold for DYNAMIC_PREEMPHASIS function

#define  FT_EQ_OFFSET_FUNCTION  0 // 1 == on / 0 == off
#define  FT_EQ_OFFSET_VALUE     2

#define  LT_ERROR_COUNT_FUNCTION 0 // 1 == on / 0 == off

#define  ENABLE_STABLE_COUNT    1 // 1 == on / 0 == off
#define  ERROR_THRESHOLD        50 // Error count threshold for ENABLE_STABLE_COUNT function
#define  LOOPOUT_DPNOSIGNAL_SOLUTION         1


#define DP_AUX_COMMAND_MAX_LENGTH           16
#define DP_AUX_COMMAND_DDC_LENGTH           3

#define DP_HBR2_REG_OFFSET(a)               (a *0x600)
#define DP_PHY_REG_OFFSET(a)                (a *0x100)
#define DP_LINK_REG_OFFSET(a)               (a *0x600)
#define DP_HBR2_AUDIO_OFFSET(a)             (a +6)

// ******************************************************
#define  DP_OUI							0
#define  DPSSC                      				1
#define  DP_NOVIDEO						1
#define  DisableDPSSC()                                (msWriteByteMask( REG_162F22, 0,BIT5 ))
#define  EnableDPSSC()                                 (msWriteByteMask( REG_162F22, BIT5,BIT5 ))

#define  DisableDPMute()                             (msWriteByteMask( REG_163088, 0,BIT4 ))								
#define  EnableDPMute()                              (msWriteByteMask( REG_163088, BIT4,BIT4))

#define DPRxSetLearnMode(port)			(msWriteByteMask( REG_113580+ port*HBR2_OFFSET,BIT0 ,BIT0|BIT1))					
#define DPRxSeMSAMode(port)				(msWriteByteMask( REG_113580+ port*HBR2_OFFSET,0 ,BIT0|BIT1))		

#define  mdrv_DP_GetInterruptFlag()           (_bit5_( MEM_MSREAD_BYTE(REG_002B39)))





#define XBYTE ((unsigned char volatile xdata *) 0)
#define mStar_ReadByte2(addr)           XBYTE[addr]
#define mStar_WriteByte2(addr, value)   XBYTE[addr] = value

#define HW_SET_AUDIO_MUTE_ENABLE()      //(MEM_MSWRITE_BIT(REG_163088, TRUE, BIT4))
#define HW_SET_AUDIO_MUTE_DISABLE()     //(MEM_MSWRITE_BIT(REG_163088, FALSE, BIT4))
//====================================================================
//		Sink DEVICE SPecific Field
//====================================================================

#define IEEE_OUI_FIRST             0x00
#define IEEE_OUI_SECOND          0x00
#define IEEE_OUI_THIRD             0x00

#define DEVICE_STRING_0          0x00
#define DEVICE_STRING_1 	   0x00	
#define DEVICE_STRING_2		   0x00	 
#define DEVICE_STRING_3 	   0x00	
#define DEVICE_STRING_4          0x00
#define DEVICE_STRING_5          0x00

#define HARDWARE_VERSION      0x00

#define FIRMWARE_MAJOR_VERSION      0x00
#define FIRMWARE_MINOR_VERSION      0x00

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
    DP_AUDIO_FMT_NON_PCM   = 0,
    DP_AUDIO_FMT_PCM       = 1,
    DP_AUDIO_FMT_UNKNOWN   = 2,
} MSCombo_DP_AUDIO_FORMAT;

typedef enum
{
	E_DP_V11 = 0,
	E_DP_V12,
	E_DP_RESERVED
} DPVersion;


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

enum
{
    eSTATE_INITIAL,     // 0
    eSTATE_RETRAIN,     // 1
    eSTATE_STARTUP,     // 2
    eSTATE_CKECKLOCK,   // 3
    eSTATE_MEASURE,     // 4
    eSTATE_NORMAL,      // 5
    eSTATE_POWERSAVE,   // 6
    eSTATE_DPIDLE,       // 7
    eSTATE_VIDEOMUTE     //8
};


enum
{
    BT601   = 0x0, //0000
    BT709,   
};

typedef enum
{
    DP_LBR  = 0x00, //0000
    DP_HBR  = 0x01, //0001
    DP_HBR2 = 0x02, // 0002
}E_DP_Rate;

typedef enum
{
    DP_1P   = 0x00, //0000
    DP_2P   = 0x01, //0001
}E_DP_OP;


typedef enum
{
    HBR2_PORT0 =0,
    HBR2_PORT1,
    HBR2_PORT2,
    HBR2_PORT3,
    HBR2_PORT4,
    HBR2_PORT5,
    HBR2_NONUSE
}HBR2_IP_SELECT;

typedef enum
{
    HBR_PORT0 =0,
    HBR_PORT1,
    HBR_PORT2,
    HBR_PORT3,
    HBR_PORT4,
    HBR_PORT5,
    HBR_NONUSE
}HBR_IP_SELECT;


typedef enum
{
    HBR2_0 =0 ,
    HBR2_1,

}HBR2_SELECT;



typedef enum
{
    Combo_PORT0 =0,
    Combo_PORT1,
    Combo_PORT2,
    Combo_PORT3,
    Combo_PORT4,
    Combo_PORT5,
    Combo_NONUSE
}Combo_IP_SELECT;


typedef enum
{
      DP_COLOR_DEPTH_6_BIT  = 0,
      DP_COLOR_DEPTH_8_BIT  = 1,
      DP_COLOR_DEPTH_10_BIT = 2,
      DP_COLOR_DEPTH_12_BIT = 3,
      DP_COLOR_DEPTH_16_BIT = 4,
      DP_COLOR_DEPTH_UNKNOWN = 5,
} DP_COLOR_DEPTH_FORMAT;


typedef enum
{
    InputColor_RGB=0,
    InputColor_YPBPR_422,
    InputColor_YPBPR_444,
    InputColor_Max
} DPColorType;

typedef enum
{
    InputRange_Default=0,
    InputRange_Limited,
    InputRange_Full,
    InputRange_Max
}DPRangeType;


#define DP_IP_SUPPORT_0PORT             0
#define DP_IP_SUPPORT_1PORT             1
#define DP_IP_SUPPORT_2PORT             2
#define DP_IP_SUPPORT_3PORT             3
#define DP_IP_SUPPORT_4PORT             4
#define DP_IP_SUPPORT_5PORT             5
#define DP_IP_SUPPORT_6PORT             6

#define DP_IP_SUPPORT_TYPE              COMBO_IP_SUPPORT_TYPE

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

enum DP_HBR2_ENGINE_TYPE
{
    DP_HBR2_ENGINE_0 = 0,
    DP_HBR2_ENGINE_1,
    DP_HBR2_ENGINE_2,
    DP_HBR2_ENGINE_3,
    DP_HBR2_ENGINE_NONE,
};

typedef struct
{
    BYTE    CTSLevel;
    Bool    bDPVPLLBIGChange;
    Bool    bDPAUPLLBIGChange;
    WORD    uwFastTrainingCntDown;
    WORD    uwDPRxStableTimeout;
    Bool    bDPAUXVALID;
    Bool    bDPPowerDown;
    Bool    bDPLoseCDRLockIRQ;
    Bool    bDPInterlaneSkewIRQ;
    WORD    uwSkewHPDCnt;
    WORD    uwCDRHPDCnt;
    BYTE    NoVscPktCnt;
    BYTE    ucState;
    Bool    bDPCDRLOCK;
    Bool    bDPLANELOCK;
    BYTE    TrainingP1Cnt;
    BYTE    TrainingP0Cnt;
    BYTE    bDPTrainingP1T;
    BYTE    bDPTrainingP0T;
    WORD    LockFailCnt;
    BYTE    DPCheckModeCnt;
    WORD    PowerSavingCnt;
    WORD    uwDPHtotal;
    WORD    uwDPVtotal;
	WORD uwDPVtotal_Temp;		////20160412
	WORD uwDPHtotal_Temp;		////20160412
    WORD    uwDPHWidth;
    WORD    uwDPVWidth;
    WORD    uwDPHPWS;
    WORD    uwDPVPWS;
    WORD    uwDPHStart;
    WORD    uwDPVStart;
    WORD    uwDPPixel;
    Bool    bDPHPol;
    Bool    bDPVPol;
    BYTE    ucDPColorFormat;
    Bool    bDPLock;
    Bool    bDPInterlace;
    Bool    bDPNoVideoStrm;
    Bool    bDPNoAudioStrm;
    Bool    bDPPowerSaving;
    //Bool    bAudioMute;
    Bool    bDPSave;
    Bool    bAupllRun;
    Bool    bSquelchWaitStable;
    Bool    bSquelchStable;
    E_DP_3D_INPUT_MODE  DP_3DMode;
    E_DP_3D_DATA_MODE   PAYLOAD;
    Bool    bDPSplitEn;
    Bool    bDPSplitEn_temp;
    BYTE    bDPSplitEn_Cnt_Up;
    BYTE    bDPSplitEn_Cnt_Down;
    Bool    bDPSplitOnOff;
    BYTE    DPState;
    DWORD   DPAUM_OLD;
    DWORD   DPAUN_OLD;
    BYTE	    DPAudioKM_OLD;
    BYTE	    DPAudioKP_OLD;
    DWORD  DPAudioFBDIV_OLD;	
    BYTE    DPMISC0;
    BYTE    DPMISC1;
	Bool           bDPGUID;
    WORD	 uwHDCPCnt;
    Bool	 bDPHDCPIRQ;
    Bool     bAutoEQReset;
    WORD	 InputPacketStatus;
    BYTE    bPacketTimer;
    BYTE     ucDPRangeFormat;  
    Bool      bColorType;	
    BYTE     ucDPColorDepth;
#if LOOPOUT_DPNOSIGNAL_SOLUTION
    WORD    uwReChkTiming;	// 140528 DMD Loop Out에서 간헐적 화면 안나옴 개선을 위한 Spot 솔루션 적용
    Bool 	bReChkEnable;	// 140528 DMD Loop Out에서 간헐적 화면 안나옴 개선을 위한 Spot 솔루션 적용
#endif
#if(ENABLE_STABLE_COUNT)
    Bool    bDPCheckFlag;
    BYTE    bDPDownSpeed;
    WORD    uwDPStableCount;
    BYTE    bDPLinkRate;
#endif
    BYTE   bDPAudioFreq;

#if(DP_NOVIDEO)
   Bool    bDPVBIDChk;
   BYTE   bDPVBIDmsCnt;
   Bool    bDPAudioSSCInit;
#endif   
  BYTE    ucDPNormalModeDelayCnt;
  Bool     bDPNormalModeFlag;
  Bool     bDPInterlaceFiledToggleFlag;
 }DP_INFO,*PDP_INFO;

typedef struct
{
	Bool AudioEn;
	
}DP_ComboInfo,*PDP_ComboInfo;


#if DPSSC   	
extern WORD DPRxAudioCnt;
extern WORD DPRxAudioSSCCnt;
extern WORD DPRxAudioSSCEn;
extern BYTE   DPAudio_State;
extern WORD   DPRxUnMuteCnt;
extern BYTE   DPRxAudioSSCEnCnt;
extern WORD DPRxPolling3Cnt;
extern BYTE   DPRxPolling3FIFOCnt;
extern WORD DPRxFreCtrl_Old;
extern WORD   DPRxCearCnt;
extern BYTE   DPRxCearCnt_On;
extern BYTE   DPFBDIV1_Old;
extern BYTE   DPFBDIV1_Cnt;
extern BYTE   DPAudioSampleCnt;
extern BYTE   DPAudioUnmute_Case1;
#endif
extern bool    DPRxAudioMuteEn;
extern BYTE  xdata ucDisplayEnableIndex_Temp;
extern WORD Aux;
extern DP_INFO xdata gDPInfo[DP_NUM];
extern DP_ComboInfo  gDPCombo[DP_COMBO];
extern BYTE DDCData[16];
extern BYTE  GUIDData[16];
extern WORD u16DPAudio_BWCnt;
//extern BYTE xdata ucDPVersion;

//========================================
extern void DPVersionSetting(BYTE enInputPort, BYTE bVer );
extern void DPRxHDCPLoadKey(void);
extern void DPRxHandle(void);
extern void DPRxInit(void);
extern void DPPrintState(BYTE state);
extern BOOL DPRxCheckLock(void);
extern BOOL DPRxCheckTiming(BYTE Count);
extern void DPRxTiming(void);
extern void DPRxVPLLHandle(void);
extern void DPRxCheckInputFormat(void);
extern void DPRx3DInform(void);
extern void DPRxCDRLossHander(void);
#if(ENABLE_STABLE_COUNT)
extern void DPRxErrorCountHander(void);
#endif
extern void DPRxHPDIRQ(void);
extern void DPRxClearAUXVliadStatus(void );
extern Bool DPRxGetSquelch( BYTE Times);
extern BOOL DPRxCheckSQTime(WORD Times);
extern void DPRxClrSQTime(void);
extern BOOL DPFastLink(void);
extern void DPRxSetSyntheizer( E_DP_Rate bRate, BYTE temp );
extern void DPRxSetFastLinkAEQ(BYTE Inputport, Bool bEnable);
extern void DPRxFastTraining2Enable(Bool bEnable);
extern void DPRxAudioHandle(void);
extern void DPRxFastTrainingInitial( BYTE temp );
extern void DPRxAUXInit(BYTE temp);
extern void DPRxEnSplit(E_DP_OP  output,  HBR2_SELECT temp);
extern void DPRxSetHBR2_1(void);
extern void DPRxSetHBR2_0(void);
//extern void DPRxComboInit(BYTE ucComboPort);
void DPRxIRQComboEnable( Bool bEnable, BYTE HBR2_Port );
void DPRxIRQAuxEnable( Bool bEnable, Combo_IP_SELECT Aux_Port );
extern void DPRxSetAUPLL(BYTE ucComboPort);
extern void DPRxHPDEnable(Bool bEnable,  Combo_IP_SELECT HPD_Port );
BYTE DPRxGetVB_ID(BYTE InputPort);
void DPRxAuxEnTx(BYTE InputPort);
void DPRxComboPort(void);
void DPRxSetAUXMask(BYTE temp);
BYTE DPRxPacketColor( void );
BYTE DPRxComboPortConvert(BYTE InputPort);
void DPAudioMuteCheck(void);
void DPRxResetAudioMN(void);
void DPRxProgramDPCD(BYTE temp);
void DPRxGUIDInit(BYTE temp, BYTE *GUIDPtr);
void DPRxProgramDPCD_ISR0(WORD uctemp);
void DPRxProgramDPCD_ISR1(WORD uctemp);
#if DP_HDCP_Reset
void DPRxProgramDPCD_ISR2(WORD uctemp);
#endif
void DPRxAutoEQ(Combo_IP_SELECT i);
void DPRxRterm(Combo_IP_SELECT Input);
void DPRxHdcpReAuthen(void);
void DPRxPHYPowerOnOff(Combo_IP_SELECT i,  bool  bEnable);
BOOL DPRXCheckDPMS(BYTE InputPort);
void DPRXAutoEQOnOff(Bool Enable, HBR2_SELECT HBR2PORT);
void DPRXGetPktStatus(void);
#if (DP_NOVIDEO == 1)
void DPRXGetAudioPktStatus(void);
BOOL DPRxVideoMuteStateCheck( void );
#endif
BYTE DPRxPacketRange(void);
extern void DPAudioSSCAdjust(void);
Bool DPRxAstroPTGInterlacePatch(BYTE InputPort);
Bool DPRxAstroPTGInterlaceFieldPatch(BYTE InputPort);
void DPRxCheckInterlaceInverse(BYTE InputPort);
void DPRxHVFSMCheck(BYTE InputPort);
BYTE DPRxPacketType(void);
BYTE DPRxColorDepth(void);
#if LOOPOUT_DPNOSIGNAL_SOLUTION
void  DPRxReChkTiming(void); 	// 140528 DMD Loop Out에서 간헐적 화면 안나옴 개선을 위한 Spot 솔루션 적용
#endif
Bool DPRxGetDRRFlag(BYTE InputPort);
void DPRxSpecificFiled(Combo_IP_SELECT i);
DWORD DPRxLongDIV(DWORD M_Value, WORD LingRate, DWORD N_Value);
extern void DPRxNoVideoCheck(void);

#if DPSSC
extern void DPAudioSSCInit(BYTE InputPort);
#endif
//=====================================

bool mapi_DP_GetDPNoVideoBit(BYTE ucInputPort);
bool mapi_DP_GetDPNoAudioBit(BYTE ucInputPort);
Bool mapi_DP_CheckAudioStatus(BYTE InputPort);
void mapi_DP_SetConfiguration(BYTE ucHBR20EnableIndex, BYTE ucHBR21EnableIndex, BYTE ucDPEnableIndex, BYTE ucDisplayEnableIndex );
void mapi_DP_TimerInterrupt(void);
void mapi_DP_PMIsrHandler(InterruptNum eIntNum);
void mapi_DP_IsrHandler(void);
void mapi_DP_Initial(void);
void mapi_DP_Handler(void);
BYTE mapi_DP_GetColorSpace(BYTE InputPort);
void mapi_DP_Get_Misc(BYTE ucInputPort, BYTE *pPacketData);
BYTE mapi_DP_GetCableDetectPort(BYTE ucInputPort);
void mapi_DP_SetGuardBand(BYTE ucInputPort, WORD usGuardBandValue);
void mapi_DP_GetHVInformation(BYTE ucInputPort, WORD *usHTotalValue, WORD *usVTotalValue);
Bool mapi_DP_GetInterlaceFlag(BYTE ucInputPort);
Bool mapi_DP_GetAstorPTGInterlacePatch(BYTE ucInputPort);
Bool mapi_DP_GetAstorPTGInterlaceFieldPatch(BYTE ucInputPort);
Bool mapi_DP_CheckIgnoreDPMS(BYTE ucInputPort);
Bool mapi_DP_GetSplitFlag(BYTE ucInputPort);
void  mapi_DP_ClrSplitFlag(BYTE ucInputPort);
void mapi_DP_Set_HPD(BYTE ucInputPort, Bool bSetHPD);
BYTE mapi_DP_GetColorDepthInfo(BYTE InputPort);
BYTE mapi_DP_ComboConvertHBR2(BYTE InputPort);
void  mapi_DP_ConfigAudioPort(BYTE InputPort , Bool  Enable);
BYTE mapi_DP_GetRangeFormat(BYTE InputPort);
BYTE mapi_DP_GetColorDepth(BYTE InputPort);
BYTE mapi_DP_GetColorType(BYTE InputPort);
Bool mapi_DP_GetMSAChgFlag(BYTE ucInputPort);
Bool mapi_DP_ClrMSAChgFlag(BYTE ucInputPort);
void  mapi_DP_PowerOnOff(BYTE ucInputPort,  Bool Enable);
//=====================================
void ISR_DPHBR2(HBR2_SELECT HBR2PORT);
void ISR_DPRxAuxISR(void);
ST_COMBO_COLOR_FORMAT mapi_DP_GetColorFormate(BYTE ucInputPort);
Bool mapi_DP_CheckSourceLock(BYTE ucInputPort);
void mapi_DP_Set_Version(BYTE ucTemp, BYTE ucDPVersion);
Bool mapi_DP_GetDRRFlag(BYTE ucInputPort);
void mapi_DP_EnableDRRFunction(BYTE ucInputPort, Bool bEnable);
WORD mapi_DP_GetPixelClock(BYTE ucInputPort);
WORD mapi_DP_GetPixelClock_10KHz(BYTE ucInputPort);
Bool mapi_DP_CheckDPLock(BYTE ucInputPort);
void mapi_DP_DaisychainFlag(BYTE ucDaisyChain);
extern void mapi_DPRxAudioFreeRun(BYTE ucComboPort);
extern BYTE mapi_DP_GetAudioFreq(BYTE ucInputPort);

extern MSCombo_DP_AUDIO_FORMAT mapi_DP_IsAudioFmtPCM(BYTE ucInputPort);
extern void  mapi_DP_AudioInfo(BYTE ucInputPort, BYTE *pPacketData);
extern void mapi_DP_SetPowerStatus(BYTE ucPowerStatus);
#if ENABLE_DP_ShortHPD_PM	 
extern void DPRxToggleShortHPD( BYTE InputPort);
extern void DPRxSet_ShortHPDFlag(BYTE ucInputPort, Bool bEnable);
extern BOOL DPRXCheckAUXValid(void);
#endif
BOOL  DPRxGetMSAFlagCheck(void);
BOOL  DPRxMSACheckDelay(void);
#endif
#endif
#endif

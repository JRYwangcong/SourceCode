///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   Mapi_DPRx.h
/// @author MStar Semiconductor Inc.
/// @brief  DP Rx driver Function
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MAPI_DPRX_H_
#define _MAPI_DPRX_H_

#include "combo_config.h"
#include "mapi_combo.h"

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define DP_DEBUG_MESSAGE                (1&ENABLE_MSTV_UART_DEBUG)

#define DP_IP_SUPPORT_0PORT             0
#define DP_IP_SUPPORT_1PORT             1
#define DP_IP_SUPPORT_2PORT             2
#define DP_IP_SUPPORT_3PORT             3
#define DP_IP_SUPPORT_4PORT             4
#define DP_IP_SUPPORT_5PORT             5
#define DP_IP_SUPPORT_6PORT             6

#define DP_IP_SUPPORT_TYPE              COMBO_IP_SUPPORT_TYPE

// HDCP 2.2
#define DP_HDCP2_FUNCTION_SUPPORT       COMBO_HDCP2_FUNCTION_SUPPORT

//-------------------------------------------------------------------------------------------------
//  Interface Link
//-------------------------------------------------------------------------------------------------
#define DP_CABLE_DETECT_PORT0           hwSDMDP0Pin_Pin
#define SET_DP_HPD_PORT0()              hw_Set_DPHpd0();
#define CLR_DP_HPD_PORT0()              hw_Clr_DPHpd0();
#define SET_DP_CABLE_5V_PORT0()         hw_Set_DPCable5V0();

#define DP_CABLE_DETECT_PORT1           hwSDMDP1Pin_Pin
#define SET_DP_HPD_PORT1()              hw_Set_DPHpd1();
#define CLR_DP_HPD_PORT1()              hw_Clr_DPHpd1();
#define SET_DP_CABLE_5V_PORT1()         hw_Set_DPCable5V1();

#define DP_CABLE_DETECT_PORT2           hwSDMDP2Pin_Pin
#define SET_DP_HPD_PORT2()              hw_Set_DPHpd2();
#define CLR_DP_HPD_PORT2()              hw_Clr_DPHpd2();
#define SET_DP_CABLE_5V_PORT2()         hw_Set_DPCable5V2();

#define DP_CABLE_DETECT_PORT3           hwSDMDP3Pin_Pin
#define SET_DP_HPD_PORT3()              hw_Set_DPHpd3();
#define CLR_DP_HPD_PORT3()              hw_Clr_DPHpd3();
#define SET_DP_CABLE_5V_PORT3()         hw_Set_DPCable5V3();

#define DP_CABLE_DETECT_PORT4           hwSDMDP4Pin_Pin
#define SET_DP_HPD_PORT4()              hw_Set_DPHpd4();
#define CLR_DP_HPD_PORT4()              hw_Clr_DPHpd4();
#define SET_DP_CABLE_5V_PORT4()         hw_Set_DPCable5V4();

#define DP_CABLE_DETECT_PORT5           hwSDMDP5Pin_Pin
#define SET_DP_HPD_PORT5()              hw_Set_DPHpd5();
#define CLR_DP_HPD_PORT5()              hw_Clr_DPHpd5();
#define SET_DP_CABLE_5V_PORT5()         hw_Set_DPCable5V5();

typedef enum
{
    DP_AUDIO_FMT_NON_PCM   = 0,
    DP_AUDIO_FMT_PCM       = 1,
    DP_AUDIO_FMT_UNKNOWN   = 2,
} MSCombo_DP_AUDIO_FORMAT;
//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef void (*DP_HDCP2_CALL_BACK_FUNC)(BYTE, BYTE*);

//-------------------------------------------------------------------------------------------------
//  Function Prototype
//-------------------------------------------------------------------------------------------------
MSCombo_DP_AUDIO_FORMAT  mapi_DP_IsAudioFmtPCM(BYTE ucInputPort);
void  mapi_DP_AudioInfo(BYTE ucInputPort, BYTE *pPacketData);
void mapi_DP_SetPowerStatus(BYTE ucPowerStatus);
void mapi_DP_SetConfiguration(BYTE ucHBR20EnableIndex, BYTE ucHBR21EnableIndex, BYTE ucDPEnableIndex, BYTE ucDisplayEnableIndex);
void mapi_DP_Initial(void);
void mapi_DP_PMIsrHandler(InterruptNum eIntNum);
void mapi_DP_IsrHandler(void);
void mapi_DP_Handler(void);
void mapi_DP_TimerInterrupt(void);
ST_COMBO_COLOR_FORMAT mapi_DP_GetColorFormate(BYTE ucInputPort);
BYTE mapi_DP_GetColorDepthInfo(BYTE ucInputPort);
void mapi_DP_Get_Misc(BYTE ucInputPort, BYTE *pPacketData);
Bool mapi_DP_GetCableDetectPort(BYTE ucInputPort);
BYTE mapi_DP_ComboConvertHBR2(BYTE ucInputPort);
void mapi_DP_ConfigAudioPort(BYTE ucInputPort, Bool bEnable);
Bool mapi_DP_CheckAudioStatus(BYTE ucInputPort);
void mapi_DP_SetGuardBand(BYTE ucInputPort, WORD usGuardBandValue);
void mapi_DP_GetHVInformation(BYTE ucInputPort, WORD *usHTotalValue, WORD *usVTotalValue);
Bool mapi_DP_GetInterlaceFlag(BYTE ucInputPort);
Bool mapi_DP_CheckIgnoreDPMS(BYTE ucInputPort);
Bool mapi_DP_GetSplitFlag(BYTE ucInputPort);
void mapi_DP_ClrSplitFlag(BYTE ucInputPort);
Bool mapi_DP_GetAstorPTGInterlacePatch(BYTE ucInputPort);
Bool mapi_DP_GetAstorPTGInterlaceFieldPatch(BYTE ucInputPort);
Bool mapi_DP_CheckSourceLock(BYTE ucInputPort);
void mapi_DP_Set_HPD(BYTE ucInputPort, Bool bSetHPD);
void mapi_DP_Set_Version(BYTE ucInputPort, BYTE ucVersion);
Bool mapi_DP_GetDRRFlag(BYTE ucInputPort);
void mapi_DP_EnableDRRFunction(BYTE ucInputPort, Bool bEnable);
WORD mapi_DP_GetPixelClock(BYTE ucInputPort);
WORD mapi_DP_GetPixelClock_10KHz(BYTE ucInputPort);
Bool mapi_DP_GetMSAChgFlag(BYTE ucInputPort);
void mapi_DP_ClrMSAChgFlag(BYTE ucInputPort);
Bool mapi_DP_CheckDPLock(BYTE ucInputPort);
BYTE mapi_DP_GetAudioFreq(BYTE ucInputPort);
void mapi_DP_PowerOnOff(BYTE ucInputPort, Bool Enable);
#if  DP_NoInput_Audio	
void mapi_DPRxAudioFreeRun(BYTE ucInputPort);
#endif
#if(DP_HDCP2_FUNCTION_SUPPORT)
void mapi_DP_SetHDCP2CallBackFunction(DP_HDCP2_CALL_BACK_FUNC pHDCP2RxFunc, BYTE *pHDCP2CertRx);
void mapi_DP_HDCP2TxEventProc(BYTE ucPortSelect, BYTE ucMessageID, BYTE ucDataSize, BYTE *pMessageData);
#endif

#endif //_MAPI_DPRX_H_


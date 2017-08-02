#ifndef _MAPI_TMDS_H_
#define _MAPI_TMDS_H_

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Function Prototype
//-------------------------------------------------------------------------------------------------
#ifdef _WORK_ON_PM_
void mapi_tmds_SetConfiguration(BYTE ucDVIEnableIndex, BYTE ucDualEnableIndex, BYTE ucHDMIEnableIndex, BYTE ucDisplayEnableIndex);
void mapi_tmds_Initial(void);
void mapi_tmds_ConfigAudioPort(BYTE ucInputPort);
void mapi_tmds_Handler(void);
void mapi_tmds_TimerInterrupt(void);
void mapi_tmds_Set_HPD(BYTE ucComboSelect, MS_BOOL bSetHPD);
ST_COMBO_COLOR_FORMAT mapi_tmds_Get_Color_Format(BYTE ucInputPort);
BYTE mapi_tmds_Input_Is_HDMI(BYTE ucInputPort);
void mapi_tmds_GetPacketContent(BYTE ucInputPort, EN_PACKET_DEFINE_TYPE ucPacketType, BYTE ucPacketLength, BYTE *pPacketData);
#endif
#if ENABLE_CABLE_DET
BYTE mapi_tmds_DetectCablePlug(BYTE ucInputPort);
#endif
#endif //_MAPI_TMDS_H_



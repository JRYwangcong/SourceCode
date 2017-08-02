
#ifndef _MHAL_DPTXAPP_H_
#define _MHAL_DPTXAPP_H_

//==============================================================================
//
//                              STRUCTURES
//
//==============================================================================

#define GET_DP_TX_SYSYEM_STATE()         			((msReadByte(REG_DPRX_AUX_62_H+ 0x100)&BIT6) ? 1 : 0)

void mhal_DPTx_VideoMute( DPTx_ID dpTx_ID, BOOL bENABLE);
void mhal_DPTx_AudioMute( DPTx_ID dpTx_ID, BOOL bENABLE);
void mhal_DPTx_SetOuputTiming(DPTx_ID dpTx_ID, WORD *TimingInfo);
void mhal_DPTx_EnableSwModeMSA(DPTx_ID dpTx_ID, BOOL bENABLE);

#endif//#ifndef _drvDPTXAPP_H_


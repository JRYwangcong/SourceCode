


#ifndef _MDRV_DPTX_H_
#define _MDRV_DPTX_H_

#define  TX_NUM     1 

typedef struct
{	
	bool bAudioMute;
	bool bvideoMute;
		
  } DPTX_INFO;





typedef enum {
	DPTx_ID_0 = 0x0,
	DPTx_ID_1 = 0x1,
	DPTX_ID_MAX,
}DPTx_ID;



enum{
    eDPTXSTATE_IDLE =0,  //0
    eDPTXSTATE_NORMAL=1, // 1
};

extern BYTE XDATA m_DPTXState[TX_NUM];
extern DPTX_INFO XDATA gDPTxInfo[TX_NUM];

extern void mdrv_DPTx_Handle(void);
extern void mdrv_DPTx_VideoMute(DPTx_ID dpTx_ID, BOOL bENABLE);
extern void mdrv_DPTx_AudioMute(DPTx_ID dpTx_ID, BOOL bENABLE);
extern void mdrv_DPTx_SetOuputTiming(DPTx_ID dpTx_ID, WORD *TimingInfo);
extern void mdrv_DPTx_EnableSwModeMSA(DPTx_ID dpTx_ID, BOOL bENABLE);

#endif//#ifndef _drvDPTXAPP_H_

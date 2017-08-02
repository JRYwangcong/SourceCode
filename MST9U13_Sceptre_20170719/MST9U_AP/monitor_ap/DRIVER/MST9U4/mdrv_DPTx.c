//******************************************************************************
//  Copyright (c) 2013-2018 MStar Semiconductor, Inc.
//  All rights reserved.
//
//  [Module Name]:
//      DPTxApp.c
//  [Abstract]:
//      This module contains code for DisplayPort Tx application level
//      procedure and subroutin
//  [Author(s)]:
//      Ben Lu
//  [Reversion History]:
//      Initial release:    06 October, 2013
//*******************************************************************************
#include "Global.h"
#include "Common.h"
#include <string.h>
#include "mdrv_DPTx.h"
#include "mhal_DPTx.h"

//==============================================================================
//
//                              VARIABLES
//
//==============================================================================
BYTE XDATA m_DPTXState[TX_NUM] ={0};
 DPTX_INFO XDATA gDPTxInfo[TX_NUM];

#define DPTX_DRV_DEBUG 1
#if (DPTX_DRV_DEBUG)
#define DPTX_DRV_printData(str, value)   	 printData(str, value)
#define DPTX_DRV_printMsg(str)                    printMsg(str)
#else
#define DPTX_DRV_printData(str, value)
#define DPTX_DRV_printMsg(str)
#endif

//**************************************************************************
//  [Function Name]:
//                  mdrv_DPTx_Handle()
//  [Description]
//                  mdrv_DPTx_Handle
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_DPTx_Handle(void)
{
	BYTE xdata i=0;
	WORD xdata  TimingInfo[8] ={0x578 , 0x44C , 0x4b , 0x26 , 0x500, 0x400 , 0x3C , 0x06}; 

		switch(m_DPTXState[i]) 
		{

			case eDPTXSTATE_IDLE:
				
				gDPTxInfo[i].bAudioMute=TRUE;
				gDPTxInfo[i].bvideoMute=TRUE;

				if(GET_DP_TX_SYSYEM_STATE())
			  	m_DPTXState[i] = eDPTXSTATE_NORMAL;			  	
				break;

			case eDPTXSTATE_NORMAL:
				 mdrv_DPTx_VideoMute(DPTx_ID_0, FALSE);
				 mdrv_DPTx_AudioMute(DPTx_ID_0, FALSE);
				 mdrv_DPTx_EnableSwModeMSA(DPTx_ID_0, TRUE);
				 mdrv_DPTx_SetOuputTiming( DPTx_ID_0, TimingInfo);									
				if(!GET_DP_TX_SYSYEM_STATE())
			  	{
			  		m_DPTXState[i] = eDPTXSTATE_IDLE;
				}
				
				break;


		}
	
	return;
}	




//**************************************************************************
//  [Function Name]:
//                  mdrv_DPTx_VideoMute()
//  [Description]
//                  mdrv_DPTx_VideoMute
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_DPTx_VideoMute( DPTx_ID dpTx_ID, BOOL bENABLE)
{

	 mhal_DPTx_VideoMute(dpTx_ID, bENABLE);

}
//**************************************************************************
//  [Function Name]:
//                  mdrv_DPTx_VideoMute()
//  [Description]
//                  mdrv_DPTx_VideoMute
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_DPTx_AudioMute( DPTx_ID dpTx_ID, BOOL bENABLE)
{

	 mhal_DPTx_AudioMute(dpTx_ID, bENABLE);

}
//**************************************************************************
//  [Function Name]:
//                  mdrv_DPTx_VideoMute()
//  [Description]
//                  mdrv_DPTx_VideoMute
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_DPTx_SetOuputTiming(DPTx_ID dpTx_ID, WORD *TimingInfo)
{
	mhal_DPTx_SetOuputTiming(dpTx_ID, TimingInfo);
	
}
//**************************************************************************
//  [Function Name]:
//                  mdrv_DPTx_EnableSwModeMSA()
//  [Description]
//                  mdrv_DPTx_EnableSwModeMSA
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_DPTx_EnableSwModeMSA(DPTx_ID dpTx_ID, BOOL bENABLE)
{
	mhal_DPTx_EnableSwModeMSA(dpTx_ID, bENABLE);

}


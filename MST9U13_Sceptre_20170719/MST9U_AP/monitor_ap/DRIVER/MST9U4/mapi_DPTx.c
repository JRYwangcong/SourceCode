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
//#include "mhal_DPTx.h"
//==============================================================================
//
//                              VARIABLES
//
//==============================================================================

#define DPTX_API_DEBUG 1
#if (DPTX_API_DEBUG)
#define DPTX_API_printData(str, value)   	 printData(str, value)
#define DPTX_API_printMsg(str)                    printMsg(str)
#else
#define DPTX_API_printData(str, value)
#define DPTX_API_printMsg(str)
#endif

//**************************************************************************
//  [Function Name]:
//                  mapi_DPTx_Handle()
//  [Description]
//                  mapi_DPTx_Handle
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************

void mapi_DPTx_Handle(void)
{
	 mdrv_DPTx_Handle();
	 return;
}


//**************************************************************************
//  [Function Name]:
//                  mapi_DPTx_VideoMute()
//  [Description]
//                  mapi_DPTx_VideoMute
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_DPTx_VideoMute( DPTx_ID dpTx_ID, BOOL bENABLE)
{

	 mdrv_DPTx_VideoMute(dpTx_ID, bENABLE);

}

//**************************************************************************
//  [Function Name]:
//                  mapi_DPTx_AudioMute()
//  [Description]
//                  mapi_DPTx_AudioMute
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_DPTx_AudioMute( DPTx_ID dpTx_ID, BOOL bENABLE)
{

	 mdrv_DPTx_AudioMute(dpTx_ID, bENABLE);

}

//**************************************************************************
//  [Function Name]:
//                  mapi_DPTx_SetOutputTiming()
//  [Description]
//                  mapi_DPTx_SetOutputTiming
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_DPTx_SetOuputTiming(DPTx_ID dpTx_ID, WORD *TimingInfo)
{

	 mdrv_DPTx_SetOuputTiming(dpTx_ID, TimingInfo);

}

//**************************************************************************
//  [Function Name]:
//                  mapi_DPTx_EnableSwModeMSA()
//  [Description]
//                  mapi_DPTx_EnableSwModeMSA
//  [Arguments]:
//				(bENABLE = 1 :  Overwrite mode 
//                                        = 0 :  Bypass mode
//  [Return]:
//
//**************************************************************************
void mapi_DPTx_EnableSwModeMSA(DPTx_ID dpTx_ID, BOOL bENABLE)
{
	mdrv_DPTx_EnableSwModeMSA(dpTx_ID, bENABLE);
}

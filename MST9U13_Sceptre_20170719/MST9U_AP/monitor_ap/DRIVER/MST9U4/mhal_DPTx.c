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
#ifndef _MHAL_DPTX_C_
#define _MHAL_DPTX_C_

#include "Global.h"
#include "Common.h"
#include "mdrv_DPTx.h"
#include "mhal_DPTx.h"
#include <string.h>

#if 1//(ENABLE_DP_OUTPUT)
//==============================================================================
//
//                              VARIABLES
//
//==============================================================================

#define DP_HAL_TX_DEBUG 1
#if (DP_HAL_TX_DEBUG)
#define DP_HAL_TX_printData(str, value)   	printData(str, value)
#define DP_HAL_TX_printMsg(str)                 printMsg(str)
#else
#define DP_HAL_TX_printData(str, value)
#define DP_HAL_TX_printMsg(str)
#endif


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


//**************************************************************************
//  [Function Name]:
//                  mhal_DPTx_VideoMute()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                
//**************************************************************************
void mhal_DPTx_VideoMute( DPTx_ID dpTx_ID, BOOL bENABLE)
{
	
	dpTx_ID =DPTx_ID_0;
	
	if(bENABLE)
	msWriteByteMask(REG_DPRX_ENCODER_E0_00_L + dpTx_ID, BIT2, BIT2); //Video mute enable
	else
	msWriteByteMask(REG_DPRX_ENCODER_E0_00_L + dpTx_ID, 0, BIT2); 
	return;
	
}


//**************************************************************************
//  [Function Name]:
//                  mhal_DPTx_AudioMute()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                
//**************************************************************************
void mhal_DPTx_AudioMute( DPTx_ID dpTx_ID, BOOL bENABLE)
{
	
	dpTx_ID =DPTx_ID_0;
	
	if(bENABLE)
	msWriteByteMask(REG_DPRX_ENCODER_E0_0C_H + dpTx_ID, BIT3, BIT3); //Video mute enable
	else
	msWriteByteMask(REG_DPRX_ENCODER_E0_0C_H + dpTx_ID, 0, BIT3); 
	return;
	
}
//**************************************************************************
//  [Function Name]:
//                  mhal_DPTx_SetOuputTiming()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                
//**************************************************************************
void mhal_DPTx_SetOuputTiming(DPTx_ID dpTx_ID, WORD *TimingInfo)
{
	dpTx_ID = DPTx_ID_0;
	//WORD xdata  test[8] ={0x578 , 0x44C , 0x4b , 0x26 , 0x500, 0x400 , 0x3C , 0x06}; 
	//memcpy(TimingInfo, test, sizeof(test));

//	TimingInfo[0]
	
	msWrite2Byte(REG_DPRX_ENCODER_E0_04_L, TimingInfo[0]);   //HTT
	msWrite2Byte(REG_DPRX_ENCODER_E0_05_L, TimingInfo[1]);   //VTT
	
	msWrite2Byte(REG_DPRX_ENCODER_E0_06_L, TimingInfo[2]);   //Hstar
	msWrite2Byte(REG_DPRX_ENCODER_E0_07_L, TimingInfo[3]);   //Vstar

	msWrite2Byte(REG_DPRX_ENCODER_E0_08_L, TimingInfo[4]);   //Hwidth
	msWrite2Byte(REG_DPRX_ENCODER_E0_09_L, TimingInfo[5]);   //Vwidth

	msWrite2Byte(REG_DPRX_ENCODER_E0_0A_L, TimingInfo[6] <<1);   //Hsw
	msWrite2Byte(REG_DPRX_ENCODER_E0_0B_L, TimingInfo[7] <<1);   //VSw

	
	
	
}


//**************************************************************************
//  [Function Name]:
//                  mhal_DPTx_SetOuputTiming()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                
//**************************************************************************
void mhal_DPTx_EnableSwModeMSA(DPTx_ID dpTx_ID, BOOL bENABLE)
{
	dpTx_ID =DPTx_ID_0;
	
	if(bENABLE)
	msWrite2ByteMask(REG_DPRX_ENCODER_E0_0C_L + dpTx_ID, 0x83FF, 0xFFFF); //Overwrite mode  SW mode
	else
	msWrite2ByteMask(REG_DPRX_ENCODER_E0_0C_L + dpTx_ID, 0, 0xFFFF); 	      // Bypass mode  (DP RX to Rx)
	return;
	
}






#endif //#if (ENABLE_DP_OUTPUT)
#endif //_MHAL_DPTX_C_

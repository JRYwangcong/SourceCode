///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    mdrv_DPRx.c
/// @author MStar Semiconductor Inc.
/// @brief  DP Rx driver Function
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MDRV_DPRX_C_
#define _MDRV_DPRX_C_

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "Global.h"
#include <string.h>
#include "mapi_DPRx.h"
#include "mdrv_DPRx.h"
#include "mhal_DPRx.h"
#include "Isr.h"
#if ENABLE_DAC
#include "drvAudio.h"
#endif
//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define DP_DRV_DEBUG_MESSAGE           	0
#define DP_AUX_DEBUG_ENABLE           	0

#if(DP_DEBUG_MESSAGE && DP_DRV_DEBUG_MESSAGE)
#define DP_DRV_DPUTSTR(str)            	printMsg(str)
#define DP_DRV_DPRINTF(str, x)         	printData(str, x)
#else
#define DP_DRV_DPUTSTR(str)
#define DP_DRV_DPRINTF(str, x)
#endif

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
DP_HDCP2_CALL_BACK_FUNC pmdrv_DP_HDCP2RxEventProc = NULL;

BYTE ucDPHBR20PortSelect = 0;
BYTE ucDPHBR21PortSelect = 0;
BYTE ucDPFunctionEnableIndex = 0;
BYTE ucDPDisplayEnableIndex = 0;
BYTE ucDPSystemIndex = 0;
BYTE ucHDCP2RxMessageID[DP_INPUT_SELECT_MASK] = {0};
BYTE ucHDCP2TxMessageID[DP_INPUT_SELECT_MASK] = {0};
BYTE *ucHDCP2TxQueue;
#if (COMBO_DP_DAISY_CHAIN_SUPPORT == 0x1)
BYTE ucGUIDData[DP_AUX_COMMAND_MAX_LENGTH] = {0x0};
#else
BYTE ucGUIDData[DP_AUX_COMMAND_MAX_LENGTH]={0x10,0xDE,0x90,0x70,0x00,0x00,0x00,0x02,0xC0,0x7C,0x4E,0x44,0x00,0x00,0x00,0x56};
#endif
BYTE ucDataQueue[DP_AUX_COMMAND_MAX_LENGTH] = {0};
BYTE ucHPulumQueue[DP_AUX_COMMAND_MAX_LENGTH] = {0};
 #if ENABLE_DP_ShortHPD_PM	
BOOL bHPDShortPulse[DP_INPUT_SELECT_MASK] = {FALSE};
#endif
BYTE ucDPCD_0x69494 = 0x0;
DP_INFO gDPInfo[DP_INPUT_SELECT_MASK];

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

//**************************************************************************
//  [Function Name]:
//                  _mdrv_DP_ClearEventFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_DP_ClearEventFlag(BYTE ucPortSelect, BYTE ucEventSelect)
{
    switch(ucEventSelect)
    {
        case DP_CLEAR_INITIAL_FLAG:
            gDPInfo[ucPortSelect].ucState = DP_STATE_STARTUP;
            gDPInfo[ucPortSelect].ucPreState = DP_STATE_INITIAL;
            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_DP_CheckInputTiming()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool _mdrv_DP_CheckInputTiming(BYTE ucPortSelect, BYTE ucHBR2Select)
{
    Bool bCheckFinish = FALSE;
    Bool bInterlace = FALSE;
    BYTE uctemp = 0;
    WORD usPixelClock = 0;
    WORD usVTotal = 0;
    WORD usVWidth = 0;
    WORD usTimingInfo[DP_CHECK_TIMING_QUEUE_SIZE] = {0};

    for(uctemp = 0; uctemp <DP_CHECK_TIMING_COUNTER; uctemp++)
    {
        usPixelClock = mhal_DP_GetTimingPixelClock(ucPortSelect, ucHBR2Select);
        bInterlace = mhal_DP_GetTimingInformation(ucPortSelect, ucHBR2Select, usTimingInfo);

        gDPInfo[ucPortSelect].bDPInterlace = bInterlace;
        gDPInfo[ucPortSelect].uwDPHtotal = usTimingInfo[0];
        gDPInfo[ucPortSelect].uwDPHWidth = usTimingInfo[2];
        gDPInfo[ucPortSelect].uwDPVtotal = usTimingInfo[1];
        gDPInfo[ucPortSelect].uwDPVWidth = usTimingInfo[3];

        if(bInterlace)
        {
            usVTotal = usTimingInfo[1] << 1;
            usVWidth = usTimingInfo[3] << 1;
        }
        else
        {
            usVTotal = usTimingInfo[1];
            usVWidth = usTimingInfo[3];
        }

        gDPInfo[ucPortSelect].uwDPHStart = usTimingInfo[4];
        gDPInfo[ucPortSelect].uwDPVStart = usTimingInfo[5];
        gDPInfo[ucPortSelect].uwDPHPWS = usTimingInfo[6] &0x7FFF;
        gDPInfo[ucPortSelect].bDPHPol = ((usTimingInfo[6] &BIT15) ?TRUE: FALSE);
        gDPInfo[ucPortSelect].uwDPVPWS = usTimingInfo[7] &0x7FFF;
        gDPInfo[ucPortSelect].bDPVPol = ((usTimingInfo[7] &BIT15) ?TRUE: FALSE);

        if((gDPInfo[ucPortSelect].uwDPHWidth < 4500) && (usVWidth < 2501) && (usPixelClock > 10) && (usPixelClock < 700)
            &&(gDPInfo[ucPortSelect].uwDPHtotal > gDPInfo[ucPortSelect].uwDPHWidth) && (usVTotal > usVWidth))
        {
            bCheckFinish = TRUE;
        }
        else
        {
            bCheckFinish = FALSE;

            break;
        }
    }

    return bCheckFinish;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_DP_ToggleHPD()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_DP_ToggleHPD(BYTE ucPortSelect)
{
    mhal_DP_HPDControl(ucPortSelect, FALSE);
    Delay1ms(5);
    mhal_DP_HPDControl(ucPortSelect, TRUE);
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_DP_ToggleShortHPD()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//

//**************************************************************************
 #if ENABLE_DP_ShortHPD_PM		
void _mdrv_DP_ToggleShortHPD(BYTE ucPortSelect)
{
    mhal_DP_HPDControl(ucPortSelect, FALSE);
    DelayUs(250);
    DelayUs(250);
    DelayUs(250);
    DelayUs(250);
    mhal_DP_HPDControl(ucPortSelect, TRUE);
}
 #endif
//**************************************************************************
//  [Function Name]:
//                  _mdrv_DP_EnableHDPs()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_DP_EnableHPD(BYTE ucPortSelect, BYTE ucDPEnableIndex)
{
    static BYTE ucHPDEnableIndex = 0;

    if(ucHPDEnableIndex != ucDPEnableIndex)
    {
        ucHPDEnableIndex = ucHPDEnableIndex |_BIT(ucPortSelect);

        mhal_DP_EnableHPD(ucPortSelect);
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_DP_CheckNormalTraining()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool _mdrv_DP_CheckNormalTraining(BYTE ucPortSelect)
{
    Bool bTrainingFlag = FALSE;

    if(gDPInfo[ucPortSelect].bDPTrainingP0T)
    {
        gDPInfo[ucPortSelect].bDPTrainingP0T = FALSE;
        bTrainingFlag = TRUE;
    }

    if(gDPInfo[ucPortSelect].bDPTrainingP1T)
    {
        gDPInfo[ucPortSelect].bDPTrainingP1T = FALSE;
        bTrainingFlag = TRUE;
    }

    return bTrainingFlag;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_DP_CheckFastTraining()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool _mdrv_DP_CheckFastTraining(BYTE ucPortSelect)
{
    Bool bSquelchStable = FALSE;

    if(mhal_DP_CheckSquelch(ucPortSelect))
    {
        if(gDPInfo[ucPortSelect].bSquelchWaitStable)
        {
            if(gDPInfo[ucPortSelect].uwDPRxStableTimeout == 0)
            {
                bSquelchStable = TRUE;

                gDPInfo[ucPortSelect].bSquelchWaitStable = FALSE;
            }
        }
        else
        {
            gDPInfo[ucPortSelect].uwDPRxStableTimeout = DP_SQUELCH_STABLE_TIME;
            gDPInfo[ucPortSelect].bSquelchWaitStable = TRUE;
        }
    }
    else
    {
        gDPInfo[ucPortSelect].bSquelchWaitStable = FALSE;
    }

    return bSquelchStable;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_DP_CheckFastTraining()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool _mdrv_DP_FastTrainingProc(BYTE ucPortSelect, BYTE ucHBR2Select)
{
    Bool bTrainFinish = FALSE;
    Bool bTrainSuccess = FALSE;
#if(DP_AUTO_EQ_ENABLE)
    Bool bFTAutoEQEnable = FALSE;
#endif
    BYTE uctemp = 0;
    BYTE ucLaneType = 0;
    BYTE ucRateType = 0;

    for(ucRateType = 0; (ucRateType < DP_FAST_TRAINING_RATE_MASK) && (!bTrainFinish); ucRateType++)
    {
    	 if(!gDPInfo[ucPortSelect].bDPTrainingFlag)
        mhal_DP_SetFTSyntheizer(ucPortSelect, ucHBR2Select, ucRateType);

        for(ucLaneType = 0; (ucLaneType < DP_FAST_TRAINING_LANE_MASK) && (!bTrainFinish); ucLaneType++)
        {
             if(!gDPInfo[ucPortSelect].bDPTrainingFlag)  	
            mhal_DP_SetFTLaneCount(ucPortSelect, ucHBR2Select, ucLaneType);

            if(!gDPInfo[ucPortSelect].bDPTrainingFlag)  	
            {
                mhal_DP_FTScrambleEnable(ucPortSelect, ucHBR2Select, FALSE);

//                mhal_DP_FastTrainingEnable(ucPortSelect, ucHBR2Select, TRUE);

#if(DP_AUTO_EQ_ENABLE)
                bFTAutoEQEnable = mhal_DP_FTAutoEQEnable(ucPortSelect, ucHBR2Select, TRUE);
#endif
		  if(!gDPInfo[ucPortSelect].bDPTrainingFlag)  
                mhal_DP_FastTrainingEnable(ucPortSelect, ucHBR2Select, TRUE);

                for(uctemp = 0; uctemp < DP_FAST_TRAINING_CHECK_TIME; uctemp++)
                {
                    Delay1ms(1);

                    if(_mdrv_DP_CheckNormalTraining(ucPortSelect))
                    {
                        bTrainFinish = TRUE;

                        break;
                    }
                }

#if(DP_AUTO_EQ_ENABLE)
                if(bFTAutoEQEnable)
                {
                    mhal_DP_FTAutoEQCheckDown(ucPortSelect, ucHBR2Select);
                }
#endif
                for(uctemp = 0; (uctemp < DP_FAST_TRAINING_LOCK_COUNT) && (!bTrainFinish); uctemp++)
                {
                    if(mhal_DP_CheckCDRLock(ucPortSelect, ucHBR2Select))
                    {
                        bTrainSuccess = TRUE;
                        bTrainFinish = TRUE;
                    }
                    else
                    {
                        bTrainSuccess = FALSE;

                        mhal_DP_FastTrainingEnable(ucPortSelect, ucHBR2Select, FALSE);

                        break;
                    }
                }
            }
        }
    }

    bTrainFinish = FALSE;

    if(bTrainSuccess)
    {
        for(uctemp = 0; uctemp < DP_FAST_TRAINING_CHECK_TIME; uctemp++)
        {
            Delay1ms(1);

            if(_mdrv_DP_CheckNormalTraining(ucPortSelect))
            {
                bTrainFinish = TRUE;
                bTrainSuccess = FALSE;

                break;
            }
        }

         if(bTrainSuccess && !gDPInfo[ucPortSelect].bDPTrainingFlag)
        {
            mhal_DP_FTCheckTimingPrepare(ucPortSelect, ucHBR2Select);

            if(!_mdrv_DP_CheckInputTiming(ucPortSelect, ucHBR2Select))
            {
                mhal_DP_FTScrambleEnable(ucPortSelect, ucHBR2Select, TRUE);

                if(!_mdrv_DP_CheckInputTiming(ucPortSelect, ucHBR2Select))
                {
                    mhal_DP_FTScrambleEnable(ucPortSelect, ucHBR2Select, FALSE);

                    if(!_mdrv_DP_CheckInputTiming(ucPortSelect, ucHBR2Select))
                    {
                        bTrainSuccess = FALSE;
                    }
                }
            }
        }

        mhal_DP_FastTrainingEnable(ucPortSelect, ucHBR2Select, FALSE);
    }

    return bTrainSuccess;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_DP_CheckVPLLBigChange()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool _mdrv_DP_CheckVPLLBigChange(BYTE ucPortSelect, BYTE ucHBR2Select)
{
    Bool bVPLLBigChange = FALSE;

    if(gDPInfo[ucPortSelect].bDPVPLLBIGChange)
    {
        gDPInfo[ucPortSelect].bDPVPLLBIGChange = FALSE;
        bVPLLBigChange = TRUE;

        mhal_DP_UpdateMVIDValue(ucPortSelect, ucHBR2Select);

        if(gDPInfo[ucPortSelect].bAudioEnable)
        {
            mhal_DP_AudioSSCInit();
        }
    }

    return bVPLLBigChange;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_DP_HPDPollingEvent()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_DP_HPDPollingEvent(BYTE ucPortSelect, BYTE ucHBR2Select, Bool bCDRLockFlag)
{
    Bool bTrainingFlag = mhal_DP_GetTrainingPatternFlag(ucPortSelect, ucHBR2Select);

    // HDCP reauthen
    if(bCDRLockFlag && (!bTrainingFlag) && mhal_DP_GetHDCPFailFlag(ucPortSelect, ucHBR2Select))
    {
        if(!gDPInfo[ucPortSelect].bDPHDCPIRQ)
        {
            gDPInfo[ucPortSelect].bDPHDCPIRQ = TRUE;
            gDPInfo[ucPortSelect].uwHDCPCnt = 1000;
        }
        else if(gDPInfo[ucPortSelect].uwHDCPCnt == 0)
        {
            gDPInfo[ucPortSelect].bDPHDCPIRQ = FALSE;

            _mdrv_DP_ToggleHPD(ucPortSelect);
        }
    }
    else if(gDPInfo[ucPortSelect].bDPHDCPIRQ)
    {
        gDPInfo[ucPortSelect].bDPHDCPIRQ = FALSE;
    }

    // CDR loss lock
    if(gDPInfo[ucPortSelect].bDPLoseCDRLockIRQ)
    {
        if(bCDRLockFlag)
        {
            gDPInfo[ucPortSelect].bDPLoseCDRLockIRQ = FALSE;
        }
        else if((gDPInfo[ucPortSelect].uwCDRHPDCnt == 0) && (!bTrainingFlag))
        {
            gDPInfo[ucPortSelect].bDPLoseCDRLockIRQ = FALSE;

            if(mhal_DP_CheckLossCDRStatus(ucPortSelect, ucHBR2Select))
            {               
                mdrv_DP_Set_HPD(ucPortSelect,FALSE);

		DelayUs(250);
		DelayUs(250);   
		DelayUs(250);   
		DelayUs(250);   
                
                mdrv_DP_Set_HPD(ucPortSelect,TRUE);
            }
        }
    }

    // Interlane skew
    if(gDPInfo[ucPortSelect].bDPInterlaneSkewIRQ)
    {
        if(!bCDRLockFlag)
        {
            gDPInfo[ucPortSelect].bDPInterlaneSkewIRQ = FALSE;
        }
        else if((gDPInfo[ucPortSelect].uwSkewHPDCnt == 0) && (!bTrainingFlag))
        {
            gDPInfo[ucPortSelect].bDPInterlaneSkewIRQ = FALSE;

            if(mhal_DP_CheckInterlaneSkewStatus(ucPortSelect, ucHBR2Select))
            {
                 mdrv_DP_Set_HPD(ucPortSelect,FALSE);
                
		DelayUs(250);
		DelayUs(250);   
		DelayUs(250);   
		DelayUs(250);   
                
                 mdrv_DP_Set_HPD(ucPortSelect,TRUE);
            }
        }
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_DP_OverWriteRegenTiming()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_DP_RegenTimingProtect(BYTE ucPortSelect, BYTE ucHBR2Select)
{
	mhal_DP_RegenTimingProtect(ucPortSelect, ucHBR2Select);
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_DP_RegenTimingProc()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_DP_RegenTimingProc(BYTE ucPortSelect, BYTE ucHBR2Select)
{
    WORD usBlank = 0;
    WORD usWidth = 0;
    WORD usFrontPorch = 0;
    WORD usBackPorch = 0;
    WORD usTimingInfo[DP_CHECK_TIMING_QUEUE_SIZE] = {0};  

    gDPInfo[ucPortSelect].bDPInterlace = mhal_DP_GetTimingInformation(ucPortSelect, ucHBR2Select, usTimingInfo);
    gDPInfo[ucPortSelect].uwDPHtotal = usTimingInfo[0];
    gDPInfo[ucPortSelect].uwDPHWidth = usTimingInfo[2];
    gDPInfo[ucPortSelect].uwDPVtotal = usTimingInfo[1];
    gDPInfo[ucPortSelect].uwDPVWidth = usTimingInfo[3];

    // V parameter
    usBlank = gDPInfo[ucPortSelect].uwDPVtotal - gDPInfo[ucPortSelect].uwDPVWidth;
    usWidth = usBlank /2;
    usFrontPorch = (usBlank - usWidth) /2;
    usBackPorch = usBlank - usWidth - usFrontPorch;

    usTimingInfo[0] = usWidth;
    usTimingInfo[1] = usFrontPorch;
    usTimingInfo[2] = usBackPorch;

	if(usBlank < 0x400) 
	{
	    // H parameter
	    //usBlank = gDPInfo[ucPortSelect].uwDPHtotal - gDPInfo[ucPortSelect].uwDPHWidth;
	    //usWidth = usBlank /3;

	    if((gDPInfo[ucPortSelect].uwDPHWidth > 3000)||(gDPInfo[ucPortSelect].uwDPVWidth > 3000)||(mhal_DP_GetTimingPixelClock(ucPortSelect, ucHBR2Select) > 300))
	    {
	       // usWidth = (gDPInfo[ucPortSelect].uwDPPixel /10) + 1;

	       //  if(usWidth < 0x34)
	       // {
	            usWidth = 0x34;
	      //  }
	    }
	    else
	    {
	            usWidth = 0x2C;
	    }

	    usTimingInfo[3] = usWidth;

	    mhal_DP_RegenTimingInformation(ucPortSelect, ucHBR2Select, usTimingInfo);
	}
}


//**************************************************************************
//  [Function Name]:
//                  _mdrv_DP_RegenTimingProc()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
 void _mdrv_DP_TimingChgProc(BYTE ucPortSelect, BYTE ucHBR2Select)    //20150722
{
	BYTE Cnt=5;
	BYTE CheckCnt=0;
	WORD usTimingInfo[DP_CHECK_TIMING_QUEUE_SIZE] = {0};

	gDPInfo[ucPortSelect].bDPInterlace = mhal_DP_GetTimingInformation(ucPortSelect, ucHBR2Select, usTimingInfo);
	gDPInfo[ucPortSelect].uwDPHtotal = usTimingInfo[0];
	gDPInfo[ucPortSelect].uwDPVtotal = usTimingInfo[1];
	
	if((gDPInfo[ucPortSelect].uwDPVtotal_Temp !=0)&&(gDPInfo[ucPortSelect].uwDPHtotal_Temp !=0))
	{
		while(Cnt--)
		{
			if((abs(gDPInfo[ucPortSelect].uwDPHtotal - gDPInfo[ucPortSelect].uwDPHtotal_Temp) >0x100)
			    ||((abs(gDPInfo[ucPortSelect].uwDPVtotal - gDPInfo[ucPortSelect].uwDPVtotal_Temp) >0x100)))	
			{
				CheckCnt++;				
			}			
		}
	}
	gDPInfo[ucPortSelect].uwDPVtotal_Temp =gDPInfo[ucPortSelect].uwDPVtotal;
	gDPInfo[ucPortSelect].uwDPHtotal_Temp =gDPInfo[ucPortSelect].uwDPHtotal;

	if(CheckCnt > 2)
	{	
		//printMsg("--------HTOTAL AND VTT Chang     ");
		IsrModeChangeCombo((ucPortSelect+1));    ////20150721
	}
}
//**************************************************************************
//  [Function Name]:
//                  _mdrv_DP_GUIDPollingProc()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_DP_GUIDPollingProc(BYTE ucPortSelect)
{
    if(gDPInfo[ucPortSelect].bDPGUID)
    {
        gDPInfo[ucPortSelect].bDPGUID = FALSE;

        mhal_DP_InsertGUIDValue(ucPortSelect, ucGUIDData);
    }
}

 #if  DP_NoInput_Audio
//**************************************************************************
//  [Function Name]:
//                  _mdrv_DP_NoAudioOutputProc()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_DP_NoAudioOutputProc(BYTE ucPortSelect, BYTE ucHBR2Select)
{
	 Bool bCDRLockFlag = mhal_DP_CheckCDRLock(ucPortSelect, ucHBR2Select);
	 
	 if(mhal_DP_GetAudioSrc(ucPortSelect, ucHBR2Select) != 12)
	 {
		return;		
	 }		
 	 if(!bCDRLockFlag)
 	 {
	 	 mhal_DP_FreeRunSetting(ucPortSelect, ucHBR2Select);		      
	 }
 
	
}
 #endif
//**************************************************************************
//  [Function Name]:
//                  _mdrv_DP_StateHandler()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool _mdrv_DP_StateHandler(BYTE ucPortSelect, BYTE ucHBR2Select)
{
    Bool bSignalStable = FALSE;
    Bool bCDRLockFlag = mhal_DP_CheckCDRLock(ucPortSelect, ucHBR2Select);
    Bool bTrainingFlag = _mdrv_DP_CheckNormalTraining(ucPortSelect);

    _mdrv_DP_HPDPollingEvent(ucPortSelect, ucHBR2Select, bCDRLockFlag);

	_mdrv_DP_RegenTimingProtect(ucPortSelect, ucHBR2Select);

    switch(gDPInfo[ucPortSelect].ucState)
    {
        case DP_STATE_STARTUP:
            gDPInfo[ucPortSelect].ucState = DP_STATE_CKECKLOCK;
            gDPInfo[ucPortSelect].bSquelchWaitStable = FALSE;
            break;

        case DP_STATE_CKECKLOCK:
            if(bCDRLockFlag)
            {
                gDPInfo[ucPortSelect].DPCheckModeCnt = 0;
                gDPInfo[ucPortSelect].ucState = DP_STATE_MEASURE;

                DP_DRV_DPRINTF("** DP CDR lock check -> measure! port %d", ucPortSelect);
            }
            else if((!bTrainingFlag) && _mdrv_DP_CheckFastTraining(ucPortSelect))
            {
                //mhal_DP_AutoEQEnable(ucPortSelect, ucHBR2Select, FALSE);
		  #if ENABLE_DP_ShortHPD_PM		
		   if(bHPDShortPulse[ucPortSelect])
                {				
                		if( mhal_DPRXCheckAUXValid(ucPortSelect, ucHBR2Select))
                		{
					_mdrv_DP_ToggleShortHPD(ucPortSelect);
					  mdrv_DP_Set_ShortHPDFlag(ucPortSelect, FALSE);
                		} 
                }else
                #endif	
                if(_mdrv_DP_FastTrainingProc(ucPortSelect, ucHBR2Select))
                {
                    gDPInfo[ucPortSelect].DPCheckModeCnt = 0;
                    gDPInfo[ucPortSelect].ucState = DP_STATE_MEASURE;

                    DP_DRV_DPRINTF("** DP fast training check -> measure! port %d", ucPortSelect);
                }
            }

            break;

        case DP_STATE_MEASURE:
        	gDPInfo[ucPortSelect].bDPTrainingFlag = FALSE;
            if(_mdrv_DP_CheckInputTiming(ucPortSelect, ucHBR2Select))
            {		
           	    mhal_DP_SetLearnMode(ucPortSelect, ucHBR2Select);  	

				mhal_DP_HVFSMCheck(ucPortSelect, ucHBR2Select); 

                gDPInfo[ucPortSelect].ucState = DP_STATE_NORMAL;
                gDPInfo[ucPortSelect].NoVscPktCnt = 0;
                gDPInfo[ucPortSelect].bDPCheckFlag = FALSE;
                gDPInfo[ucPortSelect].uwDPStableCount = 0;
                gDPInfo[ucPortSelect].bCheckInterlaceToggle = 0;
                gDPInfo[ucPortSelect].bFieldToggle = 0;
				gDPInfo[ucPortSelect].bInterlaceCheck = 0;
                gDPInfo[ucPortSelect].ucInterlaceCount = 0;

                DP_DRV_DPRINTF("** DP mode pass measure -> normal! port %d", ucPortSelect);
            }
            else if(gDPInfo[ucPortSelect].DPCheckModeCnt < DP_MEASURE_TIMING_COUNTER)
            {
                gDPInfo[ucPortSelect].ucState = DP_STATE_CKECKLOCK;
                gDPInfo[ucPortSelect].bSquelchWaitStable = FALSE;

                DP_DRV_DPRINTF("** DP mode fail measure -> check! port %d", ucPortSelect);
            }
            else
            {
                gDPInfo[ucPortSelect].DPCheckModeCnt++;
            }
            #if ENABLE_DP_ShortHPD_PM
            mdrv_DP_Set_ShortHPDFlag(ucPortSelect, FALSE);
	     #endif	
            break;

        case DP_STATE_NORMAL:
            if(!bCDRLockFlag)
            {
                gDPInfo[ucPortSelect].ucState = DP_STATE_CKECKLOCK;
                gDPInfo[ucPortSelect].bSquelchWaitStable = FALSE;

                if(gDPInfo[ucPortSelect].bAudioEnable)
                {
                    mhal_DP_AudioSSCInit();
                }

                DP_DRV_DPRINTF("** DP loss lock normal -> check! port %d", ucPortSelect);
            }
            else if(_mdrv_DP_CheckVPLLBigChange(ucPortSelect, ucHBR2Select))
            {
                gDPInfo[ucPortSelect].ucState = DP_STATE_MEASURE;
                gDPInfo[ucPortSelect].DPCheckModeCnt = 0;

                DP_DRV_DPRINTF("** DP VPLL change normal -> measure! port %d", ucPortSelect);
            }
            else
            {
                gDPInfo[ucPortSelect].bAutoEQReset = 1;

                bSignalStable = TRUE;
            }

            break;

        default:

            break;
    }

    if(gDPInfo[ucPortSelect].ucPreState != gDPInfo[ucPortSelect].ucState)
    {
        gDPInfo[ucPortSelect].ucPreState = gDPInfo[ucPortSelect].ucState;

        if(gDPInfo[ucPortSelect].ucState == DP_STATE_CKECKLOCK)
        {
            mhal_DP_UpdateAudioMNCode(ucPortSelect, ucHBR2Select, 0, 0);
        }
    }

    return bSignalStable;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_DP_StablePolling()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_DP_StablePolling(BYTE ucPortSelect, BYTE ucHBR2Select)
{
    WORD usPollingInfo[DP_STABLE_POLLING_QUEUE_SIZE] = {0};

    // Check packet status
    if(gDPInfo[ucPortSelect].bPacketTimer == 0)
	{
	    gDPInfo[ucPortSelect].bPacketTimer = 100;
		gDPInfo[ucPortSelect].InputPacketStatus = mhal_DP_GetPacketStatus(ucPortSelect, ucHBR2Select);
	}

    // Get color format
    switch(mhal_DP_GetColorSpace(ucPortSelect, ucHBR2Select, usPollingInfo))
    {
        case 0:
            gDPInfo[ucPortSelect].ucDPColorFormat = DP_COLOR_FORMAT_RGB;
            break;

        case 1:
            gDPInfo[ucPortSelect].ucDPColorFormat = DP_COLOR_FORMAT_YUV_422;
            break;

        case 2:
            gDPInfo[ucPortSelect].ucDPColorFormat = DP_COLOR_FORMAT_YUV_444;
            break;

        default:

            break;
    };

    gDPInfo[ucPortSelect].DPMISC0 = usPollingInfo[0];
    gDPInfo[ucPortSelect].DPMISC1 = usPollingInfo[1];

	if(((usPollingInfo[0]&(BIT4|BIT3|BIT2|BIT1)) == (BIT4|BIT3))||((usPollingInfo[0]&(BIT4|BIT3|BIT2|BIT1)) == (BIT3)))
	{	
		gDPInfo[ucPortSelect].ucDPRangeFormat = DP_COLOR_RANGE_FULL;
	}
	else  if((usPollingInfo[0] &BIT3) == BIT3)
	{	
	  	 gDPInfo[ucPortSelect].ucDPRangeFormat = DP_COLOR_RANGE_LIMIT;	  	 
	}
	else
	{
	    	gDPInfo[ucPortSelect].ucDPRangeFormat = DP_COLOR_RANGE_FULL;
	}
	

	if(((usPollingInfo[0] )&(BIT4|BIT3|BIT2|BIT1 )) == (BIT4|BIT3))
	{
	       gDPInfo[ucPortSelect].ucColorType = DP_COLORIMETRY_ADOBERBG;
	}
	else if((usPollingInfo[0]&BIT4) == BIT4)
	{
		gDPInfo[ucPortSelect].ucColorType = DP_COLORIMETRY_xvYCC709;
	}
	else
	{
		gDPInfo[ucPortSelect].ucColorType = DP_COLORIMETRY_xvYCC601;
	}

    // Select 2P or 1P
    if(ucHBR2Select != DP_HBR2_ENGINE_NONE)
    {
        gDPInfo[ucPortSelect].uwDPPixel = mhal_DP_GetTimingPixelClock(ucPortSelect, ucHBR2Select);

        if((gDPInfo[ucPortSelect].uwDPPixel > 330) && (!gDPInfo[ucPortSelect].bDPSplitOnOff))
        {
            if(gDPInfo[ucPortSelect].bDPSplitEn_Cnt_Up >= 200)
            {
                gDPInfo[ucPortSelect].bDPSplitOnOff = TRUE;
                gDPInfo[ucPortSelect].bDPSplitEn_Cnt_Down = 0;
            }
            else
            {
                gDPInfo[ucPortSelect].bDPSplitEn_Cnt_Up++;
            }
        }
        else if((gDPInfo[ucPortSelect].uwDPPixel <= 330) && (gDPInfo[ucPortSelect].bDPSplitOnOff))
        {
            if(gDPInfo[ucPortSelect].bDPSplitEn_Cnt_Down >= 200)
            {
                gDPInfo[ucPortSelect].bDPSplitOnOff = FALSE;
                gDPInfo[ucPortSelect].bDPSplitEn_Cnt_Up = 0;
            }
            else
            {
                gDPInfo[ucPortSelect].bDPSplitEn_Cnt_Down++;
            }
        }
       
        gDPInfo[ucPortSelect].bDPSplitEn = mhal_DP_SplitSwitch(ucHBR2Select, gDPInfo[ucPortSelect].bDPSplitOnOff);
    }

    // Regen timing
    _mdrv_DP_RegenTimingProc(ucPortSelect, ucHBR2Select);
    _mdrv_DP_TimingChgProc(ucPortSelect, ucHBR2Select);   
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_DP_AudioHandler()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_DP_AudioHandler(BYTE ucPortSelect, BYTE ucHBR2Select)
{
    if(gDPInfo[ucPortSelect].bAudioEnable)
    {
        if(mhal_DP_CheckAudioAbsent(ucPortSelect, ucHBR2Select, gDPInfo[ucPortSelect].InputPacketStatus)) // No audio
        {
        	#if  DP_NoInput_Audio	
        	 if(mhal_DP_GetAudioSrc(ucPortSelect, ucHBR2Select) == 12)
		     mhal_DP_FreeRunSetting(ucPortSelect, ucHBR2Select);
		#endif 	
        	
                gDPInfo[ucPortSelect].bDPNoAudioStrm = TRUE;
 #if !DP_SSC_ENABLE
                mhal_DP_AudioMute(FALSE);
 #endif               
        }
        else
        {
        	 gDPInfo[ucPortSelect].bDPNoAudioStrm = FALSE;
        	 
#if DP_SSC_ENABLE
            if(gDPInfo[ucPortSelect].DPAudioAdjustCnt ==0)
            {
                mhal_DP_AudioSSCAdjust(ucPortSelect, ucHBR2Select);

                gDPInfo[ucPortSelect].DPAudioAdjustCnt = 5;
            }
#endif

            mhal_DP_CheckAudioMuteEvent(ucPortSelect, ucHBR2Select);

            if(mhal_DP_CheckLossAlignment(ucPortSelect, ucHBR2Select)) // Check alignment
            {
                if(mhal_DP_CheckAUPLLBigChange(ucPortSelect, ucHBR2Select))
		 {                                  
		   	gDPInfo[ucPortSelect].DPAudioFreq= mhal_DP_CalAudioFreq(ucPortSelect, ucHBR2Select);

			if(gDPInfo[ucPortSelect].DPAudioFreq ==0)
				
                      gDPInfo[ucPortSelect].bDPAUPLLBIGChange = FALSE;
			
			 else
			 	
                    gDPInfo[ucPortSelect].bDPAUPLLBIGChange = TRUE;
			 
                }
                if(gDPInfo[ucPortSelect].bDPAUPLLBIGChange)
                {
                    gDPInfo[ucPortSelect].bDPAUPLLBIGChange = FALSE;

                    mhal_DP_AUPLLBigChangeProc(ucPortSelect, ucHBR2Select);

#if DP_SSC_ENABLE
                    if(gDPInfo[ucPortSelect].bAudioEnable)
                    {
                        mhal_DP_UpdateAudioSSC();
                    }

#endif
                }
            }
        }
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_DP_CheckErrorCount()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_DP_CheckErrorCount(BYTE ucPortSelect, BYTE ucHBR2Select)
{
    if(gDPInfo[ucPortSelect].uwDPStableCount == 0)
    {
        if(gDPInfo[ucPortSelect].bDPCheckFlag)
        {
            gDPInfo[ucPortSelect].bDPCheckFlag = FALSE;

            if(mhal_DP_GetErrorCountFlag(ucPortSelect, ucHBR2Select))
            {
                _mdrv_DP_ToggleHPD(ucPortSelect);
            }

            mhal_DP_ErrorCountEnable(ucPortSelect, ucHBR2Select, FALSE);

            gDPInfo[ucPortSelect].uwDPStableCount = 20; // 20ms
        }
        else
        {
            gDPInfo[ucPortSelect].bDPCheckFlag = TRUE;

            mhal_DP_ErrorCountEnable(ucPortSelect, ucHBR2Select, FALSE);
            mhal_DP_ErrorCountEnable(ucPortSelect, ucHBR2Select, TRUE);

            gDPInfo[ucPortSelect].uwDPStableCount = 500; // 500ms
        }
    }
}


//**************************************************************************
//  [Function Name]:
//                  _mdrv_DP_CheckInterlaceInverse()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_DP_CheckInterlaceInverse(BYTE ucPortSelect, BYTE ucHBR2Select)
{
	DWORD ulField0 = 0x00, ulField1 = 0x00;
	Bool bFieldFlag = 0, bInterlaceFlag = 0;
	BYTE ucFlagCount = 0;

	bInterlaceFlag = mhal_DP_GetInterlaceFlag(ucPortSelect, ucHBR2Select);

    if(bInterlaceFlag && (gDPInfo[ucPortSelect].ucInterlaceCount == 0) && (gDPInfo[ucPortSelect].bInterlaceCheck == 0))
    {    
        if(gDPInfo[ucPortSelect].bCheckInterlaceToggle == 0)
        {
            gDPInfo[ucPortSelect].bCheckInterlaceToggle = 1;
			ucFlagCount = 30;
            bFieldFlag = mhal_DP_GetFieldFlag(ucPortSelect, ucHBR2Select);
	        mhal_DP_SetInterlaceInverse(ucPortSelect, ucHBR2Select, TRUE);			

            while(ucFlagCount > 0)
            {
			    Delay1ms(1);

				if(bFieldFlag != mhal_DP_GetFieldFlag(ucPortSelect, ucHBR2Select))
				{
					gDPInfo[ucPortSelect].bFieldToggle = 1;
                    break;				
				}
				
                ucFlagCount--;
            }

		    gDPInfo[ucPortSelect].ucInterlaceCount = 200; // 200ms

        }  
		else
		{
			gDPInfo[ucPortSelect].bInterlaceCheck = 1;

			if(gDPInfo[ucPortSelect].bFieldToggle == 1)
			{
				ulField0 = mhal_DP_GetFieldCount(ucPortSelect, ucHBR2Select, 0);
				ulField1 = mhal_DP_GetFieldCount(ucPortSelect, ucHBR2Select, 1);
			
				if(ulField0 <= ulField1) 
				{
					//For Chroma Issue, field1 must larger than field0 over than 20, then we turn-off inverse
					if((ulField0 + 20) <= ulField1) 
					{					
						mhal_DP_SetInterlaceInverse(ucPortSelect, ucHBR2Select, FALSE); //Rx inverse Enable  //AMD
					}
					else 
					{										
						mhal_DP_SetInterlaceInverse(ucPortSelect, ucHBR2Select, TRUE); //Rx inverse Disable  //Chroma
					}
				}
				else 
				{
					mhal_DP_SetInterlaceInverse(ucPortSelect, ucHBR2Select, TRUE); //Rx inverse Disable
				}
			}
			else
			{
				mhal_DP_SetInterlaceInverse(ucPortSelect, ucHBR2Select, TRUE); //Rx inverse Disable //ASTRO
			}			
		}
    }       
}

#if(DP_AUX_DEBUG_ENABLE)
//**************************************************************************
//  [Function Name]:
//                  _mdrv_DP_AuxDebugProc()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_DP_AuxDebugProc(BYTE ucPortSelect)
{
    BYTE uctemp = 0;
    BYTE ucRegisterValue = mhal_DP_GetDebugRegisterValue(ucPortSelect, 1);

    if((ucRegisterValue &BIT0) == BIT0)
    {
        for(uctemp = 0; uctemp < DP_AUX_COMMAND_MAX_LENGTH; uctemp++)
        {
            DP_DRV_DPRINTF("** DP GUID data !!~~ %d", ucGUIDData[uctemp]);
        }
    }
}

#endif

#if(DP_HDCP2_FUNCTION_SUPPORT)
//**************************************************************************
//  [Function Name]:
//                  _mdrv_DP_HDCP2GetRxData()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool _mdrv_DP_HDCP2GetRxData(BYTE ucPortSelect, BYTE ucMessageID, BYTE *pHDCPData)
{
    Bool bGetData = FALSE;

    switch(ucMessageID)
    {
        case DP_HDCP2_MSG_ID_AKE_INIT:
            bGetData = mhal_DP_HDCP2GetRxData(ucPortSelect, DP_HDCP2_INFO_AKE_RTX, pHDCPData);
            bGetData = mhal_DP_HDCP2GetRxData(ucPortSelect, DP_HDCP2_INFO_AKE_TXCAPS, &pHDCPData[DP_HDCP2_AKE_RTX_SIZE]);
            break;

        case DP_HDCP2_MSG_ID_AKE_NO_STORED_KM:
            bGetData = mhal_DP_HDCP2GetRxData(ucPortSelect, DP_HDCP2_INFO_AKE_EKPUB_KM, pHDCPData);
            break;

        case DP_HDCP2_MSG_ID_AKE_STORED_KM:
            bGetData = mhal_DP_HDCP2GetRxData(ucPortSelect, DP_HDCP2_INFO_AKE_EKH_KM_WR, pHDCPData);
            bGetData = mhal_DP_HDCP2GetRxData(ucPortSelect, DP_HDCP2_INFO_AKE_M, &pHDCPData[DP_HDCP2_AKE_EKH_KM_WR_SIZE]);
            break;

        case DP_HDCP2_MSG_ID_LC_INIT:
            bGetData = mhal_DP_HDCP2GetRxData(ucPortSelect, DP_HDCP2_INFO_LC_RN, pHDCPData);
            break;

        case DP_HDCP2_MSG_ID_SKE_SEND_EKS:
            bGetData = mhal_DP_HDCP2GetRxData(ucPortSelect, DP_HDCP2_INFO_SKE_EDKEY_KS, pHDCPData);
            bGetData = mhal_DP_HDCP2GetRxData(ucPortSelect, DP_HDCP2_INFO_SKE_RIV, &pHDCPData[DP_HDCP2_SKE_EDKEY_KS_SIZE]);
            break;

        default:

            break;
    };

    return bGetData;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_DP_HDCP2SetTxData()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_DP_HDCP2SetTxData(BYTE ucPortSelect, BYTE ucMessageID, BYTE *pHDCPData)
{
    switch(ucMessageID)
    {
        case DP_HDCP2_MSG_ID_AKE_SEND_CERT:
            mhal_DP_HDCP2SetTxData(ucPortSelect, DP_HDCP2_INFO_AKE_RRX, pHDCPData);
            mhal_DP_HDCP2SetTxData(ucPortSelect, DP_HDCP2_INFO_AKE_RXCAPS, &pHDCPData[DP_HDCP2_AKE_RRX_SIZE]);
            break;

        case DP_HDCP2_MSG_ID_AKE_SEND_H_PRIME:
            mhal_DP_HDCP2SetTxData(ucPortSelect, DP_HDCP2_INFO_AKE_H_PRIME, pHDCPData);
            mhal_DP_HDCP2SetRxStatus(ucPortSelect, DP_HDCP2_RXSTATUS_H_PRIME_AVAILABLE, TRUE);
			mhal_DP_HDCP2SetRxStatus(ucPortSelect, DP_HDCP2_RXSTATUS_H_PRIME_AVAILABLE, FALSE);
			mhal_DP_HDCP2SetCpIrq(ucPortSelect);
			mhal_DP_HPDControl(ucPortSelect, FALSE);
			DelayUs(250);
			DelayUs(250);
			DelayUs(250);
			mhal_DP_HPDControl(ucPortSelect, TRUE);
            break;

        case DP_HDCP2_MSG_ID_AKE_SEND_PAIRING_INFO:
            mhal_DP_HDCP2SetTxData(ucPortSelect, DP_HDCP2_INFO_AKE_EKH_KM_RD, pHDCPData);
            mhal_DP_HDCP2SetRxStatus(ucPortSelect, DP_HDCP2_RXSTATUS_PAIRING_AVAILABLE, TRUE);
            mhal_DP_HDCP2SetRxStatus(ucPortSelect, DP_HDCP2_RXSTATUS_PAIRING_AVAILABLE, FALSE);
			mhal_DP_HDCP2SetCpIrq(ucPortSelect);
			mhal_DP_HPDControl(ucPortSelect, FALSE);
			DelayUs(250);
			DelayUs(250);
			DelayUs(250);
			mhal_DP_HPDControl(ucPortSelect, TRUE);
            break;

        case DP_HDCP2_MSG_ID_LC_SEND_L_PRIME:
            mhal_DP_HDCP2SetTxData(ucPortSelect, DP_HDCP2_INFO_LC_L_PRIME, pHDCPData);
            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_DP_HDCP2Handler()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_DP_HDCP2Handler(BYTE ucPortSelect)
{
    BYTE ucHDCPData[DP_HDCP2_RX_QUEUE_SIZE] = {0};

    // HDCP 2.2 Rx
    if(GET_DP_HDCP2_RX_MSG_ID_PORT(ucPortSelect) != DP_HDCP2_MSG_ID_NONE)
    {
        ucHDCPData[0] = GET_DP_HDCP2_RX_MSG_ID_PORT(ucPortSelect);

        if(_mdrv_DP_HDCP2GetRxData(ucPortSelect, GET_DP_HDCP2_RX_MSG_ID_PORT(ucPortSelect), &ucHDCPData[1]))
        {
            if(pmdrv_DP_HDCP2RxEventProc != NULL) // Have call back function
            {
                pmdrv_DP_HDCP2RxEventProc(ucPortSelect, ucHDCPData);
            }

            SET_DP_HDCP2_RX_MSG_ID_PORT(ucPortSelect, DP_HDCP2_MSG_ID_NONE);

            DP_DRV_DPRINTF("** DP receive HDCP22 message ID = %d", GET_DP_HDCP2_RX_MSG_ID_PORT(ucPortSelect));
        }
    }

    // HDCP 2.2 Tx
    if(GET_DP_HDCP2_TX_MSG_ID_PORT(ucPortSelect) != DP_HDCP2_MSG_ID_NONE)
    {
        _mdrv_DP_HDCP2SetTxData(ucPortSelect, GET_DP_HDCP2_TX_MSG_ID_PORT(ucPortSelect), GET_DP_HDCP2_TX_QUEUE_ADDRESS());

        SET_DP_HDCP2_TX_MSG_ID_PORT(ucPortSelect, DP_HDCP2_MSG_ID_NONE);

        DP_DRV_DPRINTF("** DP send HDCP22 message ID = %d", GET_DP_HDCP2_TX_MSG_ID_PORT(ucPortSelect));
    }
}

#endif

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

//**************************************************************************
//  [Function Name]:
//                  mdrv_DP_SetConfiguration()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_DP_SetConfiguration(BYTE ucHBR20EnableIndex, BYTE ucHBR21EnableIndex, BYTE ucDPEnableIndex, BYTE ucDisplayEnableIndex)
{
    if(!GET_DP_SYSTEM_CONFIG_FINISH())
    {
        ucDPHBR20PortSelect = ucHBR20EnableIndex;
        ucDPHBR21PortSelect = ucHBR21EnableIndex;
    }

    ucDPFunctionEnableIndex = ucDPEnableIndex;
    ucDPDisplayEnableIndex = ucDisplayEnableIndex;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_DP_GetHBR2PortSelect()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE mdrv_DP_GetHBR2PortSelect(BYTE ucPortSelect)
{
    BYTE ucInputSelect = 0;
    BYTE ucHBR2Select = DP_HBR2_ENGINE_NONE;
    BYTE ucHBR20PortSelect = ucDPHBR20PortSelect;
    BYTE ucHBR21PortSelect = ucDPHBR21PortSelect;

    if(!GET_DP_SYSTEM_CONFIG_FINISH())
    {
        ucDPHBR20PortSelect = DP_INPUT_SELECT_MASK;
        ucDPHBR21PortSelect = DP_INPUT_SELECT_MASK;

        for(ucInputSelect = DP_INPUT_SELECT_PORT0; ucInputSelect < DP_INPUT_SELECT_MASK; ucInputSelect++)
        {
            if(ucHBR20PortSelect %2)
            {
                if(ucHBR20PortSelect > 1) // More than one port HBR2_0 config
                {
                    DP_DRV_DPRINTF("** DP more than one HBR2_0 config %d", ucDPHBR20PortSelect);
                }

                ucHBR20PortSelect = 0;
                ucDPHBR20PortSelect = ucInputSelect;
            }
            else
            {
                ucHBR20PortSelect = ucHBR20PortSelect >> 1;
            }

            if(ucHBR21PortSelect %2)
            {
                if(ucHBR21PortSelect > 1) // More than one port HBR2_1 config
                {
                    DP_DRV_DPRINTF("** DP more than one HBR2_1 config %d", ucDPHBR21PortSelect);
                }

                ucHBR21PortSelect = 0;
                ucDPHBR21PortSelect = ucInputSelect;
            }
            else
            {
                ucHBR21PortSelect = ucHBR21PortSelect >> 1;
            }
        }

        SET_DP_SYSTEM_CONFIG_FINISH();
    }

    if(ucPortSelect == ucDPHBR20PortSelect)
    {
        ucHBR2Select = DP_HBR2_ENGINE_0;
    }
    else if(ucPortSelect == ucDPHBR21PortSelect)
    {
        ucHBR2Select = DP_HBR2_ENGINE_1;
    }

    return ucHBR2Select;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_DP_Initial()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_DP_Initial(void)
{
    Bool bIsrEnable = FALSE;
    BYTE ucPortSelect = 0;
    BYTE ucHBR2Select = 0;

    memset(&gDPInfo, 0, sizeof(gDPInfo));

    mhal_DP_AuxCommonSetting(GET_DP_FUNCTION_ENABLE_INDEX());

    for(ucPortSelect = DP_INPUT_SELECT_PORT0; ucPortSelect < DP_INPUT_SELECT_MASK; ucPortSelect++)
    {
        mhal_DP_SetInterruptMask(ucPortSelect);

        if(GET_DP_FUNCTION_ENABLE_PORT(ucPortSelect))
        {
            ucHBR2Select = mdrv_DP_GetHBR2PortSelect(ucPortSelect);

            mhal_DP_Initial(ucPortSelect, ucHBR2Select);
            // Fast training initial
            mhal_DP_SetFTSyntheizer(ucPortSelect, ucHBR2Select, DP_FAST_TRAINING_RATE_HBR);
            // DP version initial
            mhal_DP_VersionSetting(ucPortSelect, ucHBR2Select, USER_PREF_DPConfigType);
            // GUID initial
            mhal_DP_InsertGUIDValue(ucPortSelect, ucGUIDData);

            _mdrv_DP_ClearEventFlag(ucPortSelect, DP_CLEAR_INITIAL_FLAG);

            DP_DRV_DPRINTF("** DP initial setup !! port %d", ucPortSelect);

            bIsrEnable = TRUE;
        }
    }

     mhal_DP_ClockEnable();
    if(bIsrEnable)
    {
        mhal_DP_EnableAuxInterrupt(TRUE);
        mhal_DP_AudioInitial();
    }

}

//**************************************************************************
//  [Function Name]:
//                  mdrv_DP_GetAuxInterruptFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mdrv_DP_GetAuxInterruptFlag(void)
{
    return mhal_DP_GetAuxInterruptFlag();
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_DP_IsrAuxHandler()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_DP_IsrAuxHandler(void)
{
    BYTE ucPortSelect = 0;

    for(ucPortSelect = DP_INPUT_SELECT_PORT0; ucPortSelect < DP_INPUT_SELECT_MASK; ucPortSelect++)
    {
        if(GET_DP_FUNCTION_ENABLE_PORT(ucPortSelect))
        {
            if(mhal_DP_GetMCCSReceiveFlag(ucPortSelect))
            {

            }

#if(DP_PROGRAM_DPCD0_ENABLE)
            if(mhal_DP_GetProgramDPCD0Flag(ucPortSelect))
            {
                gDPInfo[ucPortSelect].bDPGUID = mhal_DP_ProgramDPCD0Proc(ucPortSelect, ucGUIDData);
            }
#endif

#if(DP_PROGRAM_DPCD1_ENABLE)
            if(mhal_DP_GetProgramDPCD1Flag(ucPortSelect))
            {
                mhal_DP_ProgramDPCD1Proc(ucPortSelect, ucDataQueue);
            }
#endif

#if(DP_PROGRAM_DPCD2_ENABLE)
            if(mhal_DP_GetProgramDPCD2Flag(ucPortSelect))
            {

#if(DP_HDCP2_FUNCTION_SUPPORT)
                mhal_DP_HDCP2GetHPulum(ucPortSelect, ucHPulumQueue);
                mhal_DP_ProgramDPCD2Proc(ucPortSelect, ucHPulumQueue);
#endif

            }
#endif

#if(DP_PROGRAM_DPCD3_ENABLE)
            if(mhal_DP_GetProgramDPCD3Flag(ucPortSelect))
            {
#if(DP_HDCP2_FUNCTION_SUPPORT)
				mhal_DP_ProgramDPCD3Proc(ucPortSelect, &ucDPCD_0x69494);
#endif
            }
#endif

#if(0)
            if(mhal_DP_GetAuxReceiveFlag(ucPortSelect))
            {

            }
#endif			
        }
    }
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_DP_GetCableDetectPort()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_DP_IsrHandler(void)
{
    BYTE ucPortSelect = 0;
    BYTE ucHBR2Select = 0;

    for(ucPortSelect = DP_INPUT_SELECT_PORT0; ucPortSelect < DP_INPUT_SELECT_MASK; ucPortSelect++)
    {
        if(GET_DP_FUNCTION_ENABLE_PORT(ucPortSelect))
        {
            ucHBR2Select = mdrv_DP_GetHBR2PortSelect(ucPortSelect);

            if(mhal_DP_GetAUPLLBigChangeFlag(ucPortSelect, ucHBR2Select))
            {
                gDPInfo[ucPortSelect].bDPAUPLLBIGChange = TRUE;
            }

            if(mhal_DP_GetInterlaneSkewFlag(ucPortSelect, ucHBR2Select))
            {
                gDPInfo[ucPortSelect].bDPInterlaneSkewIRQ = TRUE;
                gDPInfo[ucPortSelect].uwSkewHPDCnt = 1000;
            }

            if(mhal_DP_GetCDRLossLockFlag(ucPortSelect, ucHBR2Select))
            {
            	  if( gDPInfo[ucPortSelect].bAutoEQReset)
            	  {
            	  	msWriteByteMask(REG_113000 , BIT1, BIT1);    //0x66 issue.
                	msWriteByteMask(REG_113000 , 0, BIT1);
			 gDPInfo[ucPortSelect].bAutoEQReset =0;
            	  }
                gDPInfo[ucPortSelect].bDPLoseCDRLockIRQ = TRUE;
                gDPInfo[ucPortSelect].uwCDRHPDCnt = 1000;
            }

            if(mhal_DP_GetVPLLBigChangeFlag(ucPortSelect, ucHBR2Select))
            {
                gDPInfo[ucPortSelect].bDPVPLLBIGChange = TRUE;
            }

            if(mhal_DP_GetTrainingPattern1Flag(ucPortSelect, ucHBR2Select))
            {

#if(DP_ENABLE_STABLE_COUNT)
	            gDPInfo[ucPortSelect].bDPCheckFlag = FALSE;
	            gDPInfo[ucPortSelect].uwDPStableCount = 0;
#endif            
                gDPInfo[ucPortSelect].bDPLoseCDRLockIRQ= FALSE;
                gDPInfo[ucPortSelect].bDPInterlaneSkewIRQ= FALSE;
                gDPInfo[ucPortSelect].bDPTrainingP0T = TRUE;
                gDPInfo[ucPortSelect].bDPTrainingFlag = TRUE;

                //mhal_DP_HDCPReset(ucPortSelect, ucHBR2Select);
            }

            if(mhal_DP_GetTrainingPattern2Flag(ucPortSelect, ucHBR2Select))
            {
                gDPInfo[ucPortSelect].bDPTrainingP1T = TRUE;
                gDPInfo[ucPortSelect].bDPTrainingFlag = TRUE;
            }

#if(DP_HDCP2_FUNCTION_SUPPORT)
            if(mhal_DP_GetHDCP2AKEInitFlag(ucPortSelect))
            {
                SET_DP_HDCP2_RX_MSG_ID_PORT(ucPortSelect, DP_HDCP2_MSG_ID_AKE_INIT);
                mhal_DP_HDCP2SetRxStatus(ucPortSelect, DP_HDCP2_RXSTATUS_REAUTH_REQ |DP_HDCP2_RXSTATUS_LINK_INTEGRITY_FAILURE, FALSE);
            }
            else if(mhal_DP_GetHDCP2NoStoredKmFlag(ucPortSelect))
            {
                SET_DP_HDCP2_RX_MSG_ID_PORT(ucPortSelect, DP_HDCP2_MSG_ID_AKE_NO_STORED_KM);
            }
            else if(mhal_DP_GetHDCP2StoredKmFlag(ucPortSelect))
            {
                SET_DP_HDCP2_RX_MSG_ID_PORT(ucPortSelect, DP_HDCP2_MSG_ID_AKE_STORED_KM);
            }
            else if(mhal_DP_GetHDCP2LCInitFlag(ucPortSelect))
            {
                SET_DP_HDCP2_RX_MSG_ID_PORT(ucPortSelect, DP_HDCP2_MSG_ID_LC_INIT);
            }
            else if(mhal_DP_GetHDCP2SKESendFlag(ucPortSelect))
            {
                SET_DP_HDCP2_RX_MSG_ID_PORT(ucPortSelect, DP_HDCP2_MSG_ID_SKE_SEND_EKS);
            }

#endif
        }
    }
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_DP_Handler()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_DP_Handler(void)
{
    BYTE ucPortSelect = 0;
    BYTE ucHBR2Select = 0;

    for(ucPortSelect = DP_INPUT_SELECT_PORT0; ucPortSelect < DP_INPUT_SELECT_MASK; ucPortSelect++)
    {
        if(GET_DP_FUNCTION_ENABLE_PORT(ucPortSelect))
        {
            ucHBR2Select = mdrv_DP_GetHBR2PortSelect(ucPortSelect);

            _mdrv_DP_EnableHPD(ucPortSelect, GET_DP_FUNCTION_ENABLE_INDEX());

            if(GET_DP_DISPLAY_ENABLE_PORT(ucPortSelect))  // Only on-Line run
            {
			
    			_mdrv_DP_GUIDPollingProc(ucPortSelect);

                if(_mdrv_DP_StateHandler(ucPortSelect, ucHBR2Select)) // DP_STATE_NORMAL
                {
                    _mdrv_DP_CheckErrorCount(ucPortSelect, ucHBR2Select);

                    _mdrv_DP_StablePolling(ucPortSelect, ucHBR2Select);
#if(!COMBO_DP_DAISY_CHAIN_SUPPORT)
	                _mdrv_DP_CheckInterlaceInverse(ucPortSelect, ucHBR2Select);
#endif
                    _mdrv_DP_AudioHandler(ucPortSelect, ucHBR2Select);
                }
                else
                {
			#if  DP_NoInput_Audio
			_mdrv_DP_NoAudioOutputProc(ucPortSelect,ucHBR2Select);
			#endif
                }
                 
            }

#if(DP_HDCP2_FUNCTION_SUPPORT)
            _mdrv_DP_HDCP2Handler(ucPortSelect);
#endif

#if(DP_AUX_DEBUG_ENABLE)
            _mdrv_DP_AuxDebugProc(ucPortSelect);
#endif
        }
    }
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_DP_TimerHandler()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_DP_TimerHandler(void)
{
    BYTE ucPortSelect = 0;

    for(ucPortSelect = DP_INPUT_SELECT_PORT0; ucPortSelect < DP_INPUT_SELECT_MASK; ucPortSelect++)
    {
        if(GET_DP_FUNCTION_ENABLE_PORT(ucPortSelect))
        {
            if(gDPInfo[ucPortSelect].uwDPRxStableTimeout > 0)
            {
                gDPInfo[ucPortSelect].uwDPRxStableTimeout--;
            }

            if(gDPInfo[ucPortSelect].uwCDRHPDCnt > 0)
            {
                gDPInfo[ucPortSelect].uwCDRHPDCnt--;
            }

            if(gDPInfo[ucPortSelect].uwSkewHPDCnt > 0)
            {
                gDPInfo[ucPortSelect].uwSkewHPDCnt--;
            }

            if(gDPInfo[ucPortSelect].uwHDCPCnt > 0)
            {
                gDPInfo[ucPortSelect].uwHDCPCnt--;
            }

            if(gDPInfo[ucPortSelect].bPacketTimer > 0)
            {
                gDPInfo[ucPortSelect].bPacketTimer--;
            }

            if(gDPInfo[ucPortSelect].uwDPStableCount > 0)
            {
                gDPInfo[ucPortSelect].uwDPStableCount--;
            }

            if(gDPInfo[ucPortSelect].ucInterlaceCount> 0)
            {
                gDPInfo[ucPortSelect].ucInterlaceCount--;
            }

#if DP_SSC_ENABLE
            if(gDPInfo[ucPortSelect].DPAudioAdjustCnt > 0)
            {
                gDPInfo[ucPortSelect].DPAudioAdjustCnt--;
            }

#endif
        }
    }
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_DP_GetColorSpace()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE mdrv_DP_GetColorSpace(BYTE ucInputPort)
{
    return gDPInfo[ucInputPort].ucDPColorFormat;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_DP_GetColorRange()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE mdrv_DP_GetColorRange(BYTE ucInputPort)
{
    return gDPInfo[ucInputPort].ucDPRangeFormat;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_DP_GetColorimetry()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE mdrv_DP_GetColorimetry(BYTE ucInputPort)
{
    return gDPInfo[ucInputPort].ucColorType;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_DP_GetColorDepthInfo()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//  pPacketData[0]    ªº [7:5] bit
//  000 : 6 bit
//  001 : 8 bit
//  010 : 10bit
//  011 : 12bit
//  100 : 16 bit
//**************************************************************************
BYTE mdrv_DP_GetColorDepthInfo(BYTE ucInputPort)
{
	return (gDPInfo[ucInputPort].DPMISC0 >> 5)&0x07;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_DP_GetDPMisc()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_DP_Get_Misc(BYTE ucInputPort, BYTE *pPacketData)
{
    pPacketData[0] = gDPInfo[ucInputPort].DPMISC0;
    pPacketData[1] = gDPInfo[ucInputPort].DPMISC1;
}

//**************************************************************************
//  [Function Name]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void  mdrv_DP_AudioInfo(BYTE ucInputPort, BYTE *pPacketData)
{
       BYTE ucHBR2Select = mdrv_DP_GetHBR2PortSelect(ucInputPort);
	   
	 mhal_DP_AudioInfo(ucInputPort, ucHBR2Select, pPacketData);
}	

//**************************************************************************
//  [Function Name]:
//                  mdrv_DP_GetCableDetectPort()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mdrv_DP_GetCableDetectPort(BYTE ucInputPort)
{
    return mhal_DP_CableDetect(ucInputPort);
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_DP_ConfigAudioPort()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_DP_ConfigAudioPort(BYTE ucInputPort, Bool bEnable)
{
    BYTE ucPortSelect = 0;

    for(ucPortSelect = DP_INPUT_SELECT_PORT0; ucPortSelect < DP_INPUT_SELECT_MASK; ucPortSelect++)
    {
        if(GET_DP_FUNCTION_ENABLE_PORT(ucPortSelect))
        {
            if(ucPortSelect == ucInputPort)
            {
                gDPInfo[ucPortSelect].bAudioEnable = bEnable;
            }
            else
            {
                gDPInfo[ucPortSelect].bAudioEnable = FALSE;
            }
        }
    }

    if(bEnable)
    {
        gDPInfo[ucInputPort].bDPAUPLLBIGChange = TRUE;

        mhal_DP_AUPLLSetting(ucInputPort);
    }
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_DP_CheckAudioStatus()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:   1 : Audio Exit
//                  0 :  no Audio
//**************************************************************************
Bool mdrv_DP_CheckAudioStatus(BYTE ucInputPort)
{
    Bool bAudioFlag = FALSE;

    if((gDPInfo[ucInputPort].InputPacketStatus &(BIT5| BIT6)) == (BIT5|BIT6))
    {
        bAudioFlag = TRUE;
    }

    return bAudioFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_DP_SetGuardBand()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_DP_SetGuardBand(BYTE ucInputPort, WORD usGuardBandValue)
{
    mhal_DP_SetGuardBand(ucInputPort, mdrv_DP_GetHBR2PortSelect(ucInputPort), usGuardBandValue);
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_DP_GetHVInformation()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_DP_GetHVInformation(BYTE ucInputPort, WORD *usHTotalValue, WORD *usVTotalValue)
{
    *usHTotalValue = gDPInfo[ucInputPort].uwDPHtotal;
    *usVTotalValue = gDPInfo[ucInputPort].uwDPVtotal;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_DP_GetInterlaceFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE mdrv_DP_GetInterlaceFlag(BYTE ucInputPort)
{
    return gDPInfo[ucInputPort].bDPInterlace;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_DP_CheckIgnoreDPMS()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mdrv_DP_CheckIgnoreDPMS(BYTE ucInputPort)
{
    Bool bIgnoreFlag = FALSE;
    BYTE ucHBR2Select = mdrv_DP_GetHBR2PortSelect(ucInputPort);

    if(mhal_DP_CheckCDRLock(ucInputPort, ucHBR2Select))
    {
        bIgnoreFlag = TRUE;
    }
    else if(mhal_DP_CheckReceiveIdlePattern(ucInputPort, ucHBR2Select)) // Idle pattern receive
    {
        bIgnoreFlag = TRUE;
    }
    else if(mhal_DP_CheckDPCDPowerState(ucInputPort)) // DPCD power state
    {
        bIgnoreFlag = TRUE;
    }

    return bIgnoreFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mapi_DP_GetSplitFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mdrv_DP_GetSplitFlag(BYTE ucInputPort)
{
    return gDPInfo[ucInputPort].bDPSplitEn;
}

void mdrv_DP_ClrSplitFlag(BYTE ucInputPort)
{
    gDPInfo[ucInputPort].bDPSplitEn=0;;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_DP_GetAstorPTGInterlacePatch()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mdrv_DP_GetAstorPTGInterlacePatch(BYTE ucInputPort)
{
    Bool bFlag = FALSE;
    BYTE ucHBR2Select = mdrv_DP_GetHBR2PortSelect(ucInputPort);

    if(gDPInfo[ucInputPort].bDPInterlace) // Restore
    {
        mhal_DP_OverwriteInterlaceMode(ucInputPort, ucHBR2Select, FALSE);

        bFlag = FALSE;
    }
    else
    {
        if((gDPInfo[ucInputPort].uwDPHWidth %720 == 0) && (gDPInfo[ucInputPort].uwDPVWidth == 480 /2)) // Check 480i
        {
            bFlag = TRUE;
        }
        else if((gDPInfo[ucInputPort].uwDPHWidth %720 == 0) && (gDPInfo[ucInputPort].uwDPVWidth == 567 /2)) // Check 576i
        {
            bFlag = TRUE;
        }
        else if((gDPInfo[ucInputPort].uwDPHWidth == 1920) && (gDPInfo[ucInputPort].uwDPVWidth == 1080 /2)) // Check 1080i
        {
            bFlag = TRUE;
        }

        if(bFlag)
        {
            mhal_DP_OverwriteInterlaceMode(ucInputPort, ucHBR2Select, TRUE);
        }
    }

    return bFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_DP_GetAstorPTGInterlaceFieldPatch()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mdrv_DP_GetAstorPTGInterlaceFieldPatch(BYTE ucInputPort)
{
    BYTE ucHBR2Select = mdrv_DP_GetHBR2PortSelect(ucInputPort);
    Bool bFieldFlag = 0, bFieldToggle = 0;
    BYTE ucFlagCount = 30;

    if(gDPInfo[ucInputPort].bDPInterlace)
    {
		bFieldFlag = mhal_DP_GetFieldFlag(ucInputPort, ucHBR2Select);
		
		while(ucFlagCount > 0)
		{
			Delay1ms(1);
		
			if(bFieldFlag != mhal_DP_GetFieldFlag(ucInputPort, ucHBR2Select))
			{
				bFieldToggle = 1;
				break;				
			}
			
			ucFlagCount--;
		}

		if(bFieldToggle == 0)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
    }
	else
		return FALSE;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_DP_CheckSourceLock()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mdrv_DP_CheckSourceLock(BYTE ucInputPort)
{
    Bool bSourceLock = FALSE;

    if((gDPInfo[ucInputPort].ucState > DP_STATE_CKECKLOCK) && ((gDPInfo[ucInputPort].ucState <= DP_STATE_NORMAL)))
    {
        bSourceLock = TRUE;
    }

    return bSourceLock;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_DP_Set_HPD()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_DP_Set_HPD(BYTE ucInputPort, Bool bSetHPD)
{
    mhal_DP_HPDControl(ucInputPort, bSetHPD);
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_DP_Set_Version()
//  [Description]
//                  mdrv_DP_Set_Version
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_DP_Set_Version(BYTE ucInputPort, BYTE ucVersion)
{
    BYTE ucHBR2Select = mdrv_DP_GetHBR2PortSelect(ucInputPort);   //scaler Port to Combo

	mhal_DP_VersionSetting(ucInputPort, ucHBR2Select, ucVersion);
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_DP_Set_ShortHPDFlag()
//  [Description]
//                  mdrv_DP_Set_ShortHPDFlag
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
 #if ENABLE_DP_ShortHPD_PM	
void mdrv_DP_Set_ShortHPDFlag(BYTE ucInputPort, Bool bEnable)
{
        bHPDShortPulse[ucInputPort] = bEnable;
 
}
 #endif
//**************************************************************************
//  [Function Name]:
//                  mdrv_DP_GetDRRFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mdrv_DP_GetDRRFlag(BYTE ucInputPort)
{
    BYTE ucHBR2Select = mdrv_DP_GetHBR2PortSelect(ucInputPort);

    return mhal_DP_GetDRRFlag(ucInputPort, ucHBR2Select);
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_DP_EnableDRRFunction()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_DP_EnableDRRFunction(BYTE ucInputPort, Bool bEnable)
{
    BYTE ucHBR2Select = mdrv_DP_GetHBR2PortSelect(ucInputPort);

	mhal_DP_EnableDRRFunction(ucInputPort, ucHBR2Select, bEnable);
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_DP_GetPixelClock()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//                  Pixel clock = xxx MHz
//**************************************************************************
WORD mdrv_DP_GetPixelClock(BYTE ucInputPort)
{
    BYTE ucHBR2Select = mdrv_DP_GetHBR2PortSelect(ucInputPort);

	return mhal_DP_GetTimingPixelClock(ucInputPort, ucHBR2Select);
}


//**************************************************************************
//  [Function Name]:
//                  mdrv_DP_GetPixelClock_10KHz()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//                  Pixel clock = XXX x 10KHz, not MHz
//**************************************************************************
WORD mdrv_DP_GetPixelClock_10KHz(BYTE ucInputPort)
{
    BYTE ucHBR2Select = mdrv_DP_GetHBR2PortSelect(ucInputPort);

	return mhal_DP_GetTimingPixelClock_10KHz(ucInputPort, ucHBR2Select);
}


//**************************************************************************
//  [Function Name]:
//                  mdrv_DP_GetMSAChgFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mdrv_DP_GetMSAChgFlag(BYTE ucInputPort)
{
	 BYTE ucHBR2Select = mdrv_DP_GetHBR2PortSelect(ucInputPort);
	 
       return mhal_DP_GetMSAChgFlag(ucInputPort,ucHBR2Select);
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_DP_ClrMSAChgFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_DP_ClrMSAChgFlag(BYTE ucInputPort)
{

   BYTE ucHBR2Select = mdrv_DP_GetHBR2PortSelect(ucInputPort);
   
   mhal_DP_ClrMSAChgFlag(ucInputPort,ucHBR2Select);
}


//**************************************************************************
//  [Function Name]:
//                  mdrv_DP_ClrMSAChgFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mdrv_DP_CheckDPLock(BYTE ucInputPort)
{
	BYTE ucHBR2Select = mdrv_DP_GetHBR2PortSelect(ucInputPort);
	return mhal_DP_CheckCDRLock(ucInputPort, ucHBR2Select);	
}


//**************************************************************************
//  [Function Name]:
//                  mdrv_DP_GetAudioFreq()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE mdrv_DP_GetAudioFreq(BYTE ucInputPort)
{
	BYTE ucHBR2Select = mdrv_DP_GetHBR2PortSelect(ucInputPort);
	
	return  mhal_DP_CalAudioFreq(ucInputPort, ucHBR2Select);
}


//**************************************************************************
//  [Function Name]:
//                  mdrv_DP_GetAudioFreq()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_DP_PowerOnOff(BYTE ucInputPort, Bool Enable)
{
	BYTE ucHBR2Select = mdrv_DP_GetHBR2PortSelect(ucInputPort);
	
	  mhal_DP_PowerOnOff(ucInputPort, ucHBR2Select,Enable);
}
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MSCombo_DP_AUDIO_FORMAT  mdrv_DP_IsAudioFmtPCM(BYTE ucInputPort)
 {
 	BYTE ucHBR2Select = mdrv_DP_GetHBR2PortSelect(ucInputPort);
	return mhal_DP_IsAudioFmtPCM(ucInputPort,ucHBR2Select);
 }

#if  DP_NoInput_Audio	
//**************************************************************************
//  [Function Name]:
//                  mdrv_DP_DPRxAudioFreeRun()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_DP_DPRxAudioFreeRun(BYTE ucInputPort)
{
	BYTE ucHBR2Select = mdrv_DP_GetHBR2PortSelect(ucInputPort);

       mhal_DP_FreeRunSetting(ucInputPort, ucHBR2Select);
}
#endif
#if(DP_HDCP2_FUNCTION_SUPPORT)
//**************************************************************************
//  [Function Name]:
//                  mdrv_DP_SetHDCP2CallBackFunction()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_DP_SetHDCP2CallBackFunction(DP_HDCP2_CALL_BACK_FUNC pHDCP2RxFunc)
{
    pmdrv_DP_HDCP2RxEventProc = pHDCP2RxFunc;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_DP_HDCP2TxEventProc()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_DP_HDCP2TxEventProc(BYTE ucPortSelect, BYTE ucMessageID, BYTE *pMessageData)
{
    SET_DP_HDCP2_TX_MSG_ID_PORT(ucPortSelect, ucMessageID);
    SET_DP_HDCP2_TX_QUEUE_ADDRESS(pMessageData);
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_DP_HDCP2SetCertRx()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_DP_HDCP2SetCertRx(BYTE *pHDCP2CertRx)
{
    BYTE ucPortSelect = 0;

    for(ucPortSelect = DP_INPUT_SELECT_PORT0; ucPortSelect < DP_INPUT_SELECT_MASK; ucPortSelect++)
    {
        if(GET_DP_FUNCTION_ENABLE_PORT(ucPortSelect))
        {
            mhal_DP_HDCP2SetTxData(ucPortSelect, DP_HDCP2_INFO_AKE_CERTRX, pHDCP2CertRx);
        }
    }
}

#endif

#endif // _MDRV_DPRX_C_


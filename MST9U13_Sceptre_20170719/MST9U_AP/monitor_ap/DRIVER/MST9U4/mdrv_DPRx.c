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
#if ENABLE_DAC
#include "drvAudio.h"
#endif
//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define DP_DRV_DEBUG_MESSAGE           	0
#define DP_AUX_DEBUG_ENABLE             	0

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

BYTE ucDPHBR20PortSelect = 0;
BYTE ucDPHBR21PortSelect = 0;
BYTE ucDPFunctionEnableIndex = 0;
BYTE ucDPDisplayEnableIndex = 0;
BYTE ucDPSystemIndex = 0;
BYTE ucDPMSTEnableIndex = 0;

DP_INFO gDPInfo[DPRx_ID_MAX];


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
DPRx_ID _mdrv_DPRx_PortNumber2DPRxID(BYTE ucInputPort)
{			 

    switch(ucInputPort)
    {
        case DP_INPUT_SELECT_PORT0:
              return DPRx_ID_0;
            break;

        case DP_INPUT_SELECT_PORT1:
             return DPRx_ID_1;
            break;

        case DP_INPUT_SELECT_PORT2:
             return DPRx_ID_2;
            break;    

        default:
		  return DPRx_ID_MAX;
            break;
     };
    
}
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

Bool _mdrv_DP_StateHandler(BYTE dprx_id, BYTE ucHBR2Select)
{
    Bool bSignalStable = FALSE;
    ucHBR2Select =0;
    
    switch(gDPInfo[dprx_id].ucState)
    {

		case DP_STATE_INITIAL:

			if(GET_DP_RX_SYSYEM_STATE(dprx_id))

			 gDPInfo[dprx_id].ucState = DP_STATE_NORMAL;
			
		break;

		case DP_STATE_NORMAL:

			if(GET_DP_RX_SYSYEM_STATE(dprx_id)==0)
			{
				gDPInfo[dprx_id].ucState = DP_STATE_INITIAL;

			}
			else
			{
				gDPInfo[dprx_id].ucState = DP_STATE_NORMAL;	
			
				bSignalStable = TRUE;
			}
			
		break;

		default:

		break;
		
    }
    return bSignalStable;
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
void _mdrv_DP_RegenTimingProc(DPRx_ID dprx_id, BYTE ucHBR2Select)
{
    WORD usBlank = 0;
    WORD usWidth = 0;
    WORD usFrontPorch = 0;
    WORD usBackPorch = 0;
    WORD usTimingInfo[DP_CHECK_TIMING_QUEUE_SIZE] = {0};  

    gDPInfo[dprx_id].bDPInterlace = mhal_DP_GetTimingInformation(dprx_id, ucHBR2Select, usTimingInfo);
    gDPInfo[dprx_id].uwDPHtotal = usTimingInfo[0];
    gDPInfo[dprx_id].uwDPHWidth = usTimingInfo[2];
    gDPInfo[dprx_id].uwDPVtotal = usTimingInfo[1];
    gDPInfo[dprx_id].uwDPVWidth = usTimingInfo[3];

    // V parameter
    usBlank = gDPInfo[dprx_id].uwDPVtotal - gDPInfo[dprx_id].uwDPVWidth;
    usWidth = usBlank /2;
    usFrontPorch = (usBlank - usWidth) /2;
    usBackPorch = usBlank - usWidth - usFrontPorch;

    usTimingInfo[0] = usWidth;
    usTimingInfo[1] = usFrontPorch;
    usTimingInfo[2] = usBackPorch;

	if(usBlank < 0x400) 
	{ 

	    if((gDPInfo[dprx_id].uwDPHWidth > 3000)||(gDPInfo[dprx_id].uwDPVWidth > 3000)||(mhal_DP_GetTimingPixelClock(dprx_id, ucHBR2Select) > 300))
	    {	  
	            usWidth = 0x34;	
	    }
	    else
	    {
	            usWidth = 0x2C;
	    }

	    usTimingInfo[3] = usWidth;

	    mhal_DP_RegenTimingInformation(dprx_id, ucHBR2Select, usTimingInfo);
	}
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
void _mdrv_DP_StablePolling(DPRx_ID dprx_id, BYTE ucHBR2Select)
{	

       WORD usPollingInfo[DP_STABLE_POLLING_QUEUE_SIZE] = {0};
	 BYTE usColorSpace = DP_COLOR_FORMAT_UNKNOWN;
    
       if(gDPInfo[dprx_id].bPacketTimer == 0)
	{
	    gDPInfo[dprx_id].bPacketTimer = 100;

	    gDPInfo[dprx_id].InputPacketStatus = mhal_DP_GetPacketStatus(dprx_id, ucHBR2Select);
	}

	    mhal_DP_GetMISC01(dprx_id, ucHBR2Select, usPollingInfo);
	    
	    gDPInfo[dprx_id].DPMISC0 = usPollingInfo[0];
	    
	    gDPInfo[dprx_id].DPMISC1 = usPollingInfo[1];

	 if(gDPInfo[dprx_id].DPMISC1&BIT6)	// ignore MISC 0 
	 {			
		mhal_DP_GetVscSdp(dprx_id, ucHBR2Select, usPollingInfo);	

		gDPInfo[dprx_id].ucDPColorDepth = usPollingInfo[1]&MASKBIT(2:0);

		switch(usPollingInfo[0] >>4)
		{
		case 0:
		gDPInfo[dprx_id].ucDPColorFormat = DP_COLOR_FORMAT_RGB;
		break;

		case 1:
		gDPInfo[dprx_id].ucDPColorFormat = DP_COLOR_FORMAT_YUV_444;
		break;

		case 2:
		gDPInfo[dprx_id].ucDPColorFormat = DP_COLOR_FORMAT_YUV_422;
		break;

		case 3:
		gDPInfo[dprx_id].ucDPColorFormat = DP_COLOR_FORMAT_YUV_420;
		break;

		case 4:
		gDPInfo[dprx_id].ucDPColorFormat = DP_COLOR_FORMAT_YONLY;
		break;	

		case 5:
		gDPInfo[dprx_id].ucDPColorFormat = DP_COLOR_FORMAT_RAW;
		break;
		
		default:

		break;
		};
			
	 }
	 else
	 {
			gDPInfo[dprx_id].ucDPColorDepth = gDPInfo[dprx_id].DPMISC0 >> 5;
				
		     if(((gDPInfo[dprx_id].DPMISC0 &MASKBIT(3:1))) == BIT3)   // AdobeRGB
		     {

					gDPInfo[dprx_id].ucDPColorFormat = DP_COLOR_FORMAT_RGB;

					gDPInfo[dprx_id].ucDPRangeFormat = DP_COLOR_RANGE_FULL;

					gDPInfo[dprx_id].ucColorType = DP_COLORIMETRY_ADOBERBG;

		     }
		     else
		     {
	 	
			    	    usColorSpace = (gDPInfo[dprx_id].DPMISC0 &MASKBIT(2:1)) >> 1;

				    switch(usColorSpace)
    {
        case 0:
            gDPInfo[dprx_id].ucDPColorFormat = DP_COLOR_FORMAT_RGB;
            break;

        case 1:
            gDPInfo[dprx_id].ucDPColorFormat = DP_COLOR_FORMAT_YUV_422;
            break;

        case 2:
            gDPInfo[dprx_id].ucDPColorFormat = DP_COLOR_FORMAT_YUV_444;
            break;

        default:

            break;
    };


				    if((gDPInfo[dprx_id].DPMISC0&BIT3) == BIT3)
				    {
				       gDPInfo[dprx_id].ucDPRangeFormat = DP_COLOR_RANGE_LIMIT;
				    }
				    else
				    {
				        gDPInfo[dprx_id].ucDPRangeFormat = DP_COLOR_RANGE_FULL;
				    }

				    if((gDPInfo[dprx_id].DPMISC0&BIT4) == BIT4)
				    {
				        gDPInfo[dprx_id].ucColorType = DP_COLORIMETRY_xvYCC709;
				    }
				    else
				    {
				        gDPInfo[dprx_id].ucColorType = DP_COLORIMETRY_xvYCC601;
				    }
		   }
    }
  
    
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
void _mdrv_DP_AudioHandler(DPRx_ID dprx_id, BYTE ucHBR2Select)
{	 
         ucHBR2Select = dprx_id;

         return;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_DP_SetConfiguration() // cyc : need to modify
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_DP_SetConfiguration(BYTE ucHBR20EnableIndex, BYTE ucDaisyEnableIndex, BYTE ucDPEnableIndex, BYTE ucDisplayEnableIndex)
{
    if(!GET_DP_SYSTEM_CONFIG_FINISH())
    {
        ucDPHBR20PortSelect = ucHBR20EnableIndex;
        ucDPMSTEnableIndex = ucDaisyEnableIndex;       
    }
    
     ucDPFunctionEnableIndex = ucDPEnableIndex;
     ucDPDisplayEnableIndex = ucDisplayEnableIndex;

     return;	
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
BYTE mdrv_DP_GetHBR2PortSelect(BYTE ucInputPort)
{	
       DPRx_ID dprx_id = _mdrv_DPRx_PortNumber2DPRxID(ucInputPort);		
	
	dprx_id =DPRx_ID_0;
	
	if(!GET_DP_SYSTEM_CONFIG_FINISH())
	{	
	     SET_DP_SYSTEM_CONFIG_FINISH();
	}

	return 0;
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
    DPRx_ID dprx_id = DPRx_ID_0;

     for(dprx_id = DPRx_ID_0; dprx_id < DPRx_ID_MAX; dprx_id++)
    {
        if(GET_DP_FUNCTION_ENABLE_PORT(dprx_id))
        {
               
            if(gDPInfo[dprx_id].bPacketTimer > 0)
            {
                gDPInfo[dprx_id].bPacketTimer--;
            }        

        }
    }
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
	 memset(&gDPInfo, 0, sizeof(gDPInfo));
  	return;
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
    DPRx_ID dprx_id = DPRx_ID_0;

   for(dprx_id = DPRx_ID_0; dprx_id < DPRx_ID_MAX; dprx_id++)
    {
        if(GET_DP_FUNCTION_ENABLE_PORT(dprx_id))
        {
            if(mhal_DP_GetMCCSReceiveFlag(dprx_id))
            {

            }
        }
    }
    return;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_DP_GetCableDetectPort() // cyc : need to modify
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_DP_IsrHandler(void)
{
	DPRx_ID dprx_id = DPRx_ID_0;

	for(dprx_id = DPRx_ID_0; dprx_id < DPRx_ID_MAX; dprx_id++)
	{

	}

	return;
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
    DPRx_ID dprx_id = DPRx_ID_0;	
    BYTE ucHBR2Select = 0;

    for(dprx_id = DPRx_ID_0; dprx_id < DPRx_ID_MAX; dprx_id++)
    {
        if(GET_DP_FUNCTION_ENABLE_PORT(dprx_id))
        {            
            if(GET_DP_DISPLAY_ENABLE_PORT(dprx_id))  // Only on-Line run
            {			

			if(_mdrv_DP_StateHandler(dprx_id, ucHBR2Select)) // DP_STATE_NORMAL
			{
				_mdrv_DP_StablePolling(dprx_id, ucHBR2Select);                  
			}
			else
			{

			}
            }

        }
    }

     return;
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

    DPRx_ID dprx_id = _mdrv_DPRx_PortNumber2DPRxID(ucInputPort);		

    return gDPInfo[dprx_id].ucDPColorFormat;
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
    DPRx_ID dprx_id = _mdrv_DPRx_PortNumber2DPRxID(ucInputPort);			

    return gDPInfo[dprx_id].ucDPRangeFormat;
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
	DPRx_ID dprx_id = _mdrv_DPRx_PortNumber2DPRxID(ucInputPort);		

	return gDPInfo[dprx_id].ucColorType;
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
	DPRx_ID dprx_id = _mdrv_DPRx_PortNumber2DPRxID(ucInputPort);		

	return (gDPInfo[dprx_id].ucDPColorDepth)&0x07;
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
	DPRx_ID dprx_id = _mdrv_DPRx_PortNumber2DPRxID(ucInputPort);		

	pPacketData[0] = gDPInfo[dprx_id].DPMISC0;
	pPacketData[1] = gDPInfo[dprx_id].DPMISC1;

	 return;
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
     DPRx_ID dprx_id = _mdrv_DPRx_PortNumber2DPRxID(ucInputPort);	

     return mhal_DP_CableDetect(dprx_id);
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

    DPRx_ID dprx_id = _mdrv_DPRx_PortNumber2DPRxID(ucInputPort);	
    BYTE rx_id = 0;


     for(rx_id = DPRx_ID_0; rx_id < DPRx_ID_MAX; rx_id++)
    {
        if(GET_DP_FUNCTION_ENABLE_PORT(dprx_id))
        {
            if(rx_id == dprx_id)
            {
                gDPInfo[rx_id].bAudioEnable = bEnable;
            }
            else
            {
                gDPInfo[rx_id].bAudioEnable = FALSE;
            }
        }
    }

    if(bEnable)
    {
        gDPInfo[dprx_id].bDPAUPLLBIGChange = TRUE;

        mhal_DP_AUPLLSetting(dprx_id);
    }

     return;
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
    DPRx_ID dprx_id = _mdrv_DPRx_PortNumber2DPRxID(ucInputPort);		
    Bool bAudioFlag = FALSE;

    if((gDPInfo[dprx_id].InputPacketStatus &(BIT5| BIT6)) == (BIT5|BIT6))
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
	DPRx_ID dprx_id = _mdrv_DPRx_PortNumber2DPRxID(ucInputPort);	

	mhal_DP_SetGuardBand(dprx_id, mdrv_DP_GetHBR2PortSelect(ucInputPort), usGuardBandValue);

	return;
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
	DPRx_ID dprx_id = _mdrv_DPRx_PortNumber2DPRxID(ucInputPort);	

	*usHTotalValue = gDPInfo[dprx_id].uwDPHtotal;
	*usVTotalValue = gDPInfo[dprx_id].uwDPVtotal;

	return;
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
    DPRx_ID dprx_id = _mdrv_DPRx_PortNumber2DPRxID(ucInputPort);		
    BYTE ucHBR2Select = mdrv_DP_GetHBR2PortSelect(ucInputPort);

    return mhal_DP_GetInterlaceFlag(dprx_id, ucHBR2Select);
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
    DPRx_ID dprx_id = _mdrv_DPRx_PortNumber2DPRxID(ucInputPort);		
    Bool bIgnoreFlag = FALSE;
    BYTE ucHBR2Select = mdrv_DP_GetHBR2PortSelect(ucInputPort);

    if(mhal_DP_CheckCDRLock(dprx_id, ucHBR2Select))
    {
        bIgnoreFlag = TRUE;
    }
    else if(mhal_DP_CheckReceiveIdlePattern(dprx_id, ucHBR2Select)) // Idle pattern receive
    {
        bIgnoreFlag = TRUE;
    }
    else if(mhal_DP_CheckDPCDPowerState(dprx_id)) // DPCD power state
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
    DPRx_ID dprx_id = _mdrv_DPRx_PortNumber2DPRxID(ucInputPort);		

    return gDPInfo[dprx_id].bDPSplitEn;	
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
    DPRx_ID dprx_id = _mdrv_DPRx_PortNumber2DPRxID(ucInputPort);	
    BYTE ucHBR2Select = mdrv_DP_GetHBR2PortSelect(ucInputPort);

    dprx_id = DPRx_ID_0;
    ucHBR2Select =0;
    
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
    DPRx_ID dprx_id = _mdrv_DPRx_PortNumber2DPRxID(ucInputPort);	
    BYTE ucHBR2Select = mdrv_DP_GetHBR2PortSelect(ucInputPort);

     dprx_id = DPRx_ID_0;
     ucHBR2Select =0;
    
    return 1;
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
    DPRx_ID dprx_id = _mdrv_DPRx_PortNumber2DPRxID(ucInputPort);	
    Bool bSourceLock = FALSE;

    if((gDPInfo[dprx_id].ucState > DP_STATE_CKECKLOCK) && ((gDPInfo[dprx_id].ucState <= DP_STATE_NORMAL)))
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
     DPRx_ID dprx_id = _mdrv_DPRx_PortNumber2DPRxID(ucInputPort);	

     mhal_DP_HPDControl(dprx_id, bSetHPD);

     return;
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
    DPRx_ID dprx_id = _mdrv_DPRx_PortNumber2DPRxID(ucInputPort);	
    BYTE ucHBR2Select = mdrv_DP_GetHBR2PortSelect(ucInputPort);   //scaler Port to Combo

    mhal_DP_VersionSetting(dprx_id, ucHBR2Select, ucVersion);

    return;
 }

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
	DPRx_ID dprx_id = _mdrv_DPRx_PortNumber2DPRxID(ucInputPort);	
	BYTE ucHBR2Select = mdrv_DP_GetHBR2PortSelect(ucInputPort);

	return mhal_DP_GetDRRFlag(dprx_id , ucHBR2Select);
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
    DPRx_ID dprx_id = _mdrv_DPRx_PortNumber2DPRxID(ucInputPort);	
    BYTE ucHBR2Select = mdrv_DP_GetHBR2PortSelect(ucInputPort);

    mhal_DP_EnableDRRFunction(dprx_id, ucHBR2Select, bEnable);

    return;
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
	DPRx_ID dprx_id = _mdrv_DPRx_PortNumber2DPRxID(ucInputPort);	
	BYTE ucHBR2Select = mdrv_DP_GetHBR2PortSelect(ucInputPort);

	return mhal_DP_GetTimingPixelClock(dprx_id, ucHBR2Select);
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
	DPRx_ID dprx_id = _mdrv_DPRx_PortNumber2DPRxID(ucInputPort);	
	BYTE ucHBR2Select = mdrv_DP_GetHBR2PortSelect(ucInputPort);

	return mhal_DP_GetTimingPixelClock_10KHz(dprx_id, ucHBR2Select);
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
	DPRx_ID dprx_id = _mdrv_DPRx_PortNumber2DPRxID(ucInputPort);	
	BYTE ucHBR2Select = mdrv_DP_GetHBR2PortSelect(dprx_id);
	 
       return mhal_DP_GetMSAChgFlag(dprx_id,ucHBR2Select);
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
	DPRx_ID dprx_id = _mdrv_DPRx_PortNumber2DPRxID(ucInputPort);		
	BYTE ucHBR2Select = mdrv_DP_GetHBR2PortSelect(ucInputPort);

	mhal_DP_ClrMSAChgFlag(dprx_id,ucHBR2Select);

	return;
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
	DPRx_ID dprx_id = _mdrv_DPRx_PortNumber2DPRxID(ucInputPort);
	BYTE ucHBR2Select = mdrv_DP_GetHBR2PortSelect(ucInputPort);

	return mhal_DP_CheckCDRLock(dprx_id, ucHBR2Select);	
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
	DPRx_ID dprx_id = _mdrv_DPRx_PortNumber2DPRxID(ucInputPort);
	BYTE ucHBR2Select = mdrv_DP_GetHBR2PortSelect(ucInputPort);
	
	return  mhal_DP_CalAudioFreq(dprx_id, ucHBR2Select);
}
//**************************************************************************
//  [Function Name]:
//                  mdrv_DP_IsAudioFmtPCM()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MSCombo_DP_AUDIO_FORMAT  mdrv_DP_IsAudioFmtPCM(BYTE ucInputPort)
 {
 	DPRx_ID dprx_id = _mdrv_DPRx_PortNumber2DPRxID(ucInputPort);
 	BYTE ucHBR2Select = mdrv_DP_GetHBR2PortSelect(ucInputPort);

	return mhal_DP_IsAudioFmtPCM(dprx_id,ucHBR2Select);
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
	DPRx_ID dprx_id = _mdrv_DPRx_PortNumber2DPRxID(ucInputPort);
       BYTE ucHBR2Select = mdrv_DP_GetHBR2PortSelect(ucInputPort);
	   
	 mhal_DP_AudioInfo(dprx_id, ucHBR2Select, pPacketData);
}	


//**************************************************************************
//  [Function Name]:
//                  mdrv_DPRx_GetOuputMode()
//  [Description]
//                  mdrv_DPRx_GetOuputMode
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
DPRx_MODE  mdrv_DPRx_GetOuputMode(BYTE ucInputPort)
{
	DPRx_ID dprx_id = _mdrv_DPRx_PortNumber2DPRxID(ucInputPort);

 	return  mhal_DPRx_GetOuputMode( dprx_id);
}

#endif // _MDRV_DPRX_C_


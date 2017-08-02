///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    mapi_DPRx.c
/// @author MStar Semiconductor Inc.
/// @brief  DP Rx driver Function
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MAPI_DPRX_C_
#define _MAPI_DPRX_C_

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "Global.h"
#include "mapi_DPRx.h"
#include "mdrv_DPRx.h"
#include "mhal_DPRx.h"

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define DP_API_DEBUG_MESSAGE           	0

#if(DP_DEBUG_MESSAGE && DP_API_DEBUG_MESSAGE)
#define DP_API_DPUTSTR(str)            	printMsg(str)
#define DP_API_DPRINTF(str, x)         	printData(str, x)
#else
#define DP_API_DPUTSTR(str)
#define DP_API_DPRINTF(str, x)
#endif

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

 
//**************************************************************************
//  [Function Name]:
//                  mapi_DP_SetConfiguration()//cyc : need to modify 
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_DP_SetConfiguration(BYTE ucHBR20EnableIndex, BYTE ucDaisyEnableIndex, BYTE ucDPEnableIndex, BYTE ucDisplayEnableIndex)
{
    mdrv_DP_SetConfiguration(ucHBR20EnableIndex, ucDaisyEnableIndex, ucDPEnableIndex, ucDisplayEnableIndex);
}

//**************************************************************************
//  [Function Name]:
//                  mapi_DP_Initial()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
 void mapi_DP_Initial(void)
{
      mdrv_DP_Initial();
}

//**************************************************************************
//  [Function Name]:
//                  mapi_DP_TimerInterrupt()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_DP_TimerInterrupt(void)
{
    mdrv_DP_TimerHandler();
}

 
//**************************************************************************
//  [Function Name]:
//                  mapi_DP_PMIsrHandler() // cyc : need to modify
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_DP_PMIsrHandler(InterruptNum eIntNum)
{
    UNUSED(eIntNum);

    if(mdrv_DP_GetAuxInterruptFlag())
    {
        mdrv_DP_IsrAuxHandler();
    }
}

//**************************************************************************
//  [Function Name]:
//                  mapi_DP_IsrHandler() // cyc : need to modify
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_DP_IsrHandler(void)
{
    mdrv_DP_IsrHandler();
}

//**************************************************************************
//  [Function Name]:
//                  mapi_DP_Handler()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_DP_Handler(void)
{
    mdrv_DP_Handler();
}

 
//**************************************************************************
//  [Function Name]:
//                  mapi_DP_GetColorFormate() //cyc : need to modify
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
ST_COMBO_COLOR_FORMAT mapi_DP_GetColorFormate(BYTE ucInputPort)
{
    ST_COMBO_COLOR_FORMAT stColorInfo = {COMBO_COLOR_FORMAT_DEFAULT, COMBO_COLOR_RANGE_DEFAULT, COMBO_COLORIMETRY_NONE, COMBO_YUV_COLORIMETRY_ITU601};

    stColorInfo.ucColorType = (EN_COLOR_FORMAT_TYPE)mdrv_DP_GetColorSpace(ucInputPort);
    stColorInfo.ucColorRange = (EN_COLOR_RANGE_TYPE)mdrv_DP_GetColorRange(ucInputPort);
    stColorInfo.ucColorimetry = (EN_COLORIMETRY_TYPE)mdrv_DP_GetColorimetry(ucInputPort);

    return stColorInfo;
}

//**************************************************************************
//  [Function Name]:
//                  mapi_DP_GetColorDepthInfo() //cyc : need to modify
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
BYTE mapi_DP_GetColorDepthInfo(BYTE ucInputPort)
{
	return mdrv_DP_GetColorDepthInfo(ucInputPort);
}

//**************************************************************************
//  [Function Name]:
//                  mapi_DP_GetDPMisc() //cyc : need to modify
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//  
//**************************************************************************
void mapi_DP_Get_Misc(BYTE ucInputPort, BYTE *pPacketData)
{
    mdrv_DP_Get_Misc(ucInputPort, pPacketData);
}

//**************************************************************************
//  [Function Name]:
//                  mapi_DP_GetCableDetectPort()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mapi_DP_GetCableDetectPort(BYTE ucInputPort)
{
    return mdrv_DP_GetCableDetectPort(ucInputPort);
}

//**************************************************************************
//  [Function Name]:
//                  mapi_DP_ConvertHBR2()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE mapi_DP_ComboConvertHBR2(BYTE ucInputPort)
{
	return mdrv_DP_GetHBR2PortSelect(ucInputPort);	
}

//**************************************************************************
//  [Function Name]:
//                  mapi_DP_ConfigAudioPort()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_DP_ConfigAudioPort(BYTE ucInputPort, Bool bEnable)
{
    mdrv_DP_ConfigAudioPort(ucInputPort, bEnable);
}

//**************************************************************************
//  [Function Name]:
//                  mapi_DP_CheckAudioStatus()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:   1 : Audio Exit
//                  0 :  no Audio  
//**************************************************************************
Bool mapi_DP_CheckAudioStatus(BYTE ucInputPort)
{	
    return mdrv_DP_CheckAudioStatus(ucInputPort);
}

//**************************************************************************
//  [Function Name]:
//                  mapi_DP_SetGuardBand()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_DP_SetGuardBand(BYTE ucInputPort, WORD usGuardBandValue)
{
    mdrv_DP_SetGuardBand(ucInputPort, usGuardBandValue);
}

//**************************************************************************
//  [Function Name]:
//                  mapi_DP_GetHVInformation()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_DP_GetHVInformation(BYTE ucInputPort, WORD *usHTotalValue, WORD *usVTotalValue)
{
    mdrv_DP_GetHVInformation(ucInputPort, usHTotalValue, usVTotalValue);
}

//**************************************************************************
//  [Function Name]:
//                  mapi_DP_GetInterlaceFlag()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mapi_DP_GetInterlaceFlag(BYTE ucInputPort)
{
    return mdrv_DP_GetInterlaceFlag(ucInputPort);
}

//**************************************************************************
//  [Function Name]:
//                  mapi_DP_CheckIgnoreDPMS()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mapi_DP_CheckIgnoreDPMS(BYTE ucInputPort)
{
    return mdrv_DP_CheckIgnoreDPMS(ucInputPort);
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
Bool mapi_DP_GetSplitFlag(BYTE ucInputPort)
{
    return mdrv_DP_GetSplitFlag(ucInputPort);
}

//**************************************************************************
//  [Function Name]:
//                  mapi_DP_GetAstorPTGInterlacePatch()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mapi_DP_GetAstorPTGInterlacePatch(BYTE ucInputPort)
{
    return mdrv_DP_GetAstorPTGInterlacePatch(ucInputPort);
}

//**************************************************************************
//  [Function Name]:
//                  mapi_DP_GetAstorPTGInterlaceFieldPatch()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mapi_DP_GetAstorPTGInterlaceFieldPatch(BYTE ucInputPort)
{
    return mdrv_DP_GetAstorPTGInterlaceFieldPatch(ucInputPort);
}

//**************************************************************************
//  [Function Name]:
//                  mapi_DP_CheckSourceLock()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mapi_DP_CheckSourceLock(BYTE ucInputPort)
{
    return mdrv_DP_CheckSourceLock(ucInputPort);
}

//**************************************************************************
//  [Function Name]:
//                  mapi_DP_Set_HPD() // cyc : need mailbox
//  [Description]
//                  mapi_DP_Set_HPD
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_DP_Set_HPD(BYTE ucInputPort, Bool bSetHPD)
{
	mdrv_DP_Set_HPD(ucInputPort, bSetHPD);
}

//**************************************************************************
//  [Function Name]:
//                  mapi_DP_Set_Version() // cyc : need mailbox
//  [Description]
//                  mapi_DP_Set_Version
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_DP_Set_Version(BYTE ucInputPort, BYTE ucVersion)
{
	mdrv_DP_Set_Version(ucInputPort, ucVersion);
}

//**************************************************************************
//  [Function Name]:
//                  mapi_DP_GetDRRFlag()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mapi_DP_GetDRRFlag(BYTE ucInputPort)
{
    return mdrv_DP_GetDRRFlag(ucInputPort);
}

//**************************************************************************
//  [Function Name]:
//                  mapi_DP_EnableDRRFunction() // cyc : need mailbox
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_DP_EnableDRRFunction(BYTE ucInputPort, Bool bEnable)
{
	mdrv_DP_EnableDRRFunction(ucInputPort, bEnable);
}

//**************************************************************************
//  [Function Name]:
//                  mapi_DP_GetPixelClock()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  Pixel clock = xxx MHz
//**************************************************************************
WORD mapi_DP_GetPixelClock(BYTE ucInputPort)
{
	return mdrv_DP_GetPixelClock(ucInputPort);
}

//**************************************************************************
//  [Function Name]:
//                  mapi_DP_GetPixelClock_10KHz()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  Pixel clock = XXX x 10KHz, not MHz
//**************************************************************************
WORD mapi_DP_GetPixelClock_10KHz(BYTE ucInputPort)
{
	return mdrv_DP_GetPixelClock_10KHz(ucInputPort);
}

//**************************************************************************
//  [Function Name]:
//                  mapi_DP_GetMSAChgFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mapi_DP_GetMSAChgFlag(BYTE ucInputPort)
{
    return mdrv_DP_GetMSAChgFlag(ucInputPort);
}

//**************************************************************************
//  [Function Name]:
//                  mapi_DP_ClrMSAChgFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_DP_ClrMSAChgFlag(BYTE ucInputPort)
{
  
     mdrv_DP_ClrMSAChgFlag(ucInputPort);
}

//**************************************************************************
//  [Function Name]:
//                  mapi_DP_ClrMSAChgFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE mapi_DP_GetAudioFreq(BYTE ucInputPort)
{
	return mdrv_DP_GetAudioFreq(ucInputPort);
}	
//**************************************************************************
//  [Function Name]:
//                  mapi_DP_ClrMSAChgFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************

Bool mapi_DP_CheckDPLock(BYTE ucInputPort)
{
	return  mdrv_DP_CheckDPLock(ucInputPort);
}
//**************************************************************************
//  [Function Name]:
//                  mapi_DP_IsAudioFmtPCM()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MSCombo_DP_AUDIO_FORMAT mapi_DP_IsAudioFmtPCM( BYTE ucInputPort)
{ 
	return  mdrv_DP_IsAudioFmtPCM(ucInputPort);
	
}

//**************************************************************************
                
//  [Arguments]:
//
//  [Return]:
//  
//**************************************************************************
void  mapi_DP_AudioInfo(BYTE ucInputPort, BYTE *pPacketData)
 {
	mdrv_DP_AudioInfo(ucInputPort, pPacketData);
 }	


//**************************************************************************
//  [Function Name]:
//                  mapi_DP_PowerOnOff()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_DP_PowerOnOff(BYTE ucInputPort, Bool Enable)
{
	ucInputPort =0;
	Enable=0;
	//mdrv_DP_PowerOnOff( ucInputPort,  Enable);
}



//**************************************************************************
//  [Function Name]:
//                  mapi_DP_SetPowerStatus()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_DP_SetPowerStatus(BYTE ucPowerStatus)
{   
    if(ucPowerStatus == DP_POWER_STATUS_PS_ON)
    {
 	 
    }
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
DPRx_MODE  mapi_DPRx_GetOuputMode(BYTE ucInputPort)
{
 	return  mdrv_DPRx_GetOuputMode( ucInputPort);
}
#endif // _MAPI_DPRX_C_


#ifndef _MAPI_TMDS_C_
#define _MAPI_TMDS_C_

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "Global.h"
#include "mapi_tmds.h"
#include "mdrv_tmds.h"
#include "mdrv_tmds.c"


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
#ifdef _WORK_ON_PM_
//**************************************************************************
//  [Function Name]:
//                  mapi_tmds_SetConfiguration()
//  [Description]:
//                  use to each combo port type is DVI, Dual or HDMI.
//  [Arguments]:
//                  ucDVIEnableIndex : if bitx = true, means combo(x) is DVI port
//                  ucDualnableIndex :  if bitx = true, means combo(x) is Dual link port
//                  ucHDMIEnableIndex :  if bitx = true, means combo(x) is HDMI port
//                  ucDisplayEnableIndex :   if bitx = true, means combo(x) needs to be display
//  [Return]:
//                  none
//**************************************************************************
void mapi_tmds_SetConfiguration(BYTE ucDVIEnableIndex, BYTE ucDualnableIndex, BYTE ucHDMIEnableIndex, BYTE ucDisplayEnableIndex)
{
    mdrv_tmds_SetConfiguration( ucDVIEnableIndex,  ucDualnableIndex,  ucHDMIEnableIndex,  ucDisplayEnableIndex);
}


//**************************************************************************
//  [Function Name]:
//                  mapi_tmds_Initial()
//  [Description]:
//                  use to initial variable
//  [Arguments]:
//                  none
//  [Return]:
//                  none
//**************************************************************************
void mapi_tmds_Initial(void)
{
    mdrv_tmds_Initial();
}

//**************************************************************************
//  [Function Name]:
//                  mapi_tmds_ConfigAudioPort()
//  [Description]:
//                  configurate which port is going to be the audio source
//  [Arguments]:
//                  none
//  [Return]:
//                  none
//**************************************************************************
void mapi_tmds_ConfigAudioPort(BYTE ucInputPort)
{
    mdrv_tmds_ConfigAudioPort(ucInputPort);
}

//**************************************************************************
//  [Function Name]:
//                  mapi_tmds_Handler()
//  [Description]:
//                  use to polling HDMI status,
//                  ex: input format, packet status, color format
//  [Arguments]:
//                  none
//  [Return]:
//                  none
//**************************************************************************
void mapi_tmds_Handler(void)
{
    mdrv_tmds_Handler();
}

void mapi_tmds_TimerInterrupt(void)
{
    mdrv_tmds_TimerInterrupt();
}

//**************************************************************************
//  [Function Name]:
//                  mapi_tmds_Set_HPD()
//  [Description]:
//                  use to control hot plug pin
//  [Arguments]:
//                  ucComboSelect : combo port
//                  bSetHPD : 1 = HIGH, 0 = LOW
//  [Return]:
//                  none
//**************************************************************************
void mapi_tmds_Set_HPD(BYTE ucComboSelect, MS_BOOL bSetHPD)
{
    mdrv_tmds_Set_HPD(ucComboSelect, bSetHPD);
}


//**************************************************************************
//  [Function Name]:
//                  mapi_tmds_Get_Color_Format()
//  [Description]:
//                  use to get input color format
//  [Arguments]:
//                  enInputPort : combo port
//  [Return]:
//                  typedef struct
//                  {
//                      EN_COLOR_FORMAT_TYPE    ucColorType;
//                      EN_COLOR_RANGE_TYPE     ucColorRange;
//                      EN_COLORIMETRY_TYPE     ucColorimetry;
//                   } ST_COMBO_COLOR_FORMAT;
//
//**************************************************************************
ST_COMBO_COLOR_FORMAT mapi_tmds_Get_Color_Format(BYTE ucInputPort)
{
    return mdrv_tmds_Get_Color_Format(ucInputPort);
}


//**************************************************************************
//  [Function Name]:
//                  mapi_tmds_Input_Is_HDMI()
//  [Description]:
//                  use to check input signal is HDMI or DVI
//  [Arguments]:
//                  enInputPort : combo port
//  [Return]:
//                  1: HDMI
//                  0: DVI
//**************************************************************************
BYTE mapi_tmds_Input_Is_HDMI(BYTE ucInputPort)
{
	return mdrv_tmds_Input_Is_HDMI(ucInputPort);
}

//**************************************************************************
//  [Function Name]:
//                  mapi_tmds_GetPacketContent()
//  [Description]:
//                  use to obtain HDMI packet contents
//  [Arguments]:
//                  enInputPort : combo port
//                  ucPacketType: indicate the type of the packet which is going to be extracted
//                  ucPacketLength: the total length to be extracted (byte unit)
//                  pPacketData: pointer, address which store return content
//  [Return]:
//                  none
//
//**************************************************************************
void mapi_tmds_GetPacketContent(BYTE ucInputPort, EN_PACKET_DEFINE_TYPE ucPacketType, BYTE ucPacketLength, BYTE *pPacketData)
{
    mdrv_tmds_GetPacketContent(ucInputPort, ucPacketType, ucPacketLength, pPacketData);
}
#endif

#if ENABLE_CABLE_DET
//**************************************************************************
//  [Function Name]:
//                  mapi_tmds_DetectCablePlug()
//  [Description]:
//                  detect if HDMI/DVI cable is attached.
//  [Arguments]:
//                  ucInputPort : combo port
//  [Return]:
//                  none
//
//**************************************************************************
BYTE mapi_tmds_DetectCablePlug(BYTE ucInputPort)
{
    return mdrv_tmds_DetectCablePlug(ucInputPort);
}
#endif
#endif // _MAPI_TMDS_C_

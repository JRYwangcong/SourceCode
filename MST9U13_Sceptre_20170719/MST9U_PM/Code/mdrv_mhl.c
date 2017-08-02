///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    mdrv_mhl.c
/// @author MStar Semiconductor Inc.
/// @brief  MHL driver Function
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MDRV_MHL_C_
#define _MDRV_MHL_C_

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "Global.h"
#include <string.h>
#include "mapi_mhl.h"
#include "mhal_mhl.h"
#include "mdrv_mhl.h"

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define MHL_DRV_DEBUG_MESSAGE           1
#define MHL_DEBUG_RX_RECEIVE_DATA       0

#if(MHL_DEBUG_MESSAGE & MHL_DRV_DEBUG_MESSAGE)
#define MHL_DRV_DPUTSTR(str)            printMsg(str)
#define MHL_DRV_DPRINTF(str, x)         printData(str, x)
#else
#define MHL_DRV_DPUTSTR(str)
#define MHL_DRV_DPRINTF(str, x)
#endif

#if(MHL_DEBUG_RX_RECEIVE_DATA)
#define MHL_DRV_RXDEBUG(str, x)         MHL_DRV_DPRINTF(str, x)
#else
#define MHL_DRV_RXDEBUG(str, x)
#endif

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
BYTE ucMHLCbusConnect = 0;

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_SetPowerStatus()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_mhl_SetPowerStatus(BYTE ucPowerStatus)
{
    mhal_mhl_SetPowerStatus(ucPowerStatus);

    CLR_MHL_CBUS_CONNECT_PORT_ALL();
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_SetPowerStatus()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_mhl_TimerHandler(void)
{
    mhal_mhl_TimerHandler();
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_CbusWakeupDetect()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mdrv_mhl_CbusWakeupDetect(BYTE ucPortSelect)
{
    Bool bCbusConnect = FALSE;

    mhal_mhl_PMStatePolling(ucPortSelect);

    bCbusConnect = mhal_mhl_CbusConnectStatus(ucPortSelect);
    
    if(GET_MHL_CBUS_CONNECT_PORT(ucPortSelect) != bCbusConnect)
    {
        if(bCbusConnect)
        {
            // Enable Rterm control SW mode
            mhal_mhl_ComboRtermControl(ucPortSelect, RX_MHL_RTERM);
            // Disable Rterm control HW mode
            mhal_mhl_RtermControlHWMode(ucPortSelect, FALSE);

            SET_MHL_CBUS_CONNECT_PORT(ucPortSelect);
        }
        else if(GET_MHL_CBUS_CONNECT_PORT(ucPortSelect))
        {
            // Disable Rterm control SW mode
            mhal_mhl_ComboRtermControl(ucPortSelect, RX_RTERM_OFF);
            // Enable Rterm control HW mode
            mhal_mhl_RtermControlHWMode(ucPortSelect, TRUE);
            
            CLR_MHL_CBUS_CONNECT_PORT(ucPortSelect);
        }
    }

    return bCbusConnect;
}

#endif // _MDRV_MHL_C_


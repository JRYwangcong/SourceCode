///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    mapi_mhl.c
/// @author MStar Semiconductor Inc.
/// @brief  MHL driver Function
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MAPI_MHL_C_
#define _MAPI_MHL_C_

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "Global.h"
#include "Mapi_combo.h"
#include "mapi_mhl.h"
#include "mdrv_mhl.h"
#include "mhal_mhl.h"

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define MHL_API_DEBUG_MESSAGE           0

#if(MHL_DEBUG_MESSAGE & MHL_API_DEBUG_MESSAGE)
#define MHL_API_DPUTSTR(str)            printMsg(str)
#define MHL_API_DPRINTF(str, x)         printData(str, x)
#else
#define MHL_API_DPUTSTR(str)
#define MHL_API_DPRINTF(str, x)
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

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
//**************************************************************************
//  [Function Name]:
//                  _mapi_mhl_InputToPortMapping()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
#if(MHL_GET_CABLE_DETECT_SUPPORT)
BYTE _mapi_mhl_InputToPortMapping(BYTE ucInputPort)
{
    if(ucInputPort == MHL_DEFINE_COMBO_IP_PORT0)
    {
        ucInputPort = MHL_INPUT_SELECT_PORT0;
    }
    else if(ucInputPort == MHL_DEFINE_COMBO_IP_PORT1)
    {
        ucInputPort = MHL_INPUT_SELECT_PORT1;
    }
    else if(ucInputPort == MHL_DEFINE_COMBO_IP_PORT2)
    {
        ucInputPort = MHL_INPUT_SELECT_PORT2;
    }
    else if(ucInputPort == MHL_DEFINE_COMBO_IP_PORT3)
    {
        ucInputPort = MHL_INPUT_SELECT_PORT3;
    }
    else if(ucInputPort == MHL_DEFINE_COMBO_IP_PORT4)
    {
        ucInputPort = MHL_INPUT_SELECT_PORT4;
    }
    else if(ucInputPort == MHL_DEFINE_COMBO_IP_PORT5)
    {
        ucInputPort = MHL_INPUT_SELECT_PORT5;
    }
    else // VGA and DP
    {
        ucInputPort = MHL_INPUT_SELECT_NONE;
    }

    return ucInputPort;
}
#endif

#ifdef _WORK_ON_PM_
//**************************************************************************
//  [Function Name]:
//                  _mapi_mhl_PortToInputMapping()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE _mapi_mhl_PortToInputMapping(BYTE ucPortSelect)
{
    if(ucPortSelect == MHL_INPUT_SELECT_PORT0)
    {
        ucPortSelect = MHL_DEFINE_COMBO_IP_PORT0;
    }
    else if(ucPortSelect == MHL_INPUT_SELECT_PORT1)
    {
        ucPortSelect = MHL_DEFINE_COMBO_IP_PORT1;
    }
    else if(ucPortSelect == MHL_INPUT_SELECT_PORT2)
    {
        ucPortSelect = MHL_DEFINE_COMBO_IP_PORT2;
    }
    else if(ucPortSelect == MHL_INPUT_SELECT_PORT3)
    {
        ucPortSelect = MHL_DEFINE_COMBO_IP_PORT3;
    }
    else if(ucPortSelect == MHL_INPUT_SELECT_PORT4)
    {
        ucPortSelect = MHL_DEFINE_COMBO_IP_PORT4;
    }
    else if(ucPortSelect == MHL_INPUT_SELECT_PORT5)
    {
        ucPortSelect = MHL_DEFINE_COMBO_IP_PORT5;
    }

    return ucPortSelect;
}

//**************************************************************************
//  [Function Name]:
//                  _mapi_mhl_CbusRCPEventProc()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE _mapi_mhl_CbusRCPEventProc(BYTE ucKeyCode)
{
    BYTE ucStatus = MSC_RCP_NO_ERROR;
    BYTE ucInputPort = _mapi_mhl_PortToInputMapping(GET_MHL_CBUS_MSC_MSG_PROC_PORT());

    switch(ucKeyCode)
    {
        case RCP_KEYID_UP:

            break;

        case RCP_KEYID_DOWN:

            break;

        case RCP_KEYID_ROOT_MENU:

            break;

        case RCP_KEYID_EXIT:

            break;

        default:
            ucStatus = MSC_RCP_INEFFECTIVE_KEY_CODE;
            break;
    };

    if(ucInputPort)
    {

    }

    MHL_API_DPRINTF("** MHL receive RCP key code %x", ucKeyCode);

    return ucStatus;
}

//**************************************************************************
//  [Function Name]:
//                  _mapi_mhl_CbusRAPEventProc()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE _mapi_mhl_CbusRAPEventProc(BYTE ucKeyCode)
{
    BYTE ucStatus  = MSC_RAP_NO_ERROR;
    BYTE ucInputPort = _mapi_mhl_PortToInputMapping(GET_MHL_CBUS_MSC_MSG_PROC_PORT());

    switch(ucKeyCode)
    {
        case MSC_RAP_POLL:

            break;

        case MSC_RAP_CONTENT_ON:

            break;

        case MSC_RAP_CONTENT_OFF:

            break;

        default:
            ucStatus = MSC_RAP_UNRECOGNIZED_ACTION_CODE;
            break;
    };

    if(ucInputPort)
    {

    }

    MHL_API_DPRINTF("** MHL receive RAP key code %x", ucKeyCode);

    return ucStatus;
}

//**************************************************************************
//  [Function Name]:
//                  _mapi_mhl_CbusRCPEventProc()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE _mapi_mhl_CbusUCPEventProc(BYTE ucKeyCode)
{
    BYTE ucStatus  = MSC_RAP_NO_ERROR;
    BYTE ucInputPort = _mapi_mhl_PortToInputMapping(GET_MHL_CBUS_MSC_MSG_PROC_PORT());

    switch(ucKeyCode)
    {
        default:
            ucStatus = MSC_RAP_UNRECOGNIZED_ACTION_CODE;
            break;
    };

    if(ucInputPort)
    {

    }

    MHL_API_DPRINTF("** MHL receive UCP key code %x", ucKeyCode);

    return ucStatus;
}

//**************************************************************************
//  [Function Name]:
//                  _mapi_mhl_RxHandler()
//  [Description]
//                  MHL handler
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mapi_mhl_CbusRxHandler(void)
{
    mdrv_mhl_CbusRxHandler();

    mdrv_mhl_CbusMSGCommandProc();
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

//**************************************************************************
//  [Function Name]:
//                  mapi_mhl_SetConfiguration()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_mhl_SetConfiguration(BYTE ucMHLEnableIndex, BYTE ucDisplayEnableIndex)
{
    mdrv_mhl_SetConfiguration(ucMHLEnableIndex, ucDisplayEnableIndex);
}

//**************************************************************************
//  [Function Name]:
//                  mapi_mhl_SetSystemControlFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_mhl_SetSystemControlFlag(EN_MHL_SYSTEM_CONTROL ucControlType)
{
    switch(ucControlType)
    {
        case MHL_SYSTEM_FORCE_DISPLAY_ENABLE:
            SET_MHL_SYSTEM_FORCE_DISPLAY();
            break;

        case MHL_SYSTEM_FORCE_DISPLAY_DISABLE:
            CLR_MHL_SYSTEM_FORCE_DISPLAY();
            break;

        case MHL_SYSTEM_FORCE_SEND_MSG_ENABLE:
            SET_MHL_SYSTEM_FORCE_SEND_MSG();
            break;

        case MHL_SYSTEM_FORCE_SEND_MSG_DISABLE:
            CLR_MHL_SYSTEM_FORCE_SEND_MSG();
            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  mapi_mhl_Initial()
//  [Description]
//                  MHL init
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_mhl_Initial(BYTE **pComboEDID)
{
    MHL_API_DPUTSTR("** MHL initial setting");

    mdrv_mhl_Initial(pComboEDID);

    mdrv_mhl_SetMSGCallBackFunction(_mapi_mhl_CbusRCPEventProc, _mapi_mhl_CbusRAPEventProc, _mapi_mhl_CbusUCPEventProc);
}

//**************************************************************************
//  [Function Name]:
//                  mapi_mhl_CbusTimerInterrupt()
//  [Description]
//                  MHL Cbus timer control in isr
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_mhl_TimerInterrupt(void)
{
    if(GET_MHL_SYSTEM_TIMER_COUNT() == 0)
    {
        SET_MHL_SYSTEM_TIMER_COUNT(10);

        SET_MHL_SYSTEM_TIMER_FLAG();
    }
    else
    {
        DEC_MHL_SYSTEM_TIMER_COUNT();
    }
}

//**************************************************************************
//  [Function Name]:
//                  mapi_mhl_IsrHandler()
//  [Description]
//                  MHL isr handler
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_mhl_IsrHandler(InterruptNum eIntNum)
{
    UNUSED(eIntNum);

    if(mdrv_mhl_GetCbusInterruptFlag())
    {
        mdrv_mhl_IsrHandler();
    }
}

//**************************************************************************
//  [Function Name]:
//                  mapi_mhl_Handler()
//  [Description]
//                  MHL handler
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_mhl_Handler(void)
{
    mdrv_mhl_TimerHandler();

    mdrv_mhl_CheckStatusPolling();

    _mapi_mhl_CbusRxHandler();

    mdrv_mhl_CbusTxHandler();
}

//**************************************************************************
//  [Function Name]:
//                  mapi_mhl_SendRCPCommand
//  [Description]
//                  MHL Cbus MSC Send RCP Command
//  [Arguments]:
//
//  [Return]:
//                  TRUE: success
//                  FALSE: fail
//**************************************************************************
Bool mapi_mhl_SendRCPCommand(BYTE ucInputPort, BYTE ucKeyCode)
{
    BYTE ucPortSelect = _mapi_mhl_InputToPortMapping(ucInputPort);

    return mdrv_mhl_CbusSendRCPCommand(ucPortSelect, ucKeyCode);
}

//**************************************************************************
//  [Function Name]:
//                  mapi_mhl_SendRAPCommand
//  [Description]
//                  MHL Cbus MSC Send RAP Command
//  [Arguments]:
//
//  [Return]:
//                  TRUE: success
//                  FALSE: fail
//**************************************************************************
Bool mapi_mhl_SendRAPCommand(BYTE ucInputPort, BYTE ucKeyCode)
{
    BYTE ucPortSelect = _mapi_mhl_InputToPortMapping(ucInputPort);

    return mdrv_mhl_CbusSendRAPCommand(ucPortSelect, ucKeyCode);
}

//**************************************************************************
//  [Function Name]:
//                  mapi_mhl_SendUCPCommand
//  [Description]
//                  MHL Cbus MSC Send UCP Command
//  [Arguments]:
//
//  [Return]:
//                  TRUE: success
//                  FALSE: fail
//**************************************************************************
Bool mapi_mhl_SendUCPCommand(BYTE ucInputPort, BYTE ucKeyCode)
{
    BYTE ucPortSelect = _mapi_mhl_InputToPortMapping(ucInputPort);

    return mdrv_mhl_CbusSendUCPCommand(ucPortSelect, ucKeyCode);
}

//**************************************************************************
//  [Function Name]:
//                  mapi_mhl_SourceChange
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_mhl_SourceChange(void)
{

}

//**************************************************************************
//  [Function Name]:
//                  mapi_mhl_PowerControl
//  [Description]
//                  MHL power control
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_mhl_PowerControl(BYTE ucState)
{
    mdrv_mhl_PowerControl(ucState);
}
#endif

#if(MHL_GET_CABLE_DETECT_SUPPORT)
//**************************************************************************
//  [Function Name]:
//                  mapi_mhl_GetCableDetectPort
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mapi_mhl_GetCableDetectPort(BYTE ucInputPort)
{
    BYTE ucPortSelect = _mapi_mhl_InputToPortMapping(ucInputPort);

    return mdrv_mhl_GetCableDetectPort(ucPortSelect);
}

#endif

#endif // _MAPI_MHL_C_


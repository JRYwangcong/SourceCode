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
BYTE ucMHLMSCMSGKeyEvent[MHL_INPUT_SELECT_MASK] = {0};

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Other
//-------------------------------------------------------------------------------------------------
extern void mhal_tmds_EnableAudioMute(Bool EnableMute);
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
    Bool bReleaseFlag = FALSE;
    BYTE ucStatus = MHL_RCP_NO_ERROR;
    BYTE ucMSGKeyEvent = MHL_MSC_MSG_KEY_EVENT_NONE;
    BYTE ucInputPort = GET_MHL_CBUS_MSC_MSG_PROC_PORT();

    if((ucKeyCode &BIT7) == BIT7)
    {
        bReleaseFlag = TRUE;

        ucKeyCode = ucKeyCode &BITMASK(6:0);
    }
    
    switch(ucKeyCode)
    {
        case MHL_RCP_KEY_UP:
            ucMSGKeyEvent = MHL_MSC_MSG_KEY_EVENT_UP;
            break;

        case MHL_RCP_KEY_DOWN:
            ucMSGKeyEvent = MHL_MSC_MSG_KEY_EVENT_DOWN;
            break;

        case MHL_RCP_KEY_ROOT_MENU:
            ucMSGKeyEvent = MHL_MSC_MSG_KEY_EVENT_MENU;
            break;

        case MHL_RCP_KEY_EXIT:
            ucMSGKeyEvent = MHL_MSC_MSG_KEY_EVENT_EXIT;
            break;

        default:
            ucStatus = MHL_RCP_INEFFECTIVE_KEY_CODE;
            break;
    };

    if(bReleaseFlag || (!mdrv_mhl_GetDisplayFlagPort(ucInputPort))) // Off line port and release key ignore RCP key event
    {
        ucMSGKeyEvent = MHL_MSC_MSG_KEY_EVENT_NONE;
    }

    SET_MHL_MSG_KEY_EVENT_PORT(ucInputPort, ucMSGKeyEvent);

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
    BYTE ucStatus  = MHL_RAP_NO_ERROR;
    BYTE ucMSGKeyEvent = MHL_MSC_MSG_KEY_EVENT_NONE;
    BYTE ucInputPort = GET_MHL_CBUS_MSC_MSG_PROC_PORT();

    switch(ucKeyCode)
    {
        case MHL_RAP_KEY_POLL:

            break;

        case MHL_RAP_KEY_CONTENT_ON:
            if(mdrv_mhl_GetDisplayFlagPort(ucInputPort)) // On line port unmute video
            {
                WAIT_V_OUTPUT_BLANKING();
                mStar_BlackWhiteScreenCtrl(BW_SCREEN_OFF);
                hw_SetBlacklit();
                mhal_tmds_EnableAudioMute(FALSE);
                ucMSGKeyEvent = MHL_MSC_MSG_KEY_EVENT_UNMUTE;
            }
            else // Off line port switch source
            {
                ucMSGKeyEvent = MHL_MSC_MSG_KEY_EVENT_SWITCH_PORT;
            }
            
            break;

        case MHL_RAP_KEY_CONTENT_OFF:
            if(mdrv_mhl_GetDisplayFlagPort(ucInputPort)) // On line port mute video
            {
                WAIT_V_OUTPUT_BLANKING();
                mStar_BlackWhiteScreenCtrl(BW_SCREEN_BLACK);
                hw_ClrBlacklit();
                mhal_tmds_EnableAudioMute(TRUE);
                ucMSGKeyEvent = MHL_MSC_MSG_KEY_EVENT_MUTE;
            }

            break;

        case MHL_RAP_KEY_CBUS_MODE_DOWN:
        case MHL_RAP_KEY_CBUS_MODE_UP:
            mdrv_mhl_ReceiveECbusModeChange(ucInputPort, ucKeyCode);
            break;

        default:
            ucStatus = MHL_RAP_UNRECOGNIZED_ACTION_CODE;
            break;
    };

    SET_MHL_MSG_KEY_EVENT_PORT(ucInputPort, ucMSGKeyEvent);

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
    BYTE ucStatus  = MHL_UCP_NO_ERROR;
    BYTE ucMSGKeyEvent = MHL_MSC_MSG_KEY_EVENT_NONE;
    BYTE ucInputPort = GET_MHL_CBUS_MSC_MSG_PROC_PORT();

    switch(ucKeyCode)
    {
        default:
            ucStatus = MHL_UCP_INEFFECTIVE_KEY_CODE;
            break;
    };

    SET_MHL_MSG_KEY_EVENT_PORT(ucInputPort, ucMSGKeyEvent);

    MHL_API_DPRINTF("** MHL receive UCP key code %x", ucKeyCode);

    return ucStatus;
}

//**************************************************************************
//  [Function Name]:
//                  _mapi_mhl_CbusATTEventProc()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE _mapi_mhl_CbusATTEventProc(BYTE ucKeyCode)
{
    BYTE ucStatus  = MHL_ATT_NO_ERROR;
    BYTE ucMSGKeyEvent = MHL_MSC_MSG_KEY_EVENT_NONE;
    BYTE ucInputPort = GET_MHL_CBUS_MSC_MSG_PROC_PORT();

    switch(ucKeyCode)
    {
        default:
            ucStatus = MHL_ATT_UNRECOGNIZED_ACTION_CODE;
            break;
    };

    SET_MHL_MSG_KEY_EVENT_PORT(ucInputPort, ucMSGKeyEvent);

    MHL_API_DPRINTF("** MHL receive ATT key code %x", ucKeyCode);

    return ucStatus;
}

//**************************************************************************
//  [Function Name]:
//                  _mapi_mhl_CbusRBPEventProc()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE _mapi_mhl_CbusRBPEventProc(BYTE ucKeyCode)
{
    BYTE ucStatus = MHL_RBP_NO_ERROR;
    BYTE ucMSGKeyEvent = MHL_MSC_MSG_KEY_EVENT_NONE;
    BYTE ucInputPort = GET_MHL_CBUS_MSC_MSG_PROC_PORT();

    switch(ucKeyCode)
    {
        case MHL_RBP_KEY_SCREEN_PAGE_UP:
            ucMSGKeyEvent = MHL_MSC_MSG_KEY_EVENT_UP;
            break;

        case MHL_RBP_KEY_SCREEN_PAGE_DOWN:
            ucMSGKeyEvent = MHL_MSC_MSG_KEY_EVENT_DOWN;
            break;

        default:
            ucStatus = MHL_RBP_INEFFECTIVE_BUTTON_CODE;
            break;
    };

    if(!mdrv_mhl_GetDisplayFlagPort(ucInputPort)) // Off line port ignore RBP key event
    {
        ucMSGKeyEvent = MHL_MSC_MSG_KEY_EVENT_NONE;
    }

    SET_MHL_MSG_KEY_EVENT_PORT(ucInputPort, ucMSGKeyEvent);

    MHL_API_DPRINTF("** MHL receive RBP key code %x", ucKeyCode);

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
//                  mapi_mhl_SetPowerStatus()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_mhl_SetPowerStatus(BYTE ucPowerStatus)
{
    if((ucPowerStatus == MHL_POWER_STATUS_DC_ON) || (ucPowerStatus == MHL_POWER_STATUS_PS_ON))
    {
        SET_MHL_SYSTEM_POWER_IGNORE();
    }

    CLR_MHL_SYSTEM_STATUS_UPDATE();
    SET_MHL_SYSTEM_INIT_STATUS();
}

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
//                  mapi_mhl_LoadEDIDPort()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_mhl_LoadEDIDPort(BYTE ucPortSelect, BYTE **pComboEDID)
{
    mdrv_mhl_LoadEDIDPort(ucPortSelect, pComboEDID[ucPortSelect]);
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

    mdrv_mhl_Initial(pComboEDID, ucMHLMSCMSGKeyEvent);

    mdrv_mhl_SetMSGCallBackFunction(_mapi_mhl_CbusRCPEventProc, _mapi_mhl_CbusRAPEventProc, _mapi_mhl_CbusUCPEventProc);
    mdrv_mhl_SetMSGCallBackFunctionExtend(_mapi_mhl_CbusATTEventProc, _mapi_mhl_CbusRBPEventProc);
    
    CLR_MHL_SYSTEM_POWER_IGNORE();
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

#if(MHL_HW_RTERM_CONTROL_SUPPORT)
    mdrv_mhl_CbusHWControlRterm(FALSE);
#endif

    mdrv_mhl_CheckStatusPolling();

#if(MHL_HW_RTERM_CONTROL_SUPPORT)
    mdrv_mhl_CbusHWControlRterm(TRUE);
#endif

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
    switch(ucState)
    {
        case MHL_POWER_STATUS_DC_OFF:
            ucState = MHL_POWER_DOWN;
            break;

        case MHL_POWER_STATUS_POWER_SAVING:
            ucState = MHL_POWER_STANDBY;
            break;
            
        default:
            break;
    };
    
    mdrv_mhl_PowerControl(ucState);
}

#if(MHL_GET_CABLE_DETECT_SUPPORT)
//**************************************************************************
//  [Function Name]:
//                  mapi_mhl_GetCableDetectPortByInit
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mapi_mhl_GetCableDetectPortByInit(BYTE ucInputPort)
{
    Bool bFlag;
    
    bFlag = mdrv_mhl_GetCableDetectPortByInit(ucInputPort);
    return bFlag;
}
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

//**************************************************************************
//  [Function Name]:
//                  mapi_mhl_GetCbusConnectPort
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mapi_mhl_GetCbusConnectPort(BYTE ucInputPort)
{
    BYTE ucPortSelect = _mapi_mhl_InputToPortMapping(ucInputPort);

    return mdrv_mhl_GetCbusConnectPort(ucPortSelect);
}

//**************************************************************************
//  [Function Name]:
//                  mapi_mhl_GetMSGKeyEvent
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
BYTE mapi_mhl_GetMSGKeyEvent(BYTE ucInputPort)
{
    BYTE ucPortSelect = _mapi_mhl_InputToPortMapping(ucInputPort);
    BYTE ucKeyEvent = MHL_MSC_MSG_KEY_EVENT_NONE;
    
    ucKeyEvent = GET_MHL_MSG_KEY_EVENT_PORT(ucPortSelect);

    if(ucKeyEvent != MHL_MSC_MSG_KEY_EVENT_NONE)
    {
        SET_MHL_MSG_KEY_EVENT_PORT(ucPortSelect, MHL_MSC_MSG_KEY_EVENT_NONE);
    }
    
    return ucKeyEvent;
}

//**************************************************************************
//  [Function Name]:
//                  mapi_mhl_SendEMSCDataString()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE mapi_mhl_SendEMSCDataString(BYTE ucPortSelect, BYTE ucLength, BYTE *ucSendData)
{
    return mdrv_mhl_SendEMSCDataString(ucPortSelect, ucLength, ucSendData);
}
//**************************************************************************
//  [Function Name]:
//                  mapi_mhl_CablePlugProcByInit
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_mhl_CablePlugProcByInit(BYTE ucInputPort)
{
    mdrv_mhl_CablePlugProcByInit(ucInputPort);
}

#endif // _MAPI_MHL_C_


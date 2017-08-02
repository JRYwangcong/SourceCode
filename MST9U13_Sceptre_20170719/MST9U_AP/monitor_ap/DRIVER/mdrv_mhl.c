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
#include "mdrv_mhl.h"
#include "mhal_mhl.h"
#include "msEread.h"
#include "mhal_mhl.h"


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define MHL_DRV_DEBUG_MESSAGE           1
#define MHL_DEBUG_RX_RECEIVE_DATA       0
#define MHL_DEBUG_EMSC_BUFFER_SIZE      1
#define MHL_DEBUG_CHECK_CRC_VALUE       0

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

#if(MHL_DEBUG_EMSC_BUFFER_SIZE)
#define MHL_DRV_EMSCBUFFER(str, x)      MHL_DRV_DPRINTF(str, x)
#else
#define MHL_DRV_EMSCBUFFER(str, x)
#endif

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
BYTE ucMHLTimerCount = 0;
BYTE ucMHLSystemIndex = 0;
BYTE ucMHLFunctionEnableIndex = 0;
BYTE ucMHLDisplayEnableIndex = 0;
BYTE ucMHLMSCMSGProcPort = 0;

#if(MHL_CBUS_SELF_TEST_SUPPORT)
BYTE ucMHLCbusSelfTestState = 0;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT0)
MHLInfo_S stMHLBlock0;
#endif
#if(MHL_FUNCTION_SUPPORT_PORT1)
MHLInfo_S stMHLBlock1;
#endif
#if(MHL_FUNCTION_SUPPORT_PORT2)
MHLInfo_S stMHLBlock2;
#endif
#if(MHL_FUNCTION_SUPPORT_PORT3)
MHLInfo_S stMHLBlock3;
#endif
#if(MHL_FUNCTION_SUPPORT_PORT4)
MHLInfo_S stMHLBlock4;
#endif
#if(MHL_FUNCTION_SUPPORT_PORT5)
MHLInfo_S stMHLBlock5;
#endif

MHL_CALL_BACK_FUNC pmdrv_mhl_CbusRCPEventProc = NULL;
MHL_CALL_BACK_FUNC pmdrv_mhl_CbusRAPEventProc = NULL;
MHL_CALL_BACK_FUNC pmdrv_mhl_CbusUCPEventProc = NULL;
MHL_CALL_BACK_FUNC pmdrv_mhl_CbusATTEventProc = NULL;
MHL_CALL_BACK_FUNC pmdrv_mhl_CbusRBPEventProc = NULL;

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
BYTE tMHL_DEVCAP_TABLE_PORT0[] =
{
    MHL_DEV_STATE_0x00,    //DEV_STATE
    MHL_VERSION_0x01_PORT0,    //MHL_VERSION
    MHL_DEV_CAT_0x02,    //DEV_CAT, [4]:POW, [3:0]:DEV_TYPE
    MHL_ADOPTER_ID_H_0x03,    // ADOPTER_ID_H, Mstar adopter ID 715(0x2CB)
    MHL_ADOPTER_ID_L_0x04,    // ADOPTER_ID_L
    MHL_VID_LINK_MODE_0x05,    // VID_LINK_MODE, [5]:SUPP_VGA, [4]:ISLANDS, [3]:PPIXEL, [2]:YUV422, [1]:YUV444, [0]:RGB444
    MHL_AUD_LINK_MODE_0x06,    // AUD_LINK_MODE, [1]:AUD_8CH, [0]:AUD_2CH
    MHL_VIDEO_TYPE_0x07,    // VIDEO_TYPE, [7]: SUPP_VT, [3]:GAME, [2]:CINEMA, [1]: PHOTO, [0]:GRAPHICS
    MHL_LOG_DEV_MAP_0x08,    // LOG_DEV_MAP, [0]:LD_DISPLAY
    MHL_BANDWIDTH_0x09,    // BANDWIDTH, 15*5MHz = 75MHz
    MHL_FEATURE_FLAG_0x0A,    // FEATURE_FLAG, [2]:Scratchpad, [1]:RAP, [0]:RCP
    MHL_DEVICE_ID_H_0x0B,    // DEVICE_ID_H, ? CHIP_FAMILY_TYPE
    MHL_DEVICE_ID_L_0x0C,    // DEVICE_ID_L, ? CHIP_FAMILY_TYPE
    MHL_SCRATCHPAD_SIZE_0x0D,    // SCRATCHPAD_SIZE, 16 bytes
    MHL_INT_STAT_0x0E,    // INT_STAT_SIZE, [7:4]:Status 4 bytes, [3:0]:Interrupt 4 bytes
    MHL_RESERVED_0x0F,    // Reserved
    MHL_ECBUS_SPEEDS_0x80,
    MHL_TMDS_SPEEDS_0x81,
    MHL_ECBUS_DEV_ROLES_0x82,
    MHL_LOG_DEV_MAPX_0x83,
};

BYTE tMHL_DEVCAP_TABLE_PORT1[] =
{
    MHL_DEV_STATE_0x00,    //DEV_STATE
    MHL_VERSION_0x01_PORT1,    //MHL_VERSION
    MHL_DEV_CAT_0x02,    //DEV_CAT, [4]:POW, [3:0]:DEV_TYPE
    MHL_ADOPTER_ID_H_0x03,    // ADOPTER_ID_H, Mstar adopter ID 715(0x2CB)
    MHL_ADOPTER_ID_L_0x04,    // ADOPTER_ID_L
    MHL_VID_LINK_MODE_0x05,    // VID_LINK_MODE, [5]:SUPP_VGA, [4]:ISLANDS, [3]:PPIXEL, [2]:YUV422, [1]:YUV444, [0]:RGB444
    MHL_AUD_LINK_MODE_0x06,    // AUD_LINK_MODE, [1]:AUD_8CH, [0]:AUD_2CH
    MHL_VIDEO_TYPE_0x07,    // VIDEO_TYPE, [7]: SUPP_VT, [3]:GAME, [2]:CINEMA, [1]: PHOTO, [0]:GRAPHICS
    MHL_LOG_DEV_MAP_0x08,    // LOG_DEV_MAP, [0]:LD_DISPLAY
    MHL_BANDWIDTH_0x09,    // BANDWIDTH, 15*5MHz = 75MHz
    MHL_FEATURE_FLAG_0x0A,    // FEATURE_FLAG, [2]:Scratchpad, [1]:RAP, [0]:RCP
    MHL_DEVICE_ID_H_0x0B,    // DEVICE_ID_H, ? CHIP_FAMILY_TYPE
    MHL_DEVICE_ID_L_0x0C,    // DEVICE_ID_L, ? CHIP_FAMILY_TYPE
    MHL_SCRATCHPAD_SIZE_0x0D,    // SCRATCHPAD_SIZE, 16 bytes
    MHL_INT_STAT_0x0E,    // INT_STAT_SIZE, [7:4]:Status 4 bytes, [3:0]:Interrupt 4 bytes
    MHL_RESERVED_0x0F,    // Reserved
    MHL_ECBUS_SPEEDS_0x80,
    MHL_TMDS_SPEEDS_0x81,
    MHL_ECBUS_DEV_ROLES_0x82,
    MHL_LOG_DEV_MAPX_0x83,
};

BYTE tMHL_DEVCAP_TABLE_PORT2[] =
{
    MHL_DEV_STATE_0x00,    //DEV_STATE
    MHL_VERSION_0x01_PORT2,    //MHL_VERSION
    MHL_DEV_CAT_0x02,    //DEV_CAT, [4]:POW, [3:0]:DEV_TYPE
    MHL_ADOPTER_ID_H_0x03,    // ADOPTER_ID_H, Mstar adopter ID 715(0x2CB)
    MHL_ADOPTER_ID_L_0x04,    // ADOPTER_ID_L
    MHL_VID_LINK_MODE_0x05,    // VID_LINK_MODE, [5]:SUPP_VGA, [4]:ISLANDS, [3]:PPIXEL, [2]:YUV422, [1]:YUV444, [0]:RGB444
    MHL_AUD_LINK_MODE_0x06,    // AUD_LINK_MODE, [1]:AUD_8CH, [0]:AUD_2CH
    MHL_VIDEO_TYPE_0x07,    // VIDEO_TYPE, [7]: SUPP_VT, [3]:GAME, [2]:CINEMA, [1]: PHOTO, [0]:GRAPHICS
    MHL_LOG_DEV_MAP_0x08,    // LOG_DEV_MAP, [0]:LD_DISPLAY
    MHL_BANDWIDTH_0x09,    // BANDWIDTH, 15*5MHz = 75MHz
    MHL_FEATURE_FLAG_0x0A,    // FEATURE_FLAG, [2]:Scratchpad, [1]:RAP, [0]:RCP
    MHL_DEVICE_ID_H_0x0B,    // DEVICE_ID_H, ? CHIP_FAMILY_TYPE
    MHL_DEVICE_ID_L_0x0C,    // DEVICE_ID_L, ? CHIP_FAMILY_TYPE
    MHL_SCRATCHPAD_SIZE_0x0D,    // SCRATCHPAD_SIZE, 16 bytes
    MHL_INT_STAT_0x0E,    // INT_STAT_SIZE, [7:4]:Status 4 bytes, [3:0]:Interrupt 4 bytes
    MHL_RESERVED_0x0F,    // Reserved
    MHL_ECBUS_SPEEDS_0x80,
    MHL_TMDS_SPEEDS_0x81,
    MHL_ECBUS_DEV_ROLES_0x82,
    MHL_LOG_DEV_MAPX_0x83,
};

BYTE tMHL_DEVCAP_TABLE_PORT3[] =
{
    MHL_DEV_STATE_0x00,    //DEV_STATE
    MHL_VERSION_0x01_PORT3,    //MHL_VERSION
    MHL_DEV_CAT_0x02,    //DEV_CAT, [4]:POW, [3:0]:DEV_TYPE
    MHL_ADOPTER_ID_H_0x03,    // ADOPTER_ID_H, Mstar adopter ID 715(0x2CB)
    MHL_ADOPTER_ID_L_0x04,    // ADOPTER_ID_L
    MHL_VID_LINK_MODE_0x05,    // VID_LINK_MODE, [5]:SUPP_VGA, [4]:ISLANDS, [3]:PPIXEL, [2]:YUV422, [1]:YUV444, [0]:RGB444
    MHL_AUD_LINK_MODE_0x06,    // AUD_LINK_MODE, [1]:AUD_8CH, [0]:AUD_2CH
    MHL_VIDEO_TYPE_0x07,    // VIDEO_TYPE, [7]: SUPP_VT, [3]:GAME, [2]:CINEMA, [1]: PHOTO, [0]:GRAPHICS
    MHL_LOG_DEV_MAP_0x08,    // LOG_DEV_MAP, [0]:LD_DISPLAY
    MHL_BANDWIDTH_0x09,    // BANDWIDTH, 15*5MHz = 75MHz
    MHL_FEATURE_FLAG_0x0A,    // FEATURE_FLAG, [2]:Scratchpad, [1]:RAP, [0]:RCP
    MHL_DEVICE_ID_H_0x0B,    // DEVICE_ID_H, ? CHIP_FAMILY_TYPE
    MHL_DEVICE_ID_L_0x0C,    // DEVICE_ID_L, ? CHIP_FAMILY_TYPE
    MHL_SCRATCHPAD_SIZE_0x0D,    // SCRATCHPAD_SIZE, 16 bytes
    MHL_INT_STAT_0x0E,    // INT_STAT_SIZE, [7:4]:Status 4 bytes, [3:0]:Interrupt 4 bytes
    MHL_RESERVED_0x0F,    // Reserved
    MHL_ECBUS_SPEEDS_0x80,
    MHL_TMDS_SPEEDS_0x81,
    MHL_ECBUS_DEV_ROLES_0x82,
    MHL_LOG_DEV_MAPX_0x83,
};

BYTE tMHL_DEVCAP_TABLE_PORT4[] =
{
    MHL_DEV_STATE_0x00,    //DEV_STATE
    MHL_VERSION_0x01_PORT4,    //MHL_VERSION
    MHL_DEV_CAT_0x02,    //DEV_CAT, [4]:POW, [3:0]:DEV_TYPE
    MHL_ADOPTER_ID_H_0x03,    // ADOPTER_ID_H, Mstar adopter ID 715(0x2CB)
    MHL_ADOPTER_ID_L_0x04,    // ADOPTER_ID_L
    MHL_VID_LINK_MODE_0x05,    // VID_LINK_MODE, [5]:SUPP_VGA, [4]:ISLANDS, [3]:PPIXEL, [2]:YUV422, [1]:YUV444, [0]:RGB444
    MHL_AUD_LINK_MODE_0x06,    // AUD_LINK_MODE, [1]:AUD_8CH, [0]:AUD_2CH
    MHL_VIDEO_TYPE_0x07,    // VIDEO_TYPE, [7]: SUPP_VT, [3]:GAME, [2]:CINEMA, [1]: PHOTO, [0]:GRAPHICS
    MHL_LOG_DEV_MAP_0x08,    // LOG_DEV_MAP, [0]:LD_DISPLAY
    MHL_BANDWIDTH_0x09,    // BANDWIDTH, 15*5MHz = 75MHz
    MHL_FEATURE_FLAG_0x0A,    // FEATURE_FLAG, [2]:Scratchpad, [1]:RAP, [0]:RCP
    MHL_DEVICE_ID_H_0x0B,    // DEVICE_ID_H, ? CHIP_FAMILY_TYPE
    MHL_DEVICE_ID_L_0x0C,    // DEVICE_ID_L, ? CHIP_FAMILY_TYPE
    MHL_SCRATCHPAD_SIZE_0x0D,    // SCRATCHPAD_SIZE, 16 bytes
    MHL_INT_STAT_0x0E,    // INT_STAT_SIZE, [7:4]:Status 4 bytes, [3:0]:Interrupt 4 bytes
    MHL_RESERVED_0x0F,    // Reserved
    MHL_ECBUS_SPEEDS_0x80,
    MHL_TMDS_SPEEDS_0x81,
    MHL_ECBUS_DEV_ROLES_0x82,
    MHL_LOG_DEV_MAPX_0x83,
};

BYTE tMHL_DEVCAP_TABLE_PORT5[] =
{
    MHL_DEV_STATE_0x00,    //DEV_STATE
    MHL_VERSION_0x01_PORT5,    //MHL_VERSION
    MHL_DEV_CAT_0x02,    //DEV_CAT, [4]:POW, [3:0]:DEV_TYPE
    MHL_ADOPTER_ID_H_0x03,    // ADOPTER_ID_H, Mstar adopter ID 715(0x2CB)
    MHL_ADOPTER_ID_L_0x04,    // ADOPTER_ID_L
    MHL_VID_LINK_MODE_0x05,    // VID_LINK_MODE, [5]:SUPP_VGA, [4]:ISLANDS, [3]:PPIXEL, [2]:YUV422, [1]:YUV444, [0]:RGB444
    MHL_AUD_LINK_MODE_0x06,    // AUD_LINK_MODE, [1]:AUD_8CH, [0]:AUD_2CH
    MHL_VIDEO_TYPE_0x07,    // VIDEO_TYPE, [7]: SUPP_VT, [3]:GAME, [2]:CINEMA, [1]: PHOTO, [0]:GRAPHICS
    MHL_LOG_DEV_MAP_0x08,    // LOG_DEV_MAP, [0]:LD_DISPLAY
    MHL_BANDWIDTH_0x09,    // BANDWIDTH, 15*5MHz = 75MHz
    MHL_FEATURE_FLAG_0x0A,    // FEATURE_FLAG, [2]:Scratchpad, [1]:RAP, [0]:RCP
    MHL_DEVICE_ID_H_0x0B,    // DEVICE_ID_H, ? CHIP_FAMILY_TYPE
    MHL_DEVICE_ID_L_0x0C,    // DEVICE_ID_L, ? CHIP_FAMILY_TYPE
    MHL_SCRATCHPAD_SIZE_0x0D,    // SCRATCHPAD_SIZE, 16 bytes
    MHL_INT_STAT_0x0E,    // INT_STAT_SIZE, [7:4]:Status 4 bytes, [3:0]:Interrupt 4 bytes
    MHL_RESERVED_0x0F,    // Reserved
    MHL_ECBUS_SPEEDS_0x80,
    MHL_TMDS_SPEEDS_0x81,
    MHL_ECBUS_DEV_ROLES_0x82,
    MHL_LOG_DEV_MAPX_0x83,
};

BYTE *tMHL_DEVCAP_TABLE_PORT[6] =
{
    tMHL_DEVCAP_TABLE_PORT0,
    tMHL_DEVCAP_TABLE_PORT1,
    tMHL_DEVCAP_TABLE_PORT2,
    tMHL_DEVCAP_TABLE_PORT3,
    tMHL_DEVCAP_TABLE_PORT4,
    tMHL_DEVCAP_TABLE_PORT5,
};

WORD tMHL_HEV_VIC_TABLE[] =
{
    MHL_HEV_VIC_INDEX0,
    MHL_HEV_VIC_INDEX1,
    MHL_HEV_VIC_INDEX2,
    MHL_HEV_VIC_INDEX3,
    MHL_HEV_VIC_INDEX4,
    MHL_HEV_VIC_INDEX5,
};

DWORD tMHL_AUDIO_TUNNEL_TABLE[] =
{
    MHL_AUDIO_TUNNEL_INDEX0,
    MHL_AUDIO_TUNNEL_INDEX1,
    MHL_AUDIO_TUNNEL_INDEX2,
    MHL_AUDIO_TUNNEL_INDEX3,
    MHL_AUDIO_TUNNEL_INDEX4,
    MHL_AUDIO_TUNNEL_INDEX5,
};

WORD tMHL_EMSC_SUPPORT_TABLE[] =
{
    MHL_EMSC_SUPPORT_BURST_ID0,
    MHL_EMSC_SUPPORT_BURST_ID1,
    MHL_EMSC_SUPPORT_BURST_ID2,
    MHL_EMSC_SUPPORT_BURST_ID3,
    MHL_EMSC_SUPPORT_BURST_ID4,
    MHL_EMSC_SUPPORT_BURST_ID5,
};

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CheckFunctionEnable()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool _mdrv_mhl_CheckFunctionEnable(BYTE ucPortSelect)
{
    Bool bFunctionSupport = FALSE;

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            bFunctionSupport = GET_MHL_FUNCTION_ENABLE_PORT0();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            bFunctionSupport = GET_MHL_FUNCTION_ENABLE_PORT1();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            bFunctionSupport = GET_MHL_FUNCTION_ENABLE_PORT2();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            bFunctionSupport = GET_MHL_FUNCTION_ENABLE_PORT3();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            bFunctionSupport = GET_MHL_FUNCTION_ENABLE_PORT4();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            bFunctionSupport = GET_MHL_FUNCTION_ENABLE_PORT5();
            break;
#endif

        default:

            break;
    };

    return bFunctionSupport;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_SetCableDetectDisableTime
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_SetCableDetectDisableTime(BYTE ucPortSelect, BYTE ucTime)
{
    static BYTE ucCableDetectEnableTime = 0;

    if(ucTime == 0)
    {
        if(!GET_MHL_SYSTEM_CD_SENSE_ENABLE())
        {
            if(ucCableDetectEnableTime > 0)
            {
                ucCableDetectEnableTime--;
            }
            else
            {
                SET_MHL_SYSTEM_CD_SENSE_ENABLE();

                for(ucPortSelect = MHL_INPUT_SELECT_PORT0; ucPortSelect < MHL_INPUT_SELECT_MASK; ucPortSelect++)
                {
                    if(_mdrv_mhl_CheckFunctionEnable(ucPortSelect))
                    {
                        mhal_mhl_CableDetectEnableControl(ucPortSelect, TRUE);
                    }
                }
            }
        }
    }
    else
    {
        ucCableDetectEnableTime = ucTime;
        CLR_MHL_SYSTEM_CD_SENSE_ENABLE();

        mhal_mhl_CableDetectEnableControl(ucPortSelect, FALSE);
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_SensationDConPatch()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool _mdrv_mhl_SensationDConPatch(BYTE ucPortSelect, Bool bSetFlag)
{
    Bool bTimerFlag = FALSE;
    static WORD ucTimerCount[MHL_INPUT_SELECT_MASK] = {0};

    if((ucTimerCount[ucPortSelect] &BIT15) == BIT15)
    {
        bTimerFlag = TRUE;
    }

    if(bSetFlag)
    {
        ucTimerCount[ucPortSelect] = BIT15 |MHL_Sensation_DCON_TIME;

        _mdrv_mhl_SetCableDetectDisableTime(ucPortSelect, 100);
    }
    else
    {
        if((ucTimerCount[ucPortSelect] &MASKBIT(14:0)) > 0)
        {
            ucTimerCount[ucPortSelect]--;
        }
        else
        {
            ucTimerCount[ucPortSelect] = 0;
        }
    }

    return bTimerFlag;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_BufferFlyReallyChargePatch()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool _mdrv_mhl_BufferFlyReallyChargePatch(BYTE ucPortSelect, Bool bSetFlag)
{
    Bool bTimerFlag = FALSE;
    static WORD ucTimerCount[MHL_INPUT_SELECT_MASK] = {0};

    if((ucTimerCount[ucPortSelect] &BIT15) == BIT15)
    {
        bTimerFlag = TRUE;
    }

    if(bSetFlag)
    {
        ucTimerCount[ucPortSelect] = BIT15 |MHL_BUFFER_FLY_CHARGE_TIME;

        mhal_mhl_CbusForceToStandby(ucPortSelect);
    }
    else
    {
        if((ucTimerCount[ucPortSelect] &MASKBIT(14:0)) > 0)
        {
            ucTimerCount[ucPortSelect]--;
        }
        else
        {
            ucTimerCount[ucPortSelect] = 0;
        }
    }

    return bTimerFlag;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_SetMSGWaitResponseTimer()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_SetMSGWaitResponseTimer(BYTE ucPortSelect, BYTE ucTimeTick)
{
    Bool bSetTimerFlag = FALSE;
    Bool bTimeoutFlag = FALSE;
    static BYTE ucTimerCount[MHL_INPUT_SELECT_MASK] = {0};

    if(ucTimeTick == 0)
    {
        if(ucTimerCount[ucPortSelect] > 0)
        {
            ucTimerCount[ucPortSelect]--;
        }
        else
        {
            bTimeoutFlag = TRUE;
        }
    }
    else
    {
        ucTimerCount[ucPortSelect] = ucTimeTick;
        bSetTimerFlag = TRUE;
    }

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            if(bSetTimerFlag)
            {
                SET_MHL_SEND_MSG_COMMAND_PORT0();
            }
            else if(bTimeoutFlag)
            {
                CLR_MHL_SEND_MSG_COMMAND_PORT0();
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            if(bSetTimerFlag)
            {
                SET_MHL_SEND_MSG_COMMAND_PORT1();
            }
            else if(bTimeoutFlag)
            {
                CLR_MHL_SEND_MSG_COMMAND_PORT1();
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            if(bSetTimerFlag)
            {
                SET_MHL_SEND_MSG_COMMAND_PORT2();
            }
            else if(bTimeoutFlag)
            {
                CLR_MHL_SEND_MSG_COMMAND_PORT2();
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            if(bSetTimerFlag)
            {
                SET_MHL_SEND_MSG_COMMAND_PORT3();
            }
            else if(bTimeoutFlag)
            {
                CLR_MHL_SEND_MSG_COMMAND_PORT3();
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            if(bSetTimerFlag)
            {
                SET_MHL_SEND_MSG_COMMAND_PORT4();
            }
            else if(bTimeoutFlag)
            {
                CLR_MHL_SEND_MSG_COMMAND_PORT4();
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            if(bSetTimerFlag)
            {
                SET_MHL_SEND_MSG_COMMAND_PORT5();
            }
            else if(bTimeoutFlag)
            {
                CLR_MHL_SEND_MSG_COMMAND_PORT5();
            }

            break;
#endif

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_SetBISTProcTimer()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_SetBISTProcTimer(BYTE ucPortSelect, WORD ucTimeTick)
{
    Bool bTimeoutFlag = FALSE;
    static WORD usTimerCount[MHL_INPUT_SELECT_MASK] = {0};

    if(ucTimeTick == 0)
    {
        if(usTimerCount[ucPortSelect] > 0)
        {
            usTimerCount[ucPortSelect]--;
        }
        else
        {
            bTimeoutFlag = TRUE;
        }
    }
    else
    {
        usTimerCount[ucPortSelect] = ucTimeTick;
    }

    if(bTimeoutFlag)
    {
        switch(ucPortSelect)
        {
#if(MHL_FUNCTION_SUPPORT_PORT0)
            case MHL_INPUT_SELECT_PORT0:
                if(GET_MHL_BIST_PROCESS_STATE_PORT0() == MHL_CBUS_BIST_ECBUS_SWITCH)
                {
                    SET_MHL_BIST_PROCESS_STATE_PORT0(MHL_CBUS_BIST_EVENT_FINISH);
                }
                
                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
            case MHL_INPUT_SELECT_PORT1:
                if(GET_MHL_BIST_PROCESS_STATE_PORT1() == MHL_CBUS_BIST_ECBUS_SWITCH)
                {
                    SET_MHL_BIST_PROCESS_STATE_PORT1(MHL_CBUS_BIST_EVENT_FINISH);
                }
                
                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
            case MHL_INPUT_SELECT_PORT2:
                if(GET_MHL_BIST_PROCESS_STATE_PORT2() == MHL_CBUS_BIST_ECBUS_SWITCH)
                {
                    SET_MHL_BIST_PROCESS_STATE_PORT2(MHL_CBUS_BIST_EVENT_FINISH);
                }
                
                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
            case MHL_INPUT_SELECT_PORT3:
                if(GET_MHL_BIST_PROCESS_STATE_PORT3() == MHL_CBUS_BIST_ECBUS_SWITCH)
                {
                    SET_MHL_BIST_PROCESS_STATE_PORT3(MHL_CBUS_BIST_EVENT_FINISH);
                }
                
                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
            case MHL_INPUT_SELECT_PORT4:
                if(GET_MHL_BIST_PROCESS_STATE_PORT4() == MHL_CBUS_BIST_ECBUS_SWITCH)
                {
                    SET_MHL_BIST_PROCESS_STATE_PORT4(MHL_CBUS_BIST_EVENT_FINISH);
                }
                
                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
            case MHL_INPUT_SELECT_PORT5:
                if(GET_MHL_BIST_PROCESS_STATE_PORT5() == MHL_CBUS_BIST_ECBUS_SWITCH)
                {
                    SET_MHL_BIST_PROCESS_STATE_PORT5(MHL_CBUS_BIST_EVENT_FINISH);
                }
                
                break;
#endif

            default:

                break;
        };
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusTxQueueInsertMSC()
//  [Description]
//                  MHL Cbus Request a MSC Cmd into Queue
//  [Arguments]:
//                  ucIndex: Current queue index
//                  pReq: Command buffer
//  [Return]:
//                  TRUE: success
//                  FALSE: fail
//**************************************************************************
Bool _mdrv_mhl_CbusTxQueueInsertMSC(BYTE ucPortSelect, CbusReq_S *pReq)
{
    Bool bQueueFlag = FALSE;
    BYTE uctemp = 0;
    BYTE ucQueueIndex = 0;

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            if(!GET_MHL_CBUS_TX_QUEUE_FREEZE0())
            {
                ucQueueIndex = GET_MHL_TX_QUEUE_INDEX_PORT0();

                // Search free queue
                for(uctemp = 0; uctemp <(MHL_CBUS_TOTAL_QUEUE_NUM -1); uctemp++)
                {
                    if(GET_MHL_QUEUE_STATE_PORT0((ucQueueIndex +uctemp +1) %(MHL_CBUS_TOTAL_QUEUE_NUM -1)) == CBUS_REQ_IDLE)
                    {
                        uctemp = (ucQueueIndex +uctemp +1) %(MHL_CBUS_TOTAL_QUEUE_NUM -1);

                        break;
                    }
                }

                if(uctemp != (MHL_CBUS_TOTAL_QUEUE_NUM -1)) // Have free queue
                {
                    if(GET_MHL_QUEUE_STATE_PORT0((uctemp +MHL_CBUS_TOTAL_QUEUE_NUM -2) %(MHL_CBUS_TOTAL_QUEUE_NUM -1)) == CBUS_REQ_IDLE)
                    {
                        uctemp = (uctemp +MHL_CBUS_TOTAL_QUEUE_NUM -2) %(MHL_CBUS_TOTAL_QUEUE_NUM -1);
                    }

                    memcpy(&(GET_MHL_CBUS_QUEUE_PORT0(uctemp)), pReq, sizeof(CbusReq_S));

                    bQueueFlag = TRUE;
                }
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            if(!GET_MHL_CBUS_TX_QUEUE_FREEZE1())
            {
                ucQueueIndex = GET_MHL_TX_QUEUE_INDEX_PORT1();

                // Search free queue
                for(uctemp = 0; uctemp <(MHL_CBUS_TOTAL_QUEUE_NUM -1); uctemp++)
                {
                    if(GET_MHL_QUEUE_STATE_PORT1((ucQueueIndex +uctemp +1) %(MHL_CBUS_TOTAL_QUEUE_NUM -1)) == CBUS_REQ_IDLE)
                    {
                        uctemp = (ucQueueIndex +uctemp +1) %(MHL_CBUS_TOTAL_QUEUE_NUM -1);

                        break;
                    }
                }

                if(uctemp != (MHL_CBUS_TOTAL_QUEUE_NUM -1)) // Have free queue
                {
                    if(GET_MHL_QUEUE_STATE_PORT1((uctemp +MHL_CBUS_TOTAL_QUEUE_NUM -2) %(MHL_CBUS_TOTAL_QUEUE_NUM -1)) == CBUS_REQ_IDLE)
                    {
                        uctemp = (uctemp +MHL_CBUS_TOTAL_QUEUE_NUM -2) %(MHL_CBUS_TOTAL_QUEUE_NUM -1);
                    }

                    memcpy(&(GET_MHL_CBUS_QUEUE_PORT1(uctemp)), pReq, sizeof(CbusReq_S));

                    bQueueFlag = TRUE;
                }
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            if(!GET_MHL_CBUS_TX_QUEUE_FREEZE2())
            {
                ucQueueIndex = GET_MHL_TX_QUEUE_INDEX_PORT2();

                // Search free queue
                for(uctemp = 0; uctemp <(MHL_CBUS_TOTAL_QUEUE_NUM -1); uctemp++)
                {
                    if(GET_MHL_QUEUE_STATE_PORT2((ucQueueIndex +uctemp +1) %(MHL_CBUS_TOTAL_QUEUE_NUM -1)) == CBUS_REQ_IDLE)
                    {
                        uctemp = (ucQueueIndex +uctemp +1) %(MHL_CBUS_TOTAL_QUEUE_NUM -1);

                        break;
                    }
                }

                if(uctemp != (MHL_CBUS_TOTAL_QUEUE_NUM -1)) // Have free queue
                {
                    if(GET_MHL_QUEUE_STATE_PORT2((uctemp +MHL_CBUS_TOTAL_QUEUE_NUM -2) %(MHL_CBUS_TOTAL_QUEUE_NUM -1)) == CBUS_REQ_IDLE)
                    {
                        uctemp = (uctemp +MHL_CBUS_TOTAL_QUEUE_NUM -2) %(MHL_CBUS_TOTAL_QUEUE_NUM -1);
                    }

                    memcpy(&(GET_MHL_CBUS_QUEUE_PORT2(uctemp)), pReq, sizeof(CbusReq_S));

                    bQueueFlag = TRUE;
                }
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            if(!GET_MHL_CBUS_TX_QUEUE_FREEZE3())
            {
                ucQueueIndex = GET_MHL_TX_QUEUE_INDEX_PORT3();

                // Search free queue
                for(uctemp = 0; uctemp <(MHL_CBUS_TOTAL_QUEUE_NUM -1); uctemp++)
                {
                    if(GET_MHL_QUEUE_STATE_PORT3((ucQueueIndex +uctemp +1) %(MHL_CBUS_TOTAL_QUEUE_NUM -1)) == CBUS_REQ_IDLE)
                    {
                        uctemp = (ucQueueIndex +uctemp +1) %(MHL_CBUS_TOTAL_QUEUE_NUM -1);

                        break;
                    }
                }

                if(uctemp != (MHL_CBUS_TOTAL_QUEUE_NUM -1)) // Have free queue
                {
                    if(GET_MHL_QUEUE_STATE_PORT3((uctemp +MHL_CBUS_TOTAL_QUEUE_NUM -2) %(MHL_CBUS_TOTAL_QUEUE_NUM -1)) == CBUS_REQ_IDLE)
                    {
                        uctemp = (uctemp +MHL_CBUS_TOTAL_QUEUE_NUM -2) %(MHL_CBUS_TOTAL_QUEUE_NUM -1);
                    }

                    memcpy(&(GET_MHL_CBUS_QUEUE_PORT3(uctemp)), pReq, sizeof(CbusReq_S));

                    bQueueFlag = TRUE;
                }
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            if(!GET_MHL_CBUS_TX_QUEUE_FREEZE4())
            {
                ucQueueIndex = GET_MHL_TX_QUEUE_INDEX_PORT4();

                // Search free queue
                for(uctemp = 0; uctemp <(MHL_CBUS_TOTAL_QUEUE_NUM -1); uctemp++)
                {
                    if(GET_MHL_QUEUE_STATE_PORT4((ucQueueIndex +uctemp +1) %(MHL_CBUS_TOTAL_QUEUE_NUM -1)) == CBUS_REQ_IDLE)
                    {
                        uctemp = (ucQueueIndex +uctemp +1) %(MHL_CBUS_TOTAL_QUEUE_NUM -1);

                        break;
                    }
                }

                if(uctemp != (MHL_CBUS_TOTAL_QUEUE_NUM -1)) // Have free queue
                {
                    if(GET_MHL_QUEUE_STATE_PORT4((uctemp +MHL_CBUS_TOTAL_QUEUE_NUM -2) %(MHL_CBUS_TOTAL_QUEUE_NUM -1)) == CBUS_REQ_IDLE)
                    {
                        uctemp = (uctemp +MHL_CBUS_TOTAL_QUEUE_NUM -2) %(MHL_CBUS_TOTAL_QUEUE_NUM -1);
                    }

                    memcpy(&(GET_MHL_CBUS_QUEUE_PORT4(uctemp)), pReq, sizeof(CbusReq_S));

                    bQueueFlag = TRUE;
                }
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            if(!GET_MHL_CBUS_TX_QUEUE_FREEZE5())
            {
                ucQueueIndex = GET_MHL_TX_QUEUE_INDEX_PORT5();

                // Search free queue
                for(uctemp = 0; uctemp <(MHL_CBUS_TOTAL_QUEUE_NUM -1); uctemp++)
                {
                    if(GET_MHL_QUEUE_STATE_PORT5((ucQueueIndex +uctemp +1) %(MHL_CBUS_TOTAL_QUEUE_NUM -1)) == CBUS_REQ_IDLE)
                    {
                        uctemp = (ucQueueIndex +uctemp +1) %(MHL_CBUS_TOTAL_QUEUE_NUM -1);

                        break;
                    }
                }

                if(uctemp != (MHL_CBUS_TOTAL_QUEUE_NUM -1)) // Have free queue
                {
                    if(GET_MHL_QUEUE_STATE_PORT5((uctemp +MHL_CBUS_TOTAL_QUEUE_NUM -2) %(MHL_CBUS_TOTAL_QUEUE_NUM -1)) == CBUS_REQ_IDLE)
                    {
                        uctemp = (uctemp +MHL_CBUS_TOTAL_QUEUE_NUM -2) %(MHL_CBUS_TOTAL_QUEUE_NUM -1);
                    }

                    memcpy(&(GET_MHL_CBUS_QUEUE_PORT5(uctemp)), pReq, sizeof(CbusReq_S));

                    bQueueFlag = TRUE;
                }
            }

            break;
#endif

        default:

            break;
    };

    return bQueueFlag;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusSendMSGCommand()
//  [Description]
//                  MHL Cbus MSC Send RCP or RAP
//  [Arguments]:
//                  subCmd: MSC subcommands
//                  databuf: command code
//  [Return]:
//                  TRUE: success
//                  FALSE: fail
//**************************************************************************
Bool _mdrv_mhl_CbusSendMSGCommand(BYTE ucPortSelect, BYTE ucSubCommand, BYTE ucKeyCode)
{
    Bool bFlag = FALSE;
    CbusReq_S stQueue;

    stQueue.cmd = MSC_MSC_MSG;
    stQueue.msgData[0] = ucSubCommand;
    stQueue.msgData[1] = ucKeyCode;
    stQueue.len = 2;
    stQueue.reqStatus = CBUS_REQ_PENDING;

    if(_mdrv_mhl_CbusTxQueueInsertMSC(ucPortSelect, &stQueue))
    {
        bFlag = TRUE;
    }

    return bFlag;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusSendSubMessage()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool _mdrv_mhl_CbusSendSubMessage(BYTE ucPortSelect, BYTE ucSubCommand, BYTE ucKeyCode, Bool bWaitReply)
{
    Bool bSendMSG = FALSE;

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            if(!GET_MHL_SEND_MSG_COMMAND_PORT0())
            {
                bSendMSG = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            if(!GET_MHL_SEND_MSG_COMMAND_PORT1())
            {
                bSendMSG = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            if(!GET_MHL_SEND_MSG_COMMAND_PORT2())
            {
                bSendMSG = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            if(!GET_MHL_SEND_MSG_COMMAND_PORT3())
            {
                bSendMSG = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            if(!GET_MHL_SEND_MSG_COMMAND_PORT4())
            {
                bSendMSG = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            if(!GET_MHL_SEND_MSG_COMMAND_PORT5())
            {
                bSendMSG = TRUE;
            }

            break;
#endif

        default:

            break;
    };

    if(bSendMSG)
    {
        bSendMSG = _mdrv_mhl_CbusSendMSGCommand(ucPortSelect, ucSubCommand, ucKeyCode);

        if(bWaitReply && bSendMSG)
        {
            _mdrv_mhl_SetMSGWaitResponseTimer(ucPortSelect, MHL_MSG_WAIT_RESPONSE_TIME);
        }
    }

    return bSendMSG;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_SetRCPReleaseSendTimer()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_SetRCPReleaseSendTimer(BYTE ucPortSelect, BYTE ucTimeTick, BYTE ucKeyCode)
{
    Bool bSetTimerFlag = FALSE;
    Bool bSendReleaseFlag = FALSE;
    Bool bSendReleaseFinish = FALSE;
    static BYTE ucPreKeyCode = MHL_RCP_KEY_SELECT_RELEASE;
    static BYTE ucTimerCount[MHL_INPUT_SELECT_MASK] = {0};

    if(ucTimeTick == 0)
    {
        if(ucTimerCount[ucPortSelect] > 0)
        {
            ucTimerCount[ucPortSelect]--;
        }
        else
        {
            bSendReleaseFlag = TRUE;
        }
    }
    else
    {
        if((ucTimerCount[ucPortSelect] != 0) && (ucPreKeyCode != ucKeyCode))
        {
            bSendReleaseFlag = TRUE;
        }
        else
        {
            ucTimerCount[ucPortSelect] = ucTimeTick;
        }

        bSetTimerFlag = TRUE;
    }

    do
    {
        switch(ucPortSelect)
        {
#if(MHL_FUNCTION_SUPPORT_PORT0)
            case MHL_INPUT_SELECT_PORT0:
                if(bSendReleaseFinish)
                {
                    CLR_MHL_SEND_RCP_RELEASE_ENABLE0();
                }
                else if(bSendReleaseFlag)
                {
                    bSendReleaseFlag = GET_MHL_SEND_RCP_RELEASE_ENABLE0();
                }
                else if(bSetTimerFlag)
                {
                    SET_MHL_SEND_RCP_RELEASE_ENABLE0();
                }

                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
            case MHL_INPUT_SELECT_PORT1:
                if(bSendReleaseFinish)
                {
                    CLR_MHL_SEND_RCP_RELEASE_ENABLE1();
                }
                else if(bSendReleaseFlag)
                {
                    bSendReleaseFlag = GET_MHL_SEND_RCP_RELEASE_ENABLE1();
                }
                else if(bSetTimerFlag)
                {
                    SET_MHL_SEND_RCP_RELEASE_ENABLE1();
                }

                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
            case MHL_INPUT_SELECT_PORT2:
                if(bSendReleaseFinish)
                {
                    CLR_MHL_SEND_RCP_RELEASE_ENABLE2();
                }
                else if(bSendReleaseFlag)
                {
                    bSendReleaseFlag = GET_MHL_SEND_RCP_RELEASE_ENABLE2();
                }
                else if(bSetTimerFlag)
                {
                    SET_MHL_SEND_RCP_RELEASE_ENABLE2();
                }

                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
            case MHL_INPUT_SELECT_PORT3:
                if(bSendReleaseFinish)
                {
                    CLR_MHL_SEND_RCP_RELEASE_ENABLE3();
                }
                else if(bSendReleaseFlag)
                {
                    bSendReleaseFlag = GET_MHL_SEND_RCP_RELEASE_ENABLE3();
                }
                else if(bSetTimerFlag)
                {
                    SET_MHL_SEND_RCP_RELEASE_ENABLE3();
                }

                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
            case MHL_INPUT_SELECT_PORT4:
                if(bSendReleaseFinish)
                {
                    CLR_MHL_SEND_RCP_RELEASE_ENABLE4();
                }
                else if(bSendReleaseFlag)
                {
                    bSendReleaseFlag = GET_MHL_SEND_RCP_RELEASE_ENABLE4();
                }
                else if(bSetTimerFlag)
                {
                    SET_MHL_SEND_RCP_RELEASE_ENABLE4();
                }

                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
            case MHL_INPUT_SELECT_PORT5:
                if(bSendReleaseFinish)
                {
                    CLR_MHL_SEND_RCP_RELEASE_ENABLE5();
                }
                else if(bSendReleaseFlag)
                {
                    bSendReleaseFlag = GET_MHL_SEND_RCP_RELEASE_ENABLE5();
                }
                else if(bSetTimerFlag)
                {
                    SET_MHL_SEND_RCP_RELEASE_ENABLE5();
                }

                break;
#endif

            default:

                break;
        };

        bSendReleaseFinish = FALSE;

        if(bSendReleaseFlag)
        {
            ucKeyCode = ucPreKeyCode |BIT7;

            if(_mdrv_mhl_CbusSendSubMessage(ucPortSelect, MSC_MSG_RCP, ucKeyCode, TRUE))
            {
                bSendReleaseFinish = TRUE;

                MHL_DRV_DPRINTF("** MHL send RCP release key code 0x%x", ucKeyCode);
            }

            bSendReleaseFlag = FALSE;
        }
        else if(bSetTimerFlag)
        {
            ucPreKeyCode = ucKeyCode;
        }
    }
    while(bSendReleaseFinish);
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_InsertVCInformation()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_InsertVCInformation(BYTE ucPortSelect, Bool bReceiveVC, BYTE ucVCNumber, BYTE ucVCFeatureID, BYTE ucVCSlotSize)
{
    Bool bFindVC = FALSE;
    BYTE uctemp = 0;
    BYTE ucTargetVC = MHL_VIRTUAL_CHANNEL_LENGTH;
    BYTE ucTotalTimeSlot = 0;
    BYTE ucResponseCode = MHL_VC_RESPONSE_ACCEPT;
    ST_VIRTUAL_CHANNEL_INFO *ucMHLVCInformation = 0;

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            ucMHLVCInformation = &GET_MHL_VC_INFORMATION_PORT0(0);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            ucMHLVCInformation = &GET_MHL_VC_INFORMATION_PORT1(0);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            ucMHLVCInformation = &GET_MHL_VC_INFORMATION_PORT2(0);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            ucMHLVCInformation = &GET_MHL_VC_INFORMATION_PORT3(0);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            ucMHLVCInformation = &GET_MHL_VC_INFORMATION_PORT4(0);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            ucMHLVCInformation = &GET_MHL_VC_INFORMATION_PORT5(0);
            break;
#endif

        default:

            break;
    };

    if(ucVCFeatureID >= MHL_VC_FEATURE_ID_END) // Invalid feature ID
    {
        ucResponseCode = MHL_VC_RESPONSE_REJECT_FEATURE_ID;
    }
    else if((ucVCFeatureID == MHL_VC_FEATURE_ID_EMSC) && (ucVCSlotSize != MHL_EMSC_TIME_SLOT_DEFAULT))
    {
        ucResponseCode = MHL_VC_RESPONSE_REJECT_CHANNEL_SIZE;
    }

    for(uctemp = 0; uctemp <MHL_VIRTUAL_CHANNEL_LENGTH; uctemp++)
    {
        if(!bFindVC)
        {
            if(ucMHLVCInformation[uctemp].ucVCNumber == ucVCNumber) // VC present, information change
            {
                ucTargetVC = uctemp;

                bFindVC = TRUE;
            }
            else if(ucMHLVCInformation[uctemp].ucVCNumber == 0) // VC absent, insert new information
            {
                ucTargetVC = uctemp;

                ucMHLVCInformation[ucTargetVC].ucVCNumber = ucVCNumber;
                ucMHLVCInformation[ucTargetVC].ucVCFeatureID = ucVCFeatureID;

                break;
            }
        }

        ucTotalTimeSlot = ucTotalTimeSlot +ucMHLVCInformation[uctemp].ucVCSlotSize;
    }

    if(ucTargetVC < MHL_VIRTUAL_CHANNEL_LENGTH) // Can insert new VC information
    {
        ucMHLVCInformation[ucTargetVC].bVCConfirm = TRUE;

        if(ucMHLVCInformation[ucTargetVC].ucVCFeatureID != ucVCFeatureID) // Same VC number, but different feature ID
        {
            ucResponseCode = MHL_VC_RESPONSE_REJECT_NUMBER;
        }
        else if(ucTotalTimeSlot +ucVCSlotSize > MHL_ECBUS_S_TIME_SLOT_TOTAL -1) // Time slot not enough
        {
            ucResponseCode = MHL_VC_RESPONSE_REJECT_CHANNEL_SIZE;
        }
        else
        {
            if(bFindVC)
            {
                ucMHLVCInformation[ucTargetVC].ucVCResponseCode = ucMHLVCInformation[ucTargetVC].ucVCSlotSize;
            }

            ucMHLVCInformation[ucTargetVC].ucVCSlotSize = ucVCSlotSize;
        }

        if(bReceiveVC)
        {
            ucMHLVCInformation[ucTargetVC].ucVCResponseCode = ucResponseCode;
        }
    }
    else // Virtual channel length is too small
    {
        MHL_DRV_DPRINTF("** MHL virtual channel length too small port %d", ucPortSelect);
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CheckVCInformation()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_CheckVCInformation(BYTE ucPortSelect, BYTE ucVCNumber, BYTE ucVCFeatureID, BYTE ucResponseCode)
{
    BYTE uctemp = 0;
    BYTE ucTargetVC = MHL_VIRTUAL_CHANNEL_LENGTH;
    ST_VIRTUAL_CHANNEL_INFO *ucMHLVCInformation = 0;

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            ucMHLVCInformation = &GET_MHL_VC_INFORMATION_PORT0(0);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            ucMHLVCInformation = &GET_MHL_VC_INFORMATION_PORT1(0);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            ucMHLVCInformation = &GET_MHL_VC_INFORMATION_PORT2(0);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            ucMHLVCInformation = &GET_MHL_VC_INFORMATION_PORT3(0);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            ucMHLVCInformation = &GET_MHL_VC_INFORMATION_PORT4(0);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            ucMHLVCInformation = &GET_MHL_VC_INFORMATION_PORT5(0);
            break;
#endif

        default:

            break;
    };

    for(uctemp = 0; uctemp <MHL_VIRTUAL_CHANNEL_LENGTH; uctemp++)
    {
        if(ucMHLVCInformation[uctemp].ucVCNumber == ucVCNumber) // VC present, information change
        {
            ucTargetVC = uctemp;

            if(ucMHLVCInformation[uctemp].ucVCFeatureID != ucVCFeatureID)
            {

            }

            break;
        }
    }

    if(ucTargetVC < MHL_VIRTUAL_CHANNEL_LENGTH) // Can insert new VC information
    {
        if(ucResponseCode == MHL_VC_RESPONSE_REJECT_CHANNEL_SIZE)
        {
            ucMHLVCInformation[ucTargetVC].ucVCSlotSize = ucMHLVCInformation[ucTargetVC].ucVCResponseCode;
        }
        else if(ucResponseCode == MHL_VC_RESPONSE_REJECT_NUMBER)
        {
            ucMHLVCInformation[ucTargetVC].ucVCSlotSize = 0;
        }
        else if(ucResponseCode == MHL_VC_RESPONSE_REJECT_FEATURE_ID)
        {
            ucMHLVCInformation[ucTargetVC].ucVCSlotSize = 0;
        }

        ucMHLVCInformation[ucTargetVC].ucVCResponseCode = MHL_VC_RESPONSE_ACCEPT;
    }
}

#if(MHL_CBUS_SELF_TEST_SUPPORT)
//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusSelfTestHandler()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
WORD _mdrv_mhl_CbusSelfTestHandler(BYTE ucPortSelect)
{
    Bool bStateIncrease = TRUE;
    Bool bPseudoReceive = TRUE;
    WORD usTimerCount = 0;

    switch(GET_MHL_CBUS_SELF_TEST_STATE())
    {
        case MHL_CBUS_SELF_TEST_NONE:
        case MHL_CBUS_SELF_TEST_END:
            bStateIncrease = FALSE;
            bPseudoReceive = FALSE;
            break;

#if(MHL_SELF_TEST_TYPE == MHL_SELF_TEST_VC_ASSIGN)
        case MHL_CBUS_SELF_TEST_START:
            usTimerCount = 2;
            break;

        case MHL_CBUS_SELF_TEST_STEP1:
            usTimerCount = 2;
            break;

        case MHL_CBUS_SELF_TEST_STEP2:
            usTimerCount = 2;
            break;

        case MHL_CBUS_SELF_TEST_STEP3:
            usTimerCount = 2;
            break;

        case MHL_CBUS_SELF_TEST_STEP4:
            usTimerCount = 2;
            break;

        case MHL_CBUS_SELF_TEST_STEP5:
            usTimerCount = 2;
            break;

        case MHL_CBUS_SELF_TEST_STEP6:
            usTimerCount = 500;
            break;

        case MHL_CBUS_SELF_TEST_STEP7:
            usTimerCount = 500;
            break;

#elif(MHL_SELF_TEST_TYPE == MHL_SELF_TEST_FEATURE_REQUEST)
        case MHL_CBUS_SELF_TEST_START:
            usTimerCount = 2;
            break;

        case MHL_CBUS_SELF_TEST_STEP1:
            usTimerCount = 50;
            break;

        case MHL_CBUS_SELF_TEST_STEP2:
            usTimerCount = 500;
            break;

#endif

        default:

            break;
    };

    if(bStateIncrease)
    {
        INC_MHL_CBUS_SELF_TEST_STATE();
    }

    if(bPseudoReceive)
    {
        switch(ucPortSelect)
        {
#if(MHL_FUNCTION_SUPPORT_PORT0)
            case MHL_INPUT_SELECT_PORT0:
                SET_MHL_CBUS_SELF_TEST_MODE0();
                INC_MHL_MSC_RECEIVE_COUNTER0();
                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
            case MHL_INPUT_SELECT_PORT1:
                SET_MHL_CBUS_SELF_TEST_MODE1();
                INC_MHL_MSC_RECEIVE_COUNTER1();
                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
            case MHL_INPUT_SELECT_PORT2:
                SET_MHL_CBUS_SELF_TEST_MODE2();
                INC_MHL_MSC_RECEIVE_COUNTER2();
                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
            case MHL_INPUT_SELECT_PORT3:
                SET_MHL_CBUS_SELF_TEST_MODE3();
                INC_MHL_MSC_RECEIVE_COUNTER3();
                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
            case MHL_INPUT_SELECT_PORT4:
                SET_MHL_CBUS_SELF_TEST_MODE4();
                INC_MHL_MSC_RECEIVE_COUNTER4();
                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
            case MHL_INPUT_SELECT_PORT5:
                SET_MHL_CBUS_SELF_TEST_MODE5();
                INC_MHL_MSC_RECEIVE_COUNTER5();
                break;
#endif

            default:

                break;
        };
    }

    return usTimerCount;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_GetCbusSelfTestCommand()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE _mdrv_mhl_GetCbusSelfTestCommand(BYTE ucReceiveCount,BYTE *ucReceiveMSC)
{
    BYTE uctemp = 0;
    BYTE ucCommandMask = 1;
    BYTE ucCheckSum = 0;

    ucReceiveMSC[0] = 0;

    switch(GET_MHL_CBUS_SELF_TEST_STATE())
    {
#if(MHL_SELF_TEST_TYPE == MHL_SELF_TEST_VC_ASSIGN)
        case MHL_CBUS_SELF_TEST_STEP1:
            if(ucReceiveCount == 0)
            {
                ucReceiveMSC[0] = 3;
                ucReceiveMSC[1] = 0x60;
                ucReceiveMSC[2] = 0x32;
                ucReceiveMSC[3] = 0x30;
            }

            break;

        case MHL_CBUS_SELF_TEST_STEP2:
            if(ucReceiveCount == 0)
            {
                ucReceiveMSC[0] = 3;
                ucReceiveMSC[1] = 0x60;
                ucReceiveMSC[2] = 0x20;
                ucReceiveMSC[3] = 0x04;
            }

            break;

        case MHL_CBUS_SELF_TEST_STEP3:
            if(ucReceiveCount == 0)
            {
                ucReceiveMSC[0] = 19;
                ucReceiveMSC[1] = 0x6C;
                ucReceiveMSC[2] = 0x40;
                ucReceiveMSC[3] = 0x00; //burst id_H
                ucReceiveMSC[4] = 0x38; //burst id_L
                //ucReceiveMSC[5] = ; //check sum
                ucReceiveMSC[6] = 0x04; //total entries
                ucReceiveMSC[7] = 0x01; //sequence index
                ucReceiveMSC[8] = 0x03; //Number of entries in this burst
                ucReceiveMSC[9] = 0x01; //VC_NUM
                ucReceiveMSC[10] = 0x00; //feature id
                ucReceiveMSC[11] = 0x04; //channel size/response
                ucReceiveMSC[12] =  0x08; //VC_NUM
                ucReceiveMSC[13] = 0x01; //feature id
                ucReceiveMSC[14] = 0x02; //channel size/response
                ucReceiveMSC[15] = 0X04; //VC_NUM
                ucReceiveMSC[16] = 0x02; //feature id
                ucReceiveMSC[17] = 0x05; //channel size/response
                ucReceiveMSC[18] = 0;
                ucReceiveMSC[19] = 0x32;

                for( uctemp=3; uctemp <=18; uctemp++)
                {
                    ucCheckSum = ucCheckSum +ucReceiveMSC[uctemp];
                }
                ucReceiveMSC[5] = (ucCheckSum ^0xFF) +1;
            }

            break;

        case MHL_CBUS_SELF_TEST_STEP4:
            if(ucReceiveCount == 0)
            {
                ucReceiveMSC[0] = 3;
                ucReceiveMSC[1] = 0x60;
                ucReceiveMSC[2] = 0x20;
                ucReceiveMSC[3] = 0x02;
            }

            break;

        case MHL_CBUS_SELF_TEST_STEP5:
            if(ucReceiveCount == 0)
            {
                ucReceiveMSC[0] = 3;
                ucReceiveMSC[1] = 0x60;
                ucReceiveMSC[2] = 0x20;
                ucReceiveMSC[3] = 0x04;
            }

            break;

        case MHL_CBUS_SELF_TEST_STEP6:
            if(ucReceiveCount == 0)
            {
                ucReceiveMSC[0] = 19;
                ucReceiveMSC[1] = 0x6C;
                ucReceiveMSC[2] = 0x40;
                ucReceiveMSC[3] = 0x00; //burst id_H
                ucReceiveMSC[4] = 0x38; //burst id_L
                //ucReceiveMSC[5] = ; //check sum
                ucReceiveMSC[6] = 0x04; //total entries
                ucReceiveMSC[7] = 0x02; //sequence index
                ucReceiveMSC[8] = 0x01; //Number of entries in this burst
                ucReceiveMSC[9] = 0x09; //VC_NUM
                ucReceiveMSC[10] = 0x05; //feature id
                ucReceiveMSC[11] = 0x03; //channel size/response
                ucReceiveMSC[12] =  0x00; //VC_NUM
                ucReceiveMSC[13] = 0x00; //feature id
                ucReceiveMSC[14] = 0x00; //channel size/response
                ucReceiveMSC[15] = 0x00; //VC_NUM
                ucReceiveMSC[16] = 0x00; //feature id
                ucReceiveMSC[17] = 0x00; //channel size/response
                ucReceiveMSC[18] = 0;
                ucReceiveMSC[19] = 0x32;

                for( uctemp=3; uctemp <=18; uctemp++)
                {
                    ucCheckSum = ucCheckSum +ucReceiveMSC[uctemp];
                }
                ucReceiveMSC[5] = (ucCheckSum ^0xFF) +1;
            }

            break;

        case MHL_CBUS_SELF_TEST_STEP7:
            if(ucReceiveCount == 0)
            {
                ucReceiveMSC[0] = 3;
                ucReceiveMSC[1] = 0x60;
                ucReceiveMSC[2] = 0x20;
                ucReceiveMSC[3] = 0x02;
            }

            break;

        case MHL_CBUS_SELF_TEST_STEP8:
            if(ucReceiveCount == 0)
            {
                ucReceiveMSC[0] = 3;
                ucReceiveMSC[1] = 0x60;
                ucReceiveMSC[2] = 0x31;
                ucReceiveMSC[3] = 0x0C;

                _mdrv_mhl_InsertVCInformation(MHL_CBUS_SELT_TEST_PORT, FALSE, 9, 5, 6);
                SET_MHL_SEND_VC_INFORMATION_PORT2();
            }

#elif(MHL_SELF_TEST_TYPE == MHL_SELF_TEST_FEATURE_REQUEST)
        case MHL_CBUS_SELF_TEST_STEP1:
            if(ucReceiveCount == 0)
            {
                ucReceiveMSC[0] = 3;
                ucReceiveMSC[1] = 0x60;
                ucReceiveMSC[2] = 0x32;
                ucReceiveMSC[3] = 0x30;
            }

            break;

        case MHL_CBUS_SELF_TEST_STEP2:
            if(ucReceiveCount == 0)
            {
                ucReceiveMSC[0] = 3;
                ucReceiveMSC[1] = 0x60;
                ucReceiveMSC[2] = 0x20;
                ucReceiveMSC[3] = 0x20;
            }

            break;

        case MHL_CBUS_SELF_TEST_STEP3:
            mdrv_mhl_CbusSendRCPCommand(MHL_CBUS_SELT_TEST_PORT, 0);

            break;

#endif

            break;

        default:

            break;
    };

    return ucCommandMask;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_SetCbusSelfTestTimer()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_SetCbusSelfTestTimer(BYTE ucPortSelect, WORD usTimeTick)
{
    static WORD usTimerCount = 0;

    if(usTimeTick == 0)
    {
        if(usTimerCount == 0)
        {
            usTimerCount = _mdrv_mhl_CbusSelfTestHandler(ucPortSelect);
        }
        else
        {
            usTimerCount--;
        }
    }
    else
    {
        usTimerCount = usTimeTick;

        SET_MHL_CBUS_SELF_TEST_STATE(MHL_CBUS_SELF_TEST_START);
    }
}

#endif

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_InsertCellPhoneID()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_InsertCellPhoneID(BYTE ucPortSelect, WORD usAdopterID, WORD usDeviceID)
{
    BYTE ucCellPhoneID = MHL_CELL_PHONE_ID_NONE;

    switch(usAdopterID)
    {
        case MHL_ADOPTER_ID_ROKU:
            ucCellPhoneID = MHL_CELL_PHONE_ID_ROKU_DEVICE;
            break;

        case MHL_ADOPTER_ID_HTC:
            ucCellPhoneID = MHL_CELL_PHONE_ID_HTC_BUTTER_FLY;

#if(MHL_HTC_SENSATION_DC_PATCH)
            if(!_mdrv_mhl_SensationDConPatch(ucPortSelect, FALSE))
            {
                _mdrv_mhl_SensationDConPatch(ucPortSelect, TRUE);
            }
#elif(MHL_HTC_BUFFER_FLY_CHARGE_PATCH)
            if(!_mdrv_mhl_BufferFlyReallyChargePatch(ucPortSelect, FALSE))
            {
                _mdrv_mhl_BufferFlyReallyChargePatch(ucPortSelect, TRUE);
            }

#endif

            break;

        case MHL_ADOPTER_ID_DOCOMO:
            ucCellPhoneID = MHL_CELL_PHONE_ID_DOCOMO;
            break;

        case MHL_ADOPTER_ID_HUAWEI:
            ucCellPhoneID = MHL_CELL_PHONE_ID_HUAWEI;
            break;

        case MHL_ADOPTER_ID_SAMSUNG:
            ucCellPhoneID = MHL_CELL_PHONE_ID_SAMSUNG;
            break;

        case MHL_ADOPTER_ID_SONY:
            ucCellPhoneID = MHL_CELL_PHONE_ID_SONY;
            break;            

        default:
            if(usDeviceID == 0)
            {

            }

            break;
    };

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            SET_MHL_CELL_PHONE_ID_PORT0(ucCellPhoneID);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            SET_MHL_CELL_PHONE_ID_PORT1(ucCellPhoneID);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            SET_MHL_CELL_PHONE_ID_PORT2(ucCellPhoneID);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            SET_MHL_CELL_PHONE_ID_PORT3(ucCellPhoneID);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            SET_MHL_CELL_PHONE_ID_PORT4(ucCellPhoneID);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            SET_MHL_CELL_PHONE_ID_PORT5(ucCellPhoneID);
            break;
#endif

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_InsertCellPhoneID()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE _mdrv_mhl_GetCellPhoneID(BYTE ucPortSelect)
{
    BYTE ucCellPhoneID = MHL_CELL_PHONE_ID_NONE;

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            ucCellPhoneID = GET_MHL_CELL_PHONE_ID_PORT0();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            ucCellPhoneID = GET_MHL_CELL_PHONE_ID_PORT1();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            ucCellPhoneID = GET_MHL_CELL_PHONE_ID_PORT2();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            ucCellPhoneID = GET_MHL_CELL_PHONE_ID_PORT3();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            ucCellPhoneID = GET_MHL_CELL_PHONE_ID_PORT4();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            ucCellPhoneID = GET_MHL_CELL_PHONE_ID_PORT5();
            break;
#endif

        default:

            break;
    };

    return ucCellPhoneID;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_DisplayChangeCellPhonePatch()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_DisplayChangeCellPhonePatch(BYTE ucPortSelect)
{
    BYTE ucCellPhoneID = _mdrv_mhl_GetCellPhoneID(ucPortSelect);

    if(ucCellPhoneID == MHL_CELL_PHONE_ID_DOCOMO)
    {
        mhal_mhl_CbusForceToStandby(ucPortSelect);
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CheckDisplayConfig()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_CheckDisplayConfig(void)
{
    static BYTE ucPreDisplayConfig = 0;
    BYTE ucPortSelect = 0;
    BYTE ucDisplayConfig = 0;
    BYTE ucDisplayChange = 0;

    if(GET_MHL_SYSTEM_CONFIG_CHANGE())
    {
        CLR_MHL_SYSTEM_CONFIG_CHANGE();

#if(MHL_FUNCTION_SUPPORT_PORT0)
        if(GET_MHL_FUNCTION_ENABLE_PORT0() && (GET_MHL_DISPLAY_ENABLE_PORT0() || GET_MHL_SYSTEM_FORCE_DISPLAY()))
        {
            SET_MHL_SEND_DISPLAY_COMMAND0();

            ucDisplayConfig = MASKSET(ucDisplayConfig, _BIT(0), _BIT(0));
        }
        else
        {
            CLR_MHL_SEND_DISPLAY_COMMAND0();
        }
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        if(GET_MHL_FUNCTION_ENABLE_PORT1() && (GET_MHL_DISPLAY_ENABLE_PORT1() || GET_MHL_SYSTEM_FORCE_DISPLAY()))
        {
            SET_MHL_SEND_DISPLAY_COMMAND1();

            ucDisplayConfig = MASKSET(ucDisplayConfig, _BIT(1), _BIT(1));
        }
        else
        {
            CLR_MHL_SEND_DISPLAY_COMMAND1();
        }
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        if(GET_MHL_FUNCTION_ENABLE_PORT2() && (GET_MHL_DISPLAY_ENABLE_PORT2() || GET_MHL_SYSTEM_FORCE_DISPLAY()))
        {
            SET_MHL_SEND_DISPLAY_COMMAND2();

            ucDisplayConfig = MASKSET(ucDisplayConfig, _BIT(2), _BIT(2));
        }
        else
        {
            CLR_MHL_SEND_DISPLAY_COMMAND2();
        }
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        if(GET_MHL_FUNCTION_ENABLE_PORT3() && (GET_MHL_DISPLAY_ENABLE_PORT3() || GET_MHL_SYSTEM_FORCE_DISPLAY()))
        {
            SET_MHL_SEND_DISPLAY_COMMAND3();

            ucDisplayConfig = MASKSET(ucDisplayConfig, _BIT(3), _BIT(3));
        }
        else
        {
            CLR_MHL_SEND_DISPLAY_COMMAND3();
        }
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        if(GET_MHL_FUNCTION_ENABLE_PORT4() && (GET_MHL_DISPLAY_ENABLE_PORT4() || GET_MHL_SYSTEM_FORCE_DISPLAY()))
        {
            SET_MHL_SEND_DISPLAY_COMMAND4();

            ucDisplayConfig = MASKSET(ucDisplayConfig, _BIT(4), _BIT(4));
        }
        else
        {
            CLR_MHL_SEND_DISPLAY_COMMAND4();
        }
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        if(GET_MHL_FUNCTION_ENABLE_PORT5() && (GET_MHL_DISPLAY_ENABLE_PORT5() || GET_MHL_SYSTEM_FORCE_DISPLAY()))
        {
            SET_MHL_SEND_DISPLAY_COMMAND5();

            ucDisplayConfig = MASKSET(ucDisplayConfig, _BIT(5), _BIT(5));
        }
        else
        {
            CLR_MHL_SEND_DISPLAY_COMMAND5();
        }
#endif

        ucDisplayChange = (ucPreDisplayConfig ^ucDisplayConfig) &ucDisplayConfig;

        mhal_mhl_SourceChangeProc(GET_MHL_DISPLAY_ENABLE_INDEX());

        ucPreDisplayConfig = ucDisplayConfig;

        if(ucDisplayChange > 0)
        {
            for(ucPortSelect = MHL_INPUT_SELECT_PORT0; ucPortSelect < MHL_INPUT_SELECT_MASK; ucPortSelect++)
            {
                if(ucDisplayChange %2)
                {
                    _mdrv_mhl_DisplayChangeCellPhonePatch(ucPortSelect);
                }

                ucDisplayChange = ucDisplayChange >>1;
            }
        }
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_GetECbusState()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE _mdrv_mhl_GetECbusState(BYTE ucPortSelect)
{
    BYTE ucECbusState = MHL_ECBUS_STATE_NONE;
    
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            ucECbusState = GET_MHL_ECBUS_STATE_PORT0();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            ucECbusState = GET_MHL_ECBUS_STATE_PORT1();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            ucECbusState = GET_MHL_ECBUS_STATE_PORT2();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            ucECbusState = GET_MHL_ECBUS_STATE_PORT3();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            ucECbusState = GET_MHL_ECBUS_STATE_PORT4();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            ucECbusState = GET_MHL_ECBUS_STATE_PORT5();
            break;
#endif

        default:

            break;
    };

    return ucECbusState;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_SetECbusState()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_SetECbusState(BYTE ucPortSelect, BYTE ucECbusState)
{
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            SET_MHL_ECBUS_STATE_PORT0(ucECbusState);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            SET_MHL_ECBUS_STATE_PORT1(ucECbusState);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            SET_MHL_ECBUS_STATE_PORT2(ucECbusState);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            SET_MHL_ECBUS_STATE_PORT3(ucECbusState);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            SET_MHL_ECBUS_STATE_PORT4(ucECbusState);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            SET_MHL_ECBUS_STATE_PORT5(ucECbusState);
            break;
#endif

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_GetECbusSupportFlag()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool _mdrv_mhl_GetECbusSupportFlag(BYTE ucPortSelect)
{
    Bool bECbusSupport = FALSE;

    if(tMHL_DEVCAP_TABLE_PORT[ucPortSelect][MHL_CBUS_VERSION] >= 0x30)
    {
        switch(ucPortSelect)
        {
#if(MHL_FUNCTION_SUPPORT_PORT0)
            case MHL_INPUT_SELECT_PORT0:
#if(MHL_SUPPORT_MHL3_PORT0)
                bECbusSupport = TRUE;
#endif
                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
            case MHL_INPUT_SELECT_PORT1:
#if(MHL_SUPPORT_MHL3_PORT1)
                bECbusSupport = TRUE;
#endif
                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
            case MHL_INPUT_SELECT_PORT2:
#if(MHL_SUPPORT_MHL3_PORT2)
                bECbusSupport = TRUE;
#endif
                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
            case MHL_INPUT_SELECT_PORT3:
#if(MHL_SUPPORT_MHL3_PORT3)
                bECbusSupport = TRUE;
#endif
                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
            case MHL_INPUT_SELECT_PORT4:
#if(MHL_SUPPORT_MHL3_PORT4)
                bECbusSupport = TRUE;
#endif
                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
            case MHL_INPUT_SELECT_PORT5:
#if(MHL_SUPPORT_MHL3_PORT5)
                bECbusSupport = TRUE;
#endif
                break;
#endif

            default:

                break;
        };

        if(!bECbusSupport)
        {
            tMHL_DEVCAP_TABLE_PORT[ucPortSelect][MHL_CBUS_VERSION] = MHL_DEFAULT_2CLASS_VERSION;
        }
    }

    return bECbusSupport;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_ClearCbusFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_ClearCbusFlag(BYTE ucPortSelect, BYTE ucSelect)
{
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            switch(ucSelect)
            {
                case MHL_CLEAR_INITIAL_FLAG:
                    SET_MHL_READ_DEVCAP_MASK_PORT0(MHL_READ_DEVCAP_MASK_PORT0);
                    break;

                case MHL_CLEAR_CABLE_LOSS_FLAG:
                    CLR_MHL_CBUS_CONNECT0();
                    SET_MHL_PHY_CLOCK_MODE_PORT0(MHL_PHY_CLOCK_RESERVED);
                case MHL_CLEAR_CONNECT_LOSS_FLAG:
                    CLR_MHL_CBUS_DISPLAY_FLAG0();
                    CLR_MHL_RECEIVE_FLAG_PORT0();
                    CLR_MHL_SEND_FLAG_PORT0();
                    CLR_MHL_REQUEST_STATE_ALL_PORT0();
                    SET_MHL_MSC_RECEIVE_COUNTER0(0);
                    SET_MHL_EMSC_RECEIVE_COUNTER0(0);
                    SET_MHL_WRITE_BURST_SEQUENCE0(0);
                    CLR_MHL_CELL_PHONE_ID_PORT0();
                    CLR_MHL_ENABLE_CBUS_ENHANCE0();
                    CLR_MHL_CBUS_TX_QUEUE_FREEZE0();
                    CLR_MHL_ECBUS_STATE_PORT0();
                    SET_MHL_EMSC_RX_BUFFER_SIZE0(MHL_EMSC_DATA_SIZE);
                    SET_MHL_EMSC_TX_BUFFER_SIZE0(MHL_EMSC_DATA_SIZE);
                    memset(&stMHLBlock0.MhlCbusInfo.request, 0, sizeof(stMHLBlock0.MhlCbusInfo.request));

                    break;

                case MHL_CLEAR_CBUS_TIME_OUT_FLAG:
                    CLR_MHL_CBUS_SILENCE0();
                    break;

                case MHL_CLEAR_MSG_RESPONSE_FLAG:
                    CLR_MHL_SEND_MSG_COMMAND_PORT0();
                    break;

                default:

                    break;
            };

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            switch(ucSelect)
            {
                case MHL_CLEAR_INITIAL_FLAG:
                    SET_MHL_READ_DEVCAP_MASK_PORT1(MHL_READ_DEVCAP_MASK_PORT1);
                    break;

                case MHL_CLEAR_CABLE_LOSS_FLAG:
                    CLR_MHL_CBUS_CONNECT1();
                    SET_MHL_PHY_CLOCK_MODE_PORT1(MHL_PHY_CLOCK_RESERVED);
                case MHL_CLEAR_CONNECT_LOSS_FLAG:
                    CLR_MHL_CBUS_DISPLAY_FLAG1();
                    CLR_MHL_RECEIVE_FLAG_PORT1();
                    CLR_MHL_SEND_FLAG_PORT1();
                    CLR_MHL_REQUEST_STATE_ALL_PORT1();
                    SET_MHL_MSC_RECEIVE_COUNTER1(0);
                    SET_MHL_EMSC_RECEIVE_COUNTER1(0);
                    SET_MHL_WRITE_BURST_SEQUENCE1(0);
                    CLR_MHL_CELL_PHONE_ID_PORT1();
                    CLR_MHL_ENABLE_CBUS_ENHANCE1();
                    CLR_MHL_CBUS_TX_QUEUE_FREEZE1();
                    CLR_MHL_ECBUS_STATE_PORT1();
                    SET_MHL_EMSC_RX_BUFFER_SIZE1(MHL_EMSC_DATA_SIZE);
                    SET_MHL_EMSC_TX_BUFFER_SIZE1(MHL_EMSC_DATA_SIZE);
                    memset(&stMHLBlock1.MhlCbusInfo.request, 0, sizeof(stMHLBlock1.MhlCbusInfo.request));

                    break;

                case MHL_CLEAR_CBUS_TIME_OUT_FLAG:
                    CLR_MHL_CBUS_SILENCE1();
                    break;

                case MHL_CLEAR_MSG_RESPONSE_FLAG:
                    CLR_MHL_SEND_MSG_COMMAND_PORT1();
                    break;

                default:

                    break;
            };

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            switch(ucSelect)
            {
                case MHL_CLEAR_INITIAL_FLAG:
                    SET_MHL_READ_DEVCAP_MASK_PORT2(MHL_READ_DEVCAP_MASK_PORT2);
                    break;

                case MHL_CLEAR_CABLE_LOSS_FLAG:
                    CLR_MHL_CBUS_CONNECT2();
                    SET_MHL_PHY_CLOCK_MODE_PORT2(MHL_PHY_CLOCK_RESERVED);
                case MHL_CLEAR_CONNECT_LOSS_FLAG:
                    CLR_MHL_CBUS_DISPLAY_FLAG2();
                    CLR_MHL_RECEIVE_FLAG_PORT2();
                    CLR_MHL_SEND_FLAG_PORT2();
                    CLR_MHL_REQUEST_STATE_ALL_PORT2();
                    SET_MHL_MSC_RECEIVE_COUNTER2(0);
                    SET_MHL_EMSC_RECEIVE_COUNTER2(0);
                    SET_MHL_WRITE_BURST_SEQUENCE2(0);
                    CLR_MHL_CELL_PHONE_ID_PORT2();
                    CLR_MHL_ENABLE_CBUS_ENHANCE2();
                    CLR_MHL_CBUS_TX_QUEUE_FREEZE2();
                    CLR_MHL_ECBUS_STATE_PORT2();
                    SET_MHL_EMSC_RX_BUFFER_SIZE2(MHL_EMSC_DATA_SIZE);
                    SET_MHL_EMSC_TX_BUFFER_SIZE2(MHL_EMSC_DATA_SIZE);
                    memset(&stMHLBlock2.MhlCbusInfo.request, 0, sizeof(stMHLBlock2.MhlCbusInfo.request));

                    break;

                case MHL_CLEAR_CBUS_TIME_OUT_FLAG:
                    CLR_MHL_CBUS_SILENCE2();
                    break;

                case MHL_CLEAR_MSG_RESPONSE_FLAG:
                    CLR_MHL_SEND_MSG_COMMAND_PORT2();
                    break;

                default:

                    break;
            };

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            switch(ucSelect)
            {
                case MHL_CLEAR_INITIAL_FLAG:
                    SET_MHL_READ_DEVCAP_MASK_PORT3(MHL_READ_DEVCAP_MASK_PORT3);
                    break;

                case MHL_CLEAR_CABLE_LOSS_FLAG:
                    CLR_MHL_CBUS_CONNECT3();
                    SET_MHL_PHY_CLOCK_MODE_PORT3(MHL_PHY_CLOCK_RESERVED);
                case MHL_CLEAR_CONNECT_LOSS_FLAG:
                    CLR_MHL_CBUS_DISPLAY_FLAG3();
                    CLR_MHL_RECEIVE_FLAG_PORT3();
                    CLR_MHL_SEND_FLAG_PORT3();
                    CLR_MHL_REQUEST_STATE_ALL_PORT3();
                    SET_MHL_MSC_RECEIVE_COUNTER3(0);
                    SET_MHL_EMSC_RECEIVE_COUNTER3(0);
                    SET_MHL_WRITE_BURST_SEQUENCE3(0);
                    CLR_MHL_CELL_PHONE_ID_PORT3();
                    CLR_MHL_ENABLE_CBUS_ENHANCE3();
                    CLR_MHL_CBUS_TX_QUEUE_FREEZE3();
                    CLR_MHL_ECBUS_STATE_PORT3();
                    SET_MHL_EMSC_RX_BUFFER_SIZE3(MHL_EMSC_DATA_SIZE);
                    SET_MHL_EMSC_TX_BUFFER_SIZE3(MHL_EMSC_DATA_SIZE);
                    memset(&stMHLBlock3.MhlCbusInfo.request, 0, sizeof(stMHLBlock3.MhlCbusInfo.request));

                    break;

                case MHL_CLEAR_CBUS_TIME_OUT_FLAG:
                    CLR_MHL_CBUS_SILENCE3();
                    break;

                case MHL_CLEAR_MSG_RESPONSE_FLAG:
                    CLR_MHL_SEND_MSG_COMMAND_PORT3();
                    break;

                default:

                    break;
            };

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            switch(ucSelect)
            {
                case MHL_CLEAR_INITIAL_FLAG:
                    SET_MHL_READ_DEVCAP_MASK_PORT4(MHL_READ_DEVCAP_MASK_PORT4);
                    break;

                case MHL_CLEAR_CABLE_LOSS_FLAG:
                    CLR_MHL_CBUS_CONNECT4();
                    SET_MHL_PHY_CLOCK_MODE_PORT4(MHL_PHY_CLOCK_RESERVED);
                case MHL_CLEAR_CONNECT_LOSS_FLAG:
                    CLR_MHL_CBUS_DISPLAY_FLAG4();
                    CLR_MHL_RECEIVE_FLAG_PORT4();
                    CLR_MHL_SEND_FLAG_PORT4();
                    CLR_MHL_REQUEST_STATE_ALL_PORT4();
                    SET_MHL_MSC_RECEIVE_COUNTER4(0);
                    SET_MHL_EMSC_RECEIVE_COUNTER4(0);
                    SET_MHL_WRITE_BURST_SEQUENCE4(0);
                    CLR_MHL_CELL_PHONE_ID_PORT4();
                    CLR_MHL_ENABLE_CBUS_ENHANCE4();
                    CLR_MHL_CBUS_TX_QUEUE_FREEZE4();
                    CLR_MHL_ECBUS_STATE_PORT4();
                    SET_MHL_EMSC_RX_BUFFER_SIZE4(MHL_EMSC_DATA_SIZE);
                    SET_MHL_EMSC_TX_BUFFER_SIZE4(MHL_EMSC_DATA_SIZE);
                    memset(&stMHLBlock4.MhlCbusInfo.request, 0, sizeof(stMHLBlock4.MhlCbusInfo.request));

                    break;

                case MHL_CLEAR_CBUS_TIME_OUT_FLAG:
                    CLR_MHL_CBUS_SILENCE4();
                    break;

                case MHL_CLEAR_MSG_RESPONSE_FLAG:
                    CLR_MHL_SEND_MSG_COMMAND_PORT4();
                    break;

                default:

                    break;
            };

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            switch(ucSelect)
            {
                case MHL_CLEAR_INITIAL_FLAG:
                    SET_MHL_READ_DEVCAP_MASK_PORT5(MHL_READ_DEVCAP_MASK_PORT5);
                    break;

                case MHL_CLEAR_CABLE_LOSS_FLAG:
                    CLR_MHL_CBUS_CONNECT5();
                    SET_MHL_PHY_CLOCK_MODE_PORT5(MHL_PHY_CLOCK_RESERVED);
                case MHL_CLEAR_CONNECT_LOSS_FLAG:
                    CLR_MHL_CBUS_DISPLAY_FLAG5();
                    CLR_MHL_RECEIVE_FLAG_PORT5();
                    CLR_MHL_SEND_FLAG_PORT5();
                    CLR_MHL_REQUEST_STATE_ALL_PORT5();
                    SET_MHL_MSC_RECEIVE_COUNTER5(0);
                    SET_MHL_EMSC_RECEIVE_COUNTER5(0);
                    SET_MHL_WRITE_BURST_SEQUENCE5(0);
                    CLR_MHL_CELL_PHONE_ID_PORT5();
                    CLR_MHL_ENABLE_CBUS_ENHANCE5();
                    CLR_MHL_CBUS_TX_QUEUE_FREEZE5();
                    CLR_MHL_ECBUS_STATE_PORT5();
                    SET_MHL_EMSC_RX_BUFFER_SIZE5(MHL_EMSC_DATA_SIZE);
                    SET_MHL_EMSC_TX_BUFFER_SIZE5(MHL_EMSC_DATA_SIZE);
                    memset(&stMHLBlock5.MhlCbusInfo.request, 0, sizeof(stMHLBlock5.MhlCbusInfo.request));

                    break;

                case MHL_CLEAR_CBUS_TIME_OUT_FLAG:
                    CLR_MHL_CBUS_SILENCE5();
                    break;

                case MHL_CLEAR_MSG_RESPONSE_FLAG:
                    CLR_MHL_SEND_MSG_COMMAND_PORT5();
                    break;

                default:

                    break;
            };

            break;
#endif

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_InsertCbusTimer()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_InsertCbusTimer(BYTE ucPortSelect, BYTE ucTimerValue)
{
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            if(ucTimerValue > 0)
            {
                SET_MHL_CBUS_SILENCE0();

                if(ucTimerValue < 255)
                {
                    ucTimerValue++;
                }
            }

            SET_MHL_CBUS_CONTROL_TIMER0(ucTimerValue);

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            if(ucTimerValue > 0)
            {
                SET_MHL_CBUS_SILENCE1();

                if(ucTimerValue < 255)
                {
                    ucTimerValue++;
                }
            }

            SET_MHL_CBUS_CONTROL_TIMER1(ucTimerValue);

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            if(ucTimerValue > 0)
            {
                SET_MHL_CBUS_SILENCE2();

                if(ucTimerValue < 255)
                {
                    ucTimerValue++;
                }
            }

            SET_MHL_CBUS_CONTROL_TIMER2(ucTimerValue);

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            if(ucTimerValue > 0)
            {
                SET_MHL_CBUS_SILENCE3();

                if(ucTimerValue < 255)
                {
                    ucTimerValue++;
                }
            }

            SET_MHL_CBUS_CONTROL_TIMER3(ucTimerValue);

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            if(ucTimerValue > 0)
            {
                SET_MHL_CBUS_SILENCE4();

                if(ucTimerValue < 255)
                {
                    ucTimerValue++;
                }
            }

            SET_MHL_CBUS_CONTROL_TIMER4(ucTimerValue);

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            if(ucTimerValue > 0)
            {
                SET_MHL_CBUS_SILENCE5();

                if(ucTimerValue < 255)
                {
                    ucTimerValue++;
                }
            }

            SET_MHL_CBUS_CONTROL_TIMER5(ucTimerValue);

            break;
#endif

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CheckCbusTimer()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_CheckCbusTimer(BYTE ucPortSelect)
{
    Bool bFlag = FALSE;

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            if(GET_MHL_CBUS_CONTROL_TIMER0() > 0)
            {
                DEC_MHL_CBUS_CONTROL_TIMER0();
            }
            else if(GET_MHL_CBUS_SILENCE0())
            {
                bFlag = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            if(GET_MHL_CBUS_CONTROL_TIMER1() > 0)
            {
                DEC_MHL_CBUS_CONTROL_TIMER1();
            }
            else if(GET_MHL_CBUS_SILENCE1())
            {
                bFlag = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            if(GET_MHL_CBUS_CONTROL_TIMER2() > 0)
            {
                DEC_MHL_CBUS_CONTROL_TIMER2();
            }
            else if(GET_MHL_CBUS_SILENCE2())
            {
                bFlag = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            if(GET_MHL_CBUS_CONTROL_TIMER3() > 0)
            {
                DEC_MHL_CBUS_CONTROL_TIMER3();
            }
            else if(GET_MHL_CBUS_SILENCE3())
            {
                bFlag = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            if(GET_MHL_CBUS_CONTROL_TIMER4() > 0)
            {
                DEC_MHL_CBUS_CONTROL_TIMER4();
            }
            else if(GET_MHL_CBUS_SILENCE4())
            {
                bFlag = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            if(GET_MHL_CBUS_CONTROL_TIMER5() > 0)
            {
                DEC_MHL_CBUS_CONTROL_TIMER5();
            }
            else if(GET_MHL_CBUS_SILENCE5())
            {
                bFlag = TRUE;
            }

            break;
#endif

        default:

            break;
    };

    if(bFlag)
    {
        _mdrv_mhl_ClearCbusFlag(ucPortSelect, MHL_CLEAR_CBUS_TIME_OUT_FLAG);
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_ClockModeSwitchProc()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_ClockModeSwitchProc(BYTE ucPortSelect, BYTE ucClockMode)
{
    Bool bFlag = FALSE;
    Bool bPacketPixelMode = FALSE;

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            if(GET_MHL_PHY_CLOCK_MODE_PORT0() != ucClockMode)
            {
                SET_MHL_PHY_CLOCK_MODE_PORT0(ucClockMode);

                bFlag = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            if(GET_MHL_PHY_CLOCK_MODE_PORT1() != ucClockMode)
            {
                SET_MHL_PHY_CLOCK_MODE_PORT1(ucClockMode);

                bFlag = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            if(GET_MHL_PHY_CLOCK_MODE_PORT2() != ucClockMode)
            {
                SET_MHL_PHY_CLOCK_MODE_PORT2(ucClockMode);

                bFlag = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            if(GET_MHL_PHY_CLOCK_MODE_PORT3() != ucClockMode)
            {
                SET_MHL_PHY_CLOCK_MODE_PORT3(ucClockMode);

                bFlag = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            if(GET_MHL_PHY_CLOCK_MODE_PORT4() != ucClockMode)
            {
                SET_MHL_PHY_CLOCK_MODE_PORT4(ucClockMode);

                bFlag = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            if(GET_MHL_PHY_CLOCK_MODE_PORT5() != ucClockMode)
            {
                SET_MHL_PHY_CLOCK_MODE_PORT5(ucClockMode);

                bFlag = TRUE;
            }

            break;
#endif

        default:

            break;
    };

    if(bFlag)
    {
        if(ucClockMode == MHL_PHY_CLOCK_PACKET_PIXEL)
        {
            bPacketPixelMode = TRUE;

            MHL_DRV_DPRINTF("** MHL switch to PPmode!!!! port %d", ucPortSelect);
        }
        else
        {
            MHL_DRV_DPRINTF("** MHL switch back to 24bit mode!!!! port %d", ucPortSelect);
        }

        mhal_mhl_ClockModeSwitchProc(ucPortSelect, bPacketPixelMode);
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CablePlugProc()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_CablePlugProc(BYTE ucPortSelect)
{
    Bool bFlag = FALSE;

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            if(!GET_MHL_CABLE_PLUGGED0())
            {
                SET_MHL_CABLE_PLUGGED0();

                SET_MHL_PHY_CLOCK_MODE_PORT0(MHL_PHY_CLOCK_NORMAL_24BIT);

                bFlag = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            if(!GET_MHL_CABLE_PLUGGED1())
            {
                SET_MHL_CABLE_PLUGGED1();

                SET_MHL_PHY_CLOCK_MODE_PORT1(MHL_PHY_CLOCK_NORMAL_24BIT);

                bFlag = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            if(!GET_MHL_CABLE_PLUGGED2())
            {
                SET_MHL_CABLE_PLUGGED2();

                SET_MHL_PHY_CLOCK_MODE_PORT2(MHL_PHY_CLOCK_NORMAL_24BIT);
                 bFlag = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            if(!GET_MHL_CABLE_PLUGGED3())
            {
                SET_MHL_CABLE_PLUGGED3();

                SET_MHL_PHY_CLOCK_MODE_PORT3(MHL_PHY_CLOCK_NORMAL_24BIT);

                bFlag = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            if(!GET_MHL_CABLE_PLUGGED4())
            {
                SET_MHL_CABLE_PLUGGED4();

                SET_MHL_PHY_CLOCK_MODE_PORT4(MHL_PHY_CLOCK_NORMAL_24BIT);

                bFlag = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            if(!GET_MHL_CABLE_PLUGGED5())
            {
                SET_MHL_CABLE_PLUGGED5();

                SET_MHL_PHY_CLOCK_MODE_PORT5(MHL_PHY_CLOCK_NORMAL_24BIT);

                bFlag = TRUE;
            }

            break;
#endif

        default:

            break;
    };

    if(bFlag)
    {
        mhal_mhl_CablePlugProc(ucPortSelect);

        MHL_DRV_DPRINTF("** MHL cable plugged!! port %d", ucPortSelect);
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CableRemoveProc()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_CableRemoveProc(BYTE ucPortSelect)
{
    Bool bFlag = FALSE;

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            if(GET_MHL_CABLE_PLUGGED0())
            {
                CLR_MHL_CABLE_PLUGGED0();

                bFlag = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            if(GET_MHL_CABLE_PLUGGED1())
            {
                CLR_MHL_CABLE_PLUGGED1();

                bFlag = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            if(GET_MHL_CABLE_PLUGGED2())
            {
                CLR_MHL_CABLE_PLUGGED2();

                bFlag = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            if(GET_MHL_CABLE_PLUGGED3())
            {
                CLR_MHL_CABLE_PLUGGED3();

                bFlag = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            if(GET_MHL_CABLE_PLUGGED4())
            {
                CLR_MHL_CABLE_PLUGGED4();

                bFlag = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            if(GET_MHL_CABLE_PLUGGED5())
            {
                CLR_MHL_CABLE_PLUGGED5();

                bFlag = TRUE;
            }

            break;
#endif

        default:

            break;
    };

    if(bFlag)
    {
        mhal_mhl_CableRemoveProc(ucPortSelect);
        // Clear plug flag
        _mdrv_mhl_ClearCbusFlag(ucPortSelect, MHL_CLEAR_CABLE_LOSS_FLAG);
        // Stop timer
        _mdrv_mhl_InsertCbusTimer(ucPortSelect, MHL_CBUS_IDEL_FREE_TIME);

        MHL_DRV_DPRINTF("** MHL cable remove~~~ port %d", ucPortSelect);
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusConnectProc()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_CbusConnectProc(BYTE ucPortSelect)
{
    Bool bFlag = FALSE;

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            if(!GET_MHL_CBUS_CONNECT0())
            {
                SET_MHL_CBUS_CONNECT0();

#if(MHL_READ_DEVICE_VENDER_ID)
                SET_MHL_GET_VENDER_ID_ENABLE0();
#endif

                if(GET_MHL_SYSTEM_FORCE_SEND_MSG())
                {
                    SET_MHL_SEND_RCP_ENABLE_PORT0();
                    SET_MHL_SEND_RAP_ENABLE_PORT0();
                    SET_MHL_SEND_UCP_ENABLE_PORT0();
                }

                bFlag = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            if(!GET_MHL_CBUS_CONNECT1())
            {
                SET_MHL_CBUS_CONNECT1();

#if(MHL_READ_DEVICE_VENDER_ID)
                SET_MHL_GET_VENDER_ID_ENABLE1();
#endif

                if(GET_MHL_SYSTEM_FORCE_SEND_MSG())
                {
                    SET_MHL_SEND_RCP_ENABLE_PORT1();
                    SET_MHL_SEND_RAP_ENABLE_PORT1();
                    SET_MHL_SEND_UCP_ENABLE_PORT1();
                }

                bFlag = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            if(!GET_MHL_CBUS_CONNECT2())
            {
                SET_MHL_CBUS_CONNECT2();

#if(MHL_READ_DEVICE_VENDER_ID)
                SET_MHL_GET_VENDER_ID_ENABLE2();
#endif

                if(GET_MHL_SYSTEM_FORCE_SEND_MSG())
                {
                    SET_MHL_SEND_RCP_ENABLE_PORT2();
                    SET_MHL_SEND_RAP_ENABLE_PORT2();
                    SET_MHL_SEND_UCP_ENABLE_PORT2();
                }

                bFlag = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            if(!GET_MHL_CBUS_CONNECT3())
            {
                SET_MHL_CBUS_CONNECT3();

#if(MHL_READ_DEVICE_VENDER_ID)
                SET_MHL_GET_VENDER_ID_ENABLE3();
#endif

                if(GET_MHL_SYSTEM_FORCE_SEND_MSG())
                {
                    SET_MHL_SEND_RCP_ENABLE_PORT3();
                    SET_MHL_SEND_RAP_ENABLE_PORT3();
                    SET_MHL_SEND_UCP_ENABLE_PORT3();
                }

                bFlag = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            if(!GET_MHL_CBUS_CONNECT4())
            {
                SET_MHL_CBUS_CONNECT4();

#if(MHL_READ_DEVICE_VENDER_ID)
                SET_MHL_GET_VENDER_ID_ENABLE4();
#endif

                if(GET_MHL_SYSTEM_FORCE_SEND_MSG())
                {
                    SET_MHL_SEND_RCP_ENABLE_PORT4();
                    SET_MHL_SEND_RAP_ENABLE_PORT4();
                    SET_MHL_SEND_UCP_ENABLE_PORT4();
                }

                bFlag = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            if(!GET_MHL_CBUS_CONNECT5())
            {
                SET_MHL_CBUS_CONNECT5();

#if(MHL_READ_DEVICE_VENDER_ID)
                SET_MHL_GET_VENDER_ID_ENABLE5();
#endif

                if(GET_MHL_SYSTEM_FORCE_SEND_MSG())
                {
                    SET_MHL_SEND_RCP_ENABLE_PORT5();
                    SET_MHL_SEND_RAP_ENABLE_PORT5();
                    SET_MHL_SEND_UCP_ENABLE_PORT5();
                }

                bFlag = TRUE;
            }

            break;
#endif

        default:

            break;
    };

    if(bFlag)
    {
        mhal_mhl_CbusConnectProc(ucPortSelect);
        // Set timer 150ms
        _mdrv_mhl_InsertCbusTimer(ucPortSelect, MHL_CBUS_CONNECT_HOLD_TIME);

#if(MHL_CBUS_SELF_TEST_SUPPORT)
        _mdrv_mhl_SetCbusSelfTestTimer(MHL_CBUS_SELT_TEST_PORT, MHL_CBUS_SELF_TEST_START_TIME);
#endif

        MHL_DRV_DPRINTF("** MHL Cbus connected~~ ^Q^ port %d", ucPortSelect);
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusStucktoLowProc()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_CbusStucktoLowProc(BYTE ucPortSelect)
{
    Bool bFlag = FALSE;

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            if(GET_MHL_CBUS_CONNECT0())
            {
                CLR_MHL_CBUS_CONNECT0();

                bFlag = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            if(GET_MHL_CBUS_CONNECT1())
            {
                CLR_MHL_CBUS_CONNECT1();

                bFlag = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            if(GET_MHL_CBUS_CONNECT2())
            {
                CLR_MHL_CBUS_CONNECT2();

                bFlag = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            if(GET_MHL_CBUS_CONNECT3())
            {
                CLR_MHL_CBUS_CONNECT3();

                bFlag = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            if(GET_MHL_CBUS_CONNECT4())
            {
                CLR_MHL_CBUS_CONNECT4();

                bFlag = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            if(GET_MHL_CBUS_CONNECT5())
            {
                CLR_MHL_CBUS_CONNECT5();

                bFlag = TRUE;
            }

            break;
#endif

        default:

            break;
    };

    if(bFlag)
    {
        mhal_mhl_CbusStucktoLowProc(ucPortSelect);
        // Clear connect flag
        _mdrv_mhl_ClearCbusFlag(ucPortSelect, MHL_CLEAR_CONNECT_LOSS_FLAG);
        // Switch to 24 bit mode
        _mdrv_mhl_ClockModeSwitchProc(ucPortSelect, MHL_PHY_CLOCK_NORMAL_24BIT);
        // Stop timer
        _mdrv_mhl_InsertCbusTimer(ucPortSelect, MHL_CBUS_IDEL_FREE_TIME);

        MHL_DRV_DPRINTF("** MHL Cbus stuck to low @@ port %d", ucPortSelect);
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_GetCbusConnectFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool _mdrv_mhl_GetCbusConnectFlag(BYTE ucPortSelect)
{
    Bool bFlag = FALSE;

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            bFlag = (GET_MHL_FUNCTION_ENABLE_PORT0() & GET_MHL_CBUS_CONNECT0());
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            bFlag = (GET_MHL_FUNCTION_ENABLE_PORT1() & GET_MHL_CBUS_CONNECT1());
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            bFlag = (GET_MHL_FUNCTION_ENABLE_PORT2() & GET_MHL_CBUS_CONNECT2());
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            bFlag = (GET_MHL_FUNCTION_ENABLE_PORT3() & GET_MHL_CBUS_CONNECT3());
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            bFlag = (GET_MHL_FUNCTION_ENABLE_PORT4() & GET_MHL_CBUS_CONNECT4());
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            bFlag = (GET_MHL_FUNCTION_ENABLE_PORT5() & GET_MHL_CBUS_CONNECT5());
            break;
#endif

        default:

            break;
    };

    return bFlag;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_GetCbusEnhanceFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool _mdrv_mhl_GetCbusEnhanceFlag(BYTE ucPortSelect)
{
    Bool bFlag = FALSE;

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            bFlag = GET_MHL_ENABLE_CBUS_ENHANCE0();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            bFlag = GET_MHL_ENABLE_CBUS_ENHANCE1();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            bFlag = GET_MHL_ENABLE_CBUS_ENHANCE2();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            bFlag = GET_MHL_ENABLE_CBUS_ENHANCE3();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            bFlag = GET_MHL_ENABLE_CBUS_ENHANCE4();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            bFlag = GET_MHL_ENABLE_CBUS_ENHANCE5();
            break;
#endif

        default:

            break;
    };

    return bFlag;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_GetCbusSelfTestFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool _mdrv_mhl_GetCbusSelfTestFlag(BYTE ucPortSelect)
{
    Bool bFlag = FALSE;

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            bFlag = GET_MHL_CBUS_SELF_TEST_MODE0();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            bFlag = GET_MHL_CBUS_SELF_TEST_MODE1();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            bFlag = GET_MHL_CBUS_SELF_TEST_MODE2();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            bFlag = GET_MHL_CBUS_SELF_TEST_MODE3();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            bFlag = GET_MHL_CBUS_SELF_TEST_MODE4();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            bFlag = GET_MHL_CBUS_SELF_TEST_MODE5();
            break;
#endif

        default:

            break;
    };

    return bFlag;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_GetDisplayReadyFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool _mdrv_mhl_GetDisplayReadyFlag(BYTE ucPortSelect)
{
    Bool bFlag = FALSE;

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            bFlag = GET_MHL_CBUS_DISPLAY_READY0();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            bFlag = GET_MHL_CBUS_DISPLAY_READY1();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            bFlag = GET_MHL_CBUS_DISPLAY_READY2();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            bFlag = GET_MHL_CBUS_DISPLAY_READY3();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            bFlag = GET_MHL_CBUS_DISPLAY_READY4();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            bFlag = GET_MHL_CBUS_DISPLAY_READY5();
            break;
#endif

        default:

            break;
    };

    return bFlag;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusReceiveISRProc()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_CbusReceiveISRProc(BYTE ucPortSelect)
{
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            INC_MHL_MSC_RECEIVE_COUNTER0();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            INC_MHL_MSC_RECEIVE_COUNTER1();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            INC_MHL_MSC_RECEIVE_COUNTER2();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            INC_MHL_MSC_RECEIVE_COUNTER3();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            INC_MHL_MSC_RECEIVE_COUNTER4();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            INC_MHL_MSC_RECEIVE_COUNTER5();
            break;
#endif

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusReceiveEMSCISRProc()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_CbusReceiveEMSCISRProc(BYTE ucPortSelect)
{
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            INC_MHL_EMSC_RECEIVE_COUNTER0();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            INC_MHL_EMSC_RECEIVE_COUNTER1();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            INC_MHL_EMSC_RECEIVE_COUNTER2();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            INC_MHL_EMSC_RECEIVE_COUNTER3();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            INC_MHL_EMSC_RECEIVE_COUNTER4();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            INC_MHL_EMSC_RECEIVE_COUNTER5();
            break;
#endif

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusGetLeastFlag()
//  [Description]:
//                  MHL Cbus get the least True flag in index
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE _mdrv_mhl_CbusGetLeastFlag(BYTE unindex)
{
    BYTE uctemp = BIT0;

    if(unindex == 0)
    {
        return unindex;
    }

    while(!(unindex &uctemp))
    {
        uctemp = uctemp <<1;
    }

    return uctemp;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_GetLeastBitNum()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE _mdrv_mhl_GetLeastBitNum(WORD usData)
{
    BYTE uctemp = 0;

    while(!(usData &BIT0))
    {
        usData = usData >>1;
        uctemp++;
    }

    return uctemp;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_WriteBurstCheckSum()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE _mdrv_mhl_WriteBurstCheckSum(BYTE *ucData)
{
    BYTE uctemp = 0;
    BYTE ucCheckSum = 0;

    for(uctemp = 0; uctemp <MHL_MSC_SCRATCHPAD_SIZE; uctemp++)
    {
        ucCheckSum = ucCheckSum +ucData[uctemp];
    }

    ucCheckSum = (ucCheckSum ^0xFF) +1;

    return ucCheckSum;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusSetPacket()
//  [Description]
//                  MHL Cbus Packet setting
//  [Arguments]:
//                  ucHeader: Packet header type
//                  bControl: Packet control bit type
//                  ucContent: Packet payload
//  [Return]:
//                  Packet word for register write
//**************************************************************************
WORD _mdrv_mhl_CbusSetPacket(BYTE ucHeader, Bool bControl, BYTE ucContent)
{
    Bool bParity = 0;
    WORD usPacket = (ucHeader <<9) |(bControl <<8) |ucContent;

    do
    {
        bParity ^= (usPacket & BIT0);
        usPacket = usPacket >>1;
    }
    while(usPacket);

    usPacket = (bParity <<11) |(ucHeader <<9) |(bControl <<8) |ucContent;

    return (usPacket |BIT15);
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_GetCbusQueueIndex()
//  [Description]:
//                  MHL Cbus get transmitting queue index
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE _mdrv_mhl_GetCbusQueueIndex(BYTE ucPortSelect, BYTE ucQueueIndex)
{
    BYTE uctemp = 0;

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            // Check Rx queue have packet transmit success or not
            if(GET_MHL_QUEUE_STATE_PORT0(MHL_CBUS_RX_QUEUE_INDEX) != CBUS_REQ_IDLE)
            {
                return MHL_CBUS_RX_QUEUE_INDEX;
            }

            // Check Tx queue have packet transmit success or not, search start  from current index
            for(uctemp = 0; uctemp <(MHL_CBUS_TOTAL_QUEUE_NUM -1); uctemp++)
            {
                if(GET_MHL_QUEUE_STATE_PORT0((ucQueueIndex +uctemp) %(MHL_CBUS_TOTAL_QUEUE_NUM -1)) != CBUS_REQ_IDLE)
                {
                    return ((ucQueueIndex +uctemp) %(MHL_CBUS_TOTAL_QUEUE_NUM -1));
                }
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            // Check Rx queue have packet transmit success or not
            if(GET_MHL_QUEUE_STATE_PORT1(MHL_CBUS_RX_QUEUE_INDEX) != CBUS_REQ_IDLE)
            {
                return MHL_CBUS_RX_QUEUE_INDEX;
            }

            // Check Tx queue have packet transmit success or not, search start  from current index
            for(uctemp = 0; uctemp <(MHL_CBUS_TOTAL_QUEUE_NUM -1); uctemp++)
            {
                if(GET_MHL_QUEUE_STATE_PORT1((ucQueueIndex +uctemp) %(MHL_CBUS_TOTAL_QUEUE_NUM -1)) != CBUS_REQ_IDLE)
                {
                    return ((ucQueueIndex +uctemp) %(MHL_CBUS_TOTAL_QUEUE_NUM -1));
                }
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            // Check Rx queue have packet transmit success or not
            if(GET_MHL_QUEUE_STATE_PORT2(MHL_CBUS_RX_QUEUE_INDEX) != CBUS_REQ_IDLE)
            {
                return MHL_CBUS_RX_QUEUE_INDEX;
            }

            // Check Tx queue have packet transmit success or not, search start  from current index
            for(uctemp = 0; uctemp <(MHL_CBUS_TOTAL_QUEUE_NUM -1); uctemp++)
            {
                if(GET_MHL_QUEUE_STATE_PORT2((ucQueueIndex +uctemp) %(MHL_CBUS_TOTAL_QUEUE_NUM -1)) != CBUS_REQ_IDLE)
                {
                    return ((ucQueueIndex +uctemp) %(MHL_CBUS_TOTAL_QUEUE_NUM -1));
                }
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            // Check Rx queue have packet transmit success or not
            if(GET_MHL_QUEUE_STATE_PORT3(MHL_CBUS_RX_QUEUE_INDEX) != CBUS_REQ_IDLE)
            {
                return MHL_CBUS_RX_QUEUE_INDEX;
            }

            // Check Tx queue have packet transmit success or not, search start  from current index
            for(uctemp = 0; uctemp <(MHL_CBUS_TOTAL_QUEUE_NUM -1); uctemp++)
            {
                if(GET_MHL_QUEUE_STATE_PORT3((ucQueueIndex +uctemp) %(MHL_CBUS_TOTAL_QUEUE_NUM -1)) != CBUS_REQ_IDLE)
                {
                    return ((ucQueueIndex +uctemp) %(MHL_CBUS_TOTAL_QUEUE_NUM -1));
                }
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            // Check Rx queue have packet transmit success or not
            if(GET_MHL_QUEUE_STATE_PORT4(MHL_CBUS_RX_QUEUE_INDEX) != CBUS_REQ_IDLE)
            {
                return MHL_CBUS_RX_QUEUE_INDEX;
            }

            // Check Tx queue have packet transmit success or not, search start  from current index
            for(uctemp = 0; uctemp <(MHL_CBUS_TOTAL_QUEUE_NUM -1); uctemp++)
            {
                if(GET_MHL_QUEUE_STATE_PORT4((ucQueueIndex +uctemp) %(MHL_CBUS_TOTAL_QUEUE_NUM -1)) != CBUS_REQ_IDLE)
                {
                    return ((ucQueueIndex +uctemp) %(MHL_CBUS_TOTAL_QUEUE_NUM -1));
                }
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            // Check Rx queue have packet transmit success or not
            if(GET_MHL_QUEUE_STATE_PORT5(MHL_CBUS_RX_QUEUE_INDEX) != CBUS_REQ_IDLE)
            {
                return MHL_CBUS_RX_QUEUE_INDEX;
            }

            // Check Tx queue have packet transmit success or not, search start  from current index
            for(uctemp = 0; uctemp <(MHL_CBUS_TOTAL_QUEUE_NUM -1); uctemp++)
            {
                if(GET_MHL_QUEUE_STATE_PORT5((ucQueueIndex +uctemp) %(MHL_CBUS_TOTAL_QUEUE_NUM -1)) != CBUS_REQ_IDLE)
                {
                    return ((ucQueueIndex +uctemp) %(MHL_CBUS_TOTAL_QUEUE_NUM -1));
                }
            }

            break;
#endif

        default:

            break;
    };

    return MHL_CBUS_TOTAL_QUEUE_NUM;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusTxWriteCommand()
//  [Description]:
//                  MHL Cbus send command
//  [Arguments]:
//
//  [Return]:
//                  TRUE: Cbus command send successfully
//                  FALSE: Cbus command send fail
//**************************************************************************
Bool _mdrv_mhl_CbusTxWriteCommand(BYTE ucPortSelect, CbusReq_S *pReq)
{
    Bool bFlag = TRUE;
    BYTE uctemp = 0;
    mhalCbusFifo_S pCbusFifo;

    // payload 1
    pCbusFifo.databuf[0] = _mdrv_mhl_CbusSetPacket(MSC_HEADER, CBUS_CONTROL, pReq->cmd);

    switch(pReq->cmd)
    {
        case MSC_ACK:
            pCbusFifo.lens = pReq->len;

            if(pReq->len == 2)
            {
                // payload 2
                pCbusFifo.databuf[1] = _mdrv_mhl_CbusSetPacket(MSC_HEADER, CBUS_DATA, pReq->msgData[0]);
            }
            break;

        case MSC_NACK:
            pCbusFifo.lens = 1;
            break;

        case MSC_ABORT:
            pCbusFifo.lens = 1;
            break;

        case MSC_WRITE_STAT_OR_SET_INT: // 0x60
        case MSC_WRITE_XSTAT: // 0x70
            // payload 2
            pCbusFifo.databuf[1] = _mdrv_mhl_CbusSetPacket(MSC_HEADER, CBUS_DATA, pReq->msgData[0]);
            // payload 3
            pCbusFifo.databuf[2] = _mdrv_mhl_CbusSetPacket(MSC_HEADER, CBUS_DATA, pReq->msgData[1]);
            // message length
            pCbusFifo.lens = 3;
            break;

        case MSC_READ_DEVCAP: // 0x61
        case MSC_READ_XDEVCAP: // 0x71
            // payload 2
            pCbusFifo.databuf[1] = _mdrv_mhl_CbusSetPacket(MSC_HEADER, CBUS_DATA, pReq->msgData[0]);
            // message length
            pCbusFifo.lens = 2;
            break;

        case MSC_GET_STATE: // 0x62
        case MSC_GET_VENDOR_ID: // 0x63
        case MSC_SET_HPD: // 0x64
        case MSC_CLR_HPD: // 0x65
        case MSC_GET_SC1_ERRORCODE: // 0x69 - Get channel 1 command error code
        case MSC_GET_DDC_ERRORCODE: // 0x6A - Get DDC channel command error code.
        case MSC_GET_MSC_ERRORCODE: // 0x6B - Get MSC command error code.
        case MSC_GET_SC3_ERRORCODE: // 0x6D - Get channel 3 command error code.
            // message length
            pCbusFifo.lens = 1;
            break;

        case MSC_MSC_MSG: // 0x68
            // payload 2
            pCbusFifo.databuf[1] = _mdrv_mhl_CbusSetPacket(MSC_HEADER, CBUS_DATA, pReq->msgData[0]);
            // payload 3
            pCbusFifo.databuf[2] = _mdrv_mhl_CbusSetPacket(MSC_HEADER, CBUS_DATA, pReq->msgData[1]);
            // message length
            pCbusFifo.lens = 3;
            break;

        case MSC_WRITE_BURST: // 0x6C
            // payload 2 ~ 17
            for(uctemp =0; uctemp <pReq->len; uctemp++) // offset packet + the maxi 16 packets
            {
                pCbusFifo.databuf[uctemp +1] = _mdrv_mhl_CbusSetPacket(MSC_HEADER, CBUS_DATA, pReq->msgData[uctemp]);
            }
            // EOF
            pCbusFifo.databuf[pReq->len +1] = _mdrv_mhl_CbusSetPacket(MSC_HEADER, CBUS_CONTROL, MSC_EOF);
            // message length
            pCbusFifo.lens = pReq->len +2;
            break;

        default:
            bFlag = FALSE;
            break;
    }

    if(bFlag)
    {
        bFlag = mhal_mhl_CbusWriteCommand(ucPortSelect, &pCbusFifo);
    }

    return bFlag;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusWriteStatusOrSetInt()
//  [Description]:
//                  MHL Cbus write status or set interrupt
//  [Arguments]:
//
//  [Return]:
//                  TRUE: Cbus message send successfully
//                  FALSE: Cbus message send fail
//**************************************************************************
Bool _mdrv_mhl_CbusWriteStatusOrSetInt(BYTE ucPortSelect, BYTE ucAddr, BYTE ucValue, Bool bExtended)
{
    CbusReq_S stQueue;

    if(bExtended)
    {
        stQueue.cmd = MSC_WRITE_XSTAT;
    }
    else
    {
        stQueue.cmd = MSC_WRITE_STAT_OR_SET_INT;
    }
    
    stQueue.msgData[0] = ucAddr;
    stQueue.msgData[1] = ucValue;
    stQueue.len = 2;
    stQueue.reqStatus = CBUS_REQ_PENDING;

    return _mdrv_mhl_CbusTxQueueInsertMSC(ucPortSelect, &stQueue);
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusSendSetHPD()
//  [Description]:
//                  MHL Cbus set HPD state
//  [Arguments]:
//                  TRUE: SET_HPD
//                  FALSE: CLR_HPD
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_CbusSendSetHPD(BYTE ucPortSelect, Bool bSetHPD)
{
    Bool bFlag = FALSE;
    CbusReq_S stQueue;

    stQueue.cmd = (bSetHPD? MSC_SET_HPD : MSC_CLR_HPD);
    stQueue.len = 0;
    stQueue.reqStatus = CBUS_REQ_PENDING;

    bFlag = bSetHPD &_mdrv_mhl_CbusTxQueueInsertMSC(ucPortSelect, &stQueue);

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            if(bFlag)
            {
                SET_MHL_SET_HPD_FLAG0();
            }
            else
            {
                CLR_MHL_SET_HPD_FLAG0();

                if(GET_MHL_LINK_STATUS_NORMAL0())
                {
                    CLR_MHL_LINK_STATUS_NORMAL0();
                }
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            if(bFlag)
            {
                SET_MHL_SET_HPD_FLAG1();
            }
            else
            {
                CLR_MHL_SET_HPD_FLAG1();

                if(GET_MHL_LINK_STATUS_NORMAL1())
                {
                    CLR_MHL_LINK_STATUS_NORMAL1();
                }
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            if(bFlag)
            {
                SET_MHL_SET_HPD_FLAG2();
            }
            else
            {
                CLR_MHL_SET_HPD_FLAG2();

                if(GET_MHL_LINK_STATUS_NORMAL2())
                {
                    CLR_MHL_LINK_STATUS_NORMAL2();
                }
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            if(bFlag)
            {
                SET_MHL_SET_HPD_FLAG3();
            }
            else
            {
                CLR_MHL_SET_HPD_FLAG3();

                if(GET_MHL_LINK_STATUS_NORMAL3())
                {
                    CLR_MHL_LINK_STATUS_NORMAL3();
                }
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            if(bFlag)
            {
                SET_MHL_SET_HPD_FLAG4();
            }
            else
            {
                CLR_MHL_SET_HPD_FLAG4();

                if(GET_MHL_LINK_STATUS_NORMAL4())
                {
                    CLR_MHL_LINK_STATUS_NORMAL4();
                }
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            if(bFlag)
            {
                SET_MHL_SET_HPD_FLAG5();
            }
            else
            {
                CLR_MHL_SET_HPD_FLAG5();

                if(GET_MHL_LINK_STATUS_NORMAL5())
                {
                    CLR_MHL_LINK_STATUS_NORMAL5();
                }
            }

            break;
#endif

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusSendPathEnable()
//  [Description]:
//                  MHL Cbus set path enable
//  [Arguments]:
//                  TRUE: Enable
//                  FALSE: Disable
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_CbusSendPathEnable(BYTE ucPortSelect)
{
    Bool bFlag = _mdrv_mhl_CbusWriteStatusOrSetInt(ucPortSelect, 0x31, BIT3, FALSE);

    if(bFlag)
    {
        mhal_mhl_CbusSetPathEnable(ucPortSelect, bFlag);
    }

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            if(bFlag)
            {
                SET_MHL_PATH_EN_FLAG0();
            }
            else
            {
                CLR_MHL_PATH_EN_FLAG0();
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            if(bFlag)
            {
                SET_MHL_PATH_EN_FLAG1();
            }
            else
            {
                CLR_MHL_PATH_EN_FLAG1();
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            if(bFlag)
            {
                SET_MHL_PATH_EN_FLAG2();
            }
            else
            {
                CLR_MHL_PATH_EN_FLAG2();
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            if(bFlag)
            {
                SET_MHL_PATH_EN_FLAG3();
            }
            else
            {
                CLR_MHL_PATH_EN_FLAG3();
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            if(bFlag)
            {
                SET_MHL_PATH_EN_FLAG4();
            }
            else
            {
                CLR_MHL_PATH_EN_FLAG4();
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            if(bFlag)
            {
                SET_MHL_PATH_EN_FLAG5();
            }
            else
            {
                CLR_MHL_PATH_EN_FLAG5();
            }

            break;
#endif

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_GetDevcapReadyValue()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE _mdrv_mhl_GetDevcapReadyValue(BYTE ucPortSelect)
{
    BYTE ucDevcapValue = BIT0;

    if(_mdrv_mhl_GetECbusSupportFlag(ucPortSelect))
    {
        ucDevcapValue = ucDevcapValue| BIT1;
    }

    return ucDevcapValue;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusSendDevcapReady()
//  [Description]:
//                  MHL Cbus set capbility ready
//  [Arguments]:
//                  TRUE: Enable
//                  FALSE: Disable
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_CbusSendDevcapReady(BYTE ucPortSelect)
{
    Bool bFlag = _mdrv_mhl_CbusWriteStatusOrSetInt(ucPortSelect, 0x30, _mdrv_mhl_GetDevcapReadyValue(ucPortSelect), FALSE);

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            if(bFlag)
            {
                SET_MHL_DEVCAP_READY_FLAG0();
            }
            else
            {
                CLR_MHL_DEVCAP_READY_FLAG0();
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            if(bFlag)
            {
                SET_MHL_DEVCAP_READY_FLAG1();
            }
            else
            {
                CLR_MHL_DEVCAP_READY_FLAG1();
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            if(bFlag)
            {
                SET_MHL_DEVCAP_READY_FLAG2();
            }
            else
            {
                CLR_MHL_DEVCAP_READY_FLAG2();
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            if(bFlag)
            {
                SET_MHL_DEVCAP_READY_FLAG3();
            }
            else
            {
                CLR_MHL_DEVCAP_READY_FLAG3();
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            if(bFlag)
            {
                SET_MHL_DEVCAP_READY_FLAG4();
            }
            else
            {
                CLR_MHL_DEVCAP_READY_FLAG4();
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            if(bFlag)
            {
                SET_MHL_DEVCAP_READY_FLAG5();
            }
            else
            {
                CLR_MHL_DEVCAP_READY_FLAG5();
            }

            break;
#endif

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusSendDevcapChange()
//  [Description]:
//                  MHL Cbus set capbility change
//  [Arguments]:
//                  TRUE: Enable
//                  FALSE: Disable
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_CbusSendDevcapChange(BYTE ucPortSelect)
{
    Bool bFlag = _mdrv_mhl_CbusWriteStatusOrSetInt(ucPortSelect, 0x20, BIT0, FALSE);

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            if(bFlag)
            {
                SET_MHL_DEVCAP_CHANGE_FLAG0();
            }
            else
            {
                CLR_MHL_DEVCAP_CHANGE_FLAG0();
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            if(bFlag)
            {
                SET_MHL_DEVCAP_CHANGE_FLAG1();
            }
            else
            {
                CLR_MHL_DEVCAP_CHANGE_FLAG1();
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            if(bFlag)
            {
                SET_MHL_DEVCAP_CHANGE_FLAG2();
            }
            else
            {
                CLR_MHL_DEVCAP_CHANGE_FLAG2();
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            if(bFlag)
            {
                SET_MHL_DEVCAP_CHANGE_FLAG3();
            }
            else
            {
                CLR_MHL_DEVCAP_CHANGE_FLAG3();
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            if(bFlag)
            {
                SET_MHL_DEVCAP_CHANGE_FLAG4();
            }
            else
            {
                CLR_MHL_DEVCAP_CHANGE_FLAG4();
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            if(bFlag)
            {
                SET_MHL_DEVCAP_CHANGE_FLAG5();
            }
            else
            {
                CLR_MHL_DEVCAP_CHANGE_FLAG5();
            }

            break;
#endif

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusSendVersionCode()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_CbusSendVersionCode(BYTE ucPortSelect)
{
    Bool bFlag = FALSE;
    BYTE ucVersionCode = tMHL_DEVCAP_TABLE_PORT[ucPortSelect][MHL_CBUS_VERSION];

    bFlag = _mdrv_mhl_CbusWriteStatusOrSetInt(ucPortSelect, 0x32, ucVersionCode, FALSE);
    
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            if(bFlag)
            {
                SET_MHL_VERSION_CODE_FLAG0();
            }
            else
            {
                CLR_MHL_VERSION_CODE_FLAG0();
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            if(bFlag)
            {
                SET_MHL_VERSION_CODE_FLAG1();
            }
            else
            {
                CLR_MHL_VERSION_CODE_FLAG1();
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            if(bFlag)
            {
                SET_MHL_VERSION_CODE_FLAG2();
            }
            else
            {
                CLR_MHL_VERSION_CODE_FLAG2();
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            if(bFlag)
            {
                SET_MHL_VERSION_CODE_FLAG3();
            }
            else
            {
                CLR_MHL_VERSION_CODE_FLAG3();
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            if(bFlag)
            {
                SET_MHL_VERSION_CODE_FLAG4();
            }
            else
            {
                CLR_MHL_VERSION_CODE_FLAG4();
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            if(bFlag)
            {
                SET_MHL_VERSION_CODE_FLAG5();
            }
            else
            {
                CLR_MHL_VERSION_CODE_FLAG5();
            }

            break;
#endif

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusSendLinkStatusNormal()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_CbusSendLinkStatusNormal(BYTE ucPortSelect)
{
    Bool bFlag = FALSE;

    if(_mdrv_mhl_GetECbusState(ucPortSelect) == MHL_ECBUS_STATE_STABLE_POLLING)
    {
        bFlag = _mdrv_mhl_CbusWriteStatusOrSetInt(ucPortSelect, 0x91, 0x02, TRUE);
    }
    
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            if(bFlag)
            {
                SET_MHL_LINK_STATUS_NORMAL0();
            }
            else
            {
                CLR_MHL_LINK_STATUS_NORMAL0();
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            if(bFlag)
            {
                SET_MHL_LINK_STATUS_NORMAL1();
            }
            else
            {
                CLR_MHL_LINK_STATUS_NORMAL1();
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            if(bFlag)
            {
                SET_MHL_LINK_STATUS_NORMAL2();
            }
            else
            {
                CLR_MHL_LINK_STATUS_NORMAL2();
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            if(bFlag)
            {
                SET_MHL_LINK_STATUS_NORMAL3();
            }
            else
            {
                CLR_MHL_LINK_STATUS_NORMAL3();
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            if(bFlag)
            {
                SET_MHL_LINK_STATUS_NORMAL4();
            }
            else
            {
                CLR_MHL_LINK_STATUS_NORMAL4();
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            if(bFlag)
            {
                SET_MHL_LINK_STATUS_NORMAL5();
            }
            else
            {
                CLR_MHL_LINK_STATUS_NORMAL5();
            }

            break;
#endif

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_TxQueueHandler()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_TxQueueHandler(BYTE ucPortSelect)
{
    Bool bSendCommand = FALSE;
    Bool bRetrySendCommand = FALSE;
    BYTE ucQueueIndex = 0;

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            if(!GET_MHL_CBUS_SILENCE0())
            {
                ucQueueIndex = _mdrv_mhl_GetCbusQueueIndex(ucPortSelect, GET_MHL_TX_QUEUE_INDEX_PORT0());

                if(ucQueueIndex < MHL_CBUS_TOTAL_QUEUE_NUM)
                {
                    if(_mdrv_mhl_CbusTxWriteCommand(ucPortSelect, &(GET_MHL_CBUS_QUEUE_PORT0(ucQueueIndex))))
                    {
                        // Retry send packet twice
                        if(GET_MHL_QUEUE_STATE_PORT0(ucQueueIndex) == CBUS_REQ_SENT)
                        {
                            SET_MHL_QUEUE_STATE_PORT0(ucQueueIndex, CBUS_REQ_IDLE);

                            bRetrySendCommand = TRUE;
                        }
                        else if(GET_MHL_QUEUE_STATE_PORT0(ucQueueIndex) > CBUS_REQ_SENT)
                        {
                            SET_MHL_QUEUE_STATE_PORT0(ucQueueIndex, CBUS_REQ_SENT);
                        }

                        // Update current index to search index, but not Rx queue index
                        if(ucQueueIndex < MHL_CBUS_RX_QUEUE_INDEX)
                        {
                            SET_MHL_TX_QUEUE_INDEX_PORT0(ucQueueIndex);
                        }

                        bSendCommand = TRUE;
                    }
                }
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            if(!GET_MHL_CBUS_SILENCE1())
            {
                ucQueueIndex = _mdrv_mhl_GetCbusQueueIndex(ucPortSelect, GET_MHL_TX_QUEUE_INDEX_PORT1());

                if(ucQueueIndex < MHL_CBUS_TOTAL_QUEUE_NUM)
                {
                    if(_mdrv_mhl_CbusTxWriteCommand(ucPortSelect, &(GET_MHL_CBUS_QUEUE_PORT1(ucQueueIndex))))
                    {
                        // Retry send packet twice
                        if(GET_MHL_QUEUE_STATE_PORT1(ucQueueIndex) == CBUS_REQ_SENT)
                        {
                            SET_MHL_QUEUE_STATE_PORT1(ucQueueIndex, CBUS_REQ_IDLE);

                            bRetrySendCommand = TRUE;
                        }
                        else if(GET_MHL_QUEUE_STATE_PORT1(ucQueueIndex) > CBUS_REQ_SENT)
                        {
                            SET_MHL_QUEUE_STATE_PORT1(ucQueueIndex, CBUS_REQ_SENT);
                        }

                        // Update current index to search index, but not Rx queue index
                        if(ucQueueIndex < MHL_CBUS_RX_QUEUE_INDEX)
                        {
                            SET_MHL_TX_QUEUE_INDEX_PORT1(ucQueueIndex);
                        }

                        bSendCommand = TRUE;
                    }
                }
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            if(!GET_MHL_CBUS_SILENCE2())
            {
                ucQueueIndex = _mdrv_mhl_GetCbusQueueIndex(ucPortSelect, GET_MHL_TX_QUEUE_INDEX_PORT2());

                if(ucQueueIndex < MHL_CBUS_TOTAL_QUEUE_NUM)
                {
                    if(_mdrv_mhl_CbusTxWriteCommand(ucPortSelect, &(GET_MHL_CBUS_QUEUE_PORT2(ucQueueIndex))))
                    {
                        // Retry send packet twice
                        if(GET_MHL_QUEUE_STATE_PORT2(ucQueueIndex) == CBUS_REQ_SENT)
                        {
                            SET_MHL_QUEUE_STATE_PORT2(ucQueueIndex, CBUS_REQ_IDLE);

                            bRetrySendCommand = TRUE;
                        }
                        else if(GET_MHL_QUEUE_STATE_PORT2(ucQueueIndex) > CBUS_REQ_SENT)
                        {
                            SET_MHL_QUEUE_STATE_PORT2(ucQueueIndex, CBUS_REQ_SENT);
                        }

                        // Update current index to search index, but not Rx queue index
                        if(ucQueueIndex < MHL_CBUS_RX_QUEUE_INDEX)
                        {
                            SET_MHL_TX_QUEUE_INDEX_PORT2(ucQueueIndex);
                        }

                        bSendCommand = TRUE;
                    }
                }
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            if(!GET_MHL_CBUS_SILENCE3())
            {
                ucQueueIndex = _mdrv_mhl_GetCbusQueueIndex(ucPortSelect, GET_MHL_TX_QUEUE_INDEX_PORT3());

                if(ucQueueIndex < MHL_CBUS_TOTAL_QUEUE_NUM)
                {
                    if(_mdrv_mhl_CbusTxWriteCommand(ucPortSelect, &(GET_MHL_CBUS_QUEUE_PORT3(ucQueueIndex))))
                    {
                        // Retry send packet twice
                        if(GET_MHL_QUEUE_STATE_PORT3(ucQueueIndex) == CBUS_REQ_SENT)
                        {
                            SET_MHL_QUEUE_STATE_PORT3(ucQueueIndex, CBUS_REQ_IDLE);

                            bRetrySendCommand = TRUE;
                        }
                        else if(GET_MHL_QUEUE_STATE_PORT3(ucQueueIndex) > CBUS_REQ_SENT)
                        {
                            SET_MHL_QUEUE_STATE_PORT3(ucQueueIndex, CBUS_REQ_SENT);
                        }

                        // Update current index to search index, but not Rx queue index
                        if(ucQueueIndex < MHL_CBUS_RX_QUEUE_INDEX)
                        {
                            SET_MHL_TX_QUEUE_INDEX_PORT3(ucQueueIndex);
                        }

                        bSendCommand = TRUE;
                    }
                }
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            if(!GET_MHL_CBUS_SILENCE4())
            {
                ucQueueIndex = _mdrv_mhl_GetCbusQueueIndex(ucPortSelect, GET_MHL_TX_QUEUE_INDEX_PORT4());

                if(ucQueueIndex < MHL_CBUS_TOTAL_QUEUE_NUM)
                {
                    if(_mdrv_mhl_CbusTxWriteCommand(ucPortSelect, &(GET_MHL_CBUS_QUEUE_PORT4(ucQueueIndex))))
                    {
                        // Retry send packet twice
                        if(GET_MHL_QUEUE_STATE_PORT4(ucQueueIndex) == CBUS_REQ_SENT)
                        {
                            SET_MHL_QUEUE_STATE_PORT4(ucQueueIndex, CBUS_REQ_IDLE);

                            bRetrySendCommand = TRUE;
                        }
                        else if(GET_MHL_QUEUE_STATE_PORT4(ucQueueIndex) > CBUS_REQ_SENT)
                        {
                            SET_MHL_QUEUE_STATE_PORT4(ucQueueIndex, CBUS_REQ_SENT);
                        }

                        // Update current index to search index, but not Rx queue index
                        if(ucQueueIndex < MHL_CBUS_RX_QUEUE_INDEX)
                        {
                            SET_MHL_TX_QUEUE_INDEX_PORT4(ucQueueIndex);
                        }

                        bSendCommand = TRUE;
                    }
                }
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            if(!GET_MHL_CBUS_SILENCE5())
            {
                ucQueueIndex = _mdrv_mhl_GetCbusQueueIndex(ucPortSelect, GET_MHL_TX_QUEUE_INDEX_PORT5());

                if(ucQueueIndex < MHL_CBUS_TOTAL_QUEUE_NUM)
                {
                    if(_mdrv_mhl_CbusTxWriteCommand(ucPortSelect, &(GET_MHL_CBUS_QUEUE_PORT5(ucQueueIndex))))
                    {
                        // Retry send packet twice
                        if(GET_MHL_QUEUE_STATE_PORT5(ucQueueIndex) == CBUS_REQ_SENT)
                        {
                            SET_MHL_QUEUE_STATE_PORT5(ucQueueIndex, CBUS_REQ_IDLE);

                            bRetrySendCommand = TRUE;
                        }
                        else if(GET_MHL_QUEUE_STATE_PORT5(ucQueueIndex) > CBUS_REQ_SENT)
                        {
                            SET_MHL_QUEUE_STATE_PORT5(ucQueueIndex, CBUS_REQ_SENT);
                        }

                        // Update current index to search index, but not Rx queue index
                        if(ucQueueIndex < MHL_CBUS_RX_QUEUE_INDEX)
                        {
                            SET_MHL_TX_QUEUE_INDEX_PORT5(ucQueueIndex);
                        }

                        bSendCommand = TRUE;
                    }
                }
            }

            break;
#endif

        default:

            break;
    };

    if(bRetrySendCommand)
    {
        MHL_DRV_DPRINTF("** MHL Cbus Tx retry port %d", ucPortSelect);
    }

    if(bSendCommand)
    {
        _mdrv_mhl_InsertCbusTimer(ucPortSelect, MHL_CBUS_SEND_HOLD_TIME);
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_OffLineCellPhonePatch()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_OffLineCellPhonePatch(BYTE ucPortSelect)
{
    Bool bSendCapReady = FALSE;
    Bool bSendPathEn = FALSE;
    Bool bSendClrHpd = FALSE;
    BYTE ucCellPhoneID = _mdrv_mhl_GetCellPhoneID(ucPortSelect);

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            if(ucCellPhoneID == MHL_CELL_PHONE_ID_ROKU_DEVICE)
            {
                if(!GET_MHL_DEVCAP_READY_FLAG0())
                {
                    bSendCapReady = TRUE;
                }
                else if(!GET_MHL_PATH_EN_FLAG0())
                {
                    bSendPathEn = TRUE;
                }
            }
            else if((ucCellPhoneID == MHL_CELL_PHONE_ID_HTC_BUTTER_FLY) || (ucCellPhoneID == MHL_CELL_PHONE_ID_HUAWEI))
            {
                if(!GET_MHL_DEVCAP_READY_FLAG0())
                {
                    bSendCapReady = TRUE;
                }
            }

            #if MHL_SAM_SII_SELF_WAKEUP_PATCH
            if(ucCellPhoneID == MHL_CELL_PHONE_ID_SAMSUNG)
            {
            }
            else
             #endif
            {
                if(GET_MHL_SET_HPD_FLAG0())
                       bSendClrHpd = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            if(ucCellPhoneID == MHL_CELL_PHONE_ID_ROKU_DEVICE)
            {
                if(!GET_MHL_DEVCAP_READY_FLAG1())
                {
                    bSendCapReady = TRUE;
                }
                else if(!GET_MHL_PATH_EN_FLAG1())
                {
                    bSendPathEn = TRUE;
                }
            }
            else if((ucCellPhoneID == MHL_CELL_PHONE_ID_HTC_BUTTER_FLY) || (ucCellPhoneID == MHL_CELL_PHONE_ID_HUAWEI))
            {
                if(!GET_MHL_DEVCAP_READY_FLAG1())
                {
                    bSendCapReady = TRUE;
                }
            }

            #if MHL_SAM_SII_SELF_WAKEUP_PATCH
            if(ucCellPhoneID == MHL_CELL_PHONE_ID_SAMSUNG)
            {
            }
            else
             #endif
            {
                if(GET_MHL_SET_HPD_FLAG1())
                        bSendClrHpd = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            if(ucCellPhoneID == MHL_CELL_PHONE_ID_ROKU_DEVICE)
            {
                if(!GET_MHL_DEVCAP_READY_FLAG2())
                {
                    bSendCapReady = TRUE;
                }
                else if(!GET_MHL_PATH_EN_FLAG2())
                {
                    bSendPathEn = TRUE;
                }
            }
            else if((ucCellPhoneID == MHL_CELL_PHONE_ID_HTC_BUTTER_FLY) || (ucCellPhoneID == MHL_CELL_PHONE_ID_HUAWEI))
            {
                if(!GET_MHL_DEVCAP_READY_FLAG2())
                {
                    bSendCapReady = TRUE;
                }
            }

            #if MHL_SAM_SII_SELF_WAKEUP_PATCH
            if(ucCellPhoneID == MHL_CELL_PHONE_ID_SAMSUNG)
            {
            }
            else
             #endif
            {
                if(GET_MHL_SET_HPD_FLAG2())
                        bSendClrHpd = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            if(ucCellPhoneID == MHL_CELL_PHONE_ID_ROKU_DEVICE)
            {
                if(!GET_MHL_DEVCAP_READY_FLAG3())
                {
                    bSendCapReady = TRUE;
                }
                else if(!GET_MHL_PATH_EN_FLAG3())
                {
                    bSendPathEn = TRUE;
                }
            }
            else if((ucCellPhoneID == MHL_CELL_PHONE_ID_HTC_BUTTER_FLY) || (ucCellPhoneID == MHL_CELL_PHONE_ID_HUAWEI))
            {
                if(!GET_MHL_DEVCAP_READY_FLAG3())
                {
                    bSendCapReady = TRUE;
                }
            }

            #if MHL_SAM_SII_SELF_WAKEUP_PATCH
            if(ucCellPhoneID == MHL_CELL_PHONE_ID_SAMSUNG)
            {
            }
            else
             #endif
            {
                if(GET_MHL_SET_HPD_FLAG3())
                        bSendClrHpd = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            if(ucCellPhoneID == MHL_CELL_PHONE_ID_ROKU_DEVICE)
            {
                if(!GET_MHL_DEVCAP_READY_FLAG4())
                {
                    bSendCapReady = TRUE;
                }
                else if(!GET_MHL_PATH_EN_FLAG4())
                {
                    bSendPathEn = TRUE;
                }
            }
            else if((ucCellPhoneID == MHL_CELL_PHONE_ID_HTC_BUTTER_FLY) || (ucCellPhoneID == MHL_CELL_PHONE_ID_HUAWEI))
            {
                if(!GET_MHL_DEVCAP_READY_FLAG4())
                {
                    bSendCapReady = TRUE;
                }
            }

            #if MHL_SAM_SII_SELF_WAKEUP_PATCH
            if(ucCellPhoneID == MHL_CELL_PHONE_ID_SAMSUNG)
            {
            }
            else
             #endif
            {
                if(GET_MHL_SET_HPD_FLAG4())
                        bSendClrHpd = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            if(ucCellPhoneID == MHL_CELL_PHONE_ID_ROKU_DEVICE)
            {
                if(!GET_MHL_DEVCAP_READY_FLAG5())
                {
                    bSendCapReady = TRUE;
                }
                else if(!GET_MHL_PATH_EN_FLAG5())
                {
                    bSendPathEn = TRUE;
                }
            }
            else if((ucCellPhoneID == MHL_CELL_PHONE_ID_HTC_BUTTER_FLY) || (ucCellPhoneID == MHL_CELL_PHONE_ID_HUAWEI))
            {
                if(!GET_MHL_DEVCAP_READY_FLAG5())
                {
                    bSendCapReady = TRUE;
                }
            }

            #if MHL_SAM_SII_SELF_WAKEUP_PATCH
            if(ucCellPhoneID == MHL_CELL_PHONE_ID_SAMSUNG)
            {
            }
            else
             #endif
            {
                if(GET_MHL_SET_HPD_FLAG5())
                        bSendClrHpd = TRUE;
            }

            break;
#endif

        default:

            break;
    };

    if(bSendCapReady)
    {
        _mdrv_mhl_CbusSendDevcapReady(ucPortSelect);
    }

    if(bSendPathEn)
    {
        _mdrv_mhl_CbusSendPathEnable(ucPortSelect);
    }

    if(bSendClrHpd)
    {
        _mdrv_mhl_CbusSendSetHPD(ucPortSelect, FALSE);
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_GetCbusDisplayMask()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE _mdrv_mhl_GetCbusDisplayMask(BYTE ucPortSelect)
{
    BYTE ucDisplayMask = MHL_CBUS_DISPLAY_MASK;

    if(_mdrv_mhl_GetCbusEnhanceFlag(ucPortSelect) && _mdrv_mhl_GetECbusSupportFlag(ucPortSelect))
    {
        ucDisplayMask = ucDisplayMask |MHL_ECBUS_DISPLAY_MASK;
    }
    
    return ucDisplayMask;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_SendDisplayCommand()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_SendDisplayCommand(BYTE ucPortSelect)
{
    Bool bFlag = FALSE;
    BYTE ucIndex = 0xFF;
    BYTE ucDisplayMask = _mdrv_mhl_GetCbusDisplayMask(ucPortSelect);
    BYTE ucCellPhoneID = _mdrv_mhl_GetCellPhoneID(ucPortSelect);

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            if(GET_MHL_SEND_DISPLAY_COMMAND0())
            {
                ucIndex = GET_MHL_CBUS_DISPLAY_FLAG0() ^ucDisplayMask;
            }
            else if(GET_MHL_SET_HPD_FLAG0())
            {
                bFlag = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            if(GET_MHL_SEND_DISPLAY_COMMAND1())
            {
                ucIndex = GET_MHL_CBUS_DISPLAY_FLAG1() ^ucDisplayMask;
            }
            else if(GET_MHL_SET_HPD_FLAG1())
            {
                bFlag = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            if(GET_MHL_SEND_DISPLAY_COMMAND2())
            {
                ucIndex = GET_MHL_CBUS_DISPLAY_FLAG2() ^ucDisplayMask;
            }
            else if(GET_MHL_SET_HPD_FLAG2())
            {
                bFlag = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            if(GET_MHL_SEND_DISPLAY_COMMAND3())
            {
                ucIndex = GET_MHL_CBUS_DISPLAY_FLAG3() ^ucDisplayMask;
            }
            else if(GET_MHL_SET_HPD_FLAG3())
            {
                bFlag = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            if(GET_MHL_SEND_DISPLAY_COMMAND4())
            {
                ucIndex = GET_MHL_CBUS_DISPLAY_FLAG4() ^ucDisplayMask;
            }
            else if(GET_MHL_SET_HPD_FLAG4())
            {
                bFlag = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            if(GET_MHL_SEND_DISPLAY_COMMAND5())
            {
                ucIndex = GET_MHL_CBUS_DISPLAY_FLAG5() ^ucDisplayMask;
            }
            else if(GET_MHL_SET_HPD_FLAG5())
            {
                bFlag = TRUE;
            }

            break;
#endif

        default:

            break;
    };

    //if(bFlag) // Send CLR_HPD to disable content
    {
        //if(mhal_mhl_CheckSignalStable(ucPortSelect)) 
        {
           // _mdrv_mhl_CbusSendSetHPD(ucPortSelect, FALSE);
        }
    }
    //else if(ucIndex == 0xFF)
    if(ucIndex == 0xFF)
    {
        _mdrv_mhl_OffLineCellPhonePatch(ucPortSelect);
    }
    else if((ucIndex <= ucDisplayMask) && (ucIndex > 0))
    {
        switch(_mdrv_mhl_CbusGetLeastFlag(ucIndex))
        {
            case MHL_CBUS_HPD_SET_FLAG:
                _mdrv_mhl_CbusSendSetHPD(ucPortSelect, TRUE);
                break;

            case MHL_CBUS_PATH_EN_FLAG:
                _mdrv_mhl_CbusSendPathEnable(ucPortSelect);
                break;

            case MHL_CBUS_DEVCAP_READY_FLAG:
                switch(ucCellPhoneID)
                {
                    #if MHL_SONY_Z3_WITHOUT_DCAP_RDY_PATCH
                    case MHL_CELL_PHONE_ID_SONY:
                        break;
                    #endif
                        
                    default:
                        _mdrv_mhl_CbusSendDevcapReady(ucPortSelect);
                        break;
                 }
                break;

            case MHL_CBUS_DEVCAP_CHANGE_FLAG:
                _mdrv_mhl_CbusSendDevcapChange(ucPortSelect);
                break;

            case MHL_CBUS_VERSION_CODE_FLAG:
                _mdrv_mhl_CbusSendVersionCode(ucPortSelect);
                break;

            case MHL_CBUS_LINK_STATUS_NORMAL_FLAG:
                _mdrv_mhl_CbusSendLinkStatusNormal(ucPortSelect);
                break;
                
            default:

                break;
        };
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_WriteBurstEventProc()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_WriteBurstEventProc(BYTE ucPortSelect)
{
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            switch(GET_MHL_WRITE_BURST_STATE_PORT0())
            {
                case MHL_CBUS_WRITE_BURST_START:
                    if(_mdrv_mhl_CbusWriteStatusOrSetInt(ucPortSelect, 0x20, BIT2, FALSE))
                    {
                        SET_MHL_WRITE_BURST_STATE_PORT0(MHL_CBUS_WRITE_BURST_SEND_REQUEST);
                    }
                    break;

                case MHL_CBUS_WRITE_BURST_SEND_DATA:
                    if(_mdrv_mhl_CbusWriteStatusOrSetInt(ucPortSelect, 0x20, BIT1, FALSE))
                    {
                        SET_MHL_WRITE_BURST_STATE_PORT0(MHL_CBUS_WRITE_BURST_END);
                    }
                    break;

                case MHL_CBUS_WRITE_BURST_END:
                    SET_MHL_WRITE_BURST_STATE_PORT0(MHL_CBUS_WRITE_BURST_NONE);
                    break;

                default:

                    break;
            };

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            switch(GET_MHL_WRITE_BURST_STATE_PORT1())
            {
                case MHL_CBUS_WRITE_BURST_START:
                    if(_mdrv_mhl_CbusWriteStatusOrSetInt(ucPortSelect, 0x20, BIT2, FALSE))
                    {
                        SET_MHL_WRITE_BURST_STATE_PORT1(MHL_CBUS_WRITE_BURST_SEND_REQUEST);
                    }
                    break;

                case MHL_CBUS_WRITE_BURST_SEND_DATA:
                    if(_mdrv_mhl_CbusWriteStatusOrSetInt(ucPortSelect, 0x20, BIT1, FALSE))
                    {
                        SET_MHL_WRITE_BURST_STATE_PORT1(MHL_CBUS_WRITE_BURST_END);
                    }
                    break;

                case MHL_CBUS_WRITE_BURST_END:
                    SET_MHL_WRITE_BURST_STATE_PORT1(MHL_CBUS_WRITE_BURST_NONE);
                    break;

                default:

                    break;
            };

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            switch(GET_MHL_WRITE_BURST_STATE_PORT2())
            {
                case MHL_CBUS_WRITE_BURST_START:
                    if(_mdrv_mhl_CbusWriteStatusOrSetInt(ucPortSelect, 0x20, BIT2, FALSE))
                    {
                        SET_MHL_WRITE_BURST_STATE_PORT2(MHL_CBUS_WRITE_BURST_SEND_REQUEST);
                    }
                    break;

                case MHL_CBUS_WRITE_BURST_SEND_DATA:
                    if(_mdrv_mhl_CbusWriteStatusOrSetInt(ucPortSelect, 0x20, BIT1, FALSE))
                    {
                        SET_MHL_WRITE_BURST_STATE_PORT2(MHL_CBUS_WRITE_BURST_END);
                    }
                    break;

                case MHL_CBUS_WRITE_BURST_END:
                    SET_MHL_WRITE_BURST_STATE_PORT2(MHL_CBUS_WRITE_BURST_NONE);
                    break;

                default:

                    break;
            };

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            switch(GET_MHL_WRITE_BURST_STATE_PORT3())
            {
                case MHL_CBUS_WRITE_BURST_START:
                    if(_mdrv_mhl_CbusWriteStatusOrSetInt(ucPortSelect, 0x20, BIT2, FALSE))
                    {
                        SET_MHL_WRITE_BURST_STATE_PORT3(MHL_CBUS_WRITE_BURST_SEND_REQUEST);
                    }
                    break;

                case MHL_CBUS_WRITE_BURST_SEND_DATA:
                    if(_mdrv_mhl_CbusWriteStatusOrSetInt(ucPortSelect, 0x20, BIT1, FALSE))
                    {
                        SET_MHL_WRITE_BURST_STATE_PORT3(MHL_CBUS_WRITE_BURST_END);
                    }
                    break;

                case MHL_CBUS_WRITE_BURST_END:
                    SET_MHL_WRITE_BURST_STATE_PORT3(MHL_CBUS_WRITE_BURST_NONE);
                    break;

                default:

                    break;
            };

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            switch(GET_MHL_WRITE_BURST_STATE_PORT4())
            {
                case MHL_CBUS_WRITE_BURST_START:
                    if(_mdrv_mhl_CbusWriteStatusOrSetInt(ucPortSelect, 0x20, BIT2, FALSE))
                    {
                        SET_MHL_WRITE_BURST_STATE_PORT4(MHL_CBUS_WRITE_BURST_SEND_REQUEST);
                    }
                    break;

                case MHL_CBUS_WRITE_BURST_SEND_DATA:
                    if(_mdrv_mhl_CbusWriteStatusOrSetInt(ucPortSelect, 0x20, BIT1, FALSE))
                    {
                        SET_MHL_WRITE_BURST_STATE_PORT4(MHL_CBUS_WRITE_BURST_END);
                    }
                    break;

                case MHL_CBUS_WRITE_BURST_END:
                    SET_MHL_WRITE_BURST_STATE_PORT4(MHL_CBUS_WRITE_BURST_NONE);
                    break;

                default:

                    break;
            };

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            switch(GET_MHL_WRITE_BURST_STATE_PORT5())
            {
                case MHL_CBUS_WRITE_BURST_START:
                    if(_mdrv_mhl_CbusWriteStatusOrSetInt(ucPortSelect, 0x20, BIT2, FALSE))
                    {
                        SET_MHL_WRITE_BURST_STATE_PORT5(MHL_CBUS_WRITE_BURST_SEND_REQUEST);
                    }
                    break;

                case MHL_CBUS_WRITE_BURST_SEND_DATA:
                    if(_mdrv_mhl_CbusWriteStatusOrSetInt(ucPortSelect, 0x20, BIT1, FALSE))
                    {
                        SET_MHL_WRITE_BURST_STATE_PORT5(MHL_CBUS_WRITE_BURST_END);
                    }
                    break;

                case MHL_CBUS_WRITE_BURST_END:
                    SET_MHL_WRITE_BURST_STATE_PORT5(MHL_CBUS_WRITE_BURST_NONE);
                    break;

                default:

                    break;
            };

            break;
#endif

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusRHIDStateProc()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_CbusRHIDStateProc(BYTE ucPortSelect)
{
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            switch(GET_MHL_RHID_PROCESS_STATE_PORT0())
            {
                case MHL_CBUS_RHID_NONE:
                    if(GET_MHL_SEND_RHID_ENABLE_ENABLE0()) // Enable HID function
                    {
                        SET_MHL_RHID_PROCESS_STATE_PORT0(MHL_CBUS_RHID_START);
                    }
                    break;

                case MHL_CBUS_RHID_START:
                    if(_mdrv_mhl_CbusSendSubMessage(ucPortSelect, MSC_MSG_RHID, MHL_RUSB_REQUEST_HOST, TRUE)) // Send RHID request host success
                    {
                        SET_MHL_RHID_PROCESS_STATE_PORT0(MHL_CBUS_RHID_REQUEST_HOST);
                    }
                    else if(!GET_MHL_SEND_RHID_ENABLE_ENABLE0())
                    {
                        SET_MHL_RHID_PROCESS_STATE_PORT0(MHL_CBUS_RHID_END);
                    }
                    break;

                case MHL_CBUS_RHID_SINK_HOST:
                    if(!GET_MHL_SEND_RHID_ENABLE_ENABLE0()) // Disable HID function
                    {
                        if(_mdrv_mhl_CbusSendSubMessage(ucPortSelect, MSC_MSG_RHID, MHL_RUSB_RELINQUISH_HOST, TRUE)) // Send RHID relinquish host success
                        {
                            SET_MHL_RHID_PROCESS_STATE_PORT0(MHL_CBUS_RHID_RELINQUISH);
                        }
                    }
                    break;

                case MHL_CBUS_RHID_RELINQUISH:
                    SET_MHL_RHID_PROCESS_STATE_PORT0(MHL_CBUS_RHID_END);
                    break;

                case MHL_CBUS_RHID_END:
                    SET_MHL_RHID_PROCESS_STATE_PORT0(MHL_CBUS_RHID_NONE);
                    break;

                default:

                    break;
            };

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            switch(GET_MHL_RHID_PROCESS_STATE_PORT1())
            {
                case MHL_CBUS_RHID_NONE:
                    if(GET_MHL_SEND_RHID_ENABLE_ENABLE1()) // Enable HID function
                    {
                        SET_MHL_RHID_PROCESS_STATE_PORT1(MHL_CBUS_RHID_START);
                    }
                    break;

                case MHL_CBUS_RHID_START:
                    if(_mdrv_mhl_CbusSendSubMessage(ucPortSelect, MSC_MSG_RHID, MHL_RUSB_REQUEST_HOST, TRUE)) // Send RHID request host success
                    {
                        SET_MHL_RHID_PROCESS_STATE_PORT1(MHL_CBUS_RHID_REQUEST_HOST);
                    }
                    else if(!GET_MHL_SEND_RHID_ENABLE_ENABLE1())
                    {
                        SET_MHL_RHID_PROCESS_STATE_PORT1(MHL_CBUS_RHID_END);
                    }
                    break;

                case MHL_CBUS_RHID_SINK_HOST:
                    if(!GET_MHL_SEND_RHID_ENABLE_ENABLE1()) // Disable HID function
                    {
                        if(_mdrv_mhl_CbusSendSubMessage(ucPortSelect, MSC_MSG_RHID, MHL_RUSB_RELINQUISH_HOST, TRUE)) // Send RHID relinquish host success
                        {
                            SET_MHL_RHID_PROCESS_STATE_PORT1(MHL_CBUS_RHID_RELINQUISH);
                        }
                    }
                    break;

                case MHL_CBUS_RHID_RELINQUISH:
                    SET_MHL_RHID_PROCESS_STATE_PORT1(MHL_CBUS_RHID_END);
                    break;

                case MHL_CBUS_RHID_END:
                    SET_MHL_RHID_PROCESS_STATE_PORT1(MHL_CBUS_RHID_NONE);
                    break;

                default:

                    break;
            };

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            switch(GET_MHL_RHID_PROCESS_STATE_PORT2())
            {
                case MHL_CBUS_RHID_NONE:
                    if(GET_MHL_SEND_RHID_ENABLE_ENABLE2()) // Enable HID function
                    {
                        SET_MHL_RHID_PROCESS_STATE_PORT2(MHL_CBUS_RHID_START);
                    }
                    break;

                case MHL_CBUS_RHID_START:
                    if(_mdrv_mhl_CbusSendSubMessage(ucPortSelect, MSC_MSG_RHID, MHL_RUSB_REQUEST_HOST, TRUE)) // Send RHID request host success
                    {
                        SET_MHL_RHID_PROCESS_STATE_PORT2(MHL_CBUS_RHID_REQUEST_HOST);
                    }
                    else if(!GET_MHL_SEND_RHID_ENABLE_ENABLE2())
                    {
                        SET_MHL_RHID_PROCESS_STATE_PORT2(MHL_CBUS_RHID_END);
                    }
                    break;

                case MHL_CBUS_RHID_SINK_HOST:
                    if(!GET_MHL_SEND_RHID_ENABLE_ENABLE2()) // Disable HID function
                    {
                        if(_mdrv_mhl_CbusSendSubMessage(ucPortSelect, MSC_MSG_RHID, MHL_RUSB_RELINQUISH_HOST, TRUE)) // Send RHID relinquish host success
                        {
                            SET_MHL_RHID_PROCESS_STATE_PORT2(MHL_CBUS_RHID_RELINQUISH);
                        }
                    }
                    break;

                case MHL_CBUS_RHID_RELINQUISH:
                    SET_MHL_RHID_PROCESS_STATE_PORT2(MHL_CBUS_RHID_END);
                    break;

                case MHL_CBUS_RHID_END:
                    SET_MHL_RHID_PROCESS_STATE_PORT2(MHL_CBUS_RHID_NONE);
                    break;

                default:

                    break;
            };

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            switch(GET_MHL_RHID_PROCESS_STATE_PORT3())
            {
                case MHL_CBUS_RHID_NONE:
                    if(GET_MHL_SEND_RHID_ENABLE_ENABLE3()) // Enable HID function
                    {
                        SET_MHL_RHID_PROCESS_STATE_PORT3(MHL_CBUS_RHID_START);
                    }
                    break;

                case MHL_CBUS_RHID_START:
                    if(_mdrv_mhl_CbusSendSubMessage(ucPortSelect, MSC_MSG_RHID, MHL_RUSB_REQUEST_HOST, TRUE)) // Send RHID request host success
                    {
                        SET_MHL_RHID_PROCESS_STATE_PORT3(MHL_CBUS_RHID_REQUEST_HOST);
                    }
                    else if(!GET_MHL_SEND_RHID_ENABLE_ENABLE3())
                    {
                        SET_MHL_RHID_PROCESS_STATE_PORT3(MHL_CBUS_RHID_END);
                    }
                    break;

                case MHL_CBUS_RHID_SINK_HOST:
                    if(!GET_MHL_SEND_RHID_ENABLE_ENABLE3()) // Disable HID function
                    {
                        if(_mdrv_mhl_CbusSendSubMessage(ucPortSelect, MSC_MSG_RHID, MHL_RUSB_RELINQUISH_HOST, TRUE)) // Send RHID relinquish host success
                        {
                            SET_MHL_RHID_PROCESS_STATE_PORT3(MHL_CBUS_RHID_RELINQUISH);
                        }
                    }
                    break;

                case MHL_CBUS_RHID_RELINQUISH:
                    SET_MHL_RHID_PROCESS_STATE_PORT3(MHL_CBUS_RHID_END);
                    break;

                case MHL_CBUS_RHID_END:
                    SET_MHL_RHID_PROCESS_STATE_PORT3(MHL_CBUS_RHID_NONE);
                    break;

                default:

                    break;
            };

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            switch(GET_MHL_RHID_PROCESS_STATE_PORT4())
            {
                case MHL_CBUS_RHID_NONE:
                    if(GET_MHL_SEND_RHID_ENABLE_ENABLE4()) // Enable HID function
                    {
                        SET_MHL_RHID_PROCESS_STATE_PORT4(MHL_CBUS_RHID_START);
                    }
                    break;

                case MHL_CBUS_RHID_START:
                    if(_mdrv_mhl_CbusSendSubMessage(ucPortSelect, MSC_MSG_RHID, MHL_RUSB_REQUEST_HOST, TRUE)) // Send RHID request host success
                    {
                        SET_MHL_RHID_PROCESS_STATE_PORT4(MHL_CBUS_RHID_REQUEST_HOST);
                    }
                    else if(!GET_MHL_SEND_RHID_ENABLE_ENABLE4())
                    {
                        SET_MHL_RHID_PROCESS_STATE_PORT4(MHL_CBUS_RHID_END);
                    }
                    break;

                case MHL_CBUS_RHID_SINK_HOST:
                    if(!GET_MHL_SEND_RHID_ENABLE_ENABLE4()) // Disable HID function
                    {
                        if(_mdrv_mhl_CbusSendSubMessage(ucPortSelect, MSC_MSG_RHID, MHL_RUSB_RELINQUISH_HOST, TRUE)) // Send RHID relinquish host success
                        {
                            SET_MHL_RHID_PROCESS_STATE_PORT4(MHL_CBUS_RHID_RELINQUISH);
                        }
                    }
                    break;

                case MHL_CBUS_RHID_RELINQUISH:
                    SET_MHL_RHID_PROCESS_STATE_PORT4(MHL_CBUS_RHID_END);
                    break;

                case MHL_CBUS_RHID_END:
                    SET_MHL_RHID_PROCESS_STATE_PORT4(MHL_CBUS_RHID_NONE);
                    break;

                default:

                    break;
            };

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            switch(GET_MHL_RHID_PROCESS_STATE_PORT5())
            {
                case MHL_CBUS_RHID_NONE:
                    if(GET_MHL_SEND_RHID_ENABLE_ENABLE5()) // Enable HID function
                    {
                        SET_MHL_RHID_PROCESS_STATE_PORT5(MHL_CBUS_RHID_START);
                    }
                    break;

                case MHL_CBUS_RHID_START:
                    if(_mdrv_mhl_CbusSendSubMessage(ucPortSelect, MSC_MSG_RHID, MHL_RUSB_REQUEST_HOST, TRUE)) // Send RHID request host success
                    {
                        SET_MHL_RHID_PROCESS_STATE_PORT5(MHL_CBUS_RHID_REQUEST_HOST);
                    }
                    else if(!GET_MHL_SEND_RHID_ENABLE_ENABLE5())
                    {
                        SET_MHL_RHID_PROCESS_STATE_PORT5(MHL_CBUS_RHID_END);
                    }
                    break;

                case MHL_CBUS_RHID_SINK_HOST:
                    if(!GET_MHL_SEND_RHID_ENABLE_ENABLE5()) // Disable HID function
                    {
                        if(_mdrv_mhl_CbusSendSubMessage(ucPortSelect, MSC_MSG_RHID, MHL_RUSB_RELINQUISH_HOST, TRUE)) // Send RHID relinquish host success
                        {
                            SET_MHL_RHID_PROCESS_STATE_PORT5(MHL_CBUS_RHID_RELINQUISH);
                        }
                    }
                    break;

                case MHL_CBUS_RHID_RELINQUISH:
                    SET_MHL_RHID_PROCESS_STATE_PORT5(MHL_CBUS_RHID_END);
                    break;

                case MHL_CBUS_RHID_END:
                    SET_MHL_RHID_PROCESS_STATE_PORT5(MHL_CBUS_RHID_NONE);
                    break;

                default:

                    break;
            };

            break;
#endif

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_GetBISTReadyIndex()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE _mdrv_mhl_GetBISTReadyIndex(BYTE ucPortSelect)
{
    BYTE ucReadyIndex = 0;

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            ucReadyIndex = GET_MHL_BIST_READY_PORT0();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            ucReadyIndex = GET_MHL_BIST_READY_PORT1();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            ucReadyIndex = GET_MHL_BIST_READY_PORT2();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            ucReadyIndex = GET_MHL_BIST_READY_PORT3();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            ucReadyIndex = GET_MHL_BIST_READY_PORT4();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            ucReadyIndex = GET_MHL_BIST_READY_PORT5();
            break;
#endif

        default:

            break;
    };

    return ucReadyIndex;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_BISTModeUpProc()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_BISTModeUpProc(BYTE ucPortSelect)
{
    Bool bTriggerAVLinkFlag = FALSE;
    
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            bTriggerAVLinkFlag = GET_MHL_BIST_TRIGGER_AV_LINK0();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            bTriggerAVLinkFlag = GET_MHL_BIST_TRIGGER_AV_LINK1();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            bTriggerAVLinkFlag = GET_MHL_BIST_TRIGGER_AV_LINK2();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            bTriggerAVLinkFlag = GET_MHL_BIST_TRIGGER_AV_LINK3();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            bTriggerAVLinkFlag = GET_MHL_BIST_TRIGGER_AV_LINK4();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            bTriggerAVLinkFlag = GET_MHL_BIST_TRIGGER_AV_LINK5();
            break;
#endif

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_BISTModeDownProc()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_BISTModeDownProc(BYTE ucPortSelect)
{
    Bool bTriggerAVLinkFlag = FALSE;
    
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            bTriggerAVLinkFlag = GET_MHL_BIST_TRIGGER_AV_LINK0();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            bTriggerAVLinkFlag = GET_MHL_BIST_TRIGGER_AV_LINK1();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            bTriggerAVLinkFlag = GET_MHL_BIST_TRIGGER_AV_LINK2();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            bTriggerAVLinkFlag = GET_MHL_BIST_TRIGGER_AV_LINK3();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            bTriggerAVLinkFlag = GET_MHL_BIST_TRIGGER_AV_LINK4();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            bTriggerAVLinkFlag = GET_MHL_BIST_TRIGGER_AV_LINK5();
            break;
#endif

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusBISTStateProc()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_CbusBISTStateProc(BYTE ucPortSelect)
{
    BYTE ucBISTECbusMode = 0;
    
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            switch(GET_MHL_BIST_PROCESS_STATE_PORT0())
            {
                case MHL_CBUS_BIST_RECEIVE_SETUP:
                    if(_mdrv_mhl_CbusSendSubMessage(ucPortSelect, MSC_MSG_BIST_READY, _mdrv_mhl_GetBISTReadyIndex(ucPortSelect), FALSE)) // Send BIST ready
                    {
                        SET_MHL_BIST_PROCESS_STATE_PORT0(MHL_CBUS_BIST_WAIT_TRIGGER);
                    }
                    break;

                case MHL_CBUS_BIST_RECEIVE_TRIGGER:
                    SET_MHL_BIST_PROCESS_STATE_PORT0(MHL_CBUS_BIST_ECBUS_SWITCH);
                    ucBISTECbusMode = MHL_RAP_KEY_CBUS_MODE_UP;
                    break;

                case MHL_CBUS_BIST_EVENT_FINISH:
                    SET_MHL_BIST_PROCESS_STATE_PORT0(MHL_CBUS_BIST_WAIT_TRIGGER);
                    ucBISTECbusMode = MHL_RAP_KEY_CBUS_MODE_DOWN;
                    break;

                case MHL_CBUS_BIST_REQUEST_STATUS:
                    SET_MHL_SEND_BIST_RETURN_STATUS0();
                    SET_MHL_BIST_PROCESS_STATE_PORT0(MHL_CBUS_BIST_RETURN_STATUS);
                    break;

                case MHL_CBUS_BIST_RETURN_STATUS:
                    if(!GET_MHL_SEND_BIST_RETURN_STATUS0())
                    {
                        SET_MHL_BIST_PROCESS_STATE_PORT0(MHL_CBUS_BIST_WAIT_TRIGGER);
                    }
                    break;

                case MHL_CBUS_BIST_END:
                    SET_MHL_BIST_PROCESS_STATE_PORT0(MHL_CBUS_BIST_NONE);
                    break;

                default:

                    break;
            };

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            switch(GET_MHL_BIST_PROCESS_STATE_PORT1())
            {
                case MHL_CBUS_BIST_RECEIVE_SETUP:
                    if(_mdrv_mhl_CbusSendSubMessage(ucPortSelect, MSC_MSG_BIST_READY, _mdrv_mhl_GetBISTReadyIndex(ucPortSelect), FALSE)) // Send BIST ready
                    {
                        SET_MHL_BIST_PROCESS_STATE_PORT1(MHL_CBUS_BIST_WAIT_TRIGGER);
                    }
                    break;

                case MHL_CBUS_BIST_RECEIVE_TRIGGER:
                    SET_MHL_BIST_PROCESS_STATE_PORT1(MHL_CBUS_BIST_ECBUS_SWITCH);
                    ucBISTECbusMode = MHL_RAP_KEY_CBUS_MODE_UP;
                    break;

                case MHL_CBUS_BIST_EVENT_FINISH:
                    SET_MHL_BIST_PROCESS_STATE_PORT1(MHL_CBUS_BIST_WAIT_TRIGGER);
                    ucBISTECbusMode = MHL_RAP_KEY_CBUS_MODE_DOWN;
                    break;

                case MHL_CBUS_BIST_REQUEST_STATUS:
                    SET_MHL_SEND_BIST_RETURN_STATUS1();
                    SET_MHL_BIST_PROCESS_STATE_PORT1(MHL_CBUS_BIST_RETURN_STATUS);
                    break;

                case MHL_CBUS_BIST_RETURN_STATUS:
                    if(!GET_MHL_SEND_BIST_RETURN_STATUS1())
                    {
                        SET_MHL_BIST_PROCESS_STATE_PORT1(MHL_CBUS_BIST_WAIT_TRIGGER);
                    }
                    break;

                case MHL_CBUS_BIST_END:
                    SET_MHL_BIST_PROCESS_STATE_PORT1(MHL_CBUS_BIST_NONE);
                    break;

                default:

                    break;
            };
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            switch(GET_MHL_BIST_PROCESS_STATE_PORT2())
            {
                case MHL_CBUS_BIST_RECEIVE_SETUP:
                    if(_mdrv_mhl_CbusSendSubMessage(ucPortSelect, MSC_MSG_BIST_READY, _mdrv_mhl_GetBISTReadyIndex(ucPortSelect), FALSE)) // Send BIST ready
                    {
                        SET_MHL_BIST_PROCESS_STATE_PORT2(MHL_CBUS_BIST_WAIT_TRIGGER);
                    }
                    break;

                case MHL_CBUS_BIST_RECEIVE_TRIGGER:
                    SET_MHL_BIST_PROCESS_STATE_PORT2(MHL_CBUS_BIST_ECBUS_SWITCH);
                    ucBISTECbusMode = MHL_RAP_KEY_CBUS_MODE_UP;
                    break;

                case MHL_CBUS_BIST_EVENT_FINISH:
                    SET_MHL_BIST_PROCESS_STATE_PORT2(MHL_CBUS_BIST_WAIT_TRIGGER);
                    ucBISTECbusMode = MHL_RAP_KEY_CBUS_MODE_DOWN;
                    break;

                case MHL_CBUS_BIST_REQUEST_STATUS:
                    SET_MHL_SEND_BIST_RETURN_STATUS2();
                    SET_MHL_BIST_PROCESS_STATE_PORT2(MHL_CBUS_BIST_RETURN_STATUS);
                    break;

                case MHL_CBUS_BIST_RETURN_STATUS:
                    if(!GET_MHL_SEND_BIST_RETURN_STATUS2())
                    {
                        SET_MHL_BIST_PROCESS_STATE_PORT2(MHL_CBUS_BIST_WAIT_TRIGGER);
                    }
                    break;

                case MHL_CBUS_BIST_END:
                    SET_MHL_BIST_PROCESS_STATE_PORT2(MHL_CBUS_BIST_NONE);
                    break;

                default:

                    break;
            };
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            switch(GET_MHL_BIST_PROCESS_STATE_PORT3())
            {
                case MHL_CBUS_BIST_RECEIVE_SETUP:
                    if(_mdrv_mhl_CbusSendSubMessage(ucPortSelect, MSC_MSG_BIST_READY, _mdrv_mhl_GetBISTReadyIndex(ucPortSelect), FALSE)) // Send BIST ready
                    {
                        SET_MHL_BIST_PROCESS_STATE_PORT3(MHL_CBUS_BIST_WAIT_TRIGGER);
                    }
                    break;

                case MHL_CBUS_BIST_RECEIVE_TRIGGER:
                    SET_MHL_BIST_PROCESS_STATE_PORT3(MHL_CBUS_BIST_ECBUS_SWITCH);
                    ucBISTECbusMode = MHL_RAP_KEY_CBUS_MODE_UP;
                    break;

                case MHL_CBUS_BIST_EVENT_FINISH:
                    SET_MHL_BIST_PROCESS_STATE_PORT3(MHL_CBUS_BIST_WAIT_TRIGGER);
                    ucBISTECbusMode = MHL_RAP_KEY_CBUS_MODE_DOWN;
                    break;

                case MHL_CBUS_BIST_REQUEST_STATUS:
                    SET_MHL_SEND_BIST_RETURN_STATUS3();
                    SET_MHL_BIST_PROCESS_STATE_PORT3(MHL_CBUS_BIST_RETURN_STATUS);
                    break;

                case MHL_CBUS_BIST_RETURN_STATUS:
                    if(!GET_MHL_SEND_BIST_RETURN_STATUS3())
                    {
                        SET_MHL_BIST_PROCESS_STATE_PORT3(MHL_CBUS_BIST_WAIT_TRIGGER);
                    }
                    break;

                case MHL_CBUS_BIST_END:
                    SET_MHL_BIST_PROCESS_STATE_PORT3(MHL_CBUS_BIST_NONE);
                    break;

                default:

                    break;
            };
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            switch(GET_MHL_BIST_PROCESS_STATE_PORT4())
            {
                case MHL_CBUS_BIST_RECEIVE_SETUP:
                    if(_mdrv_mhl_CbusSendSubMessage(ucPortSelect, MSC_MSG_BIST_READY, _mdrv_mhl_GetBISTReadyIndex(ucPortSelect), FALSE)) // Send BIST ready
                    {
                        SET_MHL_BIST_PROCESS_STATE_PORT4(MHL_CBUS_BIST_WAIT_TRIGGER);
                    }
                    break;

                case MHL_CBUS_BIST_RECEIVE_TRIGGER:
                    SET_MHL_BIST_PROCESS_STATE_PORT4(MHL_CBUS_BIST_ECBUS_SWITCH);
                    ucBISTECbusMode = MHL_RAP_KEY_CBUS_MODE_UP;
                    break;

                case MHL_CBUS_BIST_EVENT_FINISH:
                    SET_MHL_BIST_PROCESS_STATE_PORT4(MHL_CBUS_BIST_WAIT_TRIGGER);
                    ucBISTECbusMode = MHL_RAP_KEY_CBUS_MODE_DOWN;
                    break;

                case MHL_CBUS_BIST_REQUEST_STATUS:
                    SET_MHL_SEND_BIST_RETURN_STATUS4();
                    SET_MHL_BIST_PROCESS_STATE_PORT4(MHL_CBUS_BIST_RETURN_STATUS);
                    break;

                case MHL_CBUS_BIST_RETURN_STATUS:
                    if(!GET_MHL_SEND_BIST_RETURN_STATUS4())
                    {
                        SET_MHL_BIST_PROCESS_STATE_PORT4(MHL_CBUS_BIST_WAIT_TRIGGER);
                    }
                    break;

                case MHL_CBUS_BIST_END:
                    SET_MHL_BIST_PROCESS_STATE_PORT4(MHL_CBUS_BIST_NONE);
                    break;

                default:

                    break;
            };
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            switch(GET_MHL_BIST_PROCESS_STATE_PORT5())
            {
                case MHL_CBUS_BIST_RECEIVE_SETUP:
                    if(_mdrv_mhl_CbusSendSubMessage(ucPortSelect, MSC_MSG_BIST_READY, _mdrv_mhl_GetBISTReadyIndex(ucPortSelect), FALSE)) // Send BIST ready
                    {
                        SET_MHL_BIST_PROCESS_STATE_PORT5(MHL_CBUS_BIST_WAIT_TRIGGER);
                    }
                    break;

                case MHL_CBUS_BIST_RECEIVE_TRIGGER:
                    SET_MHL_BIST_PROCESS_STATE_PORT5(MHL_CBUS_BIST_ECBUS_SWITCH);
                    ucBISTECbusMode = MHL_RAP_KEY_CBUS_MODE_UP;
                    break;

                case MHL_CBUS_BIST_EVENT_FINISH:
                    SET_MHL_BIST_PROCESS_STATE_PORT5(MHL_CBUS_BIST_WAIT_TRIGGER);
                    ucBISTECbusMode = MHL_RAP_KEY_CBUS_MODE_DOWN;
                    break;

                case MHL_CBUS_BIST_REQUEST_STATUS:
                    SET_MHL_SEND_BIST_RETURN_STATUS5();
                    SET_MHL_BIST_PROCESS_STATE_PORT5(MHL_CBUS_BIST_RETURN_STATUS);
                    break;

                case MHL_CBUS_BIST_RETURN_STATUS:
                    if(!GET_MHL_SEND_BIST_RETURN_STATUS5())
                    {
                        SET_MHL_BIST_PROCESS_STATE_PORT5(MHL_CBUS_BIST_WAIT_TRIGGER);
                    }
                    break;

                case MHL_CBUS_BIST_END:
                    SET_MHL_BIST_PROCESS_STATE_PORT5(MHL_CBUS_BIST_NONE);
                    break;

                default:

                    break;
            };
            
            break;
#endif

        default:

            break;
    };

    if(ucBISTECbusMode > 0)
    {
        mdrv_mhl_ReceiveECbusModeChange(ucPortSelect, ucBISTECbusMode);

        if(ucBISTECbusMode == MHL_RAP_KEY_CBUS_MODE_UP)
        {
            _mdrv_mhl_BISTModeUpProc(ucPortSelect);
        }
        else if(ucBISTECbusMode == MHL_RAP_KEY_CBUS_MODE_DOWN)
        {
            _mdrv_mhl_BISTModeDownProc(ucPortSelect);
        }
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_FeatureRequestStateProc()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_FeatureRequestStateProc(BYTE ucPortSelect)
{
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            switch(GET_MHL_FEATURE_REQ_STATE_PORT0())
            {
                case MHL_CBUS_FEATURE_START:
                    SET_MHL_FEATURE_REQ_STATE_PORT0(MHL_CBUS_FEATURE_SEND_3D_INFORMATION);
                    SET_MHL_SEND_3D_INFORMATION_PORT0();
                    SET_MHL_SEND_3D_DTD_PORT0();
                    SET_MHL_SEND_3D_VIC_PORT0();

                    break;

                case MHL_CBUS_FEATURE_SEND_3D_INFORMATION:
                    if(!GET_MHL_SEND_3D_INFORMATION_PORT0())
                    {
                        SET_MHL_FEATURE_REQ_STATE_PORT0(MHL_CBUS_FEATURE_SEND_HIGH_END_VIDEO);
                        SET_MHL_SEND_HIGH_END_VIDEO0();
                        SET_MHL_SEND_HEV_VIC_FLAG0();
                        SET_MHL_SEND_HEV_DTDA_FLAG0();
                        SET_MHL_SEND_HEV_DTDB_FLAG0();
                    }

                    break;

                case MHL_CBUS_FEATURE_SEND_HIGH_END_VIDEO:
                    if(!GET_MHL_SEND_HIGH_END_VIDEO0())
                    {
                        SET_MHL_FEATURE_REQ_STATE_PORT0(MHL_CBUS_FEATURE_SEND_AUDIO_DATA_TUNNEL);
                        SET_MHL_SEND_AUDIO_DATA_TUNNEL0();
                    }

                    break;

                case MHL_CBUS_FEATURE_SEND_AUDIO_DATA_TUNNEL:
                    if(!GET_MHL_SEND_AUDIO_DATA_TUNNEL0())
                    {
                        SET_MHL_FEATURE_REQ_STATE_PORT0(MHL_CBUS_FEATURE_SEND_EMSC_DATA_TUNNEL);
                        SET_MHL_SEND_EMSC_DATA_TUNNEL0();
                    }

                    break;

                case MHL_CBUS_FEATURE_SEND_EMSC_DATA_TUNNEL:
                    if(!GET_MHL_SEND_EMSC_DATA_TUNNEL0())
                    {
                        if(GET_MHL_WRITE_BURST_STATE_PORT0() == MHL_CBUS_WRITE_BURST_NONE)
                        {
                            SET_MHL_FEATURE_REQ_STATE_PORT0(MHL_CBUS_FEATURE_SEND_FEATURE_COMPLETE);
                        }
                    }

                    break;

                case MHL_CBUS_FEATURE_SEND_FEATURE_COMPLETE:
                    if(_mdrv_mhl_CbusWriteStatusOrSetInt(ucPortSelect, 0x20, BIT6, FALSE))
                    {
                        SET_MHL_FEATURE_REQ_STATE_PORT0(MHL_CBUS_FEATURE_END);
                    }

                    break;

                case MHL_CBUS_FEATURE_END:
                    SET_MHL_FEATURE_REQ_STATE_PORT0(MHL_CBUS_FEATURE_NONE);
                    break;

                default:

                    break;
            };

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            switch(GET_MHL_FEATURE_REQ_STATE_PORT1())
            {
                case MHL_CBUS_FEATURE_START:
                    SET_MHL_FEATURE_REQ_STATE_PORT1(MHL_CBUS_FEATURE_SEND_3D_INFORMATION);
                    SET_MHL_SEND_3D_INFORMATION_PORT1();
                    SET_MHL_SEND_3D_DTD_PORT1();
                    SET_MHL_SEND_3D_VIC_PORT1();

                    break;

                case MHL_CBUS_FEATURE_SEND_3D_INFORMATION:
                    if(!GET_MHL_SEND_3D_INFORMATION_PORT1())
                    {
                        SET_MHL_FEATURE_REQ_STATE_PORT1(MHL_CBUS_FEATURE_SEND_HIGH_END_VIDEO);
                        SET_MHL_SEND_HIGH_END_VIDEO1();
                        SET_MHL_SEND_HEV_VIC_FLAG1();
                        SET_MHL_SEND_HEV_DTDA_FLAG1();
                        SET_MHL_SEND_HEV_DTDB_FLAG1();
                    }

                    break;

                case MHL_CBUS_FEATURE_SEND_HIGH_END_VIDEO:
                    if(!GET_MHL_SEND_HIGH_END_VIDEO1())
                    {
                        SET_MHL_FEATURE_REQ_STATE_PORT1(MHL_CBUS_FEATURE_SEND_AUDIO_DATA_TUNNEL);
                        SET_MHL_SEND_AUDIO_DATA_TUNNEL1();
                    }

                    break;

                case MHL_CBUS_FEATURE_SEND_AUDIO_DATA_TUNNEL:
                    if(!GET_MHL_SEND_AUDIO_DATA_TUNNEL1())
                    {
                        SET_MHL_FEATURE_REQ_STATE_PORT1(MHL_CBUS_FEATURE_SEND_EMSC_DATA_TUNNEL);
                        SET_MHL_SEND_EMSC_DATA_TUNNEL1();
                    }

                    break;

                case MHL_CBUS_FEATURE_SEND_EMSC_DATA_TUNNEL:
                    if(!GET_MHL_SEND_EMSC_DATA_TUNNEL1())
                    {
                        if(GET_MHL_WRITE_BURST_STATE_PORT1() == MHL_CBUS_WRITE_BURST_NONE)
                        {
                            SET_MHL_FEATURE_REQ_STATE_PORT1(MHL_CBUS_FEATURE_SEND_FEATURE_COMPLETE);
                        }
                    }

                    break;

                case MHL_CBUS_FEATURE_SEND_FEATURE_COMPLETE:
                    if(_mdrv_mhl_CbusWriteStatusOrSetInt(ucPortSelect, 0x20, BIT6, FALSE))
                    {
                        SET_MHL_FEATURE_REQ_STATE_PORT1(MHL_CBUS_FEATURE_END);
                    }

                    break;

                case MHL_CBUS_FEATURE_END:
                    SET_MHL_FEATURE_REQ_STATE_PORT1(MHL_CBUS_FEATURE_NONE);
                    break;

                default:

                    break;
            };

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            switch(GET_MHL_FEATURE_REQ_STATE_PORT2())
            {
                case MHL_CBUS_FEATURE_START:
                    SET_MHL_FEATURE_REQ_STATE_PORT2(MHL_CBUS_FEATURE_SEND_3D_INFORMATION);
                    SET_MHL_SEND_3D_INFORMATION_PORT2();
                    SET_MHL_SEND_3D_DTD_PORT2();
                    SET_MHL_SEND_3D_VIC_PORT2();

                    break;

                case MHL_CBUS_FEATURE_SEND_3D_INFORMATION:
                    if(!GET_MHL_SEND_3D_INFORMATION_PORT2())
                    {
                        SET_MHL_FEATURE_REQ_STATE_PORT2(MHL_CBUS_FEATURE_SEND_HIGH_END_VIDEO);
                        SET_MHL_SEND_HIGH_END_VIDEO2();
                        SET_MHL_SEND_HEV_VIC_FLAG2();
                        SET_MHL_SEND_HEV_DTDA_FLAG2();
                        SET_MHL_SEND_HEV_DTDB_FLAG2();
                    }

                    break;

                case MHL_CBUS_FEATURE_SEND_HIGH_END_VIDEO:
                    if(!GET_MHL_SEND_HIGH_END_VIDEO2())
                    {
                        SET_MHL_FEATURE_REQ_STATE_PORT2(MHL_CBUS_FEATURE_SEND_AUDIO_DATA_TUNNEL);
                        SET_MHL_SEND_AUDIO_DATA_TUNNEL2();
                    }

                    break;

                case MHL_CBUS_FEATURE_SEND_AUDIO_DATA_TUNNEL:
                    if(!GET_MHL_SEND_AUDIO_DATA_TUNNEL2())
                    {
                        SET_MHL_FEATURE_REQ_STATE_PORT2(MHL_CBUS_FEATURE_SEND_EMSC_DATA_TUNNEL);
                        SET_MHL_SEND_EMSC_DATA_TUNNEL2();
                    }

                    break;

                case MHL_CBUS_FEATURE_SEND_EMSC_DATA_TUNNEL:
                    if(!GET_MHL_SEND_EMSC_DATA_TUNNEL2())
                    {
                        if(GET_MHL_WRITE_BURST_STATE_PORT2() == MHL_CBUS_WRITE_BURST_NONE)
                        {
                            SET_MHL_FEATURE_REQ_STATE_PORT2(MHL_CBUS_FEATURE_SEND_FEATURE_COMPLETE);
                        }
                    }

                    break;

                case MHL_CBUS_FEATURE_SEND_FEATURE_COMPLETE:
                    if(_mdrv_mhl_CbusWriteStatusOrSetInt(ucPortSelect, 0x20, BIT6, FALSE))
                    {
                        SET_MHL_FEATURE_REQ_STATE_PORT2(MHL_CBUS_FEATURE_END);
                    }

                    break;

                case MHL_CBUS_FEATURE_END:
                    SET_MHL_FEATURE_REQ_STATE_PORT2(MHL_CBUS_FEATURE_NONE);
                    break;

                default:

                    break;
            };

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            switch(GET_MHL_FEATURE_REQ_STATE_PORT3())
            {
                case MHL_CBUS_FEATURE_START:
                    SET_MHL_FEATURE_REQ_STATE_PORT3(MHL_CBUS_FEATURE_SEND_3D_INFORMATION);
                    SET_MHL_SEND_3D_INFORMATION_PORT3();
                    SET_MHL_SEND_3D_DTD_PORT3();
                    SET_MHL_SEND_3D_VIC_PORT3();

                    break;

                case MHL_CBUS_FEATURE_SEND_3D_INFORMATION:
                    if(!GET_MHL_SEND_3D_INFORMATION_PORT3())
                    {
                        SET_MHL_FEATURE_REQ_STATE_PORT3(MHL_CBUS_FEATURE_SEND_HIGH_END_VIDEO);
                        SET_MHL_SEND_HIGH_END_VIDEO3();
                        SET_MHL_SEND_HEV_VIC_FLAG3();
                        SET_MHL_SEND_HEV_DTDA_FLAG3();
                        SET_MHL_SEND_HEV_DTDB_FLAG3();
                    }

                    break;

                case MHL_CBUS_FEATURE_SEND_HIGH_END_VIDEO:
                    if(!GET_MHL_SEND_HIGH_END_VIDEO3())
                    {
                        SET_MHL_FEATURE_REQ_STATE_PORT3(MHL_CBUS_FEATURE_SEND_AUDIO_DATA_TUNNEL);
                        SET_MHL_SEND_AUDIO_DATA_TUNNEL3();
                    }

                    break;

                case MHL_CBUS_FEATURE_SEND_AUDIO_DATA_TUNNEL:
                    if(!GET_MHL_SEND_AUDIO_DATA_TUNNEL3())
                    {
                        SET_MHL_FEATURE_REQ_STATE_PORT3(MHL_CBUS_FEATURE_SEND_EMSC_DATA_TUNNEL);
                        SET_MHL_SEND_EMSC_DATA_TUNNEL3();
                    }

                    break;

                case MHL_CBUS_FEATURE_SEND_EMSC_DATA_TUNNEL:
                    if(!GET_MHL_SEND_EMSC_DATA_TUNNEL3())
                    {
                        if(GET_MHL_WRITE_BURST_STATE_PORT3() == MHL_CBUS_WRITE_BURST_NONE)
                        {
                            SET_MHL_FEATURE_REQ_STATE_PORT3(MHL_CBUS_FEATURE_SEND_FEATURE_COMPLETE);
                        }
                    }

                    break;

                case MHL_CBUS_FEATURE_SEND_FEATURE_COMPLETE:
                    if(_mdrv_mhl_CbusWriteStatusOrSetInt(ucPortSelect, 0x20, BIT6, FALSE))
                    {
                        SET_MHL_FEATURE_REQ_STATE_PORT3(MHL_CBUS_FEATURE_END);
                    }

                    break;

                case MHL_CBUS_FEATURE_END:
                    SET_MHL_FEATURE_REQ_STATE_PORT3(MHL_CBUS_FEATURE_NONE);
                    break;

                default:

                    break;
            };

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            switch(GET_MHL_FEATURE_REQ_STATE_PORT4())
            {
                case MHL_CBUS_FEATURE_START:
                    SET_MHL_FEATURE_REQ_STATE_PORT4(MHL_CBUS_FEATURE_SEND_3D_INFORMATION);
                    SET_MHL_SEND_3D_INFORMATION_PORT4();
                    SET_MHL_SEND_3D_DTD_PORT4();
                    SET_MHL_SEND_3D_VIC_PORT4();

                    break;

                case MHL_CBUS_FEATURE_SEND_3D_INFORMATION:
                    if(!GET_MHL_SEND_3D_INFORMATION_PORT4())
                    {
                        SET_MHL_FEATURE_REQ_STATE_PORT4(MHL_CBUS_FEATURE_SEND_HIGH_END_VIDEO);
                        SET_MHL_SEND_HIGH_END_VIDEO4();
                        SET_MHL_SEND_HEV_VIC_FLAG4();
                        SET_MHL_SEND_HEV_DTDA_FLAG4();
                        SET_MHL_SEND_HEV_DTDB_FLAG4();
                    }

                    break;

                case MHL_CBUS_FEATURE_SEND_HIGH_END_VIDEO:
                    if(!GET_MHL_SEND_HIGH_END_VIDEO4())
                    {
                        SET_MHL_FEATURE_REQ_STATE_PORT4(MHL_CBUS_FEATURE_SEND_AUDIO_DATA_TUNNEL);
                        SET_MHL_SEND_AUDIO_DATA_TUNNEL4();
                    }

                    break;

                case MHL_CBUS_FEATURE_SEND_AUDIO_DATA_TUNNEL:
                    if(!GET_MHL_SEND_AUDIO_DATA_TUNNEL4())
                    {
                        SET_MHL_FEATURE_REQ_STATE_PORT4(MHL_CBUS_FEATURE_SEND_EMSC_DATA_TUNNEL);
                        SET_MHL_SEND_EMSC_DATA_TUNNEL4();
                    }

                    break;

                case MHL_CBUS_FEATURE_SEND_EMSC_DATA_TUNNEL:
                    if(!GET_MHL_SEND_EMSC_DATA_TUNNEL4())
                    {
                        if(GET_MHL_WRITE_BURST_STATE_PORT4() == MHL_CBUS_WRITE_BURST_NONE)
                        {
                            SET_MHL_FEATURE_REQ_STATE_PORT4(MHL_CBUS_FEATURE_SEND_FEATURE_COMPLETE);
                        }
                    }

                    break;

                case MHL_CBUS_FEATURE_SEND_FEATURE_COMPLETE:
                    if(_mdrv_mhl_CbusWriteStatusOrSetInt(ucPortSelect, 0x20, BIT6, FALSE))
                    {
                        SET_MHL_FEATURE_REQ_STATE_PORT4(MHL_CBUS_FEATURE_END);
                    }

                    break;

                case MHL_CBUS_FEATURE_END:
                    SET_MHL_FEATURE_REQ_STATE_PORT4(MHL_CBUS_FEATURE_NONE);
                    break;

                default:

                    break;
            };

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            switch(GET_MHL_FEATURE_REQ_STATE_PORT5())
            {
                case MHL_CBUS_FEATURE_START:
                    SET_MHL_FEATURE_REQ_STATE_PORT5(MHL_CBUS_FEATURE_SEND_3D_INFORMATION);
                    SET_MHL_SEND_3D_INFORMATION_PORT5();
                    SET_MHL_SEND_3D_DTD_PORT5();
                    SET_MHL_SEND_3D_VIC_PORT5();

                    break;

                case MHL_CBUS_FEATURE_SEND_3D_INFORMATION:
                    if(!GET_MHL_SEND_3D_INFORMATION_PORT5())
                    {
                        SET_MHL_FEATURE_REQ_STATE_PORT5(MHL_CBUS_FEATURE_SEND_HIGH_END_VIDEO);
                        SET_MHL_SEND_HIGH_END_VIDEO5();
                        SET_MHL_SEND_HEV_VIC_FLAG5();
                        SET_MHL_SEND_HEV_DTDA_FLAG5();
                        SET_MHL_SEND_HEV_DTDB_FLAG5();
                    }

                    break;

                case MHL_CBUS_FEATURE_SEND_HIGH_END_VIDEO:
                    if(!GET_MHL_SEND_HIGH_END_VIDEO5())
                    {
                        SET_MHL_FEATURE_REQ_STATE_PORT5(MHL_CBUS_FEATURE_SEND_AUDIO_DATA_TUNNEL);
                        SET_MHL_SEND_AUDIO_DATA_TUNNEL5();
                    }

                    break;

                case MHL_CBUS_FEATURE_SEND_AUDIO_DATA_TUNNEL:
                    if(!GET_MHL_SEND_AUDIO_DATA_TUNNEL5())
                    {
                        SET_MHL_FEATURE_REQ_STATE_PORT5(MHL_CBUS_FEATURE_SEND_EMSC_DATA_TUNNEL);
                        SET_MHL_SEND_EMSC_DATA_TUNNEL5();
                    }

                    break;

                case MHL_CBUS_FEATURE_SEND_EMSC_DATA_TUNNEL:
                    if(!GET_MHL_SEND_EMSC_DATA_TUNNEL5())
                    {
                        if(GET_MHL_WRITE_BURST_STATE_PORT5() == MHL_CBUS_WRITE_BURST_NONE)
                        {
                            SET_MHL_FEATURE_REQ_STATE_PORT5(MHL_CBUS_FEATURE_SEND_FEATURE_COMPLETE);
                        }
                    }

                    break;

                case MHL_CBUS_FEATURE_SEND_FEATURE_COMPLETE:
                    if(_mdrv_mhl_CbusWriteStatusOrSetInt(ucPortSelect, 0x20, BIT6, FALSE))
                    {
                        SET_MHL_FEATURE_REQ_STATE_PORT5(MHL_CBUS_FEATURE_END);
                    }

                    break;

                case MHL_CBUS_FEATURE_END:
                    SET_MHL_FEATURE_REQ_STATE_PORT5(MHL_CBUS_FEATURE_NONE);
                    break;

                default:

                    break;
            };

            break;
#endif

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_GetDTDBurstfor3D()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool _mdrv_mhl_GetDTDBurstfor3D(BYTE ucPortSelect, BYTE *uc3DSequence, BYTE *pData)
{
    Bool bFinish = FALSE;
    BYTE uctemp = 0;
    BYTE ucDTDCount = 0;
    BYTE ucSequence = 0;
    BYTE ucMHL3DTotalEntryDTD = 0;
    BYTE *ucMHL3DInformationDTD = 0;

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            ucSequence = GET_MHL_WRITE_BURST_SEQUENCE0();
            ucMHL3DTotalEntryDTD = GET_MHL_3D_TOT_ENT_DTD_PORT0();
            ucMHL3DInformationDTD = &GET_MHL_3D_DTD_INFORMATION0(0);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            ucSequence = GET_MHL_WRITE_BURST_SEQUENCE1();
            ucMHL3DTotalEntryDTD = GET_MHL_3D_TOT_ENT_DTD_PORT1();
            ucMHL3DInformationDTD = &GET_MHL_3D_DTD_INFORMATION1(0);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            ucSequence = GET_MHL_WRITE_BURST_SEQUENCE2();
            ucMHL3DTotalEntryDTD = GET_MHL_3D_TOT_ENT_DTD_PORT2();
            ucMHL3DInformationDTD = &GET_MHL_3D_DTD_INFORMATION2(0);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            ucSequence = GET_MHL_WRITE_BURST_SEQUENCE3();
            ucMHL3DTotalEntryDTD = GET_MHL_3D_TOT_ENT_DTD_PORT3();
            ucMHL3DInformationDTD = &GET_MHL_3D_DTD_INFORMATION3(0);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            ucSequence = GET_MHL_WRITE_BURST_SEQUENCE4();
            ucMHL3DTotalEntryDTD = GET_MHL_3D_TOT_ENT_DTD_PORT4();
            ucMHL3DInformationDTD = &GET_MHL_3D_DTD_INFORMATION4(0);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            ucSequence = GET_MHL_WRITE_BURST_SEQUENCE5();
            ucMHL3DTotalEntryDTD = GET_MHL_3D_TOT_ENT_DTD_PORT5();
            ucMHL3DInformationDTD = &GET_MHL_3D_DTD_INFORMATION5(0);
            break;
#endif

        default:

            break;
    };

    ucDTDCount = ucSequence *MHL_3D_SEQUENCE_LENGTH;

    pData[0] = (MHL_BURST_ID_3D_DTD >> 8);
    pData[1] = (MHL_BURST_ID_3D_DTD &MASKBIT(7:0));
    pData[3] = ucMHL3DTotalEntryDTD;

    if(ucMHL3DTotalEntryDTD == 0)
    {
        bFinish = TRUE;
    }

    for(uctemp = 0; uctemp < MHL_3D_SEQUENCE_LENGTH; uctemp++)
    {
        if((!bFinish) && ((ucDTDCount +uctemp) == (ucMHL3DTotalEntryDTD -1)))
        {
            bFinish = TRUE;
        }

        if((ucDTDCount +uctemp) < ucMHL3DTotalEntryDTD)
        {
            pData[6 +uctemp *MHL_3D_ITEM_LENGTH] = 0;
            pData[6 +uctemp *MHL_3D_ITEM_LENGTH +1] = ucMHL3DInformationDTD[ucDTDCount +uctemp] &MASKBIT(3:0);
        }
        else
        {
            pData[6 +uctemp *MHL_3D_ITEM_LENGTH] = 0;
            pData[6 +uctemp *MHL_3D_ITEM_LENGTH +1] = 0;
        }
    }

    ucSequence++;

    pData[4] = ucSequence;

    if(bFinish)
    {
        pData[5] = ucMHL3DTotalEntryDTD -ucDTDCount;
        ucSequence = 0;
    }
    else
    {
        pData[5] = MHL_3D_SEQUENCE_LENGTH;
    }

    pData[2] = _mdrv_mhl_WriteBurstCheckSum(pData);

    *uc3DSequence = ucSequence;

    return bFinish;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_GetVICBurstfor3D()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool _mdrv_mhl_GetVICBurstfor3D(BYTE ucPortSelect, BYTE *uc3DSequence, BYTE *pData)
{
    Bool bFinish = FALSE;
    BYTE uctemp = 0;
    BYTE ucVIDCount = 0;
    BYTE ucSequence = 0;
    BYTE ucMHL3DTotalEntryVIC = 0;
    BYTE *ucMHL3DInformationVIC = 0;

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            ucSequence = GET_MHL_WRITE_BURST_SEQUENCE0();
            ucMHL3DTotalEntryVIC = GET_MHL_3D_TOT_ENT_VIC_PORT0();
            ucMHL3DInformationVIC = &GET_MHL_3D_VIC_INFORMATION0(0);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            ucSequence = GET_MHL_WRITE_BURST_SEQUENCE1();
            ucMHL3DTotalEntryVIC = GET_MHL_3D_TOT_ENT_VIC_PORT1();
            ucMHL3DInformationVIC = &GET_MHL_3D_VIC_INFORMATION1(0);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            ucSequence = GET_MHL_WRITE_BURST_SEQUENCE2();
            ucMHL3DTotalEntryVIC = GET_MHL_3D_TOT_ENT_VIC_PORT2();
            ucMHL3DInformationVIC = &GET_MHL_3D_VIC_INFORMATION2(0);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            ucSequence = GET_MHL_WRITE_BURST_SEQUENCE3();
            ucMHL3DTotalEntryVIC = GET_MHL_3D_TOT_ENT_VIC_PORT3();
            ucMHL3DInformationVIC = &GET_MHL_3D_VIC_INFORMATION3(0);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            ucSequence = GET_MHL_WRITE_BURST_SEQUENCE4();
            ucMHL3DTotalEntryVIC = GET_MHL_3D_TOT_ENT_VIC_PORT4();
            ucMHL3DInformationVIC = &GET_MHL_3D_VIC_INFORMATION4(0);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            ucSequence = GET_MHL_WRITE_BURST_SEQUENCE5();
            ucMHL3DTotalEntryVIC = GET_MHL_3D_TOT_ENT_VIC_PORT5();
            ucMHL3DInformationVIC = &GET_MHL_3D_VIC_INFORMATION5(0);
            break;
#endif

        default:

            break;
    };

    ucVIDCount = ucSequence *MHL_3D_SEQUENCE_LENGTH;

    pData[0] = (MHL_BURST_ID_3D_VIC >> 8);
    pData[1] = (MHL_BURST_ID_3D_VIC &MASKBIT(7:0));
    pData[3] = ucMHL3DTotalEntryVIC;

    if(ucMHL3DTotalEntryVIC == 0)
    {
        bFinish = TRUE;
    }

    for(uctemp = 0; uctemp < MHL_3D_SEQUENCE_LENGTH; uctemp++)
    {
        if((!bFinish) && ((ucVIDCount +uctemp) == (ucMHL3DTotalEntryVIC -1)))
        {
            bFinish = TRUE;
        }

        if((ucVIDCount +uctemp) < ucMHL3DTotalEntryVIC)
        {
            pData[6 +uctemp *MHL_3D_ITEM_LENGTH] = 0;
            pData[6 +uctemp *MHL_3D_ITEM_LENGTH +1] = ucMHL3DInformationVIC[ucVIDCount +uctemp] &MASKBIT(3:0);
        }
        else
        {
            pData[6 +uctemp *MHL_3D_ITEM_LENGTH] = 0;
            pData[6 +uctemp *MHL_3D_ITEM_LENGTH +1] = 0;
        }
    }

    ucSequence++;

    pData[4] = ucSequence;

    if(bFinish)
    {
        pData[5] = ucMHL3DTotalEntryVIC -ucVIDCount;
        ucSequence = 0;
    }
    else
    {
        pData[5] = MHL_3D_SEQUENCE_LENGTH;
    }

    pData[2] = _mdrv_mhl_WriteBurstCheckSum(pData);

    *uc3DSequence = ucSequence;

    return bFinish;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_Send3DTimingInformation()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_Send3DTimingInformation(BYTE ucPortSelect)
{
    Bool bFinish = FALSE;
    BYTE uc3DSequence = 0;
    BYTE ucData[MHL_MSC_SCRATCHPAD_SIZE] = {0};

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            if(GET_MHL_SEND_3D_DTD_PORT0())
            {
                if(_mdrv_mhl_GetDTDBurstfor3D(ucPortSelect, &uc3DSequence, ucData))
                {
                    bFinish = TRUE;
                }

                if(mdrv_mhl_CbusSendWriteBurst(ucPortSelect, 0x40, MHL_MSC_SCRATCHPAD_SIZE, ucData))
                {
                    if(bFinish)
                    {
                        CLR_MHL_SEND_3D_DTD_PORT0();
                    }

                    SET_MHL_WRITE_BURST_SEQUENCE0(uc3DSequence);
                    SET_MHL_WRITE_BURST_STATE_PORT0(MHL_CBUS_WRITE_BURST_SEND_DATA);

                    MHL_DRV_DPRINTF("** MHL sent 3D DTD in sequence %d port 0", ucData[4]);
                }
            }
            else if(GET_MHL_SEND_3D_VIC_PORT0())
            {
                if(_mdrv_mhl_GetVICBurstfor3D(ucPortSelect, &uc3DSequence, ucData))
                {
                    bFinish = TRUE;
                }

                if(mdrv_mhl_CbusSendWriteBurst(ucPortSelect, 0x40, MHL_MSC_SCRATCHPAD_SIZE, ucData))
                {
                    if(bFinish)
                    {
                        CLR_MHL_SEND_3D_VIC_PORT0();
                        CLR_MHL_SEND_3D_INFORMATION_PORT0();
                    }

                    SET_MHL_WRITE_BURST_SEQUENCE0(uc3DSequence);
                    SET_MHL_WRITE_BURST_STATE_PORT0(MHL_CBUS_WRITE_BURST_SEND_DATA);

                    MHL_DRV_DPRINTF("** MHL sent 3D VIC in sequence %d port 0", ucData[4]);
                }
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            if(GET_MHL_SEND_3D_DTD_PORT1())
            {
                if(_mdrv_mhl_GetDTDBurstfor3D(ucPortSelect, &uc3DSequence, ucData))
                {
                    bFinish = TRUE;
                }

                if(mdrv_mhl_CbusSendWriteBurst(ucPortSelect, 0x40, MHL_MSC_SCRATCHPAD_SIZE, ucData))
                {
                    if(bFinish)
                    {
                        CLR_MHL_SEND_3D_DTD_PORT1();
                    }

                    SET_MHL_WRITE_BURST_SEQUENCE1(uc3DSequence);
                    SET_MHL_WRITE_BURST_STATE_PORT1(MHL_CBUS_WRITE_BURST_SEND_DATA);

                    MHL_DRV_DPRINTF("** MHL sent 3D DTD in sequence %d port 1", ucData[4]);
                }
            }
            else if(GET_MHL_SEND_3D_VIC_PORT1())
            {
                if(_mdrv_mhl_GetVICBurstfor3D(ucPortSelect, &uc3DSequence, ucData))
                {
                    bFinish = TRUE;
                }

                if(mdrv_mhl_CbusSendWriteBurst(ucPortSelect, 0x40, MHL_MSC_SCRATCHPAD_SIZE, ucData))
                {
                    if(bFinish)
                    {
                        CLR_MHL_SEND_3D_VIC_PORT1();
                        CLR_MHL_SEND_3D_INFORMATION_PORT1();
                    }

                    SET_MHL_WRITE_BURST_SEQUENCE1(uc3DSequence);
                    SET_MHL_WRITE_BURST_STATE_PORT1(MHL_CBUS_WRITE_BURST_SEND_DATA);

                    MHL_DRV_DPRINTF("** MHL sent 3D VIC in sequence %d port 1", ucData[4]);
                }
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            if(GET_MHL_SEND_3D_DTD_PORT2())
            {
                if(_mdrv_mhl_GetDTDBurstfor3D(ucPortSelect, &uc3DSequence, ucData))
                {
                    bFinish= TRUE;
                }

                if(mdrv_mhl_CbusSendWriteBurst(ucPortSelect, 0x40, MHL_MSC_SCRATCHPAD_SIZE, ucData))
                {
                    if(bFinish)
                    {
                        CLR_MHL_SEND_3D_DTD_PORT2();
                    }

                    SET_MHL_WRITE_BURST_SEQUENCE2(uc3DSequence);
                    SET_MHL_WRITE_BURST_STATE_PORT2(MHL_CBUS_WRITE_BURST_SEND_DATA);

                    MHL_DRV_DPRINTF("** MHL sent 3D DTD in sequence %d port 2", ucData[4]);
                }
            }
            else if(GET_MHL_SEND_3D_VIC_PORT2())
            {
                if(_mdrv_mhl_GetVICBurstfor3D(ucPortSelect, &uc3DSequence, ucData))
                {
                    bFinish= TRUE;
                }

                if(mdrv_mhl_CbusSendWriteBurst(ucPortSelect, 0x40, MHL_MSC_SCRATCHPAD_SIZE, ucData))
                {
                    if(bFinish)
                    {
                        CLR_MHL_SEND_3D_VIC_PORT2();
                        CLR_MHL_SEND_3D_INFORMATION_PORT2();
                    }

                    SET_MHL_WRITE_BURST_SEQUENCE2(uc3DSequence);
                    SET_MHL_WRITE_BURST_STATE_PORT2(MHL_CBUS_WRITE_BURST_SEND_DATA);

                    MHL_DRV_DPRINTF("** MHL sent 3D VIC in sequence %d port 2", ucData[4]);
                }
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            if(GET_MHL_SEND_3D_DTD_PORT3())
            {
                if(_mdrv_mhl_GetDTDBurstfor3D(ucPortSelect, &uc3DSequence, ucData))
                {
                    bFinish= TRUE;
                }

                if(mdrv_mhl_CbusSendWriteBurst(ucPortSelect, 0x40, MHL_MSC_SCRATCHPAD_SIZE, ucData))
                {
                    if(bFinish)
                    {
                        CLR_MHL_SEND_3D_DTD_PORT3();
                    }

                    SET_MHL_WRITE_BURST_SEQUENCE3(uc3DSequence);
                    SET_MHL_WRITE_BURST_STATE_PORT3(MHL_CBUS_WRITE_BURST_SEND_DATA);

                    MHL_DRV_DPRINTF("** MHL sent 3D DTD in sequence %d port 3", ucData[4]);
                }
            }
            else if(GET_MHL_SEND_3D_VIC_PORT3())
            {
                if(_mdrv_mhl_GetVICBurstfor3D(ucPortSelect, &uc3DSequence, ucData))
                {
                    bFinish= TRUE;
                }

                if(mdrv_mhl_CbusSendWriteBurst(ucPortSelect, 0x40, MHL_MSC_SCRATCHPAD_SIZE, ucData))
                {
                    if(bFinish)
                    {
                        CLR_MHL_SEND_3D_VIC_PORT3();
                        CLR_MHL_SEND_3D_INFORMATION_PORT3();
                    }

                    SET_MHL_WRITE_BURST_SEQUENCE3(uc3DSequence);
                    SET_MHL_WRITE_BURST_STATE_PORT3(MHL_CBUS_WRITE_BURST_SEND_DATA);

                    MHL_DRV_DPRINTF("** MHL sent 3D VIC in sequence %d port 3", ucData[4]);
                }
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            if(GET_MHL_SEND_3D_DTD_PORT4())
            {
                if(_mdrv_mhl_GetDTDBurstfor3D(ucPortSelect, &uc3DSequence, ucData))
                {
                    bFinish= TRUE;
                }

                if(mdrv_mhl_CbusSendWriteBurst(ucPortSelect, 0x40, MHL_MSC_SCRATCHPAD_SIZE, ucData))
                {
                    if(bFinish)
                    {
                        CLR_MHL_SEND_3D_DTD_PORT4();
                    }

                    SET_MHL_WRITE_BURST_SEQUENCE4(uc3DSequence);
                    SET_MHL_WRITE_BURST_STATE_PORT4(MHL_CBUS_WRITE_BURST_SEND_DATA);

                    MHL_DRV_DPRINTF("** MHL sent 3D DTD in sequence %d port 4", ucData[4]);
                }
            }
            else if(GET_MHL_SEND_3D_VIC_PORT4())
            {
                if(_mdrv_mhl_GetVICBurstfor3D(ucPortSelect, &uc3DSequence, ucData))
                {
                    bFinish= TRUE;
                }

                if(mdrv_mhl_CbusSendWriteBurst(ucPortSelect, 0x40, MHL_MSC_SCRATCHPAD_SIZE, ucData))
                {
                    if(bFinish)
                    {
                        CLR_MHL_SEND_3D_VIC_PORT4();
                        CLR_MHL_SEND_3D_INFORMATION_PORT4();
                    }

                    SET_MHL_WRITE_BURST_SEQUENCE4(uc3DSequence);
                    SET_MHL_WRITE_BURST_STATE_PORT4(MHL_CBUS_WRITE_BURST_SEND_DATA);

                    MHL_DRV_DPRINTF("** MHL sent 3D VIC in sequence %d port 4", ucData[4]);
                }
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            if(GET_MHL_SEND_3D_DTD_PORT5())
            {
                if(_mdrv_mhl_GetDTDBurstfor3D(ucPortSelect, &uc3DSequence, ucData))
                {
                    bFinish= TRUE;
                }

                if(mdrv_mhl_CbusSendWriteBurst(ucPortSelect, 0x40, MHL_MSC_SCRATCHPAD_SIZE, ucData))
                {
                    if(bFinish)
                    {
                        CLR_MHL_SEND_3D_DTD_PORT5();
                    }

                    SET_MHL_WRITE_BURST_SEQUENCE5(uc3DSequence);
                    SET_MHL_WRITE_BURST_STATE_PORT5(MHL_CBUS_WRITE_BURST_SEND_DATA);

                    MHL_DRV_DPRINTF("** MHL sent 3D DTD in sequence %d port 5", ucData[4]);
                }
            }
            else if(GET_MHL_SEND_3D_VIC_PORT5())
            {
                if(_mdrv_mhl_GetVICBurstfor3D(ucPortSelect, &uc3DSequence, ucData))
                {
                    bFinish= TRUE;
                }

                if(mdrv_mhl_CbusSendWriteBurst(ucPortSelect, 0x40, MHL_MSC_SCRATCHPAD_SIZE, ucData))
                {
                    if(bFinish)
                    {
                        CLR_MHL_SEND_3D_VIC_PORT5();
                        CLR_MHL_SEND_3D_INFORMATION_PORT5();
                    }

                    SET_MHL_WRITE_BURST_SEQUENCE5(uc3DSequence);
                    SET_MHL_WRITE_BURST_STATE_PORT5(MHL_CBUS_WRITE_BURST_SEND_DATA);

                    MHL_DRV_DPRINTF("** MHL sent 3D VIC in sequence %d port 5", ucData[4]);
                }
            }

            break;
#endif

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_GetBurstforVirtualChannel()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool _mdrv_mhl_GetBurstforVirtualChannel(BYTE ucPortSelect, Bool bVCConfirm, BYTE *ucVCSequence, BYTE *pData)
{
    Bool bFinish = FALSE;
    BYTE uctemp = 0;
    BYTE ucVCCount = 0;
    BYTE ucSequence = 0;
    BYTE ucVCTotalEntry = 0;
    BYTE ucVCFinishEntry = 0;
    WORD usWriteBurstID = MHL_BURST_ID_VC_ASSIGN;
    ST_VIRTUAL_CHANNEL_INFO *ucMHLVCInformation = 0;

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            ucSequence = GET_MHL_WRITE_BURST_SEQUENCE0();
            ucMHLVCInformation = &GET_MHL_VC_INFORMATION_PORT0(0);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            ucSequence = GET_MHL_WRITE_BURST_SEQUENCE1();
            ucMHLVCInformation = &GET_MHL_VC_INFORMATION_PORT1(0);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            ucSequence = GET_MHL_WRITE_BURST_SEQUENCE2();
            ucMHLVCInformation = &GET_MHL_VC_INFORMATION_PORT2(0);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            ucSequence = GET_MHL_WRITE_BURST_SEQUENCE3();
            ucMHLVCInformation = &GET_MHL_VC_INFORMATION_PORT3(0);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            ucSequence = GET_MHL_WRITE_BURST_SEQUENCE4();
            ucMHLVCInformation = &GET_MHL_VC_INFORMATION_PORT4(0);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            ucSequence = GET_MHL_WRITE_BURST_SEQUENCE5();
            ucMHLVCInformation = &GET_MHL_VC_INFORMATION_PORT5(0);
            break;
#endif

        default:

            break;
    };

    if(bVCConfirm)
    {
        usWriteBurstID = MHL_BURST_ID_VC_CONFIRM;
    }

    ucVCFinishEntry = ucSequence *MHL_VC_SEQUENCE_LENGTH;

    pData[0] = (usWriteBurstID >> 8);
    pData[1] = (usWriteBurstID &MASKBIT(7:0));
    pData[15] = 0;

    for(uctemp = 0; uctemp < MHL_VIRTUAL_CHANNEL_LENGTH; uctemp++)
    {
        if(ucMHLVCInformation[uctemp].bVCConfirm)
        {
            ucVCTotalEntry++;

            if(ucVCFinishEntry > 0)
            {
                ucVCFinishEntry--;
            }
            else if(ucVCCount < MHL_VC_SEQUENCE_LENGTH)
            {
                pData[6 +ucVCCount *MHL_VC_ITEM_LENGTH] = ucMHLVCInformation[uctemp].ucVCNumber;
                pData[6 +ucVCCount *MHL_VC_ITEM_LENGTH +1] = ucMHLVCInformation[uctemp].ucVCFeatureID;

                if(bVCConfirm)
                {
                    pData[6 +ucVCCount *MHL_VC_ITEM_LENGTH +2] = ucMHLVCInformation[uctemp].ucVCResponseCode;
                }
                else
                {
                    pData[6 +ucVCCount *MHL_VC_ITEM_LENGTH +2] = ucMHLVCInformation[uctemp].ucVCSlotSize;
                }

                ucVCCount++;
            }
        }
    }

    ucSequence++;

    pData[3] = ucVCTotalEntry;
    pData[4] = ucSequence;
    pData[5] = ucVCCount;
    pData[2] = _mdrv_mhl_WriteBurstCheckSum(pData);

    if(((ucSequence -1) *MHL_VC_SEQUENCE_LENGTH +ucVCCount) == ucVCTotalEntry)
    {
        bFinish = TRUE;
        ucSequence = 0;
    }

    *ucVCSequence = ucSequence;

    return bFinish;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_SlotAllocationAlgorithm()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_SlotAllocationAlgorithm(BYTE ucChannelIndex, BYTE ucNValue, BYTE ucMValue, BYTE *pSlotAllocation)
{
    BYTE uctemp = 0;
    BYTE ucCounter = 0;

    for(uctemp = 0; uctemp <MHL_ECBUS_S_TIME_SLOT_TOTAL; uctemp++)
    {
        if(pSlotAllocation[uctemp] == MHL_CBUS_SLOT_NOT_ALLOCATE)
        {
            if(ucCounter >= ucNValue)
            {
                pSlotAllocation[uctemp] = ucChannelIndex;
            }

            ucCounter = (ucCounter +ucNValue) %(ucNValue +ucMValue);
        }
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_eCbusTimeSlotAllocation()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_eCbusTimeSlotAllocation(BYTE ucPortSelect)
{
    BYTE uctemp = 0;
    BYTE ucTimeSlotNumber = MHL_ECBUS_S_TIME_SLOT_TOTAL -1;
    BYTE ucSlotAllocation[MHL_ECBUS_S_TIME_SLOT_TOTAL];
    BYTE ucVirtualChannel[MHL_VIRTUAL_CHANNEL_LENGTH +2];
    ST_VIRTUAL_CHANNEL_INFO *ucMHLVCInformation = 0;

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            ucMHLVCInformation = &GET_MHL_VC_INFORMATION_PORT0(0);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            ucMHLVCInformation = &GET_MHL_VC_INFORMATION_PORT1(0);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            ucMHLVCInformation = &GET_MHL_VC_INFORMATION_PORT2(0);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            ucMHLVCInformation = &GET_MHL_VC_INFORMATION_PORT3(0);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            ucMHLVCInformation = &GET_MHL_VC_INFORMATION_PORT4(0);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            ucMHLVCInformation = &GET_MHL_VC_INFORMATION_PORT5(0);
            break;
#endif

        default:

            break;
    };

    for(uctemp = 0; uctemp <(MHL_VIRTUAL_CHANNEL_LENGTH +2); uctemp++)
    {
        ucVirtualChannel[uctemp] = 0;
    }

    for(uctemp = 0; uctemp <MHL_ECBUS_S_TIME_SLOT_TOTAL; uctemp++)
    {
        if(uctemp == 0)
        {
            ucSlotAllocation[uctemp] = 0;
        }
        else
        {
            ucSlotAllocation[uctemp] = MHL_CBUS_SLOT_NOT_ALLOCATE;
        }
    }

    ucVirtualChannel[0] = MHL_CBUS1_TIME_SLOT_DEFAULT;

    for(uctemp = 0; uctemp <MHL_VIRTUAL_CHANNEL_LENGTH; uctemp++)
    {
        if(ucMHLVCInformation[uctemp].ucVCSlotSize == 0)
        {
            break;
        }
        else
        {
            ucVirtualChannel[uctemp +1] = ucMHLVCInformation[uctemp].ucVCSlotSize;
            ucTimeSlotNumber = ucTimeSlotNumber -ucMHLVCInformation[uctemp].ucVCSlotSize;
        }
    }

    ucVirtualChannel[uctemp +1] = ucTimeSlotNumber;

    for(uctemp = 0; uctemp <(MHL_VIRTUAL_CHANNEL_LENGTH +2); uctemp++)
    {
        MHL_DRV_DPRINTF("** MHL time slot assign %d", ucVirtualChannel[uctemp]);
    }

    ucTimeSlotNumber = MHL_ECBUS_S_TIME_SLOT_TOTAL -1;

    for(uctemp = MHL_VIRTUAL_CHANNEL_LENGTH +1; ucTimeSlotNumber > 0; uctemp--)
    {
        if(ucVirtualChannel[uctemp] > 0)
        {
            ucTimeSlotNumber = ucTimeSlotNumber -ucVirtualChannel[uctemp];

            _mdrv_mhl_SlotAllocationAlgorithm(uctemp, ucTimeSlotNumber, ucVirtualChannel[uctemp], ucSlotAllocation);
        }
    }

    for(uctemp = 0; uctemp <MHL_ECBUS_S_TIME_SLOT_TOTAL; uctemp++)
    {
        MHL_DRV_DPRINTF("** MHL time slot allocation %d", ucSlotAllocation[uctemp]);
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_SortingVCInformation()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_SortingVCInformation(BYTE ucPortSelect, Bool bClearFlag)
{
    BYTE uctemp = 0;
    BYTE ucIndex = 0;
    BYTE ucTargetVC = MHL_VIRTUAL_CHANNEL_LENGTH;
    ST_VIRTUAL_CHANNEL_INFO stVirtualChannelTemp = {FALSE, 0, 0, 0, 0};
    ST_VIRTUAL_CHANNEL_INFO *ucMHLVCInformation = 0;

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            ucMHLVCInformation = &GET_MHL_VC_INFORMATION_PORT0(0);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            ucMHLVCInformation = &GET_MHL_VC_INFORMATION_PORT1(0);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            ucMHLVCInformation = &GET_MHL_VC_INFORMATION_PORT2(0);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            ucMHLVCInformation = &GET_MHL_VC_INFORMATION_PORT3(0);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            ucMHLVCInformation = &GET_MHL_VC_INFORMATION_PORT4(0);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            ucMHLVCInformation = &GET_MHL_VC_INFORMATION_PORT5(0);
            break;
#endif

        default:

            break;
    };

    for(ucIndex = 0; ucIndex <(ucTargetVC -1); ucIndex++)
    {
        for(uctemp = 0; uctemp <(ucTargetVC -ucIndex -1); uctemp++)
        {
            if(ucMHLVCInformation[uctemp +1].ucVCNumber == 0)
            {
                ucTargetVC = uctemp +1;

                break;
            }
            else
            {
                if(ucMHLVCInformation[uctemp].ucVCSlotSize == 0)
                {
                    ucMHLVCInformation[uctemp].ucVCNumber = 0xFF;
                }

                if(ucMHLVCInformation[uctemp +1].ucVCNumber < ucMHLVCInformation[uctemp].ucVCNumber)
                {
                    memcpy(&stVirtualChannelTemp, &ucMHLVCInformation[uctemp +1], sizeof(ST_VIRTUAL_CHANNEL_INFO));
                    memcpy(&ucMHLVCInformation[uctemp +1], &ucMHLVCInformation[uctemp], sizeof(ST_VIRTUAL_CHANNEL_INFO));
                    memcpy(&ucMHLVCInformation[uctemp], &stVirtualChannelTemp, sizeof(ST_VIRTUAL_CHANNEL_INFO));
                }
            }
        }
    }

    for(uctemp = 0; uctemp <MHL_VIRTUAL_CHANNEL_LENGTH; uctemp++)
    {
        if(ucMHLVCInformation[uctemp].ucVCSlotSize == 0)
        {
            ucMHLVCInformation[uctemp].ucVCNumber = 0;
        }

        if(bClearFlag)
        {
            ucMHLVCInformation[uctemp].bVCConfirm = FALSE;
        }

        MHL_DRV_DPRINTF("** MHL VC sorting number %d", ucMHLVCInformation[uctemp].ucVCNumber);
        MHL_DRV_DPRINTF("** MHL VC sorting slot %d", ucMHLVCInformation[uctemp].ucVCSlotSize);
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_SendVirtualChannelInformation()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_SendVirtualChannelInformation(BYTE ucPortSelect)
{
    Bool bFinish = FALSE;
    Bool bSendVCConfirm = FALSE;
    BYTE ucVCSequence = 0;
    BYTE ucData[MHL_MSC_SCRATCHPAD_SIZE] = {0};

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            if(_mdrv_mhl_GetBurstforVirtualChannel(ucPortSelect, GET_MHL_SEND_VC_CONFIRM_PORT0(), &ucVCSequence, ucData))
            {
                bFinish = TRUE;
            }

            if(mdrv_mhl_CbusSendWriteBurst(ucPortSelect, 0x40, MHL_MSC_SCRATCHPAD_SIZE, ucData))
            {
                if(bFinish)
                {
                    if(GET_MHL_SEND_VC_CONFIRM_PORT0())
                    {
                        CLR_MHL_SEND_VC_CONFIRM_PORT0();

                        bSendVCConfirm = TRUE;
                    }

                    CLR_MHL_SEND_VC_INFORMATION_PORT0();
                }

                SET_MHL_WRITE_BURST_SEQUENCE0(ucVCSequence);
                SET_MHL_WRITE_BURST_STATE_PORT0(MHL_CBUS_WRITE_BURST_SEND_DATA);

                MHL_DRV_DPRINTF("** MHL sent VC information in sequence %d port 0", ucData[4]);
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            if(_mdrv_mhl_GetBurstforVirtualChannel(ucPortSelect, GET_MHL_SEND_VC_CONFIRM_PORT1(), &ucVCSequence, ucData))
            {
                bFinish = TRUE;
            }

            if(mdrv_mhl_CbusSendWriteBurst(ucPortSelect, 0x40, MHL_MSC_SCRATCHPAD_SIZE, ucData))
            {
                if(bFinish)
                {
                    if(GET_MHL_SEND_VC_CONFIRM_PORT1())
                    {
                        CLR_MHL_SEND_VC_CONFIRM_PORT1();

                        bSendVCConfirm = TRUE;
                    }

                    CLR_MHL_SEND_VC_INFORMATION_PORT1();
                }

                SET_MHL_WRITE_BURST_SEQUENCE1(ucVCSequence);
                SET_MHL_WRITE_BURST_STATE_PORT1(MHL_CBUS_WRITE_BURST_SEND_DATA);

                MHL_DRV_DPRINTF("** MHL sent VC information in sequence %d port 1", ucData[4]);
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            if(_mdrv_mhl_GetBurstforVirtualChannel(ucPortSelect, GET_MHL_SEND_VC_CONFIRM_PORT2(), &ucVCSequence, ucData))
            {
                bFinish = TRUE;
            }

            if(mdrv_mhl_CbusSendWriteBurst(ucPortSelect, 0x40, MHL_MSC_SCRATCHPAD_SIZE, ucData))
            {
                if(bFinish)
                {
                    if(GET_MHL_SEND_VC_CONFIRM_PORT2())
                    {
                        CLR_MHL_SEND_VC_CONFIRM_PORT2();

                        bSendVCConfirm = TRUE;
                    }

                    CLR_MHL_SEND_VC_INFORMATION_PORT2();
                }

                SET_MHL_WRITE_BURST_SEQUENCE2(ucVCSequence);
                SET_MHL_WRITE_BURST_STATE_PORT2(MHL_CBUS_WRITE_BURST_SEND_DATA);

                MHL_DRV_DPRINTF("** MHL sent VC information in sequence %d port 2", ucData[4]);
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            if(_mdrv_mhl_GetBurstforVirtualChannel(ucPortSelect, GET_MHL_SEND_VC_CONFIRM_PORT3(), &ucVCSequence, ucData))
            {
                bFinish = TRUE;
            }

            if(mdrv_mhl_CbusSendWriteBurst(ucPortSelect, 0x40, MHL_MSC_SCRATCHPAD_SIZE, ucData))
            {
                if(bFinish)
                {
                    if(GET_MHL_SEND_VC_CONFIRM_PORT3())
                    {
                        CLR_MHL_SEND_VC_CONFIRM_PORT3();

                        bSendVCConfirm = TRUE;
                    }

                    CLR_MHL_SEND_VC_INFORMATION_PORT3();
                }

                SET_MHL_WRITE_BURST_SEQUENCE3(ucVCSequence);
                SET_MHL_WRITE_BURST_STATE_PORT3(MHL_CBUS_WRITE_BURST_SEND_DATA);

                MHL_DRV_DPRINTF("** MHL sent VC information in sequence %d port 3", ucData[4]);
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            if(_mdrv_mhl_GetBurstforVirtualChannel(ucPortSelect, GET_MHL_SEND_VC_CONFIRM_PORT4(), &ucVCSequence, ucData))
            {
                bFinish = TRUE;
            }

            if(mdrv_mhl_CbusSendWriteBurst(ucPortSelect, 0x40, MHL_MSC_SCRATCHPAD_SIZE, ucData))
            {
                if(bFinish)
                {
                    if(GET_MHL_SEND_VC_CONFIRM_PORT4())
                    {
                        CLR_MHL_SEND_VC_CONFIRM_PORT4();

                        bSendVCConfirm = TRUE;
                    }

                    CLR_MHL_SEND_VC_INFORMATION_PORT4();
                }

                SET_MHL_WRITE_BURST_SEQUENCE4(ucVCSequence);
                SET_MHL_WRITE_BURST_STATE_PORT4(MHL_CBUS_WRITE_BURST_SEND_DATA);

                MHL_DRV_DPRINTF("** MHL sent VC information in sequence %d port 4", ucData[4]);
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            if(_mdrv_mhl_GetBurstforVirtualChannel(ucPortSelect, GET_MHL_SEND_VC_CONFIRM_PORT5(), &ucVCSequence, ucData))
            {
                bFinish = TRUE;
            }

            if(mdrv_mhl_CbusSendWriteBurst(ucPortSelect, 0x40, MHL_MSC_SCRATCHPAD_SIZE, ucData))
            {
                if(bFinish)
                {
                    if(GET_MHL_SEND_VC_CONFIRM_PORT5())
                    {
                        CLR_MHL_SEND_VC_CONFIRM_PORT5();

                        bSendVCConfirm = TRUE;
                    }

                    CLR_MHL_SEND_VC_INFORMATION_PORT5();
                }

                SET_MHL_WRITE_BURST_SEQUENCE5(ucVCSequence);
                SET_MHL_WRITE_BURST_STATE_PORT5(MHL_CBUS_WRITE_BURST_SEND_DATA);

                MHL_DRV_DPRINTF("** MHL sent VC information in sequence %d port 5", ucData[4]);
            }

            break;
#endif

        default:

            break;
    };

    if(bSendVCConfirm)
    {
        _mdrv_mhl_SortingVCInformation(ucPortSelect, TRUE);

        _mdrv_mhl_eCbusTimeSlotAllocation(ucPortSelect);
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_GetHighEndVideoVIC()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool _mdrv_mhl_GetHighEndVideoVIC(BYTE ucPortSelect, BYTE *ucHEVSequence, BYTE *pData)
{
    Bool bFinish = FALSE;
    BYTE uctemp = 0;
    BYTE ucEntryCount = 0;
    BYTE ucSequence = 0;
    BYTE ucFinishEntry = 0;
    BYTE ucTotalEntry = MHL_HEV_VIC_TOTAL_ENTRY;

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            ucSequence = GET_MHL_WRITE_BURST_SEQUENCE0();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            ucSequence = GET_MHL_WRITE_BURST_SEQUENCE1();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            ucSequence = GET_MHL_WRITE_BURST_SEQUENCE2();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            ucSequence = GET_MHL_WRITE_BURST_SEQUENCE3();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            ucSequence = GET_MHL_WRITE_BURST_SEQUENCE4();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            ucSequence = GET_MHL_WRITE_BURST_SEQUENCE5();
            break;
#endif

        default:

            break;
    };

    ucFinishEntry = ucSequence *MHL_HEV_VIC_SEQUENCE_LENGTH;

    pData[0] = (MHL_BURST_ID_HEV_VIC >> 8);
    pData[1] = (MHL_BURST_ID_HEV_VIC &MASKBIT(7:0));
    pData[3] = MHL_HEV_VIC_TOTAL_ENTRY;

    for(uctemp = 0; uctemp < MHL_HEV_VIC_SEQUENCE_LENGTH; uctemp++)
    {
        if((uctemp +ucFinishEntry) == ucTotalEntry)
        {
            break;
        }

        pData[6 +(uctemp *2)] = tMHL_HEV_VIC_TABLE[uctemp +ucFinishEntry] >> 8;
        pData[7 +(uctemp *2)] = tMHL_HEV_VIC_TABLE[uctemp +ucFinishEntry] &BITMASK(7:0);

        ucEntryCount++;
    }

    ucSequence++;

    pData[4] = ucSequence;
    pData[5] = ucEntryCount;
    pData[2] = _mdrv_mhl_WriteBurstCheckSum(pData);

    if(((ucSequence -1) *MHL_HEV_VIC_SEQUENCE_LENGTH +ucEntryCount) == MHL_HEV_VIC_TOTAL_ENTRY)
    {
        bFinish = TRUE;
        ucSequence = 0;
    }

    *ucHEVSequence = ucSequence;

    return bFinish;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_GetHighEndVideoDTDA()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool _mdrv_mhl_GetHighEndVideoDTDA(BYTE ucPortSelect, BYTE *ucHEVSequence, BYTE *pData)
{
    Bool bFinish = FALSE;
    BYTE uctemp = 0;
    BYTE ucEntryCount = 0;
    BYTE ucSequence = 0;
    BYTE ucFinishEntry = 0;
    BYTE ucTotalEntry = MHL_HEV_DTD_TOTAL_ENTRY;

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            ucSequence = GET_MHL_WRITE_BURST_SEQUENCE0();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            ucSequence = GET_MHL_WRITE_BURST_SEQUENCE1();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            ucSequence = GET_MHL_WRITE_BURST_SEQUENCE2();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            ucSequence = GET_MHL_WRITE_BURST_SEQUENCE3();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            ucSequence = GET_MHL_WRITE_BURST_SEQUENCE4();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            ucSequence = GET_MHL_WRITE_BURST_SEQUENCE5();
            break;
#endif

        default:

            break;
    };

    ucFinishEntry = ucSequence *MHL_HEV_DTD_SEQUENCE_LENGTH;

    pData[0] = (MHL_BURST_ID_HEV_DTDA >> 8);
    pData[1] = (MHL_BURST_ID_HEV_DTDA &MASKBIT(7:0));
    pData[3] = MHL_HEV_DTD_TOTAL_ENTRY;

    for(uctemp = 0; uctemp < ucTotalEntry; uctemp++)
    {
        if(ucFinishEntry > 0)
        {
            ucFinishEntry--;
        }
        else if(ucEntryCount < MHL_HEV_DTD_SEQUENCE_LENGTH)
        {
            ucEntryCount++;
        }
    }

    ucSequence++;

    pData[4] = ucSequence;
    pData[2] = _mdrv_mhl_WriteBurstCheckSum(pData);

    if(((ucSequence -1) *MHL_HEV_DTD_SEQUENCE_LENGTH +ucEntryCount) == MHL_HEV_DTD_TOTAL_ENTRY)
    {
        bFinish = TRUE;
        ucSequence = 0;
    }

    *ucHEVSequence = ucSequence;

    return bFinish;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_GetHighEndVideoDTDB()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool _mdrv_mhl_GetHighEndVideoDTDB(BYTE ucPortSelect, BYTE *ucHEVSequence, BYTE *pData)
{
    Bool bFinish = FALSE;
    BYTE uctemp = 0;
    BYTE ucEntryCount = 0;
    BYTE ucSequence = 0;
    BYTE ucFinishEntry = 0;
    BYTE ucTotalEntry = MHL_HEV_DTD_TOTAL_ENTRY;

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            ucSequence = GET_MHL_WRITE_BURST_SEQUENCE0();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            ucSequence = GET_MHL_WRITE_BURST_SEQUENCE1();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            ucSequence = GET_MHL_WRITE_BURST_SEQUENCE2();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            ucSequence = GET_MHL_WRITE_BURST_SEQUENCE3();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            ucSequence = GET_MHL_WRITE_BURST_SEQUENCE4();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            ucSequence = GET_MHL_WRITE_BURST_SEQUENCE5();
            break;
#endif

        default:

            break;
    };

    ucFinishEntry = ucSequence *MHL_HEV_DTD_SEQUENCE_LENGTH;

    pData[0] = (MHL_BURST_ID_HEV_DTDB >> 8);
    pData[1] = (MHL_BURST_ID_HEV_DTDB &MASKBIT(7:0));
    pData[3] = MHL_HEV_DTD_TOTAL_ENTRY;

    for(uctemp = 0; uctemp < ucTotalEntry; uctemp++)
    {
        if(ucFinishEntry > 0)
        {
            ucFinishEntry--;
        }
        else if(ucEntryCount < MHL_HEV_DTD_SEQUENCE_LENGTH)
        {
            ucEntryCount++;
        }
    }

    ucSequence++;

    pData[4] = ucSequence;
    pData[2] = _mdrv_mhl_WriteBurstCheckSum(pData);

    if(((ucSequence -1) *MHL_HEV_DTD_SEQUENCE_LENGTH +ucEntryCount) == MHL_HEV_DTD_TOTAL_ENTRY)
    {
        bFinish = TRUE;
        ucSequence = 0;
    }

    *ucHEVSequence = ucSequence;

    return bFinish;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_SendHighEndVideoSupport()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_SendHighEndVideoSupport(BYTE ucPortSelect)
{
    Bool bFinish = FALSE;
    BYTE ucHEVSequence = 0;
    BYTE ucData[MHL_MSC_SCRATCHPAD_SIZE] = {0};

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            if(GET_MHL_SEND_HEV_VIC_FLAG0())
            {
                if(_mdrv_mhl_GetHighEndVideoVIC(ucPortSelect, &ucHEVSequence, ucData))
                {
                    bFinish = TRUE;
                }

                if(mdrv_mhl_CbusSendWriteBurst(ucPortSelect, 0x40, MHL_MSC_SCRATCHPAD_SIZE, ucData))
                {
                    if(bFinish)
                    {
                        CLR_MHL_SEND_HEV_VIC_FLAG0();
                    }

                    SET_MHL_WRITE_BURST_SEQUENCE0(ucHEVSequence);
                    SET_MHL_WRITE_BURST_STATE_PORT0(MHL_CBUS_WRITE_BURST_SEND_DATA);

                    MHL_DRV_DPRINTF("** MHL sent HEV VIC in sequence %d port 0", ucData[4]);
                }
            }
            else if(GET_MHL_SEND_HEV_DTDA_FLAG0())
            {
                if(_mdrv_mhl_GetHighEndVideoDTDA(ucPortSelect, &ucHEVSequence, ucData))
                {
                    bFinish = TRUE;
                }

                if(mdrv_mhl_CbusSendWriteBurst(ucPortSelect, 0x40, MHL_MSC_SCRATCHPAD_SIZE, ucData))
                {
                    if(bFinish)
                    {
                        CLR_MHL_SEND_HEV_DTDA_FLAG0();
                    }

                    SET_MHL_WRITE_BURST_SEQUENCE0(ucHEVSequence);
                    SET_MHL_WRITE_BURST_STATE_PORT0(MHL_CBUS_WRITE_BURST_SEND_DATA);

                    MHL_DRV_DPRINTF("** MHL sent HEV DTDA in sequence %d port 0", ucData[4]);
                }
            }
            else if(GET_MHL_SEND_HEV_DTDB_FLAG0())
            {
                if(_mdrv_mhl_GetHighEndVideoDTDB(ucPortSelect, &ucHEVSequence, ucData))
                {
                    bFinish = TRUE;
                }

                if(mdrv_mhl_CbusSendWriteBurst(ucPortSelect, 0x40, MHL_MSC_SCRATCHPAD_SIZE, ucData))
                {
                    if(bFinish)
                    {
                        CLR_MHL_SEND_HEV_DTDB_FLAG0();
                        CLR_MHL_SEND_HIGH_END_VIDEO0();
                    }

                    SET_MHL_WRITE_BURST_SEQUENCE0(ucHEVSequence);
                    SET_MHL_WRITE_BURST_STATE_PORT0(MHL_CBUS_WRITE_BURST_SEND_DATA);

                    MHL_DRV_DPRINTF("** MHL sent HEV DTDB in sequence %d port 0", ucData[4]);
                }
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            if(GET_MHL_SEND_HEV_VIC_FLAG1())
            {
                if(_mdrv_mhl_GetHighEndVideoVIC(ucPortSelect, &ucHEVSequence, ucData))
                {
                    bFinish = TRUE;
                }

                if(mdrv_mhl_CbusSendWriteBurst(ucPortSelect, 0x40, MHL_MSC_SCRATCHPAD_SIZE, ucData))
                {
                    if(bFinish)
                    {
                        CLR_MHL_SEND_HEV_VIC_FLAG1();
                    }

                    SET_MHL_WRITE_BURST_SEQUENCE1(ucHEVSequence);
                    SET_MHL_WRITE_BURST_STATE_PORT1(MHL_CBUS_WRITE_BURST_SEND_DATA);

                    MHL_DRV_DPRINTF("** MHL sent HEV VIC in sequence %d port 1", ucData[4]);
                }
            }
            else if(GET_MHL_SEND_HEV_DTDA_FLAG1())
            {
                if(_mdrv_mhl_GetHighEndVideoDTDA(ucPortSelect, &ucHEVSequence, ucData))
                {
                    bFinish = TRUE;
                }

                if(mdrv_mhl_CbusSendWriteBurst(ucPortSelect, 0x40, MHL_MSC_SCRATCHPAD_SIZE, ucData))
                {
                    if(bFinish)
                    {
                        CLR_MHL_SEND_HEV_DTDA_FLAG1();
                    }

                    SET_MHL_WRITE_BURST_SEQUENCE1(ucHEVSequence);
                    SET_MHL_WRITE_BURST_STATE_PORT1(MHL_CBUS_WRITE_BURST_SEND_DATA);

                    MHL_DRV_DPRINTF("** MHL sent HEV DTDA in sequence %d port 1", ucData[4]);
                }
            }
            else if(GET_MHL_SEND_HEV_DTDB_FLAG1())
            {
                if(_mdrv_mhl_GetHighEndVideoDTDB(ucPortSelect, &ucHEVSequence, ucData))
                {
                    bFinish = TRUE;
                }

                if(mdrv_mhl_CbusSendWriteBurst(ucPortSelect, 0x40, MHL_MSC_SCRATCHPAD_SIZE, ucData))
                {
                    if(bFinish)
                    {
                        CLR_MHL_SEND_HEV_DTDB_FLAG1();
                        CLR_MHL_SEND_HIGH_END_VIDEO1();
                    }

                    SET_MHL_WRITE_BURST_SEQUENCE1(ucHEVSequence);
                    SET_MHL_WRITE_BURST_STATE_PORT1(MHL_CBUS_WRITE_BURST_SEND_DATA);

                    MHL_DRV_DPRINTF("** MHL sent HEV DTDB in sequence %d port 1", ucData[4]);
                }
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            if(GET_MHL_SEND_HEV_VIC_FLAG2())
            {
                if(_mdrv_mhl_GetHighEndVideoVIC(ucPortSelect, &ucHEVSequence, ucData))
                {
                    bFinish = TRUE;
                }

                if(mdrv_mhl_CbusSendWriteBurst(ucPortSelect, 0x40, MHL_MSC_SCRATCHPAD_SIZE, ucData))
                {
                    if(bFinish)
                    {
                        CLR_MHL_SEND_HEV_VIC_FLAG2();
                    }

                    SET_MHL_WRITE_BURST_SEQUENCE2(ucHEVSequence);
                    SET_MHL_WRITE_BURST_STATE_PORT2(MHL_CBUS_WRITE_BURST_SEND_DATA);

                    MHL_DRV_DPRINTF("** MHL sent HEV VIC in sequence %d port 2", ucData[4]);
                }
            }
            else if(GET_MHL_SEND_HEV_DTDA_FLAG2())
            {
                if(_mdrv_mhl_GetHighEndVideoDTDA(ucPortSelect, &ucHEVSequence, ucData))
                {
                    bFinish = TRUE;
                }

                if(mdrv_mhl_CbusSendWriteBurst(ucPortSelect, 0x40, MHL_MSC_SCRATCHPAD_SIZE, ucData))
                {
                    if(bFinish)
                    {
                        CLR_MHL_SEND_HEV_DTDA_FLAG2();
                    }

                    SET_MHL_WRITE_BURST_SEQUENCE2(ucHEVSequence);
                    SET_MHL_WRITE_BURST_STATE_PORT2(MHL_CBUS_WRITE_BURST_SEND_DATA);

                    MHL_DRV_DPRINTF("** MHL sent HEV DTDA in sequence %d port 2", ucData[4]);
                }
            }
            else if(GET_MHL_SEND_HEV_DTDB_FLAG2())
            {
                if(_mdrv_mhl_GetHighEndVideoDTDB(ucPortSelect, &ucHEVSequence, ucData))
                {
                    bFinish = TRUE;
                }

                if(mdrv_mhl_CbusSendWriteBurst(ucPortSelect, 0x40, MHL_MSC_SCRATCHPAD_SIZE, ucData))
                {
                    if(bFinish)
                    {
                        CLR_MHL_SEND_HEV_DTDB_FLAG2();
                        CLR_MHL_SEND_HIGH_END_VIDEO2();
                    }

                    SET_MHL_WRITE_BURST_SEQUENCE2(ucHEVSequence);
                    SET_MHL_WRITE_BURST_STATE_PORT2(MHL_CBUS_WRITE_BURST_SEND_DATA);

                    MHL_DRV_DPRINTF("** MHL sent HEV DTDB in sequence %d port 2", ucData[4]);
                }
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            if(GET_MHL_SEND_HEV_VIC_FLAG3())
            {
                if(_mdrv_mhl_GetHighEndVideoVIC(ucPortSelect, &ucHEVSequence, ucData))
                {
                    bFinish = TRUE;
                }

                if(mdrv_mhl_CbusSendWriteBurst(ucPortSelect, 0x40, MHL_MSC_SCRATCHPAD_SIZE, ucData))
                {
                    if(bFinish)
                    {
                        CLR_MHL_SEND_HEV_VIC_FLAG3();
                    }

                    SET_MHL_WRITE_BURST_SEQUENCE3(ucHEVSequence);
                    SET_MHL_WRITE_BURST_STATE_PORT3(MHL_CBUS_WRITE_BURST_SEND_DATA);

                    MHL_DRV_DPRINTF("** MHL sent HEV VIC in sequence %d port 3", ucData[4]);
                }
            }
            else if(GET_MHL_SEND_HEV_DTDA_FLAG3())
            {
                if(_mdrv_mhl_GetHighEndVideoDTDA(ucPortSelect, &ucHEVSequence, ucData))
                {
                    bFinish = TRUE;
                }

                if(mdrv_mhl_CbusSendWriteBurst(ucPortSelect, 0x40, MHL_MSC_SCRATCHPAD_SIZE, ucData))
                {
                    if(bFinish)
                    {
                        CLR_MHL_SEND_HEV_DTDA_FLAG3();
                    }

                    SET_MHL_WRITE_BURST_SEQUENCE3(ucHEVSequence);
                    SET_MHL_WRITE_BURST_STATE_PORT3(MHL_CBUS_WRITE_BURST_SEND_DATA);

                    MHL_DRV_DPRINTF("** MHL sent HEV DTDA in sequence %d port 3", ucData[4]);
                }
            }
            else if(GET_MHL_SEND_HEV_DTDB_FLAG3())
            {
                if(_mdrv_mhl_GetHighEndVideoDTDB(ucPortSelect, &ucHEVSequence, ucData))
                {
                    bFinish = TRUE;
                }

                if(mdrv_mhl_CbusSendWriteBurst(ucPortSelect, 0x40, MHL_MSC_SCRATCHPAD_SIZE, ucData))
                {
                    if(bFinish)
                    {
                        CLR_MHL_SEND_HEV_DTDB_FLAG3();
                        CLR_MHL_SEND_HIGH_END_VIDEO3();
                    }

                    SET_MHL_WRITE_BURST_SEQUENCE3(ucHEVSequence);
                    SET_MHL_WRITE_BURST_STATE_PORT3(MHL_CBUS_WRITE_BURST_SEND_DATA);

                    MHL_DRV_DPRINTF("** MHL sent HEV DTDB in sequence %d port 3", ucData[4]);
                }
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            if(GET_MHL_SEND_HEV_VIC_FLAG4())
            {
                if(_mdrv_mhl_GetHighEndVideoVIC(ucPortSelect, &ucHEVSequence, ucData))
                {
                    bFinish = TRUE;
                }

                if(mdrv_mhl_CbusSendWriteBurst(ucPortSelect, 0x40, MHL_MSC_SCRATCHPAD_SIZE, ucData))
                {
                    if(bFinish)
                    {
                        CLR_MHL_SEND_HEV_VIC_FLAG4();
                    }

                    SET_MHL_WRITE_BURST_SEQUENCE4(ucHEVSequence);
                    SET_MHL_WRITE_BURST_STATE_PORT4(MHL_CBUS_WRITE_BURST_SEND_DATA);

                    MHL_DRV_DPRINTF("** MHL sent HEV VIC in sequence %d port 4", ucData[4]);
                }
            }
            else if(GET_MHL_SEND_HEV_DTDA_FLAG4())
            {
                if(_mdrv_mhl_GetHighEndVideoDTDA(ucPortSelect, &ucHEVSequence, ucData))
                {
                    bFinish = TRUE;
                }

                if(mdrv_mhl_CbusSendWriteBurst(ucPortSelect, 0x40, MHL_MSC_SCRATCHPAD_SIZE, ucData))
                {
                    if(bFinish)
                    {
                        CLR_MHL_SEND_HEV_DTDA_FLAG4();
                    }

                    SET_MHL_WRITE_BURST_SEQUENCE4(ucHEVSequence);
                    SET_MHL_WRITE_BURST_STATE_PORT4(MHL_CBUS_WRITE_BURST_SEND_DATA);

                    MHL_DRV_DPRINTF("** MHL sent HEV DTDA in sequence %d port 4", ucData[4]);
                }
            }
            else if(GET_MHL_SEND_HEV_DTDB_FLAG4())
            {
                if(_mdrv_mhl_GetHighEndVideoDTDB(ucPortSelect, &ucHEVSequence, ucData))
                {
                    bFinish = TRUE;
                }

                if(mdrv_mhl_CbusSendWriteBurst(ucPortSelect, 0x40, MHL_MSC_SCRATCHPAD_SIZE, ucData))
                {
                    if(bFinish)
                    {
                        CLR_MHL_SEND_HEV_DTDB_FLAG4();
                        CLR_MHL_SEND_HIGH_END_VIDEO4();
                    }

                    SET_MHL_WRITE_BURST_SEQUENCE4(ucHEVSequence);
                    SET_MHL_WRITE_BURST_STATE_PORT4(MHL_CBUS_WRITE_BURST_SEND_DATA);

                    MHL_DRV_DPRINTF("** MHL sent HEV DTDB in sequence %d port 4", ucData[4]);
                }
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            if(GET_MHL_SEND_HEV_VIC_FLAG5())
            {
                if(_mdrv_mhl_GetHighEndVideoVIC(ucPortSelect, &ucHEVSequence, ucData))
                {
                    bFinish = TRUE;
                }

                if(mdrv_mhl_CbusSendWriteBurst(ucPortSelect, 0x40, MHL_MSC_SCRATCHPAD_SIZE, ucData))
                {
                    if(bFinish)
                    {
                        CLR_MHL_SEND_HEV_VIC_FLAG5();
                    }

                    SET_MHL_WRITE_BURST_SEQUENCE5(ucHEVSequence);
                    SET_MHL_WRITE_BURST_STATE_PORT5(MHL_CBUS_WRITE_BURST_SEND_DATA);

                    MHL_DRV_DPRINTF("** MHL sent HEV VIC in sequence %d port 5", ucData[4]);
                }
            }
            else if(GET_MHL_SEND_HEV_DTDA_FLAG5())
            {
                if(_mdrv_mhl_GetHighEndVideoDTDA(ucPortSelect, &ucHEVSequence, ucData))
                {
                    bFinish = TRUE;
                }

                if(mdrv_mhl_CbusSendWriteBurst(ucPortSelect, 0x40, MHL_MSC_SCRATCHPAD_SIZE, ucData))
                {
                    if(bFinish)
                    {
                        CLR_MHL_SEND_HEV_DTDA_FLAG5();
                    }

                    SET_MHL_WRITE_BURST_SEQUENCE5(ucHEVSequence);
                    SET_MHL_WRITE_BURST_STATE_PORT5(MHL_CBUS_WRITE_BURST_SEND_DATA);

                    MHL_DRV_DPRINTF("** MHL sent HEV DTDA in sequence %d port 5", ucData[4]);
                }
            }
            else if(GET_MHL_SEND_HEV_DTDB_FLAG5())
            {
                if(_mdrv_mhl_GetHighEndVideoDTDB(ucPortSelect, &ucHEVSequence, ucData))
                {
                    bFinish = TRUE;
                }

                if(mdrv_mhl_CbusSendWriteBurst(ucPortSelect, 0x40, MHL_MSC_SCRATCHPAD_SIZE, ucData))
                {
                    if(bFinish)
                    {
                        CLR_MHL_SEND_HEV_DTDB_FLAG5();
                        CLR_MHL_SEND_HIGH_END_VIDEO5();
                    }

                    SET_MHL_WRITE_BURST_SEQUENCE5(ucHEVSequence);
                    SET_MHL_WRITE_BURST_STATE_PORT5(MHL_CBUS_WRITE_BURST_SEND_DATA);

                    MHL_DRV_DPRINTF("** MHL sent HEV DTDB in sequence %d port 5", ucData[4]);
                }
            }

            break;
#endif

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_GetAudioDataTunnel()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool _mdrv_mhl_GetAudioDataTunnel(BYTE ucPortSelect, BYTE *ucADTSequence, BYTE *pData)
{
    Bool bFinish = FALSE;
    BYTE uctemp = 0;
    BYTE ucEntryCount = 0;
    BYTE ucSequence = 0;
    BYTE ucFinishEntry = 0;
    BYTE ucTotalEntry = MHL_AUDIO_TUNNEL_TOTAL_ENTRY;

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            ucSequence = GET_MHL_WRITE_BURST_SEQUENCE0();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            ucSequence = GET_MHL_WRITE_BURST_SEQUENCE1();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            ucSequence = GET_MHL_WRITE_BURST_SEQUENCE2();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            ucSequence = GET_MHL_WRITE_BURST_SEQUENCE3();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            ucSequence = GET_MHL_WRITE_BURST_SEQUENCE4();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            ucSequence = GET_MHL_WRITE_BURST_SEQUENCE5();
            break;
#endif

        default:

            break;
    };

    ucFinishEntry = ucSequence *MHL_AUDIO_TUNNEL_SEQUENCE_LENGTH;

    pData[0] = (MHL_BURST_ID_ADT_BURSTID >> 8);
    pData[1] = (MHL_BURST_ID_ADT_BURSTID &MASKBIT(7:0));
    pData[3] = MHL_AUDIO_TUNNEL_TOTAL_ENTRY;

    if((ucSequence == 0) && (MHL_AUDIO_TUNNEL_SADB_RNTRY > 0)) // Support speaker allocation data block
    {
        pData[5] = 1;
    }
    
    for(uctemp = 0; uctemp < MHL_AUDIO_TUNNEL_SEQUENCE_LENGTH; uctemp++)
    {
        if((uctemp +ucFinishEntry) == ucTotalEntry)
        {
            break;
        }

        pData[7 +(uctemp *3)] = tMHL_AUDIO_TUNNEL_TABLE[uctemp +ucFinishEntry] >> 16;
        pData[8 +(uctemp *3)] = tMHL_AUDIO_TUNNEL_TABLE[uctemp +ucFinishEntry] >> 8;
        pData[9 +(uctemp *3)] = tMHL_AUDIO_TUNNEL_TABLE[uctemp +ucFinishEntry] &BITMASK(7:0);

        ucEntryCount++;
    }

    ucSequence++;

    pData[4] = ucSequence;
    pData[6] = ucEntryCount;
    pData[2] = _mdrv_mhl_WriteBurstCheckSum(pData);

    if(((ucSequence -1) *MHL_AUDIO_TUNNEL_SEQUENCE_LENGTH +ucEntryCount) == MHL_AUDIO_TUNNEL_TOTAL_ENTRY)
    {
        bFinish = TRUE;
        ucSequence = 0;
    }

    *ucADTSequence = ucSequence;

    return bFinish;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_SendAudioDataTunnelSupport()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_SendAudioDataTunnelSupport(BYTE ucPortSelect)
{
    Bool bFinish = FALSE;
    BYTE ucADTSequence = 0;
    BYTE ucData[MHL_MSC_SCRATCHPAD_SIZE] = {0};

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            if(_mdrv_mhl_GetAudioDataTunnel(ucPortSelect, &ucADTSequence, ucData))
            {
                bFinish = TRUE;
            }

            if(mdrv_mhl_CbusSendWriteBurst(ucPortSelect, 0x40, MHL_MSC_SCRATCHPAD_SIZE, ucData))
            {
                if(bFinish)
                {
                    CLR_MHL_SEND_AUDIO_DATA_TUNNEL0();
                }

                SET_MHL_WRITE_BURST_SEQUENCE0(ucADTSequence);
                SET_MHL_WRITE_BURST_STATE_PORT0(MHL_CBUS_WRITE_BURST_SEND_DATA);

                MHL_DRV_DPRINTF("** MHL sent audio tunnel in sequence %d port 0", ucData[4]);
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            if(_mdrv_mhl_GetAudioDataTunnel(ucPortSelect, &ucADTSequence, ucData))
            {
                bFinish = TRUE;
            }

            if(mdrv_mhl_CbusSendWriteBurst(ucPortSelect, 0x40, MHL_MSC_SCRATCHPAD_SIZE, ucData))
            {
                if(bFinish)
                {
                    CLR_MHL_SEND_AUDIO_DATA_TUNNEL1();
                }

                SET_MHL_WRITE_BURST_SEQUENCE1(ucADTSequence);
                SET_MHL_WRITE_BURST_STATE_PORT1(MHL_CBUS_WRITE_BURST_SEND_DATA);

                MHL_DRV_DPRINTF("** MHL sent audio tunnel in sequence %d port 1", ucData[4]);
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            if(_mdrv_mhl_GetAudioDataTunnel(ucPortSelect, &ucADTSequence, ucData))
            {
                bFinish = TRUE;
            }

            if(mdrv_mhl_CbusSendWriteBurst(ucPortSelect, 0x40, MHL_MSC_SCRATCHPAD_SIZE, ucData))
            {
                if(bFinish)
                {
                    CLR_MHL_SEND_AUDIO_DATA_TUNNEL2();
                }

                SET_MHL_WRITE_BURST_SEQUENCE2(ucADTSequence);
                SET_MHL_WRITE_BURST_STATE_PORT2(MHL_CBUS_WRITE_BURST_SEND_DATA);

                MHL_DRV_DPRINTF("** MHL sent audio tunnel in sequence %d port 2", ucData[4]);
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            if(_mdrv_mhl_GetAudioDataTunnel(ucPortSelect, &ucADTSequence, ucData))
            {
                bFinish = TRUE;
            }

            if(mdrv_mhl_CbusSendWriteBurst(ucPortSelect, 0x40, MHL_MSC_SCRATCHPAD_SIZE, ucData))
            {
                if(bFinish)
                {
                    CLR_MHL_SEND_AUDIO_DATA_TUNNEL3();
                }

                SET_MHL_WRITE_BURST_SEQUENCE3(ucADTSequence);
                SET_MHL_WRITE_BURST_STATE_PORT3(MHL_CBUS_WRITE_BURST_SEND_DATA);

                MHL_DRV_DPRINTF("** MHL sent audio tunnel in sequence %d port 3", ucData[4]);
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            if(_mdrv_mhl_GetAudioDataTunnel(ucPortSelect, &ucADTSequence, ucData))
            {
                bFinish = TRUE;
            }

            if(mdrv_mhl_CbusSendWriteBurst(ucPortSelect, 0x40, MHL_MSC_SCRATCHPAD_SIZE, ucData))
            {
                if(bFinish)
                {
                    CLR_MHL_SEND_AUDIO_DATA_TUNNEL4();
                }

                SET_MHL_WRITE_BURST_SEQUENCE4(ucADTSequence);
                SET_MHL_WRITE_BURST_STATE_PORT4(MHL_CBUS_WRITE_BURST_SEND_DATA);

                MHL_DRV_DPRINTF("** MHL sent audio tunnel in sequence %d port 4", ucData[4]);
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            if(_mdrv_mhl_GetAudioDataTunnel(ucPortSelect, &ucADTSequence, ucData))
            {
                bFinish = TRUE;
            }

            if(mdrv_mhl_CbusSendWriteBurst(ucPortSelect, 0x40, MHL_MSC_SCRATCHPAD_SIZE, ucData))
            {
                if(bFinish)
                {
                    CLR_MHL_SEND_AUDIO_DATA_TUNNEL5();
                }

                SET_MHL_WRITE_BURST_SEQUENCE5(ucADTSequence);
                SET_MHL_WRITE_BURST_STATE_PORT5(MHL_CBUS_WRITE_BURST_SEND_DATA);

                MHL_DRV_DPRINTF("** MHL sent audio tunnel in sequence %d port 5", ucData[4]);
            }

            break;
#endif

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_GeteMSCSupportInformation()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool _mdrv_mhl_GeteMSCSupportInformation(BYTE ucPortSelect, BYTE *ucMSCSequence, BYTE *pData)
{
    Bool bFinish = FALSE;
    BYTE uctemp = 0;
    BYTE ucEntryCount = 0;
    BYTE ucSequence = 0;
    BYTE ucFinishEntry = 0;
    BYTE ucTotalEntry = MHL_EMSC_SUPPORT_TOTAL_ENTRY;

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            ucSequence = GET_MHL_WRITE_BURST_SEQUENCE0();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            ucSequence = GET_MHL_WRITE_BURST_SEQUENCE1();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            ucSequence = GET_MHL_WRITE_BURST_SEQUENCE2();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            ucSequence = GET_MHL_WRITE_BURST_SEQUENCE3();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            ucSequence = GET_MHL_WRITE_BURST_SEQUENCE4();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            ucSequence = GET_MHL_WRITE_BURST_SEQUENCE5();
            break;
#endif

        default:

            break;
    };

    ucFinishEntry = ucSequence *MHL_EMSC_SUPPORT_SEQUENCE_LENGTH;

    pData[0] = (MHL_BURST_ID_EMSC_SUPPORT >> 8);
    pData[1] = (MHL_BURST_ID_EMSC_SUPPORT &MASKBIT(7:0));
    pData[3] = MHL_EMSC_SUPPORT_TOTAL_ENTRY;

    for(uctemp = 0; uctemp < MHL_EMSC_SUPPORT_SEQUENCE_LENGTH; uctemp++)
    {
        if((uctemp +ucFinishEntry) == ucTotalEntry)
        {
            break;
        }

        pData[6 +(uctemp *2)] = tMHL_EMSC_SUPPORT_TABLE[uctemp +ucFinishEntry] >> 8;
        pData[7 +(uctemp *2)] = tMHL_EMSC_SUPPORT_TABLE[uctemp +ucFinishEntry] &BITMASK(7:0);

        ucEntryCount++;
    }

    ucSequence++;

    pData[4] = ucSequence;
    pData[5] = ucEntryCount;
    pData[2] = _mdrv_mhl_WriteBurstCheckSum(pData);

    if(((ucSequence -1) *MHL_EMSC_SUPPORT_SEQUENCE_LENGTH +ucEntryCount) == MHL_EMSC_SUPPORT_TOTAL_ENTRY)
    {
        bFinish = TRUE;
        ucSequence = 0;
    }

    *ucMSCSequence = ucSequence;

    return bFinish;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_SendeMSCSupportInformation()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_SendeMSCSupportInformation(BYTE ucPortSelect)
{
    Bool bFinish = FALSE;
    BYTE ucMSCSequence = 0;
    BYTE ucData[MHL_MSC_SCRATCHPAD_SIZE] = {0};

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            if(_mdrv_mhl_GeteMSCSupportInformation(ucPortSelect, &ucMSCSequence, ucData))
            {
                bFinish = TRUE;
            }

            if(mdrv_mhl_CbusSendWriteBurst(ucPortSelect, 0x40, MHL_MSC_SCRATCHPAD_SIZE, ucData))
            {
                if(bFinish)
                {
                    CLR_MHL_SEND_EMSC_DATA_TUNNEL0();
                }

                SET_MHL_WRITE_BURST_SEQUENCE0(ucMSCSequence);
                SET_MHL_WRITE_BURST_STATE_PORT0(MHL_CBUS_WRITE_BURST_SEND_DATA);

                MHL_DRV_DPRINTF("** MHL sent eMSC support in sequence %d port 0", ucData[4]);
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            if(_mdrv_mhl_GeteMSCSupportInformation(ucPortSelect, &ucMSCSequence, ucData))
            {
                bFinish = TRUE;
            }

            if(mdrv_mhl_CbusSendWriteBurst(ucPortSelect, 0x40, MHL_MSC_SCRATCHPAD_SIZE, ucData))
            {
                if(bFinish)
                {
                    CLR_MHL_SEND_EMSC_DATA_TUNNEL1();
                }

                SET_MHL_WRITE_BURST_SEQUENCE1(ucMSCSequence);
                SET_MHL_WRITE_BURST_STATE_PORT1(MHL_CBUS_WRITE_BURST_SEND_DATA);

                MHL_DRV_DPRINTF("** MHL sent eMSC support in sequence %d port 1", ucData[4]);
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            if(_mdrv_mhl_GeteMSCSupportInformation(ucPortSelect, &ucMSCSequence, ucData))
            {
                bFinish = TRUE;
            }

            if(mdrv_mhl_CbusSendWriteBurst(ucPortSelect, 0x40, MHL_MSC_SCRATCHPAD_SIZE, ucData))
            {
                if(bFinish)
                {
                    CLR_MHL_SEND_EMSC_DATA_TUNNEL2();
                }

                SET_MHL_WRITE_BURST_SEQUENCE2(ucMSCSequence);
                SET_MHL_WRITE_BURST_STATE_PORT2(MHL_CBUS_WRITE_BURST_SEND_DATA);

                MHL_DRV_DPRINTF("** MHL sent eMSC support in sequence %d port 2", ucData[4]);
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            if(_mdrv_mhl_GeteMSCSupportInformation(ucPortSelect, &ucMSCSequence, ucData))
            {
                bFinish = TRUE;
            }

            if(mdrv_mhl_CbusSendWriteBurst(ucPortSelect, 0x40, MHL_MSC_SCRATCHPAD_SIZE, ucData))
            {
                if(bFinish)
                {
                    CLR_MHL_SEND_EMSC_DATA_TUNNEL3();
                }

                SET_MHL_WRITE_BURST_SEQUENCE3(ucMSCSequence);
                SET_MHL_WRITE_BURST_STATE_PORT3(MHL_CBUS_WRITE_BURST_SEND_DATA);

                MHL_DRV_DPRINTF("** MHL sent eMSC support in sequence %d port 3", ucData[4]);
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            if(_mdrv_mhl_GeteMSCSupportInformation(ucPortSelect, &ucMSCSequence, ucData))
            {
                bFinish = TRUE;
            }

            if(mdrv_mhl_CbusSendWriteBurst(ucPortSelect, 0x40, MHL_MSC_SCRATCHPAD_SIZE, ucData))
            {
                if(bFinish)
                {
                    CLR_MHL_SEND_EMSC_DATA_TUNNEL4();
                }

                SET_MHL_WRITE_BURST_SEQUENCE4(ucMSCSequence);
                SET_MHL_WRITE_BURST_STATE_PORT4(MHL_CBUS_WRITE_BURST_SEND_DATA);

                MHL_DRV_DPRINTF("** MHL sent eMSC support in sequence %d port 4", ucData[4]);
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            if(_mdrv_mhl_GeteMSCSupportInformation(ucPortSelect, &ucMSCSequence, ucData))
            {
                bFinish = TRUE;
            }

            if(mdrv_mhl_CbusSendWriteBurst(ucPortSelect, 0x40, MHL_MSC_SCRATCHPAD_SIZE, ucData))
            {
                if(bFinish)
                {
                    CLR_MHL_SEND_EMSC_DATA_TUNNEL5();
                }

                SET_MHL_WRITE_BURST_SEQUENCE5(ucMSCSequence);
                SET_MHL_WRITE_BURST_STATE_PORT5(MHL_CBUS_WRITE_BURST_SEND_DATA);

                MHL_DRV_DPRINTF("** MHL sent eMSC support in sequence %d port 5", ucData[4]);
            }

            break;
#endif

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_GetBISTReturnStatus()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_GetBISTReturnStatus(BYTE ucPortSelect, BYTE *pData)
{
    WORD usBISTeCbusErrorCount = 0;
    WORD usBISTAVLinkErrorCount = 0;
    
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            usBISTeCbusErrorCount = GET_MHL_BIST_ECBUS_ERROR_CNT0();
            usBISTAVLinkErrorCount = GET_MHL_BIST_AV_LINK_ERROR_CNT0();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            usBISTeCbusErrorCount = GET_MHL_BIST_ECBUS_ERROR_CNT1();
            usBISTAVLinkErrorCount = GET_MHL_BIST_AV_LINK_ERROR_CNT1();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            usBISTeCbusErrorCount = GET_MHL_BIST_ECBUS_ERROR_CNT2();
            usBISTAVLinkErrorCount = GET_MHL_BIST_AV_LINK_ERROR_CNT2();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            usBISTeCbusErrorCount = GET_MHL_BIST_ECBUS_ERROR_CNT3();
            usBISTAVLinkErrorCount = GET_MHL_BIST_AV_LINK_ERROR_CNT3();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            usBISTeCbusErrorCount = GET_MHL_BIST_ECBUS_ERROR_CNT4();
            usBISTAVLinkErrorCount = GET_MHL_BIST_AV_LINK_ERROR_CNT4();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            usBISTeCbusErrorCount = GET_MHL_BIST_ECBUS_ERROR_CNT5();
            usBISTAVLinkErrorCount = GET_MHL_BIST_AV_LINK_ERROR_CNT5();
            break;
#endif

        default:

            break;
    };

    pData[0] = (MHL_BURST_ID_BIST_RETURN_STAT >> 8);
    pData[1] = (MHL_BURST_ID_BIST_RETURN_STAT &MASKBIT(7:0));
    pData[12] = (usBISTeCbusErrorCount >> 8);
    pData[13] = (usBISTeCbusErrorCount &MASKBIT(7:0));
    pData[14] = (usBISTAVLinkErrorCount >> 8);
    pData[15] = (usBISTAVLinkErrorCount &MASKBIT(7:0));
    pData[2] = _mdrv_mhl_WriteBurstCheckSum(pData);
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_SendBISTReturnStatus()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_SendBISTReturnStatus(BYTE ucPortSelect)
{
    BYTE ucData[MHL_MSC_SCRATCHPAD_SIZE] = {0};

    _mdrv_mhl_GetBISTReturnStatus(ucPortSelect, ucData);

    if(mdrv_mhl_CbusSendWriteBurst(ucPortSelect, 0x40, MHL_MSC_SCRATCHPAD_SIZE, ucData))
    {
        switch(ucPortSelect)
        {
#if(MHL_FUNCTION_SUPPORT_PORT0)
            case MHL_INPUT_SELECT_PORT0:
                CLR_MHL_SEND_BIST_RETURN_STATUS0();
                SET_MHL_WRITE_BURST_STATE_PORT0(MHL_CBUS_WRITE_BURST_SEND_DATA);
                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
            case MHL_INPUT_SELECT_PORT1:
                CLR_MHL_SEND_BIST_RETURN_STATUS1();
                SET_MHL_WRITE_BURST_STATE_PORT1(MHL_CBUS_WRITE_BURST_SEND_DATA);
                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
            case MHL_INPUT_SELECT_PORT2:
                CLR_MHL_SEND_BIST_RETURN_STATUS2();
                SET_MHL_WRITE_BURST_STATE_PORT2(MHL_CBUS_WRITE_BURST_SEND_DATA);
                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
            case MHL_INPUT_SELECT_PORT3:
                CLR_MHL_SEND_BIST_RETURN_STATUS3();
                SET_MHL_WRITE_BURST_STATE_PORT3(MHL_CBUS_WRITE_BURST_SEND_DATA);
                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
            case MHL_INPUT_SELECT_PORT4:
                CLR_MHL_SEND_BIST_RETURN_STATUS4();
                SET_MHL_WRITE_BURST_STATE_PORT4(MHL_CBUS_WRITE_BURST_SEND_DATA);
                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
            case MHL_INPUT_SELECT_PORT5:
                CLR_MHL_SEND_BIST_RETURN_STATUS5();
                SET_MHL_WRITE_BURST_STATE_PORT5(MHL_CBUS_WRITE_BURST_SEND_DATA);
                break;
#endif

            default:

                break;
        };
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_SendWriteBurstInformation()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_SendWriteBurstInformation(BYTE ucPortSelect)
{
    Bool bSendWriteBurst = FALSE;
    BYTE ucWriteBurstType = MHL_CBUS_SEND_WRITE_BURST_NONE;

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            if(GET_MHL_SEND_USER_WRITE_BURST0())
            {
                ucWriteBurstType = MHL_CBUS_SEND_WRITE_BURST_USER_DATA;
            }
            else if(GET_MHL_SEND_3D_INFORMATION_PORT0())
            {
                ucWriteBurstType = MHL_CBUS_SEND_WRITE_BURST_3D_INFORMATION;
            }
            else if(GET_MHL_SEND_VC_INFORMATION_PORT0())
            {
                ucWriteBurstType = MHL_CBUS_SEND_WRITE_BURST_VIRTUAL_CHANNEL;
            }
            else if(GET_MHL_SEND_HIGH_END_VIDEO0())
            {
                ucWriteBurstType = MHL_CBUS_SEND_WRITE_BURST_HIGH_END_VIDEO;
            }
            else if(GET_MHL_SEND_AUDIO_DATA_TUNNEL0())
            {
                ucWriteBurstType = MHL_CBUS_SEND_WRITE_BURST_AUDIO_DATA_TUNNEL;
            }
            else if(GET_MHL_SEND_EMSC_DATA_TUNNEL0())
            {
                ucWriteBurstType = MHL_CBUS_SEND_WRITE_BURST_EMSC_DATA_TUNNEL;
            }
            else if(GET_MHL_SEND_BIST_RETURN_STATUS0())
            {
                ucWriteBurstType = MHL_CBUS_SEND_WRITE_BURST_BIST_RETURN_STATUS;
            }

            if(ucWriteBurstType != MHL_CBUS_SEND_WRITE_BURST_NONE)
            {
                if(GET_MHL_WRITE_BURST_STATE_PORT0() == MHL_CBUS_WRITE_BURST_NONE)
                {
                    SET_MHL_WRITE_BURST_STATE_PORT0(MHL_CBUS_WRITE_BURST_START);
                }
                else if(GET_MHL_WRITE_BURST_STATE_PORT0() == MHL_CBUS_WRITE_BURST_REVEIVE_GRANT)
                {
                    bSendWriteBurst = TRUE;
                }
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            if(GET_MHL_SEND_USER_WRITE_BURST1())
            {
                ucWriteBurstType = MHL_CBUS_SEND_WRITE_BURST_USER_DATA;
            }
            else if(GET_MHL_SEND_3D_INFORMATION_PORT1())
            {
                ucWriteBurstType = MHL_CBUS_SEND_WRITE_BURST_3D_INFORMATION;
            }
            else if(GET_MHL_SEND_VC_INFORMATION_PORT1())
            {
                ucWriteBurstType = MHL_CBUS_SEND_WRITE_BURST_VIRTUAL_CHANNEL;
            }
            else if(GET_MHL_SEND_HIGH_END_VIDEO1())
            {
                ucWriteBurstType = MHL_CBUS_SEND_WRITE_BURST_HIGH_END_VIDEO;
            }
            else if(GET_MHL_SEND_AUDIO_DATA_TUNNEL1())
            {
                ucWriteBurstType = MHL_CBUS_SEND_WRITE_BURST_AUDIO_DATA_TUNNEL;
            }
            else if(GET_MHL_SEND_EMSC_DATA_TUNNEL1())
            {
                ucWriteBurstType = MHL_CBUS_SEND_WRITE_BURST_EMSC_DATA_TUNNEL;
            }
            else if(GET_MHL_SEND_BIST_RETURN_STATUS1())
            {
                ucWriteBurstType = MHL_CBUS_SEND_WRITE_BURST_BIST_RETURN_STATUS;
            }

            if(ucWriteBurstType != MHL_CBUS_SEND_WRITE_BURST_NONE)
            {
                if(GET_MHL_WRITE_BURST_STATE_PORT1() == MHL_CBUS_WRITE_BURST_NONE)
                {
                    SET_MHL_WRITE_BURST_STATE_PORT1(MHL_CBUS_WRITE_BURST_START);
                }
                else if(GET_MHL_WRITE_BURST_STATE_PORT1() == MHL_CBUS_WRITE_BURST_REVEIVE_GRANT)
                {
                    bSendWriteBurst = TRUE;
                }
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            if(GET_MHL_SEND_USER_WRITE_BURST2())
            {
                ucWriteBurstType = MHL_CBUS_SEND_WRITE_BURST_USER_DATA;
            }
            else if(GET_MHL_SEND_3D_INFORMATION_PORT2())
            {
                ucWriteBurstType = MHL_CBUS_SEND_WRITE_BURST_3D_INFORMATION;
            }
            else if(GET_MHL_SEND_VC_INFORMATION_PORT2())
            {
                ucWriteBurstType = MHL_CBUS_SEND_WRITE_BURST_VIRTUAL_CHANNEL;
            }
            else if(GET_MHL_SEND_HIGH_END_VIDEO2())
            {
                ucWriteBurstType = MHL_CBUS_SEND_WRITE_BURST_HIGH_END_VIDEO;
            }
            else if(GET_MHL_SEND_AUDIO_DATA_TUNNEL2())
            {
                ucWriteBurstType = MHL_CBUS_SEND_WRITE_BURST_AUDIO_DATA_TUNNEL;
            }
            else if(GET_MHL_SEND_EMSC_DATA_TUNNEL2())
            {
                ucWriteBurstType = MHL_CBUS_SEND_WRITE_BURST_EMSC_DATA_TUNNEL;
            }
            else if(GET_MHL_SEND_BIST_RETURN_STATUS2())
            {
                ucWriteBurstType = MHL_CBUS_SEND_WRITE_BURST_BIST_RETURN_STATUS;
            }

            if(ucWriteBurstType != MHL_CBUS_SEND_WRITE_BURST_NONE)
            {
                if(GET_MHL_WRITE_BURST_STATE_PORT2() == MHL_CBUS_WRITE_BURST_NONE)
                {
                    SET_MHL_WRITE_BURST_STATE_PORT2(MHL_CBUS_WRITE_BURST_START);
                }
                else if(GET_MHL_WRITE_BURST_STATE_PORT2() == MHL_CBUS_WRITE_BURST_REVEIVE_GRANT)
                {
                    bSendWriteBurst = TRUE;
                }
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            if(GET_MHL_SEND_USER_WRITE_BURST3())
            {
                ucWriteBurstType = MHL_CBUS_SEND_WRITE_BURST_USER_DATA;
            }
            else if(GET_MHL_SEND_3D_INFORMATION_PORT3())
            {
                ucWriteBurstType = MHL_CBUS_SEND_WRITE_BURST_3D_INFORMATION;
            }
            else if(GET_MHL_SEND_VC_INFORMATION_PORT3())
            {
                ucWriteBurstType = MHL_CBUS_SEND_WRITE_BURST_VIRTUAL_CHANNEL;
            }
            else if(GET_MHL_SEND_HIGH_END_VIDEO3())
            {
                ucWriteBurstType = MHL_CBUS_SEND_WRITE_BURST_HIGH_END_VIDEO;
            }
            else if(GET_MHL_SEND_AUDIO_DATA_TUNNEL3())
            {
                ucWriteBurstType = MHL_CBUS_SEND_WRITE_BURST_AUDIO_DATA_TUNNEL;
            }
            else if(GET_MHL_SEND_EMSC_DATA_TUNNEL3())
            {
                ucWriteBurstType = MHL_CBUS_SEND_WRITE_BURST_EMSC_DATA_TUNNEL;
            }
            else if(GET_MHL_SEND_BIST_RETURN_STATUS3())
            {
                ucWriteBurstType = MHL_CBUS_SEND_WRITE_BURST_BIST_RETURN_STATUS;
            }

            if(ucWriteBurstType != MHL_CBUS_SEND_WRITE_BURST_NONE)
            {
                if(GET_MHL_WRITE_BURST_STATE_PORT3() == MHL_CBUS_WRITE_BURST_NONE)
                {
                    SET_MHL_WRITE_BURST_STATE_PORT3(MHL_CBUS_WRITE_BURST_START);
                }
                else if(GET_MHL_WRITE_BURST_STATE_PORT3() == MHL_CBUS_WRITE_BURST_REVEIVE_GRANT)
                {
                    bSendWriteBurst = TRUE;
                }
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            if(GET_MHL_SEND_USER_WRITE_BURST4())
            {
                ucWriteBurstType = MHL_CBUS_SEND_WRITE_BURST_USER_DATA;
            }
            else if(GET_MHL_SEND_3D_INFORMATION_PORT4())
            {
                ucWriteBurstType = MHL_CBUS_SEND_WRITE_BURST_3D_INFORMATION;
            }
            else if(GET_MHL_SEND_VC_INFORMATION_PORT4())
            {
                ucWriteBurstType = MHL_CBUS_SEND_WRITE_BURST_VIRTUAL_CHANNEL;
            }
            else if(GET_MHL_SEND_HIGH_END_VIDEO4())
            {
                ucWriteBurstType = MHL_CBUS_SEND_WRITE_BURST_HIGH_END_VIDEO;
            }
            else if(GET_MHL_SEND_AUDIO_DATA_TUNNEL4())
            {
                ucWriteBurstType = MHL_CBUS_SEND_WRITE_BURST_AUDIO_DATA_TUNNEL;
            }
            else if(GET_MHL_SEND_EMSC_DATA_TUNNEL4())
            {
                ucWriteBurstType = MHL_CBUS_SEND_WRITE_BURST_EMSC_DATA_TUNNEL;
            }
            else if(GET_MHL_SEND_BIST_RETURN_STATUS4())
            {
                ucWriteBurstType = MHL_CBUS_SEND_WRITE_BURST_BIST_RETURN_STATUS;
            }

            if(ucWriteBurstType != MHL_CBUS_SEND_WRITE_BURST_NONE)
            {
                if(GET_MHL_WRITE_BURST_STATE_PORT4() == MHL_CBUS_WRITE_BURST_NONE)
                {
                    SET_MHL_WRITE_BURST_STATE_PORT4(MHL_CBUS_WRITE_BURST_START);
                }
                else if(GET_MHL_WRITE_BURST_STATE_PORT4() == MHL_CBUS_WRITE_BURST_REVEIVE_GRANT)
                {
                    bSendWriteBurst = TRUE;
                }
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            if(GET_MHL_SEND_USER_WRITE_BURST5())
            {
                ucWriteBurstType = MHL_CBUS_SEND_WRITE_BURST_USER_DATA;
            }
            else if(GET_MHL_SEND_3D_INFORMATION_PORT5())
            {
                ucWriteBurstType = MHL_CBUS_SEND_WRITE_BURST_3D_INFORMATION;
            }
            else if(GET_MHL_SEND_VC_INFORMATION_PORT5())
            {
                ucWriteBurstType = MHL_CBUS_SEND_WRITE_BURST_VIRTUAL_CHANNEL;
            }
            else if(GET_MHL_SEND_HIGH_END_VIDEO5())
            {
                ucWriteBurstType = MHL_CBUS_SEND_WRITE_BURST_HIGH_END_VIDEO;
            }
            else if(GET_MHL_SEND_AUDIO_DATA_TUNNEL5())
            {
                ucWriteBurstType = MHL_CBUS_SEND_WRITE_BURST_AUDIO_DATA_TUNNEL;
            }
            else if(GET_MHL_SEND_EMSC_DATA_TUNNEL5())
            {
                ucWriteBurstType = MHL_CBUS_SEND_WRITE_BURST_EMSC_DATA_TUNNEL;
            }
            else if(GET_MHL_SEND_BIST_RETURN_STATUS5())
            {
                ucWriteBurstType = MHL_CBUS_SEND_WRITE_BURST_BIST_RETURN_STATUS;
            }

            if(ucWriteBurstType != MHL_CBUS_SEND_WRITE_BURST_NONE)
            {
                if(GET_MHL_WRITE_BURST_STATE_PORT5() == MHL_CBUS_WRITE_BURST_NONE)
                {
                    SET_MHL_WRITE_BURST_STATE_PORT5(MHL_CBUS_WRITE_BURST_START);
                }
                else if(GET_MHL_WRITE_BURST_STATE_PORT5() == MHL_CBUS_WRITE_BURST_REVEIVE_GRANT)
                {
                    bSendWriteBurst = TRUE;
                }
            }

            break;
#endif

        default:

            break;
    };

    if(bSendWriteBurst)
    {
        switch(ucWriteBurstType)
        {
            case MHL_CBUS_SEND_WRITE_BURST_3D_INFORMATION:
                _mdrv_mhl_Send3DTimingInformation(ucPortSelect);
                break;

            case MHL_CBUS_SEND_WRITE_BURST_VIRTUAL_CHANNEL:
                _mdrv_mhl_SendVirtualChannelInformation(ucPortSelect);
                break;

            case MHL_CBUS_SEND_WRITE_BURST_HIGH_END_VIDEO:
                _mdrv_mhl_SendHighEndVideoSupport(ucPortSelect);
                break;

            case MHL_CBUS_SEND_WRITE_BURST_AUDIO_DATA_TUNNEL:
                _mdrv_mhl_SendAudioDataTunnelSupport(ucPortSelect);
                break;

            case MHL_CBUS_SEND_WRITE_BURST_EMSC_DATA_TUNNEL:
                _mdrv_mhl_SendeMSCSupportInformation(ucPortSelect);
                break;

            case MHL_CBUS_SEND_WRITE_BURST_BIST_RETURN_STATUS:
                _mdrv_mhl_SendBISTReturnStatus(ucPortSelect);
                break;

            default:

                break;
        };
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_GetCbusAskData()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool _mdrv_mhl_GetCbusAskData(BYTE ucPortSelect, BYTE *ucData)
{
    Bool bGetData = FALSE;

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            if(GET_MHL_SEND_ASK_FINISH_PORT0())
            {
                CLR_MHL_SEND_ASK_FINISH_PORT0();

                *ucData = GET_MHL_CBUS_ASK_DATA_PORT0();

                bGetData = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            if(GET_MHL_SEND_ASK_FINISH_PORT1())
            {
                CLR_MHL_SEND_ASK_FINISH_PORT1();

                *ucData = GET_MHL_CBUS_ASK_DATA_PORT1();

                bGetData = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            if(GET_MHL_SEND_ASK_FINISH_PORT2())
            {
                CLR_MHL_SEND_ASK_FINISH_PORT2();

                *ucData = GET_MHL_CBUS_ASK_DATA_PORT2();

                bGetData = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            if(GET_MHL_SEND_ASK_FINISH_PORT3())
            {
                CLR_MHL_SEND_ASK_FINISH_PORT3();

                *ucData = GET_MHL_CBUS_ASK_DATA_PORT3();

                bGetData = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            if(GET_MHL_SEND_ASK_FINISH_PORT4())
            {
                CLR_MHL_SEND_ASK_FINISH_PORT4();

                *ucData = GET_MHL_CBUS_ASK_DATA_PORT4();

                bGetData = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            if(GET_MHL_SEND_ASK_FINISH_PORT5())
            {
                CLR_MHL_SEND_ASK_FINISH_PORT5();

                *ucData = GET_MHL_CBUS_ASK_DATA_PORT5();

                bGetData = TRUE;
            }

            break;
#endif

        default:

            break;
    };

    return bGetData;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusSendGetVenderID()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_CbusSendGetVenderID(BYTE ucPortSelect)
{
    CbusReq_S stQueue;

    stQueue.cmd = MSC_GET_VENDOR_ID;
    stQueue.len = 0;
    stQueue.reqStatus = CBUS_REQ_PENDING;

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            if((!GET_MHL_SEND_ASK_DATA_PORT0()) && (!GET_MHL_SEND_ASK_FINISH_PORT0()))
            {
                if(_mdrv_mhl_CbusTxQueueInsertMSC(ucPortSelect, &stQueue))
                {
                    SET_MHL_SEND_ASK_DATA_PORT0();
                    SET_MHL_SEND_GET_VENDER_ID_PORT0();
                }
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            if((!GET_MHL_SEND_ASK_DATA_PORT1()) && (!GET_MHL_SEND_ASK_FINISH_PORT1()))
            {
                if(_mdrv_mhl_CbusTxQueueInsertMSC(ucPortSelect, &stQueue))
                {
                    SET_MHL_SEND_ASK_DATA_PORT1();
                    SET_MHL_SEND_GET_VENDER_ID_PORT1();
                }
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            if((!GET_MHL_SEND_ASK_DATA_PORT2()) && (!GET_MHL_SEND_ASK_FINISH_PORT2()))
            {
                if(_mdrv_mhl_CbusTxQueueInsertMSC(ucPortSelect, &stQueue))
                {
                    SET_MHL_SEND_ASK_DATA_PORT2();
                    SET_MHL_SEND_GET_VENDER_ID_PORT2();
                }
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            if((!GET_MHL_SEND_ASK_DATA_PORT3()) && (!GET_MHL_SEND_ASK_FINISH_PORT3()))
            {
                if(_mdrv_mhl_CbusTxQueueInsertMSC(ucPortSelect, &stQueue))
                {
                    SET_MHL_SEND_ASK_DATA_PORT3();
                    SET_MHL_SEND_GET_VENDER_ID_PORT3();
                }
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            if((!GET_MHL_SEND_ASK_DATA_PORT4()) && (!GET_MHL_SEND_ASK_FINISH_PORT4()))
            {
                if(_mdrv_mhl_CbusTxQueueInsertMSC(ucPortSelect, &stQueue))
                {
                    SET_MHL_SEND_ASK_DATA_PORT4();
                    SET_MHL_SEND_GET_VENDER_ID_PORT4();
                }
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            if((!GET_MHL_SEND_ASK_DATA_PORT5()) && (!GET_MHL_SEND_ASK_FINISH_PORT5()))
            {
                if(_mdrv_mhl_CbusTxQueueInsertMSC(ucPortSelect, &stQueue))
                {
                    SET_MHL_SEND_ASK_DATA_PORT5();
                    SET_MHL_SEND_GET_VENDER_ID_PORT5();
                }
            }

            break;
#endif

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusReadVenderID()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool _mdrv_mhl_CbusReadVenderID(BYTE ucPortSelect, BYTE *ucVenderID, Bool bCheckReceive)
{
    Bool bGetData = FALSE;

    if(bCheckReceive)
    {
        if(_mdrv_mhl_GetCbusAskData(ucPortSelect, ucVenderID))
        {
            bGetData = TRUE;
        }
    }
    else
    {
        _mdrv_mhl_CbusSendGetVenderID(ucPortSelect);
    }

    return bGetData;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_GetDeviceVenderID()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool _mdrv_mhl_GetDeviceVenderID(BYTE ucPortSelect)
{
    Bool bFinish = FALSE;
    BYTE ucVenderID = 0;

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            if(GET_MHL_GET_VENDER_ID_ENABLE0())
            {
                if(_mdrv_mhl_CbusReadVenderID(ucPortSelect, &ucVenderID, GET_MHL_SEND_GET_VENDER_ID_PORT0()))
                {
                    SET_MHL_SOURCE_VENDER_ID_PORT0(ucVenderID);
                    CLR_MHL_GET_VENDER_ID_ENABLE0();
                    CLR_MHL_SEND_GET_VENDER_ID_PORT0();

                    bFinish = TRUE;
                }
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            if(GET_MHL_GET_VENDER_ID_ENABLE1())
            {
                if(_mdrv_mhl_CbusReadVenderID(ucPortSelect, &ucVenderID, GET_MHL_SEND_GET_VENDER_ID_PORT1()))
                {
                    SET_MHL_SOURCE_VENDER_ID_PORT1(ucVenderID);
                    CLR_MHL_GET_VENDER_ID_ENABLE1();
                    CLR_MHL_SEND_GET_VENDER_ID_PORT1();

                    bFinish = TRUE;
                }
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            if(GET_MHL_GET_VENDER_ID_ENABLE2())
            {
                if(_mdrv_mhl_CbusReadVenderID(ucPortSelect, &ucVenderID, GET_MHL_SEND_GET_VENDER_ID_PORT2()))
                {
                    SET_MHL_SOURCE_VENDER_ID_PORT2(ucVenderID);
                    CLR_MHL_GET_VENDER_ID_ENABLE2();
                    CLR_MHL_SEND_GET_VENDER_ID_PORT2();

                    bFinish = TRUE;
                }
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            if(GET_MHL_GET_VENDER_ID_ENABLE3())
            {
                if(_mdrv_mhl_CbusReadVenderID(ucPortSelect, &ucVenderID, GET_MHL_SEND_GET_VENDER_ID_PORT3()))
                {
                    SET_MHL_SOURCE_VENDER_ID_PORT3(ucVenderID);
                    CLR_MHL_GET_VENDER_ID_ENABLE3();
                    CLR_MHL_SEND_GET_VENDER_ID_PORT3();

                    bFinish = TRUE;
                }
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            if(GET_MHL_GET_VENDER_ID_ENABLE4())
            {
                if(_mdrv_mhl_CbusReadVenderID(ucPortSelect, &ucVenderID, GET_MHL_SEND_GET_VENDER_ID_PORT4()))
                {
                    SET_MHL_SOURCE_VENDER_ID_PORT4(ucVenderID);
                    CLR_MHL_GET_VENDER_ID_ENABLE4();
                    CLR_MHL_SEND_GET_VENDER_ID_PORT4();

                    bFinish = TRUE;
                }
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            if(GET_MHL_GET_VENDER_ID_ENABLE5())
            {
                if(_mdrv_mhl_CbusReadVenderID(ucPortSelect, &ucVenderID, GET_MHL_SEND_GET_VENDER_ID_PORT5()))
                {
                    SET_MHL_SOURCE_VENDER_ID_PORT5(ucVenderID);
                    CLR_MHL_GET_VENDER_ID_ENABLE5();
                    CLR_MHL_SEND_GET_VENDER_ID_PORT5();

                    bFinish = TRUE;
                }
            }

            break;
#endif

        default:

            break;
    };

    return bFinish;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_ReadDeviceVenderID()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_CheckDeviceVenderID(BYTE ucPortSelect)
{
    BYTE ucVenderID = 0;

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            ucVenderID = GET_MHL_SOURCE_VENDER_ID_PORT0();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            ucVenderID = GET_MHL_SOURCE_VENDER_ID_PORT1();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            ucVenderID = GET_MHL_SOURCE_VENDER_ID_PORT2();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            ucVenderID = GET_MHL_SOURCE_VENDER_ID_PORT3();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            ucVenderID = GET_MHL_SOURCE_VENDER_ID_PORT4();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            ucVenderID = GET_MHL_SOURCE_VENDER_ID_PORT5();
            break;
#endif

        default:

            break;
    };

    MHL_DRV_DPRINTF("** MHL get vender ID %x", ucVenderID);
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_ReadDeviceVenderID()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_ReadDeviceVenderID(BYTE ucPortSelect)
{
    if(_mdrv_mhl_GetDeviceVenderID(ucPortSelect))
    {
        _mdrv_mhl_CheckDeviceVenderID(ucPortSelect);
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusSendReadDevcap()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_CbusSendReadDevcap(BYTE ucPortSelect, BYTE ucBitNumber, Bool bExtended)
{
    CbusReq_S stQueue;

    if(bExtended)
    {
        stQueue.cmd = MSC_READ_XDEVCAP;
        stQueue.msgData[0] = 0x80 +ucBitNumber;
    }
    else
    {
        stQueue.cmd = MSC_READ_DEVCAP;
        stQueue.msgData[0] = ucBitNumber;
    }

    stQueue.len = 1;
    stQueue.reqStatus = CBUS_REQ_PENDING;

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            if((!GET_MHL_SEND_ASK_DATA_PORT0()) && (!GET_MHL_SEND_ASK_FINISH_PORT0()))
            {
                if(_mdrv_mhl_CbusTxQueueInsertMSC(ucPortSelect, &stQueue))
                {
                    SET_MHL_SEND_ASK_DATA_PORT0();
                    SET_MHL_SEND_READ_DEVCAP_PORT0();
                }
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            if((!GET_MHL_SEND_ASK_DATA_PORT1()) && (!GET_MHL_SEND_ASK_FINISH_PORT1()))
            {
                if(_mdrv_mhl_CbusTxQueueInsertMSC(ucPortSelect, &stQueue))
                {
                    SET_MHL_SEND_ASK_DATA_PORT1();
                    SET_MHL_SEND_READ_DEVCAP_PORT1();
                }
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            if((!GET_MHL_SEND_ASK_DATA_PORT2()) && (!GET_MHL_SEND_ASK_FINISH_PORT2()))
            {
                if(_mdrv_mhl_CbusTxQueueInsertMSC(ucPortSelect, &stQueue))
                {
                    SET_MHL_SEND_ASK_DATA_PORT2();
                    SET_MHL_SEND_READ_DEVCAP_PORT2();
                }
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            if((!GET_MHL_SEND_ASK_DATA_PORT3()) && (!GET_MHL_SEND_ASK_FINISH_PORT3()))
            {
                if(_mdrv_mhl_CbusTxQueueInsertMSC(ucPortSelect, &stQueue))
                {
                    SET_MHL_SEND_ASK_DATA_PORT3();
                    SET_MHL_SEND_READ_DEVCAP_PORT3();
                }
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            if((!GET_MHL_SEND_ASK_DATA_PORT4()) && (!GET_MHL_SEND_ASK_FINISH_PORT4()))
            {
                if(_mdrv_mhl_CbusTxQueueInsertMSC(ucPortSelect, &stQueue))
                {
                    SET_MHL_SEND_ASK_DATA_PORT4();
                    SET_MHL_SEND_READ_DEVCAP_PORT4();
                }
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            if((!GET_MHL_SEND_ASK_DATA_PORT5()) && (!GET_MHL_SEND_ASK_FINISH_PORT5()))
            {
                if(_mdrv_mhl_CbusTxQueueInsertMSC(ucPortSelect, &stQueue))
                {
                    SET_MHL_SEND_ASK_DATA_PORT5();
                    SET_MHL_SEND_READ_DEVCAP_PORT5();
                }
            }

            break;
#endif

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusReadDeviceCapacibility()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool _mdrv_mhl_CbusReadDeviceCapacibility(BYTE ucPortSelect, BYTE ucBitNumber, BYTE *ucDevCapData, Bool bCheckReceive, Bool bExtended)
{
    Bool bGetData = FALSE;

    if(bCheckReceive)
    {
        if(_mdrv_mhl_GetCbusAskData(ucPortSelect, ucDevCapData))
        {
            bGetData = TRUE;
        }
    }
    else
    {
        _mdrv_mhl_CbusSendReadDevcap(ucPortSelect, ucBitNumber, bExtended);
    }

    return bGetData;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_GetDeviceCapacibility()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool _mdrv_mhl_GetDeviceCapacibility(BYTE ucPortSelect)
{
    Bool bFinish = FALSE;
    Bool bCheckAgain = FALSE;
    BYTE ucBitNumber = 0;
    BYTE ucDevCapData = 0;
    WORD usDevCapMask = 0;

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            if(GET_MHL_READ_DEVCAP_ENABLE0())
            {
                usDevCapMask = GET_MHL_READ_DEVCAP_MASK_PORT0();

                do
                {
                    bCheckAgain = FALSE;

                    if(usDevCapMask & MHL_READ_DEVCAP_MASK_PORT0)
                    {
                        ucBitNumber = _mdrv_mhl_GetLeastBitNum(usDevCapMask);

                        if(_mdrv_mhl_CbusReadDeviceCapacibility(ucPortSelect, ucBitNumber, &ucDevCapData, GET_MHL_SEND_READ_DEVCAP_PORT0(), FALSE))
                        {
                            CLR_MHL_SEND_READ_DEVCAP_PORT0();

                            usDevCapMask = MASKSET(usDevCapMask, 0, _BIT(ucBitNumber));

                            SET_MHL_READ_DEVCAP_MASK_PORT0(usDevCapMask);
                            SET_MHL_SOURCE_DEVCAP_PORT0(ucBitNumber, ucDevCapData);

                            bCheckAgain = TRUE;
                        }
                    }
                    else
                    {
                        bFinish = TRUE;

                        CLR_MHL_READ_DEVCAP_ENABLE0();
                    }
                }
                while(bCheckAgain);
            }
            else
            {
                SET_MHL_READ_DEVCAP_MASK_PORT0(MHL_READ_DEVCAP_MASK_PORT0);
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            if(GET_MHL_READ_DEVCAP_ENABLE1())
            {
                usDevCapMask = GET_MHL_READ_DEVCAP_MASK_PORT1();

                do
                {
                    bCheckAgain = FALSE;

                    if(usDevCapMask & MHL_READ_DEVCAP_MASK_PORT1)
                    {
                        ucBitNumber = _mdrv_mhl_GetLeastBitNum(usDevCapMask);

                        if(_mdrv_mhl_CbusReadDeviceCapacibility(ucPortSelect, ucBitNumber, &ucDevCapData, GET_MHL_SEND_READ_DEVCAP_PORT1(), FALSE))
                        {
                            CLR_MHL_SEND_READ_DEVCAP_PORT1();

                            usDevCapMask = MASKSET(usDevCapMask, 0, _BIT(ucBitNumber));

                            SET_MHL_READ_DEVCAP_MASK_PORT1(usDevCapMask);
                            SET_MHL_SOURCE_DEVCAP_PORT1(ucBitNumber, ucDevCapData);

                            bCheckAgain = TRUE;
                        }
                    }
                    else
                    {
                        bFinish = TRUE;

                        CLR_MHL_READ_DEVCAP_ENABLE1();
                    }
                }
                while(bCheckAgain);
            }
            else
            {
                SET_MHL_READ_DEVCAP_MASK_PORT1(MHL_READ_DEVCAP_MASK_PORT1);
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            if(GET_MHL_READ_DEVCAP_ENABLE2())
            {
                usDevCapMask = GET_MHL_READ_DEVCAP_MASK_PORT2();

                do
                {
                    bCheckAgain = FALSE;

                    if(usDevCapMask & MHL_READ_DEVCAP_MASK_PORT2)
                    {
                        ucBitNumber = _mdrv_mhl_GetLeastBitNum(usDevCapMask);

                        if(_mdrv_mhl_CbusReadDeviceCapacibility(ucPortSelect, ucBitNumber, &ucDevCapData, GET_MHL_SEND_READ_DEVCAP_PORT2(), FALSE))
                        {
                            CLR_MHL_SEND_READ_DEVCAP_PORT2();

                            usDevCapMask = MASKSET(usDevCapMask, 0, _BIT(ucBitNumber));

                            SET_MHL_READ_DEVCAP_MASK_PORT2(usDevCapMask);
                            SET_MHL_SOURCE_DEVCAP_PORT2(ucBitNumber, ucDevCapData);

                            bCheckAgain = TRUE;
                        }
                    }
                    else
                    {
                        bFinish = TRUE;

                        CLR_MHL_READ_DEVCAP_ENABLE2();
                    }
                }
                while(bCheckAgain);
            }
            else
            {
                SET_MHL_READ_DEVCAP_MASK_PORT2(MHL_READ_DEVCAP_MASK_PORT2);
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            if(GET_MHL_READ_DEVCAP_ENABLE3())
            {
                usDevCapMask = GET_MHL_READ_DEVCAP_MASK_PORT3();

                do
                {
                    bCheckAgain = FALSE;

                    if(usDevCapMask & MHL_READ_DEVCAP_MASK_PORT3)
                    {
                        ucBitNumber = _mdrv_mhl_GetLeastBitNum(usDevCapMask);

                        if(_mdrv_mhl_CbusReadDeviceCapacibility(ucPortSelect, ucBitNumber, &ucDevCapData, GET_MHL_SEND_READ_DEVCAP_PORT3(), FALSE))
                        {
                            CLR_MHL_SEND_READ_DEVCAP_PORT3();

                            usDevCapMask = MASKSET(usDevCapMask, 0, _BIT(ucBitNumber));

                            SET_MHL_READ_DEVCAP_MASK_PORT3(usDevCapMask);
                            SET_MHL_SOURCE_DEVCAP_PORT3(ucBitNumber, ucDevCapData);

                            bCheckAgain = TRUE;
                        }
                    }
                    else
                    {
                        bFinish = TRUE;

                        CLR_MHL_READ_DEVCAP_ENABLE3();
                    }
                }
                while(bCheckAgain);
            }
            else
            {
                SET_MHL_READ_DEVCAP_MASK_PORT3(MHL_READ_DEVCAP_MASK_PORT3);
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            if(GET_MHL_READ_DEVCAP_ENABLE4())
            {
                usDevCapMask = GET_MHL_READ_DEVCAP_MASK_PORT4();

                do
                {
                    bCheckAgain = FALSE;

                    if(usDevCapMask & MHL_READ_DEVCAP_MASK_PORT0)
                    {
                        ucBitNumber = _mdrv_mhl_GetLeastBitNum(usDevCapMask);

                        if(_mdrv_mhl_CbusReadDeviceCapacibility(ucPortSelect, ucBitNumber, &ucDevCapData, GET_MHL_SEND_READ_DEVCAP_PORT4(), FALSE))
                        {
                            CLR_MHL_SEND_READ_DEVCAP_PORT4();

                            usDevCapMask = MASKSET(usDevCapMask, 0, _BIT(ucBitNumber));

                            SET_MHL_READ_DEVCAP_MASK_PORT4(usDevCapMask);
                            SET_MHL_SOURCE_DEVCAP_PORT4(ucBitNumber, ucDevCapData);

                            bCheckAgain = TRUE;
                        }
                    }
                    else
                    {
                        bFinish = TRUE;

                        CLR_MHL_READ_DEVCAP_ENABLE4();
                    }
                }
                while(bCheckAgain);
            }
            else
            {
                SET_MHL_READ_DEVCAP_MASK_PORT4(MHL_READ_DEVCAP_MASK_PORT4);
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            if(GET_MHL_READ_DEVCAP_ENABLE5())
            {
                usDevCapMask = GET_MHL_READ_DEVCAP_MASK_PORT5();

                do
                {
                    bCheckAgain = FALSE;

                    if(usDevCapMask & MHL_READ_DEVCAP_MASK_PORT5)
                    {
                        ucBitNumber = _mdrv_mhl_GetLeastBitNum(usDevCapMask);

                        if(_mdrv_mhl_CbusReadDeviceCapacibility(ucPortSelect, ucBitNumber, &ucDevCapData, GET_MHL_SEND_READ_DEVCAP_PORT5(), FALSE))
                        {
                            CLR_MHL_SEND_READ_DEVCAP_PORT5();

                            usDevCapMask = MASKSET(usDevCapMask, 0, _BIT(ucBitNumber));

                            SET_MHL_READ_DEVCAP_MASK_PORT5(usDevCapMask);
                            SET_MHL_SOURCE_DEVCAP_PORT5(ucBitNumber, ucDevCapData);

                            bCheckAgain = TRUE;
                        }
                    }
                    else
                    {
                        bFinish = TRUE;

                        CLR_MHL_READ_DEVCAP_ENABLE5();
                    }
                }
                while(bCheckAgain);
            }
            else
            {
                SET_MHL_READ_DEVCAP_MASK_PORT5(MHL_READ_DEVCAP_MASK_PORT5);
            }

            break;
#endif

        default:

            break;
    };

    return bFinish;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CheckDeviceCapacibility()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_CheckDeviceCapacibility(BYTE ucPortSelect, Bool bExtended)
{
    Bool bECbusSupport = _mdrv_mhl_GetECbusSupportFlag(ucPortSelect);
    BYTE uctemp = 0;
    BYTE *ucDevcap = 0;
    WORD usAdopterID = 0;
    WORD usDeviceID = 0;

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            ucDevcap = &GET_MHL_SOURCE_DEVCAP_PORT0(0);

            if(ucDevcap[MHL_CBUS_FEATURE_FLAG] & BIT0)
            {
                SET_MHL_SEND_RCP_ENABLE_PORT0();
            }

            if(ucDevcap[MHL_CBUS_FEATURE_FLAG] & BIT1)
            {
                SET_MHL_SEND_RAP_ENABLE_PORT0();
            }

            if(ucDevcap[MHL_CBUS_FEATURE_FLAG] & BIT3)
            {
                SET_MHL_SEND_UCP_ENABLE_PORT0();
            }

            if(bECbusSupport && (ucDevcap[MHL_CBUS_VERSION] >= 0x30))
            {
                SET_MHL_ENABLE_CBUS_ENHANCE0();
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            ucDevcap = &GET_MHL_SOURCE_DEVCAP_PORT1(0);

            if(ucDevcap[MHL_CBUS_FEATURE_FLAG] & BIT0)
            {
                SET_MHL_SEND_RCP_ENABLE_PORT1();
            }

            if(ucDevcap[MHL_CBUS_FEATURE_FLAG] & BIT1)
            {
                SET_MHL_SEND_RAP_ENABLE_PORT1();
            }

            if(ucDevcap[MHL_CBUS_FEATURE_FLAG] & BIT3)
            {
                SET_MHL_SEND_UCP_ENABLE_PORT1();
            }

            if(bECbusSupport && (ucDevcap[MHL_CBUS_VERSION] >= 0x30))
            {
                SET_MHL_ENABLE_CBUS_ENHANCE1();
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            ucDevcap = &GET_MHL_SOURCE_DEVCAP_PORT2(0);

            if(ucDevcap[MHL_CBUS_FEATURE_FLAG] & BIT0)
            {
                SET_MHL_SEND_RCP_ENABLE_PORT2();
            }

            if(ucDevcap[MHL_CBUS_FEATURE_FLAG] & BIT1)
            {
                SET_MHL_SEND_RAP_ENABLE_PORT2();
            }

            if(ucDevcap[MHL_CBUS_FEATURE_FLAG] & BIT3)
            {
                SET_MHL_SEND_UCP_ENABLE_PORT2();
            }

            if(bECbusSupport && (ucDevcap[MHL_CBUS_VERSION] >= 0x30))
            {
                SET_MHL_ENABLE_CBUS_ENHANCE2();
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            ucDevcap = &GET_MHL_SOURCE_DEVCAP_PORT3(0);

            if(ucDevcap[MHL_CBUS_FEATURE_FLAG] & BIT0)
            {
                SET_MHL_SEND_RCP_ENABLE_PORT3();
            }

            if(ucDevcap[MHL_CBUS_FEATURE_FLAG] & BIT1)
            {
                SET_MHL_SEND_RAP_ENABLE_PORT3();
            }

            if(ucDevcap[MHL_CBUS_FEATURE_FLAG] & BIT3)
            {
                SET_MHL_SEND_UCP_ENABLE_PORT3();
            }

            if(bECbusSupport && (ucDevcap[MHL_CBUS_VERSION] >= 0x30))
            {
                SET_MHL_ENABLE_CBUS_ENHANCE3();
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            ucDevcap = &GET_MHL_SOURCE_DEVCAP_PORT4(0);

            if(ucDevcap[MHL_CBUS_FEATURE_FLAG] & BIT0)
            {
                SET_MHL_SEND_RCP_ENABLE_PORT4();
            }

            if(ucDevcap[MHL_CBUS_FEATURE_FLAG] & BIT1)
            {
                SET_MHL_SEND_RAP_ENABLE_PORT4();
            }

            if(ucDevcap[MHL_CBUS_FEATURE_FLAG] & BIT3)
            {
                SET_MHL_SEND_UCP_ENABLE_PORT4();
            }

            if(bECbusSupport && (ucDevcap[MHL_CBUS_VERSION] >= 0x30))
            {
                SET_MHL_ENABLE_CBUS_ENHANCE4();
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            ucDevcap = &GET_MHL_SOURCE_DEVCAP_PORT5(0);

            if(ucDevcap[MHL_CBUS_FEATURE_FLAG] & BIT0)
            {
                SET_MHL_SEND_RCP_ENABLE_PORT5();
            }

            if(ucDevcap[MHL_CBUS_FEATURE_FLAG] & BIT1)
            {
                SET_MHL_SEND_RAP_ENABLE_PORT5();
            }

            if(ucDevcap[MHL_CBUS_FEATURE_FLAG] & BIT3)
            {
                SET_MHL_SEND_UCP_ENABLE_PORT5();
            }

            if(bECbusSupport && (ucDevcap[MHL_CBUS_VERSION] >= 0x30))
            {
                SET_MHL_ENABLE_CBUS_ENHANCE5();
            }

            break;
#endif

        default:

            break;
    };

    if(bExtended)
    {
        for(uctemp = 0; uctemp <MHL_XDEVICE_CAPABILITY_SIZE ;uctemp++)
        {
            MHL_DRV_DPRINTF("** MHL read xdevcap %x", *(ucDevcap +MHL_DEVICE_CAPABILITY_SIZE +uctemp));
        }
    }
    else
    {
        for(uctemp = 0; uctemp <MHL_DEVICE_CAPABILITY_SIZE ;uctemp++)
        {
            MHL_DRV_DPRINTF("** MHL read devcap %x", *(ucDevcap +uctemp));
        }
    }

    usAdopterID = *(ucDevcap +MHL_CBUS_ADOPTER_ID_L) |((*(ucDevcap +MHL_CBUS_ADOPTER_ID_H)) << 8);
    usDeviceID = *(ucDevcap +MHL_CBUS_DEVICE_ID_L) |((*(ucDevcap +MHL_CBUS_DEVICE_ID_H)) << 8);

    _mdrv_mhl_InsertCellPhoneID(ucPortSelect, usAdopterID, usDeviceID);
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_ReadDeviceCapacibility()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_ReadDeviceCapacibility(BYTE ucPortSelect)
{
    if(_mdrv_mhl_GetDeviceCapacibility(ucPortSelect))
    {
        _mdrv_mhl_CheckDeviceCapacibility(ucPortSelect, FALSE); 		
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_GetExtendDeviceCapacibility()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool _mdrv_mhl_GetExtendDeviceCapacibility(BYTE ucPortSelect)
{
    Bool bFinish = FALSE;
    Bool bCheckAgain = FALSE;
    BYTE ucBitNumber = 0;
    BYTE ucDevCapData = 0;
    WORD usExtendDevCapMask = 0;

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            if(GET_MHL_READ_XDEVCAP_ENABLE0())
            {
                usExtendDevCapMask = GET_MHL_EXTEND_READ_DEVCAP_MASK0();

                do
                {
                    bCheckAgain = FALSE;

                    if(usExtendDevCapMask & MHL_READ_EXTEND_DEVCAP_MASK0)
                    {
                        ucBitNumber = _mdrv_mhl_GetLeastBitNum(usExtendDevCapMask);

                        if(_mdrv_mhl_CbusReadDeviceCapacibility(ucPortSelect, ucBitNumber, &ucDevCapData, GET_MHL_SEND_READ_DEVCAP_PORT0(), TRUE))
                        {
                            CLR_MHL_SEND_READ_DEVCAP_PORT0();

                            usExtendDevCapMask = MASKSET(usExtendDevCapMask, 0, _BIT(ucBitNumber));

                            SET_MHL_EXTEND_READ_DEVCAP_MASK0(usExtendDevCapMask);
                            SET_MHL_SOURCE_DEVCAP_PORT0(MHL_DEVICE_CAPABILITY_SIZE +ucBitNumber, ucDevCapData);

                            bCheckAgain = TRUE;
                        }
                    }
                    else
                    {
                        bFinish = TRUE;

                        CLR_MHL_READ_XDEVCAP_ENABLE0();
                    }
                }
                while(bCheckAgain);
            }
            else
            {
                SET_MHL_EXTEND_READ_DEVCAP_MASK0(MHL_READ_EXTEND_DEVCAP_MASK0);
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            if(GET_MHL_READ_XDEVCAP_ENABLE1())
            {
                usExtendDevCapMask = GET_MHL_EXTEND_READ_DEVCAP_MASK1();

                do
                {
                    bCheckAgain = FALSE;

                    if(usExtendDevCapMask & MHL_READ_EXTEND_DEVCAP_MASK1)
                    {
                        ucBitNumber = _mdrv_mhl_GetLeastBitNum(usExtendDevCapMask);

                        if(_mdrv_mhl_CbusReadDeviceCapacibility(ucPortSelect, ucBitNumber, &ucDevCapData, GET_MHL_SEND_READ_DEVCAP_PORT1(), TRUE))
                        {
                            CLR_MHL_SEND_READ_DEVCAP_PORT1();

                            usExtendDevCapMask = MASKSET(usExtendDevCapMask, 0, _BIT(ucBitNumber));

                            SET_MHL_EXTEND_READ_DEVCAP_MASK1(usExtendDevCapMask);
                            SET_MHL_SOURCE_DEVCAP_PORT1(MHL_DEVICE_CAPABILITY_SIZE +ucBitNumber, ucDevCapData);

                            bCheckAgain = TRUE;
                        }
                    }
                    else
                    {
                        bFinish = TRUE;

                        CLR_MHL_READ_XDEVCAP_ENABLE1();
                    }
                }
                while(bCheckAgain);
            }
            else
            {
                SET_MHL_EXTEND_READ_DEVCAP_MASK1(MHL_READ_EXTEND_DEVCAP_MASK1);
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            if(GET_MHL_READ_XDEVCAP_ENABLE2())
            {
                usExtendDevCapMask = GET_MHL_EXTEND_READ_DEVCAP_MASK2();

                do
                {
                    bCheckAgain = FALSE;

                    if(usExtendDevCapMask & MHL_READ_EXTEND_DEVCAP_MASK2)
                    {
                        ucBitNumber = _mdrv_mhl_GetLeastBitNum(usExtendDevCapMask);

                        if(_mdrv_mhl_CbusReadDeviceCapacibility(ucPortSelect, ucBitNumber, &ucDevCapData, GET_MHL_SEND_READ_DEVCAP_PORT2(), TRUE))
                        {
                            CLR_MHL_SEND_READ_DEVCAP_PORT2();

                            usExtendDevCapMask = MASKSET(usExtendDevCapMask, 0, _BIT(ucBitNumber));

                            SET_MHL_EXTEND_READ_DEVCAP_MASK2(usExtendDevCapMask);
                            SET_MHL_SOURCE_DEVCAP_PORT2(MHL_DEVICE_CAPABILITY_SIZE +ucBitNumber, ucDevCapData);

                            bCheckAgain = TRUE;
                        }
                    }
                    else
                    {
                        bFinish = TRUE;

                        CLR_MHL_READ_XDEVCAP_ENABLE2();
                    }
                }
                while(bCheckAgain);
            }
            else
            {
                SET_MHL_EXTEND_READ_DEVCAP_MASK2(MHL_READ_EXTEND_DEVCAP_MASK2);
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            if(GET_MHL_READ_XDEVCAP_ENABLE3())
            {
                usExtendDevCapMask = GET_MHL_EXTEND_READ_DEVCAP_MASK3();

                do
                {
                    bCheckAgain = FALSE;

                    if(usExtendDevCapMask & MHL_READ_EXTEND_DEVCAP_MASK3)
                    {
                        ucBitNumber = _mdrv_mhl_GetLeastBitNum(usExtendDevCapMask);

                        if(_mdrv_mhl_CbusReadDeviceCapacibility(ucPortSelect, ucBitNumber, &ucDevCapData, GET_MHL_SEND_READ_DEVCAP_PORT3(), TRUE))
                        {
                            CLR_MHL_SEND_READ_DEVCAP_PORT3();

                            usExtendDevCapMask = MASKSET(usExtendDevCapMask, 0, _BIT(ucBitNumber));

                            SET_MHL_EXTEND_READ_DEVCAP_MASK3(usExtendDevCapMask);
                            SET_MHL_SOURCE_DEVCAP_PORT3(MHL_DEVICE_CAPABILITY_SIZE +ucBitNumber, ucDevCapData);

                            bCheckAgain = TRUE;
                        }
                    }
                    else
                    {
                        bFinish = TRUE;

                        CLR_MHL_READ_XDEVCAP_ENABLE3();
                    }
                }
                while(bCheckAgain);
            }
            else
            {
                SET_MHL_EXTEND_READ_DEVCAP_MASK3(MHL_READ_EXTEND_DEVCAP_MASK3);
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            if(GET_MHL_READ_XDEVCAP_ENABLE4())
            {
                usExtendDevCapMask = GET_MHL_EXTEND_READ_DEVCAP_MASK4();

                do
                {
                    bCheckAgain = FALSE;

                    if(usExtendDevCapMask & MHL_READ_EXTEND_DEVCAP_MASK4)
                    {
                        ucBitNumber = _mdrv_mhl_GetLeastBitNum(usExtendDevCapMask);

                        if(_mdrv_mhl_CbusReadDeviceCapacibility(ucPortSelect, ucBitNumber, &ucDevCapData, GET_MHL_SEND_READ_DEVCAP_PORT4(), TRUE))
                        {
                            CLR_MHL_SEND_READ_DEVCAP_PORT4();

                            usExtendDevCapMask = MASKSET(usExtendDevCapMask, 0, _BIT(ucBitNumber));

                            SET_MHL_EXTEND_READ_DEVCAP_MASK4(usExtendDevCapMask);
                            SET_MHL_SOURCE_DEVCAP_PORT4(MHL_DEVICE_CAPABILITY_SIZE +ucBitNumber, ucDevCapData);

                            bCheckAgain = TRUE;
                        }
                    }
                    else
                    {
                        bFinish = TRUE;

                        CLR_MHL_READ_XDEVCAP_ENABLE4();
                    }
                }
                while(bCheckAgain);
            }
            else
            {
                SET_MHL_EXTEND_READ_DEVCAP_MASK4(MHL_READ_EXTEND_DEVCAP_MASK4);
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            if(GET_MHL_READ_XDEVCAP_ENABLE5())
            {
                usExtendDevCapMask = GET_MHL_EXTEND_READ_DEVCAP_MASK5();

                do
                {
                    bCheckAgain = FALSE;

                    if(usExtendDevCapMask & MHL_READ_EXTEND_DEVCAP_MASK5)
                    {
                        ucBitNumber = _mdrv_mhl_GetLeastBitNum(usExtendDevCapMask);

                        if(_mdrv_mhl_CbusReadDeviceCapacibility(ucPortSelect, ucBitNumber, &ucDevCapData, GET_MHL_SEND_READ_DEVCAP_PORT5(), TRUE))
                        {
                            CLR_MHL_SEND_READ_DEVCAP_PORT5();

                            usExtendDevCapMask = MASKSET(usExtendDevCapMask, 0, _BIT(ucBitNumber));

                            SET_MHL_EXTEND_READ_DEVCAP_MASK5(usExtendDevCapMask);
                            SET_MHL_SOURCE_DEVCAP_PORT5(MHL_DEVICE_CAPABILITY_SIZE +ucBitNumber, ucDevCapData);

                            bCheckAgain = TRUE;
                        }
                    }
                    else
                    {
                        bFinish = TRUE;

                        CLR_MHL_READ_XDEVCAP_ENABLE5();
                    }
                }
                while(bCheckAgain);
            }
            else
            {
                SET_MHL_EXTEND_READ_DEVCAP_MASK5(MHL_READ_EXTEND_DEVCAP_MASK5);
            }

            break;
#endif

        default:

            break;
    };

    return bFinish;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_ReadExtendDeviceCapacibility()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_ReadExtendDeviceCapacibility(BYTE ucPortSelect)
{
    if(_mdrv_mhl_GetExtendDeviceCapacibility(ucPortSelect))
    {
        _mdrv_mhl_CheckDeviceCapacibility(ucPortSelect, TRUE);
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mapi_mhl_ReadDeviceInformation()
//  [Description]
//                  MHL read device information
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_ReadDeviceInformation(BYTE ucPortSelect)
{
    _mdrv_mhl_ReadDeviceVenderID(ucPortSelect);

    _mdrv_mhl_ReadDeviceCapacibility(ucPortSelect);
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_GetCbusReceiveMSC()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE _mdrv_mhl_GetCbusReceiveMSC(BYTE ucPortSelect, BYTE ucCommandAddress, BYTE *ucReceiveMSC)
{
    BYTE uctemp = 0;
    BYTE ucLength = mhal_mhl_GetRxXdataValue(ucPortSelect, ucCommandAddress);
    BYTE ucValue = 0;

    ucReceiveMSC[0] = 0;

    MHL_DRV_RXDEBUG(" MHL check address = %x", ucCommandAddress);

    if((ucLength & BIT7) == BIT7) // Get MSG time out
    {
        MHL_DRV_RXDEBUG(" MHL get MSC time out port %d", ucPortSelect);

        ucCommandAddress += (ucLength &MASKBIT(6:0));
    }
    else // Get MSG not time out
    {
        MHL_DRV_RXDEBUG(" MHL get MSC length = %x", ucLength);

        if(ucLength == 0) // Null command
        {
            ucCommandAddress = 0;
        }
        else
        {
            // Get MSC length
            ucReceiveMSC[0] = (ucLength -1) /2;

            // Get MSC command
            if((mhal_mhl_GetRxXdataValue(ucPortSelect, ucCommandAddress +1) &BIT0) == BIT0) // First packet is command
            {
                ucReceiveMSC[1] = mhal_mhl_GetRxXdataValue(ucPortSelect, ucCommandAddress +2);
                MHL_DRV_RXDEBUG(" MHL get MSC command = %x", ucReceiveMSC[1]);

                // Get MSC data
                for(uctemp = 4; uctemp < ucLength; uctemp += 2)
                {
                    if((mhal_mhl_GetRxXdataValue(ucPortSelect, ucCommandAddress +uctemp -1) &BIT0) == BIT0) // Command in data happen
                    {
                        ucValue = mhal_mhl_GetRxXdataValue(ucPortSelect, ucCommandAddress +uctemp);

                        // For CTS 6.3.6.5 entry 4
                        if(ucValue == MSC_ABORT)
                        {
                            // Change command to ABORT
                            ucReceiveMSC[1] = MSC_ABORT;
                            // Change MSC length to 1
                            ucReceiveMSC[0] = 1;

                            break;
                        }
                        else if(ucValue == MSC_EOF)
                        {
                            // EOF is command but it is not error
                            ucReceiveMSC[uctemp /2] = mhal_mhl_GetRxXdataValue(ucPortSelect, ucCommandAddress +uctemp);
                        }
                        else
                        {
                            // Change MSC length to 0
                            ucReceiveMSC[0] = 0;
                        }
                    }
                    else // Data is normal
                    {
                        ucReceiveMSC[uctemp /2] = mhal_mhl_GetRxXdataValue(ucPortSelect, ucCommandAddress +uctemp);
                        MHL_DRV_RXDEBUG(" MHL get MSC data = %x", ucReceiveMSC[uctemp /2]);
                    }
                }
            }
            else // First packet is not command
            {
                if(ucReceiveMSC[0] == 1) // Only receive one data, command length = 1
                {
                    // Add pseudo ACK command
                    ucReceiveMSC[1] = MSC_ACK;
                    // Change to ACK + data command format
                    ucReceiveMSC[2] = mhal_mhl_GetRxXdataValue(ucPortSelect, ucCommandAddress +2);
                    // Change MSC length to 2
                    ucReceiveMSC[0] = 2;
                }
            }

            ucCommandAddress += ucLength;
        }
    }

    return ucCommandAddress;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_GetCbusSelfTestMSC()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE _mdrv_mhl_GetCbusSelfTestMSC(BYTE ucPortSelect, BYTE ucCommandAddress, BYTE *ucReceiveMSC)
{
    BYTE ucCommandMask = 1;

#if(MHL_CBUS_SELF_TEST_SUPPORT)
    ucCommandMask = _mdrv_mhl_GetCbusSelfTestCommand(ucCommandAddress &BITMASK(6:0), ucReceiveMSC);
#endif

    ucCommandAddress = (ucCommandAddress +1) %(ucCommandMask +1);

    if((ucCommandAddress == 0) || (ucReceiveMSC[0] == 0))
    {
        switch(ucPortSelect)
        {
#if(MHL_FUNCTION_SUPPORT_PORT0)
            case MHL_INPUT_SELECT_PORT0:
                CLR_MHL_CBUS_SELF_TEST_MODE0();
                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
            case MHL_INPUT_SELECT_PORT1:
                CLR_MHL_CBUS_SELF_TEST_MODE1();
                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
            case MHL_INPUT_SELECT_PORT2:
                CLR_MHL_CBUS_SELF_TEST_MODE2();
                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
            case MHL_INPUT_SELECT_PORT3:
                CLR_MHL_CBUS_SELF_TEST_MODE3();
                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
            case MHL_INPUT_SELECT_PORT4:
                CLR_MHL_CBUS_SELF_TEST_MODE4();
                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
            case MHL_INPUT_SELECT_PORT5:
                CLR_MHL_CBUS_SELF_TEST_MODE5();
                break;
#endif

            default:

                break;
        };
    }

    return ucCommandAddress;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_InsertCbusAskData()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_InsertCbusAskData(BYTE ucPortSelect, BYTE *ucReceiveMSC)
{
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            if(GET_MHL_SEND_ASK_DATA_PORT0())
            {
                CLR_MHL_SEND_ASK_DATA_PORT0();
                SET_MHL_SEND_ASK_FINISH_PORT0();

                SET_MHL_CBUS_ASK_DATA_PORT0(ucReceiveMSC[2]);
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            if(GET_MHL_SEND_ASK_DATA_PORT1())
            {
                CLR_MHL_SEND_ASK_DATA_PORT1();
                SET_MHL_SEND_ASK_FINISH_PORT1();

                SET_MHL_CBUS_ASK_DATA_PORT1(ucReceiveMSC[2]);
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            if(GET_MHL_SEND_ASK_DATA_PORT2())
            {
                CLR_MHL_SEND_ASK_DATA_PORT2();
                SET_MHL_SEND_ASK_FINISH_PORT2();

                SET_MHL_CBUS_ASK_DATA_PORT2(ucReceiveMSC[2]);
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            if(GET_MHL_SEND_ASK_DATA_PORT3())
            {
                CLR_MHL_SEND_ASK_DATA_PORT3();
                SET_MHL_SEND_ASK_FINISH_PORT3();

                SET_MHL_CBUS_ASK_DATA_PORT3(ucReceiveMSC[2]);
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            if(GET_MHL_SEND_ASK_DATA_PORT4())
            {
                CLR_MHL_SEND_ASK_DATA_PORT4();
                SET_MHL_SEND_ASK_FINISH_PORT4();

                SET_MHL_CBUS_ASK_DATA_PORT4(ucReceiveMSC[2]);
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            if(GET_MHL_SEND_ASK_DATA_PORT5())
            {
                CLR_MHL_SEND_ASK_DATA_PORT5();
                SET_MHL_SEND_ASK_FINISH_PORT5();

                SET_MHL_CBUS_ASK_DATA_PORT5(ucReceiveMSC[2]);
            }

            break;
#endif

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_InsertMainLinkRate()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_InsertMainLinkRate(BYTE ucPortSelect, BYTE ucLinkRate)
{
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            SET_MHL_LINK_RATE_PORT0(ucLinkRate);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            SET_MHL_LINK_RATE_PORT1(ucLinkRate);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            SET_MHL_LINK_RATE_PORT2(ucLinkRate);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            SET_MHL_LINK_RATE_PORT3(ucLinkRate);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            SET_MHL_LINK_RATE_PORT4(ucLinkRate);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            SET_MHL_LINK_RATE_PORT5(ucLinkRate);
            break;
#endif

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_GetMainLinkRate()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE _mdrv_mhl_GetMainLinkRate(BYTE ucPortSelect)
{
    BYTE ucLinkRate = MHL_AV_LINK_NONE;
    
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            ucLinkRate = GET_MHL_LINK_RATE_PORT0();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            ucLinkRate = GET_MHL_LINK_RATE_PORT1();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            ucLinkRate = GET_MHL_LINK_RATE_PORT2();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            ucLinkRate = GET_MHL_LINK_RATE_PORT3();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            ucLinkRate = GET_MHL_LINK_RATE_PORT4();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            ucLinkRate = GET_MHL_LINK_RATE_PORT5();
            break;
#endif

        default:

            break;
    };

    return ucLinkRate;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_InsertCbusMSGInformation()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_InsertCbusMSGInformation(BYTE ucPortSelect, BYTE *ucReceiveMSC)
{
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            SET_MHL_RX_MSG_COMMAND_PORT0(ucReceiveMSC[2]);
            SET_MHL_RX_MSG_DATA_PORT0(ucReceiveMSC[3]);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            SET_MHL_RX_MSG_COMMAND_PORT1(ucReceiveMSC[2]);
            SET_MHL_RX_MSG_DATA_PORT1(ucReceiveMSC[3]);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            SET_MHL_RX_MSG_COMMAND_PORT2(ucReceiveMSC[2]);
            SET_MHL_RX_MSG_DATA_PORT2(ucReceiveMSC[3]);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            SET_MHL_RX_MSG_COMMAND_PORT3(ucReceiveMSC[2]);
            SET_MHL_RX_MSG_DATA_PORT3(ucReceiveMSC[3]);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            SET_MHL_RX_MSG_COMMAND_PORT4(ucReceiveMSC[2]);
            SET_MHL_RX_MSG_DATA_PORT4(ucReceiveMSC[3]);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            SET_MHL_RX_MSG_COMMAND_PORT5(ucReceiveMSC[2]);
            SET_MHL_RX_MSG_DATA_PORT5(ucReceiveMSC[3]);
            break;
#endif

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_InsertWriteBurstInformation()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool _mdrv_mhl_InsertWriteBurstInformation(BYTE ucPortSelect, BYTE *ucReceiveMSC)
{
    Bool bParsingWriteBurst = FALSE;
    BYTE uctemp = 0;

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            SET_MHL_WRITE_BURST_ADDRESS0(ucReceiveMSC[2]);
            SET_MHL_WRITE_BURST_LENGTH0(MHL_MSC_SCRATCHPAD_SIZE +1);

            for(uctemp = 0; uctemp < (MHL_MSC_SCRATCHPAD_SIZE +1); uctemp++)
            {
                if(uctemp < GET_MHL_WRITE_BURST_LENGTH0())
                {
                    if(ucReceiveMSC[uctemp +3] == MSC_EOF)
                    {
                        SET_MHL_WRITE_BURST_DATA0(uctemp, 0);

                        SET_MHL_WRITE_BURST_LENGTH0(uctemp);
                    }
                    else
                    {
                        SET_MHL_WRITE_BURST_DATA0(uctemp, ucReceiveMSC[uctemp +3]);
                    }
                }
                else if(uctemp < MHL_MSC_SCRATCHPAD_SIZE)
                {
                    SET_MHL_WRITE_BURST_DATA0(uctemp, 0);
                }
            }

            bParsingWriteBurst = TRUE;

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            SET_MHL_WRITE_BURST_ADDRESS1(ucReceiveMSC[2]);
            SET_MHL_WRITE_BURST_LENGTH1(MHL_MSC_SCRATCHPAD_SIZE +1);

            for(uctemp = 0; uctemp < (MHL_MSC_SCRATCHPAD_SIZE +1); uctemp++)
            {
                if(uctemp < GET_MHL_WRITE_BURST_LENGTH1())
                {
                    if(ucReceiveMSC[uctemp +3] == MSC_EOF)
                    {
                        SET_MHL_WRITE_BURST_DATA1(uctemp, 0);

                        SET_MHL_WRITE_BURST_LENGTH1(uctemp);
                    }
                    else
                    {
                        SET_MHL_WRITE_BURST_DATA1(uctemp, ucReceiveMSC[uctemp +3]);
                    }
                }
                else if(uctemp < MHL_MSC_SCRATCHPAD_SIZE)
                {
                    SET_MHL_WRITE_BURST_DATA1(uctemp, 0);
                }
            }

            bParsingWriteBurst = TRUE;
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            SET_MHL_WRITE_BURST_ADDRESS2(ucReceiveMSC[2]);
            SET_MHL_WRITE_BURST_LENGTH2(MHL_MSC_SCRATCHPAD_SIZE +1);

            for(uctemp = 0; uctemp < (MHL_MSC_SCRATCHPAD_SIZE +1); uctemp++)
            {
                if(uctemp < GET_MHL_WRITE_BURST_LENGTH2())
                {
                    if(ucReceiveMSC[uctemp +3] == MSC_EOF)
                    {
                        SET_MHL_WRITE_BURST_DATA2(uctemp, 0);

                        SET_MHL_WRITE_BURST_LENGTH2(uctemp);
                    }
                    else
                    {
                        SET_MHL_WRITE_BURST_DATA2(uctemp, ucReceiveMSC[uctemp +3]);
                    }
                }
                else if(uctemp < MHL_MSC_SCRATCHPAD_SIZE)
                {
                    SET_MHL_WRITE_BURST_DATA2(uctemp, 0);
                }
            }

            bParsingWriteBurst = TRUE;

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            SET_MHL_WRITE_BURST_ADDRESS3(ucReceiveMSC[2]);
            SET_MHL_WRITE_BURST_LENGTH3(MHL_MSC_SCRATCHPAD_SIZE +1);

            for(uctemp = 0; uctemp < (MHL_MSC_SCRATCHPAD_SIZE +1); uctemp++)
            {
                if(uctemp < GET_MHL_WRITE_BURST_LENGTH3())
                {
                    if(ucReceiveMSC[uctemp +3] == MSC_EOF)
                    {
                        SET_MHL_WRITE_BURST_DATA3(uctemp, 0);

                        SET_MHL_WRITE_BURST_LENGTH3(uctemp);
                    }
                    else
                    {
                        SET_MHL_WRITE_BURST_DATA3(uctemp, ucReceiveMSC[uctemp +3]);
                    }
                }
                else if(uctemp < MHL_MSC_SCRATCHPAD_SIZE)
                {
                    SET_MHL_WRITE_BURST_DATA3(uctemp, 0);
                }
            }

            bParsingWriteBurst = TRUE;

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            SET_MHL_WRITE_BURST_ADDRESS4(ucReceiveMSC[2]);
            SET_MHL_WRITE_BURST_LENGTH4(MHL_MSC_SCRATCHPAD_SIZE +1);

            for(uctemp = 0; uctemp < (MHL_MSC_SCRATCHPAD_SIZE +1); uctemp++)
            {
                if(uctemp < GET_MHL_WRITE_BURST_LENGTH4())
                {
                    if(ucReceiveMSC[uctemp +3] == MSC_EOF)
                    {
                        SET_MHL_WRITE_BURST_DATA4(uctemp, 0);

                        SET_MHL_WRITE_BURST_LENGTH4(uctemp);
                    }
                    else
                    {
                        SET_MHL_WRITE_BURST_DATA4(uctemp, ucReceiveMSC[uctemp +3]);
                    }
                }
                else if(uctemp < MHL_MSC_SCRATCHPAD_SIZE)
                {
                    SET_MHL_WRITE_BURST_DATA4(uctemp, 0);
                }
            }

            bParsingWriteBurst = TRUE;

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            SET_MHL_WRITE_BURST_ADDRESS5(ucReceiveMSC[2]);
            SET_MHL_WRITE_BURST_LENGTH5(MHL_MSC_SCRATCHPAD_SIZE +1);

            for(uctemp = 0; uctemp < (MHL_MSC_SCRATCHPAD_SIZE +1); uctemp++)
            {
                if(uctemp < GET_MHL_WRITE_BURST_LENGTH5())
                {
                    if(ucReceiveMSC[uctemp +3] == MSC_EOF)
                    {
                        SET_MHL_WRITE_BURST_DATA5(uctemp, 0);

                        SET_MHL_WRITE_BURST_LENGTH5(uctemp);
                    }
                    else
                    {
                        SET_MHL_WRITE_BURST_DATA5(uctemp, ucReceiveMSC[uctemp +3]);
                    }
                }
                else if(uctemp < MHL_MSC_SCRATCHPAD_SIZE)
                {
                    SET_MHL_WRITE_BURST_DATA5(uctemp, 0);
                }
            }

            bParsingWriteBurst = TRUE;

            break;
#endif

        default:

            break;
    };

    return bParsingWriteBurst;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_ParsingWriteBurstVCAssign()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_ParsingWriteBurstVCAssign(BYTE ucPortSelect, BYTE *ucWriteBurstData)
{
    Bool bParsingFinish = FALSE;
    BYTE uctemp = 0;
    BYTE ucSequence = ucWriteBurstData[1];
    BYTE ucNumberEntry = ucWriteBurstData[2];

    if(ucNumberEntry +(ucSequence -1) *MHL_VC_SEQUENCE_LENGTH == ucWriteBurstData[0]) // Number entry + (sequece -1) x length = total entry
    {
        bParsingFinish = TRUE;
    }

    for(uctemp = 0; uctemp <ucNumberEntry; uctemp++)
    {
        _mdrv_mhl_InsertVCInformation(ucPortSelect, TRUE, ucWriteBurstData[uctemp *MHL_VC_SEQUENCE_LENGTH +3], ucWriteBurstData[uctemp *MHL_VC_SEQUENCE_LENGTH +4], ucWriteBurstData[uctemp *MHL_VC_SEQUENCE_LENGTH +5]);
    }

    if(bParsingFinish)
    {
        switch(ucPortSelect)
        {
#if(MHL_FUNCTION_SUPPORT_PORT0)
            case MHL_INPUT_SELECT_PORT0:
                SET_MHL_RECEIVE_VC_ASSIGN_PORT0();
                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
            case MHL_INPUT_SELECT_PORT1:
                SET_MHL_RECEIVE_VC_ASSIGN_PORT1();
                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
            case MHL_INPUT_SELECT_PORT2:
                SET_MHL_RECEIVE_VC_ASSIGN_PORT2();
                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
            case MHL_INPUT_SELECT_PORT3:
                SET_MHL_RECEIVE_VC_ASSIGN_PORT3();
                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
            case MHL_INPUT_SELECT_PORT4:
                SET_MHL_RECEIVE_VC_ASSIGN_PORT4();
                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
            case MHL_INPUT_SELECT_PORT5:
                SET_MHL_RECEIVE_VC_ASSIGN_PORT5();
                break;
#endif

            default:

                break;
        };
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_ParsingWriteBurstVCConfirm()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_ParsingWriteBurstVCConfirm(BYTE ucPortSelect, BYTE *ucWriteBurstData)
{
    Bool bParsingFinish = FALSE;
    BYTE uctemp = 0;
    BYTE ucSequence = ucWriteBurstData[1];
    BYTE ucNumberEntry = ucWriteBurstData[2];

    if(ucNumberEntry +(ucSequence -1) *MHL_VC_SEQUENCE_LENGTH == ucWriteBurstData[0]) // Number entry + (sequece -1) x length = total entry
    {
        bParsingFinish = TRUE;
    }

    for(uctemp = 0; uctemp <ucNumberEntry; uctemp++)
    {
        _mdrv_mhl_CheckVCInformation(ucPortSelect, ucWriteBurstData[uctemp *MHL_VC_SEQUENCE_LENGTH +3], ucWriteBurstData[uctemp *MHL_VC_SEQUENCE_LENGTH +4], ucWriteBurstData[uctemp *MHL_VC_SEQUENCE_LENGTH +5]);
    }

    if(bParsingFinish)
    {
        _mdrv_mhl_SortingVCInformation(ucPortSelect, TRUE);

        _mdrv_mhl_eCbusTimeSlotAllocation(ucPortSelect);
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_ParsingWriteBurstBISTSetup()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_ParsingWriteBurstBISTSetup(BYTE ucPortSelect, BYTE *ucWriteBurstData)
{
    BYTE ucReadyIndex = 0x70;
    BYTE ucBISTeCbusPattern = tMHL_DEVCAP_TABLE_PORT[ucPortSelect][MHL_CBUS_ECBUS_SPEEDS];
    BYTE ucBISTAVLinkRate = tMHL_DEVCAP_TABLE_PORT[ucPortSelect][MHL_CBUS_TMDS_SPEEDS];
    WORD usBISTeCbusPatternFixed = 0;
    WORD usBISTAVLinkFixed = 0;
    WORD usBISTeCbusDuration = 0;
    WORD usBISTAVLinkDuration = 0;

    if((ucBISTeCbusPattern &BIT4) == BIT4) // MHL_ECBUS_D_150
    {
        ucBISTeCbusPattern = MHL_BIST_ECBUS_PATTERN_FIXED10;
    }
    else if((ucBISTeCbusPattern &BIT0) == BIT0) // MHL_ECBUS_S_075
    {
        ucBISTeCbusPattern = MHL_BIST_ECBUS_PATTERN_FIXED8;
    }
    else
    {
        ucBISTeCbusPattern = MHL_BIST_ECBUS_PATTERN_PRBS;
    }

    if((ucBISTAVLinkRate & BIT2) == BIT2) // MHL_TMDS_600
    {
        ucBISTAVLinkRate = MHL_BIST_AV_LINK_600;
    }
    else if((ucBISTAVLinkRate & BIT1) == BIT1) // MHL_TMDS_300
    {
        ucBISTAVLinkRate = MHL_BIST_AV_LINK_300;
    }
    else if((ucBISTAVLinkRate & BIT0) == BIT0) // MHL_TMDS_150
    {
        ucBISTAVLinkRate = MHL_BIST_AV_LINK_150;
    }
    else
    {
        ucBISTAVLinkRate = MHL_BIST_AV_LINK_RESERVED;
    }

    if(ucWriteBurstData[1] <= ucBISTeCbusPattern) // Check eCbus pattern
    {
       ucReadyIndex = MASKSET(ucReadyIndex, BIT0, (BIT0 |BIT4));

       ucBISTeCbusPattern = ucWriteBurstData[1];
       usBISTeCbusPatternFixed = ((WORD)ucWriteBurstData[2] <<8) |ucWriteBurstData[3];
    }

    if(ucWriteBurstData[5] <= ucBISTAVLinkRate) // Check AV link rate
    {
        ucReadyIndex = MASKSET(ucReadyIndex, BIT1, (BIT1 |BIT5));

        ucBISTAVLinkRate = ucWriteBurstData[5];
        usBISTAVLinkFixed = ((WORD)ucWriteBurstData[9] <<8) |ucWriteBurstData[10];
    }

    if(ucWriteBurstData[0] == 0) // Check eCbus duration
    {
        // Can't allow
    }
    else
    {
        usBISTeCbusDuration = (WORD)ucWriteBurstData[0] *1010ul;
    }

    if(ucWriteBurstData[6] <= MHL_BIST_LINK_PATTERN_FIXED8)
    {
        usBISTAVLinkDuration = (WORD)ucWriteBurstData[7] *32 *(1000 /60);
    }
    else if(ucWriteBurstData[6] == MHL_BIST_LINK_PATTERN_FIXED10)
    {
        usBISTAVLinkDuration = (WORD)ucWriteBurstData[7] *1010ul;
    }

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            SET_MHL_BIST_PROCESS_STATE_PORT0(MHL_CBUS_BIST_RECEIVE_SETUP);
            SET_MHL_BIST_READY_PORT0(ucReadyIndex);
            SET_MHL_BIST_ECBUS_PATTERN0(ucBISTeCbusPattern);
            SET_MHL_BIST_ECBUS_FIXED0(usBISTeCbusPatternFixed);
            SET_MHL_BIST_ECBUS_DURATION0(usBISTeCbusDuration);
            SET_MHL_BIST_AV_LINK_RATE0(ucBISTAVLinkRate);
            SET_MHL_BIST_AV_LINK_MODE0(ucWriteBurstData[7]);
            SET_MHL_BIST_AV_LINK_PATTERN0(ucWriteBurstData[6]);
            SET_MHL_BIST_AV_LINK_FIXED0(usBISTAVLinkFixed);
            SET_MHL_BIST_AV_LINK_DURATION0(usBISTAVLinkDuration);
            SET_MHL_BIST_AV_LINK_RANDOM0(ucWriteBurstData[11]);
            SET_MHL_BIST_IMPEDANCE_MODE0(ucWriteBurstData[12]);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            SET_MHL_BIST_PROCESS_STATE_PORT1(MHL_CBUS_BIST_RECEIVE_SETUP);
            SET_MHL_BIST_READY_PORT1(ucReadyIndex);
            SET_MHL_BIST_ECBUS_PATTERN1(ucBISTeCbusPattern);
            SET_MHL_BIST_ECBUS_FIXED1(usBISTeCbusPatternFixed);
            SET_MHL_BIST_ECBUS_DURATION1(usBISTeCbusDuration);
            SET_MHL_BIST_AV_LINK_RATE1(ucBISTAVLinkRate);
            SET_MHL_BIST_AV_LINK_MODE1(ucWriteBurstData[7]);
            SET_MHL_BIST_AV_LINK_PATTERN1(ucWriteBurstData[6]);
            SET_MHL_BIST_AV_LINK_FIXED1(usBISTAVLinkFixed);
            SET_MHL_BIST_AV_LINK_DURATION1(usBISTAVLinkDuration);
            SET_MHL_BIST_AV_LINK_RANDOM1(ucWriteBurstData[11]);
            SET_MHL_BIST_IMPEDANCE_MODE1(ucWriteBurstData[12]);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            SET_MHL_BIST_PROCESS_STATE_PORT2(MHL_CBUS_BIST_RECEIVE_SETUP);
            SET_MHL_BIST_READY_PORT2(ucReadyIndex);
            SET_MHL_BIST_ECBUS_PATTERN2(ucBISTeCbusPattern);
            SET_MHL_BIST_ECBUS_FIXED2(usBISTeCbusPatternFixed);
            SET_MHL_BIST_ECBUS_DURATION2(usBISTeCbusDuration);
            SET_MHL_BIST_AV_LINK_RATE2(ucBISTAVLinkRate);
            SET_MHL_BIST_AV_LINK_MODE2(ucWriteBurstData[7]);
            SET_MHL_BIST_AV_LINK_PATTERN2(ucWriteBurstData[6]);
            SET_MHL_BIST_AV_LINK_FIXED2(usBISTAVLinkFixed);
            SET_MHL_BIST_AV_LINK_DURATION2(usBISTAVLinkDuration);
            SET_MHL_BIST_AV_LINK_RANDOM2(ucWriteBurstData[11]);
            SET_MHL_BIST_IMPEDANCE_MODE2(ucWriteBurstData[12]);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            SET_MHL_BIST_PROCESS_STATE_PORT3(MHL_CBUS_BIST_RECEIVE_SETUP);
            SET_MHL_BIST_READY_PORT3(ucReadyIndex);
            SET_MHL_BIST_ECBUS_PATTERN3(ucBISTeCbusPattern);
            SET_MHL_BIST_ECBUS_FIXED3(usBISTeCbusPatternFixed);
            SET_MHL_BIST_ECBUS_DURATION3(usBISTeCbusDuration);
            SET_MHL_BIST_AV_LINK_RATE3(ucBISTAVLinkRate);
            SET_MHL_BIST_AV_LINK_MODE3(ucWriteBurstData[7]);
            SET_MHL_BIST_AV_LINK_PATTERN3(ucWriteBurstData[6]);
            SET_MHL_BIST_AV_LINK_FIXED3(usBISTAVLinkFixed);
            SET_MHL_BIST_AV_LINK_DURATION3(usBISTAVLinkDuration);
            SET_MHL_BIST_AV_LINK_RANDOM3(ucWriteBurstData[11]);
            SET_MHL_BIST_IMPEDANCE_MODE3(ucWriteBurstData[12]);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            SET_MHL_BIST_PROCESS_STATE_PORT4(MHL_CBUS_BIST_RECEIVE_SETUP);
            SET_MHL_BIST_READY_PORT4(ucReadyIndex);
            SET_MHL_BIST_ECBUS_PATTERN4(ucBISTeCbusPattern);
            SET_MHL_BIST_ECBUS_FIXED4(usBISTeCbusPatternFixed);
            SET_MHL_BIST_ECBUS_DURATION4(usBISTeCbusDuration);
            SET_MHL_BIST_AV_LINK_RATE4(ucBISTAVLinkRate);
            SET_MHL_BIST_AV_LINK_MODE4(ucWriteBurstData[7]);
            SET_MHL_BIST_AV_LINK_PATTERN4(ucWriteBurstData[6]);
            SET_MHL_BIST_AV_LINK_FIXED4(usBISTAVLinkFixed);
            SET_MHL_BIST_AV_LINK_DURATION4(usBISTAVLinkDuration);
            SET_MHL_BIST_AV_LINK_RANDOM4(ucWriteBurstData[11]);
            SET_MHL_BIST_IMPEDANCE_MODE4(ucWriteBurstData[12]);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            SET_MHL_BIST_PROCESS_STATE_PORT5(MHL_CBUS_BIST_RECEIVE_SETUP);
            SET_MHL_BIST_READY_PORT5(ucReadyIndex);
            SET_MHL_BIST_ECBUS_PATTERN5(ucBISTeCbusPattern);
            SET_MHL_BIST_ECBUS_FIXED5(usBISTeCbusPatternFixed);
            SET_MHL_BIST_ECBUS_DURATION5(usBISTeCbusDuration);
            SET_MHL_BIST_AV_LINK_RATE5(ucBISTAVLinkRate);
            SET_MHL_BIST_AV_LINK_MODE5(ucWriteBurstData[7]);
            SET_MHL_BIST_AV_LINK_PATTERN5(ucWriteBurstData[6]);
            SET_MHL_BIST_AV_LINK_FIXED5(usBISTAVLinkFixed);
            SET_MHL_BIST_AV_LINK_DURATION5(usBISTAVLinkDuration);
            SET_MHL_BIST_AV_LINK_RANDOM5(ucWriteBurstData[11]);
            SET_MHL_BIST_IMPEDANCE_MODE5(ucWriteBurstData[12]);
            break;
#endif

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_ParsingWriteBurstInformation()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_ParsingWriteBurstInformation(BYTE ucPortSelect, BYTE *ucWriteBurstData)
{
    BYTE uctemp = 0;
    BYTE ucCheckSum = 0;
    WORD usWriteBurstID = ((WORD)ucWriteBurstData[0] <<8) |ucWriteBurstData[1];

    for(uctemp = 0; uctemp <MHL_MSC_SCRATCHPAD_SIZE; uctemp++)
    {
        ucCheckSum = ucCheckSum +ucWriteBurstData[uctemp];
    }

    if(ucCheckSum == 0)
    {
        switch(usWriteBurstID)
        {
            case MHL_BURST_ID_VC_ASSIGN:
                _mdrv_mhl_ParsingWriteBurstVCAssign(ucPortSelect, &ucWriteBurstData[3]);
                break;

            case MHL_BURST_ID_VC_CONFIRM:
                _mdrv_mhl_ParsingWriteBurstVCConfirm(ucPortSelect, &ucWriteBurstData[3]);
                break;

            case MHL_BURST_ID_BIST_SETUP:
                _mdrv_mhl_ParsingWriteBurstBISTSetup(ucPortSelect, &ucWriteBurstData[3]);
                break;

            default:

                break;
        };
    }
    else
    {
        ucWriteBurstData[2] = 0;

        MHL_DRV_DPRINTF("** MHL write burst check sum error port %d", ucPortSelect);
        MHL_DRV_DPRINTF("** MHL correct check sum is 0x%x", _mdrv_mhl_WriteBurstCheckSum(ucWriteBurstData));
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_SetReceiveCaseFlag()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
DWORD _mdrv_mhl_SetReceiveCaseFlag(BYTE ucPortSelect, DWORD ulReceiveIndex, BYTE *ucReceiveMSC)
{
    switch(ucReceiveMSC[1])
    {
        case MSC_ACK:
            ulReceiveIndex |= MHL_CBUS_RECEIVE_ACK_FLAG;

            if(ucReceiveMSC[0] == 2) // Command length = 2
            {
                _mdrv_mhl_InsertCbusAskData(ucPortSelect, ucReceiveMSC);
            }

            break;

        case MSC_ABORT:
            ulReceiveIndex |= MHL_CBUS_RECEIVE_ABORT_FLAG;
            break;

        case MSC_MSC_MSG:
            ulReceiveIndex |= MHL_CBUS_RECEIVE_MSG_FLAG;

            _mdrv_mhl_InsertCbusMSGInformation(ucPortSelect, ucReceiveMSC);

            break;

        case MSC_WRITE_STAT_OR_SET_INT:
            if(ucReceiveMSC[2] == 0x31) // Switch to PP mode or 24 bit mode
            {
                ucReceiveMSC[3] &= MASKBIT(2:0);

                if(ucReceiveMSC[3] == MHL_PHY_CLOCK_PACKET_PIXEL)
                {
                    ulReceiveIndex = MASKSET(ulReceiveIndex, MHL_CBUS_RECEIVE_PP_MODE_FLAG, MHL_CLOCK_TYPE_MODE_MASK);
                }
                else if(ucReceiveMSC[3] == MHL_PHY_CLOCK_NORMAL_24BIT)
                {
                    ulReceiveIndex = MASKSET(ulReceiveIndex, MHL_CBUS_RECEIVE_24BIT_MODE_FLAG, MHL_CLOCK_TYPE_MODE_MASK);
                }
            }

            if(ucReceiveMSC[2] == 0x30)
            {
                if((ucReceiveMSC[3] & BIT0) == BIT0) // Devcap ready
                {
                    ulReceiveIndex |= MHL_CBUS_RECEIVE_DEVCAP_FLAG;
                }

                if((ucReceiveMSC[3] & BIT1) == BIT1) // Extended device capability support
                {
                    ulReceiveIndex |= MHL_CBUS_RECEIVE_XDEVCAP_SUPPORT_FLAG;
                }
            }

            if((ucReceiveMSC[2] == 0x32) && (ucReceiveMSC[3] >= 0x30)) // Source support MHL3
            {
                ulReceiveIndex |= MHL_CBUS_RECEIVE_MHL3_FLAG;
            }

            if(ucReceiveMSC[2] == 0x20)
            {
                if((ucReceiveMSC[3] & BIT0) == BIT0) // Devcap change
                {
                    ulReceiveIndex |= MHL_CBUS_RECEIVE_DEVCAP_FLAG;
                }

                if((ucReceiveMSC[3] & BIT1) == BIT1) // Scratchpad register change
                {
                    ulReceiveIndex |= MHL_CBUS_RECEIVE_DSCR_CHANGE_FLAG;
                }

                if((ucReceiveMSC[3] & BIT2) == BIT2) // Burst write request
                {
                    ulReceiveIndex |= MHL_CBUS_RECEIVE_WRITE_REQUEST_FLAG;
                }

                if((ucReceiveMSC[3] & BIT3) == BIT3) // Burst write grant
                {
                    ulReceiveIndex |= MHL_CBUS_RECEIVE_WRITE_GRANT_FLAG;
                }

                if((ucReceiveMSC[3] & BIT4) == BIT4) // 3D request
                {
                    ulReceiveIndex |= MHL_CBUS_RECEIVE_3D_REQ_FLAG;
                }

                if((ucReceiveMSC[3] & BIT5) == BIT5) // Feature request
                {
                    ulReceiveIndex |= MHL_CBUS_RECEIVE_FEATURE_REQUEST_FLAG;
                }

                if((ucReceiveMSC[3] & BIT6) == BIT6) // Feature complete
                {
                    ulReceiveIndex |= MHL_CBUS_RECEIVE_FEATURE_COMPLETE_FLAG;
                }
            }

            break;

        case MSC_WRITE_XSTAT:
            if(ucReceiveMSC[2] == 0x92) // Link rate
            {
                ulReceiveIndex |= MHL_CBUS_RECEIVE_LINK_RATE_FLAG;

                _mdrv_mhl_InsertMainLinkRate(ucPortSelect, ucReceiveMSC[3]);
            }
            
            break;

        case MSC_WRITE_BURST:
            ulReceiveIndex |= MHL_CBUS_RECEIVE_WRITE_BURST_FLAG;

            if(_mdrv_mhl_InsertWriteBurstInformation(ucPortSelect, ucReceiveMSC))
            {
                _mdrv_mhl_ParsingWriteBurstInformation(ucPortSelect, &ucReceiveMSC[3]);
            }

            break;

        default:

            break;
    }

    return ulReceiveIndex;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_ParsingReceiveCommand()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_ParsingReceiveCommand(BYTE ucPortSelect, Bool bSecondBlock)
{
    Bool bFinish = FALSE;
    BYTE ucParsingCount = 0;
    BYTE ucCommandAddress = 0;
    BYTE ucReceiveMSC[MHL_CBUS_DATA_SIZE +4];
    DWORD ulReceiveIndex = 0;

    if(bSecondBlock)
    {
        ucCommandAddress = MHL_RX_XRAM_SIZE_HALF;
    }

    do
    {
        if(_mdrv_mhl_GetCbusSelfTestFlag(ucPortSelect))
        {
            ucCommandAddress = _mdrv_mhl_GetCbusSelfTestMSC(ucPortSelect, ucCommandAddress, ucReceiveMSC);
        }
        else
        {
            ucCommandAddress = _mdrv_mhl_GetCbusReceiveMSC(ucPortSelect, ucCommandAddress, ucReceiveMSC);
        }

        if((ucCommandAddress > 0) && (ucReceiveMSC[0] > 0))
        {
            ulReceiveIndex = _mdrv_mhl_SetReceiveCaseFlag(ucPortSelect, ulReceiveIndex, ucReceiveMSC);

            ucParsingCount++;
        }
        else
        {
            bFinish = TRUE;

            if(ucParsingCount > 0)
            {
                ulReceiveIndex |= MHL_CBUS_RECEIVE_MSC_FLAG;
            }
            else // Must decrease receieve counter to avoid always into receive process
            {
                ucParsingCount = 1;
            }
        }
    }
    while(!bFinish);

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            SET_MHL_RECEIVE_FLAG_PORT0(GET_MHL_RECEIVE_FLAG_PORT0() | ulReceiveIndex);

            if(GET_MHL_MSC_RECEIVE_COUNTER0() > ucParsingCount)
            {
                SET_MHL_MSC_RECEIVE_COUNTER0(GET_MHL_MSC_RECEIVE_COUNTER0() -ucParsingCount);
            }
            else
            {
                SET_MHL_MSC_RECEIVE_COUNTER0(0);
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            SET_MHL_RECEIVE_FLAG_PORT1(GET_MHL_RECEIVE_FLAG_PORT1() | ulReceiveIndex);

            if(GET_MHL_MSC_RECEIVE_COUNTER1() > ucParsingCount)
            {
                SET_MHL_MSC_RECEIVE_COUNTER1(GET_MHL_MSC_RECEIVE_COUNTER1() -ucParsingCount);
            }
            else
            {
                SET_MHL_MSC_RECEIVE_COUNTER1(0);
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            SET_MHL_RECEIVE_FLAG_PORT2(GET_MHL_RECEIVE_FLAG_PORT2() | ulReceiveIndex);

            if(GET_MHL_MSC_RECEIVE_COUNTER2() > ucParsingCount)
            {
                SET_MHL_MSC_RECEIVE_COUNTER2(GET_MHL_MSC_RECEIVE_COUNTER2() -ucParsingCount);
            }
            else
            {
                SET_MHL_MSC_RECEIVE_COUNTER2(0);
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            SET_MHL_RECEIVE_FLAG_PORT3(GET_MHL_RECEIVE_FLAG_PORT3() | ulReceiveIndex);

            if(GET_MHL_MSC_RECEIVE_COUNTER3() > ucParsingCount)
            {
                SET_MHL_MSC_RECEIVE_COUNTER3(GET_MHL_MSC_RECEIVE_COUNTER3() -ucParsingCount);
            }
            else
            {
                SET_MHL_MSC_RECEIVE_COUNTER3(0);
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            SET_MHL_RECEIVE_FLAG_PORT4(GET_MHL_RECEIVE_FLAG_PORT4() | ulReceiveIndex);

            if(GET_MHL_MSC_RECEIVE_COUNTER4() > ucParsingCount)
            {
                SET_MHL_MSC_RECEIVE_COUNTER4(GET_MHL_MSC_RECEIVE_COUNTER4() -ucParsingCount);
            }
            else
            {
                SET_MHL_MSC_RECEIVE_COUNTER4(0);
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            SET_MHL_RECEIVE_FLAG_PORT5(GET_MHL_RECEIVE_FLAG_PORT5() | ulReceiveIndex);

            if(GET_MHL_MSC_RECEIVE_COUNTER5() > ucParsingCount)
            {
                SET_MHL_MSC_RECEIVE_COUNTER5(GET_MHL_MSC_RECEIVE_COUNTER5() -ucParsingCount);
            }
            else
            {
                SET_MHL_MSC_RECEIVE_COUNTER5(0);
            }

            break;
#endif

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_ParsingEMSCReceiveData()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_ParsingEMSCReceiveData(BYTE ucPortSelect)
{
    BYTE uctemp = 3;
    BYTE ucReceiveEMSC[MHL_EMSC_DATA_SIZE +1];
    WORD usEMSCRxBufferSize = 0;
    WORD usEMSCTxBufferSize = 0;
    WORD usWriteBurstID = 0;
    DWORD ulReceiveIndex = 0;

    mhal_mhl_GetEMSCReceiveData(ucPortSelect, ucReceiveEMSC);

    usEMSCTxBufferSize = ucReceiveEMSC[1]; // Rx unload ACK

    if(ucReceiveEMSC[2] > 0) // Check length remaining
    {
        while((ucReceiveEMSC[2] +3) > uctemp)
        {
            usWriteBurstID = (ucReceiveEMSC[uctemp] << 8) |ucReceiveEMSC[uctemp +1];

            switch(usWriteBurstID)
            {
                case MHL_BURST_ID_BLK_RCV_BUFFER_INFO:
                    usEMSCTxBufferSize = usEMSCTxBufferSize +((ucReceiveEMSC[uctemp +3] << 8) |ucReceiveEMSC[uctemp +2]) -MHL_EMSC_DATA_SIZE;

                    uctemp = uctemp +4;
                    
                    break;

                case MHL_BURST_ID_BITS_PER_PIXEL_FMT:
                    if(ucReceiveEMSC[uctemp +7] == MHL_VIEW_PIXEL_FORMATE_16BPP)
                    {
                        ulReceiveIndex = MASKSET(ulReceiveIndex, MHL_CBUS_RECEIVE_PP_MODE_FLAG, MHL_CLOCK_TYPE_MODE_MASK);
                    }
                    else if(ucReceiveEMSC[uctemp +7] == MHL_VIEW_PIXEL_FORMATE_24BPP)
                    {
                        ulReceiveIndex = MASKSET(ulReceiveIndex, MHL_CBUS_RECEIVE_24BIT_MODE_FLAG, MHL_CLOCK_TYPE_MODE_MASK);
                    }

                    uctemp = uctemp +8;
                    
                    break;

                default:

                    break;
            };
        }
    }
    
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            DEC_MHL_EMSC_RECEIVE_COUNTER0();
            SET_MHL_EMSC_RX_BUFFER_SIZE0(GET_MHL_EMSC_RX_BUFFER_SIZE0() - ucReceiveEMSC[0]);
            SET_MHL_EMSC_TX_BUFFER_SIZE0(usEMSCTxBufferSize +GET_MHL_EMSC_TX_BUFFER_SIZE0());
            SET_MHL_RECEIVE_FLAG_PORT0(GET_MHL_RECEIVE_FLAG_PORT0() | ulReceiveIndex);
            usEMSCRxBufferSize = GET_MHL_EMSC_RX_BUFFER_SIZE0();
            usEMSCTxBufferSize = GET_MHL_EMSC_TX_BUFFER_SIZE0();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            DEC_MHL_EMSC_RECEIVE_COUNTER1();
            SET_MHL_EMSC_RX_BUFFER_SIZE1(GET_MHL_EMSC_RX_BUFFER_SIZE1() - ucReceiveEMSC[0]);
            SET_MHL_EMSC_TX_BUFFER_SIZE1(usEMSCTxBufferSize +GET_MHL_EMSC_TX_BUFFER_SIZE1());
            SET_MHL_RECEIVE_FLAG_PORT1(GET_MHL_RECEIVE_FLAG_PORT1() | ulReceiveIndex);
            usEMSCRxBufferSize = GET_MHL_EMSC_RX_BUFFER_SIZE1();
            usEMSCTxBufferSize = GET_MHL_EMSC_TX_BUFFER_SIZE1();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            DEC_MHL_EMSC_RECEIVE_COUNTER2();
            SET_MHL_EMSC_RX_BUFFER_SIZE2(GET_MHL_EMSC_RX_BUFFER_SIZE2() - ucReceiveEMSC[0]);
            SET_MHL_EMSC_TX_BUFFER_SIZE2(usEMSCTxBufferSize +GET_MHL_EMSC_TX_BUFFER_SIZE2());
            SET_MHL_RECEIVE_FLAG_PORT2(GET_MHL_RECEIVE_FLAG_PORT2() | ulReceiveIndex);
            usEMSCRxBufferSize = GET_MHL_EMSC_RX_BUFFER_SIZE2();
            usEMSCTxBufferSize = GET_MHL_EMSC_TX_BUFFER_SIZE2();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            DEC_MHL_EMSC_RECEIVE_COUNTER3();
            SET_MHL_EMSC_RX_BUFFER_SIZE3(GET_MHL_EMSC_RX_BUFFER_SIZE3() - ucReceiveEMSC[0]);
            SET_MHL_EMSC_TX_BUFFER_SIZE3(usEMSCTxBufferSize +GET_MHL_EMSC_TX_BUFFER_SIZE3());
            SET_MHL_RECEIVE_FLAG_PORT3(GET_MHL_RECEIVE_FLAG_PORT3() | ulReceiveIndex);
            usEMSCRxBufferSize = GET_MHL_EMSC_RX_BUFFER_SIZE3();
            usEMSCTxBufferSize = GET_MHL_EMSC_TX_BUFFER_SIZE3();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            DEC_MHL_EMSC_RECEIVE_COUNTER4();
            SET_MHL_EMSC_RX_BUFFER_SIZE4(GET_MHL_EMSC_RX_BUFFER_SIZE4() - ucReceiveEMSC[0]);
            SET_MHL_EMSC_TX_BUFFER_SIZE4(usEMSCTxBufferSize +GET_MHL_EMSC_TX_BUFFER_SIZE4());
            SET_MHL_RECEIVE_FLAG_PORT4(GET_MHL_RECEIVE_FLAG_PORT4() | ulReceiveIndex);
            usEMSCRxBufferSize = GET_MHL_EMSC_RX_BUFFER_SIZE4();
            usEMSCTxBufferSize = GET_MHL_EMSC_TX_BUFFER_SIZE4();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            DEC_MHL_EMSC_RECEIVE_COUNTER5();
            SET_MHL_EMSC_RX_BUFFER_SIZE5(GET_MHL_EMSC_RX_BUFFER_SIZE5() - ucReceiveEMSC[0]);
            SET_MHL_EMSC_TX_BUFFER_SIZE5(usEMSCTxBufferSize +GET_MHL_EMSC_TX_BUFFER_SIZE5());
            SET_MHL_RECEIVE_FLAG_PORT5(GET_MHL_RECEIVE_FLAG_PORT5() | ulReceiveIndex);
            usEMSCRxBufferSize = GET_MHL_EMSC_RX_BUFFER_SIZE5();
            usEMSCTxBufferSize = GET_MHL_EMSC_TX_BUFFER_SIZE5();
            break;
#endif

        default:

            break;
    };

    MHL_DRV_EMSCBUFFER("** MHL eMSC receive Rx buffer size %d !!", usEMSCRxBufferSize);
    MHL_DRV_EMSCBUFFER("** MHL eMSC receive Tx buffer size %d !!", usEMSCTxBufferSize);
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CheckReceiveCommand()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool _mdrv_mhl_CheckReceiveCommand(BYTE ucPortSelect)
{
    Bool bReceiveCommand = FALSE;
    Bool bReceiveEMSCData = FALSE;
    Bool bFirstBlock = FALSE;
    Bool bCbusBusyFlag = mhal_mhl_GetCbusBusyFlag(ucPortSelect);
    DWORD ulReceiveIndex = 0;

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            if(!bCbusBusyFlag && (GET_MHL_MSC_RECEIVE_COUNTER0() > 0))
            {
                bReceiveCommand = TRUE;

                if(GET_MHL_RX_XDATA_CONTROL0()) // First block
                {
                    CLR_MHL_RX_XDATA_CONTROL0();
                }
                else // Second block
                {
                    SET_MHL_RX_XDATA_CONTROL0();
                }

                bFirstBlock = !GET_MHL_RX_XDATA_CONTROL0();

                MHL_DRV_RXDEBUG(" MHL =================================== %d", GET_MHL_MSC_RECEIVE_COUNTER0());
            }

            if(GET_MHL_EMSC_RECEIVE_COUNTER0() > 0)
            {
                bReceiveEMSCData = TRUE;
            }

            ulReceiveIndex = GET_MHL_RECEIVE_FLAG_PORT0();

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            if(!bCbusBusyFlag && (GET_MHL_MSC_RECEIVE_COUNTER1() > 0))
            {
                bReceiveCommand = TRUE;

                if(GET_MHL_RX_XDATA_CONTROL1()) // First block
                {
                    CLR_MHL_RX_XDATA_CONTROL1();
                }
                else // Second block
                {
                    SET_MHL_RX_XDATA_CONTROL1();
                }

                bFirstBlock = !GET_MHL_RX_XDATA_CONTROL1();

                MHL_DRV_RXDEBUG(" MHL =================================== %d", GET_MHL_MSC_RECEIVE_COUNTER1());
            }

            if(GET_MHL_EMSC_RECEIVE_COUNTER1() > 0)
            {
                bReceiveEMSCData = TRUE;
            }

            ulReceiveIndex = GET_MHL_RECEIVE_FLAG_PORT1();

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            if(!bCbusBusyFlag && (GET_MHL_MSC_RECEIVE_COUNTER2() > 0))
            {
                bReceiveCommand = TRUE;

                if(GET_MHL_RX_XDATA_CONTROL2()) // First block
                {
                    CLR_MHL_RX_XDATA_CONTROL2();
                }
                else // Second block
                {
                    SET_MHL_RX_XDATA_CONTROL2();
                }

                bFirstBlock = !GET_MHL_RX_XDATA_CONTROL2();

                MHL_DRV_RXDEBUG(" MHL =================================== %d", GET_MHL_MSC_RECEIVE_COUNTER2());
            }

            if(GET_MHL_EMSC_RECEIVE_COUNTER2() > 0)
            {
                bReceiveEMSCData = TRUE;
            }

            ulReceiveIndex = GET_MHL_RECEIVE_FLAG_PORT2();

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            if(!bCbusBusyFlag && (GET_MHL_MSC_RECEIVE_COUNTER3() > 0))
            {
                bReceiveCommand = TRUE;

                if(GET_MHL_RX_XDATA_CONTROL3()) // First block
                {
                    CLR_MHL_RX_XDATA_CONTROL3();
                }
                else // Second block
                {
                    SET_MHL_RX_XDATA_CONTROL3();
                }

                bFirstBlock = !GET_MHL_RX_XDATA_CONTROL3();

                MHL_DRV_RXDEBUG(" MHL =================================== %d", GET_MHL_MSC_RECEIVE_COUNTER3());
            }

            if(GET_MHL_EMSC_RECEIVE_COUNTER3() > 0)
            {
                bReceiveEMSCData = TRUE;
            }

            ulReceiveIndex = GET_MHL_RECEIVE_FLAG_PORT3();

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            if(!bCbusBusyFlag && (GET_MHL_MSC_RECEIVE_COUNTER4() > 0))
            {
                bReceiveCommand = TRUE;

                if(GET_MHL_RX_XDATA_CONTROL4()) // First block
                {
                    CLR_MHL_RX_XDATA_CONTROL4();
                }
                else // Second block
                {
                    SET_MHL_RX_XDATA_CONTROL4();
                }

                bFirstBlock = !GET_MHL_RX_XDATA_CONTROL4();

                MHL_DRV_RXDEBUG(" MHL =================================== %d", GET_MHL_MSC_RECEIVE_COUNTER4());
            }

            if(GET_MHL_EMSC_RECEIVE_COUNTER4() > 0)
            {
                bReceiveEMSCData = TRUE;
            }

            ulReceiveIndex = GET_MHL_RECEIVE_FLAG_PORT4();

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            if(!bCbusBusyFlag && (GET_MHL_MSC_RECEIVE_COUNTER5() > 0))
            {
                bReceiveCommand = TRUE;

                if(GET_MHL_RX_XDATA_CONTROL5()) // First block
                {
                    CLR_MHL_RX_XDATA_CONTROL5();
                }
                else // Second block
                {
                    SET_MHL_RX_XDATA_CONTROL5();
                }

                bFirstBlock = !GET_MHL_RX_XDATA_CONTROL5();

                MHL_DRV_RXDEBUG(" MHL =================================== %d", GET_MHL_MSC_RECEIVE_COUNTER5());
            }

            if(GET_MHL_EMSC_RECEIVE_COUNTER5() > 0)
            {
                bReceiveEMSCData = TRUE;
            }

            ulReceiveIndex = GET_MHL_RECEIVE_FLAG_PORT5();

            break;
#endif

        default:

            break;
    };

    if(bReceiveCommand)
    {
        // Switch to first block, else switch to second block
        mhal_mhl_SetRxXdataAddress(ucPortSelect, bFirstBlock);
        // Parsing second block, else parsing first block
        _mdrv_mhl_ParsingReceiveCommand(ucPortSelect, bFirstBlock);
    }
    else if(ulReceiveIndex > 0)
    {
        bReceiveCommand = TRUE;
    }

    if(bReceiveEMSCData)
    {
        _mdrv_mhl_ParsingEMSCReceiveData(ucPortSelect);

        bReceiveCommand = TRUE;
    }

    return bReceiveCommand;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusDisplayLinkCheck()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_CbusDisplayLinkCheck(BYTE ucPortSelect)
{
    Bool bLinkCheck = FALSE;
    BYTE ucDisplayMask = _mdrv_mhl_GetCbusDisplayMask(ucPortSelect);
    BYTE ucDisplayIndex = 0;
    WORD usDisplayCheck = 0;

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            ucDisplayIndex = GET_MHL_CBUS_DISPLAY_FLAG0();
            usDisplayCheck = GET_MHL_CBUS_DISPLAY_CHECK0();

            if((ucDisplayIndex ^ usDisplayCheck) > 0)
            {
                usDisplayCheck = usDisplayCheck ^ ucDisplayMask;

                if(usDisplayCheck > 0)
                {
                    usDisplayCheck = GET_MHL_CBUS_DISPLAY_CHECK0() | _mdrv_mhl_CbusGetLeastFlag(usDisplayCheck);

                    SET_MHL_CBUS_DISPLAY_FLAG0(ucDisplayIndex |(usDisplayCheck << MHL_CBUS_DISPLAY_MASK_SHIFT));

                    bLinkCheck = TRUE;
                }
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            ucDisplayIndex = GET_MHL_CBUS_DISPLAY_FLAG1();
            usDisplayCheck = GET_MHL_CBUS_DISPLAY_CHECK1();

            if((ucDisplayIndex ^ usDisplayCheck) > 0)
            {
                usDisplayCheck = usDisplayCheck ^ ucDisplayMask;

                if(usDisplayCheck > 0)
                {
                    usDisplayCheck = GET_MHL_CBUS_DISPLAY_CHECK1() | _mdrv_mhl_CbusGetLeastFlag(usDisplayCheck);

                    SET_MHL_CBUS_DISPLAY_FLAG1(ucDisplayIndex |(usDisplayCheck << MHL_CBUS_DISPLAY_MASK_SHIFT));

                    bLinkCheck = TRUE;
                }
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            ucDisplayIndex = GET_MHL_CBUS_DISPLAY_FLAG2();
            usDisplayCheck = GET_MHL_CBUS_DISPLAY_CHECK2();

            if((ucDisplayIndex ^ usDisplayCheck) > 0)
            {
                usDisplayCheck = usDisplayCheck ^ ucDisplayMask;

                if(usDisplayCheck > 0)
                {
                    usDisplayCheck = GET_MHL_CBUS_DISPLAY_CHECK2() | _mdrv_mhl_CbusGetLeastFlag(usDisplayCheck);

                    SET_MHL_CBUS_DISPLAY_FLAG2(ucDisplayIndex |(usDisplayCheck << MHL_CBUS_DISPLAY_MASK_SHIFT));

                    bLinkCheck = TRUE;
                }
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            ucDisplayIndex = GET_MHL_CBUS_DISPLAY_FLAG3();
            usDisplayCheck = GET_MHL_CBUS_DISPLAY_CHECK3();

            if((ucDisplayIndex ^ usDisplayCheck) > 0)
            {
                usDisplayCheck = usDisplayCheck ^ ucDisplayMask;

                if(usDisplayCheck > 0)
                {
                    usDisplayCheck = GET_MHL_CBUS_DISPLAY_CHECK3() | _mdrv_mhl_CbusGetLeastFlag(usDisplayCheck);

                    SET_MHL_CBUS_DISPLAY_FLAG3(ucDisplayIndex |(usDisplayCheck << MHL_CBUS_DISPLAY_MASK_SHIFT));

                    bLinkCheck = TRUE;
                }
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            ucDisplayIndex = GET_MHL_CBUS_DISPLAY_FLAG4();
            usDisplayCheck = GET_MHL_CBUS_DISPLAY_CHECK4();

            if((ucDisplayIndex ^ usDisplayCheck) > 0)
            {
                usDisplayCheck = usDisplayCheck ^ ucDisplayMask;

                if(usDisplayCheck > 0)
                {
                    usDisplayCheck = GET_MHL_CBUS_DISPLAY_CHECK4() | _mdrv_mhl_CbusGetLeastFlag(usDisplayCheck);

                    SET_MHL_CBUS_DISPLAY_FLAG4(ucDisplayIndex |(usDisplayCheck << MHL_CBUS_DISPLAY_MASK_SHIFT));

                    bLinkCheck = TRUE;
                }
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            ucDisplayIndex = GET_MHL_CBUS_DISPLAY_FLAG5();
            usDisplayCheck = GET_MHL_CBUS_DISPLAY_CHECK5();

            if((ucDisplayIndex ^ usDisplayCheck) > 0)
            {
                usDisplayCheck = usDisplayCheck ^ ucDisplayMask;

                if(usDisplayCheck > 0)
                {
                    usDisplayCheck = GET_MHL_CBUS_DISPLAY_CHECK5() | _mdrv_mhl_CbusGetLeastFlag(usDisplayCheck);

                    SET_MHL_CBUS_DISPLAY_FLAG5(ucDisplayIndex |(usDisplayCheck << MHL_CBUS_DISPLAY_MASK_SHIFT));

                    bLinkCheck = TRUE;
                }
            }

            break;
#endif

        default:

            break;
    };

    if(bLinkCheck)
    {
        MHL_DRV_DPRINTF("** MHL receive ACK for link port %d", ucPortSelect);
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_SetMainLinkRate()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_SetMainLinkRate(BYTE ucPortSelect)
{
    BYTE ucLinkRate = _mdrv_mhl_GetMainLinkRate(ucPortSelect);

    mhal_mhl_SetMainLinkRate(ucPortSelect, ucLinkRate);
    
    if(ucLinkRate < MHL_AV_LINK_NONE)
    {
        MHL_DRV_DPRINTF("** MHL set AV link rate mode %d", ucLinkRate);
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusReceiveEventProc()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_CbusReceiveEventProc(BYTE ucPortSelect)
{
    Bool bSetTime = FALSE;
    Bool bClockModeChange = FALSE;
    Bool bCheckDisplayLink = FALSE;
    Bool bSetLinkRate = FALSE;
    Bool bECbusSupport = _mdrv_mhl_GetECbusSupportFlag(ucPortSelect);
    BYTE ucTimeTick = MHL_CBUS_IDEL_FREE_TIME;
    BYTE ucClockMode = 0;
    BYTE ucQueueIndex = 0;

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            if(GET_MHL_RECEIVE_MSC_PORT0()) // Receive MSC
            {
                CLR_MHL_RECEIVE_MSC_PORT0();

                if(GET_MHL_RECEIVE_ABORT_PORT0()) // Receive Abort
                {
                    CLR_MHL_RECEIVE_ABORT_PORT0();

                    ucTimeTick = MHL_CBUS_ABORT_HOLD_TIME;
                }

                bSetTime = TRUE;
            }

            if(GET_MHL_RECEIVE_ACK_PORT0()) // Receive Ack
            {
                CLR_MHL_RECEIVE_ACK_PORT0();

                if(GET_MHL_QUEUE_STATE_PORT0(MHL_CBUS_RX_QUEUE_INDEX) != CBUS_REQ_IDLE)
                {
                    SET_MHL_QUEUE_STATE_PORT0(MHL_CBUS_RX_QUEUE_INDEX, CBUS_REQ_IDLE);
                }
                else if(GET_MHL_QUEUE_STATE_PORT0(GET_MHL_TX_QUEUE_INDEX_PORT0()) != CBUS_REQ_PENDING)
                {
                    SET_MHL_QUEUE_STATE_PORT0(GET_MHL_TX_QUEUE_INDEX_PORT0(), CBUS_REQ_IDLE);
                }

                ucQueueIndex = _mdrv_mhl_GetCbusQueueIndex(ucPortSelect, GET_MHL_TX_QUEUE_INDEX_PORT0());

                // Update current index to search index, but not Rx queue index
                if(ucQueueIndex < MHL_CBUS_RX_QUEUE_INDEX)
                {
                    SET_MHL_TX_QUEUE_INDEX_PORT0(ucQueueIndex);
                }

                bCheckDisplayLink = TRUE;
            }

            if(GET_MHL_RECEIVE_DEVCAP_PORT0()) // Receive devcap ready or change
            {
                if(!GET_MHL_READ_XDEVCAP_ENABLE0())
                {
                    CLR_MHL_RECEIVE_DEVCAP_PORT0();

#if(MHL_READ_DEVICE_CAPABILITY)
                    SET_MHL_READ_DEVCAP_ENABLE0();
#endif
                }
            }

            if(GET_MHL_RECEIVE_WRITE_GRANT0()) // Receive write burst grant
            {
                CLR_MHL_RECEIVE_WRITE_GRANT0();

                if(GET_MHL_WRITE_BURST_STATE_PORT0() == MHL_CBUS_WRITE_BURST_SEND_REQUEST)
                {
                    SET_MHL_WRITE_BURST_STATE_PORT0(MHL_CBUS_WRITE_BURST_REVEIVE_GRANT);
                }
            }

            if(GET_MHL_RECEIVE_3D_REQ_PORT0()) // Receive 3D request
            {
                CLR_MHL_RECEIVE_3D_REQ_PORT0();
                SET_MHL_SEND_3D_INFORMATION_PORT0();
                SET_MHL_SEND_3D_DTD_PORT0();
                SET_MHL_SEND_3D_VIC_PORT0();
            }

            if(GET_MHL_RECEIVE_24BIT_MODE_PORT0()) // Receive change to 24 bit mode
            {
                CLR_MHL_RECEIVE_24BIT_MODE_PORT0();

                bClockModeChange = TRUE;
                ucClockMode = MHL_PHY_CLOCK_NORMAL_24BIT;
            }

            if(GET_MHL_RECEIVE_PP_MODE_PORT0()) // Receive change to packet pixel mode
            {
                CLR_MHL_RECEIVE_PP_MODE_PORT0();

                bClockModeChange = TRUE;
                ucClockMode = MHL_PHY_CLOCK_PACKET_PIXEL;
            }

            if(GET_MHL_RECEIVE_WRITE_REQUEST0()) // Receive write burst request
            {
                CLR_MHL_RECEIVE_WRITE_REQUEST0();

                if(GET_MHL_WRITE_BURST_STATE_PORT0() <= MHL_CBUS_WRITE_BURST_START)
                {
                    SET_MHL_WRITE_BURST_STATE_PORT0(MHL_CBUS_WRITE_BURST_REVEIVE_REQUEST);
                }
            }

            if(GET_MHL_RECEIVE_DSCR_CHANGE0()) // Receive scratchpad register chagne
            {
                CLR_MHL_RECEIVE_DSCR_CHANGE0();
                SET_MHL_WRITE_BURST_STATE_PORT0(MHL_CBUS_WRITE_BURST_END);
            }

            if(GET_MHL_RECEIVE_MHL3_PORT0()) // Receive source support MHL3
            {
                CLR_MHL_RECEIVE_MHL3_PORT0();

                if(bECbusSupport)
                {
                    SET_MHL_ENABLE_CBUS_ENHANCE0();
                }
            }

            if(GET_MHL_RECEIVE_XDEVCAP_SUPPORT0() && GET_MHL_VERSION_CODE_FLAG0()) // Receive extended device capability support
            {
                if(!GET_MHL_READ_DEVCAP_ENABLE0())
                {
                    CLR_MHL_RECEIVE_XDEVCAP_SUPPORT0();

#if(MHL_READ_DEVICE_CAPABILITY)
                    SET_MHL_READ_XDEVCAP_ENABLE0();
#endif
                }
            }

            if(GET_MHL_RECEIVE_VC_ASSIGN_PORT0()) // Receive virtual channel assign
            {
                CLR_MHL_RECEIVE_VC_ASSIGN_PORT0();
                SET_MHL_SEND_VC_INFORMATION_PORT0();
                SET_MHL_SEND_VC_CONFIRM_PORT0();
            }

            if(GET_MHL_RECEIVE_FEATURE_REQUEST0()) // Receive feature request
            {
                CLR_MHL_RECEIVE_FEATURE_REQUEST0();

                if(GET_MHL_FEATURE_REQ_STATE_PORT0() == MHL_CBUS_FEATURE_NONE)
                {
                    SET_MHL_FEATURE_REQ_STATE_PORT0(MHL_CBUS_FEATURE_START);
                }
            }

            if(GET_MHL_RECEIVE_FEATURE_COMPLETE0()) // Receive feature complete
            {
                CLR_MHL_RECEIVE_FEATURE_COMPLETE0();

                if(GET_MHL_FEATURE_REQ_STATE_PORT0() == MHL_CBUS_FEATURE_SEND_FEATURE_REQUEST)
                {
                    SET_MHL_FEATURE_REQ_STATE_PORT0(MHL_CBUS_FEATURE_END);
                }
            }

            if(GET_MHL_RECEIVE_LINK_RATE_PORT0()) // Receive link rate
            {
                CLR_MHL_RECEIVE_LINK_RATE_PORT0();

                bSetLinkRate = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            if(GET_MHL_RECEIVE_MSC_PORT1()) // Receive MSC
            {
                CLR_MHL_RECEIVE_MSC_PORT1();

                if(GET_MHL_RECEIVE_ABORT_PORT1()) // Receive Abort
                {
                    CLR_MHL_RECEIVE_ABORT_PORT1();

                    ucTimeTick = MHL_CBUS_ABORT_HOLD_TIME;
                }

                bSetTime = TRUE;
            }

            if(GET_MHL_RECEIVE_ACK_PORT1()) // Receive Ack
            {
                CLR_MHL_RECEIVE_ACK_PORT1();

                if(GET_MHL_QUEUE_STATE_PORT1(MHL_CBUS_RX_QUEUE_INDEX) != CBUS_REQ_IDLE)
                {
                    SET_MHL_QUEUE_STATE_PORT1(MHL_CBUS_RX_QUEUE_INDEX, CBUS_REQ_IDLE);
                }
                else if(GET_MHL_QUEUE_STATE_PORT1(GET_MHL_TX_QUEUE_INDEX_PORT1()) != CBUS_REQ_PENDING)
                {
                    SET_MHL_QUEUE_STATE_PORT1(GET_MHL_TX_QUEUE_INDEX_PORT1(), CBUS_REQ_IDLE);
                }

                ucQueueIndex = _mdrv_mhl_GetCbusQueueIndex(ucPortSelect, GET_MHL_TX_QUEUE_INDEX_PORT1());

                // Update current index to search index, but not Rx queue index
                if(ucQueueIndex < MHL_CBUS_RX_QUEUE_INDEX)
                {
                    SET_MHL_TX_QUEUE_INDEX_PORT1(ucQueueIndex);
                }

                bCheckDisplayLink = TRUE;
            }

            if(GET_MHL_RECEIVE_DEVCAP_PORT1()) // Receive devcap ready or change
            {
                if(!GET_MHL_READ_XDEVCAP_ENABLE1())
                {
                    CLR_MHL_RECEIVE_DEVCAP_PORT1();

#if(MHL_READ_DEVICE_CAPABILITY)
                    SET_MHL_READ_DEVCAP_ENABLE1();
#endif
                }
            }

            if(GET_MHL_RECEIVE_WRITE_GRANT1()) // Receive write burst grant
            {
                CLR_MHL_RECEIVE_WRITE_GRANT1();

                if(GET_MHL_WRITE_BURST_STATE_PORT1() == MHL_CBUS_WRITE_BURST_SEND_REQUEST)
                {
                    SET_MHL_WRITE_BURST_STATE_PORT1(MHL_CBUS_WRITE_BURST_REVEIVE_GRANT);
                }
            }

            if(GET_MHL_RECEIVE_3D_REQ_PORT1()) // Receive 3D request
            {
                CLR_MHL_RECEIVE_3D_REQ_PORT1();
                SET_MHL_SEND_3D_INFORMATION_PORT1();
                SET_MHL_SEND_3D_DTD_PORT1();
                SET_MHL_SEND_3D_VIC_PORT1();
            }

            if(GET_MHL_RECEIVE_24BIT_MODE_PORT1()) // Receive change to 24 bit mode
            {
                CLR_MHL_RECEIVE_24BIT_MODE_PORT1();

                bClockModeChange = TRUE;
                ucClockMode = MHL_PHY_CLOCK_NORMAL_24BIT;
            }

            if(GET_MHL_RECEIVE_PP_MODE_PORT1()) // Receive change to packet pixel mode
            {
                CLR_MHL_RECEIVE_PP_MODE_PORT1();

                bClockModeChange = TRUE;
                ucClockMode = MHL_PHY_CLOCK_PACKET_PIXEL;
            }

            if(GET_MHL_RECEIVE_WRITE_REQUEST1()) // Receive write burst request
            {
                CLR_MHL_RECEIVE_WRITE_REQUEST1();

                if(GET_MHL_WRITE_BURST_STATE_PORT1() <= MHL_CBUS_WRITE_BURST_START)
                {
                    SET_MHL_WRITE_BURST_STATE_PORT1(MHL_CBUS_WRITE_BURST_REVEIVE_REQUEST);
                }
            }

            if(GET_MHL_RECEIVE_DSCR_CHANGE1()) // Receive scratchpad register chagne
            {
                CLR_MHL_RECEIVE_DSCR_CHANGE1();
                SET_MHL_WRITE_BURST_STATE_PORT1(MHL_CBUS_WRITE_BURST_END);
            }

            if(GET_MHL_RECEIVE_MHL3_PORT1()) // Receive source support MHL3
            {
                CLR_MHL_RECEIVE_MHL3_PORT1();

                if(bECbusSupport)
                {
                    SET_MHL_ENABLE_CBUS_ENHANCE1();
                }
            }

            if(GET_MHL_RECEIVE_XDEVCAP_SUPPORT1() && GET_MHL_VERSION_CODE_FLAG1()) // Receive extended device capability support
            {
                if(!GET_MHL_READ_DEVCAP_ENABLE1())
                {
                    CLR_MHL_RECEIVE_XDEVCAP_SUPPORT1();

#if(MHL_READ_DEVICE_CAPABILITY)
                    SET_MHL_READ_XDEVCAP_ENABLE1();
#endif
                }
            }

            if(GET_MHL_RECEIVE_VC_ASSIGN_PORT1()) // Receive virtual channel assign
            {
                CLR_MHL_RECEIVE_VC_ASSIGN_PORT1();
                SET_MHL_SEND_VC_INFORMATION_PORT1();
                SET_MHL_SEND_VC_CONFIRM_PORT1();
            }

            if(GET_MHL_RECEIVE_FEATURE_REQUEST1()) // Receive feature request
            {
                CLR_MHL_RECEIVE_FEATURE_REQUEST1();

                if(GET_MHL_FEATURE_REQ_STATE_PORT1() == MHL_CBUS_FEATURE_NONE)
                {
                    SET_MHL_FEATURE_REQ_STATE_PORT1(MHL_CBUS_FEATURE_START);
                }
            }

            if(GET_MHL_RECEIVE_FEATURE_COMPLETE1()) // Receive feature complete
            {
                CLR_MHL_RECEIVE_FEATURE_COMPLETE1();

                if(GET_MHL_FEATURE_REQ_STATE_PORT1() == MHL_CBUS_FEATURE_SEND_FEATURE_REQUEST)
                {
                    SET_MHL_FEATURE_REQ_STATE_PORT1(MHL_CBUS_FEATURE_END);
                }
            }

            if(GET_MHL_RECEIVE_LINK_RATE_PORT1()) // Receive link rate
            {
                CLR_MHL_RECEIVE_LINK_RATE_PORT1();

                bSetLinkRate = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            if(GET_MHL_RECEIVE_MSC_PORT2()) // Receive MSC
            {
                CLR_MHL_RECEIVE_MSC_PORT2();

                if(GET_MHL_RECEIVE_ABORT_PORT2()) // Receive Abort
                {
                    CLR_MHL_RECEIVE_ABORT_PORT2();

                    ucTimeTick = MHL_CBUS_ABORT_HOLD_TIME;
                }

                bSetTime = TRUE;
            }

            if(GET_MHL_RECEIVE_ACK_PORT2()) // Receive Ack
            {
                CLR_MHL_RECEIVE_ACK_PORT2();

                if(GET_MHL_QUEUE_STATE_PORT2(MHL_CBUS_RX_QUEUE_INDEX) != CBUS_REQ_IDLE)
                {
                    SET_MHL_QUEUE_STATE_PORT2(MHL_CBUS_RX_QUEUE_INDEX, CBUS_REQ_IDLE);
                }
                else if(GET_MHL_QUEUE_STATE_PORT2(GET_MHL_TX_QUEUE_INDEX_PORT2()) != CBUS_REQ_PENDING)
                {
                    SET_MHL_QUEUE_STATE_PORT2(GET_MHL_TX_QUEUE_INDEX_PORT2(), CBUS_REQ_IDLE);
                }

                ucQueueIndex = _mdrv_mhl_GetCbusQueueIndex(ucPortSelect, GET_MHL_TX_QUEUE_INDEX_PORT2());

                // Update current index to search index, but not Rx queue index
                if(ucQueueIndex < MHL_CBUS_RX_QUEUE_INDEX)
                {
                    SET_MHL_TX_QUEUE_INDEX_PORT2(ucQueueIndex);
                }

                bCheckDisplayLink = TRUE;
            }

            if(GET_MHL_RECEIVE_DEVCAP_PORT2()) // Receive devcap ready or change
            {
                if(!GET_MHL_READ_XDEVCAP_ENABLE2())
                {
                    CLR_MHL_RECEIVE_DEVCAP_PORT2();

#if(MHL_READ_DEVICE_CAPABILITY)
                    SET_MHL_READ_DEVCAP_ENABLE2();
#endif
                }
            }

            if(GET_MHL_RECEIVE_WRITE_GRANT2()) // Receive write burst grant
            {
                CLR_MHL_RECEIVE_WRITE_GRANT2();

                if(GET_MHL_WRITE_BURST_STATE_PORT2() == MHL_CBUS_WRITE_BURST_SEND_REQUEST)
                {
                    SET_MHL_WRITE_BURST_STATE_PORT2(MHL_CBUS_WRITE_BURST_REVEIVE_GRANT);
                }
            }

            if(GET_MHL_RECEIVE_3D_REQ_PORT2()) // Receive 3D request
            {
                CLR_MHL_RECEIVE_3D_REQ_PORT2();
                SET_MHL_SEND_3D_INFORMATION_PORT2();
                SET_MHL_SEND_3D_DTD_PORT2();
                SET_MHL_SEND_3D_VIC_PORT2();
            }

            if(GET_MHL_RECEIVE_24BIT_MODE_PORT2()) // Receive change to 24 bit mode
            {
                CLR_MHL_RECEIVE_24BIT_MODE_PORT2();

                bClockModeChange = TRUE;
                ucClockMode = MHL_PHY_CLOCK_NORMAL_24BIT;
            }

            if(GET_MHL_RECEIVE_PP_MODE_PORT2()) // Receive change to packet pixel mode
            {
                CLR_MHL_RECEIVE_PP_MODE_PORT2();

                bClockModeChange = TRUE;
                ucClockMode = MHL_PHY_CLOCK_PACKET_PIXEL;
            }

            if(GET_MHL_RECEIVE_WRITE_REQUEST2()) // Receive write burst request
            {
                CLR_MHL_RECEIVE_WRITE_REQUEST2();

                if(GET_MHL_WRITE_BURST_STATE_PORT2() <= MHL_CBUS_WRITE_BURST_START)
                {
                    SET_MHL_WRITE_BURST_STATE_PORT2(MHL_CBUS_WRITE_BURST_REVEIVE_REQUEST);
                }
            }

            if(GET_MHL_RECEIVE_DSCR_CHANGE2()) // Receive scratchpad register chagne
            {
                CLR_MHL_RECEIVE_DSCR_CHANGE2();
                SET_MHL_WRITE_BURST_STATE_PORT2(MHL_CBUS_WRITE_BURST_END);
            }

            if(GET_MHL_RECEIVE_MHL3_PORT2()) // Receive source support MHL3
            {
                CLR_MHL_RECEIVE_MHL3_PORT2();

                if(bECbusSupport)
                {
                    SET_MHL_ENABLE_CBUS_ENHANCE2();
                }
            }

            if(GET_MHL_RECEIVE_XDEVCAP_SUPPORT2() && GET_MHL_VERSION_CODE_FLAG2()) // Receive extended device capability support
            {
                if(!GET_MHL_READ_DEVCAP_ENABLE2())
                {
                    CLR_MHL_RECEIVE_XDEVCAP_SUPPORT2();

#if(MHL_READ_DEVICE_CAPABILITY)
                    SET_MHL_READ_XDEVCAP_ENABLE2();
#endif
                }
            }

            if(GET_MHL_RECEIVE_VC_ASSIGN_PORT2()) // Receive virtual channel assign
            {
                CLR_MHL_RECEIVE_VC_ASSIGN_PORT2();
                SET_MHL_SEND_VC_INFORMATION_PORT2();
                SET_MHL_SEND_VC_CONFIRM_PORT2();
            }

            if(GET_MHL_RECEIVE_FEATURE_REQUEST2()) // Receive feature request
            {
                CLR_MHL_RECEIVE_FEATURE_REQUEST2();

                if(GET_MHL_FEATURE_REQ_STATE_PORT2() == MHL_CBUS_FEATURE_NONE)
                {
                    SET_MHL_FEATURE_REQ_STATE_PORT2(MHL_CBUS_FEATURE_START);
                }
            }

            if(GET_MHL_RECEIVE_FEATURE_COMPLETE2()) // Receive feature complete
            {
                CLR_MHL_RECEIVE_FEATURE_COMPLETE2();

                if(GET_MHL_FEATURE_REQ_STATE_PORT2() == MHL_CBUS_FEATURE_SEND_FEATURE_REQUEST)
                {
                    SET_MHL_FEATURE_REQ_STATE_PORT2(MHL_CBUS_FEATURE_END);
                }
            }

            if(GET_MHL_RECEIVE_LINK_RATE_PORT2()) // Receive link rate
            {
                CLR_MHL_RECEIVE_LINK_RATE_PORT2();

                bSetLinkRate = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            if(GET_MHL_RECEIVE_MSC_PORT3()) // Receive MSC
            {
                CLR_MHL_RECEIVE_MSC_PORT3();

                if(GET_MHL_RECEIVE_ABORT_PORT3()) // Receive Abort
                {
                    CLR_MHL_RECEIVE_ABORT_PORT3();

                    ucTimeTick = MHL_CBUS_ABORT_HOLD_TIME;
                }

                bSetTime = TRUE;
            }

            if(GET_MHL_RECEIVE_ACK_PORT3()) // Receive Ack
            {
                CLR_MHL_RECEIVE_ACK_PORT3();

                if(GET_MHL_QUEUE_STATE_PORT3(MHL_CBUS_RX_QUEUE_INDEX) != CBUS_REQ_IDLE)
                {
                    SET_MHL_QUEUE_STATE_PORT3(MHL_CBUS_RX_QUEUE_INDEX, CBUS_REQ_IDLE);
                }
                else if(GET_MHL_QUEUE_STATE_PORT3(GET_MHL_TX_QUEUE_INDEX_PORT3()) != CBUS_REQ_PENDING)
                {
                    SET_MHL_QUEUE_STATE_PORT3(GET_MHL_TX_QUEUE_INDEX_PORT3(), CBUS_REQ_IDLE);
                }

                ucQueueIndex = _mdrv_mhl_GetCbusQueueIndex(ucPortSelect, GET_MHL_TX_QUEUE_INDEX_PORT3());

                // Update current index to search index, but not Rx queue index
                if(ucQueueIndex < MHL_CBUS_RX_QUEUE_INDEX)
                {
                    SET_MHL_TX_QUEUE_INDEX_PORT3(ucQueueIndex);
                }

                bCheckDisplayLink = TRUE;
            }

            if(GET_MHL_RECEIVE_DEVCAP_PORT3()) // Receive devcap ready or change
            {
                if(!GET_MHL_READ_XDEVCAP_ENABLE3())
                {
                    CLR_MHL_RECEIVE_DEVCAP_PORT3();

#if(MHL_READ_DEVICE_CAPABILITY)
                    SET_MHL_READ_DEVCAP_ENABLE3();
#endif
                }
            }

            if(GET_MHL_RECEIVE_WRITE_GRANT3()) // Receive write burst grant
            {
                CLR_MHL_RECEIVE_WRITE_GRANT3();

                if(GET_MHL_WRITE_BURST_STATE_PORT3() == MHL_CBUS_WRITE_BURST_SEND_REQUEST)
                {
                    SET_MHL_WRITE_BURST_STATE_PORT3(MHL_CBUS_WRITE_BURST_REVEIVE_GRANT);
                }
            }

            if(GET_MHL_RECEIVE_3D_REQ_PORT3()) // Receive 3D request
            {
                CLR_MHL_RECEIVE_3D_REQ_PORT3();
                SET_MHL_SEND_3D_INFORMATION_PORT3();
                SET_MHL_SEND_3D_DTD_PORT3();
                SET_MHL_SEND_3D_VIC_PORT3();
            }

            if(GET_MHL_RECEIVE_24BIT_MODE_PORT3()) // Receive change to 24 bit mode
            {
                CLR_MHL_RECEIVE_24BIT_MODE_PORT3();

                bClockModeChange = TRUE;
                ucClockMode = MHL_PHY_CLOCK_NORMAL_24BIT;
            }

            if(GET_MHL_RECEIVE_PP_MODE_PORT3()) // Receive change to packet pixel mode
            {
                CLR_MHL_RECEIVE_PP_MODE_PORT3();

                bClockModeChange = TRUE;
                ucClockMode = MHL_PHY_CLOCK_PACKET_PIXEL;
            }

            if(GET_MHL_RECEIVE_WRITE_REQUEST3()) // Receive write burst request
            {
                CLR_MHL_RECEIVE_WRITE_REQUEST3();

                if(GET_MHL_WRITE_BURST_STATE_PORT3() <= MHL_CBUS_WRITE_BURST_START)
                {
                    SET_MHL_WRITE_BURST_STATE_PORT3(MHL_CBUS_WRITE_BURST_REVEIVE_REQUEST);
                }
            }

            if(GET_MHL_RECEIVE_DSCR_CHANGE3()) // Receive scratchpad register chagne
            {
                CLR_MHL_RECEIVE_DSCR_CHANGE3();
                SET_MHL_WRITE_BURST_STATE_PORT3(MHL_CBUS_WRITE_BURST_END);
            }

            if(GET_MHL_RECEIVE_MHL3_PORT3()) // Receive source support MHL3
            {
                CLR_MHL_RECEIVE_MHL3_PORT3();

                if(bECbusSupport)
                {
                    SET_MHL_ENABLE_CBUS_ENHANCE3();
                }
            }

            if(GET_MHL_RECEIVE_XDEVCAP_SUPPORT3() && GET_MHL_VERSION_CODE_FLAG3()) // Receive extended device capability support
            {
                if(!GET_MHL_READ_DEVCAP_ENABLE3())
                {
                    CLR_MHL_RECEIVE_XDEVCAP_SUPPORT3();

#if(MHL_READ_DEVICE_CAPABILITY)
                    SET_MHL_READ_XDEVCAP_ENABLE3();
#endif
                }
            }

            if(GET_MHL_RECEIVE_VC_ASSIGN_PORT3()) // Receive virtual channel assign
            {
                CLR_MHL_RECEIVE_VC_ASSIGN_PORT3();
                SET_MHL_SEND_VC_INFORMATION_PORT3();
                SET_MHL_SEND_VC_CONFIRM_PORT3();
            }

            if(GET_MHL_RECEIVE_FEATURE_REQUEST3()) // Receive feature request
            {
                CLR_MHL_RECEIVE_FEATURE_REQUEST3();

                if(GET_MHL_FEATURE_REQ_STATE_PORT3() == MHL_CBUS_FEATURE_NONE)
                {
                    SET_MHL_FEATURE_REQ_STATE_PORT3(MHL_CBUS_FEATURE_START);
                }
            }

            if(GET_MHL_RECEIVE_FEATURE_COMPLETE3()) // Receive feature complete
            {
                CLR_MHL_RECEIVE_FEATURE_COMPLETE3();

                if(GET_MHL_FEATURE_REQ_STATE_PORT3() == MHL_CBUS_FEATURE_SEND_FEATURE_REQUEST)
                {
                    SET_MHL_FEATURE_REQ_STATE_PORT3(MHL_CBUS_FEATURE_END);
                }
            }

            if(GET_MHL_RECEIVE_LINK_RATE_PORT3()) // Receive link rate
            {
                CLR_MHL_RECEIVE_LINK_RATE_PORT3();

                bSetLinkRate = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            if(GET_MHL_RECEIVE_MSC_PORT4()) // Receive MSC
            {
                CLR_MHL_RECEIVE_MSC_PORT4();

                if(GET_MHL_RECEIVE_ABORT_PORT4()) // Receive Abort
                {
                    CLR_MHL_RECEIVE_ABORT_PORT4();

                    ucTimeTick = MHL_CBUS_ABORT_HOLD_TIME;
                }

                bSetTime = TRUE;
            }

            if(GET_MHL_RECEIVE_ACK_PORT4()) // Receive Ack
            {
                CLR_MHL_RECEIVE_ACK_PORT4();

                if(GET_MHL_QUEUE_STATE_PORT4(MHL_CBUS_RX_QUEUE_INDEX) != CBUS_REQ_IDLE)
                {
                    SET_MHL_QUEUE_STATE_PORT4(MHL_CBUS_RX_QUEUE_INDEX, CBUS_REQ_IDLE);
                }
                else if(GET_MHL_QUEUE_STATE_PORT4(GET_MHL_TX_QUEUE_INDEX_PORT4()) != CBUS_REQ_PENDING)
                {
                    SET_MHL_QUEUE_STATE_PORT4(GET_MHL_TX_QUEUE_INDEX_PORT4(), CBUS_REQ_IDLE);
                }

                ucQueueIndex = _mdrv_mhl_GetCbusQueueIndex(ucPortSelect, GET_MHL_TX_QUEUE_INDEX_PORT4());

                // Update current index to search index, but not Rx queue index
                if(ucQueueIndex < MHL_CBUS_RX_QUEUE_INDEX)
                {
                    SET_MHL_TX_QUEUE_INDEX_PORT4(ucQueueIndex);
                }

                bCheckDisplayLink = TRUE;
            }

            if(GET_MHL_RECEIVE_DEVCAP_PORT4()) // Receive devcap ready or change
            {
                if(!GET_MHL_READ_XDEVCAP_ENABLE4())
                {
                    CLR_MHL_RECEIVE_DEVCAP_PORT4();

#if(MHL_READ_DEVICE_CAPABILITY)
                    SET_MHL_READ_DEVCAP_ENABLE4();
#endif
                }
            }

            if(GET_MHL_RECEIVE_WRITE_GRANT4()) // Receive write burst grant
            {
                CLR_MHL_RECEIVE_WRITE_GRANT4();

                if(GET_MHL_WRITE_BURST_STATE_PORT4() == MHL_CBUS_WRITE_BURST_SEND_REQUEST)
                {
                    SET_MHL_WRITE_BURST_STATE_PORT4(MHL_CBUS_WRITE_BURST_REVEIVE_GRANT);
                }
            }

            if(GET_MHL_RECEIVE_3D_REQ_PORT4()) // Receive 3D request
            {
                CLR_MHL_RECEIVE_3D_REQ_PORT4();
                SET_MHL_SEND_3D_INFORMATION_PORT4();
                SET_MHL_SEND_3D_DTD_PORT4();
                SET_MHL_SEND_3D_VIC_PORT4();
            }

            if(GET_MHL_RECEIVE_24BIT_MODE_PORT4()) // Receive change to 24 bit mode
            {
                CLR_MHL_RECEIVE_24BIT_MODE_PORT4();

                bClockModeChange = TRUE;
                ucClockMode = MHL_PHY_CLOCK_NORMAL_24BIT;
            }

            if(GET_MHL_RECEIVE_PP_MODE_PORT4()) // Receive change to packet pixel mode
            {
                CLR_MHL_RECEIVE_PP_MODE_PORT4();

                bClockModeChange = TRUE;
                ucClockMode = MHL_PHY_CLOCK_PACKET_PIXEL;
            }

            if(GET_MHL_RECEIVE_WRITE_REQUEST4()) // Receive write burst request
            {
                CLR_MHL_RECEIVE_WRITE_REQUEST4();

                if(GET_MHL_WRITE_BURST_STATE_PORT4() <= MHL_CBUS_WRITE_BURST_START)
                {
                    SET_MHL_WRITE_BURST_STATE_PORT4(MHL_CBUS_WRITE_BURST_REVEIVE_REQUEST);
                }
            }

            if(GET_MHL_RECEIVE_DSCR_CHANGE4()) // Receive scratchpad register chagne
            {
                CLR_MHL_RECEIVE_DSCR_CHANGE4();
                SET_MHL_WRITE_BURST_STATE_PORT4(MHL_CBUS_WRITE_BURST_END);
            }

            if(GET_MHL_RECEIVE_MHL3_PORT4()) // Receive source support MHL3
            {
                CLR_MHL_RECEIVE_MHL3_PORT4();

                if(bECbusSupport)
                {
                    SET_MHL_ENABLE_CBUS_ENHANCE4();
                }
            }

            if(GET_MHL_RECEIVE_XDEVCAP_SUPPORT4() && GET_MHL_VERSION_CODE_FLAG4()) // Receive extended device capability support
            {
                if(!GET_MHL_READ_DEVCAP_ENABLE4())
                {
                    CLR_MHL_RECEIVE_XDEVCAP_SUPPORT4();

#if(MHL_READ_DEVICE_CAPABILITY)
                    SET_MHL_READ_XDEVCAP_ENABLE4();
#endif
                }
            }

            if(GET_MHL_RECEIVE_VC_ASSIGN_PORT4()) // Receive virtual channel assign
            {
                CLR_MHL_RECEIVE_VC_ASSIGN_PORT4();
                SET_MHL_SEND_VC_INFORMATION_PORT4();
                SET_MHL_SEND_VC_CONFIRM_PORT4();
            }

            if(GET_MHL_RECEIVE_FEATURE_REQUEST4()) // Receive feature request
            {
                CLR_MHL_RECEIVE_FEATURE_REQUEST4();

                if(GET_MHL_FEATURE_REQ_STATE_PORT4() == MHL_CBUS_FEATURE_NONE)
                {
                    SET_MHL_FEATURE_REQ_STATE_PORT4(MHL_CBUS_FEATURE_START);
                }
            }

            if(GET_MHL_RECEIVE_FEATURE_COMPLETE4()) // Receive feature complete
            {
                CLR_MHL_RECEIVE_FEATURE_COMPLETE4();

                if(GET_MHL_FEATURE_REQ_STATE_PORT4() == MHL_CBUS_FEATURE_SEND_FEATURE_REQUEST)
                {
                    SET_MHL_FEATURE_REQ_STATE_PORT4(MHL_CBUS_FEATURE_END);
                }
            }

            if(GET_MHL_RECEIVE_LINK_RATE_PORT4()) // Receive link rate
            {
                CLR_MHL_RECEIVE_LINK_RATE_PORT4();

                bSetLinkRate = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            if(GET_MHL_RECEIVE_MSC_PORT5()) // Receive MSC
            {
                CLR_MHL_RECEIVE_MSC_PORT5();

                if(GET_MHL_RECEIVE_ABORT_PORT5()) // Receive Abort
                {
                    CLR_MHL_RECEIVE_ABORT_PORT5();

                    ucTimeTick = MHL_CBUS_ABORT_HOLD_TIME;
                }

                bSetTime = TRUE;
            }

            if(GET_MHL_RECEIVE_ACK_PORT5()) // Receive Ack
            {
                CLR_MHL_RECEIVE_ACK_PORT5();

                if(GET_MHL_QUEUE_STATE_PORT5(MHL_CBUS_RX_QUEUE_INDEX) != CBUS_REQ_IDLE)
                {
                    SET_MHL_QUEUE_STATE_PORT5(MHL_CBUS_RX_QUEUE_INDEX, CBUS_REQ_IDLE);
                }
                else if(GET_MHL_QUEUE_STATE_PORT5(GET_MHL_TX_QUEUE_INDEX_PORT5()) != CBUS_REQ_PENDING)
                {
                    SET_MHL_QUEUE_STATE_PORT5(GET_MHL_TX_QUEUE_INDEX_PORT5(), CBUS_REQ_IDLE);
                }

                ucQueueIndex = _mdrv_mhl_GetCbusQueueIndex(ucPortSelect, GET_MHL_TX_QUEUE_INDEX_PORT5());

                // Update current index to search index, but not Rx queue index
                if(ucQueueIndex < MHL_CBUS_RX_QUEUE_INDEX)
                {
                    SET_MHL_TX_QUEUE_INDEX_PORT5(ucQueueIndex);
                }

                bCheckDisplayLink = TRUE;
            }

            if(GET_MHL_RECEIVE_DEVCAP_PORT5()) // Receive devcap ready or change
            {
                if(!GET_MHL_READ_XDEVCAP_ENABLE5())
                {
                    CLR_MHL_RECEIVE_DEVCAP_PORT5();

#if(MHL_READ_DEVICE_CAPABILITY)
                    SET_MHL_READ_DEVCAP_ENABLE5();
#endif
                }
            }

            if(GET_MHL_RECEIVE_WRITE_GRANT5()) // Receive write burst grant
            {
                CLR_MHL_RECEIVE_WRITE_GRANT5();

                if(GET_MHL_WRITE_BURST_STATE_PORT5() == MHL_CBUS_WRITE_BURST_SEND_REQUEST)
                {
                    SET_MHL_WRITE_BURST_STATE_PORT5(MHL_CBUS_WRITE_BURST_REVEIVE_GRANT);
                }
            }

            if(GET_MHL_RECEIVE_3D_REQ_PORT5()) // Receive 3D request
            {
                CLR_MHL_RECEIVE_3D_REQ_PORT5();
                SET_MHL_SEND_3D_INFORMATION_PORT5();
                SET_MHL_SEND_3D_DTD_PORT5();
                SET_MHL_SEND_3D_VIC_PORT5();
            }

            if(GET_MHL_RECEIVE_24BIT_MODE_PORT5()) // Receive change to 24 bit mode
            {
                CLR_MHL_RECEIVE_24BIT_MODE_PORT5();

                bClockModeChange = TRUE;
                ucClockMode = MHL_PHY_CLOCK_NORMAL_24BIT;
            }

            if(GET_MHL_RECEIVE_PP_MODE_PORT5()) // Receive change to packet pixel mode
            {
                CLR_MHL_RECEIVE_PP_MODE_PORT5();

                bClockModeChange = TRUE;
                ucClockMode = MHL_PHY_CLOCK_PACKET_PIXEL;
            }

            if(GET_MHL_RECEIVE_WRITE_REQUEST5()) // Receive write burst request
            {
                CLR_MHL_RECEIVE_WRITE_REQUEST5();

                if(GET_MHL_WRITE_BURST_STATE_PORT5() <= MHL_CBUS_WRITE_BURST_START)
                {
                    SET_MHL_WRITE_BURST_STATE_PORT5(MHL_CBUS_WRITE_BURST_REVEIVE_REQUEST);
                }
            }

            if(GET_MHL_RECEIVE_DSCR_CHANGE5()) // Receive scratchpad register chagne
            {
                CLR_MHL_RECEIVE_DSCR_CHANGE5();
                SET_MHL_WRITE_BURST_STATE_PORT5(MHL_CBUS_WRITE_BURST_END);
            }

            if(GET_MHL_RECEIVE_MHL3_PORT5()) // Receive source support MHL3
            {
                CLR_MHL_RECEIVE_MHL3_PORT5();

                if(bECbusSupport)
                {
                    SET_MHL_ENABLE_CBUS_ENHANCE5();
                }
            }

            if(GET_MHL_RECEIVE_XDEVCAP_SUPPORT5() && GET_MHL_VERSION_CODE_FLAG5()) // Receive extended device capability support
            {
                if(!GET_MHL_READ_DEVCAP_ENABLE5())
                {
                    CLR_MHL_RECEIVE_XDEVCAP_SUPPORT5();

#if(MHL_READ_DEVICE_CAPABILITY)
                    SET_MHL_READ_XDEVCAP_ENABLE5();
#endif
                }
            }

            if(GET_MHL_RECEIVE_VC_ASSIGN_PORT5()) // Receive virtual channel assign
            {
                CLR_MHL_RECEIVE_VC_ASSIGN_PORT5();
                SET_MHL_SEND_VC_INFORMATION_PORT5();
                SET_MHL_SEND_VC_CONFIRM_PORT5();
            }

            if(GET_MHL_RECEIVE_FEATURE_REQUEST5()) // Receive feature request
            {
                CLR_MHL_RECEIVE_FEATURE_REQUEST5();

                if(GET_MHL_FEATURE_REQ_STATE_PORT5() == MHL_CBUS_FEATURE_NONE)
                {
                    SET_MHL_FEATURE_REQ_STATE_PORT5(MHL_CBUS_FEATURE_START);
                }
            }

            if(GET_MHL_RECEIVE_FEATURE_COMPLETE5()) // Receive feature complete
            {
                CLR_MHL_RECEIVE_FEATURE_COMPLETE5();

                if(GET_MHL_FEATURE_REQ_STATE_PORT5() == MHL_CBUS_FEATURE_SEND_FEATURE_REQUEST)
                {
                    SET_MHL_FEATURE_REQ_STATE_PORT5(MHL_CBUS_FEATURE_END);
                }
            }

            if(GET_MHL_RECEIVE_LINK_RATE_PORT5()) // Receive link rate
            {
                CLR_MHL_RECEIVE_LINK_RATE_PORT5();

                bSetLinkRate = TRUE;
            }

            break;
#endif

        default:

            break;
    };

    if(bSetTime)
    {
        _mdrv_mhl_InsertCbusTimer(ucPortSelect, ucTimeTick);
    }

    if(bClockModeChange)
    {
        _mdrv_mhl_ClockModeSwitchProc(ucPortSelect, ucClockMode);
    }

    if(bCheckDisplayLink)
    {
        _mdrv_mhl_CbusDisplayLinkCheck(ucPortSelect);
    }

    if(bSetLinkRate)
    {
        _mdrv_mhl_SetMainLinkRate(ucPortSelect);
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusStucktoLowProc()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool _mdrv_mhl_GetReceiveMSGInformation(BYTE ucPortSelect, BYTE *ucSubCommand, BYTE *ucKeyCode)
{
    Bool bReceiveMSG = FALSE;

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            if(GET_MHL_RECEIVE_MSG_PORT0())
            {
                CLR_MHL_RECEIVE_MSG_PORT0();

                *ucSubCommand = GET_MHL_RX_MSG_COMMAND_PORT0();
                *ucKeyCode = GET_MHL_RX_MSG_DATA_PORT0();

                bReceiveMSG = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            if(GET_MHL_RECEIVE_MSG_PORT1())
            {
                CLR_MHL_RECEIVE_MSG_PORT1();

                *ucSubCommand = GET_MHL_RX_MSG_COMMAND_PORT1();
                *ucKeyCode = GET_MHL_RX_MSG_DATA_PORT1();

                bReceiveMSG = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            if(GET_MHL_RECEIVE_MSG_PORT2())
            {
                CLR_MHL_RECEIVE_MSG_PORT2();

                *ucSubCommand = GET_MHL_RX_MSG_COMMAND_PORT2();
                *ucKeyCode = GET_MHL_RX_MSG_DATA_PORT2();

                bReceiveMSG = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            if(GET_MHL_RECEIVE_MSG_PORT3())
            {
                CLR_MHL_RECEIVE_MSG_PORT3();

                *ucSubCommand = GET_MHL_RX_MSG_COMMAND_PORT3();
                *ucKeyCode = GET_MHL_RX_MSG_DATA_PORT3();

                bReceiveMSG = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            if(GET_MHL_RECEIVE_MSG_PORT4())
            {
                CLR_MHL_RECEIVE_MSG_PORT4();

                *ucSubCommand = GET_MHL_RX_MSG_COMMAND_PORT4();
                *ucKeyCode = GET_MHL_RX_MSG_DATA_PORT4();

                bReceiveMSG = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            if(GET_MHL_RECEIVE_MSG_PORT5())
            {
                CLR_MHL_RECEIVE_MSG_PORT5();

                *ucSubCommand = GET_MHL_RX_MSG_COMMAND_PORT5();
                *ucKeyCode = GET_MHL_RX_MSG_DATA_PORT5();

                bReceiveMSG = TRUE;
            }

            break;
#endif

        default:

            break;
    };

    return bReceiveMSG;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusRxQueueInsertMSG(BYTE subCmd, BYTE databuf)
//  [Description]
//                  MHL Cbus Reply MSC RCP or RAP into Queue
//  [Arguments]:
//                  subCmd: MSC subcommands
//                  databuf: command code
//  [Return]:
//                  TRUE: success
//                  FALSE: fail
//**************************************************************************
void _mdrv_mhl_CbusRxQueueInsertMSG(BYTE ucPortSelect, BYTE ucSubCommand, BYTE ucData)
{
    CbusReq_S stQueue;

    stQueue.cmd = MSC_MSC_MSG;
    stQueue.msgData[0] = ucSubCommand;
    stQueue.msgData[1] = ucData;
    stQueue.len = 2;
    stQueue.reqStatus = CBUS_REQ_PENDING;

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            memcpy(&(GET_MHL_CBUS_QUEUE_PORT0(MHL_CBUS_RX_QUEUE_INDEX)), &stQueue, sizeof(CbusReq_S));
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            memcpy(&(GET_MHL_CBUS_QUEUE_PORT1(MHL_CBUS_RX_QUEUE_INDEX)), &stQueue, sizeof(CbusReq_S));
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            memcpy(&(GET_MHL_CBUS_QUEUE_PORT2(MHL_CBUS_RX_QUEUE_INDEX)), &stQueue, sizeof(CbusReq_S));
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            memcpy(&(GET_MHL_CBUS_QUEUE_PORT3(MHL_CBUS_RX_QUEUE_INDEX)), &stQueue, sizeof(CbusReq_S));
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            memcpy(&(GET_MHL_CBUS_QUEUE_PORT4(MHL_CBUS_RX_QUEUE_INDEX)), &stQueue, sizeof(CbusReq_S));
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            memcpy(&(GET_MHL_CBUS_QUEUE_PORT5(MHL_CBUS_RX_QUEUE_INDEX)), &stQueue, sizeof(CbusReq_S));
            break;
#endif

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusMSGRCPReply()
//  [Description]
//                  MHL Cbus RCP response
//  [Arguments]:
//                  statusCode: RCP status code
//                  keyCode: RCP key code
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_CbusMSGRCPReply(BYTE ucPortSelect, BYTE ucStatus, BYTE ucKeyCode)
{
    BYTE ucSubCommand = MSC_MSG_RCPK;

    if(ucStatus != MHL_RCP_NO_ERROR)
    {
        ucSubCommand = MSC_MSG_RCPE;
        ucKeyCode = ucStatus;
    }

    _mdrv_mhl_CbusRxQueueInsertMSG(ucPortSelect, ucSubCommand, ucKeyCode);
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusMSGRAPReply()
//  [Description]
//                  MHL Cbus RAP response
//  [Arguments]:
//                  statusCode: RAP status code
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_CbusMSGRAPReply(BYTE ucPortSelect, BYTE ucStatus)
{
    _mdrv_mhl_CbusRxQueueInsertMSG(ucPortSelect, MSC_MSG_RAPK, ucStatus);
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusMSGUCPReply()
//  [Description]
//                  MHL Cbus UCP response
//  [Arguments]:
//                  statusCode: UCP status code
//                  keyCode: UCP key code
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_CbusMSGUCPReply(BYTE ucPortSelect, BYTE ucStatus, BYTE ucKeyCode)
{
    BYTE ucSubCommand = MSC_MSG_UCPK;

    if(ucStatus != MHL_UCP_NO_ERROR)
    {
        ucSubCommand = MSC_MSG_UCPE;
        ucKeyCode = ucStatus;
    }

    _mdrv_mhl_CbusRxQueueInsertMSG(ucPortSelect, ucSubCommand, ucKeyCode);
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusMSGATTReply()
//  [Description]
//                  MHL Cbus ATT response
//  [Arguments]:
//                  statusCode: ATT status code
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_CbusMSGATTReply(BYTE ucPortSelect, BYTE ucStatus)
{
    _mdrv_mhl_CbusRxQueueInsertMSG(ucPortSelect, MSC_MSG_ATTK, ucStatus);
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusMSGRBPReply()
//  [Description]
//                  MHL Cbus RBP response
//  [Arguments]:
//                  statusCode: RBP status code
//                  keyCode: RBP key code
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_CbusMSGRBPReply(BYTE ucPortSelect, BYTE ucStatus, BYTE ucKeyCode)
{
    BYTE ucSubCommand = MSC_MSG_RBPK;

    if(ucStatus != MHL_RBP_NO_ERROR)
    {
        ucSubCommand = MSC_MSG_RBPE;
        ucKeyCode = ucStatus;
    }

    _mdrv_mhl_CbusRxQueueInsertMSG(ucPortSelect, ucSubCommand, ucKeyCode);
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusMSGRHIDReply()
//  [Description]
//                  MHL Cbus RHID response
//  [Arguments]:
//                  statusCode: RHID status code
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_CbusMSGRHIDReply(BYTE ucPortSelect, BYTE ucStatus)
{
    _mdrv_mhl_CbusRxQueueInsertMSG(ucPortSelect, MSC_MSG_RHIDK, ucStatus);
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_DTDPresentinEDID()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool _mdrv_mhl_DTDPresentinEDID(BYTE *pData)
{
    Bool bPresent = FALSE;
    BYTE uctemp = 0;

    for(uctemp = 0; uctemp < 3; uctemp++)
    {
        if(pData[uctemp] > 0)
        {
            bPresent = TRUE;

            break;
        }
    }

    return bPresent;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_InsertDTD3DInformation()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE _mdrv_mhl_InsertDTD3DInformation(BYTE ucIndex, BYTE ucData, BYTE *uc3DInformationDTD)
{
    BYTE uctemp = 0;

    ucData = ((ucData &MASKBIT(6:5)) >> 4) |(ucData &BIT0);

    switch(ucData)
    {
        case MHL_3D_DTD_STEREO_FIELD_SEQUENTIAL_RIGHT:
            uctemp = MASKSET(uctemp, MHL_3D_FS_SUPPORT, MHL_3D_SUPPORT_MASK);
            break;

        case MHL_3D_DTD_STEREO_FIELD_SEQUENTIAL_LEFT:
            uctemp = MASKSET(uctemp, MHL_3D_FS_SUPPORT, MHL_3D_SUPPORT_MASK);
            break;

        case MHL_3D_DTD_STEREO_2WAY_INTERLEAVED_RIGHT:
            // No setting
            break;

        case MHL_3D_DTD_STEREO_2WAY_INTERLEAVED_LEFT:
            // No setting
            break;

        case MHL_3D_DTD_STEREO_4WAY_INTERLEAVED:
            // No setting
            break;

        case MHL_3D_DTD_STEREO_SIDE_BY_SIDE_INTERLEAVED:
            uctemp = MASKSET(uctemp, MHL_3D_LR_SUPPORT, MHL_3D_SUPPORT_MASK);
            break;

        default: // MHL_3D_DTD_STEREO_NORMAL_DISPLAY

            break;
    };

    uc3DInformationDTD[ucIndex] = uctemp;

    for(uctemp = 0, ucData = 0; uctemp < (ucIndex +1); uctemp++)
    {
        if(uc3DInformationDTD[uctemp] > 0)
        {
            ucData = uctemp +1;
        }
    }

    return ucData;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_GetDTD3DTimingInformation()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE _mdrv_mhl_GetDTD3DTimingInformation(BYTE *pData)
{
    Bool bInterlace = ((pData[17] &BIT7) ?TRUE :FALSE);
    BYTE ucIndex = 0;
    WORD usHActive = ((pData[4] &MASKBIT(7:4)) <<4) |pData[2];
    WORD usHBlank = ((pData[4] &MASKBIT(3:0)) <<8) |pData[3];
    WORD usVActive = ((pData[7] &MASKBIT(7:4)) <<4) |pData[5];
    WORD usVBlank = ((pData[7] &MASKBIT(3:0)) <<8) |pData[6];
    WORD usHTotal = usHActive +usHBlank;
    WORD usVTotal = usVActive +usVBlank;
    WORD usPixelClk = (pData[1] <<8) |pData[0];
    DWORD ulFrequenct = 0;

    if((usHTotal > 0) && (usVTotal > 0))
    {
        ulFrequenct = (usPixelClk *100 /usHTotal) *1000 /usVTotal;

        //msg_mhl(printf("** MHL get DTD timing %d x %d @ %d\r\n", usHActive, usVActive, usPixelClk));

        if((usHActive == 1920) && (usVActive == 1080)) // 1920x1080p
        {
            if((ulFrequenct > 230) && (ulFrequenct < 245)) // 24Hz
            {
                ucIndex = MHL_3D_PRESENT_VIC_1920x1080p24Hz;
            }
        }
        else if((usHActive == 1920) && (usVActive == 540) && bInterlace) // 1920x1080i
        {
            if((ulFrequenct > 590) && (ulFrequenct < 610)) // 60Hz
            {
                ucIndex = MHL_3D_PRESENT_VIC_1920x1080i60Hz;
            }
            else if((ulFrequenct > 490) && (ulFrequenct < 510)) // 50Hz
            {
                ucIndex = MHL_3D_PRESENT_VIC_1920x1080i50Hz;
            }
        }
        else if((usHActive == 1280) && (usVActive == 720)) // 1280x720p
        {
            if((ulFrequenct > 590) && (ulFrequenct < 610)) // 60Hz
            {
                ucIndex = MHL_3D_PRESENT_VIC_1280x720p60Hz;
            }
            else if((ulFrequenct > 490) && (ulFrequenct < 510)) // 50Hz
            {
                ucIndex = MHL_3D_PRESENT_VIC_1280x720p50Hz;
            }
        }
    }

    return ucIndex;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CheckVIC3DStructureAll()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE _mdrv_mhl_CheckVIC3DStructureAll(WORD usData)
{
    BYTE uctemp = 0;

    if((usData &BIT0) == BIT0) // Support frame packing 3D formats
    {
        uctemp = MASKSET(uctemp, MHL_3D_FS_SUPPORT, MHL_3D_FS_SUPPORT);
    }

    if((usData &BIT6) == BIT6) // Support top and bottom 3D formats
    {
        uctemp = MASKSET(uctemp, MHL_3D_TB_SUPPORT, MHL_3D_TB_SUPPORT);
    }

    if((usData &BIT8) == BIT8) // Support side by side 3D formats
    {
        uctemp = MASKSET(uctemp, MHL_3D_LR_SUPPORT, MHL_3D_LR_SUPPORT);
    }

    return uctemp;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CheckVIC3DStructure()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE _mdrv_mhl_CheckVIC3DStructure(BYTE ucData)
{
    BYTE uctemp = 0;

    if(ucData == 0) // Support frame packing
    {
        uctemp = MASKSET(uctemp, MHL_3D_FS_SUPPORT, MHL_3D_FS_SUPPORT);
    }
    else if(ucData == 6) // Support top and bottom
    {
        uctemp = MASKSET(uctemp, MHL_3D_TB_SUPPORT, MHL_3D_TB_SUPPORT);
    }
    else if(ucData == 8) // Support side by side
    {
        uctemp = MASKSET(uctemp, MHL_3D_LR_SUPPORT, MHL_3D_LR_SUPPORT);
    }

    return uctemp;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_ParsingVIC3DInformation()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool _mdrv_mhl_ParsingVIC3DInformation(BYTE ucPortSelect, BYTE ucExtendLength, BYTE *pData)
{
    Bool bVICParseFinish = TRUE;
    Bool b3DPresent = FALSE;
    BYTE uctemp = 0;
    BYTE ucIndex = 0;
    BYTE ucValue = 0;
    BYTE ucLength = 0;
    BYTE ucVSDBAddr = 0;
    BYTE ucVSDBLength = 0;
    BYTE uc3DMultiPresent = 0;
    BYTE ucVICTimingNum = 0;
    BYTE ucMHL3DTotalEntryVIC = 0;
    BYTE ucVICTiming[MHL_3D_VIC_INFORMATION_LENGTH];
    BYTE ucMHL3DInformationVIC[MHL_3D_VIC_INFORMATION_LENGTH];
    WORD usMaskValue = 0;

    for(uctemp = 0; uctemp < MHL_3D_VIC_INFORMATION_LENGTH; uctemp++)
    {
        ucVICTiming[uctemp] = 0;
        ucMHL3DInformationVIC[uctemp] = 0;
    }

    for(uctemp = 0; uctemp < ucExtendLength; uctemp++)
    {
        ucValue = (pData[uctemp] &MASKBIT(7:5)) >>5;
        ucLength = pData[uctemp] &MASKBIT(4:0);

        switch(ucValue)
        {
            case MHL_EDID_AUDIO_DATA_BLOCK_CODE:
                MHL_DRV_DPRINTF("** MHL get audio data block length %d", ucLength);
                break;

            case MHL_EDID_VIDEO_DATA_BLOCK_CODE:
                MHL_DRV_DPRINTF("** MHL get video data block length %d", ucLength);

                if(ucLength > MHL_3D_VIC_INFORMATION_LENGTH)
                {
                    ucVICTimingNum = MHL_3D_VIC_INFORMATION_LENGTH;
                }
                else
                {
                    ucVICTimingNum = ucLength;
                }

                for(ucIndex = 0; ucIndex <ucVICTimingNum; ucIndex++)
                {
                    ucVICTiming[ucIndex] = pData[uctemp +ucIndex +1];
                }

                break;

            case MHL_EDID_VENDER_SPECIFIC_DATA_BLOCK_CODE:
                MHL_DRV_DPRINTF("** MHL get vender specific data block length %d", ucLength);

                ucVSDBAddr = uctemp;

                bVICParseFinish = FALSE;

                break;

            case MHL_EDID_SPEAKER_ALLOCATION_DATA_BLOCK_CODE:
                MHL_DRV_DPRINTF("** MHL get speaker allocation data block length %d", ucLength);
                break;

            case MHL_EDID_VESA_DTC_DATA_BLOCK_CODE:
                MHL_DRV_DPRINTF("** MHL get VESA DTC data block length %d", ucLength);
                break;

            case MHL_EDID_USE_EXTENDED_TAG_BLOCK_CODE:
                MHL_DRV_DPRINTF("** MHL get use extended tag length %d", ucLength);
                break;

            default:

                break;
        };

        uctemp = uctemp +ucLength;
    }

    ucIndex = MHL_VIC_PARSING_START;
    ucVSDBLength = ucVSDBAddr +1;

    while(!bVICParseFinish)
    {
        ucValue = pData[ucVSDBAddr];

        if(ucVSDBAddr > ucVSDBLength) // Check total length
        {
            ucIndex = MHL_VIC_PARSING_FINISH;
        }

        switch(ucIndex)
        {
            case MHL_VIC_PARSING_START:
                ucVSDBLength = pData[ucVSDBAddr] &MASKBIT(4:0);

                if(ucVSDBLength < 8) // HDMI_Video_Present absent, no 3D information
                {
                    ucIndex = MHL_VIC_PARSING_FINISH;
                }
                else
                {
                    ucVSDBAddr = ucVSDBAddr +8;

                    ucIndex = MHL_VIC_PARSING_CHECK_HDMI_VIDEO_PRESENT;
                }

                ucVSDBLength = ucVSDBAddr +ucVSDBLength;

                break;

            case MHL_VIC_PARSING_CHECK_HDMI_VIDEO_PRESENT:
                if((ucValue &BIT5) != BIT5) // HDMI_Video_Present absent, no 3D information
                {
                    ucIndex = MHL_VIC_PARSING_FINISH;
                }
                else
                {
                    ucIndex = MHL_VIC_PARSING_CHECK_3D_PRESENT;

                    if((ucValue &BIT7) != BIT7) // Latency_Fields_Present absent, no video and audio latency and no interlaced video and audio latency
                    {
                        ucVSDBAddr++;
                    }
                    else
                    {
                        if((ucValue &BIT6) != BIT6) // I_Latency_Fields_Present absent, no video and audio latency
                        {
                            ucVSDBAddr = ucVSDBAddr +3;
                        }
                        else
                        {
                            ucVSDBAddr = ucVSDBAddr +5;
                        }
                    }
                }

                break;

            case MHL_VIC_PARSING_CHECK_3D_PRESENT:
                if((ucValue &BIT7) != BIT7) // 3D_Present absent, no 3D information
                {
                    ucIndex = MHL_VIC_PARSING_FINISH;
                }
                else
                {
                    b3DPresent = TRUE;
                    uc3DMultiPresent = (ucValue &MASKBIT(6:5)) >> 5;

                    if(uc3DMultiPresent == 0)
                    {
                        ucIndex = MHL_VIC_PARSING_3D_MULTI_PRESENT_00;
                    }
                    else if(uc3DMultiPresent == 3) // Reserved for future use
                    {
                        ucIndex = MHL_VIC_PARSING_FINISH;
                    }
                    else
                    {
                        ucIndex = MHL_VIC_PARSING_CHECK_HDMI_VIC;

                        ucVSDBAddr++;
                    }
                }

                break;

            case MHL_VIC_PARSING_CHECK_HDMI_VIC:
                ucLength = (ucValue &MASKBIT(7:5)) >> 5;

                ucVSDBAddr = ucVSDBAddr +ucLength +1;

                if(uc3DMultiPresent == 1)
                {
                    ucIndex = MHL_VIC_PARSING_3D_MULTI_PRESENT_01;
                }
                else // uc3DMultiPresent = 2
                {
                    ucIndex = MHL_VIC_PARSING_3D_MULTI_PRESENT_10;
                }

                ucLength = ucVSDBAddr +(ucValue &MASKBIT(4:0)) -1; // HDMI_3D_LEN

                break;

            case MHL_VIC_PARSING_3D_MULTI_PRESENT_00:
                ucIndex = MHL_VIC_PARSING_CHECK_3D_PRESENT_TIMING;

                break;

            case MHL_VIC_PARSING_3D_MULTI_PRESENT_01:
                ucValue = _mdrv_mhl_CheckVIC3DStructureAll((pData[ucVSDBAddr +1] |(pData[ucVSDBAddr] << 8)));

                for(uctemp = 0; uctemp <ucVICTimingNum; uctemp++)
                {
                    ucMHL3DInformationVIC[uctemp] = ucValue;
                }

                ucIndex = MHL_VIC_PARSING_CHECK_3D_PRESENT_TIMING;

                break;

            case MHL_VIC_PARSING_3D_MULTI_PRESENT_10:
                ucValue = _mdrv_mhl_CheckVIC3DStructureAll((pData[ucVSDBAddr +1] |(pData[ucVSDBAddr] << 8)));

                usMaskValue = pData[ucVSDBAddr +3] |(pData[ucVSDBAddr +2] << 8);

                for(uctemp = 0; uctemp <ucVICTimingNum; uctemp++)
                {
                    if((usMaskValue &_BIT(uctemp)) == _BIT(uctemp))
                    {
                        ucMHL3DInformationVIC[uctemp] = ucValue;
                    }
                }

                ucVSDBAddr = ucVSDBAddr +4;

                ucIndex = MHL_VIC_PARSING_CHECK_3D_STRUCTURE;

                break;

            case MHL_VIC_PARSING_CHECK_3D_STRUCTURE:
                if(ucVSDBAddr > ucLength) // No 3D sturcture
                {
                    ucIndex = MHL_VIC_PARSING_CHECK_3D_PRESENT_TIMING;
                }
                else
                {
                    uctemp = (ucValue &MASKBIT(7:4)) >> 4;
                    ucValue = ucValue &MASKBIT(3:0);

                    ucMHL3DInformationVIC[uctemp] = _mdrv_mhl_CheckVIC3DStructure(ucValue);

                    if(ucValue < 8) // Only 2D_VIC_Order and 3D_Structure, 1 byte
                    {
                        ucVSDBAddr++;
                    }
                    else // Add 3D_Detail, 2 byte
                    {
                        ucVSDBAddr = ucVSDBAddr +2;
                    }
                }

                break;

            case MHL_VIC_PARSING_CHECK_3D_PRESENT_TIMING:
                for(uctemp = 0; uctemp <ucVICTimingNum; uctemp++)
                {
                    ucValue = ucMHL3DInformationVIC[uctemp];

                    switch(ucVICTiming[uctemp])
                    {
                        case MHL_3D_PRESENT_VIC_1920x1080p24Hz: // Support frame packing with top and bottom
                        case MHL_3D_PRESENT_VIC_1280x720p60Hz: // Support frame packing with top and bottom
                        case MHL_3D_PRESENT_VIC_1280x720p50Hz: // Support frame packing with top and bottom
                            ucValue = MASKSET(ucValue, (MHL_3D_FS_SUPPORT |MHL_3D_TB_SUPPORT), (MHL_3D_FS_SUPPORT |MHL_3D_TB_SUPPORT));
                            ucMHL3DInformationVIC[uctemp] = ucValue;

                            break;

                        case MHL_3D_PRESENT_VIC_1920x1080i60Hz: // Support side by side
                        case MHL_3D_PRESENT_VIC_1920x1080i50Hz: // Support side by side
                            ucValue = MASKSET(ucValue, MHL_3D_LR_SUPPORT, MHL_3D_LR_SUPPORT);
                            ucMHL3DInformationVIC[uctemp] = ucValue;

                            break;

                        default:

                            break;
                    };
                }

                ucIndex = MHL_VIC_PARSING_FINISH;

                break;

            case MHL_VIC_PARSING_FINISH:
                bVICParseFinish = TRUE;

                break;

            default:

                break;
        };
    }

    for(uctemp = 0, ucValue = 0; uctemp < MHL_3D_VIC_INFORMATION_LENGTH; uctemp++)
    {
        if(ucMHL3DInformationVIC[uctemp] > 0)
        {
            ucValue = uctemp +1;
        }
    }

    ucMHL3DTotalEntryVIC = ucValue;

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            SET_MHL_3D_TOT_ENT_VIC_PORT0(ucMHL3DTotalEntryVIC);

            for(uctemp = 0; uctemp < MHL_3D_VIC_INFORMATION_LENGTH; uctemp++)
            {
                SET_MHL_3D_VIC_INFORMATION0(uctemp, ucMHL3DInformationVIC[uctemp]);
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            SET_MHL_3D_TOT_ENT_VIC_PORT1(ucMHL3DTotalEntryVIC);

            for(uctemp = 0; uctemp < MHL_3D_VIC_INFORMATION_LENGTH; uctemp++)
            {
                SET_MHL_3D_VIC_INFORMATION1(uctemp, ucMHL3DInformationVIC[uctemp]);
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            SET_MHL_3D_TOT_ENT_VIC_PORT2(ucMHL3DTotalEntryVIC);

            for(uctemp = 0; uctemp < MHL_3D_VIC_INFORMATION_LENGTH; uctemp++)
            {
                SET_MHL_3D_VIC_INFORMATION2(uctemp, ucMHL3DInformationVIC[uctemp]);
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            SET_MHL_3D_TOT_ENT_VIC_PORT3(ucMHL3DTotalEntryVIC);

            for(uctemp = 0; uctemp < MHL_3D_VIC_INFORMATION_LENGTH; uctemp++)
            {
                SET_MHL_3D_VIC_INFORMATION3(uctemp, ucMHL3DInformationVIC[uctemp]);
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            SET_MHL_3D_TOT_ENT_VIC_PORT4(ucMHL3DTotalEntryVIC);

            for(uctemp = 0; uctemp < MHL_3D_VIC_INFORMATION_LENGTH; uctemp++)
            {
                SET_MHL_3D_VIC_INFORMATION4(uctemp, ucMHL3DInformationVIC[uctemp]);
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            SET_MHL_3D_TOT_ENT_VIC_PORT5(ucMHL3DTotalEntryVIC);

            for(uctemp = 0; uctemp < MHL_3D_VIC_INFORMATION_LENGTH; uctemp++)
            {
                SET_MHL_3D_VIC_INFORMATION5(uctemp, ucMHL3DInformationVIC[uctemp]);
            }

            break;
#endif

        default:

            break;
    };

    return b3DPresent;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_ParsingEDIDfor3D()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_ParsingEDIDfor3D(BYTE ucPortSelect, BYTE *pComboEDID)
{
    Bool bDTDParseFinish = FALSE;
    BYTE uctemp = 0;
    BYTE ucValue = 0;
    BYTE ucIndex = 0;
    BYTE ucDTDTimingNum = 0;
    BYTE ucThirdDTDStart = 0;
    BYTE ucMHL3DTotalEntryDTD = 0;
    BYTE ucDTDTiming[MHL_3D_DTD_INFORMATION_LENGTH];
    BYTE ucMHL3DInformationDTD[MHL_3D_DTD_INFORMATION_LENGTH];

    if(pComboEDID != NULL)
    {
        for(uctemp = 0; uctemp < MHL_3D_DTD_INFORMATION_LENGTH; uctemp++)
        {
            ucDTDTiming[uctemp] = 0;
            ucMHL3DInformationDTD[uctemp] = 0;
        }

        // Parsing 3D DTD information Start
        if(_mdrv_mhl_DTDPresentinEDID(&pComboEDID[MHL_EDID_FIRST_DTD_START])) // Check first DTD
        {
            ucValue = _mdrv_mhl_InsertDTD3DInformation(MHL_3D_FIRST_DTD, pComboEDID[MHL_EDID_FIRST_DTD_START +MHL_EDID_DTD_LENGTH -1], ucMHL3DInformationDTD);

            ucDTDTiming[MHL_3D_FIRST_DTD] = _mdrv_mhl_GetDTD3DTimingInformation(&pComboEDID[MHL_EDID_FIRST_DTD_START]);
        }
        else // First DTD absent
        {
            bDTDParseFinish = TRUE;

            ucDTDTimingNum = 0;

            MHL_DRV_DPRINTF("** MHL first DTD absent in port %c", ucPortSelect);
        }

        if(!bDTDParseFinish)
        {
            if(_mdrv_mhl_DTDPresentinEDID(&pComboEDID[MHL_EDID_SECOND_DTD_START])) // Check second DTD
            {
                ucValue = _mdrv_mhl_InsertDTD3DInformation(MHL_3D_SECOND_DTD, pComboEDID[MHL_EDID_SECOND_DTD_START +MHL_EDID_DTD_LENGTH -1], ucMHL3DInformationDTD);

                ucDTDTiming[MHL_3D_SECOND_DTD] = _mdrv_mhl_GetDTD3DTimingInformation(&pComboEDID[MHL_EDID_SECOND_DTD_START]);
            }
            else    // Second DTD absent
            {
                bDTDParseFinish = TRUE;

                ucDTDTimingNum = 1;

               MHL_DRV_DPRINTF("** MHL second DTD absent in port %c", ucPortSelect);
            }
        }

        if(pComboEDID[MHL_EDID_EXTENSION_FLAG] > 0) // Have extend EDID
        {
            ucThirdDTDStart = MHL_EDID_BLOCK_LENGTH +pComboEDID[MHL_EDID_RESERVED_DATA_BLOCK];

            MHL_DRV_DPRINTF("** MHL third DTD start at %x", ucThirdDTDStart);

            if(!bDTDParseFinish)
            {
                for(uctemp = 0; uctemp <(MHL_3D_DTD_INFORMATION_LENGTH -2); uctemp++)
                {
                    if(_mdrv_mhl_DTDPresentinEDID(&pComboEDID[ucThirdDTDStart +MHL_EDID_DTD_LENGTH *uctemp])) // Check others DTD
                    {
                        ucValue = _mdrv_mhl_InsertDTD3DInformation(MHL_3D_THIRD_DTD +uctemp, pComboEDID[ucThirdDTDStart +MHL_EDID_DTD_LENGTH *(uctemp +1) -1], ucMHL3DInformationDTD);

                        ucDTDTiming[MHL_3D_THIRD_DTD +uctemp] = _mdrv_mhl_GetDTD3DTimingInformation(&pComboEDID[ucThirdDTDStart +MHL_EDID_DTD_LENGTH *uctemp]);
                    }
                    else    // Last DTD find
                    {
                        bDTDParseFinish = TRUE;

                        ucDTDTimingNum = uctemp +2;

                        MHL_DRV_DPRINTF("** MHL total have %d DTD", ucDTDTimingNum);

                        break;
                    }
                }
            }

            // Parsing 3D VIC information Start
            if(ucThirdDTDStart > (MHL_EDID_RESERVED_DATA_BLOCK +2))
            {
                if(_mdrv_mhl_ParsingVIC3DInformation(ucPortSelect, pComboEDID[MHL_EDID_RESERVED_DATA_BLOCK] -4, &pComboEDID[MHL_EDID_RESERVED_DATA_BLOCK +2]))
                {
                    // 3D Present
                    for(uctemp = 0; uctemp <ucDTDTimingNum; uctemp++)
                    {
                        ucIndex = ucMHL3DInformationDTD[uctemp];

                        switch(ucDTDTiming[uctemp])
                        {
                            case MHL_3D_PRESENT_VIC_1920x1080p24Hz: // Support frame packing with top and bottom
                            case MHL_3D_PRESENT_VIC_1280x720p60Hz: // Support frame packing with top and bottom
                            case MHL_3D_PRESENT_VIC_1280x720p50Hz: // Support frame packing with top and bottom
                                ucIndex = MASKSET(ucIndex, (MHL_3D_FS_SUPPORT |MHL_3D_TB_SUPPORT), (MHL_3D_FS_SUPPORT |MHL_3D_TB_SUPPORT));
                                ucMHL3DInformationDTD[uctemp] = ucIndex;

                                break;

                            case MHL_3D_PRESENT_VIC_1920x1080i60Hz: // Support side by side
                            case MHL_3D_PRESENT_VIC_1920x1080i50Hz: // Support side by side
                                ucIndex = MASKSET(ucIndex, MHL_3D_LR_SUPPORT, MHL_3D_LR_SUPPORT);
                                ucMHL3DInformationDTD[uctemp] = ucIndex;

                                break;

                            default:

                                break;
                        };

                        if(ucIndex > 0)
                        {
                            ucValue = uctemp +1;
                        }
                    }
                }
            }
        }

        // Parsing 3D DTD information End
        ucMHL3DTotalEntryDTD = ucValue;

        switch(ucPortSelect)
        {
#if(MHL_FUNCTION_SUPPORT_PORT0)
            case MHL_INPUT_SELECT_PORT0:
                SET_MHL_3D_TOT_ENT_DTD_PORT0(ucMHL3DTotalEntryDTD);

                for(uctemp = 0; uctemp < MHL_3D_DTD_INFORMATION_LENGTH; uctemp++)
                {
                    SET_MHL_3D_DTD_INFORMATION0(uctemp, ucMHL3DInformationDTD[uctemp]);
                }

                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
            case MHL_INPUT_SELECT_PORT1:
                SET_MHL_3D_TOT_ENT_DTD_PORT1(ucMHL3DTotalEntryDTD);

                for(uctemp = 0; uctemp < MHL_3D_DTD_INFORMATION_LENGTH; uctemp++)
                {
                    SET_MHL_3D_DTD_INFORMATION1(uctemp, ucMHL3DInformationDTD[uctemp]);
                }

                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
            case MHL_INPUT_SELECT_PORT2:
                SET_MHL_3D_TOT_ENT_DTD_PORT2(ucMHL3DTotalEntryDTD);

                for(uctemp = 0; uctemp < MHL_3D_DTD_INFORMATION_LENGTH; uctemp++)
                {
                    SET_MHL_3D_DTD_INFORMATION2(uctemp, ucMHL3DInformationDTD[uctemp]);
                }

                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
            case MHL_INPUT_SELECT_PORT3:
                SET_MHL_3D_TOT_ENT_DTD_PORT3(ucMHL3DTotalEntryDTD);

                for(uctemp = 0; uctemp < MHL_3D_DTD_INFORMATION_LENGTH; uctemp++)
                {
                    SET_MHL_3D_DTD_INFORMATION3(uctemp, ucMHL3DInformationDTD[uctemp]);
                }

                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
            case MHL_INPUT_SELECT_PORT4:
                SET_MHL_3D_TOT_ENT_DTD_PORT4(ucMHL3DTotalEntryDTD);

                for(uctemp = 0; uctemp < MHL_3D_DTD_INFORMATION_LENGTH; uctemp++)
                {
                    SET_MHL_3D_DTD_INFORMATION4(uctemp, ucMHL3DInformationDTD[uctemp]);
                }

                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
            case MHL_INPUT_SELECT_PORT5:
                SET_MHL_3D_TOT_ENT_DTD_PORT5(ucMHL3DTotalEntryDTD);

                for(uctemp = 0; uctemp < MHL_3D_DTD_INFORMATION_LENGTH; uctemp++)
                {
                    SET_MHL_3D_DTD_INFORMATION5(uctemp, ucMHL3DInformationDTD[uctemp]);
                }

                break;
#endif

            default:

                break;
        };
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_RtermControlHWMode()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_RtermControlHWMode(BYTE ucPortSelect, Bool bRtermHW)
{
    RXRtermControl_T ucRtermState = RX_RTERM_OFF;

    if(!bRtermHW) // Disable Rterm control HW mode must check SW Rterm control
    {
        if(mhal_mhl_CbusConnectStatus(ucPortSelect))
        {
            ucRtermState = RX_MHL_RTERM;
        }

        mhal_mhl_ComboRtermControl(ucPortSelect, ucRtermState);
    }

    mhal_mhl_RtermControlHWMode(ucPortSelect, bRtermHW);
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusRHIDRequestHost()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_CbusRHIDRequestHost(BYTE ucPortSelect)
{
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            SET_MHL_RHID_PROCESS_STATE_PORT0(MHL_CBUS_RHID_SOURCE_HOST);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            SET_MHL_RHID_PROCESS_STATE_PORT1(MHL_CBUS_RHID_SOURCE_HOST);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            SET_MHL_RHID_PROCESS_STATE_PORT2(MHL_CBUS_RHID_SOURCE_HOST);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            SET_MHL_RHID_PROCESS_STATE_PORT3(MHL_CBUS_RHID_SOURCE_HOST);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            SET_MHL_RHID_PROCESS_STATE_PORT4(MHL_CBUS_RHID_SOURCE_HOST);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            SET_MHL_RHID_PROCESS_STATE_PORT5(MHL_CBUS_RHID_SOURCE_HOST);
            break;
#endif

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusRHIDRelinquishHost()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_CbusRHIDRelinquishHost(BYTE ucPortSelect)
{
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            if(GET_MHL_RHID_PROCESS_STATE_PORT0() == MHL_CBUS_RHID_SOURCE_HOST)
            {
                SET_MHL_RHID_PROCESS_STATE_PORT0(MHL_CBUS_RHID_RELINQUISH);
            }
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            if(GET_MHL_RHID_PROCESS_STATE_PORT1() == MHL_CBUS_RHID_SOURCE_HOST)
            {
                SET_MHL_RHID_PROCESS_STATE_PORT1(MHL_CBUS_RHID_RELINQUISH);
            }
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            if(GET_MHL_RHID_PROCESS_STATE_PORT2() == MHL_CBUS_RHID_SOURCE_HOST)
            {
                SET_MHL_RHID_PROCESS_STATE_PORT2(MHL_CBUS_RHID_RELINQUISH);
            }
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            if(GET_MHL_RHID_PROCESS_STATE_PORT3() == MHL_CBUS_RHID_SOURCE_HOST)
            {
                SET_MHL_RHID_PROCESS_STATE_PORT3(MHL_CBUS_RHID_RELINQUISH);
            }
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            if(GET_MHL_RHID_PROCESS_STATE_PORT4() == MHL_CBUS_RHID_SOURCE_HOST)
            {
                SET_MHL_RHID_PROCESS_STATE_PORT4(MHL_CBUS_RHID_RELINQUISH);
            }
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            if(GET_MHL_RHID_PROCESS_STATE_PORT5() == MHL_CBUS_RHID_SOURCE_HOST)
            {
                SET_MHL_RHID_PROCESS_STATE_PORT5(MHL_CBUS_RHID_RELINQUISH);
            }
            break;
#endif

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusRHIDAcceptHost()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_CbusRHIDAcceptHost(BYTE ucPortSelect, Bool bAcceptHost)
{
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            if(GET_MHL_RHID_PROCESS_STATE_PORT0() == MHL_CBUS_RHID_REQUEST_HOST)
            {
                if(bAcceptHost)
                {
                    SET_MHL_RHID_PROCESS_STATE_PORT0(MHL_CBUS_RHID_SINK_HOST);
                }
                else
                {
                    SET_MHL_RHID_PROCESS_STATE_PORT0(MHL_CBUS_RHID_END);
                }
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            if(GET_MHL_RHID_PROCESS_STATE_PORT1() == MHL_CBUS_RHID_REQUEST_HOST)
            {
                if(bAcceptHost)
                {
                    SET_MHL_RHID_PROCESS_STATE_PORT1(MHL_CBUS_RHID_SINK_HOST);
                }
                else
                {
                    SET_MHL_RHID_PROCESS_STATE_PORT1(MHL_CBUS_RHID_END);
                }
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            if(GET_MHL_RHID_PROCESS_STATE_PORT2() == MHL_CBUS_RHID_REQUEST_HOST)
            {
                if(bAcceptHost)
                {
                    SET_MHL_RHID_PROCESS_STATE_PORT2(MHL_CBUS_RHID_SINK_HOST);
                }
                else
                {
                    SET_MHL_RHID_PROCESS_STATE_PORT2(MHL_CBUS_RHID_END);
                }
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            if(GET_MHL_RHID_PROCESS_STATE_PORT3() == MHL_CBUS_RHID_REQUEST_HOST)
            {
                if(bAcceptHost)
                {
                    SET_MHL_RHID_PROCESS_STATE_PORT3(MHL_CBUS_RHID_SINK_HOST);
                }
                else
                {
                    SET_MHL_RHID_PROCESS_STATE_PORT3(MHL_CBUS_RHID_END);
                }
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            if(GET_MHL_RHID_PROCESS_STATE_PORT4() == MHL_CBUS_RHID_REQUEST_HOST)
            {
                if(bAcceptHost)
                {
                    SET_MHL_RHID_PROCESS_STATE_PORT4(MHL_CBUS_RHID_SINK_HOST);
                }
                else
                {
                    SET_MHL_RHID_PROCESS_STATE_PORT4(MHL_CBUS_RHID_END);
                }
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            if(GET_MHL_RHID_PROCESS_STATE_PORT5() == MHL_CBUS_RHID_REQUEST_HOST)
            {
                if(bAcceptHost)
                {
                    SET_MHL_RHID_PROCESS_STATE_PORT5(MHL_CBUS_RHID_SINK_HOST);
                }
                else
                {
                    SET_MHL_RHID_PROCESS_STATE_PORT5(MHL_CBUS_RHID_END);
                }
            }

            break;
#endif

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusRHIDEventProc()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE _mdrv_mhl_CbusRHIDEventProc(BYTE ucKeyCode)
{
    BYTE ucStatus = MHL_RHID_NO_ERROR;
    BYTE ucPortSelect = GET_MHL_CBUS_MSC_MSG_PROC_PORT();

    switch(ucKeyCode)
    {
        case MHL_RHID_REQUEST_HOST:
            _mdrv_mhl_CbusRHIDRequestHost(ucPortSelect);
            break;

        case MHL_RHID_RELINQUISH_HOST:
            _mdrv_mhl_CbusRHIDRelinquishHost(ucPortSelect);
            break;

        default:
            ucStatus = MHL_RHID_INVALID_CODE;
            break;
    };

    MHL_DRV_DPRINTF("** MHL receive RHID key code %x", ucKeyCode);

    return ucStatus;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusBISTTriggerProc()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_CbusBISTTriggerProc(BYTE ucKeyCode)
{
    Bool bGeteCbusDuration = FALSE;
    Bool bGetAVLinkDuration = FALSE;
    BYTE ucPortSelect = GET_MHL_CBUS_MSC_MSG_PROC_PORT();
    BYTE ucBISTDeviceType = tMHL_DEVCAP_TABLE_PORT[ucPortSelect][MHL_CBUS_DEV_CAT] %MASKBIT(3:0);
    BYTE ucBISTeCbusSelect = tMHL_DEVCAP_TABLE_PORT[ucPortSelect][MHL_CBUS_ECBUS_SPEEDS];
    WORD usTimerCount = 0;

    if(((ucKeyCode &BIT3) == BIT3) && ((ucBISTeCbusSelect &BIT4) != BIT4)) // Not support eCbus-D
    {
        // Chip only support eCbus-S
    }

    if((ucBISTDeviceType == MHL_DEVICE_SOURCE) || (ucBISTDeviceType == MHL_DEVICE_DIRECT_ATTACH_SOURCE))
    {
        if((ucKeyCode &BIT0) == BIT0) // Source device and eCbus Tx
        {

        }
        else if((ucKeyCode &BIT4) == BIT4) // Source device and AV link Tx
        {

        }
    }
    else if((ucBISTDeviceType == MHL_DEVICE_SINK) || (ucBISTDeviceType == MHL_DEVICE_DIRECT_ATTACH_SINK))
    {
        if((ucKeyCode &BIT1) == BIT1) // Sink device and eCbus link Rx
        {
            bGeteCbusDuration = TRUE;
        }
        else if((ucKeyCode &BIT5) == BIT5) // Sink device and AV link Rx
        {
            bGetAVLinkDuration = TRUE;
        }
    }

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            if(GET_MHL_BIST_PROCESS_STATE_PORT0() == MHL_CBUS_BIST_WAIT_TRIGGER)
            {
                if(bGeteCbusDuration)
                {
                    usTimerCount = GET_MHL_BIST_ECBUS_DURATION0();
                    CLR_MHL_BIST_TRIGGER_AV_LINK0();
                }
                else if(bGetAVLinkDuration)
                {
                    usTimerCount = GET_MHL_BIST_AV_LINK_DURATION0();
                    SET_MHL_BIST_TRIGGER_AV_LINK0();
                }

                if(bGeteCbusDuration || bGetAVLinkDuration)
                {
                    SET_MHL_BIST_PROCESS_STATE_PORT0(MHL_CBUS_BIST_RECEIVE_TRIGGER);
                }
            }
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            if(GET_MHL_BIST_PROCESS_STATE_PORT1() == MHL_CBUS_BIST_WAIT_TRIGGER)
            {
                if(bGeteCbusDuration)
                {
                    usTimerCount = GET_MHL_BIST_ECBUS_DURATION1();
                    CLR_MHL_BIST_TRIGGER_AV_LINK1();
                }
                else if(bGetAVLinkDuration)
                {
                    usTimerCount = GET_MHL_BIST_AV_LINK_DURATION1();
                    SET_MHL_BIST_TRIGGER_AV_LINK1();
                }

                if(bGeteCbusDuration || bGetAVLinkDuration)
                {
                    SET_MHL_BIST_PROCESS_STATE_PORT1(MHL_CBUS_BIST_RECEIVE_TRIGGER);
                }
            }
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            if(GET_MHL_BIST_PROCESS_STATE_PORT2() == MHL_CBUS_BIST_WAIT_TRIGGER)
            {
                if(bGeteCbusDuration)
                {
                    usTimerCount = GET_MHL_BIST_ECBUS_DURATION2();
                    CLR_MHL_BIST_TRIGGER_AV_LINK2();
                }
                else if(bGetAVLinkDuration)
                {
                    usTimerCount = GET_MHL_BIST_AV_LINK_DURATION2();
                    SET_MHL_BIST_TRIGGER_AV_LINK2();
                }

                if(bGeteCbusDuration || bGetAVLinkDuration)
                {
                    SET_MHL_BIST_PROCESS_STATE_PORT2(MHL_CBUS_BIST_RECEIVE_TRIGGER);
                }
            }
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            if(GET_MHL_BIST_PROCESS_STATE_PORT3() == MHL_CBUS_BIST_WAIT_TRIGGER)
            {
                if(bGeteCbusDuration)
                {
                    usTimerCount = GET_MHL_BIST_ECBUS_DURATION3();
                    CLR_MHL_BIST_TRIGGER_AV_LINK3();
                }
                else if(bGetAVLinkDuration)
                {
                    usTimerCount = GET_MHL_BIST_AV_LINK_DURATION3();
                    SET_MHL_BIST_TRIGGER_AV_LINK3();
                }

                if(bGeteCbusDuration || bGetAVLinkDuration)
                {
                    SET_MHL_BIST_PROCESS_STATE_PORT3(MHL_CBUS_BIST_RECEIVE_TRIGGER);
                }
            }
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            if(GET_MHL_BIST_PROCESS_STATE_PORT4() == MHL_CBUS_BIST_WAIT_TRIGGER)
            {
                if(bGeteCbusDuration)
                {
                    usTimerCount = GET_MHL_BIST_ECBUS_DURATION4();
                    CLR_MHL_BIST_TRIGGER_AV_LINK4();
                }
                else if(bGetAVLinkDuration)
                {
                    usTimerCount = GET_MHL_BIST_AV_LINK_DURATION4();
                    SET_MHL_BIST_TRIGGER_AV_LINK4();
                }

                if(bGeteCbusDuration || bGetAVLinkDuration)
                {
                    SET_MHL_BIST_PROCESS_STATE_PORT4(MHL_CBUS_BIST_RECEIVE_TRIGGER);
                }
            }
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            if(GET_MHL_BIST_PROCESS_STATE_PORT5() == MHL_CBUS_BIST_WAIT_TRIGGER)
            {
                if(bGeteCbusDuration)
                {
                    usTimerCount = GET_MHL_BIST_ECBUS_DURATION5();
                    CLR_MHL_BIST_TRIGGER_AV_LINK5();
                }
                else if(bGetAVLinkDuration)
                {
                    usTimerCount = GET_MHL_BIST_AV_LINK_DURATION5();
                    SET_MHL_BIST_TRIGGER_AV_LINK5();
                }

                if(bGeteCbusDuration || bGetAVLinkDuration)
                {
                    SET_MHL_BIST_PROCESS_STATE_PORT5(MHL_CBUS_BIST_RECEIVE_TRIGGER);
                }
            }
            break;
#endif

        default:

            break;
    };

    if(usTimerCount > 0)
    {
        _mdrv_mhl_SetBISTProcTimer(ucPortSelect, usTimerCount);
    }

    MHL_DRV_DPRINTF("** MHL receive BIST trigger key code %x", ucKeyCode);
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusBISTRequestStatusProc()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_CbusBISTRequestStatusProc(BYTE ucKeyCode)
{
    BYTE ucPortSelect = GET_MHL_CBUS_MSC_MSG_PROC_PORT();

    if(ucKeyCode == MHL_BIST_REQ_RETURN_STATUS)
    {
        switch(ucPortSelect)
        {
#if(MHL_FUNCTION_SUPPORT_PORT0)
            case MHL_INPUT_SELECT_PORT0:
                SET_MHL_BIST_PROCESS_STATE_PORT0(MHL_CBUS_BIST_REQUEST_STATUS);
                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
            case MHL_INPUT_SELECT_PORT1:
                SET_MHL_BIST_PROCESS_STATE_PORT1(MHL_CBUS_BIST_REQUEST_STATUS);
                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
            case MHL_INPUT_SELECT_PORT2:
                SET_MHL_BIST_PROCESS_STATE_PORT2(MHL_CBUS_BIST_REQUEST_STATUS);
                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
            case MHL_INPUT_SELECT_PORT3:
                SET_MHL_BIST_PROCESS_STATE_PORT3(MHL_CBUS_BIST_REQUEST_STATUS);
                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
            case MHL_INPUT_SELECT_PORT4:
                SET_MHL_BIST_PROCESS_STATE_PORT4(MHL_CBUS_BIST_REQUEST_STATUS);
                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
            case MHL_INPUT_SELECT_PORT5:
                SET_MHL_BIST_PROCESS_STATE_PORT5(MHL_CBUS_BIST_REQUEST_STATUS);
                break;
#endif

            default:

                break;
        };
    }

    MHL_DRV_DPRINTF("** MHL receive BIST request status key code %x", ucKeyCode);
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusBISTStopProc()
//  [Description]
//                  Disable AV LINK_BIST when currently running
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_CbusBISTStopProc(BYTE ucKeyCode)
{
    BYTE ucPortSelect = GET_MHL_CBUS_MSC_MSG_PROC_PORT();

    if(ucKeyCode == 0) // Key code must be set to 0
    {
        switch(ucPortSelect)
        {
#if(MHL_FUNCTION_SUPPORT_PORT0)
            case MHL_INPUT_SELECT_PORT0:
                if(GET_MHL_BIST_PROCESS_STATE_PORT0() == MHL_CBUS_BIST_ECBUS_SWITCH)
                {
                    SET_MHL_BIST_PROCESS_STATE_PORT0(MHL_CBUS_BIST_EVENT_FINISH);
                }
                
                break;
#endif
    
#if(MHL_FUNCTION_SUPPORT_PORT1)
            case MHL_INPUT_SELECT_PORT1:
                if(GET_MHL_BIST_PROCESS_STATE_PORT1() == MHL_CBUS_BIST_ECBUS_SWITCH)
                {
                    SET_MHL_BIST_PROCESS_STATE_PORT1(MHL_CBUS_BIST_EVENT_FINISH);
                }
                
                break;
#endif
    
#if(MHL_FUNCTION_SUPPORT_PORT2)
            case MHL_INPUT_SELECT_PORT2:
                if(GET_MHL_BIST_PROCESS_STATE_PORT2() == MHL_CBUS_BIST_ECBUS_SWITCH)
                {
                    SET_MHL_BIST_PROCESS_STATE_PORT2(MHL_CBUS_BIST_EVENT_FINISH);
                }
                
                break;
#endif
    
#if(MHL_FUNCTION_SUPPORT_PORT3)
            case MHL_INPUT_SELECT_PORT3:
                if(GET_MHL_BIST_PROCESS_STATE_PORT3() == MHL_CBUS_BIST_ECBUS_SWITCH)
                {
                    SET_MHL_BIST_PROCESS_STATE_PORT3(MHL_CBUS_BIST_EVENT_FINISH);
                }
                
                break;
#endif
    
#if(MHL_FUNCTION_SUPPORT_PORT4)
            case MHL_INPUT_SELECT_PORT4:
                if(GET_MHL_BIST_PROCESS_STATE_PORT4() == MHL_CBUS_BIST_ECBUS_SWITCH)
                {
                    SET_MHL_BIST_PROCESS_STATE_PORT4(MHL_CBUS_BIST_EVENT_FINISH);
                }
                
                break;
#endif
    
#if(MHL_FUNCTION_SUPPORT_PORT5)
            case MHL_INPUT_SELECT_PORT5:
                if(GET_MHL_BIST_PROCESS_STATE_PORT5() == MHL_CBUS_BIST_ECBUS_SWITCH)
                {
                    SET_MHL_BIST_PROCESS_STATE_PORT5(MHL_CBUS_BIST_EVENT_FINISH);
                }
                
                break;
#endif
    
            default:
    
                break;
        };
    }

    MHL_DRV_DPRINTF("** MHL receive BIST stop key code %x", ucKeyCode);
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_SetCbusTxQueueFreeze()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_SetCbusTxQueueFreeze(BYTE ucPortSelect, Bool bTxFreeze)
{
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            if(bTxFreeze)
            {
                SET_MHL_CBUS_TX_QUEUE_FREEZE0();
            }
            else
            {
                CLR_MHL_CBUS_TX_QUEUE_FREEZE0();
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            if(bTxFreeze)
            {
                SET_MHL_CBUS_TX_QUEUE_FREEZE1();
            }
            else
            {
                CLR_MHL_CBUS_TX_QUEUE_FREEZE1();
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            if(bTxFreeze)
            {
                SET_MHL_CBUS_TX_QUEUE_FREEZE2();
            }
            else
            {
                CLR_MHL_CBUS_TX_QUEUE_FREEZE2();
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            if(bTxFreeze)
            {
                SET_MHL_CBUS_TX_QUEUE_FREEZE3();
            }
            else
            {
                CLR_MHL_CBUS_TX_QUEUE_FREEZE3();
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            if(bTxFreeze)
            {
                SET_MHL_CBUS_TX_QUEUE_FREEZE4();
            }
            else
            {
                CLR_MHL_CBUS_TX_QUEUE_FREEZE4();
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            if(bTxFreeze)
            {
                SET_MHL_CBUS_TX_QUEUE_FREEZE5();
            }
            else
            {
                CLR_MHL_CBUS_TX_QUEUE_FREEZE5();
            }
            
            break;
#endif

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_GetEMSCRxBufferSize()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
WORD _mdrv_mhl_GetEMSCRxBufferSize(BYTE ucPortSelect)
{
    WORD usEMSCRxBufferSize = 0;
    
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            usEMSCRxBufferSize = GET_MHL_EMSC_RX_BUFFER_SIZE0();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            usEMSCRxBufferSize = GET_MHL_EMSC_RX_BUFFER_SIZE1();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            usEMSCRxBufferSize = GET_MHL_EMSC_RX_BUFFER_SIZE2();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            usEMSCRxBufferSize = GET_MHL_EMSC_RX_BUFFER_SIZE3();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            usEMSCRxBufferSize = GET_MHL_EMSC_RX_BUFFER_SIZE4();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            usEMSCRxBufferSize = GET_MHL_EMSC_RX_BUFFER_SIZE5();
            break;
#endif

        default:

            break;
    };

    return usEMSCRxBufferSize;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_GetEMSCTxBufferSize()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
WORD _mdrv_mhl_GetEMSCTxBufferSize(BYTE ucPortSelect)
{
    WORD usEMSCTxBufferSize = 0;
    
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            usEMSCTxBufferSize = GET_MHL_EMSC_TX_BUFFER_SIZE0();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            usEMSCTxBufferSize = GET_MHL_EMSC_TX_BUFFER_SIZE1();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            usEMSCTxBufferSize = GET_MHL_EMSC_TX_BUFFER_SIZE2();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            usEMSCTxBufferSize = GET_MHL_EMSC_TX_BUFFER_SIZE3();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            usEMSCTxBufferSize = GET_MHL_EMSC_TX_BUFFER_SIZE4();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            usEMSCTxBufferSize = GET_MHL_EMSC_TX_BUFFER_SIZE5();
            break;
#endif

        default:

            break;
    };

    return usEMSCTxBufferSize;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CheckEMSCRxBufferSize()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_CheckEMSCRxBufferSize(BYTE ucPortSelect)
{
    BYTE ucSendStatus = 0;
    
    if(_mdrv_mhl_GetEMSCRxBufferSize(ucPortSelect) < MHL_EMSC_DATA_SIZE_THRESHOLD)
    {
        ucSendStatus = mdrv_mhl_SendEMSCDataString(ucPortSelect, 0, NULL);

        if(ucSendStatus == MHL_EMSC_SEND_PASS)
        {
            MHL_DRV_DPRINTF("** MHL eMSC send pass !! port %d", ucPortSelect);
        }
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_ECbusStateHandler()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_ECbusStateHandler(BYTE ucPortSelect)
{
    BYTE ucECbusState = _mdrv_mhl_GetECbusState(ucPortSelect);
    BYTE ucECbusEvent = MHL_ECBUS_EVENT_NONE;
    BYTE ucECbusStatusFlag = mhal_mhl_GetECbusStatusFlag(ucPortSelect);
    static WORD ucECbusResetCounter[6] = {0, 0, 0, 0, 0, 0};
    
    switch(ucECbusState)
    {
        case MHL_ECBUS_STATE_RECEIVE_MODE_UP:
            _mdrv_mhl_SetCbusTxQueueFreeze(ucPortSelect, TRUE);
            ucECbusResetCounter[ucPortSelect] = 0;
            ucECbusEvent = MHL_ECBUS_EVENT_PREPARE_MODE_UP;
            
            break;
            
        case MHL_ECBUS_STATE_TRIGGER_MODE_UP:
            _mdrv_mhl_SetECbusState(ucPortSelect, MHL_ECBUS_STATE_PROCESS_MODE_UP);
            ucECbusEvent = MHL_ECBUS_EVENT_MODE_UP_PROCESS;  
            
            break;
            
        case MHL_ECBUS_STATE_PROCESS_MODE_UP:
            if(ucECbusStatusFlag &MHL_ECBUS_STATUS_SLOT_SYNC_DONE)
            {
                _mdrv_mhl_SetCbusTxQueueFreeze(ucPortSelect, FALSE);
                _mdrv_mhl_SetECbusState(ucPortSelect, MHL_ECBUS_STATE_MODE_UP_CLOCK_LOCK);
                
                MHL_DRV_DPRINTF("** MHL Cbus1 command enable !! port %d", ucPortSelect);
            }

        case MHL_ECBUS_STATE_MODE_UP_CLOCK_LOCK:
            if(ucECbusStatusFlag &MHL_ECBUS_STATUS_EMSC_SYNC_DONE)
            {
                _mdrv_mhl_SetECbusState(ucPortSelect, MHL_ECBUS_STATE_STABLE_POLLING);
                
                MHL_DRV_DPRINTF("** MHL eMSC command enable !! port %d", ucPortSelect);
            }
            
        case MHL_ECBUS_STATE_STABLE_POLLING:
            if((ucECbusStatusFlag &(MHL_ECBUS_STATUS_TRAINING_PASS| MHL_ECBUS_STATUS_CLOCK_LOCK)) == MHL_ECBUS_STATUS_TRAINING_PASS)
            {
                _mdrv_mhl_SetCbusTxQueueFreeze(ucPortSelect, TRUE);
                _mdrv_mhl_SetECbusState(ucPortSelect, MHL_ECBUS_STATE_PROCESS_MODE_UP);
                ucECbusEvent = MHL_ECBUS_EVENT_UNLOCK_RESET;

                MHL_DRV_DPRINTF("** MHL uclock reset ECbus !! port %d", ucPortSelect);
            }
            else if((ucECbusStatusFlag &(MHL_ECBUS_STATUS_TRAINING_PASS| MHL_ECBUS_STATUS_CLOCK_LOCK)) == 0)
            {
                ucECbusResetCounter[ucPortSelect]++;
            }
            else if((ucECbusStatusFlag &(MHL_ECBUS_STATUS_TRAINING_PASS| MHL_ECBUS_STATUS_CLOCK_LOCK)) == MHL_ECBUS_STATUS_CLOCK_LOCK)
            {
                ucECbusResetCounter[ucPortSelect]++;
            }
            else
            {
                ucECbusResetCounter[ucPortSelect] = 0;
            }

            if((ucECbusResetCounter[ucPortSelect] == 1000) || (ucECbusStatusFlag &MHL_ECBUS_STATUS_RETRY_TIMEOUT))
            {
                _mdrv_mhl_SetECbusState(ucPortSelect, MHL_ECBUS_STATE_NONE);
                ucECbusEvent = MHL_ECBUS_EVENT_STUCK_TO_LOW;
            }
            
            break;

        case MHL_ECBUS_STATE_TRIGGER_MODE_DOWN:
            _mdrv_mhl_SetECbusState(ucPortSelect, MHL_ECBUS_STATE_PROCESS_MODE_DOWN);
            break;

        case MHL_ECBUS_STATE_PROCESS_MODE_DOWN:
            
            break;

        default:

            break;
    };

    mhal_mhl_ECbusEventProc(ucPortSelect, ucECbusEvent);

    if((ucECbusState >= MHL_ECBUS_STATE_RECEIVE_MODE_UP) && (ucECbusState < MHL_ECBUS_STATE_TRIGGER_MODE_UP))
    {
        ucECbusState++;
        
        _mdrv_mhl_SetECbusState(ucPortSelect, ucECbusState);
    }
    else if((ucECbusState >= MHL_ECBUS_STATE_RECEIVE_MODE_DOWN) && (ucECbusState < MHL_ECBUS_STATE_TRIGGER_MODE_DOWN))
    {
        ucECbusState++;
        
        _mdrv_mhl_SetECbusState(ucPortSelect, ucECbusState);
    }
}

#if(MHL_DEBUG_CHECK_CRC_VALUE)
//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CheckCRCValue()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_CheckCRCValue(void)
{
#if(MHL_CHECK_CRC_CHECK_TYPE == MHL_CHECK_CRC_CBUS1)

#elif(MHL_CHECK_CRC_CHECK_TYPE == MHL_CHECK_CRC_EMSC_ONE_BYTE)

#elif(MHL_CHECK_CRC_CHECK_TYPE == MHL_CHECK_CRC_EMSC_TWO_BYTE)
    BYTE uctemp = 0;
    BYTE uctempA = 0;
    BYTE uctempB = 0;
    BYTE ucAValue = 1;
    BYTE ucBValue = 1;
    BYTE ucCValue = 1;
    BYTE ucInputStr[13] = {0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1};

    for(uctemp = 0; uctemp < 13; uctemp++)
    {
        uctempA = ucAValue;
        uctempB = ucBValue;
        ucAValue = ucInputStr[uctemp] ^ucCValue;
        ucBValue = uctempA ^ucAValue;
        ucCValue = uctempB;
    }

    MHL_DRV_DPRINTF("** MHL CRC value A %d", !ucAValue);
    MHL_DRV_DPRINTF("** MHL CRC value B %d", !ucBValue);
    MHL_DRV_DPRINTF("** MHL CRC value C %d", !ucCValue);

    ucAValue = ucInputStr[12] ^ucInputStr[10] ^ucInputStr[9] ^ucInputStr[8] ^ucInputStr[5] ^ucInputStr[3] ^ucInputStr[2] ^ucInputStr[1];
    ucBValue = ucInputStr[12] ^ucInputStr[11] ^ucInputStr[10] ^ucInputStr[7] ^ucInputStr[5] ^ucInputStr[4] ^ucInputStr[3] ^ucInputStr[0] ^1;
    ucCValue = ucInputStr[11] ^ucInputStr[10] ^ucInputStr[9] ^ucInputStr[6] ^ucInputStr[4] ^ucInputStr[3] ^ucInputStr[2] ^1;
    
    MHL_DRV_DPRINTF("** MHL CRC value A %d", !ucAValue);
    MHL_DRV_DPRINTF("** MHL CRC value B %d", !ucBValue);
    MHL_DRV_DPRINTF("** MHL CRC value C %d", !ucCValue);

#elif(MHL_CHECK_CRC_CHECK_TYPE == MHL_CHECK_CRC_EMSC_PAYLOAD)
    BYTE uctemp = 0;
    BYTE uctempA = 0;
    BYTE ucAValue = 1;
    BYTE ucBValue = 0;
    BYTE ucCValue = 1;
    BYTE ucDValue = 1;
    BYTE ucEValue = 1;
    BYTE ucFValue = 1;
    BYTE ucGValue = 0;
    BYTE ucHValue = 1;
    BYTE ucIValue = 1;
    BYTE ucJValue = 1;
    BYTE ucKValue = 0;
    BYTE ucLValue = 0;
    BYTE ucMValue = 0;
    BYTE ucNValue = 0;
    BYTE ucOValue = 0;
    BYTE ucPValue = 0;
    BYTE ucInputStr[8] = {0, 0, 0, 0, 0, 0, 0, 0};

    for(uctemp = 0; uctemp < 8; uctemp++)
    {
        uctempA = ucInputStr[uctemp] ^ucPValue;
        ucPValue = uctempA ^ucOValue;
        ucOValue = ucNValue;
        ucNValue = ucMValue;
        ucMValue = ucLValue;
        ucLValue = ucKValue;
        ucKValue = ucJValue;
        ucJValue = ucIValue;
        ucIValue = ucHValue;
        ucHValue = ucGValue;
        ucGValue = ucFValue;
        ucFValue = ucEValue;
        ucEValue = ucDValue;
        ucDValue = ucCValue;
        ucCValue = uctempA ^ucBValue;
        ucBValue = ucAValue;
        ucAValue = uctempA;
    }

    MHL_DRV_DPRINTF("** MHL CRC value A %d", !ucAValue);
    MHL_DRV_DPRINTF("** MHL CRC value B %d", !ucBValue);
    MHL_DRV_DPRINTF("** MHL CRC value C %d", !ucCValue);
    MHL_DRV_DPRINTF("** MHL CRC value D %d", !ucDValue);
    MHL_DRV_DPRINTF("** MHL CRC value E %d", !ucEValue);
    MHL_DRV_DPRINTF("** MHL CRC value F %d", !ucFValue);
    MHL_DRV_DPRINTF("** MHL CRC value G %d", !ucGValue);
    MHL_DRV_DPRINTF("** MHL CRC value H %d", !ucHValue);
    MHL_DRV_DPRINTF("** MHL CRC value I %d", !ucIValue);
    MHL_DRV_DPRINTF("** MHL CRC value J %d", !ucJValue);
    MHL_DRV_DPRINTF("** MHL CRC value K %d", !ucKValue);
    MHL_DRV_DPRINTF("** MHL CRC value L %d", !ucLValue);
    MHL_DRV_DPRINTF("** MHL CRC value M %d", !ucMValue);
    MHL_DRV_DPRINTF("** MHL CRC value N %d", !ucNValue);
    MHL_DRV_DPRINTF("** MHL CRC value O %d", !ucOValue);
    MHL_DRV_DPRINTF("** MHL CRC value P %d", !ucPValue);

    ucAValue = ucInputStr[7] ^ucInputStr[6] ^ucInputStr[5] ^ucInputStr[4] ^ucInputStr[3] ^ucInputStr[2] ^ucInputStr[1] ^ucInputStr[0];
    ucBValue = ucInputStr[6] ^ucInputStr[5] ^ucInputStr[4] ^ucInputStr[3] ^ucInputStr[2] ^ucInputStr[1] ^ucInputStr[0] ^1;
    ucCValue = ucInputStr[7] ^ucInputStr[6];
    ucDValue = ucInputStr[6] ^ucInputStr[5];
    ucEValue = ucInputStr[5] ^ucInputStr[4];
    ucFValue = ucInputStr[4] ^ucInputStr[3];
    ucGValue = ucInputStr[3] ^ucInputStr[2];
    ucHValue = ucInputStr[2] ^ucInputStr[1];
    ucIValue = ucInputStr[1] ^ucInputStr[0] ^1;
    ucJValue = ucInputStr[0];
    ucKValue = 1;
    ucLValue = 1;
    ucMValue = 1;
    ucNValue = 1;
    ucOValue = 1;
    ucPValue = ucInputStr[7] ^ucInputStr[6] ^ucInputStr[5] ^ucInputStr[4] ^ucInputStr[3] ^ucInputStr[2] ^ucInputStr[1] ^ucInputStr[0] ^1;
    
    MHL_DRV_DPRINTF("** MHL CRC value A %d", !ucAValue);
    MHL_DRV_DPRINTF("** MHL CRC value B %d", !ucBValue);
    MHL_DRV_DPRINTF("** MHL CRC value C %d", !ucCValue);
    MHL_DRV_DPRINTF("** MHL CRC value D %d", !ucDValue);
    MHL_DRV_DPRINTF("** MHL CRC value E %d", !ucEValue);
    MHL_DRV_DPRINTF("** MHL CRC value F %d", !ucFValue);
    MHL_DRV_DPRINTF("** MHL CRC value G %d", !ucGValue);
    MHL_DRV_DPRINTF("** MHL CRC value H %d", !ucHValue);
    MHL_DRV_DPRINTF("** MHL CRC value I %d", !ucIValue);
    MHL_DRV_DPRINTF("** MHL CRC value J %d", !ucJValue);
    MHL_DRV_DPRINTF("** MHL CRC value K %d", !ucKValue);
    MHL_DRV_DPRINTF("** MHL CRC value L %d", !ucLValue);
    MHL_DRV_DPRINTF("** MHL CRC value M %d", !ucMValue);
    MHL_DRV_DPRINTF("** MHL CRC value N %d", !ucNValue);
    MHL_DRV_DPRINTF("** MHL CRC value O %d", !ucOValue);
    MHL_DRV_DPRINTF("** MHL CRC value P %d", !ucPValue);
    
#endif
}

#endif

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_SetConfiguration()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_mhl_SetConfiguration(BYTE ucMHLEnableIndex, BYTE ucDisplayEnableIndex)
{
    ucMHLFunctionEnableIndex = ucMHLEnableIndex;
    SET_MHL_DISPLAY_ENABLE_INDEX(ucDisplayEnableIndex);

    SET_MHL_SYSTEM_CONFIG_CHANGE();
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_LoadEDIDPort()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_mhl_LoadEDIDPort(BYTE ucPortSelect, BYTE *pComboEDID)
{
    if(_mdrv_mhl_CheckFunctionEnable(ucPortSelect))
    {
        mhal_mhl_LoadEDID(ucPortSelect, pComboEDID);
    }
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_Initial()
//  [Description]
//                  MHL init
//  [Arguments]:
//                  *edid: MHL EDID data
//                  *devcap: MHL device capability
//  [Return]:
//
//**************************************************************************
void mdrv_mhl_Initial(BYTE **pComboEDID, BYTE *ucKeyEvent)
{
    Bool bIsrEnable = FALSE;
    BYTE ucPortSelect = 0;
    BYTE ucEfuseIndex = msEread_GetDataFromEfuse(EFUSE_0, MHL_CBUS_EFUSE_ADDRESS);

#if(MHL_FUNCTION_SUPPORT_PORT0)
    memset(&stMHLBlock0, 0, sizeof(stMHLBlock0));
#endif
#if(MHL_FUNCTION_SUPPORT_PORT1)
    memset(&stMHLBlock1, 0, sizeof(stMHLBlock1));
#endif
#if(MHL_FUNCTION_SUPPORT_PORT2)
    memset(&stMHLBlock2, 0, sizeof(stMHLBlock2));
#endif
#if(MHL_FUNCTION_SUPPORT_PORT3)
    memset(&stMHLBlock3, 0, sizeof(stMHLBlock3));
#endif
#if(MHL_FUNCTION_SUPPORT_PORT4)
    memset(&stMHLBlock4, 0, sizeof(stMHLBlock4));
#endif
#if(MHL_FUNCTION_SUPPORT_PORT5)
    memset(&stMHLBlock5, 0, sizeof(stMHLBlock5));
#endif

    for(ucPortSelect = MHL_INPUT_SELECT_PORT0; ucPortSelect < MHL_INPUT_SELECT_MASK; ucPortSelect++)
    {
        ucKeyEvent[ucPortSelect] = 0;

        if(_mdrv_mhl_CheckFunctionEnable(ucPortSelect))
        {
            if(_mdrv_mhl_GetECbusSupportFlag(ucPortSelect))
            {
                MHL_DRV_DPRINTF("** MHL3 support !! port %d", ucPortSelect);
            }
            
            mhal_mhl_Initial(ucPortSelect, GET_MHL_SYSTEM_POWER_IGNORE(), pComboEDID[ucPortSelect], tMHL_DEVCAP_TABLE_PORT[ucPortSelect], MHL_VENDOR_ID);

            _mdrv_mhl_ParsingEDIDfor3D(ucPortSelect, pComboEDID[ucPortSelect]);

            if(!GET_MHL_SYSTEM_POWER_IGNORE())
            {
                mhal_mhl_SetRxXdataAddress(ucPortSelect, TRUE);
            }

            _mdrv_mhl_ClearCbusFlag(ucPortSelect, MHL_CLEAR_INITIAL_FLAG);

            mhal_mhl_CableDetectEnableControl(ucPortSelect, TRUE);

            mhal_mhl_RtermControlHWMode(ucPortSelect, FALSE);

            MHL_DRV_DPRINTF("** MHL initial setup !! port %d", ucPortSelect);

            bIsrEnable = TRUE;
        }
    }

    if(bIsrEnable)
    {
        mhal_mhl_EnableCbusInterrupt(TRUE);

        mhal_mhl_SetPullDownResistor(ucEfuseIndex);

#if(MHL_DEBUG_CHECK_CRC_VALUE)
        _mdrv_mhl_CheckCRCValue();
#endif
    }
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_GetCbusInterruptFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mdrv_mhl_GetCbusInterruptFlag(void)
{
    return mhal_mhl_GetCbusInterruptFlag();
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_SetMSGCallBackFunction
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_mhl_SetMSGCallBackFunction(MHL_CALL_BACK_FUNC pRCPFunc, MHL_CALL_BACK_FUNC pRAPFunc, MHL_CALL_BACK_FUNC pUCPFunc)
{
    pmdrv_mhl_CbusRCPEventProc = pRCPFunc;
    pmdrv_mhl_CbusRAPEventProc = pRAPFunc;
    pmdrv_mhl_CbusUCPEventProc = pUCPFunc;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_SetMSGCallBackFunction
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_mhl_SetMSGCallBackFunctionExtend(MHL_CALL_BACK_FUNC pATTFunc, MHL_CALL_BACK_FUNC pRBPFunc)
{
    pmdrv_mhl_CbusATTEventProc = pATTFunc;
    pmdrv_mhl_CbusRBPEventProc = pRBPFunc;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_TimerHandler()
//  [Description]
//                  MHL Cbus handle send MSC message
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_mhl_TimerHandler(void)
{
    BYTE ucPortSelect = 0;

    if(GET_MHL_SYSTEM_TIMER_FLAG())
    {
        CLR_MHL_SYSTEM_TIMER_FLAG();

        for(ucPortSelect = MHL_INPUT_SELECT_PORT0; ucPortSelect < MHL_INPUT_SELECT_MASK; ucPortSelect++)
        {
            if(_mdrv_mhl_CheckFunctionEnable(ucPortSelect))
            {
                _mdrv_mhl_CheckCbusTimer(ucPortSelect);

                _mdrv_mhl_SensationDConPatch(ucPortSelect, FALSE);
                
                _mdrv_mhl_BufferFlyReallyChargePatch(ucPortSelect, FALSE);

                _mdrv_mhl_SetMSGWaitResponseTimer(ucPortSelect, 0);

                _mdrv_mhl_SetRCPReleaseSendTimer(ucPortSelect, 0, MHL_RCP_KEY_SELECT_RELEASE);

                _mdrv_mhl_SetBISTProcTimer(ucPortSelect, 0);
            }
        }

        _mdrv_mhl_SetCableDetectDisableTime(MHL_INPUT_SELECT_NONE, 0);

#if(MHL_CBUS_SELF_TEST_SUPPORT)
        _mdrv_mhl_SetCbusSelfTestTimer(MHL_CBUS_SELT_TEST_PORT, 0);
#endif
    }
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_CheckStatusPolling()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_mhl_CheckStatusPolling(void)
{
    Bool bFlag = FALSE;
    BYTE ucPortSelect = 0;

    for(ucPortSelect = MHL_INPUT_SELECT_PORT0; ucPortSelect < MHL_INPUT_SELECT_MASK; ucPortSelect++)
    {
        if(_mdrv_mhl_CheckFunctionEnable(ucPortSelect))
        {
            bFlag = mhal_mhl_CableDetect(ucPortSelect);
             if(bFlag) // Cable detect high
            {
                _mdrv_mhl_CablePlugProc(ucPortSelect);

                bFlag = mhal_mhl_CbusConnectStatus(ucPortSelect);
                 if(bFlag) // Cbus connect
                {
                 
                    _mdrv_mhl_CbusConnectProc(ucPortSelect);
                     if(_mdrv_mhl_GetCbusEnhanceFlag(ucPortSelect))
                    {
                        _mdrv_mhl_ECbusStateHandler(ucPortSelect);
                    }

                    mhal_mhl_CDRStableMonitor(ucPortSelect, _mdrv_mhl_GetDisplayReadyFlag(ucPortSelect));
                }
                else // Cbus disconnect
                {
                    _mdrv_mhl_CbusStucktoLowProc(ucPortSelect);
                }
            }
            else // Cable detect low
            {
                _mdrv_mhl_CableRemoveProc(ucPortSelect);
            }
        }
    }

    _mdrv_mhl_CheckDisplayConfig();

    if(GET_MHL_SYSTEM_INIT_STATUS())
        CLR_MHL_SYSTEM_INIT_STATUS();
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_CbusTxHandler()
//  [Description]
//                  MHL Cbus handle send MSC message
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_mhl_CbusTxHandler(void)
{
    BYTE ucPortSelect = 0;

    for(ucPortSelect = MHL_INPUT_SELECT_PORT0; ucPortSelect < MHL_INPUT_SELECT_MASK; ucPortSelect++)
    {
        if(_mdrv_mhl_GetCbusConnectFlag(ucPortSelect))
        {
            _mdrv_mhl_TxQueueHandler(ucPortSelect);

            _mdrv_mhl_SendDisplayCommand(ucPortSelect);

            _mdrv_mhl_WriteBurstEventProc(ucPortSelect);

            _mdrv_mhl_SendWriteBurstInformation(ucPortSelect);

            _mdrv_mhl_ReadDeviceInformation(ucPortSelect);

            if(_mdrv_mhl_GetCbusEnhanceFlag(ucPortSelect))
            {
                _mdrv_mhl_ReadExtendDeviceCapacibility(ucPortSelect);

                _mdrv_mhl_CbusBISTStateProc(ucPortSelect);

                _mdrv_mhl_CbusRHIDStateProc(ucPortSelect);

                _mdrv_mhl_FeatureRequestStateProc(ucPortSelect);

                _mdrv_mhl_CheckEMSCRxBufferSize(ucPortSelect);
            }
        }
    }
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_CbusRxHandler()
//  [Description]
//                  MHL Cbus reply the received MSC message
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_mhl_CbusRxHandler(void)
{
    BYTE ucPortSelect = 0;

    for(ucPortSelect = MHL_INPUT_SELECT_PORT0; ucPortSelect < MHL_INPUT_SELECT_MASK; ucPortSelect++)
    {
        if(_mdrv_mhl_GetCbusConnectFlag(ucPortSelect))
        {
            if(_mdrv_mhl_CheckReceiveCommand(ucPortSelect))
            {
                _mdrv_mhl_CbusReceiveEventProc(ucPortSelect);		 		
            }
        }
    }
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_CbusMSGCommandProc()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_mhl_CbusMSGCommandProc(void)
{
    Bool bReceiveResponse = FALSE;
    Bool bReceiveNoError = FALSE;
    BYTE ucPortSelect = 0;
    BYTE ucStatus = 0;
    BYTE ucSubCommand = 0;
    BYTE ucKeyCode = 0;

    for(ucPortSelect = MHL_INPUT_SELECT_PORT0; ucPortSelect < MHL_INPUT_SELECT_MASK; ucPortSelect++)
    {
        if(_mdrv_mhl_GetCbusConnectFlag(ucPortSelect))
        {
            if(_mdrv_mhl_GetReceiveMSGInformation(ucPortSelect, &ucSubCommand, &ucKeyCode))
            {
                SET_MHL_CBUS_MSC_MSG_PROC_PORT(ucPortSelect);

                switch(ucSubCommand)
                {
                    case MSC_MSG_RCP:
                        if(pmdrv_mhl_CbusRCPEventProc == NULL)
                        {
                            ucStatus = MHL_RCP_NO_ERROR;
                        }
                        else
                        {
                            ucStatus = pmdrv_mhl_CbusRCPEventProc(ucKeyCode);
                        }

                        _mdrv_mhl_CbusMSGRCPReply(ucPortSelect, ucStatus, ucKeyCode);

                        break;

                    case MSC_MSG_RCPK:
                    case MSC_MSG_RCPE:
                        bReceiveResponse = TRUE;
                        break;

                    case MSC_MSG_RAP:
                        if(pmdrv_mhl_CbusRAPEventProc == NULL)
                        {
                            ucStatus = MHL_RAP_NO_ERROR;
                        }
                        else
                        {
                            ucStatus = pmdrv_mhl_CbusRAPEventProc(ucKeyCode);
                        }

                        _mdrv_mhl_CbusMSGRAPReply(ucPortSelect, ucStatus);

                        break;

                    case MSC_MSG_RAPK:
                        bReceiveResponse = TRUE;
                        break;

                    case MSC_MSG_UCP:
                        if(pmdrv_mhl_CbusUCPEventProc == NULL)
                        {
                            ucStatus = MHL_UCP_NO_ERROR;
                        }
                        else
                        {
                            ucStatus = pmdrv_mhl_CbusUCPEventProc(ucKeyCode);
                        }

                        _mdrv_mhl_CbusMSGUCPReply(ucPortSelect, ucStatus, ucKeyCode);

                        break;

                    case MSC_MSG_UCPK:
                    case MSC_MSG_UCPE:
                        bReceiveResponse = TRUE;
                        break;

                    case MSC_MSG_ATT:
                        if(pmdrv_mhl_CbusATTEventProc == NULL)
                        {
                            ucStatus = MHL_ATT_NO_ERROR;
                        }
                        else
                        {
                            ucStatus = pmdrv_mhl_CbusATTEventProc(ucKeyCode);
                        }

                        _mdrv_mhl_CbusMSGATTReply(ucPortSelect, ucStatus);

                        break;

                    case MSC_MSG_ATTK:
                        bReceiveResponse = TRUE;
                        break;

                    case MSC_MSG_RBP:
                        if(pmdrv_mhl_CbusRBPEventProc == NULL)
                        {
                            ucStatus = MHL_RBP_NO_ERROR;
                        }
                        else
                        {
                            ucStatus = pmdrv_mhl_CbusRBPEventProc(ucKeyCode);
                        }

                        _mdrv_mhl_CbusMSGRBPReply(ucPortSelect, ucStatus, ucKeyCode);

                        break;

                    case MSC_MSG_RBPK:
                    case MSC_MSG_RBPE:
                        bReceiveResponse = TRUE;
                        break;

                    case MSC_MSG_RUSB:

                        break;

                    case MSC_MSG_RUSBK:
                        bReceiveResponse = TRUE;
                        break;

                    case MSC_MSG_RHID:
                        ucStatus = _mdrv_mhl_CbusRHIDEventProc(ucKeyCode);

                        _mdrv_mhl_CbusMSGRHIDReply(ucPortSelect, ucStatus);

                        break;

                    case MSC_MSG_RHIDK:
                        bReceiveResponse = TRUE;

                        if(ucKeyCode == MHL_RHID_NO_ERROR)
                        {
                            bReceiveNoError = TRUE;
                        }

                        _mdrv_mhl_CbusRHIDAcceptHost(ucPortSelect, bReceiveNoError);

                        break;

                    case MSC_MSG_BIST_TRIGGRT:
                        _mdrv_mhl_CbusBISTTriggerProc(ucKeyCode);
                        break;

                    case MSC_MSG_BIST_REQUEST_STAT:
                        _mdrv_mhl_CbusBISTRequestStatusProc(ucKeyCode);
                        break;

                    case MSC_MSG_BIST_STOP:
                        _mdrv_mhl_CbusBISTStopProc(ucKeyCode);
                        break;

                    case MSC_MSG_SI_UNKNOWN:
                        _mdrv_mhl_CbusRxQueueInsertMSG(ucPortSelect, MSC_MSG_SI_UNKNOWNK, 0);
                        break;

                    default:

                        break;
                };

                if(bReceiveResponse)
                {
                     _mdrv_mhl_ClearCbusFlag(ucPortSelect, MHL_CLEAR_MSG_RESPONSE_FLAG);
                }
            }
        }
    }
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_IsrHandler()
//  [Description]
//                  MHL isr handler
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_mhl_IsrHandler(void)
{
    BYTE ucPortSelect = 0;

    for(ucPortSelect = MHL_INPUT_SELECT_PORT0; ucPortSelect < MHL_INPUT_SELECT_MASK; ucPortSelect++)
    {
        if(_mdrv_mhl_CheckFunctionEnable(ucPortSelect))
        {
            if(mhal_mhl_GetCbusReceiveFlag(ucPortSelect))
            {
                _mdrv_mhl_CbusReceiveISRProc(ucPortSelect);
            }

            if(mhal_mhl_GetStucktoLowFlag(ucPortSelect))
            {
                // Rterm off
                mhal_mhl_ComboRtermControl(ucPortSelect, RX_RTERM_OFF);
            }

            if(_mdrv_mhl_GetCbusEnhanceFlag(ucPortSelect))
            {
                if(mhal_mhl_GetECbusStateChangeFlag(ucPortSelect))
                {
                    mhal_mhl_ECbusStateChangeProc(ucPortSelect);
                }

                if(mhal_mhl_GetEMSCReceiveFlag(ucPortSelect))
                {
                    _mdrv_mhl_CbusReceiveEMSCISRProc(ucPortSelect);
                }
            }
        }
    }
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_CbusHWControlRterm()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_mhl_CbusHWControlRterm(Bool bEnable)
{
    BYTE ucPortSelect = 0;

    for(ucPortSelect = MHL_INPUT_SELECT_PORT0; ucPortSelect < MHL_INPUT_SELECT_MASK; ucPortSelect++)
    {
        if(mdrv_mhl_GetCableDetectPort(ucPortSelect))
        {
            _mdrv_mhl_RtermControlHWMode(ucPortSelect, bEnable);
        }
    }
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_CbusSendMSGCommand()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mdrv_mhl_CbusSendRCPCommand(BYTE ucPortSelect, BYTE ucKeyCode)
{
    Bool bSendRCP = FALSE;

    if(_mdrv_mhl_GetCbusConnectFlag(ucPortSelect))
    {
        switch(ucPortSelect)
        {
#if(MHL_FUNCTION_SUPPORT_PORT0)
            case MHL_INPUT_SELECT_PORT0:
                if(GET_MHL_SEND_RCP_ENABLE_PORT0() && (!GET_MHL_SEND_MSG_COMMAND_PORT0()))
                {
                    bSendRCP = TRUE;
                }

                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
            case MHL_INPUT_SELECT_PORT1:
                if(GET_MHL_SEND_RCP_ENABLE_PORT1() && (!GET_MHL_SEND_MSG_COMMAND_PORT1()))
                {
                    bSendRCP = TRUE;
                }

                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
            case MHL_INPUT_SELECT_PORT2:
                if(GET_MHL_SEND_RCP_ENABLE_PORT2() && (!GET_MHL_SEND_MSG_COMMAND_PORT2()))
                {
                    bSendRCP = TRUE;
                }

                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
            case MHL_INPUT_SELECT_PORT3:
                if(GET_MHL_SEND_RCP_ENABLE_PORT3() && (!GET_MHL_SEND_MSG_COMMAND_PORT3()))
                {
                    bSendRCP = TRUE;
                }

                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
            case MHL_INPUT_SELECT_PORT4:
                if(GET_MHL_SEND_RCP_ENABLE_PORT4() && (!GET_MHL_SEND_MSG_COMMAND_PORT4()))
                {
                    bSendRCP = TRUE;
                }

                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
            case MHL_INPUT_SELECT_PORT5:
                if(GET_MHL_SEND_RCP_ENABLE_PORT5() && (!GET_MHL_SEND_MSG_COMMAND_PORT5()))
                {
                    bSendRCP = TRUE;
                }

                break;
#endif

            default:

                break;
        };

        if(bSendRCP)
        {
            bSendRCP = _mdrv_mhl_CbusSendSubMessage(ucPortSelect, MSC_MSG_RCP, ucKeyCode, TRUE);

            _mdrv_mhl_SetRCPReleaseSendTimer(ucPortSelect, MHL_CBUS_RCP_RELEASE_TIME, ucKeyCode);

            MHL_DRV_DPRINTF("** MHL send RCP press key code 0x%x", ucKeyCode);
        }
    }

    return bSendRCP;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_CbusSendRAPCommand()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mdrv_mhl_CbusSendRAPCommand(BYTE ucPortSelect, BYTE ucKeyCode)
{
    Bool bSendRAP = FALSE;

    if(_mdrv_mhl_GetCbusConnectFlag(ucPortSelect))
    {
        switch(ucPortSelect)
        {
#if(MHL_FUNCTION_SUPPORT_PORT0)
            case MHL_INPUT_SELECT_PORT0:
                if(GET_MHL_SEND_RAP_ENABLE_PORT0() && (!GET_MHL_SEND_MSG_COMMAND_PORT0()))
                {
                    bSendRAP = TRUE;
                }

                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
            case MHL_INPUT_SELECT_PORT1:
                if(GET_MHL_SEND_RAP_ENABLE_PORT1() && (!GET_MHL_SEND_MSG_COMMAND_PORT1()))
                {
                    bSendRAP = TRUE;
                }

                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
            case MHL_INPUT_SELECT_PORT2:
                if(GET_MHL_SEND_RAP_ENABLE_PORT2() && (!GET_MHL_SEND_MSG_COMMAND_PORT2()))
                {
                    bSendRAP = TRUE;
                }

                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
            case MHL_INPUT_SELECT_PORT3:
                if(GET_MHL_SEND_RAP_ENABLE_PORT3() && (!GET_MHL_SEND_MSG_COMMAND_PORT3()))
                {
                    bSendRAP = TRUE;
                }

                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
            case MHL_INPUT_SELECT_PORT4:
                if(GET_MHL_SEND_RAP_ENABLE_PORT4() && (!GET_MHL_SEND_MSG_COMMAND_PORT4()))
                {
                    bSendRAP = TRUE;
                }

                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
            case MHL_INPUT_SELECT_PORT5:
                if(GET_MHL_SEND_RAP_ENABLE_PORT5() && (!GET_MHL_SEND_MSG_COMMAND_PORT5()))
                {
                    bSendRAP = TRUE;
                }

                break;
#endif

            default:

                break;
        };

        if(bSendRAP)
        {
            bSendRAP = _mdrv_mhl_CbusSendSubMessage(ucPortSelect, MSC_MSG_RAP, ucKeyCode, TRUE);
        }
    }

    return bSendRAP;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_CbusSendMSGCommand ()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mdrv_mhl_CbusSendUCPCommand(BYTE ucPortSelect, BYTE ucKeyCode)
{
    Bool bSendUCP = FALSE;

    if(_mdrv_mhl_GetCbusConnectFlag(ucPortSelect))
    {
        switch(ucPortSelect)
        {
#if(MHL_FUNCTION_SUPPORT_PORT0)
            case MHL_INPUT_SELECT_PORT0:
                if(GET_MHL_SEND_UCP_ENABLE_PORT0() && (!GET_MHL_SEND_MSG_COMMAND_PORT0()))
                {
                    bSendUCP = TRUE;
                }

                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
            case MHL_INPUT_SELECT_PORT1:
                if(GET_MHL_SEND_UCP_ENABLE_PORT1() && (!GET_MHL_SEND_MSG_COMMAND_PORT1()))
                {
                    bSendUCP = TRUE;
                }

                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
            case MHL_INPUT_SELECT_PORT2:
                if(GET_MHL_SEND_UCP_ENABLE_PORT2() && (!GET_MHL_SEND_MSG_COMMAND_PORT2()))
                {
                    bSendUCP = TRUE;
                }

                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
            case MHL_INPUT_SELECT_PORT3:
                if(GET_MHL_SEND_UCP_ENABLE_PORT3() && (!GET_MHL_SEND_MSG_COMMAND_PORT3()))
                {
                    bSendUCP = TRUE;
                }

                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
            case MHL_INPUT_SELECT_PORT4:
                if(GET_MHL_SEND_UCP_ENABLE_PORT4() && (!GET_MHL_SEND_MSG_COMMAND_PORT4()))
                {
                    bSendUCP = TRUE;
                }

                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
            case MHL_INPUT_SELECT_PORT5:
                if(GET_MHL_SEND_UCP_ENABLE_PORT5() && (!GET_MHL_SEND_MSG_COMMAND_PORT5()))
                {
                    bSendUCP = TRUE;
                }

                break;
#endif

            default:

                break;
        };

        if(bSendUCP)
        {
            bSendUCP = _mdrv_mhl_CbusSendSubMessage(ucPortSelect, MSC_MSG_UCP, ucKeyCode, TRUE);
        }
    }

    return bSendUCP;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_CbusSendWriteBurst
//  [Description]
//                  MHL Cbus write burst
//  [Arguments]:
//                  addr: offset
//                  lens: data length, offset + maxi 16 packets
//                  *buf: burst data
//  [Return]:
//                  TRUE: success
//                  FALSE: fail
//**************************************************************************
Bool mdrv_mhl_CbusSendWriteBurst(BYTE ucPortSelect, BYTE ucAddr, BYTE ucLength, BYTE *pBuffer)
{
    BYTE uctemp = 0;
    CbusReq_S stQueue;

    if(ucLength > MHL_MSC_SCRATCHPAD_SIZE)
    {
        ucLength = MHL_MSC_SCRATCHPAD_SIZE;
    }

    stQueue.cmd = MSC_WRITE_BURST;
    stQueue.len = ucLength +1; // offset + data packets
    stQueue.msgData[0] = ucAddr; // offset packet
    stQueue.reqStatus = CBUS_REQ_PENDING;

    for(uctemp =0; uctemp <ucLength; uctemp++)
    {
        stQueue.msgData[uctemp +1] = pBuffer[uctemp];
    }

    return _mdrv_mhl_CbusTxQueueInsertMSC(ucPortSelect, &stQueue);
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_CbusSendUserWriteBurst
//  [Description]
//                  MHL Cbus write burst use request and grant process
//  [Arguments]:
//                  addr: offset
//                  lens: data length, offset + maxi 16 packets
//                  *buf: burst data
//  [Return]:
//                  TRUE: success
//                  FALSE: fail
//**************************************************************************
Bool mdrv_mhl_CbusSendUserWriteBurst(BYTE ucPortSelect, BYTE ucAddr, BYTE ucLength, BYTE *pBuffer)
{
    Bool bFinish = FALSE;

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            if(GET_MHL_SEND_USER_WRITE_BURST0())
            {
                if(GET_MHL_WRITE_BURST_STATE_PORT0() == MHL_CBUS_WRITE_BURST_REVEIVE_GRANT)
                {
                    if(mdrv_mhl_CbusSendWriteBurst(ucPortSelect, ucAddr, ucLength, pBuffer))
                    {
                        CLR_MHL_SEND_USER_WRITE_BURST0();
                        SET_MHL_WRITE_BURST_STATE_PORT0(MHL_CBUS_WRITE_BURST_SEND_DATA);

                        bFinish = TRUE;
                    }
                }
            }
            else if(GET_MHL_WRITE_BURST_STATE_PORT0() == MHL_CBUS_WRITE_BURST_NONE)
            {
                SET_MHL_SEND_USER_WRITE_BURST0();
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            if(GET_MHL_SEND_USER_WRITE_BURST1())
            {
                if(GET_MHL_WRITE_BURST_STATE_PORT1() == MHL_CBUS_WRITE_BURST_REVEIVE_GRANT)
                {
                    if(mdrv_mhl_CbusSendWriteBurst(ucPortSelect, ucAddr, ucLength, pBuffer))
                    {
                        CLR_MHL_SEND_USER_WRITE_BURST1();
                        SET_MHL_WRITE_BURST_STATE_PORT1(MHL_CBUS_WRITE_BURST_SEND_DATA);

                        bFinish = TRUE;
                    }
                }
            }
            else if(GET_MHL_WRITE_BURST_STATE_PORT1() == MHL_CBUS_WRITE_BURST_NONE)
            {
                SET_MHL_SEND_USER_WRITE_BURST1();
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            if(GET_MHL_SEND_USER_WRITE_BURST2())
            {
                if(GET_MHL_WRITE_BURST_STATE_PORT2() == MHL_CBUS_WRITE_BURST_REVEIVE_GRANT)
                {
                    if(mdrv_mhl_CbusSendWriteBurst(ucPortSelect, ucAddr, ucLength, pBuffer))
                    {
                        CLR_MHL_SEND_USER_WRITE_BURST2();
                        SET_MHL_WRITE_BURST_STATE_PORT2(MHL_CBUS_WRITE_BURST_SEND_DATA);

                        bFinish = TRUE;
                    }
                }
            }
            else if(GET_MHL_WRITE_BURST_STATE_PORT2() == MHL_CBUS_WRITE_BURST_NONE)
            {
                SET_MHL_SEND_USER_WRITE_BURST2();
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            if(GET_MHL_SEND_USER_WRITE_BURST3())
            {
                if(GET_MHL_WRITE_BURST_STATE_PORT3() == MHL_CBUS_WRITE_BURST_REVEIVE_GRANT)
                {
                    if(mdrv_mhl_CbusSendWriteBurst(ucPortSelect, ucAddr, ucLength, pBuffer))
                    {
                        CLR_MHL_SEND_USER_WRITE_BURST3();
                        SET_MHL_WRITE_BURST_STATE_PORT3(MHL_CBUS_WRITE_BURST_SEND_DATA);

                        bFinish = TRUE;
                    }
                }
            }
            else if(GET_MHL_WRITE_BURST_STATE_PORT3() == MHL_CBUS_WRITE_BURST_NONE)
            {
                SET_MHL_SEND_USER_WRITE_BURST3();
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            if(GET_MHL_SEND_USER_WRITE_BURST4())
            {
                if(GET_MHL_WRITE_BURST_STATE_PORT4() == MHL_CBUS_WRITE_BURST_REVEIVE_GRANT)
                {
                    if(mdrv_mhl_CbusSendWriteBurst(ucPortSelect, ucAddr, ucLength, pBuffer))
                    {
                        CLR_MHL_SEND_USER_WRITE_BURST4();
                        SET_MHL_WRITE_BURST_STATE_PORT4(MHL_CBUS_WRITE_BURST_SEND_DATA);

                        bFinish = TRUE;
                    }
                }
            }
            else if(GET_MHL_WRITE_BURST_STATE_PORT4() == MHL_CBUS_WRITE_BURST_NONE)
            {
                SET_MHL_SEND_USER_WRITE_BURST4();
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            if(GET_MHL_SEND_USER_WRITE_BURST5())
            {
                if(GET_MHL_WRITE_BURST_STATE_PORT5() == MHL_CBUS_WRITE_BURST_REVEIVE_GRANT)
                {
                    if(mdrv_mhl_CbusSendWriteBurst(ucPortSelect, ucAddr, ucLength, pBuffer))
                    {
                        CLR_MHL_SEND_USER_WRITE_BURST5();
                        SET_MHL_WRITE_BURST_STATE_PORT5(MHL_CBUS_WRITE_BURST_SEND_DATA);

                        bFinish = TRUE;
                    }
                }
            }
            else if(GET_MHL_WRITE_BURST_STATE_PORT5() == MHL_CBUS_WRITE_BURST_NONE)
            {
                SET_MHL_SEND_USER_WRITE_BURST5();
            }

            break;
#endif

        default:

            break;
    };

    return bFinish;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_PowerControl()
//  [Description]
//                  MHL power control
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_mhl_PowerControl(BYTE ucState)
{
    BYTE ucPortSelect = 0;

    for(ucPortSelect = MHL_INPUT_SELECT_PORT0; ucPortSelect < MHL_INPUT_SELECT_MASK; ucPortSelect++)
    {
        if(_mdrv_mhl_CheckFunctionEnable(ucPortSelect))
        {
            switch(ucState)
            {
                case MHL_POWER_DOWN:
                    mhal_mhl_PowerDownProc(ucPortSelect);
                case MHL_POWER_STANDBY:
                    mhal_mhl_SetRxXdataAddress(ucPortSelect, TRUE);
                    break;

                default:

                    break;
            };
        }
    }
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

    if(mhal_mhl_CbusConnectStatus(ucPortSelect))
    {
        bCbusConnect = TRUE;
    }

    return bCbusConnect;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_ReceiveECbusModeChange()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_mhl_ReceiveECbusModeChange(BYTE ucPortSelect, BYTE ucKeyCode)
{
    BYTE ucECbusState = MHL_ECBUS_STATE_RECEIVE_MODE_DOWN;
    
    if(ucKeyCode == MHL_RAP_KEY_CBUS_MODE_UP)
    {
        ucECbusState = MHL_ECBUS_STATE_RECEIVE_MODE_UP;

        MHL_DRV_DPRINTF("** MHL receive ECbus mode up!! port %d", ucPortSelect);
    }
    else
    {
        MHL_DRV_DPRINTF("** MHL receive ECbus mode down!! port %d", ucPortSelect);
    }

    _mdrv_mhl_SetECbusState(ucPortSelect, ucECbusState);
}

#if(MHL_GET_CABLE_DETECT_SUPPORT)
//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_GetCableDetectPortByInit()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mdrv_mhl_GetCableDetectPortByInit(BYTE ucPortSelect)
{
    Bool bFlag;
    
    bFlag = mhal_mhl_SAR_Detect(ucPortSelect);
    return bFlag;
}
//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_GetCableDetectPort()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mdrv_mhl_GetCableDetectPort(BYTE ucPortSelect)
{
    Bool bFlag = FALSE;

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            bFlag = GET_MHL_CABLE_PLUGGED0();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            bFlag = GET_MHL_CABLE_PLUGGED1();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            bFlag = GET_MHL_CABLE_PLUGGED2();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            bFlag = GET_MHL_CABLE_PLUGGED3();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            bFlag = GET_MHL_CABLE_PLUGGED4();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            bFlag = GET_MHL_CABLE_PLUGGED5();
            break;
#endif

        default:

            break;
    };

    return bFlag;
}

#endif
//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_GetCbusConnectPort()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mdrv_mhl_GetCbusConnectPort(BYTE ucPortSelect)
{
    Bool bFlag = FALSE;

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            bFlag = GET_MHL_CBUS_CONNECT0();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            bFlag = GET_MHL_CBUS_CONNECT1();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            bFlag = GET_MHL_CBUS_CONNECT2();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            bFlag = GET_MHL_CBUS_CONNECT3();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            bFlag = GET_MHL_CBUS_CONNECT4();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            bFlag = GET_MHL_CBUS_CONNECT5();
            break;
#endif

        default:

            break;
    };

    return bFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_GetDisplayFlagPort()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mdrv_mhl_GetDisplayFlagPort(BYTE ucPortSelect)
{
    Bool bFlag = FALSE;

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            bFlag = GET_MHL_DISPLAY_ENABLE_PORT0();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            bFlag = GET_MHL_DISPLAY_ENABLE_PORT1();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            bFlag = GET_MHL_DISPLAY_ENABLE_PORT2();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            bFlag = GET_MHL_DISPLAY_ENABLE_PORT3();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            bFlag = GET_MHL_DISPLAY_ENABLE_PORT4();
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            bFlag = GET_MHL_DISPLAY_ENABLE_PORT5();
            break;
#endif

        default:

            break;
    };

    return bFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_SendEMSCDataString()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE mdrv_mhl_SendEMSCDataString(BYTE ucPortSelect, BYTE ucLength, BYTE *ucSendData)
{
    Bool bDoAgainFlag = FALSE;
    Bool bUpdateBufferSize = FALSE;
    BYTE uctemp = 0;
    BYTE ucSendStatus = 0;
    BYTE ucSendEMSC[MHL_EMSC_DATA_SIZE];
    static BYTE ucEMSCSendStatus[MHL_INPUT_SELECT_MASK];
    WORD usEMSCRxBufferSize = _mdrv_mhl_GetEMSCRxBufferSize(ucPortSelect);
    WORD usEMSCTxBufferSize = _mdrv_mhl_GetEMSCTxBufferSize(ucPortSelect);

    do
    {
        bDoAgainFlag = FALSE;

        switch(ucEMSCSendStatus[ucPortSelect])
        {
            case MHL_EMSC_SEND_NONE:
                if((ucLength +2) <= usEMSCTxBufferSize) // eMSC data length must small than Tx buffer +2 
                {
                    ucSendEMSC[0] = MHL_EMSC_DATA_SIZE - usEMSCRxBufferSize;
                    ucSendEMSC[1] = ucLength;

                    if(ucSendData != 0)
                    {
                        for(uctemp = 0; uctemp < ucLength; uctemp++)
                        {
                            ucSendEMSC[uctemp +2] = ucSendData[uctemp];
                        }
                    }
                    
                    ucEMSCSendStatus[ucPortSelect] = MHL_EMSC_SEND_DATA;
                    bDoAgainFlag = TRUE;
                }
                
                break;
                
            case MHL_EMSC_SEND_DATA:
                ucSendStatus = mhal_mhl_GetEMSCSendStatus(ucPortSelect);
                mhal_mhl_InsertEMSCSendData(ucPortSelect, ucLength +2, ucSendEMSC);

                ucEMSCSendStatus[ucPortSelect] = MHL_EMSC_SEND_CHECK;
                
                break;

            case MHL_EMSC_SEND_CHECK:
                ucSendStatus = mhal_mhl_GetEMSCSendStatus(ucPortSelect);

                if(ucSendStatus != MHL_EMSC_SEND_NONE)
                {
                    ucEMSCSendStatus[ucPortSelect] = ucSendStatus;
                }
                
                break;

            case MHL_EMSC_SEND_PASS:
                ucEMSCSendStatus[ucPortSelect] = MHL_EMSC_SEND_NONE;
                bUpdateBufferSize = TRUE;
                break;
                
            case MHL_EMSC_SEND_FAIL:
                ucEMSCSendStatus[ucPortSelect] = MHL_EMSC_SEND_NONE;
                break;

            default:
                ucEMSCSendStatus[ucPortSelect] = MHL_EMSC_SEND_NONE;
                break;
        };
    }while(bDoAgainFlag);

    if(bUpdateBufferSize)
    {
        switch(ucPortSelect)
        {
#if(MHL_FUNCTION_SUPPORT_PORT0)
            case MHL_INPUT_SELECT_PORT0:
                SET_MHL_EMSC_RX_BUFFER_SIZE0(MHL_EMSC_DATA_SIZE);
                SET_MHL_EMSC_TX_BUFFER_SIZE0(GET_MHL_EMSC_TX_BUFFER_SIZE0() -ucLength -2);
                usEMSCRxBufferSize = GET_MHL_EMSC_RX_BUFFER_SIZE0();
                usEMSCTxBufferSize = GET_MHL_EMSC_TX_BUFFER_SIZE0();
                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
            case MHL_INPUT_SELECT_PORT1:
                SET_MHL_EMSC_RX_BUFFER_SIZE1(MHL_EMSC_DATA_SIZE);
                SET_MHL_EMSC_TX_BUFFER_SIZE1(GET_MHL_EMSC_TX_BUFFER_SIZE1() -ucLength -2);
                usEMSCRxBufferSize = GET_MHL_EMSC_RX_BUFFER_SIZE1();
                usEMSCTxBufferSize = GET_MHL_EMSC_TX_BUFFER_SIZE1();
                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
            case MHL_INPUT_SELECT_PORT2:
                SET_MHL_EMSC_RX_BUFFER_SIZE2(MHL_EMSC_DATA_SIZE);
                SET_MHL_EMSC_TX_BUFFER_SIZE2(GET_MHL_EMSC_TX_BUFFER_SIZE2() -ucLength -2);
                usEMSCRxBufferSize = GET_MHL_EMSC_RX_BUFFER_SIZE2();
                usEMSCTxBufferSize = GET_MHL_EMSC_TX_BUFFER_SIZE2();
                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
            case MHL_INPUT_SELECT_PORT3:
                SET_MHL_EMSC_RX_BUFFER_SIZE3(MHL_EMSC_DATA_SIZE);
                SET_MHL_EMSC_TX_BUFFER_SIZE3(GET_MHL_EMSC_TX_BUFFER_SIZE3() -ucLength -2);
                usEMSCRxBufferSize = GET_MHL_EMSC_RX_BUFFER_SIZE3();
                usEMSCTxBufferSize = GET_MHL_EMSC_TX_BUFFER_SIZE3();
                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
            case MHL_INPUT_SELECT_PORT4:
                SET_MHL_EMSC_RX_BUFFER_SIZE4(MHL_EMSC_DATA_SIZE);
                SET_MHL_EMSC_TX_BUFFER_SIZE4(GET_MHL_EMSC_TX_BUFFER_SIZE4() -ucLength -2);
                usEMSCRxBufferSize = GET_MHL_EMSC_RX_BUFFER_SIZE4();
                usEMSCTxBufferSize = GET_MHL_EMSC_TX_BUFFER_SIZE4();
                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
            case MHL_INPUT_SELECT_PORT5:
                SET_MHL_EMSC_RX_BUFFER_SIZE5(MHL_EMSC_DATA_SIZE);
                SET_MHL_EMSC_TX_BUFFER_SIZE5(GET_MHL_EMSC_TX_BUFFER_SIZE5() -ucLength -2);
                usEMSCRxBufferSize = GET_MHL_EMSC_RX_BUFFER_SIZE5();
                usEMSCTxBufferSize = GET_MHL_EMSC_TX_BUFFER_SIZE5();
                break;
#endif

            default:

                break;
        };

        MHL_DRV_EMSCBUFFER("** MHL eMSC send Rx buffer size %d !!", usEMSCRxBufferSize);
        MHL_DRV_EMSCBUFFER("** MHL eMSC send Tx buffer size %d !!", usEMSCTxBufferSize);
    }

    return ucEMSCSendStatus[ucPortSelect];
}
//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_CablePlugProcByInit
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_mhl_CablePlugProcByInit(BYTE ucInputPort)
{    
    if(GET_MHL_SYSTEM_INIT_STATUS())
    {
         if(!GET_MHL_SYSTEM_POWER_IGNORE())//(g_eBootStatus == eBOOT_STATUS_ACON)
         {
                mhal_mhl_CablePlugProcByInit(ucInputPort);
                _mdrv_mhl_SetCableDetectDisableTime(ucInputPort, MHL_Cable_Detect_Off_TIME);
          }
     }
}

#endif // _MDRV_MHL_C_



#include "Board.h"

#if ENABLE_CEC

#define _API_CEC_C_

#include "datatype.h"
#include "debug.h"
#include "misc.h"
#include "Mode.h"
#include "UserPrefDef.h"
#include "Global.h"
#include "mStar.h"
#include "mdrv_CEC.c"
#include "mapi_CEC.h"

#define ENABLE_API_CEC_DEBUG 0

#if (ENABLE_API_CEC_DEBUG == 1)
#define CEC_PRINT_MSG(str)          printMsg(str)//putstr(str)
#define CEC_PRINT_DATA(str, x)      printData(str, x)//printf(str, x)
#else
#define CEC_PRINT_MSG(str)
#define CEC_PRINT_DATA(str, x)
#endif

// CEC function catalog
#define CECMSG_SUP_ONE_TOUCH_PLAY               1
#define CECMSG_SUP_ROUTING_CONTROL              0
#define CECMSG_SUP_STANDBY_COMMAND              1 // All
#define CECMSG_SUP_ONE_TOUCH_RECORD             1 // Recording devices
#define CECMSG_SUP_TIMER_PROGRAMMER             0 //
#define CECMSG_SUP_SYSTEM_INFORMATION           1 // All
#define CECMSG_SUP_DECK_CONTROL                 1
#define CECMSG_SUP_TUNER_CONTROL                0
#define CECMSG_SUP_VENDOR_SPECIFIC              1 // All
#define CECMSG_SUP_OSD_DISPLAY                  0
#define CECMSG_SUP_DEVICE_OSD_NAME_TRANSFER     0
#define CECMSG_SUP_DEVICE_MENU_CONTROL          0
#define CECMSG_SUP_REMOTE_CONTROL_PASSTHROUGH   0
#define CECMSG_SUP_POWER_STATUS                 1 // All except CEC switches
#define CECMSG_SUP_SYSTEM_AUDIO_CONTROL         0
#define CECMSG_SUP_GENERAL_PROTOCAL             1 // All
#define CECMSG_SUP_AUDIO_RETURN_CHANNEL         0 //1.4

//**************************************************************************
//  [Function Name]:
//                  api_CEC_init()
//  [Description]:ce
//                  CEC initial sequence
//  [Arguments]:
//                  [BOOL] *bCECSetConfig: array which to store which CEC set is enabled
//  [Return]:
//                  none
//
//**************************************************************************
void api_CEC_init(WORD bCECSetConfig)
{
    enMsCEC_DEV_LOGIC_ADDR i;
    enMsCEC_SET_INDEX j;

    drv_CEC_EnableSet(bCECSetConfig);
    drv_CEC_Init();

    for ( j = CEC_SET_IDX_0; j < CEC_SET_IDX_MAX; j++ )
    {
        if ( gStCECInfo[j].bIsSetEnabled == TRUE )
        {
            drv_CEC_CheckExistedDevice(j);

            // report power status to the active CEC devices
            for( i = CEC_LA_RECORDER1; i < CEC_LA_UNREGISTERED; i++ )
            {
                if( gStCECInfo[j].bIsDevExisted[i])
                    api_CECMSG_ReportPowerStatus(j, i);
            }
        }
    }
}

//**************************************************************************
//  [Function Name]:
//                  api_CEC_SendMessage()
//  [Description]:
//                  CEC handler
//  [Arguments]:
//                  [enMsCEC_SET_INDEX] enSetIndex: current active CEC set index
//                  [enMsCEC_DEV_LOGIC_ADDR] dst_addr: receiever address
//                  [enMsCEC_OPCODE_LIST] msg: opcode
//                  [BYTE *] data: command content
//                  [BYTE] len:
//  [Return]:
//                  [enMsCEC_INT_EVENT]
//
//**************************************************************************
enMsCEC_INT_EVENT api_CEC_SendMessage(enMsCEC_SET_INDEX enSetIndex, enMsCEC_DEV_LOGIC_ADDR dst_addr, enMsCEC_OPCODE_LIST msg, BYTE* operandPtr, BYTE length)
{
    enMsCEC_INT_EVENT ret;
    ret = drv_CEC_ProcessTX(enSetIndex, dst_addr, msg, operandPtr, length);

    return ret;
}

//**************************************************************************
//  [Function Name]:
//                  api_CECMSG_ReportPowerStatus()
//  [Description]:
//                  get power status of specific logical address
//  [Arguments]:
//                  [enMsCEC_SET_INDEX] enSetIndex: current active CEC set index
//                  [enMsCEC_DEV_LOGIC_ADDR] dst_addr: logical address to be query
//  [Return]:
//                  [enMsCEC_INT_EVENT]
//
//**************************************************************************
enMsCEC_INT_EVENT api_CECMSG_ReportPowerStatus(enMsCEC_SET_INDEX enSetIndex, enMsCEC_DEV_LOGIC_ADDR dst_addr)
{
    enMsCEC_INT_EVENT ret;

    gStCECInfo[enSetIndex].bTxBuf[0] = gStCECInfo[enSetIndex].enMyPwrStatus;
    //ret = drv_CEC_ProcessTX(dst_addr, CEC_OPCODE_REPORT_POWER_STATUS, gStCECInfo.bTxBuf, 1);
    ret = api_CEC_SendMessage(enSetIndex, dst_addr, CEC_OPCODE_REPORT_POWER_STATUS, gStCECInfo[enSetIndex].bTxBuf, 1);

    return ret;
    
}

//**************************************************************************
//  [Function Name]:
//                  api_CECMSG_FeatureAbort()
//  [Description]:
//                  process Feature Abort command
//  [Arguments]:
//                  [enMsCEC_SET_INDEX] enSetIndex: current active CEC set index
//                  [enMsCEC_DEV_LOGIC_ADDR] dst_addr: receiever's address
//                  [enMsCEC_OPCODE_LIST] msg: opcode
//                  [enMsCEC_MSG_ABORT_REASON]: cmd: operand
//  [Return]:
//                  [enMsCEC_INT_EVENT]
//
//**************************************************************************
enMsCEC_INT_EVENT api_CECMSG_FeatureAbort(enMsCEC_SET_INDEX enSetIndex, enMsCEC_DEV_LOGIC_ADDR dst_addr, enMsCEC_OPCODE_LIST msg, enMsCEC_MSG_ABORT_REASON cmd)
{
    enMsCEC_INT_EVENT ret;

    gStCECInfo[enSetIndex].bTxBuf[0] = msg;
    gStCECInfo[enSetIndex].bTxBuf[1] = cmd;

    //ret = drv_CEC_ProcessTX(dst_addr, msg, gStCECInfo.bTxBuf, 2);
    ret = api_CEC_SendMessage(enSetIndex, dst_addr, CEC_OPCODE_FEATURE_ABORT, gStCECInfo[enSetIndex].bTxBuf, 2);
    return ret;

}

//**************************************************************************
//  [Function Name]:
//                  api_CEC_ProcessCommand()
//  [Description]:
//                  handle CEC commands
//  [Arguments]:
//                  [enMsCEC_SET_INDEX] enSetIndex: current active CEC set index
//  [Return]:
//                  none
//
//**************************************************************************
void api_CEC_ProcessCommand(enMsCEC_SET_INDEX enSetIndex)
{
    BYTE header, para0, para1, para2, MsgLen;
    enMsCEC_OPCODE_LIST opcode;

    // no command is received
    if (gStCECInfo[enSetIndex].bMSGCnt == 0)
        return;
    
    //extract message content
    header = gStCECInfo[enSetIndex].stRxBufList[gStCECInfo[enSetIndex].bMSGQStartIdx].bData[0];
    opcode = (BYTE)(gStCECInfo[enSetIndex].stRxBufList[gStCECInfo[enSetIndex].bMSGQStartIdx].bData[1]);
    para0 = gStCECInfo[enSetIndex].stRxBufList[gStCECInfo[enSetIndex].bMSGQStartIdx].bData[2];
    para1 = gStCECInfo[enSetIndex].stRxBufList[gStCECInfo[enSetIndex].bMSGQStartIdx].bData[3];
    para2 = gStCECInfo[enSetIndex].stRxBufList[gStCECInfo[enSetIndex].bMSGQStartIdx].bData[4];
    MsgLen = gStCECInfo[enSetIndex].stRxBufList[gStCECInfo[enSetIndex].bMSGQStartIdx].bLength;

    switch (opcode)
    {
        #if (CECMSG_SUP_ONE_TOUCH_PLAY == 1)
            //----- One Touch Play ----------------------------
            case CEC_OPCODE_ACTIVE_SOURCE:
				gStCECInfo[enSetIndex].bActPhysicalAddr[0] = para0;
				gStCECInfo[enSetIndex].bActPhysicalAddr[1] = para1;
                break;
            case CEC_OPCODE_IMG_VIEW_ON:
                break;
            case CEC_OPCODE_TXT_VIEW_ON:
                break;
        #endif

        #if (CECMSG_SUP_ROUTING_CONTROL == 1)
            //----- Routing Control ---------------------------
            case CEC_OPCODE_INACTIVE_SOURCE:
                break;
            case CEC_OPCODE_REQ_ACTIVE_SOURCE:
                break;
            case CEC_OPCODE_ROUTING_CHANGE:
                break;
            case CEC_OPCODE_ROUTING_INFO:
                break;
            case CEC_OPCODE_SET_STREAM_PATH:
                break;
        #endif

        #if (CECMSG_SUP_STANDBY_COMMAND == 1)
            //----- Standby Command ---------------------------
            case CEC_OPCODE_STANDBY:
                break;
        #endif

        #if (CECMSG_SUP_ONE_TOUCH_RECORD == 1)
            //----- One Touch Record---------------------------
            case CEC_OPCODE_RECORD_ON:
                break;
            case CEC_OPCODE_RECORD_OFF:
                break;
            case CEC_OPCODE_RECORD_STATUS:
                break;
            case CEC_OPCODE_RECORD_TV_SCREEN:
                break;
        #endif

        #if (CECMSG_SUP_TIMER_PROGRAMMER == 1)
            //----- Timer programmer -------------------------- CEC1.3a
            case CEC_OPCODE_CLEAR_ANALOG_TIMER:
                break;
            case CEC_OPCODE_CLEAR_DIGITAL_TIMER:
                break;
            case CEC_OPCODE_CLEAR_EXT_TIMER:
                break;
            case CEC_OPCODE_SET_ANALOG_TIMER:
                break;
            case CEC_OPCODE_SET_DIGITAL_TIMER:
                break;
            case CEC_OPCODE_SET_EXT_TIMER:
                break;
            case CEC_OPCODE_SET_TIMER_PROGRAM_TITLE:
                break;
            case CEC_OPCODE_TIMER_CLEARD_STATUS:
                break;
            case CEC_OPCODE_TIMER_STATUS:
                break;
        #endif

        #if (CECMSG_SUP_SYSTEM_INFORMATION == 1)
            //----- System Information ------------------------
            case CEC_OPCODE_CEC_VER:
                break;
            case CEC_OPCODE_GET_CEC_VER:
                break;
            case CEC_OPCODE_GIVE_PHY_ADDR:
                break;
            case CEC_OPCODE_GET_MENU_LANGUAGE:
                break;
            case CEC_OPCODE_REPORT_PHY_ADDR:
                break;
            case CEC_OPCODE_SET_MENU_LANGUAGE:
                break;  
        #endif

        #if (CECMSG_SUP_DECK_CONTROL == 1)
            //----- Deck Control Feature-----------------------
            case CEC_OPCODE_DECK_CTRL:
                break;
            case CEC_OPCODE_DECK_STATUS:
                break;
            case CEC_OPCODE_GIVE_DECK_STATUS:
                break;
            case CEC_OPCODE_PLAY:
                break;
        #endif

        #if (CECMSG_SUP_TUNER_CONTROL == 1)
            //----- Tuner Control ------------------------------
            case CEC_OPCODE_GIVE_TUNER_STATUS:
                break;
            case CEC_OPCODE_SEL_ANALOG_SERVICE:
                break;
            case CEC_OPCODE_SEL_DIGITAL_SERVICE:
                break;
            case CEC_OPCODE_TUNER_DEVICE_STATUS:
                break;
            case CEC_OPCODE_TUNER_STEP_DECREASE:
                break;
            case CEC_OPCODE_TUNER_STEP_INCREASE:
                break;
        #endif

        #if (CECMSG_SUP_VENDOR_SPECIFIC == 1)
            //---------Vendor Specific -------------------------  
            case CEC_OPCODE_DEVICE_VENDOR_ID:
                break;
            case CEC_OPCODE_GIVE_VENDOR_ID:
                break;
            case CEC_OPCODE_VENDOR_COMMAND:
                break;
            case CEC_OPCODE_VENDOR_COMMAND_WITH_ID:
                break;
            case CEC_OPCODE_VENDOR_RC_BUT_DOWN:
                break;
            case CEC_OPCODE_VENDOR_RC_BUT_UP:
                break;
        #endif

        #if (CECMSG_SUP_OSD_DISPLAY == 1)
            //----- OSD Display --------------------------------
            case CEC_OPCODE_SET_OSD_STRING:
                break;
        #endif

        #if (CECMSG_SUP_DEVICE_OSD_NAME_TRANSFER == 1)
            //----- Device OSD Name Transfer  -------------------------
            case CEC_OPCODE_GIVE_OSD_NAME:
                break;
            case CEC_OPCODE_SET_OSD_NAME:
                break;
        #endif

        #if (CECMSG_SUP_DEVICE_MENU_CONTROL == 1)        
            //----- Device Menu Control ------------------------
            case CEC_OPCODE_MENU_REQUEST:
                break;
            case CEC_OPCODE_MENU_STATUS:
                break;
        #endif

        //----- Remote Control Passthrough ----------------
        #if ((CECMSG_SUP_DEVICE_MENU_CONTROL == 1)||(CECMSG_SUP_REMOTE_CONTROL_PASSTHROUGH == 1))
            case CEC_OPCODE_UI_PRESS:
                break;
            case CEC_OPCODE_UI_RELEASE:
                break;
        #endif

        #if (CECMSG_SUP_POWER_STATUS == 1)
            //----- Power Status  ------------------------------
            case CEC_OPCODE_GIVE_POWER_STATUS:
                break;
            case CEC_OPCODE_REPORT_POWER_STATUS:
                break;
        #endif

        #if (CECMSG_SUP_GENERAL_PROTOCAL == 1)
            //----- General Protocal Message ------------------
            //----- Abort Message -----------------------------
            case CEC_OPCODE_ABORT_MESSAGE:
                break;
            //----- Feature Abort -----------------------------
            case CEC_OPCODE_FEATURE_ABORT:
                break;
        #endif

        #if (CECMSG_SUP_SYSTEM_AUDIO_CONTROL == 1)
            //----- System Audio Control ------------------
            case CEC_OPCODE_GIVE_AUDIO_STATUS:
                break;
            case CEC_OPCODE_GIVE_SYSTEM_AUDIO_MODE_STATUS:
                break;
            case CEC_OPCODE_REPORT_AUDIO_STATUS:
                break;
            case CEC_OPCODE_SET_SYSTEM_AUDIO_MODE:
                break;
            case CEC_OPCODE_SYSTEM_AUDIO_MODE_REQUEST:
                break;
            case CEC_OPCODE_SYSTEM_AUDIO_MODE_STATUS:
                break;
            case CEC_OPCODE_SET_AUDIO_RATE:
                break;
        #endif

        #if (CECMSG_SUP_AUDIO_RETURN_CHANNEL == 1)
            //----- Audio Channel Return Control ------------
            case CEC_OPCODE_INIT_ARC:
                break;
            case CEC_OPCODE_REPORT_ARC_INIT:
                break;
            case CEC_OPCODE_REPORT_ARC_TERMINATE:
                break;
            case CEC_OPCODE_REQ_ARC_INIT:
                break;
            case CEC_OPCODE_REQ_ARC_TERMINATE:
                break;
            case CEC_OPCODE_TERMINATE_ARC:
                break;
            case CEC_OPCODE_CDC_MESSAGE:
                break;
        #endif

        default:
            CEC_PRINT_DATA( "\r\nNot Support opCode =%x", opcode );
            if (( header & 0x0F ) != 0x0F ) //broadcast will not respond feature abort
            {
                api_CECMSG_FeatureAbort(enSetIndex, gStCECInfo[enSetIndex].enActLogicAddr, opcode, CEC_AR_UNKNOWN_OPCODE);
            }
        break;
    }

    //update Message Queue index and Message Queue counter
    if ((++gStCECInfo[enSetIndex].bMSGQStartIdx) >= CEC_MSGQ_DEPTH)
        gStCECInfo[enSetIndex].bMSGQStartIdx = 0;
    gStCECInfo[enSetIndex].bMSGCnt--;
    
}

//**************************************************************************
//  [Function Name]:
//                  api_CEC_CheckRxBuffer()
//  [Description]:
//                  check CEC rx buffer content
//  [Arguments]:
//                  [enMsCEC_SET_INDEX] enSetIndex: current active CEC set index
//  [Return]:
//                  none
//
//**************************************************************************
void api_CEC_CheckRxBuffer(enMsCEC_SET_INDEX enSetIndex)
{
    drv_CEC_CheckRxBuffer(enSetIndex);
}

//**************************************************************************
//  [Function Name]:
//                  api_CEC_Handler()
//  [Description]:
//                  CEC handler
//  [Arguments]:
//                  none
//  [Return]:
//                  none
//
//**************************************************************************
void api_CEC_Handler(void)
{
    enMsCEC_SET_INDEX i;

    for ( i = CEC_SET_IDX_0; i < CEC_SET_IDX_MAX; i++ )
    {
        if ( gStCECInfo[i].bIsSetEnabled == TRUE)
        {
            #if (!ENABLE_CEC_INT) // if not enable CEC interrupt, use sw polling to instead of
                    api_CEC_CheckRxBuffer(i);
            #endif

            api_CEC_ProcessCommand(i);
        }
    }
}

//Vendor Specific
enMsCEC_INT_EVENT api_CEC_SendReqActiveSrc(enMsCEC_SET_INDEX enSetIndex)
{
	enMsCEC_INT_EVENT ret;
	
	ret = api_CEC_SendMessage(enSetIndex, CEC_LA_BOARDCAST, CEC_OPCODE_REQ_ACTIVE_SOURCE, gStCECInfo[enSetIndex].bTxBuf, 0);
	return ret;
}

#endif



//***********************************************************************************
// @file: mapi_CEC.h
// @author: MStar Semiconductor Inc.
// @brief: CEC api layer interface
//***********************************************************************************

#ifndef _API_CEC_H_
#define _API_CEC_H_

#if ENABLE_CEC
#include "mdrv_CEC.h"

#ifdef _API_CEC_C_
    #define _API_CEC_DEF_
#else
    #define _API_CEC_DEF_ extern
#endif

//***********************************************************************************
//  Macro and Define
//***********************************************************************************



//***********************************************************************************
//  Type and Structure
//***********************************************************************************



//***********************************************************************************
//  Global Variables
//***********************************************************************************

//***********************************************************************************
//  Function Prototype
//***********************************************************************************

_API_CEC_DEF_ void api_CEC_init(WORD bCECSetConfig);
_API_CEC_DEF_ enMsCEC_INT_EVENT api_CEC_SendMessage(enMsCEC_SET_INDEX enSetIndex, enMsCEC_DEV_LOGIC_ADDR dst_addr, enMsCEC_OPCODE_LIST msg, BYTE* operandPtr, BYTE length);
_API_CEC_DEF_ enMsCEC_INT_EVENT api_CECMSG_ReportPowerStatus(enMsCEC_SET_INDEX enSetIndex, enMsCEC_DEV_LOGIC_ADDR dst_addr);
_API_CEC_DEF_ enMsCEC_INT_EVENT api_CECMSG_FeatureAbort(enMsCEC_SET_INDEX enSetIndex, enMsCEC_DEV_LOGIC_ADDR dst_addr, enMsCEC_OPCODE_LIST msg, enMsCEC_MSG_ABORT_REASON cmd);
_API_CEC_DEF_ void api_CEC_ProcessCommand(enMsCEC_SET_INDEX enSetIndex);
_API_CEC_DEF_ void api_CEC_CheckRxBuffer(enMsCEC_SET_INDEX enSetIndex);
_API_CEC_DEF_ void api_CEC_Handler(void);
_API_CEC_DEF_ enMsCEC_INT_EVENT api_CEC_SendReqActiveSrc(enMsCEC_SET_INDEX enSetIndex);

#endif //#if ENABLE_CEC

#endif //#ifndef _API_CEC_H_



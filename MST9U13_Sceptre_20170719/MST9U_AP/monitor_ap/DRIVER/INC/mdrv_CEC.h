//***********************************************************************************
// @file: mdrv_CEC.h
// @author: MStar Semiconductor Inc.
// @brief: CEC drv layer interface
//***********************************************************************************

#ifndef _DRV_CEC_H_
#define _DRV_CEC_H_

#if ENABLE_CEC
#include "mhal_CEC.h"

#ifdef _DRV_CEC_C_
    #define _DRV_CEC_DEF_
#else
    #define _DRV_CEC_DEF_ extern
#endif

//***********************************************************************************
//  Function Prototype
//***********************************************************************************
_DRV_CEC_DEF_ void drv_CEC_EnableSet(WORD bCECSetConfig);
_DRV_CEC_DEF_ void drv_CEC_Init(void);
_DRV_CEC_DEF_ void drv_CEC_CheckExistedDevice(enMsCEC_SET_INDEX enSetIndex);
_DRV_CEC_DEF_ void drv_CEC_CheckRxBuffer(enMsCEC_SET_INDEX enSetIndex);
_DRV_CEC_DEF_ enMsCEC_INT_EVENT drv_CEC_ProcessTX(enMsCEC_SET_INDEX enSetIndex, enMsCEC_DEV_LOGIC_ADDR dst_addr, enMsCEC_OPCODE_LIST msg, BYTE* operand_ptr, BYTE len);
_DRV_CEC_DEF_ BOOL drv_CEC_CheckFrame(enMsCEC_SET_INDEX enSetIndex, enMsCEC_MSGTRANS_TYPE MsgType, BYTE length );

#endif //#if ENABLE_CEC
#endif //#ifndef _DRV_CEC_H_

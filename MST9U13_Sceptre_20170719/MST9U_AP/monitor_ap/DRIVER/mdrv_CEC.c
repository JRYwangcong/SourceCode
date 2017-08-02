

#if ENABLE_CEC

#define _DRV_CEC_C_
#include "Board.h"
#include "datatype.h"
#include "debug.h"
#include "misc.h"
#include "Mode.h"
#include "UserPrefDef.h"
#include "Global.h"
#include "mStar.h"
//#include "mhal_CEC.c"
#include "mdrv_CEC.h"

#define ENABLE_DRV_CEC_DEBUG 0

#if (ENABLE_DRV_CEC_DEBUG == 1)
#define DRVCEC_PRINT_MSG(str)          printMsg(str)//putstr(str)
#define DRVCEC_PRINT_DATA(str, x)      printData(str, x)//printf(str, x)
#else
#define DRVCEC_PRINT_MSG(str)
#define DRVCEC_PRINT_DATA(str, x)
#endif

//**************************************************************************
//  [Function Name]:
//                  drv_CEC_EnableSet()
//  [Description]:
//                  initialize CEC variables, register settings.
//  [Arguments]:
//                  [enMsCEC_PIN_MAPPING] *enCECPinMap: array which to store GPIO mapping for each CEC set
//  [Return]:
//                  none
//
//**************************************************************************
void drv_CEC_EnableSet(WORD bCECSetConfig)
{
    hal_CEC_EnableSet(bCECSetConfig);
}

//**************************************************************************
//  [Function Name]:
//                  drv_CEC_Init()
//  [Description]:
//                  initialize CEC variables, register settings
//  [Arguments]:
//                  none
//  [Return]:
//                  none
//
//**************************************************************************
void drv_CEC_Init(void)
{
    hal_CEC_Init();
}

//**************************************************************************
//  [Function Name]:
//                  drv_CEC_CheckExistedDevice()
//  [Description]:
//                  check current existed device
//  [Arguments]:
//                  [enMsCEC_SET_INDEX] enSetIndex: current active CEC set index
//  [Return]:
//                  none
//
//**************************************************************************
void drv_CEC_CheckExistedDevice(enMsCEC_SET_INDEX enSetIndex)
{
    hal_CEC_CheckExistedDevice(enSetIndex);
}

//**************************************************************************
//  [Function Name]:
//                  drv_CEC_CheckRxBuffer()
//  [Description]:
//                  check RX buffer
//  [Arguments]:
//                  [enMsCEC_SET_INDEX] enSetIndex: current active CEC set index;
//  [Return]:
//                  none
//
//**************************************************************************
void drv_CEC_CheckRxBuffer(enMsCEC_SET_INDEX enSetIndex) //don't place print message in this function
{
    hal_CEC_CheckRxBuffer(enSetIndex);
}

//**************************************************************************
//  [Function Name]:
//                  drv_CEC_ProcessTX()
//  [Description]:
//                  check RX buffer
//  [Arguments]:
//                  [enMsCEC_SET_INDEX] enSetIndex: current active CEC set index
//                  [enMsCEC_DEV_LOGIC_ADDR] dst_addr:
//                  [enMsCEC_OPCODE_LIST] msg:
//                  [BYTE]* operand_ptr:
//                  [BYTE] len:
//  [Return]:
//                  none
//
//**************************************************************************
enMsCEC_INT_EVENT drv_CEC_ProcessTX(enMsCEC_SET_INDEX enSetIndex, enMsCEC_DEV_LOGIC_ADDR dst_addr, enMsCEC_OPCODE_LIST msg, BYTE* operand_ptr, BYTE len)
{
    return hal_CEC_ProcessTX(enSetIndex, dst_addr, msg, operand_ptr, len);
}

//**************************************************************************
//  [Function Name]:
//                  drv_CEC_CheckFrame()
//  [Description]:
//                  check if frame is valid or not
//  [Arguments]:
//                  [enMsCEC_SET_INDEX] enSetIndex: current active CEC set index;
//                  [enMsCEC_MSGTRANS_TYPE] MsgType: CEC message types;
//                  [BYTE] length: CEC command length, including header and opcode;
//  [Return]:
//                  [BOOL]
//
//**************************************************************************
BOOL drv_CEC_CheckFrame(enMsCEC_SET_INDEX enSetIndex, enMsCEC_MSGTRANS_TYPE MsgType, BYTE length)
{
    return hal_CEC_CheckFrame(enSetIndex, MsgType, length);
}
//**************************************************************************
//  [Function Name]:
//                  drv_CEC_ConfigWakeUp()
//  [Description]:
//                  check RX buffer
//  [Arguments]:
//                  [enMsCEC_SET_INDEX] enSetIndex: current active CEC set index;
//  [Return]:
//                  none
//
//**************************************************************************
void drv_CEC_ConfigWakeUp(enMsCEC_SET_INDEX enSetIndex)
{
    hal_CEC_ConfigWakeUp(enSetIndex);
}

#endif



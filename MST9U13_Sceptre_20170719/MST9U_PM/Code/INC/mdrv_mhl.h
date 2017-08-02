///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   Mdrv_mhl.h
/// @author MStar Semiconductor Inc.
/// @brief  MHL driver Function
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MDRV_MHL_H_
#define _MDRV_MHL_H_

//-------------------------------------------------------------------------------------------------
//  Define
//-------------------------------------------------------------------------------------------------
#define MHL_DRIVER_U01                      0
#define MHL_DRIVER_U02                      1
#define MHL_DRIVER_U03                      2
#define MHL_DRIVER_U04                      3

#define MHL_DRIVER_VERSION                  MHL_DRIVER_U02

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
enum MHL_INPUT_SELECT_TYPE
{
    MHL_INPUT_SELECT_PORT0 = 0,
    MHL_INPUT_SELECT_PORT1,
    MHL_INPUT_SELECT_PORT2,
    MHL_INPUT_SELECT_PORT3,
    MHL_INPUT_SELECT_PORT4,
    MHL_INPUT_SELECT_PORT5,
    MHL_INPUT_SELECT_MASK,
    MHL_INPUT_SELECT_NONE = MHL_INPUT_SELECT_MASK,
};

enum MHL_POWER_STATUS_TYPE
{
    MHL_POWER_STATUS_NONE = 0,
    MHL_POWER_STATUS_ON,
    MHL_POWER_STATUS_DOWN,
    MHL_POWER_STATUS_SAVING,
};

#define GET_MHL_CBUS_CONNECT_PORT(a)        (Bool)((ucMHLCbusConnect & _BIT(a)) ?TRUE :FALSE)
#define SET_MHL_CBUS_CONNECT_PORT(a)        (ucMHLCbusConnect |= _BIT(a))
#define CLR_MHL_CBUS_CONNECT_PORT(a)        (ucMHLCbusConnect &= ~(_BIT(a)))
#define CLR_MHL_CBUS_CONNECT_PORT_ALL()     (ucMHLCbusConnect = 0)

extern BYTE ucMHLCbusConnect;

//-------------------------------------------------------------------------------------------------
//  Function Prototype
//-------------------------------------------------------------------------------------------------
void mdrv_mhl_SetPowerStatus(BYTE ucPowerStatus);
void mdrv_mhl_TimerHandler(void);
Bool mdrv_mhl_CbusWakeupDetect(BYTE ucPortSelect);

#endif //_MDRV_MHL_H_


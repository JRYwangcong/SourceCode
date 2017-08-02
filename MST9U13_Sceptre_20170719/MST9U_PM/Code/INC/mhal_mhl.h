///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   Mdrv_mhl.h
/// @author MStar Semiconductor Inc.
/// @brief  MHL driver Function
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MMHL_MHL_H_
#define _MMHL_MHL_H_

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define _BIT(a)                             (BIT0 << (a))
#define MASKBIT(a)                          (_BIT((1?a) +1) -_BIT((0?a)))
#define MASKSET(a, b, c)                    ((a &~c) | (b &c))
#define ABS_MINUS(a, b)                     ((a > b)? (a -b): (b -a))

#define MHL_SAR_VALTAGE_TARGET              20
#define MHL_CABLE_DETECT_LEVEL              (MHL_SAR_VALTAGE_TARGET *255 /33)

#define MHL_ECBUS_MUX_SWITCH_TIME           300 // 10ms

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef enum
{
    RX_HDMI_RTERM = 0,    // HDMI mode
    RX_MHL_RTERM,    // MHL mode
    RX_RTERM_OFF,    // R-term off
} RXRtermControl_T;

enum MHL_CABLE_DETECT_FLAG_TYPE
{
    MHL_CABLE_DETECT_FLAG_PORT0 = BIT0,
    MHL_CABLE_DETECT_FLAG_PORT1 = BIT1,
    MHL_CABLE_DETECT_FLAG_PORT2 = BIT2,
    MHL_CABLE_DETECT_FLAG_PORT3 = BIT3,
    MHL_CABLE_DETECT_FLAG_PORT4 = BIT4,
    MHL_CABLE_DETECT_FLAG_PORT5 = BIT5,
};

enum MHL_ECBUS_MUX_SELECT_TYPE
{
    MHL_ECBUS_MUX_SELECT_PORT0 = 0,
    MHL_ECBUS_MUX_SELECT_PORT1,
    MHL_ECBUS_MUX_SELECT_PORT2,
    MHL_ECBUS_MUX_SELECT_PORT3,
    MHL_ECBUS_MUX_SELECT_PORT4,
    MHL_ECBUS_MUX_SELECT_NONE,
};

#define GET_MHL_CABLE_DETECT_PORT(a)        (Bool)((ucMHLCableDetect & _BIT(a)) ?TRUE :FALSE)
#define SET_MHL_CABLE_DETECT_PORT(a)        (ucMHLCableDetect |= _BIT(a))
#define CLR_MHL_CABLE_DETECT_PORT(a)        (ucMHLCableDetect &= ~(_BIT(a)))
#define CLR_MHL_CABLE_DETECT_PORT_ALL()     (ucMHLCableDetect = 0)

#define GET_MHL_ECBUS_MUX_SELECT()          (ucECbusMuxSelect)
#define SET_MHL_ECBUS_MUX_SELECT(a)         (ucECbusMuxSelect = a)
#define CLR_MHL_ECBUS_MUX_SELECT()          (ucECbusMuxSelect = MHL_ECBUS_MUX_SELECT_NONE)

#define GET_MHL_MUX_SWITCH_TIMER()          (usECbusMuxSwitchTimer)
#define SET_MHL_MUX_SWITCH_TIMER(a)         (usECbusMuxSwitchTimer = a)
#define CLR_MHL_MUX_SWITCH_TIMER()          (usECbusMuxSwitchTimer = 0)
#define DEC_MHL_MUX_SWITCH_TIMER()          (usECbusMuxSwitchTimer--)

extern BYTE ucMHLCableDetect;
extern BYTE ucECbusMuxSelect;
extern WORD usECbusMuxSwitchTimer;

//-------------------------------------------------------------------------------------------------
//  Function Prototype
//-------------------------------------------------------------------------------------------------
void mhal_mhl_TimerHandler(void);
Bool mhal_mhl_CableDetect(BYTE ucPortSelect);
Bool mhal_mhl_CbusConnectStatus(BYTE ucPortSelect);
void mhal_mhl_ComboRtermControl(BYTE ucPortSelect, RXRtermControl_T ucControl);
void mhal_mhl_RtermControlHWMode(BYTE ucPortSelect, Bool bRtermHW);
void mhal_mhl_SetPowerStatus(BYTE ucPowerStatus);
void mhal_mhl_PMStatePolling(BYTE ucPortSelect);

#endif //_MHAL_MHL_H_


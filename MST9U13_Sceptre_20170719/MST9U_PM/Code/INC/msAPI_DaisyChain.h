#ifndef _MSAPI_DAISYCHAIN_H_
#define _MSAPI_DAISYCHAIN_H_

#ifdef _MSAPI_DAISYCHAIN_C_
#define INTERFACE
#else
#define INTERFACE extern
#endif

#if (CHIP_ID == MST9U3)	
#define IsDC_SDMConnected()		(!hwSDMDP4Pin_Pin)//(KEYPAD_SAR03>((255ul*200)/330)?TRUE:FALSE)
#else
#define IsDC_SDMConnected()		(!hwSDMDP1Pin_Pin)//(KEYPAD_SAR10>((255ul*200)/330)?TRUE:FALSE)
#endif
#define DAISY_CHAIN_SDM_DECOUNCE_TIMES           0x3

extern bit g_SDMCommandFlagForce;

INTERFACE void msPM_DCCmdInit( void );
INTERFACE BOOL msPM_DCSetCmdDcOff( BYTE u8SendData, BYTE* pu8ReplyData );
#if ENABLE_DC_SDM_FORWARD
INTERFACE void msPM_DCSdmForward( void );
#endif

#undef INTERFACE

#endif


#ifndef _MS_VBY1_TO_EDP_H_
#define _MS_VBY1_TO_EDP_H_

#include "hwi2c.h"
#include "hwi2c_debug.h"

#ifdef _MS_VBY1_TO_EDP_C_
#define INTERFACE
#else
#define INTERFACE extern
#endif

INTERFACE void msVTEInit(U8 u8Miic, U8 u8MiicEn);
INTERFACE BYTE msVTEReadByte(WORD u16Reg);
INTERFACE void msVTEWriteByte(WORD u16RegAddr, BYTE u8Value);
INTERFACE void msVTEWrite2Byte(WORD u16RegLoAddr,WORD u16Value);
INTERFACE void msVTESetPanelParams(U16 u16HTT, U16 u16VTT, U16 u16HDE, U16 u16VDE, U16 u16HStart, U16 u16VStart);
INTERFACE void msVTEVXOneOnOff(BOOL bEnable);

#undef INTERFACE

#endif


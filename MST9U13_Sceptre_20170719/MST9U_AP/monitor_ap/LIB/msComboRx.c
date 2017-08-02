/******************************************************************************
 Copyright (c) 2013 MStar Semiconductor, Inc.
 All rights reserved.

 [Module Name]: MsComboRx.c
 [Date]:        06 - March - 2013
 [Comment]:
   Color Adjust subroutines.
 [Reversion History]:
*******************************************************************************/

#define _MSCOMBORX_C_

#include <math.h>
#if 0//nop?
#include <intrins.h>
#endif
#include "datatype.h"
#include "msComboRx.h"
#include "ms_reg.h"


#define MSCOMBORX_LIB_VERSION           0x0001

#define _msRegs        ((unsigned char volatile xdata *) 0x200000) // dram 0x0000, currently no used

extern DWORD OldRWFuncCnt;
#define _msReadByte(u16Addr)                     (OldRWFuncCnt+=u16Addr,OldRWFuncCnt-=u16Addr,(OldRWFuncCnt++))
#define _msWriteByte(u16Addr,u8Value)            (OldRWFuncCnt+=(u16Addr+u8Value),OldRWFuncCnt-=(u16Addr+u8Value),(OldRWFuncCnt++))
//note: write (addr ,addr+1), not (addr, addr-1)
#define _msWrite2Byte(u16Addr,u16Value)          (OldRWFuncCnt+=(u16Addr+u16Value),OldRWFuncCnt-=(u16Addr+u16Value),(OldRWFuncCnt++))
#define _msRead2Byte(u16Addr)                    (OldRWFuncCnt+=u16Addr,OldRWFuncCnt-=u16Addr,(OldRWFuncCnt++))
#define _msWriteByteMask(u16Addr,u8Value,u8Mask) (OldRWFuncCnt+=(u16Addr+u8Value+u8Mask),OldRWFuncCnt-=(u16Addr+u8Value+u8Mask),(OldRWFuncCnt++))


void msComboRx_Init(WORD *u16Arg1, BYTE *u8Arg2)
{
    u16Arg1 = u16Arg1;
    u8Arg2 = u8Arg2;
    // wait update
}

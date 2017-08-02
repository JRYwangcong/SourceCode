/******************************************************************************
 Copyright (c) 2012 MStar Semiconductor, Inc.
 All rights reserved.

 [Module Name]: msEread.h
 [Date]:        09-Nov-2012
 [Comment]:
   Color adjust header file.
 [Reversion History]:
*******************************************************************************/

#include "Board.h"

#ifndef _MSEREAD_H_
#define _MSEREAD_H_

typedef enum
{
#if (CHIP_ID==MST9U3)
    EFUSE_0 = 0x10,
    EFUSE_1 = 0x20,
    EFUSE_BOTH = 0x30,
#else
    EFUSE_0 = 0x00,
    EFUSE_1 = 0x10,
#endif
} EFUSE_SEL;

// Check any valid HDCP key in Efuse
Bool msEread_IsHDCPKeyInEfuse(void);

// Get HDCP key from Efuse
BYTE msEread_GetHDCPKeyFromEfuse(WORD wCount);

// Compare checksum is equal to Efuse or not
Bool msEread_CompareHDCPChecksumInEfuse(WORD u16Value);

// Get ADC bandgap trimming value from Efuse
BYTE msEread_GetADCBandgapTrimValFromEfuse(EFUSE_SEL EFUSE);

// Get public data from Efuse
BYTE msEread_GetDataFromEfuse(EFUSE_SEL eFuse, WORD wCount);

// Set Combo initial value
void msEread_SetComboInitialValue(void);

// Set HDMI initial value
void msEread_SetHDMIInitialValue(void);

// Set MHL initial value
void msEread_SetMHLInitialValue(void);

// Get reserved value
BYTE msEread_GetReseredValue(void);

Bool msEread_LoadKeyViaEfuse(BYTE* HDCP_KEY_TABLE, BYTE* HDCP_BKSV, EFUSE_SEL EFUSE);

void msEread_Init(WORD *u16Arg1, BYTE *u8Arg2);

#endif

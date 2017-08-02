////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef MAPP_GLOBAL_FUNCTION_H
#define MAPP_GLOBAL_FUNCTION_H

#include "datatype.h"
#include "MApp_GlobalSettingSt.h"

/********************************************************************************/
/*                     Macro                    */
/* ******************************************************************************/
#define SECONDS_PER_HALF_MIN    30L
#define SECONDS_PER_MIN         60L
#define SECONDS_PER_HOUR        3600L
#define SECONDS_PER_HALF_HOUR   1800L
#define SECONDS_PER_DAY         86400L
#define SECONDS_PER_30DAYS      (30*SECONDS_PER_DAY)
#define MINS_PER_HOUR           60
#define HOURS_PER_DAY           24

#define XDATA_WIN1_SIZE         0x1000

#define OSD_COUNTRY_SETTING     MApp_GetOSDCountrySetting()

//*************************************************************************
//          Function prototypes
//*************************************************************************
#ifdef MAPP_GLOBAL_FUNCTION_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

INTERFACE U8 MApp_GetNoOfDigit ( U32 u32Data );
INTERFACE void MApp_UlongToU16String ( U32 ulValue, U16 *pArrOutput, S8 NoOfDigit );
INTERFACE void MApp_UlongToU8String ( U32 ulValue, U8 *pArrOutput, S8 NoOfDigit );
INTERFACE void MApp_U8StringToU16String ( U8 *pu8Str, U16 *pu16Str, U8 u8Strlen );

INTERFACE U32 MApp_U8StringToUlong(U8* InputStr, U8 len);

INTERFACE U8 MApp_GetNoOfHexDigit ( U32 u32Data );
INTERFACE void MApp_HexUlongToU16String ( U32 ulValue, U16 *pArrOutput, S8 NoOfDigit );
INTERFACE void MApp_HexUlongToU8String ( U32 ulValue, U8 *pArrOutput, S8 NoOfDigit );

INTERFACE EN_LANGUAGE MApp_GetMenuLanguage(void);
INTERFACE void MApp_SetMenuLanguage(EN_LANGUAGE eLanguage);

#undef INTERFACE
#endif

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

#ifndef _MAPP_ZUI_APICHINESEIME_H
#define _MAPP_ZUI_APICHINESEIME_H

#include "datatype.h"

#ifdef MAPP_ZUI_APICHINESEIME_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

INTERFACE void MApp_ZUI_API_GetPinYinInfoByFistChar(U8 c, U16 *pstart, U16 *pnumber);
INTERFACE void MApp_ZUI_API_GetStringByFirstChar(U8 c, U16 offset, U8 *pstr);
INTERFACE void MApp_ZUI_API_GetWordsInfoByFistChar(U8 c, U16 offset, U16 *start, U16 *number);
INTERFACE U16 MApp_ZUI_API_GetWordsByPinYin(char c, U16 offset, U16 wofs);


#undef INTERFACE

#endif // _MAPP_ZUI_APISTRINGS_H


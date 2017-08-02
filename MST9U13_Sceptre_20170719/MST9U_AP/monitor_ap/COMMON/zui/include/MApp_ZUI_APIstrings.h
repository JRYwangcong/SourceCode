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

#ifndef _MAPP_ZUI_APISTRINGS_H
#define _MAPP_ZUI_APISTRINGS_H

#include "datatype.h"
//#include "msAPI_OSD.h"

#ifdef MAPP_ZUI_APISTRINGS_C
#define INTERFACE
#else
#define INTERFACE extern
#endif


#if (SIZEOF_TCHAR == 1)
/**
 * \def MApp_ZUI_API_Strlen(x)
 * \brief Character indepentdent string processing: get string length
 *
*/
/**
 * \def MApp_ZUI_API_Strcpy(a, b)
 * \brief Character indepentdent string processing: string copy
 *
*/
    #define MApp_ZUI_API_Strlen(x) strlen(x)
    #define MApp_ZUI_API_Strcpy(a, b) strcpy(a, b)
#elif (SIZEOF_TCHAR == 2)
/**
 * \def MApp_ZUI_API_Strlen(x)
 * \brief Character indepentdent string processing: get string length
 *
*/
/**
 * \def MApp_ZUI_API_Strcpy(a, b)
 * \brief Character indepentdent string processing: string copy
 *
*/
    #define MApp_ZUI_API_Strlen(x) msAPI_OSD_u16Strlen(x)
    #define MApp_ZUI_API_Strcpy(a, b) msAPI_OSD_u16Strcpy(a, b)
    #define MApp_ZUI_API_Strcat(a, b) msAPI_OSD_u16Strcat(a, b)
#endif

//INTERFACE U16 MApp_ZUI_API_U16StringLength (LPTSTR str);
//INTERFACE U16 MApp_ZUI_API_U16StringCopy (LPTSTR dest, LPTSTR src);
INTERFACE LPTSTR MApp_ZUI_API_StringBuffU8toU16(LPTSTR pStrDest, U8* pStrSource, U16 u16Length);
INTERFACE LPTSTR MApp_ZUI_API_GetString(U16 idx);
INTERFACE LPTSTR MApp_ZUI_API_GetStringSpecificLanguage(U16 id, U8 u8Language);
INTERFACE LPTSTR MApp_ZUI_API_GetU16String (U16 value);
INTERFACE LPTSTR MApp_ZUI_API_GetU16HexString(U16 value);
INTERFACE LPTSTR MApp_ZUI_API_GetS16SignString (S16 value);
INTERFACE void MApp_ZUI_API_LoadString(U16 id, LPTSTR str);
INTERFACE LPTSTR MApp_ZUI_API_PrintU16String(U8*pu8Fmt,...);
//INTERFACE void MApp_ZUI_API_ConvertStringU8ToU16(U16 * pu16Str, U8 * pu8Str);

#undef INTERFACE

#endif // _MAPP_ZUI_APISTRINGS_H


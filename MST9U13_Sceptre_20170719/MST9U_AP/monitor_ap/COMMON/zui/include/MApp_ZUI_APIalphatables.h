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

#ifndef _MAPP_ZUI_ALPHATABLE_H
#define _MAPP_ZUI_ALPHATABLE_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#ifdef MAPP_ZUI_ALPHATABLE_C
#define INTERFACE
#else
#define INTERFACE extern
#endif


#ifndef _ZUI_INTERNAL_INSIDE_
#error  "This include file is ZUI internal file, cannot be included here!"
#endif


    /**
     * \defgroup alpha_func Window alpha value query
     * @{
     */

#if (ZUI_ENABLE_ALPHATABLE)

INTERFACE U8 MApp_ZUI_API_GetDisableAlpha (HWND hWnd);
INTERFACE U8 MApp_ZUI_API_GetFocusAlpha (HWND hWnd);
INTERFACE U8 MApp_ZUI_API_GetNormalAlpha (HWND hWnd);

#else

//NOTE: if alpha table is disabled, we assume all window is opacity

#define MApp_ZUI_API_GetDisableAlpha(x) 0xFF
#define MApp_ZUI_API_GetFocusAlpha(x) 0xFF
#define MApp_ZUI_API_GetNormalAlpha(x) 0xFF

#endif //ZUI_ENABLE_ALPHATABLE

    /**
     * @}
     */


#undef INTERFACE

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MAPP_ZUI_ALPHATABLE_H */


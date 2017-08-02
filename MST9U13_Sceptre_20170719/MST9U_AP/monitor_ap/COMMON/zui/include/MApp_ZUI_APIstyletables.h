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

#ifndef _MAPP_ZUI_STYLETABLE_H
#define _MAPP_ZUI_STYLETABLE_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#ifdef MAPP_ZUI_STYLETABLE_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

INTERFACE void _MApp_ZUI_API_DefaultOnPaint(HWND hWnd, PAINT_PARAM * param, BOOLEAN bDrawText);
INTERFACE void _MApp_ZUI_API_DrawStyleList(const GRAPHIC_DC * pdc, const RECT * rect, const DRAWSTYLE * style_list);
INTERFACE void _MApp_ZUI_API_DrawStyleListNoText(const GRAPHIC_DC * pdc, const RECT * rect, const DRAWSTYLE * style_list);
INTERFACE U16 _MApp_ZUI_API_FindFirstComponentIndex(HWND hWnd, DRAWSTYLE_TYPE type, DRAWCOMPONENT comp);
INTERFACE void _MApp_ZUI_API_ForceUpdateWindows(HWND aUpdateWindows[],U8 WinCount);
#undef INTERFACE

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MAPP_ZUI_WINDOW_H */


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

#ifndef _MAPP_ZUI_APIDRAW_H
#define _MAPP_ZUI_APIDRAW_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#ifdef MAPP_ZUI_APIGUIDRAW_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

#ifndef _ZUI_INTERNAL_INSIDE_
#error  "This include file is ZUI internal file, cannot be included here!"
#endif

INTERFACE void _MApp_ZUI_API_DrawStyleList(const GRAPHIC_DC * pdc, const RECT * rect, const DRAWSTYLE * style_list);
INTERFACE void _MApp_ZUI_API_DrawComponent(DRAWCOMPONENT component, U16 u16Index, const GRAPHIC_DC * pdc, const RECT * rect);
INTERFACE void _MApp_ZUI_API_DrawDynamicComponent(DRAWCOMPONENT component, const void * param, const GRAPHIC_DC * pdc, const RECT * rect);

#undef INTERFACE

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MAPP_ZUI_APIDRAW_H */

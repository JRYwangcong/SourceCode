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

#define MAPP_ZUI_STYLETABLE_C
#define _ZUI_INTERNAL_INSIDE_ //NOTE: for ZUI internal


///////////////////////////////////////////////////////////////////////////////////////////////////
// Description: tables for GUI window/control layout and styles
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include "MApp_ZUI_Main.h"
#include "MApp_ZUI_APItables.h"
#include "MApp_ZUI_APIstyletables.h"
#include "MApp_ZUI_APIcomponent.h"
#include "MApp_ZUI_APIdraw.h"
#include "MApp_ZUI_APIalphatables.h"

///////////////////////////////////////////////////////////////////////////////
///  private  _MApp_ZUI_API_FindFirstComponentIndex
///  find first component index in a specific drawing style
///
///  @param [in]  hWnd HWND                 window handle
///  @param [in]  type DRAWSTYLE_TYPE         draw type: normal/disable/focus
///  @param [in]  comp DRAWCOMPONENT        draw component: ex: CP_TEXT_OUT
///
///  @return U16      component index, 0xFFFF for not found...
///
///  @author MStar @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////
U16 _MApp_ZUI_API_FindFirstComponentIndex(HWND hWnd, DRAWSTYLE_TYPE type, DRAWCOMPONENT comp)
{
    DRAWSTYLE * style_list = 0;
    if (type == DS_DISABLE)
    {
        style_list = GETDISABLEDRAWSTYLE(hWnd);
    }
    else if (type == DS_FOCUS)
    {
        style_list = GETFOCUSDRAWSTYLE(hWnd);
    }
    else
    {
        style_list = GETNORMALDRAWSTYLE(hWnd);
    }

    if (style_list)
    {
        while(style_list->component != CP_NOON)
        {
            if (style_list->component == comp)
            {
                return style_list->u16Index;
            }
            style_list++;
        }
    }
    return 0xFFFF;
}


///////////////////////////////////////////////////////////////////////////////
///  private  _MApp_ZUI_API_DrawStyleList
///  draw a style array into GRAPHIC_DC (for bank issue, moved to the same as DEFUALT winproc)
///
///  @param [in]  pdc GRAPHIC_DC *    target GRAPHIC_DC wants to paint for
///  @param [in]  rect RECT *    target region wants to draw
///  @param [in]  style_list DRAWSTYLE *    array of style
///
///  This function doesn't return a value
///
///  @author MStar @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////
void _MApp_ZUI_API_DrawStyleList(const GRAPHIC_DC * pdc, const RECT * rect, const DRAWSTYLE * style_list)
{
    if (!style_list)
        return;

    while(style_list->component != CP_NOON)
    {
        _MApp_ZUI_API_DrawComponent((DRAWCOMPONENT)style_list->component, style_list->u16Index, pdc, rect);
        style_list++;
    }
}

///////////////////////////////////////////////////////////////////////////////
///  private  _MApp_ZUI_API_DrawStyleListNoText
///  draw a style array into GRAPHIC_DC (for bank issue, moved to the same as DEFUALT winproc)
///
///  @param [in]  pdc GRAPHIC_DC *    target GRAPHIC_DC wants to paint for
///  @param [in]  rect RECT *    target region wants to draw
///  @param [in]  style_list DRAWSTYLE *    array of style
///
///  This function doesn't return a value
///
///  @author MStar @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////
void _MApp_ZUI_API_DrawStyleListNoText(const GRAPHIC_DC * pdc, const RECT * rect, const DRAWSTYLE * style_list)
{
    if (!style_list)
        return;

    while(style_list->component != CP_NOON)
    {
        if (style_list->component != CP_TEXT_OUT &&
            style_list->component != CP_TEXT_OUT_DYNAMIC)
        {
            _MApp_ZUI_API_DrawComponent((DRAWCOMPONENT)style_list->component, style_list->u16Index, pdc, rect);
        }
        style_list++;
    }
}

///////////////////////////////////////////////////////////////////////////////
///  private  _MApp_ZUI_API_DefaultOnPaint
///  a default window procedure for MSG_PAINT,
///
///  @param [in]       hWnd HWND     window handle we are processing
///  @param [in]       param PAINT_PARAM *     MSG_PAINT drawing parameters
///  @param [in]       bDrawText BOOLEAN            draw background only (no CP_TEXT_OUT and CP_TEXT_OUT_DYNAMIC)
///
///  This function doesn't return a value
///
///  @author MStar @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////
void _MApp_ZUI_API_DefaultOnPaint(HWND hWnd, PAINT_PARAM * param, BOOLEAN bDrawText)
{
    //get buffer GC for offline drawing...
    //const PAINT_PARAM * param = (const PAINT_PARAM*)pMsg->wParam;

    if (bDrawText)
    {
        if (param->bIsDisable)
        {
            param->dc.u8ConstantAlpha = MApp_ZUI_API_GetDisableAlpha(hWnd);
            _MApp_ZUI_API_DrawStyleList(&param->dc, param->rect, GETDISABLEDRAWSTYLE(hWnd));
        }
        else if (param->bIsFocus) //the same focus group
        {
            param->dc.u8ConstantAlpha = MApp_ZUI_API_GetFocusAlpha(hWnd);
            _MApp_ZUI_API_DrawStyleList(&param->dc, param->rect, GETFOCUSDRAWSTYLE(hWnd));
        }
        else
        {
            param->dc.u8ConstantAlpha = MApp_ZUI_API_GetNormalAlpha(hWnd);
            _MApp_ZUI_API_DrawStyleList(&param->dc, param->rect, GETNORMALDRAWSTYLE(hWnd));
        }
    }
    else
    {
        if (param->bIsDisable)
        {
            param->dc.u8ConstantAlpha = MApp_ZUI_API_GetDisableAlpha(hWnd);
            _MApp_ZUI_API_DrawStyleListNoText(&param->dc, param->rect, GETDISABLEDRAWSTYLE(hWnd));
        }
        else if (param->bIsFocus) //the same focus group
        {
            param->dc.u8ConstantAlpha = MApp_ZUI_API_GetFocusAlpha(hWnd);
            _MApp_ZUI_API_DrawStyleListNoText(&param->dc, param->rect, GETFOCUSDRAWSTYLE(hWnd));
        }
        else
        {
            param->dc.u8ConstantAlpha = MApp_ZUI_API_GetNormalAlpha(hWnd);
            _MApp_ZUI_API_DrawStyleListNoText(&param->dc, param->rect, GETNORMALDRAWSTYLE(hWnd));
        }
    }

}

void _MApp_ZUI_API_ForceUpdateWindows(HWND aUpdateWindows[],U8 WinCount)
{
    U32 i;
    PAINT_PARAM paint_param;

    paint_param.dc = *MApp_ZUI_API_GetScreenDC();
    paint_param.dc.u8ConstantAlpha = 0xFF; //disable alpha blending
    //draw necessary windows
    for (i = 0; i < WinCount; i++)
    {

        RECT rect;
        MApp_ZUI_API_GetWindowRect(aUpdateWindows[i], &rect);
        paint_param.rect = &rect;

        paint_param.bIsVisible = TRUE;
        paint_param.bIsDisable = FALSE;
        paint_param.bIsFocus = FALSE;
        MApp_ZUI_API_SendMessage(aUpdateWindows[i], MSG_PAINT, (WPARAM)&paint_param);
    }
}

#undef MAPP_ZUI_STYLETABLE_C


////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (MStar Confidential Information!¡L) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

#define MAPP_ZUI_CTL_SCROLLER_C
#define _ZUI_INTERNAL_INSIDE_ //NOTE: for ZUI internal

//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "datatype.h"
#include "debug.h"
#include "msAPI_OSD.h"
#include "msAPI_Memory.h"
#include "MApp_ZUI_Main.h"
#include "MApp_ZUI_APIcommon.h"
#include "MApp_ZUI_APIstrings.h"
#include "MApp_ZUI_APIwindow.h"
#include "MApp_ZUI_APItables.h"
#include "MApp_ZUI_APIgdi.h"
#include "MApp_ZUI_APIdraw.h"
#include "MApp_ZUI_APIcontrols.h"
#include "MApp_ZUI_APIcomponent.h"
#include "MApp_ZUI_APIalphatables.h"
#include "MApp_ZUI_ACTglobal.h"
#include "OSDcp_Bitmap_EnumIndex.h"

static U16 m_u16Idx = 0;
static U16 m_u16TotalPage = 1;

#define BMP_ICON_HEIGHT 18
#define BMP_ICON_WIDTH  28
#define BOLDER_WIDTH    2

///////////////////////////////////////////////////////////////////////////////
///  public  MApp_ZUI_CTL_RectProgressBarWinProc
///  Window Proc for "Rect progress bar" control,
///     which a round rectangle in background, and use red, purple, red rectangles as progress
///     change value by calling
///
///  @param [in]       hWnd HWND     window handle
///  @param [in]       pMsg PMSG     message
///
///  @return S32 event handler result
///
///  @author MStarSemi @date 2008/5/26
///////////////////////////////////////////////////////////////////////////////
S32 MApp_ZUI_CTL_ScrollerWinProc(HWND hWnd, PMSG pMsg)
{

    // Background
    static  DRAW_RECT _ZUI_TBLSEG _DrawProgressBarBg =
    {
        COLOR_BLACK, //OSD_COLOR RcolorFrom;
        COLOR_BLACK, //OSD_COLOR RcolorTo;
        OSD_GRADIENT_DISABLE, //OSD_GRADIENT eRectGradient;
        0, //OSD_COLOR BroderColor;
        eRectBorder, //RECT_ATTRIB attrib;
        0, //U8 sizeBorder;
        0, //radius
    };

    // Foreground
    static  DRAW_RECT _ZUI_TBLSEG _DrawProgressBarFg =
    {
        0xBFD9FF,//COLOR_LTBLUE, //OSD_COLOR RcolorFrom;
        0xBFD9FF,//COLOR_LTBLUE, //OSD_COLOR RcolorTo;
        OSD_GRADIENT_DISABLE, //OSD_GRADIENT eRectGradient;
        0, //OSD_COLOR BroderColor;
        eRectBorder, //RECT_ATTRIB attrib;
        0, //U8 sizeBorder;
        0, //radius
    };

    // Border
    static  DRAW_RECT_BORDER _ZUI_TBLSEG _DrawProgressBarBorder =
    {
        COLOR_LTGRAY,
        COLOR_LTGRAY,
        BOLDER_WIDTH,
        eSolidLine,
    };

    switch(pMsg->message)
    {
        case MSG_PAINT:
            {
                //get buffer GC for offline drawing...
                PAINT_PARAM * param = (PAINT_PARAM*)pMsg->wParam;

                DRAW_RECT * pDraw = (DRAW_RECT*)_ZUI_MALLOC(sizeof(DRAW_RECT));
                if (pDraw)
                {
                    param->dc.u8ConstantAlpha = MApp_ZUI_API_GetFocusAlpha(hWnd);
                    memcpy(pDraw, &_DrawProgressBarBg, sizeof(DRAW_RECT));
                    _MApp_ZUI_API_DrawDynamicComponent(CP_RECT, pDraw, &param->dc, param->rect);
                    _ZUI_FREE(pDraw);
                }
                else
                {
                    __ASSERT(0);
                }

                pDraw = (DRAW_RECT*)_ZUI_MALLOC(sizeof(DRAW_RECT));
                if (pDraw)
                {
                    RECT rectFg;
                    S16 s16SegHeight = (param->rect->height - (BMP_ICON_HEIGHT+BOLDER_WIDTH)*2) / m_u16TotalPage;
                    if (s16SegHeight<2) // prevent the bar is too small
                        s16SegHeight=2;
                    rectFg.left = param->rect->left + BOLDER_WIDTH*2;
                    rectFg.top = param->rect->top + BOLDER_WIDTH/2 + BMP_ICON_HEIGHT + ((param->rect->height - (BMP_ICON_HEIGHT+BOLDER_WIDTH)*2)*m_u16Idx) / m_u16TotalPage;
                    rectFg.width = param->rect->width - (BOLDER_WIDTH+2)*2;
                    rectFg.height = s16SegHeight;
                    param->dc.u8ConstantAlpha = MApp_ZUI_API_GetFocusAlpha(hWnd);
                    memcpy(pDraw, &_DrawProgressBarFg, sizeof(DRAW_RECT));
                    _MApp_ZUI_API_DrawDynamicComponent(CP_RECT, pDraw, &param->dc, &rectFg);
                    _ZUI_FREE(pDraw);
                }
                else
                {
                    __ASSERT(0);
                }
                // Draw bitmaps
                {
                    DRAW_BITMAP draw_bitmap;
                    RECT rcNew;

                    //draw top
                    RECT_SET(rcNew,
                        param->rect->left+BOLDER_WIDTH/2,
                        param->rect->top,
                        BMP_ICON_WIDTH,
                        BMP_ICON_HEIGHT);
                    draw_bitmap.bSrcColorKey = TRUE;
                    draw_bitmap.srcColorKeyEnd = COLOR_MAGENTA;
                    draw_bitmap.srcColorKeyFrom = COLOR_MAGENTA;
                    draw_bitmap.u16BitmapIndex = E_BMP_SCROLLER_UP;
                    draw_bitmap.u8Constant_Alpha = 0xFF;
                    _MApp_ZUI_API_DrawDynamicComponent(CP_BITMAP, &draw_bitmap, &param->dc, &rcNew);

                    //draw bottom
                    RECT_SET(rcNew,
                        param->rect->left+BOLDER_WIDTH/2,
                        param->rect->top+param->rect->height-BMP_ICON_HEIGHT-BOLDER_WIDTH,
                        BMP_ICON_WIDTH,
                        BMP_ICON_HEIGHT);
                    draw_bitmap.bSrcColorKey = TRUE;
                    draw_bitmap.srcColorKeyEnd = COLOR_MAGENTA;
                    draw_bitmap.srcColorKeyFrom = COLOR_MAGENTA;
                    draw_bitmap.u16BitmapIndex = E_BMP_SCROLLER_DOWN;
                    draw_bitmap.u8Constant_Alpha = 0xFF;
                    _MApp_ZUI_API_DrawDynamicComponent(CP_BITMAP, &draw_bitmap, &param->dc, &rcNew);
                }
                // Draw bolder
                DRAW_RECT_BORDER * pDrawBorder = (DRAW_RECT_BORDER*)_ZUI_MALLOC(sizeof(DRAW_RECT_BORDER));
                if (pDrawBorder)
                {
                    param->dc.u8ConstantAlpha = MApp_ZUI_API_GetFocusAlpha(hWnd);
                    memcpy(pDrawBorder, &_DrawProgressBarBorder, sizeof(DRAW_RECT_BORDER));
                    _MApp_ZUI_API_DrawDynamicComponent(CP_RECT_BORDER, pDrawBorder, &param->dc, param->rect);
                    _ZUI_FREE(pDrawBorder);
                }
                else
                {
                    __ASSERT(0);
                }
            }
            return 0;
        default:
            break;

    }
    return DEFAULTWINPROC(hWnd, pMsg);
}

void MApp_ZUI_CTL_Scroller_SetPageIndex(U16 u16Idx, U16 u16TotalPage)
{
    if(u16Idx > u16TotalPage)
    {
        u16Idx = u16TotalPage;
    }
    m_u16Idx = u16Idx;
    if(u16TotalPage != 0)
    {
        m_u16TotalPage = u16TotalPage;
    }
    MS_DEBUG_MSG(printf("[Scroller] %d/%d\n", m_u16Idx, m_u16TotalPage));
}

#undef MAPP_ZUI_CTL_SCROLLER_C

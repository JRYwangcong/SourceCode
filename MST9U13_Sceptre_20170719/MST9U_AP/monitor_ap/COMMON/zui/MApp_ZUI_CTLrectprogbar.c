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

#define MAPP_ZUI_CTL_RECTPROGBAR_C
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

////////////////////////////////////////////////////
// Rect Progress Bar

/*
    FOCUS:
    NORMAL:
    DISABLED:

    Background (Black)

    +---+
    | [ ] |
    | [ ] |  red
    | [ ] |  ---
    | [ ] |
    | [ ] |  purple
    | [ ] |  ---
    | [ ] |
    | [ ] |  blue
    | [ ] |
    +---+

*/
#define RECT_PROGRESS_BAR_RECT_W 30
#define RECT_PROGRESS_BAR_RECT_H 10
#define RECT_PROGRESS_BAR_LR_BORDER 6
#define RECT_PROGRESS_BAR_RECT_GAP 5

#define RECT_PROGRESS_BAR_RECT_W_2 14
#define RECT_PROGRESS_BAR_RECT_H_2  13
#define RECT_PROGRESS_BAR_LR_BORDER_2 6
#define RECT_PROGRESS_RECTANGLE_GAP_2 2

typedef enum
{
    EN_RECT_COLOR_GRAY,
    EN_RECT_COLOR_GREEN,
} EN_RECTANGLEL_COLOR;


///////////////////////////////////////////////////////////////////////////////
///  private  _MApp_ZUI_CTL_RectProgressBar_DrawRect
///  drawing function for "Rect progress bar" control,
///
///  @param [in]       x U16     X-axis position
///  @param [in]       y U16     Y-axis position
///  @param [in]       percent U16     percent of the progress bar
///  @param [in]       param const PAINT_PARAM *       DC drawing parameter
///
///  @return no result value
///
///  @author MStarSemi @date 2008/5/26
///////////////////////////////////////////////////////////////////////////////
static void _MApp_ZUI_CTL_RectProgressBar_DrawRect(U16 x, U16 y, U16 percent, const PAINT_PARAM * param)
{

    static  DRAW_BITMAP _ZUI_TBLSEG DrawProgressBarRectBlue =
    {
         E_BMP_EQ_BLUE_BAR, //U16 u16BitmapIndex;
         TRUE, //BOOLEAN bSrcColorKey;
         0xFFFFFF, //OSD_COLOR srcColorKeyFrom;
         0xFFFFFF, //OSD_COLOR srcColorKeyEnd;
         255, //U8 u8Constant_Alpha;
    };

    static  DRAW_BITMAP _ZUI_TBLSEG DrawProgressBarRectPurple =
    {
         E_BMP_EQ_PURPLE_BAR, //U16 u16BitmapIndex;
         TRUE, //BOOLEAN bSrcColorKey;
         0xFFFFFF, //OSD_COLOR srcColorKeyFrom;
         0xFFFFFF, //OSD_COLOR srcColorKeyEnd;
         255, //U8 u8Constant_Alpha;
    };

    static  DRAW_BITMAP _ZUI_TBLSEG DrawProgressBarRectRed =
    {
         E_BMP_EQ_RED_BAR, //U16 u16BitmapIndex;
         TRUE, //BOOLEAN bSrcColorKey;
         0xFFFFFF, //OSD_COLOR srcColorKeyFrom;
         0xFFFFFF, //OSD_COLOR srcColorKeyEnd;
         255, //U8 u8Constant_Alpha;
    };

    RECT rect;

    DRAW_BITMAP * pDraw = (DRAW_BITMAP*)_ZUI_MALLOC(sizeof(DRAW_BITMAP));
    if (pDraw)
    {
        RECT_SET(rect, x, y, RECT_PROGRESS_BAR_RECT_W, RECT_PROGRESS_BAR_RECT_H);


        //printf("[]left=%u,percent=%u\n", x, percent);
        if (percent < 33)
        {
            memcpy(pDraw, &DrawProgressBarRectBlue, sizeof(DRAW_BITMAP));
        }
        else if (percent < 66)
        {
            memcpy(pDraw, &DrawProgressBarRectPurple, sizeof(DRAW_BITMAP));
        }
        else
        {
            memcpy(pDraw, &DrawProgressBarRectRed, sizeof(DRAW_BITMAP));
        }

        _MApp_ZUI_API_DrawDynamicComponent(CP_BITMAP, pDraw, &param->dc, &rect);
        _ZUI_FREE(pDraw);
    }
    else
    {
        __ASSERT(0);
    }

}

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
S32 MApp_ZUI_CTL_RectProgressBarWinProc(HWND hWnd, PMSG pMsg)
{

    static  DRAW_RECT _ZUI_TBLSEG _DrawProgressBarBg =
    {
        0x000000, //OSD_COLOR RcolorFrom;
        0x000000, //OSD_COLOR RcolorTo;
        OSD_GRADIENT_DISABLE, //OSD_GRADIENT eRectGradient;
        0, //OSD_COLOR BroderColor;
        eRectBorder, //RECT_ATTRIB attrib;
        0, //U8 sizeBorder;
        0, //radius
    };

    switch(pMsg->message)
    {
        case MSG_PAINT:
            {
                //get buffer GC for offline drawing...
                PAINT_PARAM * param = (PAINT_PARAM*)pMsg->wParam;
                S16 value;
                RECT rect_border;
                S16 total_count, fill_count, i;

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

                value = MApp_ZUI_ACT_GetDynamicValue(hWnd);
                if (value < 0) value = 0;
                if (value > 100) value = 99;
                value++;

                rect_border = *param->rect;
                total_count = rect_border.height / (RECT_PROGRESS_BAR_RECT_H + RECT_PROGRESS_BAR_RECT_GAP);
                fill_count = total_count * value / 100;
                for(i = 0; i < fill_count; i++)
                {
                    _MApp_ZUI_CTL_RectProgressBar_DrawRect(
                        rect_border.left,
                        rect_border.top + rect_border.height - ((i+1) * (RECT_PROGRESS_BAR_RECT_H + RECT_PROGRESS_BAR_RECT_GAP)),
                        (U16)((i * 100) / total_count),
                        param);
                }
#if 0
                rect_border = *param->rect;
                rect_border.top += (rect_border.height - RECT_PROGRESS_BAR_RECT_H);
                rect_border.height = RECT_PROGRESS_BAR_RECT_H;
                left_add = 0;

                //note: we always have at least one ball when value > 0
                _MApp_ZUI_CTL_RectProgressBar_DrawRect(rect_border.left, rect_border.top, 0, param);

                left_add += BALL_PROGRESS_BAR_BALL_W;
                fill_left = (S32)(rect_border.width)*value/100;
                fill_count = fill_left/(BALL_PROGRESS_BAR_BALL_W+BALL_PROGRESS_BAR_BALL_GAP);
                all_count = rect_border.width/(BALL_PROGRESS_BAR_BALL_W+BALL_PROGRESS_BAR_BALL_GAP);
                //printf("[]count=%u,%u\n", (U16)fill_count, (U16)all_count);

                for (i = 1; i < fill_count; i++)
                {
                    left_add += (rect_border.width-left_add-(all_count-i)*(BALL_PROGRESS_BAR_BALL_W+BALL_PROGRESS_BAR_BALL_GAP)) / (all_count-i); //additional gap, when width can not be divided by BALL_PROGRESS_BAR_BALL_W+BALL_PROGRESS_BAR_BALL_GAP
                    left_add += BALL_PROGRESS_BAR_BALL_GAP;
                    _MApp_ZUI_CTL_BallProgressBar_DrawBall(
                        rect_border.left+left_add,
                        rect_border.top,
                        (S32)(left_add+BALL_PROGRESS_BAR_BALL_W/2)*100/rect_border.width, param);
                    left_add += BALL_PROGRESS_BAR_BALL_W;
                }
#endif
            }
            return 0;
        default:
            break;

    }
    return DEFAULTWINPROC(hWnd, pMsg);
}

static void _MApp_ZUI_CTL_RectProgressBar_DrawRect_2(U16 x, U16 y, const PAINT_PARAM * param, EN_RECTANGLEL_COLOR RectangleColor)
{
    static  DRAW_BITMAP _ZUI_TBLSEG DrawProgressBarRectangleGray =
    {
         //BMPHANDLE handle;
         E_BMP_MENU_IMG_LIST_PROGRESS_NORMAL, //U16 u16BitmapIndex;
         TRUE, //BOOLEAN bSrcColorKey;
         0xFFFFFF, //OSD_COLOR srcColorKeyFrom;
         0xFFFFFF, //OSD_COLOR srcColorKeyEnd;
         255, //U8 u8Constant_Alpha;
    };

    static  DRAW_BITMAP _ZUI_TBLSEG DrawProgressBarRectangleGreen =
    {
         //BMPHANDLE handle;
         E_BMP_MENU_IMG_LIST_PROGRESS, //U16 u16BitmapIndex;
         TRUE, //BOOLEAN bSrcColorKey;
         0xFFFFFF, //OSD_COLOR srcColorKeyFrom;
         0xFFFFFF, //OSD_COLOR srcColorKeyEnd;
         255, //U8 u8Constant_Alpha;
    };

    RECT rect;

    DRAW_BITMAP * pDraw = (DRAW_BITMAP*)_ZUI_MALLOC(sizeof(DRAW_BITMAP));
    if (pDraw)
    {
        RECT_SET(rect, x, y, RECT_PROGRESS_BAR_RECT_W_2, RECT_PROGRESS_BAR_RECT_H_2);
        switch(RectangleColor)
        {
            case EN_RECT_COLOR_GRAY:
                memcpy(pDraw, &DrawProgressBarRectangleGray, sizeof(DRAW_BITMAP));
                break;
            case EN_RECT_COLOR_GREEN:
                memcpy(pDraw, &DrawProgressBarRectangleGreen, sizeof(DRAW_BITMAP));
                break;
        }

        _MApp_ZUI_API_DrawDynamicComponent(CP_BITMAP, pDraw, &param->dc, &rect);
        _ZUI_FREE(pDraw);
    }
    else
    {
        __ASSERT(0);
    }

}

S32 MApp_ZUI_CTL_RectProgressBarWinProc_2(HWND hWnd, PMSG pMsg)
{
    switch(pMsg->message)
    {
        case MSG_PAINT:
            {
                PAINT_PARAM * param = (PAINT_PARAM*)pMsg->wParam;   //get buffer GC for offline drawing...
                S16 value_Bg = MApp_ZUI_ACT_GetDynamicValue_2(hWnd);
                S16 value_Fg = MApp_ZUI_ACT_GetDynamicValue(hWnd);

                if (value_Bg < 0)
                {
                    value_Bg = 0;
                }
                else if (value_Bg > 100)
                {
                    value_Bg = 100;
                }

                if (value_Fg < 0)
                {
                    value_Fg = 0;
                }
                else if (value_Fg > 100)
                {
                    value_Fg = 100;
                }

                if (value_Bg)
                {
                    RECT rect_border;
                    S16 left_add, fill_left, fill_count, all_count, i;
                    rect_border = *param->rect;
                    rect_border.left += RECT_PROGRESS_BAR_LR_BORDER_2;
                    rect_border.width -= RECT_PROGRESS_BAR_LR_BORDER_2*2;
                    rect_border.top += (rect_border.height-RECT_PROGRESS_BAR_RECT_H_2)/2;
                    rect_border.height = RECT_PROGRESS_BAR_RECT_H_2;
                    left_add = 0;

                    //note: we always have at least one ball when value > 0
                    _MApp_ZUI_CTL_RectProgressBar_DrawRect_2(rect_border.left, rect_border.top, param, EN_RECT_COLOR_GRAY);

                    left_add += RECT_PROGRESS_BAR_RECT_W_2;
                    fill_left = (S32)(rect_border.width)*value_Bg/100;
                    fill_count = fill_left/(RECT_PROGRESS_BAR_RECT_W_2+RECT_PROGRESS_RECTANGLE_GAP_2);
                    all_count = rect_border.width/(RECT_PROGRESS_BAR_RECT_W_2+RECT_PROGRESS_RECTANGLE_GAP_2);
                    //printf("[]count=%u,%u\n", (U16)fill_count, (U16)all_count);

                    for (i = 1; i < fill_count; i++)
                    {
                        left_add += (rect_border.width-left_add-(all_count-i)*(RECT_PROGRESS_BAR_RECT_W_2+RECT_PROGRESS_RECTANGLE_GAP_2)) / (all_count-i); //additional gap, when width can not be divided by BALL_PROGRESS_BAR_BALL_W+BALL_PROGRESS_BAR_BALL_GAP
                        left_add += RECT_PROGRESS_RECTANGLE_GAP_2;
                        _MApp_ZUI_CTL_RectProgressBar_DrawRect_2( rect_border.left+left_add, rect_border.top, param, EN_RECT_COLOR_GRAY);
                        left_add += RECT_PROGRESS_BAR_RECT_W_2;

                    }
                }

                if (value_Fg)
                {
                    RECT rect_border;
                    S16 left_add, fill_left, fill_count, all_count, i;
                    rect_border = *param->rect;
                    rect_border.left += RECT_PROGRESS_BAR_LR_BORDER_2;
                    rect_border.width -= RECT_PROGRESS_BAR_LR_BORDER_2*2;
                    rect_border.top += (rect_border.height-RECT_PROGRESS_BAR_RECT_H_2)/2;
                    rect_border.height = RECT_PROGRESS_BAR_RECT_H_2;
                    left_add = 0;

                    //note: we always have at least one ball when value2 > 0
                    _MApp_ZUI_CTL_RectProgressBar_DrawRect_2(rect_border.left, rect_border.top, param,EN_RECT_COLOR_GREEN);

                    left_add += RECT_PROGRESS_BAR_RECT_W_2;
                    fill_left = (S32)(rect_border.width)*value_Fg/100;
                    fill_count = fill_left/(RECT_PROGRESS_BAR_RECT_W_2+RECT_PROGRESS_RECTANGLE_GAP_2);
                    all_count = rect_border.width/(RECT_PROGRESS_BAR_RECT_W_2+RECT_PROGRESS_RECTANGLE_GAP_2);
                    //printf("[]count=%u,%u\n", (U16)fill_count, (U16)all_count);

                    for (i = 1; i < fill_count; i++)
                    {
                        left_add += (rect_border.width-left_add-(all_count-i)*(RECT_PROGRESS_BAR_RECT_W_2+RECT_PROGRESS_RECTANGLE_GAP_2)) / (all_count-i); //additional gap, when width can not be divided by BALL_PROGRESS_BAR_BALL_W+BALL_PROGRESS_BAR_BALL_GAP
                        left_add += RECT_PROGRESS_RECTANGLE_GAP_2;
                        _MApp_ZUI_CTL_RectProgressBar_DrawRect_2(rect_border.left+left_add, rect_border.top, param, EN_RECT_COLOR_GREEN);
                        left_add += RECT_PROGRESS_BAR_RECT_W_2;

                    }
                }

            }
            return 0;
        default:
            break;

    }
    return DEFAULTWINPROC(hWnd, pMsg);
}

#undef MAPP_ZUI_CTL_RECTPROGBAR_C

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

#define MAPP_ZUI_CTL_BALLPROGBARCOLOR_C
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
//#include "MApp_ZUI_APIcontrols.h"
#include "MApp_ZUI_APIgdi.h"
//#include "MApp_ZUI_CTLspin.h"
#include "MApp_ZUI_APIdraw.h"
#include "MApp_ZUI_APIcontrols.h"
#include "MApp_ZUI_APIalphatables.h"
#include "MApp_ZUI_ACTglobal.h"
#include "MApp_ZUI_APIcomponent.h"
#include "OSDcp_Bitmap_EnumIndex.h"



////////////////////////////////////////////////////
// Ball Progress Bar

/*
    FOCUS:
                                                   gradient (y) round rectangle (white)
    /----------------------------------------------\
   |ooooooooooooooooooooooooooooooooooooooooo       |
    \----------------------------------------------/                            (gray)


        blue     |     purple      |     red




    NORMAL:
                                                   gradient (y) round rectangle (light-gray)
    /----------------------------------------------\
   |ooooooooooooooooooooooooooooooooooooooooo       |
    \----------------------------------------------/                            (light-gray)


                        gray





    DISABLED:
                                                   gradient (y) round rectangle (dark-gray)
    /----------------------------------------------\
   |ooooooooooooooooooooooooooooooooooooooooo       |
    \----------------------------------------------/                            (dark-gray)


                    gray-hide


*/

#define BALL_PROGRESS_BAR_RADIUS 8
#define BALL_PROGRESS_BAR_BALL_W 12
#define BALL_PROGRESS_BAR_BALL_H 12
#define BALL_PROGRESS_BAR_LR_BORDER 6
#define BALL_PROGRESS_BAR_BALL_GAP 1

static DRAW_BITMAP _ZUI_TBLSEG DrawProgressBarBallBlue =
{
     //BMPHANDLE handle;
     E_BMP_VOLUME_BALL_BLUE, //U16 u16BitmapIndex;
     TRUE, //BOOLEAN bSrcColorKey;
     0xFFFFFF, //OSD_COLOR srcColorKeyFrom;
     0xFFFFFF, //OSD_COLOR srcColorKeyEnd;
     255, //U8 u8Constant_Alpha;
};

static DRAW_BITMAP _ZUI_TBLSEG DrawProgressBarBallPurple =
{
     //BMPHANDLE handle;
     E_BMP_VOLUME_BALL_PURPLE, //U16 u16BitmapIndex;
     TRUE, //BOOLEAN bSrcColorKey;
     0xFFFFFF, //OSD_COLOR srcColorKeyFrom;
     0xFFFFFF, //OSD_COLOR srcColorKeyEnd;
     255, //U8 u8Constant_Alpha;
};

static DRAW_BITMAP _ZUI_TBLSEG DrawProgressBarBallRed =
{
     //BMPHANDLE handle;
     E_BMP_VOLUME_BALL_RED, //U16 u16BitmapIndex;
     TRUE, //BOOLEAN bSrcColorKey;
     0xFFFFFF, //OSD_COLOR srcColorKeyFrom;
     0xFFFFFF, //OSD_COLOR srcColorKeyEnd;
     255, //U8 u8Constant_Alpha;
};

static DRAW_RECT _ZUI_TBLSEG _DrawProgressBarBgFocus =
{
    0xFFFFFF, //OSD_COLOR RcolorFrom;
    0xFFFFFF, //OSD_COLOR RcolorTo;
    OSD_GRADIENT_DISABLE, //OSD_GRADIENT eRectGradient;
    0, //OSD_COLOR BroderColor;
    eRectBorderRound, //RECT_ATTRIB attrib;
    BALL_PROGRESS_BAR_RADIUS, //U8 sizeBorder;
    //FALSE, //BOOLEAN bShadow;
    BALL_PROGRESS_BAR_RADIUS,
};

///////////////////////////////////////////////////////////////////////////////
///  private  _MApp_ZUI_ACT_BallProgressBarColor_DrawBall
///  drawing function for "ball progress bar" control,
///
///  @param [in]       x U16     X-axis position
///  @param [in]       y U16     Y-axis position
///  @param [in]       percent U16     percent of the progress bar
///  @param [in]       param const PAINT_PARAM *       DC drawing parameter
///
///  @return no result value
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////

static void _MApp_ZUI_ACT_BallProgressBarColor_DrawBall(U16 x, U16 y, U16 percent, const PAINT_PARAM * param)
{
    //DRAW_BITMAP * pBmp = 0;
    RECT rect;

    //2007/12/22: for bank issue, we prepare it in XDATA
    DRAW_BITMAP * pDraw = (DRAW_BITMAP*)_ZUI_MALLOC(sizeof(DRAW_BITMAP));
    if (pDraw)
    {

        RECT_SET(rect, x, y, BALL_PROGRESS_BAR_BALL_W, BALL_PROGRESS_BAR_BALL_H);

        //printf("[]left=%u,percent=%u\n", x, percent);
        if (percent < 33)
        {
            memcpy(pDraw, &DrawProgressBarBallBlue, sizeof(DRAW_BITMAP)); //pBmp = &DrawProgressBarBallBlue;
        }
        else if (percent < 66)
        {
            memcpy(pDraw, &DrawProgressBarBallPurple, sizeof(DRAW_BITMAP)); //pBmp = &DrawProgressBarBallPurple;
        }
        else
        {
            memcpy(pDraw, &DrawProgressBarBallRed, sizeof(DRAW_BITMAP)); //pBmp = &DrawProgressBarBallRed;
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
///  public  MApp_ZUI_ACT_BallProgressBarWinProc
///  Window Proc for "ball progress bar" control,
///     which a round rectangle in background, and use red, purple, red balls as progress
///     change value by calling
///
///  @param [in]       hWnd HWND     window handle
///  @param [in]       pMsg PMSG     message
///
///  @return S32 event handler result
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////

S32 MApp_ZUI_ACT_BallProgressBarColorWinProc(HWND hWnd, PMSG pMsg)
{
    switch(pMsg->message)
    {
        case MSG_PAINT:
            {
                //get buffer GC for offline drawing...
                PAINT_PARAM * param = (PAINT_PARAM*)pMsg->wParam;
                S16 value;

                //2007/12/22: for bank issue, we prepare it in XDATA
                DRAW_RECT * pDraw = (DRAW_RECT*)_ZUI_MALLOC(sizeof(DRAW_RECT));
                if (pDraw)
                {

                    //find all static text => dynamic text
                    {
                        param->dc.u8ConstantAlpha = MApp_ZUI_API_GetFocusAlpha(hWnd);
                        memcpy(pDraw, &_DrawProgressBarBgFocus, sizeof(DRAW_RECT));
                    }
                    _MApp_ZUI_API_DrawDynamicComponent(CP_RECT, pDraw, &param->dc, param->rect);
                    _ZUI_FREE(pDraw);
                }
                else
                {
                    __ASSERT(0);
                }

                value = MApp_ZUI_ACT_GetDynamicValue(hWnd);
                if (value < 0) value = 0;
                if (value > 100) value = 100;

                if (value)
                {
                    RECT rect_border;
                    S16 left_add, fill_left, fill_count, all_count, i;
                    rect_border = *param->rect;
                    rect_border.left += BALL_PROGRESS_BAR_LR_BORDER;
                    rect_border.width -= BALL_PROGRESS_BAR_LR_BORDER*2;
                    rect_border.top += (rect_border.height-BALL_PROGRESS_BAR_BALL_H)/2;
                    rect_border.height = BALL_PROGRESS_BAR_BALL_H;
                    left_add = 0;

                    //note: we always have at least one ball when value > 0
                    _MApp_ZUI_ACT_BallProgressBarColor_DrawBall(rect_border.left, rect_border.top, 0, param);

                    left_add += BALL_PROGRESS_BAR_BALL_W;
                    fill_left = (S32)(rect_border.width)*value/100;
                    fill_count = fill_left/(BALL_PROGRESS_BAR_BALL_W+BALL_PROGRESS_BAR_BALL_GAP);
                    all_count = rect_border.width/(BALL_PROGRESS_BAR_BALL_W+BALL_PROGRESS_BAR_BALL_GAP);
                    //printf("[]count=%u,%u\n", (U16)fill_count, (U16)all_count);

                    for (i = 1; i < fill_count; i++)
                    {
                        left_add += (rect_border.width-left_add-(all_count-i)*(BALL_PROGRESS_BAR_BALL_W+BALL_PROGRESS_BAR_BALL_GAP)) / (all_count-i); //additional gap, when width can not be divided by BALL_PROGRESS_BAR_BALL_W+BALL_PROGRESS_BAR_BALL_GAP
                        left_add += BALL_PROGRESS_BAR_BALL_GAP;
                        _MApp_ZUI_ACT_BallProgressBarColor_DrawBall(
                            rect_border.left+left_add,
                            rect_border.top,
                            (S32)(left_add+BALL_PROGRESS_BAR_BALL_W/2)*100/rect_border.width, param);
                        left_add += BALL_PROGRESS_BAR_BALL_W;

                    }
                }

            }
            return 0;

            default:
            //
            break;
    }
    return DEFAULTWINPROC(hWnd, pMsg);
}

#undef MAPP_ZUI_CTL_BALLPROGBAR_C

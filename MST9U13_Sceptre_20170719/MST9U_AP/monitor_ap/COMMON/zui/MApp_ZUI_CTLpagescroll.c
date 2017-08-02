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

#define MAPP_ZUI_CTL_PAGESCROLL_C
#define _ZUI_INTERNAL_INSIDE_ //NOTE: for ZUI internal


//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "datatype.h"
#include "msAPI_OSD.h"
#include "MApp_ZUI_Main.h"
#include "MApp_ZUI_APIcommon.h"
#include "MApp_ZUI_APIstrings.h"
#include "MApp_ZUI_APIwindow.h"
#include "MApp_ZUI_APItables.h"
#include "MApp_ZUI_APIgdi.h"
#include "MApp_ZUI_APIdraw.h"
#include "MApp_ZUI_APIcontrols.h"
#include "MApp_ZUI_APIalphatables.h"
#include "MApp_ZUI_APIcomponent.h"
#include "MApp_ZUI_ACTglobal.h"
#include "OSDcp_Bitmap_EnumIndex.h"

/*/////////////////////////////////////////////////////////////
// Page scroll for Philip OSD

   +---+ ("scrollbar_background.bmp" not inclued in this control)
   |   |
   |   |
   |   |
   |   |
   |   |
   |   |
   |   |
   |   |
   |   |
   |   |
   |+-+| ("scrollbar_pointer_top.bmp") 6x4 -> 8x4
   || ||
   || || ("scrollbar_pointer_middle.bmp") 6x1 -> 8x1
   || ||
   |+-+| ("scrollbar_pointer_bottom.bmp") 6x4 -> 8x4
   |   |
   |   |
   |   |
   |   |
   |   |
   |   |
   |   |
   +---+



*/

#define PAGE_SCROLL_IMAGE_HEIGHT1 4
#define PAGE_SCROLL_IMAGE_HEIGHT2 1
#define PAGE_SCROLL_IMAGE_HEIGHT3 4

#define PAGE_SCROLL_BAR_MIN_HEIGHT 60

///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_CTL_PageScrollWinProc
///  Window Proc for "page scroll" control, which shows visible range of a specific dynamic list
///
///  @param [in]       hwnd HWND     window handle
///  @param [in]       pMsg PMSG     message type
///
///  @return S32 message execute result
///
///  @author MStarSemi @date 2007/1/29
///////////////////////////////////////////////////////////////////////////////
S32 MApp_ZUI_CTL_PageScrollWinProc(HWND hwnd, PMSG pMsg)
{
    //WINDOWDATA * win = &g_GUI_WindowList[hwnd];
    GUI_DATA_PAGESCROLL_VARDATA * windata =
        (GUI_DATA_PAGESCROLL_VARDATA*)MApp_ZUI_API_GetWindowData(hwnd);
    if (!windata)
    {
        //PRINT("MApp_ZUI_API_SpinWinProc(%x): data NULL\n", hwnd);
        //ABORT();
        return DEFAULTWINPROC(hwnd, pMsg);
    }

    switch(pMsg->message)
    {

        case MSG_PAINT:
            if (windata->u16TotalItems > 0 &&
                windata->u16PageBeginItem <= windata->u16PageEndItem &&
                windata->u16PageEndItem <= windata->u16TotalItems)
            {
                PAINT_PARAM * param = (PAINT_PARAM*)pMsg->wParam;
                U16 top = (U32)windata->u16PageBeginItem*param->rect->height/windata->u16TotalItems;
                U16 bottom = (U32)(windata->u16PageEndItem+1)*param->rect->height/windata->u16TotalItems - 1;

                //note: if bar legnth is too small, adjust to PAGE_SCROLL_BAR_MIN_HEIGHT
                if (bottom - top < PAGE_SCROLL_BAR_MIN_HEIGHT)
                {
                    top = (U32)windata->u16PageBeginItem*(param->rect->height-PAGE_SCROLL_BAR_MIN_HEIGHT)/windata->u16TotalItems;
                    bottom = (U32)(windata->u16PageEndItem+1)*(param->rect->height-PAGE_SCROLL_BAR_MIN_HEIGHT)/windata->u16TotalItems + PAGE_SCROLL_BAR_MIN_HEIGHT - 1;
                }

                if (param->bIsDisable)
                {
                    param->dc.u8ConstantAlpha = MApp_ZUI_API_GetDisableAlpha(hwnd);
                }
                else if (param->bIsFocus) //the same focus group
                {
                    param->dc.u8ConstantAlpha = MApp_ZUI_API_GetFocusAlpha(hwnd);
                }
                else
                {
                    param->dc.u8ConstantAlpha = MApp_ZUI_API_GetNormalAlpha(hwnd);
                }


                if (param->rect->height >= PAGE_SCROLL_IMAGE_HEIGHT1+PAGE_SCROLL_IMAGE_HEIGHT3)
                {
                    DRAW_BITMAP draw_bitmap;
                    RECT rcNew;

                    //draw top
                    RECT_SET(rcNew,
                        param->rect->left,
                        param->rect->top+top,
                        param->rect->width,
                        PAGE_SCROLL_IMAGE_HEIGHT1);
                    draw_bitmap.bSrcColorKey = TRUE;
                    draw_bitmap.srcColorKeyEnd = BITMAP_COLOR_KEY_RGB8888;
                    draw_bitmap.srcColorKeyFrom = BITMAP_COLOR_KEY_RGB8888;
                    draw_bitmap.u16BitmapIndex = E_BMP_SCROLLBAR_POINTER_TOP;
                    draw_bitmap.u8Constant_Alpha = 0xFF;

                    //printf("[top]%d,%d\n", rcNew.top, rcNew.height);

                    _MApp_ZUI_API_DrawDynamicComponent(CP_BITMAP, &draw_bitmap, &param->dc, &rcNew);
                    rcNew.top += PAGE_SCROLL_IMAGE_HEIGHT1;

                    //draw middle
                    rcNew.height = bottom-top-(PAGE_SCROLL_IMAGE_HEIGHT1+PAGE_SCROLL_IMAGE_HEIGHT3);
                    if (rcNew.height > 0)
                    {
                        draw_bitmap.u16BitmapIndex = E_BMP_SCROLLBAR_POINTER_MIDDLE;

                    //printf("[mid]%d,%d\n", rcNew.top, rcNew.height);

                        _MApp_ZUI_API_DrawDynamicComponent(CP_BITMAP, &draw_bitmap, &param->dc, &rcNew);
                        rcNew.top += rcNew.height;
                    }

                    //draw bottom
                    rcNew.height = PAGE_SCROLL_IMAGE_HEIGHT3;
                    draw_bitmap.u16BitmapIndex = E_BMP_SCROLLBAR_POINTER_BOTTOM;

                    //printf("[bot]%d,%d\n", rcNew.top, rcNew.height);

                    _MApp_ZUI_API_DrawDynamicComponent(CP_BITMAP, &draw_bitmap, &param->dc, &rcNew);

                    return 0;

                }
            }
            return 0;

        default:
            break;
    }

    return DEFAULTWINPROC(hwnd, pMsg);

}

//////////////////////////////////////////////////////////////

void MApp_ZUI_CTL_PageScrollSetRange(HWND hwnd, GUI_DATA_PAGESCROLL_VARDATA * range)
{
    GUI_DATA_PAGESCROLL_VARDATA * windata =
        (GUI_DATA_PAGESCROLL_VARDATA*)MApp_ZUI_API_GetWindowData(hwnd);
    if (!windata)
        return;

    memcpy(windata, range, sizeof(GUI_DATA_PAGESCROLL_VARDATA));
    MApp_ZUI_API_InvalidateWindow(hwnd);
}


#undef MAPP_ZUI_CTL_PAGESCROLL_C

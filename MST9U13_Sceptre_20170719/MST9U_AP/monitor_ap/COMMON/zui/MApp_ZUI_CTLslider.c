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

#define _ZUI_INTERNAL_INSIDE_ //NOTE: for ZUI internal

//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include "datatype.h"
#include "MApp_ZUI_Main.h"
#include "MApp_ZUI_APIcommon.h"
#include "MApp_ZUI_APIstrings.h"
#include "MApp_ZUI_APIwindow.h"
#include "MApp_ZUI_APItables.h"
#include "MApp_ZUI_APIcontrols.h"
#include "MApp_ZUI_APIgdi.h"
#include "MApp_ZUI_APIdraw.h"
#include "MApp_ZUI_APIalphatables.h"
#include "MApp_ZUI_ACTglobal.h"


///////////////////////////////////////////////////////////////////////////////
///  global static  _MApp_ZUI_CTL_SliderPaint
///  paint function for "Slider" control
///
///  @param [in]       hwnd HWND     window handle
///  @param [in]       pMsg PMSG     window message
///
///  This function doesn't return a value
///
///  @author MStarSemi @date 2007/1/29
///////////////////////////////////////////////////////////////////////////////
static void _MApp_ZUI_CTL_SliderPaint(HWND hwnd, PMSG pMsg)
{
    RECT ri, thumb;
    //GRAPHIC_DC * hdc = (GRAPHIC_DC*)pMsg->wParam;
    PAINT_PARAM * param = (PAINT_PARAM *)pMsg->wParam;
    GUI_DATA_SLIDER * windata = (GUI_DATA_SLIDER*)GETWNDDATA(hwnd);

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

    RECT_SET(ri,
        param->rect->left+windata->rcInnerBar.left,
        param->rect->top+windata->rcInnerBar.top,
        windata->rcInnerBar.width,
        windata->rcInnerBar.height);
    //if (ri.width < 0 || ri.height < 0)
    //    return; //illegal size

    _MApp_ZUI_API_DrawStyleList(&param->dc, &ri, windata->pInnerBar);

    if (CTL_GETSLIDERMINVALUE(hwnd) == CTL_GETSLIDERMAXVALUE(hwnd) ||
        CTL_GETSLIDERVALUE(hwnd) < CTL_GETSLIDERMINVALUE(hwnd) ||
        CTL_GETSLIDERVALUE(hwnd) > CTL_GETSLIDERMAXVALUE(hwnd))
        return; //illegal data value

    if (windata->u16FormatFlag & SL_FLAG_VERTICAL)
    {
        //vert slider
        ri.height = ri.height*(CTL_GETSLIDERVALUE(hwnd)-CTL_GETSLIDERMINVALUE(hwnd))/(CTL_GETSLIDERMAXVALUE(hwnd)-CTL_GETSLIDERMINVALUE(hwnd));
        _MApp_ZUI_API_DrawStyleList(&param->dc, &ri, windata->pActiveBar);

        RECT_SET(thumb,
            ri.left+(ri.width/2)-(windata->u16ThumbWidth/2),
            ri.top+ri.height-(windata->u16ThumbHeight/2),
            windata->u16ThumbWidth,
            windata->u16ThumbHeight);
        _MApp_ZUI_API_DrawStyleList(&param->dc, &thumb, windata->pThumb);
    }
    else
    {
        //hor slider
        ri.width = ri.width*(CTL_GETSLIDERVALUE(hwnd)-CTL_GETSLIDERMINVALUE(hwnd))/(CTL_GETSLIDERMAXVALUE(hwnd)-CTL_GETSLIDERMINVALUE(hwnd));
        _MApp_ZUI_API_DrawStyleList(&param->dc, &ri, windata->pActiveBar);

        RECT_SET(thumb,
            ri.left+ri.width-(windata->u16ThumbWidth/2),
            ri.top+(ri.height/2)-(windata->u16ThumbHeight/2),
            windata->u16ThumbWidth,
            windata->u16ThumbHeight);
        _MApp_ZUI_API_DrawStyleList(&param->dc, &thumb, windata->pThumb);
    }
}


///////////////////////////////////////////////////////////////////////////////
///  global static  _MApp_ZUI_CTL_SliderKeyDown
///  key down handler for "Slider" control
///
///  @param [in]       hwnd HWND     window handle
///  @param [in]       pMsg PMSG     message type
///
///  @return S32 message result
///
///  @author MStarSemi @date 2007/1/29
///////////////////////////////////////////////////////////////////////////////
static S32 _MApp_ZUI_CTL_SliderKeyDown(HWND hwnd, PMSG pMsg)
{
    //U16 i;
    GUI_DATA_SLIDER * windata = (GUI_DATA_SLIDER*)GETWNDDATA(hwnd);

    //control focus only if visible, enabled and got focus...
    if (!MApp_ZUI_API_IsWindowEnabled(hwnd)||
        !MApp_ZUI_API_IsWindowVisible(hwnd)||
        MApp_ZUI_API_GetFocus() != hwnd)
    {
        //PRINT("_MApp_ZUI_API_SliderKeyDown: no focus\n");
        return DEFAULTWINPROC(hwnd, pMsg);
    }

    if (CTL_GETSLIDERMINVALUE(hwnd) == CTL_GETSLIDERMAXVALUE(hwnd) ||
        CTL_GETSLIDERVALUE(hwnd) < CTL_GETSLIDERMINVALUE(hwnd) ||
        CTL_GETSLIDERVALUE(hwnd) > CTL_GETSLIDERMAXVALUE(hwnd))
    {
        //printf("_MApp_ZUI_API_SliderKeyDown(): illegal value!");
        CTL_GETSLIDERVALUE(hwnd) = CTL_GETSLIDERMINVALUE(hwnd);
        //return -1; //illegal data value
    }

    switch(pMsg->wParam)
    {
        case VK_LEFT:
            if (windata->u16FormatFlag & SL_FLAG_VERTICAL)
                return DEFAULTWINPROC(hwnd, pMsg);
            CTL_GETSLIDERVALUE(hwnd) = MAX(CTL_GETSLIDERMINVALUE(hwnd), CTL_GETSLIDERVALUE(hwnd)-CTL_GETSLIDERDECVALUE(hwnd));
            break;
        case VK_RIGHT:
            if (windata->u16FormatFlag & SL_FLAG_VERTICAL)
                return DEFAULTWINPROC(hwnd, pMsg);
            CTL_GETSLIDERVALUE(hwnd) = MIN(CTL_GETSLIDERMAXVALUE(hwnd), CTL_GETSLIDERVALUE(hwnd)+CTL_GETSLIDERINCVALUE(hwnd));
            break;
        case VK_UP:
            if (!(windata->u16FormatFlag & SL_FLAG_VERTICAL))
                return DEFAULTWINPROC(hwnd, pMsg);
            CTL_GETSLIDERVALUE(hwnd) = MAX(CTL_GETSLIDERMINVALUE(hwnd), CTL_GETSLIDERVALUE(hwnd)-CTL_GETSLIDERDECVALUE(hwnd));
            break;
        case VK_DOWN:
            if (!(windata->u16FormatFlag & SL_FLAG_VERTICAL))
                return DEFAULTWINPROC(hwnd, pMsg);
            CTL_GETSLIDERVALUE(hwnd) = MIN(CTL_GETSLIDERMAXVALUE(hwnd), CTL_GETSLIDERVALUE(hwnd)+CTL_GETSLIDERINCVALUE(hwnd));
            break;
        default:
            //PRINT("_MApp_ZUI_API_SliderKeyDown: key is not left and right\n");
            return DEFAULTWINPROC(hwnd, pMsg);
    }

    //invoke event...
    //if (windata->OnSelectionChanged)
    //    windata->OnSelectionChanged(hwnd, CTL_GETSLIDERVALUE(hwnd));
    MApp_ZUI_ACT_ExecuteWndAction(hwnd);


    MApp_ZUI_API_InvalidateWindow(hwnd);

    return 0;
}


///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_CTL_SliderWinProc
///  Window Proc for "Slider" control, which select one of a collection of item in the box
///
///  @param [in]       hwnd HWND     window handle
///  @param [in]       pMsg PMSG     message type
///
///  @return S32 message execute result
///
///  @author MStarSemi @date 2007/1/29
///////////////////////////////////////////////////////////////////////////////
S32 MApp_ZUI_CTL_SliderWinProc(HWND hwnd, PMSG pMsg)
{
    GUI_DATA_SLIDER * windata = (GUI_DATA_SLIDER*)GETWNDDATA(hwnd);
    if (!windata)
    {
        //printf("MApp_ZUI_API_SliderWinProc(%x): data NULL\n", hwnd);
        ABORT();
        return DEFAULTWINPROC(hwnd, pMsg);
    }

    switch(pMsg->message)
    {
        //case MSG_CREATE:
        //  break;

        //case MSG_SETFOCUS:
        //  break;

        case MSG_KEYDOWN:
            //override default behavior...
            return _MApp_ZUI_CTL_SliderKeyDown(hwnd, pMsg);

        //case MSG_TIMER:
        //case MSG_DESTROY:
        //  break;

        case MSG_PAINT:
            //draw default at the background...
            DEFAULTWINPROC(hwnd, pMsg);
            //should be visible: if ((win->dwState & WS_VISIBLE))
            {
                _MApp_ZUI_CTL_SliderPaint(hwnd, pMsg);
            }
            return 0;

        default:
            break;
    }

    return DEFAULTWINPROC(hwnd, pMsg);

}

///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_CTL_SliderSetValue
///  Window "Slider" control API: set a value for current selection
///
///  @param [in]       hwnd HWND     window handle
///  @param [in]       value U16     selection value
///
///  @return BOOLEAN success or not
///
///  @author MStarSemi @date 2007/1/29
///////////////////////////////////////////////////////////////////////////////

BOOLEAN MApp_ZUI_CTL_SliderSetValue(HWND hwnd, U16 value)
{
    GUI_DATA_SLIDER * windata = (GUI_DATA_SLIDER*)GETWNDDATA(hwnd);

    if (hwnd != HWND_INVALID)
    {
        if (windata)
        {
            if (value >= CTL_GETSLIDERMINVALUE(hwnd) &&
                value <= CTL_GETSLIDERMAXVALUE(hwnd))
            {
                CTL_GETSLIDERVALUE(hwnd) = value;

                //NOTE: need NOT to invoke OnSelectionChanged event..

                MApp_ZUI_API_InvalidateWindow(hwnd);
                return TRUE;
            }
            return TRUE;
        }
    }
    //PRINT("MApp_ZUI_API_SliderSetValue(%x): error\n", hwnd);
    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_CTL_SliderGetValue
///  Window "Slider" control API: get a value for current selection
///
///  @param [in]       hwnd HWND     window handle
///
///  @return U16 current selection value
///
///  @author MStarSemi @date 2007/1/29
///////////////////////////////////////////////////////////////////////////////

U16 MApp_ZUI_CTL_SliderGetValue(HWND hwnd)
{
    const GUI_DATA_SLIDER * windata = (GUI_DATA_SLIDER*)GETWNDDATA(hwnd);

    if (hwnd != HWND_INVALID)
    {
        if (windata)
        {
            return CTL_GETSLIDERVALUE(hwnd);
        }
    }
    //PRINT("MApp_ZUI_API_SliderGetValue(%x): error\n", hwnd);
    return 0;
}



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

#define MAPP_ZUI_CTL_MARQUEE_C
#define _ZUI_INTERNAL_INSIDE_ //NOTE: for ZUI internal


//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include "stdlib.h"
#include "stdio.h"
#include "datatype.h"
#include "msAPI_OSD.h"
#include "apiXC.h"
#include "apiXC_Adc.h"
#include "MApp_GlobalSettingSt.h"
#include "msAPI_Font.h"
//#include "MApp_Font.h"
//#include "MApp_GlobalVar.h"
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

#define ZUI_MARQUEE_INITIAL_INTERVAL_MS 1500
#define ZUI_MARQUEE_ANIMATION_INTERVAL_MS 500

extern void _MApp_ZUI_API_DefaultOnPaint(HWND hWnd, PAINT_PARAM * param, BOOLEAN bDrawText);
extern U16 _MApp_ZUI_API_FindFirstComponentIndex(HWND hWnd, DRAWSTYLE_TYPE type, DRAWCOMPONENT comp);
extern void _MApp_ZUI_API_ConvertTextComponentToDynamic(U16 u16TextOutIndex, DRAW_TEXT_OUT_DYNAMIC * pComp);

///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_CTL_MarqueeTextWinProc
///  Window Proc for "marquee text" control,
///     which display a static text by clipped string with dynamic color
///     if the string content is more than boundary, it will set a timer for text rotation (once)
///
///     rule: if window data == 0, no marquee animation
///           if window is not focus state, no marquee animation
///           if dynamic string content is shorter than boundary, no marquee animation
///           if pData->u8ShowStartPosition==0xFF, animation disabled
///           if pData->u8ShowStartPosition>=strlen, animation disabled
///
///
///  @param [in]       hWnd HWND     window handle
///  @param [in]       pMsg PMSG     message type
///
///  @return S32 message execute result
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////

S32 MApp_ZUI_CTL_MarqueeTextWinProc(HWND hWnd, PMSG pMsg)
{
    GUI_DATA_MARQUEE_VARDATA * pData =
        (GUI_DATA_MARQUEE_VARDATA*)MApp_ZUI_API_GetWindowData(hWnd);

    switch(pMsg->message)
    {
        case MSG_TIMER:
            if (pData)
            {
                if (pData->u8ShowStartPosition == 0)
                    MApp_ZUI_API_SetTimer(hWnd, pMsg->wParam, ZUI_MARQUEE_ANIMATION_INTERVAL_MS);
                pData->u8ShowStartPosition++;
                MApp_ZUI_API_InvalidateWindow(hWnd);
            }
            return 0;

        case MSG_PAINT:
            {
                //get buffer GC for offline drawing...
                PAINT_PARAM * param = (PAINT_PARAM*)pMsg->wParam;
                DRAWSTYLE_TYPE ds_type = DS_NORMAL;

                if (param->bIsDisable)
                {
                    param->dc.u8ConstantAlpha = MApp_ZUI_API_GetDisableAlpha(hWnd);
                    ds_type = DS_DISABLE;
                }
                else if (param->bIsFocus) //the same focus group
                {
                    param->dc.u8ConstantAlpha = MApp_ZUI_API_GetFocusAlpha(hWnd);
                    ds_type = DS_FOCUS;
                }
                else
                {
                    param->dc.u8ConstantAlpha = MApp_ZUI_API_GetNormalAlpha(hWnd);
                }

                _MApp_ZUI_API_DefaultOnPaint(hWnd, param, FALSE);
                {
                    U16 u16TxtComponentIndex = _MApp_ZUI_API_FindFirstComponentIndex(hWnd, ds_type, CP_TEXT_OUT);
                    LPTSTR pStr = MApp_ZUI_ACT_GetDynamicText(hWnd);
                    if (u16TxtComponentIndex != 0xFFFF && pStr)
                    {
                        DRAW_TEXT_OUT_DYNAMIC dyna;

                        _MApp_ZUI_API_ConvertTextComponentToDynamic(u16TxtComponentIndex, &dyna);
                        dyna.pString = pStr;
                        dyna.TextColor = MApp_ZUI_ACT_GetDynamicColor(hWnd, ds_type, dyna.TextColor);

                        //marquee animation:
                        if (ds_type == DS_FOCUS && pData != NULL &&
                            pData->u8ShowStartPosition != 0xFF)
                        {
                            if (pData->u8ShowStartPosition >= MApp_ZUI_API_Strlen(pStr))
                            {
                                pData->u8ShowStartPosition = 0xFF;
                                MApp_ZUI_API_KillTimer(hWnd, 0);
                            }
                            else if (pData->u8ShowStartPosition == 0)
                            {
                                U16 width;
                                clrBtn1.Fontfmt.flag = dyna.flag;
                                clrBtn1.Fontfmt.ifont_gap = dyna.u8dis;
                                clrBtn1.bStringIndexWidth = CHAR_IDX_2BYTE;
                                width = msAPI_OSD_GetStrWidth(Font[dyna.eSystemFont].fHandle, (U8*)pStr, &clrBtn1);
                                //note: add border for a little truncate case..
                                if (width+BTN_TEXT_GAP*2 <= param->rect->width)
                                {
                                    pData->u8ShowStartPosition = 0xFF;
                                    MApp_ZUI_API_KillTimer(hWnd, 0);
                                }
                            }
                            else
                            {
                                dyna.pString += pData->u8ShowStartPosition;
                                dyna.u8dots = 0; //note: don't show dots for animation..
                            }
                        }
                        else
                        {
                            //note: pData may be shared with others, so don't clear them
                            //      but we need to stop the timer if animation still going
                            MApp_ZUI_API_KillTimer(hWnd, 0);
                        }

                        _MApp_ZUI_API_DrawDynamicComponent(CP_TEXT_OUT_DYNAMIC, &dyna, &param->dc, param->rect);
                    }
                }

            }
            return 0;

            default:
                break;
    }

    return DEFAULTWINPROC(hWnd, pMsg);
}

///////////////////////////////////////////////////////////////
// methods

void MApp_ZUI_CTL_MarqueeTextEnableAnimation(HWND hwnd, BOOLEAN bEnable)
{
    //note: if enable, try to check string length is long enough, and then start animation
    //          if disable, stop and clear to normal status

    if (hwnd != HWND_INVALID)
    {
        GUI_DATA_MARQUEE_VARDATA * pData =
            (GUI_DATA_MARQUEE_VARDATA*)MApp_ZUI_API_GetWindowData(hwnd);

        if (pData == NULL)
            return;

        if (bEnable)
        {
            pData->u8ShowStartPosition = 0;
            MApp_ZUI_API_SetTimer(hwnd, 0, ZUI_MARQUEE_INITIAL_INTERVAL_MS);
        }
        else
        {
            pData->u8ShowStartPosition = 0xFF;
            MApp_ZUI_API_KillTimer(hwnd, 0);
        }
        MApp_ZUI_API_InvalidateWindow(hwnd);
    }
}

#undef MAPP_ZUI_CTL_MARQUEE_C

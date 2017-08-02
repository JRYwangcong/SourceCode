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

#define MAPP_ZUI_CTL_DYNAEPGPUNCTEXT_C
#define _ZUI_INTERNAL_INSIDE_ //NOTE: for ZUI internal


//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include "stdlib.h"
#include "stdio.h"
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
#include "MApp_ZUI_APIcomponent.h"
#include "MApp_ZUI_APIalphatables.h"
#include "MApp_ZUI_ACTglobal.h"

extern void _MApp_ZUI_API_DefaultOnPaint(HWND hWnd, PAINT_PARAM * param, BOOLEAN bDrawText);
extern U16 _MApp_ZUI_API_FindFirstComponentIndex(HWND hWnd, DRAWSTYLE_TYPE type, DRAWCOMPONENT comp);
extern void _MApp_ZUI_API_ConvertTextComponentToPunctuated(U16 u16TextOutIndex, DRAW_PUNCTUATED_DYNAMIC * pComp);

///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_CTL_DynamicEpgPunctuatedTextWinProc
///  Window Proc for "dynamic Epg Punctuated text" control,
///     which dynamic change the content by calling MApp_ZUI_ACT_GetDynamicText()
///     use this control to automatically handle control code
///
///  @param [in]       hWnd HWND     window handle
///  @param [in]       pMsg PMSG     message type
///
///  @return S32 message execute result
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////

S32 MApp_ZUI_CTL_DynamicEpgPunctuatedTextWinProc(HWND hWnd, PMSG pMsg)
{
    switch(pMsg->message)
    {
        case MSG_PAINT:
            {
                //get buffer GC for offline drawing...
                PAINT_PARAM * param = (PAINT_PARAM*)pMsg->wParam;
                //DRAWSTYLE * style_list = 0;
                DRAWSTYLE_TYPE ds_type = DS_NORMAL;

                //find all static text => dynamic text
                if (param->bIsDisable)
                {
                    param->dc.u8ConstantAlpha = MApp_ZUI_API_GetDisableAlpha(hWnd);
                    //style_list = GETDISABLEDRAWSTYLE(hWnd);
                    ds_type = DS_DISABLE;
                }
                else if (param->bIsFocus) //the same focus group
                {
                    param->dc.u8ConstantAlpha = MApp_ZUI_API_GetFocusAlpha(hWnd);
                    //style_list = GETFOCUSDRAWSTYLE(hWnd);
                    ds_type = DS_FOCUS;
                }
                else
                {
                    param->dc.u8ConstantAlpha = MApp_ZUI_API_GetNormalAlpha(hWnd);
                    //style_list = GETNORMALDRAWSTYLE(hWnd);
                }

                //if (style_list)
                {
                    //while(style_list->component != CP_NOON)
                    {
                        _MApp_ZUI_API_DefaultOnPaint(hWnd, param, FALSE);
                        //if (style_list->component == CP_TEXT_OUT)
                        {
                            U16 u16TxtComponentIndex = _MApp_ZUI_API_FindFirstComponentIndex(hWnd, ds_type, CP_TEXT_OUT);
                            LPTSTR pStr = MApp_ZUI_ACT_GetDynamicText(hWnd);
                            if (u16TxtComponentIndex != 0xFFFF && pStr)
                            {
                                DRAW_PUNCTUATED_DYNAMIC dyna;
                                //dyna = (DRAW_PUNCTUATED_DYNAMIC*)_ZUI_MALLOC(sizeof(DRAW_PUNCTUATED_DYNAMIC));
                                //if (dyna)
                                {

                                    _MApp_ZUI_API_ConvertTextComponentToPunctuated(u16TxtComponentIndex, &dyna);
                                    dyna.pString = pStr;
                                    dyna.max_row = MApp_ZUI_API_GetWindowData(hWnd);
                                    _MApp_ZUI_API_DrawDynamicComponent(CP_EPG_PUNCTUATED_DYNAMIC, &dyna, &param->dc, param->rect);
                                    //_ZUI_FREE(dyna);
                                }
                            }
                        }
                        //else
                        //{
                        //    _MApp_ZUI_API_DrawComponent(style_list->component, style_list->u16Index, &param->dc, param->rect);
                        //}
                        //style_list++;
                    }
                }

            }
            return 0;

        default:
            break;

    }
    return DEFAULTWINPROC(hWnd, pMsg);
}

#undef MAPP_ZUI_CTL_DYNAEPGPUNCTEXT_C

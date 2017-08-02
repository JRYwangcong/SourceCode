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

#define MAPP_ZUI_CTL_DYNABMP_C
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
#include "MApp_ZUI_APIalphatables.h"
#include "MApp_ZUI_APIcomponent.h"
#include "MApp_ZUI_ACTglobal.h"

extern U16 _MApp_ZUI_API_FindFirstComponentIndex(HWND hWnd, DRAWSTYLE_TYPE type, DRAWCOMPONENT comp);
extern void _MApp_ZUI_API_ConvertComponentToDynamic(DRAWCOMPONENT comp, U16 u16CompIndex, void * pDraw);

///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_CTL_DynamicBitmapWinProc
///  Window Proc for "dynamic bitmap" control,
///     which dynamic change the content by calling MApp_ZUI_ACT_GetDynamicBitmap()
///
///  @param [in]       hWnd HWND     window handle
///  @param [in]       pMsg PMSG     message type
///
///  @return S32 message execute result
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////

S32 MApp_ZUI_CTL_DynamicBitmapWinProc(HWND hWnd, PMSG pMsg)
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
                        //if (style_list->component == CP_BITMAP)
                        {
                            U16 u16BmpComponentIndex = _MApp_ZUI_API_FindFirstComponentIndex(hWnd, ds_type, CP_BITMAP);
                            U16 u16BmpIndex = MApp_ZUI_ACT_GetDynamicBitmap(hWnd, ds_type);
                            if (u16BmpIndex != 0xFFFF &&
                                u16BmpComponentIndex != 0xFFFF)
                            {
                                DRAW_BITMAP dyna;
                                //dyna = (DRAW_BITMAP*)_ZUI_MALLOC(sizeof(DRAW_BITMAP));
                                //if (dyna)
                                {
                                    //_MApp_ZUI_API_ConvertBitmapComponentToDynamic(style_list->u16Index, dyna);
                                    _MApp_ZUI_API_ConvertComponentToDynamic(CP_BITMAP, u16BmpComponentIndex, &dyna);
                                    dyna.u16BitmapIndex = u16BmpIndex;
                                    _MApp_ZUI_API_DrawDynamicComponent(CP_BITMAP, &dyna, &param->dc, param->rect);
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

#undef MAPP_ZUI_CTL_DYNABMP_C

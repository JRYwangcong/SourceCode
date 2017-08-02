////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (��MStar Confidential Information��) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

#define MAPP_ZUI_CTL_CHECKBOX_C
#define _ZUI_INTERNAL_INSIDE_ //NOTE: for ZUI internal


//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include "stdlib.h"
#include "stdio.h"
#include "datatype.h"
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
#include "MApp_ZUI_APIalphatables.h"
#include "MApp_ZUI_APIcomponent.h"
#include "MApp_ZUI_ACTglobal.h"
#include "OSDcp_Bitmap_EnumIndex.h"

/////////////////////////////////////////////////////

//extern void _MApp_ZUI_API_ConvertBitmapComponentToDynamic(U16 u16CompIndex , DRAW_BITMAP * pComp);
extern U16 _MApp_ZUI_API_FindFirstComponentIndex(HWND hWnd, DRAWSTYLE_TYPE type, DRAWCOMPONENT comp);
extern void _MApp_ZUI_API_ConvertComponentToDynamic(DRAWCOMPONENT comp, U16 u16CompIndex, void * pDraw);

/*/////////////////////////////////////////////////////////////
// checkbox button control for Philip OSD


value = CB_CHECKBOX_BASE+0:
            normal: Checkbox_not_checked_N.bmp
            focus:  Checkbox_not_checked_H.bmp

value = CB_CHECKBOX_BASE+1:
            normal: Checkbox_checked_N.bmp
            focus:  Checkbox_checked_H.bmp

value = CB_CHECKBOX_BASE+2:
            normal: Checkbox_half_checked_N.bmp
            focus:  Checkbox_half_checked_H.bmp

*/

/*/////////////////////////////////////////////////////////////
// radio button control for Philip OSD


value = CB_RADIOBUTTON_BASE+0:
            normal: Radio_not_checked_N.bmp
            focus:  Radio_not_checked_H.bmp

value = CB_RADIOBUTTON_BASE+1:
            normal: Radio_checked_N.bmp
            focus:  Radio_checked_H.bmp


*/

///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_CTL_CheckboxWinProc
///  Window Proc for "Checkbox" control, which show check box style icon from MApp_ZUI_ACT_GetDynamicValue()
///
///  @param [in]       hWnd HWND     window handle
///  @param [in]       pMsg PMSG     message type
///
///  @return S32 message execute result
///
///  @author MStarSemi @date 2007/1/29
///////////////////////////////////////////////////////////////////////////////
S32 MApp_ZUI_CTL_CheckboxWinProc(HWND hWnd, PMSG pMsg)
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



                {
                    S16 s16Value = MApp_ZUI_ACT_GetDynamicValue(hWnd);
                    U16 u16BitmapIndex = _MApp_ZUI_API_FindFirstComponentIndex(hWnd, ds_type, CP_BITMAP);
                    if (u16BitmapIndex != 0xFFFF &&
                        0 <= s16Value && s16Value <= 4) //0, 1, 2 is legal range..
                    {
                        DRAW_BITMAP * dyna;
                        dyna = (DRAW_BITMAP*)_ZUI_MALLOC(sizeof(DRAW_BITMAP));
                        if (dyna)
                        {
                            //_MApp_ZUI_API_ConvertBitmapComponentToDynamic(style_list->u16Index, dyna);
                            _MApp_ZUI_API_ConvertComponentToDynamic(CP_BITMAP, u16BitmapIndex, (void*)dyna);
                            if (ds_type == DS_FOCUS)
                            {
                                switch(s16Value)
                                {
                                    case CB_CHECKBOX_BASE+0:
                                        dyna->u16BitmapIndex = E_BMP_CHECKBOX_NOT_CHECKED_H;
                                        break;
                                    case CB_CHECKBOX_BASE+1:
                                        dyna->u16BitmapIndex = E_BMP_CHECKBOX_CHECKED_H;
                                        break;
                                    case CB_CHECKBOX_BASE+2:
                                        dyna->u16BitmapIndex = E_BMP_CHECKBOX_HALF_CHECKED_H;
                                        break;
                                    case CB_RADIOBUTTON_BASE+0:
                                        dyna->u16BitmapIndex = E_BMP_RADIO_NOT_CHECKED_H;
                                        break;
                                    case CB_RADIOBUTTON_BASE+1:
                                        dyna->u16BitmapIndex = E_BMP_RADIO_CHECKED_H;
                                        break;
                                    default:
                                        break;
                                }
                            }
                            else
                            {
                                switch(s16Value)
                                {
                                    case CB_CHECKBOX_BASE+0:
                                        dyna->u16BitmapIndex = E_BMP_CHECKBOX_NOT_CHECKED_N;
                                        break;
                                    case CB_CHECKBOX_BASE+1:
                                        dyna->u16BitmapIndex = E_BMP_CHECKBOX_CHECKED_N;
                                        break;
                                    case CB_CHECKBOX_BASE+2:
                                        dyna->u16BitmapIndex = E_BMP_CHECKBOX_HALF_CHECKED_N;
                                        break;
                                    case CB_RADIOBUTTON_BASE+0:
                                        dyna->u16BitmapIndex = E_BMP_RADIO_NOT_CHECKED_N;
                                        break;
                                    case CB_RADIOBUTTON_BASE+1:
                                        dyna->u16BitmapIndex = E_BMP_RADIO_CHECKED_N;
                                        break;
                                    default:
                                        break;

                                }
                            }
                            _MApp_ZUI_API_DrawDynamicComponent(CP_BITMAP, dyna, &param->dc, param->rect);
                            _ZUI_FREE(dyna);
                        }
                        else
                        {
                            __ASSERT(0);
                        }
                    }
                }

            }
            return 0;

        default:
            break;
    }

    return DEFAULTWINPROC(hWnd, pMsg);

}

#undef MAPP_ZUI_CTL_CHECKBOX_C



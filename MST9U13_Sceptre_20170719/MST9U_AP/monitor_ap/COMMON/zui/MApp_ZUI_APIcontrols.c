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

#define MAPP_ZUI_APICONTROLS_C
#define _ZUI_INTERNAL_INSIDE_ //NOTE: for ZUI internal


///////////////////////////////////////////////////////////////////////////////////////////////////
// Description: default drawing style and default window proc
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include "MApp_ZUI_Main.h"
#include "MApp_ZUI_APIcommon.h"
#include "MApp_ZUI_APIstrings.h"
#include "MApp_ZUI_APIwindow.h"
#include "MApp_ZUI_APItables.h"
#include "MApp_ZUI_APIgdi.h"
#include "MApp_ZUI_APIdraw.h"
#include "MApp_ZUI_ACTglobal.h"

/////////////////////////////////////////////////////
//NOTE: must be the same bank with table, because of GETNORMALDRAWSTYLE()

#define GUI_CTL_DBG(x) //x

#if ZUI_DBG_RECURSIVE
static BIT _bInDefaultWindowProc = 0;
#endif //ZUI_DBG_RECURSIVE


extern void _MApp_ZUI_API_DefaultOnPaint(HWND hWnd, PAINT_PARAM * param, BOOLEAN bDrawText);

///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_API_DefaultWindowProc
///  defualt window proc function, contains static image, static text
///      built-in navigation and window action behavior
///
///  @param [in]       hWnd HWND     window handle we are processing
///  @param [in]       pMsg PMSG     message type
///
///  @return S32 message execute result
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////
S32 MApp_ZUI_API_DefaultWindowProc (HWND hWnd, PMSG pMsg)
{
    //WINDOWDATA * win;
    //static U16 key;
    //HWND focus;
    //GRAPHIC_DC * pdc;

    //PRINT("\n[Note] enter default winproc\n");

#if ZUI_DBG_RECURSIVE
    if (_bInDefaultWindowProc)
    {
        ZUI_DBG_FAIL(printf("[RECURSIVE]DefaultWindowProc!!\n"));
        ABORT();
        return 0;
    }
    _bInDefaultWindowProc = 1;
#endif //ZUI_DBG_RECURSIVE

    if (hWnd == HWND_INVALID)
    {
        GUI_CTL_DBG(printf("WinProc: HWND_INVALID\n"));
        ABORT();

#if ZUI_DBG_RECURSIVE
        _bInDefaultWindowProc = 0;
#endif //ZUI_DBG_RECURSIVE
        return 0;
    }
    //win = &g_GUI_WindowList[hWnd];
    //printf("[def_winproc]hwnd=%bu,msg=%bu\n", (U8)hWnd, (U8)pMsg->message);

    switch(pMsg->message)
    {
        case MSG_CREATE:
            //initial, so we need not to call MApp_ZUI_API_GetWindowRect()
            //MApp_ZUI_API_InvalidateRect(&GETWNDINITRECT(hWnd));
            MApp_ZUI_API_InvalidateWindow(hWnd);
            break;

        case MSG_KEYDOWN:
            //note: special cases moved to MApp_ZUI_API_DispatchMessage()
            { //general case..
                HWND focus = MApp_ZUI_API_GetFocus();
                BOOLEAN handled = FALSE;

                GUI_CTL_DBG(printf("[def_winproc]focus=%bu\n", (U8)focus));

                if (focus == HWND_INVALID)
                    focus = HWND_MAINFRAME;

                if (GETWNDKEYEVENT(focus))
                {
                    KEYEVENT *keyevent;

                    keyevent = GETWNDKEYEVENT(focus);
                    while (keyevent->u16VirKey != VK_NULL)
                    {
                        if (keyevent->u16VirKey == pMsg->wParam)
                        {
                            handled = MApp_ZUI_ACT_ExecuteWndAction(keyevent->u16ExeActID);
                            //we can allow multiple actions... depend on the last one action..
                        }
                        keyevent++;
                    }
                    if (handled)
                    {
                        return 0;
                    }
                }

                switch(pMsg->wParam)
                {
                    case VK_UP:
                        //MApp_ZUI_API_SetPaneFocus(GETWNDNAVI(focus, NAV_UP));
                        MApp_ZUI_API_SetFocusByNav(focus, NAV_UP);
                        break;
                    case VK_DOWN:
                        //MApp_ZUI_API_SetPaneFocus(GETWNDNAVI(focus, NAV_DOWN));
                        MApp_ZUI_API_SetFocusByNav(focus, NAV_DOWN);
                        break;
                    case VK_RIGHT:
                        //MApp_ZUI_API_SetPaneFocus(GETWNDNAVI(focus, NAV_RIGHT));
                        MApp_ZUI_API_SetFocusByNav(focus, NAV_RIGHT);
                        break;
                    case VK_LEFT:
                        //MApp_ZUI_API_SetPaneFocus(GETWNDNAVI(focus, NAV_LEFT));
                        MApp_ZUI_API_SetFocusByNav(focus, NAV_LEFT);
                        break;
                    /*
                    case VK_SELECT:
                        MApp_ZUI_ACT_ExecuteWndAction(focus, ACTION_SELECT);
                        break;
                    */
                    default:
                        break;
                }

#if ZUI_DBG_RECURSIVE
                _bInDefaultWindowProc = 0;
#endif //ZUI_DBG_RECURSIVE

                return 0;
            }
            break;

        /* //note: don't use Send Message in window proc....
        case MSG_NOTIFY:
            if (GETWNDPARENT(hWnd) != hWnd && GETWNDPARENT(hWnd) != HWND_MAINFRAME)
                return MApp_ZUI_API_SendMessage(GETWNDPARENT(hWnd), pMsg->message, pMsg->wParam);
            break;//not supported key...*/

        case MSG_PAINT:
            {
                //get buffer GC for offline drawing...
                PAINT_PARAM * param = (PAINT_PARAM*)pMsg->wParam;

                _MApp_ZUI_API_DefaultOnPaint(hWnd, param, TRUE);
            }
            break;

        default:
            break;
    }

    //PRINT("\n[Note] leave default winproc\n");

#if ZUI_DBG_RECURSIVE
        _bInDefaultWindowProc = 0;
#endif //ZUI_DBG_RECURSIVE

    return 0;
}

#undef MAPP_ZUI_APICONTROLS_C

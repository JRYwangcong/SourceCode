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

#define MAPP_ZUI_CTL_AUTOCLOSE_C
#define _ZUI_INTERNAL_INSIDE_ //NOTE: for ZUI internal


//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include "stdlib.h"
#include "stdio.h"
#include "datatype.h"
#include "MApp_ZUI_Main.h"
#include "MApp_ZUI_APIcommon.h"
#include "MApp_ZUI_APIwindow.h"
#include "MApp_ZUI_APItables.h"
#include "MApp_ZUI_APIcontrols.h"
#include "MApp_ZUI_ACTglobal.h"
#include "ZUI_exefunc.h"
///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_CTL_AutoCloseWinProc
///  Window Proc for "auto close" control,
///     register a timer when being created.
//      And if timer event notified, then execute EN_EXE_CLOSE_CURRENT_OSD
///
///  @param [in]       hWnd HWND     window handle
///  @param [in]       pMsg PMSG     message type
///
///  @return S32 message execute result
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////

S32 MApp_ZUI_CTL_AutoCloseWinProc(HWND hWnd, PMSG pMsg)
{
    switch(pMsg->message)
    {
        case MSG_CREATE:
#ifdef ATSC_SYSTEM
            if( hWnd == HWND_CHINFO_TRANSPARENT )
            {
                U32 timeout_ms = MApp_ZUI_API_GetWindowData(hWnd);//Get ATV TimeOut

                if (IsDTVInUse() && (enChannelInfoPageType == TYPE_CHANNEL_INFO))
                    timeout_ms = ATSC_CHANNEL_INFO_TIME_OUT_MS;

                if (timeout_ms > 0)
                {
                    //setting AP timeout, auto close
                    MApp_ZUI_API_SetTimer(hWnd, 0, timeout_ms);
                }
            }
            else
#endif
            {
                U32 timeout_ms = MApp_ZUI_API_GetWindowData(hWnd);
                if (timeout_ms > 0)
                {
                    //setting AP timeout, auto close
                    MApp_ZUI_API_SetTimer(hWnd, 0, timeout_ms);
                }
            }
            break;

        case MSG_TIMER:
            {
                //if the time is up, kill the timer and then close AP!
                //maybe timeout on common dialog...MApp_ZUI_API_KillTimer(hwnd, 0);
                MApp_ZUI_ACT_ExecuteWndAction(EN_EXE_CLOSE_CURRENT_OSD);
            }
            break;

        default:
            //
            break;
    }

    return DEFAULTWINPROC(hWnd, pMsg);
}

#undef MAPP_ZUI_CTL_AUTOCLOSE_C

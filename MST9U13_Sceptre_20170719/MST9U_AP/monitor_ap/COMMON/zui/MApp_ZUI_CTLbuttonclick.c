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

#define MAPP_ZUI_CTLBUTTONCLICK_C
#define _ZUI_INTERNAL_INSIDE_ //NOTE: for ZUI internal


//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include "datatype.h"
#include "MApp_ZUI_Main.h"
#include "MApp_ZUI_APIcommon.h"
#include "MApp_ZUI_APIwindow.h"
#include "MApp_ZUI_APItables.h"
#include "MApp_ZUI_APIgdi.h"
#include "MApp_ZUI_APIcontrols.h"
#include "MApp_ZUI_APIdraw.h"
#include "MApp_ZUI_APIcontrols.h"

#include "MApp_ZUI_CTLbuttonclick.h"


#define CTL_BUTTONCLICK_DBG(x) //x

///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_ACT_ButtonAniClickWinProc
///  [MENU application customization] right bottom key icon animation
///
///  @param [in]       hwnd HWND     window handle we are processing
///  @param [in]       msg PMSG     message type
///
///  @return S32 message execute result
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////
S32 MApp_ZUI_CTL_ButtonClickWinProc(HWND hwnd, PMSG msg)
{
    switch(msg->message)
    {
        case MSG_PAINT:
            {
                //if a timer registered, then paint as pressing (disabled state)
                if (MApp_ZUI_API_IsExistTimer(hwnd, 0))
                {
                    PAINT_PARAM * param = (PAINT_PARAM *)msg->wParam;
                    param->bIsDisable = TRUE;
                    DEFAULTWINPROC(hwnd, msg);
                    param->bIsDisable = FALSE;
                    return 0;
                }
            }
            break;

        case MSG_TIMER:
            {
                //if the time is up, kill the timer and then repaint again!
                MApp_ZUI_API_KillTimer(hwnd, 0);
                MApp_ZUI_API_InvalidateWindow(hwnd);
            }
            break;

        default:
            //
            break;

    }

    return DEFAULTWINPROC(hwnd, msg);
}


#undef MAPP_ZUI_CTLBUTTONCLICK_C


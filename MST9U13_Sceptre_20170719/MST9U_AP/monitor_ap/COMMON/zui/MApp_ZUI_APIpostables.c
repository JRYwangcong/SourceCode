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

#define MAPP_ZUI_APIPOSTABLE_C
#define _ZUI_INTERNAL_INSIDE_ //NOTE: for ZUI internal


///////////////////////////////////////////////////////////////////////////////////////////////////
// Description: tables for GUI window/control layout and styles
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include "MApp_ZUI_Main.h"
#include "MApp_ZUI_APItables.h"
#include "MApp_ZUI_APIpostables.h"
#include "MApp_ZUI_APIcomponent.h"
#include "MApp_ZUI_APIdraw.h"

///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_API_GetParent
///  get the hwnd of parent
///
///  @param [in]       child HWND     child window handle
///
///  @return HWND    hwnd of parent
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////
HWND MApp_ZUI_API_GetParent(HWND child)
{
    if (child == HWND_INVALID)
        return HWND_INVALID;

    return GETWNDPARENT(child);
}

///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_API_GetLastSuccessor
///  get the hwnd of last successor
///
///  @param [in]       hwnd HWND     window handle
///
///  @return HWND    hwnd of last successor
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////
HWND MApp_ZUI_API_GetLastSuccessor(HWND hwnd)
{
    if (hwnd == HWND_INVALID)
        return HWND_INVALID;

    return GETWNDLASTSUCCESSOR(hwnd);
}


///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_API_GetWindowInitialRect
///  query window initial position and size
///
///  @param [in]       hwnd HWND     target window handle
///  @param [out]       rect RECT*     window position rectangle
///
///  @return no return value
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////

void MApp_ZUI_API_GetWindowInitialRect(HWND hwnd, RECT * rect)
{
    if (hwnd == HWND_INVALID)
        return;
    *rect = GETWNDINITRECT(hwnd);
}

#undef MAPP_ZUI_APIPOSTABLE_C


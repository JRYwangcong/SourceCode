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

#define MAPP_ZUI_CTL_GRID_C
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
#include "MApp_ZUI_ACTglobal.h"


///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_CTL_DynamicListWinProc
///  Window Proc for "dynamic list" control,
///     which dynamically change the list items by calling MApp_ZUI_ACT_QueryDynamicListItemStatus()
///
///  @param [in]       hWnd HWND     window handle
///  @param [in]       pMsg PMSG     message type
///
///  @return S32 message execute result
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////
S32 MApp_ZUI_CTL_GridWinProc(HWND hWnd, PMSG pMsg)
{
    GUI_DATA_GRID * windata = (GUI_DATA_GRID*) GETWNDDATA(hWnd);

    //no dynamic list setting data...
    if (windata == 0 || windata->pVarData == 0)
        return DEFAULTWINPROC(hWnd, pMsg);

    switch (pMsg->message)
    {
        case MSG_NOTIFY_SETFOCUS:
            {
                HWND hwndFocus = MApp_ZUI_CTL_Grid_MapIndex2Hwnd(hWnd, windata->pVarData->u8IndexInPage);
                if ( hwndFocus == HWND_INVALID )
                    return 0;
                MApp_ZUI_API_SetFocus(hwndFocus);
            }
            return 0;

        case MSG_NOTIFY_KEYDOWN:
            {
                HWND hwndFocus = MApp_ZUI_API_GetFocus();
                if (hwndFocus == HWND_INVALID)
                    return 0;

                U8 u8CountOfPage = (windata->u8Rows * windata->u8Columns);
                U8 u8Index = windata->pVarData->u8CurrentPage * u8CountOfPage + windata->pVarData->u8IndexInPage;
                S8 s8NextIndex = 0;
                switch (pMsg->wParam)
                {
                    case VK_LEFT:
                        s8NextIndex = u8Index - windata->u8Rows;
                        if ( s8NextIndex < windata->u8IndexMin )
                            s8NextIndex = windata->u8IndexMin;
                        break;
                    case VK_UP:
                        s8NextIndex = u8Index - 1;
                        if ( s8NextIndex < windata->u8IndexMin )
                            s8NextIndex = windata->u8IndexMin;
                        break;
                    case VK_RIGHT:
                        s8NextIndex = u8Index + windata->u8Rows;
                        if ( s8NextIndex > windata->u8IndexMax )
                            s8NextIndex = windata->u8IndexMax;
                        break;
                    default:
                    case VK_DOWN:
                        s8NextIndex = u8Index + 1;
                        if ( s8NextIndex > windata->u8IndexMax )
                            s8NextIndex = windata->u8IndexMax;
                        break;
                }

                hwndFocus = MApp_ZUI_CTL_Grid_MapIndex2Hwnd(hWnd, s8NextIndex%u8CountOfPage);
                MApp_ZUI_API_SetFocus(hwndFocus);
                windata->fnSetIndex((U8)s8NextIndex);

                // update var data
                u8Index = windata->fnGetIndex();
                windata->pVarData->u8CurrentPage = u8Index / u8CountOfPage;
                windata->pVarData->u8IndexInPage = u8Index % u8CountOfPage;
            }
            return 0;

        default:
            break;
    }

    return DEFAULTWINPROC(hWnd, pMsg);
}

LPTSTR MApp_ZUI_CTL_GridGetDynamicText(HWND hwndGrid, HWND hwndText)
{
    GUI_DATA_GRID *windata = (GUI_DATA_GRID*) GETWNDDATA(hwndGrid);
    U8 u8Index;

    //no dynamic list setting data...
    if (windata == 0 || windata->fnGetText== 0)
        return 0;

    u8Index = MApp_ZUI_CTL_Grid_MapHwnd2Index(hwndGrid, hwndText);
    U8 u8CountOfPage = (windata->u8Rows * windata->u8Columns);
    u8Index += windata->pVarData->u8CurrentPage * u8CountOfPage;
    return windata->fnGetText(u8Index);
}

S8 MApp_ZUI_CTL_Grid_MapHwnd2Index(HWND hwndGrid, HWND hwndText)
{
    U8 index = 0;
    HWND child, last_succ;
    last_succ = MApp_ZUI_API_GetLastSuccessor(hwndGrid);
    for (child=hwndGrid+1; child<=last_succ; child++)
    {
        if (MApp_ZUI_API_GetParent(child) == hwndGrid)
        {
            if ( (child == hwndText) || MApp_ZUI_API_IsSuccessor(child, hwndText) )
            {
                return index;
            }
            index++;
        }
    }
    return -1;
}

HWND MApp_ZUI_CTL_Grid_MapIndex2Hwnd(HWND hwndGrid, U8 u8Index)
{
    U8 count = 0;
    HWND child, last_succ;

    if(hwndGrid == HWND_INVALID)
    {
        return HWND_INVALID;
    }
    last_succ = MApp_ZUI_API_GetLastSuccessor(hwndGrid);
    for (child=hwndGrid+1; child<=last_succ; child++)
    {
        if (MApp_ZUI_API_GetParent(child) == hwndGrid)
        {
            if ( count == u8Index )
            {
                return child;
            }
            count++;
            if(count == 0xFF)
            {
                return HWND_INVALID;
            }
        }
    }
    return HWND_INVALID;
}

void MApp_ZUI_CTL_Grid_SetGridDimension(HWND hwndGrid, U8 u8Rows, U8 u8Columns)
{
    GUI_DATA_GRID * windata = (GUI_DATA_GRID*) GETWNDDATA(hwndGrid);

    //no dynamic list setting data...
    if (windata == 0 || windata->pVarData == 0)
        return;

    windata->u8Rows = u8Rows;
    windata->u8Columns = u8Columns;

    return;
}

void MApp_ZUI_CTL_Grid_SetIndex(HWND hwndGrid, FN_ZUI_CTL_Grid_GetIndex fnGetIndex, FN_ZUI_CTL_Grid_SetIndex fnSetIndex, U8 u8IndexMin, U8 u8IndexMax)
{
    GUI_DATA_GRID * windata = (GUI_DATA_GRID*) GETWNDDATA(hwndGrid);

    //no dynamic list setting data...
    if (windata == 0 || windata->pVarData == 0)
        return;

    if ( (fnGetIndex == NULL) || (fnSetIndex == NULL) )
        return;

    U8 u8Index = fnGetIndex();
    U8 u8CountOfPage = (windata->u8Rows * windata->u8Columns);
    windata->pVarData->u8CurrentPage = u8Index / u8CountOfPage;
    windata->pVarData->u8IndexInPage = u8Index % u8CountOfPage;

    windata->fnGetIndex = fnGetIndex;
    windata->fnSetIndex = fnSetIndex;
    windata->u8IndexMin = u8IndexMin;
    windata->u8IndexMax = u8IndexMax;
}

void MApp_ZUI_CTL_Grid_SetFnGetTextByIndex(HWND hwndGrid, FN_ZUI_CTL_Grid_GetTextByIndex fnGetText)
{
    GUI_DATA_GRID * windata = (GUI_DATA_GRID*) GETWNDDATA(hwndGrid);

    //no dynamic list setting data...
    if (windata == 0 || windata->pVarData == 0)
        return;

    windata->fnGetText = fnGetText;
}

#undef MAPP_ZUI_CTL_GRID_C

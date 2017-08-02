//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all
// or part of MStar Software is expressly prohibited, unless prior written
// permission has been granted by MStar.
//
// By accessing, browsing and/or using MStar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms ("Terms") and to
// comply with all applicable laws and regulations:
//
// 1. MStar shall retain any and all right, ownership and interest to MStar
//    Software and any modification/derivatives thereof.
//    No right, ownership, or interest to MStar Software and any
//    modification/derivatives thereof is transferred to you under Terms.
//
// 2. You understand that MStar Software might include, incorporate or be
//    supplied together with third party`s software and the use of MStar
//    Software may require additional licenses from third parties.
//    Therefore, you hereby agree it is your sole responsibility to separately
//    obtain any and all third party right and license necessary for your use of
//    such third party`s software.
//
// 3. MStar Software and any modification/derivatives thereof shall be deemed as
//    MStar`s confidential information and you agree to keep MStar`s
//    confidential information in strictest confidence and not disclose to any
//    third party.
//
// 4. MStar Software is provided on an "AS IS" basis without warranties of any
//    kind. Any warranties are hereby expressly disclaimed by MStar, including
//    without limitation, any warranties of merchantability, non-infringement of
//    intellectual property rights, fitness for a particular purpose, error free
//    and in conformity with any international standard.  You agree to waive any
//    claim against MStar for any loss, damage, cost or expense that you may
//    incur related to your use of MStar Software.
//    In no event shall MStar be liable for any direct, indirect, incidental or
//    consequential damages, including without limitation, lost of profit or
//    revenues, lost or damage of data, and unauthorized system use.
//    You agree that this Section 4 shall still apply without being affected
//    even if MStar Software has been modified by MStar in accordance with your
//    request or instruction for your use, except otherwise agreed by both
//    parties in writing.
//
// 5. If requested, MStar may from time to time provide technical supports or
//    services in relation with MStar Software to you for your use of
//    MStar Software in conjunction with your or your customer`s product
//    ("Services").
//    You understand and agree that, except otherwise agreed by both parties in
//    writing, Services are provided on an "AS IS" basis and the warranty
//    disclaimer set forth in Section 4 above shall apply.
//
// 6. Nothing contained herein shall be construed as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitration shall be in Taipei, Taiwan and the language shall
//    be English.
//    The arbitration award shall be final and binding to both parties.
//
//******************************************************************************

#define MAPP_ZUI_ACTIDLE_C
#define _ZUI_INTERNAL_INSIDE_ //NOTE: for ZUI internal


//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include "datatype.h"
#include "MApp_ZUI_Main.h"
#include "MApp_ZUI_APIcommon.h"
#include "MApp_ZUI_APIstrings.h"
#include "MApp_ZUI_APIwindow.h"
#include "ZUI_tables_h.inl"
#include "MApp_ZUI_APIgdi.h"
#include "MApp_ZUI_APIcontrols.h"
#include "MApp_ZUI_APIcomponent.h"
#include "MApp_ZUI_ACTeffect.h"
#include "OSDcp_String_EnumIndex.h"
#include "ZUI_exefunc.h"

#include "MApp_ZUI_ACTidle.h"
#include "MApp_ZUI_ACTglobal.h"
#include "Global.h"
#include "menufunc.h"

extern BOOLEAN _MApp_ZUI_API_AllocateVarData(void);

MS_IDLE_SETTING G_IDLE_SETTING;

/////////////////////////////////////////////////////////////////////

void MApp_ZUI_ACT_AppInit(void)
{
    G_IDLE_SETTING.nBallProgressNum = 0;
}

void MApp_ZUI_ACT_AppShowIDLE(void)
{
    HWND wnd;
    RECT rect;
    E_OSD_ID osd_id = E_OSD_IDLE;

    g_GUI_WindowList = GetWindowListOfOsdTable(osd_id);
    g_GUI_WinDrawStyleList = GetWindowStyleOfOsdTable(osd_id);
    g_GUI_WindowPositionList = GetWindowPositionOfOsdTable(osd_id);
#if ZUI_ENABLE_ALPHATABLE
    g_GUI_WinAlphaDataList = GetWindowAlphaDataOfOsdTable(osd_id);
#endif
    HWND_MAX = GetWndMaxOfOsdTable(osd_id);
    OSDPAGE_BLENDING_ENABLE = IsBlendingEnabledOfOsdTable(osd_id);
    OSDPAGE_BLENDING_VALUE = GetBlendingValueOfOsdTable(osd_id);

    if (!_MApp_ZUI_API_AllocateVarData())
    {
        ZUI_DBG_FAIL(printf("[ZUI]ALLOC\n"));
        ABORT();
        return;
    }

    RECT_SET(rect,
        ZUI_IDLE_XSTART, ZUI_IDLE_YSTART,
        ZUI_IDLE_WIDTH, ZUI_IDLE_HEIGHT);

    if (!MApp_ZUI_API_InitGDI(&rect))
    {
        ZUI_DBG_FAIL(printf("[ZUI]GDIINIT\n"));
        ABORT();
        return;
    }

    for (wnd = 0; wnd < HWND_MAX; wnd++)
    {
        //printf("create msg: %lu\n", (U32)wnd);
        MApp_ZUI_API_SendMessage(wnd, MSG_CREATE, 0);
    }
    MApp_ZUI_API_ShowWindow(HWND_MAINFRAME, SW_HIDE);
    MApp_ZUI_API_ShowWindow(HWND_TRANSPARENT_BG, SW_SHOW);

    #if 0
    MApp_ZUI_API_ShowWindow(HWND_DEFAULT_WNDPROC, SW_SHOW);
    MApp_ZUI_API_ShowWindow(HWND_MARQUEE_WNDPROC, SW_SHOW);
    MApp_ZUI_API_ShowWindow(HWND_DYNAMICTEXT_EFFECT, SW_SHOW);
    MApp_ZUI_API_ShowWindow(HWND_DYNAMICTEXT_WNDPROC, SW_SHOW);
    MApp_ZUI_API_ShowWindow(HWND_DYNAMICPUNCTEXT_WNDPROC, SW_SHOW);
    MApp_ZUI_API_ShowWindow(HWND_ANIMATION_WNDPROC, SW_SHOW);
    MApp_ZUI_API_ShowWindow(HWND_DYNAMICCOLORTEXT_WNDPROC, SW_SHOW);
    MApp_ZUI_API_ShowWindow(HWND_BALLPROGRESSBAR_WNDPROC, SW_SHOW);
    MApp_ZUI_API_ShowWindow(HWND_RECTPROGRESSBAR_WNDPROC, SW_SHOW);
    MApp_ZUI_API_ShowWindow(HWND_DYNAMICLIST_WNDPROC, SW_SHOW);
    MApp_ZUI_API_ShowWindow(HWND_DYNAMICLIST_1, SW_SHOW);
    MApp_ZUI_API_ShowWindow(HWND_DYNAMICLIST_2, SW_SHOW);
    MApp_ZUI_API_ShowWindow(HWND_DYNAMICLIST_3, SW_SHOW);
    MApp_ZUI_API_ShowWindow(HWND_DYNAMICBITMAP_WNDPROC, SW_SHOW);
    MApp_ZUI_API_ShowWindow(HWND_KEYBOARDINPUT_WNDPROC, SW_SHOW);
    MApp_ZUI_API_ShowWindow(HWND_AUTOCLOSE_WNDPROC, SW_SHOW);
    MApp_ZUI_API_ShowWindow(HWND_EFFECT_WNDPROC, SW_SHOW);
    MApp_ZUI_API_ShowWindow(HWND_GRID_WNDPROC, SW_SHOW);
    MApp_ZUI_CTL_Grid_SetIndex(HWND_GRID_WNDPROC, MApp_ZUI_ACT_GetIDLEGridIndex, MApp_ZUI_ACT_SetIDLEGridIndex, 0, 18);
    MApp_ZUI_CTL_Grid_SetFnGetTextByIndex(HWND_GRID_WNDPROC, MApp_ZUI_ACT_GetIDLEStringIDByIndex);
    MApp_ZUI_API_ShowWindow(HWND_RNGGRADIENT1, SW_SHOW);
    MApp_ZUI_API_SetFocus(HWND_DEFAULT_WNDPROC);


    #else

        #if 0 //button click sample
        MApp_ZUI_API_ShowWindow(HWND_BUTTON_CLICK_SAMPLE, SW_SHOW);
        MApp_ZUI_API_SetFocus(HWND_BUTTON_CLICK_SAMPLE);
        #endif

        #if 0 //rotate static sample
        MApp_ZUI_API_ShowWindow(HWND_ROTATE_STATIC_SAMPLE, SW_SHOW);
        MApp_ZUI_API_SetFocus(HWND_ROTATE_STATIC_SAMPLE);
        #endif

    #endif

    MApp_ZUI_ACT_TransitionEffectBegin(EN_EFFMODE_OPEN, E_ZUI_STATE_RUNNING);
}


//////////////////////////////////////////////////////////
// Key Handler

BOOLEAN MApp_ZUI_ACT_HandleIDLEKey(VIRTUAL_KEY_CODE key)
{

    switch(key)
    {
#if 0
        case VK_INPUT_SOURCE:
            printf("VK_INPUT_SOURCE\n");
            while(1);
            break;
#endif
        case VK_UP:
        case VK_DOWN:
        case VK_INPUT_SOURCE:
        case VK_MENU:
        {
            if(!SyncLossState(SrcInputPortM))
            {
                MApp_ZUI_ACT_ExecuteIDLEAction(EN_EXE_IDLE_START_MAINMENU);
                return TRUE;
            }
            break;
        }
        case VK_POWER:
        {
            if(POWER_ON_FLAG)
            {//Power off
                PowerOffSystem();
            }
            else
            {//Power on
                PowerOnSystem();
            }
            break;
        }
        default:
            break;
    }
    return FALSE;
}

void MApp_ZUI_ACT_TerminateIDLE(void)
{
    ZUI_MSG(printf("[]term:IDLE\n");)
}


//////////////////////////////////////////
BOOLEAN MApp_ZUI_ACT_ExecuteIDLEAction(U16 act)
{
    bool result = FALSE;
    switch(act)
    {
        case EN_EXE_IDLE_START_MAINMENU:
            MApp_ZUI_ACT_StartupOSD(E_OSD_MAIN_MENU);
            result = TRUE;
            break;

        case EN_EXE_IDLE_BUTTON_CLICK:
            MApp_ZUI_API_SetTimer(HWND_BUTTON_CLICK, 0, 500);
            MApp_ZUI_API_InvalidateAllSuccessors(HWND_BUTTON_CLICK);
            break;

        case EN_EXE_IDLE_BALL_PROGRESS_INC:
            if(G_IDLE_SETTING.nBallProgressNum < MAX_NUM_OF_BALL_PROGRESS)
                G_IDLE_SETTING.nBallProgressNum++;
            MApp_ZUI_API_InvalidateAllSuccessors(HWND_BALLPROGRESSBAR_WNDPROC);
            result = TRUE;
            break;

        case EN_EXE_IDLE_BALL_PROGRESS_DEC:
            if(G_IDLE_SETTING.nBallProgressNum > 0)
                G_IDLE_SETTING.nBallProgressNum--;
            MApp_ZUI_API_InvalidateAllSuccessors(HWND_BALLPROGRESSBAR_WNDPROC);
            result = TRUE;
            break;

        case EN_EXE_IDLE_RECT_PROGRESS_INC:
            if(G_IDLE_SETTING.nRectProgressNum < MAX_NUM_OF_BALL_PROGRESS)
                G_IDLE_SETTING.nRectProgressNum++;
            MApp_ZUI_API_InvalidateAllSuccessors(HWND_RECTPROGRESSBAR_WNDPROC);
            result = TRUE;
            break;

        case EN_EXE_IDLE_RECT_PROGRESS_DEC:
            if(G_IDLE_SETTING.nRectProgressNum > 0)
                G_IDLE_SETTING.nRectProgressNum--;
            MApp_ZUI_API_InvalidateAllSuccessors(HWND_RECTPROGRESSBAR_WNDPROC);
            result = TRUE;
            break;

        case EN_EXE_IDLE_DYNAMIC_TEXT_CHANGE:
            if(G_IDLE_SETTING.bDynamicTextFlag)
                G_IDLE_SETTING.nDynamicTextID = en_str_On;
            else
                G_IDLE_SETTING.nDynamicTextID = en_str_Off;

            G_IDLE_SETTING.bDynamicTextFlag = !G_IDLE_SETTING.bDynamicTextFlag;

            MApp_ZUI_API_InvalidateAllSuccessors(HWND_DYNAMICTEXT_WNDPROC);
            result = TRUE;
            break;

        case EN_EXE_IDLE_DYNAMIC_COLOR_CHANGE:
            G_IDLE_SETTING.bDynamicColorFlag = !G_IDLE_SETTING.bDynamicColorFlag;
            MApp_ZUI_API_InvalidateAllSuccessors(HWND_DYNAMICCOLORTEXT_WNDPROC);
            result = TRUE;
            break;

        case EN_EXE_IDLE_DYNAMIC_BITMAP_CHANGE:
            G_IDLE_SETTING.bDynamicBitmapFlag = !G_IDLE_SETTING.bDynamicBitmapFlag;
            MApp_ZUI_API_InvalidateAllSuccessors(HWND_DYNAMICBITMAP_WNDPROC);
            result = TRUE;
            break;

        case EN_EXE_IDLE_DYNAMIC_LIST_CHANGE:
            printf("EN_EXE_IDLE_DYNAMIC_LIST_CHANGE\n");
            G_IDLE_SETTING.bDynamicListFlag = !G_IDLE_SETTING.bDynamicListFlag;
            MApp_ZUI_CTL_DynamicListRefreshContent(HWND_DYNAMICLIST_WNDPROC);
            MApp_ZUI_API_InvalidateAllSuccessors(HWND_DYNAMICLIST_WNDPROC);
            result = TRUE;
            break;

        case EN_EXE_CLOSE_CURRENT_OSD:
            //ShutdownOSD or MApp_ZUI_ACT_TransitionEffectBegin(EN_EFFMODE_CLOSE, E_ZUI_STATE_TERMINATE);
            MApp_ZUI_API_ShowWindow(HWND_AUTOCLOSE_WNDPROC, SW_HIDE);// for test
            return TRUE;

        //=================effect for effect proc====================
        case EN_EXE_EFFECT_ZOOM_IN:
            MApp_ZUI_API_PostMessage(MApp_ZUI_API_GetFocus(), MSG_EFFECT_ZOOMIN, (WPARAM)NULL);
            return TRUE;

        case EN_EXE_EFFECT_ZOOM_OUT:
            MApp_ZUI_API_PostMessage(MApp_ZUI_API_GetFocus(), MSG_EFFECT_ZOOMOUT, (WPARAM)NULL);
            return TRUE;

        case EN_EXE_EFFECT_SLIDEITEM:
            MApp_ZUI_API_PostMessage(MApp_ZUI_API_GetFocus(), MSG_EFFECT_SLIDEITEM, MApp_ZUI_API_GetFocus());
            return FALSE;

        case EN_EXE_EFFECT_POPUP:
            MApp_ZUI_API_PostMessage(MApp_ZUI_API_GetFocus(), MSG_EFFECT_POPUP, (WPARAM)NULL);
            return TRUE;

        case EN_EXE_EFFECT_SPREADOUT:
            MApp_ZUI_API_PostMessage(MApp_ZUI_API_GetFocus(), MSG_EFFECT_SPREADOUT, (WPARAM)NULL);
            return FALSE;

        case EN_EXE_EFFECT_ROLLUP:
            MApp_ZUI_API_PostMessage(MApp_ZUI_API_GetFocus(), MSG_EFFECT_ROLLUP, (WPARAM)NULL);
            return FALSE;

        case EN_EXE_IDLE_DYNAMIC_TEXT_EFFECT_CHANGE:
            G_IDLE_SETTING.bDynamicTextEffectFlag = !G_IDLE_SETTING.bDynamicTextEffectFlag;
            if(G_IDLE_SETTING.bDynamicTextEffectFlag)
                MApp_ZUI_API_PostMessage(MApp_ZUI_API_GetFocus(), MSG_EFFECT_DYNATEXT_LEFT, (WPARAM)NULL);
            else
                MApp_ZUI_API_PostMessage(MApp_ZUI_API_GetFocus(), MSG_EFFECT_DYNATEXT_RIGHT, (WPARAM)NULL);
            return FALSE;

        case EN_EXE_IDLE_MARQUEE_START:
            MApp_ZUI_CTL_MarqueeTextEnableAnimation(HWND_MARQUEE_WNDPROC,true);
            return TRUE;

        default:
            break;
    }
    return FALSE;
}

///////////////////////////////////////////
S16 MApp_ZUI_ACT_GetIDLEDynamicValue(HWND hwnd)
{
    switch(hwnd)
    {
        case HWND_BALLPROGRESSBAR_WNDPROC:
            ZUI_MSG(printf("G_IDLE_SETTING.nBallProgressNum = %d\n",G_IDLE_SETTING.nBallProgressNum));
            return G_IDLE_SETTING.nBallProgressNum;
        case HWND_RECTPROGRESSBAR_WNDPROC:
            ZUI_MSG(printf("G_IDLE_SETTING.nRectProgressNum = %d\n",G_IDLE_SETTING.nRectProgressNum));
            return G_IDLE_SETTING.nRectProgressNum;
        default:
            break;
    }
    return 0; //for empty  data
}

LPTSTR MApp_ZUI_ACT_GetIDLEDynamicText(HWND hwnd)
{
    switch(hwnd)
    {
        case HWND_DYNAMICTEXT_WNDPROC:
        {
            if (G_IDLE_SETTING.nDynamicTextID != Empty)
                return MApp_ZUI_API_GetString(G_IDLE_SETTING.nDynamicTextID);
            break;
        }
        case HWND_DYNAMICCOLORTEXT_WNDPROC:
            return MApp_ZUI_API_GetString(en_str_On);

        case HWND_MARQUEE_WNDPROC:
            return MApp_ZUI_API_GetString(en_str_VGA_Input_Text);

        case HWND_DYNAMICTEXT_EFFECT:
            if(G_IDLE_SETTING.bDynamicTextEffectFlag)
                return MApp_ZUI_API_GetString(en_str_VGA_Input_Text);
            else
                return MApp_ZUI_API_GetString(en_str_Bri_Con_Text);

        case HWND_DYNAMICPUNCTEXT_WNDPROC:
            return MApp_ZUI_API_GetString(en_str_Bri_Con_Text);

        default:
            break;
    }
    return 0;
}

OSD_COLOR MApp_ZUI_ACT_GetIDLEDynamicColor(HWND hwnd, DRAWSTYLE_TYPE type, OSD_COLOR colorOriginal)
{
    UNUSED(type);
    UNUSED(colorOriginal);
    switch(hwnd)
    {
        case HWND_DYNAMICCOLORTEXT_WNDPROC:
            if(G_IDLE_SETTING.bDynamicColorFlag)
            {
                return COLOR_CYAN;
            }
            else
                return colorOriginal;
        default:
            break;
    }
    return colorOriginal;
}

U16 MApp_ZUI_ACT_GetIDLEDynamicBitmap(HWND hwnd, DRAWSTYLE_TYPE ds_type)
{
    UNUSED(ds_type);

    switch(hwnd)
    {
        case HWND_DYNAMICBITMAP_WNDPROC:
        {
            if(G_IDLE_SETTING.bDynamicBitmapFlag)
                return E_BMP_MENU_LOGO_4C_20110524_;
            else
                return E_BMP_ITEM_FOCUS;
        }

        default:
            break;
    }
    return 0xFFFF; //for empty bitmap....
}

GUI_ENUM_DYNAMIC_LIST_STATE MApp_ZUI_ACT_QueryIDLEItemStatus(HWND hwnd)
{
    switch(hwnd)
    {
        case HWND_DYNAMICLIST_1:
        case HWND_DYNAMICLIST_3:
        {
            if(G_IDLE_SETTING.bDynamicListFlag)
                return EN_DL_STATE_NORMAL;
            else
                return EN_DL_STATE_DISABLED;
        }
        case HWND_DYNAMICLIST_2:
        {
            if(!G_IDLE_SETTING.bDynamicListFlag)
                return EN_DL_STATE_NORMAL;
            else
                return EN_DL_STATE_DISABLED;
        }
        default:
            break;
    }

    return EN_DL_STATE_NORMAL;
}

U8 MApp_ZUI_ACT_GetIDLEGridIndex(void)
{
    return G_IDLE_SETTING.nGridIndex;
}

void MApp_ZUI_ACT_SetIDLEGridIndex(U8 u8Index)
{
    G_IDLE_SETTING.nGridIndex = u8Index;
    //user can use this nGridIndex to do sth.

    MApp_ZUI_API_InvalidateAllSuccessors(HWND_GRID_WNDPROC);
}

LPTSTR MApp_ZUI_ACT_GetIDLEStringIDByIndex(U8 u8Index)
{
    if(u8Index < 2)
        return MApp_ZUI_API_GetString(en_str_CustomeB);
    else
        return MApp_ZUI_API_GetString(en_str_CustomeG);

    return 0; //for empty string....
}


#undef MAPP_ZUI_ACTIDLE_C

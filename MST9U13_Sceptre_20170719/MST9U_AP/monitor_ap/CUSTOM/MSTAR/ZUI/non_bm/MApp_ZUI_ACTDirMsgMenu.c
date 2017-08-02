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

#define MAPP_ZUI_ACTDIRMSGMENU_C
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

#include "MApp_ZUI_ACTDirMsgMenu.h"
#include "MApp_ZUI_ACTglobal.h"
#include "MApp_ZUI_ACTMainMenu.h"
#include "Global.h"
//#include "MApp_GlobalFunction.h"
#include "UserPrefDef.h"
#include "menudef.h"
//#include "MApp_ZUI_menufunc.h"
#include "OsdDefault.h"

extern ZUI_STATE _eZUIState;
extern BOOLEAN _MApp_ZUI_API_AllocateVarData(void);
extern BYTE xdata   g_u8MenuTempValue;
/////////////////////////////////////////////////////////////////////

void MApp_ZUI_ACT_AppShowDirMsgMenu(void)
{
    HWND wnd;
    RECT rect;
    E_OSD_ID osd_id = E_OSD_DIRMSG_MENU;

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
        (gScreenWidth-ZUI_DIRMSG_MENU_WIDTH)/2, (gScreenHeight-ZUI_DIRMSG_MENU_HEIGHT)/2,
        ZUI_DIRMSG_MENU_WIDTH, ZUI_DIRMSG_MENU_HEIGHT);

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
    MApp_ZUI_API_ShowWindow(HWND_DIRMSG_MENU_TRANSPARENT_BG, SW_SHOW);
    MApp_ZUI_API_ShowWindow(HWND_DIRECT_MSG_MENU, SW_SHOW);
    MApp_ZUI_API_ResetTimer(HWND_DIRECT_MSG_MENU, 0);
    MApp_ZUI_API_ShowWindow(HWND_DIRECT_MSG_MENU, SW_SHOW);
    MApp_ZUI_API_SetTimer(HWND_DIRECT_MSG_MENU,0, DIRMSG_MENU_TIMEOUT);

    _eZUIState = E_ZUI_STATE_RUNNING;
    //MApp_ZUI_ACT_TransitionEffectBegin(EN_EFFMODE_OPEN, E_ZUI_STATE_RUNNING);
}

//////////////////////////////////////////////////////////
// Key Handler

BOOLEAN MApp_ZUI_ACT_HandleDirMsgMenuKey(VIRTUAL_KEY_CODE key)
{
    switch(key)
    {

        default:
            break;
    }
    return FALSE;
}
void MApp_ZUI_ACT_TerminateDirMsgMenu(void)
{
    ZUI_MSG(printf("[]term:Auto Adjust\n");)
}


//////////////////////////////////////////
BOOLEAN MApp_ZUI_ACT_ExecuteDirMsgMenuAction(U16 act)
{
    switch(act)
    {
        case EN_EXE_CLOSE_CURRENT_OSD:
        {
            MApp_ZUI_ACT_ShutdownOSD();
            return TRUE;
        }
        case EN_EXE_IDLE_START_MAINMENU:
        {
            MApp_ZUI_ACT_StartupOSD(E_OSD_MAIN_MENU);
            return TRUE;
        }
        default:
            break;
    }
    return FALSE;
}

LPTSTR MApp_ZUI_ACT_GetDirMsgMenuDynamicText(HWND hwnd)
{
    U16 u16TempID = Empty;

    switch(hwnd)
    {
        case HWND_DIRECT_MSG_MENU_TEXT:
        {
            if(!SyncLossState(SrcInputPortM))
            {
                if(USER_PREF_INPUT_PRIORITY == INPUT_PRIORITY_AUTO)
                    u16TempID = en_str_Auto_Select;
            #if ENABLE_VGA_INPUT
                else if(USER_PREF_INPUT_PRIORITY == INPUT_PRIORITY_VGA)
                    u16TempID = en_str_Source_VGA;
            #endif
            #if ENABLE_DVI_INPUT
                else if(USER_PREF_INPUT_PRIORITY == INPUT_PRIORITY_DVI)
                    u16TempID = en_str_Source_DVI;
            #endif
            #if ENABLE_HDMI_INPUT
                else if(USER_PREF_INPUT_PRIORITY == INPUT_PRIORITY_HDMI)
                    u16TempID = en_str_HDMI_1;
            #endif
            #if ENABLE_HDMI2ND_INPUT
                else if(USER_PREF_INPUT_PRIORITY == INPUT_PRIORITY_HDMI2ND)
                    u16TempID = en_str_HDMI_2;
            #endif
            #if ENABLE_DP_INPUT
                else if(USER_PREF_INPUT_PRIORITY == INPUT_PRIORITY_DP)
                    u16TempID = en_str_Display_Port_1;
            #endif
            #if ENABLE_DP2ND_INPUT
                else if(USER_PREF_INPUT_PRIORITY == INPUT_PRIORITY_DP2ND)
                   u16TempID = en_str_Display_Port_2;
            #endif
                }
            else // no sync
            {
                if(USER_PREF_INPUT_PRIORITY != INPUT_PRIORITY_AUTO)
                {
                #if ENABLE_VGA_INPUT
                    if(CURRENT_INPUT_IS_ANALOG1())
                        u16TempID = en_str_No_VGA_Cable;
					else
                #endif
                #if ENABLE_DVI_INPUT
                    if(CURRENT_INPUT_IS_DIGITAL_DVI())
                        u16TempID = en_str_No_DVI_1_Cable;
					else
                #endif
                #if ENABLE_DVI2ND_INPUT
                    if(CURRENT_INPUT_IS_DIGITAL_DVI2ND())
                        u16TempID = en_str_No_DVI_2_Cable;
					else
                #endif
                #if ENABLE_HDMI_INPUT
                    if(CURRENT_INPUT_IS_DIGITAL_HDMI())
                        u16TempID = en_str_No_HDMI_1_Cable;
					else
                #endif
                #if ENABLE_HDMI2ND_INPUT
                    if(CURRENT_INPUT_IS_DIGITAL_HDMI2ND())
                        u16TempID = en_str_No_HDMI_2_Cable;
					else
                #endif
                #if ENABLE_DP_INPUT
                    if(CURRENT_INPUT_IS_DIGITAL_DP())
                        u16TempID = en_str_No_Display_1_Port_Cable;
					else
                #endif
                #if ENABLE_DP2ND_INPUT
                    if(CURRENT_INPUT_IS_DIGITAL_DP2ND())
                        u16TempID = en_str_No_Display_2_Port_Cable;
					else
                #endif
						u16TempID = en_str_No_Signal_Input;
                }
                else
                  u16TempID = en_str_No_Signal_Input;
            }
                break;
        }

        default:
            break;
    }


    if (u16TempID != Empty)
        return MApp_ZUI_API_GetString(u16TempID);

    return 0; //for empty string....
}

#undef MAPP_ZUI_ACTDIRMSGMENU_C

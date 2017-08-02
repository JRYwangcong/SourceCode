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

#define MAPP_ZUI_ACTMAINMENU_C
#define _ZUI_INTERNAL_INSIDE_ //NOTE: for ZUI internal
#define MAIN_MENU_COLOR_RED 0xFFFF0000

//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include <string.h>
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

#include "MApp_ZUI_ACTMainMenu.h"
#include "MApp_ZUI_ACTglobal.h"
#include "MApp_GlobalFunction.h"
#include "MApp_LoadFontInit.h"
#include "UserPrefDef.h"
#include "menudef.h"
#include "menufunc.h"
//#include "MApp_ZUI_menufunc.h"
#include "OsdDefault.h"
#include "Global.h"
#include "Mode.h"
#include "OsdDefault.h"
#include "msScaler.h"
#include "appWindow.h"
#include "UserPref.h"

extern BOOLEAN _MApp_ZUI_API_AllocateVarData(void);
void MApp_ZUI_ReturnResolution(U8 *strBuffer,U8 WIN_Sel,HWND hwnd);
extern BYTE xdata   g_u8MenuTempValue;
extern BYTE xdata  g_u8OriginalSetting;
extern U32 u32TimerDelay;
extern BOOLEAN g_bSourceChangeFlag;
BOOLEAN g_bAutoAdjustFlag = 0;
BOOLEAN b_ReturnFocusFlag = 0;
EN_LANGUAGE u8Language;
BOOLEAN bPreFocusFlag;

/////////////////////////////////////////////////////////////////////

void MApp_ZUI_ACT_AppShowMainMenu(void)
{
    HWND wnd;
    RECT rect;
    E_OSD_ID osd_id = E_OSD_MAIN_MENU;

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
        (gScreenWidth-ZUI_MAIN_MENU_WIDTH)/2, (gScreenHeight-ZUI_MAIN_MENU_HEIGHT)/2,
        ZUI_MAIN_MENU_WIDTH, ZUI_MAIN_MENU_HEIGHT);

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
    MApp_ZUI_API_ShowWindow(HWND_MENU_TRANSPARENT_BG, SW_SHOW);

    MApp_ZUI_API_ResetTimer(HWND_MAIN_MENU_PAGE_BG, 0);
    MApp_ZUI_API_ShowWindow(HWND_MAIN_MENU_PAGE_BG, SW_SHOW);
    MApp_ZUI_API_SetTimer(HWND_MAIN_MENU_PAGE_BG,0, DEF_OSD_TIMEOUT*1000);

    MApp_ZUI_API_ShowWindow(HWND_MULTI_WIN_INFO, SW_SHOW);
    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_PAGE_INP_SRC_ITEM_LIST, SW_HIDE);
    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_COL_SET_PAGE, SW_HIDE);
    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_DIS_SET_PAGE, SW_HIDE);
    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_OTHER_SET_PAGE, SW_HIDE);
    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_MULTI_WIN_SET_PAGE, SW_HIDE);

    if(b_ReturnFocusFlag)
    {
        b_ReturnFocusFlag = FALSE;
        MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_BRI_CON_PAGE,SW_HIDE);
        MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_AUTO_ADJUST_PAGE,SW_SHOW);
        MApp_ZUI_API_SetFocus(HWND_MAIN_MENU_AUTO_ADJUST_ITEM);
    }
    else
        MApp_ZUI_API_SetFocus(HWND_MAIN_MENU_BRI_ITEM);
    MApp_ZUI_API_StoreFocusCheckpoint();
    MApp_ZUI_ACT_TransitionEffectBegin(EN_EFFMODE_OPEN, E_ZUI_STATE_RUNNING);
}


//////////////////////////////////////////////////////////
// Key Handler

BOOLEAN MApp_ZUI_ACT_HandleMainMenuKey(VIRTUAL_KEY_CODE key)
{
    MApp_ZUI_API_ResetTimer(HWND_MAIN_MENU_PAGE_BG, 0);
    switch(key)
    {
        //case VK_MENU:
            //MApp_ZUI_ACT_ExecuteMainMenuAction(EN_EXE_IDLE_START_MAINMENU);
            //return TRUE;
        case VK_POWER:
            if(POWER_ON_FLAG)
            {//Power off
                PowerOffSystem();
            }
            else
            {//Power on
                PowerOnSystem();
            }
            break;
        default:
            break;

    }
    return FALSE;
}
U16 MApp_ZUI_ACT_GetMainMenuDynamicBitmap(HWND hwnd, DRAWSTYLE_TYPE ds_type)
{
    UNUSED(ds_type);

    switch(hwnd)
    {
        case HWND_SUB_MENU_AUTO_SRC_ITEM:
        {
            if(HWND_SUB_MENU_AUTO_SRC_ITEM == MApp_ZUI_API_GetFocus() && USER_PREF_INPUT_PRIORITY == INPUT_PRIORITY_AUTO)
                return E_BMP_ITEM_FOCUS_DOT;
            else
                return E_BMP_ITEM_FOCUS;
        }
	#if ENABLE_VGA_INPUT
        case HWND_SUB_MENU_VGA_ITEM:
        {
             if(HWND_SUB_MENU_VGA_ITEM == MApp_ZUI_API_GetFocus() && USER_PREF_INPUT_PRIORITY == INPUT_PRIORITY_VGA)
                return E_BMP_ITEM_FOCUS_DOT;
            else
                return E_BMP_ITEM_FOCUS;
        }
	#endif	
    #if ENABLE_DVI_INPUT
        case HWND_SUB_MENU_DVI_ITEM:
        {
            if(HWND_SUB_MENU_DVI_ITEM ==MApp_ZUI_API_GetFocus() && USER_PREF_INPUT_PRIORITY == INPUT_PRIORITY_DVI)
                return E_BMP_ITEM_FOCUS_DOT;
            else
                return E_BMP_ITEM_FOCUS;
        }
    #endif
    #if ENABLE_HDMI_INPUT
        case HWND_SUB_MENU_HDMI1_ITEM:
        {
            if(HWND_SUB_MENU_HDMI1_ITEM ==MApp_ZUI_API_GetFocus() && USER_PREF_INPUT_PRIORITY == INPUT_PRIORITY_HDMI)
                return E_BMP_ITEM_FOCUS_DOT;
            else
                return E_BMP_ITEM_FOCUS;
        }
    #endif
    #if ENABLE_HDMI2ND_INPUT
        case HWND_SUB_MENU_HDMI2_ITEM:
        {
            if(HWND_SUB_MENU_HDMI2_ITEM ==MApp_ZUI_API_GetFocus() && USER_PREF_INPUT_PRIORITY == INPUT_PRIORITY_HDMI2ND)
                return E_BMP_ITEM_FOCUS_DOT;
            else
                return E_BMP_ITEM_FOCUS;
        }
    #endif
    #if ENABLE_DP_INPUT
        case HWND_SUB_MENU_DP1_ITEM:
        {
            if(HWND_SUB_MENU_DP1_ITEM ==MApp_ZUI_API_GetFocus() && USER_PREF_INPUT_PRIORITY == INPUT_PRIORITY_DP)
                return E_BMP_ITEM_FOCUS_DOT;
            else
                return E_BMP_ITEM_FOCUS;
        }
    #endif
    #if ENABLE_DP2ND_INPUT
        case HWND_SUB_MENU_DP2_ITEM:
        {
            if(HWND_SUB_MENU_DP2_ITEM ==MApp_ZUI_API_GetFocus() && USER_PREF_INPUT_PRIORITY == INPUT_PRIORITY_DP2ND)
                return E_BMP_ITEM_FOCUS_DOT;
            else
                return E_BMP_ITEM_FOCUS;
        }
 #endif
        case HWND_SUB_MENU_AUTO_SRC_ICON:
        {
            if(USER_PREF_INPUT_PRIORITY == INPUT_PRIORITY_AUTO)
            {

                if(MApp_ZUI_API_GetFocus() == HWND_SUB_MENU_AUTO_SRC_ITEM)
                {
                    return E_BMP_CHECK_MARK_TICK_FOCUS;
                }
                else
                    return E_BMP_CHECK_MARK_TICK_2C_20130423;
            }
            else
                break;
        }
	#if ENABLE_VGA_INPUT
        case HWND_SUB_MENU_VGA_SRC_ICON:
        {
            if(USER_PREF_INPUT_PRIORITY == INPUT_PRIORITY_VGA)
            {

                 if(MApp_ZUI_API_GetFocus() == HWND_SUB_MENU_VGA_ITEM)
                {
                    return E_BMP_CHECK_MARK_TICK_FOCUS;
                }
                else
                    return E_BMP_CHECK_MARK_TICK_2C_20130423;

            }
            else
                break;
        }
	#endif	
    #if ENABLE_DVI_INPUT
        case HWND_SUB_MENU_DVI_SRC_ICON:
        {
            if(USER_PREF_INPUT_PRIORITY == INPUT_PRIORITY_DVI)
            {

                 if(MApp_ZUI_API_GetFocus() == HWND_SUB_MENU_DVI_ITEM)
                {
                    return E_BMP_CHECK_MARK_TICK_FOCUS;
                }
                else
                    return E_BMP_CHECK_MARK_TICK_2C_20130423;

            }
            else
                break;
        }
    #endif
    #if ENABLE_HDMI_INPUT
        case HWND_SUB_MENU_HDMI1_SRC_ICON:
        {
            if(USER_PREF_INPUT_PRIORITY == INPUT_PRIORITY_HDMI)
            {

                 if(MApp_ZUI_API_GetFocus() == HWND_SUB_MENU_HDMI1_ITEM)
                {
                    return E_BMP_CHECK_MARK_TICK_FOCUS;
                }
                else
                    return E_BMP_CHECK_MARK_TICK_2C_20130423;

            }
            else
                break;
        }
    #endif
    #if ENABLE_HDMI2ND_INPUT
        case HWND_SUB_MENU_HDMI2_SRC_ICON:
        {
            if(USER_PREF_INPUT_PRIORITY == INPUT_PRIORITY_HDMI2ND)
            {

                 if(MApp_ZUI_API_GetFocus() == HWND_SUB_MENU_HDMI2_ITEM)
                {
                    return E_BMP_CHECK_MARK_TICK_FOCUS;
                }
                else
                    return E_BMP_CHECK_MARK_TICK_2C_20130423;

            }
            else
                break;
        }
    #endif
    #if ENABLE_DP_INPUT
        case HWND_SUB_MENU_DP1_SRC_ICON:
        {
            if(USER_PREF_INPUT_PRIORITY == INPUT_PRIORITY_DP)
            {

                 if(MApp_ZUI_API_GetFocus() == HWND_SUB_MENU_DP1_ITEM)
                {
                    return E_BMP_CHECK_MARK_TICK_FOCUS;
                }
                else
                    return E_BMP_CHECK_MARK_TICK_2C_20130423;

            }
            else
                break;
        }
    #endif
    #if ENABLE_DP2ND_INPUT
        case HWND_SUB_MENU_DP2_SRC_ICON:
        {
            if(USER_PREF_INPUT_PRIORITY == INPUT_PRIORITY_DP2ND)
            {

                 if(MApp_ZUI_API_GetFocus() == HWND_SUB_MENU_DP2_ITEM)
                {
                    return E_BMP_CHECK_MARK_TICK_FOCUS;
                }
                else
                    return E_BMP_CHECK_MARK_TICK_2C_20130423;

            }
            else
                break;
        }
    #endif
        default:
            break;
    }

    return 0xFFFF;
}
LPTSTR MApp_ZUI_ACT_GetMainMenuDynamicText(HWND hwnd)
{
    U16 u16TempID = Empty;
    U8 u8TempString[STRING_BUFFER_LENGTH];

    switch(hwnd)
    {
        case HWND_SUB_MENU_DISSET_RESPONSETIME_ITEM_TEXT:
        {
            u16TempID = en_str_Response_Time;
            break;
        }
        case HWND_SUB_MENU_DISSET_RESPONSETIME_ITEM_VALUE_TEXT:
        {
            if(!GetResponseTimeValue())
               u16TempID = en_str_Normal;
            else
                u16TempID = en_str_Overdrive;
            break;
        }
        case HWND_MAIN_MENU_TITLE_TEXT:
        {
            if(CURRENT_INPUT_IS_VGA())
                u16TempID = en_str_Source_VGA;
    #if  ENABLE_DVI_INPUT
            else if(CURRENT_INPUT_IS_DVI())
                u16TempID = en_str_Source_DVI;
    #endif
    #if ENABLE_HDMI_INPUT
            else if(CURRENT_INPUT_IS_DIGITAL_HDMI())
                u16TempID = en_str_HDMI_1;
    #endif
    #if ENABLE_HDMI2ND_INPUT
            else if(CURRENT_INPUT_IS_DIGITAL_HDMI2ND())
                u16TempID = en_str_HDMI_2;
    #endif
    #if ENABLE_DP_INPUT
            else if(CURRENT_INPUT_IS_DIGITAL_DP())
                u16TempID = en_str_Display_Port_1;
    #endif
    #if ENABLE_DP2ND_INPUT
            else if(CURRENT_INPUT_IS_DIGITAL_DP2ND())
                u16TempID = en_str_Display_Port_2;
    #endif
            else
                u16TempID = 0;
                break;
        }
        case HWND_MAIN_MENU_TITLE_RES_NUM:
        {
            MApp_ZUI_ReturnResolution(u8TempString,USER_PREF_WIN_SEL,hwnd);
            MApp_U8StringToU16String(u8TempString,CHAR_BUFFER,strlen((char *)u8TempString));
            return CHAR_BUFFER;
        }
        case HWND_MULTI_WIN_INFO_W1_SRC_NUM:
        case HWND_MULTI_WIN_INFO_W2_SRC_NUM:
        case HWND_MULTI_WIN_INFO_W3_SRC_NUM:
        case HWND_MULTI_WIN_INFO_W4_SRC_NUM:
        {
            if(USER_PREF_MULTI_WIN_MODE == eMULTIWIN_OFF)
            {
                if((hwnd == HWND_MULTI_WIN_INFO_W1_SRC_NUM) && (!CableNotConnectedFlag(USER_PREF_WIN_SRC(DISPLAY_WIN1))))
                    u16TempID = MApp_ZUI_ACT_GetSourceStringID(DISPLAY_WIN1);
                else
                    return 0;
            }
            else if(USER_PREF_MULTI_WIN_MODE == eMULTIWIN_2_PIP || USER_PREF_MULTI_WIN_MODE== eMULTIWIN_2_POP1 \
                        || USER_PREF_MULTI_WIN_MODE == eMULTIWIN_2_POP2)
            {
                if((hwnd == HWND_MULTI_WIN_INFO_W1_SRC_NUM) && (!CableNotConnectedFlag(USER_PREF_WIN_SRC(DISPLAY_WIN1))))
                    u16TempID = MApp_ZUI_ACT_GetSourceStringID(DISPLAY_WIN1);
                else if((hwnd == HWND_MULTI_WIN_INFO_W2_SRC_NUM) && (!CableNotConnectedFlag(USER_PREF_WIN_SRC(DISPLAY_WIN2))))
                    u16TempID = MApp_ZUI_ACT_GetSourceStringID(DISPLAY_WIN2);
                else
                    return 0;
            }
            else if(USER_PREF_MULTI_WIN_MODE == eMULTIWIN_3)
            {
                if((hwnd == HWND_MULTI_WIN_INFO_W1_SRC_NUM) && (!CableNotConnectedFlag(USER_PREF_WIN_SRC(DISPLAY_WIN1))))
                    u16TempID = MApp_ZUI_ACT_GetSourceStringID(DISPLAY_WIN1);
                else if((hwnd == HWND_MULTI_WIN_INFO_W2_SRC_NUM) && (!CableNotConnectedFlag(USER_PREF_WIN_SRC(DISPLAY_WIN2))))
                    u16TempID = MApp_ZUI_ACT_GetSourceStringID(DISPLAY_WIN2);
                else if((hwnd == HWND_MULTI_WIN_INFO_W3_SRC_NUM) && (!CableNotConnectedFlag(USER_PREF_WIN_SRC(DISPLAY_WIN3))))
                    u16TempID = MApp_ZUI_ACT_GetSourceStringID(DISPLAY_WIN3);
                else
                    return 0;
            }
            else //eMULTIWIN_4
            {
                if((hwnd == HWND_MULTI_WIN_INFO_W1_SRC_NUM) && (!CableNotConnectedFlag(USER_PREF_WIN_SRC(DISPLAY_WIN1))))
                    u16TempID = MApp_ZUI_ACT_GetSourceStringID(DISPLAY_WIN1);
                else if((hwnd == HWND_MULTI_WIN_INFO_W2_SRC_NUM) && (!CableNotConnectedFlag(USER_PREF_WIN_SRC(DISPLAY_WIN2))))
                    u16TempID = MApp_ZUI_ACT_GetSourceStringID(DISPLAY_WIN2);
                else if((hwnd == HWND_MULTI_WIN_INFO_W3_SRC_NUM) && (!CableNotConnectedFlag(USER_PREF_WIN_SRC(DISPLAY_WIN3))))
                    u16TempID = MApp_ZUI_ACT_GetSourceStringID(DISPLAY_WIN3);
                else if((hwnd == HWND_MULTI_WIN_INFO_W4_SRC_NUM) && (!CableNotConnectedFlag(USER_PREF_WIN_SRC(DISPLAY_WIN4))))
                    u16TempID = MApp_ZUI_ACT_GetSourceStringID(DISPLAY_WIN4);
                else
                    return 0;
            }
            break;
        }
        case HWND_MULTI_WIN_INFO_W1_RESOLUTION_NUM:
        case HWND_MULTI_WIN_INFO_W2_RESOLUTION_NUM:
        case HWND_MULTI_WIN_INFO_W3_RESOLUTION_NUM:
        case HWND_MULTI_WIN_INFO_W4_RESOLUTION_NUM:
        {
            if(USER_PREF_MULTI_WIN_MODE == eMULTIWIN_OFF)
            {
                if(hwnd == HWND_MULTI_WIN_INFO_W1_RESOLUTION_NUM)
                {
                    MApp_ZUI_ReturnResolution(u8TempString,DISPLAY_WIN1,hwnd);
                    MApp_U8StringToU16String(u8TempString,CHAR_BUFFER,strlen((char *)u8TempString));
                    if(!SyncLossState(USER_PREF_WIN_SRC(DISPLAY_WIN1)))
                        return CHAR_BUFFER;
                    else
                    return 0;
                }
                else
                    return 0;
            }
            else if(USER_PREF_MULTI_WIN_MODE == eMULTIWIN_2_PIP || USER_PREF_MULTI_WIN_MODE== eMULTIWIN_2_POP1 \
                        || USER_PREF_MULTI_WIN_MODE == eMULTIWIN_2_POP2)
            {
                if(hwnd == HWND_MULTI_WIN_INFO_W1_RESOLUTION_NUM)
                {
                    MApp_ZUI_ReturnResolution(u8TempString,DISPLAY_WIN1,hwnd);
                    MApp_U8StringToU16String(u8TempString,CHAR_BUFFER,strlen((char *)u8TempString));
                    if(!SyncLossState(USER_PREF_WIN_SRC(DISPLAY_WIN1)))
                        return CHAR_BUFFER;
                    else
                        return 0;
                }
                else if(hwnd == HWND_MULTI_WIN_INFO_W2_RESOLUTION_NUM)
                {
                    MApp_ZUI_ReturnResolution(u8TempString,DISPLAY_WIN2,hwnd);
                    MApp_U8StringToU16String(u8TempString,CHAR_BUFFER,strlen((char *)u8TempString));
                    if(!SyncLossState(USER_PREF_WIN_SRC(DISPLAY_WIN2)))
                        return CHAR_BUFFER;
                    else
                        return 0;
                }
                else
                    return 0;
            }
            else if(USER_PREF_MULTI_WIN_MODE == eMULTIWIN_3)
            {
                if(hwnd == HWND_MULTI_WIN_INFO_W1_RESOLUTION_NUM)
                {
                    MApp_ZUI_ReturnResolution(u8TempString,DISPLAY_WIN1,hwnd);
                    MApp_U8StringToU16String(u8TempString,CHAR_BUFFER,strlen((char *)u8TempString));
                    if(!SyncLossState(USER_PREF_WIN_SRC(DISPLAY_WIN1)))
                        return CHAR_BUFFER;
                    else
                        return 0;
                }
                else if(hwnd == HWND_MULTI_WIN_INFO_W2_RESOLUTION_NUM)
                {
                    MApp_ZUI_ReturnResolution(u8TempString,DISPLAY_WIN2,hwnd);
                    MApp_U8StringToU16String(u8TempString,CHAR_BUFFER,strlen((char *)u8TempString));
                    if(!SyncLossState(USER_PREF_WIN_SRC(DISPLAY_WIN2)))
                        return CHAR_BUFFER;
                    else
                        return 0;
                }
                else if(hwnd == HWND_MULTI_WIN_INFO_W3_RESOLUTION_NUM)
                {
                    MApp_ZUI_ReturnResolution(u8TempString,DISPLAY_WIN3,hwnd);
                    MApp_U8StringToU16String(u8TempString,CHAR_BUFFER,strlen((char *)u8TempString));
                    if(!SyncLossState(USER_PREF_WIN_SRC(DISPLAY_WIN3)))
                        return CHAR_BUFFER;
                    else
                        return 0;
                }
                else
                    return 0;
            }
            else //eMULTIWIN_4
            {
                if(hwnd == HWND_MULTI_WIN_INFO_W1_RESOLUTION_NUM)
                {
                    MApp_ZUI_ReturnResolution(u8TempString,DISPLAY_WIN1,hwnd);
                    MApp_U8StringToU16String(u8TempString,CHAR_BUFFER,strlen((char *)u8TempString));
                    if(!SyncLossState(USER_PREF_WIN_SRC(DISPLAY_WIN1)))
                        return CHAR_BUFFER;
                    else
                        return 0;
                }
                else if(hwnd == HWND_MULTI_WIN_INFO_W2_RESOLUTION_NUM)
                {
                    MApp_ZUI_ReturnResolution(u8TempString,DISPLAY_WIN2,hwnd);
                    MApp_U8StringToU16String(u8TempString,CHAR_BUFFER,strlen((char *)u8TempString));
                    if(!SyncLossState(USER_PREF_WIN_SRC(DISPLAY_WIN2)))
                        return CHAR_BUFFER;
                    else
                        return 0;
                }
                else if(hwnd == HWND_MULTI_WIN_INFO_W3_RESOLUTION_NUM)
                {
                    MApp_ZUI_ReturnResolution(u8TempString,DISPLAY_WIN3,hwnd);
                    MApp_U8StringToU16String(u8TempString,CHAR_BUFFER,strlen((char *)u8TempString));
                    if(!SyncLossState(USER_PREF_WIN_SRC(DISPLAY_WIN3)))
                        return CHAR_BUFFER;
                    else
                        return 0;
                }
                else if(hwnd == HWND_MULTI_WIN_INFO_W4_RESOLUTION_NUM)
                {
                    MApp_ZUI_ReturnResolution(u8TempString,DISPLAY_WIN4,hwnd);
                    MApp_U8StringToU16String(u8TempString,CHAR_BUFFER,strlen((char *)u8TempString));
                    if(!SyncLossState(USER_PREF_WIN_SRC(DISPLAY_WIN4)))
                        return CHAR_BUFFER;
                    else
                        return 0;
                }
                else
                    return 0;
            }

            break;
        }
        case HWND_SUB_MENU_MULTIWIN_PIPHP_VALUE_TEXT:
        case HWND_SUB_MENU_MULTIWIN_PIPHP_ITEM_VALUE_TEXT:
        #if ENABLE_MULTI_WINDOW_SETTINGS_MENU
            return MApp_ZUI_API_GetU16String(GetPIPHPositionValue());
        #else
            return 0;
        #endif
        case HWND_SUB_MENU_MULTIWIN_PIPVP_VALUE_ITEM_TEXT:
        case HWND_SUB_MENU_MULTIWIN_PIPVP_ITEM_VALUE_TEXT:
        #if ENABLE_MULTI_WINDOW_SETTINGS_MENU
            return MApp_ZUI_API_GetU16String(GetPIPVPositionValue());
        #else
            return 0;
        #endif
        case HWND_SUB_MENU_MULTIWIN_WINMODES_ITEM_VALUE_TEXT:
        {
            if(USER_PREF_MULTI_WIN_MODE == eMULTIWIN_OFF)
                u16TempID = en_str_Off;
            else if(USER_PREF_MULTI_WIN_MODE == eMULTIWIN_2_PIP)
                u16TempID = en_str_PIP_Mode;
            else if(USER_PREF_MULTI_WIN_MODE == eMULTIWIN_2_POP1)
                u16TempID = en_str_PBP_Mode_1;
            else if(USER_PREF_MULTI_WIN_MODE == eMULTIWIN_2_POP2)
                u16TempID = en_str_PBP_Mode_2;
            else if(USER_PREF_MULTI_WIN_MODE == eMULTIWIN_3)
                u16TempID = en_str_Triple_Mode;
            else
                u16TempID = en_str_Quad_Mode;
                break;
        }
        case HWND_SUB_MENU_MULTIWIN_WINSEL_ITEM_VALUE_TEXT:
        {
            if(USER_PREF_WIN_SEL == DISPLAY_WIN1)
                u16TempID = en_str_Window1;
            else if(USER_PREF_WIN_SEL == DISPLAY_WIN2)
                u16TempID = en_str_Window2;
            else if(USER_PREF_WIN_SEL == DISPLAY_WIN3)
                u16TempID = en_str_Window3;
            else
                u16TempID = en_str_Window4;
                break;
        }
        case HWND_SUB_MENU_MULTIWIN_PIPSIZE_ITEM_VALUE_TEXT:
        {
            if(USER_PREF_MULTI_WIN_SIZE(DISPLAY_WIN2) == ePIPSIZE_SMALL)
                u16TempID = en_str_Small;
            else if(USER_PREF_MULTI_WIN_SIZE(DISPLAY_WIN2) == ePIPSIZE_MIDDLE)
               u16TempID = en_str_Medium;
            else if(USER_PREF_MULTI_WIN_SIZE(DISPLAY_WIN2) == ePIPSIZE_LARGE)
               u16TempID = en_str_Large;
            else if(USER_PREF_MULTI_WIN_SIZE(DISPLAY_WIN2) == ePIPSIZE_HUGE)
                u16TempID = en_str_Huge;
                break;
        }

        case HWND_MAIN_MENU_BRI_TEXT:
            u16TempID = en_str_Bri_Con_Text;
            break;
        case HWND_SUB_MENU_BRI_TEXT:
            u16TempID = en_str_Brightness;
            break;
        case HWND_MAIN_MENU_INPUT_SOURCE_TEXT:
            u16TempID = en_str_Input_Source;
            break;
        case HWND_MAIN_MENU_COLOR_SETTINGS_TEXT:
            u16TempID = en_str_Color_Settings;
            break;
        case HWND_MAIN_MENU_DISPLAY_SETTINGS_TEXT:
            u16TempID = en_str_Display_Settings;
            break;
        case HWND_MAIN_MENU_MULTI_WIN_SETTINGS_TEXT:
            u16TempID = en_str_Multi_Window_Settings;
            break;
        case HWND_SUB_MENU_MULTIWIN_WINMODES_ITEM_TEXT:
            u16TempID = en_str_Multi_Window_Modes;
            break;
        case HWND_SUB_MENU_MULTIWIN_WINSEL_ITEM_TEXT:
            u16TempID = en_str_Window_Select;
            break;
        case HWND_SUB_MENU_MULTIWIN_PIPSIZE_ITEM_TEXT:
            u16TempID = en_str_PIP_Size;
            break;
        case HWND_SUB_MENU_MULTIWIN_PIPHP_ITEM_TEXT:
            u16TempID = en_str_PIP_Horizontal_Position;
            break;
        case HWND_SUB_MENU_MULTIWIN_PIPVP_ITEM_TEXT:
            u16TempID = en_str_PIP_Vertical_Position;
            break;


        case HWND_MAIN_MENU_OTHER_SETTINGS_TEXT:
            u16TempID = en_str_Other_Settings;
            break;
        case HWND_SUB_MENU_CON_TEXT:
            u16TempID = en_str_Contrast;
            break;
        case HWND_SUB_MENU_DYNAMIC_CON_TEXT:
            u16TempID = en_str_Dynamic_Contrast;
            break;
        case HWND_SUB_MENU_COLSET_INPCOLFMT_ITEM_TEXT:
            u16TempID = en_str_Input_Color_Format;
            break;
        case HWND_SUB_MENU_COLSET_PREMOD_ITEM_TEXT:
            u16TempID = en_str_Preset_Modes;
            break;
        case HWND_SUB_MENU_COLSET_SUPERRES_ITEM_TEXT:
            u16TempID = en_str_Super_Resolution;
            break;
        case HWND_SUB_MENU_OTHERSET_TPC_ITEM_TEXT:
            u16TempID = en_str_Menu_Transparency;
            break;
        case HWND_SUB_MENU_DISSET_ASPRAT_ITEM_TEXT:
            u16TempID = en_str_Aspect_Ratio;
            break;
        case HWND_SUB_MENU_DISSET_HP_ITEM_TEXT:
            u16TempID = en_str_Horizontal_Position;
            break;
        case HWND_SUB_MENU_DISSET_VP_ITEM_TEXT:
            u16TempID = en_str_Vertical_Position;
            break;
        case HWND_SUB_MENU_DISSET_SHP_ITEM_TEXT:
            u16TempID = en_str_Sharpness;
            break;
        case HWND_SUB_MENU_DISSET_PIXCLK_ITEM_TEXT:
            u16TempID = en_str_Pixel_Clock;
            break;
        case HWND_SUB_MENU_DISSET_PHS_ITEM_TEXT:
            u16TempID = en_str_Phase;
            break;

        case HWND_SUB_MENU_OTHERSET_LAN_VALUE_ITEM_TEXT:
        {
            if(u8Language == LANGUAGE_ENGLISH)
                u16TempID = en_str_English;
            else
                u16TempID = en_str_Chinese_Sim;
            break;
        }
        case HWND_SUB_MENU_OTHERSET_LANGUAGE_ITEM_TEXT:
            u16TempID = en_str_Language;
            break;
        case HWND_SUB_MENU_DISSET_ASPRAT_ITEM_VALUE_TEXT:
        {
            if(USER_PREF_WIDE_MODE(SrcInputPortM) == eASPECT_FULL)
                u16TempID = en_str_Full_Screen;
            else if(USER_PREF_WIDE_MODE(SrcInputPortM) == eASPECT_NATIVE)
                u16TempID = en_str_Auto;
            else if(USER_PREF_WIDE_MODE(SrcInputPortM) == eASPECT_5_4)
                u16TempID = en_str_5By4;
            else if(USER_PREF_WIDE_MODE(SrcInputPortM) == eASPECT_4_3)
                u16TempID = en_str_4By3;
            else if(USER_PREF_WIDE_MODE(SrcInputPortM) == eASPECT_16_9)
                u16TempID = en_str_Wide_16By9;
            else
                u16TempID = en_str_One_By_One;
            break;
        }

        case HWND_SUB_MENU_COLSET_PREMOD_VALUE_ITEM_TEXT:
        {
            if(CURRENT_COLOR_TEMP == CT_STANDARD)
                u16TempID = en_str_6500K;
            else if(CURRENT_COLOR_TEMP == CT_MOVIE)
                u16TempID = en_str_Movie;
            else if(CURRENT_COLOR_TEMP == CT_GAME)
                u16TempID = en_str_Game;
            else if(CURRENT_COLOR_TEMP == CT_WARM)
                u16TempID = en_str_5700K;
            else if(CURRENT_COLOR_TEMP == CT_COOL)
                u16TempID = en_str_9300K;
            else  //CUSTOMECOLOR
                u16TempID = en_str_Custom_Color;
            break;
        }
        case HWND_SUB_MENU_COLSET_SUPERRES_VALUE_ITEM_TEXT:
        {

            if(CURRENT_SUPER_RESOLUTION == SUPER_RESOLUTION_OFF)
                u16TempID = en_str_Off;
            else if(CURRENT_SUPER_RESOLUTION == SUPER_RESOLUTION_HIGH)
                u16TempID = en_str_High;
            else if(CURRENT_SUPER_RESOLUTION == SUPER_RESOLUTION_MIDDLE)
                u16TempID = en_str_Middle;
            else
                u16TempID = en_str_Low;
            break;
        }
        case HWND_SUB_MENU_COLSET_INPCOLFMT_VALUE_ITEM_TEXT:
        {
            if(CURRENT_INPUT_COLOR_FORMAT == INPUT_COLOR_RGB)
                u16TempID = en_str_RGB;
            else if(CURRENT_INPUT_COLOR_FORMAT == INPUT_COLOR_YPBPR)
                u16TempID = en_str_YPbPr;
            else
                u16TempID = en_str_Auto;
            break;
        }
        case HWND_SUB_MENU_DCR_VALUE_TEXT:
        case HWND_SUB_MENU_PAGE_BRI_ITEM_VALUE_LIST:
        {
            #if ENABLE_DCR
            if(!GetDCREnableRealValue())
                u16TempID = en_str_On;
            else
                u16TempID = en_str_Off;
            #else
                u16TempID = en_str_Off;
            #endif
                break;
        }
        case HWND_SUB_MENU_COLSET_HUE_ITEM_TEXT:
        {
            if(CURRENT_COLOR_TEMP == CT_USER)
                u16TempID = en_str_CustomeR;
            else
                u16TempID = en_str_Hue;
                break;
        }
        case HWND_SUB_MENU_COLSET_SAT_ITEM_TEXT:
        {
            if(CURRENT_COLOR_TEMP == CT_USER)
                u16TempID = en_str_CustomeG;
            else
                u16TempID = en_str_Saturation;
                break;
        }
        case HWND_SUB_MENU_ITEM5_TEXT:
        {
            if(CURRENT_COLOR_TEMP == CT_USER)
                u16TempID = en_str_CustomeB;
                break;
        }
        case HWND_SUB_MENU_HUE_VALUE_TEXT:
        case HWND_SUB_MENU_COLSET_HUE_VALUE_ITEM_TEXT:
        {
            if(CURRENT_COLOR_TEMP != CT_USER)
            #if ENABLE_MOVIE_GAME_PRESET
                return MApp_ZUI_API_GetU16String(GetHueValue());
            #else
                return MApp_ZUI_API_GetU16String(0);
            #endif
            else
                return MApp_ZUI_API_GetU16String(GetUserRedColorValue());
        }
        case HWND_SUB_MENU_COLSET_SAT_VALUE_ITEM_TEXT:
        case HWND_SUB_MENU_SAT_VALUE_TEXT:
        {
            if(CURRENT_COLOR_TEMP != CT_USER)
            #if ENABLE_MOVIE_GAME_PRESET
                return MApp_ZUI_API_GetU16String(GetSaturationValue());
            #else
                return MApp_ZUI_API_GetU16String(0);
            #endif
            else
                return MApp_ZUI_API_GetU16String(GetUserGreenColorValue());
        }
        case HWND_SUB_MENU_ITEM5_VALUE_TEXT:
        case HWND_SUB_MENU_CUSTOM_B_VALUE_ITEM_TEXT:
            return MApp_ZUI_API_GetU16String(GetUserBlueColorValue());
        case HWND_SUB_MENU_MENUTPR_VALUE_TEXT:
        case HWND_SUB_MENU_OTHERSET_MENUTPC_VALUE_ITEM_TEXT:
            return MApp_ZUI_API_GetU16String(GetTransparencyValue());
		#if ENABLE_VGA_INPUT
        case HWND_SUB_MENU_DISSET_PHS_ITEM_VALUE_TEXT:
        case HWND_SUB_MENU_PHS_VALUE_TEXT:
        {
            return MApp_ZUI_API_GetU16String(GetPhaseValue());
        }
        case HWND_SUB_MENU_DISSET_PIXCLK_ITEM_VALUE_TEXT:
        case HWND_SUB_MENU_PIXCLK_VALUE_TEXT:
        {
            return MApp_ZUI_API_GetU16String(GetClockValue());
        }
		#endif
        case HWND_SUB_MENU_DISSET_SHP_ITEM_VALUE_TEXT:
        case HWND_SUB_MENU_SHP_VALUE_TEXT:
        {
            return MApp_ZUI_API_GetU16String(GetSharpnessValue());
        }
		#if ENABLE_VGA_INPUT
        case HWND_SUB_MENU_DISSET_VP_ITEM_VALUE_TEXT:
        case HWND_SUB_MENU_VP_VALUE_TEXT:
        {
            return MApp_ZUI_API_GetU16String(GetVPositionValue());
        }
        case HWND_SUB_MENU_DISSET_HP_ITEM_VALUE_TEXT:
        case HWND_SUB_MENU_HP_VALUE_TEXT:
        {
            return MApp_ZUI_API_GetU16String(GetHPositionValue());
        }
		#endif
        case HWND_SUB_MENU_BRI_VALUE_TEXT:
        case HWND_SUB_MENU_BRIADJ_VALUE_TEXT:
        {
            return MApp_ZUI_API_GetU16String(GetBrightnessValue());
        }
        case HWND_SUB_MENU_CON_VALUE_TEXT:
        case HWND_SUB_MENU_CONADJ_VALUE_TEXT:
        {
            return MApp_ZUI_API_GetU16String(GetContrastValue());
        }

        default:
            break;
    }


    if (u16TempID != Empty)
        return MApp_ZUI_API_GetString(u16TempID);

    return 0; //for empty string....
}

S16 MApp_ZUI_ACT_GetMainMenuDynamicValue(HWND hwnd)
{
    switch(hwnd)
    {
        case HWND_SUB_MENU_MULTIWIN_PIPHP_VALUE_LIST:
        #if ENABLE_MULTI_WINDOW_SETTINGS_MENU
            return GetPIPHPositionValue();
        #else
            return 0;
        #endif
        case HWND_SUB_MENU_MULTIWIN_PIPVP_VALUE_LIST:
        #if ENABLE_MULTI_WINDOW_SETTINGS_MENU
            return GetPIPVPositionValue();
        #else
            return 0;
        #endif
        case HWND_SUB_MENU_BRIADJ_VALUE_LIST:
            return GetBrightnessValue();
        case HWND_SUB_MENU_CONADJ_VALUE_LIST:
            return GetContrastValue();
		#if ENABLE_VGA_INPUT
        case HWND_SUB_MENU_HP_VALUE_LIST:
            return GetHPositionValue();
        case HWND_SUB_MENU_VP_VALUE_LIST:
            return GetVPositionValue();
		#endif	
        case HWND_SUB_MENU_SHP_VALUE_LIST:
            return GetSharpnessValue();
		#if ENABLE_VGA_INPUT	
        case HWND_SUB_MENU_PIXCLK_VALUE_LIST:
            return GetClockValue();
        case HWND_SUB_MENU_PHS_VALUE_LIST:
            return GetPhaseValue();
		#endif	
        case HWND_SUB_MENU_HUE_VALUE_LIST:
        {
            if(CURRENT_COLOR_TEMP == CT_USER)
                return GetUserRedColorValue();
            else
            #if ENABLE_MOVIE_GAME_PRESET
                return GetHueValue();
            #else
                return 0;
            #endif
        }
        case HWND_SUB_MENU_SAT_VALUE_LIST:
        {
            if(CURRENT_COLOR_TEMP == CT_USER)
                return GetUserGreenColorValue();
            else
            #if ENABLE_MOVIE_GAME_PRESET
                return GetSaturationValue();
            #else
                return 0;
            #endif
        }
        case HWND_SUB_MENU_CUSTOM_B_VALUE_LIST:
            if(CURRENT_COLOR_TEMP == CT_USER)
                return GetUserBlueColorValue();
            else
                break;
        case HWND_SUB_MENU_MENUTPR_VALUE_LIST:
            return GetTransparencyValue();


        default:
            break;
    }
    return 0; //for empty  data
}
OSD_COLOR MApp_ZUI_ACT_GetMainMenuDynamicColor(HWND hwnd, DRAWSTYLE_TYPE type, OSD_COLOR colorOriginal)
{
    //UNUSED(type);
    //UNUSED(colorOriginal);
    switch(hwnd)
    {
        case HWND_SUB_MENU_MULTIWIN_WINMODES_ITEM_TEXT:
            if(type == DS_NORMAL &&  HWND_SUB_MENU_MULTIWIN_WINMODES_ITEM == MApp_ZUI_API_GetFocusCheckpoint() && bPreFocusFlag == 1)
                return MAIN_MENU_COLOR_RED;
            break;
        case HWND_SUB_MENU_MULTIWIN_WINSEL_ITEM_TEXT:
            if(type == DS_NORMAL &&  HWND_SUB_MENU_MULTIWIN_WINSEL_ITEM == MApp_ZUI_API_GetFocusCheckpoint() && bPreFocusFlag == 1)
                return MAIN_MENU_COLOR_RED;
            break;
        case HWND_SUB_MENU_MULTIWIN_PIPSIZE_ITEM_TEXT:
            if(type == DS_NORMAL &&  HWND_SUB_MENU_MULTIWIN_PIPSIZE_ITEM == MApp_ZUI_API_GetFocusCheckpoint() && bPreFocusFlag == 1)
                return MAIN_MENU_COLOR_RED;
            break;
        case HWND_SUB_MENU_MULTIWIN_PIPHP_ITEM_TEXT:
            if(type == DS_NORMAL &&  HWND_SUB_MENU_MULTIWIN_PIPHP_ITEM == MApp_ZUI_API_GetFocusCheckpoint() && bPreFocusFlag == 1)
                return MAIN_MENU_COLOR_RED;
            break;
        case HWND_SUB_MENU_MULTIWIN_PIPVP_ITEM_TEXT:
        {
            if(type == DS_NORMAL &&  HWND_SUB_MENU_MULTIWIN_PIPVP_ITEM == MApp_ZUI_API_GetFocusCheckpoint() && bPreFocusFlag == 1)
                return MAIN_MENU_COLOR_RED;
            break;
        }
        case HWND_MAIN_MENU_MULTI_WIN_SETTINGS_TEXT:
        {
            if((type == DS_NORMAL &&  HWND_MAIN_MENU_MULTI_WIN_SETTINGS_ITEM == MApp_ZUI_API_GetFocusCheckpoint() && bPreFocusFlag == 1)
                || HWND_SUB_MENU_MULTIWIN_WINMODES_ITEM == MApp_ZUI_API_GetFocus())
                return MAIN_MENU_COLOR_RED;
            break;
        }
        case HWND_MAIN_MENU_BRI_TEXT:
        {
            if(type == DS_NORMAL &&  HWND_MAIN_MENU_BRI_ITEM == MApp_ZUI_API_GetFocusCheckpoint() && bPreFocusFlag == 1)
                return MAIN_MENU_COLOR_RED;
            break;
        }
        case HWND_SUB_MENU_BRI_TEXT:
        {
            if(type == DS_NORMAL &&  HWND_SUB_MENU_BRI_ITEM == MApp_ZUI_API_GetFocusCheckpoint() && bPreFocusFlag == 1)
                return MAIN_MENU_COLOR_RED;
            break;
        }
        case HWND_SUB_MENU_CON_TEXT:
        {
            if(type == DS_NORMAL &&  HWND_SUB_MENU_CON_ITEM == MApp_ZUI_API_GetFocusCheckpoint() && bPreFocusFlag == 1)
                return MAIN_MENU_COLOR_RED;
            break;
        }
        case HWND_SUB_MENU_DYNAMIC_CON_TEXT:
        {
            if(type == DS_NORMAL &&  HWND_SUB_MENU_DYNAMIC_CON_ITEM == MApp_ZUI_API_GetFocusCheckpoint() && bPreFocusFlag == 1)
                return MAIN_MENU_COLOR_RED;
            break;
        }
        case HWND_MAIN_MENU_INPUT_SOURCE_TEXT:
        {
            if(type == DS_NORMAL &&  HWND_MAIN_MENU_INPUT_SOURCE_ITEM == MApp_ZUI_API_GetFocusCheckpoint() && bPreFocusFlag == 1)
                return MAIN_MENU_COLOR_RED;
            break;
        }

        case HWND_SUB_MENU_COLSET_INPCOLFMT_ITEM_TEXT:
        {
             if(type == DS_NORMAL &&  HWND_SUB_MENU_COLSET_INPCOLFMT_ITEM == MApp_ZUI_API_GetFocusCheckpoint() && bPreFocusFlag == 1)
                return MAIN_MENU_COLOR_RED;
            break;
        }
        case HWND_SUB_MENU_COLSET_PREMOD_ITEM_TEXT:
        {
             if(type == DS_NORMAL &&  HWND_SUB_MENU_COLSET_PREMOD_ITEM == MApp_ZUI_API_GetFocusCheckpoint() && bPreFocusFlag == 1)
                return MAIN_MENU_COLOR_RED;
            break;
        }
        case HWND_SUB_MENU_COLSET_SUPERRES_ITEM_TEXT:
        {
             if(type == DS_NORMAL &&  HWND_SUB_MENU_COLSET_SUPERRES_ITEM == MApp_ZUI_API_GetFocusCheckpoint() && bPreFocusFlag == 1)
                return MAIN_MENU_COLOR_RED;
            break;
        }
        case HWND_SUB_MENU_COLSET_HUE_ITEM_TEXT:
        {
            if(type == DS_NORMAL &&  HWND_SUB_MENU_COLSET_HUE_ITEM == MApp_ZUI_API_GetFocusCheckpoint() && bPreFocusFlag == 1)
                return MAIN_MENU_COLOR_RED;
            break;
        }

        case HWND_SUB_MENU_COLSET_SAT_ITEM_TEXT:
        {
            if(type == DS_NORMAL &&  HWND_SUB_MENU_COLSET_SAT_ITEM == MApp_ZUI_API_GetFocusCheckpoint() && bPreFocusFlag == 1)
                return MAIN_MENU_COLOR_RED;
            break;
        }

        case HWND_SUB_MENU_ITEM5_TEXT:
        {
            if(type == DS_NORMAL &&  HWND_SUB_MENU_ITEM5 == MApp_ZUI_API_GetFocusCheckpoint() && bPreFocusFlag == 1)
                return MAIN_MENU_COLOR_RED;
            break;
        }
        case HWND_MAIN_MENU_COLOR_SETTINGS_TEXT:
        {
            if(type == DS_NORMAL &&  HWND_MAIN_MENU_COLOR_SETTINGS_ITEM == MApp_ZUI_API_GetFocusCheckpoint() && bPreFocusFlag == 1)
                return MAIN_MENU_COLOR_RED;
            break;
        }
        case HWND_SUB_MENU_DISSET_RESPONSETIME_ITEM_TEXT:
        {
             if(type == DS_NORMAL &&  HWND_SUB_MENU_DISSET_RESPONSETIME_ITEM == MApp_ZUI_API_GetFocusCheckpoint() && bPreFocusFlag == 1)
                return MAIN_MENU_COLOR_RED;
             else
                return colorOriginal;
        }
        case HWND_SUB_MENU_DISSET_PHS_ITEM_TEXT:
        case HWND_SUB_MENU_DISSET_PIXCLK_ITEM_TEXT:
        case HWND_SUB_MENU_DISSET_SHP_ITEM_TEXT:
        case HWND_SUB_MENU_DISSET_VP_ITEM_TEXT:
        case HWND_SUB_MENU_DISSET_HP_ITEM_TEXT:
        case HWND_SUB_MENU_DISSET_ASPRAT_ITEM_TEXT:
        case HWND_MAIN_MENU_DISPLAY_SETTINGS_TEXT:
        {
            if(type == DS_NORMAL &&  HWND_MAIN_MENU_DISPLAY_SETTINGS_ITEM == MApp_ZUI_API_GetFocusCheckpoint() && bPreFocusFlag == 1)
                return MAIN_MENU_COLOR_RED;
            else if(type == DS_NORMAL &&  HWND_SUB_MENU_DISSET_ASPRAT_ITEM == MApp_ZUI_API_GetFocusCheckpoint() && bPreFocusFlag == 1)
                return MAIN_MENU_COLOR_RED;
            else if(type == DS_NORMAL &&  HWND_SUB_MENU_DISSET_HP_ITEM == MApp_ZUI_API_GetFocusCheckpoint() && bPreFocusFlag == 1)
                return MAIN_MENU_COLOR_RED;
            else if(type == DS_NORMAL &&  HWND_SUB_MENU_DISSET_VP_ITEM == MApp_ZUI_API_GetFocusCheckpoint() && bPreFocusFlag == 1)
                return MAIN_MENU_COLOR_RED;
            else if(type == DS_NORMAL &&  HWND_SUB_MENU_DISSET_SHP_ITEM == MApp_ZUI_API_GetFocusCheckpoint() && bPreFocusFlag == 1)
                return MAIN_MENU_COLOR_RED;
            else if(type == DS_NORMAL &&  HWND_SUB_MENU_DISSET_PIXCLK_ITEM == MApp_ZUI_API_GetFocusCheckpoint() && bPreFocusFlag == 1)
                return MAIN_MENU_COLOR_RED;
            else if(type == DS_NORMAL &&  HWND_SUB_MENU_DISSET_PHS_ITEM == MApp_ZUI_API_GetFocusCheckpoint() && bPreFocusFlag == 1)
                return MAIN_MENU_COLOR_RED;
            else
                return colorOriginal;
        }

        case HWND_SUB_MENU_OTHERSET_TPC_ITEM_TEXT:
        {
            if(type == DS_NORMAL &&  HWND_SUB_MENU_OTHERSET_TPC_ITEM == MApp_ZUI_API_GetFocusCheckpoint() && bPreFocusFlag == 1)
                return MAIN_MENU_COLOR_RED;
                break;
        }
        case HWND_SUB_MENU_OTHERSET_LANGUAGE_ITEM_TEXT:
        case HWND_MAIN_MENU_OTHER_SETTINGS_TEXT:
        {
            if((type == DS_NORMAL &&  HWND_MAIN_MENU_OTHER_SETTINGS_ITEM == MApp_ZUI_API_GetFocusCheckpoint() && bPreFocusFlag == 1)
                ||HWND_SUB_MENU_OTHERSET_LANGUAGE_ITEM == MApp_ZUI_API_GetFocus())
                return MAIN_MENU_COLOR_RED;
            else if(type == DS_NORMAL &&  HWND_SUB_MENU_OTHERSET_LANGUAGE_ITEM == MApp_ZUI_API_GetFocusCheckpoint() && bPreFocusFlag == 1)
                return MAIN_MENU_COLOR_RED;
            else
                return colorOriginal;
        }
        default:
            break;
    }
    return colorOriginal;
}
GUI_ENUM_DYNAMIC_LIST_STATE MApp_ZUI_ACT_QueryMainMenuItemStatus(HWND hwnd)
{
    switch(hwnd)
    {
        case HWND_SUB_MENU_AUTO_SRC_ITEM:
        case HWND_SUB_MENU_VGA_ITEM:
            return EN_DL_STATE_NORMAL;

        case HWND_SUB_MENU_DVI_ITEM:
        {
            #if ENABLE_DVI_INPUT
                return EN_DL_STATE_NORMAL;
            #else
                return EN_DL_STATE_HIDDEN;
            #endif
        }
        case HWND_SUB_MENU_HDMI1_ITEM:
        {
            #if ENABLE_HDMI_INPUT
                return EN_DL_STATE_NORMAL;
            #else
                return EN_DL_STATE_HIDDEN;
            #endif
        }
        case HWND_SUB_MENU_HDMI2_ITEM:
        {
            #if ENABLE_HDMI2ND_INPUT
                return EN_DL_STATE_NORMAL;
            #else
                return EN_DL_STATE_HIDDEN;
            #endif
        }
        case HWND_SUB_MENU_DP1_ITEM:
        {
            #if ENABLE_DP_INPUT
                return EN_DL_STATE_NORMAL;
            #else
                return EN_DL_STATE_HIDDEN;
            #endif
        }
        case HWND_SUB_MENU_DP2_ITEM:
        {
            #if ENABLE_DP2ND_INPUT
                return EN_DL_STATE_NORMAL;
            #else
                return EN_DL_STATE_HIDDEN;
            #endif
        }
        case HWND_SUB_MENU_MULTIWIN_WINSEL_ITEM_VALUE:
        {
            if(USER_PREF_MULTI_WIN_MODE == eMULTIWIN_OFF)
                return EN_DL_STATE_DISABLED;
            else
                return EN_DL_STATE_NORMAL;
        }
        case HWND_SUB_MENU_MULTIWIN_PIPSIZE_ITEM_VALUE:
        case HWND_SUB_MENU_MULTIWIN_PIPHP_ITEM_VALUE:
        case HWND_SUB_MENU_MULTIWIN_PIPVP_ITEM_VALUE:
        {
            if(USER_PREF_MULTI_WIN_MODE != eMULTIWIN_2_PIP)
                return EN_DL_STATE_DISABLED;
            else
                return EN_DL_STATE_NORMAL;
        }
        case HWND_SUB_MENU_DISSET_ASPRATIO_FULL_ITEM:
        case HWND_SUB_MENU_DISSET_ASPRATIO_AUTO_ITEM:
        case HWND_SUB_MENU_DISSET_ASPRATIO_5BY4_ITEM:
        case HWND_SUB_MENU_DISSET_ASPRATIO_4BY3_ITEM:
        case HWND_SUB_MENU_DISSET_ASPRATIO_16BY9_ITEM:
        case HWND_SUB_MENU_DISSET_ASPRATIO_ONEBYONE_ITEM:
        {
            if(USER_PREF_MULTI_WIN_MODE != eMULTIWIN_OFF)
                return EN_DL_STATE_DISABLED;
            else
                return EN_DL_STATE_NORMAL;
        }
        case HWND_SUB_MENU_COLSET_INPCOLFMT_AUTO_ITEM:
        {
            if (USER_PREF_INPUT_PRIORITY == INPUT_PRIORITY_AUTO
				#if ENABLE_VGA_INPUT
				|| USER_PREF_INPUT_PRIORITY == INPUT_PRIORITY_VGA
				#endif	
				)
                return EN_DL_STATE_DISABLED;
            else
                return EN_DL_STATE_NORMAL;
        }
        case HWND_SUB_MENU_MULTIWIN_WINSEL_WIN3_ITEM:
        {
             if(USER_PREF_MULTI_WIN_MODE != eMULTIWIN_3 && USER_PREF_MULTI_WIN_MODE != eMULTIWIN_4)
                return EN_DL_STATE_DISABLED;
             else
                return EN_DL_STATE_NORMAL;
        }
        case HWND_SUB_MENU_MULTIWIN_WINSEL_WIN4_ITEM:
        {
             if(USER_PREF_MULTI_WIN_MODE != eMULTIWIN_4)
                return EN_DL_STATE_DISABLED;
             else
                return EN_DL_STATE_NORMAL;
        }
        case HWND_SUB_MENU_MULTIWIN_WINSEL_ITEM:
        {
            if(USER_PREF_MULTI_WIN_MODE == eMULTIWIN_OFF)
                return EN_DL_STATE_DISABLED;
            else
                return EN_DL_STATE_NORMAL;
        }
        case HWND_SUB_MENU_MULTIWIN_PIPSIZE_ITEM:
        case HWND_SUB_MENU_MULTIWIN_PIPHP_ITEM:
        case HWND_SUB_MENU_MULTIWIN_PIPVP_ITEM:
        {
            if(USER_PREF_MULTI_WIN_MODE != eMULTIWIN_2_PIP)
                return EN_DL_STATE_DISABLED;
            else
                return EN_DL_STATE_NORMAL;
        }
        case HWND_SUB_MENU_COLSET_SUPERRES_ITEM:
        case HWND_SUB_MENU_COLSET_SUPERRES_VALUE_ITEM:
        #if ENABLE_SUPER_RESOLUTION
            return EN_DL_STATE_NORMAL;
        #else
            return EN_DL_STATE_DISABLED;
        #endif
        case HWND_SUB_MENU_ITEM5:
        case HWND_SUB_MENU_ITEM5_VALUE:
        {
            if(CURRENT_COLOR_TEMP == CT_USER)
                return EN_DL_STATE_NORMAL;
            else
                return EN_DL_STATE_DISABLED;
        }
        case HWND_SUB_MENU_COLSET_HUE_VALUE_ITEM:
        case HWND_SUB_MENU_COLSET_SAT_VALUE_ITEM:
        case HWND_SUB_MENU_COLSET_HUE_ITEM:
        case HWND_SUB_MENU_COLSET_SAT_ITEM:
        {
        #if ENABLE_MOVIE_GAME_PRESET
            if(CURRENT_COLOR_TEMP == CT_MOVIE || CURRENT_COLOR_TEMP == CT_GAME || CURRENT_COLOR_TEMP == CT_USER)
                return EN_DL_STATE_NORMAL;
            else
                return EN_DL_STATE_DISABLED;
        #else
                return EN_DL_STATE_DISABLED;
        #endif
        }
        case HWND_SUB_MENU_DYNAMIC_CON_ITEM:
        case HWND_SUB_MENU_DCR_VALUE_ITEM:
        {
            #if ENABLE_DCR
            if(CURRENT_COLOR_TEMP == CT_MOVIE ||CURRENT_COLOR_TEMP == CT_GAME )
                return EN_DL_STATE_NORMAL;
            else
                return EN_DL_STATE_DISABLED;
            #else
                return EN_DL_STATE_DISABLED;
            #endif
        }
        case HWND_MAIN_MENU_AUTO_ADJUST_ITEM:
        {
			#if ENABLE_VGA_INPUT
            if(USER_PREF_INPUT_TYPE !=INPUT_VGA)
                return EN_DL_STATE_DISABLED;
            else
			#endif	
                return EN_DL_STATE_NORMAL;
        }

        case HWND_SUB_MENU_DISSET_ASPRAT_ITEM:
        {
            #if ENABLE_WIDE_MODE
            return EN_DL_STATE_NORMAL;
            #else
            return EN_DL_STATE_DISABLED;
            #endif
        }
        case HWND_SUB_MENU_DISSET_HP_ITEM:
        case HWND_SUB_MENU_DISSET_VP_ITEM:
        case HWND_SUB_MENU_DISSET_PIXCLK_ITEM:
        case HWND_SUB_MENU_DISSET_HP_ITEM_VALUE:
        case HWND_SUB_MENU_DISSET_VP_ITEM_VALUE:
        case HWND_SUB_MENU_DISSET_PIXCLK_ITEM_VALUE:
        case HWND_SUB_MENU_DISSET_PHS_ITEM_VALUE:
        case HWND_SUB_MENU_DISSET_PHS_ITEM:
        {
            if(!CURRENT_INPUT_IS_VGA())
                return EN_DL_STATE_DISABLED;
            else
                return EN_DL_STATE_NORMAL;
        }
        case HWND_MAIN_MENU_MULTI_WIN_SETTINGS_ITEM:
        {
            #if ENABLE_MULTI_WINDOW_SETTINGS_MENU
                return EN_DL_STATE_NORMAL;
            #else
                return EN_DL_STATE_DISABLED;
            #endif
        }
        default:
            break;
    }

    return EN_DL_STATE_NORMAL;
}
void MApp_ZUI_ACT_TerminateMainMenu(void)
{
    ZUI_MSG(printf("[]term:IDLE\n");)
}


//////////////////////////////////////////
BOOLEAN MApp_ZUI_ACT_ExecuteMainMenuAction(U16 act)
{
    HWND hwnd = MApp_ZUI_API_GetFocus();

    switch(act)
    {
        case EN_EXE_CLOSE_CURRENT_OSD:
        {
            SaveMonitorSetting();
            Delay1ms(50);
            MApp_ZUI_ACT_ShutdownOSD();
            return TRUE;
        }
        case EN_EXE_IDLE_START_MAINMENU:
        {
            printf("start mainmenu! \n");
        }
            return TRUE;
        case EN_EXE_GOTO_SELECT_VALUE:
            return TRUE;

        case EN_EXE_CLEAN_VALUE_UP_KEY:
        {
            bPreFocusFlag = 0;
            MApp_ZUI_API_SetFocusByNav(MApp_ZUI_API_GetFocus(), NAV_UP);
        }
            return TRUE;
        case EN_EXE_CLEAN_VALUE_DOWN_KEY:
        {
            bPreFocusFlag = 0;
            MApp_ZUI_API_SetFocusByNav(MApp_ZUI_API_GetFocus(), NAV_DOWN);
        }
            return TRUE;
        case EN_EXE_GOTO_PREVIOUS:
        {
            bPreFocusFlag = 0;
            switch(hwnd)
            {
                case HWND_SUB_MENU_MULTIWIN_WINMODES_OFF_ITEM:
                case HWND_SUB_MENU_MULTIWIN_WINMODES_PIPMODE_ITEM:
                case HWND_SUB_MENU_MULTIWIN_WINMODES_PBPMODE1_ITEM:
                case HWND_SUB_MENU_MULTIWIN_WINMODES_PBPMODE2_ITEM:
                case HWND_SUB_MENU_MULTIWIN_WINMODES_TRIPLEMODE_ITEM:
                case HWND_SUB_MENU_MULTIWIN_WINMODES_QUADMODE_ITEM:
                {
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_MULTIWIN_WINMODES_ITEM_LIST,SW_HIDE);
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_MULTIWIN_ITEM_VALUE_LIST,SW_SHOW);
                    MApp_ZUI_API_SetFocus(HWND_SUB_MENU_MULTIWIN_WINMODES_ITEM);
                }
                return TRUE;
                case HWND_SUB_MENU_MULTIWIN_PIPHP_VALUE_ITEM:
                {
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_MULTIWIN_PIPHP_VALUE_ITEM,SW_HIDE);
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_MULTIWIN_PIPHP_ITEM_VALUE,SW_SHOW);
                    MApp_ZUI_API_SetFocus(HWND_SUB_MENU_MULTIWIN_PIPHP_ITEM);
                }
                return TRUE;
                case HWND_SUB_MENU_MULTIWIN_PIPVP_VALUE_ITEM:
                {
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_MULTIWIN_PIPVP_VALUE_ITEM,SW_HIDE);
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_MULTIWIN_PIPVP_ITEM_VALUE,SW_SHOW);
                    MApp_ZUI_API_SetFocus(HWND_SUB_MENU_MULTIWIN_PIPVP_ITEM);
                }
                return TRUE;
                case HWND_SUB_MENU_MULTIWIN_WINMODES_ITEM:
                case HWND_SUB_MENU_MULTIWIN_WINSEL_ITEM:
                case HWND_SUB_MENU_MULTIWIN_PIPSIZE_ITEM:
                case HWND_SUB_MENU_MULTIWIN_PIPHP_ITEM:
                case HWND_SUB_MENU_MULTIWIN_PIPVP_ITEM:
                    MApp_ZUI_API_SetFocus(HWND_MAIN_MENU_MULTI_WIN_SETTINGS_ITEM);
                    return TRUE;
                case HWND_SUB_MENU_MULTIWIN_WINSEL_WIN1_ITEM:
                case HWND_SUB_MENU_MULTIWIN_WINSEL_WIN2_ITEM:
                case HWND_SUB_MENU_MULTIWIN_WINSEL_WIN3_ITEM:
                case HWND_SUB_MENU_MULTIWIN_WINSEL_WIN4_ITEM:
                {
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_MULTIWIN_WINSEL_ITEM_LIST,SW_HIDE);
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_MULTIWIN_ITEM_VALUE_LIST,SW_SHOW);
                    MApp_ZUI_API_SetFocus(HWND_SUB_MENU_MULTIWIN_WINSEL_ITEM);
                }
                return TRUE;
                case HWND_SUB_MENU_MULTIWIN_PIPSIZE_SMALL_ITEM:
                case HWND_SUB_MENU_MULTIWIN_PIPSIZE_MEDIUM_ITEM:
                case HWND_SUB_MENU_MULTIWIN_PIPSIZE_LARGE_ITEM:
                case HWND_SUB_MENU_MULTIWIN_PIPSIZE_HUGE_ITEM:
                {
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_MULTIWIN_PIPSIZE_ITEM_LIST,SW_HIDE);
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_MULTIWIN_ITEM_VALUE_LIST,SW_SHOW);
                    MApp_ZUI_API_SetFocus(HWND_SUB_MENU_MULTIWIN_PIPSIZE_ITEM);
                }
                return TRUE;

                case HWND_MAIN_MENU_BRI_ITEM:
                case HWND_MAIN_MENU_AUTO_ADJUST_ITEM:
                case HWND_MAIN_MENU_INPUT_SOURCE_ITEM:
                case HWND_MAIN_MENU_COLOR_SETTINGS_ITEM:
                case HWND_MAIN_MENU_DISPLAY_SETTINGS_ITEM:
                case HWND_MAIN_MENU_OTHER_SETTINGS_ITEM:
                case HWND_MAIN_MENU_MULTI_WIN_SETTINGS_ITEM:
                {
                    SaveMonitorSetting();
                    Delay1ms(50);
                    MApp_ZUI_API_InvalidateAllSuccessors(HWND_MAIN_MENU_BRI_ITEM);
                    MApp_ZUI_ACT_TransitionEffectBegin(EN_EFFMODE_CLOSE, E_ZUI_STATE_TERMINATE);
                }
                    return TRUE;
                case HWND_SUB_MENU_COLSET_INPCOLFMT_RGB_ITEM:
                case HWND_SUB_MENU_COLSET_INPCOLFMT_YPBPR_ITEM:
                case HWND_SUB_MENU_COLSET_INPCOLFMT_AUTO_ITEM:
                {
                    CURRENT_INPUT_COLOR_FORMAT = g_u8OriginalSetting;
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_COL_SET,SW_HIDE);
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_COL_VALUE_ITEM_LIST,SW_SHOW);
                    MApp_ZUI_API_SetFocus(HWND_SUB_MENU_COLSET_INPCOLFMT_ITEM);
                }
                    return TRUE;
                case HWND_SUB_MENU_BRICON_DYN_CON_ON_ITEM:
                case HWND_SUB_MENU_BRICON_DYN_CON_OFF_ITEM:
                {
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_BRICON_DYNAMIC_CONTRAST_ITEM,SW_HIDE);
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_PAGE_BRI_ITEM_VALUE_LIST,SW_SHOW);
                    MApp_ZUI_API_SetFocus(HWND_SUB_MENU_DYNAMIC_CON_ITEM);
                }
                    return TRUE;

                case HWND_SUB_MENU_MENUTPR_VALUE_ITEM:
                {
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_MENUTPR_VALUE_ITEM,SW_HIDE);
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_OTHER_SET_ITEM_VALUE_LIST,SW_SHOW);
                    MApp_ZUI_API_SetFocus(HWND_SUB_MENU_OTHERSET_TPC_ITEM);
                }
                return TRUE;
                case HWND_SUB_MENU_DISSET_RESPONSETIME_NORMAL_ITEM:
                case HWND_SUB_MENU_DISSET_RESPONSETIME_OD_ITEM:
                {
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_DISSET_RESPONSETIME_ITEM_LIST,SW_HIDE);
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_DISSET_ITEM_VALUE_LIST,SW_SHOW);
                    MApp_ZUI_API_SetFocus(HWND_SUB_MENU_DISSET_RESPONSETIME_ITEM);
                }
                return TRUE;
                case HWND_SUB_MENU_OTHERSET_LAN_ENG_ITEM:
                case HWND_SUB_MENU_OTHERSET_LAN_SIM_ITEM:
                {
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_OTHER_SET_ITEM,SW_HIDE);
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_OTHER_SET_ITEM_VALUE_LIST,SW_SHOW);
                    MApp_ZUI_API_SetFocus(HWND_SUB_MENU_OTHERSET_LANGUAGE_ITEM);
                }
                return TRUE;
                case HWND_SUB_MENU_HP_VALUE_ITEM:
                {
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_HP_VALUE_ITEM,SW_HIDE);
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_DISSET_ITEM_VALUE_LIST,SW_SHOW);
                    MApp_ZUI_API_SetFocus(HWND_SUB_MENU_DISSET_HP_ITEM);
                }
                    return TRUE;
                case HWND_SUB_MENU_VP_VALUE_ITEM:
                {
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_VP_VALUE_ITEM,SW_HIDE);
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_DISSET_ITEM_VALUE_LIST,SW_SHOW);
                    MApp_ZUI_API_SetFocus(HWND_SUB_MENU_DISSET_VP_ITEM);
                }
                    return TRUE;
                case HWND_SUB_MENU_SHP_VALUE_ITEM:
                {
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_SHP_VALUE_ITEM,SW_HIDE);
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_DISSET_ITEM_VALUE_LIST,SW_SHOW);
                    MApp_ZUI_API_SetFocus(HWND_SUB_MENU_DISSET_SHP_ITEM);
                }
                    return TRUE;
                case HWND_SUB_MENU_PIXCLK_VALUE_ITEM:
                {
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_PIXCLK_VALUE_ITEM,SW_HIDE);
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_DISSET_ITEM_VALUE_LIST,SW_SHOW);
                    MApp_ZUI_API_SetFocus(HWND_SUB_MENU_DISSET_PIXCLK_ITEM);
                }
                    return TRUE;
                case HWND_SUB_MENU_PHS_VALUE_ITEM:
                {
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_PHS_VALUE_ITEM,SW_HIDE);
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_DISSET_ITEM_VALUE_LIST,SW_SHOW);
                    MApp_ZUI_API_SetFocus(HWND_SUB_MENU_DISSET_PHS_ITEM);
                }
                    return TRUE;
                case HWND_SUB_MENU_DISSET_ASPRATIO_FULL_ITEM:
                case HWND_SUB_MENU_DISSET_ASPRATIO_AUTO_ITEM:
                case HWND_SUB_MENU_DISSET_ASPRATIO_16BY9_ITEM:
                case HWND_SUB_MENU_DISSET_ASPRATIO_4BY3_ITEM:
                case HWND_SUB_MENU_DISSET_ASPRATIO_5BY4_ITEM:
                case HWND_SUB_MENU_DISSET_ASPRATIO_ONEBYONE_ITEM:
                {
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_DISSET_ASPRATIO_ITEM_LIST,SW_HIDE);
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_DISSET_ITEM_VALUE_LIST,SW_SHOW);
                    MApp_ZUI_API_SetFocus(HWND_SUB_MENU_DISSET_ASPRAT_ITEM);
                }
                    return TRUE;

                case HWND_SUB_MENU_BRIADJ_VALUE_ITEM:
                {
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_BRIADJ_VALUE_ITEM,SW_HIDE);
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_PAGE_BRI_ITEM_VALUE_LIST,SW_SHOW);
                    MApp_ZUI_API_SetFocus(HWND_SUB_MENU_BRI_ITEM);
                }
                    return TRUE;
                case HWND_SUB_MENU_CONADJ_VALUE_ITEM:
                {
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_CONADJ_VALUE_ITEM,SW_HIDE);
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_PAGE_BRI_ITEM_VALUE_LIST,SW_SHOW);
                    MApp_ZUI_API_SetFocus(HWND_SUB_MENU_CON_ITEM);
                }
                    return TRUE;
                case HWND_SUB_MENU_HUE_VALUE_ITEM:
                {
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_HUE_VALUE_ITEM,SW_HIDE);
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_COLSET_HUE_VALUE_ITEM,SW_SHOW);
                    MApp_ZUI_API_SetFocus(HWND_SUB_MENU_COLSET_HUE_ITEM);
                }
                return TRUE;
                case HWND_SUB_MENU_SAT_VALUE_ITEM:
                {
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_SAT_VALUE_ITEM,SW_HIDE);
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_COLSET_SAT_VALUE_ITEM,SW_SHOW);
                    MApp_ZUI_API_SetFocus(HWND_SUB_MENU_COLSET_SAT_ITEM);
                }
                return TRUE;
                case HWND_SUB_MENU_CUSTOM_B_VALUE_ITEM: //
                {
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_CUSTOM_B_VALUE_ITEM,SW_HIDE);
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_ITEM5_VALUE,SW_SHOW);
                    MApp_ZUI_API_SetFocus(HWND_SUB_MENU_ITEM5);
                }
                return TRUE;
                case HWND_SUB_MENU_COLSET_PREMODE_6500K_ITEM:
                case HWND_SUB_MENU_COLSET_PREMODE_MOVIE_ITEM:
                case HWND_SUB_MENU_COLSET_PREMODE_GAME_ITEM:
                case HWND_SUB_MENU_COLSET_INPCOLFMT_5700K_ITEM:
                case HWND_SUB_MENU_COLSET_INPCOLFMT_9300K_ITEM:
                case HWND_SUB_MENU_COLSET_INPCOLFMT_CUSCOL_ITEM:
                { //GOTO_PREVIOUS
                    CURRENT_COLOR_TEMP = g_u8OriginalSetting;
                    g_u8MenuTempValue = g_u8OriginalSetting;
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_COL_SET,SW_HIDE);
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_COL_SET_PAGE,SW_HIDE);
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_COL_SET_PAGE,SW_SHOW);
                    MApp_ZUI_API_SetFocus(HWND_SUB_MENU_COLSET_PREMOD_ITEM);
                    SetColorPresetMode();
                }
                    return TRUE;
                case HWND_SUB_MENU_COLSET_SUPERRES_OFF_ITEM:
                case HWND_SUB_MENU_COLSET_SUPERRES_HIGH_ITEM:
                case HWND_SUB_MENU_COLSET_SUPERRES_MIDDLE_ITEM:
                case HWND_SUB_MENU_COLSET_SUPERRES_LOW_ITEM:
                {
                    CURRENT_SUPER_RESOLUTION = g_u8OriginalSetting;
                    g_u8MenuTempValue = g_u8OriginalSetting;
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_COLSET_SUPERRES_ITEM_LIST,SW_HIDE);
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_COL_VALUE_ITEM_LIST,SW_SHOW);
                    MApp_ZUI_API_SetFocus(HWND_SUB_MENU_COLSET_SUPERRES_ITEM);
                #if ENABLE_SUPER_RESOLUTION
                    SetSuperResolutionFunction();
                #endif
                }
                return TRUE;
                case HWND_SUB_MENU_BRI_ITEM:
                case HWND_SUB_MENU_CON_ITEM:
                case HWND_SUB_MENU_DYNAMIC_CON_ITEM:
                {
                    MApp_ZUI_API_SetFocus(HWND_MAIN_MENU_BRI_ITEM);
                }
                return TRUE;

                case HWND_SUB_MENU_AUTO_SRC_ITEM:
                case HWND_SUB_MENU_VGA_ITEM:
                case HWND_SUB_MENU_DVI_ITEM:
                case HWND_SUB_MENU_HDMI1_ITEM:
                case HWND_SUB_MENU_HDMI2_ITEM:
                case HWND_SUB_MENU_DP1_ITEM:
                case HWND_SUB_MENU_DP2_ITEM:
                {
                    MApp_ZUI_API_SetFocus(HWND_MAIN_MENU_INPUT_SOURCE_ITEM);
                }
                return TRUE;

                case HWND_SUB_MENU_COLSET_INPCOLFMT_ITEM:
                case HWND_SUB_MENU_COLSET_PREMOD_ITEM:
                case HWND_SUB_MENU_COLSET_SUPERRES_ITEM:
                case HWND_SUB_MENU_COLSET_HUE_ITEM:
                case HWND_SUB_MENU_COLSET_SAT_ITEM:
                case HWND_SUB_MENU_ITEM5:
                {
                    MApp_ZUI_API_SetFocus(HWND_MAIN_MENU_COLOR_SETTINGS_ITEM);
                }
                return TRUE;

                case HWND_SUB_MENU_DISSET_ASPRAT_ITEM:
                case HWND_SUB_MENU_DISSET_HP_ITEM:
                case HWND_SUB_MENU_DISSET_VP_ITEM:
                case HWND_SUB_MENU_DISSET_SHP_ITEM:
                case HWND_SUB_MENU_DISSET_PIXCLK_ITEM:
                case HWND_SUB_MENU_DISSET_PHS_ITEM:
                case HWND_SUB_MENU_DISSET_RESPONSETIME_ITEM:
                {
                    MApp_ZUI_API_SetFocus(HWND_MAIN_MENU_DISPLAY_SETTINGS_ITEM);
                }
                return TRUE;

                case HWND_SUB_MENU_OTHERSET_LANGUAGE_ITEM:
                case HWND_SUB_MENU_OTHERSET_TPC_ITEM:
                case HWND_SUB_MENU_OTHERSET_MENUROTA_ITEM:
                case HWND_SUB_MENU_OTHERSET_FACRST_ITEM:
                {
                    MApp_ZUI_API_SetFocus(HWND_MAIN_MENU_OTHER_SETTINGS_ITEM);
                }
                return TRUE;
                default:
                    break;

            }
                return FALSE;
        }
        case EN_EXE_ADJUST_VALUE_INC_KEY:
        {
            switch(hwnd)
            {
                case HWND_SUB_MENU_COLSET_PREMODE_6500K_ITEM:
                case HWND_SUB_MENU_COLSET_PREMODE_MOVIE_ITEM:
                case HWND_SUB_MENU_COLSET_PREMODE_GAME_ITEM:
                case HWND_SUB_MENU_COLSET_INPCOLFMT_5700K_ITEM:
                case HWND_SUB_MENU_COLSET_INPCOLFMT_9300K_ITEM:
                case HWND_SUB_MENU_COLSET_INPCOLFMT_CUSCOL_ITEM:
                {
                    g_u8MenuTempValue = CURRENT_COLOR_TEMP;
                    MApp_ZUI_API_SetFocusByNav(MApp_ZUI_API_GetFocus(), NAV_DOWN);
                    AdjustColorPresetMode(TRUE);
                    SetColorPresetMode();
                    return TRUE;
                }
                case HWND_SUB_MENU_COLSET_SUPERRES_OFF_ITEM:
                case HWND_SUB_MENU_COLSET_SUPERRES_HIGH_ITEM:
                case HWND_SUB_MENU_COLSET_SUPERRES_MIDDLE_ITEM:
                case HWND_SUB_MENU_COLSET_SUPERRES_LOW_ITEM:
                {
                    g_u8MenuTempValue = CURRENT_SUPER_RESOLUTION;
                    MApp_ZUI_API_SetFocusByNav(MApp_ZUI_API_GetFocus(), NAV_DOWN);
                #if ENABLE_SUPER_RESOLUTION
                    AdjustSuperResolutionValue(TRUE);
                    SetSuperResolutionFunction();
                #endif
                    return TRUE;
                }
                case HWND_SUB_MENU_MULTIWIN_PIPHP_VALUE_ITEM:
                {
                #if ENABLE_MULTI_WINDOW_SETTINGS_MENU
                    AdjustPIPHPosition(TRUE);
                #endif
                    MApp_ZUI_API_InvalidateWindow(HWND_SUB_MENU_MULTIWIN_PIPHP_VALUE_ITEM);
                }
                return TRUE;
                case HWND_SUB_MENU_MULTIWIN_PIPVP_VALUE_ITEM:
                {
                #if ENABLE_MULTI_WINDOW_SETTINGS_MENU
                    AdjustPIPVPosition(TRUE);
                #endif
                    MApp_ZUI_API_InvalidateWindow(HWND_SUB_MENU_MULTIWIN_PIPVP_VALUE_ITEM);
                }
                return TRUE;
                case HWND_SUB_MENU_HUE_VALUE_ITEM:
                {
                    if(CURRENT_COLOR_TEMP != CT_USER)
                    {
                    #if ENABLE_MOVIE_GAME_PRESET
                        AdjustVideoHue(TRUE);
                    #else
                        return FALSE;
                    #endif
                    }
                    else
                    {
                        AdjustUserRedColor(TRUE);
                    }
                    MApp_ZUI_API_InvalidateWindow(HWND_SUB_MENU_HUE_VALUE_ITEM);
                }
                return TRUE;
                case HWND_SUB_MENU_SAT_VALUE_ITEM:
                {
                    if(CURRENT_COLOR_TEMP != CT_USER)
                    {
                    #if ENABLE_MOVIE_GAME_PRESET
                        AdjustVideoSaturation(TRUE);
                    #else
                        return FALSE;
                    #endif
                    }
                    else
                    {
                        AdjustUserGreenColor(TRUE);
                    }
                    MApp_ZUI_API_InvalidateWindow(HWND_SUB_MENU_SAT_VALUE_ITEM);
                }
                return TRUE;
                case HWND_SUB_MENU_CUSTOM_B_VALUE_ITEM:
                {
                    AdjustUserBlueColor(TRUE);
                    MApp_ZUI_API_InvalidateWindow(HWND_SUB_MENU_CUSTOM_B_VALUE_ITEM);
                }
                return TRUE;
                case HWND_SUB_MENU_MENUTPR_VALUE_ITEM:
                {
                    AdjustTransparency(TRUE);
                    MApp_ZUI_API_InvalidateWindow(HWND_SUB_MENU_MENUTPR_VALUE_ITEM);
                }
                return TRUE;
				#if ENABLE_VGA_INPUT
                case HWND_SUB_MENU_PHS_VALUE_ITEM:
                {
                    AdjustPhase(TRUE);
                    MApp_ZUI_API_InvalidateWindow(HWND_SUB_MENU_PHS_VALUE_ITEM);
                }
                return TRUE;
                case HWND_SUB_MENU_PIXCLK_VALUE_ITEM:
                {
		  			AdjustClock(TRUE);
                    MApp_ZUI_API_InvalidateWindow(HWND_SUB_MENU_PIXCLK_VALUE_ITEM);
                }
                return TRUE;
				#endif
                case HWND_SUB_MENU_SHP_VALUE_ITEM:
                {
                    AdjustSharpness(TRUE);
                    MApp_ZUI_API_InvalidateWindow(HWND_SUB_MENU_SHP_VALUE_ITEM);
                }
                return TRUE;
				#if ENABLE_VGA_INPUT
                case HWND_SUB_MENU_VP_VALUE_ITEM:
                {
                    AdjustVPosition(TRUE);
                    MApp_ZUI_API_InvalidateWindow(HWND_SUB_MENU_VP_VALUE_ITEM);
                }
                return TRUE;
                case HWND_SUB_MENU_HP_VALUE_ITEM:
                {
                    AdjustHPosition(FALSE);
                    MApp_ZUI_API_InvalidateWindow(HWND_SUB_MENU_HP_VALUE_ITEM);
                }
                return TRUE;
				#endif
                case HWND_SUB_MENU_BRIADJ_VALUE_ITEM:
                {
                    AdjustBrightness(TRUE);
                    MApp_ZUI_API_InvalidateWindow(HWND_SUB_MENU_BRIADJ_VALUE_ITEM);
                }
                    return TRUE;
                case HWND_SUB_MENU_CONADJ_VALUE_ITEM:
                {
                    AdjustContrast(TRUE);
                    MApp_ZUI_API_InvalidateWindow(HWND_SUB_MENU_CONADJ_VALUE_ITEM);
                }
                    return TRUE;
                default:
                    break;
            }
                return FALSE;
        }
        case EN_EXE_GOTO_ENTER:
        {
            switch(hwnd)
            {
                case HWND_SUB_MENU_DISSET_RESPONSETIME_NORMAL_ITEM:
                case HWND_SUB_MENU_DISSET_RESPONSETIME_OD_ITEM:
                {
                    if(hwnd == HWND_SUB_MENU_DISSET_RESPONSETIME_NORMAL_ITEM)
                        g_u8MenuTempValue = _ON_;
                    else
                        g_u8MenuTempValue = _OFF_;
                    SetRTEFunction();
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_DISSET_RESPONSETIME_ITEM_LIST,SW_HIDE);
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_DISSET_ITEM_VALUE_LIST,SW_SHOW);
                    MApp_ZUI_API_SetFocus(HWND_SUB_MENU_DISSET_RESPONSETIME_ITEM);
                    return TRUE;
                }
                case HWND_SUB_MENU_OTHERSET_LAN_ENG_ITEM:
                case HWND_SUB_MENU_OTHERSET_LAN_SIM_ITEM:
                {
                    if(hwnd == HWND_SUB_MENU_OTHERSET_LAN_ENG_ITEM)
                        u8Language = LANGUAGE_ENGLISH;
                    else
                        u8Language = LANGUAGE_CHINESE;
                    #if (!VECTOR_FONT_ENABLE)
                    // only bmp font support dynamic load font sets
                    // closed in vector font for time consuming when generate font data
                    MApp_LoadFontInit(u8Language);
                    #endif
                    MApp_ZUI_ACT_SetOsdLanguage(u8Language);
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_OTHER_SET_ITEM,SW_HIDE);
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_OTHER_SET_ITEM_VALUE_LIST,SW_SHOW);
                    MApp_ZUI_API_SetFocus(HWND_SUB_MENU_OTHERSET_LANGUAGE_ITEM);
                    return TRUE;
                }
                case HWND_SUB_MENU_BRICON_DYN_CON_ON_ITEM:
                case HWND_SUB_MENU_BRICON_DYN_CON_OFF_ITEM:
                {
                    if(hwnd == HWND_SUB_MENU_BRICON_DYN_CON_ON_ITEM)
                        g_u8MenuTempValue = _ON_;
                    else
                        g_u8MenuTempValue = _OFF_;

                    #if ENABLE_DCR
                        SetDCRFunction();
                    #endif
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_BRICON_DYNAMIC_CONTRAST_ITEM,SW_HIDE);
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_PAGE_BRI_ITEM_VALUE_LIST,SW_SHOW);
                    MApp_ZUI_API_SetFocus(HWND_SUB_MENU_DYNAMIC_CON_ITEM);
                    return TRUE;
                }
                case HWND_SUB_MENU_COLSET_PREMODE_6500K_ITEM:
                case HWND_SUB_MENU_COLSET_PREMODE_MOVIE_ITEM:
                case HWND_SUB_MENU_COLSET_PREMODE_GAME_ITEM:
                case HWND_SUB_MENU_COLSET_INPCOLFMT_5700K_ITEM:
                case HWND_SUB_MENU_COLSET_INPCOLFMT_9300K_ITEM:
                case HWND_SUB_MENU_COLSET_INPCOLFMT_CUSCOL_ITEM:
                { //Enter
                    g_u8OriginalSetting = CURRENT_COLOR_TEMP;
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_COL_SET,SW_HIDE);
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_COL_SET_PAGE,SW_HIDE);
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_COL_SET_PAGE,SW_SHOW);
                    MApp_ZUI_API_SetFocus(HWND_SUB_MENU_COLSET_PREMOD_ITEM);
                    return TRUE;
                }
                case HWND_SUB_MENU_COLSET_SUPERRES_OFF_ITEM:
                case HWND_SUB_MENU_COLSET_SUPERRES_HIGH_ITEM:
                case HWND_SUB_MENU_COLSET_SUPERRES_MIDDLE_ITEM:
                case HWND_SUB_MENU_COLSET_SUPERRES_LOW_ITEM:
                { //Enter
                    g_u8OriginalSetting = CURRENT_SUPER_RESOLUTION;
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_COLSET_SUPERRES_ITEM_LIST,SW_HIDE);
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_COL_VALUE_ITEM_LIST,SW_SHOW);
                    MApp_ZUI_API_SetFocus(HWND_SUB_MENU_COLSET_SUPERRES_ITEM);
                    //MApp_ZUI_API_InvalidateWindow(HWND_SUB_MENU_COL_VALUE_ITEM_LIST);
                    return TRUE;
                }
                case HWND_SUB_MENU_COLSET_INPCOLFMT_RGB_ITEM:
                case HWND_SUB_MENU_COLSET_INPCOLFMT_YPBPR_ITEM:
                case HWND_SUB_MENU_COLSET_INPCOLFMT_AUTO_ITEM:
                {
                    if(hwnd == HWND_SUB_MENU_COLSET_INPCOLFMT_RGB_ITEM)
                        g_u8MenuTempValue = INPUT_COLOR_RGB;
                    else if(hwnd == HWND_SUB_MENU_COLSET_INPCOLFMT_YPBPR_ITEM)
                        g_u8MenuTempValue=INPUT_COLOR_YPBPR;
                    else
                        g_u8MenuTempValue=INPUT_COLOR_FORMAT_AUTO;
                    SetInputColorFormatValue();
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_COL_SET,SW_HIDE);
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_COL_VALUE_ITEM_LIST,SW_SHOW);
                    MApp_ZUI_API_SetFocus(HWND_SUB_MENU_COLSET_INPCOLFMT_ITEM);
                    return TRUE;
                }
                case HWND_SUB_MENU_DISSET_ASPRATIO_FULL_ITEM:
                case HWND_SUB_MENU_DISSET_ASPRATIO_AUTO_ITEM:
                case HWND_SUB_MENU_DISSET_ASPRATIO_16BY9_ITEM:
                case HWND_SUB_MENU_DISSET_ASPRATIO_4BY3_ITEM:
                case HWND_SUB_MENU_DISSET_ASPRATIO_5BY4_ITEM:
                case HWND_SUB_MENU_DISSET_ASPRATIO_ONEBYONE_ITEM:
                {
                    if(hwnd == HWND_SUB_MENU_DISSET_ASPRATIO_FULL_ITEM)
                        g_u8MenuTempValue = eASPECT_FULL;
                    else if(hwnd == HWND_SUB_MENU_DISSET_ASPRATIO_AUTO_ITEM)
                        g_u8MenuTempValue = eASPECT_NATIVE;
                    else if(hwnd == HWND_SUB_MENU_DISSET_ASPRATIO_5BY4_ITEM)
                        g_u8MenuTempValue = eASPECT_5_4;
                    else if(hwnd == HWND_SUB_MENU_DISSET_ASPRATIO_4BY3_ITEM)
                        g_u8MenuTempValue = eASPECT_4_3;
                    else if(hwnd == HWND_SUB_MENU_DISSET_ASPRATIO_16BY9_ITEM)
                        g_u8MenuTempValue = eASPECT_16_9;
                    else
                        g_u8MenuTempValue = eASPECT_1_1;

                        //MApp_ZUI_ACT_ShutdownOSD();
                        #if ENABLE_WIDE_MODE
                        Menu_SetWideMode();
                        #endif
                        MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_DISSET_ASPRATIO_ITEM_LIST,SW_HIDE);
                        MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_DISSET_ITEM_VALUE_LIST,SW_SHOW);
                        MApp_ZUI_API_SetFocus(HWND_SUB_MENU_DISSET_ASPRAT_ITEM);
                }
                    return TRUE;
                case HWND_SUB_MENU_MULTIWIN_WINMODES_OFF_ITEM:
                case HWND_SUB_MENU_MULTIWIN_WINMODES_PIPMODE_ITEM:
                case HWND_SUB_MENU_MULTIWIN_WINMODES_PBPMODE1_ITEM:
                case HWND_SUB_MENU_MULTIWIN_WINMODES_PBPMODE2_ITEM:
                case HWND_SUB_MENU_MULTIWIN_WINMODES_TRIPLEMODE_ITEM:
                case HWND_SUB_MENU_MULTIWIN_WINMODES_QUADMODE_ITEM:
                {
                    if(hwnd == HWND_SUB_MENU_MULTIWIN_WINMODES_OFF_ITEM)
                        g_u8MenuTempValue = eMULTIWIN_OFF;
                    else if(hwnd == HWND_SUB_MENU_MULTIWIN_WINMODES_PIPMODE_ITEM)
                        g_u8MenuTempValue = eMULTIWIN_2_PIP;
                    else if(hwnd == HWND_SUB_MENU_MULTIWIN_WINMODES_PBPMODE1_ITEM)
                        g_u8MenuTempValue = eMULTIWIN_2_POP1;
                    else if(hwnd == HWND_SUB_MENU_MULTIWIN_WINMODES_PBPMODE2_ITEM)
                        g_u8MenuTempValue = eMULTIWIN_2_POP2;
                    else if(hwnd == HWND_SUB_MENU_MULTIWIN_WINMODES_TRIPLEMODE_ITEM)
                        g_u8MenuTempValue = eMULTIWIN_3;
                    else
                        g_u8MenuTempValue = eMULTIWIN_4;

                    if(g_u8MenuTempValue <=eMULTIWIN_2_POP2 && USER_PREF_WIN_SEL >= DISPLAY_WIN3)
                        USER_PREF_WIN_SEL = DISPLAY_WIN2;
                    else if(g_u8MenuTempValue <=eMULTIWIN_3 && USER_PREF_WIN_SEL == DISPLAY_WIN4)
                        USER_PREF_WIN_SEL = DISPLAY_WIN3;

                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_MULTIWIN_WINMODES_ITEM_LIST,SW_HIDE);
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_MULTIWIN_ITEM_VALUE_LIST,SW_SHOW);

                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_MULTIWIN_ITEM_LIST,SW_HIDE);
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_MULTIWIN_ITEM_LIST,SW_SHOW);
                    MApp_ZUI_API_InvalidateAllSuccessors(HWND_MULTI_WIN_INFO);
                    MApp_ZUI_API_SetFocus(HWND_SUB_MENU_MULTIWIN_WINMODES_ITEM);
                    #if ENABLE_MULTI_WINDOW_SETTINGS_MENU
                    if(Menu_SetMultiWindowModes())
                    #endif
                        MApp_ZUI_ACT_ShutdownOSD();

                }
                return TRUE;
                case HWND_SUB_MENU_MULTIWIN_WINSEL_WIN1_ITEM:
                case HWND_SUB_MENU_MULTIWIN_WINSEL_WIN2_ITEM:
                case HWND_SUB_MENU_MULTIWIN_WINSEL_WIN3_ITEM:
                case HWND_SUB_MENU_MULTIWIN_WINSEL_WIN4_ITEM:
                {
                    if(hwnd == HWND_SUB_MENU_MULTIWIN_WINSEL_WIN1_ITEM)
                        g_u8MenuTempValue = DISPLAY_WIN1;
                    else if(hwnd == HWND_SUB_MENU_MULTIWIN_WINSEL_WIN2_ITEM)
                        g_u8MenuTempValue = DISPLAY_WIN2;
                    else if(hwnd == HWND_SUB_MENU_MULTIWIN_WINSEL_WIN3_ITEM)
                        g_u8MenuTempValue = DISPLAY_WIN3;
                    else if(hwnd == HWND_SUB_MENU_MULTIWIN_WINSEL_WIN4_ITEM)
                        g_u8MenuTempValue = DISPLAY_WIN4;

                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_MULTIWIN_WINSEL_ITEM_LIST,SW_HIDE);
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_MULTIWIN_ITEM_VALUE_LIST,SW_SHOW);
                    MApp_ZUI_API_SetFocus(HWND_SUB_MENU_MULTIWIN_WINSEL_ITEM);
                    #if ENABLE_MULTI_WINDOW_SETTINGS_MENU
                    if(Menu_SetMultiWindowSelect())
                    #endif
                        MApp_ZUI_ACT_ShutdownOSD();
                }
                return TRUE;
                case HWND_SUB_MENU_MULTIWIN_PIPSIZE_SMALL_ITEM:
                case HWND_SUB_MENU_MULTIWIN_PIPSIZE_MEDIUM_ITEM:
                case HWND_SUB_MENU_MULTIWIN_PIPSIZE_LARGE_ITEM:
                case HWND_SUB_MENU_MULTIWIN_PIPSIZE_HUGE_ITEM:
                {
                    if(hwnd == HWND_SUB_MENU_MULTIWIN_PIPSIZE_SMALL_ITEM)
                        g_u8MenuTempValue = ePIPSIZE_SMALL;
                    else if(hwnd == HWND_SUB_MENU_MULTIWIN_PIPSIZE_MEDIUM_ITEM)
                        g_u8MenuTempValue = ePIPSIZE_MIDDLE;
                    else if(hwnd == HWND_SUB_MENU_MULTIWIN_PIPSIZE_LARGE_ITEM)
                        g_u8MenuTempValue = ePIPSIZE_LARGE;
                    else
                        g_u8MenuTempValue = ePIPSIZE_HUGE;
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_MULTIWIN_PIPSIZE_ITEM_LIST,SW_HIDE);
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_MULTIWIN_ITEM_VALUE_LIST,SW_SHOW);
                    MApp_ZUI_API_SetFocus(HWND_SUB_MENU_MULTIWIN_PIPSIZE_ITEM);
                    #if ENABLE_MULTI_WINDOW_SETTINGS_MENU
                    if(Menu_SetPIPSize())
                    #endif
                        MApp_ZUI_ACT_ShutdownOSD();

                }
                    return TRUE;

                default:
                    break;
           }
            return FALSE;
        }

        case EN_EXE_ADJUST_VALUE_DEC_KEY:
        {
            switch(hwnd)
            {
                case HWND_SUB_MENU_COLSET_PREMODE_6500K_ITEM:
                case HWND_SUB_MENU_COLSET_PREMODE_MOVIE_ITEM:
                case HWND_SUB_MENU_COLSET_PREMODE_GAME_ITEM:
                case HWND_SUB_MENU_COLSET_INPCOLFMT_5700K_ITEM:
                case HWND_SUB_MENU_COLSET_INPCOLFMT_9300K_ITEM:
                case HWND_SUB_MENU_COLSET_INPCOLFMT_CUSCOL_ITEM:
                {
                    g_u8MenuTempValue = CURRENT_COLOR_TEMP;
                    MApp_ZUI_API_SetFocusByNav(MApp_ZUI_API_GetFocus(), NAV_UP);
                    AdjustColorPresetMode(FALSE);
                    SetColorPresetMode();
                    return TRUE;
                }
                case HWND_SUB_MENU_COLSET_SUPERRES_OFF_ITEM:
                case HWND_SUB_MENU_COLSET_SUPERRES_HIGH_ITEM:
                case HWND_SUB_MENU_COLSET_SUPERRES_MIDDLE_ITEM:
                case HWND_SUB_MENU_COLSET_SUPERRES_LOW_ITEM:
                {
                    g_u8MenuTempValue = CURRENT_SUPER_RESOLUTION;
                    MApp_ZUI_API_SetFocusByNav(MApp_ZUI_API_GetFocus(), NAV_UP);
                #if ENABLE_SUPER_RESOLUTION
                    AdjustSuperResolutionValue(FALSE);
                    SetSuperResolutionFunction();
                #endif
                    return TRUE;
                }
                case HWND_SUB_MENU_MULTIWIN_PIPHP_VALUE_ITEM:
                {
                #if ENABLE_MULTI_WINDOW_SETTINGS_MENU
                    AdjustPIPHPosition(FALSE);
                #endif
                    MApp_ZUI_API_InvalidateWindow(HWND_SUB_MENU_MULTIWIN_PIPHP_VALUE_ITEM);
                }
                return TRUE;
                case HWND_SUB_MENU_MULTIWIN_PIPVP_VALUE_ITEM:
                {
                #if ENABLE_MULTI_WINDOW_SETTINGS_MENU
                    AdjustPIPVPosition(FALSE);
                #endif
                    MApp_ZUI_API_InvalidateWindow(HWND_SUB_MENU_MULTIWIN_PIPVP_VALUE_ITEM);
                }
                return TRUE;
                case HWND_SUB_MENU_HUE_VALUE_ITEM:
                {
                    if(CURRENT_COLOR_TEMP != CT_USER)
                    {
                    #if ENABLE_MOVIE_GAME_PRESET
                        AdjustVideoHue(FALSE);
                    #else
                        return FALSE;
                    #endif
                    }
                    else
                    {
                        AdjustUserRedColor(FALSE);
                    }
                    MApp_ZUI_API_InvalidateWindow(HWND_SUB_MENU_HUE_VALUE_ITEM);
                }
                return TRUE;
                case HWND_SUB_MENU_SAT_VALUE_ITEM:
                {
                    if(CURRENT_COLOR_TEMP != CT_USER)
                    {
                    #if ENABLE_MOVIE_GAME_PRESET
                        AdjustVideoSaturation(FALSE);
                    #else
                        return FALSE;
                    #endif
                    }
                    else
                    {
                        AdjustUserGreenColor(FALSE);
                    }
                    MApp_ZUI_API_InvalidateWindow(HWND_SUB_MENU_SAT_VALUE_ITEM);
                }
                return TRUE;
                case HWND_SUB_MENU_CUSTOM_B_VALUE_ITEM:
                {
                    AdjustUserBlueColor(FALSE);
                    //CURRENT_BLUE_COLOR = DecIncValue(FALSE,CURRENT_BLUE_COLOR,0,100, 1);
                    MApp_ZUI_API_InvalidateWindow(HWND_SUB_MENU_CUSTOM_B_VALUE_ITEM);
                }
                return TRUE;
                case HWND_SUB_MENU_MENUTPR_VALUE_ITEM:
                {
                    AdjustTransparency(FALSE);
                    MApp_ZUI_API_InvalidateWindow(HWND_SUB_MENU_MENUTPR_VALUE_ITEM);
                }
                return TRUE;
				#if ENABLE_VGA_INPUT
                case HWND_SUB_MENU_PHS_VALUE_ITEM:
                {
                    AdjustPhase(FALSE);
                    MApp_ZUI_API_InvalidateWindow(HWND_SUB_MENU_PHS_VALUE_ITEM);
                }
                return TRUE;
                case HWND_SUB_MENU_PIXCLK_VALUE_ITEM:
                {
                    AdjustClock(FALSE);
                    MApp_ZUI_API_InvalidateWindow(HWND_SUB_MENU_PIXCLK_VALUE_ITEM);
                }
                return TRUE;
				#endif
                case HWND_SUB_MENU_SHP_VALUE_ITEM:
                {
                    AdjustSharpness(FALSE);
                    MApp_ZUI_API_InvalidateWindow(HWND_SUB_MENU_SHP_VALUE_ITEM);
                }
                return TRUE;
				#if ENABLE_VGA_INPUT
                case HWND_SUB_MENU_VP_VALUE_ITEM:
                {
                    AdjustVPosition(FALSE);
                    MApp_ZUI_API_InvalidateWindow(HWND_SUB_MENU_VP_VALUE_ITEM);
                }
                return TRUE;
                case HWND_SUB_MENU_HP_VALUE_ITEM:
                {
                    AdjustHPosition(TRUE);
                    MApp_ZUI_API_InvalidateWindow(HWND_SUB_MENU_HP_VALUE_ITEM);
                }
                return TRUE;
				#endif
                case HWND_SUB_MENU_BRIADJ_VALUE_ITEM:
                {
                    AdjustBrightness(FALSE);
                    MApp_ZUI_API_InvalidateWindow(HWND_SUB_MENU_BRIADJ_VALUE_ITEM);
                }
                    return TRUE;
                case HWND_SUB_MENU_CONADJ_VALUE_ITEM:
                {
                    AdjustContrast(FALSE);
                    MApp_ZUI_API_InvalidateWindow(HWND_SUB_MENU_CONADJ_VALUE_ITEM);
                }
                    return TRUE;
                default:
                    break;
            }
                return FALSE;
        }
        case EN_EXE_GOTO_ADJUST_VALUE:
        {
            bPreFocusFlag = 1;
            MApp_ZUI_API_StoreFocusCheckpoint();
            switch(hwnd)
            {

                case HWND_SUB_MENU_DISSET_RESPONSETIME_ITEM:
                {
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_DISSET_ITEM_VALUE_LIST,SW_HIDE);
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_DISSET_RESPONSETIME_ITEM_LIST,SW_SHOW);
                    if(!GetResponseTimeValue())
                        MApp_ZUI_API_SetFocus(HWND_SUB_MENU_DISSET_RESPONSETIME_NORMAL_ITEM);
                    else
                        MApp_ZUI_API_SetFocus(HWND_SUB_MENU_DISSET_RESPONSETIME_OD_ITEM);
                }
                return TRUE;
                case HWND_SUB_MENU_MULTIWIN_WINSEL_ITEM:
                {
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_MULTIWIN_ITEM_VALUE_LIST,SW_HIDE);
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_MULTIWIN_WINSEL_ITEM_LIST,SW_SHOW);
                    if(USER_PREF_WIN_SEL == DISPLAY_WIN1)
                        MApp_ZUI_API_SetFocus(HWND_SUB_MENU_MULTIWIN_WINSEL_WIN1_ITEM);
                    else if(USER_PREF_WIN_SEL == DISPLAY_WIN2)
                        MApp_ZUI_API_SetFocus(HWND_SUB_MENU_MULTIWIN_WINSEL_WIN2_ITEM);
                    else if(USER_PREF_WIN_SEL == DISPLAY_WIN3)
                        MApp_ZUI_API_SetFocus(HWND_SUB_MENU_MULTIWIN_WINSEL_WIN3_ITEM);
                    else
                        MApp_ZUI_API_SetFocus(HWND_SUB_MENU_MULTIWIN_WINSEL_WIN4_ITEM);
                }
                return TRUE;
                case HWND_SUB_MENU_MULTIWIN_PIPSIZE_ITEM:
                {
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_MULTIWIN_ITEM_VALUE_LIST,SW_HIDE);
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_MULTIWIN_PIPSIZE_ITEM_LIST,SW_SHOW);
                    if(USER_PREF_MULTI_WIN_SIZE(DISPLAY_WIN2) == ePIPSIZE_SMALL)
                        MApp_ZUI_API_SetFocus(HWND_SUB_MENU_MULTIWIN_PIPSIZE_SMALL_ITEM);
                    else if(USER_PREF_MULTI_WIN_SIZE(DISPLAY_WIN2) == ePIPSIZE_MIDDLE)
                        MApp_ZUI_API_SetFocus(HWND_SUB_MENU_MULTIWIN_PIPSIZE_MEDIUM_ITEM);
                    else if(USER_PREF_MULTI_WIN_SIZE(DISPLAY_WIN2) == ePIPSIZE_LARGE)
                        MApp_ZUI_API_SetFocus(HWND_SUB_MENU_MULTIWIN_PIPSIZE_LARGE_ITEM);
                    else
                       MApp_ZUI_API_SetFocus(HWND_SUB_MENU_MULTIWIN_PIPSIZE_HUGE_ITEM);
                }
                return TRUE;
                case HWND_SUB_MENU_MULTIWIN_PIPHP_ITEM:
                {
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_MULTIWIN_PIPHP_ITEM_VALUE,SW_HIDE);
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_MULTIWIN_PIPHP_VALUE_ITEM,SW_SHOW);
                    MApp_ZUI_API_SetFocus(HWND_SUB_MENU_MULTIWIN_PIPHP_VALUE_ITEM);
                }
                return TRUE;
                case HWND_SUB_MENU_MULTIWIN_PIPVP_ITEM:
                {
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_MULTIWIN_PIPVP_ITEM_VALUE,SW_HIDE);
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_MULTIWIN_PIPVP_VALUE_ITEM,SW_SHOW);
                    MApp_ZUI_API_SetFocus(HWND_SUB_MENU_MULTIWIN_PIPVP_VALUE_ITEM);
                }
                return TRUE;

                case HWND_SUB_MENU_MULTIWIN_WINMODES_ITEM:
                {
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_MULTIWIN_ITEM_VALUE_LIST,SW_HIDE);
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_MULTIWIN_WINMODES_ITEM_LIST,SW_SHOW);
                    if(USER_PREF_MULTI_WIN_MODE == eMULTIWIN_OFF)
                        MApp_ZUI_API_SetFocus(HWND_SUB_MENU_MULTIWIN_WINMODES_OFF_ITEM);
                    else if(USER_PREF_MULTI_WIN_MODE == eMULTIWIN_2_PIP)
                        MApp_ZUI_API_SetFocus(HWND_SUB_MENU_MULTIWIN_WINMODES_PIPMODE_ITEM);
                    else if(USER_PREF_MULTI_WIN_MODE == eMULTIWIN_2_POP1)
                        MApp_ZUI_API_SetFocus(HWND_SUB_MENU_MULTIWIN_WINMODES_PBPMODE1_ITEM);
                    else if(USER_PREF_MULTI_WIN_MODE == eMULTIWIN_2_POP2)
                        MApp_ZUI_API_SetFocus(HWND_SUB_MENU_MULTIWIN_WINMODES_PBPMODE2_ITEM);
                    else if(USER_PREF_MULTI_WIN_MODE == eMULTIWIN_3)
                        MApp_ZUI_API_SetFocus(HWND_SUB_MENU_MULTIWIN_WINMODES_TRIPLEMODE_ITEM);
                    else
                        MApp_ZUI_API_SetFocus(HWND_SUB_MENU_MULTIWIN_WINMODES_QUADMODE_ITEM);
                }
                return TRUE;
                case HWND_SUB_MENU_AUTO_SRC_ITEM:
                case HWND_SUB_MENU_VGA_ITEM:
                case HWND_SUB_MENU_DVI_ITEM:
                case HWND_SUB_MENU_HDMI1_ITEM:
                case HWND_SUB_MENU_HDMI2_ITEM:
                case HWND_SUB_MENU_DP1_ITEM:
                case HWND_SUB_MENU_DP2_ITEM:
                {
                    if(hwnd == HWND_SUB_MENU_AUTO_SRC_ITEM)
                    {
                        g_u8MenuTempValue = INPUT_PRIORITY_AUTO;
                    }
		#if ENABLE_VGA_INPUT			
                    else if(hwnd == HWND_SUB_MENU_VGA_ITEM)
                    {
                        g_u8MenuTempValue = INPUT_PRIORITY_VGA;
                        //USER_PREF_INPUT_PRIORITY = INPUT_PRIORITY_VGA;
                    }
		#endif			
        #if ENABLE_DVI_INPUT
                    else if(hwnd == HWND_SUB_MENU_DVI_ITEM)
                    {
                        g_u8MenuTempValue = INPUT_PRIORITY_DVI;
                    }
        #endif
        #if ENABLE_HDMI_INPUT
                    else if(hwnd == HWND_SUB_MENU_HDMI1_ITEM)
                    {
                        g_u8MenuTempValue = INPUT_PRIORITY_HDMI;
                    }
        #endif
        #if ENABLE_HDMI2ND_INPUT
                    else if(hwnd == HWND_SUB_MENU_HDMI2_ITEM)
                    {
                        g_u8MenuTempValue = INPUT_PRIORITY_HDMI2ND;
                    }
        #endif
        #if ENABLE_DP_INPUT
                    else if(hwnd == HWND_SUB_MENU_DP1_ITEM)
                    {
                        g_u8MenuTempValue = INPUT_PRIORITY_DP;
                    }
        #endif
        #if ENABLE_DP2ND_INPUT
                    else //if(hwnd == HWND_SUB_MENU_DP2_ITEM)
                    {
                        g_u8MenuTempValue = INPUT_PRIORITY_DP2ND;
                    }
        #endif
                        MApp_ZUI_ACT_ShutdownOSD();
                        if(SetInputSelect())
                            u32TimerDelay=MsOS_GetSystemTime();
                        g_bSourceChangeFlag = 1;
                        return TRUE;
                }
                case HWND_SUB_MENU_OTHERSET_FACRST_ITEM:
                {
                    //do factory mode function here
                    ResetAllSetting();
                    MApp_ZUI_ACT_ShutdownOSD();
                }
                return TRUE;
                case HWND_SUB_MENU_DYNAMIC_CON_ITEM:
                {
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_PAGE_BRI_ITEM_VALUE_LIST,SW_HIDE);
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_BRICON_DYN_CON_ITEM_LIST,SW_SHOW);
                #if ENABLE_DCR
                    if(GetDCREnableRealValue())
                        MApp_ZUI_API_SetFocus(HWND_SUB_MENU_BRICON_DYN_CON_OFF_ITEM);
                    else
                        MApp_ZUI_API_SetFocus(HWND_SUB_MENU_BRICON_DYN_CON_ON_ITEM);
                #else
                        MApp_ZUI_API_SetFocus(HWND_SUB_MENU_BRICON_DYN_CON_OFF_ITEM);
                #endif
                }
                return TRUE;
                case HWND_SUB_MENU_OTHERSET_MENUROTA_ITEM:
                {
                    // do Menu Rotation function here
                    if (MApp_ZUI_API_GetCurrentRotateAngle() == OSD_ROTATE_0)
                        MApp_ZUI_API_DoRotate(OSD_ROTATE_90, TRUE);
                    else
                        MApp_ZUI_API_DoRotate(OSD_ROTATE_0, TRUE);
                    MApp_ZUI_API_InvalidateAllSuccessors(HWND_MAINFRAME);
                }
                return TRUE;
                case HWND_SUB_MENU_OTHERSET_TPC_ITEM:
                {
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_OTHERSET_MENUTPC_VALUE_ITEM,SW_HIDE);
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_MENUTPR_VALUE_ITEM,SW_SHOW);
                    MApp_ZUI_API_SetFocus(HWND_SUB_MENU_MENUTPR_VALUE_ITEM);
                }
                return TRUE;
                case HWND_SUB_MENU_OTHERSET_LANGUAGE_ITEM:
                {
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_OTHER_SET_ITEM_VALUE_LIST,SW_HIDE);
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_OTHER_SET_LAN_ITEM_LIST,SW_SHOW);
                    if(u8Language == LANGUAGE_ENGLISH )
                        MApp_ZUI_API_SetFocus(HWND_SUB_MENU_OTHERSET_LAN_ENG_ITEM);
                    else
                        MApp_ZUI_API_SetFocus(HWND_SUB_MENU_OTHERSET_LAN_SIM_ITEM);
                }
                return TRUE;
                case HWND_SUB_MENU_DISSET_HP_ITEM:
                {
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_DISSET_HP_ITEM_VALUE,SW_HIDE);
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_HP_VALUE_ITEM,SW_SHOW);
                    MApp_ZUI_API_SetFocus(HWND_SUB_MENU_HP_VALUE_ITEM);
                }
                    return TRUE;

                case HWND_SUB_MENU_DISSET_VP_ITEM:
                {
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_DISSET_VP_ITEM_VALUE,SW_HIDE);
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_VP_VALUE_ITEM,SW_SHOW);
                    MApp_ZUI_API_SetFocus(HWND_SUB_MENU_VP_VALUE_ITEM);
                }
                    return TRUE;
                case HWND_SUB_MENU_DISSET_SHP_ITEM:
                {
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_DISSET_SHP_ITEM_VALUE,SW_HIDE);
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_SHP_VALUE_ITEM,SW_SHOW);
                    MApp_ZUI_API_SetFocus(HWND_SUB_MENU_SHP_VALUE_ITEM);
                }
                    return TRUE;
                case HWND_SUB_MENU_DISSET_PIXCLK_ITEM:
                {
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_DISSET_PIXCLK_ITEM_VALUE,SW_HIDE);
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_PIXCLK_VALUE_ITEM,SW_SHOW);
                    MApp_ZUI_API_SetFocus(HWND_SUB_MENU_PIXCLK_VALUE_ITEM);
                }
                    return TRUE;
                case HWND_SUB_MENU_DISSET_PHS_ITEM:
                {
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_DISSET_PHS_ITEM_VALUE,SW_HIDE);
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_PHS_VALUE_ITEM,SW_SHOW);
                    MApp_ZUI_API_SetFocus(HWND_SUB_MENU_PHS_VALUE_ITEM);
                }
                    return TRUE;

                case HWND_SUB_MENU_DISSET_ASPRAT_ITEM:
                {
                     MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_DISSET_ITEM_VALUE_LIST,SW_HIDE);
                     MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_DISSET_ASPRATIO_ITEM_LIST,SW_SHOW);
                     if(USER_PREF_WIDE_MODE(SrcInputPortM) == eASPECT_FULL)
                        MApp_ZUI_API_SetFocus(HWND_SUB_MENU_DISSET_ASPRATIO_FULL_ITEM);
                     else if(USER_PREF_WIDE_MODE(SrcInputPortM) == eASPECT_NATIVE)
                        MApp_ZUI_API_SetFocus(HWND_SUB_MENU_DISSET_ASPRATIO_AUTO_ITEM);
                    else if(USER_PREF_WIDE_MODE(SrcInputPortM) == eASPECT_5_4)
                        MApp_ZUI_API_SetFocus(HWND_SUB_MENU_DISSET_ASPRATIO_5BY4_ITEM);
                    else if(USER_PREF_WIDE_MODE(SrcInputPortM) == eASPECT_4_3)
                        MApp_ZUI_API_SetFocus(HWND_SUB_MENU_DISSET_ASPRATIO_4BY3_ITEM);
                    else if(USER_PREF_WIDE_MODE(SrcInputPortM) == eASPECT_16_9)
                        MApp_ZUI_API_SetFocus(HWND_SUB_MENU_DISSET_ASPRATIO_16BY9_ITEM);
                    else
                        MApp_ZUI_API_SetFocus(HWND_SUB_MENU_DISSET_ASPRATIO_ONEBYONE_ITEM);
                }
                return TRUE;

                case HWND_SUB_MENU_COLSET_INPCOLFMT_ITEM:
                {
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_COL_VALUE_ITEM_LIST,SW_HIDE);
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_COLSET_INPCOLFMT_ITEM_LIST,SW_SHOW);
                    g_u8OriginalSetting = CURRENT_INPUT_COLOR_FORMAT;
                    if(CURRENT_INPUT_COLOR_FORMAT == INPUT_COLOR_RGB)
                        MApp_ZUI_API_SetFocus(HWND_SUB_MENU_COLSET_INPCOLFMT_RGB_ITEM);
                    else if(CURRENT_INPUT_COLOR_FORMAT == INPUT_COLOR_YPBPR)
                        MApp_ZUI_API_SetFocus(HWND_SUB_MENU_COLSET_INPCOLFMT_YPBPR_ITEM);
                    else
                        MApp_ZUI_API_SetFocus(HWND_SUB_MENU_COLSET_INPCOLFMT_AUTO_ITEM);
                }
                return TRUE;
                case HWND_SUB_MENU_COLSET_PREMOD_ITEM:
                {
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_COLSET_INPCOLFMT_ITEM_LIST,SW_HIDE);
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_COLSET_PREMODE_ITEM_LIST,SW_SHOW);
                    g_u8OriginalSetting = CURRENT_COLOR_TEMP;
                    if(CURRENT_COLOR_TEMP == CT_STANDARD)
                        MApp_ZUI_API_SetFocus(HWND_SUB_MENU_COLSET_PREMODE_6500K_ITEM);
                    else if(CURRENT_COLOR_TEMP == CT_MOVIE)
                        MApp_ZUI_API_SetFocus(HWND_SUB_MENU_COLSET_PREMODE_MOVIE_ITEM);
                    else if(CURRENT_COLOR_TEMP == CT_GAME)
                        MApp_ZUI_API_SetFocus(HWND_SUB_MENU_COLSET_PREMODE_GAME_ITEM);
                    else if(CURRENT_COLOR_TEMP == CT_WARM)
                        MApp_ZUI_API_SetFocus(HWND_SUB_MENU_COLSET_INPCOLFMT_5700K_ITEM);
                    else if(CURRENT_COLOR_TEMP == CT_COOL)
                        MApp_ZUI_API_SetFocus(HWND_SUB_MENU_COLSET_INPCOLFMT_9300K_ITEM);
                    else
                        MApp_ZUI_API_SetFocus(HWND_SUB_MENU_COLSET_INPCOLFMT_CUSCOL_ITEM);
                }
                return TRUE;
                case HWND_SUB_MENU_COLSET_SUPERRES_ITEM:
                {
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_COL_VALUE_ITEM_LIST,SW_HIDE);
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_COLSET_SUPERRES_ITEM_LIST,SW_SHOW);
                    g_u8OriginalSetting = CURRENT_SUPER_RESOLUTION;
                    if(CURRENT_SUPER_RESOLUTION==SUPER_RESOLUTION_OFF)
                        MApp_ZUI_API_SetFocus(HWND_SUB_MENU_COLSET_SUPERRES_OFF_ITEM);
                    else if(CURRENT_SUPER_RESOLUTION==SUPER_RESOLUTION_HIGH)
                        MApp_ZUI_API_SetFocus(HWND_SUB_MENU_COLSET_SUPERRES_HIGH_ITEM);
                    else if(CURRENT_SUPER_RESOLUTION==SUPER_RESOLUTION_MIDDLE)
                        MApp_ZUI_API_SetFocus(HWND_SUB_MENU_COLSET_SUPERRES_MIDDLE_ITEM);
                    else//(CURRENT_SUPER_RESOLUTION==SUPER_RESOLUTION_LOW)
                        MApp_ZUI_API_SetFocus(HWND_SUB_MENU_COLSET_SUPERRES_LOW_ITEM);
                }
                return TRUE;
                case HWND_SUB_MENU_COLSET_HUE_ITEM:
                {
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_HUE_VALUE_ITEM,SW_SHOW);
                    MApp_ZUI_API_SetFocus(HWND_SUB_MENU_HUE_VALUE_ITEM);
                }
                return TRUE;
                case HWND_SUB_MENU_COLSET_SAT_ITEM:
                {
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_SAT_VALUE_ITEM,SW_SHOW);
                    MApp_ZUI_API_SetFocus(HWND_SUB_MENU_SAT_VALUE_ITEM);
                }
                return TRUE;
                case HWND_SUB_MENU_ITEM5: //for Custome B item
                {
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_CUSTOM_B_VALUE_ITEM,SW_SHOW);
                    MApp_ZUI_API_SetFocus(HWND_SUB_MENU_CUSTOM_B_VALUE_ITEM);
                }
                return TRUE;
                case HWND_SUB_MENU_BRI_ITEM:
                {
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_BRIADJ_VALUE_ITEM, SW_SHOW);
                    MApp_ZUI_API_SetFocus(HWND_SUB_MENU_BRIADJ_VALUE_ITEM);
                }
                    return TRUE;

                case HWND_SUB_MENU_CON_ITEM:
                {
                    MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_CONADJ_VALUE_ITEM, SW_SHOW);
                    MApp_ZUI_API_SetFocus(HWND_SUB_MENU_CONADJ_VALUE_ITEM);
                }
                    return TRUE;

                default:
                    break;
            }
                return FALSE;
        }
        case EN_EXE_GOTO_NEXT_PAGE:
        {
            bPreFocusFlag = 1;
            MApp_ZUI_API_StoreFocusCheckpoint();
            switch(hwnd)
            {
                case HWND_MAIN_MENU_BRI_ITEM:
                {
                    MApp_ZUI_API_SetFocus(HWND_SUB_MENU_BRI_ITEM);
                    return TRUE;
                }
                case HWND_MAIN_MENU_AUTO_ADJUST_ITEM:
                {
                    //do auto adjust function here
                    g_bAutoAdjustFlag = TRUE;
                    b_ReturnFocusFlag = TRUE;
                    MApp_ZUI_ACT_ShutdownOSD();
                    MApp_ZUI_ACT_StartupOSD(E_OSD_AUTO_ADJUST);
                    u32TimerDelay=MsOS_GetSystemTime();
                    return TRUE;
                }
                case HWND_MAIN_MENU_INPUT_SOURCE_ITEM:
                {
                    MApp_ZUI_API_SetFocus(HWND_SUB_MENU_AUTO_SRC_ITEM);
                    return TRUE;
                }
                case HWND_MAIN_MENU_COLOR_SETTINGS_ITEM:
                {
                    MApp_ZUI_API_SetFocus(HWND_SUB_MENU_COLSET_INPCOLFMT_ITEM);
                    return TRUE;
                }
                case HWND_MAIN_MENU_DISPLAY_SETTINGS_ITEM:
                {
                    #if ENABLE_WIDE_MODE
                    MApp_ZUI_API_SetFocus(HWND_SUB_MENU_DISSET_ASPRAT_ITEM);
                    #else
                    if(CURRENT_INPUT_IS_VGA())
                        MApp_ZUI_API_SetFocus(HWND_SUB_MENU_DISSET_HP_ITEM);
                    else
                        MApp_ZUI_API_SetFocus(HWND_SUB_MENU_DISSET_SHP_ITEM);
                    #endif
                    return TRUE;
                }
                case HWND_MAIN_MENU_MULTI_WIN_SETTINGS_ITEM:
                {
                    MApp_ZUI_API_SetFocus(HWND_SUB_MENU_MULTIWIN_WINMODES_ITEM);
                    return TRUE;
                }
                case HWND_MAIN_MENU_OTHER_SETTINGS_ITEM:
                {
                    MApp_ZUI_API_SetFocus(HWND_SUB_MENU_OTHERSET_LANGUAGE_ITEM);
                    return TRUE;
                }

                default:
                    break;
            }
            return FALSE;
        }
        case EN_EXE_GOTO_NEXT_KEY_UP:
        {
            bPreFocusFlag = 0;
            MApp_ZUI_ACT_Show_Select_Page(FALSE);
            MApp_ZUI_API_SetFocusByNav(MApp_ZUI_API_GetFocus(), NAV_UP);
            MApp_ZUI_ACT_Show_Select_Page(TRUE);
        }
        return TRUE;

        case EN_EXE_GOTO_NEXT_KEY_DOWN:
        {
            bPreFocusFlag = 0;
            MApp_ZUI_ACT_Show_Select_Page(FALSE);
            MApp_ZUI_API_SetFocusByNav(MApp_ZUI_API_GetFocus(), NAV_DOWN);
            MApp_ZUI_ACT_Show_Select_Page(TRUE);
        }
        return TRUE;

    }
    return FALSE;
}
void MApp_ZUI_ACT_Show_Select_Page(BOOLEAN bflag)
{
    HWND i = MApp_ZUI_API_GetFocus();

    if(bflag == FALSE) // Hide page
    {
        if(i == HWND_MAIN_MENU_BRI_ITEM)
            MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_BRI_CON_PAGE,SW_HIDE);
        else if(i == HWND_MAIN_MENU_AUTO_ADJUST_ITEM)
            MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_AUTO_ADJUST_PAGE,SW_HIDE);
        else if(i == HWND_MAIN_MENU_INPUT_SOURCE_ITEM)
            MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_PAGE_INP_SRC_ITEM_LIST,SW_HIDE);
        else if(i == HWND_MAIN_MENU_COLOR_SETTINGS_ITEM)
            MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_COL_SET_PAGE,SW_HIDE);
        else if(i == HWND_MAIN_MENU_DISPLAY_SETTINGS_ITEM)
            MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_DIS_SET_PAGE,SW_HIDE);
        else if(i == HWND_MAIN_MENU_MULTI_WIN_SETTINGS_ITEM)
            MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_MULTI_WIN_SET_PAGE,SW_HIDE);
        else
            MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_OTHER_SET_PAGE,SW_HIDE);
    }
    else
    {
        if(i == HWND_MAIN_MENU_BRI_ITEM)
            MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_BRI_CON_PAGE ,SW_SHOW);
        else if(i == HWND_MAIN_MENU_AUTO_ADJUST_ITEM)
            MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_AUTO_ADJUST_PAGE,SW_SHOW);
        else if(i == HWND_MAIN_MENU_INPUT_SOURCE_ITEM)
            MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_PAGE_INP_SRC_ITEM_LIST,SW_SHOW);
        else if(i == HWND_MAIN_MENU_COLOR_SETTINGS_ITEM)
            MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_COL_SET_PAGE,SW_SHOW);
        else if(i == HWND_MAIN_MENU_DISPLAY_SETTINGS_ITEM)
            MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_DIS_SET_PAGE,SW_SHOW);
        else if(i == HWND_MAIN_MENU_MULTI_WIN_SETTINGS_ITEM)
            MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_MULTI_WIN_SET_PAGE,SW_SHOW);
        else
            MApp_ZUI_API_ShowWindow(HWND_SUB_MENU_OTHER_SET_PAGE,SW_SHOW);
    }
}
void MApp_ZUI_ACT_SetOsdLanguage(U8 u8Index)
{
    SET_OSD_MENU_LANGUAGE((EN_LANGUAGE)u8Index);
    MApp_ZUI_API_InvalidateAllSuccessors(HWND_MENU_TRANSPARENT_BG);
}

void MApp_ZUI_ReturnResolution(U8 *strBuffer,U8 WIN_Sel,HWND hwnd)
{
    U16 H_Width,V_Hight;
    U8 v_fre;
    static char locbuf[20];

    //WIN_Sel = 0;
    if(hwnd == HWND_MAIN_MENU_TITLE_RES_NUM)
    {
        if(INPUT_IS_VGA(MapWin2Port(USER_PREF_WIN_SEL)))
        {
            H_Width=StandardModeWidth(MapWin2Port(USER_PREF_WIN_SEL));
            V_Hight=StandardModeHeight(MapWin2Port(USER_PREF_WIN_SEL));
            v_fre=(StandardModeVFreq(MapWin2Port(USER_PREF_WIN_SEL))+5)/10;
        }
        else
        {
            H_Width = msDrvIP1ReadAutoWidth(MapPort2DetScIndex(MapWin2Port(USER_PREF_WIN_SEL)));
            V_Hight = msDrvIP1ReadAutoHeight(MapPort2DetScIndex(MapWin2Port(USER_PREF_WIN_SEL)));
            v_fre = VFREQ( HFREQ( SrcHPeriod(MapWin2Port(USER_PREF_WIN_SEL)) ), SrcVTotal(MapWin2Port(USER_PREF_WIN_SEL)));
        }
    }
    else
    {
        if(INPUT_IS_VGA(MapWin2Port(WIN_Sel)))
        {
            H_Width=StandardModeWidth(MapWin2Port(WIN_Sel));
            V_Hight=StandardModeHeight(MapWin2Port(WIN_Sel));
            v_fre=(StandardModeVFreq(MapWin2Port(WIN_Sel))+5)/10;
        }
        else
        {
            H_Width = msDrvIP1ReadAutoWidth(MapPort2DetScIndex(MapWin2Port(WIN_Sel)));
            V_Hight = msDrvIP1ReadAutoHeight(MapPort2DetScIndex(MapWin2Port(WIN_Sel)));
            v_fre = VFREQ( HFREQ( SrcHPeriod(MapWin2Port(WIN_Sel)) ), SrcVTotal(MapWin2Port(WIN_Sel)));
        }
    }
    memset(locbuf, 0, sizeof(locbuf));
    snprintf(locbuf,sizeof(locbuf), "%ux%u@%uHz", H_Width, V_Hight, v_fre);
    strcpy((char *)strBuffer,locbuf);
}

U16 MApp_ZUI_ACT_GetSourceStringID(U8 Win_Num)
{
    U16 u16TempID= Empty;

#if ENABLE_VGA_INPUT
    if(INPUT_VGA==MapWin2Port(Win_Num))
        u16TempID = en_str_Source_VGA;
	else
#endif		
#if ENABLE_DVI_INPUT
    if(INPUT_DVI==MapWin2Port(Win_Num))
        u16TempID = en_str_Source_DVI;
	else
#endif
#if ENABLE_HDMI_INPUT
    if(INPUT_HDMI==MapWin2Port(Win_Num))
        u16TempID = en_str_HDMI_1;
	else
#endif
#if ENABLE_HDMI2ND_INPUT
    if(INPUT_HDMI2ND==MapWin2Port(Win_Num))
        u16TempID = en_str_HDMI_2;
	else
#endif
#if ENABLE_DP_INPUT
    if(INPUT_DISPLAYPORT==MapWin2Port(Win_Num))
        u16TempID = en_str_Display_Port_1;
	else
#endif
#if ENABLE_DP2ND_INPUT
    if(INPUT_DISPLAYPORT2ND==MapWin2Port(Win_Num))
        u16TempID = en_str_Display_Port_2;
	else
#endif
        u16TempID = Empty;

   return u16TempID;
}
#undef MAPP_ZUI_ACTIDLE_C


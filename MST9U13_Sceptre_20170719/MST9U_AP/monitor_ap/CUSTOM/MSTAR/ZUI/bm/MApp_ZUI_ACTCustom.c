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

#define MAPP_ZUI_ACTENDOCUSTOM_C
#define _ZUI_INTERNAL_INSIDE_ //NOTE: for ZUI internal

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
#include "MApp_ZUI_ACTDirMsgMenu.h"
#include "MApp_ZUI_BM_ActionManager.h"
#include "MApp_ZUI_ACTglobal.h"
#include "MApp_GlobalFunction.h"
#include "UserPrefDef.h"
#include "menudef.h"
#include "menufunc.h"
#include "OsdDefault.h"
#include "Global.h"
#include "Mode.h"
#include "OsdDefault.h"
#include "msScaler.h"
#include "MApp_ZUI_ACTCustom.h"
#include "MApp_LoadFontInit.h"
#include "UserPref.h"
#include "appWindow.h"
#include "Power.h"


// select item handler
void MApp_ZUI_Custom_SelectionHandler(HWND currentHwnd)
{
    UNUSED(currentHwnd);
}

BOOL MApp_ZUI_Custom_ActionHandler( EN_OSD_WNDEXEACTION_ENUM execAct)
{
    UNUSED(execAct);
    return FALSE;
}

BOOL MApp_ZUI_Custom_OpenSceneHandler(EN_SCENE_TYPE sceneType)
{
    BOOL bResult = FALSE;
    switch(sceneType)
    {
#if (!ENABLE_WIDE_MODE)    
        case SCENE_MAINMENU_DISPLAY_L2:
            if(CURRENT_INPUT_IS_VGA())
                MApp_ZUI_API_SetFocus(HWND_SUB_MENU_DISSET_HP_ITEM);
            else
                MApp_ZUI_API_SetFocus(HWND_SUB_MENU_DISSET_SHP_ITEM);    
            bResult = TRUE;
            break;
#endif            
        default:
            break;
    }
    return bResult;
}

BOOL MApp_ZUI_Custom_CloseSceneHandler(EN_SCENE_TYPE sceneType)
{
    UNUSED(sceneType);
    BOOL bResult = FALSE;
    return bResult;
}

Bool MApp_ZUI_Custom_AutoCloseOSDHandler(void)
{
    Bool bResult = TRUE;
    E_OSD_ID eOSD_ID = (E_OSD_ID)MApp_ZUI_GetActiveOSD();  
    
    switch(eOSD_ID)
    {
        case E_OSD_MAIN_MENU:
            SaveMonitorSetting();
            Delay1ms(50);
            MApp_ZUI_ACT_ShutdownOSD();
            break;
        case E_OSD_AUTO_ADJUST:
            MApp_ZUI_ACT_ShutdownOSD();
            MApp_ZUI_ACT_StartupOSD(E_OSD_MAIN_MENU);
            break;
        case E_OSD_DIRMSG_MENU:
            MApp_ZUI_ACT_ShutdownOSD();
            break;
        case E_OSD_LOGO:
            g_stZuiMenuFlag.bDisplayLogo=0;
            Power_TurnOffPanel();
            Set_InputTimingChangeFlag(SrcInputPortM);
            SrcFlags(SrcInputPortM) |= SyncLoss;
            MApp_ZUI_ACT_ShutdownOSD();
        default:
            bResult = FALSE;
            break;
    }
    return bResult;
}

void MApp_ZUI_Custom_TerminateOSDHandler(E_OSD_ID eOSD_ID)
{
    switch(eOSD_ID)
    {
        //please add all OSD_ID here
        case E_OSD_MAIN_MENU:
        case E_OSD_IDLE:
        case E_OSD_AUTO_ADJUST:
        case E_OSD_DIRMSG_MENU:
        case E_OSD_LOGO:
            ZUI_MSG(printf("[]MApp_ZUI_Custom_TerminateOSDHandler, OSDID = %d\n", eOSD_ID));
            break;
        default:
            ZUI_DBG_FAIL(printf("[ZUI]MApp_ZUI_Custom_TerminateOSDHandler default case, OSD_ID=%d\n", eOSD_ID));
            //ABORT();
            break;
    }
}

BOOLEAN MApp_ZUI_Custom_GlobalKeyHandler(E_OSD_ID eOSD_ID, VIRTUAL_KEY_CODE key)
{
    BOOLEAN bResult = FALSE;
    switch(eOSD_ID)
    {
        //please add all OSD_ID here
        case E_OSD_MAIN_MENU:
            MApp_ZUI_API_ResetTimer(HWND_MAIN_MENU_PAGE_BG, 0);
            if(key == VK_POWER)
            {
                if(POWER_ON_FLAG)
                    PowerOffSystem(); //Power off
                else
                    PowerOnSystem(); //Power on
            }
            break;
        case E_OSD_IDLE:
        case E_OSD_AUTO_ADJUST:
        case E_OSD_DIRMSG_MENU:
        case E_OSD_LOGO:
            break;
        default:
            ZUI_DBG_FAIL(printf("[ZUI]MApp_ZUI_Custom_GlobalKeyHandler default case, OSD_ID=%d\n", eOSD_ID));
            //ABORT();
            break;
    }
    return bResult;
}

LPTSTR MApp_ZUI_Custom_GetDynamicTextHandler(HWND hwnd)
{
    E_OSD_ID eOSD_ID = (E_OSD_ID)MApp_ZUI_GetActiveOSD();  
    LPTSTR resultStr = NULL;

    switch(eOSD_ID)
    {
        case E_OSD_MAIN_MENU:
            resultStr = MApp_ZUI_Custom_MainmenuGetDynamicText(hwnd);
            break;
        case E_OSD_DIRMSG_MENU:
            resultStr = MApp_ZUI_Custom_DirMsgMenuGetDynamicText(hwnd);
            break;
        default:
            break;
            
    }
    return resultStr;
}

U16 MApp_ZUI_Custom_GetDynamicBitmapHandler(HWND hwnd, DRAWSTYLE_TYPE type)
{
    UNUSED(hwnd);
    UNUSED(type);
    return E_BMP_EMPTY;
}

S16 MApp_ZUI_Custom_GetDynamicValueHandler(HWND hwnd)
{
    UNUSED(hwnd);
    return DYNAMIC_VALUE_EMPTY; // for empty value
}

Bool MApp_ZUI_Custom_GetDynamicColorHandler(HWND hwnd, DRAWSTYLE_TYPE type, OSD_COLOR colorOriginal, OSD_COLOR* colorTarget)
{
    UNUSED(hwnd);
    UNUSED(type);
    UNUSED(colorOriginal);
    UNUSED(colorTarget);
    
    return FALSE;
}

Bool MApp_ZUI_Custom_GetItemStatusHandler(HWND currentHwnd, GUI_ENUM_DYNAMIC_LIST_STATE* targetState)
{
    UNUSED(currentHwnd);
    UNUSED(targetState);
    return FALSE;
}


#undef MAPP_ZUI_ACTENDOCUSTOM_C


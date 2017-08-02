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
//<MStar Software>
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

#ifndef _MAPP_ZUI_BM_OSDMANAGER_H
#define _MAPP_ZUI_BM_OSDMANAGER_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include "MApp_ZUI_APIgdi.h"
#include "MApp_ZUI_ACTCustom.h"

#ifdef _MAPP_ZUI_BM_OSDMANAGER_H
#define INTERFACE
#else
#define INTERFACE extern
#endif

#define HWND_NULL 0
#define SCENE_KEEP_OLD_WINDOWS HWND_NULL

typedef enum
{
    TYPE_UP_SELECTION,
    TYPE_DOWN_SELECTION,
}UPDOWN_SELECTION_TYPE;

typedef enum _EN_SCENE_NEXT_PAGE_EXEC_ACT
{
    EN_SCENE_NP_EXE_KEY_DOWN,
    EN_SCENE_NP_EXE_KEY_UP,
    EN_SCENE_NP_EXE_NEXT_PAGE,
    EN_SCENE_NP_EXE_PREVIOUS_PAGE,
    EN_SCENE_NP_EXE_ACT_MAX,    
}EN_SCENE_NEXT_PAGE_EXEC_ACT;

typedef struct _OSD_INIT_MAP
{
    EN_SCENE_TYPE eScene;
    U16 nWidth;
    U16 nHeight;
}OSD_INIT_MAP;


typedef struct _SCENE_NEXT_PAGE_ACT_MAP
{
    EN_SCENE_NEXT_PAGE_EXEC_ACT sceneAct;
    EN_OSD_WNDEXEACTION_ENUM osdAct;
}SCENE_NEXT_PAGE_ACT_MAP;

typedef struct _SCENE_FLOW_PATH
{
    EN_SCENE_TYPE sceneType[SCENE_FLOW_PATH_MAX];
    HWND previousFocusHwnd[SCENE_FLOW_PATH_MAX];
    HWND autoTextColorHwnd[SCENE_FLOW_PATH_MAX];
    S16 currentHwndIndex;
}SCENE_FLOW_PATH;

typedef struct _SCENE_NEXT_PAGE_MAP
{
    HWND hwnd;
    EN_SCENE_TYPE nextPageSceneType;
    bool bResetFlowPath;
}SCENE_NEXT_PAGE_MAP;

typedef struct _SCENE_OPEN_COMMON_INIT_MAP
{
    EN_SCENE_TYPE sceneType;
    E_OSD_ID OsdID;
    HWND hwndShow;
    HWND hwndFocus;
    HWND hwndShow1;
    HWND hwndShow2;
}SCENE_OPEN_COMMON_INIT_MAP;

typedef struct _SCENE_SHUTDOWN_HANDLER
{
    EN_SCENE_TYPE sceneType;
    void (*menu_end_function_ptr)(EN_SCENE_TYPE);
}SCENE_SHUTDOWN_HANDLER;

typedef struct _SCENE_OPEN_HANDLER
{
    EN_SCENE_TYPE sceneType;
    void (*open_scene_function_ptr)(EN_SCENE_TYPE);
}SCENE_OPEN_HANDLER;

typedef struct _SCENE_SAVE_SETTING_HANDLER
{
    void (*save_monitor_setting_ptr)(void);
}SCENE_SAVE_SETTING_HANDLER;

typedef struct _SCENE_SAVE_SETTING_MAP
{
    EN_SCENE_TYPE sceneType;
}SCENE_SAVE_SETTING_MAP;

typedef struct _SCENE_AUTO_TEXT_COLOR_MAP
{
    EN_SCENE_TYPE sceneType;
    HWND hwndTextColor;
}SCENE_AUTO_TEXT_COLOR_MAP;

//custom function handler
typedef struct _SCENE_CUSTOM_SELECTION_HANDLER
{
    void (*selection_function_ptr)(HWND);
}SCENE_CUSTOM_SELECTION_HANDLER;

typedef struct _SCENE_CUSTOM_ACTION_HANDLER
{
    BOOL (*action_function_ptr)(EN_OSD_WNDEXEACTION_ENUM);
}SCENE_CUSTOM_ACTION_HANDLER;

typedef struct _SCENE_CUSTOM_OPENSCENE_HANDLER
{
    BOOL (*openscene_function_ptr)(EN_SCENE_TYPE);
}SCENE_CUSTOM_OPENSCENE_HANDLER;

typedef struct _SCENE_CUSTOM_CLOSESCENE_HANDLER
{
    BOOL (*closescene_function_ptr)(EN_SCENE_TYPE);
}SCENE_CUSTOM_CLOSESCENE_HANDLER;

typedef struct _SCENE_CUSTOM_AUTO_CLOSEOSD_HANDLER
{
    Bool (*closeosd_function_ptr)(void);
}SCENE_CUSTOM_AUTO_CLOSEOSD_HANDLER;

typedef struct _SCENE_CUSTOM_TERMINATEOSD_HANDLER
{
    void (*terminateosd_function_ptr)(E_OSD_ID);
}SCENE_CUSTOM_TERMINATEOSD_HANDLER;

typedef struct _SCENE_CUSTOM_GLOBALKEY_HANDLER
{
    BOOLEAN (*globalkey_function_ptr)(E_OSD_ID, VIRTUAL_KEY_CODE);
}SCENE_CUSTOM_GLOBALKEY_HANDLER;

typedef struct _SCENE_CUSTOM_DYNAMICTEXT_HANDLER
{
    LPTSTR (*dynamictext_function_ptr)(HWND);
}SCENE_CUSTOM_DYNAMICTEXT_HANDLER;

typedef struct _SCENE_CUSTOM_DYNAMICBITMAP_HANDLER
{
    U16 (*dynamicbitmap_function_ptr)(HWND, DRAWSTYLE_TYPE);
}SCENE_CUSTOM_DYNAMICBITMAP_HANDLER;

typedef struct _SCENE_CUSTOM_DYNAMICVALUE_HANDLER
{
    S16 (*dynamicvalue_function_ptr)(HWND);
}SCENE_CUSTOM_DYNAMICVALUE_HANDLER;

typedef struct _SCENE_CUSTOM_DYNAMICCOLOR_HANDLER
{
    Bool (*dynamiccolor_function_ptr)(HWND, DRAWSTYLE_TYPE, OSD_COLOR, OSD_COLOR*);
}SCENE_CUSTOM_DYNAMICCOLOR_HANDLER;


typedef struct _SCENE_CUSTOM_DYNAMICLISTITEM_HANDLER
{
    Bool (*dynamiclisttem_function_ptr)(HWND, GUI_ENUM_DYNAMIC_LIST_STATE*);
}SCENE_CUSTOM_DYNAMICLISTITEM_HANDLER;
/////////////////////////////////////////
// Event Handlers....
INTERFACE void MApp_ZUI_ACT_OpenSceneHandler(EN_SCENE_TYPE sceneType);
INTERFACE BOOLEAN MApp_ZUI_ACT_OpenSceneCommon(EN_SCENE_TYPE sceneType);
INTERFACE BOOLEAN MApp_ZUI_ACT_OpenSceneCustom(EN_SCENE_TYPE sceneType);
INTERFACE EN_SCENE_TYPE MApp_ZUI_ACT_GetCurrentSceneType(void);
INTERFACE void MApp_ZUI_ACT_ReturnSceneHandler(void);
INTERFACE SCENE_NEXT_PAGE_MAP MApp_ZUI_ACT_GetNextScenePage(HWND hwnd,  EN_OSD_WNDEXEACTION_ENUM execAct);
INTERFACE BOOLEAN MApp_ZUI_ACT_GetReturnPathAutoColor(HWND hwnd, OSD_COLOR * colorTarget);
INTERFACE void MApp_ZUI_ACT_ResetSceneFlowPath(void);
INTERFACE BOOLEAN MApp_ZUI_ACT_InitOSD(E_OSD_ID osd_id, U16 nWidth, U16 nHeight);
INTERFACE void MApp_ZUI_ACT_OpenOSDHandler(E_OSD_ID eOSD_ID);
INTERFACE void MApp_ZUI_ACT_TerminateOSDHandler(E_OSD_ID eOSD_ID);
INTERFACE BOOLEAN MApp_ZUI_ACT_GlobalKeyHandler(E_OSD_ID eOSD_ID, VIRTUAL_KEY_CODE key);
INTERFACE BOOLEAN MApp_ZUI_ACT_AutoCloseHandler(void);
INTERFACE BOOLEAN MApp_ZUI_ACT_CustomActionHandler(EN_OSD_WNDEXEACTION_ENUM execAct);
INTERFACE void MApp_ZUI_ACT_CustomSelectionHandler(HWND hwnd);
INTERFACE LPTSTR MApp_ZUI_ACT_DynamicTextHandler(HWND hwnd);
INTERFACE U16 MApp_ZUI_ACT_DynamicBitmapHandler(HWND hwnd, DRAWSTYLE_TYPE type);
INTERFACE S16 MApp_ZUI_ACT_DynamicValueHandler(HWND hwnd);
INTERFACE BOOL MApp_ZUI_ACT_DynamicColorHandler(HWND hwnd, DRAWSTYLE_TYPE type, OSD_COLOR colorOriginal, OSD_COLOR* colorTarget);
INTERFACE BOOL MApp_ZUI_ACT_DynamicListItemHandler(HWND hwnd, GUI_ENUM_DYNAMIC_LIST_STATE* targetState);


#undef INTERFACE




#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MAPP_ZUI_BM_OSDMANAGER_H */


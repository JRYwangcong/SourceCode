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
// (MStar Confidential Information) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _MAPP_ZUI_BM_OSDTABLES_H
#define _MAPP_ZUI_BM_OSDTABLES_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include "MApp_ZUI_ACTCustom.h"

#ifdef _MAPP_ZUI_BM_OSDTABLES_H
#define INTERFACE
#else
#define INTERFACE extern
#endif


///////////////////////////////////////////////////////////////////////////////
///  File: MApp_ZUI_BM_OSDTables.h
///         Common flow for build BM's OSD
///         1. User need to use OSDInitMap to initial BM's OSD
///         2. Use SceneOpenCommonInitMap to setup BM's scene
///         3. Use SceneNextPageExecActMap/SceneNextPageMap to setup event driven mapping table.
///
///         Related Files : MApp_ZUI_BM_OSDTables.h  / MApp_ZUI_BM_CustomTables.h / MApp_ZUI_BM_ActionTextTable.h / MApp_ZUI_BM_ActionTables.h 
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Transparent Window ID, default is 1
///////////////////////////////////////////////////////////////////////////////
#define HWND_TRANSPARENT 1  

///////////////////////////////////////////////////////////////////////////////
///Auto change text color when user open/close scene, it will auto change color for all focused items of return path , 
///this setting is dependent on SceneAutoTextColorMap and RETURNPATH_AUTO_TEXTCOLOR
///if user need to use auto text color feature, 
///user must setup RETURNPATH_AUTO_TEXTCOLOR_ENABLE , RETURNPATH_AUTO_TEXTCOLOR and SceneAutoTextColorMap
///////////////////////////////////////////////////////////////////////////////
#define RETURNPATH_AUTO_TEXTCOLOR_ENABLE TRUE   

///////////////////////////////////////////////////////////////////////////////
///customize color for auto text color  
///////////////////////////////////////////////////////////////////////////////
#define RETURNPATH_AUTO_TEXTCOLOR 0xFFFF0000    

///////////////////////////////////////////////////////////////////////////////
///  global static struct OSDInitMap
///         This table is used to initial BM's OSD, this mapping table is between OSD and scene.
///         User need to fill scene ID in table by E_OSD_ID sequence if user use BM to build OSD, 
///         this table include scene ID, OSD width and height.
///         
///         Related Map : SceneOpenCommonInitMap, SceneNextPageExecActMap, SceneNextPageMap
///
/// @struct member        
///        eScene , nWidth, nHeight
///
///////////////////////////////////////////////////////////////////////////////
static OSD_INIT_MAP OSDInitMap[E_OSD_MAX] =
{
    /*
    E_OSD_EMPTY,
    E_OSD_MAIN_MENU,
    E_OSD_IDLE,
    E_OSD_AUTO_ADJUST,
    E_OSD_DIRMSG_MENU,
    E_OSD_LOGO,
    E_OSD_MAX,
    */
    {SCENE_TYPE_MAX, 0 , 0}, //don't remove it , for E_OSD_EMPTY
    {SCENE_MAINMENU_BRIGHTNESS , ZUI_MAIN_MENU_WIDTH, ZUI_MAIN_MENU_HEIGHT}, //E_OSD_MAIN_MENU
    {SCENE_TYPE_MAX, 0 , 0}, //E_OSD_IDLE
    {SCENE_AUTOADJUST_MENU, ZUI_AUTO_ADJUST_WIDTH, ZUI_AUTO_ADJUST_HEIGHT}, //E_OSD_AUTO_ADJUST
    {SCENE_DIRMSG_MENU, ZUI_DIRMSG_MENU_WIDTH, ZUI_DIRMSG_MENU_HEIGHT}, //E_OSD_DIRMSG_MENU
    {SCENE_LOGO, ZUI_LOGO_WIDTH, ZUI_LOGO_HEIGHT}, //E_OSD_LOGO
};

///////////////////////////////////////////////////////////////////////////////
///  global static struct SceneOpenCommonInitMap
///         This table is used to setup BM's scene, 
///         User can assign focus window and visible windows, 
///         BM will set focus to the window and show these windows when BM open scene.
///
///         Related Map : OSDInitMap, SceneNextPageExecActMap, SceneNextPageMap
///
/// @struct member        
///        sceneType , OsdID, hwndShow, hwndFocus, hwndShow1, hwndShow2
///
///////////////////////////////////////////////////////////////////////////////
static SCENE_OPEN_COMMON_INIT_MAP SceneOpenCommonInitMap[SCENE_TYPE_MAX] =
{    
    {SCENE_TYPE_MAX, E_OSD_MAX, HWND_NULL, HWND_NULL, HWND_NULL, HWND_NULL}, //don't remove it , 

    //Main Menu Layer 1
    {SCENE_MAINMENU_BRIGHTNESS,            E_OSD_MAIN_MENU,    HWND_MAIN_MENU_PAGE_BG,         HWND_MAIN_MENU_BRI_ITEM ,    HWND_SUB_MENU_BRI_ITEM_LIST,    HWND_SUB_MENU_PAGE_BRI_ITEM_VALUE_LIST},
    {SCENE_MAINMENU_AUTOADJUST,            E_OSD_MAIN_MENU,    HWND_MAIN_MENU_PAGE_BG,         HWND_MAIN_MENU_AUTO_ADJUST_ITEM,     HWND_NULL, HWND_NULL},
    {SCENE_MAINMENU_INPUTSOURCE,          E_OSD_MAIN_MENU,    HWND_MAIN_MENU_PAGE_BG,         HWND_MAIN_MENU_INPUT_SOURCE_ITEM ,    HWND_SUB_MENU_PAGE_INP_SRC_ITEM_LIST, HWND_NULL},
    {SCENE_MAINMENU_COLOR,                     E_OSD_MAIN_MENU,    HWND_MAIN_MENU_PAGE_BG,         HWND_MAIN_MENU_COLOR_SETTINGS_ITEM  ,    HWND_SUB_MENU_COL_SET_ITEM_LIST,  HWND_SUB_MENU_COL_VALUE_ITEM_LIST},
    {SCENE_MAINMENU_DISPLAY,                   E_OSD_MAIN_MENU,    HWND_MAIN_MENU_PAGE_BG,         HWND_MAIN_MENU_DISPLAY_SETTINGS_ITEM ,     HWND_SUB_MENU_DISSET_ITEM_LIST,  HWND_SUB_MENU_DISSET_ITEM_VALUE_LIST},
    {SCENE_MAINMENU_MULTIWINDOW,         E_OSD_MAIN_MENU,    HWND_MAIN_MENU_PAGE_BG,         HWND_MAIN_MENU_MULTI_WIN_SETTINGS_ITEM ,  HWND_SUB_MENU_MULTIWIN_ITEM_LIST,  HWND_SUB_MENU_MULTIWIN_ITEM_VALUE_LIST},
    {SCENE_MAINMENU_OTHER,                     E_OSD_MAIN_MENU,    HWND_MAIN_MENU_PAGE_BG,         HWND_MAIN_MENU_OTHER_SETTINGS_ITEM ,    HWND_SUB_MENU_OTHER_SET_ITEM_LIST,  HWND_SUB_MENU_OTHER_SET_ITEM_VALUE_LIST},

    //Main Menu Layer 2
    {SCENE_MAINMENU_BRIGHTNESS_L2,            E_OSD_MAIN_MENU,    HWND_MAIN_MENU_PAGE_BG,         HWND_SUB_MENU_BRI_ITEM,   HWND_SUB_MENU_BRI_ITEM_LIST, HWND_SUB_MENU_PAGE_BRI_ITEM_VALUE_LIST},
    {SCENE_MAINMENU_INPUTSOURCE_L2,          E_OSD_MAIN_MENU,    HWND_MAIN_MENU_PAGE_BG,         HWND_SUB_MENU_AUTO_SRC_ITEM,   HWND_SUB_MENU_PAGE_INP_SRC_ITEM_LIST, HWND_NULL},
    {SCENE_MAINMENU_COLOR_L2,                     E_OSD_MAIN_MENU,    HWND_MAIN_MENU_PAGE_BG,         HWND_SUB_MENU_COLSET_INPCOLFMT_ITEM,  HWND_SUB_MENU_COL_SET_ITEM_LIST, HWND_SUB_MENU_COL_VALUE_ITEM_LIST},
#if ENABLE_WIDE_MODE        
    {SCENE_MAINMENU_DISPLAY_L2,                   E_OSD_MAIN_MENU,    HWND_MAIN_MENU_PAGE_BG,         HWND_SUB_MENU_DISSET_ASPRAT_ITEM, HWND_SUB_MENU_DISSET_ITEM_LIST, HWND_SUB_MENU_DISSET_ITEM_VALUE_LIST},
#else
    //TODO: dynamic get focus
    {SCENE_MAINMENU_DISPLAY_L2,                   E_OSD_MAIN_MENU,    HWND_MAIN_MENU_PAGE_BG,       HWND_NULL, HWND_SUB_MENU_DISSET_ITEM_LIST,HWND_SUB_MENU_DISSET_ITEM_VALUE_LIST},
#endif
    {SCENE_MAINMENU_MULTIWINDOW_L2,         E_OSD_MAIN_MENU,    HWND_MAIN_MENU_PAGE_BG,         HWND_SUB_MENU_MULTIWIN_WINMODES_ITEM,   HWND_SUB_MENU_MULTIWIN_ITEM_LIST, HWND_SUB_MENU_MULTIWIN_ITEM_VALUE_LIST},
    {SCENE_MAINMENU_OTHER_L2,                     E_OSD_MAIN_MENU,    HWND_MAIN_MENU_PAGE_BG,         HWND_SUB_MENU_OTHERSET_LANGUAGE_ITEM,     HWND_SUB_MENU_OTHER_SET_ITEM_LIST, HWND_SUB_MENU_OTHER_SET_ITEM_VALUE_LIST},

    //Main Menu Layer 3
    {SCENE_MAINMENU_BRIGHTNESS_L3_BRIGHTNESS,                      E_OSD_MAIN_MENU,    HWND_MAIN_MENU_PAGE_BG,    HWND_SUB_MENU_BRIADJ_VALUE_ITEM,  HWND_SUB_MENU_BRI_ITEM_LIST,  HWND_SUB_MENU_BRIADJ_VALUE_ITEM},
    {SCENE_MAINMENU_BRIGHTNESS_L3_CONTRAST,                         E_OSD_MAIN_MENU,    HWND_MAIN_MENU_PAGE_BG,    HWND_SUB_MENU_CONADJ_VALUE_ITEM,  HWND_SUB_MENU_BRI_ITEM_LIST,  HWND_SUB_MENU_CONADJ_VALUE_ITEM},
#if ENABLE_DCR 
    //TODO: dynamic get focus from get function
    {SCENE_MAINMENU_BRIGHTNESS_L3_DYNAMICCONTRAST,           E_OSD_MAIN_MENU,    HWND_MAIN_MENU_PAGE_BG,    HWND_NULL,  HWND_SUB_MENU_BRI_ITEM_LIST,    HWND_SUB_MENU_BRICON_DYN_CON_ITEM_LIST},
#else
    {SCENE_MAINMENU_BRIGHTNESS_L3_DYNAMICCONTRAST,           E_OSD_MAIN_MENU,    HWND_MAIN_MENU_PAGE_BG,    HWND_SUB_MENU_BRICON_DYN_CON_OFF_ITEM,  HWND_SUB_MENU_BRI_ITEM_LIST,    HWND_SUB_MENU_BRICON_DYN_CON_ITEM_LIST},
#endif

    //TODO: dynamic get focus
    {SCENE_MAINMENU_COLOR_L3_INPUT_COLOR,                     E_OSD_MAIN_MENU,     HWND_MAIN_MENU_PAGE_BG,         HWND_NULL  ,    HWND_SUB_MENU_COL_SET_ITEM_LIST,  HWND_SUB_MENU_COLSET_INPCOLFMT_ITEM_LIST},
    {SCENE_MAINMENU_COLOR_L3_PRESET_MODE,                     E_OSD_MAIN_MENU,    HWND_MAIN_MENU_PAGE_BG,         HWND_NULL  ,    HWND_SUB_MENU_COL_SET_ITEM_LIST,  HWND_SUB_MENU_COLSET_PREMODE_ITEM_LIST},
    {SCENE_MAINMENU_COLOR_L3_SUPER_RESOLUTION,            E_OSD_MAIN_MENU,    HWND_MAIN_MENU_PAGE_BG,         HWND_NULL  ,    HWND_SUB_MENU_COL_SET_ITEM_LIST,  HWND_SUB_MENU_COLSET_SUPERRES_ITEM_LIST},
    {SCENE_MAINMENU_COLOR_L3_HUE,                                    E_OSD_MAIN_MENU,    HWND_MAIN_MENU_PAGE_BG,         HWND_SUB_MENU_HUE_VALUE_ITEM  ,    HWND_SUB_MENU_COL_SET_ITEM_LIST,  HWND_SUB_MENU_HUE_VALUE_ITEM},
    {SCENE_MAINMENU_COLOR_L3_SATURATION,                       E_OSD_MAIN_MENU,    HWND_MAIN_MENU_PAGE_BG,         HWND_SUB_MENU_SAT_VALUE_ITEM  ,    HWND_SUB_MENU_COL_SET_ITEM_LIST,  HWND_SUB_MENU_SAT_VALUE_ITEM},
    {SCENE_MAINMENU_COLOR_L3_CUSTOM_B,                          E_OSD_MAIN_MENU,    HWND_MAIN_MENU_PAGE_BG,         HWND_SUB_MENU_CUSTOM_B_VALUE_ITEM  ,    HWND_SUB_MENU_COL_SET_ITEM_LIST,  HWND_SUB_MENU_CUSTOM_B_VALUE_ITEM},

    {SCENE_MAINMENU_DISPLAY_L3_ASPECT_RATIO,             E_OSD_MAIN_MENU,    HWND_MAIN_MENU_PAGE_BG,         HWND_NULL ,     HWND_SUB_MENU_DISSET_ITEM_LIST,  HWND_SUB_MENU_DISSET_ASPRATIO_ITEM_LIST},
    {SCENE_MAINMENU_DISPLAY_L3_HOR_POS,                     E_OSD_MAIN_MENU,    HWND_MAIN_MENU_PAGE_BG,         HWND_SUB_MENU_HP_VALUE_ITEM ,     HWND_SUB_MENU_DISSET_ITEM_LIST,  HWND_SUB_MENU_HP_VALUE_ITEM},
    {SCENE_MAINMENU_DISPLAY_L3_VER_POS,                     E_OSD_MAIN_MENU,    HWND_MAIN_MENU_PAGE_BG,         HWND_SUB_MENU_VP_VALUE_ITEM ,     HWND_SUB_MENU_DISSET_ITEM_LIST,  HWND_SUB_MENU_VP_VALUE_ITEM},
    {SCENE_MAINMENU_DISPLAY_L3_SHARPNESS,                 E_OSD_MAIN_MENU,    HWND_MAIN_MENU_PAGE_BG,         HWND_SUB_MENU_SHP_VALUE_ITEM ,     HWND_SUB_MENU_DISSET_ITEM_LIST,  HWND_SUB_MENU_SHP_VALUE_ITEM},
    {SCENE_MAINMENU_DISPLAY_L3_PIXEL_CLOCK,              E_OSD_MAIN_MENU,    HWND_MAIN_MENU_PAGE_BG,         HWND_SUB_MENU_PIXCLK_VALUE_ITEM ,     HWND_SUB_MENU_DISSET_ITEM_LIST,  HWND_SUB_MENU_PIXCLK_VALUE_ITEM},
    {SCENE_MAINMENU_DISPLAY_L3_PHASE,                         E_OSD_MAIN_MENU,    HWND_MAIN_MENU_PAGE_BG,         HWND_SUB_MENU_PHS_VALUE_ITEM ,     HWND_SUB_MENU_DISSET_ITEM_LIST,  HWND_SUB_MENU_PHS_VALUE_ITEM},
    {SCENE_MAINMENU_DISPLAY_L3_RESPONSE_TIME,          E_OSD_MAIN_MENU,    HWND_MAIN_MENU_PAGE_BG,         HWND_NULL ,     HWND_SUB_MENU_DISSET_ITEM_LIST,  HWND_SUB_MENU_DISSET_RESPONSETIME_ITEM_LIST},

    {SCENE_MAINMENU_MULTIWINDOW_L3_WIN_MODE,                  E_OSD_MAIN_MENU,    HWND_MAIN_MENU_PAGE_BG,         HWND_NULL ,  HWND_SUB_MENU_MULTIWIN_ITEM_LIST,  HWND_SUB_MENU_MULTIWIN_WINMODES_ITEM_LIST},
    {SCENE_MAINMENU_MULTIWINDOW_L3_WIN_SEL,                     E_OSD_MAIN_MENU,    HWND_MAIN_MENU_PAGE_BG,         HWND_NULL ,  HWND_SUB_MENU_MULTIWIN_ITEM_LIST,  HWND_SUB_MENU_MULTIWIN_WINSEL_ITEM_LIST},
    {SCENE_MAINMENU_MULTIWINDOW_L3_PIP_SIZE,                     E_OSD_MAIN_MENU,    HWND_MAIN_MENU_PAGE_BG,         HWND_NULL ,  HWND_SUB_MENU_MULTIWIN_ITEM_LIST,  HWND_SUB_MENU_MULTIWIN_PIPSIZE_ITEM_LIST},
    {SCENE_MAINMENU_MULTIWINDOW_L3_PIP_HOR_POS,              E_OSD_MAIN_MENU,    HWND_MAIN_MENU_PAGE_BG,         HWND_SUB_MENU_MULTIWIN_PIPHP_VALUE_ITEM ,  HWND_SUB_MENU_MULTIWIN_ITEM_LIST,  HWND_SUB_MENU_MULTIWIN_PIPHP_VALUE_ITEM},
    {SCENE_MAINMENU_MULTIWINDOW_L3_PIP_VER_POS,              E_OSD_MAIN_MENU,    HWND_MAIN_MENU_PAGE_BG,         HWND_SUB_MENU_MULTIWIN_PIPVP_VALUE_ITEM ,  HWND_SUB_MENU_MULTIWIN_ITEM_LIST,  HWND_SUB_MENU_MULTIWIN_PIPVP_VALUE_ITEM},

    {SCENE_MAINMENU_OTHER_L3_LANGUAGE,                     E_OSD_MAIN_MENU,    HWND_MAIN_MENU_PAGE_BG,         HWND_NULL ,    HWND_SUB_MENU_OTHER_SET_ITEM_LIST,  HWND_SUB_MENU_OTHER_SET_LAN_ITEM_LIST},
    {SCENE_MAINMENU_OTHER_L3_MENU_TRANSPARENCY,    E_OSD_MAIN_MENU,    HWND_MAIN_MENU_PAGE_BG,         HWND_SUB_MENU_MENUTPR_VALUE_ITEM ,    HWND_SUB_MENU_OTHER_SET_ITEM_LIST,  HWND_SUB_MENU_MENUTPR_VALUE_ITEM},

    //for DirMsgMenu
    {SCENE_DIRMSG_MENU,    E_OSD_DIRMSG_MENU,    HWND_DIRECT_MSG_MENU,         HWND_NULL ,    HWND_NULL,  HWND_NULL},

    //for Auto Adjust
    {SCENE_AUTOADJUST_MENU,    E_OSD_AUTO_ADJUST,    HWND_DIRECT_MENU_AUTO_ADJUST_BG,         HWND_NULL ,    HWND_NULL,  HWND_NULL},

    //for LOGO
    {SCENE_LOGO,    E_OSD_LOGO,    HWND_LOGO,         HWND_LOGO ,    HWND_NULL,  HWND_NULL},
};

///////////////////////////////////////////////////////////////////////////////
///  global static struct SceneNextPageExecActMap
///         This mapping table is used to map between scene key event and ZUI key event.
///         User need to fill scene key event in SceneNextPageMap.
///         PS. SceneNextPageMap table is dependent on this table
///
///         Related Map : SceneNextPageMap, OSDInitMap, SceneOpenCommonInitMap, 
///
/// @struct member        
///         sceneType , OsdID, hwndShow, hwndFocus, hwndShow1, hwndShow2
///
///////////////////////////////////////////////////////////////////////////////
static SCENE_NEXT_PAGE_ACT_MAP SceneNextPageExecActMap[EN_SCENE_NP_EXE_ACT_MAX] = 
{ 
    {EN_SCENE_NP_EXE_KEY_DOWN , EN_EXE_KEY_DOWN_PAGE},
    {EN_SCENE_NP_EXE_KEY_UP, EN_EXE_KEY_UP_PAGE},
    {EN_SCENE_NP_EXE_NEXT_PAGE, EN_EXE_NEXT_PAGE},
    {EN_SCENE_NP_EXE_PREVIOUS_PAGE, EN_EXE_PREVIOUS_PAGE},
};

///////////////////////////////////////////////////////////////////////////////
///  global static struct SceneNextPageExecActMap
///         This is event driven mapping table that dependents on SceneNextPageExecActMap table.
///         When user press key in the window of this table , BM will change current scene into the scene of this table.
///         
///         PS. BM will build scene path when user use EN_SCENE_NP_EXE_NEXT_PAGE in this table,
///         BM can auto return to previous scene after build scene path.
///         
///         Related Map : OSDInitMap, SceneOpenCommonInitMap, SceneNextPageExecActMap
///
/// @struct member        
///        Focus HWND,     Scene Type of Next Page,  Execute Action,  Reset Flow Path
///
///////////////////////////////////////////////////////////////////////////////
static SCENE_NEXT_PAGE_MAP SceneNextPageMap[EN_SCENE_NP_EXE_ACT_MAX][SCENE_NEXT_PAGE_MAP_MAX] =
{ 
    { //EN_SCENE_NP_EXE_KEY_DOWN
        {HWND_MAIN_MENU_BRI_ITEM,        SCENE_MAINMENU_AUTOADJUST,  TRUE},
        {HWND_MAIN_MENU_AUTO_ADJUST_ITEM,        SCENE_MAINMENU_INPUTSOURCE,  TRUE},
        {HWND_MAIN_MENU_INPUT_SOURCE_ITEM,        SCENE_MAINMENU_COLOR,   TRUE},
        {HWND_MAIN_MENU_COLOR_SETTINGS_ITEM,        SCENE_MAINMENU_DISPLAY,     TRUE},
        {HWND_MAIN_MENU_DISPLAY_SETTINGS_ITEM,        SCENE_MAINMENU_MULTIWINDOW,     TRUE},
        {HWND_MAIN_MENU_MULTI_WIN_SETTINGS_ITEM,        SCENE_MAINMENU_OTHER,     TRUE},
        {HWND_MAIN_MENU_OTHER_SETTINGS_ITEM,        SCENE_MAINMENU_BRIGHTNESS,     TRUE},
    },
    {//EN_SCENE_NP_EXE_KEY_UP
        {HWND_MAIN_MENU_BRI_ITEM,        SCENE_MAINMENU_OTHER,     TRUE},
        {HWND_MAIN_MENU_AUTO_ADJUST_ITEM,        SCENE_MAINMENU_BRIGHTNESS,     TRUE},
        {HWND_MAIN_MENU_INPUT_SOURCE_ITEM,        SCENE_MAINMENU_AUTOADJUST,     TRUE},
        {HWND_MAIN_MENU_COLOR_SETTINGS_ITEM,        SCENE_MAINMENU_INPUTSOURCE,     TRUE},
        {HWND_MAIN_MENU_DISPLAY_SETTINGS_ITEM,        SCENE_MAINMENU_COLOR,     TRUE},
        {HWND_MAIN_MENU_MULTI_WIN_SETTINGS_ITEM,        SCENE_MAINMENU_DISPLAY,     TRUE},
        {HWND_MAIN_MENU_OTHER_SETTINGS_ITEM,        SCENE_MAINMENU_MULTIWINDOW,     TRUE},    
    },    
    {//EN_SCENE_NP_EXE_NEXT_PAGE
        {HWND_MAIN_MENU_BRI_ITEM,        SCENE_MAINMENU_BRIGHTNESS_L2,  FALSE},
        //{HWND_MAIN_MENU_AUTO_ADJUST_ITEM,        SCENE_MAINMENU_AUTOADJUST_L2,  FALSE},
        {HWND_MAIN_MENU_INPUT_SOURCE_ITEM,        SCENE_MAINMENU_INPUTSOURCE_L2, FALSE},
        {HWND_MAIN_MENU_COLOR_SETTINGS_ITEM,        SCENE_MAINMENU_COLOR_L2,    FALSE},
        {HWND_MAIN_MENU_DISPLAY_SETTINGS_ITEM,        SCENE_MAINMENU_DISPLAY_L2,    FALSE},
        {HWND_MAIN_MENU_MULTI_WIN_SETTINGS_ITEM,        SCENE_MAINMENU_MULTIWINDOW_L2,  FALSE},
        {HWND_MAIN_MENU_OTHER_SETTINGS_ITEM,        SCENE_MAINMENU_OTHER_L2, FALSE},
        {HWND_SUB_MENU_BRI_ITEM,        SCENE_MAINMENU_BRIGHTNESS_L3_BRIGHTNESS,  FALSE},
        {HWND_SUB_MENU_CON_ITEM,        SCENE_MAINMENU_BRIGHTNESS_L3_CONTRAST,  FALSE},
        {HWND_SUB_MENU_DYNAMIC_CON_ITEM,        SCENE_MAINMENU_BRIGHTNESS_L3_DYNAMICCONTRAST,  FALSE},
        {HWND_SUB_MENU_COLSET_INPCOLFMT_ITEM,        SCENE_MAINMENU_COLOR_L3_INPUT_COLOR,  FALSE},
        {HWND_SUB_MENU_COLSET_PREMOD_ITEM,        SCENE_MAINMENU_COLOR_L3_PRESET_MODE,  FALSE},
        {HWND_SUB_MENU_COLSET_SUPERRES_ITEM,        SCENE_MAINMENU_COLOR_L3_SUPER_RESOLUTION,  FALSE},
        {HWND_SUB_MENU_COLSET_HUE_ITEM,        SCENE_MAINMENU_COLOR_L3_HUE,  FALSE},
        {HWND_SUB_MENU_COLSET_SAT_ITEM,        SCENE_MAINMENU_COLOR_L3_SATURATION,  FALSE},
        {HWND_SUB_MENU_ITEM5,        SCENE_MAINMENU_COLOR_L3_CUSTOM_B,  FALSE},

        {HWND_SUB_MENU_DISSET_ASPRAT_ITEM,        SCENE_MAINMENU_DISPLAY_L3_ASPECT_RATIO,  FALSE},
        {HWND_SUB_MENU_DISSET_HP_ITEM,        SCENE_MAINMENU_DISPLAY_L3_HOR_POS,  FALSE},
        {HWND_SUB_MENU_DISSET_VP_ITEM,        SCENE_MAINMENU_DISPLAY_L3_VER_POS,  FALSE},
        {HWND_SUB_MENU_DISSET_SHP_ITEM,        SCENE_MAINMENU_DISPLAY_L3_SHARPNESS,  FALSE},
        {HWND_SUB_MENU_DISSET_PIXCLK_ITEM,        SCENE_MAINMENU_DISPLAY_L3_PIXEL_CLOCK,  FALSE},
        {HWND_SUB_MENU_DISSET_PHS_ITEM,        SCENE_MAINMENU_DISPLAY_L3_PHASE,  FALSE},
        {HWND_SUB_MENU_DISSET_RESPONSETIME_ITEM,        SCENE_MAINMENU_DISPLAY_L3_RESPONSE_TIME,  FALSE},
        {HWND_SUB_MENU_MULTIWIN_WINMODES_ITEM,        SCENE_MAINMENU_MULTIWINDOW_L3_WIN_MODE,  FALSE},
        {HWND_SUB_MENU_MULTIWIN_WINSEL_ITEM,        SCENE_MAINMENU_MULTIWINDOW_L3_WIN_SEL,  FALSE},
        {HWND_SUB_MENU_MULTIWIN_PIPSIZE_ITEM,        SCENE_MAINMENU_MULTIWINDOW_L3_PIP_SIZE,  FALSE},
        {HWND_SUB_MENU_MULTIWIN_PIPHP_ITEM,        SCENE_MAINMENU_MULTIWINDOW_L3_PIP_HOR_POS,  FALSE},
        {HWND_SUB_MENU_MULTIWIN_PIPVP_ITEM,        SCENE_MAINMENU_MULTIWINDOW_L3_PIP_VER_POS,  FALSE},
        {HWND_SUB_MENU_OTHERSET_LANGUAGE_ITEM,        SCENE_MAINMENU_OTHER_L3_LANGUAGE,  FALSE},
        {HWND_SUB_MENU_OTHERSET_TPC_ITEM,        SCENE_MAINMENU_OTHER_L3_MENU_TRANSPARENCY,  FALSE},
    },
};

///////////////////////////////////////////////////////////////////////////////
///  global static struct SceneAutoTextColorMap
///         this table is for auto change color feature
///         user need to set RETURNPATH_AUTO_TEXTCOLOR_ENABLE and RETURNPATH_AUTO_TEXTCOLOR first,
///         then user can use this table to map between scene id and auto color text window.
///         finally, BM will auto set color for all color text window of return path
///
///         Related Setting : RETURNPATH_AUTO_TEXTCOLOR_ENABLE , RETURNPATH_AUTO_TEXTCOLOR
///
/// @struct member        
///        sceneType (scene type),     hwndTextColor (previous color text hwnd )
///
///////////////////////////////////////////////////////////////////////////////
static SCENE_AUTO_TEXT_COLOR_MAP SceneAutoTextColorMap[SCENE_TYPE_MAX] = {
    {SCENE_MAINMENU_BRIGHTNESS_L2,  HWND_MAIN_MENU_BRI_TEXT},
    {SCENE_MAINMENU_INPUTSOURCE_L2, HWND_MAIN_MENU_INPUT_SOURCE_TEXT},
    {SCENE_MAINMENU_COLOR_L2, HWND_MAIN_MENU_COLOR_SETTINGS_TEXT},
    {SCENE_MAINMENU_DISPLAY_L2, HWND_MAIN_MENU_DISPLAY_SETTINGS_TEXT},
    {SCENE_MAINMENU_MULTIWINDOW_L2,HWND_MAIN_MENU_MULTI_WIN_SETTINGS_TEXT },
    {SCENE_MAINMENU_OTHER_L2, HWND_MAIN_MENU_OTHER_SETTINGS_TEXT},

    {SCENE_MAINMENU_BRIGHTNESS_L3_BRIGHTNESS,  HWND_SUB_MENU_BRI_TEXT},
    {SCENE_MAINMENU_BRIGHTNESS_L3_CONTRAST,  HWND_SUB_MENU_CON_TEXT},
    {SCENE_MAINMENU_BRIGHTNESS_L3_DYNAMICCONTRAST,  HWND_SUB_MENU_DYNAMIC_CON_TEXT},

    {SCENE_MAINMENU_COLOR_L3_INPUT_COLOR,  HWND_SUB_MENU_COLSET_INPCOLFMT_ITEM_TEXT},
    {SCENE_MAINMENU_COLOR_L3_PRESET_MODE,  HWND_SUB_MENU_COLSET_PREMOD_ITEM_TEXT},
    {SCENE_MAINMENU_COLOR_L3_SUPER_RESOLUTION,  HWND_SUB_MENU_COLSET_SUPERRES_ITEM_TEXT},
    {SCENE_MAINMENU_COLOR_L3_HUE,  HWND_SUB_MENU_COLSET_HUE_ITEM_TEXT},
    {SCENE_MAINMENU_COLOR_L3_SATURATION,  HWND_SUB_MENU_COLSET_SAT_ITEM_TEXT},
    {SCENE_MAINMENU_COLOR_L3_CUSTOM_B,  HWND_SUB_MENU_ITEM5_TEXT},
    {SCENE_MAINMENU_DISPLAY_L3_ASPECT_RATIO,  HWND_SUB_MENU_DISSET_ASPRAT_ITEM_TEXT},
    {SCENE_MAINMENU_DISPLAY_L3_HOR_POS,  HWND_SUB_MENU_DISSET_HP_ITEM_TEXT},
    {SCENE_MAINMENU_DISPLAY_L3_VER_POS,  HWND_SUB_MENU_DISSET_VP_ITEM_TEXT},
    {SCENE_MAINMENU_DISPLAY_L3_SHARPNESS,  HWND_SUB_MENU_DISSET_SHP_ITEM_TEXT},
    {SCENE_MAINMENU_DISPLAY_L3_PIXEL_CLOCK,  HWND_SUB_MENU_DISSET_PIXCLK_ITEM_TEXT},
    {SCENE_MAINMENU_DISPLAY_L3_PHASE,  HWND_SUB_MENU_DISSET_PHS_ITEM_TEXT},
    {SCENE_MAINMENU_DISPLAY_L3_RESPONSE_TIME,  HWND_SUB_MENU_DISSET_RESPONSETIME_ITEM_TEXT},
    {SCENE_MAINMENU_MULTIWINDOW_L3_WIN_MODE,  HWND_SUB_MENU_MULTIWIN_WINMODES_ITEM_TEXT},
    {SCENE_MAINMENU_MULTIWINDOW_L3_WIN_SEL,  HWND_SUB_MENU_MULTIWIN_WINSEL_ITEM_TEXT},
    {SCENE_MAINMENU_MULTIWINDOW_L3_PIP_SIZE,  HWND_SUB_MENU_MULTIWIN_PIPSIZE_ITEM_TEXT},
    {SCENE_MAINMENU_MULTIWINDOW_L3_PIP_HOR_POS,  HWND_SUB_MENU_MULTIWIN_PIPHP_ITEM_TEXT},
    {SCENE_MAINMENU_MULTIWINDOW_L3_PIP_VER_POS,  HWND_SUB_MENU_MULTIWIN_PIPVP_ITEM_TEXT},
    {SCENE_MAINMENU_OTHER_L3_LANGUAGE,  HWND_SUB_MENU_OTHERSET_LANGUAGE_ITEM_TEXT},
    {SCENE_MAINMENU_OTHER_L3_MENU_TRANSPARENCY,  HWND_SUB_MENU_OTHERSET_TPC_ITEM_TEXT},    
};

#undef INTERFACE

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MAPP_ZUI_BM_OSDTABLES_H */


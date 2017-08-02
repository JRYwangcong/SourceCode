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

#ifndef _MAPP_ZUI_BM_ACTIONTABLES_H
#define _MAPP_ZUI_BM_ACTIONTABLES_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include "MApp_ZUI_BM_ActionManager.h"
#include "MApp_ZUI_BM_ActionTextTables.h"
#include "menufunc.h"
#include "appWindow.h"
#include "UserPref.h"
#include "UserPrefDef.h"
#include "MApp_ZUI_ACTCustom.h"
#include "MApp_ZUI_ACTCustomFunc.h"


///////////////////////////////////////////////////////////////////////////////
///  global static struct SceneAdjustmentMap
///  Adjustment table for ZUI adjust item, 
///  it supports "EN_EXE_ADJUST_MENU_INCREASE" and "EN_EXE_ADJUST_MENU_DECREASE" action.
///
///  BM will auto adjust and show current value when user use this table and use increase/decrease actions in *_tables_c.inl
///
/// for GetDynamicValue case, the adjustment function field is NULL.
/// example: {HWND ID,    NULL,    GetValue Function},
/// {SCENE_TYPE_SCREEN_SIZE_MENU, HWND_BV_MENU_BAR_BRIGHTNESS,NULL,GetBrightnessValue},    
///
/// @struct member        
///     Scene Type,  
///     HWND,   
///     Adjustment Function Pointer, 
///     getter function pointer(get current value) 
///
///////////////////////////////////////////////////////////////////////////////
static SCENE_ADJUSTMENT_HANDLER_MAP SceneAdjustmentMap[] = {
    {SCENE_MAINMENU_BRIGHTNESS_L3_BRIGHTNESS, HWND_SUB_MENU_BRIADJ_VALUE_ITEM, AdjustBrightness, GetBrightnessValue},
    {SCENE_MAINMENU_BRIGHTNESS_L3_CONTRAST, HWND_SUB_MENU_CONADJ_VALUE_ITEM, AdjustContrast, GetContrastValue},

    {SCENE_MAINMENU_DISPLAY_L3_HOR_POS, HWND_SUB_MENU_HP_VALUE_ITEM, AdjustHPosition, GetHPositionValue},
    {SCENE_MAINMENU_DISPLAY_L3_VER_POS, HWND_SUB_MENU_VP_VALUE_ITEM, AdjustVPosition, GetVPositionValue},

    {SCENE_MAINMENU_DISPLAY_L3_SHARPNESS, HWND_SUB_MENU_SHP_VALUE_ITEM, AdjustSharpness, GetSharpnessValue},

    {SCENE_MAINMENU_DISPLAY_L3_PIXEL_CLOCK, HWND_SUB_MENU_PIXCLK_VALUE_ITEM, AdjustClock, GetClockValue},
    {SCENE_MAINMENU_DISPLAY_L3_PHASE, HWND_SUB_MENU_PHS_VALUE_ITEM, AdjustPhase, GetPhaseValue},

    {SCENE_MAINMENU_OTHER_L3_MENU_TRANSPARENCY, HWND_SUB_MENU_MENUTPR_VALUE_ITEM, AdjustTransparency, GetTransparencyValue},

    {SCENE_MAINMENU_COLOR_L3_HUE, HWND_SUB_MENU_HUE_VALUE_ITEM, MApp_ZUI_Custom_AdjHueOrRedColor, MApp_ZUI_Custom_GetHueOrRedColor},
    {SCENE_MAINMENU_COLOR_L3_SATURATION, HWND_SUB_MENU_SAT_VALUE_ITEM, MApp_ZUI_Custom_AdjSaturationOrGreenColor, MApp_ZUI_Custom_GetSaturationOrGreenColor},
    {SCENE_MAINMENU_COLOR_L3_CUSTOM_B, HWND_SUB_MENU_CUSTOM_B_VALUE_ITEM, AdjustUserBlueColor, GetUserBlueColorValue},

#if ENABLE_MULTI_WINDOW_SETTINGS_MENU
    {SCENE_MAINMENU_MULTIWINDOW_L3_PIP_HOR_POS, HWND_SUB_MENU_MULTIWIN_PIPHP_VALUE_ITEM, AdjustPIPHPosition, GetPIPHPositionValue},
    {SCENE_MAINMENU_MULTIWINDOW_L3_PIP_VER_POS, HWND_SUB_MENU_MULTIWIN_PIPVP_VALUE_ITEM, AdjustPIPVPosition, GetPIPVPositionValue},
#endif

};

///////////////////////////////////////////////////////////////////////////////
///  global static struct SceneSelectionMap
///  Adjustment table for ZUI selection item, 
/// it supports EN_EXE_KEY_DOWN_CHANGE_VALUE/EN_EXE_KEY_UP_CHANGE_VALUE and EN_EXE_SELECT_ITEM actions.
///
/// BM will auto select to correct function and show current value when user use this table and use select actions in *_tables_c.inl .
/// If you want to show text, you need to use text map for data mapping between value with text.
///
/// @struct member        
///        Scene Type (it just like Menu ID or Group ID, for setFocus when open menu),
///        Current HWND,
///        Adjustment Function Pointer,
///        Adjustment parameter,
///        Getter Function Pointer, (this function is for "Auto focus to previous focus item" feature. it will get value then mapping to parameter and CurrentHWND, finally it will SetFocus on CurrentHWND)
///
///////////////////////////////////////////////////////////////////////////////

static SCENE_SELECTION_MAP SceneSelectionMap[] = {
    {SCENE_MAINMENU_INPUTSOURCE_L2, HWND_SUB_MENU_AUTO_SRC_ITEM,  MApp_ZUI_Adjust_SetInputSource, (int)INPUT_PRIORITY_AUTO, NULL},
    {SCENE_MAINMENU_INPUTSOURCE_L2, HWND_SUB_MENU_VGA_ITEM,  MApp_ZUI_Adjust_SetInputSource, (int)INPUT_PRIORITY_VGA, NULL},
    {SCENE_MAINMENU_INPUTSOURCE_L2, HWND_SUB_MENU_DVI_ITEM,  MApp_ZUI_Adjust_SetInputSource, (int)INPUT_PRIORITY_DVI, NULL},
    {SCENE_MAINMENU_INPUTSOURCE_L2, HWND_SUB_MENU_HDMI1_ITEM,  MApp_ZUI_Adjust_SetInputSource, (int)INPUT_PRIORITY_HDMI, NULL},
    {SCENE_MAINMENU_INPUTSOURCE_L2, HWND_SUB_MENU_HDMI2_ITEM,  MApp_ZUI_Adjust_SetInputSource, (int)INPUT_PRIORITY_HDMI2ND, NULL},
    {SCENE_MAINMENU_INPUTSOURCE_L2, HWND_SUB_MENU_DP1_ITEM,  MApp_ZUI_Adjust_SetInputSource, (int)INPUT_PRIORITY_DP, NULL},
    {SCENE_MAINMENU_INPUTSOURCE_L2, HWND_SUB_MENU_DP2_ITEM,  MApp_ZUI_Adjust_SetInputSource, (int)INPUT_PRIORITY_DP2ND, NULL},

#if ENABLE_DCR
    {SCENE_MAINMENU_BRIGHTNESS_L3_DYNAMICCONTRAST, HWND_SUB_MENU_BRICON_DYN_CON_OFF_ITEM, MApp_ZUI_Adjust_SetDynamicContrast , 1 /*off*/,GetDCREnableRealValue },
    {SCENE_MAINMENU_BRIGHTNESS_L3_DYNAMICCONTRAST, HWND_SUB_MENU_BRICON_DYN_CON_ON_ITEM, MApp_ZUI_Adjust_SetDynamicContrast , 0/*on*/,GetDCREnableRealValue },
#endif

    {SCENE_MAINMENU_COLOR_L3_INPUT_COLOR, HWND_SUB_MENU_COLSET_INPCOLFMT_RGB_ITEM, MApp_ZUI_Adjust_SetInputColorFormat , (int)INPUT_COLOR_RGB, GetInputColorFormatRealValue},
    {SCENE_MAINMENU_COLOR_L3_INPUT_COLOR, HWND_SUB_MENU_COLSET_INPCOLFMT_YPBPR_ITEM, MApp_ZUI_Adjust_SetInputColorFormat , (int)INPUT_COLOR_YPBPR, GetInputColorFormatRealValue},
    {SCENE_MAINMENU_COLOR_L3_INPUT_COLOR, HWND_SUB_MENU_COLSET_INPCOLFMT_AUTO_ITEM, MApp_ZUI_Adjust_SetInputColorFormat , (int)INPUT_COLOR_FORMAT_AUTO, GetInputColorFormatRealValue},

    {SCENE_MAINMENU_COLOR_L3_PRESET_MODE, HWND_SUB_MENU_COLSET_PREMODE_6500K_ITEM, MApp_ZUI_Adjust_SetColorTemperature , (int)CT_STANDARD, MApp_ZUI_Adjust_GetColorTemperature},
    {SCENE_MAINMENU_COLOR_L3_PRESET_MODE, HWND_SUB_MENU_COLSET_PREMODE_MOVIE_ITEM,MApp_ZUI_Adjust_SetColorTemperature  , (int)CT_MOVIE, MApp_ZUI_Adjust_GetColorTemperature},
    {SCENE_MAINMENU_COLOR_L3_PRESET_MODE, HWND_SUB_MENU_COLSET_PREMODE_GAME_ITEM, MApp_ZUI_Adjust_SetColorTemperature , (int)CT_GAME, MApp_ZUI_Adjust_GetColorTemperature},
    {SCENE_MAINMENU_COLOR_L3_PRESET_MODE, HWND_SUB_MENU_COLSET_INPCOLFMT_5700K_ITEM, MApp_ZUI_Adjust_SetColorTemperature , (int)CT_WARM, MApp_ZUI_Adjust_GetColorTemperature},
    {SCENE_MAINMENU_COLOR_L3_PRESET_MODE, HWND_SUB_MENU_COLSET_INPCOLFMT_9300K_ITEM, MApp_ZUI_Adjust_SetColorTemperature , (int)CT_COOL, MApp_ZUI_Adjust_GetColorTemperature},
    {SCENE_MAINMENU_COLOR_L3_PRESET_MODE, HWND_SUB_MENU_COLSET_INPCOLFMT_CUSCOL_ITEM,MApp_ZUI_Adjust_SetColorTemperature  , (int)CT_USER, MApp_ZUI_Adjust_GetColorTemperature},

    {SCENE_MAINMENU_COLOR_L3_SUPER_RESOLUTION, HWND_SUB_MENU_COLSET_SUPERRES_OFF_ITEM, MApp_ZUI_Adjust_SetSuperResolutionLevel , (int)SUPER_RESOLUTION_OFF, MApp_ZUI_Adjust_GetSuperResolutionLevel},
    {SCENE_MAINMENU_COLOR_L3_SUPER_RESOLUTION, HWND_SUB_MENU_COLSET_SUPERRES_HIGH_ITEM, MApp_ZUI_Adjust_SetSuperResolutionLevel  , (int)SUPER_RESOLUTION_HIGH, MApp_ZUI_Adjust_GetSuperResolutionLevel},
    {SCENE_MAINMENU_COLOR_L3_SUPER_RESOLUTION, HWND_SUB_MENU_COLSET_SUPERRES_MIDDLE_ITEM,  MApp_ZUI_Adjust_SetSuperResolutionLevel, (int)SUPER_RESOLUTION_MIDDLE, MApp_ZUI_Adjust_GetSuperResolutionLevel},
    {SCENE_MAINMENU_COLOR_L3_SUPER_RESOLUTION, HWND_SUB_MENU_COLSET_SUPERRES_LOW_ITEM,  MApp_ZUI_Adjust_SetSuperResolutionLevel, (int)SUPER_RESOLUTION_LOW, MApp_ZUI_Adjust_GetSuperResolutionLevel},

    {SCENE_MAINMENU_DISPLAY_L3_ASPECT_RATIO, HWND_SUB_MENU_DISSET_ASPRATIO_FULL_ITEM, MApp_ZUI_Adjust_SetAspectRatioType, (int)eASPECT_FULL, MApp_ZUI_Adjust_GetAspectRatio},
    {SCENE_MAINMENU_DISPLAY_L3_ASPECT_RATIO, HWND_SUB_MENU_DISSET_ASPRATIO_AUTO_ITEM,  MApp_ZUI_Adjust_SetAspectRatioType, (int)eASPECT_NATIVE, MApp_ZUI_Adjust_GetAspectRatio},
    {SCENE_MAINMENU_DISPLAY_L3_ASPECT_RATIO, HWND_SUB_MENU_DISSET_ASPRATIO_5BY4_ITEM,  MApp_ZUI_Adjust_SetAspectRatioType, (int)eASPECT_5_4, MApp_ZUI_Adjust_GetAspectRatio},
    {SCENE_MAINMENU_DISPLAY_L3_ASPECT_RATIO, HWND_SUB_MENU_DISSET_ASPRATIO_4BY3_ITEM,  MApp_ZUI_Adjust_SetAspectRatioType, (int)eASPECT_4_3, MApp_ZUI_Adjust_GetAspectRatio},
    {SCENE_MAINMENU_DISPLAY_L3_ASPECT_RATIO, HWND_SUB_MENU_DISSET_ASPRATIO_16BY9_ITEM,  MApp_ZUI_Adjust_SetAspectRatioType, (int)eASPECT_16_9, MApp_ZUI_Adjust_GetAspectRatio},
    {SCENE_MAINMENU_DISPLAY_L3_ASPECT_RATIO, HWND_SUB_MENU_DISSET_ASPRATIO_ONEBYONE_ITEM,  MApp_ZUI_Adjust_SetAspectRatioType, (int)eASPECT_1_1, MApp_ZUI_Adjust_GetAspectRatio},
  
    {SCENE_MAINMENU_DISPLAY_L3_RESPONSE_TIME, HWND_SUB_MENU_DISSET_RESPONSETIME_NORMAL_ITEM, MApp_ZUI_Adjust_SetResponseTimeMode, (int)_ON_, GetResponseTimeValue},
    {SCENE_MAINMENU_DISPLAY_L3_RESPONSE_TIME, HWND_SUB_MENU_DISSET_RESPONSETIME_OD_ITEM, MApp_ZUI_Adjust_SetResponseTimeMode, (int)_OFF_, GetResponseTimeValue},

    {SCENE_MAINMENU_MULTIWINDOW_L3_WIN_MODE, HWND_SUB_MENU_MULTIWIN_WINMODES_OFF_ITEM, MApp_ZUI_Adjust_SetMultiWindowMode , (int)eMULTIWIN_OFF, MApp_ZUI_Adjust_GetMultiWinMode},
    {SCENE_MAINMENU_MULTIWINDOW_L3_WIN_MODE, HWND_SUB_MENU_MULTIWIN_WINMODES_PIPMODE_ITEM,  MApp_ZUI_Adjust_SetMultiWindowMode, (int)eMULTIWIN_2_PIP, MApp_ZUI_Adjust_GetMultiWinMode},
    {SCENE_MAINMENU_MULTIWINDOW_L3_WIN_MODE, HWND_SUB_MENU_MULTIWIN_WINMODES_PBPMODE1_ITEM,  MApp_ZUI_Adjust_SetMultiWindowMode, (int)eMULTIWIN_2_POP1, MApp_ZUI_Adjust_GetMultiWinMode},
    {SCENE_MAINMENU_MULTIWINDOW_L3_WIN_MODE, HWND_SUB_MENU_MULTIWIN_WINMODES_PBPMODE2_ITEM,  MApp_ZUI_Adjust_SetMultiWindowMode, (int)eMULTIWIN_2_POP2, MApp_ZUI_Adjust_GetMultiWinMode},
    {SCENE_MAINMENU_MULTIWINDOW_L3_WIN_MODE, HWND_SUB_MENU_MULTIWIN_WINMODES_TRIPLEMODE_ITEM,  MApp_ZUI_Adjust_SetMultiWindowMode, (int)eMULTIWIN_3, MApp_ZUI_Adjust_GetMultiWinMode},
    {SCENE_MAINMENU_MULTIWINDOW_L3_WIN_MODE, HWND_SUB_MENU_MULTIWIN_WINMODES_QUADMODE_ITEM, MApp_ZUI_Adjust_SetMultiWindowMode , (int)eMULTIWIN_4, MApp_ZUI_Adjust_GetMultiWinMode},

    {SCENE_MAINMENU_MULTIWINDOW_L3_WIN_SEL, HWND_SUB_MENU_MULTIWIN_WINSEL_WIN1_ITEM,  MApp_ZUI_Adjust_SetMultiWindowSelect, (int)DISPLAY_WIN1, MApp_ZUI_Adjust_GetWinSel},
    {SCENE_MAINMENU_MULTIWINDOW_L3_WIN_SEL, HWND_SUB_MENU_MULTIWIN_WINSEL_WIN2_ITEM,  MApp_ZUI_Adjust_SetMultiWindowSelect, (int)DISPLAY_WIN2, MApp_ZUI_Adjust_GetWinSel},
    {SCENE_MAINMENU_MULTIWINDOW_L3_WIN_SEL, HWND_SUB_MENU_MULTIWIN_WINSEL_WIN3_ITEM,  MApp_ZUI_Adjust_SetMultiWindowSelect, (int)DISPLAY_WIN3, MApp_ZUI_Adjust_GetWinSel},
    {SCENE_MAINMENU_MULTIWINDOW_L3_WIN_SEL, HWND_SUB_MENU_MULTIWIN_WINSEL_WIN4_ITEM,  MApp_ZUI_Adjust_SetMultiWindowSelect, (int)DISPLAY_WIN4, MApp_ZUI_Adjust_GetWinSel},
        
    {SCENE_MAINMENU_MULTIWINDOW_L3_PIP_SIZE, HWND_SUB_MENU_MULTIWIN_PIPSIZE_SMALL_ITEM,  MApp_ZUI_Adjust_SetPIPSize, (int)ePIPSIZE_SMALL, MApp_ZUI_Adjust_GetPIPSize},
    {SCENE_MAINMENU_MULTIWINDOW_L3_PIP_SIZE, HWND_SUB_MENU_MULTIWIN_PIPSIZE_MEDIUM_ITEM,  MApp_ZUI_Adjust_SetPIPSize, (int)ePIPSIZE_MIDDLE, MApp_ZUI_Adjust_GetPIPSize},
    {SCENE_MAINMENU_MULTIWINDOW_L3_PIP_SIZE, HWND_SUB_MENU_MULTIWIN_PIPSIZE_LARGE_ITEM,  MApp_ZUI_Adjust_SetPIPSize, (int)ePIPSIZE_LARGE, MApp_ZUI_Adjust_GetPIPSize},
    {SCENE_MAINMENU_MULTIWINDOW_L3_PIP_SIZE, HWND_SUB_MENU_MULTIWIN_PIPSIZE_HUGE_ITEM,  MApp_ZUI_Adjust_SetPIPSize, (int)ePIPSIZE_HUGE, MApp_ZUI_Adjust_GetPIPSize},

    {SCENE_MAINMENU_OTHER_L3_LANGUAGE, HWND_SUB_MENU_OTHERSET_LAN_ENG_ITEM,  MApp_ZUI_Adjust_SetLanguage, (int)LANGUAGE_ENGLISH, MApp_ZUI_Adjust_GetLanguage},
    {SCENE_MAINMENU_OTHER_L3_LANGUAGE, HWND_SUB_MENU_OTHERSET_LAN_SIM_ITEM,  MApp_ZUI_Adjust_SetLanguage, (int)LANGUAGE_CHINESE, MApp_ZUI_Adjust_GetLanguage},

    //single select
    {SCENE_MAINMENU_AUTOADJUST, HWND_MAIN_MENU_AUTO_ADJUST_ITEM,  MApp_ZUI_Adjust_AutoAdjust, (int)0, (void*)NULL},
    {SCENE_MAINMENU_OTHER_L2, HWND_SUB_MENU_OTHERSET_MENUROTA_ITEM,  MApp_ZUI_Adjust_MenuRotate, (int)0, (void*)NULL},
    {SCENE_MAINMENU_OTHER_L2, HWND_SUB_MENU_OTHERSET_FACRST_ITEM,  MApp_ZUI_Adjust_FactoryReset, (int)0, (void*)NULL},
};

///////////////////////////////////////////////////////////////////////////////
///  global static struct SceneSaveSettingHandler
///  it allows user to customized eeprom save function, 
///
///  this table dependents SceneReturnSaveSettingMap table.
///
/// @struct member        
///        Setting Save Function Pointer,
///
///////////////////////////////////////////////////////////////////////////////
static SCENE_SAVE_SETTING_HANDLER SceneSaveSettingHandler = { SaveMonitorSetting };

///////////////////////////////////////////////////////////////////////////////
///  global static struct SceneReturnSaveSettingMap
///  It allows user to customized eeprom save timing.
///  User need to fill SCENE ID in this table and the scenes of table will save setting when BM close anyone of the scenes.
///
///  this table dependents SceneSaveSettingHandler table.
///
/// @struct member        
///        Setting Save Function Pointer,
///
///////////////////////////////////////////////////////////////////////////////
static SCENE_SAVE_SETTING_MAP SceneReturnSaveSettingMap[SCENE_TYPE_MAX] = {
    {SCENE_MAINMENU_BRIGHTNESS},
    {SCENE_MAINMENU_AUTOADJUST},
    {SCENE_MAINMENU_INPUTSOURCE},
    {SCENE_MAINMENU_COLOR},
    {SCENE_MAINMENU_DISPLAY},
    {SCENE_MAINMENU_MULTIWINDOW},
    {SCENE_MAINMENU_OTHER},
};

///////////////////////////////////////////////////////////////////////////////
///  global static struct SceneGetValueMap
///         This table is for get dynamic value (MApp_ZUI_ACT_GetDynamicValue).
///         BM will get value directly.
///
///         Related Proc: EN_ZUI_DYNAMICTEXT_WINPROC/EN_ZUI_DYNAMICCOLORTEXT_WINPROC/EN_ZUI_RECTPROGRESSBAR_2_WINPROC/EN_ZUI_BALLPROGRESSBAR_WINPROC
///         Related Map: SceneGetValueMap,SceneGetValue1CondMap
///         
/// @struct member        
///        OSD ID,    HWND,   Value Getter Function Pointer,
///
///////////////////////////////////////////////////////////////////////////////
static SCENE_GET_VALUE_HANDLER_MAP SceneGetValueMap[] = {
    {E_OSD_MAIN_MENU, HWND_SUB_MENU_BRIADJ_VALUE_LIST, GetBrightnessValue},
    {E_OSD_MAIN_MENU, HWND_SUB_MENU_CONADJ_VALUE_LIST, GetContrastValue},
    {E_OSD_MAIN_MENU, HWND_SUB_MENU_HP_VALUE_LIST, GetHPositionValue},
    {E_OSD_MAIN_MENU, HWND_SUB_MENU_VP_VALUE_LIST, GetVPositionValue},
    {E_OSD_MAIN_MENU, HWND_SUB_MENU_SHP_VALUE_LIST, GetSharpnessValue},
    {E_OSD_MAIN_MENU, HWND_SUB_MENU_PIXCLK_VALUE_LIST, GetClockValue},
    {E_OSD_MAIN_MENU, HWND_SUB_MENU_PHS_VALUE_LIST, GetPhaseValue},
#if ENABLE_MULTI_WINDOW_SETTINGS_MENU
    {E_OSD_MAIN_MENU, HWND_SUB_MENU_MULTIWIN_PIPHP_VALUE_LIST, GetPIPHPositionValue},
    {E_OSD_MAIN_MENU, HWND_SUB_MENU_MULTIWIN_PIPVP_VALUE_LIST, GetPIPVPositionValue},
#endif
    {E_OSD_MAIN_MENU, HWND_SUB_MENU_HUE_VALUE_LIST, MApp_ZUI_Custom_GetHueOrRedColor},
    {E_OSD_MAIN_MENU, HWND_SUB_MENU_SAT_VALUE_LIST, MApp_ZUI_Custom_GetSaturationOrGreenColor},
    {E_OSD_MAIN_MENU, HWND_SUB_MENU_MENUTPR_VALUE_LIST, GetTransparencyValue},
    {E_OSD_MAIN_MENU, HWND_SUB_MENU_CUSTOM_B_VALUE_LIST, GetUserBlueColorValue},

    //for text
    {E_OSD_MAIN_MENU,  HWND_SUB_MENU_DISSET_RESPONSETIME_ITEM_VALUE_TEXT, GetResponseTimeValue},

#if ENABLE_MULTI_WINDOW_SETTINGS_MENU
    {E_OSD_MAIN_MENU, HWND_SUB_MENU_MULTIWIN_PIPHP_ITEM_VALUE_TEXT , GetPIPHPositionValue},
    {E_OSD_MAIN_MENU, HWND_SUB_MENU_MULTIWIN_PIPVP_ITEM_VALUE_TEXT , GetPIPVPositionValue},
    {E_OSD_MAIN_MENU, HWND_SUB_MENU_MULTIWIN_PIPHP_VALUE_TEXT , GetPIPHPositionValue},
    {E_OSD_MAIN_MENU, HWND_SUB_MENU_MULTIWIN_PIPVP_VALUE_ITEM_TEXT , GetPIPVPositionValue},
#endif
    {E_OSD_MAIN_MENU ,HWND_SUB_MENU_MULTIWIN_WINMODES_ITEM_VALUE_TEXT , MApp_ZUI_Adjust_GetMultiWinMode},
    {E_OSD_MAIN_MENU ,HWND_SUB_MENU_CON_VALUE_TEXT , GetContrastValue},
    {E_OSD_MAIN_MENU ,HWND_SUB_MENU_CONADJ_VALUE_TEXT , GetContrastValue},
    {E_OSD_MAIN_MENU ,HWND_SUB_MENU_BRI_VALUE_TEXT , GetBrightnessValue},
    {E_OSD_MAIN_MENU ,HWND_SUB_MENU_BRIADJ_VALUE_TEXT , GetBrightnessValue},
    {E_OSD_MAIN_MENU ,HWND_SUB_MENU_DISSET_HP_ITEM_VALUE_TEXT , GetHPositionValue},
    {E_OSD_MAIN_MENU ,HWND_SUB_MENU_HP_VALUE_TEXT , GetHPositionValue},
    {E_OSD_MAIN_MENU ,HWND_SUB_MENU_DISSET_VP_ITEM_VALUE_TEXT , GetVPositionValue},
    {E_OSD_MAIN_MENU ,HWND_SUB_MENU_VP_VALUE_TEXT , GetVPositionValue},
    {E_OSD_MAIN_MENU ,HWND_SUB_MENU_DISSET_SHP_ITEM_VALUE_TEXT , GetSharpnessValue},
    {E_OSD_MAIN_MENU ,HWND_SUB_MENU_SHP_VALUE_TEXT , GetSharpnessValue},
    {E_OSD_MAIN_MENU ,HWND_SUB_MENU_DISSET_PIXCLK_ITEM_VALUE_TEXT , GetClockValue},
    {E_OSD_MAIN_MENU ,HWND_SUB_MENU_PIXCLK_VALUE_TEXT , GetClockValue},
    {E_OSD_MAIN_MENU ,HWND_SUB_MENU_DISSET_PHS_ITEM_VALUE_TEXT , GetPhaseValue},
    {E_OSD_MAIN_MENU ,HWND_SUB_MENU_PHS_VALUE_TEXT , GetPhaseValue},
    {E_OSD_MAIN_MENU ,HWND_SUB_MENU_MENUTPR_VALUE_TEXT , GetTransparencyValue},
    {E_OSD_MAIN_MENU ,HWND_SUB_MENU_OTHERSET_MENUTPC_VALUE_ITEM_TEXT , GetTransparencyValue},
    {E_OSD_MAIN_MENU ,HWND_SUB_MENU_ITEM5_VALUE_TEXT , GetUserBlueColorValue},
    {E_OSD_MAIN_MENU ,HWND_SUB_MENU_CUSTOM_B_VALUE_ITEM_TEXT , GetUserBlueColorValue},
    {E_OSD_MAIN_MENU ,HWND_SUB_MENU_CON_VALUE_TEXT , GetContrastValue},
    {E_OSD_MAIN_MENU ,HWND_SUB_MENU_CON_VALUE_TEXT , GetContrastValue},
    {E_OSD_MAIN_MENU,HWND_SUB_MENU_COLSET_SAT_VALUE_ITEM_TEXT, MApp_ZUI_Custom_GetSaturationOrGreenColor},
    {E_OSD_MAIN_MENU,HWND_SUB_MENU_SAT_VALUE_TEXT, MApp_ZUI_Custom_GetSaturationOrGreenColor},
    {E_OSD_MAIN_MENU,HWND_SUB_MENU_COLSET_HUE_VALUE_ITEM_TEXT, MApp_ZUI_Custom_GetHueOrRedColor},
    {E_OSD_MAIN_MENU,HWND_SUB_MENU_HUE_VALUE_TEXT, MApp_ZUI_Custom_GetHueOrRedColor},

#if ENABLE_DCR
    {E_OSD_MAIN_MENU,HWND_SUB_MENU_DCR_VALUE_TEXT, GetDCREnableRealValue},
    {E_OSD_MAIN_MENU,HWND_SUB_MENU_PAGE_BRI_ITEM_VALUE_LIST, GetDCREnableRealValue},
#endif
    {E_OSD_MAIN_MENU,HWND_SUB_MENU_COLSET_INPCOLFMT_VALUE_ITEM_TEXT, GetInputColorFormatRealValue},
    {E_OSD_MAIN_MENU,HWND_SUB_MENU_COLSET_SUPERRES_VALUE_ITEM_TEXT, MApp_ZUI_Adjust_GetSuperResolutionLevel},
    {E_OSD_MAIN_MENU,HWND_SUB_MENU_COLSET_PREMOD_VALUE_ITEM_TEXT, MApp_ZUI_Adjust_GetColorTemperature},
    {E_OSD_MAIN_MENU,HWND_SUB_MENU_DISSET_ASPRAT_ITEM_VALUE_TEXT, MApp_ZUI_Adjust_GetAspectRatio},
    {E_OSD_MAIN_MENU,HWND_SUB_MENU_OTHERSET_LAN_VALUE_ITEM_TEXT, MApp_ZUI_Adjust_GetLanguage},
    {E_OSD_MAIN_MENU,HWND_SUB_MENU_MULTIWIN_PIPSIZE_ITEM_VALUE_TEXT, MApp_ZUI_Adjust_GetPIPSize},
    {E_OSD_MAIN_MENU,HWND_SUB_MENU_MULTIWIN_WINSEL_ITEM_VALUE_TEXT, MApp_ZUI_Adjust_GetWinSel},
    {E_OSD_MAIN_MENU,HWND_SUB_MENU_MULTIWIN_WINMODES_ITEM_VALUE_TEXT, MApp_ZUI_Adjust_GetMultiWinMode},
    {E_OSD_MAIN_MENU,HWND_SUB_MENU_COLSET_HUE_ITEM_TEXT, MApp_ZUI_Adjust_GetColorTemperature},
    {E_OSD_MAIN_MENU,HWND_SUB_MENU_COLSET_SAT_ITEM_TEXT, MApp_ZUI_Adjust_GetColorTemperature},
    {E_OSD_MAIN_MENU,HWND_SUB_MENU_ITEM5_TEXT, MApp_ZUI_Adjust_GetColorTemperature},
    {E_OSD_MAIN_MENU,HWND_SUB_MENU_COLSET_PREMOD_VALUE_ITEM_TEXT, MApp_ZUI_Adjust_GetColorTemperature},
    {E_OSD_MAIN_MENU,HWND_SUB_MENU_COLSET_PREMOD_VALUE_ITEM_TEXT, MApp_ZUI_Adjust_GetColorTemperature},
    {E_OSD_MAIN_MENU,HWND_MAIN_MENU_TITLE_TEXT,MApp_ZUI_Adjust_GetCurrentInput},

};

///////////////////////////////////////////////////////////////////////////////
///  global static struct SceneGetValue1CondMap
///         This table is for get dynamic value (MApp_ZUI_ACT_GetDynamicValue) with one condition.
///         BM will get value until match condition.
///         
///         Related Proc: EN_ZUI_DYNAMICTEXT_WINPROC/EN_ZUI_DYNAMICCOLORTEXT_WINPROC/EN_ZUI_RECTPROGRESSBAR_2_WINPROC/EN_ZUI_BALLPROGRESSBAR_WINPROC
///         Related Map: SceneGetValueMap,SceneGetValue1CondMap
///
/// @struct member        
///         OSD ID,    
///         HWND,   
///         Condition Getter Function Pointer,
///         Compare operator
///         Compare parameter
///         Value Getter Function Pointer,
///
///////////////////////////////////////////////////////////////////////////////
static SCENE_GET_VALUE_1CONDITION_HANDLER_MAP SceneGetValue1CondMap[] = {
};

///////////////////////////////////////////////////////////////////////////////
///  global static struct SceneGetTextMappingMap
///         This table is for get dynamic text (MApp_ZUI_ACT_GetDynamicText).
///         BM will get text from text mapping table, it needs match the value of map and return the text id of map.
///         Related Proc: EN_ZUI_DYNAMICTEXT_WINPROC/EN_ZUI_DYNAMICCOLORTEXT_WINPROC
///         
/// @struct member        
///        Scene Type, HWND,   Mapping Text Map,  Index of DefaultText (TEXTMAP_DEFAULT_TEXT_DISABLE is disable option) 
///////////////////////////////////////////////////////////////////////////////
static SCENE_GET_MAPPING_TEXT_MAP SceneGetTextMappingMap[] = {
    {E_OSD_MAIN_MENU, HWND_SUB_MENU_DISSET_RESPONSETIME_ITEM_VALUE_TEXT, ResponseTimeTextMap, TEXTMAP_DEFAULT_TEXT_DISABLE},
    {E_OSD_MAIN_MENU ,HWND_SUB_MENU_MULTIWIN_WINMODES_ITEM_VALUE_TEXT , MultiWinTextMap, TEXTMAP_DEFAULT_TEXT_DISABLE},

#if ENABLE_DCR
    {E_OSD_MAIN_MENU,HWND_SUB_MENU_DCR_VALUE_TEXT, OnOffTextMap, TEXTMAP_DEFAULT_TEXT_DISABLE},
    {E_OSD_MAIN_MENU,HWND_SUB_MENU_PAGE_BRI_ITEM_VALUE_LIST, OnOffTextMap, TEXTMAP_DEFAULT_TEXT_DISABLE},
#endif
    {E_OSD_MAIN_MENU,HWND_SUB_MENU_COLSET_INPCOLFMT_VALUE_ITEM_TEXT, InputColorFormatTextMap, TEXTMAP_DEFAULT_TEXT_DISABLE},
    {E_OSD_MAIN_MENU,HWND_SUB_MENU_COLSET_SUPERRES_VALUE_ITEM_TEXT, SuperResolutionTextMap, TEXTMAP_DEFAULT_TEXT_DISABLE},
    {E_OSD_MAIN_MENU,HWND_SUB_MENU_COLSET_PREMOD_VALUE_ITEM_TEXT, ColorTemperatureTextMap, TEXTMAP_DEFAULT_TEXT_DISABLE},
    {E_OSD_MAIN_MENU,HWND_SUB_MENU_DISSET_ASPRAT_ITEM_VALUE_TEXT, AspectRatioTextMap, TEXTMAP_DEFAULT_TEXT_DISABLE},
    {E_OSD_MAIN_MENU,HWND_SUB_MENU_OTHERSET_LAN_VALUE_ITEM_TEXT, LanguageTextMap, TEXTMAP_DEFAULT_TEXT_DISABLE},
    {E_OSD_MAIN_MENU,HWND_SUB_MENU_MULTIWIN_PIPSIZE_ITEM_VALUE_TEXT, MultiWinSizeTextMap, TEXTMAP_DEFAULT_TEXT_DISABLE},
    {E_OSD_MAIN_MENU,HWND_SUB_MENU_MULTIWIN_WINSEL_ITEM_VALUE_TEXT, WinSelTextMap, TEXTMAP_DEFAULT_TEXT_DISABLE},
    {E_OSD_MAIN_MENU,HWND_SUB_MENU_MULTIWIN_WINMODES_ITEM_VALUE_TEXT, MultiWinModeTextMap, TEXTMAP_DEFAULT_TEXT_DISABLE},
    {E_OSD_MAIN_MENU,HWND_SUB_MENU_COLSET_HUE_ITEM_TEXT, HueTextMap, 1},
    {E_OSD_MAIN_MENU,HWND_SUB_MENU_COLSET_SAT_ITEM_TEXT, SaturationTextMap, 1},
    {E_OSD_MAIN_MENU,HWND_SUB_MENU_ITEM5_TEXT, Item5TextMap, TEXTMAP_DEFAULT_TEXT_DISABLE},
    {E_OSD_MAIN_MENU,HWND_MAIN_MENU_TITLE_TEXT,CurrentInputTextMap, TEXTMAP_DEFAULT_TEXT_DISABLE},
};

///////////////////////////////////////////////////////////////////////////////
///  global static struct HwndGetTextMap
///         This table is for get dynamic text (MApp_ZUI_ACT_GetDynamicText).
///         BM will return the text ID directly.
///         In general, this table is for "Dynamic Color Text Proc"
///         Related Proc: EN_ZUI_DYNAMICTEXT_WINPROC/EN_ZUI_DYNAMICCOLORTEXT_WINPROC
///         
/// @struct member        
///        OSD ID,   HWND,   Text ID 
///////////////////////////////////////////////////////////////////////////////
static HWND_GET_TEXT_MAP HwndGetTextMap[] = {
    /*
        OSD ID, HWND,   Text ID,
    */
    {E_OSD_MAIN_MENU, HWND_MAIN_MENU_BRI_TEXT, en_str_Bri_Con_Text}, 
    {E_OSD_MAIN_MENU, HWND_MAIN_MENU_INPUT_SOURCE_TEXT, en_str_Input_Source}, 
    {E_OSD_MAIN_MENU, HWND_MAIN_MENU_COLOR_SETTINGS_TEXT, en_str_Color_Settings}, 
    {E_OSD_MAIN_MENU, HWND_MAIN_MENU_DISPLAY_SETTINGS_TEXT, en_str_Display_Settings}, 
    {E_OSD_MAIN_MENU, HWND_MAIN_MENU_MULTI_WIN_SETTINGS_TEXT, en_str_Multi_Window_Settings}, 
    {E_OSD_MAIN_MENU, HWND_MAIN_MENU_OTHER_SETTINGS_TEXT, en_str_Other_Settings}, 

    {E_OSD_MAIN_MENU, HWND_SUB_MENU_BRI_TEXT, en_str_Brightness}, 
    {E_OSD_MAIN_MENU, HWND_SUB_MENU_CON_TEXT, en_str_Contrast}, 
    {E_OSD_MAIN_MENU, HWND_SUB_MENU_DYNAMIC_CON_TEXT, en_str_Dynamic_Contrast}, 
    
    {E_OSD_MAIN_MENU, HWND_SUB_MENU_MULTIWIN_WINMODES_ITEM_TEXT, en_str_Multi_Window_Modes}, 
    {E_OSD_MAIN_MENU, HWND_SUB_MENU_MULTIWIN_WINSEL_ITEM_TEXT, en_str_Window_Select}, 
    {E_OSD_MAIN_MENU, HWND_SUB_MENU_MULTIWIN_PIPSIZE_ITEM_TEXT, en_str_PIP_Size}, 
    {E_OSD_MAIN_MENU, HWND_SUB_MENU_MULTIWIN_PIPHP_ITEM_TEXT, en_str_PIP_Horizontal_Position}, 
    {E_OSD_MAIN_MENU, HWND_SUB_MENU_MULTIWIN_PIPVP_ITEM_TEXT, en_str_PIP_Vertical_Position}, 
        
    {E_OSD_MAIN_MENU, HWND_SUB_MENU_COLSET_INPCOLFMT_ITEM_TEXT, en_str_Input_Color_Format}, 
    {E_OSD_MAIN_MENU, HWND_SUB_MENU_COLSET_PREMOD_ITEM_TEXT, en_str_Preset_Modes}, 
    {E_OSD_MAIN_MENU, HWND_SUB_MENU_COLSET_SUPERRES_ITEM_TEXT, en_str_Super_Resolution}, 
        
    {E_OSD_MAIN_MENU, HWND_SUB_MENU_DISSET_ASPRAT_ITEM_TEXT, en_str_Aspect_Ratio}, 
    {E_OSD_MAIN_MENU, HWND_SUB_MENU_DISSET_HP_ITEM_TEXT, en_str_Horizontal_Position}, 
    {E_OSD_MAIN_MENU, HWND_SUB_MENU_DISSET_VP_ITEM_TEXT, en_str_Vertical_Position}, 
    {E_OSD_MAIN_MENU, HWND_SUB_MENU_DISSET_SHP_ITEM_TEXT, en_str_Sharpness}, 
    {E_OSD_MAIN_MENU, HWND_SUB_MENU_DISSET_PIXCLK_ITEM_TEXT, en_str_Pixel_Clock}, 
    {E_OSD_MAIN_MENU, HWND_SUB_MENU_DISSET_PHS_ITEM_TEXT, en_str_Phase}, 
    {E_OSD_MAIN_MENU, HWND_SUB_MENU_DISSET_RESPONSETIME_ITEM_TEXT, en_str_Response_Time}, 

    {E_OSD_MAIN_MENU, HWND_SUB_MENU_OTHERSET_LANGUAGE_ITEM_TEXT, en_str_Language},
    {E_OSD_MAIN_MENU, HWND_SUB_MENU_OTHERSET_TPC_ITEM_TEXT, en_str_Menu_Transparency}, 

#if (!ENABLE_DCR)
    {E_OSD_MAIN_MENU, HWND_SUB_MENU_DCR_VALUE_TEXT, en_str_Off},
    {E_OSD_MAIN_MENU, HWND_SUB_MENU_PAGE_BRI_ITEM_VALUE_LIST, en_str_Off},
#endif
};


///////////////////////////////////////////////////////////////////////////////
///  global static struct SceneGetBitmapMap
///         This table is for get bitmap index (MApp_ZUI_ACT_GetDynamicBitmap) with one condition.
///         BM will compare the getter function with parameter(compareParameter) , 
///         If match, it will return nBitmapID
///         If not , it will return nElseBitmapID
///         Related Proc: EN_ZUI_DYNAMICBITMAP_WINPROC
///         
/// @struct member        
///        osdID , hwnd, focusHwnd, drawStyle, get_value_function_ptr, compareParameter, nBitmapID, nElseBitmapID
///
/**********************Logic***********************
if( currentDrawStyle != drawStyle && drawStyle != DRAW_STATUS_ALL))
    break;

if( ( (focusHwnd != 0 && focusHwnd == MApp_ZUI_API_GetFocus() ) || focusHwnd == 0)
   && (get_value_function_ptr() == compareParameter))
{
    set nBitmapID;
}
else
{
    set nElseBitmapID;
}
************************************************/
///////////////////////////////////////////////////////////////////////////////
static SCENE_GET_BITMAP_MAP SceneGetBitmapMap[] = {
    {E_OSD_MAIN_MENU, HWND_SUB_MENU_AUTO_SRC_ITEM, HWND_SUB_MENU_AUTO_SRC_ITEM, DRAW_ALL_STATUS, MApp_ZUI_Adjust_GetInputPriority, (WORD)INPUT_PRIORITY_AUTO, E_BMP_ITEM_FOCUS_DOT, E_BMP_ITEM_FOCUS},
    {E_OSD_MAIN_MENU, HWND_SUB_MENU_VGA_ITEM, HWND_SUB_MENU_VGA_ITEM, DRAW_ALL_STATUS,MApp_ZUI_Adjust_GetInputPriority, (WORD)INPUT_PRIORITY_VGA, E_BMP_ITEM_FOCUS_DOT, E_BMP_ITEM_FOCUS},
    {E_OSD_MAIN_MENU, HWND_SUB_MENU_DVI_ITEM, HWND_SUB_MENU_DVI_ITEM,DRAW_ALL_STATUS, MApp_ZUI_Adjust_GetInputPriority, (WORD)INPUT_PRIORITY_DVI, E_BMP_ITEM_FOCUS_DOT, E_BMP_ITEM_FOCUS},
    {E_OSD_MAIN_MENU, HWND_SUB_MENU_HDMI1_ITEM, HWND_SUB_MENU_HDMI1_ITEM,DRAW_ALL_STATUS, MApp_ZUI_Adjust_GetInputPriority, (WORD)INPUT_PRIORITY_HDMI, E_BMP_ITEM_FOCUS_DOT, E_BMP_ITEM_FOCUS},
    {E_OSD_MAIN_MENU, HWND_SUB_MENU_HDMI2_ITEM, HWND_SUB_MENU_HDMI2_ITEM,DRAW_ALL_STATUS, MApp_ZUI_Adjust_GetInputPriority, (WORD)INPUT_PRIORITY_HDMI2ND, E_BMP_ITEM_FOCUS_DOT, E_BMP_ITEM_FOCUS},
    {E_OSD_MAIN_MENU, HWND_SUB_MENU_DP1_ITEM, HWND_SUB_MENU_DP1_ITEM,DRAW_ALL_STATUS, MApp_ZUI_Adjust_GetInputPriority, (WORD)INPUT_PRIORITY_DP, E_BMP_ITEM_FOCUS_DOT, E_BMP_ITEM_FOCUS},
    {E_OSD_MAIN_MENU, HWND_SUB_MENU_DP2_ITEM, HWND_SUB_MENU_DP2_ITEM,DRAW_ALL_STATUS, MApp_ZUI_Adjust_GetInputPriority, (WORD)INPUT_PRIORITY_DP2ND, E_BMP_ITEM_FOCUS_DOT, E_BMP_ITEM_FOCUS},
    {E_OSD_MAIN_MENU, HWND_SUB_MENU_AUTO_SRC_ICON, HWND_NULL ,DRAW_ALL_STATUS, MApp_ZUI_Adjust_GetInputPriority, (WORD)INPUT_PRIORITY_AUTO, E_BMP_CHECK_MARK_TICK_2C_20130423, E_ZUI_BMP_MAX},
    {E_OSD_MAIN_MENU, HWND_SUB_MENU_VGA_SRC_ICON, HWND_NULL ,DRAW_ALL_STATUS, MApp_ZUI_Adjust_GetInputPriority, (WORD)INPUT_PRIORITY_VGA, E_BMP_CHECK_MARK_TICK_2C_20130423, E_ZUI_BMP_MAX},
    {E_OSD_MAIN_MENU, HWND_SUB_MENU_DVI_SRC_ICON, HWND_NULL ,DRAW_ALL_STATUS, MApp_ZUI_Adjust_GetInputPriority, (WORD)INPUT_PRIORITY_DVI, E_BMP_CHECK_MARK_TICK_2C_20130423, E_ZUI_BMP_MAX},
    {E_OSD_MAIN_MENU, HWND_SUB_MENU_HDMI1_SRC_ICON, HWND_NULL ,DRAW_ALL_STATUS, MApp_ZUI_Adjust_GetInputPriority, (WORD)INPUT_PRIORITY_HDMI, E_BMP_CHECK_MARK_TICK_2C_20130423, E_ZUI_BMP_MAX},
    {E_OSD_MAIN_MENU, HWND_SUB_MENU_HDMI2_SRC_ICON, HWND_NULL ,DRAW_ALL_STATUS, MApp_ZUI_Adjust_GetInputPriority, (WORD)INPUT_PRIORITY_HDMI2ND, E_BMP_CHECK_MARK_TICK_2C_20130423, E_ZUI_BMP_MAX},
    {E_OSD_MAIN_MENU, HWND_SUB_MENU_DP1_SRC_ICON, HWND_NULL ,DRAW_ALL_STATUS, MApp_ZUI_Adjust_GetInputPriority, (WORD)INPUT_PRIORITY_DP, E_BMP_CHECK_MARK_TICK_2C_20130423, E_ZUI_BMP_MAX},
    {E_OSD_MAIN_MENU, HWND_SUB_MENU_DP2_SRC_ICON, HWND_NULL ,DRAW_ALL_STATUS, MApp_ZUI_Adjust_GetInputPriority, (WORD)INPUT_PRIORITY_DP2ND, E_BMP_CHECK_MARK_TICK_2C_20130423, E_ZUI_BMP_MAX},
};

///////////////////////////////////////////////////////////////////////////////
///  global static struct SceneGetDynamicListStatus0CondMap
///         This table is for get item status of list (MApp_ZUI_ACT_QueryDynamicListItemStatus)
///         BM will return item status directly. 
///         PS : There is no condition in this map. 
///
///         Related Proc: EN_ZUI_DYNAMICLIST_WINPROC
///         Related Map: SceneGetDynamicListStatus0CondMap, SceneGetDynamicListStatus1CondMap , SceneGetDynamicListStatus2CondMap , SceneGetDynamicListStatus3CondMap
///
/// @struct member        
///        osdID , hwnd, eListState
///
///////////////////////////////////////////////////////////////////////////////
static SCENE_GET_DYNAMICLIST_STATUS_NO_CONDITION_MAP SceneGetDynamicListStatus0CondMap[] = {
    {E_OSD_MAIN_MENU, HWND_SUB_MENU_AUTO_SRC_ITEM, EN_DL_STATE_NORMAL},
    {E_OSD_MAIN_MENU, HWND_SUB_MENU_VGA_ITEM, EN_DL_STATE_NORMAL},
        
#if ENABLE_DVI_INPUT
    {E_OSD_MAIN_MENU, HWND_SUB_MENU_DVI_ITEM, EN_DL_STATE_NORMAL},
#else
    {E_OSD_MAIN_MENU, HWND_SUB_MENU_DVI_ITEM, EN_DL_STATE_HIDDEN},
#endif

#if ENABLE_HDMI_INPUT
    {E_OSD_MAIN_MENU, HWND_SUB_MENU_HDMI1_ITEM, EN_DL_STATE_NORMAL},
#else
    {E_OSD_MAIN_MENU, HWND_SUB_MENU_HDMI1_ITEM, EN_DL_STATE_HIDDEN},
#endif

#if ENABLE_HDMI2ND_INPUT
    {E_OSD_MAIN_MENU, HWND_SUB_MENU_HDMI2_ITEM, EN_DL_STATE_NORMAL},
#else
    {E_OSD_MAIN_MENU, HWND_SUB_MENU_HDMI2_ITEM, EN_DL_STATE_HIDDEN},
#endif

 #if ENABLE_DP_INPUT
     {E_OSD_MAIN_MENU, HWND_SUB_MENU_DP1_ITEM, EN_DL_STATE_NORMAL},
#else
    {E_OSD_MAIN_MENU, HWND_SUB_MENU_DP1_ITEM, EN_DL_STATE_HIDDEN},
#endif

 #if ENABLE_DP2ND_INPUT
      {E_OSD_MAIN_MENU, HWND_SUB_MENU_DP2_ITEM, EN_DL_STATE_NORMAL},
#else
     {E_OSD_MAIN_MENU, HWND_SUB_MENU_DP2_ITEM, EN_DL_STATE_HIDDEN},
#endif

 #if ENABLE_SUPER_RESOLUTION
     {E_OSD_MAIN_MENU, HWND_SUB_MENU_COLSET_SUPERRES_ITEM, EN_DL_STATE_NORMAL},
     {E_OSD_MAIN_MENU, HWND_SUB_MENU_COLSET_SUPERRES_VALUE_ITEM, EN_DL_STATE_NORMAL},
#else
     {E_OSD_MAIN_MENU, HWND_SUB_MENU_COLSET_SUPERRES_ITEM, EN_DL_STATE_DISABLED},
     {E_OSD_MAIN_MENU, HWND_SUB_MENU_COLSET_SUPERRES_VALUE_ITEM, EN_DL_STATE_DISABLED},
#endif

 #if ENABLE_WIDE_MODE
      {E_OSD_MAIN_MENU, HWND_SUB_MENU_DISSET_ASPRAT_ITEM, EN_DL_STATE_NORMAL},
#else
     {E_OSD_MAIN_MENU, HWND_SUB_MENU_DISSET_ASPRAT_ITEM, EN_DL_STATE_DISABLED},
#endif

 #if ENABLE_MULTI_WINDOW_SETTINGS_MENU
      {E_OSD_MAIN_MENU, HWND_MAIN_MENU_MULTI_WIN_SETTINGS_ITEM, EN_DL_STATE_NORMAL},
#else
     {E_OSD_MAIN_MENU, HWND_MAIN_MENU_MULTI_WIN_SETTINGS_ITEM, EN_DL_STATE_DISABLED},
#endif

#if (!ENABLE_DCR)
      {E_OSD_MAIN_MENU, HWND_SUB_MENU_DYNAMIC_CON_ITEM, EN_DL_STATE_DISABLED},
      {E_OSD_MAIN_MENU, HWND_SUB_MENU_DCR_VALUE_ITEM, EN_DL_STATE_DISABLED},
#endif

#if (!ENABLE_MOVIE_GAME_PRESET)
      {E_OSD_MAIN_MENU, HWND_SUB_MENU_COLSET_HUE_VALUE_ITEM, EN_DL_STATE_DISABLED},
      {E_OSD_MAIN_MENU, HWND_SUB_MENU_COLSET_SAT_VALUE_ITEM, EN_DL_STATE_DISABLED},
      {E_OSD_MAIN_MENU, HWND_SUB_MENU_COLSET_HUE_ITEM, EN_DL_STATE_DISABLED},
      {E_OSD_MAIN_MENU, HWND_SUB_MENU_COLSET_SAT_ITEM, EN_DL_STATE_DISABLED},

#endif
};

///////////////////////////////////////////////////////////////////////////////
///  global static struct SceneGetDynamicListStatus1CondMap
///         This table is for get item status of list (MApp_ZUI_ACT_QueryDynamicListItemStatus) with one condition.
///             This feature is the same with SceneGetDynamicListStatus0CondMap, it just only add one condition.
///             If specified condition is true, BM will return eListState. 
///             If not, BM will return eElseListState.
///
///         Related Proc: EN_ZUI_DYNAMICLIST_WINPROC
///         Related Map: SceneGetDynamicListStatus0CondMap, SceneGetDynamicListStatus1CondMap , SceneGetDynamicListStatus2CondMap , SceneGetDynamicListStatus3CondMap
///
/// @struct member        
///         osdID : OSD ID
///         hwnd: ZUI Window ID 
///         get_value_function_ptr (First condition set): getter function for compare value, this value is first operand.  
///         compOperator (First condition set) : compare operator for compare value  (First condition set)
///         compParameter (First condition set): second operand
///         eListState : if specified condition is true, BM will return this.
///         eElseListState : if specified condition is false, BM will return this.
///
///////////////////////////////////////////////////////////////////////////////
static SCENE_GET_DYNAMICLIST_STATUS_1CONDITION_MAP SceneGetDynamicListStatus1CondMap[] = {
     {E_OSD_MAIN_MENU, HWND_SUB_MENU_ITEM5, MApp_ZUI_Adjust_GetColorTemperature, E_COMP_OP_EQUAL, CT_USER, EN_DL_STATE_NORMAL, EN_DL_STATE_DISABLED},
     {E_OSD_MAIN_MENU, HWND_SUB_MENU_ITEM5_VALUE, MApp_ZUI_Adjust_GetColorTemperature, E_COMP_OP_EQUAL, CT_USER, EN_DL_STATE_NORMAL, EN_DL_STATE_DISABLED},
     {E_OSD_MAIN_MENU, HWND_MAIN_MENU_AUTO_ADJUST_ITEM, MApp_ZUI_Adjust_GetUserPrefInputType, E_COMP_OP_EQUAL, INPUT_VGA, EN_DL_STATE_NORMAL, EN_DL_STATE_DISABLED},
     {E_OSD_MAIN_MENU, HWND_SUB_MENU_MULTIWIN_WINSEL_ITEM_VALUE, MApp_ZUI_Adjust_GetMultiWinMode, E_COMP_OP_EQUAL, eMULTIWIN_OFF, EN_DL_STATE_DISABLED, EN_DL_STATE_NORMAL},
     {E_OSD_MAIN_MENU, HWND_SUB_MENU_MULTIWIN_WINSEL_ITEM, MApp_ZUI_Adjust_GetMultiWinMode, E_COMP_OP_EQUAL, eMULTIWIN_OFF, EN_DL_STATE_DISABLED, EN_DL_STATE_NORMAL},
     {E_OSD_MAIN_MENU, HWND_SUB_MENU_MULTIWIN_PIPSIZE_ITEM, MApp_ZUI_Adjust_GetMultiWinMode, E_COMP_OP_EQUAL, eMULTIWIN_2_PIP, EN_DL_STATE_NORMAL, EN_DL_STATE_DISABLED},
     {E_OSD_MAIN_MENU, HWND_SUB_MENU_MULTIWIN_PIPHP_ITEM, MApp_ZUI_Adjust_GetMultiWinMode, E_COMP_OP_EQUAL, eMULTIWIN_2_PIP, EN_DL_STATE_NORMAL, EN_DL_STATE_DISABLED},
     {E_OSD_MAIN_MENU, HWND_SUB_MENU_MULTIWIN_PIPVP_ITEM, MApp_ZUI_Adjust_GetMultiWinMode, E_COMP_OP_EQUAL, eMULTIWIN_2_PIP, EN_DL_STATE_NORMAL, EN_DL_STATE_DISABLED},
     {E_OSD_MAIN_MENU, HWND_SUB_MENU_MULTIWIN_PIPSIZE_ITEM_VALUE, MApp_ZUI_Adjust_GetMultiWinMode, E_COMP_OP_EQUAL, eMULTIWIN_2_PIP, EN_DL_STATE_NORMAL, EN_DL_STATE_DISABLED},
     {E_OSD_MAIN_MENU, HWND_SUB_MENU_MULTIWIN_PIPHP_ITEM_VALUE, MApp_ZUI_Adjust_GetMultiWinMode, E_COMP_OP_EQUAL, eMULTIWIN_2_PIP, EN_DL_STATE_NORMAL, EN_DL_STATE_DISABLED},
     {E_OSD_MAIN_MENU, HWND_SUB_MENU_MULTIWIN_PIPVP_ITEM_VALUE, MApp_ZUI_Adjust_GetMultiWinMode, E_COMP_OP_EQUAL, eMULTIWIN_2_PIP, EN_DL_STATE_NORMAL, EN_DL_STATE_DISABLED},
     {E_OSD_MAIN_MENU, HWND_SUB_MENU_DISSET_ASPRATIO_FULL_ITEM, MApp_ZUI_Adjust_GetMultiWinMode, E_COMP_OP_EQUAL, eMULTIWIN_OFF, EN_DL_STATE_NORMAL, EN_DL_STATE_DISABLED},
     {E_OSD_MAIN_MENU, HWND_SUB_MENU_DISSET_ASPRATIO_AUTO_ITEM, MApp_ZUI_Adjust_GetMultiWinMode, E_COMP_OP_EQUAL, eMULTIWIN_OFF, EN_DL_STATE_NORMAL, EN_DL_STATE_DISABLED},
     {E_OSD_MAIN_MENU, HWND_SUB_MENU_DISSET_ASPRATIO_5BY4_ITEM, MApp_ZUI_Adjust_GetMultiWinMode, E_COMP_OP_EQUAL, eMULTIWIN_OFF, EN_DL_STATE_NORMAL, EN_DL_STATE_DISABLED},
     {E_OSD_MAIN_MENU, HWND_SUB_MENU_DISSET_ASPRATIO_4BY3_ITEM, MApp_ZUI_Adjust_GetMultiWinMode, E_COMP_OP_EQUAL, eMULTIWIN_OFF, EN_DL_STATE_NORMAL, EN_DL_STATE_DISABLED},
     {E_OSD_MAIN_MENU, HWND_SUB_MENU_DISSET_ASPRATIO_16BY9_ITEM, MApp_ZUI_Adjust_GetMultiWinMode, E_COMP_OP_EQUAL, eMULTIWIN_OFF, EN_DL_STATE_NORMAL, EN_DL_STATE_DISABLED},
     {E_OSD_MAIN_MENU, HWND_SUB_MENU_DISSET_ASPRATIO_ONEBYONE_ITEM, MApp_ZUI_Adjust_GetMultiWinMode, E_COMP_OP_EQUAL, eMULTIWIN_OFF, EN_DL_STATE_NORMAL, EN_DL_STATE_DISABLED},
     {E_OSD_MAIN_MENU, HWND_SUB_MENU_MULTIWIN_WINSEL_WIN4_ITEM, MApp_ZUI_Adjust_GetMultiWinMode, E_COMP_OP_EQUAL, eMULTIWIN_4, EN_DL_STATE_NORMAL, EN_DL_STATE_DISABLED},
     {E_OSD_MAIN_MENU, HWND_SUB_MENU_DISSET_HP_ITEM, MApp_ZUI_Adjust_IsCurrentVGA, E_COMP_OP_EQUAL, TRUE, EN_DL_STATE_NORMAL, EN_DL_STATE_DISABLED},
     {E_OSD_MAIN_MENU, HWND_SUB_MENU_DISSET_VP_ITEM, MApp_ZUI_Adjust_IsCurrentVGA, E_COMP_OP_EQUAL, TRUE, EN_DL_STATE_NORMAL, EN_DL_STATE_DISABLED},
     {E_OSD_MAIN_MENU, HWND_SUB_MENU_DISSET_PIXCLK_ITEM, MApp_ZUI_Adjust_IsCurrentVGA, E_COMP_OP_EQUAL, TRUE, EN_DL_STATE_NORMAL, EN_DL_STATE_DISABLED},
     {E_OSD_MAIN_MENU, HWND_SUB_MENU_DISSET_HP_ITEM_VALUE, MApp_ZUI_Adjust_IsCurrentVGA, E_COMP_OP_EQUAL, TRUE, EN_DL_STATE_NORMAL, EN_DL_STATE_DISABLED},
     {E_OSD_MAIN_MENU, HWND_SUB_MENU_DISSET_VP_ITEM_VALUE, MApp_ZUI_Adjust_IsCurrentVGA, E_COMP_OP_EQUAL, TRUE, EN_DL_STATE_NORMAL, EN_DL_STATE_DISABLED},
     {E_OSD_MAIN_MENU, HWND_SUB_MENU_DISSET_PIXCLK_ITEM_VALUE, MApp_ZUI_Adjust_IsCurrentVGA, E_COMP_OP_EQUAL, TRUE, EN_DL_STATE_NORMAL, EN_DL_STATE_DISABLED},
     {E_OSD_MAIN_MENU, HWND_SUB_MENU_DISSET_PHS_ITEM_VALUE, MApp_ZUI_Adjust_IsCurrentVGA, E_COMP_OP_EQUAL, TRUE, EN_DL_STATE_NORMAL, EN_DL_STATE_DISABLED},
     {E_OSD_MAIN_MENU, HWND_SUB_MENU_DISSET_PHS_ITEM, MApp_ZUI_Adjust_IsCurrentVGA, E_COMP_OP_EQUAL, TRUE, EN_DL_STATE_NORMAL, EN_DL_STATE_DISABLED},
     {E_OSD_MAIN_MENU, HWND_SUB_MENU_COLSET_INPCOLFMT_AUTO_ITEM, MApp_ZUI_Adjust_GetInputPriority, E_COMP_OP_EQUAL, INPUT_PRIORITY_VGA, EN_DL_STATE_DISABLED, EN_DL_STATE_NORMAL},

};

///////////////////////////////////////////////////////////////////////////////
///  global static struct SceneGetDynamicListStatus2CondMap
///         This table is for get item status of list (MApp_ZUI_ACT_QueryDynamicListItemStatus) with two conditions.
///             This feature is the same with SceneGetDynamicListStatus0CondMap, it just only add two conditions.
///             If specified conditions are true, BM will return eListState. 
///             If not, BM will return eElseListState.
///
///         Related Proc: EN_ZUI_DYNAMICLIST_WINPROC
///         Related Map: SceneGetDynamicListStatus0CondMap, SceneGetDynamicListStatus1CondMap , SceneGetDynamicListStatus2CondMap , SceneGetDynamicListStatus3CondMap
///
/// @struct member        
///         osdID : OSD ID
///         hwnd: ZUI Window ID 
///         get_value_function_ptr1 (First condition set): getter function for compare value, this return value is first operand.  
///         compOperator1 (First condition set) : compare operator for compare value.
///         compParameter1 (First condition set): second operand
///         logicOperator1 : this logic operator is used to compare between first and secnd condition set.
///         get_value_function_ptr2 (Second condition set):  getter function for compare value, this return value is first operand.  
///         compOperator2 (Second condition set):  compare operator for compare value.
///         compParameter2 (Second condition set): second operand
///         eListState : if specified condition is true, BM will return this.
///         eElseListState : if specified condition is false, BM will return this.
///
///////////////////////////////////////////////////////////////////////////////

static SCENE_GET_DYNAMICLIST_STATUS_2CONDITION_MAP SceneGetDynamicListStatus2CondMap[] = {
     {E_OSD_MAIN_MENU, HWND_SUB_MENU_MULTIWIN_WINSEL_WIN3_ITEM, MApp_ZUI_Adjust_GetMultiWinMode, E_COMP_OP_NOT_EQUAL, eMULTIWIN_3, E_LOGIC_OP_AND, MApp_ZUI_Adjust_GetMultiWinMode, E_COMP_OP_NOT_EQUAL, eMULTIWIN_4,   EN_DL_STATE_DISABLED, EN_DL_STATE_NORMAL},
#if (ENABLE_DCR)
     {E_OSD_MAIN_MENU, HWND_SUB_MENU_DYNAMIC_CON_ITEM, MApp_ZUI_Adjust_GetColorTemperature, E_COMP_OP_EQUAL, CT_MOVIE, E_LOGIC_OP_OR, MApp_ZUI_Adjust_GetColorTemperature, E_COMP_OP_EQUAL, CT_GAME,   EN_DL_STATE_NORMAL, EN_DL_STATE_DISABLED},
     {E_OSD_MAIN_MENU, HWND_SUB_MENU_DCR_VALUE_ITEM, MApp_ZUI_Adjust_GetColorTemperature, E_COMP_OP_EQUAL, CT_MOVIE, E_LOGIC_OP_OR, MApp_ZUI_Adjust_GetColorTemperature, E_COMP_OP_EQUAL, CT_GAME,   EN_DL_STATE_NORMAL, EN_DL_STATE_DISABLED},
#endif
};

///////////////////////////////////////////////////////////////////////////////
///  global static struct SceneGetDynamicListStatus3CondMap
///         This table is for get item status of list (MApp_ZUI_ACT_QueryDynamicListItemStatus) with three conditions.
///             This feature is the same with SceneGetDynamicListStatus0CondMap, it just only add three conditions.
///             If specified conditions are true, BM will return eListState. 
///             If not, BM will return eElseListState.
///
///         Related Proc: EN_ZUI_DYNAMICLIST_WINPROC
///         Related Map: SceneGetDynamicListStatus0CondMap, SceneGetDynamicListStatus1CondMap , SceneGetDynamicListStatus2CondMap , SceneGetDynamicListStatus3CondMap
///
///         osdID : OSD ID
///         hwnd: ZUI Window ID 
///         get_value_function_ptr1 (First condition set): getter function for compare value, this return value is first operand.  
///         compOperator1 (First condition set) : compare operator for compare value.
///         compParameter1 (First condition set): second operand
///         logicOperator1 : this logic operator is used to compare between first and secnd condition set.
///         get_value_function_ptr2 (Second condition set):  getter function for compare value, this return value is first operand.  
///         compOperator2 (Second condition set):  compare operator for compare value.
///         compParameter2 (Second condition set): second operand
///         logicOperator2 : this logic operator is used to compare between second and third condition set.
///         get_value_function_ptr3 (Third condition set): getter function for compare value, this return value is first operand.  
///         compOperator3 (Third condition set) : compare operator for compare value.
///         compParameter3  (Third condition set): second operand
///         eListState : if specified condition is true, BM will return this.
///         eElseListState : if specified condition is false, BM will return this.
///
///////////////////////////////////////////////////////////////////////////////
static SCENE_GET_DYNAMICLIST_STATUS_3CONDITION_MAP SceneGetDynamicListStatus3CondMap[] = {
#if (ENABLE_MOVIE_GAME_PRESET)
     {E_OSD_MAIN_MENU, HWND_SUB_MENU_COLSET_HUE_VALUE_ITEM, MApp_ZUI_Adjust_GetColorTemperature, E_COMP_OP_EQUAL, CT_MOVIE, E_LOGIC_OP_OR, MApp_ZUI_Adjust_GetColorTemperature, E_COMP_OP_EQUAL, CT_GAME, E_LOGIC_OP_OR, MApp_ZUI_Adjust_GetColorTemperature, E_COMP_OP_EQUAL, CT_USER,   EN_DL_STATE_NORMAL, EN_DL_STATE_DISABLED},
     {E_OSD_MAIN_MENU, HWND_SUB_MENU_COLSET_SAT_VALUE_ITEM, MApp_ZUI_Adjust_GetColorTemperature, E_COMP_OP_EQUAL, CT_MOVIE, E_LOGIC_OP_OR, MApp_ZUI_Adjust_GetColorTemperature, E_COMP_OP_EQUAL, CT_GAME, E_LOGIC_OP_OR, MApp_ZUI_Adjust_GetColorTemperature, E_COMP_OP_EQUAL, CT_USER,   EN_DL_STATE_NORMAL, EN_DL_STATE_DISABLED},
     {E_OSD_MAIN_MENU, HWND_SUB_MENU_COLSET_HUE_ITEM, MApp_ZUI_Adjust_GetColorTemperature, E_COMP_OP_EQUAL, CT_MOVIE, E_LOGIC_OP_OR, MApp_ZUI_Adjust_GetColorTemperature, E_COMP_OP_EQUAL, CT_GAME, E_LOGIC_OP_OR, MApp_ZUI_Adjust_GetColorTemperature, E_COMP_OP_EQUAL, CT_USER,   EN_DL_STATE_NORMAL, EN_DL_STATE_DISABLED},
     {E_OSD_MAIN_MENU, HWND_SUB_MENU_COLSET_SAT_ITEM, MApp_ZUI_Adjust_GetColorTemperature, E_COMP_OP_EQUAL, CT_MOVIE, E_LOGIC_OP_OR, MApp_ZUI_Adjust_GetColorTemperature, E_COMP_OP_EQUAL, CT_GAME, E_LOGIC_OP_OR, MApp_ZUI_Adjust_GetColorTemperature, E_COMP_OP_EQUAL, CT_USER,   EN_DL_STATE_NORMAL, EN_DL_STATE_DISABLED},
#endif
};


#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MAPP_ZUI_BM_ACTIONTABLES_H */


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

#define MAPP_ZUI_ACTENDOCUSTOMFUNC_C
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
#include "MApp_ZUI_ACTCustomFunc.h"
#include "MApp_LoadFontInit.h"
#include "UserPref.h"
#include "appWindow.h"
#include "Power.h"

extern BYTE xdata   g_u8MenuTempValue;
extern U32 u32TimerDelay;
extern BOOLEAN g_bSourceChangeFlag;
EN_LANGUAGE u8Language;
BOOLEAN g_bAutoAdjustFlag = 0; //TODO: move to global.c

// adjust function
Bool MApp_ZUI_Adjust_SetInputSource(int input)
{
    g_u8MenuTempValue = input;
    if(SetInputSelect())
        u32TimerDelay=MsOS_GetSystemTime();
    g_bSourceChangeFlag = 1;
    return TRUE;
}

#if ENABLE_DCR
Bool MApp_ZUI_Adjust_SetDynamicContrast(int bOn)
{
    if(bOn != _ON_ && bOn != _OFF_ )
        return FALSE;

    g_u8MenuTempValue = bOn;
    SetDCRFunction();
    return TRUE;
}
#endif

Bool MApp_ZUI_Adjust_SetInputColorFormat(int type)
{
    g_u8MenuTempValue=type;
    SetInputColorFormatValue();
    return TRUE;
}

Bool MApp_ZUI_Adjust_SetColorTemperature(int temp)
{
    g_u8MenuTempValue = temp;
    //APPWindow_SetColorSpace(USER_PREF_WIN_SEL);
    SetColorTemperature();
    SetColorPresetMode();
    return TRUE;
}

Bool MApp_ZUI_Adjust_SetSuperResolutionLevel(int level)
{
#if ENABLE_SUPER_RESOLUTION    
    g_u8MenuTempValue = level;
    CURRENT_SUPER_RESOLUTION=g_u8MenuTempValue;
    //WAIT_V_OUTPUT_BLANKING();
    AdjustSuperResolution();
    SetSuperResolutionFunction();

    return TRUE;
#else
    CUSTOMFUNC_printMsg("[ERROR]MApp_ZUI_Adjust_SetSuperResolutionLevel() ENABLE_SUPER_RESOLUTION = 0\n");
    return FALSE;
#endif
}

Bool MApp_ZUI_Adjust_SetAspectRatioType(int type)
{
#if ENABLE_WIDE_MODE
    g_u8MenuTempValue = type;
    Menu_SetWideMode();
    return TRUE;
#else
    CUSTOMFUNC_printMsg("[ERROR]MApp_ZUI_Adjust_SetSuperResolutionLevel() ENABLE_WIDE_MODE = 0\n");
    return FALSE;
#endif
}


Bool MApp_ZUI_Adjust_SetResponseTimeMode(int bOn)
{
    if(bOn != _ON_ && bOn != _OFF_ )
        return FALSE;
    
    g_u8MenuTempValue = bOn;
    SetRTEFunction();
    return TRUE;
}

Bool MApp_ZUI_Adjust_SetMultiWindowMode(int mode)
{
#if ENABLE_MULTI_WINDOW_SETTINGS_MENU
    g_u8MenuTempValue = mode;
    if(g_u8MenuTempValue <=eMULTIWIN_2_POP2 && USER_PREF_WIN_SEL >= DISPLAY_WIN3)
        USER_PREF_WIN_SEL = DISPLAY_WIN2;
    else if(g_u8MenuTempValue <=eMULTIWIN_3 && USER_PREF_WIN_SEL == DISPLAY_WIN4)
        USER_PREF_WIN_SEL = DISPLAY_WIN3;
#if 0    
    if(Menu_SetMultiWindowModes())
        MApp_ZUI_ACT_ShutdownOSD();
#else
    Menu_SetMultiWindowModes();
#endif


    ZUI_MSG(printf("[END]MApp_ZUI_Adjust_SetMultiWindowMode() End, mode = %d\n", mode));

    return TRUE;
#else        
    ZUI_MSG(printf("[ERROR]MApp_ZUI_Adjust_SetMultiWindowMode() ENABLE_MULTI_WINDOW_SETTINGS_MENU = 0\n"));
    return FALSE
#endif

}

Bool MApp_ZUI_Adjust_SetMultiWindowSelect(int winType)
{
#if ENABLE_MULTI_WINDOW_SETTINGS_MENU
    g_u8MenuTempValue = winType;
    if(Menu_SetMultiWindowSelect())
        MApp_ZUI_ACT_ShutdownOSD();
    return TRUE;
#else        
    ZUI_MSG(printf("[ERROR]MApp_ZUI_Adjust_SetMultiWindowSelect() ENABLE_MULTI_WINDOW_SETTINGS_MENU = 0\n"));
    return FALSE;
#endif
}

Bool MApp_ZUI_Adjust_SetPIPSize(int sizeType)
{
#if ENABLE_MULTI_WINDOW_SETTINGS_MENU
    g_u8MenuTempValue = sizeType;    
    if(Menu_SetPIPSize())
        MApp_ZUI_ACT_ShutdownOSD();
    return TRUE;
#else
    ZUI_MSG(printf("[ERROR]MApp_ZUI_Adjust_SetPIPSize() ENABLE_MULTI_WINDOW_SETTINGS_MENU = 0\n"));
    return FALSE;
#endif
}

Bool MApp_ZUI_Adjust_SetLanguage(int lang)
{
    u8Language = (EN_LANGUAGE)lang;
#if (!VECTOR_FONT_ENABLE)
    // only bmp font support dynamic load font sets
    // closed in vector font for time consuming when generate font data
    MApp_LoadFontInit(u8Language);
#endif
    SET_OSD_MENU_LANGUAGE((EN_LANGUAGE)u8Language);
    MApp_ZUI_API_InvalidateAllSuccessors(HWND_MENU_TRANSPARENT_BG);
    return TRUE;
}

//single selection
Bool MApp_ZUI_Adjust_AutoAdjust(int unused)
{
    UNUSED(unused);
    //do auto adjust function here
    g_bAutoAdjustFlag = TRUE;
    MApp_ZUI_ACT_ShutdownOSD();
    MApp_ZUI_ACT_StartupOSD(E_OSD_AUTO_ADJUST);
    u32TimerDelay=MsOS_GetSystemTime();
    return TRUE;
}

Bool MApp_ZUI_Adjust_MenuRotate(int unused)
{
    UNUSED(unused);
    // do Menu Rotation function here
    if (MApp_ZUI_API_GetCurrentRotateAngle() == OSD_ROTATE_0)
        MApp_ZUI_API_DoRotate(OSD_ROTATE_90, TRUE);
    else
        MApp_ZUI_API_DoRotate(OSD_ROTATE_0, TRUE);
    MApp_ZUI_API_InvalidateAllSuccessors(HWND_MAINFRAME);
    return TRUE;
}

Bool MApp_ZUI_Adjust_FactoryReset(int unused)
{
    UNUSED(unused);
    //do factory mode function here
    ResetAllSetting();
    MApp_ZUI_ACT_ShutdownOSD();
    return TRUE;
}

WORD MApp_ZUI_Adjust_GetColorTemperature(void)
{
    return CURRENT_COLOR_TEMP;
}

WORD MApp_ZUI_Adjust_GetSuperResolutionLevel(void)
{
    return CURRENT_SUPER_RESOLUTION;
}

WORD MApp_ZUI_Adjust_GetAspectRatio(void)
{
    return USER_PREF_WIDE_MODE(SrcInputPortM);
}

WORD MApp_ZUI_Adjust_GetMultiWinMode(void)
{
    return USER_PREF_MULTI_WIN_MODE;
}

WORD MApp_ZUI_Adjust_GetWinSel(void)
{
    return USER_PREF_WIN_SEL;
}

WORD MApp_ZUI_Adjust_GetPIPSize(void)
{
    return USER_PREF_MULTI_WIN_SIZE(DISPLAY_WIN2);
}

WORD MApp_ZUI_Adjust_GetLanguage(void)
{
    return u8Language;
}

WORD MApp_ZUI_Adjust_GetCurrentInput(void)
{
    U16 nResult;
    
    if(CURRENT_INPUT_IS_VGA())
        nResult = EN_INPUT_VGA;
#if  ENABLE_DVI_INPUT
    else if(CURRENT_INPUT_IS_DVI())
        nResult = EN_INPUT_DVI;
#endif
#if ENABLE_HDMI_INPUT
    else if(CURRENT_INPUT_IS_DIGITAL_HDMI())
        nResult = EN_INPUT_DIGITAL_HDMI1;
#endif
#if ENABLE_HDMI2ND_INPUT
    else if(CURRENT_INPUT_IS_DIGITAL_HDMI2ND())
        nResult = EN_INPUT_DIGITAL_HDMI2;
#endif
#if ENABLE_DP_INPUT
    else if(CURRENT_INPUT_IS_DIGITAL_DP())
        nResult = EN_INPUT_DIGITAL_DP1;
#endif
#if ENABLE_DP2ND_INPUT
    else if(CURRENT_INPUT_IS_DIGITAL_DP2ND())
        nResult = EN_INPUT_DIGITAL_DP2;
#endif
    else
        nResult = 0;
    
    return nResult;
}

WORD MApp_ZUI_Adjust_GetUserPrefInputType(void)
{
    return USER_PREF_INPUT_TYPE;
}

WORD MApp_ZUI_Adjust_GetInputPriority(void)
{
    return USER_PREF_INPUT_PRIORITY;
}

WORD MApp_ZUI_Adjust_IsCurrentVGA(void)
{
    return CURRENT_INPUT_IS_VGA();
}

//dependent on ZUI item
Bool MApp_ZUI_Custom_AdjSaturationOrGreenColor(MENU_ITEM_ACTION enAction)
{
    if(CURRENT_COLOR_TEMP != CT_USER)
    {
#if ENABLE_MOVIE_GAME_PRESET
        AdjustVideoSaturation(enAction);
#else
        return FALSE;
#endif
    }
    else
    {
        AdjustUserGreenColor(enAction);
    }
    return TRUE;
}

//dependent on ZUI item
Bool MApp_ZUI_Custom_AdjHueOrRedColor(MENU_ITEM_ACTION enAction)
{
    if(CURRENT_COLOR_TEMP != CT_USER)
    {
#if ENABLE_MOVIE_GAME_PRESET
        AdjustVideoHue(enAction);
#else
        return FALSE;
#endif
    }
    else
    {
        AdjustUserRedColor(enAction);
    }
    return TRUE;
}

//dependent on ZUI item
WORD MApp_ZUI_Custom_GetSaturationOrGreenColor(void)
{
    if(CURRENT_COLOR_TEMP != CT_USER)
#if ENABLE_MOVIE_GAME_PRESET
        return GetSaturationValue();
#else
        return 0;
#endif
    else
        return GetUserGreenColorValue();
}

//dependent on ZUI item
WORD MApp_ZUI_Custom_GetHueOrRedColor(void)
{
    if(CURRENT_COLOR_TEMP != CT_USER)
#if ENABLE_MOVIE_GAME_PRESET
        return GetHueValue();
#else
        return 0;
#endif
    else
        return GetUserRedColorValue();
}

#undef MAPP_ZUI_ACTENDOCUSTOM_C


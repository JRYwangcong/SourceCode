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

#ifndef _MAPP_ZUI_ACTCUSTOM_H
#define _MAPP_ZUI_ACTCUSTOM_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include "MApp_ZUI_APIgdi.h"
#include "menufunc.h"

#ifdef MAPP_ZUI_ACTCUSTOM_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

#define CUSTOMFUNC_DEBUG    1
#if ENABLE_MSTV_UART_DEBUG&&CUSTOMFUNC_DEBUG
    #define CUSTOMFUNC_printData(str, value)   printData(str, value)
    #define CUSTOMFUNC_printMsg(str)           printMsg(str)
#else
    #define CUSTOMFUNC_DEBUG(str, value)
    #define CUSTOMFUNC_printMsg(str)
#endif


#define SCENE_FLOW_PATH_MAX 30
#define SCENE_NEXT_PAGE_MAP_MAX 150
#define SCENE_NEXT_LOOKUP_MAX 7 //search next scene if the hwnd of this scene is disable
#define SCENE_NULL 0


typedef enum _EN_SCENE_TYPE
{
    //mainmenu layer 1
    SCENE_MAINMENU_BRIGHTNESS=1, 
    SCENE_MAINMENU_AUTOADJUST, 
    SCENE_MAINMENU_INPUTSOURCE, 
    SCENE_MAINMENU_COLOR, 
    SCENE_MAINMENU_DISPLAY, 
    SCENE_MAINMENU_MULTIWINDOW,
    SCENE_MAINMENU_OTHER,
    
    //mainmenu layer 2
    SCENE_MAINMENU_BRIGHTNESS_L2, 
    //SCENE_MAINMENU_AUTOADJUST_L2, 
    SCENE_MAINMENU_INPUTSOURCE_L2, 
    SCENE_MAINMENU_COLOR_L2, 
    SCENE_MAINMENU_DISPLAY_L2, 
    SCENE_MAINMENU_MULTIWINDOW_L2,
    SCENE_MAINMENU_OTHER_L2,
   
    //mainmenu layer 3
    SCENE_MAINMENU_BRIGHTNESS_L3_BRIGHTNESS,
    SCENE_MAINMENU_BRIGHTNESS_L3_CONTRAST,
    SCENE_MAINMENU_BRIGHTNESS_L3_DYNAMICCONTRAST,

    SCENE_MAINMENU_COLOR_L3_INPUT_COLOR,
    SCENE_MAINMENU_COLOR_L3_PRESET_MODE,
    SCENE_MAINMENU_COLOR_L3_SUPER_RESOLUTION,
    SCENE_MAINMENU_COLOR_L3_HUE,
    SCENE_MAINMENU_COLOR_L3_SATURATION,
    SCENE_MAINMENU_COLOR_L3_CUSTOM_B,

    
    SCENE_MAINMENU_DISPLAY_L3_ASPECT_RATIO,
    SCENE_MAINMENU_DISPLAY_L3_HOR_POS,
    SCENE_MAINMENU_DISPLAY_L3_VER_POS,
    SCENE_MAINMENU_DISPLAY_L3_SHARPNESS,
    SCENE_MAINMENU_DISPLAY_L3_PIXEL_CLOCK,
    SCENE_MAINMENU_DISPLAY_L3_PHASE,
    SCENE_MAINMENU_DISPLAY_L3_RESPONSE_TIME,

    SCENE_MAINMENU_MULTIWINDOW_L3_WIN_MODE,
    SCENE_MAINMENU_MULTIWINDOW_L3_WIN_SEL,
    SCENE_MAINMENU_MULTIWINDOW_L3_PIP_SIZE,
    SCENE_MAINMENU_MULTIWINDOW_L3_PIP_HOR_POS,
    SCENE_MAINMENU_MULTIWINDOW_L3_PIP_VER_POS,
    
    SCENE_MAINMENU_OTHER_L3_LANGUAGE,
    SCENE_MAINMENU_OTHER_L3_MENU_TRANSPARENCY,

    //for DirMsgMenu
    SCENE_DIRMSG_MENU,

    //for Auto Adjust Menu
    SCENE_AUTOADJUST_MENU,

    //for LOGO
    SCENE_LOGO,
    
    SCENE_TYPE_MAX,
}EN_SCENE_TYPE;


typedef enum _EN_CURRENT_INPUT_TYPE
{
    EN_INPUT_VGA,
    EN_INPUT_DVI,   
    EN_INPUT_DIGITAL_HDMI1,  
    EN_INPUT_DIGITAL_HDMI2,  
    EN_INPUT_DIGITAL_DP1,  
    EN_INPUT_DIGITAL_DP2,  
    EN_INPUT_TYPE_MAX,
}EN_CURRENT_INPUT_TYPE;


void MApp_ZUI_Custom_SelectionHandler(HWND currentHwnd);
BOOL MApp_ZUI_Custom_ActionHandler( EN_OSD_WNDEXEACTION_ENUM execAct);
BOOL MApp_ZUI_Custom_OpenSceneHandler(EN_SCENE_TYPE sceneType);
BOOL MApp_ZUI_Custom_CloseSceneHandler(EN_SCENE_TYPE sceneType);
Bool MApp_ZUI_Custom_AutoCloseOSDHandler(void);
void MApp_ZUI_Custom_TerminateOSDHandler(E_OSD_ID eOSD_ID);
BOOLEAN MApp_ZUI_Custom_GlobalKeyHandler(E_OSD_ID eOSD_ID, VIRTUAL_KEY_CODE key);
LPTSTR MApp_ZUI_Custom_GetDynamicTextHandler(HWND hwnd);
U16 MApp_ZUI_Custom_GetDynamicBitmapHandler(HWND currentHwnd, DRAWSTYLE_TYPE type);
S16 MApp_ZUI_Custom_GetDynamicValueHandler(HWND hwnd);
Bool MApp_ZUI_Custom_GetDynamicColorHandler(HWND hwnd, DRAWSTYLE_TYPE type, OSD_COLOR colorOriginal, OSD_COLOR* colorTarget);
Bool MApp_ZUI_Custom_GetItemStatusHandler(HWND currentHwnd, GUI_ENUM_DYNAMIC_LIST_STATE* targetState);



#undef INTERFACE




#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MAPP_ZUI_ACTCUSTOM_H */


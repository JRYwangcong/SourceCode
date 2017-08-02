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

#ifndef _MAPP_ZUI_BM_CUSTOMTABLES_H
#define _MAPP_ZUI_BM_CUSTOMTABLES_H

////////////////////////////////////////////////////////////////////////////////
//Customize Behavior Handler File
//If Behavior Manager can't provide complete function to build customer OSD,
//user can use this file to customize OSD flow, it just like old ZUI style, you can use MApp_ZUI_API_SetFocus/MApp_ZUI_API_ShowWindow ....
////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
///  global static struct CustomActionHandler
///         User can handle all event in this handler.
///
///         BM will call this function pointer if ZUI trigger any event.
///
/// @struct member        
///         Function Pointer,
///////////////////////////////////////////////////////////////////////////////
SCENE_CUSTOM_ACTION_HANDLER CustomActionHandler = {MApp_ZUI_Custom_ActionHandler};

///////////////////////////////////////////////////////////////////////////////
///  global static struct CustomSelectionHandler
///         User Can handle "EN_EXE_SELECT_CUSTOM_ITEM" event in this handler.
///
///         User need to use "EN_EXE_SELECT_CUSTOM_ITEM" in .inl file , 
///         BM will call this function pointer if ZUI trigger "EN_EXE_SELECT_CUSTOM_ITEM" event.
///
/// @struct member        
///         Function Pointer,
///////////////////////////////////////////////////////////////////////////////
SCENE_CUSTOM_SELECTION_HANDLER CustomSelectionHandler = {MApp_ZUI_Custom_SelectionHandler};

///////////////////////////////////////////////////////////////////////////////
///  global static struct CustomOpenSceneHandler
///         This handler is dependent on BM open scene feature, 
///         If user need to customize OSD flow when BM open scene and BM didn't provide complete function to match customer OSD requiment,
///         user can use this handler to customize OSD flow, the usage is just like old ZUI.
///
/// @struct member        
///         Function Pointer,
///////////////////////////////////////////////////////////////////////////////
SCENE_CUSTOM_OPENSCENE_HANDLER CustomOpenSceneHandler = {MApp_ZUI_Custom_OpenSceneHandler};

///////////////////////////////////////////////////////////////////////////////
///  global static struct CustomCloseSceneHandler
///         This handler is dependent on BM close scene feature, 
///         If user need to customize OSD flow when BM close scene and BM didn't provide complete function to match customer OSD requiment,
///         user can use this handler to customize OSD flow, the usage is just like old ZUI.
///
/// @struct member        
///         Function Pointer,
///////////////////////////////////////////////////////////////////////////////
SCENE_CUSTOM_CLOSESCENE_HANDLER CustomCloseSceneHandler = {MApp_ZUI_Custom_CloseSceneHandler};

///////////////////////////////////////////////////////////////////////////////
///  global static struct CustomAutoCloseHandler
///         User can handle "EN_EXE_CLOSE_CURRENT_OSD" event in this handler.
///
///         User need to use "EN_EXE_CLOSE_CURRENT_OSD" in .inl file , 
///         BM will call this function pointer if ZUI trigger "EN_EXE_CLOSE_CURRENT_OSD" event.
///         In general, user will handler something when ZUI close OSD.
///
/// @struct member        
///         Function Pointer,
///////////////////////////////////////////////////////////////////////////////
SCENE_CUSTOM_AUTO_CLOSEOSD_HANDLER CustomAutoCloseHandler = {MApp_ZUI_Custom_AutoCloseOSDHandler};

///////////////////////////////////////////////////////////////////////////////
///  global static struct CustomTerminateOSDHandler
///         User can handle something in this handler when user call MApp_ZUI_ACT_ShutdownOSD or BM close OSD.
///
/// @struct member        
///         Function Pointer,
///////////////////////////////////////////////////////////////////////////////
SCENE_CUSTOM_TERMINATEOSD_HANDLER CustomTerminateOSDHandler = {MApp_ZUI_Custom_TerminateOSDHandler};

///////////////////////////////////////////////////////////////////////////////
///  global static struct CustomGlobalKeyHandler
///         User can handle global key in this handler if BM can't provide complete function to build OSD.
///
/// @struct member        
///         Function Pointer,
///////////////////////////////////////////////////////////////////////////////
SCENE_CUSTOM_GLOBALKEY_HANDLER CustomGlobalKeyHandler = {MApp_ZUI_Custom_GlobalKeyHandler};

///////////////////////////////////////////////////////////////////////////////
///  global static struct CustomDynamicTextHandler
///         User can handle EN_ZUI_DYNAMICTEXT_WINPROC if BM can't provide complete function to get text.
///
/// @struct member        
///         Function Pointer,
///////////////////////////////////////////////////////////////////////////////
SCENE_CUSTOM_DYNAMICTEXT_HANDLER CustomDynamicTextHandler = {MApp_ZUI_Custom_GetDynamicTextHandler};

///////////////////////////////////////////////////////////////////////////////
///  global static struct CustomDynamicBitmapHandler
///         User can handle EN_ZUI_DYNAMICBITMAP_WINPROC if BM can't provide complete function to get bitmap.
///
/// @struct member        
///         Function Pointer,
///////////////////////////////////////////////////////////////////////////////
SCENE_CUSTOM_DYNAMICBITMAP_HANDLER CustomDynamicBitmapHandler = {MApp_ZUI_Custom_GetDynamicBitmapHandler};

///////////////////////////////////////////////////////////////////////////////
///  global static struct CustomDynamicValueHandler
///         User can handle EN_ZUI_RECTPROGRESSBAR_2_WINPROC/EN_ZUI_BALLPROGRESSBAR_WINPROC if BM can't provide complete function to get value.
///
/// @struct member        
///         Function Pointer,
///////////////////////////////////////////////////////////////////////////////
SCENE_CUSTOM_DYNAMICVALUE_HANDLER CustomDynamicValueHandler = {MApp_ZUI_Custom_GetDynamicValueHandler};

///////////////////////////////////////////////////////////////////////////////
///  global static struct CustomDynamicColorHandler
///         User can handle EN_ZUI_DYNAMICCOLORTEXT_WINPROC if BM can't provide complete function to get color.
///
/// @struct member        
///         Function Pointer,
///////////////////////////////////////////////////////////////////////////////
SCENE_CUSTOM_DYNAMICCOLOR_HANDLER CustomDynamicColorHandler = {MApp_ZUI_Custom_GetDynamicColorHandler};

///////////////////////////////////////////////////////////////////////////////
///  global static struct CustomDynamicListItemHandler
///         User can handle EN_ZUI_DYNAMICLIST_WINPROC if BM can't provide complete function to get item status.
///
/// @struct member        
///         Function Pointer,
///////////////////////////////////////////////////////////////////////////////
SCENE_CUSTOM_DYNAMICLISTITEM_HANDLER CustomDynamicListItemHandler = {MApp_ZUI_Custom_GetItemStatusHandler};

#endif /* _MAPP_ZUI_BM_CUSTOMTABLES_H */


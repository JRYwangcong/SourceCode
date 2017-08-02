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

#ifndef _MAPP_ZUI_BM_ACTIONTEXTTABLES_H
#define _MAPP_ZUI_BM_ACTIONTEXTTABLES_H

#include "UserPrefDef.h"

#include "MApp_GlobalSettingSt.h"
#include "appWindow.h"


#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#ifdef _MAPP_ZUI_BM_ACTIONTEXTTABLES_H
#define INTERFACE
#else
#define INTERFACE extern
#endif

//this file is only for data mapping between value and text

///////////////////////////////////////////////////////////////////////////////
///  global static struct ResponseTimeTextMap
///  data mapping between value and text 
///  for response time
///
/// @struct member        
/// 1:value, 2:text index,
///
/// 1. Value from getter function , 
/// 2. display text index
///////////////////////////////////////////////////////////////////////////////
static SCENE_VALUE_TEXT_MAPPING_MAP ResponseTimeTextMap[] = {
    {_OFF_, en_str_Normal},
    {_ON_, en_str_Overdrive},
};

///////////////////////////////////////////////////////////////////////////////
///  global static struct MultiWinTextMap
///  data mapping between value and text 
///  for multi window mode
///
/// @struct member        
/// 1:value, 2:text index,
///
/// 1. Value from getter function , 
/// 2. display text index
///////////////////////////////////////////////////////////////////////////////
static SCENE_VALUE_TEXT_MAPPING_MAP MultiWinTextMap[] = {
    {eMULTIWIN_OFF, en_str_Off},
    {eMULTIWIN_2_PIP, en_str_PIP_Mode},
    {eMULTIWIN_2_POP1, en_str_PBP_Mode_1},
    {eMULTIWIN_2_POP2, en_str_PBP_Mode_2}, 
    {eMULTIWIN_3, en_str_Triple_Mode},
    {eMULTIWIN_4, en_str_Quad_Mode},
};

///////////////////////////////////////////////////////////////////////////////
///  global static struct OnOffTextMap
///  data mapping between value and text 
///  for on/off item
///
/// @struct member        
/// 1:value, 2:text index,
///
/// 1. Value from getter function , 
/// 2. display text index
///////////////////////////////////////////////////////////////////////////////
static SCENE_VALUE_TEXT_MAPPING_MAP OnOffTextMap[] = {
    {_ON_, en_str_On},
    {_OFF_, en_str_Off},
};

///////////////////////////////////////////////////////////////////////////////
///  global static struct InputColorFormatTextMap
///  data mapping between value and text 
///  for input color format
///
/// @struct member        
/// 1:value, 2:text index,
///
/// 1. Value from getter function , 
/// 2. display text index
///////////////////////////////////////////////////////////////////////////////
static SCENE_VALUE_TEXT_MAPPING_MAP InputColorFormatTextMap[] = {
    {INPUT_COLOR_RGB, en_str_RGB},
    {INPUT_COLOR_YPBPR, en_str_YPbPr},
    {INPUT_COLOR_FORMAT_AUTO, en_str_Auto},
};

///////////////////////////////////////////////////////////////////////////////
///  global static struct SuperResolutionTextMap
///  data mapping between value and text 
///  for super resolution
///
/// @struct member        
/// 1:value, 2:text index,
///
/// 1. Value from getter function , 
/// 2. display text index
///////////////////////////////////////////////////////////////////////////////
static SCENE_VALUE_TEXT_MAPPING_MAP SuperResolutionTextMap[] = {
    {SUPER_RESOLUTION_OFF, en_str_Off},
    {SUPER_RESOLUTION_HIGH, en_str_High},
    {SUPER_RESOLUTION_MIDDLE, en_str_Middle},
    {SUPER_RESOLUTION_LOW, en_str_Low},
};

///////////////////////////////////////////////////////////////////////////////
///  global static struct ColorTemperatureTextMap
///  data mapping between value and text 
///  for color temperature
///
/// @struct member        
/// 1:value, 2:text index,
///
/// 1. Value from getter function , 
/// 2. display text index
///////////////////////////////////////////////////////////////////////////////
static SCENE_VALUE_TEXT_MAPPING_MAP ColorTemperatureTextMap[] = {
    {CT_STANDARD, en_str_6500K},
    {CT_MOVIE, en_str_Movie},
    {CT_GAME, en_str_Game},
    {CT_WARM, en_str_5700K},
    {CT_COOL, en_str_9300K},
    {CT_USER, en_str_Custom_Color},
};

///////////////////////////////////////////////////////////////////////////////
///  global static struct AspectRatioTextMap
///  data mapping between value and text 
///  for aspect ratio
///
/// @struct member        
/// 1:value, 2:text index,
///
/// 1. Value from getter function , 
/// 2. display text index
///////////////////////////////////////////////////////////////////////////////
static SCENE_VALUE_TEXT_MAPPING_MAP AspectRatioTextMap[] = {
    {eASPECT_FULL, en_str_Full_Screen},
    {eASPECT_NATIVE, en_str_Auto},
    {eASPECT_5_4, en_str_5By4},
    {eASPECT_4_3, en_str_4By3},
    {eASPECT_16_9, en_str_Wide_16By9},
    {eASPECT_1_1, en_str_One_By_One},
};

///////////////////////////////////////////////////////////////////////////////
///  global static struct LanguageTextMap
///  data mapping between value and text 
///  for language setting
///
/// @struct member        
/// 1:value, 2:text index,
///
/// 1. Value from getter function , 
/// 2. display text index
///////////////////////////////////////////////////////////////////////////////
static SCENE_VALUE_TEXT_MAPPING_MAP LanguageTextMap[] = {
    {LANGUAGE_ENGLISH, en_str_English},
    {LANGUAGE_CHINESE, en_str_Chinese_Sim},
};

///////////////////////////////////////////////////////////////////////////////
///  global static struct MultiWinSizeTextMap
///  data mapping between value and text 
///  for multi window size
///
/// @struct member        
/// 1:value, 2:text index,
///
/// 1. Value from getter function , 
/// 2. display text index
///////////////////////////////////////////////////////////////////////////////
static SCENE_VALUE_TEXT_MAPPING_MAP MultiWinSizeTextMap[] = {
    {ePIPSIZE_SMALL, en_str_Small},
    {ePIPSIZE_MIDDLE, en_str_Medium},
    {ePIPSIZE_LARGE, en_str_Large},
    {ePIPSIZE_HUGE, en_str_Huge},
};

///////////////////////////////////////////////////////////////////////////////
///  global static struct WinSelTextMap
///  data mapping between value and text 
///  for window selection
///
/// @struct member        
/// 1:value, 2:text index,
///
/// 1. Value from getter function , 
/// 2. display text index
///////////////////////////////////////////////////////////////////////////////
static SCENE_VALUE_TEXT_MAPPING_MAP WinSelTextMap[] = {
    {DISPLAY_WIN1, en_str_Window1},
    {DISPLAY_WIN2, en_str_Window2},
    {DISPLAY_WIN3, en_str_Window3},
    {DISPLAY_WIN4, en_str_Window4},
};

///////////////////////////////////////////////////////////////////////////////
///  global static struct MultiWinModeTextMap
///  data mapping between value and text 
///  for multi-window mode
///
/// @struct member        
/// 1:value, 2:text index,
///
/// 1. Value from getter function , 
/// 2. display text index
///////////////////////////////////////////////////////////////////////////////
static SCENE_VALUE_TEXT_MAPPING_MAP MultiWinModeTextMap[] = {
    {eMULTIWIN_OFF, en_str_Off},
    {eMULTIWIN_2_PIP, en_str_PIP_Mode},
    {eMULTIWIN_2_POP1, en_str_PBP_Mode_1},
    {eMULTIWIN_2_POP2, en_str_PBP_Mode_2},
    {eMULTIWIN_3, en_str_Triple_Mode},    
    {eMULTIWIN_4, en_str_Quad_Mode},        
};

///////////////////////////////////////////////////////////////////////////////
///  global static struct HueTextMap
///  data mapping between value and text 
///  for hue adjustment
///  PS. it use "default text" feature (TEXTMAP_DEFAULT_TEXT), if value is not match, it will display default text.
///
/// @struct member        
/// 1:value, 2:text index,
///
/// 1. Value from getter function , 
/// 2. display text index
///////////////////////////////////////////////////////////////////////////////
static SCENE_VALUE_TEXT_MAPPING_MAP HueTextMap[] = {
    {CT_USER, en_str_CustomeR},
    {TEXTMAP_DEFAULT_TEXT, en_str_Hue}, // put the TEXTMAP_DEFAULT_TEXT value on the bottom
};

///////////////////////////////////////////////////////////////////////////////
///  global static struct SaturationTextMap
///  data mapping between value and text 
///  for saturation adjustment
///
/// @struct member        
/// 1:value, 2:text index,
///
/// 1. Value from getter function , 
/// 2. display text index
///////////////////////////////////////////////////////////////////////////////
static SCENE_VALUE_TEXT_MAPPING_MAP SaturationTextMap[] = {
    {CT_USER, en_str_CustomeG},
    {TEXTMAP_DEFAULT_TEXT, en_str_Saturation},  // put the TEXTMAP_DEFAULT_TEXT value on the bottom
};

///////////////////////////////////////////////////////////////////////////////
///  global static struct Item5TextMap
///  data mapping between value and text 
///  for Custom R/G/B adjustment
///
/// @struct member        
/// 1:value, 2:text index,
///
/// 1. Value from getter function , 
/// 2. display text index
///////////////////////////////////////////////////////////////////////////////
static SCENE_VALUE_TEXT_MAPPING_MAP Item5TextMap[] = {
    {CT_USER, en_str_CustomeB},
};

///////////////////////////////////////////////////////////////////////////////
///  global static struct CurrentInputTextMap
///  data mapping between value and text 
///  for input source selection
///
/// @struct member        
/// 1:value, 2:text index,
///
/// 1. Value from getter function , 
/// 2. display text index
///////////////////////////////////////////////////////////////////////////////
static SCENE_VALUE_TEXT_MAPPING_MAP CurrentInputTextMap[] = {
    {EN_INPUT_VGA, en_str_Source_VGA},
    {EN_INPUT_DVI, en_str_Source_DVI},
    {EN_INPUT_DIGITAL_HDMI1, en_str_HDMI_1},
    {EN_INPUT_DIGITAL_HDMI2, en_str_HDMI_2},
    {EN_INPUT_DIGITAL_DP1, en_str_Display_Port_1},
    {EN_INPUT_DIGITAL_DP2, en_str_Display_Port_2},
};


#undef INTERFACE

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MAPP_ZUI_BM_ACTIONTEXTTABLES_H */


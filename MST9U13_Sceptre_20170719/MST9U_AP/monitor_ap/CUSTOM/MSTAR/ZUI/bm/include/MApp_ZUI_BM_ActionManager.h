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

#ifndef _MAPP_ZUI_BM_ACTIONMANAGER_H
#define _MAPP_ZUI_BM_ACTIONMANAGER_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include "MApp_ZUI_APIgdi.h"
#include "MApp_ZUI_BM_OSDManager.h"

#ifdef _MAPP_ZUI_BM_ACTIONMANAGER_H
#define INTERFACE
#else
#define INTERFACE extern
#endif

#define TEXT_MAPPING_MAP_SIZE 20
#define TEXTMAP_DEFAULT_TEXT_DISABLE 0xFF
#define TEXTMAP_DEFAULT_TEXT 0xFF
#define E_BMP_EMPTY 0xFFFF
#define DRAW_ALL_STATUS 0xFF
#define DYNAMIC_VALUE_EMPTY 0x7FFF

//operator type
typedef enum _LOGICAL_OPERATOR_TYPE
{
    E_LOGIC_OP_AND,
    E_LOGIC_OP_OR,
}LOGICAL_OPERATOR_TYPE;

typedef enum _COMPARISON_OPERATOR_TYPE
{
    E_COMP_OP_EQUAL,
    E_COMP_OP_NOT_EQUAL,
}COMPARISON_OPERATOR_TYPE;

typedef enum _SCENE_ADJUST_TYPE
{
    E_ADJUST_INCREASE,
    E_ADJUST_DECREASE,
}SCENE_ADJUST_TYPE;

typedef struct _SCENE_ADJUSTMENT_HANDLER_MAP
{
    EN_SCENE_TYPE sceneType;
    HWND hwnd;
    Bool (*adjust_function_ptr)(Bool);
    WORD (*get_value_function_ptr)(void);
}SCENE_ADJUSTMENT_HANDLER_MAP;

typedef struct _SCENE_GET_VALUE_HANDLER_MAP
{
    E_OSD_ID osdID;
    HWND hwnd;
    WORD (*get_value_function_ptr)(void);
}SCENE_GET_VALUE_HANDLER_MAP;

typedef struct _SCENE_GET_VALUE_1CONDITION_HANDLER_MAP
{
    E_OSD_ID osdID;
    HWND hwnd;
    WORD (*get_condition1_function_ptr)(void);
    COMPARISON_OPERATOR_TYPE compOperator;
    WORD compParameter;
    WORD (*get_value_function_ptr)(void);
}SCENE_GET_VALUE_1CONDITION_HANDLER_MAP;

typedef struct _SCENE_VALUE_TEXT_MAPPING_MAP
{
   int value;
   int/*OSDCP_EN_STRING_INDEX*/ stringIndex;
}SCENE_VALUE_TEXT_MAPPING_MAP;

typedef struct _HWND_GET_TEXT_MAP
{
    E_OSD_ID osdID;
    HWND hwnd;
    U16 stringID;
}HWND_GET_TEXT_MAP;

typedef struct _SCENE_GET_MAPPING_TEXT_MAP
{
    E_OSD_ID osdID;
    HWND hwnd;
    SCENE_VALUE_TEXT_MAPPING_MAP *pTextMappingMap;
    U8 defaultTextIndex;
}SCENE_GET_MAPPING_TEXT_MAP;

typedef struct _SCENE_GET_CUSTOM_TEXT_HANDLER_MAP
{
    E_OSD_ID osdID;
    HWND hwnd;
    LPTSTR (*get_custom_text_function_ptr)(HWND hwnd);
}SCENE_GET_CUSTOM_TEXT_HANDLER_MAP;

typedef struct _SCENE_SELECTION_MAP
{
    EN_SCENE_TYPE sceneType;
    HWND hwnd;
    Bool (*select_function_ptr)(int);
    int parameter;
    U16 (*get_value_function_ptr)(void);
}SCENE_SELECTION_MAP;

typedef struct _SCENE_ADJUSTMENT_TEMP_VALUE
{
   bool bInitial;
   U32 tempValue;
}SCENE_ADJUSTMENT_TEMP_VALUE;

typedef struct _SCENE_GET_BITMAP_MAP
{
    E_OSD_ID osdID;
    HWND hwnd;
    HWND focusHwnd;
    U16 drawStyle;
    WORD (*get_value_function_ptr)(void);
    WORD compareParameter;
    OSDCP_EN_BITMAP_INDEX nBitmapID;
    OSDCP_EN_BITMAP_INDEX nElseBitmapID;
}SCENE_GET_BITMAP_MAP;

typedef struct _SCENE_GET_DYNAMICLIST_STATUS_NO_CONDITION_MAP
{
    E_OSD_ID osdID;
    HWND hwnd;
    GUI_ENUM_DYNAMIC_LIST_STATE eListState;
}SCENE_GET_DYNAMICLIST_STATUS_NO_CONDITION_MAP;

typedef struct _SCENE_GET_DYNAMICLIST_STATUS_1CONDITION_MAP
{
    E_OSD_ID osdID;
    HWND hwnd;
    WORD (*get_value_function_ptr)(void);
    COMPARISON_OPERATOR_TYPE compOperator;
    int compParameter;
    GUI_ENUM_DYNAMIC_LIST_STATE eListState;
    GUI_ENUM_DYNAMIC_LIST_STATE eElseListState;
}SCENE_GET_DYNAMICLIST_STATUS_1CONDITION_MAP;

typedef struct _SCENE_GET_DYNAMICLIST_STATUS_2CONDITION_MAP
{
    E_OSD_ID osdID;
    HWND hwnd;
    WORD (*get_value_function_ptr1)(void);
    COMPARISON_OPERATOR_TYPE compOperator1;
    int compParameter1;
    LOGICAL_OPERATOR_TYPE logicOperator1;
    WORD (*get_value_function_ptr2)(void);
    COMPARISON_OPERATOR_TYPE compOperator2;
    int compParameter2;
    GUI_ENUM_DYNAMIC_LIST_STATE eListState;
    GUI_ENUM_DYNAMIC_LIST_STATE eElseListState;
}SCENE_GET_DYNAMICLIST_STATUS_2CONDITION_MAP;

typedef struct _SCENE_GET_DYNAMICLIST_STATUS_3CONDITION_MAP
{
    E_OSD_ID osdID;
    HWND hwnd;
    WORD (*get_value_function_ptr1)(void);
    COMPARISON_OPERATOR_TYPE compOperator1;
    int compParameter1;
    LOGICAL_OPERATOR_TYPE logicOperator1;
    WORD (*get_value_function_ptr2)(void);
    COMPARISON_OPERATOR_TYPE compOperator2;
    int compParameter2;
    LOGICAL_OPERATOR_TYPE logicOperator2;
    WORD (*get_value_function_ptr3)(void);
    COMPARISON_OPERATOR_TYPE compOperator3;
    int compParameter3;
    GUI_ENUM_DYNAMIC_LIST_STATE eListState;
    GUI_ENUM_DYNAMIC_LIST_STATE eElseListState;
}SCENE_GET_DYNAMICLIST_STATUS_3CONDITION_MAP;
/////////////////////////////////////////
// Adjustment Handlers....
INTERFACE void MApp_ZUI_ACT_AdjustHandler(HWND currentHwnd, EN_OSD_WNDEXEACTION_ENUM execAct);
INTERFACE S16 MApp_ZUI_ACT_GetValueHandler(HWND currentHwnd);
INTERFACE BOOLEAN MApp_ZUI_ACT_NormalSelectionHandler(HWND focus);
INTERFACE BOOLEAN MApp_ZUI_ACT_UpDownSelectionHandler(UPDOWN_SELECTION_TYPE selectTYpe);
INTERFACE HWND MApp_ZUI_ACT_GetCurrentSceneFocus(EN_SCENE_TYPE curSceneType);
INTERFACE LPTSTR MApp_ZUI_ACT_GetTextHandler(HWND currentHwnd);
INTERFACE U16 MApp_ZUI_ACT_GetBitmapHandler(HWND currentHwnd, DRAWSTYLE_TYPE type);
INTERFACE OSD_COLOR MApp_ZUI_ACT_GetColorHandler(HWND currentHwnd, DRAWSTYLE_TYPE type, OSD_COLOR colorOriginal);
INTERFACE GUI_ENUM_DYNAMIC_LIST_STATE MApp_ZUI_ACT_GetDynamicListItemStatusHandler(HWND currentHwnd);


INTERFACE void MApp_ZUI_ACT_InitAdjustTempValue(void);
INTERFACE void MApp_ZUI_ACT_ResetAdjustTempValue(void);
INTERFACE bool MApp_ZUI_ACT_RestoreAdjustTempValue(void);
INTERFACE U16 MApp_ZUI_ACT_GetAdjustTempValue(void);
INTERFACE BOOL MApp_ZUI_ACT_ActionHandler(EN_OSD_WNDEXEACTION_ENUM execAct);


#undef INTERFACE

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MAPP_ZUI_BM_ACTIONMANAGER_H */


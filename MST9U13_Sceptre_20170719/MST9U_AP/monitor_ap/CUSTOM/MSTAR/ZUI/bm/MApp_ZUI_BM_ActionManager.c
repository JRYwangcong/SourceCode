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

#define MAPP_ZUI_BM_ACTIONMANAGER_C
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
#include "MApp_ZUI_BM_ActionTables.h"
#include "MApp_ZUI_ACTCustom.h"
#include "MApp_ZUI_ACTCustomFunc.h"
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

static SCENE_ADJUSTMENT_TEMP_VALUE AdjustTempValue;
LPTSTR _MApp_ZUI_ACT_GetTextCustom(HWND currentHwnd);
LPTSTR _MApp_ZUI_ACT_GetTextCommon(HWND currentHwnd);
void _MApp_ZUI_ACT_ReturnSceneSaveSetting(void);

//-------------------------------------------------------------------------------------------------
// Function
//-------------------------------------------------------------------------------------------------
BOOL MApp_ZUI_ACT_ActionHandler( EN_OSD_WNDEXEACTION_ENUM execAct)
{
    BOOL bResult = TRUE;
    switch(execAct)
    {
        case EN_EXE_ADJUST_MENU_INCREASE:
        case EN_EXE_ADJUST_MENU_DECREASE:
            ZUI_MSG(printf("EN_EXE_ADJUST_MENU_DECREASE EN_EXE_ADJUST_MENU_INCREASE\n"));
            MApp_ZUI_ACT_AdjustHandler(MApp_ZUI_API_GetFocus(), execAct);
            break;
        case EN_EXE_KEY_UP_CHANGE_VALUE:
            ZUI_MSG(printf("EN_EXE_KEY_UP_CHANGE_VALUE\n"));
            MApp_ZUI_ACT_UpDownSelectionHandler(TYPE_UP_SELECTION);
            break;
        case EN_EXE_KEY_DOWN_CHANGE_VALUE:
            ZUI_MSG(printf("EN_EXE_KEY_DOWN_CHANGE_VALUE\n"));
            MApp_ZUI_ACT_UpDownSelectionHandler(TYPE_DOWN_SELECTION);
            break;
        case EN_EXE_SELECT_CUSTOM_ITEM:
            ZUI_MSG(printf("EN_EXE_SELECT_CUSTOM_ITEM\n"));
            MApp_ZUI_ACT_CustomSelectionHandler(MApp_ZUI_API_GetFocus());
            //MApp_ZUI_Custom_SelectionHandler(MApp_ZUI_API_GetFocus());
            //MApp_ZUI_ACT_ReturnSceneHandler();
            break;
        case EN_EXE_SELECT_ITEM:
            ZUI_MSG(printf("EN_EXE_SELECT_ITEM\n"));
            MApp_ZUI_ACT_NormalSelectionHandler(MApp_ZUI_API_GetFocus());
            //MApp_ZUI_ACT_ReturnSceneHandler();
            break;
        case EN_EXE_CANCEL_ADJUST_VALUE:
            ZUI_MSG(printf("EN_EXE_CANCEL_ADJUST_VALUE\n"));
            MApp_ZUI_ACT_RestoreAdjustTempValue();
            MApp_ZUI_ACT_ReturnSceneHandler();
            break;
        //add custom execute action for Scene Transform 
        case EN_EXE_NEXT_PAGE:
        case EN_EXE_KEY_UP_PAGE:
        case EN_EXE_KEY_DOWN_PAGE:
        {
            ZUI_MSG(printf("[scene transform] exec act = %d, \n",execAct));
            SCENE_NEXT_PAGE_MAP sceneMap =  MApp_ZUI_ACT_GetNextScenePage(MApp_ZUI_API_GetFocus(), execAct);
            if(sceneMap.nextPageSceneType != SCENE_TYPE_MAX)
            {
                if(sceneMap.bResetFlowPath == TRUE)
                    MApp_ZUI_ACT_ResetSceneFlowPath();

                MApp_ZUI_ACT_OpenSceneHandler(sceneMap.nextPageSceneType);
            }
            
            break;
        }
        case EN_EXE_PREVIOUS_PAGE:
            ZUI_MSG(printf("EN_EXE_PREVIOUS_PAGE\n"));
            _MApp_ZUI_ACT_ReturnSceneSaveSetting();  //save eeprom handler
            MApp_ZUI_ACT_ReturnSceneHandler();
            break;      

        case EN_EXE_CLOSE_CURRENT_OSD:
            bResult = MApp_ZUI_ACT_AutoCloseHandler();
            break;            
        default:
            //custom action handler
            bResult = MApp_ZUI_ACT_CustomActionHandler(execAct);
            break;
    }

    return bResult;
}

void MApp_ZUI_ACT_AdjustHandler(HWND currentHwnd, EN_OSD_WNDEXEACTION_ENUM execAct)
{
    int x = 0;
    int mapSize = sizeof(SceneAdjustmentMap)/sizeof(SCENE_ADJUSTMENT_HANDLER_MAP);
    SCENE_ADJUST_TYPE eAdjustType = E_ADJUST_INCREASE;

    switch(execAct)
    {
        case EN_EXE_ADJUST_MENU_INCREASE:
            eAdjustType = E_ADJUST_INCREASE;
            break;
        case EN_EXE_ADJUST_MENU_DECREASE:
            eAdjustType = E_ADJUST_DECREASE;
            break;
        default:
            UNUSED(eAdjustType);
            ZUI_DBG_FAIL(printf("[ZUI]MApp_ZUI_ACT_AdjustHandler , execAct not support\n"));
            break;
    }

    for(x = 0; x < mapSize; x++)
    {
        if(SceneAdjustmentMap[x].sceneType == MApp_ZUI_ACT_GetCurrentSceneType()&& SceneAdjustmentMap[x].hwnd == currentHwnd && SceneAdjustmentMap[x].adjust_function_ptr != NULL)
        {
            ZUI_MSG(printf("[ZUI]MApp_ZUI_ACT_AdjustHandler hwnd = %d\n", currentHwnd));
            SceneAdjustmentMap[x].adjust_function_ptr(eAdjustType);
            MApp_ZUI_API_InvalidateAllSuccessors(MApp_ZUI_API_GetParent(currentHwnd));
            break;
        }
    }

}

S16 MApp_ZUI_ACT_GetValueCustom(HWND currentHwnd)
{
    return MApp_ZUI_ACT_DynamicValueHandler(currentHwnd);
}

S16 MApp_ZUI_ACT_GetValueCommon(HWND currentHwnd)
{
    ZUI_MSG(printf("[ZUI]MApp_ZUI_ACT_GetValueCommon currentHwnd = %d\n", currentHwnd));
    S16 nRetVal = 0;
    int x = 0;
    int mapSize =  sizeof(SceneGetValueMap) / sizeof(SCENE_GET_VALUE_HANDLER_MAP);

    for(x = 0; x < mapSize; x++)
    {
        if(SceneGetValueMap[x].osdID == E_OSD_EMPTY)
            break;
        
        if(SceneGetValueMap[x].osdID== MApp_ZUI_GetActiveOSD()  &&  SceneGetValueMap[x].hwnd == currentHwnd && SceneGetValueMap[x].get_value_function_ptr != NULL)
        {
            nRetVal = SceneGetValueMap[x].get_value_function_ptr();
            ZUI_MSG(printf("[ZUI]get_value nRetVal = %d\n", nRetVal));
            break;
        }
    }

    return nRetVal;
}

S16 MApp_ZUI_ACT_GetValueHandler(HWND currentHwnd)
{
    S16 nRetVal = DYNAMIC_VALUE_EMPTY;

    nRetVal = MApp_ZUI_ACT_GetValueCustom(currentHwnd);
    if(nRetVal == DYNAMIC_VALUE_EMPTY)
        nRetVal = MApp_ZUI_ACT_GetValueCommon(currentHwnd);

    return nRetVal;
}

BOOLEAN MApp_ZUI_ACT_NormalSelectionHandler(HWND focus)
{
    int x = 0;
    int mapSize =  sizeof(SceneSelectionMap) / sizeof(SCENE_SELECTION_MAP);

    for(x = 0; x < mapSize; x++)
    {
        if(SceneSelectionMap[x].sceneType == MApp_ZUI_ACT_GetCurrentSceneType() &&  SceneSelectionMap[x].hwnd == focus && SceneSelectionMap[x].select_function_ptr != NULL)
        {
            ZUI_MSG(printf("[ZUI] parameter= %d\n", SceneSelectionMap[x].parameter));
            //if(SceneSelectionMap[x].get_value_function_ptr != NULL)
            //    ZUI_MSG(printf("[ZUI]before,   get val= %d\n", SceneSelectionMap[x].get_value_function_ptr()));
            
            SceneSelectionMap[x].select_function_ptr(SceneSelectionMap[x].parameter);
            MApp_ZUI_API_InvalidateAllSuccessors( MApp_ZUI_API_GetParent(focus));     
            
            //if(SceneSelectionMap[x].get_value_function_ptr != NULL)
            //    ZUI_MSG(printf("[ZUI]after,   get val= %d\n", SceneSelectionMap[x].get_value_function_ptr()));
            
            return TRUE;
        }
    }
    ZUI_MSG(printf("[ZUI]MApp_ZUI_ACT_UpDownSelectionHandler, it cannot search the correct select_function_ptr\n"));
    return FALSE;
}

BOOLEAN MApp_ZUI_ACT_UpDownSelectionHandler(UPDOWN_SELECTION_TYPE selectTYpe)
{
    HWND focus;
    ZUI_MSG(printf("MApp_ZUI_ACT_UpDownSelectionHandler\n"));

    if(selectTYpe == TYPE_UP_SELECTION)
    {
        focus = GETWNDNAVI(MApp_ZUI_API_GetFocus())[NAV_UP];
        MApp_ZUI_API_SetFocusByNav(MApp_ZUI_API_GetFocus() , NAV_UP);
    }
    else // TYPE_DOWN_SELECTION
    {
        focus = GETWNDNAVI(MApp_ZUI_API_GetFocus())[NAV_DOWN];
        MApp_ZUI_API_SetFocusByNav(MApp_ZUI_API_GetFocus() , NAV_DOWN);
    }
    return MApp_ZUI_ACT_NormalSelectionHandler(focus);
}

LPTSTR MApp_ZUI_ACT_GetTextHandler(HWND currentHwnd)
{
    //ZUI_MSG(printf("[ZUI]MApp_ZUI_ACT_GetMenuTextHandler\n"));

    //custom text handler
    LPTSTR nRetVal = _MApp_ZUI_ACT_GetTextCustom(currentHwnd);
    if(nRetVal == NULL)
    {
        //common text handler
        nRetVal = _MApp_ZUI_ACT_GetTextCommon(currentHwnd);
    }
    return nRetVal;
}

LPTSTR _MApp_ZUI_ACT_GetTextCustom(HWND currentHwnd)
{
    //ZUI_MSG(printf("[ZUI]_MApp_ZUI_ACT_GetTextCustom\n"));
    return MApp_ZUI_ACT_DynamicTextHandler(currentHwnd);
}

LPTSTR _MApp_ZUI_ACT_GetHwndText(HWND currentHwnd)
{
    int x = 0;
    int mapSize =  sizeof(HwndGetTextMap) / sizeof(HWND_GET_TEXT_MAP);
    
    LPTSTR nRetVal = NULL;//MApp_ZUI_API_GetString(E_ZUI_STR_MAX); //for empty data

    for(x = 0; x < mapSize; x++)
    {
        if(HwndGetTextMap[x].osdID == 0)
            break;
        
        if(HwndGetTextMap[x].osdID == MApp_ZUI_GetActiveOSD()  &&  HwndGetTextMap[x].hwnd == currentHwnd)
        {
            nRetVal = MApp_ZUI_API_GetString(HwndGetTextMap[x].stringID);
        }
    }

    return nRetVal;

}

LPTSTR _MApp_ZUI_ACT_GetMenuTextMapping(HWND currentHwnd, WORD value)
{
    LPTSTR result = NULL;
    int x,y;
    int mapSize =  sizeof(SceneGetTextMappingMap) / sizeof(SCENE_GET_MAPPING_TEXT_MAP);
    BOOLEAN bSearchEnd = FALSE;
    for(x = 0; x < mapSize; x++)
    {
        if(SceneGetTextMappingMap[x].osdID == MApp_ZUI_GetActiveOSD()  &&  SceneGetTextMappingMap[x].hwnd == currentHwnd)
        {
            ZUI_MSG(printf("_MApp_ZUI_ACT_GetMenuTextMapping, currentHwnd = %d, value = %d\n", currentHwnd, value));

            int mappingMapSize = TEXT_MAPPING_MAP_SIZE;            
            //int mappingMapSize = sizeof ( *SceneGetTextMappingMap[x].pTextMappingMap) / sizeof( SCENE_VALUE_TEXT_MAPPING_MAP) ; 


            //search table
            for(y = 0; y < mappingMapSize; y++)
            {
                ZUI_MSG(printf("[ZUI]y=%d, value = %d\n", y, SceneGetTextMappingMap[x].pTextMappingMap[y].value));

                if( SceneGetTextMappingMap[x].pTextMappingMap[y].value == value)
                {
                    ZUI_MSG(printf("[ZUI]_MApp_ZUI_ACT_GetMenuTextMapping value =%d, string index = %d\n",value, SceneGetTextMappingMap[x].pTextMappingMap[y].stringIndex));
                    result = MApp_ZUI_API_GetString(SceneGetTextMappingMap[x].pTextMappingMap[y].stringIndex);
                    bSearchEnd = TRUE; // stop search
                    break; // stop for loop (y=0;.....)
                }
                //default text
                if( SceneGetTextMappingMap[x].defaultTextIndex != TEXTMAP_DEFAULT_TEXT_DISABLE && SceneGetTextMappingMap[x].pTextMappingMap[y].value == TEXTMAP_DEFAULT_TEXT) 
                {
                    ZUI_MSG(printf("[ZUI]_MApp_ZUI_ACT_GetMenuTextMapping ,hwnd = %d default text index =%d, string index =%d\n",currentHwnd, SceneGetTextMappingMap[x].defaultTextIndex,  SceneGetTextMappingMap[x].pTextMappingMap[ SceneGetTextMappingMap[x].defaultTextIndex ].stringIndex));
                    result = MApp_ZUI_API_GetString(SceneGetTextMappingMap[x].pTextMappingMap[ SceneGetTextMappingMap[x].defaultTextIndex ].stringIndex);
                    bSearchEnd = TRUE; // stop search
                    break; 
                }            
            }

            if(bSearchEnd == TRUE) // stop search
                break; // stop for loop (x=0;.....)

            ZUI_MSG(printf("[ZUI]defaultTextIndex = %d\n", y, SceneGetTextMappingMap[x].defaultTextIndex));
            break;
            
        }
    }
    return result;
}

LPTSTR _MApp_ZUI_ACT_GetValueText(HWND currentHwnd)
{
    int x = 0;
    int mapSize =  sizeof(SceneGetValueMap) / sizeof(SCENE_GET_VALUE_HANDLER_MAP);
    
    LPTSTR nRetVal = NULL;//MApp_ZUI_API_GetString(E_ZUI_STR_MAX); //for empty data

    for(x = 0; x < mapSize; x++)
    {
        if(SceneGetValueMap[x].osdID == E_OSD_EMPTY)
            break;
        
        if(SceneGetValueMap[x].osdID == MApp_ZUI_GetActiveOSD()  &&  SceneGetValueMap[x].hwnd == currentHwnd && SceneGetValueMap[x].get_value_function_ptr != NULL)
        {
            WORD nTempValue = SceneGetValueMap[x].get_value_function_ptr();

            ZUI_MSG(printf("[ZUI]_MApp_ZUI_ACT_GetValueText, hwnd = %d, get value=%d, x = %d \n",currentHwnd, nTempValue, x));

            //if value need mapping to other text , it will get mapping text first.
            //if value dont need to get mapping text, it will casting to String type.
            nRetVal = _MApp_ZUI_ACT_GetMenuTextMapping(currentHwnd, nTempValue);
            if(nRetVal == NULL)
            {
                nRetVal = MApp_ZUI_API_GetU16String(nTempValue);
            }
            //ZUI_MSG(printf("[ZUI]_MApp_ZUI_ACT_GetValueText nTempValue = %d\n", nTempValue));
            break;
        }
    }

    return nRetVal;

}

LPTSTR _MApp_ZUI_ACT_GetValueText1Condition(HWND currentHwnd)
{
    int x = 0;
    int mapSize =  sizeof(SceneGetValue1CondMap) / sizeof(SCENE_GET_VALUE_1CONDITION_HANDLER_MAP);
    
    LPTSTR nRetVal = NULL;//MApp_ZUI_API_GetString(E_ZUI_STR_MAX); //for empty data
    WORD nTempValue;

    for(x = 0; x < mapSize; x++)
    {
        if(SceneGetValue1CondMap[x].osdID == E_OSD_EMPTY || SceneGetValue1CondMap[x].get_condition1_function_ptr == NULL 
            || SceneGetValue1CondMap[x].get_value_function_ptr == NULL)
            break;
        
        if(SceneGetValue1CondMap[x].osdID == MApp_ZUI_GetActiveOSD()
            &&  SceneGetValue1CondMap[x].hwnd == currentHwnd)
        {
            if(SceneGetValue1CondMap[x].compOperator == E_COMP_OP_EQUAL)
            {
                if( SceneGetValue1CondMap[x].get_condition1_function_ptr() == SceneGetValue1CondMap[x].compParameter)
                    nTempValue = SceneGetValue1CondMap[x].get_value_function_ptr();
                else
                    continue;
            }
            else // if(SceneGetValue1CondMap[x].compOperator == E_COMP_OP_NOT_EQUAL)
            {
                if( SceneGetValue1CondMap[x].get_condition1_function_ptr() != SceneGetValue1CondMap[x].compParameter)
                    nTempValue = SceneGetValue1CondMap[x].get_value_function_ptr();
                else
                    continue;
            }
            //if value need mapping to other text , it will get mapping text first.
            //if value dont need to get mapping text, it will casting to String type.
            nRetVal = _MApp_ZUI_ACT_GetMenuTextMapping(currentHwnd, nTempValue);
            if(nRetVal == NULL)
            {
                nRetVal = MApp_ZUI_API_GetU16String(nTempValue);
            }
            
            ZUI_MSG(printf("[ZUI]_MApp_ZUI_ACT_GetValueText1Condition, hwnd = %d get value = %d\n",currentHwnd, nTempValue));
            break;
        }
    }

    return nRetVal;

}

LPTSTR _MApp_ZUI_ACT_GetTextCommon(HWND currentHwnd)
{
    LPTSTR nRetVal = NULL;

    nRetVal = _MApp_ZUI_ACT_GetHwndText(currentHwnd);

    if(nRetVal == NULL)
        nRetVal = _MApp_ZUI_ACT_GetValueText(currentHwnd);

    if(nRetVal == NULL)
        nRetVal = _MApp_ZUI_ACT_GetValueText1Condition(currentHwnd);
    
    if(nRetVal == NULL)
        nRetVal = MApp_ZUI_API_GetString(E_ZUI_STR_MAX); //for empty data

    return nRetVal;
}

U16 _MApp_ZUI_ACT_GetBitmapCustom(HWND currentHwnd, DRAWSTYLE_TYPE type)
{
    return MApp_ZUI_ACT_DynamicBitmapHandler(currentHwnd, type);
}

U16 _MApp_ZUI_ACT_GetBitmapCommon(HWND currentHwnd, DRAWSTYLE_TYPE type)
{
    int x = 0;
    int mapSize =  sizeof(SceneGetBitmapMap) / sizeof(SCENE_GET_BITMAP_MAP);
    
    U16 nRetVal = E_BMP_EMPTY;//MApp_ZUI_API_GetString(E_ZUI_STR_MAX); //for empty data

    for(x = 0; x < mapSize; x++)
    {
        if(SceneGetBitmapMap[x].osdID == E_OSD_EMPTY || ( type != SceneGetBitmapMap[x].drawStyle && SceneGetBitmapMap[x].drawStyle != DRAW_ALL_STATUS))
            break;
        
        if(SceneGetBitmapMap[x].osdID == MApp_ZUI_GetActiveOSD()  &&  SceneGetBitmapMap[x].hwnd == currentHwnd)
        {
            if( ( (SceneGetBitmapMap[x].focusHwnd != 0 && SceneGetBitmapMap[x].focusHwnd == MApp_ZUI_API_GetFocus() ) || SceneGetBitmapMap[x].focusHwnd == 0)
               && (SceneGetBitmapMap[x].get_value_function_ptr() == SceneGetBitmapMap[x].compareParameter))
            {
                nRetVal =  SceneGetBitmapMap[x].nBitmapID;
            }
            else
            {
                nRetVal =  SceneGetBitmapMap[x].nElseBitmapID;
            }
            break; //search end
        }
    }

    return nRetVal;
}



U16 MApp_ZUI_ACT_GetBitmapHandler(HWND currentHwnd, DRAWSTYLE_TYPE type)
{
    //ZUI_MSG(printf("[ZUI]MApp_ZUI_ACT_GetBitmapHandler\n"));

    //custom bitmap handler
    U16 nRetVal = _MApp_ZUI_ACT_GetBitmapCustom(currentHwnd, type);
    if(nRetVal == E_BMP_EMPTY)
    {
        //common bitmap handler
        nRetVal = _MApp_ZUI_ACT_GetBitmapCommon(currentHwnd, type);
    }
    return nRetVal;
}

Bool _MApp_ZUI_ACT_GetColorCustom(HWND currentHwnd, DRAWSTYLE_TYPE type, OSD_COLOR colorOriginal, OSD_COLOR* colorTarget)
{
    return MApp_ZUI_ACT_DynamicColorHandler(currentHwnd, type, colorOriginal, colorTarget);
}

Bool _MApp_ZUI_ACT_GetColorCommon(HWND currentHwnd, DRAWSTYLE_TYPE type, OSD_COLOR colorOriginal, OSD_COLOR* colorTarget)
{
    UNUSED(type);
    UNUSED(colorOriginal);

    Bool bResult = FALSE;
    if(! MApp_ZUI_ACT_GetReturnPathAutoColor(currentHwnd, colorTarget) ) //auto color
    {
        //TODO: implement it , Common color 
    }
    return bResult;
}

OSD_COLOR MApp_ZUI_ACT_GetColorHandler(HWND currentHwnd, DRAWSTYLE_TYPE type, OSD_COLOR colorOriginal)
{
    //ZUI_MSG(printf("[ZUI]MApp_ZUI_ACT_GetColorHandler\n"));
    OSD_COLOR nRetColor = colorOriginal;
    //custom color handler
    if(!_MApp_ZUI_ACT_GetColorCustom(currentHwnd, type , colorOriginal, &nRetColor))
    {
        //common color handler
        _MApp_ZUI_ACT_GetColorCommon(currentHwnd, type , colorOriginal, &nRetColor);
    }
    return nRetColor;
}

Bool _MApp_ZUI_ACT_GetItemStatusCustom(HWND currentHwnd, GUI_ENUM_DYNAMIC_LIST_STATE* targetState)
{
    return MApp_ZUI_ACT_DynamicListItemHandler(currentHwnd, targetState);

}

Bool _MApp_ZUI_ACT_GetItemStatusCommon_NoCondition(HWND currentHwnd, GUI_ENUM_DYNAMIC_LIST_STATE* targetState)
{
    int x;
    BOOLEAN bResult = FALSE;
    int mapSize = sizeof(SceneGetDynamicListStatus0CondMap)/sizeof(SCENE_GET_DYNAMICLIST_STATUS_NO_CONDITION_MAP);

    for(x = 0; x < mapSize; x++)
    {   
        if( (MApp_ZUI_GetActiveOSD() == SceneGetDynamicListStatus0CondMap[x].osdID) && (SceneGetDynamicListStatus0CondMap[x].hwnd ==  currentHwnd) )
        {
            *targetState = SceneGetDynamicListStatus0CondMap[x].eListState;
            bResult = TRUE;
            break; //search end
        }
    }
    return bResult;
}

Bool _MApp_ZUI_ACT_GetItemStatusCommon_1Condition(HWND currentHwnd, GUI_ENUM_DYNAMIC_LIST_STATE* targetState)
{
    int x;
    BOOLEAN bResult = FALSE;
    int mapSize = sizeof(SceneGetDynamicListStatus1CondMap)/sizeof(SCENE_GET_DYNAMICLIST_STATUS_1CONDITION_MAP);

    for(x = 0; x < mapSize; x++)
    {   
        if( (MApp_ZUI_GetActiveOSD() == SceneGetDynamicListStatus1CondMap[x].osdID) && (SceneGetDynamicListStatus1CondMap[x].hwnd ==  currentHwnd) )
        {
#if 0            
            if( ((SceneGetDynamicListStatus1CondMap[x].get_value_function_ptr != NULL) && 
                ((SceneGetDynamicListStatus1CondMap[x].compOperator == E_COMP_OP_EQUAL && SceneGetDynamicListStatus1CondMap[x].get_value_function_ptr() == SceneGetDynamicListStatus1CondMap[x].compareParameter)
                ||(SceneGetDynamicListStatus1CondMap[x].compOperator == E_COMP_OP_NOT_EQUAL && SceneGetDynamicListStatus1CondMap[x].get_value_function_ptr() != SceneGetDynamicListStatus1CondMap[x].compareParameter) ) )
                || (SceneGetDynamicListStatus1CondMap[x].get_value_function_ptr == NULL))
                *targetState = SceneGetDynamicListStatus1CondMap[x].eListState;
            else
                *targetState = SceneGetDynamicListStatus1CondMap[x].eElseListState;
#else // readable code
            ZUI_MSG(printf("x = %d ,currentHwnd=%d, get value = %d, operator = %d,  parameter = %d \n",
                x, currentHwnd,SceneGetDynamicListStatus1CondMap[x].get_value_function_ptr(),
                SceneGetDynamicListStatus1CondMap[x].compOperator,
                SceneGetDynamicListStatus1CondMap[x].compParameter ));


            if(SceneGetDynamicListStatus1CondMap[x].get_value_function_ptr == NULL)
                *targetState = SceneGetDynamicListStatus1CondMap[x].eListState;
            else if( SceneGetDynamicListStatus1CondMap[x].get_value_function_ptr != NULL)
            {
                if(SceneGetDynamicListStatus1CondMap[x].compOperator == E_COMP_OP_EQUAL)
                {
                    if(SceneGetDynamicListStatus1CondMap[x].get_value_function_ptr() == SceneGetDynamicListStatus1CondMap[x].compParameter)
                        *targetState = SceneGetDynamicListStatus1CondMap[x].eListState;
                    else
                        *targetState = SceneGetDynamicListStatus1CondMap[x].eElseListState;
                }
                else //if(SceneGetDynamicListStatus1CondMap[x].compOperator == E_COMP_OP_NOT_EQUAL)
                {
                    if(SceneGetDynamicListStatus1CondMap[x].get_value_function_ptr() != SceneGetDynamicListStatus1CondMap[x].compParameter)
                        *targetState = SceneGetDynamicListStatus1CondMap[x].eListState;
                    else
                        *targetState = SceneGetDynamicListStatus1CondMap[x].eElseListState;
                }
            }
#endif
            bResult = TRUE;
            break; //search end
        }
    }
    return bResult;
}

Bool _MApp_ZUI_ACT_GetItemStatusCommon_2Condition(HWND currentHwnd, GUI_ENUM_DYNAMIC_LIST_STATE* targetState)
{
    int x;
    BOOLEAN bResult = FALSE;
    int mapSize = sizeof(SceneGetDynamicListStatus2CondMap)/sizeof(SCENE_GET_DYNAMICLIST_STATUS_2CONDITION_MAP);
    Bool bOperand1, bOperand2;

    //ZUI_MSG(printf("[ZUI]_MApp_ZUI_ACT_GetItemStatusCommon_2Condition !!!\n"));

    for(x = 0; x < mapSize; x++)
    {   
        if( (MApp_ZUI_GetActiveOSD() == SceneGetDynamicListStatus2CondMap[x].osdID) && (SceneGetDynamicListStatus2CondMap[x].hwnd ==  currentHwnd) )
        {
            if(SceneGetDynamicListStatus2CondMap[x].get_value_function_ptr1 == NULL || SceneGetDynamicListStatus2CondMap[x].get_value_function_ptr2 == NULL)
                break;

            //ZUI_MSG(printf("[ZUI]get value 1 = %d, comp value = %d,  !!!\n", SceneGetDynamicListStatus2CondMap[x].get_value_function_ptr1() , SceneGetDynamicListStatus2CondMap[x].compParameter1));
            //ZUI_MSG(printf("[ZUI]get value 2 = %d, comp value = %d,  !!!\n", SceneGetDynamicListStatus2CondMap[x].get_value_function_ptr2() , SceneGetDynamicListStatus2CondMap[x].compParameter2));

            
            if(SceneGetDynamicListStatus2CondMap[x].compOperator1 == E_COMP_OP_EQUAL)
            {
                if(SceneGetDynamicListStatus2CondMap[x].get_value_function_ptr1() == SceneGetDynamicListStatus2CondMap[x].compParameter1)
                    bOperand1 = TRUE;
                else
                    bOperand1 = FALSE;
            }
            else //if(SceneGetDynamicListStatus1CondMap[x].compOperator1 == E_COMP_OP_NOT_EQUAL)
            {
                if(SceneGetDynamicListStatus2CondMap[x].get_value_function_ptr1() != SceneGetDynamicListStatus2CondMap[x].compParameter1)
                    bOperand1 = TRUE;
                else
                    bOperand1 = FALSE;
            }

            if(SceneGetDynamicListStatus2CondMap[x].compOperator2 == E_COMP_OP_EQUAL)
            {
                if(SceneGetDynamicListStatus2CondMap[x].get_value_function_ptr2() == SceneGetDynamicListStatus2CondMap[x].compParameter2)
                    bOperand2 = TRUE;
                else
                    bOperand2 = FALSE;
            }
            else //if(SceneGetDynamicListStatus1CondMap[x].compOperator2 == E_COMP_OP_NOT_EQUAL)
            {
                if(SceneGetDynamicListStatus2CondMap[x].get_value_function_ptr2() != SceneGetDynamicListStatus2CondMap[x].compParameter2)
                    bOperand2 = TRUE;
                else
                    bOperand2 = FALSE;
            }
            //ZUI_MSG(printf("[ZUI]bOperand1 = %d, bOperand2 = %d  !!!\n",bOperand1, bOperand2));

            if(SceneGetDynamicListStatus2CondMap[x].logicOperator1 == E_LOGIC_OP_AND)
            {
                if(bOperand1 && bOperand2)
                    *targetState = SceneGetDynamicListStatus2CondMap[x].eListState;
                else
                    *targetState = SceneGetDynamicListStatus2CondMap[x].eElseListState;
            }
            else // if(SceneGetDynamicListStatus2CondMap[x].logicOperator1 == E_LOGIC_OP_OR)
            {
                if(bOperand1 || bOperand2)
                    *targetState = SceneGetDynamicListStatus2CondMap[x].eListState;
                else
                    *targetState = SceneGetDynamicListStatus2CondMap[x].eElseListState;
            }
            bResult = TRUE;
            break; //search end
        }
    }
    return bResult;
}

Bool _MApp_ZUI_ACT_GetItemStatusCommon_3Condition(HWND currentHwnd, GUI_ENUM_DYNAMIC_LIST_STATE* targetState)
{
    int x;
    BOOLEAN bResult = FALSE;
    int mapSize = sizeof(SceneGetDynamicListStatus3CondMap)/sizeof(SCENE_GET_DYNAMICLIST_STATUS_3CONDITION_MAP);

    Bool bOperand1, bOperand2, bOperand3;
    for(x = 0; x < mapSize; x++)
    {   
        if( (MApp_ZUI_GetActiveOSD() == SceneGetDynamicListStatus3CondMap[x].osdID) && (SceneGetDynamicListStatus3CondMap[x].hwnd ==  currentHwnd) )
        {
            if(SceneGetDynamicListStatus3CondMap[x].get_value_function_ptr1 == NULL || SceneGetDynamicListStatus3CondMap[x].get_value_function_ptr2 == NULL
                 || SceneGetDynamicListStatus3CondMap[x].get_value_function_ptr3 == NULL)
                break;
            
            if(SceneGetDynamicListStatus3CondMap[x].compOperator1 == E_COMP_OP_EQUAL)
            {
                if(SceneGetDynamicListStatus3CondMap[x].get_value_function_ptr1() == SceneGetDynamicListStatus3CondMap[x].compParameter1)
                    bOperand1 = TRUE;
                else
                    bOperand1 = FALSE;
            }
            else //if(SceneGetDynamicListStatus1CondMap[x].compOperator1 == E_COMP_OP_NOT_EQUAL)
            {
                if(SceneGetDynamicListStatus3CondMap[x].get_value_function_ptr1() != SceneGetDynamicListStatus3CondMap[x].compParameter1)
                    bOperand1 = TRUE;
                else
                    bOperand1 = FALSE;
            }

            if(SceneGetDynamicListStatus3CondMap[x].compOperator2 == E_COMP_OP_EQUAL)
            {
                if(SceneGetDynamicListStatus3CondMap[x].get_value_function_ptr2() == SceneGetDynamicListStatus3CondMap[x].compParameter2)
                    bOperand2 = TRUE;
                else
                    bOperand2 = FALSE;
            }
            else //if(SceneGetDynamicListStatus1CondMap[x].compOperator2 == E_COMP_OP_NOT_EQUAL)
            {
                if(SceneGetDynamicListStatus3CondMap[x].get_value_function_ptr2() != SceneGetDynamicListStatus3CondMap[x].compParameter2)
                    bOperand2 = TRUE;
                else
                    bOperand2 = FALSE;
            }

            if(SceneGetDynamicListStatus3CondMap[x].compOperator3 == E_COMP_OP_EQUAL)
            {
                if(SceneGetDynamicListStatus3CondMap[x].get_value_function_ptr3() == SceneGetDynamicListStatus3CondMap[x].compParameter3)
                    bOperand3 = TRUE;
                else
                    bOperand3 = FALSE;
            }
            else //if(SceneGetDynamicListStatus1CondMap[x].compOperator2 == E_COMP_OP_NOT_EQUAL)
            {
                if(SceneGetDynamicListStatus3CondMap[x].get_value_function_ptr3() != SceneGetDynamicListStatus3CondMap[x].compParameter3)
                    bOperand3 = TRUE;
                else
                    bOperand3 = FALSE;
            }

            if(SceneGetDynamicListStatus3CondMap[x].logicOperator1 == E_LOGIC_OP_AND)
            {
                if(SceneGetDynamicListStatus3CondMap[x].logicOperator2 == E_LOGIC_OP_AND)
                {
                    if(bOperand1 && bOperand2 && bOperand3)
                        *targetState = SceneGetDynamicListStatus3CondMap[x].eListState;
                    else
                        *targetState = SceneGetDynamicListStatus3CondMap[x].eElseListState;
                }
                else // if(SceneGetDynamicListStatus3CondMap[x].logicOperator2 == E_LOGIC_OP_OR)
                {
                    if( (bOperand1 && bOperand2) || bOperand3)
                        *targetState = SceneGetDynamicListStatus3CondMap[x].eListState;
                    else
                        *targetState = SceneGetDynamicListStatus3CondMap[x].eElseListState;
                }
                
            }
            else // if(SceneGetDynamicListStatus2CondMap[x].logicOperator1 == E_LOGIC_OP_OR)
            {
                if(SceneGetDynamicListStatus3CondMap[x].logicOperator2 == E_LOGIC_OP_AND)
                {
                    if((bOperand1 || bOperand2) && bOperand3)
                        *targetState = SceneGetDynamicListStatus3CondMap[x].eListState;
                    else
                        *targetState = SceneGetDynamicListStatus3CondMap[x].eElseListState;
                }
                else // if(SceneGetDynamicListStatus3CondMap[x].logicOperator2 == E_LOGIC_OP_OR)
                {
                    if(bOperand1 || bOperand2 || bOperand3)
                        *targetState = SceneGetDynamicListStatus3CondMap[x].eListState;
                    else
                        *targetState = SceneGetDynamicListStatus3CondMap[x].eElseListState;
                }
            }
            bResult = TRUE;
            break; //search end
        }
    }
    return bResult;
}

GUI_ENUM_DYNAMIC_LIST_STATE MApp_ZUI_ACT_GetDynamicListItemStatusHandler(HWND currentHwnd)
{
    //ZUI_MSG(printf("[ZUI]MApp_ZUI_ACT_GetColorHandler\n"));
    GUI_ENUM_DYNAMIC_LIST_STATE nRetState = EN_DL_STATE_NORMAL;
    Bool bResult = FALSE;
    //custom color handler
    bResult = _MApp_ZUI_ACT_GetItemStatusCustom(currentHwnd, &nRetState);
    if(!bResult)
        bResult =  _MApp_ZUI_ACT_GetItemStatusCommon_NoCondition(currentHwnd, &nRetState);
    if(!bResult)
        bResult =  _MApp_ZUI_ACT_GetItemStatusCommon_1Condition(currentHwnd, &nRetState);
    if(!bResult)
        bResult =  _MApp_ZUI_ACT_GetItemStatusCommon_2Condition(currentHwnd, &nRetState);
    if(!bResult)
        bResult =  _MApp_ZUI_ACT_GetItemStatusCommon_3Condition(currentHwnd, &nRetState);
    
    return nRetState;
}

////////////////////////
//for adjustment temp value
////////////////////////
U16 _MApp_ZUI_ACT_GetAdjustCurrentValue(void)
{
    int x = 0;
    int mapSize = sizeof(SceneAdjustmentMap)/sizeof(SCENE_ADJUSTMENT_HANDLER_MAP);
    for(x = 0; x < mapSize; x++)
    {
        if( SceneAdjustmentMap[x].hwnd == MApp_ZUI_API_GetFocus() && SceneAdjustmentMap[x].get_value_function_ptr != NULL )
        {
            //ZUI_MSG(printf("[ZUI] _MApp_ZUI_ACT_GetAdjustCurrentValue, x=%d, CurrentValue = %d\n", x ,SceneAdjustmentMap[x].get_value_function_ptr()));
            return SceneAdjustmentMap[x].get_value_function_ptr();
        }
    }

    mapSize = sizeof(SceneSelectionMap)/sizeof(SCENE_SELECTION_MAP);
    for(x = 0; x < mapSize; x++)
    {
        if( SceneSelectionMap[x].hwnd == MApp_ZUI_API_GetFocus() && SceneSelectionMap[x].get_value_function_ptr != NULL)
        {
            //ZUI_MSG(printf("[ZUI] _MApp_ZUI_ACT_GetAdjustCurrentValue, x=%d, CurrentValue = %d\n", x ,SceneSelectionMap[x].get_value_function_ptr()));
            return SceneSelectionMap[x].get_value_function_ptr();
        }
    }

    //ZUI_MSG(printf("[ZUI]MApp_ZUI_ACT_GetAdjustCurrentValue end\n"));
    return 0;
}

//public API for Adjustment Temp Value
void MApp_ZUI_ACT_InitAdjustTempValue(void)
{
    AdjustTempValue.bInitial = true;
    AdjustTempValue.tempValue = _MApp_ZUI_ACT_GetAdjustCurrentValue() ;
}

void MApp_ZUI_ACT_ResetAdjustTempValue(void)
{
    AdjustTempValue.bInitial = false;
    AdjustTempValue.tempValue = 0;
}

bool MApp_ZUI_ACT_RestoreAdjustTempValue(void)
{
    int x = 0;
    //for selection map
    int mapSize = sizeof(SceneSelectionMap)/sizeof(SCENE_SELECTION_MAP);
    for(x = 0; x < mapSize; x++)
    {
        if( SceneSelectionMap[x].hwnd == MApp_ZUI_API_GetFocus() )
        {
            ZUI_MSG(printf("[ZUI]hwnd = %d\n", MApp_ZUI_API_GetFocus()));
            U16 tempValue = MApp_ZUI_ACT_GetAdjustTempValue();
            SceneSelectionMap[x].select_function_ptr(tempValue);
            ZUI_MSG(printf("[ZUI]MApp_ZUI_ACT_RestoreAdjustTempValue success\n"));
            return TRUE;
        }
    }

    mapSize = sizeof(SceneAdjustmentMap)/sizeof(SCENE_ADJUSTMENT_HANDLER_MAP);

    //for adjustment map
    for(x = 0; x < mapSize; x++)
    {
        if( SceneAdjustmentMap[x].hwnd == MApp_ZUI_API_GetFocus() )
        {
            U16 tempValue = MApp_ZUI_ACT_GetAdjustTempValue();
            U16 currentValue = _MApp_ZUI_ACT_GetAdjustCurrentValue();
            int distance = tempValue -  currentValue;
            
            //adjust value
            int count;
            for( count = 0; count < abs(distance); count ++)
            {
                if(distance > 0)
                    SceneAdjustmentMap[x].adjust_function_ptr(E_ADJUST_INCREASE); //increase
                else
                    SceneAdjustmentMap[x].adjust_function_ptr(E_ADJUST_DECREASE);//decrease
            }
            ZUI_MSG(printf("[ZUI]MApp_ZUI_ACT_RestoreAdjustTempValue success\n"));
            return TRUE;
        }
    }
    
    ZUI_MSG(printf("[ZUI]failed MApp_ZUI_ACT_InitAdjustTempValue\n"));
    return FALSE;
}

U16 MApp_ZUI_ACT_GetAdjustTempValue(void)
{
    if(AdjustTempValue.bInitial == false)
    {
        UNUSED(AdjustTempValue.bInitial);
        ZUI_MSG(printf("[ZUI] fail : MApp_ZUI_ACT_GetAdjustTempValue AdjustTempValue.bInitial== false\n"));
    }
    ZUI_MSG(printf("[ZUI] MApp_ZUI_ACT_GetAdjustTempValue AdjustTempValue.tempValue = %d\n",AdjustTempValue.tempValue));
    return AdjustTempValue.tempValue;
}
////////////////////////////
//for adjustment temp value end
////////////////////////////

HWND MApp_ZUI_ACT_GetCurrentSceneFocus(EN_SCENE_TYPE curSceneTYpe)
{
    int x;
    int mapSize = sizeof(SceneSelectionMap)/sizeof(SCENE_SELECTION_MAP);

    for(x = 0; x< mapSize; x++)
    {
#if 0        
        ZUI_MSG(printf("MApp_ZUI_ACT_GetCurrentSceneFocus sceneType = %d\n", SceneSelectionMap[x].sceneType));
        ZUI_MSG(printf("MApp_ZUI_ACT_GetCurrentSceneFocus parameter = %d\n", SceneSelectionMap[x].parameter));
        if(SceneSelectionMap[x].get_value_function_ptr != NULL)
            ZUI_MSG(printf("MApp_ZUI_ACT_GetCurrentSceneFocus get_value = %d\n", SceneSelectionMap[x].get_value_function_ptr()));
#endif        
        if( (SceneSelectionMap[x].sceneType == curSceneTYpe) && SceneSelectionMap[x].get_value_function_ptr != NULL
            && (SceneSelectionMap[x].parameter == SceneSelectionMap[x].get_value_function_ptr() ) )
        {
            ZUI_MSG(printf("MApp_ZUI_ACT_GetCurrentSceneFocus get_value = %d hwnd = %d\n", SceneSelectionMap[x].get_value_function_ptr() , SceneSelectionMap[x].hwnd));
            
            return SceneSelectionMap[x].hwnd;
        }
    }
    return HWND_INVALID;
}


void _MApp_ZUI_ACT_ReturnSceneSaveSetting(void)
{
    int x;
    EN_SCENE_TYPE curScene = MApp_ZUI_ACT_GetCurrentSceneType() ;
    
    for(x = 0; x < SCENE_TYPE_MAX; x++)
    {
        //search end
        if(SceneReturnSaveSettingMap[x].sceneType == SCENE_NULL)
            break;
        
        if( SceneReturnSaveSettingMap[x].sceneType == curScene && SceneSaveSettingHandler.save_monitor_setting_ptr != NULL)
        {
            ZUI_MSG(printf(" _MApp_ZUI_ACT_ReturnSceneSaveSetting , sceneType = %d\n", curScene));
            SceneSaveSettingHandler.save_monitor_setting_ptr();
            break;
        }
    }
}

#undef MAPP_ZUI_BM_ACTIONMANAGER_C


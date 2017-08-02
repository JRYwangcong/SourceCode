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

#define MAPP_ZUI_BM_OSDMANAGER_C
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
#include "UserPrefDef.h"
#include "MApp_ZUI_ACTMainMenu.h"
#include "MApp_ZUI_BM_OSDManager.h"
#include "MApp_ZUI_BM_OSDTables.h"
#include "MApp_ZUI_BM_ActionManager.h"
#include "MApp_ZUI_BM_CustomTables.h"
#include "MApp_ZUI_ACTglobal.h"
#include "MApp_GlobalFunction.h"
#include "menudef.h"
#include "menufunc.h"
#include "OsdDefault.h"
#include "Global.h"
#include "Mode.h"
#include "OsdDefault.h"
#include "msScaler.h"
#include "appWindow.h"

static SCENE_FLOW_PATH SceneFlowPath;
static BOOL bAutoResetScenePath = TRUE; // Auto Reset for Open OSD
//Bool g_bReturnPathAutoTextColor = RETURNPATH_AUTO_TEXTCOLOR_ENABLE;
//OSD_COLOR g_returnPathTextColor = RETURNPATH_AUTO_TEXTCOLOR;

extern BOOLEAN _MApp_ZUI_API_AllocateVarData(void);

HWND _MApp_ZUI_ACT_GetAutoTextColorHwnd(EN_SCENE_TYPE sceneType);
void _MApp_ZUI_ACT_DynamicSetSceneFocus(EN_SCENE_TYPE sceneType);
BOOLEAN _MApp_ZUI_ACT_OSDProcess(EN_SCENE_TYPE sceneType);
//-------------------------------------------------------------------------------------------------
// Function
//-------------------------------------------------------------------------------------------------
void MApp_ZUI_ACT_ResetSceneFlowPath(void)
{
    ZUI_MSG(printf("[ZUI]_MApp_ZUI_ACT_ResetSceneFlowPath , reset \n"));
    SceneFlowPath.currentHwndIndex = 0;
    int x=0;
    for(x = 0; x < SCENE_FLOW_PATH_MAX; x++ )
    {
        SceneFlowPath.sceneType[x] = SCENE_TYPE_MAX;
        SceneFlowPath.previousFocusHwnd[x] = HWND_INVALID;
        SceneFlowPath.autoTextColorHwnd[x] = HWND_INVALID;
    }
}

void _MApp_ZUI_ACT_AddSceneFlowPath(EN_SCENE_TYPE sceneType, HWND focusHwnd)
{
    ZUI_MSG(printf("[ZUI]_MApp_ZUI_ACT_AddSceneFlowPath , sceneType = %d \n", sceneType));
    if(SceneFlowPath.currentHwndIndex < SCENE_FLOW_PATH_MAX)
    {
        SceneFlowPath.sceneType[SceneFlowPath.currentHwndIndex] = sceneType;
        SceneFlowPath.previousFocusHwnd[SceneFlowPath.currentHwndIndex] = focusHwnd;
        SceneFlowPath.autoTextColorHwnd[SceneFlowPath.currentHwndIndex] = _MApp_ZUI_ACT_GetAutoTextColorHwnd(sceneType);
        SceneFlowPath.currentHwndIndex++;
    }
    else
    {
        ZUI_DBG_FAIL(printf("[ZUI]_MApp_ZUI_ACT_AddSceneFlowPath , overflow \n"));
    }
}

EN_SCENE_TYPE MApp_ZUI_ACT_GetCurrentSceneType(void)
{
    if(SceneFlowPath.currentHwndIndex > 0)
        return SceneFlowPath.sceneType[SceneFlowPath.currentHwndIndex - 1];
    return SCENE_TYPE_MAX;

}

EN_SCENE_NEXT_PAGE_EXEC_ACT MApp_ZUI_ACT_GetSceneExecAct(EN_OSD_WNDEXEACTION_ENUM act)
{
    int x = 0;
    for(x = 0; x < EN_SCENE_NP_EXE_ACT_MAX; x ++)
    {
        if(SceneNextPageExecActMap[x].osdAct == act)
            return SceneNextPageExecActMap[x].sceneAct;
    }

    ZUI_DBG_FAIL(printf("[ERROR] MApp_ZUI_ACT_GetSceneExecAct,scene act = %d\n", act));

    return EN_SCENE_NP_EXE_ACT_MAX;
    
}

HWND MApp_ZUI_ACT_GetSceneFocus(EN_SCENE_TYPE sceneType)
{
    int x = 0;
    for(x =0; x < SCENE_TYPE_MAX; x ++)
    {
        if(SceneOpenCommonInitMap[x].sceneType == sceneType)
            return SceneOpenCommonInitMap[x].hwndFocus;
            
    }
    return HWND_INVALID;
}

SCENE_NEXT_PAGE_MAP MApp_ZUI_ACT_GetNextScenePage(HWND hwnd, EN_OSD_WNDEXEACTION_ENUM execAct)
{
    int x = 0, y = 0;
    EN_SCENE_NEXT_PAGE_EXEC_ACT sceneAct = MApp_ZUI_ACT_GetSceneExecAct(execAct);
    SCENE_NEXT_PAGE_MAP sceneMap = {0,SCENE_TYPE_MAX,0};
    HWND nextHwnd = hwnd;

    //if hwnd is disable, looking for next hwnd,
    for(x = 0; x < SCENE_NEXT_LOOKUP_MAX ; x++)
    {
        for(y = 0; y < SCENE_NEXT_PAGE_MAP_MAX; y++)
        {
            if(SceneNextPageMap[sceneAct][y].hwnd == 0)
            {
                break;
            }
            if(sceneAct == EN_SCENE_NP_EXE_ACT_MAX)
            {
                ZUI_MSG(printf("[ERROR] MApp_ZUI_ACT_GetNextScenePage, sceneAct = EN_SCENE_NP_EXE_ACT_MAX \n"));
                break;
            }
            
            if(SceneNextPageMap[sceneAct][y].hwnd == nextHwnd)
            {
                ZUI_MSG(printf("NextScenePage,scene type = %d, hwnd = %d, execAct =%d \n", SceneNextPageMap[sceneAct][y].nextPageSceneType, nextHwnd, execAct));
                sceneMap.nextPageSceneType = SceneNextPageMap[sceneAct][y].nextPageSceneType;
                sceneMap.bResetFlowPath = SceneNextPageMap[sceneAct][y].bResetFlowPath;
                break;
            }
        }
        
        if(sceneMap.nextPageSceneType != SCENE_TYPE_MAX)
        {
            if(MApp_ZUI_API_IsWindowVisible(  SceneOpenCommonInitMap[sceneMap.nextPageSceneType].hwndFocus) &&
                MApp_ZUI_API_IsWindowEnabled(  SceneOpenCommonInitMap[sceneMap.nextPageSceneType].hwndFocus))
            {
                ZUI_MSG(printf("[SUCCESS] MApp_ZUI_ACT_GetNextScenePage, sceneMap.nextPageSceneType = %d \n",sceneMap.nextPageSceneType));
                break;
            }
            else
            {
                //for null focus hwnd
                if(SceneOpenCommonInitMap[sceneMap.nextPageSceneType].hwndFocus == HWND_NULL)
                    break;
                
                nextHwnd =  SceneOpenCommonInitMap[sceneMap.nextPageSceneType].hwndFocus;
                ZUI_MSG(printf("MApp_ZUI_ACT_GetNextScenePage, nextHwnd = %d \n",nextHwnd));
            }
        }
    }
    return sceneMap;
}

HWND _MApp_ZUI_ACT_GetAutoTextColorHwnd(EN_SCENE_TYPE sceneType)
{
    int x = 0;
    int mapSize = sizeof( SceneAutoTextColorMap) / sizeof(SCENE_AUTO_TEXT_COLOR_MAP);
    HWND hwndResult = HWND_NULL;
    
    for(x = 0; x < mapSize; x ++)
    {
        if(SceneAutoTextColorMap[x].sceneType ==  sceneType)
        {
            hwndResult =  SceneAutoTextColorMap[x].hwndTextColor;
            break;
        }
    }
    return hwndResult;
}

bool _MApp_ZUI_ACT_OpenSceneHandler_NoSceneFlowPath(EN_SCENE_TYPE sceneType)
{
    bool bCommonResult = false;
    bool bCustomResult = false;

    ZUI_MSG(printf("open menu, type = %d\n", sceneType));
    // force reset adjustment temp value
    MApp_ZUI_ACT_ResetAdjustTempValue();

    //check next/previous OSD id
    if( _MApp_ZUI_ACT_OSDProcess(sceneType) == true)
        return true;

    // common initialization
    bCommonResult = MApp_ZUI_ACT_OpenSceneCommon(sceneType);

    // custom initialization
    bCustomResult = MApp_ZUI_ACT_OpenSceneCustom(sceneType);

    if(bCommonResult == true || bCustomResult == true)
    {
        //init adjustment temp value, if menu is not adjustment menu, it will not initial temp value.
        MApp_ZUI_ACT_InitAdjustTempValue();
        return true;
    }
    else
    {
        ZUI_MSG(printf("[FAIL] open menu fail, shutdown OSD , menu type = %d\n", sceneType));
        MApp_ZUI_ACT_ShutdownOSD();
    }
    return false;

}

void MApp_ZUI_ACT_OpenSceneHandler(EN_SCENE_TYPE sceneType)
{
    HWND focusHwnd = MApp_ZUI_API_GetFocus();

    if( _MApp_ZUI_ACT_OpenSceneHandler_NoSceneFlowPath(sceneType) )
    {
        _MApp_ZUI_ACT_AddSceneFlowPath(sceneType, focusHwnd);
    }
}

BOOLEAN MApp_ZUI_ACT_OpenSceneCommon(EN_SCENE_TYPE sceneType)
{
    int x = 0;
    bool bResult = false;
    for(x = 0; x < SCENE_TYPE_MAX; x++)
    {
        if(SceneOpenCommonInitMap[x].sceneType == SCENE_NULL)
            break;
        
        if(SceneOpenCommonInitMap[x].sceneType == sceneType)
        {
            ZUI_MSG(printf("[Success] MApp_ZUI_ACT_OpenSceneCommon,sceneType=%d, hwndShow=%d, focus = %d, hwnd1 =%d, Hwnd2 = %d\n",
                sceneType,SceneOpenCommonInitMap[x].hwndShow, SceneOpenCommonInitMap[x].hwndFocus, SceneOpenCommonInitMap[x].hwndShow1, SceneOpenCommonInitMap[x].hwndShow2));
     
            
            if(SceneOpenCommonInitMap[x].hwndShow != HWND_NULL)
            {
                MApp_ZUI_API_ShowWindow(HWND_MAINFRAME, SW_HIDE);
                MApp_ZUI_API_ShowWindow(HWND_TRANSPARENT, SW_SHOW); //force set default transparent hwnd to 1 in Skintool, here we will show the transparent hwnd
                MApp_ZUI_API_ShowWindow(SceneOpenCommonInitMap[x].hwndShow, SW_SHOW);
            }
            //else{}   // if hwndShow == HWND_NULL, it means keep previous status of scene 

            if(SceneOpenCommonInitMap[x].hwndShow1 != HWND_NULL)
                MApp_ZUI_API_ShowWindow(SceneOpenCommonInitMap[x].hwndShow1, SW_SHOW);
            if(SceneOpenCommonInitMap[x].hwndShow2 != HWND_NULL)
                MApp_ZUI_API_ShowWindow(SceneOpenCommonInitMap[x].hwndShow2, SW_SHOW);
            if(SceneOpenCommonInitMap[x].hwndFocus != HWND_NULL)
                MApp_ZUI_API_SetFocus(SceneOpenCommonInitMap[x].hwndFocus);

            
            //set focus by value of runtime, it will get value from driver and set focus
            //if there are no return value, it will keep old focus.
            _MApp_ZUI_ACT_DynamicSetSceneFocus(sceneType);
            bResult = true;
            break;
        }
    }
    return bResult;
}

BOOLEAN MApp_ZUI_ACT_OpenSceneCustom(EN_SCENE_TYPE sceneType)
{
    return CustomOpenSceneHandler.openscene_function_ptr(sceneType);
}

BOOLEAN _MApp_ZUI_ACT_OSDProcess(EN_SCENE_TYPE sceneType)
{
    //check next/previous OSD Id, if next/previous OSD Id is not equal current OSD Id
    //then close current OSD and startup new OSD
    int x = 0;
    bool bResult = false;

    // don't handle it if open first OSD
    if(SceneFlowPath.currentHwndIndex == 0)
        return FALSE;

    for(x = 0; x < SCENE_TYPE_MAX; x++)
    {
        if(SceneOpenCommonInitMap[x].sceneType == SCENE_NULL)
            break;
        
        if(SceneOpenCommonInitMap[x].sceneType == sceneType && SceneOpenCommonInitMap[x].OsdID != MApp_ZUI_GetActiveOSD())
        {
            //startup new OSD
            bAutoResetScenePath = FALSE;
            MApp_ZUI_ACT_StartupOSD(SceneOpenCommonInitMap[x].OsdID);
            bAutoResetScenePath = TRUE;
            bResult = true;
        }
    }
    return bResult;  
}

void _MApp_ZUI_ACT_DynamicSetSceneFocus(EN_SCENE_TYPE sceneType)
{
    HWND hwndFocus =MApp_ZUI_ACT_GetCurrentSceneFocus(sceneType); //search from SceneSelectionMap
    ZUI_MSG(printf(" _MApp_ZUI_ACT_DynamicSetSceneFocus , sceneType = %d, focus = %d\n", sceneType, hwndFocus));
    
    if(hwndFocus != HWND_INVALID)
    {
        MApp_ZUI_API_SetFocus(hwndFocus);
    }
}

Bool MApp_ZUI_ACT_ReturnSceneCustom(void)
{
    return CustomCloseSceneHandler.closescene_function_ptr( MApp_ZUI_ACT_GetCurrentSceneType() );
}

void MApp_ZUI_ACT_ReturnSceneCommon(void)
{
    HWND focusHwnd;
    
    if(SceneFlowPath.currentHwndIndex == 1) // it means current page is Top Menu
    {
        ZUI_MSG(printf("[ZUI]MApp_ZUI_ACT_ReturnSceneHandler , MApp_ZUI_ACT_ShutdownOSD => SceneFlowPath.currentHwndIndex == 1 \n"));            
        MApp_ZUI_ACT_ShutdownOSD();
        return;
    }
    //reset current sceneType and focus hwnd,
    SceneFlowPath.currentHwndIndex--;
    SceneFlowPath.sceneType[SceneFlowPath.currentHwndIndex] = SCENE_TYPE_MAX;
    focusHwnd = SceneFlowPath.previousFocusHwnd[SceneFlowPath.currentHwndIndex];
    SceneFlowPath.previousFocusHwnd[SceneFlowPath.currentHwndIndex] = HWND_INVALID;

    ZUI_MSG(printf("MApp_ZUI_ACT_ReturnSceneHandler return sceneType=%d\n", MApp_ZUI_ACT_GetCurrentSceneType() ));
    _MApp_ZUI_ACT_OpenSceneHandler_NoSceneFlowPath( MApp_ZUI_ACT_GetCurrentSceneType() );

    //set previous focus hwnd
    MApp_ZUI_API_SetFocus(focusHwnd);
}

void MApp_ZUI_ACT_ReturnSceneHandler(void)
{
    if(SceneFlowPath.currentHwndIndex == 0)
    {
        ZUI_MSG(printf("[ZUI Error]MApp_ZUI_ACT_ReturnSceneHandler , SceneFlowPath.currentHwndIndex == 0, do nothing \n"));            
        return;
    }

    //custom end function
    MApp_ZUI_ACT_ReturnSceneCustom();

    //common
    MApp_ZUI_ACT_ReturnSceneCommon();

}

BOOLEAN MApp_ZUI_ACT_GetReturnPathAutoColor(HWND hwnd, OSD_COLOR * colorTarget)
{
    BOOLEAN bResult = FALSE;
    UNUSED(hwnd);
    UNUSED(colorTarget);

#if (RETURNPATH_AUTO_TEXTCOLOR_ENABLE == TRUE)
    {
        int x;
        for(x = 0; x < SceneFlowPath.currentHwndIndex ; x++)
        {   
            //printf("autoTextColorHwnd[x] = %d, scene = %d\n",SceneFlowPath.autoTextColorHwnd[x], SceneFlowPath.sceneType[x]);
            if(SceneFlowPath.autoTextColorHwnd[x] == hwnd )
            {
                *colorTarget = RETURNPATH_AUTO_TEXTCOLOR;
                bResult = TRUE;
                break; //search end
            }
        }
    }
#endif
    return bResult;
}

BOOLEAN MApp_ZUI_ACT_InitOSD(E_OSD_ID osd_id, U16 nWidth, U16 nHeight)
{
    RECT rect;
    HWND wnd;

    g_GUI_WindowList = GetWindowListOfOsdTable(osd_id);
    g_GUI_WinDrawStyleList = GetWindowStyleOfOsdTable(osd_id);
    g_GUI_WindowPositionList = GetWindowPositionOfOsdTable(osd_id);
#if ZUI_ENABLE_ALPHATABLE
    g_GUI_WinAlphaDataList = GetWindowAlphaDataOfOsdTable(osd_id);
#endif
    HWND_MAX = GetWndMaxOfOsdTable(osd_id);
    OSDPAGE_BLENDING_ENABLE = IsBlendingEnabledOfOsdTable(osd_id);
    OSDPAGE_BLENDING_VALUE = GetBlendingValueOfOsdTable(osd_id);
    if (!_MApp_ZUI_API_AllocateVarData())
    {
        ZUI_DBG_FAIL(printf("[ZUI]ALLOC\n"));
        ABORT();
        return TRUE;
    }

    RECT_SET(rect,
        (gScreenWidth-nWidth)/2, (gScreenHeight-nHeight)/2,
        nWidth, nHeight);
    if (!MApp_ZUI_API_InitGDI(&rect))
    {
        ZUI_DBG_FAIL(printf("[ZUI]GDIINIT\n"));
        ABORT();
        return TRUE;
    }

    for (wnd = 0; wnd < HWND_MAX; wnd++)
    {
        //printf("create msg: %lu\n", (U32)wnd);
        MApp_ZUI_API_SendMessage(wnd, MSG_CREATE, 0);
    }
    return FALSE;
}

void MApp_ZUI_ACT_OpenOSDHandler(E_OSD_ID eOSD_ID)
{
    if(eOSD_ID >= E_OSD_MAX )
    {
        ZUI_DBG_FAIL(printf("[ZUI]OpenOSDHandler eOSD_ID >= E_OSD_MAX,  eOSD_ID = %d\n", eOSD_ID));
        ABORT();
    }

    if( MApp_ZUI_ACT_InitOSD(eOSD_ID, OSDInitMap[eOSD_ID].nWidth, OSDInitMap[eOSD_ID].nHeight) == TRUE )
    {
        ZUI_DBG_FAIL(printf("[ZUI] MApp_ZUI_ACT_OpenOSD INIT FAIL, eOSD_ID = %d\n", eOSD_ID));
        return;
    }
    if(bAutoResetScenePath == TRUE)
        MApp_ZUI_ACT_ResetSceneFlowPath();

    MApp_ZUI_ACT_OpenSceneHandler(OSDInitMap[eOSD_ID].eScene);

    MApp_ZUI_ACT_TransitionEffectBegin(EN_EFFMODE_OPEN, E_ZUI_STATE_RUNNING);
}

void MApp_ZUI_ACT_TerminateOSDHandler(E_OSD_ID eOSD_ID)
{
    if(CustomTerminateOSDHandler.terminateosd_function_ptr != NULL)
    {
        CustomTerminateOSDHandler.terminateosd_function_ptr(eOSD_ID);
    }
    else
    {
        ZUI_DBG_FAIL(printf("[ZUI] MApp_ZUI_ACT_TerminateOSDHandler, ptr == NULL\n"));
        ABORT();
    }
}

BOOLEAN MApp_ZUI_ACT_GlobalKeyHandler(E_OSD_ID eOSD_ID, VIRTUAL_KEY_CODE key)
{
    if(CustomGlobalKeyHandler.globalkey_function_ptr != NULL)    
    {
        return CustomGlobalKeyHandler.globalkey_function_ptr(eOSD_ID, key);
    }
    else
    {
        ZUI_DBG_FAIL(printf("[ZUI] MApp_ZUI_ACT_GlobalKeyHandler, ptr == NULL\n"));
        ABORT();
    }

    return FALSE;
}

BOOLEAN MApp_ZUI_ACT_AutoCloseHandler(void)
{
    if(CustomAutoCloseHandler.closeosd_function_ptr != NULL)
    {
        return CustomAutoCloseHandler.closeosd_function_ptr();
    }
    else
    {
        ZUI_DBG_FAIL(printf("[ZUI] MApp_ZUI_ACT_AutoCloseHandler, ptr == NULL\n"));
        ABORT();
    }
    return FALSE;
}

BOOLEAN MApp_ZUI_ACT_CustomActionHandler(EN_OSD_WNDEXEACTION_ENUM execAct)
{
    if(CustomActionHandler.action_function_ptr != NULL)
    {
        return CustomActionHandler.action_function_ptr(execAct);
    }
    else
    {
        ZUI_DBG_FAIL(printf("[ZUI] MApp_ZUI_ACT_CustomActionHandler, ptr == NULL\n"));
        ABORT();
    }
}

void MApp_ZUI_ACT_CustomSelectionHandler(HWND hwnd)
{
    if(CustomSelectionHandler.selection_function_ptr != NULL)
    {
        CustomSelectionHandler.selection_function_ptr(hwnd);
    }
    else
    {
        ZUI_DBG_FAIL(printf("[ZUI] MApp_ZUI_ACT_CustomSelectionHandler, ptr == NULL\n"));
        ABORT();
    }
}

LPTSTR MApp_ZUI_ACT_DynamicTextHandler(HWND hwnd)
{
    LPTSTR nRetStr = NULL; 
    if(CustomDynamicTextHandler.dynamictext_function_ptr != NULL)
    {
        nRetStr = CustomDynamicTextHandler.dynamictext_function_ptr(hwnd);
    }
    else
    {
        ZUI_DBG_FAIL(printf("[ZUI] MApp_ZUI_ACT_DynamicTextHandler, ptr == NULL\n"));
        ABORT();
    }

    return nRetStr;
}

U16 MApp_ZUI_ACT_DynamicBitmapHandler(HWND hwnd, DRAWSTYLE_TYPE type)
{
    U16 nResult = E_BMP_EMPTY; 
    if(CustomDynamicBitmapHandler.dynamicbitmap_function_ptr != NULL)
    {
        nResult = CustomDynamicBitmapHandler.dynamicbitmap_function_ptr(hwnd, type);
    }
    else
    {
        ZUI_DBG_FAIL(printf("[ZUI] MApp_ZUI_ACT_DynamicBitmapHandler, ptr == NULL\n"));
        ABORT();
    }

    return nResult;    
}

S16 MApp_ZUI_ACT_DynamicValueHandler(HWND hwnd)
{
    //ZUI_MSG(printf("[ZUI]MApp_ZUI_ACT_DynamicValueHandler currentHwnd = %d\n", hwnd));
    S16 nResult = 0xEFFF; 
    if(CustomDynamicValueHandler.dynamicvalue_function_ptr != NULL)
    {
        nResult = CustomDynamicValueHandler.dynamicvalue_function_ptr(hwnd);
    }
    else
    {
        ZUI_DBG_FAIL(printf("[ZUI] MApp_ZUI_ACT_DynamicValueHandler, ptr == NULL\n"));
        ABORT();
    }
    return nResult;    
}

BOOL MApp_ZUI_ACT_DynamicColorHandler(HWND hwnd, DRAWSTYLE_TYPE type, OSD_COLOR colorOriginal, OSD_COLOR* colorTarget)
{
    //ZUI_MSG(printf("[ZUI]MApp_ZUI_ACT_DynamicValueHandler currentHwnd = %d\n", hwnd));
    BOOL bResult = FALSE; 
    if(CustomDynamicColorHandler.dynamiccolor_function_ptr != NULL)
    {
        bResult = CustomDynamicColorHandler.dynamiccolor_function_ptr(hwnd, type, colorOriginal, colorTarget);
    }
    else
    {
        ZUI_DBG_FAIL(printf("[ZUI] MApp_ZUI_ACT_DynamicColorHandler, ptr == NULL\n"));
        ABORT();
    }
    return bResult;    
}

BOOL MApp_ZUI_ACT_DynamicListItemHandler(HWND hwnd, GUI_ENUM_DYNAMIC_LIST_STATE* targetState)
{
    //ZUI_MSG(printf("[ZUI]MApp_ZUI_ACT_DynamicListItemHandler hwnd = %d\n", hwnd));
    BOOL bResult = FALSE; 
    if(CustomDynamicListItemHandler.dynamiclisttem_function_ptr!= NULL)
    {
        bResult = CustomDynamicListItemHandler.dynamiclisttem_function_ptr(hwnd, targetState);
    }
    else
    {
        ZUI_DBG_FAIL(printf("[ZUI] MApp_ZUI_ACT_DynamicListItemHandler, ptr == NULL\n"));
        ABORT();
    }
    return bResult;    
}
#undef MAPP_ZUI_BM_OSDMANAGER_C


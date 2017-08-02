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

#define MAPP_ZUI_ACTMAINMENU_C
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
#include "MApp_ZUI_ACTglobal.h"
#include "MApp_GlobalFunction.h"
#include "MApp_LoadFontInit.h"
#include "UserPrefDef.h"
#include "menudef.h"
#include "menufunc.h"
//#include "MApp_ZUI_menufunc.h"
#include "OsdDefault.h"
#include "Global.h"
#include "Mode.h"
#include "OsdDefault.h"
#include "msScaler.h"
#include "appWindow.h"
#include "UserPref.h"

//ZUI BM
#include "MApp_ZUI_ACTCustom.h"
#include "MApp_ZUI_BM_OSDManager.h"

LPTSTR MApp_ZUI_Custom_MainmenuGetDynamicText(HWND hwnd)
{
    U16 u16TempID = Empty;

    switch(hwnd)
    {
        case HWND_MULTI_WIN_INFO_W1_SRC_NUM:
            if ( !CableNotConnectedFlag(USER_PREF_WIN_SRC(DISPLAY_WIN1) ) )
                u16TempID = _MApp_ZUI_Custom_GetSourceStringID(DISPLAY_WIN1);
            break;
        case HWND_MULTI_WIN_INFO_W2_SRC_NUM:
            if( (USER_PREF_MULTI_WIN_MODE >= eMULTIWIN_2_PIP)&& (!CableNotConnectedFlag(USER_PREF_WIN_SRC(DISPLAY_WIN2))))
                    u16TempID = _MApp_ZUI_Custom_GetSourceStringID(DISPLAY_WIN2);
            break;
        case HWND_MULTI_WIN_INFO_W3_SRC_NUM:
            if((USER_PREF_MULTI_WIN_MODE >= eMULTIWIN_3) && (!CableNotConnectedFlag(USER_PREF_WIN_SRC(DISPLAY_WIN3))))
                    u16TempID = _MApp_ZUI_Custom_GetSourceStringID(DISPLAY_WIN3);
            break;
        case HWND_MULTI_WIN_INFO_W4_SRC_NUM:
            if((USER_PREF_MULTI_WIN_MODE >= eMULTIWIN_4) && (!CableNotConnectedFlag(USER_PREF_WIN_SRC(DISPLAY_WIN4))))
                    u16TempID = _MApp_ZUI_Custom_GetSourceStringID(DISPLAY_WIN4);
            break;

        case HWND_MAIN_MENU_TITLE_RES_NUM:
            _MApp_ZUI_Custom_GetResolutionString(CHAR_BUFFER ,USER_PREF_WIN_SEL);
            return CHAR_BUFFER;

        case HWND_MULTI_WIN_INFO_W1_RESOLUTION_NUM:
            if(!SyncLossState(USER_PREF_WIN_SRC(DISPLAY_WIN1)))
            {
                _MApp_ZUI_Custom_GetResolutionString(CHAR_BUFFER,DISPLAY_WIN1);
                return CHAR_BUFFER;
            }            
            break;    
        case HWND_MULTI_WIN_INFO_W2_RESOLUTION_NUM:
            if((USER_PREF_MULTI_WIN_MODE >= eMULTIWIN_2_PIP)&&(!SyncLossState(USER_PREF_WIN_SRC(DISPLAY_WIN2))) )
            {
                _MApp_ZUI_Custom_GetResolutionString(CHAR_BUFFER,DISPLAY_WIN2);
                return CHAR_BUFFER;
            }            
            break;
        case HWND_MULTI_WIN_INFO_W3_RESOLUTION_NUM:
            if((USER_PREF_MULTI_WIN_MODE >= eMULTIWIN_3)&&(!SyncLossState(USER_PREF_WIN_SRC(DISPLAY_WIN3))))
            {
                _MApp_ZUI_Custom_GetResolutionString(CHAR_BUFFER,DISPLAY_WIN3);
                return CHAR_BUFFER;
            }            
            break;
        case HWND_MULTI_WIN_INFO_W4_RESOLUTION_NUM:
            if((USER_PREF_MULTI_WIN_MODE >= eMULTIWIN_4)&&(!SyncLossState(USER_PREF_WIN_SRC(DISPLAY_WIN4))))
            {
                _MApp_ZUI_Custom_GetResolutionString(CHAR_BUFFER,DISPLAY_WIN4);
                return CHAR_BUFFER;
            }            
            break;

        default:
            break;
    }


    if (u16TempID != Empty)
        return MApp_ZUI_API_GetString(u16TempID);

    return 0; //for empty string....  
}

void _MApp_ZUI_Custom_GetResolutionString(U16 *strBuffer,U8 WIN_Sel)
{
    U16 H_Width,V_Hight;
    U8 v_fre;
    char locbuf[20];

    //WIN_Sel = 0;
    if(INPUT_IS_VGA(MapWin2Port(WIN_Sel)))
    {
        H_Width=StandardModeWidth(MapWin2Port(WIN_Sel));
        V_Hight=StandardModeHeight(MapWin2Port(WIN_Sel));
        v_fre=(StandardModeVFreq(MapWin2Port(WIN_Sel))+5)/10;
    }
    else
    {
        H_Width = msDrvIP1ReadAutoWidth(MapPort2DetScIndex(MapWin2Port(WIN_Sel)));
        V_Hight = msDrvIP1ReadAutoHeight(MapPort2DetScIndex(MapWin2Port(WIN_Sel)));
        v_fre = VFREQ( HFREQ( SrcHPeriod(MapWin2Port(WIN_Sel)) ), SrcVTotal(MapWin2Port(WIN_Sel)));
    }
    memset(locbuf, 0, sizeof(locbuf));
    snprintf(locbuf,sizeof(locbuf), "%ux%u@%uHz", H_Width, V_Hight, v_fre);
    MApp_U8StringToU16String((U8*)locbuf, strBuffer, strlen((char *)locbuf));
    //strcpy((char *)strBuffer,locbuf);
}

U16 _MApp_ZUI_Custom_GetSourceStringID(U8 Win_Num)
{
    U16 u16TempID= Empty;

    if(INPUT_VGA==MapWin2Port(Win_Num))
        u16TempID = en_str_Source_VGA;
    else if(INPUT_DVI==MapWin2Port(Win_Num))
        u16TempID = en_str_Source_DVI;
    else if(INPUT_HDMI==MapWin2Port(Win_Num))
        u16TempID = en_str_HDMI_1;
    else if(INPUT_HDMI2ND==MapWin2Port(Win_Num))
        u16TempID = en_str_HDMI_2;
    else if(INPUT_DISPLAYPORT==MapWin2Port(Win_Num))
        u16TempID = en_str_Display_Port_1;
    else if(INPUT_DISPLAYPORT2ND==MapWin2Port(Win_Num))
        u16TempID = en_str_Display_Port_2;
    else
        u16TempID = Empty;
   return u16TempID;
}

#undef MAPP_ZUI_ACTIDLE_C

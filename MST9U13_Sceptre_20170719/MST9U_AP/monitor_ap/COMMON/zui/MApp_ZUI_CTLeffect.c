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

#define MAPP_ZUI_CTL_EFFECT_C
#define _ZUI_INTERNAL_INSIDE_ //NOTE: for ZUI internal


//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
//#include "msAPI_Timer.h"
#include "MsTypes.h"
#include "apiGOP.h"
#include "MApp_ZUI_APIcommon.h"
#include "MApp_ZUI_APItables.h"
#include "MApp_ZUI_APIcontrols.h"
#include "MApp_ZUI_APIstyletables.h"
#include "MApp_ZUI_CTLeffect.h"
#include "MApp_ZUI_CTLeffectpopup.h"
#include "MApp_ZUI_CTLeffectslideitem.h"
#include "MApp_ZUI_CTLeffectzoom.h"
#include "MApp_ZUI_CTLeffectroll.h"
#include "MApp_ZUI_CTLmotiontrans.h"
#include "MApp_ZUI_CTLfake3d.h"

//EFF_3D_SETTING_CTL stEff3DSettingCTL ={EN_3D_MAIN_PAGE_Fake3D, EN_3D_MAIN_MENU_ITEM_ON, EN_3D_POPUP_DIALOG_ON};

extern BOOLEAN _MApp_ZUI_API_WindowProcOnIdle(void);

#if 0
void MApp_ZUI_CTL_Set3DEffectSettingPara(S8 s8Para, EN_3D_SETTING_CTL_PARA ePara)
{
    if(ePara >= EN_3D_CTL_INVALID_PARA)
    {
        printf("Error!! invalid input parameter to MApp_ZUI_CTL_Set3DEffectSettingPara()\n");
    }

    switch(ePara)
    {
        case EN_3D_MAIN_PAGE_PARA:
            stEff3DSettingCTL.eMainPageEff = (EN_3D_MAIN_PAGE) s8Para;
            break;
        case EN_3D_MAIN_MENU_ITEM_PARA:
            stEff3DSettingCTL.eMainMenuItemEff = (EN_3D_MAIN_MENU_ITEM) s8Para;
            break;
        case EN_3D_POPUP_DIALOG_PARA:
            stEff3DSettingCTL.ePopupDialogEff = (EN_3D_POPUP_DIALOG) s8Para;
            break;
        case EN_3D_CTL_INVALID_PARA:
            break;
    }
}

S8 MApp_ZUI_CTL_Get3DEffectSettingPara(EN_3D_SETTING_CTL_PARA ePara)
{
    S8 s8Para;
    if(ePara >= EN_3D_CTL_INVALID_PARA)
    {
        printf("Error!! invalid input parameter to MApp_ZUI_CTL_Get3DEffectSettingPara()\n");
    }

    switch(ePara)
    {
        case EN_3D_MAIN_PAGE_PARA:
            s8Para = (S8) stEff3DSettingCTL.eMainPageEff;
            break;
        case EN_3D_MAIN_MENU_ITEM_PARA:
            s8Para = (S8) stEff3DSettingCTL.eMainMenuItemEff;
            break;
        case EN_3D_POPUP_DIALOG_PARA:
            s8Para = (S8) stEff3DSettingCTL.ePopupDialogEff;
            break;
        default:
            s8Para = (S8) stEff3DSettingCTL.eMainPageEff;
            break;
    }
    return s8Para;
}
#endif

S32 MApp_ZUI_CTL_MainPageIconWinProc(HWND hWnd, PMSG pMsg)
{
#if 0
    switch(stEff3DSettingCTL.eMainPageEff)
    {
        case EN_3D_MAIN_PAGE_MotionTrans:
            MApp_ZUI_CTL_MotionTrans_Icon_WinProc(hWnd, pMsg);
            return 0;
        case EN_3D_MAIN_PAGE_Fake3D:
            MApp_ZUI_CTL_Fake3D_Icon_WinProc(hWnd, pMsg);
            return 0;
        default:
            break;
    }
#endif
    return DEFAULTWINPROC(hWnd, pMsg);
}

S32 MApp_ZUI_CTL_MainPageWinProc(HWND hWnd, PMSG pMsg)
{
#if 0
    switch(stEff3DSettingCTL.eMainPageEff)
    {
        case EN_3D_MAIN_PAGE_MotionTrans:
            MApp_ZUI_CTL_MotionTransWinProc(hWnd, pMsg);
            return 0;
        case EN_3D_MAIN_PAGE_Fake3D:
            MApp_ZUI_CTL_Fake3D_WinProc(hWnd, pMsg);
            return 0;
        default:
            break;
    }
#endif
    return DEFAULTWINPROC(hWnd, pMsg);
}

///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_CTL_EffectWinProc
///  Window Proc for "effect popup" control
///
///
///  @param [in]       hWnd HWND     window handle
///  @param [in]       pMsg PMSG     message type
///
///  @return S32 message execute result
///
///  @author MStarSemi @date 2009/2/11
///////////////////////////////////////////////////////////////////////////////

S32 MApp_ZUI_CTL_EffectWinProc(HWND hWnd, PMSG pMsg)
{
#if 1
    switch(pMsg->message)
    {
        case MSG_EFFECT_POPUP:
            return EFFECTPOPUP_WINPROC(hWnd, pMsg);

        case MSG_EFFECT_SLIDEITEM:
            return EFFECTSLIDEITEM_WINPROC(hWnd, pMsg);

        case MSG_EFFECT_ZOOMIN:
        case MSG_EFFECT_ZOOMOUT:
            return EFFECTZOOM_WINPROC(hWnd, pMsg);
            
        case MSG_EFFECT_SPREADOUT:
        case MSG_EFFECT_ROLLUP:
            return EFFECTROLL_WINPROC(hWnd, pMsg);
        default:
            break;
    }
#endif
    return DEFAULTWINPROC(hWnd, pMsg);
}


#undef MAPP_ZUI_CTL_EFFECT_C

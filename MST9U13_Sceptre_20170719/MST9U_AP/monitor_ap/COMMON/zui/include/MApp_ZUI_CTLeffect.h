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

#ifndef _MAPP_ZUI_CTL_EFFECT_H
#define _MAPP_ZUI_CTL_EFFECT_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#ifdef MAPP_ZUI_CTL_EFFECT_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

//////Effect control////////////////////////////////////////////////////
INTERFACE S32 MApp_ZUI_CTL_EffectWinProc(HWND, PMSG);
INTERFACE S32 MApp_ZUI_CTL_MainPageIconWinProc(HWND hWnd, PMSG pMsg);
INTERFACE S32 MApp_ZUI_CTL_MainPageWinProc(HWND hWnd, PMSG pMsg);
#if 0
INTERFACE void MApp_ZUI_CTL_Set3DEffectSettingPara(S8 s8Para, EN_3D_SETTING_CTL_PARA ePara);
INTERFACE S8 MApp_ZUI_CTL_Get3DEffectSettingPara(EN_3D_SETTING_CTL_PARA ePara);
#endif

#define EFFECT_WINPROC MApp_ZUI_CTL_EffectWinProc
#define MAINPAGE_WINPROC MApp_ZUI_CTL_MainPageWinProc
#define MAINPAGEICON_WINPROC MApp_ZUI_CTL_MainPageIconWinProc

///////////////////////////////////////////////////////////////////////////////////////
/// MACRO
///

#undef INTERFACE

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MAPP_ZUI_CTL_EFFECT_H */


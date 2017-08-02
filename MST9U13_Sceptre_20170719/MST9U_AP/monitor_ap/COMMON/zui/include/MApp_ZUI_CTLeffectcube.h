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

#ifndef _MAPP_ZUI_CTL_EFFECTCUBE_H
#define _MAPP_ZUI_CTL_EFFECTCUBE_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#ifdef MAPP_ZUI_CTL_EFFECTCUBE_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

//////Effect Cube control////////////////////////////////////////////////////
typedef enum
{
    EN_EFFECT_CUBE_NONE,
    EN_EFFECT_CUBE_CREATE,
    EN_EFFECT_CUBE_UP,
    EN_EFFECT_CUBE_DOWN,
    EN_EFFECT_CUBE_LEFT,
    EN_EFFECT_CUBE_RIGHT,
    EN_EFFECT_CUBE_MAX,
}EN_EFFECT_CUBE;

typedef struct
{
    EN_EFFECT_CUBE dir;
} GUI_DATA_CUBE_EFFECT_VARDATA;

typedef struct
{
    GUI_DATA_CUBE_EFFECT_VARDATA * pVarData;
} GUI_DATA_CUBE_EFFECT;

INTERFACE S32 MApp_ZUI_CTL_EffectCubeItemWinProc(HWND, PMSG);
INTERFACE S32 MApp_ZUI_CTL_EffectCubeWinProc(HWND, PMSG);

#define EFFECTCUBEITEM_WINPROC MApp_ZUI_CTL_EffectCubeItemWinProc
#define EFFECTCUBE_WINPROC MApp_ZUI_CTL_EffectCubeWinProc


///////////////////////////////////////////////////////////////////////////////////////
/// MACRO
///

#undef INTERFACE

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MAPP_ZUI_CTL_EFFECTCUBE_H */


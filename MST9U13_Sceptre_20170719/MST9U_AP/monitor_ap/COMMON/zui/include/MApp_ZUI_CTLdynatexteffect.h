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

#ifndef _MAPP_ZUI_CTL_DYNATEXT_EFFECT_H
#define _MAPP_ZUI_CTL_DYNATEXT_EFFECT_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#ifdef MAPP_ZUI_CTL_DYNATEXT_EFFECT_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

typedef enum
{
    EN_EFFECT_DIRECTION_UP,
    EN_EFFECT_DIRECTION_DOWN,
    EN_EFFECT_DIRECTION_LEFT,
    EN_EFFECT_DIRECTION_RIGHT,
    EN_EFFECT_DIRECTION_MAX,
}EN_EFFECT_DIRECTION;

typedef struct
{
    EN_EFFECT_DIRECTION dir;
    S8                  step;
    GRAPHIC_DC          dcFg;
} GUI_DATA_DYNATEXT_EFFECT_VARDATA;

typedef struct
{
    U8 steps;
    GUI_DATA_DYNATEXT_EFFECT_VARDATA * pVarData;
} GUI_DATA_DYNATEXT_EFFECT;

//////Dynamic Text control////////////////////////////////////////////////////
INTERFACE S32 MApp_ZUI_CTL_DynamicTextEffectWinProc(HWND, PMSG);

#define DYNAMICTEXTEFFECT_WINPROC MApp_ZUI_CTL_DynamicTextEffectWinProc


///////////////////////////////////////////////////////////////////////////////////////
/// MACRO
///

#undef INTERFACE

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MAPP_ZUI_CTL_DYNATEXT_EFFECT_H */


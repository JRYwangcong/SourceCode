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

#ifndef _MAPP_ZUI_CTL_ANIMATION_H
#define _MAPP_ZUI_CTL_ANIMATION_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include "MApp_ZUI_APItables.h"

#ifdef MAPP_ZUI_CTLANIMATE_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

//////Animation////////////////////////////////////////////////
#define FA_FLAG_REPEAT_ONCE     0x01
#define FA_FLAG_LOOP_BACK       0x02

typedef struct
{
    U8 u8State; //0=invisible, 1=disabled, 2=focus, 3=normal
    S8 s8FramePos, s8FrameStep;
    //U8 u8FocusAniState; //0=none, 1=set focus animation, 2=kill focus animation

} GUI_VARDATA_ANIMATION;


typedef struct
{
    // for variable data
    GUI_VARDATA_ANIMATION *pVardata;

    HWND FirstItem;

    //for disabled state
    U16 * pDisabledFrame; //array of DRAW_BITMAP
    S8 s8DisabledFrameCount;

    //for focus state
    U16 * pFocusFrame; //array of DRAW_BITMAP
    S8 s8FocusFrameCount;

    //for normal state
    U16 * pNormalFrame; //array of DRAW_BITMAP
    S8 s8NormalFrameCount;

    U16 u16RepeatSleepTime, u16FrameSleepTime; //in ms
    U8 u8Flags; //FA_FLAG_REPEAT_ONCE for repeat once
                    //FA_FLAG_LOOP_BACK for loop back playing

    //U16 u16NonFocusBorderWidth, u16NonFocusBorderHeight; //for normal/disabled state
    //U16 u16SetFocusResizeDuration; //for got focus animation (in ms)...
    //U16 u16KillFocusResizeDuration; //for lost focus animation (in ms)...
} GUI_DATA_ANIMATION;

INTERFACE S32 MApp_ZUI_CTL_AnimationWinProc(HWND, PMSG);

#define ANIMATION_WINPROC MApp_ZUI_CTL_AnimationWinProc

//////////////////////////////////////////////////////////////////////////////////
//  MACRO
//


#undef INTERFACE

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MAPP_ZUI_CTL_ANIMATION_H */


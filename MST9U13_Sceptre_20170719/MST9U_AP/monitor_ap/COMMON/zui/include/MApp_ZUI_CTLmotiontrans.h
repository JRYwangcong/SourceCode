////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (MStar Confidential Information!¡L) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _MAPP_ZUI_CTL_MOTIONTRANS_H
#define _MAPP_ZUI_CTL_MOTIONTRANS_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#ifdef MAPP_ZUI_CTL_MOTIONTRANS_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

//////Dynamic List control////////////////////////////////////////////////////

typedef enum
{
    EN_MT_STATE_HIDDEN,
    EN_MT_STATE_NORMAL,
    EN_MT_STATE_DISABLED,
} GUI_ENUM_MOTION_TRANS_STATE;

typedef struct
{
    U8 dir;
    U8 step;
    U8 idx;
    U8 shift1;
    U8 shift2;
    U16 u16CenterLeft;
    U16 u16CenterWidth;
    U16 u16IconX[10];
    U8 u8IconWidth;
} GUI_DATA_MOTION_TRANS_VARDATA;

typedef struct
{
    U16 x;
    U16 y;
    U8 margin;
    U8 icon_interval;
    float icon_scale[10];
    float icon_alpha[10];
    U8 icon_offset_y[10];
    U8 steps;
    GUI_DATA_MOTION_TRANS_VARDATA * pVarData;
} GUI_DATA_MOTION_TRANS;

INTERFACE S32 MApp_ZUI_CTL_MotionTrans_Icon_WinProc(HWND, PMSG);
INTERFACE S32 MApp_ZUI_CTL_MotionTransWinProc(HWND, PMSG);

///////////////////////////////////////////////////////////////////////////////////////
/// MACRO
///

#undef INTERFACE

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MAPP_ZUI_CTL_MOTIONTRANS_H */


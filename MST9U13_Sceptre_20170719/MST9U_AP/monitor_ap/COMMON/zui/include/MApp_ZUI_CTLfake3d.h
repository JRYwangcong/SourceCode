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

#ifndef _MAPP_ZUI_CTL_FAKE3D_H
#define _MAPP_ZUI_CTL_FAKE3D_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#ifdef MAPP_ZUI_CTL_FAKE3D_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

//////Dynamic List control////////////////////////////////////////////////////

typedef enum
{
    EN_F3D_STATE_HIDDEN,
    EN_F3D_STATE_NORMAL,
    EN_F3D_STATE_DISABLED,
} GUI_ENUM_FAKE3D_STATE;

typedef struct
{
    U8 dir;
    U8 step;
    U8 idx;
    U16 u16Width;
    U16 u16Height;
    U16 u16CenterLeft;
    U16 u16CenterWidth;
    U16 u16IconX[10];
    U16 u16IconW[10];
    U8 u8IconWidth;
    U8 dir_l;
    U8 dir_c;
    U8 dir_r;
    U8 step_c;
    U8 step_l;
    U8 step_r;
    U8 step_l2;
    U8 step_r2;
    U8 step_l3;
    U8 step_r3;
    U32 scale_c;
    U32 scale_l;
    U32 scale_r;
    U32 scale_l2;
    U32 scale_r2;
    U32 scale_l3;
    U32 scale_r3;
    U32 sc_array0[10];
    U32 sc_array1[2][10];
    U32 sc_array2[2][10];
    U32 sc_array3[2][10];
} GUI_DATA_FAKE3D_VARDATA;

typedef struct
{
    U16 x;
    U16 y;
    U8 margin;
    U8 icon_interval;
    U16 allsteps;
    U8 steps;
    GUI_DATA_FAKE3D_VARDATA * pVarData;
} GUI_DATA_FAKE3D;

INTERFACE S32 MApp_ZUI_CTL_Fake3D_Icon_WinProc(HWND, PMSG);
INTERFACE S32 MApp_ZUI_CTL_Fake3D_WinProc(HWND, PMSG);

///////////////////////////////////////////////////////////////////////////////////////
/// MACRO
///

#undef INTERFACE

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MAPP_ZUI_CTL_FAKE3D_H */


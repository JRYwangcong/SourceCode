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

#ifndef _MAPP_ZUI_ACTFLIPPAGE_H
#define _MAPP_ZUI_ACTFLIPPAGE_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#ifdef MAPP_ZUI_ACTFLIPPAGE_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

//////Flip Page control////////////////////////////////////////////////////
typedef enum
{
    EN_FLIPPAGE_NONE,
    EN_FLIPPAGE_UP,
    EN_FLIPPAGE_DOWN,
    EN_FLIPPAGE_LEFT,
    EN_FLIPPAGE_RIGHT,
    EN_FLIPPAGE_MAX,
}EN_FLIPPAGE;

typedef struct
{
    HWND hwndCurrent;
    HWND hwndNext;
    HWND hwndNextFocus;
    EN_FLIPPAGE dir;
} GUI_DATA_FLIPPAGE_VARDATA;

typedef struct
{
    GUI_DATA_FLIPPAGE_VARDATA * pVarData;
} GUI_DATA_FLIPPAGE;

/////////////////////////////////////////
// Cutomize Window Procedures...

INTERFACE S32 MApp_ZUI_ACT_FlipPageWinProc(HWND, PMSG);

#define FLIPPAGE_WINPROC             MApp_ZUI_ACT_FlipPageWinProc

/////////////////////////////////////////
// Event Handlers....


#undef INTERFACE

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MAPP_ZUI_ACTFLIPPAGE_H */


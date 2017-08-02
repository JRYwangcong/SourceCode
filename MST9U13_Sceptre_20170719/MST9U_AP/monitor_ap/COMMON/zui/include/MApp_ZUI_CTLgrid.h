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

#ifndef _MAPP_ZUI_CTL_GRID_H
#define _MAPP_ZUI_CTL_GRID_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#ifdef MAPP_ZUI_CTL_GRID_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

typedef LPTSTR (*FN_ZUI_CTL_Grid_GetTextByIndex)(U8 u8Index);
typedef U8 (*FN_ZUI_CTL_Grid_GetIndex)(void);
typedef void (*FN_ZUI_CTL_Grid_SetIndex)(U8 u8Index);

typedef struct
{
    U8 u8CurrentPage;
    U8 u8IndexInPage;
} GUI_DATA_GRID_VARDATA;

typedef struct
{
    U8 *pu8Index;
    U8 u8IndexMin;
    U8 u8IndexMax;
    U8 u8Rows;
    U8 u8Columns;
    FN_ZUI_CTL_Grid_GetIndex fnGetIndex;
    FN_ZUI_CTL_Grid_SetIndex fnSetIndex;
    FN_ZUI_CTL_Grid_GetTextByIndex fnGetText;
    GUI_DATA_GRID_VARDATA * pVarData;
} GUI_DATA_GRID;

INTERFACE S32 MApp_ZUI_CTL_GridWinProc(HWND hWnd, PMSG pMsg);
INTERFACE LPTSTR MApp_ZUI_CTL_GridGetDynamicText(HWND hwndGrid, HWND hwndText);
INTERFACE HWND MApp_ZUI_CTL_Grid_MapIndex2Hwnd(HWND hwndGrid, U8 u8Index);
INTERFACE S8 MApp_ZUI_CTL_Grid_MapHwnd2Index(HWND hwndGrid, HWND hwndText);
INTERFACE void MApp_ZUI_CTL_Grid_SetGridDimension(HWND hwndGrid, U8 u8Rows, U8 u8Columns);
INTERFACE void MApp_ZUI_CTL_Grid_SetIndex(HWND hwndGrid, FN_ZUI_CTL_Grid_GetIndex fnGetIndex, FN_ZUI_CTL_Grid_SetIndex fnSetIndex, U8 u8IndexMin, U8 u8IndexMax);
INTERFACE void MApp_ZUI_CTL_Grid_SetFnGetTextByIndex(HWND hwndGrid, FN_ZUI_CTL_Grid_GetTextByIndex fnGetText);

#undef INTERFACE

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MAPP_ZUI_CTL_GRID_H */


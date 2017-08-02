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

#ifndef _MAPP_ZUI_CTL_KEYBOARD_H
#define _MAPP_ZUI_CTL_KEYBOARD_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#ifdef MAPP_ZUI_CTL_KEYBOARD_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

typedef LPTSTR (*FN_ZUI_CTL_Keyboard_GetTextByIndex)(U8 u8idx,U8 u8chartype);
typedef void (*FN_ZUI_CTL_Keyboard_GetEditText)(U8 * u8str);
typedef BOOL (*FN_ZUI_CTL_Keyboard_SetEditText)(U8* u8str);

typedef struct
{
    HWND hwndFirstShown;       //the first visible item of page in run time
    HWND hwndLastShown;        //the last visible item of page in run time
} GUI_DATA_KEYBOARD_VARDATA;


typedef struct
{
    U8 u8CurIndex;
    U8 u8Rows;
    U8 u8CharIndex;
    U8 u8CharMax;
    U8 u8CharType;
    HWND hwndcharlist;
    HWND hwndbar;
    FN_ZUI_CTL_Keyboard_GetEditText fnGetEditText;
    FN_ZUI_CTL_Keyboard_SetEditText fnSetEditText;
    GUI_DATA_KEYBOARD_VARDATA * pVarData;
} GUI_DATA_KEYBOARD;


///////////////////////////////////////////////////////////////////////////////////////
/// MACRO
///
INTERFACE S32 MApp_ZUI_CTL_KeyBoardInputWinProc(HWND hWnd, PMSG pMsg);

#define KEYBOARDINPUT_WINPROC MApp_ZUI_CTL_KeyBoardInputWinProc


INTERFACE void MApp_ZUI_CTL_Keyboard_SetFnEditText(HWND hwndGrid,HWND hwndbutton,HWND hlist,FN_ZUI_CTL_Keyboard_GetEditText fnGetEditText, FN_ZUI_CTL_Keyboard_SetEditText fnSetEditText);
INTERFACE LPTSTR MApp_ZUI_CTL_KeyboardGetDynamicText(HWND hwndGrid, HWND hwndText);


#undef INTERFACE

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MAPP_ZUI_CTL_BALLPROGBAR_H */


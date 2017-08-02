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

#ifndef MAPP_KEY_H
#define MAPP_KEY_H

#include "Board.h"
#include "keypaddef.h"

#ifdef MAIN_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

//from MApp_GlobalVar.h
//INTERFACE U8 u8KeyCode;
extern BYTE xdata g_u8KeypadButton;
typedef enum
{
    ZUI_KEY_UP                     =BTN_PLUS,
    ZUI_KEY_DOWN               =BTN_MINUS,
    ZUI_KEY_MENU                =BTN_MENU,
    ZUI_KEY_INPUT_SOURCE        =BTN_EXIT,
    ZUI_KEY_POWER            =BTN_POWER,
    ZUI_KEY_NOTHING          =BTN_NOTHING,
    ZUI_KEY_EXIT                = 0x09, 
    ZUI_KEY_LEFT                = 0x0C,
    ZUI_KEY_SELECT              = 0x0D,
    ZUI_KEY_RIGHT               = 0x0F,
    ZUI_KEY_MUTE                = 0x1A,    
    ZUI_KEY_PIP                 = 0x6D,
    ZUI_KEY_NULL                = 0xFF
} EN_KEY;

#undef INTERFACE
#endif // MAPP_KEY_H


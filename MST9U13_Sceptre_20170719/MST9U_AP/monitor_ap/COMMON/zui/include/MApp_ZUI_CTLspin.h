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

#ifndef _MAPP_ZUI_CTL_SPIN_H
#define _MAPP_ZUI_CTL_SPIN_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

//////Spin control////////////////////////////////////////////////////
typedef struct //Spin control Item data
{
    DRAWSTYLE * pDrawItemNormal;
    DRAWSTYLE * pDrawItemFocus;
    U16 u16Param;

} GUI_DATA_SPIN_ITEM;

typedef struct  //Spin collection data
{
    GUI_DATA_SPIN_ITEM * pItems; //internal list array, NULL for empty set
    U16 u16Count;       //internal count
    U16 u16Size;        //array size....
    U16 u16FocusIndex;       //internal focus index, NOTE: assume single selection

}GUI_DATA_SPIN_INTERNAL;

#define SP_FLAG_LEFT_PRESSING 0x01
#define SP_FLAG_RIGHT_PRESSING 0x02
#define SP_FLAG_HIDE_LBUTTON    0x04
#define SP_FLAG_HIDE_RBUTTON 0x08

typedef struct
{
    // Collection Data
    GUI_DATA_SPIN_INTERNAL list; //internal data
} GUI_VARDATA_SPIN;

typedef struct //Spin control data
{
    GUI_VARDATA_SPIN *pVardata;
    // UI
    U16 u16ButtonWidth;
    DRAWSTYLE * pLeftButtonNormal;
    DRAWSTYLE * pLeftButtonPressing;
    DRAWSTYLE * pRightButtonNormal;
    DRAWSTYLE * pRightButtonPressing;
    U16 u16FormatFlag;          //SP_FLAG_HIDE_LBUTTON, SP_FLAG_HIDE_RBUTTON
    U16 u16PressingSleepTime; //pressing button delay (in ms)
    U16 u16ButtonBorder;

    // Event
    //CALLBACK_SEL_CHANGED OnSelectionChanged;

} GUI_DATA_SPIN;
extern S32 MApp_ZUI_CTL_SpinWinProc(HWND, PMSG);

#define SPINWINPROC MApp_ZUI_CTL_SpinWinProc

extern BOOLEAN MApp_ZUI_CTL_SpinSetSelectIndex(HWND hwnd, U16 select_idx);
extern U16 MApp_ZUI_CTL_SpinGetSelectIndex(HWND hwnd);


extern BOOLEAN MApp_ZUI_CTL_SpinSetItems(HWND hwnd, GUI_DATA_SPIN_ITEM * pItems, U16 u16Count);
extern void MApp_ZUI_CTL_SpinClearItems(HWND hwnd);

///////////////////////////////////////////////////////////////////////////////////////
/// MACRO
///
#define CTL_GETSPINLIST(hwnd)  (((GUI_DATA_SPIN *)g_GUI_WindowList[hwnd].pWindowData)->pVardata->list)

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MAPP_ZUI_CTL_SPIN_H */


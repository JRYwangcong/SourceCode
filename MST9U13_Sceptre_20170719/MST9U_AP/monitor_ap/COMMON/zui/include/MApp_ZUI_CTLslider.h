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

#ifndef _MAPP_ZUI_CTL_SLIDER_H
#define _MAPP_ZUI_CTL_SLIDER_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

//////Slider control////////////////////////////////////////////////////

#define SL_FLAG_VERTICAL 0x01

typedef struct
{
    // Data
    U16 u16MinValue, u16MaxValue; //include... min <= value <= max
    U16 u16IncValue, u16DecValue;
    U16 u16Value;
} GUI_VARDATA_SLIDER;

typedef struct //Slider control data
{
    GUI_VARDATA_SLIDER *pVardata;
    // UI
    //U16 u16BorderLeft, u16BorderTop;
    //U16 u16BorderRight, u16BorderBottom;
    RECT rcInnerBar;
    DRAWSTYLE * pInnerBar;
    U16 u16ThumbHeight, u16ThumbWidth;    //align to center of inner bar
    DRAWSTYLE * pActiveBar;     //left side for hor slider, top side for vert slider
    DRAWSTYLE * pThumb;
    U16 u16FormatFlag;          //SL_FLAG_VERTICAL

    // Event
    //CALLBACK_SEL_CHANGED OnSelectionChanged;

} GUI_DATA_SLIDER;

extern S32 MApp_ZUI_CTL_SliderWinProc(HWND, PMSG);

#define SLIDERWINPROC MApp_ZUI_CTL_SliderWinProc

extern BOOLEAN MApp_ZUI_CTL_SliderSetValue(HWND hwnd, U16 value);
extern U16 MApp_ZUI_CTL_SliderGetValue(HWND hwnd);

///////////////////////////////////////////////////////////////////////////////////
//  MACRO
//
#define CTL_GETSLIDERVALUE(hwnd)    (((GUI_DATA_SLIDER *)g_GUI_WindowList[hwnd].pWindowData)->pVardata->u16Value)
#define CTL_GETSLIDERMINVALUE(hwnd) (((GUI_DATA_SLIDER *)g_GUI_WindowList[hwnd].pWindowData)->pVardata->u16MinValue)
#define CTL_GETSLIDERMAXVALUE(hwnd) (((GUI_DATA_SLIDER *)g_GUI_WindowList[hwnd].pWindowData)->pVardata->u16MaxValue)
#define CTL_GETSLIDERINCVALUE(hwnd) (((GUI_DATA_SLIDER *)g_GUI_WindowList[hwnd].pWindowData)->pVardata->u16IncValue)
#define CTL_GETSLIDERDECVALUE(hwnd) (((GUI_DATA_SLIDER *)g_GUI_WindowList[hwnd].pWindowData)->pVardata->u16DecValue)

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MAPP_ZUI_CTL_SLIDER_H */


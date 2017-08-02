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

#ifndef _MAPP_ZUI_CTL_LIST_BOX_H
#define _MAPP_ZUI_CTL_LIST_BOX_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

//////List Box////////////////////////////////////////////////////
typedef struct //List Box Item data
{
    DRAWSTYLE * pDrawItemNormal;
    DRAWSTYLE * pDrawItemFocus;
    U16 u16Param;

} GUI_DATA_LIST_BOX_ITEM;


typedef struct  //List Box collection data
{
    GUI_DATA_LIST_BOX_ITEM * pItems; //internal thumbnail list array, NULL for empty set
    U16 u16Count;       //internal thumbnail count
    U16 u16Size;        //array size... grater or equal to count....
    U16 u16FocusIndex;       //internal thumbnail focus index, NOTE: assume single selection
    U16 u16StartItem;       //first item in this scroll view (include)
    U16 u16EndItem;     //last item in this scroll view  (exclude)

}GUI_DATA_LIST_BOX_INTERNAL;

#define LB_FLAG_SCROLL_PAGE         BIT0
#define LB_FLAG_DOWN_AS_SELECT      BIT1
#define LB_FLAG_RIGHT_AS_SELECT     BIT2

typedef enum
{
    LB_FOCUS_AUTO,
    LB_FOCUS_ALWAYS_HIDE,
    LB_FOCUS_ALWAYS_SHOW,
} GUI_DATA_LIST_BOX_FOCUS_ENUM;

typedef struct
{
    // Thumbnail Data
    GUI_DATA_LIST_BOX_INTERNAL list; //internal data
    //fpIndexString DisplayString;
    //fpIndexBitmap DisplayBitmap;

} GUI_VARDATA_LIST_BOX;

typedef struct //List Box control data
{
    GUI_VARDATA_LIST_BOX *pVardata;
    U16 u16ListBorderWidth; //reserved for outer border space
    U16 u16ListBorderHeight;    //reserved for outer border space
    U16 u16ItemBorderWidth; //reserved for item border space
    U16 u16ItemBorderHeight;    //reserved for item border space
    U16 u16ItemGapX;        //reserved between item and item
    U16 u16ItemGapY;        //reserved between item and item

    U16 u16ThumbRowCount;       //internal thumbnail display row grid count, should be > 0
    U16 u16ThumbColumnCount;    //internal thumbnail display column grid count, should be > 0
    //U16 u16ThumbItemSelBorder; //border for displaying selected items
    U16 u16ThumbFormatFlag;         //thumbnail list display flags
                            // LB_FLAG_SCROLL_PAGE: scroll a page <-> scroll a line

    GUI_DATA_LIST_BOX_FOCUS_ENUM enFocusType;

    //Background drawing style
    //DRAWSTYLE * pDrawBackground; //NULL for empty background

    //Non-Selected Item drawing style
    DRAWSTYLE * DrawItemBGNormal; //NULL for empty style

    //Selected Item drawing style
    DRAWSTYLE * DrawItemBGFocus; //NULL for empty style

    HWND hWndScrollBar;         //HWND_INVALID for no scrollbar binding

    //fpOwnerDrawThumbItem OwnerDrawItem;//NULL for static BMP case, Not-NULL for using owner draw

    // Event
    //CALLBACK_SEL_CHANGED OnSelectionChanged;


} GUI_DATA_LIST_BOX;
extern S32 MApp_ZUI_CTL_ListBoxWinProc(HWND, PMSG);

#define LISTBOXWINPROC MApp_ZUI_CTL_ListBoxWinProc

extern BOOLEAN MApp_ZUI_CTL_ListBoxSetSelectIndex(HWND hwnd, U16 select_idx);
extern U16 MApp_ZUI_CTL_ListBoxGetSelectIndex(HWND hwnd);

extern BOOLEAN MApp_ZUI_CTL_ListBoxSetItems(HWND hwnd, GUI_DATA_LIST_BOX_ITEM * pItems, U16 u16Count);
//extern BOOLEAN MApp_ZUI_CTL_ListBoxSetItemsCallback(HWND hwnd, fpIndexString fp_str, fpIndexBitmap fp_bmp, U16 u16Count);
extern void MApp_ZUI_CTL_ListBoxClearItems(HWND hwnd);


////////////////////////////////////////////////////////////////////////////////////
// MACRO
//
#define CTL_GETLISTBOXLIST(hwnd)  (((GUI_DATA_LIST_BOX *)g_GUI_WindowList[hwnd].pWindowData)->pVardata->list)
//#define CTL_GETLISTBOXSTRCALLBACK(hwnd)  (((GUI_DATA_LIST_BOX *)g_GUI_WindowList[hwnd].pWindowData)->pVardata->DisplayString)
//#define CTL_GETLISTBOXBMPCALLBACK(hwnd)  (((GUI_DATA_LIST_BOX *)g_GUI_WindowList[hwnd].pWindowData)->pVardata->DisplayBitmap)

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MAPP_ZUI_CTL_LIST_BOX_H */


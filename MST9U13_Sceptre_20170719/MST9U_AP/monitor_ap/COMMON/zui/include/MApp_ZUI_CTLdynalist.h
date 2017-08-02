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

#ifndef _MAPP_ZUI_CTL_DYNALIST_H
#define _MAPP_ZUI_CTL_DYNALIST_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#ifdef MAPP_ZUI_CTL_DYNALIST_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

//////Dynamic List control////////////////////////////////////////////////////

#define DL_STATE_IS_FIRST_PAGE BIT0
#define DL_STATE_IS_LAST_PAGE BIT1

typedef enum
{
    EN_DL_STATE_HIDDEN,
    EN_DL_STATE_NORMAL,
    EN_DL_STATE_DISABLED,
} GUI_ENUM_DYNAMIC_LIST_STATE;

typedef struct
{
    HWND hwndFirstShown;       //the first visible item of page in run time
    HWND hwndLastShown;        //the last visible item of page in run time
    U8 u8CurrentState;
} GUI_DATA_DYNAMIC_LIST_VARDATA;

typedef struct
{
    HWND hwndFirstItemOfPage;   //the first item of skintool, for position locate
    HWND hwndLastItemOfPage;    //the last item of skintool, for position locate
    GUI_DATA_DYNAMIC_LIST_VARDATA * pVarData;
} GUI_DATA_DYNAMIC_LIST;

INTERFACE S32 MApp_ZUI_CTL_DynamicListWinProc(HWND hWnd, PMSG pMsg);

#define DYNAMICLIST_WINPROC MApp_ZUI_CTL_DynamicListWinProc


///////////////////////////////////////////////////////////////////////////////////////
/// MACRO
///

INTERFACE void MApp_ZUI_CTL_DynamicListRefreshContent(HWND hwnd);
INTERFACE void MApp_ZUI_CTL_DynamicListSetItemFocus(HWND hwnd, HWND hwndItem);
INTERFACE BOOLEAN MApp_ZUI_CTL_DynamicListIsFirstPage(HWND hwnd);
INTERFACE BOOLEAN MApp_ZUI_CTL_DynamicListIsLastPage(HWND hwnd);
INTERFACE void MApp_ZUI_CTL_DynamicListSetItemFocus(HWND hwnd, HWND hwndItem);
INTERFACE void MApp_ZUI_CTL_DynamicListEnsureVisible(HWND hwnd, HWND hwndItem);
INTERFACE HWND MApp_ZUI_CTL_DynamicList_GetFirstItem(HWND hwnd);  //get first item of current page
INTERFACE HWND MApp_ZUI_CTL_DynamicList_GetLastItem(HWND hwnd);   //get last item of current page

#undef INTERFACE

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MAPP_ZUI_CTL_DYNALIST_H */


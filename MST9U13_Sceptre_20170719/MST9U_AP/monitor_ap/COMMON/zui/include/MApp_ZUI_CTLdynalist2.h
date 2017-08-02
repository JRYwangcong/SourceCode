////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (��MStar Confidential Information��) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _MAPP_ZUI_CTL_DYNALIST2_H
#define _MAPP_ZUI_CTL_DYNALIST2_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#ifdef MAPP_ZUI_CTL_DYNALIST2_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

//////Dynamic List 2 control////////////////////////////////////////////////////

//NOTE: dynamic list 2 is depent on dynamic list
//      some of data structure is the same, but behavior is different...

typedef struct
{
    //HWND hwndFirstShown;
    //HWND hwndLastShown;
    //U8 u8CurrentState;
    HWND hwndPreviousFocus;
} GUI_DATA_DYNAMIC_LIST_2_VARDATA;

typedef struct
{
    HWND hwndForwardFirstVisible; //foward part, focus will be put on first position
    HWND hwndForwardLastVisible;
    HWND hwndBackwardFirstVisible; //backward part
    HWND hwndBackwardLastVisible;
    GUI_DATA_DYNAMIC_LIST_2_VARDATA * pVarData;
} GUI_DATA_DYNAMIC_LIST_2;

INTERFACE S32 MApp_ZUI_CTL_DynamicList2WinProc(HWND, PMSG);

#define DYNAMICLIST2_WINPROC MApp_ZUI_CTL_DynamicList2WinProc


///////////////////////////////////////////////////////////////////////////////////////
/// MACRO
///

INTERFACE void MApp_ZUI_CTL_DynamicList2RefreshContent(HWND hwnd);

//INTERFACE BOOLEAN MApp_ZUI_ACT_DynamicList2IsFirstPage(HWND hwnd);
//INTERFACE BOOLEAN MApp_ZUI_ACT_DynamicList2IsLastPage(HWND hwnd);

#undef INTERFACE

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MAPP_ZUI_CTL_DYNALIST2_H */


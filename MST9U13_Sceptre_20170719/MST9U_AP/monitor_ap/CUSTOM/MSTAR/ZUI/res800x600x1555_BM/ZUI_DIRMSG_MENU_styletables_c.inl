////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2010 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (''MStar Confidential Information'') by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// This file is automatically generated by SkinTool [Version:0.2.2][Build:Apr  7 2010 20:56:42]
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////
// MAINFRAME styles..


/////////////////////////////////////////////////////
// DIRMSG_MENU_TRANSPARENT_BG styles..

static DRAWSTYLE _MP_TBLSEG _Zui_Dirmsg_Menu_Transparent_Bg_Normal_DrawStyle[] =
{
    { CP_FILL_RECT, CP_ZUI_FILL_RECT_INDEX_16 },
    { CP_NOON, 0 },
};

/////////////////////////////////////////////////////
// DIRECT_MSG_MENU styles..

static DRAWSTYLE _MP_TBLSEG _Zui_Direct_Msg_Menu_Normal_DrawStyle[] =
{
    { CP_FILL_RECT, CP_ZUI_FILL_RECT_INDEX_0 },
    { CP_NOON, 0 },
};

/////////////////////////////////////////////////////
// DIRECT_MSG_MENU_FRAM styles..

static DRAWSTYLE _MP_TBLSEG _Zui_Direct_Msg_Menu_Fram_Normal_DrawStyle[] =
{
    { CP_FILL_RECT, CP_ZUI_FILL_RECT_INDEX_17 },
    { CP_NOON, 0 },
};

/////////////////////////////////////////////////////
// DIRECT_MSG_MENU_FRAM_BG styles..

#define _Zui_Direct_Msg_Menu_Fram_Bg_Normal_DrawStyle _Zui_Direct_Msg_Menu_Normal_DrawStyle

/////////////////////////////////////////////////////
// DIRECT_MSG_MENU_TITLE styles..

#define _Zui_Direct_Msg_Menu_Title_Normal_DrawStyle _Zui_Direct_Msg_Menu_Normal_DrawStyle

/////////////////////////////////////////////////////
// DIRECT_MSG_MENU_TITLE_TEXT styles..

static DRAWSTYLE _MP_TBLSEG _Zui_Direct_Msg_Menu_Title_Text_Normal_DrawStyle[] =
{
    { CP_FILL_RECT, CP_ZUI_FILL_RECT_INDEX_1 },
    { CP_TEXT_OUT, CP_ZUI_TEXT_OUT_INDEX_196 },
    { CP_NOON, 0 },
};

/////////////////////////////////////////////////////
// DIRECT_MSG_MENU_TOP_BODER styles..

#define _Zui_Direct_Msg_Menu_Top_Boder_Normal_DrawStyle _Zui_Direct_Msg_Menu_Fram_Normal_DrawStyle

/////////////////////////////////////////////////////
// DIRECT_MSG_MENU_BTM_BODER styles..

#define _Zui_Direct_Msg_Menu_Btm_Boder_Normal_DrawStyle _Zui_Direct_Msg_Menu_Fram_Normal_DrawStyle

/////////////////////////////////////////////////////
// DIRECT_MSG_MENU_TEXT styles..

static DRAWSTYLE _MP_TBLSEG _Zui_Direct_Msg_Menu_Text_Normal_DrawStyle[] =
{
    { CP_FILL_RECT, CP_ZUI_FILL_RECT_INDEX_0 },
    { CP_TEXT_OUT, CP_ZUI_TEXT_OUT_INDEX_197 },
    { CP_NOON, 0 },
};

//////////////////////////////////////////////////////
// Window Draw Style List  (normal, focused, disable)

WINDOWDRAWSTYLEDATA _MP_TBLSEG _GUI_WindowsDrawStyleList_Zui_Dirmsg_Menu[] =
{
    // 0 = HWND_MAINFRAME
    { NULL, NULL, NULL     },

    // 1 = HWND_DIRMSG_MENU_TRANSPARENT_BG
    { _Zui_Dirmsg_Menu_Transparent_Bg_Normal_DrawStyle, NULL, NULL     },

    // 2 = HWND_DIRECT_MSG_MENU
    { _Zui_Direct_Msg_Menu_Normal_DrawStyle, NULL, NULL     },

    // 3 = HWND_DIRECT_MSG_MENU_FRAM
    { _Zui_Direct_Msg_Menu_Fram_Normal_DrawStyle, NULL, NULL     },

    // 4 = HWND_DIRECT_MSG_MENU_FRAM_BG
    { _Zui_Direct_Msg_Menu_Fram_Bg_Normal_DrawStyle, NULL, NULL     },

    // 5 = HWND_DIRECT_MSG_MENU_TITLE
    { _Zui_Direct_Msg_Menu_Title_Normal_DrawStyle, NULL, NULL     },

    // 6 = HWND_DIRECT_MSG_MENU_TITLE_TEXT
    { _Zui_Direct_Msg_Menu_Title_Text_Normal_DrawStyle, NULL, NULL     },

    // 7 = HWND_DIRECT_MSG_MENU_TOP_BODER
    { _Zui_Direct_Msg_Menu_Top_Boder_Normal_DrawStyle, NULL, NULL     },

    // 8 = HWND_DIRECT_MSG_MENU_BTM_BODER
    { _Zui_Direct_Msg_Menu_Btm_Boder_Normal_DrawStyle, NULL, NULL     },

    // 9 = HWND_DIRECT_MSG_MENU_TEXT
    { _Zui_Direct_Msg_Menu_Text_Normal_DrawStyle, NULL, NULL     },

};

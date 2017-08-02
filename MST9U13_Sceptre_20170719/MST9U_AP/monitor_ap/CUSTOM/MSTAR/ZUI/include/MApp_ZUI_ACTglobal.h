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

#ifndef _MAPP_ZUI_ACTGLOBAL_H
#define _MAPP_ZUI_ACTGLOBAL_H

#include "Global.h"
#include "ZUI_tables_h.inl"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */


#ifdef MAPP_ZUI_ACTGLOBAL_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

////////////////////////////////////////////////////////
// UI customized constants
#define OSD_GOP_ID         E_GOP_OSD
#define OSD_GOP_RIGHT_ID   E_GOP_OSD_RIGHT

//MStar OSD font settings
#if  (!VECTOR_FONT_ENABLE)
#define FONT_BIG    FONT_BMP_0
#define FONT_NORMAL FONT_BMP_1
#define FONT_SMALL  FONT_BMP_2
#else
#define FONT_BIG    FONT_0
#define FONT_NORMAL FONT_1
#define FONT_SMALL  FONT_2
#endif

#define LOGO_TIME_OUT    (2000)  // 2 second
#define MENU_TIME_OUT_MS 30000
#define CHLIST_TIME_OUT_MS 10000
#define INSTALL_GUIDE_TIME_OUT_MS 180000   // 3 min
#define PROGRAM_EDIT_TIME_OUT_MS 10000
#define AUDIO_LANGUAGE_TIME_OUT_MS 10000
#define SUBTITLE_LANGUAGE_TIME_OUT_MS 3000
#define INPUT_SOURCE_TIME_OUT_MS 5000
#define HOTKEY_OPTION_TIME_OUT_MS 3000
#define AUDIO_VOLUME_TIME_OUT_MS 3000
#define MESSAGE_BOX_TIME_OUT_MS 3000
#define TENKEY_NUMBER_TIME_OUT_MS 3000
#define EPG_TIME_OUT_MS 180000      //3 minutes
#define CHANNEL_INFO_TIME_OUT_MS 5000
#define CIMMI_TIME_OUT_MS  180000      //3 minutes
#define PVR_TIME_OUT_MS     0//infinity if this time is too small OSD will close while format is not finished.
#define PVR_BROWSER_TIME_OUT_MS     30000
#define EFFECT_SETTING_TIME_OUT_MS  10000
#define DTV_MANUAL_SCAN_END_TIME_OUT_MS  10000

#define BUTTONANICLICK_PERIOD 200 //for pressing bitmap (ms)

#define STRING_BUFFER_LENGTH    1024            // MApp_Epg_GetDetailDescription() need 2k bytes string buffer.
INTERFACE U16 u16StringBuffer[STRING_BUFFER_LENGTH];
#define CHAR_BUFFER ((LPTSTR)u16StringBuffer)

#define ZUI_COLOR_TRANSPARENT_RGB444 (((MPLAYER_CLRKEY_R>>4)<<8)|((MPLAYER_CLRKEY_G>>4)<<4)|((MPLAYER_CLRKEY_B>>4)))
#define ZUI_COLOR_TRANSPARENT_RGB555 (((MPLAYER_CLRKEY_R>>3)<<10)|((MPLAYER_CLRKEY_G>>3)<<5)|((MPLAYER_CLRKEY_B>>3)))
#define ZUI_COLOR_TRANSPARENT_RGB8888 MPLAYER_CLRKEY
#define ZUI_GWIN_BLENDING           (63*9/10)
#define ZUI_EXCEPTION_MULTITASK          1
#if ZUI_EXCEPTION_MULTITASK
#define ZUI_EXCEPTION_MULITASK_TIMER              50
#endif

////////////////////////////////////////////////////////

typedef enum
{
    ACTION_SELECT,
    ACTION_SELCHG,
    ACTION_ITEMCHG,
} ACTION_TYPE;

typedef struct
{
    U8 bDisplayLogo:1;

} ZuiMenuFlagType;

INTERFACE ZuiMenuFlagType g_stZuiMenuFlag;

/////////////////////////////////////////
//INTERFACE BOOLEAN MApp_ZUI_ACT_HandleStartupKey(VIRTUAL_KEY_CODE key);
INTERFACE BOOLEAN MApp_ZUI_ACT_HandleGlobalKey(VIRTUAL_KEY_CODE key);
INTERFACE S32 MApp_ZUI_ACT_ExecuteWndProc(U8 u8WinProcID, PMSG msg);
//INTERFACE BOOLEAN MApp_ZUI_ACT_ExecuteWndAction(U16 act); //HWND hwnd, ACTION_TYPE act);
INTERFACE BOOLEAN MApp_ZUI_ACT_PostMessageFilter(U8 u8WinProcID, MESSAGE_ENUM msg);
//INTERFACE void _MApp_ZUI_ACT_AppClose(void); //HWND sender);
INTERFACE LPTSTR MApp_ZUI_ACT_GetDynamicText(HWND hwnd);
INTERFACE U16 MApp_ZUI_ACT_GetDynamicBitmap(HWND hwnd, DRAWSTYLE_TYPE type);
INTERFACE S16 MApp_ZUI_ACT_GetDynamicValue(HWND hwnd);
INTERFACE S16 MApp_ZUI_ACT_GetDynamicValue_2(HWND hwnd);
INTERFACE GUI_ENUM_DYNAMIC_LIST_STATE MApp_ZUI_ACT_QueryDynamicListItemStatus(HWND hwnd);
INTERFACE OSD_COLOR MApp_ZUI_ACT_GetDynamicColor(HWND hwnd, DRAWSTYLE_TYPE type, OSD_COLOR colorOriginal);

#if ZUI_EXCEPTION_MULTITASK
INTERFACE void MApp_ZUI_ACT_Exception_MultiTask(void);
#endif
#undef INTERFACE

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MAPP_ZUI_ACTGLOBAL_H */


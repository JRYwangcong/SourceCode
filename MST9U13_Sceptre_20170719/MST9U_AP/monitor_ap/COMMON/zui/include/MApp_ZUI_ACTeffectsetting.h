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

#ifndef _MAPP_ZUI_ACTEFFECTSETTING_H
#define _MAPP_ZUI_ACTEFFECTSETTING_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include "MApp_ZUI_APIgdi.h"

#ifdef MAPP_ZUI_ACTEFFECTSETTING_C
#define INTERFACE
#else
#define INTERFACE extern
#endif


typedef enum
{
    EN_3D_MAIN_PAGE_MotionTrans,
    EN_3D_MAIN_PAGE_Fake3D,
//    EN_3D_MAIN_PAGE_EFF3,
//    EN_3D_MAIN_PAGE_EFF4,
    EN_3D_MAIN_PAGE_INVALID,
}EN_3D_MAIN_PAGE;

typedef enum
{
    EN_3D_MAIN_MENU_OPEN_OFF,
//    EN_3D_MAIN_MENU_OPEN_SPREAD,
    EN_3D_MAIN_MENU_OPEN_FADE,
    EN_3D_MAIN_MENU_OPEN_MOSAIC,
    EN_3D_MAIN_MENU_OPEN_UP,
    EN_3D_MAIN_MENU_OPEN_DOWN,
    EN_3D_MAIN_MENU_OPEN_LEFT,
    EN_3D_MAIN_MENU_OPEN_RIGHT,
    EN_3D_MAIN_MENU_OPEN_INVALID,
}EN_3D_MAIN_MENU_OPEN;

typedef enum
{
    EN_3D_MAIN_MENU_CLOSE_OFF,
    EN_3D_MAIN_MENU_CLOSE_ROLL,
    EN_3D_MAIN_MENU_CLOSE_FADE,
    EN_3D_MAIN_MENU_CLOSE_UP,
    EN_3D_MAIN_MENU_CLOSE_DOWN,
    EN_3D_MAIN_MENU_CLOSE_LEFT,
    EN_3D_MAIN_MENU_CLOSE_RIGHT,
    EN_3D_MAIN_MENU_CLOSE_INVALID,
}EN_3D_MAIN_MENU_CLOSE;

typedef enum
{
    EN_3D_MAIN_MENU_ITEM_OFF,
    EN_3D_MAIN_MENU_ITEM_ON,
    EN_3D_MAIN_MENU_ITEM_INVALID,
}EN_3D_MAIN_MENU_ITEM;

typedef enum
{
    EN_3D_POPUP_DIALOG_OFF,
    EN_3D_POPUP_DIALOG_ON,
    EN_3D_POPUP_DIALOG_INVALID,
}EN_3D_POPUP_DIALOG;

typedef enum
{
    EN_3D_ASSISTANT_MENU_OFF,
    EN_3D_ASSISTANT_MENU_ROLL,
    EN_3D_ASSISTANT_MENU_FADE,
    EN_3D_ASSISTANT_MENU_MOSAIC,
    EN_3D_ASSISTANT_MENU_UP,
    EN_3D_ASSISTANT_MENU_DOWN,
    EN_3D_ASSISTANT_MENU_LEFT,
    EN_3D_ASSISTANT_MENU_RIGHT,
    EN_3D_ASSISTANT_MENU_INVALID,
}EN_3D_ASSISTANT_MENU;



typedef struct
{
    EN_3D_MAIN_PAGE eMainPageEff;
    EN_3D_MAIN_MENU_ITEM eMainMenuItemEff;
    EN_3D_POPUP_DIALOG ePopupDialogEff;
} EFF_3D_SETTING_CTL;

typedef enum
{
    EN_3D_MAIN_PAGE_PARA,
    EN_3D_MAIN_MENU_ITEM_PARA,
    EN_3D_POPUP_DIALOG_PARA,
    EN_3D_CTL_INVALID_PARA,
}EN_3D_SETTING_CTL_PARA;


typedef struct
{
    EN_3D_MAIN_MENU_OPEN eMainMenuOpenEff;
    EN_3D_MAIN_MENU_CLOSE eMainMenuCloseEff;
    EN_3D_ASSISTANT_MENU eAssistantMenuEff;
} EFF_3D_SETTING_ACT;

typedef enum
{
    EN_3D_MAIN_MENU_OPEN_PARA,
    EN_3D_MAIN_MENU_CLOSE_PARA,
    EN_3D_ASSISTANT_MENU_PARA,
    EN_3D_ACT_INVALID_PARA,
}EN_3D_SETTING_ACT_PARA;




/////////////////////////////////////////
// Cutomize Window Procedures...


/////////////////////////////////////////
// Event Handlers....
INTERFACE void MApp_ZUI_ACT_AppShowEffectSetting(void);
INTERFACE BOOLEAN MApp_ZUI_ACT_HandleEffectSettingKey(VIRTUAL_KEY_CODE key);
INTERFACE void MApp_ZUI_ACT_TerminateEffectSetting(void);
INTERFACE BOOLEAN MApp_ZUI_ACT_ExecuteEffectSettingAction(U16 act);
INTERFACE LPTSTR MApp_ZUI_ACT_GetEffectSettingDynamicText(HWND hwnd);


#undef INTERFACE

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MAPP_ZUI_ACTEFFECTSETTING_H */


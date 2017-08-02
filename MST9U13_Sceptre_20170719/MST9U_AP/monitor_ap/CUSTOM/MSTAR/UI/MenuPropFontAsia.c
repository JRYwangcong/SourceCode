///////////////////////////////////////////////////////////////////////////////
/// @file MenuPropFontAsia.c
/// @brief
/// @author MStarSemi Inc.
///
/// Functions to Load OSD prop fonts according by menu page.
///
/// Features
///  -
///
///////////////////////////////////////////////////////////////////////////////
#define _MENUPROPFONTASIA_C

//#include <math.h>
#include "datatype.h"
#include "Board.h"
#include "Global.h"
#include "menudef.h"
#include "ms_reg.h"
#include "msOSD.h"
#include "Menu.h"
//#include "DebugMsg.h"
#include "Ms_rwreg.h"
#include "MenuPropFontAsiaTbl.h"
#include "Panel.h"
#include "MenuPropStr.h"

#if SIMPLY_CHINESE
extern BYTE xdata g_u8MenuPageIndex;
extern BYTE LoadPropFonts1618(BYTE u8Addr, BYTE *pu8Strings, WORD u16FontCount, BYTE *pu8Strings1, WORD u16FontCount1, BYTE u8UDShift ,BYTE u8UDShift1);

static MenuFontType * code tMenuAsiaFontTbl[]=
{
    NULL,   //PowerOffMenu,
    NULL,   //PowerOnMenu,
    MenuPage2Asia,//StandbyMenu,
    MenuPage3Asia,//SuspendMenu,
    MenuPage4Asia,//CableNotConnectedMenu,
    MenuPage5Asia,//UnsupportedModeMenu,

    NULL,//RootMenu, /// Pre-load main menu font in root menu

    NULL,//LogoMenu,
#if ENABLE_VGA_INPUT
    MenuPage8Asia,//AutoMenu,
    MenuPage8Asia,//HotAutoMenu,
#endif
#if ENABLE_MULTI_INPUT
    MenuPage10Asia,//InputMenu,
#endif
    NULL,//,    HotMenuLockMenu
    MenuPage12Asia,// LCDConditionMenu
    NULL,   //Diag test pattern
#if ENABLE_DEMURA_FUNCTION
    NULL, //MENU_DEMURA_MEASURE_MODE
#endif
    NULL,//MenuPage0,//MainMenu,
//SUB MENU
#if ENABLE_MULTI_INPUT
    MenuPage103Asia,//InputSourceMenu,
#endif
    MenuPage101Asia,//BriteContrastMenu,
    MenuPage109Asia,//ColorFormatSettingsMenu
    MenuPage102Asia,//ColorSettingsMenu
    MenuPage104Asia,//PictureQualitySettingsMenu
    MenuPage105Asia,//DisplaySettingsMenu
#if ENABLE_AUDIO_SETTINGS_MENU
    MenuPage107Asia,//AudioSettingsMenu
#endif
#if ENABLE_MULTI_WINDOW_SETTINGS_MENU
    MenuPage108Asia,//MultiWindowSettingsMenu
#endif
    MenuPage106Asia,//OtherSettingsMenu

    MenuPage200Asia,//CustomRGB

    MenuPage300Asia,//DDCCIConfirmMenu
#if ENABLE_DCR
    MenuPage306Asia,//BrightnessDCROffConfirmMenu
#endif
#if ENABLE_RTE  //Sky101111
    MenuPage304Asia,//Response Time Confirm Menu
#endif

    MenuPage301Asia,// LCDConditionConfirmmenu
    MenuPage303Asia // FactoryResetConfirmmenu
};
#if SIMPLY_CHINESE  //Mike 110524
#define ASIA_LANGUAGE_INDEX     (USER_PREF_LANGUAGE-LANG_SChinese)
#endif

void DynamicLoadAsiaFont(void)
{
    MenuFontType *pstMenuFonts;
    pstMenuFonts = tMenuAsiaFontTbl[g_u8MenuPageIndex];

    if (tMenuAsiaFontTbl[g_u8MenuPageIndex])
    {
        LoadPropFonts1618(PROP_FONT_START,(BYTE*)(pstMenuFonts+ASIA_LANGUAGE_INDEX)->pu8Fonts, ((pstMenuFonts+ASIA_LANGUAGE_INDEX)->u16FontCount),0,0,0,0);
    }
}
#if 0
#ifndef _REMOVE_WARNING_MSG_TEXT_
void LoadAsiaWarningMessageFont(void)
{
    LoadPropFonts1618(MSG_WARNING_MSG_START,(WORD*)MenuPage401Asia[ASIA_LANGUAGE_INDEX].pu8Fonts, MenuPage401Asia[ASIA_LANGUAGE_INDEX].u16FontCount,0,0,0,0);
}
#endif
void LoadAsiaMessageFont(void)
{
    LoadPropFonts1618(MSG_WARNING_MSG_START,(WORD*)MenuPage402Asia[ASIA_LANGUAGE_INDEX].pu8Fonts, MenuPage402Asia[ASIA_LANGUAGE_INDEX].u16FontCount,0,0,0,0);
}
#endif
void LoadAsiaMainMenuFont(void)
{
    LoadPropFonts1618(MM_PROP_FONT_START,(BYTE*)MenuPage0Asia[ASIA_LANGUAGE_INDEX].pu8Fonts, MenuPage0Asia[ASIA_LANGUAGE_INDEX].u16FontCount,0,0,0,0);
}
#if 0  //Steven110519
void LoadAsiaOptResolutionFont()
{
    LoadPropFonts1618(SM_OPT_RESOLUTION_START,(BYTE*)MenuPage400Asia[ASIA_LANGUAGE_INDEX].pu8Fonts, MenuPage400Asia[ASIA_LANGUAGE_INDEX].u16FontCount,0,0,0,0);
}
#endif
#if 0  //Steven110523
void LoadAsiaAutoAdjustMsgFont(void)
{
    LoadPropFonts1618(PROP_FONT_START,(BYTE*)MenuPage104Asia[ASIA_LANGUAGE_INDEX].pu8Fonts, MenuPage104Asia[ASIA_LANGUAGE_INDEX].u16FontCount,0,0,0,0);
}
#endif
void LoadAsiaOSL2Font(void)
{
    LoadPropFonts1618(OS_L2_PROP_FONT_START,(BYTE*)MenuPage1060Asia[ASIA_LANGUAGE_INDEX].pu8Fonts, MenuPage1060Asia[ASIA_LANGUAGE_INDEX].u16FontCount,0,0,0,0);
}

#endif



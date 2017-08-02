///////////////////////////////////////////////////////////////////////////////
/// @file MenuPropFontAsia.h
/// @brief
/// @author MStarSemi Inc.
///
///
///
/// Features
///  -
///
///////////////////////////////////////////////////////////////////////////////

#ifndef _MENUPROPFONTASIA_H
#define _MENUPROPFONTASIA_H
#include "menudef.h"

#ifdef _MENUPROPFONTASIA_C
#define INTERFACE
#else
#define INTERFACE   extern
#endif

INTERFACE void DynamicLoadAsiaFont(void);
#if 0
#ifndef _REMOVE_WARNING_MSG_TEXT_
INTERFACE void LoadAsiaWarningMessageFont(void);
#endif
INTERFACE void LoadAsiaMessageFont(void);
#endif
INTERFACE void LoadAsiaMainMenuFont(void);
#if 0  //Steven110519
INTERFACE void LoadAsiaOptResolutionFont();
#endif
//INTERFACE void LoadAsiaAutoAdjustMsgFont(void);
INTERFACE void LoadAsiaOSL2Font(void);

#undef INTERFACE
#endif



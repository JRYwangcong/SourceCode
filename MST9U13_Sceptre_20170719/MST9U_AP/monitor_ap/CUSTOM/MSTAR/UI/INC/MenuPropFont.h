///////////////////////////////////////////////////////////////////////////////
/// @file MenuPropFont.h
/// @brief
/// @author MStarSemi Inc.
///
///
///
/// Features
///  -
///
///////////////////////////////////////////////////////////////////////////////

#ifndef _MENUPROPFONT_H
#define _MENUPROPFONT_H
#include "menudef.h"

#ifdef _MENUPROPFONT_C
#define INTERFACE
#else
#define INTERFACE   extern
#endif
INTERFACE void Osd_DynamicLoadFont(WORD u8Addr, BYTE *pu8FontPtr, WORD u16Num);
INTERFACE void LoadLogoMenuFont(void);
INTERFACE void DynamicLoadPropFont(void);
INTERFACE void LoadCurrentResolutionStringFont(void);
INTERFACE void LoadMainMenuFont(void);
INTERFACE void LoadPropFontAndInputIconFont(void);
INTERFACE void LoadUnsupportedModeMenuFont(void);
INTERFACE void Osd_LoadHotMuteFont( void );

//INTERFACE void LoadCurrentLanguageTextFont(void);
//INTERFACE void LoadOSL2Font(void);
//INTERFACE void LoadOSL2LanguageFont(void);
#if ENABLE_MULTI_INPUT
INTERFACE void LoadInputSourceMenuFont(void);
#endif
#undef INTERFACE
#endif



///////////////////////////////////////////////////////////////////////////////
/// @file MenuIcon.h
/// @brief Head file of MenuIcon.c
/// @author MStarSemi Inc.
///
/// Functions to deal with icons.
///
/// Features
///  -
///////////////////////////////////////////////////////////////////////////////

#ifndef _MENUICON_H
#define _MENUICON_H

#ifdef _MENUICON_C
#define INTERFACE
#else
#define INTERFACE   extern
#endif
#include "menudef.h"
INTERFACE void DrawIcon(BYTE u8XPos, BYTE u8YPos, BYTE *pu8Icon, BYTE u8Flags, BYTE u8ColorSelect);

///==============================================================
#if 1  //Steven110525
INTERFACE BYTE* GetLogoMstarTopIcon(void);
INTERFACE BYTE* GetLogoMstarTop1Icon(void);
INTERFACE BYTE* GetLogoMstarDownIcon(void);
INTERFACE BYTE* GetMainMenuLogoIcon(void);  //Steven110526
#endif


//Mainmenu Icons

INTERFACE BYTE* GetSixWhiteCharIcon(void);
INTERFACE BYTE* GetSixRedCharIcon(void);
INTERFACE BYTE* GetSixGreenCharIcon(void);
INTERFACE BYTE* GetSixBlueCharIcon(void);

INTERFACE BYTE* GetMenuLockIcon(void);

#undef INTERFACE
#endif   ///_MENUICON_H



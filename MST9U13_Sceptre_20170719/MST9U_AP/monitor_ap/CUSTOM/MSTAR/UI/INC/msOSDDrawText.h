///////////////////////////////////////////////////////////////////////////////
/// @file msOSDDrawText.h
/// @brief Head file of msOSDDrawText.c
/// @author MStarSemi Inc.
///
/// Application interlace functions for drawing string.
///
/// Features
///  - Draw text module consist of following files:
///  1) msOSDDrawText.c includes the functions to draw text.
///  2) MenuPropStr.c/FMenuPropStr.c... include the function to get the (start address, length) data of string.
///  3) MenuPropStrTbl.h includes the raw data of all supported languages.
///  4) These files should be in the same bank otherwise put msOSDDrawText.c in common bank.
///////////////////////////////////////////////////////////////////////////////

#ifndef _MSOSDDRAWTEXT_H
#define _MSOSDDRAWTEXT_H

#ifdef _MSOSDDRAWTEXT_C
#define INTERFACE
#else
#define INTERFACE   extern
#endif

INTERFACE void DrawPropStr(BYTE u8XPos, BYTE u8YPos, BYTE *pu8Str);
INTERFACE void DrawCenterText(BYTE u8XPos, BYTE u8YPos, BYTE *pu8Str);
INTERFACE void DrawCenterDrawArrowText(BYTE u8XPos, BYTE u8YPos, BYTE *pu8Str);
INTERFACE void DrawFullText(BYTE u8XPos, BYTE u8YPos, BYTE *pu8Str);
INTERFACE void DrawFullCenterText(BYTE u8XPos, BYTE u8YPos, BYTE *pu8Str);
INTERFACE void DrawRightAlignText(BYTE u8XPos, BYTE u8YPos, BYTE *pu8Str);
INTERFACE void  DrawRealStr(BYTE u8XPos, BYTE u8YPos, BYTE *pu8Str);
INTERFACE void DrawArrowCenterText(BYTE u8XPos, BYTE u8YPos, BYTE *pu8Str);

///==============================================================
#ifndef DYNAMIC_STRING_MAX
#define DYNAMIC_STRING_MAX  10
#endif
INTERFACE BYTE xdata  tPropText[DYNAMIC_STRING_MAX][2];

#undef INTERFACE
#endif   ///_MSOSDDRAWTEXT_H




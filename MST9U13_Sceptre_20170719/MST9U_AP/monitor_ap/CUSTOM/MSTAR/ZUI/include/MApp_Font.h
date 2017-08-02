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

#ifndef MAPP_FONT_H
#define MAPP_FONT_H

#include "datatype.h"
#include "msAPI_Font.h"

#ifdef MAPP_LOADFONT_C
#define INTERFACE
#else
#define INTERFACE extern
#endif


//=================================================================
//                            Macro
//=================================================================
// Font width and height defined below are for reference only.
// Their actual values are kept in the font binary files
#define MS_CHAR_WIDTH          16
#define MS_CHAR_HEIGHT         22
#define MS_CHAR_WIDTHX2        32
#define MS_CHAR_HEIGHTX2       40

//==============================================================================
/*                            Structure Type                                      */
//==============================================================================

INTERFACE void MApp_LoadFont(U8 u8StepID);
INTERFACE void MApp_LoadFont_SetVecFontInfo(U16 nFontId, LOADFONT_STRUCT* strfont, U8 u8Begin, U8 u8End);
INTERFACE void MApp_LoadFont_SetBMPFontInfo(LOADFONT_PAIR* fontpair, U8 u8num);

#undef INTERFACE
#endif


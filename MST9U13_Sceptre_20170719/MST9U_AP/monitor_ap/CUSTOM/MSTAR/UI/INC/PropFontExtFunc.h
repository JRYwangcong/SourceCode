///////////////////////////////////////////////////////////////////////////////
/// @file PropFontExtFunc.h
/// @brief Head file for PropFontExtFunc.c
/// @author MStarSemi Inc.
///
///
///////////////////////////////////////////////////////////////////////////////
#ifndef _PROPFONTEXTFUNC_H
#define _PROPFONTEXTFUNC_H

#include "datatype.h"
#include "Board.h"
#include "Global.h"
//#include "Keypaddef.h"
#include "menudef.h"
#include "LoadPropFont1218.h"

#ifdef _PROPFONTEXTFUNC_C
#define INTERFACE
#else
#define INTERFACE   extern
#endif

#define WAND            (0x0FFF)



INTERFACE BYTE LoadPropNumber(BYTE u8Addr,BYTE u8Value,BYTE u8Flag);
INTERFACE void LoadResolutionString(BYTE u8StartAddr,BYTE* pu8String,WORD u16HResolution,WORD u16VResolution,BYTE u8VFreq, Bool bShowHz);
INTERFACE BYTE LoadPropFontText(WORD u8Addr,BYTE *pu8Text); ///pu8Text must be NULL terminal
//INTERFACE BYTE* VersionText(void);

#if 0//(ENABLE_DVI_INPUT||ENABLE_HDMI_INPUT)
INTERFACE BYTE* HzText(void);
#endif

#if 1 //Steven110525
INTERFACE BYTE* SelectInputSourceText(void);
#if ENABLE_MULTI_WINDOW_SETTINGS_MENU
INTERFACE BYTE* InputSourceText_MultiWindow2(void);
INTERFACE BYTE* InputSourceText_MultiWindow3(void);
INTERFACE BYTE* InputSourceText_MultiWindow4(void);
#else
INTERFACE BYTE* InputSourceText_MultiWindow1(void);
#endif //ENABLE_MULTI_WINDOW_SETTINGS_MENU

//INTERFACE BYTE* MenuWebSiteText(void);
#endif


#if 0//ENABLE_MULTI_INPUT
INTERFACE BYTE* InputMenuNumberText(void);
#endif
#if 1//(ODM_NAME==ODM_INL)
INTERFACE BYTE *FactoryVersionText(WORD u8Address);
#endif
//INTERFACE void LoadChar(BYTE u8Addr, BYTE u8Ascii, BYTE u8UDShift);
//TERFACE BYTE LoadTimePropNumber(BYTE u8Addr,WORD u16Sec); //FY12_MSG
INTERFACE void LoadFontSetAddrStart(BYTE u8Addr);

//#define OSD2_A4       0xA4
//typedef struct
//{ BYTE u8SpaceWidth;
//  WORD tLineData[18];
//} PropFontType;
//extern PropFontType code tTimingPropFontSet[];
#if 0
extern PropFontType code tPropFontSet_R[];
extern PropFontType code tPropFontSet1_R[];
extern PropFontType code tPropFontSet2_R[];
extern BYTE LanguageIndex;
extern BYTE SecondTblAddr;
#endif

#if ENABLE_COMPRESSED_PROP_FONT
extern WORD code tPropFontSetArialNarrow13[];
extern WORD code tPropFontSetArialNarrow13Bold[];
#else
extern PropFontNonCompressType code tPropFontSetArialNarrow13[];
extern PropFontNonCompressType code tPropFontSetArialNarrow13Bold[];
#endif

#undef INTERFACE
#endif   ///_LALIB_H






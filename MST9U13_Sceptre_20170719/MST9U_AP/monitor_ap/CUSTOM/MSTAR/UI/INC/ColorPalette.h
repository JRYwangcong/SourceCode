///////////////////////////////////////////////////////////////////////////////
/// @file ColorPalette.h
/// @brief
/// @author MStarSemi Inc.
///
/// Functions to load Color Palette
///
/// Features
///  -
///////////////////////////////////////////////////////////////////////////////

#ifndef _COLORPALETTE_H
#define _COLORPALETTE_H



//For S0
typedef enum
{
  CP_TRANSPARENT_COLOR,
  CP_MainGround,         // 1    //mask
  CP_WHITE,         // 2
  CP_Frame,    // 3

  CP_Black,   // 4
  CP_Cyan,      // 5
  CP_SubGround, // 6
  CP_UnSelGround, // 7

  CP_UnSel,    // 8
  CP_RGB191,         // 9
  CP_Orange,         // A
  CP_YELLOW,        // B 255, 255, 000

  CP_BLUE,          // C 000, 000, 255
  CP_GREEN,         // D 000, 255, 000
  CP_RED,           // E 255, 000, 000
  CP_PURPLE,         // F

  CP_MAX_COLOR,



  CP_BK_COLOR_H     =CP_MainGround,  //Steven110519
  CP_CLEAR          =CP_TRANSPARENT_COLOR,
//Menu L0/L1/L2 Color
  CP_BK_COLOR_L0    =CP_WHITE,
  CP_BK_COLOR_L1    =CP_WHITE,
  CP_BK_COLOR_L2    =CP_WHITE,

  CP_BK_COLOR_SEL  =CP_Frame,
//Hightlight Color

  CP_HL_COLOR_L1    =CP_BK_COLOR_L2,
//Saperation Line Color
  CP_SL_COLOR_L1    =CP_WHITE,
  CP_SL_COLOR_L2    =CP_BK_COLOR_L0,
  CP_SL_COLOR_SubFrame    =CP_RGB191,
  CP_FG_COLOR_L2    =CP_Orange,      //L2 Text Color while highlight L1
#if 1  //Steven110523
  CP_ADJ_COLOR_L2   =CP_RED,      //L2 Text Color while highlight L2
#else
  CP_ADJ_COLOR_L2   =CP_BLACK,      //L2 Text Color while highlight L2
#endif
//Button OSD Saperation Line Color

  CP_MODEL_COLOR    =CP_WHITE,
  CP_GRAY_OUT_L0    =CP_MainGround,
  CP_GRAY_OUT_L2    =CP_MainGround,


  //CP_BK_BC_ORG_L1L2 =CP_MainGround,   //FY12_BC  //Steven110519
  CP_MSG_BK         =CP_BK_COLOR_L1, //FY12_MSG
#if 1  //Steven110523
  CP_MSG_BK_H       =CP_RED
#else
  CP_MSG_BK_H       =CP_GRAY3
#endif
} CP_ColorType;
//For S1

#define CP1_4_HM_SAPERATION 45
//Color Select
//-----------------------
#define CS_NORMAL           0
#define CS_HIGHLIGHT        1
#define CS_SELECTED         2
//-----------------------
#define MONO_COLOR(FColor,BkColor)  ((FColor)<<4|(BkColor))
#define FOUR_COLOR(ColorGroup)      ((ColorGroup)<<2)
#define EIGHT_COLOR(ColorGroup)     ((ColorGroup)<<3)

extern void LoadLogoCP(void);
extern void LoadNormalCP(void);

#if 1  //Steven110523
#define CURSOR_TOP_R            0x00
#define CURSOR_TOP_G            0x00
#define CURSOR_TOP_B            0x00
#define CURSOR_COLOR_DELTA_R    0x00
#define CURSOR_COLOR_DELTA_G    0x00
#define CURSOR_COLOR_DELTA_B    0x00

#define CURSOR_DOWN_R           (CURSOR_TOP_R-CURSOR_COLOR_DELTA_R)
#define CURSOR_DOWN_G           (CURSOR_TOP_G-CURSOR_COLOR_DELTA_G)
#define CURSOR_DOWN_B           (CURSOR_TOP_B-CURSOR_COLOR_DELTA_B)
#else
#if 1 //FY12_CURSOR
#define CURSOR_TOP_R            0x00//0x07
#define CURSOR_TOP_G            0xA0//0xA7
#define CURSOR_TOP_B            0xC0//0xC4
#define CURSOR_COLOR_DELTA_R    0x00
#define CURSOR_COLOR_DELTA_G    0x20
#define CURSOR_COLOR_DELTA_B    0x30
#else
#define CURSOR_TOP_R            0xE0
#define CURSOR_TOP_G            0xC0
#define CURSOR_TOP_B            0x50
#define CURSOR_COLOR_DELTA_R    0x00
#define CURSOR_COLOR_DELTA_G    0x20
#define CURSOR_COLOR_DELTA_B    0x30
#endif
#define CURSOR_DOWN_R           (CURSOR_TOP_R-CURSOR_COLOR_DELTA_R)
#define CURSOR_DOWN_G           (CURSOR_TOP_G-CURSOR_COLOR_DELTA_G)
#define CURSOR_DOWN_B           (CURSOR_TOP_B-CURSOR_COLOR_DELTA_B)
#endif

typedef struct
{
    BYTE u8Red;
    BYTE u8Green;
    BYTE u8Blue;
    BYTE u8ColorKey;
} ColorPaletteType;

#endif ///



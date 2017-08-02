///////////////////////////////////////////////////////////////////////////////
/// @file ColorPaletteTbl.h
/// @brief
/// @author MStarSemi Inc.
///
/// Provide the color palette for ColorPalette.c module.
///
/// Features
///  -
///
///////////////////////////////////////////////////////////////////////////////
#ifndef _COLORPALETTETBL_H
#define _COLORPALETTETBL_H

#define CP_COLORKEY_T    1
#define CP_COLORKEY_F    0


#if LOGO_ENABLE

#if MSTAR_LOGO
//large logo use.
ColorPaletteType code tLogoColorPalette[]=
{
//Steven110525
  {0x00, 0x00, 0x00, CP_COLORKEY_F},
  {0xBF, 0xBF, 0xBF, CP_COLORKEY_F},
  {0xFF, 0xFF, 0xFF, CP_COLORKEY_F},
  {0xFF, 0x00, 0x00, CP_COLORKEY_F},

  {0x00, 0x00, 0x00, CP_COLORKEY_F},
  {0xFF, 0xFF, 0xFF, CP_COLORKEY_F},
  {0xA0, 0x9F, 0xFF, CP_COLORKEY_F},
  {0x00, 0xFF, 0x00, CP_COLORKEY_F},
//====================================_MSTAR_LOGO_
  {0xFF, 0xFF, 0xFF, CP_COLORKEY_F},
  {0x00, 0x00, 0x00, CP_COLORKEY_F},
  {0xFF, 0x00, 0x00, CP_COLORKEY_F},
  {0xFF, 0x00, 0xFF, CP_COLORKEY_F},

  {0x00, 0x80, 0xA0, CP_COLORKEY_F},
  {0xC0, 0xC0, 0xFF, CP_COLORKEY_F},
  {0x00, 0xA0, 0x00, CP_COLORKEY_F},
  {0xFF, 0xFF, 0x00, CP_COLORKEY_F},  //  {0x00, 0x00, 0x00, CP_COLORKEY_F}, //Jison 110621 for TSUMV black color
//====================================
  {0x00, 0x00, 0x00, CP_COLORKEY_F},
  {0xFF, 0xFF, 0xFF, CP_COLORKEY_F},
  {0x80, 0x80, 0x80, CP_COLORKEY_F},

};


#elif LOGO_RX
ColorPaletteType code tLogoColorPalette[]=
{
  {0x00, 0x00, 0x00, CP_COLORKEY_F},
  {0xFF, 0xFF, 0xFF, CP_COLORKEY_T},
  {0x00, 0x00, 0x00, CP_COLORKEY_F},
  {0xC0, 0x40, 0x40, CP_COLORKEY_T},
  
  {0x00, 0x00, 0x00, CP_COLORKEY_F},
  {0x40, 0x40, 0xC0, CP_COLORKEY_T},
  
  {0xC0, 0x40, 0x40, CP_COLORKEY_T},
  {0x40, 0x40, 0xC0, CP_COLORKEY_T},
  {0xFF, 0xFF, 0xFF, CP_COLORKEY_T},
  {0x00, 0x00, 0x00, CP_COLORKEY_F},

  {0x00, 0x00, 0x00, CP_COLORKEY_F},
  {0x00, 0x00, 0x00, CP_COLORKEY_F},

};
#elif LOGO_MONOPRICE
ColorPaletteType code tLogoColorPalette[]=
{
  {0x00, 0x00, 0x00, CP_COLORKEY_F},
  {0xFF, 0xFF, 0xFF, CP_COLORKEY_F},
  {0x00, 0x00, 0x00, CP_COLORKEY_F},
  {0x00, 0x00, 0x00, CP_COLORKEY_F},
};
#elif LOGO_TERRA
ColorPaletteType code tLogoColorPalette[]=
{
  {0xFF, 0xFF, 0xFF, CP_COLORKEY_F},
  {0x00, 0x00, 0x00, CP_COLORKEY_F},
  {0x00, 0x00, 0x00, CP_COLORKEY_F},
  {0x00, 0x00, 0x00, CP_COLORKEY_F},
};

#elif LOGO_UPSTAR
ColorPaletteType code tLogoColorPalette[]=
{
  {0x00, 0x00, 0xFF, CP_COLORKEY_F},
  {0xFF, 0xFF ,0xFF, CP_COLORKEY_F},
  {0x00, 0x00, 0xFF, CP_COLORKEY_F},
  {0xFF, 0x00 ,0x00, CP_COLORKEY_F},
};
#elif LOGO_HKC
ColorPaletteType code tLogoColorPalette[]=
{
  {0x00, 0x00, 0x00, CP_COLORKEY_F},
  {0xB7, 0x03, 0x77, CP_COLORKEY_F},
  {0x00, 0x00, 0x00, CP_COLORKEY_F},
  {0x00, 0x00, 0x00, CP_COLORKEY_F},
};
#elif LOGO_ZEOL||LOGO_ZEOL_X2
ColorPaletteType code tLogoColorPalette[]=
{
  {0x00, 0x00, 0x00, CP_COLORKEY_F},
  {0xFF, 0xFF, 0xFF, CP_COLORKEY_F},
  {0xFF, 0xFF, 0xFF, CP_COLORKEY_F},
  {0xFF, 0xFF, 0xFF, CP_COLORKEY_F},
};
#elif LOGO_Neview
ColorPaletteType code tLogoColorPalette[]=
{
  {0x00, 0x00, 0x00, CP_COLORKEY_F},
  {0xFF, 0x00, 0x00, CP_COLORKEY_F},
  {0xFF, 0xFF, 0xFF, CP_COLORKEY_F},
  {0xFF, 0xFF, 0xFF, CP_COLORKEY_F},
};
#elif LOGO_HDC
ColorPaletteType code tLogoColorPalette[]=
{
  {0xFF, 0xFF, 0xFF, CP_COLORKEY_F},
  {0x00, 0x00, 0x00, CP_COLORKEY_F},
  {0xFF, 0xFF, 0xFF, CP_COLORKEY_F},
  {0x10, 0x90, 0xD0, CP_COLORKEY_F},
};
#elif LOGO_JAPANNEXT
ColorPaletteType code tLogoColorPalette[]=
{
  {0x00, 0x00, 0x00, CP_COLORKEY_F},
  {0xFF, 0xFF, 0xFF, CP_COLORKEY_F},
  {0xFF, 0xFF, 0xFF, CP_COLORKEY_F},
  {0xFF, 0x00, 0x00, CP_COLORKEY_F},
};
#elif LOGO_KOIS
ColorPaletteType code tLogoColorPalette[]=
{
  {0x00, 0x00, 0x00, CP_COLORKEY_F},
  {0xFF, 0xFF, 0xFF, CP_COLORKEY_F},
  {0x00, 0x00, 0x00, CP_COLORKEY_F},
  {0xE6, 0x78, 0x17, CP_COLORKEY_F},
    
  {0xC0, 0x40, 0x40, CP_COLORKEY_F},
  {0xFF, 0xFF, 0xFF, CP_COLORKEY_F},
  {0xE6, 0x78, 0x17, CP_COLORKEY_F},
  {0x00, 0x00, 0x00, CP_COLORKEY_F},

  {0xE6, 0x78, 0x17, CP_COLORKEY_F},
  {0xFF, 0xFF, 0xFF, CP_COLORKEY_F},
  {0x00, 0x00, 0x00, CP_COLORKEY_F},
  {0x00, 0x00, 0x00, CP_COLORKEY_F},

};
#elif LOGO_KOGAN||LOGO_KOGAN_NEW
ColorPaletteType code tLogoColorPalette[]=
{
  {0xFF, 0xFF, 0xFF, CP_COLORKEY_F},
  {0x30, 0x30, 0x30, CP_COLORKEY_F},
  {0xFF, 0xFF, 0xFF, CP_COLORKEY_F},
  {0xE0, 0x10, 0x30, CP_COLORKEY_F},
};
#elif LOGO_BENZ
ColorPaletteType code tLogoColorPalette[]=
{
  {0x00, 0x00, 0x00, CP_COLORKEY_F},
  {0xFF, 0xFF, 0xFF, CP_COLORKEY_F},
  {0xFF, 0xFF, 0xFF, CP_COLORKEY_F},
  {0xFF, 0xFF, 0xFF, CP_COLORKEY_F},
};
#elif LOGO_TIEMU
ColorPaletteType code tLogoColorPalette[]=
{
  {0x00, 0x00, 0x00, CP_COLORKEY_F},
  {0xFF, 0xFF, 0xFF, CP_COLORKEY_F},
  {0xFF, 0xFF, 0xFF, CP_COLORKEY_F},
  {0xFF, 0xFF, 0xFF, CP_COLORKEY_F},
};
#elif LOGO_X_Star
ColorPaletteType code tLogoColorPalette[]=
{
  {0x00, 0x00, 0x00, CP_COLORKEY_F},
  {0xFF, 0xFF, 0xFF, CP_COLORKEY_F},
  {0xFF, 0xFF, 0xFF, CP_COLORKEY_F},
  {0xFF, 0xFF, 0xFF, CP_COLORKEY_F},
};
#elif LOGO_AXM
ColorPaletteType code tLogoColorPalette[]=
{
  {0xFF, 0xFF, 0xFF, CP_COLORKEY_F},
  {0x00, 146, 207, CP_COLORKEY_F},
  {0x00, 146, 207, CP_COLORKEY_F},
  {0xFF, 0xFF, 0xFF, CP_COLORKEY_F},
};

#elif LOGO_BUBALUS
ColorPaletteType code tLogoColorPalette[]=
{
  {0xFF, 0xFF, 0xFF, CP_COLORKEY_F},
  {0xFF, 0x00, 0x00, CP_COLORKEY_F},
  {0xFF, 0xFF, 0xFF, CP_COLORKEY_F},
  {0xFF, 0xFF, 0xFF, CP_COLORKEY_F},
};
#elif LOGO_Qnix
ColorPaletteType code tLogoColorPalette[]=
{
  {0x00, 0x00, 0x00, CP_COLORKEY_F},
  {0xFF, 0xFF, 0xFF, CP_COLORKEY_F},
  {0xFF, 0xFF, 0xFF, CP_COLORKEY_F},
  {0xFF, 0xFF, 0xFF, CP_COLORKEY_F},
};
#elif LOGO_AV
ColorPaletteType code tLogoColorPalette[]=
{
  {0x00, 0x00, 0x00, CP_COLORKEY_F},
  {0xFF, 0xFF, 0xFF, CP_COLORKEY_F},
  {0xFF, 0xFF, 0xFF, CP_COLORKEY_F},
  {0xFF, 0xFF, 0xFF, CP_COLORKEY_F},
};
#elif LOGO_NIXEUS
ColorPaletteType code tLogoColorPalette[]=
{
  {0x00, 0x00, 0x00, CP_COLORKEY_F},
  {0xA7, 0xA7 ,0xA7, CP_COLORKEY_F},
  {0x00, 0x00, 0x00, CP_COLORKEY_F},
  {0x00, 0x00, 0x00, CP_COLORKEY_F},
};

#elif LOGO_IMAGIC
ColorPaletteType code tLogoColorPalette[]=
{
  {0x00, 0x00, 0x00, CP_COLORKEY_F},
  {0xFF, 0xFF ,0xFF, CP_COLORKEY_F},
  {0x00, 0x00, 0x00, CP_COLORKEY_F},
  {234,  84 ,  17, CP_COLORKEY_F},
};

#elif LOGO_HYUNDAI
ColorPaletteType code tLogoColorPalette[]=
{
  {0xFF, 0xFF ,0xFF, CP_COLORKEY_F},
  {48, 48 ,146, CP_COLORKEY_F},
  {0xFF, 0xFF ,0xFF, CP_COLORKEY_F},
  {141,  198 ,  63, CP_COLORKEY_F},
};
#elif LOGO_DMM
ColorPaletteType code tLogoColorPalette[]=
{
  {0x00, 0x00 ,0x00, CP_COLORKEY_F},
  {0xFF, 0xFF ,0xFF, CP_COLORKEY_F},
  {0x00, 0x00 ,0x00, CP_COLORKEY_F},
  {30,  144 ,  191, CP_COLORKEY_F},
};
#elif LOGO_VMazon
ColorPaletteType code tLogoColorPalette[]=
{
  {0x00, 0x00 ,0x00, CP_COLORKEY_F},
  {0xFF, 0xFF ,0xFF, CP_COLORKEY_F},
  {0x00, 0x00 ,0x00, CP_COLORKEY_F},
  {0xFF, 0xFF ,0xFF, CP_COLORKEY_F},
};
#elif LOGO_Titan
ColorPaletteType code tLogoColorPalette[]=
{
  {0x00, 0x00 ,0x00, CP_COLORKEY_F},
  {0, 158 ,59, CP_COLORKEY_F},
  {0x00, 0x00 ,0x00, CP_COLORKEY_F},
  {0xFF, 0xFF ,0xFF, CP_COLORKEY_F},
};
#elif LOGO_IIP
ColorPaletteType code tLogoColorPalette[]=
{
  {0x00, 0x00, 0x00, CP_COLORKEY_F},
  {0xFF, 0xFF, 0xFF, CP_COLORKEY_T},
  {0x00, 0x00 ,0x00, CP_COLORKEY_F},
  {0xFF, 0xFF ,0xFF, CP_COLORKEY_F},
};
#elif LOGO_ForGame
ColorPaletteType code tLogoColorPalette[]=
{
  {0x00, 0x00, 0x00, CP_COLORKEY_F},
  {248, 72, 116, CP_COLORKEY_T},
  {0x00, 0x00 ,0x00, CP_COLORKEY_F},
  {230, 0 , 19, CP_COLORKEY_F},
};
#elif LOGO_GREATWALL
ColorPaletteType code tLogoColorPalette[]=
{
  {0xFF, 0xFF, 0xFF, CP_COLORKEY_T},
  {0x00, 0x00, 0x00, CP_COLORKEY_F},
  {0xFF, 0xFF, 0xFF, CP_COLORKEY_T},
  {0xFF, 0xFF ,0xFF, CP_COLORKEY_F},
};
#elif LOGO_keoushi
ColorPaletteType code tLogoColorPalette[]=
{
  {0x00, 0x00, 0x00, CP_COLORKEY_F},
  {0xFF, 0xFF, 0xFF, CP_COLORKEY_F},
  {0xFF, 0xFF, 0xFF, CP_COLORKEY_F},
  {0xFF, 0xFF, 0xFF, CP_COLORKEY_F},
};
#elif LOGO_ViewSync
ColorPaletteType code tLogoColorPalette[]=
{
  {0x00, 0x00, 0x00, CP_COLORKEY_F},
  {0xFF, 0xFF, 0xFF, CP_COLORKEY_F},
  {0xFF, 0xFF, 0xFF, CP_COLORKEY_F},
  {0xFF, 0xFF, 0xFF, CP_COLORKEY_F},
};
#elif LOGO_GameMNT
ColorPaletteType code tLogoColorPalette[]=
{
  {0x00, 0x00, 0x00, CP_COLORKEY_F},
  {230, 0, 18, CP_COLORKEY_F},
  {0xFF, 0xFF, 0xFF, CP_COLORKEY_F},
  {0xFF, 0xFF, 0xFF, CP_COLORKEY_F},
};
#elif LOGO_SaioSone
ColorPaletteType code tLogoColorPalette[]=
{
  {0x00, 0x00, 0x00, CP_COLORKEY_F},
  {230, 0, 18, CP_COLORKEY_F},
  {0xFF, 0xFF, 0xFF, CP_COLORKEY_F},
  {0xFF, 0xFF, 0xFF, CP_COLORKEY_F},
};
#elif LOGO_Sceptre
ColorPaletteType code tLogoColorPalette[]=
{
  {0x00, 0x00, 0x00, CP_COLORKEY_F},
  {230, 0, 18, CP_COLORKEY_F},
  {0xFF, 0xFF, 0xFF, CP_COLORKEY_F},
  {0xFF, 0xFF, 0xFF, CP_COLORKEY_F},
};
#elif LOGO_TCL
ColorPaletteType code tLogoColorPalette[]=
{
  {0xFF, 0x00, 0x00, CP_COLORKEY_F},
  {0x00, 0x00, 0x00, CP_COLORKEY_F},
  {0xFF, 0x00, 0x00, CP_COLORKEY_F},
  {0xFF, 0xFF, 0xFF, CP_COLORKEY_F},
  
  {0xFF, 0xFF, 0xFF, CP_COLORKEY_F},
  {0x00, 0x00, 0x00, CP_COLORKEY_F},
  {0xFF, 0xFF, 0xFF, CP_COLORKEY_F},
  {0xFF, 0xFF, 0xFF, CP_COLORKEY_F},
};
#elif LOGO_Sceptre_New
ColorPaletteType code tLogoColorPalette[]=
{
  {193, 39, 45, CP_COLORKEY_F},
  {0x00, 0x00, 0x00, CP_COLORKEY_F},
  {0x00, 0x00, 0x00, CP_COLORKEY_F},
  {193, 39, 45, CP_COLORKEY_F},
};


#endif




#endif
//2007-09-21 Webber : making color define for palette usage
//--    Start   --
#define RGB_CURSOR_WHITE    0xFF, 0xFF, 0xFF
#define RGB_CURSOR_BLACK    0x00, 0x00, 0x00

#define RGB_LOW_BLUE		0x37,0x97,0xE7
#define RGB_HIGH_BLUE		0x00,0x57,0xA7
#define RGB_LOW_GRAY		0x87,0xC7,0xFF
#define RGB_GRAY_NOSEL		0xB4,0xB4,0xB4



#define RGB_WHITE     0xFF, 0xFF, 0xFF
#define RGB_BLACK     0x00, 0x00, 0x00

#define RGB_Purple     0xFF, 0x00, 0xFF
#define RGB_Orange     240, 160, 48
#define RGB_Indigo     6, 82, 121


#define RGB_BACKGROUMD         0x87, 0x97, 0xC7
#define RGB_SELBACKGROUMD   0x27, 0x87, 0xD7
#define RGB_57A7FF			  0x57,0xA7,0xFF

#define RGB_GameClock_Ground            RGB_HIGH_BLUE//0x00, 0x50, 0xD0
#define RGB_GameClock_Ground2            RGB_HIGH_BLUE//0x00, 0x40, 0xA0

#define RGB_GRAY_0xF0            0xF0, 0xF0, 0xF0
#define RGB_Light_Cyan            RGB_LOW_GRAY




#define RGB_BK_COLOR_R      0xFF, 0x00 ,0x00 
#define RGB_BK_COLOR_G      0x00, 0xFF ,0x00 
#define RGB_BK_COLOR_B      0x00, 0x00 ,0xFF 
#define RGB_BK_COLOR_Yellow      0xFF, 0xFF ,0x00 


#define RGB_NOUSE     0xFF, 0xFF, 0xFF     
#define MainBackGround          123, 123, 123
#define RGB_Frame          193, 39, 45
#define RGB_FrameMidLine          0x00, 0x00, 0x00
#define RGB_SubFrame          191, 191, 191

//QuickStart
#define RGB_QucikStart_R     RGB_BK_COLOR_R

#define RGB_Picture_Purple   163, 73, 164
#define RGB_Picture_Cyan     0, 162, 232

#define SubBackGround          73, 73, 73


#define RGB_UnSel                  102, 102, 102
#define UnSelBackGround        33, 33, 33


//--    End     --

ColorPaletteType code tOSDColorPalette0[]=
{
//0
  {0x00, 0x00, 0x00,    CP_COLORKEY_F},           // 0 Black
  {MainBackGround,      CP_COLORKEY_F},     
  {0xFF, 0xFF, 0xFF,    CP_COLORKEY_F},         
  {RGB_Frame,    CP_COLORKEY_F},           // A - CP_GRAY4
//Common use for VGauge Bar
  {RGB_FrameMidLine,     CP_COLORKEY_F},           // 4
  {RGB_Picture_Cyan,    CP_COLORKEY_F},        
  {SubBackGround,    CP_COLORKEY_F},           
  {UnSelBackGround,    CP_COLORKEY_F},         
// 1
  {RGB_UnSel,      CP_COLORKEY_F},           // 8 Tick
  {RGB_SubFrame,    CP_COLORKEY_F},           // 9 - CP_GRAY3
  {RGB_NOUSE,    CP_COLORKEY_F},           // A - CP_GRAY4
  {RGB_BK_COLOR_Yellow,    CP_COLORKEY_F},           // B - CP_YELLOW

  {RGB_BK_COLOR_B,    CP_COLORKEY_F},           // C - CP_BLUE
  {RGB_BK_COLOR_G,    CP_COLORKEY_F},           // D - CP_GREEN
  {RGB_BK_COLOR_R,    CP_COLORKEY_F},           // E - CP_RED
  {RGB_Picture_Purple,    CP_COLORKEY_F},           // F - CP_NO_COLOR

//MidLine NoSelect
  {MainBackGround,      CP_COLORKEY_F},         //0x10
  {RGB_Frame,      CP_COLORKEY_F},     
  {MainBackGround,      CP_COLORKEY_F},     
  {RGB_FrameMidLine,      CP_COLORKEY_F},     

//QuickStart NoSelect
  {RGB_FrameMidLine,      CP_COLORKEY_F},      //0x14
  {MainBackGround,      CP_COLORKEY_F},     
  {RGB_WHITE,      CP_COLORKEY_F},     
  {RGB_QucikStart_R,      CP_COLORKEY_F},     

//QuickStart Select
  {RGB_FrameMidLine,      CP_COLORKEY_F},      //0x18
  {RGB_Frame,      CP_COLORKEY_F},     
  {RGB_WHITE,      CP_COLORKEY_F},     
  {RGB_QucikStart_R,      CP_COLORKEY_F},     


//Picture NoSelect  Picture_0_2 0_3
  {MainBackGround ,      CP_COLORKEY_F},      //0x1C
  {RGB_FrameMidLine,      CP_COLORKEY_F},     
  {RGB_NOUSE,      CP_COLORKEY_F},     
  {RGB_BK_COLOR_Yellow,      CP_COLORKEY_F},     

//Picture Select  Picture_0_2 0_3
  {RGB_Frame ,      CP_COLORKEY_F},      //0x20
  {RGB_FrameMidLine,      CP_COLORKEY_F},     
  {RGB_NOUSE,      CP_COLORKEY_F},     
  {RGB_BK_COLOR_Yellow,      CP_COLORKEY_F},     

//Picture  Picture_1_1 2_1 2_2
  {RGB_FrameMidLine ,      CP_COLORKEY_F},      //0x24
  {RGB_Picture_Purple,      CP_COLORKEY_F},     
  {RGB_BK_COLOR_B,      CP_COLORKEY_F},     
  {RGB_WHITE,      CP_COLORKEY_F},     

//Picture  Picture_2_3 2_4
  {RGB_FrameMidLine ,      CP_COLORKEY_F},      //0x28
  {RGB_BK_COLOR_B,      CP_COLORKEY_F},     
  {RGB_Picture_Cyan,      CP_COLORKEY_F},     
  {RGB_WHITE,      CP_COLORKEY_F},     

  {RGB_NOUSE,      CP_COLORKEY_F},        //0x2C
  {RGB_NOUSE,      CP_COLORKEY_F},     
  {RGB_NOUSE,      CP_COLORKEY_F},     
  {RGB_NOUSE,      CP_COLORKEY_F},     

//8Color
//Picture  Picture_1_2 1_3 1_4
  {RGB_FrameMidLine ,      CP_COLORKEY_F},      //0x30
  {RGB_BK_COLOR_G,      CP_COLORKEY_F},     
  {RGB_BK_COLOR_Yellow,      CP_COLORKEY_F},     
  {RGB_WHITE,      CP_COLORKEY_F},     

  {RGB_BK_COLOR_B ,      CP_COLORKEY_F},      //0x34
  {RGB_Picture_Purple,      CP_COLORKEY_F},     
  {RGB_Picture_Cyan,      CP_COLORKEY_F},     
  {RGB_BK_COLOR_R,      CP_COLORKEY_F},     

//Color_0_2 0_3 1_1 1_2  NoSelect
  {RGB_FrameMidLine ,      CP_COLORKEY_F},      //0x38
  {MainBackGround,      CP_COLORKEY_F},     
  {RGB_Picture_Cyan,      CP_COLORKEY_F},     
  {RGB_BK_COLOR_Yellow,      CP_COLORKEY_F},     

//Color_0_2 0_3 1_1 1_2  Select
  {RGB_FrameMidLine ,      CP_COLORKEY_F},      //0x3C
  {RGB_Frame,      CP_COLORKEY_F},     
  {RGB_Picture_Cyan,      CP_COLORKEY_F},     
  {RGB_BK_COLOR_Yellow,      CP_COLORKEY_F},     

//Color_2_1  NoSelect
  {RGB_FrameMidLine ,      CP_COLORKEY_F},      //0x40
  {MainBackGround,      CP_COLORKEY_F},     
  {RGB_Picture_Cyan,      CP_COLORKEY_F},     
  {RGB_Picture_Purple,      CP_COLORKEY_F},     
  
//Color_2_1  Select
  {RGB_FrameMidLine ,      CP_COLORKEY_F},      //0x44
  {RGB_Frame,      CP_COLORKEY_F},     
  {RGB_Picture_Cyan,      CP_COLORKEY_F},     
  {RGB_Picture_Purple,      CP_COLORKEY_F},     

//Color 3_3  NoSelect
  {RGB_FrameMidLine ,      CP_COLORKEY_F},      //0x48
  {MainBackGround,      CP_COLORKEY_F},     
  {RGB_NOUSE,      CP_COLORKEY_F},     
  {RGB_WHITE,      CP_COLORKEY_F},     
  
//Color 3_3  Select
  {RGB_FrameMidLine ,      CP_COLORKEY_F},      //0x4C
  {RGB_Frame,      CP_COLORKEY_F},     
  {RGB_NOUSE,      CP_COLORKEY_F},     
  {RGB_WHITE,      CP_COLORKEY_F},   

//System  NoSelect
  {RGB_FrameMidLine,      CP_COLORKEY_F},      //0x50
  {MainBackGround,      CP_COLORKEY_F},     
  {RGB_QucikStart_R,      CP_COLORKEY_F},     
  {RGB_WHITE,      CP_COLORKEY_F},     

//System_ Select
  {RGB_FrameMidLine,      CP_COLORKEY_F},      //0x54
  {RGB_Frame,      CP_COLORKEY_F},     
  {RGB_QucikStart_R,      CP_COLORKEY_F},     
  {RGB_WHITE,      CP_COLORKEY_F},     
/************************Sub Icon*************************/
//QuickStart_ NoSelect
  {MainBackGround,      CP_COLORKEY_F},      //0x58
  {RGB_WHITE,      CP_COLORKEY_F},     
  {RGB_FrameMidLine,      CP_COLORKEY_F},     
  {RGB_QucikStart_R,      CP_COLORKEY_F},     
//QuickStart_ Select
  {SubBackGround,      CP_COLORKEY_F},            //0x5C
  {RGB_WHITE,      CP_COLORKEY_F},     
  {RGB_FrameMidLine,      CP_COLORKEY_F},     
  {RGB_QucikStart_R,      CP_COLORKEY_F},     
//Picture0_1 1_1 0_0 1_0NoSelect 8color
  {MainBackGround,      CP_COLORKEY_F},      //0x60
  {RGB_FrameMidLine,      CP_COLORKEY_F},     
  {RGB_WHITE,      CP_COLORKEY_F},     
  {RGB_Picture_Purple,      CP_COLORKEY_F},     
  
  {RGB_NOUSE,      CP_COLORKEY_F},      //0x64
  {RGB_BK_COLOR_B,      CP_COLORKEY_F},     
  {RGB_BK_COLOR_R,      CP_COLORKEY_F},     
  {RGB_BK_COLOR_Yellow,      CP_COLORKEY_F},     
//Picture0_1 1_1 0_0 1_0 Select 8color
  {SubBackGround,      CP_COLORKEY_F},      //0x68
  {RGB_FrameMidLine,      CP_COLORKEY_F},     
  {RGB_WHITE,      CP_COLORKEY_F},     
  {RGB_Picture_Purple,      CP_COLORKEY_F},     
  
  {RGB_NOUSE,      CP_COLORKEY_F},      //0x6C
  {RGB_BK_COLOR_B,      CP_COLORKEY_F},     
  {RGB_BK_COLOR_R,      CP_COLORKEY_F},     
  {RGB_BK_COLOR_Yellow,      CP_COLORKEY_F},     
//Picture0_2 0_3 1_2 1_3 NoSelect 8color
  {MainBackGround,      CP_COLORKEY_F},      //0x70
  {RGB_FrameMidLine,      CP_COLORKEY_F},     
  {RGB_WHITE,      CP_COLORKEY_F},     
  {RGB_Picture_Cyan,      CP_COLORKEY_F},     
  
  {RGB_NOUSE,      CP_COLORKEY_F},      //0x74
  {RGB_BK_COLOR_B,      CP_COLORKEY_F},     
  {RGB_BK_COLOR_G,      CP_COLORKEY_F},     
  {RGB_BK_COLOR_Yellow,      CP_COLORKEY_F},     
//Picture0_2 0_3 1_2 1_3 Select 8color
  {SubBackGround,      CP_COLORKEY_F},      //0x78
  {RGB_FrameMidLine,      CP_COLORKEY_F},     
  {RGB_WHITE,      CP_COLORKEY_F},     
  {RGB_Picture_Cyan,      CP_COLORKEY_F},     
  
  {RGB_NOUSE,      CP_COLORKEY_F},      //0x7C
  {RGB_BK_COLOR_B,      CP_COLORKEY_F},     
  {RGB_BK_COLOR_G,      CP_COLORKEY_F},     
  {RGB_BK_COLOR_Yellow,      CP_COLORKEY_F},     


//Color0_0 0_1 0_2 0_3 1_0 1_3NoSelect 
  {MainBackGround,      CP_COLORKEY_F},      //0x80
  {RGB_FrameMidLine,      CP_COLORKEY_F},     
  {RGB_Picture_Cyan,      CP_COLORKEY_F},     
  {RGB_BK_COLOR_Yellow,      CP_COLORKEY_F},     
//Color0_0 0_1 0_2 0_3 1_0 1_3 Select 
  {SubBackGround,      CP_COLORKEY_F},      //0x84
  {RGB_FrameMidLine,      CP_COLORKEY_F},     
  {RGB_Picture_Cyan,      CP_COLORKEY_F},     
  {RGB_BK_COLOR_Yellow,      CP_COLORKEY_F},     
//Color1_1 NoSelect 
  {MainBackGround,      CP_COLORKEY_F},      //0x88
  {RGB_FrameMidLine,      CP_COLORKEY_F},     
  {RGB_Picture_Cyan,      CP_COLORKEY_F},     
  {RGB_Picture_Purple,      CP_COLORKEY_F},     
//Color1_1 Select 
  {SubBackGround,      CP_COLORKEY_F},      //0x8C
  {RGB_FrameMidLine,      CP_COLORKEY_F},     
  {RGB_Picture_Cyan,      CP_COLORKEY_F},     
  {RGB_Picture_Purple,      CP_COLORKEY_F},     
//Color1_2 NoSelect 
  {MainBackGround,      CP_COLORKEY_F},      //0x90
  {RGB_FrameMidLine,      CP_COLORKEY_F},     
  {RGB_Picture_Cyan,      CP_COLORKEY_F},     
  {RGB_WHITE,      CP_COLORKEY_F},     
//Color1_2 Select 
  {SubBackGround,      CP_COLORKEY_F},      //0x94
  {RGB_FrameMidLine,      CP_COLORKEY_F},     
  {RGB_Picture_Cyan,      CP_COLORKEY_F},     
  {RGB_WHITE,      CP_COLORKEY_F},     

//System NoSelect 
  {MainBackGround,      CP_COLORKEY_F},      //0x98
  {RGB_FrameMidLine,      CP_COLORKEY_F},     
  {RGB_WHITE,      CP_COLORKEY_F},     
  {RGB_BK_COLOR_R,      CP_COLORKEY_F},     
//System Select 
  {SubBackGround,      CP_COLORKEY_F},      //0x9C
  {RGB_FrameMidLine,      CP_COLORKEY_F},     
  {RGB_WHITE,      CP_COLORKEY_F},     
  {RGB_BK_COLOR_R,      CP_COLORKEY_F},     

//Gauge Select 
  {RGB_Frame,      CP_COLORKEY_F},      //0xA0
  {RGB_FrameMidLine,      CP_COLORKEY_F},     
  {RGB_Frame,      CP_COLORKEY_F},     
  {MainBackGround,      CP_COLORKEY_F},     

//MSG Frame 
  {MainBackGround,      CP_COLORKEY_F},      //0xA4
  {RGB_Frame,      CP_COLORKEY_F},     
  {RGB_SubFrame,      CP_COLORKEY_F},     
  {MainBackGround,      CP_COLORKEY_F},     


};

#endif  ///


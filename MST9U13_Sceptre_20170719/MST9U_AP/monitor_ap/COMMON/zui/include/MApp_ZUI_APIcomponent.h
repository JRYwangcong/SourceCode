////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (��MStar Confidential Information��) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _MAPP_ZUI_APICOMPONENT_H
#define _MAPP_ZUI_APICOMPONENT_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#define INTERFACE extern


#ifndef _ZUI_INTERNAL_INSIDE_
#error  "This include file is ZUI internal file, cannot be included here!"
#endif



/**
 * UI drawing component: rectangle border
 *
*/
typedef struct _DRAW_RECT_BORDER
{
    //OSD_COLOR_FMT eOSD_COLOR_FMT;
    OSD_COLOR colorLeftTop;
    OSD_COLOR colorRightDown;
    U8 thickness;
    LINE_TYPE lt;
} DRAW_RECT_BORDER;

//extern DRAW_RECT_BORDER g_DrawRectBorder[];

/**
 * UI drawing component: fill rectangle (pure color/gradient color)
 *
*/
typedef struct _DRAW_FILL_RECT
{
    //OSD_COLOR_FMT eOSD_COLOR_FMT;
    OSD_COLOR fromColor;
    OSD_COLOR toColor;
    OSD_GRADIENT eGradient;
} DRAW_FILL_RECT;

//extern DRAW_FILL_RECT g_DrawFillRect[];
/*
typedef struct _RECT_BORDER_EX
{
    //OSD_COLOR_FMT eOSD_COLOR_FMT;
    OSD_COLOR colorTopLeft;
    OSD_COLOR  colorTopRight;
    OSD_COLOR  colorRightTop;
    OSD_COLOR colorRightDown;
    OSD_COLOR colorDownRight;
    OSD_COLOR colorDownLeft;
    OSD_COLOR  colorLeftDown;
    OSD_COLOR  colorLeftTop;
    U8  thickness;
    LINE_TYPE lt;
} DRAW_RECT_BORDER_EX;
*/

//extern DRAW_RECT_BORDER_EX g_DrawRectBorderEx[];
/*
typedef struct _DRAW_RECT_3D
{
    //OSD_COLOR_FMT eOSD_COLOR_FMT;
    OSD_COLOR COLOR_3DBORDER_DARK;
    OSD_COLOR COLOR_3DBORDER_LIGHT;
    U8 borderWidth;
    U8 flag;
} DRAW_RECT_3D;
*/
//extern DRAW_RECT_3D g_DrawRect3D[];

/**
 * UI drawing component: text/string
 *
*/
typedef struct _DRAW_TEXT_OUT
{
    FONT_ID eSystemFont;
    U16 StringID;//LPTSTR pString;
    //#if ZUI_ENABLE_PALETTE
    //U8 TextColor; //use color index for palette
    //#else
    OSD_COLOR TextColor;
    //#endif
#if 0
    TEXT_ATTRIB eTextAttrib;
#else
    U8 eTextAttrib;
#endif
    U8 flag;
    U8 u8dis;
    //BOOLEAN bShadow;
} DRAW_TEXT_OUT;

/**
 * UI drawing component: dynamic text/string
 *
*/
typedef struct _DRAW_TEXT_OUT_DYNAMIC
{
    FONT_ID eSystemFont;
    LPTSTR pString;
    //OSD_COLOR_FMT eOSD_COLOR_FMT;
    //#if ZUI_ENABLE_PALETTE
    //U8 TextColor; //use color index for palette
    //#else
    OSD_COLOR TextColor;
    //#endif
#if 0
    TEXT_ATTRIB eTextAttrib;
#else
    U8 eTextAttrib;
#endif
    U8 flag;
    U8 u8dis;
    //BOOLEAN bShadow;
    U8 u8dots;
} DRAW_TEXT_OUT_DYNAMIC;


/**
 * UI drawing component: dynamic punctuated string (add dots if truncated)
 *
*/
typedef struct _DRAW_PUNCTUATED_DYNAMIC
{
    FONT_ID eSystemFont;
    LPTSTR pString;
    //OSD_COLOR_FMT eOSD_COLOR_FMT;
    //#if ZUI_ENABLE_PALETTE
    //U8 TextColor; //use color index for palette
    //#else
    OSD_COLOR TextColor;
    //#endif
#if 0
    TEXT_ATTRIB eTextAttrib;
#else
    U8 eTextAttrib;
#endif
    U8 flag;
    U8 u8dis;
    //BOOLEAN bShadow;
    U8 max_row;
} DRAW_PUNCTUATED_DYNAMIC;

//extern DRAW_TEXT_OUT g_DrawTextOut[];

/**
 * UI drawing component: bitmap (with color key)
 *
*/
typedef struct _DRAW_BITMAP
{
     //BMPHANDLE handle;
     U16 u16BitmapIndex;
     //U32 bmpWidth, bmpHeight, bmpTotalBytes;
     //OSD_COLOR_FMT bmpColorFmt;
     BOOLEAN bSrcColorKey;
     OSD_COLOR srcColorKeyFrom;
     OSD_COLOR srcColorKeyEnd;
     U8 u8Constant_Alpha;
} DRAW_BITMAP;

//extern DRAW_BITMAP g_DrawBitmap[];

/*
typedef struct _DRAW_ROUND_BAR
{
    //OSD_COLOR_FMT eOSD_COLOR_FMT;
    OSD_COLOR colorFrom;
    OSD_COLOR colorTo;
    OSD_GRADIENT egradient;
    BOOLEAN bshadow;
    ROUNDBAR_STYLE eOSD_RoundBar;
} DRAW_ROUND_BAR;
*/

//extern DRAW_ROUND_BAR g_DrawRoundBar[];
/*
typedef struct _DRAW_WINDOW
{
    //OSD_COLOR_FMT eOSD_COLOR_FMT;
    S16 titleHeight;
    OSD_COLOR colorTitle;
    OSD_COLOR colorBkgnd;
    RECT_ATTRIB attrib;
    OSD_COLOR BroderColor;
    U8 sizeBorder;
    BOOLEAN bShadow;
} DRAW_WINDOW;
*/

//extern DRAW_WINDOW g_DrawWindow[];

/**
 * UI drawing component: rectangle (can be gradient and round)
 *
*/
typedef struct _DRAW_RECT
{
    //OSD_COLOR_FMT eOSD_COLOR_FMT;
    OSD_COLOR RcolorFrom;
    OSD_COLOR RcolorTo;
    OSD_GRADIENT eRectGradient;
    OSD_COLOR BroderColor;
    RECT_ATTRIB attrib;
    U8 sizeBorder;
    //BOOLEAN bShadow;
    U8 radius;
} DRAW_RECT;

//extern DRAW_RECT g_DrawRect[];
/*
typedef struct _DRAW_DASH_BAR
{
    //OSD_COLOR_FMT eOSD_COLOR_FMT;
    OSD_COLOR colorFrom;
    OSD_COLOR colorTo;
    BOOLEAN bXdirection;
    U8 dash_len;
} DRAW_DASH_BAR;
*/

//extern DRAW_DASH_BAR g_DrawDashBar[];

/**
 * UI drawing component: triangle (unused)
 *
*/
typedef struct _DRAW_TRIANGLE
{
    //OSD_COLOR_FMT eOSD_COLOR_FMT;
    U8 style; //TRIANGLE_STYLE
    OSD_COLOR color;
} DRAW_TRIANGLE;

//extern DRAW_TRIANGLE g_DrawTriangle[];

/*
typedef struct _DRAW_CIRCLE
{
     //OSD_COLOR_FMT eOSD_COLOR_FMT;
     OSD_COLOR colorFrom;
     OSD_COLOR colorTo;
     BOOLEAN bShadow;
} DRAW_CIRCLE;
*/

//extern DRAW_CIRCLE g_DrawCircle[];

/*
typedef struct _DRAW_ROUND_BORDER
{
      U32 radius;
      U16 u16BorderWidth;
      //OSD_COLOR_FMT eOSD_COLOR_FMT;
      OSD_COLOR BorderColor;
} DRAW_ROUND_BORDER;
*/

/**
 * UI drawing component: line (unused)
 *
*/
typedef struct _DRAW_LINE
{
      S16 x1, y1, x2, y2;
      OSD_COLOR color;
      U8 width;
      U8 alpha;
} DRAW_LINE;

/**
 * UI drawing component: JPEG (unused)
 *
*/
typedef struct _DRAW_JPEG
{
      U8 index;
} DRAW_JPEG;

INTERFACE DRAW_BITMAP _ZUI_TBLSEG _Zui_Bitmap_List[];
INTERFACE DRAW_FILL_RECT _ZUI_TBLSEG _Zui_FillRect_List[];
INTERFACE DRAW_TEXT_OUT _ZUI_TBLSEG _Zui_TextOut_List[];
INTERFACE DRAW_RECT _ZUI_TBLSEG _Zui_Rect_List[];
INTERFACE DRAW_RECT_BORDER _ZUI_TBLSEG _Zui_RectBorder_List[];

#undef INTERFACE

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MAPP_ZUI_APICOMPONENT_H */

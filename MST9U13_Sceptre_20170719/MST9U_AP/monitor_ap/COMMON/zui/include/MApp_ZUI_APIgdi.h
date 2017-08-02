////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (MStar Confidential Information) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _MAPP_ZUI_APIGDI_H
#define _MAPP_ZUI_APIGDI_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include "msAPI_OSD.h"

#ifdef MAPP_ZUI_APIGDI_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

//ZUI_ONSCREEN_PINPON_BUFFER: create another onscreen buffer for pinpon buffer
//can reduce tearing effect caused by GE slow bitblt
//if UI is not too sophisticated, this definition could be set to 0 and could reduce on buffer
#define ZUI_ONSCREEN_PINPON_BUFFER         1

///////////////////////////////////////////////
// from apiOSD

/**
 * \var RECT
 * \brief integer rectangle data structure - rectangle (16 bits integer)
 *
*/
typedef struct _RECT
{
   S16  left;
   S16  top;
   S16  width;
   S16  height;
} RECT;

/**
 * \def RECT_SET
 * \brief initial rectangle - initial rectangle position and size
 *
*/
#define RECT_SET(r, l, t, w, h) \
        { r.top = t; r.left = l; r.width = w; r.height = h; }

/**
 * \def RECT_SHIFT
 * \brief shift rectangle position - shift rectangle position with x,y offset
 *
*/
#define RECT_SHIFT(r, w, h) \
        { r.left += w; r.top += h; }

/**
 * \def RECT_SCALE
 * \brief adjust rectangle smaller - scale rectangle position to inner side
 *
*/
#define RECT_SCALE(r, w, h)  \
        { r.left += w; r.top += h; r.width -= (w<<1); r.height -= (h<<1); }

/**
 * \def RECT_REALIZE
 * \brief move rectangle position - add rectangle2 position to rectangle1
 *
*/
#define RECT_REALIZE(r1, r2) \
        {r1.left += r2.left; r1.top += r2.top;}

/**
 * \def RECT_MAP
 * \brief move rectangle position - subtract rectangle2 position to rectangle1
 *
*/
#define RECT_MAP(r1, r2) \
        {r1.left -= r2.left; r1.top -= r2.top;}

/**
 * \def ARGB
 * \brief make 32bits color - return 32bits color by RGB and alpha value
 *
*/
#define ARGB(a, r, g, b)  ((a&0xff)<<24 | (r&0xff) << 16 | (g&0xff) <<8 | (b&0xff))


/**
 * \var OSD_COLOR
 * \brief system independent OSD color
 *
*/
typedef U32 OSD_COLOR;

/**
 * \var FONT_ID
 * \brief system dependent font handle
 *
*/
typedef U8 FONT_ID;


/**
 * \var TRIANGLE_STYLE
 * \brief triangle type
 *
*/
typedef enum _TriangleStyle
{

/*
    _
    \|
*/
   eTriangle1,
/*
    |\
     -

*/
   eTriangle2,
/*
    /|
     -

*/

   eTriangle3,
/*
     _
    |/
*/
   eTriangle4,
/*
    <|
*/
   eTriangle5,
/*
    |>
*/

   eTriangle6,

/*
    _
    \/

*/
   eTriangle7,

/*
    /\
     -
*/
   eTriangle8
}TRIANGLE_STYLE;

/**
 * \var TEXT_ATTRIB
 * \brief text alignment type (left, center, right)
 *
*/
typedef enum _TextAttrib
{
   eTextAlignLeft           = 0x1,
   eTextAlignMiddle         = 0x2,
   eTextAlignRight          = 0x4,
   eTextAlignMiddleWH       = 0x8,
   eTextAlignLeft_MiddleH   = 0x10,
   eTextAlignRight_MiddleH  = 0x20,
}TEXT_ATTRIB;

/*
typedef enum
{
    ///horizontal round bar
    OSD_HRoundBar,
    ///Vertical round bar
    OSD_VRoundBar,
} ROUNDBAR_STYLE;
*/

/**
 * \var RECT_ATTRIB
 * \brief rectangle type (round or not)
 *
*/
typedef enum
{
   eRectBorder     ,
   eRectBorderRound  ,
}RECT_ATTRIB;

/**
 * \var OSD_GRADIENT
 * \brief gradient type (none, x, y, x-center, y-center,...)
 *
*/
typedef enum
{
    ///NO GRADIENT
    OSD_GRADIENT_DISABLE = CONSTANT_COLOR,
    ///GRADIENT based on X axis
    OSD_COLOR_GRADIENT_X = GRADIENT_X_COLOR,
    ///GRADIENT based on Y axis
    OSD_COLOR_GRADIENT_Y = GRADIENT_Y_COLOR,
    ///GRADIENT based on X axis (center)
    OSD_COLOR_GRADIENT_X_CENTER = GRADIENT_X_CENTER_COLOR,
    ///GRADIENT based on Y axis (center)
    OSD_COLOR_GRADIENT_Y_CENTER = GRADIENT_Y_CENTER_COLOR,
    ///GRADIENT based on X+Y axis (center)
    OSD_COLOR_GRADIENT_XY = GRADIENT_XY_COLOR,

}OSD_GRADIENT;

/**
 * \var LINE_TYPE
 * \brief line type (solid or dash)
 *
*/
typedef enum _LineType
{
   eSolidLine,
   eDashLine
} LINE_TYPE;


///////////////////////////////////////////////

/**
 * \def GRAPHIC_DC
 * \brief system independent structure for low level drawing - we can pass framebuffer ID and alpha value to low level function
 *
*/
typedef struct _GRAPHIC_DC
{
    U8  u8FbID;
    U8  u8ConstantAlpha; //0xFF for disable alpha...
    U8  bSrcAlphaReplaceDstAlpha;

    //U32 u32MemAddr;
    //BOOLEAN bRoughMode;
} GRAPHIC_DC;

/**
 * \def DOUBLE_BUFFER
 * \brief system independent double buffer structure - store two buffer and one GWIN for double buffer drawing flow
 *
*/
typedef struct _DOUBLE_BUFFER
{
    U8     bInit : 1;
    U8     bUsed : 1;
    U8     bLoadPalette : 1;
    U8     bAllowFullScreenRelease : 1;

    //dcMain for display, dcBuff for background temp drawing...
    GRAPHIC_DC  dcMain;
#if (ZUI_ONSCREEN_PINPON_BUFFER)
    GRAPHIC_DC  dcMain2;
#endif
    GRAPHIC_DC  dcBuff;

    //the following properties should be the same between dcMain and dcBuff...
    U8          u8GWinID;
#if (CHIP_ID==MST9U2 || CHIP_ID==MST9U4)
    U8          u8GWin2ID;
#endif
    RECT        rcGWin;
    //U16         u16Pitch;
    //U16         u16FbFmt;
} DOUBLE_BUFFER;

/**
 * \def PAINT_PARAM
 * \brief paint message structure - information from the painting handler (main frame), ex: invalide region and window current state
 *
*/
typedef struct _PAINT_PARAM
{
    GRAPHIC_DC dc;
    RECT * rect; //rectangle of current drawing UI control
    RECT * rect_invalidate; //rectangle of invalid region of current redrawing
    //U8  u8ConstantAlpha; //0xFF for disable alpha...
    BOOLEAN bIsFocus;
    BOOLEAN bIsDisable;
    BOOLEAN bIsVisible; //MSG_PAINT on invisible state??
} PAINT_PARAM;

/**
 * \var OSDPAGE_RotateAngle
 * \brief global osd page table
 *
*/
typedef enum
{
    /// Do not rotate
    OSD_ROTATE_0,
    /// Rotate 90 degree
    OSD_ROTATE_90,
    /// Rotate 180 degree
    OSD_ROTATE_180,
    /// Rotate 270 degree
    OSD_ROTATE_270,

    OSD_ROTATE_NUM
} OSDPAGE_RotateAngle;

INTERFACE OSDClrBtn clrBtn1;
INTERFACE OSDClrLine g_ClrLine;
INTERFACE MSAPI_GEBitmapFmt gbmpfmt;
INTERFACE U16 gScreenWidth;
INTERFACE U16 gScreenHeight;

INTERFACE void MApp_ZUI_API_InitGDIVariables(void);
INTERFACE BOOLEAN MApp_ZUI_API_InitGDI(RECT * rect_gwin);
INTERFACE void _MApp_ZUI_API_InitPalette(U16 B_ID_PAL);
INTERFACE void MApp_ZUI_API_TerminateGDI( void );
INTERFACE GRAPHIC_DC* MApp_ZUI_API_GetBufferDC(void);
INTERFACE GRAPHIC_DC* MApp_ZUI_API_GetScreenDC(void);
INTERFACE void MApp_ZUI_API_PreReleaseDC(void);
INTERFACE void MApp_ZUI_API_PostReleaseDC(void);
INTERFACE void MApp_ZUI_API_ReleaseDC(RECT * rect_invalidate);
INTERFACE void MApp_ZUI_API_ReleaseFullScreenDC(void);
INTERFACE void MApp_ZUI_API_ReleaseWithoutUpdateDC(void);
INTERFACE U8 MApp_ZUI_API_QueryGWinID(void);
INTERFACE U8 MApp_ZUI_API_QueryGWin2ID(void);
INTERFACE void MApp_ZUI_API_QueryGWinRect(RECT* rect);
INTERFACE void MApp_ZUI_API_SetGWinPosition(U16 x, U16 y);
INTERFACE void MApp_ZUI_API_GetGWinPosition(U16* x, U16* y);
INTERFACE void MApp_ZUI_API_EnableFullScreenRelease(BOOLEAN bEnable);
INTERFACE BOOLEAN MApp_ZUI_API_IsAllowFullScreenRelease(void);

INTERFACE BOOLEAN MApp_ZUI_API_DoesIntersect (const RECT* psrc1, const RECT* psrc2);
INTERFACE void MApp_ZUI_API_UnionRect(RECT* pdrc, const RECT* psrc1);
INTERFACE void MApp_ZUI_API_IntersectRect(RECT* pdrc, const RECT* psrc1);
INTERFACE BOOLEAN MApp_ZUI_API_DoesContain (const RECT* pBig, const RECT* pInside);

INTERFACE GRAPHIC_DC MApp_ZUI_API_CreateDC(U16 w, U16 h);
INTERFACE void MApp_ZUI_API_ClearDC(GRAPHIC_DC *pDC, U32 u32Color);
INTERFACE void MApp_ZUI_API_ClearDCRect(GRAPHIC_DC *pDC, U32 u32Color, RECT* pRect);
INTERFACE void MApp_ZUI_API_DeleteDC(GRAPHIC_DC dc);

INTERFACE U8 MApp_ZUI_API_ZuiFormatToGeFormat(U8 u8ZuiFormat);
INTERFACE U8 MApp_ZUI_API_ZuiFormatToGopFormat(U8 u8ZuiFormat);
INTERFACE U8 MApp_ZUI_API_GetBPP(U8 u8ZuiFormat);
INTERFACE void MApp_ZUI_API_PrepareBufferDC(GRAPHIC_DC *pDC);
INTERFACE OSDPAGE_RotateAngle MApp_ZUI_API_GetCurrentRotateAngle(void);
INTERFACE void MApp_ZUI_API_DoRotate(OSDPAGE_RotateAngle angle, BOOLEAN bShowAfterRotate);
INTERFACE void MApp_ZUI_API_DoRotateBufferWithoutRotateGWin(OSDPAGE_RotateAngle angle, BOOLEAN bShowAfterRotate);

#undef INTERFACE

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MAPP_ZUI_APIGDI_H */


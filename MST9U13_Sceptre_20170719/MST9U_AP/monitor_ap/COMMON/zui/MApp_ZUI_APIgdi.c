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

#define MAPP_ZUI_APIGDI_C
#define _ZUI_INTERNAL_INSIDE_ //NOTE: for ZUI internal


///////////////////////////////////////////////////////////////////////////////////////////////////
// Description: GUI GDI functions
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include <stdlib.h>
#include <string.h>
#include "Board.h"
#include "MsCommon.h"
#include "Panel.h"
#include "drvOSDblend.h"
#include "msAPI_BDMA.h"
#include "apiGOP.h"
#include "msAPI_OSD.h"
#include "apiXC.h"
#include "apiXC_Adc.h"
//#include "MApp_GlobalSettingSt.h"
//#include "MApp_GlobalVar.h"
#include "OSDcp_Bitmap_EnumIndex.h"
#include "MApp_ZUI_Main.h"
#include "MApp_ZUI_APIcommon.h"
#include "MApp_ZUI_APIwindow.h"
#include "MApp_ZUI_APItables.h"
#include "MApp_ZUI_APIgdi.h"
#include "MApp_ZUI_APIstyletables.h"
#include "MApp_ZUI_ACTglobal.h"
#include "sysinfo.h"

#if (MPLAYER_BITMAP_FORMAT == MPLAYER_I8)
#include "apiGOP.h"
#include "SysInit.h"
#endif

#define ZUI_DBG(x) //x

//-------------------------------------------------------------------------------------------------
// Local Defines
//------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Local Variables
//-------------------------------------------------------------------------------------------------

static DOUBLE_BUFFER    _ScreenGC;
static U16 g_nBufferWidth = 0;
static U16 g_nBufferHeight = 0;
static U16 g_nBufferAlignWidth = 0;
static U16 g_nBufferAlignHeight = 0;
//static BOOLEAN _bEnableAlpha;
//static U8 _u8Alpha;

extern BOOLEAN _MApp_ZUI_API_WindowProcOnIdle(void);

#if 1   // Fixme: MPLAYER I8 mode is not used
#if (MPLAYER_BITMAP_FORMAT == MPLAYER_I8)
///////////////////////////////////////////////////////////////////////////////
///  init GOP/GE I8 palette
///
///  @return no return value
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////

void _MApp_ZUI_API_InitPalette(U16 B_ID_PAL)
{

    BININFO BinInfo;
    GOP_PaletteEntry *pPalEntry = 0;
    U8 *pPalData = 0;
    U8 u8Rp, u8Gp, u8Bp, u8Ap; //palette RGB
    U16 u16Index;
    BOOLEAN bResult;

    //U8 u8Rr, u8Gr, u8Br; // read out RGB
    //U8 u8Error = 0;
    //U8 u8Repeat = 0;

    //NOTE: convert RGB(8,8,8) + alpha(8) bin file => GE palette ARGB(6,6,6,6)
    //   assume two bin file size are 256*3=768 and 256 bytes
    //   we can allow less then 256 color, but not allow more then 256

    BinInfo.B_ID = B_ID_PAL;

    MDrv_Sys_Get_BinInfo(&BinInfo, &bResult);
    if ( bResult != PASS)
    {
        ZUI_DBG_FAIL(printf("[]pal_err1: ID=0x%X\n", BinInfo.B_ID));
        return;
    }

    if (BinInfo.B_Len > (768+8))
    {
        ZUI_DBG_FAIL(printf("[]pal_err2: ID=0x%X\n", BinInfo.B_ID));
        return;
    }

    MApi_BDMA_Copy(BinInfo.B_FAddr, BMP_PALETTE_BUF_START_ADR, ((768 + 0x07) & ~0x07), MIU_FLASH2SDRAM);

    BinInfo.B_ID = BIN_ID_PALALPHA_ZUI;

    MDrv_Sys_Get_BinInfo(&BinInfo, &bResult);
    if ( bResult != PASS)
    {
        ZUI_DBG_FAIL(printf("[]pal_err3: ID=0x%X\n", BinInfo.B_ID));
        return;
    }

    if (BinInfo.B_Len > (256+8))
    {
        ZUI_DBG_FAIL(printf("[]pal_err4: ID=0x%X\n", BinInfo.B_ID));
        return;
    }

    MApi_BDMA_Copy(BinInfo.B_FAddr, BMP_PALETTE_BUF_START_ADR+768, ((256 + 0x07) & ~0x07), MIU_FLASH2SDRAM);

    pPalData = (U8 xdata *) BMP_PALETTE_BUF_START_ADR;
    pPalEntry = (GOP_PaletteEntry xdata *) (BMP_PALETTE_BUF_START_ADR+1024);

    // convert palette to ARGB8888
    for (u16Index = 0; u16Index < 256; u16Index++)
    {
        u8Rp = pPalData[u16Index*3+0];
        u8Gp = pPalData[u16Index*3+1];
        u8Bp = pPalData[u16Index*3+2];
        u8Ap = pPalData[768+u16Index];

        /*
        u8Ap = (u8Ap & 0xFC) | ((u8Ap & 0xC0) >> 6);
        u8Rp = (u8Rp & 0xFC) | ((u8Rp & 0xC0) >> 6);
        u8Gp = (u8Gp & 0xFC) | ((u8Gp & 0xC0) >> 6);
        u8Bp = (u8Bp & 0xFC) | ((u8Bp & 0xC0) >> 6);
        */

        /*if (u16Index == 1) //u32Color == 0xFFFFFF) //testing...
        {
            u8Rp = 0xCC;
            u8Gp = 0x77;
            u8Bp = 0x22;
            u8Ap = 0xFF;
        }*/

        pPalEntry[u16Index].u8Data[0] = u8Ap;
        pPalEntry[u16Index].u8Data[1] = u8Rp;
        pPalEntry[u16Index].u8Data[2] = u8Gp;
        pPalEntry[u16Index].u8Data[3] = u8Bp;
    }

    /*
    while (u8Repeat < MAX_LOAD_PALETTE_REPEAT)
    {

        u8Error = 0;
        MDrv_OSDE_Palette_Set(pPalEntry, 256);

        // verify the palette values are all correct
        for (u16Index = 0; u16Index < 256; u16Index++)
        {
            MDrv_OSDE_Palette_Get(u16Index, &u8Rr, &u8Gr, &u8Br);

            u8Rp = pPalEntry[u16Index].u8R;
            u8Gp = pPalEntry[u16Index].u8G;
            u8Bp = pPalEntry[u16Index].u8B;

            if ((u8Rp != u8Rr) || (u8Gp != u8Gr) || (u8Bp != u8Br))
            {
                printf("Palette[%d] error, SrcRGB(%bu,%bu,%bu), OSDRGB(%bu,%bu,%bu)\n", u16Index,
                        pPalEntry[u16Index].u8R, pPalEntry[u16Index].u8G, pPalEntry[u16Index].u8B,
                        u8Rr, u8Gr, u8Br);
                u8Error++;
            }
        }

        if (u8Error == 0)
        {
            printf("[%bu] Palette is loaded correctly!\n", u8Repeat);

            break; // successfully load palette
        }
        else
        {
            printf("[%bu] Palette load FAIL!\n", u8Repeat);
            u8Repeat++;
            MDrv_OSDE_Palette_Reset();
        }
    }
    */

    MApi_GOP_GWIN_SetPaletteOpt(pPalEntry, 0, 255, E_GOP_PAL_ARGB8888);

    _ScreenGC.bLoadPalette = TRUE;
}

///////////////////////////////////////////////////////////////////////////////
///  private  _MApp_ZUI_API_FadePaletteAlpha
///  fade in/out alpha values in GOP/GE I8 palette
///
///     @param [in]     u8Ratio U8      alpha blending ratio (0~255)
///
///  @return no return value
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////
void _MApp_ZUI_API_FadePaletteAlpha(U8 u8Ratio)
{
    GOP_PaletteEntry *pPalEntry = 0;
    U8 *pPalData = 0;
    U8 u8Rp, u8Gp, u8Bp, u8Ap; //palette RGB
    U16 u16Index;

    //NOTE: convert RGB(8,8,8) + alpha(8) bin file => GE palette ARGB(6,6,6,6)
    //   assume two bin file size are 256*3=768 and 256 bytes

    //NOTE: originally use MApi_GOP_GWIN_ReadPalette(),
    //   but that function sometimes get wrong result...

    pPalData = (U8 xdata *) BMP_PALETTE_BUF_START_ADR;
    pPalEntry = (GOP_PaletteEntry xdata *) (BMP_PALETTE_BUF_START_ADR+1024);

    // convert palette to ARGB8888
    for (u16Index = 0; u16Index < 256; u16Index++)
    {
        u8Rp = pPalData[u16Index*3+0];
        u8Gp = pPalData[u16Index*3+1];
        u8Bp = pPalData[u16Index*3+2];
        u8Ap = (U16)pPalData[768+u16Index]*u8Ratio/255;

        /*
        u8Ap = (u8Ap & 0xFC) | ((u8Ap & 0xC0) >> 6);
        u8Rp = (u8Rp & 0xFC) | ((u8Rp & 0xC0) >> 6);
        u8Gp = (u8Gp & 0xFC) | ((u8Gp & 0xC0) >> 6);
        u8Bp = (u8Bp & 0xFC) | ((u8Bp & 0xC0) >> 6);
        */

        pPalEntry[u16Index].u8Data[0] = u8Ap;
        pPalEntry[u16Index].u8Data[1] = u8Rp;
        pPalEntry[u16Index].u8Data[2] = u8Gp;
        pPalEntry[u16Index].u8Data[3] = u8Bp;
    }

    MApi_GOP_GWIN_SetPaletteOpt(pPalEntry, 0, 255, E_GOP_PAL_ARGB8888);

}
#endif //ZUI_ENABLE_PALETTE
#endif // #if 0

///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_API_InitGDIVariables
///  init GDI variables
///
///  @return no return values
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////
void MApp_ZUI_API_InitGDIVariables(void)
{
    memset(&_ScreenGC, 0, sizeof(_ScreenGC));
}

///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_API_InitGDI
///  init GOP/GE system and allocate buffers for drawing
///
///  @param [in]       rect_gwin const RECT *     gwin position and size
///
///  @return BOOLEAN TRUE for success, FALSE for fail
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////
BOOLEAN MApp_ZUI_API_InitGDI(RECT * rect_gwin)
{

    //NOTE: referenced from MApp_MHEG5_Main.c
    //      some special restrictions for x, y, width, height...

    //U8 u8ZuiFmt = MPLAYER_BITMAP_FORMAT;
    U8 gefmt;
    U8 fbfmt;
    U8 bpp;
    U16 u16Height;
    U16 u16Width;
    U8 u8FbID;
    U8 u8GWinID;
#if (CHIP_ID==MST9U2 || CHIP_ID==MST9U4)
    U8 u8GWin2ID;
#endif
    BOOLEAN bEnableAlpha;
    U8 u8GopAlpha;
    U8 nRet = 0;

    if(_ScreenGC.bInit )
        return FALSE;

    _ScreenGC.bUsed = FALSE;
    _ScreenGC.bAllowFullScreenRelease = FALSE; //default value

    u16Height = rect_gwin->height;
    u16Width = rect_gwin->width;

    if ((g_bOSDPAGE_ROTATE_BUFFER == FALSE) && (OSDPAGE_ROTATE_ANGLE == OSD_ROTATE_90 || OSDPAGE_ROTATE_ANGLE == OSD_ROTATE_270))
    {
        // protection: bounding right must not large that UI_OSD_PANE_W
        if ( (rect_gwin->left+u16Height) > PanelWidth )
        {
            u16Height = PanelWidth - rect_gwin->left;
        }
    }
    else
    {
        // protection: bounding right must not large that UI_OSD_PANE_W
        if ( (rect_gwin->left+u16Width) > PanelWidth )
        {
            u16Width = PanelWidth - rect_gwin->left;
        }
    }

    gefmt = MApp_ZUI_API_ZuiFormatToGeFormat(MPLAYER_BITMAP_FORMAT);
    fbfmt = MApp_ZUI_API_ZuiFormatToGopFormat(MPLAYER_BITMAP_FORMAT);
    bpp = MApp_ZUI_API_GetBPP(MPLAYER_BITMAP_FORMAT);
    g_nBufferWidth = u16Width;
    g_nBufferHeight = u16Height;
    g_nBufferAlignWidth = GE_ALIGNED_VALUE(u16Width*bpp, 32) / bpp;
    g_nBufferAlignHeight = GE_ALIGNED_VALUE(u16Height*bpp, 32) / bpp;
    rect_gwin->left = ( (rect_gwin->left) & ~((32/bpp)-1) );

    MApi_GOP_GWIN_SwitchGOP(OSD_GOP_ID);

    MApi_GOP_GWIN_SetForceWrite(TRUE);
    /////////////////////////////////////////////////////////
    //Step1: create buffer dc + gwin
#if (CHIP_ID==MST9U2 || CHIP_ID==MST9U4)
    nRet = MApi_GOP_GWIN_Create2Win(g_nBufferAlignWidth, g_nBufferAlignHeight, gefmt, &u8GWinID, &u8GWin2ID, OSD_GOP_RIGHT_ID);

    if (nRet == GWIN_NO_AVAILABLE)
    {
        printf("[ZUIGDIWIN\n");
        ABORT();
        return FALSE;
    }
#else
    if ((g_bOSDPAGE_ROTATE_BUFFER == TRUE) && (OSDPAGE_ROTATE_ANGLE == OSD_ROTATE_90 || OSDPAGE_ROTATE_ANGLE == OSD_ROTATE_270))
        u8GWinID = MApi_GOP_GWIN_CreateWin(g_nBufferAlignHeight, g_nBufferAlignWidth, gefmt);
    else
       u8GWinID = MApi_GOP_GWIN_CreateWin(g_nBufferAlignWidth, g_nBufferAlignHeight, gefmt);

    if (u8GWinID == GWIN_NO_AVAILABLE)
    {
        printf("[ZUIGDIWIN\n");
        ABORT();
        return FALSE;
    }
#endif

    u8FbID = MApi_GOP_GWIN_GetFBfromGWIN(u8GWinID);

#if (CHIP_ID==MST9U2 || CHIP_ID==MST9U4)
    MApi_GOP_GWIN_Set2WinPosition(u8GWinID, u8GWin2ID, rect_gwin->left, rect_gwin->top);
#else
    MApi_GOP_GWIN_SetWinPosition(u8GWinID, rect_gwin->left, rect_gwin->top);
#endif

    MApi_GOP_GWIN_Switch2Gwin(u8GWinID);

    msAPI_OSD_SetClipWindow(0, 0, g_nBufferAlignWidth, g_nBufferAlignHeight);

    #if (MPLAYER_BITMAP_FORMAT == MPLAYER_I8)
    {
        bEnableAlpha = TRUE; //alpha table is depend by 256 color table
        u8GopAlpha = 63;
    }
    #elif (MPLAYER_BITMAP_FORMAT == MPLAYER_ARGB4444 || MPLAYER_BITMAP_FORMAT == MPLAYER_ARGB8888 || MPLAYER_BITMAP_FORMAT == MPLAYER_I8)
    {
        //for ARGB4444, ARGB8888 disable GOP alpha
        bEnableAlpha = TRUE;
        u8GopAlpha = 63;
    }
    #elif (MPLAYER_BITMAP_FORMAT == MPLAYER_ARGB1555)
    {
        bEnableAlpha = TRUE;
        u8GopAlpha = OSDPAGE_BLENDING_VALUE;
    }
    #else
    {
        bEnableAlpha = FALSE;
        u8GopAlpha = OSDPAGE_BLENDING_VALUE;
    }
    #endif

    MApi_GOP_GWIN_SetBlending(u8GWinID, bEnableAlpha, u8GopAlpha);
    MApi_GFX_SetAlpha(FALSE, COEF_CONST, ABL_FROM_CONST, 0xFF); //=> msAPI_OSD_DisableAlpha();

    #if (MPLAYER_BITMAP_FORMAT == MPLAYER_I8)
    _MApp_ZUI_API_InitPalette(BIN_ID_PAL_ZUI); //before set palette for GOP
    MApi_GOP_GWIN_SetTransClr(GOPTRANSCLR_FMT1, 0); //NOTE: index color 0 is transparent
    MApi_GOP_GWIN_EnableTransClr(GOPTRANSCLR_FMT1,TRUE);
    #elif (MPLAYER_BITMAP_FORMAT == MPLAYER_ARGB4444)
    MApi_GOP_GWIN_SetTransClr(GOPTRANSCLR_FMT2, ZUI_COLOR_TRANSPARENT_RGB444);
    MApi_GOP_GWIN_EnableTransClr(GOPTRANSCLR_FMT2, TRUE);
    #else
    MApi_GOP_GWIN_SetFMT0TransClr(ZUI_COLOR_TRANSPARENT_RGB555);
    #endif

#if (CHIP_ID==MST9U2 || CHIP_ID==MST9U4)
    MApi_GOP_GWIN_SetBlending(u8GWin2ID, bEnableAlpha, u8GopAlpha);
    MApi_GFX_SetAlpha(FALSE, COEF_CONST, ABL_FROM_CONST, 0xFF); //=> msAPI_OSD_DisableAlpha();

    MApi_GOP_GWIN_SwitchGOP(OSD_GOP_RIGHT_ID);
#if (MPLAYER_BITMAP_FORMAT == MPLAYER_I8)
    _MApp_ZUI_API_InitPalette(BIN_ID_PAL_ZUI); //before set palette for GOP
    MApi_GOP_GWIN_SetTransClr(GOPTRANSCLR_FMT1, 0); //NOTE: index color 0 is transparent
    MApi_GOP_GWIN_EnableTransClr(GOPTRANSCLR_FMT1,TRUE);
#elif (MPLAYER_BITMAP_FORMAT == MPLAYER_ARGB4444)
    MApi_GOP_GWIN_SetTransClr(GOPTRANSCLR_FMT2, ZUI_COLOR_TRANSPARENT_RGB444);
    MApi_GOP_GWIN_EnableTransClr(GOPTRANSCLR_FMT2, TRUE);
#else
    MApi_GOP_GWIN_SetFMT0TransClr(ZUI_COLOR_TRANSPARENT_RGB555);
#endif
    MApi_GOP_GWIN_SwitchGOP(OSD_GOP_ID);
#endif //#if (CHIP_ID==MST9U2)

    // get frame buffer info for background drawing...
    //MDrv_GE_GetFrameBufferInfo(&u16TmpWidth, &u16TmpHeight,
    //    &_ScreenGC.u16Pitch, &_ScreenGC.u16FbFmt,
    //    &_ScreenGC.dcBuff.u32MemAddr);
    //printf("dcBuff.addr=%lx\n", (U32)_ScreenGC.dcBuff.u32MemAddr);

    // don't show anything on the init, so we fill it as transparent...
    clrBtn1.x = 0;
    clrBtn1.y = 0;
    clrBtn1.width = g_nBufferAlignWidth;
    clrBtn1.height = g_nBufferAlignHeight;
    #if (MPLAYER_BITMAP_FORMAT == MPLAYER_I8)
    clrBtn1.b_clr = 0;
    #else
    clrBtn1.b_clr = ZUI_COLOR_TRANSPARENT_RGB8888;
    #endif
    clrBtn1.u8Gradient = CONSTANT_COLOR;
    clrBtn1.fHighLight = FALSE;
    msAPI_OSD_DrawBlock(&clrBtn1);

    //MDrv_GOP_GWIN_Enable(u8GWinID, TRUE);

    //MApi_GOP_GWIN_GetFBInfo(u8FbID,&fbAttr);
    //MDrv_GOP_GWIN_GetWinInfo(u8GWinID, &gwin_OS);

    //while(1);

    //_ScreenGC.dcBuff.bRoughMode = FALSE;
    _ScreenGC.dcBuff.u8ConstantAlpha = 0xFF;
    _ScreenGC.dcBuff.u8FbID = u8FbID;

#if (ZUI_ONSCREEN_PINPON_BUFFER)
    /////////////////////////////////////////////////////////
    //Step2.1: create main2 dc

    u8FbID = MApi_GOP_GWIN_GetFreeFBID();

    if (u8FbID == 0xFF)
    {
        printf("[ZUI]GDIFB1\n");
        ABORT();
        return FALSE;
    }

    if ((g_bOSDPAGE_ROTATE_BUFFER == FALSE) && (OSDPAGE_ROTATE_ANGLE == OSD_ROTATE_90 || OSDPAGE_ROTATE_ANGLE == OSD_ROTATE_270))
        nRet = MApi_GOP_GWIN_CreateFB(u8FbID, 0, 0, g_nBufferAlignHeight, g_nBufferWidth, gefmt);
    else
        nRet = MApi_GOP_GWIN_CreateFB(u8FbID, 0, 0, g_nBufferAlignWidth, g_nBufferHeight, gefmt);

    if (nRet != GOP_API_SUCCESS)
    {
        printf("[ZUI]GDIFB2: nRet=%d\n", nRet);
        ABORT();
        return FALSE;
    }

    //_ScreenGC.dcMain.bRoughMode = FALSE;
    _ScreenGC.dcMain2.u8ConstantAlpha = 0xFF;
    _ScreenGC.dcMain2.u8FbID = u8FbID;

    // note: call the following 4 function to switch to another framebuffer....
#if (CHIP_ID==MST9U2 || CHIP_ID==MST9U4)
    MApi_GOP_GWIN_MapFBTo2Win(u8FbID, u8GWinID, u8GWin2ID);
    MApi_GOP_GWIN_Set2WinPosition(u8GWinID, u8GWin2ID, rect_gwin->left, rect_gwin->top);
#else
    MApi_GOP_GWIN_MapFB2Win(u8FbID, u8GWinID);
    MApi_GOP_GWIN_SetWinPosition(u8GWinID, rect_gwin->left, rect_gwin->top);
#endif //#if (CHIP_ID==MST9U2)
    MApi_GOP_GWIN_Switch2Gwin(u8GWinID);
    msAPI_OSD_SetClipWindow(0, 0, g_nBufferAlignWidth, g_nBufferHeight);
    if ((g_bOSDPAGE_ROTATE_BUFFER == FALSE) && (OSDPAGE_ROTATE_ANGLE == OSD_ROTATE_90 || OSDPAGE_ROTATE_ANGLE == OSD_ROTATE_270))
    {
        msAPI_OSD_SetClipWindow(0, 0, g_nBufferAlignHeight, g_nBufferWidth);
    }

    // don't show anything on the init, so we fill it as transparent...
    clrBtn1.x = 0;
    clrBtn1.y = 0;
    clrBtn1.width = g_nBufferAlignWidth;
    clrBtn1.height = g_nBufferHeight;
    if ((g_bOSDPAGE_ROTATE_BUFFER == FALSE) && (OSDPAGE_ROTATE_ANGLE == OSD_ROTATE_90 || OSDPAGE_ROTATE_ANGLE == OSD_ROTATE_270))
    {
        clrBtn1.width = g_nBufferAlignHeight;
        clrBtn1.height = g_nBufferWidth;
    }
    #if (MPLAYER_BITMAP_FORMAT == MPLAYER_I8)
    clrBtn1.b_clr = 0;
    #else
    clrBtn1.b_clr = ZUI_COLOR_TRANSPARENT_RGB8888;
    #endif
    clrBtn1.u8Gradient = CONSTANT_COLOR;
    clrBtn1.fHighLight = FALSE;
    msAPI_OSD_DrawBlock(&clrBtn1);
#endif

    /////////////////////////////////////////////////////////
    //Step2.2: create main dc

    u8FbID = MApi_GOP_GWIN_GetFreeFBID();

    if (u8FbID == 0xFF)
    {
        printf("[ZUI]GDIFB3\n");
        ABORT();
        return FALSE;
    }

    if ((g_bOSDPAGE_ROTATE_BUFFER == FALSE) && (OSDPAGE_ROTATE_ANGLE == OSD_ROTATE_90 || OSDPAGE_ROTATE_ANGLE == OSD_ROTATE_270))
        nRet = MApi_GOP_GWIN_CreateFB(u8FbID, 0, 0, g_nBufferAlignHeight, g_nBufferWidth, gefmt);
    else
        nRet = MApi_GOP_GWIN_CreateFB(u8FbID, 0, 0, g_nBufferAlignWidth, g_nBufferHeight, gefmt);

    if (nRet != GOP_API_SUCCESS)
    {
        printf("[ZUI]GDIFB4: nRet=%d\n", nRet);
        ABORT();
        return FALSE;
    }

    //_ScreenGC.dcMain.bRoughMode = FALSE;
    _ScreenGC.dcMain.u8ConstantAlpha = 0xFF;
    _ScreenGC.dcMain.u8FbID = u8FbID;

    // note: call the following 4 function to switch to another framebuffer....
#if (CHIP_ID==MST9U2 || CHIP_ID==MST9U4)
    MApi_GOP_GWIN_MapFBTo2Win(u8FbID, u8GWinID, u8GWin2ID);
    MApi_GOP_GWIN_Set2WinPosition(u8GWinID, u8GWin2ID, rect_gwin->left, rect_gwin->top);
#else
    MApi_GOP_GWIN_MapFB2Win(u8FbID, u8GWinID);
    MApi_GOP_GWIN_SetWinPosition(u8GWinID, rect_gwin->left, rect_gwin->top);
#endif //#if (CHIP_ID==MST9U2)
    MApi_GOP_GWIN_Switch2Gwin(u8GWinID);
    msAPI_OSD_SetClipWindow(0, 0, g_nBufferAlignWidth, g_nBufferHeight);
    if ((g_bOSDPAGE_ROTATE_BUFFER == FALSE) && (OSDPAGE_ROTATE_ANGLE == OSD_ROTATE_90 || OSDPAGE_ROTATE_ANGLE == OSD_ROTATE_270))
    {
        msAPI_OSD_SetClipWindow(0, 0, g_nBufferAlignHeight, g_nBufferWidth);
    }

    // don't show anything on the init, so we fill it as transparent...
    clrBtn1.x = 0;
    clrBtn1.y = 0;
    clrBtn1.width = g_nBufferAlignWidth;
    clrBtn1.height = g_nBufferHeight;
    if ((g_bOSDPAGE_ROTATE_BUFFER == FALSE) && (OSDPAGE_ROTATE_ANGLE == OSD_ROTATE_90 || OSDPAGE_ROTATE_ANGLE == OSD_ROTATE_270))
    {
        clrBtn1.width = g_nBufferAlignHeight;
        clrBtn1.height = g_nBufferWidth;
    }
#if (MPLAYER_BITMAP_FORMAT == MPLAYER_I8)
    clrBtn1.b_clr = 0;
#else
    clrBtn1.b_clr = ZUI_COLOR_TRANSPARENT_RGB8888;
#endif
    clrBtn1.u8Gradient = CONSTANT_COLOR;
    clrBtn1.fHighLight = FALSE;
    msAPI_OSD_DrawBlock(&clrBtn1);

    // get frame buffer info for background drawing...
    //MDrv_GE_GetFrameBufferInfo(&u16TmpWidth, &u16TmpHeight,
    //    &_ScreenGC.u16Pitch, &_ScreenGC.u16FbFmt,
    //    &_ScreenGC.dcMain.u32MemAddr);
    //printf("dcMain.addr=%lx\n", (U32)_ScreenGC.dcMain.u32MemAddr);

    /////////////////////////////////////////////////////////
    //Step3: finished...
    MApi_GOP_GWIN_SetForceWrite(FALSE);

    MApi_GFX_FlushQueue();

    MApi_GOP_GWIN_Enable(u8GWinID, TRUE);
#if (CHIP_ID==MST9U2 || CHIP_ID==MST9U4)
    MApi_GOP_GWIN_Enable(u8GWin2ID, TRUE);
#endif

    RECT_SET(_ScreenGC.rcGWin, rect_gwin->left, rect_gwin->top, g_nBufferWidth, g_nBufferHeight);

    _ScreenGC.u8GWinID = u8GWinID;
#if (CHIP_ID==MST9U2 || CHIP_ID==MST9U4)
    _ScreenGC.u8GWin2ID = u8GWin2ID;
#endif


    ///////////////////////
    /// testing....

    /*
    {
        GOP_GwinFBAttr fbAttr;
        MApi_GOP_GWIN_GetFBInfo(_ScreenGC.dcMain.u8FbID, &fbAttr);
        printf("[]width=%u\n", fbAttr.width);
        printf("[]height=%u\n", fbAttr.height);
        printf("[]fbFmt=%x\n", fbAttr.fbFmt);
        printf("[]pitch=%u\n", fbAttr.pitch);
        printf("[]size=%lu\n", fbAttr.size);
    }
    */
    /*
    clrBtn1.x = 100;
    clrBtn1.y = 200;
    clrBtn1.width = 100;
    clrBtn1.height = 200;
    clrBtn1.b_clr = 0xFFFF0000;
    clrBtn1.u8Gradient = CONSTANT_COLOR;
    clrBtn1.fHighLight = FALSE;
    msAPI_OSD_DrawBlock(&clrBtn1);
    ABORT();
    */
    /*
    clrBtn1.x = 20;
    clrBtn1.y = 20;

    gbmpfmt.width = 20;
    gbmpfmt.height = 30;
    gbmpfmt.bBmpColorKeyEnable = FALSE; //bSrcColorKey;
    gbmpfmt.bScale = FALSE;

    //r_clr=((srcColorKeyFrom&0x00ff0000)>>16)&0xff;
    //g_clr=((srcColorKeyFrom&0x0000ff00)>>8)&0xff;
    //b_clr=((srcColorKeyFrom&0x000000ff))&0xff;

    gbmpfmt.clrrange.color_s.a = 0;
    gbmpfmt.clrrange.color_s.r = r_clr;
    gbmpfmt.clrrange.color_s.g = g_clr;
    gbmpfmt.clrrange.color_s.b = b_clr;

    //r_clr=((srcColorKeyEnd&0x00ff0000)>>16)&0xff;
    //g_clr=((srcColorKeyEnd&0x0000ff00)>>8)&0xff;
    //b_clr=((srcColorKeyEnd&0x000000ff))&0xff;

    gbmpfmt.clrrange.color_e.a = 0;
    gbmpfmt.clrrange.color_e.r = r_clr;
    gbmpfmt.clrrange.color_e.g = g_clr;
    gbmpfmt.clrrange.color_e.b = b_clr;

    //PRINTE("\n[DrawBitmap] rect = (%d, %d, %d, %d)", r->left, r->top, r->width, r->height);
    //PRINTE("\n[DrawBitmap] color start = %x, end = %x", srcColorKeyFrom, srcColorKeyEnd);
    //PRINTE("\n[DrawBitmap] handle = %d", handle);
    msAPI_OSD_DrawColorKeyBitmap(1, clrBtn1.x, clrBtn1.y, gbmpfmt);


    ABORT();
    */

    //printf("[%lu,%lu]\n", (U32)u8GWinID, (U32)u8FbID);

    _ScreenGC.bInit = TRUE;

    return TRUE;

}
///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_API_TerminateGDI
///  uninit GOP/GE services and release buffers
///
///  This function doesn't return a value
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////
void MApp_ZUI_API_TerminateGDI( void )
{

    GOP_GwinFBAttr fbAttr;

    if (!_ScreenGC.bInit)
        return;

#if (CHIP_ID==MST9U2)
    MApi_GOP_GWIN_UpdateRegAllBankOnceEx(TRUE);
#endif
    MApi_GOP_GWIN_Enable(_ScreenGC.u8GWinID, FALSE);
#if (CHIP_ID==MST9U2)
    MApi_GOP_GWIN_Enable(_ScreenGC.u8GWin2ID, FALSE);
    MApi_GOP_GWIN_UpdateRegAllBankOnceEx(FALSE);
#endif
	msWriteByteMask(SCL0_20_32,BIT0,BIT0);//Switch output blank polling from VDE falling to Vsync Raising
	WAIT_V_OUTPUT_BLANKING_START();	
	msWriteByteMask(SCL0_20_32,0,BIT0);//

    //clear Frame Buffer to prevent show previous UI when OSD start up
    MApi_GOP_GWIN_GetFBInfo(_ScreenGC.dcMain.u8FbID, &fbAttr);
    if(GFX_SUCCESS != MApi_GFX_ClearFrameBuffer(fbAttr.addr, fbAttr.size, 0xFF))
    {
        MS_DEBUG_MSG(printf("_ScreenGC.dcMain.u8FbID clear fail\n"));
    }
    MApi_GOP_GWIN_GetFBInfo(_ScreenGC.dcBuff.u8FbID, &fbAttr);
    if(GFX_SUCCESS != MApi_GFX_ClearFrameBuffer(fbAttr.addr, fbAttr.size, 0xFF))
    {
        MS_DEBUG_MSG(printf("_ScreenGC.dcBuff.u8FbID clear fail\n"));
    }
#if (CHIP_ID==MST9U2)
    MApi_GOP_GWIN_Delete2Win(_ScreenGC.u8GWinID, _ScreenGC.u8GWin2ID);
#else
    MApi_GOP_GWIN_DeleteWin(_ScreenGC.u8GWinID);
#endif //#if (CHIP_ID==MST9U2)
    //MApi_GOP_GWIN_DeleteWin(u8MainMaskWinId);

#if (ZUI_ONSCREEN_PINPON_BUFFER)
    MApi_GOP_GWIN_DeleteFB(_ScreenGC.dcMain2.u8FbID);
    _ScreenGC.dcMain2.u8FbID = 0xFF;
#endif

    MApi_GOP_GWIN_DeleteFB(_ScreenGC.dcMain.u8FbID);
    _ScreenGC.dcMain.u8FbID = 0xFF;

    MApi_GOP_GWIN_DeleteFB(_ScreenGC.dcBuff.u8FbID);
    _ScreenGC.dcBuff.u8FbID = 0xFF;

    _ScreenGC.bInit = FALSE;
}

///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_API_GetBufferDC
///  query a buffer for drawing temperary graphics
///
///  @return GRAPHIC_DC buffer data structure
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////
GRAPHIC_DC* MApp_ZUI_API_GetBufferDC(void)
{
    //prepare a buffer, return buffer GC

    //already called in _MApp_ZUI_API_DrawComponent(): MApi_GOP_GWIN_Switch2FB(_ScreenGC.dcBuff.u8FbID);

    /*if (_ScreenGC.bUsed)
    {
        printf("screen_used!\n");
        //PRINT("GetClientDC(): _ScreenGC.bUsed\n"); //only can be used for one window...
        ABORT();
    }
    else*/
        _ScreenGC.bUsed = TRUE;

    return &_ScreenGC.dcBuff;
}

///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_API_GetScreenDC
///  query screen graphics
///
///  @return GRAPHIC_DC buffer data structure
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////
GRAPHIC_DC* MApp_ZUI_API_GetScreenDC(void)
{
    //prepare a buffer, draw bottom and return screen GC
    //static GRAPHIC_DC hdc;
    //hdc.pGC = &_ScreenGC.dcMain;//&_ScreenGC.BufferGC;

    /*if (_ScreenGC.bUsed)
    {
        printf("screen_used!\n");
        //PRINT("GetClientDC(): _ScreenGC.bUsed\n"); //only can be used for one window...
        ABORT();
    }
    else*/
        _ScreenGC.bUsed = TRUE;

    return &_ScreenGC.dcMain;
}


///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_API_ReleaseDC
///  release GRAPHIC_DC structure and update invalidated region to main (screen) buffer
///
///  @param [in]  rect_invalidate RECT *    invalidated region
///
///  This function doesn't return a value
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////
void MApp_ZUI_API_PreReleaseDC(void)
{
#if (ZUI_ONSCREEN_PINPON_BUFFER)
    U8 u8Temp;
    u8Temp = _ScreenGC.dcMain.u8FbID;
    _ScreenGC.dcMain.u8FbID = _ScreenGC.dcMain2.u8FbID;
    _ScreenGC.dcMain2.u8FbID = u8Temp;
#endif
}

void MApp_ZUI_API_ReleaseDC(RECT * rect_invalidate)
{

    MSAPI_OSDRegion CopyFB;
    MSAPI_OSDRegion PasteFB;
    GOP_GwinFBAttr fbAttr;
    GOP_GwinFBAttr fbAttrSrc;

    ZUI_DBG(printf("releaseDC\n"));
    MApi_GOP_GWIN_Switch2Gwin(_ScreenGC.u8GWinID); //testing

    //printf("release_dc(%lu,%lu,%lu,%lu)\n",
    //    (U32)rect_invalidate->left, (U32)rect_invalidate->top,
    //    (U32)rect_invalidate->width, (U32)rect_invalidate->height);

    //printf("main=%lu, buff=%lu\n", (U32)_ScreenGC.dcMain.u8FbID,
    //    (U32)_ScreenGC.dcBuff.u8FbID);

    MApi_GOP_GWIN_GetFBInfo(_ScreenGC.dcMain.u8FbID, &fbAttr);
    MApi_GOP_GWIN_GetFBInfo(_ScreenGC.dcBuff.u8FbID, &fbAttrSrc);

    if ((rect_invalidate->left+rect_invalidate->width) > fbAttrSrc.width)
    {
        rect_invalidate->width = fbAttrSrc.width - rect_invalidate->left;
    }
    if ((rect_invalidate->top+rect_invalidate->height) > fbAttrSrc.height)
    {
        rect_invalidate->height = fbAttrSrc.height - rect_invalidate->top;
    }

    if (OSDPAGE_ROTATE_ANGLE == OSD_ROTATE_0)
    {
        PasteFB.x=rect_invalidate->left;
        PasteFB.y=rect_invalidate->top;
        PasteFB.width=rect_invalidate->width;
        PasteFB.height=rect_invalidate->height;

        //we MUST reset clip window before msAPI_OSD_CopyRegion()
        msAPI_OSD_SetClipWindow(
            PasteFB.x, PasteFB.y,
            PasteFB.x+PasteFB.width-1,
            PasteFB.y+PasteFB.height-1);
    }
    else if (OSDPAGE_ROTATE_ANGLE == OSD_ROTATE_90)
    {
        PasteFB.x=fbAttr.width-rect_invalidate->top-rect_invalidate->height;
        PasteFB.y=rect_invalidate->left;
        PasteFB.width=rect_invalidate->width;
        PasteFB.height=rect_invalidate->height;

        //we MUST reset clip window before msAPI_OSD_CopyRegion()
        msAPI_OSD_SetClipWindow(
            PasteFB.x, PasteFB.y,
            PasteFB.x+PasteFB.height-1,
            PasteFB.y+PasteFB.width-1);
    }
    else if (OSDPAGE_ROTATE_ANGLE == OSD_ROTATE_180)
    {
        if ((rect_invalidate->top+rect_invalidate->height) > fbAttr.height)
        {
            rect_invalidate->height = fbAttr.height - rect_invalidate->top;
        }
        PasteFB.x=fbAttr.width-rect_invalidate->left-rect_invalidate->width;
        PasteFB.y=fbAttr.height-rect_invalidate->top-rect_invalidate->height;
        PasteFB.width=rect_invalidate->width;
        PasteFB.height=rect_invalidate->height;

        //we MUST reset clip window before msAPI_OSD_CopyRegion()
        msAPI_OSD_SetClipWindow(
            PasteFB.x, PasteFB.y,
            PasteFB.x+PasteFB.width-1,
            PasteFB.y+PasteFB.height-1);
    }
    else if (OSDPAGE_ROTATE_ANGLE == OSD_ROTATE_270)
    {
        if ((rect_invalidate->left+rect_invalidate->width) > fbAttr.height)
        {
            rect_invalidate->width = fbAttr.height - rect_invalidate->left;
        }
        PasteFB.x=rect_invalidate->top;
        PasteFB.y=fbAttr.height-rect_invalidate->left-rect_invalidate->width;
        PasteFB.width=rect_invalidate->width;
        PasteFB.height=rect_invalidate->height;

        //we MUST reset clip window before msAPI_OSD_CopyRegion()
        msAPI_OSD_SetClipWindow(
            PasteFB.x, PasteFB.y,
            PasteFB.x+PasteFB.height-1,
            PasteFB.y+PasteFB.width-1);
    }

    //MDrv_GE_SetNearestMode(FALSE); //default setting
    //MDrv_GE_SetDither (FALSE);
    //MApi_GFX_SetAlpha(FALSE, COEF_CONST, ABL_FROM_CONST, 0xFF);

    PasteFB.fbID=_ScreenGC.dcMain.u8FbID;

    CopyFB.fbID=_ScreenGC.dcBuff.u8FbID;
    CopyFB.x=rect_invalidate->left;
    CopyFB.y=rect_invalidate->top;
    CopyFB.width=rect_invalidate->width;
    CopyFB.height=rect_invalidate->height;
    MApi_GFX_SetAlpha(TRUE, COEF_ONE, ABL_FROM_ASRC, 0xFF);
    if (OSDPAGE_ROTATE_ANGLE == OSD_ROTATE_90)
        MApi_GFX_SetRotate(GEROTATE_90);
    if (OSDPAGE_ROTATE_ANGLE == OSD_ROTATE_180)
        MApi_GFX_SetRotate(GEROTATE_180);
    if (OSDPAGE_ROTATE_ANGLE == OSD_ROTATE_270)
        MApi_GFX_SetRotate(GEROTATE_270);
    msAPI_OSD_CopyRegion(&CopyFB, &PasteFB);
    MApi_GFX_SetRotate(GEROTATE_0);

    _ScreenGC.bUsed = FALSE;
}

void MApp_ZUI_API_PostReleaseDC(void)
{
#if (ZUI_ONSCREEN_PINPON_BUFFER)
    MSAPI_OSDRegion CopyFB;
    MSAPI_OSDRegion PasteFB;
    GOP_GwinFBAttr fbAttr;

    MApi_GFX_FlushQueue();
#if (CHIP_ID==MST9U2)
    MApi_GOP_GWIN_MapFBTo2Win(_ScreenGC.dcMain.u8FbID, _ScreenGC.u8GWinID, _ScreenGC.u8GWin2ID);
#else
    MApi_GOP_GWIN_MapFB2Win(_ScreenGC.dcMain.u8FbID, _ScreenGC.u8GWinID);
#endif //#if (CHIP_ID==MST9U2)
    MApi_GOP_GWIN_Switch2Gwin(_ScreenGC.u8GWinID);

    MApi_GOP_GWIN_GetFBInfo(_ScreenGC.dcMain.u8FbID, &fbAttr);

    PasteFB.fbID=_ScreenGC.dcMain2.u8FbID;
    PasteFB.x=0;
    PasteFB.y=0;
    PasteFB.width=fbAttr.width;
    PasteFB.height=fbAttr.height;

    CopyFB.fbID=_ScreenGC.dcMain.u8FbID;
    CopyFB.x=0;
    CopyFB.y=0;
    CopyFB.width=fbAttr.width;
    CopyFB.height=fbAttr.height;

    msAPI_OSD_SetClipWindow(
        PasteFB.x, PasteFB.y,
        PasteFB.x+PasteFB.width-1,
        PasteFB.y+PasteFB.height-1);

    MApi_GFX_SetAlpha(TRUE, COEF_ONE, ABL_FROM_ASRC, 0xFF);
    msAPI_OSD_CopyRegion(&CopyFB, &PasteFB);
#endif
}

///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_API_ReleaseFullScreenDC
///  release GRAPHIC_DC structure and switch current framebuffer between off framebuffer
///
///  This function doesn't return a value
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////
void MApp_ZUI_API_ReleaseFullScreenDC(void)
{
    //note: try to swap Buff and Screen if full screen case....
    U8 u8Temp;
#if (ZUI_ONSCREEN_PINPON_BUFFER)
    MSAPI_OSDRegion CopyFB;
    MSAPI_OSDRegion PasteFB;
    GOP_GwinFBAttr fbAttr;
#endif

    ZUI_DBG(printf("releaseFullDC\n"));

    if (!_ScreenGC.bAllowFullScreenRelease)
    {
        ZUI_DBG_FAIL(printf("[ZUI]FRDC\n"));
        ABORT();
    }

    /*
    MApi_GOP_GWIN_GetWinPosition(
        _ScreenGC.u8GWinID,    &x, &y);
    if (_ScreenGC.rcGWin.left+PANEL_HSTART != x ||
        _ScreenGC.rcGWin.top+1 != y)
    {
        MApi_GOP_GWIN_Enable(_ScreenGC.u8GWinID, FALSE);
    }*/

    u8Temp = _ScreenGC.dcMain.u8FbID;
#if (ZUI_ONSCREEN_PINPON_BUFFER)
    _ScreenGC.dcMain.u8FbID = _ScreenGC.dcMain2.u8FbID;
    _ScreenGC.dcMain2.u8FbID = u8Temp;
#else
    _ScreenGC.dcMain.u8FbID = _ScreenGC.dcBuff.u8FbID;
    _ScreenGC.dcBuff.u8FbID = u8Temp;
#endif

    //u32Temp = _ScreenGC.dcMain.u32MemAddr;
    //_ScreenGC.dcMain.u32MemAddr = _ScreenGC.dcBuff.u32MemAddr;
    //_ScreenGC.dcBuff.u32MemAddr = u32Temp;

    //printf("main=%lu, buff=%lu\n", (U32)_ScreenGC.dcMain.u8FbID,
    //    (U32)_ScreenGC.dcBuff.u8FbID);

    //PRINT("\n[ReleaseFullDC] swap main FB %d, buffer FB %d", (U16)_ScreenGC.dcMain.u8FbID, (U16)_ScreenGC.BufferGC.u8FbID);

#if (ZUI_ONSCREEN_PINPON_BUFFER)
    MApi_GOP_GWIN_GetFBInfo(_ScreenGC.dcBuff.u8FbID, &fbAttr);

    if (OSDPAGE_ROTATE_ANGLE == OSD_ROTATE_0)
    {
        PasteFB.x=0;
        PasteFB.y=0;
        PasteFB.width=fbAttr.width;
        PasteFB.height=fbAttr.height;

        //we MUST reset clip window before msAPI_OSD_CopyRegion()
        msAPI_OSD_SetClipWindow(
            PasteFB.x, PasteFB.y,
            PasteFB.x+PasteFB.width-1,
            PasteFB.y+PasteFB.height-1);
    }
    else if (OSDPAGE_ROTATE_ANGLE == OSD_ROTATE_90)
    {
        PasteFB.x=0;
        PasteFB.y=0;
        PasteFB.width=fbAttr.width;
        PasteFB.height=fbAttr.height;

        //we MUST reset clip window before msAPI_OSD_CopyRegion()
        msAPI_OSD_SetClipWindow(
            PasteFB.x, PasteFB.y,
            PasteFB.x+PasteFB.height-1,
            PasteFB.y+PasteFB.width-1);
    }
    else if (OSDPAGE_ROTATE_ANGLE == OSD_ROTATE_180)
    {
        PasteFB.x=0;
        PasteFB.y=0;
        PasteFB.width=fbAttr.width;
        PasteFB.height=fbAttr.height;

        //we MUST reset clip window before msAPI_OSD_CopyRegion()
        msAPI_OSD_SetClipWindow(
            PasteFB.x, PasteFB.y,
            PasteFB.x+PasteFB.width-1,
            PasteFB.y+PasteFB.height-1);
    }
    else if (OSDPAGE_ROTATE_ANGLE == OSD_ROTATE_270)
    {
        PasteFB.x=0;
        PasteFB.y=0;
        PasteFB.width=fbAttr.width;
        PasteFB.height=fbAttr.height;

        //we MUST reset clip window before msAPI_OSD_CopyRegion()
        msAPI_OSD_SetClipWindow(
            PasteFB.x, PasteFB.y,
            PasteFB.x+PasteFB.height-1,
            PasteFB.y+PasteFB.width-1);
    }

    PasteFB.fbID=_ScreenGC.dcMain.u8FbID;

    CopyFB.fbID=_ScreenGC.dcBuff.u8FbID;
    CopyFB.x=0;
    CopyFB.y=0;
    CopyFB.width=fbAttr.width;
    CopyFB.height=fbAttr.height;
    MApi_GFX_SetAlpha(TRUE, COEF_ONE, ABL_FROM_ASRC, 0xFF);
    if (OSDPAGE_ROTATE_ANGLE == OSD_ROTATE_90)
        MApi_GFX_SetRotate(GEROTATE_90);
    if (OSDPAGE_ROTATE_ANGLE == OSD_ROTATE_180)
        MApi_GFX_SetRotate(GEROTATE_180);
    if (OSDPAGE_ROTATE_ANGLE == OSD_ROTATE_270)
        MApi_GFX_SetRotate(GEROTATE_270);
    msAPI_OSD_CopyRegion(&CopyFB, &PasteFB);
    MApi_GFX_FlushQueue();
    MApi_GFX_SetRotate(GEROTATE_0);
#endif

    // note: call the following 4 function to switch to another framebuffer....
    MApi_GOP_GWIN_MapFB2Win(_ScreenGC.dcMain.u8FbID, _ScreenGC.u8GWinID);
    MApi_GOP_GWIN_Switch2Gwin(_ScreenGC.u8GWinID);
    MApi_GOP_GWIN_SetWinPosition(_ScreenGC.u8GWinID, _ScreenGC.rcGWin.left, _ScreenGC.rcGWin.top);

    /*/2007/12/10: testing for "full screen release" with move GWin...
    //            if the gwin position is changed, and then full screen release will be flicker...
    if (_ScreenGC.rcGWin.left+PANEL_HSTART != x ||
        _ScreenGC.rcGWin.top+1 != y)
    {
        MApi_GOP_GWIN_Enable(_ScreenGC.u8GWinID, TRUE);
    }*/

    msAPI_OSD_SetClipWindow(0, 0, _ScreenGC.rcGWin.width, _ScreenGC.rcGWin.height);

    //PRINT("\n[ReleaseFullDC] swap okay");

    _ScreenGC.bUsed = FALSE;

}


///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_API_ReleaseWithoutUpdateDC
///  release GRAPHIC_DC structure but don't switch or update
///     this function is suitable for preparation before transition effects
///
///  This function doesn't return a value
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////
void MApp_ZUI_API_ReleaseWithoutUpdateDC(void)
{
    //printf("releaseWOupdateDC\n");

    //note: for transition effect, we keep background DC without updating to screen..

    _ScreenGC.bUsed = FALSE;
}

///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_API_QueryGWinID
///  query GWin ID that ZUI is using
///
///  @return U8     GWin ID, return GWIN_NO_AVAILABLE if not init
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////
U8 MApp_ZUI_API_QueryGWinID(void)
{
    if (_ScreenGC.bInit)
        return _ScreenGC.u8GWinID;
    return GWIN_NO_AVAILABLE;
}

#if (CHIP_ID==MST9U2)
U8 MApp_ZUI_API_QueryGWin2ID(void)
{
    if (_ScreenGC.bInit)
        return _ScreenGC.u8GWin2ID;
    return GWIN_NO_AVAILABLE;
}
#endif

///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_API_QueryGWinRect
///  query GWin rect that ZUI is using
///
///  @return     no return value
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////
void MApp_ZUI_API_QueryGWinRect(RECT* rect)
{
    if (_ScreenGC.bInit)
    {
        *rect = _ScreenGC.rcGWin;
    }
}

///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_API_SetGWinPosition
///  set ZUI gwin position....
///
///  @param [in]  x U16    X position
///  @param [in]  y U16    Y position
///
///  @return     no return value
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////
void MApp_ZUI_API_SetGWinPosition(U16 x, U16 y)
{
    _ScreenGC.bAllowFullScreenRelease = FALSE; //2007/12/11: disable "full screen release" if gwin moved...

    _ScreenGC.rcGWin.left = x;
    _ScreenGC.rcGWin.top = y;
    MApi_GOP_GWIN_SetWinPosition(_ScreenGC.u8GWinID,
        _ScreenGC.rcGWin.left, _ScreenGC.rcGWin.top);

}

///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_API_GetGWinPosition
///  get ZUI gwin position....
///
///  @param [out]  x U16*    X position
///  @param [out]  y U16*    Y position
///
///  @return     no return value
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////

void MApp_ZUI_API_GetGWinPosition(U16* x, U16* y)
{
    *x = _ScreenGC.rcGWin.left;
    *y = _ScreenGC.rcGWin.top;
}

///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_API_EnableFullScreenRelease
///  enable/disable full screen release DC.
///     (if you want to move gwin, please disable it for flick issue)
///
///  @param [in]  bEnable BOOLEAN     true for enable full screen release
///
///  @return     no return value
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////
void MApp_ZUI_API_EnableFullScreenRelease(BOOLEAN bEnable)
{
    _ScreenGC.bAllowFullScreenRelease = bEnable;
}

///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_API_IsAllowFullScreenRelease
///  determine if full screen release DC.
///     (if you want to move gwin, please disable it for flick issue)
///
///  @return     true for allow, false for not
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////
BOOLEAN MApp_ZUI_API_IsAllowFullScreenRelease(void)
{
    return _ScreenGC.bAllowFullScreenRelease;
}


///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_API_DoesIntersect
///  detect intersection between two rectangles
///
///  @param [in]       psrc1 const RECT *    rectangle1
///  @param [in]       psrc2 const RECT *    rectangle2
///
///  @return BOOL    TRUE for intersection, FALSE for non-intersection
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////
BOOLEAN MApp_ZUI_API_DoesIntersect (const RECT* psrc1, const RECT* psrc2)
{
    return !( psrc1->left > (psrc2->left+psrc2->width)
        || (psrc1->left+psrc1->width) < psrc2->left
        || psrc1->top > (psrc2->top+psrc2->height)
        || (psrc1->top+psrc1->height) < psrc2->top
        );
}


///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_API_UnionRect
///  union a rectangle region into another rectangle
///
///  @param [in, out]  pdrc RECT *    rectangle wants to be union
///  @param [in]       psrc1 const RECT *    rectangle
///
///  @return void
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////
void MApp_ZUI_API_UnionRect(RECT* pdrc, const RECT* psrc1)
{
#if 1
    // For speed-up union time, do not change the sequence!!!
    pdrc->width = MAX(pdrc->left+pdrc->width, psrc1->left+psrc1->width);
    pdrc->left = MIN(pdrc->left, psrc1->left);
    pdrc->height = MAX(pdrc->top+pdrc->height, psrc1->top+psrc1->height);
    pdrc->top = MIN(pdrc->top, psrc1->top);

    pdrc->width -= pdrc->left;
    pdrc->height -= pdrc->top;

    return;
#else
    S16 left;
    S16 top;

    left = pdrc->left;
    if (left > psrc1->left)
        left = psrc1->left;
    top = pdrc->top;
    if (top > psrc1->top)
        top = psrc1->top;
    pdrc->width = (((pdrc->left+pdrc->width) > (psrc1->left+psrc1->width))? (pdrc->left+pdrc->width):(psrc1->left+psrc1->width)) - left;//(MAX(pdrc->left+pdrc->width, psrc1->left+psrc1->width) - left;
    pdrc->height = (((pdrc->top+pdrc->height) > (psrc1->top+psrc1->height))? (pdrc->top+pdrc->height):(psrc1->top+psrc1->height)) - top; //( MAX(pdrc->top+pdrc->height, psrc1->top+psrc1->height) - top;
    pdrc->left = left;
    pdrc->top = top;
#endif
}

///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_API_IntersectRect
///  intersect a rectangle region with another rectangle
///
///  @param [in, out]  pdrc RECT *    rectangle wants to be intersect
///  @param [in]       psrc1 const RECT *    rectangle
///
///  @return void
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////
void MApp_ZUI_API_IntersectRect(RECT* pdrc, const RECT* psrc1)
{
    if ( MApp_ZUI_API_DoesIntersect(pdrc, psrc1) )
    {
        RECT rectInterSect;
        rectInterSect.left = MAX( pdrc->left , psrc1->left );
        rectInterSect.width = MIN( (pdrc->left+pdrc->width) , (psrc1->left+psrc1->width) ) - rectInterSect.left;
        rectInterSect.top = MAX( pdrc->top , psrc1->top );
        rectInterSect.height= MIN( (pdrc->top+pdrc->height) , (psrc1->top+psrc1->height) ) - rectInterSect.top;
        *pdrc = rectInterSect;
    }
    else
    {
        RECT_SET((*pdrc), 0, 0, 0, 0);
    }
}

///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_API_DoesContain
///  check "Inside" is inside in "Big" rectangle
///
///  @param [in]  pBig RECT *    rectangle "Big"
///  @param [in]       pInside const RECT *    rectangle "Inside"
///
///  @return BOOLEAN      TRUE for yes, FALSE for no
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////

BOOLEAN MApp_ZUI_API_DoesContain (const RECT* pBig, const RECT* pInside)
{
    return (pInside->left >= pBig->left &&
        pInside->left+pInside->width <= pBig->left+pBig->width &&
        pInside->top >= pBig->top &&
        pInside->top+pInside->height <= pBig->top+pBig->height);
}

GRAPHIC_DC MApp_ZUI_API_CreateDC(U16 w, U16 h)
{
    //U8 u16ZuiFmt = MPLAYER_BITMAP_FORMAT;
    U8 gefmt;
    U8 fbfmt;
    U16 u16alignWidth;
    U8 u8Ret;

    gefmt = MApp_ZUI_API_ZuiFormatToGeFormat(MPLAYER_BITMAP_FORMAT);
    fbfmt = MApp_ZUI_API_ZuiFormatToGopFormat(MPLAYER_BITMAP_FORMAT);
    u16alignWidth = GE_ALIGNED_VALUE(w, 8);

    GRAPHIC_DC dc;
    dc.u8FbID = MApi_GOP_GWIN_GetFreeFBID();
    dc.u8ConstantAlpha = 0xFF;
    dc.bSrcAlphaReplaceDstAlpha = TRUE;
    u8Ret = MApi_GOP_GWIN_CreateFB(dc.u8FbID, 0, 0, u16alignWidth, h, gefmt);
    if ( u8Ret != GWIN_OK )
    {
        ZUI_DBG_FAIL(printf("[ZUI]CreateDC\n"));
        dc.u8FbID = 0xFF;
        return dc;
    }

    MApi_GOP_GWIN_Switch2FB(dc.u8FbID);
    msAPI_OSD_SetClipWindow(0, 0, u16alignWidth-1, h-1);

    // don't show anything on the init, so we fill it as transparent...
    clrBtn1.x = 0;
    clrBtn1.y = 0;
    clrBtn1.width = u16alignWidth;
    clrBtn1.height = h;
    #if (MPLAYER_BITMAP_FORMAT == MPLAYER_I8)
    clrBtn1.b_clr = 0;
    #else
    clrBtn1.b_clr = ZUI_COLOR_TRANSPARENT_RGB8888;
    #endif
    clrBtn1.u8Gradient = CONSTANT_COLOR;
    clrBtn1.fHighLight = FALSE;
    msAPI_OSD_DrawBlock(&clrBtn1);

    return dc;
}

void MApp_ZUI_API_ClearDC(GRAPHIC_DC *pDC, U32 u32Color)
{
    GOP_GwinFBAttr fbAttr;
    MApi_GOP_GWIN_GetFBInfo(pDC->u8FbID, &fbAttr);

    MApi_GOP_GWIN_Switch2FB(pDC->u8FbID);
    msAPI_OSD_SetClipWindow(0, 0, fbAttr.width-1, fbAttr.height-1);

    // don't show anything on the init, so we fill it as transparent...
    clrBtn1.x = 0;
    clrBtn1.y = 0;
    clrBtn1.width = fbAttr.width;
    clrBtn1.height = fbAttr.height;
    clrBtn1.b_clr = u32Color;
    clrBtn1.u8Gradient = CONSTANT_COLOR;
    clrBtn1.fHighLight = FALSE;
    msAPI_OSD_DrawBlock(&clrBtn1);
}

void MApp_ZUI_API_ClearDCRect(GRAPHIC_DC *pDC, U32 u32Color, RECT* pRect)
{
    GOP_GwinFBAttr fbAttr;
    MApi_GOP_GWIN_GetFBInfo(pDC->u8FbID, &fbAttr);

    MApi_GOP_GWIN_Switch2FB(pDC->u8FbID);
    msAPI_OSD_SetClipWindow(0, 0, fbAttr.width-1, fbAttr.height-1);

    // don't show anything on the init, so we fill it as transparent...
    clrBtn1.x = pRect->left;
    clrBtn1.y = pRect->top;
    clrBtn1.width = pRect->width;
    clrBtn1.height = pRect->height;
    clrBtn1.b_clr = u32Color;
    clrBtn1.u8Gradient = CONSTANT_COLOR;
    clrBtn1.fHighLight = FALSE;
    msAPI_OSD_DrawBlock(&clrBtn1);
}

void MApp_ZUI_API_DeleteDC(GRAPHIC_DC dc)
{
    MApi_GOP_GWIN_DeleteFB(dc.u8FbID);
}

U8 MApp_ZUI_API_ZuiFormatToGeFormat(U8 u8ZuiFormat)
{
    switch (u8ZuiFormat)
    {
        case MPLAYER_I8:
            return GFX_FMT_I8;
        case MPLAYER_RGB565:
            return GFX_FMT_RGB565;
        case MPLAYER_ARGB4444:
            return GFX_FMT_ARGB4444;
        case MPLAYER_ARGB8888:
            return GFX_FMT_ARGB8888;
        case MPLAYER_ARGB1555:
            return GFX_FMT_ARGB1555_DST;
        default:
        case MPLAYER_RGB555:
            return GFX_FMT_ARGB1555;
    }

    ZUI_DBG_FAIL(printf("[ZUI]GeFmt\n"));
    ABORT();

    return GFX_FMT_ARGB1555;
}

U8 MApp_ZUI_API_ZuiFormatToGopFormat(U8 u8ZuiFormat)
{
    switch (u8ZuiFormat)
    {
        case MPLAYER_I8:
            return E_GOP_COLOR_I8;
        case MPLAYER_RGB565:
            return E_GOP_COLOR_RGB565;
        case MPLAYER_ARGB4444:
            return E_GOP_COLOR_ARGB4444;
        case MPLAYER_ARGB8888:
            return E_GOP_COLOR_ARGB8888;
        case MPLAYER_ARGB1555:
            return E_GOP_COLOR_ARGB1555;
        default:
        case MPLAYER_RGB555:
            return E_GOP_COLOR_RGB555_BLINK;
    }

    ZUI_DBG_FAIL(printf("[ZUI]GopFmt\n"));
    ABORT();

    return E_GOP_COLOR_RGB555_BLINK;
}

U8 MApp_ZUI_API_GetBPP(U8 u8ZuiFormat)
{
    switch (u8ZuiFormat)
    {
        case MPLAYER_I8:
            return 1;
        case MPLAYER_ARGB8888:
            return 4;
        default:
            return 2;
    }

    ZUI_DBG_FAIL(printf("[ZUI]GetBPP\n"));
    ABORT();

    return 2;
}

void MApp_ZUI_API_PrepareBufferDC(GRAPHIC_DC *pDC)
{
    //note: it is ALMOST default painting flow, but not update to screen!
    HWND hwnd;
    RECT * rect_inv;
    //BOOLEAN bFullScreen; //always full screen!
    PAINT_PARAM param;

    //redraw all controls
    rect_inv = &GETWNDINITRECT(HWND_MAINFRAME);
    //bFullScreen = TRUE;

    param.dc = *pDC;
    param.dc.u8ConstantAlpha = 0xFF; //disable alpha blending

    //draw main frame...
    param.rect = rect_inv;
    param.rect_invalidate = rect_inv;
    param.bIsDisable = param.bIsFocus = FALSE;
    param.bIsVisible = TRUE;
    _MApp_ZUI_API_DefaultOnPaint(HWND_MAINFRAME, &param, TRUE);

    //draw others....
    for (hwnd = HWND_MAINFRAME+1; hwnd < HWND_MAX; hwnd++)
    {
        param.bIsVisible = MApp_ZUI_API_IsWindowVisible(hwnd);
        if (param.bIsVisible)
        {
            RECT rect;
            MApp_ZUI_API_GetWindowRect(hwnd, &rect);
            param.rect = &rect;
            {
                param.bIsDisable = !MApp_ZUI_API_IsWindowEnabled(hwnd);

                //note: we assume if one window get focus, the child will be painted as focus style..
                param.bIsFocus = (MApp_ZUI_API_GetFocus()==hwnd || MApp_ZUI_API_IsParentFocused(hwnd));
                param.dc.bSrcAlphaReplaceDstAlpha = ((GETWNDINITSTATE(hwnd)&WS_SRCALPHAREPLACEDSTALPHA) != 0);
                MApp_ZUI_API_SendMessage(hwnd, MSG_PAINT, (WPARAM)&param);
            }
        }
    }

    MApp_ZUI_API_ReleaseWithoutUpdateDC(); //note: don't update!!
}

OSDPAGE_RotateAngle MApp_ZUI_API_GetCurrentRotateAngle(void)
{
    return OSDPAGE_ROTATE_ANGLE;
}

void _DoRotate(OSDPAGE_RotateAngle angle, BOOLEAN bShowAfterRotate)
{
    GOP_GwinFBAttr fbAttr;
    RECT rect_gwin;
    U8 u8FbID;
    U8 bpp;
    U8 nRet = 0;

    if (OSDPAGE_ROTATE_ANGLE == angle)
    {
        // do nothing
        return;
    }

    OSDPAGE_ROTATE_ANGLE = angle;

    MApi_GOP_GWIN_SwitchGOP(OSD_GOP_ID);

    // check gwin exist or not
    if (FALSE == MApi_GOP_GWIN_IsGwinExist(_ScreenGC.u8GWinID))
    {
        return;
    }

    bpp = MApp_ZUI_API_GetBPP(MPLAYER_BITMAP_FORMAT);
    MApi_GOP_GWIN_GetFBInfo(_ScreenGC.dcBuff.u8FbID, &fbAttr);

    MApp_ZUI_API_QueryGWinRect(&rect_gwin);

    MApi_GOP_GWIN_Enable(_ScreenGC.u8GWinID, FALSE);
#if (CHIP_ID==MST9U2)
    MApi_GOP_GWIN_Enable(_ScreenGC.u8GWin2ID, FALSE);
#endif
    MApi_GOP_GWIN_DeleteFB(_ScreenGC.dcMain.u8FbID);
#if (ZUI_ONSCREEN_PINPON_BUFFER)
    MApi_GOP_GWIN_DeleteFB(_ScreenGC.dcMain2.u8FbID);
#endif
    MApi_GOP_GWIN_DeleteFB(_ScreenGC.dcBuff.u8FbID);

    u8FbID = MApi_GOP_GWIN_GetFreeFBID();

    if (u8FbID == 0xFF)
    {
        printf("[ZUI]GDIFB0\n");
        ABORT();
        return;
    }

    if ((g_bOSDPAGE_ROTATE_BUFFER == TRUE) && (OSDPAGE_ROTATE_ANGLE == OSD_ROTATE_90 || OSDPAGE_ROTATE_ANGLE == OSD_ROTATE_270))
    {
        nRet = MApi_GOP_GWIN_CreateFB(u8FbID, 0, 0, g_nBufferAlignHeight, g_nBufferAlignWidth, fbAttr.fbFmt);
    }
    else
    {
        nRet = MApi_GOP_GWIN_CreateFB(u8FbID, 0, 0, g_nBufferAlignWidth, g_nBufferAlignHeight, fbAttr.fbFmt);
    }

    if (nRet != GOP_API_SUCCESS)
    {
        printf("[ZUI]GDIFB1: nRet=%d\n", nRet);
        ABORT();
        return;
    }

    _ScreenGC.dcBuff.u8ConstantAlpha = 0xFF;
    _ScreenGC.dcBuff.u8FbID = u8FbID;

#if (CHIP_ID==MST9U2)
    MApi_GOP_GWIN_MapFBTo2Win(u8FbID, _ScreenGC.u8GWinID, _ScreenGC.u8GWin2ID);
    MApi_GOP_GWIN_Set2WinPosition(_ScreenGC.u8GWinID, _ScreenGC.u8GWin2ID, rect_gwin.left, rect_gwin.top);
#else
    MApi_GOP_GWIN_MapFB2Win(u8FbID, _ScreenGC.u8GWinID);
    MApi_GOP_GWIN_SetWinPosition(_ScreenGC.u8GWinID, rect_gwin.left, rect_gwin.top);
#endif //#if (CHIP_ID==MST9U2)
    MApi_GOP_GWIN_Switch2Gwin(_ScreenGC.u8GWinID);
    msAPI_OSD_SetClipWindow(0, 0, g_nBufferAlignWidth, g_nBufferAlignHeight);
    if ((g_bOSDPAGE_ROTATE_BUFFER == TRUE) && (OSDPAGE_ROTATE_ANGLE == OSD_ROTATE_90 || OSDPAGE_ROTATE_ANGLE == OSD_ROTATE_270))
    {
        msAPI_OSD_SetClipWindow(0, 0, g_nBufferAlignHeight, g_nBufferAlignWidth);
    }

    // don't show anything on the init, so we fill it as transparent...
    clrBtn1.x = 0;
    clrBtn1.y = 0;
    clrBtn1.width = g_nBufferAlignWidth;
    clrBtn1.height = g_nBufferAlignHeight;
    if ((g_bOSDPAGE_ROTATE_BUFFER == TRUE) && (OSDPAGE_ROTATE_ANGLE == OSD_ROTATE_90 || OSDPAGE_ROTATE_ANGLE == OSD_ROTATE_270))
    {
        clrBtn1.width = g_nBufferAlignHeight;
        clrBtn1.height = g_nBufferAlignWidth;
    }
#if (MPLAYER_BITMAP_FORMAT == MPLAYER_I8)
    clrBtn1.b_clr = 0;
#else
    clrBtn1.b_clr = ZUI_COLOR_TRANSPARENT_RGB8888;
#endif
    clrBtn1.u8Gradient = CONSTANT_COLOR;
    clrBtn1.fHighLight = FALSE;
    msAPI_OSD_DrawBlock(&clrBtn1);

    u8FbID = MApi_GOP_GWIN_GetFreeFBID();

    if (u8FbID == 0xFF)
    {
        printf("[ZUI]GDIFB1\n");
        ABORT();
        return;
    }

    if ((g_bOSDPAGE_ROTATE_BUFFER == FALSE) && (OSDPAGE_ROTATE_ANGLE == OSD_ROTATE_90 || OSDPAGE_ROTATE_ANGLE == OSD_ROTATE_270))
        nRet = MApi_GOP_GWIN_CreateFB(u8FbID, 0, 0, g_nBufferAlignHeight, g_nBufferWidth, fbAttr.fbFmt);
    else
        nRet = MApi_GOP_GWIN_CreateFB(u8FbID, 0, 0, g_nBufferAlignWidth, g_nBufferHeight, fbAttr.fbFmt);

    if (nRet != GOP_API_SUCCESS)
    {
        printf("[ZUI]GDIFB2: nRet=%d\n", nRet);
        ABORT();
        return;
    }

#if (ZUI_ONSCREEN_PINPON_BUFFER)
    _ScreenGC.dcMain2.u8ConstantAlpha = 0xFF;
    _ScreenGC.dcMain2.u8FbID = u8FbID;

    // note: call the following 4 function to switch to another framebuffer....
#if (CHIP_ID==MST9U2)
    MApi_GOP_GWIN_MapFBTo2Win(u8FbID, _ScreenGC.u8GWinID, _ScreenGC.u8GWin2ID);
    MApi_GOP_GWIN_Set2WinPosition(_ScreenGC.u8GWinID, _ScreenGC.u8GWin2ID, rect_gwin.left, rect_gwin.top);
#else
    MApi_GOP_GWIN_MapFB2Win(u8FbID, _ScreenGC.u8GWinID);
    MApi_GOP_GWIN_SetWinPosition(_ScreenGC.u8GWinID, rect_gwin.left, rect_gwin.top);
#endif //#if (CHIP_ID==MST9U2)
    MApi_GOP_GWIN_Switch2Gwin(_ScreenGC.u8GWinID);
    msAPI_OSD_SetClipWindow(0, 0, g_nBufferAlignWidth, g_nBufferHeight);
    if ((g_bOSDPAGE_ROTATE_BUFFER == FALSE) && (OSDPAGE_ROTATE_ANGLE == OSD_ROTATE_90 || OSDPAGE_ROTATE_ANGLE == OSD_ROTATE_270))
    {
        msAPI_OSD_SetClipWindow(0, 0, g_nBufferAlignHeight, g_nBufferWidth);
    }

    // don't show anything on the init, so we fill it as transparent...
    clrBtn1.x = 0;
    clrBtn1.y = 0;
    clrBtn1.width = g_nBufferAlignWidth;
    clrBtn1.height = g_nBufferHeight;
    if ((g_bOSDPAGE_ROTATE_BUFFER == FALSE) && (OSDPAGE_ROTATE_ANGLE == OSD_ROTATE_90 || OSDPAGE_ROTATE_ANGLE == OSD_ROTATE_270))
    {
        clrBtn1.width = g_nBufferAlignHeight;
        clrBtn1.height = g_nBufferWidth;
    }
#if (MPLAYER_BITMAP_FORMAT == MPLAYER_I8)
    clrBtn1.b_clr = 0;
#else
    clrBtn1.b_clr = ZUI_COLOR_TRANSPARENT_RGB8888;
#endif
    clrBtn1.u8Gradient = CONSTANT_COLOR;
    clrBtn1.fHighLight = FALSE;
    msAPI_OSD_DrawBlock(&clrBtn1);

    u8FbID = MApi_GOP_GWIN_GetFreeFBID();

    if (u8FbID == 0xFF)
    {
        printf("[ZUI]GDIFB3\n");
        ABORT();
        return;
    }

    if ((g_bOSDPAGE_ROTATE_BUFFER == FALSE) && (OSDPAGE_ROTATE_ANGLE == OSD_ROTATE_90 || OSDPAGE_ROTATE_ANGLE == OSD_ROTATE_270))
        nRet = MApi_GOP_GWIN_CreateFB(u8FbID, 0, 0, g_nBufferAlignHeight, g_nBufferWidth, fbAttr.fbFmt);
    else
        nRet = MApi_GOP_GWIN_CreateFB(u8FbID, 0, 0, g_nBufferAlignWidth, g_nBufferHeight, fbAttr.fbFmt);

    if (nRet != GOP_API_SUCCESS)
    {
        printf("[ZUI]GDIFB4: nRet=%d\n", nRet);
        ABORT();
        return;
    }
#endif

    _ScreenGC.dcMain.u8ConstantAlpha = 0xFF;
    _ScreenGC.dcMain.u8FbID = u8FbID;

    // note: call the following 4 function to switch to another framebuffer....
#if (CHIP_ID==MST9U2)
    MApi_GOP_GWIN_MapFBTo2Win(u8FbID, _ScreenGC.u8GWinID, _ScreenGC.u8GWin2ID);
    MApi_GOP_GWIN_Set2WinPosition(_ScreenGC.u8GWinID, _ScreenGC.u8GWin2ID, rect_gwin.left, rect_gwin.top);
#else
    MApi_GOP_GWIN_MapFB2Win(u8FbID, _ScreenGC.u8GWinID);
    MApi_GOP_GWIN_SetWinPosition(_ScreenGC.u8GWinID, rect_gwin.left, rect_gwin.top);
#endif //#if (CHIP_ID==MST9U2)
    MApi_GOP_GWIN_Switch2Gwin(_ScreenGC.u8GWinID);
    msAPI_OSD_SetClipWindow(0, 0, g_nBufferAlignWidth, g_nBufferHeight);
    if ((g_bOSDPAGE_ROTATE_BUFFER == FALSE) && (OSDPAGE_ROTATE_ANGLE == OSD_ROTATE_90 || OSDPAGE_ROTATE_ANGLE == OSD_ROTATE_270))
    {
        msAPI_OSD_SetClipWindow(0, 0, g_nBufferAlignHeight, g_nBufferWidth);
    }

    // don't show anything on the init, so we fill it as transparent...
    clrBtn1.x = 0;
    clrBtn1.y = 0;
    clrBtn1.width = g_nBufferAlignWidth;
    clrBtn1.height = g_nBufferHeight;
    if ((g_bOSDPAGE_ROTATE_BUFFER == FALSE) && (OSDPAGE_ROTATE_ANGLE == OSD_ROTATE_90 || OSDPAGE_ROTATE_ANGLE == OSD_ROTATE_270))
    {
        clrBtn1.width = g_nBufferAlignHeight;
        clrBtn1.height = g_nBufferWidth;
    }
    #if (MPLAYER_BITMAP_FORMAT == MPLAYER_I8)
    clrBtn1.b_clr = 0;
    #else
    clrBtn1.b_clr = ZUI_COLOR_TRANSPARENT_RGB8888;
    #endif
    clrBtn1.u8Gradient = CONSTANT_COLOR;
    clrBtn1.fHighLight = FALSE;
    msAPI_OSD_DrawBlock(&clrBtn1);

    MApp_ZUI_API_InvalidateAllSuccessors(HWND_MAINFRAME);
    _MApp_ZUI_API_WindowProcOnIdle();

    MApi_GFX_FlushQueue();

    if (bShowAfterRotate)
    {
#if (CHIP_ID==MST9U2)
        MApi_GOP_GWIN_UpdateRegAllBankOnceEx(TRUE);
#endif
        MApi_GOP_GWIN_Enable(_ScreenGC.u8GWinID, TRUE);
#if (CHIP_ID==MST9U2)
        MApi_GOP_GWIN_Enable(_ScreenGC.u8GWin2ID, TRUE);
        MApi_GOP_GWIN_UpdateRegAllBankOnceEx(FALSE);
#endif
    }

    RECT_SET(_ScreenGC.rcGWin, rect_gwin.left, rect_gwin.top, g_nBufferWidth, g_nBufferHeight);

}

void MApp_ZUI_API_DoRotate(OSDPAGE_RotateAngle angle, BOOLEAN bShowAfterRotate)
{
    g_bOSDPAGE_ROTATE_BUFFER = FALSE;
    _DoRotate(angle, bShowAfterRotate);
}

void MApp_ZUI_API_DoRotateBufferWithoutRotateGWin(OSDPAGE_RotateAngle angle, BOOLEAN bShowAfterRotate)
{
    g_bOSDPAGE_ROTATE_BUFFER = TRUE;
    _DoRotate(angle, bShowAfterRotate);
}

#undef MAPP_ZUI_APIGDI_C

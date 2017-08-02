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

#define MAPP_LOADFONTINIT_C


#include <stdlib.h>
#include <stdio.h>
#include "Board.h"
#include "MsCommon.h"
//#include "msAPI_MIU.h"

// Common Definition
//#include "apiXC.h"
#include "MApp_LoadFontInit.h"
#include "MApp_Font.h"
#include "MApp_ZUI_ACTglobal.h"
#include "BinInfo.h"
#include "InfoBlock.h"

///////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////

//**********************************************************
#if (VECTOR_FONT_ENABLE && !(OBA2))
extern BOOLEAN msAPI_MVF_init_mempool (void *pool, U32 size);
#endif

//**********************************************************
LOADFONT_STRUCT font_info[] =
{
    {
        .FontID = 0,
#if (MPLAYER_BITMAP_FORMAT == MPLAYER_I8)
        .output_bpp = 1,
#else
        .output_bpp = 2,
#endif
        .output_size = 35,
        .output_width = 30,
        .output_height = 40,
        .unicode_from = { 0x0020, 0},
        .unicode_to = { 0xFFFF, 0},
    },
    {
        .FontID = 1,
#if (MPLAYER_BITMAP_FORMAT == MPLAYER_I8)
        .output_bpp = 1,
#else
        .output_bpp = 2,
#endif
        .output_size = 28,
        .output_width = 24,
        .output_height = 32,
        .unicode_from = { 0x0020, 0},
        .unicode_to = { 0xFFFF, 0},
    },
    {
        .FontID = 2,
#if (MPLAYER_BITMAP_FORMAT == MPLAYER_I8)
        .output_bpp = 1,
#else
        .output_bpp = 2,
#endif
        .output_size = 18,
        .output_width = 19,
        .output_height = 23,//25,
        .unicode_from = { 0x0020, 0},
        .unicode_to = { 0xFFFF, 0},
    },
};

#if VECTOR_FONT_ENABLE
U16 fontpair[] =
{
    BIN_ID_FONT_MSTAR_MVF_0,
    BIN_ID_FONT_MSTAR_MVF_1,
};
#else
LOADFONT_PAIR fontpair[][3] =
{
    {
        {FONT_BMP_0, BIN_ID_FONT_BMPFONT_0_0},
        {FONT_BMP_1, BIN_ID_FONT_BMPFONT_0_1},
        {FONT_BMP_2, BIN_ID_FONT_BMPFONT_0_2},
    },
    {
        {FONT_BMP_0, BIN_ID_FONT_BMPFONT_1_0},
        {FONT_BMP_1, BIN_ID_FONT_BMPFONT_1_1},
        {FONT_BMP_2, BIN_ID_FONT_BMPFONT_1_2},
    },
};
#endif

#if VECTOR_FONT_ENABLE
void MApp_LoadFontInit_VEC(U8 nSet)
{
    U8 u8num = sizeof(font_info)/sizeof(LOADFONT_STRUCT) - 1;
    MApp_LoadFont_SetVecFontInfo(fontpair[nSet], font_info, 0, u8num);

#if (ENABLE_ARABIC_OSD)
    Font[FONT_ARABIC_0].fHandle = msAPI_Font_LoadFlashFontToSDRAM(BIN_ID_FONT_ARABIC_35);
    FontHandleIndex[Font[FONT_ARABIC_0].fHandle] = FONT_ARABIC_0;
    Font[FONT_ARABIC_1].fHandle = msAPI_Font_LoadFlashFontToSDRAM(BIN_ID_FONT_ARABIC_30);
    FontHandleIndex[Font[FONT_ARABIC_1].fHandle] = FONT_ARABIC_1;
    Font[FONT_ARABIC_2].fHandle = msAPI_Font_LoadFlashFontToSDRAM(BIN_ID_FONT_ARABIC_26);
    FontHandleIndex[Font[FONT_ARABIC_2].fHandle] = FONT_ARABIC_2;
    Font[FONT_ARABIC_50].fHandle = msAPI_Font_LoadFlashFontToSDRAM(BIN_ID_FONT_ARABIC_50);
    FontHandleIndex[Font[FONT_ARABIC_50].fHandle] = FONT_ARABIC_50;
#endif

#if (ENABLE_THAI_OSD)
    Font[FONT_ARABIC_0].fHandle = msAPI_Font_LoadFlashFontToSDRAM(BIN_ID_FONT_THAI_35);
    FontHandleIndex[Font[FONT_THAI_0].fHandle] = FONT_THAI_0;
    Font[FONT_ARABIC_1].fHandle = msAPI_Font_LoadFlashFontToSDRAM(BIN_ID_FONT_THAI_30);
    FontHandleIndex[Font[FONT_THAI_1].fHandle] = FONT_THAI_1;
    Font[FONT_ARABIC_2].fHandle = msAPI_Font_LoadFlashFontToSDRAM(BIN_ID_FONT_THAI_26);
    FontHandleIndex[Font[FONT_THAI_2].fHandle] = FONT_THAI_2;
    Font[FONT_ARABIC_50].fHandle = msAPI_Font_LoadFlashFontToSDRAM(BIN_ID_FONT_THAI_50);
    FontHandleIndex[Font[FONT_THAI_50].fHandle] = FONT_THAI_50;
#endif
}
#else
static void MApp_LoadFontInit_BMP(U8 nSet)
{
    U8 u8num = sizeof(fontpair[0])/sizeof(LOADFONT_PAIR);
    MApp_LoadFont_SetBMPFontInfo(fontpair[nSet],u8num);
}
#endif

void MApp_LoadFontInit(U8 nSet)
{
    msAPI_Font_VariableInit();
  #if VECTOR_FONT_ENABLE
    #if (!OBA2)
        #if (!COPRO_MVF_ENABLE)
        msAPI_MVF_init_mempool((void*)_PA2VA(POOL_BUFFER_ADR), POOL_BUFFER_LEN);
        #endif
    #endif
    MApp_LoadFontInit_VEC(nSet);
  #else
    MApp_LoadFontInit_BMP(nSet);
  #endif
}


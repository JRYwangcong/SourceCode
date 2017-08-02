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


#define MAPP_LOADFONT_C

#include <stdlib.h>
#include <stdio.h>
#include "Board.h"
#include "datatype.h"
#include "MsCommon.h"
#include "apiXC.h"
#include "apiXC_Adc.h"
//#include "msAPI_MIU.h"
#include "msAPI_Font.h"
//#include "msAPI_Timer.h"
#include "MApp_Font.h"
#include "MApp_GlobalSettingSt.h"
//#include "MApp_GlobalVar.h"
#include "BinInfo.h"
#include "Utl.h"

/*****************************************************************************/
/*                                 Global Variable                           */
/*****************************************************************************/


//==============================================================================
/*                               Functions                                      */
//==============================================================================

void MApp_LoadFont_SetBMPFontInfo(LOADFONT_PAIR *fontpair, U8 u8num)
{
    U8 i =0;
    for(;i<u8num;i++)
    {
        if(fontpair[i].FontID > MAX_FONTTABLE_NUM_OSD)
            break;//error
        Font[fontpair[i].FontID].fHandle = msAPI_Font_LoadFlashFontToSDRAM(fontpair[i].MMapID);
    }

}


#if VECTOR_FONT_ENABLE
void MApp_LoadFont_SetVecFontInfo(U16 nFontId, LOADFONT_STRUCT* strfont, U8 u8Begin, U8 u8End)
{
    msAPI_Font_MVF_LoadFont(nFontId, strfont, u8Begin, u8End);
}
#endif


#undef MAPP_LOADFONT_C



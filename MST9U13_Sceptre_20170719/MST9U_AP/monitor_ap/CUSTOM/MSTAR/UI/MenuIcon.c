///////////////////////////////////////////////////////////////////////////////
/// @file MenuIcon.c
/// @brief Menu Icon functions.
/// @author MStarSemi Inc.
///
/// Functions to deal with Icons.
///
/// Features
///  -
///////////////////////////////////////////////////////////////////////////////
#define _MENUICON_C

#include "datatype.h"
#include "Board.h"
#include "Global.h"
#include "Keypaddef.h"
#include "menudef.h"
#include "ColorPalette.h"
#include "MenuIconTbl.h"
//#include "DebugMsg.h"
#include "msOSD.h"
#include "ms_reg.h"
#include "Ms_rwreg.h"
#include "Menu.h"
#include "misc.h"

void DrawIcon(BYTE u8XPos, BYTE u8YPos, BYTE *pu8Icon, BYTE u8Flags, BYTE u8ColorSelect)
{
    WORD u16TempValue;
    int i,j;
    BYTE xdata u8LineOffset;
    if (pu8Icon==NULL)
        return;

#define ICON_WIDTH      (*(pu8Icon))
#define ICON_HEIGHT     (*(pu8Icon+1))
#define ICON_COLOR      (*(pu8Icon+2))
#define ICON_HICOLOR    (*(pu8Icon+3))
#define ICON_SELCOLOR   (*(pu8Icon+4))
#define ICON_PTR         (pu8Icon+5)
    switch(u8ColorSelect)
    {
        case CS_NORMAL:
            g_u8OsdFontColor=ICON_COLOR;
            break;
        case CS_HIGHLIGHT:
            g_u8OsdFontColor=ICON_HICOLOR;
            break;
        case CS_SELECTED:
             g_u8OsdFontColor=ICON_SELCOLOR;
            break;
    }
#define IS_DRAW_CODE u8ColorSelect
    if (u8YPos & BIT7)
    {
        u8YPos &= 0x7F;
        IS_DRAW_CODE = FALSE;
    }
    else
        IS_DRAW_CODE = TRUE;

    u16TempValue=g_u8OsdWndNo<<5;
    u8LineOffset=msReadByte(u16TempValue+OSD2_12);
    //u16TempValue=msRead2Byte(u16TempValue+OSD2_10)+(WORD)u8YPos*u8LineOffset+u8XPos; // get real address
    u16TempValue=GET_TXT_CASTART(g_u8OsdWndNo,u8XPos,u8YPos); // get real address
    if (u8Flags&ICO_OVER256)
    {   OSD_TEXT_HI_ADDR_SET_BIT8();
    }
    else if (u8Flags&ICO_OVER512)
        OSD_TEXT_HI_ADDR_SET_BIT9();
    if (u8Flags&ICO_WAIT) //Jison 080521
        WAIT_V_OUTPUT_BLANKING();
    for (i=0 ; i<ICON_HEIGHT; i++)
    {
        #if 0
        if (u8Flags&ICO_BOTTOM && (i!=ICON_HEIGHT-1))
            continue;   ///skip  line
        else if (u8Flags&ICO_TOP && i>0)
            break;
        #endif
        WRITE_CAFSRAM_ADDR();
        msWrite2Byte(PORT_ATTR_ADDR, u16TempValue);
        WRITE_ATTRIBUTE();
        for (j=0 ; j<ICON_WIDTH; j++)
        {   msWriteByte(PORT_ATTR_DATA, g_u8OsdFontColor);
        }
        u16TempValue+=u8LineOffset; // get real address
    }
    if (IS_DRAW_CODE)
    {
        u16TempValue-=(u8LineOffset*ICON_HEIGHT); // recover real address
        for (i=0 ; i<ICON_HEIGHT ; i++)
        {
            #if 0
            if (u8Flags&ICO_BOTTOM && i==0)
                continue;   ///skip 1st line
            else if (u8Flags&ICO_TOP && i==1)
                continue;   ///skip 2nd line
            #endif
            WRITE_CAFSRAM_ADDR();
            msWrite2Byte(PORT_CODE_ADDR, u16TempValue );
            WRITE_CODE();
            for (j=0 ; j<ICON_WIDTH; j++)
            {   msWriteByte(PORT_CODE_DATA, *(ICON_PTR+i*(ICON_WIDTH)+j));
            }
            u16TempValue+=u8LineOffset; // get real address
        }
    }
    if (u8Flags&(ICO_OVER256|ICO_OVER512))
        OSD_TEXT_HI_ADDR_CLR_TO_0();
#undef ICON_WIDTH
#undef ICON_HEIGHT
#undef ICON_COLOR
#undef ICON_HICOLOR
#undef ICON_SELCOLOR
#undef ICON_PTR
#undef IS_DRAW_CODE

}


///=========================================================================
#if 0  //Steven110525
BYTE* GetLogoMstarTopIcon(void)
{ return IcoLogoMstarTop;
}
BYTE* GetLogoMstarTop1Icon(void)
{ return IcoLogoMstarTop1;
}
BYTE* GetLogoMstarDownIcon(void)
{ return IcoLogoMstarDown;
}
#endif

BYTE* GetMainMenuLogoIcon(void)  //Steven110526
{ return IcoMainMenuLogo;
}

BYTE* GetMenuLockIcon(void)
{
    if (HOT_OSD_LOCK_FLAG || OSD_LOCK_FLAG)
        return IcoLock;
    return IcoUnlock;
}


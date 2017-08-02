///////////////////////////////////////////////////////////////////////////////
/// @file MenuPropFont.c
/// @brief
/// @author MStarSemi Inc.
///
/// Functions to Load OSD prop fonts according by menu page.
///
/// Features
///  -
///
///////////////////////////////////////////////////////////////////////////////
#define _MENUPROPFONT_C

//#include <math.h>
#include "PropFontExtFunc.h"
#include "datatype.h"
#include "Board.h"
#include "Global.h"
#include "menudef.h"
#include "ms_reg.h"
#include "msOSD.h"
#include "Menu.h"
//#include "DebugMsg.h"
#include "Ms_rwreg.h"
#include "MenuPropFontTbl.h"
#include "MenuPropFontAsia.h"
#include "Panel.h"
#include "LoadPropFont1218.h"
#include "MenuFont.h"
#include "ColorPalette.h"
#include "MenuPropStr.h"
#include "MenuPropFont.h"
#include "MenuPropStrFontTbl.h"

extern BYTE xdata g_u8MenuPageIndex;

#define MENUPROP_DEBUG    1
#if ENABLE_MSTV_UART_DEBUG&&MENUPROP_DEBUG
    #define MenuProp_printData(str, value)   printData(str, value)
    #define MenuProp_printMsg(str)           printMsg(str)
#else
    #define MenuProp_printData(str, value)
    #define MenuProp_printMsg(str)
#endif

#if 1
#define ENABLE_BREAKREMINDER    0
extern void Font_Transform( BYTE u8Data );

extern WORD code tPropFontSet[];
extern WORD code tPropFontMap[];

extern void printMsg(char *str);
extern void printData(char *str, WORD value);

 
xdata BYTE Font_Transform_Counter = 0;
xdata BYTE Font_Transform_Type = 0;
xdata BYTE Font_Transform_Data[9];

void Font_Transform( BYTE u8Data )
{
    Font_Transform_Data[Font_Transform_Counter] = u8Data;
    Font_Transform_Counter++;
    if (Font_Transform_Type == 0 && Font_Transform_Counter >= 3)
    {
        DWORD xdata u32Data = 0;
        u32Data = ((DWORD)(Font_Transform_Data[0]&0x0F)<<20)
                  |((DWORD)(Font_Transform_Data[0]&0xF0)<<4)
                  |((DWORD)Font_Transform_Data[1]<<12)
                  |((DWORD)Font_Transform_Data[2]);
        WRITE_FONT();
        msWriteByte ( PORT_FONT_DATA, u32Data >> 16 );
        msWriteByte ( PORT_FONT_DATA, u32Data >> 8 );
        msWriteByte ( PORT_FONT_DATA, u32Data >> 0 );
        Font_Transform_Counter = 0;
    }
    else if (Font_Transform_Type == 1 && Font_Transform_Counter >= 3)
    {
        DWORD xdata u32Data = 0;
        WORD xdata u16FontIndex;
        WORD xdata u16FontData1 = ((WORD)Font_Transform_Data[1]) | ((WORD)(Font_Transform_Data[0]&0x0F)<<8);
        WORD xdata u16FontData2 = ((WORD)Font_Transform_Data[2]) | ((WORD)(Font_Transform_Data[0]&0xF0)<<4);
        u16FontIndex = BIT11;
        while ( u16FontIndex )
        {
            u32Data <<= 2;
            if ( u16FontData1 & u16FontIndex ) //color bit0
            {
                u32Data |= BIT0;
            }
            if ( u16FontData2 & u16FontIndex ) //color bit1
            {
                u32Data |= BIT1;
            }
            u16FontIndex >>= 1;
        }
        WRITE_FONT();
        msWriteByte ( PORT_FONT_DATA, u32Data >> 16 );
        msWriteByte ( PORT_FONT_DATA, u32Data >> 8 );
        msWriteByte ( PORT_FONT_DATA, u32Data >> 0 );
        Font_Transform_Counter = 0;
    }
    else if (Font_Transform_Type == 2 && Font_Transform_Counter >= 9)
    {
        DWORD xdata u32Data = 0;
        BYTE xdata u8FontData0, u8FontData1, u8FontData2;
        BYTE xdata u8PixelBit;
        BYTE xdata i;
        for ( i = 0; i < 3; i++ )
        {
            if ( i == 0 )
            {
                u8FontData0 =(Font_Transform_Data[0]<<4) + (Font_Transform_Data[1]>>4);
                u8FontData1 =(Font_Transform_Data[0]&0xF0) + (Font_Transform_Data[2]>>4);
                u8FontData2 =(Font_Transform_Data[3]<<4) + (Font_Transform_Data[4]>>4);
            }
            else if ( i == 1 )
            {
                u8FontData0 =(Font_Transform_Data[1]<<4) + (Font_Transform_Data[3]>>4);
                u8FontData1 =(Font_Transform_Data[2]<<4) + (Font_Transform_Data[6]&0x0F);
                u8FontData2 =(Font_Transform_Data[4]<<4) + (Font_Transform_Data[6]>>4);
            }
            else
            {
                u8FontData0 =(Font_Transform_Data[5]);
                u8FontData1 =(Font_Transform_Data[7]);
                u8FontData2 =(Font_Transform_Data[8]);
            }
            u8PixelBit = BIT7;
            u32Data = 0;
            while ( u8PixelBit )
            {
                u32Data <<= 3;
                if ( u8FontData0 & u8PixelBit ) //color bit0
                {
                    u32Data |= BIT0;
                }
                if ( u8FontData1 & u8PixelBit ) //color bit1
                {
                    u32Data |= BIT1;
                }
                if ( u8FontData2 & u8PixelBit ) //color bit2
                {
                    u32Data |= BIT2;
                }
                u8PixelBit >>= 1;
            }
            WRITE_FONT();
            msWriteByte ( PORT_FONT_DATA, u32Data >> 16 );
            msWriteByte ( PORT_FONT_DATA, u32Data >> 8 );
            msWriteByte ( PORT_FONT_DATA, u32Data );
        }
            Font_Transform_Counter = 0;
    }
    
}

void Osd_LoadFontCompress ( WORD addr, BYTE *fontPtr, WORD num )
{
    BYTE xdata value;
    BYTE xdata count;
    BYTE xdata bNibbleByte;
    BYTE xdata LineCount;
    BYTE xdata temp=0;

    OSD_WRITE_FONT_ADDRESS (LOBYTE(addr));

    while ( 1 )
    {
        LineCount = ( *fontPtr );
        fontPtr++;
        num--;

        if ( LineCount == 0 )
            break;

        if ( LineCount&0x80 )
        {
            LineCount &= 0x7f;
            bNibbleByte = 0;

            while ( 1 )
            {
                value = ( *fontPtr ) & 0x0f;
                count = ( *fontPtr ) >> 4;

                if ( LineCount == 0 )
                    break;

                while ( count )
                {
                    if ( bNibbleByte )
                    {
                        temp = ( temp << 4 ) + value;
                        bNibbleByte = 0;
                        //mStar_WriteByte ( BK0_A4, temp );
                        Font_Transform ( temp );
                    }
                    else
                    {
                        temp = value;
                        bNibbleByte = 1;
                    }

                    count--;
                }

                fontPtr++;

                LineCount--;
                num--;

                if ( num == 0 )
                    break;

            }
        }
        else
        {
            while ( 1 )
            {
                if ( LineCount == 0 )
                    break;

                temp = ( *fontPtr );

                Font_Transform ( temp );

                fontPtr++;

                LineCount--;

                num--;

                if ( num == 0 )
                    break;
            }
        }

        if ( num == 0 )
            break;
    }
}

void ClearPropFontRam ( WORD *font_ram )
{
    BYTE i;
    for ( i = 0; i < 18; i++ )
    {
        font_ram[i] = 0;
    }
}

//===================================================================================================================================================================:
WORD GetFontSpace ( BYTE index )
{
	#if ENABLE_BREAKREMINDER	//120720 Modify
	if(g_u8MenuPageIndex == MENU_BREAKWARNING)
	{
		if(index == 0x1C)		// ">"
		{
			if( g_u32BreakReminderCount / HOUR_UINT <= 24)
				index = 0x01;
		}
		else if(index == 0x14)	// "s"
		{
			if( g_u32BreakReminderCount < (HOUR_UINT*2))
				index = 0x01;
			else
			{
				if(USER_PREF_LANGUAGE == LANG_German)
					index = 0x4F;
				else if(USER_PREF_LANGUAGE == LANG_Swedish)
					index = 0x53;
				else if(USER_PREF_LANGUAGE == LANG_Finnish)
					index = 0x42;
				else if(USER_PREF_LANGUAGE == LANG_Russian)
					index = 0x42;
				else
					index = 0x54;
			}
		}
		else if(index == 0x15)
		{
			if( g_u32BreakReminderCount < (HOUR_UINT*2))
			{
				if(USER_PREF_LANGUAGE == LANG_Swedish)
					index = 0x46;
				else if(USER_PREF_LANGUAGE == LANG_Czech)
					index = 0x42;
				else if(USER_PREF_LANGUAGE == LANG_Italian)	//120723 Modify
					index = 0x42;
			}
			else
			{
				if(USER_PREF_LANGUAGE == LANG_Swedish)
					index = 0x42;
				if(USER_PREF_LANGUAGE == LANG_Czech)
					index = 0x5A;
				else if(USER_PREF_LANGUAGE == LANG_Italian)	//120723 Modify
					index = 0x46;
			}
		}
		else if(index == 0x12)	// Timer High Byte
		{
			if( g_u32BreakReminderCount/10 < HOUR_UINT)
				index = 0x01;
			else if( g_u32BreakReminderCount/HOUR_UINT > 24)
				index = 0x13;
			else
				index = 0x11+g_u32BreakReminderCount/HOUR_UINT/10;
		}
		else if(index == 0x13)	// Timer Low Byte
		{
			if( g_u32BreakReminderCount/HOUR_UINT > 24)
				index = 0x15;
			else
				index = 0x11+g_u32BreakReminderCount/HOUR_UINT%10;
		}
	}
	#endif
    return tPropFontSet[tPropFontMap[index + font_offset]];
}
//===================================================================================================================================================================:
void _GetPropCompressFontData ( BYTE index )
    {
        WORD *pTable;
        BYTE xdata count, i, row;
		#if ENABLE_BREAKREMINDER	//120720 Modify
		if(g_u8MenuPageIndex == MENU_BREAKWARNING)
		{
			if(index == 0x1C)		// ">"
			{
				if( g_u32BreakReminderCount / HOUR_UINT <= 24)
					index = 0x01;
			}
			else if(index == 0x14)	// "s"
			{
				if( g_u32BreakReminderCount < (HOUR_UINT*2))
					index = 0x01;
				else
				{
					if(USER_PREF_LANGUAGE == LANG_German)
						index = 0x4F;
					else if(USER_PREF_LANGUAGE == LANG_Swedish)
						index = 0x53;
					else if(USER_PREF_LANGUAGE == LANG_Finnish)
						index = 0x42;
					else if(USER_PREF_LANGUAGE == LANG_Russian)
						index = 0x42;
					else
						index = 0x54;
				}
			}
			else if(index == 0x15)
			{
				if( g_u32BreakReminderCount < (HOUR_UINT*2))
				{
					if(USER_PREF_LANGUAGE == LANG_Swedish)
						index = 0x46;
					else if(USER_PREF_LANGUAGE == LANG_Czech)
						index = 0x42;
					else if(USER_PREF_LANGUAGE == LANG_Italian)	//120723 Modify
						index = 0x42;
				}
				else
				{
					if(USER_PREF_LANGUAGE == LANG_Swedish)
						index = 0x42;
					if(USER_PREF_LANGUAGE == LANG_Czech)
						index = 0x5A;
					else if(USER_PREF_LANGUAGE == LANG_Italian)	//120723 Modify
						index = 0x46;
				}
			}
			else if(index == 0x12)	// Timer High Byte
			{
				if( g_u32BreakReminderCount/10 < HOUR_UINT)
					index = 0x01;
				else if( g_u32BreakReminderCount/HOUR_UINT > 24)
					index = 0x13;
				else
					index = 0x11+g_u32BreakReminderCount/HOUR_UINT/10;
			}
			else if(index == 0x13)	// Timer Low Byte
			{
				if( g_u32BreakReminderCount/HOUR_UINT > 24)
					index = 0x15;
				else
					index = 0x11+g_u32BreakReminderCount/HOUR_UINT%10;
			}
		}
		#endif
			
        pTable = & ( tPropFontSet[tPropFontMap[index + font_offset]] );
        pTable++;
        row = 0;
        do
        {
            count = ( *pTable ) >> 12;
            for ( i = 0; i < count; i++ )
            {
                CFontData[row + i] = ( *pTable ) & 0x0fff;
            }
            row += count;
            pTable++;
        }
        while ( row < 18 );
    }


//===================================================================================================================================================================:
void LoadFontRam ( WORD *font_ram )
{
    BYTE i;
    WORD font_data;
    for ( i = 0; i < 9; i++ )
    {
        font_data = ( font_ram[2 * i] >> 8 ) | ( ( font_ram[2 * i + 1] & 0xf00 ) >> 4 );
        //msWriteByte ( PORT_FONT_DATA, ( BYTE ) font_data );
        Font_Transform( ( BYTE ) font_data );
        font_data = font_ram[2 * i] & 0x0ff;
        //msWriteByte ( PORT_FONT_DATA, ( BYTE ) font_data );
        Font_Transform( ( BYTE ) font_data );
        font_data = font_ram[2 * i + 1] & 0x0ff;
        //msWriteByte ( PORT_FONT_DATA, ( BYTE ) font_data );
        Font_Transform( ( BYTE ) font_data );
    }
}

BYTE LoadCompressedPropFonts ( BYTE *font, WORD num )
{
    Bool not_prop;
    BYTE i;
    BYTE l_space, r_space, font_width;
    BYTE NeedSpaceWidth;
    WORD font_data;
    WORD TotalFont = 0;
    WORD PropFontRam[18];
    WORD FontIndex = 0;
    font_offset = 0;
#if 1
    if ( font[FontIndex] == FONT_COMMAND )
    {
        FontIndex ++;
        num--;
        font_offset = font[FontIndex] * 0x0100;
        FontIndex ++;
        num--;
    }
#endif
	#if ENABLE_BREAKREMINDER	//120720 Modify
	if(g_u8MenuPageIndex == MENU_BREAKWARNING)
	{
		if( font[FontIndex] == 0x1C )		// ">"
		{
			if( g_u32BreakReminderCount / HOUR_UINT <= 24)
			{
				FontIndex ++;
				num--;
			}
		}
	}
	#endif
    //GetPropFontTable ();
    ClearPropFontRam ( PropFontRam );
    NeedSpaceWidth = 0;
    PropFontRamWidth = INIT_WORD_SPACE;
    //PropFontRamWidth = ( BYTE ) font_shift;
    font_width = ( BYTE ) GetFontSpace ( font[FontIndex] );
    l_space = font_width >> 4;
    r_space = font_width & 0x0f;
    font_width = 12 - l_space - r_space;
    TotalFontWidth += font_width; //new
    FontUsedPixel = font_width;
    while ( num )
    {
#if 1
        if ( font[FontIndex] == FONT_COMMAND )
        {
            FontIndex ++;
            num--;
            font_offset = ( font[FontIndex]&0x0F) * 0x0100;
            FontIndex ++;
            num--;
        }
#endif
        _GetPropCompressFontData ( font[FontIndex] );
        for ( i = 0; i < 18; i++ )
        {
            font_data = CFontData[i];
            PropFontRam[i] = PropFontRam[i] | ( ( ( font_data << ( l_space + font_width - FontUsedPixel ) ) & 0x0fff ) >> PropFontRamWidth );
        }
        PropFontRamWidth += FontUsedPixel;
        if ( PropFontRamWidth > 12 )
        {
            FontUsedPixel = PropFontRamWidth - 12;
            PropFontRamWidth = 12;
        }
        else
        {
            FontUsedPixel = 0;
        }
        if ( FontUsedPixel == 0 )  // word finish
        {
            if ( num )
            {
                num--;
                if ( num == 0 )
                {
                    TotalFont++;
                    LoadFontRam ( PropFontRam );
			#if ENABLE_BREAKREMINDER	//120713 Modify
			g_u8BreakWarningMenuTotalFont=TotalFont;
			#endif
                    return ( BYTE ) TotalFont;
                }
            }
            FontIndex++;
			#if ENABLE_BREAKREMINDER	//120720 Modify
			if(g_u8MenuPageIndex == MENU_BREAKWARNING)
			{
				if ( font[FontIndex] == 0x14 )
				{
					if( g_u32BreakReminderCount < (HOUR_UINT*2))
					{
						FontIndex ++;
						num--;
					}
				}
				else if( font[FontIndex] == 0x1C )		// ">"
				{
					if( g_u32BreakReminderCount / HOUR_UINT <= 24)
					{
						FontIndex ++;
						num--;
					}
				}
				else if(font[FontIndex] == 0x12)	// Timer High Byte
				{
					if( g_u32BreakReminderCount/10 < HOUR_UINT)
					{
						FontIndex ++;
						num--;
					}
				}
			}
			#endif
#if 1
            if ( font[FontIndex] == FONT_COMMAND )
            {
                FontIndex ++;
                num--;
                font_offset =( font[FontIndex]&0x0F) * 0x0100;
                FontIndex ++;
                num--;
            }
#endif
            if ( font[FontIndex] == 0x00 )  // new string
            {
                TotalFont++;
                LoadFontRam ( PropFontRam );
                if ( num )
                {
                    num--;
                    if ( num == 0 )
                    {
                    	  #if ENABLE_BREAKREMINDER	//120713 Modify
                        g_u8BreakWarningMenuTotalFont=TotalFont;
			  #endif
                        return ( BYTE ) TotalFont;
                    }
                }
                FontIndex++;
                ClearPropFontRam ( PropFontRam );
                PropFontRamWidth = INIT_WORD_SPACE;
                NeedSpaceWidth = 0;
            }
            else  // next word
            {
#if 1
                if ( font[FontIndex] == FONT_COMMAND )
                {
                    FontIndex ++;
                    num--;
                    font_offset = ( font[FontIndex]&0x0F) * 0x0100;
                    FontIndex ++;
                    num--;
                }
#endif
                //GetPropFontTable ();
                if ( GetFontSpace ( font[FontIndex] ) & 0x8000 )
                {
                    not_prop = 1;
                }
                else
                {
                    not_prop = 0;
                    NeedSpaceWidth += WORD_SPACE;
                    TotalFontWidth += WORD_SPACE; //new
                }
            }
#if 1
            if ( font[FontIndex] == FONT_COMMAND )
            {
                FontIndex ++;
                num--;
                font_offset = ( font[FontIndex]&0x0F) * 0x0100;
                FontIndex ++;
                num--;
            }
#endif
            //GetPropFontTable ();
            font_width = ( BYTE ) GetFontSpace ( font[FontIndex] );
            l_space = font_width >> 4;
            r_space = font_width & 0x0f;
            font_width = 12 - l_space - r_space;
            TotalFontWidth += font_width; //new
            FontUsedPixel = font_width;
        }
        PropFontRamWidth += NeedSpaceWidth;
        if ( PropFontRamWidth > 12 )
        {
            NeedSpaceWidth = PropFontRamWidth - 12;
            PropFontRamWidth = 12;
        }
        else
        {
            NeedSpaceWidth = 0;
        }
        if ( PropFontRamWidth == 12 )
        {
            TotalFont++;
            LoadFontRam ( PropFontRam );
            ClearPropFontRam ( PropFontRam );
            PropFontRamWidth = 0;
        }
        PropFontRamWidth += NeedSpaceWidth;
        NeedSpaceWidth = 0;
    }
	#if ENABLE_BREAKREMINDER	//120713 Modify
	g_u8BreakWarningMenuTotalFont=TotalFont;
	#endif
    return ( BYTE ) TotalFont;
}

#endif



static MenuFontType * code tMenuFontTbl[]=
{
    NULL,   //PowerOffMenu,
    NULL,   //PowerOnMenu,
    MenuPage41,//StandbyMenu,
    MenuPage40,//CableNotConnectedMenu,
    MenuPage41,//MENU_ENTERPOWERSAVING,
    MenuPage43,//UnsupportedModeMenu,

    NULL,//RootMenu, /// Pre-load main menu font in root menu
#if (defined(LianHeChuangXin_Project))
    NULL,//  MENU_KEY_GUIDE
#endif

#if LOGO_ENABLE
    NULL,//LogoMenu,
#endif    
#if ENABLE_VGA_INPUT
    MenuPage11,//AutoMenu,
    MenuPage11,//HotAutoMenu,
#endif
#if ENABLE_MULTI_INPUT
    MenuPage1,//InputMenu,
#endif
    NULL,//,    HotMenuLockMenu
#if ENABLE_DEMURA_FUNCTION
    NULL, //MENU_DEMURA_MEASURE_MODE
#endif
    NULL,//MainMenu,
//SUB MENU
    MenuPage1,//QuickStartMenu,
    MenuPage2,//PictureMenu,
    MenuPage3,//ColorMenu,
    MenuPage4,//SystemMenu,
#if LED_HOT_Menu_Func    
    MenuPage4,//MENU_SYSTEMLLEDUSER,
#endif
    MenuPage3,//UserMenu,
    MenuPage3,//AdvancedMenu,
#if LED_HOT_Menu_Func    
    MenuPage4,//LEdModeMenu,
#endif

#if ENABLE_MULTI_INPUT
    MenuPage1,//InputSourceMenu,
#endif
#if ENABLE_AUDIO_SETTINGS_MENU
    MenuPage4,//MENU_HOTVOLUME
#endif
    MenuPage2,//MENU_HOTBRIGHTNESS,
    MenuPage2,//MENU_HOTCONTRAST,
#if Enable_HotAdjLowbluray    
    MenuPage15,//MENU_HOTLOWBLURAY,
#endif    
    MenuPage4,//MenuPage4,//MENU_HOTPICTUREFORMAT
    MenuPage2,//MenuPage3,//MENU_HOTPICTUREMODE
#if ENABLE_AUDIO_SETTINGS_MENU    
    NULL,//MENU_HOTMUTE
#endif    
#if HKC_RGB_Range_Func
    MenuPage16,//MENU_HOTRGBRANGE
#endif    
    MenuPage2,//MENU_HOTDCR
    MenuPage1,//MENU_HOTINPUT
    NULL,//MENU_HOTCROSS
#if SleepMode_Enable    
    MenuPage14,//MENU_POWEROFFMSG
#endif    

#if SleepMode_Enable    
    MenuPage4,////MENU_SLEEPMODE
#endif
#if 0//LED_HOT_Menu_Func    
    MenuPage17,////MENU_HOTLEDMODE
#endif
#if Enable_Corss_LHCX_New    

    MenuPage18,//HotKeyGameModeMenu,		
    
    NULL,//HotKeyGameSelectMenu,
    NULL,//HotKeyGameSubSelectMenu,
    NULL,//HotKeyGameClokMenu,
    NULL,//HotKeyGameClokPositionMenu,
    NULL,//HotKeyGameClokShowMenu,

#endif


    NULL,
};

void Osd_DynamicLoadFont(WORD u8Addr, BYTE *pu8FontPtr, WORD u16Num)
{
		TotalFontWidth = 0;
	if(( u8Addr & 0xFF00 ) == 0x100 )
	OSD_FONT_HI_ADDR_SET_BIT8();
	else if(( u8Addr & 0xFF00 ) == 0x200 )
	OSD_FONT_HI_ADDR_SET_BIT9();
	else if(( u8Addr & 0xFF00 ) == 0x300 )
	OSD_FONT_HI_ADDR_SET_BIT10();
	else
	OSD_FONT_HI_ADDR_CLR_TO_0();
	
        OSD_WRITE_FONT_ADDRESS(LOBYTE(u8Addr));
        LoadCompressedPropFonts( pu8FontPtr, u16Num);
	 OSD_FONT_HI_ADDR_CLR_TO_0();
  
}

void DynamicLoadPropFont(void)
{
    MenuFontType *pstMenuFonts=NULL;
    pstMenuFonts = tMenuFontTbl[g_u8MenuPageIndex];

    if (pstMenuFonts->pu8Fonts)
    {
        MenuProp_printData("LoadPropFont g_u8MenuPageIndex==%d",g_u8MenuPageIndex);
	 Osd_DynamicLoadFont(PROP_FONT_START, (BYTE*)((pstMenuFonts+PROPFONTTBL_INDEX)->pu8Fonts), (pstMenuFonts+PROPFONTTBL_INDEX)->u16FontCount);

    }
}

void LoadMenuCommonPropFont(void)
{
    MenuFontType *pstMenuFonts;
    pstMenuFonts = MenuPage8;

    printMsg("LoadMenuCommonPropFont");

    if (pstMenuFonts->pu8Fonts)
    Osd_DynamicLoadFont(Common_PROP_FONT_START, (BYTE*)((pstMenuFonts+PROPFONTTBL_INDEX)->pu8Fonts), (pstMenuFonts+PROPFONTTBL_INDEX)->u16FontCount);
}

void LoadPercentPropFont(void)
{
    MenuFontType *pstMenuFonts;

if(USER_PREF_TRANSPARENCY==1)	
    pstMenuFonts = MenuPage51;
else if(USER_PREF_TRANSPARENCY==2)
    pstMenuFonts = MenuPage52;
else if(USER_PREF_TRANSPARENCY==3)
    pstMenuFonts = MenuPage53;
else if(USER_PREF_TRANSPARENCY==4)
    pstMenuFonts = MenuPage54;
else
    pstMenuFonts = MenuPage50;
	
    printMsg("MenuPage50");

    if (pstMenuFonts->pu8Fonts)
    Osd_DynamicLoadFont(Menu_Percent_Start, (BYTE*)((pstMenuFonts+PROPFONTTBL_INDEX)->pu8Fonts), (pstMenuFonts+PROPFONTTBL_INDEX)->u16FontCount);
}

void LoadMenuDescriptionPropFont(void)
{
    MenuFontType *pstMenuFonts=NULL;

    printMsg("LoadMenuDescriptionPropFont");

if(g_u8MenuPageIndex==MENU_QUICKSTART)
{
  if(g_u8MenuItemIndex<=QS_BackLight_Adj_IDX)
        pstMenuFonts = MenuPage9;//BackLight
  else if(g_u8MenuItemIndex<=QS_Brightness_Adj_IDX)
        pstMenuFonts = MenuPage10;//Brightness
  else if(g_u8MenuItemIndex<=QS_Contrast_Adj_IDX)
        pstMenuFonts = MenuPage11;//Contrast
   else if(g_u8MenuItemIndex<=QS_Preset_Adj_IDX)
        pstMenuFonts = MenuPage12;//Preset
    else if(g_u8MenuItemIndex<=QS_Aspect_Adj_IDX)
        pstMenuFonts = MenuPage13;//Aspect
    else if(g_u8MenuItemIndex<=QS_Source_Adj_IDX)
        pstMenuFonts = MenuPage33;//Source
    else if(g_u8MenuItemIndex<=QS_BlueLight_Adj_IDX)
        pstMenuFonts = MenuPage14;//BlueLight
    #if Free_Sync_Enable
    else if(g_u8MenuItemIndex<=QS_FreeSync_Adj_IDX)
        pstMenuFonts = MenuPage15;//FreeSync
    #endif
    else 		
        pstMenuFonts = MenuPage15;//FreeSync
	
}
else if(g_u8MenuPageIndex==MENU_PICTURE)
{
  if(g_u8MenuItemIndex<=Pic_BackLight_Adj_IDX)
        pstMenuFonts = MenuPage9;//BackLight
  else if(g_u8MenuItemIndex<=Pic_Brightness_Adj_IDX)
        pstMenuFonts = MenuPage10;//Brightness
  else if(g_u8MenuItemIndex<=Pic_Contrast_Adj_IDX)
        pstMenuFonts = MenuPage11;//Contrast
   else if(g_u8MenuItemIndex<=Pic_Preset_Adj_IDX)
        pstMenuFonts = MenuPage12;//Preset
    else if(g_u8MenuItemIndex<=Pic_DCR_Adj_IDX)
        pstMenuFonts = MenuPage16;//DCR
    else if(g_u8MenuItemIndex<=Pic_Aspect_Adj_IDX)
        pstMenuFonts = MenuPage13;//Aspect
    else if(g_u8MenuItemIndex<=Pic_Sharpness_Adj_IDX)
        pstMenuFonts = MenuPage17;//Sharpness
    else 		
        pstMenuFonts = MenuPage17;//Sharpness
}
else if(g_u8MenuPageIndex==MENU_COLOR)
{
  if(g_u8MenuItemIndex<=Col_Temperature_Adj_IDX)
  {
/*  
    if(CURRENT_COLOR_TEMP==PCT_Advanced)
        pstMenuFonts = MenuPage32;//ColorTempAdvanced
    else	
*/		
        pstMenuFonts = MenuPage18;//ColorTemp
  }
  else if(g_u8MenuItemIndex<=Col_Gamma_Adj_IDX)
        pstMenuFonts = MenuPage19;//Gamma
  else if(g_u8MenuItemIndex<=Col_Tint_Adj_IDX)
        pstMenuFonts = MenuPage20;//Tint
  else if(g_u8MenuItemIndex<=Col_Saturation_Adj_IDX)
        pstMenuFonts = MenuPage21;//Saturation
  else if(g_u8MenuItemIndex<=Col_BlueLight_Adj_IDX)
        pstMenuFonts = MenuPage14;//BlueLight
  else
  	 pstMenuFonts = MenuPage14;//BlueLight
}
else if(g_u8MenuPageIndex==MENU_SYSTEM
#if LED_HOT_Menu_Func
||g_u8MenuPageIndex==MENU_SYSTEMLLEDUSER
#endif
)
{
  if(g_u8MenuItemIndex<=Sys_OverDrive_Adj_IDX)
        pstMenuFonts = MenuPage22;//OD
  #if Free_Sync_Enable
  else if(g_u8MenuItemIndex<=Sys_FreeSync_Adj_IDX)
        pstMenuFonts = MenuPage15;//FreeSync
   #endif
  else if(g_u8MenuItemIndex<=Sys_SleepMode_Adj_IDX)
        pstMenuFonts = MenuPage23;//SleepMde
#if LED_HOT_Menu_Func
  else if(g_u8MenuItemIndex<=Sys_LedMode_Adj_IDX)
        pstMenuFonts = MenuPage37;//LedMde
#endif
  else if(g_u8MenuItemIndex<=Sys_Language_Adj_IDX)
        pstMenuFonts = MenuPage24;//Language
  else if(g_u8MenuItemIndex<=Sys_OSDHPosition_Adj_IDX)
        pstMenuFonts = MenuPage25;//OSDHPosition
#if Enable_OSD_Vposition        
  else if(g_u8MenuItemIndex<=Sys_OSDVPosition_Adj_IDX)
        pstMenuFonts = MenuPage26;//OSDVPosition
#endif        
  else if(g_u8MenuItemIndex<=Sys_OSDTimer_Adj_IDX)
        pstMenuFonts = MenuPage27;//OSDTimer
  else if(g_u8MenuItemIndex<=Sys_OSDTransparencia_Adj_IDX)
        pstMenuFonts = MenuPage28;//OSDTransparency
#if ENABLE_AUDIO_SETTINGS_MENU        
  else if(g_u8MenuItemIndex<=Sys_Volume_Adj_IDX)
        pstMenuFonts = MenuPage29;//Volume
#endif        
  else if(g_u8MenuItemIndex<=Sys_Reset_Adj_IDX)
        pstMenuFonts = MenuPage30;//Reset
  else      
        pstMenuFonts = MenuPage30;//Reset

}
#if LED_HOT_Menu_Func
else if(g_u8MenuPageIndex==MENU_LEDMODE)
{
  if(g_u8MenuItemIndex<=Led_Mode_Adj_IDX)
        pstMenuFonts = MenuPage34;//Mode
  else if(g_u8MenuItemIndex<=Led_Color_Adj_IDX)
        pstMenuFonts = MenuPage35;//Color
  else if(g_u8MenuItemIndex<=Led_Brigntness_Adj_IDX)
        pstMenuFonts = MenuPage36;//Brightness
  else
        pstMenuFonts = MenuPage37;//LED All
}
#endif
else if(g_u8MenuPageIndex==MENU_USER)
{
  if(g_u8MenuItemIndex>=User_RGBCMY_IDX)
        pstMenuFonts = MenuPage32;//Advanced
  else
        pstMenuFonts = MenuPage18;//LED All

}
else
{
    pstMenuFonts = MenuPage9;
}

    if (pstMenuFonts->pu8Fonts)
    Osd_DynamicLoadFont(Description_PROP_FONT_START, (BYTE*)((pstMenuFonts+PROPFONTTBL_INDEX)->pu8Fonts), (pstMenuFonts+PROPFONTTBL_INDEX)->u16FontCount);
}



void LoadMainMenuFont(void)
{
    MenuFontType *pstMenuFonts=NULL;

    printMsg("LoadMainMenuFont");
 if(IS_MAIN_L0(g_u8MenuPageIndex))
 {
   if(g_u8MenuItemIndex==0)//MENU_QUICKSTART
    pstMenuFonts = MenuPage111;
   else if(g_u8MenuItemIndex==1)//MENU_PICTURE
    pstMenuFonts = MenuPage222;
   else if(g_u8MenuItemIndex==2)//MENU_COLOR
    pstMenuFonts = MenuPage333;
   else if(g_u8MenuItemIndex==3)//MENU_SYSTEM
    pstMenuFonts = MenuPage444;
   else
    pstMenuFonts = MenuPage444;   	
  
 }

    if (pstMenuFonts->pu8Fonts)
	 Osd_DynamicLoadFont(PROP_FONT_START, (BYTE*)((pstMenuFonts+PROPFONTTBL_INDEX)->pu8Fonts), (pstMenuFonts+PROPFONTTBL_INDEX)->u16FontCount);

}


#if LOGO_ENABLE
void LoadLogoMenuFont(void)
{
    MenuProp_printMsg("LoadLogoMenuFont");
    msWrite2ByteMask(OSD1_0A,Logo4CFontStart,0x03FF); //4 color start
    msWrite2ByteMask(OSD1_0C,0x03FF,0x03FF);//0x0200+_8COLOR_FONT_START); //8 color start
    msWrite2ByteMask(OSD1_0E,0x03FF,0x03FF);//0x2C0);                     //16 color start		
    LoadLogoFont();
    LoadLogoCP();
	
}
#endif

#define SC_SPRVST_MASK  0xFFF
#define SC_SPRHST_MASK  0xFFF
#define SC_SPRVDC_MASK  0xFFF
#define SC_VTOTAL_MASK  0xFFF

void LoadCurrentResolutionStringFont(void )
{
    pstPropFontSet1218=tPropFontSetArialNarrow13Bold;
    g_u8PropFontFlags=SPACE2PIXEL|LEFT_ALIGN;
    g_u8AlignResetIndex=0xFF;
    #define SHOW_HZ TRUE


    if (INPUT_IS_VGA(MapWin2Port(USER_PREF_WIN_SEL)))
        //LoadResolutionString(MM_PROP_RESOLUTION_INFO_START, tPropText[0],StandardModeWidth(MapWin2Port(USER_PREF_WIN_SEL)),StandardModeHeight(MapWin2Port(USER_PREF_WIN_SEL)),(BYTE)((StandardModeVFreq(MapWin2Port(USER_PREF_WIN_SEL))+5)/10),SHOW_HZ);
        LoadResolutionString(MM_PROP_RESOLUTION_INFO_START, tPropText[0],StandardModeWidth(MapWin2Port(USER_PREF_WIN_SEL)),StandardModeHeight(MapWin2Port(USER_PREF_WIN_SEL)),0, 0);
    else
    {
        WORD u16Width=0,u16Height=0,g_u16TempVFreq=0;
        g_u16TempVFreq = GetInputVfreq(USER_PREF_WIN_SEL);
#if 1
         u16Width=InputWidth;
	  u16Height=InputHeight;	
#else		
        if(FreeSyncFlag)
        {
         u16Width=InputWidth;
	  u16Height=InputHeight;	
    	}
	else
	{
        u16Width = msDrvIP1ReadAutoWidth(MapPort2DetScIndex(MapWin2Port(USER_PREF_WIN_SEL)));
        u16Height = msDrvIP1ReadAutoHeight(MapPort2DetScIndex(MapWin2Port(USER_PREF_WIN_SEL)));
        if (HBR2Flag(MapWin2Port(USER_PREF_WIN_SEL)))
		u16Width*=2;
    	}
#endif

    if(g_u16TempVFreq<10)
	 g_u16TempVFreq=InputVfreq;
	 
	 g_u16TempVFreq=(BYTE)((g_u16TempVFreq+5)/10);
	if(g_u16TempVFreq==59||g_u16TempVFreq==61)
          g_u16TempVFreq=60;	

	if(abs(u16Height-1080)<8)
          u16Height=1080;
	else if(abs(u16Height-480)<8)
          u16Height=480;
	else if(abs(u16Height-576)<8)
          u16Height=576;
	
    //if((INPUT_IS_HDMI(InputPort))&&(!INPUT_IS_HDMI_DRR(InputPort)))
    {
      if((abs(u16Width-720)<5))
      	{
	     u16Width=720;		
      	}
	else if((abs(u16Width-1280)<5))   
      	{
	     u16Width=1280;		
      	}
	else if((abs(u16Width-1920)<5))   
      	{
	     u16Width=1920;		
      	}
	else if((abs(u16Width-2560)<5))   
      	{
	     u16Width=2560;		
      	}
	else if((abs(u16Width-3840)<5))   
      	{
	     u16Width=3840;		
      	}
	else if((abs(u16Width-4096)<5))   
      	{
	     u16Width=4096;		
      	}

    }
        LoadResolutionString(MM_PROP_RESOLUTION_INFO_START, tPropText[0], u16Width, u16Height, 0 ,0);
    }

#if ENABLE_MULTI_WINDOW_SETTINGS_MENU
    if (ReadyForChangeFlag(USER_PREF_WIN_SRC(DISPLAY_WIN1)))
    {

        if (INPUT_IS_VGA(MapWin2Port(DISPLAY_WIN1)))
            LoadResolutionString(MW1_PROP_RESOLUTION_INFO_START, tPropText[2],StandardModeWidth(MapWin2Port(DISPLAY_WIN1)),StandardModeHeight(MapWin2Port(DISPLAY_WIN1)),(BYTE)((StandardModeVFreq(MapWin2Port(DISPLAY_WIN1))+5)/10),SHOW_HZ);
        else
        {
            WORD u16Width_Win1=0,u16Height_Win1=0,g_u16TempVFreq_Win1=0;
            g_u16TempVFreq_Win1 = GetInputVfreq(DISPLAY_WIN1);
#if 1
         u16Width_Win1=InputWidth;
	  u16Height_Win1=InputHeight;	
#else			
        if(FreeSyncFlag)
        {
         u16Width_Win1=InputWidth;
	  u16Height_Win1=InputHeight;	
       }
	else
	{
            u16Width_Win1 = msDrvIP1ReadAutoWidth(MapPort2DetScIndex(MapWin2Port(DISPLAY_WIN1)));
            u16Height_Win1 = msDrvIP1ReadAutoHeight(MapPort2DetScIndex(MapWin2Port(DISPLAY_WIN1)));
            if (HBR2Flag(MapWin2Port(DISPLAY_WIN1)))
            	u16Width_Win1*=2;
	}
#endif	

    if(g_u16TempVFreq_Win1<10)
	 g_u16TempVFreq_Win1=InputVfreq;
			
	 g_u16TempVFreq_Win1=(BYTE)((g_u16TempVFreq_Win1+5)/10);
	if(g_u16TempVFreq_Win1==59||g_u16TempVFreq_Win1==61)
          g_u16TempVFreq_Win1=60;		

	if(abs(u16Height_Win1-1080)<8)
          u16Height_Win1=1080;
	else if(abs(u16Height_Win1-480)<8)
          u16Height_Win1=480;
	else if(abs(u16Height_Win1-576)<8)
          u16Height_Win1=576;

{
      if((abs(u16Width_Win1-720)<5))
      	{
	     u16Width_Win1=720;		
      	}
	else if((abs(u16Width_Win1-1280)<5))   
      	{
	     u16Width_Win1=1280;		
      	}
	else if((abs(u16Width_Win1-1920)<5))   
      	{
	     u16Width_Win1=1920;		
      	}
	else if((abs(u16Width_Win1-2560)<5))   
      	{
	     u16Width_Win1=2560;		
      	}
	else if((abs(u16Width_Win1-3840)<5))   
      	{
	     u16Width_Win1=3840;		
      	}
	else if((abs(u16Width_Win1-4096)<5))   
      	{
	     u16Width_Win1=4096;		
      	}

    }
	
            LoadResolutionString(MW1_PROP_RESOLUTION_INFO_START, tPropText[2], u16Width_Win1, u16Height_Win1, g_u16TempVFreq_Win1,SHOW_HZ);
        }
    }

    if (ReadyForChangeFlag(USER_PREF_WIN_SRC(DISPLAY_WIN2)))
    {

        if (INPUT_IS_VGA(MapWin2Port(DISPLAY_WIN2)))
            LoadResolutionString(MW2_PROP_RESOLUTION_INFO_START, tPropText[3],StandardModeWidth(MapWin2Port(DISPLAY_WIN2)),StandardModeHeight(MapWin2Port(DISPLAY_WIN2)),(BYTE)((StandardModeVFreq(MapWin2Port(DISPLAY_WIN2))+5)/10),SHOW_HZ);
        else
        {
            WORD u16Width_Win2=0,u16Height_Win2=0,g_u16TempVFreq_Win2=0;
            g_u16TempVFreq_Win2 = GetInputVfreq(DISPLAY_WIN2);
            u16Width_Win2 = msDrvIP1ReadAutoWidth(MapPort2DetScIndex(MapWin2Port(DISPLAY_WIN2)));
            u16Height_Win2 = msDrvIP1ReadAutoHeight(MapPort2DetScIndex(MapWin2Port(DISPLAY_WIN2)));
            if (HBR2Flag(MapWin2Port(DISPLAY_WIN2)))
            	u16Width_Win2*=2;
    if(g_u16TempVFreq_Win2<10)
	 g_u16TempVFreq_Win2=InputVfreq;
 	 g_u16TempVFreq_Win2=(BYTE)((g_u16TempVFreq_Win2+5)/10);
	if(g_u16TempVFreq_Win2==59||g_u16TempVFreq_Win2==61)
          g_u16TempVFreq_Win2=60;		

	if(abs(u16Height_Win2-1080)<8)
          u16Height_Win2=1080;
	else if(abs(u16Height_Win2-480)<8)
          u16Height_Win2=480;
	else if(abs(u16Height_Win2-576)<8)
          u16Height_Win2=576;

{
      if((abs(u16Width_Win2-720)<5))
      	{
	     u16Width_Win2=720;		
      	}
	else if((abs(u16Width_Win2-1280)<5))   
      	{
	     u16Width_Win2=1280;		
      	}
	else if((abs(u16Width_Win2-1920)<5))   
      	{
	     u16Width_Win2=1920;		
      	}
	else if((abs(u16Width_Win2-2560)<5))   
      	{
	     u16Width_Win2=2560;		
      	}
	else if((abs(u16Width_Win2-3840)<5))   
      	{
	     u16Width_Win2=3840;		
      	}
	else if((abs(u16Width_Win2-4096)<5))   
      	{
	     u16Width_Win2=4096;		
      	}

    }
	
           LoadResolutionString(MW2_PROP_RESOLUTION_INFO_START, tPropText[3], u16Width_Win2, u16Height_Win2, g_u16TempVFreq_Win2,SHOW_HZ);
        }
    }
    if (ReadyForChangeFlag(USER_PREF_WIN_SRC(DISPLAY_WIN3)))
    {

        if (INPUT_IS_VGA(MapWin2Port(DISPLAY_WIN3)))
            LoadResolutionString(MW3_PROP_RESOLUTION_INFO_START, tPropText[4],StandardModeWidth(MapWin2Port(DISPLAY_WIN3)),StandardModeHeight(MapWin2Port(DISPLAY_WIN3)),(BYTE)((StandardModeVFreq(MapWin2Port(DISPLAY_WIN3))+5)/10),SHOW_HZ);
        else
        {
            WORD u16Width_Win3=0,u16Height_Win3=0,g_u16TempVFreq_Win3=0;

            g_u16TempVFreq_Win3 =  GetInputVfreq(DISPLAY_WIN3);
            u16Width_Win3 = msDrvIP1ReadAutoWidth(MapPort2DetScIndex(MapWin2Port(DISPLAY_WIN3)));
            u16Height_Win3 = msDrvIP1ReadAutoHeight(MapPort2DetScIndex(MapWin2Port(DISPLAY_WIN3)));
            if (HBR2Flag(MapWin2Port(DISPLAY_WIN3)))
            	u16Width_Win3*=2;

    if(g_u16TempVFreq_Win3<10)
	 g_u16TempVFreq_Win3=InputVfreq;
   	 g_u16TempVFreq_Win3=(BYTE)((g_u16TempVFreq_Win3+5)/10);
	if(g_u16TempVFreq_Win3==59||g_u16TempVFreq_Win3==61)
          g_u16TempVFreq_Win3=60;		

	if(abs(u16Height_Win3-1080)<8)
          u16Height_Win3=1080;
	else if(abs(u16Height_Win3-480)<8)
          u16Height_Win3=480;
	else if(abs(u16Height_Win3-576)<8)
          u16Height_Win3=576;

{
      if((abs(u16Width_Win3-720)<5))
      	{
	     u16Width_Win3=720;		
      	}
	else if((abs(u16Width_Win3-1280)<5))   
      	{
	     u16Width_Win3=1280;		
      	}
	else if((abs(u16Width_Win3-1920)<5))   
      	{
	     u16Width_Win3=1920;		
      	}
	else if((abs(u16Width_Win3-2560)<5))   
      	{
	     u16Width_Win3=2560;		
      	}
	else if((abs(u16Width_Win3-3840)<5))   
      	{
	     u16Width_Win3=3840;		
      	}
	else if((abs(u16Width_Win3-4096)<5))   
      	{
	     u16Width_Win3=4096;		
      	}

    }

          LoadResolutionString(MW3_PROP_RESOLUTION_INFO_START, tPropText[4], u16Width_Win3, u16Height_Win3, g_u16TempVFreq_Win3,SHOW_HZ);
        }
    }
    if (ReadyForChangeFlag(USER_PREF_WIN_SRC(DISPLAY_WIN4)))
    {

        if (INPUT_IS_VGA(MapWin2Port(DISPLAY_WIN4)))
            LoadResolutionString(MW4_PROP_RESOLUTION_INFO_START, tPropText[5],StandardModeWidth(MapWin2Port(DISPLAY_WIN4)),StandardModeHeight(MapWin2Port(DISPLAY_WIN4)),(BYTE)((StandardModeVFreq(MapWin2Port(DISPLAY_WIN4))+5)/10),SHOW_HZ);
        else
        {
            WORD u16Width_Win4=0,u16Height_Win4=0,g_u16TempVFreq_Win4=0;
            g_u16TempVFreq_Win4 =  GetInputVfreq(DISPLAY_WIN4);
            u16Width_Win4 = msDrvIP1ReadAutoWidth(MapPort2DetScIndex(MapWin2Port(DISPLAY_WIN4)));
            u16Height_Win4 = msDrvIP1ReadAutoHeight(MapPort2DetScIndex(MapWin2Port(DISPLAY_WIN4)));
            if (HBR2Flag(MapWin2Port(DISPLAY_WIN4)))
            	u16Width_Win4*=2;
    if(g_u16TempVFreq_Win4<10)
	 g_u16TempVFreq_Win4=InputVfreq;
   	 g_u16TempVFreq_Win4=(BYTE)((g_u16TempVFreq_Win4+5)/10);
	if(g_u16TempVFreq_Win4==59||g_u16TempVFreq_Win4==61)
          g_u16TempVFreq_Win4=60;		

	if(abs(u16Height_Win4-1080)<8)
          u16Height_Win4=1080;
	else if(abs(u16Height_Win4-480)<8)
          u16Height_Win4=480;
	else if(abs(u16Height_Win4-576)<8)
          u16Height_Win4=576;

{
      if((abs(u16Width_Win4-720)<5))
      	{
	     u16Width_Win4=720;		
      	}
	else if((abs(u16Width_Win4-1280)<5))   
      	{
	     u16Width_Win4=1280;		
      	}
	else if((abs(u16Width_Win4-1920)<5))   
      	{
	     u16Width_Win4=1920;		
      	}
	else if((abs(u16Width_Win4-2560)<5))   
      	{
	     u16Width_Win4=2560;		
      	}
	else if((abs(u16Width_Win4-3840)<5))   
      	{
	     u16Width_Win4=3840;		
      	}
	else if((abs(u16Width_Win4-4096)<5))   
      	{
	     u16Width_Win4=4096;		
      	}

    }
	
            LoadResolutionString(MW4_PROP_RESOLUTION_INFO_START, tPropText[5], u16Width_Win4, u16Height_Win4, g_u16TempVFreq_Win4,SHOW_HZ);
        }
    }
#else
        if (INPUT_IS_VGA(MapWin2Port(DISPLAY_WIN1)))
            //LoadResolutionString(MW1_PROP_RESOLUTION_INFO_START, tPropText[2],StandardModeWidth(MapWin2Port(DISPLAY_WIN1)),StandardModeHeight(MapWin2Port(DISPLAY_WIN1)),(BYTE)((StandardModeVFreq(MapWin2Port(DISPLAY_WIN1))+5)/10),SHOW_HZ);
            LoadResolutionString(MW1_PROP_RESOLUTION_INFO_START, tPropText[2],StandardModeWidth(MapWin2Port(DISPLAY_WIN1)),StandardModeHeight(MapWin2Port(DISPLAY_WIN1)), 0,0);
        else
        {
            WORD u16Width_Win1=0,u16Height_Win1=0,g_u16TempVFreq_Win1=0;
            g_u16TempVFreq_Win1 = GetInputVfreq(DISPLAY_WIN1);
#if 1
		u16Width_Win1=InputWidth;
		u16Height_Win1=InputHeight;	
#else			
		if(FreeSyncFlag)
		{
		u16Width_Win1=InputWidth;
		u16Height_Win1=InputHeight;	
		}
		else
		{
            u16Width_Win1 = msDrvIP1ReadAutoWidth(MapPort2DetScIndex(MapWin2Port(DISPLAY_WIN1)));
            u16Height_Win1 = msDrvIP1ReadAutoHeight(MapPort2DetScIndex(MapWin2Port(DISPLAY_WIN1)));
            if (HBR2Flag(MapWin2Port(DISPLAY_WIN1)))
            	u16Width_Win1*=2;
		}
 #endif
		
    if(g_u16TempVFreq_Win1<10)
	 g_u16TempVFreq_Win1=InputVfreq;
	 g_u16TempVFreq_Win1=(BYTE)((g_u16TempVFreq_Win1+5)/10);
	if(g_u16TempVFreq_Win1==59||g_u16TempVFreq_Win1==61)
          g_u16TempVFreq_Win1=60;	


	if(abs(u16Height_Win1-1080)<8)
          u16Height_Win1=1080;
	else if(abs(u16Height_Win1-480)<8)
          u16Height_Win1=480;
	else if(abs(u16Height_Win1-576)<8)
          u16Height_Win1=576;

{
      if((abs(u16Width_Win1-720)<5))
      	{
	     u16Width_Win1=720;		
      	}
	else if((abs(u16Width_Win1-1280)<5))   
      	{
	     u16Width_Win1=1280;		
      	}
	else if((abs(u16Width_Win1-1920)<5))   
      	{
	     u16Width_Win1=1920;		
      	}
	else if((abs(u16Width_Win1-2560)<5))   
      	{
	     u16Width_Win1=2560;		
      	}
	else if((abs(u16Width_Win1-3840)<5))   
      	{
	     u16Width_Win1=3840;		
      	}
	else if((abs(u16Width_Win1-4096)<5))   
      	{
	     u16Width_Win1=4096;		
      	}

    }
	
            //LoadResolutionString(MW1_PROP_RESOLUTION_INFO_START, tPropText[2], u16Width_Win1, u16Height_Win1, g_u16TempVFreq_Win1,SHOW_HZ);
            LoadResolutionString(MW1_PROP_RESOLUTION_INFO_START, tPropText[2], u16Width_Win1, u16Height_Win1, 0,0);
        }

	
#endif
#undef SHOW_HZ
}
void LoadPropFontAndInputIconFont(void)
{
    DynamicLoadPropFont();
    //LoadMSGInputIconFont();
}

void LoadUnsupportedModeMenuFont()
{
    DynamicLoadPropFont();
    //Append the resolution string to the prop. font end of MENU_UNSUPPORTED_MODE
}
#if ENABLE_MULTI_INPUT
void LoadInputSourceMenuFont(void)
{
    DynamicLoadPropFont();
}
#endif



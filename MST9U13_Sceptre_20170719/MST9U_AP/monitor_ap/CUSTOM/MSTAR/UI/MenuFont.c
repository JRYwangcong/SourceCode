///////////////////////////////////////////////////////////////////////////////
/// @file MenuFont.c
/// @brief
/// @author MStarSemi Inc.
///
/// Functions to load Non-Prop fonts and color fonts.
///
/// Features
///  - Using CompFont.Lib
///////////////////////////////////////////////////////////////////////////////
#include "datatype.h"
#include "Board.h"
#include "ms_reg.h"
#include "Ms_rwreg.h"
#include "Global.h"
#include "menudef.h"
#include "msOSD.h"
#include "drvOSD.h"
#include "MenuFont.h"
#include "Menu.h"
#include "ColorPalette.h"

#include "Common.h"
#include "LoadPropFont1218.h"

#include "MenuFontTbl.h"

extern void mStar_LoadCompressedFont(BYTE u8Addr, const WORD *pu16FontPtr, WORD u16Count);
extern void LoadCompressColorFont(const BYTE *pu8FontTbl, WORD *pu16MapPtr, BYTE countu8Count);

#define MENUFONT_DEBUG    1
#if ENABLE_MSTV_UART_DEBUG&&MENUFONT_DEBUG
    #define MENUFONT_printData(str, value)   printData(str, value)
    #define MENUFONT_printMsg(str)           printMsg(str)
#else
    #define MENUFONT_printData(str, value)
    #define MENUFONT_printMsg(str)
#endif

#if 0  //Steven110523, for 8 color font
//8 color font raw data
void Osd_Load8ColorFont(BYTE u8Addr, const BYTE *pu8Font, WORD u16Num)
{
    DWORD xdata u32Data;
    BYTE xdata u8FontData0,u8FontData1,u8FontData2;
    BYTE xdata u8PixelBit;
    BYTE xdata i;

    OSD_SET_WRITE_FONT_MODE(WR_FONT_MODE_3BIT);
    WRITE_CAFSRAM_ADDR();
    msWrite2Byte(PORT_FONT_ADDR, GET_FONT_RAM_ADDR(u8Addr));
    WRITE_FONT();
    // handle 2 lines, 8 pixels each time, 2 lines x 12 pixels = 8 pixels x 3 times
    while(u16Num)
    {
        for (i=0;i<3;i++)
        {
        //    0        1        2        3        4        5        6        7        8
        //11110000 00000000 11111111 00002222 22222222 00000000 22221111 11111111 22222222 //line 0/1
            if (i==0)
            {
                u8FontData0=((*(pu8Font))<<4)+((*(pu8Font+1))>>4);
                u8FontData1=((*(pu8Font))&0xF0)+((*(pu8Font+2))>>4);
                u8FontData2=((*(pu8Font+3))<<4)+((*(pu8Font+4))>>4);
            }
            else if (i==1)
            {
                u8FontData0=((*(pu8Font+1))<<4)+((*(pu8Font+3))>>4);
                u8FontData1=((*(pu8Font+2))<<4)+((*(pu8Font+6))&0x0F);
                u8FontData2=((*(pu8Font+4))<<4)+((*(pu8Font+6))>>4);
            }
            else
            {
                u8FontData0=(*(pu8Font+5));
                u8FontData1=(*(pu8Font+7));
                u8FontData2=(*(pu8Font+8));
            }
            u8PixelBit=BIT7;
            u32Data=0;
            while(u8PixelBit)
            {
                u32Data<<=3;
                if (u8FontData0&u8PixelBit) //color bit0
                    u32Data|=BIT0;
                if (u8FontData1&u8PixelBit) //color bit1
                    u32Data|=BIT1;
                if (u8FontData2&u8PixelBit) //color bit2
                    u32Data|=BIT2;
                u8PixelBit>>=1;
            }
            msWriteByte(PORT_FONT_DATA, u32Data>>16);
            msWriteByte(PORT_FONT_DATA, u32Data>>8);
            msWriteByte(PORT_FONT_DATA, u32Data);
        }
        if (u16Num>=9)
        {
            u16Num-=9;
            pu8Font+=9;
        }
        else
            u16Num=0;
    }
    OSD_SET_WRITE_FONT_MODE(WR_FONT_MODE_MONO); //recover
}
#endif

///============================================================
void InitGradualColor(void)
{
    drvOSD_SetWndCtrl(OSD_MAIN_WND, OSD_WND_CTRL0,
                     (OWC0_FP_MODE|
                      OWC0_GROUP_SEL0|
                      OWC0_LV1_ALPHA_SRC(OSD_A_FIX)|
                      OWC0_LV2_ALPHA_SRC(OSD_A_FIX)|
                      OWC0_OUT_ALPHA_SRC(OSD_A_FIX)),
                      OWC0MASK_ALL);
    drvOSD_SetWndCtrl(OSD_MAIN_WND, OSD_WND_CTRL1,
                     (OWC1_1BP_44MODE_EN|
                      OWC1_44MODE_TRS_EN|
                      OWC1_FONT_HEIGHT(18)),
                      OWC1MASK_ALL);
    drvOSD_SetWndCtrl(OSD_MAIN_WND, OSD_WND_CTRL2,
                     (OWC2_GD_COLOR_R(OSD_COLOR_FIX)|
                      OWC2_GD_COLOR_G(OSD_COLOR_FIX)|
                      OWC2_GD_COLOR_B(OSD_COLOR_FIX)),
                      OWC2MASK_ALL);
    drvOSD_SetWndCtrl(OSD_MAIN_WND, OSD_WND_CTRL3,
                     (OWC3_LV1_KM_SWITCH|OWC3MASK_CKEY_INV),
                      OWC3MASK_ALL);
    drvOSD_SetWndFixAlpha(OSD_MAIN_WND,OSD_ALPHA_LV2,0x00); //PG   //(1-a)*low_lvl+a*top_lvl
    drvOSD_SetWndFixAlpha(OSD_MAIN_WND,OSD_ALPHA_LV1,0x3F); //FG
    drvOSD_SetWndFixAlpha(OSD_MAIN_WND,OSD_ALPHA_OUT,0x3F); //OSD
    drvOSD_SetWndCABaseAddr(OSD_MAIN_WND,OSD_MAIN_WND_CA_BASE);

    drvOSD_SetWndCtrl(MAIN_GD_WIN, OSD_WND_CTRL0,
                     (OWC0_FP_MODE|
                      OWC0_GROUP_SEL0|
                      OWC0_LV1_ALPHA_SRC(OSD_A_FIX)|
                      OWC0_LV2_ALPHA_SRC(OSD_A_FIX)|
                      OWC0_OUT_ALPHA_SRC(OSD_A_FIX)),
                      OWC0MASK_ALL);
    drvOSD_SetWndCtrl(MAIN_GD_WIN, OSD_WND_CTRL1,
                     (OWC1_1BP_44MODE_EN|
                      OWC1_44MODE_TRS_EN|
                      OWC1_FONT_HEIGHT(18)),
                      OWC1MASK_ALL);
    drvOSD_SetWndCtrl(MAIN_GD_WIN, OSD_WND_CTRL2,
                     (OWC2_GD_COLOR_R(OSD_GD_EG3)|
                      OWC2_GD_COLOR_G(OSD_GD_EG4)|
                      OWC2_GD_COLOR_B(OSD_GD_EG5)),
                      OWC2MASK_ALL);
    drvOSD_SetWndCtrl(MAIN_GD_WIN, OSD_WND_CTRL3,
                      (OWC3_1BPP_ATTR_BIT1|OWC3MASK_CKEY_INV),
                      OWC3MASK_ALL);

    drvOSD_SetWndFixAlpha(MAIN_GD_WIN,OSD_ALPHA_LV2,0x00); //PG   //(1-a)*low_lvl+a*top_lvl
    drvOSD_SetWndFixAlpha(MAIN_GD_WIN,OSD_ALPHA_LV1,0x00); //PG
    drvOSD_SetWndFixAlpha(MAIN_GD_WIN,OSD_ALPHA_OUT,0x3F); //OSD

}



void Osd_LoadColorFont(BYTE ColorBit ,BYTE u8Addr, const BYTE *pu8Font, WORD u16Num)
{
    DWORD xdata u32Data;
    BYTE xdata u8FontData0,u8FontData1,u8FontData2;
    BYTE xdata u8PixelBit;
    BYTE xdata i;

    OSD_SET_WRITE_FONT_MODE(ColorBit);
    WRITE_CAFSRAM_ADDR();
    msWrite2Byte(PORT_FONT_ADDR, GET_FONT_RAM_ADDR(u8Addr));
    WRITE_FONT();
    // handle 2 lines, 8 pixels each time, 2 lines x 12 pixels = 8 pixels x 3 times
    while(u16Num)
    {
        for (i=0;i<3;i++)
        {
        //    0        1        2        3        4        5        6        7        8
        //11110000 00000000 11111111 00002222 22222222 00000000 22221111 11111111 22222222 //line 0/1
            if (i==0)
            {
                u8FontData0=((*(pu8Font))<<4)+((*(pu8Font+1))>>4);
                u8FontData1=((*(pu8Font))&0xF0)+((*(pu8Font+2))>>4);
                u8FontData2=((*(pu8Font+3))<<4)+((*(pu8Font+4))>>4);
            }
            else if (i==1)
            {
                u8FontData0=((*(pu8Font+1))<<4)+((*(pu8Font+3))>>4);
                u8FontData1=((*(pu8Font+2))<<4)+((*(pu8Font+6))&0x0F);
                u8FontData2=((*(pu8Font+4))<<4)+((*(pu8Font+6))>>4);
            }
            else
            {
                u8FontData0=(*(pu8Font+5));
                u8FontData1=(*(pu8Font+7));
                u8FontData2=(*(pu8Font+8));
            }
            u8PixelBit=BIT7;
            u32Data=0;
            while(u8PixelBit)
            {
                u32Data<<=3;
                if (u8FontData0&u8PixelBit) //color bit0
                    u32Data|=BIT0;
                if (u8FontData1&u8PixelBit) //color bit1
                    u32Data|=BIT1;
                if (u8FontData2&u8PixelBit) //color bit2
                    u32Data|=BIT2;
                u8PixelBit>>=1;
            }
            msWriteByte(PORT_FONT_DATA, u32Data>>16);
            msWriteByte(PORT_FONT_DATA, u32Data>>8);
            msWriteByte(PORT_FONT_DATA, u32Data);
        }
        if (u16Num>=9)
        {
            u16Num-=9;
            pu8Font+=9;
        }
        else
            u16Num=0;
    }
    OSD_SET_WRITE_FONT_MODE(WR_FONT_MODE_MONO); //recover
}



extern void SetPropFontIndex(void);
extern void Menu_Hide(void);


void LoadMainTextFont(void)
{

 if(USER_PREF_LANGUAGE==LANG_English)
    mStar_LoadCompressedFont(MainString2Color_Start, tMainString_Eng, 0);/// Load Common font
else if(USER_PREF_LANGUAGE==LANG_Spanish)
    mStar_LoadCompressedFont(MainString2Color_Start, tMainString_Span, 0);/// Load Common font
else if(USER_PREF_LANGUAGE==LANG_French)
    mStar_LoadCompressedFont(MainString2Color_Start, tMainString_Fren, 0);/// Load Common font
else if(USER_PREF_LANGUAGE==LANG_TChinese)
    mStar_LoadCompressedFont(MainString2Color_Start, tMainString_TChinese, 0);/// Load Common font
else
    mStar_LoadCompressedFont(MainString2Color_Start, tMainString_Eng, 0);/// Load Common font


}
void LoadCommonFont(void)
{
    mStar_LoadCompressedFont(0x00, tCommonSPACE, 0);/// Load Common font
    OSD_FONT_HI_ADDR_SET_BIT8(); //enable bit 8 0x100~
    mStar_LoadCompressedFont(LOBYTE(COMMON_FONT_START), tCommonArea, 0);/// Load Common font
    OSD_FONT_HI_ADDR_CLR_TO_0();
    LoadMainTextFont();
}
void InitOsdAfterLogo(void)
{
    MENUFONT_printMsg("load osd data");
    Menu_Hide();
    InitGradualColor();
    LoadNormalCP();
    //Static Mono Font -0x00~0x100
    LoadCommonFont();
    SetPropFontIndex();
    OSD_FONT_HI_ADDR_SET_BIT9(); //enable bit 9 0x200~
    mStar_LoadCompressedFont(LOBYTE(Menu_Scetpre_Logo), tTitleSceptre_Logo, 0);/// Load Common font
    OSD_FONT_HI_ADDR_CLR_TO_0();

    OSD_FONT_HI_ADDR_SET_BIT9(); //enable bit 9 0x200~
    //Load 4 color Cursor and 8 color gauge font

//Main Mono Font count 53
    mStar_LoadCompressedFont(MM_2COLOR_CURSOR_START, tMain2ColorIcon_QuickStart, 0);/// Load Common font
    mStar_LoadCompressedFont(MM_2COLOR_Picture_Start, tMain2ColorIcon_Picture, 0);/// Load Common font
    mStar_LoadCompressedFont(MM_2COLOR_Color_Start, tMain2ColorIcon_Color, 0);/// Load Common font
    mStar_LoadCompressedFont(MM_2COLOR_System_Start, tMain2ColorIcon_System, 0);/// Load Common font
   

    OSD_WRITE_FONT_ADDRESS(MM_4COLOR_CURSOR_START);
    LoadCompressColorFont(tFrame4ColorIconFont, NULL, 10);
	
    OSD_WRITE_FONT_ADDRESS(MM_4COLOR_Icon_START);
    LoadCompressColorFont(tMainMenu4ColorIconFont, NULL, 28);
#if Enable_Corss_LHCX_New
    OSD_WRITE_FONT_ADDRESS(HotGameFONT_4COLOR_ICON_START);
    LoadCompressColorFont(tHotKeyGameClok_4COLOR_IconFont, NULL, 18);
#endif
    OSD_WRITE_FONT_ADDRESS(MM_8COLOR_ICON_START);
    Osd_LoadColorFont(WR_FONT_MODE_3BIT,MM_8COLOR_ICON_START,tMain8ColorIconFont, sizeof(tMain8ColorIconFont));

    OSD_FONT_HI_ADDR_CLR_TO_0();

    msWrite2ByteMask(OSD1_0A,0x0200+_4COLOR_FONT_START,0x03FF); //4 color start
    msWrite2ByteMask(OSD1_0C,0x0200+_8COLOR_FONT_START,0x03FF); //8 color start
    msWrite2ByteMask(OSD1_0E,0x03FF,0x03FF);                     //16 color start
    
}


#if LOGO_ENABLE


void Osd_LoadCompressColorFont( WORD addr, BYTE *fontPtr, WORD *mapPtr, BYTE count )
{
	if(( addr & 0xFF00 ) == 0x100 )
	OSD_FONT_HI_ADDR_SET_BIT8();
	else if(( addr & 0xFF00 ) == 0x200 )
	OSD_FONT_HI_ADDR_SET_BIT9();
	else if(( addr & 0xFF00 ) == 0x300 )
	OSD_FONT_HI_ADDR_SET_BIT10();
	else
	OSD_FONT_HI_ADDR_CLR_TO_0();
	OSD_WRITE_FONT_ADDRESS(LOBYTE(addr));
	LoadCompressColorFont( fontPtr, mapPtr, count );
	//OSD_FONT_CLEAR();
}


#if MSTAR_LOGO
void LoadLogoFont(void)
{
    //Steven110525
    MENUFONT_printMsg("LoadLogoIconFont");

    mStar_LoadCompressedFont(1, tMstarLogoTop1Font, 0);
    mStar_LoadCompressedFont(LOGO_2C_START+0x01, tMstarLogoDownFont, LOGO_2C_ICON_DOWN_CNT);
    OSD_FONT_HI_ADDR_SET_BIT8();
    OSD_WRITE_FONT_ADDRESS(LOGO_4C_START+2);
    LoadCompressColorFont(tMstarLogoTopFont, NULL, LOGO_4C_ICON_TOP_CNT);

    OSD_FONT_HI_ADDR_CLR_TO_0();

    //msWrite2ByteMask(OSD1_0A,0x0100,0x03FF); //4 color start
    //msWrite2ByteMask(OSD1_0C,0x03FF,0x03FF);//0x0200+_8COLOR_FONT_START); //8 color start
    //msWrite2ByteMask(OSD1_0E,0x03FF,0x03FF);//0x2C0);                     //16 color start
}

#elif LOGO_RX
void LoadLogoFont(void) 
{
	WORD addr = 0x02;
	if(( addr & 0xFF00 ) == 0x100 )
		OSD_FONT_HI_ADDR_SET_BIT8();
	else if(( addr & 0xFF00 ) == 0x200 )
		OSD_FONT_HI_ADDR_SET_BIT9();
	else if(( addr & 0xFF00 ) == 0x300 )
		OSD_FONT_HI_ADDR_SET_BIT10();
	else
		OSD_FONT_HI_ADDR_CLR_TO_0();
       OSD_WRITE_FONT_ADDRESS(LOBYTE(addr));

	mStar_LoadCompressedFont( (LOBYTE(addr)), tOSDLogoFont2Color, 0 ); // load osd fonts
	OSD_FONT_CLEAR();

//	mStar_LoadCompressedFont( MonoSpaceST, tMonoSpaceFont, 0 ); // load osd fonts

       Osd_LoadCompressColorFont( Logo4CFontStart, tOSDLogoFont, NULL, 6 );	

       msWrite2ByteMask(OSD2_22,0, BIT15);                ///2 color Select	
 
}
#elif LOGO_KOIS
void LoadLogoFont(void) 
{
	WORD addr = 0x02;
	WORD addr_2 = 0x102;
	if(( addr & 0xFF00) == 0x100 )
		OSD_FONT_HI_ADDR_SET_BIT8();
	else if(( addr & 0xFF00 ) == 0x200 )
		OSD_FONT_HI_ADDR_SET_BIT9();
	else if(( addr & 0xFF00 ) == 0x300 )
		OSD_FONT_HI_ADDR_SET_BIT10();
	else
		OSD_FONT_HI_ADDR_CLR_TO_0();
       OSD_WRITE_FONT_ADDRESS(LOBYTE(addr));

	mStar_LoadCompressedFont( (LOBYTE(addr)), tOSDLogoFont2Color, 0 ); // load osd fonts

	if(( addr_2 & 0xFF00) == 0x100 )
		OSD_FONT_HI_ADDR_SET_BIT8();
	else if(( addr_2 & 0xFF00 ) == 0x200 )
		OSD_FONT_HI_ADDR_SET_BIT9();
	else if(( addr_2 & 0xFF00 ) == 0x300 )
		OSD_FONT_HI_ADDR_SET_BIT10();
	else
		OSD_FONT_HI_ADDR_CLR_TO_0();
       OSD_WRITE_FONT_ADDRESS(LOBYTE(addr_2));

	mStar_LoadCompressedFont( (LOBYTE(addr_2)), tOSDLogoFont2Color_2, 0 ); // load osd fonts

//	mStar_LoadCompressedFont( MonoSpaceST, tMonoSpaceFont, 0 ); // load osd fonts

       Osd_LoadCompressColorFont( Logo4CFontStart, tOSDLogoFont, NULL, 6 );	
	OSD_FONT_CLEAR();
       msWrite2ByteMask(OSD2_22,0, BIT15);                ///2 color Select	
 
}

#elif LOGO_MONOPRICE||LOGO_UPSTAR||LOGO_HKC||LOGO_ZEOL||LOGO_BENZ||LOGO_ZEOL_X2||LOGO_NIXEUS||LOGO_TIEMU||LOGO_IMAGIC||LOGO_IIP||LOGO_ForGame||LOGO_GREATWALL||LOGO_SaioSone||LOGO_Sceptre||LOGO_TCL
void LoadLogoFont(void) 
{
	WORD addr = 0x02;
	if(( addr & 0xFF00 ) == 0x100 )
		OSD_FONT_HI_ADDR_SET_BIT8();
	else if(( addr & 0xFF00 ) == 0x200 )
		OSD_FONT_HI_ADDR_SET_BIT9();
	else if(( addr & 0xFF00 ) == 0x300 )
		OSD_FONT_HI_ADDR_SET_BIT10();
	else
		OSD_FONT_HI_ADDR_CLR_TO_0();
       OSD_WRITE_FONT_ADDRESS(LOBYTE(addr));

	mStar_LoadCompressedFont( (LOBYTE(addr)), tOSDLogoFont2Color, 0 ); // load osd fonts
	OSD_FONT_CLEAR();

//	mStar_LoadCompressedFont( MonoSpaceST, tMonoSpaceFont, 0 ); // load osd fonts

//       Osd_LoadCompressColorFont( Logo4CFontStart, tOSDLogoFont, NULL, 6 );	

       msWrite2ByteMask(OSD2_22,0, BIT15);                ///2 color Select	
 
}


#elif LOGO_Neview||LOGO_HDC||LOGO_JAPANNEXT||LOGO_KOGAN||LOGO_KOGAN_NEW||LOGO_X_Star||LOGO_BUBALUS||LOGO_Qnix||LOGO_AV||LOGO_AXM||LOGO_HYUNDAI||LOGO_DMM||LOGO_VMazon||LOGO_Titan||LOGO_keoushi||LOGO_ViewSync||LOGO_GameMNT
void LoadLogoFont(void) 
{
	WORD addr = 0x00;
	if(( addr & 0xFF00 ) == 0x100 )
		OSD_FONT_HI_ADDR_SET_BIT8();
	else if(( addr & 0xFF00 ) == 0x200 )
		OSD_FONT_HI_ADDR_SET_BIT9();
	else if(( addr & 0xFF00 ) == 0x300 )
		OSD_FONT_HI_ADDR_SET_BIT10();
	else
		OSD_FONT_HI_ADDR_CLR_TO_0();
       OSD_WRITE_FONT_ADDRESS(LOBYTE(addr));

	mStar_LoadCompressedFont( (LOBYTE(addr)), tOSDLogoFont2Color, 0 ); // load osd fonts

       OSD_FONT_HI_ADDR_SET_BIT8();
       addr = 0x00;
       OSD_WRITE_FONT_ADDRESS(LOBYTE(addr));
       mStar_LoadCompressedFont( (LOBYTE(addr)), tOSDLogoFont2Color_2, 0 ); // load osd fonts
	OSD_FONT_CLEAR();
//	mStar_LoadCompressedFont( MonoSpaceST, tMonoSpaceFont, 0 ); // load osd fonts

//       Osd_LoadCompressColorFont( Logo4CFontStart, tOSDLogoFont, NULL, 6 );	

       msWrite2ByteMask(OSD2_22,0, BIT15);                ///2 color Select	
 
}
#elif LOGO_TERRA||LOGO_Sceptre_New
void LoadLogoFont(void) 
{
	WORD addr = 0x01;
	if(( addr & 0xFF00 ) == 0x100 )
		OSD_FONT_HI_ADDR_SET_BIT8();
	else if(( addr & 0xFF00 ) == 0x200 )
		OSD_FONT_HI_ADDR_SET_BIT9();
	else if(( addr & 0xFF00 ) == 0x300 )
		OSD_FONT_HI_ADDR_SET_BIT10();
	else
		OSD_FONT_HI_ADDR_CLR_TO_0();
       OSD_WRITE_FONT_ADDRESS(LOBYTE(addr));

	mStar_LoadCompressedFont( (LOBYTE(addr)), tOSDLogoFont2Color, 0 ); // load osd fonts
	OSD_FONT_CLEAR();

//	mStar_LoadCompressedFont( MonoSpaceST, tMonoSpaceFont, 0 ); // load osd fonts

//       Osd_LoadCompressColorFont( Logo4CFontStart, tOSDLogoFont, NULL, 6 );	

       msWrite2ByteMask(OSD2_22,0, BIT15);                ///2 color Select	
 
}


#endif

#endif
////Icon Fonts
void LoadMenuLockIconFont(void)
{
    mStar_LoadCompressedFont(0x2A,tHotLockIcon,0);
}
#if ENABLE_AUDIO_SETTINGS_MENU
void Osd_LoadHotMuteFont( void )
{
	WORD addr =( HotKey_ICON_Addr);
	if(( addr & 0xFF00 ) == 0x100 )
		OSD_FONT_HI_ADDR_SET_BIT8();
	else if(( addr & 0xFF00 ) == 0x200 )
		OSD_FONT_HI_ADDR_SET_BIT9();
	else if(( addr & 0xFF00 ) == 0x300 )
		OSD_FONT_HI_ADDR_SET_BIT10();
	else
		OSD_FONT_HI_ADDR_CLR_TO_0();
       OSD_WRITE_FONT_ADDRESS(LOBYTE(addr));

	mStar_LoadCompressedFont( (HotKey_ICON_Addr), tHotMuteFont, 0 ); // load osd fonts
	OSD_FONT_CLEAR();

	//mStar_LoadCompressedFont( MonoSpaceST, tMonoSpaceFont, 0 ); // load osd fonts

}
#endif
void Osd_LoadHOTCROSSFont( void )
{
	WORD addr =( HotKey_ICON_Addr);
	if(( addr & 0xFF00 ) == 0x100 )
		OSD_FONT_HI_ADDR_SET_BIT8();
	else if(( addr & 0xFF00 ) == 0x200 )
		OSD_FONT_HI_ADDR_SET_BIT9();
	else if(( addr & 0xFF00 ) == 0x300 )
		OSD_FONT_HI_ADDR_SET_BIT10();
	else
		OSD_FONT_HI_ADDR_CLR_TO_0();
       OSD_WRITE_FONT_ADDRESS(LOBYTE(addr));

#if New_Cross_Enable
     if(UserPrefCrossType==Cross_Large_Y||UserPrefCrossType==Cross_Large_R)
          mStar_LoadCompressedFont(LOBYTE(HotKey_ICON_Addr), tOSDFont_Large, 0);
     else if(UserPrefCrossType==Cross_Middle_y||UserPrefCrossType==Cross_Middle_R)	 
          mStar_LoadCompressedFont(LOBYTE(HotKey_ICON_Addr), tOSDFont_Middle, 0);
     else if(UserPrefCrossType==Cross_Small_Y||UserPrefCrossType==Cross_Small_R)
          mStar_LoadCompressedFont(LOBYTE(HotKey_ICON_Addr), tOSDFont_Small, 0);
#elif Enable_Corss_FY
     if(USER_PREF_LOSMODE==Cross_Mode_1)
          mStar_LoadCompressedFont(LOBYTE(HotKey_ICON_Addr), tOSDFont_One, 0);
     else if(USER_PREF_LOSMODE==Cross_Mode_2)	 
          mStar_LoadCompressedFont(LOBYTE(HotKey_ICON_Addr), tOSDFont_Tow, 0);
     else if(USER_PREF_LOSMODE==Cross_Mode_3)
          mStar_LoadCompressedFont(LOBYTE(HotKey_ICON_Addr), tOSDFont_Tree, 0);
#elif Enable_Corss_LHCX
       if(UserPrefCrossType==Cross_Mode1_G||UserPrefCrossType==Cross_Mode1_R)
          mStar_LoadCompressedFont(LOBYTE(HotKey_ICON_Addr), tHotCross1Font, 0);
	else   
          mStar_LoadCompressedFont(LOBYTE(HotKey_ICON_Addr), tHotCross2Font, 0);
#elif Enable_Corss_RunFeng
       if(UserPrefCrossType==Cross_Mode1)
          mStar_LoadCompressedFont(LOBYTE(HotKey_ICON_Addr), tHotCrossMode1Font, 0);
	else if(UserPrefCrossType==Cross_Mode2)
          mStar_LoadCompressedFont(LOBYTE(HotKey_ICON_Addr), tHotCrossMode2Font, 0);
	else if(UserPrefCrossType==Cross_Mode3)
          mStar_LoadCompressedFont(LOBYTE(HotKey_ICON_Addr), tHotCrossMode3Font, 0);

#else
	mStar_LoadCompressedFont( LOBYTE(HotKey_ICON_Addr), tHotCrossFont, 0 ); // load osd fonts
#endif	
	OSD_FONT_CLEAR();

	//mStar_LoadCompressedFont( MonoSpaceST, tMonoSpaceFont, 0 ); // load osd fonts

}

#if 0//LED_HOT_Menu_Func
void Osd_LoadHOTLedFont( void )
{
	WORD addr =( HotKey_Led_ICON_Addr);
	if(( addr & 0xFF00 ) == 0x100 )
		OSD_FONT_HI_ADDR_SET_BIT8();
	else if(( addr & 0xFF00 ) == 0x200 )
		OSD_FONT_HI_ADDR_SET_BIT9();
	else if(( addr & 0xFF00 ) == 0x300 )
		OSD_FONT_HI_ADDR_SET_BIT10();
	else
		OSD_FONT_HI_ADDR_CLR_TO_0();
       OSD_WRITE_FONT_ADDRESS(LOBYTE(addr));

	mStar_LoadCompressedFont( (HotKey_Led_ICON_Addr&0xFF), tHotLedFont, 0 ); // load osd fonts

	OSD_FONT_CLEAR();

}
#endif

#if Enable_Corss_LHCX_New
void DynamicLoad_HotGameFont(BYTE menuPageIndex)
{
    
	OSD_FONT_HI_ADDR_SET_BIT8();

       mStar_LoadCompressedFont(LOBYTE(HotGameFrameFONT_2COLOR_ICON_START), tHotGameModeFrameIconFont, 0);/// Load Common font
	
	if(menuPageIndex==HotKeyGameModeMenu)
	{
		mStar_LoadCompressedFont(LOBYTE(HotGameFONT_2COLOR_ICON_START), tHotGameModeIconFont, 0); 
	}
	else if(menuPageIndex==HotKeyGameSelectMenu)
	{
		mStar_LoadCompressedFont(LOBYTE(HotGameFONT_2COLOR_ICON_START), tHotGameSelectIconFont, 0); 
	}
	else if(menuPageIndex==HotKeyGameSubSelectMenu)
	{
		mStar_LoadCompressedFont(LOBYTE(HotGameFONT_2COLOR_ICON_START), tHotGameSubSelectIconFont, 0); 
	}
	else if(menuPageIndex==HotKeyGameClokMenu)
	{
		mStar_LoadCompressedFont(LOBYTE(HotGameFONT_2COLOR_ICON_START), tHotGameClokIconFont, 0); 
	}
	OSD_FONT_HI_ADDR_CLR_TO_0();  
	
}
#endif


void DynamicLoadFont(BYTE menuPageIndex)
{
    
OSD_FONT_HI_ADDR_SET_BIT9(); //enable bit 9 0x200~

if(menuPageIndex==MENU_MAIN)
{
    OSD_WRITE_FONT_ADDRESS(MM_4COLOR_Icon_START);
    LoadCompressColorFont(tMainMenu4ColorIconFont, NULL, 28);

    OSD_WRITE_FONT_ADDRESS(MM_8COLOR_ICON_START);
    Osd_LoadColorFont(WR_FONT_MODE_3BIT,MM_8COLOR_ICON_START,tMain8ColorIconFont, sizeof(tMain8ColorIconFont));
}
else if(menuPageIndex==MENU_QUICKSTART||menuPageIndex==MENU_PICTURE||menuPageIndex==MENU_COLOR||menuPageIndex==MENU_SYSTEM
#if LED_HOT_Menu_Func
	||menuPageIndex==MENU_SYSTEMLLEDUSER
#endif	
	)
{

    OSD_WRITE_FONT_ADDRESS(MM_4COLOR_Icon_START);
    LoadCompressColorFont(tSubMenu4ColorIconFont, NULL, 28);

    OSD_WRITE_FONT_ADDRESS(MM_8COLOR_ICON_START);
    Osd_LoadColorFont(WR_FONT_MODE_3BIT,MM_8COLOR_ICON_START,tSub8ColorIconFont, sizeof(tSub8ColorIconFont));

}

OSD_FONT_HI_ADDR_CLR_TO_0();

OSD_FONT_HI_ADDR_SET_BIT8(); //enable bit 8 0x100~

if(menuPageIndex==MENU_INPUT)
{
  if(USER_PREF_INPUT_TYPE==INPUT_DISPLAYPORT)
    mStar_LoadCompressedFont(LOBYTE(DP_Addr), tDP, 0);
  #if ENABLE_HDMI2ND_INPUT
  else if(USER_PREF_INPUT_TYPE==INPUT_HDMI2ND)
    mStar_LoadCompressedFont(LOBYTE(HDMI2_Addr), tHDMI2, 0);
  #endif
  else if(USER_PREF_INPUT_TYPE==INPUT_HDMI)
  #if ENABLE_HDMI2ND_INPUT
    mStar_LoadCompressedFont(LOBYTE(HDMI_Addr), tHDMI1, 0);
  #else  	
    mStar_LoadCompressedFont(LOBYTE(HDMI_Addr), tHDMI, 0);
  #endif
  else
    mStar_LoadCompressedFont(LOBYTE(DVI_Addr), tDVI, 0);
  	
}
else if(menuPageIndex==MENU_HOTPICTUREMODE)
{
 if(CURRENT_PICTURE_MODE==CT_STANDARD)
 {
  if(USER_PREF_LANGUAGE==LANG_French)
    mStar_LoadCompressedFont(LOBYTE(MS_Font_Start), tStandard_Fren, 0);
  else if(USER_PREF_LANGUAGE==LANG_Spanish)
    mStar_LoadCompressedFont(LOBYTE(MS_Font_Start), tStandard_Span, 0);
  else if(USER_PREF_LANGUAGE==LANG_TChinese)
    mStar_LoadCompressedFont(LOBYTE(MS_Font_Start), tStandard_TCH, 0);
  else
    mStar_LoadCompressedFont(LOBYTE(MS_Font_Start), tStandard, 0);
 }
 else if(CURRENT_PICTURE_MODE==CT_USER)
 {
  if(USER_PREF_LANGUAGE==LANG_French)
    mStar_LoadCompressedFont(LOBYTE(MS_Font_Start), tUser_Fren, 0);
  else if(USER_PREF_LANGUAGE==LANG_Spanish)
    mStar_LoadCompressedFont(LOBYTE(MS_Font_Start), tUser_Span, 0);
  else if(USER_PREF_LANGUAGE==LANG_TChinese)
    mStar_LoadCompressedFont(LOBYTE(MS_Font_Start), tUser_TCH, 0);
  else
    mStar_LoadCompressedFont(LOBYTE(MS_Font_Start), tUser, 0);
 }
 else if(CURRENT_PICTURE_MODE==CT_MOVIE)
 {
  if(USER_PREF_LANGUAGE==LANG_French)
    mStar_LoadCompressedFont(LOBYTE(MS_Font_Start), tMovie_Fren, 0);
  else if(USER_PREF_LANGUAGE==LANG_Spanish)
    mStar_LoadCompressedFont(LOBYTE(MS_Font_Start), tMovie_Span, 0);
  else if(USER_PREF_LANGUAGE==LANG_TChinese)
    mStar_LoadCompressedFont(LOBYTE(MS_Font_Start), tMovie_TCH, 0);
  else
    mStar_LoadCompressedFont(LOBYTE(MS_Font_Start), tMovie, 0);
 }
 else if(CURRENT_PICTURE_MODE==CT_VIVID)
    mStar_LoadCompressedFont(LOBYTE(MS_Font_Start), tECO, 0);
 else if(CURRENT_PICTURE_MODE==CT_FPS)
    mStar_LoadCompressedFont(LOBYTE(MS_Font_Start), tFPS, 0);
 else// if(CURRENT_PICTURE_MODE==CT_RTS)
    mStar_LoadCompressedFont(LOBYTE(MS_Font_Start), tRTS, 0);

}
else if(menuPageIndex==MENU_HOTINPUT)
{

    mStar_LoadCompressedFont(LOBYTE(Source_Select_Addr), tSourceSelect, 0);

    mStar_LoadCompressedFont(LOBYTE(DVI_Addr), tDVI, 0);

#if ENABLE_HDMI2ND_INPUT
    mStar_LoadCompressedFont(LOBYTE(HDMI_Addr), tHDMI1, 0);
#else
    mStar_LoadCompressedFont(LOBYTE(HDMI_Addr), tHDMI, 0);
#endif

#if ENABLE_HDMI2ND_INPUT
    mStar_LoadCompressedFont(LOBYTE(HDMI2_Addr), tHDMI2, 0);
#endif

    mStar_LoadCompressedFont(LOBYTE(DP_Addr), tDP, 0);
}
else if(IS_WARNING_MENU(menuPageIndex))
{
    mStar_LoadCompressedFont(LOBYTE(MS_Font_Start+0x40), tMsgSceptreFont, 0);


}

OSD_FONT_HI_ADDR_CLR_TO_0();

}





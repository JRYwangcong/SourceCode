///////////////////////////////////////////////////////////////////////////////
/// @file msOSD.c
/// @brief
/// @author MStarSemi Inc.
///
/// Functions to draw OSD.
///
/// Features
///  -OSD registers read/write.
///  -Draw number/Hex number/text/gauge etc.
///  -
///////////////////////////////////////////////////////////////////////////////
#define _MSOSD_C

#include <stdlib.h>
#include "datatype.h"
#include "ms_reg.h"
#include "Board.h"
#include "MDebug.h"
#include "Common.h"
#include "Ms_rwreg.h"
#include "mStar.h"
#include "misc.h"
#include "Global.h"
#include "menudef.h"
#include "drvOSD.h"
#include "msOSD.h"
#if ENABLE_OSDC_OSDD
#include "drvOSDblend.h"
#endif
#include "MenuFont.h"
#include "ColorPalette.h"
#include "LoadPropFont1218.h"

//==================================================================================
// variable declaration
//==================================================================================
WORD xdata g_u8OsdWindowWidth = 10;	// maximum value may over 1 byte. 4096/12 = 381.3333
BYTE xdata g_u8OsdWindowHeight = 10;
#if 0//defined(_OSD_ROTATION_270_)
BYTE xdata g_u8OsdWindow2Height=10;
#endif
BYTE xdata g_u8OsdFontColor = 0x01;
BYTE xdata g_u8OsdWndNo=OSD_MAIN_WND;
BYTE xdata g_u8FontAddrHiBits=0;
BYTE xdata g_u8OsdFontDataHighByte=0;
//WrapLayerFlagType xdata WrapLayerFlag;

extern BYTE xdata g_u8MenuPageIndex;
BYTE xdata g_WindowEnable=0;

#define XPosGap					10

//==================================================================================
// Code start

// setup osd window size
void Osd_SetWindowSize(BYTE u8Width, BYTE u8Height/*, Bool bReset*/)
{
    g_u8OsdWindowWidth = u8Width;
    g_u8OsdWindowHeight = u8Height;

    //bReset=bReset;
    drvOSD_SetWndLineOffset(OSD_MAIN_WND,g_u8OsdWindowWidth); //for OSD_MAIN_WND only
  #if ENABLE_OSD_ROTATION //reset CABase addr after line offset was set
 #if (defined(LianHeChuangXin_Project))
    if(g_u8MenuPageIndex==MENU_KEY_GUIDE)
    {
        drvOSD_SetWndCABaseAddr(OSD_MAIN_WND, OSD_MAIN_WND_CA_BASE);
    }
   else	   
#endif
   {
  #if Enable_Panel_Rotate_180
   if (USER_PREF_OSD_ROTATION==Osd_Rotation_90)
   	 drvOSD_SetWndCABaseAddr(OSD_MAIN_WND, GET_CABASE_0_270(OSD_MAIN_WND_CA_BASE, g_u8OsdWindowWidth));
    else if(USER_PREF_OSD_ROTATION==Osd_Rotation_180)
         drvOSD_SetWndCABaseAddr(OSD_MAIN_WND, OSD_MAIN_WND_CA_BASE);
   else if(USER_PREF_OSD_ROTATION==Osd_Rotation_270)   
        drvOSD_SetWndCABaseAddr(OSD_MAIN_WND, GET_CABASE_0_90(OSD_MAIN_WND_CA_BASE, g_u8OsdWindowWidth, g_u8OsdWindowHeight));	
    else
        drvOSD_SetWndCABaseAddr(OSD_MAIN_WND, GET_CABASE_0_180(OSD_MAIN_WND_CA_BASE, g_u8OsdWindowWidth, g_u8OsdWindowHeight));
  #else
   if (USER_PREF_OSD_ROTATION==Osd_Rotation_90)
        drvOSD_SetWndCABaseAddr(OSD_MAIN_WND, GET_CABASE_0_90(OSD_MAIN_WND_CA_BASE, g_u8OsdWindowWidth, g_u8OsdWindowHeight));
    else if(USER_PREF_OSD_ROTATION==Osd_Rotation_180)
        drvOSD_SetWndCABaseAddr(OSD_MAIN_WND, GET_CABASE_0_180(OSD_MAIN_WND_CA_BASE, g_u8OsdWindowWidth, g_u8OsdWindowHeight));
    else if(USER_PREF_OSD_ROTATION==Osd_Rotation_270)
        drvOSD_SetWndCABaseAddr(OSD_MAIN_WND, GET_CABASE_0_270(OSD_MAIN_WND_CA_BASE, g_u8OsdWindowWidth));
    else
        drvOSD_SetWndCABaseAddr(OSD_MAIN_WND, OSD_MAIN_WND_CA_BASE);
   #endif	
    }
  #endif
}
void Osd_SetMultiWindowSize(BYTE u8Width, BYTE u8Height, BYTE u8WinIndex,WORD u16WinAtr)
{
    g_u8OsdWindowWidth = u8Width;
    g_u8OsdWindowHeight = u8Height;

    //bReset=bReset;
    drvOSD_SetWndLineOffset(u8WinIndex,g_u8OsdWindowWidth); //for OSD_MAIN_WND only
   #if ENABLE_OSD_ROTATION //reset CABase addr after line offset was set
 #if (defined(LianHeChuangXin_Project))
    if(g_u8MenuPageIndex==MENU_KEY_GUIDE)
    {
        drvOSD_SetWndCABaseAddr(u8WinIndex, u16WinAtr);
    }
   else	   
#endif
   {
 #if Enable_Panel_Rotate_180
    if (USER_PREF_OSD_ROTATION==Osd_Rotation_90)
        drvOSD_SetWndCABaseAddr(u8WinIndex, GET_CABASE_0_270(u16WinAtr, g_u8OsdWindowWidth));
    else if(USER_PREF_OSD_ROTATION==Osd_Rotation_180)	
        drvOSD_SetWndCABaseAddr(u8WinIndex, u16WinAtr);
    else if(USER_PREF_OSD_ROTATION==Osd_Rotation_270)
		        drvOSD_SetWndCABaseAddr(u8WinIndex, GET_CABASE_0_90(u16WinAtr, g_u8OsdWindowWidth, g_u8OsdWindowHeight));
    else
        drvOSD_SetWndCABaseAddr(u8WinIndex, GET_CABASE_0_180(u16WinAtr, g_u8OsdWindowWidth, g_u8OsdWindowHeight));
 #else
    if (USER_PREF_OSD_ROTATION==Osd_Rotation_90)
        drvOSD_SetWndCABaseAddr(u8WinIndex, GET_CABASE_0_90(u16WinAtr, g_u8OsdWindowWidth, g_u8OsdWindowHeight));
    else if(USER_PREF_OSD_ROTATION==Osd_Rotation_180)	
        drvOSD_SetWndCABaseAddr(u8WinIndex, GET_CABASE_0_180(u16WinAtr, g_u8OsdWindowWidth, g_u8OsdWindowHeight));
    else if(USER_PREF_OSD_ROTATION==Osd_Rotation_270)	
        drvOSD_SetWndCABaseAddr(u8WinIndex, GET_CABASE_0_270(u16WinAtr, g_u8OsdWindowWidth));
    else
        drvOSD_SetWndCABaseAddr(u8WinIndex, u16WinAtr);
 #endif	
   }
  #endif
 
}

// setup
void SetOsdPositionReg(WORD u16XPos, WORD u16YPos)
{
    //only for main wnd	
 #if (defined(LianHeChuangXin_Project))
    if(g_u8MenuPageIndex==MENU_KEY_GUIDE)
    {
        drvOSD_SetWndHPosition(OSD_MAIN_WND, u16XPos, u16XPos+((WORD)g_u8OsdWindowWidth*FONT_WIDTH));
        drvOSD_SetWndVPosition(OSD_MAIN_WND, u16YPos, u16YPos+((WORD)g_u8OsdWindowHeight*FONT_HEIGHT));
    }
   else	   
#endif
  {
#if Enable_Panel_Rotate_180
#if ENABLE_OSD_ROTATION
    if (USER_PREF_OSD_ROTATION==Osd_Rotation_90||USER_PREF_OSD_ROTATION==Osd_Rotation_270)
    {
        drvOSD_SetWndHPosition(OSD_MAIN_WND, u16XPos, u16XPos+((WORD)g_u8OsdWindowHeight*FONT_HEIGHT));
        drvOSD_SetWndVPosition(OSD_MAIN_WND, u16YPos, u16YPos+((WORD)g_u8OsdWindowWidth*FONT_WIDTH));
    }
    else if(USER_PREF_OSD_ROTATION==Osd_Rotation_180)	
    {
        drvOSD_SetWndHPosition(OSD_MAIN_WND, u16XPos, u16XPos+((WORD)g_u8OsdWindowWidth*FONT_WIDTH));
        drvOSD_SetWndVPosition(OSD_MAIN_WND, u16YPos, u16YPos+((WORD)g_u8OsdWindowHeight*FONT_HEIGHT));
    }
    else
#endif
    {
        drvOSD_SetWndHPosition(OSD_MAIN_WND, u16XPos, u16XPos+((WORD)g_u8OsdWindowWidth*FONT_WIDTH));
        drvOSD_SetWndVPosition(OSD_MAIN_WND, u16YPos, u16YPos+((WORD)g_u8OsdWindowHeight*FONT_HEIGHT));
    }
#else
#if ENABLE_OSD_ROTATION
    if (USER_PREF_OSD_ROTATION==Osd_Rotation_90||USER_PREF_OSD_ROTATION==Osd_Rotation_270)
    {
        drvOSD_SetWndHPosition(OSD_MAIN_WND, u16XPos, u16XPos+((WORD)g_u8OsdWindowHeight*FONT_HEIGHT));
        drvOSD_SetWndVPosition(OSD_MAIN_WND, u16YPos, u16YPos+((WORD)g_u8OsdWindowWidth*FONT_WIDTH));
    }
    else if(USER_PREF_OSD_ROTATION==Osd_Rotation_180)	
    {
        drvOSD_SetWndHPosition(OSD_MAIN_WND, u16XPos, u16XPos+((WORD)g_u8OsdWindowWidth*FONT_WIDTH));
        drvOSD_SetWndVPosition(OSD_MAIN_WND, u16YPos, u16YPos+((WORD)g_u8OsdWindowHeight*FONT_HEIGHT));
    }
    else
#endif
    {
        drvOSD_SetWndHPosition(OSD_MAIN_WND, u16XPos, u16XPos+((WORD)g_u8OsdWindowWidth*FONT_WIDTH));
        drvOSD_SetWndVPosition(OSD_MAIN_WND, u16YPos, u16YPos+((WORD)g_u8OsdWindowHeight*FONT_HEIGHT));
    }
#endif
  }
}

//Return position with pixel unit, input value is %
WORD Osd_MenuHPosition(BYTE u8XPos, Bool Set)
{
    WORD u16PosValue;
    WORD u16width = PANEL_WIDTH;
#if ENABLE_OSDC_OSDD
    #if _LIMIT_OSDSIZE_
        if(u16width > LIMIT_OSDC_HSIZE)
            u16width = LIMIT_OSDC_HSIZE;
    #endif
#else
	#if ((CHIP_ID == MST9U3) && (!ENABLE_OSD_LR)&&(!MST9U_ASIC_1P))
         u16width /=2;
	#endif
#endif


    //only for main wnd
#if ENABLE_OSD_ROTATION
    if ((USER_PREF_OSD_ROTATION==Osd_Rotation_90||USER_PREF_OSD_ROTATION==Osd_Rotation_270)
 #if (defined(LianHeChuangXin_Project))
    &&(g_u8MenuPageIndex!=MENU_KEY_GUIDE)
 #endif
    )
    {
        u16PosValue=((DWORD)u8XPos*(u16width-(WORD)g_u8OsdWindowHeight*FONT_HEIGHT)/100);
    #if ENABLE_OSD_LR_DOUBLE
        u16PosValue&=(~BIT0);
    #endif	
        if (Set)
            drvOSD_SetWndHPosition(OSD_MAIN_WND, u16PosValue, u16PosValue+((WORD)g_u8OsdWindowHeight*FONT_HEIGHT));
    }
    else
#endif
    {
        u16PosValue=((DWORD)u8XPos*(u16width-(WORD)g_u8OsdWindowWidth*FONT_WIDTH)/100);
    #if ENABLE_OSD_LR_DOUBLE
        u16PosValue&=(~BIT0);
    #endif	
        if (Set)
            drvOSD_SetWndHPosition(OSD_MAIN_WND, u16PosValue, u16PosValue+((WORD)g_u8OsdWindowWidth*FONT_WIDTH));
    }
    return u16PosValue;
}
//Return position with pixel unit, input value is %
WORD Osd_MenuVPosition(BYTE u8YPos, Bool Set)
{
    WORD u16PosValue;
    WORD u16hiegh = PANEL_HEIGHT;
#if ENABLE_OSDC_OSDD
#if _LIMIT_OSDSIZE_
    if(u16hiegh > LIMIT_OSDC_VSIZE)
        u16hiegh = LIMIT_OSDC_VSIZE;
#endif
#endif


    //only for main wnd
#if ENABLE_OSD_ROTATION
    if ((USER_PREF_OSD_ROTATION==Osd_Rotation_90||USER_PREF_OSD_ROTATION==Osd_Rotation_270)
 #if (defined(LianHeChuangXin_Project))
    &&(g_u8MenuPageIndex!=MENU_KEY_GUIDE)
 #endif
    )
    {
        u16PosValue=((DWORD)u8YPos*(u16hiegh-(WORD)g_u8OsdWindowWidth*FONT_WIDTH)/100);
    #if ENABLE_OSD_LR_DOUBLE
        u16PosValue&=(~BIT0);
    #endif	
        if (Set)
            drvOSD_SetWndVPosition(OSD_MAIN_WND, u16PosValue, u16PosValue+((WORD)g_u8OsdWindowWidth*FONT_WIDTH));
    }
    else
#endif
    {
        u16PosValue=((DWORD)u8YPos*(u16hiegh-(WORD)g_u8OsdWindowHeight*FONT_HEIGHT)/100);
    #if ENABLE_OSD_LR_DOUBLE
        u16PosValue&=(~BIT0);
    #endif	
        if (Set)
            drvOSD_SetWndVPosition(OSD_MAIN_WND, u16PosValue, u16PosValue+((WORD)g_u8OsdWindowHeight*FONT_HEIGHT));
    }
    return u16PosValue;
}
void Osd_SetPosition(BYTE u8XPos, BYTE u8YPos)
{
    #if defined(KTC_Project)
       Osd_MenuHPosition(100-u8XPos,TRUE);
	Osd_MenuVPosition(100-u8YPos,TRUE);
    #elif (Enable_Panel_Rotate_180)
       Osd_MenuHPosition(100-u8XPos,TRUE);
       Osd_MenuVPosition(u8YPos,TRUE);
    #else
       Osd_MenuHPosition(u8XPos,TRUE);
       Osd_MenuVPosition(100-u8YPos,TRUE);
    #endif
}




WORD Osd_MultiMenuHPosition(BYTE u8XPos,BYTE u8WinIndex, Bool Set)
{
    WORD u16PosValue;
    WORD u16width = PANEL_WIDTH;

#if ENABLE_OSDC_OSDD
#if _LIMIT_OSDSIZE_
    if(u16width > LIMIT_OSDC_HSIZE)
        u16width = LIMIT_OSDC_HSIZE;
#endif
#else
	#if ((CHIP_ID == MST9U3) && (!ENABLE_OSD_LR)&&(!MST9U_ASIC_1P))
          u16width /=2;
	#endif
#endif
    //only for main wnd
#if ENABLE_OSD_ROTATION
    if ((USER_PREF_OSD_ROTATION==Osd_Rotation_90||USER_PREF_OSD_ROTATION==Osd_Rotation_270)
 #if (defined(LianHeChuangXin_Project))
    &&(g_u8MenuPageIndex!=MENU_KEY_GUIDE)
 #endif
      )
    {
        u16PosValue=((DWORD)u8XPos*(u16width-(XPosGap*2)-(WORD)g_u8OsdWindowHeight*FONT_HEIGHT)/100)+XPosGap;
    #if ENABLE_OSD_LR_DOUBLE
        u16PosValue&=(~BIT0);
    #endif	
        if (Set)
            drvOSD_SetWndHPosition(u8WinIndex, u16PosValue, u16PosValue+((WORD)g_u8OsdWindowHeight*FONT_HEIGHT));
    }
    else
#endif
    {
        u16PosValue=((DWORD)u8XPos*(u16width-(XPosGap*2)-(WORD)g_u8OsdWindowWidth*FONT_WIDTH)/100)+XPosGap;
    #if ENABLE_OSD_LR_DOUBLE
        u16PosValue&=(~BIT0);
    #endif	
        if (Set)
        {       
            	drvOSD_SetWndHPosition(u8WinIndex, u16PosValue, u16PosValue+((WORD)g_u8OsdWindowWidth*FONT_WIDTH));
        }		
    }
    return u16PosValue;

}
//Return position with pixel unit, input value is %
WORD Osd_MultiMenuVPosition(BYTE u8YPos,BYTE u8WinIndex, Bool Set)
{
    WORD u16PosValue;
    WORD u16hiegh = PANEL_HEIGHT;
#if ENABLE_OSDC_OSDD
#if _LIMIT_OSDSIZE_
    if(u16hiegh > LIMIT_OSDC_VSIZE)
        u16hiegh = LIMIT_OSDC_VSIZE;
#endif
#endif
    //only for main wnd
#if ENABLE_OSD_ROTATION
    if ((USER_PREF_OSD_ROTATION==Osd_Rotation_90||USER_PREF_OSD_ROTATION==Osd_Rotation_270)
 #if (defined(LianHeChuangXin_Project))
    &&(g_u8MenuPageIndex!=MENU_KEY_GUIDE)
 #endif
      )
    {
        u16PosValue=((DWORD)u8YPos*(u16hiegh-(WORD)g_u8OsdWindowWidth*FONT_WIDTH)/100);
    #if ENABLE_OSD_LR_DOUBLE
        u16PosValue&=(~BIT0);
    #endif	
        if (Set)
            drvOSD_SetWndVPosition(u8WinIndex, u16PosValue, u16PosValue+((WORD)g_u8OsdWindowWidth*FONT_WIDTH));
    }
    else
#endif
    {
        u16PosValue=((DWORD)u8YPos*(u16hiegh-(WORD)g_u8OsdWindowHeight*FONT_HEIGHT)/100);
    #if ENABLE_OSD_LR_DOUBLE
        u16PosValue&=(~BIT0);
    #endif	
        if (Set)
        {
                drvOSD_SetWndVPosition(u8WinIndex, u16PosValue, u16PosValue+((WORD)g_u8OsdWindowHeight*FONT_HEIGHT));
        }
    }
    return u16PosValue;
}

void Osd_SetMultiPosition(BYTE u8XPos, BYTE u8YPos,BYTE u8WinIndex)
{
	
	//u8YPos = 100 - u8YPos;
	//msDrvOSDForceWRBank(_ENABLE , FORCE_BANK_DELAY); //double buffer to avoid garbage
	Osd_MultiMenuHPosition(u8XPos,u8WinIndex,TRUE);
	Osd_MultiMenuVPosition(u8YPos,u8WinIndex,TRUE);
	
	//msDrvOSDForceWRBank(_DISABLE , FORCE_BANK_DELAY); //double buffer to avoid garbage
}

void Osd_DrawRealChar(BYTE u8XPos, BYTE u8YPos, WORD u8Char)
{
    u8Char=0xFF&u8Char;
    drvOSD_DrawRealChar(g_u8OsdWndNo, u8XPos, u8YPos, u8Char);
}
void Osd_DrawContinuousChar(BYTE u8XPos, BYTE u8YPos, BYTE u8Char, BYTE u8Num)
{
    drvOSD_DrawContinuousChar(g_u8OsdWndNo,u8XPos,u8YPos,u8Char,u8Num);
}


#define NUM2ASCII(Num) ((Num)+FONT_FACTORY_NUMBER_START)//(Num+0x30)
void DrawNum(BYTE u8XPos, BYTE u8YPos, char len, int s16Value)
{
    char _minus=0;

    if (s16Value<0)
    {
        s16Value=0-s16Value;
        _minus=1;
    }
    if (s16Value==0)
        Osd_DrawRealChar(u8XPos+(len--), u8YPos, (BYTE)NUM2ASCII(0));
    else
    {
        while (s16Value && len) // translate integer to string
        {
            Osd_DrawRealChar(u8XPos+(len--), u8YPos, (BYTE)NUM2ASCII(s16Value%10));
            s16Value/=10;
        }
    }
    if (_minus && len>=0)// add sign
        Osd_DrawRealChar(u8XPos+(len--), u8YPos, 0x0A);//(BYTE)'-');

    _minus=(_minus)?(0):(1);

    for (; _minus<=len; _minus++)
        Osd_DrawRealChar(u8XPos+_minus, u8YPos, 0x00);// space
}

void DrawNumUnsigned(BYTE u8XPos, BYTE u8YPos, char len, WORD u16Value)
{
    BYTE i;
    if (u16Value==0)
        Osd_DrawRealChar(u8XPos+(len--), u8YPos, (BYTE)NUM2ASCII(0));
    else
    {
        while (u16Value && len) // translate integer to string
        {
            Osd_DrawRealChar(u8XPos+(len--), u8YPos, (BYTE)NUM2ASCII(u16Value%10));
            u16Value/=10;
        }
    }
    for (i=1; i<=len; i++)
        Osd_DrawRealChar(u8XPos+i, u8YPos, 0x00);// space
}

void Osd_DrawNum(BYTE u8XPos, BYTE u8YPos, int s16Value)
{
    DrawNum(u8XPos, u8YPos, 3, s16Value);
}
#if 1
void Osd_Draw5Num(BYTE u8XPos, BYTE u8YPos, WORD u16Value)
{
    DrawNumUnsigned(u8XPos, u8YPos, 5, u16Value);
}
#endif

//===================================================================================
void Osd_SetWindowAtr(BYTE u8Win_Num,BYTE u8HStart,BYTE u8HEnd ,BYTE u8VStart, BYTE u8VEnd,BYTE u8Color)
{
    //drvOSD_SetWndOn(u8Win_Num, FALSE); //Jison 110922
#if ENABLE_OSD_ROTATION
    if (USER_PREF_OSD_ROTATION==Osd_Rotation_90)
    {
        WORD xdata u16MainHStart=drvOSD_GetWndInfo(OSD_MAIN_WND,OSD_WND_INFO_H_START);
        WORD xdata u16MainVEnd=drvOSD_GetWndInfo(OSD_MAIN_WND,OSD_WND_INFO_V_END);
        drvOSD_SetWndCABaseAddr(u8Win_Num, (u8HEnd-u8HStart+OSD_MAIN_WND_CA_BASE)+ //(u8HEnd-u8HStart+1-1+OSD_MAIN_WND_CA_BASE)
            (WORD)u8VStart*drvOSD_GetWndInfo(OSD_MAIN_WND,OSD_WND_INFO_LINE_OFFSET)+u8HStart);
        drvOSD_SetWndHPosition(u8Win_Num, u16MainHStart+(WORD)u8VStart*FONT_HEIGHT, u16MainHStart+((WORD)u8VEnd+1)*FONT_HEIGHT);
        drvOSD_SetWndVPosition(u8Win_Num, u16MainVEnd-((WORD)u8HEnd+1)*FONT_WIDTH, u16MainVEnd-(WORD)u8HStart*FONT_WIDTH);
      #ifndef _MSTAR_OSD_
        drvOSD_SetTGenHPosition(OSD_TG0, u16MainHStart+(WORD)u8VStart*FONT_HEIGHT, u16MainHStart+((WORD)u8VEnd+1)*FONT_HEIGHT);
        drvOSD_SetTGenVPosition(OSD_TG0, u16MainVEnd-((WORD)u8HEnd+1)*FONT_WIDTH, u16MainVEnd-(WORD)u8HStart*FONT_WIDTH);
        drvOSD_SetTGenHPosition(OSD_TG1, u16MainHStart+(WORD)u8VStart*FONT_HEIGHT+9, u16MainHStart+((WORD)u8VEnd+1)*FONT_HEIGHT);
        drvOSD_SetTGenVPosition(OSD_TG1, u16MainVEnd-((WORD)u8HEnd+1)*FONT_WIDTH, u16MainVEnd-(WORD)u8HStart*FONT_WIDTH);
      #endif
    }
    else
#endif
    {
        WORD xdata u16MainHStart=drvOSD_GetWndInfo(OSD_MAIN_WND,OSD_WND_INFO_H_START);
        WORD xdata u16MainVStart=drvOSD_GetWndInfo(OSD_MAIN_WND,OSD_WND_INFO_V_START);
        drvOSD_SetWndCABaseAddr(u8Win_Num, drvOSD_GetWndInfo(OSD_MAIN_WND,OSD_WND_INFO_CABASE)+
            (WORD)u8VStart*drvOSD_GetWndInfo(OSD_MAIN_WND,OSD_WND_INFO_LINE_OFFSET)+u8HStart);
        drvOSD_SetWndHPosition(u8Win_Num, u16MainHStart+(WORD)u8HStart*FONT_WIDTH, u16MainHStart+((WORD)u8HEnd+1)*FONT_WIDTH);
        drvOSD_SetWndVPosition(u8Win_Num, u16MainVStart+(WORD)u8VStart*FONT_HEIGHT, u16MainVStart+((WORD)u8VEnd+1)*FONT_HEIGHT);
      #ifndef _MSTAR_OSD_
        drvOSD_SetTGenHPosition(OSD_TG0, u16MainHStart+(WORD)u8HStart*FONT_WIDTH, u16MainHStart+((WORD)u8HEnd+1)*FONT_WIDTH);
        drvOSD_SetTGenVPosition(OSD_TG0, u16MainVStart+(WORD)u8VStart*FONT_HEIGHT, u16MainVStart+((WORD)u8VEnd+1)*FONT_HEIGHT);
        drvOSD_SetTGenHPosition(OSD_TG1, u16MainHStart+(WORD)u8HStart*FONT_WIDTH, u16MainHStart+((WORD)u8HEnd+1)*FONT_WIDTH);
        drvOSD_SetTGenVPosition(OSD_TG1, u16MainVStart+(WORD)u8VStart*FONT_HEIGHT+9, u16MainVStart+((WORD)u8VEnd+1)*FONT_HEIGHT);
      #endif
    }
  #if 1
    u8Color=MONO_COLOR(CP_MainGround, CP_Black); // Jison CP_MASK_COLOR to see PG
    drvOSD_SetWndFixAttr(u8Win_Num,_ENABLE,u8Color);
  #else
    u8Color=u8Color;
  #endif

  #ifndef _MSTAR_OSD_
    if (msReadByte((OSD_MAIN_WND<<5)+OSD2_00)&BIT0) // check if main wnd on
    {
        WAIT_V_OUTPUT_BLANKING_START(); //Jison wait 1 frame to avoid the last text garbage
        drvOSD_SetWndOn(u8Win_Num, TRUE);
        drvOSD_SetTGenOn(OSD_TG0,TRUE);
        drvOSD_SetTGenOn(OSD_TG1,TRUE);
    }
    else
        WrapLayerFlag.bSync2Main=1;
  #endif
}
void Osd_SetWindowEnable(BYTE u8Win_Num,Bool u8Enable)
{
    drvOSD_SetWndOn(u8Win_Num, u8Enable);
}

void Osd_SetTransparency(BYTE u8Value)
{
#if 0 //use OSD alpha blending
    msWriteByte(SCL0_10_46, u8Value);
#else //use fix Alpha
    u8Value=0x3F-(u8Value*8);
    drvOSD_SetWndFixAlpha(OSD_MAIN_WND, OSD_ALPHA_OUT, u8Value);
    drvOSD_SetWndFixAlpha(OSD_BUTTON_WND, OSD_ALPHA_OUT, u8Value);
    drvOSD_SetWndFixAlpha(OSD_CURSOR_WND, OSD_ALPHA_OUT, u8Value);
#endif
}
///==============================

void Osd_DrawBlankPlane(BYTE u8XPos,BYTE u8YPos,BYTE u8Width,BYTE u8Height)
{
    drvOSD_DrawBlankPlane(g_u8OsdWndNo,u8XPos,u8YPos,u8Width,u8Height);
}
void Osd_DrawHex(BYTE u8XPos, BYTE u8YPos, BYTE u8Value)
{
#define HEX2ASCII(u8Num) ((u8Num)+FONT_FACTORY_NUMBER_START)
    Osd_DrawRealChar(u8XPos+1, u8YPos, HEX2ASCII(u8Value&0x0F));
    Osd_DrawRealChar(u8XPos, u8YPos, HEX2ASCII(u8Value>>4));
#undef HEX2ASCII
}

BYTE Osd_DoubleBuffer(Bool u8Enable)
{
    u8Enable=0;
    return u8Enable;
}



void Osd_EnableSubWin( BYTE u8Win_Num )
{
//printData("Osd_EnableSubWin=%d",u8Win_Num);
	if (u8Win_Num == 0)
	{
		u8Win_Num = OSD_CURSOR_WND;
		g_WindowEnable|=BIT0;
	}
	else if (u8Win_Num == 1)
	{
		u8Win_Num = OSD_BUTTON_WND;
		g_WindowEnable|=BIT1;
	}
	else if (u8Win_Num == 2)
	{
		u8Win_Num = OSD_ICON_WND;
		g_WindowEnable|=BIT2;
	}
	else if (u8Win_Num == 3)
	{
		u8Win_Num = OSD_SUB_WND;
		g_WindowEnable|=BIT3;		
	}
	else if (u8Win_Num == 4)
	{
		u8Win_Num = OSD_SUB2_WND;
		g_WindowEnable|=BIT4;		
	}	

	if(g_WindowEnable&BIT7)
	{
        //mStar_WaitForDataBlank_OSD();
        WAIT_V_OUTPUT_BLANKING();
		drvOSD_SetWndOn(u8Win_Num, TRUE);
	}
}

void Osd_DisableSubWin( BYTE u8Win_Num )
{
	if (u8Win_Num == 0)
	{
		u8Win_Num = OSD_CURSOR_WND;
		g_WindowEnable&=~BIT0;
	}
	else if (u8Win_Num == 1)
	{
		u8Win_Num = OSD_BUTTON_WND;
		g_WindowEnable&=~BIT1;
	}
	else if (u8Win_Num == 2)
	{
		u8Win_Num = OSD_ICON_WND;
		g_WindowEnable&=~BIT2;		
	}
	else if (u8Win_Num == 3)
	{
		u8Win_Num = OSD_SUB_WND;
		g_WindowEnable&=~BIT3;
	}
	else if (u8Win_Num == 4)
	{
		u8Win_Num = OSD_SUB2_WND;
		g_WindowEnable&=~BIT4;
	}	
	//Delay1ms(4);
    //mStar_WaitForDataBlank_OSD();
       //WAIT_V_OUTPUT_BLANKING();
	drvOSD_SetWndOn(u8Win_Num, FALSE);
}






///////////////////////////////////////////////////////////////////////////////
/// @file msOSDDrawText.c
/// @brief Menu string functions.
/// @author MStarSemi Inc.
///
/// Functions to deal with prop. strings.
///
/// Features
///  -Multi-Language support.
///  -
///////////////////////////////////////////////////////////////////////////////
#define _MSOSDDRAWTEXT_C
#include "datatype.h"
#include "Board.h"
#include "Global.h"
#include "Menu.h"
#include "msOSD.h"
#include "ms_reg.h"
#include "Ms_rwreg.h"
#include "msOSDDrawText.h"

//#define SPACE_FONT 0x00

extern BYTE g_u8MenuPageIndex;
extern BYTE g_u8PrevMenuPageIndex;
extern BYTE g_u8MenuItemIndex;
MenuFlagType  g_stMenuFlag;

/*********************************************************************************************/
/// Draw proportional string.
///@param u8XPos \b IN: X coordinate.
///@param u8YPos \b IN: Bit 0~6=Y coordinate. Bit 7=0 means change attribute only.
///@param pu8Str \n IN: Two bytes prop. string array.
///
/*********************************************************************************************/
void DrawPropStr(BYTE u8XPos, BYTE u8YPos, BYTE *pu8Str)
{
    WORD  xdata  u16TempValue;
    Bool  xdata  IsDrawCode = TRUE;

    if (pu8Str==NULL || (*(pu8Str + 1))==0) //Jison
        return;
    if (u8YPos & BIT7)
        IsDrawCode = FALSE;
/*
    if( u8YPos == 2 || u8XPos == 0 )
    {
        u8XPos=( g_u8OsdWindowWidth - (*(pu8Str + 1)) ) / 2 + 1;
    }
*/
    u8YPos &= 0x7F; //~(BIT7|BIT6);
#define DISP_CHAR   u8XPos
#define DISP_PTR    u8YPos

/*
  if(( g_wOSDFontOffset & 0xFF00 ) == 0x100 )
     OSD_TEXT_HI_ADDR_SET_BIT8();
  else if(( g_wOSDFontOffset & 0xFF00 ) == 0x200 )
     OSD_TEXT_HI_ADDR_SET_BIT9();
  else if(( g_wOSDFontOffset & 0xFF00 ) == 0x300 )
     OSD_TEXT_HI_ADDR_SET_BIT10();
  else
     OSD_TEXT_HI_ADDR_CLR_TO_0();
*/	

    //u16TempValue=g_u8OsdWndNo<<5;
    //u16TempValue=msRead2Byte(u16TempValue+OSD2_10)+(WORD)u8YPos*msReadByte(u16TempValue+OSD2_12)+u8XPos; // get real address
    u16TempValue=GET_TXT_CASTART(g_u8OsdWndNo,u8XPos,u8YPos); // get real address
    DISP_PTR = 0;
    WRITE_CAFSRAM_ADDR();
    msWrite2Byte(PORT_ATTR_ADDR, u16TempValue);
    WRITE_ATTRIBUTE();
    while (DISP_PTR++ < *(pu8Str + 1))
        msWriteByte(PORT_ATTR_DATA, g_u8OsdFontColor);

    if (IsDrawCode == TRUE)
    {
        DISP_PTR = 0;
        WRITE_CAFSRAM_ADDR();
        msWrite2Byte(PORT_CODE_ADDR, u16TempValue);
        WRITE_CODE();
        while (DISP_PTR < *(pu8Str + 1))
        {
            msWriteByte(PORT_CODE_DATA, (*pu8Str) + DISP_PTR);
            DISP_PTR++;
        }
    }

#undef DISP_CHAR
#undef DISP_PTR

	//OSD_FONT_CLEAR();

}

void DrawCenterText(BYTE u8XPos, BYTE u8YPos, BYTE *pu8Str)
{
    BYTE u8Len=0;
    BYTE XPos_Shift=0;


    if (pu8Str==NULL ||*(pu8Str + 1)==0)
        return;

    u8Len = *(pu8Str + 1);

	extern BYTE g_u8MenuPageIndex;

	
	if(IS_MAIN_L1(g_u8MenuPageIndex))
	{
	  
		 XPos_Shift=((u8Len-3+1)/2);
		
		u8XPos=u8XPos-XPos_Shift;
	}
	else
	{
              u8Len = *(pu8Str + 1);
              
              XPos_Shift=((g_u8OsdWindowWidth-u8Len)/2);
              
              u8XPos=XPos_Shift+1;
	}


    DrawPropStr(u8XPos, u8YPos, pu8Str);
}


void DrawCenterDrawArrowText(BYTE u8XPos, BYTE u8YPos, BYTE *pu8Str)
{
    BYTE i, u8Len=0;
    BYTE XPos_Shift=0;
    BYTE g_u8OsdFontDataHighByte_BK=g_u8OsdFontDataHighByte;	


    if (pu8Str==NULL ||*(pu8Str + 1)==0)
        return;

    u8Len = *(pu8Str + 1);

	extern BYTE g_u8MenuPageIndex;

	
	if(IS_MAIN_L1(g_u8MenuPageIndex))
	{
		XPos_Shift=((u8Len-3+1)/2);
		
		u8XPos=u8XPos-XPos_Shift;
	}
	else
	{
              u8Len = *(pu8Str + 1);
              
              XPos_Shift=((g_u8OsdWindowWidth-u8Len)/2);
              
              u8XPos=XPos_Shift+1;
	}

if(g_stMenuFlag.bThisItemSelected==1)
{
   //printData("-------------- u8Len==%d",u8Len);
   OSD_TEXT_HI_ADDR_CLR_TO_0();
//   for (i = 0; i < 9; i++)
//      Osd_DrawRealChar(21+i, u8YPos, SPACE_FONT);
   for (i = 0; i < 8; i++)
      Osd_DrawRealChar(22+i, u8YPos, SPACE_FONT);
    g_u8OsdFontDataHighByte=g_u8OsdFontDataHighByte_BK; 	
}
	
    DrawPropStr(u8XPos, u8YPos, pu8Str);

//Draw Arrow
    if((g_u8MenuPageIndex==MENU_COLOR)&&(g_stMenuFlag.bEnableItemSelect==1))
		;
    else if((IS_MAIN_L1(g_u8MenuPageIndex))&&(g_u8MenuItemIndex%2==1)&&(g_stMenuFlag.bThisItemSelected==0))
      {
               OSD_TEXT_HI_ADDR_SET_BIT8(); //enable bit 8
//               Osd_DrawRealChar(20, u8YPos, ARROW_LEFT); 
//               Osd_DrawRealChar(30, u8YPos, ARROW_RIGHT); 
               Osd_DrawRealChar(21, u8YPos, ARROW_LEFT); 
               Osd_DrawRealChar(30, u8YPos, ARROW_RIGHT); 
               OSD_TEXT_HI_ADDR_CLR_TO_0();
      }

}


void DrawFullCenterText(BYTE u8XPos, BYTE u8YPos, BYTE *pu8Str)
{
    BYTE u8Len;

    if (pu8Str==NULL ||*(pu8Str + 1)==0)
        return;

    if (u8XPos == 0)
    {
        u8Len = *(pu8Str + 1);
        u8XPos = (g_u8OsdWindowWidth - u8Len + 1) / 2;
        //for (i = 0; i < u8XPos; i++)
        //    Osd_DrawRealChar(i, u8YPos, SPACE_FONT);
        //for (i = u8XPos + u8Len; i < g_u8OsdWindowWidth - 2; i++)
         //   Osd_DrawRealChar(i, u8YPos, SPACE_FONT);
    }

    DrawPropStr(u8XPos, u8YPos, pu8Str);
}
void DrawRightAlignText(BYTE u8XPos, BYTE u8YPos, BYTE *pu8Str)
{
    BYTE    u8Len;

    if (pu8Str==NULL ||*(pu8Str + 1)==0)
        return;

    u8Len = *(pu8Str + 1);
    u8Len--;
    if ((u8Len)<u8XPos)
        u8XPos-= (u8Len);
    else
        u8XPos=0;

    DrawPropStr(u8XPos, u8YPos, pu8Str);
}


void  DrawRealStr(BYTE u8XPos, BYTE u8YPos, BYTE *pu8Str)
{
	WORD xdata u16TempValue;
	BYTE xdata u8DrawCode = (!(u8YPos & BIT7));

	u8YPos &= 0x7F;

	u16TempValue=GET_TXT_CASTART(g_u8OsdWndNo,u8XPos,u8YPos); // get real address
	
#define DISP_CHAR    u8XPos
#define DISP_PTR     u8YPos
	DISP_PTR = 0;
        WRITE_CAFSRAM_ADDR();
	msWrite2Byte(PORT_ATTR_ADDR, u16TempValue); // display font attribute
        WRITE_ATTRIBUTE();
	while( *( pu8Str + DISP_PTR ) ) 	   // display string font index
	{
		msWriteByte(PORT_ATTR_DATA, g_u8OsdFontColor);
		DISP_PTR++;
	}
	if( u8DrawCode )
	{
		DISP_PTR = 0;
              WRITE_CAFSRAM_ADDR();
		msWrite2Byte(PORT_CODE_ADDR, u16TempValue); // dispaly font code
              WRITE_CODE();
		while( (DISP_CHAR = *( pu8Str + DISP_PTR )) )			  // display string font index
		{
			msWriteByte(PORT_CODE_DATA, DISP_CHAR);
			DISP_PTR++;
		}
	}
#undef DISP_CHAR
#undef DISP_PTR
}



void DrawArrowCenterText(BYTE u8XPos, BYTE u8YPos, BYTE *pu8Str)
{
    BYTE i, u8Len;

    if (pu8Str==NULL ||*(pu8Str + 1)==0)
        return;

        u8Len = *(pu8Str + 1);
#if 0//LED_HOT_Menu_Func    
   if((USER_PREF_LANGUAGE==LANG_SChinese)&&(u8Len==2))
        u8XPos = (LedColorTextXPos+3)+((15 - u8Len + 1) / 2)-1;
   else
        u8XPos = (LedColorTextXPos+3)+((15 - u8Len + 1) / 2);

   if((g_u8MenuPageIndex==MENU_HOTLEDMODE)&&(g_u8MenuItemIndex==1)||(g_u8MenuItemIndex==3)||(g_u8MenuItemIndex==5))
   {
        if(g_u8MenuItemIndex==1)
        {
        for (i = 0; i < 10; i++)
           Osd_DrawRealChar(((LedColorTextXPos+5) +i),(LedColorTextYPos+LedColorTextShiftXPos*HotLedColorIDX), SPACE_FONT);
        }
        else if(g_u8MenuItemIndex==3)
        {
        for (i = 0; i < 10; i++)
           Osd_DrawRealChar(((LedColorTextXPos+5)+i),(LedColorTextYPos+LedColorTextShiftXPos*HotLedStrengthIDX), SPACE_FONT);
        }
        else if(g_u8MenuItemIndex==5)
        {
        for (i = 0; i < 10; i++)
           Osd_DrawRealChar(((LedColorTextXPos+5)+i),(LedColorTextYPos+LedColorTextShiftXPos*HotLedColorModeIDX), SPACE_FONT);
        }
   
   }
#endif


    DrawPropStr(u8XPos, u8YPos, pu8Str);
}



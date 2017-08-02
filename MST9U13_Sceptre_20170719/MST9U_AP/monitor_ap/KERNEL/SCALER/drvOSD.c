
#define _DRVOSD_C

#include "datatype.h"
#include "Board.h"
#include "ms_reg.h"
#include "drvOSD.h"
#include "Ms_rwreg.h"
#include "msOSD.h"
#include "halRwreg.h"
#include "drvmsOVD.h"
#include "LoadPropFont1218.h"
#if CHIP_ID==MST9U3
#include "msEread.h"
#endif
#include "Utl.h"

#define DRVOSD_DEBUG    0
#if ENABLE_MSTV_UART_DEBUG && DRVOSD_DEBUG
#define DRVOSD_PRINT(format, ...)     printf(format, ##__VA_ARGS__)
#else
#define DRVOSD_PRINT(format, ...)
#endif

extern xdata BYTE g_u8OsdFontColor;

BYTE xdata g_u8OSDGuardBand=0;
Bool xdata g_bInitedOSD=FALSE;

void drvOSD_Init(void)
{
    if (!g_bInitedOSD)
    {
    #if CHIP_ID==MST9U3
        WORD u8Temp;
        msEread_Init(&u8Temp, &g_u8OSDGuardBand);
    #endif

        g_bInitedOSD = TRUE; //initial only once
    }
}

#if ENABLE_OSD_ROTATION
WORD drvOSD_GetTxtCAStart(BYTE u8Wnd, BYTE u8XPos, BYTE u8YPos)
{
    BYTE u8LineOffset=msReadByte((u8Wnd<<5)+OSD2_12);
    WORD u16CABase=msRead2Byte((u8Wnd<<5)+OSD2_10);
    BYTE xdata u8Bank=msReadByte(OSD1_00)&0x03;

	// Exchange 90 degree with 270 degree menu rotation to make the menu rotate 90 degree clockwise. 
	// The rotation action of SRAM OSD register is counterclockwise, therefore we have to set the register to 270 degree.
    if ((msReadByte(OSD1_03)>>6)==3)
    {
    	BYTE u8WndHeight;
    #if CHIP_ID==MST9U3 && ENABLE_OSD_LR
        u8WndHeight = (drvOSD_GetWndInfo(u8Wnd, OSD_WND_INFO_H_END)-
            drvOSD_GetWndInfo(u8Wnd, OSD_WND_INFO_H_START))/
            drvOSD_GetWndInfo(u8Wnd, OSD_WND_INFO_H_DUPE_RATIO)/FONT_HEIGHT;
   	#else
		u8Wnd<<=5;
        u8WndHeight = (msRead2Byte((u8Wnd)+OSD2_08)- msRead2Byte((u8Wnd)+OSD2_04))/FONT_HEIGHT;
   	#endif
        u16CABase=GET_CABASE_90_0(u16CABase,u8LineOffset,u8WndHeight);
    }
  #if defined(_OSD_ROTATION_180_)
    if ((msReadByte(OSD1_03)>>6)==2) //for 180 degree only
    {
        BYTE u8WndHeight;
        
    #if CHIP_ID==MST9U3 && ENABLE_OSD_LR
        u8WndHeight = ((msRead2Byte((u8Wnd<<5)+OSD2_0A)&0x3FFF)-
            msRead2Byte((u8Wnd<<5)+OSD2_06))/
            drvOSD_GetWndInfo(u8Wnd, OSD_WND_INFO_V_DUPE_RATIO)/FONT_HEIGHT;
    #else
        u8Wnd<<=5;
        u8WndHeight = (msRead2Byte((u8Wnd)+OSD2_0A)- msRead2Byte((u8Wnd)+OSD2_06))/FONT_HEIGHT;
    #endif
        u16CABase=GET_CABASE_180_0(u16CABase,u8LineOffset,u8WndHeight);
    }
  #endif
  #if defined(_OSD_ROTATION_270_)
    if ((msReadByte(OSD1_03)>>6)==1) //for 270 degree only
    {
        u16CABase=GET_CABASE_270_0(u16CABase,u8LineOffset);
    }
  #endif

    msWriteByteMask(OSD1_00,u8Bank, 0x03);

    return (u16CABase+(WORD)u8YPos*u8LineOffset+u8XPos);
}
#endif

void drvOSD_DrawRealChar(BYTE u8Wnd, BYTE u8XPos, BYTE u8YPos, BYTE u8Char)
{
    WORD u16TempValue;
    BYTE u8DrawCode = (!(u8YPos & BIT7));

    u8YPos &= 0x7F;

    //u8Wnd<<=5;
    //u16TempValue=msRead2Byte((u8Wnd)+OSD2_10)+(WORD)u8YPos*msReadByte((u8Wnd)+OSD2_12)+u8XPos; // get real address
    u16TempValue=GET_TXT_CASTART(u8Wnd,u8XPos,u8YPos); // get real address
    WRITE_CAFSRAM_ADDR();
    msWrite2Byte(PORT_ATTR_ADDR, u16TempValue); // display font attribute
    WRITE_ATTRIBUTE();
    msWriteByte(PORT_ATTR_DATA, g_u8OsdFontColor);
    if (u8DrawCode)
    {
        WRITE_CAFSRAM_ADDR();
        msWrite2Byte(PORT_CODE_ADDR, u16TempValue); // dispaly font code
        WRITE_CODE();
        msWriteByte(PORT_CODE_DATA, u8Char);
    }

}
void drvOSD_DrawContinuousChar(BYTE u8Wnd,BYTE u8XPos, BYTE u8YPos, BYTE u8Char, BYTE u8Num)
{
    WORD u16TempValue;
    BYTE i;
    BYTE u8DrawCode = (!(u8YPos & BIT7));

    u8YPos &= 0x7F;

    //u8Wnd<<=5;
    //u16TempValue=msRead2Byte((u8Wnd)+OSD2_10)+(WORD)u8YPos*msReadByte((u8Wnd)+OSD2_12)+u8XPos; // get real address
    u16TempValue=GET_TXT_CASTART(u8Wnd,u8XPos,u8YPos); // get real address
    WRITE_CAFSRAM_ADDR();
    msWrite2Byte(PORT_ATTR_ADDR, u16TempValue);
    WRITE_ATTRIBUTE();
    for(i=0;i<u8Num;i++)
    {
        msWriteByte(PORT_ATTR_DATA, g_u8OsdFontColor);
    }
    if (u8DrawCode)
    {
        WRITE_CAFSRAM_ADDR();
        msWrite2Byte(PORT_CODE_ADDR, u16TempValue);
        WRITE_CODE();
        for(i=0;i<u8Num;i++)
        {
            msWriteByte(PORT_CODE_DATA, u8Char);
        }
    }
}
#if 0
void drvOSD_DrawVContinuousChar(BYTE u8Wnd, BYTE u8XPos, BYTE u8YPos, BYTE u8Char, BYTE u8Num)
{
    WORD u16TempValue;
    BYTE i;
    BYTE u8DrawCode = (!(u8YPos & BIT7));

    u8YPos &= 0x7F;

    //u8Wnd<<=5;
    //u16TempValue=msRead2Byte((u8Wnd)+OSD2_10)+(WORD)u8YPos*msReadByte((u8Wnd)+OSD2_12)+u8XPos; // get real address
    u16TempValue=GET_TXT_CASTART(u8Wnd,u8XPos,u8YPos); // get real address
    #define LINE_OFFSET u8Wnd
    LINE_OFFSET=msReadByte((u8Wnd<<5)+OSD2_12);
    for (i=0;i<u8Num;i++)
    {
        WRITE_CAFSRAM_ADDR();
        msWrite2Byte(PORT_ATTR_ADDR, u16TempValue);
        WRITE_ATTRIBUTE();
        msWriteByte(PORT_ATTR_DATA, g_u8OsdFontColor);
        u16TempValue+=LINE_OFFSET;
    }
    if (u8DrawCode)
    {
        for(i=0;i<u8Num;i++)
        {
            u16TempValue-=LINE_OFFSET;
            WRITE_CAFSRAM_ADDR();
            msWrite2Byte(PORT_CODE_ADDR, u16TempValue);
            WRITE_CODE();
            msWriteByte(PORT_CODE_DATA, u8Char);
        }
    }
    #undef LINE_OFFSET
}
#endif
void drvOSD_DrawBlankPlane(BYTE u8Wnd,BYTE u8XPos,BYTE u8YPos,BYTE u8Width,BYTE u8Height)
{
    BYTE i,j;
    WORD u16TempValue;
    BYTE IsDrawCode=(!(u8YPos & BIT7));

    u8YPos&=0x7F;
    //u8Wnd<<=5;
    //u16TempValue=msRead2Byte((u8Wnd)+OSD2_10)+(WORD)u8YPos*msReadByte((u8Wnd)+OSD2_12)+u8XPos; // get real address
    u16TempValue=GET_TXT_CASTART(u8Wnd,u8XPos,u8YPos); // get real address
    #define LINE_OFFSET u8Wnd
    LINE_OFFSET=msReadByte((u8Wnd<<5)+OSD2_12);
    for (i = 0; i < u8Height; i++)
    {
        WRITE_CAFSRAM_ADDR();
        msWrite2Byte(PORT_ATTR_ADDR, u16TempValue);
        WRITE_ATTRIBUTE();
        for(j=0;j<u8Width;j++)
        {
            msWriteByte(PORT_ATTR_DATA, g_u8OsdFontColor);
        }
        u16TempValue+=LINE_OFFSET;
    }
    if (!IsDrawCode)
        return;
    u16TempValue-=(LINE_OFFSET*u8Height); // recover real address
    for (i = 0; i < u8Height; i++)
    {
        WRITE_CAFSRAM_ADDR();
        msWrite2Byte(PORT_CODE_ADDR, u16TempValue);
        WRITE_CODE();
        for(j=0;j<u8Width;j++)
        {
            msWriteByte(PORT_CODE_DATA, SPACE_FONT);
        }
        u16TempValue+=LINE_OFFSET;
    }
    #undef LINE_OFFSET

}

void drvOSD_SetWndLineOffset(BYTE u8Wnd, BYTE u8LineOffset)
{
    msWriteByte((u8Wnd<<5)+OSD2_12,u8LineOffset);
}
void drvOSD_SetWndHPosition(BYTE u8Wnd, WORD u16HStart, WORD u16HEnd)
{
    u8Wnd<<=5;

    #if ((CHIP_ID == MST9U3) && (ENABLE_OSD_LR))

    OSD_COMMON_WRITE_ONOFF(FALSE);

    if ((u16HStart < PANEL_WIDTH/2) && (u16HEnd > PANEL_WIDTH/2))        // this window is across the middle line
    {
        WORD u16CABaseAddr = msRead2Byte((u8Wnd)+OSD2_10);
        WORD u16Tmp = PANEL_WIDTH/2 - u16HStart;
        WORD u16HStartRight = 0;
        WORD u16HEndLeft = PANEL_WIDTH/2;
        BYTE u8ShiftPixel;
        BYTE u8DupeRatio = drvOSD_GetWndInfo(u8Wnd>>5, OSD_WND_INFO_H_DUPE_RATIO);

        u16HEnd -= PANEL_WIDTH/2;

        if (g_u8OSDGuardBand)
        {
            u16HEndLeft += MIN(OD_GUARD_BAND,u16HEnd);
            u16HEnd += OD_GUARD_BAND;
            u16HStartRight = ((OD_GUARD_BAND<u16Tmp)? (0+OD_GUARD_BAND%u8DupeRatio): (OD_GUARD_BAND-u16Tmp));
            u16Tmp -= MIN(OD_GUARD_BAND, u16Tmp);	
			u16Tmp += OD_GUARD_BAND%u8DupeRatio;	// for pixel alignment in duplicate ratio cases
        }

        switch (msReadByte(OSD1_03)>>6)
        {
        default:
        case 0: //normal
            u16CABaseAddr += (u16Tmp)/(12*u8DupeRatio);
            u8ShiftPixel = (u16Tmp)%(12*u8DupeRatio)/u8DupeRatio;
            break;
        case 1: //rotation of 90 degree
            u16CABaseAddr += (u16Tmp)/(18*u8DupeRatio)*msReadByte((u8Wnd)+OSD2_12);
            u8ShiftPixel = (u16Tmp)%(18*u8DupeRatio)/u8DupeRatio;
            break;
        case 2: //rotation of 180 degree
            u16CABaseAddr -= (u16Tmp)/(12*u8DupeRatio);
            u8ShiftPixel = (u16Tmp)%(12*u8DupeRatio)/u8DupeRatio;
            break;
        case 3: //rotation of 270 degree
            u16CABaseAddr -= (u16Tmp)/(18*u8DupeRatio)*msReadByte((u8Wnd)+0x00101B12);
            u8ShiftPixel = (u16Tmp)%(18*u8DupeRatio)/u8DupeRatio;
            break;
        }

        WaitVOutputBlanklingStart();

        // write OSD_L window star and end position
        msWrite2Byte((u8Wnd)+OSD2_04,u16HStart);
        msWrite2ByteMask((u8Wnd)+OSD2_08, u16HEndLeft, 0x3FFF);
        // write OSD_R window star and end position
        msWrite2Byte((u8Wnd)+0x101B04, u16HStartRight);
        msWrite2ByteMask((u8Wnd)+0x101B08, u16HEnd, 0x3FFF);

        msWrite2Byte((u8Wnd)+0x00101B10, u16CABaseAddr); // update OSD_R C/A base address
        msWriteByteMask((u8Wnd)+0x00101B0E, u8ShiftPixel, 0x1F); // update OSD_R x pixel shift
    }
    else if ((u16HStart >= PANEL_WIDTH/2) && (u16HEnd > PANEL_WIDTH/2))    // this window is on R scaler
    {
        WORD u16CABaseAddr = msRead2Byte((u8Wnd)+OSD2_10);
        BYTE u8ShiftPixel = msRead2Byte((u8Wnd)+OSD2_0E);

        if (g_u8OSDGuardBand)
        {
            u16HStart += OD_GUARD_BAND;
            u16HEnd += OD_GUARD_BAND;
        }
        u16HStart -= PANEL_WIDTH/2;
        u16HEnd -= PANEL_WIDTH/2;

        WaitVOutputBlanklingStart();

        // disable L part window by setting its size to zero
        msWrite2Byte((u8Wnd)+OSD2_04,0);
        msWrite2ByteMask((u8Wnd)+OSD2_08,0,0x3FFF);
        // write OSD_R window star and end position
        msWrite2Byte((u8Wnd)+0x101B04, u16HStart);
        msWrite2ByteMask((u8Wnd)+0x101B08, u16HEnd, 0x3FFF);

        msWrite2Byte((u8Wnd)+0x00101B10, u16CABaseAddr); // recover OSD_R C/A base address
        msWriteByteMask((u8Wnd)+0x00101B0E, u8ShiftPixel, 0x1F); // sync OSD_R x pixel shift
    }
    else if ((u16HStart < PANEL_WIDTH/2) && (u16HEnd <= PANEL_WIDTH/2))    // this window is on L scaler
    {
        WaitVOutputBlanklingStart();

        // disable R part window by setting its size to zero
        msWrite2Byte((u8Wnd)+0x101B04,0);
        msWrite2ByteMask((u8Wnd)+0x101B08,0,0x3FFF);
        // write OSD_L window star and end position
        msWrite2Byte((u8Wnd)+OSD2_04,u16HStart);
        msWrite2ByteMask((u8Wnd)+OSD2_08,u16HEnd,0x3FFF);
    }
    else
    {
        // oops!
    }

    OSD_COMMON_WRITE_ONOFF(TRUE);

    #else
	WaitVOutputBlanklingStart();
    msWrite2Byte((u8Wnd)+OSD2_04,u16HStart);
    msWrite2ByteMask((u8Wnd)+OSD2_08,u16HEnd,0x3FFF);
    #endif
}
void drvOSD_SetWndVPosition(BYTE u8Wnd, WORD u16VStart, WORD u16VEnd)
{
    u8Wnd<<=5;
    WaitVOutputBlanklingStart();
    msWrite2Byte((u8Wnd)+OSD2_06,u16VStart);
    msWrite2ByteMask((u8Wnd)+OSD2_0A,u16VEnd,0x3FFF);
}
WORD drvOSD_GetWndInfo(BYTE u8Wnd,BYTE u8Info)
{
#if CHIP_ID==MST9U3 && ENABLE_OSD_LR
    WORD u16Temp;
#endif
    u8Wnd<<=5;

    switch (u8Info)
    {
#if CHIP_ID==MST9U3 && ENABLE_OSD_LR
    case OSD_WND_INFO_H_START:
        if (g_u8OSDGuardBand)
        {
            return (msRead2Byte((u8Wnd)+OSD2_08)&0x3FFF)?
                   (msRead2Byte((u8Wnd)+OSD2_04)):
                   (msRead2Byte((u8Wnd)+0x101B04)+PANEL_WIDTH/2-OD_GUARD_BAND);
        }
        else
        {
            return (msRead2Byte((u8Wnd)+OSD2_08)&0x3FFF)?
                   (msRead2Byte((u8Wnd)+OSD2_04)):
                   (msRead2Byte((u8Wnd)+0x101B04)+PANEL_WIDTH/2);
        }
    case OSD_WND_INFO_H_END:
        if (g_u8OSDGuardBand)
        {
            return (u16Temp=(msRead2Byte((u8Wnd)+0x101B08)&0x3FFF))?
                   (u16Temp+PANEL_WIDTH/2-OD_GUARD_BAND):
                   (msRead2Byte((u8Wnd)+OSD2_08)&0x3FFF);
        }
        else
        {
            return (u16Temp=(msRead2Byte((u8Wnd)+0x101B08)&0x3FFF))?
                   (u16Temp+PANEL_WIDTH/2):
                   (msRead2Byte((u8Wnd)+OSD2_08)&0x3FFF);
        }
#else
    case OSD_WND_INFO_H_START:
        return msRead2Byte((u8Wnd)+OSD2_04);
    case OSD_WND_INFO_H_END:
        return (msRead2Byte((u8Wnd)+OSD2_08)&0x3FFF);
#endif
    case OSD_WND_INFO_V_START:
        return msRead2Byte((u8Wnd)+OSD2_06);
    case OSD_WND_INFO_V_END:
        return (msRead2Byte((u8Wnd)+OSD2_0A)&0x3FFF);
    case OSD_WND_INFO_LINE_OFFSET:
        return msReadByte((u8Wnd)+OSD2_12);
    case OSD_WND_INFO_CABASE:
        return msRead2Byte((u8Wnd)+OSD2_10);
    case OSD_WND_INFO_H_DUPE_RATIO:
        return (msReadByte((u8Wnd)+OSD2_09)>>6)+1;
    case OSD_WND_INFO_V_DUPE_RATIO:
        return (msReadByte((u8Wnd)+OSD2_0B)>>6)+1;
    }

    return 0;
}
void drvOSD_SetWndOn(BYTE u8Wnd,Bool u8On)
{
    msWriteByteMask((u8Wnd<<5)+OSD2_00,(u8On?BIT0:0),BIT0);
}
void drvOSD_SetWndCtrl(BYTE u8Wnd, BYTE u8CtrlType, WORD u16Value,WORD u16Mask)
{
    u8Wnd<<=5;
    if (u8CtrlType==OSD_WND_CTRL0)
        msWrite2ByteMask((u8Wnd)+OSD2_00,u16Value,u16Mask);
    else if (u8CtrlType==OSD_WND_CTRL1)
        msWrite2ByteMask((u8Wnd)+OSD2_02,u16Value,u16Mask);
    else if (u8CtrlType==OSD_WND_CTRL2)
        msWrite2ByteMask((u8Wnd)+OSD2_14,u16Value,u16Mask);
    else if (u8CtrlType==OSD_WND_CTRL3)
        msWrite2ByteMask((u8Wnd)+OSD2_1A,u16Value,u16Mask);
}
void drvOSD_SetWndFixAlpha(BYTE u8Wnd,BYTE u8Lvl,BYTE u8Value)
{
    u8Wnd<<=5;
    //inverse color key define
    msWriteByteMask((u8Wnd)+OSD2_15,BIT5,BIT5);
    //fix_alpha_out, fix_alpha_lv1, fix_alpha_lv2
    if (u8Lvl==OSD_ALPHA_OUT)
        msWriteByteMask((u8Wnd)+OSD2_0C,u8Value,0x3F);
    else if (u8Lvl==OSD_ALPHA_LV1)
        msWriteByteMask((u8Wnd)+OSD2_0D,u8Value,0x3F);
    else if (u8Lvl==OSD_ALPHA_LV2)
        msWriteByteMask((u8Wnd)+OSD2_19,u8Value,0x3F);
}
void drvOSD_SetWndFixColor(BYTE u8Wnd,BYTE u8Red,BYTE u8Green,BYTE u8Blue)
{
    u8Wnd<<=5;
    msWriteByte((u8Wnd)+OSD2_16,u8Red);
    msWriteByte((u8Wnd)+OSD2_17,u8Green);
    msWriteByte((u8Wnd)+OSD2_18,u8Blue);
}
void drvOSD_SetWndShiftPixel(BYTE u8Wnd,BYTE u8X,BYTE u8Y)
{
    u8Wnd<<=5;
    msWriteByte((u8Wnd)+OSD2_0E,u8X);
    msWriteByte((u8Wnd)+OSD2_0F,u8Y);
}
void drvOSD_SetWndCABaseAddr(BYTE u8Wnd,WORD u16Addr)
{
    msWrite2Byte((u8Wnd<<5)+OSD2_10,u16Addr);
}
void drvOSD_SetWndFixAttr(BYTE u8Wnd,BYTE u8Enable,BYTE u8Attr)
{
    u8Wnd<<=5;
    msWriteByte((u8Wnd)+OSD2_1A,u8Attr);
    msWriteByteMask((u8Wnd)+OSD2_15,(u8Enable?BIT7:0),BIT7);
}
void drvOSD_SetGDEngineEnableWnd(BYTE u8GDEngine, WORD u16EnableFlags,WORD u16Mask)
{
    BYTE xdata u8Bank = msReadByte(OSD1_00)&0x03;
    WORD xdata u16Addr;
    msWriteByteMask(OSD1_00,0x01,0x03);//subbank 1
    if (u8GDEngine<OSD_GD_EG4)
        u16Addr=OSD1_44+u8GDEngine;
    else
        u16Addr=OSD1_EC+(u8GDEngine-OSD_GD_EG4);// gd4/gd5 //110627
    if (u16EnableFlags&0xFF) //for wnd
    {
        msWriteByteMask(u16Addr,(u16EnableFlags&0xFF),(u16Mask&0xFF));
    }
    u16EnableFlags&=0x0F00;
    u16Mask&=0x0F00;
    if (u16Mask) //for timing gen
    {
        if (u8GDEngine&0x01) //GD1/GD3/GD5
        {
            u16EnableFlags>>=4;
            u16Mask>>=4;
        }
        else //GD0/GD2/GD4
        {
            u16EnableFlags>>=8;
            u16Mask>>=8;
        }
        msWriteByteMask(OSD1_EE+(u8GDEngine>>1),u16EnableFlags,u16Mask); //110627
    }

    msWriteByteMask(OSD1_00,u8Bank,0x03);
}

/*
 * Gradient Window (TGen) related functions
 */
void drvOSD_SetTGenHPosition(BYTE u8Tgen, WORD u16HStart, WORD u16HEnd)
{
    BYTE xdata u8Bank = msReadByte(OSD1_00)&0x03;
	
    u8Tgen<<=3;
	
    if(u16HStart > PANEL_WIDTH)
   		u16HStart = PANEL_WIDTH;
    if(u16HEnd > PANEL_WIDTH)
        u16HEnd = PANEL_WIDTH;

#if CHIP_ID==MST9U3 && ENABLE_OSD_LR
    OSD_COMMON_WRITE_ONOFF(FALSE);
    msWriteByteMask(OSD1_00,0x01,0x03);     // 0x101C subbank 1
    msWriteByteMask(0x101A00,0x01,0x03);    // 0x101A subbank 1

    if (g_u8OSDGuardBand)
    {
        msWrite2Byte((u8Tgen)+OSD1_04,((u16HStart<((PANEL_WIDTH/2)+OD_GUARD_BAND))?u16HStart:0));
        msWrite2Byte((u8Tgen)+OSD1_06,(u16HEnd>=((PANEL_WIDTH/2)+OD_GUARD_BAND)?((u16HStart<((PANEL_WIDTH/2)+OD_GUARD_BAND))?((PANEL_WIDTH/2)+OD_GUARD_BAND):0):u16HEnd));
        msWrite2Byte((u8Tgen)+0x101A04,(u16HStart>=(PANEL_WIDTH/2))?(u16HStart+OD_GUARD_BAND-(PANEL_WIDTH/2)):0);
        msWrite2Byte((u8Tgen)+0x101A06,(u16HEnd>=(PANEL_WIDTH/2))?(u16HEnd+OD_GUARD_BAND-(PANEL_WIDTH/2)):0);
    }
    else
    {
        msWrite2Byte((u8Tgen)+OSD1_04,((u16HStart<PANEL_WIDTH/2)?u16HStart:0));
        msWrite2Byte((u8Tgen)+OSD1_06,(u16HEnd>=(PANEL_WIDTH/2)?((u16HStart<(PANEL_WIDTH/2))?(PANEL_WIDTH/2):0):u16HEnd));
        msWrite2Byte((u8Tgen)+0x101A04,(u16HStart>=PANEL_WIDTH/2)?(u16HStart-PANEL_WIDTH/2):0);
        msWrite2Byte((u8Tgen)+0x101A06,(u16HEnd>=PANEL_WIDTH/2)?(u16HEnd-PANEL_WIDTH/2):0);
    }
	
    msWriteByteMask(0x101A00,u8Bank,0x03);
    msWriteByteMask(OSD1_00,u8Bank,0x03);
	OSD_COMMON_WRITE_ONOFF(TRUE);
#else
    msWriteByteMask(OSD1_00,0x01,0x03);				//subbank 1
    msWrite2Byte((u8Tgen)+OSD1_04,u16HStart);
    msWrite2Byte((u8Tgen)+OSD1_06,u16HEnd);
    msWriteByteMask(OSD1_00,u8Bank,0x03);
#endif
}
void drvOSD_SetTGenVPosition(BYTE u8Tgen, WORD u16VStart, WORD u16VEnd)
{
    BYTE xdata u8Bank = msReadByte(OSD1_00)&0x03;
	
    u8Tgen<<=3;
    msWriteByteMask(OSD1_00,0x01,0x03);				//subbank 1
    msWrite2Byte((u8Tgen)+OSD1_08,u16VStart);
    msWrite2Byte((u8Tgen)+OSD1_0A,u16VEnd);
    msWriteByteMask(OSD1_00,u8Bank,0x03);
}
void drvOSD_SetTGenFixColor(BYTE u8Tgen, BYTE u8RColor, BYTE u8GColor, BYTE u8BColor)
{	
#define TGEN_COLOR_R    ((OSD_COLOR_FIX)<<4)		// GD6 for fix color
#define TGEN_COLOR_G    ((OSD_COLOR_FIX)<<4)
#define TGEN_COLOR_B    (OSD_COLOR_FIX)
	
    BYTE xdata u8Bank = msReadByte(OSD1_00)&0x03;
	u8Tgen<<=3;
    msWriteByteMask(OSD1_00,0x00,0x03);				//subbank 0
    msWriteByteMask((u8Tgen)+OSD1_20,TGEN_COLOR_R,0x70);
    msWriteByte((u8Tgen)+OSD1_21,(TGEN_COLOR_G|TGEN_COLOR_B));
    msWriteByte((u8Tgen)+OSD1_22,u8RColor);
    msWriteByte((u8Tgen)+OSD1_23,u8GColor);
    msWriteByte((u8Tgen)+OSD1_24,u8BColor);	
    msWriteByteMask(OSD1_00,u8Bank,0x03);
	
#undef TGEN_COLOR_R
#undef TGEN_COLOR_G
#undef TGEN_COLOR_B
}
void drvOSD_SetCurveClass(BYTE u8CurveIndex, const CurveClass* pstHCurve, const CurveClass* pstVCurve)
{	
    BYTE u8Bank = msReadByte(OSD1_00)&0x03;
	
    if (!pstHCurve || !pstVCurve)
        return;

    msWriteByteMask(OSD1_00,0x00,0x03);						//subbank 0

    msWriteByteMask(OSD1_68, 0x1E, 0x1F);					//select curve sram address Port    
    u8CurveIndex *= 2;		
    msWriteByte(OSD1_66, u8CurveIndex); 					//horizontal settings

    msWriteByteMask(OSD1_68, 0x1F, 0x1F);					//select curve sram data Port  
    msWriteByte(OSD1_66, pstHCurve->u8PixelInit);			//PixelInit: inital value of the color (R or G or B),
    msWriteByte(OSD1_66, pstHCurve->u8DeltaInit);			//DeltaInit: linear function delta
    msWriteByte(OSD1_66, pstHCurve->u8Step);				//Step: pixels per step (actual value is setting value + 1)
    msWriteByte(OSD1_66, (pstHCurve->bDataSign<<7)|			//DataSign: (0)increase DeltaInit, (1)decrease DeltaInit per step
                         (pstHCurve->bDeltaSign<<6)|		//DeltaSign: (0)increase Delta on DeltaInit, (1)decrease Delta on DeltaInit per step
                         (pstHCurve->u8Delta));				//Delta: quadratic function delta
    msWriteByte(OSD1_66, pstHCurve->u16Length0&0xFF);       //Length0[7:0]: # pixels of Length0 in curve mode
    msWriteByte(OSD1_66, ((pstHCurve->u16Length1<<4)|       //Length1[3:0]: # pixels of Length1 in curve mode 
                         (pstHCurve->u16Length0>>8))&0xFF); //Length0[10:8]
    msWriteByte(OSD1_66, (pstHCurve->bOffsetSign<<7)|       //OffsetSign (hv sign): data = (0)h + v, (1)h - v 
                         (pstHCurve->u16Length1>>4));       //Length1[10:4]
    msWriteByte(OSD1_66, (pstHCurve->bOverflowEnable<<7)|	//OverflowEnable: when color value is over 255, (0)keep the value 255, (1)circulate the value
                         (pstHCurve->bDotEnable<<6)|		//DotEnable: enable dot pattern
                         (pstHCurve->u8CurveMode));			//CurveMode: curve mode

    msWriteByteMask(OSD1_68, 0x1E, 0x1F);
    msWriteByte(OSD1_66, u8CurveIndex+1); 					//vertical settings

    msWriteByteMask(OSD1_68, 0x1F, 0x1F);
    msWriteByte(OSD1_66, pstVCurve->u8PixelInit);
    msWriteByte(OSD1_66, pstVCurve->u8DeltaInit);
    msWriteByte(OSD1_66, pstVCurve->u8Step);
    msWriteByte(OSD1_66, (pstVCurve->bDataSign<<7)|
                         (pstVCurve->bDeltaSign<<6)|
                         (pstVCurve->u8Delta));
    msWriteByte(OSD1_66, pstVCurve->u16Length0&0xFF);       //vLength0[7:0]
    msWriteByte(OSD1_66, ((pstVCurve->u16Length1<<4)|       //vLength1[3:0]
                         (pstVCurve->u16Length0>>8))&0xFF); //vLength0[10:8]
    msWriteByte(OSD1_66, (pstVCurve->u16Length1)>>4);       //vLength1[10:4]
    msWriteByte(OSD1_66, (pstVCurve->bOverflowEnable<<7)|
                         (pstVCurve->bDotEnable<<6)|
                         (pstVCurve->u8CurveMode));
	
    msWriteByteMask(OSD1_00,u8Bank,0x03);
}
void drvOSD_SetTgenCurve(BYTE u8Tgen, BYTE u8ARGB, BYTE u8Engine, BYTE u8CurveIndex, const CurveClass* pstHCurve, const CurveClass* pstVCurve)
{
    #define SET_TGEN_GD_ENGINE(tgen, gd, value) ((void)(\
        msWriteByteMask(OSD1_00, 0x01, 0x03),\
        msWrite2ByteMask(OSD1_28+6*(gd), ((WORD)(value))<<(tgen), ((WORD)0x0F)<<(tgen))))

    Bool bSetCurve = FALSE;
	BYTE u8Bank = msReadByte(OSD1_00)&0x03;
	
    if (u8Engine<6)									//must use GD0~GD5
    {    	
        bSetCurve = TRUE;		
        msWriteByteMask(OSD1_00, 0x00, 0x03);		//subbank0
        switch (u8ARGB)
        {
        	case OSD_PG_RED:				
				msWriteByteMask(OSD1_20+(u8Tgen<<3), u8Engine<<4, 0x70);
				break;
			case OSD_PG_GREEN:				
				msWriteByteMask(OSD1_21+(u8Tgen<<3), u8Engine<<4, 0xF0);
				break;
			case OSD_PG_BLUE:
				msWriteByteMask(OSD1_21+(u8Tgen<<3), u8Engine, 0x0F);
				break;
			default:
				break;
        }	
        SET_TGEN_GD_ENGINE(u8Tgen<<2, u8Engine, u8CurveIndex);
    }
		
    if (bSetCurve)
    {	   
    #if (CHIP_ID==MST9U3 && ENABLE_OSD_LR)
		WORD u16HStartLeft, u16HEndLeft, u16HStartRight, u16HEndRight;
	
        u8Tgen<<=3;
		u16HStartLeft = msRead2Byte((u8Tgen) + OSD1_04);
      	u16HEndLeft = msRead2Byte((u8Tgen) + OSD1_06);
        u16HStartRight = msRead2Byte((u8Tgen) + 0x101A04);
        u16HEndRight = msRead2Byte((u8Tgen) + 0x101A06);

		DRVOSD_PRINT("u16HStartLeft = %d\n", u16HStartLeft);
		DRVOSD_PRINT("u16HEndLeft = %d\n", u16HEndLeft);		
		DRVOSD_PRINT("u16HStartRight = %d\n", u16HStartRight);		
		DRVOSD_PRINT("u16HEndRight = %d\n", u16HEndRight);
		
        if ((u16HEndLeft != 0) && (u16HEndRight != 0))	// cross middle line
        {
        	WORD u16LeftRange = 0;
            CurveClass stHCurveR;
            WORD u16Offset = 0, u16NumOfStep = 0;			
			SINT sTemp = 0;
        
			memcpy(&stHCurveR, pstHCurve, sizeof(CurveClass));
			
			// u16LeftRange is the length from left H start to middle line
			u16LeftRange = u16HEndLeft - u16HStartLeft - OD_GUARD_BAND;
		   	DRVOSD_PRINT("u16LeftRange = [%d]\n", u16LeftRange);

			if (u16LeftRange <= OD_GUARD_BAND)
			{				
				stHCurveR.u16Length0 = OD_GUARD_BAND - u16LeftRange;
			}
			else
			{				
				// (u16LeftRange - OD_GUARD_BAND) is the length from left H start to right H start (position 0 of right OSD)
				u16Offset = (u16LeftRange - OD_GUARD_BAND) % (pstHCurve->u8Step + 1);

				// The step is not alighed to the position 0 of right OSD.
				// It may be problematic if one step across the middle line and 
				// the step is larger than OD_GUARD_BAND or there is no OD_GUARD_BAND.
				// The only solution is used another higher priority TGen to cover this area.
				if (u16Offset != 0)	
				{
					stHCurveR.u16Length0 = (pstHCurve->u8Step + 1) - u16Offset;
					u16NumOfStep = (u16LeftRange - OD_GUARD_BAND) / (pstHCurve->u8Step + 1) + 1;
					
				}
				else		// The step is alighed to the position 0 of right OSD.
				{
					stHCurveR.u16Length0 = 0;
					u16NumOfStep = (u16LeftRange - OD_GUARD_BAND) / (pstHCurve->u8Step + 1);
				}

				// Use arithmetic series to calculate the PixelInit of right curve 
				sTemp = pstHCurve->u8PixelInit + ((pstHCurve->bDataSign?(-1):1) * \
        		(((u16NumOfStep) * ((2 * pstHCurve->u8DeltaInit) + \
        		((u16NumOfStep - 1) * (pstHCurve->bDeltaSign?(-1):1) * pstHCurve->u8Delta))) / 2));

				DRVOSD_PRINT("sTemp = [%d]\n", sTemp);
					
				if (sTemp <= 0)
	    		{
	    			if (pstHCurve->bOverflowEnable)
	        			stHCurveR.u8PixelInit = sTemp % 256;
					else
	        			stHCurveR.u8PixelInit = 0x00;
	    		}
	    		else if (sTemp >= 0xFF)
	    		{
	    			if (pstHCurve->bOverflowEnable)
	        			stHCurveR.u8PixelInit = sTemp % 256;
					else
	        			stHCurveR.u8PixelInit = 0xFF;
	    		}
	    		else
	    		{
	        		stHCurveR.u8PixelInit = sTemp;
	    		}
			}

			if (stHCurveR.u16Length0 != 0)
				stHCurveR.u8CurveMode = 1;
			stHCurveR.u16Length1 = u16HEndRight - u16HStartRight - stHCurveR.u16Length0;
		
            DRVOSD_PRINT("Pixel init[%d]\n", pstHCurve->u8PixelInit);
            DRVOSD_PRINT("Delta init[%d]\n", pstHCurve->u8DeltaInit);
            DRVOSD_PRINT("Step[%d]\n", pstHCurve->u8Step);
            DRVOSD_PRINT("Data sign[%d]\n", pstHCurve->bDataSign);
            DRVOSD_PRINT("Delta sign[%d]\n", pstHCurve->bDeltaSign);
            DRVOSD_PRINT("Delta[%d]\n", pstHCurve->u8Delta);
            DRVOSD_PRINT("Length0[%d]\n", pstHCurve->u16Length0);
            DRVOSD_PRINT("Length1[%d]\n", pstHCurve->u16Length1);
            DRVOSD_PRINT("Offset sign/hv sign[%d]\n", pstHCurve->bOffsetSign);
            DRVOSD_PRINT("Overflow enable[%d]\n", pstHCurve->bOverflowEnable);
            DRVOSD_PRINT("Dot enable[%d]\n", pstHCurve->bDotEnable);
            DRVOSD_PRINT("Curve mode[%d]\n", pstHCurve->u8CurveMode);

            DRVOSD_PRINT("Pixel init[%d]\n", stHCurveR.u8PixelInit);
            DRVOSD_PRINT("Delta init[%d]\n", stHCurveR.u8DeltaInit);
            DRVOSD_PRINT("Step[%d]\n", stHCurveR.u8Step);
            DRVOSD_PRINT("Data sign[%d]\n", stHCurveR.bDataSign);
            DRVOSD_PRINT("Delta sign[%d]\n", stHCurveR.bDeltaSign);
            DRVOSD_PRINT("Delta[%d]\n", stHCurveR.u8Delta);
            DRVOSD_PRINT("Length0[%d]\n", stHCurveR.u16Length0);
            DRVOSD_PRINT("Length1[%d]\n", stHCurveR.u16Length1);
            DRVOSD_PRINT("Offset sign/hv sign[%d]\n", stHCurveR.bOffsetSign);
            DRVOSD_PRINT("Overflow enable[%d]\n", stHCurveR.bOverflowEnable);
            DRVOSD_PRINT("Dot enable[%d]\n", stHCurveR.bDotEnable);
            DRVOSD_PRINT("Curve mode[%d]\n", stHCurveR.u8CurveMode);
			
			OSD_COMMON_WRITE_ONOFF(FALSE);
        	drvOSD_SetCurveClass(u8CurveIndex, pstHCurve, pstVCurve);
            drvOSD_SetCurveClassR(u8CurveIndex, &stHCurveR, pstVCurve);			
			OSD_COMMON_WRITE_ONOFF(TRUE);
        }
        else // left OSD only or right OSD only
        {
            drvOSD_SetCurveClass(u8CurveIndex, pstHCurve, pstVCurve);
        }
    #else
        drvOSD_SetCurveClass(u8CurveIndex, pstHCurve, pstVCurve);
    #endif
    }
		
	msWriteByteMask(OSD1_00, u8Bank, 0x03); 		//recover subbank
    #undef SET_TGEN_GD_ENGINE
}
void drvOSD_SetTGenOn(BYTE u8Tgen,Bool u8On)		// TGen: gradient window
{
	BYTE xdata u8Bank = msReadByte(OSD1_00)&0x03;	// store current subbank

	msWriteByteMask(OSD1_00,0x00,0x03);				// switch to OSD subbank 0 (OSD ctrl)
    msWriteByteMask((u8Tgen<<3)+OSD1_20,(u8On?BIT0:0),BIT0);
	msWriteByteMask(OSD1_00,u8Bank,0x03);			// restore subbank
}
void drvOSDSetMask0(BYTE u8MaskKeyIndex, BYTE u8OnOff)
{
    WRITE_PSRAM0_ADDR();
    SET_OSD_CTRL_REG_WR_PORT(u8MaskKeyIndex);
    WRITE_PSRAM0_MASK();
    SET_OSD_CTRL_REG_WR_PORT(u8OnOff);
}
void drvOSD_ClrOSD_SRAM(void)
{
    BYTE u8Retry;
    extern void ForceDelay1ms(WORD u16Nums);

    for (u8Retry=0;u8Retry<=10;u8Retry++)
    {
        CAFSRAM_ERASE_TRIG();
        SET_OSD_CTRL_REG_WR_PORT(_ENABLE);
        ForceDelay1ms(3);
        if (!(msReadByte(OSD1_65)&BIT1))
            break;
    }
    for (u8Retry=0;u8Retry<=10;u8Retry++)
    {
        PSRAM0_ERASE_TRIG();
        SET_OSD_CTRL_REG_WR_PORT(_ENABLE);
        ForceDelay1ms(3);
        if (!(msReadByte(OSD1_75)&BIT1))
            break;
    }
    for (u8Retry=0;u8Retry<=10;u8Retry++)
    {
        PSRAM1_ERASE_TRIG();
        SET_OSD_CTRL_REG_WR_PORT(_ENABLE);
        ForceDelay1ms(3);
        if (!(msReadByte(OSD1_85)&BIT1))
            break;
    }
}

#if (CHIP_ID == MST9U3 && ENABLE_OSD_LR)
void drvOSD_SetCurveClassR(BYTE u8CurveIndex, const CurveClass* pstHCurve, const CurveClass* pstVCurve)
{	
    BYTE u8Bank = msReadByte(0x101A00)&0x03;
	
    if (!pstHCurve || !pstVCurve)
        return;
	
    msWriteByteMask(0x101A00,0x00,0x03);					//subbank 0
	
    msWriteByteMask(0x101A68, 0x1E, 0x1F);
    u8CurveIndex *= 2;
    msWriteByte(0x101A66, u8CurveIndex); 					//horizontal settings

    msWriteByteMask(0x101A68, 0x1F, 0x1F);
    msWriteByte(0x101A66, pstHCurve->u8PixelInit);
    msWriteByte(0x101A66, pstHCurve->u8DeltaInit);
    msWriteByte(0x101A66, pstHCurve->u8Step);
    msWriteByte(0x101A66, (pstHCurve->bDataSign<<7)|
                          (pstHCurve->bDeltaSign<<6)|
                          (pstHCurve->u8Delta));
    msWriteByte(0x101A66, pstHCurve->u16Length0&0xFF);       //hLength0[7:0]
    msWriteByte(0x101A66, ((pstHCurve->u16Length1<<4)|       //hLength1[3:0]
                          (pstHCurve->u16Length0>>8))&0xFF); //hLength0[10:8]
    msWriteByte(0x101A66, (pstHCurve->bOffsetSign<<7)|       //hv sign
                          (pstHCurve->u16Length1>>4));       //hLength1[10:4]
    msWriteByte(0x101A66, (pstHCurve->bOverflowEnable<<7)|
                          (pstHCurve->bDotEnable<<6)|
                          (pstHCurve->u8CurveMode));

    msWriteByteMask(0x101A68, 0x1E, 0x1F);
    msWriteByte(0x101A66, u8CurveIndex+1); //vertical settings

    msWriteByteMask(0x101A68, 0x1F, 0x1F);
    msWriteByte(0x101A66, pstVCurve->u8PixelInit);
    msWriteByte(0x101A66, pstVCurve->u8DeltaInit);
    msWriteByte(0x101A66, pstVCurve->u8Step);
    msWriteByte(0x101A66, (pstVCurve->bDataSign<<7)|
                          (pstVCurve->bDeltaSign<<6)|
                          (pstVCurve->u8Delta));
    msWriteByte(0x101A66, pstVCurve->u16Length0&0xFF);       //vLength0[7:0]
    msWriteByte(0x101A66, ((pstVCurve->u16Length1<<4)|       //vLength1[3:0]
                          (pstVCurve->u16Length0>>8))&0xFF); //vLength0[10:8]
    msWriteByte(0x101A66, (pstVCurve->u16Length1)>>4);       //vLength1[10:4]
    msWriteByte(0x101A66, (pstVCurve->bOverflowEnable<<7)|
                          (pstVCurve->bDotEnable<<6)|
                          (pstVCurve->u8CurveMode));
	
    msWriteByteMask(0x101A00,u8Bank,0x03);
}
void drvOSD_SetTGenHPositionL(BYTE u8Tgen, WORD u16HStart, WORD u16HEnd)
{
    BYTE xdata u8Bank = msReadByte(OSD1_00)&0x03;

    u8Tgen<<=3;
    OSD_COMMON_WRITE_ONOFF(FALSE);
    msWriteByteMask(OSD1_00,0x01,0x03);					//subbank 1
    msWrite2Byte((u8Tgen)+OSD1_04,u16HStart);
    msWrite2Byte((u8Tgen)+OSD1_06,u16HEnd);
    msWriteByteMask(OSD1_00,u8Bank,0x03);
    OSD_COMMON_WRITE_ONOFF(TRUE);
}
void drvOSD_SetTGenHPositionR(BYTE u8Tgen, WORD u16HStart, WORD u16HEnd)
{
    BYTE xdata u8Bank = msReadByte(OSD1_00)&0x03;

    u8Tgen<<=3;
    OSD_COMMON_WRITE_ONOFF(FALSE);
    msWriteByteMask(0x101A00,0x01,0x03);				//subbank 1
    msWrite2Byte((u8Tgen)+0x101A04,u16HStart);
    msWrite2Byte((u8Tgen)+0x101A06,u16HEnd);
    msWriteByteMask(0x101A00,u8Bank,0x03);
    OSD_COMMON_WRITE_ONOFF(TRUE);
}
void drvOSD_SetTGenVPositionL(BYTE u8Tgen, WORD u16VStart, WORD u16VEnd)
{
    BYTE xdata u8Bank = msReadByte(OSD1_00)&0x03;

    u8Tgen<<=3;
    OSD_COMMON_WRITE_ONOFF(FALSE);
    msWriteByteMask(OSD1_00,0x01,0x03);					//subbank 1
    msWrite2Byte((u8Tgen)+OSD1_08,u16VStart);
    msWrite2Byte((u8Tgen)+OSD1_0A,u16VEnd);
    msWriteByteMask(OSD1_00,u8Bank,0x03);
    OSD_COMMON_WRITE_ONOFF(TRUE);
}
void drvOSD_SetTGenVPositionR(BYTE u8Tgen, WORD u16VStart, WORD u16VEnd)
{
    BYTE xdata u8Bank = msReadByte(OSD1_00)&0x03;

    u8Tgen<<=3;
    OSD_COMMON_WRITE_ONOFF(FALSE);
    msWriteByteMask(0x101A00,0x01,0x03);				//subbank 1
    msWrite2Byte((u8Tgen)+0x101A08,u16VStart);
    msWrite2Byte((u8Tgen)+0x101A0A,u16VEnd);
    msWriteByteMask(0x101A00,u8Bank,0x03);
    OSD_COMMON_WRITE_ONOFF(TRUE);
}
void drvOSD_SetTgenCurveL(BYTE u8Tgen, BYTE u8ARGB, BYTE u8Engine, BYTE u8CurveIndex, const CurveClass* pstHCurve, const CurveClass* pstVCurve)
{
    #define SET_TGEN_GD_ENGINE(tgen, gd, value) ((void)(\
        msWriteByteMask(OSD1_00, 0x01, 0x03),\
        msWrite2ByteMask(OSD1_28+6*(gd), ((WORD)(value))<<(tgen), ((WORD)0x0F)<<(tgen))))

    OSD_COMMON_WRITE_ONOFF(FALSE);
    Bool bSetCurve = FALSE;
	BYTE u8Bank = msReadByte(OSD1_00)&0x03;

    if (u8Engine<6)									//must use GD0~GD5
    {
        bSetCurve = TRUE;
        msWriteByteMask(OSD1_00, 0x00, 0x03);		//subbank0
        switch (u8ARGB)
        {
        	case OSD_PG_RED:
				msWriteByteMask(OSD1_20+(u8Tgen<<3), u8Engine<<4, 0x70);
				break;
			case OSD_PG_GREEN:
				msWriteByteMask(OSD1_21+(u8Tgen<<3), u8Engine<<4, 0xF0);
				break;
			case OSD_PG_BLUE:
				msWriteByteMask(OSD1_21+(u8Tgen<<3), u8Engine, 0x0F);
				break;
			default:
				break;
        }
        SET_TGEN_GD_ENGINE(u8Tgen<<2, u8Engine, u8CurveIndex);
    }

    if (bSetCurve)
    {
        drvOSD_SetCurveClass(u8CurveIndex, pstHCurve, pstVCurve);
    }
	msWriteByteMask(OSD1_00, u8Bank, 0x03); 			//recover subbank
	OSD_COMMON_WRITE_ONOFF(TRUE);
    #undef SET_TGEN_GD_ENGINE
}
void drvOSD_SetTgenCurveR(BYTE u8Tgen, BYTE u8ARGB, BYTE u8Engine, BYTE u8CurveIndex, const CurveClass* pstHCurve, const CurveClass* pstVCurve)
{
    #define SET_TGEN_GD_ENGINE(tgen, gd, value) ((void)(\
        msWriteByteMask(0x101A00, 0x01, 0x03),\
        msWrite2ByteMask(0x101A28+6*(gd), ((WORD)(value))<<(tgen), ((WORD)0x0F)<<(tgen))))

    OSD_COMMON_WRITE_ONOFF(FALSE);
    Bool bSetCurve = FALSE;
	BYTE u8Bank = msReadByte(0x101A00)&0x03;

    if (u8Engine<6)										//must use GD0~GD5
    {
        bSetCurve = TRUE;
        msWriteByteMask(0x101A00, 0x00, 0x03);			//subbank0
        switch (u8ARGB)
        {
        	case OSD_PG_RED:
				msWriteByteMask(0x101A20+(u8Tgen<<3), u8Engine<<4, 0x70);
				break;
			case OSD_PG_GREEN:
				msWriteByteMask(0x101A21+(u8Tgen<<3), u8Engine<<4, 0xF0);
				break;
			case OSD_PG_BLUE:
				msWriteByteMask(0x101A21+(u8Tgen<<3), u8Engine, 0x0F);
				break;
			default:
				break;
        }
        SET_TGEN_GD_ENGINE(u8Tgen<<2, u8Engine, u8CurveIndex);
    }

    if (bSetCurve)
    {
        drvOSD_SetCurveClassR(u8CurveIndex, pstHCurve, pstVCurve);
    }

	msWriteByteMask(0x101A00, u8Bank, 0x03); 			//recover subbank
	OSD_COMMON_WRITE_ONOFF(TRUE);
    #undef SET_TGEN_GD_ENGINE
}
void drvOSD_SetTGenOnL(BYTE u8Tgen,Bool u8On)			// TGen: gradient window
{
	BYTE xdata u8Bank = msReadByte(OSD1_00)&0x03;		// store current subbank

    OSD_COMMON_WRITE_ONOFF(FALSE);
	msWriteByteMask(OSD1_00,0x00,0x03);					// switch to OSD subbank 0 (OSD ctrl)
    msWriteByteMask((u8Tgen<<3)+OSD1_20,(u8On?BIT0:0),BIT0);
	msWriteByteMask(OSD1_00,u8Bank,0x03);				// restore subbank
    OSD_COMMON_WRITE_ONOFF(TRUE);
}
void drvOSD_SetTGenOnR(BYTE u8Tgen,Bool u8On)			// TGen: gradient window
{
	BYTE xdata u8Bank = msReadByte(OSD1_00)&0x03;		// store current subbank

    OSD_COMMON_WRITE_ONOFF(FALSE);
	msWriteByteMask(0x101A00,0x00,0x03);				// switch to OSD subbank 0 (OSD ctrl)
    msWriteByteMask((u8Tgen<<3)+0x101A20,(u8On?BIT0:0),BIT0);
	msWriteByteMask(0x101A00,u8Bank,0x03);				// restore subbank
	OSD_COMMON_WRITE_ONOFF(TRUE);
}
#endif


///////////////////////////////////////////////////////////////////////////////
/// @file msOSD.h
/// @brief Head file of msOSD.c
/// @author MStarSemi Inc.
///
/// Functions to draw OSD.
///
/// Features
///  -OSD registers read/write.
///  -Draw number/Hex number/text/gauge etc.
///  -Functions to load font.
///  -Functions to set color palette.
///  -
///////////////////////////////////////////////////////////////////////////////
#ifndef _MSOSD_H
#define _MSOSD_H

#include "drvOSD.h"

#define DOUBLE_X_SIZE   0
#define DOUBLE_Y_SIZE   0


#define SUBWIN0 0
#define SUBWIN1 1
#define SUBWIN2 2
#define SUBWIN3 3

#ifndef _MSOSD_C
extern WORD xdata g_u8OsdWindowWidth;
extern BYTE xdata g_u8OsdWindowHeight;
#if 0//defined(_OSD_ROTATION_270_)//White130702
extern BYTE xdata g_u8OsdWindow2Height;
#endif
extern BYTE xdata g_u8OsdFontColor;
extern BYTE xdata g_u8OsdFontDataHighByte;
#endif


//extern BYTE mStar_ReadOSDByte(WORD u8Address);
//extern void mStar_WriteOSDByte(WORD u8Address, BYTE u8Value);
extern void Osd_DrawNum(BYTE u8XPos, BYTE u8YPos, int s16Value);
extern void Osd_Draw5Num(BYTE u8XPos, BYTE u8YPos, WORD u16Value);
extern void DrawNumUnsigned(BYTE u8XPos, BYTE u8YPos, char len, WORD u16Value);
//extern void Osd_DrawStr(BYTE u8XPos, BYTE u8YPos, BYTE * pu8Str);
//extern void Osd_DrawHex(BYTE u8XPos, BYTE u8YPos, WORD u16Value);
extern void Osd_SetPosition(BYTE u8XPos, BYTE u8YPos);
extern void Osd_SetWindowSize(BYTE u8Width, BYTE u8Height/*, Bool bReset*/);
//extern void Osd_SetTextColor(BYTE u8ForeColor, BYTE u8BackColor);
//extern void Osd_DrawChar(BYTE u8XPos, BYTE u8YPos, BYTE u8Char);
//extern void Osd_DrawGauge(BYTE u8XPos, BYTE u8YPos, BYTE u8Length, BYTE u8Value);
//extern void InitOsdAfterLogo(void);
//extern void Osd_ReloadNumberFont(void);
//extern void Osd_DynamicLoadIcon2LFont(BYTE u8Addr, BYTE *pu8Font);

//extern void Osd_DrawRealStr(BYTE u8XPos, BYTE u8YPos, BYTE *pu8Str);
extern void Osd_DrawRealChar(BYTE u8XPos, BYTE u8YPos, WORD u8Char);
extern void Osd_DrawContinuousChar(BYTE u8XPos, BYTE u8YPos, BYTE u8Char, BYTE u8Num);
extern WORD Osd_HPositionCorrection(WORD u16Value);
extern WORD Osd_MenuHPosition(BYTE u8XPos,Bool Set);
extern WORD Osd_MenuVPosition(BYTE u8YPos,Bool Set);
extern void SetOsdPositionReg(WORD u16XPos, WORD u16YPos);

extern void Osd_SetTransparency(BYTE u8Value);
extern void Osd_DrawBlankPlane(BYTE u8XPos,BYTE u8YPos,BYTE u8Width,BYTE u8Height);
extern void Osd_DrawHex(BYTE u8XPos, BYTE u8YPos, BYTE u8Value);

extern void Osd_SetWindowAtr(BYTE u8Win_Num,BYTE u8HStart,BYTE u8HEnd ,BYTE u8VStart, BYTE u8VEnd,BYTE u8Color);
extern void Osd_SetWindowEnable(BYTE u8Win_Num,BYTE u8Enable);

//#define mStar_WriteOSDByteMask(u16Reg,u8Value,u8Mask)    mStar_WriteOSDByte(u16Reg,((mStar_ReadOSDByte(u16Reg)&(~(u8Mask)))|((u8Value)&(u8Mask))))

//OSD_WIN0 for PG

//#define OSD_WIN_USED_MAX    OSD_WIN3



#define OSD_MAIN_WND        OSD_WIN1
#define OSD_MAIN_WIN_BIT    OSD_GD_WIN1_EN_BIT
#define OSD_BUTTON_WND      OSD_WIN2
#define OSD_CURSOR_WND      OSD_WIN3
#define OSD_CURSOR_WIN_BIT  OSD_GD_WIN3_EN_BIT
#define OSD_ICON_WND        OSD_WIN4
#define OSD_ICON_WIN_BIT    OSD_GD_WIN4_EN_BIT
#define OSD_SUB_WND        OSD_WIN5
#define OSD_SUB_WIN_BIT    OSD_GD_WIN5_EN_BIT
#define OSD_SUB2_WND        OSD_WIN6
#define OSD_SUB2_WIN_BIT    OSD_GD_WIN6_EN_BIT


#define OSD_MAIN_WND_CA_BASE    0x0001
#define OSD_MAIN_WND_SIZE           1728//1600//(20+20+24)*27
#define OSD_BUTTON_WND_CA_BASE  (OSD_MAIN_WND_CA_BASE+OSD_MAIN_WND_SIZE)
#define OSD_BUTTON_WND_SIZE     60//
#define OSD_CURSOR_WND_CA_BASE  (OSD_BUTTON_WND_CA_BASE+OSD_BUTTON_WND_SIZE)
#define OSD_CURSOR_WND_SIZE     60//
#define OSD_ICON_WND_CA_BASE  (OSD_CURSOR_WND_CA_BASE+OSD_CURSOR_WND_SIZE)
#define OSD_ICON_WND_SIZE    60//
#define OSD_SUB_WND_CA_BASE  (OSD_ICON_WND_CA_BASE+OSD_ICON_WND_SIZE)
#define OSD_SUB_WND_SIZE     60//
#define OSD_SUB2_WND_CA_BASE  (OSD_SUB_WND_CA_BASE+OSD_SUB_WND_SIZE)
#define OSD_SUB2_WND_SIZE     60//

#define OSD_MSG_WND_SIZE     200//60/
#define OSD_MSG0_WND_CA_BASE  (OSD_MAIN_WND_CA_BASE)
#define OSD_MSG1_WND_CA_BASE  (OSD_MSG0_WND_CA_BASE+OSD_MSG_WND_SIZE)
#define OSD_MSG2_WND_CA_BASE  (OSD_MSG1_WND_CA_BASE+OSD_MSG_WND_SIZE)
#define OSD_MSG3_WND_CA_BASE  (OSD_MSG2_WND_CA_BASE+OSD_MSG_WND_SIZE)
#define OSD_MSG4_WND_CA_BASE  (OSD_MSG3_WND_CA_BASE+OSD_MSG_WND_SIZE)


extern BYTE xdata g_u8OsdWndNo;
extern BYTE xdata g_u8FontAddrHiBits;
extern WORD xdata g_u16OsdWndCABase[];
extern BYTE xdata g_WindowEnable;

#define OSD_SET_WND(u8Wnd)  (g_u8OsdWndNo=u8Wnd)
#define OSD_ENABLE_GRADIENT_COLOR()     //msWriteByteMask(OSD1_15, BIT3, BIT3)
#define OSD_DISABLE_GRADIENT_COLOR()    //msWriteByteMask(OSD1_15,    0, BIT3)

//Font RAM bit8/bit9
#define OSD_FONT_HI_ADDR_SET_BIT8()     (g_u8FontAddrHiBits=BIT0) //msWriteByteMask(OSD2_AE, BIT7, BIT7|BIT6); //enable bit 8
#define OSD_FONT_HI_ADDR_SET_BIT9()     (g_u8FontAddrHiBits=BIT1) //msWriteByteMask(OSD2_AE, BIT6, BIT7|BIT6); //enable bit 9
#define OSD_FONT_HI_ADDR_SET_BIT10()     	(g_u8FontAddrHiBits=BIT0|BIT1) //msWriteByteMask(OSD2_AE, BIT6, BIT7|BIT6); //enable bit 10
#define OSD_FONT_HI_ADDR_CLR_TO_0()     (g_u8FontAddrHiBits=0) //msWriteByteMask(OSD2_AE,    0, BIT7|BIT6);

#define  OSD_FONT_CLEAR()  					(g_u8FontAddrHiBits=0);\
                                              					(g_u8OsdFontDataHighByte=0)

#define OSD_FONT_START_ENTRY        0x200   //464(0x1D0) + 2160 entries= 2088 c/a + 720 fonts
#define OSD_FONT_UNIT_SIZE          3       //12x18 font, 1.5x18 = 27 Bytes = 3 x 72 bits = 3 entries
#define GET_FONT_RAM_ADDR(u8Addr)   ((OSD_FONT_START_ENTRY+((((WORD)g_u8FontAddrHiBits)<<8)+(u8Addr))*OSD_FONT_UNIT_SIZE)<<2)

typedef struct
{
    BYTE bButtonOsd:1; //For saperate OSD using space function
    BYTE bSync2Main:1; //Enable cursor Wnd while main wnd enabled(Old OSD structure, subwnd was shown while main OSD enabled)
}WrapLayerFlagType;
//extern WrapLayerFlagType xdata WrapLayerFlag;

#if 1
#define MAIN_GD_WIN         OSD_WIN2
#define MAIN_GD_WIN_EN_BIT  OSD_GD_WIN2_EN_BIT
#else
#define MAIN_GD_WIN SUBWIN1
#endif

extern BYTE Osd_DoubleBuffer(Bool u8Enable);

extern void Osd_SetInvVGradientColorDeltaRGB(BYTE u8Red,BYTE u8Green,BYTE u8Blue);
extern void Osd_SetVGradientColorDeltaRGB(BYTE u8Red,BYTE u8Green,BYTE u8Blue);
extern void Osd_SetHGradientColorDeltaRGB(BYTE u8Red,BYTE u8Green,BYTE u8Blue);
extern void Osd_SetVGradientColorStep(BYTE u8Red,BYTE u8Green,BYTE u8Blue);
extern void Osd_SetHGradientColorStep(BYTE u8Red,BYTE u8Green,BYTE u8Blue);

extern void Osd_SetMultiWindowSize(BYTE u8Width, BYTE u8Height, BYTE u8WinIndex,WORD u16WinAtr);
extern void Osd_SetMultiPosition(BYTE u8XPos, BYTE u8YPos,BYTE u8WinIndex);
extern void Osd_DisableSubWin( BYTE sw );
extern void Osd_EnableSubWin( BYTE u8Win_Num );

#define H_POS_CORRECTION        0

#endif ///_MSOSD_H

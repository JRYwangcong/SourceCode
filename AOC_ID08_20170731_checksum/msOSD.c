#include <stdlib.h>
#include "types.h"
#include "ms_reg.h"
#include "panel.h"
#include "Board.h"
#include "debug.h"
#include "ms_rwreg.h"
#include "mstar.h"
#include "misc.h"
#include "global.h"
#include "menudef.h"
#include "font.h"
#include "extlib.h"
#include "MsID_V1.h"


#define DoubleXSize 0
#define DoubleYSize 0
#if DoubleXSize
#define DOUBLE_RATIO_X  2
#else
#define DOUBLE_RATIO_X  1
#endif
#if DoubleYSize
#define DOUBLE_RATIO_Y  2
#else
#define DOUBLE_RATIO_Y  1
#endif
#define Color8FontRAMStart   0xA2 //0xA2//0x7d // 100
//==================================================================================
// Local function declaration
//
void mStar_WriteOSDByte(BYTE address, BYTE value);
BYTE mStar_ReadOSDByte(BYTE address);
#if OSD_MOVE_BY_PIXL
void mStar_WriteOSDByteMask(BYTE ucReg, BYTE ucValue, BYTE ucMask);
#endif
void Osd_SetWindowSize(BYTE width, BYTE height);
void Osd_SetPosition(BYTE xPos, BYTE yPos);
//void Osd_DrawChar(BYTE xPos, BYTE yPos, BYTE ch);
//void Osd_LoadFont(BYTE addr, WORD *fontPtr, BYTE num);
void Osd_LoadFont(BYTE addr, BYTE *fontPtr, WORD num);
#if !defined TSUMXXNX && !defined TSUMXXQX && !defined TSUMXXT		//110311 Modify
void Osd_LoadColor(BYTE *colorPtr, BYTE num);
#endif
void Osd_Load256Color(BYTE addr, BYTE *colorPtr, WORD num);
void Osd_Load291Font(BYTE addr, BYTE *fontPtr, WORD num);
void LoadCommonFont(void);
void Osd_LoadLogoFontCP(void);
//extern void LoadPropFonts(BYTE *fonts, WORD num);
void Osd_LoadMainMenuIconFont(void);
void Osd_LoadCompressColorFont(BYTE addr, BYTE *fontPtr, WORD *mapPtr, BYTE count);
void SetOsdGraduallyBackGround( BYTE Win_Num, BYTE GDValue, BYTE GDDelta , BYTE GDDERange, BYTE GDControl, BYTE GDInit );

extern void LoadMainTitleFont(BYTE index);
extern void mStar_WaitForDataBlanking(void);

//==================================================================================
// variable declaration
//
BYTE OsdWindowWidth = 10;
BYTE OsdWindowHeight = 10;
BYTE OsdFontColor = 0x01;
extern  BYTE MenuPageIndex;
extern  BYTE MenuItemIndex;

BYTE xdata OsdEngStatus=0;
//==================================================================================
#if Enable_OsdEngine1||Enable_CascadeMode
void Osd_SetOsdEngine(BYTE ucValue)
{
	mStar_WriteByte(REGBK, 0x06);

	if(ucValue)
	{
		mStar_WriteByte(0x6A, BIT4);	//BIT4:S1 RW Enable
		OsdEngStatus=1;
	}
	else
	{
		mStar_WriteByte(0x6A, 0x00);
		OsdEngStatus=0;
	}
	mStar_WriteByte(REGBK, REGBANKSCALER);
}
#endif

void Osd_InitOSD(void)
{
    // Osd transparency control
    //{
        BYTE retry = 10;
        while (retry--)
        {
            mStar_WriteByte(OSDIOA, CLR_B);
            Delay1ms(3);
            if (mStar_ReadByte(OSDIOA) &CLR_B)
            {
                mStar_ResetDClkPLL();
            }
            else
            {
                break;
            }
        }
    //}

    for(retry=0;retry<2;retry++)
    {
        #if Enable_OsdEngine1||Enable_CascadeMode
        if(retry==0)
            Osd_SetOsdEngine(1);
        else
		Osd_SetOsdEngine(0);
#endif
    mStar_WriteByte(BLENDC, 0); //
    mStar_WriteByte(BLENDL, 0); //
    mStar_WriteOSDByte(OSD_10, 0x80);
    mStar_WriteOSDByte(OSDDBC, 0x5); // enable double
    mStar_WriteOSDByte(OCBUFO, 0x00); // disable OSD code buffer Base/Offset address
    mStar_WriteOSDByte(IOSDC3, 0x00); //0x0F); // shadow function control
    mStar_WriteOSDByte(OSDHC, 0x11); // shadow width control
    Osd_SetWindowSize(20, 10); // set osd window
    mStar_WriteOSDByte(IOSDC2, 0x00); // color bit setting
#if Enable_GDColor
    mStar_WriteOSDByte(OSD_GD, BIT7);
    mStar_WriteOSDByte(OSD_ColorGD, BIT7);
#endif
    LoadCommonFont(); //eshin
    Osd_LoadMainMenuIconFont();    
    #if !defined TSUMXXNX && !defined TSUMXXQX && !defined TSUMXXT		//110311 Modify
    Osd_LoadColor(&tOSDColorPalette0, sizeof(tOSDColorPalette0)); // load osd color
    #endif
    Osd_SetPosition(50, 50); // set osd position
}
#if Enable_GDColor
	#if PanelType==PanelCLAA220WA02
		SetOsdGraduallyBackGround( SUB_WIN_02, 3, 0 , 0, 0x12, 0x56);
	#else
		SetOsdGraduallyBackGround( SUB_WIN_02, 3, 0 , 0, 0x12, 0x50);
	#endif
SetOsdGraduallyBackGround( SUB_WIN_01, 0x82, 0 , 0, 0x2, 0);//( SUB_WIN_01, 0x82, 0 , 0, 0x2, 0xDC);
SetOsdGraduallyBackGround( SUB_WIN_00, 3, 0 , 0, 0x92, 0xC0);//( SUB_WIN_00, 3, 0 , 0, 0x92, 0xC8);
#endif

#if OSD_MOVE_BY_PIXL

    mStar_WriteOSDByte(OSD_70, BIT4|BIT2);// [4]: set osd move step by 1 pixel; [2] add 9 pixels

    // scaling compensation
    if (ChipVer==TSUM_F||ChipVer==TSUM_L||ChipVer==TSUM_Q)//defined TSUMXXFX || defined TSUMXXLX || defined TSUMXXQX
    {
        mStar_WriteOSDByteMask(OSD_10, BIT5, BIT5);                            
    }
    else if (ChipVer==TSUM_E||ChipVer==TSUM_G||ChipVer==TSUM_M)//defined TSUMXXEX || defined TSUMXXGX || defined TSUMXXMX
    {
        mStar_WriteOSDByteMask(OSD_10, BIT5, BIT5);                            
        mStar_WriteOSDByte(OSD_77, mStar_ReadOSDByte(OSD_77)|BIT4);
        mStar_WriteByte(REGBK, REGBANK6);
        mStar_WriteByte(0x6A, mStar_ReadByte(0x6A)|BIT7);
        mStar_WriteByte(REGBK, REGBANK0);
    }
    else if (ChipVer==TSUM_N||ChipVer==TSUM_R)//defined TSUMXXNX || defined TSUMXXRX
    {
        mStar_WriteOSDByteMask(OSD_10, 0, BIT5);                            
    }
    
    
#endif
    
}
void MonoColorSetting(void)
{
    #if defined TSUMXXNX || defined TSUMXXQX || defined TSUMXXT		//110311 Modify
        #if OSD_MOVE_BY_PIXL
        mStar_WriteOSDByteMask(OSD_10, 0x80|BIT6, ~BIT5);
        #else
        mStar_WriteOSDByte(OSD_10, 0x80|BIT6);
        #endif
    #else
        #if OSD_MOVE_BY_PIXL
        mStar_WriteOSDByteMask(OSD_10, 0x80, ~BIT5);
        #else
        mStar_WriteOSDByte(OSD_10, 0x80);
        #endif
    #endif
    mStar_WriteOSDByte(OSDDBC, 0x5); // enable double
    mStar_WriteOSDByte(OCBUFO, 0x00); // disable OSD code buffer Base
    mStar_WriteOSDByte(IOSDC3, 0x00); //0x0F); // shadow function control
    mStar_WriteOSDByte(OSDHC, 0x11); // shadow width control
    mStar_WriteOSDByte(IOSDC2, 0x00); // color bit setting
    mStar_WriteOSDByte(0x23, 0x00);// 071224
    #if defined TSUMXXNX || defined TSUMXXQX || defined TSUMXXT		//110311 Modify
    Osd_Load256Color(0x00, (BYTE *)&tOSDColorPalette0, sizeof(tOSDColorPalette0));   //HighlightMenuPageIndex
    #else
    Osd_LoadColor(&tOSDColorPalette0, sizeof(tOSDColorPalette0)); // load osd color
    #endif
}

#if 0
    //ENABLE SUBWINDOW FUNCTION WHEN LOAD NEW FONT
    void EnableOsdWindow(BYTE winIndex, BYTE FgColor, BYTE BgColor)
    {
        mStar_WriteOSDByte(SUBW0C + winIndex * 6, 0x01); // enable sub-windows1
        //  mStar_WriteOSDByte(SUBW0A+winIndex*6, FgColor<<4|BgColor); //forground: black / background: orange
        mStar_WriteOSDByte(SUBW0A + winIndex * 6, FgColor &0x0 | BgColor &0xff); //forground: black / background: orange
    }
    void DisableOsdWindow(BYTE winIndex)
    {
        mStar_WriteOSDByte(SUBW0C + winIndex * 6, 0x00); // enable sub-windows1
    }
    void MoveOsdWindow(BYTE winIndex, BYTE YPos, BYTE xPosStar, BYTE Xlength, BYTE Ylength, BYTE FgColor, BYTE BgColor)
    {
        DisableOsdWindow(winIndex);
        mStar_WriteOSDByte(SW0HST + winIndex * 6, xPosStar);
        mStar_WriteOSDByte(SW0HEND + winIndex * 6, xPosStar + Xlength);
        mStar_WriteOSDByte(SW0VST + winIndex * 6, YPos); //2
        mStar_WriteOSDByte(SW0VEND + winIndex * 6, YPos + Ylength); //2
        EnableOsdWindow(winIndex, FgColor, BgColor);
    }
#endif
// setup osd window size
void Osd_SetWindowSize(BYTE width, BYTE height)
{
    #if defined TSUMXXNX || defined TSUMXXQX
    OsdWindowWidth = width &0x7F;
    OsdWindowHeight = height &0x3F;
    #else	
    #if (DisplayLogo==EnvisionLOGO)//(ModelName == Envision)
        OsdWindowWidth = width;
    #else
        OsdWindowWidth = width &0x3F;
    #endif
    OsdWindowHeight = height &0x1F;
    #endif
    mStar_WriteOSDByte(OSDW, OsdWindowWidth - 1);
    mStar_WriteOSDByte(OSDH, OsdWindowHeight - 1);
    mStar_WriteByte(OSDIOA, 0xDE); // clear osd content
    ForceDelay1ms(3);
    //mStar_WriteByte(OSDIOA, 0x9E);
    mStar_WriteByte(OSDIOA, 0xBE);
}
// setup osd parence
void Osd_SetTransparence(BYTE value)
{
    mStar_WriteByte(BLENDC, 0x05);
    mStar_WriteByte(BLENDL, value);
}
// setup
void Osd_SetPosition(BYTE xPos, BYTE yPos)
{
#if OSD_MOVE_BY_PIXL
    WORD OPBackPorch;
    WORD posValue;
    BYTE PIPESHIFT;

    if (ChipVer == TSUM_M)
        PIPESHIFT = 7;
    else if (ChipVer == TSUM_F || ChipVer == TSUM_L)
        PIPESHIFT = 11;
    else if (ChipVer == TSUM_R || ChipVer == TSUM_T)	// 110419 Modify
        PIPESHIFT = mStar_ReadOSDByte(0x4B);
    else
        PIPESHIFT = 6;


    if  (ChipVer == TSUM_R)//defined TSUMXXRX
        OPBackPorch = mStar_ReadWord(DEHST_H);
    else
        OPBackPorch = (WORD)(mStar_ReadWord(DEHST_H)-mStar_ReadByte(HSEND));
    
	if(ChipVer == TSUM_T)	// 110419 Modify
		OPBackPorch += 5;
	
    //posValue = ((DWORD)xPos*(PanelWidth-(WORD)OsdWindowWidth*12*DOUBLE_RATIO_X))/100+(OPBackPorch+9-PIPESHIFT);
    #if ModelName==AOC_E2237Fwh1
	posValue=((DWORD)xPos*(PanelWidth-((WORD)OsdWindowWidth*12*DOUBLE_RATIO_X)-18))/100+0x0067;
    #else
       posValue = ((DWORD)xPos*(PanelWidth-(WORD)OsdWindowWidth*12*DOUBLE_RATIO_X-18))/100+(OPBackPorch+18-PIPESHIFT);//edit by lizzie.ding,for the spec request osd can't keep to the side.
    #endif
	 
    if (ChipVer == TSUM_F|| ChipVer == TSUM_L||ChipVer==TSUM_K)//defined TSUMXXFX || defined TSUMXXLX || defined TSUMXXKX
       {
        mStar_WriteOSDByte(OHSTA_L, ((posValue>>2)&0xFF));
        mStar_WriteOSDByte(OHSTA_H, (posValue >> 10)|(posValue<<6));
    	}
    else
    	{
        mStar_WriteOSDByte(OHSTA_L, posValue & 0xFF);
        mStar_WriteOSDByte(OHSTA_H, posValue >> 8);
    	}
    

    yPos = 100-yPos;
    posValue = ((DWORD)yPos*(PanelHeight-(WORD)OsdWindowHeight*18*DOUBLE_RATIO_Y))/100;

    if (ChipVer == TSUM_F|| ChipVer == TSUM_L||ChipVer==TSUM_K)//defined TSUMXXFX || defined TSUMXXLX || defined TSUMXXKX
    	{
    mStar_WriteOSDByte(OVSTA_L, ((posValue>>2)&0xFF));
    mStar_WriteOSDByte(OVSTA_H, (posValue >> 10)|(posValue<<6) );
    	}
    else
    	{
    mStar_WriteOSDByte(OVSTA_L, posValue & 0xFF);
    mStar_WriteOSDByte(OVSTA_H, posValue >> 8);
    	}
    

#else // !OSD_MOVE_BY_PIXL
    WORD posValue;
#if OSD_PIVOT_EN  //071226
if(OSDRotateFlag)
{
#if DoubleXSize
  posValue = ((DWORD)xPos *(PanelHeight - (WORD)OsdWindowWidth *12 * 2)) / 100;
#else
  posValue=((DWORD)xPos*(PanelHeight-((WORD)OsdWindowWidth*12)-9))/100;

#endif



if(xPos==0)
{
  posValue=posValue/4+3;
}
else
{
  posValue=posValue/4;
}

  if(xPos==100)
  	mStar_WriteByte(HSEND, PanelHSyncWidth - 2);  
  else
  	mStar_WriteByte(HSEND, PanelHSyncWidth - 1);  


  mStar_WriteOSDByte(OVSTA_L, posValue&0xFF);
  mStar_WriteOSDByte(OVSTA_H, posValue>>8);

#if DoubleXSize
  posValue = ((DWORD)yPos *(PanelWidth- (WORD)OsdWindowHeight *18 * 2)) / 100+(PanelHStart - 48);
#else
  posValue=((DWORD)yPos*(PanelWidth-((WORD)OsdWindowHeight*18)-0))/100+(PanelHStart-18);
#endif
	
  posValue=posValue/4;

  mStar_WriteOSDByte(OHSTA_L, posValue&0xFF);
  mStar_WriteOSDByte(OHSTA_H, posValue>>8);
}
else
#endif
{
    yPos = 100-yPos;
    #if DoubleXSize
        posValue = ((DWORD)xPos *(PanelWidth - (WORD)OsdWindowWidth *12 * 2)) / 100+(PanelHStart - 48);
    #else
        #ifdef PanelTpye_1366// (PanelWidth == 1366)//#if PanelType==PanelM156B1L01||PanelType==PanelSVA156WX1 || PanelType == PanelCLAA185WA01|| PanelType == PanelM185B1L01||PanelType==PanelLM185WH1TLA1||PanelType==PanelAUOM185XW01 
        if (StandardModeGroup == Res_1360x768 && abs(StandardModeVFreq - 600) < 10)
        #if  ModelName==ENVISION_P971wl||ModelName==ENVISION_P971wl1||ModelName==ENVISION_P971wal1||ModelName==ENVISION_P971wal
        posValue=((DWORD)xPos*(PanelWidth-((WORD)OsdWindowWidth*12)-18))/100+9+(PanelHStart-20);	//071226
        #else
	  posValue=((DWORD)xPos*(PanelWidth-((WORD)OsdWindowWidth*12)-18))/100+9+(PanelHStart-26);	//071226
	 #endif
        else
        #endif	
		#if  ModelName==ENVISION_P2071l&&PanelType==PanelTPM200O1L02&&ModelName==ENVISION_P2071l1
        posValue=((DWORD)xPos*(PanelWidth-((WORD)OsdWindowWidth*12)-18))/100+9+(PanelHStart-24);	//071226
             #elif ModelName==ENVISION_P2471
	 posValue=((DWORD)xPos*(PanelWidth-((WORD)OsdWindowWidth*12)-18))/100+9+(PanelHStart-28);
		#elif ModelName==ENVISION_G2470||ModelName==ENVISION_G2770||ModelName==AOC_e2795v||ModelName==ENVISION_G2770L 
	posValue=((DWORD)xPos*(PanelWidth-((WORD)OsdWindowWidth*12)-18))/100+9+(PanelHStart-21);		
		#elif ModelName==AOC_E2437Fh
	posValue=((DWORD)xPos*(PanelWidth-((WORD)OsdWindowWidth*12)-18))/100+9+(PanelHStart-12);
		#else
        posValue=((DWORD)xPos*(PanelWidth-((WORD)OsdWindowWidth*12)-18))/100+9+(PanelHStart-26);	//071226
		#endif
 //posValue = ((DWORD)xPos *(PanelWidth - (WORD)OsdWindowWidth *12)) / 100+(PanelHStart - 48); //);
		//when minihtotal =1780    posValue = ( ( DWORD ) xPos * ( PanelWidth - ( WORD ) OsdWindowWidth * 12 ) ) / 100  + PanelHSyncBackPorch + 30;
        // printData("posValue=%d", posValue);
    #endif
#if 1		  //071226 
    if(xPos==0)
    posValue = posValue / 4+1;
    else		
    posValue = posValue / 4;
#else
    posValue=posValue/4;	//071226
//posValue = posValue / 4+2;
#endif
    mStar_WriteOSDByte(OHSTA_L, posValue &0xFF);
    mStar_WriteOSDByte(OHSTA_H, posValue >> 8);
    #if DoubleYSize
        posValue = ((DWORD)yPos *(PanelHeight - (WORD)OsdWindowHeight *18 * 2)) / 100;
    #else
        posValue = ((DWORD)yPos *(PanelHeight - (WORD)OsdWindowHeight *18)) / 100;
    #endif
    posValue = posValue / 4;
    //;
    mStar_WriteOSDByte(OVSTA_L, posValue &0xFF);
    mStar_WriteOSDByte(OVSTA_H, posValue >> 8);
}

#endif

}

/*Move it to Common.c
void Osd_LoadFont(BYTE addr, BYTE *fontPtr, WORD num)
{
    mStar_WriteByte(RAMFA, addr);
    while (num--)
    {
        mStar_WriteByte(RAMFD, *(fontPtr++));
    }
}*/

    void Osd_Load291Font(BYTE addr, BYTE *fontPtr, WORD num)
    {
    	 #if Enable_CascadeMode
	 Osd_SetOsdEngine(1);
	 #else
        mStar_WriteByte(OSDDF, RAMFA8_B); //enable bit 8
        #endif
		
        mStar_WriteByte(RAMFA, addr);
        while (num--)
        {
            mStar_WriteByte(RAMFD, *(fontPtr++));
        }
		
        #if Enable_CascadeMode
	  Osd_SetOsdEngine(0);
	 #else
         mStar_WriteByte(OSDDF, 0); //enable bit 8
        #endif
    }

    #if !defined TSUMXXNX && !defined TSUMXXQX && !defined TSUMXXT		//110311 Modify
    void Osd_LoadColor(BYTE *colorPtr, BYTE num)
    {
        mStar_WriteByte(OSDRA, CLR0R);
        mStar_WriteTBL(OSDRD, colorPtr, num, 0);
    }
    #endif

void Osd_Load256Color(BYTE addr, BYTE *colorPtr, WORD num)
{
    #if WaitForDataBlanking   //zjb20071219
    mStar_WaitForDataBlanking();
    #endif
    #if 0
    WORD i;

    //mStar_WriteByte(0x9F, 0x80);
   
    mStar_WriteByte(0xAC, addr);//yuhjinn
    for(i=0;i<num;i++)
    {
        mStar_WriteByte(0xAD, *colorPtr);
        colorPtr++;
        mStar_WriteByte(0xAD, *colorPtr);
        colorPtr++;
    }

    //mStar_WriteByte(0x9F, 0x00);
    #else
    mStar_WriteByte(0xAC, addr);
    mStar_WriteTBL(0xAD, colorPtr, num, 0);
    #endif
}


void Osd_SetTextMonoColor(BYTE foreColor, BYTE backColor)
{
    OsdFontColor = (foreColor &0xF) << 4 | (backColor &0xF);
}
void Osd_SetTextColor(BYTE foreColor, BYTE backColor)
{
    // OsdFontColor=(foreColor&0xF)<<4|(backColor&0xF);
    OsdFontColor = (foreColor &0x0) | (backColor &0xFF);
}

#if 0
void Osd_SetText4Color(color)
{
    // if (color&BIT7) //8 color
    OsdFontColor = (color &0x3f) << 2;
    //  else //4 color
    //  OsdFontColor=(color&0x3f)<<2;
}
void Osd_SetText8Color(color)
{
    // if (color&BIT7) //8 color
    OsdFontColor = (color &0x3f) << 3;
    //  else //4 color
    //  OsdFontColor=(color&0x3f)<<2;
}
#else

void Osd_Set256TextColor(BYTE foreColor, BYTE backColor)
{
   if(backColor==Color_2)
      OsdFontColor=foreColor<<1;
   else if(backColor==Color_4)
      OsdFontColor=foreColor<<2;
   else if(backColor==Color_8)
      OsdFontColor=foreColor<<3;
   else if(backColor==Color_2T)
      OsdFontColor=foreColor<<1|0x01;
   else if(backColor==Color_4T)
      OsdFontColor=foreColor<<2|0x01;
   else// (backColor==Color_8T)
      OsdFontColor=foreColor<<3|0x01;
}
#endif
#if 0
void Osd_DrawStr(BYTE xPos, BYTE yPos, BYTE *str)
{
    WORD tempValue;
    BYTE drawCode = TRUE;
    if (yPos &BIT7)
    {
        drawCode = FALSE;
    }
    yPos &= 0x3F; //~(BIT7|BIT6);
    #define dispChar    xPos
    #define dispPtr     yPos
    tempValue = (WORD)yPos *OsdWindowWidth + xPos; // get real address
    dispPtr = 0;
    mStar_WriteWord(DISPAA_H, tempValue);
    while (dispChar = *(str + dispPtr))
    // display string font index
    {
        mStar_WriteByte(DISPAD, OsdFontColor);
        dispPtr++;
    }
    if (drawCode == TRUE)
    {
        dispPtr = 0;
        mStar_WriteWord(DISPCA_H, tempValue);
        while (dispChar = *(str + dispPtr))
        // display string font index
        {
            mStar_WriteByte(DISPCD, dispChar);//mStar_WriteByte(DISPCD, ASCII2FONT[dispChar]);
            dispPtr++;
        }
    }
    #undef dispChar
    #undef dispPtr
}
#endif
//Character Direct Write DisplayCode
void Osd_DrawCharDirect(BYTE xPos, BYTE yPos, BYTE ch)
{
    WORD tempValue;
    tempValue = (WORD)yPos *OsdWindowWidth + xPos; // get real address
    mStar_WriteWord(DISPAA_H, tempValue); // display font attribute
    mStar_WriteByte(DISPAD, OsdFontColor);
    mStar_WriteWord(DISPCA_H, tempValue); // dispaly font code
    mStar_WriteByte(DISPCD, ch);
}



void Osd_DrawContinuesChar(BYTE xPos, BYTE yPos, BYTE ch, BYTE ucNum)
{	WORD tempValue;
	BYTE i;

	tempValue=(WORD)yPos*OsdWindowWidth+xPos; // get real address
	mStar_WriteWord(DISPAA_H, tempValue);
	for(i=0;i<ucNum;i++)
    { mStar_WriteByte(DISPAD, OsdFontColor);
    }

	mStar_WriteWord(DISPCA_H, tempValue);
	for(i=0;i<ucNum;i++)
	{ mStar_WriteByte(DISPCD, ch);
	}
}


void Osd_DrawPropStr(BYTE xPos, BYTE yPos, BYTE *str)
{
    WORD tempValue;
    BYTE drawCode = TRUE, drawCode1 = FALSE;
    if (yPos &BIT7)
    {
        drawCode = FALSE;
    }
    if (yPos &BIT6)
    //set center
    {
        drawCode1 = TRUE;
    }
    yPos &= 0x3F; //~(BIT7|BIT6);
    #define dispChar    xPos
    #define dispPtr     yPos
    if (yPos == 2 || xPos == 0)
    // just for title
    {
        //20070403
        tempValue = (WORD)yPos *OsdWindowWidth + (OsdWindowWidth - *(str + 1)) / 2+1; //tempValue = ( WORD ) yPos * OsdWindowWidth + OsdWindowWidth/2;
        drawCode1 = 1;
    }
    else
        tempValue = (WORD)yPos *OsdWindowWidth + xPos ;
    // get real address
    dispPtr = 0;
    mStar_WriteWord(DISPAA_H, tempValue);
    while (dispPtr++ < *(str + 1))
    {
        mStar_WriteByte(DISPAD, OsdFontColor);
    }
    if (drawCode1)
    {
        BYTE tlen;
        if (yPos == 2 || xPos == 0)
        {
            //tempValue =tempValue; 	//tempValue =tempValue -dispPtr/2+1 ;		//20070403
        }
        else
        {
            tlen = (OsdWindowWidth - xPos - dispPtr) / 2 -1; // get center position
            tempValue += tlen;
        }
    }
    if (drawCode == TRUE)
    {
        dispPtr = 0;
        mStar_WriteWord(DISPCA_H, tempValue);
        while (dispPtr < *(str + 1))
        {
            mStar_WriteByte(DISPCD, (*str) + dispPtr);
            dispPtr++;
            //printMsg("Draw Porational");
        }
    }
    #undef dispChar
    #undef dispPtr
}

void Osd_DrawRealStr(BYTE xPos, BYTE yPos, BYTE *str)
{
    WORD tempValue;
    BYTE drawCode = TRUE;
    BYTE y;
    if (yPos &BIT7)
    {
        drawCode = FALSE;
    }
    yPos &= 0x3F; //~(BIT7|BIT6);
    y = yPos;
    #define dispChar    xPos
    #define dispPtr     yPos
    tempValue = (WORD)yPos *OsdWindowWidth + xPos; // get real address
    dispPtr = 0;
    mStar_WriteWord(DISPAA_H, tempValue);
    while (*(str + dispPtr))
    // display string font index
    {
        mStar_WriteByte(DISPAD, OsdFontColor);
        dispPtr++;
    }
    if (drawCode == TRUE)
    {
        dispPtr = 0;
        mStar_WriteWord(DISPCA_H, tempValue);
        while (dispChar = *(str + dispPtr))
        // display string font index
        {
            mStar_WriteByte(DISPCD, dispChar);
            dispPtr++;
        }
    }
    #undef dispChar
    #undef dispPtr
}

void DrawNum(BYTE xPos, BYTE yPos, char len, WORD value) //int value)
{
    char _minus = 0;
    BYTE ZeroStart;

    if (MenuPageIndex == FactoryMenu
    #if EnablePanelServiceMenu
	||MenuPageIndex == AdjustPanelMenu
    #endif
	)
    {
        ZeroStart = FACTORY_NUM_START;//0X03
    }
    else
    {
        ZeroStart = NUM_START;//0x20;//0X1d;
    }
    if (value < 0)
    {
        value = 0-value;
        _minus = 1;
    }
    if (value == 0)
    //Osd_DrawCharDirect(xPos+(len--), yPos, (BYTE)Num2ASCII(0));
    {
        Osd_DrawCharDirect(xPos + (len--), yPos, (BYTE)(value + ZeroStart));
    }
    else
    {
        while (value && len)
        // translate integer to string
        {
            Osd_DrawCharDirect(xPos + (len--), yPos, (BYTE)((value % 10) + ZeroStart));
            value /= 10;
        }
    }
    if (_minus && len >= 0)
    // add sign
    {
        Osd_DrawCharDirect(xPos + (len--), yPos, (BYTE)'-');
    }
    _minus = (_minus) ? (0): (1);
    for (; _minus <= len; _minus++)
    {
        Osd_DrawCharDirect(xPos + _minus, yPos, (BYTE)0x01);
    }
}
void Osd_DrawNum(BYTE xPos, BYTE yPos, int value)
{
    DrawNum(xPos, yPos, 3, value);
}

void Osd_Draw4Num(BYTE xPos, BYTE yPos, int value)
{
    DrawNum(xPos, yPos, 5, value);
}

//===================================================================================
#define GuageFontStart  GuageFontStartAddr // 0X16
#define GuageFont0_4    GuageFontStartAddr+1 // 0X17
#define GuageFont1_4    GuageFontStartAddr+2// 0X18
#define GuageFont2_4    GuageFontStartAddr+3 // 0X19
#define GuageFont3_4    GuageFontStartAddr+4 // 0X1A
#define GuageFont4_4    GuageFontStartAddr+5// 0X1B
#define GuageFontEnd    GuageFontStartAddr+6// 0X1C

void Osd_DrawGuage(BYTE xPos, BYTE yPos, BYTE length, BYTE value)
{
    BYTE leadPtr;
    WORD tempValue;
	
#if 0//DEBUG_ENABLE
printData("value      =====%d", value);
#endif
    Osd_DrawCharDirect(xPos - 1, yPos, GuageFontStartAddr+7);
    //  Osd_DrawChar(xPos+length+1, yPos, '+');
    //  Osd_DrawChar(xPos, yPos, GuageFontStart);
    //  Osd_DrawChar(xPos+length-1, yPos, GuageFontEnd);
    Osd_DrawCharDirect(xPos, yPos, GuageFontStart);
    Osd_DrawCharDirect(xPos + length - 1, yPos, GuageFontEnd);
    length -= 2;
    leadPtr = (WORD)value *length / 10; // resver 0.1
    #define i   value
    tempValue = (WORD)yPos *OsdWindowWidth + (xPos + 1); // get real address
    mStar_WriteWord(DISPAA_H, tempValue); // display font attribute
    mStar_WriteWord(DISPCA_H, tempValue); // dispaly font code
    //  printData("lead %d", leadPtr);
    //  printData("value %d", value);
    tempValue = leadPtr / 10;
    if (tempValue == length && value < 100)
    {
        tempValue--;
    }
    //  for (i=1; i<=leadPtr/10; i++) // fill font with value
    for (i = 1; i <= tempValue; i++)
    // fill font with value
    {
        //mStar_WriteByte(DISPCD, ASCII2FONT[GuageFont4_4]);
        mStar_WriteByte(DISPCD, GuageFont4_4);
        mStar_WriteByte(DISPAD, OsdFontColor);
    }
    i = i + 1;
    if (i <= length)
    // fill font without value
    {
        tempValue = (WORD)yPos *OsdWindowWidth + (xPos + i); // get real address
        mStar_WriteWord(DISPAA_H, tempValue); // display font attribute
        mStar_WriteWord(DISPCA_H, tempValue); // dispaly font code
        //  for (i=leadPtr/10+2; i<=length; i++)
        for (; i <= length; i++)
        {
            // mStar_WriteByte(DISPCD, ASCII2FONT[GuageFont0_4]);
            mStar_WriteByte(DISPCD, GuageFont0_4);
            mStar_WriteByte(DISPAD, OsdFontColor);
        }
    }
    #undef i
    if (leadPtr < length *10)
    {
        value = leadPtr % 10;
        xPos += leadPtr / 10+1;
        leadPtr = ((WORD)value *12+5) / 10;
        if (leadPtr < 3 && value)
        {
            leadPtr = 3;
        }
        Osd_DrawCharDirect(xPos, yPos, GuageFont0_4 + leadPtr / 3);
    }
}
void Osd_Show(void)
{ 	BYTE regByte;
#if OSD_PIVOT_EN	//071226
	if(OSDRotateFlag)
  	regByte=(DoubleXSize<<4)|(DoubleYSize<<6)|MWIN_B|Rotate;
       else
#endif	   	
  	regByte=(DoubleXSize<<4)|(DoubleYSize<<6)|MWIN_B;

#if Enable_OsdEngine1
	if(MenuPageIndex!=LogoMenu &&  MenuPageIndex!=FactoryMenu
		#if EnablePanelServiceMenu
		&&  MenuPageIndex!=AdjustPanelMenu
		#endif
		)//MainMenu && MenuPageIndex<=InformationMenu)
	{
	Osd_SetOsdEngine(1);
  	mStar_WriteOSDByte(IOSDC1, regByte);
	Osd_SetOsdEngine(0);
	}
#endif
  	mStar_WriteOSDByte(IOSDC1, regByte);
}


void Osd_Hide(void)
{
#if Enable_OsdEngine1
	Osd_SetOsdEngine(1);
	mStar_WriteOSDByte(IOSDC1, 0x00);
	Osd_SetOsdEngine(0);
#endif
    mStar_WriteOSDByte(IOSDC1, 0x00);

}
void mStar_WriteOSDByte(BYTE address, BYTE value)
{
    mStar_WriteByte(OSDRA, address);
    mStar_WriteByte(OSDRD, value);
}

BYTE mStar_ReadOSDByte(BYTE address)
{
    mStar_WriteByte(OSDRA, address);
    return mStar_ReadByte(OSDRD);
}
#if OSD_MOVE_BY_PIXL
void mStar_WriteOSDByteMask(BYTE ucReg, BYTE ucValue, BYTE ucMask)
{
    BYTE uc;
    
    uc = mStar_ReadOSDByte(ucReg) & ~ucMask;
    mStar_WriteOSDByte(ucReg, (uc | (ucValue & ucMask)));
}
#endif

// ============================
// Draw Hex for Debug
BYTE Hex2ASCII(BYTE Num)
{
    return (Num + 3); //(Num<10)?(Num+3):(Num+3);   //3// 3 -->0   10+3-->A
}
void DrawHex(BYTE xPos, BYTE yPos, BYTE value)
{
    char len=3;
    while ( len >0 )
    {
        Osd_DrawCharDirect(xPos + (len--), yPos, (BYTE)Hex2ASCII(value % 0x10));
        value /= 0x10;
	 if(len==1)
	 	break;
    }
    Osd_DrawCharDirect(xPos + (len--), yPos, 62);//Draw "x"
    Osd_DrawCharDirect(xPos + (len--) , yPos, 3);//Draw "0"
/*    if (len > 0)
    {
        for (value = 0; value <= len; value++)
        //Osd_DrawChar(xPos+value, yPos, (BYTE)'0');
        {
            Osd_DrawCharDirect(xPos + value, yPos, (BYTE)3);
        }
    }
*/
}



void Osd_DrawHex(BYTE xPos, BYTE yPos, BYTE value)
{
    DrawHex(xPos, yPos, value);
}

#if DISPLAY_CHECK_VOL

void DrawHex1(BYTE xPos, BYTE yPos, WORD value)
{ 	char len=3;
 
  	while (value && len>=0)
   { 	Osd_DrawCharDirect(xPos+(len--), yPos, (BYTE)Hex2ASCII(value%0x10));
     	value/=0x10;
    }
  	if (len>0)
    	for (value=0; value<=len; value++)
      		Osd_DrawCharDirect(xPos+value, yPos, (BYTE)'0');
}
//*/
void Osd_DrawCheckSUM(BYTE xPos, BYTE yPos, WORD value)
{
	DrawHex1(xPos, yPos, value);
}
#endif


#if HotExpansionAdjust
void Osd_LoadExpansionPalette(void)
{
 Osd_Load256Color(32, (BYTE *)&tExpansionColorPalette256, sizeof(tExpansionColorPalette256));   //HighlightMenuPageIndex
}
#endif
void Osd_LoadDCROffPalette(void)
{
 Osd_Load256Color(225, (BYTE *)&tDCROffColorPalette256, sizeof(tDCROffColorPalette256));   //HighlightMenuPageIndex
}
void Osd_LoadOriginalPalette(void)
{
 Osd_Load256Color(225, (BYTE *)&tOriginalColorPalette256, sizeof(tOriginalColorPalette256));   //HighlightMenuPageIndex
}

#if (DisplayLogo!=NoBrand)
void Osd_LoadLogoFontCP(void)
{
    MonoColorSetting();
    #if (DisplayLogo==AcerLOGO)
	mStar_LoadCompressedFont(1,    &tOSDLogoFont, 255);

	mStar_WriteByte(0xAE, mStar_ReadByte(0xAE)|BIT7);
  	mStar_LoadCompressedFont(1,    &tOSDLogoFont_1, 55);
  	mStar_WriteByte(0xAE, mStar_ReadByte(0xAE)&(~BIT7));
    #else
    mStar_LoadCompressedFont(1, &tOSDLogoFont,0); // load osd fonts
    #endif

}
#endif
void Osd_LoadFacoryFontCP(void)
{
    MonoColorSetting();
    mStar_LoadCompressedFont(1, &tFactoryFont0,0); // load osd fonts

}

void Osd_LoadMainFactoryFont(void)
{
	    mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Factory,0); // load osd fonts
}

#if DisplayLogo!=NoBrand
void LoadMainSmallLogoFont(void)
{
#if Enable_OsdEngine1
	//Osd_SetOsdEngine(1);
       mStar_LoadCompressedFont(Frame2ColorAddr, &tMainSmallLogo,0);
	//Osd_SetOsdEngine(0);
#else
#if DisplayLogo == EnvisionLOGO
       if(MenuPageIndex==MainMenu)
       	mStar_LoadCompressedFont(Frame2ColorAddr1, &tMainSmallLogo,0);
	else
#endif		
       	mStar_LoadCompressedFont(Frame2ColorAddr, &tMainSmallLogo,0);		
#endif
}
#endif
void LoadCommonFont1(void)
{

#if Enable_OsdEngine1
	Osd_SetOsdEngine(1);
	mStar_LoadCompressedFont(1, &tOSDSTDFont0,0); // load osd fonts

       //mStar_LoadCompressedFont(Frame2ColorAddr, &tMainSmallLogo,0);
	Osd_SetOsdEngine(0);
#else
       //mStar_LoadCompressedFont(Frame2ColorAddr, &tMainSmallLogo,0);

#endif
    mStar_LoadCompressedFont(1, &tOSDSTDFont0,0); // load osd fonts
 	if(MenuPageIndex == MainMenu)
     {
 		LoadMainTitleFont(UserPrefLastMenuIndex);
     }
     else
     {
		mStar_LoadCompressedFont(CommonColorAddr, &tOSDSTDFont1,0); // load osd fonts
     }

}

void LoadCommonFont(void)
{
#if OSD_MOVE_BY_PIXL
    mStar_WriteOSDByteMask(OSD_10, 0x84, ~BIT5);
#else
    mStar_WriteOSDByte(OSD_10, 0x84);
#endif

#if 1//Enable_OsdEngine1
    mStar_WriteOSDByte(IOSDC2, 0x83); //0x83); // color bit setting
#else
    mStar_WriteOSDByte(IOSDC2, 0x03); //0x83); // color bit setting
#endif
    mStar_WriteOSDByte(OSDCFA, Start4ColorAddr); // 4 color font RAM start address
    mStar_WriteOSDByte(OSD8CFFA, Start8ColorAddr); // 8 color font RAM start address
#if !Enable_OsdEngine1
    mStar_WriteOSDByte(OSDCFHA, 0x80); // 8 color font
#endif


    LoadCommonFont1();
    Osd_Load256Color(0, &tOSDColorPalette256, sizeof(tOSDColorPalette256)); // load osd color
#if Enable_OsdEngine1
    Osd_SetOsdEngine(1);
    #if OSD_MOVE_BY_PIXL
    mStar_WriteOSDByteMask(OSD_10, 0x86, ~BIT5);
    #else
    mStar_WriteOSDByte(OSD_10, 0x86);
    #endif
    mStar_WriteOSDByte(IOSDC2, 0x83); //0x83); // color bit setting
    mStar_WriteOSDByte(OSDCFA, Frame4ColorAddr); // 4 color font RAM start address
    mStar_WriteOSDByte(OSD8CFFA, Frame8ColorAddr); // 8 color font RAM start address

    Osd_Load256Color(0, &tOSDColorPalette256a, sizeof(tOSDColorPalette256a)); // load osd color
    Osd_SetOsdEngine(0);
#endif
}

void Osd_LoadCompressColorFont(BYTE addr, BYTE *fontPtr, WORD *mapPtr, BYTE count)
{
  mStar_WriteByte(RAMFA, addr);
  LoadCompressColorFont(fontPtr, mapPtr, count);
}
#if DisplayEnergyStar
void OSD_LoadEnergyStarFont(void)
{
      mStar_WriteOSDByte(IOSDC2, 0x83);
      mStar_WriteOSDByte(OSDCFA, 0xFF);
      mStar_LoadCompressedFont(HotKey2ColorAddr, &tEnergyStar2ColorFont,0);

}
#endif
void OSD_LoadStandardEcoIconFont(void)
{
      mStar_LoadCompressedFont(HotKey2ColorAddr, &tStandardEco2ColorFont,0);

}
void OSD_LoadSportEcoIconFont(void)
{
      mStar_LoadCompressedFont(HotKey2ColorAddr, &tSportEco2ColorFont,0);

}

void OSD_LoadTextEcoIconFont(void)
{
       mStar_LoadCompressedFont(HotKey2ColorAddr, &tTextEco2ColorFont,0); // load osd fonts
	Osd_LoadCompressColorFont(HotKey4ColorAddr, &tTextEco4ColorFont, NULL, 11); // load osd fonts

}

void OSD_LoadMovieEcoIconFont(void)
{

	Osd_LoadCompressColorFont(HotKey4ColorAddr, &tMovieEco4ColorFont, NULL, 15); // load osd fonts

}

void OSD_LoadInternetEcoIconFont(void)
{
	Osd_LoadCompressColorFont(HotKey4ColorAddr, &tInternetEco4ColorFont, NULL, 15); // load osd fonts


}
void OSD_LoadGameEcoIconFont(void)
{

	Osd_LoadCompressColorFont(HotKey4ColorAddr, &tGameEco4ColorFont, NULL, 11); // load osd fonts
       Osd_LoadFont(HotKey8ColorAddr, &tGameEco8ColorFont, sizeof(tGameEco8ColorFont)); // load osd fonts

}
#if AudioFunc
void OSD_LoadVolIconFont(void)
{
      mStar_LoadCompressedFont(HotKey2ColorAddr, &tVol2ColorFont,0);
      mStar_LoadCompressedFont(CommonColorAddr, &tOSDSTDFont1,0); // load osd fonts


}
#else
void LoadHotKeyConFont(void)
{
         mStar_WriteOSDByte(IOSDC2, 0x83); //0x83); // color bit setting
         mStar_WriteOSDByte(OSDCFA, HotKey4ColorAddr); // 4 color font RAM start address
     	  //mStar_LoadCompressedFont(HotKeyDCR2ColorAddr, &tDCR2ColorFont,0); // load osd fonts
	  Osd_LoadCompressColorFont(HotKey4ColorAddr, &tHotKeyCon4ColorFont, NULL, 15); // load osd fonts
         mStar_LoadCompressedFont(CommonColorAddr, &tOSDSTDFont1,0); // load osd fonts
}
#endif
#if EnableHotBrightness
void LoadHotKeyBriFont(void)
{
         mStar_WriteOSDByte(IOSDC2, 0x83); //0x83); // color bit setting
         mStar_WriteOSDByte(OSDCFA, Luma4FontAddr); // 4 color font RAM start address
     	  //mStar_LoadCompressedFont(HotKeyDCR2ColorAddr, &tDCR2ColorFont,0); // load osd fonts
	  Osd_LoadCompressColorFont(Luma4FontAddr, &tOSDLumaIcon4ColorFont, NULL, Luma4FontNum); // load osd fonts  tOSDLumaIcon4ColorFont
         mStar_LoadCompressedFont(CommonColorAddr, &tOSDSTDFont1,0); // load osd fonts
	  mStar_LoadCompressedFont(Luma2FontAddr, &tOSDLumaIcon2ColorFont,0); 

}
#endif
void LoadHotKeyDCRFont(void)
{
	  Osd_LoadDCROffPalette();
         mStar_WriteOSDByte(IOSDC2, 0x83); //0x83); // color bit setting
         mStar_WriteOSDByte(OSDCFA, HotKey4ColorAddr); // 4 color font RAM start address
     	  //mStar_LoadCompressedFont(HotKeyDCR2ColorAddr, &tDCR2ColorFont,0); // load osd fonts
	  Osd_LoadCompressColorFont(HotKey4ColorAddr, &tDCR_On4ColorFont, NULL, 15); // load osd fonts
	  Osd_LoadCompressColorFont(HotKey4ColorAddr+30, &tDCR_Off4ColorFont, NULL, 15); // load osd fonts
}
// 090416
#if Enable_Lightsensor
void LoadHotKeyiCareFont(void)
{
         mStar_WriteOSDByte(IOSDC2, 0x83); //0x83); // color bit setting
         mStar_WriteOSDByte(OSDCFA, HotKey4ColorAddr); // 4 color font RAM start address
	  Osd_LoadCompressColorFont(HotKey4ColorAddr, &tiCare4ColorFont, NULL, 15); // load osd fonts
}
#endif
#if HotExpansionAdjust
void LoadHotKeyExpansionFont(void)
{
	  Osd_LoadExpansionPalette();
      mStar_WriteOSDByte(IOSDC2, 0x83); //0x83); // color bit setting
      mStar_WriteOSDByte(OSDCFA, HotKey4ColorAddr); // 4 color font RAM start address
	  mStar_LoadCompressedFont(HotKey2ColorAddr, &tExpansion_Native2ColorFont,0); // load osd fonts
	  Osd_LoadCompressColorFont(HotKey4ColorAddr, &tExpansion_Native4ColorFont, NULL, Expansion_Native4FontNum); // load osd fonts
	  mStar_LoadCompressedFont(HotKey2ColorAddr+Expansion_Native2FontNum, &tExpansion_Full2ColorFont,0); // load osd fonts
	  Osd_LoadCompressColorFont(HotKey4ColorAddr+Expansion_Native4FontNum*2, &tExpansion_Full4ColorFont, NULL, Expansion_Full4FontNum); // load osd fonts

         //mStar_LoadCompressedFont(CommonColorAddr, &tOSDSTDFont1,0); // load osd fonts
}
#endif
void OSD_AutoIconFont(void)
{ 
	Osd_Load256Color(32, (BYTE *)&tAutoColorPalette256, sizeof(tAutoColorPalette256)); // load osd color
	mStar_WriteOSDByte(IOSDC2, 0x83); //0x83); // color bit setting
	mStar_WriteOSDByte(OSDCFA, HotKey4ColorAddr); // 4 color font RAM start address
	mStar_WriteOSDByte(OSD8CFFA, HotKey8ColorAddr); // 4 color font RAM start address
	mStar_WriteOSDByte(OSDCFHA, 0x00);
	mStar_LoadCompressedFont(HotKey2ColorAddr, &tAuto2ColorFont,0); // load osd fonts
	Osd_LoadCompressColorFont(HotKey4ColorAddr, &tAuto4ColorFont, NULL, 1); // load osd fonts
	Osd_LoadFont(HotKey8ColorAddr, &tAuto8ColorFont, sizeof(tAuto8ColorFont)); // load osd fonts
}
void Osd_LoadMainMenuIconFont(void)
{


// 2 color
      mStar_LoadCompressedFont(Luma2FontAddr, &tOSDLumaIcon2ColorFont,0); // load osd fonts
      mStar_LoadCompressedFont(Image2FontAddr, &tOSDImageIcon2ColorFont,0); // load osd fonts
      mStar_LoadCompressedFont(Colortemp2FontAddr, &tOSDColortempIcon2ColorFont,0); // load osd fonts
      mStar_LoadCompressedFont(Picture2FontAddr, &tOSDPictureIcon2ColorFont,0); // load osd fonts
 	mStar_LoadCompressedFont(OsdSetup2FontAddr, &tOSDOsdIcon2ColorFont,0); // load osd fonts
 	mStar_LoadCompressedFont(Extra2FontAddr, &tOSDExtraIcon2ColorFont,0); // load osd fonts
      mStar_LoadCompressedFont(Reset2FontAddr, &tOSDResetIcon2ColorFont,0); // load osd fonts
 	mStar_LoadCompressedFont(Exit2FontAddr, &tOSDExitIcon2ColorFont,0); // load osd fonts

// 4 color

	Osd_LoadCompressColorFont(Luma4FontAddr, &tOSDLumaIcon4ColorFont, NULL, Luma4FontNum); // load osd fonts
	Osd_LoadCompressColorFont(Image4FontAddr, &tOSDImageIcon4ColorFont, NULL, Image4FontNum); // load osd fonts
	Osd_LoadCompressColorFont(Colortemp4FontAddr, &tOSDColortempIcon4ColorFont, NULL, Colortemp4FontNum); // load osd fonts
	Osd_LoadCompressColorFont(Color4FontAddr, &tOSDColorIcon4ColorFont, NULL, Color4FontNum); // load osd fonts
	Osd_LoadCompressColorFont(Picture4FontAddr, &tOSDPictureIcon4ColorFont, NULL, Picture4FontNum); // load osd fonts
	Osd_LoadCompressColorFont(OsdSetup4FontAddr, &tOSDOsdSetupIcon4ColorFont, NULL, OsdSetup4FontNum); // load osd fonts
 	Osd_LoadCompressColorFont(Extra4FontAddr, &tOSDExtraIcon4ColorFont, NULL, Extra4FontNum); // load osd fonts
#if Enable_OsdEngine1
 	Osd_LoadCompressColorFont(Reset4FontAddr, &tOSDResetIcon4ColorFont, NULL, Reset4FontNum); // load osd fonts
 	Osd_LoadCompressColorFont(Exit4FontAddr, &tOSDExitIcon4ColorFont, NULL, Exit4FontNum); // load osd fonts
#else
	#if Enable_CascadeMode
       Osd_SetOsdEngine(1);
	#else
       mStar_WriteByte(OSDDF, RAMFA8_B);
	#endif
       Osd_LoadCompressColorFont(Reset4FontAddr, &tOSDResetIcon4ColorFont, NULL, Reset4FontNum); // load osd fonts
 	Osd_LoadCompressColorFont(Exit4FontAddr, &tOSDExitIcon4ColorFont, NULL, Exit4FontNum); // load osd fonts
	#if Enable_CascadeMode
       Osd_SetOsdEngine(0);
	#else
       mStar_WriteByte(OSDDF, 0);
#endif
#endif


// 8 color
#if Enable_OsdEngine1
	Osd_LoadFont(Image8FontAddr, &tOSDImageIcon8ColorFont, sizeof(tOSDImageIcon8ColorFont)); // load osd fonts
	Osd_LoadFont(Color8FontAddr, &tOSDColorIcon8ColorFont, sizeof(tOSDColorIcon8ColorFont)); // load osd fonts
 	Osd_LoadFont(OsdSetup8FontAddr, &tOSDOsdSetupIcon8ColorFont, sizeof(tOSDOsdSetupIcon8ColorFont)); // load osd fonts

#else
	Osd_Load291Font(Image8FontAddr, &tOSDImageIcon8ColorFont, sizeof(tOSDImageIcon8ColorFont)); // load osd fonts
	Osd_Load291Font(Color8FontAddr, &tOSDColorIcon8ColorFont, sizeof(tOSDColorIcon8ColorFont)); // load osd fonts
 	Osd_Load291Font(OsdSetup8FontAddr, &tOSDOsdSetupIcon8ColorFont, sizeof(tOSDOsdSetupIcon8ColorFont)); // load osd fonts
#endif
	Osd_Load291Font(Picture8FontAddr, &tOSDPictureIcon8ColorFont, sizeof(tOSDPictureIcon8ColorFont)); // load osd fonts
	Osd_Load291Font(Reset8FontAddr, &tOSDResetIcon8ColorFont, sizeof(tOSDResetIcon8ColorFont)); // load osd fonts
	Osd_Load291Font(Exit8FontAddr, &tOSDExitIcon8ColorFont, sizeof(tOSDExitIcon8ColorFont)); // load osd fonts
	Osd_Load291Font(Colortemp8FontAddr, &tOSDColortempIcon8ColorFont, sizeof(tOSDColortempIcon8ColorFont)); // load osd fonts
#if Enable_CascadeMode
       mStar_WriteOSDByte(OSDCFHA, 0x80); // 8 color font

	mStar_WriteOSDByte(OSD_Reg4A, Font_Cascade|0x08);
#endif
}


#if Enable_OsdEngine1

    BYTE code tMainIconTbl[][15][3]=
    {
		{ // luma
            {2, Luma4Color, SpaceFont},
            {2, Luma4Color, Luma4FontAddr+0},
			{2, Luma4Color, Luma4FontAddr+2},
			{2, Luma4Color, Luma4FontAddr+4},
            {2, Luma4Color, SpaceFont},

			{2, Luma4Color, Luma2FontAddr+0},
			{2, Luma4Color, Luma4FontAddr+6},
			{2, Luma4Color, Luma4FontAddr+8},
			{2, Luma4Color, Luma4FontAddr+10},
            {2, Luma4Color, Luma2FontAddr+1},

            {2, Luma4Color, SpaceFont},
			{2, Luma4Color, Luma4FontAddr+12},
			{2, Luma4Color, Luma4FontAddr+14},
			{2, Luma4Color, Luma4FontAddr+16},
            {2, Luma4Color, SpaceFont},
		},
		{ // image
            {4, Image8Color, Image4FontAddr+0},
            {4, Image8Color, Image4FontAddr+2},
            {4, Image8Color, Image4FontAddr+4},
            {4, Image8Color, Image8FontAddr+0},
            {4, Image8Color, Image4FontAddr+6},

            {4, Image8Color, Image4FontAddr+8},
            {4, Image8Color, Image4FontAddr+10},
            {0x12, Image4Color1b, Image4FontAddr+12}, // skip changed color while selected
            {0x12, Image4Color1b, Image4FontAddr+14}, // skip changed color while selected
            {2, Image4Color1a, Image4FontAddr+16},

			{4, Image8Color, Image4FontAddr+18},
            {2, Image4Color1a, Image2FontAddr+0},
            {2, Image4Color1a, Image4FontAddr+20},
            {2, Image4Color1a, Image4FontAddr+22},
            {2, Image4Color1a, Image4FontAddr+24},
		},

        { // colortemp
            {1, Comm2Color, SpaceFont},
            {4, Colortemp8Color, Colortemp4FontAddr+0},// red
            {4, Colortemp8Color, Colortemp4FontAddr+2},
            {4, Colortemp8Color, Colortemp4FontAddr+4},
            {1, Comm2Color, SpaceFont},

            {2, Colortemp4Color2a, Colortemp4FontAddr+6},// green
            {2, Colortemp4Color2a, Colortemp4FontAddr+8},
            {0x84, Colortemp8Color, Colortemp8FontAddr+0},
            {2, Colortemp4Color2b, Colortemp4FontAddr+10},//blue
            {2, Colortemp4Color2b,  Colortemp4FontAddr+12 },

			{2, Colortemp4Color2a, Colortemp2FontAddr+0}, // green+blue
            {2, Colortemp4Color2a, Colortemp2FontAddr+1}, // green
            {2, Colortemp4Color1a, Colortemp4FontAddr+14}, // green+blue
            {2, Colortemp4Color2b, Colortemp2FontAddr+2}, //blue
            {2, Colortemp4Color2b, Colortemp2FontAddr+3}, // green+blue
        },

        { // colorboost
            {4, ColorBoost8Color, Color4FontAddr+0},
            {4, ColorBoost8Color, Color4FontAddr+2},
            {2, ColorBoost4Color1a, Color4FontAddr+4},
            {4, ColorBoost8Color, Color4FontAddr+6},
            {1, Comm2Color, SpaceFont},

            {4, ColorBoost8Color, Color4FontAddr+8},
            {4, ColorBoost8Color, Color4FontAddr+10},
            {4, ColorBoost8Color, Color8FontAddr+0},
            {4, ColorBoost8Color, Color8FontAddr+3},
			{2, ColorBoost4Color1a, Color4FontAddr+12},

            {4, ColorBoost8Color, Color4FontAddr+14},
            {4, ColorBoost8Color, Color4FontAddr+16},
            {1, Comm2Color, SpaceFont},
            {4, ColorBoost8Color, Color8FontAddr+6},
            {4, ColorBoost8Color, Color8FontAddr+9},
        },

        { // pictureboost
            {2, PictureBoost4Color1a, Picture4FontAddr+0},
            {2, PictureBoost4Color1a, Picture4FontAddr+2},
            {0x84, PictureBoost8Color, Picture8FontAddr+0},
            {4, PictureBoost8Color, Picture4FontAddr+4},
            {4, PictureBoost8Color, Picture4FontAddr+6},

            {0x84, PictureBoost8Color, Picture8FontAddr+3},
            {0x12, PictureBoost4Color2b, Picture4FontAddr+8},
            {0x12, PictureBoost4Color2b, Picture4FontAddr+10},
            {0x12, PictureBoost4Color2a, Picture4FontAddr+12},
            {4, PictureBoost8Color, Picture4FontAddr+14},

			{4, PictureBoost8Color, Picture4FontAddr+16},
            {4, PictureBoost8Color, Picture4FontAddr+18},
            {0x84, PictureBoost8Color, Picture8FontAddr+6},
            {2, PictureBoost4Color2c, Picture4FontAddr+20},
            {4, PictureBoost8Color, Picture2FontAddr+0},
        },
        { // osdsetup
            {4, OsdSetup8Color, OsdSetup4FontAddr+0},
            {4, OsdSetup8Color, OsdSetup4FontAddr+2},
            {4, OsdSetup8Color, OsdSetup4FontAddr+4},
            {4, OsdSetup8Color, OsdSetup4FontAddr+6},
            {1, Comm2Color, SpaceFont},

            {4, OsdSetup8Color, OsdSetup4FontAddr+8},
            {4, OsdSetup8Color, OsdSetup4FontAddr+10},
            {4, OsdSetup8Color, OsdSetup4FontAddr+12},
            {4, OsdSetup8Color, OsdSetup8FontAddr+0},
            {2, OsdSetup4Color, OsdSetup4FontAddr+14},

			{4, OsdSetup8Color, OsdSetup2FontAddr+0},
			{4, OsdSetup8Color, OsdSetup2FontAddr+1},
            {4, OsdSetup8Color, OsdSetup8FontAddr+3},
            {2, OsdSetup4Color, OsdSetup4FontAddr+16},
            {2, OsdSetup4Color, OsdSetup4FontAddr+18},
        },
        { // extra
            {2, Extra4Color1a, Extra4FontAddr+0},
            {2, Extra4Color1a, Extra4FontAddr+2},
            {2, Extra4Color1a, Extra4FontAddr+4},
            {2, Extra4Color1a, Extra4FontAddr+6},
            {2, Extra4Color1a, Extra4FontAddr+8},

            {2, Extra4Color1a, Extra2FontAddr+0},
            {2, Extra4Color1b, Extra4FontAddr+10},
            {2, Extra4Color1b, Extra4FontAddr+12},
            {2, Extra4Color1b, Extra4FontAddr+14},
            {1, Comm2Color, SpaceFont},

			{2, Extra4Color1b, Extra4FontAddr+16},
            {2, Extra4Color1b, Extra4FontAddr+18},
            {2, Extra4Color1b, Extra4FontAddr+20},
            {2, Extra4Color1a, Extra4FontAddr+22},
            {2, Extra4Color1a, Extra4FontAddr+24},
        },
        { // reset
            {4, Reset8Color, Reset4FontAddr+0},
            {4, Reset8Color, Reset4FontAddr+2},
            {4, Reset8Color, Reset4FontAddr+4},
            {4, Reset8Color, Reset4FontAddr+6},
            {4, Reset8Color, Reset2FontAddr+0},

            {0x84, Reset8Color, Reset8FontAddr+0},
            {0x84, Reset8Color, Reset8FontAddr+3},
            {1, Comm2Color, SpaceFont},
            {0x84, Reset8Color, Reset8FontAddr+6},
            {0x84, Reset8Color, Reset8FontAddr+9},

			{4, Reset8Color1a, Reset4FontAddr+8},
            {4, Reset8Color1a, Reset4FontAddr+10},
            {4, Reset8Color1a, Reset4FontAddr+12},
            {4, Reset8Color1a, Reset4FontAddr+14},
            {4, Reset8Color1a, Reset4FontAddr+16},
        },

        { // exit
            {4, Exit8Color, Exit4FontAddr+0},
            {4, Exit8Color, Exit4FontAddr+2},
            {2, Exit4Color1c, Exit4FontAddr+4},
            {0x84, Exit8Color, Exit8FontAddr+0},
            {0x84, Exit8Color, Exit8FontAddr+3},

            {4, Exit8Color, Exit4FontAddr+6},
            {4, Exit8Color, Exit4FontAddr+8},
            {0x12, Exit4Color1a, Exit4FontAddr+10},
            {0x12, Exit4Color1b, Exit4FontAddr+12},
            {2, Exit4Color1c, Exit4FontAddr+14},

			{1, Comm2Color, SpaceFont},
            {4, Exit8Color, Exit4FontAddr+16},
            {2, Exit4Color1c, Exit4FontAddr+18},
            {2, Exit4Color1c, Exit4FontAddr+20},
            {4, Exit8Color, Exit2FontAddr+0},
        },
    };

#else
    BYTE code tMainIconTbl[][15][3]=
    {
		{ // luma
            {2, Luma4Color, SpaceFont},
            {2, Luma4Color, Luma4FontAddr+0},
	     {2, Luma4Color, Luma4FontAddr+2},
	     {2, Luma4Color, Luma4FontAddr+4},
            {2, Luma4Color, SpaceFont},

	     {2, Luma4Color, Luma2FontAddr+0},
	     {2, Luma4Color, Luma4FontAddr+6},
	     {2, Luma4Color, Luma4FontAddr+8},
	     {2, Luma4Color, Luma4FontAddr+10},
            {2, Luma4Color, Luma2FontAddr+1},

            {2, Luma4Color, SpaceFont},
	     {2, Luma4Color, Luma4FontAddr+12},
	     {2, Luma4Color, Luma4FontAddr+14},
	     {2, Luma4Color, Luma4FontAddr+16},
            {2, Luma4Color, SpaceFont},
		},
		{ // image
            {4, Image8Color, Image4FontAddr+0},
            {4, Image8Color, Image4FontAddr+2},
            {4, Image8Color, Image4FontAddr+4},
            {0x84, Image8Color, Image8FontAddr+0},
            {4, Image8Color, Image4FontAddr+6},

            {4, Image8Color, Image4FontAddr+8},
            {4, Image8Color, Image4FontAddr+10},
            {0x12, Image4Color1b, Image4FontAddr+12}, // skip changed color while selected
            {0x12, Image4Color1a, Image4FontAddr+14}, // skip changed color while selected
            {2, Image4Color1a, Image4FontAddr+16},

	     {4, Image8Color, Image4FontAddr+18},
            {2, Image4Color1a, Image2FontAddr+0},
            {2, Image4Color1a, Image4FontAddr+20},
            {2, Image4Color1a, Image4FontAddr+22},
            {2, Image4Color1a, Image4FontAddr+24},
		},

        { // colortemp
            {1, Comm2Color, SpaceFont},
            {4, Colortemp8Color, Colortemp4FontAddr+0},// red
            {4, Colortemp8Color, Colortemp4FontAddr+2},
            {4, Colortemp8Color, Colortemp4FontAddr+4},
            {1, Comm2Color, SpaceFont},

            {2, Colortemp4Color2a, Colortemp4FontAddr+6},// green
            {2, Colortemp4Color2a, Colortemp4FontAddr+8},
            {0x84, Colortemp8Color, Colortemp8FontAddr+0},
            {2, Colortemp4Color2b, Colortemp4FontAddr+10},//blue
            {2, Colortemp4Color2b,  Colortemp4FontAddr+12 },

	     {2, Colortemp4Color2a, Colortemp2FontAddr+0}, // green+blue
            {2, Colortemp4Color2a, Colortemp2FontAddr+1}, // green
            {2, Colortemp4Color1a, Colortemp4FontAddr+14}, // green+blue
            {2, Colortemp4Color2b, Colortemp2FontAddr+2}, //blue
            {2, Colortemp4Color2b, Colortemp2FontAddr+3}, // green+blue
        },

        { // colorboost
            {4, ColorBoost8Color, Color4FontAddr+0},
            {4, ColorBoost8Color, Color4FontAddr+2},
            {2, ColorBoost4Color1a, Color4FontAddr+4},
            {4, ColorBoost8Color, Color4FontAddr+6},
            {1, Comm2Color, SpaceFont},

            {4, ColorBoost8Color, Color4FontAddr+8},
            {4, ColorBoost8Color, Color4FontAddr+10},
            {0x84, ColorBoost8Color, Color8FontAddr+0},
            {0x84, ColorBoost8Color, Color8FontAddr+3},
		{2, ColorBoost4Color1a, Color4FontAddr+12},

            {4, ColorBoost8Color, Color4FontAddr+14},
            {4, ColorBoost8Color, Color4FontAddr+16},
            {1, Comm2Color, SpaceFont},
            {0x84, ColorBoost8Color, Color8FontAddr+6},
            {0x84, ColorBoost8Color, Color8FontAddr+9},
        },

        { // pictureboost
            {2, PictureBoost4Color1a, Picture4FontAddr+0},
            {2, PictureBoost4Color1a, Picture4FontAddr+2},
            {0x84, PictureBoost8Color, Picture8FontAddr+0},
            {4, PictureBoost8Color, Picture4FontAddr+4},
            {4, PictureBoost8Color, Picture4FontAddr+6},

            {0x84, PictureBoost8Color, Picture8FontAddr+3},
            {0x12, PictureBoost4Color2b, Picture4FontAddr+8},
            {0x12, PictureBoost4Color2b, Picture4FontAddr+10},
            {0x12, PictureBoost4Color2a, Picture4FontAddr+12},
            {4, PictureBoost8Color, Picture4FontAddr+14},

	     {4, PictureBoost8Color, Picture4FontAddr+16},
            {4, PictureBoost8Color, Picture4FontAddr+18},
            {0x84, PictureBoost8Color, Picture8FontAddr+6},
            {2, PictureBoost4Color2c, Picture4FontAddr+20},
            {4, PictureBoost8Color, Picture2FontAddr+0},
        },
        { // osdsetup
            {4, OsdSetup8Color, OsdSetup4FontAddr+0},
            {4, OsdSetup8Color, OsdSetup4FontAddr+2},
            {4, OsdSetup8Color, OsdSetup4FontAddr+4},
            {4, OsdSetup8Color, OsdSetup4FontAddr+6},
            {1, Comm2Color, SpaceFont},

            {4, OsdSetup8Color, OsdSetup4FontAddr+8},
            {4, OsdSetup8Color, OsdSetup4FontAddr+10},
            {4, OsdSetup8Color, OsdSetup4FontAddr+12},
            {0x84, OsdSetup8Color, OsdSetup8FontAddr+0},
            {2, OsdSetup4Color, OsdSetup4FontAddr+14},

	     {4, OsdSetup8Color, OsdSetup2FontAddr+0},
	     {4, OsdSetup8Color, OsdSetup2FontAddr+1},
            {0x84, OsdSetup8Color, OsdSetup8FontAddr+3},
            {2, OsdSetup4Color, OsdSetup4FontAddr+16},
            {2, OsdSetup4Color, OsdSetup4FontAddr+18},
        },
        { // extra
            {2, Extra4Color1a, Extra4FontAddr+0},
            {2, Extra4Color1a, Extra4FontAddr+2},
            {2, Extra4Color1a, Extra4FontAddr+4},
            {2, Extra4Color1a, Extra4FontAddr+6},
            {2, Extra4Color1a, Extra4FontAddr+8},

            {2, Extra4Color1a, Extra2FontAddr+0},
            {2, Extra4Color1b, Extra4FontAddr+10},
            {2, Extra4Color1b, Extra4FontAddr+12},
            {2, Extra4Color1b, Extra4FontAddr+14},
            {1, Comm2Color, SpaceFont},

			{2, Extra4Color1b, Extra4FontAddr+16},
            {2, Extra4Color1b, Extra4FontAddr+18},
            {2, Extra4Color1b, Extra4FontAddr+20},
            {2, Extra4Color1a, Extra4FontAddr+22},
            {2, Extra4Color1a, Extra4FontAddr+24},
        },
        { // reset
            {0x84, Reset8Color, Reset4FontAddr+0},
            {0x84, Reset8Color, Reset4FontAddr+2},
            {0x84, Reset8Color, Reset4FontAddr+4},
            {0x84, Reset8Color, Reset4FontAddr+6},
            {4, Reset8Color, Reset2FontAddr+0},

            {0x84, Reset8Color, Reset8FontAddr+0},
            {0x84, Reset8Color, Reset8FontAddr+3},
            {1, Comm2Color, SpaceFont},
            {0x84, Reset8Color, Reset8FontAddr+6},
            {0x84, Reset8Color, Reset8FontAddr+9},

	     {0x84, Reset8Color1a, Reset4FontAddr+8},
            {0x84, Reset8Color1a, Reset4FontAddr+10},
            {0x84, Reset8Color1a, Reset4FontAddr+12},
            {0x84, Reset8Color1a, Reset4FontAddr+14},
            {0x84, Reset8Color1a, Reset4FontAddr+16},
        },

        { // exit
            {0x84, Exit8Color, Exit4FontAddr+0},
            {0x84, Exit8Color, Exit4FontAddr+2},
            {0x82, Exit4Color1c, Exit4FontAddr+4},//
            {0x84, Exit8Color, Exit8FontAddr+0},
            {0x84, Exit8Color, Exit8FontAddr+3},

            {0x84, Exit8Color, Exit4FontAddr+6},
            {0x84, Exit8Color, Exit4FontAddr+8},
            {0x92, Exit4Color1a, Exit4FontAddr+10},//
            {0x92, Exit4Color1b, Exit4FontAddr+12},
            {0x82, Exit4Color1c, Exit4FontAddr+14},

	     {1, Comm2Color, SpaceFont},
            {0x84, Exit8Color, Exit4FontAddr+16},
            {0x82, Exit4Color1c, Exit4FontAddr+18},//
            {0x82, Exit4Color1c, Exit4FontAddr+20},//
            {4, Exit8Color, Exit2FontAddr+0},
        },
    };
#endif

#if (EnableHotBrightness) // JueryWu 100127;
BYTE code tMainIconTbl2[][15][3]=
{
	{ // luma
		{2, Luma4Color, SpaceFont},
		{2, Luma4Color, Luma4FontAddr+0},
		{2, Luma4Color, Luma4FontAddr+2},
		{2, Luma4Color, Luma4FontAddr+4},
		{2, Luma4Color, SpaceFont},

		{2, Luma4Color, Luma2FontAddr},
		{2, Luma4Color, Luma4FontAddr+6},
		{2, Luma4Color, Luma4FontAddr+8},
		{2, Luma4Color, Luma4FontAddr+10},
		{2, Luma4Color, Luma2FontAddr+1},

		{2, Luma4Color, SpaceFont},
		{2, Luma4Color, Luma4FontAddr+12},
		{2, Luma4Color, Luma4FontAddr+14},
		{2, Luma4Color, Luma4FontAddr+16},
		{2, Luma4Color, SpaceFont},
	},
	
	{ // image
		{4, Image8Color, Image4FontAddr+0},
		{4, Image8Color, Image4FontAddr+2},
		{4, Image8Color, Image4FontAddr+4},
		{4, Image8Color, Image8FontAddr+0},
		{4, Image8Color, Image4FontAddr+6},

		{4, Image8Color, Image4FontAddr+8},
		{4, Image8Color, Image4FontAddr+10},
		{0x12, Image4Color1b, Image4FontAddr+12}, // skip changed color while selected
		{0x12, Image4Color1b, Image4FontAddr+14}, // skip changed color while selected
		{2, Image4Color1a, Image4FontAddr+16},

		{4, Image8Color, Image4FontAddr+18},
		{2, Image4Color1a, Image2FontAddr+0},
		{2, Image4Color1a, Image4FontAddr+20},
		{2, Image4Color1a, Image4FontAddr+22},
		{2, Image4Color1a, Image4FontAddr+24},
	},

	{ // colortemp
		{1, Comm2Color, SpaceFont},
		{4, Colortemp8Color, Colortemp4FontAddr+0},// red
		{4, Colortemp8Color, Colortemp4FontAddr+2},
		{4, Colortemp8Color, Colortemp4FontAddr+4},
		{1, Comm2Color, SpaceFont},

		{2, Colortemp4Color2a, Colortemp4FontAddr+6},// green
		{2, Colortemp4Color2a, Colortemp4FontAddr+8},
		{0x84, Colortemp8Color, Colortemp8FontAddr+0},
		{2, Colortemp4Color2b, Colortemp4FontAddr+10},//blue
		{2, Colortemp4Color2b,  Colortemp4FontAddr+12 },

		{2, Colortemp4Color2a, Colortemp2FontAddr+0}, // green+blue
		{2, Colortemp4Color2a, Colortemp2FontAddr+1}, // green
		{2, Colortemp4Color1a, Colortemp4FontAddr+14}, // green+blue
		{2, Colortemp4Color2b, Colortemp2FontAddr+2}, //blue
		{2, Colortemp4Color2b, Colortemp2FontAddr+3}, // green+blue
	},

	{ // colorboost
		{4, ColorBoost8Color, Color4FontAddr+0},
		{4, ColorBoost8Color, Color4FontAddr+2},
		{2, ColorBoost4Color1a, Color4FontAddr+4},
		{4, ColorBoost8Color, Color4FontAddr+6},
		{1, Comm2Color, SpaceFont},

		{4, ColorBoost8Color, Color4FontAddr+8},
		{4, ColorBoost8Color, Color4FontAddr+10},
		{4, ColorBoost8Color, Color8FontAddr+0},
		{4, ColorBoost8Color, Color8FontAddr+3},
		{2, ColorBoost4Color1a, Color4FontAddr+12},

		{4, ColorBoost8Color, Color4FontAddr+14},
		{4, ColorBoost8Color, Color4FontAddr+16},
		{1, Comm2Color, SpaceFont},
		{4, ColorBoost8Color, Color8FontAddr+6},
		{4, ColorBoost8Color, Color8FontAddr+9},
	},

	{ // pictureboost
		{2, PictureBoost4Color1a, Picture4FontAddr+0},
		{2, PictureBoost4Color1a, Picture4FontAddr+2},
		{0x84, PictureBoost8Color, Picture8FontAddr+0},
		{4, PictureBoost8Color, Picture4FontAddr+4},
		{4, PictureBoost8Color, Picture4FontAddr+6},

		{0x84, PictureBoost8Color, Picture8FontAddr+3},
		{0x12, PictureBoost4Color2b, Picture4FontAddr+8},
		{0x12, PictureBoost4Color2b, Picture4FontAddr+10},
		{0x12, PictureBoost4Color2a, Picture4FontAddr+12},
		{4, PictureBoost8Color, Picture4FontAddr+14},

		{4, PictureBoost8Color, Picture4FontAddr+16},
		{4, PictureBoost8Color, Picture4FontAddr+18},
		{0x84, PictureBoost8Color, Picture8FontAddr+6},
		{2, PictureBoost4Color2c, Picture4FontAddr+20},
		{4, PictureBoost8Color, Picture2FontAddr+0},
	},
	
	{ // osdsetup
		{4, OsdSetup8Color, OsdSetup4FontAddr+0},
		{4, OsdSetup8Color, OsdSetup4FontAddr+2},
		{4, OsdSetup8Color, OsdSetup4FontAddr+4},
		{4, OsdSetup8Color, OsdSetup4FontAddr+6},
		{1, Comm2Color, SpaceFont},

		{4, OsdSetup8Color, OsdSetup4FontAddr+8},
		{4, OsdSetup8Color, OsdSetup4FontAddr+10},
		{4, OsdSetup8Color, OsdSetup4FontAddr+12},
		{4, OsdSetup8Color, OsdSetup8FontAddr+0},
		{2, OsdSetup4Color, OsdSetup4FontAddr+14},

		{4, OsdSetup8Color, OsdSetup2FontAddr+0},
		{4, OsdSetup8Color, OsdSetup2FontAddr+1},
		{4, OsdSetup8Color, OsdSetup8FontAddr+3},
		{2, OsdSetup4Color, OsdSetup4FontAddr+16},
		{2, OsdSetup4Color, OsdSetup4FontAddr+18},
	},
	
	{ // extra
		{2, Extra4Color1a, Extra4FontAddr+0},
		{2, Extra4Color1a, Extra4FontAddr+2},
		{2, Extra4Color1a, Extra4FontAddr+4},
		{2, Extra4Color1a, Extra4FontAddr+6},
		{2, Extra4Color1a, Extra4FontAddr+8},

		{2, Extra4Color1a, Extra2FontAddr+0},
		{2, Extra4Color1b, Extra4FontAddr+10},
		{2, Extra4Color1b, Extra4FontAddr+12},
		{2, Extra4Color1b, Extra4FontAddr+14},
		{1, Comm2Color, SpaceFont},

		{2, Extra4Color1b, Extra4FontAddr+16},
		{2, Extra4Color1b, Extra4FontAddr+18},
		{2, Extra4Color1b, Extra4FontAddr+20},
		{2, Extra4Color1a, Extra4FontAddr+22},
		{2, Extra4Color1a, Extra4FontAddr+24},
	},
	
	{ // reset
		{4, Reset8Color, Reset4FontAddr+0},
		{4, Reset8Color, Reset4FontAddr+2},
		{4, Reset8Color, Reset4FontAddr+4},
		{4, Reset8Color, Reset4FontAddr+6},
		{4, Reset8Color, Reset2FontAddr+0},

		{0x84, Reset8Color, Reset8FontAddr+0},
		{0x84, Reset8Color, Reset8FontAddr+3},
		{1, Comm2Color, SpaceFont},
		{0x84, Reset8Color, Reset8FontAddr+6},
		{0x84, Reset8Color, Reset8FontAddr+9},

		{4, Reset8Color1a, Reset4FontAddr+8},
		{4, Reset8Color1a, Reset4FontAddr+10},
		{4, Reset8Color1a, Reset4FontAddr+12},
		{4, Reset8Color1a, Reset4FontAddr+14},
		{4, Reset8Color1a, Reset4FontAddr+16},
	},

	{ // exit
		{4, Exit8Color, Exit4FontAddr+0},
		{4, Exit8Color, Exit4FontAddr+2},
		{2, Exit4Color1c, Exit4FontAddr+4},
		{0x84, Exit8Color, Exit8FontAddr+0},
		{0x84, Exit8Color, Exit8FontAddr+3},

		{4, Exit8Color, Exit4FontAddr+6},
		{4, Exit8Color, Exit4FontAddr+8},
		{0x12, Exit4Color1a, Exit4FontAddr+10},
		{0x12, Exit4Color1b, Exit4FontAddr+12},
		{2, Exit4Color1c, Exit4FontAddr+14},

		{1, Comm2Color, SpaceFont},
		{4, Exit8Color, Exit4FontAddr+16},
		{2, Exit4Color1c, Exit4FontAddr+18},
		{2, Exit4Color1c, Exit4FontAddr+20},
		{4, Exit8Color, Exit2FontAddr+0},
	},
};
#endif

void Osd_DrawMainIcon(BYTE itemIndex, BYTE xpos, BYTE ypos, BYTE selected)
{
// font[icon index][icon size][icon attribute]
// [icon size]:
// 5x3 icon
// [icon attribute]:
//  0: font depth , to do shift
//  the bit7 => 1 : denote use 291 font or not
//  the bit4 => 1 : denote skip change font color
//  bit[2:0] => shift bit
//  1: use font color
//  2: font data
//
    BYTE i, j;


    for(j=0; j<3; j++) // y
    {
    	for(i=0; i<5; i++) // x
        {
        // check use 291 font
       if(tMainIconTbl[itemIndex][i+j*5][0] & 0x80 )
             	mStar_WriteByte(OSDDF, DISPCD8_B);


            // check use color depth
        	 if( (tMainIconTbl[itemIndex][i+j*5][0] & 0x04) >> 2  ) // for 8 color
		     {

                	Osd_Set256TextColor(tMainIconTbl[itemIndex][i+j*5][1]+ selected,Color_8);
                    #if Enable_OsdEngine1
			OsdFontColor = OsdFontColor+1*selected;
                    #endif
             }
             else // for 2/4 color
             {
             	if( (tMainIconTbl[itemIndex][i+j*5][0] & 0x10) >> 4  ) // for keep color
	             	Osd_SetTextColor(0, tMainIconTbl[itemIndex][i+j*5][1] );
                else
                     #if Enable_OsdEngine1
                	Osd_SetTextColor(0, tMainIconTbl[itemIndex][i+j*5][1] + (selected << tMainIconTbl[itemIndex][i+j*5][0]&0x0F)+1*selected);
                     #else
                 	Osd_SetTextColor(0, tMainIconTbl[itemIndex][i+j*5][1] + (selected << tMainIconTbl[itemIndex][i+j*5][0]&0x0F));
                     #endif
             }

            // write font
             Osd_DrawCharDirect(xpos+i, ypos+j, tMainIconTbl[itemIndex][i+j*5][2]);

            // check use 291 font
        	 if(tMainIconTbl[itemIndex][i+j*5][0] & 0x80)
                 mStar_WriteByte(OSDDF, 0);
		}
     }
}

//==============================================================================
//==============================================================================
//==============================================================================


#if HotExpansionAdjust
#define HotKeyExpansion4Color  4
#define HotKeyExpansion2Color  33
#endif
#define HotKey8Color  (14<<1|8) // for game , internet
#define HotKey8Color_b  ((13<<1)|1) // for game , internet
#define HotKey8Color_a   (14<<1) // for text, movie, standard, vol, sports
#define HotKey8Color_c   ((14<<4)|3) // for text
#define HotKeySpaceColor (1<<4) //  (15<<4|4)
#define HotKeyiCareColor  ((15<<4)|2)

// index 0: 決定把 pattle 那一種 color 使用, 可以為 2/4/8色方式
// 1: 2色; 2: 4色; 4: 8色
// index 1: 決定使用那一個pattle
// index 2: 要填入的font data
BYTE code tHotKeyIconTbl[][15][3]=
{
	{ // 1 standard
        {4, HotKey8Color, HotKey2ColorAddr+0},
        {4, HotKey8Color, HotKey2ColorAddr+1},
        {4, HotKey8Color, HotKey2ColorAddr+1},
        {4, HotKey8Color, HotKey2ColorAddr+2},
        {4, HotKey8Color, HotKey2ColorAddr+3},

        {4, HotKey8Color, HotKey2ColorAddr+4},
        {4, HotKey8Color, HotKey2ColorAddr+5},
        {4, HotKey8Color, HotKey2ColorAddr+6},
        {4, HotKey8Color, HotKey2ColorAddr+7},
        {4, HotKey8Color, HotKey2ColorAddr+8},

        {4, HotKey8Color, HotKey2ColorAddr+9},
        {4, HotKey8Color, HotKey2ColorAddr+10},
        {4, HotKey8Color, HotKey2ColorAddr+11},
        {4, HotKey8Color, HotKey2ColorAddr+12},
        {4, HotKey8Color, HotKey2ColorAddr+13},
	},
	{ // 2 text
        {0, HotKey8Color_c, HotKey4ColorAddr+0},
        {0, HotKey8Color_c, HotKey4ColorAddr+2},
		{0, HotKey8Color_c, HotKey4ColorAddr+4},
		{0, HotKey8Color_c, HotKey4ColorAddr+6},
        {0, HotKey8Color_c, HotKey4ColorAddr+8},

		{0, HotKey8Color_c, HotKey4ColorAddr+10},
		{0, HotKey8Color_c, HotKey4ColorAddr+12},
		{0, HotKey8Color_c, HotKey4ColorAddr+14},
		{0, HotKey8Color_c, HotKey4ColorAddr+16},
        {0, HotKey8Color_c, HotKey2ColorAddr+0},

        {0, HotKey8Color_c, HotKey2ColorAddr+1},
		{0, HotKey8Color_c, HotKey4ColorAddr+18},
		{0, HotKey8Color_c, HotKey4ColorAddr+20},
		{0, HotKey8Color_c, HotKey2ColorAddr+2},
        {0, HotKey8Color_c, HotKey2ColorAddr+3},
	},
    { // 3 internet
        {4, HotKey8Color, HotKey4ColorAddr+0},
        {4, HotKey8Color, HotKey4ColorAddr+2},
        {4, HotKey8Color, HotKey4ColorAddr+4},
        {4, HotKey8Color, HotKey4ColorAddr+6},
        {4, HotKey8Color, HotKey4ColorAddr+8},

        {4, HotKey8Color, HotKey4ColorAddr+10},
        {4, HotKey8Color, HotKey4ColorAddr+12},
        {4, HotKey8Color, HotKey4ColorAddr+14},
        {4, HotKey8Color, HotKey4ColorAddr+16},
        {4, HotKey8Color, HotKey4ColorAddr+18},

        {4, HotKey8Color, HotKey4ColorAddr+20},
        {4, HotKey8Color, HotKey4ColorAddr+22},
        {4, HotKey8Color, HotKey4ColorAddr+24},
        {4, HotKey8Color, HotKey4ColorAddr+26},
        {4, HotKey8Color, HotKey4ColorAddr+28},
    },
    { // 4 game
        {4, HotKey8Color_b, HotKey4ColorAddr+0},
        {4, HotKey8Color_b, HotKey4ColorAddr+2},
        {4, HotKey8Color_b, HotKey4ColorAddr+4},
        {4, HotKey8Color_b, HotKey8ColorAddr+0},
        {4, HotKey8Color_b, HotKey4ColorAddr+6},

        {4, HotKey8Color_b, HotKey4ColorAddr+8},
        {4, HotKey8Color_b, HotKey4ColorAddr+10},
        {4, HotKey8Color_b, HotKey4ColorAddr+12},
        {4, HotKey8Color_b, HotKey4ColorAddr+14},
        {4, HotKey8Color_b, HotKey4ColorAddr+16},

        {4, HotKey8Color_b, HotKey4ColorAddr+18},
        {4, HotKey8Color_b, HotKey4ColorAddr+20},
        {1, HotKeySpaceColor, SpaceFont},
        {1, HotKeySpaceColor, SpaceFont},
        {1, HotKeySpaceColor, SpaceFont},
    },
    { //5  movie
        {4, HotKey8Color, HotKey4ColorAddr+0},
        {4, HotKey8Color, HotKey4ColorAddr+2},
        {4, HotKey8Color, HotKey4ColorAddr+4},
        {4, HotKey8Color, HotKey4ColorAddr+6},
        {4, HotKey8Color, HotKey4ColorAddr+8},

        {4, HotKey8Color, HotKey4ColorAddr+10},
        {4, HotKey8Color, HotKey4ColorAddr+12},
        {4, HotKey8Color, HotKey4ColorAddr+14},
        {4, HotKey8Color, HotKey4ColorAddr+16},
        {4, HotKey8Color, HotKey4ColorAddr+18},

        {4, HotKey8Color, HotKey4ColorAddr+20},
        {4, HotKey8Color, HotKey4ColorAddr+22},
        {4, HotKey8Color, HotKey4ColorAddr+24},
        {4, HotKey8Color, HotKey4ColorAddr+26},
        {4, HotKey8Color, HotKey4ColorAddr+28},
    },
    { // 5  sports
        {1, HotKeySpaceColor, SpaceFont},
        {4, HotKey8Color, HotKey2ColorAddr+0},
        {4, HotKey8Color, HotKey2ColorAddr+1},
        {4, HotKey8Color, HotKey2ColorAddr+2},
        {4, HotKey8Color, HotKey2ColorAddr+3},

        {1, HotKeySpaceColor, SpaceFont},
        {4, HotKey8Color, HotKey2ColorAddr+4},
        {4, HotKey8Color, HotKey2ColorAddr+5},
        {4, HotKey8Color, HotKey2ColorAddr+6},
        {4, HotKey8Color, HotKey2ColorAddr+7},

        {4, HotKey8Color, HotKey2ColorAddr+8},
        {4, HotKey8Color, HotKey2ColorAddr+9},
        {4, HotKey8Color, HotKey2ColorAddr+10},
        {1, HotKeySpaceColor, SpaceFont},
        {1, HotKeySpaceColor, SpaceFont},
    },

#if AudioFunc
    { // 6 volume
        {4, HotKey8Color, HotKey2ColorAddr+0},
        {4, HotKey8Color, HotKey2ColorAddr+1},
        {4, HotKey8Color, HotKey2ColorAddr+2},
        {4, HotKey8Color, HotKey2ColorAddr+3},
        {1, HotKeySpaceColor, SpaceFont},

        {4, HotKey8Color, HotKey2ColorAddr+4},
        {4, HotKey8Color, HotKey2ColorAddr+5},
        {4, HotKey8Color, HotKey2ColorAddr+6},
        {4, HotKey8Color, HotKey2ColorAddr+7},
        {4, HotKey8Color, HotKey2ColorAddr+8},

	 {1, HotKeySpaceColor, SpaceFont},
        {4, HotKey8Color, HotKey2ColorAddr+9},
        {4, HotKey8Color, HotKey2ColorAddr+10},
        {4, HotKey8Color, HotKey2ColorAddr+11},
        {4, HotKey8Color, HotKey2ColorAddr+12},
    },
#else

    { // 6 Hot con
        {4, HotKey8Color, HotKey4ColorAddr+0},
        {4, HotKey8Color, HotKey4ColorAddr+2},
        {4, HotKey8Color, HotKey4ColorAddr+4},
        {4, HotKey8Color, HotKey4ColorAddr+6},
        {4, HotKey8Color, HotKey4ColorAddr+8},

        {4, HotKey8Color, HotKey4ColorAddr+10},
        {4, HotKey8Color, HotKey4ColorAddr+12},
        {4, HotKey8Color, HotKey4ColorAddr+14},
        {4, HotKey8Color, HotKey4ColorAddr+16},
        {4, HotKey8Color, HotKey4ColorAddr+18},

	 {4, HotKey8Color, HotKey4ColorAddr+20},
        {4, HotKey8Color, HotKey4ColorAddr+22},
        {4, HotKey8Color, HotKey4ColorAddr+24},
        {4, HotKey8Color, HotKey4ColorAddr+26},
        {4, HotKey8Color, HotKey4ColorAddr+28},
    },

#endif


    { // 7 DCR ON
        {4, HotKey8Color, HotKey4ColorAddr+0},
        {4, HotKey8Color, HotKey4ColorAddr+2},
        {4, HotKey8Color, HotKey4ColorAddr+4},
        {4, HotKey8Color, HotKey4ColorAddr+6},
        {4, HotKey8Color, HotKey4ColorAddr+8},

        {4, HotKey8Color, HotKey4ColorAddr+10},
        {4, HotKey8Color, HotKey4ColorAddr+12},
        {4, HotKey8Color, HotKey4ColorAddr+14},
        {4, HotKey8Color, HotKey4ColorAddr+16},
        {4, HotKey8Color, HotKey4ColorAddr+18},

	 {4, HotKey8Color, HotKey4ColorAddr+20},
        {4, HotKey8Color, HotKey4ColorAddr+22},
        {4, HotKey8Color, HotKey4ColorAddr+24},
        {4, HotKey8Color, HotKey4ColorAddr+26},
        {4, HotKey8Color, HotKey4ColorAddr+28},
    },

    { // 7 DCR Off
        {4, HotKey8Color, HotKey4ColorAddr+0},
        {4, HotKey8Color, HotKey4ColorAddr+30},
        {4, HotKey8Color, HotKey4ColorAddr+4},
        {4, HotKey8Color, HotKey4ColorAddr+32},
        {4, HotKey8Color, HotKey4ColorAddr+8},

        {4, HotKey8Color, HotKey4ColorAddr+10},
        {4, HotKey8Color, HotKey4ColorAddr+34},
        {4, HotKey8Color, HotKey4ColorAddr+36},
        {4, HotKey8Color, HotKey4ColorAddr+38},
        {4, HotKey8Color, HotKey4ColorAddr+18},

	 {4, HotKey8Color, HotKey4ColorAddr+20},
        {4, HotKey8Color, HotKey4ColorAddr+40},
        {4, HotKey8Color, HotKey4ColorAddr+24},
        {4, HotKey8Color, HotKey4ColorAddr+42},
        {4, HotKey8Color, HotKey4ColorAddr+28},
    },
#if HotExpansionAdjust
	{ // 8 Expansion native
		{4, HotKeyExpansion4Color, HotKey4ColorAddr+0},
		{4, HotKeyExpansion4Color, HotKey4ColorAddr+2},
		{4, HotKeyExpansion4Color, HotKey4ColorAddr+4},
		{4, HotKeyExpansion4Color, HotKey4ColorAddr+6},
		{4, HotKeyExpansion4Color, HotKey4ColorAddr+8},
		
		{4, HotKeyExpansion4Color, HotKey4ColorAddr+10},
		{0, HotKeyExpansion2Color, HotKey2ColorAddr+0},
		{0, HotKeyExpansion2Color, HotKey2ColorAddr+1},
		{0, HotKeyExpansion2Color, HotKey2ColorAddr+2},
		{4, HotKeyExpansion4Color, HotKey4ColorAddr+12},
		
		{4, HotKeyExpansion4Color, HotKey4ColorAddr+14},
		{4, HotKeyExpansion4Color, HotKey4ColorAddr+16},
		{4, HotKeyExpansion4Color, HotKey4ColorAddr+18},
		{4, HotKeyExpansion4Color, HotKey4ColorAddr+20},
		{4, HotKeyExpansion4Color, HotKey4ColorAddr+22},
	},
	{ // 9 Expansion Full
		{4, HotKeyExpansion4Color, HotKey4ColorAddr+24},
		{4, HotKeyExpansion4Color, HotKey4ColorAddr+26},
		{4, HotKeyExpansion4Color, HotKey4ColorAddr+28},
		{4, HotKeyExpansion4Color, HotKey4ColorAddr+30},
		{4, HotKeyExpansion4Color, HotKey4ColorAddr+32},
		
		{4, HotKeyExpansion4Color, HotKey4ColorAddr+34},
		{0, HotKeyExpansion2Color, HotKey2ColorAddr+3},
		{0, HotKeyExpansion2Color, HotKey2ColorAddr+4},
		{0, HotKeyExpansion2Color, HotKey2ColorAddr+5},		
		{4, HotKeyExpansion4Color, HotKey4ColorAddr+36},

		{4, HotKeyExpansion4Color, HotKey4ColorAddr+38},
		{4, HotKeyExpansion4Color, HotKey4ColorAddr+40},
		{4, HotKeyExpansion4Color, HotKey4ColorAddr+42},
		{4, HotKeyExpansion4Color, HotKey4ColorAddr+44},
		{4, HotKeyExpansion4Color, HotKey4ColorAddr+46},
	},

#endif    
#if Enable_Lightsensor // 090416
    { // icare
        {2, HotKeyiCareColor, HotKey4ColorAddr+0},
        {2, HotKeyiCareColor, HotKey4ColorAddr+2},
        {2, HotKeyiCareColor, HotKey4ColorAddr+4},
        {2, HotKeyiCareColor, HotKey4ColorAddr+6},
        {2, HotKeyiCareColor, HotKey4ColorAddr+8},

        {2, HotKeyiCareColor, HotKey4ColorAddr+10},
        {2, HotKeyiCareColor, HotKey4ColorAddr+12},
        {2, HotKeyiCareColor, HotKey4ColorAddr+14},
        {2, HotKeyiCareColor, HotKey4ColorAddr+16},
        {2, HotKeyiCareColor, HotKey4ColorAddr+18},

	 {2, HotKeyiCareColor, HotKey4ColorAddr+20},
        {2, HotKeyiCareColor, HotKey4ColorAddr+22},
        {2, HotKeyiCareColor, HotKey4ColorAddr+24},
        {2, HotKeyiCareColor, HotKey4ColorAddr+26},
        {2, HotKeyiCareColor, HotKey4ColorAddr+28},
    },
#endif

};

extern BYTE OsdFontColor  ;
extern BYTE MenuPageIndex ;

void Osd_DrawHotKeyIcon(BYTE itemIndex, BYTE xpos, BYTE ypos)
{
#if 0// for debug icon
    xpos = 0;
    ypos = 0;

    Osd_SetOsdEngine(0);
    OsdFontColor = 0xe4;
    Osd_DrawCharDirect(0, 0, tHotKeyIconTbl[itemIndex][0][2]);
    Osd_DrawCharDirect(1, 0, tHotKeyIconTbl[itemIndex][1][2]);
    Osd_DrawCharDirect(2, 0, tHotKeyIconTbl[itemIndex][2][2]);
    Osd_DrawCharDirect(3, 0, tHotKeyIconTbl[itemIndex][3][2]);
    Osd_DrawCharDirect(4, 0, tHotKeyIconTbl[itemIndex][4][2]);

    OsdFontColor = 0xe4;
    Osd_DrawCharDirect(0 ,1, tHotKeyIconTbl[itemIndex][5][2]);
    OsdFontColor = 0xe4;
    Osd_DrawCharDirect(1, 1, tHotKeyIconTbl[itemIndex][6][2]);
    Osd_DrawCharDirect(2, 1, tHotKeyIconTbl[itemIndex][7][2]);
    Osd_DrawCharDirect(3, 1, tHotKeyIconTbl[itemIndex][8][2]);
    Osd_DrawCharDirect(4, 1, tHotKeyIconTbl[itemIndex][9][2]);

    OsdFontColor = 0xe4;
    Osd_DrawCharDirect(0 ,2, tHotKeyIconTbl[itemIndex][10][2]);
    Osd_DrawCharDirect(1, 2, tHotKeyIconTbl[itemIndex][11][2]);
    Osd_DrawCharDirect(2, 2, tHotKeyIconTbl[itemIndex][12][2]);
    Osd_DrawCharDirect(3, 2, tHotKeyIconTbl[itemIndex][13][2]);
    Osd_DrawCharDirect(4, 2, tHotKeyIconTbl[itemIndex][14][2]);

#else


    BYTE i, j;

    for(j=0; j<3; j++) // y
    {
    	for(i=0; i<5; i++) // x
        {
        	// check use 291 font
        	 if(tHotKeyIconTbl[itemIndex][i+j*5][0] & 0x80 )
             	mStar_WriteByte(OSDDF, DISPCD8_B);

            // check use color depth
        	 if( (tHotKeyIconTbl[itemIndex][i+j*5][0] & 0x04) >> 2  ) // for 8 color
		     {
                	Osd_Set256TextColor(tHotKeyIconTbl[itemIndex][i+j*5][1] ,Color_8);
                   // OsdFontColor = OsdFontColor+1;
                   // if(itemIndex==ECO_Game)
                   // printData("OsdFontColor= %x", OsdFontColor);
             }
             else // for 2/4 color
             {
                	Osd_SetTextColor(0, tHotKeyIconTbl[itemIndex][i+j*5][1] + (1 << tHotKeyIconTbl[itemIndex][i+j*5][0]&0x0F));
             }

            // write font
             Osd_DrawCharDirect(xpos+i, ypos+j, tHotKeyIconTbl[itemIndex][i+j*5][2]);

            // check use 291 font
        	 if(tHotKeyIconTbl[itemIndex][i+j*5][0] & 0x80)
                 mStar_WriteByte(OSDDF, 0);
		}
     }
#endif

}



void LoadHotKeyColorFont(BYTE index)
{

#if Enable_OsdEngine1
    Osd_SetOsdEngine(0);
#endif
    Osd_SetTextColor( 0, 20 );
    mStar_WriteOSDByte(IOSDC2, 0x01); //0x83); // color bit setting
    mStar_WriteOSDByte(OSDCFA, _2ndPageColorFontAddr); // 4 color font RAM start address

    //Osd_Load256Color(0, &tOSDColorPalette256, sizeof(tOSDColorPalette256)); // load osd color
    Osd_LoadOriginalPalette();
#if AudioFunc
 	if(MenuPageIndex == HotKeyVolMenu)
 	{
		OSD_LoadVolIconFont();
	}

	else
#else
	if(MenuPageIndex == HotKeyConMenu)
 	{
		LoadHotKeyConFont();
	}

	else		
#endif
#if EnableHotBrightness
	if(MenuPageIndex == HotKeyBriMenu)
 	{
 	#if DEBUG_ENABLE
	printMsg("LoadHotKeyBriFont11111111111");
	#endif
 		//Osd_LoadMainMenuIconFont();
		LoadHotKeyBriFont();
	}

	else		
#endif
		if(MenuPageIndex == HotKeyECOMenu)
	{
		switch(index)
        {
            case ECO_Standard:
			OSD_LoadStandardEcoIconFont();
                break;
            case ECO_Sports:
			OSD_LoadSportEcoIconFont();
                break;
            case ECO_Text:
                mStar_WriteOSDByte(IOSDC2, 0x83); //0x83); // color bit setting
                mStar_WriteOSDByte(OSDCFA, HotKey4ColorAddr); // 4 color font RAM start address
                OSD_LoadTextEcoIconFont();
                break;
            case ECO_Internet:
                mStar_WriteOSDByte(IOSDC2, 0x83); //0x83); // color bit setting
                mStar_WriteOSDByte(OSDCFA, HotKey4ColorAddr); // 4 color font RAM start address
                OSD_LoadInternetEcoIconFont();
                break;
            case ECO_Movie:
                mStar_WriteOSDByte(IOSDC2, 0x83); //0x83); // color bit setting
                mStar_WriteOSDByte(OSDCFA, HotKey4ColorAddr); // 4 color font RAM start address
                OSD_LoadMovieEcoIconFont();
                break;
	    	case ECO_Game: //temp use internet icon
                mStar_WriteOSDByte(IOSDC2, 0x83); //0x83); // color bit setting
                mStar_WriteOSDByte(OSDCFA, HotKey4ColorAddr); // 4 color font RAM start address
                mStar_WriteOSDByte(OSD8CFFA, HotKey8ColorAddr); // 4 color font RAM start address
                mStar_WriteOSDByte(OSDCFHA, 0x00);
                OSD_LoadGameEcoIconFont();
	            break;
        }
	}


}


#if Enable_SubWin
void SetOsdSubWindow( BYTE Win_Num, BYTE HStar, BYTE HEnd , BYTE VStar, BYTE VEnd, BYTE Color )
{
    if (Win_Num>4) return;
    switch (Win_Num)
    {
      case SUB_WIN_00:
           Win_Num = SUBW0C;
           break;
      case SUB_WIN_01:
           Win_Num = SUBW1C;
           break;
      case SUB_WIN_02:
           Win_Num = SUBW2C;
           break;
      case SUB_WIN_03:
           Win_Num = SUBW3C;
           break;
    }
    mStar_WriteOSDByte( Win_Num, 0x00 );
    mStar_WriteOSDByte( Win_Num + 1, HStar );   
    mStar_WriteOSDByte( Win_Num + 2, HEnd );  
    mStar_WriteOSDByte( Win_Num + 3, VStar );  
    mStar_WriteOSDByte( Win_Num + 4, VEnd );  
    mStar_WriteOSDByte( Win_Num + 5, Color );   
#if WaitingForDrawOSDSUB
if(FactoryModeFlag&&MenuPageIndex == MainMenu)
	mStar_WriteOSDByte( Win_Num, 0x01|BIT1 );
else
{
	mStar_WriteOSDByte( Win_Num, BIT1 );
	mStar_WaitForDataBlanking();
	mStar_WriteOSDByte( Win_Num, 0x01|BIT1 );
}	
#else
    mStar_WriteOSDByte( Win_Num, 0x01|BIT1 );
#endif
}

void DisableOsdSubWindow( BYTE Win_Num )
{
    if (Win_Num>4) return;
    switch (Win_Num)
    {
      case 0:
           Win_Num = SUBW0C;
           break;
      case 1:
           Win_Num = SUBW1C;
           break;
      case 2:
           Win_Num = SUBW2C;
           break;
      case 3:
           Win_Num = SUBW3C;
           break;
    }
    mStar_WriteOSDByte( Win_Num, 0x00 );
}
#endif

#if Enable_GDColor

void SetOsdGraduallyBackGround( BYTE Win_Num, BYTE GDValue, BYTE GDDelta , BYTE GDDERange, BYTE GDControl, BYTE GDInit )
{
    if (Win_Num>4) return;
    switch (Win_Num)
    {
      case SUB_WIN_00:
           Win_Num = OSD_GD00;
           break;
      case SUB_WIN_01:
           Win_Num = OSD_GD01;
           break;
      case SUB_WIN_02:
           Win_Num = OSD_GD02;
           break;
      case SUB_WIN_03:
           Win_Num = OSD_GD03;
           break;
    }
    mStar_WriteOSDByte( Win_Num, 	GDValue);
    mStar_WriteOSDByte( Win_Num + 1, GDValue );   
    mStar_WriteOSDByte( Win_Num + 2, GDValue );  
    mStar_WriteOSDByte( Win_Num + 3, GDDelta );  
    mStar_WriteOSDByte( Win_Num + 4, GDDelta );  
    mStar_WriteOSDByte( Win_Num + 5, GDDERange );   
    mStar_WriteOSDByte( Win_Num + 6, GDDERange );   
    mStar_WriteOSDByte( Win_Num + 7, GDDERange );   
    mStar_WriteOSDByte( Win_Num + 8, GDDERange );	
    mStar_WriteOSDByte( Win_Num + 9, GDControl );   
    mStar_WriteOSDByte( Win_Num + 10, GDInit );   
    mStar_WriteOSDByte( Win_Num + 11, GDInit );   	
    mStar_WriteOSDByte( Win_Num + 12, GDInit );
}

#endif

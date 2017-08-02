///////////////////////////////////////////////////////////////////////////////
/// @file PropFontExtFunc.c
/// @brief Show the information at the bottom of main menu.
/// @author MStarSemi Inc.
///
///
///////////////////////////////////////////////////////////////////////////////
#define _PROPFONTEXTFUNC_C

#include <math.h>
#include "PropFontExtFunc.h"
#include "Board.h"
#include "ms_reg.h"
#include "msOSD.h"
#include "Menu.h"
//#include "DebugMsg.h"
#include "Ms_rwreg.h"
#include "PropFontExtFunc.h"
#include "LoadPropFont1218.h"
#include "menudef.h"
//#include "Panel.h"
//#include "misc.h"
//#include "msTMDS.h"
#if ENABLE_3D_FUNCTION
#include "msScaler.h"
#endif

#define PROPFONTEXTFUNC_DEBUG    1
#if ENABLE_MSTV_UART_DEBUG && PROPFONTEXTFUNC_DEBUG
#define PROPFONTEXTFUNC_PRINT(format, ...)     printf(format, ##__VA_ARGS__)
#else
#define PROPFONTEXTFUNC_PRINT(format, ...)
#endif

//MST_MODULE(PropFontExtFunc)

#define AF_BLANK_CHAR           0x01
#define AF_ZERO_CHAR            0x02    ///'0'
#define AF_A_UPERCASE_CHAR      0x0C    ///'A'
#define AF_A_LOWERCASE_CHAR     0x26   ///'a'
#define AF_I_LOWERCASE_CHAR     ('i'-'a'+AF_A_LOWERCASE_CHAR)
#define AF_P_LOWERCASE_CHAR     ('p'-'a'+AF_A_LOWERCASE_CHAR)
#define AF_X_LOWERCASE_CHAR     ('x'-'a'+AF_A_LOWERCASE_CHAR)
#define AF_Z_LOWERCASE_CHAR     ('z'-'a'+AF_A_LOWERCASE_CHAR)
#define AF_S_LOWERCASE_CHAR     ('s'-'a'+AF_A_LOWERCASE_CHAR)
#define AF_H_UPERCASE_CHAR      ('H'-'A'+AF_A_UPERCASE_CHAR)
#define AF_AT_CHAR              0x41
#define AF_DOT_CHAR             0x46    // SimonY add for TVI version
#define AF_COLON_CHAR           0x42
#define AF____CHAR           0x47
#define AF_PERCENT_CHAR         0x7E

#if 0//ENABLE_COMPRESSED_PROP_FONT
extern WORD code tPropFontSetArialNarrow13[];
extern WORD code tPropFontSetArialNarrow13Bold[];
#else
extern PropFontNonCompressType code tPropFontSetArialNarrow13[];
extern PropFontNonCompressType code tPropFontSetArialNarrow13Bold[];
#endif

extern BYTE u8ResolutionTextLen;

extern void Osd_DynamicLoadFont(WORD u8Addr, BYTE *pu8FontPtr, WORD u16Num);

//static BYTE xdata  tOORTimingPropText[2];  /// Prop. type string of "Optimum Resolution HHHHxVVVV  FFHz" for Out of Range menu.

/// Load a font char(12x18) to font RAM.
#if 0
void OSDLoadOneFont(WORD* pu16SN)
{   BYTE  i;
    #define u16SN1  (*(pu16SN+i))
    #define u16SN2  (*(pu16SN+i+1))
    WRITE_FONT();
    for(i=0; i < FONT_HEIGHT; i+=2)
    {
    #if CHIP_FAMILY_TYPE>=CHIP_FAMILY_TSUMV
        //xxxx0000 00001111
        //xxxx1111 22222222
        msWriteByte(PORT_FONT_DATA, (BYTE)(u16SN1>>4) );
        msWriteByte(PORT_FONT_DATA, (LOBYTE(u16SN1)<<4)+HIBYTE(u16SN2));
        msWriteByte(PORT_FONT_DATA, LOBYTE(u16SN2));
    #else
        msWriteByte(OSD2_A4, HIBYTE(u16SN1) + (HIBYTE(u16SN2) <<4));
        msWriteByte(OSD2_A4, LOBYTE(u16SN1));
        msWriteByte(OSD2_A4, LOBYTE(u16SN2));
    #endif
    }
    #undef u16SN1
    #undef u16SN2
}
#endif
void LoadFontSetAddrStart(BYTE u8Addr)
{
    {   extern BYTE xdata g_u8FontAddrHiBits;
    //u16StrIndex=((msRead2Byte(OSD1_08)+((((WORD)g_u8FontAddrHiBits)<<8)+(u8Addr))*(((msReadByte(OSD1_0B)>>4)&0x03)+1))<<2); //get real address in cafram, ((font base entry)+unit*(unit size))*4 addr/entry
    //u16StrIndex=GET_FONT_RAM_ADDR(u8Addr); //get real address in cafram, ((font base entry)+unit*(unit size))*4 addr/entry
    WRITE_CAFSRAM_ADDR();
    msWrite2Byte(PORT_FONT_ADDR, ((msRead2Byte(OSD1_08)+((((WORD)g_u8FontAddrHiBits)<<8)+(u8Addr))*(((msReadByte(OSD1_0B)>>4)&0x03)+1))<<2));
    }
    WRITE_FONT();
#if ENABLE_COMPRESSED_PROP_FONT //asign map pointer
    {
    extern WORD code tPropFontSetArialNarrow13BoldMap[];
    extern WORD code tPropFontSetArialNarrow13Map[];

    if (pstPropFontSet1218==tPropFontSetArialNarrow13Bold)
    pstPropFontSet1218Map=tPropFontSetArialNarrow13BoldMap;
    else if (pstPropFontSet1218==tPropFontSetArialNarrow13)
    pstPropFontSet1218Map=tPropFontSetArialNarrow13Map;
    }
#endif
}

/// Retrive the first byte value(Font left/right space info), return the sum of this two value.
static BYTE GetFontSpaceValue(BYTE ch)
{
#if ENABLE_COMPRESSED_PROP_FONT
    WORD *pstFontData;
    //Jison 120117
    if (pstPropFontSet1218==tPropFontSetArialNarrow13Bold)
        pstPropFontSet1218Map=tPropFontSetArialNarrow13BoldMap;
    else if (pstPropFontSet1218==tPropFontSetArialNarrow13)
        pstPropFontSet1218Map=tPropFontSetArialNarrow13Map;

    pstFontData = pstPropFontSet1218+pstPropFontSet1218Map[ch];
    return ( ((*pstFontData) & 0x0F) + (((*pstFontData) & 0xF0)>>4) );
#else
    PropFontNonCompressType *pstFontData;
    pstFontData = pstPropFontSet1218+ch;
    return ( (pstFontData->u8SpaceWidth & 0x0F) + ((pstFontData->u8SpaceWidth & 0xF0)>>4) );
#endif
}
#if ENABLE_WORD_FONT_INDEX
BYTE GetFontSpaceWidthW12(BYTE u8Index)
{
  #if ENABLE_COMPRESSED_PROP_FONT
    return (BYTE)pstPropFontSet1218[pstPropFontSet1218Map[u8Index]];
  #else
    return pstPropFontSet1218[u8Index].u8SpaceWidth;
  #endif
}
PropFontNonCompressType* GetFontData1218(PropFontNonCompressType *pstFontData,BYTE u8Index)
{
  #if ENABLE_COMPRESSED_PROP_FONT
    pstFontData->u8SpaceWidth=(BYTE)pstPropFontSet1218[pstPropFontSet1218Map[u8Index]];
    ExtractOneCompressedFont(pstPropFontSet1218+pstPropFontSet1218Map[u8Index]+1,pstFontData->tLineData);
  #else
    BYTE i;
    pstFontData->u8SpaceWidth=pstPropFontSet1218[u8Index].u8SpaceWidth;
    for(i=0;i<FONT_HEIGHT;i++)
        pstFontData->tLineData[i]=pstPropFontSet1218[u8Index].tLineData[i];
  #endif
    return pstFontData;
}
#endif

typedef struct
{
    BYTE *pu8StrBuf;
    BYTE u8Char;
    WORD u16Value;
    WORD u16PixelWidth;
    BYTE u8FontCount;
} FormatTimingStringType;

static void ValueToStr(FormatTimingStringType *pstFTS)
{   BYTE xdata i;
    BYTE xdata u8Pos;

    if(pstFTS->u16Value>=1000)
        i=4;
    else if(pstFTS->u16Value>=100)
        i=3;
    else if(pstFTS->u16Value>=10)
        i=2;
    else
        i=1;
    u8Pos=pstFTS->u8FontCount;
    for(;i!=0;i--)
    {
        pstFTS->u16PixelWidth = pstFTS->u16PixelWidth + (FONT_WIDTH - GetFontSpaceValue(AF_ZERO_CHAR+(pstFTS->u16Value%10)));
        pstFTS->u16PixelWidth += SP_BETWEEN_FONT;//BETWEEN_SPACE;
        //PROPFONTEXTFUNC_PRINT("\r\n width %d\n",pstFTS->u16PixelWidth);
        *(pstFTS->pu8StrBuf+u8Pos+i-1) = AF_ZERO_CHAR+(pstFTS->u16Value%10);
        //PROPFONTEXTFUNC_PRINT("\r\n u8Pos %d\n",u8Pos+i-1);
        pstFTS->u8FontCount++;
        pstFTS->u16Value/=10;
    }
}

static void CharToStr(FormatTimingStringType *pstFTS)
{   pstFTS->u16PixelWidth = pstFTS->u16PixelWidth + (FONT_WIDTH - GetFontSpaceValue(pstFTS->u8Char));
    pstFTS->u16PixelWidth += SP_BETWEEN_FONT;//BETWEEN_SPACE;
    *(pstFTS->pu8StrBuf+pstFTS->u8FontCount) = pstFTS->u8Char;
    pstFTS->u8FontCount++;
}
/// Save Timing Info to pu8Str as HHHHxVVVV  FFHz" format.
/// Return string pixel width(low WORD) and string array count(high WORD,NULL CHAR INCLUDED)
static DWORD FormatTimingString(BYTE *pu8Str,WORD u16HResolution,WORD u16VResolution, BYTE u8VFreq, Bool bShowHz)
{
    FormatTimingStringType FTS;
    DWORD  xdata u32Return;

    FTS.pu8StrBuf=pu8Str;
    FTS.u16PixelWidth=0;//SP_BETWEEN_FONT;   // First space
    FTS.u8FontCount=0;
    FTS.u8Char=0;

    FTS.u16Value=u16HResolution;
    // H resolution
    ValueToStr(&FTS);
    FTS.u8Char=AF_X_LOWERCASE_CHAR;
    CharToStr(&FTS);
    // V resolution
    FTS.u16Value=u16VResolution;
    ValueToStr(&FTS);
	
if(u8VFreq)
{
    FTS.u8Char=AF_AT_CHAR;
    CharToStr(&FTS);
    FTS.u16Value=u8VFreq;
    ValueToStr(&FTS);
}  
    if(bShowHz)
    {
        {
            FTS.u8Char=AF_H_UPERCASE_CHAR;
            CharToStr(&FTS);
            FTS.u8Char=AF_Z_LOWERCASE_CHAR;
            CharToStr(&FTS);
        }
    }

    //FTS.u16PixelWidth -= 2;//BETWEEN_SPACE;
    //ending
    *(pu8Str+FTS.u8FontCount) = 0x00; /// NULL Terminal String
    FTS.u8FontCount++;         /// *** NULL CHAR INCLUDED ***
    u32Return=FTS.u8FontCount;
    u32Return<<=16;
    u32Return+=FTS.u16PixelWidth;

   // u16Temp=((u16Temp%FONT_WIDTH) ? (u16Temp/FONT_WIDTH+1) : (u16Temp/FONT_WIDTH));

   if(u32Return%FONT_WIDTH)
   {
      ;
   }
   
    return u32Return;
}
static DWORD FormatVideoTimingString(BYTE *pu8Str,WORD u16VResolution)
{
    FormatTimingStringType FTS;
    DWORD  xdata u32Return;

    FTS.pu8StrBuf=pu8Str;
    FTS.u16PixelWidth=0;//SP_BETWEEN_FONT;   // First space
    FTS.u8FontCount=0;
    FTS.u8Char=0;

    FTS.u16Value=u16VResolution;
    ValueToStr(&FTS);
    if (SrcFlags(MapWin2Port(USER_PREF_WIN_SEL))&bInterlaceMode || SrcModeIndex(MapWin2Port(USER_PREF_WIN_SEL))==MD_1920X1080I_50) //108oi
        FTS.u8Char=AF_I_LOWERCASE_CHAR;
    else
        FTS.u8Char=AF_P_LOWERCASE_CHAR;
    CharToStr(&FTS);

    //FTS.u16PixelWidth -= 2;//BETWEEN_SPACE;
    //ending
    *(pu8Str+FTS.u8FontCount) = 0x00; /// NULL Terminal String
    FTS.u8FontCount++;         /// *** NULL CHAR INCLUDED ***
    u32Return=FTS.u8FontCount;
    u32Return<<=16;
    u32Return+=FTS.u16PixelWidth;
    return u32Return;
}
#if 0
/// Calculate the prop. font pixel width of string array.
static WORD PropStrWidth(BYTE *pu8Str)
{
    WORD  u16Len;
    BYTE  i;

    u16Len=0;
    for (i=0;*(pu8Str+i);i++)
    {
        u16Len=u16Len+SP_BETWEEN_FONT+(FONT_WIDTH-GetFontSpaceValue(*(pu8Str+i)));
    }
    return u16Len;
}
#endif
#if ENABLE_3D_FUNCTION
void Get3DTimingInfo(WORD *pu16HResolution,WORD *pu16VResolution)
{
#if 1
    if (g_SetupPathInfo.ucSCFmtIn==SC_FMT_IN_3D_FP)
        *pu16VResolution=GET_FP3D_2X_VRES(*pu16VResolution,SrcVTotal);
    *pu16HResolution=*pu16HResolution;
#else
    #define TOL 3
    //PROPFONTEXTFUNC_PRINT("pu16HResolution=%d\n",*pu16HResolution);
    //PROPFONTEXTFUNC_PRINT("pu16VResolution=%d\n",*pu16VResolution);
    if (abs(*pu16HResolution-1920)<TOL && abs(*pu16VResolution-2204)<TOL)
    {
        *pu16HResolution=1920;
        *pu16VResolution=1080;
    }
    #undef TOL
#endif
}
#endif

#define CHECK_HDCP()    (msReadByte(REG_161518) & BIT4)
void LoadResolutionString(BYTE u8StartAddr,BYTE* pu8String,WORD u16HResolution,WORD u16VResolution,BYTE u8VFreq, Bool bShowHz)
{
    BYTE  xdata u8TimingFontCount;
    WORD  xdata u16Temp;
    DWORD xdata u32Rlt;
    BYTE  xdata tTimingText[16];
    BYTE InputPort=MapWin2Port(USER_PREF_WIN_SEL);

    if(IsVideoTiming(InputPort)) // SOG video timing
    {
    #if ENABLE_DIGITAL_INPUT
        if (!INPUT_IS_VGA(InputPort))
        {
        #if (YPBPR_PERF_OVERSCAN)
            if (IsOverScan(InputPort))
            {
                u16HResolution=StandardModeWidth(InputPort);
                u16VResolution=StandardModeHeight(InputPort);
            }
        #endif
            if  (
              #if ENABLE_HDMI_INPUT
                (INPUT_IS_HDMI(InputPort) && (InputColorFormat(InputPort)!=INPUTCOLOR_RGB)) ||   //all HDMI video timing
              #endif
              #if ENABLE_DP_INPUT
                (INPUT_IS_DISPLAYPORT(InputPort) && ((BYTE)msAPI_combo_IPGetColorFormat((EN_COMBO_IP_SELECT)(g_InputPort[InputPort].eIPMux)).ucColorType !=(BYTE)INPUTCOLOR_RGB)) || //all DP video timing
              #endif
                (SrcFlags(InputPort)&bInterlaceMode) || (SrcModeIndex(InputPort)==MD_1920X1080I_50) //interlace mode
                || (INPUT_IS_DVI(InputPort) && CHECK_HDCP())) //DVI HDCP //avoid HDMI HDCP show video timing
                u32Rlt=FormatVideoTimingString((BYTE*)tTimingText,u16VResolution);
            else
                u32Rlt=FormatTimingString((BYTE*)tTimingText,u16HResolution,u16VResolution,u8VFreq,bShowHz);
        }
        else //VGA
    #endif
        {
        #if ENABLE_VGA_INPUT
            if (((msDrvIP1GetInputSyncType(MapPort2DetScIndex(INPUT_VGA))&COMP_B) && CURRENT_INPUT_COLOR_FORMAT==INPUT_COLOR_YPBPR) || (SrcFlags(InputPort)&bInterlaceMode)) //fix VGA interlace mode show PC timing
                u32Rlt=FormatVideoTimingString((BYTE*)tTimingText,u16VResolution);
            else
        #endif
                u32Rlt=FormatTimingString((BYTE*)tTimingText,u16HResolution,u16VResolution,u8VFreq,bShowHz);
        }
    }
    else //non-video timing
    {
        u32Rlt=FormatTimingString((BYTE*)tTimingText,u16HResolution,u16VResolution,u8VFreq,bShowHz);
    }

    u8TimingFontCount=(BYTE)(u32Rlt>>16);
    u16Temp=(WORD)(u32Rlt&0xFFFF);
    //u16Temp=(u16Temp/FONT_WIDTH);//((u16Temp%FONT_WIDTH) ? (u16Temp/FONT_WIDTH+1) : (u16Temp/FONT_WIDTH));
#if ENABLE_OSD_LR_DOUBLE
    u16Temp=((u16Temp%FONT_WIDTH) ? (u16Temp/FONT_WIDTH) : (u16Temp/FONT_WIDTH-1));
#else
    u16Temp=((u16Temp%FONT_WIDTH) ? (u16Temp/FONT_WIDTH+1) : (u16Temp/FONT_WIDTH));
#endif


   #if 0	
    Osd_DynamicLoadFont((u8StartAddr+0x100), tTimingText, u8TimingFontCount);	
  #else	
  
    OSD_FONT_HI_ADDR_SET_BIT9() ; //enable bit 8 0x100~
    LoadPropFonts1218(u8StartAddr, tTimingText, u8TimingFontCount,NULL,0, 0, 0);
    OSD_FONT_HI_ADDR_CLR_TO_0();
  #endif  
        /// Init. Prop string type
    *pu8String=u8StartAddr;
    *(pu8String+1)=u16Temp;

    u8ResolutionTextLen=u16Temp;
  
}

#if 1
static BYTE GetRealFontIndex(BYTE u8Ascii)
{
    if (u8Ascii=='.')   // SimonY add for TVI version
        return AF_DOT_CHAR;
    if (u8Ascii==':')   // SimonY add for TVI version
        return AF_COLON_CHAR;
    if (u8Ascii=='-')  
   	 return AF____CHAR;
    if (u8Ascii>='A' && u8Ascii<='Z')
        return ((u8Ascii-'A')+AF_A_UPERCASE_CHAR);
    if (u8Ascii>='a' && u8Ascii<='z')
        return ((u8Ascii-'a')+AF_A_LOWERCASE_CHAR);
    if (u8Ascii>='0' && u8Ascii<='9')
        return ((u8Ascii-'0')+AF_ZERO_CHAR);
    if (u8Ascii&0x80 && u8Ascii<0xB0)
        return u8Ascii;
    return AF_BLANK_CHAR;
}

#define MAX_TEXT_LENGTH     13//18
BYTE LoadPropFontText(WORD u8Addr,BYTE *pu8Text) ///pu8Text must be NULL terminal
{
    BYTE tText[MAX_TEXT_LENGTH];
    WORD i=0;

    while(*pu8Text)
    {
        tText[i]=GetRealFontIndex(*pu8Text);
        i++;
        pu8Text++;

        MST_ASSERT(i<MAX_TEXT_LENGTH);
    }

    MST_ASSERT(i!=0);

    tText[i++]=0;
    pstPropFontSet1218=tPropFontSetArialNarrow13Bold;//Jison 081030
    g_u8PropFontFlags=SPACE2PIXEL|LEFT_ALIGN;
    g_u8AlignResetIndex=0xFF;

    //return Osd_DynamicLoadFont((u8Addr), tText, i);	
	
    return LoadPropFonts1218(LOBYTE(u8Addr), tText, i, NULL,0, 0, 0);

	
 //   return LoadPropFonts1218(u8Addr, tText, i,NULL,0, 0, 0);
}

//extern BYTE GetEndOfMM(void);
extern BYTE xdata  tPropText[][2];
/*
BYTE* VersionText(void)
{
    BYTE code strVersion[]=VERSION_STRING;

    if (IS_MAIN_L0(g_u8MenuPageIndex))
        tPropText[1][0]=MM_VERSION_START;
    else ///Factory Menu
        tPropText[1][0]=FM_TEXT0_START;

#if 0
    Osd_DynamicLoadFont((u8Addr), tText, i);	
#else
    pstPropFontSet1218=tPropFontSetArialNarrow13Bold;
    g_u8PropFontFlags=SPACE2PIXEL|LEFT_ALIGN;
    tPropText[1][1]=LoadPropFontText(tPropText[1][0],strVersion);
#endif	

    return tPropText[1];
}
*/
#if 0//(ENABLE_DVI_INPUT||ENABLE_HDMI_INPUT)
BYTE* HzText(void)
{
    BYTE code strHz[]="Hz";
#if RUSSIAN
    BYTE code strHzRussian[]={0x81,0xA7,0x00};
#endif
    tPropText[1][0]=SM_HZ_STRING_START;
    pstPropFontSet1218=tPropFontSetArialNarrow13Bold;
    g_u8PropFontFlags=SPACE2PIXEL|LEFT_ALIGN;
#if RUSSIAN
    if (USER_PREF_LANGUAGE==LANG_RUSSIAN)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strHzRussian);
    else
#endif
    tPropText[1][1]=LoadPropFontText(tPropText[1][0],strHz);
    return tPropText[1];
}
#endif

#if ENABLE_VGA_INPUT
    #define VGA_SOURCE_TEXT     "VGA"
#endif
#if ENABLE_DVI_INPUT
  #if ENABLE_DVI2ND_INPUT
    #define DVI_SOURCE_TEXT     "DVI1"
    #define DVI2ND_SOURCE_TEXT  "DVI2"
    #if ENABLE_DVI3ND_INPUT
    #define DVI3ND_SOURCE_TEXT  "DVI3"
    #endif
  #else
    #define DVI_SOURCE_TEXT     "DVI"
  #endif
#endif
#if ENABLE_HDMI_INPUT
  #if ENABLE_HDMI2ND_INPUT

   #if Internal_Power_Setting
    #define HDMI_SOURCE_TEXT    "HDMI-3  2.0"
    #define HDMI2ND_SOURCE_TEXT "HDMI-2 MHL"
    #if ENABLE_HDMI3ND_INPUT
    #define HDMI3ND_SOURCE_TEXT "HDMI-1  1.4"
    #endif
  #elif (defined(FengYuan_Project))
    #define HDMI_SOURCE_TEXT    "HDMI1"
    #define HDMI2ND_SOURCE_TEXT "HDMI2"
    #if ENABLE_HDMI3ND_INPUT
    #define HDMI3ND_SOURCE_TEXT "HDMI3"
    #endif
  #elif (defined(SenYo_Project))
    #define HDMI_SOURCE_TEXT    "HDMI2.0"
    #define HDMI2ND_SOURCE_TEXT "HDMI-1"
    #if ENABLE_HDMI3ND_INPUT
    #define HDMI3ND_SOURCE_TEXT "HDMI-2"
    #endif
  #elif (defined(XuLi_Project))
    #define HDMI_SOURCE_TEXT    "HDMI60"
    #define HDMI2ND_SOURCE_TEXT "HDMI30"
    #if ENABLE_HDMI3ND_INPUT
    #define HDMI3ND_SOURCE_TEXT "HDMI30b"
    #endif
  /*
    #define HDMI_SOURCE_TEXT    "HDMI2.0(2160P1)"
    #define HDMI2ND_SOURCE_TEXT "HDMI-MHL(2160P2)"
    #if ENABLE_HDMI3ND_INPUT
    #define HDMI3ND_SOURCE_TEXT "HDMI(2160P3)"
    #endif
*/	
  #elif (defined(LianHeChuangXin_Project))
    #define HDMI_SOURCE_TEXT    "HDMI1"
    #define HDMI2ND_SOURCE_TEXT "HDMI2"
    #if ENABLE_HDMI3ND_INPUT
    #define HDMI3ND_SOURCE_TEXT "HDMI3"
    #endif
  #elif (defined(ZhongXinKeJi_Project))
    #define HDMI_SOURCE_TEXT    "HDMI-1"
    #define HDMI2ND_SOURCE_TEXT "HDMI-2"
    #if ENABLE_HDMI3ND_INPUT
    #define HDMI3ND_SOURCE_TEXT "HDMI-3"
    #endif
  
  #else
    #define HDMI_SOURCE_TEXT    "HDMI2.0"
    #define HDMI2ND_SOURCE_TEXT "HDMI-1 MHL"
    #if ENABLE_HDMI3ND_INPUT
    #define HDMI3ND_SOURCE_TEXT "HDMI-2"
    #endif
   #endif
   
    #if ENABLE_HDMI4ND_INPUT
    #define HDMI4ND_SOURCE_TEXT "HDMI4"
    #endif	
  #else
    #define HDMI_SOURCE_TEXT    "HDMI"
  #endif
#endif
#if ENABLE_DP_INPUT
  #if ENABLE_DP2ND_INPUT
    #define DP_SOURCE_TEXT      "DP1"
    #define DP2ND_SOURCE_TEXT   "DP2"
    #if ENABLE_DP3ND_INPUT
    #define DP3ND_SOURCE_TEXT   "DP3"
    #endif
    #if ENABLE_DP4ND_INPUT
    #define DP4ND_SOURCE_TEXT   "DP4"
    #endif
    #if ENABLE_DP5ND_INPUT
    #define DP5ND_SOURCE_TEXT   "DP5"
    #endif
    #if ENABLE_DP6ND_INPUT
    #define DP6ND_SOURCE_TEXT   "DP6"
    #endif
  #else
    #define DP_SOURCE_TEXT      "DP"
  #endif
#endif
BYTE* SelectInputSourceText(void)
{
  #if ENABLE_VGA_INPUT
    BYTE code strInput_VGA[]=VGA_SOURCE_TEXT;
  #endif
  #if ENABLE_DVI_INPUT
    BYTE code strInput_DVI[]=DVI_SOURCE_TEXT;
    #if ENABLE_DVI2ND_INPUT
    BYTE code strInput_DVI2ND[]=DVI2ND_SOURCE_TEXT;
    #endif
    #if ENABLE_DVI3ND_INPUT
    BYTE code strInput_DVI3ND[]=DVI3ND_SOURCE_TEXT;
    #endif
  #endif
  #if ENABLE_HDMI_INPUT
    BYTE code strInput_HDMI[]=HDMI_SOURCE_TEXT;
    #if ENABLE_HDMI2ND_INPUT
    BYTE code strInput_HDMI2ND[]=HDMI2ND_SOURCE_TEXT;
    #endif
    #if ENABLE_HDMI3ND_INPUT
    BYTE code strInput_HDMI3ND[]=HDMI3ND_SOURCE_TEXT;
    #endif
    #if ENABLE_HDMI4ND_INPUT
    BYTE code strInput_HDMI4ND[]=HDMI4ND_SOURCE_TEXT;
    #endif
  #endif
  #if ENABLE_DP_INPUT
    BYTE code strInput_DP[]=DP_SOURCE_TEXT;
    #if ENABLE_DP2ND_INPUT
    BYTE code strInput_DP2ND[]=DP2ND_SOURCE_TEXT;
    #endif
    #if ENABLE_DP3ND_INPUT
    BYTE code strInput_DP3ND[]=DP3ND_SOURCE_TEXT;
    #endif
    #if ENABLE_DP4ND_INPUT
    BYTE code strInput_DP4ND[]=DP4ND_SOURCE_TEXT;
    #endif
    #if ENABLE_DP5ND_INPUT
    BYTE code strInput_DP5ND[]=DP5ND_SOURCE_TEXT;
    #endif
    #if ENABLE_DP6ND_INPUT
    BYTE code strInput_DP6ND[]=DP6ND_SOURCE_TEXT;
    #endif
  #endif

#if ENABLE_MULTI_WINDOW_SETTINGS_MENU
    if (USER_PREF_WIN_SEL==DISPLAY_WIN4)
        tPropText[1][0]=MM_MW4_INPUT_SOURCE_TEXT_START;
    else if (USER_PREF_WIN_SEL==DISPLAY_WIN3)
        tPropText[1][0]=MM_MW3_INPUT_SOURCE_TEXT_START;
    else if (USER_PREF_WIN_SEL==DISPLAY_WIN2)
        tPropText[1][0]=MM_MW2_INPUT_SOURCE_TEXT_START;
    else  //MW_SEL_WINDOW1
        tPropText[1][0]=MM_MW1_INPUT_SOURCE_TEXT_START;
#else
    tPropText[1][0]=MM_INPUT_SOURCE_TEXT_START;
#endif
    pstPropFontSet1218=tPropFontSetArialNarrow13Bold;
    g_u8PropFontFlags=SPACE2PIXEL|RIGHT_ALIGN;
  #if ENABLE_VGA_INPUT
    if (MapWin2Port(USER_PREF_WIN_SEL)==INPUT_VGA)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strInput_VGA);
  #endif
  #if ENABLE_DVI_INPUT
    if (MapWin2Port(USER_PREF_WIN_SEL)==INPUT_DVI)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strInput_DVI);
    #if ENABLE_DVI2ND_INPUT
    if (MapWin2Port(USER_PREF_WIN_SEL)==INPUT_DVI2ND)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strInput_DVI2ND);
    #endif
    #if ENABLE_DVI3ND_INPUT
    if (MapWin2Port(USER_PREF_WIN_SEL)==INPUT_DVI3ND)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strInput_DVI3ND);
    #endif
  #endif
  #if ENABLE_HDMI_INPUT
    if (MapWin2Port(USER_PREF_WIN_SEL)==INPUT_HDMI)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strInput_HDMI);
    #if ENABLE_HDMI2ND_INPUT
    if (MapWin2Port(USER_PREF_WIN_SEL)==INPUT_HDMI2ND)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strInput_HDMI2ND);
    #endif
    #if ENABLE_HDMI3ND_INPUT
    if (MapWin2Port(USER_PREF_WIN_SEL)==INPUT_HDMI3ND)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strInput_HDMI3ND);
    #endif
    #if ENABLE_HDMI4ND_INPUT
    if (MapWin2Port(USER_PREF_WIN_SEL)==INPUT_HDMI4ND)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strInput_HDMI4ND);
    #endif
  #endif
  #if ENABLE_DP_INPUT
    if (MapWin2Port(USER_PREF_WIN_SEL)==INPUT_DISPLAYPORT)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strInput_DP);
    #if ENABLE_DP2ND_INPUT
    if(MapWin2Port(USER_PREF_WIN_SEL)==INPUT_DISPLAYPORT2ND)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strInput_DP2ND);
    #endif
    #if ENABLE_DP3ND_INPUT
    if(MapWin2Port(USER_PREF_WIN_SEL)==INPUT_DISPLAYPORT3ND)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strInput_DP3ND);
    #endif
    #if ENABLE_DP4ND_INPUT
    if(MapWin2Port(USER_PREF_WIN_SEL)==INPUT_DISPLAYPORT4ND)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strInput_DP4ND);
    #endif
    #if ENABLE_DP5ND_INPUT
    if(MapWin2Port(USER_PREF_WIN_SEL)==INPUT_DISPLAYPORT5ND)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strInput_DP5ND);
    #endif
    #if ENABLE_DP6ND_INPUT
    if(MapWin2Port(USER_PREF_WIN_SEL)==INPUT_DISPLAYPORT6ND)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strInput_DP6ND);
    #endif

  #endif
    return tPropText[1];
}

#if ENABLE_MULTI_WINDOW_SETTINGS_MENU
BYTE* InputSourceText_MultiWindow1(void)
{
  #if ENABLE_VGA_INPUT
    BYTE code strMW1Input_VGA[]=VGA_SOURCE_TEXT;
  #endif
  #if ENABLE_DVI_INPUT
    BYTE code strMW1Input_DVI[]=DVI_SOURCE_TEXT;
    #if ENABLE_DVI2ND_INPUT
    BYTE code strMW1Input_DVI2ND[]=DVI2ND_SOURCE_TEXT;
    #endif
    #if ENABLE_DVI3ND_INPUT
    BYTE code strMW1Input_DVI3ND[]=DVI3ND_SOURCE_TEXT;
    #endif
  #endif
  #if ENABLE_HDMI_INPUT
    BYTE code strMW1Input_HDMI[]=HDMI_SOURCE_TEXT;
    #if ENABLE_HDMI2ND_INPUT
    BYTE code strMW1Input_HDMI2ND[]=HDMI2ND_SOURCE_TEXT;
    #endif
    #if ENABLE_HDMI3ND_INPUT
    BYTE code strMW1Input_HDMI3ND[]=HDMI3ND_SOURCE_TEXT;
    #endif
    #if ENABLE_HDMI4ND_INPUT
    BYTE code strMW1Input_HDMI4ND[]=HDMI4ND_SOURCE_TEXT;
    #endif
  #endif
  #if ENABLE_DP_INPUT
    BYTE code strMW1Input_DP[]=DP_SOURCE_TEXT;
    #if ENABLE_DP2ND_INPUT
    BYTE code strMW1Input_DP2ND[]=DP2ND_SOURCE_TEXT;
    #endif
    #if ENABLE_DP3ND_INPUT
    BYTE code strMW1Input_DP3ND[]=DP3ND_SOURCE_TEXT;
    #endif
    #if ENABLE_DP4ND_INPUT
    BYTE code strMW1Input_DP4ND[]=DP4ND_SOURCE_TEXT;
    #endif
    #if ENABLE_DP5ND_INPUT
    BYTE code strMW1Input_DP5ND[]=DP5ND_SOURCE_TEXT;
    #endif
    #if ENABLE_DP6ND_INPUT
    BYTE code strMW1Input_DP6ND[]=DP6ND_SOURCE_TEXT;
    #endif
  #endif

    tPropText[1][0]=MM_MW1_INPUT_SOURCE_TEXT_START;
    pstPropFontSet1218=tPropFontSetArialNarrow13Bold;
    g_u8PropFontFlags=SPACE2PIXEL|RIGHT_ALIGN;
  #if ENABLE_VGA_INPUT
    if (MapWin2Port(DISPLAY_WIN1)==INPUT_VGA)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW1Input_VGA);
  #endif
  #if ENABLE_DVI_INPUT
    if (MapWin2Port(DISPLAY_WIN1)==INPUT_DVI)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW1Input_DVI);
    #if ENABLE_DVI2ND_INPUT
    if (MapWin2Port(DISPLAY_WIN1)==INPUT_DVI2ND)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW1Input_DVI2ND);
    #endif
    #if ENABLE_DVI3ND_INPUT
    if (MapWin2Port(DISPLAY_WIN1)==INPUT_DVI3ND)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW1Input_DVI3ND);
    #endif
  #endif
  #if ENABLE_HDMI_INPUT
    if (MapWin2Port(DISPLAY_WIN1)==INPUT_HDMI)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW1Input_HDMI);
    #if ENABLE_HDMI2ND_INPUT
    if (MapWin2Port(DISPLAY_WIN1)==INPUT_HDMI2ND)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW1Input_HDMI2ND);
    #endif
    #if ENABLE_HDMI3ND_INPUT
    if (MapWin2Port(DISPLAY_WIN1)==INPUT_HDMI3ND)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW1Input_HDMI3ND);
    #endif
    #if ENABLE_HDMI4ND_INPUT
    if (MapWin2Port(DISPLAY_WIN1)==INPUT_HDMI4ND)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW1Input_HDMI4ND);
    #endif

  #endif
  #if ENABLE_DP_INPUT
    if (MapWin2Port(DISPLAY_WIN1)==INPUT_DISPLAYPORT)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW1Input_DP);
    #if ENABLE_DP2ND_INPUT
    if (MapWin2Port(DISPLAY_WIN1)==INPUT_DISPLAYPORT2ND)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW1Input_DP2ND);
    #endif
    #if ENABLE_DP3ND_INPUT
    if (MapWin2Port(DISPLAY_WIN1)==INPUT_DISPLAYPORT3ND)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW1Input_DP3ND);
    #endif
    #if ENABLE_DP4ND_INPUT
    if (MapWin2Port(DISPLAY_WIN1)==INPUT_DISPLAYPORT4ND)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW1Input_DP4ND);
    #endif
    #if ENABLE_DP5ND_INPUT
    if (MapWin2Port(DISPLAY_WIN1)==INPUT_DISPLAYPORT5ND)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW1Input_DP5ND);
    #endif
    #if ENABLE_DP6ND_INPUT
    if (MapWin2Port(DISPLAY_WIN1)==INPUT_DISPLAYPORT6ND)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW1Input_DP6ND);
    #endif

  #endif
    return tPropText[1];
}
BYTE* InputSourceText_MultiWindow2(void)
{
  #if ENABLE_VGA_INPUT
    BYTE code strMW2Input_VGA[]=VGA_SOURCE_TEXT;
  #endif
  #if ENABLE_DVI_INPUT
    BYTE code strMW2Input_DVI[]=DVI_SOURCE_TEXT;
    #if ENABLE_DVI2ND_INPUT
    BYTE code strMW2Input_DVI2ND[]=DVI2ND_SOURCE_TEXT;
    #endif
    #if ENABLE_DVI3ND_INPUT
    BYTE code strMW2Input_DVI3ND[]=DVI3ND_SOURCE_TEXT;
    #endif
  #endif
  #if ENABLE_HDMI_INPUT
    BYTE code strMW2Input_HDMI[]=HDMI_SOURCE_TEXT;
    #if ENABLE_HDMI2ND_INPUT
    BYTE code strMW2Input_HDMI2ND[]=HDMI2ND_SOURCE_TEXT;
    #endif
    #if ENABLE_HDMI3ND_INPUT
    BYTE code strMW2Input_HDMI3ND[]=HDMI3ND_SOURCE_TEXT;
    #endif
    #if ENABLE_HDMI4ND_INPUT
    BYTE code strMW2Input_HDMI4ND[]=HDMI4ND_SOURCE_TEXT;
    #endif
  #endif
  #if ENABLE_DP_INPUT
    BYTE code strMW2Input_DP[]=DP_SOURCE_TEXT;
    #if ENABLE_DP2ND_INPUT
    BYTE code strMW2Input_DP2ND[]=DP2ND_SOURCE_TEXT;
    #endif
    #if ENABLE_DP3ND_INPUT
    BYTE code strMW2Input_DP3ND[]=DP3ND_SOURCE_TEXT;
    #endif
    #if ENABLE_DP4ND_INPUT
    BYTE code strMW2Input_DP4ND[]=DP4ND_SOURCE_TEXT;
    #endif
    #if ENABLE_DP5ND_INPUT
    BYTE code strMW2Input_DP5ND[]=DP5ND_SOURCE_TEXT;
    #endif
    #if ENABLE_DP6ND_INPUT
    BYTE code strMW2Input_DP6ND[]=DP6ND_SOURCE_TEXT;
    #endif
  #endif

    tPropText[1][0]=MM_MW2_INPUT_SOURCE_TEXT_START;
    pstPropFontSet1218=tPropFontSetArialNarrow13Bold;
    g_u8PropFontFlags=SPACE2PIXEL|RIGHT_ALIGN;
  #if ENABLE_VGA_INPUT
    if (MapWin2Port(DISPLAY_WIN2)==INPUT_VGA)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW2Input_VGA);
  #endif
  #if ENABLE_DVI_INPUT
    if (MapWin2Port(DISPLAY_WIN2)==INPUT_DVI)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW2Input_DVI);
    #if ENABLE_DVI2ND_INPUT
    if (MapWin2Port(DISPLAY_WIN2)==INPUT_DVI2ND)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW2Input_DVI2ND);
    #endif
    #if ENABLE_DVI3ND_INPUT
    if (MapWin2Port(DISPLAY_WIN2)==INPUT_DVI3ND)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW2Input_DVI3ND);
    #endif
  #endif
  #if ENABLE_HDMI_INPUT
    if (MapWin2Port(DISPLAY_WIN2)==INPUT_HDMI)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW2Input_HDMI);
    #if ENABLE_HDMI2ND_INPUT
    if (MapWin2Port(DISPLAY_WIN2)==INPUT_HDMI2ND)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW2Input_HDMI2ND);
    #endif
    #if ENABLE_HDMI3ND_INPUT
    if (MapWin2Port(DISPLAY_WIN2)==INPUT_HDMI3ND)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW2Input_HDMI3ND);
    #endif
    #if ENABLE_HDMI4ND_INPUT
    if (MapWin2Port(DISPLAY_WIN2)==INPUT_HDMI4ND)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW2Input_HDMI4ND);
    #endif

  #endif
  #if ENABLE_DP_INPUT
    if (MapWin2Port(DISPLAY_WIN2)==INPUT_DISPLAYPORT)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW2Input_DP);
    #if ENABLE_DP2ND_INPUT
    if (MapWin2Port(DISPLAY_WIN2)==INPUT_DISPLAYPORT2ND)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW2Input_DP2ND);
    #endif
    #if ENABLE_DP3ND_INPUT
    if (MapWin2Port(DISPLAY_WIN2)==INPUT_DISPLAYPORT3ND)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW2Input_DP3ND);
    #endif
    #if ENABLE_DP4ND_INPUT
    if (MapWin2Port(DISPLAY_WIN2)==INPUT_DISPLAYPORT4ND)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW2Input_DP4ND);
    #endif
    #if ENABLE_DP5ND_INPUT
    if (MapWin2Port(DISPLAY_WIN2)==INPUT_DISPLAYPORT5ND)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW2Input_DP5ND);
    #endif
    #if ENABLE_DP6ND_INPUT
    if (MapWin2Port(DISPLAY_WIN2)==INPUT_DISPLAYPORT6ND)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW2Input_DP6ND);
    #endif

  #endif
    return tPropText[1];
}
BYTE* InputSourceText_MultiWindow3(void)
{
  #if ENABLE_VGA_INPUT
    BYTE code strMW3Input_VGA[]=VGA_SOURCE_TEXT;
  #endif
  #if ENABLE_DVI_INPUT
    BYTE code strMW3Input_DVI[]=DVI_SOURCE_TEXT;
    #if ENABLE_DVI2ND_INPUT
    BYTE code strMW3Input_DVI2ND[]=DVI2ND_SOURCE_TEXT;
    #endif
    #if ENABLE_DVI3ND_INPUT
    BYTE code strMW3Input_DVI3ND[]=DVI3ND_SOURCE_TEXT;
    #endif
  #endif
  #if ENABLE_HDMI_INPUT
    BYTE code strMW3Input_HDMI[]=HDMI_SOURCE_TEXT;
    #if ENABLE_HDMI2ND_INPUT
    BYTE code strMW3Input_HDMI2ND[]=HDMI2ND_SOURCE_TEXT;
    #endif
    #if ENABLE_HDMI3ND_INPUT
    BYTE code strMW3Input_HDMI3ND[]=HDMI3ND_SOURCE_TEXT;
    #endif
    #if ENABLE_HDMI4ND_INPUT
    BYTE code strMW3Input_HDMI4ND[]=HDMI4ND_SOURCE_TEXT;
    #endif
  #endif
  #if ENABLE_DP_INPUT
    BYTE code strMW3Input_DP[]=DP_SOURCE_TEXT;
    #if ENABLE_DP2ND_INPUT
    BYTE code strMW3Input_DP2ND[]=DP2ND_SOURCE_TEXT;
    #endif
    #if ENABLE_DP3ND_INPUT
    BYTE code strMW3Input_DP3ND[]=DP3ND_SOURCE_TEXT;
    #endif
    #if ENABLE_DP4ND_INPUT
    BYTE code strMW3Input_DP4ND[]=DP4ND_SOURCE_TEXT;
    #endif
    #if ENABLE_DP5ND_INPUT
    BYTE code strMW3Input_DP5ND[]=DP5ND_SOURCE_TEXT;
    #endif
    #if ENABLE_DP6ND_INPUT
    BYTE code strMW3Input_DP6ND[]=DP6ND_SOURCE_TEXT;
    #endif

  #endif

    tPropText[1][0]=MM_MW3_INPUT_SOURCE_TEXT_START;
    pstPropFontSet1218=tPropFontSetArialNarrow13Bold;
    g_u8PropFontFlags=SPACE2PIXEL|RIGHT_ALIGN;
  #if ENABLE_VGA_INPUT
    if (MapWin2Port(DISPLAY_WIN3)==INPUT_VGA)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW3Input_VGA);
  #endif
  #if ENABLE_DVI_INPUT
    if (MapWin2Port(DISPLAY_WIN3)==INPUT_DVI)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW3Input_DVI);
    #if ENABLE_DVI2ND_INPUT
    if (MapWin2Port(DISPLAY_WIN3)==INPUT_DVI2ND)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW3Input_DVI2ND);
    #endif
    #if ENABLE_DVI3ND_INPUT
    if (MapWin2Port(DISPLAY_WIN3)==INPUT_DVI3ND)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW3Input_DVI3ND);
    #endif
  #endif
  #if ENABLE_HDMI_INPUT
    if (MapWin2Port(DISPLAY_WIN3)==INPUT_HDMI)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW3Input_HDMI);
    #if ENABLE_HDMI2ND_INPUT
    if (MapWin2Port(DISPLAY_WIN3)==INPUT_HDMI2ND)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW3Input_HDMI2ND);
    #endif
    #if ENABLE_HDMI3ND_INPUT
    if (MapWin2Port(DISPLAY_WIN3)==INPUT_HDMI3ND)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW3Input_HDMI3ND);
    #endif
    #if ENABLE_HDMI4ND_INPUT
    if (MapWin2Port(DISPLAY_WIN3)==INPUT_HDMI4ND)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW3Input_HDMI4ND);
    #endif

  #endif
  #if ENABLE_DP_INPUT
    if (MapWin2Port(DISPLAY_WIN3)==INPUT_DISPLAYPORT)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW3Input_DP);
    #if ENABLE_DP2ND_INPUT
    if (MapWin2Port(DISPLAY_WIN3)==INPUT_DISPLAYPORT2ND)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW3Input_DP2ND);
    #endif
    #if ENABLE_DP3ND_INPUT
    if (MapWin2Port(DISPLAY_WIN3)==INPUT_DISPLAYPORT3ND)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW3Input_DP3ND);
    #endif
    #if ENABLE_DP4ND_INPUT
    if (MapWin2Port(DISPLAY_WIN3)==INPUT_DISPLAYPORT4ND)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW3Input_DP4ND);
    #endif
    #if ENABLE_DP5ND_INPUT
    if (MapWin2Port(DISPLAY_WIN3)==INPUT_DISPLAYPORT5ND)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW3Input_DP5ND);
    #endif
    #if ENABLE_DP6ND_INPUT
    if (MapWin2Port(DISPLAY_WIN3)==INPUT_DISPLAYPORT6ND)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW3Input_DP6ND);
    #endif

  #endif
    return tPropText[1];
}
BYTE* InputSourceText_MultiWindow4(void)
{
  #if ENABLE_VGA_INPUT
    BYTE code strMW4Input_VGA[]=VGA_SOURCE_TEXT;
  #endif
  #if ENABLE_DVI_INPUT
    BYTE code strMW4Input_DVI[]=DVI_SOURCE_TEXT;
    #if ENABLE_DVI2ND_INPUT
    BYTE code strMW4Input_DVI2ND[]=DVI2ND_SOURCE_TEXT;
    #endif
    #if ENABLE_DVI3ND_INPUT
    BYTE code strMW4Input_DVI3ND[]=DVI3ND_SOURCE_TEXT;
    #endif
  #endif
  #if ENABLE_HDMI_INPUT
    BYTE code strMW4Input_HDMI[]=HDMI_SOURCE_TEXT;
    #if ENABLE_HDMI2ND_INPUT
    BYTE code strMW4Input_HDMI2ND[]=HDMI2ND_SOURCE_TEXT;
    #endif
    #if ENABLE_HDMI3ND_INPUT
    BYTE code strMW4Input_HDMI3ND[]=HDMI3ND_SOURCE_TEXT;
    #endif
    #if ENABLE_HDMI4ND_INPUT
    BYTE code strMW4Input_HDMI4ND[]=HDMI4ND_SOURCE_TEXT;
    #endif

  #endif
  #if ENABLE_DP_INPUT
    BYTE code strMW4Input_DP[]=DP_SOURCE_TEXT;
    #if ENABLE_DP2ND_INPUT
    BYTE code strMW4Input_DP2ND[]=DP2ND_SOURCE_TEXT;
    #endif
    #if ENABLE_DP3ND_INPUT
    BYTE code strMW4Input_DP3ND[]=DP3ND_SOURCE_TEXT;
    #endif
    #if ENABLE_DP4ND_INPUT
    BYTE code strMW4Input_DP4ND[]=DP4ND_SOURCE_TEXT;
    #endif
    #if ENABLE_DP5ND_INPUT
    BYTE code strMW4Input_DP5ND[]=DP5ND_SOURCE_TEXT;
    #endif
    #if ENABLE_DP6ND_INPUT
    BYTE code strMW4Input_DP6ND[]=DP6ND_SOURCE_TEXT;
    #endif

  #endif

    tPropText[1][0]=MM_MW4_INPUT_SOURCE_TEXT_START;
    pstPropFontSet1218=tPropFontSetArialNarrow13Bold;
    g_u8PropFontFlags=SPACE2PIXEL|RIGHT_ALIGN;
  #if ENABLE_VGA_INPUT
    if (MapWin2Port(DISPLAY_WIN4)==INPUT_VGA)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW4Input_VGA);
  #endif
  #if ENABLE_DVI_INPUT
    if (MapWin2Port(DISPLAY_WIN4)==INPUT_DVI)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW4Input_DVI);
    #if ENABLE_DVI2ND_INPUT
    if (MapWin2Port(DISPLAY_WIN4)==INPUT_DVI2ND)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW4Input_DVI2ND);
    #endif
    #if ENABLE_DVI3ND_INPUT
    if (MapWin2Port(DISPLAY_WIN4)==INPUT_DVI3ND)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW4Input_DVI3ND);
    #endif
  #endif
  #if ENABLE_HDMI_INPUT
    if (MapWin2Port(DISPLAY_WIN4)==INPUT_HDMI)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW4Input_HDMI);
    #if ENABLE_HDMI2ND_INPUT
    if (MapWin2Port(DISPLAY_WIN4)==INPUT_HDMI2ND)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW4Input_HDMI2ND);
    #endif
    #if ENABLE_HDMI3ND_INPUT
    if (MapWin2Port(DISPLAY_WIN4)==INPUT_HDMI3ND)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW4Input_HDMI3ND);
    #endif
    #if ENABLE_HDMI4ND_INPUT
    if (MapWin2Port(DISPLAY_WIN4)==INPUT_HDMI4ND)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW4Input_HDMI4ND);
    #endif

  #endif
  #if ENABLE_DP_INPUT
    if (MapWin2Port(DISPLAY_WIN4)==INPUT_DISPLAYPORT)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW4Input_DP);
    #if ENABLE_DP2ND_INPUT
    if (MapWin2Port(DISPLAY_WIN4)==INPUT_DISPLAYPORT2ND)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW4Input_DP2ND);
    #endif
    #if ENABLE_DP3ND_INPUT
    if (MapWin2Port(DISPLAY_WIN4)==INPUT_DISPLAYPORT3ND)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW4Input_DP3ND);
    #endif
    #if ENABLE_DP4ND_INPUT
    if (MapWin2Port(DISPLAY_WIN4)==INPUT_DISPLAYPORT4ND)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW4Input_DP4ND);
    #endif
    #if ENABLE_DP5ND_INPUT
    if (MapWin2Port(DISPLAY_WIN4)==INPUT_DISPLAYPORT5ND)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW4Input_DP5ND);
    #endif
    #if ENABLE_DP6ND_INPUT
    if (MapWin2Port(DISPLAY_WIN4)==INPUT_DISPLAYPORT6ND)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW4Input_DP6ND);
    #endif

  #endif
    return tPropText[1];
}
#else
BYTE* InputSourceText_MultiWindow1(void)
{
  #if ENABLE_VGA_INPUT
    BYTE code strMW1Input_VGA[]=VGA_SOURCE_TEXT;
  #endif
  #if ENABLE_DVI_INPUT
    BYTE code strMW1Input_DVI[]=DVI_SOURCE_TEXT;
    #if ENABLE_DVI2ND_INPUT
    BYTE code strMW1Input_DVI2ND[]=DVI2ND_SOURCE_TEXT;
    #endif
    #if ENABLE_DVI3ND_INPUT
    BYTE code strMW1Input_DVI3ND[]=DVI3ND_SOURCE_TEXT;
    #endif
  #endif
  #if ENABLE_HDMI_INPUT
    BYTE code strMW1Input_HDMI[]=HDMI_SOURCE_TEXT;
    #if ENABLE_HDMI2ND_INPUT
    BYTE code strMW1Input_HDMI2ND[]=HDMI2ND_SOURCE_TEXT;
    #endif
    #if ENABLE_HDMI3ND_INPUT
    BYTE code strMW1Input_HDMI3ND[]=HDMI3ND_SOURCE_TEXT;
    #endif
    #if ENABLE_HDMI4ND_INPUT
    BYTE code strMW1Input_HDMI4ND[]=HDMI4ND_SOURCE_TEXT;
    #endif
  #endif
  #if ENABLE_DP_INPUT
    BYTE code strMW1Input_DP[]=DP_SOURCE_TEXT;
    #if ENABLE_DP2ND_INPUT
    BYTE code strMW1Input_DP2ND[]=DP2ND_SOURCE_TEXT;
    #endif
    #if ENABLE_DP3ND_INPUT
    BYTE code strMW1Input_DP3ND[]=DP3ND_SOURCE_TEXT;
    #endif
    #if ENABLE_DP4ND_INPUT
    BYTE code strMW1Input_DP4ND[]=DP4ND_SOURCE_TEXT;
    #endif
    #if ENABLE_DP5ND_INPUT
    BYTE code strMW1Input_DP5ND[]=DP5ND_SOURCE_TEXT;
    #endif
    #if ENABLE_DP6ND_INPUT
    BYTE code strMW1Input_DP6ND[]=DP6ND_SOURCE_TEXT;
    #endif
  #endif



#if ENABLE_MULTI_WINDOW_SETTINGS_MENU
    tPropText[1][0]=MM_MW1_INPUT_SOURCE_TEXT_START;
#else
    tPropText[1][0]=MM_INPUT_SOURCE_TEXT_START;
#endif
    pstPropFontSet1218=tPropFontSetArialNarrow13Bold;
    g_u8PropFontFlags=SPACE2PIXEL|RIGHT_ALIGN;


				
  #if ENABLE_VGA_INPUT
    if (MapWin2Port(DISPLAY_WIN1)==INPUT_VGA)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW1Input_VGA);
  #endif
  #if ENABLE_DVI_INPUT
 //   if (MapWin2Port(DISPLAY_WIN1)==INPUT_DVI)
   if(MapWin2Port(InputStatusWinNo)==INPUT_DVI)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW1Input_DVI);
    #if ENABLE_DVI2ND_INPUT
    if (MapWin2Port(DISPLAY_WIN1)==INPUT_DVI2ND)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW1Input_DVI2ND);
    #endif
    #if ENABLE_DVI3ND_INPUT
    if (MapWin2Port(DISPLAY_WIN1)==INPUT_DVI3ND)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW1Input_DVI3ND);
    #endif
  #endif
  #if ENABLE_HDMI_INPUT
    if(MapWin2Port(InputStatusWinNo)==INPUT_HDMI)
//    if (MapWin2Port(DISPLAY_WIN1)==INPUT_HDMI)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW1Input_HDMI);
    #if ENABLE_HDMI2ND_INPUT
    if (MapWin2Port(DISPLAY_WIN1)==INPUT_HDMI2ND)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW1Input_HDMI2ND);
    #endif
    #if ENABLE_HDMI3ND_INPUT
    if (MapWin2Port(DISPLAY_WIN1)==INPUT_HDMI3ND)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW1Input_HDMI3ND);
    #endif
    #if ENABLE_HDMI4ND_INPUT
    if (MapWin2Port(DISPLAY_WIN1)==INPUT_HDMI4ND)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW1Input_HDMI4ND);
    #endif

  #endif
  #if ENABLE_DP_INPUT
    if (MapWin2Port(DISPLAY_WIN1)==INPUT_DISPLAYPORT)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW1Input_DP);
    #if ENABLE_DP2ND_INPUT
    if (MapWin2Port(DISPLAY_WIN1)==INPUT_DISPLAYPORT2ND)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW1Input_DP2ND);
    #endif
    #if ENABLE_DP3ND_INPUT
    if (MapWin2Port(DISPLAY_WIN1)==INPUT_DISPLAYPORT3ND)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW1Input_DP3ND);
    #endif
    #if ENABLE_DP4ND_INPUT
    if (MapWin2Port(DISPLAY_WIN1)==INPUT_DISPLAYPORT4ND)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW1Input_DP4ND);
    #endif
    #if ENABLE_DP5ND_INPUT
    if (MapWin2Port(DISPLAY_WIN1)==INPUT_DISPLAYPORT5ND)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW1Input_DP5ND);
    #endif
    #if ENABLE_DP6ND_INPUT
    if (MapWin2Port(DISPLAY_WIN1)==INPUT_DISPLAYPORT6ND)
        tPropText[1][1]=LoadPropFontText(tPropText[1][0],strMW1Input_DP6ND);
    #endif

  #endif
    return tPropText[1];
}


#endif //ENABLE_MULTI_WINDOW_SETTINGS_MENU
#if 0
#define WEB_SITE_TEXT    "www.mstarsemi.com"
BYTE* MenuWebSiteText(void)
{
    BYTE code strWebSite[]=WEB_SITE_TEXT;

    tPropText[1][0]=MM_WEB_SITE_TEXT_START;
    pstPropFontSet1218=tPropFontSetArialNarrow13Bold;
    g_u8PropFontFlags=SPACE2PIXEL|RIGHT_ALIGN;
    tPropText[1][1]=LoadPropFontText(tPropText[1][0],strWebSite);
    return tPropText[1];
}
#endif

#endif
#if 0//ENABLE_MULTI_INPUT
BYTE* InputMenuNumberText(void)
{
    BYTE xdata strNumber[]="1: ";
    extern BYTE xdata   g_u8MenuTempValue;

//Jison 090525 Input Menu will not showing by pressed src input key
    if (!g_stMenuFlag.bPressSrcInputKey)
        g_u8MenuTempValue = USER_PREF_INPUT_PRIORITY;

    strNumber[0]=g_u8MenuTempValue+'1';
    tPropText[1][0]=INPUT_MENU_NUMBER_START;
    if (IS_ASIA_LANGUAGE())
    {
        pstPropFontSet1218=tPropFontSetArialNarrow13;
    }
    else
    {
        pstPropFontSet1218=tPropFontSetArialNarrow13Bold;//tPropFontSetArialNarrow13; FY12_MSG
    }
    g_u8PropFontFlags=SPACE2PIXEL|RIGHT_ALIGN;
    tPropText[1][1]=LoadPropFontText(tPropText[1][0],strNumber);
    return tPropText[1];
}
#endif
#if (defined(FengYuan_Project)||defined(XuLi_Project)||defined(MingCai_Project))


BYTE* FactoryVersionText(WORD u8Address)
{
    BYTE code strFactoryVersion[]=VERSION;
	
    tPropText[2][0]=u8Address;
	
    //tPropText[2][0]=MM_INPUT_SOURCE_TEXT_START;
    pstPropFontSet1218=tPropFontSetArialNarrow13Bold;
    g_u8PropFontFlags=SPACE2PIXEL|RIGHT_ALIGN;
    
    tPropText[2][1]=LoadPropFontText(tPropText[2][0],strFactoryVersion);
    return tPropText[2];
}
#endif

#if 0
BYTE *TviVersion(void)
{
    BYTE code strVersion1[] = FACTORY_VERSION;
    tPropText[1][0] = FM_VERSION_START;
    pstPropFontSet1218=tPropFontSetArialNarrow13Bold;
    g_u8PropFontFlags = SPACE2PIXEL | LEFT_ALIGN;
    tPropText[1][1] = LoadPropFontText(tPropText[1][0], strVersion1);
    return tPropText[1];
}
#endif

#if 0//For 1908/2208WFP series
static void ClearOneFont(BYTE u8Addr)
{
    WORD  xdata tSN[FONT_HEIGHT];

    ClearFontBuf(tSN);
    mStar_WriteByte(OSD2_A3, u8Addr);
    OSDLoadOneFont(tSN);
}
#endif
#define NUM2ASCII(Num) ((Num)+AF_ZERO_CHAR)
BYTE LoadPropNumber(BYTE u8Addr,BYTE u8Value,BYTE u8Flag)
{
    BYTE tNumberText[]={AF_BLANK_CHAR, AF_BLANK_CHAR, AF_BLANK_CHAR, AF_BLANK_CHAR, AF_BLANK_CHAR, 0};
    //BYTE u8LeadingBlank=(u8Value&BIT7);

    //u8Value&=0x7F;
#if ENABLE_MSTV_UART_DEBUG
    if (u8Value>100)
        PROPFONTEXTFUNC_PRINT("Value OoR\n");
#endif
    if (u8Value<10)
    {
        tNumberText[1]=NUM2ASCII(u8Value);
        if (u8Flag&NUM_SEC)
            tNumberText[3]=AF_S_LOWERCASE_CHAR;
    }
    else if (u8Value<100)
    {
        tNumberText[1]=NUM2ASCII(u8Value/10);
        tNumberText[2]=NUM2ASCII(u8Value%10);
        if (u8Flag&NUM_SEC)
            tNumberText[4]=AF_S_LOWERCASE_CHAR;
    }
    else
    {
        tNumberText[1]=NUM2ASCII(1);
        tNumberText[2]=NUM2ASCII(0);
        tNumberText[3]=NUM2ASCII(0);
    }
	
    pstPropFontSet1218=tPropFontSetArialNarrow13Bold;
    g_u8PropFontFlags=SPACE2PIXEL|LEFT_ALIGN;
    
    g_u8AlignResetIndex=0xFF;
    if (u8Flag&NUM_LEADING_BLANK)
        return LoadPropFonts1218(u8Addr, tNumberText,6, NULL,0, 0, 0);
    return LoadPropFonts1218(u8Addr, tNumberText+1,5, NULL,0, 0, 0);
}
#if 0
BYTE LoadTimePropNumber(BYTE u8Addr,WORD u16Sec) //FY12_MSG
{
    BYTE tNumberText[]={'0',AF_COLON_CHAR,'0','0',NULL};
    BYTE u8Value;

    u8Value=u16Sec/60; //get min
    tNumberText[0]=NUM2ASCII(u8Value);
    u8Value=(u16Sec%60)/10; //get sec
    tNumberText[2]=NUM2ASCII(u8Value);
    u8Value=(u16Sec%60)%10; //get sec
    tNumberText[3]=NUM2ASCII(u8Value);

    if (IS_ASIA_LANGUAGE())
        pstPropFontSet1218=tPropFontSetArialNarrow13;
    else
        pstPropFontSet1218=tPropFontSetArialNarrow13Bold;
    g_u8PropFontFlags=SPACE3PIXEL|LEFT_ALIGN;
    g_u8AlignResetIndex=0xFF;
    return LoadPropFonts1218(u8Addr, tNumberText,4, NULL,0, 0, 0);
}
#endif
#if 0
void LoadChar(BYTE u8Addr, BYTE u8Ascii, BYTE u8UDShift)
{
    BYTE tNumberText[]={AF_BLANK_CHAR, NULL};
    tNumberText[0]=GetRealFontIndex(u8Ascii);
    LoadPropFonts1218(u8Addr, tNumberText,1, NULL,0, u8UDShift, 0);
}
#endif






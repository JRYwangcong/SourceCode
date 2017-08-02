#ifndef _ADJSUT_H_
#define _ADJSUT_H_

#ifdef _ADJSUT_C_
  #define _ADJSUTDEC_
#else
  #define _ADJSUTDEC_ extern
#endif

typedef struct
{
  BYTE u8Red;
  BYTE u8Green;
  BYTE u8Blue;
} ColorType;


typedef enum
{
//Bit:
//  7   6   5   4   3   2   1   0
//  R   R   R   G   G   G   B   B

    ePATTERN_COLOR_RED      = 0xE0,
    ePATTERN_COLOR_GREEN    = 0x1C,
    ePATTERN_COLOR_BLUE     = 0x03,
    ePATTERN_COLOR_CYNA     = 0x1F,
    ePATTERN_COLOR_MEGENTA  = 0xE3,
    ePATTERN_COLOR_YELLOW   = 0xFC,
    ePATTERN_COLOR_WHITE    = 0xFF,
    ePATTERN_COLOR_BLACK    = 0x00,
}BGColorType;

enum
{
    eCUSTOM_HUE_R,
    eCUSTOM_HUE_G,
    eCUSTOM_HUE_B,
    eCUSTOM_HUE_C,
    eCUSTOM_HUE_M,
    eCUSTOM_HUE_Y,
};

typedef enum
{
    eBorderInside,
    eBorderOutside,
}BorderDirectionType;

//_ADJSUTDEC_ BGColorType tblTestPattern[8];
extern BGColorType tblTestPattern[8]; // use extern since init value is assigned/declared in Adjust.c

_ADJSUTDEC_ void msAPI_EnableBorder(BYTE u8WinIdx, Bool bEnable);
_ADJSUTDEC_ void msAPI_SetBorderFormat(BYTE u8WinIdx, BYTE u8Left, BYTE u8Right, BYTE u8Up, BYTE u8Down, BYTE u8color);
_ADJSUTDEC_ void msAPI_SetBorderBatchFormat(BYTE u8WinIdx, BYTE u8Width, BorderDirectionType u8Direction, BYTE u8Color);

_ADJSUTDEC_ void msAPI_MWEActive(BYTE u8WinIdx, Bool bEnable);

_ADJSUTDEC_ void msAPI_AdjustSharpness(BYTE u8DispWin, BYTE u8Sharpness, BYTE u8Step);
_ADJSUTDEC_ void msAPI_AdjustBrightness( BYTE u8Brightness );
_ADJSUTDEC_ void msAPI_AdjustBlacklevel(BYTE u8DispWin, WORD u16Redblacklevel, WORD u16Greenblacklevel, WORD u16Blueblacklevel);
_ADJSUTDEC_ void msAPI_AdjustUserPrefBlacklevel( BYTE Redblacklevel, BYTE Greenblacklevel, BYTE Blueblacklevel );
_ADJSUTDEC_ void msAPI_AdjustContrast( BYTE u8DispWin , WORD u16Contrast );

_ADJSUTDEC_ void msAPI_AdjustHPosition( BYTE u8DispWin , WORD u16Position);
_ADJSUTDEC_ void msAPI_AdjustVPosition( BYTE u8DispWin , WORD u16Position);

_ADJSUTDEC_ void msAPI_AdjustOutputHPosition(BYTE u8DispWin,WORD u16ShiftAmount,BOOL bDirection);
_ADJSUTDEC_ void msAPI_AdjustOutputVPosition(BYTE u8DispWin,WORD u16ShiftAmount,BOOL bDirection);

_ADJSUTDEC_ Bool msAPI_SetInputCSC(BYTE u8DispWin, Bool bYUVDomain);

_ADJSUTDEC_ void msAPI_AdjustBackgoundColor( BYTE u8DispWin ,BGColorType u8Pattern);
_ADJSUTDEC_ void msAPI_ForceFrameColor(BOOL bEnable, BYTE u8Red, BYTE u8Green, BYTE u8Blue);
#if ENABLE_VGA_INPUT
_ADJSUTDEC_ void msAPI_AdjustVGASourceType( BYTE u8DispWin);
#endif
_ADJSUTDEC_ void msAPI_AdjustRGBColor( BYTE u8DispWin, WORD u16Red, WORD u16Green, WORD u16Blue);

_ADJSUTDEC_ void msAPI_AdjustSaturation(BYTE u8WinIndex, BYTE u8Saturation);
_ADJSUTDEC_ void msAPI_AdjustVideoHue(BYTE u8WinIndex, WORD u8Hue);
_ADJSUTDEC_ void msAPI_CustomHueEnable(BYTE u8WinIndex, Bool bEnable);
_ADJSUTDEC_ void msAPI_AdjustCustomHue(BYTE u8WinIndex, BYTE u8HueValue, BYTE u8ColorIndex);
#if (CHIP_ID == MST9U3)
_ADJSUTDEC_ void msAPI_AdjustCustomSatRGBMode(BYTE u8WinIndex, BYTE u8Value, BYTE u8ColorIndex);
_ADJSUTDEC_ void msAPI_CustomSatEnableRGBMode(BYTE u8WinIndex, Bool bEnable);
_ADJSUTDEC_ void msAPI_AdjustCustomBriRGBMode(BYTE u8WinIndex, BYTE u8Value, BYTE u8ColorIndex);
_ADJSUTDEC_ void msAPI_CustomBriEnableRGBMode(BYTE u8WinIndex, Bool bEnable);
#endif
//_ADJSUTDEC_ void msAPI_HistogramSetRange(BYTE u8WinIndex);
_ADJSUTDEC_ void msAPI_AutoSetHistogramRange(BYTE u8WinIndex);
_ADJSUTDEC_ void msAPI_HistogramReportEnable(BYTE u8WinIdx);
_ADJSUTDEC_ Bool msAPI_HistogramCheckACK(BYTE u8WinIdx);
_ADJSUTDEC_ void msAPI_HistogramGetData(BYTE u8WinIndex, BYTE u8Section, DWORD * pu32Data);
_ADJSUTDEC_ void msAPI_HistogramUpDateData(BYTE u8WinIndex);

_ADJSUTDEC_ void msAPI_OverDriveInit(void);
#if ENABLE_OD_MTB
_ADJSUTDEC_ void msAPI_OverDriveMultiTable(BYTE index);
#else
_ADJSUTDEC_ void msAPI_OverDriveTable(BYTE *pODTbl);
#endif
_ADJSUTDEC_ void msAPI_OverDriveEnable(BOOL bEnable);
_ADJSUTDEC_ void msAPI_DeGammaLoadTbl_256E_12B(BYTE u8WinIndex, WORD **pu8Table);
_ADJSUTDEC_ void msAPI_InitialVideoColorMatrix(BYTE u8DispWin, WORD wHue, BYTE ucSaturation, WORD u16Contrast, WORD wRCon, WORD wGCon, WORD wBCon);

_ADJSUTDEC_ void msAPI_GammaLoadTbl_256E_8B(BYTE u8WinIndex, BYTE **pu8TableIndex);
_ADJSUTDEC_ void msAPI_GammaLoadTbl_256E_10B(BYTE u8WinIndex, BYTE **pu8TableIndex);
_ADJSUTDEC_ void msAPI_GammaLoadTbl_1024E_14B(BYTE u8WinIndex, BYTE **pu8TableIndex);
_ADJSUTDEC_ void msAPI_GammaEnable(BYTE u8WinIndex, BYTE bEnable);
_ADJSUTDEC_ void msAPI_DeGammaLoadTbl(BYTE u8WinIndex, BYTE **pu8TableIndex);
_ADJSUTDEC_ void msAPI_DeGammaLoadTbl_Compressed(BYTE u8WinIndex, BYTE **pu8TableIndex, WORD** pu16SegPntTbl);
_ADJSUTDEC_ void msAPI_DeGammaEnable(BYTE u8WinIndex, BYTE bEnable);

_ADJSUTDEC_ void msAPI_FixGammaLoadTbl_256E_12B(BYTE u8WinIndex, BYTE **pu8TableIndex);
_ADJSUTDEC_ void msAPI_FixGammaLoadTbl_256E_16B(BYTE u8WinIndex, BYTE **pu8TableIndex);
_ADJSUTDEC_ void msAPI_FixGammaEnable(BYTE u8WinIndex, BYTE bEnable);
_ADJSUTDEC_ void msAPI_AdjustVolume(BYTE u8Volume);
_ADJSUTDEC_ void msAPI_SetDLCEnable(BYTE u8WinIndex, Bool bEnable);

_ADJSUTDEC_ void msAPI_SetDLCCurve(BYTE u8WinIndex, BYTE *u8Table);
_ADJSUTDEC_ void msAPI_LoadICCInitialTable(void);
_ADJSUTDEC_ void msAPI_AdjustCustomSaturation(BYTE u8WinIndex, BYTE u8HueValue, BYTE u8ColorIndex);
_ADJSUTDEC_ void msAPI_CustomSaturationEnable(BYTE u8WinIndex, Bool bEnable);

_ADJSUTDEC_ void  msAPI_AdjustVIPHue(BYTE u8WinIndex, BYTE u8HueValue, BYTE u8ColorIndex);
_ADJSUTDEC_ void  msAPI_VIPHueEnable(BYTE u8WinIndex, Bool bEnable);
_ADJSUTDEC_ void  msAPI_AdjustSkinTone(BYTE u8WinIndex, Bool bEnable, BYTE adjValue);

_ADJSUTDEC_ void msAPI_CustomBrightnessEnable(BYTE u8WinIndex, Bool bEnable);
_ADJSUTDEC_ void msAPI_AdjustCustomBrightness(BYTE u8WinIndex, BYTE u8BriValue, BYTE u8ColorIndex);
_ADJSUTDEC_ void msAPI_ColorMatrixEnable(BYTE u8WinIndex, BYTE bEnable, BYTE bCarryEn);
#if ENABLE_DEMURA_FUNCTION
_ADJSUTDEC_ void msAPI_DeMuraEnable(BYTE bEnable);
#endif

_ADJSUTDEC_ void TurnOffWholeUserColorMode(void);
_ADJSUTDEC_ void SetupUserColorMode(BOOL IsEn, BYTE* pUserColorMode);

#undef _ADJSUTDEC_
#endif

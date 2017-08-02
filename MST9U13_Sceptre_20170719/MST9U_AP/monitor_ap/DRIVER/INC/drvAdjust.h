
#ifndef _DRVADJUST_H_
#define _DRVADJUST_H_

///////////////////////////////////////
// DRVADJUST_V VERSION: V01
////////////////////////////////////////
/* BORDER */
extern void msDrvSetBorderFormat(BYTE u8WinIndex, BYTE u8Left, BYTE u8Right, BYTE u8Up, BYTE u8Down, BYTE u8color);
extern void msDrvEnableBorder(BYTE u8WinIndex, Bool bEnable);

/* MWE */
#if (ENABLE_MWE)
extern void msDrvMWE_GetScalerMask(BYTE u8WinIndex);
extern void msDrvMWE_ResetScalerMask(BYTE u8WinIndex);
#endif

/* HBR2 HPosition */
extern void msDrvAdjustHBR2HPosition(BYTE u8DispWin, WORD u16Position);

/* Sharpness */
extern void msDrvAdjustSharpness(BYTE u8WinIndex, BYTE u8SharpLevel, BYTE u8Step, EN_WINDOW_COLOR ecolor);

/* Black Level */
extern void msDrvBlackLevelEnable(BYTE u8WinIndex, Bool bEnable);
extern void msDrvAdjustBlacklevel(BYTE u8WinIndex, BYTE u8Redblacklevel, BYTE u8Greenblacklevel, BYTE u8Blueblacklevel);
extern void msDrvAdjustBlacklevel_10Bits(BYTE u8WinIndex, WORD u16Redblacklevel, WORD u16Greenblacklevel, WORD u16Blueblacklevel);

/* Custom Hue R/G/B/C/M/Y */
extern void msDrvCustomHueEnable(BYTE u8WinIdx, Bool bEnable);
extern void msDrvAdjustCustomHue(BYTE u8WinIdx, BYTE u8HueValue, BYTE u8RegOffset);

#if (CHIP_ID == MST9U3)
/* Custom Sat RGB mode R/G/B/C/M/Y */
extern void msDrvCustomSatEnableRGBMode(BYTE u8WinIdx, Bool bEnable);
extern void msDrvAdjustCustomSatRGBMode(BYTE u8WinIdx, BYTE u8Value, BYTE u8RegOffset);

/* Custom Bri RGB mode R/G/B/C/M/Y */
extern void msDrvCustomBriEnableRGBMode(BYTE u8WinIdx, Bool bEnable);
extern void msDrvAdjustCustomBriRGBMode(BYTE u8WinIdx, BYTE u8Value, BYTE u8RegOffset);
#endif

/*Gamma*/
extern void msDrvGammaLoadTbl_256E_8B(BYTE u8WinIdx, BYTE **pu8GammaTblIdx);
extern void msDrvGammaLoadTbl_256E_10B(BYTE u8WinIdx, BYTE **pu8GammaTblIdx);
extern void msDrvGammaLoadTbl_1024E_14B(BYTE u8WinIdx, BYTE **pu8GammaTblIdx);

extern void msDrvGammaWriteTbl_256E_8B(BYTE u8WinIdx, BYTE u8Color, BYTE **pu8GammaTblIdx);

#if Enable_AutoDownloadGamma
		extern void msDrvGamma_DMA_Reader_Trigger(BYTE u8WinIdx);
		extern void msDrvDeGamma_DMA_Reader_Trigger(BYTE u8WinIdx);
		extern void msDrvFixGamma_DMA_Reader_Trigger(BYTE u8WinIdx);
		extern void msDrvGamma_DMA_Reader_PreFetchEnble(BYTE u8WinIdx, Bool bEnable);
		extern void msDrvDeGammaLoadTbl_Compress_ByDMA(BYTE u8WinIdx, BYTE u8Color, BYTE **pu8GammaTblIdx );
		extern void msDrvDeGammaLoadTbl_Compress_Offset(BYTE u8WinIdx, BYTE u8Color, BYTE **pu8DeGammaTblIdx, WORD** pu16SegPntTbl);
		extern void msDrvDeGamma_Compress_DMA_Reader_Trigger(BYTE u8WinIdx);
		extern void msDrvDeGammaWriteTbl_256E_12B_ByDMA(BYTE u8WinIdx, BYTE u8Color, BYTE **pu8GammaTblIdx );
		extern void msDrvFixGammaWriteTbl_256E_12B_ByDMA(BYTE u8WinIdx, BYTE u8Color, BYTE **pu8GammaTblIdx );
		extern void msDrvGammaWriteTbl_1024E_14B_ByDMA(BYTE u8WinIdx, BYTE u8Color, BYTE **pu8GammaTblIdx );
		extern void msDrvGammaWriteTbl_256E_10B_ByDMA(BYTE u8WinIdx, BYTE u8Color, BYTE **pu8GammaTblIdx );
#else
		extern void msDrvGammaWriteTbl_1024E_14B(BYTE u8WinIdx, BYTE u8Color, BYTE **pu8GammaTblIdx);
		extern void msDrvGammaWriteTbl_256E_10B(BYTE u8WinIdx, BYTE u8Color, BYTE **pu8GammaTblIdx);
#endif


extern void msDrvGammaEnable(BYTE u8WinIdx, Bool bEnable);


/*de-Gamma*/
extern void msDrvDeGammaLoadTbl(BYTE u8WinIdx, BYTE u8Color,BYTE **pu8DeGammaTblIdx);
extern void msDrvDeGammaLoadTbl_Compress(BYTE u8WinIdx, BYTE u8Color,BYTE **pu8DeGammaTblIdx,WORD** pu16SegPntTbl);
extern void msDrvDeGammaLoadTbl_256E_12B(BYTE u8WinIdx, BYTE u8Color,WORD **pu8DeGammaTbl);
extern void msDrvDeGammaEnable(BYTE u8WinIdx, Bool bEnable);

/*fix gamma*/
extern void msDrvFixGammaLoadTbl_256E_10B(BYTE u8WinIdx, BYTE u8Color,  BYTE **pu8FixGammaTblIdx);
extern void msDrvFixGammaLoadTbl_256E_16B(BYTE u8WinIdx, BYTE u8Color,  BYTE **pu8FixGammaTblIdx);
extern void msDrvFixGammaEnable(BYTE u8WinIdx, Bool bEnable);

/*Histogram*/
extern xdata DWORD u32Histogram[DISPLAY_MAX_NUMS][32];
extern void msDrvHistogramYSetRGBMode(BYTE u8WinIdx, Bool u8Enable);
extern void msDrvAutoSetHistogramRange(BYTE u8WinIndex);
extern void msDrvHistogramReportEnable(BYTE u8WinIdx);
extern Bool msDrvHistogramCheckACK(BYTE u8WinIdx);
extern void msDrvHistogramSetRange(BYTE u8WinIndex);
extern void msDrvHistogramGetData(BYTE u8WinIndex, BYTE u8Section, DWORD * pu32Data);
extern void msDrvHistogramUpDateData(BYTE u8WinIndex);
extern void msDrvHistogramRangeEnable(BYTE u8WinIndex, BYTE bEnable);


/*CSC*/
extern void msDrvColorFormat_CSCEnable(BYTE u8WinIdx, BYTE u8CSCSel, Bool bEnable);
extern void msDrvColorFormat_YSub16Enable(BYTE u8WinIdx, BYTE u8YSUBSel, Bool bEnable);

/*DLC*/
extern void msDrvSetDLCCurve(BYTE u8WinIdx, BYTE* u8Table);
extern void msDrvSetDLCEnable(BYTE u8WinIdx, Bool bEnable, EN_WINDOW_COLOR ecolor);


/* ICC */
extern void msDrvLoadICCInitialTable(void);
extern void msDrvCustomSaturationEnable(BYTE u8WinIdx, Bool bEnable);
extern void msDrvAdjustCustomSaturation(BYTE u8WinIdx, BYTE u8HueValue, BYTE u8RegOffset);

/* IBC */
extern void msDrvCustomBrightnessEnable(BYTE u8WinIdx, Bool bEnable);
extern void msDrvAdjustCustomBrightness(BYTE u8WinIdx, BYTE u8BriValue, BYTE u8RegOffset);

/*XvYCC Color Matrix*/
extern void msDrvColorMatrixEnable(BYTE u8WinIdx, Bool bEnable, Bool bCarryEn);

/* IHC */
extern void msDrvVIPHueEnable(BYTE u8WinIdx, Bool bEnable);
extern void msDrvAdjustVIPHue(BYTE u8WinIdx, BYTE u8HueValue, BYTE u8RegOffset);

/* FCC */
extern void msDrvFCCEnable(BYTE u8WinIdx, BYTE FCCRegion, Bool bEnable);
extern void msDrvAdjustFCC(BYTE u8WinIdx, BYTE FCCRegion, BYTE Strength, BYTE CbTarget, BYTE CrTarget, BYTE Distance);

/* Get Window Color Report*/
extern void msDrvGetWinColorReport(WORD* minR, WORD *maxR, WORD *minG, WORD *maxG, WORD *minB, WORD *maxB);
#endif

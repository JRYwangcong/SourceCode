#ifndef __MSDLC_H
#define __MSDLC_H

#if 1//MST9U
extern WORD g_u16HistogramFlag;

#define HISTOGRAM_READY_BIT             BIT0
#define HISTOGRAM_READY_FLAG            (g_u16HistogramFlag & BIT0)
#define SET_HISTOGRAM_READY_FLAG()      (g_u16HistogramFlag |= BIT0)
#define CLR_HISTOGRAM_READY_FLAG()      (g_u16HistogramFlag &= ~BIT0)
#endif
extern XDATA BYTE g_ucFixLumaCurve[8][16];
extern XDATA BYTE g_ucLumaCurve[16];
extern XDATA BYTE g_ucPreLumaCurve[8][16];
extern XDATA BYTE g_LowStrength;
extern XDATA BYTE g_HighStrength;
extern XDATA BYTE g_bDLC_On;

//extern BYTE code t_MWEDLC_GraphicsGame_Table[];
//extern BYTE code t_MWEDLC_VideoMovie_Table[];
//extern BYTE code t_MWEDLC_VideoSports_Table[];
//extern BYTE code t_MWEDLC_VideoNature_Table[];
///////////////////////////////////////////////////////////////////////////////
// DLC(Dynamic luma curve
///////////////////////////////////////////////////////////////////////////////

extern void InitDLCVar(Bool IsDynamic, WORD wPanelWidth, WORD wPanelHeight);


// The value of Strength is small, and the effect is strong!
// Lstrength :  0.50(strong)   <------------>   1.70 (weak) ; default = 1.20
// Hstrength:   0.50(strong)   <------------>   2.00 (weak) ; default = 1.00
// **This function must called after msDlcInit()
extern void msSetDlcStrength(BYTE ucLowStrength, BYTE ucHighStrenght);
extern void LoadDLCTable(BYTE *DLCtable);

#if 1//ENABLE_DCR
extern BYTE xdata u8BacklightNow;
extern BYTE xdata u8BacklightPrev;
//u8BacklightMax=0 && u8BacklightMin==0, these two value init with default
//u8WeightAvgMax=0 && u8WeightAvgMin==0, these two value init with default
extern void msDLC_SetDCR_DD_Criteria(BYTE u8BacklightMax,BYTE u8BacklightMin,BYTE u8WeightAvgMax,BYTE u8WeightAvgMin);
extern void msDLC_SetDCR_LowGrayLevel(BYTE u8LowGrayLvlMax);
#endif
extern BYTE msDLC_LibHandler(BYTE u8Flags);
extern WORD msDLC_GetVersion(void); //hibyte=major, lobyte=minor
extern void msDLC_ClearFlags(BYTE u8Flags);
extern void msDLC_SetLvlMode(Bool u8IsLimited);
extern void msDLC_DD_OnOff(BYTE u8WinIdx, BYTE u8Switch);
extern void msDLCAlgorithm(BYTE ucWinIndex, BYTE u8DMode);
extern void msDLC_SetCustomDLC(BYTE ucWinIndex, BYTE *u8DlcTbl);
//DLC Lib Flag
#define DLF_DCR_ACTIVE              BIT0    //for DCR enabled, this function will be disabled during DD function enabled
#define DLF_LOW_GRAY_REPORT         BIT1    //for DCR, enable low gray backlight function
//DLC Lib Return FLAG
#define DLRF_LOW_GRAY_DETECTED      BIT0    //for DCR low gray level detected
#define DLRF_DCR_BACKLIGHT_UPDATE   BIT1
#define DLRF_DD_BACKLIGHT_UPDATE    BIT2

#endif

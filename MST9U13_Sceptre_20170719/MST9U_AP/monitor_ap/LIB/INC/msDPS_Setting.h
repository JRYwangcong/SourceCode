
//===================================================================================================================================================================:
#ifndef _MSDPS_H_
#define _MSDPS_H_
//===================================================================================================================================================================:
#define DPS_UserBrightControl_EN 0  //Accuray Mode as Adjust PWM, and disable (UserPrefBrightness*PWM)/255)
#define DPS_AggrPowerSaveMode_EN 1
//#define DPS_YminForWindowSave_EN 0
//#define DPS_HistPseuMax_MOD1_EN  0 // Mouse Detection
//#define DPS_HistPseuMax_MOD2_EN  0
#define DPS_InputProcess_Mode 0 //for VGA un-stable input data
#define DPS_FAD_OnOff_Mode 0
//===================================================================================================================================================================:
// Extern XDATA variable from msDLC to share the parameter
//===================================================================================================================================================================:
extern xdata DWORD ISR_LumaHistogram32H[32];
extern xdata DWORD  ISR_HistogramTotalPix;
extern xdata DWORD  ISR_HistogramTotalLuma;
extern xdata BYTE ISR_HistogramMax;
extern xdata BYTE ISR_HistogramMin;

extern xdata WORD  REG_DPS_UserBrightContlGain;// 0~256: 0~100%
//===============DPS_AggrPowerSaveMode===============================================================================================================================:
extern xdata WORD  REG_DPS_UserBrightLimitGain;
extern xdata WORD  REG_DPS_AggressiveYgainGain;//  256   // 0~256: 0~100%
extern xdata WORD  REG_DPS_AggressivePdutyGain;//  256   // 0~256: 0~100%
extern xdata WORD  REG_DPS_AggressiveGainRange;//  240   // apply AggressiveGain when ImgYmax < Range
//===================================================================================================================================================================:
extern xdata BYTE  ISR_preLoopCount;
extern xdata BYTE  ISR_curLoopCount;
extern xdata BYTE  Flag_DPS_Latch_Busy;

extern xdata BYTE  REG_DPS_YOUT_ENABLE;
extern xdata BYTE  REG_DPS_BW_MODE_EN;
extern xdata BYTE  REG_DPS_BW_MODE_Gain;
extern xdata BYTE  REG_DPS_RUN_Flag;
extern xdata BYTE  DPS_YminForWindowSave_EN;
extern xdata BYTE  DPS_HistPseuMax_MOD1_EN;
extern xdata BYTE  DPS_HistPseuMax_MOD2_EN;
extern BYTE code DPS_Lumi_Table[17][11];

//#if DPS_YminForWindowSave_EN
extern xdata BYTE  REG_DPS_PowerLevel;
extern xdata WORD  REG_DPS_WindowSaveDensityTH;
extern xdata BYTE  REG_DPS_WindowSaveYavgLimit;
extern xdata WORD  REG_DPS_WindowSaveStaticTH;
extern xdata WORD  REG_DPS_WindowSavePatTH;
extern xdata BYTE  REG_DPS_WindowSaveMotionDiff;
extern xdata BYTE  REG_DPS_WindowSaveHistDiffCouThd;
extern xdata WORD  REG_DPS_WindowSaveFrameNum;
//#endif 
//#if DPS_HistPseuMax_MOD1_EN
extern xdata BYTE  REG_MouseCount;
//#endif
extern void msDPS_Interrupt ( void );
extern void msDPS_On_Setting ( void );
extern void msDPS_Off_Setting ( void );
extern void msDPS_AdjustYgain ( WORD YgainValue ) ;
extern void msDPS_AdjustBacklight ( BYTE BLValue );
extern BYTE msRGBIsYUV235Domain(void);
extern void msYuvInputProcess(BYTE En);
extern void msDPSHandler(void);
extern xdata BYTE  REG_DPS_Off_FadFlag;
//===================================================================================================================================================================:
// DPS Debug Mode
//===================================================================================================================================================================:
#define DEBUG_Ygain 0
#define DEBUG_PWM 0
#define DEBUG_HISTOGRAM 0
#define DEBUG_HISTOGRAM_PAM 0

#endif
//===================================================================================================================================================================:


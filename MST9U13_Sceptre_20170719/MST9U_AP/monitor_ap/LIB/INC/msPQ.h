#ifndef _msPQ_H_
#define _msPQ_H_

typedef enum
{
    ePQ_SR_OFF,
    ePQ_SR_Level_UHD,
    ePQ_SR_Level_HighPlus,
    ePQ_SR_Level_High,
    ePQ_SR_Level_MiddlePlus,
    ePQ_SR_Level_Middle,
    ePQ_SR_Level_Low,
    ePQ_SR_Level_ScalingDown,
    ePQ_SR_Level_Normal_High,
    ePQ_SR_Level_Normal_Middle,
    ePQ_SR_Level_Normal_Low,
    ePQ_SR_Numbers,

} EN_PQ_SUPER_RESOLUTION_LEVEL;

typedef enum
{
    ePQ_DISPLAY_ONE,
    ePQ_DISPLAY_PIP,
    ePQ_DISPLAY_POP,
    ePQ_DISPLAY_NONE,
}EN_PQ_DISPLAY_TYPE;



void msPQ_Init(EN_PQ_DISPLAY_TYPE enMultiWindowMode);
void msPQ_LoadTimingSetting(BYTE ucWinIndex, ST_WINDOW_INFO sAutoWindow, BOOL bDumpPQ);
void msPQ_LoadXRuleTable(BYTE ucWinIndex);

void msPQ_ForceYUV422(BYTE ucWinIdx, BOOL bForceYUV422);

//********* Bypass feature***************//
void msPQ_SetGRule_Bypass_NR(BYTE ucWinIndex, BYTE level);
void msPQ_SetGRule_Bypass_DI(BYTE ucWinIndex, BYTE level);
void msPQ_SetGRule_Bypass_Peaking(BYTE ucWinIndex, BYTE level);
void msPQ_SetGRule_Bypass_Others(BYTE ucWinIndex, BYTE level);
//***************************************

void msPQ_SetGRule_SR(BYTE ucWinIndex, BYTE level);
void msPQ_SetGRule_NR(BYTE ucWinIndex, BYTE level);
void msPQ_SetGRule_OSDBW(BYTE ucWinIndex, BYTE bOn);
void msPQ_SetGRule_MpegNR(BYTE ucWinIndex, BYTE level);
void msPQ_SetGRule_FilmMode(BYTE ucWinIndex, BYTE bOn);
void msPQ_SetGRule_DynmCont(BYTE ucWinIndex, BYTE bOn);
void msPQ_SetGRule_UltraClear(BYTE ucWinIndex, BYTE bOn);

void msPQ_SetMPQ(BYTE ucWinIndex);

void msPQ_UserDumpTable(BYTE ucWinIndex, BYTE ucIPIndex, BYTE ucTableIndex);

BYTE msPQ_GetTableIndex(BYTE ucWinIndex, BYTE ucIPIndex);
char*   msPQ_GetIPName(BYTE ucIPIndex);

/*
Return value : 0 = 422MF; 1 = 444_10BIT; 2 = 444_8BIT
*/
void msPQ_UpdatePQTable(BYTE ucWinIdx);

void msPQ_LoadUserTiming(BYTE ucWinIdx, BYTE ucTimingIdx);

extern BYTE g_u8PQEnable;

extern XDATA BYTE g_bPQ_Adaptive_On[DISPLAY_MAX_NUMS];
void msPQ_AdaptiveTuning(BYTE ucWin);

void msPQ_GetMemInfo(BYTE ucWinIdx, BYTE *u8MemFmt, BYTE *u8FieldNum, WORD *u8FiledMode);

void msPQ_BypassPQ(BYTE ucWinIdx, BYTE bOn);

void msPQ_IsVideoMode(BYTE ucWinIdx, BOOL bVideo);

void msPQ_InitSuperResolution(void);

void msPQ_CTI_Enable(BYTE ucWinIdx, BOOL bEn);

void msPQ_LTI_Enable(BYTE ucWinIdx, BOOL bEn);

#endif

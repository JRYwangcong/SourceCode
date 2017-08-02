
#ifndef _DRVMSTAR_H_
#define _DRVMSTAR_H_

#include "Global.h"
///////////////////////////////////////
// DRVMSTAR_V VERSION: V01
////////////////////////////////////////
#ifndef _DRVMSTAR_D_H_
#define _DRVMSTAR_D_H_

#ifdef _DRVMSTAR_D_C_
#define _DRVMSTARDEC_
#else
#define _DRVMSTARDEC_   extern
#endif


#ifndef USE_MOD_HW_CAL
#define USE_MOD_HW_CAL        (1)   // coding test// choose hardware(1) or software(0) MOD calibration
#endif


#ifndef I_GEN_CH //use a defined port
  #if PanelDualPort!=0
    #define I_GEN_CH  CH2    //should defined in BD_board.h or panel.h
  #else
    #if PanelSwapPort!=0
      #define I_GEN_CH  CH6 // need check
    #else
      #define I_GEN_CH  CH2 // need check
    #endif
  #endif
#endif

#ifndef LVDS_XSWING_CH //LVDS double swing ch
#define LVDS_XSWING_CH  0 // Bit0~Bit9 maps to ch0~ch9
#endif

_DRVMSTARDEC_ void msDrvEnableClockGating(BOOL bEnableGating);
_DRVMSTARDEC_ void msDrvCheckVBY1LockN(BYTE LockTemp);
_DRVMSTARDEC_ void msDrvmStarInit( void );
//_DRVMSTARDEC_ void mStar_InterlaceModeSetting( BYTE InputPort );
_DRVMSTARDEC_ void mStar_SetDDRSSCEnable( Bool bEnable );
_DRVMSTARDEC_ void mStar_SetDDRSSCFactor( BYTE freqMod, BYTE range );
_DRVMSTARDEC_ void mStar_SetPanelSSCEnable( Bool bEnable );
_DRVMSTARDEC_ void mStar_SetPanelSSCFactor( BYTE freqMod, BYTE range ); //2011.9.13 18:33 CC
_DRVMSTARDEC_ void msDrvIGenTuning( void );
_DRVMSTARDEC_ void msDrvPowerModCtrl(BYTE ucSwitch);
_DRVMSTARDEC_ BYTE mStar_ScalerDoubleBuffer(Bool u8Enable);
_DRVMSTARDEC_ void msDrvScSetScalingFactor(BYTE SCDetWin);
_DRVMSTARDEC_ void msDrvSetDClkPLL(DWORD u32ODCLK_KHZ);
_DRVMSTARDEC_ void msDrvScSetScalingFilter(BYTE SCDetWin);
_DRVMSTARDEC_ void msDrvSetTimingGen(BYTE SCDetWin);
_DRVMSTARDEC_ void drvmStar_SetupFreeRunMode( BYTE SCDetWin);
_DRVMSTARDEC_ void msDrvResetDClkPLL(void);
_DRVMSTARDEC_ void msDrvResetOutputHtt(void);

typedef enum
{
    DVI_POWER_ON,
    DVI_POWER_STANDBY,
    DVI_POWER_DOWN

} DVI_PowerCtrlType;


#define GetImageOutH()  PANEL_WIDTH
#define GetImageOutV()  PANEL_HEIGHT
#define V_FREQ_IN_10X(InputPort)        ((VFREQ( HFREQ( SrcHPeriod(InputPort)), SrcVTotal(InputPort))) * ((SrcFlags(InputPort)&bInterlaceMode)?2:1))
#define V_FREQ_IN(InputPort)            (((VFREQ( HFREQ( SrcHPeriod(InputPort)), SrcVTotal(InputPort))+5)/10) * ((SrcFlags(InputPort)&bInterlaceMode)?2:1))
#define IVS_DEFAULT     22//19 //12
#define VFREQ_4MFRC     30 // If VFreqIn <= VFREQ_4MFRC, do FRC
#define WaitOutputFpllStable()  {TimeOutCounter=255;while(!(msRead2Byte(REG_LPLL_54)==0x1F4) && (TimeOutCounter));}//if(!DDCCI_MCCS_CMD_FLAG) {g_u16DelayCounter=5000;while(!(_msRead2Byte(REG_3854)==0x1F4) && (g_u16DelayCounter));}

typedef struct _RegTblType
{
    DWORD dwReg;
    BYTE ucValue;
    BYTE ucMask;
}RegTblType;
typedef struct _RegTbl2Type
{
    DWORD dwReg;
    WORD wValue;
    WORD wMask;
}RegTbl2Type;
typedef enum _FSyncMode
{
    FLM_NO_LOCK,                // used in MST9Ux
    FLM_FBL,
    FLM_FB_TUNE_VT,
    FLM_FBL_FRAMEPLL,
    FLM_FB_TUNE_CLK,            // used in MST9Ux
    FLM_FB_NEW_LOCK,            // used in MST9Ux
    FLM_FB_FAST_LOCK,           // used in MST9Ux
    FLM_FB_IVS_SPREADER,
    FLM_FB_FLOCK,               // used in MST9Ux
    FLM_FBL_FLOCK,              // used in MST9Ux
    FLM_INIT
}FSyncMode;

typedef enum
{
    ADL_GAMMA_TBL,//for gamma table
    ADL_OD_TBL,//for OD table1~3
    ADL_LOCAL_DIM_TBL,//for local dimming table
    ADL_OD_GAMMA,//for overdrive gamma
    ADL_OD_DEGAMMA,//for overdrive de-gamma
    ADL_XVYCC,//for xvycc
    ADL_GAMMA_1024,//for gamma_1024
    ADL_PIU_IRE,//for PIU IRE
    ADL_T3D//for T3D
}ADL_CLIENT_ID;

typedef enum
{
    ADL_MODE_TRIG,//download table one time unless enable bit is set.
    ADL_MODE_ENABLE//download table continuously.
}ADL_MODE;

typedef enum
{
    MFT_NoChg,
    MFT_LR2OE,
    MFT_OE2LR,
    MFT_LR24B,
}MFT_MODE;

enum DVI_INPUT_PORT_TYPE
{
    DVI_INPUT_NONE = 0,
    DVI_INPUT_PORT0,
    DVI_INPUT_PORT1,
    DVI_INPUT_PORT2,
    DVI_INPUT_PORT_MASK,
};

typedef enum
{
    IP4P_BYPASS,
    IP4P_1TO4,
    IP4P_2TO4,
}IP4P_MODE;

typedef enum
{
    Pure_Black_TesetPattern=0,
    Pure_White_TesetPattern,
    Pure_Red_TesetPattern,
    Pure_Green_TesetPattern,
    Pure_Blue_TesetPattern,
    Vertical_GrayScale_TesetPattern,
    Horizontal_GrayScale_TesetPattern,
    Disable_MODTesetPattern,
}MODTestPattern;

typedef enum
{
    OriHDE_OriHsync_OriVDE_OriVsync=0,
    OriHDE_OriHsync_OriVDE_TgenVsync,
    OriHDE_OriHsync_TgenVDE_OriVsync,
    OriHDE_OriHsync_TgenVDE_TgenVsync,
    OriHDE_TgenHsync_OriVDE_OriVsync,
    OriHDE_TgenHsync_OriVDE_TgenVsync,
    OriHDE_TgenHsync_TgenVDE_OriVsync,
    OriHDE_TgenHsync_TgenVDE_TgenVsync,
    TgenHDE_OriHsync_OriVDE_OriVsync,
    TgenHDE_OriHsync_OriVDE_TgenVsync,
    TgenHDE_OriHsync_TgenVDE_OriVsync,
    TgenHDE_OriHsync_TgenVDE_TgenVsync,
    TgenHDE_TgenHsync_OriVDE_OriVsync,
    TgenHDE_TgenHsync_OriVDE_TgenVsync,
    TgenHDE_TgenHsync_TgenVDE_OriVsync,
    TgenHDE_TgenHsync_TgenVDE_TgenVsync
}ReplaceSignalByMODtgen;

#define VBY1_SWING_950_DIV2 0
#define VBY1_SWING_925_DIV2 1
#define VBY1_SWING_900_DIV2 2
#define VBY1_SWING_875_DIV2 3
#define VBY1_SWING_850_DIV2 4
#define VBY1_SWING_825_DIV2 5
#define VBY1_SWING_800_DIV2 6
#define VBY1_SWING_775_DIV2 7
#define VBY1_SWING_750_DIV2 8
#define VBY1_SWING_725_DIV2 9
#define VBY1_SWING_700_DIV2 10
#define VBY1_SWING_675_DIV2 11
#define VBY1_SWING_650_DIV2 12

_DRVMSTARDEC_ void msWritePowerOnTble(void);
_DRVMSTARDEC_ void msDrvSetFrameSyncMode( FSyncMode ucFrameLockMode, BYTE SCDetWin );
_DRVMSTARDEC_ BOOL msDrvGetFrameSyncBankDiffZero(void);
_DRVMSTARDEC_ void msDrvFrameSyncSrcSel(BYTE u8SCSel);
_DRVMSTARDEC_ BOOL msDrvSetFrameDivider( BYTE vfreq_N );
_DRVMSTARDEC_ BYTE GetVfreq( void );
_DRVMSTARDEC_ void msDrvSetAutoDownload(ADL_CLIENT_ID client_id, BOOL enable, ADL_MODE mode, DWORD addr, WORD length);

//_DRVMSTARDEC_ void mStar_SetIVS( void );
_DRVMSTARDEC_ BYTE GetVSyncTime( BYTE SCDetWin );
_DRVMSTARDEC_ BOOL msDrvISSCDualPort(void);
#if CHIP_ID==MST9U||CHIP_ID==MST9U2||CHIP_ID==MST9U3
_DRVMSTARDEC_ BYTE msDrvMFTCheckMode(void);
#endif
_DRVMSTARDEC_ void msDrvMFTSetup(void);
_DRVMSTARDEC_ void msDrvSetIPMux(BYTE InputPort, BYTE SCDetWin);
_DRVMSTARDEC_ void msDrvSetIPCLKMux(BYTE u8IDCLK, BYTE SCDetWin);
_DRVMSTARDEC_ BYTE msDrvGetIPCLKMux(BYTE SCDetWin);
#if (CHIP_ID == MST9U) || (CHIP_ID == MST9U2)
_DRVMSTARDEC_ void msDrvGOPSetTopSC(WORD u16SCMask);
#endif
_DRVMSTARDEC_ BYTE msDrvIP1GetInputSyncType(BYTE SCDetWin);
_DRVMSTARDEC_ void msDrvIP1SetInputSyncType(WORD u16SCMask, BYTE u8Value);
_DRVMSTARDEC_ void msDrvIP1SetInputVSyncDelay(WORD u16SCMask, BOOL bEnable);
_DRVMSTARDEC_ void msDrvIP1SetInputYpbprFormat(WORD u16SCMask, BYTE u8Value);
_DRVMSTARDEC_ void msDrvIP1AdcSetCoast(WORD u16SCMask, BYTE u8Enable, BYTE u8Start, BYTE u8End);
_DRVMSTARDEC_ void msDrvIP1AdcGlitchRemoval(WORD u16SCMask, BYTE u8Value);
_DRVMSTARDEC_ void msDrvIP1AutoPositionResultReady(BYTE u8SCDetWin, BYTE u8InputPort);
_DRVMSTARDEC_ void msDrvIP1AutoPositionSetValidValue(BYTE u8SCDetWin, BYTE u8Value);
_DRVMSTARDEC_ BYTE msDrvIP1AutoPositionReadValidValue(BYTE u8SCDetWin);
_DRVMSTARDEC_ WORD msDrvIP1AutoPositionReadHstart(BYTE u8SCDetWin, BYTE u8InputPort);
_DRVMSTARDEC_ WORD msDrvIP1AutoPositionReadHend(BYTE u8SCDetWin, BYTE u8InputPort);
_DRVMSTARDEC_ WORD msDrvIP1AutoPositionReadVstart(BYTE u8SCDetWin, BYTE u8InputPort);
_DRVMSTARDEC_ WORD msDrvIP1AutoPositionReadVend(BYTE u8SCDetWin, BYTE u8InputPort);
_DRVMSTARDEC_ BYTE msDrvIP1AutoPositionReadTranstionPosition(WORD u16DelayTime, BYTE u8SCDetWin, BYTE u8InputPort);
_DRVMSTARDEC_ void msDrvIP1AutoPhaseResultReady(BYTE u8SCDetWin);
_DRVMSTARDEC_ DWORD msDrvIP1AutoPhaseReadValue(BYTE u8SCDetWin);
_DRVMSTARDEC_ void msDrvIP1AutoPhaseSetNoiseMask(BYTE u8Value, BYTE u8SCDetWin);
//scaler
_DRVMSTARDEC_ void msDrvIP1SetOutputLockMode(BYTE u8Value, BYTE u8SCDetWin);
_DRVMSTARDEC_ void msDrvIP1SetOutputFreerun(BYTE u8Value, BYTE u8SCDetWin);
_DRVMSTARDEC_ void msDrvIP1SetHperiodDetectMode(BOOL bEnable8lines, BYTE u8SCDetWin);
_DRVMSTARDEC_ BOOL msDrvIP1GetHperiodDetectMode(BYTE u8SCDetWin); // TRUE: 8 line mode, FALSE: 1 line mode
_DRVMSTARDEC_ WORD msDrvIP1ReadHperiod(BYTE u8SCDetWin);
_DRVMSTARDEC_ WORD msDrvIP1ReadHtotal(BYTE u8SCDetWin);
_DRVMSTARDEC_ WORD msDrvIP1ReadVtotal(BYTE u8SCDetWin);
_DRVMSTARDEC_ BOOL msDrvIP1CheckComboStable(BYTE u8SCDetWin);
#if ENABLE_FORCE_4K_PMODE
_DRVMSTARDEC_ BYTE msDrvIP1ReadUserForceToProgressOrInterless(BYTE u8SCDetWin);
#endif
_DRVMSTARDEC_ BYTE msDrvIP1ReadSyncStatus(BYTE u8SCDetWin);
_DRVMSTARDEC_ WORD msDrvIP1ReadAutoHStart(BYTE u8SCDetWin);
_DRVMSTARDEC_ WORD msDrvIP1ReadAutoHEnd(BYTE u8SCDetWin);
_DRVMSTARDEC_ WORD msDrvIP1ReadAutoVStart(BYTE u8SCDetWin);
_DRVMSTARDEC_ WORD msDrvIP1ReadAutoVEnd(BYTE u8SCDetWin);
_DRVMSTARDEC_ WORD msDrvIP1ReadAutoWidth(BYTE u8SCDetWin);
_DRVMSTARDEC_ WORD msDrvIP1ReadAutoHeight(BYTE u8SCDetWin);
_DRVMSTARDEC_ WORD msDrvIP1ReadImageVStart(BYTE u8SCDetWin);
_DRVMSTARDEC_ WORD msDrvIP1ReadImageHeight(BYTE u8SCDetWin);
_DRVMSTARDEC_ WORD msDrvIP1ReadImageWidth(BYTE u8SCDetWin);
_DRVMSTARDEC_ void msDrvIP1SetImageHeight(WORD u16Value, BYTE u8SCDetWin);
_DRVMSTARDEC_ void msDrvIP1SetImageWidth(WORD u16Value, BYTE u8SCDetWin);
_DRVMSTARDEC_ void msDrvIP1SetImageHStart(WORD u16Value, BYTE u8SCDetWin);
_DRVMSTARDEC_ void msDrvIP1SetImageVStart(WORD u16Value, BYTE u8SCDetWin);
_DRVMSTARDEC_ BOOL msDrvIP1IsDEOnlyMode(BYTE u8SCDetWin);
_DRVMSTARDEC_ BOOL msDrvIP1IsVideoFieldInvert(BYTE u8SCDetWin);
_DRVMSTARDEC_ BYTE msDrvIP1ReadVSyncWidth(BYTE u8SCDetWin);
_DRVMSTARDEC_ void msDrvIP1AutoRange(BYTE u8SCDetWin, ST_WINDOW_INFO *sDisplayWin, Bool bEnable);
_DRVMSTARDEC_ WORD msDrvIP2ReadHSDCnt(BYTE u8SCDetWin);
_DRVMSTARDEC_ WORD msDrvIP2ReadVSDCnt(BYTE u8SCDetWin);
_DRVMSTARDEC_ void msDrvMIUInit(void);
_DRVMSTARDEC_ void msDrvMODInit(void);
_DRVMSTARDEC_ WORD msDrvOP2GetImageHStart(BYTE u8SCDetWin);
_DRVMSTARDEC_ WORD msDrvOP2GetImageHEnd(BYTE u8SCDetWin);
_DRVMSTARDEC_ WORD msDrvOP2GetImageVStart(BYTE u8SCDetWin);
_DRVMSTARDEC_ WORD msDrvOP2GetImageVEnd(BYTE u8SCDetWin);
_DRVMSTARDEC_ void msDrvOP2SetImageHStart(BYTE u8SCDetWin,WORD u16Value);
_DRVMSTARDEC_ void msDrvOP2SetImageHEnd(BYTE u8SCDetWin,WORD u16Value);
_DRVMSTARDEC_ void msDrvOP2SetImageVStart(BYTE u8SCDetWin,WORD u16Value);
_DRVMSTARDEC_ void msDrvOP2SetImageVEnd(BYTE u8SCDetWin,WORD u16Value);
_DRVMSTARDEC_ BOOL msDrvSrcIsInterlaceMode( BYTE InputPort );
_DRVMSTARDEC_ WORD GetInputVfreq(BYTE u8WinIndex);
_DRVMSTARDEC_ void msDrvIP1SetHVSyncCheck(MS_BOOL bEnable, WORD u16SCMask);
_DRVMSTARDEC_ void msDrvIP1SetNewInterlaceDetect(MS_BOOL bEnable, WORD u16SCMask);
_DRVMSTARDEC_ Bool msDrvIP1ReadNewInterlaceDetect(BYTE u16SCMask);
_DRVMSTARDEC_ void msDrvIP1SyncSourceSelect(BYTE u8Source, WORD u16SCMask);
_DRVMSTARDEC_ void msDrvIP1SetVttCountbyPixel(MS_BOOL bEnable, WORD u16SCMask);
_DRVMSTARDEC_ void msDrvIP1SetVsyncInvert(MS_BOOL bInvert, WORD u16SCMask);
_DRVMSTARDEC_ void msDrvIP1SetHsyncInvert(MS_BOOL bInvert,  WORD u16SCMask);
_DRVMSTARDEC_ void msDrvIP1Init(void);
#if (CHIP_ID == MST9U4)
_DRVMSTARDEC_ DWORD msDrvIP1RegBaseMapping(BYTE u8SCDetWin);
_DRVMSTARDEC_ void msDrvIP2Set4PMode(BYTE u8SCDetWin, IP4P_MODE eIP4PMode);
#endif
_DRVMSTARDEC_ void msDrvIP22ndHCropEn(BYTE u8SCDetWin, BOOL Enable);
_DRVMSTARDEC_ void msDrvIP22ndVCropEn(BYTE u8SCDetWin, BOOL Enable);
_DRVMSTARDEC_ void msDrvIP22ndSetImageHStart(BYTE u8SCDetWin, WORD u16Value);
_DRVMSTARDEC_ void msDrvIP22ndSetImageVStart(BYTE u8SCDetWin, WORD u16Value);
_DRVMSTARDEC_ void msDrvIP22ndSetImageWidth(BYTE u8SCDetWin, WORD u16Value);
_DRVMSTARDEC_ void msDrvIP22ndSetImageHeight(BYTE u8SCDetWin, WORD u16Value);
#undef _DRVMSTARDEC_
#endif


#endif

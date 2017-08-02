
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

#ifdef _WORK_ON_PM_
#ifndef I_GEN_CH //use a defined port
  #if PanelDualPort!=0
    #define I_GEN_CH  CH2
  #else
    #if PanelSwapPort!=0
      #define I_GEN_CH  CH6 // need check
    #else
      #define I_GEN_CH  CH2 // need check
    #endif
  #endif
#endif
#endif
#ifndef LVDS_XSWING_CH //LVDS double swing ch
#define LVDS_XSWING_CH  0 // Bit0~Bit9 maps to ch0~ch9
#endif






_DRVMSTARDEC_ void msDrvEnableClockGating(BOOL bEnableGating);
_DRVMSTARDEC_ void msDrvCheckVBY1LockN(void);
_DRVMSTARDEC_ void msTMDSInit(void);
//_DRVMSTARDEC_ void TMDSISR(void);
_DRVMSTARDEC_ void msDrvmStarInit( void );
_DRVMSTARDEC_ void mStar_InterlaceModeSetting( BYTE InputPort );
_DRVMSTARDEC_ void mStar_SetPanelSSC( BYTE freqMod, BYTE range ); //2011.9.13 18:33 CC
_DRVMSTARDEC_ void mStar_IPPowerControl(void);
_DRVMSTARDEC_ void msTMDSSetMux( BYTE inport );
_DRVMSTARDEC_ void msDrvIGenTuning( void );
_DRVMSTARDEC_ void msDrvPowerModCtrl(BYTE ucSwitch);
_DRVMSTARDEC_ BYTE mStar_ScalerDoubleBuffer(Bool u8Enable);
_DRVMSTARDEC_ void msDrvScSetScalingFactor(BYTE SCDetWin);
_DRVMSTARDEC_ void msDrvSetDClkPLL(DWORD u32ODCLK_KHZ);
_DRVMSTARDEC_ void msDrvScSetScalingFilter(BYTE SCDetWin);
_DRVMSTARDEC_ void msDrvSetTimingGen(BYTE SCDetWin);
_DRVMSTARDEC_ void drvmStar_SetupFreeRunMode( BYTE SCDetWin);

typedef enum
{
    DVI_POWER_ON,
    DVI_POWER_STANDBY,
    DVI_POWER_DOWN

} DVI_PowerCtrlType;

_DRVMSTARDEC_ void drvDVI_PowerCtrl(DVI_PowerCtrlType ctrl);



#define GetImageOutH()  PANEL_WIDTH
#define GetImageOutV()  PANEL_HEIGHT


#define SSC_STEP  0x00B2//0x0124
#define SSC_SPAN  0x013A//0x0050
#define LPLL_SSC  _DISABLE
#define V_FREQ_IN(InputPort)       (((VFREQ( HFREQ( SrcHPeriod(InputPort)), SrcVTotal(InputPort))+5)/10) * ((SrcFlags(InputPort)&bInterlaceMode)?2:1))
#define IVS_DEFAULT     22//19 //12
#define VFREQ_4MFRC     30 // If VFreqIn <= VFREQ_4MFRC, do FRC
#define WaitOutputFpllStable()  {TimeOutCounter=255;while(!(msRead2Byte(REG_140554)==0x1F4) && (TimeOutCounter));}//if(!DDCCI_MCCS_CMD_FLAG) {g_u16DelayCounter=5000;while(!(_msRead2Byte(REG_3854)==0x1F4) && (g_u16DelayCounter));}

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
    FLM_NO_LOCK,
    FLM_FBL,
    FLM_FB_TUNE_VT, // Old Lcok
    //#if (MAIN_FRAME_BFF_SEL == MAIN_FRAME_BFFLESS_FRAMEPLL) //20090303 ThruMode
    FLM_FBL_FRAMEPLL,
    //#endif
    FLM_FB_TUNE_CLK, // New Lock
    FLM_FB_TUNE_CLK_DOUBLE, // New Lock   input:output >> 1:2
    FLM_FB_NEW_LOCK, //New lock
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

_DRVMSTARDEC_ void msWritePowerOnTble(void);
_DRVMSTARDEC_ void msDrvSetFrameSyncMode( FSyncMode ucFrameLockMode, BYTE SCDetWin );
_DRVMSTARDEC_ void msDrvSetFrameDivider( BYTE vfreq_N );
_DRVMSTARDEC_ BYTE GetVfreq( void );
_DRVMSTARDEC_ void msDrvSetAutoDownload(ADL_CLIENT_ID client_id, BOOL enable, ADL_MODE mode, DWORD addr, WORD length);
_DRVMSTARDEC_ void msDrvSoftwareReset(DWORD ucParam, BYTE SCDetWin);

//_DRVMSTARDEC_ void mStar_SetIVS( void );
_DRVMSTARDEC_ BYTE GetVSyncTime( BYTE SCDetWin );
_DRVMSTARDEC_ BOOL msDrvISSCDualPort(void);
_DRVMSTARDEC_ BYTE msDrvMFTCheckMode(void);
_DRVMSTARDEC_ void msDrvMFTSetup(void);
_DRVMSTARDEC_ void msDrvSetOfflineIPMux(BYTE InputPort, BYTE SCDetWin);
_DRVMSTARDEC_ BYTE msDrvIP1OfflineGetInputSyncType(BYTE SCDetWin);
_DRVMSTARDEC_ void msDrvIP1OfflineSetInputSyncType(WORD u16SCMask, BYTE u8Value);
_DRVMSTARDEC_ void msDrvIP1OfflineAdcSetCoast(WORD u16SCMask, BYTE u8Enable, BYTE u8Start, BYTE u8End);
_DRVMSTARDEC_ void msDrvIP1OfflineAdcGlitchRemoval(WORD u16SCMask, BYTE u8Value);
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
_DRVMSTARDEC_ WORD msDrvIP1OfflineReadHperiod(BYTE u8SCDetWin);
_DRVMSTARDEC_ WORD msDrvIP1OfflineReadVtotal(BYTE u8SCDetWin);
_DRVMSTARDEC_ BYTE msDrvIP1OfflineReadSyncStatus(BYTE u8SCDetWin);
#ifdef _WORK_ON_PM_
_DRVMSTARDEC_ WORD msDrvIP1ReadAutoHStart(BYTE u8SCDetWin);
_DRVMSTARDEC_ WORD msDrvIP1ReadAutoHEnd(BYTE u8SCDetWin);
_DRVMSTARDEC_ WORD msDrvIP1ReadAutoVStart(BYTE u8SCDetWin);
_DRVMSTARDEC_ WORD msDrvIP1ReadAutoVEnd(BYTE u8SCDetWin);
_DRVMSTARDEC_ WORD msDrvIP1ReadAutoWidth(BYTE u8SCDetWin);
_DRVMSTARDEC_ WORD msDrvIP1ReadAutoHeight(BYTE u8SCDetWin);
_DRVMSTARDEC_ WORD msDrvIP1ReadImageHeight(BYTE u8SCDetWin);
_DRVMSTARDEC_ WORD msDrvIP1ReadImageWidth(BYTE u8SCDetWin);
_DRVMSTARDEC_ void msDrvIP1SetImageHeight(WORD u16Value, BYTE u8SCDetWin);
_DRVMSTARDEC_ void msDrvIP1SetImageWidth(WORD u16Value, BYTE u8SCDetWin);
_DRVMSTARDEC_ void msDrvIP1SetImageHStart(WORD u16Value, BYTE u8SCDetWin);
_DRVMSTARDEC_ void msDrvIP1SetImageVStart(WORD u16Value, BYTE u8SCDetWin);
_DRVMSTARDEC_ BOOL msDrvIP1IsDEOnlyMode(BYTE u8SCDetWin);
#endif
_DRVMSTARDEC_ BYTE msDrvIP1OfflineReadVSyncWidth(BYTE u8SCDetWin);
#ifdef _WORK_ON_PM_
_DRVMSTARDEC_ void msDrvIP1AutoRange(BYTE u8SCDetWin, ST_WINDOW_INFO *sDisplayWin, Bool bEnable);
#endif

#if ENABLE_CABLE_DET
_DRVMSTARDEC_ Bool msAPISrcIsCableConnected( BYTE InputPort );
#endif
#undef _DRVMSTARDEC_
#endif


#endif

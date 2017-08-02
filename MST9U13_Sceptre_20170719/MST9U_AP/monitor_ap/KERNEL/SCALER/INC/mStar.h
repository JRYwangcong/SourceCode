#ifndef _MSTAR_H_
#define _MSTAR_H_

#ifdef _MSTAR_C_
#define _MSTARDEC_
#else
#define _MSTARDEC_  extern
#endif

#define BW_SCREEN_OFF   0
#define BW_SCREEN_WHITE 1
#define BW_SCREEN_BLACK 2

#if CHIP_ID == MST9U4
#define SC_INT_MUTE_IP      0x0086 //0x00C6 // scaler int for fast mute
#define SC_INT_VSYNC        BIT4
#define SC_INT_IP_VSYNC     BIT4
#define SC_INT_ATP          BIT9
#else
#define SC_INT_MUTE_MAIN    0x00100140//0x00140140 // scaler int for fast mute
#define SC_INT_MUTE_SUB     0x00200280//0x00280280 // scaler int for fast mute
#define SC_INT_MUTE         (SC_INT_MUTE_MAIN|SC_INT_MUTE_SUB)
#define SC_INT_VSYNC        BIT5
#define SC_INT_IP_VSYNC        BIT12
#define SC_INT_ATP          BIT28
#endif

#include "msScaler.h"
#include "msScalerAP.h"

_MSTARDEC_ BYTE xdata iGenTuningFinished;
//_MSTARDEC_ void I_Gen_Tuning( void );

_MSTARDEC_ void mStar_Init( void );
_MSTARDEC_ Bool msDrvSetupMode(BYTE SCDetWin);
_MSTARDEC_ void mStar_SetupFreeRunMode(void);
_MSTARDEC_ void mStar_SetAnalogInputPort( BYTE InputPort, Bool ToSOGPort );
_MSTARDEC_ ST_WINDOW_INFO mStar_ReadAutoWindow( BYTE InputPort );
_MSTARDEC_ void msDrvPWMInit(void);
#if !ENABLE_LED_CONTROLLER
_MSTARDEC_ void SetPWMFreq( WORD freq );
#endif
#if BrightFreqByVfreq
_MSTARDEC_ void SetFourtimesPWMFreq( BYTE SCDetWin );
#endif

_MSTARDEC_ void mStar_SetupADC(BYTE SCDetWin);

_MSTARDEC_ void mStar_MCU_Clock_Select(Bool IsNormal);
_MSTARDEC_ void mStar_EnableModeChangeINT(Bool u8Enable, WORD u16SCMask);

_MSTARDEC_ void mStar_BlackWhiteScreenCtrl(BYTE u8Ctrl);
_MSTARDEC_ void mStar_SetPanelTiming(void);
//#if ENABLE_HDMI_INPUT
_MSTARDEC_  void  SettingInputColorimetry(BYTE ucWinIndex);
//#endif
#endif

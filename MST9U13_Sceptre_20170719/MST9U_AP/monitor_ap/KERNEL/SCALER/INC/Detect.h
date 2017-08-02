#ifndef _DETECT_H_
#define  _DETECT_H_

extern void mStar_MonitorInputTiming( BYTE InputPort );
extern void mStar_ModeHandler( BYTE SCDetWin );
extern Bool mStar_ValidTimingDetect( BYTE InputPort );
//extern Bool mStar_IsStatusChanged( void );
extern BYTE mStar_GetInputStatus( BYTE InputPort );
extern Bool mStar_SyncLossStateDetect( BYTE InputPort );
extern void mStar_PrepareForTimingChange( BYTE InputPort );
extern void mStar_NewModeInterlacedDetect(BYTE InputPort);
#endif


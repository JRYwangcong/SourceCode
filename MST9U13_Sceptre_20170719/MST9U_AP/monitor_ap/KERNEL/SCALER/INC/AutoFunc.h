#ifndef _AUTOFUNC_H_
#define _AUTOFUNC_H_       (1)

#ifdef _AUTOFUNC_C_
#define _AUTOFUNCDEC_
#else
#define _AUTOFUNCDEC_ extern
#endif

_AUTOFUNCDEC_ Bool CheckSyncLoss( BYTE InputPort );
_AUTOFUNCDEC_ void WaitAutoStatusReady( DWORD u16RegAddr, BYTE regFlag, BYTE InputPort );
_AUTOFUNCDEC_ WORD GetAutoValue(DWORD u32RegLoAddr, BYTE InputPort);
_AUTOFUNCDEC_ BYTE GetTranstionPosition( BYTE vsyncTime, DWORD u32RegLoAddr, BYTE InputPort );
_AUTOFUNCDEC_ BYTE AutoSetDataThreshold( BYTE vsyncTime, BYTE InputPort );
_AUTOFUNCDEC_ WORD SearchMaxWidth( BYTE vsyncTime, BYTE InputPort );
_AUTOFUNCDEC_ Bool mStar_AutoHTotal( BYTE vsyncTime, BYTE SCDetWin );
_AUTOFUNCDEC_ Bool mStar_AutoPhase( BYTE vsyncTime, BYTE SCDetWin );
_AUTOFUNCDEC_ Bool mStar_AutoPosition( BYTE u8DispWin);
_AUTOFUNCDEC_ Bool mStar_AutoGeomtry(BYTE u8DispWin);
_AUTOFUNCDEC_ Bool mStar_AutoColor( BYTE InputPort );
_AUTOFUNCDEC_ void mStar_YUVAutoOffset(BYTE u8Value);
#endif

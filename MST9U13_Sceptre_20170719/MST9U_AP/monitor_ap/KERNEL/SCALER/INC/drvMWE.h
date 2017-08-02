#ifndef _DRV_MWE_H_
#define _DRV_MWE_H_

#ifdef _DRV_MWE_C_
  #define _DRVMWEC_
#else
  #define _DRVMWEC_ extern
#endif

typedef enum{
    eMWE_WIN1,
    eMWE_WIN2,
    eMWE_WIN3,
    eMWE_WIN4,
}eMWEWIN_SEL;


_DRVMWEC_ void msDrvMWEInit(void);
_DRVMWEC_ void msDrvMWEWindowConfig(BYTE u8ScalerIdx, BYTE u8MEWSel, ST_WINDOW_INFO sWinInfo);
_DRVMWEC_ void msDrvMWEWindowEnable(BYTE u8ScalerIdx, BYTE u8MEWSel, Bool bMWEEnable, Bool bBorderEnable);

_DRVMWEC_ void msDrvOp2ReportWindowSetRange(BYTE u8WinIdx, WORD u16HStart, WORD u16HEnd, WORD u16VStart, WORD u16VEnd);
_DRVMWEC_ void msDrvOp2ReportWindowEnable(BYTE u8WinIdx ,BYTE bEnable );
_DRVMWEC_ void msDrvOp2ReportWindowGetData(BYTE u8WinIdx );
_DRVMWEC_ void msDrvOp2CursorEnable(BYTE u8WinIdx ,BYTE bEnable );
_DRVMWEC_ void msDrvOp2CursorSetCoordination(BYTE u8WinIdx, WORD u16x, WORD u16y);
_DRVMWEC_ void msDrvOp2CursorGetRGBValue(BYTE u8WinIdx );

#undef _DRVMWEC_
#endif

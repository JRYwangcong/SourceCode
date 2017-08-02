#include "Board.h"
#ifndef _DEBUG_H_
#define  _DEBUG_H_

#ifdef _DEBUG_C_
  #define _DEBUGDEC_
#else
  #define _DEBUGDEC_ extern
#endif

extern BYTE xdata VKeyPad;
_DEBUGDEC_ void putSIOChar( unsigned char sendData );
_DEBUGDEC_ BOOL ProcessCommand( void );
_DEBUGDEC_ BOOL ProcessCommandExt( void );
_DEBUGDEC_ Bool GetCommand( void );

#endif

#ifndef _COMMON_H_
#define _COMMON_H_

#ifdef _COMMON_C_
  #define _COMMONDEC_
#else
  #define _COMMONDEC_ extern
#endif


_COMMONDEC_ void printData( char *str, WORD value );
_COMMONDEC_ void printMsg( char * str );
_COMMONDEC_ void putSIOChar( unsigned char sendData );

_COMMONDEC_ void printData_UART1(char * str, WORD value); // Uart 1
_COMMONDEC_ void printMsg_UART1(char * str); // Uart 1
_COMMONDEC_ void putSIOChar_UART1(unsigned char sendData); // Uart 1
#endif
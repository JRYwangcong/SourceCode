#define _COMMON_C_
#include "Board.h"
#include "datatype.h"
//#include "debugDef.h"
#include "Global.h"
#include "ms_reg.h"
#include "Ms_rwreg.h"
#include "misc.h"
#include "MDebug.h"
#include "Common.h"


extern void putcharb(MS_U8 byte);
void printMsg( char *str )
{
#if ENABLE_MSTV_UART_DEBUG
    printf(str);
    putcharb( 0xd );
    putcharb(( char )'\n' );
#else
    str=str;
#endif
}

void printData( char *str, WORD value )
{
#if ENABLE_MSTV_UART_DEBUG
    printf(str, value);
    putcharb( 0xd );
    putcharb(( char )'\n' );
#else
    str=str;
    value=value;
#endif
}



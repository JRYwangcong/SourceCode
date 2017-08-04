typedef unsigned char BYTE;
typedef unsigned int WORD;
typedef unsigned long DWORD;
typedef signed char Schar;
typedef signed int Sint;
typedef signed long Slong;
//#if IT6251
typedef unsigned short USHORT, *PUSHORT ;
typedef unsigned long ULONG, *PULONG ;
//#endif
typedef enum bool_enum
{
    FALSE, TRUE
} Bool;
#define XDATA xdata
#define BIT0	0x0001
#define BIT1	0x0002
#define BIT2	0x0004
#define BIT3	0x0008
#define BIT4	0x0010
#define BIT5	0x0020
#define BIT6	0x0040
#define BIT7	0x0080
#define BIT8	0x0100
#define BIT9	0x0200
#define BIT10	0x0400
#define BIT11	0x0800
#define BIT12	0x1000
#define BIT13	0x2000
#define BIT14	0x4000
#define BIT15	0x8000
#define _ENABLE      1
#define _DISABLE     0

#define	LOCAL_TYPE		xdata

#define MAIN_WINDOW 0
#define SUB_WINDOW  1


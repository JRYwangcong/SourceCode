#ifndef __TYPES__
#define __TYPES__
typedef unsigned char BYTE;
typedef unsigned int WORD;
typedef unsigned long DWORD;
typedef signed char Schar;
typedef signed int Sint;
typedef signed long Slong;

typedef enum bool_enum
{
    FALSE, TRUE
} Bool;

typedef enum reset_enum
{ RETAIN,
   RESET
} BReset;

typedef struct
{
    WORD u16Reg;
    BYTE u8Value;
} RegUnitType;

#define BOOL    Bool
#define XDATA xdata
#define BIT0    0x0001
#define BIT1    0x0002
#define BIT2    0x0004
#define BIT3    0x0008
#define BIT4    0x0010
#define BIT5    0x0020
#define BIT6    0x0040
#define BIT7    0x0080
#define BIT8    0x0100
#define BIT9    0x0200
#define BIT10   0x0400
#define BIT11   0x0800
#define BIT12   0x1000
#define BIT13   0x2000
#define BIT14   0x4000
#define BIT15   0x8000
#define BIT16                       0x00010000
#define BIT17                       0x00020000
#define BIT18                       0x00040000
#define BIT19                       0x00080000
#define BIT20                       0x00100000
#define BIT21                       0x00200000
#define BIT22                       0x00400000
#define BIT23                       0x00800000
#define BIT24                       0x01000000
#define BIT25                       0x02000000
#define BIT26                       0x04000000
#define BIT27                       0x08000000
#define BIT28                       0x10000000
#define BIT29                       0x20000000
#define BIT30                       0x40000000
#define BIT31                       0x80000000


#define BIT(_bit_)                  (1 << (_bit_))
#define BMASK(_bits_)               (BIT(((1)?_bits_)+1)-BIT(((0)?_bits_)))


// bit
#define _BIT0	0x0001
#define _BIT1	0x0002
#define _BIT2	0x0004
#define _BIT3	0x0008
#define _BIT4	0x0010
#define _BIT5	0x0020
#define _BIT6	0x0040
#define _BIT7	0x0080
#define _BIT8	0x0100
#define _BIT9	0x0200
#define _BIT10	0x0400
#define _BIT11	0x0800
#define _BIT12	0x1000
#define _BIT13	0x2000
#define _BIT14	0x4000
#define _BIT15	0x8000

#define _ENABLE      1
#define _DISABLE     0

#define LOCAL_TYPE      xdata

#define _bit0_(val)  ((bit)(val & BIT0))
#define _bit1_(val)  ((bit)(val & BIT1))
#define _bit2_(val)  ((bit)(val & BIT2))
#define _bit3_(val)  ((bit)(val & BIT3))
#define _bit4_(val)  ((bit)(val & BIT4))
#define _bit5_(val)  ((bit)(val & BIT5))
#define _bit6_(val)  ((bit)(val & BIT6))
#define _bit7_(val)  ((bit)(val & BIT7))
#define _bit8_(val)  ((bit)(val & BIT8))
#define _bit9_(val)  ((bit)(val & BIT9))
#define _bit10_(val) ((bit)(val & BIT10))
#define _bit11_(val) ((bit)(val & BIT11))
#define _bit12_(val) ((bit)(val & BIT12))
#define _bit13_(val) ((bit)(val & BIT13))
#define _bit14_(val) ((bit)(val & BIT14))
#define _bit15_(val) ((bit)(val & BIT15))

#ifdef NULL
#undef NULL
#endif
#define NULL                        0

#endif

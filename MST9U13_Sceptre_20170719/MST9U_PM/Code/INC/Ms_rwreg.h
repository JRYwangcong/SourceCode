///////////////////////////////////////////////////////////////////////////////
/// @file ms_rwreg.h
/// @brief Head file of ms_rwreg.c
/// @author MStarSemi Inc.
///
/// Functions for MStar scaler register access.
///
/// Features
///  -Write byte/word/bulk data
///  -Read byte/word data.
///  -
///  -
///////////////////////////////////////////////////////////////////////////////
#include "global.h"
#ifndef _MS_RWREG_H
#define _MS_RWREG_H

#ifdef _MS_RWREG_C
#define INTERFACE
#else
#define INTERFACE   extern
#endif

/////////////////////////////////////////////////////////////////////////////
//Must use sc series R/W function if input scaler bank address is BYTE data.
//Must use bank save/restore future while use sc series function.
/////////////////////////////////////////////////////////////////////////////
#define MAIN_WINDOW 0
#define SUB_WINDOW  1
#define msRegs        ((unsigned char volatile xdata *) 0x000000)
#define msCodeArea        ((unsigned char volatile code *) 0x000000)
//#define scRegs        ((unsigned char volatile xdata *) 0x2F00)
#define MAKEWORD(value1, value2)    (((WORD)(value1))<<8) + (value2)

////////////////////////
// RIU Access Semaphore//
extern WORD idata g_u16RIUReqCnt;
#define ReqstRIU(ISR_SRC)   do {\
                                if(g_bR2Status)\
                                {\
                                    WORD idata u16Timeout = 0xFFFF;\
                                    g_u16RIUReqCnt++;\
                                    do {\
                                         u16Timeout--;\
                                         GetRIU();\
                                    }while(u16Timeout && !IsRIUGet(ISR_SRC));\
                                }\
                            }while(0)
#define RelesRIU(ISR_SRC)   do {\
                                if(g_bR2Status)\
                                {\
                                    g_u16RIUReqCnt--;\
                                    if(IsRIUGet(ISR_SRC) && (g_u16RIUReqCnt==0))\
                                        FreeRIU();\
                                }\
                            }while(0)
// RIU Access Semaphore//

//Jison 110308 for GPIO define and can be used in interrupt
#if 0
#define MEM_MSREAD_BYTE(u16Addr)                        (msRegs[u16Addr])
#else // Need Change Sub Bank Manually if necessary, for performanace consideration in ISR !!!!
INTERFACE BYTE MEM_MSREAD_BYTE( DWORD u32Reg );
INTERFACE BYTE MEM_MSREAD_BYTE_EXT0(DWORD u32Reg);
INTERFACE BYTE MEM_MSREAD_BYTE_EXT1(DWORD u32Reg);
INTERFACE BYTE MEM_MSREAD_BYTE_TIME0(DWORD u32Reg);
#endif
#define MEM_MSWRITE_BYTE(u32Reg,u8Value)               (P2 = ((u32Reg & 0x00FF0000)>>16), msRegs[u32Reg & 0x0000FFFF]=(u8Value), P2 = 0)
#define MEM_MSWRITE_BYTE_MASK(u32Reg,u8Value,u8Mask)   (msWriteByteMask(u32Reg,u8Value,u8Mask))//(P2 = ((u32Reg & 0x00FF0000)>>16), msRegs[u32Reg & 0x0000FFFF] = ( (msRegs[u32Reg & 0x0000FFFF] & (~(u8Mask))) | ((u8Value) & (u8Mask))), P2 = 0)
#define MEM_MSWRITE_BIT(u32Reg,u8True,u8Bit)           (msWriteBit(u32Reg,u8True,u8Bit))//(P2 = ((u32Reg & 0x00FF0000)>>16), msRegs[u32Reg & 0x0000FFFF] = ( (msRegs[u32Reg & 0x0000FFFF] & (~(u8Bit))) | ((u8True)?(u8Bit):0)), P2 = 0)
//#define MEM_SCREAD_BYTE(u16Addr)                        (scRegs[(u16Addr)&0xFF])
//#define MEM_SCWRITE_BYTE(u16Addr,u8Value)               (scRegs[(u16Addr)&0xFF]=(u8Value))
#define MEM_MSWRITE_BYTE_MASK_ISR( ISR_SRC, u32Reg, u8Value, u8Mask )\
        do {\
            ReqstRIU(ISR_SRC);\
            P2 = (u32Reg & 0x00FF0000)>>16;\
            msRegs[u32Reg & 0x0000FFFF] = (msRegs[u32Reg&0x0000FFFF]&(~(u8Mask)))|((u8Value)&(u8Mask));\
            P2 = 0;\
            RelesRIU(ISR_SRC);\
        }while(0)

#define MEM_MSWRITE_BIT_ISR( ISR_SRC, u32Reg, bBit, u8BitPos )\
        do {\
            ReqstRIU(ISR_SRC);\
            P2 = (u32Reg & 0x00FF0000)>>16;\
            msRegs[u32Reg & 0x0000FFFF] = (msRegs[u32Reg&0x0000FFFF]&(~(u8BitPos)))|((bBit)?(u8BitPos):0);\
            P2 = 0;\
            RelesRIU(ISR_SRC);\
        }while(0)


INTERFACE BYTE msReadByte( DWORD u32Reg );
INTERFACE void msWriteByte( DWORD u32Reg, BYTE u8Val );
INTERFACE WORD msRead2Byte( DWORD u32RegLoAddr );
INTERFACE void msWriteBit( DWORD u32Reg, Bool bBit, BYTE u8BitPos );
#if 0
INTERFACE BYTE scReadByte( BYTE u8Reg );
INTERFACE void scWriteByte( BYTE u8Reg, BYTE u8Value );
#endif
INTERFACE void msWrite2Byte( DWORD u32RegLoAddr, WORD u16Value );
INTERFACE void msWriteByteMask( DWORD u32Reg, BYTE u8Value, BYTE u8Mask );
INTERFACE void msWrite2ByteMask( DWORD u32RegLoAddr, WORD wValue , WORD ucMask );
//INTERFACE void scWrite2ByteMask( BYTE u8RegLoAddr, WORD u16Value, WORD u16Mask );
//INTERFACE void scWrite3Byte( BYTE u8RegLoAddr, DWORD u32Value );
INTERFACE void msWrite3Byte( DWORD u32RegLoAddr, DWORD u32Value );
INTERFACE void msWrite4Byte( DWORD u32RegLoAddr, DWORD u32Value );


#if ENABLE_SW_DOUBLE_BUFFER
#define DB_Mode(mode)                       msSWDBMode(mode)
#define DB_WB(wReg,ucVal)                   msSWDBWriteByte(wReg,ucVal)
#define DB_W2B(wReg,wVal)                   msSWDBWrite2Byte(wReg,wVal)
#define DB_W3B(wReg,wVal)                   msSWDBWrite3Byte(wReg,wVal)
#define DB_WBMask(wReg,ucVal,ucMask)        msSWDBWriteByteMask(wReg,ucVal,ucMask)
#define DB_W2BMask(wReg,wVal,wMask)         msSWDBWrite2ByteMask(wReg,wVal,wMask)
#define DB_WBit(wReg,bBit,ucBitPos)         msSWDBWriteBit(wReg,bBit,ucBitPos)
#else
#define DB_Mode(mode)
#define DB_WB(wReg,ucVal)                   msWriteByte(wReg,ucVal)
#define DB_W2B(wReg,wVal)                   msWrite2Byte(wReg,wVal)
#define DB_W3B(wReg,wVal)                   msWrite3Byte(wReg,wVal)
#define DB_WBMask(wReg,ucVal,ucMask)        msWriteByteMask(wReg,ucVal,ucMask)
#define DB_W2BMask(wReg,wVal,wMask)         msWrite2ByteMask(wReg,wVal,wMask)
#define DB_WBit(wReg,bBit,ucBitPos)         msWriteBit(wReg,bBit,ucBitPos)
#endif

#if 1//ENABLE_SW_DOUBLE_BUFFER
#define SWDB_MAX_SIZE  99 // Beware of overflow problem in xdata allocation

typedef struct
{
    BYTE ucSWDB_OnFire;
    BYTE ucSWDB_Mode;
    BYTE ucSWDB_Mode_Log;
    BYTE ucSWDB_LastSCBank;
    BYTE ucSWDB_Num;
    BYTE ucSWDB_Counter;
    BYTE ucSWDB_Buffer[SWDB_MAX_SIZE];
} StuSWDBCtrl;

typedef enum _DBTYPE
{
    eDB_NORMAL_MODE     = 0x00,
    eDB_SUCCESSIVE_MODE = 0x40,
    eDB_BURST_MODE      = 0x80,
}DB_TYPE;



INTERFACE void msSWDBInit(void);
INTERFACE void msSWDBMode(DB_TYPE type);
INTERFACE void msSWDBWriteByte(WORD wReg,BYTE ucVal);
INTERFACE void msSWDBWrite2Byte(WORD wReg,WORD wVal);
INTERFACE void msSWDBWrite3Byte(WORD wReg, DWORD dwValue);
INTERFACE void msSWDBWriteByteMask(WORD wReg,BYTE ucVal,BYTE ucMask);
INTERFACE void msSWDBWrite2ByteMask(WORD wReg,WORD wVal,WORD wMask);
INTERFACE void msSWDBWriteBit(WORD wReg,BOOL bBit,BYTE ucBitPos);
INTERFACE void msSWDBWriteToRegister(void);
INTERFACE void msSWDBWaitForRdy(void);
#endif




#undef INTERFACE


#endif   ///_MS_RWREG_H

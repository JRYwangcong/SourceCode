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
#include "Global.h"
#ifndef _MS_RWREG_H
#define _MS_RWREG_H

#ifdef _MS_RWREG_C
#define INTERFACE
#else
#define INTERFACE   extern
#endif
//#define _REG_DIRECT_ACCESS_

/////////////////////////////////////////////////////////////////////////////
//Must use sc series R/W function if input scaler bank address is BYTE data.
//Must use bank save/restore future while use sc series function.
/////////////////////////////////////////////////////////////////////////////
#define MAIN_WINDOW 0
#define SUB_WINDOW  1

#define __msRegs            ((unsigned char  volatile *) RIU_MAP)
#define msRegs(u32Reg)      __msRegs[(((u32Reg) & 0x00FFFFFF) * 2) - ((u32Reg) & 1)]

//////////////////////
/////// old ///////////
#define _msRegs        ((unsigned char volatile xdata *) 0x000000) // dram 0x0000, currently no used
#define _scRegs        ((unsigned char volatile xdata *) 0x000000) // dram 0x0000, currently no used
/////// old ///////////
//////////////////////

////////////////////////
// RIU Access Semaphore//
extern U16 g_u16RIUReqCnt;
#define ReqstRIU()          do {\
                                WORD u16Timeout = 0xFFFF;\
                                g_u16RIUReqCnt++;\
                                do {\
                                    u16Timeout--;\
                                    GetRIU();\
                                }while(u16Timeout && !IsRIUGet());\
                            }while(0)
#define RelesRIU()          do {\
                                g_u16RIUReqCnt--;\
                                if(IsRIUGet() && (g_u16RIUReqCnt==0))\
                                    FreeRIU();\
                            }while(0)
// RIU Access Semaphore//
////////////////////////

//#define MAKEWORD(value1, value2)    (((WORD)(value1))<<8) + (value2)
//Jison 110308 for GPIO define and can be used in interrupt
////////////////////////////////////////////////////////////////////////////////
// Need Change Sub Bank Manually if necessary, for performanace consideration in ISR !!!! //
////////////////////////////////////////////////////////////////////////////////
#define MEM_MSREAD_BYTE(u32Reg)                        (msRegs(u32Reg))
#define MEM_MSWRITE_BYTE(u32Reg,u8Value)               (msRegs(u32Reg)=(u8Value))
#define MEM_MSWRITE_BYTE_MASK(u32Reg,u8Value,u8Mask)   (msWriteByteMask(u32Reg,u8Value,u8Mask))//(msRegs(u32Reg) = ( (msRegs(u32Reg) & (~(u8Mask))) | ((u8Value) & (u8Mask))))
#define MEM_MSWRITE_BIT(u32Reg,bTrue,u8Bit)            (msWriteBit(u32Reg,bTrue,u8Bit))//(msRegs(u32Reg) = ( (msRegs(u32Reg) & (~(u8Bit))) | ((u8True)?(u8Bit):0)))

//////////////////////
/////// old ///////////
extern DWORD OldRWFuncCnt;
#define _MEM_MSWRITE_BYTE(u16Addr,u8Value)               (OldRWFuncCnt+=(u16Addr+u8Value),OldRWFuncCnt-=(u16Addr+u8Value),(OldRWFuncCnt++))
#define _MEM_MSWRITE_BYTE_MASK(u16Addr,u8Value,u8Mask)   (OldRWFuncCnt+=(u16Addr+u8Value+u8Mask),OldRWFuncCnt-=(u16Addr+u8Value+u8Mask),(OldRWFuncCnt++))
#define _MEM_MSWRITE_BIT(u16Addr,u8True,u8Bit)           (OldRWFuncCnt+=(u16Addr+u8True+u8Bit),OldRWFuncCnt-=(u16Addr+u8True+u8Bit),(OldRWFuncCnt++))
#define _MEM_SCWRITE_BYTE(u16Addr,u8Value)               (OldRWFuncCnt+=(u16Addr+u8Value),OldRWFuncCnt-=(u16Addr+u8Value),(OldRWFuncCnt++))
/////// old ///////////
//////////////////////
#ifdef EXTERNAL_MCU
// 8 bit ddr direct bus
#define ddrBusSet(_ALE_PIN_, _BIT_, _is_hl_)\
        if( _is_hl_ )\
            _ALE_PIN_ |= _BIT_;\
        else\
            _ALE_PIN_ &= (~_BIT_);

#define Delay_ExtRIU()  DelayUs(20)

#define XRIU_BIT_ALE2   _BIT4
#define XRIU_BIT_WRE2   _BIT5
#define XRIU_BIT_RDE2   _BIT6
#define XRIU_CTRL2      msRegs(REG_000440)
#define XRIU_DATA2      msRegs(REG_000446)
#define XRIU_DATA_IN2   msRegs(REG_000448)
#define XRIU_DATA_OEZ2  msRegs(REG_000447)

#define _ddrBusInit2()\
    msRegs(REG_000442) = 0x8F;\
    ddrBusSet(XRIU_CTRL2, XRIU_BIT_ALE2, _LOW);\
    ddrBusSet(XRIU_CTRL2, XRIU_BIT_RDE2, _HIGH);\
    ddrBusSet(XRIU_CTRL2, XRIU_BIT_WRE2, _HIGH);\

#define _ddrBusSetAddr2(_ADDR_)\
    {\
        XRIU_DATA2 = ((_ADDR_)>>8) & 0xFF;\
        Delay_ExtRIU();\
        ddrBusSet(XRIU_CTRL2, XRIU_BIT_ALE2, _HIGH);\
        Delay_ExtRIU();\
        XRIU_DATA2 = (_ADDR_) & 0xFF;\
        Delay_ExtRIU();\
        ddrBusSet(XRIU_CTRL2, XRIU_BIT_ALE2, _LOW);\
        Delay_ExtRIU();\
    }

#define _ddrBusSetData2(_DATA_)\
    {\
        XRIU_DATA2 = _DATA_;\
        Delay_ExtRIU();\
        ddrBusSet(XRIU_CTRL2, XRIU_BIT_WRE2, _LOW);\
        Delay_ExtRIU();\
        Delay_ExtRIU();\
        ddrBusSet(XRIU_CTRL2, XRIU_BIT_WRE2, _HIGH);\
        Delay_ExtRIU();\
        Delay_ExtRIU();\
    }

#define _ddrBusGetData2(_DATA_)\
    {\
        Delay_ExtRIU();\
        XRIU_DATA_OEZ2 = 0xFF;\
        Delay_ExtRIU();\
        ddrBusSet(XRIU_CTRL2, XRIU_BIT_RDE2, _LOW);\
        Delay_ExtRIU();\
        Delay_ExtRIU();\
        _DATA_ = XRIU_DATA_IN2;\
        ddrBusSet(XRIU_CTRL2, XRIU_BIT_RDE2, _HIGH);\
        XRIU_DATA_OEZ2 = 0x00;\
        Delay_ExtRIU();\
    }
#endif

// memory R/W functions - non-cached !! directly access memory.
INTERFACE U8 msMemReadByte(U32 u32MemAddr );
INTERFACE U16 msMemRead2Byte(U32 u32MemAddr );
INTERFACE U32 msMemRead3Byte(U32 u32MemAddr );
INTERFACE U32 msMemRead4Byte(U32 u32MemAddr );
INTERFACE void msMemWriteByte(U32 u32MemAddr, U8 u8Data );
INTERFACE void msMemWrite2Byte(U32 u32MemAddr, U16 u16Data );
INTERFACE void msMemWrite3Byte( U32 u32RegLoAddr, U32 u32Value );
INTERFACE void msMemWrite4Byte( U32 u32RegLoAddr, U32 u32Value );
INTERFACE void msMemWriteByteMask( U32 u32Reg, U8 u8Value, U8 u8Mask );
INTERFACE void msMemWrite2ByteMask( U32 u32RegLoAddr, U16 u16Value , U16 u16Mask );
INTERFACE void msMemWrite3ByteMask( U32 u32RegLoAddr, U32 u32Value , U32 u32Mask );
INTERFACE void msMemWrite4ByteMask( U32 u32RegLoAddr, U32 u32Value , U32 u32Mask );
INTERFACE void msSWDBWriteByte(DWORD dwReg,BYTE ucVal);
INTERFACE void msSWDBWrite2Byte(DWORD dwReg,WORD wVal);
INTERFACE void msSWDBWrite3Byte(DWORD dwReg, DWORD dwValue);
INTERFACE void msSWDBWrite4Byte(DWORD dwReg, DWORD dwValue);
INTERFACE void msSWDBWriteByteMask(DWORD dwReg,BYTE ucVal,BYTE ucMask);
INTERFACE void msSWDBWrite2ByteMask(DWORD dwReg,WORD wVal,WORD wMask);
INTERFACE void msSWDBWriteBit(DWORD dwReg,BOOL bBit,BYTE ucBitPos);
INTERFACE void msSWDBTrigger(void);


#if ENABLE_MENULOAD
#define DB_WB(dwReg,ucVal)                   (u8MenuLoadFlag)?msSWDBWriteByte(dwReg,ucVal):msWriteByte(dwReg,ucVal)
#define DB_W2B(dwReg,wVal)                   (u8MenuLoadFlag)?msSWDBWrite2Byte(dwReg,wVal):msWrite2Byte(dwReg,wVal)
#define DB_W3B(dwReg,dwVal)                  (u8MenuLoadFlag)?msSWDBWrite3Byte(dwReg,dwVal):msWrite3Byte(dwReg,dwVal)
#define DB_W4B(dwReg,dwVal)                  (u8MenuLoadFlag)?msSWDBWrite4Byte(dwReg,dwVal):msWrite4Byte(dwReg,dwVal)
#define DB_WBMask(dwReg,ucVal,ucMask)        (u8MenuLoadFlag)?msSWDBWriteByteMask(dwReg,ucVal,ucMask):msWriteByteMask(dwReg,ucVal,ucMask)
#define DB_W2BMask(dwReg,wVal,wMask)         (u8MenuLoadFlag)?msSWDBWrite2ByteMask(dwReg,wVal,wMask):msWrite2ByteMask(dwReg,wVal,wMask)
#define DB_WBit(dwReg,bBit,ucBitPos)         (u8MenuLoadFlag)?msSWDBWriteBit(dwReg,bBit,ucBitPos):msWriteBit(dwReg,bBit,ucBitPos)
#define DB_Trigger()                         msSWDBTrigger()
#define MENU_LOAD_START()   {u8MenuLoadFlag = 1;msDrvSCCommonWrite(SCALER_MASK_MAIN, ENABLE); msWriteByteMask(SCL0_00_02, 0x00,BIT0);msDrvSCCommonWrite(SCALER_MASK_MAIN, DISABLE);}
#define MENU_LOAD_END()      {DB_Trigger();u8MenuLoadFlag = 0;msDrvSCCommonWrite(SCALER_MASK_MAIN, ENABLE); msWriteByteMask(SCL0_00_02, BIT0,BIT0);msDrvSCCommonWrite(SCALER_MASK_MAIN, DISABLE);}
#else
#define DB_WB(dwReg,ucVal)                   msWriteByte(dwReg,ucVal)
#define DB_W2B(dwReg,wVal)                   msWrite2Byte(dwReg,wVal)
#define DB_W3B(dwReg,dwVal)                   msWrite3Byte(dwReg,dwVal)
#define DB_W4B(dwReg,dwVal)                  msWrite4Byte(dwReg,dwVal)
#define DB_WBMask(dwReg,ucVal,ucMask)        msWriteByteMask(dwReg,ucVal,ucMask)
#define DB_W2BMask(dwReg,wVal,wMask)         msWrite2ByteMask(dwReg,wVal,wMask)
#define DB_WBit(dwReg,bBit,ucBitPos)         msWriteBit(dwReg,bBit,ucBitPos)
#define DB_Trigger()
#define MENU_LOAD_START()
#define MENU_LOAD_END()
#endif
#ifdef _REG_DIRECT_ACCESS_
#define _msReadByte(u16Addr)                     (((u16Addr)&0x8000) ?\
                                                ((_scRegs[0]=((u16Addr)>>8)&0x7F), _scRegs[(u16Addr)&0xFF]) :\
                                                (_msRegs[(u16Addr)]))

#define _msWriteByte(u16Addr,u8Value)            (((u16Addr)&0x8000) ?\
                                                ((_scRegs[0]=((u16Addr)>>8)&0x7F) ? (_scRegs[(u16Addr)&0xFF]=(u8Value)):(_scRegs[(u16Addr)&0xFF]=(u8Value))) :\
                                                (_msRegs[u16Addr] = (u8Value)))

//#define _msWriteByteMask( wReg,  ucValue,  ucMask )  (_msWriteByte( (wReg), ( (_msReadByte( wReg ) & ( ~(ucMask) )) | ( (ucValue) & (ucMask) ) ) ))

#define _msWriteBit(u16Addr,u8True,u8Bit)        _msWriteByte((u16Addr),_msReadByte(u16Addr)&(~(u8Bit))|((u8True)?(u8Bit):0))

#define _msRead2Byte( u8RegLoAddr )              ((((WORD)_msReadByte((u8RegLoAddr)+1))<<8)|_msReadByte((u8RegLoAddr)))

//#define _msWrite2Byte(wReg, wValue )       {_msWriteByte( (wReg) - 1, (wValue)&0xFF ); _msWriteByte( (wReg), ( (wValue) >> 8 ) );}

//#define _scReadByte( u16Addr )                   (_scRegs[(u16Addr)&0xFF])

//#define _scWriteByte( u16Addr, u16Value )        (_scRegs[(u16Addr)&0xFF] = (u16Value))

//#define scRead2Byte( u8RegLoAddr )              ((((WORD)_scReadByte((u8RegLoAddr)+1))<<8)|_scReadByte((u8RegLoAddr)))

//#define scWrite2Byte(u8RegLoAddr,u16Value)      {(_scWriteByte((u8RegLoAddr),(u16Value)&0xFF)); _scWriteByte((u8RegLoAddr)+1,((u16Value)>>8));}

//#define scWriteByteMask(u8Reg,u8Value,u8Mask)   (_scWriteByte( (u8Reg), ( (_scReadByte( u8Reg ) & ( ~(u8Mask) )) | ( (u8Value) & (u8Mask) ) ) ))

#else
INTERFACE BYTE scReadByte( DWORD u32Reg );
INTERFACE void scWriteByte( DWORD u32Reg, BYTE u8Val );
INTERFACE BYTE msReadByte( DWORD u32Reg );
INTERFACE void msWriteByte( DWORD u32Reg, BYTE u8Val );
INTERFACE WORD msRead2Byte( DWORD u32RegLoAddr );
INTERFACE void msWriteBit( DWORD u32Reg, Bool bBit, BYTE u8BitPos );

#ifdef EXTERNAL_MCU
INTERFACE void msWriteByte_Ext(DWORD u32Reg, BYTE u8Val);
INTERFACE BYTE msReadByte_Ext(DWORD u32Reg);
#endif
//////////////////////
/////// old ///////////
INTERFACE BYTE _msReadByte( WORD u16Reg );
INTERFACE void _msWriteByte( WORD u16Reg, BYTE u8Val );
INTERFACE WORD _msRead2Byte( WORD u8RegLoAddr );
INTERFACE void _msWriteBit( WORD u16Reg, Bool bBit, BYTE u8BitPos );
INTERFACE BYTE _scReadByte( WORD u16Reg );
INTERFACE void _scWriteByte( WORD u16Reg, BYTE u8Value );
/////// old ///////////
//////////////////////
#endif
INTERFACE void msWrite2Byte( DWORD u32RegLoAddr, WORD u16Value );
INTERFACE void scWriteByteMask( DWORD u32Reg, BYTE u8Value, BYTE u8Mask );
INTERFACE void msWriteByteMask( DWORD u32Reg, BYTE u8Value, BYTE u8Mask );
INTERFACE void msDrvSCCommonWrite( WORD u8SCMask, BOOL bEnable);
INTERFACE void msWrite2ByteMask( DWORD u32RegLoAddr, WORD wValue , WORD ucMask );
INTERFACE DWORD msRead3Byte( DWORD u32RegLoAddr );
INTERFACE void msWrite3Byte( DWORD u32RegLoAddr, DWORD u32Value );
INTERFACE void msWrite3ByteMask( DWORD u32RegLoAddr, DWORD u32Value , DWORD u32Mask );
INTERFACE DWORD msRead4Byte( DWORD u32RegLoAddr );
INTERFACE void msWrite4Byte( DWORD u32RegLoAddr, DWORD u32Value );
INTERFACE void msWrite4ByteMask( DWORD u32RegLoAddr, DWORD u32Value , DWORD u32Mask );
//////////////////////
/////// old ///////////
INTERFACE void _msWrite2Byte( WORD u16RegLoAddr, WORD u16Value );
INTERFACE void _msWriteByteMask( WORD u16Reg, BYTE u8Value, BYTE u8Mask );
INTERFACE void _msWrite2ByteMask( WORD u16RegLoAddr, WORD wValue , WORD ucMask );
INTERFACE void _msWrite3Byte( WORD u16RegLoAddr, DWORD u32Value );
INTERFACE BYTE _MEM_MSREAD_BYTE( WORD u16Reg );
INTERFACE BYTE _MEM_SCREAD_BYTE( WORD u16Reg );

/////// old ///////////
//////////////////////

#undef INTERFACE


#endif   ///_MS_RWREG_H

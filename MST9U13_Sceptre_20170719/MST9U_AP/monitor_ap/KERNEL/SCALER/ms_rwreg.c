///////////////////////////////////////////////////////////////////////////////
/// @file ms_rwreg.h
/// @brief MStar Scaler register access.
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
#define _MS_RWREG_C

#include "datatype.h"
#include "Board.h"
#include "Global.h"
#include "ms_reg.h"
#include "misc.h"
#include "Ms_rwreg.h"
#if ENABLE_MENULOAD
#include "apiXC_SC_MenuLoad.h"
#endif

#define DELAY1US()  _nop_()

U16 g_u16RIUReqCnt = 0;

U8 msMemReadByte(U32 u32MemAddr )
{
    U8 *pu8;
    pu8 = (U8*)(u32MemAddr | (0x80000000)); // non-cached
    return *pu8;
}

void msMemWriteByte(U32 u32MemAddr, U8 u8Data )
{
    U8 *pu8;
    pu8 = (U8*)(u32MemAddr| (0x80000000)); // non-cached
    *pu8 = u8Data;
}
void msMemWriteByteMask( U32 u32Reg, U8 u8Value, U8 u8Mask )
{
    msMemWriteByte( u32Reg, ( (msMemReadByte( u32Reg ) & ( ~u8Mask )) | ( u8Value & u8Mask ) ) );
}

U16 msMemRead2Byte(U32 u32MemAddr )
{
    return ((( U16 )msMemReadByte( u32MemAddr+1 ) ) << 8 ) | msMemReadByte( u32MemAddr );
}

void msMemWrite2Byte(U32 u32MemAddr, U16 u16Data )
{
    msMemWriteByte( u32MemAddr, ( U8 )u16Data );
    msMemWriteByte( u32MemAddr + 1, ( U8 )( u16Data >> 8 ) );
}

void msMemWrite2ByteMask( U32 u32RegLoAddr, U16 u16Value , U16 u16Mask )
{
    msMemWriteByteMask( u32RegLoAddr, ( U8 )u16Value, ( U8 ) u16Mask );
    msMemWriteByteMask( u32RegLoAddr + 1, ( U8 )( u16Value >> 8 ), ( U8 )( u16Mask >> 8 ) );
}

U32 msMemRead3Byte(U32 u32MemAddr )
{
    return ((( U32 )msMemReadByte( u32MemAddr+2 ) ) << 16 ) | ((( U32 )msMemReadByte( u32MemAddr+1 ) ) << 8 ) | msMemReadByte( u32MemAddr );
}

void msMemWrite3Byte( U32 u32RegLoAddr, U32 u32Value )
{
    msMemWriteByte( u32RegLoAddr, ( U8 )u32Value );
    msMemWriteByte( u32RegLoAddr + 1, ( U8 )( u32Value >> 8 ) );
    msMemWriteByte( u32RegLoAddr + 2, ( U8 )( u32Value >> 16 ) );
}

void msMemWrite3ByteMask( U32 u32RegLoAddr, U32 u32Value , U32 u32Mask )
{
    msMemWriteByteMask( u32RegLoAddr, ( U8 )u32Value, ( U8 ) u32Mask );
    msMemWriteByteMask( u32RegLoAddr + 1, ( U8 )( u32Value >> 8 ), ( U8 )( u32Mask >> 8 ) );
    msMemWriteByteMask( u32RegLoAddr + 2, ( U8 )( u32Value >> 16 ), ( U8 )( u32Mask >> 16 ) );
}

U32 msMemRead4Byte(U32 u32MemAddr )
{
    return ((( U32 )msMemReadByte( u32MemAddr+3 ) ) << 24 ) | ((( U32 )msMemReadByte( u32MemAddr+2 ) ) << 16 ) | ((( U32 )msMemReadByte( u32MemAddr+1 ) ) << 8 ) | msMemReadByte( u32MemAddr );
}

void msMemWrite4Byte( U32 u32RegLoAddr, U32 u32Value )
{
    msMemWriteByte( u32RegLoAddr, ( U8 )u32Value );
    msMemWriteByte( u32RegLoAddr + 1, ( U8 )( u32Value >> 8 ) );
    msMemWriteByte( u32RegLoAddr + 2, ( U8 )( u32Value >> 16 ) );
    msMemWriteByte( u32RegLoAddr + 3, ( U8 )( u32Value >> 24 ) );
}

void msMemWrite4ByteMask( U32 u32RegLoAddr, U32 u32Value , U32 u32Mask )
{
    msMemWriteByteMask( u32RegLoAddr, ( U8 )u32Value, ( U8 ) u32Mask );
    msMemWriteByteMask( u32RegLoAddr + 1, ( U8 )( u32Value >> 8 ), ( U8 )( u32Mask >> 8 ) );
    msMemWriteByteMask( u32RegLoAddr + 2, ( U8 )( u32Value >> 16 ), ( U8 )( u32Mask >> 16 ) );
    msMemWriteByteMask( u32RegLoAddr + 3, ( U8 )( u32Value >> 24 ), ( U8 )( u32Mask >> 24 ) );
}
#if 1//DIRECT_BUS
/////////////////////////////////////////////////////////////////////////
#ifdef EXTERNAL_MCU
void msWriteByte_Ext(DWORD u32Reg, BYTE u8Val)
{

    //_intrDisable();
    _ddrBusInit2();
 #if 1
    {
        // Address
        _ddrBusSetAddr2(((WORD)0x0000));

        // Data
        _ddrBusSetData2((u32Reg>>16)&0x00FF);

    }
#endif
    {
        // Address
        _ddrBusSetAddr2(u32Reg);
    }

    // Data
    _ddrBusSetData2(u8Val);
    //_intrEnable();

}


BYTE msReadByte_Ext(DWORD u32Reg)
{
    BYTE u8value;

    //_intrDisable();
    _ddrBusInit2();
    #if 1
    {
        // Address
        _ddrBusSetAddr2((WORD)0x0000);

        // Data
        _ddrBusSetData2((u32Reg>>16)&0x00FF);

    }
   #endif
    {
        // Address
        _ddrBusSetAddr2(u32Reg&0xFFFF);
    }

    // Data
    _ddrBusGetData2(u8value);

    //_intrEnable();
    return u8value;

}


#endif

#ifndef _REG_DIRECT_ACCESS_
BYTE scReadByte( DWORD u32Reg )
{
    return msRegs(u32Reg);
}

void scWriteByte( DWORD u32Reg, BYTE u8Val )
{
#if ENABLE_SW_SCALER_COMMON_WRITE
    DWORD u32SCRegBase;
    BYTE u8SCIdx;
    WORD u16SCTemp;

    if (g_u16CommonWriteSCMask)
    {
        u16SCTemp = g_u16CommonWriteSCMask;
        u8SCIdx = 0;
        u32Reg &= ~0x00FFFF00;

        while(u16SCTemp!=0)
        {
            if(u16SCTemp & (BIT0|BIT1))
            {
                u32SCRegBase = SCRegBase(u8SCIdx);
                msRegs(u32SCRegBase+u32Reg) = u8Val;
            }
            u16SCTemp >>= 2;
            u8SCIdx += 2;
        }
    }
    else
#endif
        msRegs(u32Reg) = u8Val;
}

BYTE msReadByte( DWORD u32Reg )
{
    if( u32Reg & 0x80000000 ) // Sub bank
    {
        msRegs((u32Reg & 0x00FFFF00)) = ( u32Reg >> 24 ) & 0x7F;
        return msRegs(u32Reg);
    }
    else
    {
        return msRegs(u32Reg);
    }
}

void msWriteByte( DWORD u32Reg, BYTE u8Val )
{
#if ENABLE_SW_SCALER_COMMON_WRITE
    DWORD u32SCRegBase;
    BYTE u8SCIdx;
    WORD u16SCTemp;

    if (g_u16CommonWriteSCMask && (u32Reg & 0x80000000))
    {
        u16SCTemp = g_u16CommonWriteSCMask;
        u8SCIdx = 0;
        u32Reg &= ~0x00FFFF00;

        while(u16SCTemp!=0)
        {
            if(u16SCTemp & (BIT0|BIT1))
            {
                u32SCRegBase = SCRegBase(u8SCIdx);
                msRegs(u32SCRegBase) = ( u32Reg >> 24 ) & 0x7F;
                msRegs(u32SCRegBase+u32Reg) = u8Val;
            }
            u16SCTemp >>= 2;
            u8SCIdx += 2;
        }
    }
    else
#endif
    {
        if( u32Reg & 0x80000000 ) // Sub bank
        {
            msRegs((u32Reg & 0x00FFFF00)) = ( u32Reg >> 24 ) & 0x7F;
            msRegs(u32Reg) = u8Val;
        }
        else
        {
            msRegs(u32Reg) = u8Val;
        }
    }
}
void msWriteBit_Isr( DWORD u32Reg, Bool bBit, BYTE u8BitPos )
{
    BYTE u8Value;

    //ReqstRIU();
    u8Value = msReadByte( u32Reg );
    if( bBit )
    {
        u8Value = u8Value | u8BitPos;
    }
    else
    {
        u8Value = u8Value & ( ~u8BitPos );
    }
    msWriteByte( u32Reg, u8Value );
    //RelesRIU();
}
void msWriteBit( DWORD u32Reg, Bool bBit, BYTE u8BitPos )
{
    BYTE u8Value;

    ReqstRIU();
    u8Value = msReadByte( u32Reg );
    if( bBit )
    {
        u8Value = u8Value | u8BitPos;
    }
    else
    {
        u8Value = u8Value & ( ~u8BitPos );
    }
    msWriteByte( u32Reg, u8Value );
    RelesRIU();
}

//Jison:110428 change msRead2Byte() u16Reg to low addr.
WORD msRead2Byte( DWORD u32RegLoAddr )
{
    return ((( WORD )msReadByte( u32RegLoAddr+1 ) ) << 8 ) | msReadByte( u32RegLoAddr );
}
#endif
//Jison:110428 u16Reg is low addr.
void msWrite2Byte( DWORD u32RegLoAddr, WORD u16Value )
{
    msWriteByte( u32RegLoAddr, ( BYTE )u16Value );
    msWriteByte( u32RegLoAddr + 1, ( BYTE )( u16Value >> 8 ) );
}

void scWriteByteMask( DWORD u32Reg, BYTE u8Value, BYTE u8Mask )
{
    ReqstRIU();
    scWriteByte( u32Reg, ( (scReadByte( u32Reg ) & ( ~u8Mask )) | ( u8Value & u8Mask ) ) );
    RelesRIU();
}

void msWriteByteMask( DWORD u32Reg, BYTE u8Value, BYTE u8Mask )
{
    ReqstRIU();
    msWriteByte( u32Reg, ( (msReadByte( u32Reg ) & ( ~u8Mask )) | ( u8Value & u8Mask ) ) );
    RelesRIU();
}

void msWriteByteMask_Isr( DWORD u32Reg, BYTE u8Value, BYTE u8Mask )
{
    //ReqstRIU();
    msWriteByte( u32Reg, ( (msReadByte( u32Reg ) & ( ~u8Mask )) | ( u8Value & u8Mask ) ) );
    //RelesRIU();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: msDrvSCCommonWrite
/// @param <IN>         \b u8SCMask: 4 scaler mask
/// @param <IN>         \b bEnable: function enable or disable
/// @brief \b Function  \b Description: Set common write for 4 scaler.
/// Note that the scaler main and sub setting are in the different address.
/// If main scaler is set, common write sub scaler would not be accessed.
////////////////////////////////////////////////////////////////////////////////
void msDrvSCCommonWrite( WORD u8SCMask, BOOL bEnable)
{
#if ENABLE_SW_SCALER_COMMON_WRITE
    if (bEnable)
        g_u16CommonWriteSCMask = u8SCMask;
    else
        g_u16CommonWriteSCMask = 0;
#else
    DWORD u32SCRegBase;
    BYTE u8SCIdx;
    WORD u8SCMaskTemp;
    BYTE u8CommWRMask = 0;

    u8SCMaskTemp = u8SCMask;
    u8SCIdx = 0;
    while(u8SCMaskTemp!=0)
    {
        if(u8SCMaskTemp & BIT0)
        {
            switch(u8SCIdx)
            {
                case SCL0_MAIN:
                case SCL0_SUB:
                    u8CommWRMask|=BIT0;
                    break;
                case SCR0_MAIN:
                case SCR0_SUB:
                    u8CommWRMask|=BIT2;
                    break;
#if (CHIP_ID!=MST9U3)
                case SCL1_MAIN:
                case SCL1_SUB:
                    u8CommWRMask|=BIT1;
                    break;
                case SCR1_MAIN:
                case SCR1_SUB:
                    u8CommWRMask|=BIT3;
                    break;
#endif
                default:
                    break;
            }
        }
        u8SCMaskTemp >>=1;
        u8SCIdx++;
    }
    u8SCMaskTemp = u8SCMask;
    u8SCIdx = 0;
    while(u8SCMaskTemp!=0)
    {
        if(u8SCMaskTemp & BIT0)
        {
            u32SCRegBase = SCRegBase(u8SCIdx);
            msWriteBit(u32SCRegBase+SC00_84, bEnable, u8CommWRMask);
        }

        u8SCMaskTemp >>=1;
        u8SCIdx ++;
    }
#endif
}

//Jison:110428 u16Reg is low addr.
void msWrite2ByteMask( DWORD u32RegLoAddr, WORD u16Value , WORD u16Mask )
{
    msWriteByteMask( u32RegLoAddr, ( BYTE )u16Value, ( BYTE ) u16Mask );
    msWriteByteMask( u32RegLoAddr + 1, ( BYTE )( u16Value >> 8 ), ( BYTE )( u16Mask >> 8 ) );
}

DWORD msRead3Byte( DWORD u32RegLoAddr )
{
    return ((( DWORD )msReadByte( u32RegLoAddr+2 ) ) << 16 ) | ((( DWORD )msReadByte( u32RegLoAddr+1 ) ) << 8 ) | msReadByte( u32RegLoAddr );
}
void msWrite3Byte( DWORD u32RegLoAddr, DWORD u32Value )
{
    msWriteByte( u32RegLoAddr, ( BYTE )u32Value );
    msWriteByte( u32RegLoAddr + 1, ( BYTE )( u32Value >> 8 ) );
    msWriteByte( u32RegLoAddr + 2, ( BYTE )( u32Value >> 16 ) );
}
void msWrite3ByteMask( DWORD u32RegLoAddr, DWORD u32Value , DWORD u32Mask )
{
    msWriteByteMask( u32RegLoAddr, ( BYTE )u32Value, ( BYTE ) u32Mask );
    msWriteByteMask( u32RegLoAddr + 1, ( BYTE )( u32Value >> 8 ), ( BYTE )( u32Mask >> 8 ) );
    msWriteByteMask( u32RegLoAddr + 2, ( BYTE )( u32Value >> 16 ), ( BYTE )( u32Mask >> 16 ) );
}
DWORD msRead4Byte( DWORD u32RegLoAddr )
{
    return ((( DWORD )msReadByte( u32RegLoAddr+3 ) ) << 24 ) | ((( DWORD )msReadByte( u32RegLoAddr+2 ) ) << 16 ) | ((( DWORD )msReadByte( u32RegLoAddr+1 ) ) << 8 ) | msReadByte( u32RegLoAddr );
}
void msWrite4Byte( DWORD u32RegLoAddr, DWORD u32Value )
{
    msWriteByte( u32RegLoAddr, ( BYTE )u32Value );
    msWriteByte( u32RegLoAddr + 1, ( BYTE )( u32Value >> 8 ) );
    msWriteByte( u32RegLoAddr + 2, ( BYTE )( u32Value >> 16 ) );
    msWriteByte( u32RegLoAddr + 3, ( BYTE )( u32Value >> 24 ) );
}
void msWrite4ByteMask_Isr( DWORD u32RegLoAddr, DWORD u32Value , DWORD u32Mask )
{
    msWriteByteMask_Isr( u32RegLoAddr, ( BYTE )u32Value, ( BYTE ) u32Mask );
    msWriteByteMask_Isr( u32RegLoAddr + 1, ( BYTE )( u32Value >> 8 ), ( BYTE )( u32Mask >> 8 ) );
    msWriteByteMask_Isr( u32RegLoAddr + 2, ( BYTE )( u32Value >> 16 ), ( BYTE )( u32Mask >> 16 ) );
    msWriteByteMask_Isr( u32RegLoAddr + 3, ( BYTE )( u32Value >> 24 ), ( BYTE )( u32Mask >> 24 ) );
}
void msWrite4ByteMask( DWORD u32RegLoAddr, DWORD u32Value , DWORD u32Mask )
{
    msWriteByteMask( u32RegLoAddr, ( BYTE )u32Value, ( BYTE ) u32Mask );
    msWriteByteMask( u32RegLoAddr + 1, ( BYTE )( u32Value >> 8 ), ( BYTE )( u32Mask >> 8 ) );
    msWriteByteMask( u32RegLoAddr + 2, ( BYTE )( u32Value >> 16 ), ( BYTE )( u32Mask >> 16 ) );
    msWriteByteMask( u32RegLoAddr + 3, ( BYTE )( u32Value >> 24 ), ( BYTE )( u32Mask >> 24 ) );
}
#if ENABLE_MENULOAD
void msSWDBWriteByte(DWORD dwReg,BYTE ucValue)
{
    MApi_XC_MLoad_WriteCmd(dwReg, (0x0000)|(ucValue), 0x00FF);
}


void msSWDBWrite2Byte(DWORD dwReg,WORD wValue)
{
    MApi_XC_MLoad_WriteCmd(dwReg, wValue, 0xFFFF);
}

void msSWDBWrite3Byte(DWORD dwReg, DWORD dwValue)
{
    if(dwReg&BIT0)//odd addr
    {
        MApi_XC_MLoad_WriteCmd(dwReg, (0x0000)|dwValue, 0x00FF);
        MApi_XC_MLoad_WriteCmd(dwReg + 1, (0x0000)|( dwValue >> 8 ), 0xFFFF);
    }
    else // even addr
    {
        MApi_XC_MLoad_WriteCmd(dwReg, dwValue, 0xFFFF);
        MApi_XC_MLoad_WriteCmd(dwReg + 2, (0x0000)|( dwValue >> 16 ), 0x00FF);
    }
}

void msSWDBWrite4Byte(DWORD dwReg, DWORD dwValue)
{
    if(dwReg&BIT0)//odd addr
    {
        MApi_XC_MLoad_WriteCmd(dwReg - 1, (0x0000)|( dwValue << 8 ), 0xFF00);
        MApi_XC_MLoad_WriteCmd(dwReg + 1, (0x0000)|( dwValue >> 8 ), 0xFFFF);
        MApi_XC_MLoad_WriteCmd(dwReg + 3, (0x0000)|( dwValue >> 24 ), 0x00FF);
    }
    else // even addr
    {
        MApi_XC_MLoad_WriteCmd(dwReg, dwValue, 0xFFFF);
        MApi_XC_MLoad_WriteCmd(dwReg + 2, (0x0000)|( dwValue >> 16 ), 0xFFFF);
    }
}

void msSWDBWriteByteMask(DWORD dwReg,BYTE ucVal,BYTE ucMask)
{
    MApi_XC_MLoad_WriteCmd(dwReg, (0x0000)|(ucVal), (0x0000)|(ucMask));
}


void msSWDBWrite2ByteMask(DWORD dwReg,WORD wVal,WORD wMask)
{
    MApi_XC_MLoad_WriteCmd(dwReg, wVal, wMask);
}

void msSWDBWriteBit(DWORD dwReg,BOOL bBit,BYTE ucBitPos)
{
    XDATA BYTE u8Value;

    u8Value = msReadByte( dwReg );
    if( bBit )
    {
        u8Value = u8Value | ucBitPos;
    }
    else
    {
        u8Value = u8Value & ( ~ucBitPos );
    }
    msSWDBWriteByte( dwReg, u8Value );
}

void msSWDBTrigger(void)
{
    MApi_XC_MLoad_Fire(TRUE);
}
#endif

#if 1 // Old r/w function
DWORD OldRWFuncCnt=0;
BYTE _msReadByte( WORD u16Reg )
{
    u16Reg=u16Reg;
    OldRWFuncCnt++;
    return 0;
}
void _msWriteByte( WORD u16Reg, BYTE u8Val )
{
    u16Reg=u8Val;
    OldRWFuncCnt++;
}

void _msWriteBit( WORD u16Reg, Bool bBit, BYTE u8BitPos )
{
    u16Reg=u8BitPos|bBit;
    OldRWFuncCnt++;
}
//Jison:110428 change _msRead2Byte() u16Reg to low addr.
WORD _msRead2Byte( WORD u8RegLoAddr )
{
    u8RegLoAddr=u8RegLoAddr;
    OldRWFuncCnt++;
    return 0;
}
BYTE _scReadByte( WORD u16Reg )
{
    u16Reg=u16Reg;
    OldRWFuncCnt++;
    return 0;
}
void _scWriteByte( WORD u16Reg, BYTE u8Value )
{
    u16Reg=u8Value;
    OldRWFuncCnt++;
}
//Jison:110428 u16Reg is low addr.
void _msWrite2Byte( WORD u16RegLoAddr, WORD u16Value )
{
    u16RegLoAddr=u16Value;
    OldRWFuncCnt++;
}
void _msWriteByteMask( WORD u16Reg, BYTE u8Value, BYTE u8Mask )
{
    u16Reg=u8Value|u8Mask;
    OldRWFuncCnt++;
}
//Jison:110428 u16Reg is low addr.
void _msWrite2ByteMask( WORD u16RegLoAddr, WORD u16Value , WORD u16Mask )
{
    u16RegLoAddr=u16Value|u16Mask;
    OldRWFuncCnt++;
}
void _msWrite3Byte( WORD u16RegLoAddr, DWORD u32Value )
{
    u32Value=u16RegLoAddr;
    OldRWFuncCnt++;
}
BYTE _MEM_MSREAD_BYTE( WORD u16Reg )
{
    u16Reg=u16Reg;
    OldRWFuncCnt++;
    return 0;
}
BYTE _MEM_SCREAD_BYTE( WORD u16Reg )
{
    u16Reg=u16Reg;
    OldRWFuncCnt++;
    return 0;
}
#endif
#if 0
void mStar_WriteWord( BYTE addr, WORD value )
{
    //static xdata WORD ucCntOldWW = 0;
    value = addr;

    //ucCntOldWW++;
}
#endif
//===============================================================
#elif DDR_BUS
void mStar_Start(BYTE u8Addr)
{
#define DDR_PORT    u8Addr
    SET_DDR_PORT;
    DELAY1US();
    SET_MS_ALE();

    DDR_PORT >>= 4;
    SET_DDR_PORT;
    DELAY1US();
    CLR_MS_ALE();
#undef DDR_PORT
}

void mStar_SendByte(BYTE u8Value)
{
#define DDR_PORT    u8Value
    SET_DDR_PORT;
    DELAY1US();
    CLR_MS_WRZ();

    DDR_PORT >>= 4;
    SET_DDR_PORT;
    DELAY1US();
    SET_MS_WRZ();
#undef DDR_PORT
}

#if 0
BYTE mStar_ReceiveByte(void)
{
    BYTE    u8Value;
    BYTE    ddrPort = 0xF;

    CLR_MS_RDZ();
    SET_DDR_PORT;
    u8Value = (HWMS_DDR_PORT >> HWMS_DDR_SHIFT_BITS) & 0xF;
    SET_MS_RDZ();

    DELAY1US();
    CLR_MS_RDZ();
    SET_DDR_PORT;
    u8Value = u8Value | (((HWMS_DDR_PORT >> HWMS_DDR_SHIFT_BITS) & 0xF) << 4);
    SET_MS_RDZ();

    return u8Value;
}

#else
BYTE mStar_ReceiveByte(void)
{
    BYTE    u8Value;
    BYTE    ddrPort = 0xF;

    SET_DDR_PORT;
    DELAY1US();
    CLR_MS_RDZ();
    u8Value = (HWMS_DDR_PORT >> HWMS_DDR_SHIFT_BITS) & 0xF;
    SET_MS_RDZ();

    DELAY1US();
    SET_DDR_PORT;
    DELAY1US();
    CLR_MS_RDZ();
    u8Value = u8Value | (((HWMS_DDR_PORT >> HWMS_DDR_SHIFT_BITS) & 0xF) << 4);
    SET_MS_RDZ();

    return u8Value;
}
#endif
BYTE mStar_ReadByte(BYTE u8Addr)
{
    mStar_Start(u8Addr);
    return mStar_ReceiveByte();
}

void mStar_WriteByte(BYTE u8Addr, BYTE u8Value)
{
    mStar_Start(u8Addr);
    mStar_SendByte(u8Value);
}

WORD mStar_ReadWord(BYTE u8Addr)
{
    WORD    u16Value;

    u16Value = mStar_ReadByte(u8Addr);
    u16Value = (u16Value << 8) | mStar_ReadByte(u8Addr - 1);

    return u16Value;
}

void mStar_WriteWord(BYTE u8Addr, WORD u16Value)
{
    mStar_WriteByte(u8Addr - 1, u16Value & 0xFF);
    mStar_WriteByte(u8Addr, u16Value >> 8);
}

void mStar_WriteTBL(BYTE u8Addr, BYTE *pu8Tbl, BYTE u8Count, BYTE u8Inc)
{
    u8Inc = u8Inc;
    while (u8Count--)
        mStar_WriteByte(u8Addr, *(pu8Tbl++));
}

#else // !DIRECT_BUS
void mStar_Start(msBus_Direction direct)
{
    SET_MS_CSZ();       // CS=1
    CLR_MS_SCL();       // SCL=0
    if (direct)         // Read
        SET_MS_SDA();   //SDA=1
    else
        CLR_MS_SDA();   //SDA=0
    SET_MS_SCL();       //SCL=1
    CLR_MS_SCL();       //SCL=0
}

void mStar_SendByte(BYTE u8Value, Bool lastOne)
{
    BYTE    i;

    for (i = 0; i < 7; i++)
    {
        (u8Value & BIT7) ? (SET_MS_SDA()) : (CLR_MS_SDA());
        u8Value <<= 1;
        SET_MS_SCL();
        DELAY4US();
        CLR_MS_SCL();
    }

    if (lastOne)
        CLR_MS_CSZ();
    (u8Value & BIT7) ? (SET_MS_SDA()) : (CLR_MS_SDA());
    DELAY4US();
    SET_MS_SCL();
    DELAY4US();
    CLR_MS_SCL();

    if (lastOne)
    {
        SET_MS_SDA();
        SET_MS_SCL();
    }
}

BYTE mStar_ReceiveByte(Bool lastOne)
{
    BYTE    i;
    BYTE    u8Value;

    for (i = 0; i < 7; i++)
    {
        u8Value <<= 1;
        SET_MS_SCL();
        DELAY4US();
        if (IS_MS_SDA_HI())
            u8Value |= BIT0;
        CLR_MS_SCL();
    }

    if (lastOne)
        CLR_MS_CSZ();
    {
        u8Value <<= 1;
        SET_MS_SCL();
        DELAY4US();
        if (IS_MS_SDA_HI())
            u8Value |= BIT0;
        CLR_MS_SCL();
    }

    if (lastOne)
    {
        SET_MS_SDA();
        SET_MS_SCL();
    }

    return u8Value;
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
BYTE mStar_ReadByte(BYTE u8Addr)
{
    mStar_Start(MS_READ);
    mStar_SendByte(u8Addr, 0);
    SET_MS_SDA();

    return mStar_ReceiveByte(1);
}

WORD mStar_ReadWord(BYTE u8Addr)
{
    WORD    u16Value;

    u16Value = mStar_ReadByte(u8Addr);
    u16Value = (u16Value << 8) | mStar_ReadByte(u8Addr - 1);

    return u16Value;
}

void mStar_WriteByte(BYTE u8Addr, BYTE u8Value)
{
    mStar_Start(MS_WRITE);
    mStar_SendByte(u8Addr, 0);
    mStar_SendByte(u8Value, 1);
}

void mStar_WriteWord(BYTE u8Addr, WORD u16Value)
{
    mStar_WriteByte(u8Addr - 1, u16Value & 0xFF);
    mStar_WriteByte(u8Addr, u16Value >> 8);
}

void mStar_SendDDRByte(BYTE u8Value, Bool lastOne)
{
    BYTE    i;

    for (i = 0; i < 7; i++)
    {
        HWMS_SDA_PIN = (u8Value & BIT7);
        u8Value <<= 1;
        HWMS_SCL_PIN = !HWMS_SCL_PIN;
    }

    if (lastOne)
        CLR_MS_CSZ();
    (u8Value & BIT7) ? (SET_MS_SDA()) : (CLR_MS_SDA());
    HWMS_SCL_PIN = !HWMS_SCL_PIN;

    if (lastOne)
    {
        SET_MS_SDA();
        SET_MS_SCL();
    }
}

void mStar_WriteDDRByte(BYTE u8Addr, BYTE u8Value)
{
    CLR_MS_SCL();
    SET_MS_CSZ();   // CS=1
    CLR_MS_SDA();   //SDA=0
    SET_MS_SCL();
    mStar_SendDDRByte(u8Addr, 0);
    mStar_SendDDRByte(u8Value, 1);
}

void mStar_WriteTBL(BYTE u8Addr, BYTE *pu8Tbl, BYTE u8Count, BYTE u8Inc)
{
    if (u8Inc)
        mStar_WriteByte(REGBK, mStar_ReadByte(REGBK) &~AINC);
    else
        mStar_WriteByte(REGBK, mStar_ReadByte(REGBK) | AINC);

    mStar_Start(MS_WRITE);
    mStar_SendByte(u8Addr, 0);
    while (u8Count--)
        mStar_SendByte(*(pu8Tbl++), 0);
    mStar_SendByte(*pu8Tbl, 1);
}


#if 0
void mStar_SwitchRegBank(BYTE bank_index)
{
    BYTE    regValue = mStar_ReadByte(REGBK);
    mStar_WriteByte(REGBK, (regValue & REGBANK_MASK) | bank_index);
}
#endif
#endif



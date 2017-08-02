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

#include <intrins.h>
#include "types.h"
#include "board.h"
#include "ms_reg.h"
#include "misc.h"
#include "ms_rwreg.h"

#define DB_DEBUG    1
#if ENABLE_DEBUG&&DB_DEBUG
#define DB_DEBUG_printData(str, value)   printData(str, value)
#define DB_DEBUG_printMsg(str)           printMsg(str)
#else
#define DB_DEBUG_printData(str, value)
#define DB_DEBUG_printMsg(str)
#endif

WORD idata g_u16RIUReqCnt = 0;

#if ENABLE_SW_DOUBLE_BUFFER
enum
{
    SW_DB_SW_TRIG,
    SW_DB_OVSYNC_TRIG,
    SW_DB_IVSYNC_TRIG,
    SW_DB_OVDE_TRIG,
};

XDATA StuSWDBCtrl SWDBCtrl _at_ XDATA_SWDB_ADDR_START;
void msSWDBInit(void)
{
    SWDBCtrl.ucSWDB_Mode = eDB_NORMAL_MODE;
    SWDBCtrl.ucSWDB_Mode_Log = eDB_NORMAL_MODE;
    SWDBCtrl.ucSWDB_Num = 1;
    SWDBCtrl.ucSWDB_Counter = 0;
    SWDBCtrl.ucSWDB_OnFire = 0;
    SWDBCtrl.ucSWDB_LastSCBank = 0xFF;
    //msWriteByteMask(REG_003C25,SW_DB_OVDE_TRIG<<4,BIT5|BIT4);
    msWriteByteMask(REG_003C25,SW_DB_SW_TRIG<<4,BIT5|BIT4);
    msWrite2ByteMask(REG_003C24,((WORD)&SWDBCtrl.ucSWDB_Buffer[0])&0x7FF,0x07FF);
    msWriteBit(REG_002B08,0,BIT4);  //PM FIQ
    msWriteBit(REG_002B28,0,BIT1);  //PM IRQ
    DB_DEBUG_printMsg("ML initial");
}

void msSWDBWaitForRdy(void)
{
    XDATA BYTE dlycnt=60;
    while(SWDBCtrl.ucSWDB_OnFire)
    {
        dlycnt--;
        if(!dlycnt)
        {
            DB_DEBUG_printMsg("ML Time out, switch to SW Trig Mode !!");
            msWriteByteMask(REG_003C25,SW_DB_SW_TRIG<<4,BIT5|BIT4);
            msWriteByteMask(REG_003C25,BIT6,BIT6);
            while(SWDBCtrl.ucSWDB_OnFire);
            msWriteByteMask(REG_003C25,0x00,0xC0);
            msWriteByteMask(REG_003C25,SW_DB_OVDE_TRIG<<4,BIT5|BIT4);
            SWDBCtrl.ucSWDB_Counter = 0;
            SWDBCtrl.ucSWDB_OnFire = 0;
            break;
        }
        ForceDelay1ms(1);
    }
}

void msSWDBWriteToRegister(void)
{
    XDATA BYTE index;

    msSWDBWaitForRdy();
    if(SWDBCtrl.ucSWDB_Counter)
    {
        index = SWDBCtrl.ucSWDB_Counter;
        msWrite2ByteMask(REG_003C26,((WORD)&SWDBCtrl.ucSWDB_Buffer[index-1])&0x7FF,0x07FF);
        SWDBCtrl.ucSWDB_OnFire = 1;
        msWriteByte(REG_003C25,msReadByte(REG_003C25)|0xC0);
    }
}

void msSWDBMode(DB_TYPE type)
{
    SWDBCtrl.ucSWDB_Mode = type;
}

static BOOL msSWDBNewModeBufChk(WORD wReg)
{
    BOOL NewMode = FALSE;
    XDATA BYTE size = 4;

    if((SWDBCtrl.ucSWDB_Mode_Log != SWDBCtrl.ucSWDB_Mode)
       || (SWDBCtrl.ucSWDB_Counter == 0))
    {
        NewMode = TRUE;
    }
    else
    {
        switch(SWDBCtrl.ucSWDB_Mode)
        {
            case eDB_SUCCESSIVE_MODE:
                if((wReg>>8) != (SWDBCtrl.ucSWDB_Buffer[SWDBCtrl.ucSWDB_Num - 1]&0x3F))
                    NewMode = TRUE;
                break;

            case eDB_BURST_MODE:
                if(((wReg>>8) != (SWDBCtrl.ucSWDB_Buffer[SWDBCtrl.ucSWDB_Num - 1]&0x3F))
                   || ((wReg&0xFF) != (SWDBCtrl.ucSWDB_Buffer[SWDBCtrl.ucSWDB_Num + 1])))
                   NewMode = TRUE;
                break;

            default:
                break;
        }
    }

    // if buffer size is not large enough then write to register first
    if(SWDBCtrl.ucSWDB_Mode == eDB_NORMAL_MODE)
        size = 3;
    else if(SWDBCtrl.ucSWDB_Mode == eDB_SUCCESSIVE_MODE)
        size = NewMode?4:2;
    else if(SWDBCtrl.ucSWDB_Mode == eDB_BURST_MODE)
        size = NewMode?4:1;

    if( (SWDBCtrl.ucSWDB_Counter+size) >= SWDB_MAX_SIZE )
    {
        DB_DEBUG_printMsg("SWDB buffer is Full !!");
        msSWDBWriteToRegister();
        msSWDBWaitForRdy();
        NewMode = TRUE;
    }

    if(NewMode)
        SWDBCtrl.ucSWDB_Num = SWDBCtrl.ucSWDB_Counter + 1;

    return NewMode;
}
void msSWDBWriteByte(WORD wReg,BYTE ucValue)
{
    XDATA BYTE index, SWDBModeChg;
    XDATA BYTE SCBank,SCAddr,SCVal;

    msSWDBWaitForRdy();
    if( wReg&_BIT15 ) // Scaler bank
    {
        SCBank = (wReg>>8)&0x7F;
        SCAddr = (wReg&0xFF);
        SCVal = ucValue;
        if(SWDBCtrl.ucSWDB_LastSCBank!= SCBank)
        {
            SWDBCtrl.ucSWDB_LastSCBank = SCBank;
            msSWDBWriteByte(0x2F00,SCBank);
            msSWDBWriteByte(0x2F00|SCAddr,SCVal);
        }
        else
            msSWDBWriteByte(0x2F00|SCAddr,SCVal);
    }
    else
    {
        SWDBModeChg = msSWDBNewModeBufChk(wReg);
        index  = SWDBCtrl.ucSWDB_Counter;

        switch(SWDBCtrl.ucSWDB_Mode)
        {
            case eDB_NORMAL_MODE:
                SWDBCtrl.ucSWDB_Mode_Log = SWDBCtrl.ucSWDB_Mode;
                SWDBCtrl.ucSWDB_Buffer[index] = (wReg>>8)|eDB_NORMAL_MODE;
                SWDBCtrl.ucSWDB_Buffer[index+1] = (wReg&0xFF);
                SWDBCtrl.ucSWDB_Buffer[index+2] = ucValue;
                SWDBCtrl.ucSWDB_Counter += 3;
                break;

            case eDB_SUCCESSIVE_MODE:
                if(SWDBModeChg)
                {
                    SWDBCtrl.ucSWDB_Mode_Log = SWDBCtrl.ucSWDB_Mode;
                    SWDBCtrl.ucSWDB_Buffer[SWDBCtrl.ucSWDB_Num - 1] = (wReg>>8)|eDB_SUCCESSIVE_MODE;
                    SWDBCtrl.ucSWDB_Buffer[SWDBCtrl.ucSWDB_Num] = 0;
                    SWDBCtrl.ucSWDB_Buffer[index + 2] = (wReg&0xFF);
                    SWDBCtrl.ucSWDB_Buffer[index + 3] = ucValue;
                    SWDBCtrl.ucSWDB_Counter += 4;
                }
                else
                {
                    SWDBCtrl.ucSWDB_Buffer[SWDBCtrl.ucSWDB_Num - 1] = (wReg>>8)|eDB_SUCCESSIVE_MODE;
                    SWDBCtrl.ucSWDB_Buffer[SWDBCtrl.ucSWDB_Num]++;
                    SWDBCtrl.ucSWDB_Buffer[index ] = (wReg&0xFF);
                    SWDBCtrl.ucSWDB_Buffer[index + 1] = ucValue;
                    SWDBCtrl.ucSWDB_Counter += 2;
                }
                break;

            case eDB_BURST_MODE:
                if(SWDBModeChg)
                {
                    SWDBCtrl.ucSWDB_Mode_Log = SWDBCtrl.ucSWDB_Mode;
                    SWDBCtrl.ucSWDB_Buffer[SWDBCtrl.ucSWDB_Num - 1] = (wReg>>8)|eDB_BURST_MODE;
                    SWDBCtrl.ucSWDB_Buffer[SWDBCtrl.ucSWDB_Num] = 0;
                    SWDBCtrl.ucSWDB_Buffer[SWDBCtrl.ucSWDB_Num + 1] = (wReg&0xFF);
                    SWDBCtrl.ucSWDB_Buffer[index + 3] = ucValue;
                    SWDBCtrl.ucSWDB_Counter += 4;
                }
                else
                {
                    SWDBCtrl.ucSWDB_Buffer[SWDBCtrl.ucSWDB_Num - 1] = (wReg>>8)|eDB_BURST_MODE;
                    SWDBCtrl.ucSWDB_Buffer[SWDBCtrl.ucSWDB_Num]++;
                    SWDBCtrl.ucSWDB_Buffer[SWDBCtrl.ucSWDB_Num + 1 ] = (wReg&0xFF);
                    SWDBCtrl.ucSWDB_Buffer[index] = ucValue;
                    SWDBCtrl.ucSWDB_Counter += 1;
                }
                break;
        }
    }
}


void msSWDBWrite2Byte(WORD wReg,WORD wValue)
{
    msSWDBWriteByte(wReg, wValue );
    msSWDBWriteByte(wReg + 1, wValue>>8);
}

void msSWDBWrite3Byte(WORD wReg, DWORD dwValue)
{
    msSWDBWrite2Byte( wReg, dwValue&0xFFFF );
    msSWDBWriteByte( wReg + 2, ( BYTE )( dwValue >> 16 ) );
}

void msSWDBWriteByteMask(WORD wReg,BYTE ucVal,BYTE ucMask)
{
    msSWDBWriteByte(wReg , (msReadByte( wReg ) & ( ~ucMask ) ) | ((ucVal) & (ucMask)) );
}


void msSWDBWrite2ByteMask(WORD wReg,WORD wVal,WORD wMask)
{
    msSWDBWriteByteMask(wReg, (BYTE)wVal, (BYTE)wMask);
    msSWDBWriteByteMask(wReg+1, (BYTE)(wVal>>8), (BYTE)(wMask>>8));
}

void msSWDBWriteBit(WORD wReg,BOOL bBit,BYTE ucBitPos)
{
    XDATA BYTE u8Value;

    u8Value = msReadByte( wReg );
    if( bBit )
    {
        u8Value = u8Value | (ucBitPos);
    }
    else
    {
        u8Value = u8Value & ( ~ucBitPos );
    }
    msSWDBWriteByte( wReg, u8Value );
}
#endif


#define DELAY1US()  _nop_()
//DIRECT_BUS
/////////////////////////////////////////////////////////////////////////
BYTE msReadByte( DWORD u32Reg )
{
    DWORD ucdata = 0;

    if((g_bCorePowerStatus==0) && ((u32Reg&0x00FF0000)))
    {
        DB_DEBUG_printMsg("Error >> Illegal NonPM Domain Access !!!!!");
        return 0;
    }

    P2 = (u32Reg & 0x00FF0000)>>16; // Bank switch for 51 (P2 need bk in ISR)
    if( u32Reg & 0x80000000 ) // Sub bank
    {
        msRegs[u32Reg & 0x0000FF00] = ( u32Reg >> 24 ) & 0x7F;
    }
    ucdata = msRegs[u32Reg & 0x0000FFFF];
    P2 = 0; // added for 51 switch code bank via xfr, need move to .a51??
    return ucdata;

}
void msWriteByte( DWORD u32Reg, BYTE u8Val )
{
    if((g_bCorePowerStatus==0) && ((u32Reg&0x00FF0000)))
    {
        DB_DEBUG_printMsg("Error >> Illegal NonPM Domain Access !!!!!");
        return;
    }

    P2 = (u32Reg & 0x00FF0000)>>16; // Bank switch for 51 (P2 need bk in ISR)
    if( u32Reg & 0x80000000 ) // Sub bank
    {
        msRegs[u32Reg & 0x0000FF00] = ( u32Reg >> 24 ) & 0x7F;
        msRegs[u32Reg & 0x0000FFFF] = u8Val;
    }
    else
        msRegs[u32Reg & 0x0000FFFF] = u8Val;
    P2 = 0; // added for 51 switch code bank via xfr, need move to .a51??
}

void msWriteBit( DWORD u32Reg, Bool bBit, BYTE u8BitPos )
{
    BYTE u8Value;

    ReqstRIU();
    u8Value = msReadByte( u32Reg );
    if( bBit )
    {
        u8Value = u8Value | (u8BitPos);
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


#if 0
BYTE scReadByte( BYTE u8Reg )
{
    return scRegs[u8Reg];
}

void scWriteByte( BYTE u8Reg, BYTE u8Value )
{
    scRegs[u8Reg] = u8Value;
}
#endif

//Jison:110428 u16Reg is low addr.
void msWrite2Byte( DWORD u32RegLoAddr, WORD u16Value )
{
    msWriteByte( u32RegLoAddr, ( BYTE )u16Value );
    msWriteByte( u32RegLoAddr + 1, ( BYTE )( u16Value >> 8 ) );
}
void msWriteByteMask( DWORD u32Reg, BYTE u8Value, BYTE u8Mask )
{
    ReqstRIU();
    msWriteByte( u32Reg, ( (msReadByte( u32Reg ) & ( ~u8Mask )) | ( (u8Value) & (u8Mask) ) ) );
    RelesRIU();
}

//Jison:110428 u16Reg is low addr.
void msWrite2ByteMask( DWORD u32RegLoAddr, WORD u16Value , WORD u16Mask )
{
    msWriteByteMask( u32RegLoAddr, ( BYTE )u16Value, ( BYTE ) u16Mask );
    msWriteByteMask( u32RegLoAddr + 1, ( BYTE )( u16Value >> 8 ), ( BYTE )( u16Mask >> 8 ) );
}

void msWrite3Byte( DWORD u32RegLoAddr, DWORD u32Value )
{
    msWriteByte( u32RegLoAddr, ( BYTE )u32Value );
    msWriteByte( u32RegLoAddr + 1, ( BYTE )( u32Value >> 8 ) );
    msWriteByte( u32RegLoAddr + 2, ( BYTE )( u32Value >> 16 ) );
}

void msWrite4Byte( DWORD u32RegLoAddr, DWORD u32Value )
{
    msWriteByte( u32RegLoAddr, ( BYTE )u32Value );
    msWriteByte( u32RegLoAddr + 1, ( BYTE )( u32Value >> 8 ) );
    msWriteByte( u32RegLoAddr + 2, ( BYTE )( u32Value >> 16 ) );
    msWriteByte( u32RegLoAddr + 3, ( BYTE )( u32Value >> 24 ) );
}


BYTE MEM_MSREAD_BYTE( DWORD u32Reg )  // Need Change Sub Bank Manually if necessary, for performanace consideration in ISR !!!!
{
    BYTE u8Value;
    P2 = (u32Reg & 0x00FF0000)>>16; // Bank switch for 51 (P2 need bk in ISR)
#if 1 // Need Change Sub Bank Manually if necessary, for performanace consideration in ISR !!!!
    u8Value = msRegs[u32Reg & 0x0000FFFF];
    P2 = 0; // added for 51 switch code bank via xfr, need move to .a51??
    return u8Value;
#else
    if( u32Reg & 0x80000000 ) // Sub bank
    {
        msRegs[u32Reg & 0x0000FF00] = ( u32Reg >> 24 ) & 0x7F;
    }
    u8Value = msRegs[u32Reg & 0x0000FFFF];
    P2 = 0; // added for 51 switch code bank via xfr, need move to .a51??
    return u8Value;
#endif
}
BYTE MEM_MSREAD_BYTE_EXT0(DWORD u32Reg)
{
    BYTE u8Value;
    P2 = (u32Reg & 0x00FF0000)>>16; // Bank switch for 51 (P2 need bk in ISR)
    u8Value = msRegs[u32Reg & 0x0000FFFF];
    P2 = 0; // added for 51 switch code bank via xfr, need move to .a51??
    return u8Value;
}
BYTE MEM_MSREAD_BYTE_EXT1(DWORD u32Reg)
{
    BYTE u8Value;
    P2 = (u32Reg & 0x00FF0000)>>16; // Bank switch for 51 (P2 need bk in ISR)
    u8Value = msRegs[u32Reg & 0x0000FFFF];
    P2 = 0; // added for 51 switch code bank via xfr, need move to .a51??
    return u8Value;
}

#if _NEW_SOG_WAKEUP_DET_
BYTE MEM_MSREAD_BYTE_TIME0(DWORD u32Reg)
{
    BYTE u8Value;
    P2 = (u32Reg & 0x00FF0000)>>16; // Bank switch for 51 (P2 need bk in ISR)
    u8Value = msRegs[u32Reg & 0x0000FFFF];
    P2 = 0; // added for 51 switch code bank via xfr, need move to .a51??
    return u8Value;
}
#endif


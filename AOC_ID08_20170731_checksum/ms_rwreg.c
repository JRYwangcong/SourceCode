#include <intrins.h>
#include "types.h"
#include "board.h"
#include "ms_reg.h"
#include "misc.h"
#include "debug.h"
extern void Delay4us(void);
#define Delay1us()	_nop_()//;_nop_()
#if DirectBus
    #if MST_MCU
        //xdata BYTE regio[256] _at_ 0x0000;
        #define regio ((BYTE xdata*)0x0000)
    #else 
        #define MS_REG(addr)		(((unsigned char xdata*)0)[addr])
    #endif 
    /////////////////////////////////////////////////////////////////////////
    BYTE mStar_ReadByte(BYTE addr)
    {
        #if MST_MCU
            return regio[addr];
        #else 
            return MS_REG(addr);
        #endif 
    }
    WORD mStar_ReadWord(BYTE addr)
    {
        WORD value;
        value = mStar_ReadByte(addr);
        value = (value << 8) | mStar_ReadByte(addr - 1);
        return value;
    }
    void mStar_WriteByte(BYTE addr, BYTE value)
    {
        #if MST_MCU
            regio[addr] = value;
        #else 
            MS_REG(addr) = value;
        #endif 
    }
    void mStar_WriteWord(BYTE addr, WORD value)
    {
        mStar_WriteByte(addr - 1, value &0xFF);
        mStar_WriteByte(addr, value >> 8);
    }
    void mStar_WriteTBL(BYTE addr, BYTE *tbl, WORD count, BYTE inc)
    {
        while (count--)
        {
            mStar_WriteByte(addr, *(tbl++));
        }
        inc = 0;
    }
#elif DDRBus
    void mStar_Start(BYTE addr)
    {
        #define ddrPort addr
        SetDDRPort;
        Delay1us();
        Set_ms_ALE();
        ddrPort >>= 4;
        SetDDRPort;
        Delay1us();
        Clr_ms_ALE();
        #undef ddrPort
    }
    void mStar_SendByte(BYTE value)
    {
        #define ddrPort value
        SetDDRPort;
        Delay1us();
        Clr_ms_WRZ();
        ddrPort >>= 4;
        SetDDRPort;
        Delay1us();
        Set_ms_WRZ();
        #undef ddrPort
    }
    #if ChipTSU
        BYTE mStar_ReceiveByte(void)
        {
            BYTE value;
            BYTE ddrPort = 0xF;
            SetDDRPort;
            Delay1us();
            Clr_ms_RDZ();
            value = (hwMS_DDRPort >> hwMS_DDRShiftBits) &0xF;
            Set_ms_RDZ();
            Delay1us();
            SetDDRPort;
            Delay1us();
            Clr_ms_RDZ();
            value = value | (((hwMS_DDRPort >> hwMS_DDRShiftBits) &0xF) << 4);
            Set_ms_RDZ();
            return value;
        }
    #else 
        BYTE mStar_ReceiveByte(void)
        {
            BYTE value;
            BYTE ddrPort = 0xF;
            Clr_ms_RDZ();
            SetDDRPort;
            value = (hwMS_DDRPort >> hwMS_DDRShiftBits) &0xF;
            Set_ms_RDZ();
            Delay1us();
            Clr_ms_RDZ();
            SetDDRPort;
            value = value | (((hwMS_DDRPort >> hwMS_DDRShiftBits) &0xF) << 4);
            Set_ms_RDZ();
            return value;
        }
    #endif 
    BYTE mStar_ReadByte(BYTE addr)
    {
        mStar_Start(addr);
        return mStar_ReceiveByte();
    }
    void mStar_WriteByte(BYTE addr, BYTE value)
    {
        mStar_Start(addr);
        mStar_SendByte(value);
    }
    WORD mStar_ReadWord(BYTE addr)
    {
        WORD value;
        value = mStar_ReadByte(addr);
        //  mStar_WriteByte(addr, value);
        //  mStar_Start(addr);
        value = (value << 8) | mStar_ReadByte(addr - 1);
        return value;
    }
    void mStar_WriteWord(BYTE addr, WORD value)
    {
        mStar_WriteByte(addr - 1, value &0xFF);
        mStar_WriteByte(addr, value >> 8);
    }
    void mStar_WriteTBL(BYTE addr, BYTE *tbl, WORD count, BYTE inc)
    {
        inc = inc;
        while (count--)
            mStar_WriteByte(addr, *(tbl++));
    }
#else 
    void mStar_Start(msBus_Direction direct)
    {
        Set_ms_CSZ(); // CS=1
        Clr_ms_SCL(); // SCL=0
        if (direct)
        // Read
            Set_ms_SDA();
        //SDA=1
        else
            Clr_ms_SDA();
        //SDA=0
        Set_ms_SCL(); //SCL=1
        Clr_ms_SCL(); //SCL=0
    }
    void mStar_SendByte(BYTE value, Bool lastOne)
    {
        BYTE i;
        for (i = 0; i < 7; i++)
        {
            (value &BIT7) ? (Set_ms_SDA()): (Clr_ms_SDA());
            value <<= 1;
            Set_ms_SCL();
            Delay4us();
            Clr_ms_SCL();
        }
        if (lastOne)
            Clr_ms_CSZ();
        (value &BIT7) ? (Set_ms_SDA()): (Clr_ms_SDA());
        Delay4us();
        Set_ms_SCL();
        Delay4us();
        Clr_ms_SCL();
        if (lastOne)
        {
            Set_ms_SDA();
            Set_ms_SCL();
        }
    }
    BYTE mStar_ReceiveByte(Bool lastOne)
    {
        BYTE i;
        BYTE value;
        for (i = 0; i < 7; i++)
        {
            value <<= 1;
            Set_ms_SCL();
            Delay4us();
            if (ms_SDAHi())
                value |= BIT0;
            Clr_ms_SCL();
        }
        if (lastOne)
            Clr_ms_CSZ();
        {
            value <<= 1;
            Set_ms_SCL();
            Delay4us();
            if (ms_SDAHi())
                value |= BIT0;
            Clr_ms_SCL();
        }
        if (lastOne)
        {
            Set_ms_SDA();
            Set_ms_SCL();
        }
        return value;
    }
    /////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////
    BYTE mStar_ReadByte(BYTE addr)
    {
        mStar_Start(MS_READ);
        mStar_SendByte(addr, 0);
        Set_ms_SDA();
        return mStar_ReceiveByte(1);
    }
    WORD mStar_ReadWord(BYTE addr)
    {
        WORD value;
        value = mStar_ReadByte(addr);
        value = (value << 8) | mStar_ReadByte(addr - 1);
        return value;
    }
    void mStar_WriteByte(BYTE addr, BYTE value)
    {
        mStar_Start(MS_WRITE);
        mStar_SendByte(addr, 0);
        mStar_SendByte(value, 1);
    }
    void mStar_WriteWord(BYTE addr, WORD value)
    {
        mStar_WriteByte(addr - 1, value &0xFF);
        mStar_WriteByte(addr, value >> 8);
    }
    //=======================================================================
    // Only for chip after D-version
    #if ChipID>=ChipAD
        void mStar_SendDDRByte(BYTE value, Bool lastOne)
        {
            BYTE i;
            for (i = 0; i < 7; i++)
            {
                hwMS_SDA_Pin = (value &BIT7);
                value <<= 1;
                hwMS_SCL_Pin = !hwMS_SCL_Pin;
            }
            if (lastOne)
                Clr_ms_CSZ();
            (value &BIT7) ? (Set_ms_SDA()): (Clr_ms_SDA());
            hwMS_SCL_Pin = !hwMS_SCL_Pin;
            if (lastOne)
            {
                Set_ms_SDA();
                Set_ms_SCL();
            }
        }
        void mStar_WriteDDRByte(BYTE addr, BYTE value)
        {
            Clr_ms_SCL();
            Set_ms_CSZ(); // CS=1
            Clr_ms_SDA(); //SDA=0
            Set_ms_SCL();
            mStar_SendDDRByte(addr, 0);
            mStar_SendDDRByte(value, 1);
        }
    #endif 
    //=======================================================================
    void mStar_WriteTBL(BYTE addr, BYTE *tbl, WORD count, BYTE inc)
    {
        if (inc)
            mStar_WriteByte(REGBK, mStar_ReadByte(REGBK) &~AINC);
        else
            mStar_WriteByte(REGBK, mStar_ReadByte(REGBK) | AINC);
        mStar_Start(MS_WRITE);
        mStar_SendByte(addr, 0);
        while (count--)
            mStar_SendByte(*(tbl++), 0);
        mStar_SendByte(*tbl, 1);
    }
#endif

void msWriteByteMask(BYTE ucReg, BYTE ucValue, BYTE ucMask)
{
	BYTE uc;

	uc = mStar_ReadByte(ucReg) & (~ucMask);
	mStar_WriteByte( ucReg, ( uc | (ucValue & ucMask)) );
}
#if (defined TSUMXXT)||(defined TSUMXXNX)	//110311 Modify
void mStar_WriteByteMask(BYTE addr, BYTE value, BYTE mask)
{
    BYTE ucTemp;

    value &= mask;
    ucTemp = regio[addr] & (~mask);
    regio[addr] = value|ucTemp;
}
#endif

///////////////////////////////////////////////////////////////////////////////
/// @file drvScalerGPIO.c
/// @brief Scaler GPIO/PWM access
/// @author MStarSemi Inc.
///
/// Driver for Scaler GPIO/PWM access.
///
/// Features
///  -
///  -
///////////////////////////////////////////////////////////////////////////////
#define _DRVGPIO_C

#include "Types.h"
#include "Board.h"
#include "Global.h"
#include "Ms_reg.h"
#include "Ms_rwreg.h"
//#include "DebugMsg.h"
//#include "MDebug.h"
#include "drvGPIO.h"

//MST_MODULE(drvGPIO)
#define GPIO_DEBUG    1
#if ENABLE_DEBUG&&GPIO_DEBUG
    #define GPIO_printData(str, value)   printData(str, value)
    #define GPIO_printMsg(str)           printMsg(str)
#else
    #define GPIO_printData(str, value)
    #define GPIO_printMsg(str)
#endif
//PWM driver
WORD msDrvPWMMaskTransfer(DWORD u32Mask)
{
    if(u32Mask&(_PWM0_GP0_|_PWM0_GP1_))
        return _PWM0_;
    else if(u32Mask&(_PWM1_GP0_|_PWM1_GP1_))
        return _PWM1_;
    else if(u32Mask&(_PWM2_GP0_|_PWM2_GP1_))
        return _PWM2_;
    else if(u32Mask&(_PWM3_GP0_|_PWM3_GP1_))
        return _PWM3_;
    else if(u32Mask&(_PWM4_GP0_|_PWM4_GP1_))
        return _PWM4_;
    else if(u32Mask&(_PWM5_GP0_|_PWM5_GP1_))
        return _PWM5_;
    else if(u32Mask&(_PWM6_GP0_|_PWM6_GP1_))
        return _PWM6_;
    else if(u32Mask&(_PWM7_GP0_|_PWM7_GP1_))
        return _PWM7_;
    else if(u32Mask&(_PWM8_GP0_|_PWM8_GP1_))
        return _PWM8_;
    else if(u32Mask&(_PWM9_GP0_|_PWM9_GP1_))
        return _PWM9_;
    else if(u32Mask&(_DISP_PWM0_GP0_|_DISP_PWM0_GP1_))
        return _DISP_PWM0_;
    else if(u32Mask&(_DISP_PWM1_GP0_|_DISP_PWM1_GP1_))
        return _DISP_PWM1_;
    else if(u32Mask&(_DISP_PWM2_GP0_|_DISP_PWM2_GP1_))
        return _DISP_PWM2_;
    else if(u32Mask&(_DISP_PWM3_GP0_|_DISP_PWM3_GP1_))
        return _DISP_PWM3_;
    else if(u32Mask&(_DISP_PWM4_GP0_|_DISP_PWM4_GP1_))
        return _DISP_PWM4_;
    else if(u32Mask&(_DISP_PWM5_GP0_|_DISP_PWM5_GP1_))
        return _DISP_PWM5_;
    else
        return _NOUSE_;
}
DWORD msDrvPWMPeriodGet(DWORD u32PWMCh)
{
    DWORD u32Period;
    WORD u16PWMCh = msDrvPWMMaskTransfer(u32PWMCh);

    if(u16PWMCh<_DISP_PWM0_||u16PWMCh==_NOUSE_)
        return 0;

    //read period bit[15:0]
    u32Period = msRead2Byte(REG_103D04+6*(u16PWMCh-_DISP_PWM0_));
    //read period bit[17:16]
    u32Period|= ((((DWORD)(msRead2Byte(REG_103D40))>>(2*(u16PWMCh-_DISP_PWM0_)))&0x03)<<16);
    return u32Period;
}

DWORD msDrvPWMShiftGet(DWORD u32PWMCh)
{
    WORD u16PWMCh = msDrvPWMMaskTransfer(u32PWMCh);
    DWORD u32PWMShift = 0;

    if(u16PWMCh<_DISP_PWM0_||u16PWMCh==_NOUSE_)
        return 0;

    u32PWMShift = msRead2Byte(REG_103D50+4*(u16PWMCh-_DISP_PWM0_));
    u32PWMShift|= (msReadByte(REG_103D52+4*(u16PWMCh-_DISP_PWM0_))&0x03)<<16;
    return u32PWMShift;
}

void msDrvPWMShift(DWORD u32PWMCh, DWORD u32PWMShift)
{
    WORD u16PWMCh = msDrvPWMMaskTransfer(u32PWMCh);

    if(u16PWMCh<_DISP_PWM0_||u16PWMCh==_NOUSE_)
        return;

    msWrite2Byte(REG_103D50+4*(u16PWMCh-_DISP_PWM0_), u32PWMShift);
    msWriteByteMask(REG_103D52+4*(u16PWMCh-_DISP_PWM0_), (u32PWMShift>>16), BIT0|BIT1);
}

void msDrvPWMAlignSync(DWORD u32PWMCh,BYTE u8Config, BYTE u8Counter)
{
    WORD u16PWMCh = msDrvPWMMaskTransfer(u32PWMCh);
    Bool bVEnable = u8Config&BIT2;
    Bool bHEnable = u8Config&BIT3;

    if(u16PWMCh >= _NOUSE_)
        return;
    if(bVEnable && bHEnable)
    {
        GPIO_printMsg("Both Hsync and Vsync align are enable, please redefine config");
        return;
    }
    if(u16PWMCh >= _DISP_PWM0_)     //DISP PWM
    {
        msWriteBit(REG_103D09+6*(u16PWMCh-_DISP_PWM0_), bVEnable|bHEnable, BIT2);     //reset enable

        if(u8Counter == 0&&bVEnable)        //vsync align
        {
            if(u16PWMCh % 2)
                msWriteBit(REG_103D28+(u16PWMCh-_DISP_PWM1_), 0, BIT7);
            else
                msWriteBit(REG_103D29+(u16PWMCh-_DISP_PWM0_), 0, BIT7);

            if(msDrvPWMShiftGet(u32PWMCh)==0)
                msDrvPWMShift(u32PWMCh,1);
        }
        else        //vsync align with counter
        {
            if(u16PWMCh % 2)
            {
                msWriteBit(REG_103D28+(u16PWMCh-_DISP_PWM1_), bVEnable|bHEnable, BIT7);
                msWriteByteMask(REG_103D28+(u16PWMCh-_DISP_PWM1_), u8Counter, 0x0F);
            }
            else
            {
                msWriteBit(REG_103D29+(u16PWMCh-_DISP_PWM0_), bVEnable|bHEnable, BIT7);
                msWriteByteMask(REG_103D29+(u16PWMCh-_DISP_PWM0_), u8Counter, 0x0F);
            }
            msWriteBit(REG_103D68, bVEnable, TransNumToBit(u16PWMCh-_DISP_PWM0_));
        }
    }
    else        //PWM
    {
        if(u16PWMCh < _PWM4_)
        {
            if(bVEnable | bHEnable)
                msWriteBit(REG_000583+(u16PWMCh<<1), 1, (bVEnable? BIT3: BIT4));
            else
                msWriteBit(REG_000583+(u16PWMCh<<1), 0, BIT3|BIT4);
        }
        else
        {
            if(bVEnable | bHEnable)
                msWriteBit(REG_00059D+((u16PWMCh-_PWM3_)<<1), 1, (bVEnable? BIT3: BIT4));
            else
                msWriteBit(REG_00059D+((u16PWMCh-_PWM3_)<<1), 0, BIT3|BIT4);
        }
    }
}
void msDrvPWMPolarity(DWORD u32PWMCh, BYTE u8Config)
{
    WORD u16PWMCh = msDrvPWMMaskTransfer(u32PWMCh);
    Bool bEnable = u8Config&BIT4;

    if(u16PWMCh<_DISP_PWM0_||u16PWMCh==_NOUSE_)
        return;

    msWriteBit(REG_103D09+6*(u16PWMCh-_DISP_PWM0_), bEnable, BIT0);
}

void msDrvPWMDuty(DWORD u32PWMCh,DWORD u32Duty)
{
    WORD u16PWMCh = msDrvPWMMaskTransfer(u32PWMCh);

    if(u16PWMCh >= _NOUSE_)
        return;
    if(u16PWMCh >= _DISP_PWM0_)
    {
        msWrite2Byte(REG_103D06+6*(u16PWMCh-_DISP_PWM0_), u32Duty);
        msWriteByteMask(REG_103D42, (u32Duty>>16)<<((u16PWMCh-_DISP_PWM0_)*2), (0x03)<<((u16PWMCh-_DISP_PWM0_)*2));
    }
    else if(u16PWMCh < _PWM4_)
    {
        msWriteByte(REG_000590+(u16PWMCh), (BYTE)u32Duty);
    }
    else
    {
        msWriteByte(REG_0005AA+(u16PWMCh-_PWM4_), (BYTE)u32Duty);
    }
}

void msDrvPWMFreq(DWORD u32PWMCh,DWORD u32PWMFreq)
{ //****keep divider is default 0****
    WORD u16PWMCh = msDrvPWMMaskTransfer(u32PWMCh);
    DWORD u32PWMPeriod;

    if( u16PWMCh == _NOUSE_ )
        return;


    //PWM DIV
    if(u16PWMCh>=_DISP_PWM0_)
    {
        u32PWMPeriod = (u32PWMFreq?(((unsigned long)XTAL_CLOCK_KHZ*1000/u32PWMFreq)):0);
        if (u32PWMPeriod<0x40000)
        {
            msWrite2Byte(REG_103D04+6*(u16PWMCh-_DISP_PWM0_),u32PWMPeriod);
            msWrite2ByteMask(REG_103D40,(u32PWMPeriod>>16)<<((u16PWMCh-_DISP_PWM0_)*2),(0x03)<<((u16PWMCh-_DISP_PWM0_)*2));
        }
    }
    else
    {
        u32PWMPeriod = (u32PWMFreq?(((unsigned long)XTAL_CLOCK_KHZ*1000/((u32PWMFreq+1)*256))):0);
        if (u32PWMPeriod<0x800)
        {
            if(u16PWMCh<_PWM3_)
                msWrite2ByteMask(REG_000582+2*(u16PWMCh-_PWM0_),u32PWMPeriod,0x7FF);
            else
                msWrite2ByteMask(REG_00059C+2*(u16PWMCh-_PWM3_),u32PWMPeriod,0x7FF);
        }
    }

}

void msAPIPWMDuty(DWORD u32PWMCh,WORD u16ByteDuty)
{
    DWORD u32Temp=msDrvPWMPeriodGet(u32PWMCh);
    WORD u16PWMCh = msDrvPWMMaskTransfer(u32PWMCh);
    //printData("u16ByteDuty=%d",u16ByteDuty);
    if( u16PWMCh == _NOUSE_ )
        return;

    if(u16PWMCh>=_DISP_PWM0_)
        u32Temp = LINEAR_MAPPING_VALUE(u16ByteDuty, 0, 0xFFFF, 0, u32Temp);
    else
        u32Temp = u16ByteDuty>>8;

        //u32Temp=LINEAR_MAPPING_VALUE(u16ByteDuty, 0, 0xFFFF, 0, 0xFF);
    //printData("Duty hi=%xh",u32Temp>>16);
    //printData("Duty lo=%xh",(WORD)u32Temp);
    msDrvPWMDuty(u32PWMCh, u32Temp);
}

void msDrvPWMDoubleBuffer(DWORD u32PWMCh, BYTE u8Config)
{
    WORD u16PWMCh = msDrvPWMMaskTransfer(u32PWMCh);
    Bool bEnable = u8Config & _PWM_DB_EN_;
    Bool bDBVsyncAlign = u8Config & _PWM_DB_VSYNC_MODE_;

    if( u16PWMCh == _NOUSE_ )
        return;
    if(u16PWMCh>=_DISP_PWM0_)
    {
        if(bEnable)
        {
            msWriteBit(REG_103D09+6*(u16PWMCh-_DISP_PWM0_),bDBVsyncAlign,BIT1);
            msWriteBit(REG_103D09+6*(u16PWMCh-_DISP_PWM0_),!bDBVsyncAlign,BIT3);
            msWriteBit(REG_103D09+6*(u16PWMCh-_DISP_PWM0_),bDBVsyncAlign,BIT6);
        }
        else
        {
            msWriteBit(REG_103D09+6*(u16PWMCh-_DISP_PWM0_),FALSE,BIT1);
            msWriteBit(REG_103D09+6*(u16PWMCh-_DISP_PWM0_),FALSE,BIT3);
            msWriteBit(REG_103D09+6*(u16PWMCh-_DISP_PWM0_),FALSE,BIT6);
        }
    }
    else
    {
        if(u16PWMCh<_PWM3_)
            msWriteBit(REG_000583+2*u16PWMCh,bEnable,BIT5);
        else
            msWriteBit(REG_00059D+2*(u16PWMCh-_PWM3_),bEnable,BIT5);
    }

}

void msAPIPWMConfig(DWORD u32PWMCh, WORD u16Freq, WORD u16Duty, BYTE u8Config, BYTE u8Counter)
{
    msDrvPWMFreq(u32PWMCh,u16Freq);
    msAPIPWMDuty(u32PWMCh,u16Duty);
    msDrvPWMAlignSync(u32PWMCh,u8Config,u8Counter);
    msDrvPWMDoubleBuffer(u32PWMCh,u8Config);
    msDrvPWMPolarity(u32PWMCh,u8Config);
}

void msDrvPWMEnableSwitch(DWORD u32PWMCh, Bool bEnable)
{
    if(u32PWMCh & (_PWM0_GP0_|_PWM0_GP1_))
        msWriteBit(REG_000580, bEnable, BIT0);
    else if(u32PWMCh & (_PWM1_GP0_|_PWM1_GP1_))
        msWriteBit(REG_000580, bEnable, BIT1);
    else if(u32PWMCh & (_PWM2_GP0_|_PWM2_GP1_))
        msWriteBit(REG_000580, bEnable, BIT2);
    else if(u32PWMCh & (_PWM3_GP0_|_PWM3_GP1_))
        msWriteBit(REG_000580, bEnable, BIT7);
    else if(u32PWMCh & (_PWM4_GP0_|_PWM4_GP1_))
        msWriteBit(REG_000581, bEnable, BIT0);
    else if(u32PWMCh & (_PWM5_GP0_|_PWM5_GP1_))
        msWriteBit(REG_000581, bEnable, BIT1);
    else if(u32PWMCh & (_PWM6_GP0_|_PWM6_GP1_))
        msWriteBit(REG_000581, bEnable, BIT2);
    else if(u32PWMCh & (_PWM7_GP0_|_PWM7_GP1_))
        msWriteBit(REG_000581, bEnable, BIT3);
    else if(u32PWMCh & (_PWM8_GP0_|_PWM8_GP1_))
        msWriteBit(REG_000581, bEnable, BIT4);
    else if(u32PWMCh & (_PWM9_GP0_|_PWM9_GP1_))
        msWriteBit(REG_000581, bEnable, BIT5);
}
void drvGPIO_dummy(void)
{
    msAPIPWMConfig(0,0,0,0,0);
}


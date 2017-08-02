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

#include "datatype.h"
#include "Board.h"
#include "Global.h"
#include "ms_reg.h"
#include "Ms_rwreg.h"
#include "MDebug.h"
#include "drvGPIO.h"

//#define GPIO_DEBUG    1  // move to drvGPIO.h
#if ENABLE_MSTV_UART_DEBUG && GPIO_DEBUG
#define GPIO_PRINT(format, ...)     printf(format, ##__VA_ARGS__)
#else
#define GPIO_PRINT(format, ...)
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
        GPIO_PRINT("Both Hsync and Vsync align are enable, please redefine config \r\n");
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
        else if(bHEnable)        //vsync align
        {	
        	DWORD u32PWMFreq;
        	DWORD u32ModHFreq;

        	u32PWMFreq = (unsigned long)XTAL_CLOCK_KHZ*1000/msDrvPWMPeriodGet(u32PWMCh);
		//switch MODpin to TTL mode,to check HV freq only
		#if 0
        	msWrite2Byte(REG_MOD1_34, 0xFFFF);
        	msWrite2Byte(REG_MOD1_20, 0x0000);
        	msWrite2Byte(REG_MOD1_22, 0x0000);
		#endif
			
        	u32ModHFreq=PanelVTotal*GetVfreq();
        	//GPIO_PRINT(" mode hsync freq ===%d \r\n",u32ModHFreq);
        	//GPIO_PRINT(" PWM freq ===%d \r\n",u32PWMFreq);

        	if (u32PWMFreq>=(u8Counter*u32ModHFreq))
        	{
	            if(u16PWMCh % 2)
	            {
	                msWriteBit(REG_103D28+(u16PWMCh-_DISP_PWM1_), 1, BIT7);
	                msWriteByteMask(REG_103D28, u8Counter,0x0F);
	            }
	            else
	            {
	                msWriteBit(REG_103D29+(u16PWMCh-_DISP_PWM0_), 1, BIT7);
	                msWriteByteMask(REG_103D29, u8Counter,0x0F);
	            }
	            if(msDrvPWMShiftGet(u32PWMCh)==0)
	                msDrvPWMShift(u32PWMCh,1);
        	}
        	else
        	{
	                msWriteBit(REG_103D28+(u16PWMCh-_DISP_PWM1_), 0, BIT7);
	                msWriteBit(REG_103D29+(u16PWMCh-_DISP_PWM0_), 0, BIT7);
        	}
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
        msWrite2ByteMask(REG_103D42, (u32Duty>>16)<<((u16PWMCh-_DISP_PWM0_)*2), (0x03)<<((u16PWMCh-_DISP_PWM0_)*2));
        //GPIO_PRINT("msDrvPWMDuty              u32Duty====%x,REG_103D06==%x\r\n\r\n",u32Duty   ,msRead2Byte(REG_103D06+6*(u16PWMCh-_DISP_PWM0_)));
        //GPIO_PRINT("msDrvPWMDuty              (u32Duty>>16)<<((u16PWMCh-_DISP_PWM0_)*2)==%x\r\n\r\n"   ,(u32Duty>>16)<<((u16PWMCh-_DISP_PWM0_)*2));
        //GPIO_PRINT("msDrvPWMDuty              (0x03)<<((u16PWMCh-_DISP_PWM0_)*2)==%x\r\n\r\n"   ,(0x03)<<((u16PWMCh-_DISP_PWM0_)*2));
    }
    else if(u16PWMCh < _PWM4_)
    {
        msWriteByte(REG_000590+(u16PWMCh), (BYTE)u32Duty);
        //GPIO_PRINT("duty_Add====%x,u32Duty==%x\r\n",REG_000590+(u16PWMCh),u32Duty);
    }
    else
    {
        msWriteByte(REG_0005AA+(u16PWMCh-_PWM4_), (BYTE)u32Duty);
        //GPIO_PRINT("duty_Add====%x,u32Duty==%x\r\n",REG_0005AA+(u16PWMCh-_PWM4_),u32Duty);
    }
}

void msDrvPWMFreq(DWORD u32PWMCh,DWORD u32PWMFreq)
{ 
    //_PWM0_GP0_ ~_PWM9_GP1_ :XTAL/((N+1)*256) NOTE:N=0~2048	 Freq:27Hz~55.9KHz
    //_DISP_PWM0_GP0_ ~_DISP_PWM5_GP1_ :	 Freq:~60~6M

    WORD u16PWMCh = msDrvPWMMaskTransfer(u32PWMCh);
    DWORD u32PWMPeriod;
    BYTE u8CheckEnableDB;

    if( u16PWMCh == _NOUSE_ )
        return;
	
    //GPIO_PRINT("\r\n\r\n\r\nu32PWMFreq==%x\r\n",u32PWMFreq);

    //PWM DIV
    if(u16PWMCh>=_DISP_PWM0_)
    {
        u32PWMPeriod = (u32PWMFreq?(((unsigned long)XTAL_CLOCK_KHZ*1000/u32PWMFreq)):0);
        //GPIO_PRINT(" u32PWMPeriod==%x     \r\n",u32PWMPeriod);
		
        if (u32PWMPeriod<0x40000)
        {
            msWrite2Byte(REG_103D04+6*(u16PWMCh-_DISP_PWM0_),u32PWMPeriod);
            msWrite2ByteMask(REG_103D40,(u32PWMPeriod>>16)<<((u16PWMCh-_DISP_PWM0_)*2),(0x03)<<((u16PWMCh-_DISP_PWM0_)*2));
			
            //GPIO_PRINT("\r\n\r\n\r\(0x03)<<((u16PWMCh-_DISP_PWM0_)*2)==%x\r\n",(0x03)<<((u16PWMCh-_DISP_PWM0_)*2));
            //GPIO_PRINT("\r\n\r\n\r\(u32PWMPeriod>>16)<<((u16PWMCh-_DISP_PWM0_)*2)==%x\r\n",(u32PWMPeriod>>16)<<((u16PWMCh-_DISP_PWM0_)*2));
        }
        #if 1 //if enable DB of disp_pwm, the duty isnt correct if we set duty after setting frequency immediately.
        u8CheckEnableDB=msReadByte(REG_103D09+6*(u16PWMCh-_DISP_PWM0_));
        if (u8CheckEnableDB & BIT3)
        {
            ForceDelay1ms(50);
        }
        #endif
    }
    else
    {
				
		//u32PWMPeriod = (u32PWMFreq?(((unsigned long)XTAL_CLOCK_KHZ*1000/(u32PWMFreq*256))-1):0);
              u32PWMPeriod = (u32PWMFreq?(((unsigned long)XTAL_CLOCK_KHZ*1000/((u32PWMFreq+1)*256))):0);
		
		if (u32PWMPeriod<0x800)
        {
            if(u16PWMCh<_PWM3_)
            {
                //msWrite2ByteMask(REG_000582+2*(u16PWMCh-_PWM0_),u32PWMPeriod,0x7FF);
		  msWrite2Byte(REG_000582+2*(u16PWMCh-_PWM0_),(u32PWMPeriod&0x7FF));	
                GPIO_PRINT("u32PWMPeriod 333=%x\n\r",msRead2Byte(REG_000582+2*(u16PWMCh-_PWM0_))&0x7FF);
            }
            else
            {
                //msWrite2ByteMask(REG_00059C+2*(u16PWMCh-_PWM3_),u32PWMPeriod,0x7FF);
  		  msWrite2Byte(REG_00059C+2*(u16PWMCh-_PWM3_),(u32PWMPeriod&0x7FF));	
              GPIO_PRINT("u32PWMPeriod=%x\n\r",msRead2Byte(REG_00059C+(2*(u16PWMCh-_PWM3_)))&0x7FF);
            }
        }
    }
}

void msAPIPWMDuty(DWORD u32PWMCh,WORD u16ByteDuty)
{
    DWORD u32Temp=msDrvPWMPeriodGet(u32PWMCh);
    WORD u16PWMCh = msDrvPWMMaskTransfer(u32PWMCh);
    DWORD u32RealDutyMapToRegSet=0;

    //GPIO_PRINT("u16ByteDuty=%x\r\n",u16ByteDuty);
    //GPIO_PRINT("getPreiod=%x\r\n",u32Temp);
    //GPIO_PRINT("u32RealDutyMapToRegSet=%x\r\n",u32RealDutyMapToRegSet);
	
    if( u16PWMCh == _NOUSE_ )
        return;

    if(u16PWMCh>=_DISP_PWM0_)
        u32RealDutyMapToRegSet=LINEAR_MAPPING_VALUE(u16ByteDuty, 0, 0xFF, 0, u32Temp);  //input pwm duty 0~0xFF , extend to 0~0x3FFFF;;
    else
        u32RealDutyMapToRegSet=u16ByteDuty;
    
    //GPIO_PRINT("@@@@@@@@@ u32Temp==%x\r\n",u32RealDutyMapToRegSet);
    msDrvPWMDuty(u32PWMCh, u32RealDutyMapToRegSet);
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
        {
            msWriteBit(REG_000583+2*u16PWMCh,bEnable,BIT5);
            //GPIO_PRINT("DoubleBuffer_ADD==%x\r\n",REG_000583+2*u16PWMCh);
        }
        else
        {
            msWriteBit(REG_00059D+2*(u16PWMCh-_PWM3_),bEnable,BIT5);
            //GPIO_PRINT("DoubleBuffer_ADD==%x\r\n",REG_00059D+2*(u16PWMCh-_PWM3_));
        }
    }
}

void msAPIPWMConfig(DWORD u32PWMCh, DWORD u32Freq, WORD u16Duty, BYTE u8Config, BYTE u8Counter)
{	
    //enable double buffer before setting duty to avoid incorrected duty after adjust pwm frequency.
    msDrvPWMDoubleBuffer(u32PWMCh,u8Config);
    msDrvPWMFreq(u32PWMCh,u32Freq);
    msAPIPWMDuty(u32PWMCh,u16Duty);
    msDrvPWMAlignSync(u32PWMCh,u8Config,u8Counter);
    msDrvPWMPolarity(u32PWMCh,u8Config);
}

void msDrvPWMEnableSwitch(DWORD u32PWMCh, Bool bEnable)//para1:_PWM0_GP0_~_DISP_PWM5_GP1_
{
    //this function will not disable previous selected PWM
    BYTE u8RegShift;
	
    if(u32PWMCh & (_PWM0_GP0_|_PWM0_GP1_))
        msWriteBit(REG_000580, bEnable, BIT0);
    if(u32PWMCh & (_PWM1_GP0_|_PWM1_GP1_))
        msWriteBit(REG_000580, bEnable, BIT1);
    if(u32PWMCh & (_PWM2_GP0_|_PWM2_GP1_))
        msWriteBit(REG_000580, bEnable, BIT2);
    if(u32PWMCh & (_PWM3_GP0_|_PWM3_GP1_))
        msWriteBit(REG_000580, bEnable, BIT7);
    if(u32PWMCh & (_PWM4_GP0_|_PWM4_GP1_))
        msWriteBit(REG_000581, bEnable, BIT0);
    if(u32PWMCh & (_PWM5_GP0_|_PWM5_GP1_))
        msWriteBit(REG_000581, bEnable, BIT1);
    if(u32PWMCh & (_PWM6_GP0_|_PWM6_GP1_))
        msWriteBit(REG_000581, bEnable, BIT2);
    if(u32PWMCh & (_PWM7_GP0_|_PWM7_GP1_))
        msWriteBit(REG_000581, bEnable, BIT3);
    if(u32PWMCh & (_PWM8_GP0_|_PWM8_GP1_))
        msWriteBit(REG_000581, bEnable, BIT4);
    if(u32PWMCh & (_PWM9_GP0_|_PWM9_GP1_))
        msWriteBit(REG_000581, bEnable, BIT5);
	
    for(u8RegShift=0;u8RegShift<2;u8RegShift++)     //PWM pad control
    {
#if 1    
        if(BACKLIGHT_PWM_CONFIG & _PWM_OD_EN_)//open drain
            msWrite2ByteMask(REG_00040A+4*u8RegShift,u32PWMCh>>10*u8RegShift,((u32PWMCh)>>10*u8RegShift)&0x3FF);
        else
            msWrite2ByteMask(REG_000408+4*u8RegShift,u32PWMCh>>10*u8RegShift,((u32PWMCh)>>10*u8RegShift)&0x3FF);
		msWriteByteMask(REG_000418+u8RegShift,(bEnable ? ((u32PWMCh)>>(20+6*u8RegShift)):0),((u32PWMCh)>>(20+6*u8RegShift))&0x3F);
#else	
	msWrite2ByteMask(REG_000408+4*u8RegShift,(u32PWMCh)>>10*u8RegShift,((u32PWMCh)>>10*u8RegShift)&0x3FF);
	msWriteByteMask(REG_000418+u8RegShift,(u32PWMCh)>>(20+6*u8RegShift),((u32PWMCh)>>(20+6*u8RegShift))&0x3F);
#endif	
	//msWrite2ByteMask(REG_000408+4*u8RegShift,(u32PWMCh)>>10*u8RegShift,0x3FF);
	//msWriteByteMask(REG_000418+u8RegShift,(u32PWMCh)>>(20+6*u8RegShift),0x3F);
	//printData("111111111111 (u32PWMCh)>>10*u8RegShift====%x\r\n", (u32PWMCh)>>10*u8RegShift);
	//printData("222222222222 (u32PWMCh)>>(20+6*u8RegShift)====%x\r\n", (u32PWMCh)>>(20+6*u8RegShift));
    }
}

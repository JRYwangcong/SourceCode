#define _MSMINILVDS_C_
#include "types.h"
#include "board.h"
#include "global.h"
#include "i2cdef.h"

#include "debug.h"
#include "misc.h"
#include "panel.h"


#if PANEL_VCOM_ADJUST		//121031 Modify

#define I2C_CHECK	1
#if I2C_CHECK
#define I2C_CHECK_PIN_DUMMY 100

void msMiniLVDSi2cSCL_Chk(void)
{
    BYTE ucDummy = I2C_CHECK_PIN_DUMMY; // initialize dummy
    while (HW_GET_VCOM_I2C_SCL_Lo()&&(ucDummy--))
        ; // check SCL pull high
}

void msMiniLVDSi2cSDA_Chk(void)
{  
    BYTE ucDummy = I2C_CHECK_PIN_DUMMY; // initialize dummy
    while (HW_GET_VCOM_I2C_SDA_Lo()&&(ucDummy--))
        ; // check SCL pull high
}
#endif


Bool Wait_Bus_Release(void)
{
    BYTE Counter=100;
    
    do
    {
        Counter--;
        Delay4us();
    }
    
    while((HW_GET_VCOM_I2C_SDA_HI()) && (Counter >0) );
    
    if(HW_GET_VCOM_I2C_SDA_Lo())                     
        return TRUE;
    
    return FALSE;           
}

Bool msMiniLVDSi2c_Start(void)
{   
    HW_SET_VCOM_I2C_SDA();
    #if I2C_CHECK
    msMiniLVDSi2cSDA_Chk();
    #endif
    Delay4us();
    HW_SET_VCOM_I2C_SCL();
    #if I2C_CHECK
    msMiniLVDSi2cSCL_Chk();//ywb++
    #endif
    Delay4us();
    if (HW_GET_VCOM_I2C_SDA_Lo() || HW_GET_VCOM_I2C_SCL_Lo())
        return FALSE;
    HW_CLR_VCOM_I2C_SDA();
    Delay4us();
    HW_CLR_VCOM_I2C_SCL();
    
    return TRUE;
}

void msMiniLVDSi2c_Stop(void)
{
    HW_CLR_VCOM_I2C_SCL();
    Delay4us();
    HW_CLR_VCOM_I2C_SDA();
    Delay4us();

    HW_SET_VCOM_I2C_SCL();
    Delay4us();
    HW_SET_VCOM_I2C_SDA();
}

Bool msMiniLVDSi2c_SendByte(BYTE value)
{   
    BYTE i;
    Bool result;

    for (i=0; i<8; i++)
    {   
        if (value&BIT7)
        {
            HW_SET_VCOM_I2C_SDA();
            #if I2C_CHECK
            msMiniLVDSi2cSDA_Chk();
            #endif
        }
        else
            HW_CLR_VCOM_I2C_SDA();
        Delay4us();
        Delay4us();
        Delay4us();
        HW_SET_VCOM_I2C_SCL();
        Delay4us();	//111118 Modify for LNT request
        value<<=1;
        HW_CLR_VCOM_I2C_SCL();
    }
    HW_SET_VCOM_I2C_SDA();
    Wait_Bus_Release();
    Delay4us();
    HW_SET_VCOM_I2C_SCL();
    #if I2C_CHECK
    msMiniLVDSi2cSCL_Chk();
    #endif

    result=HW_GET_VCOM_I2C_SDA_Lo();

    HW_CLR_VCOM_I2C_SCL();
    Delay4us();
    HW_CLR_VCOM_I2C_SDA();

    return result;
}


BYTE msMiniLVDSi2c_ReceiveByte(const Bool ack)
{   
    BYTE i;
    BYTE value=0;

    for (i=0; i<8; i++)
    {   
        value<<=1;
        HW_SET_VCOM_I2C_SDA();
        #if I2C_CHECK
        msMiniLVDSi2cSDA_Chk();
        #endif
        HW_SET_VCOM_I2C_SCL();
        #if I2C_CHECK
        msMiniLVDSi2cSCL_Chk();
        #endif
        Delay4us();
        Delay4us();
        Delay4us();
        if (HW_GET_VCOM_I2C_SDA_HI())
            value|=BIT0;
        HW_CLR_VCOM_I2C_SCL();
    }

    if (ack)
        HW_CLR_VCOM_I2C_SDA();
    else
    {
        HW_SET_VCOM_I2C_SDA();
        #if I2C_CHECK
        msMiniLVDSi2cSDA_Chk();
        #endif
    }
    Delay4us();

    HW_SET_VCOM_I2C_SCL();
    Delay4us();
    HW_CLR_VCOM_I2C_SCL();

    return value;
}

/*
Bool msMiniLVDSi2c_BurstWrite(BYTE count, BYTE *buffer)
{   
    while (count--)
    { 
        if (msMiniLVDSi2c_SendByte(*(buffer++))==FALSE)
            return FALSE;
    }
    
    return TRUE;
}

Bool msMiniLVDSi2c_BurstRead(BYTE count, BYTE * buffer)
{   
    BYTE i;

    for (i=0; i<count-1; i++)
        *(buffer+i)=msMiniLVDSi2c_ReceiveByte(1);
    *(buffer+i)=msMiniLVDSi2c_ReceiveByte(0);

    msMiniLVDSi2c_Stop();

    return TRUE;
}
*/
Bool msMiniLVDSi2c_MasterStart(I2C_Direction direct, BYTE addr)
{ 	
    BYTE retry=5;

    if (direct==I2C_READ)
        addr|=BIT0;
    else
    	addr&=~BIT0;

    while (retry--)
    { 	
        if (msMiniLVDSi2c_Start()==FALSE)
            continue;

        if (msMiniLVDSi2c_SendByte(addr)==TRUE)
            return TRUE;

        msMiniLVDSi2c_Stop();
        ForceDelay1ms(1);		//Delay1ms(1);
    }

    return FALSE;
}

#if VCOM_TYPE == VCOM_MAXIM		//120322 Modify
Bool msMiniLVDSi2c_Write2Byte(BYTE add, WORD val)
{
#if 0
    BYTE XDATA tempbuf;
    tempbuf[0] = (BYTE)(val>>8);
    tempbuf[1] = (BYTE)val;
#else
    BYTE XDATA tempbuf1, tempbuf2;
    tempbuf1 = (BYTE)(val>>8);
    tempbuf2 = (BYTE)val;
#endif
    if (msMiniLVDSi2c_MasterStart(I2C_WRITE, MINILVDSTCON_DEVICE) == FALSE)
        return FALSE;
#if 1//0
    if (msMiniLVDSi2c_SendByte(add) == FALSE)
    {
        msMiniLVDSi2c_Stop();
        return FALSE;
    }

    if (msMiniLVDSi2c_SendByte(tempbuf1) == FALSE)
    {
        msMiniLVDSi2c_Stop();
        return FALSE;
    }
    if (msMiniLVDSi2c_SendByte(tempbuf2) == FALSE)
    {
        msMiniLVDSi2c_Stop();
        return FALSE;
    }

    msMiniLVDSi2c_Stop();

    ForceDelay1ms(50); //yywang // PACKET2PACKET_PEROID     
#endif
    return TRUE;
}

Bool msMiniLVDSi2c_Read2Byte( BYTE add, WORD *val )
{
    if (msMiniLVDSi2c_MasterStart(I2C_WRITE, MINILVDSTCON_DEVICE) == FALSE)
        return FALSE;
    if (msMiniLVDSi2c_SendByte(add) == FALSE)
    {
        msMiniLVDSi2c_Stop();
        return FALSE;
    }
    
    if (msMiniLVDSi2c_MasterStart(I2C_READ, MINILVDSTCON_DEVICE) == FALSE)
    {
        msMiniLVDSi2c_Stop();
        return FALSE;
    }

    *val= ((msMiniLVDSi2c_ReceiveByte(1)<<8)+msMiniLVDSi2c_ReceiveByte(0));
    
    msMiniLVDSi2c_Stop();
    return TRUE;
}

WORD msReadMiniLVDSi2c_Vcom(void)
{
	WORD ucVcom=0;

    msMiniLVDSi2c_Read2Byte(0x12, &ucVcom);
  
    return ucVcom;
}
Bool msWriteMiniLVDSi2c_Vcom(WORD ucVcom)
{    
    if (msMiniLVDSi2c_Write2Byte(0x12, BIT14 | ucVcom) == TRUE)
        return TRUE;
    
    return FALSE;
}
Bool msWriteMiniLVDSi2c_Vcom_DAC(WORD ucVcom)	//111103 Modify
{    
    if(msMiniLVDSi2c_Write2Byte(0x12, BIT15 | ucVcom) == TRUE)
        return TRUE;
    
    return FALSE;
}
#if 0
WORD msReadMiniLVDSi2c_Vcom_DAC(void)
{
  WORD ucVcom=0;
  
    msMiniLVDSi2c_Read2Byte(0x12, &ucVcom);
    
    return ucVcom;
}
#endif
#else
Bool msMiniLVDSi2c_WriteByte(BYTE add, BYTE val)
{
    if (msMiniLVDSi2c_MasterStart(I2C_WRITE, MINILVDSTCON_DEVICE) == FALSE)
        return FALSE;

    if (msMiniLVDSi2c_SendByte(add) == FALSE)
    {
        msMiniLVDSi2c_Stop();
        return FALSE;
    }

    if (msMiniLVDSi2c_SendByte(val) == FALSE)
    {
        msMiniLVDSi2c_Stop();
        return FALSE;
    }

    msMiniLVDSi2c_Stop();

    ForceDelay1ms(50); //yywang 

    return TRUE;
}

Bool msMiniLVDSi2c_ReadByte( BYTE add, BYTE *val )
{
    if (msMiniLVDSi2c_MasterStart(I2C_WRITE, MINILVDSTCON_DEVICE) == FALSE)
        return FALSE;

    if (msMiniLVDSi2c_SendByte(add) == FALSE)
    {
        msMiniLVDSi2c_Stop();
        return FALSE;
    }

    if (msMiniLVDSi2c_MasterStart(I2C_READ, MINILVDSTCON_DEVICE) == FALSE)
    {
        msMiniLVDSi2c_Stop();
        return FALSE;
    }

    *val = msMiniLVDSi2c_ReceiveByte(0);
    
    msMiniLVDSi2c_Stop();
    
    return TRUE;
}


BYTE msReadMiniLVDSi2c_Vcom(void)
{
  BYTE ucVcom=0;
  
    msMiniLVDSi2c_WriteByte(0x02, 0x00);

    msMiniLVDSi2c_ReadByte(0x00, &ucVcom);

    return ucVcom;
}

Bool msWriteMiniLVDSi2c_Vcom(BYTE ucVcom)
{    
    msMiniLVDSi2c_WriteByte(0x02, 0x00);
    msMiniLVDSi2c_WriteByte(0x00, ucVcom);
    
    return TRUE;
}
Bool msWriteMiniLVDSi2c_Vcom_DAC(BYTE ucVcom)
{    
    msMiniLVDSi2c_WriteByte(0x02, 0x80);
    msMiniLVDSi2c_WriteByte(0x00, ucVcom);
    
    return TRUE;
}
#endif
#endif

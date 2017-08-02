#include "Types.h"
#include "Board.h"
//#include "MDebug.h"
#include "Common.h"
#include "misc.h"
#include "Global.h"
#include "ms_reg.h"
#include "Ms_rwreg.h"
#include "hwi2c.h"


#define HW_I2C_SPEED     400000ul
#define SYSTEM_CLOCK     12000000ul
#define HIGH_COUNT      ((SYSTEM_CLOCK/HW_I2C_SPEED)*6/10)
#define LOW_COUNT       ((SYSTEM_CLOCK/HW_I2C_SPEED)*4/10)
#define START_COUNT     (LOW_COUNT>>2)
#define STOP_COUNT      (LOW_COUNT>>2)

#define SDA_COUNT         4
#define LATCH_COUNT     2

#define WAIT_READY()    (msReadByte(HWI2C_REG_BANK+0x08) & _BIT0) 
#define CLEARFLAG()      msWriteByte(HWI2C_REG_BANK+0x08, 0x00) 
#define CHECK_NONACK()    (msReadByte(HWI2C_REG_BANK+0x05) & _BIT0) 
#if (HW_I2C_SPEED > 400000ul)
#warning "Hw I2c speek must under 400kHz !"
#endif

#if (USE_SW_I2C == 0)
void i2C_Intial(void)
{
    // Config. PAD mux
    msWriteByteMask(REG_000405, MIIC0_ENABLE, MIIC0_ENABLE_MASK);
    msWriteByteMask(REG_000405, MIIC1_ENABLE, MIIC1_ENABLE_MASK);    
    
    // Init I2C speed
    msWrite2Byte(HWI2C_HIGH_CNT_REG, HIGH_COUNT );
    msWrite2Byte(HWI2C_LOW_CNT_REG, LOW_COUNT);
    msWrite2Byte(HWI2C_START_CNT_REG, START_COUNT);
    msWrite2Byte(HWI2C_STOP_CNT_REG, STOP_COUNT);
    msWrite2Byte(HWI2C_SDA_CNT_REG, SDA_COUNT);
    msWrite2Byte(HWI2C_LATCH_CNT_REG, LATCH_COUNT);    
    //Reset 
    //msWriteByte(HWI2C_CONFIG_REG, 0x21);
    //msWriteByte(HWI2C_CONFIG_REG, 0x20);
    msWriteByteMask(HWI2C_CONFIG_REG, BIT6|BIT5|BIT0	, BIT6|BIT5|BIT0);     //Enable reg_oen_push_en
    msWriteByteMask(HWI2C_CONFIG_REG, BIT6|BIT5		, BIT6|BIT5|BIT0);    
	
    //Delay1ms(2);    
}


#define BREAK_TIME          5 //ms
#define GET_MS_COUNT() ((ms_Counter < BREAK_TIME)?(ms_Counter+ SystemTick):(ms_Counter))

BOOL i2c_Start(void)
{
    WORD Break_count;    

    //Trigger Start
    msWriteByte(HWI2C_REG_BANK +0x02, 0x01);
    Break_count =  GET_MS_COUNT()-BREAK_TIME;    
    while(WAIT_READY() == FALSE)
    {
        if(Break_count == ms_Counter)
            break;
    }
    CLEARFLAG();
    return TRUE;
}

void i2c_Stop(void)
{
    WORD Break_count;

    //Trigger Stop
    msWriteByte(HWI2C_REG_BANK +0x03, 0x01);
    Break_count =  GET_MS_COUNT()-BREAK_TIME;    
    while(WAIT_READY() == FALSE)
    {
        if(Break_count == ms_Counter)
            break;
    }
    CLEARFLAG();
}


bit i2c_SendByte(BYTE ucWriteData)  
{
    bit ucACK = TRUE;
    WORD Break_count;
    
    msWriteByte(HWI2C_WDATA_REG, ucWriteData);
    Break_count =  GET_MS_COUNT()-BREAK_TIME;    
    while(WAIT_READY() == FALSE)
    {
        if(Break_count == ms_Counter)
            break;
    }
    if(CHECK_NONACK())
        ucACK = FALSE;
     
    CLEARFLAG();

    return ucACK;
}

BYTE i2c_ReceiveByte(BOOL bAck)
{
    BYTE ucReceiveByte;
    WORD Break_count;
    
    if(bAck)
        msWriteByte(HWI2C_REG_BANK +0x07, 0x01);
    else
        msWriteByte(HWI2C_REG_BANK +0x07, 0x03);
        
    Break_count =  GET_MS_COUNT() - BREAK_TIME;    
    while(WAIT_READY() == FALSE)
    {
        if(Break_count == ms_Counter)
            break;
    }
    CLEARFLAG();
    ucReceiveByte = msReadByte(HWI2C_RDATA_REG);
    
    return TRUE;
}

#endif



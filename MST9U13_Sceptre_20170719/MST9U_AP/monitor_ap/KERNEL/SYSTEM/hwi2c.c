#define _HWI2C_C_
#include "datatype.h"
#include "Board.h"
#include "MDebug.h"
#include "Common.h"
#include "misc.h"
#include "Global.h"
#include "ms_reg.h"
#include "Ms_rwreg.h"
#include "hwi2c.h"
#include "assert.h"

/* These are definitions related to I2C spped */
#define HW_I2C_SPEED    400000ul // initila speed: 100KHz
#define SYSTEM_CLOCK    12000000ul
#define HIGH_COUNT      ((SYSTEM_CLOCK/HW_I2C_SPEED)*6/10)
#define LOW_COUNT       ((SYSTEM_CLOCK/HW_I2C_SPEED)*4/10)
#define START_COUNT     (LOW_COUNT>>2)
#define STOP_COUNT      (LOW_COUNT>>2)
#define SDA_COUNT       4
#define LATCH_COUNT     2

/* These are definitions of timeout counters for preventing infinite while loop */
#define BREAK_TIME      5           // unit: ms
//#define CHECK_ACK_TIME  25          // CHECK_ACK_TIME *4us
#define GET_MS_COUNT() ((ms_Counter < BREAK_TIME)?(ms_Counter+ SystemTick):(ms_Counter))

/*
 * The states of master I2C.
 *
 * After sending stop bit, the state machine must go into IDLE STATE before
 * sending the start bit of next command. Similarly, the state machine must
 * go into WAITING_STATE after sending start bit or a byte.
 */
#define IDLE_STATE      0x00
#define WAITING_STATE   0x0C

#if (HW_I2C_SPEED > 400000ul)
#warning "The initial speek of master I2C must under 400kHz !"
#endif

#define HWI2C_DEBUG    1
#if ENABLE_MSTV_UART_DEBUG&&HWI2C_DEBUG
    #define HWI2C_PRINT(format, ...)     printf(format, ##__VA_ARGS__)
#else
    #define HWI2C_PRINT(format, ...)
#endif

struct error_info
{
    const int value;
    const char* const name;
    const char* const msg;
};

#define ENTRY(value, name, msg) {value, name, msg}
const struct error_info i2c_error_table[] =
{
    ENTRY(0, NULL, NULL),
    ENTRY(EMIICSTART, "EMIICSTART", "Fail to send start bit"),
    ENTRY(EMIICSTOP, "EMIICSTOP", "Fail to send stop bit"),
    ENTRY(EMIICSTATE, "EMIICSTATE", "Master I2C state machine error"),
    ENTRY(EMIICSB, "EMIICSB", "Fail to send a byte"),
    ENTRY(EMIICRB, "EMIICRB", "Fail to receive a byte"),
    ENTRY(EMIICACK, "EMIICACK", "No acknowledgement"),
};

void hw_i2c_intial(BYTE i2c_dev)
{
    assert((i2c_dev == MIIC0) || (i2c_dev == MIIC1));

    /* Configure PAD mux */
    if(i2c_dev == MIIC0)
        msWriteByteMask(REG_000405, MIIC0_ENABLE, MIIC0_ENABLE_MASK);
    else
        msWriteByteMask(REG_000405, MIIC1_ENABLE, MIIC1_ENABLE_MASK);

    /* Initial I2C speed */
    msWrite2Byte(HWI2C_HIGH_CNT_REG(i2c_dev), HIGH_COUNT);
    msWrite2Byte(HWI2C_LOW_CNT_REG(i2c_dev), LOW_COUNT);
    msWrite2Byte(HWI2C_START_CNT_REG(i2c_dev), START_COUNT);
    msWrite2Byte(HWI2C_STOP_CNT_REG(i2c_dev), STOP_COUNT);
    msWrite2Byte(HWI2C_SDA_CNT_REG(i2c_dev), SDA_COUNT);
    msWrite2Byte(HWI2C_LATCH_CNT_REG(i2c_dev), LATCH_COUNT);

    /* Reset I2C */
    //msWriteByte(HWI2C_CONFIG_REG(i2c_dev), 0x21);
    //msWriteByte(HWI2C_CONFIG_REG(i2c_dev), 0x20);
    msWriteByteMask(HWI2C_CONFIG_REG(i2c_dev), BIT6|BIT5|BIT0   , BIT6|BIT5|BIT0);     //Enable reg_oen_push_en
    msWriteByteMask(HWI2C_CONFIG_REG(i2c_dev), BIT6|BIT5        , BIT6|BIT5|BIT0);

}

BYTE hw_i2c_start(BYTE i2c_dev)
{
    WORD break_count;

    /* Trigger start bit */
    STARTBIT(i2c_dev);
    break_count =  GET_MS_COUNT() - BREAK_TIME;
    while(WAIT_READY(i2c_dev) == FALSE)
    {
        if(break_count == ms_Counter)
            return EMIICSTART;
    }
    CLEARFLAG(i2c_dev);

    /* Wait I2C state machine going to WAITING_STATE */
    break_count = GET_MS_COUNT() - BREAK_TIME;
    while(MIIC_STATE(i2c_dev) != WAITING_STATE)
    {
        if(break_count == ms_Counter)
            return EMIICSTATE;
    }

    return 0;
}

BYTE hw_i2c_stop(BYTE i2c_dev)
{
    WORD break_count;

    /* Trigger stop bit */
    STOPBIT(i2c_dev);
    break_count =  GET_MS_COUNT() - BREAK_TIME;
    while(WAIT_READY(i2c_dev) == FALSE)
    {
        if(break_count == ms_Counter)
            return EMIICSTOP;
    }
    CLEARFLAG(i2c_dev);

    /* Wait IIC state machine going to IDLE_STATE */
    break_count = GET_MS_COUNT() - BREAK_TIME;
    while(MIIC_STATE(i2c_dev) != IDLE_STATE)
    {
        if(break_count == ms_Counter)
            return EMIICSTATE;
    }

    return 0;
}

BYTE hw_i2c_send_byte(BYTE i2c_dev, BYTE send_data)
{
    BYTE ack = 0;
    WORD break_count;
    //use GPIO25 to verify item4.1 (Clock stretchting mechanism)
    //MEM_MSWRITE_BIT(_REG_GPIO2_OUT, BIT5, BIT5);
    msWriteByte(HWI2C_WDATA_REG(i2c_dev), send_data);
    //MsOS_DelayTaskUs(50);//check every 6us
    //MEM_MSWRITE_BIT(_REG_GPIO2_OUT, 0, BIT5);
    //MsOS_DelayTaskUs(100);//check every 6us
    //MEM_MSWRITE_BIT(_REG_GPIO2_OUT, BIT5, BIT5);
    break_count =  GET_MS_COUNT() - BREAK_TIME;
    while(WAIT_READY(i2c_dev) == FALSE)
    {
        if(break_count == ms_Counter)
            return EMIICSB;
    }
    if(CHECK_NONACK(i2c_dev))
        ack = EMIICACK;

    CLEARFLAG(i2c_dev);

    /* Wait IIC state machine going to WAITING_STATE */
    break_count = GET_MS_COUNT() - BREAK_TIME;
    while(MIIC_STATE(i2c_dev) != WAITING_STATE)
    {
        if(break_count == ms_Counter)
             return EMIICSTATE;
    }

    return ack;
}

BYTE hw_i2c_receive_byte(BYTE i2c_dev, BOOL ack_for_rev, BYTE *rev_buf)
{
    WORD break_count;

    assert(rev_buf != NULL);

    if(ack_for_rev)
        ACK_REV(i2c_dev);
    else
        NACK_REV(i2c_dev);

    break_count =  GET_MS_COUNT() - BREAK_TIME;
    while(WAIT_READY(i2c_dev) == FALSE)
    {
        if(break_count == ms_Counter)
            return EMIICRB;
    }
    CLEARFLAG(i2c_dev);
    *rev_buf = msReadByte(HWI2C_RDATA_REG(i2c_dev));

    return 0;
}

BOOL hw_i2c_burst_write(BYTE u8IICIndex, WORD count, BYTE *buffer)
{
    while(count--)
    {
        if(hw_i2c_send_byte(u8IICIndex, *(buffer++)) != 0)
            return FALSE;
    }
    return TRUE;
}

BOOL hw_i2c_burst_read(BYTE u8IICIndex, BYTE count, BYTE * buffer)
{
    BYTE i, result;

    for(i = 0; i < count - 1; i++)
        result = hw_i2c_receive_byte(u8IICIndex, TRUE, (buffer + i));
    result = hw_i2c_receive_byte(u8IICIndex, FALSE, (buffer + i));

    if (result != 0)
        return FALSE;

    return TRUE;
}

/*
 * These functions (i2C_Intial, i2c_Start, i2c_Stop, i2c_SendByte, i2c_ReceiveByte)
 * share the same names with software I2C functions in i2c.c. They are mainly used
 * for read/write EEROM through master I2C 1 (MIIC1).
 */
#if (USE_SW_I2C == 0)
void i2C_Intial(void)
{
   hw_i2c_intial(USE_MIIC);
}

BOOL i2c_Start(void)
{
    BYTE result;

    if ((result = hw_i2c_start(USE_MIIC)) > 0)
    {
        HWI2C_PRINT("%s\n", i2c_error_table[result].msg);
        return FALSE;
    }

    return TRUE;
}

void i2c_Stop(void)
{
    BYTE result;

    if ((result = hw_i2c_stop(USE_MIIC)) > 0)
        HWI2C_PRINT("%s\n", i2c_error_table[result].msg);
}

BOOL i2c_SendByte(BYTE ucWriteData)
{
    BYTE result;

    if ((result = hw_i2c_send_byte(USE_MIIC, ucWriteData)) > 0)
    {
        HWI2C_PRINT("%s\n", i2c_error_table[result].msg);
        return FALSE;
    }

    return TRUE;
}

BYTE i2c_ReceiveByte(BOOL bAck)
{
    BYTE ucReceiveByte = 0, result;

    if ((result = hw_i2c_receive_byte(USE_MIIC, bAck, &ucReceiveByte)) > 0)
        HWI2C_PRINT("%s\n", i2c_error_table[result].msg);

    return ucReceiveByte;
}
#endif              /* End of  (USE_SW_I2C == 0) */

#undef _HWI2C_C_


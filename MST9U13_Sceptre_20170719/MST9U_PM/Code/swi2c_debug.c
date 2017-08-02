/*
 * Copyright (c) 2014 MStar Semiconductor, Inc.
 * All rights reserved.
 *
 * MST9U_PM/Code/swi2c_debug.c
 *
 * Date:        05-Feb-2014
 * Comment:
 *       Software I2C debug mode subroutines.
 *
 * Note:
 *		
 * Reversion History:
 */

#define _SWI2C_DEBUG_C_

#include "board.h"

#if ENABLE_DAISY_CHAIN

#include "Ms_rwreg.h"
#include "ms_reg.h"
#include "misc.h"
#include "Common.h"
#include "types.h"

#define SWI2C_DBMODE_DEBUG    0

#if ENABLE_DEBUG && SWI2C_DBMODE_DEBUG
#define SWI2C_PRINT_DATA(str, value)	printData(str, value)
#define SWI2C_PRINT_MSG(str) 			printMsg(str)
#else
#define SWI2C_PRINT_DATA(str, value)  	
#define SWI2C_PRINT_MSG(str)	 			
#endif

#define set_sw_i2c_scl()  		(MEM_MSWRITE_BIT(_REG_GPIO0_OEZ, TRUE, BIT4))
#define clr_sw_i2c_scl()   		(MEM_MSWRITE_BIT(_REG_GPIO0_OUT, FALSE, BIT4), MEM_MSWRITE_BIT(_REG_GPIO0_OEZ, FALSE, BIT4))
#define sw_i2c_scl_high()   	(_bit4_(MEM_MSREAD_BYTE(_REG_GPIO0_IN)))
#define sw_i2c_scl_low()   		(!sw_i2c_scl_high())
#define set_sw_i2c_sda()  		(MEM_MSWRITE_BIT(_REG_GPIO0_OEZ, TRUE, BIT5))
#define clr_sw_i2c_sda()   		(MEM_MSWRITE_BIT(_REG_GPIO0_OUT, FALSE, BIT5), MEM_MSWRITE_BIT(_REG_GPIO0_OEZ, FALSE, BIT5))
#define sw_i2c_sda_high()  		(_bit5_(MEM_MSREAD_BYTE(_REG_GPIO0_IN)))
#define sw_i2c_sda_low()       	(!sw_i2c_sda_high())

static BOOL sw_i2c_start( void )
{
    set_sw_i2c_sda();
    set_sw_i2c_scl();
    Delay4us();
    if( sw_i2c_sda_low() || sw_i2c_scl_low() )
        return FALSE;
    clr_sw_i2c_sda();
    Delay4us();
    clr_sw_i2c_scl();
    Delay4us();
    return TRUE;
}

static BOOL sw_i2c_stop( void )
{
    clr_sw_i2c_scl();
    clr_sw_i2c_sda();
    Delay4us();
    set_sw_i2c_scl();
    Delay4us();
    set_sw_i2c_sda();
    Delay4us();
    return TRUE;
}

static BOOL sw_i2c_wait_ack( void )
{
    BYTE i;
    for( i = 0; i < 5; i++ )
    {    	
        Delay4us();
        if( sw_i2c_sda_low() )
            return TRUE;
    }
    if( sw_i2c_sda_low() )
        return TRUE;
    else
        return FALSE;
}

static BOOL sw_i2c_send_byte( BYTE value )
{
    BYTE i;
    Bool result;

    for( i = 0; i < 8; i++ )
    {
        if( value & BIT7 )
            set_sw_i2c_sda();
        else
            clr_sw_i2c_sda();
        Delay4us();
        set_sw_i2c_scl();
        Delay4us();
        value <<= 1;
        clr_sw_i2c_scl();
    }
    set_sw_i2c_sda();
    result = sw_i2c_wait_ack();
    set_sw_i2c_scl();
    Delay4us();
    clr_sw_i2c_scl();
    Delay4us();
    clr_sw_i2c_sda();

    return result;
}

static BYTE sw_i2c_receive_byte( const Bool ack )
{
    BYTE i;
    BYTE value = 0;

    for( i = 0; i < 8; i++ )
    {
        value <<= 1;
        set_sw_i2c_sda();
        Delay4us();
        set_sw_i2c_scl();
        Delay4us();
        if( sw_i2c_sda_high() )
            value |= BIT0;
        clr_sw_i2c_scl();
    }
    if( ack )
    {
        clr_sw_i2c_sda();
    }
    else
    {
        set_sw_i2c_sda();
    }
    Delay4us();
    set_sw_i2c_scl();
    Delay4us();
    clr_sw_i2c_scl();

    return value;
}

static BOOL sw_i2c_burst_write( WORD count, BYTE *buffer )
{
    while(count--)
    {
        if(sw_i2c_send_byte(*(buffer++)) != TRUE)
            return FALSE;
    }
    return TRUE;
}

static BOOL sw_i2c_burst_read( WORD count, BYTE * buffer )
{
    BYTE i;
	
    for(i = 0; i < count - 1; i++)
   		*(buffer + i) = sw_i2c_receive_byte(TRUE);
    *(buffer + i) = sw_i2c_receive_byte(FALSE);

	return TRUE;
}

void sw_i2c_db_init( void )
{
	/* Disable DDC, MIIC, PWM functions of GPIO04 nad GPIO05 */
	msWriteBit(REG_000405, FALSE, BIT4);
	msWriteBit(REG_000405, FALSE, BIT6);
	msWriteBit(REG_000408, FALSE, BIT2);
	msWriteBit(REG_000408, FALSE, BIT3);
}

BOOL sw_i2c_db_enter( BYTE client )
{
	unsigned char cmd_buf[] = {0x00, 0x53, 0x45, 0x52, 0x44, 0x42};
    BOOL ret;

	cmd_buf[0] = client;

    sw_i2c_start();
    if ((ret = sw_i2c_burst_write(6, cmd_buf)) == FALSE)
    	SWI2C_PRINT_MSG("[SWI2C_DB] Failed to enter serial debug mode!");
    sw_i2c_stop();

    return ret;
}

BOOL sw_i2c_db_exit( BYTE client )
{
	unsigned char cmd_buf[2];

	cmd_buf[0] = client;
	cmd_buf[1] = 0x45;

    sw_i2c_start();
    sw_i2c_burst_write(2, cmd_buf);
    sw_i2c_stop();

    return TRUE;
}

BOOL sw_i2c_db_other_cmd(BYTE client, BYTE cmd)
{
    unsigned char cmd_buf[2];
    BOOL ret;

    cmd_buf[0] = client;
    cmd_buf[1] = cmd;

    sw_i2c_start();
    if ((ret = sw_i2c_burst_write(2, cmd_buf)) == FALSE)
   		SWI2C_PRINT_DATA("[SWI2C_DB] Failed to command:0x%x!\n",cmd);
    sw_i2c_stop();

    return ret;
}

BOOL sw_i2c_db_acquire_bus( BYTE client, BOOL flag )
{
    unsigned char cmd_buf[2];
    BOOL ret;

    cmd_buf[0] = client;
    if (flag)
    	cmd_buf[1] = 0x35;	/* Acquire bus */
   	else
      	cmd_buf[1] = 0x34; 	/* Release bus */

    sw_i2c_start();
    if ((ret = sw_i2c_burst_write(2, cmd_buf)) == FALSE)
   		SWI2C_PRINT_MSG("[SWI2C_DB] Failed to acquire or release I2C bus!");
    sw_i2c_stop();

    return ret;
}

BOOL sw_i2c_db_read_byte( BYTE client, WORD addr, BYTE* data_buf )
{
    unsigned char cmd_buf[4];
    BOOL ret;

	cmd_buf[0] = client;
	cmd_buf[1] = 0x10;
	cmd_buf[2] = addr >> 8;
	cmd_buf[3] = addr & 0xFF;

    /* Send address of requested register */
    sw_i2c_start();
    if ((ret = sw_i2c_burst_write(4, cmd_buf)) == FALSE)
   		SWI2C_PRINT_MSG("[SWI2C_DB] Failed to send address of requested register!");

    /* Receive register value */
    sw_i2c_start();
	cmd_buf[0] = (client | 0x01);
    if ((ret = sw_i2c_burst_write(1, cmd_buf)) == FALSE)
   	{
   		SWI2C_PRINT_MSG("[SWI2C_DB] Failed to send receiver id!");
   	}
	else if ((ret = sw_i2c_burst_read(1, data_buf)) == FALSE)
	{
    	SWI2C_PRINT_MSG("[SWI2C_DB] Failed to receive data!");
	}
    sw_i2c_stop();

    return ret;
}

BOOL sw_i2c_db_write_byte( BYTE client, WORD addr, BYTE value )
{
    unsigned char cmd_buf[5];
    BOOL ret;

	cmd_buf[0] = client;
	cmd_buf[1] = 0x10;
	cmd_buf[2] = addr >> 8;
	cmd_buf[3] = addr & 0xFF;
	cmd_buf[4] = value;

    sw_i2c_start();
    if ((ret = sw_i2c_burst_write(5, cmd_buf)) == FALSE)
   		SWI2C_PRINT_MSG("[SWI2C_DB] Failed to write register!");
    sw_i2c_stop();

    return ret;
}

#endif	// end of ENABLE_DAISY_CHAIN

#undef 	_SWI2C_DEBUG_C_


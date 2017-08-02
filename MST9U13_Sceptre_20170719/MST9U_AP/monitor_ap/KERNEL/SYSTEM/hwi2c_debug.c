/*
 * Copyright (c) 2013 MStar Semiconductor, Inc.
 * All rights reserved.
 *
 * monitor_ap/kernel/system/hwi2c_debug.c
 *
 * Date:        18-Oct-2013
 * Comment:
 *       Hardware I2C debug mode subroutines.
 *
 * Note:
 *		Remember to enable corresponding I2C hardware in hwi2c.h.
 * Reversion History:
 */

#define _HWI2C_DEBUG_C_
#include "datatype.h"
#include "hwi2c.h"

#define HWI2C_DBMODE_DEBUG    0
#if ENABLE_MSTV_UART_DEBUG && HWI2C_DBMODE_DEBUG
#define HWI2C_DBMODE_PRINT(format, ...)     printf(format, ##__VA_ARGS__)
#else
#define HWI2C_DBMODE_PRINT(format, ...)		do{}while(0)
#endif

/**
 * hw_i2c_db_enter - send out a "enter serial debug mode" command
 * @u8IICIndex: the I2C device of master device
 * @client: the slave ID of client device
 *
 * The master device calls this function to send the command through
 * its I2C to tell the slave device to enter serial debug mode.
 */
bool hw_i2c_db_enter(BYTE u8IICIndex, BYTE client)
{
    unsigned char cmd_buf[] = {client, 0x53, 0x45, 0x52, 0x44, 0x42};
    unsigned short buf_size;
    bool ret;

	buf_size = sizeof(cmd_buf)/sizeof(cmd_buf[0]);

    hw_i2c_start(u8IICIndex);
    if ((ret = hw_i2c_burst_write(u8IICIndex, buf_size, cmd_buf)) == FALSE)
    	HWI2C_DBMODE_PRINT("[HW_I2C_DB] Failed to enter serial debug mode!\n");
    hw_i2c_stop(u8IICIndex);

    return ret;
}

bool hw_i2c_db_exit(BYTE u8IICIndex, BYTE client)
{
    unsigned char cmd_buf[] = {client, 0x45};
    unsigned short buf_size;

    buf_size = sizeof(cmd_buf)/sizeof(cmd_buf[0]);

    hw_i2c_start(u8IICIndex);
    hw_i2c_burst_write(u8IICIndex, buf_size, cmd_buf);
    hw_i2c_stop(u8IICIndex);

    return TRUE;
}

/**
 * hw_i2c_make_mcu_pause - send out a "pause MCU" or "run MCU" command
 * @u8IICIndex: the I2C device of master device
 * @client: the slave ID of client device
 * @flag: flag = 1, make MCU pause; flag = 0, make MCU run
 *
 * The master device calls this function to send the command through
 * its I2C to tell the slave device to pause or run its MCU.
 */
bool hw_i2c_db_pause_mcu(BYTE u8IICIndex, BYTE client, bool flag)
{
    unsigned char cmd_buf[2];
    bool ret;

    cmd_buf[0] = client;
    if (flag)
        cmd_buf[1] = 0x37;	/* Make 8051 pause */
    else
        cmd_buf[1] = 0x36;	/* Make 8051 run */

    hw_i2c_start(u8IICIndex);
    if ((ret = hw_i2c_burst_write(u8IICIndex, 2, cmd_buf)) == FALSE)
   		HWI2C_DBMODE_PRINT("[HW_I2C_DB] Failed to make 8051 pause or run!\n");
    hw_i2c_stop(u8IICIndex);

    return ret;
}

bool hw_i2c_db_acquire_bus(BYTE u8IICIndex, BYTE client, bool flag)
{
    unsigned char cmd_buf[2];
    bool ret;

    cmd_buf[0] = client;
    if (flag)
    	cmd_buf[1] = 0x35;	/* Acquire bus */
   	else
      	cmd_buf[1] = 0x34; 	/* Release bus */

    hw_i2c_start(u8IICIndex);
    if ((ret = hw_i2c_burst_write(u8IICIndex, 2, cmd_buf)) == FALSE)
   		HWI2C_DBMODE_PRINT("[HW_I2C_DB] Failed to acquire or release I2C bus!\n");
    hw_i2c_stop(u8IICIndex);

    return ret;
}

bool hw_i2c_db_other_cmd(BYTE u8IICIndex, BYTE client, BYTE cmd)
{
    unsigned char cmd_buf[2];
    bool ret;

    cmd_buf[0] = client;
    cmd_buf[1] = cmd;

    hw_i2c_start(u8IICIndex);
    if ((ret = hw_i2c_burst_write(u8IICIndex, 2, cmd_buf)) == FALSE)
   		HWI2C_DBMODE_PRINT("[HW_I2C_DB] Failed to command:0x%x!\n",cmd);
    hw_i2c_stop(u8IICIndex);

    return ret;
}

bool hw_i2c_db_read_byte(BYTE u8IICIndex, BYTE client, WORD addr, BYTE* data_buf)
{
    unsigned char cmd_buf[] = {client, 0x10, addr >> 8, addr & 0xFF};
    unsigned short buf_size;
    bool ret;

    buf_size = sizeof(cmd_buf)/sizeof(cmd_buf[0]);

    /* Send address of requested register */
    hw_i2c_start(u8IICIndex);
    if ((ret = hw_i2c_burst_write(u8IICIndex, buf_size, cmd_buf)) == FALSE)
   		HWI2C_DBMODE_PRINT("[HW_I2C_DB] Failed to send address of requested register!\n");

    /* Receive register value */
    hw_i2c_start(u8IICIndex);
	cmd_buf[0] = (client | 0x01);
    if ((ret = hw_i2c_burst_write(u8IICIndex, 1, cmd_buf)) == FALSE)
   	{
   		HWI2C_DBMODE_PRINT("[HW_I2C_DB] Failed to send receiver id!\n");
   	}
	else if ((ret = hw_i2c_burst_read(u8IICIndex, 1, data_buf)) == FALSE)
	{
    	HWI2C_DBMODE_PRINT("[HW_I2C_DB] Failed to receive data!\n");
	}
    hw_i2c_stop(u8IICIndex);

    return ret;
}

bool hw_i2c_db_write_byte(BYTE u8IICIndex, BYTE client, WORD addr, BYTE data)
{
    unsigned char cmd_buf[] = {client, 0x10, addr >> 8, addr & 0xFF, data};
    unsigned short buf_size;
    bool ret;

    buf_size = sizeof(cmd_buf)/sizeof(cmd_buf[0]);

    hw_i2c_start(u8IICIndex);
    if ((ret = hw_i2c_burst_write(u8IICIndex, buf_size, cmd_buf)) == FALSE)
   		HWI2C_DBMODE_PRINT("[HW_I2C_DB] Failed to write register!\n");
    hw_i2c_stop(u8IICIndex);

    return ret;
}

bool hw_i2c_db_write_2byte(BYTE u8IICIndex, BYTE client, WORD addr, WORD data)
{
    unsigned char cmd_buf[] = {client, 0x10, addr >> 8, addr & 0xFF, data & 0xFF, data >> 8};
    unsigned short buf_size;
    bool ret;

    buf_size = sizeof(cmd_buf)/sizeof(cmd_buf[0]);

    hw_i2c_start(u8IICIndex);
    if ((ret = hw_i2c_burst_write(u8IICIndex, buf_size, cmd_buf)) == FALSE)
    	HWI2C_DBMODE_PRINT("[HW_I2C_DB] Failed to write register!\n");
    hw_i2c_stop(u8IICIndex);

    return ret;
}

bool hw_i2c_db_write_4byte(BYTE u8IICIndex, BYTE client, WORD addr, DWORD data)
{
    unsigned char cmd_buf[] = {client, 0x10, addr >> 8, addr & 0xFF, data & 0xFF, data >> 8, data >> 16, data >> 24};
    unsigned short buf_size;
    bool ret;

    buf_size = sizeof(cmd_buf)/sizeof(cmd_buf[0]);

    hw_i2c_start(u8IICIndex);
    if ((ret = hw_i2c_burst_write(u8IICIndex, buf_size, cmd_buf)) == FALSE)
            HWI2C_DBMODE_PRINT("[HW_I2C_DB] Failed to write register!\n");
    hw_i2c_stop(u8IICIndex);

    return ret;
}

#undef _HWI2C_DEBUG_C_


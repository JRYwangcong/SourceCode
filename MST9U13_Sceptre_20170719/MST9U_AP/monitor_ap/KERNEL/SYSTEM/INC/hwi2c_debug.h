/*
 * Copyright (c) 2013 MStar Semiconductor, Inc.
 * All rights reserved.
 *
 * monitor_ap/kernel/system/inc/hwi2c_debug.h
 *
 * Date:        18-Oct-2013
 * Comment:
 * Reversion History:
 */

#ifndef _HWI2C_DEBUG_H_
#define _HWI2C_DEBUG_H_

#ifdef _HWI2C_DEBUG_C_
#define INTERFACE
#else
#define INTERFACE  extern
#endif

INTERFACE bool hw_i2c_db_enter(BYTE, BYTE);
INTERFACE bool hw_i2c_db_exit(BYTE, BYTE);
INTERFACE bool hw_i2c_db_pause_mcu(BYTE, BYTE, bool);
INTERFACE bool hw_i2c_db_acquire_bus(BYTE, BYTE, bool);
INTERFACE bool hw_i2c_db_other_cmd(BYTE, BYTE, BYTE);
INTERFACE bool hw_i2c_db_read_byte(BYTE, BYTE, WORD, BYTE *);
INTERFACE bool hw_i2c_db_write_byte(BYTE, BYTE, WORD, BYTE);
INTERFACE bool hw_i2c_db_write_2byte(BYTE, BYTE, WORD, WORD);
INTERFACE bool hw_i2c_db_write_4byte(BYTE, BYTE, WORD, DWORD);

#undef INTERFACE

#endif	/* _HWI2C_DEBUG_H_ */


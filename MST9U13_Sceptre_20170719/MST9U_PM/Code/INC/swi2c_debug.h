/*
 * Copyright (c) 2014 MStar Semiconductor, Inc.
 * All rights reserved.
 *
 * MST9U_PM/Code/swi2c_debug.h
 *
 * Date:        05-Feb-2014
 * Comment:
 *       Software I2C debug mode header file.
 *
 * Note:
 *		
 * Reversion History:
 */

#ifndef _SWI2C_DEBUG_H_
#define _SWI2C_DEBUG_H_
 
#ifdef _SWI2C_DEBUG_C_
#define INTERFACE
#else
#define INTERFACE  extern
#endif

INTERFACE void sw_i2c_db_init( void );
INTERFACE BOOL sw_i2c_db_enter( BYTE );
INTERFACE BOOL sw_i2c_db_exit( BYTE );
INTERFACE BOOL sw_i2c_db_other_cmd( BYTE, BYTE );
INTERFACE BOOL sw_i2c_db_acquire_bus( BYTE, BOOL );
INTERFACE BOOL sw_i2c_db_read_byte( BYTE, WORD, BYTE * );
INTERFACE BOOL sw_i2c_db_write_byte( BYTE, WORD, BYTE );

#undef INTERFACE

#endif


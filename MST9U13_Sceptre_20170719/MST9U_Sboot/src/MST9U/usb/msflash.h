/******************************************************************************
 Copyright (c) 2005 MStar Semiconductor, Inc.
 All rights reserved.

 [Module Name]: Msflash.h
 [Date]:        25-Apr-2003
 [Comment]:
   MST Flash subroutines.
   1. Only for size 0x40000 BYTE = 2Mbit flash
   2. msFlash.c need include in bank 3 only
   3. Because msFlash.c include in bank3 , so address can use the LSB WORD
 [Reversion History]:
*******************************************************************************/

#include "Board.h"
//#include "UserPrefDef.h"

#ifdef _MSFLASH_C_
#define _MSFLASHDEC_
#else
#define _MSFLASHDEC_ extern
#endif

#ifndef     MSFLASH_H
#define     MSFLASH_H

#ifndef FLASH_READ_BYTE_BY_CODE_POINTER
#define FLASH_READ_BYTE_BY_CODE_POINTER     1
#endif

//#define TOTAL_BANK_NUM  9
//#define FDATA_START_ADDR    0x0000

#define IDENT_NOT_FOUND 0xFFFF


#define SPI_SI_BUFF(addr) (REG8(0x0009C0+addr))
#define SPI_SO_BUFF(addr) (REG8(0x0009CA+addr))
#define SPI_WBF_SIZE21  (REG8(0x0009D4)) //hi=2nd cmd size, lo=1st cmd size
#define SPI_WBF_SIZE3   (REG8(0x0009D5)) //lo=3rd cmd size
#define SPI_RBF_SIZE21  (REG8(0x0009D6)) //hi=2nd cmd size, lo=1st cmd size
#define SPI_RBF_SIZE3   (REG8(0x0009D7)) //lo=3rd cmd size
#define SPI_CTRL0       (REG8(0x0009D8))
#define SPI_CTRL1       (REG8(0x0009D9))
#define SPI_TRIGGER     (REG8(0x0009DA))
#define SPI_STATUS      (REG8(0x0009DC))
#define SPI_CLR_STATUS  (REG8(0x0009DE))
//CTRL1
#define EN_2ND_CMD      BIT7
#define EN_3RD_CMD      BIT6
#define RDSR_IN_1ST     0x00
#define EN_AUTO_CHECK   BIT5
#define RDSR_IN_2ND     BIT3
#define RDSR_IN_3RD     BIT4
#define SET_WBF_SIZE(cmd1st, cmd2nd, cmd3rd)   {SPI_WBF_SIZE21=((cmd2nd<<4)+cmd1st);SPI_WBF_SIZE3=cmd3rd;}
#define SET_RBF_SIZE(cmd1st, cmd2nd, cmd3rd)   {SPI_RBF_SIZE21=((cmd2nd<<4)+cmd1st);SPI_RBF_SIZE3=cmd3rd;}
#define SPI_BUF_MAX     10
#define SPI_CTRL_VALUE  0x03


typedef enum
{
    BYTE_READ   = 0x03,
    BYTE_WRITE  = 0x02,
    H_READ      = 0x0B,
    RDSR        = 0x05,
    WRSR        = 0x01,
    WREN        = 0x06,
    WRDI        = 0x04,
    EWSR        = 0x50,
    CHIP_ERASE  = 0xC7,
} FLASH_OP;

// 25VF020B
typedef enum
{
    SST_READ        = 0x03,
    SST_H_READ      = 0x0B,
    SST_ERASE_SECTOR= 0x20,
    SST_ERASE_BLOCK = 0x52,
    SST_ERASE_CHIP  = 0x60,
    SST_BYTE_WRITE  = 0x02,
    SST_AAI         = 0xAF,
    SST_RDSR        = 0x05,
    SST_EWSR        = 0x50,
    SST_WRSR        = 0x01,
    SST_WREN        = 0x06,
    SST_WRDI        = 0x04,
    SST_READ_ID     = 0x9F // read out: 0xBF, 0x25, 0x8C
} FLASH_OP_SST;

typedef enum
{
    PMC_READ        = 0x03,
    PMC_H_READ      = 0x0B,
    PMC_ERASE_SECTOR= 0xD7,
    PMC_ERASE_BLOCK = 0xD8,
    PMC_ERASE_CHIP  = 0xC7,
    PMC_BYTE_WRITE  = 0x02,
    PMC_RDSR        = 0x05,
    PMC_WRSR        = 0x01,
    PMC_WREN        = 0x06,
    PMC_WRDI        = 0x04,
    PMC_READ_ID     = 0x9F, //0xAB
    PMC_UNPROTECT   = 0x26,
    PMC_PROTECT     = 0x24
} FLASH_OP_PMC;
typedef enum
{
    MXIC_READ           = 0x03,
    MXIC_H_READ         = 0x0B,
    MXIC_ERASE_SECTOR   = 0x20,
    MXIC_ERASE_BLOCK    = 0xD8,
    MXIC_ERASE_CHIP     = 0x60,
    MXIC_BYTE_WRITE     = 0x02,
    //AAI=0xAF,
    MXIC_RDSR           = 0x05,
    //EWSR=0x50,
    MXIC_WRSR           = 0x01,
    MXIC_WREN           = 0x06,
    MXIC_WRDI           = 0x04,
    MXIC_READ_ID        = 0x9F, // read output: 0xC2, 0x20, 0x12(2025 is same with 2026)
    MXIC_RES            = 0xAB  // read output: 0x03(2026); 0x11(2025)
} FLASH_OP_MXIC;
typedef enum
{
    EON_READ        = 0x03,
    EON_H_READ      = 0x0B,
    EON_ERASE_SECTOR= 0x20,
    EON_ERASE_BLOCK = 0x52,
    EON_ERASE_CHIP  = 0x60,
    EON_BYTE_WRITE  = 0x02,
    //AAI=0xAF,
    EON_RDSR        = 0x05,
    //EWSR=0x50,
    EON_WRSR        = 0x01,
    EON_WREN        = 0x06,
    EON_WRDI        = 0x04,
    EON_READ_ID     = 0x9F
} FLASH_OP_EON;
typedef enum
{
    AMIC_READ           = 0x03,
    AMIC_H_READ         = 0x0B,
    AMIC_ERASE_SECTOR   = 0x20,
    AMIC_ERASE_BLOCK    = 0xD8,
    AMIC_ERASE_CHIP     = 0xC7,
    AMIC_BYTE_WRITE     = 0x02,
    //AAI=0xAF,
    AMIC_RDSR           = 0x05,
    //EWSR=0x50,
    AMIC_WRSR           = 0x01,
    AMIC_WREN           = 0x06,
    AMIC_WRDI           = 0x04,
    AMIC_READ_ID        = 0x9F
} FLASH_OP_AMIC;

typedef enum
{
    WINBOND_READ            = 0x03,
    WINBOND_H_READ          = 0x0B,
    WINBOND_ERASE_SECTOR    = 0x20,
    WINBOND_ERASE_BLOCK     = 0xD8,
    WINBOND_ERASE_CHIP      = 0xC7,
    WINBOND_BYTE_WRITE      = 0x02,
    //AAI=0xAF,
    WINBOND_RDSR            = 0x05,
    //EWSR=0x50,
    WINBOND_WRSR            = 0x01,
    WINBOND_WREN            = 0x06,
    WINBOND_WRDI            = 0x04,

    WINBOND_READ_ID         = 0x9F,
    WINBOND_READ_UNIQUE_ID  = 0x4B
} FLASH_OP_WINBOND;

typedef enum
{
	SPANSION_READ_ID 		= 0x9F,
	SPANSION_SECTOR_ERASE 	= 0xD8,
} FLASH_OP_SPANSION;

typedef enum
{
    MX_2025     = 0x12,
    MX_2026     = 0x03,
    PM25_LV020  = 0x7D,
    PM25_LD020  = 0x22,
    W25X05CL    = 0x10, // 512K bit // 9F command ID15~0: 0x3010
    W25X10CL    = 0x11, // 1M bit // 0x3011
    W25X20CL    = 0x12, // 2M bit // 0x3012
    W25X10BV    = W25X10CL, // 1M bit // 0x3011
    W25X20BV    = W25X20CL, // 2M bit // 0x3012
    W25X40BV    = 0x13, // 4M bit // 0x3013
    W25X21CL    = 0x95, // 2M bit with special unprotection // 0x3012 by 9F command // 21CL: 0x95; 20CL: 0xD5 by 4B command
    W25X41CL    = 0x97, // 4M bit with special unprotection // 0x3013 by 9F command // 41CL: 0x97; by 4B command // 120619 coding wait for x41 unique id
} _DEVICE_ID;

typedef enum
{
    FLASH_SST     = 0xBF,
    FLASH_PMC     = 0x9D,
    FLASH_MXIC    = 0xC2,
    FLASH_EON     = 0x1C,
    FLASH_WINBOND = 0xEF, // 90h
    FLASH_AMIC    = 0x37,
    FLASH_SPANSION= 0x01,
    FLASH_UNKNOW  = 0xFF
} _FLASH_ID;

typedef void (* pProgessCB)(U8);

#define CHECKSUM_VALUE              0x34
#define FLASH_CHECKSUM              CHECKSUM_VALUE//0xAA

_MSFLASHDEC_ BYTE xdata g_ucFlashID;
//==================================================
///////////////////////////////////////////////////////////////////////////////
// <DRV><Description>: Check current MsFlash.c code version
// <Returns> : MsFlash.c version number
///////////////////////////////////////////////////////////////////////////////
//_MSFLASHDEC_ WORD FlashVersionCheck( void );
///////////////////////////////////////////////////////////////////////////////
// <DRV><Description>: Read flash Manufacturer ID include PMC, MXIC, EON, and SST.
//                Return FLASH_UNKNOW if not found
// <Returns> : Manufacturer ID.
///////////////////////////////////////////////////////////////////////////////
_MSFLASHDEC_ BYTE ReadFlashID( void );
///////////////////////////////////////////////////////////////////////////////
// <DRV><Description>: Flash erase function
// <Parameter>:  -  <Flow>  -  <Description>
//-----------------------------------------------------------------------------
//  bDoWP   -   In  -   Doing FlashDisableWP at start and FlashEnableWP at end
//  wAddr   -   In  -   Erase 4K BYTE sector address
///////////////////////////////////////////////////////////////////////////////
_MSFLASHDEC_ void FlashSectorErase( Bool bDoWP, DWORD wAddr );
///////////////////////////////////////////////////////////////////////////////
// <DRV><Description>: Write one byte to flash
// <Parameter>: -  <Flow>   -   <Description>
//-----------------------------------------------------------------------------
//  bDoWP   -   In  -   Doing FlashDisableWP at start and FlashEnableWP at end
//  wAddr   -   In  -   Flash Address to be write, high byte address will be (TOTAL_BANK_NUM-1)
//                      wAddr < FDATA_FACTORY_ADDR will be limit, so address < 0x3B000 will be limit
//  value   -   In  -   The value write to flash
///////////////////////////////////////////////////////////////////////////////
_MSFLASHDEC_ void FlashWriteByte( Bool bDoWP, DWORD wAddr, BYTE value );
///////////////////////////////////////////////////////////////////////////////
// <DRV><Description>: Read one byte from flash.
// <Returns> : Data value in the address
// <Parameter>:  -  <Flow>  -  <Description>
//-----------------------------------------------------------------------------
//  wAddr   -   In  -   Only in the same bank ot MsFlash.c
///////////////////////////////////////////////////////////////////////////////
_MSFLASHDEC_ void FlashHDCPWriteByte( DWORD wAddr, BYTE value );

_MSFLASHDEC_ BYTE FlashReadByte( DWORD wAddr );
//////////////////////////////////////////////////////////////////////////////
// <DRV><Description>: Write a table to flash
// <Parameter>:     -   <Flow>  -   <Description>
// ---------------------------------------------------------------------------
//  bDoWP   -   In  -   Doing FlashDisableWP at start and FlashEnableWP at end
//  wAddr   -   In  -   Flash Address to be write, high byte address will be (TOTAL_BANK_NUM-1)
//                      wAddr < FDATA_FACTORY_ADDR will be limit, so address < 0x3B000 will be limit
//  buffer  -   In  -   Data pointer of the table
//  count   -   In  -   Number of data
//////////////////////////////////////////////////////////////////////////////
_MSFLASHDEC_ void Flash_WriteTbl( Bool bDoWP, DWORD dwAddr, BYTE *buffer, DWORD count );
//////////////////////////////////////////////////////////////////////////////
// <DRV><Description>: Read a table from flash
// <Parameter>:     -   <Flow>  -   <Description>
// ---------------------------------------------------------------------------
//  wAddr   -   In  -   Flash Address
//  buffer  -   Out -   Data point of the table
//  count   -   In  -   Number of BYTE data
//////////////////////////////////////////////////////////////////////////////
_MSFLASHDEC_ void Flash_ReadTbl( DWORD dwAddr, BYTE *buffer, DWORD count );
_MSFLASHDEC_ void FlashDisableWP( DWORD wAddr );
_MSFLASHDEC_ void FlashEnableWP( void );
_MSFLASHDEC_ void FlashChipErase( Bool bDoWP );
#define FLASH_BURST_UNIT 4//256
_MSFLASHDEC_ void FlashBurstWrite( Bool bDoWP, DWORD dwAddr, BYTE *buffer, DWORD count, Bool bChipErase);
_MSFLASHDEC_ void FlashUpdateFW( Bool bDoWP, DWORD dwAddr, BYTE *buffer, DWORD count, Bool bChipErase);
_MSFLASHDEC_ void FlashProgressFuncAttach( pProgessCB pShowOSD);

#endif//#ifndef     MSFLASH_H


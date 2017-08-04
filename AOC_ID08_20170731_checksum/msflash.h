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

#include "board.h"

#ifdef _MSFLASH_C_
#define _MSFLASHDEC_
#else
#define _MSFLASHDEC_ extern
#endif

#ifndef		MSFLASH_H
#define		MSFLASH_H

#define IDENT_NOT_FOUND 0xFFFF

typedef enum
{
	READ=0x03,
	BYTE_WRITE=0x02,
	H_READ=0x0B,
	RDSR=0x05,
	WRSR=0x01,
	WREN=0x06,
	WRDI=0x04,
	EWSR=0x50,
}FLASH_OP;


typedef enum
{
	SST_READ=0x03,
	SST_H_READ=0x0B,
	SST_ERASE_SECTOR=0x20,
	SST_ERASE_BLOCK=0x52,
	SST_ERASE_CHIP=0x60,
	SST_BYTE_WRITE=0x02,
	SST_AAI=0xAF,
	SST_RDSR=0x05,
	SST_EWSR=0x50,
	SST_WRSR=0x01,
	SST_WREN=0x06,
	SST_WRDI=0x04,
	SST_READ_ID=0xAB//0x90
}FLASH_OP_SST;
typedef enum
{
	PMC_READ=0x03,//
	PMC_H_READ=0x0B,//
	PMC_ERASE_SECTOR=0xD7,
	PMC_ERASE_BLOCK=0xD8,
	PMC_ERASE_CHIP=0xC7,
	PMC_BYTE_WRITE=0x02,
	PMC_RDSR=0x05,//
	PMC_WRSR=0x01,//
	PMC_WREN=0x06,//
	PMC_WRDI=0x04,//
	PMC_READ_ID=0x9F,//0xAB
	PMC_UNPROTECT=0x26,
	PMC_PROTECT=0x24
}FLASH_OP_PMC;
typedef enum
{
	MXIC_READ=0x03,
	MXIC_H_READ=0x0B,
	MXIC_ERASE_SECTOR=0x20,
	MXIC_ERASE_BLOCK=0xD8,
	MXIC_ERASE_CHIP=0x60,
	MXIC_BYTE_WRITE=0x02,
	//AAI=0xAF,
	MXIC_RDSR=0x05,
	//EWSR=0x50,
	MXIC_WRSR=0x01,
	MXIC_WREN=0x06,
	MXIC_WRDI=0x04,
	MXIC_READ_ID=0x9F,
	MXIC_RES=0xAB
}FLASH_OP_MXIC;
typedef enum
{
	EON_READ=0x03,
	EON_H_READ=0x0B,
	EON_ERASE_SECTOR=0x20,
	EON_ERASE_BLOCK=0x52,
	EON_ERASE_CHIP=0x60,
	EON_BYTE_WRITE=0x02,
	//AAI=0xAF,
	EON_RDSR=0x05,
	//EWSR=0x50,
	EON_WRSR=0x01,
	EON_WREN=0x06,
	EON_WRDI=0x04,
	EON_READ_ID=0x9F
}FLASH_OP_EON;
typedef enum
{
	AMIC_READ=0x03,
	AMIC_H_READ=0x0B,
	AMIC_ERASE_SECTOR=0x20,
	AMIC_ERASE_BLOCK=0xD8,
	AMIC_ERASE_CHIP=0xC7,
	AMIC_BYTE_WRITE=0x02,
	//AAI=0xAF,
	AMIC_RDSR=0x05,
	//EWSR=0x50,
	AMIC_WRSR=0x01,
	AMIC_WREN=0x06,
	AMIC_WRDI=0x04,
	AMIC_READ_ID=0x9F
}FLASH_OP_AMIC;

typedef enum
{
	MX_2025     = 0x12,
	MX_2026     = 0x03,
}_MX_DEVICE_ID;

typedef enum
{
	FLASH_SST     = 0xBF,
	FLASH_PMC     = 0x9D,
	FLASH_MXIC    = 0xC2,
	FLASH_EON     = 0x1C,
	FLASH_WINBOND = 0xEF,
	FLASH_AMIC    = 0x37,
	FLASH_UNKNOW  = 0xFF
}_FLASH_ID;


typedef enum
{
	FBufType_MonitorSetting	    =0x88,
	FBufType_MonitorSetting2    =0x44,
	FBufType_TimingMode         =0x22,
	FBufType_FactorySetting     =0x11,
	FBufType_FullBuffer         =0x00, // 091022 coding, if buffer type is 0, it should be free buffer.
	FBufType_EmptyBuffer        =0xFF,
}FLASH_BUFFTER_TYPE;

//#define FLASH_SIZE                  0x40000
#define TOTAL_BANK_NUM              4 // (TOTAL_BANK_NUM-1) : current code bank
#define FDATA_DYNAMIC_SECTOR_NUM    4
#define FDATA_SECTOR_SIZE           0x1000

#define FDATA_START_ADDR              0xA000 // 0x3A000
#define FDATA_FACTORY_ADDR          0xB000 // 0x3B000
#define FDATA_DYNAMIC_ADDR          0xC000 // 0x3C000
#define FDATA_BUF_ADDR(Num)         (FDATA_DYNAMIC_ADDR+(Num<<12))//Flash Data Start

#define FLASH_IDENTDATA             0x55
#define FLASH_IDENTDATA_CLEAR       0x00
#define FLASH_CHECKSUM              0xAA
#define FLASH_EMPTYDATA             0xFF


#define MONITORSET_START            (FLASH_MonitorSettingAddr+1)
#define MONITORSET_SIZE             MonitorSettingSize
#define MONITORSET_BUF_START        (FLASH_FreeBufferAddr+1)
#define MAX_MONITORSET_NUM          ((FDATA_SECTOR_SIZE-1)/MonitorSettingSize-1)

#define MONITORSET2_START           (FLASH_MonitorSetting2Addr+1)
#define MONITORSET2_SIZE            MonitorSetting2Size
#define MONITORSET2_BUF_START       (FLASH_FreeBufferAddr+1)
#define MAX_MONITORSET2_NUM         ((FDATA_SECTOR_SIZE-1)/MonitorSetting2Size-1)

#define MODESET_START               (FLASH_TimingModeAddr+1)
#define MODESET_SIZE                ModeSettingSize
#define MODESET_BUF_START           (FLASH_FreeBufferAddr+1)
#define MAX_MODESET_NUM             ((FDATA_SECTOR_SIZE-1)/ModeSettingSize-1)


#if 	DISPLAY_CHECK_VOL
#define PROTECT_START_ADDR			0x009ff0ul//0x039FF0 
#endif	


typedef enum
{
	FLASH_KEY_FACTORY,
	FLASH_KEY_PANEL,
	FLASH_KEY_GAMMA,
	FLASH_KEY_HDCP,
	FLASH_KEY_DDCA,
	FLASH_KEY_DDCD,
	FLASH_KEY_DDCH,
	FLASH_KEY_DDCDP,
}FLASH_KEY_TYPE;

#define FACTORY_SECTOR_SIZE         (256-0x10)
#define FACTORY_STATUS_ADDR         (FLASH_FactorySettingAddr+1+FLASH_KEY_FACTORY)
#define FACTORY_BUF_STATUS_ADDR     (FLASH_FreeBufferAddr+1+FLASH_KEY_FACTORY)
#define FACTORY_SIZE                FactorySettingSize
#define FACTORY_START               (FLASH_FactorySettingAddr+0x10)//0x3B010
#define FACTORY_BUF_START           (FLASH_FreeBufferAddr+0x10)

#define PANEL_SECTOR_SIZE           256
#define PANEL_STATUS_ADDR           (FLASH_FactorySettingAddr+1+FLASH_KEY_PANEL)
#define PANEL_BUF_STATUS_ADDR       (FLASH_FreeBufferAddr+1+FLASH_KEY_PANEL)
//#define PANEL_SIZE                  PanelSettingSize
#define PANEL_START                 (FACTORY_START+FACTORY_SECTOR_SIZE)//0x3B100
#define PANEL_BUF_START             (FACTORY_BUF_START+FACTORY_SECTOR_SIZE)

#define GAMMA_SECTOR_SIZE           1024
#define GAMMA_STATUS_ADDR           (FLASH_FactorySettingAddr+1+FLASH_KEY_GAMMA)
#define GAMMA_BUF_STATUS_ADDR       (FLASH_FreeBufferAddr+1+FLASH_KEY_GAMMA)
#define GAMMA_SIZE                  960
#define GAMMA_START                 (PANEL_START+PANEL_SECTOR_SIZE)//0x3B200
#define GAMMA_BUF_START             (PANEL_BUF_START+PANEL_SECTOR_SIZE)

#define RESERVE_SECTOR_SIZE         1024
#define RESERVE_START               (GAMMA_START+GAMMA_SECTOR_SIZE)//0x3B600
#define RESERVE_BUF_START           (GAMMA_BUF_START+GAMMA_SECTOR_SIZE)

/*#define HDCPKEY_SECTOR_SIZE         512
#define HDCPKEY_STATUS_ADDR         (FLASH_FactorySettingAddr+1+FLASH_KEY_HDCP)
#define HDCPKEY_BUF_STATUS_ADDR     (FLASH_FreeBufferAddr+1+FLASH_KEY_HDCP)
#define HDCPKEY_SIZE                304
#define HDCPKEYSET_START            (RESERVE_START+RESERVE_SECTOR_SIZE)//0x3BA00
#define HDCPKEYSET_BUF_START        (RESERVE_BUF_START+RESERVE_SECTOR_SIZE)
*/

#if FDDCCI_WriteEDID
#define FLASH_DDCA_START_ADDR           0x37000
#define FLASH_DDCD_START_ADDR           0x37100
#define FLASH_DDCDP_START_ADDR         0x37200
#endif

#define HDCPKEY_SIZE                304
#define HDCPKEYSET_START            0xA000

#define DDCAKEY_SECTOR_SIZE         256
#define DDCAKEY_STATUS_ADDR         (FLASH_FactorySettingAddr+1+FLASH_KEY_DDCA)
#define DDCAKEY_BUF_STATUS_ADDR     (FLASH_FreeBufferAddr+1+FLASH_KEY_DDCA)
#define DDCAKEY_SIZE                128
#define DDCAKEYSET_START            (RESERVE_START+RESERVE_SECTOR_SIZE)//0x3BA00
#define DDCAKEYSET_BUF_START         (RESERVE_BUF_START+RESERVE_SECTOR_SIZE)

#define DDCDKEY_SECTOR_SIZE         256
#define DDCDKEY_STATUS_ADDR         (FLASH_FactorySettingAddr+1+FLASH_KEY_DDCD)
#define DDCDKEY_BUF_STATUS_ADDR     (FLASH_FreeBufferAddr+1+FLASH_KEY_DDCD)
#define DDCDKEY_SIZE                128//256
#define DDCDKEYSET_START            (DDCAKEYSET_START+DDCAKEY_SECTOR_SIZE)//0x3BB00
#define DDCDKEYSET_BUF_START        (DDCAKEYSET_BUF_START+DDCAKEY_SECTOR_SIZE)

#define DDCHKEY_SECTOR_SIZE         256
#define DDCHKEY_STATUS_ADDR         (FLASH_FactorySettingAddr+1+FLASH_KEY_DDCH)
#define DDCHKEY_BUF_STATUS_ADDR     (FLASH_FreeBufferAddr+1+FLASH_KEY_DDCH)
#define DDCHKEY_SIZE                256
#define DDCHKEYSET_START            (DDCDKEYSET_START+DDCDKEY_SECTOR_SIZE)//0x3BC00
#define DDCHKEYSET_BUF_START        (DDCDKEYSET_BUF_START+DDCDKEY_SECTOR_SIZE)

#define DDCDPKEY_SECTOR_SIZE        256
#define DDCDPKEY_STATUS_ADDR        (FLASH_FactorySettingAddr+1+FLASH_KEY_DDCDP)
#define DDCDPKEY_BUF_STATUS_ADDR    (FLASH_FreeBufferAddr+1+FLASH_KEY_DDCDP)
#define DDCDPKEY_SIZE               256
#define DDCDPKEYSET_START           (DDCHKEYSET_START+DDCHKEY_SECTOR_SIZE)//0x3BD00
#define DDCDPKEYSET_BUF_START       (DDCHKEYSET_BUF_START+DDCHKEY_SECTOR_SIZE)



typedef enum
{
	FLASH_KEY_EMPTY     = 0xFF,
	FLASH_KEY_WRITE     = 0xAA,
	FLASH_KEY_VALID     = 0x22,
	FLASH_KEY_INVALID   = 0x00,
}FLASH_KEY_STATUS;

#ifdef ReduceDDC
typedef enum
{ 
	VGA_EDID,
	DVI_EDID
} EDIDType;
#endif

//==================================================
_MSFLASHDEC_ BYTE xdata g_ucFlashID;
#if 1//USEFLASH
_MSFLASHDEC_ WORD xdata FLASH_MonitorSettingAddr;	//monitor Setting
_MSFLASHDEC_ WORD xdata FLASH_MonitorSetting2Addr;	//monitor Setting
_MSFLASHDEC_ WORD xdata FLASH_TimingModeAddr;	//Timing Mode
_MSFLASHDEC_ WORD xdata FLASH_FactorySettingAddr;	//Factory Setting
_MSFLASHDEC_ WORD xdata FLASH_FreeBufferAddr;	//Empty buffer
#endif
//==================================================
///////////////////////////////////////////////////////////////////////////////
// <DRV><Description>: Check current MsFlash.c code version
// <Returns> : MsFlash.c version number
///////////////////////////////////////////////////////////////////////////////
_MSFLASHDEC_ WORD FlashVersionCheck(void);
///////////////////////////////////////////////////////////////////////////////
// <DRV><Description>: Read flash Manufacturer ID include PMC, MXIC, EON, and SST.
//                Return FLASH_UNKNOW if not found
// <Returns> : Manufacturer ID.
///////////////////////////////////////////////////////////////////////////////
_MSFLASHDEC_ BYTE ReadFlashID(void);
#if 1//USEFLASH //------------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
// <DRV><Description>: Flash erase function
// <Parameter>:  -  <Flow>  -  <Description>
//-----------------------------------------------------------------------------
//  bDoWP   -   In  -   Doing FlashDisableWP at start and FlashEnableWP at end
//  wAddr   -   In  -   Erase 4K BYTE sector address
///////////////////////////////////////////////////////////////////////////////
_MSFLASHDEC_ void FlashSectorErase(Bool bDoWP, WORD wAddr);
///////////////////////////////////////////////////////////////////////////////
// <DRV><Description>: Write one byte to flash
// <Parameter>:	-  <Flow>	-	<Description>
//-----------------------------------------------------------------------------
//  bDoWP   -   In  -   Doing FlashDisableWP at start and FlashEnableWP at end
//  wAddr   -   In  -   Flash Address to be write, high byte address will be (TOTAL_BANK_NUM-1)
//                      wAddr < FDATA_FACTORY_ADDR will be limit, so address < 0x3B000 will be limit
//  value   -   In  -   The value write to flash
///////////////////////////////////////////////////////////////////////////////
_MSFLASHDEC_ void FlashWriteByte(Bool bDoWP, WORD wAddr, BYTE value);
#if DISPLAY_CHECK_VOL
_MSFLASHDEC_ void FlashWriteCheckSumTbl(Bool bDoWP,WORD wAddr, BYTE *buffer, WORD count);
#endif
///////////////////////////////////////////////////////////////////////////////
// <DRV><Description>: Read one byte from flash.
// <Returns> : Data value in the address
// <Parameter>:  -  <Flow>  -  <Description>
//-----------------------------------------------------------------------------
//  wAddr   -   In  -   Only in the same bank ot MsFlash.c
///////////////////////////////////////////////////////////////////////////////
_MSFLASHDEC_ void FlashHDCPWriteByte(WORD wAddr, BYTE value);

_MSFLASHDEC_ BYTE FlashReadByte(WORD wAddr);
//////////////////////////////////////////////////////////////////////////////
// <DRV><Description>: Write a table to flash
// <Parameter>: 	- 	<Flow>	-	<Description>
// ---------------------------------------------------------------------------
//  bDoWP   -   In  -   Doing FlashDisableWP at start and FlashEnableWP at end
//  wAddr   -   In  -   Flash Address to be write, high byte address will be (TOTAL_BANK_NUM-1)
//                      wAddr < FDATA_FACTORY_ADDR will be limit, so address < 0x3B000 will be limit
//  buffer  -   In  -   Data pointer of the table
//  count   -   In  -   Number of data
//////////////////////////////////////////////////////////////////////////////
_MSFLASHDEC_ void Flash_WriteTbl(Bool bDoWP, WORD dwAddr, BYTE *buffer, WORD count);
//////////////////////////////////////////////////////////////////////////////
// <DRV><Description>: Read a table from flash
// <Parameter>: 	- 	<Flow>	-	<Description>
// ---------------------------------------------------------------------------
//  wAddr   -   In  -   Flash Address
//  buffer  -   Out -   Data point of the table
//  count   -   In  -   Number of BYTE data
//////////////////////////////////////////////////////////////////////////////
_MSFLASHDEC_ void Flash_ReadTbl(WORD dwAddr, BYTE *buffer, WORD count);

//API start=================================================================================
//////////////////////////////////////////////////////////////////////////////
//  <API><Description>: Get each buffer address and assign to variable
//	FLASH_MonitorSettingAddr
//  FLASH_MonitorSetting2Addr
//	FLASH_TimingModeAddr
//  FLASH_FactorySettingAddr
//	FLASH_FreeBufferAddr
//////////////////////////////////////////////////////////////////////////////
_MSFLASHDEC_ void Flash_GetBufferAddr(void);
//////////////////////////////////////////////////////////////////////////////
// <API><Description>: After execute Flash_GetBufferAddr, makesure each address reasonable
//////////////////////////////////////////////////////////////////////////////
_MSFLASHDEC_ void Flash_AllocateBufferAddr(void);
//////////////////////////////////////////////////////////////////////////////
// <API><Description>: Check each buffer type correct or not,
//				  Erase the sector when type wrong
//////////////////////////////////////////////////////////////////////////////
_MSFLASHDEC_ void Flash_CheckBufferType(void);
//////////////////////////////////////////////////////////////////////////////
// <API><Description>: Clear all timing mode save in flash
//////////////////////////////////////////////////////////////////////////////
_MSFLASHDEC_ void Flash_ClearModeSet(void);
//////////////////////////////////////////////////////////////////////////////
// <API><Description>:	Write Factory setting to flash,
//					It will exchange buffer when not found empty space
//////////////////////////////////////////////////////////////////////////////
_MSFLASHDEC_ void Flash_WriteFactorySet(BYTE* pbuf,BYTE ucLen);
//////////////////////////////////////////////////////////////////////////////
// <API><Description>:	Read latest factory setting struct from flash.
// <Returns> : return TRUE when found factory setting
//////////////////////////////////////////////////////////////////////////////
_MSFLASHDEC_ Bool Flash_ReadFactorySet(BYTE* pbuf,BYTE ucLen);
//////////////////////////////////////////////////////////////////////////////
// <API><Description>:	Write HDCP ,DDCA or DDCD key to flash,
//					Need set correct type of key.
//                  ucIndex from 0 to (key size / ucBufSize) or (key size / ucBufSize+1)
//                  The last write ucLen = (key size % ucBufSize)
//                  Will backup to free buffer if write to index = (MAX_HDCPKEYSET_NUM-1)
// <Parameter>: 	- 	<Flow>	-	<Description>
// ---------------------------------------------------------------------------
//  ucType          -    In		-	FLASH_KEY_HDCP, FLASH_KEY_DDCA or FLASH_KEY_DDCD
//  wIndex			-    In		-	From 0 ~ x
//  ucBufSize	    -    In		-	Size of buf
//  *buf			-    In		-	point to the data of each set
//  wLen			-    In		-	Length of this time write buffer, ucBufSize>=ucLen
//////////////////////////////////////////////////////////////////////////////
_MSFLASHDEC_ void Flash_Write_Factory_KeySet(FLASH_KEY_TYPE ucType, WORD wIndex, WORD wBufSize, BYTE* buf, WORD wLen);

_MSFLASHDEC_ void Flash_Erase_FreeBuffer(FLASH_KEY_TYPE ucType,WORD wIndex);// jeff add it in 2009 0907


//////////////////////////////////////////////////////////////////////////////
// <API><Description>:	Read HDCP ,DDCA or DDCD key each ucLen from flash,
//					Need set correct ucType.
//                  ucIndex from 0 to (key size / ucBufSize) or (key size / ucBufSize+1)
//                  The last read ucLen = (key size % ucBufSize)
// <Returns> : return TRUE when found key ident and CheckSum
// <Parameter>: 	- 	<Flow>	-	<Description>
// ---------------------------------------------------------------------------
//  ucType          -    In		-	FLASH_KEY_HDCP, FLASH_KEY_DDCA or FLASH_KEY_DDCD
//  wIndex			-    In		-	From 0 ~ x
//  ucBufSize	    -    In		-	Size of buf
//  *buf			-    In		-	point to the data of each set
//  wLen			-    In		-	Length of this time write buffer, ucBufSize>=ucLen
//////////////////////////////////////////////////////////////////////////////
_MSFLASHDEC_ Bool Flash_Read_Factory_KeySet(FLASH_KEY_TYPE ucType, Bool bReadVaild, WORD wIndex, WORD wBufSize, BYTE* buf, WORD wLen);	//110311 Modify
//////////////////////////////////////////////////////////////////////////////
// <API><Description>:	Write Monitor setting struct to flash,
//					It will exchange buffer when buffer full
//////////////////////////////////////////////////////////////////////////////
_MSFLASHDEC_ void Flash_WriteMonitorSet(BYTE* buf,BYTE len);
//////////////////////////////////////////////////////////////////////////////
// <API><Description>:	Read latest Monitor setting struct from flash.
//					1.Got monitor setting index by check flash identity
//					2.If not found read CheckSum to got correct one
//					3.read the Monitor setting struct
// <Returns> : return TRUE when found MonitorSetting
//////////////////////////////////////////////////////////////////////////////
_MSFLASHDEC_ Bool Flash_ReadMonitorSet(BYTE* buf,BYTE len);
//////////////////////////////////////////////////////////////////////////////
// <API><Description>:	Flash_WriteMonitorSet2 isues to save MonitorSetting2
//					It will exchange buffer when buffer full
//////////////////////////////////////////////////////////////////////////////
_MSFLASHDEC_ void Flash_WriteMonitorSet2(BYTE* buf,BYTE len);
//////////////////////////////////////////////////////////////////////////////
// <API><Description>:	Read MonitorSetting2 struct from flash.
//					1.Got monitor setting index by check flash identity
// <Returns> : return TRUE when found MonitorSetting2
//////////////////////////////////////////////////////////////////////////////
_MSFLASHDEC_ Bool Flash_ReadMonitorSet2(BYTE* buf,BYTE len);
//////////////////////////////////////////////////////////////////////////////
// <API><Description>:	Write timing mode with index to flash
//					1.Search the timing mode from timing buffer
//					2.Clear the timing ident
//					3.Find empty timing mode bufer
//					4.Exchange buffer if FLASH_TimingModeAddr buffer full
//					5.Write timing struct to empty buffer
// <Parameter>: 	- 	<Flow>	-	<Description>
// ---------------------------------------------------------------------------
//  index			-    In		-	timing index, UserMode need |_BIT7 in index
//  *buf			-    In		-	point to the address of timing struct
//////////////////////////////////////////////////////////////////////////////
_MSFLASHDEC_ void Flash_WriteModeSet(BYTE index,BYTE *buf);
//////////////////////////////////////////////////////////////////////////////
// <API><Description>:	Read timing mode with index from flash
//					1.Search the timing mode from timing buffer by compare FLASH_IDENTDATA&TimingIndex
//					2.No update & return when not found
//					3.Read timing mode from buffer
//					4.upper level function check timing mode FLASH_IDENTDATA to makesure
// <Returns> : return TRUE when found the timing mode index
// <Parameter>: 	- 	<Flow>	-	<Description>
// ---------------------------------------------------------------------------
//  index			-    In		-	timing index, UserMode need |_BIT7 in index
//  *buf			-    In		-	point to the address of timing mode struct
//////////////////////////////////////////////////////////////////////////////
_MSFLASHDEC_ Bool Flash_ReadModeSet(BYTE index,BYTE *buf);
_MSFLASHDEC_ void FlashDisableWP(WORD wAddr);
_MSFLASHDEC_ void FlashEnableWP(void);

#if 0//HKC_ColorTemp_Adjust
extern void FlashErase(DWORD address);
extern void Flash_ReadTbl1(DWORD addr, BYTE *buffer, BYTE count);
extern void Flash_WriteTbl1(DWORD addr, BYTE *buffer, BYTE count);
#endif
extern void FlashEnableWP(void);
#endif
#endif//#ifndef		MSFLASH_H

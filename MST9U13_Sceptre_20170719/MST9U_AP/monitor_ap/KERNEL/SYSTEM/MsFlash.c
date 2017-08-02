/******************************************************************************
 Copyright (c) 2005 MStar Semiconductor, Inc.
 All rights reserved.

 [Module Name]: Msflash.c
 [Date]:        25-Apr-2003
 [Comment]:
   MST Flash subroutines.
 [Reversion History]:
*******************************************************************************/
#define _MSFLASH_C_

#include "datatype.h"
#include "Board.h"
//#include "Mode.h"
//#include "UserPrefDef.h"
#include "Global.h"
#include "MsOS.h"
#include "ms_reg.h"
#include "Ms_rwreg.h"
#include "SysInit.h"
#include "menudef.h"
#include "msOSD.h"
#include "MDebug.h"
#include "Common.h"
#include "misc.h"
#include "Mcu.h"
#include "msflash.h"
#include "GPIO_DEF.h"
#include "Utl.h"
//#include "halRwreg.h"
#define _FLASH_CODE_VERSION_    0x200

#define FLASH_DEBUG    1
#if ENABLE_MSTV_UART_DEBUG && FLASH_DEBUG
#define FLASH_PRINT(format, ...)     printf(format, ##__VA_ARGS__)
#else
#define FLASH_PRINT(format, ...)
#endif

Bool xdata bFlashWriteFactory = FALSE;
Bool xdata bFlashEraseBufferFinish = FALSE; // // jeff add it in 2009 0907
BYTE xdata g_ucFlashDeviceID = 0;
static pProgessCB ProgessCB = NULL;
FLASH_SEL g_ucFlashSelect = SPI0;
FLASH_INFO g_FlashInfo[MAX_SPI_NUM];
static BYTE NandReadFlashID(void);
static BYTE NorReadFlashID(void);
static void NandFlashPageProgram(Bool bDoWP,DWORD wAddr,BYTE * data,DWORD count);
static BYTE NandFlashReadByte(DWORD dwAddr);
static void NandFlashDisableWP(void);
static void NandFlashEnableWP(void);
static void NorFlashWriteByte( Bool bDoWP, DWORD wAddr, BYTE value );
static void NandFlashReadPage(DWORD dwAddr,BYTE *buffer, DWORD count);
static void NandFlashWritePage(DWORD wAddr);
static void NorFlashWritePage(DWORD wAddr);
static void NorFlashBurstWrite( Bool bDoWP, DWORD wAddr, BYTE *buffer, DWORD count, Bool bChipErase );
FLASH_INFO g_FlashInfoTable[] =
{   //Type,      mID,          DeviceID,    PageSize,   SpareArea,  BlockSize,  PlaneSize,  DeviceSize,     Plane,  PagesPerBlock,  BlockPerPlane,  BlockPerDevice,     SectorSize
    {NAND_FLASH, FLASH_MICRON, 0x12,        2048,       64,         0x20000,    0x8000000,  0x10000000,     2,      64,             1024,           2048,               0},
	{NAND_FLASH, FLASH_MXIC,   0x12,		2048,		64, 		0x20000,	0x8000000,	0x8000000,		1,		64, 			1024,			1024,				0},
    {NOR_FLASH,  FLASH_MXIC,   MX_1606E,    0,          0,          0,          0,          0,              0,      0,              0,              0,                  4096},
};

void SetFlashInfo(FLASH_SEL sel)
{
	BYTE i;
	BYTE index = (BYTE)sel;
	for(i=0;i<sizeof(g_FlashInfoTable);i++)
	{
		if((g_FlashInfo[index].mID == g_FlashInfoTable[i].mID) \
			&&(g_FlashInfo[index].DeviceID == g_FlashInfoTable[i].DeviceID) \
			&&(g_FlashInfo[index].Type == g_FlashInfoTable[i].Type) )
		{ // found
			memcpy(&g_FlashInfo[index],&g_FlashInfoTable[i],sizeof(FLASH_INFO));
			break;
		}
	}
	return;
}
FLASH_INFO GetFlashInfo(FLASH_SEL sel)
{
	return g_FlashInfo[(BYTE)sel];
}

void SetFlashType(FLASH_SEL sel,FLASH_TYPE type)
{
	g_FlashInfo[(BYTE)sel].Type = type;
	return;
}
void SetFlashSelection(FLASH_SEL sel)
{
	if(g_ucFlashSelect == sel)
		return;
	
	g_ucFlashSelect = sel;
	if(g_FlashInfo[(BYTE)sel].Type == NAND_FLASH)
	{
		msWriteByteMask(REG_00041B,BIT0|BIT1,BIT0|BIT1);
		msWriteByteMask(REG_000EF4,sel,0x03); // set csz selection		
	}
	else
	{
		msWriteByteMask(REG_00041B,0,BIT0|BIT1);
		msWriteByteMask(REG_000EF4,sel,0x03); // set csz selection
	}
	return;
}

FLASH_SEL GetFlashSelection(void)
{
	return g_ucFlashSelect;
}

DWORD NandFlashAddrCal(NAND_FLASH_OP Cmd, DWORD dwAddr)
{
	if(g_ucFlashSelect == SPI1 && GetFlashInfo(g_ucFlashSelect).Type == NAND_FLASH)
	{
		DWORD BlockAddr = (dwAddr/GetFlashInfo(GetFlashSelection()).BlockSize);
		DWORD CmdAddr = 0;
		switch (Cmd)
		{
			case READ_FROM_CACHE: // 2 byte 
			case PROGRAM_LOAD: // 2 byte
				CmdAddr = dwAddr % GetFlashInfo(GetFlashSelection()).PageSize;
				if(GetFlashInfo(GetFlashSelection()).PlaneNum)
					CmdAddr = CmdAddr | ((BlockAddr%2)<<12);
				break;
			case PAGE_READ: // 3 byte
			case BLOCK_ERASE: // 3 bytes
			case PROGRAM_EXECUTE: // 3 byte 
				CmdAddr = dwAddr / GetFlashInfo(GetFlashSelection()).PageSize;
				break;
			default:
				break;
		}
		//FLASH_PRINT("Cmd: %x, CmdAddr = 0x%X\n",Cmd,CmdAddr);	
		return CmdAddr;
	}
	return 0;
}

void FlashOutsideModeEnable(BOOL bEnable, WORD wSize)
{
	msWriteBit(FSP_BANK_SEL+0xF3,bEnable,BIT4);
	if(bEnable)
	{
 		msWrite2Byte(FSP_BANK_SEL+0xF0,wSize);
		if(g_ucFlashSelect == SPI1 && GetFlashInfo(g_ucFlashSelect).Type == NAND_FLASH)
	      msWriteByte(FSP_BANK_SEL+0xF2,0x3);
		else
		  msWriteByte(FSP_BANK_SEL+0xF2,0x4);	
	}
	else
	{
 		msWrite2Byte(FSP_BANK_SEL+0xF0,0);
	    msWriteByte(FSP_BANK_SEL+0xF2,0);
	}
}
///////////////////////////////////////////////////////////////////////////////
// <DRV><Description>: Check current MsFlash.c code version
// <Returns> : MsFlash.c version number
///////////////////////////////////////////////////////////////////////////////
void drvFlashWaitSPINotBusy(void)
{
    WORD u16Retry = 0xFFFF;
    while(!(SPI_STATUS&BIT0)&&u16Retry--)
        DelayUs(2);
    if (SPI_STATUS&BIT0)
        SPI_CLR_STATUS= 0x01; //clr int
    //FLASH_PRINT("u8Retry=%d",u8Retry);
}

///////////////////////////////////////////////////////////////////////////////
// <DRV><Description>: Return true when flash type is PMC .
///////////////////////////////////////////////////////////////////////////////
Bool FlashTypeIs_PMC( void )
{
    BYTE ucID = 0;

    // 1st
    SPI_SI_BUFF(0) = PMC_READ_ID;

    // 2nd
    SPI_SI_BUFF(1) = RDSR;

    SPI_CTRL0       = SPI_CTRL_VALUE; //ctrl en FSP/reset
    SPI_CTRL1       = (EN_2ND_CMD|EN_AUTO_CHECK|RDSR_IN_2ND);
    SET_WBF_SIZE(1, 1, 0);
    SET_RBF_SIZE(3, 1, 0);

    SPI_TRIGGER  = 0x01; //start flash programming
    drvFlashWaitSPINotBusy();

    ucID = SPI_SO_BUFF(1);

    if( ucID == FLASH_PMC )
    {
        if( SPI_SO_BUFF(2) == PM25_LD020 )
            g_ucFlashDeviceID = PM25_LD020;
        else if( SPI_SO_BUFF(2) == PM25_LV020 )
            g_ucFlashDeviceID = PM25_LV020;
        return TRUE;
    }
    return FALSE;
}
///////////////////////////////////////////////////////////////////////////////
// <DRV><Description>: Return true when flash type is SST.
///////////////////////////////////////////////////////////////////////////////
Bool FlashTypeIs_SST( void )
{
    BYTE ucID = 0;

    // 1st
    SPI_SI_BUFF(0) = SST_READ_ID;

    // 2nd
    SPI_SI_BUFF(1) = RDSR;

    SPI_CTRL0       = SPI_CTRL_VALUE; //ctrl en FSP/reset
    SPI_CTRL1       = (EN_2ND_CMD|EN_AUTO_CHECK|RDSR_IN_2ND);
    SET_WBF_SIZE(1, 1, 0);
    SET_RBF_SIZE(3, 1, 0);

    SPI_TRIGGER  = 0x01; //start flash programming
    drvFlashWaitSPINotBusy();

    ucID = SPI_SO_BUFF(0);

    if( ucID == FLASH_SST )
        return TRUE;
    return FALSE;
}
///////////////////////////////////////////////////////////////////////////////
// <DRV><Description>: Return true when flash type is MXIC.
///////////////////////////////////////////////////////////////////////////////
Bool FlashTypeIs_MXIC( void )
{
    BYTE ucID = 0;

    // 1st
    SPI_SI_BUFF(0) = MXIC_READ_ID;

    // 2nd
    SPI_SI_BUFF(1) = RDSR;

    SPI_CTRL0       = SPI_CTRL_VALUE; //ctrl en FSP/reset
    SPI_CTRL1       = (EN_2ND_CMD|EN_AUTO_CHECK|RDSR_IN_2ND);
    SET_WBF_SIZE(1, 1, 0);
    SET_RBF_SIZE(3, 1, 0);

    SPI_TRIGGER  = 0x01; //start flash programming
    drvFlashWaitSPINotBusy();

    ucID = SPI_SO_BUFF(0);

    if( ucID == FLASH_MXIC )
        return TRUE;
    return FALSE;
}
///////////////////////////////////////////////////////////////////////////////
// <DRV><Description>: Return true when flash type is EON.
///////////////////////////////////////////////////////////////////////////////
Bool FlashTypeIs_EON( void )
{
    BYTE ucID = 0;

    // 1st
    SPI_SI_BUFF(0) = EON_READ_ID;

    // 2nd
    SPI_SI_BUFF(1) = RDSR;

    SPI_CTRL0       = SPI_CTRL_VALUE; //ctrl en FSP/reset
    SPI_CTRL1       = (EN_2ND_CMD|EN_AUTO_CHECK|RDSR_IN_2ND);
    SET_WBF_SIZE(1, 1, 0);
    SET_RBF_SIZE(3, 1, 0);

    SPI_TRIGGER  = 0x01; //start flash programming
    drvFlashWaitSPINotBusy();

    ucID = SPI_SO_BUFF(0);

    if( ucID == FLASH_EON )
        return TRUE;
    return FALSE;
}
///////////////////////////////////////////////////////////////////////////////
// <DRV><Description>: Return true when flash type is AMIC.
///////////////////////////////////////////////////////////////////////////////
Bool FlashTypeIs_AMIC( void )
{
    BYTE ucID = 0;

    // 1st
    SPI_SI_BUFF(0) = AMIC_READ_ID;

    // 2nd
    SPI_SI_BUFF(1) = RDSR;

    SPI_CTRL0       = SPI_CTRL_VALUE; //ctrl en FSP/reset
    SPI_CTRL1       = (EN_2ND_CMD|EN_AUTO_CHECK|RDSR_IN_2ND);
    SET_WBF_SIZE(1, 1, 0);
    SET_RBF_SIZE(3, 1, 0);

    SPI_TRIGGER  = 0x01; //start flash programming
    drvFlashWaitSPINotBusy();

    ucID = SPI_SO_BUFF(0);

    if( ucID == FLASH_AMIC )
        return TRUE;
    return FALSE;
}
///////////////////////////////////////////////////////////////////////////////
// <DRV><Description>: Return true when flash type is WINBOND.
///////////////////////////////////////////////////////////////////////////////
Bool FlashTypeIs_WINBOND( void )
{
    BYTE ucID = 0;

    // 1st
    SPI_SI_BUFF(0) = WINBOND_READ_ID;

    // 2nd
    SPI_SI_BUFF(1) = RDSR;

    SPI_CTRL0       = SPI_CTRL_VALUE; //ctrl en FSP/reset
    SPI_CTRL1       = (EN_2ND_CMD|EN_AUTO_CHECK|RDSR_IN_2ND);
    SET_WBF_SIZE(1, 1, 0);
    SET_RBF_SIZE(3, 1, 0);

    SPI_TRIGGER  = 0x01; //start flash programming
    drvFlashWaitSPINotBusy();

    ucID = SPI_SO_BUFF(0);

    //FLASH_PRINT("W9F-2:%x\r\n",SPI_SO_BUFF(2));

    if( ucID == FLASH_WINBOND )
    {
        ucID = SPI_SO_BUFF(2);
        if(ucID == W25X05CL)
            g_ucFlashDeviceID = W25X05CL;
        else if(ucID == W25X10CL)
            g_ucFlashDeviceID = W25X10CL;
        else if(ucID == W25X20CL)
            g_ucFlashDeviceID = W25X20CL;
        else if(ucID == W25X40BV)
            g_ucFlashDeviceID = W25X40BV;

        return TRUE;
    }
    return FALSE;
}

Bool FlashTypeIs_SPANSION( void )
{
    BYTE ucID = 0;

    // 1st
    SPI_SI_BUFF(0) = SPANSION_READ_ID;

    // 2nd
    SPI_SI_BUFF(1) = RDSR;

    SPI_CTRL0       = SPI_CTRL_VALUE; //ctrl en FSP/reset
    SPI_CTRL1       = (EN_2ND_CMD|EN_AUTO_CHECK|RDSR_IN_2ND);
    SET_WBF_SIZE(1, 1, 0);
    SET_RBF_SIZE(3, 1, 0);

    SPI_TRIGGER  = 0x01; //start flash programming
    drvFlashWaitSPINotBusy();

    ucID = SPI_SO_BUFF(0);
    if ( ucID == FLASH_SPANSION )
    {
        g_ucFlashDeviceID = SPI_SO_BUFF(1);
        return TRUE;
    }

    return FALSE;
}

BYTE ReadFlashUniqueID_Winbond(void)
{
    // 1st
    SPI_SI_BUFF(0) = WINBOND_READ_UNIQUE_ID; // with 4 dummy bytes

    // 2nd
    SPI_SI_BUFF(5) = RDSR;

    SPI_CTRL0       = SPI_CTRL_VALUE; //ctrl en FSP/reset
    SPI_CTRL1       = (EN_2ND_CMD|EN_AUTO_CHECK|RDSR_IN_2ND);
    SET_WBF_SIZE(5, 1, 0);
    SET_RBF_SIZE(1, 1, 0);

    SPI_TRIGGER  = 0x01; //start flash programming
    drvFlashWaitSPINotBusy();

    return SPI_SO_BUFF(0);
}

BYTE ReadFlashDeviceID_MXIC( void )
{
    // 1st
    SPI_SI_BUFF(0) = MXIC_RES;
    SPI_SI_BUFF(1) = 0x00;
    SPI_SI_BUFF(2) = 0x00;
    SPI_SI_BUFF(3) = 0x00;
    // 2nd
    SPI_SI_BUFF(4) = RDSR;

    SPI_CTRL0       = SPI_CTRL_VALUE; //ctrl en FSP/reset
    SPI_CTRL1       = (EN_2ND_CMD|EN_AUTO_CHECK|RDSR_IN_2ND);
    SET_WBF_SIZE(4, 1, 0);
    SET_RBF_SIZE(1, 1, 0);
    SPI_TRIGGER  = 0x01; //start flash programming
    drvFlashWaitSPINotBusy();

    return SPI_SO_BUFF(0);
}
BYTE ReadFlashID( void )
{
	if (g_ucFlashSelect == SPI1 && GetFlashInfo(g_ucFlashSelect).Type == NAND_FLASH)
	{
		return NandReadFlashID();
	}
	else
	{
		return NorReadFlashID();
	}
}
BYTE NandReadFlashID(void)
{
	SPI_SI_BUFF(0) = READ_ID;
	SPI_SI_BUFF(1) = 0; // dummy byte
	
	SET_WBF_SIZE(2,0,0);
	SET_RBF_SIZE(2,0,0);

    SPI_CTRL0       = SPI_CTRL_VALUE; //ctrl en FSP/reset
    SPI_CTRL1       = 0;//(EN_2ND_CMD|EN_AUTO_CHECK|RDSR_IN_2ND);

	SPI_TRIGGER = 0x01;
	drvFlashWaitSPINotBusy();

	FLASH_PRINT("Read ID:\n");
	FLASH_PRINT("[0]:0x%X\n",SPI_SO_BUFF(0));
	FLASH_PRINT("[1]:0x%X\n",SPI_SO_BUFF(1));
	g_FlashInfo[(BYTE)GetFlashSelection()].mID = SPI_SO_BUFF(0);
	g_FlashInfo[(BYTE)GetFlashSelection()].DeviceID= SPI_SO_BUFF(1);

	SetFlashInfo(GetFlashSelection());

	return SPI_SO_BUFF(0);
}
///////////////////////////////////////////////////////////////////////////////
// <DRV><Description>: Read flash Manufacturer ID include PMC, MXIC, EON, and SST.
//                Return FLASH_UNKNOW if not found
// <Returns> : Manufacturer ID.
///////////////////////////////////////////////////////////////////////////////
BYTE NorReadFlashID( void )
{
    BYTE ucID;

    //FlashVersionCheck();

    if( FlashTypeIs_PMC() )
        ucID = FLASH_PMC;
    else if( FlashTypeIs_MXIC() )
    {
        ucID = ReadFlashDeviceID_MXIC();
		if( ucID == MX_1606E)
			g_ucFlashDeviceID = MX_1606E;
		else if( ucID == 0x03 )
            g_ucFlashDeviceID = MX_2026;
        else
            g_ucFlashDeviceID = MX_2025;
        ucID = FLASH_MXIC;
        FLASH_PRINT( "   g_ucFlashDeviceID[%x]\r\n", g_ucFlashDeviceID );
    }
    else if( FlashTypeIs_EON() )
        ucID = FLASH_EON;
    else if( FlashTypeIs_SST() )
        ucID = FLASH_SST;
    else if( FlashTypeIs_AMIC() )
        ucID = FLASH_AMIC;
    else if( FlashTypeIs_WINBOND() )
    {
        ucID = FLASH_WINBOND;
        if(g_ucFlashDeviceID == W25X20CL)
        {
            if(W25X21CL == ReadFlashUniqueID_Winbond())
                g_ucFlashDeviceID = W25X21CL;
        }
        else if(g_ucFlashDeviceID == W25X40BV)
        {
            if(W25X41CL == ReadFlashUniqueID_Winbond())
                g_ucFlashDeviceID = W25X41CL;
        }
        FLASH_PRINT( "   WinbondUniqueID[%x]\r\n", g_ucFlashDeviceID );
    }
    else if ( FlashTypeIs_SPANSION() )
        ucID = FLASH_SPANSION;
    else
    {
        ucID = FLASH_UNKNOW;
        FLASH_PRINT("UNKNOW FLASH!\r\n");
    }
	g_FlashInfo[(BYTE)GetFlashSelection()].mID= ucID;
	g_FlashInfo[(BYTE)GetFlashSelection()].DeviceID= g_ucFlashDeviceID;
	SetFlashInfo(GetFlashSelection());
    return ucID;
}

#if USEFLASH || XMODEM_DWNLD_ENABLE || ENABLE_USB_HOST
///////////////////////////////////////////////////////////////////////////////
// <DRV><Description>: Read flash status register.
//
// <Returns> : Flash status register.
///////////////////////////////////////////////////////////////////////////////
BYTE FlashReadSR( void )
{
	if((g_ucFlashSelect == SPI1) && (GetFlashInfo(g_ucFlashSelect).Type == NAND_FLASH))
	{
		SPI_SI_BUFF(0) = GET_FEATURE;
		SPI_SI_BUFF(1) = 0xC0;
	    SPI_CTRL0       = SPI_CTRL_VALUE;   //ctrl en FSP/reset
	    SPI_CTRL1       = RDSR_IN_1ST|EN_AUTO_CHECK;      //(RDSR_IN_1ST|EN_AUTO_CHECK);
	    SET_WBF_SIZE(2, 0, 0);
		SET_RBF_SIZE(1, 0, 0);		
		SPI_TRIGGER 	= 0x01; //start flash programming
		drvFlashWaitSPINotBusy();
		while(SPI_SO_BUFF(0) & BIT0)
			ForceDelay1ms(1);
		return SPI_SO_BUFF(0);
	}	
	else // NOR flash
	{
        SPI_SI_BUFF(0)   = RDSR;

        SPI_CTRL0       = SPI_CTRL_VALUE;   //ctrl en FSP/reset
        SPI_CTRL1       = RDSR_IN_1ST;      //(RDSR_IN_1ST|EN_AUTO_CHECK);
        SET_WBF_SIZE(1, 0, 0);
        SET_RBF_SIZE(1, 0, 0);
        SPI_TRIGGER     = 0x01; //start flash programming
        drvFlashWaitSPINotBusy();

        return SPI_SO_BUFF(0);
    }
}
///////////////////////////////////////////////////////////////////////////////
// <DRV><Description>: Flash Write enable
//Prior to any Byte-Program, AAI-Program, Sector-Erase, Block-Erase, or Chip-Erase operation, the Write-Enable (WREN)
//instruction must be executed.
//Wait WIP(SR[0]) clear then send WREN command
///////////////////////////////////////////////////////////////////////////////
//void FlashWriteEnable( void )
//{}

///////////////////////////////////////////////////////////////////////////////
// <DRV><Description>: Write value to flash status register
// <Parameter>:  -  <Flow>  -  <Description>
//-----------------------------------------------------------------------------
//  value  -    In    -   which value write to flash status register
///////////////////////////////////////////////////////////////////////////////
void FlashWriteSR( BYTE value )
{
	if(g_ucFlashSelect == SPI1 && GetFlashInfo(g_ucFlashSelect).Type == NAND_FLASH)
	{
		SPI_SI_BUFF(0) = SET_FEATURE;
		SPI_SI_BUFF(1) = 0xA0;
		SPI_SI_BUFF(2) = value;//BIT7|BIT5|BIT4|BIT3; // unlock

		SPI_SI_BUFF(3) = GET_FEATURE;
		SPI_SI_BUFF(4) = 0xA0;

		SPI_CTRL0 	   = SPI_CTRL_VALUE;
		SPI_CTRL1	   = RDSR_IN_2ND|EN_2ND_CMD|EN_AUTO_CHECK;
		SET_WBF_SIZE(3, 2, 0);
		SET_RBF_SIZE(0, 1, 0);
	}
	else 
    if( g_ucFlashID == FLASH_SST )
    {
        // 1st
        SPI_SI_BUFF(0)   = EWSR;
        // 2nd
        SPI_SI_BUFF(1)   = WRSR;
        SPI_SI_BUFF(2)   = value;
        // 3rd
        SPI_SI_BUFF(3)   = RDSR;

        SPI_CTRL0       = SPI_CTRL_VALUE; //ctrl en FSP/reset
        SPI_CTRL1       = (EN_2ND_CMD|EN_3RD_CMD|RDSR_IN_3RD|EN_AUTO_CHECK);
        SET_WBF_SIZE(1, 2, 1);
        SET_RBF_SIZE(0, 0, 1);
    }
    else if( g_ucFlashID == FLASH_AMIC )
    {
        BYTE SSR = FlashReadSR();
        if( SSR & 0x40 ) //_BIT6)
        {
            // 1st
            SPI_SI_BUFF(0)   = WREN;
            // 2nd
            SPI_SI_BUFF(1) = 0x90;
            SPI_SI_BUFF(2) = 0x00;
            SPI_SI_BUFF(3) = 0x28;
            SPI_SI_BUFF(4) = 0x00;
            SPI_SI_BUFF(5) = 0x3B;
            SPI_SI_BUFF(6) = 0x00;
            SPI_SI_BUFF(7) = 0x28;
            SPI_SI_BUFF(8) = 0xFF;
            // 3rd
            SPI_SI_BUFF(9)   = RDSR;

            SPI_CTRL0       = SPI_CTRL_VALUE; //ctrl en FSP/reset
            SPI_CTRL1       = (EN_2ND_CMD|EN_3RD_CMD|RDSR_IN_3RD|EN_AUTO_CHECK);
            SET_WBF_SIZE(1, 8, 1);
            SET_RBF_SIZE(0, 0, 1);

            SPI_TRIGGER     = 0x01; //start flash programming
            drvFlashWaitSPINotBusy();

            // 1st
            SPI_SI_BUFF(0)   = WRSR;
            SPI_SI_BUFF(1)   = value;
            // 2nd
            SPI_SI_BUFF(2)   = RDSR;

            SPI_CTRL0       = SPI_CTRL_VALUE; //ctrl en FSP/reset
            SPI_CTRL1       = (EN_2ND_CMD|RDSR_IN_2ND|EN_AUTO_CHECK);
            SET_WBF_SIZE(2, 1, 0);
            SET_RBF_SIZE(0, 1, 0);
        }
        else
        {
            // 1st
            SPI_SI_BUFF(0)   = WREN;
            // 2nd
            SPI_SI_BUFF(1)   = WRSR;
            SPI_SI_BUFF(2)   = value;
            // 3rd
            SPI_SI_BUFF(3)   = RDSR;

            SPI_CTRL0       = SPI_CTRL_VALUE; //ctrl en FSP/reset
            SPI_CTRL1       = (EN_2ND_CMD|EN_3RD_CMD|RDSR_IN_3RD|EN_AUTO_CHECK);
            SET_WBF_SIZE(1, 2, 1);
            SET_RBF_SIZE(0, 0, 1);
        }
    }
    else
    {
        // 1st
        SPI_SI_BUFF(0)   = WREN;
        // 2nd
        SPI_SI_BUFF(1)   = WRSR;
        SPI_SI_BUFF(2)   = value;
        // 3rd
        SPI_SI_BUFF(3)   = RDSR;

        SPI_CTRL0       = SPI_CTRL_VALUE; //ctrl en FSP/reset
        SPI_CTRL1       = (EN_2ND_CMD|EN_3RD_CMD|RDSR_IN_3RD|EN_AUTO_CHECK);
        SET_WBF_SIZE(1, 2, 1);
        SET_RBF_SIZE(0, 0, 1);
    }

    SPI_TRIGGER     = 0x01; //start flash programming
    drvFlashWaitSPINotBusy();

}
///////////////////////////////////////////////////////////////////////////////
// <DRV><Description>: Only used in PMC type flash. To disable protect of specific sector.
//                     if wAddr < FDATA_FACTORY_ADDR will not unprotect
// <Parameter>:  <Description>
//  wAddr : Which sector address will be unportect
///////////////////////////////////////////////////////////////////////////////
void FlashPMCUnprotectEnable( DWORD wAddr )
{
    if( g_ucFlashDeviceID == PM25_LD020 )
    {
        // 1st
        SPI_SI_BUFF(0)   = WREN;
        // 2nd
        SPI_SI_BUFF(1)   = PMC_UNPROTECT;
        SPI_SI_BUFF(2)   = (wAddr>>16)&0x0ff;
        SPI_SI_BUFF(3)   = ( wAddr >> 8 ) & 0x0f0;
        SPI_SI_BUFF(4)   = 0x00;
        // 3rd
        SPI_SI_BUFF(5)   = RDSR;

        SPI_CTRL0       = SPI_CTRL_VALUE; //ctrl en FSP/reset
        SPI_CTRL1       = (EN_2ND_CMD|EN_3RD_CMD|RDSR_IN_3RD|EN_AUTO_CHECK);
        SET_WBF_SIZE(1, 4, 1);
        SET_RBF_SIZE(0, 0, 1);

        /*
        SPI_SI_BUFF(0) = WREN;
        SPI_CTRL = 0xF8;
        SPI_SI_BUFF(0) = PMC_UNPROTECT;
        SPI_SI_BUFF(1) = TOTAL_BANK_NUM - 1; //(wAddr>>16)&0x0ff;
        SPI_SI_BUFF(2) = ( wAddr >> 8 ) & 0x0f0;
        SPI_SI_BUFF(3) = 0x00; //wAddr&0x0ff;
        SPI_CTRL = 0xFB;
        */
    }
    else
    {
        hw_ClrFlashWP();
        Delay1ms( 1 );

        // 1st
        SPI_SI_BUFF(0) = PMC_UNPROTECT;
        SPI_SI_BUFF(1) = (wAddr>>16)&0x0ff; //(wAddr>>16)&0x0ff;
        SPI_SI_BUFF(2) = ( wAddr >> 8 ) & 0x0ff;
        SPI_SI_BUFF(3) = wAddr & 0x0ff;
        // 2nd
        SPI_SI_BUFF(4)   = RDSR;

        SPI_CTRL0       = SPI_CTRL_VALUE; //ctrl en FSP/reset
        SPI_CTRL1       = (EN_2ND_CMD|RDSR_IN_2ND|EN_AUTO_CHECK);
        SET_WBF_SIZE(4, 1, 0);
        SET_RBF_SIZE(0, 1, 0);

        /*
        SPI_SI_BUFF(0) = PMC_UNPROTECT;
        SPI_SI_BUFF(1) = TOTAL_BANK_NUM - 1; //(wAddr>>16)&0x0ff;
        SPI_SI_BUFF(2) = ( wAddr >> 8 ) & 0x0ff;
        SPI_SI_BUFF(3) = wAddr & 0x0ff;
        SPI_CTRL = 0xBB;
        */
    }

    SPI_TRIGGER     = 0x01; //start flash programming
    drvFlashWaitSPINotBusy();

}
///////////////////////////////////////////////////////////////////////////////
// <DRV><Description>: Only used in PMC type flash. To disable un-protect of specific sector.
///////////////////////////////////////////////////////////////////////////////
void FlashPMCUnprotectDisable( void )
{
    // 1st
    SPI_SI_BUFF(0) = PMC_PROTECT;
    // 2nd
    SPI_SI_BUFF(1) = RDSR;

    SPI_CTRL0       = SPI_CTRL_VALUE; //ctrl en FSP/reset
    SPI_CTRL1       = (EN_2ND_CMD|RDSR_IN_2ND|EN_AUTO_CHECK);
    SET_WBF_SIZE(1, 1, 0);
    SET_RBF_SIZE(0, 1, 0);
    SPI_TRIGGER     = 0x01; //start flash programming
    drvFlashWaitSPINotBusy();
    /*
    SPI_SI_BUFF(0) = PMC_PROTECT;
    SPI_CTRL = 0xF8;
    */
}


void FlashDisableStausProtect_MXIC2026( void )
{
	if(g_ucFlashSelect == SPI1 && GetFlashInfo(g_ucFlashSelect).Type == NAND_FLASH)
		FlashWriteSR( BIT7|BIT5|BIT4|BIT3 );
    if( g_ucFlashID == FLASH_MXIC && g_ucFlashDeviceID == MX_2026 )
    {
        FlashWriteSR( 0x7F );
        if( bFlashWriteFactory )
            FlashWriteSR( 0x40 );
        else
            FlashWriteSR( 0x60 );
    }
}

#define FlashEnableWP_PMC     FlashPMCUnprotectDisable
void FlashEnableWP_EON(BYTE SRValue)
{
    if(( SRValue & 0x94 ) != 0x94 )
    {
        //FLASH_PRINT("  EnableWP_EON_SR[%x] W_94\r\n",SSR);
        hw_ClrFlashWP();
        Delay1ms( 1 );
        FlashWriteSR( 0x94 );
    }
}
void FlashEnableWP_AMIC(BYTE SRValue)
{
    if(( SRValue & 0xFC ) != 0xE8 )
    {
        //FLASH_PRINT("  EnableWP_AMIC_SR[%x] E8_0\r\n",SSR);
        hw_ClrFlashWP();
        Delay1ms( 1 );
        FlashWriteSR( 0xE8 );
        //FLASH_PRINT("  EnableWP_AMIC_SR[%x] E8_1\r\n",SSR);
    }
}
void FlashEnableWP_WINBOND(BYTE SRValue)
{
    if(g_ucFlashDeviceID == W25X40BV)
    {
        if((SRValue&0xAC) != 0xAC)
        {
            hw_ClrFlashWP();
            Delay1ms( 1 );
            FlashWriteSR( 0xAC ); // protect lower 256K(0~0x3FFFF)
        }
    }
    else if(g_ucFlashDeviceID == W25X21CL)
    {
        if((SRValue&0xE0) != 0xE0)
        {
            hw_ClrFlashWP();
            Delay1ms( 1 );
            FlashWriteSR( 0xE0 ); // protect 0~0x3BFFF
        }
    }
    else if(g_ucFlashDeviceID == W25X20BV)
    {
        if((SRValue&0xA8) != 0xA8)
        {
            hw_ClrFlashWP();
            Delay1ms( 1 );
            FlashWriteSR( 0xA8 ); // protect lower 128K(0~0x1FFFF)
        }
    }
    else if(g_ucFlashDeviceID == W25X10BV)
    {
        if((SRValue&0xA4) != 0xA4)
        {
            hw_ClrFlashWP();
            Delay1ms( 1 );
            FlashWriteSR( 0xA4 ); // protect lower 64K(0~0x0FFFF)
        }
    }
    else
    {
        if((SRValue&0x8C) != 0x8C)
        {
            hw_ClrFlashWP();
            Delay1ms( 1 );
            FlashWriteSR( 0x8C ); // protect all
        }
    }
}
void FlashEnableWP_OTHERS(BYTE SRValue)
{
    if(( SRValue & 0x8C ) != 0x8C )
    {
        //FLASH_PRINT("  EnableWP__SR[%x] W_8C\r\n",SSR);
        hw_ClrFlashWP();
        Delay1ms( 1 );
        FlashWriteSR( 0x8C );
    }
}

#define FlashDisableWP_PMC      FlashPMCUnprotectEnable
#define FlashDisableWP_MXIC_MX2026      FlashDisableStausProtect_MXIC2026
void FlashDisableWP_EON(BYTE SRValue)
{
    hw_ClrFlashWP();
    Delay1ms( 1 );
    if( bFlashWriteFactory )
    {
        //FLASH_PRINT("  _EON_SR[%x] W_00\r\n",SSR);
        FlashWriteSR( 0x00 );
    }
    else
    {
        if(( SRValue & 0x94 ) != 0x94 )
        {
            //FLASH_PRINT("  _EON_SR[%x] W_94\r\n",SSR);
            FlashWriteSR( 0x94 );
        }
    }
}
void FlashDisableWP_WINBOND_W25X21CL(void)
{
    // X21CL: // 0xC0: protect range 0~0x39FFF; 0xE0: 0~0x3BFFF
    // X41CL: // 0xC0: protect range 0~0x79FFF; 0xE0: 0~0x7BFFF

    hw_ClrFlashWP();
    Delay1ms( 1 );
    if( bFlashWriteFactory )
    {
        FlashWriteSR( 0xC0 );
    }
    else
    {
        FlashWriteSR( 0xE0 );
    }
}

void FlashDisableWP_OTHERS(BYTE SRValue)
{
    hw_ClrFlashWP();
    Delay1ms( 1 );
    if( SRValue != 0x00 )
    {
        FlashWriteSR( 0x00 );
    }
}

///////////////////////////////////////////////////////////////////////////////
// <DRV><Description>: Enable flash write protect
///////////////////////////////////////////////////////////////////////////////
void FlashEnableWP( void )
{
    BYTE SSR;

    SSR = FlashReadSR();
	if((g_ucFlashSelect == SPI1) && (GetFlashInfo(g_ucFlashSelect).Type == NAND_FLASH))
	{
		NandFlashEnableWP();
	}
	else
    if( g_ucFlashID == FLASH_PMC )
    {
        FlashEnableWP_PMC();//FlashPMCUnprotectDisable();
    }
    else if( g_ucFlashID == FLASH_EON )
    {
        FlashEnableWP_EON(SSR);
    }
    else if( g_ucFlashID == FLASH_AMIC )
    {
        FlashEnableWP_AMIC(SSR);
    }
    else if( g_ucFlashID == FLASH_MXIC && g_ucFlashDeviceID == MX_2026 )
    {
        //SRAM type auto protect when write finish
    }
    else if( g_ucFlashID == FLASH_WINBOND )
    {
        FlashEnableWP_WINBOND(SSR);
    }
    else
    {
        FlashEnableWP_OTHERS(SSR);
    }

    hw_SetFlashWP();

}

void NandFlashDisableWP( void )
{	
	FlashWriteSR(0);
	return;
}
void NandFlashEnableWP(void)
{
	FlashWriteSR(BIT3|BIT4|BIT5);
	return;
}
///////////////////////////////////////////////////////////////////////////////
// <DRV><Description>: Disable flash write protect
///////////////////////////////////////////////////////////////////////////////
void FlashDisableWP( DWORD wAddr )
{
    BYTE SSR;
/*
    if( wAddr < FDATA_START_ADDR )
        return;
*/
    SSR = FlashReadSR();
	if((g_ucFlashSelect == SPI1) && (GetFlashInfo(g_ucFlashSelect).Type == NAND_FLASH))
	{
		NandFlashDisableWP();
	}
	else
    if( g_ucFlashID == FLASH_PMC )
    {
        FlashDisableWP_PMC(wAddr);//FlashPMCUnprotectEnable( wAddr );
    }
    else if( g_ucFlashID == FLASH_EON )
    {
        FlashDisableWP_EON(SSR);
    }
    else if( g_ucFlashID == FLASH_AMIC )
    {
        ;
    }
    else if( g_ucFlashID == FLASH_MXIC && g_ucFlashDeviceID == MX_2026 )
    {
        FlashDisableWP_MXIC_MX2026();//FlashDisableStausProtect_MXIC2026();
    }
    else if( g_ucFlashID == FLASH_WINBOND && (g_ucFlashDeviceID == W25X21CL || g_ucFlashDeviceID == W25X41CL)) // just 21CL needs to change SR
    {
        FlashDisableWP_WINBOND_W25X21CL();
    }
    else
    {
        FlashDisableWP_OTHERS(SSR);
    }

}

///////////////////////////////////////////////////////////////////////////////
// <DRV><Description>: Write one byte to flash
// <Parameter>: -  <Flow>   -   <Description>
//-----------------------------------------------------------------------------
//  bDoWP   -   In  -   Doing FlashDisableWP at start and FlashEnableWP at end
//  wAddr   -   In  -   Flash Address to be write, high byte address will be (TOTAL_BANK_NUM-1)
//                      wAddr < FDATA_FACTORY_ADDR will be limit, so address < 0x3B000 will be limit
//  value   -   In  -   The value write to flash
///////////////////////////////////////////////////////////////////////////////
void FlashWriteByte( Bool bDoWP, DWORD wAddr, BYTE value )
{
	if (g_ucFlashSelect == SPI1 && GetFlashInfo(g_ucFlashSelect).Type == NAND_FLASH)
		return; // NAND flash use page program
	else
		NorFlashWriteByte(bDoWP,wAddr,value);
	return;
}
void NorFlashWriteByte( Bool bDoWP, DWORD wAddr, BYTE value )
{
    if( bDoWP )
        FlashDisableWP( wAddr );
    else
        FlashDisableStausProtect_MXIC2026();

    // 1st
    SPI_SI_BUFF(0)   = WREN;
    // 2nd
    SPI_SI_BUFF(1)   = BYTE_WRITE;
    SPI_SI_BUFF(2)   = (wAddr>>16)&0x0ff;
    SPI_SI_BUFF(3)   = (wAddr>>8)&0x0ff;
    SPI_SI_BUFF(4)   = wAddr&0x0ff;
    SPI_SI_BUFF(5)   = value;
    // 3rd
    SPI_SI_BUFF(6)   = RDSR;

    SPI_CTRL0       = SPI_CTRL_VALUE; //ctrl en FSP/reset
    SPI_CTRL1       = (EN_2ND_CMD|EN_3RD_CMD|RDSR_IN_3RD|EN_AUTO_CHECK);
    SET_WBF_SIZE(1, 5, 1);
    SET_RBF_SIZE(0, 0, 1);
    SPI_TRIGGER     = 0x01; //start flash programming
    drvFlashWaitSPINotBusy();

    if( bDoWP )
        FlashEnableWP();
}

#if HDCPKEY_IN_Flash
void FlashHDCPWriteByte( DWORD wAddr, BYTE value )
{
    FlashDisableStausProtect_MXIC2026();//For MXIC 2026

    // 1st
    SPI_SI_BUFF(0)   = WREN;
    // 2nd
    SPI_SI_BUFF(1) = BYTE_WRITE;
    SPI_SI_BUFF(2) = (wAddr>>16)&0x0ff;
    SPI_SI_BUFF(3) = ( wAddr >> 8 ) & 0x0ff;
    SPI_SI_BUFF(4) = wAddr & 0x0ff;
    SPI_SI_BUFF(5) = value;
    // 3rd
    SPI_SI_BUFF(6)   = RDSR;

    SPI_CTRL0       = SPI_CTRL_VALUE; //ctrl en FSP/reset
    SPI_CTRL1       = (EN_2ND_CMD|EN_3RD_CMD|RDSR_IN_3RD|EN_AUTO_CHECK);
    SET_WBF_SIZE(1, 5, 1);
    SET_RBF_SIZE(0, 0, 1);
    SPI_TRIGGER     = 0x01; //start flash programming
    drvFlashWaitSPINotBusy();

    /*
    FlashWriteEnable();
    SPI_SI_BUFF(0) = BYTE_WRITE;
    SPI_SI_BUFF(1) = TOTAL_BANK_NUM - 1; //(wAddr>>16)&0x0ff;
    SPI_SI_BUFF(2) = ( wAddr >> 8 ) & 0x0ff;
    SPI_SI_BUFF(3) = wAddr & 0x0ff;
    SPI_SI_BUFF(4) = value;
    SPI_CTRL = 0xFC;
    */

}
#endif
BYTE NorFlashReadByte( DWORD wAddr )
{
    //1st
    SPI_SI_BUFF(0)   = READ;
    SPI_SI_BUFF(1)   = (wAddr>>16)&0x0ff;
    SPI_SI_BUFF(2)   = (wAddr>>8)&0x0ff;
    SPI_SI_BUFF(3)   = wAddr&0x0ff;
    //2nd
    SPI_SI_BUFF(4)   = RDSR;

    SPI_CTRL0       = SPI_CTRL_VALUE; //ctrl en FSP/reset
    SPI_CTRL1       = (EN_2ND_CMD|RDSR_IN_2ND|EN_AUTO_CHECK);
    SET_WBF_SIZE( 4, 1, 0);
    SET_RBF_SIZE(1, 1, 0);
    SPI_TRIGGER     = 0x01; //start flash programming

    drvFlashWaitSPINotBusy();
    return SPI_SO_BUFF(0);
}
BYTE NandFlashReadByte(DWORD dwAddr)
{
	BYTE buffer=0;

	NandFlashReadPage(dwAddr, &buffer, 1);
	
	return buffer;
	
}

void NandFlashReadToCache(DWORD dwAddr)
{
	DWORD CmdAddr = NandFlashAddrCal(PAGE_READ,dwAddr);
	SPI_SI_BUFF(0) = PAGE_READ;
	SPI_SI_BUFF(1) = (CmdAddr >> 16) & 0xFF;
	SPI_SI_BUFF(2) = (CmdAddr >> 8) & 0xFF;
	SPI_SI_BUFF(3) = CmdAddr & 0xFF;

	SPI_SI_BUFF(4) = GET_FEATURE;
	SPI_SI_BUFF(5) = 0xC0;

	SET_WBF_SIZE(4, 2, 0);
	SET_RBF_SIZE(0, 1, 0);

	SPI_CTRL0 = SPI_CTRL_VALUE;
	SPI_CTRL1 = (EN_2ND_CMD | RDSR_IN_2ND |EN_AUTO_CHECK);

	SPI_TRIGGER = 0x01;
	while(SPI_SO_BUFF(0) & BIT0)
		ForceDelay1ms(1);
	return;
	
		
}
void NandFlashReadPage(DWORD dwAddr,BYTE *buffer, DWORD count)
{

	BYTE u8PlaneIndex = dwAddr/GetFlashInfo(GetFlashSelection()).PlaneSize;
	DWORD PageAddr = (dwAddr/GetFlashInfo(GetFlashSelection()).PageSize) & (GetFlashInfo(GetFlashSelection()).PagesPerBlock-1) ;
	DWORD BlockAddr = (dwAddr/GetFlashInfo(GetFlashSelection()).BlockSize) & (GetFlashInfo(GetFlashSelection()).BlocksPerPlane-1);	
	DWORD CmdAddr = (BlockAddr*GetFlashInfo(GetFlashSelection()).PageSize*2) | (u8PlaneIndex*GetFlashInfo(GetFlashSelection()).PageSize) | PageAddr;
	DWORD ReadSize = 0;
	BYTE i=0;
	// Step1: Page Read to Cache (13h)
	SPI_SI_BUFF(0) = WRITE_ENABLE;

	CmdAddr = NandFlashAddrCal(PAGE_READ,dwAddr);
	SPI_SI_BUFF(1) = PAGE_READ;
	SPI_SI_BUFF(2) = (CmdAddr >> 16) & 0xFF; 
	SPI_SI_BUFF(3) = (CmdAddr >> 8) & 0xFF;
	SPI_SI_BUFF(4) = CmdAddr & 0xFF;
	
	SPI_SI_BUFF(5) = GET_FEATURE;
	SPI_SI_BUFF(6) = 0xC0;

	SPI_CTRL0 = SPI_CTRL_VALUE;
	SPI_CTRL1 = (EN_2ND_CMD|EN_3RD_CMD|RDSR_IN_3RD|EN_AUTO_CHECK);
	SET_WBF_SIZE(1, 4, 2);
	SET_RBF_SIZE(0, 0, 1);
	SPI_TRIGGER = 0x01;
	
    drvFlashWaitSPINotBusy();

	while(SPI_SO_BUFF(0) & BIT0)
	{
		ForceDelay1ms(1);
	}	
	// Step2: Read From Cache (0Bh or 03h)
	
	// sw cz
	if(g_ucFlashSelect == SPI0)
		msWriteByte(REG_000A14, 0x01);
	else
	{
		msWriteByte(0x000E14, 0x03);
		ForceDelay1ms(1);
		msWriteByte(0x000E14, 0x01);
	}
	CmdAddr =  NandFlashAddrCal(READ_FROM_CACHE,dwAddr);
	SPI_SI_BUFF(0) = READ_FROM_CACHE;
	SPI_SI_BUFF(1) = (CmdAddr >> 8) & 0xFF;
	SPI_SI_BUFF(2) = CmdAddr & 0xFF;
	SPI_SI_BUFF(3) = 0;// dummy
	ReadSize = count;
	if(ReadSize > 9)
		ReadSize = 9;

	SPI_CTRL0 = SPI_CTRL_VALUE;
	SPI_CTRL1 = 0;
	SET_WBF_SIZE(4, 0, 0);
	SET_RBF_SIZE(ReadSize, 0, 0);
	SPI_TRIGGER = 0x01;
	drvFlashWaitSPINotBusy();

	for(i=0;i<ReadSize;i++)
	{
		*(buffer++) = SPI_SO_BUFF(i);
	}
	count -= ReadSize;

	while(count>0)
	{
		
		dwAddr += ReadSize;
		ReadSize = count;
		if(ReadSize > 10)
			ReadSize = 10;
		SPI_CTRL0 = SPI_CTRL_VALUE;
		SPI_CTRL1 = 0;
		SET_WBF_SIZE(0, 0, 0);		
		SET_RBF_SIZE(ReadSize,0,0);
		SPI_TRIGGER = 0x01;
		drvFlashWaitSPINotBusy();
		for(i=0;i<ReadSize;i++)
		{
			*(buffer++) = SPI_SO_BUFF(i);
		}
		count -= ReadSize;
	}

	//disable sw cz
	if(g_ucFlashSelect == SPI0)
		msWriteByte(REG_000A14, 0x00);
	else
		msWriteByte(0x000E14, 0x00);
	//Status = FlashReadSR();
	while(FlashReadSR() & BIT0)
	{
		ForceDelay1ms(1);
	}
		
}
///////////////////////////////////////////////////////////////////////////////
// <DRV><Description>: Read one byte from flash.
// <Returns> : Data value in the address
// <Parameter>:  -  <Flow>  -  <Description>
//-----------------------------------------------------------------------------
//  wAddr   -   In  -   Only in the same bank ot MsFlash.c
///////////////////////////////////////////////////////////////////////////////
#if FLASH_READ_BYTE_BY_CODE_POINTER
#define xfr_FlashCode ((unsigned char  volatile *) 0xC0000000)
BYTE FlashReadByte( DWORD wAddr )
{
	if(g_ucFlashSelect == SPI0)
    return xfr_FlashCode[wAddr];
 	else if(g_ucFlashSelect == SPI1 && GetFlashInfo(g_ucFlashSelect).Type == NAND_FLASH)
		return NandFlashReadByte(wAddr);
	else
		return NorFlashReadByte(wAddr);
}
#else
BYTE FlashReadByte( DWORD wAddr )
{	
	if(g_ucFlashSelect == SPI1 && GetFlashInfo(g_ucFlashSelect).Type == NAND_FLASH)
		return NandFlashReadByte(wAddr);
	else
		return NorFlashReadByte(wAddr);
}
#endif
void NandFlashProgramExecute(DWORD wAddr)
{
	DWORD CmdAddr = NandFlashAddrCal(PROGRAM_EXECUTE,wAddr);
	SPI_SI_BUFF(0) = PROGRAM_EXECUTE;
	SPI_SI_BUFF(1) = (CmdAddr >> 16) & 0xFF;
	SPI_SI_BUFF(2) = (CmdAddr >> 8) & 0xFF;
	SPI_SI_BUFF(3) = CmdAddr & 0xFF;

	SPI_SI_BUFF(4) = GET_FEATURE;
	SPI_SI_BUFF(5) = 0xC0;
	
	//SPI_SI_BUFF(6) = GET_FEATURE;
	//SPI_SI_BUFF(7) = 0xC0;
	
	SET_WBF_SIZE(4,2,0);
	SET_RBF_SIZE(0,1,0);
	
	SPI_CTRL0 = SPI_CTRL_VALUE;
	SPI_CTRL1 = EN_2ND_CMD|RDSR_IN_2ND|EN_AUTO_CHECK;

	SPI_TRIGGER = 0x01;
	drvFlashWaitSPINotBusy();

	while(SPI_SO_BUFF(0) & BIT0)
		ForceDelay1ms(1);
	FlashReadSR();
	return;
}
void FlashWritePage(DWORD wAddr)
{
	
	if(g_ucFlashSelect == SPI1 && GetFlashInfo(g_ucFlashSelect).Type == NAND_FLASH)
		NandFlashWritePage(wAddr);
	else
		NorFlashWritePage(wAddr);
	return;

}
void NandFlashWritePage(DWORD wAddr)
{
	DWORD CmdAddr = NandFlashAddrCal(PROGRAM_LOAD,wAddr);
	
	SPI_SI_BUFF(0) = PROGRAM_LOAD;
	SPI_SI_BUFF(1) = (CmdAddr >> 8) & 0xFF;
	SPI_SI_BUFF(2) = CmdAddr & 0xFF;
	SPI_SI_BUFF(3) = 0;// to be replaced

	SET_WBF_SIZE(4, 0, 0);
	SET_RBF_SIZE(0, 0, 0);

	SPI_CTRL0 = SPI_CTRL_VALUE;
	SPI_CTRL1 = 0;

	SPI_TRIGGER = 0x01;
	return;
}

void NorFlashWritePage(DWORD wAddr)
{
	SPI_SI_BUFF(0) = BYTE_WRITE;	
	SPI_SI_BUFF(1) = (wAddr>>16)& 0xff;
	SPI_SI_BUFF(2) = ( wAddr >> 8 ) & 0xff;
    SPI_SI_BUFF(3)   = wAddr&0x0ff;
	SPI_SI_BUFF(4) = 0 ; // to be replaced

	SET_WBF_SIZE(5, 0, 0);
	SET_RBF_SIZE(0, 0, 0);
	
	SPI_CTRL0 = SPI_CTRL_VALUE;
	SPI_CTRL1 = 0;

	SPI_TRIGGER = 0x01;
	//drvFlashWaitSPINotBusy();
	return;
}

void NandFlashPageProgram (Bool bDoWP, DWORD wAddr, BYTE *data, DWORD count)
{
	DWORD i = 0;
	BYTE buffindex=0;
	DWORD CmdAddr;
	if (g_ucFlashSelect != SPI1 || GetFlashInfo(g_ucFlashSelect).Type != NAND_FLASH)
		return;
	
    FlashDisableWP( wAddr ); 

	
	SPI_SI_BUFF(buffindex) = WRITE_ENABLE;
	buffindex++;

	CmdAddr = NandFlashAddrCal(PROGRAM_LOAD,wAddr);
	SPI_SI_BUFF(buffindex) = PROGRAM_LOAD;
	buffindex++;
	SPI_SI_BUFF(buffindex) = ((CmdAddr>>8)&0x1F);
	buffindex++;
	SPI_SI_BUFF(buffindex) = (CmdAddr&0xFF);
	buffindex++;
	while(i<count && buffindex < WBF_SIZE)
	{	
		SPI_SI_BUFF(buffindex) = *(data + (i++));
		buffindex++;
	}	
		
	SPI_CTRL0	   = SPI_CTRL_VALUE; //ctrl en FSP/reset
	SPI_CTRL1	   = EN_2ND_CMD;		
	SET_WBF_SIZE(1, (buffindex-1), 0);
	SET_RBF_SIZE(0, 0, 0);
	SPI_TRIGGER    = 0x01; //start flash programming
	drvFlashWaitSPINotBusy();

	// sw cz
	if(g_ucFlashSelect == SPI0)
		msWriteByte(REG_000A14, 0x01);
	else
	{
		//msWriteByte(0x000E14, 0x03);
		//ForceDelay1ms(1);
		msWriteByte(0x000E14, 0x01);
	}

	while(i<count)
	{
		buffindex = 0;
		while(i<count && buffindex < WBF_SIZE)
		{	
			SPI_SI_BUFF(buffindex) = *(data + (i++));
			buffindex ++;
		}	
    SPI_CTRL0       = SPI_CTRL_VALUE; //ctrl en FSP/reset
		SPI_CTRL1	   = 0;		
		SET_WBF_SIZE(buffindex, 0, 0);
		SET_RBF_SIZE(0, 0, 0);
    SPI_TRIGGER     = 0x01; //start flash programming
		drvFlashWaitSPINotBusy();
	}
	//disable sw cz
	if(g_ucFlashSelect == SPI0)
		msWriteByte(REG_000A14, 0x00);
	else
		msWriteByte(0x000E14, 0x00);

	// Step2: Get Feature

	// Step3: Program Execute (10, Raw addr[0:23])

	CmdAddr = NandFlashAddrCal(PROGRAM_EXECUTE,wAddr);
	SPI_SI_BUFF(0)  = PROGRAM_EXECUTE;
	SPI_SI_BUFF(1)  = ((CmdAddr>>16)&0xFF);
	SPI_SI_BUFF(2)  = ((CmdAddr>>8)&0xFF);
	SPI_SI_BUFF(3)  = (CmdAddr&0xFF);

	SPI_SI_BUFF(4)  = GET_FEATURE;
	SPI_SI_BUFF(5)  = 0xC0;

	SPI_CTRL0       = SPI_CTRL_VALUE;
	SPI_CTRL1       = (RDSR_IN_2ND|EN_2ND_CMD|EN_AUTO_CHECK);

	SET_WBF_SIZE(4,2,0);
	SET_RBF_SIZE(0,1,0);

	SPI_TRIGGER     = 0x01;
    drvFlashWaitSPINotBusy();

	while(SPI_SO_BUFF(0) & BIT0)
	{
		ForceDelay1ms(1);
	}
	if(bDoWP)
		FlashEnableWP();
	return;
}
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
void NorFlash_WriteTbl( Bool bDoWP, DWORD wAddr, BYTE *buffer, DWORD count )
{
    unsigned int i = 0;
/*
    if( wAddr < FDATA_START_ADDR )
        return;
*/
    if( bDoWP )
        FlashDisableWP( wAddr ); // 2005/5/11 上午 10:19:39 by keng
    //MCU_CACHE_CTL = 0x00;
    while( count > 0 )
    {
        FlashDisableStausProtect_MXIC2026();

        // 1st
        SPI_SI_BUFF(0)   = WREN;
        // 2nd
        SPI_SI_BUFF(1) = BYTE_WRITE;
        SPI_SI_BUFF(2) = (wAddr>>16)&0x0ff; //(wAddr>>16)&0x0ff;
        SPI_SI_BUFF(3) = ( wAddr >> 8 ) & 0x0ff;
        SPI_SI_BUFF(4) = wAddr & 0x0ff;
        SPI_SI_BUFF(5) = *( buffer + ( i++ ) );
        // 3rd
        SPI_SI_BUFF(6)   = RDSR;

        SPI_CTRL0       = SPI_CTRL_VALUE; //ctrl en FSP/reset
        SPI_CTRL1       = (EN_2ND_CMD|EN_3RD_CMD|RDSR_IN_3RD|EN_AUTO_CHECK);
        SET_WBF_SIZE(1, 5, 1);
        SET_RBF_SIZE(0, 0, 1);
        SPI_TRIGGER     = 0x01; //start flash programming
        drvFlashWaitSPINotBusy();

        /*
        FlashWriteEnable();
        SPI_SI_BUFF(0) = BYTE_WRITE;
        SPI_SI_BUFF(1) = TOTAL_BANK_NUM - 1; //(wAddr>>16)&0x0ff;
        SPI_SI_BUFF(2) = ( wAddr >> 8 ) & 0x0ff;
        SPI_SI_BUFF(3) = wAddr & 0x0ff;
        SPI_SI_BUFF(4) = *( buffer + ( i++ ) );
        SPI_CTRL = 0xFC;
        */

        wAddr += 1;
        count -= 1;

    }
    //MCU_CACHE_CTL = 0x20;


    if( bDoWP )
        FlashEnableWP();// 2005/5/11 上午 10:19:43 by keng
}
void Flash_WriteTbl( Bool bDoWP, DWORD wAddr, BYTE *buffer, DWORD count )
{
	
	if(g_ucFlashSelect == SPI1 && GetFlashInfo(g_ucFlashSelect).Type == NAND_FLASH)
	{
		DWORD StartAddr;
		DWORD ProgSize ;
		DWORD bufferindex = 0;
		if(buffer == NULL)
			return;
		while(count)
		{
			StartAddr = (wAddr % GetFlashInfo(SPI1).PageSize);
			ProgSize = ((StartAddr+count)<GetFlashInfo(SPI1).PageSize)?count : GetFlashInfo(SPI1).PageSize-StartAddr;

			NandFlashPageProgram(bDoWP,wAddr,buffer+bufferindex,ProgSize);
			wAddr += ProgSize;
			count -= ProgSize;
			bufferindex += ProgSize;
		}
	}
	else
		NorFlash_WriteTbl(bDoWP,wAddr,buffer,count);
	return;

}

void Flash_MoveTbl( Bool bDoWP, DWORD wSrcAddr, DWORD wDestAddr, DWORD count )
{
    BYTE ucTemp;
/*
    if( wDestAddr < FDATA_START_ADDR )
        return;
*/
	if(g_ucFlashSelect == SPI1 && GetFlashInfo(g_ucFlashSelect).Type == NAND_FLASH)
	{
		return; // TBD
	}
    if( bDoWP )
        FlashDisableWP( wDestAddr ); // 2005/5/11 上午 10:19:39 by keng
    while( count > 0 )
    {
        ucTemp = FlashReadByte( wSrcAddr );
        FlashDisableStausProtect_MXIC2026();

        // 1st
        SPI_SI_BUFF(0)   = WREN;
        // 2nd
        SPI_SI_BUFF(1) = BYTE_WRITE;
        SPI_SI_BUFF(2) = (wDestAddr>>16)&0x0ff;
        SPI_SI_BUFF(3) = ( wDestAddr >> 8 ) & 0x0ff;
        SPI_SI_BUFF(4) = wDestAddr & 0x0ff;
        SPI_SI_BUFF(5) = ucTemp;
        // 3rd
        SPI_SI_BUFF(6)   = RDSR;

        SPI_CTRL0       = SPI_CTRL_VALUE; //ctrl en FSP/reset
        SPI_CTRL1       = (EN_2ND_CMD|EN_3RD_CMD|RDSR_IN_3RD|EN_AUTO_CHECK);
        SET_WBF_SIZE(1, 5, 1);
        SET_RBF_SIZE(0, 0, 1);
        SPI_TRIGGER     = 0x01; //start flash programming
        drvFlashWaitSPINotBusy();

        /*
        FlashWriteEnable();
        SPI_SI_BUFF(0) = BYTE_WRITE;
        SPI_SI_BUFF(1) = TOTAL_BANK_NUM - 1; //(wDestAddr>>16)&0x0ff; // bank 3
        SPI_SI_BUFF(2) = ( wDestAddr >> 8 ) & 0x0ff;
        SPI_SI_BUFF(3) = wDestAddr & 0x0ff;
        SPI_SI_BUFF(4) = ucTemp;
        SPI_CTRL = 0xFC;
        */

        wSrcAddr += 1;
        wDestAddr += 1;
        count -= 1;
    }
    if( bDoWP )
        FlashEnableWP();// 2005/5/11 上午 10:19:43 by keng
}
//////////////////////////////////////////////////////////////////////////////
// <DRV><Description>: Read a table from flash
// <Parameter>:     -   <Flow>  -   <Description>
// ---------------------------------------------------------------------------
//  wAddr   -   In  -   Flash Address
//  buffer  -   Out -   Data point of the table
//  count   -   In  -   Number of BYTE data
//////////////////////////////////////////////////////////////////////////////
void Flash_ReadTbl( DWORD wAddr, BYTE *buffer, DWORD count )
{
	if((g_ucFlashSelect == SPI1) && (GetFlashInfo(g_ucFlashSelect).Type == NAND_FLASH))
	{
		DWORD StartAddr;
		DWORD ReadSize ;
		DWORD bufferindex = 0;
		while(count>0)
		{	
			StartAddr = (wAddr % GetFlashInfo(SPI1).PageSize);
			ReadSize = ((StartAddr+count)<GetFlashInfo(SPI1).PageSize)?count : GetFlashInfo(SPI1).PageSize-StartAddr;
			NandFlashReadPage(wAddr,(buffer+bufferindex),ReadSize);
			wAddr += ReadSize;
			count -= ReadSize;
			bufferindex += ReadSize;
		}

		return;
	}
	else
	{
    DWORD i;
    for(i = 0; i < count; i++)
#if FLASH_READ_BYTE_BY_CODE_POINTER
        *( buffer + i ) = xfr_FlashCode[wAddr+i];
#else
        *(buffer+i) = FlashReadByte(wAddr+i);
#endif
    }
}

///////////////////////////////////////////////////////////////////////////////
// <DRV><Description>: Flash erase function
// <Parameter>:  -  <Flow>  -  <Description>
//-----------------------------------------------------------------------------
//  bDoWP   -   In  -   Doing FlashDisableWP at start and FlashEnableWP at end
//  wAddr   -   In  -   Erase 4K BYTE sector address
///////////////////////////////////////////////////////////////////////////////
void FlashSectorErase( Bool bDoWP, DWORD wAddr )
{
	if((g_ucFlashSelect == SPI1) && (GetFlashInfo(g_ucFlashSelect).Type == NAND_FLASH))
		return; // NAND use block erase
    if( bDoWP )
        FlashDisableWP( wAddr );

    // 1st
    SPI_SI_BUFF(0)   = WREN;
    // 2nd
    if( g_ucFlashID == FLASH_SST )
        SPI_SI_BUFF(1) = SST_ERASE_SECTOR;
    else if( g_ucFlashID == FLASH_PMC )
        SPI_SI_BUFF(1) = PMC_ERASE_SECTOR;
    else if( g_ucFlashID == FLASH_MXIC )
        SPI_SI_BUFF(1) = MXIC_ERASE_SECTOR;
    else if( g_ucFlashID == FLASH_EON )
        SPI_SI_BUFF(1) = EON_ERASE_SECTOR;
    else if( g_ucFlashID == FLASH_AMIC )
        SPI_SI_BUFF(1) = AMIC_ERASE_SECTOR;
    else if( g_ucFlashID == FLASH_WINBOND )
        SPI_SI_BUFF(1) = WINBOND_ERASE_SECTOR;
    else if( g_ucFlashID == FLASH_SPANSION)
        SPI_SI_BUFF(1) = SPANSION_SECTOR_ERASE;
    else
        SPI_SI_BUFF(1) = SST_ERASE_SECTOR;

    SPI_SI_BUFF(2) = (wAddr>>16)&0x0ff;//TOTAL_BANK_NUM - 1; //(wAddr>>16)&0x0ff
    SPI_SI_BUFF(3) = ( wAddr >> 8 ) & 0x0ff;
    SPI_SI_BUFF(4) = wAddr & 0x0ff;
    // 3rd
    SPI_SI_BUFF(5)   = RDSR;

    SPI_CTRL0       = SPI_CTRL_VALUE; //ctrl en FSP/reset
    SPI_CTRL1       = (EN_2ND_CMD|EN_3RD_CMD|RDSR_IN_3RD|EN_AUTO_CHECK);
    SET_WBF_SIZE(1, 4, 1);
    SET_RBF_SIZE(0, 0, 1);
    SPI_TRIGGER     = 0x01; //start flash programming

    while(!(SPI_STATUS&BIT0))
        ForceDelay1ms(30);
    if (SPI_STATUS&BIT0)
        SPI_CLR_STATUS= 0x01;   //clr int

    if( bDoWP )
        FlashEnableWP();
}

void FlashWriteEnable(void)
{
    SPI_SI_BUFF(0) = WRITE_ENABLE;
    SPI_CTRL0 = SPI_CTRL_VALUE; //ctrl en FSP/reset
    SPI_CTRL1 = 0;
    SET_WBF_SIZE(1, 0, 0);
    SET_RBF_SIZE(0, 0, 0);
    SPI_TRIGGER = 0x01; //start flash programming
	drvFlashWaitSPINotBusy();
	return;

}
void FlashBlockErase ( BOOL bDoWP, DWORD wAddr)
{
	DWORD CmdAddr;
	if((g_ucFlashSelect != SPI1) || (GetFlashInfo(g_ucFlashSelect).Type != NAND_FLASH))
		return; // only for NAND flash
	// Disable WP
    FlashDisableWP( wAddr );
	

    /* 1st command */
    SPI_SI_BUFF(0) = WRITE_ENABLE;
    /* 2nd command */
	CmdAddr = NandFlashAddrCal(BLOCK_ERASE,wAddr);
	SPI_SI_BUFF(1) = BLOCK_ERASE;	
	SPI_SI_BUFF(2) = (CmdAddr>>16)&0xff;
	SPI_SI_BUFF(3) = ( CmdAddr>> 8 ) & 0xff;
	SPI_SI_BUFF(4) = CmdAddr & 0x0ff;
    /* 3rd command */
    SPI_SI_BUFF(5) = GET_FEATURE;
	SPI_SI_BUFF(6) = 0xC0;
		
    SPI_CTRL0 = SPI_CTRL_VALUE; //ctrl en FSP/reset
    SPI_CTRL1 = (EN_2ND_CMD|EN_3RD_CMD|RDSR_IN_3RD|EN_AUTO_CHECK);
    SET_WBF_SIZE(1, 4, 2);
    SET_RBF_SIZE(0, 0, 1);
    SPI_TRIGGER = 0x01; //start flash programming
	drvFlashWaitSPINotBusy();

    while(SPI_SO_BUFF(0) & BIT0)
        ForceDelay1ms(5);

	ForceDelay1ms(1); // For next program success.

	if(bDoWP)
		FlashEnableWP();
	
	return;	
}

void FlashChipErase( Bool bDoWP )
{
	if((g_ucFlashSelect == SPI1) && (GetFlashInfo(g_ucFlashSelect).Type == NAND_FLASH))
		return; // NAND use block erase
    if( bDoWP )
        FlashDisableWP( 0x00000000 );

    /* 1st command */
    SPI_SI_BUFF(0)   = WREN;
    /* 2nd command */
    if( g_ucFlashID == FLASH_MXIC )
        SPI_SI_BUFF(1) = MXIC_ERASE_CHIP;
    else
        SPI_SI_BUFF(1) = CHIP_ERASE;
    /* 3rd command */
    SPI_SI_BUFF(2)   = RDSR;

    SPI_CTRL0       = SPI_CTRL_VALUE; //ctrl en FSP/reset
    SPI_CTRL1       = (EN_2ND_CMD|EN_3RD_CMD|RDSR_IN_3RD|EN_AUTO_CHECK);
    SET_WBF_SIZE(1, 1, 1);
    SET_RBF_SIZE(0, 0, 1);
    SPI_TRIGGER     = 0x01; //start flash programming

    /* wait auto check (check flash WIP status), while WIP = 0, SPI done flag = 1 */
    while(!(SPI_STATUS&BIT0))
        ForceDelay1ms(500);
    if (SPI_STATUS&BIT0)
        SPI_CLR_STATUS= 0x01;   //clr int

    if( bDoWP )
        FlashEnableWP();
}

void FlashBurstWrite( Bool bDoWP, DWORD wAddr, BYTE *buffer, DWORD count, Bool bChipErase )
{
	if((g_ucFlashSelect == SPI1) && (GetFlashInfo(g_ucFlashSelect).Type == NAND_FLASH))
        return; // TBD
	else
        NorFlashBurstWrite(bDoWP,wAddr,buffer,count,bChipErase);
	return;
}
void NorFlashBurstWrite( Bool bDoWP, DWORD wAddr, BYTE *buffer, DWORD count, Bool bChipErase )
{
    unsigned int i = 0, k = 0, percentage = 0,  interval = 1;
    unsigned char j;
    unsigned long total_size, sector_size;
    DWORD dwStartAdr = wAddr;

#define USBDownloadSteps    20
#define Erase_Time    80
#define Program_Time    20

    interval = 100/USBDownloadSteps;
    total_size = count;

    if ( g_ucFlashID == FLASH_SPANSION && g_ucFlashDeviceID == S25FL0XXA)
    {
        sector_size = 65536;
        FLASH_PRINT("Sector size: 64K Bytes");
    }
    else
        sector_size = 4096;

    if (ProgessCB != NULL)
        ProgessCB(0);

    FLASH_PRINT("[MSFLASH] Erase   ");

    if( bChipErase )
    {
        FlashChipErase(TRUE);
    }
    else
    {
        k = 0;

        for (i = 0; i < ((count-1)/sector_size)+1; i++)
        {
            if( i > k )
            {
                k += (2*total_size+sector_size*Erase_Time/interval)/(2*sector_size*Erase_Time/interval);
                percentage++;

                if (ProgessCB != NULL)
                    ProgessCB(percentage*interval);
            }
            FlashSectorErase(TRUE, wAddr + (sector_size * i));
        }
    }

    FLASH_PRINT("\r\n[MSFLASH] Program ");
    if( bDoWP )
        FlashDisableWP( wAddr );

    i = 0;
    k = total_size - (total_size*interval/Program_Time);

    while( count > 0 )
    {
        if (count < k)
        {
            k -=  (total_size*interval/Program_Time);
            percentage++;

            if (ProgessCB != NULL  && (percentage*interval < 100))
                ProgessCB(percentage*interval);
        }

        if ( count >= FLASH_BURST_UNIT)    // unit: 256 bytes
        {
            /* 1st transfer: WREN + PROGRAM + ADDR (3bytes) + DATA (5 bytes) */
            // 1st command
            SPI_SI_BUFF(0) = WREN;
            // 2nd command
            SPI_SI_BUFF(1) = BYTE_WRITE;
            SPI_SI_BUFF(2) = (wAddr >> 16) & 0x0ff;
            SPI_SI_BUFF(3) = (wAddr >> 8) & 0x0ff;
            SPI_SI_BUFF(4) = wAddr & 0x0ff;
            SPI_SI_BUFF(5) = *( buffer + ( i++ ) );
            SPI_SI_BUFF(6) = *( buffer + ( i++ ) );
            SPI_SI_BUFF(7) = *( buffer + ( i++ ) );
            SPI_SI_BUFF(8) = *( buffer + ( i++ ) );
            SPI_SI_BUFF(9) = *( buffer + ( i++ ) );

            SPI_CTRL0       = SPI_CTRL_VALUE;   // ctrl en FSP/reset
            SPI_CTRL1       = (EN_2ND_CMD);
            SET_WBF_SIZE(1, 9, 0);
            SET_RBF_SIZE(0, 0, 0);
            SPI_TRIGGER     = 0x01;             // start flash programming
            drvFlashWaitSPINotBusy();
#if CHIP_ID == MST9U4
			msWriteByte(REG_000A14, 0x01);
#else
            msWriteByte(REG_000AD4, 0x01);      // [0]: enable CSZ software control, [1]: keep CSZ low
#endif            
            /* 25 times continuous data transfers: DATA (10 bytes) */
           for (j = 0; j < 25; j++)
           {
                SPI_SI_BUFF(0) = *( buffer + ( i++ ) );
                SPI_SI_BUFF(1) = *( buffer + ( i++ ) );
                SPI_SI_BUFF(2) = *( buffer + ( i++ ) );
                SPI_SI_BUFF(3) = *( buffer + ( i++ ) );
                SPI_SI_BUFF(4) = *( buffer + ( i++ ) );
                SPI_SI_BUFF(5) = *( buffer + ( i++ ) );
                SPI_SI_BUFF(6) = *( buffer + ( i++ ) );
                SPI_SI_BUFF(7) = *( buffer + ( i++ ) );
                SPI_SI_BUFF(8) = *( buffer + ( i++ ) );
                SPI_SI_BUFF(9) = *( buffer + ( i++ ) );
                SPI_CTRL0       = SPI_CTRL_VALUE;   //ctrl en FSP/reset
                SPI_CTRL1       = 0x00;
                SET_WBF_SIZE(10, 0, 0);
                SPI_TRIGGER     = 0x01;             //start flash programming
                drvFlashWaitSPINotBusy();
            }

            SPI_SI_BUFF(0) = *( buffer + ( i++ ) );
            SPI_CTRL0       = SPI_CTRL_VALUE;   //ctrl en FSP/reset
            SPI_CTRL1       = 0x00;
            SET_WBF_SIZE(1, 0, 0);
            SPI_TRIGGER     = 0x01;             //start flash programming
            drvFlashWaitSPINotBusy();
#if CHIP_ID == MST9U4
			msWriteByte(REG_000A14, 0x00);
#else
            //msWriteByte(REG_000AD4, 0x11);  // disable CSZ software control
            msWriteByte(REG_000AD4, 0x00);  // disable CSZ software control
#endif
            SPI_SI_BUFF(0) = RDSR;
            SPI_SI_BUFF(1) = RDSR;
            SPI_SI_BUFF(2) = RDSR;
            SPI_CTRL0       = SPI_CTRL_VALUE; //ctrl en FSP/reset
            SPI_CTRL1       = (EN_2ND_CMD|EN_3RD_CMD|RDSR_IN_3RD|EN_AUTO_CHECK);
            SET_WBF_SIZE(1, 1, 1);
            SET_RBF_SIZE(1, 1, 1);
            SPI_TRIGGER     = 0x01; //start flash programming
            drvFlashWaitSPINotBusy();

            count -= FLASH_BURST_UNIT;
            wAddr += FLASH_BURST_UNIT;
        }
        else    // < 256 bytes
        {
            Flash_WriteTbl(FALSE, wAddr, (buffer+i), count);
            count = 0;
        }
    }

    if( bDoWP )
        FlashEnableWP();

        if (ProgessCB != NULL)
            ProgessCB(100);

    FLASH_PRINT("\r\n[MSFLASH] Verify: ");

    for( i = 0 ; i < total_size; i++ )
		#if FLASH_READ_BYTE_BY_CODE_POINTER
        if( xfr_FlashCode[i+dwStartAdr] != buffer[i] )
		#else
		if( FlashReadByte(i+dwStartAdr) != buffer[i])
		#endif
            break;

    if( i == total_size )
    {
        FLASH_PRINT("OK!\r\n");
        hw_ClrAmberLed();
        hw_SetGreenLed();
	 ForceDelay1ms(300);
        hw_ClrGreenLed();
	 ForceDelay1ms(300);
        hw_SetGreenLed();
	 ForceDelay1ms(300);
        hw_ClrGreenLed();
	 ForceDelay1ms(300);
        hw_SetGreenLed();
	 ForceDelay1ms(300);
        hw_ClrGreenLed();
	 ForceDelay1ms(300);
        hw_SetGreenLed();

		
#if USB_Updata_FW_Enable		
	 USBUpdataFwStatus=USB_Pass;	
#endif
    }
    else
    {
        FLASH_PRINT("FAIL!\r\n");
        hw_ClrGreenLed();
        hw_SetAmberLed();
#if USB_Updata_FW_Enable		
	 USBUpdataFwStatus=USB_Fail;	
#endif
		
    }
}


void FlashProgressFuncAttach( pProgessCB pShowOSD)
{
	ProgessCB = pShowOSD;
}

#if USB_Updata_FW_Enable	
void DrawOsdMenuItem_USB(void);
#endif

void FlashUpdateFW( Bool bDoWP, DWORD wAddr, BYTE *buffer, DWORD count, Bool bChipErase )
{
    MDrv_Sys_DisableWatchDog();
    msWriteByte(REG_000393, 0x5F);      /* Stop 51 */
    msWriteBit(REG_00038A, 0, BIT6);
    msWriteByte(REG_000AE4, 0x00);      /* Set SPI to normal mode */

    FlashBurstWrite(bDoWP, wAddr, buffer, count, bChipErase);

  #if USB_Updata_FW_Enable		
    DrawOsdMenuItem_USB();
  #endif		
   

    FLASH_PRINT("[MSFLASH] Please Reboot The System!\r\n");
    while(1);
}

DWORD FlashRead4Byte( DWORD wAddr)
{
    return ((( DWORD )FlashReadByte( wAddr+3 ) ) << 24 ) | ((( DWORD )FlashReadByte( wAddr+2 ) ) << 16 ) | ((( DWORD )FlashReadByte( wAddr+1 ) ) << 8 ) | FlashReadByte( wAddr );
}

#endif//USEFLASH


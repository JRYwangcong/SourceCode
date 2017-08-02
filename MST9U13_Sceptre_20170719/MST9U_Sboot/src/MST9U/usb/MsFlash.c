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
//#include "Global.h"
//#include "MsOS.h"
//#include "ms_reg.h"
//#include "Ms_rwreg.h"
//#include "SysInit.h"
//#include "menudef.h"
//#include "msOSD.h"
//#include "MDebug.h"
//#include "Common.h"
//#include "misc.h"
//#include "Mcu.h"
#include "msflash.h"
//#include "GPIO_DEF.h"
//#include "halRwreg.h"
#include "../drvGlobal.h"

#define _FLASH_CODE_VERSION_    0x200

Bool xdata bFlashWriteFactory = FALSE;
Bool xdata bFlashEraseBufferFinish = FALSE; // // jeff add it in 2009 0907
BYTE xdata g_ucFlashDeviceID = 0;
static pProgessCB ProgessCB = NULL;

///////////////////////////////////////////////////////////////////////////////
// <DRV><Description>: Check current MsFlash.c code version
// <Returns> : MsFlash.c version number
///////////////////////////////////////////////////////////////////////////////
void drvFlashWaitSPINotBusy(void);
void drvFlashWaitSPINotBusy(void)
{
    BYTE u8Retry=100;
    while(!(SPI_STATUS&BIT0)&&u8Retry--)
        MDrv_SysDelayUs(1000);
    if (SPI_STATUS&BIT0)
        SPI_CLR_STATUS= 0x01; //clr int
    //printf("u8Retry=%d",u8Retry);
}

///////////////////////////////////////////////////////////////////////////////
// <DRV><Description>: Return true when flash type is PMC .
///////////////////////////////////////////////////////////////////////////////
Bool FlashTypeIs_PMC( void );
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
Bool FlashTypeIs_SST( void );
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
Bool FlashTypeIs_MXIC( void );
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
Bool FlashTypeIs_EON( void );
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
Bool FlashTypeIs_AMIC( void );
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
Bool FlashTypeIs_WINBOND( void );
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

    //printf("W9F-2:%x\r\n",SPI_SO_BUFF(2));

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

Bool FlashTypeIs_SPANSION( void );
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

BYTE ReadFlashUniqueID_Winbond(void);
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

BYTE ReadFlashDeviceID_MXIC( void );
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
///////////////////////////////////////////////////////////////////////////////
// <DRV><Description>: Read flash Manufacturer ID include PMC, MXIC, EON, and SST.
//                Return FLASH_UNKNOW if not found
// <Returns> : Manufacturer ID.
///////////////////////////////////////////////////////////////////////////////
BYTE ReadFlashID( void );
BYTE ReadFlashID( void )
{
    BYTE ucID;

    //FlashVersionCheck();

    if( FlashTypeIs_PMC() )
        ucID = FLASH_PMC;
    else if( FlashTypeIs_MXIC() )
    {
        ucID = ReadFlashDeviceID_MXIC();
        if( ucID == 0x03 )
            g_ucFlashDeviceID = MX_2026;
        else
            g_ucFlashDeviceID = MX_2025;
        ucID = FLASH_MXIC;
        printf( "   g_ucFlashDeviceID[%x]\r\n", g_ucFlashDeviceID );
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
        printf( "   WinbondUniqueID[%x]\r\n", g_ucFlashDeviceID );
    }
    else if ( FlashTypeIs_SPANSION() )
        ucID = FLASH_SPANSION;
    else
        ucID = FLASH_UNKNOW;

    return ucID;
}

///////////////////////////////////////////////////////////////////////////////
// <DRV><Description>: Read flash status register.
//
// <Returns> : Flash status register.
///////////////////////////////////////////////////////////////////////////////
BYTE FlashReadSR( void );
BYTE FlashReadSR( void )
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
void FlashWriteSR( BYTE value );
void FlashWriteSR( BYTE value )
{
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
void FlashPMCUnprotectEnable( DWORD wAddr );
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
        MDrv_SysDelayUs(1000);

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
void FlashPMCUnprotectDisable( void );
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


void FlashDisableStausProtect_MXIC2026( void );
void FlashDisableStausProtect_MXIC2026( void )
{
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
void FlashEnableWP_EON(BYTE SRValue);
void FlashEnableWP_EON(BYTE SRValue)
{
    if(( SRValue & 0x94 ) != 0x94 )
    {
        //printf("  EnableWP_EON_SR[%x] W_94\r\n",SSR);
        hw_ClrFlashWP();
        MDrv_SysDelayUs(1000);
        FlashWriteSR( 0x94 );
    }
}
void FlashEnableWP_AMIC(BYTE SRValue);
void FlashEnableWP_AMIC(BYTE SRValue)
{
    if(( SRValue & 0xFC ) != 0xE8 )
    {
        //printf("  EnableWP_AMIC_SR[%x] E8_0\r\n",SSR);
        hw_ClrFlashWP();
        MDrv_SysDelayUs(1000);
        FlashWriteSR( 0xE8 );
        //printf("  EnableWP_AMIC_SR[%x] E8_1\r\n",SSR);
    }
}
void FlashEnableWP_WINBOND(BYTE SRValue);
void FlashEnableWP_WINBOND(BYTE SRValue)
{
    if(g_ucFlashDeviceID == W25X40BV)
    {
        if((SRValue&0xAC) != 0xAC)
        {
            hw_ClrFlashWP();
            MDrv_SysDelayUs(1000);
            FlashWriteSR( 0xAC ); // protect lower 256K(0~0x3FFFF)
        }
    }
    else if(g_ucFlashDeviceID == W25X21CL)
    {
        if((SRValue&0xE0) != 0xE0)
        {
            hw_ClrFlashWP();
            MDrv_SysDelayUs(1000);
            FlashWriteSR( 0xE0 ); // protect 0~0x3BFFF
        }
    }
    else if(g_ucFlashDeviceID == W25X20BV)
    {
        if((SRValue&0xA8) != 0xA8)
        {
            hw_ClrFlashWP();
            MDrv_SysDelayUs(1000);
            FlashWriteSR( 0xA8 ); // protect lower 128K(0~0x1FFFF)
        }
    }
    else if(g_ucFlashDeviceID == W25X10BV)
    {
        if((SRValue&0xA4) != 0xA4)
        {
            hw_ClrFlashWP();
            MDrv_SysDelayUs(1000);
            FlashWriteSR( 0xA4 ); // protect lower 64K(0~0x0FFFF)
        }
    }
    else
    {
        if((SRValue&0x8C) != 0x8C)
        {
            hw_ClrFlashWP();
            MDrv_SysDelayUs(1000);
            FlashWriteSR( 0x8C ); // protect all
        }
    }
}
void FlashEnableWP_OTHERS(BYTE SRValue);
void FlashEnableWP_OTHERS(BYTE SRValue)
{
    if(( SRValue & 0x8C ) != 0x8C )
    {
        //printf("  EnableWP__SR[%x] W_8C\r\n",SSR);
        hw_ClrFlashWP();
        MDrv_SysDelayUs(1000);
        FlashWriteSR( 0x8C );
    }
}

#define FlashDisableWP_PMC      FlashPMCUnprotectEnable
#define FlashDisableWP_MXIC_MX2026      FlashDisableStausProtect_MXIC2026
void FlashDisableWP_EON(BYTE SRValue);
void FlashDisableWP_EON(BYTE SRValue)
{
    hw_ClrFlashWP();
    MDrv_SysDelayUs(1000);
    if( bFlashWriteFactory )
    {
        //printf("  _EON_SR[%x] W_00\r\n",SSR);
        FlashWriteSR( 0x00 );
    }
    else
    {
        if(( SRValue & 0x94 ) != 0x94 )
        {
            //printf("  _EON_SR[%x] W_94\r\n",SSR);
            FlashWriteSR( 0x94 );
        }
    }
}
void FlashDisableWP_WINBOND_W25X21CL(void);
void FlashDisableWP_WINBOND_W25X21CL(void)
{
    // X21CL: // 0xC0: protect range 0~0x39FFF; 0xE0: 0~0x3BFFF
    // X41CL: // 0xC0: protect range 0~0x79FFF; 0xE0: 0~0x7BFFF

    hw_ClrFlashWP();
    MDrv_SysDelayUs(1000);
    if( bFlashWriteFactory )
    {
        FlashWriteSR( 0xC0 );
    }
    else
    {
        FlashWriteSR( 0xE0 );
    }
}

void FlashDisableWP_OTHERS(BYTE SRValue);
void FlashDisableWP_OTHERS(BYTE SRValue)
{
    hw_ClrFlashWP();
    MDrv_SysDelayUs(1000);
    if( SRValue != 0x00 )
    {
        FlashWriteSR( 0x00 );
    }
}

///////////////////////////////////////////////////////////////////////////////
// <DRV><Description>: Enable flash write protect
///////////////////////////////////////////////////////////////////////////////
void FlashEnableWP( void );
void FlashEnableWP( void )
{
    BYTE SSR;

    SSR = FlashReadSR();

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

///////////////////////////////////////////////////////////////////////////////
// <DRV><Description>: Disable flash write protect
///////////////////////////////////////////////////////////////////////////////
void FlashDisableWP( DWORD wAddr );
void FlashDisableWP( DWORD wAddr )
{
    BYTE SSR;
/*
    if( wAddr < FDATA_START_ADDR )
        return;
*/
    SSR = FlashReadSR();

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
void FlashWriteByte( Bool bDoWP, DWORD wAddr, BYTE value );
void FlashWriteByte( Bool bDoWP, DWORD wAddr, BYTE value )
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

#if 0//HDCPKEY_IN_Flash
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
///////////////////////////////////////////////////////////////////////////////
// <DRV><Description>: Read one byte from flash.
// <Returns> : Data value in the address
// <Parameter>:  -  <Flow>  -  <Description>
//-----------------------------------------------------------------------------
//  wAddr   -   In  -   Only in the same bank ot MsFlash.c
///////////////////////////////////////////////////////////////////////////////
#if FLASH_READ_BYTE_BY_CODE_POINTER
//#define xfr_FlashCode ((unsigned char  volatile *) 0xC0000000)
#define xfr_FlashCode ((unsigned char  volatile *) 0x00000000)
BYTE FlashReadByte( DWORD wAddr );
BYTE FlashReadByte( DWORD wAddr )
{
    return xfr_FlashCode[wAddr];
}
#else
BYTE FlashReadByte( DWORD wAddr );
BYTE FlashReadByte( DWORD wAddr )
{
    //1st
    SPI_SI_BUFF(0)   = BYTE_READ;
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
#endif
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
void Flash_WriteTbl( Bool bDoWP, DWORD wAddr, BYTE *buffer, DWORD count );
void Flash_WriteTbl( Bool bDoWP, DWORD wAddr, BYTE *buffer, DWORD count )
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
void Flash_MoveTbl( Bool bDoWP, DWORD wSrcAddr, DWORD wDestAddr, DWORD count );
void Flash_MoveTbl( Bool bDoWP, DWORD wSrcAddr, DWORD wDestAddr, DWORD count )
{
    BYTE ucTemp;
/*
    if( wDestAddr < FDATA_START_ADDR )
        return;
*/
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
void Flash_ReadTbl( DWORD wAddr, BYTE *buffer, DWORD count );
void Flash_ReadTbl( DWORD wAddr, BYTE *buffer, DWORD count )
{
    WORD i;
    for(i = 0; i < count; i++)
#if FLASH_READ_BYTE_BY_CODE_POINTER
        *( buffer + i ) = xfr_FlashCode[wAddr+i];
#else
        *(buffer+i) = FlashReadByte(wAddr+i);
#endif
}

///////////////////////////////////////////////////////////////////////////////
// <DRV><Description>: Flash erase function
// <Parameter>:  -  <Flow>  -  <Description>
//-----------------------------------------------------------------------------
//  bDoWP   -   In  -   Doing FlashDisableWP at start and FlashEnableWP at end
//  wAddr   -   In  -   Erase 4K BYTE sector address
///////////////////////////////////////////////////////////////////////////////
void FlashSectorErase( Bool bDoWP, DWORD wAddr );
void FlashSectorErase( Bool bDoWP, DWORD wAddr )
{
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
        MDrv_SysDelayUs(30000);
    if (SPI_STATUS&BIT0)
        SPI_CLR_STATUS= 0x01;   //clr int

    if( bDoWP )
        FlashEnableWP();
}

void FlashChipErase( Bool bDoWP );
void FlashChipErase( Bool bDoWP )
{
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
        MDrv_SysDelayUs(500000);
    if (SPI_STATUS&BIT0)
        SPI_CLR_STATUS= 0x01;   //clr int

    if( bDoWP )
        FlashEnableWP();
}

void FlashBurstWrite( Bool bDoWP, DWORD wAddr, BYTE *buffer, DWORD count, Bool bChipErase );
void FlashBurstWrite( Bool bDoWP, DWORD wAddr, BYTE *buffer, DWORD count, Bool bChipErase )
{
    unsigned int i = 0, k = 0, power = 1, percentage = 0;
    //unsigned char j;
    unsigned long total_size, sector_size;
    DWORD dwStartAdr = wAddr;

    total_size = count;

    if ( g_ucFlashID == FLASH_SPANSION )
        sector_size = 65536;
    else
        sector_size = 4096;

    if (ProgessCB != NULL)
        ProgessCB(0);

    printf("[MSFLASH] Erase   ");

    for (i = 0; i < 30; i++)
    {
        if( total_size >> i )
                power = (i+1);
    }

    if( bChipErase )
    {
        FlashChipErase(TRUE);
    }
    else
    {
        k = total_size/(sector_size*4);	//    2 bits for 4 steps

        for (i = 0; i < ((count-1)/sector_size)+1; i++)
        {
            if( i > k )
            {
                k += total_size/(sector_size*4);
                percentage++;

                if (ProgessCB != NULL)
                    ProgessCB(percentage*5);
            }
            FlashSectorErase(TRUE, wAddr + (sector_size * i));
        }
    }

    printf("\r\n[MSFLASH] Program ");
    if( bDoWP )
        FlashDisableWP( wAddr );

    i = 0;
    k = total_size - (total_size>>4);	// 4 bits for 16 steps

    while( count > 0 )
    {
        if( count < k )
        {
            k -= (total_size>>4);
            percentage++;

            if (ProgessCB != NULL)
                ProgessCB(percentage*5);
        }

        if ( count >= FLASH_BURST_UNIT)    // unit: 256 bytes
        {
#if 0
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
            MDrv_WriteByte(0x000AD4, 0x01);      // [0]: enable CSZ software control, [1]: keep CSZ low

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

            //msWriteByte(REG_000AD4, 0x11);  // disable CSZ software control
            MDrv_WriteByte(0x000AD4, 0x00);  // disable CSZ software control
            SPI_SI_BUFF(0) = RDSR;
            SPI_SI_BUFF(1) = RDSR;
            SPI_SI_BUFF(2) = RDSR;
            SPI_CTRL0       = SPI_CTRL_VALUE; //ctrl en FSP/reset
            SPI_CTRL1       = (EN_2ND_CMD|EN_3RD_CMD|RDSR_IN_3RD|EN_AUTO_CHECK);
            SET_WBF_SIZE(1, 1, 1);
            SET_RBF_SIZE(1, 1, 1);
            SPI_TRIGGER     = 0x01; //start flash programming
            drvFlashWaitSPINotBusy();
#else
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
            // 3rd command
            SPI_SI_BUFF(9) = RDSR;

            SPI_CTRL0       = SPI_CTRL_VALUE;   // ctrl en FSP/reset
            SPI_CTRL1       = (EN_2ND_CMD|EN_3RD_CMD|RDSR_IN_3RD|EN_AUTO_CHECK);
            SET_WBF_SIZE(1, 8, 1);
            SET_RBF_SIZE(0, 0, 1);
            SPI_TRIGGER     = 0x01;             // start flash programming
            drvFlashWaitSPINotBusy();
#endif

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

    printf("\r\n[MSFLASH] Verify: ");

    for( i = 0 ; i < total_size; i++ )
        if( xfr_FlashCode[i+dwStartAdr] != buffer[i] )
            break;

    if( i == total_size )
        printf("OK!\r\n");
    else
        printf("FAIL!\r\n");
}


void FlashProgressFuncAttach( pProgessCB pShowOSD);
void FlashProgressFuncAttach( pProgessCB pShowOSD)
{
	ProgessCB = pShowOSD;
}


void FlashUpdateFW( Bool bDoWP, DWORD wAddr, BYTE *buffer, DWORD count, Bool bChipErase );
void FlashUpdateFW( Bool bDoWP, DWORD wAddr, BYTE *buffer, DWORD count, Bool bChipErase )
{
    //MDrv_Sys_DisableWatchDog();
    MDrv_WriteByte(0x000393, 0x5F);      /* Stop 51 */
    MDrv_WriteRegBit(0x00038A, 0, BIT6);
    MDrv_WriteByte(0x000AE4, 0x00);      /* Set SPI to normal mode */

    FlashBurstWrite(bDoWP, wAddr, buffer, count, bChipErase);

    printf("[MSFLASH] System Reboot!\r\n");
    MDrv_Write4Byte(0x002C08, (  (((5) * MST_XTAL_CLOCK_HZ)) ));
    while(1);
}



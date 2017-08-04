#define _MSFLASH_C_
#include <intrins.h>
#include "types.h"
#include "board.h"
#include "misc.h"
#include "global.h"
#include "ms_reg.h"
#include "ms_rwreg.h"
#include "msflash.h"
#include "misc.h"
#include "Mcu.h"
#if DEBUG_ENABLE
#include "debug.h"
#endif

#define _FLASH_CODE_VERSION_    0x100

#define FLASH_DEBUG    0
#if FLASH_DEBUG
    #define FLASH_printData(str, value)   printData(str, value)
    #define FLASH_printMsg(str)           printMsg(str)
#else
    #define FLASH_printData(str, value)
    #define FLASH_printMsg(str)
#endif

Bool xdata bFlashWriteFactory=FALSE;
Bool xdata bFlashEraseBufferFinish=FALSE;  // // jeff add it in 2009 0907
BYTE xdata g_ucFlashDeviceID=0;
///////////////////////////////////////////////////////////////////////////////
// <DRV><Description>: Check current MsFlash.c code version
// <Returns> : MsFlash.c version number
///////////////////////////////////////////////////////////////////////////////
#if DEBUG_IOPORT
WORD FlashVersionCheck(void)
{
    BYTE ucTemp=0;
    if(ucTemp)
    {
        Flash_GetBufferAddr();
        Flash_AllocateBufferAddr();
        Flash_CheckBufferType();
        Flash_ClearModeSet();
        Flash_WriteFactorySet(&ucTemp, 0);
        Flash_ReadFactorySet(&ucTemp, 0);
        Flash_WriteMonitorSet(&ucTemp, 0);
        Flash_ReadMonitorSet(&ucTemp, 0);
        Flash_WriteMonitorSet2(&ucTemp, 0);
        Flash_ReadMonitorSet2(&ucTemp, 0);
        Flash_WriteModeSet(0, &ucTemp);
        Flash_ReadModeSet(0, &ucTemp);
    }
    return _FLASH_CODE_VERSION_;
}
#endif
///////////////////////////////////////////////////////////////////////////////
// <DRV><Description>: Clear SPI Buffer.
///////////////////////////////////////////////////////////////////////////////
#if 0
void CleanSPI_SI_BUF(void)
{
	BYTE i=0;

	for(i=0; i < 8; i ++)
		SPI_SI_BUF[i] = 0x00;
}
#endif
///////////////////////////////////////////////////////////////////////////////
// <DRV><Description>: Return true when flash type is PMC .
///////////////////////////////////////////////////////////////////////////////
Bool FlashTypeIs_PMC(void)
{
	//BYTE retry=10;
	BYTE ucID=0;

 	SPI_SI_BUF[0]=PMC_READ_ID;

 	SPI_SI_BUF[1]=0x00;
 	SPI_SI_BUF[2]=0x00;
 	SPI_SI_BUF[3]=0x00;

 	SPI_CTRL=0xBB;

 	//while((SPI_CTRL&BIT7)&&retry--)
  	//	ForceDelay1ms(1);
    ucID=SPI_SO_BUF[1];

    if(ucID == FLASH_PMC)
        return TRUE;
    return FALSE;
}
///////////////////////////////////////////////////////////////////////////////
// <DRV><Description>: Return true when flash type is SST.
///////////////////////////////////////////////////////////////////////////////
Bool FlashTypeIs_SST(void)
{
	//BYTE retry=10;
	BYTE ucID=0;

 	SPI_SI_BUF[0]=SST_READ_ID;

 	SPI_SI_BUF[1]=0x00;
 	SPI_SI_BUF[2]=0x00;
 	SPI_SI_BUF[3]=0x00;

 	SPI_CTRL=0xBD;

 	//while((SPI_CTRL&BIT7)&&retry--)
  	//	ForceDelay1ms(1);
    ucID=SPI_SO_BUF[3];

    if(ucID == FLASH_SST)
        return TRUE;
    return FALSE;
}
///////////////////////////////////////////////////////////////////////////////
// <DRV><Description>: Return true when flash type is MXIC.
///////////////////////////////////////////////////////////////////////////////
Bool FlashTypeIs_MXIC(void)
{
	//BYTE retry=10;
	BYTE ucID=0;

 	SPI_SI_BUF[0]=MXIC_READ_ID;

 	SPI_SI_BUF[1]=0x00;
 	SPI_SI_BUF[2]=0x00;
 	SPI_SI_BUF[3]=0x00;
 	SPI_CTRL=0xBB;

 	//while((SPI_CTRL&BIT7)&&retry--)
  	//	ForceDelay1ms(1);
    ucID=SPI_SO_BUF[0];

    if(ucID == FLASH_MXIC)
        return TRUE;
    return FALSE;
}
///////////////////////////////////////////////////////////////////////////////
// <DRV><Description>: Return true when flash type is EON.
///////////////////////////////////////////////////////////////////////////////
Bool FlashTypeIs_EON(void)
{
	//BYTE retry=10;
	BYTE ucID=0;

 	SPI_SI_BUF[0]=EON_READ_ID;

 	SPI_SI_BUF[1]=0x00;
 	SPI_SI_BUF[2]=0x00;
 	SPI_SI_BUF[3]=0x00;

 	SPI_CTRL=0xBB;

 	//while((SPI_CTRL&BIT7)&&retry--)
  	//	ForceDelay1ms(1);
    ucID=SPI_SO_BUF[0];

    if(ucID == FLASH_EON)
        return TRUE;
    return FALSE;
}
///////////////////////////////////////////////////////////////////////////////
// <DRV><Description>: Return true when flash type is AMIC.
///////////////////////////////////////////////////////////////////////////////
Bool FlashTypeIs_AMIC(void)
{
	//BYTE retry=10;
	BYTE ucID=0;

 	SPI_SI_BUF[0]=EON_READ_ID;

 	SPI_SI_BUF[1]=0x00;
 	SPI_SI_BUF[2]=0x00;
 	SPI_SI_BUF[3]=0x00;

 	SPI_CTRL=0xBB;

 	//while((SPI_CTRL&BIT7)&&retry--)
  	//	ForceDelay1ms(1);
    ucID=SPI_SO_BUF[0];

    if(ucID == FLASH_AMIC)
        return TRUE;
    return FALSE;
}
BYTE ReadFlashDeviceID_MXIC(void)
{
    //BYTE retry=10;
 	SPI_SI_BUF[0]=MXIC_RES;

 	SPI_SI_BUF[1]=(0>>16)&0x0ff;
 	SPI_SI_BUF[2]=(0>>8)&0x0ff;
 	SPI_SI_BUF[3]=0&0x0ff;

 	SPI_CTRL=0xBC;

 	//while((SPI_CTRL&BIT7)&&retry--)
  	//	ForceDelay1ms(1);

    return SPI_SO_BUF[3];
}
///////////////////////////////////////////////////////////////////////////////
// <DRV><Description>: Read flash Manufacturer ID include PMC, MXIC, EON, and SST.
//                Return FLASH_UNKNOW if not found
// <Returns> : Manufacturer ID.
///////////////////////////////////////////////////////////////////////////////
BYTE ReadFlashID(void)
{
    //BYTE retry=10;
    BYTE ucID;


 	//CleanSPI_SI_BUF();
    if(FlashTypeIs_PMC())
        ucID = FLASH_PMC;
    else if(FlashTypeIs_MXIC())
    {
        ucID = ReadFlashDeviceID_MXIC();
        if(ucID == 0x03)
            g_ucFlashDeviceID = MX_2026;
        else
            g_ucFlashDeviceID = MX_2025;
        ucID = FLASH_MXIC;
        FLASH_printData( "   g_ucFlashDeviceID[%x]",g_ucFlashDeviceID );
    }
    else if(FlashTypeIs_EON())
        ucID = FLASH_EON;
    else if(FlashTypeIs_SST())
        ucID = FLASH_SST;
    else if(FlashTypeIs_AMIC())
        ucID = FLASH_AMIC;
    else
        ucID = FLASH_UNKNOW;

    return ucID;
}

#if 1//USEFLASH //------------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
// <DRV><Description>: Read flash status register.
//
// <Returns> : Flash status register.
///////////////////////////////////////////////////////////////////////////////
BYTE FlashReadSR(void)
{
	//BYTE retry=10;

	//CleanSPI_SI_BUF();
	SPI_SI_BUF[0]=RDSR;
	SPI_SI_BUF[1]=0x00;

	SPI_CTRL=0xB9;

	//while((SPI_CTRL&BIT7)&&retry--)
	//	ForceDelay1ms(1);
	return SPI_SO_BUF[0];
}
///////////////////////////////////////////////////////////////////////////////
// <DRV><Description>: Flash Write enable
//Prior to any Byte-Program, AAI-Program, Sector-Erase, Block-Erase, or Chip-Erase operation, the Write-Enable (WREN)
//instruction must be executed.
//Wait WIP(SR[0]) clear then send WREN command
///////////////////////////////////////////////////////////////////////////////
void FlashWriteEnable(void)
{
    BYTE retry=10;
    while((FlashReadSR()&BIT0)&&retry--)
        ForceDelay1ms(1);

    //CleanSPI_SI_BUF();
    SPI_SI_BUF[0]=WREN;

    SPI_CTRL=0xF8;
    //retry=10;
    //while((SPI_CTRL&BIT7)&&retry--)
    //    ForceDelay1ms(1);
}
///////////////////////////////////////////////////////////////////////////////
// <DRV><Description>: Write value to flash status register
// <Parameter>:  -  <Flow>  -  <Description>
//-----------------------------------------------------------------------------
//  value  -    In    -   which value write to flash status register
///////////////////////////////////////////////////////////////////////////////
void FlashWriteSR(BYTE value)
{
	//BYTE retry=10;
    if(g_ucFlashID==FLASH_SST)
    {
    	//CleanSPI_SI_BUF();
    	SPI_SI_BUF[0]=EWSR;
    	SPI_SI_BUF[1]=WRSR;
    	SPI_SI_BUF[2]=value;
    	SPI_CTRL=0xC8;
    }
    else if(g_ucFlashID==FLASH_PMC)
    {
    	FlashWriteEnable();
    	//CleanSPI_SI_BUF();
    	SPI_SI_BUF[0]=WRSR;
    	SPI_SI_BUF[1]=value;
    	SPI_CTRL=0xF9;
    }
    else if(g_ucFlashID==FLASH_MXIC)
    {
    	FlashWriteEnable();
    	//CleanSPI_SI_BUF();
    	SPI_SI_BUF[0]=MXIC_WRSR;
    	SPI_SI_BUF[1]=value;
    	SPI_CTRL=0xF9;
    }
    else if(g_ucFlashID==FLASH_EON)
    {
    	FlashWriteEnable();
    	//CleanSPI_SI_BUF();
    	SPI_SI_BUF[0]=WRSR;
    	SPI_SI_BUF[1]=value;
    	SPI_CTRL=0xF9;
    }
    else if(g_ucFlashID==FLASH_AMIC)
    {
    	BYTE SSR = FlashReadSR();
        if(SSR&0x40)//_BIT6)
        {
            SPI_SI_BUF[0]=WREN;
            SPI_CTRL=0xF8;
            SPI_SI_BUF[0]=0x90;
            SPI_SI_BUF[1]=0x00;
            SPI_SI_BUF[2]=0x28;
            SPI_SI_BUF[3]=0x00;
            SPI_SI_BUF[4]=0x3B;
            SPI_SI_BUF[5]=0x00;
            SPI_SI_BUF[6]=0x28;
            SPI_SI_BUF[7]=0xFF;
            SPI_CTRL=0x9B;
            SPI_SI_BUF[0]=WRSR;
            SPI_SI_BUF[1]=value;
            SPI_CTRL=0xF9;
        }
        else
        {
            SPI_SI_BUF[0]=WREN;
            SPI_CTRL=0xF8;
            SPI_SI_BUF[0]=WRSR;
            SPI_SI_BUF[1]=value;
            SPI_CTRL=0xF9;
        }
    }
    else
    {
    	FlashWriteEnable();
    	//CleanSPI_SI_BUF();
    	SPI_SI_BUF[0]=WRSR;
    	SPI_SI_BUF[1]=value;
    	SPI_CTRL=0xF9;
    }
	//while((SPI_CTRL&BIT7)&&retry--)
	//	ForceDelay1ms(1);
}
///////////////////////////////////////////////////////////////////////////////
// <DRV><Description>: Only used in PMC type flash. To disable protect of specific sector.
//                     if wAddr < FDATA_FACTORY_ADDR will not unprotect
// <Parameter>:  <Description>
//  wAddr : Which sector address will be unportect
///////////////////////////////////////////////////////////////////////////////
void FlashPMCUnprotectEnable(WORD wAddr)
{
	//BYTE retry=10;
	if(wAddr < FDATA_START_ADDR)
        return;
	SPI_SI_BUF[0]=PMC_UNPROTECT;
	SPI_SI_BUF[1]=TOTAL_BANK_NUM-1;//(wAddr>>16)&0x0ff;
	SPI_SI_BUF[2]=(wAddr>>8)&0x0ff;
	SPI_SI_BUF[3]=wAddr&0x0ff;
	SPI_CTRL=0xBB;
	//retry=10;
	//while((SPI_CTRL&BIT7)&&retry--)
	//	ForceDelay1ms(1);
}
///////////////////////////////////////////////////////////////////////////////
// <DRV><Description>: Only used in PMC type flash. To disable un-protect of specific sector.
///////////////////////////////////////////////////////////////////////////////
void FlashPMCUnprotectDisable(void)
{
	//BYTE retry=10;
	SPI_SI_BUF[0]=PMC_PROTECT;
	SPI_CTRL=0xF8;
	//retry=10;
	//while((SPI_CTRL&BIT7)&&retry--)
	//	ForceDelay1ms(1);
}


void FlashDisableStausProtect_MXIC2026(void)
{
    if(g_ucFlashID == FLASH_MXIC && g_ucFlashDeviceID == MX_2026)
    {
        FlashWriteSR(0x7F);
        if(bFlashWriteFactory)
            FlashWriteSR(0x40);
        else
            FlashWriteSR(0x60);
    }
}
///////////////////////////////////////////////////////////////////////////////
// <DRV><Description>: Enable flash write protect
///////////////////////////////////////////////////////////////////////////////
void FlashEnableWP(void)
{
    BYTE SSR;

    SSR = FlashReadSR();

    if(g_ucFlashID==FLASH_PMC)
        FlashPMCUnprotectDisable();
    else if(g_ucFlashID == FLASH_EON)
    {
        if((SSR&0x94)!=0x94)
        {
            //FLASH_printData("  EnableWP_EON_SR[%x] W_94",SSR);
            hw_ClrFlashWP();
            Delay1ms(1);
            FlashWriteSR(0x94);
        }
    }
    else if(g_ucFlashID == FLASH_AMIC)
    {
        if((SSR&0xFC)!=0xE8)
        {
            //FLASH_printData("  EnableWP_AMIC_SR[%x] E8_0",SSR);
            hw_ClrFlashWP();
            Delay1ms(1);
            FlashWriteSR(0xE8);
            //FLASH_printData("  EnableWP_AMIC_SR[%x] E8_1",SSR);
        }
    }
    else if(g_ucFlashID == FLASH_MXIC && g_ucFlashDeviceID == MX_2026)
    {
        //SRAM type auto protect when write finish
    }
    else
    {
        if((SSR & 0x8C) != 0x8C)
    	{
            //FLASH_printData("  EnableWP__SR[%x] W_8C",SSR);
            hw_ClrFlashWP();
        	Delay1ms(1);
    	    FlashWriteSR(0x8C);
        }
    }
    //FlashWriteDisable();
	hw_SetFlashWP();

}
///////////////////////////////////////////////////////////////////////////////
// <DRV><Description>: Disable flash write protect
///////////////////////////////////////////////////////////////////////////////
void FlashDisableWP(WORD wAddr)
{
    BYTE SSR;

    SSR = FlashReadSR();
    hw_ClrFlashWP();
    Delay1ms(1);

    if(g_ucFlashID==FLASH_PMC)
    {
        FlashPMCUnprotectEnable(wAddr);
    }
    else if(g_ucFlashID==FLASH_EON)
    {
        if(bFlashWriteFactory)
        {
            //FLASH_printData("  _EON_SR[%x] W_00",SSR);
            FlashWriteSR(0x00);
        }
        else
        {
            if((SSR&0x94)!=0x94)
            {
                //FLASH_printData("  _EON_SR[%x] W_94",SSR);
                FlashWriteSR(0x94);
            }
        }
    }
    else if(g_ucFlashID==FLASH_AMIC)
    {
        ;
    }
    else if(g_ucFlashID==FLASH_MXIC && g_ucFlashDeviceID == MX_2026)
    {
        FlashDisableStausProtect_MXIC2026();
    }
    else
    {
        if(SSR != 0x00)
        {
            //FLASH_printData("  __SR[%x] W_00",SSR);
            FlashWriteSR(0x00);
        }
    }

}

///////////////////////////////////////////////////////////////////////////////
// <DRV><Description>: Flash erase function
// <Parameter>:  -  <Flow>  -  <Description>
//-----------------------------------------------------------------------------
//  bDoWP   -   In  -   Doing FlashDisableWP at start and FlashEnableWP at end
//  wAddr   -   In  -   Erase 4K BYTE sector address
///////////////////////////////////////////////////////////////////////////////
void FlashSectorErase(Bool bDoWP, WORD wAddr)
{
    //BYTE retry=10;
    bit bcachestatus = MCU_CACHE_CTL&BIT5;

	if(wAddr < FDATA_START_ADDR)
        return;

    if(bcachestatus)
        MCU_CACHE_CTL = 0x00;
    
    if(bDoWP)
        FlashDisableWP(wAddr);

    FlashWriteEnable();
    //CleanSPI_SI_BUF();
    if(g_ucFlashID==FLASH_SST)
        SPI_SI_BUF[0]=SST_ERASE_SECTOR;
    else if(g_ucFlashID==FLASH_PMC)
        SPI_SI_BUF[0]=PMC_ERASE_SECTOR;
    else if(g_ucFlashID==FLASH_MXIC)
        SPI_SI_BUF[0]=MXIC_ERASE_SECTOR;
    else if(g_ucFlashID==FLASH_EON)
        SPI_SI_BUF[0]=EON_ERASE_SECTOR;
    else if(g_ucFlashID==FLASH_AMIC)
        SPI_SI_BUF[0]=AMIC_ERASE_SECTOR;
    else
        SPI_SI_BUF[0]=SST_ERASE_SECTOR;
    //SPI_SI_BUF[0]=op_code;

    SPI_SI_BUF[1]=TOTAL_BANK_NUM-1;//(wAddr>>16)&0x0ff;
    SPI_SI_BUF[2]=(wAddr>>8)&0x0ff;
    SPI_SI_BUF[3]=wAddr&0x0ff;

    SPI_CTRL=0xFB;//0xFC;
    //while((FlashReadSR()&BIT0)&&retry--)
    //    ForceDelay1ms(4);

    if(bDoWP)
        FlashEnableWP();
	
    if(bcachestatus)
    {
        MCU_CACHE_CTL = 0x80;
        MCU_CACHE_CTL = 0x00;
        MCU_CACHE_CTL = 0x20;
    }

}

///////////////////////////////////////////////////////////////////////////////
// <DRV><Description>: Write one byte to flash
// <Parameter>:	-  <Flow>	-	<Description>
//-----------------------------------------------------------------------------
//  bDoWP   -   In  -   Doing FlashDisableWP at start and FlashEnableWP at end
//  wAddr   -   In  -   Flash Address to be write, high byte address will be (TOTAL_BANK_NUM-1)
//                      wAddr < FDATA_FACTORY_ADDR will be limit, so address < 0x3B000 will be limit
//  value   -   In  -   The value write to flash
///////////////////////////////////////////////////////////////////////////////
void FlashWriteByte(Bool bDoWP, WORD wAddr, BYTE value)
{
	//BYTE retry=10;
    bit bcachestatus = MCU_CACHE_CTL&BIT5;

    if(wAddr < FDATA_START_ADDR)
        return;

    if(bcachestatus)
        MCU_CACHE_CTL = 0x00;

    if(bDoWP)
	    FlashDisableWP(wAddr);
    else
        FlashDisableStausProtect_MXIC2026();
	FlashWriteEnable();
	//CleanSPI_SI_BUF();
    //MCU_CACHE_CTL = 0x00;
	SPI_SI_BUF[0]=BYTE_WRITE;
	SPI_SI_BUF[1]=TOTAL_BANK_NUM-1;//(wAddr>>16)&0x0ff;
	SPI_SI_BUF[2]=(wAddr>>8)&0x0ff;
	SPI_SI_BUF[3]=wAddr&0x0ff;
	SPI_SI_BUF[4]=value;
	SPI_CTRL=0xFC;
    //MCU_CACHE_CTL = 0x20;
	//retry=10;
	//while((SPI_CTRL&BIT7)&&retry--)
	//	ForceDelay1ms(1);

    if(bDoWP)
    	FlashEnableWP();
	//FLASH_printData("value=%x\n",value);
	//FLASH_printData("S4=%x",SPI_SI_BUF[4]);

    if(bcachestatus)
    {
        MCU_CACHE_CTL = 0x80;
        MCU_CACHE_CTL = 0x00;
        MCU_CACHE_CTL = 0x20;
    }
    
}
#if HDCPKEY_IN_Flash
void FlashHDCPWriteByte(WORD wAddr, BYTE value)
{
	//BYTE retry=10;
    bit bcachestatus = MCU_CACHE_CTL&BIT5;

    if(wAddr < FDATA_START_ADDR)
        return;

    if(bcachestatus)
        MCU_CACHE_CTL = 0x00;

	FlashDisableStausProtect_MXIC2026();
	FlashWriteEnable();
	SPI_SI_BUF[0]=BYTE_WRITE;
	SPI_SI_BUF[1]=TOTAL_BANK_NUM-1;//(wAddr>>16)&0x0ff;
	SPI_SI_BUF[2]=(wAddr>>8)&0x0ff;
	SPI_SI_BUF[3]=wAddr&0x0ff;
	SPI_SI_BUF[4]=value;
	SPI_CTRL=0xFC;

    if(bcachestatus)
    {
        MCU_CACHE_CTL = 0x80;
        MCU_CACHE_CTL = 0x00;
        MCU_CACHE_CTL = 0x20;
    }
    
}
#endif

///////////////////////////////////////////////////////////////////////////////
// <DRV><Description>: Read one byte from flash.
// <Returns> : Data value in the address
// <Parameter>:  -  <Flow>  -  <Description>
//-----------------------------------------------------------------------------
//  wAddr   -   In  -   Only in the same bank ot MsFlash.c
///////////////////////////////////////////////////////////////////////////////
#define xfr_FlashCode ((BYTE code*)0x0000)
BYTE FlashReadByte(WORD wAddr)
{
    return xfr_FlashCode[wAddr];
}
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
void Flash_WriteTbl(Bool bDoWP,WORD wAddr, BYTE *buffer, WORD count)
{
    BYTE i=0;
    //BYTE retry;
    bit bcachestatus = MCU_CACHE_CTL&BIT5;
    
    if(wAddr < FDATA_START_ADDR)
        return;

    if(bcachestatus)
        MCU_CACHE_CTL = 0x00;

    if(bDoWP)
        FlashDisableWP(wAddr);// 2005/5/11 ¤W¤È 10:19:39 by keng
    //MCU_CACHE_CTL = 0x00;
    while(count>0)
    {
        FlashDisableStausProtect_MXIC2026();
        FlashWriteEnable();
        SPI_SI_BUF[0]=BYTE_WRITE;
        SPI_SI_BUF[1]=TOTAL_BANK_NUM-1;//(wAddr>>16)&0x0ff;
        SPI_SI_BUF[2]=(wAddr>>8)&0x0ff;
        SPI_SI_BUF[3]=wAddr&0x0ff;
        SPI_SI_BUF[4]=*(buffer+(i++));
        SPI_CTRL=0xFC;
        //retry=10;
        //while((SPI_CTRL&BIT7)&&retry--)
        //    ForceDelay1ms(1);
        wAddr+=1;
        count-=1;
     
    }
    //MCU_CACHE_CTL = 0x20;


    if(bDoWP)
        FlashEnableWP();// 2005/5/11 ¤W¤È 10:19:43 by keng

    if(bcachestatus)
    {
        MCU_CACHE_CTL = 0x80;
        MCU_CACHE_CTL = 0x00;
        MCU_CACHE_CTL = 0x20;
    }
        
}
#if DISPLAY_CHECK_VOL
void FlashWriteCheckSumTbl(Bool bDoWP,WORD wAddr, BYTE *buffer, WORD count)
{
    BYTE i=0;
    bit bcachestatus = MCU_CACHE_CTL&BIT5;
    
    if(wAddr < 0x9000)//0x39000
        return;

    if(bcachestatus)
        MCU_CACHE_CTL = 0x00;

    if(bDoWP)
        FlashDisableWP(wAddr);// 2005/5/11 ¤
    while(count>0)
    {
        FlashDisableStausProtect_MXIC2026();
        FlashWriteEnable();
        SPI_SI_BUF[0]=BYTE_WRITE;
        SPI_SI_BUF[1]=TOTAL_BANK_NUM-1;//(wAddr>>16)&0x0ff;
        SPI_SI_BUF[2]=(wAddr>>8)&0x0ff;
        SPI_SI_BUF[3]=wAddr&0x0ff;
        SPI_SI_BUF[4]=*(buffer+(i++));
        SPI_CTRL=0xFC;
        wAddr+=1;
        count-=1;
    }

    if(bDoWP)
        FlashEnableWP();// 2005/5/11 ¤W¤È 10:19:43 by keng

    if(bcachestatus)
    {
        MCU_CACHE_CTL = 0x80;
        MCU_CACHE_CTL = 0x00;
        MCU_CACHE_CTL = 0x20;
    }
        
}
#endif

void Flash_MoveTbl(Bool bDoWP, WORD wSrcAddr, WORD wDestAddr, WORD count)
{
    BYTE ucTemp;
	//BYTE retry;
    bit bcachestatus = MCU_CACHE_CTL&BIT5;
    
    if(bcachestatus)
        MCU_CACHE_CTL = 0x00;
        
    if(bDoWP)
        FlashDisableWP(wDestAddr);// 2005/5/11 ¤W¤È 10:19:39 by keng
    while(count>0)
    {
        ucTemp = FlashReadByte(wSrcAddr);
        FlashDisableStausProtect_MXIC2026();
        FlashWriteEnable();
        SPI_SI_BUF[0]=BYTE_WRITE;
        SPI_SI_BUF[1]=TOTAL_BANK_NUM-1;//(wDestAddr>>16)&0x0ff; // bank 3
        SPI_SI_BUF[2]=(wDestAddr>>8)&0x0ff;
        SPI_SI_BUF[3]=wDestAddr&0x0ff;
        SPI_SI_BUF[4]=ucTemp;
        SPI_CTRL=0xFC;
        //retry=10;
        //while((SPI_CTRL&BIT7)&&retry--)
        //    ForceDelay1ms(1);
        wSrcAddr+=1;
        wDestAddr+=1;
        count-=1;
    }
    if(bDoWP)
        FlashEnableWP();// 2005/5/11 ¤W¤È 10:19:43 by keng

    if(bcachestatus)
    {
        MCU_CACHE_CTL = 0x80;
        MCU_CACHE_CTL = 0x00;
        MCU_CACHE_CTL = 0x20;
    }
        
}
//////////////////////////////////////////////////////////////////////////////
// <DRV><Description>: Read a table from flash
// <Parameter>: 	- 	<Flow>	-	<Description>
// ---------------------------------------------------------------------------
//  wAddr   -   In  -   Flash Address
//  buffer  -   Out -   Data point of the table
//  count   -   In  -   Number of BYTE data
//////////////////////////////////////////////////////////////////////////////
void Flash_ReadTbl(WORD wAddr, BYTE *buffer, WORD count)
{	BYTE i;
	for(i = 0; i < count; i++)
		*(buffer+i) = xfr_FlashCode[wAddr+i];
}

#if 0//HKC_ColorTemp_Adjust

void CleanSPI_SI_BUF(void)
{
	BYTE i = 0;
	
	for (i = 0; i < 8; i++)
		SPI_SI_BUF[i] = 0x00;
}

void FlashWriteByte1(DWORD address, BYTE value)
{
	BYTE retry = 3;
	
	FlashWriteEnable();
	CleanSPI_SI_BUF();
	SPI_SI_BUF[0] = BYTE_WRITE;
	SPI_SI_BUF[1] = (address >> 16) &0x0ff;
	SPI_SI_BUF[2] = (address >> 8) &0x0ff;
	SPI_SI_BUF[3] = address &0x0ff;
	SPI_SI_BUF[4] = value;
	SPI_CTRL = 0xFC;
	retry = 5;
	while ((SPI_CTRL &BIT7) && retry--)
		ForceDelay1ms(1);
}

BYTE FlashReadByte1(DWORD address)
{
	BYTE retry = 10;
	
	CleanSPI_SI_BUF();
	SPI_SI_BUF[0] = READ;
	SPI_SI_BUF[1] = (address >> 16) &0x0ff;
	SPI_SI_BUF[2] = (address >> 8) &0x0ff;
	SPI_SI_BUF[3] = address &0x0ff;
	SPI_CTRL = 0xFC;
	while ((SPI_CTRL &BIT7) && retry--)
		ForceDelay1ms(1);
	
	return SPI_SO_BUF[3];
}

void FlashWriteSR1(BYTE value)
{
	BYTE retry = 10;
	//BYTE retry=2;

	if (g_FlashID == 0x11) //for EON flash	( ********SST Flash  use******)
	// if (g_FlashID == 0xBF)
	// SST flash
	{
		CleanSPI_SI_BUF();
		SPI_SI_BUF[0] = EWSR;
		SPI_SI_BUF[1] = WRSR;
		SPI_SI_BUF[2] = value;
		SPI_CTRL = 0xC8;
	}
	else
	{
		FlashWriteEnable();
		CleanSPI_SI_BUF();
		SPI_SI_BUF[0] = WRSR;
		SPI_SI_BUF[1] = value;
		SPI_CTRL = 0xF9;
	}
	//#endif
	while ((SPI_CTRL &BIT7) && retry--)
		ForceDelay1ms(1);
}



void FlashDisableWP1(void)
{
	hw_ClrFlashWP();
	ForceDelay1ms(2);
	FlashWriteSR1(0x00);
}



void Flash_WriteTbl1(DWORD addr, BYTE *buffer, BYTE count)
{
	BYTE i;
	for(i = 0; i < count; i++)
		FlashWriteByte1(addr+i, *(buffer+i));
}

void Flash_ReadTbl1(DWORD addr, BYTE *buffer, BYTE count)
{	BYTE i;
	for(i = 0; i < count; i++)
		*(buffer+i) = FlashReadByte1(addr+i);
}

void FlashErase(DWORD address)// 
{
	BYTE retry;
	
#define	op_code		retry

	/*if (g_FlashID == 0x11) //for EON flash	( ********SST Flash  use******)
	//if (g_FlashID == 0xBF)// SST Flash
		op_code=SST_ERASE_SECTOR; // */
	if((g_FlashID == 0x11) || (g_FlashID==0x12) || (g_FlashID==0x13))//MX Flash
		op_code=MXIC_ERASE_SECTOR;
	else//PMC Flash
		op_code=PMC_ERASE_SECTOR;
	
	FlashDisableWP1(); // 2005/5/11 ¤W¤È 10:19:32 by keng
	FlashWriteEnable();
	CleanSPI_SI_BUF();
	SPI_SI_BUF[0] = op_code;
	SPI_SI_BUF[1] = (address >> 16) &0x0ff;
	SPI_SI_BUF[2] = (address >> 8) &0x0ff;
	SPI_SI_BUF[3] = address &0x0ff;
	SPI_CTRL = 0xFB; //0xFC;
	
#undef op_code
	retry=10;
	while ((FlashReadSR() &BIT0) && retry--)
		ForceDelay1ms(1);
}


#endif



//////////////////////////////////////////////////////////////////////////////
// <API><Description>: Search specific FLASH_BUFFTER_TYPE form FDATA_BUF_ADDR(x)
// <Returns> : Specific buffer address when found; 0 when not found
// <Parameter>: 	- 	<Flow>	-	<Description>
// ---------------------------------------------------------------------------
//  ucType  -   In  -   buffer type, such as FBufType_MonitorSetting, FBufType_TimingMode ..
//////////////////////////////////////////////////////////////////////////////
WORD Flash_SearchBufferAddr(FLASH_BUFFTER_TYPE ucType)
{	BYTE ucTemp;

	for(ucTemp=0; ucTemp<FDATA_DYNAMIC_SECTOR_NUM; ucTemp++)
	{
		if(FlashReadByte(FDATA_BUF_ADDR(ucTemp))==ucType)
			return FDATA_BUF_ADDR(ucTemp);
	}
	return 0;
}
//////////////////////////////////////////////////////////////////////////////
// <API><Description>: Compare the FlashReadByte(wAddr + i*wRange)==ucIdent
//                                 FlashReadByte(wAddr + i*wRange + wRange - 1) == ucChecksum
//                     return i if equal ,else return IDENT_NOT_FOUND
// Search index from wStartIdx to wEndIdx to replace i. Can from 0 search to 255(include)
// Also can from 255 to 0(include)
// <Returns> : Specific buffer address when found; IDENT_NOT_FOUND when not found
// <Parameter>: 	- 	<Flow>	-	<Description>
// ---------------------------------------------------------------------------
//  wAddr       -   In  -   Flash start Address
//  wStartIdx   -   In  -   wAddr + wStartIdx*wRange
//  wEndIdx     -   In  -   wAddr + wEndIdx*wRange
//  wRange      -   In  -   Each search range
//  ucIdent     -   In  -   Data of want to compare
//  ucChecksum  -   In  -   Data of want to compare
//////////////////////////////////////////////////////////////////////////////
WORD Flash_SearchIdentChecksum(WORD wAddr, WORD wStartIdx, WORD wEndIdx, WORD wRange, BYTE ucIdent, BYTE ucChecksum)
{	WORD i;

    if(wStartIdx < wEndIdx)
    {
        i = wStartIdx;
        while( i >= wStartIdx && i <= wEndIdx)
        {
            if(FlashReadByte(wAddr + i*wRange)==ucIdent && FlashReadByte(wAddr + i*wRange + wRange - 1) == ucChecksum)
                return i;
            i++;
        }
    }
    else
    {
        i = wStartIdx;
        while( i >= wEndIdx && i <= wStartIdx)
        {
            if(FlashReadByte(wAddr + i*wRange)==ucIdent && FlashReadByte(wAddr + i*wRange + wRange - 1) == ucChecksum)
                return i;
            i--;
        }

    }
    //FLASH_printMsg("------- IDENT_CHECKSUM_NOT_FOUND -------");
	return IDENT_NOT_FOUND;
}

//////////////////////////////////////////////////////////////////////////////
//  <API><Description>: Get each buffer address and assign to variable
//	FLASH_MonitorSettingAddr
//  FLASH_MonitorSetting2Addr
//	FLASH_TimingModeAddr
//  FLASH_FactorySettingAddr
//	FLASH_FreeBufferAddr
//////////////////////////////////////////////////////////////////////////////
void Flash_GetBufferAddr(void)
	{
		WORD wAddr;
	
		FLASH_FactorySettingAddr = FDATA_FACTORY_ADDR;
		FLASH_MonitorSettingAddr = 0;
		FLASH_MonitorSetting2Addr = 0;
		FLASH_TimingModeAddr = 0;
		FLASH_FreeBufferAddr = 0;
	
		FLASH_printData("_FGBA_ FLASH_FactorySettingAddr %x ",(WORD)(FLASH_FactorySettingAddr));
	
		wAddr = Flash_SearchBufferAddr(FBufType_MonitorSetting);
		if(wAddr)
		{	FLASH_MonitorSettingAddr = wAddr;
	
			FLASH_printData("_FGBA_ FLASH_MonitorSettingAddr %x ",(WORD)(FLASH_MonitorSettingAddr));
	
		}
	
		wAddr = Flash_SearchBufferAddr(FBufType_MonitorSetting2);
		if(wAddr)
		{	FLASH_MonitorSetting2Addr = wAddr;
	
			FLASH_printData("_FGBA_ FLASH_MonitorSetting2Addr %x ",(WORD)(FLASH_MonitorSetting2Addr));
	
		}
	
		wAddr = Flash_SearchBufferAddr(FBufType_TimingMode);
		if(wAddr)
		{	FLASH_TimingModeAddr = wAddr;
	
			FLASH_printData("_FGBA_ FLASH_TimingModeAddr %x ",(WORD)(FLASH_TimingModeAddr));
	
		}
	
		wAddr = Flash_SearchBufferAddr(FBufType_EmptyBuffer);
		if(wAddr)
		{	FLASH_FreeBufferAddr = wAddr;
	
			FLASH_printData("FLASH_FreeBufferAddr %x ",(WORD)(FLASH_FreeBufferAddr));
	
		}
		// 091022 coding, free buffer type should be 0xff or 0x00
			else
			{
				if((wAddr = Flash_SearchBufferAddr(FBufType_FullBuffer)) >= FDATA_DYNAMIC_ADDR)
					FLASH_FreeBufferAddr = wAddr;
			}
	
		return;
	}


//////////////////////////////////////////////////////////////////////////////
// <API><Description>: After execute Flash_GetBufferAddr, makesure each address reasonable
//////////////////////////////////////////////////////////////////////////////
void Flash_AllocateBufferAddr(void)
	{	BYTE ucTemp;
	
		if(FLASH_MonitorSettingAddr==0)
		{
			for(ucTemp=0;ucTemp<FDATA_DYNAMIC_SECTOR_NUM;ucTemp++)
			{
			#if 0
				FLASH_printData("Flash_Allocate 01 %x ",(WORD)(FLASH_TimingModeAddr>>8));
				FLASH_printData("Flash_Allocate 02 %x ",(WORD)(FLASH_FreeBufferAddr>>8));
				FLASH_printData("Flash_Allocate 03 %x ",(WORD)(FDATA_BUF_ADDR(ucTemp)>>8));
			#endif
				if(FDATA_BUF_ADDR(ucTemp)!=FLASH_TimingModeAddr
					&& FDATA_BUF_ADDR(ucTemp)!=FLASH_FreeBufferAddr
					&& FDATA_BUF_ADDR(ucTemp)!= FLASH_MonitorSetting2Addr)
				{	FLASH_MonitorSettingAddr = FDATA_BUF_ADDR(ucTemp);
					break;
				}
			}
	
			FLASH_printData("_FABA_ FLASH_MonitorSettingAddr %x ",(WORD)(FLASH_MonitorSettingAddr));
	
		}
		if(FLASH_MonitorSetting2Addr==0)
		{
			for(ucTemp=0;ucTemp<FDATA_DYNAMIC_SECTOR_NUM;ucTemp++)
			{
				if(FDATA_BUF_ADDR(ucTemp)!=FLASH_MonitorSettingAddr
					&& FDATA_BUF_ADDR(ucTemp)!=FLASH_TimingModeAddr
					&& FDATA_BUF_ADDR(ucTemp)!=FLASH_FreeBufferAddr)
				{	FLASH_MonitorSetting2Addr = FDATA_BUF_ADDR(ucTemp);
					break;
				}
			}
	
			FLASH_printData("_FABA_ FLASH_MonitorSetting2Addr %x ",(WORD)(FLASH_MonitorSetting2Addr));
	
		}
		if(FLASH_TimingModeAddr==0)
		{
			for(ucTemp=0;ucTemp<FDATA_DYNAMIC_SECTOR_NUM;ucTemp++)
			{
				if(FDATA_BUF_ADDR(ucTemp)!=FLASH_MonitorSettingAddr
					&& FDATA_BUF_ADDR(ucTemp)!=FLASH_FreeBufferAddr
					&& FDATA_BUF_ADDR(ucTemp)!= FLASH_MonitorSetting2Addr)
				{	FLASH_TimingModeAddr = FDATA_BUF_ADDR(ucTemp);
					break;
				}
			}
	
			FLASH_printData("_FABA_ FLASH_TimingModeAddr %x ",(WORD)(FLASH_TimingModeAddr));
	
		}
		if(FLASH_FreeBufferAddr==0)
		{
			for(ucTemp=0;ucTemp<FDATA_DYNAMIC_SECTOR_NUM;ucTemp++)
			{
				if(FDATA_BUF_ADDR(ucTemp)!=FLASH_MonitorSettingAddr
					&& FDATA_BUF_ADDR(ucTemp)!=FLASH_TimingModeAddr
					&& FDATA_BUF_ADDR(ucTemp)!= FLASH_MonitorSetting2Addr)
				{	FLASH_FreeBufferAddr = FDATA_BUF_ADDR(ucTemp);
					break;
				}
			}
	
			FLASH_printData("_FABA_ FLASH_FreeBufferAddr %x ",(WORD)(FLASH_FreeBufferAddr));
	
		}
	
		if(FLASH_MonitorSettingAddr==0 || FLASH_MonitorSetting2Addr==0 || FLASH_TimingModeAddr==0 || FLASH_FreeBufferAddr==0)
		{
				FLASH_MonitorSettingAddr = FDATA_BUF_ADDR(0);
				FLASH_MonitorSetting2Addr = FDATA_BUF_ADDR(1);
				FLASH_TimingModeAddr = FDATA_BUF_ADDR(2);
				FLASH_FreeBufferAddr = FDATA_BUF_ADDR(3);
	
			FLASH_printMsg("_FABA_ Addr NEW");
			FLASH_printData("_FABA_ FLASH_MonitorSettingAddr %x ",(WORD)(FLASH_MonitorSettingAddr));
			FLASH_printData("_FABA_ FLASH_MonitorSetting2Addr %x ",(WORD)(FLASH_MonitorSetting2Addr));
			FLASH_printData("_FABA_ FLASH_TimingModeAddr %x ",(WORD)(FLASH_TimingModeAddr));
			FLASH_printData("_FABA_ FLASH_FactorySettingAddr %x ",(WORD)(FLASH_FactorySettingAddr));
			FLASH_printData("_FABA_ FLASH_FreeBufferAddr %x ",(WORD)(FLASH_FreeBufferAddr));
		}
	
	}

//////////////////////////////////////////////////////////////////////////////
// <API><Description>: Check each buffer type correct or not,
//				  Erase the sector when type wrong
//////////////////////////////////////////////////////////////////////////////
void Flash_CheckBufferType(void)
	{
			    #if 0//Enable_Cache
                        MCU_DisableCache();
                        #endif

		if(FBufType_FactorySetting !=FlashReadByte(FLASH_FactorySettingAddr))
		{	bFlashWriteFactory = TRUE;
		#if 0
		 if(FlashReadByte(HDCPKEYSET_START)==0xFF&&FlashReadByte(HDCPKEYSET_START+1)==0xFF
			&&FlashReadByte(HDCPKEYSET_START+2)==0xFF&&FlashReadByte(HDCPKEYSET_START+3)==0xFF
			&&FlashReadByte(HDCPKEYSET_START+4)==0xFF)			//charles test 1125
		#endif
			FlashSectorErase(TRUE, FLASH_FactorySettingAddr);
			FlashWriteByte(TRUE,FLASH_FactorySettingAddr, FBufType_FactorySetting);
			bFlashWriteFactory = FALSE;
			FLASH_printMsg("Flash_CheckBufferType SetFactoryType");
		}
		if(FBufType_MonitorSetting!=FlashReadByte(FLASH_MonitorSettingAddr))
		{	FlashSectorErase(TRUE, FLASH_MonitorSettingAddr);
			FlashWriteByte(TRUE,FLASH_MonitorSettingAddr, FBufType_MonitorSetting);
			FLASH_printMsg("Flash_CheckBufferType SetMonitorType");
		}
		if(FBufType_MonitorSetting2!=FlashReadByte(FLASH_MonitorSetting2Addr))
		{	FlashSectorErase(TRUE, FLASH_MonitorSetting2Addr);
			FlashWriteByte(TRUE,FLASH_MonitorSetting2Addr, FBufType_MonitorSetting2);
			FLASH_printMsg("Flash_CheckBufferType SetMonitorSetting2Type");
		}
		if(FBufType_TimingMode!=FlashReadByte(FLASH_TimingModeAddr))
		{	FlashSectorErase(TRUE, FLASH_TimingModeAddr);
			FlashWriteByte(TRUE,FLASH_TimingModeAddr, FBufType_TimingMode);
			FLASH_printMsg("Flash_CheckBufferType SetTimingModeAddrType");
		}
			   #if 0//Enable_Cache
                        MCU_EnableCache();
                        #endif

	
	}


//////////////////////////////////////////////////////////////////////////////
// <API><Description>: Clear all timing mode save in flash
//////////////////////////////////////////////////////////////////////////////
void Flash_ClearModeSet(void)
{
	FLASH_printMsg("_FCMS_ Flash_ClearModeSet");
	#if 0//Enable_Cache
              MCU_DisableCache();
       #endif

	FlashSectorErase(TRUE, FLASH_TimingModeAddr);
	FlashWriteByte(TRUE, FLASH_TimingModeAddr,FBufType_TimingMode);
	#if 0//Enable_Cache
             MCU_EnableCache();
        #endif

}

//////////////////////////////////////////////////////////////////////////////
// <API><Description>:	Write Factory setting, HDCP Key, DDCA, DDCD key to free buffer,
//                  Exchange buffer address FLASH_FreeBufferAddr and FLASH_FactorySettingAddr
//////////////////////////////////////////////////////////////////////////////
void Flash_BackupFactorySetToBuffer(void)
{
    WORD wAddr;

    FLASH_printMsg("  Flash_BackupFactorySetToBuffer ");

    FlashDisableWP(FLASH_FreeBufferAddr);
    if(FlashReadByte(FACTORY_STATUS_ADDR) == FLASH_KEY_VALID)
    {
        Flash_MoveTbl(FALSE, FACTORY_START, FACTORY_BUF_START, FACTORY_SIZE);
        FlashWriteByte(FALSE, FACTORY_BUF_STATUS_ADDR, FLASH_KEY_VALID);
        FLASH_printMsg("  _FACT_ ");
    }
    if(FlashReadByte(PANEL_STATUS_ADDR) == FLASH_KEY_VALID)
    {
      //  Flash_MoveTbl(FALSE, PANEL_START, PANEL_BUF_START, PANEL_SIZE);
        FlashWriteByte(FALSE, PANEL_BUF_STATUS_ADDR, FLASH_KEY_VALID);
        FLASH_printMsg("  _PANEL_ ");
    }
    if(FlashReadByte(GAMMA_STATUS_ADDR) == FLASH_KEY_VALID)
    {
        Flash_MoveTbl(FALSE, GAMMA_START, GAMMA_BUF_START, GAMMA_SIZE);
        FlashWriteByte(FALSE, GAMMA_BUF_STATUS_ADDR, FLASH_KEY_VALID);
        FLASH_printMsg("  _GAMMA_ ");
    }
    /*if(FlashReadByte(HDCPKEY_STATUS_ADDR) == FLASH_KEY_VALID)
    {
        Flash_MoveTbl(FALSE, HDCPKEYSET_START, HDCPKEYSET_BUF_START, HDCPKEY_SIZE);
        FlashWriteByte(FALSE, HDCPKEY_BUF_STATUS_ADDR, FLASH_KEY_VALID);
        FLASH_printMsg("  _HDCP_ ");
    }
    */
    if(FlashReadByte(DDCAKEY_STATUS_ADDR) == FLASH_KEY_VALID)
    {
        Flash_MoveTbl(FALSE, DDCAKEYSET_START, DDCAKEYSET_BUF_START, DDCAKEY_SIZE);
        FlashWriteByte(FALSE, DDCAKEY_BUF_STATUS_ADDR, FLASH_KEY_VALID);
        FLASH_printMsg("  _DDCA_ ");
    }
    if(FlashReadByte(DDCDKEY_STATUS_ADDR) == FLASH_KEY_VALID)
    {
        Flash_MoveTbl(FALSE, DDCDKEYSET_START, DDCDKEYSET_BUF_START, DDCDKEY_SIZE);
        FlashWriteByte(FALSE, DDCDKEY_BUF_STATUS_ADDR, FLASH_KEY_VALID);
        FLASH_printMsg("  _DDCD_ ");
    }
    if(FlashReadByte(DDCHKEY_STATUS_ADDR) == FLASH_KEY_VALID)
    {
        Flash_MoveTbl(FALSE, DDCHKEYSET_START, DDCHKEYSET_BUF_START, DDCHKEY_SIZE);
        FlashWriteByte(FALSE, DDCHKEY_BUF_STATUS_ADDR, FLASH_KEY_VALID);
        FLASH_printMsg("  _DDCH_ ");
    }
    if(FlashReadByte(DDCDPKEY_STATUS_ADDR) == FLASH_KEY_VALID)
    {
        Flash_MoveTbl(FALSE, DDCDPKEYSET_START, DDCDPKEYSET_BUF_START, DDCDPKEY_SIZE);
        FlashWriteByte(FALSE, DDCDPKEY_BUF_STATUS_ADDR, FLASH_KEY_VALID);
        FLASH_printMsg("  _DDCDP_ ");
    }
    FlashWriteByte(FALSE, FLASH_FreeBufferAddr,FBufType_FactorySetting);
    FlashEnableWP();
    FlashWriteByte(TRUE, FLASH_FactorySettingAddr,0x00);
    wAddr = FLASH_FreeBufferAddr;
    FLASH_FreeBufferAddr = FLASH_FactorySettingAddr;
    FLASH_FactorySettingAddr = wAddr;

}

//////////////////////////////////////////////////////////////////////////////
// <API><Description>:	Write HDCP ,DDCA or DDCD key to flash,
//					Need set correct type of key.
//                  wIndex from 0 to (key size / ucBufSize) or (key size / ucBufSize+1)
//                  The last write wLen = (key size % ucBufSize)
//                  Will backup to free buffer if write to index = (MAX_HDCPKEYSET_NUM-1)
// <Parameter>: 	- 	<Flow>	-	<Description>
// ---------------------------------------------------------------------------
//  ucType          -    In		-	FLASH_KEY_HDCP, FLASH_KEY_DDCA or FLASH_KEY_DDCD
//  wIndex			-    In		-	From 0 ~ x
//  ucBufSize	    -    In		-	Size of buf
//  *buf			-    In		-	point to the data of each set
//  wLen			-    In		-	Length of this time write buffer, wBufSize>=wLen
//////////////////////////////////////////////////////////////////////////////
void Flash_Write_Factory_KeySet(FLASH_KEY_TYPE ucType, WORD wIndex, WORD wBufSize, BYTE* buf, WORD wLen)
{
    //WORD xdata wKEYSET_SIZE;
    WORD xdata wKEY_SIZE;
    WORD xdata wKEYSET_START;
    WORD xdata wKEYSET_BUF_START;
    WORD xdata wKEYSET_STATUS_ADDR;
    WORD xdata wKEYSET_BUF_STATUS_ADDR;

    if(ucType == FLASH_KEY_FACTORY)
    {
        //wKEYSET_SIZE            = FACTORY_SECTOR_SIZE;
        wKEY_SIZE               = FACTORY_SIZE;
        wKEYSET_START           = FACTORY_START;
        wKEYSET_BUF_START       = FACTORY_BUF_START;
        wKEYSET_STATUS_ADDR     = FACTORY_STATUS_ADDR;
        wKEYSET_BUF_STATUS_ADDR = FACTORY_BUF_STATUS_ADDR;
    }
    else if(ucType == FLASH_KEY_PANEL)
    {
        //wKEYSET_SIZE            = PANEL_SECTOR_SIZE;
      //  wKEY_SIZE               = PANEL_SIZE;
        wKEYSET_START           = PANEL_START;
        wKEYSET_BUF_START       = PANEL_BUF_START;
        wKEYSET_STATUS_ADDR     = PANEL_STATUS_ADDR;
        wKEYSET_BUF_STATUS_ADDR = PANEL_BUF_STATUS_ADDR;
    }
    else if(ucType == FLASH_KEY_GAMMA)
    {
        //wKEYSET_SIZE            = GAMMA_SECTOR_SIZE;
        wKEY_SIZE               = GAMMA_SIZE;
        wKEYSET_START           = GAMMA_START;
        wKEYSET_BUF_START       = GAMMA_BUF_START;
        wKEYSET_STATUS_ADDR     = GAMMA_STATUS_ADDR;
        wKEYSET_BUF_STATUS_ADDR = GAMMA_BUF_STATUS_ADDR;
    }
   /* else if(ucType == FLASH_KEY_HDCP)
    {
        //wKEYSET_SIZE            = HDCPKEY_SECTOR_SIZE;
        wKEY_SIZE               = HDCPKEY_SIZE;
        wKEYSET_START           = HDCPKEYSET_START;
        wKEYSET_BUF_START       = HDCPKEYSET_BUF_START;
        wKEYSET_STATUS_ADDR     = HDCPKEY_STATUS_ADDR;
        wKEYSET_BUF_STATUS_ADDR = HDCPKEY_BUF_STATUS_ADDR;
    }*/
    else if(ucType == FLASH_KEY_DDCA)
    {
        //wKEYSET_SIZE            = DDCAKEY_SECTOR_SIZE;
        wKEY_SIZE               = DDCAKEY_SIZE;
        wKEYSET_START           = DDCAKEYSET_START;
        wKEYSET_BUF_START       = DDCAKEYSET_BUF_START;
        wKEYSET_STATUS_ADDR     = DDCAKEY_STATUS_ADDR;
        wKEYSET_BUF_STATUS_ADDR = DDCAKEY_BUF_STATUS_ADDR;
    }
    else if(ucType == FLASH_KEY_DDCD)
    {
        //wKEYSET_SIZE            = DDCDKEY_SECTOR_SIZE;
        wKEY_SIZE               = DDCDKEY_SIZE;
        wKEYSET_START           = DDCDKEYSET_START;
        wKEYSET_BUF_START       = DDCDKEYSET_BUF_START;
        wKEYSET_STATUS_ADDR     = DDCDKEY_STATUS_ADDR;
        wKEYSET_BUF_STATUS_ADDR = DDCDKEY_BUF_STATUS_ADDR;
    }
    else if(ucType == FLASH_KEY_DDCH)
    {
        //wKEYSET_SIZE            = DDCHKEY_SECTOR_SIZE;
        wKEY_SIZE               = DDCHKEY_SIZE;
        wKEYSET_START           = DDCHKEYSET_START;
        wKEYSET_BUF_START       = DDCHKEYSET_BUF_START;
        wKEYSET_STATUS_ADDR     = DDCHKEY_STATUS_ADDR;
        wKEYSET_BUF_STATUS_ADDR = DDCHKEY_BUF_STATUS_ADDR;
    }
    else if(ucType == FLASH_KEY_DDCDP)
    {
        //wKEYSET_SIZE            = DDCDPKEY_SECTOR_SIZE;
        wKEY_SIZE               = DDCDPKEY_SIZE;
        wKEYSET_START           = DDCDPKEYSET_START;
        wKEYSET_BUF_START       = DDCDPKEYSET_BUF_START;
        wKEYSET_STATUS_ADDR     = DDCDPKEY_STATUS_ADDR;
        wKEYSET_BUF_STATUS_ADDR = DDCDPKEY_BUF_STATUS_ADDR;
    }
    else
        return;

    bFlashWriteFactory = TRUE;

    if( FlashReadByte(wKEYSET_STATUS_ADDR) == FLASH_KEY_EMPTY || FlashReadByte(wKEYSET_STATUS_ADDR) == FLASH_KEY_WRITE)
    {
        if(wIndex == 0x00)
        {
           // FLASH_printData("  _FWFK_StatusAddr [%x] ",wKEYSET_STATUS_ADDR);
           // FlashWriteByte(TRUE, wKEYSET_STATUS_ADDR, FLASH_KEY_WRITE);
        }
        Flash_WriteTbl(TRUE, wKEYSET_START+wIndex*wBufSize, buf, wLen);
        if((wIndex*wBufSize+wLen) >= wKEY_SIZE) // check write end
        {
            FlashWriteByte(TRUE, wKEYSET_STATUS_ADDR, FLASH_KEY_VALID);
           // FLASH_printData("  _FWFK_direct write End [%x] ",ucType);
        }
    }
    else
    {
        if(wIndex == 0x00)
        {
            //FLASH_printData("  _FWFK_StatusAddr [%x] ",wKEYSET_STATUS_ADDR);
            //FLASH_printData("  _FWFK_Addr [%x] ",wKEYSET_START);
           
           if(bFlashEraseBufferFinish) // jeff add it in 2009 0907
               bFlashEraseBufferFinish=FALSE;
           else
            {
            // bFlashEraseBufferFinish=FALSE;
            FlashWriteByte(TRUE, wKEYSET_STATUS_ADDR, FLASH_KEY_INVALID);
            FlashSectorErase(TRUE, FLASH_FreeBufferAddr);
            FlashWriteByte(TRUE, wKEYSET_BUF_STATUS_ADDR, FLASH_KEY_WRITE);

            }
           
        }

        
        
        Flash_WriteTbl(TRUE, wKEYSET_BUF_START+wIndex*wBufSize, buf, wLen);
        if((wIndex*wBufSize+wLen) >= wKEY_SIZE) // check write end
        {
            FlashWriteByte(TRUE, wKEYSET_BUF_STATUS_ADDR, FLASH_KEY_VALID);
            //FLASH_printData("  _FWFK_buffer write End [%x] ",ucType);
        	Flash_BackupFactorySetToBuffer();
            FlashSectorErase(TRUE, FLASH_FreeBufferAddr);
        	Flash_BackupFactorySetToBuffer();
        }

    }
    bFlashWriteFactory = FALSE;
}

#if ENABLE_HDCP
#if HDCPKEY_IN_Flash
//////////////////////////////////////////////////////////////////////////////
// <API><Description>:Erase Buffer,
//					Need set correct type of key.
//                  wIndex from 0 to (key size / ucBufSize) or (key size / ucBufSize+1)

// <Parameter>: 	- 	<Flow>	-	<Description>
// ---------------------------------------------------------------------------
//  ucType          -    In		-	FLASH_KEY_HDCP, FLASH_KEY_DDCA or FLASH_KEY_DDCD
//  wIndex			-    In		-	From 0 ~ x

//   This function is use to speed up the time for write HDCP key
//////////////////////////////////////////////////////////////////////////////

#if 0
void Flash_Erase_FreeBuffer(FLASH_KEY_TYPE ucType,WORD wIndex)  // jeff add it in 2009 0907
{
  //WORD xdata wKEYSET_SIZE;
    WORD xdata wKEY_SIZE;
    WORD xdata wKEYSET_START;
    WORD xdata wKEYSET_BUF_START;
    WORD xdata wKEYSET_STATUS_ADDR;
    WORD xdata wKEYSET_BUF_STATUS_ADDR;

    if(ucType == FLASH_KEY_FACTORY)
    {
        //wKEYSET_SIZE            = FACTORY_SECTOR_SIZE;
        wKEY_SIZE               = FACTORY_SIZE;
        wKEYSET_START           = FACTORY_START;
        wKEYSET_BUF_START       = FACTORY_BUF_START;
        wKEYSET_STATUS_ADDR     = FACTORY_STATUS_ADDR;
        wKEYSET_BUF_STATUS_ADDR = FACTORY_BUF_STATUS_ADDR;
    }
    else if(ucType == FLASH_KEY_PANEL)
    {
        //wKEYSET_SIZE            = PANEL_SECTOR_SIZE;
        wKEY_SIZE               = PANEL_SIZE;
        wKEYSET_START           = PANEL_START;
        wKEYSET_BUF_START       = PANEL_BUF_START;
        wKEYSET_STATUS_ADDR     = PANEL_STATUS_ADDR;
        wKEYSET_BUF_STATUS_ADDR = PANEL_BUF_STATUS_ADDR;
    }
    else if(ucType == FLASH_KEY_GAMMA)
    {
        //wKEYSET_SIZE            = GAMMA_SECTOR_SIZE;
        wKEY_SIZE               = GAMMA_SIZE;
        wKEYSET_START           = GAMMA_START;
        wKEYSET_BUF_START       = GAMMA_BUF_START;
        wKEYSET_STATUS_ADDR     = GAMMA_STATUS_ADDR;
        wKEYSET_BUF_STATUS_ADDR = GAMMA_BUF_STATUS_ADDR;
    }
   /* else if(ucType == FLASH_KEY_HDCP)
    {
        //wKEYSET_SIZE            = HDCPKEY_SECTOR_SIZE;
        wKEY_SIZE               = HDCPKEY_SIZE;
        wKEYSET_START           = HDCPKEYSET_START;
        wKEYSET_BUF_START       = HDCPKEYSET_BUF_START;
        wKEYSET_STATUS_ADDR     = HDCPKEY_STATUS_ADDR;
        wKEYSET_BUF_STATUS_ADDR = HDCPKEY_BUF_STATUS_ADDR;
    }*/
    else if(ucType == FLASH_KEY_DDCA)
    {
        //wKEYSET_SIZE            = DDCAKEY_SECTOR_SIZE;
        wKEY_SIZE               = DDCAKEY_SIZE;
        wKEYSET_START           = DDCAKEYSET_START;
        wKEYSET_BUF_START       = DDCAKEYSET_BUF_START;
        wKEYSET_STATUS_ADDR     = DDCAKEY_STATUS_ADDR;
        wKEYSET_BUF_STATUS_ADDR = DDCAKEY_BUF_STATUS_ADDR;
    }
    else if(ucType == FLASH_KEY_DDCD)
    {
        //wKEYSET_SIZE            = DDCDKEY_SECTOR_SIZE;
        wKEY_SIZE               = DDCDKEY_SIZE;
        wKEYSET_START           = DDCDKEYSET_START;
        wKEYSET_BUF_START       = DDCDKEYSET_BUF_START;
        wKEYSET_STATUS_ADDR     = DDCDKEY_STATUS_ADDR;
        wKEYSET_BUF_STATUS_ADDR = DDCDKEY_BUF_STATUS_ADDR;
    }
    else if(ucType == FLASH_KEY_DDCH)
    {
        //wKEYSET_SIZE            = DDCHKEY_SECTOR_SIZE;
        wKEY_SIZE               = DDCHKEY_SIZE;
        wKEYSET_START           = DDCHKEYSET_START;
        wKEYSET_BUF_START       = DDCHKEYSET_BUF_START;
        wKEYSET_STATUS_ADDR     = DDCHKEY_STATUS_ADDR;
        wKEYSET_BUF_STATUS_ADDR = DDCHKEY_BUF_STATUS_ADDR;
    }
    else if(ucType == FLASH_KEY_DDCDP)
    {
        //wKEYSET_SIZE            = DDCDPKEY_SECTOR_SIZE;
        wKEY_SIZE               = DDCDPKEY_SIZE;
        wKEYSET_START           = DDCDPKEYSET_START;
        wKEYSET_BUF_START       = DDCDPKEYSET_BUF_START;
        wKEYSET_STATUS_ADDR     = DDCDPKEY_STATUS_ADDR;
        wKEYSET_BUF_STATUS_ADDR = DDCDPKEY_BUF_STATUS_ADDR;
    }
    else
        return;

    bFlashWriteFactory = TRUE;

    if( FlashReadByte(wKEYSET_STATUS_ADDR) == FLASH_KEY_EMPTY || FlashReadByte(wKEYSET_STATUS_ADDR) == FLASH_KEY_WRITE)
    {
         bFlashWriteFactory = FALSE;

    }
    else
    {
        if(wIndex == 0x00)
        {
           // FLASH_printData("  _FWFK_StatusAddr [%x] ",wKEYSET_STATUS_ADDR);
           // FLASH_printData("  _FWFK_Addr [%x] ",wKEYSET_START);

            FlashWriteByte(TRUE, wKEYSET_STATUS_ADDR, FLASH_KEY_INVALID);
            FlashSectorErase(TRUE, FLASH_FreeBufferAddr);
           FlashWriteByte(TRUE, wKEYSET_BUF_STATUS_ADDR, FLASH_KEY_WRITE);
            bFlashEraseBufferFinish=TRUE;

        }

    }
    bFlashWriteFactory = FALSE;

}
#endif
#endif
#endif
//////////////////////////////////////////////////////////////////////////////
// <API><Description>:	Read HDCP ,DDCA or DDCD key each wLen from flash,
//					Need set correct ucType.
//                  wIndex from 0 to (key size / wBufSize) or (key size / wBufSize+1)
//                  The last read wLen = (key size % wBufSize)
// <Returns> : return TRUE when found key ident and CheckSum
// <Parameter>: 	- 	<Flow>	-	<Description>
// ---------------------------------------------------------------------------
//  ucType          -    In		-	FLASH_KEY_HDCP, FLASH_KEY_DDCA or FLASH_KEY_DDCD
//  wIndex			-    In		-	From 0 ~ x
//  wBufSize	    -    In		-	Size of buf
//  *buf			-    In		-	point to the data of each set
//  wLen			-    In		-	Length of this time write buffer, wBufSize>=wLen
//////////////////////////////////////////////////////////////////////////////
Bool Flash_Read_Factory_KeySet(FLASH_KEY_TYPE ucType, Bool bReadVaild, WORD wIndex, WORD wBufSize, BYTE* buf, WORD wLen)		//110311 Modify
{
    //WORD xdata wKEYSET_SIZE;
    WORD xdata wKEY_SIZE;
    WORD xdata wKEYSET_START;
    WORD xdata wKEYSET_STATUS_ADDR;

    if(ucType == FLASH_KEY_FACTORY)
    {
        //wKEYSET_SIZE        = FACTORY_SECTOR_SIZE;
        wKEY_SIZE           = FACTORY_SIZE;
        wKEYSET_START       = FACTORY_START;
        wKEYSET_STATUS_ADDR = FACTORY_STATUS_ADDR;
    }
    else if(ucType == FLASH_KEY_PANEL)
    {
        //wKEYSET_SIZE        = PANEL_SECTOR_SIZE;
       // wKEY_SIZE           = PANEL_SIZE;
      //  wKEYSET_START       = PANEL_START;
      //  wKEYSET_STATUS_ADDR = PANEL_STATUS_ADDR;
    }
    else if(ucType == FLASH_KEY_GAMMA)
    {
        //wKEYSET_SIZE        = GAMMA_SECTOR_SIZE;
        wKEY_SIZE           = GAMMA_SIZE;
        wKEYSET_START       = GAMMA_START;
        wKEYSET_STATUS_ADDR = GAMMA_STATUS_ADDR;
    }
    /*else if(ucType == FLASH_KEY_HDCP)
    {
        //wKEYSET_SIZE        = HDCPKEY_SECTOR_SIZE;
        wKEY_SIZE           = HDCPKEY_SIZE;
        wKEYSET_START       = HDCPKEYSET_START;
        wKEYSET_STATUS_ADDR = HDCPKEY_STATUS_ADDR;
    }*/
    else if(ucType == FLASH_KEY_DDCA)
    {
        //wKEYSET_SIZE        = DDCAKEY_SECTOR_SIZE;
        wKEY_SIZE           = DDCAKEY_SIZE;
        wKEYSET_START       = DDCAKEYSET_START;
        wKEYSET_STATUS_ADDR = DDCAKEY_STATUS_ADDR;
    }
    else if(ucType == FLASH_KEY_DDCD)
    {
        //wKEYSET_SIZE    = DDCDKEY_SECTOR_SIZE;
        wKEY_SIZE       = DDCDKEY_SIZE;
        wKEYSET_START   = DDCDKEYSET_START;
        wKEYSET_STATUS_ADDR = DDCDKEY_STATUS_ADDR;
    }
    else if(ucType == FLASH_KEY_DDCH)
    {
        //wKEYSET_SIZE    = DDCHKEY_SECTOR_SIZE;
        wKEY_SIZE       = DDCHKEY_SIZE;
        wKEYSET_START   = DDCHKEYSET_START;
        wKEYSET_STATUS_ADDR = DDCHKEY_STATUS_ADDR;
    }
    else if(ucType == FLASH_KEY_DDCDP)
    {
        //wKEYSET_SIZE    = DDCDPKEY_SECTOR_SIZE;
        wKEY_SIZE       = DDCDPKEY_SIZE;
        wKEYSET_START   = DDCDPKEYSET_START;
        wKEYSET_STATUS_ADDR = DDCDPKEY_STATUS_ADDR;
    }
    else
        return FALSE;


    if(FlashReadByte(wKEYSET_STATUS_ADDR) == FLASH_KEY_WRITE || FlashReadByte(wKEYSET_STATUS_ADDR) == FLASH_KEY_VALID)
    {
        //FLASH_printData("  Flash_ReadDDCAKeySet Succ [%x]",wKEYSET_START+i*wKEY_SIZE+1+wIndex*wBufSize);
        if(bReadVaild && FlashReadByte(wKEYSET_STATUS_ADDR) != FLASH_KEY_VALID)		//110311 Modify
            return FALSE;
        Flash_ReadTbl(wKEYSET_START+wIndex*wBufSize, buf, wLen);
        return TRUE;
    }
    else
    {
       // FLASH_printData("  Flash_ReadKeySet Fail ucType[%x h]",ucType);
        return FALSE;
    }
}

//////////////////////////////////////////////////////////////////////////////
// <API><Description>:	Write Factory setting to flash,
//					It will exchange buffer when not found empty space
//////////////////////////////////////////////////////////////////////////////
void Flash_WriteFactorySet(BYTE* pbuf,BYTE ucLen)
{
    Flash_Write_Factory_KeySet(FLASH_KEY_FACTORY, 0, FACTORY_SIZE, pbuf, ucLen);
}
//////////////////////////////////////////////////////////////////////////////
// <API><Description>:	Read latest factory setting struct from flash.
// <Returns> : return TRUE when found factory setting
//////////////////////////////////////////////////////////////////////////////
Bool Flash_ReadFactorySet(BYTE* pbuf,BYTE ucLen)
{
    return Flash_Read_Factory_KeySet(FLASH_KEY_FACTORY, TRUE, 0, FACTORY_SIZE, pbuf, ucLen);	//110311 Modify
}
//////////////////////////////////////////////////////////////////////////////
// <API><Description>:	Write Monitor setting struct to flash,
//					It will exchange buffer when buffer full
//////////////////////////////////////////////////////////////////////////////
void Flash_WriteMonitorSet(BYTE* buf,BYTE len)
{
	WORD wAddr;
	WORD i;

    i = Flash_SearchIdentChecksum(MONITORSET_START, 0, MAX_MONITORSET_NUM-1, MONITORSET_SIZE, FLASH_EMPTYDATA, FLASH_EMPTYDATA);
    if(i != IDENT_NOT_FOUND)// Found the empty index
    {
     	//MonitorSetting.Ident= FLASH_IDENTDATA;
     	//MonitorSetting.CheckSum	= FLASH_CHECKSUM;
        wAddr = MONITORSET_START + i*MONITORSET_SIZE;
		Flash_WriteTbl(TRUE, wAddr, (BYTE*)buf, len);
		FLASH_printData("_FWMS_ wAddr %x",wAddr);
        if(i>0)
        {
            wAddr = MONITORSET_START + (i-1)*MONITORSET_SIZE;
            FlashWriteByte(TRUE, wAddr,FLASH_IDENTDATA_CLEAR);
        }
    }
    else
    {
		FlashSectorErase(TRUE, FLASH_FreeBufferAddr);
		Flash_WriteTbl(TRUE, MONITORSET_BUF_START, (BYTE*)buf, len);
		FLASH_printData("_FWMS_ wAddr %x",MONITORSET_BUF_START);
		FlashWriteByte(TRUE, FLASH_FreeBufferAddr,FBufType_MonitorSetting);
		FlashWriteByte(TRUE, FLASH_MonitorSettingAddr,0x00);
		wAddr = FLASH_FreeBufferAddr;
		FLASH_FreeBufferAddr = FLASH_MonitorSettingAddr;
		FLASH_MonitorSettingAddr = wAddr;
    }
}
//////////////////////////////////////////////////////////////////////////////
// <API><Description>:	Read latest Monitor setting struct from flash.
//					1.Got monitor setting index by check flash identity
//					2.If not found read CheckSum to got correct one
//					3.read the Monitor setting struct
// <Returns> : return TRUE when found MonitorSetting
//////////////////////////////////////////////////////////////////////////////
Bool Flash_ReadMonitorSet(BYTE* buf,BYTE len)
{
    WORD i;

    i = Flash_SearchIdentChecksum(MONITORSET_START, (MAX_MONITORSET_NUM-1), 0, MONITORSET_SIZE, FLASH_IDENTDATA, FLASH_CHECKSUM);
    if(i != IDENT_NOT_FOUND)
    {
    	Flash_ReadTbl(MONITORSET_START + i*MONITORSET_SIZE, buf, len);
    	FLASH_printData("_FRMS_ wAddr %x",MONITORSET_START + i*MONITORSET_SIZE);
        return TRUE;
    }
    else
    {
        i = Flash_SearchIdentChecksum(MONITORSET_START, (MAX_MONITORSET_NUM-1), 0, MONITORSET_SIZE, FLASH_IDENTDATA_CLEAR, FLASH_CHECKSUM);
        if(i != IDENT_NOT_FOUND)
        {
        	Flash_ReadTbl(MONITORSET_START + i*MONITORSET_SIZE, buf, len);
         //   FLASH_printMsg("_FRMS_ Only Checksum Correct! ");
        //	FLASH_printData("_FRMS_ wAddr %x",MONITORSET_START + i*MONITORSET_SIZE);
            return TRUE;
        }
        else
            return FALSE;
    }
}


//////////////////////////////////////////////////////////////////////////////
// <API><Description>:	Flash_WriteMonitorSet2 isues to save MonitorSetting2
//					It will exchange buffer when buffer full
//////////////////////////////////////////////////////////////////////////////
void Flash_WriteMonitorSet2(BYTE* buf,BYTE len)
{
	WORD wAddr;
	WORD i;

    i = Flash_SearchIdentChecksum(MONITORSET2_START, 0, MAX_MONITORSET2_NUM-1, MONITORSET2_SIZE, FLASH_EMPTYDATA, FLASH_EMPTYDATA);
    if(i != IDENT_NOT_FOUND)
    {
		wAddr = MONITORSET2_START + i*MONITORSET2_SIZE;
		Flash_WriteTbl(TRUE, wAddr, (BYTE*)buf, len);
		//FLASH_printData("_FWMS2_ wAddr %x",wAddr);
    }
    else
    {
		FlashSectorErase(TRUE, FLASH_FreeBufferAddr);
		Flash_WriteTbl(TRUE, MONITORSET2_BUF_START, (BYTE*)buf, len);
		//FLASH_printData("_FWMS2_ wAddr %x",MONITORSET2_BUF_START);
		FlashWriteByte(TRUE, FLASH_FreeBufferAddr,FBufType_MonitorSetting2);
		FlashWriteByte(TRUE, FLASH_MonitorSetting2Addr,0x00);
		wAddr = FLASH_FreeBufferAddr;
		FLASH_FreeBufferAddr = FLASH_MonitorSetting2Addr;
		FLASH_MonitorSetting2Addr = wAddr;
    }
}

//////////////////////////////////////////////////////////////////////////////
// <API><Description>:	Read MonitorSetting2 struct from flash.
//					1.Got monitor setting index by check flash identity
// <Returns> : return TRUE when found MonitorSetting2
//////////////////////////////////////////////////////////////////////////////
Bool Flash_ReadMonitorSet2(BYTE* buf,BYTE len)
{
    Bool bFound=FALSE;
	WORD i;

    i = Flash_SearchIdentChecksum(MONITORSET2_START, (MAX_MONITORSET2_NUM-1), 0, MONITORSET2_SIZE, FLASH_IDENTDATA, FLASH_CHECKSUM);
    if(i != IDENT_NOT_FOUND)
    {
    	Flash_ReadTbl(MONITORSET2_START + i*MONITORSET2_SIZE, buf, len);
    	//FLASH_printData("_FRMS2_ wAddr %x",MONITORSET2_START + i*MONITORSET2_SIZE);
        return TRUE;
    }
    else
    {
        FLASH_printMsg("  Flash_ReadMonitorSet2 Fail");
        return FALSE;
    }
}


//////////////////////////////////////////////////////////////////////////////
// <API><Description>:	Check exit timing mode number and compare to MAX_MODESET_NUM.
// <Returns> : return TRUE when saved timing mode size equal to MAX_MODESET_NUM
//////////////////////////////////////////////////////////////////////////////
Bool Flash_CheckModeFull(void)
{
    BYTE i,wExitModeCount=0;
    //check buffer full
	for(i=0;i < MAX_MODESET_NUM; i++)
	{
		if(FlashReadByte(MODESET_START+i*MODESET_SIZE)==FLASH_IDENTDATA && FlashReadByte(MODESET_START+i*MODESET_SIZE+MODESET_SIZE-1)==FLASH_CHECKSUM)
            wExitModeCount++;
    }
    if(wExitModeCount == MAX_MODESET_NUM)
        return TRUE;
    else
        return FALSE;
}
//////////////////////////////////////////////////////////////////////////////
// <API><Description>:	Clear all user mode ident.
//
//////////////////////////////////////////////////////////////////////////////
void Flash_ClearUserModeSpace(void)
{
    BYTE i,ucIdent=0;
    //check buffer full
	for(i=0;i < MAX_MODESET_NUM; i++)
	{
	    ucIdent = FlashReadByte(MODESET_START+i*MODESET_SIZE);
		if(ucIdent == FLASH_IDENTDATA)
        {
			if((FlashReadByte(MODESET_START+i*MODESET_SIZE+1)&0x80)==0x80)
                FlashWriteByte(TRUE, MODESET_START+i*MODESET_SIZE, 0x00); // Clear Identdata.
        }
    }
	FLASH_printMsg("Flash_ClearUserModeSpace");
}

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
void Flash_WriteModeSet(BYTE index,BYTE *buf)
{
	WORD i=0, j=0;
	WORD wAddr;
	xdata ModeSettingType modeSetting;

	//FLASH_printData("_FWMS_ Target index 0x%x",index);
    // find orginal mode index in flash
	for(i=0;i < MAX_MODESET_NUM; i++)
	{
		if(FlashReadByte(MODESET_START+i*MODESET_SIZE)==FLASH_IDENTDATA && FlashReadByte(MODESET_START+i*MODESET_SIZE+MODESET_SIZE-1)==FLASH_CHECKSUM)
		{
			if(FlashReadByte(MODESET_START+i*MODESET_SIZE+1)==index)
			{
				FlashWriteByte(TRUE, MODESET_START+i*MODESET_SIZE, 0x00); // Clear Identdata.
				FLASH_printData("_FWMS_ Clear index %x",i);
			}
		}
	}

    // find empty position
    i = Flash_SearchIdentChecksum(MODESET_START, 0, (MAX_MODESET_NUM-1), MODESET_SIZE, FLASH_EMPTYDATA, FLASH_EMPTYDATA);
    if(i != IDENT_NOT_FOUND)
    {
        FLASH_printData("_FWMS_ write to empty Addr %x",MODESET_START+i*MODESET_SIZE);
        Flash_WriteTbl(TRUE, MODESET_START+i*MODESET_SIZE, buf, MODESET_SIZE);
    }
    else
    {
        if(Flash_CheckModeFull())
            Flash_ClearUserModeSpace();

        FlashSectorErase(TRUE, FLASH_FreeBufferAddr);
		for(i=0;i < MAX_MODESET_NUM; i++)
		{
			if(FlashReadByte(MODESET_START+i*MODESET_SIZE)==FLASH_IDENTDATA  && FlashReadByte(MODESET_START+i*MODESET_SIZE+MODESET_SIZE-1)==FLASH_CHECKSUM)
			{
				Flash_ReadTbl(MODESET_START+i*MODESET_SIZE, (BYTE*)&modeSetting, MODESET_SIZE);
				Flash_WriteTbl(TRUE, MODESET_BUF_START+j*MODESET_SIZE, (BYTE*)&modeSetting, MODESET_SIZE);
				j++;
			}
		}
        if(j < (MAX_MODESET_NUM-1))
        {
            FLASH_printData("_FWMS_ write to empty Addr %x",MODESET_BUF_START+j*MODESET_SIZE);
            Flash_WriteTbl(TRUE, MODESET_BUF_START+j*MODESET_SIZE, buf, MODESET_SIZE);
        }

        //	Exchange Buffer
		FlashWriteByte(TRUE, FLASH_FreeBufferAddr,FBufType_TimingMode);
		FlashWriteByte(TRUE, FLASH_TimingModeAddr,0x00);
		wAddr = FLASH_FreeBufferAddr;
		FLASH_FreeBufferAddr = FLASH_TimingModeAddr;
		FLASH_TimingModeAddr = wAddr;
		//i=j;//Write empty one index

		FLASH_printData("_FWMS_ Buffer exchange TimingAddr %x ",(WORD)(FLASH_TimingModeAddr>>8));
		FLASH_printData("_FWMS_ Buffer exchange FreeAddr %x ",(WORD)(FLASH_FreeBufferAddr>>8));
    }
}


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
Bool Flash_ReadModeSet(BYTE index,BYTE *buf)
{
    WORD i;

    for(i=0;i < MAX_MODESET_NUM; i++)
    {
        if(FlashReadByte(MODESET_START+i*MODESET_SIZE)==FLASH_IDENTDATA
            && FlashReadByte(MODESET_START+i*MODESET_SIZE+MODESET_SIZE-1)==FLASH_CHECKSUM
            && FlashReadByte(MODESET_START+i*MODESET_SIZE+1)==index)
        {
            break;
        }
    }

    if(i >= MAX_MODESET_NUM)
    {
        FLASH_printMsg("_FRMS_ RModeSet Not Found");
        return FALSE;
    }

	FLASH_printData("_FRMS_ RModeSet=%x",i);
	FLASH_printData("_FRMS_ ModeR=%x",(MODESET_START+i*MODESET_SIZE)&0xFFFF);
	Flash_ReadTbl(MODESET_START+i*MODESET_SIZE, buf, MODESET_SIZE);
    return TRUE;
}

#endif//USEFLASH



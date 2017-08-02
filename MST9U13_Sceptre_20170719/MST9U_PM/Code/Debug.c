#define _DEBUG_C_
#include "types.h"
#include "Common.h"
#include "debugDef.h"
#include "global.h"
#include "board.h"
#include "ms_reg.h"
#include "ms_rwreg.h"
#include "misc.h"
#include "debug.h"
#include "Common.h"
#include "gpio_def.h"
#include "Mcu.h"
#include "NVRam.h"
#include "Misc.h"

#if ENABLE_DEBUG
#message "Please remember to disable the debug function!"//Zhifeng.Wu Please don't canel it,just only a message,thanks.
#endif

//BYTE xdata SIORxIndex = 0;
//BYTE xdata ReSyncCounter;
extern Bool xdata bFlashWriteFactory;
#define DEBUG_Massage    1
#if ENABLE_DEBUG&&DEBUG_Massage
    #define DB_printData(str, value)   printData(str, value)
    #define DB_printMsg(str)           printMsg(str)
#else
    #define DB_printData(str, value)
    #define DB_printMsg(str)
#endif


void putSIOChar( unsigned char sendData )
{
   if(!UART_READ_ES())
        return;

    UART_BUFFER() = sendData;
    while( 1 )
    {
        if( EA && UART_READ_ES() )
        {
            if( TI0_FLAG )
            {
                TI0_FLAG = 0;
                break;
            }
        }
        else
        {
            if(UART_TI_FLAG())
            {
                UART_CLR_TI_FLAG();
                break;
            }
        }
    }
}

#if ENABLE_R2_INT_M51_Test
void TestCommand_FIQ_INT(BYTE ucPara1)
{
        u8HostS2D=ucPara1;
        DB_printData("u8HostS2D==%d",u8HostS2D);
    switch (ucPara1)
    {
        case hst3to0_int:
		msWriteByteMask(REG_10190E, 0x00, BIT2);
		msWriteByteMask(REG_10054C, BIT0, BIT0);
		DB_printData("REG_10190E==%x",msReadByte(REG_10190E));
		DB_printData("REG_10054C==%x",msReadByte(REG_10054C));
		DB_printData("REG_10191E==%x",msReadByte(REG_10191E));
            break;
        case hst3to1_int:
		msWriteByteMask(REG_10190E, 0x00, BIT1);
		msWriteByteMask(REG_10054C, BIT1, BIT1);
		DB_printData("REG_10190E==%x",msReadByte(REG_10190E));
		DB_printData("REG_10054C==%x",msReadByte(REG_10054C));
		DB_printData("REG_10191E==%x",msReadByte(REG_10191E));
            break;
        case hst3to2_int:
		msWriteByteMask(REG_10190E, 0x00, BIT0);
		msWriteByteMask(REG_10054C, BIT2, BIT2);
		DB_printData("REG_10190E==%x",msReadByte(REG_10190E));
		DB_printData("REG_10054C==%x",msReadByte(REG_10054C));
		DB_printData("REG_10191E==%x",msReadByte(REG_10191E));
            break;

        case hst2to0_int:
		msWriteByteMask(REG_10190D, 0x00, BIT6);
		msWriteByteMask(REG_100548, BIT0, BIT0);
		DB_printData("REG_10190D==%x",msReadByte(REG_10190D));
		DB_printData("REG_100548==%x",msReadByte(REG_100548));
		DB_printData("REG_10191D==%x",msReadByte(REG_10191D));
            break;
        case hst2to1_int:
		msWriteByteMask(REG_10190D, 0x00, BIT5);
		msWriteByteMask(REG_100548, BIT1, BIT1);
		DB_printData("REG_10190D==%x",msReadByte(REG_10190D));
		DB_printData("REG_100548==%x",msReadByte(REG_100548));
		DB_printData("REG_10191D==%x",msReadByte(REG_10191D));
            break;
        case hst2to3_int:
		msWriteByteMask(REG_10190D, 0x00, BIT4);
		msWriteByteMask(REG_100548, BIT2, BIT2);
		DB_printData("REG_10190D==%x",msReadByte(REG_10190D));
		DB_printData("REG_100548==%x",msReadByte(REG_100548));
		DB_printData("REG_10191D==%x",msReadByte(REG_10191D));
            break;

        case hst1to0_int:
		msWriteByteMask(REG_10190D, 0x00, BIT2);
		msWriteByteMask(REG_100544, BIT0, BIT0);
		DB_printData("REG_10190D==%x",msReadByte(REG_10190D));
		DB_printData("REG_100544==%x",msReadByte(REG_100544));
		DB_printData("REG_10191D==%x",msReadByte(REG_10191D));
            break;
        case hst1to2_int:
		msWriteByteMask(REG_10190D, 0x00, BIT1);
		msWriteByteMask(REG_100544, BIT1, BIT1);
		DB_printData("REG_10190D==%x",msReadByte(REG_10190D));
		DB_printData("REG_100544==%x",msReadByte(REG_100544));
		DB_printData("REG_10191D==%x",msReadByte(REG_10191D));
            break;
        case hst1to3_int:
		msWriteByteMask(REG_10190D, 0x00, BIT0);
		msWriteByteMask(REG_100544, BIT2, BIT2);
		DB_printData("REG_10190D==%x",msReadByte(REG_10190D));
		DB_printData("REG_100544==%x",msReadByte(REG_100544));
		DB_printData("REG_10191D==%x",msReadByte(REG_10191D));
            break;

        case hst0to1_int:
		msWriteByteMask(REG_10190C, 0x00, BIT6);
		msWriteByteMask(REG_100540, BIT0, BIT0);
		DB_printData("REG_10190C==%x",msReadByte(REG_10190C));
		DB_printData("REG_100540==%x",msReadByte(REG_100540));
		DB_printData("REG_10191C==%x",msReadByte(REG_10191C));
            break;
        case hst0to2_int:
		msWriteByteMask(REG_10190C, 0x00, BIT5);
		msWriteByteMask(REG_100540, BIT1, BIT1);
		DB_printData("REG_10190C==%x",msReadByte(REG_10190C));
		DB_printData("REG_100540==%x",msReadByte(REG_100540));
		DB_printData("REG_10191C==%x",msReadByte(REG_10191C));
            break;
        case hst0to3_int:
		msWriteByteMask(REG_10190C, 0x00, BIT4);
		msWriteByteMask(REG_100540, BIT2, BIT2);
		DB_printData("REG_10190C==%x",msReadByte(REG_10190C));
		DB_printData("REG_100540==%x",msReadByte(REG_100540));
		DB_printData("REG_10191C==%x",msReadByte(REG_10191C));
            break;
        default:
			msWriteByte(REG_10190C,0xFF);
			msWriteByte(REG_10190D,0xFF);
			msWriteByte(REG_10190E,0xFF);

			msWriteByte(REG_10054C,0x00);
			msWriteByte(REG_100548,0x00);
			msWriteByte(REG_100544,0x00);
			msWriteByte(REG_100540,0x00);
			DB_printMsg("Disable nonPM FIQ mark and enable it");
        break;

    }

}
#endif
XDATA BYTE ucTempTbl[16];
#if ENABLE_SW_DOUBLE_BUFFER
void checkMenuloadWriteData(void)
{
	BYTE i;
	DB_Mode(eDB_NORMAL_MODE);
	for(i=0; i<0xFF; i++)
	{
        	DB_WB(REG_003D00+i,i);
	}
	#if ENABLE_SW_DOUBLE_BUFFER
		msSWDBWriteToRegister();
	#endif

	#if ENABLE_SW_DOUBLE_BUFFER
		msSWDBWriteToRegister();
		msSWDBWaitForRdy();
	#endif        
	
	for(i=0; i<0x10; i++)
	{
        	if (msReadByte(REG_003D00+i)==i)
    		{
	        	//printData("i===%x", i);
	        	DB_printData("msReadByte(REG_003D00+i)===%x", msReadByte(REG_003D00+i));
    		}
	}
}
#endif
void ExecTestCommand( void )
{
    BYTE ucPara1 = SIORxBuffer[2];
    BYTE ucPara2 = SIORxBuffer[3];
    BYTE ucPara3 = SIORxBuffer[4];
    BYTE i;

    switch( SIORxBuffer[1] )
    {
        case 0x00:
            //SPI mode 0~8
            if(ucPara1 > 8)
                ucPara1 = 8;
            else if(ucPara1 < 0)
                ucPara1 = 0;
            mcuSetSpiSpeed(ucPara1);
            break;
        case 0xFC:
            NVRam_WriteWord((ucPara1<<8|ucPara1), ucPara3);
            break;
        case 0xFD:
            if(ucPara1 == 0x00)
            {
                for(i=0; i<16; i++)
                {
                    ucTempTbl[i] = 0xFF-i;
                }
            }
            else if(ucPara1 == 0x01)
            {
                for(i=0; i<16; i++)
                {
                    ucTempTbl[i] = 0x00;
                }
            }
            else if(ucPara1 == 0x02)
            {
                NVRam_ReadTbl(0x0000, (BYTE *)&ucTempTbl, 16);
            }
            else if(ucPara1 == 0x03)
            {
                Clr_EEPROM_WP();
                NVRam_WriteTbl(0x0000, (BYTE *)&ucTempTbl, 16);
            }
            else if(ucPara1 == 0xFF)
            {
                for(i=0; i<16; i++)
                {
                    DB_printData("0x%x", ucTempTbl[i]);
                }
            }
            break;

#if ENABLE_DEBUG
        case 0x01:
        {
            WORD addr;
            for(addr=0x4000; addr<0x4200 ; addr++)
                msWriteByte(addr, 0xAA);
            for(addr=0x4800; addr<0x5100 ; addr++)
                msWriteByte(addr, 0xAB);
            for(addr=0x5F00; addr<0x5FFF ; addr++)
                msWriteByte(addr, 0xAC);
            DB_printMsg("test                   for    SRAM");
            break;
        }

        case 0x02:
        {
            mcuDMAMemoryFill(CHANNEL_1,0,0x100000,0x40,0xAABBCCDD);
            DB_printMsg("BDMA pattern fill !!!!!!!");
            break;
        }
        case 0x03:
        {
            DB_printMsg("BDMA pattern search !!!!!!!");
            DB_printData("Result= 0x%x",mcuDMAPatternSearch(CHANNEL_0,0,0xFFFF0,0x100,0xAABBCCDD));
        }

#if DEBUG_MCU_GPIO
        //BYTE i;
        //Open-drain:REG_1021/22/23/27/2A/2D[1:0] set to 2'b00 and REG_1029/2C/2E[1:0] set to 2'b11
        //Push-pull: REG_1021/22/23[1:0] need set to 2'b00 and REG_1027/2A/2D[1:0] need set to 2'b11
        //P10 and P11: GPIO34/35
        //P20 and P21: GPIO32/33
        //P30 and P31: GPIO40/41
        case 0x04:
        {
            BYTE temp;
            if(ucPara1==0x01)
            {
                DB_printMsg("P1");
                P1_0 = P1_1 = P1_2 = P1_3 = P1_4 = P1_5 = P1_6 = P1_7 = 0;
                msWriteByte(REG_00104E, 0x00);          //cltr init
                msWriteByte(REG_001050, 0x00);          //oe init

                msWriteBit(REG_000417, _ENABLE, _BIT2);
                msWriteByte(REG_001042, 0x00);          //ov
                switch(ucPara2)
                {
                    case 0x00:
                        temp = _BIT0;
                        P1_0 = 1;
                        break;
                    case 0x01:
                        temp = _BIT1;
                        P1_1 = 1;
                        break;
                    case 0x02:
                        temp = _BIT2;
                        P1_2 = 1;
                        break;
                    case 0x03:
                        temp = _BIT3;
                        P1_3 = 1;
                        break;
                    case 0x04:
                        temp = _BIT4;
                        P1_4 = 1;
                        break;
                    case 0x05:
                        temp = _BIT5;
                        P1_5 = 1;
                        break;
                    case 0x06:
                        temp = _BIT6;
                        P1_6 = 1;
                        break;
                    case 0x07:
                        temp = _BIT7;
                        P1_7 = 1;
                        break;
                }
                msWriteBit(REG_00104E, _ENABLE, temp);
                msWriteBit(REG_001050, _ENABLE, temp);
                DB_printData("P1_%x",ucPara2);
            }
            else if(ucPara1==0x03)
            {
                DB_printMsg("P3");
                P3_0 = P3_1 = P3_2 = 0;
                msWriteBit(REG_000417, _ENABLE, _BIT3);
                msWriteByte(REG_001046, 0x00);
                msWriteByte(REG_00105A, 0x00);
                msWriteByte(REG_00105C, 0x00);
                switch(ucPara2)
                {
                    case 0x00:
                        temp = _BIT0;
                        P3_0 = 1;
                        break;
                    case 0x01:
                        temp = _BIT1;
                        P3_1 = 1;
                        break;
                    case 0x02:
                        temp = _BIT2;
                        P3_2= 1;
                        break;
                }
                msWriteBit(REG_00105A, _ENABLE, temp);
                msWriteBit(REG_00105C, _ENABLE, temp);
                DB_printData("P3_%x",ucPara2);
            }
            //msWriteByte(REG_00104E, 0x01);
            //msWriteByte(REG_001050, 0x01);
            break;
        }
#endif
        case 0x05:
        {
#if ENABLE_HK_CODE_ON_DRAM
            mcuDMADownloadCode(CHANNEL_0,DEST_MIU0,DMA_TO_DRAM_SOURCE_ADDR, HK_CODE_ADDR, DMA_TO_DRAM_BYTE_COUNT);
            mcuArrangeCodeAddr(_DRAM, MCU_ON_DRAM_START_ADDR , MCU_ON_DRAM_END_ADDR);
            mcuArrangeCodeAddr(_SPI, MCU_ON_SPI_START_ADDR, MCU_ON_SPI_END_ADDR);
#endif
            break;
        }
        case 0x06:
        {
#if ENABLE_HK_CODE_ON_SPI_DRAM_PSRAM    //this function must allocate right code area
            mcuArrangeCodeAddr(_SPI, MCU_ON_SPI_START_ADDR, MCU_ON_SPI_END_ADDR);
#endif

#if ENABLE_HK_CODE_ON_SPI_DRAM_PSRAM
            mcuXdataMapToSRAM((PSRAM_MAP_XDATA_START>>10),(PSRAM_MAP_XDATA_END>>10));
            codeMoveToXdata(FLASH_TO_XDATA_START, PSRAM_MAP_XDATA_START, FLASH_TO_XDATA_COUNT);
            mcuDMADownloadCode(CHANNEL_0,DEST_MIU0,DMA_TO_DRAM_SOURCE_ADDR, HK_CODE_ADDR, DMA_TO_DRAM_BYTE_COUNT);  //spi to dram

            mcuArrangeCodeAddr(_DRAM, MCU_ON_DRAM_START_ADDR , MCU_ON_DRAM_END_ADDR);
            mcuArrangeCodeAddr(_PSRAM, MCU_ON_PSRAM_START_ADDR, MCU_ON_PSRAM_END_ADDR);
#endif
            break;
        }
        case 0x07:          //for PSRAM 1K mapping
        {
            WORD i;

            msWriteByte(REG_0010E0, (PSRAM_MAP_XDATA_START+0x400)>>10);
            msWriteByte(REG_0010E1, PSRAM_MAP_XDATA_START>>10);
            msWriteByte(REG_0010E4, 0x00);

            msWriteBit(REG_0010E6, _ENABLE, _BIT3);
            DB_printMsg("1:1k mapping");
            P2 = 0;
            for(i=0; i<0x800; i++)
            {
                if(i < 0x400)
                    msRegs[PSRAM_MAP_XDATA_START+i]=0xAA;
                else
                    msRegs[PSRAM_MAP_XDATA_START+i]=0xCC;
            }

            for(i=0; i<0x1f; i++)
                DB_printData("address = %x",msRegs[PSRAM_MAP_XDATA_START+i]);
            DB_printMsg("2:1k mapping");
            break;
        }

        case 0x08:
            DB_printData("PMState = %d",sPMInfo.ePMState);
            DB_printData("PMMode  = %d",sPMInfo.ucPMMode);
            break;

        case 0x09:  //for menuload sram
        {
            WORD i;
            msWriteByteMask(REG_0010C1, BIT7, BIT7);
            for(i=0x6000; i<0x6100; i++)
                msWriteByte(i, 0x75);
            for(i=0x6100; i<0x6200; i++)
                msWriteByte(i, 0x37);
            DB_printMsg("menuload sram is ok!!");
            break;
        }

        case 0x0A:  //for test Decrease move data step 1
        {
            mcuDMAMemoryFill(CHANNEL_0,0,0x100000,0x1000,0xAA);
            mcuDMAMemoryFill(CHANNEL_0,0,0x101000,0x1000,0x77);
            DB_printMsg("bdma fill datat!");
            break;
        }
        case 0x0B:  //for test Decrease move data step 2
        {
            msWriteByteMask(REG_100906, BIT0, BIT0);
            mcuDMADownloadCode(CHANNEL_0,DEST_MIU0,0x102FFF,0x101FFF,0x2000);
            DB_printMsg("BDMA decrease move data!");
            break;
        }

        case 0x0C:  //for test mailbox
        {
            BYTE i;
            for(i=0x00; i<=0x2F; i++)
                msWriteByte(REG_103380+i, i);
            DB_printMsg("write mailbox finish!");
            break;
        }
	#if ENABLE_R2_INT_M51_Test
        case 0x0D:  //for test mailbox
        {
            BYTE i;
            DB_printMsg("=======TEST nonPM FIQ interrupt==============!");
            TestCommand_FIQ_INT(ucPara1);
            break;
        }
	#endif
       case 0x11:
            u8Loop4us = ucPara1;
            break;
        case 0x12:
        {
            mcuDMADataTransfer(CHANNEL_0,SOURCE_MIU0,DEST_MIU0,0x80000,0x80200,0x400);
        }

        case 0x13:
            {
                    // write Pattern to DRAM first
                    XDATA DWORD result;
                    result = mcuDMACRC32(CHANNEL_0,SOURCE_MIU0,0x80000,0x40,0xB71DC104,0);
                    DB_printData("Check CRC Result_H: 0x%x",result>>16);
                    DB_printData("Check CRC Result_L: 0x%x",result);

            }
            break;
        case 0x14:
            {
                    XDATA DWORD result;
                    result = mcuDMAPatternSearch(CHANNEL_0,SOURCE_MIU0,0x80000,0x400,0xAABBCCDD);
                    DB_printData("Pattern Search Result_H: 0x%x",result>>16);
                    DB_printData("Pattern Search Result_L: 0x%x",result);
            }
            break;
        case 0x15:
            {
                    mcuDMAMemoryFill(CHANNEL_0,DEST_MIU0,0x80000,0x200,0xAABBCCDD);

            }
            break;
        case 0x16:
            {

                    mcuDMADownloadCode(CHANNEL_0,DEST_MIU0,0x0000,0x80000,0x20000);
            }
            break;
        case 0x17:
            {
                    mcuDMADataTransfer(CHANNEL_0,SOURCE_MIU0,DEST_MIU0,0x80000,0x80100,0x20);
                    mcuDMADataTransfer(CHANNEL_1,SOURCE_MIU0,DEST_MIU0,0x80110,0x80010,0x20);
                    mcuDMADataTransfer(CHANNEL_0,SOURCE_MIU0,DEST_MIU0,0x80020,0x80120,0x20);
                    mcuDMADataTransfer(CHANNEL_1,SOURCE_MIU0,DEST_MIU0,0x80130,0x80030,0x20);
                    mcuDMADataTransfer(CHANNEL_0,SOURCE_MIU0,DEST_MIU0,0x80040,0x80140,0x20);
                    mcuDMADataTransfer(CHANNEL_1,SOURCE_MIU0,DEST_MIU0,0x80150,0x80050,0x20);
            }
            break;

	#if FPGA_HW_PMMODE
            case 0x18:
            {
            	extern void msPM_SetFlagDPMS(void); //Command 18 01 force to DPMS mode
            	msPM_SetFlagDPMS();
            	DB_printMsg("msPM_SetFlagDPMS ");
            }
            break;
            case 0x19:
            {
            	extern void msPM_SetFlagDPMS(void);//Write MCU bank to check if MCU bank can be reseted while PM wakes up.
            	msWriteByte(0x0010C0, 0x11);
            	DB_printData("0x0010C0==%x ",msReadByte(0x0010C0));
            }
            break;

	#endif



        case 0x50:
            if( DebugOnlyFlag )
            {
                Clr_DebugOnlyFlag();
                DB_printMsg( "<Free>" );
            }
            else
            {
                Set_DebugOnlyFlag();
                DB_printMsg( "<Debug>" );
            }
            break;

	#if ENABLE_SW_DOUBLE_BUFFER
            case 0x51:
            {
            	checkMenuloadWriteData();
            }
            break;
	#endif


			
#endif // ENABLE_DEBUG

        default:
        break;
    }
}

//I2C_SCL_Pin               // GPIO00
//I2C_SDA_Pin               // GPIO01
#undef Init_hwI2C_SCL_Pin
#define Init_hwI2C_SCL_Pin()        (MEM_MSWRITE_BIT(_REG_GPIO0_OEZ, TRUE, BIT0))
#undef Init_hwI2C_SDA_Pin
#define Init_hwI2C_SDA_Pin()        (MEM_MSWRITE_BIT(_REG_GPIO0_OEZ, TRUE, BIT1))

#define DSet_i2c_SDA()               (MEM_MSWRITE_BIT(_REG_GPIO0_OEZ, TRUE, BIT1))
#define DClr_i2c_SDA()               (MEM_MSWRITE_BIT(_REG_GPIO0_OUT, FALSE, BIT1), MEM_MSWRITE_BIT(_REG_GPIO0_OEZ, FALSE, BIT1))
#define Di2c_SDAHi()                 (_bit1_(MEM_MSREAD_BYTE(_REG_GPIO0_IN)))
#define Di2c_SDALo()                 (!Di2c_SDAHi())
#define DSet_i2c_SCL()               (MEM_MSWRITE_BIT(_REG_GPIO0_OEZ, TRUE, BIT0))
#define DClr_i2c_SCL()               (MEM_MSWRITE_BIT(_REG_GPIO0_OUT, FALSE, BIT0), MEM_MSWRITE_BIT(_REG_GPIO0_OEZ, FALSE, BIT0))
#define Di2c_SCLHi()                 (_bit0_(MEM_MSREAD_BYTE(_REG_GPIO0_IN)))
#define Di2c_SCLLo()                 (!Di2c_SCLHi())

typedef enum _I2cIoTransType
{
    I2C_TRANS_READ,
    I2C_TRANS_WRITE
}I2cIoTransType;

#define I2C_DEVICE_ADR_WRITE(slave_adr)   (slave_adr & ~_BIT0)
#define I2C_DEVICE_ADR_READ(slave_adr)    (slave_adr | _BIT0)

#define I2C_ACCESS_DUMMY_TIME   3

#define FWI2C_ACKNOWLEDGE           TRUE
#define FWI2C_NON_ACKNOWLEDGE       FALSE
#if 1//(USE_SW_I2C == 1)
//====================================
// Setup i2c Start condition
Bool Di2c_Start( void )
{
    DSet_i2c_SDA();
    DSet_i2c_SCL();
    Delay4us();
    if( Di2c_SDALo() || Di2c_SCLLo() )
        return FALSE;
    DClr_i2c_SDA();
    Delay4us();
    DClr_i2c_SCL();
    Delay4us();
    return TRUE;
}
//=============================================
// Setup i2c Stop condition
void Di2c_Stop( void )
{
    // SCL=L, SDA=L, Stop condition.
    DClr_i2c_SCL();
    DClr_i2c_SDA();
    Delay4us();
    DSet_i2c_SCL();
    Delay4us();
    DSet_i2c_SDA();
    Delay4us();
}
//============================================
Bool DWait_i2C_Ack( void )
{
    BYTE i;
    for( i = 0; i < 5; i++ )
    {
        if( Di2c_SDALo() )
            return TRUE;
        Delay4us();
    }

    if( Di2c_SDALo() )
        return TRUE;
    else
        return FALSE;

}
//============================================
Bool Di2c_SendByte( BYTE value )
{
    BYTE i;
    Bool result;

    for( i = 0; i < 8; i++ ) // Send data via i2c pin
    {
        if( value & BIT7 )
            DSet_i2c_SDA();
        else
            DClr_i2c_SDA();
        Delay4us();
        DSet_i2c_SCL();
        Delay4us();
        value <<= 1;
        DClr_i2c_SCL();
    }
    DSet_i2c_SDA();
    result = DWait_i2C_Ack();
    DSet_i2c_SCL();
    Delay4us();
    DClr_i2c_SCL();
    Delay4us();
    DClr_i2c_SDA();

    return result;
}

//============================================
BYTE Di2c_ReceiveByte( const Bool ack )
{
    BYTE i;
    BYTE value = 0;

    for( i = 0; i < 8; i++ )
    {
        value <<= 1;
        DSet_i2c_SDA();
        Delay4us();
        DSet_i2c_SCL();
        Delay4us();
        if( Di2c_SDAHi() )
            value |= BIT0;
        DClr_i2c_SCL();
    }
    if( ack )
    {
        DClr_i2c_SDA();
    }
    else
    {
        DSet_i2c_SDA();
    }
    Delay4us();
    DSet_i2c_SCL();
    Delay4us();
    DClr_i2c_SCL();

    return value;
}

void Di2c_Start1(void)
{
    DSet_i2c_SDA();
    Delay4us();
    DSet_i2c_SCL();
    Delay4us();
    DClr_i2c_SDA();
    Delay4us();
    DClr_i2c_SCL();
}
void Di2C_Intial(void)
{
    BYTE i, j;

    for(i=0 ;i < 20; i++)   // Generate SCL signals to reset EEPROM.
    {
        DSet_i2c_SCL();
        Delay4us();
        DClr_i2c_SCL();
        Delay4us();
    }

    j = 20;

    while( j-- )    // Male EEPROM to Release I2C bus.
    {
        for(i=0 ;i < 9; i++)    // 9 STARTs
        {
            Di2c_Start1();
        }
        Di2c_Stop();
        ForceDelay1ms(10);

        if(Di2c_SDAHi())
            break;
    }

}

#endif


BOOL i2c_AccessStart(BYTE ucSlaveAdr, I2cIoTransType trans_t)
{
    BYTE ucDummy; // loop dummy

    if (trans_t == I2C_TRANS_READ) // check i2c read or write
        ucSlaveAdr = I2C_DEVICE_ADR_READ(ucSlaveAdr); // read
    else
        ucSlaveAdr = I2C_DEVICE_ADR_WRITE(ucSlaveAdr); // write
    ucDummy = I2C_ACCESS_DUMMY_TIME;
    while (ucDummy--)
    {
        Di2c_Start();

        if (Di2c_SendByte(ucSlaveAdr) == FWI2C_ACKNOWLEDGE) // check acknowledge
            return TRUE;
        Di2c_Stop();

        Delay1ms(1);
    } // while
    return FALSE;
}

void i2cBurstReadBytes(BYTE ucSlaveAdr, BYTE ucSubAdr, BYTE *pBuf, BYTE ucBufLen)
{
    BYTE ucDummy; // loop dummy

    ucDummy = I2C_ACCESS_DUMMY_TIME;
    while(ucDummy--)
    {
        if (i2c_AccessStart(ucSlaveAdr, I2C_TRANS_WRITE) == FALSE)
        {
            continue;
        }
        if (Di2c_SendByte(ucSubAdr) == FWI2C_NON_ACKNOWLEDGE) // check non-acknowledge
        {
            continue;
        }
        if (i2c_AccessStart(ucSlaveAdr, I2C_TRANS_READ) == FALSE)
        {
            continue;
        }

        while(ucBufLen) // loop to burst read
        {
            ucBufLen--;
            *pBuf = Di2c_ReceiveByte(ucBufLen); // receive byte

            pBuf++; // next byte pointer
        } // while

        break;
    } // while

    Di2c_Stop();

    while(ucBufLen--) // if read error, fill 0xff
    {
        *pBuf = 0xff; // clear
        pBuf++; // next byte pointer
    } // while
}

void i2cBurstWriteBytes(BYTE ucSlaveAdr, BYTE ucSubAdr, BYTE *pBuf, BYTE ucBufLen)
{
    BYTE ucDummy; // loop dummy

    ucDummy = I2C_ACCESS_DUMMY_TIME;
    while(ucDummy--)
    {
        if (i2c_AccessStart(ucSlaveAdr, I2C_TRANS_WRITE) == FALSE)
            continue;

        if (Di2c_SendByte(ucSubAdr) == FWI2C_NON_ACKNOWLEDGE) // check non-acknowledge
            continue;

        while(ucBufLen--) // loop of writting data
        {
            Di2c_SendByte(*pBuf); // send byte

            pBuf++; // next byte pointer
        } // while

        break;
    } // while

    Di2c_Stop();
}


BYTE i2cReadByte(BYTE ucSlaveAdr, BYTE ucSubAdr)
{
    BYTE ucBuf; // byte buffer

    i2cBurstReadBytes(ucSlaveAdr, ucSubAdr, &ucBuf, 1);
    return ucBuf;
}

void i2cWriteByte(BYTE ucSlaveAdr, BYTE ucSubAdr, BYTE ucVal)
{
    i2cBurstWriteBytes(ucSlaveAdr, ucSubAdr, &ucVal, 1);        // wz@ fpga hang
}

void ProcessCommand( void )
{
    Di2C_Intial();
    switch( UART_CMD )
    {
#if 0
        case kWrite_MST_Bank_n:
        {
            {
                const volatile BYTE ucBank = scReadByte( SC0_00 ); // store original bank
                scWriteByte( SC0_00, UART_CMD_MS_BANK );
                scWriteByte( UART_CMD_MS_REGINDEX, UART_CMD_MS_REGDATA );
                scWriteByte( SC0_00, ucBank ); // restore original bank
            }
            putSIOChar( 0xF1 );
            break;
        }

        case kRead_MST_Bank_n:
        {
            {
                const volatile BYTE ucBank = scReadByte( SC0_00 ); // store original bank
                scWriteByte( SC0_00, UART_CMD_MS_BANK );
                UART_CMD_MS_REGDATA = scReadByte( UART_CMD_MS_REGINDEX );
                scWriteByte( SC0_00, ucBank ); // restore original bank
            }
            putSIOChar( 0xF2 );
            putSIOChar( UART_CMD_MS_REGDATA );
            break;
        }
#endif
        case kWrite_MCU_XDATA:
        {
            //msWriteByte( MAKEWORD( UART_CMD_MCU_IDX_H, UART_CMD_MCU_IDX_L ), UART_CMD_MCU_DATA );
            msRegs[MAKEWORD( UART_CMD_MCU_IDX_H, UART_CMD_MCU_IDX_L )] =  UART_CMD_MCU_DATA;
            putSIOChar( 0xF1 );
            break;
        }

        case kRead_MCU_XDATA:
        {
            //UART_CMD_MCU_DATA = msReadByte( MAKEWORD( UART_CMD_MCU_IDX_H, UART_CMD_MCU_IDX_L ) );
            UART_CMD_MCU_DATA = msRegs[( MAKEWORD( UART_CMD_MCU_IDX_H, UART_CMD_MCU_IDX_L ) )];
            putSIOChar( 0xF2 );
            putSIOChar( UART_CMD_MCU_DATA );
            break;
        }

        case kTest_Command:

        {
            ExecTestCommand();
            break;
        }
    #if !USEFLASH
        case kWrite_EEPROM:
        {
            WORD uwEEADDR;
            uwEEADDR = MAKEWORD(SIORxBuffer[1],SIORxBuffer[2]);
            NVRam_WriteByte(uwEEADDR, SIORxBuffer[3]);
            //if (!g_bDebugASCIICommandFlag)
                putSIOChar(0xF1);
                //DB_printData("EEPROM[0x%x]=", uwEEADDR);
                //DB_printData("0x%x \r\n", SIORxBuffer[_UART_CMD_INDEX3_]);

            break;
        }
        case kRead_EEPROM:
        {
        WORD uwEEADDR;
            uwEEADDR = MAKEWORD(SIORxBuffer[1],SIORxBuffer[2]);
            //uwEEADDR = g_UartCommand.Buffer[_UART_CMD_INDEX1_] & 0x0E;
            //uwEEADDR = (uwEEADDR << 7) + g_UartCommand.Buffer[_UART_CMD_INDEX2_];
        NVRam_ReadByte(uwEEADDR , &SIORxBuffer[3]);
            putSIOChar(0xF2);
            putSIOChar(SIORxBuffer[3]);
            //if (g_bDebugASCIICommandFlag)
            //{
                //DB_printData("EEPROM[0x%x]=", uwEEADDR);
                //DB_printData("0x%x \r\n", SIORxBuffer[_UART_CMD_INDEX3_]);
            //}
            break;
        }
        case uartI2C_Device_Write:
        {
            WORD adr;
            adr = SIORxBuffer[1];
            if (g_bDebugASCIICommandFlag)
            {
                DB_printData("w I2C 0x%x ", SIORxBuffer[1]);
                DB_printData("w [0x%x]=", SIORxBuffer[2]);
                DB_printData("w 0x%x \r\n", SIORxBuffer[3]);
            }

            i2cWriteByte( SIORxBuffer[1],SIORxBuffer[2],SIORxBuffer[3]);


            //i2c_WriteTBL( g_UartCommand.Buffer[_UART_CMD_INDEX1_],
            //  g_UartCommand.Buffer[_UART_CMD_INDEX2_],
            //  &g_UartCommand.Buffer[_UART_CMD_INDEX3_],1);
            //if (!g_bDebugASCIICommandFlag)
                putSIOChar(0xF1);
        }
        break;

    case uartI2C_Device_Read:
    //i2c_ReadTBL( NVRAM_DEVICE, addr, value, 1 );
    {
    // ======== for touch sensor =========
        //SIORxBuffer[3] = i2cReadByte( SIORxBuffer[1],  SIORxBuffer[2]);
        SIORxBuffer[3] = i2cReadByte( SIORxBuffer[1],  SIORxBuffer[2]);
        if (g_bDebugASCIICommandFlag)
        {
            DB_printData("I2C 0x%x ", SIORxBuffer[1]);
            DB_printData("[0x%x]=", SIORxBuffer[2]);
            DB_printData("0x%x \r\n", SIORxBuffer[3]);
        }

    //i2c_ReadTBL( g_UartCommand.Buffer[_UART_CMD_INDEX1_],
    //          g_UartCommand.Buffer[_UART_CMD_INDEX2_],
    //          (BYTE *)&g_UartCommand.Buffer[_UART_CMD_INDEX3_] ,1);
        putSIOChar(0xF2);
        putSIOChar(SIORxBuffer[3]);
    }
    break;

    #endif
        default:
            break;
    }
}

Bool GetCommand( void )
{
    if ( (SIORxIndex >= UART_CMD_LENGTH) && (UART_CMD_LENGTH))
    {
        //Set_RxBusyFlag();
        return TRUE;
    }
    else
        return FALSE;
}
void DebugHandler( void )
{

    if( !UART_READ_ES())
        return;

    if( GetCommand() )
    {
        ProcessCommand();
        SIORxIndex = 0;
        //Clr_RxBusyFlag();
    }
}


#if UART1
void putSIOChar_UART1( unsigned char sendData )
{
   if(!UART1_READ_ES())
        return;

    UART1_BUFFER() = sendData;
    while( 1 )
    {
        if( EA && UART1_READ_ES() )
        {
            if( TI1_FLAG )
            {
                TI1_FLAG = 0;
                break;
            }
        }
        else
        {
            if(UART1_TI_FLAG())
            {
                UART1_CLR_TI_FLAG();
                break;
            }
        }
    }
}

void ExecTestCommand_UART1( void )
{
    BYTE ucPara1 = SIORxBuffer1[2];
    BYTE ucPara2 = SIORxBuffer1[3];
    BYTE ucPara3 = SIORxBuffer1[4];

    switch( SIORxBuffer1[1] )
    {
        case 0x00:
            printData_UART1("PrintBuf[2]: %d", ucPara1);
            printData_UART1("PrintBuf[3]: %d", ucPara2);
            printData_UART1("PrintBuf[4]: %d", ucPara3);
            printMsg_UART1("Uart1 Print Msg Test");
            break;

        case 0x50:
            if( DebugOnlyFlag )
            {
                Clr_DebugOnlyFlag();
                printMsg_UART1( "<Free - UART1 >" );
            }
            else
            {
                Set_DebugOnlyFlag();
                printMsg_UART1( "<Debug - UART1 >" );
            }
            break;
        default:
            break;
    }
}

void ProcessCommand_UART1( void )
{
    switch( UART1_CMD )
    {
        case kWrite_MCU_XDATA:
        {
            msRegs[MAKEWORD( UART1_CMD_MCU_IDX_H, UART1_CMD_MCU_IDX_L )] =  UART1_CMD_MCU_DATA;
            putSIOChar_UART1( 0xF1 );
            break;
        }

        case kRead_MCU_XDATA:
        {
            UART1_CMD_MCU_DATA = msRegs[( MAKEWORD( UART1_CMD_MCU_IDX_H, UART1_CMD_MCU_IDX_L ) )];
            putSIOChar_UART1( 0xF2 );
            putSIOChar_UART1( UART1_CMD_MCU_DATA );
            break;
        }

        case kTest_Command:
        {
            ExecTestCommand_UART1();
            break;
        }
        default:
            break;
    }
}

Bool GetCommand_UART1( void )
{
    if ( (SIORxIndex1 >= UART1_CMD_LENGTH) && (UART1_CMD_LENGTH))
    {
        //Set_RxBusyFlag();
        return TRUE;
    }
    else
        return FALSE;
}
void UART1_Handler( void )
{

    if( !UART1_READ_ES())
        return;

    if( GetCommand_UART1() )
    {
        ProcessCommand_UART1();
        SIORxIndex1 = 0;
        //Clr_RxBusyFlag();
    }
}
#endif

#if !ENABLE_DEBUG
BYTE code msDebugNullData[] = {0};
void msDebugDummy(void)
{
    BYTE xdata i = msDebugNullData[0];
}
#endif

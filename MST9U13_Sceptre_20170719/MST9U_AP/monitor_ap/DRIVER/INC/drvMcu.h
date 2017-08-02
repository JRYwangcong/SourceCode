
#ifndef _DRVMCU_H_
#define _DRVMCU_H_

///////////////////////////////////////
// DRVMCU_V VERSION: V01
////////////////////////////////////////
#ifdef _DRVMCU_C_
#define INTERFACE
#else
#define INTERFACE extern
#endif

#define R2_ADDR_MASK_MIU1   _BIT29      // => 64 address bits for reg_new_miu_size

#ifndef UART1
#define UART1       0
#endif
#define _SMOD       1
#define S0REL       (1024-((_SMOD*CRYSTAL_CLOCK+CRYSTAL_CLOCK)/SERIAL_BAUD_RATE)/64)
#define S1REL       (1024-(                      CRYSTAL_CLOCK/SERIAL_BAUD_RATE)/32)

#define    CLOCK_216MHZ    216000000ul
#define    CLOCK_MPLL_MHZ  172000000ul
#define    CLOCK_144MHZ    144000000ul
#define    CLOCK_108MHZ    108000000ul
#define    CLOCK_86MHZ      86000000ul
#define    CLOCK_54MHZ      54000000ul
#define    CLOCK_43MHZ      43000000ul
#define    CLOCK_36MHZ      36000000ul
#define    CLOCK_27MHZ      27000000ul
#define    CLOCK_48MHZ      48000000ul
#define    CLOCK_12MHZ      12000000ul
#define    CLOCK_XTAL    CRYSTAL_CLOCK
#define    CLOCK_4MHZ        4000000ul
#define    CLOCK_0MHZ              0ul

//mapping to g_mcuPLLFreqTable[]
#define    IDX_MCU_CLK_4MHZ      0
#define    IDX_MCU_CLK_12MHZ     1
#define    IDX_MCU_CLK_216MHZ    2
#define    IDX_MCU_CLK_MPLL_DIV  3
#define    IDX_MCU_CLK_108MHZ    4
#define    IDX_MCU_CLK_86MHZ     5
#define    IDX_MCU_CLK_54MHZ     6
#define    IDX_MCU_CLK_48MHZ     7
#define    IDX_MCU_CLK_XTAL      8

//not used
#define    IDX_MCU_CLK_144MHZ    9



#ifndef MCU_SPEED_INDEX
#error "Please define MCU_SPEED_INDEX!!!"
#endif
//Jison, used for decide the cpu clock in compile time
//#if MCU_SPEED_INDEX==IDX_MCU_CLK_RESERVED0
//#error "Wrong MCU_SPEED_INDEX define!!!"
//#endif

  #if MCU_SPEED_INDEX==IDX_MCU_CLK_144MHZ
  #error "Wrong MCU_SPEED_INDEX define!!!"
  #endif


// SPI Clock Selection item define
#define    IDX_SPI_CLK_4MHZ     0
#define    IDX_SPI_CLK_27MHZ    1
#define    IDX_SPI_CLK_36MHZ    2
#define    IDX_SPI_CLK_43MHZ    3
#define    IDX_SPI_CLK_54MHZ    4
#define    IDX_SPI_CLK_86MHZ    5
#define    IDX_SPI_CLK_SSC      6
#define    IDX_SPI_CLK_12MHZ    7
#define    IDX_SPI_CLK_XTAL     8


typedef enum
{
    SPI_MODE_NORMAL,
    SPI_MODE_FR,
    SPI_MODE_SADD,
    SPI_MODE_DADD,
    SPI_MODE_SAQD,
    SPI_MODE_QAQD
} SPI_ModeType;

typedef enum
{
    SPEED_4MHZ_MODE,
    SPEED_12MHZ_MODE,
    SPEED_XTAL_MODE,
    SPEED_NORMAL_MODE
} SystemSpeedMode;


#if (ENABLE_R2_CODE_ON_DRAM)
#define MCU_ON_DRAM_START_ADDR          (0UL)
#define HK_CODE_ON_SPI_START_ADDR       (0UL)
#define HK_CODE_SIZE                    (1024UL*64*7)

#if ENABLE_USB_INPUT
#define R2_CODE_USB_SIZE                (1024*128UL)
#else
#define R2_CODE_USB_SIZE                (0UL)
#endif

#define R2_CODE_2DTO3D_ON_SPI_START_ADDR    (HK_CODE_ON_SPI_START_ADDR+HK_CODE_SIZE) // 8th bank
#define R2_CODE_USB_ON_SPI_START_ADDR       (R2_CODE_2DTO3D_ON_SPI_START_ADDR+R2_CODE_2DTO3D_SIZE) // 6th bank

#if ENABLE_USB_INPUT
#define MCU_ON_DRAM_END_ADDR            (MCU_ON_DRAM_START_ADDR+R2_CODE_USB_SIZE)
#else
#define MCU_ON_DRAM_END_ADDR            (0UL)
#endif
#define MCU_ON_SPI_START_ADDR           (MCU_ON_DRAM_END_ADDR)
#define MCU_ON_SPI_END_ADDR             (MCU_ON_SPI_START_ADDR+(1024ul*64)*16)

#if ENABLE_USB_INPUT
#define DMA_TO_DRAM_SOURCE_ADDR         R2_CODE_USB_ON_SPI_START_ADDR
#endif
#define DMA_TO_DRAM_DESTIN_ADDR         MCU_ON_DRAM_START_ADDR
#define DMA_TO_DRAM_BYTE_COUNT          (MCU_ON_DRAM_END_ADDR-MCU_ON_DRAM_START_ADDR+1)
#endif

typedef enum
{
    _SPI,
    _DRAM,
} _CodeType;

////////////////////////////////
///// Struct MUST Sync with 51 /////
// use reserved 16th semaphore to record.
#define NUM_MAILBOX         0x60
#define IsMailBoxValid()    (msReadByte(REG_10181E)==0x02) // 1:R2, 2:51
#define SetMailBoxValid()   (msWriteByte(REG_10181E, 0x00),msWriteByte(REG_10181E, 0x01)) // 1:R2, 2:51
// use reserved 15th semaphore to record.
#define IsWDTAck()          (msReadByte(REG_10181C)!=0x01) // 1:R2, 2:51
#define SetWDTAck()         (msWriteByte(REG_10181C, 0x00),msWriteByte(REG_10181C, 0x01)) // 1:R2, 2:51
// use reserved 14th semaphore to record.
#define FreeRIU()           (msRegs(REG_10181A)=(0x00))//(msWriteByte(REG_10181A, 0x00)) // 1:R2, 2:51
#define IsRIUFree()         (msRegs(REG_10181A)==0x00)//(msReadByte(REG_10181A)==0x00) // 1:R2, 2:51
#define GetRIU()            (msRegs(REG_10181A)=(0x01))//(msWriteByte(REG_10181A, 0x01)) // 1:R2, 2:51
#define IsRIUGet()          (msRegs(REG_10181A)==0x01)//(msReadByte(REG_10181A)==0x01) // 1:R2, 2:51
// 11th for DAISYCHAIN  	#define DCCOMMAND_SEMID		0x0A
// 08th for BDMA 			#define BDMASEMID     		0x07
// 03rd for PREGE 			#define PREGESEMID			0x02
// 02nd for GE 				#define GESEMID				0x01

///// Struct MUST Sync with 51 /////
////////////////////////////////
#define Is51Active()        msDrvMcuCheck51Active()//(msRead3Byte(REG_0010FC)!=0x000000)
#define IsSelfRefreshBoot() ((msReadByte(REG_0003AA)&(BIT3|BIT2))==BIT3)

INTERFACE void msDrvMcuR2Miu1(void);
INTERFACE void msDrvMcuR2Reset51( void );
INTERFACE void msDrvMcuR2To51(void);
INTERFACE void msDrvMcuWakeUp51(void);
INTERFACE void msDrvMcuMailBoxRead(void);
INTERFACE void msDrvMcuMailBoxWrite(void);
INTERFACE void msDrvMcuMailBoxClear(void);
INTERFACE void msDrvMcuCheckSRBoot(void);
INTERFACE BOOL msDrvMcuCheck51Active(void);
INTERFACE void msDrvMcuXD2MIUAddrSet(DWORD dwAddr);
INTERFACE BYTE R2_XSRAM_READ(WORD u16Addr);
INTERFACE void R2_XSRAM_WRITE(WORD u16Addr, BYTE u8Data);
INTERFACE void DelayUs(BYTE u8Delayus);
INTERFACE void msAPI_InitWDT( BYTE bEnable );
INTERFACE void msAPI_ClearWDT( void );
INTERFACE void msAPI_TriggerWDT( void );
#if ENABLE_MSTV_UART_DEBUG
INTERFACE void mcuSetUartMux( UART_ENGINE ucUartEngine, UART_GPIO ucUartGPIO );
#endif

#undef INTERFACE
#endif


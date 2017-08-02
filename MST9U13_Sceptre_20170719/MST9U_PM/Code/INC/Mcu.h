#ifndef _MCU_H_
#define _MCU_H_

#ifdef _MCU_C_
#define _MCUDEC_
#else
#define _MCUDEC_    extern
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
#define    IDX_MCU_CLK_RESERVED0 7
#define    IDX_MCU_CLK_XTAL      8


//not used
#define    IDX_MCU_CLK_144MHZ    10



#ifndef MCU_SPEED_INDEX
#error "Please define MCU_SPEED_INDEX!!!"
#endif
//Jison, used for decide the cpu clock in compile time
#if MCU_SPEED_INDEX==IDX_MCU_CLK_RESERVED0
#error "Wrong MCU_SPEED_INDEX define!!!"
#endif


#if MCU_SPEED_INDEX==IDX_MCU_CLK_216MHZ
#define CPU_CLOCK_KHZ   (CLOCK_216MHZ/1000)
#elif MCU_SPEED_INDEX==IDX_MCU_CLK_MPLL_DIV
#define CPU_CLOCK_KHZ   (CLOCK_MPLL_MHZ/1000)
#elif MCU_SPEED_INDEX==IDX_MCU_CLK_144MHZ
#define CPU_CLOCK_KHZ   (CLOCK_144MHZ/1000)
#elif MCU_SPEED_INDEX==IDX_MCU_CLK_108MHZ
#define CPU_CLOCK_KHZ   (CLOCK_108MHZ/1000)
#elif MCU_SPEED_INDEX==IDX_MCU_CLK_86MHZ
#define CPU_CLOCK_KHZ   (CLOCK_86MHZ/1000)
#elif MCU_SPEED_INDEX==IDX_MCU_CLK_54MHZ
#define CPU_CLOCK_KHZ   (CLOCK_54MHZ/1000)
#elif MCU_SPEED_INDEX==IDX_MCU_CLK_27MHZ
#define CPU_CLOCK_KHZ   (CLOCK_27MHZ/1000)
#elif MCU_SPEED_INDEX==IDX_MCU_CLK_XTAL
#define CPU_CLOCK_KHZ   (CLOCK_XTAL/1000)
#elif MCU_SPEED_INDEX==IDX_MCU_CLK_4MHZ
#define CPU_CLOCK_KHZ   (CLOCK_4MHZ/1000)
#endif

// SPI Clock Selection item define
//(the following confirmed MST9U settings, register table might be wrong !!)
#define    IDX_SPI_CLK_4MHZ     0
#define    IDX_SPI_CLK_12MHZ    1
#define    IDX_SPI_CLK_27MHZ    2
#define    IDX_SPI_CLK_36MHZ    3
#define    IDX_SPI_CLK_43MHZ    4
#define    IDX_SPI_CLK_54MHZ    5
#define    IDX_SPI_CLK_86MHZ    6
#define    IDX_SPI_CLK_SSC      7
#define    IDX_SPI_CLK_XTAL     8

#if SPI_SPEED_INDEX==IDX_SPI_CLK_86MHZ
#define SPI_CLOCK_KHZ   (CLOCK_86MHZ/1000)
#elif SPI_SPEED_INDEX==IDX_SPI_CLK_54MHZ
#define SPI_CLOCK_KHZ   (CLOCK_54MHZ/1000)
#elif SPI_SPEED_INDEX==IDX_SPI_CLK_43MHZ
#define SPI_CLOCK_KHZ   (CLOCK_43MHZ/1000)
#elif SPI_SPEED_INDEX==IDX_SPI_CLK_36MHZ
#define SPI_CLOCK_KHZ   (CLOCK_36MHZ/1000)
#elif SPI_SPEED_INDEX==IDX_SPI_CLK_27MHZ
#define SPI_CLOCK_KHZ   (CLOCK_27MHZ/1000)
#elif SPI_SPEED_INDEX==IDX_SPI_CLK_12MHZ
#define SPI_CLOCK_KHZ   (CLOCK_12MHZ/1000)
#elif SPI_SPEED_INDEX==IDX_SPI_CLK_4MHZ
#define SPI_CLOCK_KHZ   (CLOCK_4MHZ/1000)
#elif SPI_SPEED_INDEX==IDX_SPI_CLK_XTAL
#define SPI_CLOCK_KHZ   (CLOCK_XTAL/1000)
#endif

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

extern void Init_WDT( BYTE bEnable );
extern void SetWDTClk( BYTE clk_live_sel );
extern void ClearWDT( void );
extern void TriggerWDT( BYTE clk_live_sel, BYTE second );
#if ENABLE_DEBUG
extern void mcuSetUartMux( UART_ENGINE ucUartEngine, UART_GPIO ucUartGPIO );
#endif
extern void mcuSetSpiSpeed( BYTE ucIndex );
extern void mcuSetSpiMode( BYTE ucMode );
extern void mcuSetMcuSpeed( BYTE ucSpeedIdx );
extern void mcuSetSystemSpeed(BYTE u8Mode);

#if (ENABLE_HK_CODE_ON_DRAM||ENABLE_HK_CODE_ON_SPI_DRAM_PSRAM)
#if ENABLE_HK_CODE_ON_DRAM
#define MCU_ON_DRAM_START_ADDR      0x00020000UL
#define MCU_ON_DRAM_END_ADDR        0x000FFFFFUL
#define MCU_ON_SPI_START_ADDR       0x00000000UL
#define MCU_ON_SPI_END_ADDR         0x0001FFFFUL
#define RESERVERD_AREA          0x800000UL
#define HK_CODE_ADDR            (((0UL)+RESERVERD_AREA+0xFFFFUL)&0xFFF0000L) // allocate in 64KB boundary// // allocate in 64KB boundary
#define DMA_TO_DRAM_SOURCE_ADDR     MCU_ON_DRAM_START_ADDR
#define DMA_TO_DRAM_DESTIN_ADDR     HK_CODE_ADDR
#define DMA_TO_DRAM_BYTE_COUNT      (MCU_ON_DRAM_END_ADDR-MCU_ON_DRAM_START_ADDR+1)

#elif ENABLE_HK_CODE_ON_SPI_DRAM_PSRAM
#define MCU_ON_SPI_START_ADDR       0x00000000UL
#define MCU_ON_SPI_END_ADDR         0x000034FFUL
#define MCU_ON_PSRAM_START_ADDR     0x00003500UL
#define MCU_ON_PSRAM_END_ADDR       0x000074FFUL
#define MCU_ON_DRAM_START_ADDR      0x00007500UL
#define MCU_ON_DRAM_END_ADDR        0x000FFFFFUL

#define RESERVERD_AREA          0x800000UL
#define MCU_TO_PSRAM_BYTE_COUNT     (MCU_ON_PSRAM_END_ADDR-MCU_ON_PSRAM_START_ADDR+1)
#define HK_CODE_ADDR            (((0UL)+RESERVERD_AREA+0xFFFFUL)&0xFFF0000L) // allocate in 64KB boundary// // allocate in 64KB boundary

#define DMA_TO_DRAM_SOURCE_ADDR     MCU_ON_DRAM_START_ADDR
#define DMA_TO_DRAM_DESTIN_ADDR     HK_CODE_ADDR
#define DMA_TO_DRAM_BYTE_COUNT      (MCU_ON_DRAM_END_ADDR-MCU_ON_DRAM_START_ADDR+1)
#endif
#else
#define RESERVERD_AREA          0UL
#define HK_CODE_ADDR            0UL
#endif

typedef enum
{
    _SPI,
    _DRAM,
    _PSRAM,
} _CodeType;

typedef enum
{
    CHANNEL_AUTO,
    CHANNEL_0,
    CHANNEL_1,
} BDMA_CHANNEL;
typedef enum
{
    SOURCE_MIU0 = 0,
    SOURCE_MIU1 = 1,
    SOURCE_MEMORY_FILL = 4,
    SOURCE_SPI = 5,

}BDMA_SOURCE_TYPE;
typedef enum
{
    DEST_MIU0 = 0,
    DEST_MIU1 = 1,
    DEST_PATTERN_SEARCH = 2,
    DEST_CRC32= 3,
    DEST_VDMCU = 6,
    DEST_DMDMCU = 7,
    DEST_DST = 8,
    DEST_HKMCU = 10,

}BDMA_DESTINATION_TYPE;

typedef enum
{
    DOWNLOAD_CODE,
    PATTERN_FILL,
    PATTERN_SEARCH,
} BDMA_CMD;
//////////////////////////////////////////////////////////////////////////////////
//WIN0_ADDR_START = 0xC000 ==> relative to MailBox
//////////////////////////////////////////////////////////////////////////////////
#define WIN0_ADDR_START         (0xC000>>10)//(0x6400>>10)  // map to xdata 0x6000~0xEFFF
#define WIN0_ADDR_END           (0xF000>>10)//(0xB000>>10)
#define WIN1_ADDR_START         WIN0_ADDR_END // map to xdata 0xF000~0xFFFF
#define WIN1_ADDR_END           (0xFFFF>>10)// (0xC000>>10)

#define PSRAM_MAP_XDATA_START   (0x0000)//(0xC000)
#define PSRAM_MAP_XDATA_END     (0x0000)//(0xEFFF)
#if ENABLE_HK_CODE_ON_SPI_DRAM_PSRAM
#define FLASH_TO_XDATA_START    (0x0000)
#define FLASH_TO_XDATA_COUNT    (0x4000)
#endif


////////////////////////////////
///// Struct MUST Sync with 51 /////
typedef struct
{ //MUST CARE 32 bits align & endian (WORD only)
    //////////////////////
    //MSTAR DRIVER AREA//
    WORD    u16PMMode;
    //MSTAR DRIVER AREA//
    //////////////////////

    //////////////////////
    /// CUSTOMER AREA ///
    WORD    u16PMPortIndex;
    WORD    u16PMPortSkipIndex;
    WORD    u16PMFlag;	
    WORD    u16PMCounter;      
    /// CUSTOMER AREA ///
    //////////////////////
}/*__attribute__((packed))*/sMAILBOX_R2;

typedef struct
{ //MUST CARE 32 bits align & endian (WORD only)
    //////////////////////
    //MSTAR DRIVER AREA//
    WORD    u16PMWakeupStatus;
    //MSTAR DRIVER AREA//
    //////////////////////

    //////////////////////
    /// CUSTOMER AREA ///
    WORD    u8PMActivePort;
    /// CUSTOMER AREA ///
    //////////////////////
}/*__attribute__((packed))*/sMAILBOX_51;

_MCUDEC_ XDATA sMAILBOX_R2 g_sMailBoxR2;
_MCUDEC_ XDATA sMAILBOX_51 g_sMailBox51;

// use reserved 16th semaphore to record.
#define NUM_MAILBOX         0x60
#define IsMailBoxValid()  (msReadByte(REG_10181E)==0x01) // 1:R2, 2:51
#define SetMailBoxValid() (msWriteByte(REG_10181E, 0x00),msWriteByte(REG_10181E, 0x02)) // 1:R2, 2:51
// use reserved 15th semaphore to record.
#define IsWDTAck()        (msReadByte(REG_10181C)!=0x02) // 1:R2, 2:51
#define SetWDTAck()       (msWriteByte(REG_10181C, 0x00),msWriteByte(REG_10181C, 0x02)) // 1:R2, 2:51
// use reserved 14th semaphore to record.
#define FreeRIU()           MEM_MSWRITE_BYTE(REG_10181A,0x00)//(msWriteByte(REG_10181A, 0x00)) // 1:R2, 2:51
#define IsRIUFree(ISR_SRC)  (MEM_MSREAD_BYTE##ISR_SRC(REG_10181A)==0x00)//(msReadByte(REG_10181A)==0x00) // 1:R2, 2:51
#define GetRIU()            MEM_MSWRITE_BYTE(REG_10181A,0x02)//(msWriteByte(REG_10181A, 0x02)) // 1:R2, 2:51
#define IsRIUGet(ISR_SRC)   (MEM_MSREAD_BYTE##ISR_SRC(REG_10181A)==0x02)//(msReadByte(REG_10181A)==0x02) // 1:R2, 2:51
// 11th for DAISYCHAIN  	#define DCCOMMAND_SEMID		0x0A
// 08th for BDMA 			#define BDMASEMID     		0x07
// 03rd for PREGE 			#define PREGESEMID			0x02
// 02nd for GE 				#define GESEMID				0x01

///// Struct MUST Sync with 51 /////
////////////////////////////////
_MCUDEC_ void msDrvMcu51ResetR2( void );
_MCUDEC_ void msDrvMcu51ToR2(void);
_MCUDEC_ void msDrvMcuMailBoxRead(void);
_MCUDEC_ void msDrvMcuMailBoxWrite(void);
_MCUDEC_ void msDrvMcuMailBoxClear(void);

extern void mcuInitXdataMapToDRAM(void);
extern void mcuXdataMapToDRAM(DWORD dwADDR);
extern void mcu40kXdataMapToDRAM(WORD w64k);
extern void mcu4kXdataMapToDRAM(DWORD dw4k);
extern void mcuDMADownloadCode(BYTE ch,BYTE DestinaType,DWORD dwSourceAddr,DWORD dwDestinAddr,DWORD dwByteCount);
extern void mcuXdataMapToSRAM(BYTE lbAddr, BYTE hbAddr);
extern void codeMoveToXdata(DWORD FStartAddr, DWORD XStartAddr, DWORD XCOUNT);
extern void mcuArrangeCodeAddr(BYTE ucType, DWORD dwAddrStart, DWORD dwAddrEnd);
extern void mcuDMADataTransfer(BYTE ch,BYTE SourceType,BYTE DestinaType,DWORD dwSourceAddr,DWORD dwDestinAddr,DWORD dwByteCount);
extern DWORD mcuDMACRC32(BYTE channel,BYTE SourceType,DWORD dwSourceAddr,DWORD dwByteCount,DWORD dwPolynimial,DWORD dwSeed);
extern void mcuDMAMemoryFill(BYTE channel,BYTE DestinaType,DWORD dwSourceAddr,DWORD dwByteCount,DWORD dwPattern);
extern DWORD mcuDMAPatternSearch(BYTE channel,BYTE SourceType,DWORD dwSourceAddr,DWORD dwByteCount,DWORD dwPattern);
extern DWORD BDMA_Operation(BYTE ch, BYTE SourceId, BYTE DestinId,DWORD dwSourceAddr, DWORD dwDestinAddr, DWORD dwByteCount, DWORD dwPattern, DWORD dwExternPattern);
#if ENABLE_DRAM_SELFREFRESH
extern void DeInit_Dram(void);
#endif
extern void Init_Dram(void);
_MCUDEC_ void Init_MCU( void );
_MCUDEC_ void Init_ExternalInterrupt( void );
_MCUDEC_ void Init_Timer( void );
_MCUDEC_ void Init_SerialPort( void );
_MCUDEC_ void Init_IOPorts( void );
//_MCUDEC_ void MCU_EnableCache(Bool u8Enable);

_MCUDEC_ void SetSPI_Quad_En(BYTE ucEnable);

#endif

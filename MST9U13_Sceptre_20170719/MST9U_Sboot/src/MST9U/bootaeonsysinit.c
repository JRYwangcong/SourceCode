/*------------------------------------------------------------------------------*/
/*  INCLUDE                                                                     */
/*------------------------------------------------------------------------------*/
#include "datatype.h"
#include "hwreg.h"
#include "drvGlobal.h"
#include "drvAeonUART.h"
#include "risc32_spr.h"
#include "drvPadConf.h"
#include "Board.h"
#if ENABLE_USB
#include "./usb/MApp_SwUpdate.h"
#endif

/*------------------------------------------------------------------------------*/
/*  VARAIBLES DECLARED IN LINKER SCRIPT                                         */
/*------------------------------------------------------------------------------*/
U16 *risc32_regbase;

extern unsigned char _text_length[];
extern unsigned char loader_2nd_lma[];
extern unsigned char loader_2nd_vma[];

extern unsigned char loader_2nd_len[];
extern unsigned char _sboot_end[];

#define BEON_MEM_ADR            0x200000
#define chunk_header_addr       0x20000
#define chunk_header_len        0x80

#define MHAL_DCACHE_SIZE        8192 // MST9U: 8K
#ifdef __AEONR2__
#define MHAL_DCACHE_WAYS        4 // MST9U: 4 way
#else
#define MHAL_DCACHE_WAYS        2
#endif
#define MHAL_DCACHE_LINE_SIZE   16 // ICACHE 32


void loadAP(void);
void BL_Init(void);

#define QMEM     ((unsigned  short volatile *) 0xB0000000ul)    //T3
#define MEM32(x) *((unsigned long volatile *)(x))
/*------------------------------------------------------------------------------*/
/*  LOCAL FUNCTIONS DECLARATION                                                 */
/*------------------------------------------------------------------------------*/
void load2ndLoader(U32 dst, U32 src, U32 size);

/*------------------------------------------------------------------------------*/
/*  LOCAL FUNCTIONS IMPLEMENTATION                                              */
/*------------------------------------------------------------------------------*/
void SetBdmaSrcDstDevice( U8 SrcDEv,U8 SrcDataWidth, U8 DstDEv , U8 DstDataWidth);
void SetBdmaSrcDstDevice( U8 SrcDEv,U8 SrcDataWidth, U8 DstDEv , U8 DstDataWidth)
{
   MDrv_Write2Byte( (BDMA_REG_BASE+0x04) ,  ( (DstDEv|DstDataWidth)  <<8) |(SrcDataWidth|SrcDEv) );

   //Both with 1 byte data width for FPGA test
}

/*------------------------------------------------------------------------------*/
/*  Copy binary image from SPI to DRAM                                          */
/*------------------------------------------------------------------------------*/
//#define REG8(a)       RIU8[((a) * 2) - ((a) & 1)]
void CopyFLH2MIU(U32 dst, U32 src, U32 size);
void CopyFLH2MIU(U32 dst, U32 src, U32 size)
{
    //printf("SPI %lx -> MIU %lx  length: %lx\n", spi_addr, miu_addr, length);

    while ( RIU8[(BDMA_REG_BASE + 0x02)*2]  & 0x01);   ///queue ==0?
    MDrv_Write4Byte((BDMA_REG_BASE + 0x08 ) , src  );
    MDrv_Write4Byte((BDMA_REG_BASE + 0x0c ),  dst  );
    MDrv_Write4Byte((BDMA_REG_BASE + 0x10 ), (size + 0x0f) & ~0x0fuL);
    MDrv_WriteByte((BDMA_REG_BASE + 0x06),0x00);   //increasing
    MDrv_WriteByte((BDMA_REG_BASE + 0x07),0x02);   //Set Dummy write count
    SetBdmaSrcDstDevice( SPI_FLASH , DW_16BYTE/*MST9U: 16Bytes*/, DRAM_MIU_0, DW_16BYTE/*MST9U: 16Bytes*/);
    MDrv_WriteByte( (BDMA_REG_BASE + 0x00), 0x01 );
    while(1) //for(k=0;k<12000;k++)
    {
        if ( (RIU8[(BDMA_REG_BASE+0x02)*2] & 0x08) == 0x08 )
        {
            return;        //pass
        }
    }
}

void CopyFLH2MIU1(U32 dst, U32 src, U32 size);
void CopyFLH2MIU1(U32 dst, U32 src, U32 size)
{
    //printf("SPI %lx -> MIU %lx  length: %lx\n", spi_addr, miu_addr, length);

    MDrv_Write2Byte((0x100600 + 0xF0 ), (BIT14|BIT5|BIT4|BIT3) ); //Set MIU_Sel of PM51, HK_R2, CP_R2, BDMA to HW mode, ie., choosen by clinet.
    MDrv_WriteByteMask((0x100400 + 0x03 ), 0x8A, 0xFE );   //force HK_R2 to MIU1
    MDrv_WriteByteMask((0x100480 + 0x03 ), 0x8A, 0xFE );   //force CP_R2 to MIU1

    while ( RIU8[(BDMA_REG_BASE + 0x02)*2]  & 0x01);   ///queue ==0?
    MDrv_Write4Byte((BDMA_REG_BASE + 0x08 ) , src  );
    MDrv_Write4Byte((BDMA_REG_BASE + 0x0c ),  dst  );
    MDrv_Write4Byte((BDMA_REG_BASE + 0x10 ), (size + 0x0f) & ~0x0fuL);
    MDrv_WriteByte((BDMA_REG_BASE + 0x06),0x00);   //increasing
    MDrv_WriteByte((BDMA_REG_BASE + 0x07),0x02);   //Set Dummy write count
    SetBdmaSrcDstDevice( SPI_FLASH , DW_16BYTE/*MST9U: 16Bytes*/, DRAM_MIU_1, DW_16BYTE/*MST9U: 16Bytes*/);
    MDrv_WriteByte( (BDMA_REG_BASE + 0x00), 0x01 );
    while(1) //for(k=0;k<12000;k++)
    {
        if ( (RIU8[(BDMA_REG_BASE+0x02)*2] & 0x08) == 0x08 )
        {
            return;        //pass
        }
    }
}

void BootFromDRAM(U32 u32BootAddr);
void BootFromDRAM(U32 u32BootAddr)
{
#if 1
    //set up reset vector base
    //printf("u32BootAddr=0x%x\r\n", u32BootAddr);
    MDrv_Write2Byte(0x100FB4, (U16)(u32BootAddr >> 16));
    //R2 boot from sdram
    MDrv_Write2Byte(0x100F80, 0x002F);

    __asm__ __volatile__ (
        "\tl.syncwritebuffer\n"
        "\tl.jr     %0\n"
        "\tl.syncwritebuffer\n"
        : : "r" (u32BootAddr));

    while(1);

#else
    register U32 reg_bak;

    //The following sequence is not supposed to be modified!!
    MDrv_Write2Byte(0x100FB4, u32BootAddr>>16 );
    while(MDrv_Read2Byte(0x100FB4) != (u32BootAddr>>16)) { } // Wait setting
    MDrv_Write2Byte(0x00039A, 0xB8D2); // Set password
    while(MDrv_Read2Byte(0x00039A) != 0xB8D2) { } // Wait setting
    MDrv_Write2Byte(0x00039C, MDrv_Read2Byte(0x00039C)&(~BIT15));  // non-fire wtd_mcu_rst
    asm __volatile__ ( "l.syncwritebuffer;" );
    asm __volatile__ ( "l.nop;" ); // For instruction alignment
    asm __volatile__ ( "l.nop;" ); // For instruction alignment
    asm __volatile__ ( "l.nop;" ); // For instruction alignment

    reg_bak = MDrv_Read2Byte(0x00039C);

    MDrv_Write2Byte(0x100F80, 0x004F);
    while(MDrv_Read2Byte(0x100F80) != 0x004F) { } // Wait setting

    //reset to dram: bellow 2 lines must in the same line 32bytes
    MDrv_Write2Byte(0x100F80, 0x002F);
    MDrv_Write2Byte(0x00039C, reg_bak|BIT15);

    while(1);
#endif
}

void SetSPI_Quad_En(void);
void SetSPI_Quad_En(void)
{
//    WREG(0x09C0) = 0x06;   //SPI Flash WREN Command
//    WREG(0x09C1) = 0x01;   //SPI Flash Write Command
    MDrv_Write2Byte(0x09C0, 0x0106);

//    WREG(0x09C2) = 0x00;   //ADR(23:16)
//    WREG(0x09C3) = 0x02;  //ADR(15:8)
    MDrv_Write2Byte(0x09C2, 0x0200);

//    WREG(0x09C4) = 0x05;   //SPI Flash RDSR Command
    MDrv_Write2Byte(0x09C4, 0x0005);


//    WREG(0x09D4) = 0x31;   //Write length: 1 Bytes / 8 Bytes / 1 Bytes
//    WREG(0x09D5) = 0x01;
    MDrv_Write2Byte(0x09D4, 0x0131);

//    WREG(0x09D6) = 0x00;   //Read length: 0 Bytes / 0 Bytes / 1 Bytes
//    WREG(0x09D7) = 0x01;
    MDrv_Write2Byte(0x09D6, 0x0100);

//    WREG(0x09D9) = 0xf0;   //enable second/third commands and auto check status
//    WREG(0x09D8) = 0x07;  //fsp enable and interrupt enable
    MDrv_Write2Byte(0x09D8, 0xf007);

//    WREG(0x09DA) = 0x01;        //Trigger
    MDrv_Write2Byte(0x09DA, 0x0001);

    //Done = RREG(0x09DC, Bit(0))  //Check FSP done flag
    //WREG(0x09DE, Bit(0)) = 1     //Clear FSP done flag
//    while(!(WREG(0x09DC)&BIT0))
    while(!(MDrv_ReadByte(0x09DC)&BIT0))
    {
    }
    MDrv_Write2Byte(0x09DE, (MDrv_ReadByte(0x09DE)|BIT0));     //Clear FSP done flag
}

#define    IDX_SPI_CLK_12MHZ    1
#define    IDX_SPI_CLK_27MHZ    2
#define    IDX_SPI_CLK_36MHZ    3
#define    IDX_SPI_CLK_43MHZ    4
#define    IDX_SPI_CLK_54MHZ    5
#define    IDX_SPI_CLK_86MHZ    6

#define SPI_CLOCK      IDX_SPI_CLK_43MHZ // need sync with PM code setting for AC_ON consistency

void SetSPI_Mode(void);
void SetSPI_Mode(void)
{
#define REG_SPI_BANK           (0xA00)
#define REG_SEL_MODE           (REG_SPI_BANK + 0x72*2)
#define REG_PADTOP_BANK        (0x400)
#define REG_SPI_QUAD_EN        (REG_PADTOP_BANK + 0x2*2)
#define REG_SPI_CLOCK           (REG_SPI_BANK+ 0xE0)

typedef enum spi_mode
{
    spi_normal,
    spi_fast,
    spi_single_dual,
    spi_dual_dual,
    spi_single_quad = 0xa,
    spi_quad_quad = 0xb
} spi_mode_t;

    U16  u16SpiMode = spi_fast; // need sync with PM code setting for AC_ON consistency

#if 1 // default 4mA
    MDrv_WriteByte(0x000413, 0xFF);  //spi drive
#else
    if(SPI_CLOCK > IDX_SPI_CLK_27MHZ)
        MDrv_WriteByte(0x000413, 0x02);  //spi drive
    else
        MDrv_WriteByte(0x000413, 0x01);  //spi drive
#endif

    //printf(" SPI_MODE=0x%x\r\n", MDrv_Read2Byte(REG_SEL_MODE));
    if (u16SpiMode >= 0xa)
    {
        MDrv_Write2Byte(REG_SEL_MODE, spi_fast); // for FSP (SetSPI_Quad_En()) not support quad mode
        SetSPI_Quad_En();
        MDrv_Write2Byte(REG_SPI_QUAD_EN, MDrv_Read2Byte(REG_SPI_QUAD_EN)|BIT11);
    }
    MDrv_Write2Byte(REG_SEL_MODE, u16SpiMode);

    MDrv_WriteByte(REG_SPI_CLOCK, ((SPI_CLOCK)<<3)|_BIT2); // spi clock

    printf(" SPI_MODE=0x%x\r\n", MDrv_Read2Byte(REG_SEL_MODE));

}

#if (ENABLE_DRAM_SELFREFRESH)
Bool TestSelfRefreshGuard( void );
Bool TestSelfRefreshGuard( void )
{
#define BIST_SIZE       0x200000
#define BIST_BASE_ADDR  0x800000

#if (CHIP_ID == MST9U3)
#define MIU_GROUP0_MASK     0xFFFE
#define MIU_GROUP2_MASK     0x7FFF
#else
#define MIU_GROUP0_MASK     0x7FFE
#define MIU_GROUP2_MASK     0xFFFF
#endif

    BYTE u8LoopMode  = 0;
    BYTE u8ReadOnly  = 1;
    BYTE u8WriteOnly = 0;

    WORD i, bist_try;
    WORD u16TestMode, u16ExtraMode;
    Bool bReturn = FALSE;

    bist_try = 5;

    u16TestMode  = (BIT2|BIT1); //BIST test mode [2:1]
    u16ExtraMode = u16TestMode;

    if (u8LoopMode)
        u16ExtraMode |= BIT4;
    if (u8ReadOnly)
        u16ExtraMode |= BIT8;
    if (u8WriteOnly)
        u16ExtraMode |= BIT9;

    MDrv_Write2Byte(0x101246, MIU_GROUP0_MASK); //mask other request
    MDrv_Write2Byte(0x101266, 0xffff); //mask other request
    MDrv_Write2Byte(0x101286, MIU_GROUP2_MASK); //mask other request
    MDrv_Write2Byte(0x1012a6, 0xffff); //mask other request

    //DQS reset
    MDrv_Write2Byte(0x10110e, MDrv_Read2Byte(0x10110e)|BIT6);
    MDrv_Write2Byte(0x10110e, MDrv_Read2Byte(0x10110e)&~BIT6);
//-------------
//miu self test
//-------------
    MDrv_Write2Byte(0x1012e0, 0x0000);
#if (CHIP_ID == MST9U3)
    MDrv_WriteByteMask(0x101203, 0x00, 0x08);
#else
    MDrv_Write2Byte(0x1012F0, 0x0000);
#endif
    MDrv_Write2Byte(0x1012e2, BIST_BASE_ADDR/BIST_BASE_UNIT); //test base address
    MDrv_Write4Byte(0x1012e4, BIST_SIZE/MIU_UNIT);          //test length
    MDrv_Write2Byte(0x1012e8, 0xff00);                        //test pattern

    MDrv_Write2Byte(0x1012e0, u16ExtraMode);
    MDrv_SysDelayUs(1000);
    MDrv_Write2Byte(0x1012e0, u16ExtraMode|BIT0);

    for (i=0; i<bist_try; i++)
    {
        const U32 BIST_TRY_MAX = 100;
        U32 u32BistTry = 0;

        while(!(MDrv_Read2Byte(0x1012e0)&BIT15) && (u32BistTry++ < BIST_TRY_MAX));

        if (!(MDrv_Read2Byte(0x1012e0)&0x6000) && (MDrv_Read2Byte(0x1012e0)&BIT15))
        {
            bReturn = TRUE;
            break;
        }
        MDrv_Write2Byte(0x1012e0, u16ExtraMode);
        MDrv_SysDelayUs(1000);
        MDrv_Write2Byte(0x1012e0, u16ExtraMode|BIT0);
    }
    if (i >= bist_try)
    {
        bReturn = FALSE;
    }

    MDrv_Write2Byte(0x101246, 0x0000); //unmask other request
    MDrv_Write2Byte(0x101266, 0x0000); //unmask other request
    MDrv_Write2Byte(0x101286, 0x0000); //unmask other request
    MDrv_Write2Byte(0x1012a6, 0x0000); //unmask other request

    if (bReturn == FALSE)
        printf("(bist_try=%d) ", i);

    return bReturn;
}

Bool InitDataSection(void);
Bool InitDataSection(void)
{
#include "ms_decompress_priv.h"
//#define CHUNK_HEADER_ADDR       0x20000
#define CHUNK_HEADER_LEN        0x80
#define CHUNK_HEADER_ADDR       _sboot_end//(0x10000-CHUNK_HEADER_LEN)

    U32 u32ROM_START = 0, u32ROM_END = 0;    //This address should be 4' alignment
    U32 u32RAM_START = 0, u32RAM_END = 0;
    U32 u32CRC32_HW = 0, u32CRC32_SW = 0;
    U32 u32UncompLength = 0;

    u32ROM_START = *((U32 *)(CHUNK_HEADER_ADDR));
    u32RAM_START = *((U32 *)(CHUNK_HEADER_ADDR + 4));
    u32RAM_END = *((U32 *)(CHUNK_HEADER_ADDR + 8));
    u32ROM_END = *((U32 *)(CHUNK_HEADER_ADDR + 12));

    u32CRC32_SW = *((U8 *)(CHUNK_HEADER_ADDR + CRC32_POS+3));
    u32CRC32_SW <<= 8;
    u32CRC32_SW |= *((U8 *)(CHUNK_HEADER_ADDR + CRC32_POS+2));
    u32CRC32_SW <<= 8;
    u32CRC32_SW |= *((U8 *)(CHUNK_HEADER_ADDR + CRC32_POS+1));
    u32CRC32_SW <<= 8;
    u32CRC32_SW |= *((U8 *)(CHUNK_HEADER_ADDR + CRC32_POS+0));

    {
//1 Need to check CRC first, TBD....
#if 1
         ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
         //Calculate CRC32 from flash
        while (RIU8[(BDMA_REG_BASE + 0x02)*2] & 0x01);
        RIU[(BDMA_REG_BASE + 0x08 )] = (U16)((U32)(u32ROM_START&0x3fffff));
        RIU[(BDMA_REG_BASE + 0x0A )] = (U16)(((U32)(u32ROM_START&0x3fffff)) >> 16);
        RIU[(BDMA_REG_BASE + 0x0C )] = 0;
        RIU[(BDMA_REG_BASE + 0x0E )] = 0;
        RIU[(BDMA_REG_BASE + 0x10 )] = (U16)((U32)(u32ROM_END-u32ROM_START));
        RIU[(BDMA_REG_BASE + 0x12 )] = (U16)((U32)((u32ROM_END-u32ROM_START) >> 16));
        //polynomial
        RIU[(BDMA_REG_BASE + 0x14 )] = (U16)(0x1DB7);
        RIU[(BDMA_REG_BASE + 0x16 )] = (U16)(0x04C1);
        //seed
        RIU[(BDMA_REG_BASE + 0x18 )] = 0;
        RIU[(BDMA_REG_BASE + 0x1a )] = 0;

        RIU8[(BDMA_REG_BASE + 0x06)*2] = 0x0;   //increasing
        RIU[(BDMA_REG_BASE+0x04)] = 0x0345; //((DstDEv|DstDataWidth)  <<8) |(SrcDataWidth|SrcDEv) ;

        RIU8[ (BDMA_REG_BASE*2)] = 0x01 ;
        while( 1 )
        {
            if ( (RIU8[(BDMA_REG_BASE+0x02)*2] & 0x08) == 0x08 )
                break;
        }

        //Get CRC32 result
        u32CRC32_HW = RIU[(BDMA_REG_BASE + 0x18 )] ;        //low word
        u32CRC32_HW |= (RIU[(BDMA_REG_BASE + 0x1a )]  <<  16);      //high word

        if ( u32CRC32_HW != u32CRC32_SW )
        {
            return FALSE;
        }
#endif

        u32UncompLength = *((U8 *)(u32ROM_START+UNCOMPRESSED_LENGTH_POS+3));
        u32UncompLength <<= 8;
        u32UncompLength |= *((U8 *)(u32ROM_START+UNCOMPRESSED_LENGTH_POS+2));
        u32UncompLength <<= 8;
        u32UncompLength |= *((U8 *)(u32ROM_START+UNCOMPRESSED_LENGTH_POS+1));
        u32UncompLength <<= 8;
        u32UncompLength |= *((U8 *)(u32ROM_START+UNCOMPRESSED_LENGTH_POS+0));

        //Copy uncompressed part
        while (RIU8[(BDMA_REG_BASE + 0x02)*2] & 0x01);
        RIU[(BDMA_REG_BASE + 0x08 )] = (U16)((U32)(u32ROM_START));
        RIU[(BDMA_REG_BASE + 0x0A )] = (U16)(((U32)(u32ROM_START)) >> 16);
        RIU[(BDMA_REG_BASE + 0x0C )] = (U16)((U32)u32RAM_START);
        RIU[(BDMA_REG_BASE + 0x0E )] = (U16)((U32)(u32RAM_START) >> 16);
        RIU[(BDMA_REG_BASE + 0x10 )] = (U16)(((U32)(u32UncompLength) + 0x0f ) & ~ 0x0ful);
        RIU[(BDMA_REG_BASE + 0x12 )] = (U16)(((((U32)(u32UncompLength)) + 0x0f ) & ~ 0x0ful)>>16);

        RIU8[(BDMA_REG_BASE + 0x06)*2] = 0x00;   //increasing
        RIU[(BDMA_REG_BASE+0x04)] = 0x4045; //((DstDEv|DstDataWidth)  <<8) |(SrcDataWidth|SrcDEv) ;

        RIU8[ (BDMA_REG_BASE*2)] = 0x01 ;
        while( 1 )
        {
            if ( (RIU8[(BDMA_REG_BASE+0x02)*2] & 0x08) == 0x08 )
                break;
        }
    }

    printf("InitDataSection done ..\r\n");

    return TRUE;
}
#endif

void Jump2AP(void);
void Jump2AP(void)
{
#define _DRAM_RESET_VECTOR_BASE_ADDR  0x200000

    U32 u32Addr = _DRAM_RESET_VECTOR_BASE_ADDR;

    SET_SELFREFRESH_BOOT();

    MDrv_Write2Byte(0x100FAC, 0xC000);  //IO2_base(SPI)
    MDrv_Write2Byte(0x100F90, 0xC000);  //SPI_base

    //set up reset vector base
    RIU[0x100FB4] = (U16)(u32Addr >> 16);
    while(RIU[0x100FB4] != (u32Addr>>16)) { } // Wait setting

    //R2 boot from sdram
    MDrv_Write2Byte(0x100F80, 0x002F);

    // using jump instead of reset to run AP
    __asm__ __volatile__ (
        "\tl.syncwritebuffer\n"
        "\tl.jr     %0\n"
        "\tl.syncwritebuffer\n"
        : : "r" (u32Addr+0x100));

    while(1);
}

void SetMIU_SEL(void);
void SetMIU_SEL(void)
{
#define FORCE_MIU0  0x55667788
#define FORCE_MIU1  0xAABBCCDD
#define MIU_128MB   0x8000000
#define MIU_256MB   0x10000000
#define MIU_512MB   0x20000000
#define MIU_1GB     0x40000000
#define MIU_2GB     0x80000000

#define MIU1_BASE   MIU_512MB

    U8 u8MIU_SEL = 0;

    switch ( MIU1_BASE )
    {
        case MIU_256MB:
            u8MIU_SEL = 0;
            break;
        case MIU_512MB:
            u8MIU_SEL = 1;
            break;
        case MIU_1GB:
            u8MIU_SEL = 2;
            break;
        case MIU_2GB:
            u8MIU_SEL = 3;
            break;
        case FORCE_MIU0:
            u8MIU_SEL = 4;
            break;
        case FORCE_MIU1:
            u8MIU_SEL = 5;
            break;
        case MIU_128MB:
            u8MIU_SEL = 6;
            break;
        default:
            u8MIU_SEL = 0;
            break;
    }
    MDrv_WriteByteMask((0x100400 + 0x03 ), 0x80+(u8MIU_SEL<<1), 0xFE );   // Set MIU_SEL mode of HK_R2
    printf("SetMIU_SEL, [0x100403]=0x%x\r\n", MDrv_ReadByte(0x100403));

    MEM32(MIU1_BASE + 0x0) = 0x88998899;
    MEM32(MIU1_BASE + 0x4) = 0xaabbaabb;
    MEM32(MIU1_BASE + 0x8) = 0xccddccdd;
    MEM32(MIU1_BASE + 0xC) = 0xeeffeeff;
}

extern unsigned char __data2_start_s[];
extern unsigned char __data2_end_s[];
extern unsigned char __data2_start_d[];

void Init_Data2(void);
void Init_Data2(void)
{
    U8 *spt = __data2_start_s; // source
    U8 *dpt = __data2_start_d; // dest

    while(spt<__data2_end_s)
    {
        *dpt++ = *spt++;
    }
}

void start(void);
void start(void)
{
    //(1) disable watch dog
    MDrv_Write2Byte(0x2C08/*0x3008*/, 0x0000);
    MDrv_Write2Byte(0x2C0A/*0x300A*/, 0x0000);


    //(2) boot initialization
    Init_Data2(); // init .data2 which are placed in DQMEM but initial data in flash
    BL_Init();
    printf("\n\r BL_Init OK \n\r");

    //(3)Pad Init
    MDrv_Pad_Init();
    printf("\n\r MDrv_Pad_Init OK \n\r");

    //extra test..
    if (!IS_WAKEUP_FROM_PM)
    {
        SetSPI_Mode();
    }

    printf("\r\n ***PM Status  : [0x%x]=0x%x", 0x03AA, MDrv_Read2Byte(0x03AA));
    printf("\r\n ***MBX Status : [0x%x]=0x%x\r\n\r\n", 0x103380, MDrv_Read2Byte(0x103380));

#if ENABLE_USB
#if ENABLE_USB_HOST_DUAL_IMAGE
    usb_dual_image_setting();
#else
    if(usb_dl_condition())
    {
        usb_check(FALSE);
    }
#endif
#endif

#if (ENABLE_DRAM_SELFREFRESH)
    if (IS_WAKEUP_FROM_PM)
    {
        if (TestSelfRefreshGuard())
        {
            //ReInit Data section only, instead of decompressing whole AP image.
            if (InitDataSection())
                Jump2AP();
            else
                printf("InitDataSection failed, ");
        }
        else
        {
            printf("TestSelfRefreshGuard failed, ");
        }
        printf("2nd boot-loader launches..\r\n");
    }
#endif

    //(4) copy 2nd loader here
    CopyFLH2MIU( (U32)loader_2nd_vma, (U32)loader_2nd_lma, (U32)loader_2nd_len );
    printf("\n\r CopyFLH2MIU \n\r");

    MDrv_Write2Byte(0x100FAC, 0xC000);  //IO2_base(SPI)
    MDrv_Write2Byte(0x100F90, 0xC000);  //SPI_base

    //(5) load and execute application
    BootFromDRAM((U32)loadAP);

    while(1);

}

#define __loader_start __attribute__ ((__section__ (".loader_2nd_start.text")))
#define __loader_2nd __attribute__ ((__section__ (".loader_2nd.text")))
extern void __loader_2nd DoMSDecompression(void);
//########################################
//#
//# Especially use  __loader_start specified section to
//# let it run at address 64Kx N+0x100
//#   Ex:  0x1000000+0x100 (see sboot.lds.S) => N=0x100
//########################################
void __loader_start loadAP(void)
{
    // Enable I-Cache to speed up the load code process
    __asm__ __volatile__ (
        /* Invalidate I-Cache */
        "\tl.addi  r13,r0,0\n"
        "1:\n"
        "\tl.mtspr %2,r13,0\n"
        "\tl.sfne  r13,%3\n"
        #ifdef __AEONR2__
        "\tl.addi  r13,r13,%4\n"
        "\tl.bf    1b\n"
        #else
        "\tl.bf    1b\n"
        "\tl.addi  r13,r13,%4\n"
        #endif

        /* Enable I-Cache */
        "\tl.mfspr r13,%0,0\n"
        "\tl.ori   r13,r13,%1\n"
        "\tl.mtspr %0,r13,0\n"

        /* Flush instructions out of instruction buffer */
        "\tl.nop\n"
        "\tl.nop\n"
        "\tl.nop\n"
        "\tl.nop\n"
        "\tl.nop\n"
        : // read out from in-line asm
        : //write to in-line asm
          "r" (SPR_SR), "i" (SPR_SR_ICE), "r" (SPR_ICBIR),
          "r" (8192), "i" (32)//  "r" (8192*2), "i" (16)
        : "r13"); // clober define - before enter in-line assembly, stack this reg

    // Enable D-Cache to speed up decompress
    __asm__ __volatile__ (
        /* Invalidate DC    */
        "\tl.addi  r13,r0,0\n"
        "1:\n"

        "\tl.invalidate_line  0(r13), 0\n"
        "\tl.invalidate_line  0(r13), 1\n"
        "\tl.invalidate_line  0(r13), 2\n"
        "\tl.invalidate_line  0(r13), 3\n"

        "\tl.sfne  r13,%2\n"
        #ifdef __AEONR2__
        "\tl.addi  r13,r13,%3\n"
        "\tl.bf    1b\n"
        #else
        "\tl.bf    1b\n"
        "\tl.addi  r13,r13,%3\n"
        #endif

        /* Enable D-Cache */
        "\tl.mfspr r13,%0,0\n"
        "\tl.ori   r13,r13,%1\n"
        "\tl.mtspr %0,r13,0\n"
        :
        : "r" (SPR_SR), "i" (SPR_SR_DCE),
          "r" ((MHAL_DCACHE_SIZE/MHAL_DCACHE_WAYS) - MHAL_DCACHE_LINE_SIZE),
          "i" (MHAL_DCACHE_LINE_SIZE)
        : "r13");

    //start to do decompress task
    DoMSDecompression();

    #ifdef __AEONR2__
    while(1);
    #else
    __asm__ __volatile__ (
        "\tl.syncwritebuffer\n"
        "\tl.jr     %0\n"
        "\tl.syncwritebuffer\n"
        : : "r" (0x100));
    #endif
}


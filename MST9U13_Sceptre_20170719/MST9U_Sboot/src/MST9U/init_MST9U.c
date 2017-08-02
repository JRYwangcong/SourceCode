////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2008 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// ( ß¡±MStar Confidential Information ßÆØ) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include "datatype.h"
#include "drvGlobal.h"
#include "drvAeonUART.h"
#include "risc32_spr.h"
#include "hwreg.h"
#include "Board.h"
#include "drv_miu_init.h"
#include "risc32.h"
///////////////////////////////////////////////////////////////////////////////
// power up and turn on clock
///////////////////////////////////////////////////////////////////////////////
#include "hwreg_MST9U.h"
#include "ddr_bd.h"

#if ENABLE_PRINT_MSG
#warning "Please remember to disable the <ENABLE_PRINT_MSG>!"
#endif

///////////////////////////////////////////////////////////////////////////////
// power up and turn on clock
///////////////////////////////////////////////////////////////////////////////
#define  MDrv_MaskAllMiuRequest()   do{\
                              } while(0)

#define  MDrv_UnMaskAllMiuRequest()   do{\
                              } while(0)


#define MDrv_OpenAllMiumask()  do{\
                                MDrv_Write2Byte((RIUBASE_MIU  + (0x0023*2)),  0x0000);   /* de_mask all miu_0/miu_1 request & reset */\
                                MDrv_Write2Byte((RIUBASE_MIU  + (0x0033*2)),  0x0000);   /* de_mask all miu_0/miu_1 request & reset */\
                                MDrv_Write2Byte((RIUBASE_MIU  + (0x0043*2)),  0x0000);   /* de_mask all miu_0/miu_1 request & reset */\
                                MDrv_Write2Byte((RIUBASE_MIU  + (0x0053*2)),  0x0000);   /* de_mask all miu_0/miu_1 request & reset */\
                              } while(0)

// Auto DQS phase detection
#define ENABLE_AUTO_DQS             1
#define AUTO_DQS_START_ADDRESS      0x1FFF000
#define SET_AUTO_DQS_START_ADDR     (AUTO_DQS_START_ADDRESS/BIST_BASE_UNIT)  //8kbyte/unit in register setting
#define AUTO_DQS_VERIFY_SIZE        0x80000
#define SET_AUTO_DQS_VERIFY_SIZE    (AUTO_DQS_VERIFY_SIZE/MIU_UNIT)      //16 byte/unit in register setting

// Auto DQS phase detection
#define AUTO_DQS_DUMP(x) x

U8 hex[] = "0123456789ABCDEF";
//const U8 g_MemTestMode[] = { 0x07, 0x09, 0x05, 0x03, 0x0B };
const U8 g_MemTestMode[] = { 0x83,0x87, 0x85};  //, 0x05, 0x03, 0x0B };
const U8 g_MemTestMask[] = { (U8)~33, (U8)~0xCC };
U8 g_DoMiuInit = 1;
#define DQSN(step)  (1 - step / 16) // (step / 16)

extern U16 risc32_regbase[];

unsigned char *loader_2nd_lma;
unsigned char *loader_2nd_vma;
unsigned char *loader_2nd_len;

#if (DRAM_BUS_WIDTH == DRAM_BUS_16B)
#define DRAM_BUS_MASK           0x00
#else
#define DRAM_BUS_MASK           0x04
#endif

extern void loadAP(void);
extern void CopyFLH2MIU(U32 dst, U32 src, U32 size);

void MDrv_WriteRegTbl32( const U16 * pRegTable );
//================================================
void MDrv_WriteRegTbl32( const U16 * pRegTable )
{
   U16 u16Index; // register index
   U16 u16Dummy;

   u16Dummy = 2000;

   u16Index=0;
   while( u16Dummy )
   {
       if ( pRegTable[u16Index] == 0xffff )
          break;
       RIU[ (0x100000ul+pRegTable[u16Index]) ]=pRegTable [u16Index+1];
       u16Index+=2;
   }
}

#if (!ENABLE_MSTAR_MST9U)
static const MS_REG_INIT TurnOnAllClk[] =
{
    //adcpll_top
    _RV32_1(0x10250E,  0x1F),    //soft-reset atop/pllb/adcb/adca/plla
    _RV32_1(0x10250E,  0x00),
    _RV32_1(0x102508,  0x00),    //power on adc_atop
    _RV32_1(0x102509,  0x00),
    _RV32_1(0x10250A,  0x00),
    _RV32_1(0x10250B,  0x00),
    _RV32_1(0x10250C,  0x00),
    _RV32_1(0x10250D,  0x00),
    _RV32_1(0x1025BC,  0x00),
    _RV32_1(0x1025BD,  0x00),
    _RV32_1(0x102590,  0x08),    //Enable the IDAC(Ch1), Control the output voltage range of IDAC(Ch1): 2'b00 (1.3V)


    //dvi_atop
    _RV32_1(0x11090C,  0x00),    //power on xtal/plla/adca/pllb/adb.....
    _RV32_1(0x11090D,  0x00),
    _RV32_1(0x1109C0,  0x00),    //power on DVI PLL,bias ....
    _RV32_1(0x1109C1,  0x00),
    _RV32_1(0x1109D2,  0x00),    //power on DVI_2 PLL,bias ....
    _RV32_1(0x1109D3,  0x00),
    _RV32_1(0x110906,  0x00),    //dummy?
    _RV32_1(0x110907,  0x00),    //dummy?
    _RV32_1(0x11320C,  0x00),    //dummy?
    _RV32_1(0x11320D,  0x00),    //dummy?
    _RV32_1(0x11340C,  0x00),    //dummy?
    _RV32_1(0x11340D,  0x00),    //dummy?
    _RV32_1(0x1132C0,  0x00),    //dummy?
    _RV32_1(0x1132C1,  0x00),    //dummy?
    _RV32_1(0x1134C0,  0x00),    //dummy?
    _RV32_1(0x1134C1,  0x00),    //dummy?


    //clk_dvi in pm_sleep
    _RV32_1(0x000E94,  0x00),
    _RV32_1(0x000E95,  0x00),
    _RV32_1(0x000E96,  0x00),
    _RV32_1(0x000E97,  0x00),
    _END_OF_TBL32_

};

//---------------------------------------------------------------------

static const MS_REG_INIT InitialSystemClk[] =    //CLK_GEN_0
{
    _RV32_1(0x100b00, 0x10),  // turn xtal clk  #4 = 1 (CKg_RIU must be "1" in VIF input)
    _RV32_1(0x100b02, 0x07),  // SRAM bist test clk 7:xtal, 6:54M, 5:62M 4:72M 3:86M 2:108M 1:123M 0:144M
    // Dig_top_pm mcu clock
    _RV32_1(0x000f40, 0x00),      // step 1/2:set hk51 to 216M
    _RV32_1(0x000f40, 0x80),      // step 2/2:set hk51 to 216M
    // dig_top
    _RV32_1(0x100b21, 0x00),      //turn ckg_mcu for turn riu_bridge,db_bus....,db_bus....
    _RV32_1(0x100b20, 0x01),      //change to fast clock
    // clk_aeon
    _RV32_1(0x100b24, 0x00),      // aeon clk aeon to 216Mhz
    _RV32_1(0x100b24, 0x80),      // change to fast clock
    // clk_R2
    _RV32_1(0x100b25, 0x08),      // aeon clk r2 to 216Mhz
    _RV32_1(0x100b25, 0x28),      // change to fast clock
    // clk_UART
    _RV32_1(0x100b27, 0x0C),      // uart0 clk to 123Mhz
    _RV32_1(0x100b28, 0x0C),      // uart1 clk to 123Mhz
    //_RV32_1(0x100b29, 0x0C),      // uart2 clk to 123Mhz
    // support spi fast read
    _RV32_1(0x0008e2, 0xFF),
    _RV32_1(0x0008e3, 0x0F),
    _RV32_1(0x000E6A, 0x30),      // max SPI pad driving
//    _RV32_1(0x0008e4, 0x01),      //Enable fast read mode, (SPI command is 0x0B)
    // clk_spi
    // Dig_top_pm clk_spi
    _RV32_1(0x000F41, 0x14),      // clk spi to 72Mhz
    _RV32_1(0x000F41, 0x54),      // change to fast clock
    // dig_top clk_spi
    _RV32_1(0x100b2c, 0x14),      // clk spi to 72Mhz
    _RV32_1(0x100b2c, 0x34),      // change to fast clock
    // clk_pcm
    _RV32_1(0x100b34, 0x00),      // set pcm clk to 27Mhz,  pcmcia
    // clk_jtag
    _RV32_1(0x100b35, 0x00),      // turn tck clk on
    // MIU relative
    _RV32_1(0x100b3d, 0x00),      // MPLL syn = 216MHz ???
    _RV32_1(0x100b3e, 0x00),      // CLK_MIU = MEMPLL_out (200MHz)
    _RV32_1(0x100bfc, 0x01),      // reg_clkgen0_reserved0 ??
    // clk_vd
    _RV32_1(0x100b40, 0x00),      //clk vd_sel, 0: vd_adc_clk(Y), 1: adc_clk(RGB)
    _RV32_1(0x100b41, 0x00),      //ckg_vd= vd_sel
    // clk_vd mcu
    _RV32_1(0x100b42, 0x10),      //ckg_vdmcu = 108Mhz , need fine tune in demod
    _RV32_1(0x100b43, 0x04),      //clk vd_200=216Mhz
    // clk_mail
    _RV32_1(0x100b44, 0x0c),      //clk mail box0 =xtal  <<hk51 <--mail box 0--> aeon
    _RV32_1(0x100b45, 0x0c),      //clk mail box1 =xtal  <<hk51 <--mail box 1--> aeon
    _RV32_1(0x100b46, 0x00),      //tur on vd2x
    _RV32_1(0x100b47, 0x00),      //tur on vd_32fsc
    // clk_ve
    _RV32_1(0x100b48, 0x00),      //ckg_Ve=27M, remove
    _RV32_1(0x100b49, 0x00),      //ckg_Vedec=27M, remove
    _RV32_1(0x100b4a, 0x0c),      //ckg_Ve_in=clk_DC0, remove
    _RV32_1(0x100b4c, 0x04),      //clk_daca2=VD_clk
    // clk_dscb2
    _RV32_1(0x100b4d, 0x04),      //clk_dscb2=VD_CLK
    // clk_tsp
    _RV32_1(0x100b50, 0x08),      //clk_ts0=demode, CLK_TS1= gating
    _RV32_1(0x100b54, 0x00),      //CLK_TSP = 144M
    _RV32_1(0x100b55, 0x00),      //Ckg_stc0 = stc0_sync_output
    _RV32_1(0x100b60, 0x00),      //ckg_vd_mheg5 =160Mhz
    _RV32_1(0x100b62, 0x08),      //ckg_HVD =144Mhz
    _RV32_1(0x100b6a, 0x0c),      //ckg_jpg=160M  0:72m 4:108m c:123m c:160m
    // clk_mvdxx
    _RV32_1(0x100b72, 0x00),      //mvd_clk=144m ON
    _RV32_1(0x100b73, 0x00),      //mvd2_clk=172m ON
    _RV32_1(0x100b74, 0x00),      //mvd_chroma ON
    _RV32_1(0x100b75, 0x00),      //mvd_luma_a ON
    _RV32_1(0x100b76, 0x00),      //mvd_luma_b ON
    _RV32_1(0x100b77, 0x00),      //mvd_luma_c ON
    _RV32_1(0x100b78, 0x00),      //mvd_rmem ON
    _RV32_1(0x100b79, 0x00),      //mvd_rmem1 ON
    // clk_gop and ge
    _RV32_1(0x100b80, 0x00),      //clk_gop_0 ==odclk_p
    _RV32_1(0x100b81, 0x00),      //clk_gop_1 ==odclk_p
    _RV32_1(0x100b83, 0x01),      //gating first, ((clk_gopd (DWIN) = CLK_ODCLK))
    _RV32_1(0x100b86, 0x00),      //psram0=on, select gop0
    _RV32_1(0x100b87, 0x00),      //psram1=on, select gop0
    _RV32_1(0x100b90, 0x0c),      //ckg_ge=144M
    // clk_dc
    _RV32_1(0x100b98, 0x00),      //ckg_dc0=synchronous mode      #TBD: what is synchronous mode?
    _RV32_1(0x100b99, 0x00),      //ckg_dc1=synchronous mode
    // SC
    _RV32_1(0x100ba3, 0x04),      //main_window fclk_f2, and select fclk
    _RV32_1(0x100ba4, 0x04),      //main_window ficlk2_f2, and select fclk
    _RV32_1(0x100ba5, 0x00),      //flck=170M
    _RV32_1(0x100ba6, 0x1c),      //odclk (output pixel clock) = LPLL
    _RV32_1(0x100ba7, 0x1c),      //bt656 = LPLL
    // clk_mod
    _RV32_1(0x103200, 0x00),      //switch bank
    _RV32_1(0x103241, 0x00),      //dot mmini_pre clk= clk_lpll_buf
    _RV32_1(0x100bae, 0x00),      //fifo mmini = LPLL   //LVDS   fifo_clk=mini_lvds         #TBD: not found (the same as T3, by jeff)
    // clk_vif
    _RV32_1(0x103314, 0x00),      // reg_ckg_dvbtc_adc@0x0a[3:0] : ADC_CLK
    _RV32_1(0x103315, 0x01),      // reg_ckg_dvbtc_innc@0x0a[11:8]
    _RV32_1(0x10330a, 0x00),      // reg_ckg_atsc_adcd_sync@0x05[3:0] : ADCCLK
    _RV32_1(0x10330b, 0x00),
    _RV32_1(0x10331a, 0x04),      // reg_ckg_vifdbb_dac@0x0d[3:0] : MPLLDIV10/2=43MHz
    _RV32_1(0x10331b, 0x04),      // reg_ckg_vifdbb_vdac@0x0d[11:8]: MPLLDIV10/2=43MHz
    _RV32_1(0x10331c, 0x00),      // reg_ckg_vifdbb_43m@0x0e[3:0] : MPLLDIV10/2=43MHz
    _RV32_1(0x10331d, 0x00),      // reg_ckg_vifdbb_144m@0x0e[11:8]: ADC_CLKOUT=144MHz
    // ckg_idclk_lpll
    _RV32_1(0x103364, 0x04),      // reg_ckg_idclk_lpll and select clk_idclk2

    _END_OF_TBL32_
};

static void MDrv_USBPLL_Init_M12( void )
{
    // turn 480Mhz in USB
    // USBC0 <--> UTMI0
    MDrv_WriteByte(0x100700, 0x0a);    // Disable MAC initial suspend, Reset UHC
    MDrv_WriteByte(0x100700, 0x28);    // Release UHC reset, enable UHC and OTG XIU function

    MDrv_WriteByte(0x103aa2, 0xE0);    // Set PLL_TEST[23:22] for enable 480MHz clock
    MDrv_WriteByte(0x103aa0, 0x03);    // UTMI offset 0x20 bit1=1//Set PLL_TEST[1] for PLL multiplier 20X
    #if (RTL_SIMULATION == 1)
    MDrv_SysDelayUs(1);
    #else
    MDrv_SysDelayUs(1000);             // delay 1ms
    #endif
    MDrv_WriteByte(0x103aa1, 0x80);    // UTMI offset 0x20 bit1=1//Set PLL_TEST[1] for PLL multiplier 20X
    #if (RTL_SIMULATION == 1)
    MDrv_SysDelayUs(1);
    #else
    MDrv_SysDelayUs(1000);             // delay 1ms
    #endif
    MDrv_WriteByte(0x103a82, 0x84);    // Enable CLK12_SEL bit <2> for select low voltage crystal clock
    MDrv_WriteByte(0x103a83, 0x20);    // Enable CLK12_SEL bit <2> for select low voltage crystal clock
    MDrv_WriteByte(0x103a89, 0x00);    // #3=0 Disable force_pll_on
    MDrv_WriteByte(0x103a88, 0x00);    // #7=0 Enable band-gap current
    MDrv_WriteByte(0x103a81, 0x6b);    // Turn on reference voltage and regulator
    MDrv_WriteByte(0x103a80, 0xc3);    // reg_pdn: bit<15>, bit <2> ref_pdn
    MDrv_WriteByte(0x103a81, 0x69);    // Turn on UPLL, reg_pdn: bit<9>
    MDrv_WriteByte(0x103a80, 0x01);    // Turn all (including hs_current) use override mode
    MDrv_WriteByte(0x103a81, 0x00);
    MDrv_WriteByte(0x103a83, 0x00);    //#7=0
}

static void MDrv_SysInitMPLL_M12( void )
{
    // DMOD ATOP XTAL Setting
    MDrv_WriteByte(0x101E38, 0x00); //turn on bit0=0 : mpll power on, bit1 =0: MIPSPLL and bit 2: USB PLL
    MDrv_WriteByte(0x101E39, 0x00);
    MDrv_WriteByte(0x101ED2, 0x00); //Enable MPLL input reference clock

    MDrv_WriteByte(0x112003, 0x00);
    MDrv_WriteByte(0x11286B, 0x24);
    MDrv_WriteByte(0x11286D, 0x00);

    // MPLL Setting
    MDrv_WriteByte(0x110C02, 0x00); //[0]: mpll_in_select=0, selec 1.2v clock in
    MDrv_WriteByte(0x110C03, 0x00); //[8]: mpll_pd = 0,[9]: adc216_pd = 0,[10]: adc413_pd = 0,

    // DSP PLL Setting
    MDrv_WriteByte(0x110c40, 0x00); //MPLL_TEST       0x110C40[15:0]  = 16'd0 (default
    MDrv_WriteByte(0x110c41, 0x00);
    MDrv_WriteByte(0x110c42, 0x01); //MPLL_CLK_ADC432M_PD 0x110C42[10]   = 1'b1  (Power Down unused outp
    MDrv_WriteByte(0x110c43, 0x06); //MPLL_CLK_ADC216M_PD 0x110C42[9]    = 1'b1  (Power Down unused output)
                                    //MPLL_PD         0x110C42[8]    = 1'd1  (default, DFT Gated)
                                    //MPLL_IN_SELECT  0x110C42[0]    = 1'b1  (default, Ref. clock from MPLL_CLK_IN_3
    MDrv_WriteByte(0x110c44, 0x01); //MPLL_CLK_ADC432M_PD   0x110C42[10]     = 1'b1  (Power Down unused outp
    MDrv_WriteByte(0x110c45, 0x04); //MPLL_INPUT_DIV_FIRST  0x110C44[5:4]   = 2'd0  (default)
                                    //MPLL_CLK33_IN_DIV2_EN 0x110C44[0]   = 1'b1  (default)
    MDrv_WriteByte(0x110c46, 0x2C); //MPLL_OUTPUT_DIV_FIRST 0x110C46[13:12] = 2'd0  (default)
    MDrv_WriteByte(0x110c47, 0x00); //MPLL_VCO_DIV2_DISABLE 0x110C46[8]  = 1'b0  (default)
                                    //MPLL_LOOP_DIV_SECOND  0x110C46[7:0]   = 8'h36 (default)
    MDrv_WriteByte(0x110c48, 0xa0); //MPLL_ENDISC       0x110C48[9] = 1'd0  (default)
    MDrv_WriteByte(0x110c49, 0x00); //MPLL_ENFRUN       0x110C48[8] = 1'd0  (default)
                                    //MPLL_IBIAS_ICTRL  0x110C48[7:6]   = 2'd0  (default)
                                    //MPLL_ICP_ICTRL    0x110C48[5:4]   = 2'd2  (default)
                                    //MPLL_TEST_EN      0x110C48[0] = 1'd0  (default)
}

static int InitialSystemPowerUp_M12( void )
{
    int i;
#if FPGA_VERIFY
    return 0;
  #endif

    MDrv_WriteRegTbl_32( TurnOnAllClk );
    MDrv_SysInitMPLL_M12();

  #if (RTL_SIMULATION == 1)
    MDrv_Write2Byte(0x103380, 0x0001);
  #endif

    MDrv_USBPLL_Init_M12();

  #if (RTL_SIMULATION == 1)
    MDrv_Write2Byte(0x103380, 0x0002);
  #endif
    MDrv_WriteByte(0x0008E4, 0x01);//Enable fast read mode, (SPI command is 0x0B)

    for (i = 0; i < 500; i++)
       __asm__ __volatile__("l.nop");

    // Need a branch for fast mode to take effect,
    // use inline assembly to avoid eliminated by compiler or linker
    __asm__ __volatile__ (
        "\tl.j      1f\n"
        "\tl.nop    0\n"
        "\tl.nop    0\n"
        "1:");


    MDrv_WriteRegTbl_32( InitialSystemClk );

  #if (RTL_SIMULATION == 1)
    MDrv_Write2Byte(0x103380, 0x0003);
  #endif

    return 1;
}

static void MDrv_WriteMiuAtopTbl( const MS_REG_INIT * pRegTable )
{
    U32 u32Index; // register index
    while( 1 )
    {
        u32Index = (((U32)pRegTable[0] << 16) + ((U32)pRegTable[1] << 8) + pRegTable[2] );
        if (u32Index == 0xFFFFFF) // check end of table
            break;
        u32Index &= 0x1FFFFF;
        MDrv_Write2Byte( u32Index         , ( pRegTable[3] + (pRegTable[4] << 8))  );   //miu0_atop
        pRegTable += 5;
    }
}

static void MDrv_WriteMiuTbl( const MS_REG_INIT * pRegTable )
{
    U32 u32Index; // register index
    while( 1 )
    {
        u32Index = (((U32)pRegTable[0] << 16) + ((U32)pRegTable[1] << 8) + pRegTable[2] );
        if (u32Index == 0xFFFFFF) // check end of table
            break;
        u32Index &= 0x1FFFFF;
        MDrv_Write2Byte(u32Index           , ( pRegTable[3] + (pRegTable[4] << 8))  ); //miu0
        pRegTable += 5;
    }
}

static void  MDrv_AutoInitMiuCycle(void)
{
    MDrv_Write2Byte( RIUBASE_MIU, 0x0000);  /*reg_0x1200= 0x0000,*/
    MDrv_SysDelayUs(200);                   //delay 200us
    MDrv_Write2Byte( RIUBASE_MIU, 0x0008);  //reg_0x1200= 0x0008, DRAM reset
    MDrv_SysDelayUs(500);                   //delay 500us
    MDrv_Write2Byte( RIUBASE_MIU, 0x000c);  //reg_0x1200= 0x000c, DRAmreset+DRAM chip select
    MDrv_Write2Byte( RIUBASE_MIU, 0x000e);  //reg_0x1200= 0x000E,DRAmreset+DRAM chip select+enable CKE + ODT ON
    MDrv_SysDelayUs(1);                     //delay 1us
  #if (MIU_SPEED == DDR2_800MHz)//MCP : Internal DDR2
    MDrv_Write2Byte((RIUBASE_MIU ), 0x000f);//reg_0x1200= 0x000F,DRAmreset+DRAM chip select+enable CKE +auto initial dram cycle
  #elif(MIU_SPEED == DDR2_1066MHz)//MCP : Internal DDR2
    MDrv_Write2Byte((RIUBASE_MIU ), 0x001f);//reg_0x1200= 0x001F,DRAmreset+DRAM chip select+enable CKE +auto initial dram cycle + ODT ON
  #else //external DDR3/DDR2
    MDrv_Write2Byte((RIUBASE_MIU ), 0x001f);//reg_0x1200= 0x001F,DRAmreset+DRAM chip select+enable CKE +auto initial dram cycle + ODT ON
  #endif

  #if (RTL_SIMULATION == 1)
    MDrv_SysDelayUs(1);                     //delay 1us
  #else
    MDrv_SysDelayUs(10*1000);               // delay 10ms : wait initial done
  #endif

}

static U8 MDrv_MiuAutoDQSPhase_M12( U8 u8Miu )
{
#if ENABLE_AUTO_DQS
    U8 u8Step;
    U8 u8Mode;
    U8 u8DqsSum;
    U8 u8DqsCnt,u8TotalGoodPhase;
    U8 u8DqsPhase;
    U8 u8DefDqsPhase;
    U8 bUseDefDqsPh;
    U32 u32DqsPhAddr = 0;
    U32 u32PhaseResult = 0;
    u32DqsPhAddr = 0x110D6E;
    u8Miu=u8Miu;
    //set test area here
    MDrv_Write2Byte( (RIUBASE_MIU +(0x0071 *2)) , SET_AUTO_DQS_START_ADDR );  //0x3FFF );     //(4kbytes)/unit
    MDrv_Write4Byte( (RIUBASE_MIU +(0x0072 *2)) , SET_AUTO_DQS_VERIFY_SIZE ); //0x8000 );     //(16 byte)/unit
    MDrv_Write2Byte( (RIUBASE_MIU + (0x0074 *2)), 0x5aa5);   //test data

    u8DqsSum = 0;
    u8DqsCnt = 0;
    u8DqsPhase = 0;
    bUseDefDqsPh = 0;
    u8TotalGoodPhase=0;

    u8DefDqsPhase = MDrv_ReadByte( u32DqsPhAddr );
    printf("\n\r%02X\n",MDrv_ReadByte(u32DqsPhAddr));

    for ( u8Step = 0; u8Step < 32; u8Step++ )
    {
       // MDrv_WriteByte( RIUBASE_MIU+0xe7, g_MemTestMask[DQSN(u8Step)] );

        AUTO_DQS_DUMP( if (!(u8Step & 0x0F)) uart_putc( '[' ) );

        MDrv_WriteByte( u32DqsPhAddr, DQSN(u8Step) ? ((u8Step & 0x0F) << 4 | 0x07) : ((u8Step & 0x0F) | 0x70));

        for ( u8Mode = 0 ; u8Mode < sizeof(g_MemTestMode) ; ++u8Mode )
        {
            // enable test, order (high byte then low byte) is important
            MDrv_WriteByte( (RIUBASE_MIU +(0x0070 *2)+1), 0x00 );
            MDrv_WriteByte( (RIUBASE_MIU +(0x0070 *2)), g_MemTestMode[u8Mode] );  // pattern from data
            //MDrv_Write2Byte( (RIUBASE_MIU +(0x0070 *2)), (U16)(g_MemTestMode[u8Mode]) );  // pattern from data
            while ( !MDrv_ReadRegBit( (RIUBASE_MIU +(0x0070 *2)+1), BIT7 ) ) ;

            if ( MDrv_ReadRegBit( (RIUBASE_MIU +(0x0070 *2)+1), BIT6 ) )
            {
                break;
            }
            MDrv_WriteByte( (RIUBASE_MIU +(0x0070 *2)), 0x00 );
        }
        MDrv_WriteByte( (RIUBASE_MIU +(0x0070 *2)), 0x00 );

        if ( u8Mode ==  sizeof( g_MemTestMode ) )
        {
            AUTO_DQS_DUMP( uart_putc( (u8Step % 16) < 10 ? ('0' + (u8Step % 16)) : ('A' + (u8Step % 16) - 10)) );
            u8DqsSum += (u8Step & 0x0F);
            ++u8DqsCnt;
            u32PhaseResult |= (1 << u8Step);
        }

        if ( (u8Step % 16 == 15) )
        {
            if (u8DqsCnt == 0)
            {
                bUseDefDqsPh = 1;
            }
            else
            {
                u8DqsPhase |= ((u8DqsSum / u8DqsCnt) << (DQSN( u8Step ) * 4));
                u8DqsSum = 0;
                u8TotalGoodPhase+=u8DqsCnt;
                u8DqsCnt = 0;
            }
            AUTO_DQS_DUMP( uart_putc( ']' ) );
        }
    }
    MDrv_WriteByte( u32DqsPhAddr, bUseDefDqsPh ? u8DefDqsPhase : u8DqsPhase );
    MDrv_Write4Byte(0x103390, u32PhaseResult);
    //clear miu1 result
    MDrv_Write4Byte(0x103394, 0);

    AUTO_DQS_DUMP( uart_putc( ' ' );
                   printf( "\n\r%04X\n", MDrv_ReadByte(u32DqsPhAddr) );
                   uart_putc( '\n' ) );
    return( u8TotalGoodPhase );
#else
    u8Miu=u8Miu;
    return 0;
#endif
}

static void MDrv_MiuReset( void )
{
    MDrv_Write2Byte( 0x10121e, 0x0c01 );
    MDrv_Write2Byte( 0x10121e, 0x0c00 );
  #if (RTL_SIMULATION == 1)
    MDrv_SysDelayUs(1);
  #else
    MDrv_SysDelayUs(1000);  // delay 1ms
  #endif

    //PKG type select ( 0:DDR3 ; 1:DDR2_MCP ; 2:DDR2(External) )
  #if (MIU_SPEED == DDR2_800MHz) //DDR2_MCP
    MDrv_Write2Byte( 0x110d7e, 0x0088 );
    MDrv_Write2Byte( 0x110d00, 0x0001 );
    MDrv_Write2Byte( 0x110d02, 0xaaaa );
    MDrv_Write2Byte( 0x110d08, 0x00ff );
    MDrv_Write2Byte( 0x110d74, 0x0333 ); //trig lvl
    MDrv_Write2Byte( 0x110d5c, 0x3333 ); //drvn
    MDrv_Write2Byte( 0x110d5e, 0x3333 ); //drvp
  #elif (MIU_SPEED == DDR2_1066MHz) //DDR2_MCP
    MDrv_Write2Byte( 0x110d7e, 0x0088 );
    MDrv_Write2Byte( 0x110d00, 0x0001 );
    MDrv_Write2Byte( 0x110d02, 0xaaaa );
    MDrv_Write2Byte( 0x110d08, 0x00ff );
    if(MDrv_ReadByte(0x001e03)==(U8)0x00)
    {
        MDrv_Write2Byte( 0x110d74, 0x0333 ); //trig lvl
        MDrv_Write2Byte( 0x110d5c, 0x3333 ); //drvn
        MDrv_Write2Byte( 0x110d5e, 0x3333 ); //drvp
    }
    else
    {
        MDrv_Write2Byte( 0x110d74, 0x3300 ); //trig lvl
        MDrv_Write2Byte( 0x110d5c, 0x1111 ); //drvn
        MDrv_Write2Byte( 0x110d5e, 0x1111 ); //drvp
    }
  #elif (MIU_SPEED == DDR2_800MHz_e)//DDR2(External)
    MDrv_Write2Byte( 0x110d7e, 0x0088 );
    MDrv_Write2Byte( 0x110d00, 0x0402 );
    MDrv_Write2Byte( 0x110d24, 0x0400 );
    MDrv_Write2Byte( 0x110d02, 0xaaaa );
    MDrv_Write2Byte( 0x110d08, 0x00ff );
    MDrv_Write2Byte( 0x110d74, 0x0333 ); //trig lvl
    MDrv_Write2Byte( 0x110d5a, 0x0002 ); //drv offset
    MDrv_Write2Byte( 0x110d5c, 0x7033 ); //drvn
    MDrv_Write2Byte( 0x110d5e, 0x7033 ); //drvp
  #elif (MIU_SPEED == DDR2_1066MHz_e)//DDR2(External)
    MDrv_Write2Byte( 0x110d7e, 0x0088 );
    MDrv_Write2Byte( 0x110d00, 0x0402 );
    MDrv_Write2Byte( 0x110d24, 0x0400 );
    MDrv_Write2Byte( 0x110d02, 0xaaaa );
    MDrv_Write2Byte( 0x110d08, 0x00ff );
    MDrv_Write2Byte( 0x110d74, 0x0333 ); //trig lvl
    MDrv_Write2Byte( 0x110d5c, 0x7033 ); //drvn
    MDrv_Write2Byte( 0x110d5e, 0x7033 ); //drvp
  #elif (MIU_SPEED == DDR3_1600MHz_e)//DDR3
    MDrv_Write2Byte( 0x110d7e, 0x0000 );
    MDrv_Write2Byte( 0x110d00, 0x0c00 );
    MDrv_Write2Byte( 0x110d02, 0xaaaa );
    MDrv_Write2Byte( 0x110d08, 0x02ff );
    if(MDrv_ReadByte(0x001e03)==(U8)0x00)
    {
        MDrv_Write2Byte( 0x110d5a, 0x0000 ); //drv
        MDrv_Write2Byte( 0x110d5c, 0x3333 ); //drv
        MDrv_Write2Byte( 0x110d5e, 0x3333 ); //drv
        MDrv_Write2Byte( 0x110d10, 0x0200 ); //reg_sel_dqs_diff
        MDrv_Write2Byte( 0x110d74, 0x0333 ); //trig level
    }
    else
    {
        MDrv_Write2Byte( 0x110d5a, 0x0003 ); //drv
        MDrv_Write2Byte( 0x110d5c, 0x4444 ); //drv
        MDrv_Write2Byte( 0x110d5e, 0x4444 ); //drv
        MDrv_Write2Byte( 0x110d10, 0x0200 ); //reg_sel_dqs_diff
        MDrv_Write2Byte( 0x110d74, 0x3003 ); //trig level
    }
  #elif (MIU_SPEED == DDR3_1300MHz_e)//DDR3
    MDrv_Write2Byte( 0x110d7e, 0x0000 );
    MDrv_Write2Byte( 0x110d00, 0x0400 );
    MDrv_Write2Byte( 0x110d02, 0xaaaa );
    MDrv_Write2Byte( 0x110d08, 0x02ff );
    if(MDrv_ReadByte(0x001e03)==(U8)0x00)
    {
        MDrv_Write2Byte( 0x110d5a, 0x0002 ); //drv
        MDrv_Write2Byte( 0x110d5c, 0x7033 ); //drv
        MDrv_Write2Byte( 0x110d5e, 0x7033 ); //drv
        MDrv_Write2Byte( 0x110d10, 0x0200 ); //reg_sel_dqs_diff
        MDrv_Write2Byte( 0x110d74, 0x0333 ); //trig level
    }
    else
    {
        MDrv_Write2Byte( 0x110d5a, 0x0003 ); //drv
        MDrv_Write2Byte( 0x110d5c, 0x4444 ); //drv
        MDrv_Write2Byte( 0x110d5e, 0x4444 ); //drv
        MDrv_Write2Byte( 0x110d10, 0x0200 ); //reg_sel_dqs_diff
        MDrv_Write2Byte( 0x110d74, 0x3003 ); //trig level
    }
  #else
    #error "MIU clock speed isn't defined"
  #endif

}

static void MDrv_BandWidth_Adjustment(void)
{
  #if ((MIU_SPEED == DDR2_800MHz)||(MIU_SPEED == DDR2_800MHz_e))
    MDrv_Write2Byte(0x101240,0x8001);
    MDrv_Write2Byte(0x101260,0x8001);
  #elif ((MIU_SPEED == DDR2_1066MHz)||(MIU_SPEED == DDR2_1066MHz_e))
    MDrv_Write2Byte(0x101240,0xe001);
    MDrv_Write2Byte(0x101260,0xe001);
  #elif (MIU_SPEED == DDR3_1600MHz_e)
    MDrv_Write2Byte(0x101240,0x8001);
    MDrv_Write2Byte(0x101260,0x8001);
  #elif (MIU_SPEED == DDR3_1300MHz_e)
    MDrv_Write2Byte(0x101240,0x8001);
    MDrv_Write2Byte(0x101260,0x8001);
  #else
    #error "MIU interface isn't defined"
  #endif
    MDrv_Write2Byte(0x101280,0x8001); //enable group 2 run-robin
    MDrv_Write2Byte(0x1012a0,0x8001);
    MDrv_Write2Byte(0x10121a,0x0001);

  #if 1//default bandwidth settings
    //Let these IPs HVD/MVD/G3D(removed in Macaw) use MIU Act 2 bank function to improve DRAM total Bandwidth utilization
    MDrv_Write2Byte(0x10122a, 0xa3a4); //act 2 bank for HVD/MVD
    //MDrv_Write2Byte(0x10123e, 0x4000); //reg_post_order_ctrl_en (Removed for U01 / U02 compatible issue)
    MDrv_Write2Byte(0x101242, 0x0020); //limit 128 burst length
    MDrv_Write2Byte(0x101244, 0x0200); //timeout = 512T
    MDrv_Write2Byte(0x101240, 0x8415); //enable round_robin ; member_limit ; timeout ; skip_on
    MDrv_Write2Byte(0x101262, 0x0020); //limit 128 burst length
    MDrv_Write2Byte(0x101264, 0x0200); //timeout = 512T
    MDrv_Write2Byte(0x101260, 0x8415); //enable round_robin ; member_limit ; timeout ; skip_on
    MDrv_Write2Byte(0x101282, 0x0020); //limit 128 burst length
    MDrv_Write2Byte(0x101284, 0x0200); //timeout = 512T
    MDrv_Write2Byte(0x101280, 0x8415); //enable round_robin ; member_limit ; timeout ; skip_on
  #endif
}

static void MDrv_MiuAtopProgramDLL0(void)
{
    //start to program DLL0
    MDrv_Write2Byte(0x110d60,0x000c);
    MDrv_Write2Byte(0x110d60,0x0008);
  #if ((MIU_SPEED == DDR2_800MHz)||(MIU_SPEED == DDR2_800MHz_e))
    MDrv_Write2Byte(0x110d60,0x2738);
    MDrv_Write2Byte(0x110d62,0x0c7f); //U02 ECO setting for reg_dll_test[15:8]=0x0c
    MDrv_Write2Byte(0x110d64,0xf200);
  #elif ((MIU_SPEED == DDR2_1066MHz)||(MIU_SPEED == DDR2_1066MHz_e))
    MDrv_Write2Byte(0x110d60,0x2738);
    MDrv_Write2Byte(0x110d62,0x0c7f); //U02 ECO setting for reg_dll_test[15:8]=0x0c
    MDrv_Write2Byte(0x110d64,0xf200);
  #elif ((MIU_SPEED == DDR3_1600MHz_e)||(MIU_SPEED == DDR3_1300MHz_e))
    MDrv_Write2Byte(0x110d60,0x2738);
    MDrv_Write2Byte(0x110d62,0x007f);
    MDrv_Write2Byte(0x110d64,0xf200);
  #else
    #error "MIU interface isn't defined"
  #endif

  #if (RTL_SIMULATION == 1)
    MDrv_SysDelayUs(1);
  #else
    MDrv_SysDelayUs(10*1000);  // delay 10ms
  #endif
}

static void MDrv_MiuDDRInitMakeup(void)
{
    if(MDrv_ReadByte(0x001e03)==(U8)0x00)
    {
        // do nothing
        return;
    }

    #if (MIU_SPEED == DDR2_800MHz)
    MDrv_Write2Byte( 0x110d3a, 0x0112 );
    MDrv_Write2Byte( 0x110d6e, 0x0076 );
    #elif (MIU_SPEED == DDR2_1066MHz)
    MDrv_Write2Byte( 0x110d38, 0x0066 );
    MDrv_Write2Byte( 0x110d3a, 0x0440 );
    MDrv_Write2Byte( 0x110d6e, 0x0065 );
    #elif (MIU_SPEED == DDR3_1600MHz_e)
    MDrv_Write2Byte( 0x110d38, 0x0043 );
    MDrv_Write2Byte( 0x110d5a, 0x0003 );
    #elif (MIU_SPEED == DDR3_1300MHz_e)
    MDrv_Write2Byte( 0x110d38, 0x0043 );
    MDrv_Write2Byte( 0x110d5a, 0x0003 );
    #endif
}

static void MDrv_MiuBistTest_M12( void )
{
#define MIU0_SIZE 0x4000000
#define M12_MIU_UNIT 0x8

    MDrv_Write2Byte(0x1012e0 , 0x0000);
    MDrv_Write2Byte(0x1012e2 , 0x0000);

    MDrv_Write2Byte((RIUBASE_MIU + (0x0071 *2)), 0x0000);    //test base address
    MDrv_Write4Byte((RIUBASE_MIU + (0x0072 *2)),(MIU0_SIZE / M12_MIU_UNIT )  ); //test length
    MDrv_Write2Byte((RIUBASE_MIU + (0x0074 *2)), 0x5aa5);   //test data
    MDrv_Write2Byte((RIUBASE_MIU + (0x0070 *2)), 0x0003);   //enable test

    while (!MDrv_ReadRegBit( RIUBASE_MIU + (0x0070 *2) + 1, BIT7 ) );
    if( MDrv_Read2Byte( RIUBASE_MIU + (0x0070 *2) ) & 0x6000 )
        printf("BIST_0 error.\n");
    else
        printf("BIST_0 PASS.\n");
}

static int MDrv_MiuInitial_M12(void)
{
    //Macaw12 MIU atop initial
  #if ((MIU_SPEED == DDR3_1600MHz_e)||(MIU_SPEED == DDR3_1300MHz_e))
    MDrv_Write2Byte((0x1012fe ) , 0x0000);
    MDrv_Write2Byte((0x101220 ) , 0x0000);
  #endif
    MDrv_Write2Byte((0x10121e ) , 0x0c00);
    MDrv_Write2Byte((0x10121e ) , 0x0c00);
    MDrv_Write2Byte((0x10121e ) , 0x0c00);
    MDrv_Write2Byte((0x10121e ) , 0x0c01);
    MDrv_Write2Byte((0x10121e ) , 0x0c00);
    MDrv_Write2Byte((0x101202 ) , 0x0000);
  #if ((MIU_SPEED != DDR3_1600MHz_e))
    MDrv_Write2Byte((0x101246 ) , 0xFFFE);   //Mask MIU0_group_0
    MDrv_Write2Byte((0x101266 ) , 0xFFFF);   //Mask MIU0_group_1
    MDrv_Write2Byte((0x101286 ) , 0xFFFF);   //Mask MIU0_group_2
    MDrv_Write2Byte((0x1012a6 ) , 0xFFFF);   //Mask MIU0_group_3
  #endif

  #if ((MIU_SPEED == DDR3_1600MHz_e)||(MIU_SPEED == DDR3_1300MHz_e))
    MDrv_Write2Byte((0x100b3c ) , 0x0002); //select mempll0_clk025x_buf
  #endif
    MDrv_WriteMiuAtopTbl (  MIU_ATOP_Init );

    #if (RTL_SIMULATION == 1)
    MDrv_SysDelayUs(1);
    #else
    MDrv_SysDelayUs(1000);  // delay 1ms
    #endif

    MDrv_WriteMiuTbl ( MIU_DDR_Init );
    // compensate DDR initialized settings for U01/U02
    MDrv_MiuDDRInitMakeup();

    MDrv_MiuReset();
    MDrv_AutoInitMiuCycle();

    while( (MDrv_Read2Byte( RIUBASE_MIU  ) & 0x8000 ) == 0x0000 );

    MDrv_BandWidth_Adjustment();
    MDrv_MiuAtopProgramDLL0();
  #if ((MIU_SPEED == DDR3_1600MHz_e))
    MDrv_Write2Byte(0x1012f8, 0xffff); //Address balance mode
  #else
    MDrv_OpenAllMiumask();
    MDrv_Write2Byte(0x10121e, 0x0c08);
    MDrv_Write2Byte(0x1012f0, 0x0000);
  #endif

    //set dram size
  #if ((MIU_SPEED == DDR2_800MHz) || (MIU_SPEED == DDR2_1066MHz)) //DDR2_MCP
    MDrv_Write2Byte(0x1012c0, 0x0600 );
  #else
    MDrv_Write2Byte(0x1012c0, 0x0700 );
  #endif

    //for tvtool miu access
    MDrv_Write2Byte(0x002bc6, 0x3c11);
    MDrv_Write2Byte(0x002bca, 0x403c);
    MDrv_Write2Byte(0x002bc4, 0x0004);

  #if(RTL_SIMULATION == 0)
    MDrv_MiuBistTest_M12();
    MDrv_MiuAutoDQSPhase_M12( 0 );
  #else
    MDrv_Write2Byte(0x103380, 0x0004);
  #endif

    return 1;
}
#endif


static bool MDrv_MiuBistTest( U8 u8MIU_SEL, U32 u32BistSize, U8 u8LoopMode, U8 u8ReadOnly, U8 u8WriteOnly )
{
#if (CHIP_ID == MST9U3)
#define MIU_GROUP0_MASK     0xFFFE
#define MIU_GROUP2_MASK     0x7FFF
#else
#define MIU_GROUP0_MASK     0x7FFE
#define MIU_GROUP2_MASK     0xFFFF
#endif

    int i, bist_try;
    U16 u16TestMode, u16ExtraMode;
    bool bReturn =  0;

#if ENABLE_AUTO_DQS
    bist_try = 2;
#else
    bist_try = 10;
#endif

    u16TestMode  = (BIT2|BIT1); //BIST test mode [2:1]
    u16ExtraMode = u16TestMode;

    if (u8LoopMode)
        u16ExtraMode |= BIT4;
    if (u8ReadOnly)
        u16ExtraMode |= BIT8;
    if (u8WriteOnly)
        u16ExtraMode |= BIT9;

    printf(" (BIST_SIZE=0x%x) ", (unsigned int)u32BistSize);

    MDrv_Write2Byte(0x101246, MIU_GROUP0_MASK); //mask other request
    MDrv_Write2Byte(0x101266, 0xffff); //mask other request
    MDrv_Write2Byte(0x101286, MIU_GROUP2_MASK); //mask other request
    MDrv_Write2Byte(0x1012a6, 0xffff); //mask other request
#if (CHIP_ID == MST9U2)
    MDrv_Write2Byte(0x100646, MIU_GROUP0_MASK); //mask other request
    MDrv_Write2Byte(0x100666, 0xffff); //mask other request
    MDrv_Write2Byte(0x100686, MIU_GROUP2_MASK); //mask other request
    MDrv_Write2Byte(0x1006a6, 0xffff); //mask other request
#endif

    //DQS reset
    MDrv_Write2Byte(0x10110e, MDrv_Read2Byte(0x10110e)|BIT6);
    MDrv_Write2Byte(0x10110e, MDrv_Read2Byte(0x10110e)&~BIT6);
//-------------
//miu self test
//-------------
    MDrv_Write2Byte(0x1012e0, 0x0000);
#if (CHIP_ID == MST9U3)
    if (u8MIU_SEL == 1)
        MDrv_WriteByteMask(0x101203, 0x08, 0x08);
    else
        MDrv_WriteByteMask(0x101203, 0x00, 0x08);
#else
    if (u8MIU_SEL == 1)
        MDrv_Write2Byte(0x1012f0, 0x8000); //sel MIU1
    else
        MDrv_Write2Byte(0x1012f0, 0x0000); //sel MIU0
#endif

    MDrv_Write2Byte(0x1012e0, 0x0000);
    MDrv_Write2Byte(0x1012e2, 0x0000/BIST_BASE_UNIT);             //test base address
    MDrv_Write4Byte(0x1012e4, u32BistSize/MIU_UNIT); //test length
    MDrv_Write2Byte(0x1012e8, 0xff00);             //test pattern

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
            printf(" try(%d) passed..\r\n", i);
            bReturn = TRUE;
            break;
        }
        printf("\r\nbist=0x%x\r\n", MDrv_Read2Byte(0x1012e0));
        MDrv_Write2Byte(0x1012e0, u16ExtraMode);
        MDrv_SysDelayUs(1000);
        MDrv_Write2Byte(0x1012e0, u16ExtraMode|BIT0);
    }
    if (i >= bist_try)
    {
        printf(" failed..\r\n");
        bReturn = FALSE;
    }

    MDrv_Write2Byte(0x101246, 0x0000); //unmask other request
    MDrv_Write2Byte(0x101266, 0x0000); //unmask other request
    MDrv_Write2Byte(0x101286, 0x0000); //unmask other request
    MDrv_Write2Byte(0x1012a6, 0x0000); //unmask other request
#if (CHIP_ID == MST9U2)
    MDrv_Write2Byte(0x100646, 0x0000); //unmask other request
    MDrv_Write2Byte(0x100666, 0x0000); //unmask other request
    MDrv_Write2Byte(0x100686, 0x0000); //unmask other request
    MDrv_Write2Byte(0x1006a6, 0x0000); //unmask other request
#endif

    return bReturn;
}

U8 MDrv_MiuAutoDQSPhase( U8 u8MIU_SEL );
U8 MDrv_MiuAutoDQSPhase( U8 u8MIU_SEL )
{
#define AUTO_DQS_DBG  0
#define DQSN_4(x)  (x/16)

    U8 u8Step;
    U8 u8Mode;
    U8 u8DqsSum;
    U8 u8DqsCnt,u8TotalGoodPhase;
    U32 u32DqsPhAddr = 0;
    U32 u32PhaseResult = 0;
    U16 u16Write = 0;
    U16 u16DqsPhase;
    U16 u16Round = 0;
    U8 u8offset; //offset for MIU0 or MIU1
#if ( CHIP_ID == MST9U3 )
    if(u8MIU_SEL == 0)
    {
        u8offset = 0;
        MDrv_WriteByteMask( 0x101203, 0x00, 0x08 );
    }
    else
    {
        u8offset = 0x80;
        MDrv_WriteByteMask( 0x101203, 0x08, 0x08 );
    }
#else
    if(u8MIU_SEL == 0)
    {
        u8offset = 0;
        MDrv_Write2Byte( 0x1012f0, 0x0000 );
    }
    else
    {
        u8offset = 0x80;
        MDrv_Write2Byte( 0x1012f0, 0x8000 );
    }
#endif
    for (u16Round=0; u16Round<2; u16Round++)
    {
        if (u16Round==0)
            u32DqsPhAddr = ( 0x10116C + u8offset );//0x10116E;
        else
            u32DqsPhAddr = ( 0x10116E + u8offset );//0x10116C;

        MDrv_Write2Byte( 0x1012E2 , SET_AUTO_DQS_START_ADDR );  //(4kbytes)/unit
#if 0
        MDrv_Write4Byte( 0x1012E4 , SET_AUTO_DQS_VERIFY_SIZE ); //(16 byte)/unit
#else
        MDrv_Write4Byte( 0x1012E4 , 0x40); //(16 byte)/unit
#endif
        MDrv_Write2Byte( 0x1012E8 , 0xFF);   //test data

        u8DqsSum = 0;
        u8DqsCnt = 0;
        u8TotalGoodPhase=0;
        u16DqsPhase = MDrv_Read2Byte( u32DqsPhAddr );

#if (AUTO_DQS_DBG)
        if(u8MIU_SEL == 0)
            printf("\r\nAutoDQS bist for MIU0...");
        else
            printf("\r\nAutoDQS bist for MIU1...");
#endif

        uart_putc('\r');
        uart_putc('\n');
        for ( u8Step = 0; u8Step < 64; u8Step++ )
        {
            if (!(u8Step & 0x0F)) uart_putc('[');

            MDrv_WriteByte((0x10110E + u8offset), MDrv_ReadByte((0x10110E + u8offset))|0x40 );

            if(DQSN_4(u8Step)==3)
                u16Write=(((u8Step & 0x0F) << 12) | (u16DqsPhase&0x0FFF));
            else if(DQSN_4(u8Step)==2)
                u16Write=(((u8Step & 0x0F) << 8) | (u16DqsPhase&0xF0FF));
            else if(DQSN_4(u8Step)==1)
                u16Write=(((u8Step & 0x0F) << 4) | (u16DqsPhase&0xFF0F));
            else if(DQSN_4(u8Step)==0)
                u16Write=(((u8Step & 0x0F) << 0) | (u16DqsPhase&0xFFF0));
            MDrv_Write2Byte( u32DqsPhAddr, u16Write);

            MDrv_WriteByte((0x10110E + u8offset), MDrv_ReadByte((0x10110E + u8offset))&(~0x40) );

            for ( u8Mode = 0 ; u8Mode < sizeof(g_MemTestMode) ; ++u8Mode )
            {
                const U32 BIST_TRY_MAX = 100;
                U32 u32BistTry = 0;

                MDrv_Write2Byte(0x1012E0, 0x0000 );
                MDrv_Write2Byte(0x1012E0, (U16)g_MemTestMode[u8Mode] );  // pattern from data

                while ( !(MDrv_Read2Byte(0x1012E0)&0x8000) && (u32BistTry++ < BIST_TRY_MAX));

                if (!(MDrv_Read2Byte(0x1012E0)&0x8000))
                {
#if (AUTO_DQS_DBG)
                    printf("AutoDQS bist failed(try=%d)..\r\n", (int)u32BistTry);
#endif
                    break;
                }

                if ( MDrv_Read2Byte(0x1012E0)&0x6000)
                {
                    break;
                }
            }
            MDrv_Write2Byte(0x1012E0, 0x0000 );

            if ( u8Mode ==  sizeof( g_MemTestMode ) )
            {
#if (AUTO_DQS_DBG)
                if((u8Step%16)<10) uart_putc('0' + (u8Step % 16));
                else uart_putc('A' + (u8Step % 16) - 10);
#endif
                u8DqsSum += (u8Step & 0x0F);
                ++u8DqsCnt;
                u32PhaseResult |= (1 << u8Step);
            }

            if ( (u8Step % 16 == 15) )
            {
                if (u8DqsCnt)
                {

                    if(DQSN_4(u8Step)==3) u16DqsPhase=(u16DqsPhase & 0x0FFF);
                    else if(DQSN_4(u8Step)==2) u16DqsPhase=(u16DqsPhase & 0xF0FF);
                    else if(DQSN_4(u8Step)==1) u16DqsPhase=(u16DqsPhase & 0xFF0F);
                    else if(DQSN_4(u8Step)==0) u16DqsPhase=(u16DqsPhase & 0xFFF0);
                    u16DqsPhase |= ((u8DqsSum / u8DqsCnt) << (DQSN_4( u8Step ) * 4));
                    u8DqsSum = 0;
                    u8TotalGoodPhase+=u8DqsCnt;
                    u8DqsCnt = 0;
                }
                uart_putc(']');
            }
        }

        //refine the MIU DQS phase code flow for HW issue
        MDrv_WriteByte((0x10110E + u8offset)/*0x110D0E*/, MDrv_ReadByte((0x10110E + u8offset)/*0x110D0E*/)|0x40 );
        MDrv_Write2Byte( u32DqsPhAddr, u16DqsPhase );
        MDrv_WriteByte((0x10110E + u8offset)/*0x110D0E*/, MDrv_ReadByte((0x10110E + u8offset)/*0x110D0E*/)&(~0x40) );

        //MDrv_Write4Byte(0x103390, u32PhaseResult);

        uart_putc( ' ' );
        printf( "%04X", MDrv_Read2Byte(u32DqsPhAddr) );
        uart_putc( '\n' ) ;
    }
    return( u8TotalGoodPhase );
}

static int InitialSystemPowerUp( void )
{
  #if FPGA_VERIFY
    return 0;
  #endif

//-----------------------------------------------------------------
// MPLL setting
//-----------------------------------------------------------------
    MDrv_Write2Byte(0x101Ed0, 0x0402);
    MDrv_Write2Byte(0x101Edc, 0x0008);

//-----------------------------------------------------------------
//  UTMI  setting
//-----------------------------------------------------------------
    MDrv_Write2Byte(0x150308, 0x040f);
    MDrv_Write2Byte(0x150320, 0x80a1);
    MDrv_Write2Byte(0x150322, 0x0088);
    MDrv_Write2Byte(0x150300, 0x0001);
    MDrv_Write2Byte(0x150338, 0x2801);

//-------------------------------------------------------------------
//  turn on all clocks
//-------------------------------------------------------------------
    MDrv_Write2Byte(0x120F24, 0x0000);
    MDrv_Write2Byte(0x120F26, 0x0000);
    MDrv_Write2Byte(0x120F28, 0x0800);
    MDrv_Write2Byte(0x120F2a, 0x0001);
    MDrv_Write2Byte(0x120F2c, 0x0000);
    MDrv_Write2Byte(0x120F2e, 0x0800);
    MDrv_Write2Byte(0x120F30, 0x0000);
    MDrv_Write2Byte(0x120F32, 0x0000);
    MDrv_Write2Byte(0x120F34, 0x0000);
    MDrv_Write2Byte(0x120F36, 0x0000);
    MDrv_Write2Byte(0x120F38, 0x0008);
    MDrv_Write2Byte(0x120F3a, 0x0000);
    MDrv_Write2Byte(0x120F3e, 0x2020);
    MDrv_Write2Byte(0x120F44, 0x0000);

//-------------------------------------------------------------------
//  clk_mcu ("switch nonPM clk_mcu to 216MHz");
//-------------------------------------------------------------------
    MDrv_WriteByte(0x120f20, 0x01);

//-------------------------------------------------------------------
//  clk_mcu ("switch PM clk_mcu to <MCU_SPEED_INDEX> in PM code");
//-------------------------------------------------------------------
    //////////////////////////////////////////////////
    // need sync with PM code setting for AC_ON consistency //
    if (!IS_WAKEUP_FROM_PM) // AC ON
    {
        #define    IDX_MCU_CLK_4MHZ      0
        #define    IDX_MCU_CLK_12MHZ     1
        #define    IDX_MCU_CLK_216MHZ    2
        #define    IDX_MCU_CLK_MPLL_DIV  3
        #define    IDX_MCU_CLK_108MHZ    4
        #define    IDX_MCU_CLK_86MHZ     5
        #define    IDX_MCU_CLK_54MHZ     6
        #define    IDX_MCU_CLK_RESERVED0 7
        #define    IDX_MCU_CLK_XTAL      8
#if ( CHIP_ID == MST9U3 )
        MDrv_WriteByte(0x0003bb, IDX_MCU_CLK_MPLL_DIV); // need sync with PM code setting for AC_ON consistency
#else
        MDrv_WriteByte(0x0003bb, IDX_MCU_CLK_216MHZ); // need sync with PM code setting for AC_ON consistency
#endif
        MDrv_WriteByte(0x0003bc, 0x01);
    }
    // need sync with PM code setting for AC_ON consistency //
    //////////////////////////////////////////////////

//-------------------------------------------------------------------
//  clk_r2 ("switch clk_r2 to 345MHz");
//-------------------------------------------------------------------
    //MDrv_WriteByte(0x120f0b, 0x20);  //345 Mhz
    MDrv_WriteByte(0x120f0b, 0x20|R2_CLK_SET);  //320 Mhz

//-------------------------------------------------------------------
//  clk_pq_r2 ("switch clk_pq_r2 to 345MHz");
//-------------------------------------------------------------------
    //MDrv_WriteByte(0x120f0c, 0x20);  //345 Mhz
    MDrv_WriteByte(0x120f0c, 0x20|R2_CLK_SET);  //320 Mhz

//-------------------------------------------------------------------
//  SPI_clk = 54Mhz, [4:2]:spi clk sel 000~100: xtal, 101: 54, 110: 86, 111: 108
//-------------------------------------------------------------------
    MDrv_WriteByteMask(0x120f08, 0x14, 0x1F);

    return 1;
}

void MDrv_MiuInitial_FPGA(void);
void MDrv_MiuInitial_FPGA(void)
{
    MDrv_Write2Byte((0x2bc6 ) , 0x3c19);
    MDrv_Write2Byte((0x2bca ) , 0x403c);
    MDrv_Write2Byte((0x2bc4 ) , 0x0004);


    // MIU Init
    MDrv_Write2Byte((0x101202 ) , 0x0000);
    MDrv_Write2Byte((0x101202 ) , 0x0818);
    MDrv_Write2Byte((0x101204 ) , 0x01C5);
    MDrv_Write2Byte((0x101206 ) , 0x0120);
    MDrv_Write2Byte((0x101208 ) , 0x0833);
    MDrv_Write2Byte((0x10120A ) , 0x0C33);
    MDrv_Write2Byte((0x10120C ) , 0x7111);
    MDrv_Write2Byte((0x10120E ) , 0x100E);
    MDrv_Write2Byte((0x101210 ) , 0x0031);
    MDrv_Write2Byte((0x10121E ) , 0x1a08);

    MDrv_Write2Byte((0x10122A ) , 0x400F);

    MDrv_Write2Byte((0x101200 ) , 0x0000);
    MDrv_Write2Byte((0x101200 ) , 0x0008);
    MDrv_Write2Byte((0x101200 ) , 0x000C);
    MDrv_Write2Byte((0x101200 ) , 0x000E);
    MDrv_Write2Byte((0x101200 ) , 0x001F);

    // DRAM BIST
    MDrv_Write2Byte((0x1012E2 ) , 0x0000); // base
    MDrv_Write2Byte((0x1012E4 ) , 0x0021); // length
    MDrv_Write2Byte((0x1012E6 ) , 0x0000); // length
    MDrv_Write2Byte((0x1012E8 ) , 0x5AA5); // test data
    MDrv_Write2Byte((0x1012E0 ) , 0x0001); // reg_test_en :[0]


    printf("\n\r MIU BIST Starting ... \n\r");
    while( (BIT15 & MDrv_Read2Byte(0x1012E0))==0 );

    printf("\n\r MIU BIST OK.\n\r");
}

#if (CHIP_ID==MST9U)
#if (MIU_SPEED == DDR3_1866MHz_e)
void MiuInit_1866Mhz(U8 u8STRInit);
void MiuInit_1866Mhz(U8 u8STRInit)  // 1866Mhz
{
#define CLK_DDR_1866    1866  // 1866,  1600,  1400
//-------------------------------------------------------------------
// initialize MIU
//-------------------------------------------------------------------
// =====MIU0===
    MDrv_Write2Byte(0x101220, 0x0000);
    MDrv_Write2Byte(0x10121e, 0x0c00);
    MDrv_Write2Byte(0x10121e, 0x0c00);
    MDrv_Write2Byte(0x10121e, 0x0c00);
    MDrv_Write2Byte(0x10121e, 0x0c01);
    MDrv_Write2Byte(0x10121e, 0x0c00);
    MDrv_Write2Byte(0x101246, 0xfffe); //mask other request
    MDrv_Write2Byte(0x101266, 0xffff); //mask other request
    MDrv_Write2Byte(0x101286, 0xffff); //mask other request
    MDrv_Write2Byte(0x1012a6, 0xffff); //mask other request
    MDrv_Write2Byte(0x101124, 0x1000); //It needs ddrat[28] to do rst avoid unknown condition
    MDrv_Write2Byte(0x101124, 0x0000);
//-----------------------
//set DDRPLL0 1866MHz
//-----------------------
    MDrv_Write2Byte(0x101136, MIU_0_CLK);
    MDrv_Write2Byte(0x101134, 0x0400);
    MDrv_Write2Byte(0x101130, 0xeec9);
#if (CLK_DDR_1866 == 1866)
    MDrv_Write2Byte(0x101132, 0x001d);
#elif  (CLK_DDR_1866 == 1600)
    MDrv_Write2Byte(0x101132, 0x0022);
#else
    MDrv_Write2Byte(0x101132, 0x0026); // 1400Mhz.
#endif
    MDrv_Write2Byte(0x101120, 0x0020);

    //wait 1    // delay 1ms
    MDrv_SysDelayUs(1000);
//---------------------------
//set DDR3_32_8X_CL13_1866
//---------------------------
    if(u8STRInit)
    {
        MDrv_Write2Byte(0x101202, 0xf3a7);
    }
    else
    {
        MDrv_Write2Byte(0x101202, 0x03a7);
    }
    MDrv_Write2Byte(0x101204, 0x004b);
    MDrv_Write2Byte(0x101206, 0x1538);  //[12:12] reg_cke_always_on
    MDrv_Write2Byte(0x101208, 0x20dd);
    MDrv_Write2Byte(0x10120a, 0x2e76);
    MDrv_Write2Byte(0x10120c, 0xa7e9);
    MDrv_Write2Byte(0x10120e, 0x4096);
    MDrv_Write2Byte(0x101210, 0x1f14);
    MDrv_Write2Byte(0x101212, 0x4004);
    MDrv_Write2Byte(0x101214, 0x8020);
    MDrv_Write2Byte(0x101216, 0xc000);
    MDrv_Write2Byte(0x101102, 0xaaaa); //[ 7: 0] reg_cko_state
                           //[ 7: 0] reg_dqs_state
    if(u8STRInit)
    {
        MDrv_Write2Byte(0x101202, 0xf3a7);
    }
    else
    {
        MDrv_Write2Byte(0x101202, 0x03a7);
    }

    MDrv_Write2Byte(0x10122a, 0x4000);

    if(u8STRInit)
    {
        MDrv_Write2Byte(0x101202, 0xf3a7);
    }
    else
    {
        MDrv_Write2Byte(0x101202, 0x03a7);
    }

    MDrv_Write2Byte(0x101104, 0x0080); //[ 7: 0] reg_dqs_pre_state
    MDrv_Write2Byte(0x10113c, MIU_0_DQS_CS_SKEW); //[ 2: 0] reg_dqsm0_skew
                           //[ 6: 4] reg_dqsm1_skew
                           //[10: 8] reg_dqs_skew
                           //[14:12] reg_cmd1_skew
    MDrv_Write2Byte(0x10113e, MIU_0_DQS_CS_CLK_SKEW); //[ 2: 0] reg_oen_skew
                           //[ 6: 4] reg_dq_skew
                           //[10: 8] reg_cmd0_skew
                           //[14:12] reg_cko_skew
    MDrv_Write2Byte(0x10114c, MIU_0_RDQSM_SKEW_H); //[ 2: 0] reg_dqsm2_skew
                           //[ 6: 4] reg_dqsm3_skew
    MDrv_Write2Byte(0x101150, MIU_0_CSZ_SKEW); //[ 2: 0] reg_csz0_skew
                           //[ 6: 4] reg_csz1_skew
                           //[10: 8] reg_csz2_skew
                           //[14:12] reg_csz3_skew
    MDrv_Write2Byte(0x101138, MIU_0_DQ_DQS_L); //[ 3: 0] reg_clkph_dq0
                           //[ 7: 4] reg_clkph_dq1
                           //[11: 8] reg_clkph_dqs0
                           //[15:12] reg_clkph_dqs1
    MDrv_Write2Byte(0x10113a, MIU_0_RDQSM_CMD_L); //[ 3: 0] reg_clkph_cmd
                           //[ 7: 4] reg_clkph_dqsm0
                           //[11: 8] reg_clkph_clk_a
                           //[15:12] reg_clkph_dqsm1
    MDrv_Write2Byte(0x101148, MIU_0_DQ_DQS_H); //[ 3: 0] reg_clkph_dq2
                           //[ 7: 4] reg_clkph_dq3
                           //[11: 8] reg_clkph_dqs2
                           //[15:12] reg_clkph_dqs3
    MDrv_Write2Byte(0x10114a, MIU_0_RDQSM_CMD_H); //[ 3: 0] reg_clkph_cmd1
                           //[ 7: 4] reg_clkph_dqsm2
                           //[11: 8] reg_reserved_25_1
                           //[15:12] reg_clkph_dqsm3
    MDrv_Write2Byte(0x101152, MIU_0_CS); //[ 3: 0] reg_clkph_csz0
                           //[ 7: 4] reg_clkph_csz1
                           //[15: 8] reg_reserved_29
    MDrv_Write2Byte(0x101168, 0x0020); //[ 9: 0] reg_dll0_code
    MDrv_Write2Byte(0x10116a, 0x0020); //[ 9: 0] reg_dll1_code
    MDrv_Write2Byte(0x10116c, MIU_0_RDQS_L); //[ 3: 0] reg_dll4_ph
                           //[ 7: 4] reg_dll5_ph
                           //[11: 8] reg_dll6_ph
                           //[15:12] reg_dll7_ph
    MDrv_Write2Byte(0x10116e, MIU_0_RDQS_H); //[ 3: 0] reg_dll0_ph
                           //[ 7: 4] reg_dll1_ph
                           //[11: 8] reg_dll2_ph
                           //[15:12] reg_dll3_ph
    MDrv_Write2Byte(0x10110e, 0x00a9); //[ 0: 0] reg_en_mask
                           //[ 4: 1] reg_dqsm_dly
                           //[ 5: 5] reg_dqsm_sw_rst
                           //[ 6: 6] reg_dqsm_rst_sel
                           //[ 7: 7] reg_dqsm_sta_rst
                           //[11: 8] reg_r_dqs025x_snt
                           //[15:12] reg_r_dqsm_status_fall
    MDrv_Write2Byte(0x10110e, 0x00e9); //[ 0: 0] reg_en_mask
                           //[ 4: 1] reg_dqsm_dly
                           //[ 5: 5] reg_dqsm_sw_rst
                           //[ 6: 6] reg_dqsm_rst_sel
                           //[ 7: 7] reg_dqsm_sta_rst
                           //[11: 8] reg_r_dqs025x_snt
                           //[15:12] reg_r_dqsm_status_fal
    MDrv_Write2Byte(0x10110e, 0x00a9); //[ 0: 0] reg_en_mask
                           //[ 4: 1] reg_dqsm_dly
                           //[ 5: 5] reg_dqsm_sw_rst
                           //[ 6: 6] reg_dqsm_rst_sel
                           //[ 7: 7] reg_dqsm_sta_rst
                           //[11: 8] reg_r_dqs025x_snt
                           //[15:12] reg_r_dqsm_status_fall
    MDrv_Write2Byte(0x10121e, 0x0c01); //[ 0: 0] reg_sw_rst_miu
                           //[ 3: 3] reg_sw_init_done
    MDrv_Write2Byte(0x10121e, 0x0c00); //[ 0: 0] reg_sw_rst_miu
                           //[ 3: 3] reg_sw_init_done

    if(u8STRInit)
    {
        MDrv_Write2ByteMask(0x101100, MIU_0_ODT, BIT12|BIT11|BIT10|BIT9|BIT5); //[ 5: 5] reg_mclk_pd(this bit no use in MST9U1/2)
    }
    else
    {
        MDrv_Write2Byte(0x101100, 0x0018); //[ 0: 0] reg_pkg_sel
                               //[ 3: 3] reg_gpio_oenz
                               //[ 4: 4] reg_gpio_mode
                               //[ 5: 5] reg_mclk_pd(this bit no use)
        MDrv_Write2Byte(0x101100, 0x0010); //[ 0: 0] reg_pkg_sel
                               //[ 3: 3] reg_gpio_oenz
                               //[ 4: 4] reg_gpio_mode
                               //[ 5: 5] reg_mclk_pd(this bit no use)
        MDrv_Write2Byte(0x101100, 0x0000|MIU_0_ODT); //[ 0: 0] reg_pkg_sel
                               //[ 3: 3] reg_gpio_oenz
                               //[ 4: 4] reg_gpio_mode
                               //[ 5: 5] reg_mclk_pd(this bit no use)
    }

    MDrv_Write2Byte(0x10117e, 0x0000); //[15: 0] reg_ddr_test
    if(u8STRInit)
    {
        MDrv_Write2Byte(0x101154, 0xc070); //[15:14] reg_cmd_mode_sel
    }
    else
    {
        MDrv_Write2Byte(0x101154, 0xc000); //[15:14] reg_cmd_mode_sel
    }

    if(u8STRInit)
    {
        MDrv_Write2Byte(0x101108, 0x0000); //[ 5: 0] reg_rx_en
    }
    else
    {
        MDrv_Write2Byte(0x101108, 0x003f); //[ 5: 0] reg_rx_en
    }


    MDrv_Write2Byte(0x101174, MIU_0_TRIG_LVL); //trig lvl
    MDrv_Write2Byte(0x10115c, MIU_0_DRV_N); //drvn
    MDrv_Write2Byte(0x10115e, MIU_0_DRV_P); //drvp
    MDrv_Write2Byte(0x10115a, MIU_0_DRV_NP); //drvp
    MDrv_Write2Byte(0x101110, 0x0200); //[10:10] reg_drv_cmd

    MDrv_Write2Byte(0x10121e, 0x1000); //[ 0: 0] reg_sw_rst_miu   //updated @ 2013.07.15
                           //[ 3: 3] reg_sw_init_done

//$display($time," start to program DLL0 ");
    MDrv_Write2Byte(0x101160, 0x000c);
    MDrv_Write2Byte(0x101160, 0x0008);
    MDrv_Write2Byte(0x101162, 0x007f);
    MDrv_Write2Byte(0x101164, 0xf200);
    MDrv_Write2Byte(0x101160, 0x2370);

    MDrv_Write2Byte(0x101200, 0x0000);
    MDrv_Write2Byte(0x101200, 0x0008);
    MDrv_Write2Byte(0x101200, 0x000c);
//==================================================================
//--------Wait 10us, Reset Dram(200us in real case)
//==================================================================
    //wait 1 //delay 1ms
    MDrv_SysDelayUs(1000);
    MDrv_Write2Byte(0x101200, 0x000e);
//======================================================================
//--------Wait 500ns, dram initernal state initialization(real in 500us)
//======================================================================
    //wait 1 //delay 1ms
    MDrv_SysDelayUs(1000);
//--------Initial DRAM start here!!!
    MDrv_Write2Byte(0x101200, 0x001f);
    //wait 10 //check reg_101200 #15 for wait initial done
    MDrv_SysDelayUs(10000);

    MDrv_Write2Byte(0x101240, 0x0001); //[ 0: 0] reg_rq0_qound_robin
    MDrv_Write2Byte(0x101260, 0x0001); //[ 0: 0] reg_rq1_qound_robin
    MDrv_Write2Byte(0x101280, 0x0001); //[ 0: 0] reg_rq2_qound_robin
    MDrv_Write2Byte(0x1012a0, 0x0001); //[ 0: 0] reg_rq3_qound_robin
    MDrv_Write2Byte(0x101246, 0x0000); //[15: 0] reg_rq0_mask
    MDrv_Write2Byte(0x101266, 0x0000); //[15: 0] reg_rq1_mask
    MDrv_Write2Byte(0x101286, 0x0000); //[15: 0] reg_rq2_mask
    MDrv_Write2Byte(0x1012a6, 0x0000); //[15: 0] reg_rq3_mask
    MDrv_Write2Byte(0x101240, 0x8001); //[ 0: 0] reg_rq0_qound_robin
    MDrv_Write2Byte(0x101260, 0x8001); //[ 0: 0] reg_rq1_qound_robin
    MDrv_Write2Byte(0x101280, 0x8001); //[ 0: 0] reg_rq2_qound_robin
    MDrv_Write2Byte(0x1012a0, 0x8001); //[ 0: 0] reg_rq3_qound_robin
//    MDrv_Write2Byte(0x10121e, 0x0c08); //[ 0: 0] reg_sw_rst_miu
    MDrv_Write2Byte(0x10121e, 0x1008); //[ 0: 0] reg_sw_rst_miu   //updated @ 2013.07.15
                           //[ 3: 3] reg_sw_init_done

    MDrv_Write2Byte(0x1012fe, 0x30E1);
//    MDrv_Write2Byte(0x10123c, 0x0080//turn on adc reg_adc_test_en  ==>comment out by RD
}
#elif (MIU_SPEED == DDR3_2133MHz_e)
void MiuInit_2133Mhz(U8 u8STRInit);  //2133_2133
void MiuInit_2133Mhz(U8 u8STRInit)  //2133_2133
{
#define CLK_DDR_2133    2133  // 2133,  1866,  1600,  1400
//-------------------------------------------------------------------
// initialize MIU
//-------------------------------------------------------------------
// =====MIU0===
    MDrv_Write2Byte(0x101220, 0x0000);
    MDrv_Write2Byte(0x10121e, 0x0c00);
    MDrv_Write2Byte(0x10121e, 0x0c00);
    MDrv_Write2Byte(0x10121e, 0x0c00);
    MDrv_Write2Byte(0x10121e, 0x0c01);
    MDrv_Write2Byte(0x10121e, 0x0c00);
    MDrv_Write2Byte(0x101246, 0xfffe); //mask other request
    MDrv_Write2Byte(0x101266, 0xffff); //mask other request
    MDrv_Write2Byte(0x101286, 0xffff); //mask other request
    MDrv_Write2Byte(0x1012a6, 0xffff); //mask other request
    MDrv_Write2Byte(0x101124, 0x1000); //It needs ddrat[28] to do rst avoid unknown condition
    MDrv_Write2Byte(0x101124, 0x0000);
//-----------------------
//set DDRPLL0 2133MHz
//-----------------------
    MDrv_Write2Byte(0x101136, MIU_0_CLK);
    MDrv_Write2Byte(0x101134, 0x0400);
    MDrv_Write2Byte(0x101130, 0x2f97);
#if (CLK_DDR_2133 == 2133)
    MDrv_Write2Byte(0x101132, 0x001a); //1% decrease for SSC
#elif (CLK_DDR_2133 == 1866)
    MDrv_Write2Byte(0x101132, 0x001d);
#elif  (CLK_DDR_2133 == 1600)
    MDrv_Write2Byte(0x101132, 0x0022);
#else
    MDrv_Write2Byte(0x101132, 0x0027); // 1400Mhz.
#endif
    MDrv_Write2Byte(0x101120, 0x0020);
    //wait 1    // delay 1ms
    MDrv_SysDelayUs(1000);

    MDrv_Write2Byte(0x1013f0, 0x7fff);
    MDrv_Write2Byte(0x1013f2, 0xffff);
    MDrv_Write2Byte(0x1013f4, 0xffff);
    MDrv_Write2Byte(0x1013f6, 0xffff);
//---------------------------
//set DDR3_32_8X_CL14_2133
//---------------------------
    if(u8STRInit)
    {
        MDrv_Write2Byte(0x101202, 0xf3a7);
    }
    else
    {
        MDrv_Write2Byte(0x101202, 0x03a7);
    }
    MDrv_Write2Byte(0x101204, 0x004c);
    MDrv_Write2Byte(0x101206, 0x1640); //[12:12] reg_cke_always_on
    MDrv_Write2Byte(0x101208, 0x24ee);
    MDrv_Write2Byte(0x10120a, 0x3487);
    MDrv_Write2Byte(0x10120c, 0xc80a);
    MDrv_Write2Byte(0x10120e, 0xc0b7);
    MDrv_Write2Byte(0x101210, 0x1124);
    MDrv_Write2Byte(0x101212, 0x4004);
    MDrv_Write2Byte(0x101214, 0x8028);
    MDrv_Write2Byte(0x101216, 0xc000);
    MDrv_Write2Byte(0x101102, 0xaaaa); //[ 7: 0] reg_cko_state
                           //[ 7: 0] reg_dqs_state
    if(u8STRInit)
    {
        MDrv_Write2Byte(0x101202, 0xf3a7);
    }
    else
    {
        MDrv_Write2Byte(0x101202, 0x03a7);
    }
    MDrv_Write2Byte(0x10122a, 0x4000);
    if(u8STRInit)
    {
        MDrv_Write2Byte(0x101202, 0xf3a7);
    }
    else
    {
        MDrv_Write2Byte(0x101202, 0x03a7);
    }
    MDrv_Write2Byte(0x101104, 0x0080); //[ 7: 0] reg_dqs_pre_state
    MDrv_Write2Byte(0x10113c, MIU_0_DQS_CS_SKEW); //[ 2: 0] reg_dqsm0_skew
                           //[ 6: 4] reg_dqsm1_skew
                           //[10: 8] reg_dqs_skew
                           //[14:12] reg_cmd1_skew
    MDrv_Write2Byte(0x10113e, MIU_0_DQS_CS_CLK_SKEW); //[ 2: 0] reg_oen_skew
                           //[ 6: 4] reg_dq_skew
                           //[10: 8] reg_cmd0_skew
                           //[14:12] reg_cko_skew
    MDrv_Write2Byte(0x10114c, MIU_0_RDQSM_SKEW_H); //[ 2: 0] reg_dqsm2_skew
                           //[ 6: 4] reg_dqsm3_skew
    MDrv_Write2Byte(0x101150, MIU_0_CSZ_SKEW); //[ 2: 0] reg_csz0_skew
                           //[ 6: 4] reg_csz1_skew
                           //[10: 8] reg_csz2_skew
                           //[14:12] reg_csz3_skew
    MDrv_Write2Byte(0x101138, MIU_0_DQ_DQS_L); //[ 3: 0] reg_clkph_dq0
                           //[ 7: 4] reg_clkph_dq1
                           //[11: 8] reg_clkph_dqs0
                           //[15:12] reg_clkph_dqs1
    MDrv_Write2Byte(0x10113a, MIU_0_RDQSM_CMD_L); //[ 3: 0] reg_clkph_cmd
                           //[ 7: 4] reg_clkph_dqsm0
                           //[11: 8] reg_clkph_clk_a
                           //[15:12] reg_clkph_dqsm1
    MDrv_Write2Byte(0x101148, MIU_0_DQ_DQS_H); //[ 3: 0] reg_clkph_dq2
                           //[ 7: 4] reg_clkph_dq3
                           //[11: 8] reg_clkph_dqs2
                           //[15:12] reg_clkph_dqs3
    MDrv_Write2Byte(0x10114a, MIU_0_RDQSM_CMD_H); //[ 3: 0] reg_clkph_cmd1
                           //[ 7: 4] reg_clkph_dqsm2
                           //[11: 8] reg_reserved_25_1
                           //[15:12] reg_clkph_dqsm3
    MDrv_Write2Byte(0x101152, MIU_0_CS); //[ 3: 0] reg_clkph_csz0
                           //[ 7: 4] reg_clkph_csz1
                           //[15: 8] reg_reserved_29
//    MDrv_Write2Byte(0x101168, 0x0020 //[ 9: 0] reg_dll0_code  ==>comment out by RD
//    MDrv_Write2Byte(0x10116a, 0x0020 //[ 9: 0] reg_dll1_code  ==>comment out by RD
    MDrv_Write2Byte(0x10116c, MIU_0_RDQS_L); //[ 3: 0] reg_dll4_ph
                           //[ 7: 4] reg_dll5_ph
                           //[11: 8] reg_dll6_ph
                           //[15:12] reg_dll7_ph
    MDrv_Write2Byte(0x10116e, MIU_0_RDQS_H); //[ 3: 0] reg_dll0_ph
                           //[ 7: 4] reg_dll1_ph
                           //[11: 8] reg_dll2_ph
                           //[15:12] reg_dll3_ph
    MDrv_Write2Byte(0x10110e, 0x00ab); //[ 0: 0] reg_en_mask
                           //[ 4: 1] reg_dqsm_dly
                           //[ 5: 5] reg_dqsm_sw_rst
                           //[ 6: 6] reg_dqsm_rst_sel
                           //[ 7: 7] reg_dqsm_sta_rst
                           //[11: 8] reg_r_dqs025x_snt
                           //[15:12] reg_r_dqsm_status_fall
    MDrv_Write2Byte(0x10110e, 0x00eb); //[ 0: 0] reg_en_mask
                           //[ 4: 1] reg_dqsm_dly
                           //[ 5: 5] reg_dqsm_sw_rst
                           //[ 6: 6] reg_dqsm_rst_sel
                           //[ 7: 7] reg_dqsm_sta_rst
                           //[11: 8] reg_r_dqs025x_snt
                           //[15:12] reg_r_dqsm_status_fall
    MDrv_Write2Byte(0x10110e, 0x00ab); //[ 0: 0] reg_en_mask
                           //[ 4: 1] reg_dqsm_dly
                           //[ 5: 5] reg_dqsm_sw_rst
                           //[ 6: 6] reg_dqsm_rst_sel
                           //[ 7: 7] reg_dqsm_sta_rst
                           //[11: 8] reg_r_dqs025x_snt
                           //[15:12] reg_r_dqsm_status_fall
    MDrv_Write2Byte(0x10121e, 0x0c01); //[ 0: 0] reg_sw_rst_miu
                           //[ 3: 3] reg_sw_init_done
    MDrv_Write2Byte(0x10121e, 0x0c00); //[ 0: 0] reg_sw_rst_miu
                           //[ 3: 3] reg_sw_init_done
    if(u8STRInit)
    {
        MDrv_Write2ByteMask(0x101100, MIU_0_ODT, BIT12|BIT11|BIT10|BIT9|BIT5); //[ 5: 5] reg_mclk_pd(this bit no use in MST9U1/2)
    }
    else
    {
        MDrv_Write2Byte(0x101100, 0x0018); //[ 0: 0] reg_pkg_sel
                               //[ 3: 3] reg_gpio_oenz
                               //[ 4: 4] reg_gpio_mode
                               //[ 5: 5] reg_mclk_pd(this bit no use)
        MDrv_Write2Byte(0x101100, 0x0010); //[ 0: 0] reg_pkg_sel
                               //[ 3: 3] reg_gpio_oenz
                               //[ 4: 4] reg_gpio_mode
                               //[ 5: 5] reg_mclk_pd(this bit no use)
        MDrv_Write2Byte(0x101100, 0x0000|MIU_0_ODT); //[ 0: 0] reg_pkg_sel
                               //[ 3: 3] reg_gpio_oenz
                               //[ 4: 4] reg_gpio_mode
                               //[ 5: 5] reg_mclk_pd(this bit no use)
    }
    MDrv_Write2Byte(0x10117e, 0x0000); //[15: 0] reg_ddr_test
    if(u8STRInit)
    {
        MDrv_Write2Byte(0x101154, 0xc070); //[15:14] reg_cmd_mode_sel
    }
    else
    {
        MDrv_Write2Byte(0x101154, 0xc000); //[15:14] reg_cmd_mode_sel
    }

    if(u8STRInit)
    {
        MDrv_Write2Byte(0x101108, 0x0000); //[ 5: 0] reg_rx_en
    }
    else
    {
        MDrv_Write2Byte(0x101108, 0x003f); //[ 5: 0] reg_rx_en
    }

    MDrv_Write2Byte(0x101174, MIU_0_TRIG_LVL); //trig lvl
    MDrv_Write2Byte(0x10115c, MIU_0_DRV_N); //drvn
    MDrv_Write2Byte(0x10115e, MIU_0_DRV_P); //drvp
    MDrv_Write2Byte(0x10115a, MIU_0_DRV_NP); //drvp
    MDrv_Write2Byte(0x101110, 0x0200); //[10:10] reg_drv_cmd

    MDrv_Write2Byte(0x10121e, 0x1000); //[ 0: 0] reg_sw_rst_miu   //updated @ 2013.07.15
                           //[ 3: 3] reg_sw_init_done
//$display($time," start to program DLL0 ");
    MDrv_Write2Byte(0x101160, 0x000c);
    MDrv_Write2Byte(0x101160, 0x0008);
    MDrv_Write2Byte(0x101162, 0x007f);
    MDrv_Write2Byte(0x101164, 0xf200);
    MDrv_Write2Byte(0x101160, 0x2370);

    MDrv_Write2Byte(0x101200, 0x0000);
    MDrv_Write2Byte(0x101200, 0x0008);
    MDrv_Write2Byte(0x101200, 0x000c);
//==================================================================
//--------Wait 10us, Reset Dram(200us in real case)
//==================================================================
    //wait 1 //delay 1ms
    MDrv_SysDelayUs(1000);

    MDrv_Write2Byte(0x101200, 0x000e);
//======================================================================
//--------Wait 500ns, dram initernal state initialization(real in 500us)
//======================================================================
    //wait 1 //delay 1ms
    MDrv_SysDelayUs(1000);

//--------Initial DRAM start here!!!
    MDrv_Write2Byte(0x101200, 0x001f);
    //wait 10 //check reg_101200 #15 for wait initial done
    MDrv_SysDelayUs(10000);

    MDrv_Write2Byte(0x101240, 0x0001); //[ 0: 0] reg_rq0_qound_robin
    MDrv_Write2Byte(0x101260, 0x0001); //[ 0: 0] reg_rq1_qound_robin
    MDrv_Write2Byte(0x101280, 0x0001); //[ 0: 0] reg_rq2_qound_robin
    MDrv_Write2Byte(0x1012a0, 0x0001); //[ 0: 0] reg_rq3_qound_robin
    MDrv_Write2Byte(0x101246, 0x0000); //[15: 0] reg_rq0_mask
    MDrv_Write2Byte(0x101266, 0x0000); //[15: 0] reg_rq1_mask
    MDrv_Write2Byte(0x101286, 0x0000); //[15: 0] reg_rq2_mask
    MDrv_Write2Byte(0x1012a6, 0x0000); //[15: 0] reg_rq3_mask
    MDrv_Write2Byte(0x101240, 0x8001); //[ 0: 0] reg_rq0_qound_robin
    MDrv_Write2Byte(0x101260, 0x8001); //[ 0: 0] reg_rq1_qound_robin
    MDrv_Write2Byte(0x101280, 0x8001); //[ 0: 0] reg_rq2_qound_robin
    MDrv_Write2Byte(0x1012a0, 0x8001); //[ 0: 0] reg_rq3_qound_robin
//    MDrv_Write2Byte(0x10121e, 0x0c08); //[ 0: 0] reg_sw_rst_miu
    MDrv_Write2Byte(0x10121e, 0x1008); //[ 0: 0] reg_sw_rst_miu   //updated @ 2013.07.15
                           //[ 3: 3] reg_sw_init_done

    MDrv_Write2Byte(0x1012fe, 0x30E1);
//    MDrv_Write2Byte(0x10123c  0x0080//turn on adc reg_adc_test_en  ==>comment out by RD
}
#endif
#endif

#if (CHIP_ID==MST9U3)
#if MST9U3_BGA
void MiuInit_1333Mhz_PreInit(U8 u8STRInit);
void MiuInit_1333Mhz_PreInit(U8 u8STRInit)
{
    if(u8STRInit)
    {
        MDrv_Write2Byte(0x101100, 0x2018);
        MDrv_Write2Byte(0x101154, 0xC070);
        MDrv_Write2Byte(0x101180, 0x2018);
        MDrv_Write2Byte(0x1011D4, 0xC070);
        MDrv_Write2Byte(0x101202, (0xF392|DRAM_BUS_MASK));
        MDrv_Write2Byte(0x101100, 0x0208|MST9U3_BGA);
        MDrv_Write2Byte(0x101180, 0x0208|MST9U3_BGA);
        MDrv_Write2Byte(0x101108, 0x0000);
        MDrv_Write2Byte(0x101188, 0x0000);
    }
    else
    {
        MDrv_Write2Byte(0x101100, 0x2010);
        MDrv_Write2Byte(0x101154, 0xC000);
        MDrv_Write2Byte(0x101180, 0x2010);
        MDrv_Write2Byte(0x1011D4, 0xC000);
        MDrv_Write2Byte(0x101202, (0x0392|DRAM_BUS_MASK));
        MDrv_Write2Byte(0x101100, 0x0200|MST9U3_BGA);
        MDrv_Write2Byte(0x101180, 0x0200|MST9U3_BGA);
        MDrv_Write2Byte(0x101108, 0x003F);
        MDrv_Write2Byte(0x101188, 0x003F);
    }
}
#else
void MiuInit_1333Mhz_PreInit(U8 u8STRInit, U8 u8Type);
void MiuInit_1333Mhz_PreInit(U8 u8STRInit, U8 u8Type)
{
    if(u8STRInit)
    {
        MDrv_Write2Byte(0x101100, 0x2018);
        MDrv_Write2Byte(0x101180, 0x2018);
        if( u8Type )
        {
            MDrv_Write2Byte(0x101154, 0x8070);
            MDrv_Write2Byte(0x1011D4, 0x8070);
        }
        else
        {
            MDrv_Write2Byte(0x101154, 0xC070);
            MDrv_Write2Byte(0x1011D4, 0xC070);
        }
        MDrv_Write2Byte(0x101202, (0xF392|DRAM_BUS_MASK));
        MDrv_Write2Byte(0x101100, 0x0208|MST9U3_BGA);
        MDrv_Write2Byte(0x101180, 0x0208|MST9U3_BGA);
        MDrv_Write2Byte(0x101108, 0x0000);
        MDrv_Write2Byte(0x101188, 0x0000);
    }
    else
    {
        MDrv_Write2Byte(0x101100, 0x2010);
        MDrv_Write2Byte(0x101180, 0x2010);
        if( u8Type )
        {
            MDrv_Write2Byte(0x101154, 0x8000);
            MDrv_Write2Byte(0x1011D4, 0x8000);
        }
        else
        {
            MDrv_Write2Byte(0x101154, 0xC000);
            MDrv_Write2Byte(0x1011D4, 0xC000);
        }
        MDrv_Write2Byte(0x101202, (0x0392|DRAM_BUS_MASK));
        MDrv_Write2Byte(0x101100, 0x0200|MST9U3_BGA);
        MDrv_Write2Byte(0x101180, 0x0200|MST9U3_BGA);
        MDrv_Write2Byte(0x101108, 0x003F);
        MDrv_Write2Byte(0x101188, 0x003F);
    }
}
#endif

void MiuInit_1333Mhz_Winbond_QFP(void);  //2133_2133
void MiuInit_1333Mhz_Winbond_QFP(void)  //2133_2133
{
//-------------------------------------------------------------------
// initialize MIU
//-------------------------------------------------------------------
// =====MIU0===
    MDrv_Write2Byte(0x101220, 0x0000);
    MDrv_Write2Byte(0x10121e, 0x0c00);
    MDrv_Write2Byte(0x10121e, 0x0c00);
    MDrv_Write2Byte(0x10121e, 0x0c00);
    MDrv_Write2Byte(0x10121e, 0x0c01);
    MDrv_Write2Byte(0x10121e, 0x0c00);
    MDrv_Write2Byte(0x101246, 0xfffe); //mask other request
    MDrv_Write2Byte(0x101266, 0xffff); //mask other request
    MDrv_Write2Byte(0x101286, 0xffff); //mask other request
    MDrv_Write2Byte(0x1012a6, 0xffff); //mask other request

    MDrv_Write2Byte(0x101124, 0x1000); //It needs ddrat[28] to do rst avoid unknown condition
    MDrv_Write2Byte(0x1011A4, 0x1000);
    MDrv_Write2Byte(0x101124, 0x2000);
    MDrv_Write2Byte(0x1011A4, 0x2000);
    MDrv_Write2Byte(0x101178, 0x0102);
    MDrv_Write2Byte(0x1011F8, 0x0102);
//-----------------------
//set DDRPLL0 1866MHz
//-----------------------
    MDrv_Write2Byte(0x101136, 0x4000);
    //MDrv_Write2Byte(0x101136, MIU_0_CLK);
    MDrv_Write2Byte(0x101134, 0x0000);
    MDrv_Write2Byte(0x101130, 0x8000);
#if (MIU_SPEED == DDR2_1333MHz_e)
    MDrv_Write2Byte(0x101132, 0x0029);
#else   // 1240
    MDrv_Write2Byte(0x101132, 0x002C);
#endif
    MDrv_Write2Byte(0x101120, 0x0020);

    MDrv_Write2Byte(0x1011B6, 0x4000);
    MDrv_Write2Byte(0x1011B4, 0x0000);
    MDrv_Write2Byte(0x1011B0, 0x8000);
#if (MIU_SPEED == DDR2_1333MHz_e)
    MDrv_Write2Byte(0x1011B2, 0x0029);
#else   // 1240
    MDrv_Write2Byte(0x1011B2, 0x002C);
#endif
    MDrv_Write2Byte(0x1011A0, 0x0020);

    //wait 1    // delay 1ms
    MDrv_SysDelayUs(1000);
//---------------------------
//set DDR3_32_8X_CL13_1866
//---------------------------
    //MDrv_Write2Byte(0x101202, (0xf392|DRAM_BUS_MASK));
    MDrv_Write2Byte(0x101204, 0x000a);
#if (MIU_SPEED == DDR2_1333MHz_e)
    MDrv_Write2Byte(0x101206, 0x0340);  //[12:12] reg_cke_always_on
#else
    MDrv_Write2Byte(0x101206, 0x1B40);  //[12:12] reg_cke_always_on
#endif
    MDrv_Write2Byte(0x101208, 0x1e99);
    MDrv_Write2Byte(0x10120a, 0x2767);
    MDrv_Write2Byte(0x10120c, 0x5598);
    MDrv_Write2Byte(0x10120e, 0x2046);
    MDrv_Write2Byte(0x101210, 0x0003);
    MDrv_Write2Byte(0x101212, 0x4004);
    MDrv_Write2Byte(0x101214, 0x8000);
    MDrv_Write2Byte(0x101216, 0xc000);
    MDrv_Write2Byte(0x101102, 0xaaaa); //[ 7: 0] reg_cko_state
                           //[ 7: 0] reg_dqs_state
    //MDrv_Write2Byte(0x101202, 0x03a7);
    //MDrv_Write2Byte(0x10122a, 0x4000);
    //MDrv_Write2Byte(0x101202, 0x03a7);
    MDrv_Write2Byte(0x101104, 0x0080); //[ 7: 0] reg_dqs_pre_state
#if (MIU_SPEED == DDR2_1333MHz_e)
    MDrv_Write2Byte(0x10113c, 0x1244); //[ 2: 0] reg_dqsm0_skew
#else
    MDrv_Write2Byte(0x10113c, 0x2244); //[ 2: 0] reg_dqsm0_skew
#endif
    //MDrv_Write2Byte(0x10113c, MIU_0_DQS_CS_SKEW); //[ 2: 0] reg_dqsm0_skew
                           //[ 6: 4] reg_dqsm1_skew
                           //[10: 8] reg_dqs_skew
                           //[14:12] reg_cmd1_skew
    //MDrv_Write2Byte(0x10113e, MIU_0_DQS_CS_CLK_SKEW); //[ 2: 0] reg_oen_skew
    MDrv_Write2Byte(0x10113e, 0x0122); //[ 2: 0] reg_oen_skew
                           //[ 6: 4] reg_dq_skew
                           //[10: 8] reg_cmd0_skew
                           //[14:12] reg_cko_skew
   //MDrv_Write2Byte(0x10114c, MIU_0_RDQSM_SKEW_H); //[ 2: 0] reg_dqsm2_skew
                           //[ 6: 4] reg_dqsm3_skew
    //MDrv_Write2Byte(0x101150, MIU_0_CSZ_SKEW); //[ 2: 0] reg_csz0_skew
   MDrv_Write2Byte(0x101150, 0x2222); //[ 2: 0] reg_csz0_skew
                           //[ 6: 4] reg_csz1_skew
                           //[10: 8] reg_csz2_skew
                           //[14:12] reg_csz3_skew
    MDrv_Write2Byte(0x101138, 0x0065); //[ 3: 0] reg_clkph_dq0
    //MDrv_Write2Byte(0x101138, MIU_0_DQ_DQS_L); //[ 3: 0] reg_clkph_dq0
                           //[ 7: 4] reg_clkph_dq1
                           //[11: 8] reg_clkph_dqs0
                           //[15:12] reg_clkph_dqs1
    //MDrv_Write2Byte(0x10113a, MIU_0_RDQSM_CMD_L); //[ 3: 0] reg_clkph_cmd
#if (MIU_SPEED == DDR2_1333MHz_e)
    MDrv_Write2Byte(0x10113a, 0x2020); //[ 3: 0] reg_clkph_cmd
#else
    MDrv_Write2Byte(0x10113a, 0x2424); //[ 3: 0] reg_clkph_cmd
#endif
                           //[ 7: 4] reg_clkph_dqsm0
                           //[11: 8] reg_clkph_clk_a
                           //[15:12] reg_clkph_dqsm1
    MDrv_Write2Byte(0x10110a, 0x1144);
    MDrv_Write2Byte(0x101152, 0x0000);
    MDrv_Write2Byte(0x10112E, 0x2200);

    MDrv_Write2Byte(0x10110E, 0x00A5);
    MDrv_Write2Byte(0x10110E, 0x00E5);
    MDrv_Write2Byte(0x10110E, 0x00A5);

    MDrv_Write2Byte(0x101168, 0x2020);
    MDrv_Write2Byte(0x10116A, 0x2020);
    MDrv_Write2Byte(0x10116E, 0x6666);
    MDrv_Write2Byte(0x10116C, 0x0000);
    MDrv_Write2Byte(0x101118, 0x0000);
    MDrv_Write2Byte(0x101158, 0x0707);
    MDrv_Write2Byte(0x10115A, 0x2222);
    MDrv_Write2Byte(0x10115C, 0x2222);
    MDrv_Write2Byte(0x10115E, 0x2222);
    MDrv_Write2Byte(0x101174, 0x8882);

    MDrv_Write2Byte(0x101182, 0xaaaa);
    MDrv_Write2Byte(0x101184, 0x0080);
#if (MIU_SPEED == DDR2_1333MHz_e)
    MDrv_Write2Byte(0x1011bc, 0x1244);
#else
    MDrv_Write2Byte(0x1011bc, 0x2244);
#endif
    MDrv_Write2Byte(0x1011be, 0x0122);
    MDrv_Write2Byte(0x1011d0, 0x2222);
    MDrv_Write2Byte(0x1011b8, 0x0065);
#if (MIU_SPEED == DDR2_1333MHz_e)
    MDrv_Write2Byte(0x1011ba, 0x2020);
#else
    MDrv_Write2Byte(0x1011ba, 0x2424);
#endif
    MDrv_Write2Byte(0x10118a, 0x1144);
    MDrv_Write2Byte(0x1011d2, 0x0000);
    MDrv_Write2Byte(0x1011ae, 0x2200);

    MDrv_Write2Byte(0x10118E, 0x00A5);
    MDrv_Write2Byte(0x10118E, 0x00E5);
    MDrv_Write2Byte(0x10118E, 0x00A5);

    MDrv_Write2Byte(0x1011e8, 0x2020);
    MDrv_Write2Byte(0x1011EA, 0x2020);
    MDrv_Write2Byte(0x1011ee, 0x6666);
    MDrv_Write2Byte(0x1011ec, 0x0000);
    MDrv_Write2Byte(0x101198, 0x0000);
    MDrv_Write2Byte(0x1011d8, 0x0707);
#if (MIU_SPEED == DDR2_1333MHz_e)
    MDrv_Write2Byte(0x1011dA, 0x4444);
    MDrv_Write2Byte(0x1011dC, 0x4444);
    MDrv_Write2Byte(0x1011dE, 0x4444);
#else
    MDrv_Write2Byte(0x1011dA, 0x2222);
    MDrv_Write2Byte(0x1011dC, 0x2222);
    MDrv_Write2Byte(0x1011dE, 0x2222);
#endif
    MDrv_Write2Byte(0x1011f4, 0x8882);

    MDrv_Write2Byte(0x10121E, 0x0c01);
    MDrv_Write2Byte(0x10121E, 0x0c00);

    MDrv_Write2Byte(0x10117c, 0xfc00);
    MDrv_Write2Byte(0x1011fc, 0xfc00);
    MDrv_Write2Byte(0x101160, 0x000c);
    MDrv_Write2Byte(0x101160, 0x0008);
    MDrv_Write2Byte(0x101160, 0x2370);
    MDrv_Write2Byte(0x1011e0, 0x000c);
    MDrv_Write2Byte(0x1011e0, 0x0008);
    MDrv_Write2Byte(0x1011e0, 0x2370);


    MDrv_Write2Byte(0x101240, 0x8001);
    MDrv_Write2Byte(0x101260, 0x8001);
    MDrv_Write2Byte(0x101280, 0x8001);
    MDrv_Write2Byte(0x1012a0, 0x8001);
    MDrv_Write2Byte(0x101110, 0x0200);
    MDrv_Write2Byte(0x101190, 0x0200);
    MDrv_Write2Byte(0x10121e, 0x1a00);
    MDrv_Write2Byte(0x1012f0, 0x0000);
    MDrv_Write2Byte(0x1012d2, 0x0000);
    MDrv_Write2Byte(0x101228, 0x4090);

    MDrv_Write2Byte(0x10121e, 0x0c01);
    MDrv_Write2Byte(0x10121e, 0x0c00);
    MDrv_Write2Byte(0x10110E, 0x00A5);
    MDrv_Write2Byte(0x10110E, 0x00E5);
    MDrv_Write2Byte(0x10110E, 0x00A5);

    MDrv_Write2Byte(0x10118E, 0x00A5);
    MDrv_Write2Byte(0x10118E, 0x00E5);
    MDrv_Write2Byte(0x10118E, 0x00A5);

    MDrv_Write2Byte(0x101200, 0x0000);
    MDrv_Write2Byte(0x101200, 0x0008);
    MDrv_Write2Byte(0x101200, 0x000c);
    //wait 1    // delay 1ms
    MDrv_SysDelayUs(1000);
    MDrv_Write2Byte(0x101200, 0x000e);
    //wait 1    // delay 1ms
    MDrv_SysDelayUs(1000);
    MDrv_Write2Byte(0x101200, 0x001f);

    MDrv_SysDelayUs(10000);


    MDrv_Write2Byte(0x101246, 0xfffe);
    MDrv_Write2Byte(0x101266, 0xffff);
    MDrv_Write2Byte(0x101286, 0x7fff);
    MDrv_Write2Byte(0x1012a6, 0xffff);


    MDrv_Write2Byte(0x10121e, 0x1a08);

//    MDrv_Write2Byte(0x10123c  0x0080//turn on adc reg_adc_test_en  ==>comment out by RD
}

void MiuInit_1333Mhz_ESMT_QFP(void);  //2133_2133
void MiuInit_1333Mhz_ESMT_QFP(void)  //2133_2133
{
//-------------------------------------------------------------------
// initialize MIU
//-------------------------------------------------------------------
// =====MIU0===
    MDrv_Write2Byte(0x101220,  0x0000);
    MDrv_Write2Byte(0x10121e,  0x0c00);
    MDrv_Write2Byte(0x10121e,  0x0c00);
    MDrv_Write2Byte(0x10121e,  0x0c00);
    MDrv_Write2Byte(0x10121e,  0x0c01);
    MDrv_Write2Byte(0x10121e,  0x0c00);
    MDrv_Write2Byte(0x101246,  0xfffe);
    MDrv_Write2Byte(0x101266,  0xffff);
    MDrv_Write2Byte(0x101286,  0xffff);
    MDrv_Write2Byte(0x1012a6,  0xffff);

    MDrv_Write2Byte(0x101124,  0x1000);
    MDrv_Write2Byte(0x1011a4,  0x1000);
    MDrv_Write2Byte(0x101124,  0x2000);
    MDrv_Write2Byte(0x1011a4,  0x2000);
    MDrv_Write2Byte(0x101178,  0x0102);
    MDrv_Write2Byte(0x1011f8,  0x0102);

    //-----------------------
    //set DDRPLL0 1333MHz
    //-----------------------
    MDrv_Write2Byte(0x101136,  0x4000);
    MDrv_Write2Byte(0x101134,  0x0000);
    MDrv_Write2Byte(0x101130,  0x8000);
#if (MIU_SPEED == DDR2_1333MHz_e)
    MDrv_Write2Byte(0x101132,  0x0029);
#else   // 1240
    MDrv_Write2Byte(0x101132,  0x002C);
#endif
    MDrv_Write2Byte(0x101120,  0x0020);
    //----------------------  -
    //set DDRPLL0 1333MHz
    //-----------------------
    MDrv_Write2Byte(0x1011b6,  0x4000);
    MDrv_Write2Byte(0x1011b4,  0x0000);
    MDrv_Write2Byte(0x1011b0,  0x8000);
#if (MIU_SPEED == DDR2_1333MHz_e)
    MDrv_Write2Byte(0x1011b2,  0x0029);
#else   // 1240
    MDrv_Write2Byte(0x1011b2,  0x002C);
#endif
    MDrv_Write2Byte(0x1011a0,  0x0020);

    MDrv_SysDelayUs(1000);
    //MDrv_Write2Byte(0x101202,  (0xf392|DRAM_BUS_MASK));
    MDrv_Write2Byte(0x101204,  0x000a);
    MDrv_Write2Byte(0x101206,  0x0540);
    MDrv_Write2Byte(0x101208,  0x1e99);
    MDrv_Write2Byte(0x10120a,  0x2767);
    MDrv_Write2Byte(0x10120c,  0x55a6);
    MDrv_Write2Byte(0x10120e,  0x2057);
    MDrv_Write2Byte(0x101210,  0x0e73);
    MDrv_Write2Byte(0x101212,  0x4004);
    MDrv_Write2Byte(0x101214,  0x8000);
    MDrv_Write2Byte(0x101216,  0xc000);

    MDrv_Write2Byte(0x101102,  0xaaaa);
    MDrv_Write2Byte(0x101104,  0x0080);
    MDrv_Write2Byte(0x10113c,  0x1600);
    MDrv_Write2Byte(0x10113e,  0x0166);
    MDrv_Write2Byte(0x101150,  0x2222);
    MDrv_Write2Byte(0x101138,  0x0066);
    MDrv_Write2Byte(0x10113a,  0x3030);

    MDrv_Write2Byte(0x10110a,  0x2244);
    MDrv_Write2Byte(0x101152,  0x0000);
    MDrv_Write2Byte(0x10112e,  0x2200);

    MDrv_Write2Byte(0x10110e,  0x00a5);
    MDrv_Write2Byte(0x10110e,  0x00e5); //dqsm_rst
    MDrv_Write2Byte(0x10110e,  0x00a5);

    MDrv_Write2Byte(0x101168,  0x2020);
    MDrv_Write2Byte(0x10116a,  0x2020);
    MDrv_Write2Byte(0x10116e,  0x6666);
    MDrv_Write2Byte(0x10116c,  0x0000);
    MDrv_Write2Byte(0x101118,  0x0000);
    MDrv_Write2Byte(0x101158,  0x0707);
    MDrv_Write2Byte(0x10115a,  0x4444);
    MDrv_Write2Byte(0x10115c,  0x4444);
    MDrv_Write2Byte(0x10115e,  0x4444);
    MDrv_Write2Byte(0x101174,  0x8882);

    MDrv_Write2Byte(0x101182,  0xaaaa);
    MDrv_Write2Byte(0x101184,  0x0080);
    MDrv_Write2Byte(0x1011bc,  0x1600);
    MDrv_Write2Byte(0x1011be,  0x0166);
    MDrv_Write2Byte(0x1011d0,  0x2222);
    MDrv_Write2Byte(0x1011b8,  0x0066);
    MDrv_Write2Byte(0x1011ba,  0x3030);
    MDrv_Write2Byte(0x10118a,  0x2244);
    MDrv_Write2Byte(0x1011d2,  0x0000);
    MDrv_Write2Byte(0x1011ae,  0x2200);

    MDrv_Write2Byte(0x10118e,  0x00a5);
    MDrv_Write2Byte(0x10118e,  0x00e5); //dqsm_rst
    MDrv_Write2Byte(0x10118e,  0x00a5);

    MDrv_Write2Byte(0x1011e8,  0x2020);
    MDrv_Write2Byte(0x1011ea,  0x2020);
    MDrv_Write2Byte(0x1011ee,  0x6666);
    MDrv_Write2Byte(0x1011ec,  0x0000);
    MDrv_Write2Byte(0x101198,  0x0000);
    MDrv_Write2Byte(0x1011d8,  0x0707);
    MDrv_Write2Byte(0x1011da,  0x4444);
    MDrv_Write2Byte(0x1011dc,  0x4444);
    MDrv_Write2Byte(0x1011de,  0x4444);
    MDrv_Write2Byte(0x1011f4,  0x8882);

    MDrv_Write2Byte(0x10121e,  0x0c01);
    MDrv_Write2Byte(0x10121e,  0x0c00);
    MDrv_Write2Byte(0x10117c,  0xfc00);
    MDrv_Write2Byte(0x1011fc,  0xfc00);

    MDrv_Write2Byte(0x101160,  0x000c); //dll_pd
    MDrv_Write2Byte(0x101160,  0x0008);
    MDrv_Write2Byte(0x101160,  0x2370);
    MDrv_Write2Byte(0x1011e0,  0x000c); //dll_pd
    MDrv_Write2Byte(0x1011e0,  0x0008);
    MDrv_Write2Byte(0x1011e0,  0x2370);


    MDrv_Write2Byte(0x101240,  0x8001);
    MDrv_Write2Byte(0x101260,  0x8001);
    MDrv_Write2Byte(0x101280,  0x8001);
    MDrv_Write2Byte(0x1012a0,  0x8001);
    MDrv_Write2Byte(0x101110,  0x0200);
    MDrv_Write2Byte(0x101190,  0x0200);
    MDrv_Write2Byte(0x10121e,  0x1a00);
    MDrv_Write2Byte(0x1012f0,  0x0000);
    MDrv_Write2Byte(0x1012d2,  0x0000);
    MDrv_Write2Byte(0x101228,  0x4090); // pack_en = 1'b1

    MDrv_Write2Byte(0x10121e,  0x0c01);
    MDrv_Write2Byte(0x10121e,  0x0c00);
    MDrv_Write2Byte(0x10110e,  0x00a5);
    MDrv_Write2Byte(0x10110e,  0x00e5); //dqsm_rst
    MDrv_Write2Byte(0x10110e,  0x00a5);
    MDrv_Write2Byte(0x10118e,  0x00a5);
    MDrv_Write2Byte(0x10118e,  0x00e5); //dqsm_rst
    MDrv_Write2Byte(0x10118e,  0x00a5);

    MDrv_Write2Byte(0x101200,  0x0000);
    MDrv_Write2Byte(0x101200,  0x0008);
    MDrv_Write2Byte(0x101200,  0x000c);
    //==================================================================
    //--------Wait 10us, Reset Dram(200us in real case)
    //==================================================================
    MDrv_SysDelayUs(1000); //delay 1ms
    MDrv_Write2Byte(0x101200,  0x000e);
    //======================================================================
    //--------Wait 500ns, dram initernal state initialization(real in 500us)
    //======================================================================
    MDrv_SysDelayUs(1000);
    //--------Initial DRAM start here!!!
    MDrv_Write2Byte(0x101200,  0x001f);
    MDrv_SysDelayUs(10000); //check reg_101200 #15 for wait initial done

    MDrv_Write2Byte(0x101246,  0xfffe);
    MDrv_Write2Byte(0x101266,  0xffff);
    MDrv_Write2Byte(0x101286,  0x7fff);
    MDrv_Write2Byte(0x1012a6,  0xffff);

    MDrv_Write2Byte(0x10121e,  0x1a08);

//    MDrv_Write2Byte(0x10123c  0x0080//turn on adc reg_adc_test_en  ==>comment out by RD
}
#endif

#if (CHIP_ID==MST9U2)
#if (MIU_SPEED == DDR3_1866MHz_e)
void MiuInit2_1866Mhz(U8 u8STRInit);
void MiuInit2_1866Mhz(U8 u8STRInit)
{
//================================================
// Start MIU init !!!
//================================================
// =====MIU0===
    MDrv_Write2Byte(0x101220, 0x0000); //set pll power done
// =====MIU1===
    MDrv_Write2Byte(0x100620, 0x0000); //set pll power done
// =====MIU0===
    MDrv_Write2Byte(0x10121e, 0x0c00); //dummy register setting
    MDrv_Write2Byte(0x10121e, 0x0c00); //dummy register setting
    MDrv_Write2Byte(0x10121e, 0x0c00); //dummy register setting
    MDrv_Write2Byte(0x10121e, 0x0c01); //miu software reset
    MDrv_Write2Byte(0x10121e, 0x0c00);
    MDrv_Write2Byte(0x101202, 0x0000);
    MDrv_Write2Byte(0x101246, 0xfffe); //mask other request
    MDrv_Write2Byte(0x101266, 0xffff); //mask other request
    MDrv_Write2Byte(0x101286, 0xffff); //mask other request
    MDrv_Write2Byte(0x1012a6, 0xffff); //mask other request
// =====MIU1===
    MDrv_Write2Byte(0x10061e, 0x0c00); //dummy register setting
    MDrv_Write2Byte(0x10061e, 0x0c00); //dummy register setting
    MDrv_Write2Byte(0x10061e, 0x0c00); //dummy register setting
    MDrv_Write2Byte(0x10061e, 0x0c01); //miu software reset
    MDrv_Write2Byte(0x10061e, 0x0e00);
    MDrv_Write2Byte(0x100602, 0x0000);
    MDrv_Write2Byte(0x100646, 0xfffe); //mask other request
    MDrv_Write2Byte(0x100666, 0xffff); //mask other request
    MDrv_Write2Byte(0x100686, 0xffff); //mask other request
    MDrv_Write2Byte(0x1006a6, 0xffff); //mask other request
//--------------------
//set DDRPLL0 1866MHz
//--------------------
    MDrv_Write2Byte(0x101136, MIU_0_CLK);
    MDrv_Write2Byte(0x101134, 0x0400);
    MDrv_Write2Byte(0x101130, 0xeec9); //1% decrease for SSC
    MDrv_Write2Byte(0x101132, 0x001d);
    MDrv_Write2Byte(0x101120, 0x0020);
//--------------------
//set DDRPLL1 1866MHz
//--------------------
    MDrv_Write2Byte(0x1011b6, MIU_1_CLK);
    MDrv_Write2Byte(0x1011b4, 0x0400);
    MDrv_Write2Byte(0x1011b0, 0xeec9); //1% decrease for SSC
    MDrv_Write2Byte(0x1011b2, 0x001d);
    MDrv_Write2Byte(0x1011a0, 0x0020);
//Avoid DDRPLL unknown(MCLK05X & MCLK025X) by resset ddrat[28]
//wait 1    // delay 1ms
    MDrv_SysDelayUs(1000);
    MDrv_Write2Byte(0x101124, 0x1000);
    MDrv_Write2Byte(0x1011a4, 0x1000);
//wait 1    // delay 1ms
    MDrv_SysDelayUs(1000);
    MDrv_Write2Byte(0x101124, 0x0000);
    MDrv_Write2Byte(0x1011a4, 0x0000);
//wait 1    // delay 1ms
    MDrv_SysDelayUs(1000);
//For improve MCLK jitter
//This setting will be included in bringup script
    if(u8STRInit)
    {
        MDrv_Write2Byte(0x10117e, 0x00fc); //[    2] 1 reg_ddr_test
        MDrv_Write2Byte(0x1011fe, 0x0004); //[    2] 1 reg_ddr_test
    }
    else
    {
        MDrv_Write2Byte(0x10117e, 0x0004); //[    2] 1 reg_ddr_test
        MDrv_Write2Byte(0x1011fe, 0x0004); //[    2] 1 reg_ddr_test
    }
//------------------
// Wait PLL Stable
//------------------
//wait 10
    MDrv_SysDelayUs(10000);
//---------------------------
//set DDR3-16-8X-CL13_1866
//---------------------------
    MDrv_Write2Byte(0x101202, (0xf3a3|DRAM_BUS_MASK));
    MDrv_Write2Byte(0x101204, 0x004b);
    MDrv_Write2Byte(0x101206, 0x1538); //[12:12] reg_cke_always_on
    MDrv_Write2Byte(0x101208, 0x20dd);
    MDrv_Write2Byte(0x10120a, 0x2e76);
    MDrv_Write2Byte(0x10120c, 0xa7e9);
    MDrv_Write2Byte(0x10120e, 0x4096);
    MDrv_Write2Byte(0x101210, 0x1f14);
    MDrv_Write2Byte(0x101212, 0x4004);
    MDrv_Write2Byte(0x101214, 0x8020);
    MDrv_Write2Byte(0x101216, 0xc000);
    MDrv_Write2Byte(0x10117c, 0x0000);
    if(!u8STRInit)
    {
        MDrv_Write2Byte(0x101100, 0x0008);
    }
    MDrv_Write2Byte(0x101104, 0x0080);
    MDrv_Write2Byte(0x101138, MIU_0_DQ_DQS_L);
    MDrv_Write2Byte(0x10113a, MIU_0_RDQSM_CMD_L);
    MDrv_Write2Byte(0x10114a, MIU_0_RDQSM_CMD_H);
    MDrv_Write2Byte(0x10113c, MIU_0_DQS_CS_SKEW);
    MDrv_Write2Byte(0x101148, MIU_0_DQ_DQS_H);
    MDrv_Write2Byte(0x10114c, MIU_0_RDQSM_SKEW_H);
    MDrv_Write2Byte(0x101152, MIU_0_CS);
    MDrv_Write2Byte(0x101102, 0x0000);
    MDrv_Write2Byte(0x10113e, MIU_0_DQS_CS_CLK_SKEW);
    MDrv_Write2Byte(0x101150, MIU_0_CSZ_SKEW);
    MDrv_Write2Byte(0x10110e, 0x00e9);
    MDrv_Write2Byte(0x10110e, 0x00a9);
    MDrv_Write2Byte(0x10110e, 0x00e9);
    MDrv_Write2Byte(0x10110e, 0x00a9);
    MDrv_Write2Byte(0x10116e, MIU_0_RDQS_H);
    MDrv_Write2Byte(0x10116c, MIU_0_RDQS_L);
    MDrv_Write2Byte(0x10116a, 0x0020);
    MDrv_Write2Byte(0x101168, 0x0020);

    if(u8STRInit)
    {
        MDrv_Write2Byte(0x101154, 0xc070);
        MDrv_Write2Byte(0x10121e, 0x0c01);
        MDrv_Write2Byte(0x10121e, 0x0c00);
        MDrv_Write2ByteMask(0x101100, MIU_0_ODT, BIT12|BIT11|BIT10|BIT9|BIT5); //[ 5: 5] reg_mclk_pd(this bit no use in MST9U1/2)
        MDrv_Write2Byte(0x101102, 0xaaaa);
        MDrv_Write2Byte(0x101202, (0xf3a3|DRAM_BUS_MASK));
        MDrv_Write2Byte(0x10122a, 0x00c0);
        MDrv_Write2Byte(0x101202, (0xf3a3|DRAM_BUS_MASK));
    }
    else
    {
        MDrv_Write2Byte(0x101154, 0xc000);
        MDrv_Write2Byte(0x10121e, 0x0c01);
        MDrv_Write2Byte(0x10121e, 0x0c00);
        MDrv_Write2Byte(0x101100, 0x0000|MIU_0_ODT);
        MDrv_Write2Byte(0x101102, 0xaaaa);
        MDrv_Write2Byte(0x101202, (0x03a3|DRAM_BUS_MASK));
        MDrv_Write2Byte(0x10122a, 0x00c0);
        MDrv_Write2Byte(0x101202, (0x03a3|DRAM_BUS_MASK));
    }
//---------------------------
//set DDR3-16-8X-CL13_1866
//---------------------------
    MDrv_Write2Byte(0x100602, 0xf3a7);
    MDrv_Write2Byte(0x100604, 0x004b);
    MDrv_Write2Byte(0x100606, 0x1538); //[12:12] reg_cke_always_on
    MDrv_Write2Byte(0x100608, 0x20dd);
    MDrv_Write2Byte(0x10060a, 0x2e76);
    MDrv_Write2Byte(0x10060c, 0xa7e9);
    MDrv_Write2Byte(0x10060e, 0x4096);
    MDrv_Write2Byte(0x100610, 0x1f14);
    MDrv_Write2Byte(0x100612, 0x4004);
    MDrv_Write2Byte(0x100614, 0x8020);
    MDrv_Write2Byte(0x100616, 0xc000);
    MDrv_Write2Byte(0x1011fc, 0x0000);
    MDrv_Write2Byte(0x101180, 0x0010);
    MDrv_Write2Byte(0x101184, 0x0080);
    MDrv_Write2Byte(0x1011b8, MIU_1_DQ_DQS_L);
    MDrv_Write2Byte(0x1011c8, MIU_1_DQ_DQS_H);
    MDrv_Write2Byte(0x1011ba, MIU_1_RDQSM_CMD_L);
    MDrv_Write2Byte(0x1011ca, MIU_1_RDQSM_CMD_H);
    MDrv_Write2Byte(0x1011bc, MIU_1_DQS_CS_SKEW);
    MDrv_Write2Byte(0x1011cc, MIU_1_RDQSM_SKEW_H);
    MDrv_Write2Byte(0x1011d2, MIU_1_CS);
    MDrv_Write2Byte(0x101182, 0x0000);
    MDrv_Write2Byte(0x1011be, MIU_1_DQS_CS_CLK_SKEW);
    MDrv_Write2Byte(0x1011d0, MIU_1_CSZ_SKEW);
    MDrv_Write2Byte(0x10118e, 0x00e9);
    MDrv_Write2Byte(0x10118e, 0x00a9);
    MDrv_Write2Byte(0x10118e, 0x00e9);
    MDrv_Write2Byte(0x10118e, 0x00a9);
    MDrv_Write2Byte(0x1011ee, MIU_1_RDQS_H);
    MDrv_Write2Byte(0x1011ec, MIU_1_RDQS_L);
    MDrv_Write2Byte(0x1011ea, 0x0020);
    MDrv_Write2Byte(0x1011e8, 0x0020);
    MDrv_Write2Byte(0x1011d4, 0xc000);
    MDrv_Write2Byte(0x10061e, 0x0c01);
    MDrv_Write2Byte(0x10061e, 0x0c00);
    MDrv_Write2Byte(0x101180, 0x0000|MIU_1_ODT);
    MDrv_Write2Byte(0x101182, 0xaaaa);
    MDrv_Write2Byte(0x100602, 0x03a7);
    MDrv_Write2Byte(0x10062a, 0x00c0);
    MDrv_Write2Byte(0x100602, 0x03a7);

    MDrv_Write2Byte(0x101174, MIU_0_TRIG_LVL); //trig lvl
    MDrv_Write2Byte(0x10115a, MIU_0_DRV_NP); //drvp
    MDrv_Write2Byte(0x10115c, MIU_0_DRV_N); //drvn
    MDrv_Write2Byte(0x10115e, MIU_0_DRV_P); //drvp
    MDrv_Write2Byte(0x1011f4, MIU_1_TRIG_LVL); //trig lvl
    MDrv_Write2Byte(0x1011da, MIU_1_DRV_NP); //drvp
    MDrv_Write2Byte(0x1011dc, MIU_1_DRV_N); //drvn
    MDrv_Write2Byte(0x1011de, MIU_1_DRV_P); //drvp

//-------------
// BW setting
//-------------
    MDrv_Write2Byte(0x101240, 0x8015);
    MDrv_Write2Byte(0x101260, 0x8015);
    MDrv_Write2Byte(0x101280, 0x8011);
    MDrv_Write2Byte(0x1012a0, 0x8011);
    MDrv_Write2Byte(0x100640, 0x8015);
    MDrv_Write2Byte(0x100660, 0x8015);
    MDrv_Write2Byte(0x100680, 0x8011);
    MDrv_Write2Byte(0x1006a0, 0x8011);
//---------------------------------
// Pack setting
//---------------------------------
    MDrv_Write2Byte(0x101228, 0x4090); //[14:14] reg_pack_en
    MDrv_Write2Byte(0x100628, 0x4090); //[14:14] reg_pack_en
//-------------------------
// Reduce latency setting
//-------------------------
    MDrv_Write2Byte(0x1012d4, 0x0000);
    MDrv_Write2Byte(0x1006d4, 0x0000);
//------------------------------
// sync in fifo stage
//------------------------------
    MDrv_Write2Byte(0x1012f8, 0x0600);
    MDrv_Write2Byte(0x1012fa, 0x0420);
    MDrv_Write2Byte(0x1012fc, 0x4600);
    MDrv_Write2Byte(0x1012fe, 0x1087);
    MDrv_Write2Byte(0x1006f8, 0x0600);
    MDrv_Write2Byte(0x1006fa, 0x0420);
    MDrv_Write2Byte(0x1006fc, 0x4600);
    MDrv_Write2Byte(0x1006fe, 0x1087);

    MDrv_Write2Byte(0x10121e, 0x1a00); //[ 0: 0] reg_sw_rst_miu
                           //[ 3: 3] reg_sw_init_done
                           //[12: 8] reg_sync_out_threshold
    MDrv_Write2Byte(0x10061e, 0x1a00); //[ 0: 0] reg_sw_rst_miu
                           //[ 3: 3] reg_sw_init_done
                           //[12: 8] reg_sync_out_threshold

    MDrv_Write2Byte(0x10121e, 0x1a01);
    MDrv_Write2Byte(0x10121e, 0x1a00);
    MDrv_Write2Byte(0x10061e, 0x1a01);
    MDrv_Write2Byte(0x10061e, 0x1a00);

//$display($time," start to program DLL0 ");
    MDrv_Write2Byte(0x101160, 0x000c);
    MDrv_Write2Byte(0x101160, 0x0008);
    MDrv_Write2Byte(0x101162, 0x007f);
    MDrv_Write2Byte(0x101164, 0xf200);
    MDrv_Write2Byte(0x101160, 0x2370);
//$display($time," start to program DLL1 ");
    MDrv_Write2Byte(0x1011e0, 0x000c);
    MDrv_Write2Byte(0x1011e0, 0x0008);
    MDrv_Write2Byte(0x1011e2, 0x007f);
    MDrv_Write2Byte(0x1011e4, 0xf200);
    MDrv_Write2Byte(0x1011e0, 0x2370);

    if(u8STRInit)
    {
        MDrv_Write2Byte(0x101108, 0x0000);
    }
    else
    {
        MDrv_Write2Byte(0x101108, 0x003f);
    }
    MDrv_Write2Byte(0x101200, 0x0000);
    MDrv_Write2Byte(0x101188, 0x003f);
    MDrv_Write2Byte(0x100600, 0x0000);
//-----Wait 200us, (keep DRAM RESET = 0)-----
//wait 1
    MDrv_SysDelayUs(1000);
    MDrv_Write2Byte(0x101200, 0x0008);
    MDrv_Write2Byte(0x101200, 0x000c);
    MDrv_Write2Byte(0x100600, 0x0008);
    MDrv_Write2Byte(0x100600, 0x000c);
//-----Wait 500us, (keep DRAM RESET = 1 , CKE = 0)-----
//wait 1
    MDrv_SysDelayUs(1000);
    MDrv_Write2Byte(0x101200, 0x000e);
    MDrv_Write2Byte(0x100600, 0x000e);
//-----Wait tXPR=400ns-----
//wait 1
    MDrv_SysDelayUs(1000);
//--------Initial DRAM start here!!!
    MDrv_Write2Byte(0x101200, 0x001f);
#if (DRAM_BUS_WIDTH == DRAM_BUS_64B)
    MDrv_Write2Byte(0x100600, 0x001f);
#endif
//-----Wait init done-----
//wait 100
    MDrv_SysDelayUs(100000);
//--------Initial Done
//--------DDR2 wait 400ns for tXPR(?), DDR3 wait 512T for tZQinit
//wait 1
    MDrv_SysDelayUs(1000);

//------------------------------
// unmask all
//------------------------------
    MDrv_Write2Byte(0x10121a, 0x0001);
    MDrv_Write2Byte(0x101246, 0x0000);
    MDrv_Write2Byte(0x101266, 0x0000);
    MDrv_Write2Byte(0x101286, 0x0000);
    MDrv_Write2Byte(0x1012a6, 0x0000);
    MDrv_Write2Byte(0x10121e, 0x1a08); //[ 0: 0] reg_sw_rst_miu
                           //[ 3: 3] reg_sw_init_done
                           //[12: 8] reg_sync_out_threshold
    MDrv_Write2Byte(0x10061a, 0x0001);
    MDrv_Write2Byte(0x100646, 0x0000);
    MDrv_Write2Byte(0x100666, 0x0000);
    MDrv_Write2Byte(0x100686, 0x0000);
    MDrv_Write2Byte(0x1006a6, 0x0000);
    MDrv_Write2Byte(0x10061e, 0x1a08); //[ 0: 0] reg_sw_rst_miu
                           //[ 3: 3] reg_sw_init_done
                           //[12: 8] reg_sync_out_threshold

    MDrv_Write2Byte(0x10110e, 0x00a9);
    MDrv_Write2Byte(0x10110e, 0x00e9);
    MDrv_Write2Byte(0x10110e, 0x00a9);

    MDrv_Write2Byte(0x10118e, 0x00a9);
    MDrv_Write2Byte(0x10118e, 0x00e9);
    MDrv_Write2Byte(0x10118e, 0x00a9);
//=================================================================
// End of MIU init !!!
//=================================================================
#if 0
    MDrv_Write2Byte(0x1012f0, 0x0000); //sel MIU0
//-------------------------------
//miu self test : mode 0007
//-------------------------------
    MDrv_Write2Byte(0x1012e0, 0x0000);
    MDrv_Write2Byte(0x1012e2, 0x0080);
    MDrv_Write2Byte(0x1012e4, 0x0021);
    MDrv_Write2Byte(0x1012e6, 0x0000);
    MDrv_Write2Byte(0x1012e8, 0x5aa5);
    MDrv_Write2Byte(0x1012d8, 0x0000);
    MDrv_Write2Byte(0x1012e0, 0x0006);
    MDrv_Write2Byte(0x1012e0, 0x0007);
//-----------------------------
//Wait for MIU self test result
//-----------------------------
    MDrv_Write2Byte(0x1012e0, 0x0000);
    MDrv_Write2Byte(0x1012f0, 0x8000); //sel MIU1
//-------------------------------
//miu self test : mode 0007
//-------------------------------
    MDrv_Write2Byte(0x1012e0, 0x0000);
    MDrv_Write2Byte(0x1012e2, 0x0080);
    MDrv_Write2Byte(0x1012e4, 0x0021);
    MDrv_Write2Byte(0x1012e6, 0x0000);
    MDrv_Write2Byte(0x1012e8, 0x5aa5);
    MDrv_Write2Byte(0x1012d8, 0x0000);
    MDrv_Write2Byte(0x1012e0, 0x0006);
    MDrv_Write2Byte(0x1012e0, 0x0007);
//-----------------------------
//Wait for MIU self test result
//-----------------------------
//    MDrv_Write2Byte(0x1012e0, 0x0000);
//    MDrv_Write2Byte(0x10123c, 0x0040); //[    7] 1 turn on reg_adc_test_en
//    MDrv_Write2Byte(0x1012fe, 0x0887); //[   11] 1 turn off miu engine, reg_miu_test_off
//    MDrv_Write2Byte(0x1006f0, 0x7ffe); //miu_sel --> hw sel
//    MDrv_Write2Byte(0x1006f2, 0xffff); //miu_sel --> hw sel
//    MDrv_Write2Byte(0x1006f4, 0xffff); //miu_sel --> hw sel
//    MDrv_Write2Byte(0x1006f6, 0xffff); //miu_sel --> hw sel
#endif
}
#elif (MIU_SPEED == DDR3_2133MHz_e)
void MiuInit2_2133Mhz(U8 u8STRInit);
void MiuInit2_2133Mhz(U8 u8STRInit)
{

//================================================
// Start MIU init !!!
//================================================
// =====MIU0===
    MDrv_Write2Byte(0x101220, 0x0000); //set pll power done
// =====MIU1===
    MDrv_Write2Byte(0x100620, 0x0000); //set pll power done
// =====MIU0===
    MDrv_Write2Byte(0x10121e, 0x0c00); //dummy register setting
    MDrv_Write2Byte(0x10121e, 0x0c00); //dummy register setting
    MDrv_Write2Byte(0x10121e, 0x0c00); //dummy register setting
    MDrv_Write2Byte(0x10121e, 0x0c01); //miu software reset
    MDrv_Write2Byte(0x10121e, 0x0c00);
    MDrv_Write2Byte(0x101202, 0x0000);
    MDrv_Write2Byte(0x101246, 0xfffe); //mask other request
    MDrv_Write2Byte(0x101266, 0xffff); //mask other request
    MDrv_Write2Byte(0x101286, 0xffff); //mask other request
    MDrv_Write2Byte(0x1012a6, 0xffff); //mask other request
// =====MIU1===
    MDrv_Write2Byte(0x10061e, 0x0c00); //dummy register setting
    MDrv_Write2Byte(0x10061e, 0x0c00); //dummy register setting
    MDrv_Write2Byte(0x10061e, 0x0c00); //dummy register setting
    MDrv_Write2Byte(0x10061e, 0x0c01); //miu software reset
    MDrv_Write2Byte(0x10061e, 0x0e00);
    MDrv_Write2Byte(0x100602, 0x0000);
    MDrv_Write2Byte(0x100646, 0xfffe); //mask other request
    MDrv_Write2Byte(0x100666, 0xffff); //mask other request
    MDrv_Write2Byte(0x100686, 0xffff); //mask other request
    MDrv_Write2Byte(0x1006a6, 0xffff); //mask other request
//--------------------
//set DDRPLL0 2133MHz
//--------------------
    MDrv_Write2Byte(0x101136, MIU_0_CLK);
    MDrv_Write2Byte(0x101134, 0x0400);
    MDrv_Write2Byte(0x101130, 0x2f97);
    MDrv_Write2Byte(0x101132, 0x001a); //1% decrease for SSC
    MDrv_Write2Byte(0x101120, 0x0020);
//--------------------
//set DDRPLL1 2133MHz
//--------------------
    MDrv_Write2Byte(0x1011b6, MIU_1_CLK);
    MDrv_Write2Byte(0x1011b4, 0x0400);
    MDrv_Write2Byte(0x1011b0, 0x2f97);
    MDrv_Write2Byte(0x1011b2, 0x001a); //1% decrease for SSC
    MDrv_Write2Byte(0x1011a0, 0x0020);
//Avoid DDRPLL unknown(MCLK05X & MCLK025X) by resset ddrat[28]
//wait 1    // delay 1ms
    MDrv_SysDelayUs(1000);
    MDrv_Write2Byte(0x101124, 0x1000);
    MDrv_Write2Byte(0x1011a4, 0x1000);
//wait 1
    MDrv_SysDelayUs(1000);
    MDrv_Write2Byte(0x101124, 0x0000);
    MDrv_Write2Byte(0x1011a4, 0x0000);
//wait 1
    MDrv_SysDelayUs(1000);
//For improve MCLK jitter
//This setting will be included in bringup script
    if(u8STRInit)
    {
        MDrv_Write2Byte(0x10117e, 0x00fc); //[    2] 1 reg_ddr_test
        MDrv_Write2Byte(0x1011fe, 0x0004); //[    2] 1 reg_ddr_test
    }
    else
    {
        MDrv_Write2Byte(0x10117e, 0x0004); //[    2] 1 reg_ddr_test
        MDrv_Write2Byte(0x1011fe, 0x0004); //[    2] 1 reg_ddr_test
    }
//------------------
// Wait PLL Stable
//------------------
//wait 10
    MDrv_SysDelayUs(10000);
//---------------------------
//set DDR3-16-8X-CL14_2133
//---------------------------
    MDrv_Write2Byte(0x101202, (0xf3a3|DRAM_BUS_MASK));
    MDrv_Write2Byte(0x101204, 0x004c);
    MDrv_Write2Byte(0x101206, 0x1640); //[12:12] reg_cke_always_on
    MDrv_Write2Byte(0x101208, 0x24ee);
    MDrv_Write2Byte(0x10120a, 0x3487);
    MDrv_Write2Byte(0x10120c, 0xc80a);
    MDrv_Write2Byte(0x10120e, 0xc0b7);
    MDrv_Write2Byte(0x101210, 0x1124);
    MDrv_Write2Byte(0x101212, 0x4004);
    MDrv_Write2Byte(0x101214, 0x8028);
    MDrv_Write2Byte(0x101216, 0xc000);
    MDrv_Write2Byte(0x10117c, 0x0000);
    if(!u8STRInit)
    {
        MDrv_Write2Byte(0x101100, 0x0008);
    }
    MDrv_Write2Byte(0x101104, 0x0080);
    MDrv_Write2Byte(0x101138, MIU_0_DQ_DQS_L);
    MDrv_Write2Byte(0x101148, MIU_0_DQ_DQS_H);
    MDrv_Write2Byte(0x10113a, MIU_0_RDQSM_CMD_L);
    MDrv_Write2Byte(0x10114a, MIU_0_RDQSM_CMD_H);
    MDrv_Write2Byte(0x101152, MIU_0_CS);
    MDrv_Write2Byte(0x101102, 0x0000);
    MDrv_Write2Byte(0x10113c, MIU_0_DQS_CS_SKEW);
    MDrv_Write2Byte(0x10114c, MIU_0_RDQSM_SKEW_H);
    MDrv_Write2Byte(0x10113e, MIU_0_DQS_CS_CLK_SKEW);
    MDrv_Write2Byte(0x101150, MIU_0_CSZ_SKEW);
    MDrv_Write2Byte(0x10110e, 0x00eb);
    MDrv_Write2Byte(0x10110e, 0x00ab);
    MDrv_Write2Byte(0x10110e, 0x00eb);
    MDrv_Write2Byte(0x10110e, 0x00ab);
    MDrv_Write2Byte(0x10116e, MIU_0_RDQS_H);
    MDrv_Write2Byte(0x10116c, MIU_0_RDQS_L);
    MDrv_Write2Byte(0x10116a, 0x0020);
    MDrv_Write2Byte(0x101168, 0x0020);

    if(u8STRInit)
    {
        MDrv_Write2Byte(0x101154, 0xc070);
        MDrv_Write2Byte(0x10121e, 0x0c01);
        MDrv_Write2Byte(0x10121e, 0x0c00);
        MDrv_Write2ByteMask(0x101100, MIU_0_ODT, BIT12|BIT11|BIT10|BIT9|BIT5); //[ 5: 5] reg_mclk_pd(this bit no use in MST9U1/2)
        MDrv_Write2Byte(0x101102, 0xaaaa);
        MDrv_Write2Byte(0x101202, (0xf3a3|DRAM_BUS_MASK));
        MDrv_Write2Byte(0x10122a, 0x00c0);
        MDrv_Write2Byte(0x101202, (0xf3a3|DRAM_BUS_MASK));
    }
    else
    {
        MDrv_Write2Byte(0x101154, 0xc000);
        MDrv_Write2Byte(0x10121e, 0x0c01);
        MDrv_Write2Byte(0x10121e, 0x0c00);
        MDrv_Write2Byte(0x101100, 0x0000|MIU_0_ODT);
        MDrv_Write2Byte(0x101102, 0xaaaa);
        MDrv_Write2Byte(0x101202, (0x03a3|DRAM_BUS_MASK));
        MDrv_Write2Byte(0x10122a, 0x00c0);
        MDrv_Write2Byte(0x101202, (0x03a3|DRAM_BUS_MASK));
    }
//---------------------------
//set DDR3-16-8X-CL14_2133
//---------------------------
    MDrv_Write2Byte(0x100602, 0xf3a7);
    MDrv_Write2Byte(0x100604, 0x004c);
    MDrv_Write2Byte(0x100606, 0x1640); //[12:12] reg_cke_always_on
    MDrv_Write2Byte(0x100608, 0x24ee);
    MDrv_Write2Byte(0x10060a, 0x3487);
    MDrv_Write2Byte(0x10060c, 0xc80a);
    MDrv_Write2Byte(0x10060e, 0xc0b7);
    MDrv_Write2Byte(0x100610, 0x1124);
    MDrv_Write2Byte(0x100612, 0x4004);
    MDrv_Write2Byte(0x100614, 0x8028);
    MDrv_Write2Byte(0x100616, 0xc000);
    MDrv_Write2Byte(0x1011fc, 0x0000);
    MDrv_Write2Byte(0x101180, 0x0010);
    MDrv_Write2Byte(0x101184, 0x0080);
    MDrv_Write2Byte(0x1011b8, MIU_1_DQ_DQS_L);
    MDrv_Write2Byte(0x1011c8, MIU_1_DQ_DQS_H);
    MDrv_Write2Byte(0x1011ba, MIU_1_RDQSM_CMD_L);
    MDrv_Write2Byte(0x1011ca, MIU_1_RDQSM_CMD_H);
    MDrv_Write2Byte(0x1011d2, MIU_1_CS);
    MDrv_Write2Byte(0x101182, 0x0000);
    MDrv_Write2Byte(0x1011bc, MIU_1_DQS_CS_SKEW);
    MDrv_Write2Byte(0x1011cc, MIU_1_RDQSM_SKEW_H);
    MDrv_Write2Byte(0x1011be, MIU_1_DQS_CS_CLK_SKEW);
    MDrv_Write2Byte(0x1011d0, MIU_1_CSZ_SKEW);
    MDrv_Write2Byte(0x10118e, 0x00eb);
    MDrv_Write2Byte(0x10118e, 0x00ab);
    MDrv_Write2Byte(0x10118e, 0x00eb);
    MDrv_Write2Byte(0x10118e, 0x00ab);
    MDrv_Write2Byte(0x1011ee, MIU_1_RDQS_H);
    MDrv_Write2Byte(0x1011ec, MIU_1_RDQS_L);
    MDrv_Write2Byte(0x1011ea, 0x0020);
    MDrv_Write2Byte(0x1011e8, 0x0020);
    MDrv_Write2Byte(0x1011d4, 0xc000);
    MDrv_Write2Byte(0x10061e, 0x0c01);
    MDrv_Write2Byte(0x10061e, 0x0c00);
    MDrv_Write2Byte(0x101180, 0x0000|MIU_1_ODT);
    MDrv_Write2Byte(0x101182, 0xaaaa);
    MDrv_Write2Byte(0x100602, 0x03a7);
    MDrv_Write2Byte(0x10062a, 0x00c0);
    MDrv_Write2Byte(0x100602, 0x03a7);

    MDrv_Write2Byte(0x101174, MIU_0_TRIG_LVL); //trig lvl
    MDrv_Write2Byte(0x10115a, MIU_0_DRV_NP); //drvp
    MDrv_Write2Byte(0x10115c, MIU_0_DRV_N); //drvn
    MDrv_Write2Byte(0x10115e, MIU_0_DRV_P); //drvp
    MDrv_Write2Byte(0x1011f4, MIU_1_TRIG_LVL); //trig lvl
    MDrv_Write2Byte(0x1011da, MIU_1_DRV_NP); //drvp
    MDrv_Write2Byte(0x1011dc, MIU_1_DRV_N); //drvn
    MDrv_Write2Byte(0x1011de, MIU_1_DRV_P); //drvp

//-------------
// BW setting
//-------------
    MDrv_Write2Byte(0x101240, 0x8015);
    MDrv_Write2Byte(0x101260, 0x8015);
    MDrv_Write2Byte(0x101280, 0x8011);
    MDrv_Write2Byte(0x1012a0, 0x8011);
    MDrv_Write2Byte(0x100640, 0x8015);
    MDrv_Write2Byte(0x100660, 0x8015);
    MDrv_Write2Byte(0x100680, 0x8011);
    MDrv_Write2Byte(0x1006a0, 0x8011);
//---------------------------------
// Pack setting
//---------------------------------
    MDrv_Write2Byte(0x101228, 0x4090); //[14:14] reg_pack_en
    MDrv_Write2Byte(0x100628, 0x4090); //[14:14] reg_pack_en
//-------------------------
// Reduce latency setting
//-------------------------
    MDrv_Write2Byte(0x1012d4, 0x0000);
    MDrv_Write2Byte(0x1006d4, 0x0000);
//------------------------------
// sync in fifo stage
//------------------------------
    MDrv_Write2Byte(0x1012f8, 0x0600);
    MDrv_Write2Byte(0x1012fa, 0x0420);
    MDrv_Write2Byte(0x1012fc, 0x4600);
    MDrv_Write2Byte(0x1012fe, 0x1087);
    MDrv_Write2Byte(0x1006f8, 0x0600);
    MDrv_Write2Byte(0x1006fa, 0x0420);
    MDrv_Write2Byte(0x1006fc, 0x4600);
    MDrv_Write2Byte(0x1006fe, 0x1087);

    MDrv_Write2Byte(0x10121e, 0x1a00); //[ 0: 0] reg_sw_rst_miu
                           //[ 3: 3] reg_sw_init_done
                           //[12: 8] reg_sync_out_threshold
    MDrv_Write2Byte(0x10061e, 0x1a00); //[ 0: 0] reg_sw_rst_miu
                           //[ 3: 3] reg_sw_init_done
                           //[12: 8] reg_sync_out_threshold

    MDrv_Write2Byte(0x10121e, 0x1a01);
    MDrv_Write2Byte(0x10121e, 0x1a00);
    MDrv_Write2Byte(0x10061e, 0x1a01);
    MDrv_Write2Byte(0x10061e, 0x1a00);

//$display($time," start to program DLL0 ");
    MDrv_Write2Byte(0x101160, 0x000c);
    MDrv_Write2Byte(0x101160, 0x0008);
    MDrv_Write2Byte(0x101162, 0x007f);
    MDrv_Write2Byte(0x101164, 0xf200);
    MDrv_Write2Byte(0x101160, 0x2370);
//$display($time," start to program DLL1 ");
    MDrv_Write2Byte(0x1011e0, 0x000c);
    MDrv_Write2Byte(0x1011e0, 0x0008);
    MDrv_Write2Byte(0x1011e2, 0x007f);
    MDrv_Write2Byte(0x1011e4, 0xf200);
    MDrv_Write2Byte(0x1011e0, 0x2370);

    if(u8STRInit)
    {
        MDrv_Write2Byte(0x101108, 0x0000);
    }
    else
    {
        MDrv_Write2Byte(0x101108, 0x003f);
    }
    MDrv_Write2Byte(0x101200, 0x0000);
    MDrv_Write2Byte(0x101188, 0x003f);
    MDrv_Write2Byte(0x100600, 0x0000);
//-----Wait 200us, (keep DRAM RESET = 0)-----
//wait 1
    MDrv_SysDelayUs(1000);
    MDrv_Write2Byte(0x101200, 0x0008);
    MDrv_Write2Byte(0x101200, 0x000c);
    MDrv_Write2Byte(0x100600, 0x0008);
    MDrv_Write2Byte(0x100600, 0x000c);
//-----Wait 500us, (keep DRAM RESET = 1 , CKE = 0)-----
//wait 1
    MDrv_SysDelayUs(1000);
    MDrv_Write2Byte(0x101200, 0x000e);
    MDrv_Write2Byte(0x100600, 0x000e);
//-----Wait tXPR=400ns-----
//wait 1
    MDrv_SysDelayUs(1000);
//--------Initial DRAM start here!!!
    MDrv_Write2Byte(0x101200, 0x001f);
#if (DRAM_BUS_WIDTH == DRAM_BUS_64B)
    MDrv_Write2Byte(0x100600, 0x001f);
#endif
//-----Wait init done-----
//wait 100
    MDrv_SysDelayUs(100000);
//--------Initial Done
//--------DDR2 wait 400ns for tXPR(?), DDR3 wait 512T for tZQinit
//wait 1
    MDrv_SysDelayUs(1000);

//------------------------------
// unmask all
//------------------------------
    MDrv_Write2Byte(0x10121a, 0x0001);
    MDrv_Write2Byte(0x101246, 0x0000);
    MDrv_Write2Byte(0x101266, 0x0000);
    MDrv_Write2Byte(0x101286, 0x0000);
    MDrv_Write2Byte(0x1012a6, 0x0000);
    MDrv_Write2Byte(0x10121e, 0x1a08); //[ 0: 0] reg_sw_rst_miu
                           //[ 3: 3] reg_sw_init_done
                           //[12: 8] reg_sync_out_threshold
    MDrv_Write2Byte(0x10061a, 0x0001);
    MDrv_Write2Byte(0x100646, 0x0000);
    MDrv_Write2Byte(0x100666, 0x0000);
    MDrv_Write2Byte(0x100686, 0x0000);
    MDrv_Write2Byte(0x1006a6, 0x0000);
    MDrv_Write2Byte(0x10061e, 0x1a08); //[ 0: 0] reg_sw_rst_miu
                           //[ 3: 3] reg_sw_init_done
                           //[12: 8] reg_sync_out_threshold

    MDrv_Write2Byte(0x10110e, 0x00ab);
    MDrv_Write2Byte(0x10110e, 0x00eb);
    MDrv_Write2Byte(0x10110e, 0x00ab);

    MDrv_Write2Byte(0x10118e, 0x00ab);
    MDrv_Write2Byte(0x10118e, 0x00eb);
    MDrv_Write2Byte(0x10118e, 0x00ab);
//=================================================================
// End of MIU init !!!
//=================================================================
#if 0
    MDrv_Write2Byte(0x1012f0, 0x0000); //sel MIU0
//-------------------------------
//miu self test : mode 0007
//-------------------------------
    MDrv_Write2Byte(0x1012e0, 0x0000);
    MDrv_Write2Byte(0x1012e2, 0x0080);
    MDrv_Write2Byte(0x1012e4, 0x0021);
    MDrv_Write2Byte(0x1012e6, 0x0000);
    MDrv_Write2Byte(0x1012e8, 0x5aa5);
    MDrv_Write2Byte(0x1012d8, 0x0000);
    MDrv_Write2Byte(0x1012e0, 0x0006);
    MDrv_Write2Byte(0x1012e0, 0x0007);
//-----------------------------
//Wait for MIU self test result
//-----------------------------
    MDrv_Write2Byte(0x1012e0, 0x0000);
    MDrv_Write2Byte(0x1012f0, 0x8000); //sel MIU1
//------------------------------
//miu self test : mode 0007
//-------------------------------
    MDrv_Write2Byte(0x1012e0, 0x0000);
    MDrv_Write2Byte(0x1012e2, 0x0080);
    MDrv_Write2Byte(0x1012e4, 0x0021);
    MDrv_Write2Byte(0x1012e6, 0x0000);
    MDrv_Write2Byte(0x1012e8, 0x5aa5);
    MDrv_Write2Byte(0x1012d8, 0x0000);
    MDrv_Write2Byte(0x1012e0, 0x0006);
    MDrv_Write2Byte(0x1012e0, 0x0007);
//-----------------------------
//Wait for MIU self test result
//-----------------------------
//    MDrv_Write2Byte(0x1012e0, 0x0000);
//    MDrv_Write2Byte(0x10123c, 0x0040); //[    7] 1 turn on reg_adc_test_en
//    MDrv_Write2Byte(0x1012fe, 0x0887); //[   11] 1 turn off miu engine, reg_miu_test_off
//    MDrv_Write2Byte(0x1006f0, 0x7ffe); //miu_sel --> hw sel
//    MDrv_Write2Byte(0x1006f2, 0xffff); //miu_sel --> hw sel
//    MDrv_Write2Byte(0x1006f4, 0xffff); //miu_sel --> hw sel
//    MDrv_Write2Byte(0x1006f6, 0xffff); //miu_sel --> hw sel
#endif

}
#endif
#endif

static int MDrv_MiuInitial(U8 u8AutoDQS, U8 u8Bist, U8 u8STRInit)
{
    int ddr_clk = 0;
    bool bReturn = 1;
    U8 u8dqsReturn = 0;

    u8STRInit = u8STRInit;
#if FPGA_VERIFY
    MDrv_MiuInitial_FPGA();
    return 1;
#endif

    //for tvtool miu access
    MDrv_Write2Byte(0x002bc6, 0x3c19);
    MDrv_Write2Byte(0x002bca, 0x403c);
    MDrv_Write2Byte(0x002bc4, 0x0004);

    if(g_DoMiuInit)
    {
        //set miu artbiter clock
        //h0002 h0002   11  8 (reg_ckg_miu_256b)
        MDrv_WriteByteMask(0x120f05, 0x0C, 0x0f); //0: miu_256b_gate
                                                  //1: miu_256b_inv
                                                  //[3:2]:miu_256b clk sel
                                                  //00: miu256b
                                                  //01: 320
                                                  //10: 240
                                                  //11: memp110_clk025x

        //h0003 h0003   3   0 (reg_ckg_miu)
        MDrv_WriteByteMask(0x120f06, 0x08, 0x0f); //0: miu_gate
                                                  //1: miu_inv
                                                  //[3:2]:miu clk sel
                                                  //00: miu128b
                                                  //01: 345
                                                  //10: 288
                                                  //11: memp110_clk025x

        //Will merge register settings after scripts comfirmed..
#if (CHIP_ID==MST9U2)
        g_DoMiuInit = 0;
      #if (MIU_SPEED == DDR3_1866MHz_e)
        #warning "RUN MiuInit2_1866Mhz..."
        MiuInit2_1866Mhz(u8STRInit);
        ddr_clk = 1866;
      #else
        #warning "RUN MiuInit2_2133Mhz..."
        MiuInit2_2133Mhz(u8STRInit);
        ddr_clk = 2133;
      #endif
        MDrv_Write2Byte(0x1006F4, 0x183F);
        MDrv_Write2Byte(0x1006F6, 0x703E);
        //Set MIU_Sel of PM51, HK_R2, CP_R2, BDMA to HW mode, ie., choosen by clinet.
        MDrv_Write2Byte(0x1006F0, BIT14|BIT5|BIT4|BIT3 );
      #if (DRAM_BUS_WIDTH == DRAM_BUS_64B)
        MDrv_Write2Byte(0x1012F4, 0x00C0);
      #else
        // =====MIU1 PD===
        MDrv_Write2Byte(0x101188, 0x0000);
        MDrv_Write2Byte(0x1011d4, 0xc030);
        MDrv_Write2Byte(0x1011b2, 0x801d);
        MDrv_Write2Byte(0x1011e0, 0x2372);
      #endif
      #if (DRAM_BUS_WIDTH == DRAM_BUS_16B)
        // =====MIU0 HW PD===
        MDrv_Write2Byte(0x101108, 0x003d);
        MDrv_Write2Byte(0x101154, 0xc020);
        MDrv_Write2Byte(0x101148, 0xffff);
        MDrv_Write2Byte(0x10114a, 0xf0f2);
        MDrv_Write2Byte(0x101152, 0x00f5);
      #endif
#elif (CHIP_ID==MST9U3)
        g_DoMiuInit = 0;

        #if (MIU_SPEED == DDR2_1333MHz_e)
            #warning "RUN MiuInit_1333Mhz..."
        #else
            #warning "RUN MiuInit_1240Mhz..."
        #endif

      #if MST9U3_BGA //BGA
        {
            U8 u8DDRSel;

            MiuInit_1333Mhz_PreInit(u8STRInit);

            MDrv_WriteByteMask(0x043E, 0x20, 0x20);// GPIO65 OEZ
            u8DDRSel = MDrv_ReadByte(0x043F);

            if(u8DDRSel&0x20) // GPIO65: Hi->ESMT, Lo->WB
                MiuInit_1333Mhz_ESMT();
            else
                MiuInit_1333Mhz_Winbond();
        }
      #elif MST9U3_QFP // QFP
        {
            U8 u8DDRSel;
            u8DDRSel = MDrv_ReadByte(0x101EFA);

            if( (u8DDRSel&0x01) && (MIU_SPEED == DDR2_1240MHz_e) )
                MiuInit_1333Mhz_PreInit(u8STRInit, TRUE);
            else
                MiuInit_1333Mhz_PreInit(u8STRInit, FALSE);

            if(u8DDRSel&0x01)
                MiuInit_1333Mhz_Winbond_QFP();
            else
                MiuInit_1333Mhz_ESMT_QFP();

        }
      #endif
        ddr_clk = 1333;
#else // MST9U
        g_DoMiuInit = 0;
      #if (MIU_SPEED == DDR3_1866MHz_e)
        #warning "RUN MiuInit_1866Mhz..."
        MiuInit_1866Mhz(u8STRInit);
        ddr_clk = 1866;
      #elif (MIU_SPEED == DDR3_2133MHz_e)
        #warning "RUN MiuInit_2133Mhz..."
        MiuInit_2133Mhz(u8STRInit);
        ddr_clk = 2133;
      #endif
#endif
        //set dram size
        //h0069 h0069   15  12  reg_dram_size
        //0:for test, 1:2MB, 2:4MB, 3:8MB, 4:16MB,
        //5:32MB, 6:64MB, 7:128MB, 8:256MB, 9:512MB, A:1GB
#if (CHIP_ID==MST9U3)
#if (DRAM_BUS_WIDTH == DRAM_BUS_32B)
        MDrv_WriteByteMask(0x1012d3, 0x70, 0xF0);
#else   // DRAM_BUS_16B
        MDrv_WriteByteMask(0x1012d3, 0x60, 0xF0);
#endif
#else
        MDrv_WriteByteMask(0x1012d3, 0x90, 0xF0);
#endif
        //enable pack
        MDrv_WriteByteMask(0x101229, BIT6, BIT6);
    }


#if ENABLE_AUTO_DQS
    if (u8AutoDQS)
    {
        u8dqsReturn = MDrv_MiuAutoDQSPhase(0); //MIU0 autoDQS
#if(    ((CHIP_ID==MST9U2) && (DRAM_BUS_WIDTH == DRAM_BUS_64B))\
    ||  ((CHIP_ID==MST9U3) && (DRAM_BUS_WIDTH == DRAM_BUS_32B)) )
        if(u8dqsReturn)
            u8dqsReturn = MDrv_MiuAutoDQSPhase(1); //MIU1 autoDQS
#endif

    }
#endif

    //DQS reset
    MDrv_Write2Byte(0x10110e, MDrv_Read2Byte(0x10110e)|BIT6);
    MDrv_Write2Byte(0x10110e, MDrv_Read2Byte(0x10110e)&~BIT6);
#if (CHIP_ID==MST9U2)||(CHIP_ID==MST9U3)
    MDrv_Write2Byte(0x10118e, MDrv_Read2Byte(0x10118e)|BIT6);
    MDrv_Write2Byte(0x10118e, MDrv_Read2Byte(0x10118e)&~BIT6);
#endif

    //Decrease DDR speed by DDR_SSC_DEVIATION
    if(!u8STRInit)
    {
        U32 u32DdrSet = MDrv_Read3Byte(0x101130);
        u32DdrSet += ((u32DdrSet*DDR_SSC_DEVIATION)/(1000-DDR_SSC_DEVIATION));
        MDrv_Write3Byte(0x101130,u32DdrSet);
#if (CHIP_ID==MST9U2)||(CHIP_ID==MST9U3)
        MDrv_Write3Byte(0x1011B0,u32DdrSet); // follow 0x101130, SUPPOSE 0x101130 = 0x1011B0
#endif
    }

    if (u8Bist)
    {
        printf("\r\nddr_%d bist", ddr_clk);
        bReturn = MDrv_MiuBistTest(0, 0x1000, 0, 0, 0); //MIU0 bist
#if(   ((CHIP_ID==MST9U2) && (DRAM_BUS_WIDTH == DRAM_BUS_64B))\
    || ((CHIP_ID==MST9U3) && (DRAM_BUS_WIDTH == DRAM_BUS_32B)) )
        if (bReturn)
            bReturn = MDrv_MiuBistTest(1, 0x1000, 0, 0, 0); //MIU1 bist
#endif
        //MDrv_MiuBistTest(0x1000, 1, 1, 0); // Read only
        //MDrv_MiuBistTest(0x1000, 1, 0, 1); // Write only
    }
    MDrv_Write2Byte(0x1012e0, 0x0000);

    return bReturn;
}

#if (ENABLE_DRAM_SELFREFRESH)
static void MIU_EnterSelfRefresh(void)
{
    MDrv_Write2Byte(0x1012E0, 0x0000);

//*************************************
#if CHIP_ID == MST9U3
    MDrv_SysDelayUs(1000);

    MDrv_Write2ByteMask(0x101206, BIT12, BIT12);
    MDrv_Write2Byte(0x101246, 0xfffe); //mask other request => reg_rq0_mask(save path "0" for miu_cmd)
    MDrv_Write2Byte(0x101266, 0xffff); //mask other request => reg_rq1_mask
    MDrv_Write2Byte(0x101286, 0xffff); //mask other request => reg_rq2_mask
    MDrv_Write2Byte(0x1012a6, 0xffff); //mask other request => reg_rq3_mask

    MDrv_SysDelayUs(1000);

    MDrv_Write2Byte(0x101218, 0x0400);
    MDrv_Write2Byte(0x101200, 0x002f);
    MDrv_Write2Byte(0x101200, 0x052f);
    MDrv_Write2Byte(0x101200, 0x002f);
    MDrv_Write2Byte(0x101200, 0x032f);
    MDrv_Write2Byte(0x101200, 0x002f);

    MDrv_SysDelayUs(1000);

    MDrv_Write2Byte(0x101246, 0xffff);
    MDrv_Write2Byte(0x101200, 0x202f);

    MDrv_SysDelayUs(1000);

#else
    MDrv_Write2Byte(0x101246, 0xfffe); //mask other request => reg_rq0_mask(save path "0" for miu_cmd)
    MDrv_Write2Byte(0x101266, 0xffff); //mask other request => reg_rq1_mask
    MDrv_Write2Byte(0x101286, 0xffff); //mask other request => reg_rq2_mask
    MDrv_Write2Byte(0x1012a6, 0xffff); //mask other request => reg_rq3_mask
    //wait 1 //delay 1ms
    MDrv_SysDelayUs(1000);

    MDrv_Write2Byte(0x101218, 0x0400); //tie A10=1 to enter precharge all
                           //reg_mrx = address in miu_cmd
    MDrv_Write2Byte(0x101200, 0x002f); //turn off auto refresh
                           //reg_auto_ref_off                      1 : [5]
    MDrv_Write2Byte(0x101200, 0x052f); //trigger precharge all
                           //single cmd [9:7] = {wez, casz, rasz} = {0, 1, 0}
                           //reg_single_cmd_en                     1 : [8]
    MDrv_Write2Byte(0x101200, 0x002f);
    MDrv_Write2Byte(0x101200, 0x032f); //trigger refresh
                           //single cmd [9:7] = {wez, casz, rasz} = {0, 0, 1}
                           //reg_single_cmd_en                     1 : [8]
    MDrv_Write2Byte(0x101200, 0x002f);
    MDrv_Write2ByteMask(0x101206, BIT12, BIT12); //csz cke always on
                           //reg_csz_always_on                     1 : [11] (ECO method)
                           //reg_cke_always_on                     1 : [12]
    //wait 1 //delay 1ms
    MDrv_SysDelayUs(1000);

    MDrv_Write2Byte(0x101246, 0xffff); //turn off command request
    MDrv_Write2Byte(0x101200, 0x202f); //Enter self refresh mode
                           //reg_self_refresh                      1 : [13]
    //wait 1 //delay 1ms
    MDrv_SysDelayUs(1000);
#endif
}

void MIU_ExitSelfRefresh(void);
void MIU_ExitSelfRefresh(void)
{
#if CHIP_ID == MST9U3
    MDrv_MiuInitial(0, 0, 1);

    MIU_EnterSelfRefresh();

    //AN_PowerUp.txt
    MDrv_WriteByteMask(0x101203, 0x00, 0xF0);
    MDrv_WriteByteMask(0x101100, 0x00, 0x08);
    MDrv_WriteByteMask(0x101180, 0x00, 0x08);
    MDrv_Write2Byte(0x101154, 0xC000);
    MDrv_Write2Byte(0x1011D4, 0xC000);
    MDrv_Write2Byte(0x101108, 0x003F);
    MDrv_Write2Byte(0x101188, 0x003F);

    //Exit_self_refresh.txt
    MDrv_Write2Byte(0x101202, (0x0392|DRAM_BUS_MASK));
    MDrv_Write2Byte(0x101200, 0x002f);
    MDrv_Write2Byte(0x101246, 0xfffe);
    MDrv_Write2Byte(0x101200, 0x002f);

    MDrv_SysDelayUs(1000);

    MDrv_Write2Byte(0x101200, 0x032f);
    MDrv_Write2Byte(0x101200, 0x002f);
    MDrv_Write2Byte(0x101200, 0x001f);

    MDrv_SysDelayUs(10000);

    MDrv_Write2Byte(0x101246, 0xfffe); //turn on all mask
    MDrv_Write2Byte(0x101266, 0xffff); //turn on all mask
    MDrv_Write2Byte(0x101286, 0x7fff); //turn on all mask
    MDrv_Write2Byte(0x1012a6, 0xffff); //turn on all mask

    //dummy_read.txt
    MDrv_Write2Byte(0x1012E0, 0x0000);
    MDrv_Write2Byte(0x1012E2, 0x0000);
    MDrv_Write2Byte(0x1012E4, 0x8001);
    MDrv_Write2Byte(0x1012E6, 0x0000);
    MDrv_Write2Byte(0x1012E0, 0x0100);
    MDrv_Write2Byte(0x1012E0, 0x0101);
    MDrv_SysDelayUs(10000);
    MDrv_Write2Byte(0x1012E0, 0x0000);

    MDrv_Write2Byte(0x101246, 0x0000); //turn on all mask
    MDrv_Write2Byte(0x101266, 0x0000); //turn on all mask
    MDrv_Write2Byte(0x101286, 0x0000); //turn on all mask
    MDrv_Write2Byte(0x1012a6, 0x0000); //turn on all mask

#else
//*************************************
//*PM_poweron
//*Set cko_oenz,adr_oenz,dq_oenz,cke_oenz as 1'b1
//*Set miu all mask
//*Re-Init miu with atop sequence(avoid glitch)
//*Set gpio_oenz from 1'b1 to 1'b0
//*Write stored RX dqs phase value

//*Re-Enter SELF REFRESH
//*Set cko_oenz,adr_oenz,dq_oenz,cke_oenz from 1'b1 to 1'b0
//*Set PD from 1'b1 to 1'b0
//*Set RX_EN to 1'b1
//*Exit SELF REFRESH
//*DQSM Reset
//*Set miu all unmask
//*************************************

    //Miu Mask
    MDrv_Write2Byte(0x101246, 0x7ffe); //mask other request
    MDrv_Write2Byte(0x101266, 0xffff); //mask other request
    MDrv_Write2Byte(0x101286, 0xffff); //mask other request
    MDrv_Write2Byte(0x1012a6, 0xffff); //mask other request
    #if (CHIP_ID == MST9U2)
    MDrv_Write2Byte(0x100646, 0x7ffe); //mask other request
    MDrv_Write2Byte(0x100666, 0xffff); //mask other request
    MDrv_Write2Byte(0x100686, 0xffff); //mask other request
    MDrv_Write2Byte(0x1006a6, 0xffff); //mask other request
    #endif

    MDrv_MiuInitial(0, 0, 1);
    MIU_EnterSelfRefresh();

    //wait 1 //delay 1ms
    MDrv_SysDelayUs(1000);
    MDrv_Write2Byte(0x101202, (0x03a3|DRAM_BUS_MASK)); //turn on oenz
                           //reg_cke_oenz                          0 : [12]
                           //reg_dq_oenz                           0 : [13]
                           //reg_adr_oenz                          0 : [14]
                           //reg_cko_oenz                          0 : [15]

    MDrv_Write2ByteMask(0x101204, 0, BIT11|BIT10); //reg_cs_oenz                           0 : [10]
                           //reg_cs1_oenz                          0 : [11]

    //PLL PD
    MDrv_Write2ByteMask(0x101132, 0, BIT15); //reg_ddrpll_pd                         0 : [15]
    //wait 1 //delay 1ms
    MDrv_SysDelayUs(1000);

    MDrv_Write2ByteMask(0x101100, 0, BIT4); //[ 0: 0] reg_pkg_sel
                           //[ 3: 3] reg_gpio_oenz                 1
                           //[ 4: 4] reg_gpio_mode                 0
                           //[ 5: 5] reg_mclk_pd(this bit no use)
    MDrv_Write2Byte(0x10117e, 0x0004); //[15: 0] reg_ddr_test                  0: [7:3] (cortrol gpio mode), [2] for MST9U2, no loading in MST9U1


    MDrv_Write2ByteMask(0x101100, 0, BIT3); //[ 0: 0] reg_pkg_sel
                           //[ 3: 3] reg_gpio_oenz                 0
                           //[ 4: 4] reg_gpio_mode                 0
                           //[ 5: 5] reg_mclk_pd(this bit no use)

     //atop PD
    MDrv_Write2Byte(0x101154, 0xc000); //reg_atop_pd                           0 : [ 6: 4]
    MDrv_Write2Byte(0x101108, 0x003f); //[ 5: 0] reg_rx_en                     1

    MDrv_Write2Byte(0x101200, 0x002f); //exit self refresh
    MDrv_Write2Byte(0x101246, 0xfffe); //mask other request => reg_rq0_mask(save path "0" for miu_cmd)
    MDrv_Write2Byte(0x101200, 0x002f);
    //wait 1 //delay 1ms
    MDrv_SysDelayUs(1000);
    MDrv_Write2Byte(0x101200, 0x032f); //trigger refresh
                           //single cmd [9:7] = {wez, casz, rasz} = {0, 0, 1}
                           //reg_single_cmd_en                     1 : [8]
    MDrv_Write2Byte(0x101200, 0x002f);
    MDrv_Write2Byte(0x101200, 0x000f); //turn on auto refresh
                           //reg_auto_ref_off                      0 : [5]

    MDrv_Write2Byte(0x101200, 0x001f);

    //wait 10 //delay 10ms
    MDrv_SysDelayUs(10000);
    MDrv_Write2ByteMask(0x10110e, BIT6, BIT6); //[ 0: 0] reg_en_mask
                           //[ 4: 1] reg_dqsm_dly
                           //[ 5: 5] reg_dqsm_sw_rst
                           //[ 6: 6] reg_dqsm_rst_sel              1
                           //[ 7: 7] reg_dqsm_sta_rst
                           //[11: 8] reg_r_dqs025x_snt
                           //[15:12] reg_r_dqsm_status_fal
    MDrv_Write2ByteMask(0x10110e, 0, BIT6); //[ 0: 0] reg_en_mask
                           //[ 4: 1] reg_dqsm_dly
                           //[ 5: 5] reg_dqsm_sw_rst
                           //[ 6: 6] reg_dqsm_rst_sel              0
                           //[ 7: 7] reg_dqsm_sta_rst
                           //[11: 8] reg_r_dqs025x_snt
                           //[15:12] reg_r_dqsm_status_fall


    MDrv_Write2ByteMask(0x101206, BIT12, BIT12); //csz cke always on
                           //reg_csz_always_on                     0 : [11]
                           //reg_cke_always_on                     1 : [12]
    MDrv_Write2Byte(0x101246, 0x0000); //turn on all mask
    MDrv_Write2Byte(0x101266, 0x0000); //turn on all mask
    MDrv_Write2Byte(0x101286, 0x0000); //turn on all mask
    MDrv_Write2Byte(0x1012a6, 0x0000); //turn on all mask
#endif
}
#endif

#if ENABLE_PRINT_MSG
void mcuSetUartMux( UART_ENGINE ucUartEngine, UART_GPIO ucUartGPIO );
void mcuSetUartMux( UART_ENGINE ucUartEngine, UART_GPIO ucUartGPIO )
{
    switch(ucUartGPIO)
    {
        case UART_GPIO02_03:
            MDrv_WriteByteMask(0x000414, ucUartEngine, 0x07);
            break;
        case UART_GPIO40_41:
            MDrv_WriteByteMask(0x000415, ucUartEngine, 0x07);
            break;
        case UART_GPIO50_51:
            MDrv_WriteByteMask(0x000414, ucUartEngine<<4, 0x70);
            break;
        case UART_GPIOX03_04:
            MDrv_WriteByteMask(0x000410, ucUartEngine, 0x07);
            break;
        case UART_GPIOX13_14:
            MDrv_WriteByteMask(0x000410, ucUartEngine<<4, 0x70);
            break;
        case UART_GPIOX23_24:
            MDrv_WriteByteMask(0x000411, ucUartEngine, 0x07);
            break;
        case UART_GPIOX33_34:
            MDrv_WriteByteMask(0x000411, ucUartEngine<<4, 0x70);
            break;
        case UART_GPIOX43_44:
            MDrv_WriteByteMask(0x000412, ucUartEngine, 0x07);
            break;
        case UART_GPIOX53_54:
            MDrv_WriteByteMask(0x000412, ucUartEngine<<4, 0x70);
            break;
        default:
            //DRVMCU_PRINT("******UART MUX ERROR*****\n");
            break;
    }
}
#endif

static void MDrv_InitConsole( void )
{
    #if FPGA_VERIFY
    {
    #if ENABLE_PRINT_MSG
        mcuSetUartMux(DEFAULT_UART_DEV, DEFAULT_UART_PORT);
        uart_init(R2_CLK/*12000000*/, UART0_BAUDRATE_FPGA);
    #endif
        mtspr( SPR_TTMR, 0x40000000 | (R2_CLK/1000)/*12000*/ );
    }
    #else
    {
    #if ENABLE_PRINT_MSG
        mcuSetUartMux(DEFAULT_UART_DEV, DEFAULT_UART_PORT);
        uart_init(R2_CLK, UART0_BAUDRATE);
    #endif
        mtspr( SPR_TTMR, 0x40000000 | (R2_CLK/1000) );
    }
    #endif
}

extern unsigned char _sboot_end[];
void BL_Init(void);
void BL_Init(void)
{
#if (!ENABLE_MSTAR_MST9U)
    U16 temp=0;
#endif
    const U32 MIU_TRY_MAX = 10;
    U32 u32MIU_Init_try = 0;

    // system MPLL & CLKs initialization
    InitialSystemPowerUp();

#if (!ENABLE_USB_HOST_DUAL_IMAGE)
    // PM.bin address offset setting
    MDrv_Write2Byte(0x0010F6, (WORD)(_sboot_end+0x80)); // 0x80: chunk header size
    MDrv_Write2Byte(0x0010F8, (((DWORD)_sboot_end+0x80)>>16));
#endif

    // console initialization
    MDrv_InitConsole();

  #if (RTL_SIMULATION == 0)
    printf("\r\n MST9U: %s @%s(%s)\r\n", BOARD_NAME, __TIME__, __DATE__);
    printf(" SPI BOOT \r\n");
    printf(" Console Initial OK \r\n");
  #endif

#if (!ENABLE_MSTAR_MST9U)
    // disable AVD before MIU init
    temp = MDrv_Read2Byte(0x10362a);
    temp = temp | 0x0010;
    MDrv_Write2Byte(0x10362a, temp);
#endif

    // MIU initialization
#if (ENABLE_DRAM_SELFREFRESH)
    if (IS_WAKEUP_FROM_PM)
    {
        //wakeup from PM
        MIU_ExitSelfRefresh();
        printf("\r\n ExitSelfRefresh Ok\r\n");
    }
#endif
    while (u32MIU_Init_try++ < MIU_TRY_MAX)
    {
        if (MDrv_MiuInitial(1, 1, 0))
        {
            printf("\n\r MIU Initial OK (try=%d)\n\r", (int)u32MIU_Init_try);
            break;
        }
        g_DoMiuInit = 1;
    }
    if (u32MIU_Init_try >= MIU_TRY_MAX)
        printf("\n\r MIU Initial failed (try=%d)\n\r", (int)u32MIU_Init_try);

#if (CHIP_ID == MST9U3) && (!ENABLE_DRAM_SELFREFRESH)
    MDrv_Write2Byte(0x1014ec, 0xc000); //enable DRAM OBF
#endif
  #if (RTL_SIMULATION == 1)
    while(1);
  #endif

}

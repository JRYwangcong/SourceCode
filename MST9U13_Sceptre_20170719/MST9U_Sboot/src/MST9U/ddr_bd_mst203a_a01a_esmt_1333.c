
#include "hwreg_MST9U.h"

#if (CHIP_ID==MST9U3)

#define DDR2_TYPE               DDR2_TYPE_ESMT
#include "drv_miu_init.h"
#include "ddr_bd.h"
#include "drvGlobal.h"


void MiuInit_1333Mhz_ESMT(void);
void MiuInit_1333Mhz_ESMT(void)  //2133_2133
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
    MDrv_Write2Byte(0x101136, MIU_0_CLK);
    MDrv_Write2Byte(0x101134, 0x0000);
    MDrv_Write2Byte(0x101130, 0x8000);
#if (MIU_SPEED == DDR2_1333MHz_e)
    MDrv_Write2Byte(0x101132, 0x0029);
#else   // 1240
    MDrv_Write2Byte(0x101132, 0x002C);
#endif
    MDrv_Write2Byte(0x101120, 0x0020);

    MDrv_Write2Byte(0x1011B6, MIU_1_CLK);
    MDrv_Write2Byte(0x1011B4, 0x0000);
    MDrv_Write2Byte(0x1011B0, 0x8000);
#if (MIU_SPEED == DDR2_1333MHz_e)
    MDrv_Write2Byte(0x1011B2, 0x0029);
#else   // 1240
    MDrv_Write2Byte(0x1011B2, 0x002C);
#endif
    MDrv_Write2Byte(0x1011A0, 0x0020);

    MDrv_Write2Byte(0x10111a, MIU_0_SEL_ODT_MODE);
    MDrv_Write2Byte(0x10119a, MIU_1_SEL_ODT_MODE);
    MDrv_Write2Byte(0x10114e, MIU_ODT_SKEW);
    //wait 1    // delay 1ms
    MDrv_SysDelayUs(1000);
//---------------------------
//set DDR3_32_8X_CL13_1866
//---------------------------
    //MDrv_Write2Byte(0x101202, (0xf392|DRAM_BUS_MASK));
    MDrv_Write2Byte(0x101204, 0x000a);
    MDrv_Write2Byte(0x101206, MIU_CYCLE_PERIOD);  //[12:12] reg_cke_always_on
    MDrv_Write2Byte(0x101208, 0x1e99);
    MDrv_Write2Byte(0x10120a, 0x2767);
    MDrv_Write2Byte(0x10120c, MIU_TWLR_TIMING);
    MDrv_Write2Byte(0x10120e, MIU_TRFC_TIMING);
    MDrv_Write2Byte(0x101210, MIU_MODE_REG);
    MDrv_Write2Byte(0x101212, 0x4004);
    MDrv_Write2Byte(0x101214, 0x8000);
    MDrv_Write2Byte(0x101216, 0xc000);
    MDrv_Write2Byte(0x101102, 0xaaaa); //[ 7: 0] reg_cko_state
                           //[ 7: 0] reg_dqs_state
    //MDrv_Write2Byte(0x101202, 0x03a7);
    //MDrv_Write2Byte(0x10122a, 0x4000);
    //MDrv_Write2Byte(0x101202, 0x03a7);
    MDrv_Write2Byte(0x101104, 0x0080); //[ 7: 0] reg_dqs_pre_state
    MDrv_Write2Byte(0x10113c, MIU_0_DQS_CS_SKEW); //[ 2: 0] reg_dqsm0_skew
                           //[ 6: 4] reg_dqsm1_skew
                           //[10: 8] reg_dqs_skew
                           //[14:12] reg_cmd1_skew
    MDrv_Write2Byte(0x10113e, MIU_0_DQS_CS_CLK_SKEW); //[ 2: 0] reg_oen_skew
                           //[ 6: 4] reg_dq_skew
                           //[10: 8] reg_cmd0_skew
                           //[14:12] reg_cko_skew
   //MDrv_Write2Byte(0x10114c, MIU_0_RDQSM_SKEW_H); //[ 2: 0] reg_dqsm2_skew
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
    MDrv_Write2Byte(0x10110a, MIU_0_CLK_PHASE);
    MDrv_Write2Byte(0x101152, MIU_0_CS);
    MDrv_Write2Byte(0x10112E, 0x2200);

    MDrv_Write2Byte(0x10110E, 0x00A5);
    MDrv_Write2Byte(0x10110E, 0x00E5);
    MDrv_Write2Byte(0x10110E, 0x00A5);

    MDrv_Write2Byte(0x101168, 0x2020);
    MDrv_Write2Byte(0x10116A, 0x2020);
    MDrv_Write2Byte(0x10116E, MIU_0_RDQS_H);
    MDrv_Write2Byte(0x10116C, MIU_0_RDQS_L);
    MDrv_Write2Byte(0x101118, 0x0000);
    MDrv_Write2Byte(0x101158, 0x0707);
    MDrv_Write2Byte(0x10115A, MIU_0_DRV_NP);
    MDrv_Write2Byte(0x10115C, MIU_0_DRV_N);
    MDrv_Write2Byte(0x10115E, MIU_0_DRV_P);
    MDrv_Write2Byte(0x101174, MIU_0_TRIG_LVL);

    MDrv_Write2Byte(0x101182, 0xaaaa);
    MDrv_Write2Byte(0x101184, 0x0080);
    MDrv_Write2Byte(0x1011bc, MIU_1_DQS_CS_SKEW);
    MDrv_WriteByte(0x1011ce,  (U8)MIU_1_DQS_CS_SKEW);
    MDrv_Write2Byte(0x1011be, MIU_1_DQS_CS_CLK_SKEW);
    MDrv_Write2Byte(0x1011d0, MIU_1_CSZ_SKEW);
    MDrv_Write2Byte(0x1011b8, MIU_1_DQ_DQS_L);
    MDrv_Write2Byte(0x1011ba, MIU_1_RDQSM_CMD_L);
    MDrv_Write2Byte(0x10118a, MIU_1_CLK_PHASE);
    MDrv_Write2Byte(0x1011d2, MIU_1_CS);
    MDrv_Write2Byte(0x1011ae, 0x2200);

    MDrv_Write2Byte(0x10118E, 0x00A5);
    MDrv_Write2Byte(0x10118E, 0x00E5);
    MDrv_Write2Byte(0x10118E, 0x00A5);

    MDrv_Write2Byte(0x1011e8, 0x2020);
    MDrv_Write2Byte(0x1011EA, 0x2020);
    MDrv_Write2Byte(0x1011ee, MIU_1_RDQS_H);
    MDrv_Write2Byte(0x1011ec, MIU_1_RDQS_L);
    MDrv_Write2Byte(0x101198, 0x0000);

    MDrv_Write2Byte(0x1011d8, 0x0707);
    MDrv_Write2Byte(0x1011dA, MIU_1_DRV_NP);
    MDrv_Write2Byte(0x1011dC, MIU_1_DRV_N);
    MDrv_Write2Byte(0x1011dE, MIU_1_DRV_P);
    MDrv_Write2Byte(0x1011f4, MIU_1_TRIG_LVL);

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
    MDrv_Write2Byte(0x101228, MIU_ODT_DELAY);

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

#endif

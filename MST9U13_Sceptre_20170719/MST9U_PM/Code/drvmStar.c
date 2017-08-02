
///////////////////////////////////////
// DRVMSTAR_V VERSION: V02
////////////////////////////////////////
#include "Global.h"
//#include "drvDPRxApp.h"
//#include "msEread.h"
//#include "appWindow.h"
//#include "appInput.h"
//#include "appSystem.h"
//#include "msScaler.h"
//#include "mapi_combo.h"
//#include "AutoFunc.h"
//#include "menufunc.h"
#include "drvmStar.h"
#include "Mapi_combo.h"

#ifndef _DRVMSTAR_D_C_
#define _DRVMSTAR_D_C_

#define drvmStar_DEBUG    1
#if ENABLE_DEBUG &&drvmStar_DEBUG
    #define drvmStar_printData(str, value)   printData(str, value)
    #define drvmStar_printMsg(str)           printMsg(str)
#else
    #define drvmStar_printData(str, value)
    #define drvmStar_printMsg(str)
#endif

#define LPLL_REG_NUM    18

extern code ST_INPUTPORT_PARAM g_InputPort[Input_Nums];

#ifdef _WORK_ON_PM_
typedef enum
{
#if PANEL_TTL
    E_PNL_SUPPORTED_LPLL_TTL_75to150MHz,          //0
    E_PNL_SUPPORTED_LPLL_TTL_40to75MHz,          //1
    E_PNL_SUPPORTED_LPLL_TTL_25to40MHz,          //2
#elif PANEL_LVDS_1CH
    E_PNL_SUPPORTED_LPLL_LVDS_1ch_1P_75to150MHz,          //3
#elif PANEL_LVDS_2CH
    E_PNL_SUPPORTED_LPLL_LVDS_2ch_1P_150to300MHz,          //4
    E_PNL_SUPPORTED_LPLL_LVDS_2ch_2P_75to150MHz,            //5
#elif PANEL_LVDS_4CH
    E_PNL_SUPPORTED_LPLL_LVDS_4ch_1P_300to300MHz,          //6
    E_PNL_SUPPORTED_LPLL_LVDS_4ch_2P_150to300MHz,          //7
#elif PANEL_LVDS_8CH
    E_PNL_SUPPORTED_LPLL_LVDS_8ch_2P_150to300MHz,          //8
#elif PANEL_VBY1_1CH_10Bit
    E_PNL_SUPPORTED_LPLL_VBY1_1ch_10bit_1P_37_5to75MHz,          //9
#elif PANEL_VBY1_1CH_8Bit
    E_PNL_SUPPORTED_LPLL_VBY1_1ch_8bit_1P_37_5to75MHz,          //10
#elif PANEL_VBY1_2CH_10Bit
    E_PNL_SUPPORTED_LPLL_VBY1_2ch_10bit_1P_75to150MHz,          //11
#elif PANEL_VBY1_2CH_8Bit
    E_PNL_SUPPORTED_LPLL_VBY1_2ch_8bit_1P_75to150MHz,          //12
#elif PANEL_VBY1_4CH_10Bit
    E_PNL_SUPPORTED_LPLL_VBY1_4ch_10bit_1P_150to300MHz,          //13
    E_PNL_SUPPORTED_LPLL_VBY1_4ch_10bit_2P_75to150MHz,          //14
#elif PANEL_VBY1_4CH_8Bit
    E_PNL_SUPPORTED_LPLL_VBY1_4ch_8bit_1P_150to300MHz,          //15
    E_PNL_SUPPORTED_LPLL_VBY1_4ch_8bit_2P_75to150MHz,          //16
#elif PANEL_VBY1_8CH_10Bit
    E_PNL_SUPPORTED_LPLL_VBY1_8ch_10bit_2P_150to300MHz,          //17
#elif PANEL_VBY1_8CH_8Bit
    E_PNL_SUPPORTED_LPLL_VBY1_8ch_8bit_2P_150to300MHz,          //18
#elif PANEL_EDP
    E_PNL_SUPPORTED_LPLL_eDP_High_bit_rate_150to300MHz,          //19
    E_PNL_SUPPORTED_LPLL_eDP_High_bit_rate_75to150MHz,          //20
    E_PNL_SUPPORTED_LPLL_eDP_High_bit_rate_37_5to75MHz,          //21
    E_PNL_SUPPORTED_LPLL_eDP_High_bit_rate_20to37_5MHz,          //22
    E_PNL_SUPPORTED_LPLL_eDP_High_bit_rate_12_5to20MHz,          //23

    E_PNL_SUPPORTED_LPLL_eDP_Reduce_bit_rate_150to300MHz,          //24
    E_PNL_SUPPORTED_LPLL_eDP_Reduce_bit_rate_75to150MHz,          //25
    E_PNL_SUPPORTED_LPLL_eDP_Reduce_bit_rate_37_5to75MHz,          //26
    E_PNL_SUPPORTED_LPLL_eDP_Reduce_bit_rate_20to37_5MHz,          //27
    E_PNL_SUPPORTED_LPLL_eDP_Reduce_bit_rate_12_5to20MHz,          //28
#endif
    E_PNL_SUPPORTED_LPLL_MAX,          //29
} E_PNL_SUPPORTED_LPLL_TYPE;

typedef struct
{
    MS_U8  address;
    MS_U16 value;
    MS_U16 mask;
}TBLStruct,*pTBLStruct;

code RegTbl2Type tblMIU[] =
{
    {REG_10123A, 0x0000, 0x8000},
    {REG_101240, 0x0100, 0x0100},  //enable flow control0
    {REG_101254, 0x8007, 0xFFFF},  //set BDMA flow control0
};

code RegUnitType tblInit[]=
{
    //=========================================================================
#if 1
    // disable GOP to IP
    {SCL0_00_0A, BIT3},
    {SCL0_00_0B, BIT3},

    {SCL0_10_21, BIT6 | BIT3 | BIT2 | BIT1}, // enable auto-htotal

    //{SC0_28, BIT5}, // setup clock mode
//old    {SC0_49, 0xAF},
#endif

///////////Overdrive Initial Settings/////////
#if ENABLE_RTE
    {SCL0_00_80, 0x13},// od L/R common write
    {SCL0_16_32, 0x10},// r thrd
    {SCL0_16_34, 0x30},// r length
    {SCL0_16_35, 0x40},
    {SCL0_16_36, 0x0A},
    {SCL0_16_38, 0x30},
    {SCL0_16_39, 0x40},
    {SCL0_16_22, 0x00},
    {SCL0_16_23, 0x0F},// user weight
    {SCL0_16_74, 0xFF},// w cnt
    {SCL0_16_9A, 0x00},
    {SCL0_16_9B, 0x00},
    {SCL0_16_20, 0x00},
    {SCL0_16_21, 0x00},
    {SCL0_16_3E, 0x00},
    {SCL0_16_3F, 0x85},
    {SCL0_16_42, 0x18},
    {SCL0_16_43, 0x12},
    {SCL0_16_44, 0x0C},
    {SCL0_16_45, 0x06},
    {SCL0_16_46, 0x17},
    {SCL0_16_47, 0x14},
    {SCL0_16_48, 0x11},
    {SCL0_16_49, 0x8E},
    {SCL0_16_4E, 0x18},
    {SCL0_16_4F, 0x02},
    {SCL0_16_64, 0x00},
    {SCL0_16_50, 0x0F},
    {SCL0_16_51, 0x0F},
    {SCL0_16_52, 0x02},
    {SCL0_16_53, 0x0F},
    {SCL0_16_56, 0x40},
    {SCL0_16_57, 0x1E},
    {SCL0_16_58, 0x1C},
    {SCL0_16_59, 0x00},
    {SCL0_16_5A, 0x03},
    {SCL0_16_5B, 0x03},
    {SCL0_16_5C, 0x00},
    {SCL0_16_5D, 0x10},
    {SCL0_16_5E, 0x80},//v range
    {SCL0_16_5F, 0x92},//h range
    {SCL0_16_D9, 0x00},//h range
    {SCL0_16_62, 0x80},//fitch number  h size
    {SCL0_16_63, 0x42},//h range
    {SCL0_16_B0, 0xE0},//fitch number  h size
    {SCL0_16_B1, 0x01},//h range
#endif
    {REG_14061D, PanelDClkDelay}, // DE delay, clock delay for TTL output
    {REG_140600, /*(PanelInvDClk&BIT4)can't find 20130410*/(PanelInvHSync&BIT5)|(PanelInvDE&BIT7)|(PanelInvVSync&BIT6)},
//old               |(PanelDualPort&BIT1)|(PanelSwapPort&BIT0)},

    //{REG_3089, PanelDualPort&BIT4},can't find 20130410
    {REG_140600, LVDS_TI_BitModeSel<<2},
    //{REG_3092, 0x00},can't find 20130410
    {REG_140620, 0x55},
    {REG_140621, 0x55},
    {REG_140622, 0x55},
    {REG_140623, 0x55},
    {REG_140624, 0x55},
    {REG_140625, 0x55},
    {REG_14064E, 0x3F},
    {REG_140650, BIT0},
/////LPLL parameter init
    {REG_14050A, 0xFF},
    {REG_14050B, 0xFF},
    {REG_14050E, 0x02},
    {REG_14050D, 0x00},
    {REG_140512, 0x02},
    {REG_140513, 0x00},
    {REG_140514, 0x00},
    {REG_140515, 0xF0},
//old    {SC0_6C, 0x90}, // short line tune coef. // 120928 coding, modified default value 0x70 to 0x90

    // Clock generator control 0xD0~0xDB
//old    {SC0_D0, 0x00},
    //{REG_30AA, 0x00}, // BK0_D0[0] Rose <--> REG_30AA[0] CHIP_FAMILY_TSUML
    //{REG_1ED1, BIT7}, // setup output pll & master pll  //// 20071219 RD Anson suggest for B1/B2/Goya enable Bit7 fixed line clock in 172Mhz
    //{REG_1ED1, BIT2}, //Jison110311 move to mcuSetSystemSpeed()
    // Master PLL
    //{MPLL_M, 0x6F},//0x72:12MHz // set master pll as 215 MHz & drive current
    //{REG_1ED2, 0x0F}, //Jison110311 move to mcuSetSystemSpeed()
//old    {REG_1ED3, 0x40},
    #if PanelLVDS
//old    {REG_1ED4, 0x03},
    #else
//old    {REG_1ED4, 0x03},
    #endif

    // Spectrum control
#if !defined(_SSC_BY_OUTPUT_DCLK_)  // the LPLL related settings are located in mStar_SetPanelTiming().
    {REG_14052E, LOBYTE(SSC_STEP)},
    {REG_14052F, HIBYTE(SSC_STEP)}, //bit8~bit4 have data, TSUMT should use _SSC_BY_OUTPUT_DCLK_
    {REG_140530, LOBYTE(SSC_SPAN)},
    {REG_140531, HIBYTE(SSC_SPAN)},
    {REG_14051B, LPLL_SSC|0x03},
#endif

     //MRW Init Settings
//old    {SC6_01, 0x00}, //can be deleted
//old    {SC6_02, 0x14}, //[2]IPM CLK [4]DB_EN
//old    {SC6_03, 0x00},

//old    {SC6_1A, 0x02}, //OPM CLK
//old    {SC6_1B, 0x00}, //can be deleted


//old    {SC6_06 ,0x40},    //[7:0]write  MIU unit of N-data per time ,initial only.
//old    {SC6_07 ,0x40},    //[7:0]IPM Write high priority thrd
//old    {SC6_08 ,0x40},    //[7:0]Read MIU unit of N-data per time ,initial only.
//old    {SC6_09 ,0x40},    //[7:0]IPM Read high priority thrd
//old    {SC6_0A ,0x40},
//old    {SC6_0B ,0x40},    //reg_ipr_rreq_hpri

//old    {SC6_0C ,0x28},    //reg_ipm_arb_lvl_thrd
//old    {SC6_0D ,0x28},    //reg_ipm_arb_req_length
//old    {SC6_0E ,0xC8},    //reg_opm_arb_lvl_thrd
//old    {SC6_0F ,0x28},    //reg_opm_arb_req_length

//old    {SC6_10 ,0x28},    //reg_ipr_arb_lvl_thrd
//old    {SC6_11 ,0x28},    //reg_ipr_arb_req_length

//old    {SC6_14 ,0x00},    //[2:0] memory config:3'000: YG, CB, CR 3'b001: Y, C 3'b010: YG only 3'b100: YG, CB, CR, M 'b101: Y, C, M 3'b110: Y, M
//old    {SC6_15 ,0x00},    //IPM channel
//old    {SC6_16 ,0x00},    //OPM channel
//old    {SC6_17 ,0x00},    //IPR channel

//old    {SC6_C0 ,0x60},    //REG_MAX_ADDR
//old    {SC6_C1 ,0x3d},    //REG_MAX_ADDR
//old    {SC6_C2 ,0x08},    //REG_MAX_ADDR
//old    {SC6_C3 ,0x00},    //REG_MAX_ADDR
//old    {SC6_C4 ,0x00},    //REG_MIN_ADDR
//old    {SC6_C5 ,0x00},    //REG_MIN_ADDR
//old    {SC6_C6 ,0x00},    //REG_MIN_ADDR
//old    {SC6_C7 ,0x00},    //REG_MIN_ADDR
//old    {SC6_C8 ,0x80},    //REG_MAX_ADDR_M
//old    {SC6_C9 ,0xfc},    //REG_MAX_ADDR_M
//old    {SC6_CA ,0x0a},    //REG_MAX_ADDR_M
//old    {SC6_CB ,0x00},    //REG_MAX_ADDR_M
//old    {SC6_CC ,0x60},    //REG_MIN_ADDR_M
//old    {SC6_CD ,0x3d},    //REG_MIN_ADDR_M
//old    {SC6_CE ,0x08},    //REG_MIN_ADDR_M
//old    {SC6_CF,0x00},     //REG_MIN_ADDR_M
//old    {SCC_42,0x00},  //disable DNR before IPR setting is ready
#if 0
    {SC1_CC,0xE5},
#endif
    // R2Y Sel
//old    {SC7_41, 0x11}, //Main & Sub window RGB to YCbCr equation selection 0: coefficient(RGB range 16~235), BT601. 1 : coefficient(RGB range 0~255), BT601

//old    {SC8_30, 0xFF}, //reg_gamma_r_max_base0
//old    {SC8_31, 0x03}, //
//old    {SC8_34, 0xFF}, //reg_gamma_g_max_base0
//old    {SC8_35, 0x03}, //
//old    {SC8_38, 0xFF}, //reg_gamma_b_max_base0
//old    {SC8_39, 0x03}, //

    // Sub window init setting
//old    {SCB_28, 0x02}, // SCB_28[1], sub window init setting, better to set/clr according to sub win enable/disable (SC0_5C[3])


    // Dither control //0x36~0x38

#if PanelDither==6
    #if defined(_6BIT_EN_)  //Steven110126, for Output pixel bits number
//old    {SC0_36, 0x03},
    #else
//old    {SC0_36, 0x01},
    #endif
#else
//old    {SC0_36, 0x00},
#endif
//old    {SC0_37, DITHER_COEFF}, //coefficient
//old    {SC0_38,     DITHER_METHOD}, // dither method

//old    {SC0_BC, BIT2 | BIT5 | BIT7}, // 071026 RD suggest, New added for auto position error
    // display port

    // brightness 0x2A~0x2D
//old    {SC0_2A, BIT0}, // disable birghtness control
    // contrast 0x2E~0x31
//old    {SC0_2E, 0x60}, // enable contrast function and set 0 as center

    // border color 0x32~0x35
//old    {SC0_32, 0},// panel background color

//old    {SC0_8B, 0x19}, // enable auto phase
//old    {SC0_78, 0},//x11}, // enable auto Gain
//old    {SC0_8A, 0x1C}, // enable auto phase

    // VSync Status
//old    {SC0_9B, 0x30},
//old    {SC0_9E, 0x20},

#if 0//USE_HDMI_DVI_INT
    //{REG_29E1, 0xFF}, //Disable HDMI INT_MASK
    //{REG_05C0, 0xFF}, //Disable HDMI INT_MASK
    //{REG_05C1, 0xFF}, //Disable HDMI INT_MASK
#endif

    // interrupt control
//old    {SC0_CA, 0x0F}, // BIT0 for level trigger, BIT1 for priority
//old    {SC0_CE, 0},
//old    {SC0_CF, 0},

//old    {SC0_E6, 3},//1},//10}, // RD suggest 20081008
    //{SC0_E7, 1}, //default=1
//old    {SC0_E5, 0},
//old    {SC0_EB, 0x1E},
    // interlace judgement
    //{SC0_E8, 0}, //0x01} //1:non-interlace default=0
//old    {SC0_E9, BIT7}, // 20071219 RD Anson suggest for B1/B2/Goya, bk0_80~81 don't need to -0x10
   // {0x5E, 0x20},// Coring threshold

    // combo
//old    {SC0_FA, BIT2}, // Need set for combo hv mode ([2] always 1)

    // SAR
    {REG_003A60, 0x20},
    {REG_003A61, 0x20},
    {REG_003A62, SARKEY_EN|CABLE_DET_SAR_EN}, // SAR0-3
    {REG_003A66, CABLE_DET_SAR2_EN}, // SAR4-7 - HDMI cable det

    {REG_000462, 0x00},
    {REG_000463, 0x00},//disable pad pull high
};

code RegUnitType2 tblInitMOD[]=
{
///////////MOD Initial Settings///////////
#if PANEL_LVDS_1CH
    {REG_140402, 0x0010},
    {REG_140604, 0x8000},
    {REG_14064E, 0x00FF},
    {REG_140620, 0x0555},
    {REG_140628, 0xFFFF},
    {REG_14062A, 0xFFFF},
    {REG_14062C, 0xFFFF},
    {REG_14062E, 0xFFFF},
    {REG_140630, 0x0000},
    {REG_140632, 0x0000},
    {REG_140600, 0x0001},
    {REG_14060A, 0x2000},
    {REG_140650, 0x0000},
    {REG_14061E, 0xE400},
    {REG_140660, 0x8000},
    {REG_140678, 0x0000},
    {REG_1406A0, 0x0000},
    {REG_1406A2, 0x0000},
    {REG_140780, 0x00FF},
#elif PANEL_LVDS_2CH
    {REG_140604, 0x8000},
    {REG_14064E, 0x00FF},
    {REG_140620, 0x5555},
    {REG_140622, 0x0055},
    {REG_140628, 0xFFFF},
    {REG_14062A, 0xFFFF},
    {REG_14062C, 0xFFFF},
    {REG_14062E, 0xFFFF},
    {REG_140630, 0x0000},
    {REG_140632, 0x0000},
    {REG_140600, 0x0201},
    {REG_14060A, 0x6008},
    {REG_140650, 0x0000},
    #if MST9U_ASIC_1P
    {REG_14061E, 0xE400},
    #else //MST9U_ASIC_2P
    {REG_14061E, 0xE800},
    #endif
    {REG_140660, 0x8000},
    {REG_1406A0, 0x0000},
    {REG_1406A2, 0x0000},
    {REG_140780, 0x00FF},
#elif PANEL_LVDS_4CH
    {REG_140604, 0x8000},
    {REG_14064E, 0x00FF},
    {REG_140620, 0x5555},
    {REG_140622, 0x5555},
    {REG_140624, 0x5555},
    {REG_140628, 0xFFFF},
    {REG_14062A, 0xFFFF},
    {REG_14062C, 0xFFFF},
    {REG_14062E, 0xFFFF},
    {REG_140630, 0x0000},
    {REG_140632, 0x0000},
    {REG_140600, 0x0601},
    {REG_14060A, 0x2008},
    {REG_140650, 0x0000},
    {REG_14061E, 0x2000},
    {REG_140660, 0x8000},
    {REG_140678, 0x2600},
    {REG_1406A0, 0x0000},
    {REG_1406A2, 0x0000},
    {REG_140780, 0x00FF},
    {REG_140782, 0x5555},
    {REG_140784, 0x5555},
    {REG_140786, 0x5555},
#elif PANEL_LVDS_8CH
    {REG_140604, 0x8000},
    {REG_14064E, 0x00FF},
    {REG_140620, 0x5555},
    {REG_140622, 0x5555},
    {REG_140624, 0x5555},
    {REG_140628, 0xFFFF},
    {REG_14062A, 0xFFFF},
    {REG_14062C, 0xFFFF},
    {REG_14062E, 0xFFFF},
    {REG_140630, 0x0000},
    {REG_140632, 0x0000},
    {REG_140600, 0x0601},
    {REG_14060A, 0x2000},
    {REG_140650, 0x0000},
    {REG_14061E, 0xE400},
    {REG_140660, 0x8000},
    {REG_140678, 0x2600},
    {REG_1406A0, 0x0000},
    {REG_1406A2, 0x0000},
    {REG_140780, 0x00FF},
    {REG_140782, 0x5555},
    {REG_140784, 0x5555},
    {REG_140786, 0x5555},
#elif PANEL_VBY1_1CH_8Bit
    {REG_140402, 0x0011},
    {REG_14064E, 0x00FF},
    {REG_140624, 0x0200},
    {REG_140600, 0x0001},
    {REG_14060A, 0x2000},
    {REG_140650, 0x0000},
    {REG_14061E, 0xD800},
    {REG_140660, 0xD000},
    {REG_140678, 0x0000},
    {REG_1406A0, 0x0000},
    {REG_1406A2, 0x0000},
    {REG_140676, 0xFFFF},
    {REG_1406C0, 0xD000},
    {REG_1406C0, 0xD330},
    {REG_1406C0, 0xD320},
    {REG_1406C2, 0x8F3F},
    {REG_1406D0, 0x2000},
    {REG_1406C4, 0xB020},
    {REG_1406C0, 0x0F56},
    {REG_1406C0, 0x0FD6},
    {REG_1406C0, 0x0F96},
    {REG_1406C0, 0x0FB6},
    {REG_1406C0, 0x0FBE},
    {REG_1406C0, 0x0FAE},
    {REG_140780, 0x00FF},
#elif PANEL_VBY1_1CH_10Bit
    {REG_140402, 0x0011},
    {REG_14064E, 0x00FF},
    {REG_140624, 0x0200},
    {REG_140600, 0x0001},
    {REG_14060A, 0x2000},
    {REG_140650, 0x0000},
    {REG_14061E, 0xD800},
    {REG_140660, 0xD000},
    {REG_140678, 0x0000},
    {REG_1406A0, 0x0000},
    {REG_1406A2, 0x0000},
    {REG_140676, 0xFFFF},
    {REG_1406C0, 0xD000},
    {REG_1406C0, 0xD330},
    {REG_1406C0, 0xD320},
    {REG_1406C2, 0x8F3F},
    {REG_1406D0, 0x2000},
    {REG_1406C4, 0xA020},
    {REG_1406C0, 0x0F56},
    {REG_1406C0, 0x0FD6},
    {REG_1406C0, 0x0F96},
    {REG_1406C0, 0x0FB6},
    {REG_1406C0, 0x0FBE},
    {REG_1406C0, 0x0FAE},
    {REG_140780, 0x00FF},
#elif PANEL_VBY1_2CH_8Bit
    {REG_140402, 0x0011},
    {REG_14064E, 0x00FF},
    {REG_140624, 0x0500},
    {REG_140600, 0x0201},
    {REG_14060A, 0x2000},
    {REG_140650, 0x0000},
    {REG_14061E, 0xE400},
    {REG_140660, 0xd000},
    {REG_140678, 0x0000},
    {REG_1406A0, 0x0000},
    {REG_1406A2, 0x0000},
    {REG_140676, 0xFFFF},
    {REG_1406C0, 0xD000},
    {REG_1406C0, 0xD330},
    {REG_1406C0, 0xD320},
    {REG_1406C2, 0x8F3F},
    {REG_1406D0, 0x2000},
    {REG_1406C4, 0xB020},
    {REG_1406C0, 0x0F56},
    {REG_1406C0, 0x0FD6},
    {REG_1406C0, 0x0F96},
    {REG_1406C0, 0x0FB6},
    {REG_1406C0, 0x0FBE},
    {REG_1406C0, 0x0FAE},
    {REG_140780, 0x00FF},
#elif PANEL_VBY1_2CH_10Bit
    {REG_140402, 0x0011},
    {REG_14064E, 0x00ff},
    {REG_140624, 0x0500},
    {REG_140600, 0x0201},
    {REG_14060A, 0x2000},
    {REG_140650, 0x0000},
    {REG_14061E, 0xE400},
    {REG_140660, 0xd000},
    {REG_140678, 0x0000},
    {REG_1406A0, 0x0000},
    {REG_1406A2, 0x0000},
    {REG_140676, 0xFFFF},
    {REG_1406C0, 0xD000},
    {REG_1406C0, 0xD330},
    {REG_1406C0, 0xD320},
    {REG_1406C2, 0x8F3F},
    {REG_1406D0, 0x2000},
    {REG_1406C4, 0xA020},
    {REG_1406C0, 0x0F56},
    {REG_1406C0, 0x0FD6},
    {REG_1406C0, 0x0F96},
    {REG_1406C0, 0x0FB6},
    {REG_1406C0, 0x0FBE},
    {REG_1406C0, 0x0FAE},
    {REG_140780, 0x00FF},
#elif PANEL_VBY1_4CH_8Bit
    {REG_140402, 0x0011},
    {REG_14064E, 0x00ff},
    {REG_140624, 0x5500},
    {REG_140600, 0x0601},
    {REG_14060A, 0x2000},
    {REG_140650, 0x0000},
    {REG_14061E, 0xe400},
    {REG_140660, 0xd000},
    {REG_140678, 0x0400},
    {REG_1406A0, 0x0000},
    {REG_1406A2, 0x0000},
    {REG_140676, 0xffff},
    {REG_1406C0, 0xd000},
    {REG_1406C0, 0xd330},
    {REG_1406C0, 0xd320},
    {REG_1406C2, 0x8f3f},
    {REG_1406D0, 0x2000},
    {REG_1406C4, 0xf020},
    {REG_1406C0, 0x0f56},
    {REG_1406C0, 0x0fd6},
    {REG_1406C0, 0x0f96},
    {REG_1406C0, 0x0fb6},
    {REG_1406C0, 0x0fbe},
    {REG_1406C0, 0x0fae},
    {REG_140780, 0x01ff},
    {REG_140654, 0x00f0},
#elif PANEL_VBY1_4CH_10Bit
    {REG_140402, 0x0011},
    {REG_14064E, 0x00FF},
    {REG_140624, 0x5500},
    {REG_140600, 0x0601},
    {REG_14060A, 0x2000},
    {REG_140650, 0x0000},
    {REG_14061E, 0xE400},
    {REG_140660, 0xD000},
    {REG_140678, 0x0400},
    {REG_1406A0, 0x0000},
    {REG_1406A2, 0x0000},
    {REG_140676, 0xFFFF},
    {REG_1406C0, 0xD000},
    {REG_1406C0, 0xD330},
    {REG_1406C0, 0xD320},
    {REG_1406C2, 0x8F3F},
    {REG_1406D0, 0x2000},
    {REG_1406C4, 0xe020},
    {REG_1406C0, 0x0F56},
    {REG_1406C0, 0x0FD6},
    {REG_1406C0, 0x0F96},
    {REG_1406C0, 0x0FB6},
    {REG_1406C0, 0x0FBE},
    {REG_1406C0, 0x0FAE},
    {REG_140780, 0x01FF},
    {REG_140654, 0x00F0},
#elif PANEL_VBY1_8CH_8Bit
    {REG_140402, 0x0001},
    {REG_14064E, 0x00FF},
    {REG_140620, 0x5555},
    {REG_140622, 0x5555},
    {REG_140624, 0x5555},
    {REG_140626, 0x5555},
    {REG_140600, 0x0601},
    {REG_14060A, 0x2000},
    {REG_140650, 0x0000},
    {REG_14061E, 0xE400},
    {REG_140660, 0xD000},
    {REG_140678, 0x0400},
    {REG_1406A0, 0x0000},
    {REG_1406A2, 0x0000},
    {REG_140676, 0xFFFF},
    {REG_1406C0, 0xD000},
    {REG_1406C0, 0xD330},
    {REG_1406C0, 0xD320},
    {REG_1406C2, 0x8F3F},
    {REG_1406D0, 0x2000},
    {REG_1406C4, 0xF020},
    {REG_1406C0, 0x0F56},
    {REG_1406C0, 0x0FD6},
    {REG_1406C0, 0x0F96},
    {REG_1406C0, 0x0FB6},
    {REG_1406C0, 0x0FBE},
    {REG_1406C0, 0x0FAE},
    {REG_140780, 0x01FF},
    {REG_140782, 0x5555},
    {REG_140784, 0x5555},
    {REG_140786, 0x5555},
    {REG_140654, 0x00F0},
    {REG_1407B2, 0x000F},
#elif PANEL_VBY1_8CH_10Bit
    {REG_140402, 0x0001},
    {REG_14064E, 0x00FF},
    {REG_140620, 0x0000},
    {REG_140622, 0x0000},
    {REG_140624, 0x5500},
    {REG_140626, 0x0000},
    {REG_140600, 0x0601},
    {REG_14060A, 0x6000},
    {REG_14061E, 0xE400},
    {REG_140660, 0xD000},
    {REG_140678, 0x0400},
    {REG_1406A0, 0x0000},
    {REG_1406A2, 0x0000},
    {REG_140676, 0xFFFF},
    {REG_1406C0, 0xD000},
    {REG_1406C0, 0xD330},
    {REG_1406C0, 0xD320},
    {REG_1406C2, 0x8F3F},
    {REG_1406D0, 0x2000},
    {REG_1406C4, 0xE020},
    {REG_1406C0, 0x0F56},
    {REG_1406C0, 0x0FD6},
    {REG_1406C0, 0x0F96},
    {REG_1406C0, 0x0FB6},
    {REG_1406C0, 0x0FBE},
#if PANEL_VBY1_HW_MODE
    {REG_1406C0, 0x0AAE},
#else
    {REG_1406C0, 0x0FAE},
#endif
    {REG_140780, 0x0118},
    {REG_140782, 0x0055},
    {REG_140784, 0x0000},
    {REG_140786, 0x0000},
    {REG_14069A, 0xF000},
    {REG_14069C, 0x00FF},
    {REG_140654, 0x0000},
    {REG_1407B2, 0x000F},
    {REG_1407A0, 0x0FFF},
    {REG_1407AA, 0x00FF},
    {REG_1407A2, 0xFFFF},
    {REG_1407A4, 0xFFFF},
    {REG_1407A6, 0xFFFF},
    {REG_1407A8, 0xFFFF},
    {REG_1407CA, 0xC000},
    {REG_1407D4, 0x0000},
//Analog Settings
    {REG_140644, 0x00F0},
    {REG_140652, 0x0000},
    {REG_140654, 0x00F0},
    {REG_1407AA, 0x000F},
    {REG_1407B2, 0x000F},
    {REG_1407B4, 0x0000},
    {REG_1407A0, 0x0FFF},
    {REG_1407A2, 0x0000},
    {REG_1407A4, 0x0000},
    {REG_1407A6, 0x0000},
    {REG_1407A8, 0x0000},
#elif PANEL_EDP
    {REG_140620, 0x0000},
    {REG_140622, 0x0000},
    {REG_140624, 0x0000},
    {REG_140626, 0x0000},
    {REG_14064E, 0x00FF},
    {REG_140650, 0x0000},
    {REG_140654, 0x00F0},
    {REG_1406B2, 0x000F},
    {REG_140678, 0x0000},
    {REG_140660, 0xD000},
    {REG_140714, 0x0003},
    {REG_140780, 0x01FF},
    {REG_140782, 0x5555},
    {REG_140784, 0x5555},
    {REG_140786, 0x5555},
    {REG_1406A0, 0x0000},
    {REG_1406A2, 0x0000},
#endif

    {REG_140702, (LVDS_CH_D_SWAP<<12)|(LVDS_CH_C_SWAP<<8)|(LVDS_CH_B_SWAP<<4)|(LVDS_CH_A_SWAP)},
    {REG_140704, (LVDS_CH_H_SWAP<<12)|(LVDS_CH_G_SWAP<<8)|(LVDS_CH_F_SWAP<<4)|(LVDS_CH_E_SWAP)},
    {REG_14060B, BIT5|(PanelSwapMLSB&BIT2)|(BIT3&PANEL_SWAP_PN)|(BIT6&LVDS_TIMode)},
    {REG_140603, (PanelSwapOddRB&BIT2)|(PanelSwapEvenRB&BIT5)|(PanelSwapOddML&BIT2)|(PanelSwapEvenML&BIT3)},
#if PANEL_VBY1()
    {REG_140646, (PANEL_SWAP_PN?0xFFFF:0x0000)},//PN swap for VBY1
    {REG_140648, (PANEL_SWAP_PN?0x00FF:0x0000)},//PN swap for VBY1
    {REG_1407AE, (PANEL_SWAP_PN?0xFFFF:0x0000)},//PN swap for VBY1
    {REG_1407B0, (PANEL_SWAP_PN?(PANEL_SWAP_PN_LOCKN?0x00FF:0x003F):(PANEL_SWAP_PN_LOCKN?0x00C0:0x0000))},//PN swap for VBY1
#endif
#if PANEL_OUTPUT_FMT_4B
    {REG_140402, 0x8001},
    {REG_140406, 0x4000},
    {REG_140702, 0x6240},
    {REG_140704, 0x7351},
#endif

};

code RegUnitType tb_3D_DITHER_TABLE[] =
{
    {SC9_D0, 0x19},
    {SC9_D1, 0x00},
    {SC9_D2, 0x99},
    {SC9_D3, 0x27},
    {SC9_F4, 0x8D},
    {SC9_F5, 0x63},
    {SC9_F6, 0x9C},
    {SC9_F7, 0x4E},
    {SC9_F8, 0x4B},
    {SC9_F9, 0x93},
    {SC9_FA, 0x39},
    {SC9_FB, 0x0C},

    {SC0_00, 0x00},
};

/* @brief Enable or Disable clock gating.
 * @param enableGating  (_ENABLE ) to stop clocking;
 *                      (_DISABLE) to send clocks.
 */
void msDrvEnableClockGating(BOOL bEnableGating)
{
    if( bEnableGating == _ENABLE )
    {
        msWriteByteMask(REG_120F01, _BIT4|_BIT3|_BIT2|_BIT1|_BIT0, _BIT4|_BIT3|_BIT2|_BIT1|_BIT0);
        ////reg_ckg_sc_l0_gate_en,reg_ckg_sc_l1_gate_en,reg_ckg_sc_r0_gate_en,reg_ckg_sc_r1_gate_en,reg_ckg_gfx_gate_en
        msWriteByteMask(REG_120F05, _BIT0, _BIT0);//reg_ckg_miu_256b
        msWriteByteMask(REG_120F06, _BIT0, _BIT0);//reg_ckg_miu
        msWriteByteMask(REG_120F07, _BIT0, _BIT0);//reg_ckg_uart0
        msWriteByteMask(REG_120F08, _BIT0, _BIT0);//reg_ckg_spi
        msWriteByteMask(REG_120F09, _BIT0, _BIT0);//reg_ckg_miic0
        msWriteByteMask(REG_120F0A, _BIT0, _BIT0);//reg_ckg_miic1
        msWriteByteMask(REG_120F0B, _BIT0, _BIT0);//reg_ckg_r2
        msWriteByteMask(REG_120F0C, _BIT0, _BIT0);//reg_ckg_r2_t3d
        msWriteByteMask(REG_120F0D, _BIT0, _BIT0);//reg_ckg_odclk
        msWriteByteMask(REG_120F0E, _BIT0, _BIT0);//reg_ckg_vby1_fifo
        msWriteByteMask(REG_120F0F, _BIT0, _BIT0);//reg_ckg_mod_sr_wclk
        msWriteByteMask(REG_120F10, _BIT0, _BIT0);//reg_ckg_mod_sr_rclk
        msWriteByteMask(REG_120F11, _BIT0, _BIT0);//reg_ckg_sosd
        msWriteByteMask(REG_120F12, _BIT0, _BIT0);//reg_ckg_fclk
        msWriteByteMask(REG_120F16, _BIT0, _BIT0);//reg_ckg_fodclk
        msWriteByteMask(REG_120F17, _BIT0, _BIT0);//reg_ckg_osdc
        msWriteByteMask(REG_120F18, _BIT0, _BIT0);//reg_ckg_idclk_l0_f0
        msWriteByteMask(REG_120F19, _BIT0, _BIT0);//reg_ckg_idclk_l0_f1
        msWriteByteMask(REG_120F1A, _BIT0, _BIT0);//reg_ckg_idclk_l0_f2
        msWriteByteMask(REG_120F1B, _BIT0, _BIT0);//reg_ckg_idclk_l1_f1
        msWriteByteMask(REG_120F1C, _BIT0, _BIT0);//reg_ckg_idclk_l1_f2
        msWriteByteMask(REG_120F1D, _BIT0, _BIT0);//reg_ckg_idclk_r0_f1
        msWriteByteMask(REG_120F1E, _BIT0, _BIT0);//reg_ckg_idclk_r0_f2
        msWriteByteMask(REG_120F1F, _BIT0, _BIT0);//reg_ckg_pll_idclk
        msWriteByteMask(REG_120F21, _BIT0, _BIT0);//reg_ckg_ge
        msWriteByteMask(REG_120F23, _BIT0, _BIT0);//reg_ckg_gopg0
        msWriteByteMask(REG_120F24, _BIT0, _BIT0);//reg_ckg_gopg1
        msWriteByteMask(REG_120F25, _BIT0, _BIT0);//reg_ckg_gopg2
        msWriteByteMask(REG_120F26, _BIT0, _BIT0);//reg_ckg_gopg3
        msWriteByteMask(REG_120F28, _BIT0, _BIT0);//reg_ckg_psram0
        msWriteByteMask(REG_120F29, _BIT0, _BIT0);//reg_ckg_psram1
        msWriteByteMask(REG_120F2C, _BIT0, _BIT0);//reg_ckg_miu_rec
        msWriteByteMask(REG_120F2E, _BIT0, _BIT0);//reg_ckg_idclk_r1_f1
        msWriteByteMask(REG_120F2F, _BIT0, _BIT0);//reg_ckg_idclk_r1_f2
        msWriteByteMask(REG_120F30, _BIT0, _BIT0);//reg_ckg_sosd_ft
        msWriteByteMask(REG_120F47, _BIT0, _BIT0);//reg_ckg_misc
        msWriteByteMask(REG_120F64, _BIT4, _BIT4);//reg_ckg_ficlk_f1_l0
        msWriteByteMask(REG_120F65, _BIT4|_BIT0, _BIT4|_BIT0);//reg_ckg_ficlk_f2_l0, reg_ckg_ficlk_f2_l0
        msWriteByteMask(REG_120F66, _BIT4|_BIT0, _BIT4|_BIT0);//reg_ckg_fmclk_f2_l0,reg_ckg_moclk_od_sram0_2nd_l0
        msWriteByteMask(REG_120F67, _BIT4|_BIT0, _BIT4|_BIT0);//reg_ckg_moclk_od_sram0_l0,reg_ckg_moclk_od_sram1_l0
        msWriteByteMask(REG_120F68, _BIT4|_BIT0, _BIT4|_BIT0);//reg_ckg_moclk_od_sram2_l0,reg_ckg_moclk_t3d_sram_l0
        msWriteByteMask(REG_120F69, _BIT4|_BIT0, _BIT4|_BIT0);//reg_ckg_moclk_ga_sram_l0,reg_ckg_moclk_xvycc_sram_l0
        msWriteByteMask(REG_120F6A, _BIT0, _BIT0);//reg_ckg_moclk_lut_sram_l0
        msWriteByteMask(REG_120F6B, _BIT0, _BIT0);//reg_ckg_misc_d
        msWriteByteMask(REG_120F84, _BIT4, _BIT4);//reg_ckg_ficlk_f1_l1
        msWriteByteMask(REG_120F89, _BIT0, _BIT0);//reg_ckg_ficlk_f2_l1
        msWriteByteMask(REG_120F85, _BIT4, _BIT4);//reg_ckg_ficlk2_f2_l1
        msWriteByteMask(REG_120F86, _BIT0, _BIT0);//reg_ckg_fmclk_f2_l1
        msWriteByteMask(REG_120F87, _BIT4|_BIT0, _BIT4|_BIT0);//reg_ckg_moclk_od_sram0_l1,reg_ckg_moclk_od_sram1_l1
        msWriteByteMask(REG_120F88, _BIT4|_BIT0, _BIT4|_BIT0);//reg_ckg_moclk_od_sram2_l1,reg_ckg_moclk_t3d_sram_l1
        msWriteByteMask(REG_120F89, _BIT4|_BIT0, _BIT4|_BIT0);//reg_ckg_moclk_ga_sram_l1,reg_ckg_moclk_xvycc_sram_l1
        msWriteByteMask(REG_120F8A, _BIT0, _BIT0);//reg_ckg_moclk_lut_sram_l1
        msWriteByteMask(REG_120FA4, _BIT0, _BIT0);//reg_ckg_ficlk_f1_r0
        msWriteByteMask(REG_120FA5, _BIT4|_BIT0, _BIT4|_BIT0);//reg_ckg_ficlk_f2_r0,reg_ckg_ficlk2_f2_r0
        msWriteByteMask(REG_120FA6, _BIT4|_BIT0, _BIT4|_BIT0);//reg_ckg_fmclk_f2_r0,reg_ckg_moclk_od_sram0_2nd_r0
        msWriteByteMask(REG_120FA7, _BIT4|_BIT0, _BIT4|_BIT0);//reg_ckg_moclk_od_sram0_r0,reg_ckg_moclk_od_sram1_r0
        msWriteByteMask(REG_120FA8, _BIT4|_BIT0, _BIT4|_BIT0);//reg_ckg_moclk_od_sram2_r0,reg_ckg_moclk_t3d_sram_r0
        msWriteByteMask(REG_120FA9, _BIT4|_BIT0, _BIT4|_BIT0);//reg_ckg_moclk_ga_sram_r0,reg_ckg_moclk_xvycc_sram_r0
        msWriteByteMask(REG_120FAA, _BIT0, _BIT0);//reg_ckg_moclk_lut_sram_r0
        msWriteByteMask(REG_120FC4, _BIT0, _BIT0);//reg_ckg_ficlk_f1_r1
        msWriteByteMask(REG_120FC5, _BIT0, _BIT0);//reg_ckg_ficlk_f2_r1,reg_ckg_ficlk2_f2_r1
        msWriteByteMask(REG_120FC6, _BIT4|_BIT0, _BIT4|_BIT0);//reg_ckg_fmclk_f2_r1,reg_ckg_moclk_od_sram0_2nd_r1
        msWriteByteMask(REG_120FC7, _BIT4|_BIT0, _BIT4|_BIT0);//reg_ckg_moclk_od_sram0_2nd_r1,reg_ckg_moclk_od_sram1_r1
        msWriteByteMask(REG_120FC8, _BIT4|_BIT0, _BIT4|_BIT0);//reg_ckg_moclk_od_sram2_r1,reg_ckg_moclk_t3d_sram_r1
        msWriteByteMask(REG_120FC9, _BIT4|_BIT0, _BIT4|_BIT0);//reg_ckg_moclk_ga_sram_r1,reg_ckg_moclk_xvycc_sram_r1
        msWriteByteMask(REG_120FCA, _BIT4|_BIT0, _BIT4|_BIT0);//reg_ckg_moclk_lut_sram_r1

    }
    else
    {
        msWriteByteMask(REG_120F01, 0x00, _BIT4|_BIT3|_BIT2|_BIT1|_BIT0);
        ////reg_ckg_sc_l0_gate_en,reg_ckg_sc_l1_gate_en,reg_ckg_sc_r0_gate_en,reg_ckg_sc_r1_gate_en,reg_ckg_gfx_gate_en
        msWriteByteMask(REG_120F05, 0x00, _BIT0);//reg_ckg_miu_256b
        msWriteByteMask(REG_120F06, 0x00, _BIT0);//reg_ckg_miu
        msWriteByteMask(REG_120F07, 0x00, _BIT0);//reg_ckg_uart0
        msWriteByteMask(REG_120F08, 0x00, _BIT0);//reg_ckg_spi
        msWriteByteMask(REG_120F09, 0x00, _BIT0);//reg_ckg_miic0
        msWriteByteMask(REG_120F0A, 0x00, _BIT0);//reg_ckg_miic1
        msWriteByteMask(REG_120F0B, 0x00, _BIT0);//reg_ckg_r2
        msWriteByteMask(REG_120F0C, 0x00, _BIT0);//reg_ckg_r2_t3d
        msWriteByteMask(REG_120F0D, 0x00, _BIT0);//reg_ckg_odclk
        msWriteByteMask(REG_120F0E, 0x00, _BIT0);//reg_ckg_vby1_fifo
        msWriteByteMask(REG_120F0F, 0x00, _BIT0);//reg_ckg_mod_sr_wclk
        msWriteByteMask(REG_120F10, 0x00, _BIT0);//reg_ckg_mod_sr_rclk
        msWriteByteMask(REG_120F11, 0x00, _BIT0);//reg_ckg_sosd
        msWriteByteMask(REG_120F12, 0x00, _BIT0);//reg_ckg_fclk
        msWriteByteMask(REG_120F16, 0x00, _BIT0);//reg_ckg_fodclk
        msWriteByteMask(REG_120F17, 0x00, _BIT0);//reg_ckg_osdc
        msWriteByteMask(REG_120F18, 0x00, _BIT0);//reg_ckg_idclk_l0_f0
        msWriteByteMask(REG_120F19, 0x00, _BIT0);//reg_ckg_idclk_l0_f1
        msWriteByteMask(REG_120F1A, 0x00, _BIT0);//reg_ckg_idclk_l0_f2
        msWriteByteMask(REG_120F1B, 0x00, _BIT0);//reg_ckg_idclk_l1_f1
        msWriteByteMask(REG_120F1C, 0x00, _BIT0);//reg_ckg_idclk_l1_f2
        msWriteByteMask(REG_120F1D, 0x00, _BIT0);//reg_ckg_idclk_r0_f1
        msWriteByteMask(REG_120F1E, 0x00, _BIT0);//reg_ckg_idclk_r0_f2
        msWriteByteMask(REG_120F1F, 0x00, _BIT0);//reg_ckg_pll_idclk
        msWriteByteMask(REG_120F21, 0x00, _BIT0);//reg_ckg_ge
        msWriteByteMask(REG_120F23, 0x00, _BIT0);//reg_ckg_gopg0
        msWriteByteMask(REG_120F24, 0x00, _BIT0);//reg_ckg_gopg1
        msWriteByteMask(REG_120F25, 0x00, _BIT0);//reg_ckg_gopg2
        msWriteByteMask(REG_120F26, 0x00, _BIT0);//reg_ckg_gopg3
        msWriteByteMask(REG_120F28, 0x00, _BIT0);//reg_ckg_psram0
        msWriteByteMask(REG_120F29, 0x00, _BIT0);//reg_ckg_psram1
        msWriteByteMask(REG_120F2C, 0x00, _BIT0);//reg_ckg_miu_rec
        msWriteByteMask(REG_120F2E, 0x00, _BIT0);//reg_ckg_idclk_r1_f1
        msWriteByteMask(REG_120F2F, 0x00, _BIT0);//reg_ckg_idclk_r1_f2
        msWriteByteMask(REG_120F30, 0x00, _BIT0);//reg_ckg_sosd_ft
        msWriteByteMask(REG_120F47, 0x00, _BIT0);//reg_ckg_misc
        msWriteByteMask(REG_120F64, 0x00, _BIT4);//reg_ckg_ficlk_f1_l0
        msWriteByteMask(REG_120F65, 0x00, _BIT4|_BIT0);//reg_ckg_ficlk_f2_l0, reg_ckg_ficlk_f2_l0
        msWriteByteMask(REG_120F66, 0x00, _BIT4|_BIT0);//reg_ckg_fmclk_f2_l0,reg_ckg_moclk_od_sram0_2nd_l0
        msWriteByteMask(REG_120F67, 0x00, _BIT4|_BIT0);//reg_ckg_moclk_od_sram0_l0,reg_ckg_moclk_od_sram1_l0
        msWriteByteMask(REG_120F68, 0x00, _BIT4|_BIT0);//reg_ckg_moclk_od_sram2_l0,reg_ckg_moclk_t3d_sram_l0
        msWriteByteMask(REG_120F69, 0x00, _BIT4|_BIT0);//reg_ckg_moclk_ga_sram_l0,reg_ckg_moclk_xvycc_sram_l0
        msWriteByteMask(REG_120F6A, 0x00, _BIT0);//reg_ckg_moclk_lut_sram_l0
        msWriteByteMask(REG_120F6B, 0x00, _BIT0);//reg_ckg_misc_d
        msWriteByteMask(REG_120F84, 0x00, _BIT4);//reg_ckg_ficlk_f1_l1
        msWriteByteMask(REG_120F89, 0x00, _BIT0);//reg_ckg_ficlk_f2_l1
        msWriteByteMask(REG_120F85, 0x00, _BIT4);//reg_ckg_ficlk2_f2_l1
        msWriteByteMask(REG_120F86, 0x00, _BIT0);//reg_ckg_fmclk_f2_l1
        msWriteByteMask(REG_120F87, 0x00, _BIT4|_BIT0);//reg_ckg_moclk_od_sram0_l1,reg_ckg_moclk_od_sram1_l1
        msWriteByteMask(REG_120F88, 0x00, _BIT4|_BIT0);//reg_ckg_moclk_od_sram2_l1,reg_ckg_moclk_t3d_sram_l1
        msWriteByteMask(REG_120F89, 0x00, _BIT4|_BIT0);//reg_ckg_moclk_ga_sram_l1,reg_ckg_moclk_xvycc_sram_l1
        msWriteByteMask(REG_120F8A, 0x00, _BIT0);//reg_ckg_moclk_lut_sram_l1
        msWriteByteMask(REG_120FA4, 0x00, _BIT0);//reg_ckg_ficlk_f1_r0
        msWriteByteMask(REG_120FA5, 0x00, _BIT4|_BIT0);//reg_ckg_ficlk_f2_r0,reg_ckg_ficlk2_f2_r0
        msWriteByteMask(REG_120FA6, 0x00, _BIT4|_BIT0);//reg_ckg_fmclk_f2_r0,reg_ckg_moclk_od_sram0_2nd_r0
        msWriteByteMask(REG_120FA7, 0x00, _BIT4|_BIT0);//reg_ckg_moclk_od_sram0_r0,reg_ckg_moclk_od_sram1_r0
        msWriteByteMask(REG_120FA8, 0x00, _BIT4|_BIT0);//reg_ckg_moclk_od_sram2_r0,reg_ckg_moclk_t3d_sram_r0
        msWriteByteMask(REG_120FA9, 0x00, _BIT4|_BIT0);//reg_ckg_moclk_ga_sram_r0,reg_ckg_moclk_xvycc_sram_r0
        msWriteByteMask(REG_120FAA, 0x00, _BIT0);//reg_ckg_moclk_lut_sram_r0
        msWriteByteMask(REG_120FC4, 0x00, _BIT0);//reg_ckg_ficlk_f1_r1
        msWriteByteMask(REG_120FC5, 0x00, _BIT0);//reg_ckg_ficlk_f2_r1,reg_ckg_ficlk2_f2_r1
        msWriteByteMask(REG_120FC6, 0x00, _BIT4|_BIT0);//reg_ckg_fmclk_f2_r1,reg_ckg_moclk_od_sram0_2nd_r1
        msWriteByteMask(REG_120FC7, 0x00, _BIT4|_BIT0);//reg_ckg_moclk_od_sram0_2nd_r1,reg_ckg_moclk_od_sram1_r1
        msWriteByteMask(REG_120FC8, 0x00, _BIT4|_BIT0);//reg_ckg_moclk_od_sram2_r1,reg_ckg_moclk_t3d_sram_r1
        msWriteByteMask(REG_120FC9, 0x00, _BIT4|_BIT0);//reg_ckg_moclk_ga_sram_r1,reg_ckg_moclk_xvycc_sram_r1
        msWriteByteMask(REG_120FCA, 0x00, _BIT4|_BIT0);//reg_ckg_moclk_lut_sram_r1
    }
}


///////////////////////////////////////////////////////////////////////////////
// <Function>: msDrvCheckVBY1LockN
//
// <Description>: Check LockN value for VBY1 interface
//
// <Parameter>: None
//
///////////////////////////////////////////////////////////////////////////////
void msDrvCheckVBY1LockN(void)
{
#if !PANEL_VBY1_HW_MODE
    if((msReadByte(REG_1407D1)&BIT6)||((msReadByte(REG_1407D1)&BIT7)))
    {
        TimeOutCounter = 100;
        while( TimeOutCounter )
        {
            msWrite2Byte(REG_1406C0, 0x0F56);
            msWrite2Byte(REG_1406C0, 0x0FD6);
            msWrite2Byte(REG_1406C0, 0x0F96);
            msWrite2Byte(REG_1406C0, 0x0FB6);
            msWrite2Byte(REG_1406C0, 0x0FBE);
            Delay1ms(1);
            msWrite2Byte(REG_1406C0, 0x0FAE);
            Delay1ms(1);
            if((!(msReadByte(REG_1407D1)&BIT6))&&(!(msReadByte(REG_1407D1)&BIT7)))
            {
                Delay1ms(1);
                if((!(msReadByte(REG_1407D1)&BIT6))&&(!(msReadByte(REG_1407D1)&BIT7)))
                {
                    break;
                }
            }
        }
        if( TimeOutCounter == 0)
        {
            drvmStar_printMsg("******VBY1 Training Timeout*******");
        }
    }
#else
    if(msReadByte(REG_1407D1)&BIT6)
    {
        msWriteBit(REG_1407B0, (msReadByte(REG_1407B0)&BIT7)?0x00:BIT7 ,BIT7);
        ForceDelay1ms(1);
        msWriteBit(REG_1407B0, (msReadByte(REG_1407B0)&BIT7)?0x00:BIT7 ,BIT7);
    }
    else if(msReadByte(REG_1407D1)&BIT7)
    {
        msWriteBit(REG_1407B0, (msReadByte(REG_1407B0)&BIT6)?0x00:BIT6 ,BIT6);
        ForceDelay1ms(1);
         msWriteBit(REG_1407B0, (msReadByte(REG_1407B0)&BIT6)?0x00:BIT6 ,BIT6);
    }
#endif
}
///////////////////////////////////////////////////////////////////////////////
// <Function>: msTMDSInit
//
// <Description>: HDMI and HDMI audio initial function
//
// <Parameter>: None
//
///////////////////////////////////////////////////////////////////////////////
void msTMDSInit(void)
{
    // DVI power enable
    //_msWriteByteMask(REG_PM_B2, 0, BIT2);         // power on DVI CKIN. (port 0 and port 1) (reg_b2[2])
    // DVI enable
    _msWriteByteMask(REG_1740, BIT0, BIT0);      // [0]: HDMI select
    // 1200301 coding, set EQ to 1
    _msWriteByteMask(REG_17AC, 0xE0, 0xE0); // EQ overwrite enable.
    _msWriteByte(REG_17B1, 0x0A); // EQ strength level
    _msWriteByte(REG_17B2, 0x0A); // EQ strength level
    _msWriteByte(REG_17B3, 0x0A); // EQ strength level
    _msWriteByte(REG_01C7,0x00);                 // DVI phase dac power on for CHIP_FAMILY_TSUML2

    _msWrite2Byte(REG_1606, 0x0A88);// Set compvalue 145Mhz
    _msWrite2Byte(REG_1608, 0x0944);// Set compvalue 72.5Mhz
    _msWrite2Byte(REG_160A, 0x08A2);// Set compvalue 36.25Mhz

    // Audio
    _msWriteByteMask(REG_1712, 0x22, 0xFF); //[3:0]:KP, [7:4]:KM
    _msWriteByteMask(REG_1713, 0x00, 0x60); //[6:5]:KN
    _msWriteByteMask(REG_1715, 0x31, 0xFF); //[B:8]:DDIV, [F:C]:FBDIV

    // Word Boundary setting
    _msWriteByteMask(REG_161D, 0xFF, 0xFF);
    _msWriteByteMask(REG_1621, 0xFF, 0xFF);
    _msWriteByteMask(REG_1625, 0xFF, 0xFF);
    _msWriteByteMask(REG_1629, 0xFF, 0xFF);

    // SPDIF Out Enable
    _msWriteByteMask(REG_1426, 0x02, 0x02);

#if 0 //If support I2S, need these settings
    _msWriteByteMask(REG_0207, 0x03, 0x03); //Enable I2S output pad and AUMCK output pad control
    _msWriteByteMask(REG_1426, 0x05, 0x05); //[2]: I2S out mute OEN, [0]: I2S out OEN
    _msWriteByteMask(REG_174B, 0x04, 0x04); //[2]: I2S main clock 192fs out
#endif

    _msWriteByteMask(REG_1569, 0x00, BIT0);

    _msWrite2Byte(REG_1604, 0x00C0);
    _msWrite2Byte(REG_1438, 0x0010);
    _msWrite2Byte(REG_142A, 0x0017);

    //CTS
    _msWriteByteMask(REG_1730, BIT7, BIT7);  //Enable HDMI CTS-N synthesizer
    _msWriteByteMask(REG_1740, 0x00, 0x06);
    _msWriteByteMask(REG_1706, BIT0, BIT0);
    _msWriteByteMask(REG_1711, BIT7, BIT7);
    _msWriteByteMask(REG_1707, BIT4, BIT4);

    //_msWriteByteMask(REG_0200, 0x02, 0x02);
    _msWriteByteMask(REG_1439, 0xC0, 0xC0);  //Audio volume attenuation control
    _msWriteByteMask(REG_1427, 0x10, 0x10);  //Reset Audio FIFO
    _msWriteByteMask(REG_1427, 0x00, 0x10);

    // Lane 0 delay control
    _msWriteByteMask(REG_1613, 0x0C, 0x0E);

    _msWrite2Byte(REG_16B0, 0x0859); //clock stable IRQ boumdary
    _msWrite2Byte(REG_16B2, 0x145D);  //clock stable IRQ boumdary

    // PLL + Digital CDR (For U01 Demo)
    //_msWriteByteMask(REG_17AD, 0x03, 0x03);
    //_msWriteByteMask(REG_17A8, 0x1B, 0x2F);
    _msWriteByteMask(REG_17CF, 0x80, 0x80);

    _msWriteByteMask(REG_163D, 0x1F, 0x1F);
    _msWrite2Byte(REG_175C, 0xFFFF);  //IRQ Mask
    _msWrite2Byte(REG_175A, 0xFFFF);  //IRQ CLR
    _msWrite2Byte(REG_175A, 0x0000);  //IRQ CLR
    _msWrite2Byte(REG_1418, 0xFFFF);  //IRQ Mask Error Status
    _msWrite2Byte(REG_1508, 0xFF00);  //IRQ Mask HDCP
    _msWrite2Byte(REG_153C, 0x0001);  //Clr IRQ
    _msWriteByteMask(REG_2B19, 0x00, 0x80);  //IRQ bank Mask off

    // Power up Xtal clocks and DVI detection clock for DVI clock detection
    //msWriteWordMask(REG_290D, 0, 0x601);      // HDCP related power
    // DVI PLL power control
    //msWriteWordMask(REG_29C1, 0, 0xFFFF);     // enable DVI0 PLL power
    // DVI slowly update
    //_msWriteByteMask(REG_2853, 0xE3, 0xFF);      // [15]:update Bch slowly; [14:8]: 0x63(100 lines)
    //_msWrite2ByteMask(REG_2854, 0xE3E3, 0xFFFF); // [15:8]: update Rch slowly, [7:0]:update Gch slowly
    // Enable DVI phase accumulator extension and clock stable gate
    //_msWriteByteMask(REG_2801, BIT6|BIT5, BIT6|BIT5); // [6]:Enable of acc8lsb extension ; [5]:Enable of acc8lsb extension select
    //_msWriteByteMask(REG_2841, BIT2, BIT2);      // [2]:Enable the function that clock gen gate signal controlled by TMDS clock stable
    // enable Vsync glitch filter
    //_msWriteByteMask(REG_283C, BIT4, BIT4);      // enable DVI port0 Vsync glitch filter
    // Enable DVI clock tolerance: 4*0.7 = 2.8 MHz
    //_msWriteByteMask(REG_2827, 0x20, 0x70);      //the comp value is reg_comp_tol * 4
    //_msWriteByteMask(REG_2825, 0xF0, 0xF0);      //0x110A25=0xB0, DVI clock detection counter tolerance set to 7
    //_msWriteByte(REG_2852,0x10);                 // [4]: enable auto DVIPLL adjust
}

#if DHDMI_SIMPLAYHD_PATCH // SimplayHD CTS 8-18

void Delay1us(void)
{
    BYTE xdata i; // Add by Jonson 20100917

    if(g_bMcuPMClock)
        return;
    i=5; // 315K // 20: 135K
    while(i--)
    {
        _nop_();
    }
}
#endif // #if DHDMI_SIMPLAYHD_PATCH // SimplayHD CTS 8-18
#if 0
void TMDSISR(void)
{
#if DHDMI_SIMPLAYHD_PATCH // SimplayHD CTS 8-18
    BYTE xdata u8AksvVal = 0;
    BYTE xdata u8HDCPFSM = 0;
    static Bool xdata bflag = FALSE;
    BYTE xdata i_loop;
#endif

    if(!CURRENT_INPUT_IS_TMDS())
    {
        _MEM_MSWRITE_BYTE_MASK(REG_175D, 0x20, 0x20);
        _MEM_MSWRITE_BYTE_MASK(REG_175B, 0x20, 0x20);
        _MEM_MSWRITE_BYTE_MASK(REG_175B, 0x00, 0x20);
        return;
    }

    if(_bit5_(_MEM_MSREAD_BYTE(REG_1757))) // CLK back to stable IRQ
    {
        WORD clk_cnts =0;
        _MEM_MSWRITE_BYTE_MASK(REG_175D, 0x20, 0x20);//Mask
        _MEM_MSWRITE_BYTE_MASK(REG_1543, 0x00, 0x10);//disable overwrite
        _MEM_MSWRITE_BYTE_MASK(REG_1569, 0x01, 0x01);//auto_CLR
        _MEM_MSWRITE_BYTE_MASK(REG_1569, 0x00, 0x01);
        clk_cnts = ((((WORD)_MEM_MSREAD_BYTE((REG_16B6)+1))<<8));
        clk_cnts += _MEM_MSREAD_BYTE((REG_16B6));
        //if (!(_bit2_(_msRegs[REG_0C01]))) // not MHL
        {
            if( clk_cnts > 0x0288 ) //div 1
            {
                _MEM_MSWRITE_BYTE_MASK(REG_17AB, 0x60, 0x60);
                _MEM_MSWRITE_BYTE_MASK(REG_17AA, 0x00, 0x0C);
                _MEM_MSWRITE_BYTE_MASK(REG_17A5, 0x00, 0x0C);
                _MEM_MSWRITE_BYTE_MASK(REG_17AC, 0x01, 0x01); //ICtrl overwrite
                _MEM_MSWRITE_BYTE_MASK(REG_17AA, 0x10, 0xF0); //ICtrl value
            }
            else if( clk_cnts > 0x0144 ) //div 2
            {
                _MEM_MSWRITE_BYTE_MASK(REG_17AB, 0x60, 0x60);
                _MEM_MSWRITE_BYTE_MASK(REG_17AA, 0x04, 0x0C);
                _MEM_MSWRITE_BYTE_MASK(REG_17A5, 0x04, 0x0C);
                _MEM_MSWRITE_BYTE_MASK(REG_17AC, 0x01, 0x01); //ICtrl overwrite
                _MEM_MSWRITE_BYTE_MASK(REG_17AA, 0x20, 0xF0); //ICtrl value

            }
            else if( clk_cnts > 0x00A2 ) //div 4
            {
                _MEM_MSWRITE_BYTE_MASK(REG_17AB, 0x60, 0x60);
                _MEM_MSWRITE_BYTE_MASK(REG_17AA, 0x08, 0x0C);
                _MEM_MSWRITE_BYTE_MASK(REG_17A5, 0x08, 0x0C);
                _MEM_MSWRITE_BYTE_MASK(REG_17AC, 0x01, 0x01); //ICtrl overwrite
                _MEM_MSWRITE_BYTE_MASK(REG_17AA, 0x40, 0xF0); //ICtrl value
            }
            else //div 8
            {
                _MEM_MSWRITE_BYTE_MASK(REG_17AB, 0x60, 0x60);
                _MEM_MSWRITE_BYTE_MASK(REG_17AA, 0x0C, 0x0C);
                _MEM_MSWRITE_BYTE_MASK(REG_17A5, 0x0C, 0x0C);
                _MEM_MSWRITE_BYTE_MASK(REG_17AC, 0x01, 0x01); //ICtrl overwrite
                _MEM_MSWRITE_BYTE_MASK(REG_17AA, 0x40, 0xF0); //ICtrl value
            }
        }

        _MEM_MSWRITE_BYTE_MASK(REG_175B, 0x20, 0x20);
        _MEM_MSWRITE_BYTE_MASK(REG_175B, 0x00, 0x20);
        _MEM_MSWRITE_BYTE_MASK(REG_175D, 0x00, 0x20);
    }
#if DHDMI_SIMPLAYHD_PATCH // SimplayHD CTS 8-18
    if(_bit2_(_MEM_MSREAD_BYTE(REG_1539)))
    {

        _MEM_MSWRITE_BYTE_MASK(REG_1509, 0x84, 0x84);
        _MEM_MSWRITE_BYTE(REG_1539, 0xFF);

        u8HDCPFSM = _MEM_MSREAD_BYTE(REG_1531) & 0x1F;

        if(((u8HDCPFSM !=  0x02) && !((u8HDCPFSM ==  0x03) && (_MEM_MSREAD_BYTE(REG_1518) & BIT7))) && !bflag)
        //if((_MEM_MSREAD_BYTE(REG_1518) & BIT7) && !bflag)
        {
            _MEM_MSWRITE_BYTE_MASK(REG_150F, 0x00, 0xC0);
            _MEM_MSWRITE_BYTE_MASK(REG_150C, 0x14, 0xFF);
            _MEM_MSWRITE_BYTE_MASK(REG_150D, 0, 0x03);
            _MEM_MSWRITE_BYTE(REG_153A, 0x10);
            for(i_loop=0; i_loop<2;i_loop++) {Delay1us();}
            _MEM_MSWRITE_BYTE(REG_153A, 0x04);
            for(i_loop=0; i_loop<2;i_loop++) {Delay1us();}
            u8AksvVal = _MEM_MSREAD_BYTE(REG_1525);
            _MEM_MSWRITE_BYTE_MASK(REG_150F, 0x80, 0xC0);
            _MEM_MSWRITE_BYTE(REG_153A, 0x10);
            for(i_loop=0; i_loop<2;i_loop++) {Delay1us();}
            _MEM_MSWRITE_BYTE(REG_150E, u8AksvVal);
            _MEM_MSWRITE_BYTE(REG_153A, 0x08);
            for(i_loop=0; i_loop<2;i_loop++) {Delay1us();}
            _MEM_MSWRITE_BYTE_MASK(REG_150F, 0x00, 0xC0);
            bflag = TRUE;
        }
        else
        {
            bflag = FALSE;
        }
        _MEM_MSWRITE_BYTE_MASK(REG_1509, 0x00, 0x84);
    }
#endif // #if DHDMI_SIMPLAYHD_PATCH // SimplayHD CTS 8-18
}
#endif
void msDrvmStarInit( void )
{
    BYTE i;

#ifndef MST9U_FPGA
    if (!msPM_StartRCOSCCal()) // calibration here for other IP to use FRO_12M
    {
        drvmStar_printData("CALIBARYION RCOSC_12M FAIL!",0);
    }
    else
    {
        drvmStar_printData("CALIBARYION RCOSC_12M Success!",0);
    }
#endif
    msDrvEnableClockGating(_DISABLE);

    msTMDSInit();

    msDrvIP1SetOutputLockMode(FALSE, SCL0_MAIN/*//tmp*/);//_msWriteByteMask(SC0_02, BIT7, BIT7);
    msDrvIP1SetOutputFreerun(TRUE, SCL0_MAIN/*//tmp*/);

    for( i = 0; i < sizeof( tblInit ) / sizeof( RegUnitType ); i++ )
        msWriteByte( tblInit[i].u16Reg, tblInit[i].u8Value );
    for( i = 0; i < sizeof( tb_3D_DITHER_TABLE ) / sizeof( RegUnitType ); i++ )
        _msWriteByte( tb_3D_DITHER_TABLE[i].u16Reg, tb_3D_DITHER_TABLE[i].u8Value );
#ifndef MST9U_FPGA
    for( i = 0; i < sizeof( tblInitMOD ) / sizeof( RegUnitType2 ); i++ )
        msWrite2Byte( tblInitMOD[i].u16Reg, tblInitMOD[i].u8Value );

#endif
    for( i = 0; i < sizeof( tblMIU ) / sizeof( RegUnitType2 ); i++ )
        msWrite2ByteMask( tblMIU[i].dwReg, tblMIU[i].wValue, tblMIU[i].wMask );



#if (ENABLE_TNR)||(ENABLE_RTE)||(ENABLE_R2_CODE_ON_DRAM)||(ENABLE_USB_INPUT)
#if 0 // R2 already on dram
    msInitMemory();
#endif
#endif

    //_msWriteByteMask(SC0_0E, BIT7, BIT7); // H coring disable, for dot pattern certical block issue, Jison 100610
    msDrvScSetupPathInit(); //Jison need put here because mStar_SetupFreeRunMode() will use g_SetupPathInfo variable
    APPSystem_Init();
    APPInput_Init();
    APPWindow_Init();
    msDrvSrcConfigInit();
#ifndef MST9U_FPGA // prevent HDCP key access XIU
    msAPI_combo_IPInitial();
#endif

    // For Each Scaler Init.
    for( i = SCL0_SUB ; i<SC_MAX_NUMS ; i++)
    {
        DWORD u32SCRegBase = SCRegBase(i);

        if(i & BIT0) // MAIN
        {
            msWrite2Byte(SC01_20 + u32SCRegBase, 0x0001|(AOVDV_VALUE<<12)); // Auto position enable
            msWriteByteMask(SC10_03 + u32SCRegBase, BIT6, BIT6); // Sub window mute default by Sub source
            msWriteByte(SC10_48 + u32SCRegBase, 0x03); // frame color
        }
        else
        {
            msWrite2Byte(SC03_20 + u32SCRegBase, 0x0001|(AOVDV_VALUE<<12)); // Auto position enable
            msWriteByte(SC0F_2F + u32SCRegBase, 0x03); // frame color
        }

        // init window timing gen
        {
            WORD  u16PnlHDEStart = PanelHStart/(msDrvISSCDualPort() ? 2:1);
            WORD  u16PnlHDEEnd = PanelHStart/(msDrvISSCDualPort() ? 2:1) + PANEL_WIDTH/(msDrvISSCDualPort() ? 2:1) -1;
            WORD  u16Htotal = ( PanelHTotal/(msDrvISSCDualPort() ? 2:1) - 1 );
            BYTE  u8HsyncEnd = (BYTE)(( PanelHSyncWidth/(msDrvISSCDualPort() ? 2:1) ) - 2);
            BYTE  u8DBBackup = msReadByte(SC00_02 + u32SCRegBase);

            msWriteByte(SC00_02 + u32SCRegBase , 0x00); // double buffer
            msWrite2ByteMask(u32SCRegBase+SC10_0C,0x00,0x0FFF);//[11:0]vfDE_start
            msWrite2ByteMask(u32SCRegBase+SC10_0E,( PANEL_HEIGHT - 1 ),0x0FFF);//[11:0]vfDE_end
            msWrite2ByteMask(u32SCRegBase+SC10_08,u16PnlHDEStart,0x1FFF);//[12:0]hfDE_start
            msWrite2ByteMask(u32SCRegBase+SC10_0A,u16PnlHDEEnd,0x1FFF);//[12:0]hfDE_end
            msWrite2ByteMask(u32SCRegBase+SC10_1A, ( PanelVTotal - 1 ),0x0FFF);//[11:0]Output Vtt
            msWrite2ByteMask(u32SCRegBase+SC10_04,( PanelVTotal - PanelVStart ),0x07FF);//[10:0]vsync_start
            msWrite2ByteMask(u32SCRegBase+SC10_06,( PanelVTotal - PanelVSyncBackPorch ),0x0FFF);//[10:0]vsync_start
            msWrite2ByteMask(u32SCRegBase+SC10_18,u16Htotal,0x0FFF);//[11:0]Output Htt
            msWriteByte(u32SCRegBase+SC10_02,u8HsyncEnd);//[7:0]hsync_end
            msWriteByte(SC00_02 + u32SCRegBase , u8DBBackup); // double buffer
        }
    }

#if ENABLE_R2_2DTO3D
    if(UserPref2Dto3D)
    {
        g_SetupPathInfo.b2DTo3DEnable = TRUE;
        Sc2DTo3DInit();
    }
#endif

    _msWriteByteMask(REG_06AE,0x00,0x07);
    _msWriteByteMask(REG_07AE,0x00,0x07);
}

//Average short line tuning
void mStar_STuneAverageMode(Bool bEnable)
{
    if(bEnable)
    {
        _msWriteByteMask(SC0_FA, BIT3,BIT3);
    }
    else
    {
        _msWriteByteMask(SC0_FA, 0,BIT3);
    }
}

void mStar_InterlaceModeSetting( BYTE InputPort )
{
#if EN_V4TAP_SCALING
#define DELAY_LINES  3 //  default: 1
#else
#define DELAY_LINES  2
#endif

    if( InterlaceModeFlag(InputPort) )
    {
        _msWriteByte(SC0_6F,0x00);
        //_msWriteByte(SC0_90, (ucDelayLine)<<4);
        _msWriteByteMask(SC0_0F,BIT7,BIT7); //shift line mode enable
        if(INPUT_IS_VGA(InputPort))//(MapScaler2Port == Input_Analog1)
        {
            if(_msReadByte(SC0_ED)&BIT5)
               _msWriteByteMask(SC0_E9,0,BIT0);
            else
               _msWriteByteMask(SC0_E9,BIT0,BIT0);
        }
        else            //110921 Rick modified - B011
        {
            if(_msReadByte(SC0_04)&BIT6)
            {
                _msWriteByteMask(SC0_E9,0,BIT0);//0x01); //Jison 080925 for DVI interlace //0x00 090512
                _msWriteByteMask(SC0_05,0,BIT0);
            }
            else
            {
                 if(CURRENT_INPUT_IS_DISPLAYPORT())      // For DP interlace 20121025
                     _msWriteByteMask(SC0_E9,0,BIT0);
                 else
                     _msWriteByteMask(SC0_E9,BIT0,BIT0);      //0x01); //Jison 080925 for DVI interlace //0x00 090512

                 _msWriteByteMask(SC0_05,BIT0,BIT0);
            }

        }

        // mStar_WriteByte(BK0_74, 0x10); //enable Vertical 2-tap and CB0
        // mStar_WriteByte(BK0_90, 0x10); //set delay line=1 for 2-tap scaling
         //delay line number needs to calculate from formula
         mStar_STuneAverageMode(TRUE);
    }
    else
    {
        _msWriteByte(SC0_6F,0x00);
        //_msWriteByte(SC0_90, (ucDelayLine<<4));
        _msWriteByteMask(SC0_0F,0,BIT7); //shift line mode disable
        _msWriteByteMask(SC0_E9,0,BIT0);
       // mStar_WriteByte(BK0_74, 0x80); //enable Vertical 3-tap and WinSinc
       // mStar_WriteByte(BK0_90, 0x20); //set delay line=2 for 3-tap scaling
       mStar_STuneAverageMode(FALSE);
    }

}

void mStar_SetPanelSSC( BYTE freqMod, BYTE range )
{
    DWORD xdata dwFactor;
    WORD xdata wStep;
    WORD xdata wSpan;
    DWORD xdata u32MPLL_MHZ = 429ul;

    if( (freqMod == 0) || (range == 0) )
    {
        msWrite2ByteMask(REG_14052E, 0, 0x3FF);
        msWrite2ByteMask(REG_140530, 0, 0x3FFF);
        return;
    }

    // freqMod: 0~40 means 0~40K
    if(freqMod > PANEL_SSC_MODULATION_MAX)
        freqMod = PANEL_SSC_MODULATION_MAX;

    // range: 0~30 means 0~3%
    if(range > PANEL_SSC_PERCENTAGE_MAX)
        range = PANEL_SSC_PERCENTAGE_MAX;

    dwFactor = msReadByte(REG_140520);
    dwFactor <<= 16;
    dwFactor |= msRead2Byte(REG_14051E);

    wSpan = ((u32MPLL_MHZ*1024ul/freqMod) * 131072ul) / dwFactor;
    wStep = (float)(dwFactor*range/10)/wSpan/100; // 120320 coding modified

    //drvmStar_printData("PanelSSC SPAN[%x]",wSpan);
    //drvmStar_printData("PanelSSC STEP[%x]",wStep);

    if (wStep > 0x3FF)
        wStep = 0x3FF;

    if(wSpan > 0x3FFF)
        wSpan = 0x3FFF;

    msWrite2ByteMask(REG_14052E, wStep, 0x3FF);
    msWrite2ByteMask(REG_140530, wSpan, 0x3FFF);
    msWriteBit(REG_14051B, TRUE, BIT3); // ssc enable
}

void drvDVI_RtermControl(BYTE ucPort)
{
#if(ENABLE_MHL)
    if(!MHL_CABLE_DETECT_PORT0())
    {
        if(ucPort == DVI_INPUT_PORT0)
        {
            _msWriteByteMask(REG_01C4, 0, 0x0F); // power on R-term
        }
        else
        {
            _msWriteByteMask(REG_01C4, 0x0D, 0x0F); // power down R-term
        }
    }

    if(!MHL_CABLE_DETECT_PORT1())
    {
        if(ucPort == DVI_INPUT_PORT1)
        {
            _msWriteByteMask(REG_01C4, 0, 0xF0); // power on R-term
        }
        else
        {
            _msWriteByteMask(REG_01C4, 0xD0, 0xF0); // power down R-term
        }
    }

    if(!MHL_CABLE_DETECT_PORT2())
    {
        if(ucPort == DVI_INPUT_PORT2)
        {
            _msWriteByteMask(REG_01C5, 0, 0x0F); // power on R-term
        }
        else
        {
            _msWriteByteMask(REG_01C5, 0x0D, 0x0F); // power down R-term
        }
    }

    if(ucPort == DVI_INPUT_NONE)
    {
        if(MHL_CABLE_DETECT_PORT0() ||MHL_CABLE_DETECT_PORT1() ||MHL_CABLE_DETECT_PORT2())
        {
            _msWriteByteMask(REG_01C5, 0xD0, 0xF0); // power down R-term
        }
        else
        {
            _msWriteByteMask(REG_01C5, 0xF0, 0xF0); // power down R-term
        }
    }
    else
    {
        _msWriteByteMask(REG_01C5, 0, 0xF0); // power on R-term
    }

#else
    if(ucPort == DVI_INPUT_PORT0)
    {
        _msWriteByteMask(REG_01C4, 0, 0x0F); // power on R-term
    }
    else
    {
        _msWriteByteMask(REG_01C4, 0x0F, 0x0F); // power down R-term
    }

    if(ucPort == DVI_INPUT_PORT1)
    {
        _msWriteByteMask(REG_01C4, 0, 0xF0); // power on R-term
    }
    else
    {
        _msWriteByteMask(REG_01C4, 0xF0, 0xF0); // power down R-term
    }

    if(ucPort == DVI_INPUT_PORT2)
    {
        _msWriteByteMask(REG_01C5, 0, 0x0F); // power on R-term
    }
    else
    {
        _msWriteByteMask(REG_01C5, 0x0F, 0x0F); // power down R-term
    }

    if(ucPort == DVI_INPUT_NONE)
    {
        _msWriteByteMask(REG_01C5, 0xF0, 0xF0); // power down R-term
    }
    else
    {
        _msWriteByteMask(REG_01C5, 0, 0xF0); // power on R-term
    }

#endif
}

// 111220 coding test
void drvDVI_PowerCtrl(DVI_PowerCtrlType ctrl)
{
    if(ctrl == DVI_POWER_STANDBY)
    {
        // DVI power on
        // _msWriteByteMask(REG_01C3, 0, BIT4); // power on DVI CKIN.
        _msWriteByteMask(REG_01CE, 0, BIT4); // power on PLL band-gap.
        // Power up Xtal clocks and DVI detection clock for DVI clock detection
        _msWrite2ByteMask(REG_174C, 0x0001, 0xFFFF); // enable main link clock
        _msWrite2ByteMask(REG_174E, 0x1000, 0x3FFF); // enable Xtal
        // DVI PLL power control
        // _msWrite2ByteMask(REG_01C2, 0xC000, 0xC000); // power down DVI0 PLL power
        // _msWrite2ByteMask(REG_01C4, 0x0FFF, 0x0FFF); // power down R-term

        _msWrite2ByteMask(REG_01C2, 0xCEE8, 0xFFE8);
        //_msWrite2ByteMask(REG_01C4, 0xFEEE, 0xFEEE);
        drvDVI_RtermControl(DVI_INPUT_NONE);
        _msWriteByteMask(REG_01CE, 0x00, 0x07);
        _msWrite2ByteMask(REG_01C6, 0xFFFF, 0xFFFF);
    }
    else if(ctrl == DVI_POWER_DOWN)
    {
        // DVI power down
        // _msWriteByteMask(REG_01C3, BIT4, BIT4); // power down DVI CKIN.
        _msWriteByteMask(REG_01CE, BIT4, BIT4); // power down PLL band-gap.
        // Power up Xtal clocks and DVI detection clock for DVI clock detection
        _msWrite2ByteMask(REG_174C, 0x0000, 0xFFFF);
        _msWrite2ByteMask(REG_174E, 0x0000, 0x3FFF);
        // DVI PLL power control
        // _msWrite2ByteMask(REG_01C2, 0xC000, 0xC000); // power down DVI0 PLL power
        // _msWrite2ByteMask(REG_01C4, 0x0FFF, 0x0FFF); // power down R-term

        _msWrite2ByteMask(REG_01C2, 0xFF08, 0xFFE8);
        //_msWrite2ByteMask(REG_01C4, 0xFFFF, 0xFEEE);
        drvDVI_RtermControl(DVI_INPUT_NONE);
        _msWriteByteMask(REG_01CE, 0x07, 0x07);
        _msWrite2ByteMask(REG_01C6, 0xFFFF, 0xFFFF);

    }
    else
    {
        // DVI power on
        //_msWriteByteMask(REG_01C3, 0, BIT4); // power on DVI CKIN.
        _msWriteByteMask(REG_01CE, 0, BIT4); // power on PLL band-gap
        _msWrite2ByteMask(REG_174C, 0xFFFF, 0xFFFF);
        _msWrite2ByteMask(REG_174E, 0x3FFF, 0x3FFF);

        if(INPUT_IS_MUX_C0(SrcInputPortM/*//tmp*/))
        {
            _msWriteByteMask( REG_01C2, BIT5, BIT7|BIT6|BIT5 );
            //_msWrite2ByteMask(REG_01C4, 0x0EE0, 0xFEEE); // power on R-term
            drvDVI_RtermControl(DVI_INPUT_PORT0);
            _msWrite2ByteMask(REG_01CE, 0x06, 0x07); // power on CLK R-term
        }
        else if(INPUT_IS_MUX_C1(SrcInputPortM/*//tmp*/))
        {
            _msWriteByteMask( REG_01C2, BIT6, BIT7|BIT6|BIT5 );
            //_msWrite2ByteMask(REG_01C4, 0x0E0E, 0xFEEE); // power on R-term
            drvDVI_RtermControl(DVI_INPUT_PORT1);
            _msWrite2ByteMask(REG_01CE, 0x05, 0x07); // power on CLK R-term
        }
        else if(INPUT_IS_MUX_C2(SrcInputPortM/*//tmp*/))
        {
            _msWriteByteMask( REG_01C2, BIT7, BIT7|BIT6|BIT5 );
            //_msWrite2ByteMask(REG_01C4, 0x00EE, 0xFEEE); // power on R-term
            drvDVI_RtermControl(DVI_INPUT_PORT2);
            _msWrite2ByteMask(REG_01CE, 0x03, 0x07); // power on CLK R-term
        }
        else
        {
            _msWriteByteMask( REG_01C2, 0x00, BIT7|BIT6|BIT5 );
            //_msWrite2ByteMask(REG_01C4, 0xFEEE, 0xFFFF); // power down R-term
            drvDVI_RtermControl(DVI_INPUT_NONE);
            _msWrite2ByteMask(REG_01CE, 0x07, 0x07); // power down CLK R-term
        }

        _msWrite2ByteMask(REG_01C2, 0x0000, 0xFF08);
        _msWrite2ByteMask(REG_01C6, 0x0000, 0xFFFF);
    }
}
void mStar_IPPowerControl(void)
{
    if (CURRENT_INPUT_IS_VGA())//( (MapScaler2Port == Input_Analog1) ) // ADC
    {
        // ADC power on
        drvADC_PowerCtrl(ADC_POWER_ON);

  #ifdef _IP_POWER_CTRL_
        drvDVI_PowerCtrl(DVI_POWER_DOWN);
  #endif
    }
    else if( (CURRENT_INPUT_IS_DVI())//(MapScaler2Port == Input_Digital)
        #if ENABLE_HDMI_INPUT
        || (CURRENT_INPUT_IS_HDMI())//(MapScaler2Port == Input_HDMI)
        #endif // #if Enable_DVI2
    ) // DVI / HDMI
    {
  #ifdef _IP_POWER_CTRL_
        drvDVI_PowerCtrl(DVI_POWER_ON);
  #endif
        // ADC power down
        drvADC_PowerCtrl(ADC_POWER_DOWN); //CHIP_FAMILY_TSUMV need power down ADC for DVI good phase
    }
     else if(CURRENT_INPUT_IS_DISPLAYPORT())
    {
  #ifdef _IP_POWER_CTRL_
        drvDVI_PowerCtrl(DVI_POWER_ON);
  #endif
        // ADC power down
        drvADC_PowerCtrl(ADC_POWER_DOWN); //CHIP_FAMILY_TSUMV need power down ADC for DVI good phase
    }
}

void msTMDSSetMux( BYTE inport )
{
    if(INPUT_IS_DVI(inport))//( inport == Input_Digital )
    {
        BYTE i;
        //_msWriteByteMask(REG_01C3, 0, BIT4); // power on DVI CKIN.
        _msWriteByteMask(REG_01CE, 0, BIT4); // power on PLL band-gap.
        // Power up Xtal clocks and DVI detection clock for DVI clock detection
        _msWrite2ByteMask(REG_174C, 0xFFFF, 0xFFFF);
        _msWrite2ByteMask(REG_174E, 0x3FFF, 0x3FFF);
        // DVI PLL power control
        _msWrite2ByteMask(REG_01C2, 0x4000, 0x4000); // power off DVI PLL power
        //_msWrite2ByteMask(REG_01C4, 0x0000, 0x0FFF); // power on R-term
        //_msWrite2ByteMask(REG_01C4, 0x0000, 0xFFFF);
        _msWriteByteMask( REG_1740, BIT0, BIT0 ); //HDMI select.
        _msWriteByteMask( REG_17A5, BIT0, BIT1|BIT0 ); //DCDR demux ratio select.
        //_msWriteByteMask( REG_17A5, BIT4, BIT5|BIT4 ); //EQ mode setting.
        _msWriteByteMask( REG_17A5, BIT5|BIT4, BIT5|BIT4 ); //EQ mode setting.

        for(i=0;i<sizeof(tComboTMDSMuxTbl)/sizeof(RegTbl2Type);i++)
            _msWrite2ByteMask(tComboTMDSMuxTbl[i].dwReg,tComboTMDSMuxTbl[i].wValue,tComboTMDSMuxTbl[i].wMask );
        //Port Select
        if(INPUT_IS_MUX_C0(inport))
        {
            _msWriteByteMask( REG_01C2, BIT5, BIT7|BIT6|BIT5 );
            _msWriteByteMask( REG_1501, BIT4, BIT5|BIT4 );
        }
        else if(INPUT_IS_MUX_C1(inport))
        {
            _msWriteByteMask( REG_01C2, BIT6, BIT7|BIT6|BIT5 );
            _msWriteByteMask( REG_1501, 0, BIT5|BIT4 );
        }
        else if(INPUT_IS_MUX_C2(inport))
        {
            _msWriteByteMask( REG_01C2, BIT7, BIT7|BIT6|BIT5 );
            _msWriteByteMask( REG_1501, BIT5, BIT5|BIT4 );
        }
        msEread_SetHDMIInitialValue();
    }
  #if ENABLE_HDMI_INPUT
    else if( INPUT_IS_HDMI(inport) ) // HDMI - Port B
    {
        BYTE i;
        //_msWriteByteMask(REG_01C3, 0, BIT4); // power on DVI CKIN.
        _msWriteByteMask(REG_01CE, 0, BIT4); // power on PLL band-gap.
        // Power up Xtal clocks and DVI detection clock for DVI clock detection
        _msWrite2ByteMask(REG_174C, 0xFFFF, 0xFFFF);
        _msWrite2ByteMask(REG_174E, 0x3FFF, 0x3FFF);
        // DVI PLL power control
        _msWrite2ByteMask(REG_01C2, 0x4000, 0x4000); // power off DVI PLL power
        //_msWrite2ByteMask(REG_01C4, 0x0000, 0x0FFF); // power on R-term
        //_msWrite2ByteMask(REG_01C4, 0x0000, 0xFFFF);
        _msWriteByteMask( REG_1740, BIT0, BIT0 ); //HDMI select.
        _msWriteByteMask( REG_17A5, BIT0, BIT1|BIT0 ); //DCDR demux ratio select.
        //_msWriteByteMask( REG_17A5, BIT4, BIT5|BIT4 ); //EQ mode setting.
        _msWriteByteMask( REG_17A5, BIT5|BIT4, BIT5|BIT4 ); //EQ mode setting.
        _msWriteByteMask( REG_1427, 0, BIT0 ); //audio source selection

        for(i=0;i<sizeof(tComboTMDSMuxTbl)/sizeof(RegTbl2Type);i++)
            _msWrite2ByteMask(tComboTMDSMuxTbl[i].dwReg,tComboTMDSMuxTbl[i].wValue,tComboTMDSMuxTbl[i].wMask );
        //Port Select
        if(INPUT_IS_MUX_C0(inport))
        {
            _msWriteByteMask( REG_01C2, BIT5, BIT7|BIT6|BIT5 );
            _msWriteByteMask( REG_1501, BIT4, BIT5|BIT4 );
        }
        else if(INPUT_IS_MUX_C1(inport))
        {
            _msWriteByteMask( REG_01C2, BIT6, BIT7|BIT6|BIT5 );
            _msWriteByteMask( REG_1501, 0, BIT5|BIT4 );
        }
        else if(INPUT_IS_MUX_C2(inport))
        {
            _msWriteByteMask( REG_01C2, BIT7, BIT7|BIT6|BIT5 );
            _msWriteByteMask( REG_1501, BIT5, BIT5|BIT4 );
        }
        msEread_SetHDMIInitialValue();
    }
  #endif // #if ENABLE_HDMI_INPUT
    else // non-DVI/HDMI ports
    {
        //_msWriteByteMask(REG_01C3, BIT4, BIT4); // power down DVI CKIN.
        _msWriteByteMask(REG_01CE, BIT4, BIT4); // power down PLL band-gap.
        // Power up Xtal clocks and DVI detection clock for DVI clock detection
        _msWrite2ByteMask(REG_174C, 0x0001, 0xFFFF); // enable main link clock
        _msWrite2ByteMask(REG_174E, 0x2000, 0x3FFF); // enable Xtal
        // DVI PLL power control
        //_msWrite2ByteMask(REG_01C2, 0xC000, 0xC000); // power on DVI0 PLL power
        //_msWrite2ByteMask(REG_01C4, 0x0FFF, 0x0FFF); // power on R-term
        _msWrite2ByteMask(REG_01C2, 0xCE08, 0xFF08);
        //_msWrite2ByteMask(REG_01C4, 0xFEEE, 0xFFFF);
        drvDVI_RtermControl(DVI_INPUT_NONE);
        _msWrite2ByteMask(REG_01C6, 0xFFFF, 0xFFFF);

        _msWriteByteMask( REG_01C2, 0, BIT7|BIT6|BIT5 );
    }
}


#if defined(_ENABLE_LPLL_FINETUNE_)
void mStar_LPLLFineTune(WORD u16DST_HTotal)
{
   DWORD   u32Factor;
   WORD u16HttFraction;
   WORD u16LockHtt, u16OHtt;

   _msWriteByteMask(REG_1ED3, 0,BIT6);// disable SSC
   Delay1ms(40);// at least 2 frame delay after LPLL update for fraction value stable
   //PRINT_DATA("\r\n u16DST_HTotal=%x",u16DST_HTotal);

   _msWriteByteMask(SC0_96, BIT7,BIT7);
   u16HttFraction = _msRead2Byte(SC0_91)&0x0FFF;// Read Fraction
   //PRINT_DATA(" u16HttFraction :%x",u16HttFraction);

   u32Factor = _msReadByte(REG_1ED7); // Read output dot clock
   u32Factor=(u32Factor<<16);
   u32Factor+= _msRead2Byte(REG_1ED5);
   //PRINT_DATA(" REG_1ED7=%x",_msReadByte(REG_1ED7));
   //PRINT_DATA(" REG_1ED4_5=%x",_msRead2Byte(REG_1ED5));
   //((float)u16OHtt*(PANEL_HEIGHT-1)+3), +3 to avoid too close to cause fraction value floating
   #if 1 //Jison 110727 use theoretical value u16DST_HTotal to make sure get the normal dclk divider
   u32Factor = (((float)u16DST_HTotal*(PANEL_HEIGHT-1)+u16HttFraction)/((float)u16DST_HTotal*(PANEL_HEIGHT-1)+3))*u32Factor;
   #else
   u16OHtt = _msRead2Byte(SC0_24)&0x0FFF;
   u16LockHtt = _msRead2Byte(SC0_97)&0x0FFF;
   PRINT_DATA("\r\n u16LockHtt :%x",u16LockHtt);
   PRINT_DATA(" u16OHtt :%x",u16OHtt);
   u32Factor = (((float)u16LockHtt*(PANEL_HEIGHT-1)+u16HttFraction)/((float)u16OHtt*(PANEL_HEIGHT-1)+3))*u32Factor;
   #endif
   // program LPLL parameters to generate expected output dclk
   _msWrite2Byte(REG_1ED5, u32Factor&0xFFFF);
   _msWriteByte(REG_1ED7, u32Factor>>16); // setup output dot clock
   #if 0//DEBUG_EN
   Delay1ms(40); //at least 2 frame
   PRINT_DATA(" NEW_Fration :%x",_msRead2Byte(SC0_91)&0x0FFF);
   //PRINT_DATA(" new REG_1ED7=%x",_msReadByte(REG_1ED7));
   //PRINT_DATA(" new REG_1ED4_5=%x\r\n",_msRead2Byte(REG_1ED5));
   #endif
   _msWriteByteMask(SC0_96, 0,BIT7);
   _msWriteByteMask(REG_1ED3, BIT6,BIT6);// Enable SSC
}
#endif


//Jison 091023 Do I_Gen_Tuning at turn on panel 1st time. 100305 alway enable


#if( USE_MOD_HW_CAL )
// MOD hardware current calibration
// @pre MOD is not in power-down (BK1_26[7]==0)
// @pre LPLL is not in power-down
void msDrvIGenTuning( void )
{
#ifndef MST9U_FPGA
    // 0.backup reg
    BYTE XDATA  i;
    BYTE XDATA  reg_140642   = msReadByte(REG_140642);
    WORD XDATA  reg_140694   = msRead2Byte(REG_140694);
    WORD XDATA  reg_140696   = msRead2Byte(REG_140696);
    BYTE XDATA  reg_14060B   = msReadByte(REG_14060B);
    //BYTE XDATA  reg_308B   = msReadByte(REG_308B);
    WORD XDATA  reg_140666   = msRead2Byte(REG_140666); // ch double swing
#if !PANEL_VBY1()
    //drvmStar_printMsg("HW I_Gen_Tuning_Start");

    //_msWriteByteMask(REG_3079, BIT7, BIT7); // Auto update Cal result to control swing reg in HW mode, SW mode needs set to 0 (for FT use)

    // 1.Disable double differential output swing
    msWrite2Byte(REG_140666,   0x00);
    // 2.Select calibration source.
    #if(I_GEN_CH==CH2)
    msWriteByteMask(REG_14067C,   0x00,  BIT4|BIT3|BIT2); // Ch2
    msWrite2ByteMask(REG_140694,  0x00,  BIT8| BIT7|BIT6);
    msWriteByteMask(REG_140642,   0x00,  BIT2);
    #else
    msWriteByteMask(REG_14067C,   BIT2,  BIT4|BIT3|BIT2); // Ch6
    msWrite2ByteMask(REG_140696,  0x00,  BIT4| BIT3|BIT2);
    msWriteByteMask(REG_140642,   0x00,  BIT6);
    #endif
    // 3.Enable LVDS Test Mode, and make output data=High for LVDS output.
    //Note:
    //Please disable MOD Differential output polarity swap. (BK36_E6[7:0]=8'h00, BK36_E7[1:0]=2'h0)
    msWriteByteMask(REG_14060B,   BIT7,  BIT7);
    //_msWriteByteMask(REG_308B,   0x00,  BIT1|BIT0);
    // 4.Set LVDS voltage target.
    msWriteByteMask(REG_14067C,   LVDS_VOLTAGE_TARGET,  BIT1|BIT0);
    // 5.Enable Hardware Calibration
    msWriteByteMask(REG_14067C,   BIT7,  BIT7);
    msWriteByteMask(REG_1406E1,   BIT7,  BIT7);
    // 6.Wait until BK36_FCh[7] = 1. Hardware Calibration ready.
    TimeOutCounter = 50;
    while (! ( msReadByte( REG_1406E1 ) & BIT6 ) && TimeOutCounter );

    // 7.If REG_1406E1[5] = 1, calibration failed.
    #if ENABLE_MSTV_UART_DEBUG
    if(msReadByte( REG_1406E1 ) & BIT5 )
        drvmStar_printMsg("HW MOD Cali Fail !!");
    else
    {
        drvmStar_printMsg("HW MOD Cali Pass !!");
        drvmStar_printData("Cali result=0x%x", msReadByte(REG_1406E0));
    }
    #endif

    // write calibration value for each ch
    i = msReadByte(REG_1406E0);
#else
    i = 0x3F;
#endif
    drvmStar_printData("I_gen_tuning = %x",i);
    msWrite2Byte(REG_140680, ((WORD)i<<12)|(i<<6)| i); // ch0~ch2
    msWrite2Byte(REG_140682, ((WORD)i<<14)|(i<<8)|(i<<2)| (i>>4)); // ch2~ch5
    msWrite2Byte(REG_140684, ((WORD)i<<10)|(i<<4)| (i>>2)); // ch5~ch7

    msWrite2Byte(REG_140686, ((WORD)i<<12)|(i<<6)| i); // ch8~ch10
    msWrite2Byte(REG_140688, ((WORD)i<<14)|(i<<8)|(i<<2)| (i>>4)); // ch10~ch13
    msWrite2Byte(REG_14068A, ((WORD)i<<10)|(i<<4)| (i>>2)); // ch13~ch15

    msWrite2Byte(REG_14068C, ((WORD)i<<12)|(i<<6)| i); // ch16~ch18
    msWrite2Byte(REG_14068E, ((WORD)i<<14)|(i<<8)|(i<<2)| (i>>4)); // ch18~ch21
    msWrite2Byte(REG_140690, ((WORD)i<<10)|(i<<4)| (i>>2)); // ch21~ch23


    msWriteByte(REG_14078A, i); // ch24
    msWrite2Byte(REG_14078C, ((WORD)i<<12)| (i<<6)); // ch25~ch26
    msWrite2Byte(REG_14078E, ((WORD)i<<14)|(i<<8)|(i<<2)| (i>>4)); // ch26~ch29
    msWrite2Byte(REG_140790, ((WORD)i<<10)|(i<<4)| (i>>2)); // ch29~ch31

    msWrite2Byte(REG_140792, ((WORD)i<<12)|(i<<6)| i); // ch32~ch34
    msWrite2Byte(REG_140794, ((WORD)i<<14)|(i<<8)|(i<<2)| (i>>4)); // ch34~ch37
    msWrite2Byte(REG_140796, ((WORD)i<<10)|(i<<4)| (i>>2)); // ch37~ch39

    msWrite2Byte(REG_140798, ((WORD)i<<12)|(i<<6)| i); // ch40~ch42
    msWrite2Byte(REG_14079A, ((WORD)i<<14)|(i<<8)|(i<<2)| (i>>4)); // ch42~ch45
    msWrite2Byte(REG_14079C, ((WORD)i<<10)|(i<<4)| (i>>2)); // ch45~ch47
    // 8.Disable Hardware Calibration.
    msWriteByteMask(REG_14067C,   0x00,  BIT7);
    msWriteByteMask(REG_1406E1,   0x00,  BIT7);
    //9.Set register "BK36_E2[2], BK36_E2[7], BK36_46[1:0], BK36_80[1], BK36_8B[1:0]" back to the original normal function.
    msWriteByte(REG_140642, reg_140642);
    msWrite2Byte(REG_140694, reg_140694);
    msWrite2Byte(REG_140696, reg_140696);
    msWriteByte(REG_14060B, reg_14060B);
    //_msWriteByte(REG_308B, reg_308B);
    msWrite2Byte(REG_140666, reg_140666);

    //drvmStar_printMsg("HW I_Gen_Tuning_End");
#endif
    iGenTuningFinished=1;
}

#else //SW
// @brief MOD Current Calibration
// @pre   Current bank is bank-0.
// @pre   LPLL is on, i.e., ASSERT( BK0_D1[4]==0 )
void msDrvIGenTuning( void )
{
    BYTE i=0;
    // 0.backup reg
    volatile BYTE reg_140642   = msReadByte(REG_140642);
    volatile WORD reg_140694   = msRead2Byte(REG_140694);
    volatile WORD reg_140696   = msRead2Byte(REG_140696);
    volatile BYTE reg_14060B   = msReadByte(REG_14060B);
    //volatile BYTE reg_308A   = msReadByte(REG_308A);
    //volatile BYTE reg_308B   = _msReadByte(REG_308B);
    volatile WORD reg_140666   = msRead2Byte(REG_140666); // ch double swing
    volatile BYTE reg_140660   = msReadByte(REG_140660);

    #define SWING_MASK  0x3F
    drvmStar_printMsg("FW I_Gen_Tuning_Start");

    msWriteByteMask(REG_1406E3, 0, BIT7); // Auto update Cal result to control swing reg in HW mode, SW mode needs set to 0 (for FT use)

    // 1.Disable power down mod
    msWriteByteMask(REG_140650,   0x00, BIT0);
    // ((// 2.Enable MOD self current bias.))
    //_msWriteByteMask(REG_14067C,   BIT6, BIT6);
    // 3.Set output bias current.
    //_msWriteByteMask(REG_14067C,   0x00, 0x3F);
    // 4.Disable double differential output swing
    msWrite2Byte(REG_140666,   0x00);
    // 5.select calibration source.
    //Note:
    //if BK36_FBh[1]=0, set BK36_DAh[5:4]=2'b01, BK36_46[1:0] = 2'b00, BK_36_E2[2] = 1'b0, and BK36_EEh[3:0]=4'hF to make PADA_OUT[PN]_CH[2] as LVDS output.
    //if BK36_FBh[1]=1, set BK36_DBh[7:6]=2'b01, BK36_46[1:0] = 2'b00, BK_36_E2[7] = 1'b0, and BK36_EEh[3:0]=4'hF to make PADA_OUT[PN]_CH[7] as LVDS output.
    #if(I_GEN_CH==CH2)
    msWrite2ByteMask(REG_140681,   0x00, 0x03F0); // ch2 control swing
    msWriteByteMask(REG_14067C,   0x00, BIT4|BIT3|BIT2);
    msWrite2ByteMask(REG_140694,  0x00, BIT8| BIT7|BIT6);
    msWriteByteMask(REG_140642,   0x00, BIT2);
    msWriteByteMask(REG_14064E,   0x3F, 0x3F);
    #else
    msWrite2ByteMask(REG_140684,   0x00, 0x03F0); // ch6 control swing
    msWriteByteMask(REG_14067C,   BIT2, BIT4|BIT3|BIT2);
    msWrite2ByteMask(REG_140696,  0, BIT4| BIT3|BIT2);
    msWriteByteMask(REG_140642,   0, BIT6);
    msWriteByteMask(REG_14064E,   0x3F, 0x3F);
    #endif
    // 6.Enable LVDS Test Mode, and make output data=High for LVDS output.
    msWriteByteMask(REG_140660,   BIT0, BIT0);
    msWriteByteMask(REG_14060B,   BIT7, BIT7);
    msWriteByteMask(REG_140641,   BIT6, BIT6);
    //_msWriteByteMask(REG_308B,   0x00, BIT1|BIT0);
    msWriteByteMask(REG_140604,   0xFF, 0xFF);
    // 7.Enable MOD Calibration.
    msWriteByteMask(REG_14067C,   BIT7, BIT7);
    // 8.Used to set LVDS voltage target.
    msWriteByteMask(REG_14067C,   LVDS_VOLTAGE_TARGET,  BIT1|BIT0);
    //9.Adjust the value of BK36_FAh[5:0] step by step to control output swing and check whether BK36_FBh[0] is equal to "1" or "0".
    for (i=0; i<=SWING_MASK; i++)
    {
        #if(I_GEN_CH==CH2)
        msWrite2ByteMask(REG_140681, i<<4, 0x03F0); // ch2 control swing
        #else
        msWrite2ByteMask(REG_140684, i<<4, 0x03F0); // ch6 control swing
        #endif

        ForceDelay1ms(10);
#if !PANEL_VBY1()
        //10.If BK36_FBh[0] is toggling between "1" and "0", terminal the fine-tune procedure.
        if (msReadByte(REG_14067D)&BIT0)
#else
        i = 0x3F;
#endif
        {
            msWrite2Byte(REG_140680, ((WORD)i<<12)|(i<<6)| i); // ch0~ch2
            msWrite2Byte(REG_140682, ((WORD)i<<14)|(i<<8)|(i<<2)| (i>>4)); // ch2~ch5
            msWrite2Byte(REG_140684, ((WORD)i<<10)|(i<<4)| (i>>2)); // ch5~ch7

            msWrite2Byte(REG_140686, ((WORD)i<<12)|(i<<6)| i); // ch8~ch10
            msWrite2Byte(REG_140688, ((WORD)i<<14)|(i<<8)|(i<<2)| (i>>4)); // ch10~ch13
            msWrite2Byte(REG_14068A, ((WORD)i<<10)|(i<<4)| (i>>2)); // ch13~ch15

            msWrite2Byte(REG_14068C, ((WORD)i<<12)|(i<<6)| i); // ch16~ch18
            msWrite2Byte(REG_14068E, ((WORD)i<<14)|(i<<8)|(i<<2)| (i>>4)); // ch18~ch21
            msWrite2Byte(REG_140690, ((WORD)i<<10)|(i<<4)| (i>>2)); // ch21~ch23

            msWriteByte(REG_14078A, i); // ch24
            msWrite2Byte(REG_14078C, ((WORD)i<<12)| (i<<6)); // ch25~ch26
            msWrite2Byte(REG_14078E, ((WORD)i<<14)|(i<<8)|(i<<2)| (i>>4)); // ch26~ch29
            msWrite2Byte(REG_140790, ((WORD)i<<10)|(i<<4)| (i>>2)); // ch29~ch31

            msWrite2Byte(REG_140792, ((WORD)i<<12)|(i<<6)| i); // ch32~ch34
            msWrite2Byte(REG_140794, ((WORD)i<<14)|(i<<8)|(i<<2)| (i>>4)); // ch34~ch37
            msWrite2Byte(REG_140796, ((WORD)i<<10)|(i<<4)| (i>>2)); // ch37~ch39

            msWrite2Byte(REG_140798, ((WORD)i<<12)|(i<<6)| i); // ch40~ch42
            msWrite2Byte(REG_14079A, ((WORD)i<<14)|(i<<8)|(i<<2)| (i>>4)); // ch42~ch45
            msWrite2Byte(REG_14079C, ((WORD)i<<10)|(i<<4)| (i>>2)); // ch45~ch47
            drvmStar_printMsg("FW MOD Cali Pass !!");
            drvmStar_printData("Cali result=0x%x", i);
            break;
        }
    }

    if(!(_msReadByte(REG_14067D)&BIT0))
        drvmStar_printMsg("FW MOD Cali Fail !!");

    // 11.Disable MOD Calibration.
    msWriteByteMask(REG_14067C,   0x00, BIT7);
    // 12.Set register "BK36_DAh[5:4], BK36_DBh[15:14], BK36_E2[2], BK36_E2[7], BK36_46[5:4], BK36_47[7:6], BK36_80[1], BK36_8A[6], BK36_8B[1:0]" back to the original normal function.
    msWriteByte(REG_140642, reg_140642);   /* restore original value of BK0_AA */
    msWrite2Byte(REG_140694, reg_140694);   /* restore original value of BK0_AB */
    msWrite2Byte(REG_140696, reg_140696);   /* restore original value of BK0_08 */
    msWriteByte(REG_14060B, reg_14060B);   /* restore original value of BK0_09 */
    //_msWriteByte(REG_308A, reg_308A);   /* restore original value of BK0_AA */
    //_msWriteByte(REG_308B, reg_308B);   /* restore original value of BK0_AB */
    msWrite2Byte(REG_140666, reg_140666);
    msWriteByte(REG_140660, reg_140660);
    drvmStar_printMsg("FW I_Gen_Tuning_End");

    iGenTuningFinished=1;
}

#endif  // end of #if( USE_MOD_HW_CAL )

void msDrvPowerModCtrl(BYTE ucSwitch)
{
    if (ucSwitch)
    {
#if PANEL_LVDS_1CH
        msWrite2Byte(REG_140620, 0x0555);
#elif PANEL_LVDS_2CH
        msWrite2Byte(REG_140600, 0x0201);
        msWrite2Byte(REG_140620, 0x5555);
        msWrite2Byte(REG_140622, 0x0055);
        msWrite2Byte(REG_140402, 0x0010);// 1ch lvds mux
        if(msDrvISSCDualPort())
        {
            msWriteByteMask(REG_140679, 0x00, BIT5);
            msWrite2Byte(REG_140402, 0x8001);
            msWriteByteMask(REG_140601, 0x00, BIT1);
        }
        else
        {
            msWriteByteMask(REG_140679, BIT5, BIT5);
            msWrite2Byte(REG_140402, 0x0010);
            msWriteByteMask(REG_140601, BIT1, BIT1);
        }
#elif PANEL_LVDS_4CH
        if(msDrvISSCDualPort())
        {
            msWriteByteMask(REG_140679, 0x00, BIT5);

        }
        else
        {
            msWriteByteMask(REG_140679, BIT5, BIT5);
        }
            msWriteByteMask(REG_140402, BIT0, BIT0);

        msWrite2Byte(REG_140600, 0x0401);
        msWrite2Byte(REG_140678, 0x0600);
        msWrite2Byte(REG_140620, 0x5555);
        msWrite2Byte(REG_140622, 0x5555);
        msWrite2Byte(REG_140624, 0x5555);
#elif PANEL_LVDS_8CH
        msWrite2Byte(REG_140620, 0x5555);
        msWrite2Byte(REG_140622, 0x5555);
        msWrite2Byte(REG_140624, 0x5555);
        msWrite2Byte(REG_140782, 0x5555);
        msWrite2Byte(REG_140784, 0x5555);
        msWrite2Byte(REG_140786, 0x5555);
#elif PANEL_VBY1_1CH_8Bit||PANEL_VBY1_1CH_10Bit
        msWrite2Byte(REG_140624, 0x0200);
#elif PANEL_VBY1_2CH_8Bit||PANEL_VBY1_2CH_10Bit
        msWrite2Byte(REG_140624, 0x0500);
#elif PANEL_VBY1_4CH_8Bit||PANEL_VBY1_4CH_10Bit
        msWrite2Byte(REG_140402, 0x0001);
        msWrite2Byte(REG_140600, 0x0401);
        msWrite2Byte(REG_140624, 0x5500);
#elif PANEL_VBY1_8CH_8Bit||PANEL_VBY1_8CH_10Bit
        msWrite2Byte(REG_140624, 0x5500);
#elif PANEL_EDP
        msWrite2Byte(REG_140620, 0x0000);
        msWrite2Byte(REG_140622, 0x0000);
        msWrite2Byte(REG_140624, 0x0000);
          #endif

        msWriteByteMask(REG_14064E,0x3F,0x3F);
        msWriteByteMask(REG_140650,0,BIT0);

        msWrite2Byte(REG_140666, LVDS_XSWING_CH); // ch double swing
#if PANEL_VBY1()
        msDrvCheckVBY1LockN();
#endif
    }
    else
    {
        msWriteByteMask(REG_140650,BIT0,BIT0);
        msWriteByteMask(REG_14064E,0x00,0x07);
        msWrite2Byte(REG_140620, 0x0000);
        msWrite2Byte(REG_140622, 0x0000);
        msWrite2Byte(REG_140624, 0x0000);
#if PANEL_LVDS_8CH
        msWrite2Byte(REG_140782, 0x0000);
        msWrite2Byte(REG_140784, 0x0000);
        msWrite2Byte(REG_140786, 0x0000);
#endif
    }
}

BYTE mStar_ScalerDoubleBuffer(Bool u8Enable)
{
    u8Enable=FALSE;
    return u8Enable;
}

void msDrvSetDClkPLL(DWORD u32ODCLK_KHZ)
{
    BYTE uctemp = 0, ucLPLL_Type=0;
    XDATA BYTE  u8LPLL_LOOP_GAIN=0, u8LPLL_LOOP_DIV=0;
    XDATA DWORD u32Factor, u32MPLL_KHZ;
    XDATA float     u32i_gain ,u32p_gain;

    if( msDrvISSCDualPort() )
        u32ODCLK_KHZ/=2;
    u32MPLL_KHZ = 432000UL;  // 429.54MHz
#if PANEL_TTL
    if( u32ODCLK_KHZ <= 40*1000UL )    // 25MHz~40MHz
    {
        ucLPLL_Type = E_PNL_SUPPORTED_LPLL_TTL_25to40MHz;
        u8LPLL_LOOP_GAIN = 16;
        u8LPLL_LOOP_DIV = 56;
    }
    else if( u32ODCLK_KHZ <= 75*1000UL )    // 40MHz~75MHz
    {
        ucLPLL_Type = E_PNL_SUPPORTED_LPLL_TTL_40to75MHz;
        u8LPLL_LOOP_GAIN = 16;
        u8LPLL_LOOP_DIV = 28;
    }
    else    // 75MHz~150MHz
    {
        ucLPLL_Type = E_PNL_SUPPORTED_LPLL_TTL_75to150MHz;
        u8LPLL_LOOP_GAIN = 16;
        u8LPLL_LOOP_DIV = 14;
    }
#elif PANEL_LVDS_1CH
    ucLPLL_Type = E_PNL_SUPPORTED_LPLL_LVDS_1ch_1P_75to150MHz;
    u8LPLL_LOOP_GAIN = 16;
    u8LPLL_LOOP_DIV = 14;
#elif PANEL_LVDS_2CH
    if( !msDrvISSCDualPort() )
    {
        ucLPLL_Type = E_PNL_SUPPORTED_LPLL_LVDS_2ch_1P_150to300MHz;
        u8LPLL_LOOP_GAIN = 16;
        u8LPLL_LOOP_DIV = 7;
    }
    else
    {
        ucLPLL_Type = E_PNL_SUPPORTED_LPLL_LVDS_2ch_2P_75to150MHz;
        u8LPLL_LOOP_GAIN = 16;
        u8LPLL_LOOP_DIV = 14;
    }
    //ucLPLL_Type = E_PNL_SUPPORTED_LPLL_LVDS_2ch_2P_75to150MHz;

#elif PANEL_LVDS_4CH
    ucLPLL_Type = E_PNL_SUPPORTED_LPLL_LVDS_4ch_2P_150to300MHz;
    u8LPLL_LOOP_GAIN = 16;
    u8LPLL_LOOP_DIV = 7;
#elif PANEL_LVDS_8CH //8CH
    ucLPLL_Type = E_PNL_SUPPORTED_LPLL_LVDS_8ch_2P_150to300MHz;
    u8LPLL_LOOP_GAIN = 16;
    u8LPLL_LOOP_DIV         = 7;
#elif PANEL_VBY1_1CH_10Bit
    ucLPLL_Type = E_PNL_SUPPORTED_LPLL_VBY1_1ch_10bit_1P_37_5to75MHz;
    u8LPLL_LOOP_GAIN = 20;
    u8LPLL_LOOP_DIV = 40;
#elif PANEL_VBY1_1CH_8Bit
    ucLPLL_Type = E_PNL_SUPPORTED_LPLL_VBY1_1ch_8bit_1P_37_5to75MHz;
    u8LPLL_LOOP_GAIN = 20;
    u8LPLL_LOOP_DIV = 30;
#elif PANEL_VBY1_2CH_10Bit
    ucLPLL_Type = E_PNL_SUPPORTED_LPLL_VBY1_2ch_10bit_1P_75to150MHz;
    u8LPLL_LOOP_GAIN = 20;
    u8LPLL_LOOP_DIV = 20;
#elif PANEL_VBY1_2CH_8Bit
    ucLPLL_Type = E_PNL_SUPPORTED_LPLL_VBY1_2ch_8bit_1P_75to150MHz;
    u8LPLL_LOOP_GAIN = 20;
    u8LPLL_LOOP_DIV = 15;
#elif PANEL_VBY1_4CH_10Bit
    ucLPLL_Type = E_PNL_SUPPORTED_LPLL_VBY1_4ch_10bit_2P_75to150MHz;
    u8LPLL_LOOP_GAIN = 20;
    u8LPLL_LOOP_DIV = 20;
#elif PANEL_VBY1_4CH_8Bit
    ucLPLL_Type = E_PNL_SUPPORTED_LPLL_VBY1_4ch_8bit_2P_75to150MHz;
    u8LPLL_LOOP_GAIN = 20;
    u8LPLL_LOOP_DIV = 15;
#elif PANEL_VBY1_8CH_10Bit
    ucLPLL_Type = E_PNL_SUPPORTED_LPLL_VBY1_8ch_10bit_2P_150to300MHz;
    u8LPLL_LOOP_GAIN = 20;
    u8LPLL_LOOP_DIV = 10;
#elif PANEL_VBY1_8CH_8Bit
    ucLPLL_Type = E_PNL_SUPPORTED_LPLL_VBY1_8ch_8bit_2P_150to300MHz;
    u8LPLL_LOOP_GAIN = 20;
    u8LPLL_LOOP_DIV = 7.5;
#elif PANEL_EDP
    if( u32ODCLK_KHZ <= 20*1000UL )    // 12.5MHz~20MHz
    {
        ucLPLL_Type = E_PNL_SUPPORTED_LPLL_eDP_High_bit_rate_12_5to20MHz;
        u8LPLL_LOOP_GAIN = 16;
        u8LPLL_LOOP_DIV = 112;
    }
    else if( u32ODCLK_KHZ <= 375*100UL ) // 20MHz~37.5MHz
    {
        ucLPLL_Type = E_PNL_SUPPORTED_LPLL_eDP_High_bit_rate_20to37_5MHz;
        u8LPLL_LOOP_GAIN = 16;
        u8LPLL_LOOP_DIV = 56;
    }
    else if( u32ODCLK_KHZ <= 75*1000UL ) // 37.5MHz~75MHz
    {
        ucLPLL_Type = E_PNL_SUPPORTED_LPLL_eDP_High_bit_rate_37_5to75MHz;
        u8LPLL_LOOP_GAIN = 16;
        u8LPLL_LOOP_DIV = 28;
    }
    else if( u32ODCLK_KHZ <= 150*1000UL ) // 75MHz~150MHz
    {
        ucLPLL_Type = E_PNL_SUPPORTED_LPLL_eDP_High_bit_rate_75to150MHz;
        u8LPLL_LOOP_GAIN = 16;
        u8LPLL_LOOP_DIV = 14;
    }
    else // 150MHz~300MHz
    {
        ucLPLL_Type = E_PNL_SUPPORTED_LPLL_eDP_High_bit_rate_150to300MHz;
        u8LPLL_LOOP_GAIN = 16;
        u8LPLL_LOOP_DIV = 7;
    }
#endif


    //mStar_SetIVS();

#ifdef MST9U_FPGA
    if(PanelWidth==800&&PanelHeight==600)
        u32Factor = (float)(0x6C0000);//For 800x600 panel
    else
        u32Factor = (float)(0xA10000);//For 640x480 panel
    u32i_gain = 0x3;
    u32p_gain = 0x4;
    u8LPLL_LOOP_GAIN = u8LPLL_LOOP_GAIN;
    u8LPLL_LOOP_DIV = u8LPLL_LOOP_DIV;
    u32ODCLK_KHZ = u32ODCLK_KHZ;
    ucLPLL_Type = ucLPLL_Type;
    uctemp = uctemp;
#else
    u32Factor = (float)((float)u32MPLL_KHZ/u8LPLL_LOOP_DIV)*((float)524288UL*(float)u8LPLL_LOOP_GAIN/u32ODCLK_KHZ);
    u32i_gain = ( float )(( float )30*524288/8/(PanelHTotal/PANEL_H_DIV)/(WORD)PanelVTotal*u8LPLL_LOOP_GAIN )/(float)(( ((msReadByte(REG_003819)>>4) + ((msReadByte(REG_00381C)&BIT1)<<1)) +1)*u8LPLL_LOOP_DIV);

    u32i_gain = 5 + GetLog(u32i_gain);
    u32p_gain = u32i_gain + 1;
    for(uctemp = 0; uctemp <LPLL_REG_NUM; uctemp++)
    {
        msWrite2ByteMask((BANK_SC3+(LPLLSettingTBL[ucLPLL_Type][uctemp].address)*2), LPLLSettingTBL[ucLPLL_Type][uctemp].value, LPLLSettingTBL[ucLPLL_Type][uctemp].mask);
    }
#endif
    msWriteBit(REG_140518,FALSE,_BIT3); // lock input //20081110 Frame pll enable
    msWrite3Byte(REG_14051E,u32Factor);//SET Value
    msWriteByteMask(REG_140516,(BYTE)u32i_gain       ,BIT0|BIT1|BIT2|BIT3);//i_gain
    msWriteByteMask(REG_140516,((BYTE)u32p_gain)<<4  ,BIT4|BIT5|BIT6|BIT7);//p_gain
    msWriteByteMask(REG_140517,(BYTE)u32i_gain       ,BIT0|BIT1|BIT2|BIT3);//i_gain_phase
    msWriteByteMask(REG_140517,((BYTE)u32p_gain)<<4  ,BIT4|BIT5|BIT6|BIT7);//p_gain_phase

}

void drvmStar_SetupFreeRunMode(BYTE SCDetWin)
{
    msDrvSetFrameSyncMode(FLM_NO_LOCK, SCDetWin);
    Set_FreeRunModeFlag(SCDetWin);
}

void msDrvSetFrameSyncMode( FSyncMode ucFrameLockMode, BYTE SCDetWin )
{
    BOOL bEnableFramePLLMode = 0;
    BYTE ucLockYLine = 0;
    WORD wOutputVtotal = 0;
    WORD wModeVtotal = 0,wK,wLockPoint = 0,wCVcntFreezePoint = 0;
    BYTE InputPort = MapScaler2Port(SCDetWin);

    if(SCDetWin!=SCL0_MAIN)
        return;

    if( ucFrameLockMode == g_ScalerInfo[SCDetWin].u8SiSyncMode )
        return;
    else
        g_ScalerInfo[SCDetWin].u8SiSyncMode = ucFrameLockMode;

    drvmStar_printData("\r\n[msSetFrameSyncMode] %d",ucFrameLockMode);

    msWriteBit(SCL0_01_07, TRUE, _BIT7); // Frame Lock Always ON
    //20090312_1 25Hz & 30Hz double frame rate
    msWrite2Byte(SCL0_20_34, 0x08);
    msWrite2Byte(SCL0_20_36, 0x0A);
    msWriteBit(REG_140518,FALSE,_BIT3);
    //msSetLPLL_PIGainPhase();
#if 0//temp 20130409
    #ifdef FRAME_SYNC_NEW_LOCK_DEMO
    g_bFrameSync_Status = ucFrameLockMode;
    #endif
#endif
    switch( ucFrameLockMode )
    {
        case FLM_NO_LOCK:
            bEnableFramePLLMode = 1;
            wOutputVtotal = PanelVTotal;
            ucLockYLine = 0x00;
            //Force 1, msWriteBit(SCL0_0F_AC, FALSE, _BIT1);
            break;

        case FLM_FBL:
            bEnableFramePLLMode = 0;
            wOutputVtotal = 0x1000;
            ucLockYLine = 0x03;
            msDrvSCCommonWrite( 0xFF, TRUE);
            msWrite2ByteMask(SCL0_10_39, _BIT0, _BIT0);
            msWriteByte(SCL0_10_21, 0x46);
            msDrvSCCommonWrite( 0xFF, FALSE);
            msWrite2ByteMask(SCL0_10_39, 0x00, _BIT0);
            break;

        case FLM_FB_TUNE_VT: // Old lock
            bEnableFramePLLMode = 1;
            wOutputVtotal = 0x7FF;
            ucLockYLine = 0x04;
            break;

        case FLM_FBL_FRAMEPLL:
            bEnableFramePLLMode = 1;
            //cloude    suggestion
            ucLockYLine = 0x02; //0x03;
            if( INPUT_IS_VGA(InputPort) )
            {
                wModeVtotal = StandardMode[SrcModeIndex(InputPort)].VTotal;
            }
            else
            {
                if(SrcModeIndex(InputPort) == MD_720X576I_50)
                    wModeVtotal = 625;
                else
                    wModeVtotal = 525;
            }

            wOutputVtotal = ((DWORD)PANEL_HEIGHT*wModeVtotal+(msDrvIP1ReadImageHeight(SCDetWin)/2))/msDrvIP1ReadImageHeight(SCDetWin);

            if( InterlaceModeFlag(InputPort) )
                wK = ((12*PANEL_HEIGHT) + (msDrvIP1ReadImageHeight(SCDetWin)/2))/(msDrvIP1ReadImageHeight(SCDetWin)); //20081203
            else
                wK = ((12*PANEL_HEIGHT) + msDrvIP1ReadImageHeight(SCDetWin))/(2*msDrvIP1ReadImageHeight(SCDetWin)); //20081203

            wLockPoint = wOutputVtotal - wK -2;     //20081203
            wCVcntFreezePoint = wLockPoint - 2;


            drvmStar_printData("\r\n wModeVtotall = %d",wModeVtotal);
            drvmStar_printData("\r\n wK %d",wK);
            drvmStar_printData("\r\n ucLockPoint %x",wLockPoint);
            drvmStar_printData("\r\n ucCVcntFreezePoint %x",wCVcntFreezePoint);

            //Force 1, msWriteBit(SCL0_0F_AC,TRUE,_BIT1);
            //--- ori code ---//
            //msWrite2Byte(SCF_AA,wOutputVtotal-0x0E);
            //msWrite2Byte(SCF_A8,wOutputVtotal-0x10);
            //--- cloud suggestion ---//
            msWrite2ByteMask(SCL0_01_38, 1<<4, _BIT9|_BIT8|_BIT7|_BIT6|_BIT5|_BIT4);   //delay line

            msWrite2Byte(SCL0_0F_AA,wLockPoint);
            msWrite2Byte(SCL0_0F_A8,wCVcntFreezePoint);
            msWriteByteMask(SCL0_10_3D, 0, _BIT7);
            msWriteByteMask(SCL0_10_21, _BIT2, _BIT2);
            break;

        case FLM_FB_TUNE_CLK: // New lock
            msDrvSetFrameDivider(V_FREQ_IN(MapScaler2Port(SCL0_MAIN)));
            bEnableFramePLLMode = 1;
#if 0//temp 20130409
        #if defined(MS_3D_HDMI) || defined(MS_3D_CL)
            if( g_bStereView )
                wOutputVtotal = g_wOutputVTotal;
            else
        #endif
#endif
            wOutputVtotal = PanelVTotal;
            ucLockYLine = 0x04;
            //Force 1, msWriteBit(SCL0_0F_AC, FALSE, _BIT1);

            break;

        //20090312_1 25Hz & 30Hz double frame rate
        case FLM_FB_TUNE_CLK_DOUBLE:
            bEnableFramePLLMode = 1;
            wOutputVtotal = PanelVTotal;
            ucLockYLine = 0x04;
            //msWriteByte(REG_3117,msReadByte(REG_3117)-0x11);
            break;

        case FLM_FB_NEW_LOCK:
            bEnableFramePLLMode = 1;
            ucLockYLine = 0x02;
            wModeVtotal = StandardMode[SrcModeIndex(InputPort)].VTotal;
            wOutputVtotal = PanelVTotal;
            msWrite2Byte(SCL0_10_1A, wOutputVtotal-1); // Output V total
            Delay1ms(50);

            if(((DWORD)StandardModeResolution[StandardModeGroup(InputPort)].DispHeight * 1000/ StandardMode[SrcModeIndex(InputPort)].VTotal) >
                    ((DWORD)PANEL_HEIGHT*1000/wOutputVtotal))
            {
                wK = (((DWORD)wOutputVtotal*(StandardModeResolution[StandardModeGroup(InputPort)].DispHeight+ 12)) / StandardMode[SrcModeIndex(InputPort)].VTotal )
                            - PANEL_HEIGHT; // new
            }
            else
            {
                wK = msDrvIP1ReadAutoVStart(SCDetWin) + 12;
            }
            wLockPoint = msRead2Byte(SCL0_20_82) -6;
            msWrite2Byte(SCL0_20_34, wLockPoint - 8);
            msWrite2Byte(SCL0_20_36, wLockPoint);

            wLockPoint = wOutputVtotal - wK - 1;
            wCVcntFreezePoint = wLockPoint - 2;

            drvmStar_printData("\r\n wModeVtotall = %d",wModeVtotal);
            drvmStar_printData("\r\n woutputVtotall = %d",wOutputVtotal);
            drvmStar_printData("\r\n ucLockPoint %x",wLockPoint);
            drvmStar_printData("\r\n ucCVcntFreezePoint %x",wCVcntFreezePoint);

            msWrite2Byte(SCL0_0F_AA,wLockPoint);
            msWrite2Byte(SCL0_0F_A8,wCVcntFreezePoint);


            //Force 1, msWriteBit(SCL0_0F_AC, TRUE, _BIT1);
            //msWriteByteMask(SC10_3D, 0, _BIT7);
            //msWriteByteMask(SC10_21, _BIT2, _BIT2);

            break;

        case FLM_INIT:
        default:
            break;
    }

    msWriteBit(SCL0_00_02,TRUE,_BIT0);//double buffer enable
    msWriteBit(SCL0_00_02,TRUE,_BIT2);//double buffer load by vsync


    msWriteBit(SCL0_10_39, bEnableFramePLLMode, _BIT0); // Output freerun enable

    msWriteByteMask(SCL0_01_12, ucLockYLine, 0x0F);

    msWriteBit(REG_140518,bEnableFramePLLMode,_BIT3); // lock input //20081110 Frame pll enable
    msDrvSCCommonWrite( 0xFF, TRUE);
    msWrite2Byte(SCL0_10_1A, wOutputVtotal-1); // Output V total
    msDrvSCCommonWrite( 0xFF, FALSE);

}

void msDrvSetTimingGen(BYTE SCDetWin)
{
    DWORD u32SCRegBase = SCRegBase(SCDetWin);
    WORD u16ImgHDEStart = PanelHStart/(msDrvISSCDualPort() ? 2:1) + g_ScalerInfo[SCDetWin].sSiOP2DisplayWin.x;
    WORD u16ImgVDEStart = g_ScalerInfo[SCDetWin].sSiOP2DisplayWin.y;
    WORD u16ImgHDEEnd = u16ImgHDEStart + g_ScalerInfo[SCDetWin].sSiOP2DisplayWin.width - 1;
    WORD u16ImgVDEEnd = u16ImgVDEStart + g_ScalerInfo[SCDetWin].sSiOP2DisplayWin.height - 1;
    BYTE u8GuardBandLeft = 0, u8GuardBandRight = 0;

#if !ENABLE_SUBSCOUT
    if((SCDetWin&BIT0) == 0) // sub
        return;
#endif

    if( g_ScalerInfo[SCDetWin].sSiOP2GuardBand.u8Left )
        u8GuardBandLeft = g_ScalerInfo[SCDetWin].u8SiGuardBandOut;
    else if( g_ScalerInfo[SCDetWin].sSiOP2GuardBand.u8Right )
        u8GuardBandRight = g_ScalerInfo[SCDetWin].u8SiGuardBandOut;

    WaitVOutputBlankling();

    if( (g_ScalerInfo[SCDetWin].eSiDispStatus != SC_STATUS_IDLE)
      &&(g_ScalerInfo[SCDetWin].sSiOP2DisplayWin.width != 0) ) // for 2 sc ==> 1 sc case
    {
        if( SCDetWin & BIT0 )//Main
        {
            msWriteByteMask(u32SCRegBase+SC10_03, 0, BIT5); //[5]: main timing gen mute enable
            msWrite2ByteMask(u32SCRegBase+SC10_10,(u16ImgHDEStart-u8GuardBandLeft),0x1FFF);//[12:0]hmDE_start
            msWrite2ByteMask(u32SCRegBase+SC10_12,(u16ImgHDEEnd+u8GuardBandRight),0x1FFF);//[12:0]hmDE_end
            msWrite2ByteMask(u32SCRegBase+SC10_14,u16ImgVDEStart,0x0FFF);//[11:0]vmDE_start
            msWrite2ByteMask(u32SCRegBase+SC10_16,u16ImgVDEEnd,0x0FFF);//[11:0]vmDE_end
        }
        else//Sub
        {
            msWriteByteMask(u32SCRegBase+SC10_03, 0, BIT4); //[5]: main timing gen mute enable
            msWrite2ByteMask(u32SCRegBase+SC0F_0E,(u16ImgHDEStart-u8GuardBandLeft),0x0FFF);//[11:0]shDE_start
            msWrite2ByteMask(u32SCRegBase+SC0F_10,(u16ImgHDEEnd+u8GuardBandRight),0x0FFF);//[11:0]shDE_end
            msWrite2ByteMask(u32SCRegBase+SC0F_12,u16ImgVDEStart,0x0FFF);//[11:0]svDE_start
            msWrite2ByteMask(u32SCRegBase+SC0F_14,u16ImgVDEEnd,0x0FFF);//[11:0]svDE_end

            msWriteByteMask(u32SCRegBase+SC20_20, _BIT1, _BIT1);
        }
    }
    else
    {
        if( SCDetWin & BIT0 )//Main
        {
            msWriteByteMask(u32SCRegBase+SC10_03, BIT5, BIT5); //[5]: main timing gen mute enable
            msWrite2ByteMask(u32SCRegBase+SC10_10, 0x00, 0x1FFF);//[12:0]hmDE_start
            msWrite2ByteMask(u32SCRegBase+SC10_12, 0x00, 0x1FFF);//[12:0]hmDE_end
            msWrite2ByteMask(u32SCRegBase+SC10_14, 0x00, 0x0FFF);//[11:0]vmDE_start
            msWrite2ByteMask(u32SCRegBase+SC10_16, 0x00, 0x0FFF);//[11:0]vmDE_end
        }
        else//Sub
        {
            /*
            msWriteByteMask(u32SCRegBase+SC10_03, BIT4, BIT4); //[4]: sub timing gen mute enable
            msWrite2ByteMask(u32SCRegBase+SC0F_0E, 0x00, 0x0FFF);//[11:0]shDE_start
            msWrite2ByteMask(u32SCRegBase+SC0F_10, 0x00, 0x1FFF);//[11:0]hmDE_end
            msWrite2ByteMask(u32SCRegBase+SC0F_12, 0x00, 0x0FFF);//[11:0]svDE_start
            msWrite2ByteMask(u32SCRegBase+SC0F_14, 0x00, 0x0FFF);//[11:0]vmDE_end
            // SC20_20[1] : disable, sub disable in scaler drive
            */
            msWriteByteMask(SC20_20+u32SCRegBase, 0, _BIT1);
        }
    }

    msWrite2Byte(u32SCRegBase+SC10_8C, 0x4000); //RD suggest
    msWriteBit(u32SCRegBase+SC0F_AC, TRUE, _BIT1); // RD suggest


#ifdef MST9U_FPGA
    msWrite2Byte(0x00102F00+SC0D_A8, (msDrvISSCDualPort()?_BIT15:0)|(PANEL_WIDTH-1));
#endif
}


void msDrvSetFrameDivider( BYTE vfreq_N )
{
    ////////////////////////////////
    //  FRC ratio N/M ==> (N-1), (M-1)//
    ////////////////////////////////
    XDATA BYTE u8Index_N;
    XDATA BYTE u8Index_M;
    XDATA BYTE u8Div_N=0; // input divider
    XDATA BYTE u8Div_M=0; // output divider
    XDATA BYTE u8VFreq_M; // output clock
    XDATA DWORD u32MinDiff; // minimum differencec
    XDATA BYTE u8Index_N_step;

    u8VFreq_M = GetVfreq();
    //printData( "input VFreq_N = %d", vfreq_N);
    //printData( "output VFreq_M = %d", u8VFreq_M);
    u8Index_N_step = (SrcFlags(SrcInputPortM)&bInterlaceMode)?2:1;
    for(u32MinDiff=0xFFFFFFFF,u8Index_N=u8Index_N_step;u8Index_N<=32;u8Index_N+=u8Index_N_step)
    {
        for(u8Index_M=1;u8Index_M<=32;u8Index_M++)
        {
            if((u8Index_N*u8VFreq_M) == (u8Index_M*vfreq_N))
            {
                u8Div_N   = u8Index_N;
                u8Div_M   = u8Index_M;
                u8Index_N = 33; //break 1st loop
                break;
            }else if( labs(((DWORD)(u8Index_N<<16)/u8Index_M) - ((DWORD)(vfreq_N<<16)/u8VFreq_M)) < u32MinDiff)
            {
                u8Div_N    = u8Index_N;
                u8Div_M    = u8Index_M;
                u32MinDiff = labs(((DWORD)(u8Index_N<<16)/u8Index_M) - ((DWORD)(vfreq_N<<16)/u8VFreq_M));
            }
        }
    }

    u8Div_N = u8Div_N-1;
    u8Div_M = u8Div_M-1;
    drvmStar_printData( "Set input V_divier_N = %d", u8Div_N);
    drvmStar_printData( "Set output V_divier_M = %d", u8Div_M);
    msWriteByte(REG_140519, (u8Div_M<<4)|(u8Div_N&0x0F)); //[3:0] input frame div for frame sync, [7:4] output frame div for frame sync
    msWriteByteMask(REG_14051C, ((u8Div_M&BIT4)>>3)|(u8Div_N>>4),BIT1|BIT0); //[0] input frame div[4], [1] output frame div [4]
}

BYTE GetVfreq( void )
{
    return 60;//V_FREQ_IN(SCDetWin);
}
#if 0
void mStar_SetIVS( void )
{
    XDATA WORD ivs, Sample_vst;

    Sample_vst = ( msRead2Byte( SCL0_03_08 ) & SC_MASK_V )/((SrcFlags&bInterlaceMode)?2:1);

    if(g_SetupPathInfo.bFBMode)
    {
#if 0
        if( (g_SetupPathInfo.ucSCFmtIn == SC_FMT_IN_3D_TB)
                 && (g_SetupPathInfo.ucSCFmtOut == SC_FMT_OUT_3D_PSV) )
        {
            ivs = IVS_DEFAULT + g_SetupPathInfo.wImgSizeInV + Sample_vst;
        }
        else
        {
            ivs = IVS_DEFAULT + Sample_vst;
        }
#endif
        ivs = Sample_vst + 10*GetVfreq()/V_FREQ_IN;
    }
    else
        ivs = 4 + Sample_vst;

    _msWrite2ByteMask(SC0_B8, ivs<<4,0xFFF0);//IVS:{SC0_B9[7:0],SC0_B8[7:4]}
    msWrite2ByteMask(SCL0_0F_AA, 0x0000, 0x007FF);//ovs
}
#endif

void msDrvSetAutoDownload(ADL_CLIENT_ID eClient_id, BOOL bEnable, ADL_MODE eMode, DWORD u32Addr, WORD u16Length)
{
    switch(eClient_id)
    {
        case ADL_GAMMA_TBL:
        {
            msWriteBit(SCL0_5A_02, eMode, BIT3);
            msWriteBit(SCL0_5A_02, bEnable, BIT0);
            msWrite2Byte(SCL0_5A_04, u32Addr>>4);
            msWrite2Byte(SCL0_5A_06, u32Addr>>20);
            msWrite2Byte(SCL0_5A_10, u16Length);
            msWrite2Byte(SCL0_5A_16, u16Length);
        }
        break;
        case ADL_OD_TBL:
        {
            msWriteBit(SCL0_5A_02, eMode, BIT4);
            msWriteBit(SCL0_5A_02, bEnable, BIT1);//client2_1
            msWriteBit(SCL0_5A_42, bEnable, BIT0);//client2_2
            msWriteBit(SCL0_5A_42, bEnable, BIT1);//client2_3
            msWrite2Byte(SCL0_5A_08, u32Addr>>4);
            msWrite2Byte(SCL0_5A_0A, u32Addr>>20);
            msWrite2Byte(SCL0_5A_12, u16Length);
            msWrite2Byte(SCL0_5A_18, u16Length);
        }
        break;
        case ADL_LOCAL_DIM_TBL:
        {
            msWriteBit(SCL0_5A_02, eMode, BIT5);
            msWriteBit(SCL0_5A_02, bEnable, BIT2);
            msWrite2Byte(SCL0_5A_0C, u32Addr>>4);
            msWrite2Byte(SCL0_5A_0E, u32Addr>>20);
            msWrite2Byte(SCL0_5A_14, u16Length);
            msWrite2Byte(SCL0_5A_1A, u16Length);
        }
        break;
        case ADL_OD_GAMMA:
        {
            msWriteBit(SCL0_5A_02, eMode, BIT6);
            msWriteBit(SCL0_5A_22, bEnable, BIT0);
            msWrite2Byte(SCL0_5A_24, u32Addr>>4);
            msWrite2Byte(SCL0_5A_26, u32Addr>>20);
            msWrite2Byte(SCL0_5A_30, u16Length);
            msWrite2Byte(SCL0_5A_36, u16Length);
        }
        break;
        case ADL_OD_DEGAMMA:
        {
            msWriteBit(SCL0_5A_02, eMode, BIT7);
            msWriteBit(SCL0_5A_22, bEnable, BIT1);
            msWrite2Byte(SCL0_5A_28, u32Addr>>4);
            msWrite2Byte(SCL0_5A_2A, u32Addr>>20);
            msWrite2Byte(SCL0_5A_32, u16Length);
            msWrite2Byte(SCL0_5A_38, u16Length);
        }
        break;
        case ADL_XVYCC:
        {
            msWriteBit(SCL0_5A_03, eMode, BIT7);
            msWriteBit(SCL0_5A_22, bEnable, BIT2);
            msWrite2Byte(SCL0_5A_2C, u32Addr>>4);
            msWrite2Byte(SCL0_5A_2E, u32Addr>>20);
            msWrite2Byte(SCL0_5A_14, u16Length);
            msWrite2Byte(SCL0_5A_3A, u16Length);
        }
        break;
        case ADL_GAMMA_1024:
        {
            msWriteBit(SCL0_5A_44, eMode, BIT1);
            msWriteBit(SCL0_5A_44, bEnable, BIT0);
            msWrite2Byte(SCL0_5A_46, u32Addr>>4);
            msWrite2Byte(SCL0_5A_48, u32Addr>>20);
            msWrite2Byte(SCL0_5A_4A, u16Length);
            msWrite2Byte(SCL0_5A_4C, u16Length);
        }
        break;
        case ADL_PIU_IRE:
        {
            msWriteBit(SCL0_5A_50, eMode, BIT1);
            msWriteBit(SCL0_5A_50, bEnable, BIT0);
            msWrite2Byte(SCL0_5A_52, u32Addr>>4);
            msWrite2Byte(SCL0_5A_54, u32Addr>>20);
            msWrite2Byte(SCL0_5A_36, u16Length);
            msWrite2Byte(SCL0_5A_58, u16Length);
        }
        break;
        case ADL_T3D:
        {
            msWriteBit(SCL0_5A_64, eMode, BIT1);
            msWriteBit(SCL0_5A_64, bEnable, BIT0);
            msWrite2Byte(SCL0_5A_64, u32Addr>>4);
            msWrite2Byte(SCL0_5A_66, u32Addr>>20);
            msWrite2Byte(SCL0_5A_6A, u16Length);
            msWrite2Byte(SCL0_5A_6C, u16Length);
        }
        break;
    }
}

BYTE GetVSyncTime( BYTE InputPort )
{
    WORD freq;
    freq = HFREQ( SrcHPeriod(InputPort) );//(( DWORD )MST_CLOCK_MHZ * 10 + SrcHPeriod / 2 ) / SrcHPeriod; //get hfreq round 5
    freq = (( WORD )SrcVTotal(InputPort) * 10 + ( freq / 2 ) ) / freq;
    return ( BYTE )freq;
}

BOOL msDrvISSCDualPort(void)
{
#if (defined MST9U_FPGA_1P)||(MST9U_ASIC_1P==1)
        return FALSE;
#else
        return TRUE;
#endif
}

BYTE msDrvMFTCheckMode(void)
{
#if PANEL_OUTPUT_FMT_LR || (MST9U_ASIC_1P==1)
        return MFT_NoChg;
#elif PANEL_OUTPUT_FMT_4B
        return MFT_LR24B;
#else
        return MFT_LR2OE;
#endif
}

void msDrvMFTSetup(void)
{
    BYTE u8Mode = msDrvMFTCheckMode();

    switch(u8Mode)
    {
        case MFT_NoChg:
            msWriteBit(REG_140403, FALSE, BIT7);//MFT enable
            msWriteBit(REG_140401, FALSE, BIT7);
            break;
        case MFT_LR2OE:
            msWriteBit(REG_140403, TRUE, BIT7);
            msWriteBit(REG_140401, TRUE, BIT7);
            msWrite2ByteMask(REG_140400, (PanelWidth/2 -1), 0x0FFF);
            msWriteByteMask(REG_140407, 0x00, BIT7|BIT6);
            msWriteByteMask(REG_140408, BIT0, BIT0);
            break;
        case MFT_OE2LR:
            msWriteBit(REG_140403, TRUE, BIT7);
            msWriteBit(REG_140401, TRUE, BIT7);
            msWrite2ByteMask(REG_140400, (PanelWidth/2 -1), 0x0FFF);
            msWriteByteMask(REG_140407, BIT7, BIT7|BIT6);
            msWriteByteMask(REG_140408, 0x00, BIT0);
            break;
        case MFT_LR24B:
            msWriteBit(REG_140403, TRUE, BIT7);
            msWriteBit(REG_140401, TRUE, BIT7);
            msWrite2ByteMask(REG_140400, (PanelWidth/2 -1), 0x0FFF);
            msWriteByteMask(REG_140407, BIT6, BIT7|BIT6);
            msWriteByteMask(REG_140408, 0x00, BIT0);
            break;
    }
}
#endif
void msDrvSetOfflineComboMux(BYTE InputPort, BYTE SCDetWin)
{
    BYTE u8ComboMux = (g_InputPort[InputPort].eIPMux - 1);
    BYTE u8ComboCLK = (g_InputPort[InputPort].eIPMux - 1);
    //Combo Mux Select
    SCDetWin = SCDetWin;
    //if((g_InputPort[InputPort].eIPType)==TYPE_DP)//For Vpll selection
#if (CHIP_ID == MST9U3)

    if(g_InputPort[InputPort].eIPMux == MUX_COMBO3)
    {
        u8ComboMux--;
        u8ComboCLK--;
    }
    if (INPUT_IS_DISPLAYPORT(InputPort))
        u8ComboCLK = 6;
    msWriteByteMask(REG_162DEE, BIT2, BIT2);
    msWriteByteMask(REG_162DE9, u8ComboMux, 0x0F);
    msWriteByteMask(REG_162DF1, u8ComboCLK, 0x0F);
    msWriteByteMask(REG_120F32, u8ComboCLK, 0x0F);

#else
    if(g_InputPort[InputPort].eIPType==TYPE_DVI_DUAL)
    {
        u8ComboMux &= 0xFE;//DVI dual link is select combo0, combo2, and combo4
        u8ComboCLK &= 0xFE;//DVI dual link is select combo0, combo2, and combo4
    }
    else if(INPUT_IS_HBR2(InputPort))
    {
        u8ComboMux = 0;
        //HBR2 to L
        {
            u8ComboMux+=6;
        }

        if(INPUT_IS_HBR2_1(InputPort))
        {
            u8ComboMux+=2;
            u8ComboCLK=7;
        }
        else
        {
            u8ComboCLK=6;
        }
    }

        msWriteByteMask(REG_1630E9,u8ComboMux,0x0F);
        msWriteByteMask(REG_1630F1,u8ComboCLK,0x0F);
        msWriteByteMask(REG_120F32, u8ComboCLK,0x0F);
#endif
}

void msDrvSetOfflineIPCLKMux(BYTE u8IDCLK, BYTE SCDetWin)
{
    SCDetWin = SCDetWin;

    msWriteByteMask(REG_120F18,u8IDCLK<<2,BIT0|BIT2|BIT3);
}
void msDrvSetOfflineIPMux(BYTE InputPort, BYTE SCDetWin)
{
    DWORD XDATA u32SCRegBase = 0x00102F00;
    DWORD XDATA u32IPMuxRegBase = u32SCRegBase - 0x100;
    BYTE XDATA u8IPMuxShift, u8IPRst;
    BYTE XDATA u8Sync = 0, u8Mux = 0, u8VD = 0, bInput10b = 0, u8RegSc1_48 = 0, u8RegSc1_49 = 0, u8RegSc1_4A = 0, u8RegSc1_4C = 0;
    BYTE XDATA u8RegSc1_42 = 0,u8RegSc1_3B =0;
    WORD XDATA u8IPMuxMask;

        u8IPMuxShift = 0x08;
        u8IPMuxMask = 0x0F00;
        u8IPRst = RST_IP;

    if( g_InputPort[InputPort].eIPType & TYPE_ANALOG )
    {
        u8Sync = _BIT3;
        u8Mux = ISEL_ANALOG;
        u8VD = 0;
        bInput10b = 1;
        u8RegSc1_48 = 0x01;
        u8RegSc1_49 = 0x00;
        u8RegSc1_4A = 0x00;
        u8RegSc1_4C = 0x00;
        u8RegSc1_42 = 0x00;
        u8RegSc1_3B = 0x00;
        msWrite2ByteMask(u32IPMuxRegBase+0x02,(WORD)IP_MUX_ADC<<u8IPMuxShift,u8IPMuxMask);
        msDrvSetOfflineIPCLKMux(ID_CLK_ADC, SCDetWin);
        msWriteByteMask(REG_1025CF,0x00,BIT0|BIT1);
    }
    else if( INPUT_IS_TMDS(InputPort))
    {
        u8Sync = _BIT6 | _BIT3 | _BIT1 | _BIT0;
        u8Mux = ISEL_COMBO|SCALER_STYPE_HV;
        u8VD = 0;
        bInput10b = 1;
        u8RegSc1_48 = 0x00;
        u8RegSc1_4C = 0x00;
        u8RegSc1_42 = 0x00;
        u8RegSc1_3B = 0x00;
        msWrite2ByteMask(u32IPMuxRegBase+0x02,(WORD)IP_MUX_COMBO<<u8IPMuxShift,u8IPMuxMask);
        msDrvSetOfflineComboMux(InputPort, SCDetWin);
        msDrvSetOfflineIPCLKMux(ID_CLK_COMBO, SCDetWin);
    }
    else if(INPUT_IS_DISPLAYPORT(InputPort))
    {
        u8Sync = _BIT4 | _BIT3 | _BIT1 | _BIT0;
        u8Mux = ISEL_COMBO|SCALER_STYPE_HV;
        u8VD = BIT7;
        bInput10b = 1;
        u8RegSc1_48 = 0x00;
        u8RegSc1_4C = 0x00;
            u8RegSc1_42 = 0x3C;
        u8RegSc1_3B = BIT5;
        msWrite2ByteMask(u32IPMuxRegBase+0x02,(WORD)IP_MUX_COMBO<<u8IPMuxShift,u8IPMuxMask);
        msDrvSetOfflineComboMux(InputPort, SCDetWin);
        msDrvSetOfflineIPCLKMux(ID_CLK_COMBO, SCDetWin);
    }
    else //if( g_InputPort[InputPort].eIPType & TYPE_VD )
    {
        u8Sync = 0x9B;
        u8Mux = ISEL_VD|SCALER_STYPE_HV;
        u8VD = _BIT2;
        bInput10b = 0;
        u8RegSc1_48 = 0x00;
        u8RegSc1_4C = 0x00;
        u8RegSc1_42 = 0x00;
        u8RegSc1_3B = 0x00;
        msWrite2ByteMask(u32IPMuxRegBase+0x02,(WORD)IP_MUX_VD<<u8IPMuxShift,u8IPMuxMask);
        msDrvSetOfflineIPCLKMux(ID_CLK_VD, SCDetWin);
    }
    

    {
        msWriteByte(u32SCRegBase+SC13_04, u8Mux);
        msWriteByte(u32SCRegBase+SC13_05, u8VD);
        msWriteByte(u32SCRegBase+SC13_06, u8Sync);
        msWriteBit(u32SCRegBase+SC13_07, bInput10b, _BIT3);
        msWriteBit(u32SCRegBase+SC13_3B, u8RegSc1_3B, _BIT5);
        msWriteByte(u32SCRegBase+SC13_42, u8RegSc1_42);
        msWriteByte(u32SCRegBase+SC13_48, u8RegSc1_48);
        msWriteByte(u32SCRegBase+SC13_49, u8RegSc1_49);
        msWriteByte(u32SCRegBase+SC13_4A, u8RegSc1_4A);
        msWriteByte(u32SCRegBase+SC13_4C, u8RegSc1_4C);

        msWriteBit(REG_120F62, (u8Sync & _BIT6), _BIT0);
    }

    msDrvSoftwareReset( u8IPRst,  SCDetWin);

}

BYTE msDrvIP1OfflineGetInputSyncType(BYTE u8SCDetWin)
{
    BYTE XDATA u8Rtn;
    DWORD XDATA u32SCRegBase = 0x00102F00;

    u8SCDetWin = u8SCDetWin;
    u8Rtn = msReadByte(SC13_04+u32SCRegBase);

    return u8Rtn;
}
void msDrvIP1OfflineSetInputSyncType(WORD u16SCMask, BYTE u8Value)
{
    BYTE XDATA u8ScIndex=0;
    DWORD XDATA u32SCRegBase = 0x00102F00;

    u16SCMask = u16SCMask;
    msWriteByte(u32SCRegBase+SC13_04, u8Value);
}

///////////////////////////////////////////////////////////////////////////////
// <Function>: msDrvSoftwareReset
//
// <Description>: MST chip software reset.
//
// <Parameter>:  -  <Flow>  -  <Description>
//-----------------------------------------------------------------------------
// ucParam       -    In    -  Software reset parameter
///////////////////////////////////////////////////////////////////////////////
void msDrvSoftwareReset(DWORD ucParam, BYTE SCDetWin)
{
    WORD XDATA ucParam1, ucParam2;
    DWORD XDATA u32SCRegBase = 0x00102F00;

    SCDetWin = SCDetWin;
    ucParam1 = (BYTE)(ucParam & 0xFF);
    ucParam2 = (WORD)(ucParam & 0xFFFF00) >> 8;


    if( ucParam1 )
    {
        SC0_SCALER_RESET(ucParam1, u32SCRegBase);
        SC0_SCALER_RESET(0x00, u32SCRegBase);
    }

    if( ucParam2 )
    {
        msWrite2Byte(REG_10250E, ucParam2);//(BK1_0D, ucParam2);
        msWrite2Byte(REG_10250E, 0x00);
    }

}

void msDrvIP1OfflineAdcSetCoast(WORD u16SCMask, BYTE u8Enable, BYTE u8Start, BYTE u8End)
{
    BYTE XDATA u8ScIndex=0;
    DWORD XDATA u32SCRegBase = 0x00102F00;
    u16SCMask = u16SCMask;

    msWriteByte(SC13_48+u32SCRegBase, u8Enable);
    msWriteByte(SC13_49+u32SCRegBase, u8Start);
    msWriteByte(SC13_4A+u32SCRegBase, u8End);

}

void msDrvIP1OfflineAdcGlitchRemoval(WORD u16SCMask, BYTE u8Value)
{
    BYTE XDATA u8ScIndex=0;
    DWORD XDATA u32SCRegBase = 0x00102F00;

    u16SCMask = u16SCMask;
    msWriteByte(SC13_4C+u32SCRegBase, u8Value);

}
#ifdef _WORK_ON_PM_

///////////////////////////////////////////////////////////////////////////////
// <Function>: msDrvIP1AutoRange
//
// <Description>: Set auto function range
//
// <Parameter>:  -  <Flow>  -  <Description>
//-----------------------------------------------------------------------------
// u8SCDetWin
// sDisplayWin
// bEnable
///////////////////////////////////////////////////////////////////////////////
void msDrvIP1AutoRange(BYTE u8SCDetWin, ST_WINDOW_INFO *sDisplayWin, Bool bEnable)
{
    DWORD u32SCRegBase = SCRegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
    {
          msWriteByteMask(SC01_53+u32SCRegBase, bEnable, BIT0);
          msWrite2Byte(SC01_54+u32SCRegBase, sDisplayWin->y);
          msWrite2Byte(SC01_56+u32SCRegBase, sDisplayWin->x);
          msWrite2Byte(SC01_58+u32SCRegBase, sDisplayWin->height);
          msWrite2Byte(SC01_5A+u32SCRegBase, sDisplayWin->width);

    }
    else
    {
          msWriteByteMask(SC03_53+u32SCRegBase, bEnable, BIT0);
          msWrite2Byte(SC03_54+u32SCRegBase, sDisplayWin->y);
          msWrite2Byte(SC03_56+u32SCRegBase, sDisplayWin->x);
          msWrite2Byte(SC03_58+u32SCRegBase, sDisplayWin->height);
          msWrite2Byte(SC03_5A+u32SCRegBase, sDisplayWin->width);

    }

    drvmStar_printData("\n u8SCDetWin = %x", u8SCDetWin);
    drvmStar_printData("\n  sDisplayWin->y = %x",  sDisplayWin->y);
    drvmStar_printData("\n  sDisplayWin->x = %x",  sDisplayWin->x);
    drvmStar_printData("\n sDisplayWin->height = %x", sDisplayWin->height);
    drvmStar_printData("\n sDisplayWin->width = %x", sDisplayWin->width);

}
// Auto position
void msDrvIP1AutoPositionResultReady(BYTE u8SCDetWin, BYTE u8InputPort)
{
    DWORD u32SCRegBase = SCRegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        WaitAutoStatusReady(SC01_20+u32SCRegBase, BIT1, u8InputPort);
    else
        WaitAutoStatusReady(SC03_20+u32SCRegBase, BIT1, u8InputPort);
}
void msDrvIP1AutoPositionSetValidValue(BYTE u8SCDetWin, BYTE u8Value)
{
    DWORD u32SCRegBase = SCRegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        msWriteByteMask(SC01_21+u32SCRegBase, (u8Value) << 4, 0xF0);
    else
        msWriteByteMask(SC03_21+u32SCRegBase, (u8Value) << 4, 0xF0);
}
BYTE msDrvIP1AutoPositionReadValidValue(BYTE u8SCDetWin)
{
    BYTE u8Rtn;
    DWORD u32SCRegBase = SCRegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        u8Rtn = msReadByte(SC01_21+u32SCRegBase)>>4;
    else
        u8Rtn = msReadByte(SC03_21+u32SCRegBase)>>4;

    return u8Rtn;
}
WORD msDrvIP1AutoPositionReadHstart(BYTE u8SCDetWin, BYTE u8InputPort)
{
    WORD u8Rtn;
    DWORD u32SCRegBase = SCRegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        u8Rtn = GetAutoValue(SC01_26 + u32SCRegBase, u8InputPort);
    else
        u8Rtn = GetAutoValue(SC03_26 + u32SCRegBase, u8InputPort);

    return u8Rtn;
}

WORD msDrvIP1AutoPositionReadHend(BYTE u8SCDetWin, BYTE u8InputPort)
{
    WORD u8Rtn;
    DWORD u32SCRegBase = SCRegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        u8Rtn = GetAutoValue(SC01_2A + u32SCRegBase, u8InputPort);
    else
        u8Rtn = GetAutoValue(SC03_2A + u32SCRegBase, u8InputPort);

    return u8Rtn;
}

WORD msDrvIP1AutoPositionReadVstart(BYTE u8SCDetWin, BYTE u8InputPort)
{
    WORD u8Rtn;
    DWORD u32SCRegBase = SCRegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        u8Rtn = GetAutoValue(SC01_24 + u32SCRegBase, u8InputPort);
    else
        u8Rtn = GetAutoValue(SC03_24 + u32SCRegBase, u8InputPort);

    return u8Rtn;
}

WORD msDrvIP1AutoPositionReadVend(BYTE u8SCDetWin, BYTE u8InputPort)
{
    WORD u8Rtn;
    DWORD u32SCRegBase = SCRegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        u8Rtn = GetAutoValue(SC01_28 + u32SCRegBase, u8InputPort);
    else
        u8Rtn = GetAutoValue(SC03_28 + u32SCRegBase, u8InputPort);

    return u8Rtn;
}

BYTE msDrvIP1AutoPositionReadTranstionPosition(WORD u16DelayTime, BYTE u8SCDetWin, BYTE u8InputPort)
{
    BYTE u8Rtn;
    DWORD u32SCRegBase = SCRegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        u8Rtn = GetTranstionPosition( u16DelayTime, SC01_26 + u32SCRegBase, u8InputPort );
    else
        u8Rtn = GetTranstionPosition( u16DelayTime, SC03_26 + u32SCRegBase, u8InputPort );

    return u8Rtn;
}

void msDrvIP1AutoPhaseResultReady(BYTE u8SCDetWin)
{
    DWORD u32SCRegBase = SCRegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        drvADC_WaitAutoStatusReady(SC01_32+u32SCRegBase, BIT1);
    else
        drvADC_WaitAutoStatusReady(SC03_32+u32SCRegBase, BIT1);
}

DWORD msDrvIP1AutoPhaseReadValue(BYTE u8SCDetWin)
{
    DWORD u8Rtn;
    DWORD u32SCRegBase = SCRegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        u8Rtn = (((DWORD)msRead2Byte(SC01_36+u32SCRegBase))<<16)|msRead2Byte(SC01_34+u32SCRegBase);
    else
        u8Rtn = (((DWORD)msRead2Byte(SC03_36+u32SCRegBase))<<16)|msRead2Byte(SC03_34+u32SCRegBase);

    return u8Rtn;
}

// Auto Phase Nose Mask.
// #000: Mask 0 bit, #001: Mask 1 bit. #010: Mask 2 bit. #011: Mask 3 bit. #100: Mask 4 bit. #101: Mask 5 bit. #110: Mask 6 bit. #111: Mask 7 bit.
void msDrvIP1AutoPhaseSetNoiseMask(BYTE u8Value, BYTE u8SCDetWin)
{
    DWORD u32SCRegBase = SCRegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        msWriteByteMask(SC01_32+u32SCRegBase, u8Value<<2|BIT0, 0x1D);
    else
        msWriteByteMask(SC03_32+u32SCRegBase, u8Value<<2|BIT0, 0x1D);
}


// scaler
void msDrvIP1SetOutputLockMode(BYTE u8Value, BYTE u8SCDetWin)
{
    DWORD u32SCRegBase = SCRegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        msWriteByteMask(SC01_07+u32SCRegBase, (u8Value)?(BIT7):(0) , BIT7);
    else
        msWriteByteMask(SC03_07+u32SCRegBase, (u8Value)?(BIT7):(0) , BIT7);
}
void msDrvIP1SetOutputFreerun(BYTE u8Value, BYTE u8SCDetWin)
{
    DWORD u32SCRegBase = SCRegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        msWriteByteMask(SC01_04+u32SCRegBase, (u8Value)?(BIT7):(0) , BIT7);
    else
        msWriteByteMask(SC03_04+u32SCRegBase, (u8Value)?(BIT7):(0) , BIT7);
}
void msDrvIP1SetHperiodDetectMode(BOOL bEnable8lines, BYTE u8SCDetWin)
{
    DWORD u32SCRegBase = SCRegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        msWriteByte(SC01_41+u32SCRegBase, (bEnable8lines)?(BIT7):(0));
    else
        msWriteByte(SC03_41+u32SCRegBase, (bEnable8lines)?(BIT7):(0));
}
#endif
WORD msDrvIP1OfflineReadHperiod(BYTE u8SCDetWin)
{
    WORD XDATA u8Rtn;
    DWORD XDATA u32SCRegBase = 0x00102F00;
    u8SCDetWin = u8SCDetWin;
    u8Rtn = (msRead2Byte(SC13_40+u32SCRegBase) & MASK_14BIT);

    return u8Rtn;
}
WORD msDrvIP1OfflineReadVtotal(BYTE u8SCDetWin)
{
    WORD XDATA u8Rtn;
    DWORD XDATA u32SCRegBase = 0x00102F00;
    u8SCDetWin = u8SCDetWin;

    u8Rtn = (msRead2Byte(SC13_3E+u32SCRegBase) & MASK_13BIT);

    return u8Rtn;
}
BYTE msDrvIP1OfflineReadSyncStatus(BYTE u8SCDetWin)
{
    BYTE XDATA u8Rtn;
    DWORD XDATA u32SCRegBase = 0x00102F00;
    u8SCDetWin = u8SCDetWin;

    u8Rtn = msReadByte(SC13_3D+u32SCRegBase);

    return u8Rtn;
}
#ifdef _WORK_ON_PM_
WORD msDrvIP1ReadAutoHStart(BYTE u8SCDetWin)
{
    WORD u8Rtn;
    DWORD u32SCRegBase = SCRegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        u8Rtn = msRead2Byte(SC01_26+u32SCRegBase);
    else
        u8Rtn = msRead2Byte(SC03_26+u32SCRegBase);

    return u8Rtn;
}
WORD msDrvIP1ReadAutoHEnd(BYTE u8SCDetWin)
{
    WORD u8Rtn;
    DWORD u32SCRegBase = SCRegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        u8Rtn = msRead2Byte(SC01_2A+u32SCRegBase);
    else
        u8Rtn = msRead2Byte(SC03_2A+u32SCRegBase);

    return u8Rtn;
}
WORD msDrvIP1ReadAutoVStart(BYTE u8SCDetWin)
{
    WORD u8Rtn;
    DWORD u32SCRegBase = SCRegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        u8Rtn = msRead2Byte(SC01_24+u32SCRegBase);
    else
        u8Rtn = msRead2Byte(SC03_24+u32SCRegBase);

    return u8Rtn;
}
WORD msDrvIP1ReadAutoVEnd(BYTE u8SCDetWin)
{
    WORD u8Rtn;
    DWORD u32SCRegBase = SCRegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        u8Rtn = msRead2Byte(SC01_28+u32SCRegBase);
    else
        u8Rtn = msRead2Byte(SC03_28+u32SCRegBase);

    return u8Rtn;
}
WORD msDrvIP1ReadAutoWidth(BYTE u8SCDetWin)
{
    return (msDrvIP1ReadAutoHEnd(u8SCDetWin) - msDrvIP1ReadAutoHStart(u8SCDetWin) + 1);
}
WORD msDrvIP1ReadAutoHeight(BYTE u8SCDetWin)
{
    return (msDrvIP1ReadAutoVEnd(u8SCDetWin) - msDrvIP1ReadAutoVStart(u8SCDetWin) + 1);
}
WORD msDrvIP1ReadImageHeight(BYTE u8SCDetWin)
{
    WORD u8Rtn;
    DWORD u32SCRegBase = SCRegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        u8Rtn = (msRead2Byte(SC01_0C+u32SCRegBase)&MASK_13BIT);
    else
        u8Rtn = (msRead2Byte(SC03_0C+u32SCRegBase)&MASK_13BIT);

    return u8Rtn;
}
WORD msDrvIP1ReadImageWidth(BYTE u8SCDetWin)
{
    WORD u8Rtn;
    DWORD u32SCRegBase = SCRegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        u8Rtn = (msRead2Byte(SC01_0E+u32SCRegBase)&MASK_13BIT);
    else
        u8Rtn = (msRead2Byte(SC03_0E+u32SCRegBase)&MASK_13BIT);

    return u8Rtn;
}
void msDrvIP1SetImageHeight(WORD u16Value, BYTE u8SCDetWin)
{
    DWORD u32SCRegBase = SCRegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        msWrite2Byte(SC01_0C+u32SCRegBase, (u16Value)&MASK_13BIT);
    else
        msWrite2Byte(SC03_0C+u32SCRegBase, (u16Value)&MASK_13BIT);
}
void msDrvIP1SetImageWidth(WORD u16Value, BYTE u8SCDetWin)
{
    DWORD u32SCRegBase = SCRegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        msWrite2Byte(SC01_0E+u32SCRegBase, (u16Value)&MASK_13BIT);
    else
        msWrite2Byte(SC03_0E+u32SCRegBase, (u16Value)&MASK_13BIT);
}
void msDrvIP1SetImageHStart(WORD u16Value, BYTE u8SCDetWin)
{
    DWORD u32SCRegBase = SCRegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        msWrite2Byte(SC01_0A+u32SCRegBase, (u16Value)&MASK_13BIT);
    else
        msWrite2Byte(SC03_0A+u32SCRegBase, (u16Value)&MASK_13BIT);
}
void msDrvIP1SetImageVStart(WORD u16Value, BYTE u8SCDetWin)
{
    DWORD u32SCRegBase = SCRegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        msWrite2Byte(SC01_08+u32SCRegBase, (u16Value)&MASK_13BIT);
    else
        msWrite2Byte(SC03_08+u32SCRegBase, (u16Value)&MASK_13BIT);
}

BOOL msDrvIP1IsDEOnlyMode(BYTE u8SCDetWin)
{
    BOOL u8Rtn;
    DWORD u32SCRegBase = SCRegBase(u8SCDetWin);

    u8SCDetWin = u8SCDetWin;
    if(  & BIT0 )   // MAIN
        u8Rtn = ((msReadByte(SC01_06+u32SCRegBase)&BIT6) == BIT6);
    else
        u8Rtn = ((msReadByte(SC03_06+u32SCRegBase)&BIT6) == BIT6);

    return u8Rtn;
}
#endif

BYTE msDrvIP1OfflineReadVSyncWidth(BYTE u8SCDetWin)
{
    BYTE XDATA u8Rtn;
    DWORD XDATA u32SCRegBase = 0x00102F00;
    u8SCDetWin = u8SCDetWin;

    u8Rtn = msReadByte(SC13_45+u32SCRegBase);

    return u8Rtn;
}

/////////////////////////////////////////////////////////////////////////
//////////////////////////Check Is Cable Connected/////////////////////////
/////////////////////////////////////////////////////////////////////////
#if ENABLE_CABLE_DET
Bool msAPISrcIsCableConnected( BYTE InputPort )
{
    if(INPUT_IS_MUX_ADC(InputPort))
    {
        if(hwDSUBCable_Pin)
        {
            Delay1ms(20);
            if(hwDSUBCable_Pin)
                return FALSE;
        }
    }
    else
    {
        if(msAPI_combo_IPGetCableDetect((EN_COMBO_IP_SELECT)g_InputPort[InputPort].eIPMux)==COMBO_IP_CABLE_NONE)
            return FALSE;
    }
    return TRUE;
}
#endif

#endif // end of #ifndef _DRVMSTAR_D_C_


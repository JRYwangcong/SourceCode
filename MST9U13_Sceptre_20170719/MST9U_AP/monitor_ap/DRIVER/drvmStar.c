
///////////////////////////////////////
// DRVMSTAR_V VERSION: V02
////////////////////////////////////////
#include "Global.h"
#include "drvDPRxApp.h"
#include "msEread.h"
#include "appWindow.h"
#include "appInput.h"
#include "appSystem.h"
#include "msScaler.h"
#include "mapi_combo.h"
#include "AutoFunc.h"
#include "mdrv_tmds.h"
#include "drvmsOVD.h"
#include "LPLL_Tbl.h"
#include "menufunc.h"
#include "i2c.h"
#if ENABLE_DAISY_CHAIN
#include "msDaisyChain.h"
#endif
#if ENABLE_VBY1_TO_EDP
#include "msVBy1ToEDP.h"
#endif
#include "msScaler.h"
#include "Ms_PM.h"
#include "mapi_CEC.h"
#include "appPanel.h"
#include "Mcu.h"

#ifndef _DRVMSTAR_D_C_
#define _DRVMSTAR_D_C_

#define drvmStar_DEBUG    1
#if ENABLE_MSTV_UART_DEBUG&&drvmStar_DEBUG
    #define drvmStar_printData(str, value)   printData(str, value)
    #define drvmStar_printMsg(str)           printMsg(str)
#else
    #define drvmStar_printData(str, value)
    #define drvmStar_printMsg(str)
#endif

#define _DELAY_DISP_TRIG_     12
#define _DELAY_INPUT_LINE_    6
#define _Y_LOCK_LINE_         2

BYTE  u8LPLL_LOOP_GAIN=0;
float fLPLL_LOOP_DIV=0;

BYTE u8FrameLockModeIndex=0;

static void msDrvSetFpllFastLock( BYTE SCDetWin );
void APPSystem_ActivePort_For_PowerOn( void );
void APPSystem_ActivePort_For_PowerSaving( void );
void CheckACKeyStatus( void );
extern BYTE tCOMBO_USER_HDCP22_CERTRX_TABLE[1008];


code RegTbl2Type tComboTMDSMuxTbl[]=
{
#if ENABLE_AUDIO_AUTO_MUTE
    {REG_1428, 0xAF00, 0xFF00},
#endif
    {REG_1600, 0x000F, 0xFFFF},
    {REG_1602, 0x020A, 0xFFFF},
    {REG_1606, 0x0A88, 0xFFFF},
    {REG_1608, 0x0944, 0xFFFF},
    {REG_160A, 0x08A2, 0xFFFF},
    {REG_1612, 0x0D11, 0xFFFF},
    {REG_161C, 0xFF00, 0xFFFF},
    {REG_1620, 0xFF00, 0xFFFF},
    {REG_1624, 0xFF00, 0xFFFF},
    {REG_1628, 0xFF00, 0xFFFF},
    {REG_1646, 0x081F, 0xFFFF},
    {REG_165C, 0x000D, 0xFFFF},

    {REG_1700, 0x7045, 0xFFFF},
    {REG_1706, 0x1001, 0xFFFF},
    {REG_1710, 0x8000, 0xFFFF},
    {REG_1714, 0x3130, 0xFFFF},
    {REG_1730, 0x0080, 0xFFFF},
    {REG_1752, 0x0000, 0xFFFF},
    {REG_175A, 0x0000, 0xFFFF},
    {REG_175C, 0xDFFF, 0xFFFF},
    {REG_17A2, 0x800E, 0xFFFF},
    //{REG_17AE, 0x0080, 0xFFFF},
    {REG_17AE, 0x0F80, 0xFFFF}, // 20130201 - follow RD new IP driver table
    {REG_17B0, 0x0A0A, 0xFFFF},
    {REG_17B2, 0x0A0A, 0xFFFF},
    {REG_17BC, 0x0040, 0xFFFF},   // [6]: for all mode,   VCO switch cap always on
    {REG_1782, 0x8101, 0xFFFF},
    {REG_17A8, 0x001B, 0xFFFF},
    {REG_17AA, 0x0000, 0x6000},   // Disable overwrite [D]: Phase DAC DIV select, [E]: PLL DIV select
    //{REG_17AC, 0x8FE0, 0xFFFF},
    {REG_17AC, 0x87E0, 0xFFFF}, // 20130201 - follow RD new IP driver table
    {REG_1740, 0x0001, 0xFFFF},
    {REG_17B4, 0x0010, 0xFFFF},   // [4]: for all mode,  phase dac vring LDO +5%
    {REG_174C, 0xFFFF, 0xFFFF},
    {REG_174E, 0x3FFF, 0xFFFF},
    {REG_17BE, 0x0000, 0xFFFF},
    {REG_1542, 0x0000, 0xFFFF},
    {REG_01C2, 0x0124, 0xFFFF},
    //{REG_01C4, 0x0000, 0xFFFF},
    {REG_01C6, 0x111F, 0xFFFF},
    {REG_01C8, 0x0003, 0xFFFF},
    {REG_01CA, 0x10FE, 0xFFFF},
    {REG_01CC, 0x0000, 0xFFFF},
    {REG_01CE, 0x0020, 0xFFF8},
    {REG_0C01, 0x0000, 0x0300},
    {REG_1506, 0x0000, 0x0004},
#if DHDMI_SIMPLAYHD_PATCH // SimplayHD CTS 8-18
    {REG_1508, 0x000C, 0x840C}, //km INT
    {REG_153C, 0x0002, 0x0002}, //km INT
#endif // #if DHDMI_SIMPLAYHD_PATCH // SimplayHD CTS 8-18
    {REG_1569, 0x0000, 0x0001},
    {REG_1569, 0x0001, 0x0001},
    {REG_1569, 0x0000, 0x0001},
#if ENABLE_DP_INPUT
    {REG_18E0, 0x0000, 0x0006}, //disable FT
#endif
};

code RegTbl2Type tblMIU[] =
{
#if (CHIP_ID==MST9U2)
    {REG_10123A, 0x0000, 0x8000},
    {REG_101240, 0x0100, 0x0100},  //enable flow control0
    {REG_101254, 0x4047, 0xFFFF},  //set BDMA flow control0
#else
    {REG_10123A, 0x0000, 0x8000},
    {REG_101240, 0x0100, 0x0100},  //enable flow control0
    {REG_101254, 0x8027, 0xFFFF},  //set BDMA flow control0
#endif
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
#if (CHIP_ID==MST9U3)
    {SCL0_00_80, 0x13},// od L/R common write, SC L1/R1 on top, Related to <ENABLE_SUBSC_HOLLOW_OUT>

    {SCL0_16_32, 0x30},// r thrd
    {SCL0_16_34, 0x18},// r length

    {SCL0_16_35, 0x40},
    {SCL0_16_36, 0x30},

    {SCL0_16_38, 0x18},
    {SCL0_16_39, 0x40},

    {SCL0_16_22, 0x00},
    {SCL0_16_23, 0x0F},// user weight
    {SCL0_16_74, 0xFF},// w cnt
    {SCL0_16_9A, 0x00},
    {SCL0_16_9B, 0x00},
    {SCL0_16_20, 0x00},
    {SCL0_16_21, 0x00},
    {SCL0_16_D8, 0x00},
    {SCL0_16_54, 0x98},  //set to 2200
    {SCL0_16_55, 0x08},  //set to 2200

   {SCL0_16_4E, 0x10},
   {SCL0_16_4F, 0x00},

   // {SCL0_58_54, 0x98},  //set to 2200
   // {SCL0_16_55, 0x08},  //set to 2200

    {SCL0_16_D9, 0x00},//h range
#else

    {SCL0_00_80, 0x13},// od L/R common write, SC L1/R1 on top, Related to <ENABLE_SUBSC_HOLLOW_OUT>
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
#else
    {SCL0_00_80, 0x03},// SC L1/R1 on top, Related to <ENABLE_SUBSC_HOLLOW_OUT>
#endif

#if CHIP_ID==MST9U||CHIP_ID==MST9U2||CHIP_ID==MST9U3
    {REG_MOD1_1D, PanelDClkDelay}, // DE delay, clock delay for TTL output
    {REG_MOD1_00, /*(PanelInvDClk&BIT4)can't find 20130410*/(PanelInvHSync&BIT5)|(PanelInvDE&BIT7)|(PanelInvVSync&BIT6)},
//old               |(PanelDualPort&BIT1)|(PanelSwapPort&BIT0)},

    //{REG_3089, PanelDualPort&BIT4},can't find 20130410
    {REG_MOD1_00, LVDS_TI_BitModeSel<<2},
    //{REG_3092, 0x00},can't find 20130410
    {REG_MOD1_20, 0x00},
    {REG_MOD1_21, 0x00},
    {REG_MOD1_22, 0x00},
    {REG_MOD1_23, 0x00},
    {REG_MOD1_24, 0x00},
    {REG_MOD1_25, 0x00},
    {REG_MOD1_4E, 0x38},
    {REG_MOD1_50, BIT0},
    {REG_MOD1_51, BIT7},
/////LPLL parameter init
    {REG_LPLL_0A, 0xFF},
    {REG_LPLL_0B, 0xFF},
    {REG_LPLL_0E, 0x02},
    {REG_LPLL_0D, 0x00},
    {REG_LPLL_12, 0x02},
    {REG_LPLL_13, 0x00},
    {REG_LPLL_14, 0x00},
    {REG_LPLL_15, 0xF0},
#endif
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


#if PanelDither==6  //is there still has 6bit panel?
    {SCL0_24_7E, BIT7|BIT0},  //[0]PAFRC enable [2]:dithering bits [7]:for 12bits data path , 1 for output 6bits, 0 for output 10bits or 8bits
#elif PanelDither==10 ||((PANEL_EDP && eDPTX10Bits)||(PANEL_VBY1_8CH_10Bit||PANEL_VBY1_2CH_10Bit||PANEL_VBY1_4CH_10Bit))
    #if (CHIP_ID == MST9U3)
    {SCL0_24_7E, 0}, // 10 to 10
    #else
    {SCL0_24_7E, BIT0}, // 12 to 10
    #endif
#else // 8 bits panel
    #if (CHIP_ID == MST9U3)
    {SCL0_24_7E, BIT0}, // 10 to 8
    #else
    {SCL0_24_7E, BIT2|BIT0}, // 12 to 8
    #endif

    {SCL0_24_8C, 0xAA},
    {SCL0_24_8D, 0xAA},
    {SCL0_24_8E, 0xAA},
    {SCL0_24_8F, 0xAA},
    {SCL0_24_90, 0xAA},
    {SCL0_24_91, 0xAA},
    {SCL0_24_92, 0xAA},
    {SCL0_24_93, 0xAA},
    {SCL0_24_94, 0xAA},
    {SCL0_24_95, 0xAA},

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

    //Move to msDrvmStarInit
    //{REG_003A62, SARKEY_EN|VGA_CABLE_DET_SAR_EN},   // SAR0-3
    //{REG_003A66, TMDS_CABLE_DET_SAR2_EN},           // SAR4-11 - HDMI cable det

    //{REG_000462, 0x00},
    //{REG_000463, 0x00},//disable pad pull high
};
#if CHIP_ID==MST9U||CHIP_ID==MST9U2||CHIP_ID==MST9U3
code RegUnitType2 tblInitMOD[]=
{
///////////MOD Initial Settings///////////
#if PANEL_TTL
    {REG_MFT_02, 0x0010},   //[4]:1channel LVDS mux
    {REG_MOD1_02, 0x0002},   //[1]:8bit new
    {REG_MOD1_00, 0x0001},   //[0]:REG_SW_RST
    {REG_MOD1_20, 0x0000},   //ch0~7 set to TTL
    {REG_MOD1_22, 0x0000},   //ch8~14 set to TTL
    {REG_MOD1_24, 0x0000},   //ch15~23 set to TTL
    {REG_MOD1_26, 0x0000},   //dont have this register
    {REG_MOD1_30, 0x0000},   //disable external GPIO
    {REG_MOD1_32, 0x0000},   //disable external GPIO
    {REG_MOD1_34, 0xFFFF},   //enable TTL function
    {REG_MOD1_36, 0xFFFF},   //enable TTL function
    {REG_MOD1_50, 0x8000},   //[0] disable mod power down
    {REG_MOD1_A0, 0x0000},   //lock clock gen selection
    {REG_MOD1_A2, 0x0000},   //lock clock gen selection
    {REG_MOD2_80, 0x00FF},   //enable clk of bank
    {REG_MOD2_82, 0x0000},   //
    {REG_MOD2_84, 0x0000},
    {REG_MOD2_86, 0x0000},
#elif PANEL_LVDS_1CH
    {REG_MFT_02, 0x0010},
    {REG_MOD1_04, 0x8000},   //[15]CRC enable
    {REG_MOD1_4E, 0x00F8},   //ABCDE LVDS clk enable
    {REG_MOD1_20, 0x0000},
    {REG_MOD1_28, 0x0000},   //Set to TTL output mode
    {REG_MOD1_2A, 0x0000},   //Set to TTL output mode
    {REG_MOD1_2C, 0x0000},   //Set to TTL output mode
    {REG_MOD1_0C, 0x0000},   //TTL output LOW
    {REG_MOD1_0E, 0x0000},   //TTL output LOW
    {REG_MOD1_10, 0x0000},   //TTL output LOW
    {REG_MOD1_2E, 0xFFFF},   //no function
    {REG_MOD1_30, 0x0000},   //disable external GPIO
    {REG_MOD1_32, 0x0000},   //disable external GPIO
    {REG_MOD1_00, 0x0001},   //
#if (PanelDither == 6)
    {REG_MOD1_0A, 0x200C},   //VESA bitmode, for JEIDA , REG_MOD1_0A[3:2] set to 2'b11 6bit
#elif (PanelDither == 10)
    {REG_MOD1_0A, 0x2000},   //VESA bitmode, for JEIDA , REG_MOD1_0A[3:2] set to 2'b00 10bit
#else
    {REG_MOD1_0A, 0x2008},   //VESA bitmode, for JEIDA , REG_MOD1_0A[3:2] set to 2'b10 8bit
#endif
    {REG_MOD1_50, 0x8001},   //[0] power down MOD ATOP
    {REG_MOD1_1E, 0xE400},   //channel swap register
    {REG_MOD1_60, 0x8000},   //[14:12]0>LVDS 1>miniLVDS
    {REG_MOD1_78, 0x0000},
    {REG_MOD1_A0, 0x0000},
    {REG_MOD1_A2, 0x0000},
    {REG_MOD2_80, 0x00FF},
#elif PANEL_LVDS_2CH
    {REG_MOD1_04, 0x8000},
    {REG_MOD1_4E, 0x00F8},   //ABCDE LVDS clk enable
    {REG_MOD1_20, 0x0000},
    {REG_MOD1_22, 0x0000},
    {REG_MOD1_28, 0x0000},   //Set to TTL output mode
    {REG_MOD1_2A, 0x0000},   //Set to TTL output mode
    {REG_MOD1_2C, 0x0000},   //Set to TTL output mode
    {REG_MOD1_0C, 0x0000},   //TTL output LOW
    {REG_MOD1_0E, 0x0000},   //TTL output LOW
    {REG_MOD1_10, 0x0000},   //TTL output LOW
    {REG_MOD1_2E, 0xFFFF},   //empty
    {REG_MOD1_30, 0x0000},
    {REG_MOD1_32, 0x0000},
    {REG_MOD1_00, 0x0201},
#if (PanelDither == 6)
    {REG_MOD1_0A, 0x600C},   //VESA bitmode, for JEIDA , REG_MOD1_0A[3:2] set to 2'b11 6bit
#elif (PanelDither == 10)
    {REG_MOD1_0A, 0x6000},   //VESA bitmode, for JEIDA , REG_MOD1_0A[3:2] set to 2'b00 10bit
#else
    {REG_MOD1_0A, 0x6008},   //VESA bitmode, for JEIDA , REG_MOD1_0A[3:2] set to 2'b10 8bit
#endif
    {REG_MOD1_50, 0x8001},
    #if MST9U_ASIC_1P
    {REG_MOD1_1E, 0xE400},
    #else //MST9U_ASIC_2P
    {REG_MOD1_1E, 0xE800},
    #endif
    {REG_MOD1_60, 0x8000},
    {REG_MOD1_A0, 0x0000},
    {REG_MOD1_A2, 0x0000},
    {REG_MOD2_80, 0x00FF},
#elif PANEL_LVDS_4CH
    {REG_MOD1_04, 0x8000},
    {REG_MOD1_4E, 0x00F8},
    {REG_MOD1_20, 0x0000},
    {REG_MOD1_22, 0x0000},
    {REG_MOD1_24, 0x0000},
    {REG_MOD1_28, 0x0000}, //Set to TTL output mode
    {REG_MOD1_2A, 0x0000},//Set to TTL output mode
    {REG_MOD1_2C, 0x0000},//Set to TTL output mode
    {REG_MOD1_0C, 0x0000}, //TTL output LOW
    {REG_MOD1_0E, 0x0000}, //TTL output LOW
    {REG_MOD1_10, 0x0000}, //TTL output LOW
    {REG_MOD1_2E, 0xFFFF},
    {REG_MOD1_30, 0x0000},
    {REG_MOD1_32, 0x0000},
    {REG_MOD1_00, 0x0601},
#if (PanelDither == 6)
    {REG_MOD1_0A, 0x200C},   //VESA bitmode, for JEIDA , REG_MOD1_0A[3:2] set to 2'b11 6bit
#elif (PanelDither == 10)
    {REG_MOD1_0A, 0x2000},   //VESA bitmode, for JEIDA , REG_MOD1_0A[3:2] set to 2'b00 10bit
#else
    {REG_MOD1_0A, 0x2008},   //VESA bitmode, for JEIDA , REG_MOD1_0A[3:2] set to 2'b10 8bit
#endif
    {REG_MOD1_50, 0x8001},
    {REG_MOD1_1E, 0x2000},
    {REG_MOD1_60, 0x8000},
    {REG_MOD1_78, 0x2600},
    {REG_MOD1_A0, 0x0000},
    {REG_MOD1_A2, 0x0000},
    {REG_MOD2_80, 0x00FF},
    {REG_MOD2_82, 0x0000},
    {REG_MOD2_84, 0x0000},
    {REG_MOD2_86, 0x0000},
#elif PANEL_LVDS_8CH
    {REG_MOD1_04, 0x8000},
    {REG_MOD1_4E, 0x00F8},
    {REG_MOD1_20, 0x0000},
    {REG_MOD1_22, 0x0000},
    {REG_MOD1_24, 0x0000},
    {REG_MOD1_28, 0x0000}, //Set to TTL output mode
    {REG_MOD1_2A, 0x0000}, //Set to TTL output mode
    {REG_MOD1_2C, 0x0000}, //Set to TTL output mode
    {REG_MOD1_0C, 0x0000}, //TTL output LOW
    {REG_MOD1_0E, 0x0000}, //TTL output LOW
    {REG_MOD1_10, 0x0000}, //TTL output LOW
    {REG_MOD1_2E, 0xFFFF},
    {REG_MOD1_30, 0x0000},
    {REG_MOD1_32, 0x0000},
    {REG_MOD1_00, 0x0601},
#if (PanelDither == 6)
    {REG_MOD1_0A, 0x200C},   //VESA bitmode, for JEIDA , REG_MOD1_0A[3:2] set to 2'b11 6bit
#elif (PanelDither == 10)
    {REG_MOD1_0A, 0x2000},   //VESA bitmode, for JEIDA , REG_MOD1_0A[3:2] set to 2'b00 10bit
#else
    {REG_MOD1_0A, 0x2008},   //VESA bitmode, for JEIDA , REG_MOD1_0A[3:2] set to 2'b10 8bit
#endif
    {REG_MOD1_50, 0x8001},
    {REG_MOD1_1E, 0xE400},
    {REG_MOD1_60, 0x8000},
    {REG_MOD1_78, 0x2600},
    {REG_MOD1_A0, 0x0000},
    {REG_MOD1_A2, 0x0000},
    {REG_MOD2_80, 0x00FF},
    {REG_MOD2_82, 0x0000},
    {REG_MOD2_84, 0x0000},
    {REG_MOD2_86, 0x0000},
#elif PANEL_VBY1_1CH_8Bit
    {REG_120F0E, 0x0400},
    {REG_MFT_02, 0x0011},
    {REG_MOD1_4E, 0x00F8},
    {REG_MOD1_24, 0x0000},
    {REG_MOD1_00, 0x0001},
    {REG_MOD1_0A, 0x2000},
    {REG_MOD1_50, 0x8001},
    {REG_MOD1_1E, 0xD800},
    {REG_MOD1_60, 0xD000},
    {REG_MOD1_78, 0x0000},
    {REG_MOD1_A0, 0x0000},
    {REG_MOD1_A2, 0x0000},
    {REG_MOD1_76, 0xFFFF},
    {REG_MOD1_C0, 0xD000},
    {REG_MOD1_C0, 0xD330},
    {REG_MOD1_C0, 0xD320},
    {REG_MOD1_C2, 0x8F3F},
    {REG_MOD1_D0, 0x2000},
    {REG_MOD1_C4, 0xB020},
    {REG_MOD1_C0, 0x0F56},
    {REG_MOD1_C0, 0x0FD6},
    {REG_MOD1_C0, 0x0F96},
    {REG_MOD1_C0, 0x0FB6},
    {REG_MOD1_C0, 0x0FBE},
    {REG_MOD1_C0, 0x0FAE},
    {REG_MOD2_80, 0x00FF},
#elif PANEL_VBY1_1CH_10Bit
    {REG_120F0E, 0x0400},
    {REG_MFT_02, 0x0011},
    {REG_MOD1_4E, 0x00F8},
    {REG_MOD1_24, 0x0000},
    {REG_MOD1_00, 0x0001},
    {REG_MOD1_0A, 0x2000},
    {REG_MOD1_50, 0x8001},
    {REG_MOD1_1E, 0xD800},
    {REG_MOD1_60, 0xD000},
    {REG_MOD1_78, 0x0000},
    {REG_MOD1_A0, 0x0000},
    {REG_MOD1_A2, 0x0000},
    {REG_MOD1_76, 0xFFFF},
    {REG_MOD1_C0, 0xD000},
    {REG_MOD1_C0, 0xD330},
    {REG_MOD1_C0, 0xD320},
    {REG_MOD1_C2, 0x8F3F},
    {REG_MOD1_D0, 0x2000},
    {REG_MOD1_C4, 0xA020},
    {REG_MOD1_C0, 0x0F56},
    {REG_MOD1_C0, 0x0FD6},
    {REG_MOD1_C0, 0x0F96},
    {REG_MOD1_C0, 0x0FB6},
    {REG_MOD1_C0, 0x0FBE},
    {REG_MOD1_C0, 0x0FAE},
    {REG_MOD2_80, 0x00FF},
#elif PANEL_VBY1_2CH_8Bit
    {REG_120F0E, 0x0400},
    {REG_MFT_02, 0x0011},
    {REG_MOD1_4E, 0x00F8},
    {REG_MOD1_24, 0x0000},
    {REG_MOD1_00, 0x0201},
    {REG_MOD1_0A, 0x2000},
    {REG_MOD1_50, 0x8001},
    {REG_MOD1_1E, 0xE400},
    {REG_MOD1_60, 0xd000},
    {REG_MOD1_78, 0x0000},
    {REG_MOD1_A0, 0x0000},
    {REG_MOD1_A2, 0x0000},
    {REG_MOD1_76, 0xFFFF},
    {REG_MOD1_C0, 0xD000},
    {REG_MOD1_C0, 0xD330},
    {REG_MOD1_C0, 0xD320},
    {REG_MOD1_C2, 0x8F3F},
    {REG_MOD1_D0, 0x2000},
    {REG_MOD1_C4, 0xB020},
    {REG_MOD1_C0, 0x0F56},
    {REG_MOD1_C0, 0x0FD6},
    {REG_MOD1_C0, 0x0F96},
    {REG_MOD1_C0, 0x0FB6},
    {REG_MOD1_C0, 0x0FBE},
    {REG_MOD1_C0, 0x0FAE},
    {REG_MOD2_80, 0x00FF},
#elif PANEL_VBY1_2CH_10Bit
    {REG_120F0E, 0x0400},
    {REG_MFT_02, 0x0011},
    {REG_MOD1_4E, 0x00f8},
    {REG_MOD1_24, 0x0000},
    {REG_MOD1_00, 0x0201},
    {REG_MOD1_0A, 0x2000},
    {REG_MOD1_50, 0x8001},
    {REG_MOD1_1E, 0xE400},
    {REG_MOD1_60, 0xd000},
    {REG_MOD1_78, 0x0000},
    {REG_MOD1_A0, 0x0000},
    {REG_MOD1_A2, 0x0000},
    {REG_MOD1_76, 0xFFFF},
    {REG_MOD1_C0, 0xD000},
    {REG_MOD1_C0, 0xD330},
    {REG_MOD1_C0, 0xD320},
    {REG_MOD1_C2, 0x8F3F},
    {REG_MOD1_D0, 0x2000},
    {REG_MOD1_C4, 0xA020},
    {REG_MOD1_C0, 0x0F56},
    {REG_MOD1_C0, 0x0FD6},
    {REG_MOD1_C0, 0x0F96},
    {REG_MOD1_C0, 0x0FB6},
    {REG_MOD1_C0, 0x0FBE},
    {REG_MOD1_C0, 0x0FAE},
    {REG_MOD2_80, 0x00FF},
#elif PANEL_VBY1_4CH_8Bit
    {REG_120F0E, 0x0400},
    {REG_MFT_02, 0x0011},
    {REG_MOD1_4E, 0x00f8},
    {REG_MOD1_24, 0x0000},
    {REG_MOD1_00, 0x0601},
    {REG_MOD1_0A, 0x2000},
    {REG_MOD1_50, 0x8001},
    {REG_MOD1_1E, 0xe400},
    {REG_MOD1_60, 0xd000},
    {REG_MOD1_78, 0x0400},
    {REG_MOD1_A0, 0x0000},
    {REG_MOD1_A2, 0x0000},
    {REG_MOD1_76, 0xffff},
    {REG_MOD1_C0, 0xd000},
    {REG_MOD1_C0, 0xd330},
    {REG_MOD1_C0, 0xd320},
    {REG_MOD1_C2, 0x8f3f},
    {REG_MOD1_D0, 0x2000},
    {REG_MOD1_C4, 0xf020},
    {REG_MOD1_C0, 0x0f56},
    {REG_MOD1_C0, 0x0fd6},
    {REG_MOD1_C0, 0x0f96},
    {REG_MOD1_C0, 0x0fb6},
    {REG_MOD1_C0, 0x0fbe},
    {REG_MOD1_C0, 0x0fae},
    {REG_MOD2_80, 0x01ff},
#if (CHIP_ID == MST9U3)
    {REG_MOD1_52, 0x0F00},
#else
    {REG_MOD1_54, 0x00f0},
#endif
#elif PANEL_VBY1_4CH_10Bit
    {REG_120F0E, 0x0400},
    {REG_MFT_02, 0x0011},
    {REG_MOD1_4E, 0x00F8},
    {REG_MOD1_24, 0x0000},
    {REG_MOD1_00, 0x0601},
    {REG_MOD1_0A, 0x2000},
    {REG_MOD1_50, 0x8001},
    {REG_MOD1_1E, 0xE400},
    {REG_MOD1_60, 0xD000},
    {REG_MOD1_78, 0x0400},
    {REG_MOD1_A0, 0x0000},
    {REG_MOD1_A2, 0x0000},
    {REG_MOD1_76, 0xFFFF},
    {REG_MOD1_C0, 0xD000},
    {REG_MOD1_C0, 0xD330},
    {REG_MOD1_C0, 0xD320},
    {REG_MOD1_C2, 0x8F3F},
    {REG_MOD1_D0, 0x2000},
    {REG_MOD1_C4, 0xe020},
    {REG_MOD1_C0, 0x0F56},
    {REG_MOD1_C0, 0x0FD6},
    {REG_MOD1_C0, 0x0F96},
    {REG_MOD1_C0, 0x0FB6},
    {REG_MOD1_C0, 0x0FBE},
    {REG_MOD1_C0, 0x0FAE},
    {REG_MOD2_80, 0x01FF},
#if (CHIP_ID == MST9U3)
    {REG_MOD1_52, 0x0F00},
#else
    {REG_MOD1_54, 0x00F0},
#endif
#elif PANEL_VBY1_8CH_8Bit
    {REG_120F0E, 0x0400},
    {REG_MFT_02, 0x0001},
    {REG_MOD1_4E, 0x00F8},
    {REG_MOD1_20, 0x0000},
#if (CHIP_ID == MST9U3)
    {REG_MOD1_22, 0x5555},
    {REG_MOD1_24, 0x0000},
    {REG_MOD1_26, 0x0000},
#else
    {REG_MOD1_22, 0x0000},
    {REG_MOD1_24, 0x0000},
    {REG_MOD1_26, 0x5555},
#endif
    {REG_MOD1_00, 0x0601},
    {REG_MOD1_0A, 0x2000},
    {REG_MOD1_50, 0x8001},
    {REG_MOD1_1E, 0xE400},
    {REG_MOD1_60, 0xD000},
    {REG_MOD1_78, 0x0400},
    {REG_MOD1_A0, 0x0000},
    {REG_MOD1_A2, 0x0000},
    {REG_MOD1_76, 0xFFFF},
    {REG_MOD1_C0, 0xD000},
    {REG_MOD1_C0, 0xD330},
    {REG_MOD1_C0, 0xD320},
    {REG_MOD1_C2, 0x8F3F},
    {REG_MOD1_D0, 0x2000},
    {REG_MOD1_C4, 0xF020},
    {REG_MOD1_C0, 0x0F56},
    {REG_MOD1_C0, 0x0FD6},
    {REG_MOD1_C0, 0x0F96},
    {REG_MOD1_C0, 0x0FB6},
    {REG_MOD1_C0, 0x0FBE},
    {REG_MOD1_C0, 0x0FAE},
#if (CHIP_ID == MST9U3)
    {REG_MOD2_80, 0x0116},
#else
    {REG_MOD2_80, 0x01FF},
#endif
    {REG_MOD2_82, 0x0000},
    {REG_MOD2_84, 0x0000},
    {REG_MOD2_86, 0x0000},
#if (CHIP_ID == MST9U3)
    {REG_MOD1_52, 0xFF00},
#else
    {REG_MOD1_54, 0x00F0},
    {REG_MOD2_B2, 0x000F},
#endif
#elif PANEL_VBY1_8CH_10Bit
    {REG_120F0E, 0x0400},
    {REG_MFT_02, 0x0001},
    {REG_MOD1_4E, 0x00F8},
    {REG_MOD1_20, 0x0000},
    {REG_MOD1_22, 0x0000},
    {REG_MOD1_24, 0x0000},
    {REG_MOD1_26, 0x0000},
    {REG_MOD1_00, 0x0601},
    {REG_MOD1_0A, 0x6000},
    {REG_MOD1_1E, 0xE400},
    {REG_MOD1_60, 0xD000},
    {REG_MOD1_78, 0x0400},
    {REG_MOD1_A0, 0x0000},
    {REG_MOD1_A2, 0x0000},
    {REG_MOD1_76, 0xFFFF},
    {REG_MOD1_C0, 0xD000},
    {REG_MOD1_C0, 0xD330},
    {REG_MOD1_C0, 0xD320},
    {REG_MOD1_C2, 0x8F3F},
    {REG_MOD1_D0, 0x2000},
    {REG_MOD1_C4, 0xE020},
    {REG_MOD1_C0, 0x0F56},
    {REG_MOD1_C0, 0x0FD6},
    {REG_MOD1_C0, 0x0F96},
    {REG_MOD1_C0, 0x0FB6},
    {REG_MOD1_C0, 0x0FBE},
#if PANEL_VBY1_HW_MODE
    {REG_MOD1_C0, 0x0AAE},
#else
    {REG_MOD1_C0, 0x0FAE},
#endif
#if (CHIP_ID == MST9U3)
    {REG_MOD2_80, 0x0116},
#else
    {REG_MOD2_80, 0x0118},
#endif
    {REG_MOD2_82, 0x0000},
    {REG_MOD2_84, 0x0000},
    {REG_MOD2_86, 0x0000},

#if (CHIP_ID == MST9U3)
    {REG_MOD1_52, 0xFF00},
#else
    {REG_MOD1_54, 0x00F0},
    {REG_MOD2_B2, 0x000F},
#endif
#if (CHIP_ID==MST9U3)
    {REG_MOD1_96, 0xFF00},
    {REG_MOD1_98, 0xFFFF},
    {REG_MOD1_42, 0xFF00},
#elif (CHIP_ID==MST9U2) // removed pre-emphasis by kennyg, 20131119
    {REG_MOD1_9A, 0x0000},
    {REG_MOD1_9C, 0x0000},
    {REG_MOD2_A0, 0x0000},
    {REG_MOD1_44, 0x00F0},
    {REG_MOD2_AA, 0x000F},
#else
    {REG_MOD1_9A, 0xF000},
    {REG_MOD1_9C, 0x00FF},
    {REG_MOD2_A0, 0x0FFF},
    {REG_MOD1_44, 0x00F0},
    {REG_MOD2_AA, 0x000F},
#endif
    {REG_MOD2_AA, 0x00FF},
    {REG_MOD2_A2, 0xFFFF},
    {REG_MOD2_A4, 0xFFFF},
    {REG_MOD2_A6, 0xFFFF},
    {REG_MOD2_A8, 0xFFFF},
    {REG_MOD2_CA, 0xC000},
    {REG_MOD2_D4, 0x0000},
//Analog Settings

    {REG_MOD2_B4, 0x0000},
    {REG_MOD2_A2, 0x0000},
    {REG_MOD2_A4, 0x0000},
    {REG_MOD2_A6, 0x0000},
    {REG_MOD2_A8, 0x0000},
#ifdef VBY1_SWPATH
    {REG_MOD1_50, 0x8101},
#endif
#elif PANEL_EDP
    {REG_120F0E, 0x0800},
    {REG_MOD1_20, 0x0000},
    {REG_MOD1_22, 0x0000},
    {REG_MOD1_24, 0x0000},
    {REG_MOD1_26, 0x0000},
    {REG_MOD1_4E, 0x00F8},
    {REG_MOD1_50, 0x8001},
#if (CHIP_ID == MST9U3)
    {REG_MOD1_52, 0xFF00},
    {REG_MOD1_F2, 0x8142},
#else
    {REG_MOD1_54, 0x00F0},
    {REG_MOD2_B2, 0x000F},
    {REG_MOD1_62, 0x1021},
#endif
    {REG_MOD1_B2, 0x000F},
    {REG_MOD1_78, 0x0000},
    {REG_MOD1_60, 0xD000},
    {REG_MOD2_14, 0x0003},
    {REG_MOD2_80, 0x01FF},
    {REG_MOD2_82, 0x0000},
    {REG_MOD2_84, 0x0000},
    {REG_MOD2_86, 0x0000},
    {REG_MOD1_A0, 0x0000},
    {REG_MOD1_A2, 0x0000},
#endif
    {REG_MOD2_02, (LVDS_CH_D_SWAP<<12)|(LVDS_CH_C_SWAP<<8)|(LVDS_CH_B_SWAP<<4)|(LVDS_CH_A_SWAP)},
    {REG_MOD2_04, (LVDS_CH_H_SWAP<<12)|(LVDS_CH_G_SWAP<<8)|(LVDS_CH_F_SWAP<<4)|(LVDS_CH_E_SWAP)},
    {REG_MOD1_0B, BIT5|(PanelSwapMLSB&BIT2)|(BIT3&PANEL_SWAP_PN)|(BIT6&LVDS_TIMode)},
    {REG_MOD1_03, (PanelSwapOddRB&BIT2)|(PanelSwapEvenRB&BIT5)|(PanelSwapOddML&BIT2)|(PanelSwapEvenML&BIT3)},
#if PANEL_VBY1()
    {REG_MOD1_46, (PANEL_SWAP_PN?0xFFFF:0x0000)},//PN swap for VBY1
#if (CHIP_ID == MST9U3)
    {REG_MOD1_48, (PANEL_SWAP_PN?(PANEL_SWAP_PN_LOCKN?0x00FF:0x003F):(PANEL_SWAP_PN_LOCKN?0x00C0:0x0000))},//PN swap for VBY1
#else
    {REG_MOD1_48, (PANEL_SWAP_PN?0x00FF:0x0000)},//PN swap for VBY1
#endif
    {REG_MOD2_AE, (PANEL_SWAP_PN?0xFFFF:0x0000)},//PN swap for VBY1
    {REG_MOD2_B0, (PANEL_SWAP_PN?(PANEL_SWAP_PN_LOCKN?0x00FF:0x003F):(PANEL_SWAP_PN_LOCKN?0x00C0:0x0000))},//PN swap for VBY1
    {REG_MOD1_62, 0x1021},
#if (CHIP_ID!=MST9U)
    {REG_MOD1_C7, 0x0020},
#endif
#endif
#if PANEL_OUTPUT_FMT_4B
    {REG_MFT_02, 0x8001},
    {REG_MFT_06, 0x4000},
    {REG_MOD2_02, 0x6240},
    {REG_MOD2_04, 0x7351},
#endif

};
#else //MST9U4
code RegUnitType2 tblInitMOD[]=
{
///////////MOD Initial Settings///////////
#if PANEL_TTL
    {REG_MOD1_0C ,0x0000},
    {REG_MOD2_02 ,0x0003},
    {REG_MOD2_08 ,0x0000},
    {REG_MOD2_0A ,0x0000},
    {REG_MOD2_0C ,0x0000},
    {REG_MOD2_0E ,0x0000},
    {REG_MOD2_10 ,0x0000},
    {REG_MOD2_12 ,0x0000},
    {REG_MOD2_40 ,0x0000},
    {REG_MOD2_42 ,0x0000},
    {REG_MOD2_44 ,0x0000},
    {REG_MOD2_46 ,0x0000},
    {REG_MOD2_48 ,0x0000},
    {REG_MOD2_4A ,0x0000},
    {REG_MOD2_4C ,0x0000},
    {REG_MOD2_4E ,0x0000},
    {REG_MOD2_50 ,0x0000},
    {REG_MOD2_52 ,0x0000},
    {REG_MOD2_54 ,0x0000},
    {REG_MOD2_56 ,0x0000},
    {REG_MOD2_58 ,0x0000},
    {REG_MOD2_5A ,0x0000},
    {REG_MOD2_5C ,0x0000},
    {REG_MOD2_5E ,0x0000},
    {REG_MOD2_80 ,0x1111},
    {REG_MOD2_82 ,0x1111},
    {REG_MOD2_84 ,0x1111},
    {REG_MOD2_86 ,0x1111},
    {REG_MOD2_88 ,0x1111},
    {REG_MOD2_8A ,0x1111},
    {REG_MOD2_8C ,0x1111},
    {REG_MOD2_8E ,0x1111},
    {REG_MOD2_90 ,0x1111},
    {REG_MOD2_92 ,0x1111},
    {REG_MOD2_94 ,0x1111},
    {REG_MOD2_96 ,0x1111},
    {REG_MOD2_98 ,0x1111},
    {REG_MOD2_9A ,0x1111},
    {REG_MOD2_9C ,0x1111},
    {REG_MOD2_9E ,0x1111},
    {REG_MOD2_A0 ,0x4444},
    {REG_MOD2_A2 ,0x4444},
    {REG_MOD2_A4 ,0x4444},
    {REG_MOD2_A6 ,0x4444},
    {REG_MOD2_A8 ,0x4444},
    {REG_MOD2_AA ,0x4444},
    {REG_MOD2_AC ,0x4444},
    {REG_MOD2_AE ,0x4444},
    {REG_MOD2_38 ,0x0000},
    {REG_MOD2_3A ,0x0000},
    {REG_MOD2_3C ,0x0000},
    {REG_MOD2_3E ,0x0000},
    {REG_MOD2_60 ,0x1919},
    {REG_MOD2_62 ,0x1919},
    {REG_MOD2_64 ,0x1919},
    {REG_MOD2_66 ,0x1919},
    {REG_MOD2_68 ,0x1919},
    {REG_MOD2_6A ,0x1919},
    {REG_MOD2_6C ,0x1919},
    {REG_MOD2_6E ,0x1919},
    {REG_MOD2_70 ,0x1919},
    {REG_MOD2_72 ,0x1919},
    {REG_MOD2_74 ,0x1919},
    {REG_MOD2_76 ,0x1919},
    {REG_MOD2_78 ,0x1919},
    {REG_MOD2_7A ,0x1919},
    {REG_MOD2_7C ,0x1919},
    {REG_MOD2_7E ,0x1919},

    {REG_DISPMISC_20 ,0x0020},
    {REG_DISPMISC_22 ,0x0048},
    {REG_DISPMISC_24 ,0x0000},
    {REG_DISPMISC_26 ,0x0002},
    {REG_MOD2_EC ,0x0000},

    {REG_MOD1_00 ,0x0001|},
    {REG_MOD1_0A ,0x2000},
    {REG_MOD1_60 ,0x0000},
    {REG_MOD1_78 ,0x2000},

    {REG_MOD2_00 ,0x0000},
    {REG_MOD2_30 ,0x0000},
    {REG_MOD2_32 ,0x0000},
    {REG_MOD2_34 ,0x0000},
    {REG_MOD2_36 ,0x0000},

#elif PANEL_LVDS_2CH
    {REG_MOD1_0C ,0x0001},
    {REG_MOD2_02 ,0x0002},
    {REG_MOD2_08 ,0x0000},
    {REG_MOD2_0A ,0x0000},
    {REG_MOD2_0C ,0x0000},
    {REG_MOD2_0E ,0x0000},
    {REG_MOD2_10 ,0x0000},
    {REG_MOD2_12 ,0x0000},
    {REG_MOD2_40 ,0x0000},
    {REG_MOD2_42 ,0x0000},
    {REG_MOD2_44 ,0x0000},
    {REG_MOD2_46 ,0x0000},
    {REG_MOD2_48 ,0x0000},
    {REG_MOD2_4A ,0x0000},
    {REG_MOD2_4C ,0x0000},
    {REG_MOD2_4E ,0x0000},
    {REG_MOD2_50 ,0x0000},
    {REG_MOD2_52 ,0x0000},
    {REG_MOD2_54 ,0x0000},
    {REG_MOD2_56 ,0x0000},
    {REG_MOD2_58 ,0x0000},
    {REG_MOD2_5A ,0x0000},
    {REG_MOD2_5C ,0x0000},
    {REG_MOD2_5E ,0x0000},
    {REG_MOD2_80 ,0x1111},
    {REG_MOD2_82 ,0x1111},
    {REG_MOD2_84 ,0x1111},
    {REG_MOD2_86 ,0x1111},
    {REG_MOD2_88 ,0x1111},
    {REG_MOD2_8A ,0x1111},
    {REG_MOD2_8C ,0x1111},
    {REG_MOD2_8E ,0x1111},
    {REG_MOD2_90 ,0x1111},
    {REG_MOD2_92 ,0x1111},
    {REG_MOD2_94 ,0x1111},
    {REG_MOD2_96 ,0x1111},
    {REG_MOD2_98 ,0x1111},
    {REG_MOD2_9A ,0x1111},
    {REG_MOD2_9C ,0x1111},
    {REG_MOD2_9E ,0x1111},
    {REG_MOD2_A0 ,0x4444},
    {REG_MOD2_A2 ,0x4444},
    {REG_MOD2_A4 ,0x4444},
    {REG_MOD2_A6 ,0x4444},
    {REG_MOD2_A8 ,0x4444},
    {REG_MOD2_AA ,0x4444},
    {REG_MOD2_AC ,0x4444},
    {REG_MOD2_AE ,0x4444},
    {REG_MOD2_38 ,0x0000},
    {REG_MOD2_3A ,0x0000},
    {REG_MOD2_3C ,0x0000},
    {REG_MOD2_3E ,0x0000},
    {REG_MOD2_60 ,0x1919},
    {REG_MOD2_62 ,0x1919},
    {REG_MOD2_64 ,0x1919},
    {REG_MOD2_66 ,0x1919},
    {REG_MOD2_68 ,0x1919},
    {REG_MOD2_6A ,0x1919},
    {REG_MOD2_6C ,0x1919},
    {REG_MOD2_6E ,0x1919},
    {REG_MOD2_70 ,0x1919},
    {REG_MOD2_72 ,0x1919},
    {REG_MOD2_74 ,0x1919},
    {REG_MOD2_76 ,0x1919},
    {REG_MOD2_78 ,0x1919},
    {REG_MOD2_7A ,0x1919},
    {REG_MOD2_7C ,0x1919},
    {REG_MOD2_7E ,0x1919},

    {REG_DISPMISC_20 ,0x0010},
    {REG_DISPMISC_22 ,0x0004},
    {REG_DISPMISC_24 ,0x0000},
    {REG_DISPMISC_26 ,0x0002},
    {REG_MOD2_EC ,0x0000},

    {REG_MOD1_00 ,0x0001},
    {REG_MOD1_0A ,0x2000},
    {REG_MOD1_60 ,0x0000},
    {REG_MOD1_78 ,0x0000},

    {REG_MOD2_00 ,0x0003},
    {REG_MOD2_30 ,0x0555},
    {REG_MOD2_32 ,0x0555},
    {REG_MOD2_34 ,0x0000},
    {REG_MOD2_36 ,0x0000},

    {REG_MOD1_1E ,0x6420},

#elif PANEL_LVDS_4CH
    {REG_MOD1_0C ,0x0001},
    {REG_MOD2_02 ,0x0002},
    {REG_MOD2_08 ,0x0000},
    {REG_MOD2_0A ,0x0000},
    {REG_MOD2_0C ,0x0000},
    {REG_MOD2_0E ,0x0000},
    {REG_MOD2_10 ,0x0000},
    {REG_MOD2_12 ,0x0000},
    {REG_MOD2_40 ,0x0000},
    {REG_MOD2_42 ,0x0000},
    {REG_MOD2_44 ,0x0000},
    {REG_MOD2_46 ,0x0000},
    {REG_MOD2_48 ,0x0000},
    {REG_MOD2_4A ,0x0000},
    {REG_MOD2_4C ,0x0000},
    {REG_MOD2_4E ,0x0000},
    {REG_MOD2_50 ,0x0000},
    {REG_MOD2_52 ,0x0000},
    {REG_MOD2_54 ,0x0000},
    {REG_MOD2_56 ,0x0000},
    {REG_MOD2_58 ,0x0000},
    {REG_MOD2_5A ,0x0000},
    {REG_MOD2_5C ,0x0000},
    {REG_MOD2_5E ,0x0000},
    {REG_MOD2_80 ,0x1111},
    {REG_MOD2_82 ,0x1111},
    {REG_MOD2_84 ,0x1111},
    {REG_MOD2_86 ,0x1111},
    {REG_MOD2_88 ,0x1111},
    {REG_MOD2_8A ,0x1111},
    {REG_MOD2_8C ,0x1111},
    {REG_MOD2_8E ,0x1111},
    {REG_MOD2_90 ,0x1111},
    {REG_MOD2_92 ,0x1111},
    {REG_MOD2_94 ,0x1111},
    {REG_MOD2_96 ,0x1111},
    {REG_MOD2_98 ,0x1111},
    {REG_MOD2_9A ,0x1111},
    {REG_MOD2_9C ,0x1111},
    {REG_MOD2_9E ,0x1111},
    {REG_MOD2_A0 ,0x4444},
    {REG_MOD2_A2 ,0x4444},
    {REG_MOD2_A4 ,0x4444},
    {REG_MOD2_A6 ,0x4444},
    {REG_MOD2_A8 ,0x4444},
    {REG_MOD2_AA ,0x4444},
    {REG_MOD2_AC ,0x4444},
    {REG_MOD2_AE ,0x4444},
    {REG_MOD2_38 ,0x0000},
    {REG_MOD2_3A ,0x0000},
    {REG_MOD2_3C ,0x0000},
    {REG_MOD2_3E ,0x0000},
    {REG_MOD2_60 ,0x1919},
    {REG_MOD2_62 ,0x1919},
    {REG_MOD2_64 ,0x1919},
    {REG_MOD2_66 ,0x1919},
    {REG_MOD2_68 ,0x1919},
    {REG_MOD2_6A ,0x1919},
    {REG_MOD2_6C ,0x1919},
    {REG_MOD2_6E ,0x1919},
    {REG_MOD2_70 ,0x1919},
    {REG_MOD2_72 ,0x1919},
    {REG_MOD2_74 ,0x1919},
    {REG_MOD2_76 ,0x1919},
    {REG_MOD2_78 ,0x1919},
    {REG_MOD2_7A ,0x1919},
    {REG_MOD2_7C ,0x1919},
    {REG_MOD2_7E ,0x1919},

    {REG_DISPMISC_20 ,0x0000},
    {REG_DISPMISC_22 ,0x0000},
    {REG_DISPMISC_24 ,0x0000},
    {REG_DISPMISC_26 ,0x0001},
    {REG_MOD2_EC ,0x0000},

    {REG_MOD1_00 ,0x0201},
    {REG_MOD1_0A ,0x2000},
    {REG_MOD1_60 ,0x0000},
    {REG_MOD1_78 ,0x0000},

    {REG_MOD2_00 ,0x000F},
    {REG_MOD2_30 ,0x0555},
    {REG_MOD2_32 ,0x0555},
    {REG_MOD2_34 ,0x0555},
    {REG_MOD2_36 ,0x0555},

    {REG_MOD1_1E ,0x6420},

#elif PANEL_LVDS_8CH
#elif PANEL_VBY1_1CH_8Bit
#elif PANEL_VBY1_1CH_10Bit
#elif PANEL_VBY1_2CH_8Bit
    {REG_MOD1_0C ,0x0002},
    {REG_MOD2_02 ,0x0002},
    {REG_MOD2_08 ,0x0000},
    {REG_MOD2_0A ,0x0000},
    {REG_MOD2_0C ,0x0000},
    {REG_MOD2_0E ,0x0000},
    {REG_MOD2_10 ,0x0000},
    {REG_MOD2_12 ,0x0000},
    {REG_MOD2_40 ,0x0202},
    {REG_MOD2_42 ,0x0202},
    {REG_MOD2_44 ,0x0202},
    {REG_MOD2_46 ,0x0202},
    {REG_MOD2_48 ,0x0202},
    {REG_MOD2_4A ,0x0202},
    {REG_MOD2_4C ,0x0202},
    {REG_MOD2_4E ,0x0202},
    {REG_MOD2_50 ,0x0202},
    {REG_MOD2_52 ,0x0202},
    {REG_MOD2_54 ,0x0202},
    {REG_MOD2_56 ,0x0202},
    {REG_MOD2_58 ,0x0202},
    {REG_MOD2_5A ,0x0202},
    {REG_MOD2_5C ,0x0202},
    {REG_MOD2_5E ,0x0202},
    {REG_MOD2_80 ,0x1111},
    {REG_MOD2_82 ,0x1111},
    {REG_MOD2_84 ,0x1111},
    {REG_MOD2_86 ,0x1111},
    {REG_MOD2_88 ,0x1111},
    {REG_MOD2_8A ,0x1111},
    {REG_MOD2_8C ,0x1111},
    {REG_MOD2_8E ,0x1111},
    {REG_MOD2_90 ,0x1111},
    {REG_MOD2_92 ,0x1111},
    {REG_MOD2_94 ,0x1111},
    {REG_MOD2_96 ,0x1111},
    {REG_MOD2_98 ,0x1111},
    {REG_MOD2_9A ,0x1111},
    {REG_MOD2_9C ,0x1111},
    {REG_MOD2_9E ,0x1111},
    {REG_MOD2_A0 ,0x4444},
    {REG_MOD2_A2 ,0x4444},
    {REG_MOD2_A4 ,0x4444},
    {REG_MOD2_A6 ,0x4444},
    {REG_MOD2_A8 ,0x4444},
    {REG_MOD2_AA ,0x4444},
    {REG_MOD2_AC ,0x4444},
    {REG_MOD2_AE ,0x4444},
    {REG_MOD2_38 ,0x0004},
    {REG_MOD2_3A ,0x0000},
    {REG_MOD2_3C ,0x0000},
    {REG_MOD2_3E ,0x0000},
    {REG_MOD2_60 ,0x1919},
    {REG_MOD2_62 ,0x1919},
    {REG_MOD2_64 ,0x1919},
    {REG_MOD2_66 ,0x1919},
    {REG_MOD2_68 ,0x1919},
    {REG_MOD2_6A ,0x1919},
    {REG_MOD2_6C ,0x1919},
    {REG_MOD2_6E ,0x1919},
    {REG_MOD2_70 ,0x1919},
    {REG_MOD2_72 ,0x1919},
    {REG_MOD2_74 ,0x1919},
    {REG_MOD2_76 ,0x1919},
    {REG_MOD2_78 ,0x1919},
    {REG_MOD2_7A ,0x1919},
    {REG_MOD2_7C ,0x1919},
    {REG_MOD2_7E ,0x1919},

    {REG_DISPMISC_20 ,0x001A},
    {REG_DISPMISC_22 ,0x0004},
    {REG_DISPMISC_24 ,0x0400},
    {REG_DISPMISC_26 ,0x0002},
    {REG_MOD2_EC ,0x0000},

    {REG_DISPMISC_00 ,0x0000},
    {REG_MOD2_EE ,0x0003},

    {REG_MOD1_00 ,0x0001},
    {REG_MOD1_0A ,0x2000},
    {REG_MOD1_60 ,0x5000},
    {REG_MOD1_78 ,0x0000},

    {REG_MOD1_C2 ,0x8F3F},
    {REG_MOD1_C4 ,0xB000},
    {REG_MOD1_C6 ,0xB400},
    {REG_MOD1_C0 ,0x0AAE},

    {REG_MOD2_00 ,0x0001},
    {REG_MOD2_30 ,0x0005},
    {REG_MOD2_32 ,0x0000},
    {REG_MOD2_34 ,0x0000},
    {REG_MOD2_36 ,0x0000},

#elif PANEL_VBY1_2CH_10Bit
    {REG_MOD1_0C ,0x0002},
    {REG_MOD2_02 ,0x0002},
    {REG_MOD2_08 ,0x0000},
    {REG_MOD2_0A ,0x0000},
    {REG_MOD2_0C ,0x0000},
    {REG_MOD2_0E ,0x0000},
    {REG_MOD2_10 ,0x0000},
    {REG_MOD2_12 ,0x0000},
    {REG_MOD2_40 ,0x0202},
    {REG_MOD2_42 ,0x0202},
    {REG_MOD2_44 ,0x0202},
    {REG_MOD2_46 ,0x0202},
    {REG_MOD2_48 ,0x0202},
    {REG_MOD2_4A ,0x0202},
    {REG_MOD2_4C ,0x0202},
    {REG_MOD2_4E ,0x0202},
    {REG_MOD2_50 ,0x0202},
    {REG_MOD2_52 ,0x0202},
    {REG_MOD2_54 ,0x0202},
    {REG_MOD2_56 ,0x0202},
    {REG_MOD2_58 ,0x0202},
    {REG_MOD2_5A ,0x0202},
    {REG_MOD2_5C ,0x0202},
    {REG_MOD2_5E ,0x0202},
    {REG_MOD2_80 ,0x1111},
    {REG_MOD2_82 ,0x1111},
    {REG_MOD2_84 ,0x1111},
    {REG_MOD2_86 ,0x1111},
    {REG_MOD2_88 ,0x1111},
    {REG_MOD2_8A ,0x1111},
    {REG_MOD2_8C ,0x1111},
    {REG_MOD2_8E ,0x1111},
    {REG_MOD2_90 ,0x1111},
    {REG_MOD2_92 ,0x1111},
    {REG_MOD2_94 ,0x1111},
    {REG_MOD2_96 ,0x1111},
    {REG_MOD2_98 ,0x1111},
    {REG_MOD2_9A ,0x1111},
    {REG_MOD2_9C ,0x1111},
    {REG_MOD2_9E ,0x1111},
    {REG_MOD2_A0 ,0x4444},
    {REG_MOD2_A2 ,0x4444},
    {REG_MOD2_A4 ,0x4444},
    {REG_MOD2_A6 ,0x4444},
    {REG_MOD2_A8 ,0x4444},
    {REG_MOD2_AA ,0x4444},
    {REG_MOD2_AC ,0x4444},
    {REG_MOD2_AE ,0x4444},
    {REG_MOD2_38 ,0x0004},
    {REG_MOD2_3A ,0x0000},
    {REG_MOD2_3C ,0x0000},
    {REG_MOD2_3E ,0x0000},
    {REG_MOD2_60 ,0x1919},
    {REG_MOD2_62 ,0x1919},
    {REG_MOD2_64 ,0x1919},
    {REG_MOD2_66 ,0x1919},
    {REG_MOD2_68 ,0x1919},
    {REG_MOD2_6A ,0x1919},
    {REG_MOD2_6C ,0x1919},
    {REG_MOD2_6E ,0x1919},
    {REG_MOD2_70 ,0x1919},
    {REG_MOD2_72 ,0x1919},
    {REG_MOD2_74 ,0x1919},
    {REG_MOD2_76 ,0x1919},
    {REG_MOD2_78 ,0x1919},
    {REG_MOD2_7A ,0x1919},
    {REG_MOD2_7C ,0x1919},
    {REG_MOD2_7E ,0x1919},

    {REG_DISPMISC_20 ,0x001A},
    {REG_DISPMISC_22 ,0x0004},
    {REG_DISPMISC_24 ,0x0400},
    {REG_DISPMISC_26 ,0x0002},
    {REG_MOD2_EC ,0x0000},

    {REG_DISPMISC_00 ,0x0000},
    {REG_MOD2_EE ,0x0003},

    {REG_MOD1_00 ,0x0001},
    {REG_MOD1_0A ,0x2000},
    {REG_MOD1_60 ,0x5000},
    {REG_MOD1_78 ,0x0000},

    {REG_MOD1_C2 ,0x8F3F},
    {REG_MOD1_C4 ,0xA000},
    {REG_MOD1_C6 ,0xB600},
    {REG_MOD1_C0 ,0x0AAE},

    {REG_MOD2_00 ,0x0001},
    {REG_MOD2_30 ,0x0005},
    {REG_MOD2_32 ,0x0000},
    {REG_MOD2_34 ,0x0000},
    {REG_MOD2_36 ,0x0000},

#elif PANEL_VBY1_4CH_8Bit
    {REG_MOD1_0C ,0x0002},
    {REG_MOD2_02 ,0x0002},
    {REG_MOD2_08 ,0x0000},
    {REG_MOD2_0A ,0x0000},
    {REG_MOD2_0C ,0x0000},
    {REG_MOD2_0E ,0x0000},
    {REG_MOD2_10 ,0x0000},
    {REG_MOD2_12 ,0x0000},
    {REG_MOD2_40 ,0x0202},
    {REG_MOD2_42 ,0x0202},
    {REG_MOD2_44 ,0x0202},
    {REG_MOD2_46 ,0x0202},
    {REG_MOD2_48 ,0x0202},
    {REG_MOD2_4A ,0x0202},
    {REG_MOD2_4C ,0x0202},
    {REG_MOD2_4E ,0x0202},
    {REG_MOD2_50 ,0x0202},
    {REG_MOD2_52 ,0x0202},
    {REG_MOD2_54 ,0x0202},
    {REG_MOD2_56 ,0x0202},
    {REG_MOD2_58 ,0x0202},
    {REG_MOD2_5A ,0x0202},
    {REG_MOD2_5C ,0x0202},
    {REG_MOD2_5E ,0x0202},
    {REG_MOD2_80 ,0x1111},
    {REG_MOD2_82 ,0x1111},
    {REG_MOD2_84 ,0x1111},
    {REG_MOD2_86 ,0x1111},
    {REG_MOD2_88 ,0x1111},
    {REG_MOD2_8A ,0x1111},
    {REG_MOD2_8C ,0x1111},
    {REG_MOD2_8E ,0x1111},
    {REG_MOD2_90 ,0x1111},
    {REG_MOD2_92 ,0x1111},
    {REG_MOD2_94 ,0x1111},
    {REG_MOD2_96 ,0x1111},
    {REG_MOD2_98 ,0x1111},
    {REG_MOD2_9A ,0x1111},
    {REG_MOD2_9C ,0x1111},
    {REG_MOD2_9E ,0x1111},
    {REG_MOD2_A0 ,0x4444},
    {REG_MOD2_A2 ,0x4444},
    {REG_MOD2_A4 ,0x4444},
    {REG_MOD2_A6 ,0x4444},
    {REG_MOD2_A8 ,0x4444},
    {REG_MOD2_AA ,0x4444},
    {REG_MOD2_AC ,0x4444},
    {REG_MOD2_AE ,0x4444},
    {REG_MOD2_38 ,0x0044},
    {REG_MOD2_3A ,0x0000},
    {REG_MOD2_3C ,0x0000},
    {REG_MOD2_3E ,0x0000},
    {REG_MOD2_60 ,0x1919},
    {REG_MOD2_62 ,0x1919},
    {REG_MOD2_64 ,0x1919},
    {REG_MOD2_66 ,0x1919},
    {REG_MOD2_68 ,0x1919},
    {REG_MOD2_6A ,0x1919},
    {REG_MOD2_6C ,0x1919},
    {REG_MOD2_6E ,0x1919},
    {REG_MOD2_70 ,0x1919},
    {REG_MOD2_72 ,0x1919},
    {REG_MOD2_74 ,0x1919},
    {REG_MOD2_76 ,0x1919},
    {REG_MOD2_78 ,0x1919},
    {REG_MOD2_7A ,0x1919},
    {REG_MOD2_7C ,0x1919},
    {REG_MOD2_7E ,0x1919},

    {REG_DISPMISC_20 ,0x000A},
    {REG_DISPMISC_22 ,0x0000},
    {REG_DISPMISC_24 ,0x0400},
    {REG_DISPMISC_26 ,0x0002},
    {REG_MOD2_EC ,0x0000},

    {REG_DISPMISC_00 ,0x0000},
    {REG_MOD2_EE ,0x0003},

    {REG_MOD1_00 ,0x0001},
    {REG_MOD1_0A ,0x2000},
    {REG_MOD1_60 ,0x5000},
    {REG_MOD1_78 ,0x0000},

    {REG_MOD1_C2 ,0x8F3F},
    {REG_MOD1_C4 ,0xB000},
    {REG_MOD1_C6 ,0xB400},
    {REG_MOD1_C0 ,0x0AAE},

    {REG_MOD2_00 ,0x0001},
    {REG_MOD2_30 ,0x0055},
    {REG_MOD2_32 ,0x0000},
    {REG_MOD2_34 ,0x0000},
    {REG_MOD2_36 ,0x0000},

#elif PANEL_VBY1_4CH_10Bit
    {REG_MOD1_0C ,0x0002},
    {REG_MOD2_02 ,0x0002},
    {REG_MOD2_08 ,0x0000},
    {REG_MOD2_0A ,0x0000},
    {REG_MOD2_0C ,0x0000},
    {REG_MOD2_0E ,0x0000},
    {REG_MOD2_10 ,0x0000},
    {REG_MOD2_12 ,0x0000},
    {REG_MOD2_40 ,0x0202},
    {REG_MOD2_42 ,0x0202},
    {REG_MOD2_44 ,0x0202},
    {REG_MOD2_46 ,0x0202},
    {REG_MOD2_48 ,0x0202},
    {REG_MOD2_4A ,0x0202},
    {REG_MOD2_4C ,0x0202},
    {REG_MOD2_4E ,0x0202},
    {REG_MOD2_50 ,0x0202},
    {REG_MOD2_52 ,0x0202},
    {REG_MOD2_54 ,0x0202},
    {REG_MOD2_56 ,0x0202},
    {REG_MOD2_58 ,0x0202},
    {REG_MOD2_5A ,0x0202},
    {REG_MOD2_5C ,0x0202},
    {REG_MOD2_5E ,0x0202},
    {REG_MOD2_80 ,0x1111},
    {REG_MOD2_82 ,0x1111},
    {REG_MOD2_84 ,0x1111},
    {REG_MOD2_86 ,0x1111},
    {REG_MOD2_88 ,0x1111},
    {REG_MOD2_8A ,0x1111},
    {REG_MOD2_8C ,0x1111},
    {REG_MOD2_8E ,0x1111},
    {REG_MOD2_90 ,0x1111},
    {REG_MOD2_92 ,0x1111},
    {REG_MOD2_94 ,0x1111},
    {REG_MOD2_96 ,0x1111},
    {REG_MOD2_98 ,0x1111},
    {REG_MOD2_9A ,0x1111},
    {REG_MOD2_9C ,0x1111},
    {REG_MOD2_9E ,0x1111},
    {REG_MOD2_A0 ,0x4444},
    {REG_MOD2_A2 ,0x4444},
    {REG_MOD2_A4 ,0x4444},
    {REG_MOD2_A6 ,0x4444},
    {REG_MOD2_A8 ,0x4444},
    {REG_MOD2_AA ,0x4444},
    {REG_MOD2_AC ,0x4444},
    {REG_MOD2_AE ,0x4444},
    {REG_MOD2_38 ,0x0044},
    {REG_MOD2_3A ,0x0000},
    {REG_MOD2_3C ,0x0000},
    {REG_MOD2_3E ,0x0000},
    {REG_MOD2_60 ,0x1919},
    {REG_MOD2_62 ,0x1919},
    {REG_MOD2_64 ,0x1919},
    {REG_MOD2_66 ,0x1919},
    {REG_MOD2_68 ,0x1919},
    {REG_MOD2_6A ,0x1919},
    {REG_MOD2_6C ,0x1919},
    {REG_MOD2_6E ,0x1919},
    {REG_MOD2_70 ,0x1919},
    {REG_MOD2_72 ,0x1919},
    {REG_MOD2_74 ,0x1919},
    {REG_MOD2_76 ,0x1919},
    {REG_MOD2_78 ,0x1919},
    {REG_MOD2_7A ,0x1919},
    {REG_MOD2_7C ,0x1919},
    {REG_MOD2_7E ,0x1919},

    {REG_DISPMISC_20 ,0x000A},
    {REG_DISPMISC_22 ,0x0000},
    {REG_DISPMISC_24 ,0x0400},
    {REG_DISPMISC_26 ,0x0002},
    {REG_MOD2_EC ,0x0000},

    {REG_DISPMISC_00 ,0x0000},
    {REG_MOD2_EE ,0x0003},

    {REG_MOD1_00 ,0x0001},
    {REG_MOD1_0A ,0x2000},
    {REG_MOD1_60 ,0x5000},
    {REG_MOD1_78 ,0x0000},

    {REG_MOD1_C2 ,0x8F3F},
    {REG_MOD1_C4 ,0xB000},
    {REG_MOD1_C6 ,0xB400},
    {REG_MOD1_C0 ,0x0AAE},

    {REG_MOD2_00 ,0x0001},
    {REG_MOD2_30 ,0x0055},
    {REG_MOD2_32 ,0x0000},
    {REG_MOD2_34 ,0x0000},
    {REG_MOD2_36 ,0x0000},

#elif PANEL_VBY1_8CH_8Bit
    {REG_MOD1_0C ,0x0002},
    {REG_MOD2_02 ,0x0002},
    {REG_MOD2_08 ,0xFFFF},
    {REG_MOD2_0A ,0xFFFF},
    {REG_MOD2_0C ,0x0000},
    {REG_MOD2_0E ,0x0000},
    {REG_MOD2_10 ,0xFFFF},
    {REG_MOD2_12 ,0xFFFF},
    {REG_MOD2_40 ,0x0202},
    {REG_MOD2_42 ,0x0202},
    {REG_MOD2_44 ,0x0202},
    {REG_MOD2_46 ,0x0202},
    {REG_MOD2_48 ,0x0202},
    {REG_MOD2_4A ,0x0202},
    {REG_MOD2_4C ,0x0202},
    {REG_MOD2_4E ,0x0202},
    {REG_MOD2_50 ,0x0202},
    {REG_MOD2_52 ,0x0202},
    {REG_MOD2_54 ,0x0202},
    {REG_MOD2_56 ,0x0202},
    {REG_MOD2_58 ,0x0202},
    {REG_MOD2_5A ,0x0202},
    {REG_MOD2_5C ,0x0202},
    {REG_MOD2_5E ,0x0202},
    {REG_MOD2_80 ,0x1111},
    {REG_MOD2_82 ,0x1111},
    {REG_MOD2_84 ,0x1111},
    {REG_MOD2_86 ,0x1111},
    {REG_MOD2_88 ,0x1111},
    {REG_MOD2_8A ,0x1111},
    {REG_MOD2_8C ,0x1111},
    {REG_MOD2_8E ,0x1111},
    {REG_MOD2_90 ,0x1111},
    {REG_MOD2_92 ,0x1111},
    {REG_MOD2_94 ,0x1111},
    {REG_MOD2_96 ,0x1111},
    {REG_MOD2_98 ,0x1111},
    {REG_MOD2_9A ,0x1111},
    {REG_MOD2_9C ,0x1111},
    {REG_MOD2_9E ,0x1111},
    {REG_MOD2_A0 ,0x4444},
    {REG_MOD2_A2 ,0x4444},
    {REG_MOD2_A4 ,0x4444},
    {REG_MOD2_A6 ,0x4444},
    {REG_MOD2_A8 ,0x4444},
    {REG_MOD2_AA ,0x4444},
    {REG_MOD2_AC ,0x4444},
    {REG_MOD2_AE ,0x4444},
    {REG_MOD2_38 ,0x4444},
    {REG_MOD2_3A ,0x0000},
    {REG_MOD2_3C ,0x0000},
    {REG_MOD2_3E ,0x0000},
    {REG_MOD2_60 ,0x1919},
    {REG_MOD2_62 ,0x1919},
    {REG_MOD2_64 ,0x1919},
    {REG_MOD2_66 ,0x1919},
    {REG_MOD2_68 ,0x1919},
    {REG_MOD2_6A ,0x1919},
    {REG_MOD2_6C ,0x1919},
    {REG_MOD2_6E ,0x1919},
    {REG_MOD2_70 ,0x1919},
    {REG_MOD2_72 ,0x1919},
    {REG_MOD2_74 ,0x1919},
    {REG_MOD2_76 ,0x1919},
    {REG_MOD2_78 ,0x1919},
    {REG_MOD2_7A ,0x1919},
    {REG_MOD2_7C ,0x1919},
    {REG_MOD2_7E ,0x1919},

    {REG_DISPMISC_20 ,0x0009},
    {REG_DISPMISC_22 ,0x0000},
    {REG_DISPMISC_24 ,0x0400},
    {REG_DISPMISC_26 ,0x0002},
    {REG_MOD2_EC ,0x0000},

    {REG_DISPMISC_00 ,0x0000},
    {REG_MOD2_EE ,0x0003},

    {REG_MOD1_00 ,0x0201},
    {REG_MOD1_0A ,0x2000},
    {REG_MOD1_60 ,0x5000},
    {REG_MOD1_78 ,0x0000},

    {REG_MOD1_C2 ,0x8F3F},
    {REG_MOD1_C4 ,0xB000},
    {REG_MOD1_C6 ,0xB400},
    {REG_MOD1_C0 ,0x0AAE},

    {REG_MOD2_00 ,0x0003},
    {REG_MOD2_30 ,0x5555},
    {REG_MOD2_32 ,0x0000},
    {REG_MOD2_34 ,0x0000},
    {REG_MOD2_36 ,0x0000},

#elif PANEL_VBY1_8CH_10Bit
    {REG_MOD1_0C ,0x0002},
    {REG_MOD2_02 ,0x0002},
    {REG_MOD2_08 ,0xFFFF},
    {REG_MOD2_0A ,0xFFFF},
    {REG_MOD2_0C ,0x0000},
    {REG_MOD2_0E ,0x0000},
    {REG_MOD2_10 ,0xFFFF},
    {REG_MOD2_12 ,0xFFFF},
    {REG_MOD2_40 ,0x0202},
    {REG_MOD2_42 ,0x0202},
    {REG_MOD2_44 ,0x0202},
    {REG_MOD2_46 ,0x0202},
    {REG_MOD2_48 ,0x0202},
    {REG_MOD2_4A ,0x0202},
    {REG_MOD2_4C ,0x0202},
    {REG_MOD2_4E ,0x0202},
    {REG_MOD2_50 ,0x0202},
    {REG_MOD2_52 ,0x0202},
    {REG_MOD2_54 ,0x0202},
    {REG_MOD2_56 ,0x0202},
    {REG_MOD2_58 ,0x0202},
    {REG_MOD2_5A ,0x0202},
    {REG_MOD2_5C ,0x0202},
    {REG_MOD2_5E ,0x0202},
    {REG_MOD2_80 ,0x1111},
    {REG_MOD2_82 ,0x1111},
    {REG_MOD2_84 ,0x1111},
    {REG_MOD2_86 ,0x1111},
    {REG_MOD2_88 ,0x1111},
    {REG_MOD2_8A ,0x1111},
    {REG_MOD2_8C ,0x1111},
    {REG_MOD2_8E ,0x1111},
    {REG_MOD2_90 ,0x1111},
    {REG_MOD2_92 ,0x1111},
    {REG_MOD2_94 ,0x1111},
    {REG_MOD2_96 ,0x1111},
    {REG_MOD2_98 ,0x1111},
    {REG_MOD2_9A ,0x1111},
    {REG_MOD2_9C ,0x1111},
    {REG_MOD2_9E ,0x1111},
    {REG_MOD2_A0 ,0x4444},
    {REG_MOD2_A2 ,0x4444},
    {REG_MOD2_A4 ,0x4444},
    {REG_MOD2_A6 ,0x4444},
    {REG_MOD2_A8 ,0x4444},
    {REG_MOD2_AA ,0x4444},
    {REG_MOD2_AC ,0x4444},
    {REG_MOD2_AE ,0x4444},
    {REG_MOD2_38 ,0x4444},
    {REG_MOD2_3A ,0x0000},
    {REG_MOD2_3C ,0x0000},
    {REG_MOD2_3E ,0x0000},
    {REG_MOD2_60 ,0x1919},
    {REG_MOD2_62 ,0x1919},
    {REG_MOD2_64 ,0x1919},
    {REG_MOD2_66 ,0x1919},
    {REG_MOD2_68 ,0x1919},
    {REG_MOD2_6A ,0x1919},
    {REG_MOD2_6C ,0x1919},
    {REG_MOD2_6E ,0x1919},
    {REG_MOD2_70 ,0x1919},
    {REG_MOD2_72 ,0x1919},
    {REG_MOD2_74 ,0x1919},
    {REG_MOD2_76 ,0x1919},
    {REG_MOD2_78 ,0x1919},
    {REG_MOD2_7A ,0x1919},
    {REG_MOD2_7C ,0x1919},
    {REG_MOD2_7E ,0x1919},

    {REG_DISPMISC_20 ,0x0009},
    {REG_DISPMISC_22 ,0x0000},
    {REG_DISPMISC_24 ,0x0400},
    {REG_DISPMISC_26 ,0x0002},
    {REG_MOD2_EC ,0x0000},

    {REG_DISPMISC_00 ,0x0000},
    {REG_MOD2_EE ,0x0003},

    {REG_MOD1_00 ,0x0201},
    {REG_MOD1_0A ,0x2000},
    {REG_MOD1_60 ,0x5000},
    {REG_MOD1_78 ,0x0000},

    {REG_MOD1_C2 ,0x8F3F},
    {REG_MOD1_C4 ,0xA000},
    {REG_MOD1_C6 ,0xB600},
    {REG_MOD1_C0 ,0x0AAE},

    {REG_MOD2_00 ,0x0003},
    {REG_MOD2_30 ,0x5555},
    {REG_MOD2_32 ,0x0000},
    {REG_MOD2_34 ,0x0000},
    {REG_MOD2_36 ,0x0000},

#elif PANEL_VBY1_16CH_8Bit
    #if ((PanelWidth==5120)&&(PanelHeight==2160)&&(PanelVsyncFreq==120)) //8P
        {REG_MOD1_0C ,0x0002},
        {REG_MOD2_02 ,0x0002},
        {REG_MOD2_08 ,0xFFFF},
        {REG_MOD2_0A ,0xFFFF},
        {REG_MOD2_0C ,0x0000},
        {REG_MOD2_0E ,0x0000},
        {REG_MOD2_10 ,0xFFFF},
        {REG_MOD2_12 ,0xFFFF},
        {REG_MOD2_40 ,0x0202},
        {REG_MOD2_42 ,0x0202},
        {REG_MOD2_44 ,0x0202},
        {REG_MOD2_46 ,0x0202},
        {REG_MOD2_48 ,0x0202},
        {REG_MOD2_4A ,0x0202},
        {REG_MOD2_4C ,0x0202},
        {REG_MOD2_4E ,0x0202},
        {REG_MOD2_50 ,0x0202},
        {REG_MOD2_52 ,0x0202},
        {REG_MOD2_54 ,0x0202},
        {REG_MOD2_56 ,0x0202},
        {REG_MOD2_58 ,0x0202},
        {REG_MOD2_5A ,0x0202},
        {REG_MOD2_5C ,0x0202},
        {REG_MOD2_5E ,0x0202},
        {REG_MOD2_80 ,0x1111},
        {REG_MOD2_82 ,0x1111},
        {REG_MOD2_84 ,0x1111},
        {REG_MOD2_86 ,0x1111},
        {REG_MOD2_88 ,0x1111},
        {REG_MOD2_8A ,0x1111},
        {REG_MOD2_8C ,0x1111},
        {REG_MOD2_8E ,0x1111},
        {REG_MOD2_90 ,0x1111},
        {REG_MOD2_92 ,0x1111},
        {REG_MOD2_94 ,0x1111},
        {REG_MOD2_96 ,0x1111},
        {REG_MOD2_98 ,0x1111},
        {REG_MOD2_9A ,0x1111},
        {REG_MOD2_9C ,0x1111},
        {REG_MOD2_9E ,0x1111},
        {REG_MOD2_A0 ,0x4444},
        {REG_MOD2_A2 ,0x4444},
        {REG_MOD2_A4 ,0x4444},
        {REG_MOD2_A6 ,0x4444},
        {REG_MOD2_A8 ,0x4444},
        {REG_MOD2_AA ,0x4444},
        {REG_MOD2_AC ,0x4444},
        {REG_MOD2_AE ,0x4444},
        {REG_MOD2_38 ,0x4444},
        {REG_MOD2_3A ,0x4444},
        {REG_MOD2_3C ,0x0000},
        {REG_MOD2_3E ,0x0000},
        {REG_MOD2_60 ,0x1919},
        {REG_MOD2_62 ,0x1919},
        {REG_MOD2_64 ,0x1919},
        {REG_MOD2_66 ,0x1919},
        {REG_MOD2_68 ,0x1919},
        {REG_MOD2_6A ,0x1919},
        {REG_MOD2_6C ,0x1919},
        {REG_MOD2_6E ,0x1919},
        {REG_MOD2_70 ,0x1919},
        {REG_MOD2_72 ,0x1919},
        {REG_MOD2_74 ,0x1919},
        {REG_MOD2_76 ,0x1919},
        {REG_MOD2_78 ,0x1919},
        {REG_MOD2_7A ,0x1919},
        {REG_MOD2_7C ,0x1919},
        {REG_MOD2_7E ,0x1919},

        {REG_DISPMISC_20 ,0x0008},
        {REG_DISPMISC_22 ,0x0000},
        {REG_DISPMISC_24 ,0x0400},
        {REG_DISPMISC_26 ,0x0002},
        {REG_MOD2_EC ,0x0000},

        {REG_DISPMISC_00 ,0x0000},
        {REG_MOD2_EE ,0x0003},

        {REG_MOD1_00 ,0x0201},
        {REG_MOD1_0A ,0x2000},
        {REG_MOD1_60 ,0x5000},
        {REG_MOD1_78 ,0x0000},

        {REG_MOD1_C2 ,0x8F3F},
        {REG_MOD1_C4 ,0xB000},
        {REG_MOD1_C6 ,0xB400},
        {REG_MOD1_C0 ,0x0AAE},

        {REG_MOD2_00 ,0x0007},
        {REG_MOD2_30 ,0x5555},
        {REG_MOD2_32 ,0x5555},
        {REG_MOD2_34 ,0x0000},
        {REG_MOD2_36 ,0x0000},
    #else //4P
        {REG_MOD1_0C ,0x0002},
        {REG_MOD2_02 ,0x0002},
        {REG_MOD2_08 ,0xFFFF},
        {REG_MOD2_0A ,0xFFFF},
        {REG_MOD2_0C ,0x0000},
        {REG_MOD2_0E ,0x0000},
        {REG_MOD2_10 ,0xFFFF},
        {REG_MOD2_12 ,0xFFFF},
        {REG_MOD2_40 ,0x0202},
        {REG_MOD2_42 ,0x0202},
        {REG_MOD2_44 ,0x0202},
        {REG_MOD2_46 ,0x0202},
        {REG_MOD2_48 ,0x0202},
        {REG_MOD2_4A ,0x0202},
        {REG_MOD2_4C ,0x0202},
        {REG_MOD2_4E ,0x0202},
        {REG_MOD2_50 ,0x0202},
        {REG_MOD2_52 ,0x0202},
        {REG_MOD2_54 ,0x0202},
        {REG_MOD2_56 ,0x0202},
        {REG_MOD2_58 ,0x0202},
        {REG_MOD2_5A ,0x0202},
        {REG_MOD2_5C ,0x0202},
        {REG_MOD2_5E ,0x0202},
        {REG_MOD2_80 ,0x1111},
        {REG_MOD2_82 ,0x1111},
        {REG_MOD2_84 ,0x1111},
        {REG_MOD2_86 ,0x1111},
        {REG_MOD2_88 ,0x1111},
        {REG_MOD2_8A ,0x1111},
        {REG_MOD2_8C ,0x1111},
        {REG_MOD2_8E ,0x1111},
        {REG_MOD2_90 ,0x1111},
        {REG_MOD2_92 ,0x1111},
        {REG_MOD2_94 ,0x1111},
        {REG_MOD2_96 ,0x1111},
        {REG_MOD2_98 ,0x1111},
        {REG_MOD2_9A ,0x1111},
        {REG_MOD2_9C ,0x1111},
        {REG_MOD2_9E ,0x1111},
        {REG_MOD2_A0 ,0x4444},
        {REG_MOD2_A2 ,0x4444},
        {REG_MOD2_A4 ,0x4444},
        {REG_MOD2_A6 ,0x4444},
        {REG_MOD2_A8 ,0x4444},
        {REG_MOD2_AA ,0x4444},
        {REG_MOD2_AC ,0x4444},
        {REG_MOD2_AE ,0x4444},
        {REG_MOD2_38 ,0x4444},
        {REG_MOD2_3A ,0x4444},
        {REG_MOD2_3C ,0x0000},
        {REG_MOD2_3E ,0x0000},
        {REG_MOD2_60 ,0x1919},
        {REG_MOD2_62 ,0x1919},
        {REG_MOD2_64 ,0x1919},
        {REG_MOD2_66 ,0x1919},
        {REG_MOD2_68 ,0x1919},
        {REG_MOD2_6A ,0x1919},
        {REG_MOD2_6C ,0x1919},
        {REG_MOD2_6E ,0x1919},
        {REG_MOD2_70 ,0x1919},
        {REG_MOD2_72 ,0x1919},
        {REG_MOD2_74 ,0x1919},
        {REG_MOD2_76 ,0x1919},
        {REG_MOD2_78 ,0x1919},
        {REG_MOD2_7A ,0x1919},
        {REG_MOD2_7C ,0x1919},
        {REG_MOD2_7E ,0x1919},

        {REG_DISPMISC_20 ,0x0008},
        {REG_DISPMISC_22 ,0x0000},
        {REG_DISPMISC_24 ,0x0400},
        {REG_DISPMISC_26 ,0x0002},
        {REG_MOD2_EC ,0x0000},

        {REG_DISPMISC_00 ,0x0000},
        {REG_MOD2_EE ,0x0003},

        {REG_MOD1_00 ,0x0201},
        {REG_MOD1_0A ,0x2000},
        {REG_MOD1_60 ,0x5000},
        {REG_MOD1_78 ,0x0000},

        {REG_MOD1_C2 ,0x8F3F},
        {REG_MOD1_C4 ,0xF000},
        {REG_MOD1_C6 ,0xB400},
        {REG_MOD1_C0 ,0x0AAE},

        {REG_MOD2_00 ,0x0007},
        {REG_MOD2_30 ,0x5555},
        {REG_MOD2_32 ,0x5555},
        {REG_MOD2_34 ,0x0000},
        {REG_MOD2_36 ,0x0000},
    #endif
#elif PANEL_VBY1_16CH_10Bit
    #if ((PanelWidth==5120)&&(PanelHeight==2160)&&(PanelVsyncFreq==120)) //8P
        {REG_MOD1_0C ,0x0002},
        {REG_MOD2_02 ,0x0002},
        {REG_MOD2_08 ,0xFFFF},
        {REG_MOD2_0A ,0xFFFF},
        {REG_MOD2_0C ,0x0000},
        {REG_MOD2_0E ,0x0000},
        {REG_MOD2_10 ,0xFFFF},
        {REG_MOD2_12 ,0xFFFF},
        {REG_MOD2_40 ,0x0202},
        {REG_MOD2_42 ,0x0202},
        {REG_MOD2_44 ,0x0202},
        {REG_MOD2_46 ,0x0202},
        {REG_MOD2_48 ,0x0202},
        {REG_MOD2_4A ,0x0202},
        {REG_MOD2_4C ,0x0202},
        {REG_MOD2_4E ,0x0202},
        {REG_MOD2_50 ,0x0202},
        {REG_MOD2_52 ,0x0202},
        {REG_MOD2_54 ,0x0202},
        {REG_MOD2_56 ,0x0202},
        {REG_MOD2_58 ,0x0202},
        {REG_MOD2_5A ,0x0202},
        {REG_MOD2_5C ,0x0202},
        {REG_MOD2_5E ,0x0202},
        {REG_MOD2_80 ,0x1111},
        {REG_MOD2_82 ,0x1111},
        {REG_MOD2_84 ,0x1111},
        {REG_MOD2_86 ,0x1111},
        {REG_MOD2_88 ,0x1111},
        {REG_MOD2_8A ,0x1111},
        {REG_MOD2_8C ,0x1111},
        {REG_MOD2_8E ,0x1111},
        {REG_MOD2_90 ,0x1111},
        {REG_MOD2_92 ,0x1111},
        {REG_MOD2_94 ,0x1111},
        {REG_MOD2_96 ,0x1111},
        {REG_MOD2_98 ,0x1111},
        {REG_MOD2_9A ,0x1111},
        {REG_MOD2_9C ,0x1111},
        {REG_MOD2_9E ,0x1111},
        {REG_MOD2_A0 ,0x4444},
        {REG_MOD2_A2 ,0x4444},
        {REG_MOD2_A4 ,0x4444},
        {REG_MOD2_A6 ,0x4444},
        {REG_MOD2_A8 ,0x4444},
        {REG_MOD2_AA ,0x4444},
        {REG_MOD2_AC ,0x4444},
        {REG_MOD2_AE ,0x4444},
        {REG_MOD2_38 ,0x4444},
        {REG_MOD2_3A ,0x4444},
        {REG_MOD2_3C ,0x0000},
        {REG_MOD2_3E ,0x0000},
        {REG_MOD2_60 ,0x1919},
        {REG_MOD2_62 ,0x1919},
        {REG_MOD2_64 ,0x1919},
        {REG_MOD2_66 ,0x1919},
        {REG_MOD2_68 ,0x1919},
        {REG_MOD2_6A ,0x1919},
        {REG_MOD2_6C ,0x1919},
        {REG_MOD2_6E ,0x1919},
        {REG_MOD2_70 ,0x1919},
        {REG_MOD2_72 ,0x1919},
        {REG_MOD2_74 ,0x1919},
        {REG_MOD2_76 ,0x1919},
        {REG_MOD2_78 ,0x1919},
        {REG_MOD2_7A ,0x1919},
        {REG_MOD2_7C ,0x1919},
        {REG_MOD2_7E ,0x1919},

        {REG_DISPMISC_20 ,0x0008},
        {REG_DISPMISC_22 ,0x0000},
        {REG_DISPMISC_24 ,0x0400},
        {REG_DISPMISC_26 ,0x0002},
        {REG_MOD2_EC ,0x0000},

        {REG_DISPMISC_00 ,0x0000},
        {REG_MOD2_EE ,0x0003},

        {REG_MOD1_00 ,0x0201},
        {REG_MOD1_0A ,0x2000},
        {REG_MOD1_60 ,0x5000},
        {REG_MOD1_78 ,0x0000},

        {REG_MOD1_C2 ,0x8F3F},
        {REG_MOD1_C4 ,0xA000},
        {REG_MOD1_C6 ,0xB600},
        {REG_MOD1_C0 ,0x0AAE},

        {REG_MOD2_00 ,0x0007},
        {REG_MOD2_30 ,0x5555},
        {REG_MOD2_32 ,0x5555},
        {REG_MOD2_34 ,0x0000},
        {REG_MOD2_36 ,0x0000},
    #else //4P
        {REG_MOD1_0C ,0x0002},
        {REG_MOD2_02 ,0x0002},
        {REG_MOD2_08 ,0xFFFF},
        {REG_MOD2_0A ,0xFFFF},
        {REG_MOD2_0C ,0x0000},
        {REG_MOD2_0E ,0x0000},
        {REG_MOD2_10 ,0xFFFF},
        {REG_MOD2_12 ,0xFFFF},
        {REG_MOD2_40 ,0x0202},
        {REG_MOD2_42 ,0x0202},
        {REG_MOD2_44 ,0x0202},
        {REG_MOD2_46 ,0x0202},
        {REG_MOD2_48 ,0x0202},
        {REG_MOD2_4A ,0x0202},
        {REG_MOD2_4C ,0x0202},
        {REG_MOD2_4E ,0x0202},
        {REG_MOD2_50 ,0x0202},
        {REG_MOD2_52 ,0x0202},
        {REG_MOD2_54 ,0x0202},
        {REG_MOD2_56 ,0x0202},
        {REG_MOD2_58 ,0x0202},
        {REG_MOD2_5A ,0x0202},
        {REG_MOD2_5C ,0x0202},
        {REG_MOD2_5E ,0x0202},
        {REG_MOD2_80 ,0x1111},
        {REG_MOD2_82 ,0x1111},
        {REG_MOD2_84 ,0x1111},
        {REG_MOD2_86 ,0x1111},
        {REG_MOD2_88 ,0x1111},
        {REG_MOD2_8A ,0x1111},
        {REG_MOD2_8C ,0x1111},
        {REG_MOD2_8E ,0x1111},
        {REG_MOD2_90 ,0x1111},
        {REG_MOD2_92 ,0x1111},
        {REG_MOD2_94 ,0x1111},
        {REG_MOD2_96 ,0x1111},
        {REG_MOD2_98 ,0x1111},
        {REG_MOD2_9A ,0x1111},
        {REG_MOD2_9C ,0x1111},
        {REG_MOD2_9E ,0x1111},
        {REG_MOD2_A0 ,0x4444},
        {REG_MOD2_A2 ,0x4444},
        {REG_MOD2_A4 ,0x4444},
        {REG_MOD2_A6 ,0x4444},
        {REG_MOD2_A8 ,0x4444},
        {REG_MOD2_AA ,0x4444},
        {REG_MOD2_AC ,0x4444},
        {REG_MOD2_AE ,0x4444},
        {REG_MOD2_38 ,0x4444},
        {REG_MOD2_3A ,0x4444},
        {REG_MOD2_3C ,0x0000},
        {REG_MOD2_3E ,0x0000},
        {REG_MOD2_60 ,0x1919},
        {REG_MOD2_62 ,0x1919},
        {REG_MOD2_64 ,0x1919},
        {REG_MOD2_66 ,0x1919},
        {REG_MOD2_68 ,0x1919},
        {REG_MOD2_6A ,0x1919},
        {REG_MOD2_6C ,0x1919},
        {REG_MOD2_6E ,0x1919},
        {REG_MOD2_70 ,0x1919},
        {REG_MOD2_72 ,0x1919},
        {REG_MOD2_74 ,0x1919},
        {REG_MOD2_76 ,0x1919},
        {REG_MOD2_78 ,0x1919},
        {REG_MOD2_7A ,0x1919},
        {REG_MOD2_7C ,0x1919},
        {REG_MOD2_7E ,0x1919},

        {REG_DISPMISC_20 ,0x0008},
        {REG_DISPMISC_22 ,0x0000},
        {REG_DISPMISC_24 ,0x0400},
        {REG_DISPMISC_26 ,0x0002},
        {REG_MOD2_EC ,0x0000},

        {REG_DISPMISC_00 ,0x0000},
        {REG_MOD2_EE ,0x0003},

        {REG_MOD1_00 ,0x0201},
        {REG_MOD1_0A ,0x2000},
        {REG_MOD1_60 ,0x5000},
        {REG_MOD1_78 ,0x0000},

        {REG_MOD1_C2 ,0x8F3F},
        {REG_MOD1_C4 ,0xE000},
        {REG_MOD1_C6 ,0xB600},
        {REG_MOD1_C0 ,0x0AAE},

        {REG_MOD2_00 ,0x0007},
        {REG_MOD2_30 ,0x5555},
        {REG_MOD2_32 ,0x5555},
        {REG_MOD2_34 ,0x0000},
        {REG_MOD2_36 ,0x0000},
    #endif
#elif PANEL_VBY1_20CH_8Bit
    {REG_MOD1_0C ,0x0002},
    {REG_MOD2_02 ,0x0002},
    {REG_MOD2_08 ,0xFFFF},
    {REG_MOD2_0A ,0xFFFF},
    {REG_MOD2_0C ,0x0000},
    {REG_MOD2_0E ,0x0000},
    {REG_MOD2_10 ,0xFFFF},
    {REG_MOD2_12 ,0xFFFF},
    {REG_MOD2_40 ,0x0202},
    {REG_MOD2_42 ,0x0202},
    {REG_MOD2_44 ,0x0202},
    {REG_MOD2_46 ,0x0202},
    {REG_MOD2_48 ,0x0202},
    {REG_MOD2_4A ,0x0202},
    {REG_MOD2_4C ,0x0202},
    {REG_MOD2_4E ,0x0202},
    {REG_MOD2_50 ,0x0202},
    {REG_MOD2_52 ,0x0202},
    {REG_MOD2_54 ,0x0202},
    {REG_MOD2_56 ,0x0202},
    {REG_MOD2_58 ,0x0202},
    {REG_MOD2_5A ,0x0202},
    {REG_MOD2_5C ,0x0202},
    {REG_MOD2_5E ,0x0202},
    {REG_MOD2_80 ,0x1111},
    {REG_MOD2_82 ,0x1111},
    {REG_MOD2_84 ,0x1111},
    {REG_MOD2_86 ,0x1111},
    {REG_MOD2_88 ,0x1111},
    {REG_MOD2_8A ,0x1111},
    {REG_MOD2_8C ,0x1111},
    {REG_MOD2_8E ,0x1111},
    {REG_MOD2_90 ,0x1111},
    {REG_MOD2_92 ,0x1111},
    {REG_MOD2_94 ,0x1111},
    {REG_MOD2_96 ,0x1111},
    {REG_MOD2_98 ,0x1111},
    {REG_MOD2_9A ,0x1111},
    {REG_MOD2_9C ,0x1111},
    {REG_MOD2_9E ,0x1111},
    {REG_MOD2_A0 ,0x4444},
    {REG_MOD2_A2 ,0x4444},
    {REG_MOD2_A4 ,0x4444},
    {REG_MOD2_A6 ,0x4444},
    {REG_MOD2_A8 ,0x4444},
    {REG_MOD2_AA ,0x4444},
    {REG_MOD2_AC ,0x4444},
    {REG_MOD2_AE ,0x4444},
    {REG_MOD2_38 ,0x4444},
    {REG_MOD2_3A ,0x4444},
    {REG_MOD2_3C ,0x0444},
    {REG_MOD2_3E ,0x0000},
    {REG_MOD2_60 ,0x1919},
    {REG_MOD2_62 ,0x1919},
    {REG_MOD2_64 ,0x1919},
    {REG_MOD2_66 ,0x1919},
    {REG_MOD2_68 ,0x1919},
    {REG_MOD2_6A ,0x1919},
    {REG_MOD2_6C ,0x1919},
    {REG_MOD2_6E ,0x1919},
    {REG_MOD2_70 ,0x1919},
    {REG_MOD2_72 ,0x1919},
    {REG_MOD2_74 ,0x1919},
    {REG_MOD2_76 ,0x1919},
    {REG_MOD2_78 ,0x1919},
    {REG_MOD2_7A ,0x1919},
    {REG_MOD2_7C ,0x1919},
    {REG_MOD2_7E ,0x1919},

    {REG_DISPMISC_20 ,0x0010},
    {REG_DISPMISC_22 ,0x0004},
    {REG_DISPMISC_24 ,0x0400},
    {REG_DISPMISC_26 ,0x0002},
    {REG_MOD2_EC ,0x0000},

    {REG_DISPMISC_00 ,0x0000},
    {REG_MOD2_EE ,0x0003},

    {REG_MOD1_00 ,0x0201},
    {REG_MOD1_0A ,0x2000},
    {REG_MOD1_60 ,0x5000},
    {REG_MOD1_78 ,0x0000},

    {REG_MOD1_C2 ,0x8F3F},
    {REG_MOD1_C4 ,0xF000},
    {REG_MOD1_C6 ,0xB400},
    {REG_MOD1_C0 ,0x0AAE},

    {REG_MOD2_00 ,0x0007},
    {REG_MOD2_30 ,0x5555},
    {REG_MOD2_32 ,0x5555},
    {REG_MOD2_34 ,0x0055},
    {REG_MOD2_36 ,0x0000},

#elif PANEL_VBY1_20CH_10Bit
    {REG_MOD1_0C ,0x0002},
    {REG_MOD2_02 ,0x0002},
    {REG_MOD2_08 ,0xFFFF},
    {REG_MOD2_0A ,0xFFFF},
    {REG_MOD2_0C ,0x0000},
    {REG_MOD2_0E ,0x0000},
    {REG_MOD2_10 ,0xFFFF},
    {REG_MOD2_12 ,0xFFFF},
    {REG_MOD2_40 ,0x0202},
    {REG_MOD2_42 ,0x0202},
    {REG_MOD2_44 ,0x0202},
    {REG_MOD2_46 ,0x0202},
    {REG_MOD2_48 ,0x0202},
    {REG_MOD2_4A ,0x0202},
    {REG_MOD2_4C ,0x0202},
    {REG_MOD2_4E ,0x0202},
    {REG_MOD2_50 ,0x0202},
    {REG_MOD2_52 ,0x0202},
    {REG_MOD2_54 ,0x0202},
    {REG_MOD2_56 ,0x0202},
    {REG_MOD2_58 ,0x0202},
    {REG_MOD2_5A ,0x0202},
    {REG_MOD2_5C ,0x0202},
    {REG_MOD2_5E ,0x0202},
    {REG_MOD2_80 ,0x1111},
    {REG_MOD2_82 ,0x1111},
    {REG_MOD2_84 ,0x1111},
    {REG_MOD2_86 ,0x1111},
    {REG_MOD2_88 ,0x1111},
    {REG_MOD2_8A ,0x1111},
    {REG_MOD2_8C ,0x1111},
    {REG_MOD2_8E ,0x1111},
    {REG_MOD2_90 ,0x1111},
    {REG_MOD2_92 ,0x1111},
    {REG_MOD2_94 ,0x1111},
    {REG_MOD2_96 ,0x1111},
    {REG_MOD2_98 ,0x1111},
    {REG_MOD2_9A ,0x1111},
    {REG_MOD2_9C ,0x1111},
    {REG_MOD2_9E ,0x1111},
    {REG_MOD2_A0 ,0x4444},
    {REG_MOD2_A2 ,0x4444},
    {REG_MOD2_A4 ,0x4444},
    {REG_MOD2_A6 ,0x4444},
    {REG_MOD2_A8 ,0x4444},
    {REG_MOD2_AA ,0x4444},
    {REG_MOD2_AC ,0x4444},
    {REG_MOD2_AE ,0x4444},
    {REG_MOD2_38 ,0x4444},
    {REG_MOD2_3A ,0x4444},
    {REG_MOD2_3C ,0x0444},
    {REG_MOD2_3E ,0x0000},
    {REG_MOD2_60 ,0x1919},
    {REG_MOD2_62 ,0x1919},
    {REG_MOD2_64 ,0x1919},
    {REG_MOD2_66 ,0x1919},
    {REG_MOD2_68 ,0x1919},
    {REG_MOD2_6A ,0x1919},
    {REG_MOD2_6C ,0x1919},
    {REG_MOD2_6E ,0x1919},
    {REG_MOD2_70 ,0x1919},
    {REG_MOD2_72 ,0x1919},
    {REG_MOD2_74 ,0x1919},
    {REG_MOD2_76 ,0x1919},
    {REG_MOD2_78 ,0x1919},
    {REG_MOD2_7A ,0x1919},
    {REG_MOD2_7C ,0x1919},
    {REG_MOD2_7E ,0x1919},

    {REG_DISPMISC_20 ,0x0010},
    {REG_DISPMISC_22 ,0x0004},
    {REG_DISPMISC_24 ,0x0400},
    {REG_DISPMISC_26 ,0x0002},
    {REG_MOD2_EC ,0x0000},

    {REG_DISPMISC_00 ,0x0000},
    {REG_MOD2_EE ,0x0003},

    {REG_MOD1_00 ,0x0201},
    {REG_MOD1_0A ,0x2000},
    {REG_MOD1_60 ,0x5000},
    {REG_MOD1_78 ,0x0000},

    {REG_MOD1_C2 ,0x8F3F},
    {REG_MOD1_C4 ,0xE000},
    {REG_MOD1_C6 ,0xB400},
    {REG_MOD1_C0 ,0x0AAE},

    {REG_MOD2_00 ,0x0007},
    {REG_MOD2_30 ,0x5555},
    {REG_MOD2_32 ,0x5555},
    {REG_MOD2_34 ,0x0055},
    {REG_MOD2_36 ,0x0000},

#elif PANEL_VBY1_32CH_8Bit
    {REG_MOD1_0C ,0x0002},
    {REG_MOD2_02 ,0x0002},
    {REG_MOD2_08 ,0xFFFF},
    {REG_MOD2_0A ,0xFFFF},
    {REG_MOD2_0C ,0x0000},
    {REG_MOD2_0E ,0x0000},
    {REG_MOD2_10 ,0xFFFF},
    {REG_MOD2_12 ,0xFFFF},
    {REG_MOD2_40 ,0x0202},
    {REG_MOD2_42 ,0x0202},
    {REG_MOD2_44 ,0x0202},
    {REG_MOD2_46 ,0x0202},
    {REG_MOD2_48 ,0x0202},
    {REG_MOD2_4A ,0x0202},
    {REG_MOD2_4C ,0x0202},
    {REG_MOD2_4E ,0x0202},
    {REG_MOD2_50 ,0x0202},
    {REG_MOD2_52 ,0x0202},
    {REG_MOD2_54 ,0x0202},
    {REG_MOD2_56 ,0x0202},
    {REG_MOD2_58 ,0x0202},
    {REG_MOD2_5A ,0x0202},
    {REG_MOD2_5C ,0x0202},
    {REG_MOD2_5E ,0x0202},
    {REG_MOD2_80 ,0x1111},
    {REG_MOD2_82 ,0x1111},
    {REG_MOD2_84 ,0x1111},
    {REG_MOD2_86 ,0x1111},
    {REG_MOD2_88 ,0x1111},
    {REG_MOD2_8A ,0x1111},
    {REG_MOD2_8C ,0x1111},
    {REG_MOD2_8E ,0x1111},
    {REG_MOD2_90 ,0x1111},
    {REG_MOD2_92 ,0x1111},
    {REG_MOD2_94 ,0x1111},
    {REG_MOD2_96 ,0x1111},
    {REG_MOD2_98 ,0x1111},
    {REG_MOD2_9A ,0x1111},
    {REG_MOD2_9C ,0x1111},
    {REG_MOD2_9E ,0x1111},
    {REG_MOD2_A0 ,0x4444},
    {REG_MOD2_A2 ,0x4444},
    {REG_MOD2_A4 ,0x4444},
    {REG_MOD2_A6 ,0x4444},
    {REG_MOD2_A8 ,0x4444},
    {REG_MOD2_AA ,0x4444},
    {REG_MOD2_AC ,0x4444},
    {REG_MOD2_AE ,0x4444},
    {REG_MOD2_38 ,0x4444},
    {REG_MOD2_3A ,0x4444},
    {REG_MOD2_3C ,0x4444},
    {REG_MOD2_3E ,0x4444},
    {REG_MOD2_60 ,0x1919},
    {REG_MOD2_62 ,0x1919},
    {REG_MOD2_64 ,0x1919},
    {REG_MOD2_66 ,0x1919},
    {REG_MOD2_68 ,0x1919},
    {REG_MOD2_6A ,0x1919},
    {REG_MOD2_6C ,0x1919},
    {REG_MOD2_6E ,0x1919},
    {REG_MOD2_70 ,0x1919},
    {REG_MOD2_72 ,0x1919},
    {REG_MOD2_74 ,0x1919},
    {REG_MOD2_76 ,0x1919},
    {REG_MOD2_78 ,0x1919},
    {REG_MOD2_7A ,0x1919},
    {REG_MOD2_7C ,0x1919},
    {REG_MOD2_7E ,0x1919},

    {REG_DISPMISC_20 ,0x0008},
    {REG_DISPMISC_22 ,0x0000},
    {REG_DISPMISC_24 ,0x0400},
    {REG_DISPMISC_26 ,0x0002},
    {REG_MOD2_EC ,0x0000},

    {REG_DISPMISC_00 ,0x0000},
    {REG_MOD2_EE ,0x0003},

    {REG_MOD1_00 ,0x0201},
    {REG_MOD1_0A ,0x2000},
    {REG_MOD1_60 ,0x5000},
    {REG_MOD1_78 ,0x0000},

    {REG_MOD1_C2 ,0x8F3F},
    {REG_MOD1_C4 ,0xF000},
    {REG_MOD1_C6 ,0xB400},
    {REG_MOD1_C0 ,0x0AAE},

    {REG_MOD2_00 ,0x1F},
    {REG_MOD2_30 ,0x5555},
    {REG_MOD2_32 ,0x5555},
    {REG_MOD2_34 ,0x5555},
    {REG_MOD2_36 ,0x5555},

#elif PANEL_VBY1_32CH_10Bit
    {REG_MOD1_0C ,0x0002},
    {REG_MOD2_02 ,0x0002},
    {REG_MOD2_08 ,0xFFFF},
    {REG_MOD2_0A ,0xFFFF},
    {REG_MOD2_0C ,0x0000},
    {REG_MOD2_0E ,0x0000},
    {REG_MOD2_10 ,0xFFFF},
    {REG_MOD2_12 ,0xFFFF},
    {REG_MOD2_40 ,0x0202},
    {REG_MOD2_42 ,0x0202},
    {REG_MOD2_44 ,0x0202},
    {REG_MOD2_46 ,0x0202},
    {REG_MOD2_48 ,0x0202},
    {REG_MOD2_4A ,0x0202},
    {REG_MOD2_4C ,0x0202},
    {REG_MOD2_4E ,0x0202},
    {REG_MOD2_50 ,0x0202},
    {REG_MOD2_52 ,0x0202},
    {REG_MOD2_54 ,0x0202},
    {REG_MOD2_56 ,0x0202},
    {REG_MOD2_58 ,0x0202},
    {REG_MOD2_5A ,0x0202},
    {REG_MOD2_5C ,0x0202},
    {REG_MOD2_5E ,0x0202},
    {REG_MOD2_80 ,0x1111},
    {REG_MOD2_82 ,0x1111},
    {REG_MOD2_84 ,0x1111},
    {REG_MOD2_86 ,0x1111},
    {REG_MOD2_88 ,0x1111},
    {REG_MOD2_8A ,0x1111},
    {REG_MOD2_8C ,0x1111},
    {REG_MOD2_8E ,0x1111},
    {REG_MOD2_90 ,0x1111},
    {REG_MOD2_92 ,0x1111},
    {REG_MOD2_94 ,0x1111},
    {REG_MOD2_96 ,0x1111},
    {REG_MOD2_98 ,0x1111},
    {REG_MOD2_9A ,0x1111},
    {REG_MOD2_9C ,0x1111},
    {REG_MOD2_9E ,0x1111},
    {REG_MOD2_A0 ,0x4444},
    {REG_MOD2_A2 ,0x4444},
    {REG_MOD2_A4 ,0x4444},
    {REG_MOD2_A6 ,0x4444},
    {REG_MOD2_A8 ,0x4444},
    {REG_MOD2_AA ,0x4444},
    {REG_MOD2_AC ,0x4444},
    {REG_MOD2_AE ,0x4444},
    {REG_MOD2_38 ,0x4444},
    {REG_MOD2_3A ,0x4444},
    {REG_MOD2_3C ,0x4444},
    {REG_MOD2_3E ,0x4444},
    {REG_MOD2_60 ,0x1919},
    {REG_MOD2_62 ,0x1919},
    {REG_MOD2_64 ,0x1919},
    {REG_MOD2_66 ,0x1919},
    {REG_MOD2_68 ,0x1919},
    {REG_MOD2_6A ,0x1919},
    {REG_MOD2_6C ,0x1919},
    {REG_MOD2_6E ,0x1919},
    {REG_MOD2_70 ,0x1919},
    {REG_MOD2_72 ,0x1919},
    {REG_MOD2_74 ,0x1919},
    {REG_MOD2_76 ,0x1919},
    {REG_MOD2_78 ,0x1919},
    {REG_MOD2_7A ,0x1919},
    {REG_MOD2_7C ,0x1919},
    {REG_MOD2_7E ,0x1919},

    {REG_DISPMISC_20 ,0x0008},
    {REG_DISPMISC_22 ,0x0000},
    {REG_DISPMISC_24 ,0x0400},
    {REG_DISPMISC_26 ,0x0002},
    {REG_MOD2_EC ,0x0000},

    {REG_DISPMISC_00 ,0x0000},
    {REG_MOD2_EE ,0x0003},

    {REG_MOD1_00 ,0x0201},
    {REG_MOD1_0A ,0x2000},
    {REG_MOD1_60 ,0x5000},
    {REG_MOD1_78 ,0x0000},

    {REG_MOD1_C2 ,0x8F3F},
    {REG_MOD1_C4 ,0xE000},
    {REG_MOD1_C6 ,0xB600},
    {REG_MOD1_C0 ,0x0AAE},

    {REG_MOD2_00 ,0x1F},
    {REG_MOD2_30 ,0x5555},
    {REG_MOD2_32 ,0x5555},
    {REG_MOD2_34 ,0x5555},
    {REG_MOD2_36 ,0x5555},

#elif PANEL_EDP
#endif

};
#endif

code RegTbl2Type tblInitMOD2[]=
{
    #if CHIP_ID==MST9U||CHIP_ID==MST9U2||CHIP_ID==MST9U3
        {0, 0 ,0},
    #else //MST9U4
        {REG_MOD1_00, (PanelInvHSync&BIT5)|(PanelInvDE&BIT7)|(PanelInvVSync&BIT6)   ,BIT7|BIT6|BIT5},
        {REG_MOD1_0B, (BIT6&LVDS_TIMode)|(BIT3&PANEL_SWAP_PN)|(PanelSwapMLSB&BIT2)  ,BIT6|BIT3|BIT2},
        {REG_MOD1_E8, PanelDClkDelay<<12 ,0xFFFF }, // DE delay, clock delay for TTL output
        //{REG_MOD1_03, (PanelSwapOddRB&BIT2)|(PanelSwapEvenRB&BIT5)|(PanelSwapOddML&BIT2)|(PanelSwapEvenML&BIT3)},
        {REG_MOD1_1E, (LVDS_CH_D_SWAP<<12)|(LVDS_CH_C_SWAP<<8)|(LVDS_CH_B_SWAP<<4)|(LVDS_CH_A_SWAP),0xFFFF},

        #if PANEL_VBY1()
            {REG_MOD2_04, (PANEL_SWAP_PN?0xFFFF:0x0000) ,0xFFFF},//PN swap for VBY1
        #endif
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

#if MSTAR_TCON
BYTE code TCON_RESET_CMD[7] =
{
    0x43, 0x1B, 0x10, 0x27, 0x02, 0x00, 0x00,
};
BYTE code TCON_RESTORE_CMD[7] =
{
    0x33, 0x1B, 0x10, 0x27, 0x02, 0xFF, 0x00,
};
#endif

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
#if (CHIP_ID == MST9U3)
        msWriteByteMask(REG_120F49, _BIT0, _BIT0);//reg_ckg_aesdma
#endif
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
        msWriteByteMask(REG_120F1F, _BIT2, _BIT3|_BIT2|_BIT0);//reg_ckg_pll_idclk
        msWriteByteMask(REG_120F21, _BIT3|_BIT2, _BIT3|_BIT2|_BIT0);//reg_ckg_ge
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
#if (CHIP_ID == MST9U3)
        msWriteByteMask(REG_120F49, 0x00, _BIT0);//reg_ckg_aesdma
#endif
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

#if PANEL_VBY1()
///////////////////////////////////////////////////////////////////////////////
// <Function>: msDrvCheckVBY1LockN
//
// <Description>: Check LockN value for VBY1 interface
//
// <Parameter>: None
//
///////////////////////////////////////////////////////////////////////////////
#if CHIP_ID==MST9U||CHIP_ID==MST9U2||CHIP_ID==MST9U3
void msDrvCheckVBY1LockN(BYTE LockTemp)
{
    bit bVby1CheckLockPass = false;
    int chkLock;
    unsigned int u32Cnt;
    BYTE u8Temp0, u8Temp1;      // For spped up regsiter setting instead of unsing msWriteByteMask

    #if !PANEL_VBY1_HW_MODE
    #if ( CHIP_ID == MST9U3)
        #define VX1_HPDN_STATUS     (msReadByte(REG_MOD1_19)&BIT7)
        #define VX1_LOCKN_STATUS    (msReadByte(REG_MOD1_19)&BIT6)
    #else
        #define VX1_HPDN_STATUS     (msReadByte(REG_MOD2_D1)&BIT7)
        #define VX1_LOCKN_STATUS    (msReadByte(REG_MOD2_D1)&BIT6)
    #endif

    if(VX1_LOCKN_STATUS||VX1_HPDN_STATUS)
    {
        #if ENABLE_VBY1_TO_EDP
        #define LOCKN_STABLE_COUNTER    4000    // about 1280us (lockN timeout is about 1270us in MSG1235)
        #else
        #define LOCKN_STABLE_COUNTER    2000//250     // about 80us
        #endif

        drvmStar_printMsg("VBY1 Training (SW) start");
        #if ENABLE_VBY1_TO_EDP
		#if PANEL_VBY1_4CH_8Bit||PANEL_VBY1_4CH_10Bit
		msVTEWriteByte(0x2704, 0x55);	// enable signal that whether lockn consider this lane or not
		msVTEWriteByte(0x2705, 0x00);
		msVTEWriteByte(0x2712, 0x55);	// enable lane inter skew function
		msVTEWriteByte(0x2713, 0x00);
		msVTEWriteByte(0x273A, 0x77);	// bypass 1st stage 2p->1p
		msVTEWriteByte(0x273B, 0x00);
		msVTEWriteByte(0x2732, VBY1_PHY_LANE0);	// video clk select
		msVTEWriteByte(0x272A, VBY1_PHY_LANE0);	// lane selection
		msVTEWriteByte(0x272B, VBY1_PHY_LANE2);
		msVTEWriteByte(0x272C, VBY1_PHY_LANE1);
		msVTEWriteByte(0x272D, VBY1_PHY_LANE3);
		#endif
        msVTEWriteByte(0x2630,0x0E);                // Set MSG1235 clock tolerance
        #endif
        u8Temp0 = msReadByte(REG_MOD1_61) & 0x7F;
        u8Temp1 = msReadByte(REG_MOD1_C5) & 0x7F;
        if(LockTemp)
        TimeOutCounter = 1500;
        else
        TimeOutCounter = 500;

        while( TimeOutCounter )
        {
            msWriteByteMask(REG_MOD1_61, BIT7, BIT7);        // Enable HW check when lock lossing
            msWriteByteMask(REG_MOD1_00, 0x00, 0x01);
            msWriteByteMask(REG_MOD1_00, 0x01, 0x01);
            msWriteByteMask(REG_MOD1_C5, 0x00, 0x80);
            msWriteByteMask(REG_MOD1_C5, 0x80, 0x80);

            if((VX1_LOCKN_STATUS)&&!(VX1_HPDN_STATUS))
            {
                MsOS_CPU_DisableInterrupt();
                msWrite2Byte(REG_MOD1_C0, 0x0F5E);
                msWrite2Byte(REG_MOD1_C0, 0x0FDE);
                msWrite2Byte(REG_MOD1_C0, 0x0F9E);
                msWrite2Byte(REG_MOD1_C0, 0x0FBE);           // 20140916 by Version 5.0
                msWrite2Byte(REG_MOD1_C0, 0x0F9E);
                msWrite2Byte(REG_MOD1_C0, 0x0FBE);

                #if MSTAR_TCON
                Panel_i2cBurstWriteBytes(PANEL_DEVICE, TCON_RESET_CMD, sizeof(TCON_RESET_CMD));
                for( u32Cnt=0; u32Cnt<7500; u32Cnt++)
                    Delay4us();
                Panel_i2cBurstWriteBytes(PANEL_DEVICE, TCON_RESTORE_CMD, sizeof(TCON_RESTORE_CMD));
                drvmStar_printMsg("******   Sent TCON reset command  *******");
                #endif

                #if ENABLE_VBY1_TO_EDP
                msVTEVXOneOnOff(TRUE);
                #endif

                u32Cnt = 10000;                             // Polling lockN status maximun 10ms
                while((VX1_LOCKN_STATUS || VX1_HPDN_STATUS) && (u32Cnt-- > 0))
                    DelayUs(1);

                msWrite2Byte(REG_MOD1_C0, 0x0FAE);
                msRegs(REG_MOD1_61) = u8Temp0;               // Disable HW check when lock done
                msRegs(REG_MOD1_C5) = u8Temp1;
                msRegs(REG_MOD1_C5) = u8Temp1 | 0x80;

                bVby1CheckLockPass=TRUE;
                for(chkLock=0 ; chkLock<LOCKN_STABLE_COUNTER ; chkLock++)
                {
                    if(VX1_LOCKN_STATUS || VX1_HPDN_STATUS)
                    {
                        bVby1CheckLockPass=FALSE;
                        drvmStar_printMsg("Check Lock ERROR!");
                        drvmStar_printData("chkLock = %d", chkLock);
                        break;
                    }
                }
                MsOS_CPU_EnableInterrupt();

                if(bVby1CheckLockPass)
                {
                    #if ENABLE_VBY1_TO_EDP
                    msVTEWriteByte(0x2630,0x2B);            // Set MSG1235 clock tolerance
                    #endif
                    drvmStar_printData("VBY1 Training PASS!",0);
                    break;
                }
                else
                {
                    #if ENABLE_VBY1_TO_EDP
                    msVTEVXOneOnOff(FALSE);
                    #endif
                    drvmStar_printData("VBY1 Training FAIL!",0);
                }
            }
        }

        if( TimeOutCounter == 0)
            drvmStar_printMsg("******VBY1 Training Timeout(SW)*******");
        else
            drvmStar_printData("******VBY1 LockTotalTime===%d",TimeOutCounter);

        #undef  LOCKN_STABLE_COUNTER
    }
#else
#if 1
    if((msReadByte(REG_MOD2_D1)&BIT6)||((msReadByte(REG_MOD2_D1)&BIT7)))
    {
        drvmStar_printMsg("******VBY1 Software Reset *******");
        msWriteByteMask(REG_MOD1_61, BIT7, BIT7);//Enable HW check when lock lossing
        TimeOutCounter = 500;
        while( TimeOutCounter )
        {
            msWriteByteMask(REG_MOD1_00, 0x00, 0x01);
            ForceDelay1ms(5); // need check
            msWriteByteMask(REG_MOD1_00, 0x01, 0x01);

            msWriteByteMask(REG_MOD1_C5, 0x00, 0x80);
            ForceDelay1ms(5); // need check
            msWriteByteMask(REG_MOD1_C5, 0x80, 0x80);
            if((!(msReadByte(REG_MOD2_D1)&BIT6))&&(!(msReadByte(REG_MOD2_D1)&BIT7)))
            {
                ForceDelay1ms(2); // need check
                if((!(msReadByte(REG_MOD2_D1)&BIT6))&&(!(msReadByte(REG_MOD2_D1)&BIT7)))
                {
                    msWriteByteMask(REG_MOD1_61, 0x00, BIT7);//Disable HW check when lock done
                    break;
                }
            }
        }
        if( TimeOutCounter == 0)
        {
            drvmStar_printMsg("******VBY1 Training Timeout(HW)*******");
        }
    }
#else
    if(msReadByte(REG_MOD2_D1)&BIT6)
    {
        msWriteBit(REG_MOD2_B0, (msReadByte(REG_MOD2_B0)&BIT7)?0x00:BIT7 ,BIT7);
        ForceDelay1ms(2);
        msWriteBit(REG_MOD2_B0, (msReadByte(REG_MOD2_B0)&BIT7)?0x00:BIT7 ,BIT7);
    }
    else if(msReadByte(REG_MOD2_D1)&BIT7)
    {
        msWriteBit(REG_MOD2_B0, (msReadByte(REG_MOD2_B0)&BIT6)?0x00:BIT6 ,BIT6);
        ForceDelay1ms(2);
         msWriteBit(REG_MOD2_B0, (msReadByte(REG_MOD2_B0)&BIT6)?0x00:BIT6 ,BIT6);
    }
#endif
#endif
}
#else   //MST9U4
void msDrvCheckVBY1LockN(BYTE LockTemp)
{
    bit bVby1CheckLockPass = false;
    int chkLock;
    unsigned int u32Cnt;

    #define VX1_LOCKN_STATUS    (msReadByte(REG_MOD1_C3)&BIT6)


#if !PANEL_VBY1_HW_MODE
    BYTE u8Temp0, u8Temp1;      // For spped up regsiter setting instead of unsing msWriteByteMask

    if(VX1_LOCKN_STATUS)
    {
        #define LOCKN_STABLE_COUNTER    250     // about 80us

        drvmStar_printMsg("VBY1 Training (SW) start");

        u8Temp0 = msReadByte(REG_MOD1_61) & 0x7F;
        u8Temp1 = msReadByte(REG_MOD1_C5) & 0x7F;
		
        if(LockTemp)
        TimeOutCounter = 1500;
        else
        TimeOutCounter = 500;

        while( TimeOutCounter )
        {
            msWriteByteMask(REG_MOD1_61, BIT7, BIT7);        // Enable HW check when lock lossing
            msWriteByteMask(REG_MOD1_00, 0x00, 0x01);
            msWriteByteMask(REG_MOD1_00, 0x01, 0x01);
            msWriteByteMask(REG_MOD1_C5, 0x00, 0x80);
            msWriteByteMask(REG_MOD1_C5, 0x80, 0x80);

            if((VX1_LOCKN_STATUS))
            {
                MsOS_CPU_DisableInterrupt();
                msWrite2Byte(REG_MOD1_C0, 0x0F5E);
                msWrite2Byte(REG_MOD1_C0, 0x0FDE);
                msWrite2Byte(REG_MOD1_C0, 0x0F9E);
                msWrite2Byte(REG_MOD1_C0, 0x0FBE);           // 20140916 by Version 5.0
                msWrite2Byte(REG_MOD1_C0, 0x0F9E);
                msWrite2Byte(REG_MOD1_C0, 0x0FBE);

                #if MSTAR_TCON
                Panel_i2cBurstWriteBytes(PANEL_DEVICE, TCON_RESET_CMD, sizeof(TCON_RESET_CMD));
                for( u32Cnt=0; u32Cnt<7500; u32Cnt++)
                    Delay4us();
                Panel_i2cBurstWriteBytes(PANEL_DEVICE, TCON_RESTORE_CMD, sizeof(TCON_RESTORE_CMD));
                drvmStar_printMsg("******   Sent TCON reset command  *******");
                #endif

                u32Cnt = 10000;                             // Polling lockN status maximun 10ms
                while((VX1_LOCKN_STATUS) && (u32Cnt-- > 0))
                    DelayUs(1);

                msWrite2Byte(REG_MOD1_C0, 0x0FAE);
                msRegs(REG_MOD1_61) = u8Temp0;               // Disable HW check when lock done
                msRegs(REG_MOD1_C5) = u8Temp1;
                msRegs(REG_MOD1_C5) = u8Temp1 | 0x80;

                bVby1CheckLockPass=TRUE;
                for(chkLock=0 ; chkLock<LOCKN_STABLE_COUNTER ; chkLock++)
                {
                    if(VX1_LOCKN_STATUS)
                    {
                        bVby1CheckLockPass=FALSE;
                        drvmStar_printMsg("Check Lock ERROR!");
                        drvmStar_printData("chkLock = %d", chkLock);
                        break;
                    }
                }
                MsOS_CPU_EnableInterrupt();

                if(bVby1CheckLockPass)
                {
                    drvmStar_printData("VBY1 Training PASS!",0);
                    break;
                }
                else
                {
                    drvmStar_printData("VBY1 Training FAIL!",0);
                }
            }
        }

        if( TimeOutCounter == 0)
            drvmStar_printMsg("******VBY1 Training Timeout(SW)*******");
        else
            drvmStar_printData("******VBY1 LockTotalTime===%d",TimeOutCounter);

        #undef  LOCKN_STABLE_COUNTER
    }
#else
    #define LOCKN_STABLE_COUNTER    250     // about 80us
    drvmStar_printMsg("VBY1 Training (HW start");

    TimeOutCounter = 500;
    while( TimeOutCounter )
    {
        msWrite2Byte(REG_MOD1_C0, 0x0000);
        ForceDelay1ms(1);
        msWrite2Byte(REG_MOD1_C0, 0x0AAE);

        if(VX1_LOCKN_STATUS)
        {
            u32Cnt = 10000;                             // Polling lockN status maximun 10ms
            while((VX1_LOCKN_STATUS ) && (u32Cnt-- > 0))
                DelayUs(1);
            bVby1CheckLockPass=TRUE;
        }
        for(chkLock=0 ; chkLock<LOCKN_STABLE_COUNTER ; chkLock++)
        {
            if(VX1_LOCKN_STATUS)
            {
                bVby1CheckLockPass=FALSE;
                drvmStar_printMsg("Check Lock ERROR!");
                drvmStar_printData("chkLock = %d", chkLock);
                break;
            }
        }
        if(bVby1CheckLockPass)
        {
            drvmStar_printData("VBY1 Training PASS!",0);
            break;
        }
        else
        {
            drvmStar_printData("VBY1 Training FAIL!",0);
        }
     }
     if( TimeOutCounter == 0)
        drvmStar_printMsg("******VBY1 Training Timeout(SW)*******");
     else
        drvmStar_printData("******VBY1 LockTotalTime===%d",TimeOutCounter);
     #undef  LOCKN_STABLE_COUNTER
#endif
}
#endif
#endif

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
    DWORD u32FrmaeColor;
    BYTE u8Pattern = DEF_FRAME_COLOR_PATTERN;

    #if ENABLE_DAISY_CHAIN
    extern BYTE tCOMBO_HDCP_KEY_TABLE[], tCOMBO_HDCP_BKSV[], tMHL_EDID_SIZE_PORT[];
    extern BYTE* tCOMBO_EDID_TALBE_PORT[];
    ST_DC_HOST_FEATURE stDCHostFeature;
    #endif

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

    //remove redundant function calls which use a non-initialized global variable, they will be called in APPWindow_Init();
    //msDrvIP1SetOutputLockMode(FALSE, SCL0_MAIN/*//tmp*/);//_msWriteByteMask(SC0_02, BIT7, BIT7);
    //msDrvIP1SetOutputFreerun(TRUE, SCL0_MAIN/*//tmp*/);

    for( i = 0; i < sizeof( tblInit ) / sizeof( RegUnitType ); i++ )
        msWriteByte( tblInit[i].u16Reg, tblInit[i].u8Value );
    //for( i = 0; i < sizeof( tb_3D_DITHER_TABLE ) / sizeof( RegUnitType ); i++ )
    //    _msWriteByte( tb_3D_DITHER_TABLE[i].u16Reg, tb_3D_DITHER_TABLE[i].u8Value );

    msDrvMODInit();

#ifdef MST9U_FPGA //MST9U4 FPGA
    msWriteByteMask(SCL0_24_7E, BIT6, BIT6);
    msWrite2Byte(REG_101E06, 0x4000);
#endif


#if ENABLE_DAISY_CHAIN
    stDCHostFeature.u8ChipID = CHIP_ID;
    stDCHostFeature.u8CorePowerPin = CORE_POWER_PIN_HIGH_ACTIVE;
    stDCHostFeature.u8FreeSyncMode = ((FreeSyncFlag)?1:0);

    //For the case that Host chip need to load Rx HDCP key to DC
    #if(ENABLE_LOAD_KEY_VIA_EFUSE)
    msEread_LoadKeyViaEfuse(tCOMBO_HDCP_KEY_TABLE, tCOMBO_HDCP_BKSV, EFUSE_0);
    #endif

    switch(USER_PREF_DPConfigType)
    {
        case DP11:      // DP1.1 SST
            stDCHostFeature.u8StreamMode = DP1_1_SST;
            break;
        case DP12:      // DP1.2 SST
            stDCHostFeature.u8StreamMode = DP1_2_SST;
            break;
        //case DP12MST:   // DP1.2 MST
        //    stDCHostFeature.u8StreamMode = DP1_2_MST;
        //    break;
        default:
            stDCHostFeature.u8StreamMode = DP1_2_SST;
            break;
    }
    msDCInit(TRUE, tCOMBO_HDCP_KEY_TABLE, tCOMBO_HDCP_BKSV, tCOMBO_EDID_TALBE_PORT[COMBO_DAISY_CHAIN_EDID_PORT], tMHL_EDID_SIZE_PORT[COMBO_DAISY_CHAIN_EDID_PORT]*128, stDCHostFeature);
#endif

    //SAR Enable
    msWriteByteMask(REG_003A62, (SAR_ENABLE&0x0F), 0x0F);   // SAR0-3
    msWriteByte(REG_003A66, (SAR_ENABLE>>4));               // SAR4-11

    msWriteByteMask(REG_003E60, BIT7, BIT7);//reply ACK while source accesses A0_EDID with address is over 128

    //_msWriteByteMask(SC0_0E, BIT7, BIT7); // H coring disable, for dot pattern certical block issue, Jison 100610
    msDrvIP1Init();
    msDrvScSetupPathInit(); //Jison need put here because mStar_SetupFreeRunMode() will use g_SetupPathInfo variable

	
    APPSystem_Init();
    //APPInput_Init();
    APPWindow_Init();
    msDrvSrcConfigInit();
    msAPIWinFreerun(USER_PREF_WIN_SEL, TRUE);
#ifndef MST9U_FPGA // prevent HDCP key access XIU

//	msAPI_combo_IPInsertHDCP22Key(TRUE, ExtKey); //external key
#if ENABLE_HDCP22
#if 0//ENABLE_USER_PREF_HDCP
    msAPI_combo_IPInsertHDCP22Key(TRUE, tCOMBO_USER_HDCP22_CERTRX_TABLE);
#else
    msAPI_combo_IPInsertHDCP22Key(FALSE, NULL);
#endif
#endif
//    msAPI_combo_IPInsertHDCP22Key(FALSE, NULL); //internal key

    #if ((ENABLE_SECU_R2) && (COMBO_HDCP2_INITPROC_NEW_MODE))
        while (!msAPI_combo_HDCP2InitHandler()); //wait until hdcp22 init done;
    #endif

    msAPI_combo_IPInitial();

    #if ENABLE_DP_INPUT && !ENABLE_DAISY_CHAIN
        U8 PortIndex = 0;
        for(PortIndex=First_Input_Port;PortIndex<Input_Nums;PortIndex++)
            msAPI_combo_IPSetDPDRRFunction((EN_COMBO_IP_SELECT)g_InputPort[PortIndex].eIPMux, ((FreeSyncFlag)?1:0));
    #endif

    #if (ENABLE_CEC == 1)
    api_CEC_init((WORD)EN_CEC_SET_LIST);
    #endif

#endif

    u32FrmaeColor = (((DWORD)((u8Pattern & 0x03) << 6) | (((u8Pattern&_BIT0)?0x3F:0x00))) << 16);
    u32FrmaeColor |= (((DWORD)((u8Pattern & 0x1C) << 3) | (((u8Pattern&_BIT2)?0x1F:0x00))) << 8);
    u32FrmaeColor |=  ((DWORD)((u8Pattern & 0xE0)) | (((u8Pattern&_BIT5)?0x1F:0x00)));

   // For Each Scaler Init.
    for( i = SCL0_SUB ; i<SC_MAX_NUMS ; i++)
    {
        DWORD u32SCRegBase = SCRegBase(i);

        if(i & BIT0) // MAIN
        {
            #if (PanelDither == 8)
            msWriteByteMask(SC10_38 + u32SCRegBase, BIT7, BIT7);  //PARFC 10/12 to 8 reset noise generator by frames enable
            msWriteByteMask(SC10_D5 + u32SCRegBase, BIT4, BIT4);  //PARFC 10/12 to 8 frame period for dither pseudo noise reset
            #endif
            #if (CHIP_ID != MST9U4)
            msWrite2Byte(SC01_20 + u32SCRegBase, 0x0001|(AOVDV_VALUE<<12)); // Auto position enable
            msWriteByteMask(SC01_52 + u32SCRegBase, BIT5, BIT5|BIT4);  //Interlace detect mode
            #endif
            msWriteByteMask(SC10_03 + u32SCRegBase, BIT7|BIT6, BIT7|BIT6); // Main/Sub window mute default by Main/Sub source
            msWriteByteMask(SC10_47 + u32SCRegBase, 0x0F, 0x0F); // Main/Sub window mute source: no signal/av mute enable
            msWriteByte(SC10_48 + u32SCRegBase, u8Pattern); // frame color
            msWrite3Byte(SC10_33 + u32SCRegBase, u32FrmaeColor);

        #if ENABLE_SUPER_RESOLUTION
            msWriteByteMask(SC19_20 + u32SCRegBase, BIT7, BIT7);
        #endif
        }
        else
        {
            #if (CHIP_ID != MST9U4)
            msWrite2Byte(SC03_20 + u32SCRegBase, 0x0001|(AOVDV_VALUE<<12)); // Auto position enable
            msWriteByteMask(SC03_52 + u32SCRegBase, BIT5, BIT5|BIT4);  //Interlace detect mode
            #endif
            msWriteByte(SC0F_2F + u32SCRegBase, u8Pattern); // frame color
        }

        // init Hperiod detect 8 line mode
        msDrvIP1SetHperiodDetectMode(TRUE, i);

        // init window timing gen
        {
            WORD u16PnlHDEStart = PanelHStart/(msDrvISSCDualPort() ? 2:1);
            WORD u16PnlHDEEnd = PanelHStart/(msDrvISSCDualPort() ? 2:1) + PANEL_WIDTH/(msDrvISSCDualPort() ? 2:1) -1;
            WORD u16PnlVDEStart = (g_ScalerInfo[SCL0_MAIN].u8SiFbMode ? VERTICAL_DE_START:0x00);
            WORD u16PnlVDEEnd = u16PnlVDEStart + PANEL_HEIGHT - 1;
            WORD u16Htotal = ( PanelHTotal/(msDrvISSCDualPort() ? 2:1) - 1 );
            BYTE u8HsyncEnd = (BYTE)(( PanelHSyncWidth/(msDrvISSCDualPort() ? 2:1) ) - 2);
            BYTE u8DBBackup = msReadByte(SC00_02 + u32SCRegBase);
            WORD u16VsyncStart = PanelVTotal - PanelVStart + VERTICAL_DE_START;

            u16VsyncStart = (u16VsyncStart%PanelVTotal)|(((u16VsyncStart%PanelVTotal)<<1)&0x1000);

            u16PnlHDEStart -=(i == SCR0_MAIN)?OD_GUARD_BAND:0;
            u16PnlHDEEnd += (i == SCL0_MAIN || i == SCL0_OFFLINE)?OD_GUARD_BAND:0;
            msWriteByte(SC00_02 + u32SCRegBase , 0x00); // double buffer
            msWrite2ByteMask(u32SCRegBase+SC10_0C,u16PnlVDEStart,0x0FFF);//[11:0]vfDE_start
            msWrite2ByteMask(u32SCRegBase+SC10_0E,u16PnlVDEEnd,0x0FFF);//[11:0]vfDE_end
            msWrite2ByteMask(u32SCRegBase+SC10_08,u16PnlHDEStart,0x1FFF);//[12:0]hfDE_start
#if (PANEL_EDP && ((CHIP_ID == MST9U)||(CHIP_ID == MST9U2)))
            msWrite2ByteMask(u32SCRegBase+SC10_0A,u16PnlHDEEnd + 16,0x1FFF);//[12:0]hfDE_end
#else
            msWrite2ByteMask(u32SCRegBase+SC10_0A,u16PnlHDEEnd,0x1FFF);//[12:0]hfDE_end
#endif
            msWrite2ByteMask(u32SCRegBase+SC10_1A, ( PanelVTotal - 1 ),0x0FFF);//[11:0]Output Vtt
            msWrite2ByteMask(u32SCRegBase+SC10_04,u16VsyncStart,0x17FF);//[10:0]vsync_start
            msWrite2ByteMask(u32SCRegBase+SC10_06, (( PanelVTotal - PanelVSyncBackPorch + VERTICAL_DE_START )%PanelVTotal),0x0FFF);//[10:0]vsync_start
            msWrite2ByteMask(u32SCRegBase+SC10_18,u16Htotal,0x0FFF);//[11:0]Output Htt
            msWriteByte(u32SCRegBase+SC10_02,u8HsyncEnd);//[7:0]hsync_end
            msWriteByte(SC00_02 + u32SCRegBase , u8DBBackup); // double buffer
        }
    }

#if (ENABLE_DAISY_CHAIN && (CHIP_ID == MST9U3))
    //For chip except MST9U2DC, we init Daisy_chain's Rx HPD here.
    if(g_eBootStatus != eBOOT_STATUS_DCOFF) {
    msDCInit_DPRxHPD();
        if((g_eBootStatus == eBOOT_STATUS_DCON)
            || (g_eBootStatus == eBOOT_STATUS_DPMSON))
       {
        msDCForceTx1Retrain();
    }
    }
#endif


if(POWER_ON_FLAG)
{
   
  if(( ePMWakeupStatus == ePMSTS_NON)||(ePMWakeupStatus == ePMSTS_POWERGPIO_ACT || ePMWakeupStatus == ePMSTS_MCCS01_ACT || ePMWakeupStatus == ePMSTS_SAR_ACT||ePMWakeupStatus==ePMSTS_KEYWAKEUP_ACT))
  {
     //Set_HPD_Enable_Flag(); 
     APPSystem_ActivePort_For_PowerOn();
  }
  else
  {
     //Clr_HPD_Enable_Flag(); 
     APPSystem_ActivePort_For_PowerSaving();	
  }
     APPInput_Init();
}
}
#if 0
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
#endif

//////////////////////////////////////////////////////////////////
// Modulation Freq: 0~30KHz (freqMod -0~30), Range: 0~1% (range -0~10) //
//////////////////////////////////////////////////////////////////
void mStar_SetDDRSSCEnable( Bool bEnable )
{
    drvmStar_printData("DDRSSC Enable[%x]",bEnable);
    msWriteByteMask(REG_101129, bEnable?(BIT7|BIT6):BIT7, BIT7|BIT6);
#if (CHIP_ID==MST9U2)||(CHIP_ID==MST9U3)
    msWriteByteMask(REG_1011A9, bEnable?(BIT7|BIT6):BIT7, BIT7|BIT6);
#endif
}

//////////////////////////////////////////////////////////////////
// Modulation Freq: 0~30KHz (freqMod -0~30), Range: 0~1% (range -0~10) //
// Note: MIU1 setting is based on  MIU0 set value                                         //
//////////////////////////////////////////////////////////////////
void mStar_SetDDRSSCFactor( BYTE freqMod, BYTE range )
{
    static DWORD xdata dwFactorBK=0;
    DWORD xdata dwFactor;
    WORD xdata wStep;
    WORD xdata wSpan;
    DWORD xdata u32MPLL_MHZ = 432ul;

    if( (freqMod == 0) || (range == 0) )
    {
        dwFactorBK=0;
        msWrite2ByteMask(REG_101128, 0x00, 0x3FF);
        msWrite2ByteMask(REG_10112A, 0x00, 0x3FFF);
#if (CHIP_ID==MST9U2)||(CHIP_ID==MST9U3)
        msWrite2ByteMask(REG_1011A8, 0x00, 0x3FF);
        msWrite2ByteMask(REG_1011AA, 0x00, 0x3FFF);
#endif
        return;
    }

    // freqMod: 0~30 means 0~30K
    if(freqMod > DDR_SSC_MODULATION_MAX)
        freqMod = DDR_SSC_MODULATION_MAX;

    // range: 0~10 means 0~1%
    if(range > DDR_SSC_PERCENTAGE_MAX)
        range = DDR_SSC_PERCENTAGE_MAX;

    dwFactor = msRead3Byte(REG_101130); // MIU1 setting is based on  MIU0 set value

    if(dwFactorBK == dwFactor)
    {
        return;
    }

    dwFactorBK = dwFactor;

    wSpan = (((float)u32MPLL_MHZ*1000ul/freqMod) * 131072ul) / dwFactor;
    wStep = (((float)range*dwFactor+(DWORD)500*wSpan)/((DWORD)1000*wSpan));

    //drvmStar_printData("DDRSSC SPAN[%x]",wSpan);
    //drvmStar_printData("DDRSSC STEP[%x]",wStep);

    if (wStep > 0x3FF)
        wStep = 0x3FF;

    if(wSpan > 0x3FFF)
        wSpan = 0x3FFF;

    msWrite2ByteMask(REG_101128, wStep, 0x3FF);
    msWrite2ByteMask(REG_10112A, wSpan, 0x3FFF);
#if (CHIP_ID==MST9U2)||(CHIP_ID==MST9U3)
    msWrite2ByteMask(REG_1011A8, wStep, 0x3FF);
    msWrite2ByteMask(REG_1011AA, wSpan, 0x3FFF);
#endif

}

//////////////////////////////////////////////////////////////////
// Modulation Freq: 0~40KHz (freqMod -0~40), Range: 0~3% (range -0~30) //
//////////////////////////////////////////////////////////////////
void mStar_SetPanelSSCEnable( Bool bEnable )
{
    drvmStar_printData("PanelSSC Enable[%x]",bEnable);
    msWriteBit(REG_LPLL_1B, bEnable, BIT3); // ssc enable
}

//////////////////////////////////////////////////////////////////
// Modulation Freq: 0~40KHz (freqMod -0~40), Range: 0~3% (range -0~30) //
//////////////////////////////////////////////////////////////////
void mStar_SetPanelSSCFactor( BYTE freqMod, BYTE range )
{
    static DWORD xdata dwFactorBK=0;
    DWORD xdata dwFactor;
    WORD xdata wStep;
    WORD xdata wSpan;
    DWORD xdata u32MPLL_MHZ = 432ul;

    if( (freqMod == 0) || (range == 0) )
    {
        dwFactorBK=0;
        msWrite2ByteMask(REG_LPLL_2E, 0, 0xFFF);
        msWrite2ByteMask(REG_LPLL_30, 0, 0x3FFF);
        return;
    }

    // freqMod: 0~40 means 0~40K
    if(freqMod > PANEL_SSC_MODULATION_MAX)
        freqMod = PANEL_SSC_MODULATION_MAX;

    // range: 0~30 means 0~3%
    if(range > PANEL_SSC_PERCENTAGE_MAX)
        range = PANEL_SSC_PERCENTAGE_MAX;

    dwFactor = msReadByte(REG_LPLL_20);
    dwFactor <<= 16;
    dwFactor |= msRead2Byte(REG_LPLL_1E);

    if(dwFactorBK == dwFactor)
    {
        return;
    }

    dwFactorBK = dwFactor;

    wSpan = (((float)u32MPLL_MHZ*1024ul/freqMod) * 131072ul) / dwFactor;
    wStep = (float)(dwFactor*range/10)/wSpan/100; // 120320 coding modified

    //drvmStar_printData("PanelSSC SPAN[%x]",wSpan);
    //drvmStar_printData("PanelSSC STEP[%x]",wStep);

    if (wStep > 0xFFF)
        wStep = 0xFFF;

    if(wSpan > 0x3FFF)
        wSpan = 0x3FFF;

    msWrite2ByteMask(REG_LPLL_2E, wStep, 0xFFF);
    msWrite2ByteMask(REG_LPLL_30, wSpan, 0x3FFF);
}

#if 0// defined(_ENABLE_LPLL_FINETUNE_)
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

#if CHIP_ID==MST9U||CHIP_ID==MST9U2||CHIP_ID==MST9U3
#if( USE_MOD_HW_CAL )
// MOD hardware current calibration
// @pre MOD is not in power-down (BK1_26[7]==0)
// @pre LPLL is not in power-down
void msDrvIGenTuning( void )
{
#ifndef MST9U_FPGA
    BYTE XDATA  i, u8Status=FALSE;
    //drvmStar_printMsg(" HW msDrvIGenTuning Start      !!");
#if !PANEL_VBY1()
    msWriteByteMask(REG_MOD1_E3,  BIT4,  BIT6|BIT5|BIT4);                //[14:12]:reg_5us_cnt (can fine tune for cal time)
    msWriteByteMask(REG_MOD1_E3,  BIT2|BIT0,  BIT3|BIT2|BIT1|BIT0);      //[11:8]:reg_cal_tog_time (can fine tune for cal time)
    //#if(I_GEN_CH==CH2)
    msWriteByteMask(REG_MOD1_7C,  I_GEN_CH,  BIT4|BIT3|BIT2);            //[4:2]:Select calibration source pair (can select cal pair,Ch2,6,14,20,30,34,42,47)
    //#else //CH6
    //msWriteByteMask(REG_MOD1_7C,  BIT2,  BIT4|BIT3|BIT2);            //[4:2]:Select calibration source pair (can select cal pair,Ch2,6,14,20,30,34,42,47)
    //#endif
    msWriteByteMask(REG_MOD1_7C,   LVDS_VOLTAGE_TARGET,  BIT1|BIT0); //[1:0]:reg_gcr_cal_level(LVDS = 250mv)
    msWriteByteMask(REG_MOD1_7C,   BIT7,  BIT7);                         //[7]:reg_gcr_cal_en(Cal enable)
    msWriteByteMask(REG_MOD1_E1,   BIT7,  BIT7);                             //[15]:reg_keep_cal_value(Cal trigger)
    TimeOutCounter = 50;
    while (! ( msReadByte( REG_MOD1_E1 ) & BIT6 ) && TimeOutCounter );

    if(msReadByte( REG_MOD1_E1 ) & BIT5 )
    {
        i = LVDS_FAIL_TARGET;
    }
    else
    {
        i = msReadByte(REG_MOD1_E0);

    #ifdef ENABLE_UP_LOW_BOUNDARY_PROTECTION
        if (i >ICON_Upper_Boundary || i<ICON_Lower_Boundary)
        {
            i = LVDS_FAIL_TARGET;
        }
        else
    #endif
        {
            u8Status=TRUE;
        }
    }
    msWriteByteMask(REG_MOD1_7C,   0x00,  BIT7);                         //[7]:reg_gcr_cal_en(Cal disable)
    //msWriteByteMask(REG_MOD1_E1,   0x00,  BIT7);                           //[15]:reg_keep_cal_value(Cal trigger)

#else
    {
#define VBY1_VOLTAGE_TARGET 400
        BYTE u8EfuseData = msEread_GetDataFromEfuse(EFUSE_0, 0x133);

        if(u8EfuseData&BIT7)  //Trimming flag
        {
            u8EfuseData = (u8EfuseData & 0x3F);
            msWriteByteMask(REG_MOD1_51, BIT0, BIT0); // Turn on additional 3.7mA

            if(u8EfuseData<26) // 26 = 60*0.5 - 4(56 is efuse typical value)
            {
                drvmStar_printMsg("Efuse value is out of range");
                drvmStar_printMsg("Restore the typical Icon value");
                i = (((DWORD)60)*VBY1_VOLTAGE_TARGET)/300 - 41 + 1; // If Efuse data is not reasonable, set the typical value
            }
            else
                i = ((4+(DWORD)u8EfuseData)*VBY1_VOLTAGE_TARGET)/300 - 41 + 1;

            if(( i > 0x3F )||( i & BIT7 )) // i>63 or i<0
            {
                drvmStar_printMsg("Target value is not correct, please check");
                msWriteByteMask(REG_MOD1_51, 0x00, BIT0);
                i = 0x3F;
            }
        }
        else
        {
            msWriteByteMask(REG_MOD1_51, 0x00, BIT0);
            i = 0x3F;
        }
        u8Status=TRUE;
    }
#endif

    msWrite2Byte(REG_MOD1_80, ((WORD)i<<12)|(i<<6)| i); // ch0~ch2
    msWrite2Byte(REG_MOD1_82, ((WORD)i<<14)|(i<<8)|(i<<2)| (i>>4)); // ch2~ch5
    msWrite2Byte(REG_MOD1_84, ((WORD)i<<10)|(i<<4)| (i>>2)); // ch5~ch7

    msWrite2Byte(REG_MOD1_86, ((WORD)i<<12)|(i<<6)| i); // ch8~ch10
    msWrite2Byte(REG_MOD1_88, ((WORD)i<<14)|(i<<8)|(i<<2)| (i>>4)); // ch10~ch13
    msWrite2Byte(REG_MOD1_8A, ((WORD)i<<10)|(i<<4)| (i>>2)); // ch13~ch15

    msWrite2Byte(REG_MOD1_8C, ((WORD)i<<12)|(i<<6)| i); // ch16~ch18
    msWrite2Byte(REG_MOD1_8E, ((WORD)i<<14)|(i<<8)|(i<<2)| (i>>4)); // ch18~ch21
    msWrite2Byte(REG_MOD1_90, ((WORD)i<<10)|(i<<4)| (i>>2)); // ch21~ch23

    msWriteByte(REG_MOD2_8A, i); // ch24
    msWrite2Byte(REG_MOD2_8C, ((WORD)i<<12)| (i<<6)); // ch25~ch26
    msWrite2Byte(REG_MOD2_8E, ((WORD)i<<14)|(i<<8)|(i<<2)| (i>>4)); // ch26~ch29
    msWrite2Byte(REG_MOD2_90, ((WORD)i<<10)|(i<<4)| (i>>2)); // ch29~ch31

    msWrite2Byte(REG_MOD2_92, ((WORD)i<<12)|(i<<6)| i); // ch32~ch34
    msWrite2Byte(REG_MOD2_94, ((WORD)i<<14)|(i<<8)|(i<<2)| (i>>4)); // ch34~ch37
    msWrite2Byte(REG_MOD2_96, ((WORD)i<<10)|(i<<4)| (i>>2)); // ch37~ch39

    msWrite2Byte(REG_MOD2_98, ((WORD)i<<12)|(i<<6)| i); // ch40~ch42
    msWrite2Byte(REG_MOD2_9A, ((WORD)i<<14)|(i<<8)|(i<<2)| (i>>4)); // ch42~ch45
    msWrite2Byte(REG_MOD2_9C, ((WORD)i<<10)|(i<<4)| (i>>2)); // ch45~ch47

    if (u8Status==TRUE)
    {
        drvmStar_printMsg("HW MOD Cali Pass !!");
    }
    else
    {
        drvmStar_printMsg("HW MOD Cali Fail !!");
    }

    drvmStar_printData("I_gen_tuning = %x",i);
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
    volatile BYTE reg_140642   = msReadByte(REG_MOD1_42);
    volatile WORD reg_140694   = msRead2Byte(REG_MOD1_94);
    volatile WORD reg_140696   = msRead2Byte(REG_MOD1_96);
    volatile BYTE reg_14060B   = msReadByte(REG_MOD1_0B);
    //volatile BYTE reg_308A   = msReadByte(REG_308A);
    //volatile BYTE reg_308B   = _msReadByte(REG_308B);
    volatile WORD reg_140666   = msRead2Byte(REG_MOD1_66); // ch double swing
    volatile BYTE reg_140660   = msReadByte(REG_MOD1_60);

    #define SWING_MASK  0x3F
    drvmStar_printMsg("FW I_Gen_Tuning_Start");

    msWriteByteMask(REG_MOD1_E3, 0, BIT7); // Auto update Cal result to control swing reg in HW mode, SW mode needs set to 0 (for FT use)

    // 1.Disable power down mod
    msWriteByteMask(REG_MOD1_50,   0x00, BIT0);
    // ((// 2.Enable MOD self current bias.))
    //_msWriteByteMask(REG_MOD1_7C,   BIT6, BIT6);
    // 3.Set output bias current.
    //_msWriteByteMask(REG_MOD1_7C,   0x00, 0x3F);
    // 4.Disable double differential output swing
    msWrite2Byte(REG_MOD1_66,   0x00);
    // 5.select calibration source.
    //Note:
    //if BK36_FBh[1]=0, set BK36_DAh[5:4]=2'b01, BK36_46[1:0] = 2'b00, BK_36_E2[2] = 1'b0, and BK36_EEh[3:0]=4'hF to make PADA_OUT[PN]_CH[2] as LVDS output.
    //if BK36_FBh[1]=1, set BK36_DBh[7:6]=2'b01, BK36_46[1:0] = 2'b00, BK_36_E2[7] = 1'b0, and BK36_EEh[3:0]=4'hF to make PADA_OUT[PN]_CH[7] as LVDS output.
    #if(I_GEN_CH==CH2)
    msWrite2ByteMask(REG_MOD1_81,   0x00, 0x03F0); // ch2 control swing
    msWriteByteMask(REG_MOD1_7C,   0x00, BIT4|BIT3|BIT2);
    msWrite2ByteMask(REG_MOD1_94,  0x00, BIT8| BIT7|BIT6);
    msWriteByteMask(REG_MOD1_42,   0x00, BIT2);
    msWriteByteMask(REG_MOD1_4E,   0x3F, 0x3F);
    #else
    msWrite2ByteMask(REG_MOD1_84,   0x00, 0x03F0); // ch6 control swing
    msWriteByteMask(REG_MOD1_7C,   BIT2, BIT4|BIT3|BIT2);
    msWrite2ByteMask(REG_MOD1_96,  0, BIT4| BIT3|BIT2);
    msWriteByteMask(REG_MOD1_42,   0, BIT6);
    msWriteByteMask(REG_MOD1_4E,   0x3F, 0x3F);
    #endif
    // 6.Enable LVDS Test Mode, and make output data=High for LVDS output.
    msWriteByteMask(REG_MOD1_60,   BIT0, BIT0);
    msWriteByteMask(REG_MOD1_0B,   BIT7, BIT7);
    msWriteByteMask(REG_MOD1_41,   BIT6, BIT6);
    //_msWriteByteMask(REG_308B,   0x00, BIT1|BIT0);
    msWriteByteMask(REG_MOD1_04,   0xFF, 0xFF);
    // 7.Enable MOD Calibration.
    msWriteByteMask(REG_MOD1_7C,   BIT7, BIT7);
    // 8.Used to set LVDS voltage target.
    msWriteByteMask(REG_MOD1_7C,   LVDS_VOLTAGE_TARGET,  BIT1|BIT0);
    //9.Adjust the value of BK36_FAh[5:0] step by step to control output swing and check whether BK36_FBh[0] is equal to "1" or "0".
    for (i=0; i<=SWING_MASK; i++)
    {
        #if(I_GEN_CH==CH2)
        msWrite2ByteMask(REG_MOD1_81, i<<4, 0x03F0); // ch2 control swing
        #else
        msWrite2ByteMask(REG_MOD1_84, i<<4, 0x03F0); // ch6 control swing
        #endif

        ForceDelay1ms(10);
#if !PANEL_VBY1()
        //10.If BK36_FBh[0] is toggling between "1" and "0", terminal the fine-tune procedure.
        if (msReadByte(REG_MOD1_7D)&BIT0)
#else
        i = 0x3F;
#endif
        {
            msWrite2Byte(REG_MOD1_80, ((WORD)i<<12)|(i<<6)| i); // ch0~ch2
            msWrite2Byte(REG_MOD1_82, ((WORD)i<<14)|(i<<8)|(i<<2)| (i>>4)); // ch2~ch5
            msWrite2Byte(REG_MOD1_84, ((WORD)i<<10)|(i<<4)| (i>>2)); // ch5~ch7

            msWrite2Byte(REG_MOD1_86, ((WORD)i<<12)|(i<<6)| i); // ch8~ch10
            msWrite2Byte(REG_MOD1_88, ((WORD)i<<14)|(i<<8)|(i<<2)| (i>>4)); // ch10~ch13
            msWrite2Byte(REG_MOD1_8A, ((WORD)i<<10)|(i<<4)| (i>>2)); // ch13~ch15

            msWrite2Byte(REG_MOD1_8C, ((WORD)i<<12)|(i<<6)| i); // ch16~ch18
            msWrite2Byte(REG_MOD1_8E, ((WORD)i<<14)|(i<<8)|(i<<2)| (i>>4)); // ch18~ch21
            msWrite2Byte(REG_MOD1_90, ((WORD)i<<10)|(i<<4)| (i>>2)); // ch21~ch23

            msWriteByte(REG_MOD2_8A, i); // ch24
            msWrite2Byte(REG_MOD2_8C, ((WORD)i<<12)| (i<<6)); // ch25~ch26
            msWrite2Byte(REG_MOD2_8E, ((WORD)i<<14)|(i<<8)|(i<<2)| (i>>4)); // ch26~ch29
            msWrite2Byte(REG_MOD2_90, ((WORD)i<<10)|(i<<4)| (i>>2)); // ch29~ch31

            msWrite2Byte(REG_MOD2_92, ((WORD)i<<12)|(i<<6)| i); // ch32~ch34
            msWrite2Byte(REG_MOD2_94, ((WORD)i<<14)|(i<<8)|(i<<2)| (i>>4)); // ch34~ch37
            msWrite2Byte(REG_MOD2_96, ((WORD)i<<10)|(i<<4)| (i>>2)); // ch37~ch39

            msWrite2Byte(REG_MOD2_98, ((WORD)i<<12)|(i<<6)| i); // ch40~ch42
            msWrite2Byte(REG_MOD2_9A, ((WORD)i<<14)|(i<<8)|(i<<2)| (i>>4)); // ch42~ch45
            msWrite2Byte(REG_MOD2_9C, ((WORD)i<<10)|(i<<4)| (i>>2)); // ch45~ch47
            drvmStar_printMsg("FW MOD Cali Pass !!");
            drvmStar_printData("Cali result=0x%x", i);
            break;
        }
    }

    if(!(msReadByte(REG_MOD1_7D)&BIT0))
        drvmStar_printMsg("FW MOD Cali Fail !!");

    // 11.Disable MOD Calibration.
    msWriteByteMask(REG_MOD1_7C,   0x00, BIT7);
    // 12.Set register "BK36_DAh[5:4], BK36_DBh[15:14], BK36_E2[2], BK36_E2[7], BK36_46[5:4], BK36_47[7:6], BK36_80[1], BK36_8A[6], BK36_8B[1:0]" back to the original normal function.
    msWriteByte(REG_MOD1_42, reg_140642);   /* restore original value of BK0_AA */
    msWrite2Byte(REG_MOD1_94, reg_140694);   /* restore original value of BK0_AB */
    msWrite2Byte(REG_MOD1_96, reg_140696);   /* restore original value of BK0_08 */
    msWriteByte(REG_MOD1_0B, reg_14060B);   /* restore original value of BK0_09 */
    //_msWriteByte(REG_308A, reg_308A);   /* restore original value of BK0_AA */
    //_msWriteByte(REG_308B, reg_308B);   /* restore original value of BK0_AB */
    msWrite2Byte(REG_MOD1_66, reg_140666);
    msWriteByte(REG_MOD1_60, reg_140660);
    drvmStar_printMsg("FW I_Gen_Tuning_End");

    iGenTuningFinished=1;
}

#endif  // end of #if( USE_MOD_HW_CAL )
#else  //MST9U4
void msDrvIGenTuning( void )
{
#ifndef MST9U_FPGA
    BYTE XDATA  i, u8Status=FALSE;
    BYTE XDATA  j;
    #if !PANEL_VBY1()//LVDS
    {
        //TBD:Efuse address for LVDS
        BYTE u8EfuseData = msEread_GetDataFromEfuse(EFUSE_0, 0x133);
        if(u8EfuseData&BIT7)  //Trimming flag
        {
            u8EfuseData = (u8EfuseData & 0x7F);
            if ((u8EfuseData>110)||(u8EfuseData<60))
            {
                drvmStar_printMsg("Efuse value is out of range");
                drvmStar_printMsg("Restore the typical Icon value");
                i = 103; // If Efuse data is not reasonable, set the typical value
                u8Status=FALSE;
            }
            else
            {
                i = u8EfuseData;
                u8Status=TRUE;
            }
            if(( i > 0x7F )||( i & BIT7 )) // i>63 or i<0
            {
                drvmStar_printMsg("Target value is not correct, please check");
                i = 103;
                u8Status=FALSE;
            }
        }
        else
        {
            i = 103;
            u8Status=FALSE;
        }
        //Set DC level(Bias con)
        msWriteByteMask(REG_MOD2_22, i, 0x7F); // [6:0]bias current control(bias current control)

        //Calculate icon setting for LVDS Swing
        i = (((DWORD)LVDS_VOLTAGE_TARGET-60)*10/75);
        for (j = 0; j < 16; j++)
            msWrite2ByteMask(REG_MOD2_60+(j*2), (((WORD)i<<8)&0x7F00)|(i&0x7F), 0x7F7F); // [6:0]Control swing of channel X (Icon)
    }
#else //VBY1
{
        #define VBY1_VOLTAGE_TARGET VBY1_SWING_850
        //TBD:Efuse address for LVDS
        BYTE u8EfuseData = msEread_GetDataFromEfuse(EFUSE_0, 0x133);

        if(u8EfuseData&BIT7)  //Trimming flag
        {
            u8EfuseData = (u8EfuseData & 0x3F);
            if ((u8EfuseData>40)||(u8EfuseData<7))
            {
                drvmStar_printMsg("Efuse value is out of range");
                drvmStar_printMsg("Restore the typical Icon value");
                i = 18; // If Efuse data is not reasonable, set the typical value
                u8Status=FALSE;
            }
            else
            {
                i = u8EfuseData;
                u8Status=TRUE;
            }
            if(( i > 0x3F )||( i & BIT7 )) // i>63 or i<0
            {
                drvmStar_printMsg("Target value is not correct, please check");
                i = 18;
                u8Status=FALSE;
            }
        }
        else
        {
            i = 18;
            u8Status=FALSE;
        }
        //Set RCON current
        for (j = 0; j < 16; j++)
            msWrite2ByteMask(REG_MOD2_80+(j*2), (((WORD)i<<8)&0x3F00)|(i&0x3F), 0x3F3F); // [5:0]reg_gcr_rcon_chxx(Control swing of channel xx)
        //Set VBY1 SWING
        i=VBY1_SWING_850_DIV2;
        for (j = 0; j < 8; j++)
            msWrite2ByteMask(REG_MOD2_A0+(j*4), ((WORD)i<<12)|(i<<8)|(i<<4)|(i<<0), 0xFFFF); // [3:0]vby1 vreg channel X
    }
#endif
    if (u8Status==TRUE)
    {
        drvmStar_printMsg("HW MOD Cali Pass !!");
    }
    else
    {
        drvmStar_printMsg("HW MOD Cali Fail !!");
    }

    drvmStar_printData("I_gen_tuning = %x",i);
    iGenTuningFinished=1;
#endif
}

#endif

#if CHIP_ID==MST9U||CHIP_ID==MST9U2||CHIP_ID==MST9U3
void msDrvPowerModCtrl(BYTE ucSwitch)
{
    #if CHIP_ID == MST9U3
        msWriteByteMask(REG_MOD1_51,BIT5,BIT5);  // reg_test_mod
        ForceDelay1ms(1);
    #elif CHIP_ID == MST9U2
        msWriteByteMask(REG_MOD1_51,BIT2,BIT2);  // reg_test_mod
        ForceDelay1ms(1);
    #else
    #endif

    if (ucSwitch)
    {
#if ENABLE_SCALER_LRSWAP
        msWrite2Byte(REG_MFT_20,0x8000);  // L R SWAP
#endif
        msWriteByteMask(REG_MOD1_4E,0x3F,0x3F);
        msWriteByteMask(REG_MOD1_50,0,BIT0);
        msWrite2Byte(REG_MOD1_66, LVDS_XSWING_CH); // ch double swing

#if PANEL_LVDS_1CH
        msWrite2Byte(REG_MOD1_20, 0x0555);
#elif PANEL_LVDS_2CH
        msWrite2Byte(REG_MOD1_00, 0x0201);
        msWrite2Byte(REG_MFT_02, 0x0010);// 1ch lvds mux
        if(msDrvISSCDualPort())
        {
            msWriteByteMask(REG_MOD1_79, 0x00, BIT5);
            msWrite2Byte(REG_MFT_02, 0x8001);
            msWriteByteMask(REG_MOD1_01, 0x00, BIT1);
        }
        else
        {
            msWriteByteMask(REG_MOD1_79, BIT5, BIT5);
            msWrite2Byte(REG_MFT_02, 0x0010);
            msWriteByteMask(REG_MOD1_01, BIT1, BIT1);
        }
        //msWrite2Byte(REG_MOD1_20, 0x5555);
        //msWrite2Byte(REG_MOD1_22, 0x0055);
        msWrite2Byte(REG_MOD1_20, (((LVDS_2CH_B_OUT&0x0F)<<12)|LVDS_2CH_A_OUT));
        msWrite2Byte(REG_MOD1_22, (((LVDS_2CH_C_OUT&0xFF)<<8)|(LVDS_2CH_B_OUT>>4)));
        msWrite2Byte(REG_MOD1_24, ((LVDS_2CH_D_OUT<<4)|(LVDS_2CH_C_OUT>>8)));
        msWrite2Byte(REG_MOD2_82, (((LVDS_2CH_F_OUT&0x0F)<<12)|LVDS_2CH_E_OUT));
        msWrite2Byte(REG_MOD2_84, (((LVDS_2CH_G_OUT&0xFF)<<8)|(LVDS_2CH_F_OUT>>4)));
        msWrite2Byte(REG_MOD2_86, ((LVDS_2CH_H_OUT<<4)|(LVDS_2CH_G_OUT>>8)));
#elif PANEL_LVDS_4CH
        if(msDrvISSCDualPort())
        {
            msWriteByteMask(REG_MOD1_79, 0x00, BIT5);

        }
        else
        {
            msWriteByteMask(REG_MOD1_79, BIT5, BIT5);
        }
        msWriteByteMask(REG_MFT_02, BIT0, BIT0);

        msWrite2Byte(REG_MOD1_00, 0x0401);
        msWrite2Byte(REG_MOD1_78, 0x0600);
        msWrite2Byte(REG_MOD1_20, 0x5555);
        msWrite2Byte(REG_MOD1_22, 0x5555);
        msWrite2Byte(REG_MOD1_24, 0x5555);
#elif PANEL_LVDS_8CH
        msWriteByteMask(REG_MFT_02, BIT0, BIT0);
        msWrite2Byte(REG_MOD1_20, 0x5555);
        msWrite2Byte(REG_MOD1_22, 0x5555);
        msWrite2Byte(REG_MOD1_24, 0x5555);
        msWrite2Byte(REG_MOD2_82, 0x5555);
        msWrite2Byte(REG_MOD2_84, 0x5555);
        msWrite2Byte(REG_MOD2_86, 0x5555);
#elif PANEL_VBY1_1CH_8Bit||PANEL_VBY1_1CH_10Bit
        msWrite2Byte(REG_MOD1_24, 0x0200);
#elif PANEL_VBY1_2CH_8Bit||PANEL_VBY1_2CH_10Bit
        msWrite2Byte(REG_MOD1_24, 0x0500);
#elif PANEL_VBY1_4CH_8Bit||PANEL_VBY1_4CH_10Bit
        msWrite2Byte(REG_MFT_02, 0x0001);
        msWrite2Byte(REG_MOD1_00, 0x0401);
#if (CHIP_ID == MST9U3)
        msWrite2Byte(REG_MOD1_22, (0x01<<2*VBY1_PHY_LANE0)|(0x01<<2*VBY1_PHY_LANE1)|(0x01<<2*VBY1_PHY_LANE2)|(0x01<<2*VBY1_PHY_LANE3));
#else
        msWrite2Byte(REG_MOD1_24, (((0x01<<2*VBY1_PHY_LANE0)|(0x01<<2*VBY1_PHY_LANE1)|(0x01<<2*VBY1_PHY_LANE2)|(0x01<<2*VBY1_PHY_LANE3)) & 0x00FF) << 8);	// lane 0~3
		msWrite2Byte(REG_MOD2_82, (((0x01<<2*VBY1_PHY_LANE0)|(0x01<<2*VBY1_PHY_LANE1)|(0x01<<2*VBY1_PHY_LANE2)|(0x01<<2*VBY1_PHY_LANE3)) & 0xFF00) >> 8);	// lane 4~7
#endif
#elif PANEL_VBY1_8CH_8Bit||PANEL_VBY1_8CH_10Bit
#if (CHIP_ID == MST9U3)
        msWrite2Byte(REG_MOD1_22, 0x5555);
#else
        msWrite2Byte(REG_MOD1_24, 0x5500);
        msWrite2Byte(REG_MOD2_82, 0x0055);
#endif
#elif PANEL_EDP
        msWrite2Byte(REG_MFT_02, 0x0001);
        msWrite2Byte(REG_MOD1_20, 0x0000);
        msWrite2Byte(REG_MOD1_22, 0x0000);
        msWrite2Byte(REG_MOD1_24, 0x0000);
#endif
        msDrvMFTSetup();
    }
    else
    {
#if (CHIP_ID==MST9U2)
        msWriteByteMask(REG_MOD1_51, BIT2, BIT2);
#endif
        msWrite2Byte(REG_MOD1_20, 0x0000);
        msWrite2Byte(REG_MOD1_22, 0x0000);
        msWrite2Byte(REG_MOD1_24, 0x0000);
        msWrite2Byte(REG_MOD2_82, 0x0000);
        msWrite2Byte(REG_MOD2_84, 0x0000);
        msWrite2Byte(REG_MOD2_86, 0x0000);
        msWriteByteMask(REG_MOD1_50,BIT0,BIT0);
        msWriteByteMask(REG_MOD1_4E,0x00,0x07);
#if (CHIP_ID==MST9U2)
        msWriteByteMask(REG_MOD1_51, 0x00, BIT2);
#endif
    }
    #if CHIP_ID == MST9U3
        msWriteByteMask(REG_MOD1_51,0x00,BIT5);  // reg_test_mod
        ForceDelay1ms(1);
    #elif CHIP_ID == MST9U2
        msWriteByteMask(REG_MOD1_51,0x00,BIT2);  // reg_test_mod
        ForceDelay1ms(1);
    #else
    #endif
}
#else //MST9U4
void msDrvPowerModCtrl(BYTE ucSwitch)
{
    if (ucSwitch)
    {
#if ENABLE_SCALER_LRSWAP  //TBD
        msWrite2Byte(REG_MFT_20,0x8000);  // L R SWAP
#endif
        msWriteByteMask(REG_MOD2_02,BIT1,BIT1|BIT0); //[1]reg_gcr_en_reg,[0]reg_pd_mod
        msWrite2Byte(REG_MOD2_18, LVDS_XSWING_CH); // ch double swing

#if PANEL_LVDS_1CH
#elif PANEL_LVDS_2CH
        msWrite2ByteMask(REG_MOD2_30, LVDS_2CH_A_OUT ,0xFFFF);
        msWrite2ByteMask(REG_MOD2_32, LVDS_2CH_B_OUT ,0xFFFF);
        msWrite2ByteMask(REG_MOD2_34, LVDS_2CH_C_OUT ,0xFFFF);
        msWrite2ByteMask(REG_MOD2_36, LVDS_2CH_D_OUT ,0xFFFF);
        msWrite2ByteMask(REG_MOD2_00, 0x03 ,0x1F); //REG_EN_CLK_BANK

#elif PANEL_LVDS_4CH
        //msWrite2Byte(REG_MOD1_78, 0x0600);
        msWrite2Byte(REG_MOD2_30, 0x5555);
        msWrite2Byte(REG_MOD2_32, 0x5555);
        msWrite2Byte(REG_MOD2_34, 0x5555);
        msWrite2Byte(REG_MOD2_36, 0x0000);
        msWrite2ByteMask(REG_MOD2_00, 0x0F ,0x1F); //REG_EN_CLK_BANK

#elif PANEL_LVDS_8CH
#elif PANEL_VBY1_1CH_8Bit||PANEL_VBY1_1CH_10Bit
#elif PANEL_VBY1_2CH_8Bit||PANEL_VBY1_2CH_10Bit
        msWrite2Byte(REG_MOD2_30, 0x0005);
        msWrite2Byte(REG_MOD2_32, 0x0000);
        msWrite2Byte(REG_MOD2_34, 0x0000);
        msWrite2Byte(REG_MOD2_36, 0x0000);
        msWrite2ByteMask(REG_MOD2_00, 0x01 ,0x1F); //REG_EN_CLK_BANK

#elif PANEL_VBY1_4CH_8Bit||PANEL_VBY1_4CH_10Bit
        msWrite2Byte(REG_MOD2_30, 0x0055);
        msWrite2Byte(REG_MOD2_32, 0x0000);
        msWrite2Byte(REG_MOD2_34, 0x0000);
        msWrite2Byte(REG_MOD2_36, 0x0000);
        msWrite2ByteMask(REG_MOD2_00, 0x01 ,0x1F); //REG_EN_CLK_BANK

#elif PANEL_VBY1_8CH_8Bit||PANEL_VBY1_8CH_10Bit
        msWrite2Byte(REG_MOD2_30, 0x5555);
        msWrite2Byte(REG_MOD2_32, 0x0000);
        msWrite2Byte(REG_MOD2_34, 0x0000);
        msWrite2Byte(REG_MOD2_36, 0x0000);
        msWrite2ByteMask(REG_MOD2_00, 0x03 ,0x1F); //REG_EN_CLK_BANK

#elif PANEL_VBY1_16CH_8Bit||PANEL_VBY1_16CH_10Bit
        msWrite2Byte(REG_MOD2_30, 0x5555);
        msWrite2Byte(REG_MOD2_32, 0x5555);
        msWrite2Byte(REG_MOD2_34, 0x0000);
        msWrite2Byte(REG_MOD2_36, 0x0000);
        msWrite2ByteMask(REG_MOD2_00, 0x07 ,0x1F); //REG_EN_CLK_BANK

#elif PANEL_VBY1_20CH_8Bit||PANEL_VBY1_20CH_10Bit
        msWrite2Byte(REG_MOD2_30, 0x5555);
        msWrite2Byte(REG_MOD2_32, 0x5555);
        msWrite2Byte(REG_MOD2_34, 0x0055);
        msWrite2Byte(REG_MOD2_36, 0x0000);
        msWrite2ByteMask(REG_MOD2_00, 0x07 ,0x1F); //REG_EN_CLK_BANK

#elif PANEL_VBY1_32CH_8Bit||PANEL_VBY1_32CH_10Bit
        msWrite2Byte(REG_MOD2_30, 0x5555);
        msWrite2Byte(REG_MOD2_32, 0x5555);
        msWrite2Byte(REG_MOD2_34, 0x5555);
        msWrite2Byte(REG_MOD2_36, 0x5555);
        msWrite2ByteMask(REG_MOD2_00, 0x1F ,0x1F); //REG_EN_CLK_BANK

#elif PANEL_EDP
#endif
        msDrvMFTSetup();
    }
    else
    {
        msWrite2Byte(REG_MOD2_30, 0x0000);
        msWrite2Byte(REG_MOD2_32, 0x0000);
        msWrite2Byte(REG_MOD2_34, 0x0000);
        msWrite2Byte(REG_MOD2_36, 0x0000);
        msWriteByteMask(REG_MOD2_02,BIT0,BIT1|BIT0); //reg_pd_mod
        msWrite2ByteMask(REG_MOD2_00, 0x00 ,0x1F); //REG_EN_CLK_BANK

    }
}
#endif
BYTE mStar_ScalerDoubleBuffer(Bool u8Enable)
{
    u8Enable=FALSE;
    return u8Enable;
}

#if CHIP_ID==MST9U4
void msDrvSetDClkPLL(DWORD u32ODCLK_KHZ)
{
    BYTE uctemp = 0, ucLPLL_Type=0;
    DWORD u32Factor, u32MPLL_KHZ;
    bit bOutut8PFlag = 0;

    if (msDrvISSCDualPort())
    {
        u32ODCLK_KHZ/=8;
        bOutut8PFlag=TRUE;
    }
    else
    {
        u32ODCLK_KHZ/=4;
        bOutut8PFlag=FALSE;
    }
    u32MPLL_KHZ = 432000UL;  // 429.54MHz

#if PANEL_TTL
    if( u32ODCLK_KHZ <= 25*1000UL )    // 0MHz~25MHz
    {
        ucLPLL_Type = E_PNL_SUPPORTED_LPLL_TTL_4P_25to25MHz;
    }
    else if( u32ODCLK_KHZ <= 50*1000UL )    // 25MHz~50MHz
    {
        ucLPLL_Type = E_PNL_SUPPORTED_LPLL_TTL_4P_25to50MHz;
    }
    else if( u32ODCLK_KHZ <= 75*1000UL )    // 50MHz~100MHz
    {
        ucLPLL_Type = E_PNL_SUPPORTED_LPLL_TTL_4P_50to75MHz;
    }
    else    // 75MHz~150MHz
    {
        ucLPLL_Type = E_PNL_SUPPORTED_LPLL_TTL_4P_75to150MHz;
    }
#elif PANEL_LVDS_1CH
    if( u32ODCLK_KHZ <= 100*1000UL )    // 0MHz~100MHz
    {
        ucLPLL_Type = E_PNL_SUPPORTED_LPLL_HS_LVDS_1ch_4P_100to100MHz;
    }
    else    //100~200Mhz
    {
        ucLPLL_Type = E_PNL_SUPPORTED_LPLL_HS_LVDS_1ch_4P_100to200MHz;
    }
#elif PANEL_LVDS_2CH
    if( u32ODCLK_KHZ <= 375*100UL )    // 0MHz~37.5MHz
    {
        ucLPLL_Type = E_PNL_SUPPORTED_LPLL_LVDS_2ch_37_5to37_5MHz;
    }
    else if( u32ODCLK_KHZ <= 100*1000UL )    // 37.5MHz~50MHz
    {
        ucLPLL_Type = E_PNL_SUPPORTED_LPLL_LVDS_2ch_37_5to50MHz;
    }
    else if( u32ODCLK_KHZ <= 100*1000UL )    // 50MHz~75MHz
    {
        ucLPLL_Type = E_PNL_SUPPORTED_LPLL_LVDS_2ch_50to75MHz;
    }
    else if( u32ODCLK_KHZ <= 100*1000UL )    // 75MHz~100MHz
    {
        ucLPLL_Type = E_PNL_SUPPORTED_LPLL_HS_LVDS_2ch_4P_100to100MHz;
    }
    else    //100~200Mhz
    {
        ucLPLL_Type = E_PNL_SUPPORTED_LPLL_HS_LVDS_2ch_4P_100to200MHz;
    }
#elif PANEL_LVDS_4CH
    if( u32ODCLK_KHZ <= 375*100UL )    // 0MHz~37.5MHz
    {
        ucLPLL_Type = E_PNL_SUPPORTED_LPLL_LVDS_4ch_37_5to37_5MHz;
    }
    else if( u32ODCLK_KHZ <= 100*1000UL )    // 37.5MHz~50MHz
    {
        ucLPLL_Type = E_PNL_SUPPORTED_LPLL_LVDS_4ch_37_5to50MHz;
    }
    else if( u32ODCLK_KHZ <= 100*1000UL )    // 50MHz~75MHz
    {
        ucLPLL_Type = E_PNL_SUPPORTED_LPLL_LVDS_4ch_50to75MHz;
    }
    else if( u32ODCLK_KHZ <= 100*1000UL )    // 75MHz~100MHz
    {
        ucLPLL_Type = E_PNL_SUPPORTED_LPLL_HS_LVDS_4ch_4P_100to100MHz;
    }
    else    //100~200Mhz
    {
        ucLPLL_Type = E_PNL_SUPPORTED_LPLL_HS_LVDS_4ch_4P_100to200MHz;
    }
#elif PANEL_LVDS_8CH //8CH
#elif PANEL_VBY1_1CH_10Bit
#elif PANEL_VBY1_1CH_8Bit
#elif PANEL_VBY1_2CH_10Bit
    if( u32ODCLK_KHZ <= 375*100UL )    // 0MHz~37.5MHz
    {
        ucLPLL_Type = E_PNL_SUPPORTED_LPLL_VBY1_2ch_10bit_4P_37_5to37_5MHz;
    }
    else    //37.5~75Mhz
    {
        ucLPLL_Type = E_PNL_SUPPORTED_LPLL_VBY1_2ch_10bit_4P_37_5to75MHz;
    }
#elif PANEL_VBY1_2CH_8Bit
    if( u32ODCLK_KHZ <= 50*1000UL )    // 0MHz~50MHz
    {
        ucLPLL_Type = E_PNL_SUPPORTED_LPLL_VBY1_2ch_8bit_4P_50to50MHz;
    }
    else    //50~100Mhz
    {
        ucLPLL_Type = E_PNL_SUPPORTED_LPLL_VBY1_2ch_8bit_4P_50to100MHz;
    }
#elif PANEL_VBY1_4CH_10Bit
    if( u32ODCLK_KHZ <= 375*100UL )    // 0MHz~37.5MHz
    {
        ucLPLL_Type = E_PNL_SUPPORTED_LPLL_VBY1_4ch_10bit_4P_37_5to37_5MHz;
    }
    else    //37.5~75Mhz
    {
        ucLPLL_Type = E_PNL_SUPPORTED_LPLL_VBY1_4ch_10bit_4P_37_5to75MHz;
    }
#elif PANEL_VBY1_4CH_8Bit
    if( u32ODCLK_KHZ <= 50*1000UL )    // 0MHz~50MHz
    {
        ucLPLL_Type = E_PNL_SUPPORTED_LPLL_VBY1_4ch_8bit_4P_50to50MHz;
    }
    else    //50~100Mhz
    {
        ucLPLL_Type = E_PNL_SUPPORTED_LPLL_VBY1_4ch_8bit_4P_50to100MHz;
    }
#elif PANEL_VBY1_8CH_10Bit
    if( u32ODCLK_KHZ <= 75*1000UL )    // 0MHz~75MHz
    {
        ucLPLL_Type = E_PNL_SUPPORTED_LPLL_VBY1_8ch_10bit_4P_75to75MHz;
    }
    else    //75~150Mhz
    {
        ucLPLL_Type = E_PNL_SUPPORTED_LPLL_VBY1_8ch_10bit_4P_75to150MHz;
    }
#elif PANEL_VBY1_8CH_8Bit
    if( u32ODCLK_KHZ <= 100*1000UL )    // 0MHz~100MHz
    {
        ucLPLL_Type = E_PNL_SUPPORTED_LPLL_VBY1_8ch_8bit_4P_100to100MHz;
    }
    else    //100~200Mhz
    {
        ucLPLL_Type = E_PNL_SUPPORTED_LPLL_VBY1_8ch_8bit_4P_100to200MHz;
    }
#elif PANEL_VBY1_16CH_10Bit
    if (bOutut8PFlag==TRUE)
    {
        if( u32ODCLK_KHZ <= 9788*10UL )    // 0MHz~97.88MHz
        {
            ucLPLL_Type = E_PNL_SUPPORTED_LPLL_VBY1_16ch_10bit_8P_97_88to97_88MHz;
        }
        else if( u32ODCLK_KHZ <= 135*1000UL )     //97.88~135Mhz
        {
            ucLPLL_Type = E_PNL_SUPPORTED_LPLL_VBY1_16ch_10bit_8P_97_88to135MHz;
        }
        else  //135~195.75Mhz
        {
            ucLPLL_Type = E_PNL_SUPPORTED_LPLL_VBY1_16ch_10bit_8P_135to195_75MHz;
        }
    }
    else
    {
        if( u32ODCLK_KHZ <= 150*1000UL )    // 0MHz~150MHz
        {
            ucLPLL_Type = E_PNL_SUPPORTED_LPLL_VBY1_16ch_10bit_HighSpeed_4P_150to150MHz;
        }
        else //150~300Mhz
        {
            ucLPLL_Type = E_PNL_SUPPORTED_LPLL_VBY1_16ch_10bit_HighSpeed_4P_150to300MHz;
        }
    }
#elif PANEL_VBY1_16CH_8Bit
    if (bOutut8PFlag==TRUE)
    {
        if( u32ODCLK_KHZ <= 9788*10UL )    // 0MHz~97.88MHz
        {
            ucLPLL_Type = E_PNL_SUPPORTED_LPLL_VBY1_16ch_8bit_8P_97_88to97_88MHz;
        }
        else if( u32ODCLK_KHZ <= 120*1000UL )   //97.88~120Mhz
        {
            ucLPLL_Type = E_PNL_SUPPORTED_LPLL_VBY1_16ch_8bit_8P_97_88to120MHz;
        }
        else         //120~195.75Mhz
        {
            ucLPLL_Type = E_PNL_SUPPORTED_LPLL_VBY1_16ch_8bit_8P_120to195_75MHz;
        }
    }
    else
    {
        if( u32ODCLK_KHZ <= 150*1000UL )    // 0MHz~150MHz
        {
            ucLPLL_Type = E_PNL_SUPPORTED_LPLL_VBY1_16ch_8bit_HighSpeed_4P_150to150MHz;
        }
        else if( u32ODCLK_KHZ <= 225*1000UL )    // 150MHz~225MHz
        {
            ucLPLL_Type = E_PNL_SUPPORTED_LPLL_VBY1_16ch_8bit_HighSpeed_4P_150to225MHz;
        }
        else    //225~300Mhz
        {
            ucLPLL_Type = E_PNL_SUPPORTED_LPLL_VBY1_16ch_8bit_HighSpeed_4P_150to150MHz;
        }
    }
#elif PANEL_VBY1_20CH_10Bit
    if( u32ODCLK_KHZ <= 19575*10UL )    // 0MHz~195.75MHz
    {
        ucLPLL_Type = E_PNL_SUPPORTED_LPLL_VBY1_20ch_10bit_8P_195_75to195_75MHz;
    }
    else    // 195.75MHz~391.5MHz
    {
        ucLPLL_Type = E_PNL_SUPPORTED_LPLL_VBY1_20ch_10bit_8P_195_75to391_5MHz;
    }
#elif PANEL_VBY1_20CH_8Bit
    if( u32ODCLK_KHZ <= 19575*10UL )    // 0MHz~195.75MHz
    {
        ucLPLL_Type = E_PNL_SUPPORTED_LPLL_VBY1_20ch_8bit_8P_195_75to195_75MHz;
    }
    else if( u32ODCLK_KHZ <= 250*1000UL )    // 195.75~250MHz
    {
        ucLPLL_Type = E_PNL_SUPPORTED_LPLL_VBY1_20ch_8bit_8P_195_75to250MHz;
    }
    else    // 250MHz~391.5MHz
    {
        ucLPLL_Type = E_PNL_SUPPORTED_LPLL_VBY1_20ch_8bit_8P_250to391_5MHz;
    }
#elif PANEL_VBY1_32CH_10Bit
    if( u32ODCLK_KHZ <= 150*1000UL )    // 0MHz~150MHz
    {
        ucLPLL_Type = E_PNL_SUPPORTED_LPLL_VBY1_32ch_10bit_8P_150to150MHz;
    }
    else if( u32ODCLK_KHZ <= 225*1000UL )    // 150~225MHz
    {
        ucLPLL_Type = E_PNL_SUPPORTED_LPLL_VBY1_32ch_10bit_8P_150to225MHz;
    }
    else    // 225~300MHz
    {
        ucLPLL_Type = E_PNL_SUPPORTED_LPLL_VBY1_32ch_10bit_8P_225to300MHz;
    }
#elif PANEL_VBY1_32CH_8Bit
    if( u32ODCLK_KHZ <= 150*1000UL )    // 0MHz~150MHz
    {
        ucLPLL_Type = E_PNL_SUPPORTED_LPLL_VBY1_32ch_8bit_8P_150to150MHz;
    }
    else if( u32ODCLK_KHZ <= 225*1000UL )    // 150~225MHz
    {
        ucLPLL_Type = E_PNL_SUPPORTED_LPLL_VBY1_32ch_8bit_8P_150to225MHz;
    }
    else    // 225~300MHz
    {
        ucLPLL_Type = E_PNL_SUPPORTED_LPLL_VBY1_32ch_8bit_8P_225to300MHz;
    }
#endif

    u8LPLL_LOOP_GAIN = u8LoopGain[ucLPLL_Type];
    fLPLL_LOOP_DIV = u8LoopDiv[ucLPLL_Type];

#ifdef MST9U_FPGA
    if(PanelWidth==800&&PanelHeight==600)
        u32Factor = (float)(0x6C0000);//For 800x600 panel
    else
        u32Factor = (float)(0xA10000);//For 640x480 panel
    u8LPLL_LOOP_GAIN = u8LPLL_LOOP_GAIN;
    fLPLL_LOOP_DIV = fLPLL_LOOP_DIV;
    u32ODCLK_KHZ = u32ODCLK_KHZ;
    ucLPLL_Type = ucLPLL_Type;
    uctemp = uctemp;
#else
    u32Factor = (float)((float)u32MPLL_KHZ/fLPLL_LOOP_DIV)*((float)524288UL*(float)u8LPLL_LOOP_GAIN/u32ODCLK_KHZ);
    for(uctemp = 0; uctemp <LPLL_REG_NUM; uctemp++)
    {
        msWrite2ByteMask((BANK_SC3+(LPLLSettingTBL[ucLPLL_Type][uctemp].address)*2), LPLLSettingTBL[ucLPLL_Type][uctemp].value, LPLLSettingTBL[ucLPLL_Type][uctemp].mask);
    }
#endif
    msWriteBit(REG_LPLL_18,FALSE,_BIT3); // lock input //20081110 Frame pll enable
    msWrite3Byte(REG_LPLL_1E,u32Factor);//SET Value

#if FPLL_TUNE_SW_MODE
#if PANEL_VBY1()
    #if CHIP_ID!=MST9U4
    msWriteByteMask(REG_MOD1_60, 0x80, 0x80);  //need update
    #endif
#endif
    {
        DWORD u32LimitFreqPhaseCorrection;

        u32LimitFreqPhaseCorrection = u32Factor / FPLL_TUNE_LIMIT_HW;
        msWrite3Byte(REG_LPLL_0C, u32LimitFreqPhaseCorrection);
        msWrite3Byte(REG_LPLL_10, u32LimitFreqPhaseCorrection);
        g_u32FPLLTuneMax = u32Factor * (1000 + FPLL_TUNE_LIMIT_SW) / 1000;
        g_u32FPLLTuneMin = u32Factor * (1000 - FPLL_TUNE_LIMIT_SW) / 1000;
    }
#else
#if PANEL_VBY1()
    {
        DWORD u32LimitFreqPhaseCorrection;

        u32LimitFreqPhaseCorrection = u32Factor / 250;// Limit in 0.4%
        msWrite3Byte(REG_LPLL_0C, u32LimitFreqPhaseCorrection);
        msWrite3Byte(REG_LPLL_10, u32LimitFreqPhaseCorrection);
        #if CHIP_ID!=MST9U4
        msWriteByteMask(REG_MOD1_60, 0x80, 0x80);
        #endif
    }
#elif PANEL_EDP
#endif
#endif
}
#else
void msDrvSetDClkPLL(DWORD u32ODCLK_KHZ)
{
    BYTE uctemp = 0, ucLPLL_Type=0;
    DWORD u32Factor, u32MPLL_KHZ;
	
printf("hhhhhhhhh u32ODCLK_KHZ==%x",u32ODCLK_KHZ);

    if( msDrvISSCDualPort() )
        u32ODCLK_KHZ/=2;
    u32MPLL_KHZ = 432000UL;  // 429.54MHz
#if PANEL_TTL
    if( u32ODCLK_KHZ <= 40*1000UL )    // 25MHz~40MHz
    {
        ucLPLL_Type = E_PNL_SUPPORTED_LPLL_TTL_25to40MHz;
        u8LPLL_LOOP_GAIN = 16;
        fLPLL_LOOP_DIV = 56;
    }
    else if( u32ODCLK_KHZ <= 75*1000UL )    // 40MHz~75MHz
    {
        ucLPLL_Type = E_PNL_SUPPORTED_LPLL_TTL_40to75MHz;
        u8LPLL_LOOP_GAIN = 16;
        fLPLL_LOOP_DIV = 28;
    }
    else    // 75MHz~150MHz
    {
        ucLPLL_Type = E_PNL_SUPPORTED_LPLL_TTL_75to150MHz;
        u8LPLL_LOOP_GAIN = 16;
        fLPLL_LOOP_DIV = 14;
    }
#elif PANEL_LVDS_1CH
    ucLPLL_Type = E_PNL_SUPPORTED_LPLL_LVDS_1ch_1P_75to150MHz;
    u8LPLL_LOOP_GAIN = 16;
    fLPLL_LOOP_DIV = 14;
#elif PANEL_LVDS_2CH
    if( !msDrvISSCDualPort() )
    {
        ucLPLL_Type = E_PNL_SUPPORTED_LPLL_LVDS_2ch_1P_150to300MHz;
        u8LPLL_LOOP_GAIN = 16;
        fLPLL_LOOP_DIV = 7;
    }
    else
    {
#if (CHIP_ID == MST9U3)
        ucLPLL_Type = E_PNL_SUPPORTED_LPLL_LVDS_2ch_2P_75to75MHz;
#else
        ucLPLL_Type = E_PNL_SUPPORTED_LPLL_LVDS_2ch_2P_75to150MHz;      // same as 2ch_2P_150to300M
#endif
        u8LPLL_LOOP_GAIN = 16;
        fLPLL_LOOP_DIV = 14;
    }
    //ucLPLL_Type = E_PNL_SUPPORTED_LPLL_LVDS_2ch_2P_75to150MHz;

#elif PANEL_LVDS_4CH
    ucLPLL_Type = E_PNL_SUPPORTED_LPLL_LVDS_4ch_2P_150to300MHz;
    u8LPLL_LOOP_GAIN = 16;
    fLPLL_LOOP_DIV = 7;
#elif PANEL_LVDS_8CH //8CH
    ucLPLL_Type = E_PNL_SUPPORTED_LPLL_LVDS_8ch_2P_150to300MHz;
    u8LPLL_LOOP_GAIN = 16;
    fLPLL_LOOP_DIV         = 7;
#elif PANEL_VBY1_1CH_10Bit
    ucLPLL_Type = E_PNL_SUPPORTED_LPLL_VBY1_1ch_10bit_1P_37_5to75MHz;
    u8LPLL_LOOP_GAIN = 20;
    fLPLL_LOOP_DIV = 40;
#elif PANEL_VBY1_1CH_8Bit
    ucLPLL_Type = E_PNL_SUPPORTED_LPLL_VBY1_1ch_8bit_1P_37_5to75MHz;
    u8LPLL_LOOP_GAIN = 20;
    fLPLL_LOOP_DIV = 30;
#elif PANEL_VBY1_2CH_10Bit
    ucLPLL_Type = E_PNL_SUPPORTED_LPLL_VBY1_2ch_10bit_1P_75to150MHz;
    u8LPLL_LOOP_GAIN = 20;
    fLPLL_LOOP_DIV = 20;
#elif PANEL_VBY1_2CH_8Bit
    ucLPLL_Type = E_PNL_SUPPORTED_LPLL_VBY1_2ch_8bit_1P_75to150MHz;
    u8LPLL_LOOP_GAIN = 20;
    fLPLL_LOOP_DIV = 15;
#elif PANEL_VBY1_4CH_10Bit
    ucLPLL_Type = E_PNL_SUPPORTED_LPLL_VBY1_4ch_10bit_2P_75to150MHz;
    u8LPLL_LOOP_GAIN = 20;
    fLPLL_LOOP_DIV = 20;
#elif PANEL_VBY1_4CH_8Bit
    ucLPLL_Type = E_PNL_SUPPORTED_LPLL_VBY1_4ch_8bit_2P_75to150MHz;
    u8LPLL_LOOP_GAIN = 20;
    fLPLL_LOOP_DIV = 15;
#elif PANEL_VBY1_8CH_10Bit
    ucLPLL_Type = E_PNL_SUPPORTED_LPLL_VBY1_8ch_10bit_2P_150to300MHz;
    u8LPLL_LOOP_GAIN = 20;
    fLPLL_LOOP_DIV = 10;
#elif PANEL_VBY1_8CH_8Bit
    ucLPLL_Type = E_PNL_SUPPORTED_LPLL_VBY1_8ch_8bit_2P_150to300MHz;
    u8LPLL_LOOP_GAIN = 20;
    fLPLL_LOOP_DIV = 7.5;
#elif PANEL_EDP
    if( u32ODCLK_KHZ <= 20*1000UL )    // 12.5MHz~20MHz
    {
        ucLPLL_Type = E_PNL_SUPPORTED_LPLL_eDP_High_bit_rate_12_5to20MHz;
        u8LPLL_LOOP_GAIN = 16;
        fLPLL_LOOP_DIV = 112;
    }
    else if( u32ODCLK_KHZ <= 375*100UL ) // 20MHz~37.5MHz
    {
        ucLPLL_Type = E_PNL_SUPPORTED_LPLL_eDP_High_bit_rate_20to37_5MHz;
        u8LPLL_LOOP_GAIN = 16;
        fLPLL_LOOP_DIV = 56;
    }
    else if( u32ODCLK_KHZ <= 75*1000UL ) // 37.5MHz~75MHz
    {
        ucLPLL_Type = E_PNL_SUPPORTED_LPLL_eDP_High_bit_rate_37_5to75MHz;
        u8LPLL_LOOP_GAIN = 16;
        fLPLL_LOOP_DIV = 28;
    }
    else if( u32ODCLK_KHZ <= 150*1000UL ) // 75MHz~150MHz
    {
        ucLPLL_Type = E_PNL_SUPPORTED_LPLL_eDP_High_bit_rate_75to150MHz;
        u8LPLL_LOOP_GAIN = 16;
        fLPLL_LOOP_DIV = 14;
    }
    else // 150MHz~300MHz
    {
        ucLPLL_Type = E_PNL_SUPPORTED_LPLL_eDP_High_bit_rate_150to300MHz;
        u8LPLL_LOOP_GAIN = 16;
        fLPLL_LOOP_DIV = 7;
    }
#endif
#if CHIP_ID == MST9U3
   {
    u8LPLL_LOOP_GAIN = u8LoopGain[ucLPLL_Type];
    fLPLL_LOOP_DIV = u8LoopDiv[ucLPLL_Type];
   }
#endif
    //mStar_SetIVS();

#ifdef MST9U_FPGA
    if(PanelWidth==800&&PanelHeight==600)
        u32Factor = (float)(0x6C0000);//For 800x600 panel
    else
        u32Factor = (float)(0xA10000);//For 640x480 panel
    //tmp u32i_gain = 0x3;
    //tmp u32p_gain = 0x4;
    u8LPLL_LOOP_GAIN = u8LPLL_LOOP_GAIN;
    fLPLL_LOOP_DIV = fLPLL_LOOP_DIV;
    u32ODCLK_KHZ = u32ODCLK_KHZ;
    ucLPLL_Type = ucLPLL_Type;
    uctemp = uctemp;
#else
    u32Factor = (float)((float)u32MPLL_KHZ/fLPLL_LOOP_DIV)*((float)524288UL*(float)u8LPLL_LOOP_GAIN/u32ODCLK_KHZ);


        printf("RRRRR ============u32MPLL_KHZ==%d \r\n",u32MPLL_KHZ);
        printf("RRRRR ============fLPLL_LOOP_DIV==%d \r\n",(WORD)fLPLL_LOOP_DIV);
        printf("RRRRR ============u8LPLL_LOOP_GAIN==%d \r\n",u8LPLL_LOOP_GAIN);
        printf("RRRRR ============u32ODCLK_KHZ==%d \r\n",u32ODCLK_KHZ);
        printf("RRRRR ============u32Factor==%x \r\n",u32Factor);
    for(uctemp = 0; uctemp <LPLL_REG_NUM; uctemp++)
    {
        msWrite2ByteMask((BANK_SC3+(LPLLSettingTBL[ucLPLL_Type][uctemp].address)*2), LPLLSettingTBL[ucLPLL_Type][uctemp].value, LPLLSettingTBL[ucLPLL_Type][uctemp].mask);
    }
#endif
    msWriteBit(REG_LPLL_18,FALSE,_BIT3); // lock input //20081110 Frame pll enable

        printf("============u32Factor==%x \r\n",u32Factor);

#if (PanelType == PanelM350DVR01_01||PanelType==PanelM350DVA01_1)
if(u8FrameLockModeIndex==FLM_FB_FLOCK||u8FrameLockModeIndex==FLM_FB_NEW_LOCK)
 {
  if((abs(InputWidth-800)<10)&&(abs(InputHeight-600)<10)&&(abs(InputVfreq-750)<10))
  {
        u32Factor = 0x486043;//for DP 800x600 75FreeSync On NG
  }


}
#elif (PanelType == PanelLSM315DP01||PanelType == PanelLSM270DP01)
if(u8FrameLockModeIndex==FLM_FB_FLOCK||u8FrameLockModeIndex==FLM_FB_NEW_LOCK)
{
  
  if((abs(InputWidth-800)<10)&&(abs(InputHeight-600)<10)&&(abs(InputVfreq-600)<10))
  {
        u32Factor = 0x1F817C;//for 800x600 60 FreeSync On NG
  }

}
#elif (PanelType == PanelLM270WR2)
  if((abs(InputWidth-800)<10)&&(abs(InputHeight-600)<10)&&(abs(0x2051AF-u32Factor)<0x100))
  {
        u32Factor = 0x1D7847;//for 800x600 60 FreeSync On NG
  }
  else if((abs(InputWidth-2560)<10)&&(abs(InputHeight-1440)<10)&&(abs(InputVfreq-1000)<20))
  {
        u32Factor = 0x1D8847;//for 2560x1440 100
  }
#elif(PanelType==PanelLSM270HP06)
printData("u8FrameLockModeIndex==%x \r\n",u8FrameLockModeIndex);
if(u8FrameLockModeIndex==FLM_FB_FLOCK||u8FrameLockModeIndex==FLM_FB_NEW_LOCK)
{
  
  if((abs(InputWidth-1920)<10)&&(abs(InputHeight-1080)<10)&&(abs(InputVfreq-1440)<10))
  {
        u32Factor = 0x2BEB2B;//for 800x600 60 FreeSync On NG
  }

}
#endif

    msWrite3Byte(REG_LPLL_1E,u32Factor);//SET Value

#if FPLL_TUNE_SW_MODE
#if PANEL_VBY1()
    #if CHIP_ID!=MST9U4
    msWriteByteMask(REG_MOD1_60, 0x80, 0x80);
    #endif
#endif
    {
        DWORD u32LimitFreqPhaseCorrection;

        u32LimitFreqPhaseCorrection = u32Factor / FPLL_TUNE_LIMIT_HW;
        msWrite3Byte(REG_LPLL_0C, u32LimitFreqPhaseCorrection);
        msWrite3Byte(REG_LPLL_10, u32LimitFreqPhaseCorrection);
        g_u32FPLLTuneMax = u32Factor * (1000 + FPLL_TUNE_LIMIT_SW) / 1000;
        g_u32FPLLTuneMin = u32Factor * (1000 - FPLL_TUNE_LIMIT_SW) / 1000;
    }
#else
#if PANEL_VBY1()
    {
        DWORD u32LimitFreqPhaseCorrection;

        u32LimitFreqPhaseCorrection = u32Factor / 250;// Limit in 0.4%
        msWrite3Byte(REG_LPLL_0C, u32LimitFreqPhaseCorrection);
        msWrite3Byte(REG_LPLL_10, u32LimitFreqPhaseCorrection);
        #if CHIP_ID!=MST9U4
        msWriteByteMask(REG_MOD1_60, 0x80, 0x80);
        #endif
    }
#elif PANEL_EDP
    {
        DWORD u32LimitFreqPhaseCorrection;

        u32LimitFreqPhaseCorrection = u32Factor / 1000;// Limit in 0.1%
        msWrite3Byte(REG_LPLL_0C, u32LimitFreqPhaseCorrection);
        msWrite3Byte(REG_LPLL_10, u32LimitFreqPhaseCorrection);
        #if CHIP_ID!=MST9U4
        msWriteByteMask(REG_MOD1_60, 0x80, 0x80);
        #endif
    }
#endif
#endif
}
#endif
void msDrvResetOutputHtt(void)
{
    // can't support in FRC mode
    DWORD u32Ivs;
    DWORD u32Ovs;
    WORD u16Htt;
    WORD u16Div = (msDrvISSCDualPort() ? 2 : 1);

    u16Htt = PanelHTotal / u16Div - 1;

    msDrvSCCommonWrite(SCALER_MASK_MAIN, ENABLE);
    msWriteByte(SCL0_00_02, 0x05);
    msWrite2ByteMask(SCL0_10_18, u16Htt, 0x0FFF);
    msDrvSCCommonWrite(SCALER_MASK_MAIN, DISABLE);

    msWrite3Byte(REG_LPLL_1E, msRead3Byte(REG_LPLL_1E));

    Delay1ms(50);

    u32Ivs = msRead3Byte(REG_LPLL_42);
    u32Ovs = msRead3Byte(REG_LPLL_46);

    if( (u32Ivs == 0) || (u32Ovs == 0) )
        return;

    u16Htt = (WORD)((DWORD)PanelHTotal * u32Ivs / u32Ovs);
    if( (u16Htt > PanelMaxHTotal) || (u16Htt < PanelMinHTotal) )
        return;

    u16Htt = (u16Htt/u16Div - 1) | BIT0;

    msDrvSCCommonWrite(SCALER_MASK_MAIN, ENABLE);
    msWrite2ByteMask(SCL0_10_18, u16Htt, 0x0FFF);
    msDrvSCCommonWrite(SCALER_MASK_MAIN, DISABLE);

    if( g_ScalerInfo[SCL0_MAIN].u8SiSyncMode == FLM_FB_FAST_LOCK )
        msDrvSetFpllFastLock(SCL0_MAIN);
    msWriteBit(REG_LPLL_18, TRUE, _BIT3); // lock input //20081110 Frame pll enable

    drvmStar_printData("msDrvResetOutputHtt:%x\n", u16Htt);
}

void msDrvResetDClkPLL(void)
{
    DWORD u32Factor, u32MPLL_MHZ, u32XTALCLK_MHZ, u32IvsPeriod, u32PreIvsPeriod = 0;
    BYTE u8OutputFrameDiv, u8InputFrameDiv;
    WORD u8TimeOutCnt;
    unsigned long long TempA, TempB;
    DWORD u32IvsPeriodMax, u32IvsPeriodMin;
    DWORD u32LimitFreqPhaseCorrection;
    BYTE u8VFreqIn = V_FREQ_IN(MapScaler2Port(SCL0_MAIN));

#if CHIP_ID==MST9U4
    u8InputFrameDiv = ((msReadByte(REG_LPLL_1C) & 0x0F)<<4)+ (msReadByte(REG_LPLL_19) & 0x0F) + 1;
    u8OutputFrameDiv= ((msReadByte(REG_LPLL_1C) & 0xF0))   +((msReadByte(REG_LPLL_19) & 0xF0)>>4 ) +  1;
#else
    u8InputFrameDiv = (msReadByte(REG_LPLL_19) & 0x0F) + ((msReadByte(REG_LPLL_1C) & BIT0)<<4) + 1;
    u8OutputFrameDiv = ((msReadByte(REG_LPLL_19) & 0xF0)>>4 ) + ((msReadByte(REG_LPLL_1C) & BIT1)<<3) + 1;
#endif
    u32XTALCLK_MHZ = 12UL;
    u32MPLL_MHZ = 432UL;  // 429.54MHz

    u32IvsPeriodMax = ((DWORD)CRYSTAL_CLOCK * u8OutputFrameDiv) / (GetVfreq()-1);// V freq:59~61
    u32IvsPeriodMin = ((DWORD)CRYSTAL_CLOCK * u8OutputFrameDiv) / (GetVfreq()+1);

    if(u8VFreqIn == 0)//To prevent divide by "0"
        return;

    Delay1ms(1000 * u8InputFrameDiv / u8VFreqIn);//wait IVS period stable

    u8TimeOutCnt = 600;
    while(u8TimeOutCnt!=0)
    {
        u8TimeOutCnt--;
        u32IvsPeriod = msRead3Byte(REG_LPLL_42);
        if((u32IvsPeriodMin<u32IvsPeriod)&&(u32IvsPeriod<u32IvsPeriodMax))
        {
            if(abs(u32PreIvsPeriod - u32IvsPeriod)<10)
            {
                break;
            }
            else
            {
                u32PreIvsPeriod = u32IvsPeriod;
            }
        }
        Delay1ms(5);

    }
    if(u8TimeOutCnt!=0)
    {
        TempA = (unsigned long long)u32IvsPeriod*u32MPLL_MHZ*524288*u8LPLL_LOOP_GAIN;
        TempB = (unsigned long long)u32XTALCLK_MHZ*PanelHTotal/(msDrvISSCDualPort()?2:1)*PanelVTotal*u8OutputFrameDiv*fLPLL_LOOP_DIV;
        u32Factor = TempA/TempB;
        msWrite3Byte(REG_LPLL_1E,u32Factor);//SET Value

#if PANEL_EDP
        u32LimitFreqPhaseCorrection = u32Factor / 500;// Limit in 0.2%
#else
        u32LimitFreqPhaseCorrection = u32Factor / 250;// Limit in 0.4%
#endif
        msWrite3Byte(REG_LPLL_0C, u32LimitFreqPhaseCorrection);
        msWrite3Byte(REG_LPLL_10, u32LimitFreqPhaseCorrection);
    }


}

void drvmStar_SetupFreeRunMode(BYTE SCDetWin)
{
    msDrvSetFrameSyncMode(FLM_NO_LOCK, SCDetWin);
    Set_FreeRunModeFlag(SCDetWin);
}

static void msDrvSetFpllSpreader( BYTE SCDetWin )
{
    BYTE InputDiv; // input divider
    BYTE OutputDiv; // output divider
    DWORD InputClkDiv; // input clock divider
    BYTE InputPort = MapScaler2Port(SCDetWin);
    WORD usDPHTotal = 0;
    WORD usDPVTotal = 0;

#if CHIP_ID==MST9U4
    InputDiv = ((msReadByte(REG_LPLL_1C) & 0x0F)<<4)+ (msReadByte(REG_LPLL_19) & 0x0F) + 1;
    OutputDiv= ((msReadByte(REG_LPLL_1C) & 0xF0))   +((msReadByte(REG_LPLL_19) & 0xF0)>>4 ) +  1;
#else
    InputDiv  = (msReadByte(REG_LPLL_19) & 0x0F) + ((msReadByte(REG_LPLL_1C) & BIT0)<<4) + 1;
    OutputDiv = ((msReadByte(REG_LPLL_19) & 0xF0)>>4 ) + ((msReadByte(REG_LPLL_1C) & BIT1)<<3) + 1;
#endif

    if ((SrcFlags(InputPort)&bInterlaceMode)&&!( INPUT_IS_DISPLAYPORT(InputPort))) // interlace
        OutputDiv *=2;
        //InputDiv /=2;

    if ( INPUT_IS_HDMI(InputPort) ) // hdmi pixel repetition
        InputDiv *=(msAPI_combo_IPGetPixelRepetitionInfo((EN_COMBO_IP_SELECT)g_InputPort[InputPort].eIPMux)+1);

    if ( INPUT_IS_DISPLAYPORT(InputPort))
    {
        msAPI_combo_IPGetDPHVInformation((EN_COMBO_IP_SELECT)g_InputPort[InputPort].eIPMux, &usDPHTotal, &usDPVTotal);

        InputClkDiv  = (DWORD) InputDiv * usDPHTotal * usDPVTotal;
        InputClkDiv /= (HBR2Flag(InputPort)?2:1);
    }
    else
    {
        InputClkDiv  = (DWORD) InputDiv  * ((msRead2Byte(SCL0_01_50) & 0x1FFF)+1) * ((msRead2Byte(SCL0_01_3E) & 0x1FFF));
    }

    InputClkDiv /= OutputDiv;
    msWriteByte(REG_LPLL_32, InputClkDiv);
    msWriteByte(REG_LPLL_33, InputClkDiv>>8);
    #if CHIP_ID==MST9U4
    msWrite2Byte(REG_LPLL_34,(InputClkDiv>>16)&0x1FF);
    #else
    msWrite2Byte(REG_LPLL_34,(InputClkDiv>>16)&0xFF);
    #endif
    msWriteByteMask(REG_LPLL_FD, BIT7, BIT7);//ivs spreader enable

}

static void msDrvSetFpllFastLock( BYTE SCDetWin )
{
    BYTE InputDiv; // input divider
    BYTE OutputDiv; // output divider
    DWORD InputClkDiv; // input clock divider
    DWORD OutputClkDiv; // output clock divider
    DWORD GCD=1; // GCD
    DWORD TempA; // GCD
    DWORD TempB; // GCD
    BYTE InputPort = MapScaler2Port(SCDetWin);
    WORD usDPHTotal = 0;
    WORD usDPVTotal = 0;
    DWORD u32SCRegBase = SCRegBase(SCDetWin);

    u32SCRegBase += ((SCDetWin&BIT0)?0x00:0x2000000);
#if CHIP_ID==MST9U4
    InputDiv = ((msReadByte(REG_LPLL_1C) & 0x0F)<<4)+ (msReadByte(REG_LPLL_19) & 0x0F) + 1;
    OutputDiv= ((msReadByte(REG_LPLL_1C) & 0xF0))   +((msReadByte(REG_LPLL_19) & 0xF0)>>4 ) +  1;
#else
    InputDiv  = (msReadByte(REG_LPLL_19) & 0x0F) + ((msReadByte(REG_LPLL_1C) & BIT0)<<4) + 1;
    OutputDiv = ((msReadByte(REG_LPLL_19) & 0xF0)>>4 ) + ((msReadByte(REG_LPLL_1C) & BIT1)<<3) + 1;
#endif

    if ((SrcFlags(InputPort)&bInterlaceMode)) // interlace
        OutputDiv *=2;
        //InputDiv /=2;

    if ( INPUT_IS_HDMI(InputPort) ) // hdmi pixel repetition
        InputDiv *=(msAPI_combo_IPGetPixelRepetitionInfo((EN_COMBO_IP_SELECT)g_InputPort[InputPort].eIPMux)+1);

    if ( INPUT_IS_DISPLAYPORT(InputPort))
    {
        msAPI_combo_IPGetDPHVInformation((EN_COMBO_IP_SELECT)g_InputPort[InputPort].eIPMux, &usDPHTotal, &usDPVTotal);

        InputClkDiv  = (DWORD) InputDiv * usDPHTotal * (msDrvIP1ReadVtotal(SCDetWin));
        InputClkDiv /= (HBR2Flag(InputPort)?2:1);

    }
    else
    {
        InputClkDiv  = (DWORD) InputDiv  * ((msDrvIP1ReadHtotal(SCDetWin))+1) * (msDrvIP1ReadVtotal(SCDetWin));
    }
    OutputClkDiv = (DWORD) OutputDiv * ((msRead2Byte(SCL0_10_18) & 0x0FFF)+1) * ((msRead2Byte(SCL0_10_1A) & 0x0FFF)+1);

    if((InputClkDiv == 0)||(OutputClkDiv == 0))//To prevent divide by "0"
        return;
    TempA = InputClkDiv;
    TempB = OutputClkDiv;

    TimeOutCounter = 250;
    while( (TempA !=0) && (TempB !=0) && TimeOutCounter) // GCD
    {
        if(TempA >= TempB)
        {
            GCD = TempB;
            TempA %= TempB;
        }
        else // Temp A < TempB
        {
            GCD = TempA;
            TempB %= TempA;
        }
    }

    InputClkDiv  /= GCD;
    OutputClkDiv /= GCD;

    if( (InputClkDiv<2000) || (OutputClkDiv<2000) )
    {
        if(InputClkDiv>=OutputClkDiv)
            TempA = 2000/OutputClkDiv +1;
        else
            TempA = 2000/InputClkDiv +1;

        InputClkDiv  *= TempA;
        OutputClkDiv *= TempA;
    }

    InputClkDiv  -= 1;
    OutputClkDiv -= 1;

    if( (InputClkDiv>>24) || (OutputClkDiv>>24) ) // overflow
    {
        msWriteByteMask(REG_LPLL_E1, 0, BIT7|BIT3);
    }
    else
    {
        msWriteByte(REG_LPLL_32, InputClkDiv);
        msWriteByte(REG_LPLL_33, InputClkDiv>>8);
        #if CHIP_ID==MST9U4
        msWrite2Byte(REG_LPLL_34,(InputClkDiv>>16)&0x1FF);
        #else
        msWrite2Byte(REG_LPLL_34,(InputClkDiv>>16)&0xFF);
        #endif
        msWriteByte(REG_LPLL_36, OutputClkDiv);
        msWriteByte(REG_LPLL_37, OutputClkDiv>>8);
        msWriteByte(REG_LPLL_38, OutputClkDiv>>16);
        //msWriteByteMask(REG_LPLL_E1, BIT7|BIT3, BIT7|BIT3);
        msWriteByteMask(REG_LPLL_E1, BIT7, BIT7);
    }
}

static BOOL msDrvSetOvs(BYTE SCDetWin)
{
    WORD u16InputHeight = 0, u16InputVtotal = 0, wOutputVtotal=0;
    BYTE u8InputDelay = 0, u8LockYLineDelay = 0;
    WORD u16OutputHeight = g_DisplayWindow[MapScaler2Win(SCDetWin)].sDwDisplayWin.height;
    WORD u16VStartOffset = g_DisplayWindow[MapScaler2Win(SCDetWin)].sDwDisplayWin.y;
    BYTE InputPort = MapScaler2Port(SCDetWin);
    WORD wK,wLockPoint = 0,wCVcntFreezePoint = 0;
    DWORD u32SCRegBase = SCRegBase(SCDetWin);

    u16InputHeight = g_ScalerInfo[SCDetWin].sSiIPCaptureWin.height/((SrcFlags(InputPort)&bInterlaceMode)?2:1);
    u16InputVtotal = (msDrvIP1ReadVtotal(SCDetWin))/((SrcFlags(InputPort)&bInterlaceMode)?2:1);
    u8LockYLineDelay = _Y_LOCK_LINE_*2;
    u8InputDelay = _DELAY_INPUT_LINE_;

    wOutputVtotal = msRead2Byte(u32SCRegBase+SC10_1A)&0xFFF;
    // To avoid timing change when set frame sync mode
    if((u16InputVtotal==0)||(wOutputVtotal==0))
    {
        drvmStar_printMsg("Output/Input Vtt is unstable, skip ovs setting");
        return FALSE;
    }

    // ovs position, vcnt value
    if(((DWORD)(u16InputHeight-u8InputDelay) * 1000/ u16InputVtotal) > (((DWORD)u16OutputHeight )*1000/wOutputVtotal))
        wK = (WORD)((int)wOutputVtotal - (((((int)u16InputHeight + u8LockYLineDelay) * wOutputVtotal) / u16InputVtotal) - (u16OutputHeight - _DELAY_DISP_TRIG_ - (6 * u16OutputHeight / u16InputHeight)  - 2)));
    else
    {
        if(((DWORD)(u16InputHeight) * 1000/ u16InputVtotal) > (((DWORD)u16OutputHeight )*1000/wOutputVtotal))
        {
            BYTE u8Temp;

            u8Temp = ( u16InputHeight * wOutputVtotal / u16InputVtotal ) - u16OutputHeight;
            wK = wOutputVtotal - _DELAY_DISP_TRIG_ - (WORD)(((DWORD)(u8InputDelay + u8LockYLineDelay) * wOutputVtotal) / u16InputVtotal) - u8Temp;
        }
        else
            wK = wOutputVtotal - _DELAY_DISP_TRIG_ - (WORD)(((DWORD)(u8InputDelay + u8LockYLineDelay) * wOutputVtotal) / u16InputVtotal);
    }
    wLockPoint = (wK + VERTICAL_DE_START + u16VStartOffset) % wOutputVtotal;
    wCVcntFreezePoint = (wK - 2 + VERTICAL_DE_START + u16VStartOffset) % wOutputVtotal;
            msWrite2Byte(SCL0_0F_AA,wLockPoint);
            msWrite2Byte(SCL0_0F_A8,wCVcntFreezePoint);
#if ENABLE_SW_FLOCK			
#if !PANEL_EDP
            msWriteByteMask(SCL0_0F_AC, _BIT0, _BIT0|BIT3);    //V count freeze will work when this register set from 0->1
            msWriteByteMask(SCL0_0F_AC, 0x00, _BIT0|BIT3);
#endif
#else
#if !PANEL_EDP
            msWriteBit(SCL0_0F_AC, TRUE, _BIT0);    //V count freeze will work when this register set from 0->1
            msWriteBit(SCL0_0F_AC, FALSE, _BIT0);
#endif
#endif

    drvmStar_printData("woutputVtotall = %d",wOutputVtotal);
    drvmStar_printData("ucLockPoint %x",wLockPoint);
    drvmStar_printData("ucCVcntFreezePoint %x",wCVcntFreezePoint);
    return TRUE;
}

BOOL msDrvGetFrameSyncBankDiffZero(void)
{
    #if 0//IS_VTOTAL_BY_TYPE
	//Because this panel 50~75hz vtotal is too large, so the clk is too large also, and scaler maybe can't latch the data 
	//So when input freq is between 50~75, it can't return true to delay 1 frame
	//So when input freq is between 50~75, it just delay >1 frame.
    //WORD u16InputVfreq = 0;
    //WORD hFreq = 0;
    //WORD vFreq = 0;
   // BYTE u8DetSCIndex = MapPort2DetScIndex(USER_PREF_INPUT_TYPE);

    // vFreq = msDrvIP1ReadVtotal(u8DetSCIndex);//_msRead2Byte(SC0_E2) & 0xFFF;
   // hFreq = HFREQ(msDrvIP1ReadHperiod(u8DetSCIndex));
    //vFreq = VFREQ(hFreq, msDrvIP1ReadVtotal(u8DetSCIndex));
    //u16InputVfreq = GetCurrentInputVfreq();//GetInputVfreq(DISPLAY_WIN1);//vFreq;
//printData("__xxxx Timingxxxxx_PanelVfreq =%d", u16InputVfreq);
    // u16InputVfreq=VFREQ( HFREQ( SrcHPeriod(MapWin2Port(USER_PREF_WIN_SEL)) ), SrcVTotal(MapWin2Port(USER_PREF_WIN_SEL)) );
	//if( (u16InputVfreq < (750+20)) && (u16InputVfreq > (500-10)) )
	//{
	//	printData("_444_xxxx50~75 Timingxxxxx_PanelVfreq =%d", u16InputVfreq);
	//	return FALSE;
	//}
	if(CheckPanelNormalTiming())
	{
		return FALSE;
	}
#endif


    if( ((g_ScalerInfo[SCL0_MAIN].u8SiSyncMode == FLM_FB_FLOCK)&&(g_DisplayWindow[DISPLAY_WIN1].sDwDisplayWin.height == PanelHeight))
     || (g_ScalerInfo[SCL0_MAIN].u8SiSyncMode == FLM_FB_NEW_LOCK) )
    	{
    	#if (PanelType == PanelLSM315HP01)
		drvmStar_printData("LowInputLag_Other__msDrvGetFrameSyncBankDiffZero = %d",0);
              return FALSE;
          #endif
        return TRUE;
    	}

	
    return FALSE;
}

void msDrvSetFrameSyncMode( FSyncMode ucFrameLockMode, BYTE SCDetWin )
{
    DWORD u32SCRegBase = SCRegBase(SCDetWin);
    DWORD u32SCIP1RegBase = SCIP1RegBase(SCDetWin);	
    BOOL bEnableFramePLLMode = 0;
    BOOL bEnableFreeRun = 0;
    BYTE ucLockYLine = 0, ucDelayLine =0x02;
    WORD wOutputVtotal = 0;
    WORD u16Htotal = 0;
    WORD wModeVtotal = 0,wK,wLockPoint = 0,wCVcntFreezePoint = 0;
    BYTE InputPort = MapScaler2Port(SCDetWin);
    BYTE bDisableAutoOVS = FALSE;
    BOOL bFRC;
    BOOL bSrcSync = TRUE;
	
     #if IS_VTOTAL_BY_TYPE
    DWORD u32OutputCLK;
    #endif

    if((g_ScalerInfo[SCDetWin].eSiDispStatus == SC_STATUS_IDLE)||(g_ScalerInfo[SCDetWin].eSiDispStatus == SC_STATUS_HOLLOW))
    {
        return;
    }
	
    u8FrameLockModeIndex=ucFrameLockMode;

    // auto vsync control for fb_new_lock & fb_flock
#if defined(_DISABLE_AUTO_OUTPUT_VSYNC_)
    bDisableAutoOVS = TRUE;
#endif
    msDrvSCCommonWrite(g_DisplayWindow[MapScaler2Win(SCDetWin)].u16DwScMaskOut, ENABLE);
    if( ucFrameLockMode == FLM_FB_NEW_LOCK )
        msWriteBit(SC10_21+u32SCRegBase, TRUE, _BIT7);
    else if((ucFrameLockMode == FLM_FB_FLOCK)||(ucFrameLockMode == FLM_FBL_FLOCK))
        msWriteBit(SC10_21+u32SCRegBase, FALSE, _BIT7);
    else
        msWriteBit(SC10_21+u32SCRegBase, bDisableAutoOVS, _BIT7);
    msDrvSCCommonWrite(g_DisplayWindow[MapScaler2Win(SCDetWin)].u16DwScMaskOut, DISABLE);

#if (!ENABLE_SW_FLOCK)
    msWriteBit(SCL0_01_07, TRUE, _BIT7); // Frame Lock Always ON
#endif

    if( SCDetWin == SCL0_MAIN )
    {
        bFRC = msDrvSetFrameDivider(V_FREQ_IN(MapScaler2Port(SCL0_MAIN)));
        if( bFRC && (ucFrameLockMode != FLM_NO_LOCK) && (ucFrameLockMode != FLM_FB_FLOCK) && (ucFrameLockMode != FLM_FBL_FLOCK)
#if INPUT_SAME_OUT
       &&(ucFrameLockMode != FLM_FB_FAST_LOCK)
#endif
	)
        {
            drvmStar_printData("\r\n[msSetFrameSyncMode] Override Mode From %d To FLM_NO_LOCK", ucFrameLockMode);
            ucFrameLockMode = FLM_NO_LOCK;
        }
    }

    if( (ucFrameLockMode == g_ScalerInfo[SCDetWin].u8SiSyncMode)
     && (g_ScalerInfo[SCDetWin].u8SiSyncMode != FLM_FB_FAST_LOCK) )
    {
        // avoid duplicate setting in same FLM mode
        return;
    }
    else
    {
        // retore DCLK if prev mode is FLM_FB_FLOCK or FLM_FBL_FLOCK
        if( (((g_ScalerInfo[SCDetWin].u8SiSyncMode == FLM_FB_FLOCK) && (ucFrameLockMode != FLM_FB_FLOCK))
         || ((g_ScalerInfo[SCDetWin].u8SiSyncMode == FLM_FBL_FLOCK) && (ucFrameLockMode != FLM_FBL_FLOCK)))
         && (SCDetWin == SCL0_MAIN) )
            msDrvSetDClkPLL((DWORD)PanelHTotal*PanelVTotal*GetVfreq()/1000);

        g_ScalerInfo[SCDetWin].u8SiSyncMode = ucFrameLockMode;
    }

    if(g_ScalerInfo[SCDetWin].u8SiFbMode)
        msWriteByteMask(SC12_02+u32SCRegBase, 0x00, _BIT7);
    else
        msWriteByteMask(SC12_02+u32SCRegBase, _BIT7, _BIT7);

    if(SCDetWin!=SCL0_MAIN)
        return;
	
    drvmStar_printData("\r\n[msSetFrameSyncMode] %d",ucFrameLockMode);

    //20090312_1 25Hz & 30Hz double frame rate
    msDrvSCCommonWrite(SCALER_MASK_MAIN, ENABLE);
    msWrite2Byte(SCL0_20_34, 0x07);
    msWrite2Byte(SCL0_20_36, 0x0A);
    msDrvSCCommonWrite(SCALER_MASK_MAIN, DISABLE);

    msWrite2Byte(SCL0_0F_AA, 0x00);
    msWrite2Byte(SCL0_0F_A8, 0x00);

    msWriteBit(REG_LPLL_18,FALSE,_BIT3);
    msWriteByteMask(REG_LPLL_FD, 0x00, BIT7);//ivs spreader disable
    msWriteByteMask(REG_LPLL_E1, 0x00, BIT7|BIT3);//fast lock mode disable
    msWriteByteMask(SCL0_10_3D, 0, _BIT7);//RD suggest this setting only enable in frame lock mode.

    u16Htotal = ( PanelHTotal/(msDrvISSCDualPort() ? 2:1) - 1 );
    switch( ucFrameLockMode )
    {
        case FLM_NO_LOCK:
            bEnableFreeRun = 1;
            bEnableFramePLLMode = 0;
	 #if IS_VTOTAL_BY_TYPE  //   160712_04
            wOutputVtotal=PanelNormalVTotal;
        #else
            wOutputVtotal = PanelVTotal;
        #endif
           ucLockYLine = 0x00;
            //Force 1, msWriteBit(SCL0_0F_AC, FALSE, _BIT1);
            msWriteBit(SCL0_10_21, FALSE, BIT1);
            break;

        case FLM_FBL:
            bEnableFreeRun = 0;
            bEnableFramePLLMode = 0;
            wOutputVtotal = 0x1000;
            ucLockYLine = 0x05;
            msWriteBit(SCL0_10_21, TRUE, BIT1);
            msDrvSetDClkPLL((DWORD)PanelHTotal*PanelVTotal*GetVfreq()/1000);
            Delay1ms(50);
            u16Htotal = ((msReadByte(SCL0_10_68))|((msReadByte(SCL0_10_6C)&0x0F)<<8));
            break;

        case FLM_FB_TUNE_VT: // Old lock
            bEnableFreeRun = 1;
            bEnableFramePLLMode = 1;
            wOutputVtotal = 0x7FF;
            ucLockYLine = 0x04;
            break;

        case FLM_FBL_FRAMEPLL:
            bEnableFreeRun = 1;
            bEnableFramePLLMode = 1;
            //cloude    suggestion
            ucLockYLine = 0x02; //0x03;
            msWriteBit(SCL0_10_21, FALSE, BIT1);

            wModeVtotal = (msRead2Byte(SCL0_01_3E) & 0x1FFF);
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


            msWrite2Byte(SCL0_0F_AA,wLockPoint);
            msWrite2Byte(SCL0_0F_A8,wCVcntFreezePoint);

            //msWriteByteMask(SCL0_10_21, _BIT2, _BIT2);
            break;

        case FLM_FB_TUNE_CLK: // New lock
            bEnableFreeRun = 1;
            bEnableFramePLLMode = 1;
            msWriteBit(SCL0_10_21, FALSE, BIT1);

            wOutputVtotal = PanelVTotal;
            ucLockYLine = 0x04;
            //Force 1, msWriteBit(SCL0_0F_AC, FALSE, _BIT1);
            break;

        case FLM_FB_NEW_LOCK:
        {
            WORD u16InputHeight = 0, u16InputVtotal = 0;
            BYTE u8InputDelay = 0, u8LockYLineDelay = 0;
            WORD u16DispTrig = 0;
            WORD u16TrainTrig = 0;
            WORD u16OutputHeight = g_DisplayWindow[MapScaler2Win(SCDetWin)].sDwDisplayWin.height;
            WORD u16VStartOffset = g_DisplayWindow[MapScaler2Win(SCDetWin)].sDwDisplayWin.y;

            u16InputHeight = g_ScalerInfo[SCDetWin].sSiIPCaptureWin.height/((SrcFlags(InputPort)&bInterlaceMode)?2:1);
            u16InputVtotal = msDrvIP1ReadVtotal(SCDetWin)/((SrcFlags(InputPort)&bInterlaceMode)?2:1);
            bEnableFreeRun = 1;
            bEnableFramePLLMode = 1;
            ucLockYLine = _Y_LOCK_LINE_;
            u8LockYLineDelay = ucLockYLine*2;
            u8InputDelay = _DELAY_INPUT_LINE_;

            wOutputVtotal = msRead2Byte(SCL0_10_1A)&0xFFF;
            // To avoid timing change when set frame sync mode
            if((u16InputVtotal==0)||(wOutputVtotal==0))
            {
                TimeOutCounter = 500;
                while(TimeOutCounter)
                {
                    u16InputVtotal = msDrvIP1ReadVtotal(SCDetWin)/((SrcFlags(InputPort)&bInterlaceMode)?2:1);
                    wOutputVtotal = msRead2Byte(SCL0_10_1A)&0xFFF;
                    if((u16InputVtotal!=0)&&(wOutputVtotal!=0))
                        break;
                }
                if((u16InputVtotal==0)||(wOutputVtotal==0))// change to FLM_FB_TUNE_CLK mode
                {
                    BYTE ucMemFormat = g_DisplayWindow[MapScaler2Win(SCDetWin)].u8DwMemFormat;
                    BYTE SCDetWinMask = g_DisplayWindow[MapScaler2Win(SCDetWin)].u16DwScMaskOut;

                    bEnableFreeRun = 1;
                    bEnableFramePLLMode = 1;
                    msWriteBit(SCL0_10_21, FALSE, BIT1);
                    wOutputVtotal = PanelVTotal;
                    ucLockYLine = 0x04;
                    msAPIWinSetReadCurFrameMode(MapScaler2Win(SCDetWin),FALSE);
                    msDrvSCCommonWrite(SCDetWinMask, ENABLE);
                    if( ucMemFormat == MEM_FMT_444_12 )
                    {
                        if(SCDetWin & BIT0) // MAIN
                            msWriteByteMask(SCL0_12_0F, 0x40, 0xE0);
                        else
                            msWriteByteMask(SCL0_12_8F, 0x40, 0xE0);
                    }
                    else
                    {
                        if(SCDetWin & BIT0) // MAIN
                            msWriteByteMask(SCL0_12_0F, 0x20, 0xE0);
                        else
                            msWriteByteMask(SCL0_12_8F, 0x20, 0xE0);
                    }
                    msDrvSCCommonWrite(SCDetWinMask, DISABLE);
                    break;
                }
            }

            if(wOutputVtotal != (PanelVTotal-1))
            {
                msWrite2Byte(SCL0_10_1A, PanelVTotal-1); // Output V total
                Delay1ms(50);
            }
            wOutputVtotal = PanelVTotal;

            // trig point, vde start - 6
            u16DispTrig = PanelVStart - _DELAY_DISP_TRIG_;
            u16TrainTrig = PanelVStart - _DELAY_DISP_TRIG_ - 3;
            msDrvSCCommonWrite(SCALER_MASK_MAIN, ENABLE);
            msWrite2Byte(SCL0_20_34, u16TrainTrig);
            msWrite2Byte(SCL0_20_36, u16DispTrig);
            msDrvSCCommonWrite(SCALER_MASK_MAIN, DISABLE);

            // ovs position, vcnt value
            if(((DWORD)(u16InputHeight-u8InputDelay) * 1000/ u16InputVtotal) > (((DWORD)u16OutputHeight )*1000/wOutputVtotal))
                wK = (WORD)((int)wOutputVtotal - (((((int)u16InputHeight + u8LockYLineDelay) * wOutputVtotal) / u16InputVtotal) - (u16OutputHeight - _DELAY_DISP_TRIG_ - (6 * u16OutputHeight / u16InputHeight)  - 2)));
            else
            {
                if(((DWORD)(u16InputHeight) * 1000/ u16InputVtotal) > (((DWORD)u16OutputHeight )*1000/wOutputVtotal))
                {
                    BYTE u8Temp;

                    u8Temp = ( u16InputHeight * wOutputVtotal / u16InputVtotal ) - u16OutputHeight;
                    wK = wOutputVtotal - _DELAY_DISP_TRIG_ - (WORD)(((DWORD)(u8InputDelay + u8LockYLineDelay) * wOutputVtotal) / u16InputVtotal) - u8Temp;
                }
                else
                    wK = wOutputVtotal - _DELAY_DISP_TRIG_ - (WORD)(((DWORD)(u8InputDelay + u8LockYLineDelay) * wOutputVtotal) / u16InputVtotal);
            }
            wLockPoint = (wK + VERTICAL_DE_START + u16VStartOffset) % wOutputVtotal;
            wCVcntFreezePoint = (wK - 2 + VERTICAL_DE_START + u16VStartOffset) % wOutputVtotal;
            msWrite2Byte(SCL0_0F_AA,wLockPoint);
            msWrite2Byte(SCL0_0F_A8,wCVcntFreezePoint);
#if !PANEL_EDP && !ENABLE_VBY1_TO_EDP
            msWriteByteMask(SCL0_0F_AC, _BIT0, _BIT0|BIT3);    //V count freeze will work when this register set from 0->1
            msWriteByteMask(SCL0_0F_AC, 0x00, _BIT0|BIT3);
#endif

            msDrvSetDClkPLL((unsigned long long)PanelHTotal*wOutputVtotal*V_FREQ_IN(MapScaler2Port(SCDetWin))/1000);
            drvmStar_printData("woutputVtotall = %d",wOutputVtotal);
            drvmStar_printData("ucLockPoint %x",wLockPoint);
            drvmStar_printData("ucCVcntFreezePoint %x",wCVcntFreezePoint);

            //Force 1, msWriteBit(SCL0_0F_AC, TRUE, _BIT1);
            //msWriteByteMask(SC10_3D, 0, _BIT7);
            //msWriteByteMask(SC10_21, _BIT2, _BIT2);
            break;
        }
        case FLM_FB_FAST_LOCK:
            bEnableFreeRun = 1;
            bEnableFramePLLMode = 1;
            msWriteBit(SCL0_10_21, FALSE, BIT1);
            wOutputVtotal = PanelVTotal;	
            ucLockYLine = 0x04;
            msDrvSetFpllFastLock(SCDetWin);
 #if INPUT_SAME_OUT
        #if 0//(PanelType == PanelLSM315HP01)
	    if((InputWidth==1920)&&(InputHeight==1080)
          #if ENABLE_MULTI_WINDOW_SETTINGS_MENU
            &&(USER_PREF_MULTI_WIN_MODE==eMULTIWIN_OFF)
          #endif
	     )
            msDrvSetDClkPLL(Get_TMDS_ClockRateType(InputPort));
	    else
            msDrvSetDClkPLL((unsigned long long)PanelHTotal*wOutputVtotal*GetVfreq()/1000);
        #elif 1
            msDrvSetDClkPLL((unsigned long long)PanelHTotal*wOutputVtotal*GetVfreq()/1000);
	 #else	
            msDrvSetDClkPLL((DWORD)PanelHTotal*wOutputVtotal*(V_FREQ_IN(MapScaler2Port(SCL0_MAIN)))/1000);
	 #endif		
#endif //INPUT_SAME_OUT
           //Force 1, msWriteBit(SCL0_0F_AC, FALSE, _BIT1);
            break;

        case FLM_FB_IVS_SPREADER:
            bEnableFreeRun = 1;
            bEnableFramePLLMode = 1;
            msWriteBit(SCL0_10_21, FALSE, BIT1);
            wOutputVtotal = PanelVTotal;
            ucLockYLine = 0x04;
            msDrvSetFpllSpreader(SCDetWin);
            //Force 1, msWriteBit(SCL0_0F_AC, FALSE, _BIT1);
            break;

        case FLM_FB_FLOCK:
    #if ENABLE_SW_FLOCK
    {
            WORD u16InputHeight, u16InputVtotal, u16InputHtotal;
            DWORD u32OutputCLK, u32InputCLK, u32InDEDuration, u32OutDEDuration;
            WORD u16DispTrig;
            WORD u16TrainTrig;
            WORD u16OutputHeight = g_DisplayWindow[MapScaler2Win ( SCDetWin )].sDwDisplayWin.height;
            WORD u16ATPVstart;
            WORD u16OutputHTotal;

            u16InputHeight = g_ScalerInfo[SCDetWin].sSiIPCaptureWin.height / ( ( SrcFlags ( InputPort ) &bInterlaceMode ) ? 2 : 1 );
            bEnableFreeRun = 1;
            bEnableFramePLLMode = 0;
            ucLockYLine = 0x08;
            u16InputVtotal = msDrvIP1ReadVtotal ( SCDetWin ) / ( ( SrcFlags ( InputPort ) &bInterlaceMode ) ? 2 : 1 );
            bSrcSync = FALSE;
            u16DispTrig = PanelVStart - _DELAY_DISP_TRIG_;
            u16TrainTrig = PanelVStart - _DELAY_DISP_TRIG_ - 3;

            if ( INPUT_IS_DISPLAYPORT ( InputPort ) )
            {
				msAPI_combo_IPGetDPHVInformation((EN_COMBO_IP_SELECT)g_InputPort[InputPort].eIPMux, &u16InputHtotal, &u16InputVtotal);
            }
			else
            {
				u16InputHtotal = msDrvIP1ReadHtotal(SCDetWin)*(HBR2Flag(InputPort) ? 2:1);
            }

			if(!(SrcHPeriod(InputPort) && u16InputHtotal && u16InputVtotal))
			{
				bEnableFramePLLMode = 1;
				msWriteBit(SCL0_10_21, FALSE, BIT1);
				ucLockYLine = 0x04;
				g_ScalerInfo[SCDetWin].u8SiSyncMode = FLM_FB_TUNE_CLK;
				break;
			}
			
	     if(INPUT_IS_DISPLAYPORT(InputPort)||INPUT_IS_TMDS(InputPort))
            {
				//u32InputCLK = msAPI_combo_IPGetPixelClock_10KHz((EN_COMBO_IP_SELECT)g_InputPort[InputPort].eIPMux)*10;
                u32InputCLK = msAPI_combo_IPGetPixelClock_10KHz((EN_COMBO_IP_SELECT)g_InputPort[InputPort].eIPMux)*10;
                if(u32InputCLK== 0)//neo 20170111 Fix system halted after doing DC on/off(if DE is not stalbe will return 0)
                {
                    Delay1ms(50);
                    u32InputCLK = msAPI_combo_IPGetPixelClock_10KHz((EN_COMBO_IP_SELECT)g_InputPort[InputPort].eIPMux)*10;
                    if(u32InputCLK== 0)//TBD
                    {
                        Set_InputTimingChangeFlag(InputPort); // setup mode failed
                        drvmStar_SetupFreeRunMode(SCDetWin); // setup freerun mode
                        return;
                    }
                }
            }
	    else
            {
				u32InputCLK = (unsigned long long)MST_XTAL_CLOCK_KHZ * u16InputHtotal / SrcHPeriod(InputPort);
            }

    #if USE_FIXED_DCLK_FOR_FREESYNCMODE  //fix output dclk too small when enable Freesync Mode
            if ((INPUT_IS_HDMI_DRR(InputPort))||( INPUT_IS_DP_DRR(InputPort)))
            {
                u32OutputCLK = GetFreesyncFixClock(InputPort);
                msDrvSetDClkPLL(u32OutputCLK);
                wOutputVtotal = PanelVTotal;
                u16OutputHTotal = GetFreesyncHTotal(InputPort);
                u16Htotal = (GetFreesyncHTotal(InputPort))/(msDrvISSCDualPort() ? 2:1) - 1;
            }
            else
            {
                wOutputVtotal = PanelVTotal;
                u32OutputCLK = (unsigned long long)u16Htotal*wOutputVtotal*u32InputCLK/(u16InputHtotal*u16InputVtotal);
                wOutputVtotal = PanelMinVTotal+5;
				u16OutputHTotal = PanelHTotal; // 20170218 kater Fix u16OutputHTotal not init return  0 issue
                msDrvSetDClkPLL(u32OutputCLK);
            }

            printData("u16InputHtotal=%d", u16InputHtotal);
            printData("u16InputHeight=%d", u16InputHeight);
            printData("u16OutputHTotal=%d", u16OutputHTotal);
            printData("u16Htotal=%d", u16Htotal);
            printData("u16InputVtotal=%d", u16InputVtotal);

            u32InDEDuration = (u32InputCLK *1000) / (u16InputHtotal * u16InputHeight);
            u32OutDEDuration = (u32OutputCLK *1000) / (u16OutputHTotal * u16OutputHeight);
    #else
            if ( INPUT_IS_HDMI_DRR ( InputPort ) )
            {
                wOutputVtotal = PanelVStart + PanelHeight + 5;
                u32OutputCLK = ( unsigned long long ) PanelHTotal * PanelHeight * u32InputCLK / ( u16InputHtotal * u16InputHeight );
            }
            else
            {
                wOutputVtotal = PanelVTotal;
                u32OutputCLK = ( unsigned long long ) PanelHTotal * wOutputVtotal * u32InputCLK / ( u16InputHtotal * u16InputVtotal );
            }
            msDrvSetDClkPLL(u32OutputCLK);

            u32InDEDuration = (u32InputCLK *1000) / (u16InputHtotal * u16InputHeight);
            u32OutDEDuration = (u32OutputCLK *1000) / (PanelHTotal * u16OutputHeight);
    #endif

            if((u32OutDEDuration > u32InDEDuration)&&(u32InDEDuration!=0)&&(u32OutDEDuration!=0))
            {
                u16ATPVstart = g_ScalerInfo[SCDetWin].sSiIPCaptureWin.y + _DELAY_INPUT_LINE_ + ((u32InputCLK*1000)/(u16InputHtotal*u32InDEDuration)) - ((u32InputCLK*1000)/(u16InputHtotal*u32OutDEDuration));
            }
            else
            {
                u16ATPVstart = g_ScalerInfo[SCDetWin].sSiIPCaptureWin.y + _DELAY_INPUT_LINE_;
            }

            msWriteByteMask(SCL0_01_32, BIT0, BIT0);
            msWriteByteMask(SCL0_01_53, BIT0, BIT0);
            msWrite2ByteMask(SCL0_01_54, u16ATPVstart, 0x1FFF);
            msWriteBit(SCL0_10_21, TRUE, BIT1);
            msWriteByteMask(SCL0_10_3D, _BIT7, _BIT7);
            msDrvSCCommonWrite(SCALER_MASK_MAIN, ENABLE);
            msWrite2Byte(SCL0_20_34, u16TrainTrig);
            msWrite2Byte(SCL0_20_36, u16DispTrig);
        #if ENABLE_VCNT_FREEZE
            msWrite2Byte(SCL0_0F_A8, (PanelVStart+PANEL_HEIGHT-1+3));
        #else
            msWrite2Byte(SCL0_0F_A8,wOutputVtotal-3);
        #endif
            msDrvSCCommonWrite(SCALER_MASK_MAIN, DISABLE);
        #if ENABLE_VCNT_FREEZE
            wOutputVtotal=msRead2Byte(SCL0_0F_A8)+3;
            msWrite2Byte(SCL0_10_1A, msRead2Byte(SCL0_0F_A8)+3); // Output V total
        #else
            msWrite2Byte(SCL0_10_1A, wOutputVtotal-1); // Output V total
        #endif

            msWriteByteMask(SCL0_01_12, ucLockYLine, 0x0F);
        #if CHIP_ID==MST9U4
            msWriteByte(REG_LPLL_19, (0<<4)|(0&0x0F)); //[3:0] input frame div for frame sync, [7:4] output frame div for frame sync
            msWriteByte(REG_LPLL_1C, (0<<4)|(0&0x0F)); //[3:0] input frame div[7:4], [7:4] output frame div [7:4]
        #else
            msWriteByte(REG_LPLL_19, (0<<4)|(0&0x0F)); //[3:0] input frame div for frame sync, [7:4] output frame div for frame sync
            msWriteByteMask(REG_LPLL_1C, ((0&BIT4)>>3)|(0>>4),BIT1|BIT0); //[0] input frame div[4], [1] output frame div [4]
        #endif

            drvmStar_printData("g_ScalerInfo[SCDetWin].sSiIPCaptureWin.y = %d", g_ScalerInfo[SCDetWin].sSiIPCaptureWin.y);
            drvmStar_printData("u32InputCLK = %d", u32InputCLK>>16);
            drvmStar_printData("u32InputCLK2 = %d", u32InputCLK);
            drvmStar_printData("u32OutputCLK = %d", u32OutputCLK>>16);
            drvmStar_printData("u32OutputCLK2 = %d", u32OutputCLK);
            drvmStar_printData("u32InDEDuration = %d", u32InDEDuration);
            drvmStar_printData("u32OutDEDuration = %d", u32OutDEDuration);
            drvmStar_printData("u16ATPVstart = %d", u16ATPVstart);
            drvmStar_printData("u32InputHTotal = %d", u16InputHtotal);
            drvmStar_printData("u16OutputHTotal = %d", u16OutputHTotal);

            break;
        }

    #else	
    	{
            WORD u16InputHeight, u16InputVtotal;
            WORD u16DispTrig;
            WORD u16TrainTrig;
            BYTE TempA=0;
            WORD u16OutputHeight = g_DisplayWindow[MapScaler2Win(SCDetWin)].sDwDisplayWin.height;
	  #if ((PanelType==PanelLSM315HP01)||(PanelType==PanelM270QAN01))
	     DWORD InputDCLK=Get_TMDS_ClockRateType(InputPort);
	  #endif

			
            u16InputHeight = g_ScalerInfo[SCDetWin].sSiIPCaptureWin.height/((SrcFlags(InputPort)&bInterlaceMode)?2:1);
            bEnableFreeRun = 0;
            bEnableFramePLLMode = 0;

#if (PanelType==PanelLSM315DP02)
   if(!SyncLossState(MapScaler2Port(SCL0_MAIN))) 
   	{
            ucLockYLine = 0x08;
            wOutputVtotal = 1468;//PanelVTotal;//
   	}
      else
#endif	
      {
            ucLockYLine = 0x08;
            wOutputVtotal = PanelVTotal;
      }
            u16InputVtotal = (msRead2Byte(SCL0_01_3E) & 0x1FFF)/((SrcFlags(InputPort)&bInterlaceMode)?2:1);


            // To avoid timing change when set frame sync mode
            if(u16InputVtotal==0)
            {
                TimeOutCounter = 500;
                while(TimeOutCounter)
                {
                    u16InputVtotal = (msRead2Byte(SCL0_01_3E) & 0x1FFF)/((SrcFlags(InputPort)&bInterlaceMode)?2:1);
                    if(u16InputVtotal!=0)
                        break;
                }
                if(u16InputVtotal==0)// change to FLM_FB_TUNE_CLK mode
                {
                    BYTE ucMemFormat = g_DisplayWindow[MapScaler2Win(SCDetWin)].u8DwMemFormat;
                    BYTE SCDetWinMask = g_DisplayWindow[MapScaler2Win(SCDetWin)].u16DwScMaskOut;

                    bEnableFreeRun = 1;
                    bEnableFramePLLMode = 1;
                    msWriteBit(SCL0_10_21, FALSE, BIT1);
                    wOutputVtotal = PanelVTotal;

          #if 0//(PanelType==PanelLSM315HP01)
                 if((!SyncLossState(MapScaler2Port(SCL0_MAIN)))&&(V_FREQ_IN(MapScaler2Port(SCL0_MAIN))<=110))	 
                    wOutputVtotal = 2100;
	   #endif
					
                    ucLockYLine = 0x04;
                    msAPIWinSetReadCurFrameMode(MapScaler2Win(SCDetWin),FALSE);
                    msDrvSCCommonWrite(SCDetWinMask, ENABLE);
                    if( ucMemFormat == MEM_FMT_444_12 )
                    {
                        if(SCDetWin & BIT0) // MAIN
                            msWriteByteMask(SCL0_12_0F, 0x40, 0xE0);
                        else
                            msWriteByteMask(SCL0_12_8F, 0x40, 0xE0);
                    }
                    else
                    {
                        if(SCDetWin & BIT0) // MAIN
                            msWriteByteMask(SCL0_12_0F, 0x20, 0xE0);
                        else
                            msWriteByteMask(SCL0_12_8F, 0x20, 0xE0);
                    }
                    msDrvSCCommonWrite(SCDetWinMask, DISABLE);
                    break;
                }
            }
            msWriteByteMask(SCL0_10_3D, _BIT7, _BIT7);

            u16DispTrig = PanelVStart - _DELAY_DISP_TRIG_;
            u16TrainTrig = PanelVStart - _DELAY_DISP_TRIG_ - 3;
            msDrvSCCommonWrite(SCALER_MASK_MAIN, ENABLE);
            msWrite2Byte(SCL0_20_34, u16TrainTrig);
            msWrite2Byte(SCL0_20_36, u16DispTrig);
            msDrvSCCommonWrite(SCALER_MASK_MAIN, DISABLE);
            msWrite2Byte(SCL0_10_1A, wOutputVtotal-1); // Output V total
            msWriteByteMask(SCL0_01_12, ucLockYLine, 0x0F);
            if(((DWORD)(u16InputHeight) * 1000/ u16InputVtotal) > (((DWORD)u16OutputHeight )*1000/wOutputVtotal))
            {
                TempA = (u16InputHeight - (u16OutputHeight * u16InputVtotal /  wOutputVtotal));
                ucDelayLine = ucLockYLine + _DELAY_INPUT_LINE_  + TempA;
            }
            else
            {
                ucDelayLine = ucLockYLine + _DELAY_INPUT_LINE_ ;
            }
            msWriteBit(SCL0_10_21, TRUE, BIT1);

            if( INPUT_IS_HDMI_DRR(InputPort) )
            {
            #if (PanelType==PanelLSM315HP01)    
		  InputDCLK=Get_TMDS_ClockRateType(InputPort);

		printData("HDMI InputVfreq==%d",InputVfreq);
		printData("HDMI GetVfreq==%d",GetVfreq());
		printf("HDMI InputDCLK==%d",InputDCLK);
	     if((abs(InputWidth-1920)<10)&&(abs(InputHeight-1080)<10)/*&&((abs(InputVfreq-1200)<10)||(abs(InputVfreq-1440)<10))*/)
	     	{
		     msDrvSetDClkPLL(InputDCLK);	
	     	}
              else if(abs((InputVfreq/10)-GetVfreq())<8)
                   msDrvSetDClkPLL((DWORD)PanelHTotal*wOutputVtotal*(V_FREQ_IN(MapScaler2Port(SCL0_MAIN)))/1000);
              else
		     msDrvSetDClkPLL(InputDCLK);	
			  
		if(V_FREQ_IN(MapScaler2Port(SCL0_MAIN))<=110)	
		  ucDelayLine=0;	
		
	     #elif 	(PanelType==PanelLM270WR2)
                msDrvSetDClkPLL((DWORD)PanelHTotal*wOutputVtotal*(V_FREQ_IN(MapScaler2Port(SCL0_MAIN)))/1000);
            #elif (PanelType==PanelM270QAN01)
		  msDrvSetDClkPLL(InputDCLK);	
	     #else			
                msDrvSetDClkPLL((unsigned long long)(MST_XTAL_CLOCK_KHZ)* PanelHTotal * (PanelHeight-1) / SrcHPeriod(InputPort)  / (u16InputHeight-1));
            #endif
            }
            else if( INPUT_IS_DP_DRR(InputPort) )
            {
                WORD u16PixelClock = msAPI_combo_IPGetDPPixelClock_10KHz((EN_COMBO_IP_SELECT)g_InputPort[InputPort].eIPMux);
                WORD usDPHTotal = 0, usDPVTotal = 0;
                WORD u16Vfreq = 0;
		
                msAPI_combo_IPGetDPHVInformation((EN_COMBO_IP_SELECT)g_InputPort[InputPort].eIPMux, &usDPHTotal, &usDPVTotal);
                u16Vfreq = ((unsigned long long)u16PixelClock*1000*1000+((DWORD)usDPVTotal*usDPHTotal)/2)/((DWORD)usDPVTotal*usDPHTotal);
	
        #if 1
           #if (PanelType==PanelLSM315HP01)    
		 InputDCLK=Get_TMDS_ClockRateType(InputPort);
		printData("InputVfreq==%d",InputVfreq);
		printData("GetVfreq==%d",GetVfreq());

		printf("InputDCLK==%x",InputDCLK);
		
		if((abs(InputWidth-1920)<10)&&(abs(InputHeight-1080)<10)/*&&((abs(InputVfreq-1200)<10)||(abs(InputVfreq-1440)<10))*/)
		 msDrvSetDClkPLL(InputDCLK);	
              else
               msDrvSetDClkPLL((unsigned long long)PanelHTotal*wOutputVtotal*u16Vfreq/1000/100);
		#else
             msDrvSetDClkPLL((unsigned long long)PanelHTotal*wOutputVtotal*u16Vfreq/1000/100);
		#endif
       #else      
             if((u16Vfreq/100)>123)
             msDrvSetDClkPLL(Get_TMDS_ClockRateType(InputPort));
            else
             msDrvSetDClkPLL((unsigned long long)PanelHTotal*wOutputVtotal*u16Vfreq/1000/100);
	#endif		

            #if (PanelType==PanelM350DVR01_01||PanelType==PanelM350DVA01_1)    
		if(V_FREQ_IN(MapScaler2Port(SCL0_MAIN))<=110)	
		  ucDelayLine=0;		
	     #endif
	
            #if (PanelType==PanelLSC400FN04)    
		if(abs(V_FREQ_IN(MapScaler2Port(SCL0_MAIN))-60)<2)	
		  ucDelayLine=ucDelayLine+0x30;		
	     #endif	
            }
            else
            {
                drvmStar_printData("VFreqIn(InputPort) = %d", V_FREQ_IN_10X(InputPort));
                msDrvSetDClkPLL((unsigned long long)PanelHTotal*wOutputVtotal*V_FREQ_IN_10X(InputPort)/10000);
            }



#if CHIP_ID==MST9U4
            msWriteByte(REG_LPLL_19, (0<<4)|(0&0x0F)); //[3:0] input frame div for frame sync, [7:4] output frame div for frame sync
            msWriteByte(REG_LPLL_1C, (0<<4)|(0&0x0F)); //[3:0] input frame div[7:4], [7:4] output frame div [7:4]
#else
            msWriteByte(REG_LPLL_19, (0<<4)|(0&0x0F)); //[3:0] input frame div for frame sync, [7:4] output frame div for frame sync
            msWriteByteMask(REG_LPLL_1C, ((0&BIT4)>>3)|(0>>4),BIT1|BIT0); //[0] input frame div[4], [1] output frame div [4]
#endif
            break;
        }
  #endif


		
        case FLM_FBL_FLOCK:
        {
            WORD u16InputHeight, u16InputHTotal, u16InputVTotal;
            WORD u16OutputHeight, u16OutputHTotal;
            DWORD u32IvsPeriod;

            u16InputHeight = g_ScalerInfo[SCDetWin].sSiIPCaptureWin.height/((SrcFlags(InputPort)&bInterlaceMode)?2:1);
            u16OutputHeight = g_DisplayWindow[MapScaler2Win(SCDetWin)].sDwDisplayWin.height;
            ucLockYLine = 0x04;
            ucDelayLine = 0x01;
            wOutputVtotal = PanelVTotal;
            bEnableFramePLLMode = 1;
            bEnableFreeRun = 0;
          #if ENABLE_SW_FLOCK
            msDrvIP1SetOutputFreerun(FALSE, SCDetWin); //disable input no signal
          #else
            msWriteByteMask(SC01_04+u32SCIP1RegBase, 0x00, BIT7); //disable input no signal
	   #endif		
            ForceDelay1ms(100);

            u32IvsPeriod = msRead3Byte(REG_LPLL_42);

            if(u32IvsPeriod==0)
            {
                TimeOutCounter = 500;
                while(TimeOutCounter)
                {
                    u32IvsPeriod = msRead3Byte(REG_LPLL_42);
                    if(u32IvsPeriod!=0)
                        break;
                }

                if(u32IvsPeriod==0)
                { //TBD---TBD---TBD---
                    u32IvsPeriod = CRYSTAL_CLOCK/(GetInputVfreq(MapScaler2Win(SCDetWin))/10);
                    if(u32IvsPeriod==0)
                    {
                        u32IvsPeriod = CRYSTAL_CLOCK/GetVfreq();
                        drvmStar_printMsg("Warning!!! u32IvsPeriod is equal to 0, reset VfreqIn to PanelVfreq\n");
                    }
                }
            }

             if(INPUT_IS_DISPLAYPORT(InputPort))
             {
                msAPI_combo_IPGetDPHVInformation((EN_COMBO_IP_SELECT)g_InputPort[InputPort].eIPMux, &u16InputHTotal, &u16InputVTotal);
                u16InputHTotal = u16InputHTotal/(msDrvISSCDualPort()?2:1);
             }
             else
             {
                u16InputHTotal = msDrvIP1ReadHtotal(SCDetWin);
                u16InputVTotal = msDrvIP1ReadVtotal(SCDetWin);
             }

            u16OutputHTotal = PanelHTotal/(msDrvISSCDualPort()?2:1);
            msWriteByteMask(SCL0_10_3D, _BIT7, _BIT7);
            msDrvSetDClkPLL((unsigned long long)PanelHTotal*u16InputVTotal*CRYSTAL_CLOCK*u16OutputHeight/u32IvsPeriod/u16InputHeight/1000); //panelHtt*Vtti*Vfreqi*VDEi/Vdeo

            msWriteByte(REG_LPLL_32, u16InputHTotal); //input clock divider
            msWriteByte(REG_LPLL_33, u16InputHTotal>>8);
        #if CHIP_ID==MST9U4
            msWrite2Byte(REG_LPLL_34,(u16InputHTotal>>16)&0x1FF);
        #else
            msWrite2Byte(REG_LPLL_34,(u16InputHTotal>>16)&0xFF);
        #endif

            msWriteByte(REG_LPLL_36, u16OutputHTotal); //output clock divider
            msWriteByte(REG_LPLL_37, u16OutputHTotal>>8);
            msWriteByte(REG_LPLL_38, u16OutputHTotal>>16);
            msWriteByteMask(REG_LPLL_E1, BIT7, BIT7); //enable lock freq mode

            //drvmStar_printData("u32IvsPeriod = %x", u32IvsPeriod);
            //drvmStar_printData("u32InputHTotal = %d", u16InputHTotal);
            //drvmStar_printData("u32InputVTotal = %d", u16InputVTotal);
            break;
        }
        case FLM_INIT:
        default:
            break;
    }

    msDrvSCCommonWrite(SCALER_MASK_MAIN, TRUE);
    msWrite2ByteMask(SCL0_10_18,u16Htotal,0x0FFF);//[11:0]Output Htt
    msWrite2Byte(SCL0_10_1A, wOutputVtotal-1); // Output V total
    msWrite2ByteMask(SCL0_10_39, _BIT0, _BIT0);
    msWriteByteMask(SCL0_01_12, ucLockYLine, 0x0F);
    msWrite2ByteMask(SCL0_01_38, ucDelayLine<<4, _BIT9|_BIT8|_BIT7|_BIT6|_BIT5|_BIT4);   //delay line
    msDrvSCCommonWrite(SCALER_MASK_MAIN, FALSE);

#if FPLL_TUNE_SW_MODE
    if( bEnableFramePLLMode )
    {
            msWrite2Byte(REG_LPLL_16, 0x1010);
            g_u16FPLLTuneTimes = 0x400;
            g_u16FPLLTuneDelay = (g_u16FPLLTuneDelay * GetVSyncTime(InputPort) / 10 + 1);
    }
    else
        g_u16FPLLTuneTimes = 0;
#else
    g_u16FPLLTuneTimes = 0;
#endif
#if FPLL_TUNE_SW_HTT
    bEnableFramePLLMode = 0;    // pll lock enable at msDrvResetOutputVtt
#endif

    msWriteBit(SCL0_10_39, bEnableFreeRun, _BIT0); // Output freerun enable
    msWriteBit(REG_LPLL_18, bEnableFramePLLMode,_BIT3); // lock input //20081110 Frame pll enable

#if !ENABLE_MENULOAD
    msWriteBit(SCL0_00_02,TRUE,_BIT0);//double buffer enable
#endif
#if ENABLE_SW_FLOCK
    msWriteBit(SCL0_01_07, bSrcSync, _BIT7); // Frame Lock Always ON
#endif    
    msWriteBit(SCL0_00_02,TRUE,_BIT2);//double buffer load by vsync
}


void msDrvSetTimingGen(BYTE SCDetWin)
{
    DWORD u32SCRegBase = SCRegBase(SCDetWin);
    BYTE u8WinIndex = MapScaler2Win(SCDetWin);
    WORD u16ImgHDEStart = PanelHStart/(msDrvISSCDualPort() ? 2:1) + g_ScalerInfo[SCDetWin].sSiOP2DisplayWin.x + g_ScalerInfo[SCDetWin].sSiImgDEOffset.u32OffsetHStart;
    WORD u16ImgHDEEnd = (u16ImgHDEStart + g_ScalerInfo[SCDetWin].sSiOP2DisplayWin.width - 1) - g_ScalerInfo[SCDetWin].sSiImgDEOffset.u32OffsetHStart - g_ScalerInfo[SCDetWin].sSiImgDEOffset.u32OffsetHEnd;
    WORD u16PnlHDEEnd = (PanelHStart/(msDrvISSCDualPort() ? 2:1) + PANEL_WIDTH/(msDrvISSCDualPort() ? 2:1) -1); // Not real register setting, please refer to msDrvmStarInit().
    WORD u16PnlVDEStart, u16PnlVDEEnd;
    WORD u16ImgVDEStart, u16ImgVDEEnd;
    WORD u16VsyncStart, u16VsyncEnd;
    BYTE u8GuardBandLeft = 0, u8GuardBandRight = 0, u8Guardband;
    BYTE u8SCIndex;

	if( g_ScalerInfo[SCDetWin].u8SiFbMode == 0 )
    {
        u16PnlVDEStart = 0x00;
        u16ImgVDEStart = 0x00;
        u16VsyncStart = PanelVTotal - PanelVStart + VERTICAL_DE_START;
        u16VsyncEnd = ( PanelVTotal - PanelVSyncBackPorch + VERTICAL_DE_START )%PanelVTotal;
    }
    else if( g_ScalerInfo[SCDetWin].u8SiSyncMode == FLM_FB_FLOCK )
    {
        u16PnlVDEStart = PanelVStart;
        u16ImgVDEStart = PanelVStart;
        u16VsyncStart = 0x00;
        u16VsyncEnd = PanelVSyncWidth;
    }
    else
    {
        u16PnlVDEStart = VERTICAL_DE_START;
        u16ImgVDEStart = VERTICAL_DE_START;
        u16VsyncStart = PanelVTotal - PanelVStart + VERTICAL_DE_START;
        u16VsyncEnd = (PanelVTotal - PanelVSyncBackPorch + VERTICAL_DE_START) % PanelVTotal;
    }

    u16PnlVDEEnd = u16PnlVDEStart + PANEL_HEIGHT - 1;
    u16ImgVDEStart += g_ScalerInfo[SCDetWin].sSiOP2DisplayWin.y + g_ScalerInfo[SCDetWin].sSiImgDEOffset.u32OffsetVStart;
    u16ImgVDEEnd = (u16ImgVDEStart + g_ScalerInfo[SCDetWin].sSiOP2DisplayWin.height - 1) - g_ScalerInfo[SCDetWin].sSiImgDEOffset.u32OffsetVStart - g_ScalerInfo[SCDetWin].sSiImgDEOffset.u32OffsetVEnd;
    u16VsyncStart = (u16VsyncStart%PanelVTotal)|(((u16VsyncStart%PanelVTotal)<<1)&0x1000);

#if !((ENABLE_SUBSCOUT) || (ENABLE_SUBSC_HOLLOW_OUT))
    if((SCDetWin&BIT0) == 0) // sub
        return;
#endif

    if( g_ScalerInfo[SCDetWin].sSiOP2GuardBand.u8Left && !g_ScalerInfo[SCDetWin].sSiImgDEOffset.u32OffsetHStart )
        u8GuardBandLeft = g_ScalerInfo[SCDetWin].u8SiGuardBandOut;
    else if( g_ScalerInfo[SCDetWin].sSiOP2GuardBand.u8Right && !g_ScalerInfo[SCDetWin].sSiImgDEOffset.u32OffsetHEnd )
        u8GuardBandRight = g_ScalerInfo[SCDetWin].u8SiGuardBandOut;

    if( HBR2Flag(MapScaler2Port(SCDetWin)) && IsWindowMemBal(u8WinIndex) )
    {
        if( (g_ScalerInfo[SCDetWin].u8SiGuardBandOut > SC_GUARD_BAND_PQ) && g_ScalerInfo[SCDetWin].u8SiFbMode )
            u8Guardband = SC_GUARD_BAND_PQ;
        else
            u8Guardband = g_ScalerInfo[SCDetWin].u8SiGuardBandOut;

#if ENABLE_SCALER_LRSWAP
        if( !IsScalerRight(SCDetWin) )
#else
        if( IsScalerRight(SCDetWin) )
#endif
            u8GuardBandLeft = u8Guardband;
        else
            u8GuardBandRight = u8Guardband;
    }

#ifndef MST9U_FPGA // MST9U4_TBD
    if( (u16ImgHDEEnd+u8GuardBandRight) >= u16PnlHDEEnd)
        u16ImgHDEEnd +=1;
#endif

    if( (g_ScalerInfo[SCDetWin].eSiDispStatus != SC_STATUS_IDLE)
      &&(g_ScalerInfo[SCDetWin].sSiOP2DisplayWin.width != 0) ) // for 2 sc ==> 1 sc case
    {
		for( u8SCIndex=SCL0_MAIN; u8SCIndex < SC_MAX_NUMS-1 ; u8SCIndex+=2 ) // Consider Vsync?, Scaler Disable Case?
	    {
			DB_W2BMask(SCRegBase(u8SCIndex)+SC10_0C,u16PnlVDEStart,0x0FFF);//[11:0]vfDE_start
			DB_W2BMask(SCRegBase(u8SCIndex)+SC10_0E,u16PnlVDEEnd,0x0FFF);//[11:0]vfDE_end
	    }

        if( SCDetWin & BIT0 )//Main
        {
            DB_WBMask(u32SCRegBase+SC10_03, 0, BIT5); //[5]: main timing gen mute enable
            DB_W2BMask(u32SCRegBase+SC10_10,(u16ImgHDEStart-u8GuardBandLeft),0x1FFF);//[12:0]hmDE_start
            DB_W2BMask(u32SCRegBase+SC10_12,(u16ImgHDEEnd+u8GuardBandRight),0x1FFF);//[12:0]hmDE_end
            DB_W2BMask(u32SCRegBase+SC10_14,u16ImgVDEStart,0x0FFF);//[11:0]vmDE_start
            DB_W2BMask(u32SCRegBase+SC10_16,u16ImgVDEEnd,0x0FFF);//[11:0]vmDE_end
            DB_WBMask(u32SCRegBase+SC20_20, _BIT0, _BIT0);   //Enable main window
            DB_W2BMask(u32SCRegBase+SC10_04,u16VsyncStart,0x17FF);//[10:0]vsync_start
            DB_W2BMask(u32SCRegBase+SC10_06,u16VsyncEnd,0x0FFF);//[10:0]vsync_start
        }
        else//Sub
        {
            DB_WBMask(u32SCRegBase+SC10_03, 0, BIT4); //[5]: main timing gen mute enable
            DB_W2BMask(u32SCRegBase+SC0F_0E,(u16ImgHDEStart-u8GuardBandLeft),0x0FFF);//[11:0]shDE_start
            DB_W2BMask(u32SCRegBase+SC0F_10,(u16ImgHDEEnd+u8GuardBandRight),0x0FFF);//[11:0]shDE_end
            DB_W2BMask(u32SCRegBase+SC0F_12,u16ImgVDEStart,0x0FFF);//[11:0]svDE_start
            DB_W2BMask(u32SCRegBase+SC0F_14,u16ImgVDEEnd,0x0FFF);//[11:0]svDE_end
            DB_WBMask(u32SCRegBase+SC20_20, _BIT1, _BIT1);
        }
    }
    else
    {
        if( SCDetWin & BIT0 )//Main
        {
            DB_WBMask(u32SCRegBase+SC10_03, BIT5, BIT5); //[5]: main timing gen mute enable
            // Only set H start, H end to "0" for solving garbage issue during TG on/off
            DB_W2BMask(u32SCRegBase+SC10_10, 0x00, 0x1FFF);//[12:0]hmDE_start
            DB_W2BMask(u32SCRegBase+SC10_12, 0x00, 0x1FFF);//[12:0]hmDE_end
            //DB_W2BMask(u32SCRegBase+SC10_14, 0x00, 0x0FFF);//[11:0]vmDE_start
            //DB_W2BMask(u32SCRegBase+SC10_16, 0x00, 0x0FFF);//[11:0]vmDE_end
            DB_WBMask(u32SCRegBase+SC20_20, _BIT0, _BIT0);   //Enable main window
        }
        else//Sub
        {
            DB_WBMask(u32SCRegBase+SC10_03, BIT4, BIT4); //[4]: sub timing gen mute enable
            DB_W2BMask(u32SCRegBase+SC0F_0E, 0x00, 0x0FFF);//[11:0]shDE_start
            DB_W2BMask(u32SCRegBase+SC0F_10, 0x00, 0x1FFF);//[11:0]hmDE_end
            //DB_W2BMask(u32SCRegBase+SC0F_12, 0x00, 0x0FFF);//[11:0]svDE_start
            //DB_W2BMask(u32SCRegBase+SC0F_14, 0x00, 0x0FFF);//[11:0]vmDE_end
            // SC20_20[1] : disable, sub disable in scaler drive
            DB_WBMask(SC20_20+u32SCRegBase, 0, _BIT1);
        }
    }

    DB_W2B(u32SCRegBase+SC10_8C, 0x4000); //RD suggest
    DB_WBit(u32SCRegBase+SC0F_AC, TRUE, _BIT1); // RD suggest


#ifdef MST9U_FPGA
    DB_W2B(0x00102F00+SC0D_A8, (msDrvISSCDualPort()?_BIT15:0)|(PANEL_WIDTH-1));
#endif
}

#define MIN_VFREQ_DIFF      2
BOOL msDrvSetFrameDivider( BYTE vfreq_N )
{
    ////////////////////////////////
    //  FRC ratio N/M ==> (N-1), (M-1)//
    ////////////////////////////////
    BYTE u8Index_N;
    BYTE u8Index_M;
    BYTE u8Div_N=1; // input divider
    BYTE u8Div_M=1; // output divider
    BYTE u8VFreq_M; // output clock
    DWORD u32MinDiff; // minimum differencec
    BYTE u8Index_N_step;
    float u32i_gain ,u32p_gain;
    BOOL bFRC = FALSE;

    u8VFreq_M = GetVfreq();
    if(labs(u8VFreq_M - vfreq_N)> MIN_VFREQ_DIFF)
    {
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
    }
    // Set i gain, p gain
    u32i_gain = ( float )(( float )30*524288/8/(PanelHTotal/(msDrvISSCDualPort()?2:1))/(WORD)PanelVTotal*u8LPLL_LOOP_GAIN )/(float)(u8Div_M*fLPLL_LOOP_DIV)*1024;
    if(u32i_gain < 32)
        u32i_gain = 0;
    else
        u32i_gain = GetLog((DWORD)(u32i_gain)) - 5;
    u32p_gain = u32i_gain + 1;

    g_u16FPLLTuneDelay = u8Div_N;

    u8Div_N = u8Div_N-1;
    u8Div_M = u8Div_M-1;

    if( u8Div_N || u8Div_M )
        bFRC = TRUE;

    msWriteByteMask(REG_LPLL_16,(BYTE)u32i_gain       ,BIT0|BIT1|BIT2|BIT3);//i_gain
    msWriteByteMask(REG_LPLL_16,((BYTE)u32p_gain)<<4  ,BIT4|BIT5|BIT6|BIT7);//p_gain
    msWriteByteMask(REG_LPLL_17,(BYTE)u32i_gain       ,BIT0|BIT1|BIT2|BIT3);//i_gain_phase
    msWriteByteMask(REG_LPLL_17,((BYTE)u32p_gain)<<4  ,BIT4|BIT5|BIT6|BIT7);//p_gain_phase

    //drvmStar_printData( "Set input V_divier_N = %d", u8Div_N);
    //drvmStar_printData( "Set output V_divier_M = %d", u8Div_M);
#if CHIP_ID==MST9U4
    msWriteByte(REG_LPLL_19, ((u8Div_M&0x0F)<<4)|(u8Div_N&0x0F)); //[3:0] input frame div for frame sync, [7:4] output frame div for frame sync
    msWriteByteMask(REG_LPLL_1C, (u8Div_M&0xF0)|((u8Div_N&0xF0)>>4),0xFF); //[3:0] input frame div[7:4], [7:4] output frame div [7:4]
#else
    msWriteByte(REG_LPLL_19, (u8Div_M<<4)|(u8Div_N&0x0F)); //[3:0] input frame div for frame sync, [7:4] output frame div for frame sync
    msWriteByteMask(REG_LPLL_1C, ((u8Div_M&BIT4)>>3)|(u8Div_N>>4),BIT1|BIT0); //[0] input frame div[4], [1] output frame div [4]
#endif

    return bFRC;
}

BYTE GetVfreq( void )
{
#if (defined(JieBo_BangZi_Project)&&(PanelType==PanelV400DK1_KS1))
  if((V_FREQ_IN(MapScaler2Port(SCL0_MAIN)))>62)
        return (PanelVfreq/10);	
  else
    return V_FREQ_IN(MapScaler2Port(SCL0_MAIN));

#elif 0//(PanelType==PanelM350DVR01_01||PanelType==PanelM350DVA01_1)
    if(!SyncLossState(MapScaler2Port(SCL0_MAIN)))
    {
     #if ENABLE_MULTI_WINDOW_SETTINGS_MENU
       if(USER_PREF_MULTI_WIN_MODE==eMULTIWIN_OFF)
     #else
	 if(1)
     #endif
        return V_FREQ_IN(MapScaler2Port(SCL0_MAIN));
	else
        return 60;
     }
     else if(!SyncLossState(MapScaler2Port(SCL0_SUB)))
    {
     #if ENABLE_MULTI_WINDOW_SETTINGS_MENU
       if(USER_PREF_MULTI_WIN_MODE==eMULTIWIN_OFF)
     #else
	 if(1)
     #endif
        return V_FREQ_IN(MapScaler2Port(SCL0_SUB));
	else
        return 60;
     }	
     else if(!SyncLossState(MapScaler2Port(SCR0_MAIN)))
    {
     #if ENABLE_MULTI_WINDOW_SETTINGS_MENU
       if(USER_PREF_MULTI_WIN_MODE==eMULTIWIN_OFF)
     #else
	 if(1)
     #endif
        return V_FREQ_IN(MapScaler2Port(SCR0_MAIN));
	else
        return 60;
     }	
     else if(!SyncLossState(MapScaler2Port(SCR0_SUB)))
    {
     #if ENABLE_MULTI_WINDOW_SETTINGS_MENU
       if(USER_PREF_MULTI_WIN_MODE==eMULTIWIN_OFF)
     #else
	 if(1)
     #endif
        return V_FREQ_IN(MapScaler2Port(SCR0_SUB));
	else
        return 60;
     }	
     else
         return (PanelVfreq/10);		 	
#elif (PanelType==PanelM270DTN01||PanelType==PanelLTM340YP01)
    if((!SyncLossState(MapScaler2Port(SCL0_MAIN)))
  #if ENABLE_MULTI_WINDOW_SETTINGS_MENU
    &&(USER_PREF_MULTI_WIN_MODE==eMULTIWIN_OFF)
  #endif
	)
    {
    #if (PanelType==PanelLTM340YP01)
      if((V_FREQ_IN(MapScaler2Port(SCL0_MAIN))>=110)||(V_FREQ_IN(MapScaler2Port(SCL0_MAIN))<=40))
        return (PanelVfreq/10);	
      else		
    #else
      if(V_FREQ_IN(MapScaler2Port(SCL0_MAIN))>130)
        return (PanelVfreq/10);	
      else		
    #endif	  	
        return V_FREQ_IN(MapScaler2Port(SCL0_MAIN));
     }
  #if (PanelType==PanelLTM340YP01)
    else if(V_FREQ_IN(MapScaler2Port(SCL0_MAIN))<=40)
        return (PanelVfreq/10);	
   #endif	
    else
        return (PanelVfreq/10);	
#else
#if INPUT_SAME_OUT
   WORD u16InputVfreq=0;
   WORD hFreq=0;
   WORD vFreq=0;
   WORD hFreqTemp=0;
   WORD vFreqTemp=0;
   BYTE u8DetSCIndex = MapPort2DetScIndex(USER_PREF_INPUT_TYPE);
   // vFreq = msDrvIP1ReadVtotal(u8DetSCIndex);//_msRead2Byte(SC0_E2) & 0xFFF;
    hFreqTemp=msDrvIP1ReadHperiod(u8DetSCIndex);
    hFreq = HFREQ( hFreqTemp );
    vFreqTemp=msDrvIP1ReadVtotal(u8DetSCIndex);
    vFreq = VFREQ( hFreq,   vFreqTemp );
     u16InputVfreq=vFreq;
  // u16InputVfreq=VFREQ( HFREQ( SrcHPeriod(MapWin2Port(USER_PREF_WIN_SEL)) ), SrcVTotal(MapWin2Port(USER_PREF_WIN_SEL)) );
	  // drvmStar_printData("@@@@@@@@@@ u16InputVfreq=%d",u16InputVfreq);
	if(UnsupportedModeFlag(USER_PREF_WIN_SRC(DISPLAY_WIN1)))  //   160712_04
	{
	 #if ENABLE_MULTI_WINDOW_SETTINGS_MENU
	    if(USER_PREF_MULTI_WIN_MODE!=eMULTIWIN_OFF)
	    {
             u16InputVfreq=600;
     	      return u16InputVfreq/10;   
	    }
	    else	
	#endif		
		return PanelVfreq / 10;    //V_FREQ_IN(SCDetWin);
	}
	else if( u16InputVfreq < (PanelMinVfreq) || u16InputVfreq > (PanelMaxVfreq) )
	{
	   //drvmStar_printData("OUTOFRANGE  IN PUT==%d",u16InputVfreq);
	 #if ENABLE_MULTI_WINDOW_SETTINGS_MENU
	    if(USER_PREF_MULTI_WIN_MODE!=eMULTIWIN_OFF)
	    {
             u16InputVfreq=600;
     	      return u16InputVfreq/10;   
	    }
	    else	
	#endif		
     	      return PanelVfreq/10;    //V_FREQ_IN(SCDetWin);
       }
       else
       {
	   //drvmStar_printData("OUT BY IN PUT==%d",u16InputVfreq);
	   #if 1//QHD_2560x1440_FreeSync_FrameSyncMode
	      return u16InputVfreq/10+(u16InputVfreq%10)/5;// u16InputVfreq/10+1;//u16InputVfreq/10;    //V_FREQ_IN(SCDetWin);
	   #else
   	      return u16InputVfreq/10;    //V_FREQ_IN(SCDetWin);
	   #endif
       }
#else
    WORD u16OutputVfreq=0;
    if((g_DisplayWindow[DISPLAY_WIN1].bDwReadCurFrame) && (!SyncLossState(USER_PREF_WIN_SRC(DISPLAY_WIN1))))
        u16OutputVfreq= GetInputVfreq(DISPLAY_WIN1)/10;
    else
        u16OutputVfreq= PanelVfreq/10;    //V_FREQ_IN(SCDetWin);

	   //drvmStar_printData("OUTPUT VFreq==%d",u16OutputVfreq);
	return u16OutputVfreq;
#endif

	
#endif 




   
}

WORD GetCurrentInputVfreq(void)
{
	BYTE nDetScIndex = MapPort2DetScIndex ( MapWin2Port ( DISPLAY_WIN1 ) );

	WORD xdata VFreq =(GetInputVfreq(DISPLAY_WIN1))/10;

	if (INPUT_IS_DRR(MapWin2Port(DISPLAY_WIN1))&&INPUT_IS_HDMI(MapWin2Port ( DISPLAY_WIN1 ) ))
	{

	}
	else
	{
		if ( !INPUT_IS_VGA ( MapWin2Port ( DISPLAY_WIN1 ) ) )
		{
			if ( ( msDrvIP1ReadSyncStatus ( nDetScIndex ) &INTM_B ) //_msReadByte( SC0_E1 ) &INTM_B
#if (ENABLE_DP_INPUT && (CHIP_ID!=MST9U3))
			|| ( INPUT_IS_DISPLAYPORT ( MapWin2Port ( DISPLAY_WIN1 ) ) && ( DPRxGetVB_ID ( MapWin2Port ( DISPLAY_WIN1 ) ) ) )
#endif
			)
			{
				VFreq*=2;
				printf("__Why_Is Interlace Mode__%d\n", VFreq);
			}
		}
		else
		{
			if((InterlaceModeFlag( MapWin2Port ( DISPLAY_WIN1 ) )))
			{
				VFreq*=2;
			}
		}
	}
	printf("__789789789789789789__%d\n", VFreq);
    return VFreq;
}


WORD GetInputVfreq(BYTE u8WinIndex)
{
#if 1//(PanelType==PanelLSM315HP01||PanelType==PanelLSM315DP01)
    WORD u16InputVfreq=0;

    if(g_NotSettingLPLL)  //  160714_11
    {
	    WORD hFreq = 0;
	    WORD vFreq = 0;
	    WORD hFreqTemp=0;
	    WORD vFreqTemp=0;

	    //BYTE u8DetSCIndex = MapPort2DetScIndex(USER_PREF_INPUT_TYPE);
	    BYTE u8DetSCIndex = MapPort2DetScIndex(MapWin2Port(u8WinIndex));

	    // vFreq = msDrvIP1ReadVtotal(u8DetSCIndex);//_msRead2Byte(SC0_E2) & 0xFFF;
#if 1
	    hFreqTemp=msDrvIP1ReadHperiod(u8DetSCIndex);
	    hFreq = HFREQ( hFreqTemp );
	    vFreqTemp=msDrvIP1ReadVtotal(u8DetSCIndex);
	    vFreq = VFREQ( hFreq,   vFreqTemp );
#else
	hFreq = HFREQ(msDrvIP1ReadHperiod(u8DetSCIndex));
	vFreq = VFREQ(hFreq, msDrvIP1ReadVtotal(u8DetSCIndex));
#endif
	    u16InputVfreq = vFreq;
	    printf("__g_NotSettingLPLL_GetInputVfreq__DRR__%d\n",u16InputVfreq);
    }
    else
    {

         if (INPUT_IS_DRR(MapWin2Port(u8WinIndex)))
         {
             u16InputVfreq=msAPISrcGetDRRFPS()*10;
         }
         else
             u16InputVfreq=VFREQ( HFREQ( SrcHPeriod(MapWin2Port(u8WinIndex)) ), SrcVTotal(MapWin2Port(u8WinIndex)) );
    }
    return u16InputVfreq;

#else

    WORD u16InputVfreq=0;
    if (INPUT_IS_DRR(MapWin2Port(u8WinIndex)))
    {
        u16InputVfreq=msAPISrcGetDRRFPS()*10;
    }
    else
        u16InputVfreq=VFREQ( HFREQ( SrcHPeriod(MapWin2Port(u8WinIndex)) ), SrcVTotal(MapWin2Port(u8WinIndex)) );
    return u16InputVfreq;
#endif
}

void msDrvFrameSyncSrcSel(BYTE u8SCSel)
{
    DWORD u32SCRegBase = SCRegBase(u8SCSel);

    if(u8SCSel<SCR0_SUB)    //L0
        msWriteByteMask(SCL0_00_85, 0x00, BIT6|BIT7);
#if (CHIP_ID==MST9U3)       //R0
    else
        msWriteByteMask(SCL0_00_85, BIT7, BIT6|BIT7);
#else   // MST9U/MST9U2
    else if(u8SCSel<SCL1_SUB)   //R0
        msWriteByteMask(SCL0_00_85, BIT7, BIT6|BIT7);
    else if(u8SCSel<SCR1_SUB)   //L1
        msWriteByteMask(SCL0_00_85, BIT6, BIT6|BIT7);
    else                        //R1
        msWriteByteMask(SCL0_00_85, BIT6|BIT7, BIT6|BIT7);
#endif

    if(u8SCSel&BIT0)    //main scaler
    {
        msWriteByteMask(u32SCRegBase+SC12_07, 0x00, BIT6);
    }
    else                //sub scaler
    {
        msWriteByteMask(u32SCRegBase+SC12_07, BIT6, BIT6);
    }
    msDrvIP1SetOutputLockMode(TRUE, u8SCSel);

    msDrvSetFrameDivider(V_FREQ_IN(MapScaler2Port(u8SCSel)));
    msDrvResetDClkPLL();

    if( g_DisplayWindow[MapScaler2Win(u8SCSel)].bDwReadCurFrame )
    {
        BYTE SCDetWinMask = g_DisplayWindow[MapScaler2Win(u8SCSel)].u16DwScMaskOut;
        BYTE u8RWBankMode = 0;

        if(!msDrvSetOvs(u8SCSel))
        {
            if( g_DisplayWindow[MapScaler2Win(u8SCSel)].u8DwMemFormat == MEM_FMT_444_12 )
                u8RWBankMode = 0x40;
            else
                u8RWBankMode = 0x20;
        }
        msDrvSCCommonWrite(SCDetWinMask, ENABLE);
        if(u8SCSel & BIT0) // MAIN
            msWriteByteMask(u32SCRegBase+SC12_0F, u8RWBankMode, 0xE0);
        else
            msWriteByteMask(u32SCRegBase+SC12_8F, u8RWBankMode, 0xE0);
        msDrvSCCommonWrite(SCDetWinMask, DISABLE);
    }

    if(g_ScalerInfo[SCL0_MAIN].u8SiSyncMode == FLM_FB_FAST_LOCK)
        msDrvSetFpllFastLock( u8SCSel );
}

void msDrvSetAutoDownload(ADL_CLIENT_ID eClient_id, BOOL bEnable, ADL_MODE eMode, DWORD u32Addr, WORD u16Length)
{
    switch(eClient_id)
    {
        case ADL_GAMMA_TBL:
        {
            #if CHIP_ID==MST9U4
            msWriteBit(SCL0_67_02, eMode, BIT3);
            msWriteBit(SCL0_67_02, bEnable, BIT0);
            msWrite2Byte(SCL0_67_04, u32Addr>>4);
            msWrite2Byte(SCL0_67_06, u32Addr>>20);
            msWrite2Byte(SCL0_67_10, u16Length);
            msWrite2Byte(SCL0_67_16, u16Length);
            #else //MST9U,MST9U2,MST9U3
            msWriteBit(SCL0_5A_02, eMode, BIT3);
            msWriteBit(SCL0_5A_02, bEnable, BIT0);
            msWrite2Byte(SCL0_5A_04, u32Addr>>4);
            msWrite2Byte(SCL0_5A_06, u32Addr>>20);
            msWrite2Byte(SCL0_5A_10, u16Length);
            msWrite2Byte(SCL0_5A_16, u16Length);
            #endif
        }
        break;
        case ADL_OD_TBL:
        {
            #if CHIP_ID==MST9U4
            msWriteBit(SCL0_67_02, eMode, BIT4);
            msWriteBit(SCL0_67_02, bEnable, BIT1);//client2_1
            msWriteBit(SCL0_67_42, bEnable, BIT0);//client2_2
            msWriteBit(SCL0_67_42, bEnable, BIT1);//client2_3
            msWrite2Byte(SCL0_67_08, u32Addr>>4);
            msWrite2Byte(SCL0_67_0A, u32Addr>>20);
            msWrite2Byte(SCL0_67_12, u16Length);
            msWrite2Byte(SCL0_67_18, u16Length);
            #else //MST9U,MST9U2,MST9U3
            msWriteBit(SCL0_5A_02, eMode, BIT4);
            msWriteBit(SCL0_5A_02, bEnable, BIT1);//client2_1
            msWriteBit(SCL0_5A_42, bEnable, BIT0);//client2_2
            msWriteBit(SCL0_5A_42, bEnable, BIT1);//client2_3
            msWrite2Byte(SCL0_5A_08, u32Addr>>4);
            msWrite2Byte(SCL0_5A_0A, u32Addr>>20);
            msWrite2Byte(SCL0_5A_12, u16Length);
            msWrite2Byte(SCL0_5A_18, u16Length);
            #endif
        }
        break;
        case ADL_LOCAL_DIM_TBL:
        {
            #if CHIP_ID==MST9U4
            msWriteBit(SCL0_67_02, eMode, BIT5);
            msWriteBit(SCL0_67_02, bEnable, BIT2);
            msWrite2Byte(SCL0_67_0C, u32Addr>>4);
            msWrite2Byte(SCL0_67_0E, u32Addr>>20);
            msWrite2Byte(SCL0_67_14, u16Length);
            msWrite2Byte(SCL0_67_1A, u16Length);
            #else //MST9U,MST9U2,MST9U3
            msWriteBit(SCL0_5A_02, eMode, BIT5);
            msWriteBit(SCL0_5A_02, bEnable, BIT2);
            msWrite2Byte(SCL0_5A_0C, u32Addr>>4);
            msWrite2Byte(SCL0_5A_0E, u32Addr>>20);
            msWrite2Byte(SCL0_5A_14, u16Length);
            msWrite2Byte(SCL0_5A_1A, u16Length);
            #endif
        }
        break;
        case ADL_OD_GAMMA:
        {
            #if CHIP_ID==MST9U4
            msWriteBit(SCL0_67_02, eMode, BIT6);
            msWriteBit(SCL0_67_22, bEnable, BIT0);
            msWrite2Byte(SCL0_67_24, u32Addr>>4);
            msWrite2Byte(SCL0_67_26, u32Addr>>20);
            msWrite2Byte(SCL0_67_30, u16Length);
            msWrite2Byte(SCL0_67_36, u16Length);
            #else //MST9U,MST9U2,MST9U3
            msWriteBit(SCL0_5A_02, eMode, BIT6);
            msWriteBit(SCL0_5A_22, bEnable, BIT0);
            msWrite2Byte(SCL0_5A_24, u32Addr>>4);
            msWrite2Byte(SCL0_5A_26, u32Addr>>20);
            msWrite2Byte(SCL0_5A_30, u16Length);
            msWrite2Byte(SCL0_5A_36, u16Length);
            #endif
        }
        break;
        case ADL_OD_DEGAMMA:
        {
            #if CHIP_ID==MST9U4
            msWriteBit(SCL0_67_02, eMode, BIT7);
            msWriteBit(SCL0_67_22, bEnable, BIT1);
            msWrite2Byte(SCL0_67_28, u32Addr>>4);
            msWrite2Byte(SCL0_67_2A, u32Addr>>20);
            msWrite2Byte(SCL0_67_32, u16Length);
            msWrite2Byte(SCL0_67_38, u16Length);
            #else //MST9U,MST9U2,MST9U3
            msWriteBit(SCL0_5A_02, eMode, BIT7);
            msWriteBit(SCL0_5A_22, bEnable, BIT1);
            msWrite2Byte(SCL0_5A_28, u32Addr>>4);
            msWrite2Byte(SCL0_5A_2A, u32Addr>>20);
            msWrite2Byte(SCL0_5A_32, u16Length);
            msWrite2Byte(SCL0_5A_38, u16Length);
            #endif
        }
        break;
        case ADL_XVYCC:
        {
            #if CHIP_ID==MST9U4
            msWriteBit(SCL0_67_03, eMode, BIT7);
            msWriteBit(SCL0_67_22, bEnable, BIT2);
            msWrite2Byte(SCL0_67_2C, u32Addr>>4);
            msWrite2Byte(SCL0_67_2E, u32Addr>>20);
            msWrite2Byte(SCL0_67_14, u16Length);
            msWrite2Byte(SCL0_67_3A, u16Length);
            #else //MST9U,MST9U2,MST9U3
            msWriteBit(SCL0_5A_03, eMode, BIT7);
            msWriteBit(SCL0_5A_22, bEnable, BIT2);
            msWrite2Byte(SCL0_5A_2C, u32Addr>>4);
            msWrite2Byte(SCL0_5A_2E, u32Addr>>20);
            msWrite2Byte(SCL0_5A_14, u16Length);
            msWrite2Byte(SCL0_5A_3A, u16Length);
            #endif
        }
        break;
        case ADL_GAMMA_1024:
        {
            #if CHIP_ID==MST9U4
            msWriteBit(SCL0_67_44, eMode, BIT1);
            msWriteBit(SCL0_67_44, bEnable, BIT0);
            msWrite2Byte(SCL0_67_46, u32Addr>>4);
            msWrite2Byte(SCL0_67_48, u32Addr>>20);
            msWrite2Byte(SCL0_67_4A, u16Length);
            msWrite2Byte(SCL0_67_4C, u16Length);
            #else //MST9U,MST9U2,MST9U3
            msWriteBit(SCL0_5A_44, eMode, BIT1);
            msWriteBit(SCL0_5A_44, bEnable, BIT0);
            msWrite2Byte(SCL0_5A_46, u32Addr>>4);
            msWrite2Byte(SCL0_5A_48, u32Addr>>20);
            msWrite2Byte(SCL0_5A_4A, u16Length);
            msWrite2Byte(SCL0_5A_4C, u16Length);
            #endif
        }
        break;
        case ADL_PIU_IRE:
        {
            #if CHIP_ID==MST9U4
            msWriteBit(SCL0_67_50, eMode, BIT1);
            msWriteBit(SCL0_67_50, bEnable, BIT0);
            msWrite2Byte(SCL0_67_52, u32Addr>>4);
            msWrite2Byte(SCL0_67_54, u32Addr>>20);
            msWrite2Byte(SCL0_67_36, u16Length);
            msWrite2Byte(SCL0_67_58, u16Length);
            #else //MST9U,MST9U2,MST9U3
            msWriteBit(SCL0_5A_50, eMode, BIT1);
            msWriteBit(SCL0_5A_50, bEnable, BIT0);
            msWrite2Byte(SCL0_5A_52, u32Addr>>4);
            msWrite2Byte(SCL0_5A_54, u32Addr>>20);
            msWrite2Byte(SCL0_5A_36, u16Length);
            msWrite2Byte(SCL0_5A_58, u16Length);
            #endif
        }
        break;
        case ADL_T3D:
        {
            #if CHIP_ID==MST9U4
            msWriteBit(SCL0_67_64, eMode, BIT1);
            msWriteBit(SCL0_67_64, bEnable, BIT0);
            msWrite2Byte(SCL0_67_64, u32Addr>>4);
            msWrite2Byte(SCL0_67_66, u32Addr>>20);
            msWrite2Byte(SCL0_67_6A, u16Length);
            msWrite2Byte(SCL0_67_6C, u16Length);
            #else //MST9U,MST9U2,MST9U3
            msWriteBit(SCL0_5A_64, eMode, BIT1);
            msWriteBit(SCL0_5A_64, bEnable, BIT0);
            msWrite2Byte(SCL0_5A_64, u32Addr>>4);
            msWrite2Byte(SCL0_5A_66, u32Addr>>20);
            msWrite2Byte(SCL0_5A_6A, u16Length);
            msWrite2Byte(SCL0_5A_6C, u16Length);
            #endif
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
#if CHIP_ID==MST9U4
    #if (defined (MST9U_FPGA_1P))
        return FALSE;
    #elif ((PANEL_VBY1_16CH_8Bit)||(PANEL_VBY1_16CH_10Bit))
        #if ((PanelWidth==3840)&&(PanelHeight==2160)&&(PanelVsyncFreq==120))
            return FALSE;
        #elif ((PanelWidth==5120)&&(PanelHeight==2160)&&(PanelVsyncFreq==120))
            return TRUE;
        #else
            return FALSE;
        #endif
    #elif ((PANEL_VBY1_20CH_8Bit)||(PANEL_VBY1_20CH_10Bit)||(PANEL_VBY1_32CH_8Bit)||(PANEL_VBY1_32CH_10Bit))
        return TRUE;
    #else
        return FALSE;
    #endif
#else
    #if (defined MST9U_FPGA_1P)||(MST9U_ASIC_1P==1)
        return FALSE;
    #else
        return TRUE;
    #endif
#endif
}
#if CHIP_ID==MST9U||CHIP_ID==MST9U2||CHIP_ID==MST9U3
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
            msWriteBit(REG_MFT_03, FALSE, BIT7);//MFT enable
            msWriteBit(REG_MFT_01, FALSE, BIT7);
            msWrite2ByteMask(REG_MFT_00, (PanelWidth/2 -1), 0x0FFF);
            break;
        case MFT_LR2OE:
            msWriteBit(REG_MFT_03, TRUE, BIT7);
            msWriteBit(REG_MFT_01, TRUE, BIT7);
            msWrite2ByteMask(REG_MFT_00, (PanelWidth/2 -1), 0x0FFF);
            msWriteByteMask(REG_MFT_07, 0x00, BIT7|BIT6);
            msWriteByteMask(REG_MFT_08, BIT0, BIT0);
            break;
        case MFT_OE2LR:
            msWriteBit(REG_MFT_03, TRUE, BIT7);
            msWriteBit(REG_MFT_01, TRUE, BIT7);
            msWrite2ByteMask(REG_MFT_00, (PanelWidth/2 -1), 0x0FFF);
            msWriteByteMask(REG_MFT_07, BIT7, BIT7|BIT6);
            msWriteByteMask(REG_MFT_08, 0x00, BIT0);
            break;
        case MFT_LR24B:
            msWriteBit(REG_MFT_03, TRUE, BIT7);
            msWriteBit(REG_MFT_01, TRUE, BIT7);
            msWrite2ByteMask(REG_MFT_00, (PanelWidth/2 -1), 0x0FFF);
            msWriteByteMask(REG_MFT_07, BIT6, BIT7|BIT6);
            msWriteByteMask(REG_MFT_08, 0x00, BIT0);
            break;
    }
}
#else //MST9U4
void msDrvMFTSetup(void)
{
    BYTE uctemp = 0, ucMFT_Type=0;
    bit bOutut8PFlag = 0;

    if (msDrvISSCDualPort())
    {
        bOutut8PFlag=TRUE;
    }
    else
    {
        bOutut8PFlag=FALSE;
    }
#if PANEL_TTL
#elif PANEL_LVDS_1CH
#elif PANEL_LVDS_2CH
    ucMFT_Type=E_PNL_SUPPORTED_MFT_LVDS_2CH_FHD60_OE_50to75MHz;          //0
#elif PANEL_LVDS_4CH
    #if PANEL_OUTPUT_FMT_LR
        ucMFT_Type=E_PNL_SUPPORTED_MFT_LVDS_4CH_FHD120_LR_50to75MHz;          //2  4POE to LR
    #else
        ucMFT_Type=E_PNL_SUPPORTED_MFT_LVDS_4CH_FHD120_OE_50to75MHz;          //1  4POE to OE
    #endif
#elif PANEL_LVDS_8CH
#elif PANEL_VBY1_1CH_10Bit||PANEL_VBY1_1CH_8Bit
#elif PANEL_VBY1_2CH_10Bit||PANEL_VBY1_2CH_8Bit
    ucMFT_Type=E_PNL_SUPPORTED_MFT_VBY1_2LANE_1BLK_FHD60_4P_37_5to75MHz;              //3  4POE to OE
#elif PANEL_VBY1_4CH_10Bit||PANEL_VBY1_4CH_8Bit
    #if PANEL_OUTPUT_FMT_LR
        ucMFT_Type=E_PNL_SUPPORTED_MFT_VBY1_4LANE_2BLK_FHD120_4P_37_5to75MHz;          //5  4POE to LR
    #else
        ucMFT_Type=E_PNL_SUPPORTED_MFT_VBY1_4LANE_1BLK_FHD120_4P_37_5to75MHz;          //4  4POE to OE
    #endif
#elif PANEL_VBY1_8CH_10Bit||PANEL_VBY1_8CH_8Bit
    #if PANEL_OUTPUT_FMT_LR
        ucMFT_Type=E_PNL_SUPPORTED_MFT_VBY1_8LANE_2BLK_UHD60_4P_75to150MHz;          //7  4POE to LR
    #elif PANEL_OUTPUT_FMT_4B
        ucMFT_Type=E_PNL_SUPPORTED_MFT_VBY1_8LANE_4BLK_UHD60_4P_75to150MHz;          //8  4POE to 4B
    #else
        ucMFT_Type=E_PNL_SUPPORTED_MFT_VBY1_8LANE_1BLK_UHD60_4P_75to150MHz;          //6  4POE to OE
    #endif
#elif PANEL_VBY1_16CH_10Bit||PANEL_VBY1_16CH_8Bit
    if (bOutut8PFlag==TRUE)
    {
        #if PANEL_OUTPUT_FMT_LR
            ucMFT_Type=E_PNL_SUPPORTED_MFT_VBY1_16LANE_2BLK_5K120_8P_135to195_75MHz;          //14  8PLR to LR
        #elif PANEL_OUTPUT_FMT_4B
            ucMFT_Type=E_PNL_SUPPORTED_MFT_VBY1_16LANE_4BLK_5K120_8P_135to195_75MHz;          //15  8PLR to 4B
        #elif PANEL_OUTPUT_FMT_8B
            ucMFT_Type=E_PNL_SUPPORTED_MFT_VBY1_16LANE_8BLK_5K120_8P_135to195_75MHz;          //16  8PLR to 8B
        #else
            ucMFT_Type=E_PNL_SUPPORTED_MFT_VBY1_16LANE_1BLK_5K120_8P_135to195_75MHz;          //13  8PLR to OE
        #endif
    }
    else
    {
        #if PANEL_OUTPUT_FMT_LR
            ucMFT_Type=E_PNL_SUPPORTED_MFT_VBY1_16LANE_2BLK_UHD120_4P_150to300MHz;          //10  4POE to LR
        #elif PANEL_OUTPUT_FMT_4B
            ucMFT_Type=E_PNL_SUPPORTED_MFT_VBY1_16LANE_4BLK_UHD120_4P_150to300MHz;          //11  4POE to 4B
        #elif PANEL_OUTPUT_FMT_8B
            ucMFT_Type=E_PNL_SUPPORTED_MFT_VBY1_16LANE_8BLK_UHD120_4P_150to300MHz;          //12  4POE to 8B
        #else
            ucMFT_Type=E_PNL_SUPPORTED_MFT_VBY1_16LANE_1BLK_UHD120_4P_150to300MHz;          //9  4POE to OE
        #endif
    }
#elif PANEL_VBY1_20CH_10Bit||PANEL_VBY1_20CH_8Bit
    #if PANEL_OUTPUT_FMT_5B
        ucMFT_Type=E_PNL_SUPPORTED_MFT_VBY1_20LANE_5BLK_5K120_8P_195_75to391_5MHz;          //18  8PLR to 5B
    #elif PANEL_OUTPUT_FMT_10B
        ucMFT_Type=E_PNL_SUPPORTED_MFT_VBY1_20LANE_10BLK_5K120_8P_195_75to391_5MHz;         //19  8PLR to 10B
    #else
        ucMFT_Type=E_PNL_SUPPORTED_MFT_VBY1_20LANE_1BLK_5K120_8P_195_75to391_5MHz;          //17  8PLR to OE
    #endif
#elif PANEL_VBY1_32CH_10Bit||PANEL_VBY1_32CH_8Bit
    #if PANEL_OUTPUT_FMT_LR
        ucMFT_Type=E_PNL_SUPPORTED_MFT_VBY1_32LANE_2BLK_8K60_8P_225to300MHz;          //21  8PLR to LR
    #elif PANEL_OUTPUT_FMT_4B
        ucMFT_Type=E_PNL_SUPPORTED_MFT_VBY1_32LANE_4BLK_8K60_8P_225to300MHz;          //22  8PLR to 4B
    #elif PANEL_OUTPUT_FMT_8B
        ucMFT_Type=E_PNL_SUPPORTED_MFT_VBY1_32LANE_8BLK_8K60_8P_225to300MHz;          //23  8PLR to 8B
    #else
        ucMFT_Type=E_PNL_SUPPORTED_MFT_VBY1_32LANE_1BLK_8K60_8P_225to300MHz;          //20  8PLR to OE
    #endif
#endif

    for(uctemp = 0; uctemp <MFT_REG_NUM; uctemp++)
    {
        msWrite2ByteMask(MFT_REG_ADDRESS(MFTSettingTBL[ucMFT_Type][uctemp].address), MFTSettingTBL[ucMFT_Type][uctemp].value, MFTSettingTBL[ucMFT_Type][uctemp].mask);

        //msWrite2ByteMask(((MFTSettingTBL[ucMFT_Type][uctemp].address)&0xFFFF00+(MFTSettingTBL[ucMFT_Type][uctemp].address&0x0000FF)*2), MFTSettingTBL[ucMFT_Type][uctemp].value, MFTSettingTBL[ucMFT_Type][uctemp].mask);
    }
}
#endif
void msDrvSetComboMux(BYTE InputPort, BYTE SCDetWin)
{
    BYTE u8ComboMux = (g_InputPort[InputPort].eIPMux - 1);
    BYTE u8ComboCLK = (g_InputPort[InputPort].eIPMux - 1);
    //Combo Mux Select

#if (CHIP_ID == MST9U) || (CHIP_ID == MST9U2)

    //if((g_InputPort[InputPort].eIPType)==TYPE_DP)//For Vpll selection

#if 0 // board defines the master mux of dual link, combo driver will do the switch.
    if(g_InputPort[InputPort].eIPType==TYPE_DVI_DUAL)
    {
        u8ComboMux &= 0xFE;//DVI dual link is select combo0, combo2, and combo4
        u8ComboCLK &= 0xFE;//DVI dual link is select combo0, combo2, and combo4
    }
    else
#endif
    if(INPUT_IS_HBR2(InputPort))
    {
        u8ComboMux = 0;
        if(g_ScalerInfo[SCDetWin].u8SiIPHBR2LR)// 0: L, 1:R
        {
            u8ComboMux+=7;
        }
        else
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
    else if (INPUT_IS_DISPLAYPORT(InputPort))
    {
        u8ComboCLK = 8;     // vpll2 for DP
    }

    if( INPUT_IS_DVI_D(InputPort)
    && (    msAPI_combo_IPGetTmdsIsDVIDualLink((EN_COMBO_IP_SELECT)g_InputPort[InputPort].eIPMux)
         || msAPI_combo_IPGetTmdsIsDVIDualLink((EN_COMBO_IP_SELECT)(g_InputPort[InputPort].eIPMux+1))
       )
    ){
        u8ComboMux &= (~BIT0);
        u8ComboCLK &= (~BIT0);
    }

    switch(SCDetWin)
    {
        case SCL0_SUB:
            msWriteByteMask(REG_1630E8, u8ComboMux<<4, 0xF0);
            msWriteByteMask(REG_1630F0, u8ComboCLK<<4, 0xF0);
            msWriteByteMask(REG_120F33, u8ComboCLK, 0x0F);
            break;
        case SCL0_MAIN:
            msWriteByteMask(REG_1630E8, u8ComboMux, 0x0F);
            msWriteByteMask(REG_1630F0, u8ComboCLK, 0x0F);
            msWriteByteMask(REG_120F40, u8ComboCLK, 0x0F);
            break;
        case SCR0_SUB:
            msWriteByteMask(REG_1630EB, u8ComboMux, 0x0F);
            msWriteByteMask(REG_1630F3, u8ComboCLK, 0x0F);
            msWriteByteMask(REG_120F43, u8ComboCLK, 0x0F);
            break;
        case SCR0_MAIN:
            msWriteByteMask(REG_1630EA, u8ComboMux<<4, 0xF0);
            msWriteByteMask(REG_1630F2, u8ComboCLK<<4, 0xF0);
            msWriteByteMask(REG_120F44, u8ComboCLK, 0x0F);
            break;
        case SCL1_SUB:
            msWriteByteMask(REG_1630EA, u8ComboMux, 0x0F);
            msWriteByteMask(REG_1630F2, u8ComboCLK, 0x0F);
            msWriteByteMask(REG_120F41, u8ComboCLK, 0x0F);
            break;
        case SCL1_MAIN:
            msWriteByteMask(REG_1630E9, u8ComboMux<<4, 0xF0);
            msWriteByteMask(REG_1630F1, u8ComboCLK<<4, 0xF0);
            msWriteByteMask(REG_120F42, u8ComboCLK, 0x0F);
            break;
        case SCR1_SUB:
            msWriteByteMask(REG_1630EC, u8ComboMux, 0x0F);
            msWriteByteMask(REG_1630F4, u8ComboCLK, 0x0F);
            msWriteByteMask(REG_120F45, u8ComboCLK, 0x0F);
            break;
        case SCR1_MAIN:
            msWriteByteMask(REG_1630EB, u8ComboMux<<4, 0xF0);
            msWriteByteMask(REG_1630F3, u8ComboCLK<<4, 0xF0);
            msWriteByteMask(REG_120F46, u8ComboCLK, 0x0F);
            break;
        case SCL0_OFFLINE:
            msWriteByteMask(REG_1630E9, u8ComboMux, 0x0F);
            msWriteByteMask(REG_1630F1, u8ComboCLK, 0x0F);
            msWriteByteMask(REG_120F32, u8ComboCLK, 0x0F);
            break;
    }

#elif (CHIP_ID == MST9U3)

#if 0 // board defines the master mux of dual link, combo driver will do the switch.
    if(g_InputPort[InputPort].eIPType==TYPE_DVI_DUAL)
    {
        u8ComboMux &= 0xFE;//DVI dual link is select combo0, combo2, and combo4
        u8ComboCLK &= 0xFE;//DVI dual link is select combo0, combo2, and combo4
    }
    else
#endif
    {
        BYTE u8Combo2SC_CLK =   ((SCDetWin==SCL0_MAIN)?BIT0:\
                                ((SCDetWin==SCL0_SUB))?BIT1:\
                                ((SCDetWin==SCL0_OFFLINE))?BIT2:\
                                ((SCDetWin==SCR0_MAIN))?BIT3:BIT4);
        msWriteByteMask(REG_162DEE, u8Combo2SC_CLK, u8Combo2SC_CLK);
    }
    if(g_InputPort[InputPort].eIPMux == MUX_COMBO3)
    {
        u8ComboMux--;
        u8ComboCLK--;
    }
    if(INPUT_IS_HBR2(InputPort))
    {
        if(g_ScalerInfo[SCDetWin].u8SiIPHBR2LR)// 0: L, 1:R
        {
            u8ComboMux++;//MUX_COMBO5
        }
    }
    else if(INPUT_IS_HDMI_2_0(InputPort))
    {
        if(HBR2Flag(InputPort))
        {
            u8ComboMux = 6;
            if(g_ScalerInfo[SCDetWin].u8SiIPHBR2LR)//R
                u8ComboMux++;
            if(g_InputPort[InputPort].bIPInputSplitter23)//Splitter 23
                u8ComboMux+=2;
        }
    }
    if (INPUT_IS_DISPLAYPORT(InputPort))
        u8ComboCLK = 6;

    switch(SCDetWin)
    {
        case SCL0_SUB:
            msWriteByteMask(REG_162DE8, u8ComboMux<<4, 0xF0);
            msWriteByteMask(REG_162DF0, u8ComboCLK<<4, 0xF0);
            msWriteByteMask(REG_120F33, u8ComboCLK, 0x0F);
            break;
        case SCL0_MAIN:
            msWriteByteMask(REG_162DE8, u8ComboMux, 0x0F);
            msWriteByteMask(REG_162DF0, u8ComboCLK, 0x0F);
            msWriteByteMask(REG_120F40, u8ComboCLK, 0x0F);
            break;
        case SCR0_SUB:
            msWriteByteMask(REG_162DEA, u8ComboMux, 0x0F);
            msWriteByteMask(REG_162DF2, u8ComboCLK, 0x0F);
            msWriteByteMask(REG_120F43, u8ComboCLK, 0x0F);
            break;
        case SCR0_MAIN:
            msWriteByteMask(REG_162DE9, u8ComboMux<<4, 0xF0);
            msWriteByteMask(REG_162DF1, u8ComboCLK<<4, 0xF0);
            msWriteByteMask(REG_120F44, u8ComboCLK, 0x0F);
            break;
        case SCL0_OFFLINE:
            msWriteByteMask(REG_162DE9, u8ComboMux, 0x0F);
            msWriteByteMask(REG_162DF1, u8ComboCLK, 0x0F);
            msWriteByteMask(REG_120F32, u8ComboCLK, 0x0F);
            break;
    }
#elif (CHIP_ID == MST9U4)
    SCDetWin = SCDetWin;
    u8ComboMux = u8ComboMux;
    u8ComboCLK = u8ComboCLK;
#endif
}

void msDrvSetIPCLKMux(BYTE u8IDCLK, BYTE SCDetWin)
{
    switch(SCDetWin)
    {
        case SCL0_SUB:
            msWriteByteMask(REG_120F19,u8IDCLK<<2,BIT2|BIT3);
            break;
        case SCL0_MAIN:
            msWriteByteMask(REG_120F1A,u8IDCLK<<2,BIT2|BIT3);
            break;
        case SCR0_SUB:
            msWriteByteMask(REG_120F1D,u8IDCLK<<2,BIT2|BIT3);
            break;
        case SCR0_MAIN:
            msWriteByteMask(REG_120F1E,u8IDCLK<<2,BIT2|BIT3);
            break;
        case SCL1_SUB:
            msWriteByteMask(REG_120F1B,u8IDCLK<<2,BIT2|BIT3);
            break;
        case SCL1_MAIN:
            msWriteByteMask(REG_120F1C,u8IDCLK<<2,BIT2|BIT3);
            break;
        case SCR1_SUB:
            msWriteByteMask(REG_120F2E,u8IDCLK<<2,BIT2|BIT3);
            break;
        case SCR1_MAIN:
            msWriteByteMask(REG_120F2F,u8IDCLK<<2,BIT2|BIT3);
            break;
        case SCL0_OFFLINE:
            msWriteByteMask(REG_120F18,u8IDCLK<<2,BIT2|BIT3);
            break;
    }
}

BYTE msDrvGetIPCLKMux(BYTE SCDetWin)
{
    BYTE u8IDCLK = ID_CLK_ODCLK;

    switch(SCDetWin)
    {
        case SCL0_SUB:
            u8IDCLK = (msReadByte(REG_120F19)&(BIT2|BIT3))>>2;
            break;
        case SCL0_MAIN:
            u8IDCLK = (msReadByte(REG_120F1A)&(BIT2|BIT3))>>2;
            break;
        case SCR0_SUB:
            u8IDCLK = (msReadByte(REG_120F1D)&(BIT2|BIT3))>>2;
            break;
        case SCR0_MAIN:
            u8IDCLK = (msReadByte(REG_120F1E)&(BIT2|BIT3))>>2;
            break;
        case SCL1_SUB:
            u8IDCLK = (msReadByte(REG_120F1B)&(BIT2|BIT3))>>2;
            break;
        case SCL1_MAIN:
            u8IDCLK = (msReadByte(REG_120F1C)&(BIT2|BIT3))>>2;
            break;
        case SCR1_SUB:
            u8IDCLK = (msReadByte(REG_120F2E)&(BIT2|BIT3))>>2;
            break;
        case SCR1_MAIN:
            u8IDCLK = (msReadByte(REG_120F2F)&(BIT2|BIT3))>>2;
            break;
        case SCL0_OFFLINE:
            u8IDCLK = (msReadByte(REG_120F18)&(BIT2|BIT3))>>2;
            break;
    }

    return u8IDCLK;
}

void msDrvSetIPMux(BYTE InputPort, BYTE SCDetWin) //  MST9U4_TBD
{
#ifdef MST9U_FPGA
#if (CHIP_ID==MST9U) || (CHIP_ID==MST9U2)
    if(InputPort == Input_Analog1)
    {
        if(SCDetWin == SCR0_MAIN)
        {
            msWrite2Byte(REG_102A02,0x0010); // ip mux
        }
        else
        {
            msWrite2Byte(REG_102E02,0x0010); // ip mux
        }
    }
    else
    {
        if(SCDetWin == SCR0_MAIN)
        {
            msWrite2Byte(REG_102A02,0x0000); // ip mux
        }
        else
        {
            msWrite2Byte(REG_102E02,0x0000); // ip mux
        }
    }
    msWrite2Byte(SCIP1RegBase(SCDetWin)+SC01_04,0x0104); // video/source sel
#elif (CHIP_ID==MST9U3)
    if((SCDetWin%SCL0_OFFLINE) < SCR0_SUB) // SC L
    {
        msWrite2ByteMask(REG_102E02+((SCDetWin==SCL0_OFFLINE)?1:0),(SCDetWin&BIT0)?BIT4:BIT0,(SCDetWin&BIT0)?BIT4:BIT0); // ip mux
    }
    else // SC R
    {
        msWrite2ByteMask(REG_102A02,(SCDetWin&BIT0)?BIT4:BIT0,(SCDetWin&BIT0)?BIT4:BIT0);
    }
#elif (CHIP_ID==MST9U4)
    InputPort = InputPort;
    SCDetWin = SCDetWin;

    // 480i script IPmux
    msWriteByte(0x130204, 0x07);//msWriteByte(0x130204, 0x08);
    //msWriteByte(0x130205, 0x50);
    msWriteByte(0x130206, 0x98);
    msWriteByte(0x130207, 0x80);
    //msWriteByte(0x130208, 0x24);
    //msWriteByte(0x130209, 0x00);
    //msWriteByte(0x13020a, 0x38);
    //msWriteByte(0x13020b, 0x00);
    //msWriteByte(0x13020c, 0xe0);
    //msWriteByte(0x13020d, 0x01);
    //msWriteByte(0x13020e, 0xd0);
    //msWriteByte(0x13020f, 0x02);
    //msWriteByte(0x130212, 0x04);
    //msWriteByte(0x130216, 0xdf);
    //msWriteByte(0x130238, 0x30);
    //msWriteByte(0x130242, 0x03);
    //msWriteByte(0x130243, 0xc1);
    //msWriteByte(0x130200, 0x01);
    //msWriteByte(0x130208, 0x1d);
    //msWriteByte(0x130209, 0x00);
    //msWriteByte(0x13020a, 0x38);
    //msWriteByte(0x13020b, 0x00);
    //msWriteByte(0x13020e, 0xd0);
    //msWriteByte(0x13020f, 0x02);

    //msWriteByte(0x130306, 0x03);
    //msWriteByte(0x13031e, 0x3e);
    //msWriteByte(0x13031f, 0x79);
    //msWriteByte(0x130320, 0x32);
    //msWriteByte(0x13031e, 0x3e);
    //msWriteByte(0x13031f, 0x79);
    //msWriteByte(0x130320, 0x32);
    //msWriteByte(0x130302, 0x00);
    //msWriteByte(0x130303, 0x07);
    //msWriteByte(0x13028a, 0x00);
    //msWriteByte(0x130295, 0x01);
    //msWriteByte(0x130306, 0x03);
    //msWriteByte(0x13031e, 0x3e);
    //msWriteByte(0x13031f, 0x79);
    //msWriteByte(0x130320, 0x32);
    //msWriteByte(0x13031e, 0x3e);
    //msWriteByte(0x13031f, 0x79);
    //msWriteByte(0x130320, 0x32);
    //msWriteByte(0x130302, 0x00);
    //msWriteByte(0x130303, 0x07);
#endif
#else
    DWORD u32SCRegBase = SCRegBase(SCDetWin);
    DWORD u32IPMuxRegBase = u32SCRegBase - 0x100;
    BYTE u8IPMuxShift, u8IPRst, u8RegOffset;
    BYTE u8Sync = 0, u8Mux = 0, u8VD = 0, bInput10b = 0, u8RegSc1_48 = 0, u8RegSc1_49 = 0, u8RegSc1_4A = 0, u8RegSc1_4C = 0;
    BYTE u8RegSc1_42 = 0,u8RegSc1_3B =0, u8RegSc1_4E =0x04, u8RegSc1_5E = 0;
    WORD u8IPMuxMask;

    if(SCDetWin==SCL0_OFFLINE)
    {
        u8IPMuxShift = 0x08;
        u8IPMuxMask = 0x0F00;
        u8IPRst = RST_IP_OFFLINE;
    }
    else if( SCDetWin & BIT0 )//Main
    {
        u8IPMuxShift = 0x04;
        u8IPMuxMask = 0x00F0;
        u8IPRst = RST_IP_MAIN;
    }
    else//Sub
    {
        u8IPMuxShift = 0x00;
        u8IPMuxMask = 0x000F;
        u8IPRst = RST_IP_SUB;
    }

    if( g_InputPort[InputPort].eIPType & TYPE_ANALOG )
    {
        u8Sync = _BIT7|_BIT3;
        if(g_bInputSOGFlag) // current ADC setting is for SOG
        {
            u8Mux = NIS_B|ISEL_ANALOG|SCALER_STYPE_SOG;
        }
        else
        {
            u8Mux = NIS_B|ISEL_ANALOG|SCALER_STYPE_AUTO;
        }
        u8VD = 0;
        bInput10b = 1;
        u8RegSc1_48 = 0x01;
        u8RegSc1_49 = 0x00;
        u8RegSc1_4A = 0x00;
        u8RegSc1_4C = 0x00;
        u8RegSc1_42 = 0x00;
        u8RegSc1_3B = BIT5;
        msWrite2ByteMask(u32IPMuxRegBase+0x02,(WORD)IP_MUX_ADC<<u8IPMuxShift,u8IPMuxMask);
        msDrvSetIPCLKMux(ID_CLK_ADC, SCDetWin);
        msWriteByteMask(REG_1025CF,0x00,BIT0|BIT1);
    }
    else if( g_InputPort[InputPort].eIPType & (TYPE_DVI_SINGLE|TYPE_DVI_DUAL|TYPE_HDMI|TYPE_MHL) )
    {
#if Enable_Auto_Switch_DE_Mode    
      if(FreeSyncFlag)
      {
         u8Sync = _BIT7| _BIT3 | _BIT1 | _BIT0;
      }
      else
      {
         u8RegSc1_5E = _BIT4;
         u8Sync = _BIT7|_BIT6 | _BIT3 | _BIT1 | _BIT0;
      }
#else	
    #if ENABLE_DEONLY_MODE
        u8RegSc1_5E = _BIT4;
        u8Sync = _BIT7|_BIT6 | _BIT3 | _BIT1 | _BIT0;
    #else
        u8Sync = _BIT7| _BIT3 | _BIT1 | _BIT0;
    #endif
 #endif	
        u8Mux = NIS_B|ISEL_COMBO|SCALER_STYPE_HV;
        u8VD = 0;
        bInput10b = 1;
        u8RegSc1_48 = 0x00;
        u8RegSc1_49 = 0x01;//for 720p30
        u8RegSc1_4C = 0xF0;
        #if ENABLE_FORCE_4K_PMODE
            if(INPUT_IS_HDMI_2_0(InputPort)&& HBR2Flag(InputPort))
            {
                u8RegSc1_42 = 0x00|BIT0; //force to P mode
            }
            else
            {
                u8RegSc1_42 = 0x00;
            }
        #else
            u8RegSc1_42 = 0x00;
        #endif
        #if ENABLE_HV_TOLERANCE_ENLARGE
            u8RegSc1_3B = BIT4|BIT5|V_IntTorlance; //reg_vs_tol(BIT3~0) and disable AutoNoSignal(BIT4)

        #else
            u8RegSc1_3B = BIT4|BIT5; //reg_htt_filtermd ,reg_auto_no_signal
        #endif
        u8RegSc1_4E = 0x24;
        msWrite2ByteMask(u32IPMuxRegBase+0x02,(WORD)IP_MUX_COMBO<<u8IPMuxShift,u8IPMuxMask);
        msDrvSetComboMux(InputPort, SCDetWin);
        msDrvSetIPCLKMux(ID_CLK_COMBO, SCDetWin);
    }
    else if( g_InputPort[InputPort].eIPType & TYPE_DP )
    {
        u8Sync = _BIT7|_BIT4 | _BIT3 | _BIT1 | _BIT0;
        u8Mux = NIS_B|ISEL_COMBO|SCALER_STYPE_HV;
        u8VD = BIT7;
        bInput10b = 1;
        u8RegSc1_48 = 0x00;
        u8RegSc1_4C = 0x00;

#if DP_ASTRO_INTERLACE_PATCH
        if(((msAPI_combo_IPGetDPAstorPTGInterlacePatch((EN_COMBO_IP_SELECT)g_InputPort[InputPort].eIPMux)) && (msDrvIP1ReadSyncStatus(SCDetWin)&INTM_B)) || (msAPI_combo_IPGetDPAstorPTGInterlaceFieldPatch((EN_COMBO_IP_SELECT)g_InputPort[InputPort].eIPMux)))
            u8RegSc1_42 = 0x3F;
        else
            u8RegSc1_42 = 0x3C;
#else
        u8RegSc1_42 = 0x3C;
#endif
        u8RegSc1_3B = BIT5;
        msWrite2ByteMask(u32IPMuxRegBase+0x02,(WORD)IP_MUX_COMBO<<u8IPMuxShift,u8IPMuxMask);
        msDrvSetComboMux(InputPort, SCDetWin);
        msDrvSetIPCLKMux(ID_CLK_COMBO, SCDetWin);
    }
    else //if( g_InputPort[InputPort].eIPType & TYPE_VD )
    {
        u8Sync = 0x9B;
        u8Mux = NIS_B|ISEL_VD|SCALER_STYPE_HV;
        u8VD = _BIT2;
        bInput10b = 0;
        u8RegSc1_48 = 0x00;
        u8RegSc1_4C = 0x00;
        u8RegSc1_42 = 0x00;
        u8RegSc1_3B = BIT5;
        msWrite2ByteMask(u32IPMuxRegBase+0x02,(WORD)IP_MUX_VD<<u8IPMuxShift,u8IPMuxMask);
        msDrvSetIPCLKMux(ID_CLK_VD, SCDetWin);
    }

    u8RegOffset = (IsScalerLeft(SCDetWin)) ? ((SCDetWin>>2)*0x20) : (((SCDetWin>>2)*0x20) + 0x40);

    if( SCDetWin == SCL0_OFFLINE )
    {
        msWriteByte(u32SCRegBase+SC13_04, (u8Mux&(~NIS_B)));
        msWriteByte(u32SCRegBase+SC13_05, u8VD);
        msWriteByte(u32SCRegBase+SC13_06, u8Sync);
        msWriteBit(u32SCRegBase+SC13_07, bInput10b, _BIT3);
        msWriteBit(u32SCRegBase+SC13_3B, u8RegSc1_3B, _BIT5);
        msWriteByte(u32SCRegBase+SC13_42, u8RegSc1_42);
        msWriteByte(u32SCRegBase+SC13_48, u8RegSc1_48);
        msWriteByte(u32SCRegBase+SC13_49, u8RegSc1_49);
        msWriteByte(u32SCRegBase+SC13_4A, u8RegSc1_4A);
        msWriteByte(u32SCRegBase+SC13_4C, u8RegSc1_4C);
#if (CHIP_ID==MST9U3)
        msWriteByteMask(u32SCRegBase+SC13_5E, u8RegSc1_5E, BIT4);
#endif
        msWriteBit(REG_120F62, (u8Sync & _BIT6), _BIT0);
    }
    else if( SCDetWin & BIT0 )
    {
        msWriteByte(u32SCRegBase+SC01_04, u8Mux);
        msWriteByte(u32SCRegBase+SC01_05, u8VD);
        msWriteByte(u32SCRegBase+SC01_06, u8Sync);
        msWriteBit(REG_120F62+u8RegOffset, (u8Sync & _BIT6), _BIT2);
        msWriteBit(u32SCRegBase+SC01_07, bInput10b, _BIT3);
        #if ENABLE_HV_TOLERANCE_ENLARGE
        msWriteByteMask(u32SCRegBase+SC01_3A, H_IntTorlance, 0x3F|BIT6);    //Hsync tolerance   reg_raw_vsmd
        msWriteByteMask(u32SCRegBase+SC01_3B, u8RegSc1_3B, _BIT5|_BIT4|0x0F); //Vsync tolerance,AutoNosignal,Htt filter mode
        #else
        msWriteBit(u32SCRegBase+SC01_3B, u8RegSc1_3B, _BIT5);
        #endif
        msWriteByte(u32SCRegBase+SC01_42, u8RegSc1_42);
        msWriteByte(u32SCRegBase+SC01_48, u8RegSc1_48);
        msWriteByte(u32SCRegBase+SC01_49, u8RegSc1_49);
        msWriteByte(u32SCRegBase+SC01_4A, u8RegSc1_4A);
        msWriteByte(u32SCRegBase+SC01_4C, u8RegSc1_4C);
        msWriteByte(u32SCRegBase+SC01_4E, u8RegSc1_4E);
#if (CHIP_ID==MST9U3)
        msWriteByteMask(u32SCRegBase+SC01_5E, u8RegSc1_5E, BIT4);
#endif
    }
    else
    {
        msWriteByte(u32SCRegBase+SC03_04, u8Mux);
        msWriteByte(u32SCRegBase+SC03_05, u8VD);
        msWriteByte(u32SCRegBase+SC03_06, u8Sync);
        msWriteBit(REG_120F62+u8RegOffset, (u8Sync & _BIT6), _BIT1);
        msWriteBit(u32SCRegBase+SC03_07, bInput10b, _BIT3);
        #if ENABLE_HV_TOLERANCE_ENLARGE
        msWriteByteMask(u32SCRegBase+SC03_3A, H_IntTorlance, 0x3F|BIT6);
        msWriteByteMask(u32SCRegBase+SC03_3B, u8RegSc1_3B, _BIT5|_BIT4|0x0F);
        #else
        msWriteBit(u32SCRegBase+SC03_3B, u8RegSc1_3B, _BIT5);
        #endif
        msWriteByte(u32SCRegBase+SC03_42, u8RegSc1_42);
        msWriteByte(u32SCRegBase+SC03_48, u8RegSc1_48);
        msWriteByte(u32SCRegBase+SC03_49, u8RegSc1_49);
        msWriteByte(u32SCRegBase+SC03_4A, u8RegSc1_4A);
        msWriteByte(u32SCRegBase+SC03_4C, u8RegSc1_4C);
        msWriteByte(u32SCRegBase+SC03_4E, u8RegSc1_4E);
#if (CHIP_ID==MST9U3)
        msWriteByteMask(u32SCRegBase+SC03_5E, u8RegSc1_5E, BIT4);
#endif
    }

    msDrvSoftwareReset( u8IPRst,  SCDetWin);

#endif
}

#if (CHIP_ID == MST9U) || (CHIP_ID == MST9U2)
void msDrvGOPSetTopSC(WORD u16SCMask)
{
    BYTE u8SCOrder = 0;

    u8SCOrder  = (u16SCMask&(bSCL1_MAIN|bSCL1_SUB))?BIT0:0;
    u8SCOrder |= (u16SCMask&(bSCR1_MAIN|bSCR1_SUB))?BIT1:0;

    DB_WBMask(SCL0_00_80, u8SCOrder, BIT0|BIT1);
}
#endif

BYTE msDrvIP1GetInputSyncType(BYTE u8SCDetWin)
{
    BYTE u8Rtn;
    DWORD u32SCIP1RegBase = SCIP1RegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        u8Rtn = msReadByte(SC01_04+u32SCIP1RegBase);
    else
        u8Rtn = msReadByte(SC03_04+u32SCIP1RegBase);

    return u8Rtn;
}

void msDrvIP1SetInputSyncType(WORD u16SCMask, BYTE u8Value)
{
    BYTE u8ScIndex=0;
    DWORD u32SCIP1RegBase;

    while(u16SCMask!=0)
    {
        if(u16SCMask & BIT0)
        {
            u32SCIP1RegBase = SCIP1RegBase(u8ScIndex);
            if( u8ScIndex & BIT0 )   // MAIN
                msWriteByte(SC01_04+u32SCIP1RegBase, u8Value);
            else
                msWriteByte(SC03_04+u32SCIP1RegBase, u8Value);
        }

        u16SCMask >>=1;
        u8ScIndex ++;
    }
}

void msDrvIP1SetInputVSyncDelay(WORD u16SCMask, BOOL bEnable)
{
    BYTE u8ScIndex=0;
    DWORD u32SCIP1RegBase;

    while(u16SCMask!=0)
    {
        if(u16SCMask & BIT0)
        {
            u32SCIP1RegBase = SCIP1RegBase(u8ScIndex);
            // bit value 0: vsync delay enable, 1: disable
            if( u8ScIndex & BIT0 )   // MAIN
                msWriteBit(SC01_06+u32SCIP1RegBase, !bEnable, _BIT5);
            else
                msWriteBit(SC03_06+u32SCIP1RegBase, !bEnable, _BIT5);
        }

        u16SCMask >>=1;
        u8ScIndex ++;
    }
}

void msDrvIP1SetInputYpbprFormat(WORD u16SCMask, BYTE u8Value)
{
    BYTE u8ScIndex=0;
    DWORD u32SCIP1RegBase;

    while(u16SCMask!=0)
    {
        if(u16SCMask & BIT0)
        {
            u32SCIP1RegBase = SCIP1RegBase(u8ScIndex);
            if( u8ScIndex & BIT0 )   // MAIN
                msWriteBit(SC01_05+u32SCIP1RegBase, u8Value, BIT2);
            else
                msWriteBit(SC03_05+u32SCIP1RegBase, u8Value, BIT2);
        }

        u16SCMask >>=1;
        u8ScIndex ++;
    }
}

void msDrvIP1AdcSetCoast(WORD u16SCMask, BYTE u8Enable, BYTE u8Start, BYTE u8End)
{
    BYTE u8ScIndex=0;
    DWORD u32SCIP1RegBase;

    while(u16SCMask!=0)
    {
        if(u16SCMask & BIT0)
        {
            u32SCIP1RegBase = SCIP1RegBase(u8ScIndex);
            if( u8ScIndex & BIT0 )   // MAIN
            {
                msWriteByte(SC01_48+u32SCIP1RegBase, u8Enable);
                msWriteByte(SC01_49+u32SCIP1RegBase, u8Start);
                msWriteByte(SC01_4A+u32SCIP1RegBase, u8End);
            }
            else
            {
                msWriteByte(SC03_48+u32SCIP1RegBase, u8Enable);
                msWriteByte(SC03_49+u32SCIP1RegBase, u8Start);
                msWriteByte(SC03_4A+u32SCIP1RegBase, u8End);
            }
        }

        u16SCMask >>=1;
        u8ScIndex ++;
    }
}

void msDrvIP1AdcGlitchRemoval(WORD u16SCMask, BYTE u8Value)
{
    BYTE u8ScIndex=0;
    DWORD u32SCIP1RegBase;

    while(u16SCMask!=0)
    {
        if(u16SCMask & BIT0)
        {
            u32SCIP1RegBase = SCIP1RegBase(u8ScIndex);
            if( u8ScIndex & BIT0 )   // MAIN
                msWriteByte(SC01_4C+u32SCIP1RegBase, u8Value);
            else
                msWriteByte(SC03_4C+u32SCIP1RegBase, u8Value);
        }

        u16SCMask >>=1;
        u8ScIndex ++;
    }
}
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
    DWORD u32SCIP1RegBase = SCIP1RegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
    {
          msWriteByteMask(SC01_53+u32SCIP1RegBase, bEnable, BIT0);
          msWrite2Byte(SC01_54+u32SCIP1RegBase, sDisplayWin->y);
          msWrite2Byte(SC01_56+u32SCIP1RegBase, sDisplayWin->x);
          msWrite2Byte(SC01_58+u32SCIP1RegBase, sDisplayWin->height);
          msWrite2Byte(SC01_5A+u32SCIP1RegBase, sDisplayWin->width);

    }
    else
    {
          msWriteByteMask(SC03_53+u32SCIP1RegBase, bEnable, BIT0);
          msWrite2Byte(SC03_54+u32SCIP1RegBase, sDisplayWin->y);
          msWrite2Byte(SC03_56+u32SCIP1RegBase, sDisplayWin->x);
          msWrite2Byte(SC03_58+u32SCIP1RegBase, sDisplayWin->height);
          msWrite2Byte(SC03_5A+u32SCIP1RegBase, sDisplayWin->width);

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
    DWORD u32SCIP1RegBase = SCIP1RegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        WaitAutoStatusReady(SC01_20+u32SCIP1RegBase, BIT1, u8InputPort);
    else
        WaitAutoStatusReady(SC03_20+u32SCIP1RegBase, BIT1, u8InputPort);
}
void msDrvIP1AutoPositionSetValidValue(BYTE u8SCDetWin, BYTE u8Value)
{
    DWORD u32SCIP1RegBase = SCIP1RegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        msWriteByteMask(SC01_21+u32SCIP1RegBase, (u8Value) << 4, 0xF0);
    else
        msWriteByteMask(SC03_21+u32SCIP1RegBase, (u8Value) << 4, 0xF0);
}
BYTE msDrvIP1AutoPositionReadValidValue(BYTE u8SCDetWin)
{
    BYTE u8Rtn;
    DWORD u32SCIP1RegBase = SCIP1RegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        u8Rtn = msReadByte(SC01_21+u32SCIP1RegBase)>>4;
    else
        u8Rtn = msReadByte(SC03_21+u32SCIP1RegBase)>>4;

    return u8Rtn;
}
WORD msDrvIP1AutoPositionReadHstart(BYTE u8SCDetWin, BYTE u8InputPort)
{
    WORD u8Rtn;
    DWORD u32SCIP1RegBase = SCIP1RegBase(u8SCDetWin);
#if Enable_Auto_Switch_DE_Mode
    if(!FreeSyncFlag)
    {
    if( u8SCDetWin & BIT0 )   // MAIN
        u8Rtn = GetAutoValue(SC01_26 + u32SCIP1RegBase, u8InputPort);
    else
        u8Rtn = GetAutoValue(SC03_26 + u32SCIP1RegBase, u8InputPort);
    }
    else
    {
    if(INPUT_IS_HDMI_2_0(u8InputPort)&& HBR2Flag(u8InputPort))
    {
        if( u8SCDetWin & BIT0 )   // MAIN
            u8Rtn = GetAutoValue(SC01_2A + u32SCIP1RegBase, u8InputPort);
        else
            u8Rtn = GetAutoValue(SC03_2A + u32SCIP1RegBase, u8InputPort);
    }
    else
    {
        if( u8SCDetWin & BIT0 )   // MAIN
            u8Rtn = GetAutoValue(SC01_26 + u32SCIP1RegBase, u8InputPort);
        else
            u8Rtn = GetAutoValue(SC03_26 + u32SCIP1RegBase, u8InputPort);
    }
   }
 #else
    #if ENABLE_DEONLY_MODE
    if( u8SCDetWin & BIT0 )   // MAIN
        u8Rtn = GetAutoValue(SC01_26 + u32SCIP1RegBase, u8InputPort);
    else
        u8Rtn = GetAutoValue(SC03_26 + u32SCIP1RegBase, u8InputPort);
    #else
    if(INPUT_IS_HDMI_2_0(u8InputPort)&& HBR2Flag(u8InputPort))
    {
        if( u8SCDetWin & BIT0 )   // MAIN
            u8Rtn = GetAutoValue(SC01_2A + u32SCIP1RegBase, u8InputPort);
        else
            u8Rtn = GetAutoValue(SC03_2A + u32SCIP1RegBase, u8InputPort);
    }
    else
    {
        if( u8SCDetWin & BIT0 )   // MAIN
            u8Rtn = GetAutoValue(SC01_26 + u32SCIP1RegBase, u8InputPort);
        else
            u8Rtn = GetAutoValue(SC03_26 + u32SCIP1RegBase, u8InputPort);
    }
    #endif
   
   #endif
    return u8Rtn;
}

WORD msDrvIP1AutoPositionReadHend(BYTE u8SCDetWin, BYTE u8InputPort)
{
    WORD u8Rtn;
    DWORD u32SCIP1RegBase = SCIP1RegBase(u8SCDetWin);
#if Enable_Auto_Switch_DE_Mode
    if(!FreeSyncFlag)
    {
    if( u8SCDetWin & BIT0 )   // MAIN
        u8Rtn = GetAutoValue(SC01_2A + u32SCIP1RegBase, u8InputPort);
    else
        u8Rtn = GetAutoValue(SC03_2A + u32SCIP1RegBase, u8InputPort);
    if(HBR2Flag(u8InputPort))
        u8Rtn -= DFT_HBR2_GUARDBAND;
    }
    else
    {
    if(INPUT_IS_HDMI_2_0(u8InputPort)&& HBR2Flag(u8InputPort))
    {
        if( u8SCDetWin & BIT0 )   // MAIN
            u8Rtn = GetAutoValue(SC01_26 + u32SCIP1RegBase, u8InputPort);
        else
            u8Rtn = GetAutoValue(SC03_26 + u32SCIP1RegBase, u8InputPort);
    }
    else
    {
        if( u8SCDetWin & BIT0 )   // MAIN
            u8Rtn = GetAutoValue(SC01_2A + u32SCIP1RegBase, u8InputPort);
        else
            u8Rtn = GetAutoValue(SC03_2A + u32SCIP1RegBase, u8InputPort);
    }
        if(HBR2Flag(u8InputPort))
            u8Rtn -= DFT_HBR2_GUARDBAND;
    }
#else
    #if ENABLE_DEONLY_MODE
    if( u8SCDetWin & BIT0 )   // MAIN
        u8Rtn = GetAutoValue(SC01_2A + u32SCIP1RegBase, u8InputPort);
    else
        u8Rtn = GetAutoValue(SC03_2A + u32SCIP1RegBase, u8InputPort);
    if(HBR2Flag(u8InputPort))
        u8Rtn -= DFT_HBR2_GUARDBAND;
    #else
    if(INPUT_IS_HDMI_2_0(u8InputPort)&& HBR2Flag(u8InputPort))
    {
        if( u8SCDetWin & BIT0 )   // MAIN
            u8Rtn = GetAutoValue(SC01_26 + u32SCIP1RegBase, u8InputPort);
        else
            u8Rtn = GetAutoValue(SC03_26 + u32SCIP1RegBase, u8InputPort);
    }
    else
    {
        if( u8SCDetWin & BIT0 )   // MAIN
            u8Rtn = GetAutoValue(SC01_2A + u32SCIP1RegBase, u8InputPort);
        else
            u8Rtn = GetAutoValue(SC03_2A + u32SCIP1RegBase, u8InputPort);
    }
        if(HBR2Flag(u8InputPort))
            u8Rtn -= DFT_HBR2_GUARDBAND;
    #endif
#endif

    return u8Rtn;
}

WORD msDrvIP1AutoPositionReadVstart(BYTE u8SCDetWin, BYTE u8InputPort)
{
    WORD u8Rtn;
    DWORD u32SCIP1RegBase = SCIP1RegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        u8Rtn = GetAutoValue(SC01_24 + u32SCIP1RegBase, u8InputPort);
    else
        u8Rtn = GetAutoValue(SC03_24 + u32SCIP1RegBase, u8InputPort);

    return u8Rtn;
}

WORD msDrvIP1AutoPositionReadVend(BYTE u8SCDetWin, BYTE u8InputPort)
{
    WORD u8Rtn;
    DWORD u32SCIP1RegBase = SCIP1RegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        u8Rtn = GetAutoValue(SC01_28 + u32SCIP1RegBase, u8InputPort);
    else
        u8Rtn = GetAutoValue(SC03_28 + u32SCIP1RegBase, u8InputPort);

    return u8Rtn;
}

BYTE msDrvIP1AutoPositionReadTranstionPosition(WORD u16DelayTime, BYTE u8SCDetWin, BYTE u8InputPort)
{
    BYTE u8Rtn;
    DWORD u32SCIP1RegBase = SCIP1RegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        u8Rtn = GetTranstionPosition( u16DelayTime, SC01_26 + u32SCIP1RegBase, u8InputPort );
    else
        u8Rtn = GetTranstionPosition( u16DelayTime, SC03_26 + u32SCIP1RegBase, u8InputPort );

    return u8Rtn;
}

void msDrvIP1AutoPhaseResultReady(BYTE u8SCDetWin)
{
    DWORD u32SCIP1RegBase = SCIP1RegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        drvADC_WaitAutoStatusReady(SC01_32+u32SCIP1RegBase, BIT1);
    else
        drvADC_WaitAutoStatusReady(SC03_32+u32SCIP1RegBase, BIT1);
}

DWORD msDrvIP1AutoPhaseReadValue(BYTE u8SCDetWin)
{
    DWORD u8Rtn;
    DWORD u32SCIP1RegBase = SCIP1RegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        u8Rtn = (((DWORD)msRead2Byte(SC01_36+u32SCIP1RegBase))<<16)|msRead2Byte(SC01_34+u32SCIP1RegBase);
    else
        u8Rtn = (((DWORD)msRead2Byte(SC03_36+u32SCIP1RegBase))<<16)|msRead2Byte(SC03_34+u32SCIP1RegBase);

    return u8Rtn;
}

// Auto Phase Nose Mask.
// #000: Mask 0 bit, #001: Mask 1 bit. #010: Mask 2 bit. #011: Mask 3 bit. #100: Mask 4 bit. #101: Mask 5 bit. #110: Mask 6 bit. #111: Mask 7 bit.
void msDrvIP1AutoPhaseSetNoiseMask(BYTE u8Value, BYTE u8SCDetWin)
{
    DWORD u32SCIP1RegBase = SCIP1RegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        msWriteByteMask(SC01_32+u32SCIP1RegBase, u8Value<<2|BIT0, 0x1D);
    else
        msWriteByteMask(SC03_32+u32SCIP1RegBase, u8Value<<2|BIT0, 0x1D);
}


// scaler
void msDrvIP1SetOutputLockMode(BYTE u8Value, BYTE u8SCDetWin)
{
    DWORD u32SCIP1RegBase = SCIP1RegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        msWriteByteMask(SC01_07+u32SCIP1RegBase, (u8Value)?(BIT7):(0) , BIT7);
    else
        msWriteByteMask(SC03_07+u32SCIP1RegBase, (u8Value)?(BIT7):(0) , BIT7);
}
void msDrvIP1SetOutputFreerun(BYTE u8Value, BYTE u8SCDetWin)
{
#if ENABLE_SW_FLOCK
    DWORD u32SCIP1RegBase = SCIP1RegBase ( u8SCDetWin );
    #if CHIP_ID==MST9U4
    BYTE  u8OriFreerun = ( msDrvIP1GetInputSyncType ( u8SCDetWin ) &BIT7 ) ? TRUE : FALSE;

    if ( u8Value )
    {
        msDrvScMuteScreen ( BIT0 << u8SCDetWin, TRUE );

        if ( ! ( ( SyncLossState ( MapScaler2Port ( u8SCDetWin ) ) ) || ( GetBitCnts ( g_InputPort[MapScaler2Port ( u8SCDetWin )].u16IPScMask ) == 1 ) ) )
        {
            return;
        }
    }

    #endif

    if ( u8SCDetWin & BIT0 )  // MAIN
    {
        msWriteByteMask ( SC01_04 + u32SCIP1RegBase, ( u8Value ) ? ( BIT7 ) : ( 0 ) , BIT7 );
        #if CHIP_ID!=MST9U4 // MST9U4_TBD

        if ( u8Value )
        {
            msWriteByte ( SC5B_03 + SCRegBase ( u8SCDetWin ), 0x00 );
       #if 0
#if ((CHIP_ID!=MST9U4))
            msDrvSCCommonWrite(SCALER_MASK_MAIN, ENABLE);
            msWriteByteMask(SCL0_20_32,0x00,BIT0);
            msWriteByteMask(SCL0_01_32,0x00,BIT0);
            msWriteByteMask(SCL0_01_53, 0x00, BIT0);
            msWrite2ByteMask(SCL0_01_54, 0x01, 0x1FFF);
            msDrvATPINTEnable(FALSE);
            msDrvSCCommonWrite(SCALER_MASK_MAIN, DISABLE);
#endif
	   
	#else		
            msDrvSCCommonWrite ( SCALER_MASK_MAIN, ENABLE );
            msWriteByteMask ( SCL0_20_32, 0x00, BIT0 );
        #if ENABLE_VCNT_FREEZE
            msWriteByteMask(SCL0_01_32, 0x00, BIT0);
        #endif
            msWriteByteMask ( SCL0_01_53, 0x00, BIT0 );
            msWrite2ByteMask ( SCL0_01_54, 0x01, 0x1FFF );
            #if CHIP_ID != MST9U4
            msDrvATPINTEnable ( FALSE );
            #endif
            msWriteBit ( SCL0_0F_AC, TRUE, _BIT3 );
			msWriteByteMask(SCL0_00_02, 0,BIT0);
			msWrite2Byte(SCL0_10_1A, ( PanelVTotal - 1 ) );
            msDrvSCCommonWrite ( SCALER_MASK_MAIN, DISABLE );
	#endif		
        }

        #endif
    }
    else
    {
        msWriteByteMask ( SC03_04 + u32SCIP1RegBase, ( u8Value ) ? ( BIT7 ) : ( 0 ) , BIT7 );
    }

    #if CHIP_ID!=MST9U4

    if ( u8Value )
    {
        msDrvScSetDefaultFiclk ( u8SCDetWin );
    }

    #endif

    // Update Vtt accordingly by no_signal in FLOCK mode & Frame sync scaler.
    if ( ( g_ScalerInfo[SCL0_MAIN].u8SiSyncMode == FLM_FB_FLOCK || g_ScalerInfo[SCL0_MAIN].u8SiSyncMode == FLM_FBL_FLOCK )
     #if CHIP_ID==MST9U4
         && ( MapScaler2Port ( u8SCDetWin ) == MapScaler2Port ( SCL0_MAIN ) )
         && ( u8OriFreerun != u8Value )
     #else
         && ( u8SCDetWin == SCL0_MAIN )
     #endif
       )
    {
        msDrvSCCommonWrite ( SCALER_MASK_MAIN, ENABLE );

        if ( u8Value )
        {
            msWrite2Byte ( SCL0_10_1A, ( PanelVTotal - 1 ) );
        }
        else
        {
            if ( g_ScalerInfo[SCL0_MAIN].u8SiSyncMode == FLM_FBL_FLOCK )
            {
                msWrite2Byte ( SCL0_10_1A, 0xFFF );
            }

            if ( g_ScalerInfo[SCL0_MAIN].u8SiSyncMode == FLM_FB_FLOCK )
            {
                msWriteByteMask ( SCL0_20_32, BIT0, BIT0 ); //Switch output blank polling from VDE falling to Vsync Raising
                #if CHIP_ID != MST9U4
                msDrvATPINTEnable ( TRUE );
                #endif
                WAIT_V_OUTPUT_BLANKING_START();
                msWriteBit ( SCL0_10_21, TRUE, _BIT7 );
            }
        }

        msDrvSCCommonWrite ( SCALER_MASK_MAIN, DISABLE );
    }



#else
    DWORD u32SCRegBase = SCRegBase(u8SCDetWin);
    DWORD u32SCIP1RegBase = SCIP1RegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
    {
        msWriteByteMask(SC01_04+u32SCIP1RegBase, (u8Value)?(BIT7):(0) , BIT7);
        if(u8Value)
        {
            msWriteByte(SC5B_03+u32SCRegBase, 0x00);
        }
    }
    else
        msWriteByteMask(SC03_04+u32SCIP1RegBase, (u8Value)?(BIT7):(0) , BIT7);

    if( u8Value )
        msDrvScSetDefaultFiclk(u8SCDetWin);


#endif

}

void msDrvIP1SetHperiodDetectMode(BOOL bEnable8lines, BYTE u8SCDetWin)
{
    DWORD u32SCIP1RegBase = SCIP1RegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        msWriteByte(SC01_41+u32SCIP1RegBase, (bEnable8lines)?(BIT7):(0));
    else
        msWriteByte(SC03_41+u32SCIP1RegBase, (bEnable8lines)?(BIT7):(0));
}

BOOL msDrvIP1GetHperiodDetectMode(BYTE u8SCDetWin) // TRUE: 8 line mode, FALSE: 1 line mode
{
    DWORD u32SCIP1RegBase = SCIP1RegBase(u8SCDetWin);
    BOOL bRtn;

    if( u8SCDetWin & BIT0 )   // MAIN
        bRtn = (msReadByte(SC01_41+u32SCIP1RegBase)&BIT7)?TRUE:FALSE;
    else
        bRtn = (msReadByte(SC03_41+u32SCIP1RegBase)&BIT7)?TRUE:FALSE;

    return bRtn;
}

WORD msDrvIP1ReadHperiod(BYTE u8SCDetWin)
{
    WORD u8Rtn;
    DWORD u32SCIP1RegBase = SCIP1RegBase(u8SCDetWin);
#ifndef MST9U_FPGA
    BYTE ucInputPort = MapScaler2Port(u8SCDetWin);

    if(INPUT_IS_TMDS(ucInputPort) && !msAPI_combo_IPGetTmdsClkStable((EN_COMBO_IP_SELECT)g_InputPort[ucInputPort].eIPMux)
        && !msAPI_combo_IPGetTmdsSplitterDone((EN_COMBO_IP_SELECT)g_InputPort[ucInputPort].eIPMux))
        u8Rtn = 0;
    else
#endif
        if( u8SCDetWin & BIT0 )   // MAIN
        u8Rtn = (msRead2Byte(SC01_40+u32SCIP1RegBase) & SC_MASK_IP1_HPD);
    else
        u8Rtn = (msRead2Byte(SC03_40+u32SCIP1RegBase) & SC_MASK_IP1_HPD);

    if(msDrvIP1GetHperiodDetectMode(u8SCDetWin)) // 8 line mode
    {
        if( u8Rtn != SC_MASK_IP1_HPD && u8Rtn >= 10 )
        {
            u8Rtn = (u8Rtn+4)/8;
        }
    }

    return u8Rtn;
}
WORD msDrvIP1ReadHtotal(BYTE u8SCDetWin)
{
    WORD u8Rtn;
    DWORD u32SCIP1RegBase = SCIP1RegBase(u8SCDetWin);

    // DP need consider reading from DP band !!??

    if( u8SCDetWin & BIT0 )   // MAIN
        u8Rtn = (msRead2Byte(SC01_50+u32SCIP1RegBase) & SC_MASK_IP1_HVTT);
    else
        u8Rtn = (msRead2Byte(SC03_50+u32SCIP1RegBase) & SC_MASK_IP1_HVTT);

    return u8Rtn;
}
WORD msDrvIP1ReadVtotal(BYTE u8SCDetWin)
{
    WORD u8Rtn;
    DWORD u32SCIP1RegBase = SCIP1RegBase(u8SCDetWin);

#ifndef MST9U_FPGA
    if(msDrvIP1CheckComboStable(u8SCDetWin))
        u8Rtn = 0;
    else
#endif
        if( u8SCDetWin & BIT0 )   // MAIN
        u8Rtn = (msRead2Byte(SC01_3E+u32SCIP1RegBase) & SC_MASK_IP1_HVTT);
    else
        u8Rtn = (msRead2Byte(SC03_3E+u32SCIP1RegBase) & SC_MASK_IP1_HVTT);

    return u8Rtn;
}
BOOL msDrvIP1CheckComboStable(BYTE u8SCDetWin)
{
    BYTE ucInputPort = MapScaler2Port(u8SCDetWin);

    if(INPUT_IS_TMDS(ucInputPort) && !msAPI_combo_IPGetTmdsClkStable((EN_COMBO_IP_SELECT)g_InputPort[ucInputPort].eIPMux)
        && !msAPI_combo_IPGetTmdsSplitterDone((EN_COMBO_IP_SELECT)g_InputPort[ucInputPort].eIPMux))
       return TRUE;
    else if(INPUT_IS_DISPLAYPORT(ucInputPort) && !msAPI_combo_CheckDPLock((EN_COMBO_IP_SELECT)g_InputPort[ucInputPort].eIPMux))
	return TRUE;
    else
	return FALSE;
}
#if ENABLE_FORCE_4K_PMODE
BYTE msDrvIP1ReadUserForceToProgressOrInterless(BYTE u8SCDetWin)
{
    BYTE u8Rtn;
    DWORD u32SCIP1RegBase = SCIP1RegBase(u8SCDetWin);

    //00,10: detect interlace by chip, 01:force to P,11:for to I,
    if( u8SCDetWin & BIT0 )   // MAIN
        u8Rtn = msReadByte(SC01_42+u32SCIP1RegBase)&(BIT1|BIT0);
    else
        u8Rtn = msReadByte(SC03_42+u32SCIP1RegBase)&(BIT1|BIT0);

    return u8Rtn;
}
#endif
BYTE msDrvIP1ReadSyncStatus(BYTE u8SCDetWin)
{
    BYTE u8Rtn;
    DWORD u32SCIP1RegBase = SCIP1RegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        u8Rtn = msReadByte(SC01_3D+u32SCIP1RegBase);
    else
        u8Rtn = msReadByte(SC03_3D+u32SCIP1RegBase);

    return u8Rtn;
}
WORD msDrvIP1ReadAutoHStart(BYTE u8SCDetWin)
{
    WORD u8Rtn;
    DWORD u32SCIP1RegBase = SCIP1RegBase(u8SCDetWin);
#if Enable_Auto_Switch_DE_Mode
      if(!FreeSyncFlag)
      {
    if( u8SCDetWin & BIT0 )   // MAIN
        u8Rtn = msRead2Byte(SC01_26+u32SCIP1RegBase);
    else
        u8Rtn = msRead2Byte(SC03_26+u32SCIP1RegBase);
     }
  else
    {
    BYTE InputPort = MapScaler2Port(u8SCDetWin);

    if(INPUT_IS_HDMI_2_0(InputPort)&& HBR2Flag(InputPort))
    {
        if( u8SCDetWin & BIT0 )   // MAIN
            u8Rtn = msRead2Byte(SC01_2A+u32SCIP1RegBase);
        else
            u8Rtn = msRead2Byte(SC03_2A+u32SCIP1RegBase);
    }
    else
    {
        if( u8SCDetWin & BIT0 )   // MAIN
            u8Rtn = msRead2Byte(SC01_26+u32SCIP1RegBase);
        else
            u8Rtn = msRead2Byte(SC03_26+u32SCIP1RegBase);
    }
    }
#else
    #if ENABLE_DEONLY_MODE
    if( u8SCDetWin & BIT0 )   // MAIN
        u8Rtn = msRead2Byte(SC01_26+u32SCIP1RegBase);
    else
        u8Rtn = msRead2Byte(SC03_26+u32SCIP1RegBase);
    #else
    BYTE InputPort = MapScaler2Port(u8SCDetWin);

    if(INPUT_IS_HDMI_2_0(InputPort)&& HBR2Flag(InputPort))
    {
        if( u8SCDetWin & BIT0 )   // MAIN
            u8Rtn = msRead2Byte(SC01_2A+u32SCIP1RegBase);
        else
            u8Rtn = msRead2Byte(SC03_2A+u32SCIP1RegBase);
    }
    else
    {
        if( u8SCDetWin & BIT0 )   // MAIN
            u8Rtn = msRead2Byte(SC01_26+u32SCIP1RegBase);
        else
            u8Rtn = msRead2Byte(SC03_26+u32SCIP1RegBase);
    }
   #endif	
#endif	  

    return u8Rtn;
}
WORD msDrvIP1ReadAutoHEnd(BYTE u8SCDetWin)
{
    WORD u8Rtn;
    DWORD u32SCIP1RegBase = SCIP1RegBase(u8SCDetWin);
#if Enable_Auto_Switch_DE_Mode
   if(!FreeSyncFlag)
  {
    if( u8SCDetWin & BIT0 )   // MAIN
        u8Rtn = msRead2Byte(SC01_2A+u32SCIP1RegBase);
    else
        u8Rtn = msRead2Byte(SC03_2A+u32SCIP1RegBase);
   }
   else
   {
    BYTE InputPort = MapScaler2Port(u8SCDetWin);

    if(INPUT_IS_HDMI_2_0(InputPort)&& HBR2Flag(InputPort))
    {
        if( u8SCDetWin & BIT0 )   // MAIN
            u8Rtn = msRead2Byte(SC01_26+u32SCIP1RegBase);
        else
            u8Rtn = msRead2Byte(SC03_26+u32SCIP1RegBase);
    }
    else
    {
        if( u8SCDetWin & BIT0 )   // MAIN
            u8Rtn = msRead2Byte(SC01_2A+u32SCIP1RegBase);
        else
            u8Rtn = msRead2Byte(SC03_2A+u32SCIP1RegBase);
    }
    }
#else
    #if ENABLE_DEONLY_MODE
    if( u8SCDetWin & BIT0 )   // MAIN
        u8Rtn = msRead2Byte(SC01_2A+u32SCIP1RegBase);
    else
        u8Rtn = msRead2Byte(SC03_2A+u32SCIP1RegBase);
    #else
    BYTE InputPort = MapScaler2Port(u8SCDetWin);

    if(INPUT_IS_HDMI_2_0(InputPort)&& HBR2Flag(InputPort))
    {
        if( u8SCDetWin & BIT0 )   // MAIN
            u8Rtn = msRead2Byte(SC01_26+u32SCIP1RegBase);
        else
            u8Rtn = msRead2Byte(SC03_26+u32SCIP1RegBase);
    }
    else
    {
        if( u8SCDetWin & BIT0 )   // MAIN
            u8Rtn = msRead2Byte(SC01_2A+u32SCIP1RegBase);
        else
            u8Rtn = msRead2Byte(SC03_2A+u32SCIP1RegBase);
    }
    #endif
#endif
    if(HBR2Flag(MapScaler2Port(u8SCDetWin)))
        u8Rtn -= DFT_HBR2_GUARDBAND;

    return u8Rtn;
}
WORD msDrvIP1ReadAutoVStart(BYTE u8SCDetWin)
{
    WORD u8Rtn;
    DWORD u32SCIP1RegBase = SCIP1RegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        u8Rtn = msRead2Byte(SC01_24+u32SCIP1RegBase);
    else
        u8Rtn = msRead2Byte(SC03_24+u32SCIP1RegBase);

    return u8Rtn;
}
WORD msDrvIP1ReadAutoVEnd(BYTE u8SCDetWin)
{
    WORD u8Rtn;
    DWORD u32SCIP1RegBase = SCIP1RegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        u8Rtn = msRead2Byte(SC01_28+u32SCIP1RegBase);
    else
        u8Rtn = msRead2Byte(SC03_28+u32SCIP1RegBase);

    return u8Rtn;
}
WORD msDrvIP1ReadAutoWidth(BYTE u8SCDetWin)
{
#if Enable_Auto_Switch_DE_Mode
  if(!FreeSyncFlag)
  {
        return (msDrvIP1ReadAutoHEnd(u8SCDetWin) - msDrvIP1ReadAutoHStart(u8SCDetWin) + 1);
  }
  else
   {
    BYTE InputPort = MapScaler2Port(u8SCDetWin);

    if(INPUT_IS_HDMI_2_0(InputPort)&& HBR2Flag(InputPort))
    {
        return (msDrvIP1ReadHtotal(u8SCDetWin)-(msDrvIP1ReadAutoHStart(u8SCDetWin)-msDrvIP1ReadAutoHEnd(u8SCDetWin))+2);
    }
    else
    {
        return (msDrvIP1ReadAutoHEnd(u8SCDetWin) - msDrvIP1ReadAutoHStart(u8SCDetWin) + 1);
    }
   }
#else
    #if ENABLE_DEONLY_MODE
        return (msDrvIP1ReadAutoHEnd(u8SCDetWin) - msDrvIP1ReadAutoHStart(u8SCDetWin) + 1);
    #else
    BYTE InputPort = MapScaler2Port(u8SCDetWin);

    if(INPUT_IS_HDMI_2_0(InputPort)&& HBR2Flag(InputPort))
    {
        return (msDrvIP1ReadHtotal(u8SCDetWin)-(msDrvIP1ReadAutoHStart(u8SCDetWin)-msDrvIP1ReadAutoHEnd(u8SCDetWin))+2);
    }
    else
    {
        return (msDrvIP1ReadAutoHEnd(u8SCDetWin) - msDrvIP1ReadAutoHStart(u8SCDetWin) + 1);
    }
    #endif
#endif    
}
WORD msDrvIP1ReadAutoHeight(BYTE u8SCDetWin)
{

#if Enable_Auto_Switch_DE_Mode
   if(!FreeSyncFlag)
   {
        return (msDrvIP1ReadAutoVEnd(u8SCDetWin) - msDrvIP1ReadAutoVStart(u8SCDetWin) + 1);
   }
   else
   {
    BYTE InputPort = MapScaler2Port(u8SCDetWin);

    if(INPUT_IS_HDMI_2_0(InputPort)&& HBR2Flag(InputPort))
    {
        return (msDrvIP1ReadAutoVEnd(u8SCDetWin) - msDrvIP1ReadAutoVStart(u8SCDetWin));
    }
    else
    {
        return (msDrvIP1ReadAutoVEnd(u8SCDetWin) - msDrvIP1ReadAutoVStart(u8SCDetWin) + 1);
    }
    }
#else
    #if ENABLE_DEONLY_MODE
        return (msDrvIP1ReadAutoVEnd(u8SCDetWin) - msDrvIP1ReadAutoVStart(u8SCDetWin) + 1);
    #else
    BYTE InputPort = MapScaler2Port(u8SCDetWin);

    if(INPUT_IS_HDMI_2_0(InputPort)&& HBR2Flag(InputPort))
    {
        return (msDrvIP1ReadAutoVEnd(u8SCDetWin) - msDrvIP1ReadAutoVStart(u8SCDetWin));
    }
    else
    {
        return (msDrvIP1ReadAutoVEnd(u8SCDetWin) - msDrvIP1ReadAutoVStart(u8SCDetWin) + 1);
    }
    #endif
#endif   
}
WORD msDrvIP1ReadImageVStart(BYTE u8SCDetWin)
{
    WORD u8Rtn;
    DWORD u32SCIP1RegBase = SCIP1RegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        u8Rtn = (msRead2Byte(SC01_08+u32SCIP1RegBase)&SC_MASK_IP1_CapVST);
    else
        u8Rtn = (msRead2Byte(SC03_08+u32SCIP1RegBase)&SC_MASK_IP1_CapVST);

    return u8Rtn;
}
WORD msDrvIP1ReadImageHeight(BYTE u8SCDetWin)
{
    WORD u8Rtn;
    DWORD u32SCIP1RegBase = SCIP1RegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        u8Rtn = (msRead2Byte(SC01_0C+u32SCIP1RegBase)&SC_MASK_IP1_CapVDC);
    else
        u8Rtn = (msRead2Byte(SC03_0C+u32SCIP1RegBase)&SC_MASK_IP1_CapVDC);

    return u8Rtn;
}
WORD msDrvIP1ReadImageWidth(BYTE u8SCDetWin)
{
    WORD u8Rtn;
    DWORD u32SCIP1RegBase = SCIP1RegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        u8Rtn = (msRead2Byte(SC01_0E+u32SCIP1RegBase)&SC_MASK_IP1_CapHDC);
    else
        u8Rtn = (msRead2Byte(SC03_0E+u32SCIP1RegBase)&SC_MASK_IP1_CapHDC);

    return u8Rtn;
}
void msDrvIP1SetImageHeight(WORD u16Value, BYTE u8SCDetWin)
{
    DWORD u32SCIP1RegBase = SCIP1RegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        msWrite2Byte(SC01_0C+u32SCIP1RegBase, (u16Value)&SC_MASK_IP1_CapVDC);
    else
        msWrite2Byte(SC03_0C+u32SCIP1RegBase, (u16Value)&SC_MASK_IP1_CapVDC);
}
void msDrvIP1SetImageWidth(WORD u16Value, BYTE u8SCDetWin)
{
    DWORD u32SCIP1RegBase = SCIP1RegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        msWrite2Byte(SC01_0E+u32SCIP1RegBase, (u16Value)&SC_MASK_IP1_CapHDC);
    else
        msWrite2Byte(SC03_0E+u32SCIP1RegBase, (u16Value)&SC_MASK_IP1_CapHDC);
}
void msDrvIP1SetImageHStart(WORD u16Value, BYTE u8SCDetWin)
{
    DWORD u32SCIP1RegBase = SCIP1RegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        msWrite2Byte(SC01_0A+u32SCIP1RegBase, (u16Value)&SC_MASK_IP1_CapHST);
    else
        msWrite2Byte(SC03_0A+u32SCIP1RegBase, (u16Value)&SC_MASK_IP1_CapHST);
}
void msDrvIP1SetImageVStart(WORD u16Value, BYTE u8SCDetWin)
{
    DWORD u32SCIP1RegBase = SCIP1RegBase(u8SCDetWin);
    //u16Value= ((msReadByte(SC01_3D+u32SCRegBase)&INTM_B)?(2):(1))*u16Value;

    if( u8SCDetWin & BIT0 )   // MAIN
        msWrite2Byte(SC01_08+u32SCIP1RegBase, (u16Value)&SC_MASK_IP1_CapVST);
    else
        msWrite2Byte(SC03_08+u32SCIP1RegBase, (u16Value)&SC_MASK_IP1_CapVST);
}

BOOL msDrvIP1IsDEOnlyMode(BYTE u8SCDetWin)
{
    BOOL u8Rtn;
    DWORD u32SCIP1RegBase = SCIP1RegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        u8Rtn = ((msReadByte(SC01_06+u32SCIP1RegBase)&BIT6) == BIT6);
    else
        u8Rtn = ((msReadByte(SC03_06+u32SCIP1RegBase)&BIT6) == BIT6);

    return u8Rtn;
}

BOOL msDrvIP1IsVideoFieldInvert(BYTE u8SCDetWin)
{
    BOOL u8Rtn;
    DWORD u32SCIP1RegBase = SCIP1RegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        u8Rtn = ((msReadByte(SC01_43+u32SCIP1RegBase)&BIT3) == BIT3);
    else
        u8Rtn = ((msReadByte(SC03_43+u32SCIP1RegBase)&BIT3) == BIT3);

    return u8Rtn;
}

BYTE msDrvIP1ReadVSyncWidth(BYTE u8SCDetWin)
{
    BYTE u8Rtn;
    DWORD u32SCIP1RegBase = SCIP1RegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        u8Rtn = msReadByte(SC01_45+u32SCIP1RegBase);
    else
        u8Rtn = msReadByte(SC03_45+u32SCIP1RegBase);

    return u8Rtn;
}

WORD msDrvIP2ReadHSDCnt(BYTE u8SCDetWin)
{
    WORD u8Rtn;
    DWORD u32SCRegBase = SCRegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        u8Rtn = (msRead2Byte(SC02_7C+u32SCRegBase)&SC_MASK_IP2_HSD);
    else
        u8Rtn = (msRead2Byte(SC04_7C+u32SCRegBase)&SC_MASK_IP2_HSD);

    return u8Rtn;
}

WORD msDrvIP2ReadVSDCnt(BYTE u8SCDetWin)
{
    WORD u8Rtn;
    DWORD u32SCRegBase = SCRegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        u8Rtn = (msRead2Byte(SC02_7E+u32SCRegBase)&SC_MASK_IP2_VSD);
    else
        u8Rtn = (msRead2Byte(SC04_7E+u32SCRegBase)&SC_MASK_IP2_VSD);

    return u8Rtn;
}
void msDrvMIUInit()
{
    BYTE i;

    for( i = 0; i < sizeof( tblMIU ) / sizeof( RegTbl2Type ); i++ )
        msWrite2ByteMask( tblMIU[i].dwReg, tblMIU[i].wValue, tblMIU[i].wMask );
}

void msDrvMODInit()
{
    #ifndef MST9U_FPGA
    BYTE i;
        for( i = 0; i < sizeof( tblInitMOD ) / sizeof( RegUnitType2 ); i++ )
            msWrite2Byte( tblInitMOD[i].u32Reg, tblInitMOD[i].u16Value );

        for( i = 0; i < sizeof( tblInitMOD2 ) / sizeof( RegTbl2Type ); i++ )
            msWrite2ByteMask( tblInitMOD2[i].dwReg, tblInitMOD2[i].wValue, tblInitMOD2[i].wMask );
    #endif
}

WORD msDrvOP2GetImageHStart(BYTE u8SCDetWin)
{
    WORD u16Rtn;
    DWORD u32SCRegBase = SCRegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        u16Rtn = msRead2Byte(SC10_10+u32SCRegBase) & SC_MASK_OP2_H;
    else
        u16Rtn = msRead2Byte(SC0F_0E+u32SCRegBase) & SC_MASK_OP2_H;

    return u16Rtn;
}

WORD msDrvOP2GetImageHEnd(BYTE u8SCDetWin)
{
    WORD u16Rtn;
    DWORD u32SCRegBase = SCRegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        u16Rtn = msRead2Byte(SC10_12+u32SCRegBase) & SC_MASK_OP2_H;
    else
        u16Rtn = msRead2Byte(SC0F_10+u32SCRegBase) & SC_MASK_OP2_H;

    return u16Rtn;
}

WORD msDrvOP2GetImageVStart(BYTE u8SCDetWin)
{
    WORD u16Rtn;
    DWORD u32SCRegBase = SCRegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        u16Rtn = msRead2Byte(SC10_14+u32SCRegBase) & SC_MASK_OP2_V;
    else
        u16Rtn = msRead2Byte(SC0F_12+u32SCRegBase) & SC_MASK_OP2_V;

    return u16Rtn;
}

WORD msDrvOP2GetImageVEnd(BYTE u8SCDetWin)
{
    WORD u16Rtn;
    DWORD u32SCRegBase = SCRegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        u16Rtn = msRead2Byte(SC10_16+u32SCRegBase) & SC_MASK_OP2_V;
    else
        u16Rtn = msRead2Byte(SC0F_14+u32SCRegBase) & SC_MASK_OP2_V;

    return u16Rtn;
}

void msDrvOP2SetImageHStart(BYTE u8SCDetWin, WORD u16Value)
{
    DWORD u32SCRegBase = SCRegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        msWrite2Byte(SC10_10+u32SCRegBase, (u16Value)&SC_MASK_OP2_H);
    else
        msWrite2Byte(SC0F_0E+u32SCRegBase, (u16Value)&SC_MASK_OP2_H);
}

void msDrvOP2SetImageHEnd(BYTE u8SCDetWin, WORD u16Value)
{
    DWORD u32SCRegBase = SCRegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        msWrite2Byte(SC10_12+u32SCRegBase, (u16Value)&SC_MASK_OP2_H);
    else
        msWrite2Byte(SC0F_10+u32SCRegBase, (u16Value)&SC_MASK_OP2_H);
}

void msDrvOP2SetImageVStart(BYTE u8SCDetWin, WORD u16Value)
{
    DWORD u32SCRegBase = SCRegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        msWrite2Byte(SC10_14+u32SCRegBase, (u16Value)&SC_MASK_OP2_V);
    else
        msWrite2Byte(SC0F_12+u32SCRegBase, (u16Value)&SC_MASK_OP2_V);
}

void msDrvOP2SetImageVEnd(BYTE u8SCDetWin, WORD u16Value)
{
    DWORD u32SCRegBase = SCRegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        msWrite2Byte(SC10_16+u32SCRegBase, (u16Value)&SC_MASK_OP2_V);
    else
        msWrite2Byte(SC0F_14+u32SCRegBase, (u16Value)&SC_MASK_OP2_V);
}

BOOL msDrvSrcIsInterlaceMode( BYTE InputPort )
{
    BOOL u8Rtn;
    BYTE u8DetSCIndex = MapPort2DetScIndex(InputPort);
    WORD usDPHTotal = 0, usDPVTotal = 0;
	 msAPI_combo_IPGetDPHVInformation((EN_COMBO_IP_SELECT)g_InputPort[InputPort].eIPMux, &usDPHTotal, &usDPVTotal);

    if( ( (msDrvIP1ReadSyncStatus(u8DetSCIndex)&INTM_B)&& (!INPUT_IS_HDMI_DRR(InputPort))
#if ENABLE_FORCE_4K_PMODE
       && (msDrvIP1ReadUserForceToProgressOrInterless(u8DetSCIndex) != 0x01)
#endif
        )
#if ENABLE_DP_INPUT
#if DP_ASTRO_INTERLACE_PATCH
    ||(INPUT_IS_DISPLAYPORT(InputPort)&&((msAPI_combo_IPGetDPInterlaceFlag((EN_COMBO_IP_SELECT)g_InputPort[InputPort].eIPMux))||((msAPI_combo_IPGetDPAstorPTGInterlacePatch((EN_COMBO_IP_SELECT)g_InputPort[InputPort].eIPMux)) && (((msDrvIP1ReadSyncStatus(u8DetSCIndex)&INTM_B)||(usDPVTotal > (1200 /2))))) || (msAPI_combo_IPGetDPAstorPTGInterlaceFieldPatch((EN_COMBO_IP_SELECT)g_InputPort[InputPort].eIPMux))))
#else
    ||(INPUT_IS_DISPLAYPORT(InputPort)&&(msAPI_combo_IPGetDPInterlaceFlag((EN_COMBO_IP_SELECT)g_InputPort[InputPort].eIPMux)))
#endif
#endif
    )
        u8Rtn = TRUE;
    else
        u8Rtn = FALSE;

    return u8Rtn;
}

/******************************************************************************/
///Set H/V sync status check enable/disable
///@return
/******************************************************************************/

void msDrvIP1SetHVSyncCheck(MS_BOOL bEnable, WORD u16SCMask)
{
    DWORD u32SCIP1RegBase;
    BYTE  u8SCIdx = 0;

    while(u16SCMask!=0)
    {
        if(u16SCMask & BIT0)
        {
            u32SCIP1RegBase = SCIP1RegBase(u8SCIdx);
            if(u8SCIdx & BIT0)
            {
                msWriteBit(SC01_90+u32SCIP1RegBase, bEnable, BIT(6));
            }
            else
            {
                msWriteBit(SC03_90+u32SCIP1RegBase, bEnable, BIT(6));
            }
        }
        u16SCMask >>=1;
        u8SCIdx ++;
    }
}

/******************************************************************************/
///Set new mode interlaced detect enable/disable
///@return
/******************************************************************************/
void msDrvIP1SetNewInterlaceDetect(MS_BOOL bEnable, WORD u16SCMask)
{
    DWORD u32SCIP1RegBase;
    BYTE  u8SCIdx = 0;

    while(u16SCMask!=0)
    {
        if(u16SCMask & BIT0)
        {
            u32SCIP1RegBase = SCIP1RegBase(u8SCIdx);
            if(u8SCIdx & BIT0)
            {
                msWriteBit(SC01_90+u32SCIP1RegBase, bEnable, BIT(0));
            }
            else
            {
                msWriteBit(SC03_90+u32SCIP1RegBase, bEnable, BIT(0));
            }
        }
        u16SCMask >>=1;
        u8SCIdx ++;
    }
}
/******************************************************************************/
///Set new mode interlaced detect enable/disable
///@return
/******************************************************************************/
Bool msDrvIP1ReadNewInterlaceDetect(BYTE u16SCMask)
{
    Bool bstatus=FALSE;
    DWORD u32SCIP1RegBase;
    BYTE  u8SCIdx = 0;

    while(u16SCMask!=0)
    {
        if(u16SCMask & BIT0)
        {
            u32SCIP1RegBase = SCIP1RegBase(u8SCIdx);
            if(u8SCIdx & BIT0)
            {
                if(msReadByte(SC01_90+u32SCIP1RegBase)& BIT0)
                    bstatus=TRUE;
            }
            else
            {
                if(msReadByte(SC03_90+u32SCIP1RegBase)& BIT0)
                    bstatus=TRUE;
            }
        }
        u16SCMask >>=1;
        u8SCIdx ++;
    }
    return bstatus;
}

/******************************************************************************/
///Set H/V sync source select for mode detection
///@return
/******************************************************************************/
void msDrvIP1SyncSourceSelect(BYTE u8Source, WORD u16SCMask)
{
    DWORD u32SCIP1RegBase;
    BYTE  u8SCIdx = 0;

    while(u16SCMask!=0)
    {
        if(u16SCMask & BIT0)
        {
            u32SCIP1RegBase = SCIP1RegBase(u8SCIdx);
            if(u8SCIdx & BIT0)
            {
                msWriteByteMask(SC01_90+u32SCIP1RegBase, (u8Source & 0x3) << 1, BIT(2)|BIT(1));
            }
            else
            {
                msWriteByteMask(SC03_90+u32SCIP1RegBase, (u8Source & 0x3) << 1, BIT(2)|BIT(1));
            }
        }
        u16SCMask >>=1;
        u8SCIdx ++;
    }
}

/******************************************************************************/
///Set V total count by pixel clock enable/disable
///@return
/******************************************************************************/
void msDrvIP1SetVttCountbyPixel(MS_BOOL bEnable, WORD u16SCMask)
{
    DWORD u32SCIP1RegBase;
    BYTE  u8SCIdx = 0;

    while(u16SCMask!=0)
    {
        if(u16SCMask & BIT0)
        {
            u32SCIP1RegBase = SCIP1RegBase(u8SCIdx);
            if(u8SCIdx & BIT0)
            {
                msWriteBit(SC01_90+u32SCIP1RegBase, bEnable, BIT(3));
            }
            else
            {
                msWriteBit(SC02_90+u32SCIP1RegBase, bEnable, BIT(3));
            }
        }
        u16SCMask >>=1;
        u8SCIdx ++;
    }
}

/******************************************************************************/
///Set V sync invert
///@return
/******************************************************************************/
void msDrvIP1SetVsyncInvert(MS_BOOL bInvert, WORD u16SCMask)
{
    DWORD u32SCIP1RegBase;
    BYTE  u8SCIdx = 0;

    while(u16SCMask!=0)
    {
        if(u16SCMask & BIT0)
        {
            u32SCIP1RegBase = SCIP1RegBase(u8SCIdx);
            if(u8SCIdx & BIT0)
            {
                msWriteBit(SC01_90+u32SCIP1RegBase, bInvert , BIT(4));
            }
            else
            {
                msWriteBit(SC03_90+u32SCIP1RegBase, bInvert , BIT(4));
            }
        }
        u16SCMask >>=1;
        u8SCIdx ++;
    }
}

/******************************************************************************/
///Set H sync invert
///@return
/******************************************************************************/
void msDrvIP1SetHsyncInvert(MS_BOOL bInvert,  WORD u16SCMask)
{

    DWORD u32SCIP1RegBase;
    BYTE  u8SCIdx = 0;

    while(u16SCMask!=0)
    {
        if(u16SCMask & BIT0)
        {
            u32SCIP1RegBase = SCIP1RegBase(u8SCIdx);
            if(u8SCIdx & BIT0)
            {
                msWriteBit(SC01_90+u32SCIP1RegBase, bInvert, BIT(5));
            }
            else
            {
                msWriteBit(SC03_90+u32SCIP1RegBase, bInvert, BIT(5));
            }
        }
        u16SCMask >>=1;
        u8SCIdx ++;
    }
}

void msDrvIP1Init(void)
{
#if (CHIP_ID == MST9U4)
    BYTE i;
   // For Each Scaler IP1 Detection Module Init.
    for( i = SCIP1_0; i<SCIP1_MAX_NUMS ; i++)
    {
        DWORD u32SCIP1RegBase = REG_130200 + i*0x200;
        msWrite2Byte(SC01_20 + u32SCIP1RegBase, 0x0001|(AOVDV_VALUE<<12)); // Auto position enable
        msWriteByteMask(SC01_52 + u32SCIP1RegBase, BIT5, BIT5|BIT4);  //Interlace detect mode
    }
#endif

}
#if (CHIP_ID == MST9U4)
// Mapping to external IP1 register base.
DWORD msDrvIP1RegBaseMapping(BYTE u8SCDetWin)
{
    DWORD dwRegBase=0;
    BYTE ucInputPort = MapScaler2Port(u8SCDetWin);

    switch(g_InputPort[ucInputPort].eIPMux)
    {
        case MUX_COMBO0: // MST9U4:H0
            dwRegBase = REG_130A00; // IP_H4
            break;
        case MUX_COMBO1: // MST9U4:H1
            dwRegBase = REG_130C00; // IP_H5
            break;
        case MUX_COMBO2: // MST9U4:H2
            dwRegBase = REG_130E00; // IP_H6
            break;
        case MUX_COMBO3: // MST9U4:H3
            dwRegBase = REG_131000; // IP_H7
            break;
        case MUX_COMBO4: // MST9U4:DP0
            dwRegBase = REG_130200; // IP_H0
            break;
        case MUX_COMBO5: // MST9U4:DP1
            dwRegBase = REG_130400; // IP_H1
            break;
        case MUX_COMBO6: // MST9U4:DP2
            dwRegBase = REG_130600; // IP_H2
            break;
        case MUX_COMBO7: // MST9U4:VB1/LVDS
            dwRegBase = REG_131200; // IP_H8
            break;
        default:
            break;
    }

    return dwRegBase;
}

//1p to 4p pack mode   00: bypass   01: 1p-4p   10: 2p-4p
void msDrvIP2Set4PMode(BYTE u8SCDetWin, IP4P_MODE eIP4PMode)
{
    DWORD u32SCRegBase = SCRegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        msWriteByteMask(SC02_15+u32SCRegBase, eIP4PMode<<3, BIT4|BIT3);
    else
        msWriteByteMask(SC04_15+u32SCRegBase, eIP4PMode<<3, BIT4|BIT3);
}
void msMOD8PTestPattern(Bool bLocalTgenEnable, MODTestPattern eTestPattern, ReplaceSignalByMODtgen eTgenReplaceSel)
{
    WORD u16PnlHDEStart = PanelHStart/(msDrvISSCDualPort() ? 2:1);
    WORD u16PnlHDEEnd = PanelHStart/(msDrvISSCDualPort() ? 2:1) + PANEL_WIDTH/(msDrvISSCDualPort() ? 2:1) -1;
    WORD u16PnlVDEStart = (g_ScalerInfo[SCL0_MAIN].u8SiFbMode ? VERTICAL_DE_START:0x00);
    WORD u16PnlVDEEnd = u16PnlVDEStart + PANEL_HEIGHT - 1;
    switch (eTestPattern)
    {
        case Pure_Black_TesetPattern:
            msWrite2ByteMask(REG_MOD1_D2, 0x000, 0x3FF);
            msWrite2ByteMask(REG_MOD1_D4, 0x000, 0x3FF);
            msWrite2ByteMask(REG_MOD1_D6, 0x000, 0x3FF);
            msWrite2ByteMask(REG_MOD1_D1, BIT4, BIT4|BIT3|BIT2|BIT1|BIT0);
        break;
        case Pure_White_TesetPattern:
            msWrite2ByteMask(REG_MOD1_D2, 0x3FF, 0x3FF);
            msWrite2ByteMask(REG_MOD1_D4, 0x3FF, 0x3FF);
            msWrite2ByteMask(REG_MOD1_D6, 0x3FF, 0x3FF);
            msWrite2ByteMask(REG_MOD1_D1, BIT4, BIT4|BIT3|BIT2|BIT1|BIT0);
        break;
        case Pure_Red_TesetPattern:
            msWrite2ByteMask(REG_MOD1_D2, 0x3FF, 0x3FF);
            msWrite2ByteMask(REG_MOD1_D4, 0x000, 0x3FF);
            msWrite2ByteMask(REG_MOD1_D6, 0x000, 0x3FF);
            msWrite2ByteMask(REG_MOD1_D1, BIT4, BIT4|BIT3|BIT2|BIT1|BIT0);
        break;
        case Pure_Green_TesetPattern:
            msWrite2ByteMask(REG_MOD1_D2, 0x000, 0x3FF);
            msWrite2ByteMask(REG_MOD1_D4, 0x3FF, 0x3FF);
            msWrite2ByteMask(REG_MOD1_D6, 0x000, 0x3FF);
            msWrite2ByteMask(REG_MOD1_D1, BIT4, BIT4|BIT3|BIT2|BIT1|BIT0);
        break;
        case Pure_Blue_TesetPattern:
            msWrite2ByteMask(REG_MOD1_D2, 0x000, 0x3FF);
            msWrite2ByteMask(REG_MOD1_D4, 0x000, 0x3FF);
            msWrite2ByteMask(REG_MOD1_D6, 0x3FF, 0x3FF);
            msWriteByteMask(REG_MOD1_D1, BIT4, BIT4|BIT3|BIT2|BIT1|BIT0);
        break;
        case Vertical_GrayScale_TesetPattern:
            msWrite2ByteMask(REG_MOD1_D8, u16PnlVDEStart, 0xFFF);
            msWrite2ByteMask(REG_MOD1_DA, u16PnlVDEEnd, 0xFFF);
            msWrite2ByteMask(REG_MOD1_D1, BIT2, BIT4|BIT3|BIT2|BIT1|BIT0);
        break;
        case Horizontal_GrayScale_TesetPattern:
            msWrite2ByteMask(REG_MOD1_DC, u16PnlHDEStart, 0xFFF);
            msWrite2ByteMask(REG_MOD1_DE, u16PnlHDEEnd, 0xFFF);
            msWrite2ByteMask(REG_MOD1_D1, BIT3, BIT4|BIT3|BIT2|BIT1|BIT0);
        break;
        default:
            msWrite2ByteMask(REG_MOD1_D1, 0x00, BIT4|BIT3|BIT2|BIT1|BIT0);
        break;                                                                                  \
    }
    //Local TGEN
    msWriteByteMask(REG_MOD1_31, BIT0, BIT0); //DB
    msWriteByteMask(REG_MOD1_30, msDrvISSCDualPort()?(BIT3|BIT2):(BIT2), BIT3|BIT2);
    msWriteByteMask(REG_MOD1_30, eTgenReplaceSel<<4, 0xF0);

    msWriteBit(REG_MOD1_30, 0, BIT1);
    ForceDelay1ms(1);
    msWriteBit(REG_MOD1_30, 1, BIT1);

    msWrite2ByteMask(REG_MOD1_40, PanelHTotal, 0xFFF);
    msWrite2ByteMask(REG_MOD1_44, PanelHSyncWidth, 0xFFF);
    msWrite2ByteMask(REG_MOD1_48, PanelHStart, 0xFFF);
    msWrite2ByteMask(REG_MOD1_4C, PanelWidth, 0xFFF);

    msWrite2ByteMask(REG_MOD1_40, PanelVTotal, 0xFFF);
    msWrite2ByteMask(REG_MOD1_44, PanelVSyncWidth, 0xFFF);
    msWrite2ByteMask(REG_MOD1_48, PanelVStart, 0xFFF);
    msWrite2ByteMask(REG_MOD1_4C, PanelHeight, 0xFFF);
    msWriteByteMask(REG_MOD1_30, bLocalTgenEnable, BIT0);

}
#endif

void msDrvIP22ndHCropEn(BYTE u8SCDetWin, BOOL bEnable)
{
    DWORD u32SCRegBase = SCRegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        msWriteByteMask(SC4C_16+u32SCRegBase, bEnable?BIT0:0, BIT0);
    else
        msWriteByteMask(SC65_16+u32SCRegBase, bEnable?BIT0:0, BIT0);
}

void msDrvIP22ndVCropEn(BYTE u8SCDetWin, BOOL bEnable)
{
    DWORD u32SCRegBase = SCRegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        msWriteByteMask(SC4C_16+u32SCRegBase, bEnable?BIT1:0, BIT1);
    else
        msWriteByteMask(SC65_16+u32SCRegBase, bEnable?BIT1:0, BIT1);
}

void msDrvIP22ndSetImageHStart(BYTE u8SCDetWin, WORD u16Value)
{
    DWORD u32SCRegBase = SCRegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        msWrite2Byte(SC4C_1A+u32SCRegBase, (u16Value)&SC_MASK_IP2_CapHST);
    else
        msWrite2Byte(SC65_1A+u32SCRegBase, (u16Value)&SC_MASK_IP2_CapHST);
}

void msDrvIP22ndSetImageVStart(BYTE u8SCDetWin, WORD u16Value)
{
    DWORD u32SCRegBase = SCRegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        msWrite2Byte(SC4C_18+u32SCRegBase, (u16Value)&SC_MASK_IP2_CapVST);
    else
        msWrite2Byte(SC65_18+u32SCRegBase, (u16Value)&SC_MASK_IP2_CapVST);
}

void msDrvIP22ndSetImageWidth(BYTE u8SCDetWin, WORD u16Value)
{
    DWORD u32SCRegBase = SCRegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        msWrite2Byte(SC4C_1E+u32SCRegBase, (u16Value)&SC_MASK_IP2_CapHDC);
    else
        msWrite2Byte(SC65_1E+u32SCRegBase, (u16Value)&SC_MASK_IP2_CapHDC);
}

void msDrvIP22ndSetImageHeight(BYTE u8SCDetWin, WORD u16Value)
{
    DWORD u32SCRegBase = SCRegBase(u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
        msWrite2Byte(SC4C_1C+u32SCRegBase, (u16Value)&SC_MASK_IP2_CapVDC);
    else
        msWrite2Byte(SC65_1C+u32SCRegBase, (u16Value)&SC_MASK_IP2_CapVDC);
}

#endif

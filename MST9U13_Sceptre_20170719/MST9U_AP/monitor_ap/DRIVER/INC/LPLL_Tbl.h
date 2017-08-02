#ifndef _LPLL_H_
#define _LPLL_H_


#if CHIP_ID == MST9U3
#define LPLL_REG_NUM          19
#elif CHIP_ID == MST9U4
#define LPLL_REG_NUM          27
#else
#define LPLL_REG_NUM          18
#endif

typedef struct
{
    MS_U8  address;
    MS_U16 value;
    MS_U16 mask;
}TBLStruct,*pTBLStruct;


#if ( CHIP_ID == MST9U3)
typedef enum
{
#if PANEL_TTL
    E_PNL_SUPPORTED_LPLL_TTL_75to150MHz,          //0
    E_PNL_SUPPORTED_LPLL_TTL_40to75MHz,          //1
    E_PNL_SUPPORTED_LPLL_TTL_25to40MHz,          //2
    E_PNL_SUPPORTED_LPLL_TTL_25to25MHz,          //3
#elif PANEL_LVDS_1CH
    E_PNL_SUPPORTED_LPLL_LVDS_1ch_1P_75to150MHz,          //4
    E_PNL_SUPPORTED_LPLL_LVDS_1ch_1P_75to75MHz,          //5
#elif PANEL_LVDS_2CH
    E_PNL_SUPPORTED_LPLL_LVDS_2ch_2P_75to75MHz,          //6

    E_PNL_SUPPORTED_LPLL_LVDS_2ch_1P_150to300MHz,          //7
    E_PNL_SUPPORTED_LPLL_LVDS_2ch_1P_150to150MHz,          //8
#elif PANEL_LVDS_4CH
    E_PNL_SUPPORTED_LPLL_LVDS_4ch_1P_300to300MHz,          //9

    E_PNL_SUPPORTED_LPLL_LVDS_4ch_2P_150to300MHz,          //10
    E_PNL_SUPPORTED_LPLL_LVDS_4ch_2P_150to150MHz,          //11
#elif PANEL_VBY1_1CH_10Bit
    E_PNL_SUPPORTED_LPLL_VBY1_1ch_10bit_1P_37_5to75MHz,          //12
    E_PNL_SUPPORTED_LPLL_VBY1_1ch_10bit_1P_37_5to37_5MHz,          //13
#elif PANEL_VBY1_1CH_8Bit
    E_PNL_SUPPORTED_LPLL_VBY1_1ch_8bit_1P_37_5to75MHz,          //14
    E_PNL_SUPPORTED_LPLL_VBY1_1ch_8bit_1P_37_5to37_5MHz,          //15
#elif PANEL_VBY1_2CH_10Bit
    E_PNL_SUPPORTED_LPLL_VBY1_2ch_10bit_1P_75to150MHz,          //16
    E_PNL_SUPPORTED_LPLL_VBY1_2ch_10bit_1P_75to75MHz,          //17
#elif PANEL_VBY1_2CH_8Bit
    E_PNL_SUPPORTED_LPLL_VBY1_2ch_8bit_1P_75to150MHz,          //18
    E_PNL_SUPPORTED_LPLL_VBY1_2ch_8bit_1P_75to75MHz,          //19
#elif PANEL_VBY1_4CH_10Bit
    E_PNL_SUPPORTED_LPLL_VBY1_4ch_10bit_1P_150to300MHz,          //20
    E_PNL_SUPPORTED_LPLL_VBY1_4ch_10bit_1P_150to150MHz,          //21

    E_PNL_SUPPORTED_LPLL_VBY1_4ch_10bit_2P_75to150MHz,          //22
    E_PNL_SUPPORTED_LPLL_VBY1_4ch_10bit_2P_75to75MHz,          //23
#elif PANEL_VBY1_4CH_8Bit
    E_PNL_SUPPORTED_LPLL_VBY1_4ch_8bit_1P_150to300MHz,          //24
    E_PNL_SUPPORTED_LPLL_VBY1_4ch_8bit_1P_150to150MHz,          //25

    E_PNL_SUPPORTED_LPLL_VBY1_4ch_8bit_2P_75to150MHz,          //26
    E_PNL_SUPPORTED_LPLL_VBY1_4ch_8bit_2P_75to75MHz,          //27
#elif PANEL_VBY1_8CH_10Bit
    E_PNL_SUPPORTED_LPLL_VBY1_8ch_10bit_2P_150to300MHz,          //28
    E_PNL_SUPPORTED_LPLL_VBY1_8ch_10bit_2P_150to150MHz,          //29
#elif PANEL_VBY1_8CH_8Bit
    E_PNL_SUPPORTED_LPLL_VBY1_8ch_8bit_2P_150to300MHz,          //30
    E_PNL_SUPPORTED_LPLL_VBY1_8ch_8bit_2P_150to150MHz,          //31
#elif PANEL_EDP
    E_PNL_SUPPORTED_LPLL_eDP_High_bit_rate_150to300MHz,          //32
    E_PNL_SUPPORTED_LPLL_eDP_High_bit_rate_75to150MHz,          //33
    E_PNL_SUPPORTED_LPLL_eDP_High_bit_rate_37_5to75MHz,          //34
    E_PNL_SUPPORTED_LPLL_eDP_High_bit_rate_20to37_5MHz,          //35
    E_PNL_SUPPORTED_LPLL_eDP_High_bit_rate_12_5to20MHz,          //36
    E_PNL_SUPPORTED_LPLL_eDP_High_bit_rate_12_5to12_5MHz,          //37

    E_PNL_SUPPORTED_LPLL_eDP_Reduce_bit_rate_150to300MHz,          //38
    E_PNL_SUPPORTED_LPLL_eDP_Reduce_bit_rate_75to150MHz,          //39
    E_PNL_SUPPORTED_LPLL_eDP_Reduce_bit_rate_37_5to75MHz,          //40
    E_PNL_SUPPORTED_LPLL_eDP_Reduce_bit_rate_20to37_5MHz,          //41
    E_PNL_SUPPORTED_LPLL_eDP_Reduce_bit_rate_12_5to20MHz,          //42
    E_PNL_SUPPORTED_LPLL_eDP_Reduce_bit_rate_12_5to12_5MHz,          //43
#endif
#if 0
    E_PNL_SUPPORTED_LPLL_LVDS_1ch_1P_SKEW_103to150MHz,          //44
    E_PNL_SUPPORTED_LPLL_LVDS_1ch_1P_SKEW_75to103MHz,          //45
    E_PNL_SUPPORTED_LPLL_LVDS_1ch_1P_SKEW_75to75MHz,          //46

    E_PNL_SUPPORTED_LPLL_LVDS_2ch_2P_SKEW_75to75MHz,          //47

    E_PNL_SUPPORTED_LPLL_LVDS_2ch_1P_SKEW_206to300MHz,          //48
    E_PNL_SUPPORTED_LPLL_LVDS_2ch_1P_SKEW_150to206MHz,          //49
    E_PNL_SUPPORTED_LPLL_LVDS_2ch_1P_SKEW_150to150MHz,          //50

    E_PNL_SUPPORTED_LPLL_LVDS_4ch_1P_SKEW_300to300MHz,          //51

    E_PNL_SUPPORTED_LPLL_LVDS_4ch_2P_SKEW_206to300MHz,          //52
    E_PNL_SUPPORTED_LPLL_LVDS_4ch_2P_SKEW_150to206MHz,          //53
    E_PNL_SUPPORTED_LPLL_LVDS_4ch_2P_SKEW_150to150MHz,          //54

    E_PNL_SUPPORTED_LPLL_LVDS_FT_SKEW_300to300MHz,          //55

    E_PNL_SUPPORTED_LPLL_eDP 270MHz_270to270MHz,          //56
#endif
    E_PNL_SUPPORTED_LPLL_MAX,          //57
} E_PNL_SUPPORTED_LPLL_TYPE;

BYTE u8LoopGain[E_PNL_SUPPORTED_LPLL_MAX]=
{
#if PANEL_TTL
    32,          // //E_PNL_SUPPORTED_LPLL_TTL_75to150MHz    NO.0
    32,          // //E_PNL_SUPPORTED_LPLL_TTL_40to75MHz    NO.1
    32,          // //E_PNL_SUPPORTED_LPLL_TTL_25to40MHz    NO.2
    32,          // //E_PNL_SUPPORTED_LPLL_TTL_25to25MHz    NO.3
#elif PANEL_LVDS_1CH
    32,          // //E_PNL_SUPPORTED_LPLL_LVDS-1ch_1P_75to150MHz    NO.4
    32,          // //E_PNL_SUPPORTED_LPLL_LVDS-1ch_1P_75to75MHz    NO.5
#elif PANEL_LVDS_2CH
    32,          // //E_PNL_SUPPORTED_LPLL_LVDS-2ch_2P_75to75MHz    NO.6
    32,          // //E_PNL_SUPPORTED_LPLL_LVDS-2ch_1P_150to300MHz    NO.7
    32,          // //E_PNL_SUPPORTED_LPLL_LVDS-2ch_1P_150to150MHz    NO.8
#elif PANEL_LVDS_4CH
    32,          // //E_PNL_SUPPORTED_LPLL_LVDS-4ch_1P_300to300MHz    NO.9
    32,          // //E_PNL_SUPPORTED_LPLL_LVDS-4ch_2P_150to300MHz    NO.10
    32,          // //E_PNL_SUPPORTED_LPLL_LVDS-4ch_2P_150to150MHz    NO.11
#elif PANEL_VBY1_1CH_10Bit
    40,          // //E_PNL_SUPPORTED_LPLL_VBY1 1ch 10bit_1P_80to150MHz    NO.12
    40,          // //E_PNL_SUPPORTED_LPLL_VBY1 1ch 10bit_1P_50to80MHz    NO.13
#elif PANEL_VBY1_1CH_8Bit
    40,          // //E_PNL_SUPPORTED_LPLL_VBY1 1ch 8bit_1P_50to50MHz    NO.14
    40,          // //E_PNL_SUPPORTED_LPLL_VBY1 1ch 8bit_1P_160to300MHz    NO.15
#elif PANEL_VBY1_2CH_10Bit
    40,          // //E_PNL_SUPPORTED_LPLL_VBY1 2ch 10bit_1P_80to160MHz    NO.16
    40,          // //E_PNL_SUPPORTED_LPLL_VBY1 2ch 10bit_1P_50to80MHz    NO.17
#elif PANEL_VBY1_2CH_8Bit
    40,          // //E_PNL_SUPPORTED_LPLL_VBY1 2ch 8bit_1P_50to50MHz    NO.18
    40,          // //E_PNL_SUPPORTED_LPLL_VBY1 2ch 8bit_1P_50to80MHz    NO.19
#elif PANEL_VBY1_4CH_10Bit
    40,          // //E_PNL_SUPPORTED_LPLL_VBY1 4ch 10bit_1P_50to50MHz    NO.20
    40,          // //E_PNL_SUPPORTED_LPLL_VBY1 4ch 10bit_1P_90to150MHz    NO.21
    40,          // //E_PNL_SUPPORTED_LPLL_VBY1 4ch 10bit_2P_50to90MHz    NO.22
    40,          // //E_PNL_SUPPORTED_LPLL_VBY1 4ch 10bit_2P_50to50MHz    NO.23
#elif PANEL_VBY1_4CH_8Bit
    40,          // //E_PNL_SUPPORTED_LPLL_VBY1 4ch 8bit_1P_90to150MHz    NO.24
    40,          // //E_PNL_SUPPORTED_LPLL_VBY1 4ch 8bit_1P_50to90MHz    NO.25
    40,          // //E_PNL_SUPPORTED_LPLL_VBY1 4ch 8bit_2P_50to50MHz    NO.26
    40,          // //E_PNL_SUPPORTED_LPLL_VBY1 4ch 8bit_2P_60to80MHz    NO.27
#elif PANEL_VBY1_8CH_10Bit
    40,          // //E_PNL_SUPPORTED_LPLL_VBY1 8ch 10bit_2P_50to60MHz    NO.28
    40,          // //E_PNL_SUPPORTED_LPLL_VBY1 8ch 10bit_2P_50to50MHz    NO.29
#elif PANEL_VBY1_8CH_8Bit
    40,          // //E_PNL_SUPPORTED_LPLL_VBY1 8ch 8bit_2P_120to150MHz    NO.30
    40,          // //E_PNL_SUPPORTED_LPLL_VBY1 8ch 8bit_2P_60to120MHz    NO.31
#elif PANEL_EDP
    32,          // //E_PNL_SUPPORTED_LPLL_eDP High bit rate_50to60MHz    NO.32
    32,          // //E_PNL_SUPPORTED_LPLL_eDP High bit rate_50to50MHz    NO.33
    32,          // //E_PNL_SUPPORTED_LPLL_eDP High bit rate_120to150MHz    NO.34
    32,          // //E_PNL_SUPPORTED_LPLL_eDP High bit rate_60to120MHz    NO.35
    32,          // //E_PNL_SUPPORTED_LPLL_eDP High bit rate_50to60MHz    NO.36
    32,          // //E_PNL_SUPPORTED_LPLL_eDP High bit rate_50to50MHz    NO.37
    32,          // //E_PNL_SUPPORTED_LPLL_eDP Reduce bit rate_80to150MHz    NO.38
    32,          // //E_PNL_SUPPORTED_LPLL_eDP Reduce bit rate_80to80MHz    NO.39
    32,          // //E_PNL_SUPPORTED_LPLL_eDP Reduce bit rate_80to150MHz    NO.40
    32,          // //E_PNL_SUPPORTED_LPLL_eDP Reduce bit rate_80to80MHz    NO.41
    32,          // //E_PNL_SUPPORTED_LPLL_eDP Reduce bit rate_100to150MHz    NO.42
    32,          // //E_PNL_SUPPORTED_LPLL_eDP Reduce bit rate_80to100MHz    NO.43
#endif
#if 0
    24,          // //E_PNL_SUPPORTED_LPLL_LVDS_1ch_1P_SKEW_103to150MHz    NO.44
    24,          // //E_PNL_SUPPORTED_LPLL_LVDS_1ch_1P_SKEW_75to103MHz    NO.45
    24,          // //E_PNL_SUPPORTED_LPLL_LVDS_1ch_1P_SKEW_75to75MHz    NO.46
    24,          // //E_PNL_SUPPORTED_LPLL_LVDS_2ch_2P_SKEW_75to75MHz    NO.47
    24,          // //E_PNL_SUPPORTED_LPLL_LVDS_2ch_1P_SKEW_206to300MHz    NO.48
    24,          // //E_PNL_SUPPORTED_LPLL_LVDS_2ch_1P_SKEW_150to206MHz    NO.49
    24,          // //E_PNL_SUPPORTED_LPLL_LVDS_2ch_1P_SKEW_150to150MHz    NO.50
    24,          // //E_PNL_SUPPORTED_LPLL_LVDS_4ch_1P_SKEW_300to300MHz    NO.51
    24,          // //E_PNL_SUPPORTED_LPLL_LVDS_4ch_2P_SKEW_206to300MHz    NO.52
    24,          // //E_PNL_SUPPORTED_LPLL_LVDS_4ch_2P_SKEW_150to206MHz    NO.53
    24,          // //E_PNL_SUPPORTED_LPLL_LVDS_4ch_2P_SKEW_150to150MHz    NO.54
    24,          // //E_PNL_SUPPORTED_LPLL_LVDS_FT_SKEW_300to300MHz    NO.55
    32,          // //E_PNL_SUPPORTED_LPLL_eDP 270MHz_270to270MHz    NO.56
#endif
};
BYTE u8LoopDiv[E_PNL_SUPPORTED_LPLL_MAX]=
{
#if PANEL_TTL
    28,          // //E_PNL_SUPPORTED_LPLL_TTL_75to150MHz    NO.0
    56,          // //E_PNL_SUPPORTED_LPLL_TTL_40to75MHz    NO.1
    112,          // //E_PNL_SUPPORTED_LPLL_TTL_25to40MHz    NO.2
    112,          // //E_PNL_SUPPORTED_LPLL_TTL_25to25MHz    NO.3
#elif PANEL_LVDS_1CH
    28,          // //E_PNL_SUPPORTED_LPLL_LVDS-1ch_1P_75to150MHz    NO.4
    28,          // //E_PNL_SUPPORTED_LPLL_LVDS-1ch_1P_75to75MHz    NO.5
#elif PANEL_LVDS_2CH
    56,          // //E_PNL_SUPPORTED_LPLL_LVDS-2ch_2P_75to75MHz    NO.6
    14,          // //E_PNL_SUPPORTED_LPLL_LVDS-2ch_1P_150to300MHz    NO.7
    14,          // //E_PNL_SUPPORTED_LPLL_LVDS-2ch_1P_150to150MHz    NO.8
#elif PANEL_LVDS_4CH
    14,          // //E_PNL_SUPPORTED_LPLL_LVDS-4ch_1P_300to300MHz    NO.9
    14,          // //E_PNL_SUPPORTED_LPLL_LVDS-4ch_2P_150to300MHz    NO.10
    14,          // //E_PNL_SUPPORTED_LPLL_LVDS-4ch_2P_150to150MHz    NO.11
#elif PANEL_VBY1_1CH_10Bit
    80,          // //E_PNL_SUPPORTED_LPLL_VBY1 1ch 10bit_1P_80to150MHz    NO.12
    80,          // //E_PNL_SUPPORTED_LPLL_VBY1 1ch 10bit_1P_50to80MHz    NO.13
#elif PANEL_VBY1_1CH_8Bit
    60,          // //E_PNL_SUPPORTED_LPLL_VBY1 1ch 8bit_1P_50to50MHz    NO.14
    60,          // //E_PNL_SUPPORTED_LPLL_VBY1 1ch 8bit_1P_160to300MHz    NO.15
#elif PANEL_VBY1_2CH_10Bit
    40,          // //E_PNL_SUPPORTED_LPLL_VBY1 2ch 10bit_1P_80to160MHz    NO.16
    40,          // //E_PNL_SUPPORTED_LPLL_VBY1 2ch 10bit_1P_50to80MHz    NO.17
#elif PANEL_VBY1_2CH_8Bit
    30,          // //E_PNL_SUPPORTED_LPLL_VBY1 2ch 8bit_1P_50to50MHz    NO.18
    30,          // //E_PNL_SUPPORTED_LPLL_VBY1 2ch 8bit_1P_50to80MHz    NO.19
#elif PANEL_VBY1_4CH_10Bit
    20,          // //E_PNL_SUPPORTED_LPLL_VBY1 4ch 10bit_1P_50to50MHz    NO.20
    20,          // //E_PNL_SUPPORTED_LPLL_VBY1 4ch 10bit_1P_90to150MHz    NO.21
    40,          // //E_PNL_SUPPORTED_LPLL_VBY1 4ch 10bit_2P_50to90MHz    NO.22
    40,          // //E_PNL_SUPPORTED_LPLL_VBY1 4ch 10bit_2P_50to50MHz    NO.23
#elif PANEL_VBY1_4CH_8Bit
    15,          // //E_PNL_SUPPORTED_LPLL_VBY1 4ch 8bit_1P_90to150MHz    NO.24
    15,          // //E_PNL_SUPPORTED_LPLL_VBY1 4ch 8bit_1P_50to90MHz    NO.25
    30,          // //E_PNL_SUPPORTED_LPLL_VBY1 4ch 8bit_2P_50to50MHz    NO.26
    30,          // //E_PNL_SUPPORTED_LPLL_VBY1 4ch 8bit_2P_60to80MHz    NO.27
#elif PANEL_VBY1_8CH_10Bit
    20,          // //E_PNL_SUPPORTED_LPLL_VBY1 8ch 10bit_2P_50to60MHz    NO.28
    20,          // //E_PNL_SUPPORTED_LPLL_VBY1 8ch 10bit_2P_50to50MHz    NO.29
#elif PANEL_VBY1_8CH_8Bit
    15,          // //E_PNL_SUPPORTED_LPLL_VBY1 8ch 8bit_2P_120to150MHz    NO.30
    15,          // //E_PNL_SUPPORTED_LPLL_VBY1 8ch 8bit_2P_60to120MHz    NO.31
#elif PANEL_EDP
    14,          // //E_PNL_SUPPORTED_LPLL_eDP High bit rate_50to60MHz    NO.32
    28,          // //E_PNL_SUPPORTED_LPLL_eDP High bit rate_50to50MHz    NO.33
    56,          // //E_PNL_SUPPORTED_LPLL_eDP High bit rate_120to150MHz    NO.34
    112,          // //E_PNL_SUPPORTED_LPLL_eDP High bit rate_60to120MHz    NO.35
    224,          // //E_PNL_SUPPORTED_LPLL_eDP High bit rate_50to60MHz    NO.36
    224,          // //E_PNL_SUPPORTED_LPLL_eDP High bit rate_50to50MHz    NO.37
    14,          // //E_PNL_SUPPORTED_LPLL_eDP Reduce bit rate_80to150MHz    NO.38
    28,          // //E_PNL_SUPPORTED_LPLL_eDP Reduce bit rate_80to80MHz    NO.39
    56,          // //E_PNL_SUPPORTED_LPLL_eDP Reduce bit rate_80to150MHz    NO.40
    112,          // //E_PNL_SUPPORTED_LPLL_eDP Reduce bit rate_80to80MHz    NO.41
    224,          // //E_PNL_SUPPORTED_LPLL_eDP Reduce bit rate_100to150MHz    NO.42
    224,          // //E_PNL_SUPPORTED_LPLL_eDP Reduce bit rate_80to100MHz    NO.43
#endif
#if 0
    14,          // //E_PNL_SUPPORTED_LPLL_LVDS_1ch_1P_SKEW_103to150MHz    NO.44
    28,          // //E_PNL_SUPPORTED_LPLL_LVDS_1ch_1P_SKEW_75to103MHz    NO.45
    28,          // //E_PNL_SUPPORTED_LPLL_LVDS_1ch_1P_SKEW_75to75MHz    NO.46
    28,          // //E_PNL_SUPPORTED_LPLL_LVDS_2ch_2P_SKEW_75to75MHz    NO.47
    7,          // //E_PNL_SUPPORTED_LPLL_LVDS_2ch_1P_SKEW_206to300MHz    NO.48
    14,          // //E_PNL_SUPPORTED_LPLL_LVDS_2ch_1P_SKEW_150to206MHz    NO.49
    14,          // //E_PNL_SUPPORTED_LPLL_LVDS_2ch_1P_SKEW_150to150MHz    NO.50
    7,          // //E_PNL_SUPPORTED_LPLL_LVDS_4ch_1P_SKEW_300to300MHz    NO.51
    7,          // //E_PNL_SUPPORTED_LPLL_LVDS_4ch_2P_SKEW_206to300MHz    NO.52
    14,          // //E_PNL_SUPPORTED_LPLL_LVDS_4ch_2P_SKEW_150to206MHz    NO.53
    14,          // //E_PNL_SUPPORTED_LPLL_LVDS_4ch_2P_SKEW_150to150MHz    NO.54
    7,          // //E_PNL_SUPPORTED_LPLL_LVDS_FT_SKEW_300to300MHz    NO.55
    14,          // //E_PNL_SUPPORTED_LPLL_eDP 270MHz_270to270MHz    NO.56
#endif
};

code TBLStruct LPLLSettingTBL[E_PNL_SUPPORTED_LPLL_MAX][LPLL_REG_NUM]=
{
#if PANEL_TTL
    { //E_PNL_SUPPORTED_LPLL_TTL_75to150MHz    NO.0
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0002,0x0003},//reg_lpll1_pd
        {0x03,0x0010,0x001C},//reg_lpll1_icp_ictrl
        {0x01,0x0003,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0200,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x1000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[5:4]
        {0x35,0x0000,0x7000},//reg_lpll1_output_div_second[3:0]
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div[2:0]
        {0x37,0x0000,0x0004},//reg_lpll1_fifo_div
        {0x37,0x0000,0x4000},//reg_lpll1_test[18]
        {0x2E,0x0000,0x2000},//reg_lpll1_test[30]
        {0x2E,0x0000,0x0040},//reg_lpll1_en_skew_clk
        {0x35,0x0300,0x0300},//reg_lpll1_en_fifo
        {0x2E,0x0020,0x0020},//reg_lpll1_pd_phdac[1:0]
        {0x37,0x0002,0x0002},//reg_lpll1_vco_sel
        {0x60,0x0000,0x0008},//reg_lpll1_test[17]
        {0x2E,0x0010,0x0010},//reg_lpll_en_hdmi
        {0x37,0x0000,0x2000},//reg_lpll1_en_scalar
    },

    { //E_PNL_SUPPORTED_LPLL_TTL_40to75MHz    NO.1
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0002,0x0003},//reg_lpll1_pd
        {0x03,0x0010,0x001C},//reg_lpll1_icp_ictrl
        {0x01,0x0003,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0200,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x2000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[5:4]
        {0x35,0x0000,0x7000},//reg_lpll1_output_div_second[3:0]
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div[2:0]
        {0x37,0x0000,0x0004},//reg_lpll1_fifo_div
        {0x37,0x0000,0x4000},//reg_lpll1_test[18]
        {0x2E,0x0000,0x2000},//reg_lpll1_test[30]
        {0x2E,0x0000,0x0040},//reg_lpll1_en_skew_clk
        {0x35,0x0300,0x0300},//reg_lpll1_en_fifo
        {0x2E,0x0020,0x0020},//reg_lpll1_pd_phdac[1:0]
        {0x37,0x0002,0x0002},//reg_lpll1_vco_sel
        {0x60,0x0000,0x0008},//reg_lpll1_test[17]
        {0x2E,0x0010,0x0010},//reg_lpll_en_hdmi
        {0x37,0x0000,0x2000},//reg_lpll1_en_scalar
    },

    { //E_PNL_SUPPORTED_LPLL_TTL_25to40MHz    NO.2
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0002,0x0003},//reg_lpll1_pd
        {0x03,0x0010,0x001C},//reg_lpll1_icp_ictrl
        {0x01,0x0003,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0200,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x2000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0E00,0x0F00},//reg_lpll1_output_div_second[5:4]
        {0x35,0x0000,0x7000},//reg_lpll1_output_div_second[3:0]
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div[2:0]
        {0x37,0x0000,0x0004},//reg_lpll1_fifo_div
        {0x37,0x0000,0x4000},//reg_lpll1_test[18]
        {0x2E,0x0000,0x2000},//reg_lpll1_test[30]
        {0x2E,0x0000,0x0040},//reg_lpll1_en_skew_clk
        {0x35,0x0300,0x0300},//reg_lpll1_en_fifo
        {0x2E,0x0020,0x0020},//reg_lpll1_pd_phdac[1:0]
        {0x37,0x0002,0x0002},//reg_lpll1_vco_sel
        {0x60,0x0000,0x0008},//reg_lpll1_test[17]
        {0x2E,0x0010,0x0010},//reg_lpll_en_hdmi
        {0x37,0x0000,0x2000},//reg_lpll1_en_scalar
    },

    { //E_PNL_SUPPORTED_LPLL_TTL_25to25MHz    NO.3
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0002,0x0003},//reg_lpll1_pd
        {0x03,0x0010,0x001C},//reg_lpll1_icp_ictrl
        {0x01,0x0003,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0200,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x2000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0E00,0x0F00},//reg_lpll1_output_div_second[5:4]
        {0x35,0x0000,0x7000},//reg_lpll1_output_div_second[3:0]
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div[2:0]
        {0x37,0x0000,0x0004},//reg_lpll1_fifo_div
        {0x37,0x0000,0x4000},//reg_lpll1_test[18]
        {0x2E,0x0000,0x2000},//reg_lpll1_test[30]
        {0x2E,0x0000,0x0040},//reg_lpll1_en_skew_clk
        {0x35,0x0300,0x0300},//reg_lpll1_en_fifo
        {0x2E,0x0020,0x0020},//reg_lpll1_pd_phdac[1:0]
        {0x37,0x0002,0x0002},//reg_lpll1_vco_sel
        {0x60,0x0000,0x0008},//reg_lpll1_test[17]
        {0x2E,0x0010,0x0010},//reg_lpll_en_hdmi
        {0x37,0x0000,0x2000},//reg_lpll1_en_scalar
    },
#elif PANEL_LVDS_1CH
     { //E_PNL_SUPPORTED_LPLL_LVDS_1ch_1P_75to150MHz    NO.4
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0002,0x0003},//reg_lpll1_pd
        {0x03,0x000C,0x001C},//reg_lpll1_icp_ictrl
        {0x01,0x0003,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0200,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x1000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[5:4]
        {0x35,0x0000,0x7000},//reg_lpll1_output_div_second[3:0]
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div[2:0]
        {0x37,0x0000,0x0004},//reg_lpll1_fifo_div
        {0x37,0x0000,0x4000},//reg_lpll1_test[18]
        {0x2E,0x0000,0x2000},//reg_lpll1_test[30]
        {0x2E,0x0000,0x0040},//reg_lpll1_en_skew_clk
        {0x35,0x0300,0x0300},//reg_lpll1_en_fifo
        {0x2E,0x0020,0x0020},//reg_lpll1_pd_phdac[1:0]
        {0x37,0x0000,0x0002},//reg_lpll1_vco_sel
        {0x60,0x0000,0x0008},//reg_lpll1_test[17]
        {0x2E,0x0010,0x0010},//reg_lpll_en_hdmi
        {0x37,0x0000,0x2000},//reg_lpll1_en_scalar
    },

    { //E_PNL_SUPPORTED_LPLL_LVDS_1ch_1P_75to75MHz    NO.5
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0002,0x0003},//reg_lpll1_pd
        {0x03,0x000C,0x001C},//reg_lpll1_icp_ictrl
        {0x01,0x0003,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0200,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x1000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[5:4]
        {0x35,0x0000,0x7000},//reg_lpll1_output_div_second[3:0]
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div[2:0]
        {0x37,0x0000,0x0004},//reg_lpll1_fifo_div
        {0x37,0x0000,0x4000},//reg_lpll1_test[18]
        {0x2E,0x0000,0x2000},//reg_lpll1_test[30]
        {0x2E,0x0000,0x0040},//reg_lpll1_en_skew_clk
        {0x35,0x0300,0x0300},//reg_lpll1_en_fifo
        {0x2E,0x0020,0x0020},//reg_lpll1_pd_phdac[1:0]
        {0x37,0x0000,0x0002},//reg_lpll1_vco_sel
        {0x60,0x0000,0x0008},//reg_lpll1_test[17]
        {0x2E,0x0010,0x0010},//reg_lpll_en_hdmi
        {0x37,0x0000,0x2000},//reg_lpll1_en_scalar
    },

#elif PANEL_LVDS_2CH
    { //E_PNL_SUPPORTED_LPLL_LVDS_2ch_2P_75to75MHz    NO.6
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0002,0x0003},//reg_lpll1_pd
        {0x03,0x000C,0x001C},//reg_lpll1_icp_ictrl
        {0x01,0x0003,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0200,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x2000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[5:4]
        {0x35,0x1000,0x7000},//reg_lpll1_output_div_second[3:0]
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div[2:0]
        {0x37,0x0000,0x0004},//reg_lpll1_fifo_div
        {0x37,0x0000,0x4000},//reg_lpll1_test[18]
        {0x2E,0x0000,0x2000},//reg_lpll1_test[30]
        {0x2E,0x0000,0x0040},//reg_lpll1_en_skew_clk
        {0x35,0x0300,0x0300},//reg_lpll1_en_fifo
        {0x2E,0x0020,0x0020},//reg_lpll1_pd_phdac[1:0]
        {0x37,0x0000,0x0002},//reg_lpll1_vco_sel
        {0x60,0x0000,0x0008},//reg_lpll1_test[17]
        {0x2E,0x0010,0x0010},//reg_lpll_en_hdmi
        {0x37,0x0000,0x2000},//reg_lpll1_en_scalar
    },

    { //E_PNL_SUPPORTED_LPLL_LVDS_2ch_1P_150to300MHz    NO.7
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0002,0x0003},//reg_lpll1_pd
        {0x03,0x000C,0x001C},//reg_lpll1_icp_ictrl
        {0x01,0x0003,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0200,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x0000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[5:4]
        {0x35,0x0000,0x7000},//reg_lpll1_output_div_second[3:0]
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div[2:0]
        {0x37,0x0000,0x0004},//reg_lpll1_fifo_div
        {0x37,0x0000,0x4000},//reg_lpll1_test[18]
        {0x2E,0x0000,0x2000},//reg_lpll1_test[30]
        {0x2E,0x0000,0x0040},//reg_lpll1_en_skew_clk
        {0x35,0x0300,0x0300},//reg_lpll1_en_fifo
        {0x2E,0x0020,0x0020},//reg_lpll1_pd_phdac[1:0]
        {0x37,0x0000,0x0002},//reg_lpll1_vco_sel
        {0x60,0x0000,0x0008},//reg_lpll1_test[17]
        {0x2E,0x0010,0x0010},//reg_lpll_en_hdmi
        {0x37,0x0000,0x2000},//reg_lpll1_en_scalar
    },

    { //E_PNL_SUPPORTED_LPLL_LVDS_2ch_1P_150to150MHz    NO.8
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0002,0x0003},//reg_lpll1_pd
        {0x03,0x000C,0x001C},//reg_lpll1_icp_ictrl
        {0x01,0x0003,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0200,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x0000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[5:4]
        {0x35,0x0000,0x7000},//reg_lpll1_output_div_second[3:0]
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div[2:0]
        {0x37,0x0000,0x0004},//reg_lpll1_fifo_div
        {0x37,0x0000,0x4000},//reg_lpll1_test[18]
        {0x2E,0x0000,0x2000},//reg_lpll1_test[30]
        {0x2E,0x0000,0x0040},//reg_lpll1_en_skew_clk
        {0x35,0x0300,0x0300},//reg_lpll1_en_fifo
        {0x2E,0x0020,0x0020},//reg_lpll1_pd_phdac[1:0]
        {0x37,0x0000,0x0002},//reg_lpll1_vco_sel
        {0x60,0x0000,0x0008},//reg_lpll1_test[17]
        {0x2E,0x0010,0x0010},//reg_lpll_en_hdmi
        {0x37,0x0000,0x2000},//reg_lpll1_en_scalar
    },

#elif PANEL_LVDS_4CH
    { //E_PNL_SUPPORTED_LPLL_LVDS_4ch_1P_300to300MHz    NO.9
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0002,0x0003},//reg_lpll1_pd
        {0x03,0x000C,0x001C},//reg_lpll1_icp_ictrl
        {0x01,0x0003,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0200,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x0000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[5:4]
        {0x35,0x1000,0x7000},//reg_lpll1_output_div_second[3:0]
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div[2:0]
        {0x37,0x0000,0x0004},//reg_lpll1_fifo_div
        {0x37,0x0000,0x4000},//reg_lpll1_test[18]
        {0x2E,0x0000,0x2000},//reg_lpll1_test[30]
        {0x2E,0x0000,0x0040},//reg_lpll1_en_skew_clk
        {0x35,0x0300,0x0300},//reg_lpll1_en_fifo
        {0x2E,0x0020,0x0020},//reg_lpll1_pd_phdac[1:0]
        {0x37,0x0000,0x0002},//reg_lpll1_vco_sel
        {0x60,0x0000,0x0008},//reg_lpll1_test[17]
        {0x2E,0x0010,0x0010},//reg_lpll_en_hdmi
        {0x37,0x0000,0x2000},//reg_lpll1_en_scalar
    },

    { //E_PNL_SUPPORTED_LPLL_LVDS_4ch_2P_150to300MHz    NO.10
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0002,0x0003},//reg_lpll1_pd
        {0x03,0x000C,0x001C},//reg_lpll1_icp_ictrl
        {0x01,0x0003,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0200,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x0000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[5:4]
        {0x35,0x0000,0x7000},//reg_lpll1_output_div_second[3:0]
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div[2:0]
        {0x37,0x0000,0x0004},//reg_lpll1_fifo_div
        {0x37,0x0000,0x4000},//reg_lpll1_test[18]
        {0x2E,0x0000,0x2000},//reg_lpll1_test[30]
        {0x2E,0x0000,0x0040},//reg_lpll1_en_skew_clk
        {0x35,0x0300,0x0300},//reg_lpll1_en_fifo
        {0x2E,0x0020,0x0020},//reg_lpll1_pd_phdac[1:0]
        {0x37,0x0000,0x0002},//reg_lpll1_vco_sel
        {0x60,0x0000,0x0008},//reg_lpll1_test[17]
        {0x2E,0x0010,0x0010},//reg_lpll_en_hdmi
        {0x37,0x0000,0x2000},//reg_lpll1_en_scalar
    },

    { //E_PNL_SUPPORTED_LPLL_LVDS_4ch_2P_150to150MHz    NO.11
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0002,0x0003},//reg_lpll1_pd
        {0x03,0x000C,0x001C},//reg_lpll1_icp_ictrl
        {0x01,0x0003,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0200,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x0000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[5:4]
        {0x35,0x0000,0x7000},//reg_lpll1_output_div_second[3:0]
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div[2:0]
        {0x37,0x0000,0x0004},//reg_lpll1_fifo_div
        {0x37,0x0000,0x4000},//reg_lpll1_test[18]
        {0x2E,0x0000,0x2000},//reg_lpll1_test[30]
        {0x2E,0x0000,0x0040},//reg_lpll1_en_skew_clk
        {0x35,0x0300,0x0300},//reg_lpll1_en_fifo
        {0x2E,0x0020,0x0020},//reg_lpll1_pd_phdac[1:0]
        {0x37,0x0000,0x0002},//reg_lpll1_vco_sel
        {0x60,0x0000,0x0008},//reg_lpll1_test[17]
        {0x2E,0x0010,0x0010},//reg_lpll_en_hdmi
        {0x37,0x0000,0x2000},//reg_lpll1_en_scalar
    },

#elif PANEL_VBY1_1CH_10Bit
    { //E_PNL_SUPPORTED_LPLL_VBY1_1ch_10bit_1P_37.5to75MHz    NO.12
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0003,0x0003},//reg_lpll1_pd
        {0x03,0x0010,0x001C},//reg_lpll1_icp_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0500,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x3000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0200,0x0F00},//reg_lpll1_output_div_second[5:4]
        {0x35,0x4000,0x7000},//reg_lpll1_output_div_second[3:0]
        {0x2E,0x0004,0x000F},//reg_lpll1_skew_div[2:0]
        {0x37,0x0004,0x0004},//reg_lpll1_fifo_div
        {0x37,0x4000,0x4000},//reg_lpll1_test[18]
        {0x2E,0x0000,0x2000},//reg_lpll1_test[30]
        {0x2E,0x0040,0x0040},//reg_lpll1_en_skew_clk
        {0x35,0x0300,0x0300},//reg_lpll1_en_fifo
        {0x2E,0x0020,0x0020},//reg_lpll1_pd_phdac[1:0]
        {0x37,0x0000,0x0002},//reg_lpll1_vco_sel
        {0x60,0x0000,0x0008},//reg_lpll1_test[17]
        {0x2E,0x0010,0x0010},//reg_lpll_en_hdmi
        {0x37,0x0000,0x2000},//reg_lpll1_en_scalar
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_1ch_10bit_1P_37.5to37.5MHz    NO.13
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0003,0x0003},//reg_lpll1_pd
        {0x03,0x0010,0x001C},//reg_lpll1_icp_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0500,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x3000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0200,0x0F00},//reg_lpll1_output_div_second[5:4]
        {0x35,0x4000,0x7000},//reg_lpll1_output_div_second[3:0]
        {0x2E,0x0004,0x000F},//reg_lpll1_skew_div[2:0]
        {0x37,0x0004,0x0004},//reg_lpll1_fifo_div
        {0x37,0x4000,0x4000},//reg_lpll1_test[18]
        {0x2E,0x0000,0x2000},//reg_lpll1_test[30]
        {0x2E,0x0040,0x0040},//reg_lpll1_en_skew_clk
        {0x35,0x0300,0x0300},//reg_lpll1_en_fifo
        {0x2E,0x0020,0x0020},//reg_lpll1_pd_phdac[1:0]
        {0x37,0x0000,0x0002},//reg_lpll1_vco_sel
        {0x60,0x0000,0x0008},//reg_lpll1_test[17]
        {0x2E,0x0010,0x0010},//reg_lpll_en_hdmi
        {0x37,0x0000,0x2000},//reg_lpll1_en_scalar
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_1ch_8bit_1P_37.5to75MHz    NO.14
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0003,0x0003},//reg_lpll1_pd
        {0x03,0x0010,0x001C},//reg_lpll1_icp_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0500,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x2000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0300,0x0F00},//reg_lpll1_output_div_second[5:4]
        {0x35,0x4000,0x7000},//reg_lpll1_output_div_second[3:0]
        {0x2E,0x0004,0x000F},//reg_lpll1_skew_div[2:0]
        {0x37,0x0004,0x0004},//reg_lpll1_fifo_div
        {0x37,0x4000,0x4000},//reg_lpll1_test[18]
        {0x2E,0x0000,0x2000},//reg_lpll1_test[30]
        {0x2E,0x0040,0x0040},//reg_lpll1_en_skew_clk
        {0x35,0x0300,0x0300},//reg_lpll1_en_fifo
        {0x2E,0x0020,0x0020},//reg_lpll1_pd_phdac[1:0]
        {0x37,0x0000,0x0002},//reg_lpll1_vco_sel
        {0x60,0x0000,0x0008},//reg_lpll1_test[17]
        {0x2E,0x0010,0x0010},//reg_lpll_en_hdmi
        {0x37,0x0000,0x2000},//reg_lpll1_en_scalar
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_1ch_8bit_1P_37.5to37.5MHz    NO.15
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0003,0x0003},//reg_lpll1_pd
        {0x03,0x0010,0x001C},//reg_lpll1_icp_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0500,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x2000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0300,0x0F00},//reg_lpll1_output_div_second[5:4]
        {0x35,0x4000,0x7000},//reg_lpll1_output_div_second[3:0]
        {0x2E,0x0004,0x000F},//reg_lpll1_skew_div[2:0]
        {0x37,0x0004,0x0004},//reg_lpll1_fifo_div
        {0x37,0x4000,0x4000},//reg_lpll1_test[18]
        {0x2E,0x0000,0x2000},//reg_lpll1_test[30]
        {0x2E,0x0040,0x0040},//reg_lpll1_en_skew_clk
        {0x35,0x0300,0x0300},//reg_lpll1_en_fifo
        {0x2E,0x0020,0x0020},//reg_lpll1_pd_phdac[1:0]
        {0x37,0x0000,0x0002},//reg_lpll1_vco_sel
        {0x60,0x0000,0x0008},//reg_lpll1_test[17]
        {0x2E,0x0010,0x0010},//reg_lpll_en_hdmi
        {0x37,0x0000,0x2000},//reg_lpll1_en_scalar
    },

#elif PANEL_VBY1_2CH_10Bit
    { //E_PNL_SUPPORTED_LPLL_VBY1_2ch_10bit_1P_75to150MHz    NO.16
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0003,0x0003},//reg_lpll1_pd
        {0x03,0x0010,0x001C},//reg_lpll1_icp_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0500,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x2000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0200,0x0F00},//reg_lpll1_output_div_second[5:4]
        {0x35,0x4000,0x7000},//reg_lpll1_output_div_second[3:0]
        {0x2E,0x0004,0x000F},//reg_lpll1_skew_div[2:0]
        {0x37,0x0004,0x0004},//reg_lpll1_fifo_div
        {0x37,0x4000,0x4000},//reg_lpll1_test[18]
        {0x2E,0x0000,0x2000},//reg_lpll1_test[30]
        {0x2E,0x0040,0x0040},//reg_lpll1_en_skew_clk
        {0x35,0x0300,0x0300},//reg_lpll1_en_fifo
        {0x2E,0x0020,0x0020},//reg_lpll1_pd_phdac[1:0]
        {0x37,0x0000,0x0002},//reg_lpll1_vco_sel
        {0x60,0x0000,0x0008},//reg_lpll1_test[17]
        {0x2E,0x0010,0x0010},//reg_lpll_en_hdmi
        {0x37,0x0000,0x2000},//reg_lpll1_en_scalar
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_2ch_10bit_1P_75to75MHz    NO.17
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0003,0x0003},//reg_lpll1_pd
        {0x03,0x0010,0x001C},//reg_lpll1_icp_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0500,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x2000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0200,0x0F00},//reg_lpll1_output_div_second[5:4]
        {0x35,0x4000,0x7000},//reg_lpll1_output_div_second[3:0]
        {0x2E,0x0004,0x000F},//reg_lpll1_skew_div[2:0]
        {0x37,0x0004,0x0004},//reg_lpll1_fifo_div
        {0x37,0x4000,0x4000},//reg_lpll1_test[18]
        {0x2E,0x0000,0x2000},//reg_lpll1_test[30]
        {0x2E,0x0040,0x0040},//reg_lpll1_en_skew_clk
        {0x35,0x0300,0x0300},//reg_lpll1_en_fifo
        {0x2E,0x0020,0x0020},//reg_lpll1_pd_phdac[1:0]
        {0x37,0x0000,0x0002},//reg_lpll1_vco_sel
        {0x60,0x0000,0x0008},//reg_lpll1_test[17]
        {0x2E,0x0010,0x0010},//reg_lpll_en_hdmi
        {0x37,0x0000,0x2000},//reg_lpll1_en_scalar
    },

#elif PANEL_VBY1_2CH_8Bit
   { //E_PNL_SUPPORTED_LPLL_VBY1_2ch_8bit_1P_75to150MHz    NO.18
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0003,0x0003},//reg_lpll1_pd
        {0x03,0x0010,0x001C},//reg_lpll1_icp_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0500,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x1000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0300,0x0F00},//reg_lpll1_output_div_second[5:4]
        {0x35,0x4000,0x7000},//reg_lpll1_output_div_second[3:0]
        {0x2E,0x0004,0x000F},//reg_lpll1_skew_div[2:0]
        {0x37,0x0004,0x0004},//reg_lpll1_fifo_div
        {0x37,0x4000,0x4000},//reg_lpll1_test[18]
        {0x2E,0x0000,0x2000},//reg_lpll1_test[30]
        {0x2E,0x0040,0x0040},//reg_lpll1_en_skew_clk
        {0x35,0x0300,0x0300},//reg_lpll1_en_fifo
        {0x2E,0x0020,0x0020},//reg_lpll1_pd_phdac[1:0]
        {0x37,0x0000,0x0002},//reg_lpll1_vco_sel
        {0x60,0x0000,0x0008},//reg_lpll1_test[17]
        {0x2E,0x0010,0x0010},//reg_lpll_en_hdmi
        {0x37,0x0000,0x2000},//reg_lpll1_en_scalar
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_2ch_8bit_1P_75to75MHz    NO.19
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0003,0x0003},//reg_lpll1_pd
        {0x03,0x0010,0x001C},//reg_lpll1_icp_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0500,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x1000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0300,0x0F00},//reg_lpll1_output_div_second[5:4]
        {0x35,0x4000,0x7000},//reg_lpll1_output_div_second[3:0]
        {0x2E,0x0004,0x000F},//reg_lpll1_skew_div[2:0]
        {0x37,0x0004,0x0004},//reg_lpll1_fifo_div
        {0x37,0x4000,0x4000},//reg_lpll1_test[18]
        {0x2E,0x0000,0x2000},//reg_lpll1_test[30]
        {0x2E,0x0040,0x0040},//reg_lpll1_en_skew_clk
        {0x35,0x0300,0x0300},//reg_lpll1_en_fifo
        {0x2E,0x0020,0x0020},//reg_lpll1_pd_phdac[1:0]
        {0x37,0x0000,0x0002},//reg_lpll1_vco_sel
        {0x60,0x0000,0x0008},//reg_lpll1_test[17]
        {0x2E,0x0010,0x0010},//reg_lpll_en_hdmi
        {0x37,0x0000,0x2000},//reg_lpll1_en_scalar
    },
#elif PANEL_VBY1_4CH_10Bit
   { //E_PNL_SUPPORTED_LPLL_VBY1_4ch_10bit_1P_150to300MHz    NO.20
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0003,0x0003},//reg_lpll1_pd
        {0x03,0x0010,0x001C},//reg_lpll1_icp_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0500,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x2000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[5:4]
        {0x35,0x4000,0x7000},//reg_lpll1_output_div_second[3:0]
        {0x2E,0x0004,0x000F},//reg_lpll1_skew_div[2:0]
        {0x37,0x0004,0x0004},//reg_lpll1_fifo_div
        {0x37,0x4000,0x4000},//reg_lpll1_test[18]
        {0x2E,0x0000,0x2000},//reg_lpll1_test[30]
        {0x2E,0x0040,0x0040},//reg_lpll1_en_skew_clk
        {0x35,0x0300,0x0300},//reg_lpll1_en_fifo
        {0x2E,0x0020,0x0020},//reg_lpll1_pd_phdac[1:0]
        {0x37,0x0000,0x0002},//reg_lpll1_vco_sel
        {0x60,0x0000,0x0008},//reg_lpll1_test[17]
        {0x2E,0x0010,0x0010},//reg_lpll_en_hdmi
        {0x37,0x0000,0x2000},//reg_lpll1_en_scalar
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_4ch_10bit_1P_150to150MHz    NO.21
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0003,0x0003},//reg_lpll1_pd
        {0x03,0x0010,0x001C},//reg_lpll1_icp_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0500,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x2000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[5:4]
        {0x35,0x4000,0x7000},//reg_lpll1_output_div_second[3:0]
        {0x2E,0x0004,0x000F},//reg_lpll1_skew_div[2:0]
        {0x37,0x0004,0x0004},//reg_lpll1_fifo_div
        {0x37,0x4000,0x4000},//reg_lpll1_test[18]
        {0x2E,0x0000,0x2000},//reg_lpll1_test[30]
        {0x2E,0x0040,0x0040},//reg_lpll1_en_skew_clk
        {0x35,0x0300,0x0300},//reg_lpll1_en_fifo
        {0x2E,0x0020,0x0020},//reg_lpll1_pd_phdac[1:0]
        {0x37,0x0000,0x0002},//reg_lpll1_vco_sel
        {0x60,0x0000,0x0008},//reg_lpll1_test[17]
        {0x2E,0x0010,0x0010},//reg_lpll_en_hdmi
        {0x37,0x0000,0x2000},//reg_lpll1_en_scalar
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_4ch_10bit_2P_75to150MHz    NO.22
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0003,0x0003},//reg_lpll1_pd
        {0x03,0x0010,0x001C},//reg_lpll1_icp_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0500,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x2000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0200,0x0F00},//reg_lpll1_output_div_second[5:4]
        {0x35,0x4000,0x7000},//reg_lpll1_output_div_second[3:0]
        {0x2E,0x0004,0x000F},//reg_lpll1_skew_div[2:0]
        {0x37,0x0004,0x0004},//reg_lpll1_fifo_div
        {0x37,0x4000,0x4000},//reg_lpll1_test[18]
        {0x2E,0x0000,0x2000},//reg_lpll1_test[30]
        {0x2E,0x0040,0x0040},//reg_lpll1_en_skew_clk
        {0x35,0x0300,0x0300},//reg_lpll1_en_fifo
        {0x2E,0x0020,0x0020},//reg_lpll1_pd_phdac[1:0]
        {0x37,0x0000,0x0002},//reg_lpll1_vco_sel
        {0x60,0x0000,0x0008},//reg_lpll1_test[17]
        {0x2E,0x0010,0x0010},//reg_lpll_en_hdmi
        {0x37,0x0000,0x2000},//reg_lpll1_en_scalar
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_4ch_10bit_2P_75to75MHz    NO.23
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0003,0x0003},//reg_lpll1_pd
        {0x03,0x0010,0x001C},//reg_lpll1_icp_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0500,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x2000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0200,0x0F00},//reg_lpll1_output_div_second[5:4]
        {0x35,0x4000,0x7000},//reg_lpll1_output_div_second[3:0]
        {0x2E,0x0004,0x000F},//reg_lpll1_skew_div[2:0]
        {0x37,0x0004,0x0004},//reg_lpll1_fifo_div
        {0x37,0x4000,0x4000},//reg_lpll1_test[18]
        {0x2E,0x0000,0x2000},//reg_lpll1_test[30]
        {0x2E,0x0040,0x0040},//reg_lpll1_en_skew_clk
        {0x35,0x0300,0x0300},//reg_lpll1_en_fifo
        {0x2E,0x0020,0x0020},//reg_lpll1_pd_phdac[1:0]
        {0x37,0x0000,0x0002},//reg_lpll1_vco_sel
        {0x60,0x0000,0x0008},//reg_lpll1_test[17]
        {0x2E,0x0010,0x0010},//reg_lpll_en_hdmi
        {0x37,0x0000,0x2000},//reg_lpll1_en_scalar
    },

#elif PANEL_VBY1_4CH_8Bit
   { //E_PNL_SUPPORTED_LPLL_VBY1_4ch_8bit_1P_150to300MHz    NO.24
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0003,0x0003},//reg_lpll1_pd
        {0x03,0x0010,0x001C},//reg_lpll1_icp_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0500,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x0000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0300,0x0F00},//reg_lpll1_output_div_second[5:4]
        {0x35,0x4000,0x7000},//reg_lpll1_output_div_second[3:0]
        {0x2E,0x0004,0x000F},//reg_lpll1_skew_div[2:0]
        {0x37,0x0004,0x0004},//reg_lpll1_fifo_div
        {0x37,0x4000,0x4000},//reg_lpll1_test[18]
        {0x2E,0x0000,0x2000},//reg_lpll1_test[30]
        {0x2E,0x0040,0x0040},//reg_lpll1_en_skew_clk
        {0x35,0x0300,0x0300},//reg_lpll1_en_fifo
        {0x2E,0x0020,0x0020},//reg_lpll1_pd_phdac[1:0]
        {0x37,0x0000,0x0002},//reg_lpll1_vco_sel
        {0x60,0x0000,0x0008},//reg_lpll1_test[17]
        {0x2E,0x0010,0x0010},//reg_lpll_en_hdmi
        {0x37,0x0000,0x2000},//reg_lpll1_en_scalar
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_4ch_8bit_1P_150to150MHz    NO.25
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0003,0x0003},//reg_lpll1_pd
        {0x03,0x0010,0x001C},//reg_lpll1_icp_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0500,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x0000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0300,0x0F00},//reg_lpll1_output_div_second[5:4]
        {0x35,0x4000,0x7000},//reg_lpll1_output_div_second[3:0]
        {0x2E,0x0004,0x000F},//reg_lpll1_skew_div[2:0]
        {0x37,0x0004,0x0004},//reg_lpll1_fifo_div
        {0x37,0x4000,0x4000},//reg_lpll1_test[18]
        {0x2E,0x0000,0x2000},//reg_lpll1_test[30]
        {0x2E,0x0040,0x0040},//reg_lpll1_en_skew_clk
        {0x35,0x0300,0x0300},//reg_lpll1_en_fifo
        {0x2E,0x0020,0x0020},//reg_lpll1_pd_phdac[1:0]
        {0x37,0x0000,0x0002},//reg_lpll1_vco_sel
        {0x60,0x0000,0x0008},//reg_lpll1_test[17]
        {0x2E,0x0010,0x0010},//reg_lpll_en_hdmi
        {0x37,0x0000,0x2000},//reg_lpll1_en_scalar
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_4ch_8bit_2P_75to150MHz    NO.26
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0003,0x0003},//reg_lpll1_pd
        {0x03,0x0010,0x001C},//reg_lpll1_icp_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0500,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x1000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0300,0x0F00},//reg_lpll1_output_div_second[5:4]
        {0x35,0x4000,0x7000},//reg_lpll1_output_div_second[3:0]
        {0x2E,0x0004,0x000F},//reg_lpll1_skew_div[2:0]
        {0x37,0x0004,0x0004},//reg_lpll1_fifo_div
        {0x37,0x4000,0x4000},//reg_lpll1_test[18]
        {0x2E,0x0000,0x2000},//reg_lpll1_test[30]
        {0x2E,0x0040,0x0040},//reg_lpll1_en_skew_clk
        {0x35,0x0300,0x0300},//reg_lpll1_en_fifo
        {0x2E,0x0020,0x0020},//reg_lpll1_pd_phdac[1:0]
        {0x37,0x0000,0x0002},//reg_lpll1_vco_sel
        {0x60,0x0000,0x0008},//reg_lpll1_test[17]
        {0x2E,0x0010,0x0010},//reg_lpll_en_hdmi
        {0x37,0x0000,0x2000},//reg_lpll1_en_scalar
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_4ch_8bit_2P_75to75MHz    NO.27
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0003,0x0003},//reg_lpll1_pd
        {0x03,0x0010,0x001C},//reg_lpll1_icp_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0500,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x1000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0300,0x0F00},//reg_lpll1_output_div_second[5:4]
        {0x35,0x4000,0x7000},//reg_lpll1_output_div_second[3:0]
        {0x2E,0x0004,0x000F},//reg_lpll1_skew_div[2:0]
        {0x37,0x0004,0x0004},//reg_lpll1_fifo_div
        {0x37,0x4000,0x4000},//reg_lpll1_test[18]
        {0x2E,0x0000,0x2000},//reg_lpll1_test[30]
        {0x2E,0x0040,0x0040},//reg_lpll1_en_skew_clk
        {0x35,0x0300,0x0300},//reg_lpll1_en_fifo
        {0x2E,0x0020,0x0020},//reg_lpll1_pd_phdac[1:0]
        {0x37,0x0000,0x0002},//reg_lpll1_vco_sel
        {0x60,0x0000,0x0008},//reg_lpll1_test[17]
        {0x2E,0x0010,0x0010},//reg_lpll_en_hdmi
        {0x37,0x0000,0x2000},//reg_lpll1_en_scalar
    },

#elif PANEL_VBY1_8CH_10Bit
    { //E_PNL_SUPPORTED_LPLL_VBY1_8ch_10bit_2P_150to300MHz    NO.28
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0003,0x0003},//reg_lpll1_pd
        {0x03,0x0010,0x001C},//reg_lpll1_icp_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0500,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x2000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[5:4]
        {0x35,0x4000,0x7000},//reg_lpll1_output_div_second[3:0]
        {0x2E,0x0004,0x000F},//reg_lpll1_skew_div[2:0]
        {0x37,0x0004,0x0004},//reg_lpll1_fifo_div
        {0x37,0x4000,0x4000},//reg_lpll1_test[18]
        {0x2E,0x0000,0x2000},//reg_lpll1_test[30]
        {0x2E,0x0040,0x0040},//reg_lpll1_en_skew_clk
        {0x35,0x0300,0x0300},//reg_lpll1_en_fifo
        {0x2E,0x0020,0x0020},//reg_lpll1_pd_phdac[1:0]
        {0x37,0x0000,0x0002},//reg_lpll1_vco_sel
        {0x60,0x0000,0x0008},//reg_lpll1_test[17]
        {0x2E,0x0010,0x0010},//reg_lpll_en_hdmi
        {0x37,0x0000,0x2000},//reg_lpll1_en_scalar
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_8ch_10bit_2P_150to150MHz    NO.29
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0003,0x0003},//reg_lpll1_pd
        {0x03,0x0010,0x001C},//reg_lpll1_icp_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0500,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x2000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[5:4]
        {0x35,0x4000,0x7000},//reg_lpll1_output_div_second[3:0]
        {0x2E,0x0004,0x000F},//reg_lpll1_skew_div[2:0]
        {0x37,0x0004,0x0004},//reg_lpll1_fifo_div
        {0x37,0x4000,0x4000},//reg_lpll1_test[18]
        {0x2E,0x0000,0x2000},//reg_lpll1_test[30]
        {0x2E,0x0040,0x0040},//reg_lpll1_en_skew_clk
        {0x35,0x0300,0x0300},//reg_lpll1_en_fifo
        {0x2E,0x0020,0x0020},//reg_lpll1_pd_phdac[1:0]
        {0x37,0x0000,0x0002},//reg_lpll1_vco_sel
        {0x60,0x0000,0x0008},//reg_lpll1_test[17]
        {0x2E,0x0010,0x0010},//reg_lpll_en_hdmi
        {0x37,0x0000,0x2000},//reg_lpll1_en_scalar
    },
#elif PANEL_VBY1_8CH_8Bit
    { //E_PNL_SUPPORTED_LPLL_VBY1_8ch_8bit_2P_150to300MHz    NO.30
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0003,0x0003},//reg_lpll1_pd
        {0x03,0x0010,0x001C},//reg_lpll1_icp_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0500,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x0000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0300,0x0F00},//reg_lpll1_output_div_second[5:4]
        {0x35,0x4000,0x7000},//reg_lpll1_output_div_second[3:0]
        {0x2E,0x0004,0x000F},//reg_lpll1_skew_div[2:0]
        {0x37,0x0004,0x0004},//reg_lpll1_fifo_div
        {0x37,0x4000,0x4000},//reg_lpll1_test[18]
        {0x2E,0x0000,0x2000},//reg_lpll1_test[30]
        {0x2E,0x0040,0x0040},//reg_lpll1_en_skew_clk
        {0x35,0x0300,0x0300},//reg_lpll1_en_fifo
        {0x2E,0x0020,0x0020},//reg_lpll1_pd_phdac[1:0]
        {0x37,0x0000,0x0002},//reg_lpll1_vco_sel
        {0x60,0x0000,0x0008},//reg_lpll1_test[17]
        {0x2E,0x0010,0x0010},//reg_lpll_en_hdmi
        {0x37,0x0000,0x2000},//reg_lpll1_en_scalar
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_8ch_8bit_2P_150to150MHz    NO.31
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0003,0x0003},//reg_lpll1_pd
        {0x03,0x0010,0x001C},//reg_lpll1_icp_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0500,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x0000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0300,0x0F00},//reg_lpll1_output_div_second[5:4]
        {0x35,0x4000,0x7000},//reg_lpll1_output_div_second[3:0]
        {0x2E,0x0004,0x000F},//reg_lpll1_skew_div[2:0]
        {0x37,0x0004,0x0004},//reg_lpll1_fifo_div
        {0x37,0x4000,0x4000},//reg_lpll1_test[18]
        {0x2E,0x0000,0x2000},//reg_lpll1_test[30]
        {0x2E,0x0040,0x0040},//reg_lpll1_en_skew_clk
        {0x35,0x0300,0x0300},//reg_lpll1_en_fifo
        {0x2E,0x0020,0x0020},//reg_lpll1_pd_phdac[1:0]
        {0x37,0x0000,0x0002},//reg_lpll1_vco_sel
        {0x60,0x0000,0x0008},//reg_lpll1_test[17]
        {0x2E,0x0010,0x0010},//reg_lpll_en_hdmi
        {0x37,0x0000,0x2000},//reg_lpll1_en_scalar
    },
#elif PANEL_EDP
    { //E_PNL_SUPPORTED_LPLL_eDP_High_bit_rate_150to300MHz    NO.32
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0002,0x0003},//reg_lpll1_pd
        {0x03,0x0010,0x001C},//reg_lpll1_icp_ictrl
        {0x01,0x0003,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0200,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x0000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[5:4]
        {0x35,0x0000,0x7000},//reg_lpll1_output_div_second[3:0]
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div[2:0]
        {0x37,0x0000,0x0004},//reg_lpll1_fifo_div
        {0x37,0x0000,0x4000},//reg_lpll1_test[18]
        {0x2E,0x0000,0x2000},//reg_lpll1_test[30]
        {0x2E,0x0000,0x0040},//reg_lpll1_en_skew_clk
        {0x35,0x0300,0x0300},//reg_lpll1_en_fifo
        {0x2E,0x0020,0x0020},//reg_lpll1_pd_phdac[1:0]
        {0x37,0x0002,0x0002},//reg_lpll1_vco_sel
        {0x60,0x0000,0x0008},//reg_lpll1_test[17]
        {0x2E,0x0010,0x0010},//reg_lpll_en_hdmi
        {0x37,0x0000,0x2000},//reg_lpll1_en_scalar
    },

    { //E_PNL_SUPPORTED_LPLL_eDP_High_bit_rate_75to150MHz    NO.33
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0002,0x0003},//reg_lpll1_pd
        {0x03,0x0010,0x001C},//reg_lpll1_icp_ictrl
        {0x01,0x0003,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0200,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x1000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[5:4]
        {0x35,0x0000,0x7000},//reg_lpll1_output_div_second[3:0]
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div[2:0]
        {0x37,0x0000,0x0004},//reg_lpll1_fifo_div
        {0x37,0x0000,0x4000},//reg_lpll1_test[18]
        {0x2E,0x0000,0x2000},//reg_lpll1_test[30]
        {0x2E,0x0000,0x0040},//reg_lpll1_en_skew_clk
        {0x35,0x0300,0x0300},//reg_lpll1_en_fifo
        {0x2E,0x0020,0x0020},//reg_lpll1_pd_phdac[1:0]
        {0x37,0x0002,0x0002},//reg_lpll1_vco_sel
        {0x60,0x0000,0x0008},//reg_lpll1_test[17]
        {0x2E,0x0010,0x0010},//reg_lpll_en_hdmi
        {0x37,0x0000,0x2000},//reg_lpll1_en_scalar
    },

    { //E_PNL_SUPPORTED_LPLL_eDP_High_bit_rate_37.5to75MHz    NO.34
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0002,0x0003},//reg_lpll1_pd
        {0x03,0x0010,0x001C},//reg_lpll1_icp_ictrl
        {0x01,0x0003,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0200,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x2000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[5:4]
        {0x35,0x0000,0x7000},//reg_lpll1_output_div_second[3:0]
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div[2:0]
        {0x37,0x0000,0x0004},//reg_lpll1_fifo_div
        {0x37,0x0000,0x4000},//reg_lpll1_test[18]
        {0x2E,0x0000,0x2000},//reg_lpll1_test[30]
        {0x2E,0x0000,0x0040},//reg_lpll1_en_skew_clk
        {0x35,0x0300,0x0300},//reg_lpll1_en_fifo
        {0x2E,0x0020,0x0020},//reg_lpll1_pd_phdac[1:0]
        {0x37,0x0002,0x0002},//reg_lpll1_vco_sel
        {0x60,0x0000,0x0008},//reg_lpll1_test[17]
        {0x2E,0x0010,0x0010},//reg_lpll_en_hdmi
        {0x37,0x0000,0x2000},//reg_lpll1_en_scalar
    },

    { //E_PNL_SUPPORTED_LPLL_eDP_High_bit_rate_20to37.5MHz    NO.35
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0002,0x0003},//reg_lpll1_pd
        {0x03,0x0010,0x001C},//reg_lpll1_icp_ictrl
        {0x01,0x0003,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0200,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x3000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[5:4]
        {0x35,0x0000,0x7000},//reg_lpll1_output_div_second[3:0]
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div[2:0]
        {0x37,0x0000,0x0004},//reg_lpll1_fifo_div
        {0x37,0x0000,0x4000},//reg_lpll1_test[18]
        {0x2E,0x0000,0x2000},//reg_lpll1_test[30]
        {0x2E,0x0000,0x0040},//reg_lpll1_en_skew_clk
        {0x35,0x0300,0x0300},//reg_lpll1_en_fifo
        {0x2E,0x0020,0x0020},//reg_lpll1_pd_phdac[1:0]
        {0x37,0x0002,0x0002},//reg_lpll1_vco_sel
        {0x60,0x0000,0x0008},//reg_lpll1_test[17]
        {0x2E,0x0010,0x0010},//reg_lpll_en_hdmi
        {0x37,0x0000,0x2000},//reg_lpll1_en_scalar
    },

    { //E_PNL_SUPPORTED_LPLL_eDP_High_bit_rate_12.5to20MHz    NO.36
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0002,0x0003},//reg_lpll1_pd
        {0x03,0x0010,0x001C},//reg_lpll1_icp_ictrl
        {0x01,0x0003,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0200,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x3000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0E00,0x0F00},//reg_lpll1_output_div_second[5:4]
        {0x35,0x0000,0x7000},//reg_lpll1_output_div_second[3:0]
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div[2:0]
        {0x37,0x0000,0x0004},//reg_lpll1_fifo_div
        {0x37,0x0000,0x4000},//reg_lpll1_test[18]
        {0x2E,0x0000,0x2000},//reg_lpll1_test[30]
        {0x2E,0x0000,0x0040},//reg_lpll1_en_skew_clk
        {0x35,0x0300,0x0300},//reg_lpll1_en_fifo
        {0x2E,0x0020,0x0020},//reg_lpll1_pd_phdac[1:0]
        {0x37,0x0002,0x0002},//reg_lpll1_vco_sel
        {0x60,0x0000,0x0008},//reg_lpll1_test[17]
        {0x2E,0x0010,0x0010},//reg_lpll_en_hdmi
        {0x37,0x0000,0x2000},//reg_lpll1_en_scalar
    },

    { //E_PNL_SUPPORTED_LPLL_eDP_High_bit_rate_12.5to12.5MHz    NO.37
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0002,0x0003},//reg_lpll1_pd
        {0x03,0x0010,0x001C},//reg_lpll1_icp_ictrl
        {0x01,0x0003,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0200,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x3000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0E00,0x0F00},//reg_lpll1_output_div_second[5:4]
        {0x35,0x0000,0x7000},//reg_lpll1_output_div_second[3:0]
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div[2:0]
        {0x37,0x0000,0x0004},//reg_lpll1_fifo_div
        {0x37,0x0000,0x4000},//reg_lpll1_test[18]
        {0x2E,0x0000,0x2000},//reg_lpll1_test[30]
        {0x2E,0x0000,0x0040},//reg_lpll1_en_skew_clk
        {0x35,0x0300,0x0300},//reg_lpll1_en_fifo
        {0x2E,0x0020,0x0020},//reg_lpll1_pd_phdac[1:0]
        {0x37,0x0002,0x0002},//reg_lpll1_vco_sel
        {0x60,0x0000,0x0008},//reg_lpll1_test[17]
        {0x2E,0x0010,0x0010},//reg_lpll_en_hdmi
        {0x37,0x0000,0x2000},//reg_lpll1_en_scalar
    },

    { //E_PNL_SUPPORTED_LPLL_eDP_Reduce_bit_rate_150to300MHz    NO.38
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0002,0x0003},//reg_lpll1_pd
        {0x03,0x0010,0x001C},//reg_lpll1_icp_ictrl
        {0x01,0x0003,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0200,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x0000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[5:4]
        {0x35,0x0000,0x7000},//reg_lpll1_output_div_second[3:0]
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div[2:0]
        {0x37,0x0000,0x0004},//reg_lpll1_fifo_div
        {0x37,0x0000,0x4000},//reg_lpll1_test[18]
        {0x2E,0x0000,0x2000},//reg_lpll1_test[30]
        {0x2E,0x0000,0x0040},//reg_lpll1_en_skew_clk
        {0x35,0x0300,0x0300},//reg_lpll1_en_fifo
        {0x2E,0x0020,0x0020},//reg_lpll1_pd_phdac[1:0]
        {0x37,0x0002,0x0002},//reg_lpll1_vco_sel
        {0x60,0x0000,0x0008},//reg_lpll1_test[17]
        {0x2E,0x0010,0x0010},//reg_lpll_en_hdmi
        {0x37,0x0000,0x2000},//reg_lpll1_en_scalar
    },

    { //E_PNL_SUPPORTED_LPLL_eDP_Reduce_bit_rate_75to150MHz    NO.39
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0002,0x0003},//reg_lpll1_pd
        {0x03,0x0010,0x001C},//reg_lpll1_icp_ictrl
        {0x01,0x0003,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0200,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x1000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[5:4]
        {0x35,0x0000,0x7000},//reg_lpll1_output_div_second[3:0]
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div[2:0]
        {0x37,0x0000,0x0004},//reg_lpll1_fifo_div
        {0x37,0x0000,0x4000},//reg_lpll1_test[18]
        {0x2E,0x0000,0x2000},//reg_lpll1_test[30]
        {0x2E,0x0000,0x0040},//reg_lpll1_en_skew_clk
        {0x35,0x0300,0x0300},//reg_lpll1_en_fifo
        {0x2E,0x0020,0x0020},//reg_lpll1_pd_phdac[1:0]
        {0x37,0x0002,0x0002},//reg_lpll1_vco_sel
        {0x60,0x0000,0x0008},//reg_lpll1_test[17]
        {0x2E,0x0010,0x0010},//reg_lpll_en_hdmi
        {0x37,0x0000,0x2000},//reg_lpll1_en_scalar
    },

    { //E_PNL_SUPPORTED_LPLL_eDP_Reduce_bit_rate_37.5to75MHz    NO.40
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0002,0x0003},//reg_lpll1_pd
        {0x03,0x0010,0x001C},//reg_lpll1_icp_ictrl
        {0x01,0x0003,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0200,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x2000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[5:4]
        {0x35,0x0000,0x7000},//reg_lpll1_output_div_second[3:0]
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div[2:0]
        {0x37,0x0000,0x0004},//reg_lpll1_fifo_div
        {0x37,0x0000,0x4000},//reg_lpll1_test[18]
        {0x2E,0x0000,0x2000},//reg_lpll1_test[30]
        {0x2E,0x0000,0x0040},//reg_lpll1_en_skew_clk
        {0x35,0x0300,0x0300},//reg_lpll1_en_fifo
        {0x2E,0x0020,0x0020},//reg_lpll1_pd_phdac[1:0]
        {0x37,0x0002,0x0002},//reg_lpll1_vco_sel
        {0x60,0x0000,0x0008},//reg_lpll1_test[17]
        {0x2E,0x0010,0x0010},//reg_lpll_en_hdmi
        {0x37,0x0000,0x2000},//reg_lpll1_en_scalar
    },

    { //E_PNL_SUPPORTED_LPLL_eDP_Reduce_bit_rate_20to37.5MHz    NO.41
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0002,0x0003},//reg_lpll1_pd
        {0x03,0x0010,0x001C},//reg_lpll1_icp_ictrl
        {0x01,0x0003,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0200,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x3000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[5:4]
        {0x35,0x0000,0x7000},//reg_lpll1_output_div_second[3:0]
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div[2:0]
        {0x37,0x0000,0x0004},//reg_lpll1_fifo_div
        {0x37,0x0000,0x4000},//reg_lpll1_test[18]
        {0x2E,0x0000,0x2000},//reg_lpll1_test[30]
        {0x2E,0x0000,0x0040},//reg_lpll1_en_skew_clk
        {0x35,0x0300,0x0300},//reg_lpll1_en_fifo
        {0x2E,0x0020,0x0020},//reg_lpll1_pd_phdac[1:0]
        {0x37,0x0002,0x0002},//reg_lpll1_vco_sel
        {0x60,0x0000,0x0008},//reg_lpll1_test[17]
        {0x2E,0x0010,0x0010},//reg_lpll_en_hdmi
        {0x37,0x0000,0x2000},//reg_lpll1_en_scalar
    },

    { //E_PNL_SUPPORTED_LPLL_eDP_Reduce_bit_rate_12.5to20MHz    NO.42
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0002,0x0003},//reg_lpll1_pd
        {0x03,0x0010,0x001C},//reg_lpll1_icp_ictrl
        {0x01,0x0003,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0200,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x3000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0E00,0x0F00},//reg_lpll1_output_div_second[5:4]
        {0x35,0x0000,0x7000},//reg_lpll1_output_div_second[3:0]
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div[2:0]
        {0x37,0x0000,0x0004},//reg_lpll1_fifo_div
        {0x37,0x0000,0x4000},//reg_lpll1_test[18]
        {0x2E,0x0000,0x2000},//reg_lpll1_test[30]
        {0x2E,0x0000,0x0040},//reg_lpll1_en_skew_clk
        {0x35,0x0300,0x0300},//reg_lpll1_en_fifo
        {0x2E,0x0020,0x0020},//reg_lpll1_pd_phdac[1:0]
        {0x37,0x0002,0x0002},//reg_lpll1_vco_sel
        {0x60,0x0000,0x0008},//reg_lpll1_test[17]
        {0x2E,0x0010,0x0010},//reg_lpll_en_hdmi
        {0x37,0x0000,0x2000},//reg_lpll1_en_scalar
    },

    { //E_PNL_SUPPORTED_LPLL_eDP_Reduce_bit_rate_12.5to12.5MHz    NO.43
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0002,0x0003},//reg_lpll1_pd
        {0x03,0x0010,0x001C},//reg_lpll1_icp_ictrl
        {0x01,0x0003,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0200,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x3000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0E00,0x0F00},//reg_lpll1_output_div_second[5:4]
        {0x35,0x0000,0x7000},//reg_lpll1_output_div_second[3:0]
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div[2:0]
        {0x37,0x0000,0x0004},//reg_lpll1_fifo_div
        {0x37,0x0000,0x4000},//reg_lpll1_test[18]
        {0x2E,0x0000,0x2000},//reg_lpll1_test[30]
        {0x2E,0x0000,0x0040},//reg_lpll1_en_skew_clk
        {0x35,0x0300,0x0300},//reg_lpll1_en_fifo
        {0x2E,0x0020,0x0020},//reg_lpll1_pd_phdac[1:0]
        {0x37,0x0002,0x0002},//reg_lpll1_vco_sel
        {0x60,0x0000,0x0008},//reg_lpll1_test[17]
        {0x2E,0x0010,0x0010},//reg_lpll_en_hdmi
        {0x37,0x0000,0x2000},//reg_lpll1_en_scalar
    },
#endif
#if 0
    { //E_PNL_SUPPORTED_LPLL_LVDS_1ch_1P_SKEW_103to150MHz    NO.44
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0002,0x0003},//reg_lpll1_pd
        {0x03,0x0010,0x001C},//reg_lpll1_icp_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x0000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[5:4]
        {0x35,0x4000,0x7000},//reg_lpll1_output_div_second[3:0]
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div[2:0]
        {0x37,0x0000,0x0004},//reg_lpll1_fifo_div
        {0x37,0x0000,0x4000},//reg_lpll1_test[18]
        {0x2E,0x2000,0x2000},//reg_lpll1_test[30]
        {0x2E,0x0000,0x0040},//reg_lpll1_en_skew_clk
        {0x35,0x0000,0x0300},//reg_lpll1_en_fifo
        {0x2E,0x0000,0x0020},//reg_lpll1_pd_phdac[1:0]
        {0x37,0x0000,0x0002},//reg_lpll1_vco_sel
        {0x60,0x0000,0x0008},//reg_lpll1_test[17]
        {0x2E,0x0010,0x0010},//reg_lpll_en_hdmi
        {0x37,0x0000,0x2000},//reg_lpll1_en_scalar
    },

    { //E_PNL_SUPPORTED_LPLL_LVDS_1ch_1P_SKEW_75to103MHz    NO.45
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0002,0x0003},//reg_lpll1_pd
        {0x03,0x0010,0x001C},//reg_lpll1_icp_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x1000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[5:4]
        {0x35,0x0000,0x7000},//reg_lpll1_output_div_second[3:0]
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div[2:0]
        {0x37,0x0000,0x0004},//reg_lpll1_fifo_div
        {0x37,0x0000,0x4000},//reg_lpll1_test[18]
        {0x2E,0x2000,0x2000},//reg_lpll1_test[30]
        {0x2E,0x0000,0x0040},//reg_lpll1_en_skew_clk
        {0x35,0x0000,0x0300},//reg_lpll1_en_fifo
        {0x2E,0x0000,0x0020},//reg_lpll1_pd_phdac[1:0]
        {0x37,0x0000,0x0002},//reg_lpll1_vco_sel
        {0x60,0x0000,0x0008},//reg_lpll1_test[17]
        {0x2E,0x0010,0x0010},//reg_lpll_en_hdmi
        {0x37,0x0000,0x2000},//reg_lpll1_en_scalar
    },

    { //E_PNL_SUPPORTED_LPLL_LVDS_1ch_1P_SKEW_75to75MHz    NO.46
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0002,0x0003},//reg_lpll1_pd
        {0x03,0x0010,0x001C},//reg_lpll1_icp_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x1000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[5:4]
        {0x35,0x0000,0x7000},//reg_lpll1_output_div_second[3:0]
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div[2:0]
        {0x37,0x0000,0x0004},//reg_lpll1_fifo_div
        {0x37,0x0000,0x4000},//reg_lpll1_test[18]
        {0x2E,0x2000,0x2000},//reg_lpll1_test[30]
        {0x2E,0x0000,0x0040},//reg_lpll1_en_skew_clk
        {0x35,0x0000,0x0300},//reg_lpll1_en_fifo
        {0x2E,0x0000,0x0020},//reg_lpll1_pd_phdac[1:0]
        {0x37,0x0000,0x0002},//reg_lpll1_vco_sel
        {0x60,0x0000,0x0008},//reg_lpll1_test[17]
        {0x2E,0x0010,0x0010},//reg_lpll_en_hdmi
        {0x37,0x0000,0x2000},//reg_lpll1_en_scalar
    },

    { //E_PNL_SUPPORTED_LPLL_LVDS_2ch_2P_SKEW_75to75MHz    NO.47
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0002,0x0003},//reg_lpll1_pd
        {0x03,0x0010,0x001C},//reg_lpll1_icp_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x1000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[5:4]
        {0x35,0x0000,0x7000},//reg_lpll1_output_div_second[3:0]
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div[2:0]
        {0x37,0x0000,0x0004},//reg_lpll1_fifo_div
        {0x37,0x0000,0x4000},//reg_lpll1_test[18]
        {0x2E,0x2000,0x2000},//reg_lpll1_test[30]
        {0x2E,0x0000,0x0040},//reg_lpll1_en_skew_clk
        {0x35,0x0000,0x0300},//reg_lpll1_en_fifo
        {0x2E,0x0000,0x0020},//reg_lpll1_pd_phdac[1:0]
        {0x37,0x0000,0x0002},//reg_lpll1_vco_sel
        {0x60,0x0000,0x0008},//reg_lpll1_test[17]
        {0x2E,0x0010,0x0010},//reg_lpll_en_hdmi
        {0x37,0x0000,0x2000},//reg_lpll1_en_scalar
    },

    { //E_PNL_SUPPORTED_LPLL_LVDS_2ch_1P_SKEW_206to300MHz    NO.48
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0002,0x0003},//reg_lpll1_pd
        {0x03,0x0010,0x001C},//reg_lpll1_icp_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x0000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[5:4]
        {0x35,0x4000,0x7000},//reg_lpll1_output_div_second[3:0]
        {0x2E,0x0004,0x000F},//reg_lpll1_skew_div[2:0]
        {0x37,0x0000,0x0004},//reg_lpll1_fifo_div
        {0x37,0x0000,0x4000},//reg_lpll1_test[18]
        {0x2E,0x2000,0x2000},//reg_lpll1_test[30]
        {0x2E,0x0040,0x0040},//reg_lpll1_en_skew_clk
        {0x35,0x0000,0x0300},//reg_lpll1_en_fifo
        {0x2E,0x0000,0x0020},//reg_lpll1_pd_phdac[1:0]
        {0x37,0x0000,0x0002},//reg_lpll1_vco_sel
        {0x60,0x0000,0x0008},//reg_lpll1_test[17]
        {0x2E,0x0000,0x0010},//reg_lpll_en_hdmi
        {0x37,0x2000,0x2000},//reg_lpll1_en_scalar
    },

    { //E_PNL_SUPPORTED_LPLL_LVDS_2ch_1P_SKEW_150to206MHz    NO.49
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0002,0x0003},//reg_lpll1_pd
        {0x03,0x0010,0x001C},//reg_lpll1_icp_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x0000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[5:4]
        {0x35,0x0000,0x7000},//reg_lpll1_output_div_second[3:0]
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div[2:0]
        {0x37,0x0000,0x0004},//reg_lpll1_fifo_div
        {0x37,0x0000,0x4000},//reg_lpll1_test[18]
        {0x2E,0x2000,0x2000},//reg_lpll1_test[30]
        {0x2E,0x0000,0x0040},//reg_lpll1_en_skew_clk
        {0x35,0x0000,0x0300},//reg_lpll1_en_fifo
        {0x2E,0x0000,0x0020},//reg_lpll1_pd_phdac[1:0]
        {0x37,0x0000,0x0002},//reg_lpll1_vco_sel
        {0x60,0x0000,0x0008},//reg_lpll1_test[17]
        {0x2E,0x0010,0x0010},//reg_lpll_en_hdmi
        {0x37,0x0000,0x2000},//reg_lpll1_en_scalar
    },

    { //E_PNL_SUPPORTED_LPLL_LVDS_2ch_1P_SKEW_150to150MHz    NO.50
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0002,0x0003},//reg_lpll1_pd
        {0x03,0x0010,0x001C},//reg_lpll1_icp_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x0000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[5:4]
        {0x35,0x0000,0x7000},//reg_lpll1_output_div_second[3:0]
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div[2:0]
        {0x37,0x0000,0x0004},//reg_lpll1_fifo_div
        {0x37,0x0000,0x4000},//reg_lpll1_test[18]
        {0x2E,0x2000,0x2000},//reg_lpll1_test[30]
        {0x2E,0x0000,0x0040},//reg_lpll1_en_skew_clk
        {0x35,0x0000,0x0300},//reg_lpll1_en_fifo
        {0x2E,0x0000,0x0020},//reg_lpll1_pd_phdac[1:0]
        {0x37,0x0000,0x0002},//reg_lpll1_vco_sel
        {0x60,0x0000,0x0008},//reg_lpll1_test[17]
        {0x2E,0x0010,0x0010},//reg_lpll_en_hdmi
        {0x37,0x0000,0x2000},//reg_lpll1_en_scalar
    },

    { //E_PNL_SUPPORTED_LPLL_LVDS_4ch_1P_SKEW_300to300MHz    NO.51
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0002,0x0003},//reg_lpll1_pd
        {0x03,0x0010,0x001C},//reg_lpll1_icp_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x0000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[5:4]
        {0x35,0x0000,0x7000},//reg_lpll1_output_div_second[3:0]
        {0x2E,0x0004,0x000F},//reg_lpll1_skew_div[2:0]
        {0x37,0x0000,0x0004},//reg_lpll1_fifo_div
        {0x37,0x0000,0x4000},//reg_lpll1_test[18]
        {0x2E,0x2000,0x2000},//reg_lpll1_test[30]
        {0x2E,0x0040,0x0040},//reg_lpll1_en_skew_clk
        {0x35,0x0000,0x0300},//reg_lpll1_en_fifo
        {0x2E,0x0000,0x0020},//reg_lpll1_pd_phdac[1:0]
        {0x37,0x0000,0x0002},//reg_lpll1_vco_sel
        {0x60,0x0000,0x0008},//reg_lpll1_test[17]
        {0x2E,0x0000,0x0010},//reg_lpll_en_hdmi
        {0x37,0x2000,0x2000},//reg_lpll1_en_scalar
    },

    { //E_PNL_SUPPORTED_LPLL_LVDS_4ch_2P_SKEW_206to300MHz    NO.52
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0002,0x0003},//reg_lpll1_pd
        {0x03,0x0010,0x001C},//reg_lpll1_icp_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x0000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[5:4]
        {0x35,0x4000,0x7000},//reg_lpll1_output_div_second[3:0]
        {0x2E,0x0004,0x000F},//reg_lpll1_skew_div[2:0]
        {0x37,0x0000,0x0004},//reg_lpll1_fifo_div
        {0x37,0x0000,0x4000},//reg_lpll1_test[18]
        {0x2E,0x2000,0x2000},//reg_lpll1_test[30]
        {0x2E,0x0040,0x0040},//reg_lpll1_en_skew_clk
        {0x35,0x0000,0x0300},//reg_lpll1_en_fifo
        {0x2E,0x0000,0x0020},//reg_lpll1_pd_phdac[1:0]
        {0x37,0x0000,0x0002},//reg_lpll1_vco_sel
        {0x60,0x0000,0x0008},//reg_lpll1_test[17]
        {0x2E,0x0000,0x0010},//reg_lpll_en_hdmi
        {0x37,0x2000,0x2000},//reg_lpll1_en_scalar
    },

    { //E_PNL_SUPPORTED_LPLL_LVDS_4ch_2P_SKEW_150to206MHz    NO.53
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0002,0x0003},//reg_lpll1_pd
        {0x03,0x0010,0x001C},//reg_lpll1_icp_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x0000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[5:4]
        {0x35,0x0000,0x7000},//reg_lpll1_output_div_second[3:0]
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div[2:0]
        {0x37,0x0000,0x0004},//reg_lpll1_fifo_div
        {0x37,0x0000,0x4000},//reg_lpll1_test[18]
        {0x2E,0x2000,0x2000},//reg_lpll1_test[30]
        {0x2E,0x0000,0x0040},//reg_lpll1_en_skew_clk
        {0x35,0x0000,0x0300},//reg_lpll1_en_fifo
        {0x2E,0x0000,0x0020},//reg_lpll1_pd_phdac[1:0]
        {0x37,0x0000,0x0002},//reg_lpll1_vco_sel
        {0x60,0x0000,0x0008},//reg_lpll1_test[17]
        {0x2E,0x0010,0x0010},//reg_lpll_en_hdmi
        {0x37,0x0000,0x2000},//reg_lpll1_en_scalar
    },

    { //E_PNL_SUPPORTED_LPLL_LVDS_4ch_2P_SKEW_150to150MHz    NO.54
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0002,0x0003},//reg_lpll1_pd
        {0x03,0x0010,0x001C},//reg_lpll1_icp_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x0000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[5:4]
        {0x35,0x0000,0x7000},//reg_lpll1_output_div_second[3:0]
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div[2:0]
        {0x37,0x0000,0x0004},//reg_lpll1_fifo_div
        {0x37,0x0000,0x4000},//reg_lpll1_test[18]
        {0x2E,0x2000,0x2000},//reg_lpll1_test[30]
        {0x2E,0x0000,0x0040},//reg_lpll1_en_skew_clk
        {0x35,0x0000,0x0300},//reg_lpll1_en_fifo
        {0x2E,0x0000,0x0020},//reg_lpll1_pd_phdac[1:0]
        {0x37,0x0000,0x0002},//reg_lpll1_vco_sel
        {0x60,0x0000,0x0008},//reg_lpll1_test[17]
        {0x2E,0x0010,0x0010},//reg_lpll_en_hdmi
        {0x37,0x0000,0x2000},//reg_lpll1_en_scalar
    },

    { //E_PNL_SUPPORTED_LPLL_LVDS_FT_SKEW_300to300MHz    NO.55
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0002,0x0003},//reg_lpll1_pd
        {0x03,0x0010,0x001C},//reg_lpll1_icp_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x0000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[5:4]
        {0x35,0x4000,0x7000},//reg_lpll1_output_div_second[3:0]
        {0x2E,0x0004,0x000F},//reg_lpll1_skew_div[2:0]
        {0x37,0x0000,0x0004},//reg_lpll1_fifo_div
        {0x37,0x0000,0x4000},//reg_lpll1_test[18]
        {0x2E,0x2000,0x2000},//reg_lpll1_test[30]
        {0x2E,0x0040,0x0040},//reg_lpll1_en_skew_clk
        {0x35,0x0000,0x0300},//reg_lpll1_en_fifo
        {0x2E,0x0000,0x0020},//reg_lpll1_pd_phdac[1:0]
        {0x37,0x0000,0x0002},//reg_lpll1_vco_sel
        {0x60,0x0000,0x0008},//reg_lpll1_test[17]
        {0x2E,0x0000,0x0010},//reg_lpll_en_hdmi
        {0x37,0x2000,0x2000},//reg_lpll1_en_scalar
    },

    { //E_PNL_SUPPORTED_LPLL_eDP 270MHz_270to270MHz    NO.56
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0002,0x0003},//reg_lpll1_pd
        {0x03,0x0010,0x001C},//reg_lpll1_icp_ictrl
        {0x01,0x0003,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0200,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x0000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[5:4]
        {0x35,0x0000,0x7000},//reg_lpll1_output_div_second[3:0]
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div[2:0]
        {0x37,0x0000,0x0004},//reg_lpll1_fifo_div
        {0x37,0x0000,0x4000},//reg_lpll1_test[18]
        {0x2E,0x0000,0x2000},//reg_lpll1_test[30]
        {0x2E,0x0000,0x0040},//reg_lpll1_en_skew_clk
        {0x35,0x0300,0x0300},//reg_lpll1_en_fifo
        {0x2E,0x0020,0x0020},//reg_lpll1_pd_phdac[1:0]
        {0x37,0x0002,0x0002},//reg_lpll1_vco_sel
        {0x60,0x0000,0x0008},//reg_lpll1_test[17]
        {0x2E,0x0010,0x0010},//reg_lpll_en_hdmi
        {0x37,0x0000,0x2000},//reg_lpll1_en_scalar
    },

#endif
};
#elif( CHIP_ID == MST9U4)  //TTL,LVDS,Vby1
typedef enum
{
#if PANEL_TTL
    E_PNL_SUPPORTED_LPLL_TTL_4P_75to150MHz,          //0
    E_PNL_SUPPORTED_LPLL_TTL_4P_50to75MHz,          //1
    E_PNL_SUPPORTED_LPLL_TTL_4P_25to50MHz,          //2
    E_PNL_SUPPORTED_LPLL_TTL_4P_25to25MHz,          //3
#elif PANEL_LVDS_1CH
    E_PNL_SUPPORTED_LPLL_HS_LVDS_1ch_4P_100to200MHz,          //10
    E_PNL_SUPPORTED_LPLL_HS_LVDS_1ch_4P_100to100MHz,          //11
#elif PANEL_LVDS_2CH
    E_PNL_SUPPORTED_LPLL_HS_LVDS_2ch_4P_100to200MHz,          //12
    E_PNL_SUPPORTED_LPLL_HS_LVDS_2ch_4P_100to100MHz,          //13
    E_PNL_SUPPORTED_LPLL_LVDS_2ch_50to75MHz,          //4
    E_PNL_SUPPORTED_LPLL_LVDS_2ch_37_5to50MHz,          //5
    E_PNL_SUPPORTED_LPLL_LVDS_2ch_37_5to37_5MHz,          //6
#elif PANEL_LVDS_4CH
    E_PNL_SUPPORTED_LPLL_HS_LVDS_4ch_4P_100to200MHz,          //14
    E_PNL_SUPPORTED_LPLL_HS_LVDS_4ch_4P_100to100MHz,          //15
    E_PNL_SUPPORTED_LPLL_LVDS_4ch_50to75MHz,          //7
    E_PNL_SUPPORTED_LPLL_LVDS_4ch_37_5to50MHz,          //8
    E_PNL_SUPPORTED_LPLL_LVDS_4ch_37_5to37_5MHz,          //9
#elif PANEL_VBY1_1CH_10Bit
#elif PANEL_VBY1_1CH_8Bit
#elif PANEL_VBY1_2CH_10Bit
    E_PNL_SUPPORTED_LPLL_VBY1_2ch_10bit_4P_37_5to75MHz,          //16
    E_PNL_SUPPORTED_LPLL_VBY1_2ch_10bit_4P_37_5to37_5MHz,          //17
#elif PANEL_VBY1_2CH_8Bit
    E_PNL_SUPPORTED_LPLL_VBY1_2ch_8bit_4P_50to100MHz,          //18
    E_PNL_SUPPORTED_LPLL_VBY1_2ch_8bit_4P_50to50MHz,          //19
#elif PANEL_VBY1_4CH_10Bit
    E_PNL_SUPPORTED_LPLL_VBY1_4ch_10bit_4P_37_5to75MHz,          //20
    E_PNL_SUPPORTED_LPLL_VBY1_4ch_10bit_4P_37_5to37_5MHz,          //21
#elif PANEL_VBY1_4CH_8Bit
    E_PNL_SUPPORTED_LPLL_VBY1_4ch_8bit_4P_50to100MHz,          //22
    E_PNL_SUPPORTED_LPLL_VBY1_4ch_8bit_4P_50to50MHz,          //23
#elif PANEL_VBY1_8CH_10Bit
    E_PNL_SUPPORTED_LPLL_VBY1_8ch_10bit_4P_75to150MHz,          //24
    E_PNL_SUPPORTED_LPLL_VBY1_8ch_10bit_4P_75to75MHz,          //25
#elif PANEL_VBY1_8CH_8Bit
    E_PNL_SUPPORTED_LPLL_VBY1_8ch_8bit_4P_100to200MHz,          //26
    E_PNL_SUPPORTED_LPLL_VBY1_8ch_8bit_4P_100to100MHz,          //27
#elif PANEL_VBY1_16CH_10Bit
    E_PNL_SUPPORTED_LPLL_VBY1_16ch_10bit_HighSpeed_4P_150to300MHz,          //27
    E_PNL_SUPPORTED_LPLL_VBY1_16ch_10bit_HighSpeed_4P_150to150MHz,          //29
    E_PNL_SUPPORTED_LPLL_VBY1_16ch_10bit_8P_135to195_75MHz,          //30
    E_PNL_SUPPORTED_LPLL_VBY1_16ch_10bit_8P_97_88to135MHz,          //31
    E_PNL_SUPPORTED_LPLL_VBY1_16ch_10bit_8P_97_88to97_88MHz,          //32
#elif PANEL_VBY1_16CH_8Bit
    E_PNL_SUPPORTED_LPLL_VBY1_16ch_8bit_HighSpeed_4P_225to300MHz,          //33
    E_PNL_SUPPORTED_LPLL_VBY1_16ch_8bit_HighSpeed_4P_150to225MHz,          //34
    E_PNL_SUPPORTED_LPLL_VBY1_16ch_8bit_HighSpeed_4P_150to150MHz,          //35
    E_PNL_SUPPORTED_LPLL_VBY1_16ch_8bit_8P_120to195_75MHz,          //36
    E_PNL_SUPPORTED_LPLL_VBY1_16ch_8bit_8P_97_88to120MHz,          //37
    E_PNL_SUPPORTED_LPLL_VBY1_16ch_8bit_8P_97_88to97_88MHz,          //38
#elif PANEL_VBY1_20CH_10Bit
    E_PNL_SUPPORTED_LPLL_VBY1_20ch_10bit_8P_195_75to391_5MHz,          //39
    E_PNL_SUPPORTED_LPLL_VBY1_20ch_10bit_8P_195_75to195_75MHz,          //40
#elif PANEL_VBY1_20CH_8Bit
    E_PNL_SUPPORTED_LPLL_VBY1_20ch_8bit_8P_250to391_5MHz,          //41
    E_PNL_SUPPORTED_LPLL_VBY1_20ch_8bit_8P_195_75to250MHz,          //42
    E_PNL_SUPPORTED_LPLL_VBY1_20ch_8bit_8P_195_75to195_75MHz,          //43
#elif PANEL_VBY1_32CH_10Bit
    E_PNL_SUPPORTED_LPLL_VBY1_32ch_10bit_8P_225to300MHz,          //44
    E_PNL_SUPPORTED_LPLL_VBY1_32ch_10bit_8P_150to225MHz,          //45
    E_PNL_SUPPORTED_LPLL_VBY1_32ch_10bit_8P_150to150MHz,          //46
#elif PANEL_VBY1_32CH_8Bit
    E_PNL_SUPPORTED_LPLL_VBY1_32ch_8bit_8P_225to300MHz,          //47
    E_PNL_SUPPORTED_LPLL_VBY1_32ch_8bit_8P_150to225MHz,          //48
    E_PNL_SUPPORTED_LPLL_VBY1_32ch_8bit_8P_150to150MHz,          //49
#elif PANEL_EDP
#endif
    E_PNL_SUPPORTED_LPLL_MAX,          //57
} E_PNL_SUPPORTED_LPLL_TYPE;

BYTE u8LoopGain[E_PNL_SUPPORTED_LPLL_MAX]=
{
#if PANEL_TTL
    12,          // //E_PNL_SUPPORTED_LPLL_TTL_4P_75to150MHz    NO.0
    12,          // //E_PNL_SUPPORTED_LPLL_TTL_4P_50to75MHz    NO.1
    12,          // //E_PNL_SUPPORTED_LPLL_TTL_4P_25to50MHz    NO.2
    12,          // //E_PNL_SUPPORTED_LPLL_TTL_4P_25to25MHz    NO.3
#elif PANEL_LVDS_1CH
    12,          // //E_PNL_SUPPORTED_LPLL_HS_LVDS_1ch_4P_100to200MHz    NO.4
    12,          // //E_PNL_SUPPORTED_LPLL_HS_LVDS_1ch_4P_100to100MHz    NO.5
#elif PANEL_LVDS_2CH
    12,          // //E_PNL_SUPPORTED_LPLL_HS_LVDS_2ch_4P_100to200MHz    NO.6
    12,          // //E_PNL_SUPPORTED_LPLL_HS_LVDS_2ch_4P_100to100MHz    NO.7
    12,          // //E_PNL_SUPPORTED_LPLL_LVDS-2ch_50to75MHz    NO.4
    12,          // //E_PNL_SUPPORTED_LPLL_LVDS-2ch_37.5to50MHz    NO.5
    12,          // //E_PNL_SUPPORTED_LPLL_LVDS-2ch_37.5to37.5MHz    NO.6
#elif PANEL_LVDS_4CH
    12,          // //E_PNL_SUPPORTED_LPLL_HS_LVDS_4ch_4P_100to200MHz    NO.8
    12,          // //E_PNL_SUPPORTED_LPLL_HS_LVDS_4ch_4P_100to100MHz    NO.9
    12,          // //E_PNL_SUPPORTED_LPLL_LVDS-4ch_50to75MHz    NO.7
    12,          // //E_PNL_SUPPORTED_LPLL_LVDS-4ch_37.5to50MHz    NO.8
    12,          // //E_PNL_SUPPORTED_LPLL_LVDS-4ch_37.5to37.5MHz    NO.9
#elif PANEL_VBY1_1CH_10Bit
#elif PANEL_VBY1_2CH_10Bit
    24,          // //E_PNL_SUPPORTED_LPLL_VBY1_2ch_10bit_4P_37.5to75MHz    NO.10
    24,          // //E_PNL_SUPPORTED_LPLL_VBY1_2ch_10bit_4P_37.5to37.5MHz    NO.11
#elif PANEL_VBY1_2CH_8Bit
    24,          // //E_PNL_SUPPORTED_LPLL_VBY1_2ch_8bit_4P_50to100MHz    NO.12
    24,          // //E_PNL_SUPPORTED_LPLL_VBY1_2ch_8bit_4P_50to50MHz    NO.13
#elif PANEL_VBY1_4CH_10Bit
    24,          // //E_PNL_SUPPORTED_LPLL_VBY1_4ch_10bit_4P_37.5to75MHz    NO.14
    24,          // //E_PNL_SUPPORTED_LPLL_VBY1_4ch_10bit_4P_37.5to37.5MHz    NO.15
#elif PANEL_VBY1_4CH_8Bit
    24,          // //E_PNL_SUPPORTED_LPLL_VBY1_4ch_8bit_4P_50to100MHz    NO.16
    24,          // //E_PNL_SUPPORTED_LPLL_VBY1_4ch_8bit_4P_50to50MHz    NO.17
#elif PANEL_VBY1_8CH_10Bit
    24,          // //E_PNL_SUPPORTED_LPLL_VBY1_8ch_10bit_4P_75to150MHz    NO.18
    24,          // //E_PNL_SUPPORTED_LPLL_VBY1_8ch_10bit_4P_75to75MHz    NO.19
#elif PANEL_VBY1_8CH_8Bit
    24,          // //E_PNL_SUPPORTED_LPLL_VBY1_8ch_8bit_4P_100to200MHz    NO.20
    24,          // //E_PNL_SUPPORTED_LPLL_VBY1_8ch_8bit_4P_100to100MHz    NO.21
#elif PANEL_VBY1_16CH_10Bit
    24,          // //E_PNL_SUPPORTED_LPLL_VBY1_16ch_10bit _HighSpeed_4P_150to300MHz    NO.22
    24,          // //E_PNL_SUPPORTED_LPLL_VBY1_16ch_10bit _HighSpeed_4P_150to150MHz    NO.23
    24,          // //E_PNL_SUPPORTED_LPLL_VBY1_16ch_10bit_8P_135to195.75MHz    NO.27
    24,          // //E_PNL_SUPPORTED_LPLL_VBY1_16ch_10bit_8P_97.88to135MHz    NO.28
    24,          // //E_PNL_SUPPORTED_LPLL_VBY1_16ch_10bit_8P_97.88to97.88MHz    NO.29
#elif PANEL_VBY1_16CH_8Bit
    48,          // //E_PNL_SUPPORTED_LPLL_VBY1_16ch_8bit_HighSpeed_4P_225to300MHz    NO.24
    24,          // //E_PNL_SUPPORTED_LPLL_VBY1_16ch_8bit_HighSpeed_4P_150to225MHz    NO.25
    24,          // //E_PNL_SUPPORTED_LPLL_VBY1_16ch_8bit_HighSpeed_4P_150to150MHz    NO.26
    32,          // //E_PNL_SUPPORTED_LPLL_VBY1_16ch_8bit_8P_120to195.75MHz    NO.30
    32,          // //E_PNL_SUPPORTED_LPLL_VBY1_16ch_8bit_8P_97.88to120MHz    NO.31
    32,          // //E_PNL_SUPPORTED_LPLL_VBY1_16ch_8bit_8P_97.88to97.88MHz    NO.32
#elif PANEL_VBY1_20CH_10Bit
    24,          // //E_PNL_SUPPORTED_LPLL_VBY1_20ch_10bit_8P _195.75to391.5MHz    NO.33
    24,          // //E_PNL_SUPPORTED_LPLL_VBY1_20ch_10bit_8P _195.75to195.75MHz    NO.34
#elif PANEL_VBY1_20CH_8Bit
    24,          // //E_PNL_SUPPORTED_LPLL_VBY1_20ch_8bit_8P_250to391.5MHz    NO.35
    24,          // //E_PNL_SUPPORTED_LPLL_VBY1_20ch_8bit_8P_195.75to250MHz    NO.36
    24,          // //E_PNL_SUPPORTED_LPLL_VBY1_20ch_8bit_8P_195.75to195.75MHz    NO.37
#elif PANEL_VBY1_32CH_10Bit
    24,          // //E_PNL_SUPPORTED_LPLL_VBY1_32ch_10bit_8P_225to300MHz    NO.38
    24,          // //E_PNL_SUPPORTED_LPLL_VBY1_32ch_10bit_8P_150to225MHz    NO.39
    24,          // //E_PNL_SUPPORTED_LPLL_VBY1_32ch_10bit_8P_150to150MHz    NO.40
#elif PANEL_VBY1_32CH_8Bit
    48,          // //E_PNL_SUPPORTED_LPLL_VBY1_32ch_8bit_8P_225to300MHz    NO.41
    24,          // //E_PNL_SUPPORTED_LPLL_VBY1_32ch_8bit_8P_150to225MHz    NO.42
    24,          // //E_PNL_SUPPORTED_LPLL_VBY1_32ch_8bit_8P_150to150MHz    NO.43
#elif PANEL_EDP

#endif
};
BYTE u8LoopDiv[E_PNL_SUPPORTED_LPLL_MAX]=
{
#if PANEL_TTL
    10,          // //E_PNL_SUPPORTED_LPLL_TTL_4P_75to150MHz    NO.0
    20,          // //E_PNL_SUPPORTED_LPLL_TTL_4P_50to75MHz    NO.1
    28,          // //E_PNL_SUPPORTED_LPLL_TTL_4P_25to50MHz    NO.2
    28,          // //E_PNL_SUPPORTED_LPLL_TTL_4P_25to25MHz    NO.3#elif PANEL_LVDS_1CH
#elif PANEL_LVDS_1CH
    7,          // //E_PNL_SUPPORTED_LPLL_HS_LVDS_1ch_4P_100to200MHz    NO.4
    7,          // //E_PNL_SUPPORTED_LPLL_HS_LVDS_1ch_4P_100to100MHz    NO.5
#elif PANEL_LVDS_2CH
    7,          // //E_PNL_SUPPORTED_LPLL_HS_LVDS_2ch_4P_100to200MHz    NO.6
    7,          // //E_PNL_SUPPORTED_LPLL_HS_LVDS_2ch_4P_100to100MHz    NO.7
    14,          // //E_PNL_SUPPORTED_LPLL_LVDS-2ch_50to75MHz    NO.4
    28,          // //E_PNL_SUPPORTED_LPLL_LVDS-2ch_37.5to50MHz    NO.5
    28,          // //E_PNL_SUPPORTED_LPLL_LVDS-2ch_37.5to37.5MHz    NO.6
#elif PANEL_LVDS_4CH
    7,          // //E_PNL_SUPPORTED_LPLL_HS_LVDS_4ch_4P_100to200MHz    NO.8
    7,          // //E_PNL_SUPPORTED_LPLL_HS_LVDS_4ch_4P_100to100MHz    NO.9
    14,          // //E_PNL_SUPPORTED_LPLL_LVDS-4ch_50to75MHz    NO.7
    28,          // //E_PNL_SUPPORTED_LPLL_LVDS-4ch_37.5to50MHz    NO.8
    28,          // //E_PNL_SUPPORTED_LPLL_LVDS-4ch_37.5to37.5MHz    NO.9
#elif PANEL_VBY1_1CH_10Bit
#elif PANEL_VBY1_2CH_10Bit
    40,          // //E_PNL_SUPPORTED_LPLL_VBY1_2ch_10bit_4P_37.5to75MHz    NO.10
    40,          // //E_PNL_SUPPORTED_LPLL_VBY1_2ch_10bit_4P_37.5to37.5MHz    NO.11
#elif PANEL_VBY1_2CH_8Bit
    30,          // //E_PNL_SUPPORTED_LPLL_VBY1_2ch_8bit_4P_50to100MHz    NO.12
    30,          // //E_PNL_SUPPORTED_LPLL_VBY1_2ch_8bit_4P_50to50MHz    NO.13
#elif PANEL_VBY1_4CH_10Bit
    40,          // //E_PNL_SUPPORTED_LPLL_VBY1_4ch_10bit_4P_37.5to75MHz    NO.14
    40,          // //E_PNL_SUPPORTED_LPLL_VBY1_4ch_10bit_4P_37.5to37.5MHz    NO.15
#elif PANEL_VBY1_4CH_8Bit
    30,          // //E_PNL_SUPPORTED_LPLL_VBY1_4ch_8bit_4P_50to100MHz    NO.16
    30,          // //E_PNL_SUPPORTED_LPLL_VBY1_4ch_8bit_4P_50to50MHz    NO.17
#elif PANEL_VBY1_8CH_10Bit
    20,          // //E_PNL_SUPPORTED_LPLL_VBY1_8ch_10bit_4P_75to150MHz    NO.18
    20,          // //E_PNL_SUPPORTED_LPLL_VBY1_8ch_10bit_4P_75to75MHz    NO.19
#elif PANEL_VBY1_8CH_8Bit
    15,          // //E_PNL_SUPPORTED_LPLL_VBY1_8ch_8bit_4P_100to200MHz    NO.20
    15,          // //E_PNL_SUPPORTED_LPLL_VBY1_8ch_8bit_4P_100to100MHz    NO.21
#elif PANEL_VBY1_16CH_10Bit
    10,          // //E_PNL_SUPPORTED_LPLL_VBY1_16ch_10bit _HighSpeed_4P_150to300MHz    NO.22
    10,          // //E_PNL_SUPPORTED_LPLL_VBY1_16ch_10bit _HighSpeed_4P_150to150MHz    NO.23
    20,          // //E_PNL_SUPPORTED_LPLL_VBY1_16ch_10bit_8P_135to195.75MHz    NO.27
    20,          // //E_PNL_SUPPORTED_LPLL_VBY1_16ch_10bit_8P_97.88to135MHz    NO.28
    20,          // //E_PNL_SUPPORTED_LPLL_VBY1_16ch_10bit_8P_97.88to97.88MHz    NO.29
#elif PANEL_VBY1_16CH_8Bit
    15,          // //E_PNL_SUPPORTED_LPLL_VBY1_16ch_8bit_HighSpeed_4P_225to300MHz    NO.24
    15,          // //E_PNL_SUPPORTED_LPLL_VBY1_16ch_8bit_HighSpeed_4P_150to225MHz    NO.25
    15,          // //E_PNL_SUPPORTED_LPLL_VBY1_16ch_8bit_HighSpeed_4P_150to150MHz
    15,          // //E_PNL_SUPPORTED_LPLL_VBY1_16ch_8bit_8P_120to195.75MHz    NO.30
    30,          // //E_PNL_SUPPORTED_LPLL_VBY1_16ch_8bit_8P_97.88to120MHz    NO.31
    30,          // //E_PNL_SUPPORTED_LPLL_VBY1_16ch_8bit_8P_97.88to97.88MHz    NO.32
#elif PANEL_VBY1_20CH_10Bit
    8,          // //E_PNL_SUPPORTED_LPLL_VBY1_20ch_10bit_8P _195.75to391.5MHz    NO.33
    8,          // //E_PNL_SUPPORTED_LPLL_VBY1_20ch_10bit_8P _195.75to195.75MHz    NO.34
#elif PANEL_VBY1_20CH_8Bit
    6,          // //E_PNL_SUPPORTED_LPLL_VBY1_20ch_8bit_8P_250to391.5MHz    NO.35
    12,          // //E_PNL_SUPPORTED_LPLL_VBY1_20ch_8bit_8P_195.75to250MHz    NO.36
    12,          // //E_PNL_SUPPORTED_LPLL_VBY1_20ch_8bit_8P_195.75to195.75MHz    NO.37
#elif PANEL_VBY1_32CH_10Bit
    10,          // //E_PNL_SUPPORTED_LPLL_VBY1_32ch_10bit_8P_225to300MHz    NO.38
    10,          // //E_PNL_SUPPORTED_LPLL_VBY1_32ch_10bit_8P_150to225MHz    NO.39
    10,          // //E_PNL_SUPPORTED_LPLL_VBY1_32ch_10bit_8P_150to150MHz    NO.40
#elif PANEL_VBY1_32CH_8Bit
    15,          // //E_PNL_SUPPORTED_LPLL_VBY1_32ch_8bit_8P_225to300MHz    NO.41
    15,          // //E_PNL_SUPPORTED_LPLL_VBY1_32ch_8bit_8P_150to225MHz    NO.42
    15,          // //E_PNL_SUPPORTED_LPLL_VBY1_32ch_8bit_8P_150to150MHz    NO.43
#elif PANEL_EDP

#endif
};
//MST9U4
code TBLStruct LPLLSettingTBL[E_PNL_SUPPORTED_LPLL_MAX][LPLL_REG_NUM]=
{
    #if PANEL_TTL
    { //E_PNL_SUPPORTED_LPLL_TTL_4P_75to150MHz    NO.0
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0004,0x001C},//reg_lpll1_pd
        {0x15,0x0000,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x1000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0500,0x0F00},//reg_lpll1_scalar_div_first
        {0x35,0x7000,0x7000},//reg_lpll1_scalar_div_second
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div
        {0x2E,0x1000,0x1000},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},// reg_lpll1_fifo_div5_en
        {0x03,0x0000,0x0400},//reg_lpll1_duap_lp_en
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_sdiv3p5_en
        {0x2E,0x0000,0x0040},//reg_lpll1_en_mini
        {0x2E,0x0010,0x0010},//reg_lpll1_en_fifo
        {0x35,0x0000,0x0020},//reg_lpll1_en_scalar
        {0x33,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x33,0x0000,0x0004},//reg_lpll2_pd
        {0x30,0x0000,0x001F},//reg_lpll2_ibias_ictrl
        {0x31,0x0000,0x0003},//reg_lpll2_input_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_first
        {0x32,0x0000,0x000F},//reg_lpll2_loop_div_second
        {0x36,0x0000,0x0800},//reg_lpll2_output_div_first
        {0x36,0x0000,0x1000},//reg_lpll1_test[11]
        {0x36,0x0000,0x4000},//reg_lpll1_test[12]
    },
    { //E_PNL_SUPPORTED_LPLL_TTL_4P_50to75MHz    NO.1
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0004,0x001C},//reg_lpll1_pd
        {0x15,0x0000,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x2000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0500,0x0F00},//reg_lpll1_scalar_div_first
        {0x35,0x7000,0x7000},//reg_lpll1_scalar_div_second
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div
        {0x2E,0x1000,0x1000},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},// reg_lpll1_fifo_div5_en
        {0x03,0x0000,0x0400},//reg_lpll1_duap_lp_en
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_sdiv3p5_en
        {0x2E,0x0000,0x0040},//reg_lpll1_en_mini
        {0x2E,0x0010,0x0010},//reg_lpll1_en_fifo
        {0x35,0x0000,0x0020},//reg_lpll1_en_scalar
        {0x33,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x33,0x0000,0x0004},//reg_lpll2_pd
        {0x30,0x0000,0x001F},//reg_lpll2_ibias_ictrl
        {0x31,0x0000,0x0003},//reg_lpll2_input_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_first
        {0x32,0x0000,0x000F},//reg_lpll2_loop_div_second
        {0x36,0x0000,0x0800},//reg_lpll2_output_div_first
        {0x36,0x0000,0x1000},//reg_lpll1_test[11]
        {0x36,0x0000,0x4000},//reg_lpll1_test[12]
    },
    { //E_PNL_SUPPORTED_LPLL_TTL_4P_25to50MHz    NO.2
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0004,0x001C},//reg_lpll1_pd
        {0x15,0x0000,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x2000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0700,0x0F00},//reg_lpll1_scalar_div_first
        {0x35,0x7000,0x7000},//reg_lpll1_scalar_div_second
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div
        {0x2E,0x1000,0x1000},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},// reg_lpll1_fifo_div5_en
        {0x03,0x0000,0x0400},//reg_lpll1_duap_lp_en
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_sdiv3p5_en
        {0x2E,0x0000,0x0040},//reg_lpll1_en_mini
        {0x2E,0x0010,0x0010},//reg_lpll1_en_fifo
        {0x35,0x0000,0x0020},//reg_lpll1_en_scalar
        {0x33,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x33,0x0000,0x0004},//reg_lpll2_pd
        {0x30,0x0000,0x001F},//reg_lpll2_ibias_ictrl
        {0x31,0x0000,0x0003},//reg_lpll2_input_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_first
        {0x32,0x0000,0x000F},//reg_lpll2_loop_div_second
        {0x36,0x0000,0x0800},//reg_lpll2_output_div_first
        {0x36,0x0000,0x1000},//reg_lpll1_test[11]
        {0x36,0x0000,0x4000},//reg_lpll1_test[12]
    },
    { //E_PNL_SUPPORTED_LPLL_TTL_4P_25to25MHz    NO.3
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0004,0x001C},//reg_lpll1_pd
        {0x15,0x0000,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x2000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0700,0x0F00},//reg_lpll1_scalar_div_first
        {0x35,0x7000,0x7000},//reg_lpll1_scalar_div_second
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div
        {0x2E,0x1000,0x1000},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},// reg_lpll1_fifo_div5_en
        {0x03,0x0000,0x0400},//reg_lpll1_duap_lp_en
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_sdiv3p5_en
        {0x2E,0x0000,0x0040},//reg_lpll1_en_mini
        {0x2E,0x0010,0x0010},//reg_lpll1_en_fifo
        {0x35,0x0000,0x0020},//reg_lpll1_en_scalar
        {0x33,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x33,0x0000,0x0004},//reg_lpll2_pd
        {0x30,0x0000,0x001F},//reg_lpll2_ibias_ictrl
        {0x31,0x0000,0x0003},//reg_lpll2_input_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_first
        {0x32,0x0000,0x000F},//reg_lpll2_loop_div_second
        {0x36,0x0000,0x0800},//reg_lpll2_output_div_first
        {0x36,0x0000,0x1000},//reg_lpll1_test[11]
        {0x36,0x0000,0x4000},//reg_lpll1_test[12]
    },
    { //E_PNL_SUPPORTED_LPLL_HS_LVDS_1ch_4P_100to200MHz    NO.4
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0004,0x001C},//reg_lpll1_pd
        {0x15,0x0000,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x0000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0700,0x0F00},//reg_lpll1_scalar_div_first
        {0x35,0x0000,0x7000},//reg_lpll1_scalar_div_second
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div
        {0x2E,0x1000,0x1000},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},// reg_lpll1_fifo_div5_en
        {0x03,0x0000,0x0400},//reg_lpll1_duap_lp_en
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_sdiv3p5_en
        {0x2E,0x0000,0x0040},//reg_lpll1_en_mini
        {0x2E,0x0010,0x0010},//reg_lpll1_en_fifo
        {0x35,0x0000,0x0020},//reg_lpll1_en_scalar
        {0x33,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x33,0x0000,0x0004},//reg_lpll2_pd
        {0x30,0x0000,0x001F},//reg_lpll2_ibias_ictrl
        {0x31,0x0000,0x0003},//reg_lpll2_input_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_first
        {0x32,0x0000,0x000F},//reg_lpll2_loop_div_second
        {0x36,0x0000,0x0800},//reg_lpll2_output_div_first
        {0x36,0x0000,0x1000},//reg_lpll1_test[11]
        {0x36,0x0000,0x4000},//reg_lpll1_test[12]
    },
    #elif PANEL_LVDS_1CH
    { //E_PNL_SUPPORTED_LPLL_HS_LVDS_1ch_4P_100to200MHz    NO.10
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0004,0x001C},//reg_lpll1_pd
        {0x15,0x0000,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x0000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0700,0x0F00},//reg_lpll1_scalar_div_first
        {0x35,0x0000,0x7000},//reg_lpll1_scalar_div_second
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div
        {0x2E,0x1000,0x1000},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},// reg_lpll1_fifo_div5_en
        {0x03,0x0000,0x0400},//reg_lpll1_duap_lp_en
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_sdiv3p5_en
        {0x2E,0x0000,0x0040},//reg_lpll1_en_mini
        {0x2E,0x0010,0x0010},//reg_lpll1_en_fifo
        {0x35,0x0000,0x0020},//reg_lpll1_en_scalar
        {0x33,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x33,0x0000,0x0004},//reg_lpll2_pd
        {0x30,0x0000,0x001F},//reg_lpll2_ibias_ictrl
        {0x31,0x0000,0x0003},//reg_lpll2_input_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_first
        {0x32,0x0000,0x000F},//reg_lpll2_loop_div_second
        {0x36,0x0000,0x0800},//reg_lpll2_output_div_first
        {0x36,0x0000,0x1000},//reg_lpll1_test[11]
        {0x36,0x0000,0x4000},//reg_lpll1_test[12]
    },
    { //E_PNL_SUPPORTED_LPLL_HS_LVDS_1ch_4P_100to100MHz    NO.5
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0004,0x001C},//reg_lpll1_pd
        {0x15,0x0000,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x0000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0700,0x0F00},//reg_lpll1_scalar_div_first
        {0x35,0x0000,0x7000},//reg_lpll1_scalar_div_second
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div
        {0x2E,0x1000,0x1000},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},// reg_lpll1_fifo_div5_en
        {0x03,0x0000,0x0400},//reg_lpll1_duap_lp_en
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_sdiv3p5_en
        {0x2E,0x0000,0x0040},//reg_lpll1_en_mini
        {0x2E,0x0010,0x0010},//reg_lpll1_en_fifo
        {0x35,0x0000,0x0020},//reg_lpll1_en_scalar
        {0x33,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x33,0x0000,0x0004},//reg_lpll2_pd
        {0x30,0x0000,0x001F},//reg_lpll2_ibias_ictrl
        {0x31,0x0000,0x0003},//reg_lpll2_input_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_first
        {0x32,0x0000,0x000F},//reg_lpll2_loop_div_second
        {0x36,0x0000,0x0800},//reg_lpll2_output_div_first
        {0x36,0x0000,0x1000},//reg_lpll1_test[11]
        {0x36,0x0000,0x4000},//reg_lpll1_test[12]
    },
    #elif PANEL_LVDS_2CH
    { //E_PNL_SUPPORTED_LPLL_HS_LVDS_2ch_4P_100to200MHz    NO.6
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0004,0x001C},//reg_lpll1_pd
        {0x15,0x0000,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x0000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0700,0x0F00},//reg_lpll1_scalar_div_first
        {0x35,0x0000,0x7000},//reg_lpll1_scalar_div_second
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div
        {0x2E,0x1000,0x1000},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},// reg_lpll1_fifo_div5_en
        {0x03,0x0000,0x0400},//reg_lpll1_duap_lp_en
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_sdiv3p5_en
        {0x2E,0x0000,0x0040},//reg_lpll1_en_mini
        {0x2E,0x0010,0x0010},//reg_lpll1_en_fifo
        {0x35,0x0000,0x0020},//reg_lpll1_en_scalar
        {0x33,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x33,0x0000,0x0004},//reg_lpll2_pd
        {0x30,0x0000,0x001F},//reg_lpll2_ibias_ictrl
        {0x31,0x0000,0x0003},//reg_lpll2_input_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_first
        {0x32,0x0000,0x000F},//reg_lpll2_loop_div_second
        {0x36,0x0000,0x0800},//reg_lpll2_output_div_first
        {0x36,0x0000,0x1000},//reg_lpll1_test[11]
        {0x36,0x0000,0x4000},//reg_lpll1_test[12]
    },
    { //E_PNL_SUPPORTED_LPLL_HS_LVDS_2ch_4P_100to100MHz    NO.7
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0004,0x001C},//reg_lpll1_pd
        {0x15,0x0000,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x0000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0700,0x0F00},//reg_lpll1_scalar_div_first
        {0x35,0x0000,0x7000},//reg_lpll1_scalar_div_second
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div
        {0x2E,0x1000,0x1000},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},// reg_lpll1_fifo_div5_en
        {0x03,0x0000,0x0400},//reg_lpll1_duap_lp_en
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_sdiv3p5_en
        {0x2E,0x0000,0x0040},//reg_lpll1_en_mini
        {0x2E,0x0010,0x0010},//reg_lpll1_en_fifo
        {0x35,0x0000,0x0020},//reg_lpll1_en_scalar
        {0x33,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x33,0x0000,0x0004},//reg_lpll2_pd
        {0x30,0x0000,0x001F},//reg_lpll2_ibias_ictrl
        {0x31,0x0000,0x0003},//reg_lpll2_input_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_first
        {0x32,0x0000,0x000F},//reg_lpll2_loop_div_second
        {0x36,0x0000,0x0800},//reg_lpll2_output_div_first
        {0x36,0x0000,0x1000},//reg_lpll1_test[11]
        {0x36,0x0000,0x4000},//reg_lpll1_test[12]
    },
    { //E_PNL_SUPPORTED_LPLL_LVDS-2ch_50to75MHz    NO.4
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0004,0x001C},//reg_lpll1_pd
        {0x15,0x0000,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x1000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0700,0x0F00},//reg_lpll1_scalar_div_first
        {0x35,0x1000,0x7000},//reg_lpll1_scalar_div_second
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div
        {0x2E,0x1000,0x1000},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},// reg_lpll1_fifo_div5_en
        {0x03,0x0000,0x0400},//reg_lpll1_duap_lp_en
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_sdiv3p5_en
        {0x2E,0x0000,0x0040},//reg_lpll1_en_mini
        {0x2E,0x0010,0x0010},//reg_lpll1_en_fifo
        {0x35,0x0000,0x0020},//reg_lpll1_en_scalar
        {0x33,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x33,0x0000,0x0004},//reg_lpll2_pd
        {0x30,0x0000,0x001F},//reg_lpll2_ibias_ictrl
        {0x31,0x0000,0x0003},//reg_lpll2_input_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_first
        {0x32,0x0000,0x000F},//reg_lpll2_loop_div_second
        {0x36,0x0000,0x0800},//reg_lpll2_output_div_first
        {0x36,0x0000,0x1000},//reg_lpll1_test[11]
        {0x36,0x0000,0x4000},//reg_lpll1_test[12]
    },
    { //E_PNL_SUPPORTED_LPLL_LVDS-2ch_37.5to50MHz    NO.5
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0004,0x001C},//reg_lpll1_pd
        {0x15,0x0000,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x2000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0700,0x0F00},//reg_lpll1_scalar_div_first
        {0x35,0x2000,0x7000},//reg_lpll1_scalar_div_second
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div
        {0x2E,0x1000,0x1000},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},// reg_lpll1_fifo_div5_en
        {0x03,0x0000,0x0400},//reg_lpll1_duap_lp_en
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_sdiv3p5_en
        {0x2E,0x0000,0x0040},//reg_lpll1_en_mini
        {0x2E,0x0010,0x0010},//reg_lpll1_en_fifo
        {0x35,0x0000,0x0020},//reg_lpll1_en_scalar
        {0x33,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x33,0x0000,0x0004},//reg_lpll2_pd
        {0x30,0x0000,0x001F},//reg_lpll2_ibias_ictrl
        {0x31,0x0000,0x0003},//reg_lpll2_input_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_first
        {0x32,0x0000,0x000F},//reg_lpll2_loop_div_second
        {0x36,0x0000,0x0800},//reg_lpll2_output_div_first
        {0x36,0x0000,0x1000},//reg_lpll1_test[11]
        {0x36,0x0000,0x4000},//reg_lpll1_test[12]
    },
    { //E_PNL_SUPPORTED_LPLL_LVDS-2ch_37.5to37.5MHz    NO.6
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0004,0x001C},//reg_lpll1_pd
        {0x15,0x0000,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x2000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0700,0x0F00},//reg_lpll1_scalar_div_first
        {0x35,0x2000,0x7000},//reg_lpll1_scalar_div_second
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div
        {0x2E,0x1000,0x1000},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},// reg_lpll1_fifo_div5_en
        {0x03,0x0000,0x0400},//reg_lpll1_duap_lp_en
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_sdiv3p5_en
        {0x2E,0x0000,0x0040},//reg_lpll1_en_mini
        {0x2E,0x0010,0x0010},//reg_lpll1_en_fifo
        {0x35,0x0000,0x0020},//reg_lpll1_en_scalar
        {0x33,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x33,0x0000,0x0004},//reg_lpll2_pd
        {0x30,0x0000,0x001F},//reg_lpll2_ibias_ictrl
        {0x31,0x0000,0x0003},//reg_lpll2_input_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_first
        {0x32,0x0000,0x000F},//reg_lpll2_loop_div_second
        {0x36,0x0000,0x0800},//reg_lpll2_output_div_first
        {0x36,0x0000,0x1000},//reg_lpll1_test[11]
        {0x36,0x0000,0x4000},//reg_lpll1_test[12]
    },
    #elif PANEL_LVDS_4CH
    { //E_PNL_SUPPORTED_LPLL_HS_LVDS_4ch_4P_100to200MHz    NO.8
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0004,0x001C},//reg_lpll1_pd
        {0x15,0x0000,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x0000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0700,0x0F00},//reg_lpll1_scalar_div_first
        {0x35,0x0000,0x7000},//reg_lpll1_scalar_div_second
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div
        {0x2E,0x1000,0x1000},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},// reg_lpll1_fifo_div5_en
        {0x03,0x0000,0x0400},//reg_lpll1_duap_lp_en
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_sdiv3p5_en
        {0x2E,0x0000,0x0040},//reg_lpll1_en_mini
        {0x2E,0x0010,0x0010},//reg_lpll1_en_fifo
        {0x35,0x0000,0x0020},//reg_lpll1_en_scalar
        {0x33,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x33,0x0000,0x0004},//reg_lpll2_pd
        {0x30,0x0000,0x001F},//reg_lpll2_ibias_ictrl
        {0x31,0x0000,0x0003},//reg_lpll2_input_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_first
        {0x32,0x0000,0x000F},//reg_lpll2_loop_div_second
        {0x36,0x0000,0x0800},//reg_lpll2_output_div_first
        {0x36,0x0000,0x1000},//reg_lpll1_test[11]
        {0x36,0x0000,0x4000},//reg_lpll1_test[12]
    },
    { //E_PNL_SUPPORTED_LPLL_HS_LVDS_4ch_4P_100to100MHz    NO.9
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0004,0x001C},//reg_lpll1_pd
        {0x15,0x0000,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x0000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0700,0x0F00},//reg_lpll1_scalar_div_first
        {0x35,0x0000,0x7000},//reg_lpll1_scalar_div_second
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div
        {0x2E,0x1000,0x1000},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},// reg_lpll1_fifo_div5_en
        {0x03,0x0000,0x0400},//reg_lpll1_duap_lp_en
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_sdiv3p5_en
        {0x2E,0x0000,0x0040},//reg_lpll1_en_mini
        {0x2E,0x0010,0x0010},//reg_lpll1_en_fifo
        {0x35,0x0000,0x0020},//reg_lpll1_en_scalar
        {0x33,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x33,0x0000,0x0004},//reg_lpll2_pd
        {0x30,0x0000,0x001F},//reg_lpll2_ibias_ictrl
        {0x31,0x0000,0x0003},//reg_lpll2_input_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_first
        {0x32,0x0000,0x000F},//reg_lpll2_loop_div_second
        {0x36,0x0000,0x0800},//reg_lpll2_output_div_first
        {0x36,0x0000,0x1000},//reg_lpll1_test[11]
        {0x36,0x0000,0x4000},//reg_lpll1_test[12]
    },
    { //E_PNL_SUPPORTED_LPLL_LVDS-4ch_50to75MHz    NO.7
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0004,0x001C},//reg_lpll1_pd
        {0x15,0x0000,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x1000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0700,0x0F00},//reg_lpll1_scalar_div_first
        {0x35,0x1000,0x7000},//reg_lpll1_scalar_div_second
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div
        {0x2E,0x1000,0x1000},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},// reg_lpll1_fifo_div5_en
        {0x03,0x0000,0x0400},//reg_lpll1_duap_lp_en
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_sdiv3p5_en
        {0x2E,0x0000,0x0040},//reg_lpll1_en_mini
        {0x2E,0x0010,0x0010},//reg_lpll1_en_fifo
        {0x35,0x0000,0x0020},//reg_lpll1_en_scalar
        {0x33,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x33,0x0000,0x0004},//reg_lpll2_pd
        {0x30,0x0000,0x001F},//reg_lpll2_ibias_ictrl
        {0x31,0x0000,0x0003},//reg_lpll2_input_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_first
        {0x32,0x0000,0x000F},//reg_lpll2_loop_div_second
        {0x36,0x0000,0x0800},//reg_lpll2_output_div_first
        {0x36,0x0000,0x1000},//reg_lpll1_test[11]
        {0x36,0x0000,0x4000},//reg_lpll1_test[12]
    },
    { //E_PNL_SUPPORTED_LPLL_LVDS-4ch_37.5to50MHz    NO.8
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0004,0x001C},//reg_lpll1_pd
        {0x15,0x0000,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x2000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0700,0x0F00},//reg_lpll1_scalar_div_first
        {0x35,0x2000,0x7000},//reg_lpll1_scalar_div_second
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div
        {0x2E,0x1000,0x1000},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},// reg_lpll1_fifo_div5_en
        {0x03,0x0000,0x0400},//reg_lpll1_duap_lp_en
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_sdiv3p5_en
        {0x2E,0x0000,0x0040},//reg_lpll1_en_mini
        {0x2E,0x0010,0x0010},//reg_lpll1_en_fifo
        {0x35,0x0000,0x0020},//reg_lpll1_en_scalar
        {0x33,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x33,0x0000,0x0004},//reg_lpll2_pd
        {0x30,0x0000,0x001F},//reg_lpll2_ibias_ictrl
        {0x31,0x0000,0x0003},//reg_lpll2_input_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_first
        {0x32,0x0000,0x000F},//reg_lpll2_loop_div_second
        {0x36,0x0000,0x0800},//reg_lpll2_output_div_first
        {0x36,0x0000,0x1000},//reg_lpll1_test[11]
        {0x36,0x0000,0x4000},//reg_lpll1_test[12]
    },
    { //E_PNL_SUPPORTED_LPLL_LVDS-4ch_37.5to37.5MHz    NO.9
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0004,0x001C},//reg_lpll1_pd
        {0x15,0x0000,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x2000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0700,0x0F00},//reg_lpll1_scalar_div_first
        {0x35,0x2000,0x7000},//reg_lpll1_scalar_div_second
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div
        {0x2E,0x1000,0x1000},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},// reg_lpll1_fifo_div5_en
        {0x03,0x0000,0x0400},//reg_lpll1_duap_lp_en
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_sdiv3p5_en
        {0x2E,0x0000,0x0040},//reg_lpll1_en_mini
        {0x2E,0x0010,0x0010},//reg_lpll1_en_fifo
        {0x35,0x0000,0x0020},//reg_lpll1_en_scalar
        {0x33,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x33,0x0000,0x0004},//reg_lpll2_pd
        {0x30,0x0000,0x001F},//reg_lpll2_ibias_ictrl
        {0x31,0x0000,0x0003},//reg_lpll2_input_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_first
        {0x32,0x0000,0x000F},//reg_lpll2_loop_div_second
        {0x36,0x0000,0x0800},//reg_lpll2_output_div_first
        {0x36,0x0000,0x1000},//reg_lpll1_test[11]
        {0x36,0x0000,0x4000},//reg_lpll1_test[12]
    },
    #elif PANEL_VBY1_1CH_10Bit
    #elif PANEL_VBY1_2CH_10Bit
    { //E_PNL_SUPPORTED_LPLL_VBY1_2ch_10bit_4P_37.5to75MHz    NO.10
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0004,0x001C},//reg_lpll1_pd
        {0x15,0x0000,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0600,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x3000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0200,0x0F00},//reg_lpll1_scalar_div_first
        {0x35,0x0000,0x7000},//reg_lpll1_scalar_div_second
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div
        {0x2E,0x1000,0x1000},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},// reg_lpll1_fifo_div5_en
        {0x03,0x0400,0x0400},//reg_lpll1_duap_lp_en
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_sdiv3p5_en
        {0x2E,0x0040,0x0040},//reg_lpll1_en_mini
        {0x2E,0x0010,0x0010},//reg_lpll1_en_fifo
        {0x35,0x0000,0x0020},//reg_lpll1_en_scalar
        {0x33,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x33,0x0000,0x0004},//reg_lpll2_pd
        {0x30,0x0000,0x001F},//reg_lpll2_ibias_ictrl
        {0x31,0x0000,0x0003},//reg_lpll2_input_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_first
        {0x32,0x0000,0x000F},//reg_lpll2_loop_div_second
        {0x36,0x0000,0x0800},//reg_lpll2_output_div_first
        {0x36,0x0000,0x1000},//reg_lpll1_test[11]
        {0x36,0x0000,0x4000},//reg_lpll1_test[12]
    },
    { //E_PNL_SUPPORTED_LPLL_VBY1_2ch_10bit_4P_37.5to37.5MHz    NO.11
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0004,0x001C},//reg_lpll1_pd
        {0x15,0x0000,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0600,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x3000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0200,0x0F00},//reg_lpll1_scalar_div_first
        {0x35,0x0000,0x7000},//reg_lpll1_scalar_div_second
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div
        {0x2E,0x1000,0x1000},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},// reg_lpll1_fifo_div5_en
        {0x03,0x0400,0x0400},//reg_lpll1_duap_lp_en
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_sdiv3p5_en
        {0x2E,0x0040,0x0040},//reg_lpll1_en_mini
        {0x2E,0x0010,0x0010},//reg_lpll1_en_fifo
        {0x35,0x0000,0x0020},//reg_lpll1_en_scalar
        {0x33,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x33,0x0000,0x0004},//reg_lpll2_pd
        {0x30,0x0000,0x001F},//reg_lpll2_ibias_ictrl
        {0x31,0x0000,0x0003},//reg_lpll2_input_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_first
        {0x32,0x0000,0x000F},//reg_lpll2_loop_div_second
        {0x36,0x0000,0x0800},//reg_lpll2_output_div_first
        {0x36,0x0000,0x1000},//reg_lpll1_test[11]
        {0x36,0x0000,0x4000},//reg_lpll1_test[12]
    },
    #elif PANEL_VBY1_2CH_8Bit
    { //E_PNL_SUPPORTED_LPLL_VBY1_2ch_8bit_4P_50to100MHz    NO.12
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0004,0x001C},//reg_lpll1_pd
        {0x15,0x0000,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0600,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x2000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0300,0x0F00},//reg_lpll1_scalar_div_first
        {0x35,0x0000,0x7000},//reg_lpll1_scalar_div_second
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div
        {0x2E,0x1000,0x1000},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},// reg_lpll1_fifo_div5_en
        {0x03,0x0400,0x0400},//reg_lpll1_duap_lp_en
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_sdiv3p5_en
        {0x2E,0x0040,0x0040},//reg_lpll1_en_mini
        {0x2E,0x0010,0x0010},//reg_lpll1_en_fifo
        {0x35,0x0000,0x0020},//reg_lpll1_en_scalar
        {0x33,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x33,0x0000,0x0004},//reg_lpll2_pd
        {0x30,0x0000,0x001F},//reg_lpll2_ibias_ictrl
        {0x31,0x0000,0x0003},//reg_lpll2_input_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_first
        {0x32,0x0000,0x000F},//reg_lpll2_loop_div_second
        {0x36,0x0000,0x0800},//reg_lpll2_output_div_first
        {0x36,0x0000,0x1000},//reg_lpll1_test[11]
        {0x36,0x0000,0x4000},//reg_lpll1_test[12]
    },
    { //E_PNL_SUPPORTED_LPLL_VBY1_2ch_8bit_4P_50to50MHz    NO.13
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0004,0x001C},//reg_lpll1_pd
        {0x15,0x0000,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0600,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x2000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0300,0x0F00},//reg_lpll1_scalar_div_first
        {0x35,0x0000,0x7000},//reg_lpll1_scalar_div_second
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div
        {0x2E,0x1000,0x1000},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},// reg_lpll1_fifo_div5_en
        {0x03,0x0400,0x0400},//reg_lpll1_duap_lp_en
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_sdiv3p5_en
        {0x2E,0x0040,0x0040},//reg_lpll1_en_mini
        {0x2E,0x0010,0x0010},//reg_lpll1_en_fifo
        {0x35,0x0000,0x0020},//reg_lpll1_en_scalar
        {0x33,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x33,0x0000,0x0004},//reg_lpll2_pd
        {0x30,0x0000,0x001F},//reg_lpll2_ibias_ictrl
        {0x31,0x0000,0x0003},//reg_lpll2_input_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_first
        {0x32,0x0000,0x000F},//reg_lpll2_loop_div_second
        {0x36,0x0000,0x0800},//reg_lpll2_output_div_first
        {0x36,0x0000,0x1000},//reg_lpll1_test[11]
        {0x36,0x0000,0x4000},//reg_lpll1_test[12]
    },
    #elif PANEL_VBY1_4CH_10Bit
    { //E_PNL_SUPPORTED_LPLL_VBY1_4ch_10bit_4P_37.5to75MHz    NO.14
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0004,0x001C},//reg_lpll1_pd
        {0x15,0x0000,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0600,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x3000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0200,0x0F00},//reg_lpll1_scalar_div_first
        {0x35,0x0000,0x7000},//reg_lpll1_scalar_div_second
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div
        {0x2E,0x1000,0x1000},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},// reg_lpll1_fifo_div5_en
        {0x03,0x0400,0x0400},//reg_lpll1_duap_lp_en
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_sdiv3p5_en
        {0x2E,0x0040,0x0040},//reg_lpll1_en_mini
        {0x2E,0x0010,0x0010},//reg_lpll1_en_fifo
        {0x35,0x0000,0x0020},//reg_lpll1_en_scalar
        {0x33,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x33,0x0000,0x0004},//reg_lpll2_pd
        {0x30,0x0000,0x001F},//reg_lpll2_ibias_ictrl
        {0x31,0x0000,0x0003},//reg_lpll2_input_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_first
        {0x32,0x0000,0x000F},//reg_lpll2_loop_div_second
        {0x36,0x0000,0x0800},//reg_lpll2_output_div_first
        {0x36,0x0000,0x1000},//reg_lpll1_test[11]
        {0x36,0x0000,0x4000},//reg_lpll1_test[12]
    },
    { //E_PNL_SUPPORTED_LPLL_VBY1_4ch_10bit_4P_37.5to37.5MHz    NO.15
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0004,0x001C},//reg_lpll1_pd
        {0x15,0x0000,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0600,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x3000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0200,0x0F00},//reg_lpll1_scalar_div_first
        {0x35,0x0000,0x7000},//reg_lpll1_scalar_div_second
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div
        {0x2E,0x1000,0x1000},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},// reg_lpll1_fifo_div5_en
        {0x03,0x0400,0x0400},//reg_lpll1_duap_lp_en
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_sdiv3p5_en
        {0x2E,0x0040,0x0040},//reg_lpll1_en_mini
        {0x2E,0x0010,0x0010},//reg_lpll1_en_fifo
        {0x35,0x0000,0x0020},//reg_lpll1_en_scalar
        {0x33,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x33,0x0000,0x0004},//reg_lpll2_pd
        {0x30,0x0000,0x001F},//reg_lpll2_ibias_ictrl
        {0x31,0x0000,0x0003},//reg_lpll2_input_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_first
        {0x32,0x0000,0x000F},//reg_lpll2_loop_div_second
        {0x36,0x0000,0x0800},//reg_lpll2_output_div_first
        {0x36,0x0000,0x1000},//reg_lpll1_test[11]
        {0x36,0x0000,0x4000},//reg_lpll1_test[12]
    },
    #elif PANEL_VBY1_4CH_8Bit
    { //E_PNL_SUPPORTED_LPLL_VBY1_4ch_8bit_4P_50to100MHz    NO.16
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0004,0x001C},//reg_lpll1_pd
        {0x15,0x0000,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0600,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x2000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0300,0x0F00},//reg_lpll1_scalar_div_first
        {0x35,0x0000,0x7000},//reg_lpll1_scalar_div_second
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div
        {0x2E,0x1000,0x1000},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},// reg_lpll1_fifo_div5_en
        {0x03,0x0400,0x0400},//reg_lpll1_duap_lp_en
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_sdiv3p5_en
        {0x2E,0x0040,0x0040},//reg_lpll1_en_mini
        {0x2E,0x0010,0x0010},//reg_lpll1_en_fifo
        {0x35,0x0000,0x0020},//reg_lpll1_en_scalar
        {0x33,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x33,0x0000,0x0004},//reg_lpll2_pd
        {0x30,0x0000,0x001F},//reg_lpll2_ibias_ictrl
        {0x31,0x0000,0x0003},//reg_lpll2_input_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_first
        {0x32,0x0000,0x000F},//reg_lpll2_loop_div_second
        {0x36,0x0000,0x0800},//reg_lpll2_output_div_first
        {0x36,0x0000,0x1000},//reg_lpll1_test[11]
        {0x36,0x0000,0x4000},//reg_lpll1_test[12]
    },
    { //E_PNL_SUPPORTED_LPLL_VBY1_4ch_8bit_4P_50to50MHz    NO.17
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0004,0x001C},//reg_lpll1_pd
        {0x15,0x0000,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0600,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x2000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0300,0x0F00},//reg_lpll1_scalar_div_first
        {0x35,0x0000,0x7000},//reg_lpll1_scalar_div_second
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div
        {0x2E,0x1000,0x1000},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},// reg_lpll1_fifo_div5_en
        {0x03,0x0400,0x0400},//reg_lpll1_duap_lp_en
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_sdiv3p5_en
        {0x2E,0x0040,0x0040},//reg_lpll1_en_mini
        {0x2E,0x0010,0x0010},//reg_lpll1_en_fifo
        {0x35,0x0000,0x0020},//reg_lpll1_en_scalar
        {0x33,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x33,0x0000,0x0004},//reg_lpll2_pd
        {0x30,0x0000,0x001F},//reg_lpll2_ibias_ictrl
        {0x31,0x0000,0x0003},//reg_lpll2_input_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_first
        {0x32,0x0000,0x000F},//reg_lpll2_loop_div_second
        {0x36,0x0000,0x0800},//reg_lpll2_output_div_first
        {0x36,0x0000,0x1000},//reg_lpll1_test[11]
        {0x36,0x0000,0x4000},//reg_lpll1_test[12]
    },
    #elif PANEL_VBY1_8CH_10Bit
    { //E_PNL_SUPPORTED_LPLL_VBY1_8ch_10bit_4P_75to150MHz    NO.18
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0004,0x001C},//reg_lpll1_pd
        {0x15,0x0000,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0600,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x3000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0000,0x0F00},//reg_lpll1_scalar_div_first
        {0x35,0x0000,0x7000},//reg_lpll1_scalar_div_second
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div
        {0x2E,0x1000,0x1000},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},// reg_lpll1_fifo_div5_en
        {0x03,0x0400,0x0400},//reg_lpll1_duap_lp_en
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_sdiv3p5_en
        {0x2E,0x0040,0x0040},//reg_lpll1_en_mini
        {0x2E,0x0010,0x0010},//reg_lpll1_en_fifo
        {0x35,0x0000,0x0020},//reg_lpll1_en_scalar
        {0x33,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x33,0x0000,0x0004},//reg_lpll2_pd
        {0x30,0x0000,0x001F},//reg_lpll2_ibias_ictrl
        {0x31,0x0000,0x0003},//reg_lpll2_input_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_first
        {0x32,0x0000,0x000F},//reg_lpll2_loop_div_second
        {0x36,0x0000,0x0800},//reg_lpll2_output_div_first
        {0x36,0x0000,0x1000},//reg_lpll1_test[11]
        {0x36,0x0000,0x4000},//reg_lpll1_test[12]
    },
    { //E_PNL_SUPPORTED_LPLL_VBY1_8ch_10bit_4P_75to75MHz    NO.19
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0004,0x001C},//reg_lpll1_pd
        {0x15,0x0000,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0600,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x3000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0000,0x0F00},//reg_lpll1_scalar_div_first
        {0x35,0x0000,0x7000},//reg_lpll1_scalar_div_second
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div
        {0x2E,0x1000,0x1000},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},// reg_lpll1_fifo_div5_en
        {0x03,0x0400,0x0400},//reg_lpll1_duap_lp_en
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_sdiv3p5_en
        {0x2E,0x0040,0x0040},//reg_lpll1_en_mini
        {0x2E,0x0010,0x0010},//reg_lpll1_en_fifo
        {0x35,0x0000,0x0020},//reg_lpll1_en_scalar
        {0x33,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x33,0x0000,0x0004},//reg_lpll2_pd
        {0x30,0x0000,0x001F},//reg_lpll2_ibias_ictrl
        {0x31,0x0000,0x0003},//reg_lpll2_input_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_first
        {0x32,0x0000,0x000F},//reg_lpll2_loop_div_second
        {0x36,0x0000,0x0800},//reg_lpll2_output_div_first
        {0x36,0x0000,0x1000},//reg_lpll1_test[11]
        {0x36,0x0000,0x4000},//reg_lpll1_test[12]
    },
    #elif PANEL_VBY1_8CH_8Bit
    { //E_PNL_SUPPORTED_LPLL_VBY1_8ch_8bit_4P_100to200MHz    NO.20
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0004,0x001C},//reg_lpll1_pd
        {0x15,0x0000,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0600,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x1000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0300,0x0F00},//reg_lpll1_scalar_div_first
        {0x35,0x0000,0x7000},//reg_lpll1_scalar_div_second
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div
        {0x2E,0x1000,0x1000},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},// reg_lpll1_fifo_div5_en
        {0x03,0x0400,0x0400},//reg_lpll1_duap_lp_en
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_sdiv3p5_en
        {0x2E,0x0040,0x0040},//reg_lpll1_en_mini
        {0x2E,0x0010,0x0010},//reg_lpll1_en_fifo
        {0x35,0x0000,0x0020},//reg_lpll1_en_scalar
        {0x33,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x33,0x0000,0x0004},//reg_lpll2_pd
        {0x30,0x0000,0x001F},//reg_lpll2_ibias_ictrl
        {0x31,0x0000,0x0003},//reg_lpll2_input_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_first
        {0x32,0x0000,0x000F},//reg_lpll2_loop_div_second
        {0x36,0x0000,0x0800},//reg_lpll2_output_div_first
        {0x36,0x0000,0x1000},//reg_lpll1_test[11]
        {0x36,0x0000,0x4000},//reg_lpll1_test[12]
    },
    { //E_PNL_SUPPORTED_LPLL_VBY1_8ch_8bit_4P_100to100MHz    NO.21
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0004,0x001C},//reg_lpll1_pd
        {0x15,0x0000,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0600,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x1000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0300,0x0F00},//reg_lpll1_scalar_div_first
        {0x35,0x0000,0x7000},//reg_lpll1_scalar_div_second
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div
        {0x2E,0x1000,0x1000},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},// reg_lpll1_fifo_div5_en
        {0x03,0x0400,0x0400},//reg_lpll1_duap_lp_en
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_sdiv3p5_en
        {0x2E,0x0040,0x0040},//reg_lpll1_en_mini
        {0x2E,0x0010,0x0010},//reg_lpll1_en_fifo
        {0x35,0x0000,0x0020},//reg_lpll1_en_scalar
        {0x33,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x33,0x0000,0x0004},//reg_lpll2_pd
        {0x30,0x0000,0x001F},//reg_lpll2_ibias_ictrl
        {0x31,0x0000,0x0003},//reg_lpll2_input_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_first
        {0x32,0x0000,0x000F},//reg_lpll2_loop_div_second
        {0x36,0x0000,0x0800},//reg_lpll2_output_div_first
        {0x36,0x0000,0x1000},//reg_lpll1_test[11]
        {0x36,0x0000,0x4000},//reg_lpll1_test[12]
    },
    #elif PANEL_VBY1_16CH_8Bit
    { //E_PNL_SUPPORTED_LPLL_VBY1_16ch_8bit_HighSpeed_4P_225to300MHz    NO.24
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0004,0x001C},//reg_lpll1_pd
        {0x15,0x0000,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0600,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x0000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0300,0x0F00},//reg_lpll1_scalar_div_first
        {0x35,0x0000,0x7000},//reg_lpll1_scalar_div_second
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div
        {0x2E,0x1000,0x1000},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},// reg_lpll1_fifo_div5_en
        {0x03,0x0400,0x0400},//reg_lpll1_duap_lp_en
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_sdiv3p5_en
        {0x2E,0x0040,0x0040},//reg_lpll1_en_mini
        {0x2E,0x0010,0x0010},//reg_lpll1_en_fifo
        {0x35,0x0000,0x0020},//reg_lpll1_en_scalar
        {0x33,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x33,0x0000,0x0004},//reg_lpll2_pd
        {0x30,0x0000,0x001F},//reg_lpll2_ibias_ictrl
        {0x31,0x0000,0x0003},//reg_lpll2_input_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_first
        {0x32,0x0000,0x000F},//reg_lpll2_loop_div_second
        {0x36,0x0000,0x0800},//reg_lpll2_output_div_first
        {0x36,0x0000,0x1000},//reg_lpll1_test[11]
        {0x36,0x0000,0x4000},//reg_lpll1_test[12]
    },
    { //E_PNL_SUPPORTED_LPLL_VBY1_16ch_8bit_HighSpeed_4P_150to225MHz    NO.25
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0004,0x001C},//reg_lpll1_pd
        {0x15,0x0000,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0600,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x1000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0300,0x0F00},//reg_lpll1_scalar_div_first
        {0x35,0x1000,0x7000},//reg_lpll1_scalar_div_second
        {0x2E,0x0004,0x000F},//reg_lpll1_skew_div
        {0x2E,0x1000,0x1000},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},// reg_lpll1_fifo_div5_en
        {0x03,0x0400,0x0400},//reg_lpll1_duap_lp_en
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_sdiv3p5_en
        {0x2E,0x0040,0x0040},//reg_lpll1_en_mini
        {0x2E,0x0010,0x0010},//reg_lpll1_en_fifo
        {0x35,0x0000,0x0020},//reg_lpll1_en_scalar
        {0x33,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x33,0x0000,0x0004},//reg_lpll2_pd
        {0x30,0x0000,0x001F},//reg_lpll2_ibias_ictrl
        {0x31,0x0000,0x0003},//reg_lpll2_input_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_first
        {0x32,0x0000,0x000F},//reg_lpll2_loop_div_second
        {0x36,0x0000,0x0800},//reg_lpll2_output_div_first
        {0x36,0x0000,0x1000},//reg_lpll1_test[11]
        {0x36,0x0000,0x4000},//reg_lpll1_test[12]
    },
    { //E_PNL_SUPPORTED_LPLL_VBY1_16ch_8bit_HighSpeed_4P_150to150MHz    NO.26
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0004,0x001C},//reg_lpll1_pd
        {0x15,0x0000,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0600,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x1000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0300,0x0F00},//reg_lpll1_scalar_div_first
        {0x35,0x1000,0x7000},//reg_lpll1_scalar_div_second
        {0x2E,0x0004,0x000F},//reg_lpll1_skew_div
        {0x2E,0x1000,0x1000},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},// reg_lpll1_fifo_div5_en
        {0x03,0x0400,0x0400},//reg_lpll1_duap_lp_en
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_sdiv3p5_en
        {0x2E,0x0040,0x0040},//reg_lpll1_en_mini
        {0x2E,0x0010,0x0010},//reg_lpll1_en_fifo
        {0x35,0x0000,0x0020},//reg_lpll1_en_scalar
        {0x33,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x33,0x0000,0x0004},//reg_lpll2_pd
        {0x30,0x0000,0x001F},//reg_lpll2_ibias_ictrl
        {0x31,0x0000,0x0003},//reg_lpll2_input_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_first
        {0x32,0x0000,0x000F},//reg_lpll2_loop_div_second
        {0x36,0x0000,0x0800},//reg_lpll2_output_div_first
        {0x36,0x0000,0x1000},//reg_lpll1_test[11]
        {0x36,0x0000,0x4000},//reg_lpll1_test[12]
    },
    { //E_PNL_SUPPORTED_LPLL_VBY1_16ch_8bit_8P_120to195.75MHz    NO.30
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0004,0x001C},//reg_lpll1_pd
        {0x15,0x0000,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0003,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0400,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x1000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0300,0x0F00},//reg_lpll1_scalar_div_first
        {0x35,0x0000,0x7000},//reg_lpll1_scalar_div_second
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div
        {0x2E,0x1000,0x1000},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},// reg_lpll1_fifo_div5_en
        {0x03,0x0400,0x0400},//reg_lpll1_duap_lp_en
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_sdiv3p5_en
        {0x2E,0x0040,0x0040},//reg_lpll1_en_mini
        {0x2E,0x0010,0x0010},//reg_lpll1_en_fifo
        {0x35,0x0000,0x0020},//reg_lpll1_en_scalar
        {0x33,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x33,0x0000,0x0004},//reg_lpll2_pd
        {0x30,0x0000,0x001F},//reg_lpll2_ibias_ictrl
        {0x31,0x0000,0x0003},//reg_lpll2_input_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_first
        {0x32,0x0000,0x000F},//reg_lpll2_loop_div_second
        {0x36,0x0000,0x0800},//reg_lpll2_output_div_first
        {0x36,0x0000,0x1000},//reg_lpll1_test[11]
        {0x36,0x0000,0x4000},//reg_lpll1_test[12]
    },
    { //E_PNL_SUPPORTED_LPLL_VBY1_16ch_8bit_8P_97.88to120MHz    NO.31
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0004,0x001C},//reg_lpll1_pd
        {0x15,0x0000,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0003,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0400,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x2000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0300,0x0F00},//reg_lpll1_scalar_div_first
        {0x35,0x1000,0x7000},//reg_lpll1_scalar_div_second
        {0x2E,0x0004,0x000F},//reg_lpll1_skew_div
        {0x2E,0x1000,0x1000},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},// reg_lpll1_fifo_div5_en
        {0x03,0x0400,0x0400},//reg_lpll1_duap_lp_en
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_sdiv3p5_en
        {0x2E,0x0040,0x0040},//reg_lpll1_en_mini
        {0x2E,0x0010,0x0010},//reg_lpll1_en_fifo
        {0x35,0x0000,0x0020},//reg_lpll1_en_scalar
        {0x33,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x33,0x0000,0x0004},//reg_lpll2_pd
        {0x30,0x0000,0x001F},//reg_lpll2_ibias_ictrl
        {0x31,0x0000,0x0003},//reg_lpll2_input_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_first
        {0x32,0x0000,0x000F},//reg_lpll2_loop_div_second
        {0x36,0x0000,0x0800},//reg_lpll2_output_div_first
        {0x36,0x0000,0x1000},//reg_lpll1_test[11]
        {0x36,0x0000,0x4000},//reg_lpll1_test[12]
    },
    { //E_PNL_SUPPORTED_LPLL_VBY1_16ch_8bit_8P_97.88to97.88MHz    NO.32
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0004,0x001C},//reg_lpll1_pd
        {0x15,0x0000,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0003,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0400,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x2000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0300,0x0F00},//reg_lpll1_scalar_div_first
        {0x35,0x1000,0x7000},//reg_lpll1_scalar_div_second
        {0x2E,0x0004,0x000F},//reg_lpll1_skew_div
        {0x2E,0x1000,0x1000},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},// reg_lpll1_fifo_div5_en
        {0x03,0x0400,0x0400},//reg_lpll1_duap_lp_en
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_sdiv3p5_en
        {0x2E,0x0040,0x0040},//reg_lpll1_en_mini
        {0x2E,0x0010,0x0010},//reg_lpll1_en_fifo
        {0x35,0x0000,0x0020},//reg_lpll1_en_scalar
        {0x33,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x33,0x0000,0x0004},//reg_lpll2_pd
        {0x30,0x0000,0x001F},//reg_lpll2_ibias_ictrl
        {0x31,0x0000,0x0003},//reg_lpll2_input_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_first
        {0x32,0x0000,0x000F},//reg_lpll2_loop_div_second
        {0x36,0x0000,0x0800},//reg_lpll2_output_div_first
        {0x36,0x0000,0x1000},//reg_lpll1_test[11]
        {0x36,0x0000,0x4000},//reg_lpll1_test[12]
    },
    #elif PANEL_VBY1_16CH_10Bit
    { //E_PNL_SUPPORTED_LPLL_VBY1_16ch_10bit _HighSpeed_4P_150to300MHz    NO.22
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0004,0x001C},//reg_lpll1_pd
        {0x15,0x0000,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0600,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x2000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0000,0x0F00},//reg_lpll1_scalar_div_first
        {0x35,0x0000,0x7000},//reg_lpll1_scalar_div_second
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div
        {0x2E,0x1000,0x1000},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},// reg_lpll1_fifo_div5_en
        {0x03,0x0400,0x0400},//reg_lpll1_duap_lp_en
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_sdiv3p5_en
        {0x2E,0x0040,0x0040},//reg_lpll1_en_mini
        {0x2E,0x0010,0x0010},//reg_lpll1_en_fifo
        {0x35,0x0000,0x0020},//reg_lpll1_en_scalar
        {0x33,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x33,0x0000,0x0004},//reg_lpll2_pd
        {0x30,0x0000,0x001F},//reg_lpll2_ibias_ictrl
        {0x31,0x0000,0x0003},//reg_lpll2_input_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_first
        {0x32,0x0000,0x000F},//reg_lpll2_loop_div_second
        {0x36,0x0000,0x0800},//reg_lpll2_output_div_first
        {0x36,0x0000,0x1000},//reg_lpll1_test[11]
        {0x36,0x0000,0x4000},//reg_lpll1_test[12]
    },
    { //E_PNL_SUPPORTED_LPLL_VBY1_16ch_10bit _HighSpeed_4P_150to150MHz    NO.23
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0004,0x001C},//reg_lpll1_pd
        {0x15,0x0000,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0600,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x2000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0000,0x0F00},//reg_lpll1_scalar_div_first
        {0x35,0x0000,0x7000},//reg_lpll1_scalar_div_second
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div
        {0x2E,0x1000,0x1000},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},// reg_lpll1_fifo_div5_en
        {0x03,0x0400,0x0400},//reg_lpll1_duap_lp_en
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_sdiv3p5_en
        {0x2E,0x0040,0x0040},//reg_lpll1_en_mini
        {0x2E,0x0010,0x0010},//reg_lpll1_en_fifo
        {0x35,0x0000,0x0020},//reg_lpll1_en_scalar
        {0x33,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x33,0x0000,0x0004},//reg_lpll2_pd
        {0x30,0x0000,0x001F},//reg_lpll2_ibias_ictrl
        {0x31,0x0000,0x0003},//reg_lpll2_input_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_first
        {0x32,0x0000,0x000F},//reg_lpll2_loop_div_second
        {0x36,0x0000,0x0800},//reg_lpll2_output_div_first
        {0x36,0x0000,0x1000},//reg_lpll1_test[11]
        {0x36,0x0000,0x4000},//reg_lpll1_test[12]
    },
    { //E_PNL_SUPPORTED_LPLL_VBY1_16ch_10bit_8P_135to195.75MHz    NO.27
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0004,0x001C},//reg_lpll1_pd
        {0x15,0x0000,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0600,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x2000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0200,0x0F00},//reg_lpll1_scalar_div_first
        {0x35,0x0000,0x7000},//reg_lpll1_scalar_div_second
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div
        {0x2E,0x1000,0x1000},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},// reg_lpll1_fifo_div5_en
        {0x03,0x0400,0x0400},//reg_lpll1_duap_lp_en
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_sdiv3p5_en
        {0x2E,0x0040,0x0040},//reg_lpll1_en_mini
        {0x2E,0x0010,0x0010},//reg_lpll1_en_fifo
        {0x35,0x0000,0x0020},//reg_lpll1_en_scalar
        {0x33,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x33,0x0000,0x0004},//reg_lpll2_pd
        {0x30,0x0000,0x001F},//reg_lpll2_ibias_ictrl
        {0x31,0x0000,0x0003},//reg_lpll2_input_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_first
        {0x32,0x0000,0x000F},//reg_lpll2_loop_div_second
        {0x36,0x0000,0x0800},//reg_lpll2_output_div_first
        {0x36,0x0000,0x1000},//reg_lpll1_test[11]
        {0x36,0x0000,0x4000},//reg_lpll1_test[12]
    },
    { //E_PNL_SUPPORTED_LPLL_VBY1_16ch_10bit_8P_97.88to135MHz    NO.28
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0004,0x001C},//reg_lpll1_pd
        {0x15,0x0000,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0600,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x2000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0200,0x0F00},//reg_lpll1_scalar_div_first
        {0x35,0x0000,0x7000},//reg_lpll1_scalar_div_second
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div
        {0x2E,0x1000,0x1000},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},// reg_lpll1_fifo_div5_en
        {0x03,0x0400,0x0400},//reg_lpll1_duap_lp_en
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_sdiv3p5_en
        {0x2E,0x0040,0x0040},//reg_lpll1_en_mini
        {0x2E,0x0010,0x0010},//reg_lpll1_en_fifo
        {0x35,0x0000,0x0020},//reg_lpll1_en_scalar
        {0x33,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x33,0x0000,0x0004},//reg_lpll2_pd
        {0x30,0x0000,0x001F},//reg_lpll2_ibias_ictrl
        {0x31,0x0000,0x0003},//reg_lpll2_input_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_first
        {0x32,0x0000,0x000F},//reg_lpll2_loop_div_second
        {0x36,0x0000,0x0800},//reg_lpll2_output_div_first
        {0x36,0x0000,0x1000},//reg_lpll1_test[11]
        {0x36,0x0000,0x4000},//reg_lpll1_test[12]
    },
    { //E_PNL_SUPPORTED_LPLL_VBY1_16ch_10bit_8P_97.88to97.88MHz    NO.29
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0004,0x001C},//reg_lpll1_pd
        {0x15,0x0000,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0600,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x2000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0200,0x0F00},//reg_lpll1_scalar_div_first
        {0x35,0x0000,0x7000},//reg_lpll1_scalar_div_second
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div
        {0x2E,0x1000,0x1000},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},// reg_lpll1_fifo_div5_en
        {0x03,0x0400,0x0400},//reg_lpll1_duap_lp_en
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_sdiv3p5_en
        {0x2E,0x0040,0x0040},//reg_lpll1_en_mini
        {0x2E,0x0010,0x0010},//reg_lpll1_en_fifo
        {0x35,0x0000,0x0020},//reg_lpll1_en_scalar
        {0x33,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x33,0x0000,0x0004},//reg_lpll2_pd
        {0x30,0x0000,0x001F},//reg_lpll2_ibias_ictrl
        {0x31,0x0000,0x0003},//reg_lpll2_input_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_first
        {0x32,0x0000,0x000F},//reg_lpll2_loop_div_second
        {0x36,0x0000,0x0800},//reg_lpll2_output_div_first
        {0x36,0x0000,0x1000},//reg_lpll1_test[11]
        {0x36,0x0000,0x4000},//reg_lpll1_test[12]
    },
    #elif PANEL_VBY1_20CH_10Bit
    { //E_PNL_SUPPORTED_LPLL_VBY1_20ch_10bit_8P _195.75to391.5MHz    NO.33
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0004,0x001C},//reg_lpll1_pd
        {0x15,0x0000,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0600,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x3000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0000,0x0F00},//reg_lpll1_scalar_div_first
        {0x35,0x0000,0x7000},//reg_lpll1_scalar_div_second
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div
        {0x2E,0x1000,0x1000},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},// reg_lpll1_fifo_div5_en
        {0x03,0x0000,0x0400},//reg_lpll1_duap_lp_en
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_sdiv3p5_en
        {0x2E,0x0040,0x0040},//reg_lpll1_en_mini
        {0x2E,0x0010,0x0010},//reg_lpll1_en_fifo
        {0x35,0x0000,0x0020},//reg_lpll1_en_scalar
        {0x33,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x33,0x0000,0x0004},//reg_lpll2_pd
        {0x30,0x0000,0x001F},//reg_lpll2_ibias_ictrl
        {0x31,0x0000,0x0003},//reg_lpll2_input_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_first
        {0x32,0x0000,0x000F},//reg_lpll2_loop_div_second
        {0x36,0x0000,0x0800},//reg_lpll2_output_div_first
        {0x36,0x0000,0x1000},//reg_lpll1_test[11]
        {0x36,0x0000,0x4000},//reg_lpll1_test[12]
    },
    { //E_PNL_SUPPORTED_LPLL_VBY1_20ch_10bit_8P _195.75to195.75MHz    NO.34
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0004,0x001C},//reg_lpll1_pd
        {0x15,0x0000,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0600,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x3000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0000,0x0F00},//reg_lpll1_scalar_div_first
        {0x35,0x0000,0x7000},//reg_lpll1_scalar_div_second
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div
        {0x2E,0x1000,0x1000},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},// reg_lpll1_fifo_div5_en
        {0x03,0x0000,0x0400},//reg_lpll1_duap_lp_en
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_sdiv3p5_en
        {0x2E,0x0040,0x0040},//reg_lpll1_en_mini
        {0x2E,0x0010,0x0010},//reg_lpll1_en_fifo
        {0x35,0x0000,0x0020},//reg_lpll1_en_scalar
        {0x33,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x33,0x0000,0x0004},//reg_lpll2_pd
        {0x30,0x0000,0x001F},//reg_lpll2_ibias_ictrl
        {0x31,0x0000,0x0003},//reg_lpll2_input_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_first
        {0x32,0x0000,0x000F},//reg_lpll2_loop_div_second
        {0x36,0x0000,0x0800},//reg_lpll2_output_div_first
        {0x36,0x0000,0x1000},//reg_lpll1_test[11]
        {0x36,0x0000,0x4000},//reg_lpll1_test[12]
    },
    #elif PANEL_VBY1_20CH_8Bit
    { //E_PNL_SUPPORTED_LPLL_VBY1_20ch_8bit_8P_250to391.5MHz    NO.35
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0004,0x001C},//reg_lpll1_pd
        {0x15,0x0000,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0600,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x1000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0300,0x0F00},//reg_lpll1_scalar_div_first
        {0x35,0x0000,0x7000},//reg_lpll1_scalar_div_second
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div
        {0x2E,0x1000,0x1000},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},// reg_lpll1_fifo_div5_en
        {0x03,0x0000,0x0400},//reg_lpll1_duap_lp_en
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_sdiv3p5_en
        {0x2E,0x0040,0x0040},//reg_lpll1_en_mini
        {0x2E,0x0010,0x0010},//reg_lpll1_en_fifo
        {0x35,0x0000,0x0020},//reg_lpll1_en_scalar
        {0x33,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x33,0x0000,0x0004},//reg_lpll2_pd
        {0x30,0x0000,0x001F},//reg_lpll2_ibias_ictrl
        {0x31,0x0000,0x0003},//reg_lpll2_input_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_first
        {0x32,0x0000,0x000F},//reg_lpll2_loop_div_second
        {0x36,0x0000,0x0800},//reg_lpll2_output_div_first
        {0x36,0x0000,0x1000},//reg_lpll1_test[11]
        {0x36,0x0000,0x4000},//reg_lpll1_test[12]
    },
    { //E_PNL_SUPPORTED_LPLL_VBY1_20ch_8bit_8P_195.75to250MHz    NO.36
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0004,0x001C},//reg_lpll1_pd
        {0x15,0x0000,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0600,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x2000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0300,0x0F00},//reg_lpll1_scalar_div_first
        {0x35,0x1000,0x7000},//reg_lpll1_scalar_div_second
        {0x2E,0x0004,0x000F},//reg_lpll1_skew_div
        {0x2E,0x1000,0x1000},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},// reg_lpll1_fifo_div5_en
        {0x03,0x0000,0x0400},//reg_lpll1_duap_lp_en
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_sdiv3p5_en
        {0x2E,0x0040,0x0040},//reg_lpll1_en_mini
        {0x2E,0x0010,0x0010},//reg_lpll1_en_fifo
        {0x35,0x0000,0x0020},//reg_lpll1_en_scalar
        {0x33,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x33,0x0000,0x0004},//reg_lpll2_pd
        {0x30,0x0000,0x001F},//reg_lpll2_ibias_ictrl
        {0x31,0x0000,0x0003},//reg_lpll2_input_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_first
        {0x32,0x0000,0x000F},//reg_lpll2_loop_div_second
        {0x36,0x0000,0x0800},//reg_lpll2_output_div_first
        {0x36,0x0000,0x1000},//reg_lpll1_test[11]
        {0x36,0x0000,0x4000},//reg_lpll1_test[12]
    },
    { //E_PNL_SUPPORTED_LPLL_VBY1_20ch_8bit_8P_195.75to195.75MHz    NO.37
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0004,0x001C},//reg_lpll1_pd
        {0x15,0x0000,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0600,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x2000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0300,0x0F00},//reg_lpll1_scalar_div_first
        {0x35,0x1000,0x7000},//reg_lpll1_scalar_div_second
        {0x2E,0x0004,0x000F},//reg_lpll1_skew_div
        {0x2E,0x1000,0x1000},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},// reg_lpll1_fifo_div5_en
        {0x03,0x0000,0x0400},//reg_lpll1_duap_lp_en
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_sdiv3p5_en
        {0x2E,0x0040,0x0040},//reg_lpll1_en_mini
        {0x2E,0x0010,0x0010},//reg_lpll1_en_fifo
        {0x35,0x0000,0x0020},//reg_lpll1_en_scalar
        {0x33,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x33,0x0000,0x0004},//reg_lpll2_pd
        {0x30,0x0000,0x001F},//reg_lpll2_ibias_ictrl
        {0x31,0x0000,0x0003},//reg_lpll2_input_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_first
        {0x32,0x0000,0x000F},//reg_lpll2_loop_div_second
        {0x36,0x0000,0x0800},//reg_lpll2_output_div_first
        {0x36,0x0000,0x1000},//reg_lpll1_test[11]
        {0x36,0x0000,0x4000},//reg_lpll1_test[12]
    },
    #elif PANEL_VBY1_32CH_10Bit
    { //E_PNL_SUPPORTED_LPLL_VBY1_32ch_10bit_8P_225to300MHz    NO.38
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0004,0x001C},//reg_lpll1_pd
        {0x15,0x0000,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0600,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x2000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0000,0x0F00},//reg_lpll1_scalar_div_first
        {0x35,0x0000,0x7000},//reg_lpll1_scalar_div_second
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div
        {0x2E,0x1000,0x1000},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},// reg_lpll1_fifo_div5_en
        {0x03,0x0400,0x0400},//reg_lpll1_duap_lp_en
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_sdiv3p5_en
        {0x2E,0x0040,0x0040},//reg_lpll1_en_mini
        {0x2E,0x0010,0x0010},//reg_lpll1_en_fifo
        {0x35,0x0000,0x0020},//reg_lpll1_en_scalar
        {0x33,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x33,0x0000,0x0004},//reg_lpll2_pd
        {0x30,0x0000,0x001F},//reg_lpll2_ibias_ictrl
        {0x31,0x0000,0x0003},//reg_lpll2_input_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_first
        {0x32,0x0000,0x000F},//reg_lpll2_loop_div_second
        {0x36,0x0000,0x0800},//reg_lpll2_output_div_first
        {0x36,0x0000,0x1000},//reg_lpll1_test[11]
        {0x36,0x0000,0x4000},//reg_lpll1_test[12]
    },
    { //E_PNL_SUPPORTED_LPLL_VBY1_32ch_10bit_8P_150to225MHz    NO.39
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0004,0x001C},//reg_lpll1_pd
        {0x15,0x0000,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0600,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x2000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0000,0x0F00},//reg_lpll1_scalar_div_first
        {0x35,0x0000,0x7000},//reg_lpll1_scalar_div_second
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div
        {0x2E,0x1000,0x1000},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},// reg_lpll1_fifo_div5_en
        {0x03,0x0400,0x0400},//reg_lpll1_duap_lp_en
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_sdiv3p5_en
        {0x2E,0x0040,0x0040},//reg_lpll1_en_mini
        {0x2E,0x0010,0x0010},//reg_lpll1_en_fifo
        {0x35,0x0000,0x0020},//reg_lpll1_en_scalar
        {0x33,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x33,0x0000,0x0004},//reg_lpll2_pd
        {0x30,0x0000,0x001F},//reg_lpll2_ibias_ictrl
        {0x31,0x0000,0x0003},//reg_lpll2_input_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_first
        {0x32,0x0000,0x000F},//reg_lpll2_loop_div_second
        {0x36,0x0000,0x0800},//reg_lpll2_output_div_first
        {0x36,0x0000,0x1000},//reg_lpll1_test[11]
        {0x36,0x0000,0x4000},//reg_lpll1_test[12]
    },
    { //E_PNL_SUPPORTED_LPLL_VBY1_32ch_10bit_8P_150to150MHz    NO.40
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0004,0x001C},//reg_lpll1_pd
        {0x15,0x0000,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0600,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x2000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0000,0x0F00},//reg_lpll1_scalar_div_first
        {0x35,0x0000,0x7000},//reg_lpll1_scalar_div_second
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div
        {0x2E,0x1000,0x1000},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},// reg_lpll1_fifo_div5_en
        {0x03,0x0400,0x0400},//reg_lpll1_duap_lp_en
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_sdiv3p5_en
        {0x2E,0x0040,0x0040},//reg_lpll1_en_mini
        {0x2E,0x0010,0x0010},//reg_lpll1_en_fifo
        {0x35,0x0000,0x0020},//reg_lpll1_en_scalar
        {0x33,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x33,0x0000,0x0004},//reg_lpll2_pd
        {0x30,0x0000,0x001F},//reg_lpll2_ibias_ictrl
        {0x31,0x0000,0x0003},//reg_lpll2_input_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_first
        {0x32,0x0000,0x000F},//reg_lpll2_loop_div_second
        {0x36,0x0000,0x0800},//reg_lpll2_output_div_first
        {0x36,0x0000,0x1000},//reg_lpll1_test[11]
        {0x36,0x0000,0x4000},//reg_lpll1_test[12]
    },
    #elif PANEL_VBY1_32CH_8Bit
    { //E_PNL_SUPPORTED_LPLL_VBY1_32ch_8bit_8P_225to300MHz    NO.41
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0004,0x001C},//reg_lpll1_pd
        {0x15,0x0000,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0600,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x0000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0300,0x0F00},//reg_lpll1_scalar_div_first
        {0x35,0x0000,0x7000},//reg_lpll1_scalar_div_second
        {0x2E,0x0000,0x000F},//reg_lpll1_skew_div
        {0x2E,0x1000,0x1000},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},// reg_lpll1_fifo_div5_en
        {0x03,0x0400,0x0400},//reg_lpll1_duap_lp_en
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_sdiv3p5_en
        {0x2E,0x0040,0x0040},//reg_lpll1_en_mini
        {0x2E,0x0010,0x0010},//reg_lpll1_en_fifo
        {0x35,0x0000,0x0020},//reg_lpll1_en_scalar
        {0x33,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x33,0x0000,0x0004},//reg_lpll2_pd
        {0x30,0x0000,0x001F},//reg_lpll2_ibias_ictrl
        {0x31,0x0000,0x0003},//reg_lpll2_input_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_first
        {0x32,0x0000,0x000F},//reg_lpll2_loop_div_second
        {0x36,0x0000,0x0800},//reg_lpll2_output_div_first
        {0x36,0x0000,0x1000},//reg_lpll1_test[11]
        {0x36,0x0000,0x4000},//reg_lpll1_test[12]
    },
    { //E_PNL_SUPPORTED_LPLL_VBY1_32ch_8bit_8P_150to225MHz    NO.42
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0004,0x001C},//reg_lpll1_pd
        {0x15,0x0000,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0600,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x1000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0300,0x0F00},//reg_lpll1_scalar_div_first
        {0x35,0x1000,0x7000},//reg_lpll1_scalar_div_second
        {0x2E,0x0004,0x000F},//reg_lpll1_skew_div
        {0x2E,0x1000,0x1000},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},// reg_lpll1_fifo_div5_en
        {0x03,0x0400,0x0400},//reg_lpll1_duap_lp_en
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_sdiv3p5_en
        {0x2E,0x0040,0x0040},//reg_lpll1_en_mini
        {0x2E,0x0010,0x0010},//reg_lpll1_en_fifo
        {0x35,0x0000,0x0020},//reg_lpll1_en_scalar
        {0x33,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x33,0x0000,0x0004},//reg_lpll2_pd
        {0x30,0x0000,0x001F},//reg_lpll2_ibias_ictrl
        {0x31,0x0000,0x0003},//reg_lpll2_input_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_first
        {0x32,0x0000,0x000F},//reg_lpll2_loop_div_second
        {0x36,0x0000,0x0800},//reg_lpll2_output_div_first
        {0x36,0x0000,0x1000},//reg_lpll1_test[11]
        {0x36,0x0000,0x4000},//reg_lpll1_test[12]
    },
    { //E_PNL_SUPPORTED_LPLL_VBY1_32ch_8bit_8P_150to150MHz    NO.43
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//
        {0x03,0x0004,0x001C},//reg_lpll1_pd
        {0x15,0x0000,0x0003},//reg_lpll1_ibias_ictrl
        {0x01,0x0002,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0600,0x0F00},//reg_lpll1_loop_div_first
        {0x02,0x1000,0x3000},//reg_lpll1_loop_div_second
        {0x02,0x0300,0x0F00},//reg_lpll1_scalar_div_first
        {0x35,0x1000,0x7000},//reg_lpll1_scalar_div_second
        {0x2E,0x0004,0x000F},//reg_lpll1_skew_div
        {0x2E,0x1000,0x1000},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},// reg_lpll1_fifo_div5_en
        {0x03,0x0400,0x0400},//reg_lpll1_duap_lp_en
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_sdiv3p5_en
        {0x2E,0x0040,0x0040},//reg_lpll1_en_mini
        {0x2E,0x0010,0x0010},//reg_lpll1_en_fifo
        {0x35,0x0000,0x0020},//reg_lpll1_en_scalar
        {0x33,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x33,0x0000,0x0004},//reg_lpll2_pd
        {0x30,0x0000,0x001F},//reg_lpll2_ibias_ictrl
        {0x31,0x0000,0x0003},//reg_lpll2_input_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_first
        {0x32,0x0000,0x000F},//reg_lpll2_loop_div_second
        {0x36,0x0000,0x0800},//reg_lpll2_output_div_first
        {0x36,0x0000,0x1000},//reg_lpll1_test[11]
        {0x36,0x0000,0x4000},//reg_lpll1_test[12]
    },
    #elif PANEL_EDP

    #endif
};
#else
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
    E_PNL_SUPPORTED_LPLL_LVDS_2ch_2P_75to150MHz,            //5 same as 2ch_2P_150to300M
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

code TBLStruct LPLLSettingTBL[E_PNL_SUPPORTED_LPLL_MAX][LPLL_REG_NUM]=
{
#if PANEL_TTL
    { //E_PNL_SUPPORTED_LPLL_TTL_75to150MHz    NO.0
      //Address,Value,Mask
        {0x03,0x0000,0x0020},
        {0x03,0x0002,0x0003},
        {0x03,0x0010,0x001C},
        {0x01,0x0003,0x0003},
        {0x01,0x0200,0x0F00},
        {0x02,0x1000,0x3000},
        {0x02,0x0700,0x0F00},
        {0x35,0x0000,0x7000},
        {0x2E,0x0000,0x000F},
        {0x37,0x0000,0x0004},
        {0x37,0x0000,0x4000},
        {0x03,0x0000,0x0200},
        {0x2E,0x0000,0x0040},
        {0x35,0x0F00,0x0F00},
        {0x2E,0x0020,0x0020},
        {0x37,0x0002,0x0002},
        {0x2C,0x0000,0x8000},
        {0x2E,0x0010,0x0010},
    },

    { //E_PNL_SUPPORTED_LPLL_TTL_40to75MHz    NO.1
      //Address,Value,Mask
        {0x03,0x0000,0x0020},
        {0x03,0x0002,0x0003},
        {0x03,0x0010,0x001C},
        {0x01,0x0003,0x0003},
        {0x01,0x0200,0x0F00},
        {0x02,0x2000,0x3000},
        {0x02,0x0700,0x0F00},
        {0x35,0x0000,0x7000},
        {0x2E,0x0000,0x000F},
        {0x37,0x0000,0x0004},
        {0x37,0x0000,0x4000},
        {0x03,0x0000,0x0200},
        {0x2E,0x0000,0x0040},
        {0x35,0x0F00,0x0F00},
        {0x2E,0x0020,0x0020},
        {0x37,0x0002,0x0002},
        {0x2C,0x0000,0x8000},
        {0x2E,0x0010,0x0010},
    },

    { //E_PNL_SUPPORTED_LPLL_TTL_25to40MHz    NO.2
      //Address,Value,Mask
        {0x03,0x0000,0x0020},
        {0x03,0x0002,0x0003},
        {0x03,0x0010,0x001C},
        {0x01,0x0003,0x0003},
        {0x01,0x0200,0x0F00},
        {0x02,0x2000,0x3000},
        {0x02,0x0E00,0x0F00},
        {0x35,0x0000,0x7000},
        {0x2E,0x0000,0x000F},
        {0x37,0x0000,0x0004},
        {0x37,0x0000,0x4000},
        {0x03,0x0000,0x0200},
        {0x2E,0x0000,0x0040},
        {0x35,0x0F00,0x0F00},
        {0x2E,0x0020,0x0020},
        {0x37,0x0002,0x0002},
        {0x2C,0x0000,0x8000},
        {0x2E,0x0010,0x0010},
    },
#elif PANEL_LVDS_1CH
    { //E_PNL_SUPPORTED_LPLL_LVDS_1ch_1P_75to150MHz    NO.3
      //Address,Value,Mask
        {0x03,0x0000,0x0020},
        {0x03,0x0002,0x0003},
        {0x03,0x000C,0x001C},
        {0x01,0x0003,0x0003},
        {0x01,0x0200,0x0F00},
        {0x02,0x1000,0x3000},
        {0x02,0x0700,0x0F00},
        {0x35,0x0000,0x7000},
        {0x2E,0x0000,0x000F},
        {0x37,0x0000,0x0004},
        {0x37,0x0000,0x4000},
        {0x03,0x0000,0x0200},
        {0x2E,0x0000,0x0040},
        {0x35,0x0F00,0x0F00},
        {0x2E,0x0020,0x0020},
        {0x37,0x0000,0x0002},
        {0x2C,0x0000,0x8000},
        {0x2E,0x0010,0x0010},
    },
#elif PANEL_LVDS_2CH
    { //E_PNL_SUPPORTED_LPLL_LVDS_2ch_1P_150to300MHz    NO.4
      //Address,Value,Mask
        {0x03,0x0000,0x0020},
        {0x03,0x0002,0x0003},
        {0x03,0x000C,0x001C},
        {0x01,0x0003,0x0003},
        {0x01,0x0200,0x0F00},
        {0x02,0x0000,0x3000},
        {0x02,0x0700,0x0F00},
        {0x35,0x0000,0x7000},
        {0x2E,0x0000,0x000F},
        {0x37,0x0000,0x0004},
        {0x37,0x0000,0x4000},
        {0x03,0x0000,0x0200},
        {0x2E,0x0000,0x0040},
        {0x35,0x0F00,0x0F00},
        {0x2E,0x0020,0x0020},
        {0x37,0x0000,0x0002},
        {0x2C,0x0000,0x8000},
        {0x2E,0x0010,0x0010},
    },
    { //E_PNL_SUPPORTED_LPLL_LVDS_2ch_2P_75to150MHz    NO.5
      //Address,Value,Mask
        {0x03,0x0000,0x0020},
        {0x03,0x0002,0x0003},
        {0x03,0x000C,0x001C},
        {0x01,0x0003,0x0003},
        {0x01,0x0200,0x0F00},
        {0x02,0x1000,0x3000},
        {0x02,0x0700,0x0F00},
        {0x35,0x0000,0x7000},
        {0x2E,0x0000,0x000F},
        {0x37,0x0000,0x0004},
        {0x37,0x0000,0x4000},
        {0x03,0x0000,0x0200},
        {0x2E,0x0000,0x0040},
        {0x35,0x0F00,0x0F00},
        {0x2E,0x0020,0x0020},
        {0x37,0x0000,0x0002},
        {0x2C,0x0000,0x8000},
        {0x2E,0x0010,0x0010},
    },
#elif PANEL_LVDS_4CH
    { //E_PNL_SUPPORTED_LPLL_LVDS_4ch_1P_300to300MHz    NO.6
      //Address,Value,Mask
        {0x03,0x0000,0x0020},
        {0x03,0x0002,0x0003},
        {0x03,0x000C,0x001C},
        {0x01,0x0003,0x0003},
        {0x01,0x0200,0x0F00},
        {0x02,0x0000,0x3000},
        {0x02,0x0700,0x0F00},
        {0x35,0x1000,0x7000},
        {0x2E,0x0000,0x000F},
        {0x37,0x0000,0x0004},
        {0x37,0x0000,0x4000},
        {0x03,0x0000,0x0200},
        {0x2E,0x0000,0x0040},
        {0x35,0x0F00,0x0F00},
        {0x2E,0x0020,0x0020},
        {0x37,0x0000,0x0002},
        {0x2C,0x0000,0x8000},
        {0x2E,0x0010,0x0010},
    },

    { //E_PNL_SUPPORTED_LPLL_LVDS_4ch_2P_150to300MHz    NO.7
      //Address,Value,Mask
        {0x03,0x0000,0x0020},
        {0x03,0x0002,0x0003},
        {0x03,0x000C,0x001C},
        {0x01,0x0003,0x0003},
        {0x01,0x0200,0x0F00},
        {0x02,0x0000,0x3000},
        {0x02,0x0700,0x0F00},
        {0x35,0x0000,0x7000},
        {0x2E,0x0000,0x000F},
        {0x37,0x0000,0x0004},
        {0x37,0x0000,0x4000},
        {0x03,0x0000,0x0200},
        {0x2E,0x0000,0x0040},
        {0x35,0x0F00,0x0F00},
        {0x2E,0x0020,0x0020},
        {0x37,0x0000,0x0002},
        {0x2C,0x0000,0x8000},
        {0x2E,0x0010,0x0010},
    },
#elif PANEL_LVDS_8CH
    { //E_PNL_SUPPORTED_LPLL_LVDS_8ch_2P_150to300MHz    NO.8
      //Address,Value,Mask
        {0x03,0x0000,0x0020},
        {0x03,0x0002,0x0003},
        {0x03,0x000C,0x001C},
        {0x01,0x0003,0x0003},
        {0x01,0x0200,0x0F00},
        {0x02,0x0000,0x3000},
        {0x02,0x0700,0x0F00},
        {0x35,0x1000,0x7000},
        {0x2E,0x0000,0x000F},
        {0x37,0x0000,0x0004},
        {0x37,0x0000,0x4000},
        {0x03,0x0000,0x0200},
        {0x2E,0x0000,0x0040},
        {0x35,0x0F00,0x0F00},
        {0x2E,0x0020,0x0020},
        {0x37,0x0000,0x0002},
        {0x2C,0x0000,0x8000},
        {0x2E,0x0010,0x0010},
    },
#elif PANEL_VBY1_1CH_10Bit
    { //E_PNL_SUPPORTED_LPLL_VBY1_1ch_10bit_1P_37_5to75MHz    NO.9
      //Address,Value,Mask
        {0x03,0x0000,0x0020},
        {0x03,0x0003,0x0003},
        {0x03,0x0010,0x001C},
        {0x01,0x0002,0x0003},
        {0x01,0x0500,0x0F00},
        {0x02,0x3000,0x3000},
        {0x02,0x0200,0x0F00},
        {0x35,0x4000,0x7000},
        {0x2E,0x0004,0x000F},
        {0x37,0x0004,0x0004},
        {0x37,0x4000,0x4000},
        {0x03,0x0000,0x0200},
        {0x2E,0x0040,0x0040},
        {0x35,0x0F00,0x0F00},
        {0x2E,0x0020,0x0020},
        {0x37,0x0000,0x0002},
        {0x2C,0x0000,0x8000},
        {0x2E,0x0010,0x0010},
    },
#elif PANEL_VBY1_1CH_8Bit
    { //E_PNL_SUPPORTED_LPLL_VBY1_1ch_8bit_1P_37_5to75MHz    NO.10
      //Address,Value,Mask
        {0x03,0x0000,0x0020},
        {0x03,0x0003,0x0003},
        {0x03,0x0010,0x001C},
        {0x01,0x0002,0x0003},
        {0x01,0x0500,0x0F00},
        {0x02,0x2000,0x3000},
        {0x02,0x0300,0x0F00},
        {0x35,0x4000,0x7000},
        {0x2E,0x0004,0x000F},
        {0x37,0x0004,0x0004},
        {0x37,0x4000,0x4000},
        {0x03,0x0000,0x0200},
        {0x2E,0x0040,0x0040},
        {0x35,0x0F00,0x0F00},
        {0x2E,0x0020,0x0020},
        {0x37,0x0000,0x0002},
        {0x2C,0x0000,0x8000},
        {0x2E,0x0010,0x0010},
    },
#elif PANEL_VBY1_2CH_10Bit
    { //E_PNL_SUPPORTED_LPLL_VBY1_2ch_10bit_1P_75to150MHz    NO.11
      //Address,Value,Mask
        {0x03,0x0000,0x0020},
        {0x03,0x0003,0x0003},
        {0x03,0x0010,0x001C},
        {0x01,0x0002,0x0003},
        {0x01,0x0500,0x0F00},
        {0x02,0x2000,0x3000},
        {0x02,0x0200,0x0F00},
        {0x35,0x4000,0x7000},
        {0x2E,0x0004,0x000F},
        {0x37,0x0004,0x0004},
        {0x37,0x4000,0x4000},
        {0x03,0x0000,0x0200},
        {0x2E,0x0040,0x0040},
        {0x35,0x0F00,0x0F00},
        {0x2E,0x0020,0x0020},
        {0x37,0x0000,0x0002},
        {0x2C,0x0000,0x8000},
        {0x2E,0x0010,0x0010},
    },
#elif PANEL_VBY1_2CH_8Bit
    { //E_PNL_SUPPORTED_LPLL_VBY1_2ch_8bit_1P_75to150MHz    NO.12
      //Address,Value,Mask
        {0x03,0x0000,0x0020},
        {0x03,0x0003,0x0003},
        {0x03,0x0010,0x001C},
        {0x01,0x0002,0x0003},
        {0x01,0x0500,0x0F00},
        {0x02,0x1000,0x3000},
        {0x02,0x0300,0x0F00},
        {0x35,0x4000,0x7000},
        {0x2E,0x0004,0x000F},
        {0x37,0x0004,0x0004},
        {0x37,0x4000,0x4000},
        {0x03,0x0000,0x0200},
        {0x2E,0x0040,0x0040},
        {0x35,0x0F00,0x0F00},
        {0x2E,0x0020,0x0020},
        {0x37,0x0000,0x0002},
        {0x2C,0x0000,0x8000},
        {0x2E,0x0010,0x0010},
    },
#elif PANEL_VBY1_4CH_10Bit
    { //E_PNL_SUPPORTED_LPLL_VBY1_4ch_10bit_1P_150to300MHz    NO.13
      //Address,Value,Mask
        {0x03,0x0000,0x0020},
        {0x03,0x0003,0x0003},
        {0x03,0x0010,0x001C},
        {0x01,0x0002,0x0003},
        {0x01,0x0500,0x0F00},
        {0x02,0x2000,0x3000},
        {0x02,0x0000,0x0F00},
        {0x35,0x4000,0x7000},
        {0x2E,0x0004,0x000F},
        {0x37,0x0004,0x0004},
        {0x37,0x4000,0x4000},
        {0x03,0x0000,0x0200},
        {0x2E,0x0040,0x0040},
        {0x35,0x0F00,0x0F00},
        {0x2E,0x0020,0x0020},
        {0x37,0x0000,0x0002},
        {0x2C,0x0000,0x8000},
        {0x2E,0x0010,0x0010},
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_4ch_10bit_2P_75to150MHz    NO.14
      //Address,Value,Mask
        {0x03,0x0000,0x0020},
        {0x03,0x0003,0x0003},
        {0x03,0x0010,0x001C},
        {0x01,0x0002,0x0003},
        {0x01,0x0500,0x0F00},
        {0x02,0x2000,0x3000},
        {0x02,0x0200,0x0F00},
        {0x35,0x4000,0x7000},
        {0x2E,0x0004,0x000F},
        {0x37,0x0004,0x0004},
        {0x37,0x4000,0x4000},
        {0x03,0x0000,0x0200},
        {0x2E,0x0040,0x0040},
        {0x35,0x0F00,0x0F00},
        {0x2E,0x0020,0x0020},
        {0x37,0x0000,0x0002},
        {0x2C,0x0000,0x8000},
        {0x2E,0x0010,0x0010},
    },
#elif PANEL_VBY1_4CH_8Bit
    { //E_PNL_SUPPORTED_LPLL_VBY1_4ch_8bit_1P_150to300MHz    NO.15
      //Address,Value,Mask
        {0x03,0x0000,0x0020},
        {0x03,0x0003,0x0003},
        {0x03,0x0010,0x001C},
        {0x01,0x0002,0x0003},
        {0x01,0x0500,0x0F00},
        {0x02,0x0000,0x3000},
        {0x02,0x0300,0x0F00},
        {0x35,0x4000,0x7000},
        {0x2E,0x0004,0x000F},
        {0x37,0x0004,0x0004},
        {0x37,0x4000,0x4000},
        {0x03,0x0000,0x0200},
        {0x2E,0x0040,0x0040},
        {0x35,0x0F00,0x0F00},
        {0x2E,0x0020,0x0020},
        {0x37,0x0000,0x0002},
        {0x2C,0x0000,0x8000},
        {0x2E,0x0010,0x0010},
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_4ch_8bit_2P_75to150MHz    NO.16
      //Address,Value,Mask
        {0x03,0x0000,0x0020},
        {0x03,0x0003,0x0003},
        {0x03,0x0010,0x001C},
        {0x01,0x0002,0x0003},
        {0x01,0x0500,0x0F00},
        {0x02,0x1000,0x3000},
        {0x02,0x0300,0x0F00},
        {0x35,0x4000,0x7000},
        {0x2E,0x0004,0x000F},
        {0x37,0x0004,0x0004},
        {0x37,0x4000,0x4000},
        {0x03,0x0000,0x0200},
        {0x2E,0x0040,0x0040},
        {0x35,0x0F00,0x0F00},
        {0x2E,0x0020,0x0020},
        {0x37,0x0000,0x0002},
        {0x2C,0x0000,0x8000},
        {0x2E,0x0010,0x0010},
    },
#elif PANEL_VBY1_8CH_10Bit
    { //E_PNL_SUPPORTED_LPLL_VBY1_8ch_10bit_2P_150to300MHz    NO.17
      //Address,Value,Mask
        {0x03,0x0000,0x0020},
        {0x03,0x0003,0x0003},
        {0x03,0x0010,0x001C},
        {0x01,0x0002,0x0003},
        {0x01,0x0500,0x0F00},
        {0x02,0x2000,0x3000},
        {0x02,0x0000,0x0F00},
        {0x35,0x4000,0x7000},
        {0x2E,0x0004,0x000F},
        {0x37,0x0004,0x0004},
        {0x37,0x4000,0x4000},
        {0x03,0x0000,0x0200},
        {0x2E,0x0040,0x0040},
        {0x35,0x0F00,0x0F00},
        {0x2E,0x0020,0x0020},
        {0x37,0x0000,0x0002},
        {0x2C,0x0000,0x8000},
        {0x2E,0x0010,0x0010},
    },
#elif PANEL_VBY1_8CH_8Bit
    { //E_PNL_SUPPORTED_LPLL_VBY1_8ch_8bit_2P_150to300MHz    NO.18
      //Address,Value,Mask
        {0x03,0x0000,0x0020},
        {0x03,0x0003,0x0003},
        {0x03,0x0010,0x001C},
        {0x01,0x0002,0x0003},
        {0x01,0x0500,0x0F00},
        {0x02,0x0000,0x3000},
        {0x02,0x0300,0x0F00},
        {0x35,0x4000,0x7000},
        {0x2E,0x0004,0x000F},
        {0x37,0x0004,0x0004},
        {0x37,0x4000,0x4000},
        {0x03,0x0000,0x0200},
        {0x2E,0x0040,0x0040},
        {0x35,0x0F00,0x0F00},
        {0x2E,0x0020,0x0020},
        {0x37,0x0000,0x0002},
        {0x2C,0x0000,0x8000},
        {0x2E,0x0010,0x0010},
    },
#elif PANEL_EDP
    { //E_PNL_SUPPORTED_LPLL_eDP_High_bit_rate_150to300MHz    NO.19
      //Address,Value,Mask
        {0x03,0x0000,0x0020},
        {0x03,0x0002,0x0003},
        {0x03,0x0010,0x001C},
        {0x01,0x0003,0x0003},
        {0x01,0x0200,0x0F00},
        {0x02,0x0000,0x3000},
        {0x02,0x0700,0x0F00},
        {0x35,0x0000,0x7000},
        {0x2E,0x0000,0x000F},
        {0x37,0x0000,0x0004},
        {0x37,0x0000,0x4000},
        {0x03,0x0000,0x0200},
        {0x2E,0x0000,0x0040},
        {0x35,0x0F00,0x0F00},
        {0x2E,0x0020,0x0020},
        {0x37,0x0002,0x0002},
        {0x2C,0x0000,0x8000},
        {0x2E,0x0010,0x0010},
    },

    { //E_PNL_SUPPORTED_LPLL_eDP_High_bit_rate_75to150MHz    NO.20
      //Address,Value,Mask
        {0x03,0x0000,0x0020},
        {0x03,0x0002,0x0003},
        {0x03,0x0010,0x001C},
        {0x01,0x0003,0x0003},
        {0x01,0x0200,0x0F00},
        {0x02,0x1000,0x3000},
        {0x02,0x0700,0x0F00},
        {0x35,0x0000,0x7000},
        {0x2E,0x0000,0x000F},
        {0x37,0x0000,0x0004},
        {0x37,0x0000,0x4000},
        {0x03,0x0000,0x0200},
        {0x2E,0x0000,0x0040},
        {0x35,0x0F00,0x0F00},
        {0x2E,0x0020,0x0020},
        {0x37,0x0002,0x0002},
        {0x2C,0x0000,0x8000},
        {0x2E,0x0010,0x0010},
    },

    { //E_PNL_SUPPORTED_LPLL_eDP_High_bit_rate_37_5to75MHz    NO.21
      //Address,Value,Mask
        {0x03,0x0000,0x0020},
        {0x03,0x0002,0x0003},
        {0x03,0x0010,0x001C},
        {0x01,0x0003,0x0003},
        {0x01,0x0200,0x0F00},
        {0x02,0x2000,0x3000},
        {0x02,0x0700,0x0F00},
        {0x35,0x0000,0x7000},
        {0x2E,0x0000,0x000F},
        {0x37,0x0000,0x0004},
        {0x37,0x0000,0x4000},
        {0x03,0x0000,0x0200},
        {0x2E,0x0000,0x0040},
        {0x35,0x0F00,0x0F00},
        {0x2E,0x0020,0x0020},
        {0x37,0x0002,0x0002},
        {0x2C,0x0000,0x8000},
        {0x2E,0x0010,0x0010},
    },

    { //E_PNL_SUPPORTED_LPLL_eDP_High_bit_rate_20to37_5MHz    NO.22
      //Address,Value,Mask
        {0x03,0x0000,0x0020},
        {0x03,0x0002,0x0003},
        {0x03,0x0010,0x001C},
        {0x01,0x0003,0x0003},
        {0x01,0x0200,0x0F00},
        {0x02,0x3000,0x3000},
        {0x02,0x0700,0x0F00},
        {0x35,0x0000,0x7000},
        {0x2E,0x0000,0x000F},
        {0x37,0x0000,0x0004},
        {0x37,0x0000,0x4000},
        {0x03,0x0000,0x0200},
        {0x2E,0x0000,0x0040},
        {0x35,0x0F00,0x0F00},
        {0x2E,0x0020,0x0020},
        {0x37,0x0002,0x0002},
        {0x2C,0x0000,0x8000},
        {0x2E,0x0010,0x0010},
    },

    { //E_PNL_SUPPORTED_LPLL_eDP_High_bit_rate_12_5to20MHz    NO.23
      //Address,Value,Mask
        {0x03,0x0000,0x0020},
        {0x03,0x0002,0x0003},
        {0x03,0x0010,0x001C},
        {0x01,0x0003,0x0003},
        {0x01,0x0200,0x0F00},
        {0x02,0x3000,0x3000},
        {0x02,0x0E00,0x0F00},
        {0x35,0x0000,0x7000},
        {0x2E,0x0000,0x000F},
        {0x37,0x0000,0x0004},
        {0x37,0x0000,0x4000},
        {0x03,0x0000,0x0200},
        {0x2E,0x0000,0x0040},
        {0x35,0x0F00,0x0F00},
        {0x2E,0x0020,0x0020},
        {0x37,0x0002,0x0002},
        {0x2C,0x0000,0x8000},
        {0x2E,0x0010,0x0010},
    },

    { //E_PNL_SUPPORTED_LPLL_eDP_Reduce_bit_rate_150to300MHz    NO.24
      //Address,Value,Mask
        {0x03,0x0000,0x0020},
        {0x03,0x0002,0x0003},
        {0x03,0x0010,0x001C},
        {0x01,0x0003,0x0003},
        {0x01,0x0200,0x0F00},
        {0x02,0x0000,0x3000},
        {0x02,0x0700,0x0F00},
        {0x35,0x0000,0x7000},
        {0x2E,0x0000,0x000F},
        {0x37,0x0000,0x0004},
        {0x37,0x0000,0x4000},
        {0x03,0x0000,0x0200},
        {0x2E,0x0000,0x0040},
        {0x35,0x0F00,0x0F00},
        {0x2E,0x0020,0x0020},
        {0x37,0x0002,0x0002},
        {0x2C,0x0000,0x8000},
        {0x2E,0x0010,0x0010},
    },

    { //E_PNL_SUPPORTED_LPLL_eDP_Reduce_bit_rate_75to150MHz    NO.25
      //Address,Value,Mask
        {0x03,0x0000,0x0020},
        {0x03,0x0002,0x0003},
        {0x03,0x0010,0x001C},
        {0x01,0x0003,0x0003},
        {0x01,0x0200,0x0F00},
        {0x02,0x1000,0x3000},
        {0x02,0x0700,0x0F00},
        {0x35,0x0000,0x7000},
        {0x2E,0x0000,0x000F},
        {0x37,0x0000,0x0004},
        {0x37,0x0000,0x4000},
        {0x03,0x0000,0x0200},
        {0x2E,0x0000,0x0040},
        {0x35,0x0F00,0x0F00},
        {0x2E,0x0020,0x0020},
        {0x37,0x0002,0x0002},
        {0x2C,0x0000,0x8000},
        {0x2E,0x0010,0x0010},
    },

    { //E_PNL_SUPPORTED_LPLL_eDP_Reduce_bit_rate_37_5to75MHz    NO.26
      //Address,Value,Mask
        {0x03,0x0000,0x0020},
        {0x03,0x0002,0x0003},
        {0x03,0x0010,0x001C},
        {0x01,0x0003,0x0003},
        {0x01,0x0200,0x0F00},
        {0x02,0x2000,0x3000},
        {0x02,0x0700,0x0F00},
        {0x35,0x0000,0x7000},
        {0x2E,0x0000,0x000F},
        {0x37,0x0000,0x0004},
        {0x37,0x0000,0x4000},
        {0x03,0x0000,0x0200},
        {0x2E,0x0000,0x0040},
        {0x35,0x0F00,0x0F00},
        {0x2E,0x0020,0x0020},
        {0x37,0x0002,0x0002},
        {0x2C,0x0000,0x8000},
        {0x2E,0x0010,0x0010},
    },

    { //E_PNL_SUPPORTED_LPLL_eDP_Reduce_bit_rate_20to37_5MHz    NO.27
      //Address,Value,Mask
        {0x03,0x0000,0x0020},
        {0x03,0x0002,0x0003},
        {0x03,0x0010,0x001C},
        {0x01,0x0003,0x0003},
        {0x01,0x0200,0x0F00},
        {0x02,0x3000,0x3000},
        {0x02,0x0700,0x0F00},
        {0x35,0x0000,0x7000},
        {0x2E,0x0000,0x000F},
        {0x37,0x0000,0x0004},
        {0x37,0x0000,0x4000},
        {0x03,0x0000,0x0200},
        {0x2E,0x0000,0x0040},
        {0x35,0x0F00,0x0F00},
        {0x2E,0x0020,0x0020},
        {0x37,0x0002,0x0002},
        {0x2C,0x0000,0x8000},
        {0x2E,0x0010,0x0010},
    },

    { //E_PNL_SUPPORTED_LPLL_eDP_Reduce_bit_rate_12_5to20MHz    NO.28
      //Address,Value,Mask
        {0x03,0x0000,0x0020},
        {0x03,0x0002,0x0003},
        {0x03,0x0010,0x001C},
        {0x01,0x0003,0x0003},
        {0x01,0x0200,0x0F00},
        {0x02,0x3000,0x3000},
        {0x02,0x0E00,0x0F00},
        {0x35,0x0000,0x7000},
        {0x2E,0x0000,0x000F},
        {0x37,0x0000,0x0004},
        {0x37,0x0000,0x4000},
        {0x03,0x0000,0x0200},
        {0x2E,0x0000,0x0040},
        {0x35,0x0F00,0x0F00},
        {0x2E,0x0020,0x0020},
        {0x37,0x0002,0x0002},
        {0x2C,0x0000,0x8000},
        {0x2E,0x0010,0x0010},
    },
#endif
};
#endif

#if CHIP_ID == MST9U4
#define MFT_REG_ADDRESS(u32Address)						((u32Address&0xFFFF00)+(u32Address&0x0000FF)*2)

#define MFT_REG_NUM    33
typedef struct
{
    MS_U32  address;
    MS_U16 value;
    MS_U16 mask;
}TBLStruct2,*pTBLStruct2;

typedef enum
{
#if PANEL_LVDS_2CH
    E_PNL_SUPPORTED_MFT_LVDS_2CH_FHD60_OE_50to75MHz,          //0
#elif PANEL_LVDS_4CH
    E_PNL_SUPPORTED_MFT_LVDS_4CH_FHD120_OE_50to75MHz,          //1
    E_PNL_SUPPORTED_MFT_LVDS_4CH_FHD120_LR_50to75MHz,          //2
#elif PANEL_VBY1_1CH_8Bit||PANEL_VBY1_1CH_10Bit
#elif PANEL_VBY1_2CH_8Bit||PANEL_VBY1_2CH_10Bit
    E_PNL_SUPPORTED_MFT_VBY1_2LANE_1BLK_FHD60_4P_37_5to75MHz,          //3
#elif PANEL_VBY1_4CH_8Bit||PANEL_VBY1_4CH_10Bit
    E_PNL_SUPPORTED_MFT_VBY1_4LANE_1BLK_FHD120_4P_37_5to75MHz,          //4
    E_PNL_SUPPORTED_MFT_VBY1_4LANE_2BLK_FHD120_4P_37_5to75MHz,          //5
#elif PANEL_VBY1_8CH_8Bit||PANEL_VBY1_8CH_10Bit
    E_PNL_SUPPORTED_MFT_VBY1_8LANE_1BLK_UHD60_4P_75to150MHz,          //6
    E_PNL_SUPPORTED_MFT_VBY1_8LANE_2BLK_UHD60_4P_75to150MHz,          //7
    E_PNL_SUPPORTED_MFT_VBY1_8LANE_4BLK_UHD60_4P_75to150MHz,          //8
#elif PANEL_VBY1_16CH_8Bit||PANEL_VBY1_16CH_10Bit
    E_PNL_SUPPORTED_MFT_VBY1_16LANE_1BLK_UHD120_4P_150to300MHz,          //9
    E_PNL_SUPPORTED_MFT_VBY1_16LANE_2BLK_UHD120_4P_150to300MHz,          //10
    E_PNL_SUPPORTED_MFT_VBY1_16LANE_4BLK_UHD120_4P_150to300MHz,          //11
    E_PNL_SUPPORTED_MFT_VBY1_16LANE_8BLK_UHD120_4P_150to300MHz,          //12

    E_PNL_SUPPORTED_MFT_VBY1_16LANE_1BLK_5K120_8P_135to195_75MHz,          //13
    E_PNL_SUPPORTED_MFT_VBY1_16LANE_2BLK_5K120_8P_135to195_75MHz,          //14
    E_PNL_SUPPORTED_MFT_VBY1_16LANE_4BLK_5K120_8P_135to195_75MHz,          //15
    E_PNL_SUPPORTED_MFT_VBY1_16LANE_8BLK_5K120_8P_135to195_75MHz,          //16
#elif PANEL_VBY1_20CH_8Bit||PANEL_VBY1_20CH_10Bit
    E_PNL_SUPPORTED_MFT_VBY1_20LANE_1BLK_5K120_8P_195_75to391_5MHz,          //17
    E_PNL_SUPPORTED_MFT_VBY1_20LANE_5BLK_5K120_8P_195_75to391_5MHz,          //18
    E_PNL_SUPPORTED_MFT_VBY1_20LANE_10BLK_5K120_8P_195_75to391_5MHz,          //19
#elif PANEL_VBY1_32CH_8Bit||PANEL_VBY1_32CH_10Bit
    E_PNL_SUPPORTED_MFT_VBY1_32LANE_1BLK_8K60_8P_225to300MHz,          //20
    E_PNL_SUPPORTED_MFT_VBY1_32LANE_2BLK_8K60_8P_225to300MHz,          //21
    E_PNL_SUPPORTED_MFT_VBY1_32LANE_4BLK_8K60_8P_225to300MHz,          //22
    E_PNL_SUPPORTED_MFT_VBY1_32LANE_8BLK_8K60_8P_225to300MHz,          //23
#else

#endif
    E_PNL_SUPPORTED_MFT_MAX,          //24
} E_PNL_SUPPORTED_MFT_TYPE;

TBLStruct2 MFTSettingTBL[E_PNL_SUPPORTED_MFT_MAX][MFT_REG_NUM]=
{
#if PANEL_LVDS_2CH
    { //E_PNL_SUPPORTED_MFT_LVDS_2CH_FHD60_OE_50to75MHz    NO.0
      //Address,Value,Mask
        {0x111211,0x0004,0xFFFF},//
        {0x111420,0x0000,0x0001},//reg_control_01
        {0x111421,0x3210,0xFFFF},//reg_control_02
        {0x111422,0x7654,0xFFFF},//reg_control_03
        {0x111402,0x0000,0xFFFF},//reg_control_04
        {0x111401,0x0102,0xFFFF},//reg_control_05
        {0x111410,0x0F00,0xFFFF},//reg_control_06
        {0x111411,0x0780,0xFFFF},//reg_control_07
        {0x111412,0x0000,0xFFFF},//reg_control_08
        {0x111413,0x3FFF,0xFFFF},//reg_control_09
        {0x111414,0x3FFF,0xFFFF},//reg_control_10
        {0x111415,0x3FFF,0xFFFF},//reg_control_11
        {0x111416,0x3FFF,0xFFFF},//reg_control_12
        {0x111417,0x3FFF,0xFFFF},//reg_control_13
        {0x111418,0x3FFF,0xFFFF},//reg_control_14
        {0x111419,0x3FFF,0xFFFF},//reg_control_15
        {0x111440,0x0000,0xFFFF},//reg_control_16
        {0x111655,0x0100,0xFFFF},//reg_control_17
        {0x111656,0x0302,0xFFFF},//reg_free_swap_1_0
        {0x111657,0x0504,0xFFFF},//reg_free_swap_3_2
        {0x111658,0x0706,0xFFFF},//reg_free_swap_5_4
        {0x111659,0x0908,0xFFFF},//reg_free_swap_7_6
        {0x11165A,0x0B0A,0xFFFF},//reg_free_swap_9_8
        {0x11165B,0x0D0C,0xFFFF},//reg_free_swap_10_11
        {0x11165C,0x0F0E,0xFFFF},//reg_free_swap_13_12
        {0x11165D,0x1110,0xFFFF},//reg_free_swap_15_14
        {0x11165E,0x1312,0xFFFF},//reg_free_swap_17_16
        {0x111672,0x1514,0xFFFF},//reg_free_swap_19_18
        {0x111673,0x1716,0xFFFF},//reg_free_swap_21_20
        {0x111674,0x1918,0xFFFF},//reg_free_swap_23_22
        {0x111675,0x1B1A,0xFFFF},//reg_free_swap_25_24
        {0x111676,0x1D1C,0xFFFF},//reg_free_swap_27_26
        {0x111677,0x1F1E,0xFFFF},//reg_free_swap_29_28
    },
#elif PANEL_LVDS_4CH
    { //E_PNL_SUPPORTED_MFT_LVDS_4CH_FHD120_OE_50to75MHz    NO.1
      //Address,Value,Mask
        {0x111211,0x0000,0xFFFF},//
        {0x111420,0x0000,0x0001},//reg_control_01
        {0x111421,0x3210,0xFFFF},//reg_control_02
        {0x111422,0x7654,0xFFFF},//reg_control_03
        {0x111402,0x0000,0xFFFF},//reg_control_04
        {0x111401,0x0002,0xFFFF},//reg_control_05
        {0x111410,0x0F00,0xFFFF},//reg_control_06
        {0x111411,0x0780,0xFFFF},//reg_control_07
        {0x111412,0x0000,0xFFFF},//reg_control_08
        {0x111413,0x3FFF,0xFFFF},//reg_control_09
        {0x111414,0x3FFF,0xFFFF},//reg_control_10
        {0x111415,0x3FFF,0xFFFF},//reg_control_11
        {0x111416,0x3FFF,0xFFFF},//reg_control_12
        {0x111417,0x3FFF,0xFFFF},//reg_control_13
        {0x111418,0x3FFF,0xFFFF},//reg_control_14
        {0x111419,0x3FFF,0xFFFF},//reg_control_15
        {0x111440,0x0000,0xFFFF},//reg_control_16
        {0x111655,0x0100,0xFFFF},//reg_control_17
        {0x111656,0x0302,0xFFFF},//reg_free_swap_1_0
        {0x111657,0x0504,0xFFFF},//reg_free_swap_3_2
        {0x111658,0x0706,0xFFFF},//reg_free_swap_5_4
        {0x111659,0x0908,0xFFFF},//reg_free_swap_7_6
        {0x11165A,0x0B0A,0xFFFF},//reg_free_swap_9_8
        {0x11165B,0x0D0C,0xFFFF},//reg_free_swap_10_11
        {0x11165C,0x0F0E,0xFFFF},//reg_free_swap_13_12
        {0x11165D,0x1110,0xFFFF},//reg_free_swap_15_14
        {0x11165E,0x1312,0xFFFF},//reg_free_swap_17_16
        {0x111672,0x1514,0xFFFF},//reg_free_swap_19_18
        {0x111673,0x1716,0xFFFF},//reg_free_swap_21_20
        {0x111674,0x1918,0xFFFF},//reg_free_swap_23_22
        {0x111675,0x1B1A,0xFFFF},//reg_free_swap_25_24
        {0x111676,0x1D1C,0xFFFF},//reg_free_swap_27_26
        {0x111677,0x1F1E,0xFFFF},//reg_free_swap_29_28
    },
    { //E_PNL_SUPPORTED_MFT_LVDS_4CH_FHD120_LR_50to75MHz    NO.2
      //Address,Value,Mask
        {0x111211,0x0000,0xFFFF},//
        {0x111420,0x0000,0x0001},//reg_control_01
        {0x111421,0x3210,0xFFFF},//reg_control_02
        {0x111422,0x7654,0xFFFF},//reg_control_03
        {0x111402,0x0000,0xFFFF},//reg_control_04
        {0x111401,0x0003,0xFFFF},//reg_control_05
        {0x111410,0x0F00,0xFFFF},//reg_control_06
        {0x111411,0x03C0,0xFFFF},//reg_control_07
        {0x111412,0x0000,0xFFFF},//reg_control_08
        {0x111413,0x0780,0xFFFF},//reg_control_09
        {0x111414,0x3FFF,0xFFFF},//reg_control_10
        {0x111415,0x3FFF,0xFFFF},//reg_control_11
        {0x111416,0x3FFF,0xFFFF},//reg_control_12
        {0x111417,0x3FFF,0xFFFF},//reg_control_13
        {0x111418,0x3FFF,0xFFFF},//reg_control_14
        {0x111419,0x3FFF,0xFFFF},//reg_control_15
        {0x111440,0x0000,0xFFFF},//reg_control_16
        {0x111655,0x0100,0xFFFF},//reg_control_17
        {0x111656,0x0302,0xFFFF},//reg_free_swap_1_0
        {0x111657,0x0504,0xFFFF},//reg_free_swap_3_2
        {0x111658,0x0706,0xFFFF},//reg_free_swap_5_4
        {0x111659,0x0908,0xFFFF},//reg_free_swap_7_6
        {0x11165A,0x0B0A,0xFFFF},//reg_free_swap_9_8
        {0x11165B,0x0D0C,0xFFFF},//reg_free_swap_10_11
        {0x11165C,0x0F0E,0xFFFF},//reg_free_swap_13_12
        {0x11165D,0x1110,0xFFFF},//reg_free_swap_15_14
        {0x11165E,0x1312,0xFFFF},//reg_free_swap_17_16
        {0x111672,0x1514,0xFFFF},//reg_free_swap_19_18
        {0x111673,0x1716,0xFFFF},//reg_free_swap_21_20
        {0x111674,0x1918,0xFFFF},//reg_free_swap_23_22
        {0x111675,0x1B1A,0xFFFF},//reg_free_swap_25_24
        {0x111676,0x1D1C,0xFFFF},//reg_free_swap_27_26
        {0x111677,0x1F1E,0xFFFF},//reg_free_swap_29_28
    },
#elif PANEL_VBY1_2CH_8Bit||PANEL_VBY1_2CH_10Bit
    { //E_PNL_SUPPORTED_MFT_VBY1_2LANE_1BLK_FHD60_4P_37.5to75MHz    NO.3
      //Address,Value,Mask
        {0x111211,0x0004,0xFFFF},//
        {0x111420,0x0000,0x0001},//reg_control_01
        {0x111421,0x3210,0xFFFF},//reg_control_02
        {0x111422,0x7654,0xFFFF},//reg_control_03
        {0x111402,0x0000,0xFFFF},//reg_control_04
        {0x111401,0x0102,0xFFFF},//reg_control_05
        {0x111410,0x0F00,0xFFFF},//reg_control_06
        {0x111411,0x0780,0xFFFF},//reg_control_07
        {0x111412,0x0000,0xFFFF},//reg_control_08
        {0x111413,0x3FFF,0xFFFF},//reg_control_09
        {0x111414,0x3FFF,0xFFFF},//reg_control_10
        {0x111415,0x3FFF,0xFFFF},//reg_control_11
        {0x111416,0x3FFF,0xFFFF},//reg_control_12
        {0x111417,0x3FFF,0xFFFF},//reg_control_13
        {0x111418,0x3FFF,0xFFFF},//reg_control_14
        {0x111419,0x3FFF,0xFFFF},//reg_control_15
        {0x111440,0x0000,0xFFFF},//reg_control_16
        {0x111655,0x0200,0xFFFF},//reg_control_17
        {0x111656,0x1F1F,0xFFFF},//reg_free_swap_1_0
        {0x111657,0x1F1F,0xFFFF},//reg_free_swap_3_2
        {0x111658,0x1F1F,0xFFFF},//reg_free_swap_5_4
        {0x111659,0x1F1F,0xFFFF},//reg_free_swap_7_6
        {0x11165A,0x1F1F,0xFFFF},//reg_free_swap_9_8
        {0x11165B,0x1F1F,0xFFFF},//reg_free_swap_10_11
        {0x11165C,0x1F1F,0xFFFF},//reg_free_swap_13_12
        {0x11165D,0x1F1F,0xFFFF},//reg_free_swap_15_14
        {0x11165E,0x1F1F,0xFFFF},//reg_free_swap_17_16
        {0x111672,0x1F1F,0xFFFF},//reg_free_swap_19_18
        {0x111673,0x1F1F,0xFFFF},//reg_free_swap_21_20
        {0x111674,0x1F1F,0xFFFF},//reg_free_swap_23_22
        {0x111675,0x1F1F,0xFFFF},//reg_free_swap_25_24
        {0x111676,0x1F1F,0xFFFF},//reg_free_swap_27_26
        {0x111677,0x1F1F,0xFFFF},//reg_free_swap_29_28
    },
#elif PANEL_VBY1_4CH_8Bit||PANEL_VBY1_4CH_10Bit
    { //E_PNL_SUPPORTED_MFT_VBY1_4LANE_1BLK_FHD120_4P_37.5to75MHz    NO.4
      //Address,Value,Mask
        {0x111211,0x0000,0xFFFF},//
        {0x111420,0x0000,0x0001},//reg_control_01
        {0x111421,0x3210,0xFFFF},//reg_control_02
        {0x111422,0x7654,0xFFFF},//reg_control_03
        {0x111402,0x0000,0xFFFF},//reg_control_04
        {0x111401,0x0002,0xFFFF},//reg_control_05
        {0x111410,0x0F00,0xFFFF},//reg_control_06
        {0x111411,0x0780,0xFFFF},//reg_control_07
        {0x111412,0x0000,0xFFFF},//reg_control_08
        {0x111413,0x3FFF,0xFFFF},//reg_control_09
        {0x111414,0x3FFF,0xFFFF},//reg_control_10
        {0x111415,0x3FFF,0xFFFF},//reg_control_11
        {0x111416,0x3FFF,0xFFFF},//reg_control_12
        {0x111417,0x3FFF,0xFFFF},//reg_control_13
        {0x111418,0x3FFF,0xFFFF},//reg_control_14
        {0x111419,0x3FFF,0xFFFF},//reg_control_15
        {0x111440,0x0000,0xFFFF},//reg_control_16
        {0x111655,0x0200,0xFFFF},//reg_control_17
        {0x111656,0x0604,0xFFFF},//reg_free_swap_1_0
        {0x111657,0x1F1F,0xFFFF},//reg_free_swap_3_2
        {0x111658,0x1F1F,0xFFFF},//reg_free_swap_5_4
        {0x111659,0x1F1F,0xFFFF},//reg_free_swap_7_6
        {0x11165A,0x1F1F,0xFFFF},//reg_free_swap_9_8
        {0x11165B,0x1F1F,0xFFFF},//reg_free_swap_10_11
        {0x11165C,0x1F1F,0xFFFF},//reg_free_swap_13_12
        {0x11165D,0x1F1F,0xFFFF},//reg_free_swap_15_14
        {0x11165E,0x1F1F,0xFFFF},//reg_free_swap_17_16
        {0x111672,0x1F1F,0xFFFF},//reg_free_swap_19_18
        {0x111673,0x1F1F,0xFFFF},//reg_free_swap_21_20
        {0x111674,0x1F1F,0xFFFF},//reg_free_swap_23_22
        {0x111675,0x1F1F,0xFFFF},//reg_free_swap_25_24
        {0x111676,0x1F1F,0xFFFF},//reg_free_swap_27_26
        {0x111677,0x1F1F,0xFFFF},//reg_free_swap_29_28
    },
    { //E_PNL_SUPPORTED_MFT_VBY1_4LANE_2BLK_FHD120_4P_37.5to75MHz    NO.5
      //Address,Value,Mask
        {0x111211,0x0000,0xFFFF},//
        {0x111420,0x0000,0x0001},//reg_control_01
        {0x111421,0x3210,0xFFFF},//reg_control_02
        {0x111422,0x7654,0xFFFF},//reg_control_03
        {0x111402,0x0000,0xFFFF},//reg_control_04
        {0x111401,0x0003,0xFFFF},//reg_control_05
        {0x111410,0x0F00,0xFFFF},//reg_control_06
        {0x111411,0x03C0,0xFFFF},//reg_control_07
        {0x111412,0x0000,0xFFFF},//reg_control_08
        {0x111413,0x0780,0xFFFF},//reg_control_09
        {0x111414,0x3FFF,0xFFFF},//reg_control_10
        {0x111415,0x3FFF,0xFFFF},//reg_control_11
        {0x111416,0x3FFF,0xFFFF},//reg_control_12
        {0x111417,0x3FFF,0xFFFF},//reg_control_13
        {0x111418,0x3FFF,0xFFFF},//reg_control_14
        {0x111419,0x3FFF,0xFFFF},//reg_control_15
        {0x111440,0x0000,0xFFFF},//reg_control_16
        {0x111655,0x0200,0xFFFF},//reg_control_17
        {0x111656,0x0604,0xFFFF},//reg_free_swap_1_0
        {0x111657,0x1F1F,0xFFFF},//reg_free_swap_3_2
        {0x111658,0x1F1F,0xFFFF},//reg_free_swap_5_4
        {0x111659,0x1F1F,0xFFFF},//reg_free_swap_7_6
        {0x11165A,0x1F1F,0xFFFF},//reg_free_swap_9_8
        {0x11165B,0x1F1F,0xFFFF},//reg_free_swap_10_11
        {0x11165C,0x1F1F,0xFFFF},//reg_free_swap_13_12
        {0x11165D,0x1F1F,0xFFFF},//reg_free_swap_15_14
        {0x11165E,0x1F1F,0xFFFF},//reg_free_swap_17_16
        {0x111672,0x1F1F,0xFFFF},//reg_free_swap_19_18
        {0x111673,0x1F1F,0xFFFF},//reg_free_swap_21_20
        {0x111674,0x1F1F,0xFFFF},//reg_free_swap_23_22
        {0x111675,0x1F1F,0xFFFF},//reg_free_swap_25_24
        {0x111676,0x1F1F,0xFFFF},//reg_free_swap_27_26
        {0x111677,0x1F1F,0xFFFF},//reg_free_swap_29_28
    },
#elif PANEL_VBY1_8CH_8Bit||PANEL_VBY1_8CH_10Bit
    { //E_PNL_SUPPORTED_MFT_VBY1_8LANE_1BLK_UHD60_4P_75to150MHz    NO.6
      //Address,Value,Mask
        {0x111211,0x0000,0xFFFF},//
        {0x111420,0x0000,0x0001},//reg_control_01
        {0x111421,0x3210,0xFFFF},//reg_control_02
        {0x111422,0x7654,0xFFFF},//reg_control_03
        {0x111402,0x0000,0xFFFF},//reg_control_04
        {0x111401,0x0002,0xFFFF},//reg_control_05
        {0x111410,0x1E00,0xFFFF},//reg_control_06
        {0x111411,0x0F00,0xFFFF},//reg_control_07
        {0x111412,0x0000,0xFFFF},//reg_control_08
        {0x111413,0x3FFF,0xFFFF},//reg_control_09
        {0x111414,0x3FFF,0xFFFF},//reg_control_10
        {0x111415,0x3FFF,0xFFFF},//reg_control_11
        {0x111416,0x3FFF,0xFFFF},//reg_control_12
        {0x111417,0x3FFF,0xFFFF},//reg_control_13
        {0x111418,0x3FFF,0xFFFF},//reg_control_14
        {0x111419,0x3FFF,0xFFFF},//reg_control_15
        {0x111440,0x0000,0xFFFF},//reg_control_16
        {0x111655,0x0200,0xFFFF},//reg_control_17
        {0x111656,0x0604,0xFFFF},//reg_free_swap_1_0
        {0x111657,0x0301,0xFFFF},//reg_free_swap_3_2
        {0x111658,0x0705,0xFFFF},//reg_free_swap_5_4
        {0x111659,0x1F1F,0xFFFF},//reg_free_swap_7_6
        {0x11165A,0x1F1F,0xFFFF},//reg_free_swap_9_8
        {0x11165B,0x1F1F,0xFFFF},//reg_free_swap_10_11
        {0x11165C,0x1F1F,0xFFFF},//reg_free_swap_13_12
        {0x11165D,0x1F1F,0xFFFF},//reg_free_swap_15_14
        {0x11165E,0x1F1F,0xFFFF},//reg_free_swap_17_16
        {0x111672,0x1F1F,0xFFFF},//reg_free_swap_19_18
        {0x111673,0x1F1F,0xFFFF},//reg_free_swap_21_20
        {0x111674,0x1F1F,0xFFFF},//reg_free_swap_23_22
        {0x111675,0x1F1F,0xFFFF},//reg_free_swap_25_24
        {0x111676,0x1F1F,0xFFFF},//reg_free_swap_27_26
        {0x111677,0x1F1F,0xFFFF},//reg_free_swap_29_28
    },
    { //E_PNL_SUPPORTED_MFT_VBY1_8LANE_2BLK_UHD60_4P_75to150MHz    NO.7
      //Address,Value,Mask
        {0x111211,0x0000,0xFFFF},//
        {0x111420,0x0000,0x0001},//reg_control_01
        {0x111421,0x3210,0xFFFF},//reg_control_02
        {0x111422,0x7654,0xFFFF},//reg_control_03
        {0x111402,0x0000,0xFFFF},//reg_control_04
        {0x111401,0x0003,0xFFFF},//reg_control_05
        {0x111410,0x1E00,0xFFFF},//reg_control_06
        {0x111411,0x0780,0xFFFF},//reg_control_07
        {0x111412,0x0000,0xFFFF},//reg_control_08
        {0x111413,0x0F00,0xFFFF},//reg_control_09
        {0x111414,0x3FFF,0xFFFF},//reg_control_10
        {0x111415,0x3FFF,0xFFFF},//reg_control_11
        {0x111416,0x3FFF,0xFFFF},//reg_control_12
        {0x111417,0x3FFF,0xFFFF},//reg_control_13
        {0x111418,0x3FFF,0xFFFF},//reg_control_14
        {0x111419,0x3FFF,0xFFFF},//reg_control_15
        {0x111440,0x0000,0xFFFF},//reg_control_16
        {0x111655,0x0200,0xFFFF},//reg_control_17
        {0x111656,0x0301,0xFFFF},//reg_free_swap_1_0
        {0x111657,0x0604,0xFFFF},//reg_free_swap_3_2
        {0x111658,0x0705,0xFFFF},//reg_free_swap_5_4
        {0x111659,0x1F1F,0xFFFF},//reg_free_swap_7_6
        {0x11165A,0x1F1F,0xFFFF},//reg_free_swap_9_8
        {0x11165B,0x1F1F,0xFFFF},//reg_free_swap_10_11
        {0x11165C,0x1F1F,0xFFFF},//reg_free_swap_13_12
        {0x11165D,0x1F1F,0xFFFF},//reg_free_swap_15_14
        {0x11165E,0x1F1F,0xFFFF},//reg_free_swap_17_16
        {0x111672,0x1F1F,0xFFFF},//reg_free_swap_19_18
        {0x111673,0x1F1F,0xFFFF},//reg_free_swap_21_20
        {0x111674,0x1F1F,0xFFFF},//reg_free_swap_23_22
        {0x111675,0x1F1F,0xFFFF},//reg_free_swap_25_24
        {0x111676,0x1F1F,0xFFFF},//reg_free_swap_27_26
        {0x111677,0x1F1F,0xFFFF},//reg_free_swap_29_28
    },
    { //E_PNL_SUPPORTED_MFT_VBY1_8LANE_4BLK_UHD60_4P_75to150MHz    NO.8
      //Address,Value,Mask
        {0x111211,0x0000,0xFFFF},//
        {0x111420,0x0000,0x0001},//reg_control_01
        {0x111421,0x3210,0xFFFF},//reg_control_02
        {0x111422,0x7654,0xFFFF},//reg_control_03
        {0x111402,0x0000,0xFFFF},//reg_control_04
        {0x111401,0x0004,0xFFFF},//reg_control_05
        {0x111410,0x1E00,0xFFFF},//reg_control_06
        {0x111411,0x03C0,0xFFFF},//reg_control_07
        {0x111412,0x0000,0xFFFF},//reg_control_08
        {0x111413,0x0780,0xFFFF},//reg_control_09
        {0x111414,0x0F00,0xFFFF},//reg_control_10
        {0x111415,0x1680,0xFFFF},//reg_control_11
        {0x111416,0x3FFF,0xFFFF},//reg_control_12
        {0x111417,0x3FFF,0xFFFF},//reg_control_13
        {0x111418,0x3FFF,0xFFFF},//reg_control_14
        {0x111419,0x3FFF,0xFFFF},//reg_control_15
        {0x111440,0x0000,0xFFFF},//reg_control_16
        {0x111655,0x0100,0xFFFF},//reg_control_17
        {0x111656,0x0302,0xFFFF},//reg_free_swap_1_0
        {0x111657,0x0504,0xFFFF},//reg_free_swap_3_2
        {0x111658,0x0706,0xFFFF},//reg_free_swap_5_4
        {0x111659,0x1F1F,0xFFFF},//reg_free_swap_7_6
        {0x11165A,0x1F1F,0xFFFF},//reg_free_swap_9_8
        {0x11165B,0x1F1F,0xFFFF},//reg_free_swap_10_11
        {0x11165C,0x1F1F,0xFFFF},//reg_free_swap_13_12
        {0x11165D,0x1F1F,0xFFFF},//reg_free_swap_15_14
        {0x11165E,0x1F1F,0xFFFF},//reg_free_swap_17_16
        {0x111672,0x1F1F,0xFFFF},//reg_free_swap_19_18
        {0x111673,0x1F1F,0xFFFF},//reg_free_swap_21_20
        {0x111674,0x1F1F,0xFFFF},//reg_free_swap_23_22
        {0x111675,0x1F1F,0xFFFF},//reg_free_swap_25_24
        {0x111676,0x1F1F,0xFFFF},//reg_free_swap_27_26
        {0x111677,0x1F1F,0xFFFF},//reg_free_swap_29_28
    },
#elif PANEL_VBY1_16CH_8Bit||PANEL_VBY1_16CH_10Bit
    { //E_PNL_SUPPORTED_MFT_VBY1_16LANE_1BLK_UHD120_4P_150to300MHz    NO.9
      //Address,Value,Mask
        {0x111211,0x0000,0xFFFF},//
        {0x111420,0x0000,0x0001},//reg_control_01
        {0x111421,0x3210,0xFFFF},//reg_control_02
        {0x111422,0x7654,0xFFFF},//reg_control_03
        {0x111402,0x0000,0xFFFF},//reg_control_04
        {0x111401,0x0002,0xFFFF},//reg_control_05
        {0x111410,0x1E00,0xFFFF},//reg_control_06
        {0x111411,0x0F00,0xFFFF},//reg_control_07
        {0x111412,0x0000,0xFFFF},//reg_control_08
        {0x111413,0x3FFF,0xFFFF},//reg_control_09
        {0x111414,0x3FFF,0xFFFF},//reg_control_10
        {0x111415,0x3FFF,0xFFFF},//reg_control_11
        {0x111416,0x3FFF,0xFFFF},//reg_control_12
        {0x111417,0x3FFF,0xFFFF},//reg_control_13
        {0x111418,0x3FFF,0xFFFF},//reg_control_14
        {0x111419,0x3FFF,0xFFFF},//reg_control_15
        {0x111440,0x0000,0xFFFF},//reg_control_16
        {0x111655,0x0200,0xFFFF},//reg_control_17
        {0x111656,0x0604,0xFFFF},//reg_free_swap_1_0
        {0x111657,0x0301,0xFFFF},//reg_free_swap_3_2
        {0x111658,0x0705,0xFFFF},//reg_free_swap_5_4
        {0x111659,0x1210,0xFFFF},//reg_free_swap_7_6
        {0x11165A,0x1614,0xFFFF},//reg_free_swap_9_8
        {0x11165B,0x1311,0xFFFF},//reg_free_swap_10_11
        {0x11165C,0x1715,0xFFFF},//reg_free_swap_13_12
        {0x11165D,0x1F1F,0xFFFF},//reg_free_swap_15_14
        {0x11165E,0x1F1F,0xFFFF},//reg_free_swap_17_16
        {0x111672,0x1F1F,0xFFFF},//reg_free_swap_19_18
        {0x111673,0x1F1F,0xFFFF},//reg_free_swap_21_20
        {0x111674,0x1F1F,0xFFFF},//reg_free_swap_23_22
        {0x111675,0x1F1F,0xFFFF},//reg_free_swap_25_24
        {0x111676,0x1F1F,0xFFFF},//reg_free_swap_27_26
        {0x111677,0x1F1F,0xFFFF},//reg_free_swap_29_28
    },
    { //E_PNL_SUPPORTED_MFT_VBY1_16LANE_2BLK_UHD120_4P_150to300MHz    NO.10
      //Address,Value,Mask
        {0x111211,0x0000,0xFFFF},//
        {0x111420,0x0000,0x0001},//reg_control_01
        {0x111421,0x3210,0xFFFF},//reg_control_02
        {0x111422,0x7654,0xFFFF},//reg_control_03
        {0x111402,0x0000,0xFFFF},//reg_control_04
        {0x111401,0x0003,0xFFFF},//reg_control_05
        {0x111410,0x1E00,0xFFFF},//reg_control_06
        {0x111411,0x0780,0xFFFF},//reg_control_07
        {0x111412,0x0000,0xFFFF},//reg_control_08
        {0x111413,0x3FFF,0xFFFF},//reg_control_09
        {0x111414,0x3FFF,0xFFFF},//reg_control_10
        {0x111415,0x3FFF,0xFFFF},//reg_control_11
        {0x111416,0x3FFF,0xFFFF},//reg_control_12
        {0x111417,0x3FFF,0xFFFF},//reg_control_13
        {0x111418,0x3FFF,0xFFFF},//reg_control_14
        {0x111419,0x3FFF,0xFFFF},//reg_control_15
        {0x111440,0x0000,0xFFFF},//reg_control_16
        {0x111655,0x0200,0xFFFF},//reg_control_17
        {0x111656,0x0301,0xFFFF},//reg_free_swap_1_0
        {0x111657,0x1210,0xFFFF},//reg_free_swap_3_2
        {0x111658,0x1311,0xFFFF},//reg_free_swap_5_4
        {0x111659,0x0604,0xFFFF},//reg_free_swap_7_6
        {0x11165A,0x0705,0xFFFF},//reg_free_swap_9_8
        {0x11165B,0x1614,0xFFFF},//reg_free_swap_10_11
        {0x11165C,0x1715,0xFFFF},//reg_free_swap_13_12
        {0x11165D,0x1F1F,0xFFFF},//reg_free_swap_15_14
        {0x11165E,0x1F1F,0xFFFF},//reg_free_swap_17_16
        {0x111672,0x1F1F,0xFFFF},//reg_free_swap_19_18
        {0x111673,0x1F1F,0xFFFF},//reg_free_swap_21_20
        {0x111674,0x1F1F,0xFFFF},//reg_free_swap_23_22
        {0x111675,0x1F1F,0xFFFF},//reg_free_swap_25_24
        {0x111676,0x1F1F,0xFFFF},//reg_free_swap_27_26
        {0x111677,0x1F1F,0xFFFF},//reg_free_swap_29_28
    },
    { //E_PNL_SUPPORTED_MFT_VBY1_16LANE_4BLK_UHD120_4P_150to300MHz    NO.11
      //Address,Value,Mask
        {0x111211,0x0000,0xFFFF},//
        {0x111420,0x0000,0x0001},//reg_control_01
        {0x111421,0x3210,0xFFFF},//reg_control_02
        {0x111422,0x7654,0xFFFF},//reg_control_03
        {0x111402,0x0000,0xFFFF},//reg_control_04
        {0x111401,0x0004,0xFFFF},//reg_control_05
        {0x111410,0x1E00,0xFFFF},//reg_control_06
        {0x111411,0x03C0,0xFFFF},//reg_control_07
        {0x111412,0x0000,0xFFFF},//reg_control_08
        {0x111413,0x0780,0xFFFF},//reg_control_09
        {0x111414,0x0F00,0xFFFF},//reg_control_10
        {0x111415,0x1680,0xFFFF},//reg_control_11
        {0x111416,0x3FFF,0xFFFF},//reg_control_12
        {0x111417,0x3FFF,0xFFFF},//reg_control_13
        {0x111418,0x3FFF,0xFFFF},//reg_control_14
        {0x111419,0x3FFF,0xFFFF},//reg_control_15
        {0x111440,0x0000,0xFFFF},//reg_control_16
        {0x111655,0x0100,0xFFFF},//reg_control_17
        {0x111656,0x1110,0xFFFF},//reg_free_swap_1_0
        {0x111657,0x0302,0xFFFF},//reg_free_swap_3_2
        {0x111658,0x1312,0xFFFF},//reg_free_swap_5_4
        {0x111659,0x0504,0xFFFF},//reg_free_swap_7_6
        {0x11165A,0x1514,0xFFFF},//reg_free_swap_9_8
        {0x11165B,0x0706,0xFFFF},//reg_free_swap_10_11
        {0x11165C,0x1716,0xFFFF},//reg_free_swap_13_12
        {0x11165D,0x1F1F,0xFFFF},//reg_free_swap_15_14
        {0x11165E,0x1F1F,0xFFFF},//reg_free_swap_17_16
        {0x111672,0x1F1F,0xFFFF},//reg_free_swap_19_18
        {0x111673,0x1F1F,0xFFFF},//reg_free_swap_21_20
        {0x111674,0x1F1F,0xFFFF},//reg_free_swap_23_22
        {0x111675,0x1F1F,0xFFFF},//reg_free_swap_25_24
        {0x111676,0x1F1F,0xFFFF},//reg_free_swap_27_26
        {0x111677,0x1F1F,0xFFFF},//reg_free_swap_29_28
    },
    { //E_PNL_SUPPORTED_MFT_VBY1_16LANE_8BLK_UHD120_4P_150to300MHz    NO.12
      //Address,Value,Mask
        {0x111211,0x0000,0xFFFF},//
        {0x111420,0x0000,0x0001},//reg_control_01
        {0x111421,0x3210,0xFFFF},//reg_control_02
        {0x111422,0x7654,0xFFFF},//reg_control_03
        {0x111402,0x0000,0xFFFF},//reg_control_04
        {0x111401,0x0005,0xFFFF},//reg_control_05
        {0x111410,0x1E00,0xFFFF},//reg_control_06
        {0x111411,0x01E0,0xFFFF},//reg_control_07
        {0x111412,0x0000,0xFFFF},//reg_control_08
        {0x111413,0x03C0,0xFFFF},//reg_control_09
        {0x111414,0x0780,0xFFFF},//reg_control_10
        {0x111415,0x0B40,0xFFFF},//reg_control_11
        {0x111416,0x0F00,0xFFFF},//reg_control_12
        {0x111417,0x12C0,0xFFFF},//reg_control_13
        {0x111418,0x1680,0xFFFF},//reg_control_14
        {0x111419,0x1A40,0xFFFF},//reg_control_15
        {0x111440,0x0000,0xFFFF},//reg_control_16
        {0x111655,0x1000,0xFFFF},//reg_control_17
        {0x111656,0x1202,0xFFFF},//reg_free_swap_1_0
        {0x111657,0x1404,0xFFFF},//reg_free_swap_3_2
        {0x111658,0x1606,0xFFFF},//reg_free_swap_5_4
        {0x111659,0x1101,0xFFFF},//reg_free_swap_7_6
        {0x11165A,0x1303,0xFFFF},//reg_free_swap_9_8
        {0x11165B,0x1505,0xFFFF},//reg_free_swap_10_11
        {0x11165C,0x1707,0xFFFF},//reg_free_swap_13_12
        {0x11165D,0x1F1F,0xFFFF},//reg_free_swap_15_14
        {0x11165E,0x1F1F,0xFFFF},//reg_free_swap_17_16
        {0x111672,0x1F1F,0xFFFF},//reg_free_swap_19_18
        {0x111673,0x1F1F,0xFFFF},//reg_free_swap_21_20
        {0x111674,0x1F1F,0xFFFF},//reg_free_swap_23_22
        {0x111675,0x1F1F,0xFFFF},//reg_free_swap_25_24
        {0x111676,0x1F1F,0xFFFF},//reg_free_swap_27_26
        {0x111677,0x1F1F,0xFFFF},//reg_free_swap_29_28
    },
    { //E_PNL_SUPPORTED_MFT_VBY1_16LANE_1BLK_5K120_8P_135to195.75MHz    NO.13
      //Address,Value,Mask
        {0x111211,0x4444,0xFFFF},//
        {0x111420,0x0000,0x0001},//reg_control_01
        {0x111421,0x3210,0xFFFF},//reg_control_02
        {0x111422,0x7654,0xFFFF},//reg_control_03
        {0x111402,0x0000,0xFFFF},//reg_control_04
        {0x111401,0x0001,0xFFFF},//reg_control_05
        {0x111410,0x1400,0xFFFF},//reg_control_06
        {0x111411,0x1400,0xFFFF},//reg_control_07
        {0x111412,0x0000,0xFFFF},//reg_control_08
        {0x111413,0x3FFF,0xFFFF},//reg_control_09
        {0x111414,0x3FFF,0xFFFF},//reg_control_10
        {0x111415,0x3FFF,0xFFFF},//reg_control_11
        {0x111416,0x3FFF,0xFFFF},//reg_control_12
        {0x111417,0x3FFF,0xFFFF},//reg_control_13
        {0x111418,0x3FFF,0xFFFF},//reg_control_14
        {0x111419,0x3FFF,0xFFFF},//reg_control_15
        {0x111440,0x0000,0xFFFF},//reg_control_16
        {0x111655,0x0200,0xFFFF},//reg_control_17
        {0x111656,0x0604,0xFFFF},//reg_free_swap_1_0
        {0x111657,0x0A08,0xFFFF},//reg_free_swap_3_2
        {0x111658,0x0E0C,0xFFFF},//reg_free_swap_5_4
        {0x111659,0x0301,0xFFFF},//reg_free_swap_7_6
        {0x11165A,0x0705,0xFFFF},//reg_free_swap_9_8
        {0x11165B,0x0B09,0xFFFF},//reg_free_swap_10_11
        {0x11165C,0x0F0D,0xFFFF},//reg_free_swap_13_12
        {0x11165D,0x1F1F,0xFFFF},//reg_free_swap_15_14
        {0x11165E,0x1F1F,0xFFFF},//reg_free_swap_17_16
        {0x111672,0x1F1F,0xFFFF},//reg_free_swap_19_18
        {0x111673,0x1F1F,0xFFFF},//reg_free_swap_21_20
        {0x111674,0x1F1F,0xFFFF},//reg_free_swap_23_22
        {0x111675,0x1F1F,0xFFFF},//reg_free_swap_25_24
        {0x111676,0x1F1F,0xFFFF},//reg_free_swap_27_26
        {0x111677,0x1F1F,0xFFFF},//reg_free_swap_29_28
    },
    { //E_PNL_SUPPORTED_MFT_VBY1_16LANE_2BLK_5K120_8P_135to195.75MHz    NO.14
      //Address,Value,Mask
        {0x111211,0x4444,0xFFFF},//
        {0x111420,0x0000,0x0001},//reg_control_01
        {0x111421,0x3210,0xFFFF},//reg_control_02
        {0x111422,0x7654,0xFFFF},//reg_control_03
        {0x111402,0x0000,0xFFFF},//reg_control_04
        {0x111401,0x0002,0xFFFF},//reg_control_05
        {0x111410,0x1400,0xFFFF},//reg_control_06
        {0x111411,0x0A00,0xFFFF},//reg_control_07
        {0x111412,0x0000,0xFFFF},//reg_control_08
        {0x111413,0x3FFF,0xFFFF},//reg_control_09
        {0x111414,0x3FFF,0xFFFF},//reg_control_10
        {0x111415,0x3FFF,0xFFFF},//reg_control_11
        {0x111416,0x3FFF,0xFFFF},//reg_control_12
        {0x111417,0x3FFF,0xFFFF},//reg_control_13
        {0x111418,0x3FFF,0xFFFF},//reg_control_14
        {0x111419,0x3FFF,0xFFFF},//reg_control_15
        {0x111440,0x0000,0xFFFF},//reg_control_16
        {0x111655,0x0200,0xFFFF},//reg_control_17
        {0x111656,0x0604,0xFFFF},//reg_free_swap_1_0
        {0x111657,0x0301,0xFFFF},//reg_free_swap_3_2
        {0x111658,0x0705,0xFFFF},//reg_free_swap_5_4
        {0x111659,0x0A08,0xFFFF},//reg_free_swap_7_6
        {0x11165A,0x0E0C,0xFFFF},//reg_free_swap_9_8
        {0x11165B,0x0B09,0xFFFF},//reg_free_swap_10_11
        {0x11165C,0x0F0D,0xFFFF},//reg_free_swap_13_12
        {0x11165D,0x1F1F,0xFFFF},//reg_free_swap_15_14
        {0x11165E,0x1F1F,0xFFFF},//reg_free_swap_17_16
        {0x111672,0x1F1F,0xFFFF},//reg_free_swap_19_18
        {0x111673,0x1F1F,0xFFFF},//reg_free_swap_21_20
        {0x111674,0x1F1F,0xFFFF},//reg_free_swap_23_22
        {0x111675,0x1F1F,0xFFFF},//reg_free_swap_25_24
        {0x111676,0x1F1F,0xFFFF},//reg_free_swap_27_26
        {0x111677,0x1F1F,0xFFFF},//reg_free_swap_29_28
    },
    { //E_PNL_SUPPORTED_MFT_VBY1_16LANE_4BLK_5K120_8P_135to195.75MHz    NO.15
      //Address,Value,Mask
        {0x111211,0x4444,0xFFFF},//
        {0x111420,0x0000,0x0001},//reg_control_01
        {0x111421,0x3210,0xFFFF},//reg_control_02
        {0x111422,0x7654,0xFFFF},//reg_control_03
        {0x111402,0x0000,0xFFFF},//reg_control_04
        {0x111401,0x0003,0xFFFF},//reg_control_05
        {0x111410,0x1400,0xFFFF},//reg_control_06
        {0x111411,0x0500,0xFFFF},//reg_control_07
        {0x111412,0x0000,0xFFFF},//reg_control_08
        {0x111413,0x0A00,0xFFFF},//reg_control_09
        {0x111414,0x3FFF,0xFFFF},//reg_control_10
        {0x111415,0x3FFF,0xFFFF},//reg_control_11
        {0x111416,0x3FFF,0xFFFF},//reg_control_12
        {0x111417,0x3FFF,0xFFFF},//reg_control_13
        {0x111418,0x3FFF,0xFFFF},//reg_control_14
        {0x111419,0x3FFF,0xFFFF},//reg_control_15
        {0x111440,0x0000,0xFFFF},//reg_control_16
        {0x111655,0x0200,0xFFFF},//reg_control_17
        {0x111656,0x0301,0xFFFF},//reg_free_swap_1_0
        {0x111657,0x0604,0xFFFF},//reg_free_swap_3_2
        {0x111658,0x0705,0xFFFF},//reg_free_swap_5_4
        {0x111659,0x0A08,0xFFFF},//reg_free_swap_7_6
        {0x11165A,0x0B09,0xFFFF},//reg_free_swap_9_8
        {0x11165B,0x0E0C,0xFFFF},//reg_free_swap_10_11
        {0x11165C,0x0F0D,0xFFFF},//reg_free_swap_13_12
        {0x11165D,0x1F1F,0xFFFF},//reg_free_swap_15_14
        {0x11165E,0x1F1F,0xFFFF},//reg_free_swap_17_16
        {0x111672,0x1F1F,0xFFFF},//reg_free_swap_19_18
        {0x111673,0x1F1F,0xFFFF},//reg_free_swap_21_20
        {0x111674,0x1F1F,0xFFFF},//reg_free_swap_23_22
        {0x111675,0x1F1F,0xFFFF},//reg_free_swap_25_24
        {0x111676,0x1F1F,0xFFFF},//reg_free_swap_27_26
        {0x111677,0x1F1F,0xFFFF},//reg_free_swap_29_28
    },
    { //E_PNL_SUPPORTED_MFT_VBY1_16LANE_8BLK_5K120_8P_135to195.75MHz    NO.16
      //Address,Value,Mask
        {0x111211,0x4444,0xFFFF},//
        {0x111420,0x0000,0x0001},//reg_control_01
        {0x111421,0x3210,0xFFFF},//reg_control_02
        {0x111422,0x7654,0xFFFF},//reg_control_03
        {0x111402,0x0000,0xFFFF},//reg_control_04
        {0x111401,0x0004,0xFFFF},//reg_control_05
        {0x111410,0x1400,0xFFFF},//reg_control_06
        {0x111411,0x0280,0xFFFF},//reg_control_07
        {0x111412,0x0000,0xFFFF},//reg_control_08
        {0x111413,0x0500,0xFFFF},//reg_control_09
        {0x111414,0x0A00,0xFFFF},//reg_control_10
        {0x111415,0x0F00,0xFFFF},//reg_control_11
        {0x111416,0x3FFF,0xFFFF},//reg_control_12
        {0x111417,0x3FFF,0xFFFF},//reg_control_13
        {0x111418,0x3FFF,0xFFFF},//reg_control_14
        {0x111419,0x3FFF,0xFFFF},//reg_control_15
        {0x111440,0x0000,0xFFFF},//reg_control_16
        {0x111655,0x0100,0xFFFF},//reg_control_17
        {0x111656,0x0302,0xFFFF},//reg_free_swap_1_0
        {0x111657,0x0504,0xFFFF},//reg_free_swap_3_2
        {0x111658,0x0706,0xFFFF},//reg_free_swap_5_4
        {0x111659,0x0908,0xFFFF},//reg_free_swap_7_6
        {0x11165A,0x0B0A,0xFFFF},//reg_free_swap_9_8
        {0x11165B,0x0D0C,0xFFFF},//reg_free_swap_10_11
        {0x11165C,0x0F0E,0xFFFF},//reg_free_swap_13_12
        {0x11165D,0x1F1F,0xFFFF},//reg_free_swap_15_14
        {0x11165E,0x1F1F,0xFFFF},//reg_free_swap_17_16
        {0x111672,0x1F1F,0xFFFF},//reg_free_swap_19_18
        {0x111673,0x1F1F,0xFFFF},//reg_free_swap_21_20
        {0x111674,0x1F1F,0xFFFF},//reg_free_swap_23_22
        {0x111675,0x1F1F,0xFFFF},//reg_free_swap_25_24
        {0x111676,0x1F1F,0xFFFF},//reg_free_swap_27_26
        {0x111677,0x1F1F,0xFFFF},//reg_free_swap_29_28
    },
#elif PANEL_VBY1_20CH_8Bit||PANEL_VBY1_20CH_10Bit
    { //E_PNL_SUPPORTED_MFT_VBY1_20LANE_1BLK_5K120_8P_195.75to391.5MHz    NO.17
      //Address,Value,Mask
        {0x111211,0x0004,0xFFFF},//
        {0x111420,0x0001,0x0001},//reg_control_01
        {0x111421,0x3120,0xFFFF},//reg_control_02
        {0x111422,0x7564,0xFFFF},//reg_control_03
        {0x111402,0x0000,0xFFFF},//reg_control_04
        {0x111401,0x0106,0xFFFF},//reg_control_05
        {0x111410,0x1400,0xFFFF},//reg_control_06
        {0x111411,0x1400,0xFFFF},//reg_control_07
        {0x111412,0x0000,0xFFFF},//reg_control_08
        {0x111413,0x0100,0xFFFF},//reg_control_09
        {0x111414,0x0200,0xFFFF},//reg_control_10
        {0x111415,0x0300,0xFFFF},//reg_control_11
        {0x111416,0x0400,0xFFFF},//reg_control_12
        {0x111417,0x3FFF,0xFFFF},//reg_control_13
        {0x111418,0x3FFF,0xFFFF},//reg_control_14
        {0x111419,0x3FFF,0xFFFF},//reg_control_15
        {0x111440,0x0000,0xFFFF},//reg_control_16
        {0x111655,0x0100,0xFFFF},//reg_control_17
        {0x111656,0x1110,0xFFFF},//reg_free_swap_1_0
        {0x111657,0x0302,0xFFFF},//reg_free_swap_3_2
        {0x111658,0x1312,0xFFFF},//reg_free_swap_5_4
        {0x111659,0x0504,0xFFFF},//reg_free_swap_7_6
        {0x11165A,0x1514,0xFFFF},//reg_free_swap_9_8
        {0x11165B,0x0706,0xFFFF},//reg_free_swap_10_11
        {0x11165C,0x1716,0xFFFF},//reg_free_swap_13_12
        {0x11165D,0x0908,0xFFFF},//reg_free_swap_15_14
        {0x11165E,0x1918,0xFFFF},//reg_free_swap_17_16
        {0x111672,0x1F1F,0xFFFF},//reg_free_swap_19_18
        {0x111673,0x1F1F,0xFFFF},//reg_free_swap_21_20
        {0x111674,0x1F1F,0xFFFF},//reg_free_swap_23_22
        {0x111675,0x1F1F,0xFFFF},//reg_free_swap_25_24
        {0x111676,0x1F1F,0xFFFF},//reg_free_swap_27_26
        {0x111677,0x1F1F,0xFFFF},//reg_free_swap_29_28
    },
    { //E_PNL_SUPPORTED_MFT_VBY1_20LANE_5BLK_5K120_8P_195.75to391.5MHz    NO.18
      //Address,Value,Mask
        {0x111211,0x0004,0xFFFF},//
        {0x111420,0x0001,0x0001},//reg_control_01
        {0x111421,0x3120,0xFFFF},//reg_control_02
        {0x111422,0x7564,0xFFFF},//reg_control_03
        {0x111402,0x0000,0xFFFF},//reg_control_04
        {0x111401,0x0107,0xFFFF},//reg_control_05
        {0x111410,0x1400,0xFFFF},//reg_control_06
        {0x111411,0x0100,0xFFFF},//reg_control_07
        {0x111412,0x0000,0xFFFF},//reg_control_08
        {0x111413,0x0100,0xFFFF},//reg_control_09
        {0x111414,0x0200,0xFFFF},//reg_control_10
        {0x111415,0x0300,0xFFFF},//reg_control_11
        {0x111416,0x0400,0xFFFF},//reg_control_12
        {0x111417,0x3FFF,0xFFFF},//reg_control_13
        {0x111418,0x3FFF,0xFFFF},//reg_control_14
        {0x111419,0x3FFF,0xFFFF},//reg_control_15
        {0x111440,0x0000,0xFFFF},//reg_control_16
        {0x111655,0x0100,0xFFFF},//reg_control_17
        {0x111656,0x1110,0xFFFF},//reg_free_swap_1_0
        {0x111657,0x0302,0xFFFF},//reg_free_swap_3_2
        {0x111658,0x1312,0xFFFF},//reg_free_swap_5_4
        {0x111659,0x0504,0xFFFF},//reg_free_swap_7_6
        {0x11165A,0x1514,0xFFFF},//reg_free_swap_9_8
        {0x11165B,0x0706,0xFFFF},//reg_free_swap_10_11
        {0x11165C,0x1716,0xFFFF},//reg_free_swap_13_12
        {0x11165D,0x0908,0xFFFF},//reg_free_swap_15_14
        {0x11165E,0x1918,0xFFFF},//reg_free_swap_17_16
        {0x111672,0x1F1F,0xFFFF},//reg_free_swap_19_18
        {0x111673,0x1F1F,0xFFFF},//reg_free_swap_21_20
        {0x111674,0x1F1F,0xFFFF},//reg_free_swap_23_22
        {0x111675,0x1F1F,0xFFFF},//reg_free_swap_25_24
        {0x111676,0x1F1F,0xFFFF},//reg_free_swap_27_26
        {0x111677,0x1F1F,0xFFFF},//reg_free_swap_29_28
    },
    { //E_PNL_SUPPORTED_MFT_VBY1_20LANE_10BLK_5K120_8P_195.75to391.5MHz    NO.19
      //Address,Value,Mask
        {0x111211,0x0004,0xFFFF},//
        {0x111420,0x0001,0x0001},//reg_control_01
        {0x111421,0x3120,0xFFFF},//reg_control_02
        {0x111422,0x7564,0xFFFF},//reg_control_03
        {0x111402,0x0000,0xFFFF},//reg_control_04
        {0x111401,0x0108,0xFFFF},//reg_control_05
        {0x111410,0x1400,0xFFFF},//reg_control_06
        {0x111411,0x0100,0xFFFF},//reg_control_07
        {0x111412,0x0000,0xFFFF},//reg_control_08
        {0x111413,0x0100,0xFFFF},//reg_control_09
        {0x111414,0x0200,0xFFFF},//reg_control_10
        {0x111415,0x0300,0xFFFF},//reg_control_11
        {0x111416,0x0400,0xFFFF},//reg_control_12
        {0x111417,0x3FFF,0xFFFF},//reg_control_13
        {0x111418,0x3FFF,0xFFFF},//reg_control_14
        {0x111419,0x3FFF,0xFFFF},//reg_control_15
        {0x111440,0x0000,0xFFFF},//reg_control_16
        {0x111655,0x0100,0xFFFF},//reg_control_17
        {0x111656,0x0302,0xFFFF},//reg_free_swap_1_0
        {0x111657,0x0504,0xFFFF},//reg_free_swap_3_2
        {0x111658,0x0706,0xFFFF},//reg_free_swap_5_4
        {0x111659,0x0908,0xFFFF},//reg_free_swap_7_6
        {0x11165A,0x1110,0xFFFF},//reg_free_swap_9_8
        {0x11165B,0x1312,0xFFFF},//reg_free_swap_10_11
        {0x11165C,0x1514,0xFFFF},//reg_free_swap_13_12
        {0x11165D,0x1716,0xFFFF},//reg_free_swap_15_14
        {0x11165E,0x1918,0xFFFF},//reg_free_swap_17_16
        {0x111672,0x1F1F,0xFFFF},//reg_free_swap_19_18
        {0x111673,0x1F1F,0xFFFF},//reg_free_swap_21_20
        {0x111674,0x1F1F,0xFFFF},//reg_free_swap_23_22
        {0x111675,0x1F1F,0xFFFF},//reg_free_swap_25_24
        {0x111676,0x1F1F,0xFFFF},//reg_free_swap_27_26
        {0x111677,0x1F1F,0xFFFF},//reg_free_swap_29_28
    },
#elif PANEL_VBY1_32CH_8Bit||PANEL_VBY1_32CH_10Bit
    { //E_PNL_SUPPORTED_MFT_VBY1_32LANE_1BLK_8K60_8P_225to300MHz    NO.20
      //Address,Value,Mask
        {0x111211,0x0000,0xFFFF},//
        {0x111420,0x0000,0x0001},//reg_control_01
        {0x111421,0x3210,0xFFFF},//reg_control_02
        {0x111422,0x7654,0xFFFF},//reg_control_03
        {0x111402,0x0000,0xFFFF},//reg_control_04
        {0x111401,0x0001,0xFFFF},//reg_control_05
        {0x111410,0x1E00,0xFFFF},//reg_control_06
        {0x111411,0x1E00,0xFFFF},//reg_control_07
        {0x111412,0x0000,0xFFFF},//reg_control_08
        {0x111413,0x3FFF,0xFFFF},//reg_control_09
        {0x111414,0x3FFF,0xFFFF},//reg_control_10
        {0x111415,0x3FFF,0xFFFF},//reg_control_11
        {0x111416,0x3FFF,0xFFFF},//reg_control_12
        {0x111417,0x3FFF,0xFFFF},//reg_control_13
        {0x111418,0x3FFF,0xFFFF},//reg_control_14
        {0x111419,0x3FFF,0xFFFF},//reg_control_15
        {0x111440,0x0000,0xFFFF},//reg_control_16
        {0x111655,0x0200,0xFFFF},//reg_control_17
        {0x111656,0x0604,0xFFFF},//reg_free_swap_1_0
        {0x111657,0x0A08,0xFFFF},//reg_free_swap_3_2
        {0x111658,0x0E0C,0xFFFF},//reg_free_swap_5_4
        {0x111659,0x0301,0xFFFF},//reg_free_swap_7_6
        {0x11165A,0x0705,0xFFFF},//reg_free_swap_9_8
        {0x11165B,0x0B09,0xFFFF},//reg_free_swap_10_11
        {0x11165C,0x0F0D,0xFFFF},//reg_free_swap_13_12
        {0x11165D,0x1210,0xFFFF},//reg_free_swap_15_14
        {0x11165E,0x1614,0xFFFF},//reg_free_swap_17_16
        {0x111672,0x1A18,0xFFFF},//reg_free_swap_19_18
        {0x111673,0x1E1C,0xFFFF},//reg_free_swap_21_20
        {0x111674,0x1311,0xFFFF},//reg_free_swap_23_22
        {0x111675,0x1715,0xFFFF},//reg_free_swap_25_24
        {0x111676,0x1B19,0xFFFF},//reg_free_swap_27_26
        {0x111677,0x1F1D,0xFFFF},//reg_free_swap_29_28
    },
    { //E_PNL_SUPPORTED_MFT_VBY1_32LANE_2BLK_8K60_8P_225to300MHz    NO.21
      //Address,Value,Mask
        {0x111211,0x0000,0xFFFF},//
        {0x111420,0x0000,0x0001},//reg_control_01
        {0x111421,0x3210,0xFFFF},//reg_control_02
        {0x111422,0x7654,0xFFFF},//reg_control_03
        {0x111402,0x0000,0xFFFF},//reg_control_04
        {0x111401,0x0002,0xFFFF},//reg_control_05
        {0x111410,0x1E00,0xFFFF},//reg_control_06
        {0x111411,0x0F00,0xFFFF},//reg_control_07
        {0x111412,0x0000,0xFFFF},//reg_control_08
        {0x111413,0x3FFF,0xFFFF},//reg_control_09
        {0x111414,0x3FFF,0xFFFF},//reg_control_10
        {0x111415,0x3FFF,0xFFFF},//reg_control_11
        {0x111416,0x3FFF,0xFFFF},//reg_control_12
        {0x111417,0x3FFF,0xFFFF},//reg_control_13
        {0x111418,0x3FFF,0xFFFF},//reg_control_14
        {0x111419,0x3FFF,0xFFFF},//reg_control_15
        {0x111440,0x0000,0xFFFF},//reg_control_16
        {0x111655,0x0200,0xFFFF},//reg_control_17
        {0x111656,0x0604,0xFFFF},//reg_free_swap_1_0
        {0x111657,0x0301,0xFFFF},//reg_free_swap_3_2
        {0x111658,0x0705,0xFFFF},//reg_free_swap_5_4
        {0x111659,0x1210,0xFFFF},//reg_free_swap_7_6
        {0x11165A,0x1614,0xFFFF},//reg_free_swap_9_8
        {0x11165B,0x1311,0xFFFF},//reg_free_swap_10_11
        {0x11165C,0x1715,0xFFFF},//reg_free_swap_13_12
        {0x11165D,0x0A08,0xFFFF},//reg_free_swap_15_14
        {0x11165E,0x0E0C,0xFFFF},//reg_free_swap_17_16
        {0x111672,0x0B09,0xFFFF},//reg_free_swap_19_18
        {0x111673,0x0F0D,0xFFFF},//reg_free_swap_21_20
        {0x111674,0x1A18,0xFFFF},//reg_free_swap_23_22
        {0x111675,0x1E1C,0xFFFF},//reg_free_swap_25_24
        {0x111676,0x1B19,0xFFFF},//reg_free_swap_27_26
        {0x111677,0x1F1D,0xFFFF},//reg_free_swap_29_28
    },
    { //E_PNL_SUPPORTED_MFT_VBY1_32LANE_8K60_4BLK_8P_225to300MHz    NO.22
      //Address,Value,Mask
        {0x111211,0x0000,0xFFFF},//
        {0x111420,0x0000,0x0001},//reg_control_01
        {0x111421,0x3210,0xFFFF},//reg_control_02
        {0x111422,0x7654,0xFFFF},//reg_control_03
        {0x111402,0x0000,0xFFFF},//reg_control_04
        {0x111401,0x0003,0xFFFF},//reg_control_05
        {0x111410,0x1E00,0xFFFF},//reg_control_06
        {0x111411,0x0780,0xFFFF},//reg_control_07
        {0x111412,0x0000,0xFFFF},//reg_control_08
        {0x111413,0x0F00,0xFFFF},//reg_control_09
        {0x111414,0x3FFF,0xFFFF},//reg_control_10
        {0x111415,0x3FFF,0xFFFF},//reg_control_11
        {0x111416,0x3FFF,0xFFFF},//reg_control_12
        {0x111417,0x3FFF,0xFFFF},//reg_control_13
        {0x111418,0x3FFF,0xFFFF},//reg_control_14
        {0x111419,0x3FFF,0xFFFF},//reg_control_15
        {0x111440,0x0000,0xFFFF},//reg_control_16
        {0x111655,0x0200,0xFFFF},//reg_control_17
        {0x111656,0x0301,0xFFFF},//reg_free_swap_1_0
        {0x111657,0x1210,0xFFFF},//reg_free_swap_3_2
        {0x111658,0x1311,0xFFFF},//reg_free_swap_5_4
        {0x111659,0x0604,0xFFFF},//reg_free_swap_7_6
        {0x11165A,0x0705,0xFFFF},//reg_free_swap_9_8
        {0x11165B,0x1614,0xFFFF},//reg_free_swap_10_11
        {0x11165C,0x1715,0xFFFF},//reg_free_swap_13_12
        {0x11165D,0x0A08,0xFFFF},//reg_free_swap_15_14
        {0x11165E,0x0B09,0xFFFF},//reg_free_swap_17_16
        {0x111672,0x1A18,0xFFFF},//reg_free_swap_19_18
        {0x111673,0x1B19,0xFFFF},//reg_free_swap_21_20
        {0x111674,0x0E0C,0xFFFF},//reg_free_swap_23_22
        {0x111675,0x0F0D,0xFFFF},//reg_free_swap_25_24
        {0x111676,0x1E1C,0xFFFF},//reg_free_swap_27_26
        {0x111677,0x1F1D,0xFFFF},//reg_free_swap_29_28
    },
    { //E_PNL_SUPPORTED_MFT_VBY1_32LANE_8BLK_8K60_8P_225to300MHz    NO.23
      //Address,Value,Mask
        {0x111211,0x0000,0xFFFF},//
        {0x111420,0x0000,0x0001},//reg_control_01
        {0x111421,0x3210,0xFFFF},//reg_control_02
        {0x111422,0x7654,0xFFFF},//reg_control_03
        {0x111402,0x0000,0xFFFF},//reg_control_04
        {0x111401,0x0004,0xFFFF},//reg_control_05
        {0x111410,0x1E00,0xFFFF},//reg_control_06
        {0x111411,0x03C0,0xFFFF},//reg_control_07
        {0x111412,0x0000,0xFFFF},//reg_control_08
        {0x111413,0x0780,0xFFFF},//reg_control_09
        {0x111414,0x0F00,0xFFFF},//reg_control_10
        {0x111415,0x1680,0xFFFF},//reg_control_11
        {0x111416,0x3FFF,0xFFFF},//reg_control_12
        {0x111417,0x3FFF,0xFFFF},//reg_control_13
        {0x111418,0x3FFF,0xFFFF},//reg_control_14
        {0x111419,0x3FFF,0xFFFF},//reg_control_15
        {0x111440,0x0000,0xFFFF},//reg_control_16
        {0x111655,0x0100,0xFFFF},//reg_control_17
        {0x111656,0x0100,0xFFFF},//reg_free_swap_1_0
        {0x111657,0x1110,0xFFFF},//reg_free_swap_3_2
        {0x111658,0x0302,0xFFFF},//reg_free_swap_5_4
        {0x111659,0x1312,0xFFFF},//reg_free_swap_7_6
        {0x11165A,0x0504,0xFFFF},//reg_free_swap_9_8
        {0x11165B,0x1514,0xFFFF},//reg_free_swap_10_11
        {0x11165C,0x0706,0xFFFF},//reg_free_swap_13_12
        {0x11165D,0x1716,0xFFFF},//reg_free_swap_15_14
        {0x11165E,0x0908,0xFFFF},//reg_free_swap_17_16
        {0x111672,0x1918,0xFFFF},//reg_free_swap_19_18
        {0x111673,0x0B0A,0xFFFF},//reg_free_swap_21_20
        {0x111674,0x1B1A,0xFFFF},//reg_free_swap_23_22
        {0x111675,0x0D0C,0xFFFF},//reg_free_swap_25_24
        {0x111676,0x1D1C,0xFFFF},//reg_free_swap_27_26
        {0x111677,0x0F0E,0xFFFF},//reg_free_swap_29_28
    },
#else

#endif
};


#endif

#endif


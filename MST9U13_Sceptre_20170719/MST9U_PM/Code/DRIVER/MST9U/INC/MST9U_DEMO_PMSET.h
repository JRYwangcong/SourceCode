#ifndef _MS_PMDEF_H
#define _MS_PMDEF_H
#if FPGA_HW_PMMODE
#define PM_DISABLE_R2_COREPOWER      (0)
#else
#define PM_DISABLE_R2_COREPOWER      (1)
#endif
#define PM_CLOCK                XTAL   //modified clock rate on PM mode - A059
#define PM_SUPPORT_AC2DC                    (0) // (1)supports to AC2DC;                                                 (0)doesn't support to AC2DC
#define PM_SUPPORT_SOG_TIME_SHARE       (0) // (1)supports to SoG time sharing function to reduce power consumption  (0)doesn't support to SoG time sharing function
#define PM_SUPPORT_DVI_TIME_SHARE       (0) // (1)supports to DVI time sharing function to reduce power consumption  (0)doesn't support to DVI time sharing function
#define PM_SUPPORT_ADC_TIME_SHARE       (0)
#define PM_SUPPORT_WAKEUP_DVI               (1) // (1)supports to DVI wakeup;                                            (0)doesn't support to DVI wakeup
#define PM_SUPPORT_WAKEUP_DP                (ENABLE_DP_INPUT)
#define PM_SUPPORT_DVI5V_TurnonRterm        (1)

#define PM_SUPPORT_WAKEUP_CEC			(0)

#if FPGA_HW_PMMODE
#define PM_POWERkEY_GETVALUE                (0)
#else
#define PM_POWERkEY_GETVALUE                (1)
#endif
#define PM_SARKEY_GETVALUE              (0||ENABLE_DC_SDM_FORWARD)
#define PM_CABLEDETECT_USE_GPIO         (0)
#define PM_CABLE_DETECT_USE_SAR         (CABLE_DETECT_VGA_USE_SAR||CABLE_DETECT_VGA_USE_SAR)
#define PM_XTAL_AAC                                 (1)
#define _NEW_SOG_WAKEUP_DET_             (_NEW_SOG_DET_)
#define ENABLE_SYNC_CHECK_AGAIN          (0)
#define ENABLE_SW_SARMODE          (1||ENABLE_MHL)
#define PM_POWERSAVING_WAKEUP_GPIO              (0||ENABLE_DAISY_CHAIN)
#define PM_POWERSAVING_WAKEUP_SAR               1
#define PM_POWERSAVING_WAKEUP_SOG               1
#define PM_POWERSAVING_WAKEUP_MCCS              1
#define PM_POWERSAVING_SARmode                  ePMSAR_SAR3

#define PM_POWEROFF_WAKEUP_GPIO                     0
#define PM_POWEROFF_WAKEUP_SAR                      0
#define PM_POWEROFF_WAKEUP_SOG                  0
#define PM_POWEROFF_WAKEUP_MCCS                     1
#define PM_POWEROFF_SARmode                     ePMSAR_SAR3
#if PM_SUPPORT_WAKEUP_DVI
#define DVI5VP0_GPIO 1
#define DVI5VP1_GPIO 0
#endif
#define ENABLE_POWER_BOARD_CONTROL  0 // control power board enter to saving mode


#define XBYTE       ((unsigned char volatile xdata *) 0)
//-------------------------------------------------------------------------------------------------
//  PM CLOCK
//-------------------------------------------------------------------------------------------------
#define   RCOSC         0
#define   XTAL          1
#define   XTALDIV2      2
#define   XTALDIV4      3

enum
{
    CLK_LIVE_XTAL,
    CLK_LIVE_RCOSC_4M
};

//-------------------------------------------------------------------------------------------------
//  VGA
//-------------------------------------------------------------------------------------------------
#define REG_SYNC_DET    REG_0003A0

#define EN_CSYNC_DET    _BIT7
#define EN_HSYNC_DET    _BIT6
#define EN_VSYNC_DET    _BIT5
#define EN_SOG_DET      _BIT4
#define EN_HVSYNC_DET   _BIT3
#define EN_HSYNC_LOST   _BIT2
#define EN_VSYNC_LOST   _BIT1
#define EN_SOG_LOST     _BIT0

#define EN_SYNC_DET_SET     EN_HVSYNC_DET|EN_CSYNC_DET
#define EN_SYNC_DET_MASK    EN_HSYNC_DET|EN_VSYNC_DET|EN_HVSYNC_DET|EN_CSYNC_DET
//-------------------------------------------------------------------------------------------------
//  DVI
//-------------------------------------------------------------------------------------------------
#define REG_DVIRAW_0      REG_00038F
#define EN_GATING_CLK0   _BIT3

#define REG_DVIRAW_12    REG_000394
#define EN_GATING_CLK1   _BIT3
#define EN_GATING_CLK2   _BIT7


#define REG_DVIRAW_34    REG_000395
#define EN_GATING_CLK3   _BIT3
#define EN_GATING_CLK4   _BIT7


#define REG_DVIRAW_5      REG_000396
#define EN_GATING_CLK5   _BIT3




#define REG_DVI_DET         REG_00039E
#define EN_DVI_LOST_4       _BIT2
#define EN_DVI_LOST_5      _BIT3
#define EN_DVI_LOST_0       _BIT12
#define EN_DVI_LOST_1      _BIT13
#define EN_DVI_LOST_2      _BIT14
#define EN_DVI_LOST_3      _BIT15

#define EN_DVI_DET_4        _BIT0
#define EN_DVI_DET_5       _BIT1
#define EN_DVI_DET_0        _BIT8
#define EN_DVI_DET_1       _BIT9
#define EN_DVI_DET_2        _BIT10
#define EN_DVI_DET_3       _BIT11

#define EN_DVI_DET_SET      (EN_DVI_DET_2|EN_DVI_DET_3|EN_DVI_DET_5)
#define EN_DVI_DET_MASK     0xFFFF
//---------------------------------------
#define REG_DVI_CHEN        REG_0003BE
#define DVI_ON_PERIOD           0x7F //0x07    // <Janus>2010/07/14: valid range 1~7. Period = DVI_ON_PERIOD * 8.2 ms

#define DVI_CTRL_PERIOD         0x07    // <Janus>2010/07/14: valid range 0~7. control period = (DVI_CTRL_PERIOD+1) * 131.2ms
#define EN_DVICLK_CTRL             _BIT13
#define EN_DVI_ALWAYS_ON        _BIT12
#define EN_DVICH0             0
#define EN_DVICH1             BIT14
#define EN_DVICH2             BIT15
#define EN_DVI_CHEN_AUTO        (BIT14|BIT15)

#define EN_DVI_CHEN_SET       EN_DVI_CHEN_AUTO
#define EN_DVI_CHEN_MASK    (BIT14|BIT15)
#define EN_DVI_CTRL_PERIOD         EN_DVICLK_CTRL|(DVI_CTRL_PERIOD<<8)|DVI_ON_PERIOD
#define EN_DVI_CTRL_ALWAYS         EN_DVICLK_CTRL|EN_DVI_ALWAYS_ON

#define EN_DVI_CTRL_MASK        0xFFFF

#define EN_DVI_RTERM        _BIT3


#define REG_DVI5V_DETECT_01     REG_0017CD
#define DVI5V_DETECT_0               BIT0
#define DVI5V_DETECT_1               BIT4

#define REG_DVI5V_DETECT_2       REG_0017CF
#define DVI5V_DETECT_2               BIT0

#define REG_DVI5V_DETECT_345   REG_001780
#define DVI5V_DETECT_3                BIT0
#define DVI5V_DETECT_4                BIT1
#define DVI5V_DETECT_5                BIT2


//-------------------------------------------------------------------------------------------------
//  SAR
//-------------------------------------------------------------------------------------------------
#define REG_SAR_GPIO        REG_000384

#define EN_SAR_DET          _BIT7
#define INV_SAR_POL         _BIT6

#define EN_SAR_DET_SET      EN_SAR_DET
#define EN_SAR_DET_MASK     EN_SAR_DET|INV_SAR_POL

//-- GPIO ---------------------------------
#define REG_PMGPIO_EN        REG_000360 // use word access to this register
#define EN_GPIO54_DET       _BIT0      // enable wakeup detect of PM GPIO 54
#define EN_GPIO55_DET       _BIT1      // enable wakeup detect of PM GPIO 55
#define EN_GPIO56_DET       _BIT2      // enable wakeup detect of PM GPIO 56
#define EN_GPIO57_DET       _BIT3      // enable wakeup detect of PM GPIO 57
#define EN_GPIO00_DET       _BIT4      // enable wakeup detect of PM GPIO 00
#define EN_GPIO01_DET       _BIT5      // enable wakeup detect of PM GPIO 01
#define EN_GPIO06_DET       _BIT6      // enable wakeup detect of PM GPIO 06
#define EN_GPIO07_DET       _BIT7      // enable wakeup detect of PM GPIO 07
#define EN_GPIO46_DET       _BIT8      // enable wakeup detect of PM GPIO 46
#define EN_GPIO47_DET       _BIT9      // enable wakeup detect of PM GPIO 47
#define EN_GPIOX02_DET      _BIT10     // enable wakeup detect of PM GPIO X02
#define EN_GPIOX12_DET      _BIT11     // enable wakeup detect of PM GPIO X12
#define EN_GPIOX22_DET      _BIT12     // enable wakeup detect of PM GPIO X22
#define EN_GPIOX32_DET      _BIT13     // enable wakeup detect of PM GPIO X32
#define EN_GPIOX42_DET      _BIT14     // enable wakeup detect of PM GPIO X42
#define EN_GPIOX52_DET      _BIT15     // enable wakeup detect of PM GPIO X52


#define REG_PMGPIO_POL       REG_000362 // use word access to this register
#define INV_GPIO54_POL       _BIT0      // invert input signal polarity of PM GPIO 54
#define INV_GPIO55_POL       _BIT1      // invert input signal polarity of PM GPIO 55
#define INV_GPIO56_POL       _BIT2      // invert input signal polarity of PM GPIO 56
#define INV_GPIO57_POL       _BIT3      // invert input signal polarity of PM GPIO 57
#define INV_GPIO00_POL       _BIT4      // invert input signal polarity of PM GPIO 00
#define INV_GPIO01_POL       _BIT5      // invert input signal polarity of PM GPIO 01
#define INV_GPIO06_POL       _BIT6      // invert input signal polarity of PM GPIO 06
#define INV_GPIO07_POL       _BIT7      // invert input signal polarity of PM GPIO 07
#define INV_GPIO46_POL       _BIT8      // invert input signal polarity of PM GPIO 46
#define INV_GPIO47_POL       _BIT9      // invert input signal polarity of PM GPIO 47
#define INV_GPIOX02_POL      _BIT10     // invert input signal polarity of PM GPIO X02
#define INV_GPIOX12_POL      _BIT11     // invert input signal polarity of PM GPIO X12
#define INV_GPIOX22_POL      _BIT12     // invert input signal polarity of PM GPIO X22
#define INV_GPIOX32_POL      _BIT13     // invert input signal polarity of PM GPIO X32
#define INV_GPIOX42_POL      _BIT14     // invert input signal polarity of PM GPIO X42
#define INV_GPIOX52_POL      _BIT15     // invert input signal polarity of PM GPIO X52


#define REG_PMGPIO_STS   REG_000364  // use word access to this register
#define GPIO54_INT       _BIT0       // wakeup status of PM GPIO 54
#define GPIO55_INT       _BIT1       // wakeup status of PM GPIO 55
#define GPIO56_INT       _BIT2       // wakeup status of PM GPIO 56
#define GPIO57_INT       _BIT3       // wakeup status of PM GPIO 57
#define GPIO00_INT       _BIT4       // wakeup status of PM GPIO 00
#define GPIO01_INT       _BIT5       // wakeup status of PM GPIO 01
#define GPIO06_INT       _BIT6       // wakeup status of PM GPIO 06
#define GPIO07_INT       _BIT7       // wakeup status of PM GPIO 07
#define GPIO46_INT       _BIT8       // wakeup status of PM GPIO 46
#define GPIO47_INT       _BIT9       // wakeup status of PM GPIO 47
#define GPIOX02_INT      _BIT10      // wakeup status of PM GPIO X02
#define GPIOX12_INT      _BIT11      // wakeup status of PM GPIO X12
#define GPIOX22_INT      _BIT12      // wakeup status of PM GPIO X22
#define GPIOX32_INT      _BIT13      // wakeup status of PM GPIO X32
#define GPIOX42_INT      _BIT14      // wakeup status of PM GPIO X42
#define GPIOX52_INT      _BIT15      // wakeup status of PM GPIO X52


#define PM_POWERKEY_INT     PM_POWERKEY_WAKEUP

#if ENABLE_DAISY_CHAIN
#define PM_GPIO_DET_DC      EN_GPIOX02_DET
#define EN_GPIO_DET_SET     (PM_GPIO_DET_DC | PM_POWERKEY_WAKEUP)
#else
#define EN_GPIO_DET_SET     PM_POWERKEY_WAKEUP
#endif

#define EN_GPIO_DET_MASK    ( EN_GPIO54_DET | EN_GPIO55_DET | EN_GPIO56_DET | EN_GPIO57_DET \
                            | EN_GPIO00_DET | EN_GPIO01_DET | EN_GPIO06_DET | EN_GPIO07_DET \
                            | EN_GPIO46_DET | EN_GPIO47_DET | EN_GPIOX02_DET | EN_GPIOX12_DET \
                            | EN_GPIOX22_DET | EN_GPIOX32_DET | EN_GPIOX42_DET | EN_GPIOX52_DET )

#define INV_GPIO_POL_SET    ( PM_POWERKEY_INT )
#define INV_GPIO_POL_MASK   ( INV_GPIO54_POL | INV_GPIO55_POL | INV_GPIO56_POL | INV_GPIO57_POL \
                             |INV_GPIO00_POL | INV_GPIO01_POL | INV_GPIO06_POL | INV_GPIO07_POL \
                             |INV_GPIO46_POL | INV_GPIO47_POL | INV_GPIOX02_POL | INV_GPIOX12_POL \
                             |INV_GPIOX22_POL | INV_GPIOX32_POL | INV_GPIOX42_POL | INV_GPIOX52_POL )


//#define PM_VGACBL_DET         0
//#define PM_DVICBL_DET         0
//#define PM_HDMIACBL_DET         0
//#define PM_HDMIBCBL_DET         0
//#define PM_HDMICCBL_DET         0
//#define PM_DPCBL_DET            INV_GPIO00_POL
//----------------------------------------
#define REG_SINGLE_KEY          REG_003A60
#define SAR_CH0                 0
#define SAR_CH1                 1
#define SAR_CH2                 2
#define SAR_CH3                 3
#define SAR_CHMASK              BIT1|BIT0
#define EN_SAR_SINGLE           _BIT4
#define PD_SAR                         _BIT6
#define SINGLE_KEY_SEL          SAR_CH0

#define EN_SAR_SINGLE_SET       EN_SAR_SINGLE|SINGLE_KEY_SEL
#define EN_SAR_SINGLE_MASK      EN_SAR_SINGLE|SAR_CHMASK|PD_SAR
//----------------------------------------
#define REG_SAR_CMP             REG_003A64
#define EN_SAR_05V              0x00
#define EN_SAR_14V              0x40
#define EN_SAR_25V              0xC0

#define EN_SAR_CMP_MASK         0xC0
//----------------------------------------
#define REG_SAR_ANYKEY          REG_003A24
#define EN_ANY_KEY              _BIT7
#define EN_TWO_CH               _BIT6
#define SAR_SWITCH_CNT          0x02    // SAR channel switch period

#define EN_SAR_3CH_SET          EN_ANY_KEY|SAR_SWITCH_CNT
#define EN_SAR_2CH_SET          EN_ANY_KEY|EN_TWO_CH|SAR_SWITCH_CNT

#define EN_SAR_ANYKEY_MASK      0xFF

//-------------------------------------------------------------------------------------------------
//  MCCS
//-------------------------------------------------------------------------------------------------
//#define REG_D2B_EN              REG_PM_90
//#define D2B_EN_A0               _BIT3
//#define D2B_EN_A1               _BIT2
//#define D2B_EN_D0               _BIT1
//#define D2B_EN_D1               _BIT0
//#define REG_D2B_EN_MASK         D2B_EN_A0|D2B_EN_A1|D2B_EN_D0|D2B_EN_D1
//#define REG_D2B_EN_SET          D2B_EN_A0|D2B_EN_A1|D2B_EN_D0|D2B_EN_D1

//-------------------------------------------------------------------------------------------------
//  DDC
//-------------------------------------------------------------------------------------------------
//#define REG_DDC_EN              REG_PM_91
//#define DDC_EN_A0               _BIT3
//#define DDC_EN_A1               _BIT2
//#define DDC_EN_D0               _BIT1
//#define DDC_EN_D1               _BIT0
//#define REG_DDC_EN_MASK         DDC_EN_A0|DDC_EN_A1|DDC_EN_D0|DDC_EN_D1
//#define REG_DDC_EN_SET          DDC_EN_A0|DDC_EN_A1|DDC_EN_D0|DDC_EN_D1

//-------------------------------------------------------------------------------------------------
//  Others
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Wakeup Event
//-------------------------------------------------------------------------------------------------
#define REG_EVENT1          REG_000385

#define HSYNC_DET_0         BIT7
#define VSYNC_DET_0         BIT6
#define SOG_DET_0           BIT5
#define HV_DET_0            BIT4
//#define HSYNC_DET_1     BIT3
//#define VSYNC_DET_1     BIT2
//#define SOG_DET_1         BIT1
//#define HV_DET_1           BIT0
#define HVSYNC_CHK(REG_85)        (REG_85&(HSYNC_DET_0|VSYNC_DET_0|HV_DET_0))
#define SOG_CHK(REG_85)              (REG_85&(SOG_DET_0))
//----------------------------------------
#define REG_EVENT2          REG_000386

#define D2B_WAKEUP          _BIT7
#define MHL_CBUS_WAKEUP     _BIT6
#define DVI_CLK_DET_0       _BIT5
//#define DVI_CLK_DET_1       0
#define SAR_IN_DET          _BIT4
#define DP_WAKEUP           _BIT3
#define CS_DET              _BIT2   // composite sync detect status
//#define CEC_WAKEUP          0
#define IR_WAKEUP          _BIT0

#define CSYNC_CHK(reg_86)              (reg_86&CS_DET)



#define REG_EVENT3         REG_00038B

#define DVI_CLK_DET_1     _BIT0
#define DVI_CLK_DET_2     _BIT1
#define DVI_CLK_DET_3     _BIT2
#define DVI_CLK_DET_4     _BIT3
#define DVI_CLK_DET_5     _BIT4
//REG_0003A2
#define CEC_WAKEUP       _BIT1 //_BIT7
#define REG_EVENT4         REG_0003A3
#define HVSYNC_ACT(REG_A3) (REG_A3&_BIT0)
#define SOGDet_ACT(REG_A3) (REG_A3&_BIT1)

#define CSYNC_ACT(REG_A2) (REG_A2&_BIT0)

//-------------------------------------------------------------------------------------------------
//  AC2DC function
//-------------------------------------------------------------------------------------------------
#define AC2DC_PAD                   AC2DC_PAD_GPIOP25    /*BY HW SETTING */
#define AC2DC_PAD_Disable           0x00
#define AC2DC_PAD_GPIOP25           BIT0
#define AC2DC_PAD_GPIOP24           BIT1
#define AC2DC_MODE                  VL_MODE
#define OFF_MODE                    0x04
#define OFF_PERIOD                  0x3E                                    /* OFF period = (OFF_PERIOD+1)X32.8mS , should >1S */   /*BY HW SETTING */
#define VL_MODE                     0xA4
#define VH_MODE                     0x91
#define VHL_MODE                    0xB5

//-------------------------------------------------------------------------------------------------
//  msPM_StartRCOSCCal function
//-------------------------------------------------------------------------------------------------
//#define AC2DC_RCOSC_CLKOFF          0xBB
//#define AC2DC_CLKOFF                0x0B
//#define RCOSC_CLKOFF                0xB0



#define WAKEUP_RESET_CHIP 1
#endif

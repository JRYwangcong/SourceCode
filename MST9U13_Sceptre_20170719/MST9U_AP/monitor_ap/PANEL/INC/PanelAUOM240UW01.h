#ifndef _PanelLTM230HT3_
#define _PanelLTM230HT3_
#define PanelName               "AUOM240UW01"

#define PanelDither                 8
#define DITHER_COEFF                0x2D
#define DITHER_METHOD               0x42

#define PanelTTL                    0
#define PanelTCON                   0
#define PanelLVDS                   0xff
#define PanelRSDS                   0
#define PanelminiLVDS               0

#define ShortLineCheck              0
#define LVDS_TIMode                 0xff
#define PanelDualPort               0xff
//////////////Select Panel Type/////////////
#define PANEL_TTL                   0
#define PANEL_LVDS_1CH              0
#define PANEL_LVDS_2CH              1// 0
#define PANEL_LVDS_4CH              0
#define PANEL_LVDS_8CH              0
#define PANEL_VBY1_1CH_8Bit         0
#define PANEL_VBY1_1CH_10Bit        0
#define PANEL_VBY1_2CH_8Bit         0
#define PANEL_VBY1_2CH_10Bit        0
#define PANEL_VBY1_4CH_8Bit         0
#define PANEL_VBY1_4CH_10Bit        0
#define PANEL_VBY1_8CH_8Bit         0
#define PANEL_VBY1_8CH_10Bit        0
#define PANEL_VBY1()       ((PANEL_VBY1_1CH_8Bit)  ||  \
                            (PANEL_VBY1_1CH_10Bit) ||  \
                            (PANEL_VBY1_2CH_8Bit)  ||  \
                            (PANEL_VBY1_2CH_10Bit) ||  \
                            (PANEL_VBY1_4CH_8Bit)  ||  \
                            (PANEL_VBY1_4CH_10Bit) ||  \
                            (PANEL_VBY1_8CH_8Bit)  ||  \
                            (PANEL_VBY1_8CH_10Bit) )
#define PANEL_EDP                   0
////////Select Panel Output Format/////////
#define PANEL_OUTPUT_FMT_LR         0
#define PANEL_OUTPUT_FMT_4B         0

#define PanelSwapPort               0//0xff
#define PanelSwapOddML              0
#define PanelSwapEvenML             0
#define PanelSwapOddRB              0
#define PanelSwapEvenRB             0
#define PanelSwapMLSB               0
#define PanelDClkDelay              8
#define PanelInvDE                  0
#define PanelInvDClk                0
#define PanelInvHSync               0
#define PanelInvVSync               0

#define PANEL_SWAP_PN               0xFF


#define PanelDCLKCurrent            1       // Dclk current
#define PanelDECurrent              1       // DE signal current
#define PanelOddDataCurrent         1       // Odd data current
#define PanelEvenDataCurrent        1       // Even data current
#define PanelOnTiming0              2       // time between 3.3V - 12V
#define PanelOnTiming1              25      // time between panel & data while turn on power
#define PanelOnTiming2              550     // time between data & back light while turn on power  //20050701
#define PanelOffTiming1             100     // time between back light & data while turn off power
#define PanelOffTiming2             10      // time between data & panel while turn off power
#define PanelOffTiming0             0       // time between 12V - 3.3V
#define PanelHSyncWidth             16
#define PanelHSyncBackPorch         32
#define PanelVSyncWidth             8
#define PanelVSyncBackPorch         16      // 46
#define PanelHStart                 (PanelHSyncWidth+PanelHSyncBackPorch)
#define PanelVStart                 (PanelVSyncWidth+PanelVSyncBackPorch)
#define PanelWidth                  1920
#define PanelHeight                 1200
#define PanelHTotal                 2235
#define PanelVTotal                 1232
#define PanelMaxHTotal              2300
#define PanelMinHTotal              2100
#define PanelMaxVTotal              1224
#define PanelMinVTotal              900
#define PanelDCLK                   165
#define PanelMaxDCLK                200
#define PanelMinDCLK                120
#define HV_OUTPUT_TYPE              HSRM_B

#define PanelVfreq                  (600)
#define PanelMaxVfreq               (750+20) // 75Hz
#define PanelMinVfreq               (500-20) // 50Hz

//=================================================================
// TCON  setting for LVDS
//PVS / FSYNC
#define GPO9_VStart     0x7
#define GPO9_VEnd       0x705
#define GPO9_HStart     0x0
#define GPO9_HEnd       0x0
#define GPO9_Control        0x7

//HSYNC
#define GPOA_VStart     0x006
#define GPOA_VEnd       0x408
#define GPOA_HStart     0
#define GPOA_HEnd       0
#define GPOA_Control        0x4

//=================================================================
//Color Temp
#define DefCool_RedColor                    105
#define DefCool_GreenColor                113
#define DefCool_BlueColor                   130
#define DefNormal_RedColor               116
#define DefNormal_GreenColor           122
#define DefNormal_BlueColor              130
#define DefWarm_RedColor                 130
#define DefWarm_GreenColor              130
#define DefWarm_BlueColor                130
#define DefsRGB_RedColor                   130
#define DefsRGB_GreenColor               130
#define DefsRGB_BlueColor                  130
#endif

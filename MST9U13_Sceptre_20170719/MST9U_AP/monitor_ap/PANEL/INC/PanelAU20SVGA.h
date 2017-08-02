#ifndef __PANEL_AU170N5E_
#define __PANEL_AU170N5E_
#define PanelName   "AU170N5E"

#define PanelDither         6
#define DITHER_COEFF        0x2D
#define DITHER_METHOD       0x42

#define PanelTTL            0
#define PanelTCON           0
#define PanelLVDS           0xFF
#define PanelRSDS           0
#define PanelMiniLVDS3	    0

#define ShortLineCheck      0
#define LVDS_TIMode         0xFF
#define PanelDualPort       0
#define PANEL_TTL      0
#define PANEL_LVDS_1CH      0
#define PANEL_LVDS_2CH      1// 0
#define PANEL_LVDS_4CH      0
#define PANEL_LVDS_8CH      0
#define PANEL_VBY1_1CH_8Bit      0
#define PANEL_VBY1_1CH_10Bit      0
#define PANEL_VBY1_2CH_8Bit      0
#define PANEL_VBY1_2CH_10Bit      0
#define PANEL_VBY1_4CH_8Bit      0
#define PANEL_VBY1_4CH_10Bit      0
#define PANEL_VBY1_8CH_8Bit      0
#define PANEL_VBY1_8CH_10Bit      0
#define PANEL_VBY1()       ((PANEL_VBY1_1CH_8Bit)  ||  \
                            (PANEL_VBY1_1CH_10Bit) ||  \
                            (PANEL_VBY1_2CH_8Bit)  ||  \
                            (PANEL_VBY1_2CH_10Bit) ||  \
                            (PANEL_VBY1_4CH_8Bit)  ||  \
                            (PANEL_VBY1_4CH_10Bit) ||  \
                            (PANEL_VBY1_8CH_8Bit)  ||  \
                            (PANEL_VBY1_8CH_10Bit) )
#define PANEL_EDP      0
////////Select Panel Output Format/////////
#define PANEL_OUTPUT_FMT_LR     0
#define PANEL_OUTPUT_FMT_4B     0

#define PanelSwapPort     0//0xff
#define PanelSwapOddML    0
#define PanelSwapEvenML   0
#define PanelSwapOddRB    0
#define PanelSwapEvenRB   0
#define PanelSwapMLSB   0
#define PanelDClkDelay    8
#define PanelInvDE      0
#define PanelInvDClk    0
#define PanelInvHSync   0
#define PanelInvVSync   0

#define PANEL_SWAP_PN       0xFF
//==================================================================

// driving current setting 0==>4mA, 1==>6mA, 2==>8mA ,3==>12mA
#define PanelDCLKCurrent        1 // Dclk current
#define PanelDECurrent          1 // DE signal current
#define PanelOddDataCurrent     1 // Odd data current
#define PanelEvenDataCurrent    1 // Even data current

#define PanelOnTiming1          30  // time between panel & data while turn on power
#define PanelOnTiming2          100// time between data & back light while turn on power
#define PanelOffTiming1         20 // time between back light & data while turn off power
#define PanelOffTiming2         20//16  // time between data & panel while turn off power
#ifdef MST9U_FPGA
#if 1//800x600
#define PanelHSyncWidth         40//MST9U fpga 128      // a multiple of 8
#define PanelHSyncBackPorch     36//MST9U fpga 88//32      // a multiple of 8

#define PanelVSyncWidth         4
#define PanelVSyncBackPorch     16//MST9U fpga 20

#define PanelHStart     (PanelHSyncWidth+PanelHSyncBackPorch)
#define PanelVStart     (PanelVSyncWidth+PanelVSyncBackPorch)
#define PanelWidth      800
#define PanelHeight     600

#define PanelHTotal     937//MST9U fpga 1056
#define PanelVTotal     630

#define PanelMaxHTotal  1300
#define PanelMinHTotal  1035

#define PanelMaxVTotal  750
#define PanelMinVTotal  610

#define PanelDCLK       (((DWORD)PanelHTotal*PanelVTotal*60)/1000000)
#define PanelMaxDCLK    49
#define PanelMinDCLK    39
#else //640x480
#define PanelHSyncWidth         40
#define PanelHSyncBackPorch     36

#define PanelVSyncWidth         4
#define PanelVSyncBackPorch     16

#define PanelHStart     (PanelHSyncWidth+PanelHSyncBackPorch)
#define PanelVStart     (PanelVSyncWidth+PanelVSyncBackPorch)
#define PanelWidth      640
#define PanelHeight     480

#define PanelHTotal     (937-160)
#define PanelVTotal     (630-120)

#define PanelMaxHTotal  (1300-160)
#define PanelMinHTotal  (1035-160)

#define PanelMaxVTotal  (750-120)
#define PanelMinVTotal  (610-120)

#define PanelDCLK       (((DWORD)PanelHTotal*PanelVTotal*60)/1000000)
#define PanelMaxDCLK    43
#define PanelMinDCLK    26
#endif
#else
#define PanelHSyncWidth         128      // a multiple of 8
#define PanelHSyncBackPorch     88//32      // a multiple of 8

#define PanelVSyncWidth         4
#define PanelVSyncBackPorch     20

#define PanelHStart     (PanelHSyncWidth+PanelHSyncBackPorch)
#define PanelVStart     (PanelVSyncWidth+PanelVSyncBackPorch)
#define PanelWidth      800
#define PanelHeight     600

#define PanelHTotal     1056
#define PanelVTotal     630

#define PanelMaxHTotal  1300
#define PanelMinHTotal  1035

#define PanelMaxVTotal  750
#define PanelMinVTotal  610

#define PanelDCLK       (((DWORD)PanelHTotal*PanelVTotal*60)/1000000)
#define PanelMaxDCLK    49
#define PanelMinDCLK    39
#endif
//=================================================================
// TCON  setting for LVDS
//PVS / FSYNC
#define GPO9_VStart     0x7
#define GPO9_VEnd       0x705
#define GPO9_HStart     0x0
#define GPO9_HEnd       0x0
#define GPO9_Control    0x7

//HSYNC
#define GPOA_VStart     0x006
#define GPOA_VEnd       0x408
#define GPOA_HStart     0
#define GPOA_HEnd       0
#define GPOA_Control    0x4

#define PanelVfreq      (600)

//Color Temp
#define DefCool_RedColor        117//114//111
#define DefCool_GreenColor      110//114
#define DefCool_BlueColor       127//130//117
#define DefNormal_RedColor      126//127
#define DefNormal_GreenColor    121//125
#define DefNormal_BlueColor     128//127
#define DefWarm_RedColor        130
#define DefWarm_GreenColor      126//132
#define DefWarm_BlueColor       128//130
#define DefsRGB_RedColor        DefWarm_RedColor
#define DefsRGB_GreenColor      DefWarm_GreenColor
#define DefsRGB_BlueColor       DefWarm_BlueColor
#endif


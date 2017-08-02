#ifndef __PANEL_CMIM236HGJ_L21__
#define __PANEL_CMIM236HGJ_L21__
#define PanelNumber             1800
#define PanelName               "PanelCMIM236HGJ L21"

#define PanelDither         8
#define DITHER_COEFF        0x2D
#define DITHER_METHOD       0x42

#define PanelTTL    0
#define PanelTCON   0
#define PanelLVDS   0xff
#define PanelRSDS   0
#define PanelminiLVDS      0

#define ShortLineCheck    0
#define LVDS_TIMode   0xff
#define PanelDualPort   0xff
//////////////Select Panel Type/////////////
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
#if 0
#define LVDS_2CH_A_OUT			LVDS_2CH_A_OUT	
#define LVDS_2CH_B_OUT			LVDS_2CH_A_OUT
#define LVDS_2CH_C_OUT			0x000
#define LVDS_2CH_D_OUT			0x000
#define LVDS_2CH_E_OUT			0x000	
#define LVDS_2CH_F_OUT			0x000
#define LVDS_2CH_G_OUT			0x000
#define LVDS_2CH_H_OUT			0x000

#define LVDS_CH_A_SWAP              LVDS_CH_C  
#define LVDS_CH_B_SWAP              LVDS_CH_D
#define LVDS_CH_C_SWAP              LVDS_CH_E
#define LVDS_CH_D_SWAP              LVDS_CH_A 

#define LVDS_CH_E_SWAP              LVDS_CH_B 
#define LVDS_CH_F_SWAP              LVDS_CH_F
#define LVDS_CH_G_SWAP              LVDS_CH_G
#define LVDS_CH_H_SWAP              LVDS_CH_H 
#endif
#if 0
// driving current setting 0==>4mA, 1==>6mA, 2==>8mA ,3==>12mA
#define PanelDCLKCurrent    1 // Dclk current
#define PanelDECurrent      1 // DE signal current
#define PanelOddDataCurrent 1 // Odd data current
#define PanelEvenDataCurrent    1 // Even data current
#define PanelOnTiming0      15//20 // time between 3.3v & 12v while turn on power
#define PanelOnTiming1      25//50//20//15//20//Delay1ms * 2 already. 60//10 //80 // time between panel & data while turn on power
#define PanelOnTiming2      200//180 time between data & back light while turn on power
#define PanelOffTiming0     10//20 // time between 3.3v & 12v while turn on power
#define PanelOffTiming1     100//10//80//Delay1ms * 2 already. 60//200 // time between back light & data while turn off power
#define PanelOffTiming2     25//10//80//Delay1ms * 2 already. 60//10 // time between data & panel while turn off power

#define PanelHSyncWidth        32
#define PanelHSyncBackPorch 40

#define PanelVSyncWidth        2
#define PanelVSyncBackPorch 34

#define PanelHStart     (PanelHSyncWidth+PanelHSyncBackPorch)
#define PanelVStart     (PanelVSyncWidth+PanelVSyncBackPorch)
#define PanelWidth      1280
#define PanelHeight     720
#define PanelHTotal     1440
#define PanelVTotal     758

#define PanelMaxHTotal      1600
#define PanelMinHTotal      1440//1520//1400//1340
#define PanelMaxVTotal      770
#define PanelMinVTotal      730
#define PanelDCLK           65
#define PanelMaxDCLK        91
#define PanelMinDCLK        54

#define HV_OUTPUT_TYPE          HSRM_B

// 111003 Rick add
#define PanelVfreq          (600)
#define PanelMaxVfreq   (750+20) // 75Hz
#define PanelMinVfreq   (500-20) // 50Hz

#elif 1 // 1080P output

// driving current setting 0==>4mA, 1==>6mA, 2==>8mA ,3==>12mA
#define PanelDCLKCurrent  1 // Dclk current
#define PanelDECurrent    1 // DE signal current
#define PanelOddDataCurrent 1 // Odd data current
#define PanelEvenDataCurrent  1 // Even data current
#define PanelOnTiming0    2 // time between 3.3V - 12V
#define PanelOnTiming1    25  // time between panel & data while turn on power
#define PanelOnTiming2    550 // time between data & back light while turn on power  //20050701
#define PanelOffTiming1    100 // time between back light & data while turn off power
#define PanelOffTiming2    10//25 // time between data & panel while turn off power
#define PanelOffTiming0 0 // time between 12V - 3.3V
#define PanelHSyncWidth   38//22//24
#define PanelHSyncBackPorch 41
#define PanelVSyncWidth         4
#define PanelVSyncBackPorch     31
#define PanelHStart   (PanelHSyncWidth+PanelHSyncBackPorch)
#define PanelVStart   (PanelVSyncWidth+PanelVSyncBackPorch)
#define PanelWidth    1920
#define PanelHeight   1080
#define PanelHTotal   2240
#define PanelVTotal   1125
#define PanelMaxHTotal     2300
#define PanelMinHTotal     2100
#define PanelMaxVTotal     1136
#define PanelMinVTotal    1115
#define PanelDCLK            148 // 74.25
#define PanelMaxDCLK      196 // 98
#define PanelMinDCLK       117 // 58.54
#define HV_OUTPUT_TYPE          HSRM_B

// 111003 Rick add
#define PanelVfreq          (600)
#define PanelMaxVfreq   (750+20) // 75Hz
#define PanelMinVfreq   (500-20) // 50Hz

#else   // 4k@30 2 chann output

#define PanelDCLKCurrent  1 // Dclk current
#define PanelDECurrent    1 // DE signal current
#define PanelOddDataCurrent 1 // Odd data current
#define PanelEvenDataCurrent  1 // Even data current
#define PanelOnTiming0    2 // time between 3.3V - 12V
#define PanelOnTiming1    25  // time between panel & data while turn on power
#define PanelOnTiming2    550 // time between data & back light while turn on power  //20050701
#define PanelOffTiming1    100 // time between back light & data while turn off power
#define PanelOffTiming2    10//25 // time between data & panel while turn off power
#define PanelOffTiming0 0 // time between 12V - 3.3V
#define PanelHSyncWidth         88//38//22//24
#define PanelHSyncBackPorch     255//41
#define PanelVSyncWidth         5 // 3
#define PanelVSyncBackPorch     77//21 // 46
#define PanelHStart         (PanelHSyncWidth+PanelHSyncBackPorch)
#define PanelVStart         (PanelVSyncWidth+PanelVSyncBackPorch)
#define PanelWidth          (1920*2)//3840
#define PanelHeight         2160
#define PanelHTotal         (2200*2) //4280 //4400
#define PanelVTotal         2250
#define PanelMaxHTotal      4560//4800
#define PanelMinHTotal      4240//4336
#define PanelMaxVTotal      2790//3000
#define PanelMinVTotal      2200//2218
#define PanelDCLK           148*2//(74*8)
#define PanelMaxDCLK        158*2//(80*8)//(76*8)
#define PanelMinDCLK        138*2//(70*8)//(69*8)
#define HV_OUTPUT_TYPE          HSRM_B

#define PanelVfreq          (300)
#define PanelMaxVfreq       (300+20)
#define PanelMinVfreq       (300-20)

#endif
//=================================================================
// TCON  setting for RSDS
// TCON  setting for RSDS
#define TCON_Format1  0x1 // OFC1: control polarity & inversion
#define ESP_AfterData   0 // Even Start Pulse after Data
#define ESP_Offset  1 // Even start pulse position
#define OSP_AfterData   0 // Odd Start Pulse after Data
#define OSP_Offset  1 // Odd start pulse position
// driving current setting 0==>4mA, 1==>6mA, 2==>8mA ,3==>12mA
#define PanelOESPCurrent  2 // OSP/ESP drive current
#define PanelOEGCurrent   2 // OPOL/EPOL/GPOL drive current
#define PanelEINVCurrent  0 // EINV drive current
#define PanelOINVCurrent  0 // OINV drive current
#define OuputFormat   (BIT3|BIT5)//Enable Auto Toggle GPO0 & data invert
// POL
#define GPO0_VStart   0x000
#define GPO0_VEnd   0x000
#define GPO0_HStart   0x186 //0x068
#define GPO0_HEnd   0x186 //0x222
#define GPO0_Control    0x02 //0x02 //0x82
//LP
#define GPO1_VStart   0
#define GPO1_VEnd   0
#define GPO1_HStart   0x406 //0x407
#define GPO1_HEnd   0x41b //0x428
#define GPO1_Control  0x00
// STV
#define GPO2_VStart   0x000
#define GPO2_VEnd   0x001
#define GPO2_HStart   0x166 //x170
#define GPO2_HEnd   0x166 //x170
#define GPO2_Control  0x00
// CLKV
#define GPO3_VStart   0
#define GPO3_VEnd   0x00 //0x300
#define GPO3_HStart   0x166 //0x173
#define GPO3_HEnd   0x406 //0x407
#define GPO3_Control  0x01
// OE
#define GPO4_VStart   0
#define GPO4_VEnd   0
#define GPO4_HStart   0x384 //0x357
#define GPO4_HEnd   0x442 //0x427
#define GPO4_Control  0x00
//PVS / FSYNC
#define GPO9_VStart   0 //0x002
#define GPO9_VEnd   0 //0x306
#define GPO9_HStart   0 //0x00A
#define GPO9_HEnd   0 //0x005
#define GPO9_Control  0 //0x08
//HSYNC
#define GPOA_VStart   0
#define GPOA_VEnd   0
#define GPOA_HStart   0
#define GPOA_HEnd   0
#define GPOA_Control  0

//=================================================================
// Color Temp
#define DefCool_RedColor            108
#define DefCool_GreenColor          128
#define DefCool_BlueColor           128
#define DefNormal_RedColor          118
#define DefNormal_GreenColor        128
#define DefNormal_BlueColor         128
#define DefWarm_RedColor            128
#define DefWarm_GreenColor      128
#define DefWarm_BlueColor           128
#define DefsRGB_RedColor      DefWarm_RedColor
#define DefsRGB_GreenColor  DefWarm_GreenColor
#define DefsRGB_BlueColor     DefWarm_BlueColor
//==================================================================
//LED Backlight Control
#if 0 // power measurement temply
#define LED_CH_0    1
#define LED_CH_1    1
#define LED_CH_2    1
#define LED_CH_3    1
#define LEDBL_PanelDefaultCurrent           110//50//5o for LG set , 110            //need set by panel spec
#define PanelLightBarDefaultVoltage         40          //need set by panel spec
#define PanelLightBarMaximumVoltage     52//52          //need set by panel spec
#else // product panel
#define LED_CH_0    1
#define LED_CH_1    1
#define LED_CH_2    1
#define LED_CH_3    1
#define LEDBL_PanelDefaultCurrent           50//110         //need set by panel spec
#define PanelLightBarDefaultVoltage         44          //need set by panel spec
#define PanelLightBarMaximumVoltage     52          //need set by panel spec
#endif

#endif


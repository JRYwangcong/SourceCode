#ifndef __PANEL_SHARP_ULTRA_HD__
#define __PANEL_SHARP_ULTRA_HD__
#define PanelNumber             1800
#define PanelName               "HV320QHM"

#define PanelSize                  PanelSize32W

#define PanelSize_Num           "320"

#define PanelDither         8
#define DITHER_COEFF        0x2D
#define DITHER_METHOD       0x42

#define PanelTTL            0
#define PanelTCON           0
#define PanelLVDS           0xff
#define PanelRSDS           0
#define PanelminiLVDS       0

#define ShortLineCheck      0
#define LVDS_TIMode         0//0xff
#define PanelDualPort       0xff
//////////////Select Panel Type/////////////
#define PANEL_TTL               0
#define PANEL_LVDS_1CH          0
#define PANEL_LVDS_2CH          0
#define PANEL_LVDS_4CH          0
#define PANEL_LVDS_8CH          0
#define PANEL_VBY1_1CH_8Bit     0
#define PANEL_VBY1_1CH_10Bit    0
#define PANEL_VBY1_2CH_8Bit     0
#define PANEL_VBY1_2CH_10Bit    0
#define PANEL_VBY1_4CH_8Bit     0
#define PANEL_VBY1_4CH_10Bit    0
#define PANEL_VBY1_8CH_8Bit     0
#define PANEL_VBY1_8CH_10Bit    0
#define PANEL_VBY1()       ((PANEL_VBY1_1CH_8Bit)  ||  \
                            (PANEL_VBY1_1CH_10Bit) ||  \
                            (PANEL_VBY1_2CH_8Bit)  ||  \
                            (PANEL_VBY1_2CH_10Bit) ||  \
                            (PANEL_VBY1_4CH_8Bit)  ||  \
                            (PANEL_VBY1_4CH_10Bit) ||  \
                            (PANEL_VBY1_8CH_8Bit)  ||  \
                            (PANEL_VBY1_8CH_10Bit) )
#define PANEL_EDP               1


typedef enum
{
eDPTXHPD_USE_GPIO50=0,
eDPTXHPD_USE_GPIO51,
eDPTXHPD_USE_GPIO52,
eDPTXHPD_USE_GPIO53,
}eDPTXHPD_USE_GPIO_GP0_TYP;
typedef enum
{
eDPTXHPD_USE_GPIO54=0,
eDPTXHPD_USE_GPIO55,
eDPTXHPD_USE_GPIO56,
eDPTXHPD_USE_GPIO57,
}eDPTXHPD_USE_GPIO_GP1_TYP;
typedef enum
{
eDPTXHPDUseGPIOGroup0=0,
eDPTXHPDUseGPIOGroup1,
}eDPTXHPD_USE_GPIO_Group_TYP;
////////////////////////////////////////////////////////////
// ####  Fix Parameter for eDPTX (Do Not Change this Parameter) ####
////////////////////////////////////////////////////////////
#define eDPTX_HBR2 0x14
#define eDPTX_HBR   0x0A
#define eDPTX_RBR   0x06
#define eDPTX_4Lanes  4
#define eDPTX_2Lanes  2
#define eDPTX_1Lanes  1
////////////////////////////////////////////////////////////
// #######   Configure for different eDP Panel  ####################
////////////////////////////////////////////////////////////
#if PANEL_EDP

#define eDPTXHBR2Enable 1  // set 1 when use MST9U3 chip && eDPTX ouput in HBR2 path

////////////////////////////////////////////////////////////
// ####  Fix Parameter for eDPTX (Do Not Change this Parameter) ####
////////////////////////////////////////////////////////////
#define eDPTXHBR2PortEn  ((CHIP_ID==MST9U3) && eDPTXHBR2Enable)
////////////////////////////////////////////////////////////

//###########################################################
#if eDPTXHBR2PortEn    // For HBR2
//###########################################################
//###########################################################
#define eDPTXHBR2PNSWAP   0    // HBR2Port#  (ch0~ch3) PN swap
#define eDPTXHBR2LaneSWAP 0 // HBR2Port# (ch0~ch3) lane swap
#define eDPTXAuxP0PNSWAP 1//0  // AuxPort#0  PN swap
#define eDPTXAuxP1PNSWAP 1  // AuxPort#1 PN swap
#define eDPTXAuxP2PNSWAP 1  // AuxPort#2  PN swap
#define eDPTXAuxP3PNSWAP 1  // AuxPort#3 PN swap
// //////// config ML<->AUX<->HPD Port Mapping ///////////////
#define eDPTXHBR2PUseAUXPortNum 1 // 0~3
#define eDPTXHBR2PUseHPDPortNum 0 // 0~1
////////////////////////////////////////////////////////////
#define eDP_HBR2_SwTBL_ShiftLv                2///1         // set 1 means => sw Lv1 shift to Lv0 , Lv2 shift to Lv1, Lv3 shift to Lv2 ,Lv3=Lv2
                                                                                    // set 2 means => sw Lv2 shift to Lv0 , Lv3 shift to Lv1, Lv3=Lv2=Lv1 
////////////////////////////////////////////////////////////
#define eDP_SWING_DefaultValue                  0x2E   // 0x00~0x3F
#define eDP_PREMPHASIS_DefaultValue      0x00   // 0x00~0x07
////////////////////////////////////////////////////////////
#define eDPTXLinkRate               eDPTX_HBR2       // eDPTX_RBR
#define eDPTXLaneCount            eDPTX_4Lanes   // eDPTX_2Lanes  //eDPTX_1Lanes
#define eDPTXDETECTHPD       1 // 1=enable TX detect HPD
#define eDPTXHPDUseGPIOGroupNum   eDPTXHPDUseGPIOGroup0 // 0=GPIO50~53 ; 1=GPIO54~57
#define eDPTXHPDPort0             eDPTXHPD_USE_GPIO51
#define eDPTXHPDPort1             eDPTXHPD_USE_GPIO51  // if  project only use One HPD port => please define HPDPort1 = HPDPort0
////////////////////////////////////////////////////////////
#define eDPTX10Bits 1
#define eDPTXEnSSC                          0
////// choose either method#3a or method#3b ////////////////////
#define eDPTXEnASSR                        0     // method  #3a
#define eDPTXEnAFR                           0    // method  #3b
////// choose support No Aux HandShake Link Training  ///////////
#define eDPTX_NO_AUX_HANDSHAKE_LINK_TRAINING  0  // 
///////////////////////////////////////////////////////////
#define eDPTXEnPG 0
#define eDPTXMODPhyCalEn 0
////// choose support MCCS Cmd ////////////////////////////
#define eDPTxAuxMCCSEn 0
///##############################################################
#else  // for MOD HBR/RBR  L/R  2PORTs
//##############################################################
#define eDPTXMODOnePort 0 // if MOD play in Only One eDPTX Port , Set 1 to support  it.
#define eDPTXSWAP          0
#define eDPTXP0PNSWAP eDPTXSWAP  // Port#0  (ch8~ch11) PN swap
#define eDPTXP1PNSWAP eDPTXSWAP  // Port#1 (ch12~ch15) PN swap
#define eDPTXP0LaneSWAP eDPTXSWAP // Port#0 (ch8~ch11) lane swap
#define eDPTXP1LaneSWAP eDPTXSWAP // Port#1 (ch11~ch15) lane swap
#define eDPTXPortSWAP         0 // eDP Port#0 <-> Port#1 swap == LR SWAP
#if (MS_BOARD_TYPE_SEL == BD_MST9UHL_V3)
#define eDPTXAuxP0PNSWAP 1  // AuxPort#0  PN swap
#define eDPTXAuxP1PNSWAP 1  // AuxPort#1 PN swap
#define eDPTXAuxP2PNSWAP 1  // AuxPort#2  PN swap
#define eDPTXAuxP3PNSWAP 1  // AuxPort#3 PN swap
#else
#define eDPTXAuxP0PNSWAP 0  // AuxPort#0  PN swap
#define eDPTXAuxP1PNSWAP 0  // AuxPort#1 PN swap
#define eDPTXAuxP2PNSWAP 0  // AuxPort#2  PN swap
#define eDPTXAuxP3PNSWAP 0  // AuxPort#3 PN swap
#endif
// //////// config ML<->AUX<->HPD Port Mapping ///////////////
#if (CHIP_ID==MST9U3)
#define eDPTXP0UseAUXPortNum 1 // 0~3
#define eDPTXP0UseHPDPortNum 0 // 0~1
#define eDPTXP1UseAUXPortNum 0 // 0~3
#define eDPTXP1UseHPDPortNum 1 // 0~1
#else
#define eDPTXP0UseAUXPortNum 0 // 0~3
#define eDPTXP0UseHPDPortNum 0 // 0~1
#define eDPTXP1UseAUXPortNum 1 // 0~3
#define eDPTXP1UseHPDPortNum 1 // 0~1
#endif
////////////////////////////////////////////////////////////
#define eDP_SWING_DefaultValue                  0x3B   // 0x00~0x3F
#define eDP_PREMPHASIS_DefaultValue      0x00   // 0x00~0x07
////////////////////////////////////////////////////////////
#define eDPTXLinkRate               eDPTX_HBR       // eDPTX_RBR
#define eDPTXLaneCount            eDPTX_4Lanes   // eDPTX_2Lanes  //eDPTX_1Lanes
#define eDPTXDETECTHPD       1 // 1=enable TX detect HPD
#define eDPTXHPDUseGPIOGroupNum   eDPTXHPDUseGPIOGroup1 // 0=GPIO50~53 ; 1=GPIO54~57
#define eDPTXHPDPort0             eDPTXHPD_USE_GPIO51
#define eDPTXHPDPort1             eDPTXHPD_USE_GPIO51
////////////////////////////////////////////////////////////
#define eDPTX10Bits                          0
#define eDPTXEnSSC                          0
////// choose either method#3a or method#3b ////////////////////
#define eDPTXEnASSR                        0     // method  #3a
#define eDPTXEnAFR                           0    // method  #3b
////// choose support No Aux HandShake Link Training  ///////////
#define eDPTX_NO_AUX_HANDSHAKE_LINK_TRAINING  0  // 
////////////////////////////////////////////////////////////
#define eDPTXEnPG                            0
#define eDPTXMODPhyCalEn              0                        // Set 1 for MP
////// choose support MCCS Cmd ////////////////////////////
#define eDPTxAuxMCCSEn 0

#endif

#endif



////////Select Panel Output Format/////////
#define PANEL_OUTPUT_FMT_LR     0//xFF
#define PANEL_OUTPUT_FMT_4B     0

#define PanelSwapPort       0//0xff
#define PanelSwapOddML      0
#define PanelSwapEvenML     0
#define PanelSwapOddRB      0
#define PanelSwapEvenRB     0
#define PanelSwapMLSB       0
#define PanelDClkDelay      8
#define PanelInvDE          0
#define PanelInvDClk        0
#define PanelInvHSync       0
#define PanelInvVSync       0

#define PANEL_SWAP_PN      0xFF
#define PANEL_SWAP_PN_LOCKN     0x00
#define PANEL_VBY1_HW_MODE       0
//==================================================================
#define LVDS_CH_A_SWAP              LVDS_CH_A
#define LVDS_CH_B_SWAP              LVDS_CH_C
#define LVDS_CH_C_SWAP              LVDS_CH_B
#define LVDS_CH_D_SWAP              LVDS_CH_D
#define LVDS_CH_E_SWAP              LVDS_CH_E
#define LVDS_CH_F_SWAP              LVDS_CH_G
#define LVDS_CH_G_SWAP              LVDS_CH_F
#define LVDS_CH_H_SWAP              LVDS_CH_H

#define VBY1_SWPATH

//==================================================================

// driving current setting 0==>4mA, 1==>6mA, 2==>8mA ,3==>12mA
#define PanelDCLKCurrent        1 // Dclk current
#define PanelDECurrent          1 // DE signal current
#define PanelOddDataCurrent     1 // Odd data current
#define PanelEvenDataCurrent    1 // Even data current
#define PanelOnTiming0          2 // time between 3.3V - 12V
#define PanelOnTiming1          25  // time between panel & data while turn on power
#define PanelOnTiming2          550 // time between data & back light while turn on power  //20050701
#define PanelOffTiming1         100 // time between back light & data while turn off power
#define PanelOffTiming2         10//25 // time between data & panel while turn off power
#define PanelOffTiming0         0 // time between 12V - 3.3V

#define PanelHSyncWidth           32
#define PanelHSyncBackPorch     48
#define PanelVSyncWidth             3
#define PanelVSyncBackPorch     21

#define PanelHStart         (PanelHSyncWidth+PanelHSyncBackPorch)
#define PanelVStart         (PanelVSyncWidth+PanelVSyncBackPorch)

#define PanelWidth          2560
#define PanelHeight         1440
#define PanelHTotal         2720
#define PanelVTotal         1481

#define PanelMaxHTotal      2760
#define PanelMinHTotal       PanelHTotal
#define PanelMaxVTotal      2430
#define PanelMinVTotal       1470

#define PanelDCLK              (298)
#define PanelMaxDCLK        (575)
#define PanelMinDCLK        (189)
#define HV_OUTPUT_TYPE      HSRM_B

#define PanelVfreq              (1200)
#define PanelMaxVfreq       (1460)
#define PanelMinVfreq        (480) // 50Hz

//==================================================================

#if PANEL_EDP
// ##### eDPTiming index ###############################
#define eDP480P 0
#define eDP720P 1
#define eDP1080P 2
#define eDP2K2K 3
#define eDP2K2KR 4
#define eDPanelDefine 5
// ##### eDPTiming define ###############################
#define eDPTiming eDPanelDefine // eDP2K2KR // eDP2K2K  // eDP1080P //eDP720P //eDP480P
// ##### eDPTiming MSA ###############################
#if (eDPTiming==eDPanelDefine)
#if eDPTXHBR2PortEn
#define eDPTXMSA_Htotal      PanelHTotal // Htotal
#define eDPTXMSA_Hwidth    PanelWidth // Hactive
#else
#if eDPTXMODOnePort
#define eDPTXMSA_Htotal      PanelHTotal // Htotal
#define eDPTXMSA_Hwidth    PanelWidth // Hactive
#else
#if PANEL_OUTPUT_FMT_4B
#define eDPTXMSA_Htotal      PanelHTotal/4 // Htotal
#define eDPTXMSA_Hwidth    PanelWidth/4 // Hactive
#else
#define eDPTXMSA_Htotal      PanelHTotal/2 // Htotal
#define eDPTXMSA_Hwidth    PanelWidth/2 // Hactive
#endif
#endif
#endif
#define eDPTXMSA_HStart     PanelHSyncWidth+PanelHSyncBackPorch // HStart  == HSW+HBACKPORCH
#define eDPTXMSA_HSP         1           // HSP   [15]=Polarity  set 0 or 1
#define eDPTXMSA_HSW        PanelHSyncWidth // Hsw 20  [15]=Polarity
#define eDPTXMSA_Vtotal      PanelVTotal // Vtotal
#define eDPTXMSA_VStart     PanelVSyncWidth+PanelVSyncBackPorch // Vstart == VSW+VBACKPORCH
#define eDPTXMSA_VSP         0           // VSP   [15]=Polarity  set 0 or 1
#define eDPTXMSA_VSW       PanelVSyncWidth // Vsw   [15]=Polarity
#define eDPTXMSA_Vheight   PanelHeight // Vactive
#elif (eDPTiming==eDP2K2KR)
#define eDPTXMSA_Htotal      0x0800 // Htotal 2048
#define eDPTXMSA_HStart     0x0028 // HStart 40  == HSW+HBACKPORCH
#define eDPTXMSA_HSP         1           // HSP   [15]=Polarity
#define eDPTXMSA_HSW        0x0014 // Hsw 20  [15]=Polarity
#define eDPTXMSA_Vtotal      0x0898 // Vtotal  2200
#define eDPTXMSA_VStart     0x0008 // Vstart  8 == VSW+VBACKPORCH
#define eDPTXMSA_VSP         0           // VSP   [15]=Polarity
#define eDPTXMSA_VSW        0x0005 // Vsw   [15]=Polarity
#define eDPTXMSA_Hwidth    0x0780 // Hactive 1920
#define eDPTXMSA_Vheight  0x0870 // Vactive 2160
#elif  (eDPTiming==eDP2K2K)
#define eDPTXMSA_Htotal      0x0820 // Htotal 2080
#define eDPTXMSA_HStart     0x0058 // HStart 88 == HSW+HBACKPORCH
#define eDPTXMSA_HSP         1           // HSP   [15]=Polarity
#define eDPTXMSA_HSW        0x002C // Hsw 44  [15]=Polarity
#define eDPTXMSA_Vtotal      0x08CA // Vtotal  2250
#define eDPTXMSA_VStart     0x0008 // Vstart  8 == VSW+VBACKPORCH
#define eDPTXMSA_VSP         0           // VSP   [15]=Polarity
#define eDPTXMSA_VSW        0x0005 // Vsw   [15]=Polarity
#define eDPTXMSA_Hwidth    0x0780 // Hactive 1920
#define eDPTXMSA_Vheight  0x0870 // Vactive 2160
#elif (eDPTiming==eDP1080P)
#define eDPTXMSA_Htotal      0x0898 // Htotal 2220
#define eDPTXMSA_HStart     0x0058 // HStart 88 == HSW+HBACKPORCH
#define eDPTXMSA_HSP         1           // HSP   [15]=Polarity
#define eDPTXMSA_HSW        0x002C // Hsw 44  [15]=Polarity
#define eDPTXMSA_Vtotal      0x0465 // Vtotal  1125
#define eDPTXMSA_VStart     0x0010 // Vstart  16 == VSW+VBACKPORCH
#define eDPTXMSA_VSP         0           // VSP   [15]=Polarity
#define eDPTXMSA_VSW        0x0005 // Vsw   [15]=Polarity
#define eDPTXMSA_Hwidth    0x0780 // Hactive 1920
#define eDPTXMSA_Vheight  0x0438 // Vactive 1080
#elif (eDPTiming==eDP720P)
#define eDPTXMSA_Htotal      0x0672 // Htotal 1650
#define eDPTXMSA_HStart     0x006E // HStart 110 == HSW+HBACKPORCH
#define eDPTXMSA_HSP         1           // HSP   [15]=Polarity
#define eDPTXMSA_HSW        0x0028 // Hsw 40  [15]=Polarity
#define eDPTXMSA_Vtotal      0x02EE // Vtotal  750
#define eDPTXMSA_VStart     0x0008 // Vstart  8 == VSW+VBACKPORCH
#define eDPTXMSA_VSP         0           // VSP   [15]=Polarity
#define eDPTXMSA_VSW        0x0005 // Vsw   [15]=Polarity
#define eDPTXMSA_Hwidth    0x0500 // Hactive 1280
#define eDPTXMSA_Vheight  0x02D0 // Vactive  720
#elif (eDPTiming==eDP480P)
#define eDPTXMSA_Htotal      0x035A // Htotal 858
#define eDPTXMSA_HStart     0x0010 // HStart 16 == HSW+HBACKPORCH
#define eDPTXMSA_HSP         1           // HSP   [15]=Polarity
#define eDPTXMSA_HSW        0x003E // Hsw 62  [15]=Polarity
#define eDPTXMSA_Vtotal      0x020D // Vtotal  525
#define eDPTXMSA_VStart     0x0009 // Vstart  9 == VSW+VBACKPORCH
#define eDPTXMSA_VSP         1           // VSP   [15]=Polarity
#define eDPTXMSA_VSW        0x0006 // Vsw   [15]=Polarity
#define eDPTXMSA_Hwidth    0x01E0 // Hactive 480
#define eDPTXMSA_Vheight  0x02D0 // Vactive  720
#endif
#define eDPTXMSA_HB         (eDPTXMSA_Htotal)-(eDPTXMSA_Hwidth)  // HB   Blanking
#define eDPTXMSA_VB         (eDPTXMSA_Vtotal)-(eDPTXMSA_Vheight) // VB   Blanking
#define eDPTXMSA_HFP         (eDPTXMSA_HB)-(PanelHStart)  // H Front porch
#define eDPTXMSA_VFP         (eDPTXMSA_VB)-(PanelVStart) // VFront porch
#define eDPTXMSA_HBP         PanelHSyncBackPorch  // H  Back porch
#define eDPTXMSA_VBP         PanelVSyncBackPorch // V Back porch
#if eDPTX10Bits
#define eDPTXMSAColorMISC BIT6  // [7:5]=  000=6bits  / 001=8bits / 010 = 10bits  / 011= 12bits  /  100=16bits
#else
#define eDPTXMSAColorMISC BIT5  // [7:5]=  000=6bits  / 001=8bits / 010 = 10bits  / 011= 12bits  /  100=16bits
#endif
#endif


//=================================================================
// TCON  setting for RSDS
// TCON  setting for RSDS
#define TCON_Format1    0x1 // OFC1: control polarity & inversion
#define ESP_AfterData   0 // Even Start Pulse after Data
#define ESP_Offset      1 // Even start pulse position
#define OSP_AfterData   0 // Odd Start Pulse after Data
#define OSP_Offset      1 // Odd start pulse position
// driving current setting 0==>4mA, 1==>6mA, 2==>8mA ,3==>12mA
#define PanelOESPCurrent  2 // OSP/ESP drive current
#define PanelOEGCurrent   2 // OPOL/EPOL/GPOL drive current
#define PanelEINVCurrent  0 // EINV drive current
#define PanelOINVCurrent  0 // OINV drive current
#define OuputFormat   (BIT3|BIT5)//Enable Auto Toggle GPO0 & data invert
// POL
#define GPO0_VStart     0x000
#define GPO0_VEnd       0x000
#define GPO0_HStart     0x186 //0x068
#define GPO0_HEnd       0x186 //0x222
#define GPO0_Control    0x02 //0x02 //0x82
//LP
#define GPO1_VStart     0
#define GPO1_VEnd       0
#define GPO1_HStart     0x406 //0x407
#define GPO1_HEnd       0x41b //0x428
#define GPO1_Control    0x00
// STV
#define GPO2_VStart     0x000
#define GPO2_VEnd       0x001
#define GPO2_HStart     0x166 //x170
#define GPO2_HEnd       0x166 //x170
#define GPO2_Control    0x00
// CLKV
#define GPO3_VStart     0
#define GPO3_VEnd       0x00 //0x300
#define GPO3_HStart     0x166 //0x173
#define GPO3_HEnd       0x406 //0x407
#define GPO3_Control    0x01
// OE
#define GPO4_VStart     0
#define GPO4_VEnd       0
#define GPO4_HStart     0x384 //0x357
#define GPO4_HEnd       0x442 //0x427
#define GPO4_Control    0x00
//PVS / FSYNC
#define GPO9_VStart     0 //0x002
#define GPO9_VEnd       0 //0x306
#define GPO9_HStart     0 //0x00A
#define GPO9_HEnd       0 //0x005
#define GPO9_Control    0 //0x08
//HSYNC
#define GPOA_VStart     0
#define GPOA_VEnd       0
#define GPOA_HStart     0
#define GPOA_HEnd       0
#define GPOA_Control    0

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




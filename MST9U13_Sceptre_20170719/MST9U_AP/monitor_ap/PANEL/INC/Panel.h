///////////////////////////////////////////////////////////////////////////////
/// @file panel.h
/// @brief Head file for panel header file switch.
/// @author MStarSemi Inc.
///
///
///
/// Features
///  -
///  -
///  -
///  -
///////////////////////////////////////////////////////////////////////////////
#ifndef __PANEL__
#define __PANEL__
//#include "Board.h"
#include "Panel_define.h"
#include "datatype.h"


#if (PanelType==PanelM156B1L01)
#include "PanelM156B1L01.h"

#elif (PanelType==PanelM190A1L02)
#include "PanelCMOM190A1L02.h"

#elif (PanelType==PanelM185B1L01)
#include "PanelM185B1L01.h"

#elif (PanelType==PanelLM185WH1TLA1)
#include "PanelLM185WH1TLA1.h"

#elif (PanelType==PanelM216H1L01)
#include "PanelM216H1L01.h"

#elif (PanelType==PanelLM215WF1)
#include "PanelLM215WF1.h"

#elif (PanelType==PanelLM220WE1)
#include "PanelLM220WE1.h"

#elif (PanelType==PanelLTM200KT01)
#include "PanelLTM200KT01.h"

#elif (PanelType==PanelLM230WF1)
#include "PanelLM230WF1.h"

#elif (PanelType==PanelTPM200O1L02)
#include "PanelTPM200O1L02.h"

#elif (PanelType==PanelCLAA185W)
#include "PanelCLAA185W.h"

#elif (PanelType==PanelLM200WD3)
#include "PanelLM200WD3.h"

#elif (PanelType==PanelM270HW01)
#include "PanelM270HW01.h"

#elif (PanelType==PanelLTM230HT03)
#include "PanelLTM230HT03.h"

#elif (PanelType==PanelLTM200KT07)
#include "PanelLTM200KT07.h"

#elif (PanelType==PanelTPM215HW01_HGE)      //2011.9.16 cc add
#include "PanelTPM215HW01_HGE.h"

#elif (PanelType==PanelLM230WF5TLD1)
#include "PanelLM230WF5TLD1.h"

#elif (PanelType==PanelAUOM215HW01VB)
#include "PanelAUOM215HW01VB.h"

#elif (PanelType==PanelCMIM236HGJ_L21)
#include "PanelCMIM236HGJ_L21.h"

#elif (PanelType==PanelLTM220M1L09) //111012 Modify
#include "PanelLTM220M1L09.h"

#elif (PanelType==PanelLM215WF3_S2A2)
#include "PanelLM215WF3_S2A2.h"

#elif (PanelType==PanelBM230WF3_SJC2)
#include "PanelBM230WF3_SJC2.h"

#elif (PanelType==PanelAUOM240UW01)
#include "PanelAUOM240UW01.h"

#elif (PanelType==PanelAU20SVGA)
#include "PanelAU20SVGA.h"

#elif (PanelType==PanelCMO190)
#include "PanelCMO190.h"

#elif (PanelType==PanelSharpUltraHD)
#include "PanelSharpUltraHD.h"

#elif (PanelType==PanelAUOUltraHD_eDP)
#include "PanelAUOUltraHD_eDP.h"

#elif (PanelType==PanelAU185)
#include "PanelAU185.h"

#elif (PanelType==PanelCM27HH1A120HZ)
#include "PanelCM27HH1A_120HZ.h"

#elif (PanelType==PanelLGLM340UW1)
#include "PanelLGLM340UW1.h"

#elif (PanelType==PanelSAM_LTM27_2560)
#include "PanelLTM27_2560.h"

#elif (PanelType==PanelCMI28UltraHD)
#include "PanelCMI28UltraHD.h"

#elif (PanelType==PanelLGLM238_4K2K)
#include "PanelLGLM238_4K2K.h"

#elif (PanelType==PanelLGLM310_4K2K)
#include "PanelLGLM310_4K2K.h"

#elif (PanelType==PanelLGLM238WR1_UHD)
#include "PanelLGLM238WR1_UHD.h"

#elif (PanelType==PanelVby1ToEDP)
#include "PanelVby1ToEDP.h"

#elif (PanelType==PanelV400DK1_KS1)
#include "PanelV400DK1_KS1.h"

#elif (PanelType==PanelLC430EQE_FHM2)
#include "PanelLC430EQE_FHM2.h"

#elif (PanelType==PanelLC490EQE_XGF1)
#include "PanelLC490EQE_XGF1.h"

#elif (PanelType==PanelLSC400FP02)
#include "PanelLSC400FP02.h"

#elif (PanelType==PanelM270QAN01)
#include "PanelM270QAN01.h"

#elif (PanelType==PanelLC420EQE_PGM1)
#include "PanelLC420EQE_PGM1.h"

#elif (PanelType==PanelM320QAN01)
#include "PanelM320QAN01.h"

#elif (PanelType==PanelLTM340YP01)
#include "PanelLTM340YP01.h"

#elif (PanelType==PanelP550QVN01)
#include "PanelP550QVN01.h"

#elif (PanelType==PanelV500DJ2_KS5)
#include "PanelV500DJ2_KS5.h"

#elif (PanelType==PanelLTM315FL01)
#include "PanelLTM315FL01.h"

#elif (PanelType==PanelM315DJJ_K30)
#include "PanelM315DJJ_K30.h"

#elif (PanelType==PanelLSC400FN03)
#include "PanelLSC400FN03.h"

#elif (PanelType==PanelLSC400FN04)
#include "PanelLSC400FN04.h"

#elif (PanelType==PanelV400DK1_QS1)
#include "PanelV400DK1_QS1.h"

#elif (PanelType==PanelV500DK2_KS1)
#include "PanelV500DK2_KS1.h"

#elif (PanelType==PanelV580DK2_KS1)
#include "PanelV580DK2_KS1.h"

#elif (PanelType==PanelV650DK1_KS1)
#include "PanelV650DK1_KS1.h"

#elif (PanelType==PanelV420DK1_KS1)
#include "PanelV420DK1_KS1.h"

#elif (PanelType==PanelM350DVR01_01)
#include "PanelM350DVR01_01.h"

#elif (PanelType==PanelLC550EQE_PGF1)
#include "PanelLC550EQE_PGF1.h"

#elif (PanelType==PanelV390DK1_LS1)
#include "PanelV390DK1_LS1.h"

#elif (PanelType==PanelT500QVN02)
#include "PanelT500QVN02.h"

#elif (PanelType==PanelS550QF62V1_V2)
#include "PanelS550QF62V1_V2.h"

#elif (PanelType==PanelV400DJ1_QS5)
#include "PanelV400DJ1_QS5.h"

#elif (PanelType==PanelLM310UH1_SLA1)
#include "PanelLM310UH1_SLA1.h"

#elif (PanelType==PanelLM340UW1_SSA1)
#include "PanelLM340UW1_SSA1.h"

#elif (PanelType==PanelM270DTN01)
#include "PanelM270DTN01.h"

#elif (PanelType==PanelT430QVN01)
#include "PanelT430QVN01.h"

#elif (PanelType==PanelLSM315HP01)
#include "PanelLSM315HP01.h"

#elif (PanelType==PanelLC430EGY_SHA1)
#include "PanelLC430EGY_SHA1.h"

#elif (PanelType==PanelLC550EGY_SGM1)
#include "PanelLC550EGY_SGM1.h"

#elif (PanelType==PanelLSM315DP01)
#include "PanelLSM315DP01.h"

#elif (PanelType==PanelM315DVR011)
#include "PanelM315DVR011.h"

#elif (PanelType==PanelLSM270DP01)
#include "PanelLSM270DP01.h"

#elif (PanelType==PanelHV320QHM_C81)
#include "PanelHV320QHM_C81.h"

#elif (PanelType==PanelT550QVR05)
#include "PanelT550QVR05.h"

#elif (PanelType==PanelLM270WQ1)
#include "PanelLM270WQ1.h"

#elif (PanelType==PanelST5461D04_2)
#include "PanelST5461D04_2.h"

#elif (PanelType==PanelLM270WR2)
#include "PanelLM270WR2.h"

#elif (PanelType==PanelHV550QUB)
#include "PanelHV550QUB.h"

#elif (PanelType==PanelLSF490FJ02)
#include "PanelLSF490FJ02.h"

#elif (PanelType==PanelT500QVR05)
#include "PanelT500QVR05.h"

#elif (PanelType==PanelM238DCJ_E50)
#include "PanelM238DCJ.h"

#elif (PanelType==PanelM350DVA01_1)
#include "PanelM350DVA01_1.h"

#elif (PanelType==PanelLSM315DP02)
#include "PanelLSM315DP02.h"

#elif (PanelType==PanelLM270WQ1_SLA1)
#include "PanelLM270WQ1_SLA1.h"

#elif (PanelType==PanelLSM270HP06)
#include "PanelLSM270HP06.h"


#endif




#ifndef DITHER_COEFF
#define DITHER_COEFF    0x2D
#endif
#ifndef DITHER_METHOD
#define DITHER_METHOD   0x42
#endif

#ifndef PanelVfreq
#define PanelVfreq      (600)
#endif

#ifndef PanelMaxVfreq
#define PanelMaxVfreq   (750+12) // 75Hz
#endif

#ifndef PanelMinVfreq
#define PanelMinVfreq   (500-12) // 50Hz
#endif

#ifndef PanelMiniLVDS3
#define PanelMiniLVDS3  0
#endif




#ifndef PANEL_SSC_MODULATION_DEF
#if PANEL_VBY1()
#define PANEL_SSC_MODULATION_DEF    30 // unit: 1KHz, range 0~40 means 0~30KHz
#else
#define PANEL_SSC_MODULATION_DEF    40 // unit: 1KHz, range 0~40 means 0~40KHz
#endif
#endif

#ifndef PANEL_SSC_PERCENTAGE_DEF
#if PANEL_VBY1()
#if 0//(PanelType==PanelLTM315FL01)
#define PANEL_SSC_PERCENTAGE_DEF    1	// unit: 0.1%, range 0~30 means 0~0.5%
#else
#define PANEL_SSC_PERCENTAGE_DEF    2	// unit: 0.1%, range 0~30 means 0~0.5%
#endif
#else
#define PANEL_SSC_PERCENTAGE_DEF      20  // unit: 0.1%, range 0~30 means 0~3%
#endif
#endif

#ifndef PANEL_SSC_MODULATION_MAX
#if PANEL_VBY1()
#define PANEL_SSC_MODULATION_MAX    30 // unit: 1KHz, range 0~40 means 0~30KHz
#else
#define PANEL_SSC_MODULATION_MAX    40 // unit: 1KHz, range 0~40 means 0~40KHz
#endif
#endif

#ifndef PANEL_SSC_PERCENTAGE_MAX
#if PANEL_VBY1()
#define PANEL_SSC_PERCENTAGE_MAX      5 // unit: 0.1%, range 0~30 means 0~0.5%
#else
#define PANEL_SSC_PERCENTAGE_MAX      30 // unit: 0.1%, range 0~30 means 0~3%
#endif
#endif

#ifndef LVDS_VOLTAGE_TARGET
#define LVDS_VOLTAGE_TARGET  IGT_TARGET_250
#endif

#ifndef DISABLE_SHORT_LINE_TUNE
#define DISABLE_SHORT_LINE_TUNE    0
#endif

#ifndef DISABLE_SHORT_FRAME
#define DISABLE_SHORT_FRAME    0
#endif

#ifndef PANEL_SWAP_PN
#define PANEL_SWAP_PN       0
#endif

#ifndef PANEL_SWAP_PN_LOCKN
#define PANEL_SWAP_PN_LOCKN       0
#endif

#ifndef PANEL_VBY1_HW_MODE
#define PANEL_VBY1_HW_MODE       0
#endif


#ifndef PAIR_SWAP_B
#define PAIR_SWAP_B         0
#endif
#ifndef PANEL_SWAP_AB_PORT
#define PANEL_SWAP_AB_PORT  0 //0: non-swap, 0xFF: AB swap
#endif
#ifndef PANEL_SWAP_CD_PORT
#define PANEL_SWAP_CD_PORT  0//0: non-swap ,0xFF: CD swap
#endif

#ifndef PANEL_3D_PASSIVE
#define PANEL_3D_PASSIVE            0
#endif

#ifndef PANEL_3D_PASSIVE_4M
#define PANEL_3D_PASSIVE_4M         0
#endif

#ifndef PANEL_OUTPUT_FMT_3D_LR
#define PANEL_OUTPUT_FMT_3D_LR      0
#endif

#ifndef PANEL_OUTPUT_FMT_3D_CB
#define PANEL_OUTPUT_FMT_3D_CB      0
#endif

#ifndef PANEL_OUTPUT_FMT_3D_FS
#define PANEL_OUTPUT_FMT_3D_FS      0
#endif

#ifndef PANEL_OUTPUT_FMT_3D_TB
#define PANEL_OUTPUT_FMT_3D_TB      0
#endif

#ifndef PANEL_OUTPUT_FMT_3D_SBS
#define PANEL_OUTPUT_FMT_3D_SBS      0
#endif

#ifndef PANEL_QUAD_PORT
#define PANEL_QUAD_PORT      0
#endif

#ifndef PANEL_TTL
#define PANEL_TTL      0
#endif

#ifndef PANEL_LVDS_1CH
#define PANEL_LVDS_1CH      0
#endif

#ifndef PANEL_LVDS_2CH
#define PANEL_LVDS_2CH      0
#endif

#ifndef PANEL_LVDS_4CH
#define PANEL_LVDS_4CH      0
#endif

#ifndef PANEL_LVDS_8CH
#define PANEL_LVDS_8CH      0
#endif

#ifndef PANEL_VBY1_1CH_8Bit
#define PANEL_VBY1_1CH_8Bit      0
#endif

#ifndef PANEL_VBY1_1CH_10Bit
#define PANEL_VBY1_1CH_10Bit      0
#endif

#ifndef PANEL_VBY1_2CH_8Bit
#define PANEL_VBY1_2CH_8Bit      0
#endif

#ifndef PANEL_VBY1_2CH_10Bit
#define PANEL_VBY1_2CH_10Bit      0
#endif

#ifndef PANEL_VBY1_4CH_8Bit
#define PANEL_VBY1_4CH_8Bit      0
#endif

#ifndef PANEL_VBY1_4CH_10Bit
#define PANEL_VBY1_4CH_10Bit      0
#endif

#ifndef PANEL_VBY1_8CH_8Bit
#define PANEL_VBY1_8CH_8Bit      0
#endif

#ifndef PANEL_VBY1_8CH_10Bit
#define PANEL_VBY1_8CH_10Bit      0
#endif

#ifndef PANEL_EDP
#define PANEL_EDP      0
#endif


#ifndef LVDS_TI_BitModeSel
#define LVDS_TI_BitModeSel          2 // TI_10BIT:1 ,TI_8BIT:2,TI_6BIT:3
#endif

//#ifndef LVDS_CHANNEL
//#define LVDS_CHANNEL (MOD_PORTA|MOD_PORTB|MOD_PORTC|MOD_PORTD)
//#endif
#ifndef LVDS_CH_OUT
#define LVDS_CH_OUT           		0x555
#endif

#ifndef LVDS_2CH_A_OUT
#define LVDS_2CH_A_OUT			LVDS_CH_OUT	
#endif

#ifndef LVDS_2CH_B_OUT
#define LVDS_2CH_B_OUT			LVDS_CH_OUT
#endif

#ifndef LVDS_2CH_C_OUT
#define LVDS_2CH_C_OUT			0
#endif

#ifndef LVDS_2CH_D_OUT
#define LVDS_2CH_D_OUT			0
#endif

#ifndef LVDS_2CH_E_OUT
#define LVDS_2CH_E_OUT			0	
#endif

#ifndef LVDS_2CH_F_OUT
#define LVDS_2CH_F_OUT			0
#endif

#ifndef LVDS_2CH_G_OUT
#define LVDS_2CH_G_OUT			0
#endif

#ifndef LVDS_2CH_H_OUT
#define LVDS_2CH_H_OUT			0
#endif


#ifndef PANEL_V_SYNC_FREQ
#define PANEL_V_SYNC_FREQ   60
#endif

#ifndef LVDS_CH_A
#define LVDS_CH_A           0
#endif

#ifndef LVDS_CH_B
#define LVDS_CH_B           1
#endif

#ifndef LVDS_CH_C
#define LVDS_CH_C           2
#endif

#ifndef LVDS_CH_D
#define LVDS_CH_D           3
#endif

#ifndef LVDS_CH_E
#define LVDS_CH_E           4
#endif

#ifndef LVDS_CH_F
#define LVDS_CH_F           5
#endif

#ifndef LVDS_CH_G
#define LVDS_CH_G           6
#endif

#ifndef LVDS_CH_H
#define LVDS_CH_H           7
#endif

#ifndef LVDS_CH_A_SWAP
#define LVDS_CH_A_SWAP              LVDS_CH_A
#endif

#ifndef LVDS_CH_B_SWAP
#define LVDS_CH_B_SWAP              LVDS_CH_B
#endif

#ifndef LVDS_CH_C_SWAP
#define LVDS_CH_C_SWAP              LVDS_CH_C
#endif

#ifndef LVDS_CH_D_SWAP
#define LVDS_CH_D_SWAP              LVDS_CH_D
#endif

#ifndef LVDS_CH_E_SWAP
#define LVDS_CH_E_SWAP              LVDS_CH_E
#endif

#ifndef LVDS_CH_F_SWAP
#define LVDS_CH_F_SWAP              LVDS_CH_F
#endif

#ifndef LVDS_CH_G_SWAP
#define LVDS_CH_G_SWAP              LVDS_CH_G
#endif

#ifndef LVDS_CH_H_SWAP
#define LVDS_CH_H_SWAP              LVDS_CH_H
#endif

#if PANEL_VBY1_4CH_8Bit||PANEL_VBY1_4CH_10Bit
#ifndef VBY1_PHY_LANE0
#define VBY1_PHY_LANE0				0		// 4
#endif
#ifndef VBY1_PHY_LANE1
#define VBY1_PHY_LANE1				1		// 5
#endif
#ifndef VBY1_PHY_LANE2
#define VBY1_PHY_LANE2				2		// 6
#endif
#ifndef VBY1_PHY_LANE3
#define VBY1_PHY_LANE3				3		// 7
#endif
#endif

 // 111227 coding addition, if V(80pin) with dual LVDS output, enable the setting.
//#define DOUBLE_LVDS_CLK_SWING   (PanelDualPort && MS_BOARD_TYPE_SEL == BD_5270_M0A)
 // 120920 coding modified, remove the setting of double LVDS clock swing to main board
 #ifndef DOUBLE_LVDS_CLK_SWING
 #define DOUBLE_LVDS_CLK_SWING      0
 #endif

// 120119 coding addition for Y / B type LED light bar control
// "00= 87mV 01= 127mV 10=160mV 11= 212mV"
#ifndef BOOST_CURRENT_LIMIT_LEVEL
#define BOOST_CURRENT_LIMIT_LEVEL   2
#endif

#define PANEL_VTT_3D_120    1588
#define PANEL_VTT_3D_110    1732
#define PANEL_VTT_3D_100    1905
#define PANEL_VTT_3D        PANEL_VTT_3D_120

#ifndef VERTICAL_DE_START
#define VERTICAL_DE_START   8
#endif

#if CHIP_FAMILY_TYPE == CHIP_FAMILY_MST9U
    #define LVDS_FAIL_TARGET  21//21
#else
    #define LVDS_FAIL_TARGET  0x2A // 0x3F*2/3
    #warning "--------------------not define LVDS_FAIL_TARGET---------------------------"
#endif

#ifndef ENABLE_UP_LOW_BOUNDARY_PROTECTION
#define ENABLE_UP_LOW_BOUNDARY_PROTECTION   1
#endif

#ifdef ENABLE_UP_LOW_BOUNDARY_PROTECTION
    #if CHIP_FAMILY_TYPE==CHIP_FAMILY_MST9U
        #define ICON_Upper_Boundary  34
        #define ICON_Lower_Boundary  8
    #else
        #warning "--------------------not define ICON_Upper/Lower_Boundary---------------------------"
    #endif
#endif

#if PANEL_EDP
#define ENABLE_PANEL_SSC                    0 // eDP panel must set 0
#else
#define ENABLE_PANEL_SSC                    1
#endif
#ifndef MSTAR_TCON
#define MSTAR_TCON							0
#endif

#ifndef PanelSize_Num
#define PanelSize_Num           "40"
#endif

#ifndef Panel_DE_only
#define Panel_DE_only 0 //set 1 to discard virtual BS for panel DE only mode
#endif

#ifndef eDPTXMODOnePort
#define eDPTXMODOnePort 0 
#endif

#if ( (!MST9U_ASIC_1P) && (ENABLE_4K_FBL_MODE) && (PanelHStart < 94)) //(DFT_HBR2_GUARDBAND + 15) * 2
    #warning "--------------------PanelHStart must be more than or equal to 94---------------------------"
#endif

#if 0//( (!MST9U_ASIC_1P) && (PanelHStart < 62)) //(SC_GUARD_BAND_PQ + 15) * 2
    #warning "--------------------PanelHStart must be more than or equal to 62---------------------------"
#endif

#if ( (MST9U_ASIC_1P) && (PanelHStart < 15))
    #warning "--------------------PanelHStart must be more than or equal to 15---------------------------"
#endif

#if 0//(PanelVTotal - PanelHeight - PanelVStart) < 10 //if use frame lock mode, VfrontPorch must be more than 10
    #warning "--------------------PanelVsyncForntPorch must be more than or equal to 10---------------------------"
#endif

#if( (CHIP_ID == MST9U3) && (PanelWidth ==1920) &&(PanelHeight ==1080) && (PanelHTotal<2120))
    #warning "--------------------PanelHTotal must be more than or equal to 2176---------------------------"
#endif

#if (PanelVStart<16)
    #warning "--------------------PanelVStart must be more than or equal to 16---------------------------"
#endif

#ifndef PanelSize                  
#define PanelSize                  PanelSize28W
#endif

#ifndef PanelNormalVTotal
#define PanelNormalVTotal	PanelVTotal
#endif
#ifndef Panel_120CLK
#define Panel_120CLK	              PanelDCLK
#endif

#define EDID_1920x1080_144      ((PanelWidth==1920&&PanelHeight==1080&&(PanelVfreq==1200||PanelVfreq==1440))||(PanelType == PanelLSM315HP01))

#define EDID_1920x1080_240       (PanelWidth==1920&&PanelHeight==1080&&PanelVfreq==2400)

#define EDID_2048x2048               (PanelWidth==2048&&PanelHeight==2048)
#define EDID_2560x1080               (PanelWidth==2560&&PanelHeight==1080&&PanelVfreq==600)
#define EDID_2560x1080_120       (PanelWidth==2560&&PanelHeight==1080&&PanelVfreq==1200)

#define EDID_2560x1440_120       ((PanelWidth==2560&&PanelHeight==1440&&PanelVfreq==1200)||(PanelType == PanelLSM315DP01)||(PanelType == PanelLSM270DP01))
#define EDID_2560x1440               ((PanelWidth==2560&&PanelHeight==1440&&!EDID_2560x1440_120))
#define EDID_3440x1440               (PanelWidth==3440&&PanelHeight==1440)

#define EDID_3840x2160               (PanelWidth==3840&&PanelHeight==2160)
#define EDID_4096x2160               (PanelWidth==4096&&PanelHeight==2160)


#define INPUT_SAME_OUT              ((PanelType == PanelLSM315HP01)||(PanelType == PanelHV320QHM_C81)||(PanelType == PanelLSM270DP01)||(PanelType == PanelLSM315DP01)||(PanelType==PanelM350DVR01_01)||(PanelType==PanelM350DVA01_1)||(PanelType==PanelLSM270HP06))

#define IS_VTOTAL_BY_TYPE              ((PanelType == PanelLSM315HP01))

#define  LOGOTimeOutDelayForVBY1PanelLock  (0&&(PanelType == PanelLSM315HP01))

#define ENABLE_TIMINGCHANGE_CLRBL_ONLY			0	 //  160714_14
#define ENABLE_TIMINGCHANGE_TURNOFFPANEL		(0&&(PanelType == PanelLSM315HP01))    // 160729_04

#define FREESYNC_SW_PATCH_FOR_WITHLINEBUFEER     1//160714_17  update by svn_6856 freesync SW patch for vtotal counter(4095 lines) and delay line(63 lines) issues.
#define SET_BLIT_WITH_OSDFreeSyncSwitchAction        0// 1// (Free_Sync_Enable)

#define ENABLE_SW_FLOCK                                              (1&&((PanelType==PanelLSM270HP06)||(PanelType == PanelLSM270DP01)||(PanelType == PanelLSM315DP01)))

#define USE_FIXED_DCLK_FOR_FREESYNCMODE              0

#define ENABLE_VCNT_FREEZE                                         ((PanelType==PanelLSM270HP06))


#define QHD_2560x1440_Limit_Max_140                        (0&&((PanelType == PanelLSM270DP01)||(PanelType == PanelLSM315DP01)))
#endif

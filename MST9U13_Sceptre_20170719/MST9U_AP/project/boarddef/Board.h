//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all
// or part of MStar Software is expressly prohibited, unless prior written
// permission has been granted by MStar.
//
// By accessing, browsing and/or using MStar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms ("Terms") and to
// comply with all applicable laws and regulations:
//
// 1. MStar shall retain any and all right, ownership and interest to MStar
//    Software and any modification/derivatives thereof.
//    No right, ownership, or interest to MStar Software and any
//    modification/derivatives thereof is transferred to you under Terms.
//
// 2. You understand that MStar Software might include, incorporate or be
//    supplied together with third party`s software and the use of MStar
//    Software may require additional licenses from third parties.
//    Therefore, you hereby agree it is your sole responsibility to separately
//    obtain any and all third party right and license necessary for your use of
//    such third party`s software.
//
// 3. MStar Software and any modification/derivatives thereof shall be deemed as
//    MStar`s confidential information and you agree to keep MStar`s
//    confidential information in strictest confidence and not disclose to any
//    third party.
//
// 4. MStar Software is provided on an "AS IS" basis without warranties of any
//    kind. Any warranties are hereby expressly disclaimed by MStar, including
//    without limitation, any warranties of merchantability, non-infringement of
//    intellectual property rights, fitness for a particular purpose, error free
//    and in conformity with any international standard.  You agree to waive any
//    claim against MStar for any loss, damage, cost or expense that you may
//    incur related to your use of MStar Software.
//    In no event shall MStar be liable for any direct, indirect, incidental or
//    consequential damages, including without limitation, lost of profit or
//    revenues, lost or damage of data, and unauthorized system use.
//    You agree that this Section 4 shall still apply without being affected
//    even if MStar Software has been modified by MStar in accordance with your
//    request or instruction for your use, except otherwise agreed by both
//    parties in writing.
//
// 5. If requested, MStar may from time to time provide technical supports or
//    services in relation with MStar Software to you for your use of
//    MStar Software in conjunction with your or your customer`s product
//    ("Services").
//    You understand and agree that, except otherwise agreed by both parties in
//    writing, Services are provided on an "AS IS" basis and the warranty
//    disclaimer set forth in Section 4 above shall apply.
//
// 6. Nothing contained herein shall be construed as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitration shall be in Taipei, Taiwan and the language shall
//    be English.
//    The arbitration award shall be final and binding to both parties.
//
//******************************************************************************
//<MStar Software>

////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _BOARD_H_
#define _BOARD_H_

#include "datatype.h"
#include "hwreg.h"
#include "chiptype.h"
#include "drvXC_IOPort.h"
//------------------------------BOOTLOADER--------------------------------------

//------------------------------DRAM type---------------------------------------
#define SDRAM_32                        0
#define DDRAM_32                        1
#define DDRAM_16                        2

// new style for S4...
#define SDR                             0x10
#define DDR_I                           0x11
#define DDR_II                          0x12

//------------------------------DRAM Bus---------------------------------------
#define DRAM_BUS_16                     0
#define DRAM_BUS_32                     1

//------------------------------DRAM Chip---------------------------------------
#define DRAM_ETRON                      0
#define DRAM_NANYA                      1

//------------------------------DRAM Size---------------------------------------
#define DDRAM16_32M                     0
#define DDRAM16_64M                     1

//------------------------------IO_DEVICE_SEL-----------------------------------
#define IO_DEV_NULL                     -1
#define IO_DEV_UART0                    0
#define IO_DEV_UART1                    1
#define IO_DEV_PIU_UART0                2

//------------------------------MEMORY_MAP--------------------------------------
#define MMAP_24MB                       0
#define MMAP_16MB                       1
#define MMAP_8MB                        2
#define MMAP_4MB                        3
#define MMAP_4MB_FLASH                  4
#define MMAP_32MB                       5
#define MMAP_64MB                       6
#define MMAP_64_32MB                    7
#define MMAP_128MB                      8
#define MMAP_64MB_SD                    9
#define MMAP_64_64MB                    10
#define MMAP_128_128MB                  11
#define MMAP_256_256MB                  12
#define MMAP_256MB                      13
#define MMAP_512_512MB                  14
#define MMAP_512MB                      15

#define  DDR2_INTERFACE_QFP             0
#define  DDR2_INTERFACE_BGA             1
#define  DDR3_INTERFACE_BGA             2
#define  DDR2_INTERFACE_BGA_4X_MODE     3

//------------------------------------------------------------------------------
#define MEMCLK_1066MHZ                  1
#define MEMCLK_1300MHZ                  2
#define MEMCLK_800MHZ                   3
#define MEMCLK_667MHZ                   4

//------------------------------FLASH_SIZE--------------------------------------
#define FLASH_SIZE_1MB                  0x100000
#define FLASH_SIZE_2MB                  0x200000
#define FLASH_SIZE_4MB                  0x400000
#define FLASH_SIZE_8MB                  0x800000
#define FLASH_SIZE_16MB                 0x1000000

//------------------------------UNUSED MACROS-----------------------------------
#define _FUNC_NOT_USED()                do {} while ( 0 )

//------------------------------MS_BOARD_TYPE_SEL-------------------------------
// Demo board made in Taiwan
#define BD_MST9U                        0x0000
#define BD_MST182A_A01A_S               0x0001 //MST9U
#define BD_MST182A_A01B_S               0x0002 //MST9U
#define BD_MST192A_A01A_S               0x0003 //MST9U2
#define BD_MST192A_A01C_S               0x0005 //MST9U2 6DP
#define BD_MST203A_A01A_S               0x0006 //MST9U3 BGA
#define BD_MST203B_A01A_S               0x0007 //MST9U3 QFP
#define BD_MST203A_A01B_S               0x0008 //MST9U3 4HDMI
#define BD_MST203C_A01A_S               0x0009 //MST9U3 QFP, 2-layers

#define BD_MST9UHL_A0                       0x000A //JRY MST98A0Q1
#define BD_MST9UHL_V3                       0x000B //1V+3HDMI+1DP
#define BD_MST9UHL_V4                       0x000C //D+H+DP
#define BD_MST9UHL_V6                       0x000D //D+H+DP
#define BD_MST9UHL_V4_1H1DP          0x000E //H+DP
#define BD_MST9UHL_V7                       0x000F //D+H+DP //9UHD_AV1
#define BD_MST9UHD_BV1                     0x0010 //3HDMI+DP 
#define BD_MST9UHD_DV1                     0x0011 //1D+2HDMI+DP 
#define BD_MST9UHD_GV1                     0x0012 //2HDMI+DP 


// Others
#define MODEL_NAME                      "L-QMD55MWWAC-3333"     // Used in imginfo.c. 9 characters.

////////////////////////////////////////////////////////////////////////////////
#define ENABLE_UART1_DEBUG              0

#ifndef ENABLE_UART2
#define ENABLE_UART2                    0
#endif

#define XMODEM_DWNLD_ENABLE             1
#define XMODEM_CRC_ENABLE               1

//------------------------------------------------------------------------------
// SW feature configure header file
#include "SW_Config.h"

#include "Enum_Device.h"


#define MST9U   0 //Odin
#define MST9U2  1 //Odin2
#define MST9U3  2 //Leona
#define MST9U4  3

/////////////////////////////////////////////////////////
////////////////////  51 board  ///////////////////////////
////////////////////////////////////////////////////////////
// INPUT_TYPE DEFINE
////////////////////////////////////////////////////////////
#define INPUT_1A        BIT0//0//BIT0    // 1A   //if project doesnt have VGA port , set this define to 0 and dont set ENABLE_VGA to 0 directly to avoid port define incorrectly.
#define INPUT_1C        BIT1    // digital portx1
#define INPUT_2C        BIT2    // digital portx2
#define INPUT_3C        BIT3    // digital portx3
#define INPUT_4C        BIT4    // digital portx4
#define INPUT_5C        BIT5    // digital portx5
#define INPUT_6C        BIT6    // digital portx6
#define INPUT_7C        BIT7    // digital portx7
#define INPUT_8C        BIT8    // digital portx8

//////////////////////////////////////////////////////////////
// KEY SELECTION
//////////////////////////////////////////////////////////////
#define KEY_TYPE_IOKEY              0
#define KEY_TYPE_ADKEY              1
#define KEY_TYPE_IOWITHADKEY        2
#define KEY_TYPE_I2CKEY_CAPACTIVE   3

////////////////////////////////////////////////////////////
// ODM_NAME DEFINE
////////////////////////////////////////////////////////////
#define ODM_AOC                     0
#define ODM_INL                     1
#define ODM_BENQ                    2
#define ODM_WISTRON                 3
#define ODM_AMTRAN                  4

#define ODM_NUUSE                   0xFF
////////////////////////////////////////////////////////////
// FACTORYALIGN_TYPE DEFINE
////////////////////////////////////////////////////////////
#define FACTORYALIGN_RS232          0
#define FACTORYALIGN_DDCCI          1

//*****************************************************************
#include "Model.h"
//#include "DefaultCompilerOption.h"

#ifndef ENABLE_MENULOAD
#define ENABLE_MENULOAD             1
#endif

#if ENABLE_MENULOAD && CHIP_ID == MST9U3
#define ENABLE_BITMASK_MENULOAD     1
#else
#define ENABLE_BITMASK_MENULOAD     0
#endif

#ifndef ENABLE_SW_SCALER_COMMON_WRITE
#define ENABLE_SW_SCALER_COMMON_WRITE       0
#endif
//#if !defined(UseInternalDDCRam)
//#define UseVGACableReadWriteAllPortsEDID
//#endif

#if AudioFunc
#define ENABLE_DAC        1
#define EnableDACDePop              (1&ENABLE_DAC) //Earout and Lineout need to do Depop flow for salving pop noise
#if CHIP_ID == MST9U3
#define EnableDACPowerDown      (1&ENABLE_DAC) //DAC needs to do power down sequence to avoid pop noise
#else
#define EnableDACPowerDown       (0&ENABLE_DAC) //DAC needs to do power down sequence to avoid pop noise but it takes 2.xSec.
#endif
#else
#define ENABLE_DAC        0
#endif


#define LoadDefaultEDIDTo24C02

#define AutoSwitchPort_Enable                 (1)

#define VBY1_PowerSequence                    0

#define OSD_LOCK_Enable                          (defined(FengYuan_Project))

#define Auto_Switch_AudioSource               (defined(LeXingEnMa_Project))

#ifndef ENABLE_FACTORY_SSCADJ
#define ENABLE_FACTORY_SSCADJ 0
#endif

#define Enable_Auto_Switch_DE_Mode         (0&&Free_Sync_Enable)

#define LED_HOT_Menu_Func                        (defined(LianHeChuangXin_Project)&&(ModeName==K3D6G||ModeName==V3K1M))// 1//(1&&(MS_BOARD_TYPE_SEL == BD_MST9UHL_A0||MS_BOARD_TYPE_SEL == BD_MST9UHL_V4||MS_BOARD_TYPE_SEL == BD_MST9UHL_V6||MS_BOARD_TYPE_SEL==BD_MST9UHL_V4_1H1DP||MS_BOARD_TYPE_SEL==BD_MST9UHL_V7)&&(defined(W3K_Project)||defined(V3L_Project)||defined(V3K_Project)||defined(V3G_Project)||defined(K3D_Project)||defined(K3L_CrossOver_Project)||defined(GK3B_Project)||defined(GV3B_Project)||defined(K3L_LED_Project)||defined(V3L_CrossOver_Project)||defined(K3D_FHD144_Project)))

#define New_LED_Menu_Icon                         (1&&(defined(GK3B_Project)))

#define Enable_SPDIF                                    ((MS_BOARD_TYPE_SEL == BD_MST9UHL_V3)&&ENABLE_ADJ_AUDIO_OUTPUT&&(!defined(Japan_Project)))

#define DP_HPD_Test                                     1

#define Disable_DDCCI                                   (1&&(defined(LianHeChuangXin_Project)||defined(ZhongXinKeJi_Project)))

#define ENABLE_USER_PREF_HDCP            	 1//((defined(LianHeChuangXin_Project))&&(ModeName==K3D6G||ModeName==M3CDG))

#define ENABLE_AUDIO_NEW_CURVE             (0&&(defined(LianHeChuangXin_Project)))

//*****************************************************************
//*****************************************************************
#if  ENABLE_HDMI_INPUT
#define ENABLE_AUDIO_AUTO_MUTE          1 // auto mute setting
#define ENABLE_AUDIO_AUTO_FADING        1 // auto fading setting
#else
#define ENABLE_AUDIO_AUTO_MUTE          0
#define ENABLE_AUDIO_AUTO_FADING        0
#endif

#define Enable_Gamma                    1
#define Enable_AutoDownloadGamma        (0&Enable_Gamma)
#define USE_NORMAL_GAMMA_TBL            (1&Enable_Gamma)

#define Enable_AutoDownload3dLut        0

#define DDCCI_ENABLE                    1

#define PANEL_WIDTH                 PanelWidth
#define PANEL_HEIGHT                PanelHeight
#include "Panel.h"
#include "DefaultCompilerOption.h"

//*****************************************************************
#ifndef ENABLE_COMPRESSED_PROP_FONT
#define ENABLE_COMPRESSED_PROP_FONT    0
#endif
#ifndef PropFontUseCommonArea
#define PropFontUseCommonArea          0
#endif
////////////////////  51 board  ///////////////////////////
/////////////////////////////////////////////////////////
#if (MS_BOARD_TYPE_SEL == BD_MST9U)
    #define USEFLASH    0
    #include "BD_MST9U.h"
#elif (MS_BOARD_TYPE_SEL == BD_MST182A_A01A_S)
    #define USEFLASH    0
    #include "BD_MST182A_A01A_S.h"
#elif (MS_BOARD_TYPE_SEL == BD_MST182A_A01B_S)
    #define USEFLASH    0
    #include "BD_MST182A_A01B_S.h"
#elif (MS_BOARD_TYPE_SEL == BD_MST192A_A01A_S)
    #define USEFLASH    0
    #include "BD_MST192A_A01A_S.h"
#elif (MS_BOARD_TYPE_SEL == BD_MST192A_A01C_S)
    #define USEFLASH    0
    #include "BD_MST192A_A01C_S.h"
#elif (MS_BOARD_TYPE_SEL == BD_MST203A_A01A_S)
    #define USEFLASH    0
    #include "BD_MST203A_A01A_S.h"
#elif (MS_BOARD_TYPE_SEL == BD_MST203A_A01B_S)
    #define USEFLASH    0
    #include "BD_MST203A_A01B_S.h"
#elif (MS_BOARD_TYPE_SEL == BD_MST203B_A01A_S)
    #define USEFLASH    0
    #include "BD_MST203B_A01A_S.h"
#elif (MS_BOARD_TYPE_SEL == BD_MST203C_A01A_S)
    #define USEFLASH    0
    #include "BD_MST203C_A01A_S.h"
#elif (MS_BOARD_TYPE_SEL == BD_MST9UHL_A0)
    #define USEFLASH    0
    #include "BD_MST9UHL_A0.h"

#elif (MS_BOARD_TYPE_SEL == BD_MST9UHL_V3)
    #define USEFLASH    0
    #include "BD_MST9UHL_V3.h"
	
#elif (MS_BOARD_TYPE_SEL == BD_MST9UHL_V4)
    #define USEFLASH    0
    #include "BD_MST9UHL_V4.h"
	
#elif (MS_BOARD_TYPE_SEL == BD_MST9UHL_V6)
    #define USEFLASH    0
    #include "BD_MST9UHL_V6.h"
	
#elif (MS_BOARD_TYPE_SEL==BD_MST9UHL_V4_1H1DP)
    #define USEFLASH    0
    #include "BD_MST9UHL_V4_1H1DP.h"
	
#elif (MS_BOARD_TYPE_SEL==BD_MST9UHL_V7)
    #define USEFLASH    0
    #include "BD_MST9UHL_V7.h"
	
#elif (MS_BOARD_TYPE_SEL == BD_MST9UHD_BV1)
    #define USEFLASH    0
    #include "BD_MST9UHD_BV1.h"
	
#elif (MS_BOARD_TYPE_SEL == BD_MST9UHD_DV1)
    #define USEFLASH    0
    #include "BD_MST9UHD_DV1.h"
	
#elif (MS_BOARD_TYPE_SEL == BD_MST9UHD_GV1)
    #define USEFLASH    0
    #include "BD_MST9UHD_GV1.h"

	
#else
    #error Unknow board type
#endif
/////////////////////////////////////////////////////////
////////////////////  51 board  ///////////////////////////
// to reduce power consumption during monitor active
#define _IP_POWER_CTRL_

#ifndef Init_hwHDCP_Hpd_Pin2
#define Init_hwHDCP_Hpd_Pin2()
#endif

#ifndef Init_hwHDCP_Hpd_Pin3
#define Init_hwHDCP_Hpd_Pin3()
#endif

#ifndef CABLE_DETECT_VGA_USE_SAR
#define CABLE_DETECT_VGA_USE_SAR    0
#endif

#ifndef CABLE_DET_SAR_EN
#define CABLE_DET_SAR_EN    0
#endif

// Chip definition
#if 0//ndef CHIP_ID
#define CHIP_ID         CHIP_TSUMV
#endif

#ifndef ENABLE_DP_INPUT
#define ENABLE_DP_INPUT 0
#endif

#ifndef ENABLE_VD_INPUT
#define ENABLE_VD_INPUT 0
#endif

#ifndef ENABLE_DP_AUDIO
#define ENABLE_DP_AUDIO 0
#endif

#ifndef ENABLE_DVI_DUAL_LINK
#define ENABLE_DVI_DUAL_LINK    0
#endif

#ifndef  ENABLE_LED_CONTROLLER
#define ENABLE_LED_CONTROLLER   0
#endif

#ifndef ENABLE_SUPER_RESOLUTION
#define ENABLE_SUPER_RESOLUTION 0
#endif

#ifndef USE_DAC_ADJ
#define USE_DAC_ADJ     0
#endif
#ifndef ENABLE_DLC
#define ENABLE_DLC  0
#endif
#ifndef ENABLE_DPS
#define ENABLE_DPS  0
#endif

#ifndef ENABLE_DeBlocking
#define ENABLE_DeBlocking  0
#endif


// 120106 coding addition, default set adc dithering to disable
#ifndef ENABLE_ADC_DITHERING
#define ENABLE_ADC_DITHERING    1
#endif

//I_Gen_Tuning Target Select
#if CHIP_FAMILY_TYPE == CHIP_FAMILY_TSUMV || CHIP_FAMILY_TYPE == CHIP_FAMILY_TSUM2
#define IGT_TARGET_250  0
#define IGT_TARGET_300  1
#define IGT_TARGET_200  2
#define IGT_TARGET_350  3
#elif CHIP_FAMILY_TYPE == CHIP_FAMILY_TSUMC ||CHIP_FAMILY_TYPE == CHIP_FAMILY_TSUMJ
#define IGT_TARGET_250  0
#define IGT_TARGET_350  1
#define IGT_TARGET_300  2
#define IGT_TARGET_200  3
#elif CHIP_FAMILY_TYPE == CHIP_FAMILY_MST9U
#define IGT_TARGET_250  0
#define IGT_TARGET_350  1
#define IGT_TARGET_300  2
#define IGT_TARGET_400  3
#else
#define IGT_TARGET_250  0
#define IGT_TARGET_300  1
#define IGT_TARGET_350  2
#define IGT_TARGET_200  3
#endif
//I_Gen_Tuning CH Source Select
#if (CHIP_FAMILY_TYPE == CHIP_FAMILY_TSUMC)||(CHIP_FAMILY_TYPE == CHIP_FAMILY_TSUMJ)
#define CH2                     0
#define CH6                     1
#define CH8                     2
#elif(CHIP_FAMILY_TYPE == CHIP_FAMILY_MST9U)
#define CH2                     0
#define CH6                     (BIT2)
#define CH14                    (BIT3)
#define CH20                    (BIT3|BIT2)
#define CH30                    (BIT4)
#define CH34                    (BIT4|BIT2)
#define CH42                    (BIT4|BIT3)
#define CH47                    (BIT4|BIT3|BIT2)
#else
#define CH3_CLK         0
#define CH9_CLK         1
#define CH15_CLK        2
#define CH21_CLK        3
#endif
//LVDS_CHANNEL select
#define MOD_PORTA       BIT0
#define MOD_PORTB       BIT1
#define MOD_PORTC       BIT2
#define MOD_PORTD       BIT3

//TMDS PORT
#define TMDS_PORT_UNUSE 0
#define TMDS_PORT_A     BIT0
#define TMDS_PORT_B     BIT1
#define TMDS_PORT_C     BIT2

//OD mode
#define OD_MODE_444             0
#define OD_MODE_565             1
#define OD_MODE_666             4
#define OD_MODE_COMPRESS        5 //Output H blank should be over 300 when use OD compress mode.
#define OD_MODE_555             6
#define OD_MODE_888             7

#define OD_HSCAL_EN             _ENABLE
#define OD_VSCAL_EN             _ENABLE

#ifndef OD_MODE
#if CHIP_ID==MST9U4
#define OD_MODE                 OD_MODE_565
#else //MST9U,MST9U2,MST9U3
#define OD_MODE                 OD_MODE_COMPRESS
#endif
#endif

// 3D glasses type
#define GLASSES_NONE    0
#define GLASSES_IR      1
#define GLASSES_INTEL   2
#define GLASSES_NVIDIA  3

#ifndef GLASSES_TYPE
#define GLASSES_TYPE          GLASSES_NONE
#endif

////////////////////////////////////////////////////////
// Frame buffer setting
////////////////////////////////////////////////////////

#ifndef PANEL_3D_PASSIVE_4M
#define PANEL_3D_PASSIVE_4M     0
#endif

#if 0
#define DOWN_COUNTER_V_SYNC     0 // 120403 coding
#endif

#if CHIP_FAMILY_TYPE==CHIP_FAMILY_TSUMU
#define PANEL_H_DIV             2 // H relative parameters(display timing) divided by 1 or 2
#else
#define PANEL_H_DIV             1 // H relative parameters(display timing) divided by 1 or 2
#endif

#if PANEL_H_DIV ==2
#define ACC_HDIV_BIT    BIT15
#else
#define ACC_HDIV_BIT    0
#endif

#define EN_V4TAP_SCALING            0 //(CHIP_FAMILY_TYPE==CHIP_FAMILY_TSUMB || CHIP_FAMILY_TYPE==CHIP_FAMILY_TSUMY) //for vertical 4-tap scaling mode

#ifndef ENABLE_3D_FUNCTION
#define ENABLE_3D_FUNCTION  0
#endif

#ifndef MST_ASSERT
#define MST_ASSERT(_test_)
#endif

#define CRYSTAL_CLOCK       (12000000ul) //14318000ul
#define XTAL_CLOCK_KHZ      (12000ul)    //(14318ul)

#define WATCH_DOG_TIME_RESET        (10)   // need sync with 51 setting, unit: second
#define WATCH_DOG_TIME_INTERRUPT    (3)   // need sync with 51 setting, unit: second

#define INT_PERIOD          1// timer interrupt: unit = 1ms
//#define BaudRatePeriod    (0x10000 - (((unsigned long)CPU_CLOCK_MHZ * 1000000)/((unsigned long) SERIAL_BAUD_RATE / 10 * 32) + 5) / 10)
#define SystemTick  1000    // 1 second
#define UseINT      1
#define UseVsyncINT 0
#define UsesRGB     0

#define ClockPeriod (0x10000-(((unsigned long)XTAL_CLOCK_KHZ*INT_PERIOD)/12))    // Interrupt every 1ms for Time 0
#define BaudRatePeriod  (0x10000-(((unsigned long)XTAL_CLOCK_KHZ * 1000+(DWORD)SERIAL_BAUD_RATE*16)/((DWORD)SERIAL_BAUD_RATE*32)))
#define T1BaudRatePeriod    (0x100-(((unsigned long)XTAL_CLOCK_KHZ * 1000+(DWORD)SERIAL_BAUD_RATE*192)/((DWORD)SERIAL_BAUD_RATE*384)))
#define HFREQ(hPeriod)  ((hPeriod)?(((DWORD)XTAL_CLOCK_KHZ*10+hPeriod/2)/hPeriod):(1)) // 120330 coding modified to avoid divide 0
#define VFREQ(hFreq, vTotal)    ((vTotal)?(((DWORD)hFreq*1000+vTotal/2)/vTotal):(1)) // 120330 coding modified to avoid divide 0
#define MST_HPeriod_UINT    (XTAL_CLOCK_KHZ*16)
#define VFreqIn         (((VFREQ( HFREQ( SrcHPeriod ), SrcVTotal )+5)/10) * ((SrcFlags&bInterlaceMode)?2:1))

////////////////////  51 board  ///////////////////////////
/////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
// The file can fix compile error if some board definition didn't define
#include "default_option_define.h"

//------PCI_PORT_FUNCTION---------------------------------------------------------

//PWM0~3 mode
#define NORMAL_MODE                     0
#define ENHANCE_MODE                    1

//------------------------------ Board Default -----------------------------------
#ifndef I2C_IO_EXPANDER_TYPE
    #define I2C_IO_EXPANDER_TYPE        I2C_IO_EXPANDER_NONE
#endif

#ifndef USE_SW_I2C
    #define USE_SW_I2C                  1
#endif

#ifndef INPUT_HDMI1_MUX
    #define INPUT_HDMI1_MUX             INPUT_PORT_DVI0
#endif
#ifndef INPUT_HDMI2_MUX
    #define INPUT_HDMI2_MUX             INPUT_PORT_DVI1
#endif
#ifndef INPUT_HDMI3_MUX
    #define INPUT_HDMI3_MUX             INPUT_PORT_DVI2
#endif
#ifndef INPUT_HDMI4_MUX
    #define INPUT_HDMI4_MUX             INPUT_PORT_DVI3
#endif

#ifndef FONT_SUPPORT_VECTOR_AND_BITMAP
    #define FONT_SUPPORT_VECTOR_AND_BITMAP    0
#endif

#ifndef ENABLE_DMP_SWITCH
    #define ENABLE_DMP_SWITCH           0
#endif

//----SPI FLASH------------------------------------------------------------------
#ifndef FLASH_SIZE
    #define FLASH_SIZE                  FLASH_SIZE_8MB  // the extern flash use for power on music
#endif
#ifndef FLASH_BLOCK_SIZE
    #define FLASH_BLOCK_SIZE            0x10000
#endif
//#define FLASH_BLOCK_COUNT         (FLASH_SIZE/FLASH_BLOCK_SIZE)

#ifndef ENABLE_POWERON_VIDEO
    #define ENABLE_POWERON_VIDEO        DISABLE //enable it to display Video when system boot up
#endif

#ifndef ENABLE_MM_HD_FB
    #define ENABLE_MM_HD_FB             DISABLE
#endif

#ifndef ENABLE_JPEGPNG_OSD
#define ENABLE_JPEGPNG_OSD                  0
#endif

#ifndef ENABLE_SPI_2ND
#define ENABLE_SPI_2ND                0 // SPI1
#endif

#ifndef SPI1_FLASH_TYPE
#define SPI1_FLASH_TYPE                 NAND_FLASH
#endif


//---- eDP----------------------------------------------------------
#ifndef eDPTX_AUXChannel_SWAP
#define eDPTX_AUXChannel_SWAP       0
#endif


// ---- REMOTE SELECTION  ----------------------------------------------------------
#define DEBUG_IR_SW_MODE            0
#define NO_REMOTE                   0
#define TW_REMOTE                   1
#define MITSUBISHI_REMOTE           2
#define RC5_MAGNAVOX                3
#define MSTAR_REMOTE                4
#define RC6_PHILIPS                 5
#define JRY_IR_MODE                       6

#define UNKNOW_REMOTE               0xFF

#if(IR_REMOTE_SEL != NO_REMOTE)
// IR mode selection
#define IR_SOFTWARE_MODE             0
#define IR_FULL_INT_NEC_MODE         1
#define IR_FULL_POL_NEC_MODE         2
#define IR_RAW_DATA_POL_MODE         3
#define IR_RAW_DATA_INT_MODE         4
#define IR_SW_RC5_MODE               5
#define IR_FULL_INT_MIT_MODE         6
#define IR_FULL_INT_RC5_EXT_MODE     7
#define IR_FULL_INT_RC5_MODE         8
#define IR_FULL_INT_RC6_HEAD888MS_MODE      9   //Header is double by other bit length
#define IR_FULL_INT_RC6_HEAD444MS_MODE     10  //Header is not double by other bit length

#if(IR_REMOTE_SEL==TW_REMOTE)
  #define IR_MODE_SEL           IR_FULL_POL_NEC_MODE//IR_FULL_INT_NEC_MODE//IR_SW_RC5_MODE//IR_FULL_POL_MODE//IR_FULL_INT_MODE
  #define IR_HEADER_CODE0       0x41
  #define IR_HEADER_CODE1       0x01
#elif(IR_REMOTE_SEL==MSTAR_REMOTE)
  #define IR_MODE_SEL           IR_SOFTWARE_MODE//IR_FULL_POL_NEC_MODE//IR_FULL_INT_NEC_MODE//IR_SW_RC5_MODE//IR_FULL_POL_MODE//IR_FULL_INT_MODE
  #define IR_HEADER_CODE0       0x80
  #define IR_HEADER_CODE1       0x7F
#elif(IR_REMOTE_SEL==RC5_MAGNAVOX)
  #define IR_MODE_SEL           IR_FULL_INT_RC5_EXT_MODE//IR_FULL_INT_NEC_MODE
  #define IR_RC5_ADDR1          0x03
  #define IR_RC5_ADDR2          0x00
#elif(IR_REMOTE_SEL==MITSUBISHI_REMOTE)
  #define IR_MODE_SEL           IR_FULL_INT_MIT_MODE//IR_FULL_INT_NEC_MODE
  #define IR_HEADER_CODE0       0xF4
  #define IR_HEADER_CODE1       0x00
  #define MIT_SEP_FROM_BIT      0x08
#elif(IR_REMOTE_SEL == RC6_PHILIPS)
  #define IR_MODE_SEL           IR_FULL_INT_RC6_HEAD444MS_MODE//IR_FULL_INT_NEC_MODE
  #define IR_RC6_ADDR          0x00
#elif(IR_REMOTE_SEL==JRY_IR_MODE)

#if (defined(LeShiYouXian_Project))
  #define IR_MODE_SEL           IR_RAW_DATA_POL_MODE
  #define IR_HEADER_CODE0       0xFF//0xCB
  #define IR_HEADER_CODE1       0x00//0x04
#elif (defined(Korean_Project))
  #define IR_MODE_SEL           IR_RAW_DATA_POL_MODE
  #define IR_HEADER_CODE0       0xF7//0xCB
  #define IR_HEADER_CODE1       0x08//0x04
#elif (defined(LeXingEnMa_Project)||LHCX_IR_Function_Enable)
  #define IR_MODE_SEL           IR_RAW_DATA_INT_MODE
  #define IR_HEADER_CODE0       0xFF
  #define IR_HEADER_CODE1       0x00
  
  #define IR_MODE_SEL_2
  #ifdef IR_MODE_SEL_2
  #define IR_HEADER_CODE2       0x04
  #define IR_HEADER_CODE3       0xFB
  #endif
    
  #define IR_MODE_SEL_3
  #ifdef IR_MODE_SEL_3
  #define IR_HEADER_CODE4       0x80
  #define IR_HEADER_CODE5       0xFF
  #endif

  #define IR_MODE_SEL_4
  #ifdef IR_MODE_SEL_4
  #define IR_HEADER_CODE6       0xAB
  #define IR_HEADER_CODE7       0xCD

  #define IR_HEADER_CODE6_2       0x86
  #define IR_HEADER_CODE7_2       0xE7
  #endif

  #define IR_MODE_SEL_5
  #ifdef IR_MODE_SEL_5
  #define IR_HEADER_CODE8_Low       0x02
  #define IR_HEADER_CODE9_High      0xFD
  #endif

#elif (defined(ZhouYang_Project))
  #define IR_MODE_SEL           IR_RAW_DATA_INT_MODE
  #define IR_HEADER_CODE0       0x00
  #define IR_HEADER_CODE1       0xFF

#elif (JieBo_IR_Function_Enable)
  #define IR_MODE_SEL           IR_RAW_DATA_INT_MODE
  #define IR_HEADER_CODE0       0x08
  #define IR_HEADER_CODE1       0xF7
  
   #define IR_MODE_SEL_4_Customer
  #ifdef IR_MODE_SEL_4_Customer
  #define IR_HEADER_CODE6_Customer       0x04
  #define IR_HEADER_CODE7_Customer       0x10
  #endif

#elif (defined(TaiBei_Project))
  #define IR_MODE_SEL           IR_RAW_DATA_INT_MODE
  #define IR_HEADER_CODE0       0x08
  #define IR_HEADER_CODE1       0xF7
  
  #define IR_MODE_SEL_2
  #ifdef IR_MODE_SEL_2
  #define IR_HEADER_CODE2       0x01
  #define IR_HEADER_CODE3       0xFE
  #endif
  
  #define IR_MODE_SEL_3
  #ifdef IR_MODE_SEL_3
  #define IR_HEADER_CODE4       0x60 
  #define IR_HEADER_CODE5       0x9F
  #endif

#elif (defined(HuaKe_Project))

  #define IR_MODE_SEL           IR_RAW_DATA_INT_MODE
  #define IR_HEADER_CODE0       0x08
  #define IR_HEADER_CODE1       0xF7

/*
   #define IR_MODE_SEL           IR_FULL_INT_RC6_HEAD444MS_MODE//IR_FULL_INT_NEC_MODE
  #define IR_RC6_ADDR          0x01
 */
#elif (defined(Japan_Project))
  #define IR_MODE_SEL           IR_RAW_DATA_INT_MODE
  #define IR_HEADER_CODE0       0x02
  #define IR_HEADER_CODE1       0xFD

#elif (defined(FengYuan_Project))
  #define IR_MODE_SEL           IR_RAW_DATA_INT_MODE
  #define IR_HEADER_CODE0       0x86
  #define IR_HEADER_CODE1       0xE7
  
 #define IR_MODE_SEL_2
#ifdef IR_MODE_SEL_2
  #define IR_HEADER_CODE2       0x04
 #define IR_HEADER_CODE3       0xFB
 #endif
 
#elif (defined(KunDe_Project))
  #define IR_MODE_SEL           IR_RAW_DATA_INT_MODE
  #define IR_HEADER_CODE0       0x40
  #define IR_HEADER_CODE1       0xBF
  
 #define IR_MODE_SEL_2
#ifdef IR_MODE_SEL_2
  #define IR_HEADER_CODE2       0x86
 #define IR_HEADER_CODE3       0x6B
 #endif
 
#elif (defined(ZhongChuangLianHe_Project))

  #define IR_MODE_SEL           IR_RAW_DATA_INT_MODE
  #define IR_HEADER_CODE0       0x00
  #define IR_HEADER_CODE1       0xFF

#elif (defined(Changjia_Project))
  #define IR_MODE_SEL           IR_RAW_DATA_INT_MODE
  #define IR_HEADER_CODE0       0x00
  #define IR_HEADER_CODE1       0xBF

#elif (defined(XuLi_Project))
  #define IR_MODE_SEL           IR_RAW_DATA_INT_MODE
  #define IR_HEADER_CODE0       0x02//0x86
  #define IR_HEADER_CODE1       0xFD//0xE7

#elif (defined(MingCai_Project))
  #define IR_MODE_SEL           IR_RAW_DATA_INT_MODE
  #define IR_HEADER_CODE0       0x08//0x86
  #define IR_HEADER_CODE1       0xF7//0xE7

#else
  #define IR_MODE_SEL           IR_RAW_DATA_POL_MODE
  #define IR_HEADER_CODE0       0xF7//0xCB
  #define IR_HEADER_CODE1       0x08//0x04
#endif  
  
#else
  #define IR_MODE_SEL           IR_FULL_INT_NEC_MODE//IR_FULL_INT_NEC_MODE
  #define IR_HEADER_CODE0       0x41
  #define IR_HEADER_CODE1       0x01
#endif
#endif  //(IR_REMOTE_SEL != NO_REMOTE)

#define PM_USE_EEPROM       0

#if( (CHIP_ID == MST9U || CHIP_ID == MST9U2) && (OD_MODE == OD_MODE_COMPRESS) && (PanelHTotal - PanelWidth) < 320)
   #warning "--------------------HBlanking must be more than or equal to 320---------------------------"
#endif

#endif // _BOARD_H_


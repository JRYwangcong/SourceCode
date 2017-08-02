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
#ifndef _MSBOARD_H_
#define _MSBOARD_H_

#define BOARD_NAME                  "BD_MST203A_A01B_S"

//------I2C devices-------------------------------------------------------------
#if USE_SW_I2C
#undef USE_SW_I2C
#define USE_SW_I2C 1
#define USE_SW_I2C_HIGHSPEED        0
#endif

// HWi2c for enabling MIIC0 and MIIC1 // Yuchen.Lai 140521
#define MIIC0_ENABLE    MIIC0_2_ENABLE    // Default-DaisyChain:MIIC0_2_ENABLE, EEPROM:MIIC0_1_ENABLE,
#define MIIC1_ENABLE    MIIC1_0_ENABLE    // Vby1
#define USE_MIIC        MIIC0

//------IR & Key Setting--------------------------------------------------------
#define IR_REMOTE_SEL                   NO_REMOTE//MSTAR_REMOTE//TW_REMOTE//SHINCO_REMOTE//RC5_MAGNAVOX//SZ_REMOTE

//------Memory Setting----------------------------------------------------------
#ifndef MEMORY_MAP
#define MEMORY_MAP                      MMAP_128MB
#endif

//------MCU use Scaler internal MPLL clock-------------------
#define MST_XTAL_CLOCK_HZ               FREQ_12MHZ
#define MST_XTAL_CLOCK_KHZ              (MST_XTAL_CLOCK_HZ/1000UL)
#define MST_XTAL_CLOCK_MHZ              (MST_XTAL_CLOCK_KHZ/1000UL)

//------8051 Serial Port Setting------------------------------------------------
#if(ENABLE_UART1_DEBUG)
#define ENABLE_UART0                    DISABLE
#define ENABLE_UART0_INTERRUPT          DISABLE
#define ENABLE_UART1                    ENABLE
#define ENABLE_UART1_INTERRUPT          ENABLE
//------STDIO device setting----------------------------------------------------
#define STDIN_DEVICE                    IO_DEV_UART1
#define STDOUT_DEVICE                   IO_DEV_UART1
#else
#define ENABLE_UART0                    ENABLE
#define ENABLE_UART0_INTERRUPT          ENABLE
#define ENABLE_UART1                    DISABLE
#define ENABLE_UART1_INTERRUPT          DISABLE
//------STDIO device setting----------------------------------------------------
#define STDIN_DEVICE                    IO_DEV_UART0
#define STDOUT_DEVICE                   IO_DEV_UART0
#endif

//DDR Related
#define MIU0_SIZE_128M  0x8000000UL  //refine later
#define MIU0_BANK       0x8

#define  MEMPLL_SEL         MEMCLK_800MHZ







/////////////////////////////////////////////////////////////
///////////////////////   51 Board    //////////////////////////
/////////////////////////////////////////////////////////////
#define MCU_SPEED_INDEX         IDX_MCU_CLK_MPLL_DIV // 172M
#define SPI_SPEED_INDEX         IDX_SPI_CLK_43MHZ
#define SPI_MODE                SPI_MODE_FR
#define HPD_INVERSE             0 // If it has BJT circuit on board, the HPD_INVERSE should be set to TRUE

#define SPI_SSC_EN              (SPI_SPEED_INDEX==IDX_SPI_CLK_SSC)// use memory clock source, need to enable memclk SSC, and divide it to the one closest & lower to SPI_SPEED_INDEX
#define SPI_SSC_SOURCE          0 // 0:mempll, 1:lpll (Current code flow for mempll only)
#define SPI_SSC_DIVIDER         4 // Target Freq => (MEMORY_CLOCK_MHZ*(100+PERCENTAGE)/(SPI_SSC_DIVIDER+1))

typedef enum // UART GPIO
{
    UART_GPIO02_03,
    UART_GPIO40_41,  // VGA ?
    UART_GPIO50_51,
    UART_GPIOX03_04, // combo 0
    UART_GPIOX13_14, // combo 1
    UART_GPIOX23_24, // combo 2
    UART_GPIOX33_34, // combo 3
    UART_GPIOX43_44, // combo 4
    UART_GPIOX53_54 // combo 5
}UART_GPIO;

typedef enum // UART Engine
{
    UART_ENGINE_51_UART0,
    UART_ENGINE_51_UART1,
    UART_ENGINE_R2_HK,
    UART_ENGINE_R2_NONHK,
    UART_ENGINE_DW_uart,
    RESERVED
}UART_ENGINE;

#define DFT_R2_UART0_GPIO           UART_GPIO40_41 // default mux uart 0 - VGA
#define DFT_R2_UART1_GPIO           UART_GPIOX03_04

#define PORT_FLOW_0     MUX_ADC
#define PORT_FLOW_1     MUX_COMBO4
#define PORT_FLOW_2     MUX_COMBO2
#define PORT_FLOW_3     MUX_COMBO3
#define PORT_FLOW_4     MUX_COMBO0
#define PORT_FLOW_5     MUX_COMBO1
//#define PORT_FLOW_7

#define PORT_TYPE_0     TYPE_ANALOG
#define PORT_TYPE_1     (TYPE_DP|TYPE_HBR2_0|TYPE_DAISY_CHAIN)
#define PORT_TYPE_2     (TYPE_HDMI |TYPE_MHL |TYPE_HDMI_2_0)
#define PORT_TYPE_3     (TYPE_HDMI |TYPE_MHL |TYPE_HDMI_2_0)
#define PORT_TYPE_4     (TYPE_HDMI |TYPE_MHL)
#define PORT_TYPE_5     (TYPE_HDMI |TYPE_MHL)
//#define PORT_TYPE_7

// Define TMDS/DP port used
#define TMDS_Port_D0    1 //HDMI
#define TMDS_Port_D1    1 //HDMI
#define TMDS_Port_D2    1 //HDMI
#define TMDS_Port_D3    1 //HDMI
#define TMDS_Port_D4    0 //none
#define TMDS_Port_D5    0 //none
#define TMDS_Port_Num   (TMDS_Port_D0+TMDS_Port_D1+TMDS_Port_D2+TMDS_Port_D3+TMDS_Port_D4+TMDS_Port_D5)

#define DP_Port_D0      0 //none
#define DP_Port_D1      0 //none
#define DP_Port_D2      0 //none
#define DP_Port_D3      0 //none
#define DP_Port_D4      1 //DP
#define DP_Port_D5      0 //none
#define DP_Port_Num     (DP_Port_D0+DP_Port_D1+DP_Port_D2+DP_Port_D3+DP_Port_D4+DP_Port_D5)
//=================================================
//for switch input port priority and input source OSD item priority setting
#define DVI_PORT_PRIORITY       6 //Input_Digital5
#define DVI2ND_PORT_PRIORITY    6 //Input_Nums
#define DVI3ND_PORT_PRIORITY    6 //Input_Nums
#define HDMI_PORT_PRIORITY      2 //Input_Digital3
#define HDMI2ND_PORT_PRIORITY   3 //Input_Digital4
#define HDMI3ND_PORT_PRIORITY   4 //Input_Nums
#define HDMI4ND_PORT_PRIORITY   5 //Input_Nums
#define DP_PORT_PRIORITY        1 //Input_Digital1
#define DP2ND_PORT_PRIORITY     6 //Input_Digital2
#define DP3ND_PORT_PRIORITY     6 //Input_Nums
#define VD_PORT_PRIORITY        6 //Input_Digital6

//=================================================

#define SERIAL_BAUD_RATE        9600

#define BACKLIGHT_PWM                           _PWM1_GP1_//Driving Non-LED BL Module      [1] PWM1 on GPIO55
#define BACKLIGHT_PWM_FREQ                  240         //unit:Hz
#define BACKLIGHT_PWM_DUTY                  0x7F        //range:0~0xFF  0x7F=50%
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_
#define BACKLIGHT_PWM_VS_ALG_NUM        _NO_USE_    //vsync align number
#if AudioFunc
#define VOLUME_PWM                          _NO_USE_    // Not Finishhed Yet in CHIP_FAMILY_TSUMC //_DISP_PWM3_GP0_
#define VOLUME_PWM_FREQ                     240
#define VOLUME_PWM_DUTY                     0x7F                //0~0xFF  0x7F=50%
#define VOLUME_PWM_CONFIG               _PWM_DB_EN_
#define VOLUME_PWM_VS_ALG_NUM           _NO_USE_
#endif

#define NonUsepin
//================================================================================================================
//================================================================================================================
// 1.Write protection:
#define hwDDC_WP_Pin            //Pull to Ground in Schematic
#define hwEEPROM_WP             //GPIO03
#define hwFlash_WP_Pin          //GPIO10

// ddc eeprom wp be pulled high to 5V, set to input for output high level
#define hw_SetDDC_WP()          //(_MEM_MSWRITE_BIT(_REG_PMGPIO2_OUT, 1, BIT5), DDCWPActiveFlag=1)//(_MEM_MSWRITE_BIT(_REG_PMGPIO1_OUT, 0, BIT6), _MEM_MSWRITE_BIT(_REG_PMGPIO1_OEZ, 0, BIT6), DDCWPActiveFlag=0)
#define hw_ClrDDC_WP()          //(_MEM_MSWRITE_BIT(_REG_PMGPIO2_OUT, 0, BIT5), DDCWPActiveFlag=0)
#define Init_hwDDC_WP_Pin()     //_MEM_MSWRITE_BIT(_REG_PMGPIO2_OEZ, 0, BIT5)//(_MEM_MSWRITE_BIT(_REG_PMGPIO1_OEZ, 1, BIT6)) // output disable

#define Set_EEPROM_WP()         MEM_MSWRITE_BIT(_REG_GPIO2_OUT, 1, BIT4)
#define Clr_EEPROM_WP()         MEM_MSWRITE_BIT(_REG_GPIO2_OUT, 0, BIT4)
#define Init_hwEEPROM_WP()      MEM_MSWRITE_BIT(_REG_GPIO2_OEZ, 0, BIT4)

#define hw_SetFlashWP()         MEM_MSWRITE_BIT(_REG_GPIO1_OUT, 0, BIT0)
#define hw_ClrFlashWP()         MEM_MSWRITE_BIT(_REG_GPIO1_OUT, 1, BIT0)
#define Init_hwFlash_WP_Pin()   MEM_MSWRITE_BIT(_REG_GPIO1_OEZ, 0, BIT0)

//================================================================================================================
//================================================================================================================
// 2.Backlight and panelon:
#define hw_SetBlacklit()        (MEM_MSWRITE_BIT(_REG_GPIO0_OUT, 0, BIT6), Set_BacklightFlag())
#define hw_ClrBlacklit()        (MEM_MSWRITE_BIT(_REG_GPIO0_OUT, 1, BIT6), Clr_BacklightFlag())
#define Init_hwBlacklit_Pin()   MEM_MSWRITE_BIT(_REG_GPIO0_OEZ, 0, BIT6)

#define hw_SetPanel()           MEM_MSWRITE_BIT(_REG_GPIOSAR_OUT, 0, BIT6)
#define hw_ClrPanel()           MEM_MSWRITE_BIT(_REG_GPIOSAR_OUT, 1, BIT6)
#define Init_hwPanel_Pin()      MEM_MSWRITE_BIT(_REG_GPIOSAR_OEZ, 0, BIT2)

//================================================================================================================
//================================================================================================================
// 4.Detect Cable
//-----------------------------
// Cable Detect : VGA Port
//-----------------------------
#define CABLE_DETECT_VGA_USE_SAR    0
#define VGA_CABLE_DET_SAR_EN        (CABLE_DET_VGA_SAR)
#if CABLE_DETECT_VGA_USE_SAR
#define hwDSUBCable_Pin             0
#define Init_hwDSUBCable_Pin()
#define CABLE_DET_VGA_SAR           SAR3_EN // 2.8V
#else
#define hwDSUBCable_Pin             (_bit7_(MEM_MSREAD_BYTE(_REG_GPIO5_IN)))  //GPIO57
#define Init_hwDSUBCable_Pin()      (MEM_MSWRITE_BYTE_MASK(_REG_GPIO5_OEZ, BIT7, BIT7))
#define CABLE_DET_VGA_SAR           0
#endif
//-----------------------------
// Cable Detect : TMDS Port
//-----------------------------
#define TMDS_CABLE_DET_SAR2_EN      (CABLE_DET_DIGITAL0_SAR|CABLE_DET_DIGITAL1_SAR|CABLE_DET_DIGITAL2_SAR|CABLE_DET_DIGITAL3_SAR)
#if TMDS_Port_D0 // SAR08 //HDMI use SAR and DVI use GPIO
#define hwDigital0Cable_Pin         (KEYPAD_SAR08)
#define Init_hwDigital0Cable_Pin()  (MEM_MSWRITE_BIT(REG_003A69,1,BIT0)) // Enable SAR ch
#define CABLE_DET_DIGITAL0_SAR      SAR8_EN
#else
#define hwDigital0Cable_Pin         0
#define Init_hwDigital0Cable_Pin()
#define CABLE_DET_DIGITAL0_SAR      0
#endif
#if TMDS_Port_D1 // SAR07
#define hwDigital1Cable_Pin         (KEYPAD_SAR07)
#define Init_hwDigital1Cable_Pin()  (MEM_MSWRITE_BIT(REG_003A68,1,BIT7)) // Enable SAR ch
#define CABLE_DET_DIGITAL1_SAR      SAR7_EN
#else
#define hwDigital1Cable_Pin         0
#define Init_hwDigital1Cable_Pin()
#define CABLE_DET_DIGITAL1_SAR      0
#endif
#if TMDS_Port_D2 // SAR10
#define hwDigital2Cable_Pin         (KEYPAD_SAR10)
#define Init_hwDigital2Cable_Pin()  (MEM_MSWRITE_BIT(REG_003A69,1,BIT2)) // Enable SAR ch
#define CABLE_DET_DIGITAL2_SAR      SAR10_EN
#else
#define hwDigital2Cable_Pin         0
#define Init_hwDigital2Cable_Pin()
#define CABLE_DET_DIGITAL2_SAR      0
#endif
#if TMDS_Port_D3// SAR9
#define hwDigital3Cable_Pin         (KEYPAD_SAR09)
#define Init_hwDigital3Cable_Pin()  (MEM_MSWRITE_BIT(REG_003A69,1,BIT1)) // Enable SAR ch
#define CABLE_DET_DIGITAL3_SAR      SAR9_EN
#else
#define hwDigital3Cable_Pin         0
#define Init_hwDigital3Cable_Pin()
#define CABLE_DET_DIGITAL3_SAR      0
#endif
#if TMDS_Port_D4
#define hwDigital4Cable_Pin         0
#define Init_hwDigital4Cable_Pin()
#define CABLE_DET_DIGITAL4_SAR      0
#else
#define hwDigital4Cable_Pin         0
#define Init_hwDigital4Cable_Pin()
#define CABLE_DET_DIGITAL4_SAR      0
#endif
#if TMDS_Port_D5
#define hwDigital5Cable_Pin         0
#define Init_hwDigital5Cable_Pin()  0
#define CABLE_DET_DIGITAL5_SAR      0
#else
#define hwDigital5Cable_Pin         0
#define Init_hwDigital5Cable_Pin()
#define CABLE_DET_DIGITAL5_SAR      0
#endif
//-----------------------------
// Cable Detect : DP Port
//-----------------------------
#if DP_Port_D0
#define hwSDMDP0Pin_Pin             0//(_bit0_(MEM_MSREAD_BYTE(_REG_GPIO_X0_IN)))      //GPIOX00
#define Init_hw_DP0_SDMPin()        //(MEM_MSWRITE_BIT(_REG_GPIO_X0_OEZ, TRUE, BIT0))
#else
#define hwSDMDP0Pin_Pin             0
#define Init_hw_DP0_SDMPin()
#endif
#if DP_Port_D1
#define hwSDMDP1Pin_Pin             0
#define Init_hw_DP1_SDMPin()        0
#else
#define hwSDMDP1Pin_Pin             0
#define Init_hw_DP1_SDMPin()
#endif
#if DP_Port_D2
#define hwSDMDP2Pin_Pin             0
#define Init_hw_DP2_SDMPin()
#else
#define hwSDMDP2Pin_Pin             0
#define Init_hw_DP2_SDMPin()
#endif
#if DP_Port_D3
#define hwSDMDP3Pin_Pin             0
#define Init_hw_DP3_SDMPin()
#else
#define hwSDMDP3Pin_Pin             0
#define Init_hw_DP3_SDMPin()
#endif
#if DP_Port_D4
	#if ENABLE_DC_SDM_FORWARD
#define hwSDMDP4Pin_Pin         	(KEYPAD_SAR03<=((255ul*200)/330)?TRUE:FALSE)	// FALSE: connected, TRUE: not connected
#define Init_hw_DP4_SDMPin()        {MEM_MSWRITE_BIT(_REG_GPIOSAR_OEZ, TRUE, BIT3);\
									 MEM_MSWRITE_BIT(REG_003A62, TRUE, BIT3);MEM_MSWRITE_BIT(REG_003A68, TRUE, BIT3);\
									 MEM_MSWRITE_BIT(_REG_GPIO_X0_OEZ, FALSE, BIT0);}	// SAR3 for SDM detect, GPIOX00 for SDM forward									 
#define DC_SDM_DET_SAR_EN			SAR3_EN
	#else
#define hwSDMDP4Pin_Pin             (_bit0_(MEM_MSREAD_BYTE(_REG_GPIO_X0_IN)))      //GPIOX00
#define Init_hw_DP4_SDMPin()        (MEM_MSWRITE_BIT(_REG_GPIO_X0_OEZ, TRUE, BIT0))
	#endif
#else
#define hwSDMDP4Pin_Pin             0
#define Init_hw_DP4_SDMPin()
#endif
#if DP_Port_D5
#define hwSDMDP5Pin_Pin             0
#define Init_hw_DP5_SDMPin()
#else
#define hwSDMDP5Pin_Pin             0
#define Init_hw_DP5_SDMPin()
#endif

//================================================================================================================
//================================================================================================================
// 5.Led
#define hwGreenLed_Pin                  // GPIO21
#define hwAmberLed_Pin                  // GPIO20

#define hw_IsGreenLedOn                 (_bit1_(MEM_MSREAD_BYTE(_REG_GPIO2_OUT)))

#define hw_SetGreenLed()                {MEM_MSWRITE_BIT(_REG_GPIO2_OEZ,0,BIT1);MEM_MSWRITE_BIT(_REG_GPIO2_OUT,1,BIT1);}
#define hw_ClrGreenLed()                {MEM_MSWRITE_BIT(_REG_GPIO2_OEZ,0,BIT1);MEM_MSWRITE_BIT(_REG_GPIO2_OUT,0,BIT1);}
#define Init_hwGreenLed_Pin()           MEM_MSWRITE_BIT(_REG_GPIO2_OEZ, 0,BIT1)// output enable

#define hw_SetAmberLed()                {MEM_MSWRITE_BIT(_REG_GPIO2_OEZ,0,BIT0);MEM_MSWRITE_BIT(_REG_GPIO2_OUT, 0, BIT0);}
#define hw_ClrAmberLed()                {MEM_MSWRITE_BIT(_REG_GPIO2_OEZ,0,BIT0);MEM_MSWRITE_BIT(_REG_GPIO2_OUT, 1, BIT0);}
#define Init_hwAmberLed_Pin()           MEM_MSWRITE_BIT(_REG_GPIO2_OEZ, 0,BIT0)// output enable

//================================================================================================================
//================================================================================================================
// 6.Audio control:
#define hwMute_Pin                      //NO_USE_PIN
#define hw_SetMute()                    //(_MEM_MSWRITE_BIT(_REG_PMGPIO1_OUT, FALSE, BIT4))
#define hw_ClrMute()                    //(_MEM_MSWRITE_BIT(_REG_PMGPIO1_OUT, TRUE, BIT4))
#define Init_hwMute_Pin()               //(_MEM_MSWRITE_BIT(_REG_PMGPIO1_OEZ, FALSE, BIT4))

//to enable demo board's external DAC , // Set demo board's AUMUTE_S and AUMUTE pin to high(enable pin and mute pin of lineout and I2S amplifier)
#define hw_SetAudio_SD()            {MEM_MSWRITE_BIT(_REG_GPIO0_OUT, FALSE, BIT7);MEM_MSWRITE_BIT(_REG_GPIOAU_OUT_L, FALSE, BIT0);}
#define hw_ClrAudio_SD()                {MEM_MSWRITE_BIT(_REG_GPIO0_OUT, TRUE, BIT7);MEM_MSWRITE_BIT(_REG_GPIOAU_OUT_L, TRUE, BIT0);}
#define Init_Audio_SD_Pin()         {MEM_MSWRITE_BIT(_REG_GPIO0_OEZ, FALSE, BIT7);MEM_MSWRITE_BIT(_REG_GPIOAU_OEZ_L, FALSE, BIT0);MEM_MSWRITE_BIT(_REG_GPIOAU_SEL_L, FALSE, BIT0);}

#if ENABLE_I2S_PassThroughDAC
#define Init_SPIFGPIO_Pin()         	(MEM_MSWRITE_BIT(REG_000406, TRUE, BIT2))  
// 1: Enable AUMUTE pad control  //dont enable HDMI's AUMUTE pin function and set GPIO_A0 to high to enable line-out amplifier of demo board.
#define Init_I2S_Switch_Pin()		{MEM_MSWRITE_BYTE_MASK(REG_000407,BIT6|BIT5/*|BIT4*/,BIT6|BIT5/*|BIT4*/);MEM_MSWRITE_BYTE_MASK(REG_000416,BIT4,BIT4);} //[1]:reg_i2s_out_au_en [2]:reg_i2s_out_mck_au_en
//#define Init_I2S_Switch_Pin()		{MEM_MSWRITE_BYTE_MASK(REG_000407,BIT6|BIT5|BIT4,BIT6|BIT5|BIT4);MEM_MSWRITE_BYTE_MASK(REG_000416,BIT4,BIT4);} //[1]:reg_i2s_out_au_en [2]:reg_i2s_out_mck_au_en
#else //direct output to PAD from HDMI
#define Init_SPIFGPIO_Pin()         	(MEM_MSWRITE_BIT(REG_000406, TRUE, BIT0))  
#define Init_I2S_Switch_Pin()		{MEM_MSWRITE_BYTE_MASK(REG_000407,BIT1|BIT0,BIT1|BIT0);/*MEM_MSWRITE_BYTE_MASK(REG_000406,BIT5,BIT5);*/MEM_MSWRITE_BYTE_MASK(REG_000416,BIT0,BIT0);} //[1]:reg_i2s_out_au_en [2]:reg_i2s_out_mck_au_en
#endif

//Since GPIO01 is the power switch on demo board, output S/PDIF on this pad will cause latch up when AC on.
//The register definition is wrong.

//================================================================================================================
//================================================================================================================
// I2C
#if defined(UseVGACableReadWriteAllPortsEDID) //&&!defined(UseInternalDDCRam)
#define Set_i2c_SDA_VGA()           //(_MEM_MSWRITE_BIT(_REG_PMGPIO1_OEZ, TRUE, BIT2))
#define Clr_i2c_SDA_VGA()           //(_MEM_MSWRITE_BIT(_REG_PMGPIO1_OUT, FALSE, BIT2), _MEM_MSWRITE_BIT(_REG_PMGPIO1_OEZ, FALSE, BIT2))
#define i2c_SDAHi_VGA()             //(_bit2_(_MEM_MSREAD_BYTE(_REG_PMGPIO1_IN)))
#define i2c_SDALo_VGA()             //(!i2c_SDAHi())
#define Set_i2c_SCL_VGA()           //(_MEM_MSWRITE_BIT(_REG_PMGPIO1_OEZ, TRUE, BIT3))
#define Clr_i2c_SCL_VGA()           //(_MEM_MSWRITE_BIT(_REG_PMGPIO1_OUT, FALSE, BIT3), _MEM_MSWRITE_BIT(_REG_PMGPIO1_OEZ, FALSE, BIT3))
#define i2c_SCLHi_VGA()             //(_bit3_(_MEM_MSREAD_BYTE(_REG_PMGPIO1_IN)))
#define i2c_SCLLo_VGA()             //(!i2c_SCLHi())

#define Set_i2c_SDA_DVI()           //(_MEM_MSWRITE_BIT(_REG_PMGPIO2_OEZ, TRUE, BIT1))
#define Clr_i2c_SDA_DVI()           //(_MEM_MSWRITE_BIT(_REG_PMGPIO2_OUT, FALSE, BIT1), _MEM_MSWRITE_BIT(_REG_PMGPIO2_OEZ, FALSE, BIT1))
#define i2c_SDAHi_DVI()             //(_bit1_(_MEM_MSREAD_BYTE(_REG_PMGPIO2_IN)))
#define i2c_SDALo_DVI()             //(!i2c_SDAHi())
#define Set_i2c_SCL_DVI()           //(_MEM_MSWRITE_BIT(_REG_PMGPIO2_OEZ, TRUE, BIT2))
#define Clr_i2c_SCL_DVI()           //(_MEM_MSWRITE_BIT(_REG_PMGPIO2_OUT, FALSE, BIT2), _MEM_MSWRITE_BIT(_REG_PMGPIO2_OEZ, FALSE, BIT2))
#define i2c_SCLHi_DVI()             //(_bit2_(_MEM_MSREAD_BYTE(_REG_PMGPIO2_IN)))
#define i2c_SCLLo_DVI()             //(!i2c_SCLHi())

#define Set_i2c_SDA()               //((LoadEDIDSelectPort == DVI1)?(Set_i2c_SDA_DVI()):(Set_i2c_SDA_VGA()))
#define Clr_i2c_SDA()               //((LoadEDIDSelectPort == DVI1)?(Clr_i2c_SDA_DVI()):(Clr_i2c_SDA_VGA()))
#define i2c_SDAHi()                 0//((LoadEDIDSelectPort == DVI1)?(i2c_SDAHi_DVI()):(i2c_SDAHi_VGA()))
#define i2c_SDALo()                 0//((LoadEDIDSelectPort == DVI1)?(i2c_SDALo_DVI()):(i2c_SDALo_VGA()))

#define Set_i2c_SCL()               //((LoadEDIDSelectPort == DVI1)?(Set_i2c_SCL_DVI()):(Set_i2c_SCL_VGA()))
#define Clr_i2c_SCL()               //((LoadEDIDSelectPort == DVI1)?(Clr_i2c_SCL_DVI()):(Clr_i2c_SCL_VGA()))
#define i2c_SCLHi()                 0//((LoadEDIDSelectPort == DVI1)?(i2c_SCLHi_DVI()):(i2c_SCLHi_VGA()))
#define i2c_SCLLo()                 0//((LoadEDIDSelectPort == DVI1)?(i2c_SCLLo_DVI()):(i2c_SCLLo_VGA()))
#endif

#if !USEFLASH
#define hwI2C_SCL_Pin               // GPIO00
#define hwI2C_SDA_Pin               // GPIO01
#define Init_hwI2C_SCL_Pin()        (MEM_MSWRITE_BIT(_REG_GPIO0_OEZ, TRUE, BIT0))
#define Init_hwI2C_SDA_Pin()        (MEM_MSWRITE_BIT(_REG_GPIO0_OEZ, TRUE, BIT1))

#define Set_i2c_SDA()               (MEM_MSWRITE_BIT(_REG_GPIO0_OEZ, TRUE, BIT1))
#define Clr_i2c_SDA()               (MEM_MSWRITE_BIT(_REG_GPIO0_OUT, FALSE, BIT1), MEM_MSWRITE_BIT(_REG_GPIO0_OEZ, FALSE, BIT1))
#define i2c_SDAHi()                 (_bit1_(MEM_MSREAD_BYTE(_REG_GPIO0_IN)))
#define i2c_SDALo()                 (!i2c_SDAHi())
#define Set_i2c_SCL()               (MEM_MSWRITE_BIT(_REG_GPIO0_OEZ, TRUE, BIT0))
#define Clr_i2c_SCL()               (MEM_MSWRITE_BIT(_REG_GPIO0_OUT, FALSE, BIT0), MEM_MSWRITE_BIT(_REG_GPIO0_OEZ, FALSE, BIT0))
#define i2c_SCLHi()                 (_bit0_(MEM_MSREAD_BYTE(_REG_GPIO0_IN)))
#define i2c_SCLLo()                 (!i2c_SCLHi())
#endif

#if ENABLE_HDCP
#define hwHDCP_Hpd_Pin

#define hw_Set_HdcpHpd()        (MEM_MSWRITE_BIT(_REG_GPIO_X3_OUT, TRUE, BIT2), MEM_MSWRITE_BIT(_REG_GPIO_X3_OEZ, TRUE, BIT2), MEM_MSWRITE_BIT(REG_002FF2, TRUE, BIT0))
#define hw_Clr_HdcpHpd()        (MEM_MSWRITE_BIT(_REG_GPIO_X3_OUT, FALSE, BIT2), MEM_MSWRITE_BIT(_REG_GPIO_X3_OEZ, FALSE, BIT2), MEM_MSWRITE_BIT(REG_002FF2, TRUE, BIT0))
#define Init_hwHDCP_Hpd_Pin()   (hw_Clr_HdcpHpd())//(hw_Set_HdcpHpd()) //default high
#define hw_Set_HdcpHpd1()       (MEM_MSWRITE_BIT(_REG_GPIO_X4_OUT, TRUE, BIT2), MEM_MSWRITE_BIT(_REG_GPIO_X4_OEZ, TRUE, BIT2), MEM_MSWRITE_BIT(REG_0031F2, TRUE, BIT0))
#define hw_Clr_HdcpHpd1()       (MEM_MSWRITE_BIT(_REG_GPIO_X4_OUT, FALSE, BIT2), MEM_MSWRITE_BIT(_REG_GPIO_X4_OEZ, FALSE, BIT2), MEM_MSWRITE_BIT(REG_0031F2, TRUE, BIT0))
#define Init_hwHDCP_Hpd_Pin1()  (hw_Clr_HdcpHpd1())//(hw_Set_HdcpHpd1()) //default high
#define hw_Set_HdcpHpd2()       (MEM_MSWRITE_BIT(_REG_GPIO_X1_OUT, TRUE, BIT2), MEM_MSWRITE_BIT(_REG_GPIO_X1_OEZ, TRUE, BIT2), MEM_MSWRITE_BIT(REG_0033F2, TRUE, BIT0))
#define hw_Clr_HdcpHpd2()       (MEM_MSWRITE_BIT(_REG_GPIO_X1_OUT, FALSE, BIT2), MEM_MSWRITE_BIT(_REG_GPIO_X1_OEZ, FALSE, BIT2), MEM_MSWRITE_BIT(REG_0033F2, TRUE, BIT0))
#define Init_hwHDCP_Hpd_Pin2()  (hw_Clr_HdcpHpd2())//(hw_Set_HdcpHpd2()) //default high
#define hw_Set_HdcpHpd3()       (MEM_MSWRITE_BIT(_REG_GPIO_X2_OUT, TRUE, BIT2), MEM_MSWRITE_BIT(_REG_GPIO_X2_OEZ, TRUE, BIT2), MEM_MSWRITE_BIT(REG_0035F2, TRUE, BIT0))
#define hw_Clr_HdcpHpd3()       (MEM_MSWRITE_BIT(_REG_GPIO_X2_OUT, FALSE, BIT2), MEM_MSWRITE_BIT(_REG_GPIO_X2_OEZ, FALSE, BIT2), MEM_MSWRITE_BIT(REG_0035F2, TRUE, BIT0))
#define Init_hwHDCP_Hpd_Pin3()  (hw_Clr_HdcpHpd3())//(hw_Set_HdcpHpd3()) //default high
#define hw_Set_HdcpHpd4()       //(MEM_MSWRITE_BIT(_REG_GPIO_X4_OUT, FALSE, BIT2), MEM_MSWRITE_BIT(_REG_GPIO_X4_OEZ, FALSE, BIT2))
#define hw_Clr_HdcpHpd4()       //(MEM_MSWRITE_BIT(_REG_GPIO_X4_OUT, TRUE, BIT2), MEM_MSWRITE_BIT(_REG_GPIO_X4_OEZ, FALSE, BIT2))
#define Init_hwHDCP_Hpd_Pin4()  //(hw_Set_HdcpHpd4())  //default high
#define hw_Set_HdcpHpd5()       //(MEM_MSWRITE_BIT(_REG_GPIO_X4_OUT, FALSE, BIT4), MEM_MSWRITE_BIT(_REG_GPIO_X4_OEZ, FALSE, BIT1))
#define hw_Clr_HdcpHpd5()       //(MEM_MSWRITE_BIT(_REG_GPIO_X4_OUT, TRUE, BIT4), MEM_MSWRITE_BIT(_REG_GPIO_X4_OEZ, FALSE, BIT1))
#define Init_hwHDCP_Hpd_Pin5()  //(hw_Set_HdcpHpd5()) //default high
#endif //ENABLE_HDCP

#if ENABLE_DP_INPUT

#if ENABLE_DAISY_CHAIN 
#define Init_hwDP_TXPWR_Pin()         (MEM_MSWRITE_BIT(_REG_GPIO6_OUT, TRUE, BIT1), MEM_MSWRITE_BIT(_REG_GPIO6_OEZ, FALSE, BIT1)) //DP Tx PWR pin should default be 3.3V
#define hw_Set_DP_TXPWR_Pin()		   (MEM_MSWRITE_BIT(_REG_GPIO6_OUT, TRUE, BIT1), MEM_MSWRITE_BIT(_REG_GPIO6_OEZ, FALSE, BIT1)) //DP Tx PWR pin
#define hw_Clr_DP_TXPWR_Pin()		   (MEM_MSWRITE_BIT(_REG_GPIO6_OUT, FALSE, BIT1), MEM_MSWRITE_BIT(_REG_GPIO6_OEZ, FALSE, BIT1)) //DP Tx PWR pin
#endif //#if ENABLE_DAISY_CHAIN 

#define Init_hwDP_Hpd_Pin0()    //(MEM_MSWRITE_BIT(REG_000462, TRUE, BIT1),MEM_MSWRITE_BIT(_REG_GPIO_X0_OUT, FALSE, BIT2), MEM_MSWRITE_BIT(_REG_GPIO_X0_OEZ, FALSE, BIT2))
#define hw_Set_DPHpd0()         //(MEM_MSWRITE_BIT(REG_000462, TRUE, BIT1),MEM_MSWRITE_BIT(_REG_GPIO_X0_OUT, TRUE, BIT2), MEM_MSWRITE_BIT(_REG_GPIO_X0_OEZ, FALSE, BIT2))
#define hw_Clr_DPHpd0()         //(MEM_MSWRITE_BIT(_REG_GPIO_X0_OUT, FALSE, BIT2), MEM_MSWRITE_BIT(_REG_GPIO_X0_OEZ, FALSE, BIT2)) //default high

#define Init_hwDP_Hpd_Pin1()    //(MEM_MSWRITE_BIT(REG_000462, TRUE, BIT1),MEM_MSWRITE_BIT(_REG_GPIO_X0_OUT, FALSE, BIT2), MEM_MSWRITE_BIT(_REG_GPIO_X0_OEZ, FALSE, BIT2))
#define hw_Set_DPHpd1()         //(MEM_MSWRITE_BIT(REG_000462, TRUE, BIT1),MEM_MSWRITE_BIT(_REG_GPIO_X0_OUT, TRUE, BIT2), MEM_MSWRITE_BIT(_REG_GPIO_X0_OEZ, FALSE, BIT2))
#define hw_Clr_DPHpd1()         //(MEM_MSWRITE_BIT(_REG_GPIO_X0_OUT, FALSE, BIT2), MEM_MSWRITE_BIT(_REG_GPIO_X0_OEZ, FALSE, BIT2)) //default high //default high

#define Init_hwDP_Hpd_Pin2()    //(MEM_MSWRITE_BIT(REG_000462, TRUE, BIT5),MEM_MSWRITE_BIT(_REG_GPIO_X2_OUT, FALSE, BIT2), MEM_MSWRITE_BIT(_REG_GPIO_X2_OEZ, FALSE, BIT2))
#define hw_Set_DPHpd2()         //(MEM_MSWRITE_BIT(REG_000462, TRUE, BIT5),MEM_MSWRITE_BIT(_REG_GPIO_X2_OUT, TRUE, BIT2), MEM_MSWRITE_BIT(_REG_GPIO_X2_OEZ, FALSE, BIT2))
#define hw_Clr_DPHpd2()         //(MEM_MSWRITE_BIT(_REG_GPIO_X2_OUT, FALSE, BIT2), MEM_MSWRITE_BIT(_REG_GPIO_X2_OEZ, FALSE, BIT2)) //default high

#define Init_hwDP_Hpd_Pin3()    //(MEM_MSWRITE_BIT(REG_000462, TRUE, BIT7),MEM_MSWRITE_BIT(_REG_GPIO_X3_OUT, FALSE, BIT2), MEM_MSWRITE_BIT(_REG_GPIO_X3_OEZ, FALSE, BIT2))
#define hw_Set_DPHpd3()         //(MEM_MSWRITE_BIT(REG_000462, TRUE, BIT7),MEM_MSWRITE_BIT(_REG_GPIO_X3_OUT, TRUE, BIT2), MEM_MSWRITE_BIT(_REG_GPIO_X3_OEZ, FALSE, BIT2))
#define hw_Clr_DPHpd3()         //(MEM_MSWRITE_BIT(_REG_GPIO_X3_OUT, FALSE, BIT2), MEM_MSWRITE_BIT(_REG_GPIO_X3_OEZ, FALSE, BIT2)) //default high

#define Init_hwDP_Hpd_Pin4()    (MEM_MSWRITE_BIT(REG_000462, TRUE, BIT1),MEM_MSWRITE_BIT(_REG_GPIO_X0_OUT, FALSE, BIT2), MEM_MSWRITE_BIT(_REG_GPIO_X0_OEZ, FALSE, BIT2))
#define hw_Set_DPHpd4()         (MEM_MSWRITE_BIT(REG_000462, TRUE, BIT1),MEM_MSWRITE_BIT(_REG_GPIO_X0_OUT, TRUE, BIT2), MEM_MSWRITE_BIT(_REG_GPIO_X0_OEZ, FALSE, BIT2))
#define hw_Clr_DPHpd4()         (MEM_MSWRITE_BIT(_REG_GPIO_X0_OUT, FALSE, BIT2), MEM_MSWRITE_BIT(_REG_GPIO_X0_OEZ, FALSE, BIT2)) //default high

#define Init_hwDP_RXPWR_Pin4()         (MEM_MSWRITE_BIT(_REG_GPIO_X0_OUT, TRUE, BIT1), MEM_MSWRITE_BIT(_REG_GPIO_X0_OEZ, FALSE, BIT1)) //DP Rx PWR pin should default be 3.3V
#define hw_Set_DP_RXPWR_Pin4()		   (MEM_MSWRITE_BIT(_REG_GPIO_X0_OUT, TRUE, BIT1), MEM_MSWRITE_BIT(_REG_GPIO_X0_OEZ, FALSE, BIT1)) //DP Rx PWR pin
#define hw_Clr_DP_RXPWR_Pin4()		   (MEM_MSWRITE_BIT(_REG_GPIO_X0_OUT, FALSE, BIT1), MEM_MSWRITE_BIT(_REG_GPIO_X0_OEZ, FALSE, BIT1)) //DP Rx PWR pin

#define Init_hwDP_Hpd_Pin5()    //(MEM_MSWRITE_BIT(REG_000463, TRUE, BIT3),MEM_MSWRITE_BIT(_REG_GPIO_X5_OUT, FALSE, BIT2), MEM_MSWRITE_BIT(_REG_GPIO_X5_OEZ, FALSE, BIT2))
#define hw_Set_DPHpd5()         //(MEM_MSWRITE_BIT(REG_000463, TRUE, BIT3),MEM_MSWRITE_BIT(_REG_GPIO_X5_OUT, TRUE, BIT2), MEM_MSWRITE_BIT(_REG_GPIO_X5_OEZ, FALSE, BIT2))
#define hw_Clr_DPHpd5()         //(MEM_MSWRITE_BIT(_REG_GPIO_X5_OUT, FALSE, BIT2), MEM_MSWRITE_BIT(_REG_GPIO_X5_OEZ, FALSE, BIT2)) //default high
//================================================================================================================
#define hw_Set_DPCable5V0()     //(MEM_MSWRITE_BIT(_REG_GPIO_X0_OUT, TRUE, BIT1), MEM_MSWRITE_BIT(_REG_GPIO_X0_OEZ, FALSE, BIT1))
#define hw_Clr_DPCable5V0()     //(MEM_MSWRITE_BIT(_REG_GPIO_X0_OUT, TRUE, BIT1), MEM_MSWRITE_BIT(_REG_GPIO_X0_OEZ, TRUE, BIT1))
#define Init_hwDP_Cable5V0()    //(MEM_MSWRITE_BIT(_REG_GPIO_X0_OUT, FALSE, BIT1), MEM_MSWRITE_BIT(_REG_GPIO_X0_OEZ, FALSE, BIT1)) //default high

#define hw_Set_DPCable5V1()     //(MEM_MSWRITE_BIT(_REG_GPIO_X0_OUT, TRUE, BIT1), MEM_MSWRITE_BIT(_REG_GPIO_X0_OEZ, FALSE, BIT1))
#define hw_Clr_DPCable5V1()     //(MEM_MSWRITE_BIT(_REG_GPIO_X0_OUT, TRUE, BIT1), MEM_MSWRITE_BIT(_REG_GPIO_X0_OEZ, TRUE, BIT1))
#define Init_hwDP_Cable5V1()    //(MEM_MSWRITE_BIT(_REG_GPIO_X0_OUT, FALSE, BIT1), MEM_MSWRITE_BIT(_REG_GPIO_X0_OEZ, FALSE, BIT1)) //default high

#define hw_Set_DPCable5V2()     //(MEM_MSWRITE_BIT(_REG_GPIO_X2_OUT, TRUE, BIT1), MEM_MSWRITE_BIT(_REG_GPIO_X2_OEZ, FALSE, BIT1))
#define hw_Clr_DPCable5V2()     //(MEM_MSWRITE_BIT(_REG_GPIO_X2_OUT, TRUE, BIT1), MEM_MSWRITE_BIT(_REG_GPIO_X2_OEZ, TRUE, BIT1))
#define Init_hwDP_Cable5V2()    //(MEM_MSWRITE_BIT(_REG_GPIO_X2_OUT, FALSE, BIT1), MEM_MSWRITE_BIT(_REG_GPIO_X2_OEZ, FALSE, BIT1)) //default high

#define hw_Set_DPCable5V3()     //(MEM_MSWRITE_BIT(_REG_GPIO_X3_OUT, TRUE, BIT1), MEM_MSWRITE_BIT(_REG_GPIO_X3_OEZ, FALSE, BIT1))
#define hw_Clr_DPCable5V3()     //(MEM_MSWRITE_BIT(_REG_GPIO_X3_OUT, TRUE, BIT1), MEM_MSWRITE_BIT(_REG_GPIO_X3_OEZ, TRUE, BIT1))
#define Init_hwDP_Cable5V3()    //(MEM_MSWRITE_BIT(_REG_GPIO_X3_OUT, FALSE, BIT1), MEM_MSWRITE_BIT(_REG_GPIO_X3_OEZ, FALSE, BIT1)) //default high

#define hw_Set_DPCable5V4()     //(MEM_MSWRITE_BIT(_REG_GPIO_X0_OUT, TRUE, BIT1), MEM_MSWRITE_BIT(_REG_GPIO_X0_OEZ, FALSE, BIT1))
#define hw_Clr_DPCable5V4()     //(MEM_MSWRITE_BIT(_REG_GPIO_X0_OUT, TRUE, BIT1), MEM_MSWRITE_BIT(_REG_GPIO_X0_OEZ, TRUE, BIT1))
#define Init_hwDP_Cable5V4()    //(MEM_MSWRITE_BIT(_REG_GPIO_X0_OUT, FALSE, BIT1), MEM_MSWRITE_BIT(_REG_GPIO_X0_OEZ, FALSE, BIT1)) //default high

#define hw_Set_DPCable5V5()     //(MEM_MSWRITE_BIT(_REG_GPIO_X5_OUT, TRUE, BIT1), MEM_MSWRITE_BIT(_REG_GPIO_X5_OEZ, FALSE, BIT1))
#define hw_Clr_DPCable5V5()     //(MEM_MSWRITE_BIT(_REG_GPIO_X5_OUT, TRUE, BIT1), MEM_MSWRITE_BIT(_REG_GPIO_X5_OEZ, TRUE, BIT1))
#define Init_hwDP_Cable5V5()    //(MEM_MSWRITE_BIT(_REG_GPIO_X5_OUT, FALSE, BIT1), MEM_MSWRITE_BIT(_REG_GPIO_X5_OEZ, FALSE, BIT1)) //default high

#endif

#if ENABLE_USB_INPUT
#define hwUSB_Pin
#define hw_USB_HI()
#define hw_USB_LOW()
#define Init_hwUSB_Pin();
#endif

//====================================KEYPAD Setting=====================================
//SARKEY_ENABLE
#define SARKEY_EN       (SAR0_EN|SAR1_EN)

//SAR SELECT
#define KEYPAD_ADC_A    KEYPAD_SAR00
#define KEYPAD_ADC_B    KEYPAD_SAR01
//SAR_KEY_VALUE


//GPIO_KEY
#define HW_KEY_PLUS     NonUsepin
#define HW_KEY_MINUS    NonUsepin
#define HW_KEY_TICK     NonUsepin
#define HW_KEY_MENU     NonUsepin
//POWER_KEY
#define HW_POWER_KEY        (_bit6_(MEM_MSREAD_BYTE(_REG_GPIO6_IN)))
#define INIT_HW_POWER_KEY() (MEM_MSWRITE_BIT(_REG_GPIO6_OEZ, 1, BIT6))
#define PM_POWERKEY_WAKEUP      //GPIO02_INT
//=======================================================================================
//SAR_ENABLE_BIT
#define SAR0_EN         BIT0
#define SAR1_EN         BIT1
#define SAR2_EN         BIT2
#define SAR3_EN         BIT3
#define SAR4_EN         BIT4
#define SAR5_EN         BIT5
#define SAR6_EN         BIT6
#define SAR7_EN         BIT7
#define SAR8_EN         BIT8
#define SAR9_EN         BIT9
#define SAR10_EN        BIT10
#define SAR11_EN        BIT11

#if ENABLE_DC_SDM_FORWARD
#define SAR_ENABLE      (SARKEY_EN|VGA_CABLE_DET_SAR_EN|TMDS_CABLE_DET_SAR2_EN|DC_SDM_DET_SAR_EN)
#else
#define SAR_ENABLE      (SARKEY_EN|VGA_CABLE_DET_SAR_EN|TMDS_CABLE_DET_SAR2_EN)
#endif
//======================================================================================

#define AOVDV_VALUE     6// 4

//#define LVDS_CHANNEL    (MOD_PORTB|MOD_PORTC)//(MOD_PORTA|MOD_PORTB|MOD_PORTC|MOD_PORTD)
//#define I_GEN_CH        CH2

// MHL PS_CTRL
#define SET_MHL_PS_CTRL_CHARGE0()   (MEM_MSWRITE_BIT(_REG_GPIO6_OEZ, FALSE, BIT7), MEM_MSWRITE_BIT(_REG_GPIO6_OUT, TRUE, BIT7))
#define CLR_MHL_PS_CTRL_CHARGE0()   (MEM_MSWRITE_BIT(_REG_GPIO6_OEZ, FALSE, BIT7), MEM_MSWRITE_BIT(_REG_GPIO6_OUT, FALSE, BIT7))

#define SET_MHL_PS_CTRL_CHARGE1()   (MEM_MSWRITE_BIT(_REG_GPIO4_OEZ, FALSE, BIT6), MEM_MSWRITE_BIT(_REG_GPIO4_OUT, TRUE, BIT6))
#define CLR_MHL_PS_CTRL_CHARGE1()   (MEM_MSWRITE_BIT(_REG_GPIO4_OEZ, FALSE, BIT6), MEM_MSWRITE_BIT(_REG_GPIO4_OUT, FALSE, BIT6))

#define SET_MHL_PS_CTRL_CHARGE2()   (MEM_MSWRITE_BIT(_REG_GPIO2_OEZ, FALSE, BIT2), MEM_MSWRITE_BIT(_REG_GPIO2_OUT, TRUE, BIT2))
#define CLR_MHL_PS_CTRL_CHARGE2()   (MEM_MSWRITE_BIT(_REG_GPIO2_OEZ, FALSE, BIT2), MEM_MSWRITE_BIT(_REG_GPIO2_OUT, FALSE, BIT2))

#define SET_MHL_PS_CTRL_CHARGE3()   (MEM_MSWRITE_BIT(_REG_GPIO6_OEZ, FALSE, BIT0), MEM_MSWRITE_BIT(_REG_GPIO6_OUT, TRUE, BIT0))
#define CLR_MHL_PS_CTRL_CHARGE3()   (MEM_MSWRITE_BIT(_REG_GPIO6_OEZ, FALSE, BIT0), MEM_MSWRITE_BIT(_REG_GPIO6_OUT, FALSE, BIT0))

#define SET_MHL_PS_CTRL_CHARGE4()
#define CLR_MHL_PS_CTRL_CHARGE4()

#define SET_MHL_PS_CTRL_CHARGE5()
#define CLR_MHL_PS_CTRL_CHARGE5()

#if ENABLE_TOUCH_PANEL
#define Set_TOCUCH_PANEL_POWER()
#define Clr_TOCUCH_PANEL_POWER()
#define Init_TOCUCH_PANEL_POWER_Pin()
#define SPI_MISO_Pin                //(_bit2_(_MEM_MSREAD_BYTE(_REG_PMGPIO1_IN)))
#define Init_SPI_MISO_Pin()         //(_MEM_MSWRITE_BIT(_REG_PMGPIO1_OEZ, TRUE, BIT2))   // PMGPIO12

#define Set_SPI_SCK()               //(_MEM_MSWRITE_BIT(_REG_PMGPIO1_OUT, TRUE, BIT3))
#define Clr_SPI_SCK()               //(_MEM_MSWRITE_BIT(_REG_PMGPIO1_OUT, FALSE, BIT3))
#define Init_SPI_SCK_Pin()          //(_MEM_MSWRITE_BIT(_REG_PMGPIO1_OEZ, FALSE, BIT3))  // PMGPIO13

#define Set_SPI_MOSI()              //(_MEM_MSWRITE_BIT(_REG_PMGPIO1_OUT, TRUE, BIT4))
#define Clr_SPI_MOSI()              //(_MEM_MSWRITE_BIT(_REG_PMGPIO1_OUT, FALSE, BIT4))
#define Init_SPI_MOSI_Pin()         //(_MEM_MSWRITE_BIT(_REG_PMGPIO1_OEZ, FALSE, BIT4)) // PMGPIO14

#define Set_SPI_SEL()               //(_MEM_MSWRITE_BIT(_REG_PMGPIO1_OUT, TRUE, BIT5))
#define Clr_SPI_SEL()               //(_MEM_MSWRITE_BIT(_REG_PMGPIO1_OUT, FALSE, BIT5))
#define Init_SPI_SEL_Pin()          //(_MEM_MSWRITE_BIT(_REG_PMGPIO1_OEZ, FALSE, BIT5))// PMGPIO15
#endif

#define Init_CORE_PWR_VID_HIGH()    (MEM_MSWRITE_BIT(_REG_GPIO6_OUT, TRUE, BIT4), MEM_MSWRITE_BIT(_REG_GPIO6_OEZ, FALSE, BIT4), \
                                    MEM_MSWRITE_BIT(_REG_GPIO6_OUT, FALSE, BIT3), MEM_MSWRITE_BIT(_REG_GPIO6_OEZ, FALSE, BIT3))
#define Init_CORE_PWR_VID_LOW()

#define ENABLE_CEC          0
#define ENABLE_CEC_INT      0 //not implement yet, TBD

#if ENABLE_CEC

#define MAX_CEC_SET_NUM		4 //fixed, may different from each chip
#define ENALBE_CEC_SET(enable, SetNum)	( enable == TRUE ) ? (1 << SetNum) : 0x00

#define GPIO23_AS_CEC		3	//value = cec set index
#define GPIO24_AS_CEC		2	//value = cec set index
#define GPIO25_AS_CEC		1	//value = cec set index
#define GPIO26_AS_CEC		0	//value = cec set index

#if TMDS_Port_D0
	#define TMDS_P0_CEC		ENALBE_CEC_SET(TRUE, GPIO26_AS_CEC)
#else
	#define TMDS_P0_CEC		0x00
#endif
#if TMDS_Port_D1
	#define TMDS_P1_CEC		ENALBE_CEC_SET(FALSE, 0)
#else
	#define TMDS_P1_CEC		0x00
#endif
#if TMDS_Port_D2
	#define TMDS_P2_CEC		ENALBE_CEC_SET(FALSE, 0)
#else
	#define TMDS_P2_CEC		0x00
#endif
#if TMDS_Port_D3
	#define TMDS_P3_CEC		ENALBE_CEC_SET(FALSE, 0)
#else
	#define TMDS_P3_CEC		0x00
#endif
#if TMDS_Port_D4
	#define TMDS_P4_CEC		ENALBE_CEC_SET(FALSE, 0)
#else
	#define TMDS_P4_CEC		0x00
#endif
#if TMDS_Port_D5
	#define TMDS_P5_CEC		ENALBE_CEC_SET(FALSE, 0)
#else
	#define TMDS_P5_CEC		0x00
#endif

#define EN_CEC_SET_LIST		(TMDS_P0_CEC | TMDS_P1_CEC | TMDS_P2_CEC | TMDS_P3_CEC | TMDS_P4_CEC | TMDS_P5_CEC)

#endif //end of #if ENABLE_CEC




#endif // _MSBOARD_H_


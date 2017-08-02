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
///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2008-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting
// therefrom.
//
//
/// @file  regAudio.h
/// @brief Hardware register definition for Video Decoder
/// @author MStar Semiconductor Inc.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _REG_AUDIO_H_
#define _REG_AUDIO_H_

#include "sysinfo2_MST9U.h"

typedef enum
{
    AUDIO_FMT_NON_PCM   = 0,
    AUDIO_FMT_PCM       = 1,
    AUDIO_FMT_UNKNOWN   = 2,
} AUDIO_FORMAT;

typedef enum
{
    AUDIO_2_ch   = 0,
    AUDIO_8_ch       = 1,
    AUDIO_ch_UNKNOWN   = 2,
} AUDIO_Get_ChannelNum;




///////////////////////////////////////////////////////////////////////////////
// Constant & Macro Definition
///////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------
/// DMA Relation Define
//-------------------------------------------------------------------------------------------------
#define DMA_READER_WRITE_LINE_SIZE_INIT         256     //(DMA_READER_BUFFER_LINE_SIZE >> 3)
#define DMA_WRITER_READ_LINE_SIZE_INIT          256     //(DMA_WRITER_BUFFER_LINE_SIZE >> 3)

#define DMA_READER_BLOCK_NUM                    32      //32
#define DMA_WRITER_BLOCK_NUM                    256

#define DMA_WRITER_BUFFER_LINE_SIZE             DMA_WRITER_READ_LINE_SIZE_INIT * DMA_WRITER_BLOCK_NUM     //8192
#define DMA_READER_BUFFER_LINE_SIZE             DMA_READER_WRITE_LINE_SIZE_INIT * DMA_READER_BLOCK_NUM    //8192
#define DMA_WRITER_BUFFER_BYTE_SIZE             (DMA_WRITER_BUFFER_LINE_SIZE*16)        //DMA_WRITER_BUFFER_LINE_SIZE * 16 bytes
#define DMA_READER_BUFFER_BYTE_SIZE             (DMA_READER_BUFFER_LINE_SIZE*16)        //DMA_READER_WRITE_LINE_SIZE * 16 bytes

#define DMA_WriterRead_2LineSize                (DMA_WRITER_READ_LINE_SIZE_INIT  >> 1)
#define DMA_ReaderWrite_2LineSize               (DMA_READER_WRITE_LINE_SIZE_INIT >> 1)


// 0x1402AA
#define DMA_READER_OVERRUN_THR1                 ( DMA_READER_WRITE_LINE_SIZE_INIT * ( DMA_READER_BLOCK_NUM - 4 ))
// 0x1402AA
#define DMA_READER_OVERRUN_THR2                 ( DMA_READER_WRITE_LINE_SIZE_INIT * ( DMA_READER_BLOCK_NUM - 4 ))
// 0x1402AA
#define DMA_READER_OVERRUN_THR3                 ( DMA_READER_WRITE_LINE_SIZE_INIT * ( DMA_READER_BLOCK_NUM - 4 ))
// 0x1402AC
#define DMA_READER_UNDERRUN_THR1                ( DMA_READER_WRITE_LINE_SIZE_INIT * 2 )
// 0x1402AC
#define DMA_READER_UNDERRUN_THR2                ( DMA_READER_WRITE_LINE_SIZE_INIT * 2 )
// 0x1402AC
#define DMA_READER_UNDERRUN_THR3                ( DMA_READER_WRITE_LINE_SIZE_INIT * 2 )

 // 0x1402BA
#define DMA_WRITER_OVERRUN_THR1                 ( DMA_WRITER_READ_LINE_SIZE_INIT * 12 )
 // 0x1402BA
#define DMA_WRITER_OVERRUN_THR2                 ( DMA_WRITER_READ_LINE_SIZE_INIT * 10 )
 // 0x1402BA
#define DMA_WRITER_OVERRUN_THR3                 ( DMA_WRITER_READ_LINE_SIZE_INIT * 5 )
// 0x1402BC
#define DMA_WRITER_UNDERRUN_THR1                ( DMA_WRITER_READ_LINE_SIZE_INIT * 2 )
// 0x1402BC
#define DMA_WRITER_UNDERRUN_THR2                ( DMA_WRITER_READ_LINE_SIZE_INIT * 2 )


//-------------------------------------------------------------------------------------------------
// Base Address
//-------------------------------------------------------------------------------------------------
/* Base address of R2 register */
#define MIU_AUDIO_BUFFER_BASE_ADDR              (AudioPCM_ADR)
#define MIU_AUDIO_BUFFER_END_ADDR               (AudioPCM_ADR + AudioPCM_LEN-1)
#define MIU_RANGE_BYTE_SIZE                     0x1000

#define MIU_AUDIO_DMA_WRITER_BUFFER_BASE_ADDR   MIU_AUDIO_BUFFER_BASE_ADDR
#define MIU_AUDIO_DMA_READER_BUFFER_BASE_ADDR   (MIU_AUDIO_DMA_WRITER_BUFFER_BASE_ADDR + DMA_WRITER_BUFFER_BYTE_SIZE + MIU_RANGE_BYTE_SIZE)

#define MIU_AUDIO_DMA_WRITER_BUFFER_END_ADDR    (MIU_AUDIO_DMA_WRITER_BUFFER_BASE_ADDR + DMA_WRITER_BUFFER_BYTE_SIZE - 1)
#define MIU_AUDIO_DMA_READER_BUFFER_END_ADDR    (MIU_AUDIO_DMA_READER_BUFFER_BASE_ADDR + DMA_READER_BUFFER_BYTE_SIZE+ DMA_WRITER_BUFFER_BYTE_SIZE - 1)

//-------------------------------------------------------------------------------------------------
/// Audio Register Define
//-------------------------------------------------------------------------------------------------
#define AUDIO_REG_BASE                          REG_140200    // R/W      0x2C00      // 0x2C00 - 0x2DFF

// MailBox Control Register
#define REG_AUDMBOX_SW_MUTE_140340              REG_140340    // REG_140340[0]  = 1: Software Mute
                                                            //              = 0: Software unMute

#define REG_AUDMBOX_SYS_UNLOCK_140340           REG_140340    // REG_140340[8]  = 1: System Port Unlock
                                                            //              = 0: System Port lock

#define REG_AUDMBOX_CH_TYPE_140342              REG_140342    // REG_140342[3:0]= 0: Ch_Stereo,
                                                            //                1: CH_Multi ,
                                                            //                2: CH_Downmix
                                                            //                4: Mute

#define REG_AUDMBOX_AUDIO_CP_START              REG_140344    // REG_140344[0]  = 1: Start
                                                            //                0: Stop

#define REG_AUDMBOX_AUDIO_CP_START              REG_140344    // REG_140344[15] = 1: Reset


#define REG_AUDMBOX_DELAY_VALUE_14034E          REG_14034E    // Delay vaule setting

#define REG_AUDMBOX_HK2CP_STATE_CTRL_140350     REG_140350    // REG_140350[7:0]= 0: R2CP_STOP,
                                                            //                1: R2CP_INIT ,
                                                            //                2: R2CP_START_DMA

#define REG_AUDMBOX_CP2HK_REPLY_STATE_140352    REG_140352    // REG_140352[7:0]= 0: R2CP_AUDIO_IS_STOP,
                                                            //                1: R2CP_AUDIO_IS_INIT ,
                                                            //                2: R2CP_AUDIO_INIT_FINISH
                                                            //                3: R2CP_AUDIO_START_BUSY

#define REG_AUDMBOX_CP2HK_FRMCNT_140354         REG_140354    // REG_140354 :     CP Audio Frame counter,


#define REG_AUDMBOX_EQ_Gain1_140350     	0x140350    	// 0x140350[12:0] Band 120 Hz
#define REG_AUDMBOX_EQ_Gain2_140352     	0x140352    	// 0x140352[12:0] Band 500 Hz
#define REG_AUDMBOX_EQ_Gain3_140354     	0x140354    	// 0x140354[12:0] Band 1.5K Hz
#define REG_AUDMBOX_EQ_Gain4_140356     	0x140356    	// 0x140356[12:0] Band 5 KHz
#define REG_AUDMBOX_EQ_Gain5_140358     	0x140358    	// 0x140358[12:0] Band 10 KHz
#define REG_AUDMBOX_BASS_Gain_14035A     	0x14035A    	// 0x14035A[6:0]
#define REG_AUDMBOX_TREBLE_Gain_14035C    	0x14035C    	// 0x14035C[6:0]

#define REG_AUDMBOX_AUDIO_VERSION_14035E        REG_14035E    // REG_14035E :     CP Audio version control,

//-------------------------------------------------------------------------------------------------
// Other System Register
//-------------------------------------------------------------------------------------------------
#define REG_AUDIO_AUPLL_CFG1                    REG_101ED4    // R/W
#define REG_AUDIO_AUPLL_CFG2                    REG_101ED6    // R/W
#define REG_AUDIO_AUPLL_CFG3                    REG_101ED8    // R/W

//#define REG_GPIO_PAD_AUD_CFG1                   REG_000406    // R/W
//#define REG_GPIO_PAD_AUD_I2S                    REG_000416    // R/W


//-------------------------------------------------------------------------------------------------
// AUDIO System Register
//-------------------------------------------------------------------------------------------------
#define REG_AUDIO_SW_RST                        REG_140200    // R/W          0x2C00

/* Audio Channel Control Register */
#define REG_AUDIO_CH1_CTRL                      REG_140202    // R/W          0x2C02
#define REG_AUDIO_CH2_CTRL                      REG_140204    // R/W          0x2C04
#define REG_AUDIO_CH3_CTRL                      REG_140206    // R/W          0x2C06
#define REG_AUDIO_CH4_CTRL                      REG_140208    // R/W          0x2C08

#define REG_AUDIO_ANA_CTRL0                     REG_14020A    // R/W,         0x2C0A
#define REG_AUDIO_ANA_CTRL1                     REG_14020C    // R/W,         0x2C0C
#define REG_AUDIO_ANA_CTRL2                     REG_14020E    // R/W,         0x2C0E
#define REG_AUDIO_ANA_CTRL3                     REG_140210    // R/W,         0x2C10
#define REG_AUDIO_ANA_CTRL4                     REG_140212    // R/W,         0x2C12
#define REG_AUDIO_ANA_CTRL5                     REG_140214    // R/W,         0x2C14
#define REG_AUDIO_ANA_CTRL6                     REG_140216    // R/W,         0x2C16
#define REG_AUDIO_ANA_CTRL7                     REG_140220    // R/W,         0x2C20
#define REG_AUDIO_ANA_CTRL8                     REG_140222    // R/W,         0x2C22

/* Audio HDMI Register */
#define REG_AUDIO_HDMI_RX_CTRL0                 REG_140218    // R/W          0x2C18
#define REG_AUDIO_HDMI_RX_CTRL1                 REG_14021A    // R/W          0x2C1A
#define REG_AUDIO_HDMI_RX_CTRL2                 REG_14021C    // R/W          0x2C1C
#define REG_AUDIO_HDMI_RX_CTRL3                 REG_14021E    // R/W          0x2C1E

/* Audio Clock Register */
#define REG_AUDIO_CLK_CTRL0                     REG_140240    // R/W          0x2C40
#define REG_AUDIO_CLK_CTRL1                     REG_140242    // R/W          0x2C42
#define REG_AUDIO_CLK_CTRL2                     REG_140244    // R/W          0x2C44
#define REG_AUDIO_CLK_CTRL3                     REG_140246    // R/W          0x2C46
#define REG_AUDIO_CLK_CTRL4                     REG_140248    // R/W          0x2C48
#define REG_AUDIO_CLK_CTRL5                     REG_14024A    // R/W          0x2C4A
#define REG_AUDIO_CLK_CTRL6                     REG_14024C    // R/W          0x2C4C
#define REG_AUDIO_CLK_CTRL7                     REG_14024E    // R/W          0x2C4E
#define REG_AUDIO_CLK_CTRL8                     REG_140250    // R/W          0x2C50
#define REG_AUDIO_CLK_CTRL9                     REG_140252    // R/W          0x2C52
#define REG_AUDIO_CLK_CTRL10                    REG_140254    // R/W          0x2C54
#define REG_AUDIO_CLK_CTRL11                    REG_140256    // R/W          0x2C56
#define REG_AUDIO_CLK_CTRL12                    REG_140258    // R/W          0x2C58
#define REG_AUDIO_CLK_CTRL13                    REG_14025A    // R/W          0x2C5A
#define REG_AUDIO_CLK_CTRL14                    REG_14025C    // R/W          0x2C5C
#define REG_AUDIO_CLK_CTRL15                    REG_14025E    // R/W          0x2C5E

/* Audio I2S Register */
#define REG_AUDIO_I2S_TX_CTRL0                  REG_14026C    // R/W          0x2C6C
#define REG_AUDIO_I2S_TX_CTRL1                  REG_14026E    // R/W          0x2C6E

#define REG_AUDIO_I2S_RX_CTRL0                  REG_140278    // R/W          0x2C78
#define REG_AUDIO_I2S_RX_CTRL1                  REG_14027A    // R/W          0x2C7A

/* Audio System Status Register */
#define REG_AUDIO_SYSTEM_STS_SEL                REG_1402F2    // R/W          0x2CF2
#define REG_AUDIO_SYSTEM_STS0                   REG_1402F4    // R/W          0x2CF4
#define REG_AUDIO_SYSTEM_STS1                   REG_1402F6    // R/W          0x2CF6

/* Audio Debug Control Register */
#define REG_AUDIO_DEBUG_CTRL0                   REG_1402F8    // R/W          0x2CF8
#define REG_AUDIO_DEBUG_CTRL1                   REG_1402FA    // R/W          0x2CFA

/* Audio Interrupt Register */
#define REG_AUDIO_IRQ_CTRL0                     REG_1402FC    // R/W          0x2CFC
#define REG_AUDIO_IRQ_STS0                      REG_1402FE    // R/W          0x2CFE

/* Audio SPDIF Register (Only for Test) */
#define REG_AUDIO_SPDIF_TX_CTRL0                REG_140260    // R/W          0x2C60
#define REG_AUDIO_SPDIF_TX_CTRL1                REG_140262    // R/W          0x2C62
#define REG_AUDIO_SPDIF_TX_CTRL2                REG_140264    // R/W          0x2C64
#define REG_AUDIO_SPDIF_TX_CTRL3                REG_140266    // R/W          0x2C66

#define REG_AUDIO_SPDIF_RX_CTRL0                REG_140270    // R/W          0x2C70
#define REG_AUDIO_SPDIF_RX_CTRL1                REG_140272    // R/W          0x2C72


/* Audio PAD Control Register*/
#define REG_AUDIO_PAD_CTRL0                     REG_14027C    // R/W          0x2C7C
#define REG_AUDIO_PAD_CTRL1                     REG_14027E    // R/W          0x2C7E

//-------------------------------------------------------------------------------------------------
// AUDIO DMA Register
//-------------------------------------------------------------------------------------------------
#define REG_AUDIO_DMA1_CTRL0                    REG_140280    // R/W          0x2C80
#define REG_AUDIO_DMA1_CTRL1                    REG_140282    // R/W          0x2C82
#define REG_AUDIO_DMA1_CTRL2                    REG_140284    // R/W          0x2C84
#define REG_AUDIO_DMA1_CTRL3                    REG_140285    // R/W          0x2C85
#define REG_AUDIO_DMA1_CTRL4                    REG_140286    // R/W          0x2C86
#define REG_AUDIO_DMA1_CTRL5                    REG_14028A    // R/W          0x2C8A
#define REG_AUDIO_DMA1_CTRL6                    REG_14028C    // R/W          0x2C8C
#define REG_AUDIO_DMA1_CTRL7                    REG_14028E    // Read Only    0x2C8E
#define REG_AUDIO_DMA1_CTRL8                    REG_140290    // Read Only    0x2C90
#define REG_AUDIO_DMA1_CTRL9                    REG_140292    // R/W          0x2C92
#define REG_AUDIO_DMA1_CTRL10                   REG_140294    // R/W          0x2C94
#define REG_AUDIO_DMA1_CTRL11                   REG_140295    // R/W          0x2C95
#define REG_AUDIO_DMA1_CTRL12                   REG_140296    // R/W          0x2C96
#define REG_AUDIO_DMA1_CTRL13                   REG_14029A    // R/W          0x2C9A
#define REG_AUDIO_DMA1_CTRL14                   REG_14029C    // R/W          0x2C9C
#define REG_AUDIO_DMA1_CTRL15                   REG_14029E    // Read Only    0x2C9E


#define REG_AUDIO_DMA2_CTRL0                    REG_1402A0    // R/W          0x2CA0
#define REG_AUDIO_DMA2_CTRL1                    REG_1402A2    // R/W          0x2CA2
#define REG_AUDIO_DMA2_CTRL2                    REG_1402A4    // R/W          0x2CA4
#define REG_AUDIO_DMA2_CTRL3                    REG_1402A5    // R/W          0x2CA5
#define REG_AUDIO_DMA2_CTRL4                    REG_1402A6    // R/W          0x2CA6
#define REG_AUDIO_DMA2_CTRL5                    REG_1402AA    // R/W          0x2CAA
#define REG_AUDIO_DMA2_CTRL6                    REG_1402AC    // R/W          0x2CAC
#define REG_AUDIO_DMA2_CTRL7                    REG_1402AE    // Read Only    0x2CAE
#define REG_AUDIO_DMA2_CTRL8                    REG_1402B0    // Read Only    0x2CB0
#define REG_AUDIO_DMA2_CTRL9                    REG_1402B2    // R/W          0x2CB2
#define REG_AUDIO_DMA2_CTRL10                   REG_1402B4    // R/W          0x2CB4
#define REG_AUDIO_DMA2_CTRL11                   REG_1402B5    // R/W          0x2CB5
#define REG_AUDIO_DMA2_CTRL12                   REG_1402B6    // R/W          0x2CB6
#define REG_AUDIO_DMA2_CTRL13                   REG_1402BA    // R/W          0x2CBA
#define REG_AUDIO_DMA2_CTRL14                   REG_1402BC    // R/W          0x2CBC
#define REG_AUDIO_DMA2_CTRL15                   REG_1402BE    // Read Only    0x2CBE

#define DMA2_CTRL_0_L                           REG_1402A0
#define DMA2_CTRL_0_H                           REG_1402A1
#define DMA2_CTRL_1_L                           REG_1402A2
#define DMA2_CTRL_1_H                           REG_1402A3
#define DMA2_CTRL_2_L                           REG_1402A4
#define DMA2_CTRL_2_H                           REG_1402A5
#define DMA2_CTRL_3_L                           REG_1402A6
#define DMA2_CTRL_3_H                           REG_1402A7
#define DMA2_CTRL_4_L                           REG_1402A8
#define DMA2_CTRL_4_H                           REG_1402A9
#define DMA2_CTRL_5_L                           REG_1402AA
#define DMA2_CTRL_5_H                           REG_1402AB
#define DMA2_CTRL_6_L                           REG_1402AC
#define DMA2_CTRL_6_H                           REG_1402AD
#define DMA2_CTRL_7_L                           REG_1402AE
#define DMA2_CTRL_7_H                           REG_1402AF
#define DMA2_CTRL_8_L                           REG_1402B0
#define DMA2_CTRL_8_H                           REG_1402B1
#define DMA2_CTRL_9_L                           REG_1402B2
#define DMA2_CTRL_9_H                           REG_1402B3
#define DMA2_CTRL_10_L                          REG_1402B4
#define DMA2_CTRL_10_H                          REG_1402B5
#define DMA2_CTRL_11_L                          REG_1402B6
#define DMA2_CTRL_11_H                          REG_1402B7
#define DMA2_CTRL_12_L                          REG_1402B8
#define DMA2_CTRL_12_H                          REG_1402B9
#define DMA2_CTRL_13_L                          REG_1402BA
#define DMA2_CTRL_13_H                          REG_1402BB
#define DMA2_CTRL_14_L                          REG_1402BC
#define DMA2_CTRL_14_H                          REG_1402BD
#define DMA2_CTRL_15_L                          REG_1402BE
#define DMA2_CTRL_15_H                          REG_1402BF

//-------------------------------------------------------------------------------------------------
// AUDIO ADC Register
//-------------------------------------------------------------------------------------------------
#define REG_AUDIO_SRC_ADC_CMPS0                 REG_1402D4    // R/W          0x2CD4
#define REG_AUDIO_SRC_ADC_CMPS1                 REG_1402D6    // R/W          0x2CD6
#define REG_AUDIO_SRC_ADC_CMPS2                 REG_1402D8    // R/W          0x2CD8

#define REG_AUDIO_SRC_ADC_GAIN                  REG_1402DA    // R/W          0x2CDA

//-------------------------------------------------------------------------------------------------
// AUDIO Synth Register
//-------------------------------------------------------------------------------------------------
#define REG_AUDIO_2nd_Order_Synth_0             REG_14022A    // R/W          0x2C2A
#define REG_AUDIO_2nd_Order_Synth_1             REG_14022C    // R/W          0x2C2C

#define REG_AUDIO_Synth0_H                      REG_1402C0    // R/W          0x2CC0
#define REG_AUDIO_Synth0_L                      REG_1402C2    // R/W          0x2CC2
#define REG_AUDIO_Synth1_H                      REG_1402C4    // R/W          0x2CC4
#define REG_AUDIO_Synth1_L                      REG_1402C6    // R/W          0x2CC6
#define REG_AUDIO_Synth2_H                      REG_1402C8    // R/W          0x2CC8
#define REG_AUDIO_Synth2_L                      REG_1402CA    // R/W          0x2CCA
#define REG_AUDIO_Synth3_H                      REG_1402CC    // R/W          0x2CCC
#define REG_AUDIO_Synth3_L                      REG_1402CE    // R/W          0x2CCE
#define REG_AUDIO_Synth4_H                      REG_1402D0    // R/W          0x2CD0
#define REG_AUDIO_Synth5_L                      REG_1402D2    // R/W          0x2CD2

#define REG_AUDIO_Synth_CTRL                    REG_1402DC    // R/W          0x2CDC
#define REG_AUDIO_Synth_TRIGER                  REG_1402DE    // R/W          0x2CDE

//-------------------------------------------------------------------------------------------------
// AUDIO SRC Register
//-------------------------------------------------------------------------------------------------
#define REG_AUDIO_SRC_CTRL0                     REG_1402E0    // R/W          0x2CE0
#define REG_AUDIO_SRC_CTRL1                     REG_1402E2    // R/W          0x2CE2
#define REG_AUDIO_SRC_CTRL2                     REG_1402E4    // R/W          0x2CE4
#define REG_AUDIO_SRC_CTRL3                     REG_1402E6    // R/W          0x2CE6
#define REG_AUDIO_SRC_CTRL4                     REG_1402E8    // R/W          0x2CE8
#define REG_AUDIO_SRC_CTRL5                     REG_1402EA    // R/W          0x2CEA
#define REG_AUDIO_SRC_CTRL6                     REG_1402EC    // R/W          0x2CEC
#define REG_AUDIO_SRC_CTRL7                     REG_1402EE    // Read Only    0x2CEE


//-------------------------------------------------------------------------------------------------
// AUDIO Bypass Register
//-------------------------------------------------------------------------------------------------
#define REG_AUDIO_BYPASS_CTRL1                  REG_140330    // R/W          0x2D30
#define REG_AUDIO_BYPASS_CTRL2                  REG_140332    // R/W          0x2D30

//-------------------------------------------------------------------------------------------------
// AUDIO DPGA Register
//-------------------------------------------------------------------------------------------------
#if CHIP_ID==MST9U4
#define REG_AUDIO_DPGA0_VOLUME_LEFT             REG_112D20    // R/W
#define REG_AUDIO_DPGA0_VOLUME_RIGHT            REG_112D22    // R/W
#define REG_AUDIO_DPGA1_VOLUME_LEFT             REG_112D24    // R/W
#define REG_AUDIO_DPGA1_VOLUME_RIGHT            REG_112D26    // R/W
#define REG_AUDIO_DPGA2_VOLUME_LEFT             REG_112D28    // R/W
#define REG_AUDIO_DPGA2_VOLUME_RIGHT            REG_112D2A    // R/W
#define REG_AUDIO_DPGA3_VOLUME_LEFT             REG_112D2C    // R/W
#define REG_AUDIO_DPGA3_VOLUME_RIGHT            REG_112D2E    // R/W

#define REG_AUDIO_DPGA0_VOLUME_CTRL             REG_112D20    // R/W
#define REG_AUDIO_DPGA1_VOLUME_CTRL             REG_112D24    // R/W
#define REG_AUDIO_DPGA2_VOLUME_CTRL             REG_112D28    // R/W
#define REG_AUDIO_DPGA3_VOLUME_CTRL             REG_112D2C    // R/W


#else
#define REG_AUDIO_DPGA0_VOLUME_LEFT             REG_140302    // R/W
#define REG_AUDIO_DPGA0_VOLUME_RIGHT            REG_140304    // R/W
#define REG_AUDIO_DPGA1_VOLUME_LEFT             REG_14030A    // R/W
#define REG_AUDIO_DPGA1_VOLUME_RIGHT            REG_14030C    // R/W
#define REG_AUDIO_DPGA2_VOLUME_LEFT             REG_140312    // R/W
#define REG_AUDIO_DPGA2_VOLUME_RIGHT            REG_140314    // R/W
#define REG_AUDIO_DPGA3_VOLUME_LEFT             REG_14031A    // R/W
#define REG_AUDIO_DPGA3_VOLUME_RIGHT            REG_14031C    // R/W

#define REG_AUDIO_DPGA0_VOLUME_CTRL             REG_140300    // R/W
#define REG_AUDIO_DPGA1_VOLUME_CTRL             REG_140308    // R/W
#define REG_AUDIO_DPGA2_VOLUME_CTRL             REG_140310    // R/W
#define REG_AUDIO_DPGA3_VOLUME_CTRL             REG_140318    // R/W


#endif


#endif // _REG_AUDIO_H_

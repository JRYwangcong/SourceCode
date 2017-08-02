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

#ifndef __INTPUTPORT_H__
#define __INTPUTPORT_H__

typedef enum // Inputport mux
{
    MUX_ADC = 0,
    MUX_COMBO0,
    MUX_COMBO1,
    MUX_COMBO2,
    MUX_COMBO3,
    MUX_COMBO4,
    MUX_COMBO5,
    MUX_VD,
    MUX_RESERVED
}EN_INPUTPORT_MUX;

/// Define input status
typedef enum
{
    IP_STATUS_IDLE = 0,         // idle

    IP_STATUS_WAITSYNC,         // mStar_MonitorInputTiming(), wait input stable sync
    IP_STATUS_NOSYNC,            // mStar_MonitorInputTiming(),  input stable no sync
    IP_STATUS_FINDMODE,         // mStar_MonitorInputTiming(), input stable sync, start find mode
    IP_STATUS_FINDMODEOK,       // mStar_MonitorInputTiming(), input stable sync, find mode ok
}EN_INPUT_STATUS;


typedef enum // Inputport type
{
    TYPE_ANALOG      = BIT0,
    TYPE_DVI_SINGLE  = BIT1,
    TYPE_DVI_DUAL    = BIT2,
    TYPE_HDMI        = BIT3,
    TYPE_MHL         = BIT4,
    TYPE_DP          = BIT5,
    TYPE_DPTX        = BIT6,
    TYPE_VD          = BIT7,
    TYPE_HBR2_0      = BIT8,
    TYPE_HBR2_1      = BIT9,
    TYPE_HDMI_2_0    = BIT10,
    TYPE_FAKE_DUAL   = BIT11,
    TYPE_FAKE_DUAL_S = BIT12,
    TYPE_DAISY_CHAIN = BIT13,
    TYPE_RESERVED    = BIT14
}EN_INPUTPORT_TYPE;


typedef enum
{
    INPUTCOLOR_RGB,
    INPUTCOLOR_YUV,
    //INPUTCOLOR_YUV_422,
    //INPUTCOLOR_YUV_444,
    INPUTCOLOR_MAX
}EN_InputColorFormatType;

enum
{
  #if (INPUT_TYPE&INPUT_1A)
    Input_Analog1,
  #endif
  #if (INPUT_TYPE >= INPUT_1C)
    Input_Digital1,
  #endif
  #if (INPUT_TYPE >= INPUT_2C)
    Input_Digital2,
  #endif
  #if (INPUT_TYPE >= INPUT_3C)
    Input_Digital3,
  #endif
  #if (INPUT_TYPE >= INPUT_4C)
    Input_Digital4,
  #endif
  #if (INPUT_TYPE >= INPUT_5C)
    Input_Digital5,
  #endif
  #if (INPUT_TYPE >= INPUT_6C)
    Input_Digital6,
  #endif
  #if (INPUT_TYPE >= INPUT_7C)
    Input_Digital7,
  #endif
    Input_Nums,
///////////////////////////////
///////////////////////////////
  #if ENABLE_VGA_INPUT
    INPUT_VGA           = Input_Analog1,
  #endif
  #if ENABLE_DVI_INPUT
    INPUT_DVI           = DVI_PORT_PRIORITY,
   #if ENABLE_DVI2ND_INPUT
    INPUT_DVI2ND        = DVI2ND_PORT_PRIORITY,
    #if ENABLE_DVI3ND_INPUT
    INPUT_DVI3ND        = DVI3ND_PORT_PRIORITY,
    #endif
   #endif
  #endif
  #if ENABLE_HDMI_INPUT
    INPUT_HDMI          = HDMI_PORT_PRIORITY,
   #if ENABLE_HDMI2ND_INPUT
    INPUT_HDMI2ND       = HDMI2ND_PORT_PRIORITY,
    #if ENABLE_HDMI3ND_INPUT
    INPUT_HDMI3ND       = HDMI3ND_PORT_PRIORITY,
    #endif
   #endif
  #endif
  #if ENABLE_DP_INPUT
    INPUT_DISPLAYPORT   = DP_PORT_PRIORITY,
   #if ENABLE_DP2ND_INPUT
    INPUT_DISPLAYPORT2ND= DP2ND_PORT_PRIORITY,
    #if ENABLE_DP3ND_INPUT
    INPUT_DISPLAYPORT3ND= DP3ND_PORT_PRIORITY,
    #endif
   #endif
  #endif
};

enum
{
    SCL0_SUB    =0,                 // L0 - f1 sub det
    SCL0_MAIN   =1,                 // L0 - f2 main det
    SCR0_SUB    =2,                 // R0 - f1 sub det
    SCR0_MAIN   =3,                 // R0 - f2 main det
    SCL1_SUB    =4,                 // L1 - f1 sub det
    SCL1_MAIN   =5,                 // L1 - f2 main det
    SCR1_SUB    =6,                 // R1 - f1 sub det
    SCR1_MAIN   =7,                 // R1 - f2 main det
    SCL0_OFFLINE=8,                 // L0 - f0 offline det
    SC_MAX_NUMS =9,                 // The max sc nums: non scaler
};

enum // IP_SysFlags
{
    bInputTimingChangeBit   = BIT0,
    bInputTimingStableBit   = BIT1,
    bDoModeSettingBit       = BIT2,
    bReadyForChangeBit      = BIT3,
    bCableNotConnectedBit   = BIT4,
};

typedef struct
{
    BYTE fFlags;
    WORD HPeiod;
    WORD VTotal;
    BYTE ModeIndex;
    WORD AutoHstart; // for digital port valid timing detect
    WORD AutoVstart; // for digital port valid timing detect
} InputTimingType;

typedef struct
{
    WORD DispWidth;
    WORD DispHeight;
} InputResolutionType;

typedef struct
{
    WORD    u16HPDCnt;      //1:HPD duration counter
    WORD    u16HPDClkLow;   //2:CLK r-term low thd
    WORD    u16HPDClkHigh;  //3:CLK r-term high thd
} ST_HPD_PERIOD_CNT;

typedef struct
{
    EN_INPUTPORT_MUX            eIPMux;
    EN_INPUTPORT_TYPE           eIPType;

    EN_INPUT_STATUS             eIPStatus;

    BYTE                        u8IPScIndex;            // scaler index for detecting sync
    WORD                        u16IPScMask;           // scaler index - bit based
//    ST_WINDOW_INFO                    sIPCaptureWin;                    // input capture window

    BYTE                        u8IPModeDetectCounter;
    WORD                        u16IPInputTimingStableCounter;
    WORD                        u16IPInputSyncValidThd;
    WORD                        u16IPInputSyncLossThd;
    BYTE                        u8IPSysFlags;
    InputTimingType             sIPInputTiming;

    EN_InputColorFormatType     eIPInputColorFormat;    // indicate input color space is RGB or YUV
    BOOL                        bIPInputHBR2;           // 1: input HBR2 detected

    ST_HPD_PERIOD_CNT           sIPHPDCntr;           // down counter for HPD duration
}ST_INPUTPORT_PARAM; //for PM mode.



#define INPUT_IS_VGA(u8InputPort)         (g_InputPort[u8InputPort].eIPType & TYPE_ANALOG)
#define INPUT_IS_DVI_S(u8InputPort)       (g_InputPort[u8InputPort].eIPType & TYPE_DVI_SINGLE)
#define INPUT_IS_DVI_D(u8InputPort)       (g_InputPort[u8InputPort].eIPType & TYPE_DVI_DUAL)
#define INPUT_IS_DVI(u8InputPort)         (INPUT_IS_DVI_S(u8InputPort) | INPUT_IS_DVI_D(u8InputPort))
#define INPUT_IS_HDMI(u8InputPort)        (g_InputPort[u8InputPort].eIPType & TYPE_HDMI)
#define INPUT_IS_MHL(u8InputPort)        (g_InputPort[u8InputPort].eIPType & TYPE_MHL)
#define INPUT_IS_DISPLAYPORT(u8InputPort) (g_InputPort[u8InputPort].eIPType & TYPE_DP)
#define INPUT_IS_TMDS(u8InputPort)        (INPUT_IS_DVI(u8InputPort) | INPUT_IS_HDMI(u8InputPort))
#define INPUT_IS_VD(u8InputPort)          (g_InputPort[u8InputPort].eIPType & TYPE_VD)
#define INPUT_IS_HBR2(u8InputPort)        (g_InputPort[u8InputPort].eIPType & (TYPE_HBR2_0|TYPE_HBR2_1))
#define INPUT_IS_HBR2_0(u8InputPort)      (g_InputPort[u8InputPort].eIPType & TYPE_HBR2_0)
#define INPUT_IS_HBR2_1(u8InputPort)      (g_InputPort[u8InputPort].eIPType & TYPE_HBR2_1)
#if (CHIP_ID == MST9U3)
#define INPUT_IS_DC_RX(u8InputPort)       (INPUT_IS_MUX_C4(u8InputPort) && ENABLE_DAISY_CHAIN)
#else
#define INPUT_IS_DC_RX(u8InputPort)       (INPUT_IS_MUX_C1(u8InputPort) && ENABLE_DAISY_CHAIN)
#endif

#define INPUT_IS_MUX_ADC(u8InputPort)     (g_InputPort[u8InputPort].eIPMux == MUX_ADC)
#define INPUT_IS_MUX_C0(u8InputPort)      (g_InputPort[u8InputPort].eIPMux == MUX_COMBO0)
#define INPUT_IS_MUX_C1(u8InputPort)      (g_InputPort[u8InputPort].eIPMux == MUX_COMBO1)
#define INPUT_IS_MUX_C2(u8InputPort)      (g_InputPort[u8InputPort].eIPMux == MUX_COMBO2)
#define INPUT_IS_MUX_C3(u8InputPort)      (g_InputPort[u8InputPort].eIPMux == MUX_COMBO3)
#define INPUT_IS_MUX_C4(u8InputPort)      (g_InputPort[u8InputPort].eIPMux == MUX_COMBO4)
#define INPUT_IS_MUX_C5(u8InputPort)      (g_InputPort[u8InputPort].eIPMux == MUX_COMBO5)
#define INPUT_IS_MUX_VD(u8InputPort)      (g_InputPort[u8InputPort].eIPMux == MUX_VD)


#define SyncLoss                       (BIT2|BIT3)
#endif /* __INTPUTPORT_H__ */

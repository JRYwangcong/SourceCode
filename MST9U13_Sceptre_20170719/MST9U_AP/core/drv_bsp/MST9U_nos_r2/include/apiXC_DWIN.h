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
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////
//==============================================================================
// [apiXC_DWIN.h]
// Date: 20120221
// Descriptions: structure pool
//==============================================================================
#ifndef APIXC_DWIN_H
#define APIXC_DWIN_H

#include "MsCommon.h"
//#include "mvideo_context.h"
//==============================================================================
//==============================================================================
#ifdef APIXC_DWIN_C//MVIDEO_CONTEXT_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

/// Define window type for DIP
typedef enum
{
    DIP_WINDOW = 0,         ///< DIP window
    MAX_DIP_WINDOW          /// The max support window
}SCALER_DIP_WIN;

/// Define source type for DIP
typedef enum
{
    SCALER_DIP_SOURCE_TYPE_SUB2 = 0,     ///< DIP from SUB2 
    SCALER_DIP_SOURCE_TYPE_MAIN = 1,     ///< DIP from MAIN
    SCALER_DIP_SOURCE_TYPE_SUB  = 2,     ///< DIP from SUB
    MAX_SCALER_DIP_SOURCE_NUM          /// The max support window
}SCALER_DIP_SOURCE_TYPE;

/// DWIN source data format
typedef enum
{
    /// CrYCb domain.
    DIP_DATA_FMT_YUV422 = 0,
    /// CrYCb domain.
    DIP_DATA_FMT_RGB565,
    /// RGB domain (CSC bypass).
    DIP_DATA_FMT_ARGB8888,
    /// YUV420
    DIP_DATA_FMT_YUV420,
    DIP_DATA_FMT_MAX
} EN_XC_DWIN_DATA_FMT;

/// DWIN scan mode
typedef enum
{
    /// progessive.
    DIP_SCAN_MODE_PROGRESSIVE,
    /// interlace.
    DIP_SCAN_MODE_extern,
} EN_XC_DWIN_SCAN_MODE;

E_APIXC_ReturnValue MApi_XC_DIP_Init(void);
E_APIXC_ReturnValue MApi_XC_DIP_SetFrameBufferAddress(MS_PHYADDR u32FBAddress, MS_PHYADDR u32FBSize, SCALER_DIP_WIN eWindow);
E_APIXC_ReturnValue MApi_XC_DIP_SetInputSource(INPUT_SOURCE_TYPE_t enInputSourceType, SCALER_DIP_WIN eWindow);
E_APIXC_ReturnValue MApi_XC_DIP_GetInputSource(INPUT_SOURCE_TYPE_t *enInputSourceType, SCALER_DIP_WIN eWindow);
E_APIXC_ReturnValue MApi_XC_DIP_DisableInputSource(MS_BOOL bDisable, SCALER_DIP_WIN eWindow);
E_APIXC_ReturnValue MApi_XC_DIP_SetWindow(XC_SETWIN_INFO *pstXC_SetWin_Info, MS_U32 u32InitDataLen, SCALER_DIP_WIN eWindow);
E_APIXC_ReturnValue MApi_XC_DIP_GetDEWindow(MS_WINDOW_TYPE *psWin, SCALER_DIP_WIN eWindow);
E_APIXC_ReturnValue MApi_XC_DIP_SetCaptureWindowVstart(MS_U16 u16Vstart, SCALER_DIP_WIN eWindow);
E_APIXC_ReturnValue MApi_XC_DIP_SetCaptureWindowHstart(MS_U16 u16Hstart, SCALER_DIP_WIN eWindow);
E_APIXC_ReturnValue MApi_XC_DIP_SetCaptureWindowVsize(MS_U16 u16Vsize, SCALER_DIP_WIN eWindow);
E_APIXC_ReturnValue MApi_XC_DIP_SetCaptureWindowHsize(MS_U16 u16Hsize, SCALER_DIP_WIN eWindow);
E_APIXC_ReturnValue MApi_XC_DIP_GetCaptureWindow(MS_WINDOW_TYPE* capture_win, SCALER_DIP_WIN eWindow);
E_APIXC_ReturnValue MApi_XC_DIP_EnableR2YCSC(MS_BOOL benable, SCALER_DIP_WIN eWindow);

//==============Set===================
E_APIXC_ReturnValue MApi_XC_DIP_CaptureOneFrame(void);
E_APIXC_ReturnValue MApi_XC_DIP_CaptureOneFrame2(void);
E_APIXC_ReturnValue MApi_XC_DIP_ClearIntr(MS_U16 u8mask);
E_APIXC_ReturnValue MApi_XC_DIP_Enable(MS_BOOL bEnable);
E_APIXC_ReturnValue MApi_XC_DIP_EnableIntr(MS_U16 u8mask, MS_BOOL bEnable);
E_APIXC_ReturnValue MApi_XC_DIP_SelectSourceScanType(EN_XC_DWIN_SCAN_MODE enScanMode);
E_APIXC_ReturnValue MApi_XC_DIP_DeInterlaceWrite(MS_BOOL bEnable);
E_APIXC_ReturnValue MApi_XC_DIP_SetDataFmt(EN_XC_DWIN_DATA_FMT fmt);
E_APIXC_ReturnValue MApi_XC_DIP_SetWinProperty(MS_U8 u8BufCnt, MS_U16 u16Width, MS_U16 u16Height, MS_U32 u32BufStart, MS_U32 u32BufEnd, SCALER_DIP_SOURCE_TYPE eSource);
E_APIXC_ReturnValue MApi_XC_DIP_SetY2R(MS_BOOL bEnable);
E_APIXC_ReturnValue MApi_XC_DIP_SetAlphaValue(MS_U8 u8AlphaVal);
E_APIXC_ReturnValue MApi_XC_DIP_SetUVSwap(MS_BOOL bEnable);
E_APIXC_ReturnValue MApi_XC_DIP_SetYCSwap(MS_BOOL bEnable);
E_APIXC_ReturnValue MApi_XC_DIP_SetRGBSwap(MS_BOOL bEnable);
//==============Get===================
MS_U16 MApi_XC_DIP_GetIntrStatus(void);



#endif /* APIXC_DWIN_H */


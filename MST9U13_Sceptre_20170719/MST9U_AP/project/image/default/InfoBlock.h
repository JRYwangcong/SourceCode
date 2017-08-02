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
// (��MStar Confidential Information��) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////
#include "BinInfo.h"
#include "datatype.h"
#include "sysinfo.h"

#ifndef _INFOBLOCK_H_
#define _INFOBLOCK_H_
////////////////////////////////////////////////////////////////////////////////
///@file InfoBlock.h
///@version rev.00 00000
///@author MStarSemi Inc.
///@brief System Information Block Data
///
///@endcode
////////////////////////////////////////////////////////////////////////////////
#ifdef _INFOBLOCK_C_
#define INTERFACE
#else
#define INTERFACE extern
#endif

// Utility define
#define PlaceU32(x) x&0xFF, (x>>8)&0xFF, (x>>16)&0xFF, (x>>24)&0xFF
#define PlaceU16(y) y&0xFF, (y>>8)&0xFF
#define PlaceU32_(x) ((x)>>24)&0xFF, ((x)>>16)&0xFF, ((x)>>8)&0xFF, (x)&0xFF
#define PlaceU16_(y) ((y)>>8)&0xFF, (y)&0xFF
#define PlaceU8_(z) z

#if ENABLE_USB_HOST_DUAL_IMAGE
#define DUAL_IMAGE_OFFSET_THRESHOLD     0x40000 // sboot size < 64K*4
#endif

//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------
INTERFACE BOOLEAN Get_BinInfo(BININFO *pBinInfo);
INTERFACE void Get_BinaryInfoBlock(U32 *u32Addr, U32 *u32Size);
INTERFACE void Get_BinInfoBlock(BININFO *Fmt, U8 *u8Addr);

#if (UI_SKIN_SEL==UI_SKIN_BM_800X600X1555)
    #define BIN_ID_OSDCP_BMP            BIN_ID_OSDCP_BMP_21
#elif (UI_SKIN_SEL==UI_SKIN_NON_BM_800X600X1555)
    #define BIN_ID_OSDCP_BMP            BIN_ID_OSDCP_BMP_22
#else
    #Bitmap bin file not assigned.
#endif

#if (UI_SKIN_SEL==UI_SKIN_BM_800X600X1555)
    #define BIN_ID_OSDCP_TEXT           BIN_ID_OSDCP_TEXT_21
#elif (UI_SKIN_SEL==UI_SKIN_NON_BM_800X600X1555)
    #define BIN_ID_OSDCP_TEXT           BIN_ID_OSDCP_TEXT_22
#else
    #String bin file not assigned.
#endif

#if (ENABLE_JPEGPNG_OSD)
 #define BIN_ID_OSDCP_JPEG           BIN_ID_OSDCP_JPEG_21
#endif

  #if defined(MIPS_CHAKRA)
    #define BIN_ID_CODE_MHEG5           BIN_ID_CODE_MHEG5_A
  #else
    #define BIN_ID_CODE_MHEG5           BIN_ID_CODE_MHEG5_B
  #endif

#if defined(MIPS_CHAKRA)
    #define BIN_ID_CODE_VDPLAYER        BIN_ID_CODE_AEON_VDPLAYER_HK_MIPS_NOS
#elif defined (MSOS_TYPE_LINUX)
    #define BIN_ID_CODE_VDPLAYER        BIN_ID_CODE_AEON_VDPLAYER_HK_MIPS_LINUX
#else
  #if (MEMORY_MAP <= MMAP_32MB)
    #define BIN_ID_CODE_VDPLAYER        BIN_ID_CODE_MIPS_VDPLAYER_HK_AEON_NOS_32M
  #else
    #define BIN_ID_CODE_VDPLAYER        BIN_ID_CODE_MIPS_VDPLAYER_HK_AEON_NOS
#endif
#endif


#ifdef BIN_ID_CODE_AEON_BENCHMARK_HK_MIPS_NOS
    #define BIN_ID_CODE_AEON_BENCHMARK  BIN_ID_CODE_AEON_BENCHMARK_HK_MIPS_NOS
#endif

    #define FONT_ASIA_I2                BIN_ID_FONT_ASIA_ALL
    #define FONT_BIG5_I2                BIN_ID_FONT_BIG5_CH
#if FONT_SUPPORT_VECTOR_AND_BITMAP
    #define FONT_CHINESE_I2             BIN_ID_FONT_SIMP_CH
    #define FONT_MSTAR_UNICODE_MVF      BIN_ID_FONT_UI_MVF
#else
    #define FONT_MSTAR_UNICODE_MVF      BIN_ID_FONT_MSTAR_MVF_0
#endif

    #define BIN_ID_JPEG_BOOT_LOGO       BIN_ID_JPEG_BOOT_LOGO_DVB
    #define BIN_ID_MELODY_ON            BIN_ID_MELODY_ON_DVB
#if(ENABLE_POWERON_VIDEO)
    #define BIN_ID_VIDEO_ON             BIN_ID_VIDEO_ON_DVB
#endif

/*************************************************************************************************************/
#undef INTERFACE
#endif // _INFOBLOCK_H_


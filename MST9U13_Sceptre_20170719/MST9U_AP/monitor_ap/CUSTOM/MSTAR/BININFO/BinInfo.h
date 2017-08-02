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
// (MStar Confidential Information) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef _BIN_INFO_H_
#define _BIN_INFO_H_
////////////////////////////////////////////////////////////////////////////////
///@file BinInfo.h
///@version rev.00 00000
///@author MStarSemi Inc.
///@brief Binary files information, Unique ID for each binary used to identify
///
/// Binary file information description include
///     - Binary ID Name, ID code
///     - Binary File Reserved Space, Prefix Word "//~"
///     - Binary File Location, Prefix Word "//@"
/// Example:
///#define BIN_ID_CODE_AEON_MVF 0x0051 //~40KB+512 //@..\LIB\S4\aeon_mvf.bin
///     - Reserved Space 40.5KB
///#define BIN_ID_CODE_AEON_NOSHK 0x0052 //~128KB //@ ..\LIB\S4\HKAEON.bin
///     - Reserved Space 128KB
///
/// PS. 0xC000~0xCFFF Reserved for Customer, Please avoid using these IDs
///
///@endcode
////////////////////////////////////////////////////////////////////////////////

// Code: CP-R2
#define BIN_ID_CODE_CP_R2           0x0101  //~64KB     //@..\..\..\core\bin\cp_r2\MST9U_CP.bin
// Code: SECU-R2
#define BIN_ID_CODE_SECU_R2         0x0107  //~64KB     //@..\..\..\core\bin\cp_r2\MST9U_SECU.bin

#define BIN_ID_DAISY_CHAIN          0x0102  //~64KB     //@..\..\..\core\bin\daisy_chain\daisy_chain.bin
#define BIN_ID_VBY1_TO_EDP          0x0108  //~32KB     //@..\..\..\core\bin\vby1_to_edp\vby1_to_edp.bin

// PQ BIN
#define BIN_ID_PQ_MAIN              0x0103   //~400KB   //@..\..\..\core\driver\pq\hal\m12\include\Macaw12_Main.bin
#define BIN_ID_PQ_MAIN_TEXT         0x0104   //~22KB    //@..\..\..\core\driver\pq\hal\m12\include\Macaw12_Main_Text.bin
#define BIN_ID_PQ_MAIN_EX           0x0105   //~20KB    //@..\..\..\core\driver\pq\hal\m12\include\Macaw12_Main_Ex.bin
#define BIN_ID_PQ_MAIN_EX_TEXT      0x0106   //~4KB     //@..\..\..\core\driver\pq\hal\m12\include\Macaw12_Main_Ex_Text.bin

#define BIN_ID_FONT_BMPFONT_0_0     0x0110  //~0KB    //@..\..\..\monitor_ap\CUSTOM\MSTAR\ZUI\font\monitor_0_35.bin -g
#define BIN_ID_FONT_BMPFONT_0_1     0x0111  //~0KB    //@..\..\..\monitor_ap\CUSTOM\MSTAR\ZUI\font\monitor_0_28.bin -g
#define BIN_ID_FONT_BMPFONT_0_2     0x0112  //~0KB    //@..\..\..\monitor_ap\CUSTOM\MSTAR\ZUI\font\monitor_0_18.bin -g
#define BIN_ID_FONT_BMPFONT_1_0     0x0120  //~0KB    //@..\..\..\monitor_ap\CUSTOM\MSTAR\ZUI\font\monitor_1_35.bin -g
#define BIN_ID_FONT_BMPFONT_1_1     0x0121  //~0KB    //@..\..\..\monitor_ap\CUSTOM\MSTAR\ZUI\font\monitor_1_28.bin -g
#define BIN_ID_FONT_BMPFONT_1_2     0x0122  //~0KB    //@..\..\..\monitor_ap\CUSTOM\MSTAR\ZUI\font\monitor_1_18.bin -g
#define BIN_ID_FONT_MSTAR_MVF_0     0x01A0  //~0KB    //@..\..\..\monitor_ap\CUSTOM\MSTAR\ZUI\font\MStar_Unicode5.mvf -g
#define BIN_ID_FONT_MSTAR_MVF_1     0x01A1  //~0KB    //@..\..\..\monitor_ap\CUSTOM\MSTAR\ZUI\font\MStar_Unicode6.mvf -g

//for ZUI_BM
#define BIN_ID_OSDCP_BMP_21         0x0816  //~0KB    //@..\..\..\monitor_ap\CUSTOM\MSTAR\ZUI\res800x600x1555_BM\Osdcomposer\Osdbin\ZUI_bitmap.bin -g
#define BIN_ID_OSDCP_TEXT_21        0x0826  //~0KB    //@..\..\..\monitor_ap\CUSTOM\MSTAR\ZUI\res800x600x1555_BM\Osdcomposer\Osdbin\ZUI_strings.bin -g
// for Non-BM
#define BIN_ID_OSDCP_BMP_22         0x0816  //~0KB    //@..\..\..\monitor_ap\CUSTOM\MSTAR\ZUI\res800x600x1555\Osdcomposer\Osdbin\ZUI_bitmap.bin -g
#define BIN_ID_OSDCP_TEXT_22        0x0826  //~0KB    //@..\..\..\monitor_ap\CUSTOM\MSTAR\ZUI\res800x600x1555\Osdcomposer\Osdbin\ZUI_strings.bin -g

#if ENABLE_FB_BMP_DRAW
#define BIN_ID_BMP_TEST_FILE        0xC000  //~900KB    //@..\..\..\project\image\TestBmp.bmp
#endif

#if ENABLE_FB_JPG_DRAW
#define BIN_ID_JPG_TEST_FILE        0xC001  //~1500KB	//@..\..\..\project\image\TestJpg.jpg
#endif

#define BIN_ID_MERGE_END            0xD000  //~1KB  //@..\..\..\project\image\MagicNum55AAABCD.bin

// ISP update flag
#define BIN_ID_ISP_UPDATE_FLAG      0xE000

// Magic number
#define BIN_ID_MAGIC_55AAABCD       0xF000  //~1KB  //@..\..\..\project\image\MagicNum55AAABCD.bin

#define BIN_PACKAGE_HEADER_ID0  0x54454C09
#define BIN_PACKAGE_HEADER_ID1  0x58336900
#define BIN_DISPLAY_HEADER_ID0  0x54454C09
#define BIN_DISPLAY_HEADER_ID1  0x31207341
#define BIN_DRAMMAP_HEADER_ID0  0x4D5354FA
#define BIN_DRAMMAP_HEADER_ID1  0x50417461
#define BIN_IRMAP_HEADER_ID     0x77554321
#define BIN_KEYPAD_HEADER_ID    0x4368656E

#define BIN_PACKAGE_HEADER_SIZE 0x0C
#define BIN_PACKAGE_NUMBER_SIZE 0x02
#define BIN_PACKAGE_BINFO_SIZE  0x0B

#define BIN_PACKAGE_HEADER_ADDR 0x00
#define BIN_PACKAGE_NUMBER_ADDR (BIN_PACKAGE_HEADER_ADDR+BIN_PACKAGE_HEADER_SIZE)
#define BIN_PACKAGE_BINFO_ADDR  (BIN_PACKAGE_NUMBER_ADDR+BIN_PACKAGE_NUMBER_SIZE)


//--------------------------------------------
// Reserved for Customer: Range 0xC000-0xCFFF
//--------------------------------------------
//#define BIN_ID_CUSTOMER_RESERVED_1    0xC000
//...
//#define BIN_ID_CUSTOMER_RESERVED_X    0xCFFF
typedef struct _BIN_INFO_
{
    U16 B_ID;                 //!< Unique ID
    U32 B_FAddr;              //!< Start address
    U32 B_Len;                //!< Length in bytes
    U8  B_IsComp;             //!< Is bin compressed
} BININFO;

#endif  //_BIN_INFO_H_


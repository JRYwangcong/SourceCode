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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   apiXC_SC_MenuLoad.h
/// @brief  XC MenuLoad Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef _API_XC_MENULOAD_H_
#define _API_XC_MENULOAD_H_

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum
{
    E_MLOAD_UNSUPPORTED = 0,
    E_MLOAD_DISABLED    = 1,
    E_MLOAD_ENABLED     = 2,
}MLOAD_TYPE;

typedef enum
{
    E_MLG_UNSUPPORTED = 0,
    E_MLG_DISABLED    = 1,
    E_MLG_ENABLED     = 2,
}MLG_TYPE;
//------------------------------
// MLoad Function, xc command burst
//------------------------------
//-------------------------------------------------------------------------------------------------
/// Initialize the Menuload
/// @param  PhyAddr                 \b IN: the physical address for the menuload
/// @param  u32BufByteLen           \b IN: the buffer length of the menu load commands
//-------------------------------------------------------------------------------------------------
void    MApi_XC_MLoad_Init(MS_PHYADDR PhyAddr, MS_U32 u32BufByteLen);

//-------------------------------------------------------------------------------------------------
/// Enable/Disable the MLoad
/// @return  void
//-------------------------------------------------------------------------------------------------
void    MApi_XC_MLoad_Enable(MS_BOOL bEnable);

//-------------------------------------------------------------------------------------------------
/// Get the status of MLoad
//-------------------------------------------------------------------------------------------------
MLOAD_TYPE MApi_XC_MLoad_GetStatus(void);
//-------------------------------------------------------------------------------------------------
/// MLOAD ENTRY
//-------------------------------------------------------------------------------------------------
void  _MLOAD_ENTRY(void) ;
//-------------------------------------------------------------------------------------------------
/// EXIT THE MENULOAD
//-------------------------------------------------------------------------------------------------
void  _MLOAD_RETURN(void) ;
//-------------------------------------------------------------------------------------------------
/// Enable the menuload trigger for data update
/// @param  bImmeidate              \b IN: true , after enable the trigger , it will check the data done or not done;if done , it will return true, or it will return false; 
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_MLoad_Fire(MS_BOOL bImmeidate);
//-------------------------------------------------------------------------------------------------
/// MApi_XC_MLoad_WriteCmd
/// @param  u32Addr              \b IN: register
/// @param  u16Data              \b IN: value
/// @param  u16Mask              \b IN: mask
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_MLoad_WriteCmd(MS_U32 u32Addr, MS_U16 u16Data, MS_U16 u16Mask);


/// MApi_XC_MLoad_WriteComCmd
/// @param  u32Addr              \b IN: register
/// @param  u16Data              \b IN: value
/// @param  u16Mask              \b IN: mask
/// @param  u16Mask              \b IN: mask
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_MLoad_WriteComCmd(MS_U32 u32Addr, MS_U16 u16Data, MS_U16 u16Mask, MS_MLCOMWRITEMODE ML_CWM);

//-------------------------------------------------------------------------------------------------
/// write signal command to Mload and fire
/// @param  u32Addr              \b IN: register
/// @param  u16Data              \b IN: value
/// @param  u16Mask              \b IN: mask
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_MLoad_WriteCmd_And_Fire(MS_U32 u32Addr, MS_U16 u16Data, MS_U16 u16Mask);

//-------------------------------------------------------------------------------------------------
/// write multi commands to Mload and fire
/// @param  pu32Addr              \b IN: buffer of register
/// @param  pu16Data              \b IN: buffer of value
/// @param  pu16Mask              \b IN: buffer of mask
/// @param  u16CmdCnt             \b IN: counter of command
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_MLoad_WriteCmds_And_Fire(MS_U32 *pu32Addr, MS_U16 *pu16Data, MS_U16 *pu16Mask, MS_U16 u16CmdCnt);

//-------------------------------------------------------------------------------------------------
/// Enable the BitMask menuload trigger for data update
/// @param  bImmeidate              \b IN: true , after enable the trigger , it will check the data done or not done;if done , it will return true, or it will return false; 
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_MLoad_BitMask_Fire(MS_BOOL bImmeidate);

//-------------------------------------------------------------------------------------------------
/// MApi_XC_MLoad_BitMask_WriteCmd
/// @param  u32Addr              \b IN: register
/// @param  u16Data              \b IN: value
/// @param  u16Mask              \b IN: mask
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_MLoad_BitMask_WriteCmd(MS_U32 u32Addr, MS_U16 u16Data, MS_U16 u16Mask);

//-------------------------------------------------------------------------------------------------
/// Write command to the Menuload buffer
/// Note: [47] = 1 (spread mode)
/// @param  u32Addr                 \b IN: the address (bank >> 1 + 16-bit address)
/// @param  u16Data                 \b IN: the data
/// @param  u16Mask                 \b IN: the mask
/// @return  TRUE if succeed, FALSE if failed
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_MLoad_BitMask_WriteSpreadCmd(MS_U32 u32Addr, MS_U16 u16Data, MS_U16 u16Mask);

#ifdef __cplusplus
}
#endif

#endif /* _API_XC_MENULOAD_H_ */

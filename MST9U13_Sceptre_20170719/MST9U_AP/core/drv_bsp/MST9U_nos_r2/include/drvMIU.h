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
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
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

//////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   drvMIU.h
/// @brief  MIU Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_MIU_H_
#define _DRV_MIU_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"

//-------------------------------------------------------------------------------------------------
// Defines
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------
#define MIU_DRV_VERSION                 /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    {'M','I','U','_'},                  /* IP__                                             */  \
    {'0','1'},                          /* 0.0 ~ Z.Z                                        */  \
    {'0','2'},                          /* 00 ~ 99                                          */  \
    {'0','0','2','6','4','8','8','5'},  /* CL#                                              */  \
    MSIF_OS

#define MIU_CLIENT_MAX  0x30


#define bit_check(x,bit_pos)  ((x & bit_pos) != 0) ? TRUE : FALSE


#define set_req_mask_info(reg_addr,base_addr,bEnable,id_pos)  \
    MST_MACRO_START                                                     \
    reg_addr = base_addr;                                               \
    reg_addr = reg_addr + (MS_U16)((id_pos >> 4) * 0x20);                  \
    if(reg_addr == 0x1286)                                              \
        reg_addr = 0x06F6;                                              \
    else if(reg_addr == 0x0686)                                         \
        reg_addr = 0x0628;                                              \
    reg_addr = reg_addr + (MS_U16)(bit_check(id_pos,BIT3));                \
    HAL_MIU_WriteRegBit(reg_addr, bEnable , (1 << (id_pos & (BIT2|BIT1|BIT0)))); \
    MST_MACRO_END

#define ALIGN(_val_,_shift_) (((_val_) >> _shift_) << _shift_)


//-------------------------------------------------------------------------------------------------
// Type and Structure Declaration
//-------------------------------------------------------------------------------------------------
typedef struct
{
    MS_U8 miu;
    MS_U8 protect_id;
} stMIUClientID_t;

typedef enum
{
    MIU_CLIENT_NONE,    //none can access
    MIU_CLIENT_DUMMY,
    MIU_CLIENT_HAYDEN_RW,
    MIU_CLIENT_HK_R2_RW,
    MIU_CLIENT_PQ_R2_RW,
    MIU_CLIENT_USB_RW,
    MIU_CLIENT_PM51_RW,
    MIU_CLIENT_GE_RW,
    MIU_CLIENT_T3D_SC0_RW,
    MIU_CLIENT_T3D_SC1_RW,
    MIU_CLIENT_T3D_SC2_RW,
    MIU_CLIENT_T3D_SC3_RW,
    MIU_CLIENT_AUTODOWNLOAD_R,
    MIU_CLIENT_BDMA_RW,
    MIU_CLIENT_ADCDVIPLL_W,
    MIU_CLIENT_SC0_IPMAIN_RW,
    MIU_CLIENT_SC0_IPSUB_RW,
    MIU_CLIENT_SC0_OPMAIN_RW,
    MIU_CLIENT_SC1_IPMAIN_RW,
    MIU_CLIENT_SC1_IPSUB_RW,
    MIU_CLIENT_SC1_OPMAIN_RW,
    MIU_CLIENT_OD_L_RW,
    MIU_CLIENT_OD_R_RW,
    MIU_CLIENT_GOP0_R,
    MIU_CLIENT_GOP1_R,
    MIU_CLIENT_GOPD_W,
    MIU_CLIENT_SC0_IPW_W,
    MIU_CLIENT_SC1_IPW_W,
    MIU_CLIENT_SC2_IPMAIN_RW,
    MIU_CLIENT_SC2_IPSUB_RW,
    MIU_CLIENT_SC2_OPMAIN_RW,
    MIU_CLIENT_SC3_IPMAIN_RW,
    MIU_CLIENT_SC3_IPSUB_RW,
    MIU_CLIENT_SC3_OPMAIN_RW,
    MIU_CLIENT_OSDC_L_W,
    MIU_CLIENT_OSDC_R_W,
    MIU_CLIENT_OSDD_L_R,
    MIU_CLIENT_OSDD_R_R,
    MIU_CLIENT_GOP2_R,
    MIU_CLIENT_GOP3_R,
    MIU_CLIENT_SC2_IPW_W,
    MIU_CLIENT_SC3_IPW_W,
}eMIUClientID;

typedef enum
{
    //IP Access MIU0 only
    MIU_SELTYPE_MIU0 = 0,
    //IP Access MIU1 only
    MIU_SELTYPE_MIU1,
    //IP can access MIU0 & MIU1
    MIU_SELTYPE_MIU_ALL
}eMIU_SelType;

typedef struct
{
    MS_U8 u8Gp:4;
    MS_U8 u8BitPos:4;
}MIU_ClientInfo;

typedef struct
{
    MS_BOOL bHit;
    MS_U8   u8Group;
    MS_U8   u8ClientID;
    MS_U8   u8Block;
}MIU_PortectInfo;

typedef struct
{
    MS_U8   u81st;
    MS_U8   u82nd;
    MS_U8   u83rd;
    MS_U8   u84th;
}MIU_GroupPriority;

typedef enum
{
  E_MIU_0 = 0,
  E_MIU_1,
} MIU_ID;

typedef enum
{
  E_MIU_DDR_32MB = 0,
  E_MIU_DDR_64MB,
  E_MIU_DDR_128MB,
  E_MIU_DDR_256MB,
  E_MIU_DDR_512MB,
  E_MIU_DDR_1024MB,
  E_MIU_DDR_2048MB,
}MIU_DDR_SIZE;


#define MST_MACRO_START     do {
#define MST_MACRO_END       } while (0)


#define set_client_info(client_data,miu_pos,id_code)         \
        MST_MACRO_START                                      \
        client_data->miu = miu_pos;                          \
        client_data->protect_id = id_code;                   \
        MST_MACRO_END


//-------------------------------------------------------------------------------------------------
// Extern Global Variabls
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Extern Functions
//-------------------------------------------------------------------------------------------------
void    MDrv_MIU_Mask_Req_OPM_R(MS_U8 u8Mask, MS_U8 u8Miu);
void    MDrv_MIU_Mask_Req_DNRB_R(MS_U8 u8Mask, MS_U8 u8Miu);
void    MDrv_MIU_Mask_Req_DNRB_W(MS_U8 u8Mask, MS_U8 u8Miu);
void    MDrv_MIU_Mask_Req_DNRB_RW(MS_U8 u8Mask, MS_U8 u8Miu);
void    MDrv_MIU_Mask_Req_SC_RW(MS_U8 u8Mask, MS_U8 u8Miu);
void    MDrv_MIU_Mask_Req_MVOP_R(MS_U8 u8Mask, MS_U8 u8Miu);
void    MDrv_MIU_Mask_Req_MVD_R(MS_U8 u8Mask, MS_U8 u8Miu);
void    MDrv_MIU_Mask_Req_MVD_W(MS_U8 u8Mask, MS_U8 u8Miu);
void    MDrv_MIU_Mask_Req_MVD_RW(MS_U8 u8Mask, MS_U8 u8Miu);
void    MDrv_MIU_Mask_Req_AUDIO_RW(MS_U8 u8Mask, MS_U8 u8Miu);
void    MDrv_MIU_MaskReq(MS_U8 u8Miu, eMIUClientID eClientID);
void    MDrv_MIU_UnMaskReq(MS_U8 u8Miu, eMIUClientID eClientID);
void    MDrv_MIU_InitCounter(void);
void    MDrv_MIU_VOP_SwitchMIU(MS_U8 u8MiuID);
MS_U8*  MDrv_MIU_GetDefaultClientID_KernelProtect(void);
MS_U16  MDrv_MIU_GetBusWidth(void);
MS_BOOL MDrv_MIU_SetSsc(MS_U16 u16Fmodulation, MS_U16 u16FDeviation,MS_BOOL bEnable);
MS_BOOL MDrv_MIU_SetSscValue(MS_U8 u8MiuDev, MS_U16 u16Fmodulation, MS_U16 u16FDeviation, MS_BOOL bEnable);
MS_BOOL MDrv_MIU_SetIOMapBase(void);
MS_BOOL MDrv_MIU_ProtectAlign(MS_U32 *u32PageShift);
MS_BOOL MDrv_MIU_Dram_Size(MS_U8 MiuID, MS_U8 DramSize);
MS_U8 MDrv_MIU_ClinetNumber(MS_U8 u8Blockx);
MS_BOOL MDrv_MIU_Protect(MS_U8 u8MiuDev, MS_U8 u8Blockx, MS_U8 *pu8ProtectId, MS_U32 u32Start, MS_U32 u32End, MS_BOOL  bSetInvert, MS_BOOL bSetFlag );
MS_BOOL MDrv_MIU_SelMIU(eMIUClientID eClientID, eMIU_SelType eType);
MS_BOOL MDrv_MIU_GetClientInfo(MS_U8 u8MiuDev, eMIUClientID eClientID, MIU_ClientInfo *pInfo);
MS_BOOL MDrv_MIU_GetProtectInfo(MS_U8 u8MiuDev, MIU_PortectInfo *pInfo);
MS_BOOL MDrv_MIU_SetGroupPriority(MS_U8 u8MiuDev, MIU_GroupPriority sPriority);
MS_BOOL MDrv_MIU_SetHPriorityMask(MS_U8 u8MiuDev, eMIUClientID eClientID, MS_BOOL bMask);
MS_BOOL MDrv_MIU_GetLibVer(const MSIF_Version **ppVersion);
MS_BOOL MDrv_MIU_EnableScramble(MS_BOOL bEnable);
MS_BOOL MDrv_MIU_IsScrambleEnabled(void);
#ifdef __cplusplus
}
#endif

#endif // _DRV_MIU_H_


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

// utopia header FIXME
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif
#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"
#include "drvPQ.h"

#ifndef _DRVPQ_TEMPLATE_H_
#define _DRVPQ_TEMPLATE_H_

#define PQ_INSTALL_FUNCTION
#define SC_MAX_NUM  4

#ifndef UNUSED //to avoid compile warnings...
#define UNUSED(var) (void)((var) = (var))
#endif

#if(PQ_ONLY_SUPPORT_BIN == 0)
static PQTABLE_INFO _PQTableInfo;

#if (PQ_ENABLE_PIP == 1)
#if (PQ_IP_NUM_Main > PQ_IP_NUM_Sub)
static MS_U8 _u8PQTabIdx[PQ_IP_NUM_Main];    // store all TabIdx of all IPs
#else
static MS_U8 _u8PQTabIdx[PQ_IP_NUM_Sub];    // store all TabIdx of all IPs
#endif
#else
static MS_U8 _u8PQTabIdx[PQ_IP_NUM_Main];    // store all TabIdx of all IPs
#endif

static MS_BOOL _bMLoadEn = FALSE;

#define MLOAD_MAX_CMD   192
static MS_U32 _u32MLoadCmd[MLOAD_MAX_CMD];
static MS_U16 _u16MLoadMsk[MLOAD_MAX_CMD];
static MS_U16 _u16MLoadVal[MLOAD_MAX_CMD];
static MS_U16 _u16MLoadCmdCnt = 0;
#if(ENABLE_PQ_LOAD_TABLE_INFO)
static MS_PQ_LOAD_TABLE_INFO g_PQLoadTableInfo;
#endif
#endif

#if(PQ_ONLY_SUPPORT_BIN == 0)

static MS_U8 _u8SRAM1Table[SC_MAX_NUM];
static MS_U8 _u8SRAM2Table[SC_MAX_NUM];
static MS_U8 _u8SRAM3Table[SC_MAX_NUM];
static MS_U8 _u8SRAM4Table[SC_MAX_NUM];
static MS_U8 _u8CSRAM1Table[SC_MAX_NUM];
static MS_U8 _u8CSRAM2Table[SC_MAX_NUM];
static MS_U8 _u8CSRAM3Table[SC_MAX_NUM];
static MS_U8 _u8CSRAM4Table[SC_MAX_NUM];
static MS_U8 _u8ICC_CRD_Table[SC_MAX_NUM];
static MS_U8 _u8IHC_CRD_Table[SC_MAX_NUM];
static void _MDrv_PQ_Set_MLoadEn(MS_BOOL bEn)
{
    MS_U16 i = 0, j = 0;

    if(bEn == DISABLE)
    {
        //if spread reg, no need to use mutex, but it's ok to use mutex
        //   (because it's not MApi_XC_W2BYTE(), which has mutex already)
        //if not spread reg, must use mutex to protect MLoad_trigger func.
        SC_BK_STORE_MUTEX;

        if(_u16MLoadCmdCnt)
        {
           //Note: If there some same reg in the Mload cmd queue,
           //  MLoad will use the one at the last's value to fire. others same reg's setting will be ignore.
           // So we handle this situation here, to combine every same reg's setting value to the same.
           for(i=1; i<_u16MLoadCmdCnt; i++)
            {
                for(j=i+1; j<_u16MLoadCmdCnt; j++)
                {
                    if(_u32MLoadCmd[i-1] == _u32MLoadCmd[j-1])
                    {
                        _u16MLoadMsk[i-1] |= _u16MLoadMsk[j-1];
                        _u16MLoadVal[i-1] |= _u16MLoadVal[j-1];
                        _u16MLoadMsk[j-1] = _u16MLoadMsk[i-1];
                        _u16MLoadVal[j-1] = _u16MLoadVal[i-1];
                    }
                }
            }
    #if(ENABLE_PQ_MLOAD)

            MApi_XC_MLoad_WriteCmds_And_Fire(
                &_u32MLoadCmd[0], &_u16MLoadVal[0], &_u16MLoadMsk[0], _u16MLoadCmdCnt);
    #endif
            _u16MLoadCmdCnt = 0;
        }

        SC_BK_RESTORE_MUTEX;
    }
    else
    {
        //_u16MLoadCmdCnt = 0;
    }

    _bMLoadEn = bEn;
}

static void _MDrv_PQ_PreInitLoadTableInfo(void)
{
#if(ENABLE_PQ_LOAD_TABLE_INFO)

    MS_U16 u16IPIdx;
    g_PQLoadTableInfo._u16CurInputSrcType = -1;
#if (PQ_ENABLE_PIP == 1)
#if (PQ_IP_NUM_Main > PQ_IP_NUM_Sub)
    for( u16IPIdx = 0; u16IPIdx < PQ_IP_NUM_Main; ++ u16IPIdx )
#else
    for( u16IPIdx = 0; u16IPIdx < PQ_IP_NUM_Sub; ++ u16IPIdx )
#endif
#else
    for( u16IPIdx = 0; u16IPIdx < PQ_IP_NUM_Main; ++ u16IPIdx )
#endif
    {
        g_PQLoadTableInfo._au8IPGroupIdx[u16IPIdx] = 0xFF;
    }
#endif

}

static void _MDrv_PQ_Set_LoadTableInfo_IP_Tab(MS_U16 u16IPIdx, MS_U8 u8TabIdx)
{
#if(ENABLE_PQ_LOAD_TABLE_INFO)
    g_PQLoadTableInfo._au8IPGroupIdx[u16IPIdx] = u8TabIdx;
#else
    UNUSED(u16IPIdx);
    UNUSED(u8TabIdx);
#endif

}

static MS_U8 _MDrv_PQ_Get_LoadTableInfo_IP_Tab(MS_U16 u16IPIdx)
{
#if(ENABLE_PQ_LOAD_TABLE_INFO)
    return g_PQLoadTableInfo._au8IPGroupIdx[u16IPIdx];
#else
    UNUSED(u16IPIdx);
    return 0xFF;
#endif
}

static void _MDrv_PQ_Set_LoadTableInfo_SrcType(MS_U16 u16SrcType)
{
#if(ENABLE_PQ_LOAD_TABLE_INFO)
   g_PQLoadTableInfo._u16CurInputSrcType = u16SrcType;
#else
   UNUSED(u16SrcType);
#endif
}

static MS_U16 _MDrv_PQ_Get_LoadTableInfo_SrcType(void)
{
#if(ENABLE_PQ_LOAD_TABLE_INFO)
   return g_PQLoadTableInfo._u16CurInputSrcType;
#else
   return 0xFFFF;
#endif
}

static void _MDrv_PQ_DumpGeneralRegTable(EN_IP_Info* pIP_Info)
{
    MS_U32 u32Addr;
    MS_U8 u8Mask;
    MS_U8 u8Value;

    PQ_DUMP_DBG(printf("tab: general\n"));
    if (pIP_Info->u8TabIdx >= pIP_Info->u8TabNums){
        PQ_DUMP_DBG(printf("[PQ]IP_Info error: General Reg Table\n"));
        return;
    }

    while (1)
    {
         u32Addr = (pIP_Info->pIPTable[0]<<8) + pIP_Info->pIPTable[1];
         u8Mask  = pIP_Info->pIPTable[2];
         u8Value = pIP_Info->pIPTable[REG_ADDR_SIZE+REG_MASK_SIZE+pIP_Info->u8TabIdx];

         if (u32Addr == _END_OF_TBL_) // check end of table
             break;

         u32Addr |= NON_PM_BASE;
         PQ_DUMP_DBG(printf("[addr=%04lx, msk=%02x, val=%02x]\n", u32Addr, u8Mask, u8Value));
         PQ_REG_FUNC(u32Addr, u8Value, u8Mask);

         pIP_Info->pIPTable+=(REG_ADDR_SIZE+REG_MASK_SIZE+pIP_Info->u8TabNums); // next
    }
}

static void _MDrv_PQ_DumpCombRegTable(EN_IP_Info* pIP_Info)
{
    MS_U32 u32Addr;
    MS_U8 u8Mask;
    MS_U8 u8Value;
    MS_U8 u8CurBank = 0xff;
    COMB_BK_STORE;

    PQ_DUMP_DBG(printf("tab: comb\n"));
    if (pIP_Info->u8TabIdx >= pIP_Info->u8TabNums){
        PQ_DUMP_DBG(printf("[PQ]IP_Info error: Comb Reg Table\n"));
        return;
    }

    while (1)
    {
        u32Addr = (pIP_Info->pIPTable[0]<<8) + pIP_Info->pIPTable[1];
        u8Mask  = pIP_Info->pIPTable[2];
        u8Value = pIP_Info->pIPTable[REG_ADDR_SIZE+REG_MASK_SIZE+pIP_Info->u8TabIdx];

        if (u32Addr == _END_OF_TBL_) // check end of table
            break;

        u8CurBank = u32Addr >> 8;
        if (u8CurBank != COMB_BK_CURRENT)
        {
            PQ_DUMP_DBG(printf("<<bankswitch=%02x>>\n", u8CurBank));
            COMB_BK_SWITCH(u8CurBank);
        }
		//$$$
        //u32Addr = (COMB_REG_BASE | (u32Addr & 0x00FF)) + (0x80000000 + (MS_U16)(u8CurBank) * 0x1000000) ;
        u32Addr = COMB_REG_BASE | (u32Addr & 0x00FF);

        PQ_DUMP_DBG(printf("[addr=%04x, msk=%02x, val=%02x]\n", u32Addr, u8Mask, u8Value));

/*
//Only works for T4, obsolete in main trunk.
    #if (CHIP_FAMILY_TYPE == CHIP_FAMILY_S7LD)
        // patch Comb register H part can not be written by 16 bits riu
        if((u32Addr == (COMB_REG_BASE + 0x47)) ||
           (u32Addr == (COMB_REG_BASE + 0x5F)) ||
           (u32Addr == (COMB_REG_BASE + 0x8F)) ||
           (u32Addr == (COMB_REG_BASE + 0xD7)) ||
           (u32Addr == (COMB_REG_BASE + 0xDF)))
        {
            MS_U8 u8low, u8High;
            MS_U16 u16value;

            u8low = MApi_XC_ReadByte(u32Addr-1);
            u8High = MApi_XC_ReadByte(u32Addr);
            u16value = (u8High & ~u8Mask) | (u8Value & u8Mask);
            u16value = (u16value<<8) | u16value;

            MApi_XC_Write2ByteMask(u32Addr-1, u16value, 0xFFFF);
            PQ_REG_FUNC(u32Addr-1, u8low, 0xFF);
        }
        else
    #endif
*/
        {
            PQ_REG_FUNC(u32Addr, u8Value, u8Mask);
        }

        pIP_Info->pIPTable+=(REG_ADDR_SIZE+REG_MASK_SIZE+pIP_Info->u8TabNums); // next
    }

    COMB_BK_RESTORE;
}

static void _MDrv_PQ_DumpScalerRegTable(EN_IP_Info* pIP_Info)
{
    MS_U32 u32Addr;
    MS_U8 u8Mask;
    MS_U8 u8Value;
    MS_U8 u8CurBank = 0xff;

#ifdef MSOS_TYPE_LINUX
#if(ENABLE_PQ_MLOAD)
    pthread_mutex_lock(&_PQ_MLoad_Mutex);
#endif
#endif

    SC_BK_STORE_MUTEX;
    PQ_DUMP_DBG(printf("tab: sc\n"));
    if (pIP_Info->u8TabIdx >= pIP_Info->u8TabNums)
    {
        PQ_DUMP_DBG(printf("[PQ]IP_Info error: Scaler Reg Table\n"));
        if(_bMLoadEn == TRUE)
        {
            _bMLoadEn = FALSE;
        }

        SC_BK_RESTORE_MUTEX;
#ifdef MSOS_TYPE_LINUX
#if(ENABLE_PQ_MLOAD)
        pthread_mutex_unlock(&_PQ_MLoad_Mutex);
#endif
#endif

        return;
    }

    while (1)
    {
        u32Addr = (pIP_Info->pIPTable[0]<<8) + pIP_Info->pIPTable[1];
        u8Mask  = pIP_Info->pIPTable[2];
        u8Value = pIP_Info->pIPTable[REG_ADDR_SIZE+REG_MASK_SIZE+pIP_Info->u8TabIdx];

        if (u32Addr == _END_OF_TBL_) // check end of table
            break;

        u8CurBank = u32Addr >> 8;
        PQ_DUMP_DBG(printf("XC bk =%x, addr=%x, msk=%x, value=%x\n", (MS_U16)((u32Addr&0xFF00)>>8), (MS_U16)(u32Addr&0xFF), (MS_U16)u8Mask, (MS_U16)u8Value));
        if(_bMLoadEn)
        {
            //printf("MLad: %lx, %x, %x\n", u32Addr, u8Value, u8Mask);
            PQ_REG_MLOAD_WRITE_CMD(u32Addr, u8Value, u8Mask);
            //PQ_REG_MLOAD_FUNC(u32Addr, u8Value, u8Mask);
        }
        else
        {
#if (SCALER_REGISTER_SPREAD)
            u32Addr = BK_SCALER_BASE | u32Addr;
#else
            if (u8CurBank != SC_BK_CURRENT)
            {
                PQ_DUMP_DBG(printf("<<bankswitch=%02x>>\n", u8CurBank));
                SC_BK_SWITCH(u8CurBank);
            }
			//$$$ Need to check below modification
            //u32Addr = BK_SCALER_BASE | (u32Addr & 0x00FF);
            //(0x80000000 + (MS_U16)(bank) * 0x1000000)
            //u32Addr = (u32ScBaseAddr | (u32Addr & 0x00FF)) + (0x80000000 + (MS_U16)(u8CurBank) * 0x1000000) ;
            u32Addr = u32ScBaseAddr | (u32Addr & 0x00FF);
#endif
			PQ_DUMP_DBG(printf("[PQ_REG_FUNC]: addr = %02x, value = %x, mask = %x\n", u32Addr, u8Value, u8Mask));

            PQ_REG_FUNC(u32Addr, u8Value, u8Mask);
        }
        pIP_Info->pIPTable+=(REG_ADDR_SIZE+REG_MASK_SIZE+pIP_Info->u8TabNums); // next
    }

    if(_bMLoadEn == TRUE)
    {
        _bMLoadEn = FALSE;
    }

    SC_BK_RESTORE_MUTEX;
#ifdef MSOS_TYPE_LINUX
#if(ENABLE_PQ_MLOAD)
    pthread_mutex_unlock(&_PQ_MLoad_Mutex);
#endif
#endif
}

static void _MDrv_PQ_DumpFilterTable(EN_IP_Info* pIP_Info)
{

    PQ_DUMP_FILTER_DBG(printf("tab: sram\n"));
    if (pIP_Info->u8TabIdx >= pIP_Info->u8TabNums){
        PQ_DUMP_DBG(printf("[PQ]IP_Info error: SRAM Table\n"));
        return;
    }
    PQ_DUMP_FILTER_DBG(printf("pIP_Info->u8TabType: %u\n",pIP_Info->u8TabType));
    switch(pIP_Info->u8TabType)
    {
    case PQ_TABTYPE_SRAM1:
        Hal_PQ_set_y_sram_table(SC_FILTER_Y_SRAM1, (void *)((MS_U32)&pIP_Info->pIPTable[pIP_Info->u8TabIdx * Hal_PQ_get_sram_size(SC_FILTER_Y_SRAM1)]));
        break;
    case PQ_TABTYPE_SRAM2:
        Hal_PQ_set_y_sram_table(SC_FILTER_Y_SRAM2, (void *)((MS_U32)&pIP_Info->pIPTable[pIP_Info->u8TabIdx * Hal_PQ_get_sram_size(SC_FILTER_Y_SRAM2)]));
        break;

    case PQ_TABTYPE_SRAM3:
        Hal_PQ_set_y_sram_table(SC_FILTER_Y_SRAM3, (void *)((MS_U32)&pIP_Info->pIPTable[pIP_Info->u8TabIdx * Hal_PQ_get_sram_size(SC_FILTER_Y_SRAM3)]));
        break;
    case PQ_TABTYPE_SRAM4:
        Hal_PQ_set_y_sram_table(SC_FILTER_Y_SRAM4, (void *)((MS_U32)&pIP_Info->pIPTable[pIP_Info->u8TabIdx * Hal_PQ_get_sram_size(SC_FILTER_Y_SRAM4)]));
        break;

    case PQ_TABTYPE_C_SRAM1:
        Hal_PQ_set_c_sram_table(SC_FILTER_C_SRAM1, (void *)((MS_U32)&pIP_Info->pIPTable[pIP_Info->u8TabIdx * Hal_PQ_get_sram_size(SC_FILTER_C_SRAM1)]));
        break;

    case PQ_TABTYPE_C_SRAM2:
        Hal_PQ_set_c_sram_table(SC_FILTER_C_SRAM2, (void *)((MS_U32)&pIP_Info->pIPTable[pIP_Info->u8TabIdx * Hal_PQ_get_sram_size(SC_FILTER_C_SRAM2)]));
        break;

    case PQ_TABTYPE_C_SRAM3:
        Hal_PQ_set_c_sram_table(SC_FILTER_C_SRAM3, (void *)((MS_U32)&pIP_Info->pIPTable[pIP_Info->u8TabIdx * Hal_PQ_get_sram_size(SC_FILTER_C_SRAM3)]));
        break;

    case PQ_TABTYPE_C_SRAM4:
        Hal_PQ_set_c_sram_table(SC_FILTER_C_SRAM4, (void *)((MS_U32)&pIP_Info->pIPTable[pIP_Info->u8TabIdx * Hal_PQ_get_sram_size(SC_FILTER_C_SRAM4)]));
        break;

    case PQ_TABTYPE_SRAM_COLOR_INDEX:
        Hal_PQ_set_sram_color_index_table(SC_FILTER_SRAM_COLOR_INDEX, (void *)((MS_U32)&pIP_Info->pIPTable[pIP_Info->u8TabIdx * Hal_PQ_get_sram_size(SC_FILTER_SRAM_COLOR_INDEX)]));
        break;

    case PQ_TABTYPE_SRAM_COLOR_GAIN_SNR:
        Hal_PQ_set_sram_color_gain_snr_table(SC_FILTER_SRAM_COLOR_GAIN_SNR, (void *)((MS_U32)&pIP_Info->pIPTable[pIP_Info->u8TabIdx * Hal_PQ_get_sram_size(SC_FILTER_SRAM_COLOR_GAIN_SNR)]));
        break;

    case PQ_TABTYPE_SRAM_COLOR_GAIN_DNR:
        Hal_PQ_set_sram_color_gain_dnr_table(SC_FILTER_SRAM_COLOR_GAIN_DNR, (void *)((MS_U32)&pIP_Info->pIPTable[pIP_Info->u8TabIdx * Hal_PQ_get_sram_size(SC_FILTER_SRAM_COLOR_GAIN_DNR)]));
        break;


    case PQ_TABTYPE_VIP_ICC_CRD_SRAM:
        Hal_PQ_set_sram_icc_crd_table(SC_FILTER_SRAM_ICC_CRD, (void *)((MS_U32)&pIP_Info->pIPTable[pIP_Info->u8TabIdx * Hal_PQ_get_sram_size(SC_FILTER_SRAM_ICC_CRD)]));
        break;

    case PQ_TABTYPE_VIP_IHC_CRD_SRAM:
        Hal_PQ_set_sram_ihc_crd_table(SC_FILTER_SRAM_IHC_CRD, (void *)((MS_U32)&pIP_Info->pIPTable[pIP_Info->u8TabIdx * Hal_PQ_get_sram_size(SC_FILTER_SRAM_IHC_CRD)]));
        break;
    /*
	case PQ_TABTYPE_XVYCC_DE_GAMMA_SRAM:
		Hal_PQ_set_sram_xvycc_gamma_table(SC_FILTER_SRAM_XVYCC_DE_GAMMA, (void*)((MS_U32)&pIP_Info->pIPTable[pIP_Info->u8TabIdx * Hal_PQ_get_sram_size(SC_FILTER_SRAM_XVYCC_DE_GAMMA)]));
		break;
	*/
    default:
        printf("[PQ]Unknown sram type %u\n", pIP_Info->u8TabType);
        break;
    }
    PQ_DUMP_FILTER_DBG(printf("tab load finish\n"));
}

static void _MDrv_PQ_DumpTable(EN_IP_Info* pIP_Info)
{
    // to save loading SRAM table time, SRAM are only downloaded
    // when current SRAM table is different to previous SRAM table

    /*DWORD	_u32ScBaseAddr = 0xFF;

    static MS_U8 _u8SRAM1Table=0xFF;
    static MS_U8 _u8SRAM2Table=0xFF;
    static MS_U8 _u8SRAM3Table=0xFF;
    static MS_U8 _u8SRAM4Table=0xFF;
    static MS_U8 _u8CSRAM1Table=0xFF;
    static MS_U8 _u8CSRAM2Table=0xFF;
    static MS_U8 _u8CSRAM3Table=0xFF;
    static MS_U8 _u8CSRAM4Table=0xFF;
    static MS_U8 _u8ICC_CRD_Table = 0xFF;
    static MS_U8 _u8IHC_CRD_Table = 0xFF;
	static MS_U8 _u8XYVCC_DEGAMMA_SRAM_Table = 0xFF;*/

    //WORD    _curScalerMask = 0x00;
    BYTE    _scIdx = 0;
    BYTE    _isNewSetting  = false;



    //SC_BK_SWITCH(0x00);
    //_curScalerMask = MApi_XC_ReadByte(u32ScBaseAddr | 0x84);
    if (u32ScBaseAddr == 0x102D00)
        _scIdx = 0;
    else if (u32ScBaseAddr == 0x102B00)
        _scIdx = 1;
    else if (u32ScBaseAddr == 0x103100)
        _scIdx = 2;
    else
        _scIdx = 3;

    if (pIP_Info->pIPTable == NULL)
    {
        PQ_DUMP_DBG(printf("NULL Table\n"););
        return;
    }
    PQ_DUMP_DBG(printf("Table Type =%x, Index =%x\n", (MS_U16)pIP_Info->u8TabType, (MS_U16)pIP_Info->u8TabIdx));
    switch(pIP_Info->u8TabType )
    {
    /*
    case PQ_TABTYPE_XVYCC_DE_GAMMA_SRAM:
		if ( _u8XYVCC_DEGAMMA_SRAM_Table != pIP_Info->u8TabIdx)
		{
			PQ_DUMP_FILTER_DBG(printf("old XVYCC DE GAMMA SRAM: %u, new XVYCC DE GAMMA SRAM %u\n",
				(MS_U16)_u8XYVCC_DEGAMMA_SRAM_Table, (MS_U16)pIP_Info->u8TabIdx));
			_u8XYVCC_DEGAMMA_SRAM_Table = pIP_Info->u8TabIdx;
			_MDrv_PQ_DumpFilterTable(pIP_Info);
		}
		break;*/
    case PQ_TABTYPE_SCALER:
        _MDrv_PQ_DumpScalerRegTable(pIP_Info);
        break;
    case PQ_TABTYPE_COMB:
        _MDrv_PQ_DumpCombRegTable(pIP_Info);
        break;
    case PQ_TABTYPE_GENERAL:
        _MDrv_PQ_DumpGeneralRegTable(pIP_Info);
        break;

    case PQ_TABTYPE_SRAM1:
		//_MDrv_PQ_DumpFilterTable(pIP_Info);

        if (_u8SRAM1Table[_scIdx] != pIP_Info->u8TabIdx)
        {
            _isNewSetting = true;
        }
        _u8SRAM1Table[_scIdx] = pIP_Info->u8TabIdx;

		if (_isNewSetting)
		{
            _MDrv_PQ_DumpFilterTable(pIP_Info);
		}
		/*
        if (_u8SRAM1Table != pIP_Info->u8TabIdx)
        {
            PQ_DUMP_FILTER_DBG(printf("old sram1: %u, new sram1: %u\n",
                (MS_U16)_u8SRAM1Table, (MS_U16)pIP_Info->u8TabIdx));
            _u8SRAM1Table = pIP_Info->u8TabIdx;
            _MDrv_PQ_DumpFilterTable(pIP_Info);
        }
        else
        {
            PQ_DUMP_FILTER_DBG(printf("use the same sram1: %u\n", (MS_U16)pIP_Info->u8TabIdx));
        }*/
        break;
    case PQ_TABTYPE_SRAM2:
		//_MDrv_PQ_DumpFilterTable(pIP_Info);

        if (_u8SRAM2Table[_scIdx] != pIP_Info->u8TabIdx)
        {
            _isNewSetting = true;
        }
        _u8SRAM2Table[_scIdx] = pIP_Info->u8TabIdx;

		if (_isNewSetting)
		{
            _MDrv_PQ_DumpFilterTable(pIP_Info);
		}
		/*
        if (_u8SRAM2Table != pIP_Info->u8TabIdx)
        {
            PQ_DUMP_FILTER_DBG(printf("old sram2: %u, new sram2: %u\n",
                (MS_U16)_u8SRAM2Table, (MS_U16)pIP_Info->u8TabIdx));
            _u8SRAM2Table = pIP_Info->u8TabIdx;
            _MDrv_PQ_DumpFilterTable(pIP_Info);
        }
        else
        {
            PQ_DUMP_FILTER_DBG(printf("use the same sram2: %u\n", (MS_U16)pIP_Info->u8TabIdx));
        }*/
        break;

    case PQ_TABTYPE_SRAM3:
		//_MDrv_PQ_DumpFilterTable(pIP_Info);
		if (_u8SRAM3Table[_scIdx] != pIP_Info->u8TabIdx)
        {
            _isNewSetting = true;
        }
        _u8SRAM3Table[_scIdx] = pIP_Info->u8TabIdx;
		if (_isNewSetting)
		{
            _MDrv_PQ_DumpFilterTable(pIP_Info);
		}
		/*
        if (_u8SRAM3Table != pIP_Info->u8TabIdx)
        {
            PQ_DUMP_FILTER_DBG(printf("old sram3: %u, new sram3: %u\n",
                (MS_U16)_u8SRAM3Table, (MS_U16)pIP_Info->u8TabIdx));
            _u8SRAM3Table = pIP_Info->u8TabIdx;
            _MDrv_PQ_DumpFilterTable(pIP_Info);
        }
        else
        {
            PQ_DUMP_FILTER_DBG(printf("use the same sram3: %u\n", (MS_U16)pIP_Info->u8TabIdx));
        }*/
        break;

    case PQ_TABTYPE_SRAM4:
		//_MDrv_PQ_DumpFilterTable(pIP_Info);
		if (_u8SRAM4Table[_scIdx] != pIP_Info->u8TabIdx)
        {
            _isNewSetting = true;
        }
        _u8SRAM4Table[_scIdx] = pIP_Info->u8TabIdx;
		if (_isNewSetting)
		{
            _MDrv_PQ_DumpFilterTable(pIP_Info);
		}
		/*
        if (_u8SRAM4Table != pIP_Info->u8TabIdx)
        {
            PQ_DUMP_FILTER_DBG(printf("old sram4: %u, new sram4: %u\n",
                (MS_U16)_u8SRAM4Table, (MS_U16)pIP_Info->u8TabIdx));
            _u8SRAM4Table = pIP_Info->u8TabIdx;
            _MDrv_PQ_DumpFilterTable(pIP_Info);
        }
        else
        {
            PQ_DUMP_FILTER_DBG(printf("use the same sram4: %u\n", (MS_U16)pIP_Info->u8TabIdx));
        }*/
        break;


    case PQ_TABTYPE_C_SRAM1:
		//_MDrv_PQ_DumpFilterTable(pIP_Info);

        if (_u8CSRAM1Table[_scIdx] != pIP_Info->u8TabIdx)
        {
            _isNewSetting = true;
        }
        _u8CSRAM1Table[_scIdx] = pIP_Info->u8TabIdx;

		if (_isNewSetting)
		{
            _MDrv_PQ_DumpFilterTable(pIP_Info);
		}
		/*
        if (_u8CSRAM1Table != pIP_Info->u8TabIdx)
        {
            PQ_DUMP_FILTER_DBG(printf("old Csram1: %u, new Csram1: %u\n",
                (MS_U16)_u8CSRAM1Table, (MS_U16)pIP_Info->u8TabIdx));
            _u8CSRAM1Table = pIP_Info->u8TabIdx;
            _MDrv_PQ_DumpFilterTable(pIP_Info);
        }
        else
        {
            PQ_DUMP_FILTER_DBG(printf("use the same Csram1: %u\n", (MS_U16)pIP_Info->u8TabIdx));
        }*/
        break;
    case PQ_TABTYPE_C_SRAM2:
		//_MDrv_PQ_DumpFilterTable(pIP_Info);
        if (_u8CSRAM2Table[_scIdx] != pIP_Info->u8TabIdx)
        {
            _isNewSetting = true;
        }
        _u8CSRAM2Table[_scIdx] = pIP_Info->u8TabIdx;

		if (_isNewSetting)
		{
            _MDrv_PQ_DumpFilterTable(pIP_Info);
		}
		/*
        if (_u8CSRAM2Table != pIP_Info->u8TabIdx)
        {
            PQ_DUMP_FILTER_DBG(printf("old Csram2: %u, new Csram2: %u\n",
                (MS_U16)_u8CSRAM2Table, (MS_U16)pIP_Info->u8TabIdx));
            _u8CSRAM2Table = pIP_Info->u8TabIdx;
            _MDrv_PQ_DumpFilterTable(pIP_Info);
        }
        else
        {
            PQ_DUMP_FILTER_DBG(printf("use the same Csram2: %u\n", (MS_U16)pIP_Info->u8TabIdx));
        }*/
        break;

    case PQ_TABTYPE_C_SRAM3:
		if (_u8CSRAM3Table[_scIdx] != pIP_Info->u8TabIdx)
        {
            _isNewSetting = true;
        }
        _u8CSRAM3Table[_scIdx] = pIP_Info->u8TabIdx;

		if (_isNewSetting)
		{
            _MDrv_PQ_DumpFilterTable(pIP_Info);
		}
        break;

    case PQ_TABTYPE_C_SRAM4:
		if (_u8CSRAM4Table[_scIdx] != pIP_Info->u8TabIdx)
        {
            _isNewSetting = true;
        }
        _u8CSRAM4Table[_scIdx] = pIP_Info->u8TabIdx;

		if (_isNewSetting)
		{
            _MDrv_PQ_DumpFilterTable(pIP_Info);
		}
        break;

    case PQ_TABTYPE_VIP_ICC_CRD_SRAM:
		//_MDrv_PQ_DumpFilterTable(pIP_Info);

        if (_u8ICC_CRD_Table[_scIdx] != pIP_Info->u8TabIdx)
        {
            _isNewSetting = true;
        }
        _u8ICC_CRD_Table[_scIdx] = pIP_Info->u8TabIdx;

		if (_isNewSetting)
		{
            _MDrv_PQ_DumpFilterTable(pIP_Info);
		}
		/*
        if( _u8ICC_CRD_Table != pIP_Info->u8TabIdx)
        {
            PQ_DUMP_FILTER_DBG(printf("old ICC_CRD_SRAM: %u, new ICC_CRD_SRAM: %u\n",
                (MS_U16)_u8ICC_CRD_Table, (MS_U16)pIP_Info->u8TabIdx));
            _u8ICC_CRD_Table = pIP_Info->u8TabIdx;
            _MDrv_PQ_DumpFilterTable(pIP_Info);

        }*/
        break;

    case PQ_TABTYPE_VIP_IHC_CRD_SRAM:
		//_MDrv_PQ_DumpFilterTable(pIP_Info);

        if (_u8IHC_CRD_Table[_scIdx] != pIP_Info->u8TabIdx)
        {
            _isNewSetting = true;
        }
        _u8IHC_CRD_Table[_scIdx] = pIP_Info->u8TabIdx;

		if (_isNewSetting)
		{
            _MDrv_PQ_DumpFilterTable(pIP_Info);
		}
		/*
        if( _u8IHC_CRD_Table != pIP_Info->u8TabIdx)
        {
            PQ_DUMP_FILTER_DBG(printf("old IHC_CRD_SRAM: %u, new IHC_CRD_SRAM: %u\n",
                (MS_U16)_u8IHC_CRD_Table, (MS_U16)pIP_Info->u8TabIdx));
            _u8IHC_CRD_Table = pIP_Info->u8TabIdx;
            _MDrv_PQ_DumpFilterTable(pIP_Info);

        }*/
        break;

    default:
        PQ_DUMP_DBG(printf("[PQ]DumpTable:unknown type: %u\n", pIP_Info->u8TabType));
        break;
    }

}

// return total IP count
static MS_U16 _MDrv_PQ_GetIPNum(void)
{
    PQ_DBG(printf("[PQ]IPNum=%u\n",_PQTableInfo.u8PQ_IP_Num));
    return (MS_U16)_PQTableInfo.u8PQ_IP_Num;
}

// return total table count of given IP
static MS_U16 _MDrv_PQ_GetTableNum(MS_U8 u8PQIPIdx)
{
    PQ_DBG(printf("[PQ]TabNum=%u\n", _PQTableInfo.pIPTAB_Info[u8PQIPIdx].u8TabNums));
    return (MS_U16)_PQTableInfo.pIPTAB_Info[u8PQIPIdx].u8TabNums;
}

// return current used table index of given IP
static MS_U16 _MDrv_PQ_GetCurrentTableIndex(MS_U8 u8PQIPIdx)
{
    PQ_DBG(printf("[PQ]CurrTableIdx=%u\n", _u8PQTabIdx[u8PQIPIdx]));
    return (MS_U16)_u8PQTabIdx[u8PQIPIdx];
}

static MS_U16 _MDrv_PQ_GetTableIndex(MS_U16 u16PQSrcType, MS_U8 u8PQIPIdx)
{
    if (u16PQSrcType >=_PQTableInfo.u8PQ_InputType_Num){
        //PQ_DBG(printf("[PQ]invalid input type\n"));
        #if PQ_ENABLE_DEBUG
			printf("[PQ]invalid input type\n");
		#endif
        return PQ_IP_NULL;
    }
    if (u8PQIPIdx >= _PQTableInfo.u8PQ_IP_Num){
        //PQ_DBG(printf("[PQ]invalid ip type\n"));
        #if PQ_ENABLE_DEBUG
			printf("[PQ]invalid ip type\n");
		#endif
        return PQ_IP_NULL;
    }
	#if PQ_ENABLE_DEBUG
		printf("[PQ]TableIdx=%u\n",(MS_U16)_PQTableInfo.pQuality_Map_Aray[u16PQSrcType * _PQTableInfo.u8PQ_IP_Num + u8PQIPIdx]);
	#endif
    //PQ_DBG(printf("[PQ]TableIdx=%u\n",(MS_U16)_PQTableInfo.pQuality_Map_Aray[u16PQSrcType * _PQTableInfo.u8PQ_IP_Num + u8PQIPIdx]));

    return (MS_U16)_PQTableInfo.pQuality_Map_Aray[u16PQSrcType * _PQTableInfo.u8PQ_IP_Num + u8PQIPIdx];
}

static EN_IP_Info _MDrv_PQ_GetTable(MS_U8 u8TabIdx, MS_U8 u8PQIPIdx)
{
    EN_IP_Info ip_Info;
    _u8PQTabIdx[u8PQIPIdx] = u8TabIdx;
    if (u8TabIdx != PQ_IP_NULL && u8TabIdx != PQ_IP_COMM) {
        ip_Info.pIPTable  = _PQTableInfo.pIPTAB_Info[u8PQIPIdx].pIPTable;
        ip_Info.u8TabNums = _PQTableInfo.pIPTAB_Info[u8PQIPIdx].u8TabNums;
        ip_Info.u8TabType = _PQTableInfo.pIPTAB_Info[u8PQIPIdx].u8TabType;
        ip_Info.u8TabIdx = u8TabIdx;
    }
    else if (u8TabIdx == PQ_IP_COMM) {
        ip_Info.pIPTable = _PQTableInfo.pIPTAB_Info[u8PQIPIdx].pIPCommTable;
        ip_Info.u8TabNums = 1;
        ip_Info.u8TabType = _PQTableInfo.pIPTAB_Info[u8PQIPIdx].u8TabType;
        ip_Info.u8TabIdx = 0;
    }
    else {
        ip_Info.pIPTable  = 0;
        ip_Info.u8TabNums = 0;
        ip_Info.u8TabType = 0;
        ip_Info.u8TabIdx = 0;
    }
    return ip_Info;
}

static void _MDrv_PQ_LoadTableData(MS_U8 u8TabIdx, MS_U8 u8PQIPIdx, MS_U8 *pTable, MS_U16 u16TableSize)
{
    EN_IP_Info ip_Info;
    MS_U32 u32Addr;
    MS_U16 u16Idx = 0;
	MS_U8  u8Mask, u8Value;

    ip_Info = _MDrv_PQ_GetTable(u8TabIdx, u8PQIPIdx);
    if (ip_Info.u8TabIdx >= ip_Info.u8TabNums){
        PQ_DUMP_DBG(printf("[PQ]IP_Info error: Scaler Reg Table\n"));
        return;
    }

    while (1)
    {
        u32Addr = (ip_Info.pIPTable[0]<<8) + ip_Info.pIPTable[1];
        pTable[u16Idx++] = ip_Info.pIPTable[REG_ADDR_SIZE+REG_MASK_SIZE+ip_Info.u8TabIdx];

        if(u16Idx > u16TableSize || u32Addr == _END_OF_TBL_)
            break;
		u8Mask = ip_Info.pIPTable[2];
		u8Value = ip_Info.pIPTable[3];
        PQ_DUMP_DBG(printf("XC bk =%x, addr=%x, msk=%x, value=%x\n", (MS_U16)((u32Addr&0xFF00)>>8), (MS_U16)(u32Addr&0xFF), (MS_U16)u8Mask, (MS_U16)u8Value));
        ip_Info.pIPTable+=(REG_ADDR_SIZE+REG_MASK_SIZE+ip_Info.u8TabNums); // next
    }
}

static void _MDrv_PQ_LoadTable(MS_U8 u8TabIdx, MS_U8 u8PQIPIdx)
{
    EN_IP_Info ip_Info;
    ip_Info = _MDrv_PQ_GetTable(u8TabIdx, u8PQIPIdx);
    _MDrv_PQ_DumpTable(&ip_Info);
#if(ENABLE_PQ_LOAD_TABLE_INFO)
    _MDrv_PQ_Set_LoadTableInfo_IP_Tab(u8PQIPIdx, u8TabIdx);
#endif

}

static void _MDrv_PQ_LoadCommTable(void)
{
    MS_U8 i;
    EN_IP_Info ip_Info;

    for (i = 0; i < SC_MAX_NUM; i++)
    {
        _u8SRAM1Table[i] = 0xFF;
        _u8SRAM2Table[i] = 0xFF;
        _u8SRAM3Table[i] = 0xFF;
        _u8SRAM4Table[i] = 0xFF;
        _u8CSRAM1Table[i] = 0xFF;
        _u8CSRAM2Table[i] = 0xFF;
        _u8CSRAM3Table[i] = 0xFF;
        _u8CSRAM4Table[i] = 0xFF;
        _u8ICC_CRD_Table[i] = 0xFF;
        _u8IHC_CRD_Table[i] = 0xFF;
    }

    PQ_DBG(printf("[PQ]LoadCommTable\n"));
    for (i=0; i<_PQTableInfo.u8PQ_IP_Num; i++){
        PQ_DBG(printf("  IP:%u\n", (MS_U16)i));
        //20140817 skip dump comm table according to skip rule
        if (_PQTableInfo.pSkipRuleIP[i]) {
            PQ_DBG(printf("skip ip idx:%u\n", i));
            continue;
        }
        if (i == PQ_IP_PQ_ByPass_Main || i == PQ_IP_SCinit_Main)
        {
            PQ_DBG(printf("[PQ LoadCommm] Skip SCinit and Bypass\n"));
            continue;
        }

        ip_Info = _MDrv_PQ_GetTable(PQ_IP_COMM, i);

    #if (PQTBL_REGTYPE == PQTBL_NORMAL)
        if((ip_Info.u8TabType >= PQ_TABTYPE_SRAM1 && ip_Info.u8TabType <= PQ_TABTYPE_C_SRAM4)||
           (ip_Info.u8TabType >= PQ_TABTYPE_VIP_IHC_CRD_SRAM && ip_Info.u8TabType <= PQ_TABTYPE_VIP_ICC_CRD_SRAM))
        {
            continue;
        }
    #endif
        _MDrv_PQ_DumpTable(&ip_Info);
    }
}

static MS_U8 _MDrv_PQ_GetXRuleTableIndex(MS_U8 u8XRuleType, MS_U8 u8XRuleIdx, MS_U8 u8XRuleIP)
{
    MS_U8 *pArray = _PQTableInfo.pXRule_Array[u8XRuleType];
    return pArray[((MS_U16)u8XRuleIdx) * _PQTableInfo.u8PQ_XRule_IP_Num[u8XRuleType] + u8XRuleIP];
}

static MS_U8 _MDrv_PQ_GetXRuleIPIndex(MS_U8 u8XRuleType, MS_U8 u8XRuleIP)
{
    MS_U8 *pArray = _PQTableInfo.pXRule_IP_Index[u8XRuleType];
    return pArray[u8XRuleIP];
}

static MS_U8 _MDrv_PQ_GetXRuleIPNum(MS_U8 u8XRuleType)
{
    return _PQTableInfo.u8PQ_XRule_IP_Num[u8XRuleType];
}

static MS_U8 _MDrv_PQ_GetGRule_LevelIndex(MS_U8 u8GRuleType, MS_U8 u8GRuleLvlIndex)
{
    MS_U8 *pArray = _PQTableInfo.pGRule_Level[u8GRuleType];
    return pArray[u8GRuleLvlIndex];
}

static MS_U8 _MDrv_PQ_GetGRule_IPIndex(MS_U8 u8GRuleType, MS_U8 u8GRuleIPIndex)
{
     MS_U8 *pArray = _PQTableInfo.pGRule_IP_Index[u8GRuleType];
    return pArray[u8GRuleIPIndex];
}

static MS_U8 _MDrv_PQ_GetGRule_TableIndex(MS_U8 u8GRuleType, MS_U8 u8PQSrcType, MS_U8 u8PQ_NRIdx, MS_U8 u8GRuleIPIndex)
{
//    return _PQTableInfo.pGRule_Array[u8PQSrcType][u8PQ_NRIdx][u8GRuleIPIndex];
    MS_U16 u16index;
    MS_U8 *pArray = _PQTableInfo.pGRule_Array[u8GRuleType];

    u16index = ((MS_U16)u8PQSrcType) * _PQTableInfo.u8PQ_GRule_Num[u8GRuleType] * _PQTableInfo.u8PQ_GRule_IPNum[u8GRuleType] +
               ((MS_U16)u8PQ_NRIdx) * _PQTableInfo.u8PQ_GRule_IPNum[u8GRuleType] +
               u8GRuleIPIndex;
    return pArray[u16index];
}

static void _MDrv_PQ_LoadTableBySrcType(MS_U16 u16PQSrcType, MS_U8 u8PQIPIdx, MS_BOOL bIsRot)
{
    EN_IP_Info ip_Info;
    MS_U8 QMIPtype_size,i;
    MS_U8 u8TabIdx;
    //XC_ApiStatusEx stXCStatusEx;	//$$$TODO: Need Check below skip HSD flow
    PQ_WIN eWindow = PQ_MAIN_WINDOW;
#if (PQ_ENABLE_PIP)
    MS_U16 u16SFilter = 0;
#endif
    if(u16PQSrcType == MDrv_PQ_GetSrcType(PQ_MAIN_WINDOW))
    {
        eWindow = PQ_MAIN_WINDOW;
    }
    else
    {
        u16PQSrcType -= 0x80;
        eWindow = PQ_SUB_WINDOW;
    }

    if (u8PQIPIdx==PQ_IP_ALL)
    {
        QMIPtype_size=_PQTableInfo.u8PQ_IP_Num;
        u8PQIPIdx=0;
    }
    else
    {
        QMIPtype_size=1;
    }

    //for debug
    //msAPI_Scaler_SetBlueScreen(DISABLE, 0x00);
    //MApi_XC_GenerateBlackVideo(FALSE);

    for(i=0; i<QMIPtype_size; i++, u8PQIPIdx++)
    {
        if (_PQTableInfo.pSkipRuleIP[u8PQIPIdx]) {
            PQ_DBG(printf("skip ip idx:%u\n", u8PQIPIdx));
            continue;
        }
#if (ENABLE_QMAP_DI_SETTING == 0)
        if (u8PQIPIdx >= PQ_IP_MADi_Main && u8PQIPIdx <= PQ_IP_UC_CTL_Main)
            continue;
#endif
        // below action must be applied to each corresponding eWindow
        // otherwise, sub window case will take PQ_IP_HSD_C_Main for other PQIP!
        // then other IP will be skipped.
        #if 0
        memset(&stXCStatusEx, 0, sizeof(XC_ApiStatusEx));
        stXCStatusEx.u16ApiStatusEX_Length = sizeof(XC_ApiStatusEx);
        stXCStatusEx.u32ApiStatusEx_Version = API_STATUS_EX_VERSION;
        if(eWindow == PQ_MAIN_WINDOW)
        {
            if(MApi_XC_GetStatusEx(&stXCStatusEx, MAIN_WINDOW) == sizeof(XC_ApiStatusEx))
            {
                //if customer scaling, and now it's going to dump HSD_Y and HSD_C, just skip it.
                if((!stXCStatusEx.bPQSetHSD) &&
                    ((u8PQIPIdx == PQ_IP_HSD_C_Main) || (u8PQIPIdx == PQ_IP_HSD_Y_Main)))
                {
                    PQ_DBG(printf("skip ip idx:%u because of customer scaling case\n", u8PQIPIdx));
                    continue;
                }
            }
        }
        #endif
#if (PQ_ENABLE_PIP)
        else
        {
        	#if 0
            if(MApi_XC_GetStatusEx(&stXCStatusEx, SUB_WINDOW) == sizeof(XC_ApiStatusEx))
            {
                //if customer scaling, and now it's going to dump HSD_Y and HSD_C, just skip it.
                if((!stXCStatusEx.bPQSetHSD) &&
                    ((u8PQIPIdx == PQ_IP_HSD_C_Sub) || (u8PQIPIdx == PQ_IP_HSD_Y_Sub)))
                {
                    PQ_DBG(printf("skip ip idx:%u because of customer scaling case\n", u8PQIPIdx));
                    continue;
                }
            }
			#endif
        }
#endif

        u8TabIdx = _MDrv_PQ_GetTableIndex(u16PQSrcType, u8PQIPIdx);
        if (bIsRot)
        {
            if (u8PQIPIdx == PQ_IP_MADi_Motion_Main)
            {
                u8TabIdx = PQ_IP_MADi_Motion_MOT_PMODE_Main;    //Set MADi Motion = MOT_PMODE for window rotate
            }
            else if (u8PQIPIdx >= PQ_IP_MADi_Main && u8PQIPIdx <= PQ_IP_UC_CTL_Main)
            {
                u8TabIdx = 0; //set to off for rotate
            }
        }
        PQ_DBG(printf("[PQ]SrcType=%u, IPIdx=%u, TabIdx=%u\n",
            (MS_U16)u16PQSrcType, (MS_U16)u8PQIPIdx, (MS_U16)u8TabIdx));

        ip_Info = _MDrv_PQ_GetTable(u8TabIdx, u8PQIPIdx);

#if (PQ_ENABLE_PIP)
        //Sub SRAM3,4 dumping will make main window flash, so set it to 0x0303 to skip sram affection.
        if(((u8PQIPIdx == PQ_IP_SRAM3_Sub) || (u8PQIPIdx == PQ_IP_SRAM4_Sub) ) && (PQ_SUB_WINDOW == eWindow))
        {
			#if 1
            u16SFilter = MApi_XC_R2BYTE(REG_SC_BK23_0B_L);
            MApi_XC_W2BYTEMSK(REG_SC_BK23_0B_L, 0x0303 , 0xFFFF );
			#else
			u16SFilter = msRead2Byte(u32ScBaseAddr + REG_SC_BK23_0B_L);
			msWrite2ByteMask(u32ScBaseAddr + REG_SC_BK23_0B_L, 0x0303, 0xFFFF);
			#endif
        }
#endif
        _MDrv_PQ_DumpTable(&ip_Info);

#if (PQ_ENABLE_PIP)
        if(((u8PQIPIdx == PQ_IP_SRAM3_Sub) || (u8PQIPIdx == PQ_IP_SRAM4_Sub) ) && (PQ_SUB_WINDOW == eWindow))
        {
            MApi_XC_W2BYTEMSK(REG_SC_BK23_0B_L, u16SFilter , 0xFFFF );
        }
#endif

#if(ENABLE_PQ_LOAD_TABLE_INFO)
        _MDrv_PQ_Set_LoadTableInfo_IP_Tab(u8PQIPIdx, u8TabIdx);
#endif

        //MsOS_DelayTask(1500);
    }
}

static void _MDrv_PQ_AddTable(PQTABLE_INFO *pPQTableInfo)
{
    _PQTableInfo = *pPQTableInfo;
}

static void _MDrv_PQ_CheckCommTable(void)
{
#if (PQ_ENABLE_CHECK == 1)
    u8PQfunction = PQ_FUNC_CHK_REG;
    _MDrv_PQ_LoadCommTable();
    u8PQfunction = PQ_FUNC_DUMP_REG;
#endif
}

static void _MDrv_PQ_CheckTableBySrcType(MS_U16 u16PQSrcType, MS_U8 u8PQIPIdx)
{
#if (PQ_ENABLE_CHECK == 1)
    u8PQfunction = PQ_FUNC_CHK_REG;
    _MDrv_PQ_LoadTableBySrcType(u16PQSrcType, u8PQIPIdx);
    u8PQfunction = PQ_FUNC_DUMP_REG;
#else
    UNUSED(u16PQSrcType);
    UNUSED(u8PQIPIdx);
#endif
}


INSTALL_PQ_FUNCTIONS(PQTABLE_NAME)
#endif

#undef _DRVPQ_TEMPLATE_H_
#endif /* _DRVPQ_TEMPLATE_H_ */


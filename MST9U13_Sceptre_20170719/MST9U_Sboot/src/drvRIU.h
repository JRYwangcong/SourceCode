////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
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

#ifndef __DRV_RIU_H__
#define __DRV_RIU_H__

typedef unsigned char MS_U8;
typedef MS_U8 MS_REG_INIT;

#define _RV1(addr, value)   (((addr) >> 8) & 0x3F), (MS_U8)(addr), (MS_U8)(value)
#define _RV2(addr, value)   0x40 + _RV1(addr, value), (MS_U8)((value) >> 8)
#define _RV3(addr, value)   0x40 + _RV2(addr, value), (MS_U8)((value) >> 16)
#define _RV4(addr, value)   0x40 + _RV3(addr, value), (MS_U8)((value) >> 24)

#define _RV32_1(addr, value)   (MS_U8)(((addr) >> 16) & 0xFF), (MS_U8)(((addr) >> 8) & 0xFF), (MS_U8)(addr), (MS_U8)(value)
#define _RV32_2(addr, value)   0x20 + _RV32_1(addr, value), (MS_U8)((value) >> 8)
#define _RV32_3(addr, value)   0x20 + _RV32_2(addr, value), (MS_U8)((value) >> 16)
#define _RV32_4(addr, value)   0x20 + _RV32_3(addr, value), (MS_U8)((value) >> 24)
#define _END_OF_TBL32_         0xFF, 0xFF, 0xFF

#define _RVM1(addr, value, mask) (((addr) >> 8) & 0x3F), (MS_U8)(addr), (MS_U8)(value), (MS_U8)(mask)
#define _RVM2(addr, value, mask) 0x40 + _RVM1(addr, value, mask), (MS_U8)((value) >> 8), (MS_U8)((mask) >> 8)
#define _RVM3(addr, value, mask) 0x40 + _RVM2(addr, value, mask), (MS_U8)((value) >> 16), (MS_U8)((mask) >> 16)
#define _RVM4(addr, value, mask) 0x40 + _RVM3(addr, value, mask), (MS_U8)((value) >> 24), (MS_U8)((mask) >> 24)
#define _END_OF_TBL_        0xFF, 0xFF

#ifdef __cplusplus
extern "C" {
#endif

void MDrv_WriteRegMaskTbl(const MS_REG_INIT *pRegMaskTbl);
void MDrv_WriteRegTbl32(const MS_REG_INIT *pRegTbl32);
void MDrv_WriteRegTbl32Off(const MS_REG_INIT *pRegTbl32, int iRegOffset);

#ifdef __cplusplus
}
#endif

#endif /* __DRV_RIU_H__ */

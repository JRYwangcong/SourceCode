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

#ifndef DRV_GLOBAL_H
#define DRV_GLOBAL_H

#include "datatype.h"

/********************************************************************************/
/*                           BDMA definition                                              */
/********************************************************************************/
#define    DW_1BYTE	                           0x00
#define    DW_2BYTE	                           0x10
#define    DW_4BYTE	                           0x20
#define    DW_8BYTE	                           0x30
#define    DW_16BYTE	                       0x40

#define    DRAM_MIU_0                          0x00//unknown 0x30
#define    DRAM_MIU_1                          0x01//unknown0x31
#define    PATTERN_SEARCH                      0x02
#define    CRC32                               0x03
#define    PATTERN_FILL                        0x04//unknown0x34
#define    SPI_FLASH                           0x05//unknown0x35
#define    NA_RESERVED                         0x06
#define    VDMCU                               0x07
#define    DSP                                 0x08
#define    TSP_MCU                             0x09


/********************************************************************************/
/*                           Macro                                              */
/********************************************************************************/

typedef U8 MS_REG_INIT;

#define _RV1(addr, value)   (((addr) >> 8) & 0x3F), (U8)(addr), (U8)(value)
#define _RV2(addr, value)   0x40 + _RV1(addr, value), (U8)((value) >> 8)
#define _RV3(addr, value)   0x40 + _RV2(addr, value), (U8)((value) >> 16)
#define _RV4(addr, value)   0x40 + _RV3(addr, value), (U8)((value) >> 24)


#define _RV32_1(addr, value)    (U8)(((addr) >> 16) & 0xFF),(U8)(((addr) >> 8) & 0xFF), (U8)(addr), (U8)(value)
#define _RV32_2(addr, value)   0x20 + _RV32_1(addr, value), (U8)((value) >> 8)
#define _RV32_3(addr, value)   0x20 + _RV32_2(addr, value), (U8)((value) >> 16)
#define _RV32_4(addr, value)   0x20 + _RV32_3(addr, value), (U8)((value) >> 24)
#define _END_OF_TBL32_         0xFF, 0xFF, 0xFF


#define _RVM1(addr, value, mask) (((addr) >> 8) & 0x3F), (U8)(addr), (U8)(value), (U8)(mask)
#define _RVM2(addr, value, mask) 0x40 + _RVM1(addr, value, mask), (U8)((value) >> 8), (U8)((mask) >> 8)
#define _RVM3(addr, value, mask) 0x40 + _RVM2(addr, value, mask), (U8)((value) >> 16), (U8)((mask) >> 16)
#define _RVM4(addr, value, mask) 0x40 + _RVM3(addr, value, mask), (U8)((value) >> 24), (U8)((mask) >> 24)

#define _END_OF_TBL2_       0xFF, 0xFF

/******************************************************************************/
/*                   Function Prototypes                                      */
/******************************************************************************/

#ifdef DRV_GLOBAL_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

#define MDrv_ReadByte( u32Reg )                                                         \
    (__builtin_constant_p( u32Reg ) ?                                                   \
        (((u32Reg) & 0x01) ? RIU8[(u32Reg) * 2 - 1] : RIU8[(u32Reg) * 2]) :             \
        __MDrv_ReadByte( u32Reg ))

#define MDrv_Read2Byte( u32Reg )                                                        \
    (__builtin_constant_p( u32Reg ) && !((u32Reg) & 0x01) ?                             \
        RIU[u32Reg] : __MDrv_Read2Byte( u32Reg ))

#define MDrv_Read3Byte( u32Reg )    __MDrv_Read3Byte( u32Reg )

#define MDrv_Read4Byte( u32Reg )                                                        \
    ({                                                                                  \
    U32 value;                                                                          \
    if ( __builtin_constant_p( u32Reg ) && !((u32Reg) & 0x01) )                         \
    {                                                                                   \
        value = ((U32)RIU[(u32Reg) + 2] << 16) | RIU[u32Reg];                           \
    }                                                                                   \
    else                                                                                \
    {                                                                                   \
        value = __MDrv_Read4Byte( u32Reg );                                             \
    }                                                                                   \
    value;                                                                              \
    })

#define MDrv_ReadRegBit( u32Reg, u8Mask )                                               \
        (RIU8[(u32Reg) * 2 - ((u32Reg) & 1)] & (u8Mask))

#define MDrv_WriteRegBit( u32Reg, bEnable, u8Mask )                                     \
    MST_MACRO_START                                                                     \
    U32 u32Reg8 = ((u32Reg) * 2) - ((u32Reg) & 1);                                      \
    RIU8[u32Reg8] = (bEnable) ? (RIU8[u32Reg8] |  (u8Mask)) :                           \
                                (RIU8[u32Reg8] & ~(u8Mask));                            \
    MST_MACRO_END

#define MDrv_WriteByte( u32Reg, u8Val )                                                 \
    MST_MACRO_START                                                                     \
    if ( __builtin_constant_p( u32Reg ) )                                               \
    {                                                                                   \
        RIU8[((u32Reg) * 2) - ((u32Reg) & 1)] = u8Val;                                  \
    }                                                                                   \
    else                                                                                \
    {                                                                                   \
        __MDrv_WriteByte( u32Reg, u8Val );                                              \
    }                                                                                   \
    MST_MACRO_END

#define MDrv_Write2Byte( u32Reg, u16Val )                                               \
    MST_MACRO_START                                                                     \
    if ( __builtin_constant_p( u32Reg ) )                                               \
    {                                                                                   \
        if ( ((u32Reg) & 0x01) )                                                        \
        {                                                                               \
            RIU8[((u32Reg) * 2) - 1] = (U8)((u16Val));                                  \
            RIU8[((u32Reg) + 1) * 2] = (U8)((u16Val) >> 8);                             \
        }                                                                               \
        else                                                                            \
        {                                                                               \
            RIU[u32Reg] = u16Val;                                                       \
        }                                                                               \
    }                                                                                   \
    else                                                                                \
    {                                                                                   \
        __MDrv_Write2Byte( u32Reg, u16Val );                                            \
    }                                                                                   \
    MST_MACRO_END

#define MDrv_Write3Byte( u32Reg, u32Val )   __MDrv_Write3Byte( u32Reg, u32Val )

#define MDrv_Write4Byte( u32Reg, u32Val )                                               \
    MST_MACRO_START                                                                     \
    if ( __builtin_constant_p( u32Reg ) && !((u32Reg) & 0x01) )                         \
    {                                                                                   \
        RIU[u32Reg] = (U16)(u32Val);                                                    \
        RIU[(u32Reg) + 2] = (U16)((u32Val) >> 16);                                      \
    }                                                                                   \
    else                                                                                \
    {                                                                                   \
        __MDrv_Write4Byte( u32Reg, u32Val );                                            \
    }                                                                                   \
    MST_MACRO_END

#define MDrv_WriteByteMask( u32Reg, u8Val, u8Msk )                                      \
    MST_MACRO_START                                                                     \
    U32 u32Reg8 = ((u32Reg) * 2) - ((u32Reg) & 1);                                      \
    RIU8[u32Reg8] = (RIU8[u32Reg8] & ~(u8Msk)) | ((u8Val) & (u8Msk));                   \
    MST_MACRO_END

#define MDrv_Write2ByteMask( u32Reg, u16Val, u16Msk )                                   \
    MST_MACRO_START                                                                     \
    MDrv_WriteByteMask(u32Reg,(U8)(u16Val),(U8)(u16Msk));                               \
    MDrv_WriteByteMask((u32Reg)+1,(U8)((u16Val)>>8),(U8)((u16Msk)>>8));                \
    MST_MACRO_END

INTERFACE U8   __MDrv_ReadByte( U32 u32Reg );
INTERFACE U16  __MDrv_Read2Byte( U32 u32Reg );
INTERFACE U32  __MDrv_Read3Byte( U32 u32Reg );
INTERFACE U32  __MDrv_Read4Byte( U32 u32Reg );

INTERFACE void __MDrv_WriteByte( U32 u32Reg, U8 u8Val );
INTERFACE void __MDrv_Write2Byte( U32 u32Reg, U16 u16Val );
INTERFACE void __MDrv_Write3Byte( U32 u32Reg, U32 u32Val );
INTERFACE void __MDrv_Write4Byte( U32 u32Reg, U32 u32Val );

INTERFACE void MDrv_WriteRegTbl ( const MS_REG_INIT *pRegTable );
INTERFACE void MDrv_WriteRegTbl_32 ( const MS_REG_INIT *pRegTable );

INTERFACE void MDrv_SysDelayUs(U32 u32InputTime);
#if ENABLE_USB
INTERFACE U32 MDrv_GetSystemTime (void); //   1ms/systime
INTERFACE U32 MDrv_Timer_DiffTimeFromNow(U32 u32TaskTimer); //unit = ms
#endif
#define MDrv_WriteRegTbl2( pRegTable )  MDrv_WriteRegTbl( pRegTable )

#undef INTERFACE

#endif /* DRV_GLOBAL_H */

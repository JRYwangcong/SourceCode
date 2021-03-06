////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2008 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (; MStar; Confidential; Information; ) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
//****************************************************
//   ADC initial table CodeGen Version 1.0
// Rose_ADC_Driver
// 2011/5/23 09:43
//****************************************************

#ifndef _DRVADCTBL_H_
#define _DRVADCTBL_H_

//   ADC initial table CodeGen Version 1.0
#define DRV_ADC_REG(reg) ((reg>>8)&0xFF), (reg&0xFF)
#define REG_ADDR_SIZE       2
#define REG_MASK_SIZE       1
#define REG_DATA_SIZE       2
typedef enum
{
    ADC_TABLE_INIT_All,
    ADC_TABLE_INIT_NUMS
} ADC_INIT_TYPE;

typedef enum
{
    ADC_TABLE_FreeRunEn_All,
    ADC_TABLE_FreeRunEn_NUMS
} ADC_FreeRunEn_TYPE;

typedef enum
{
    ADC_TABLE_FreeRunDis_All,
    ADC_TABLE_FreeRunDis_NUMS
} ADC_FreeRunDis_TYPE;

typedef enum
{
    ADC_TABLE_PorstEn_All,
    ADC_TABLE_PorstEn_NUMS
} ADC_PorstEn_TYPE;

typedef enum
{
    ADC_TABLE_PorstDis_All,
    ADC_TABLE_PorstDis_NUMS
} ADC_PorstDis_TYPE;

typedef enum
{
    ADC_TABLE_MUX_RGB0_Sync,
    ADC_TABLE_MUX_RGB0_Data,
    ADC_TABLE_MUX_NUMS
} ADC_MUX_TYPE;

typedef enum
{
    ADC_TABLE_SOURCE_RGB,
    ADC_TABLE_SOURCE_YUV,
    ADC_TABLE_SOURCE_SOG,
    ADC_TABLE_SOURCE_YUV_HV,
    ADC_TABLE_SOURCE_NUMS
} ADC_SOURCE_TYPE;

typedef enum
{
    ADC_TABLE_AdcCal_ALL,
    ADC_TABLE_AdcCal_NUMS
} ADC_AdcCal_TYPE;

typedef enum
{
    ADC_TABLE_FREQ_SECTION1,
    ADC_TABLE_FREQ_SECTION2,
    ADC_TABLE_FREQ_SECTION3,
    ADC_TABLE_FREQ_SECTION4,
    ADC_TABLE_FREQ_SECTION5,
    ADC_TABLE_FREQ_SECTION6,
    ADC_TABLE_FREQ_SECTION7,
    ADC_TABLE_FREQ_SECTION8,
    ADC_TABLE_FREQ_SECTION9,
    ADC_TABLE_FREQ_SECTION10,
    ADC_TABLE_FREQ_SECTION11,
    ADC_TABLE_FREQ_SECTION12,
    ADC_TABLE_FREQ_SECTION13,
    ADC_TABLE_FREQ_SECTION14,
    ADC_TABLE_FREQ_SECTION15,
    ADC_TABLE_FREQ_SECTION16,
    ADC_TABLE_FREQ_SECTION17,
    ADC_TABLE_SetMode_NUMS
} ADC_SetMode_TYPE;

typedef enum
{
    ADC_TABLE_PowerDown_NUMS
} ADC_PowerDown_TYPE;

typedef struct
{
    WORD FreqHLimit;
    WORD FreqLLimit;
} ADC_FREQ_RANGE;

#endif


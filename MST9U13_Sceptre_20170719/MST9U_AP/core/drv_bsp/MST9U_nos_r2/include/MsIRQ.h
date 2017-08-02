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
/// file   MsIRQ.h
/// @brief  MStar IRQ
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef _MS_IRQ_H_
#define _MS_IRQ_H_



#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
// Type and Structure Declaration
//-------------------------------------------------------------------------------------------------
#define MS_IRQ_MAX          (128) //64 IRQs + 64 FIQs
#define ENABLE_USB_PORT0
#define E_IRQ_FIQ_INVALID   0xFFFF

// Interrupt related
typedef enum
{
    // FIQ
    E_INT_FIQL_START                = 0,
    E_INT_FIQ_EXTIMER0              = E_INT_FIQL_START +  0,
    E_INT_FIQ_WDT                   = E_INT_FIQL_START +  1,
    E_INT_FIQ_VIVALDI_STR           = E_INT_FIQL_START +  2,
    E_INT_FIQ_VIVALDI_PTS           = E_INT_FIQL_START +  3,
    E_INT_FIQ_XIU_TIMEOUT           = E_INT_FIQL_START + 19,
    E_INT_FIQ_PWM_FP_R              = E_INT_FIQL_START + 23,
    E_INT_FIQ_PWM_RP_R              = E_INT_FIQL_START + 24,
    E_INT_FIQ_PWM_FP_L              = E_INT_FIQL_START + 25,
    E_INT_FIQ_PWM_RP_L              = E_INT_FIQL_START + 26,
    E_INT_FIQH_END                  = 31,

    // FIQEXP
    E_INT_FIQEXPL_START             = 32,
    E_INT_FIQ_8051_TO_HST3          = E_INT_FIQEXPL_START +  4,
    E_INT_FIQ_8051_TO_PQR2          = E_INT_FIQEXPL_START +  5,
    E_INT_FIQ_8051_TO_R2          = E_INT_FIQEXPL_START +  6,
    E_INT_FIQ_R2_TO_HST3          = E_INT_FIQEXPL_START +  8,
    E_INT_FIQ_R2_TO_PQR2          = E_INT_FIQEXPL_START +  9,
    E_INT_FIQ_R2_TO_8051          = E_INT_FIQEXPL_START + 10,
    E_INT_FIQ_PQR2_TO_HST3          = E_INT_FIQEXPL_START + 12,
    E_INT_FIQ_PQR2_TO_R2          = E_INT_FIQEXPL_START + 13,
    E_INT_FIQ_PQR2_TO_8051          = E_INT_FIQEXPL_START + 14,
    E_INT_FIQ_HST3TOHST2_INT        = E_INT_FIQEXPL_START + 16,
    E_INT_FIQ_HST3TOHST1_INT        = E_INT_FIQEXPL_START + 17,
    E_INT_FIQ_HST3TOHST0_INT        = E_INT_FIQEXPL_START + 18,
    E_INT_FIQEXPH_END               = 63,


    // IRQ
    E_INT_IRQL_START                = 64,
    E_INT_IRQ_UART0                 = E_INT_IRQL_START  +  0,
    E_INT_IRQ_VIVALDI_DMA_INTR1     = E_INT_IRQL_START  +  1,
    E_INT_IRQ_PM_FIQ_OUT            = E_INT_IRQL_START  +  2,
    E_INT_IRQ_COMB                  = E_INT_IRQL_START  +  7,
    E_INT_IRQ_USB                   = E_INT_IRQL_START  +  8,
    E_INT_IRQ_UHC                   = E_INT_IRQL_START  +  9,
    E_INT_IRQ_OTG                   = E_INT_IRQL_START  + 10,
    E_INT_IRQ_IIC1                  = E_INT_IRQL_START  + 13,
    E_INT_IRQ_IIC0                  = E_INT_IRQL_START  + 22,
    E_INT_IRQ_AUX_TX3               = E_INT_IRQL_START  + 27,
    E_INT_IRQ_AUX_TX2               = E_INT_IRQL_START  + 28,
    E_INT_IRQ_AUX_TX1               = E_INT_IRQL_START  + 29,
    E_INT_IRQ_AUX_TX0               = E_INT_IRQL_START  + 30,
    E_INT_IRQH_END                  = 95,

    //IRQEXP
    E_INT_IRQEXPL_START             = 96,
    E_INT_IRQ_EPD_TX                = E_INT_IRQEXPL_START +  0,
    E_INT_IRQ_PM_IRQ_OUT            = E_INT_IRQEXPL_START + 14,
    E_INT_IRQ_BDMA0                 = E_INT_IRQEXPL_START + 16,
    E_INT_IRQ_BDMA1                 = E_INT_IRQEXPL_START + 17,
    E_INT_IRQ_SC_R1                 = E_INT_IRQEXPL_START + 21,
    E_INT_IRQ_SC_R0                 = E_INT_IRQEXPL_START + 22,
    E_INT_IRQ_SC_L1                 = E_INT_IRQEXPL_START + 23,
    E_INT_IRQ_SC_L0                 = E_INT_IRQEXPL_START + 24,
    E_INT_IRQ_GOP                   = E_INT_IRQEXPL_START + 28,
    E_INT_IRQ_GE                    = E_INT_IRQEXPL_START + 29,
    E_INT_IRQEXPH_END               = 127,

    E_INT_IRQ_FIQ_NONE              = 0xFE,
    E_INT_IRQ_FIQ_ALL               = 0xFF,
    E_INT_FIQ_0xF0_END              = 0xFF,


    //1 !!NON existed interrupt for MST9U, workaround solution, need to remove later..
    E_INT_FIQ_EXTIMER1              = E_INT_IRQ_FIQ_NONE,
    E_INT_IRQ_UART1                 = E_INT_IRQ_FIQ_NONE,
    E_INT_IRQ_PM                    = E_INT_IRQ_FIQ_NONE,
} InterruptNum;

#ifdef __cplusplus
}
#endif

#endif // _MS_IRQ_H_

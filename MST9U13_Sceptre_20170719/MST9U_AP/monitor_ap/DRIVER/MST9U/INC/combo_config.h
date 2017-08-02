///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   Combo_config.h
/// @author MStar Semiconductor Inc.
/// @brief  MHL driver Function
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "msEread.h"

#ifndef _COMBO_CONFIG_H_
#define _COMBO_CONFIG_H_

//-------------------------------------------------------------------------------------------------
// This chip support 6 digital input
//-------------------------------------------------------------------------------------------------
// Port 0: DVI/ HDMI 1.4/ MHL 2.0/ DP 1.2
// Port 1: DVI/ HDMI 1.4/ MHL 2.0/ DP 1.2
// Port 2: DVI/ HDMI 1.4/ MHL 2.0/ DP 1.2
// Port 3: DVI/ HDMI 1.4/ MHL 2.0/ DP 1.2
// Port 4: DVI/ HDMI 1.4/ MHL 2.0/ DP 1.2
// Port 5: DVI/ HDMI 1.4/ MHL 2.0/ DP 1.2
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define COMBO_IP_SUPPORT_0PORT          0
#define COMBO_IP_SUPPORT_1PORT          1
#define COMBO_IP_SUPPORT_2PORT          2
#define COMBO_IP_SUPPORT_3PORT          3
#define COMBO_IP_SUPPORT_4PORT          4
#define COMBO_IP_SUPPORT_5PORT          5
#define COMBO_IP_SUPPORT_6PORT          6

#define COMBO_IP_SUPPORT_TYPE           COMBO_IP_SUPPORT_6PORT

// Just for chip bring to force disable some block function
#define COMBO_EDID_BLOCK_ENABLE         1
#define COMBO_HDCP14_BLOCK_ENABLE       1
#define COMBO_AUDIO_BLOCK_ENABLE        1
#define COMBO_ADC_BLOCK_ENABLE          1
#define COMBO_TMDS_BLOCK_ENABLE         1
#define COMBO_MHL_BLOCK_ENABLE          1
#define COMBO_DP_BLOCK_ENABLE           1

// MHL part
#define COMBO_MHL_SUPPORT_PORT0         1
#define COMBO_MHL_SUPPORT_PORT1         1
#define COMBO_MHL_SUPPORT_PORT2         1
#define COMBO_MHL_SUPPORT_PORT3         1
#define COMBO_MHL_SUPPORT_PORT4         1
#define COMBO_MHL_SUPPORT_PORT5         1

#define COMBO_MHL3_SUPPORT_PORT0        0
#define COMBO_MHL3_SUPPORT_PORT1        0
#define COMBO_MHL3_SUPPORT_PORT2        0
#define COMBO_MHL3_SUPPORT_PORT3        0
#define COMBO_MHL3_SUPPORT_PORT4        0
#define COMBO_MHL3_SUPPORT_PORT5        0

#define COMBO_MHL_CTS_MODE_ENABLE       0

// HDCP 2.2
#if (ENABLE_SECU_R2 == 1) && (ENABLE_HDCP22 == 1)
#define COMBO_HDCP2_FUNCTION_SUPPORT    0
#define COMBO_HDCP2_DECODE_KEY          0
#define COMBO_HDCP2_HDMI14PORT_EN       0
#define COMBO_HDCP2_HDMI20PORT_EN       0
#define COMBO_HDCP2_INITPROC_NEW_MODE   0
    #if (COMBO_HDCP2_INITPROC_NEW_MODE)
    #define COMBO_HDCP2_SECUR2INT_WDT       0 //unit: ms; not used in MST9U project
    #else
    #define COMBO_HDCP2_SECUR2INT_WDT       0 //unit: ms; not used in MST9U project
    #endif
#else
#define COMBO_HDCP2_FUNCTION_SUPPORT    0
#endif

// Daisy chain
#define COMBO_DP_DAISY_CHAIN_SUPPORT    ENABLE_DAISY_CHAIN
#define COMBO_DAISY_CHAIN_PORT_SELECT   1

#define COMBO_DP_DC_SUPPORT_PORT0       0
#define COMBO_DP_DC_SUPPORT_PORT1       1
#define COMBO_DP_DC_SUPPORT_PORT2       0
#define COMBO_DP_DC_SUPPORT_PORT3       0
#define COMBO_DP_DC_SUPPORT_PORT4       0
#define COMBO_DP_DC_SUPPORT_PORT5       0

// HDCP key load from efuse
#define ENABLE_LOAD_KEY_VIA_EFUSE       0

// TMDS use internal EDID
#define COMBO_TMDS_USE_INTERNAL_EDID    0

// EQ
// 0x00 < 0x01 < ... < 0x1E < 0x1F
#define COMBO_HDMI_270_340MHZ_EQ_VALUE            \
	( (msEread_GetDataFromEfuse(EFUSE_0, 0x13C) & 0xC0) ? (msEread_GetDataFromEfuse(EFUSE_0, 0x139) & 0x1F) : 0x0C)
#define COMBO_HDMI_25_270MHZ_EQ_VALUE            \
	( (msEread_GetDataFromEfuse(EFUSE_0, 0x13C) & 0xC0) ? (msEread_GetDataFromEfuse(EFUSE_0, 0x13A) & 0x1F) : 0x18)
#define COMBO_HDMI_EQ_VALUE             COMBO_HDMI_25_270MHZ_EQ_VALUE//0xA

#define COMBO_DP_EQ_VALUE            \
	( (msEread_GetDataFromEfuse(EFUSE_0, 0x13C) & 0xC0) ? (msEread_GetDataFromEfuse(EFUSE_0, 0x13B) & 0x1F) : 20)

#define COMBO_HDMI14_CLK_CHANNEL_EQ     COMBO_HDMI_EQ_VALUE
#define COMBO_HDMI14_B_CHANNEL_EQ       COMBO_HDMI_EQ_VALUE
#define COMBO_HDMI14_G_CHANNEL_EQ       COMBO_HDMI_EQ_VALUE
#define COMBO_HDMI14_R_CHANNEL_EQ       COMBO_HDMI_EQ_VALUE

#define COMBO_HDMI20_CLK_CHANNEL_EQ     COMBO_HDMI_EQ_VALUE
#define COMBO_HDMI20_B_CHANNEL_EQ       COMBO_HDMI_EQ_VALUE
#define COMBO_HDMI20_G_CHANNEL_EQ       COMBO_HDMI_EQ_VALUE
#define COMBO_HDMI20_R_CHANNEL_EQ       COMBO_HDMI_EQ_VALUE

#define COMBO_DP_LANE0_EQ               COMBO_DP_EQ_VALUE
#define COMBO_DP_LANE1_EQ               COMBO_DP_EQ_VALUE
#define COMBO_DP_LANE2_EQ               COMBO_DP_EQ_VALUE
#define COMBO_DP_LANE3_EQ               COMBO_DP_EQ_VALUE

#define COMBO_TMDS_CLOCK_DETECT         1
#define COMBO_TMDS_POWER_CONTROL        0

#endif



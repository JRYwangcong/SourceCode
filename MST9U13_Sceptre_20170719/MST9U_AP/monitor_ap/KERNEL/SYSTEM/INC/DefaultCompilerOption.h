#ifndef _DEFAULT_COMPILER_OPTION_H_
#define _DEFAULT_COMPILER_OPTION_H_

////////////////////////////////////////////////////////////
// SI
////////////////////////////////////////////////////////////
#ifndef ODM_NAME
#define ODM_NAME                                ODM_AOC
#endif

#ifndef FACTORYALIGN_TYPE
#define FACTORYALIGN_TYPE                       FACTORYALIGN_DDCCI
#endif

#ifndef ENABLE_POWEROFF_1X_MCU_CLK
#define ENABLE_POWEROFF_1X_MCU_CLK              0
#endif
////////////////////////////////////////////////////////////
// APPLICATION
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// MCU
////////////////////////////////////////////////////////////
#ifndef ENABLE_MSTV_UART_DEBUG
#define ENABLE_MSTV_UART_DEBUG        1
#endif

#ifndef UART1
#define UART1               0
#endif

#ifndef ENABLE_PIU_UART_ResetCheck   //mcu_if  6.11
#define ENABLE_PIU_UART_ResetCheck        0
#endif


/////////////////////////////////////////////////////////////
// ADC
/////////////////////////////////////////////////////////////
#ifndef ENABLE_ADC_RESET
#define ENABLE_ADC_RESET    1 // 120413 coding addition
#endif

#ifndef ADC_HSYNC_LVL_DEF // each main board may have different setting
#define ADC_HSYNC_LVL_DEF   ADC_HSYNC_LVL_5 // 120725 coding, default set to 5 to compatible with old chips
#endif
////////////////////////////////////////////////////////////
// OSD
////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////
#ifndef MS_VGA_SOG_EN
#define MS_VGA_SOG_EN               0
#endif

#ifndef DVI_RB_SWAP
#define DVI_RB_SWAP                 0
#endif

#ifndef DVI_PN_SWAP
#define DVI_PN_SWAP                 0
#endif

#ifndef HDMI_RB_SWAP
#define HDMI_RB_SWAP                0
#endif

#ifndef HDMI_PN_SWAP
#define HDMI_PN_SWAP                0
#endif

#ifndef DISPLAY_UNDERSCAN_SPACE
#define DISPLAY_UNDERSCAN_SPACE     0// unit: pixel
#endif

#ifndef DISPLAY_UNDERSCAN_FREQ
#define DISPLAY_UNDERSCAN_FREQ      480 // 48Hz
#endif

#ifndef ENABLE_RTE
#define ENABLE_RTE                  0
#endif

#ifndef ENABLE_OD_MTB
#define ENABLE_OD_MTB             0
#endif

#ifndef ENABLE_TNR
#define ENABLE_TNR                  0
#endif

#ifndef USE_VCTRL
#define USE_VCTRL                   0
#endif

#ifndef USE_EXTERNAL_LDO
#define USE_EXTERNAL_LDO            0
#endif

#ifndef DHDMI_SIMPLAYHD_PATCH
#define DHDMI_SIMPLAYHD_PATCH       0
#endif

#ifndef ENABLE_SPDIF
#define ENABLE_SPDIF                0
#endif
#ifndef ENABLE_I2S
#define ENABLE_I2S                0
#endif

////////////////////////////////////////////////////////////
// AUTO FUNC.
////////////////////////////////////////////////////////////
#ifndef ENABLE_HWAUTO_ADCOFFSET
#define ENABLE_HWAUTO_ADCOFFSET     1
#endif

////////////////////////////////////////////////////////////
// POWERMANAGEMENT
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// SYSTEM
////////////////////////////////////////////////////////////
#ifndef HDCPKEY_USE_CODE
#define HDCPKEY_USE_CODE            1
#endif

#ifndef _NEW_SOG_DET_
#define _NEW_SOG_DET_               0
#endif

#ifndef ENABLE_WATCH_DOG
#define ENABLE_WATCH_DOG            0 // need sync with 51 setting,
#endif

#ifndef ENABLE_USB_INPUT
#define ENABLE_USB_INPUT               0
#endif

#ifndef ENABLE_R2_CODE_ON_DRAM
#define ENABLE_R2_CODE_ON_DRAM      0
#endif

#ifndef R2_51_DUAL_MODE
#define R2_51_DUAL_MODE             0 // need sync with 51 setting
#endif

#ifndef ENABLE_AUTO_SELECT_SOURCE
#define ENABLE_AUTO_SELECT_SOURCE   1
#endif

#ifndef ENABLE_MBX
#define ENABLE_MBX                  0 // need sync with 51 setting
#endif

#if ENABLE_MBX
#define ENABLE_MBX_SAR              1 // need sync with 51 setting
#else
#define ENABLE_MBX_SAR              0
#endif


#ifndef DDR_SSC_MODULATION_DEF
#define DDR_SSC_MODULATION_DEF      30 // unit: 1KHz, range 0~30 means 0~30KHz
#endif

#ifndef DDR_SSC_PERCENTAGE_DEF
#define DDR_SSC_PERCENTAGE_DEF      10 // unit: 0.1%, range 0~10 means 0~1%
#endif

#ifndef DDR_SSC_MODULATION_MAX
#define DDR_SSC_MODULATION_MAX      30 // unit: 1KHz, range 0~30 means 0~30KHz
#endif

#ifndef DDR_SSC_PERCENTAGE_MAX
#define DDR_SSC_PERCENTAGE_MAX      10 // unit: 0.1%, range 0~10 means 0~1%
#endif

#ifndef ENABLE_SCALER_LRSWAP
#define ENABLE_SCALER_LRSWAP         0
#endif

////////////////////////////////////////////////////////////
// DDCCI
////////////////////////////////////////////////////////////

#ifndef DDCCI_REPLY_NULL_MESSAGE    // wait for coding
#define DDCCI_REPLY_NULL_MESSAGE    0 // set to reply null message for special case
#endif

////////////////////////////////////////////////////////////
// external device
////////////////////////////////////////////////////////////
//============= TOUCH KEY==============================
#ifndef ENABLE_TOUCH_KEY
#define ENABLE_TOUCH_KEY            0
#endif

#define TOUCH_KEY_SOURCE_ITE        0
#define TOUCH_KEY_SOURCE_SMSC       1
#define TOUCH_KEY_SOURCE_CYPRESS    2
#ifndef TOUCH_KEY_SOURCE
#define TOUCH_KEY_SOURCE            TOUCH_KEY_SOURCE_ITE
#endif

#if ENABLE_TOUCH_KEY && (TOUCH_KEY_SOURCE == TOUCH_KEY_SOURCE_ITE)
#define ENABLE_LOW_CONTACT          1
#else
#define ENABLE_LOW_CONTACT          0
#endif

#ifndef MOBILE_INTERFERENCE
#define MOBILE_INTERFERENCE         0
#endif

#ifndef TOUCH_KEY_CTRL_LED
#define TOUCH_KEY_CTRL_LED          0
#endif

#ifndef TOUCH_KEY_POWER_KEY_DEBOUNCE
#define TOUCH_KEY_POWER_KEY_DEBOUNCE    0
#endif

#define POWER_KEY_DEBOUNCE_PEROID   10 // unit: 50ms
//==========================================================
//==============TOUCH PANEL==================================
#ifndef ENABLE_TOUCH_PANEL
#define ENABLE_TOUCH_PANEL          0
#endif

#ifndef ENABLE_TOUCH_PANEL_DATA_FROM_USB
#define ENABLE_TOUCH_PANEL_DATA_FROM_USB     0
#endif

#ifndef ENABLE_TOUCH_PANEL_CTRL_OSD
#define ENABLE_TOUCH_PANEL_CTRL_OSD     0
#endif
//=============================================================

////////////////////////////////////////////////////////////
// OTHER
////////////////////////////////////////////////////////////
#ifndef ENABLE_DELTAE_FUNCTION
#define ENABLE_DELTAE_FUNCTION      0
#endif

#ifndef ENABLE_ColorMode_FUNCTION
#define ENABLE_ColorMode_FUNCTION       0
#endif

#ifndef ENABLE_DICOM_FUNCTION
#define ENABLE_DICOM_FUNCTION       0
#endif

#ifndef ENABLE_GAMMA_FUNCTION
#define ENABLE_GAMMA_FUNCTION       0
#endif

#ifndef ENABLE_COLORTEMP_FUNCTION
#define ENABLE_COLORTEMP_FUNCTION       0
#endif

#ifndef ENABLE_COLORTRACK_FUNCTION
#define ENABLE_COLORTRACK_FUNCTION       0
#endif

#ifndef ENABLE_DEMURA_FUNCTION
#define ENABLE_DEMURA_FUNCTION      0
#endif

#ifndef PANEL_MINI_LVDS
#define PANEL_MINI_LVDS             0
#endif

#ifndef AdjustVolume_UseTable
#define AdjustVolume_UseTable       0
#endif

#ifndef ENABLE_TIME_MEASUREMENT
#define ENABLE_TIME_MEASUREMENT    0
#endif

// 120119 coding test for TPV
// 0: brightness control by PWM duty
// 1: brightness control by string current and fix PWM duty to max
#ifndef LED_CTRL_BRIGHTNESS_BY_CURRENT
#define LED_CTRL_BRIGHTNESS_BY_CURRENT  0
#endif
#ifndef ENABLE_3DLUT
#define ENABLE_3DLUT                0
#endif

#if (CHIP_ID == MST9U2)
#define VID_THRESHOLD               57      // 11.3771 * 5
#else
#define VID_THRESHOLD               00
#endif

#ifndef FPLL_TUNE_SW_MODE
#define FPLL_TUNE_SW_MODE           0
#endif

#ifndef FPLL_TUNE_LIMIT_SW
#define FPLL_TUNE_LIMIT_SW          20      // +/- 2%
#endif

#ifndef FPLL_TUNE_LIMIT_HW
#define FPLL_TUNE_LIMIT_HW          2500    // +/- 0.04%
#endif

#ifndef FPLL_TUNE_SW_HTT
#define FPLL_TUNE_SW_HTT            0
#endif

#ifndef MST9U_ASIC_1P2SC
#define MST9U_ASIC_1P2SC            0
#endif

#ifndef ENABLE_DEONLY_MODE
#define ENABLE_DEONLY_MODE          1
#endif

#ifndef ENABLE_HV_TOLERANCE_ENLARGE
#define ENABLE_HV_TOLERANCE_ENLARGE 0
#endif

#ifndef ENABLE_FORCE_4K_PMODE
#define ENABLE_FORCE_4K_PMODE       0
#endif

#endif

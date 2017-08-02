#ifndef MST9U_DEMO_H
#define MST9U_DEMO_H

////////////////////////////////////////////////////////////
// CHIP SELECTION
////////////////////////////////////////////////////////////
#define CHIP_ID                 MST9U4
////////////////////////////////////////////////////////////
// BOARD SELECTION
////////////////////////////////////////////////////////////
#define CHIP_FAMILY_TYPE        CHIP_FAMILY_MST9U

////////////////////////////////////////////////////////////
// INPUT_TYPE SELECTION
////////////////////////////////////////////////////////////
#define DVI_USB                 0 // 1: DVI from USB
#define DVI_PN_SWAP             1
#define HDMI_PN_SWAP            1
#if (MS_BOARD_TYPE_SEL == BD_MST9U)
#define INPUT_TYPE              (INPUT_8C)
#define ENABLE_VGA_INPUT        (INPUT_TYPE&INPUT_1A)
#define ENABLE_DVI_INPUT        0
#define ENABLE_DVI2ND_INPUT     0
#define ENABLE_DVI3ND_INPUT     0
#define ENABLE_HDMI_INPUT       1
#define ENABLE_HDMI2ND_INPUT    1
#define ENABLE_HDMI3ND_INPUT    1
#define ENABLE_HDMI4ND_INPUT    1
#define ENABLE_DP_OUTPUT       PANEL_EDP // 1
#define ENABLE_DP_INPUT         1
#define ENABLE_DP2ND_INPUT      1
#define ENABLE_DP3ND_INPUT      1
#define ENABLE_DP4ND_INPUT      0
#define ENABLE_DP5ND_INPUT      0
#define ENABLE_DP6ND_INPUT      0
#define ENABLE_VD_INPUT         0
#define ENABLE_LED_CONTROLLER   0
#define ENABLE_DAISY_CHAIN      0
#define ENABLE_DC_SDM_FORWARD   (0 && ENABLE_DAISY_CHAIN)   // need sync with 51 setting
#else
#warning "please define MST9U4 new board type for Port_Func_Enable"
#endif
#define ENABLE_MULTI_INPUT      (INPUT_TYPE!=INPUT_1A || INPUT_TYPE!=INPUT_1C)
#define ENABLE_DIGITAL_INPUT    (ENABLE_DVI_INPUT || ENABLE_HDMI_INPUT || ENABLE_DP_INPUT)

#define MS_VGA_SOG_EN           1// confirmed
//#define _NEW_SOG_WAKEUP_DET_

////////////////////////////////////////////////////////////
// PANEL SELECTION
////////////////////////////////////////////////////////////
#define ENABLE_VBY1_TO_EDP                  (0)
#ifdef MST9U_FPGA
#define PanelType               PanelAU20SVGA//PanelCMIM236HGJ_L21//PanelAU20SVGA // 800x600
#else
#define PanelType               PanelCMIM236HGJ_L21//PanelSharpUltraHD//
#endif
////////////////////////////////////////////////////////////
// BRIGHTNESS CONTROL
////////////////////////////////////////////////////////////
#define BrightFreqByVfreq               1 // set brightness freq. by input Vfreq.
#define BACKLIGHT_PWM_REVERSE           0
#define BRIGHTNESS_FREQ                 240 // unit: Hz
#define BRIGHTNESS_VSYNC_ALIGN          1

////////////////////////////////////////////////////////////
// HDMI
////////////////////////////////////////////////////////////
#define DHDMI_SIMPLAYHD_PATCH           0 // SimplayHD CTS 8-18

////////////////////////////////////////////////////////////
// AUDIO
////////////////////////////////////////////////////////////
#define AudioFunc                       1
#define ENABLE_SPDIF                    (1 && AudioFunc)
#define ENABLE_I2S                      (1 && AudioFunc)
#define ENABLE_I2S_PassThroughDAC                      (0 && ENABLE_I2S)

#if AudioFunc
#define AUDIDO_ADC_HPF_N                1   // high pass filter N setting
#endif
#define VOLUME_INVERSE                  0
#define VOLUME_FREQ                     30000 // unit: Hz
#define VOLUME_VSYNC_ALIGN              0
#define DAC_IDLE_TONE_IMPROVEMENT       0


////////////////////////////////////////////////////////////
// SYSTEM
////////////////////////////////////////////////////////////
#define AUTO_DATA_PATH                  0
//COLOR function
#define ENABLE_DPS                      1
#define ENABLE_SKIN_TONE_FUNCTION       1
#define ENABLE_SNR_FUNCTION             1
#define ENABLE_STATIC_LUMINANCE_CONTROL 1
#define ENABLE_SUPER_RESOLUTION         1
#define ENABLE_DeBlocking               0
#define ENABLE_DEMURA_FUNCTION          0//MST9U_FPGA temply set to 0 for booting speed up,  1
#define ENABLE_DELTAE_FUNCTION          1
#define ENABLE_ColorMode_FUNCTION       1
#define ENABLE_COLOR_CALIBRATION_FOR_THIRD_PARTY    0
#define ENABLE_SaveMCCSData             0
#define Enable3DLUTColorMode            0
#define ENABLE_DICOM_FUNCTION           0
#define ENABLE_GAMMA_FUNCTION           0
#define ENABLE_COLORTEMP_FUNCTION       0
#define ENABLE_COLORTRACK_FUNCTION      0
#define EABLE_Y_EXTENSION               1
#define ENABLE_PREFER_POST_CONTRAST     0
#define ENABLE_CHECK_RANGE_COLORIMETRY_CHANGE   0
#define tool_model_panel_name  "MST9U4_PanelCMIM236HGJ_L21" //length must smaller than 128

#define FACTORYALIGN_TYPE               FACTORYALIGN_DDCCI
#define ENABLE_POWEROFF_1X_MCU_CLK      0
#define USB_Contral                     0
#if ENABLE_DIGITAL_INPUT
#define ENABLE_HDCP                     1
#define HDCPKEY_IN_Flash                0//1
#else
#define ENABLE_HDCP                     0
#define HDCPKEY_IN_Flash                0
#endif

#define EnablePanelServiceMenu          0// 1
#define ENABLE_R2_USB                   0
#define ENABLE_R2_CODE_ON_DRAM          ENABLE_R2_USB
//#define ENABLE_HK_DATA_ON_DRAM        1
#define ENABLE_RTE                      1
#define ENABLE_OD_MTB                   (0&&ENABLE_RTE)
#define ENABLE_OD_AutoDownTBL           (0&&ENABLE_RTE)
#define ENABLE_TNR                      0
#define ENABLE_TOUCH_PANEL              0
#define ENABLE_TOUCH_PANEL_DATA_FROM_USB     0
#define ENABLE_TOUCH_PANEL_CTRL_OSD     0
#define ENABLE_HDMI_DRR                 0
// Instead of switching EDID, it use MCCS VPC Code to indicate the status of FreeSync support.
// Requirement: The Byte 4 of HDMI FreeSync VSD Block has to be 0xE6, not 0x00.
#define ENABLE_HDMI_DRR_MCCS			(0 && ENABLE_HDMI_DRR)

// 120410 coding test for touch key
//#define ENABLE_TOUCH_KEY    1
//#define TOUCH_KEY_CTRL_LED  1
////////////////////////////////////////////////////////////
// POWERMANAGEMENT
////////////////////////////////////////////////////////////
#define PM_MCUSLEEP                     0
#define LED_PWM                         0

////////////////////////////////////////////////////////////
// SI
////////////////////////////////////////////////////////////
#define ODM_NAME    ODM_WISTRON

////////////////////////////////////////////////////////////
// INFORMATION
////////////////////////////////////////////////////////////
#define ChipName            "MST9U"
#define FWVersion           "000"
#define ChangeDate          "20120612"

#define VERSION_STRING      "MStar_01"
#define MODEL_NAME_STRING   "MStar"
#define MCCS_MODEL_NAME     "MStar"
#define FACTORY_VERSION     "V01"
#define PANEL_TYPE_STRING   "PANEL"
///////////////////////////////////////////////////////////////
// SYSTEM DEBUG
//////////////////////////////////////////////////////////////
//#define ENABLE_MSTV_UART_DEBUG    1 // define in makefile
//#define HDCPKEY_USE_CODE    1

///////////////////////////////////////////////////////////////
#define _MSTAR_OSD_
#define ENABLE_3DLUT                        0//MST9U_FPGA temply set to 0 for booting speed up,  1

#define ENABLE_MULTI_WINDOW_SETTINGS_MENU   1
#define ENABLE_OSD_ROTATION                 1
#define ENABLE_WIDE_MODE                    1
#define OSD_CORNER_INPUT_ICON               0
#define ENABLE_3D_FUNCTION                  0
#define ENABLE_AUDIO_SETTINGS_MENU          (1 && AudioFunc)
#define ENABLE_DCR                          1
#define ENABLE_MOVIE_GAME_PRESET            1
#define SIMPLY_CHINESE                      1
#define ENABLE_ACE                          1
#define ENABLE_OSDC_OSDD                    0
#define ENABLE_OSD_LR                       (!MST9U_ASIC_1P) // enable both L/R OSD
#define ENABLE_DIGITAL_BYPASS_FINDMODE      1
#define YPBPR_PERF_OVERSCAN                 0

#define ENABLE_NOISE_REDUCTION              1
#define ENABLE_DEINTERLACE                  0
#define ENABLE_DP_ShortHPD_PM               0
#define R2_51_DUAL_MODE                     1 // need sync with 51 setting
#define ENABLE_MBX                          0 // need sync with 51 setting
#define ENABLE_USB_HOST                     1
#define ENABLE_USB_HOST_DUAL_IMAGE          (ENABLE_USB_HOST && 0) //need sync with sboot
#define ENABLE_FILESYSTEM                   ENABLE_USB_HOST
#define ENABLE_CP_R2                        0
#define ENABLE_SECU_R2                      0
#define ENABLE_SECU_R2_DBG                  0
#ifdef MST9U_FPGA
#define ENABLE_DEONLY_MODE                  0
#else
#define ENABLE_DEONLY_MODE                  (1 && (!ENABLE_HDMI_DRR))
#endif
#define ENABLE_4K_FBL_MODE                  0 // 4K 1:1 frame buffer less mode
#define ENABLE_DDR_SSC                      0 // Need Sync with Sboot:DDR_SSC_DEVIATION
#define ENABLE_JUMP                         0 // Jump Test
#if (ENABLE_SECU_R2 == 1)
#define ENABLE_HDCP22                       1
#endif
#define ENABLE_COMPILE_INFO                 1 // Enable compiling infomation in BIN file, checksum of code will change every time after compile (Better to sync with SB/PM on the same option)

#define DP_COMBO_PORT                       1         // HBR  Combo0 ~ Combo5
#define DP_ASTRO_INTERLACE_PATCH            1 // 1 == on / 0 == off
#define DP_NoInput_Audio                    0 // 1: no input audio On.

#define ENABLE_FB_BMP_DRAW                  0
#define ENABLE_FB_JPG_DRAW                  0

#define ENABLE_SW_SCALER_COMMON_WRITE       1 // MST9U4 keeps 1 temply. MST9U4_TBD

#define ENABLE_MSPI_FLASH_ACCESS			0
#define PANEL_VBY1_HW_MODE                  (1&&PANEL_VBY1()) //default: HW mode

#ifdef MST9U_FPGA
#define MST9U_FPGA_1P    1
#define MST9U_ASIC_1P    1
#define ENABLE_MENULOAD  0 // MST9U4_TBD, wait driver ready !!
#else
#define MST9U_ASIC_1P    0
#endif

#endif


#ifndef MST9U_DEMO_H
#define MST9U_DEMO_H

////////////////////////////////////////////////////////////
// CHIP SELECTION
////////////////////////////////////////////////////////////
#define CHIP_ID                 MST9U3
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
#if (MS_BOARD_TYPE_SEL == BD_MST203A_A01A_S)
#define INPUT_TYPE              (INPUT_1A|INPUT_4C)
#define ENABLE_VGA_INPUT        (INPUT_TYPE&INPUT_1A)
#define ENABLE_DVI_INPUT        1
#define ENABLE_DVI2ND_INPUT     0
#define ENABLE_DVI3ND_INPUT     0
#define ENABLE_HDMI_INPUT       1
#define ENABLE_HDMI2ND_INPUT    1
#define ENABLE_HDMI3ND_INPUT    0
#define ENABLE_HDMI4ND_INPUT    0
#define ENABLE_DP_OUTPUT       PANEL_EDP // 1
#define ENABLE_DP_INPUT         1
#define ENABLE_DP2ND_INPUT      1
#define ENABLE_DP3ND_INPUT      0
#define ENABLE_DP4ND_INPUT      0
#define ENABLE_DP5ND_INPUT      0
#define ENABLE_DP6ND_INPUT      0
#define ENABLE_VD_INPUT         0
#define ENABLE_LED_CONTROLLER   0
#define ENABLE_DAISY_CHAIN     0 // 1 temp set to 0 ....
#define ENABLE_DC_SDM_FORWARD   (0 && ENABLE_DAISY_CHAIN)   // need sync with 51 setting
#elif (MS_BOARD_TYPE_SEL == BD_MST203A_A01B_S)
#define INPUT_TYPE              (INPUT_1A|INPUT_5C)
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
#define ENABLE_DP3ND_INPUT      0
#define ENABLE_DP4ND_INPUT      0
#define ENABLE_DP5ND_INPUT      0
#define ENABLE_DP6ND_INPUT      0
#define ENABLE_VD_INPUT         0
#define ENABLE_LED_CONTROLLER   0
#define ENABLE_DAISY_CHAIN      1 // 1 temp set to 0 ....
#define ENABLE_DC_SDM_FORWARD   (0 && ENABLE_DAISY_CHAIN)	// need sync with 51 setting
#elif ( (MS_BOARD_TYPE_SEL == BD_MST203B_A01A_S) || (MS_BOARD_TYPE_SEL == BD_MST203C_A01A_S) )
#define INPUT_TYPE              (INPUT_1A|INPUT_3C)
#define ENABLE_VGA_INPUT        (INPUT_TYPE&INPUT_1A)
#define ENABLE_DVI_INPUT        1
#define ENABLE_DVI2ND_INPUT     0
#define ENABLE_DVI3ND_INPUT     0
#define ENABLE_HDMI_INPUT       1
#define ENABLE_HDMI2ND_INPUT    0
#define ENABLE_HDMI3ND_INPUT    0
#define ENABLE_HDMI4ND_INPUT    0
#define ENABLE_DP_OUTPUT       PANEL_EDP // 1
#define ENABLE_DP_INPUT         1
#define ENABLE_DP2ND_INPUT      0
#define ENABLE_DP3ND_INPUT      0
#define ENABLE_DP4ND_INPUT      0
#define ENABLE_DP5ND_INPUT      0
#define ENABLE_DP6ND_INPUT      0
#define ENABLE_VD_INPUT         0
#define ENABLE_LED_CONTROLLER   0
#define ENABLE_DAISY_CHAIN      0
#define ENABLE_DC_SDM_FORWARD   (0 && ENABLE_DAISY_CHAIN)	// need sync with 51 setting

#elif (MS_BOARD_TYPE_SEL == BD_MST9UHL_A0)
#define INPUT_TYPE              (INPUT_1A|INPUT_3C)
#define ENABLE_VGA_INPUT        (INPUT_TYPE&INPUT_1A)
#define ENABLE_DVI_INPUT        1
#define ENABLE_DVI2ND_INPUT     0
#define ENABLE_DVI3ND_INPUT     0
#define ENABLE_HDMI_INPUT       1
#define ENABLE_HDMI2ND_INPUT    0
#define ENABLE_HDMI3ND_INPUT    0
#define ENABLE_HDMI4ND_INPUT    0
#define ENABLE_DP_OUTPUT       PANEL_EDP // 1
#define ENABLE_DP_INPUT         1
#define ENABLE_DP2ND_INPUT      0
#define ENABLE_DP3ND_INPUT      0
#define ENABLE_DP4ND_INPUT      0
#define ENABLE_DP5ND_INPUT      0
#define ENABLE_DP6ND_INPUT      0
#define ENABLE_VD_INPUT         0
#define ENABLE_LED_CONTROLLER   0
#define ENABLE_DAISY_CHAIN      0
#define ENABLE_DC_SDM_FORWARD   (0 && ENABLE_DAISY_CHAIN)	// need sync with 51 setting

#elif (MS_BOARD_TYPE_SEL == BD_MST9UHL_V3)
#define INPUT_TYPE              (INPUT_1A|INPUT_4C)
#define ENABLE_VGA_INPUT        (INPUT_TYPE&INPUT_1A)
#define ENABLE_DVI_INPUT           0
#define ENABLE_DVI2ND_INPUT     0
#define ENABLE_DVI3ND_INPUT     0
#define ENABLE_HDMI_INPUT          1
#define ENABLE_HDMI2ND_INPUT    1
#define ENABLE_HDMI3ND_INPUT    1
#define ENABLE_HDMI4ND_INPUT    0
#define ENABLE_DP_OUTPUT       PANEL_EDP // 1
#define ENABLE_DP_INPUT         1
#define ENABLE_DP2ND_INPUT      0
#define ENABLE_DP3ND_INPUT      0
#define ENABLE_DP4ND_INPUT      0
#define ENABLE_DP5ND_INPUT      0
#define ENABLE_DP6ND_INPUT      0
#define ENABLE_VD_INPUT         0
#define ENABLE_LED_CONTROLLER   0
#define ENABLE_DAISY_CHAIN      0
#define ENABLE_DC_SDM_FORWARD   (0 && ENABLE_DAISY_CHAIN)	// need sync with 51 setting

#elif (MS_BOARD_TYPE_SEL== BD_MST9UHL_V4||MS_BOARD_TYPE_SEL==BD_MST9UHL_V6||MS_BOARD_TYPE_SEL==BD_MST9UHL_V7)
#if AIO_Board
#if SwitchPortByDP_Detect
#define INPUT_TYPE              (INPUT_2C)
#define ENABLE_VGA_INPUT        (INPUT_TYPE&INPUT_1A)
#define ENABLE_DVI_INPUT            0
#define ENABLE_DVI2ND_INPUT     0
#define ENABLE_DVI3ND_INPUT     0
#define ENABLE_HDMI_INPUT          1
#define ENABLE_HDMI2ND_INPUT    0
#define ENABLE_HDMI3ND_INPUT    0
#define ENABLE_HDMI4ND_INPUT    0
#define ENABLE_DP_OUTPUT       PANEL_EDP // 1
#define ENABLE_DP_INPUT            1
#define ENABLE_DP2ND_INPUT      0
#define ENABLE_DP3ND_INPUT      0
#define ENABLE_DP4ND_INPUT      0
#define ENABLE_DP5ND_INPUT      0
#define ENABLE_DP6ND_INPUT      0
#define ENABLE_VD_INPUT         0
#define ENABLE_LED_CONTROLLER   0
#define ENABLE_DAISY_CHAIN      0
#define ENABLE_DC_SDM_FORWARD   (0 && ENABLE_DAISY_CHAIN)	// need sync with 51 setting
#else
#define INPUT_TYPE              (INPUT_1C)
#define ENABLE_VGA_INPUT        (INPUT_TYPE&INPUT_1A)
#define ENABLE_DVI_INPUT            0
#define ENABLE_DVI2ND_INPUT     0
#define ENABLE_DVI3ND_INPUT     0
#define ENABLE_HDMI_INPUT          1
#define ENABLE_HDMI2ND_INPUT    0
#define ENABLE_HDMI3ND_INPUT    0
#define ENABLE_HDMI4ND_INPUT    0
#define ENABLE_DP_OUTPUT       PANEL_EDP // 1
#define ENABLE_DP_INPUT            0
#define ENABLE_DP2ND_INPUT      0
#define ENABLE_DP3ND_INPUT      0
#define ENABLE_DP4ND_INPUT      0
#define ENABLE_DP5ND_INPUT      0
#define ENABLE_DP6ND_INPUT      0
#define ENABLE_VD_INPUT         0
#define ENABLE_LED_CONTROLLER   0
#define ENABLE_DAISY_CHAIN      0
#define ENABLE_DC_SDM_FORWARD   (0 && ENABLE_DAISY_CHAIN)	// need sync with 51 setting
#endif
#else
#define INPUT_TYPE              (INPUT_3C)
#define ENABLE_VGA_INPUT        (INPUT_TYPE&INPUT_1A)
#define ENABLE_DVI_INPUT        1
#define ENABLE_DVI2ND_INPUT     0
#define ENABLE_DVI3ND_INPUT     0
#define ENABLE_HDMI_INPUT       1
#define ENABLE_HDMI2ND_INPUT    0
#define ENABLE_HDMI3ND_INPUT    0
#define ENABLE_HDMI4ND_INPUT    0
#define ENABLE_DP_OUTPUT       PANEL_EDP // 1
#define ENABLE_DP_INPUT         1
#define ENABLE_DP2ND_INPUT      0
#define ENABLE_DP3ND_INPUT      0
#define ENABLE_DP4ND_INPUT      0
#define ENABLE_DP5ND_INPUT      0
#define ENABLE_DP6ND_INPUT      0
#define ENABLE_VD_INPUT         0
#define ENABLE_LED_CONTROLLER   0
#define ENABLE_DAISY_CHAIN      0
#define ENABLE_DC_SDM_FORWARD   (0 && ENABLE_DAISY_CHAIN)	// need sync with 51 setting
#endif

#elif (MS_BOARD_TYPE_SEL==BD_MST9UHL_V4_1H1DP)
#define INPUT_TYPE              (INPUT_2C)
#define ENABLE_VGA_INPUT        (INPUT_TYPE&INPUT_1A)
#define ENABLE_DVI_INPUT            0
#define ENABLE_DVI2ND_INPUT     0
#define ENABLE_DVI3ND_INPUT     0
#define ENABLE_HDMI_INPUT          1
#define ENABLE_HDMI2ND_INPUT    0
#define ENABLE_HDMI3ND_INPUT    0
#define ENABLE_HDMI4ND_INPUT    0
#define ENABLE_DP_OUTPUT       PANEL_EDP // 1
#define ENABLE_DP_INPUT            1
#define ENABLE_DP2ND_INPUT      0
#define ENABLE_DP3ND_INPUT      0
#define ENABLE_DP4ND_INPUT      0
#define ENABLE_DP5ND_INPUT      0
#define ENABLE_DP6ND_INPUT      0
#define ENABLE_VD_INPUT         0
#define ENABLE_LED_CONTROLLER   0
#define ENABLE_DAISY_CHAIN      0
#define ENABLE_DC_SDM_FORWARD   (0 && ENABLE_DAISY_CHAIN)	// need sync with 51 setting

#elif (MS_BOARD_TYPE_SEL == BD_MST9UHD_BV1)
#define INPUT_TYPE              (INPUT_4C)
#define ENABLE_VGA_INPUT        (INPUT_TYPE&INPUT_1A)
#define ENABLE_DVI_INPUT           0
#define ENABLE_DVI2ND_INPUT     0
#define ENABLE_DVI3ND_INPUT     0
#define ENABLE_HDMI_INPUT          1
#define ENABLE_HDMI2ND_INPUT    1
#define ENABLE_HDMI3ND_INPUT    1
#define ENABLE_HDMI4ND_INPUT    0
#define ENABLE_DP_OUTPUT       PANEL_EDP // 1
#define ENABLE_DP_INPUT         1
#define ENABLE_DP2ND_INPUT      0
#define ENABLE_DP3ND_INPUT      0
#define ENABLE_DP4ND_INPUT      0
#define ENABLE_DP5ND_INPUT      0
#define ENABLE_DP6ND_INPUT      0
#define ENABLE_VD_INPUT         0
#define ENABLE_LED_CONTROLLER   0
#define ENABLE_DAISY_CHAIN      0
#define ENABLE_DC_SDM_FORWARD   (0 && ENABLE_DAISY_CHAIN)	// need sync with 51 setting
#elif (MS_BOARD_TYPE_SEL==BD_MST9UHD_DV1)
#define INPUT_TYPE              (INPUT_4C)
#define ENABLE_VGA_INPUT        (INPUT_TYPE&INPUT_1A)
#define ENABLE_DVI_INPUT           1
#define ENABLE_DVI2ND_INPUT     0
#define ENABLE_DVI3ND_INPUT     0
#define ENABLE_HDMI_INPUT          1
#define ENABLE_HDMI2ND_INPUT    1
#define ENABLE_HDMI3ND_INPUT    0
#define ENABLE_HDMI4ND_INPUT    0
#define ENABLE_DP_OUTPUT       PANEL_EDP // 1
#define ENABLE_DP_INPUT         1
#define ENABLE_DP2ND_INPUT      0
#define ENABLE_DP3ND_INPUT      0
#define ENABLE_DP4ND_INPUT      0
#define ENABLE_DP5ND_INPUT      0
#define ENABLE_DP6ND_INPUT      0
#define ENABLE_VD_INPUT         0
#define ENABLE_LED_CONTROLLER   0
#define ENABLE_DAISY_CHAIN      0
#define ENABLE_DC_SDM_FORWARD   (0 && ENABLE_DAISY_CHAIN)	// need sync with 51 setting




#else
#warning "please define MST9U new board type for Port_Func_Enable"
#endif
#define ENABLE_MULTI_INPUT      (INPUT_TYPE!=INPUT_1A || INPUT_TYPE!=INPUT_1C)
#define ENABLE_DIGITAL_INPUT    (ENABLE_DVI_INPUT || ENABLE_HDMI_INPUT || ENABLE_DP_INPUT)

#define MS_VGA_SOG_EN           0// confirmed
//#define _NEW_SOG_WAKEUP_DET_

////////////////////////////////////////////////////////////
// PANEL SELECTION
////////////////////////////////////////////////////////////
#include "Panel_define.h"

#define ENABLE_VBY1_TO_EDP					((CHIP_ID==MST9U2)&&0)
#ifdef MST9U_FPGA
#define PanelType               PanelAU20SVGA//PanelCMIM236HGJ_L21//PanelAU20SVGA // 800x600
#else
#define PanelType               PanelM270QAN01//PanelCMI28UltraHD//PanelLSM315HP01//PanelM270QAN01//PanelCMI28UltraHD//PanelLSM315HP01//PanelT430QVN01//PanelCMIM236HGJ_L21//PanelM320QAN01//PanelSharpUltraHD//
#endif
////////////////////////////////////////////////////////////
// BRIGHTNESS CONTROL
////////////////////////////////////////////////////////////
#define BrightFreqByVfreq               0 // set brightness freq. by input Vfreq.

#define BRIGHTNESS_FREQ                 240 // unit: Hz
#define BRIGHTNESS_VSYNC_ALIGN          1

////////////////////////////////////////////////////////////
// HDMI
////////////////////////////////////////////////////////////
#define DHDMI_SIMPLAYHD_PATCH           0 // SimplayHD CTS 8-18

////////////////////////////////////////////////////////////
// AUDIO
////////////////////////////////////////////////////////////
#if (defined(HuiGe_Project))
    #define AudioFunc                                                       New_Key_HuiGe
#elif (defined(LianHeChuangXin_Project))
    #define AudioFunc                                                       (ModeName!=M3CDG)
#elif ((defined(SenYo_Project)&&(MS_BOARD_TYPE_SEL == BD_MST9UHL_V3))||(defined(RunFeng_Project)))
    #define AudioFunc                                                       0
#else
    #define AudioFunc                                                       1
#endif
#define ENABLE_5707                                                 ((MS_BOARD_TYPE_SEL!=BD_MST9UHL_V6)&&(MS_BOARD_TYPE_SEL!=BD_MST9UHL_V7)&&(MS_BOARD_TYPE_SEL!=BD_MST9UHD_DV1)&&(MS_BOARD_TYPE_SEL!=BD_MST9UHD_GV1)&& AudioFunc)
#define ENABLE_SPDIF                                               (0 && AudioFunc)
#define ENABLE_I2S                                                    (1 && AudioFunc&&ENABLE_5707)
#define ENABLE_I2S_PassThroughDAC                      (1&& ENABLE_I2S)

#define Audio_Driver_Use_5707_Need_To_Check         (ENABLE_5707||(MS_BOARD_TYPE_SEL==BD_MST9UHL_A0)||(MS_BOARD_TYPE_SEL==BD_MST9UHL_V3)||(MS_BOARD_TYPE_SEL==BD_MST9UHL_V4)||(MS_BOARD_TYPE_SEL==BD_MST9UHD_BV1))

#if AudioFunc
#define AUDIDO_ADC_HPF_N                1   // high pass filter N setting
#endif
#define VOLUME_INVERSE                  0
#define VOLUME_FREQ                     30000 // unit: Hz
#define VOLUME_VSYNC_ALIGN              0
#define DAC_IDLE_TONE_IMPROVEMENT       0

#define AudioNewCurve                          1

#define Audio_Input_Jack_Only_For_Line_In         0//(AudioFunc&&(/*(MS_BOARD_TYPE_SEL == BD_MST9UHL_A0)||*/((MS_BOARD_TYPE_SEL== BD_MST9UHL_V4)||(MS_BOARD_TYPE_SEL==BD_MST9UHL_V6)||(MS_BOARD_TYPE_SEL==BD_MST9UHL_V7))&&(!defined(XiaBao_Project))&&(!defined(K3C_Project))&&(!defined(K3L_LED_Project))&&(!defined(K3L_CrossOver_Project))&&(!defined(XinZuoEr_Project))&&(!defined(K3D_FHD144_Project))||(MS_BOARD_TYPE_SEL==BD_MST9UHL_V4_1H1DP)))//&&(defined(JieBo_Project)||defined(FengYuan_Project)||defined(Korean_Project)||defined(SanSe_Project)||defined(RX_Project)||defined(HuiGe_Project))))
#define Audio_Input_Jack_Only_For_Output           (defined(LianHeChuangXin_Project))// 1//(AudioFunc&&((defined(K3C_Project))||(defined(K3L_LED_Project))||(defined(K3L_CrossOver_Project))||(defined(XinZuoEr_Project)&&(MS_BOARD_TYPE_SEL != BD_MST9UHL_A0))))

#define ENABLE_ADJ_AUDIO_OUTPUT                      (AudioFunc&&(defined(KeOuShi_Project)||defined(MingCai_Project)||(defined(KTC_Project))||(defined(ForZhouYang315HP_Project))||((MS_BOARD_TYPE_SEL == BD_MST9UHL_V3)&&(!Audio_Input_Jack_Only_For_Line_In)&&(!Audio_Input_Jack_Only_For_Output)&&((defined(LeXingEnMa_Project))||(defined(FengYuan_Project))|(defined(XuLi_Project))))))

#define ENABLE_ADJ_AUDIO_OUTPUT_OSD                (ENABLE_ADJ_AUDIO_OUTPUT&&(!(defined(KTC_Project)&&PanelType==PanelLSM315HP01)))




////////////////////////////////////////////////////////////
// SYSTEM
////////////////////////////////////////////////////////////
#define AUTO_DATA_PATH                  0
//COLOR function
#define ENABLE_DPS                                  0
#define ENABLE_SKIN_TONE_FUNCTION       0
#define ENABLE_SNR_FUNCTION                   0
#define ENABLE_STATIC_LUMINANCE_CONTROL 0
#define ENABLE_DeBlocking               0
#define ENABLE_DELTAE_FUNCTION          1
#define ENABLE_ColorMode_FUNCTION       1
#define ENABLE_COLOR_CALIBRATION_FOR_THIRD_PARTY    0
#define ENABLE_SaveMCCSData             0
#define Enable3DLUTColorMode                0
#define ENABLE_DICOM_FUNCTION           0
#define ENABLE_GAMMA_FUNCTION           0
#define ENABLE_COLORTEMP_FUNCTION       0
#define ENABLE_COLORTRACK_FUNCTION      0
#define EABLE_Y_EXTENSION               1
#define ENABLE_PREFER_POST_CONTRAST     0
#define ENABLE_CHECK_RANGE_COLORIMETRY_CHANGE   0//(defined(LianHeChuangXin_Project))
#define tool_model_panel_name  "MST9U3_PanelCMIM236HGJ_L21" //length must smaller than 128

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
#define ENABLE_HDMI_DRR                  (Free_Sync_Enable)
// Instead of switching EDID, it use MCCS VPC Code to indicate the status of FreeSync support.
// Requirement: The Byte 4 of HDMI FreeSync VSD Block has to be 0xE6, not 0x00.
#define ENABLE_HDMI_DRR_MCCS			(1 && ENABLE_HDMI_DRR)

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
#define ODM_NAME    ODM_NUUSE

#define GOP                                  0
////////////////////////////////////////////////////////////
// INFORMATION
////////////////////////////////////////////////////////////
#if (defined(HuiGe_Project))
#define FWVersion           "FW\x02F 206"
#elif (defined(KunDe_Project))
#define FWVersion           "FW\x02F 003"
#elif (defined(FengYuan_Project))
#define VERSION               "V016"
#define FWVersion           "FW\x02F 016"
#elif (defined(LianHeChuangXin_Project))
#if (ModeName==K3D6G)
#define FWVersion           "FW\x02F M004"
#elif (ModeName==M3CDG)
#define FWVersion           "FW\x02F E007"
#else //V3K1M
#define FWVersion           "FW\x02F S001"
#endif
#elif (defined(RunFeng_Project))
#define FWVersion           "FW\x02F 009  A"
#elif (defined(HongTian_Project))
#define FWVersion           "FW\x02F 009"
#elif (defined(XuLi_Project))
#define VERSION               "V017"
#define FWVersion           "FW\x02F 017"
#elif (defined(MingCai_Project))
#define VERSION               "V018"
#define FWVersion           "FW\x02F 018"
#elif (defined(ZhongXinKeJi_Project))
#define FWVersion           "FW\x02F D001"
#else
#define FWVersion           "FW\x02F 006"
#endif

#define ChipName            "MST9U"
#define ChangeDate          "20170626"

#define VERSION_STRING      "JRY"
#define MCCS_MODEL_NAME     "JRY"

///////////////////////////////////////////////////////////////
// SYSTEM DEBUG
//////////////////////////////////////////////////////////////
//#define ENABLE_MSTV_UART_DEBUG    1 // define in makefile
//#define HDCPKEY_USE_CODE    1

///////////////////////////////////////////////////////////////
#define _MSTAR_OSD_

#define ENABLE_3DLUT                      1

#define ENABLE_MULTI_WINDOW_SETTINGS_MENU    0//((!AIO_Board)&&(!(defined(KTC_Project)&&(PanelType ==PanelLSM315HP01 )))&&(!defined(ForZhouYang315HP_Project)))

#define ENABLE_ShowInformation_SETTINGS_MENU    (1&&(defined(KTC_Project)&&PanelType==PanelLSM315HP01))

#define ENABLE_OSD_ROTATION                 1
#define _OSD_ROTATION_180_
#define _OSD_ROTATION_270_

#define ENABLE_WIDE_MODE                    1
#define OSD_CORNER_INPUT_ICON               0
#define ENABLE_3D_FUNCTION                  0
#define ENABLE_AUDIO_SETTINGS_MENU         (((defined(ZhongXinKeJi_Project))/*||(defined(LianHeChuangXin_Project))*/) && AudioFunc)
#define ENABLE_DCR                          1
#define ENABLE_MOVIE_GAME_PRESET            1
#define SIMPLY_CHINESE                      0
#define ENABLE_ACE                          1
#define ENABLE_OSDC_OSDD                    0
#define ENABLE_OSD_LR                       (!MST9U_ASIC_1P) // enable both L/R OSD
#define ENABLE_OSD_LR_DOUBLE                    ((defined(HuaKe_Project))||(defined(TaiWan_Project))||(defined(Changjia_Project))||(defined(ZhongXinKeJi_Project)))
#define ENABLE_DIGITAL_BYPASS_FINDMODE      1
#define YPBPR_PERF_OVERSCAN                 0

#define ENABLE_NOISE_REDUCTION              (1&&(!defined(KTC_Project)))
#define ENABLE_DEINTERLACE                  0
#define ENABLE_DP_ShortHPD_PM		   0

#define ENABLE_FACTORY_SSCADJ           1


#define R2_51_DUAL_MODE                     1 // need sync with 51 setting
#define ENABLE_MBX                                0 // need sync with 51 setting
#define ENABLE_USB_HOST                     ((defined(LeXingEnMa_Project))||(defined(XingYuan_Project))||(defined(SenYo_Project))||((defined(FengYuan_Project))&&(!(defined(USA_Project)))&&(!(defined(Japan_Project))))||(defined(JieBo_BangZi_Project))||(defined(XinZuoEr_Project))||(defined(ZhouYang_Project))||(defined(Changjia_Project))||defined(MingCai_Project)||(defined(XuLi_Project))||(defined(Aigao_Project))||(defined(KTC_Project))||(defined(KeOuShi_Project)))
#define ENABLE_USB_HOST_DUAL_IMAGE          (ENABLE_USB_HOST && 0) //need sync with sboot
#define ENABLE_FILESYSTEM                   ENABLE_USB_HOST
#define ENABLE_CP_R2                               0
#define ENABLE_SECU_R2                           ((PanelType==PanelM270QAN01)||(PanelType==PanelM350DVR01_01))
#define ENABLE_SECU_R2_DBG                  0
#define ENABLE_DEONLY_MODE                  0//(1 && (!ENABLE_HDMI_DRR))
#define ENABLE_4K_FBL_MODE                  0 // 4K 1:1 frame buffer less mode
#define ENABLE_DDR_SSC                      0 // Need Sync with Sboot:DDR_SSC_DEVIATION
#define ENABLE_JUMP                         0 // Jump Test
#if (ENABLE_SECU_R2 == 1)
#define ENABLE_HDCP22				    1
#else
#define ENABLE_HDCP22				    0
#endif
#define ENABLE_COMPILE_INFO                 1 // Enable compiling infomation in BIN file, checksum of code will change every time after compile (Better to sync with SB/PM on the same option)

#define DP_COMBO_PORT                       1 // HBR  Combo0 ~ Combo5
#define DP_ASTRO_INTERLACE_PATCH            1 // 1 == on / 0 == off
#define DP_NoInput_Audio                    0 // 1: no input audio On.

#define ENABLE_FB_BMP_DRAW                  0
#define ENABLE_FB_JPG_DRAW					0

#ifdef MST9U_FPGA
#define MST9U_FPGA_1P    1
#endif
    #if (PanelType==PanelM315DVR011||PanelType==PanelLSM315DP02)
    #define MST9U_ASIC_1P    1
    #else
    #define MST9U_ASIC_1P    0
    #endif
#endif


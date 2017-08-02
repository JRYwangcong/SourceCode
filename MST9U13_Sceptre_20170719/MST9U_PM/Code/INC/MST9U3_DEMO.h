#ifndef MST9U_DEMO_H
#define MST9U_DEMO_H

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////


//#define LeXingEnMa_Project                    

//#define ZhiFei_Project                       

//#define ZDCC_Project

//#define HuiGe_Project

//#define SanSe_Project

//#define KunDe_Project

//#define JieBo_Project

//#define FengYuan_Project

//#define Korean_Project

//#define RX_Project

//#define KTC_Project

//#define WanMei_Project

//#define HaiQi_Project

//#define ZhouYang_Project

//#define LanChuangKeJi_Project

//#define ChongQing_Project

//#define WoPai_Project 

//#define SenYo_Project

//#define JieBo_BangZi_Project

//#define KeTe_Project

//#define HuaKe_Project

//#define HKC_Project

//#define TaiBei_Project

//#define SenPaiTe_Project

//#define YiHongGaoQing_Project

//#define LianHeChuangXin_Project

//#define RunFeng_Project

//#define XinZuoEr_Project

//#define Kogan_Project

//#define CaiTeng_Project

//#define XingYuan_Project

//#define WeiGuan_Project

//#define WeiZhen_Project

//#define SuChang_Project

//#define MengPai_Project

//#define TianJiDisplay_Project

//#define JiSu_Project

//#define ZhongChuangLianHe_Project

//#define DaYu_Project

//#define HongHan_Project

//#define GuangZhou_Project

//#define Changjia_Project

//#define DongShanJingMi_Project

//#define  HongTian_Project

//#define XuLi_Project

//#define MingCai_Project

//#define Honghan_Project

#define Sceptre_Project

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// BOARD SELECTION
////////////////////////////////////////////////////////////
#define CHIP_FAMILY_TYPE    CHIP_FAMILY_MST9U
#if ((defined(HKC_Project))||(defined(DaYu_Project))||(defined(HongTian_Project))||(defined(HongHan_Project)))
#define MS_BOARD_TYPE_SEL   BD_MST9UHL_V4
#elif (defined(LeXingEnMa_Project)||defined(FengYuan_Project)||defined(MingCai_Project)||defined(ZhouYang_Project)||defined(KunDe_Project)||defined(Changjia_Project)||(defined(XuLi_Project)))
#define MS_BOARD_TYPE_SEL   BD_MST9UHL_V3
#elif (defined(LianHeChuangXin_Project))
//#define MS_BOARD_TYPE_SEL   BD_MST9UHL_V4
//#define MS_BOARD_TYPE_SEL   BD_MST9UHL_V4_1H1DP
//#define MS_BOARD_TYPE_SEL   BD_MST9UHL_V4
//#define MS_BOARD_TYPE_SEL   BD_MST9UHD_BV1
#elif (defined(Sceptre_Project))
//#define MS_BOARD_TYPE_SEL   BD_MST9UHL_V7
#define MS_BOARD_TYPE_SEL   BD_MST9UHL_A0//BD_MST9UHD_DV1
//#define MS_BOARD_TYPE_SEL   BD_MST9UHD_GV1

#else
#define MS_BOARD_TYPE_SEL   BD_MST9UHL_A0
#endif


#define AIO_Board_PM                         0//(defined(LianHeChuangXin_Project))

#define SwitchPortByDP_Detect_PM     (0&&(AIO_Board_PM))                

//#define FPGA   //20140218 enable this define to verify 51 with FPGA system
////////////////////////////////////////////////////////////
// INPUT_TYPE SELECTION
////////////////////////////////////////////////////////////
#if MS_BOARD_TYPE_SEL == BD_MST203A_A01A_S
#define INPUT_TYPE              (INPUT_1A|INPUT_4C)
#define ENABLE_VGA_INPUT        (INPUT_TYPE&INPUT_1A)
#define ENABLE_DVI_INPUT        1
#define ENABLE_DVI2ND_INPUT     0
#define ENABLE_DVI3ND_INPUT     0
#define ENABLE_HDMI_INPUT       1
#define ENABLE_HDMI2ND_INPUT    1
#define ENABLE_HDMI3ND_INPUT    0
#define ENABLE_MHL_C1           0// 1
#define ENABLE_MHL_C2           0// 1
#define ENABLE_MHL_C3           0// 1
#define ENABLE_MHL              1 //  (ENABLE_MHL_C1|ENABLE_MHL_C2|ENABLE_MHL_C3)
#define ENABLE_DP_OUTPUT        PANEL_EDP // 1
#define ENABLE_DP_INPUT         1
#define ENABLE_DP2ND_INPUT      1
#define ENABLE_DP3ND_INPUT      0
#define ENABLE_VD_INPUT         0
#define ENABLE_LED_CONTROLLER   0
#define ENABLE_DAISY_CHAIN      1 // Sync with Epona Power Saving Mode ....
#define ENABLE_DC_SDM_FORWARD   (0 && ENABLE_DAISY_CHAIN)	// need sync with R2 setting
#define DISABLE_AUTO_SWITCH     0
#elif MS_BOARD_TYPE_SEL == BD_MST203A_A01B_S
#define INPUT_TYPE              (INPUT_1A|INPUT_5C)
#define ENABLE_VGA_INPUT        (INPUT_TYPE&INPUT_1A)
#define ENABLE_DVI_INPUT        0
#define ENABLE_DVI2ND_INPUT     0
#define ENABLE_DVI3ND_INPUT     0
#define ENABLE_HDMI_INPUT       1
#define ENABLE_HDMI2ND_INPUT    1
#define ENABLE_HDMI3ND_INPUT    1
#define ENABLE_HDMI4ND_INPUT    1
#define ENABLE_MHL_C1           0// 1
#define ENABLE_MHL_C2           0// 1
#define ENABLE_MHL_C3           0// 1
#define ENABLE_MHL              1 //  (ENABLE_MHL_C1|ENABLE_MHL_C2|ENABLE_MHL_C3)
#define ENABLE_DP_OUTPUT        PANEL_EDP // 1
#define ENABLE_DP_INPUT         1
#define ENABLE_DP2ND_INPUT      1
#define ENABLE_DP3ND_INPUT      0
#define ENABLE_VD_INPUT         0
#define ENABLE_LED_CONTROLLER   0
#define ENABLE_DAISY_CHAIN      1 // Sync with Epona Power Saving Mode ....
#define ENABLE_DC_SDM_FORWARD	(0 && ENABLE_DAISY_CHAIN)	// need sync with R2 setting
#define DISABLE_AUTO_SWITCH     0
#elif   ((MS_BOARD_TYPE_SEL == BD_MST203B_A01A_S) || (MS_BOARD_TYPE_SEL == BD_MST203C_A01A_S))
#define INPUT_TYPE              (INPUT_1A|INPUT_3C)
#define ENABLE_VGA_INPUT        (INPUT_TYPE&INPUT_1A)
#define ENABLE_DVI_INPUT        1
#define ENABLE_DVI2ND_INPUT     0
#define ENABLE_DVI3ND_INPUT     0
#define ENABLE_HDMI_INPUT       1
#define ENABLE_HDMI2ND_INPUT    0
#define ENABLE_HDMI3ND_INPUT    0
#define ENABLE_MHL_C1           0// 1
#define ENABLE_MHL_C2           0// 1
#define ENABLE_MHL_C3           0// 1
#define ENABLE_MHL              1 //  (ENABLE_MHL_C1|ENABLE_MHL_C2|ENABLE_MHL_C3)
#define ENABLE_DP_OUTPUT        PANEL_EDP // 1
#define ENABLE_DP_INPUT         1
#define ENABLE_DP2ND_INPUT      0
#define ENABLE_DP3ND_INPUT      0
#define ENABLE_VD_INPUT         0
#define ENABLE_LED_CONTROLLER   0
#define ENABLE_DAISY_CHAIN      0 // 1 temp set to 0 ....
#define ENABLE_DC_SDM_FORWARD   (0 && ENABLE_DAISY_CHAIN)	// need sync with R2 setting
#define DISABLE_AUTO_SWITCH     0
#elif (MS_BOARD_TYPE_SEL == BD_MST9UHL_A0)
#define INPUT_TYPE              (INPUT_1A|INPUT_3C)
#define ENABLE_VGA_INPUT        (INPUT_TYPE&INPUT_1A)
#define ENABLE_DVI_INPUT        1
#define ENABLE_DVI2ND_INPUT     0
#define ENABLE_DVI3ND_INPUT     0
#define ENABLE_HDMI_INPUT       1
#define ENABLE_HDMI2ND_INPUT    0
#define ENABLE_HDMI3ND_INPUT    0
#define ENABLE_MHL_C1           0// 1
#define ENABLE_MHL_C2           0// 1
#define ENABLE_MHL_C3           0// 1
#define ENABLE_MHL              1 //  (ENABLE_MHL_C1|ENABLE_MHL_C2|ENABLE_MHL_C3)
#define ENABLE_DP_OUTPUT        PANEL_EDP // 1
#define ENABLE_DP_INPUT         1
#define ENABLE_DP2ND_INPUT      0
#define ENABLE_DP3ND_INPUT      0
#define ENABLE_VD_INPUT         0
#define ENABLE_LED_CONTROLLER   0
#define ENABLE_DAISY_CHAIN      0 // 1 temp set to 0 ....
#define ENABLE_DC_SDM_FORWARD   (0 && ENABLE_DAISY_CHAIN)	// need sync with R2 setting
#define DISABLE_AUTO_SWITCH     0

#elif (MS_BOARD_TYPE_SEL == BD_MST9UHL_V3)
#define INPUT_TYPE              (INPUT_1A|INPUT_4C)
#define ENABLE_VGA_INPUT        (INPUT_TYPE&INPUT_1A)
#define ENABLE_DVI_INPUT           0
#define ENABLE_DVI2ND_INPUT     0
#define ENABLE_DVI3ND_INPUT     0
#define ENABLE_HDMI_INPUT          1
#define ENABLE_HDMI2ND_INPUT    1
#define ENABLE_HDMI3ND_INPUT    1
#define ENABLE_MHL_C1           0// 1
#define ENABLE_MHL_C2           0// 1
#define ENABLE_MHL_C3           0// 1
#define ENABLE_MHL              1 //  (ENABLE_MHL_C1|ENABLE_MHL_C2|ENABLE_MHL_C3)
#define ENABLE_DP_OUTPUT        PANEL_EDP // 1
#define ENABLE_DP_INPUT         1
#define ENABLE_DP2ND_INPUT      0
#define ENABLE_DP3ND_INPUT      0
#define ENABLE_VD_INPUT         0
#define ENABLE_LED_CONTROLLER   0
#define ENABLE_DAISY_CHAIN      0 // 1 temp set to 0 ....
#define ENABLE_DC_SDM_FORWARD   (0 && ENABLE_DAISY_CHAIN)	// need sync with R2 setting
#define DISABLE_AUTO_SWITCH     0

#elif (MS_BOARD_TYPE_SEL== BD_MST9UHL_V4||MS_BOARD_TYPE_SEL==BD_MST9UHL_V6||MS_BOARD_TYPE_SEL==BD_MST9UHL_V7)
#if AIO_Board_PM
#if SwitchPortByDP_Detect_PM
#define INPUT_TYPE              (INPUT_2C)
#define ENABLE_VGA_INPUT        (INPUT_TYPE&INPUT_1A)
#define ENABLE_DVI_INPUT           0
#define ENABLE_DVI2ND_INPUT     0
#define ENABLE_DVI3ND_INPUT     0
#define ENABLE_HDMI_INPUT         1
#define ENABLE_HDMI2ND_INPUT    0
#define ENABLE_HDMI3ND_INPUT    0
#define ENABLE_MHL_C1           0// 1
#define ENABLE_MHL_C2           0// 1
#define ENABLE_MHL_C3           0// 1
#define ENABLE_MHL              1 //  (ENABLE_MHL_C1|ENABLE_MHL_C2|ENABLE_MHL_C3)
#define ENABLE_DP_OUTPUT        PANEL_EDP // 1
#define ENABLE_DP_INPUT            1
#define ENABLE_DP2ND_INPUT      0
#define ENABLE_DP3ND_INPUT      0
#define ENABLE_VD_INPUT            0
#define ENABLE_LED_CONTROLLER   0
#define ENABLE_DAISY_CHAIN      0 // 1 temp set to 0 ....
#define ENABLE_DC_SDM_FORWARD   (0 && ENABLE_DAISY_CHAIN)	// need sync with R2 setting
#define DISABLE_AUTO_SWITCH     0
#else
#define INPUT_TYPE              (INPUT_1C)
#define ENABLE_VGA_INPUT        (INPUT_TYPE&INPUT_1A)
#define ENABLE_DVI_INPUT           0
#define ENABLE_DVI2ND_INPUT     0
#define ENABLE_DVI3ND_INPUT     0
#define ENABLE_HDMI_INPUT         1
#define ENABLE_HDMI2ND_INPUT    0
#define ENABLE_HDMI3ND_INPUT    0
#define ENABLE_MHL_C1           0// 1
#define ENABLE_MHL_C2           0// 1
#define ENABLE_MHL_C3           0// 1
#define ENABLE_MHL              1 //  (ENABLE_MHL_C1|ENABLE_MHL_C2|ENABLE_MHL_C3)
#define ENABLE_DP_OUTPUT        PANEL_EDP // 1
#define ENABLE_DP_INPUT            0
#define ENABLE_DP2ND_INPUT      0
#define ENABLE_DP3ND_INPUT      0
#define ENABLE_VD_INPUT            0
#define ENABLE_LED_CONTROLLER   0
#define ENABLE_DAISY_CHAIN      0 // 1 temp set to 0 ....
#define ENABLE_DC_SDM_FORWARD   (0 && ENABLE_DAISY_CHAIN)	// need sync with R2 setting
#define DISABLE_AUTO_SWITCH     0
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
#define ENABLE_MHL_C1           0// 1
#define ENABLE_MHL_C2           0// 1
#define ENABLE_MHL_C3           0// 1
#define ENABLE_MHL              0 //  (ENABLE_MHL_C1|ENABLE_MHL_C2|ENABLE_MHL_C3)
#define ENABLE_DP_OUTPUT        PANEL_EDP // 1
#define ENABLE_DP_INPUT         1
#define ENABLE_DP2ND_INPUT      0
#define ENABLE_DP3ND_INPUT      0
#define ENABLE_VD_INPUT         0
#define ENABLE_LED_CONTROLLER   0
#define ENABLE_DAISY_CHAIN      0 // 1 temp set to 0 ....
#define ENABLE_DC_SDM_FORWARD   (0 && ENABLE_DAISY_CHAIN)	// need sync with R2 setting
#define DISABLE_AUTO_SWITCH     0
#endif

#elif (MS_BOARD_TYPE_SEL==BD_MST9UHL_V4_1H1DP)
#define INPUT_TYPE              (INPUT_2C)
#define ENABLE_VGA_INPUT        (INPUT_TYPE&INPUT_1A)
#define ENABLE_DVI_INPUT           0
#define ENABLE_DVI2ND_INPUT     0
#define ENABLE_DVI3ND_INPUT     0
#define ENABLE_HDMI_INPUT         1
#define ENABLE_HDMI2ND_INPUT    0
#define ENABLE_HDMI3ND_INPUT    0
#define ENABLE_MHL_C1           0// 1
#define ENABLE_MHL_C2           0// 1
#define ENABLE_MHL_C3           0// 1
#define ENABLE_MHL              1 //  (ENABLE_MHL_C1|ENABLE_MHL_C2|ENABLE_MHL_C3)
#define ENABLE_DP_OUTPUT        PANEL_EDP // 1
#define ENABLE_DP_INPUT            1
#define ENABLE_DP2ND_INPUT      0
#define ENABLE_DP3ND_INPUT      0
#define ENABLE_VD_INPUT            0
#define ENABLE_LED_CONTROLLER   0
#define ENABLE_DAISY_CHAIN      0 // 1 temp set to 0 ....
#define ENABLE_DC_SDM_FORWARD   (0 && ENABLE_DAISY_CHAIN)	// need sync with R2 setting
#define DISABLE_AUTO_SWITCH     0

#elif (MS_BOARD_TYPE_SEL == BD_MST9UHD_BV1)
#define INPUT_TYPE              (INPUT_4C)
#define ENABLE_VGA_INPUT        (INPUT_TYPE&INPUT_1A)
#define ENABLE_DVI_INPUT           0
#define ENABLE_DVI2ND_INPUT     0
#define ENABLE_DVI3ND_INPUT     0
#define ENABLE_HDMI_INPUT          1
#define ENABLE_HDMI2ND_INPUT    1
#define ENABLE_HDMI3ND_INPUT    1
#define ENABLE_MHL_C1           0// 1
#define ENABLE_MHL_C2           0// 1
#define ENABLE_MHL_C3           0// 1
#define ENABLE_MHL              1 //  (ENABLE_MHL_C1|ENABLE_MHL_C2|ENABLE_MHL_C3)
#define ENABLE_DP_OUTPUT        PANEL_EDP // 1
#define ENABLE_DP_INPUT         1
#define ENABLE_DP2ND_INPUT      0
#define ENABLE_DP3ND_INPUT      0
#define ENABLE_VD_INPUT         0
#define ENABLE_LED_CONTROLLER   0
#define ENABLE_DAISY_CHAIN      0 // 1 temp set to 0 ....
#define ENABLE_DC_SDM_FORWARD   (0 && ENABLE_DAISY_CHAIN)	// need sync with R2 setting
#define DISABLE_AUTO_SWITCH     0
#elif (MS_BOARD_TYPE_SEL==BD_MST9UHD_DV1)
#define INPUT_TYPE              (INPUT_4C)
#define ENABLE_VGA_INPUT        (INPUT_TYPE&INPUT_1A)
#define ENABLE_DVI_INPUT           1
#define ENABLE_DVI2ND_INPUT     0
#define ENABLE_DVI3ND_INPUT     0
#define ENABLE_HDMI_INPUT          1
#define ENABLE_HDMI2ND_INPUT    1
#define ENABLE_HDMI3ND_INPUT    0
#define ENABLE_MHL_C1           0// 1
#define ENABLE_MHL_C2           0// 1
#define ENABLE_MHL_C3           0// 1
#define ENABLE_MHL              0 //  (ENABLE_MHL_C1|ENABLE_MHL_C2|ENABLE_MHL_C3)
#define ENABLE_DP_OUTPUT        PANEL_EDP // 1
#define ENABLE_DP_INPUT         1
#define ENABLE_DP2ND_INPUT      0
#define ENABLE_DP3ND_INPUT      0
#define ENABLE_VD_INPUT         0
#define ENABLE_LED_CONTROLLER   0
#define ENABLE_DAISY_CHAIN      0 // 1 temp set to 0 ....
#define ENABLE_DC_SDM_FORWARD   (0 && ENABLE_DAISY_CHAIN)	// need sync with R2 setting
#define DISABLE_AUTO_SWITCH     0
#elif (MS_BOARD_TYPE_SEL==BD_MST9UHD_GV1)
#define INPUT_TYPE              (INPUT_3C)
#define ENABLE_VGA_INPUT        (INPUT_TYPE&INPUT_1A)
#define ENABLE_DVI_INPUT           0
#define ENABLE_DVI2ND_INPUT     0
#define ENABLE_DVI3ND_INPUT     0
#define ENABLE_HDMI_INPUT          1
#define ENABLE_HDMI2ND_INPUT    1
#define ENABLE_HDMI3ND_INPUT    0
#define ENABLE_MHL_C1           0// 1
#define ENABLE_MHL_C2           0// 1
#define ENABLE_MHL_C3           0// 1
#define ENABLE_MHL              0 //  (ENABLE_MHL_C1|ENABLE_MHL_C2|ENABLE_MHL_C3)
#define ENABLE_DP_OUTPUT        PANEL_EDP // 1
#define ENABLE_DP_INPUT         1
#define ENABLE_DP2ND_INPUT      0
#define ENABLE_DP3ND_INPUT      0
#define ENABLE_VD_INPUT         0
#define ENABLE_LED_CONTROLLER   0
#define ENABLE_DAISY_CHAIN      0 // 1 temp set to 0 ....
#define ENABLE_DC_SDM_FORWARD   (0 && ENABLE_DAISY_CHAIN)	// need sync with R2 setting
#define DISABLE_AUTO_SWITCH     0


#else
#message "please define MST9U new board type for Port_Func_Enable"
#endif

////////////////////////////////////////////////////////////
// PANEL SELECTION
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// BRIGHTNESS CONTROL
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// HDMI
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// AUDIO
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// OSD
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// SYSTEM
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// POWERMANAGEMENT
////////////////////////////////////////////////////////////
#define DISABLE_R2CORE_POWER()    do {\
                                                                   g_bCorePowerStatus=0;\
                                                                   msWriteByteMask(REG_00043A, 0 ,BIT4);\
                                                                   msWriteByteMask(REG_00043B, 0 ,BIT4);\
                                                                 } while(0)

#define ENABLE_R2CORE_POWER()    do {\
                                                                   g_bCorePowerStatus=1;\
                                                                   msWriteByteMask(REG_00043B, 0x00 ,BIT4);\
                                                                   msWriteByteMask(REG_00043A, BIT4 ,BIT4);\
                                                                 } while(0)
#if 1//New IO
#define DISABLE_DDR_POWER()    do {\
                                                                   msWriteByteMask(REG_00043A, 0x00 ,BIT4);\
                                                                   msWriteByteMask(REG_00043B, 0x00 ,BIT4);\
                                                                 } while(0)

#define ENABLE_DDR_POWER()    do {\
                                                                   msWriteByteMask(REG_00043B, 0x00 ,BIT4);\
                                                                   msWriteByteMask(REG_00043A, BIT4 ,BIT4);\
                                                                 } while(0)
#else//DEMO CODE
#define DISABLE_DDR_POWER()    do {\
                                                                   msWriteByteMask(_REG_GPIO2_OUT, 0x00 ,BIT3);\
                                                                   msWriteByteMask(_REG_GPIO2_OEZ, 0x00 ,BIT3);\
                                                                 } while(0)

#define ENABLE_DDR_POWER()    do {\
                                                                   msWriteByteMask(_REG_GPIO2_OEZ, BIT3 ,BIT3);\
                                                                   msWriteByteMask(_REG_GPIO2_OUT, BIT3 ,BIT3);\
                                                                 } while(0)
#endif

////////////////////////////////////////////////////////////
// INFORMATION
////////////////////////////////////////////////////////////
#define ChipName            "TSUMD"
#define Model                    "XXXXXXXXX"
#define FWVersion           "000"
#define ChangeDate          "20150402"
///////////////////////////////////////////////////////////////
// SYSTEM DEBUG
//////////////////////////////////////////////////////////////
#define ENABLE_DEBUG               1// 0//(defined(RunFeng_Project))

#define R2_51_DUAL_MODE            1 // need sync with R2 setting
#define ENABLE_MBX                       0 // need sync with R2 setting
#define ENABLE_CABLE_DET           0
#define ENABLE_DRAM_SELFREFRESH    0
#define NEW_HDMI2_0_CLK_RTERM        1
#define CORE_POWER_DELAY_TIME    200

#define LHCX_IR_Function_Enable                 (0&&(defined(LianHeChuangXin_Project)))

#define InputKeyWakeupEnable                   ((defined(LeXingEnMa_Project))||(defined(JieBo_BangZi_Project))||(defined(FengYuan_Project))||defined(XuLi_Project)||defined(MingCai_Project)||LHCX_IR_Function_Enable)//||(defined(LianHeChuangXin_Project)))

#define LED_FLICKER                                     (defined(KTC_Project)||defined(ZhouYang_Project)||defined(LianHeChuangXin_Project)||defined(Sceptre_Project))

#define JieBo_IR_Function_Enable               (defined(JieBo_BangZi_Project))

#define Special_One_SAR_Key_Func            (0&&(defined(TaiBei_Project)||defined(RunFeng_Project)))

#endif


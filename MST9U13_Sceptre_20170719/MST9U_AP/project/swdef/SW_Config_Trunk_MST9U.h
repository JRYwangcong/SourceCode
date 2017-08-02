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
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (MStar Confidential Information!¡L) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
// File Name: SW_Config_Trunk_MST9U.h
// Description: Customization and Specialization for default board!
// Revision History:
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _SW_CONFIG_TRUNK_MST9U_H_
#define _SW_CONFIG_TRUNK_MST9U_H_

#ifndef ENABLE
#define ENABLE                      1
#endif

#ifndef DISABLE
#define DISABLE                     0
#endif

//-------------------------------------------------------------------------------
// Bootloader System
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
// HW IP configuration for SW part
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
// Common Monitors System
//-------------------------------------------------------------------------------

//--------- ZUI, related with Panel resolution ----------------------------------
#define UI_SKIN_BM_800X600X1555                 0
#define UI_SKIN_NON_BM_800X600X1555        1
#if (ZUI_BM)
#define UI_SKIN_SEL                 UI_SKIN_BM_800X600X1555
#else
#define UI_SKIN_SEL                 UI_SKIN_NON_BM_800X600X1555
#endif

// ---- Mirror ---------------------------------------------------------------------

#if (ENABLE_MIRROR)
#define MirrorEnable                            ENABLE
#else
#define MirrorEnable                    DISABLE
#endif

#if (ENABLE_FORCE_MM_HD_FB)
#define ENABLE_MM_HD_FB                            ENABLE
#else
#define ENABLE_MM_HD_FB                            DISABLE
#endif


//------ DataBase in NAND -----------------------------------------------------
#define DB_IN_NAND                      DISABLE

//------ DataBase in EEPROM -----------------------------------------------------
#define EEPROM_SAVE_NONE                0
#define EEPROM_SAVE_WITHOUT_CH_DB       1
#define EEPROM_SAVE_ALL                 3

#if (DB_IN_NAND)
#define EEPROM_DB_STORAGE               EEPROM_SAVE_NONE//EEPROM_SAVE_WITHOUT_CH_DB
#else
#define EEPROM_DB_STORAGE               EEPROM_SAVE_NONE//EEPROM_SAVE_WITHOUT_CH_DB
#endif

#if (EEPROM_DB_STORAGE==EEPROM_SAVE_NONE)
#define STANDBY_EEPROM_ACCESS           DISABLE
#else
#define STANDBY_EEPROM_ACCESS           ENABLE
#endif


//------ FLASH DataBase size ------------------------------------------------------
#if (EEPROM_DB_STORAGE == EEPROM_SAVE_NONE)
#define  CM_DATABASE_FLASH_SIZE    FLASH_BLOCK_SIZE * 4
#elif (EEPROM_DB_STORAGE == EEPROM_SAVE_WITHOUT_CH_DB)
#define  CM_DATABASE_FLASH_SIZE    FLASH_BLOCK_SIZE * 2
#elif (EEPROM_DB_STORAGE == EEPROM_SAVE_ALL)
#define  CM_DATABASE_FLASH_SIZE    0
#endif

//----WatchDog-------------------------------------------------------------------
#define ENABLE_WATCH_DOG                DISABLE // need sync with 51 setting,
//------------------------------VECTOR FONT-------------------------------------
#define FONT_SUPPORT_VECTOR_AND_BITMAP  DISABLE
#define VECTOR_FONT_ENABLE              DISABLE

#if (VECTOR_FONT_ENABLE)
    #define ENABLE_ARABIC_OSD           DISABLE
    #define ENABLE_THAI_OSD             DISABLE
#endif

#ifndef ENABLE_ARABIC_OSD
    #define ENABLE_ARABIC_OSD           DISABLE
    #define ENABLE_THAI_OSD             DISABLE
#endif

#define CHINESE_SIMP_FONT_ENABLE        DISABLE
#define CHINESE_BIG5_FONT_ENABLE        DISABLE
#if (CHINESE_SIMP_FONT_ENABLE && CHINESE_BIG5_FONT_ENABLE)
#error "CHINESE_SIMP_FONT_ENABLE and CHINESE_BIG5_FONT_ENABLE can not both enabled!!"
#endif


//#define SUPPORT_AP_BIN_IN_FLASH_2
#ifdef SUPPORT_AP_BIN_IN_FLASH_2
//=== About SUPPORT_AP_BIN_IN_FLASH_2 Flag
//** Feature:
//   -- You can put some bin file to secondary Flash
//   -- If you only modify some bin file in flash_2, you can only update the flash_2
//   -- First bin file name = MERGE.bin Secondary = MERGE2.bin
//
//** How to enable:
//   --Step1: Enable this flag "SUPPORT_AP_BIN_IN_FLASH_2"
//   --Step2: choice the Secondary flash name "#define FLASH_2_NUM       FLASH_ID2" in the board define
//   --Step3: To set the Bin file to Flash-2 , add "-2" to BinInfo.h
//     Example: #define BIN_ID_MSB1210                  0x837   //~24KB
//              //@..\..\..\core\bin\demodulator\demod_msb1210.bin -2
//   --Step4: To add function call before Bin copy "InfoBlock_Flash_2_Checking_Start(&BinInfo);"
//   --Step5: To add function call after Bin copy " InfoBlock_Flash_2_Checking_End(&BinInfo);"

#define FLASH_2_NUM       FLASH_ID2
#endif

//-------------------------------------------------------------------------------
// miscellany
//-------------------------------------------------------------------------------
#define KEEP_UNUSED_FUNC            DISABLE


//-------------------------------------------------------------------------------
// Project
//-------------------------------------------------------------------------------

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

//#define HongTian_Project

//#define XiaBao_Project

//#define XuLi_Project

//#define MingCai_Project

//#define Aigao_Project

//#define KeOuShi_Project

#define ZhongXinKeJi_Project
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//LianHeChuangXin_Project 
#if (defined(LianHeChuangXin_Project))
//1920x1080 144
//#define K3C_Project
//#define GK3B_Project
//#define K3L_LED_Project
//#define K3D_FHD144_Project

//***********3440x1440 60/100

//#define Vage34A1_Project    
//#define Vage34A1_1H1DP_Project    
//#define GV3B_Project    
//Add_2560x1080 100 144
//#define IMA_Project
//#define WESCOM_Project              
//#define MicroBoard_Project
//#define Vmazon_Project
//#define Titan_Project
//#define Bangzi_Project
//Add 2560x1080 100
//#define Gaems_Project
//************************

//2560x1080 144
//#define V3L_Project
//#define V3G_Project
//#define V3K_Project
//#define V3L_CrossOver_Project

//3840x2160 60
//#define W3K_Project
//#define W5K_Project
//#define W315D_Project

//2560x1440 144
//K3L
//#define K3D_Project
//#define M3C_Project
//#define K3L_CrossOver_Project

//2560x1440 75
//PanelM315DVR011
//#define Test_Project

///************Sceptre*************///

#define K3D6G    1  //FHD144 32"
#define M3CDG    2  //FHD144 27"
#define V3K1M    3  //FHD+ 144 35"

#define ModeName              M3CDG
#endif
//-------------------------------------------------------------------------------
//FengYuan_Project
#if (defined(FengYuan_Project))
//#define KOIS_Project
//#define MU320K_Project
//#define USA_Project
//#define Korean_HYUNDAI_Project
//#define M280PU_Project
//#define ForZhouYang315HP_Project
//#define Japan_Project
#endif

//-------------------------------------------------------------------------------
//JieBo_Project
#if (defined(JieBo_Project))
#define TaiWan_Project
#endif
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//SenYo_Project
#if (defined(SenYo_Project))
#define CGC28W_Project
#endif
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//XinZuoEr_Project
#if (defined(XinZuoEr_Project))
#define H6_Project
//#define S273_Project
#endif
//-------------------------------------------------------------------------------

// Project
//-------------------------------------------------------------------------------
#define Disable_MultiWindow_AutoSwitch  (1&&ENABLE_MULTI_WINDOW_SETTINGS_MENU&&(defined(LianHeChuangXin_Project)))

#define DP_PBP_RELOAD_EDID               (ENABLE_DP_INPUT&&ENABLE_MULTI_WINDOW_SETTINGS_MENU&&(EDID_3840x2160||EDID_2560x1080_120||EDID_1920x1080_144||EDID_2560x1440_120||EDID_3440x1440))

#define HDMI_PBP_RELOAD_EDID           ((ENABLE_HDMI_INPUT&&ENABLE_MULTI_WINDOW_SETTINGS_MENU&&(!Disable_MultiWindow_AutoSwitch)&&(EDID_3840x2160||EDID_2560x1080_120||EDID_1920x1080_144||EDID_2560x1440_120||EDID_3440x1440))&&(!defined(JieBo_BangZi_Project)))

#define DVI_PBP_RELOAD_EDID             (ENABLE_MULTI_WINDOW_SETTINGS_MENU&&(!Disable_MultiWindow_AutoSwitch)&&ENABLE_DVI_INPUT&&(defined(KunDe_Project)))

#define Disable_4K60HZ_Timing            ((DP_PBP_RELOAD_EDID||HDMI_PBP_RELOAD_EDID||DVI_PBP_RELOAD_EDID)&&(defined(LianHeChuangXin_Project)))

#define New_Key_HuiGe                          (1&&(defined(HuiGe_Project)))

#define SevenKey_Function                   0//((!defined(XiaBao_Project))&&(!defined(HongTian_Project))&&(!(defined(KTC_Project)&&(PanelType == PanelLSM315HP01)))&&(!defined(SenPaiTe_Project))&&(!defined(FengYuan_Project))&&(!defined(RunFeng_Project))&&(!defined(LianHeChuangXin_Project))&&(!defined(ZhouYang_Project))&&(!defined(XinZuoEr_Project))&&(!defined(DaYu_Project)))

#define Rock5Key_Function			(defined(HongTian_Project))

#define Menu_Key_Use_For_Enter         1//(defined(XiaBao_Project)||defined(ZhouYang_Project)||defined(SenYo_Project)||New_Key_HuiGe||defined(HuaKe_Project)||defined(HKC_Project)||((defined(FengYuan_Project))&&(!defined(M280PU_Project)))||(defined(RunFeng_Project))||(defined(LianHeChuangXin_Project))||(defined(XinZuoEr_Project))||(defined(KTC_Project))||(defined(DaYu_Project))||((defined(JieBo_Project))/*&&(LOGO_AXM||LOGO_NIXEUS)*/))

#define Low_Blue_Light_Enable             1//(defined(LeXingEnMa_Project)||(defined(JieBo_BangZi_Project)))||(defined(LianHeChuangXin_Project)||(defined(RunFeng_Project))||(defined(KTC_Project))||(defined(HuiGe_Project)))

#define Enable_HotAdjLowbluray            0//   (defined(LeXingEnMa_Project)||(defined(LianHeChuangXin_Project)))//(defined(FengYuan_Project))  //Hot LowBluray

#define Enable_Lowbluray_FY                  0// (defined(FengYuan_Project)||defined(XiaBao_Project))  //Hot LowBluray

#define SleepMode_Enable                     (defined(LeXingEnMa_Project)) 

#define USB_Updata_FW_Enable            ((ENABLE_USB_HOST)&&((MS_BOARD_TYPE_SEL == BD_MST9UHL_V3)||(MS_BOARD_TYPE_SEL == BD_MST9UHL_A0))&&((defined(FengYuan_Project))||(((PanelType==PanelCMI28UltraHD)||(PanelType==PanelLC430EQE_FHM2))&&(defined(LeXingEnMa_Project)))))

#define HDMI_VerSion_Enable               ((MS_BOARD_TYPE_SEL == BD_MST9UHL_A0)&&(defined(LeXingEnMa_Project)))

#define BangZi_Special_Color                0

#if 0//
#define Enable_PQ_Setting                    0      
#else
#define Enable_PQ_Setting                    1     
#endif

#define AIO_Board                                       (1&&(defined(Vage34A1_Project)||defined(GV3B_Project)||defined(GK3B_Project)||defined(GV3B_Project)||defined(Vage34A1_1H1DP_Project)))

#define Enable_AutoInput_Menu                0//(defined(LeXingEnMa_Project)||defined(SanSe_Project)||((defined(FengYuan_Project))&&(!defined(ForZhouYang315HP_Project)))||(defined(JieBo_BangZi_Project))||(defined(Bangzi_Project)))

#define Audio_Source_Enable                     (1&&(defined(LeXingEnMa_Project)||((defined(FengYuan_Project))&&(!defined(ForZhouYang315HP_Project)))||defined(KunDe_Project)||defined(ZhouYang_Project)||defined(JieBo_BangZi_Project)||defined(RX_Project)||defined(Bangzi_Project))&&ENABLE_AUDIO_SETTINGS_MENU)

#define Free_Sync_Enable                          (1&&(defined(LianHeChuangXin_Project)))

#define ENABLE_SUPER_RESOLUTION         (Enable_PQ_Setting&&(!defined(KTC_Project))&&(!defined(XiaBao_Project)))

#define PXP_VGA_Auto_Enable                   ( 1&&!defined(KTC_Project))

#define POWER_KEY_FUNC                           ( (defined(KTC_Project)&&PanelType==PanelLSM315HP01)||(defined(HongTian_Project)&&Rock5Key_Function))

#define ADJ_BackLight_Enable                   1//(((PanelType==PanelCMI28UltraHD||PanelType==PanelV400DK1_KS1||PanelType==PanelLC430EQE_FHM2/*||PanelType==PanelM320QAN01*/)&&(defined(LeXingEnMa_Project)))||(defined(Bangzi_Project)))

#define DP_Version_Enable                         ((ENABLE_DP_INPUT)&&(!defined(KunDe_Project))&&(!defined(KTC_Project)))

#if (defined(HuiGe_Project)||defined(KunDe_Project)||defined(JieBo_Project)||defined(FengYuan_Project)||defined(Korean_Project)||defined(RX_Project)||defined(KTC_Project)||defined(HaiQi_Project)||defined(LanChuangKeJi_Project)||defined(JieBo_BangZi_Project)||defined(KeTe_Project)||defined(HuaKe_Project)||defined(XinZuoEr_Project)||defined(Kogan_Project))
#define ENABLE_DEMURA_FUNCTION              (1)
#else
#define ENABLE_DEMURA_FUNCTION              (0)
#endif

#define Internal_Power_Setting                     ((MS_BOARD_TYPE_SEL == BD_MST9UHL_V3)&&(PanelType==PanelV400DK1_KS1)&&(defined(LeXingEnMa_Project)))

#define PowerSequence_New                         (PANEL_VBY1())

#define JieBo_IR_Function_Enable                (defined(JieBo_BangZi_Project))

#define New_Cross_Enable                            (((!Internal_Power_Setting)&&(defined(LeXingEnMa_Project))))

#define Enable_Corss_FY                              ((defined(FengYuan_Project))||(defined(XiaBao_Project)))

#define Enable_Corss_LHCX                           1//(0&&(defined(LianHeChuangXin_Project)))

#define Enable_Corss_LHCX_New                  0//(defined(LianHeChuangXin_Project))

#define Enable_Corss_RunFeng                     (defined(RunFeng_Project))

#define NewIRFunction                                   ((defined(HuaKe_Project))||(defined(FengYuan_Project))||(defined(XuLi_Project)))

#define Enable_Panel_Rotate_180                (1&&(PanelType==PanelLSM315HP01))//((defined(XiaBao_Project)&&(PanelType==PanelLSM315HP01))||(defined(HongTian_Project)&&(PanelType==PanelLSM315HP01||PanelType==PanelM238DCJ_E50))||((defined(KTC_Project))&&(PanelType==PanelLSM315HP01))||((defined(LianHeChuangXin_Project))&&(PanelType==PanelLSM315HP01))||((defined(SenPaiTe_Project))&&(PanelType==PanelM315DJJ_K30))||(defined(SanSe_Project)&&(PanelType== PanelV500DK2_KS1))||(defined(SuChang_Project)&&(PanelType== PanelLC550EGY_SGM1))||(defined(ForZhouYang315HP_Project)))

#define Enable_Menu_Panel_Rotate_180      0// (defined(FengYuan_Project))

#define LHCX_EnterFactoryMode                  	 0//((defined(LianHeChuangXin_Project))||((defined(RunFeng_Project))&&(PanelType==PanelT500QVR05)))

#define HKC_PCToolCommand                        (defined(LianHeChuangXin_Project))

#define Aspect_1_1_Enable                           1//(!defined(LianHeChuangXin_Project))

#define Aspect_Auto_Enable                          0//(defined(LianHeChuangXin_Project))

#define EnableDdcLed                                     (defined(LianHeChuangXin_Project))

#define Enable_ECOGameMode                      1//((defined(LianHeChuangXin_Project))||(defined(RunFeng_Project)))

#define HongTian_MenuToFactoryMode         (defined(HongTian_Project))

#define Enable_Multi_Win3                            (1&&ENABLE_MULTI_WINDOW_SETTINGS_MENU)

#define Enable_Multi_Win4                            (1&&ENABLE_MULTI_WINDOW_SETTINGS_MENU)

#define Enable_ColorTemp_5700K                1

#define ENABLE_ADOBE_SRGB                        (1&&(defined(W315D_Project)))

#define HKC_RGB_Range_Func                      (0&&(defined(LianHeChuangXin_Project)))

#define HDMI_HPD_Enable                             (1&&(defined(FengYuan_Project)))

#define HKC_ColorTemp_Adjust                      1//(defined(LianHeChuangXin_Project))

#define FDDCCI_JRY_WriteEDID                      0

#define HKC_DDCCI_WRITEEDID                      0
//#define Adj_DefBri_PWM

#define MultiwindowON_NearCenter               (1&&(defined(FengYuan_Project)))

#define Special_One_SAR_Key_Func             (1&&(((defined(TaiBei_Project))&&(PanelType==PanelM270QAN01)))||((defined(RunFeng_Project))&&(PanelType==PanelT500QVR05)))

#define ChecK_EDID_256                                0

#define AC_ON_ChecK_EDID                           (1&&(defined(HuiGe_Project)))

#define LED_Control_By_PWM                        ((defined(FengYuan_Project))&&(PanelType==PanelLSC400FN04))//

#define SwitchPortByDP_Detect                     ((MS_BOARD_TYPE_SEL == BD_MST9UHL_V4)&&(defined(Vage34A1_1H1DP_Project)))

#define Gray_ColorMenupageItems                1//(1&&defined(KTC_Project))

#define F_ResetItem                                       (1&&defined(KTC_Project))

#define LHCX_IR_Function_Enable                 (1&&defined(W3K_Project))

#define FreeSync_Not_Support_Info              (1&&Free_Sync_Enable&&(defined(W3K_Project)))

#if (defined(KTC_Project))
#define Enable_PQ_Bypass                              0
#else
#define Enable_PQ_Bypass                              1
#endif

#define New_Japanese_String                         (1&&(defined(XuLi_Project)||defined(Japan_Project))) 


#define Enable_OSD_Vposition                        (1&&(!ENABLE_OSD_LR_DOUBLE)&&(defined(ZhongXinKeJi_Project)))


//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//----Control Backlight Function-------------------------------------------------
#if (defined(HKC_Project)||defined(TaiBei_Project)||defined(SenPaiTe_Project)||defined(YiHongGaoQing_Project)||defined(KTC_Project) )
#define ENABLE_DLC                          (DISABLE&&Enable_PQ_Setting)
#else
#define ENABLE_DLC                          (DISABLE&&Enable_PQ_Setting)
#endif
#define MWE_FUNCTION                    DISABLE//ENABLE
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------

#define elan_sun		ENABLE

#endif // _SW_CONFIG_TRUNK_MST9U_H_


///////////////////////////////////////////////////////////////////////////////
/// @file OsdDefault.h
/// @brief Head file for OSD default value definition.
/// @author MStarSemi Inc.
///
///
///////////////////////////////////////////////////////////////////////////////


#ifndef _OSD_DEFAULT_H
#define _OSD_DEFAULT_H

#include "Board.h"
#include "UserPrefDef.h"
#include "menudef.h"
#include "datatype.h"
//======================================================================
// Audio Setup Item Define
//======================================================================
#define VOLUME_OSD_MAX     		(100) 
#define VOLUME_OSD_MID               (50)

#if ENABLE_AUDIO_NEW_CURVE
#define VOLUME_OSD_MIN     		(0)
#define DEF_VOLUME              		(80)
#else
#define VOLUME_OSD_MIN     		(0)
#define DEF_VOLUME              		(100)
#endif

#define MIN_CUSTOMER_RED_COLOR      0x41
#define MIN_CUSTOMER_GREEN_COLOR    0x41
#define MIN_CUSTOMER_BLUE_COLOR     0x41

#define MAX_CONTRAST_VALUE          100
#define MIN_CONTRAST_VALUE          0



#if (defined(LeXingEnMa_Project))
  #if (PanelType==PanelLC490EQE_XGF1)
  #define DEF_R_COLOR_VALUE                128
  #define DEF_G_COLOR_VALUE                128  //factory require
  #define DEF_B_COLOR_VALUE                110//128  //factory require
  #elif (PanelType==PanelV400DK1_KS1)
  #if (MS_BOARD_TYPE_SEL == BD_MST9UHL_V3)
  #define DEF_R_COLOR_VALUE                126
  #define DEF_G_COLOR_VALUE                126  //factory require
  #define DEF_B_COLOR_VALUE                118//128  //factory require
  #else
  #define DEF_R_COLOR_VALUE                126
  #define DEF_G_COLOR_VALUE                126  //factory require
  #define DEF_B_COLOR_VALUE                126//128  //factory require
  #endif
  #elif (PanelType==PanelLC430EQE_FHM2)
  #define DEF_R_COLOR_VALUE                128
  #define DEF_G_COLOR_VALUE                134  //factory require
  #define DEF_B_COLOR_VALUE                120//126//128  //factory require
  /*
  #define DEF_R_COLOR_VALUE                120
  #define DEF_G_COLOR_VALUE                122  //factory require
  #define DEF_B_COLOR_VALUE                118//128  //factory require
  */
  #elif (PanelType==PanelCMI28UltraHD)
  #define DEF_R_COLOR_VALUE                128
  #define DEF_G_COLOR_VALUE                128  //factory require
  #define DEF_B_COLOR_VALUE                128//128  //factory require
  #elif PanelType==PanelM320QAN01
  #define DEF_R_COLOR_VALUE                125//120
  #define DEF_G_COLOR_VALUE                128  //factory require
  #define DEF_B_COLOR_VALUE                138//118//128  //factory require
/*
//20151201 Special Setting
  #define DEF_R_COLOR_VALUE                128//120
  #define DEF_G_COLOR_VALUE                128  //factory require
  #define DEF_B_COLOR_VALUE                133//118//128  //factory require
 */ 
  #else
  #define DEF_R_COLOR_VALUE                128
  #define DEF_G_COLOR_VALUE                128  //factory require
  #define DEF_B_COLOR_VALUE                128//128  //factory require
  
  #endif
#elif (defined(FengYuan_Project))
  #if (PanelType==PanelM320QAN01)
  #define DEF_R_COLOR_VALUE                125//136//141//128
  #define DEF_G_COLOR_VALUE                125//128  //factory require
  #define DEF_B_COLOR_VALUE                120//97//95//128  //factory require
  #elif (defined(ForZhouYang315HP_Project))
  #define DEF_R_COLOR_VALUE                128
  #define DEF_G_COLOR_VALUE                128
  #define DEF_B_COLOR_VALUE                128
  #else//PanelLSC400FN04
  #define DEF_R_COLOR_VALUE                125//136//141//128
  #define DEF_G_COLOR_VALUE                115//125//128  //factory require
  #define DEF_B_COLOR_VALUE                105//120//97//95//128  //factory require
  #endif
#elif (defined(Bangzi_Project))
  #define DEF_R_COLOR_VALUE                128
  #define DEF_G_COLOR_VALUE                128  //factory require
  #define DEF_B_COLOR_VALUE                128//128  //factory require
#else
  #define DEF_R_COLOR_VALUE                128
  #define DEF_G_COLOR_VALUE                128  //factory require
  #define DEF_B_COLOR_VALUE                128//128  //factory require
#endif

#define MAX_COLOR_VALUE                0xFF  //factory require

#if (defined(LeXingEnMa_Project))
  #if (PanelType==PanelLC430EQE_FHM2||PanelType==PanelM320QAN01)
  #define MAX_USER_R_COLOR_VALUE                255  //factory require
  #define MAX_USER_G_COLOR_VALUE                255  //factory require
  #define MAX_USER_B_COLOR_VALUE                255  //factory require
  /*
  #define MAX_USER_R_COLOR_VALUE                244  //factory require
  #define MAX_USER_G_COLOR_VALUE                244  //factory require
  #define MAX_USER_B_COLOR_VALUE                251  //factory require
  */
  #else
  #define MAX_USER_R_COLOR_VALUE                ((DEF_R_COLOR_VALUE>=128)?255:(126*2))  //factory require
  #define MAX_USER_G_COLOR_VALUE                ((DEF_G_COLOR_VALUE>=128)?255:(126*2))  //factory require
  #define MAX_USER_B_COLOR_VALUE                ((DEF_B_COLOR_VALUE>=128)?255:(126*2))  //factory require
  #endif
#else
#define MAX_USER_R_COLOR_VALUE                0xFF  //factory require
#define MAX_USER_G_COLOR_VALUE                0xFF  //factory require
#define MAX_USER_B_COLOR_VALUE                0xFF  //factory require
#endif


#define MAX_CUSTOM_HUE_VALUE        100
#define MIN_CUSTOM_HUE_VALUE        0

#define MAX_VIDEO_HUE_VALUE         100

#define MAX_CUSTOM_SATURATION_VALUE 100
#define MIN_CUSTOM_SATURATION_VALUE 0

#define MAX_VIDEO_SATURATION_VALUE  100

#define MAX_CUSTOM_COLOR_VALUE 100
#define MIN_CUSTOM_COLOR_VALUE 0


#define MAX_SHARPNESS               4///10

#define MAX_BLACK_LEVEL_VALUE       100

#define ADC_MAX_LIMIT               200
#define ADC_MIN_LIMIT               50

#define DEF_PGA_GAIN                0x03

#define DEF_RED_GAIN                5070
#define DEF_GREEN_GAIN              5188
#define DEF_BLUE_GAIN               5168

#define CONTRAST_VISTA_REQ          25
#define MAX_CONTRAST_GAIN           150
#define MIN_CONTRAST_GAIN           0
#define CONTRAST_GAIN_VISTA_REQ     68

#define DEF_PRESET_MODE_INDEX    	(PRESET_MODE_NUMBER-1)
#define DEF_USER_MODE_INDEX    		(USER_MODE_NUMBER-1)

///======================================
#define DEF_SRGB_BRIGHTNESS_ADJ     0xD0

#define FACTORY_DEF_BRIGHTNESS_MAX  0xFF
#define FACTORY_DEF_BRIGHTNESS_MIN  0x20

#define INIT_FACTORY_DEF_BRIGHTNESS 0xD2
#define INIT_FACTORY_DEF_CONTRAST	0x32

///======================================

///===== Color Block default value =====///
#define DEF_RESERVE_X           0x00
#define DEF_INPUT_COLOR_FORMAT  INPUT_COLOR_FORMAT_AUTO //(RGB: 0, YUV: 1, AUTO: 2)

#define DEF_IMAGE_MODE          CT_STANDARD

#define DEF_VIDEO_SATURATION    50
#define DEF_VIDEO_HUE           50

#if (defined(LeXingEnMa_Project))
#define DEF_CONTRAST            50
  #if (PanelType==PanelV400DK1_KS1)
  #define DEF_GAMMA               eGAMMA_24
  #define DEF_COLOR_TEMP              PCT_USER
  #elif ((PanelType==PanelLC430EQE_FHM2))//20150720
    #if BangZi_Special_Color
    #define DEF_GAMMA               eGAMMA_20
    #define DEF_COLOR_TEMP              PCT_6500K
    #else
    #define DEF_GAMMA               eGAMMA_22
    #define DEF_COLOR_TEMP              PCT_USER
    #endif
  #elif 0//(PanelType==PanelM320QAN01)
  //#define DEF_GAMMA               eGAMMA_18   //20151201 Special Setting
  //#define DEF_COLOR_TEMP              PCT_USER
  
  #define DEF_GAMMA               eGAMMA_22
  #define DEF_COLOR_TEMP              PCT_USER
  
  #else
  #define DEF_GAMMA               eGAMMA_22
  #define DEF_COLOR_TEMP              PCT_USER
  #endif

#elif (defined(HuiGe_Project)||defined(JieBo_Project)||defined(Kogan_Project)||defined(FengYuan_Project)||(defined(HaiQi_Project))||(defined(JieBo_BangZi_Project))||(defined(LianHeChuangXin_Project)))
#if (defined(Bangzi_Project))//for lianhechuangxin korean
#define DEF_COLOR_TEMP              PCT_USER
#define DEF_CONTRAST            50
#define DEF_GAMMA               eGAMMA_22
#else
#define DEF_COLOR_TEMP              PCT_6500K
#define DEF_CONTRAST            50
#define DEF_GAMMA               eGAMMA_22//eGAMMA_18
#endif
#elif (defined(SenYo_Project))
#define DEF_COLOR_TEMP              PCT_5700K
#define DEF_CONTRAST           50// 75
#define DEF_GAMMA               eGAMMA_22
#elif (defined(RunFeng_Project)||defined(XuLi_Project)||defined(KeOuShi_Project)||defined(MingCai_Project))
#define DEF_COLOR_TEMP              PCT_6500K
#define DEF_CONTRAST           50
#define DEF_GAMMA               eGAMMA_22
#elif (defined(HongHan_Project))
#define DEF_COLOR_TEMP              PCT_6500K
#define DEF_CONTRAST           50
#define DEF_GAMMA               eGAMMA_22

#elif (defined(KTC_Project))
#define DEF_COLOR_TEMP              PCT_9300K
#define DEF_CONTRAST           50
#define DEF_GAMMA               eGAMMA_22

#elif (defined(HongTian_Project))
#define DEF_COLOR_TEMP              PCT_5700K
#define DEF_CONTRAST           50
#define DEF_GAMMA               eGAMMA_22

#else
#define DEF_COLOR_TEMP              PCT_6500K
#define DEF_CONTRAST            50//75
#define DEF_GAMMA               eGAMMA_22//eGAMMA_18
#endif

#define DEF_PICTURE_MODE          CT_VIVID

#define DEF_SHARPNESS           2//5

#define DefLowbluray		0
#define MaxLowblurayValue	3///4
#define MinLowblurayValue	0


#define DEF_LOWBLUELIGHT           0

#define DEF_RED_COLOR           100
#define DEF_GREEN_COLOR         100
#define DEF_BLUE_COLOR          100

#define DEF_R_HUE               50
#define DEF_G_HUE               50
#define DEF_B_HUE               50
#define DEF_C_HUE               50
#define DEF_M_HUE               50
#define DEF_Y_HUE               50

#define DEF_R_SATURATION        50
#define DEF_G_SATURATION        50
#define DEF_B_SATURATION        50
#define DEF_C_SATURATION        50
#define DEF_M_SATURATION        50
#define DEF_Y_SATURATION        50


#define DEF_COLOR_R        50
#define DEF_COLOR_G        50
#define DEF_COLOR_B        50
#define DEF_COLOR_C        50
#define DEF_COLOR_M        50
#define DEF_COLOR_Y        50

#if 0//(defined(RunFeng_Project))
#define DEF_SUPER_RESOLUTION    SUPER_RESOLUTION_MIDDLE
#else
#define DEF_SUPER_RESOLUTION    SUPER_RESOLUTION_OFF
#endif
#define DEF_NOISE_REDUCTION     NR_OFF

#define DEF_COLOR_RESERVE_BYTE  0xFF


#define DEF_RESPONSE_TIME     RT_ON
///=====================================///

#if (defined(LeXingEnMa_Project))//(PanelType==PanelLC490EQE_XGF1)

#if (PanelType==PanelLC490EQE_XGF1)
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xD6//0xF0
#define BRIGHTNESS_REAL_MIN          0x00 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
#define BACKLIGHT_PWM_REVERSE          

#elif (PanelType==PanelV400DK1_KS1)
  #if (MS_BOARD_TYPE_SEL == BD_MST9UHL_V3)
    #define BRIGHTNESS_REAL_MAX         0xFF
    #define BRIGHTNESS_REAL_MID          0x83//0xA4
    #define BRIGHTNESS_REAL_MIN          0x00 
    
    #define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
    #define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_
    
    //#define PWM_ADJ_Brightness          
    #define DEF_BRIGHTNESS             		(50)
    #define DEF_rgbBRIGHTNESS             		(50)
   #if (!Internal_Power_Setting)	
    #define BACKLIGHT_PWM_REVERSE          
    #endif
  #else
    #define BRIGHTNESS_REAL_MAX         0xFF
    #define BRIGHTNESS_REAL_MID          0xA4
    #define BRIGHTNESS_REAL_MIN          0x00 
    
    #define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
    #define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_
    
    //#define PWM_ADJ_Brightness          
    #define DEF_BRIGHTNESS             		(50)
    #define DEF_rgbBRIGHTNESS             		(50)
    #define BACKLIGHT_PWM_REVERSE          
 #endif
 
#elif (PanelType==PanelLC430EQE_FHM2)
#if ((MS_BOARD_TYPE_SEL == BD_MST9UHL_A0))//&&(BangZi_Special_Color))
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0x7D//0xA4//0x8A(OSD:50)//2015020
#define BRIGHTNESS_REAL_MIN          0x00 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
#define BACKLIGHT_PWM_REVERSE          

#else
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0x66//0x8A //(OSD:50)//2015020
#define BRIGHTNESS_REAL_MIN          0x00 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
#define BACKLIGHT_PWM_REVERSE          
#endif

#elif (PanelType==PanelCMI28UltraHD)
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0x5A//0x70//0xA0 //(OSD:50)//2015020
#define BRIGHTNESS_REAL_MIN          0x00 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
#define BACKLIGHT_PWM_REVERSE          

#elif (PanelType==PanelLSC400FN04)
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0x70//0xA0 //(OSD:50)//2015020
#define BRIGHTNESS_REAL_MIN          0x00 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
#define BACKLIGHT_PWM_REVERSE          

#elif (PanelType==PanelM320QAN01)
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0x59//0x47//0x65//0xA0
//#define BRIGHTNESS_REAL_MID          0x50//0xA3//20151201
#define BRIGHTNESS_REAL_MIN          0x00 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_OD_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
#define BACKLIGHT_PWM_REVERSE          

#else
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xC8
#define BRIGHTNESS_REAL_MIN          0x19 

#define BACKLIGHT_PWM_FREQ                  240         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_

#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
//#define BACKLIGHT_PWM_REVERSE          

#endif

#elif (defined(ZhiFei_Project))
#if (PanelType==PanelV400DK1_KS1)
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0x87
#define BRIGHTNESS_REAL_MIN          0x19 

#define BACKLIGHT_PWM_FREQ                  160         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_

#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(80)
#define DEF_rgbBRIGHTNESS             		(80)
//#define BACKLIGHT_PWM_REVERSE          

#else
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0x87
#define BRIGHTNESS_REAL_MIN          0x19 

#define BACKLIGHT_PWM_FREQ                  160         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_

#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(80)
#define DEF_rgbBRIGHTNESS             		(80)
#define BACKLIGHT_PWM_REVERSE          

#endif

#elif (defined(ZDCC_Project))//PanelLSC400FP02
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0x90
#define BRIGHTNESS_REAL_MIN          0x19 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(80)
#define DEF_rgbBRIGHTNESS             		(80)
//#define BACKLIGHT_PWM_REVERSE          

#elif (defined(HuiGe_Project))//PanelCMI28UltraHD
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0x90
#define BRIGHTNESS_REAL_MIN          0x00 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_OD_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
#define BACKLIGHT_PWM_REVERSE          
#elif (defined(SanSe_Project))//PanelM270QAN01
#if (PanelType==PanelM270QAN01)
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xD0
#define BRIGHTNESS_REAL_MIN          0x19 

#define BACKLIGHT_PWM_FREQ                  300         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_

#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(80)
#define DEF_rgbBRIGHTNESS             		(80)
//#define BACKLIGHT_PWM_REVERSE          
#elif (PanelType==PanelV500DK2_KS1)
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xD0
#define BRIGHTNESS_REAL_MIN          0x00 

#define BACKLIGHT_PWM_FREQ                  180         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_

#define PWM_ADJ_Brightness          
//#define BACKLIGHT_PWM_REVERSE          
#define DEF_BRIGHTNESS             		(80)
#define DEF_rgbBRIGHTNESS             		(80)

#else
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xD0
#define BRIGHTNESS_REAL_MIN          0x00 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_

//#define PWM_ADJ_Brightness          
#define BACKLIGHT_PWM_REVERSE          
#define DEF_BRIGHTNESS             		(80)
#define DEF_rgbBRIGHTNESS             		(80)

#endif

#elif (defined(KunDe_Project))
#if (PanelType== PanelCMI28UltraHD)
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0x37//0xD0
#define BRIGHTNESS_REAL_MIN          0x08 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_OD_EN_

//#define PWM_ADJ_Brightness          
#define BACKLIGHT_PWM_REVERSE          
#define DEF_BRIGHTNESS             		(80)
#define DEF_rgbBRIGHTNESS             		(80)
#elif (PanelType== PanelM320QAN01)
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0x50
#define BRIGHTNESS_REAL_MIN          0x10//0x08 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_OD_EN_

//#define PWM_ADJ_Brightness          
#define BACKLIGHT_PWM_REVERSE          
#define DEF_BRIGHTNESS             		(80)
#define DEF_rgbBRIGHTNESS             		(80)
#else
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0x37//0xD0
#define BRIGHTNESS_REAL_MIN          0x08 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_OD_EN_

//#define PWM_ADJ_Brightness          
#define BACKLIGHT_PWM_REVERSE          
#define DEF_BRIGHTNESS             		(80)
#define DEF_rgbBRIGHTNESS             		(80)
#endif

#elif (defined(JieBo_Project))//PanelCMI28UltraHD
#if (PanelType==PanelV400DK1_KS1)
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xA4
#define BRIGHTNESS_REAL_MIN          0x19

#define BACKLIGHT_PWM_FREQ                  160         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_

#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
//#define BACKLIGHT_PWM_REVERSE          
#elif (PanelType==PanelCMI28UltraHD)
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0x80
#define BRIGHTNESS_REAL_MIN          0x00 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
#define BACKLIGHT_PWM_REVERSE          
#elif (PanelType==PanelLC420EQE_PGM1)
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0x80
#define BRIGHTNESS_REAL_MIN          0x00 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
#define BACKLIGHT_PWM_REVERSE          
#elif (PanelType==PanelM270QAN01)
#define BRIGHTNESS_REAL_MAX          0x4D//0xBF//0xFF
#define BRIGHTNESS_REAL_MID          0x26//0x80
#define BRIGHTNESS_REAL_MIN          0x00 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_OD_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
#define BACKLIGHT_PWM_REVERSE          

#else
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0x80
#define BRIGHTNESS_REAL_MIN          0x00 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
#define BACKLIGHT_PWM_REVERSE          
#endif

#elif (defined(JieBo_BangZi_Project))

#if (PanelType==PanelV400DK1_KS1)
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xA4
#define BRIGHTNESS_REAL_MIN          0x19

#define BACKLIGHT_PWM_FREQ                  160         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_

#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
//#define BACKLIGHT_PWM_REVERSE          
#elif (PanelType==PanelV400DJ1_QS5)
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xA4
#define BRIGHTNESS_REAL_MIN          0x00

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_OD_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
//#define BACKLIGHT_PWM_REVERSE          

#elif (PanelType==PanelCMI28UltraHD)
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0x80
#define BRIGHTNESS_REAL_MIN          0x00 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
#define BACKLIGHT_PWM_REVERSE          
#elif (PanelType==PanelLC420EQE_PGM1||PanelType==PanelLC550EQE_PGF1)
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0x80
#define BRIGHTNESS_REAL_MIN          0x00 

#define BACKLIGHT_PWM_FREQ                  120         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_

#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
//#define BACKLIGHT_PWM_REVERSE          

#elif (PanelType==PanelLM310UH1_SLA1||PanelType==PanelLM340UW1_SSA1)
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0x80
#define BRIGHTNESS_REAL_MIN          0x00 

#define BACKLIGHT_PWM_FREQ                  120         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_

#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
//#define BACKLIGHT_PWM_REVERSE          

#else
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0x80
#define BRIGHTNESS_REAL_MIN          0x00 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
#define BACKLIGHT_PWM_REVERSE          

#endif


#elif (defined(FengYuan_Project))//PanelCMI28UltraHD

#if (PanelType==PanelCMI28UltraHD)
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xA0
#define BRIGHTNESS_REAL_MIN          0x00 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_OD_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
#define BACKLIGHT_PWM_REVERSE          

#elif (PanelType==PanelLTM315FL01)
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xA0
#define BRIGHTNESS_REAL_MIN          0x00 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
#define BACKLIGHT_PWM_REVERSE          

#elif (PanelType==PanelV390DK1_LS1)
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xA0
#define BRIGHTNESS_REAL_MIN          0x12 

#define BACKLIGHT_PWM_FREQ                  160         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_

#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
//#define BACKLIGHT_PWM_REVERSE          

#elif (PanelType==PanelM320QAN01)
#if 0//(defined(Korean_HYUNDAI_Project))
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xA4
#define BRIGHTNESS_REAL_MIN          0x00 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_OD_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
#define BACKLIGHT_PWM_REVERSE          
#else
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0x67//0x73//0xA0
#define BRIGHTNESS_REAL_MIN          0x00 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_OD_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
#define BACKLIGHT_PWM_REVERSE          
#endif

#elif(PanelType==PanelLSC400FN04)//For Zhouyang
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xA8
#define BRIGHTNESS_REAL_MIN          0x00 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(100)
#define DEF_rgbBRIGHTNESS             		(100)
#define BACKLIGHT_PWM_REVERSE          

#elif (defined(ForZhouYang315HP_Project))//PanelLSM315HP01
#define BRIGHTNESS_REAL_MAX         80//0xFF
#define BRIGHTNESS_REAL_MID          50//0xA0
#define BRIGHTNESS_REAL_MIN          0x00 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_OD_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(80)
#define DEF_rgbBRIGHTNESS             		(80)
#define BACKLIGHT_PWM_REVERSE          

#else
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xA0
#define BRIGHTNESS_REAL_MIN          0x00 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_OD_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
#define BACKLIGHT_PWM_REVERSE          

#endif

#elif (defined(Korean_Project))//PanelCMI28UltraHD
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xA0
#define BRIGHTNESS_REAL_MIN          0x00 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
#define BACKLIGHT_PWM_REVERSE          

#elif (defined(RX_Project))//PanelCMI28UltraHD
#if 1
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0x2A//0xA7//0x50
#define BRIGHTNESS_REAL_MIN          0x00 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_OD_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(80)
#define DEF_rgbBRIGHTNESS             		(80)
#define BACKLIGHT_PWM_REVERSE          
#else
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xA7//0x50
#define BRIGHTNESS_REAL_MIN          0x00 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_OD_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(80)
#define DEF_rgbBRIGHTNESS             		(80)
#define BACKLIGHT_PWM_REVERSE          
#endif

#elif (defined(KTC_Project))//PanelM315DJJ_K30
#if (PanelType==PanelM315DJJ_K30)
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xA0
#define BRIGHTNESS_REAL_MIN          0x10 

#define BACKLIGHT_PWM_FREQ                  120         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_

#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(80)
#define DEF_rgbBRIGHTNESS             		(50)
//#define BACKLIGHT_PWM_REVERSE        

#elif (PanelType==PanelLSM315HP01)
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xE0
#define BRIGHTNESS_REAL_MIN          0x80 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_OD_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(80)
#define DEF_rgbBRIGHTNESS             		(50)
//#define BACKLIGHT_PWM_REVERSE          

#elif (PanelType==PanelLC430EGY_SHA1)
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xE0
#define BRIGHTNESS_REAL_MIN          0xA0 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_OD_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(80)
#define DEF_rgbBRIGHTNESS             		(50)
//#define BACKLIGHT_PWM_REVERSE          




#else
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xD0
#define BRIGHTNESS_REAL_MIN          0x80 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_OD_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
//#define BACKLIGHT_PWM_REVERSE          
#endif

#elif (defined(WanMei_Project))//PanelCMI28UltraHD
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xA0
#define BRIGHTNESS_REAL_MIN          0x00 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
#define BACKLIGHT_PWM_REVERSE          

#elif (defined(HaiQi_Project))//PanelCMI28UltraHD
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xA0
#define BRIGHTNESS_REAL_MIN          0x00 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
#define BACKLIGHT_PWM_REVERSE          

#elif (defined(ZhouYang_Project))//PanelV400DK1_QS1
#if (PanelType==PanelV400DK1_QS1)
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xEC
#define BRIGHTNESS_REAL_MIN          0x5A 

#define BACKLIGHT_PWM_FREQ                  200         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_

#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(90)
#define DEF_rgbBRIGHTNESS             		(90)
//#define BACKLIGHT_PWM_REVERSE          
#elif (PanelType==PanelM350DVR01_01) 
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xA8
#define BRIGHTNESS_REAL_MIN          0x00 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(90)
#define DEF_rgbBRIGHTNESS             		(90)
#define BACKLIGHT_PWM_REVERSE          

#elif(PanelType==PanelLSC400FN04)
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xA8
#define BRIGHTNESS_REAL_MIN          0x00 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(100)
#define DEF_rgbBRIGHTNESS             		(100)
#define BACKLIGHT_PWM_REVERSE          

#else //PanelCMI28UltraHD
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xA8
#define BRIGHTNESS_REAL_MIN          0x00 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(90)
#define DEF_rgbBRIGHTNESS             		(90)
#define BACKLIGHT_PWM_REVERSE          

#endif

#elif (defined(LanChuangKeJi_Project))//PanelLSC400FN03
#if (PanelType==PanelLSC400FN03)
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xA0
#define BRIGHTNESS_REAL_MIN          0x00 

#define BACKLIGHT_PWM_FREQ                  200         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_

#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
//#define BACKLIGHT_PWM_REVERSE      

#else
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xD0
#define BRIGHTNESS_REAL_MIN          0x00 

#define BACKLIGHT_PWM_FREQ                  4600         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
#define BACKLIGHT_PWM_REVERSE          

#endif

#elif (defined(ChongQing_Project))

#if (PanelType==PanelV400DK1_KS1)

#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xD0
#define BRIGHTNESS_REAL_MIN          0x00 

#define BACKLIGHT_PWM_FREQ                  160         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_

#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
//#define BACKLIGHT_PWM_REVERSE          

#elif (PanelType==PanelV500DK2_KS1)
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xD0
#define BRIGHTNESS_REAL_MIN          0x00 

#define BACKLIGHT_PWM_FREQ                  180         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_

#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
//#define BACKLIGHT_PWM_REVERSE          

#elif (PanelType==PanelV580DK2_KS1)
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xD0
#define BRIGHTNESS_REAL_MIN          0x00 

#define BACKLIGHT_PWM_FREQ                  180         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_

#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
//#define BACKLIGHT_PWM_REVERSE     

#elif (PanelType==PanelV650DK1_KS1)
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xD0
#define BRIGHTNESS_REAL_MIN          0x00 

#define BACKLIGHT_PWM_FREQ                  160         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_

#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
//#define BACKLIGHT_PWM_REVERSE          

#else//PanelCMI28UltraHD
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xD0
#define BRIGHTNESS_REAL_MIN          0x00 

#define BACKLIGHT_PWM_FREQ                  4600         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
#define BACKLIGHT_PWM_REVERSE          
#endif

#elif (defined(LanChuangKeJi_Project))//PanelV580DK2_KS1
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xA0
#define BRIGHTNESS_REAL_MIN          0x00 

#define BACKLIGHT_PWM_FREQ                  200         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_

#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
//#define BACKLIGHT_PWM_REVERSE          

#elif (defined(WoPai_Project))//PanelV580DK2_KS1
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xA0
#define BRIGHTNESS_REAL_MIN          0x00 

#define BACKLIGHT_PWM_FREQ                  160         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_

#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
//#define BACKLIGHT_PWM_REVERSE          

#elif (defined(SenYo_Project))//PanelCMI28UltraHD
#if 1
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0x82//0xD9//0xA0
#define BRIGHTNESS_REAL_MIN          0x00 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
#define BACKLIGHT_PWM_REVERSE          
#else
#define BRIGHTNESS_REAL_MAX         0xD0//0xFF
#define BRIGHTNESS_REAL_MID          0x88//0xA0
#define BRIGHTNESS_REAL_MIN          0x20 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
#define BACKLIGHT_PWM_REVERSE          
#endif

#elif (defined(KeTe_Project))//PanelV420DK1_KS1
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xA0
#define BRIGHTNESS_REAL_MIN          0x00 

#define BACKLIGHT_PWM_FREQ                  160         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_

#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
//#define BACKLIGHT_PWM_REVERSE          

#elif (defined(HuaKe_Project))//PanelM320QAN01
#if (PanelType==PanelM320QAN01)
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xF0//0xA0
#define BRIGHTNESS_REAL_MIN          0x10 

#define BACKLIGHT_PWM_FREQ                  180         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_OD_EN_

#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(80)
#define DEF_rgbBRIGHTNESS             		(50)
//#define BACKLIGHT_PWM_REVERSE          
#else
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xF0//0xA0
#define BRIGHTNESS_REAL_MIN          0x10 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_OD_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(80)
#define DEF_rgbBRIGHTNESS             		(50)
#define BACKLIGHT_PWM_REVERSE          

#endif

#elif (defined(HKC_Project))//PanelM315DJJ_K30
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xA0
#define BRIGHTNESS_REAL_MIN          0x10 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
#define BACKLIGHT_PWM_REVERSE          

#elif (defined(TaiBei_Project))
//PanelV390DK1_LS1
#if (PanelType==PanelV390DK1_LS1||PanelType==PanelT500QVN02||PanelType==PanelT550QVR05)
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xA0
#define BRIGHTNESS_REAL_MIN          0x00 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
#define BACKLIGHT_PWM_REVERSE       

#elif (PanelType==PanelS550QF62V1_V2)

#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xA0
#define BRIGHTNESS_REAL_MIN          0x00 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
#define BACKLIGHT_PWM_REVERSE          

#elif (PanelType==PanelM270QAN01)

#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xA0
#define BRIGHTNESS_REAL_MIN          0x10 

#define BACKLIGHT_PWM_FREQ                  180         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_

#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
//#define BACKLIGHT_PWM_REVERSE          


#else

#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xA0
#define BRIGHTNESS_REAL_MIN          0x00 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
#define BACKLIGHT_PWM_REVERSE          
#endif

#elif (defined(SenPaiTe_Project))
//PanelM315DJJ_K30
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xA0
#define BRIGHTNESS_REAL_MIN          0x00 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
#define BACKLIGHT_PWM_REVERSE          

#elif (defined(YiHongGaoQing_Project))
//PanelV400DK1_QS1
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xA0
#define BRIGHTNESS_REAL_MIN          0x00 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
#define BACKLIGHT_PWM_REVERSE          

#elif (defined(LianHeChuangXin_Project))
  #if (defined(Bangzi_Project)||defined(W315D_Project))
  #define BRIGHTNESS_REAL_MAX         0xFF
  #define BRIGHTNESS_REAL_MID          0xD8
  #define BRIGHTNESS_REAL_MIN          0x00 
  
  #define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
  #define BACKLIGHT_PWM_CONFIG                _PWM_OD_EN_
  
  //#define PWM_ADJ_Brightness          
  #define DEF_BRIGHTNESS             		(80)
  #define DEF_rgbBRIGHTNESS             		(DEF_BRIGHTNESS)
  #define BACKLIGHT_PWM_REVERSE          
  #else
  //PanelM315DJJ_K30
  #if (PanelType == PanelLSC400FN04)
  #define BRIGHTNESS_REAL_MAX         0xFF
  #define BRIGHTNESS_REAL_MID          0xD8
  #define BRIGHTNESS_REAL_MIN          0x12//0x00 
  
  #define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
  #define BACKLIGHT_PWM_CONFIG                _PWM_OD_EN_
  
  //#define PWM_ADJ_Brightness          
  #define DEF_BRIGHTNESS             		(90)
  #define DEF_rgbBRIGHTNESS             		(50)
  #define BACKLIGHT_PWM_REVERSE          
  #elif (PanelType == PanelT430QVN01)
  #define BRIGHTNESS_REAL_MAX         0xFF
  #define BRIGHTNESS_REAL_MID          0xD8
  #define BRIGHTNESS_REAL_MIN          0x13
  
  #define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
  #define BACKLIGHT_PWM_CONFIG                _PWM_OD_EN_
  
  //#define PWM_ADJ_Brightness          
  #define DEF_BRIGHTNESS             		(90)
  #define DEF_rgbBRIGHTNESS             		(DEF_BRIGHTNESS)
  #define BACKLIGHT_PWM_REVERSE          
  #elif (PanelType == PanelM350DVR01_01)
  #define BRIGHTNESS_REAL_MAX         0xFF
  #define BRIGHTNESS_REAL_MID          0xD8
  #define BRIGHTNESS_REAL_MIN          0x18
  
  #define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
  #define BACKLIGHT_PWM_CONFIG                _PWM_OD_EN_
  
  //#define PWM_ADJ_Brightness          
  #define DEF_BRIGHTNESS             		(90)
  #define DEF_rgbBRIGHTNESS             		(DEF_BRIGHTNESS)
  #define BACKLIGHT_PWM_REVERSE          
  #elif (PanelType == PanelLSM315DP01)
  #define BRIGHTNESS_REAL_MAX         0xFF
  #define BRIGHTNESS_REAL_MID          0xD8
  #define BRIGHTNESS_REAL_MIN          0x14
  
  #define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
  #define BACKLIGHT_PWM_CONFIG                _PWM_OD_EN_
  
  //#define PWM_ADJ_Brightness          
  #define DEF_BRIGHTNESS             		(90)
  #define DEF_rgbBRIGHTNESS             		(DEF_BRIGHTNESS)
  #define BACKLIGHT_PWM_REVERSE          
  #elif (PanelType == PanelLSM270DP01)
  #define BRIGHTNESS_REAL_MAX         0xFF
  #define BRIGHTNESS_REAL_MID          0xD8
  #define BRIGHTNESS_REAL_MIN          0x14
  
  #define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
  #define BACKLIGHT_PWM_CONFIG                _PWM_OD_EN_
  
  //#define PWM_ADJ_Brightness          
  #define DEF_BRIGHTNESS             		(90)
  #define DEF_rgbBRIGHTNESS             		(DEF_BRIGHTNESS)
  #define BACKLIGHT_PWM_REVERSE          
  #elif (PanelType == PanelLSM315HP01)
  #define BRIGHTNESS_REAL_MAX         0xFF
  #define BRIGHTNESS_REAL_MID          0xD8
  #define BRIGHTNESS_REAL_MIN          0x00
  
  #define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
  #define BACKLIGHT_PWM_CONFIG                _PWM_OD_EN_ //BD_MST9UHLV6
  
  //#define PWM_ADJ_Brightness          
  #define DEF_BRIGHTNESS             		(80)
  #define DEF_rgbBRIGHTNESS             		(50)
  #define BACKLIGHT_PWM_REVERSE          

  #elif (PanelType == PanelLSM270HP06)
  #define BRIGHTNESS_REAL_MAX         0xFF
  #define BRIGHTNESS_REAL_MID          0xD8
  #define BRIGHTNESS_REAL_MIN          0x00
  
  #define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
  #define BACKLIGHT_PWM_CONFIG                _PWM_OD_EN_ //BD_MST9UHLV6
  
  //#define PWM_ADJ_Brightness          
  #define DEF_BRIGHTNESS             		(80)
  #define DEF_rgbBRIGHTNESS             		(50)
  #define BACKLIGHT_PWM_REVERSE          
  
  #else //PanelLTM340YP01
  #if AIO_Board
  #define BRIGHTNESS_REAL_MAX          0xFF
  #define BRIGHTNESS_REAL_MID          0xF0//0xD8
  #define BRIGHTNESS_REAL_MIN          0x30//0x00 
  
  #define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
  #define BACKLIGHT_PWM_CONFIG                _PWM_OD_EN_
  
  //#define PWM_ADJ_Brightness          
  #define DEF_BRIGHTNESS             		(90)
  #define DEF_rgbBRIGHTNESS             		(50)
  #define BACKLIGHT_PWM_REVERSE          
  #else
  #define BRIGHTNESS_REAL_MAX         0xFF
  #define BRIGHTNESS_REAL_MID          0xD8
  #define BRIGHTNESS_REAL_MIN          0x00 
  
  #define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
  #define BACKLIGHT_PWM_CONFIG                _PWM_OD_EN_
  
  //#define PWM_ADJ_Brightness          
  #define DEF_BRIGHTNESS             		(90)
  #define DEF_rgbBRIGHTNESS             		(50)
  #define BACKLIGHT_PWM_REVERSE          
  #endif
  #endif
  #endif

#elif (defined(ZhongXinKeJi_Project))

#if 1 //350MA
  #define BRIGHTNESS_REAL_MAX         0x4D//0x4E//0xF0//0xFF
  #define BRIGHTNESS_REAL_MID          0x2D
  #define BRIGHTNESS_REAL_MIN          0x00
#else
  #define BRIGHTNESS_REAL_MAX         0x8A//0x4E//0xF0//0xFF
  #define BRIGHTNESS_REAL_MID          0x6A
  #define BRIGHTNESS_REAL_MIN          0x00
#endif  
  #define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
  #define BACKLIGHT_PWM_CONFIG                _PWM_OD_EN_ //BD_MST9UHLV6
  
  //#define PWM_ADJ_Brightness          
  #define DEF_BRIGHTNESS             		(80)
  #define DEF_rgbBRIGHTNESS             		(50)
  #define BACKLIGHT_PWM_REVERSE          

#elif (defined(RunFeng_Project))

#if (PanelType==PanelM350DVR01_01) 
//PanelM350DVR01_01
#define BRIGHTNESS_REAL_MAX         0xFF//0xFF
#define BRIGHTNESS_REAL_MID          0xEF//0xD9//0xA0
#define BRIGHTNESS_REAL_MIN          0x00 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_OD_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(80)
#define DEF_rgbBRIGHTNESS             	(DEF_BRIGHTNESS)
//#define BACKLIGHT_PWM_REVERSE      
#elif (PanelType==PanelT500QVR05)
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xE2
#define BRIGHTNESS_REAL_MIN          0x00

#define BACKLIGHT_PWM_FREQ                  160         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_OD_EN_

#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(80)
#define DEF_rgbBRIGHTNESS             		(DEF_BRIGHTNESS)
//#define BACKLIGHT_PWM_REVERSE          
#elif (PanelType==PanelT430QVN01)
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xE2
#define BRIGHTNESS_REAL_MIN          0x00

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_OD_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(80)
#define DEF_rgbBRIGHTNESS             		(DEF_BRIGHTNESS)
#define BACKLIGHT_PWM_REVERSE          

#else
//PanelM350DVR01_01
#define BRIGHTNESS_REAL_MAX         0xF7//0xFF
#define BRIGHTNESS_REAL_MID          0xE2//0xD9//0xA0
#define BRIGHTNESS_REAL_MIN          0x00 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_OD_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(80)
#define DEF_rgbBRIGHTNESS             	(DEF_BRIGHTNESS)
//#define BACKLIGHT_PWM_REVERSE          
#endif
#elif (defined(XinZuoEr_Project))

#if (defined(S273_Project))
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xD5
#define BRIGHTNESS_REAL_MIN          0x00 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_OD_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(80)
#define DEF_rgbBRIGHTNESS             		(DEF_BRIGHTNESS)
//#define BACKLIGHT_PWM_REVERSE          

#else
//PanelCMI28UltraHD
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xEC//0xA0
#define BRIGHTNESS_REAL_MIN          0x00 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_OD_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(90)
#define DEF_rgbBRIGHTNESS             		(DEF_BRIGHTNESS)
//#define BACKLIGHT_PWM_REVERSE          

#endif

#elif (defined(Kogan_Project))//PanelCMI28UltraHD
#if (PanelType==PanelCMI28UltraHD)
#define BRIGHTNESS_REAL_MAX         0x3D//0xFF
#define BRIGHTNESS_REAL_MID          0x21//0x80
#define BRIGHTNESS_REAL_MIN          0x0D 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_OD_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
#define BACKLIGHT_PWM_REVERSE          
#else
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0x80
#define BRIGHTNESS_REAL_MIN          0x00 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
#define BACKLIGHT_PWM_REVERSE          
#endif

#elif (defined(CaiTeng_Project))
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xA0
#define BRIGHTNESS_REAL_MIN          0x00 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
#define BACKLIGHT_PWM_REVERSE          

#elif (defined(XingYuan_Project))
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xA0
#define BRIGHTNESS_REAL_MIN          0x00 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
#define BACKLIGHT_PWM_REVERSE          

#elif (defined(WeiGuan_Project))
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xA0
#define BRIGHTNESS_REAL_MIN          0x00 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
#define BACKLIGHT_PWM_REVERSE       

#elif (defined(WeiZhen_Project))
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xA0
#define BRIGHTNESS_REAL_MIN          0x00 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
#define BACKLIGHT_PWM_REVERSE          

#elif (defined(SuChang_Project))
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xA0
#define BRIGHTNESS_REAL_MIN          0x00 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
#define BACKLIGHT_PWM_REVERSE          

#elif (defined(MengPai_Project))
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xD0
#define BRIGHTNESS_REAL_MIN          0x10 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_OD_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
#define BACKLIGHT_PWM_REVERSE          

#elif (defined(TianJiDisplay_Project))//PanelLC550EGY_SGM1
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xD0
#define BRIGHTNESS_REAL_MIN          0x00//0x10 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_OD_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
#define BACKLIGHT_PWM_REVERSE         

#elif (defined(JiSu_Project))//PanelCMI28UltraHD
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xD0
#define BRIGHTNESS_REAL_MIN          0x00//0x10 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_OD_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
#define BACKLIGHT_PWM_REVERSE          

#elif (defined(ZhongChuangLianHe_Project))//PanelLC490EQE_XGF1
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xD0
#define BRIGHTNESS_REAL_MIN          0x00//0x10 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_OD_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
#define BACKLIGHT_PWM_REVERSE          

#elif (defined(DaYu_Project))
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xD0
#define BRIGHTNESS_REAL_MIN          0x00//0x10 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_OD_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
#define BACKLIGHT_PWM_REVERSE          

#elif (defined(HongHan_Project))
//PanelLSM315HP01
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xD0
#define BRIGHTNESS_REAL_MIN          0x00//0x10 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_OD_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
#define BACKLIGHT_PWM_REVERSE          

#elif (defined(GuangZhou_Project))
//PanelCMI28UltraHD
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xD0
#define BRIGHTNESS_REAL_MIN          0x00//0x10 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_OD_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
#define BACKLIGHT_PWM_REVERSE          

#elif (defined(Changjia_Project))
//PanelLC550EGY_SGM1
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xD0
#define BRIGHTNESS_REAL_MIN          0x20 

#define BACKLIGHT_PWM_FREQ                  200         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_OD_EN_

#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
//#define BACKLIGHT_PWM_REVERSE          

#elif (defined(DongShanJingMi_Project))
//PanelLC550EGY_SGM1
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xA0
#define BRIGHTNESS_REAL_MIN          0x00//0x10 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_OD_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
#define BACKLIGHT_PWM_REVERSE          

#elif (defined(HongTian_Project))
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xA0
#define BRIGHTNESS_REAL_MIN          0x14 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_OD_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(80)
#define DEF_rgbBRIGHTNESS             		(DEF_BRIGHTNESS)
#define BACKLIGHT_PWM_REVERSE          

#elif (defined(XiaBao_Project))
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xA0
#define BRIGHTNESS_REAL_MIN          0x00//0x10 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_OD_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(80)
#define DEF_rgbBRIGHTNESS             		(DEF_BRIGHTNESS)
#define BACKLIGHT_PWM_REVERSE          
#elif (defined(XuLi_Project))
//PanelLSF490FJ02
#define BRIGHTNESS_REAL_MAX          0xFF//0xEE
#define BRIGHTNESS_REAL_MID          0xE0//0xDD
#define BRIGHTNESS_REAL_MIN          0x50

#define BACKLIGHT_PWM_FREQ                  160         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_OD_EN_

#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(100)
#define DEF_rgbBRIGHTNESS             		(DEF_BRIGHTNESS)
//#define BACKLIGHT_PWM_REVERSE          

#elif (defined(MingCai_Project))

//PanelLSF490FJ02
#define BRIGHTNESS_REAL_MAX          0xFF//0xFF
#define BRIGHTNESS_REAL_MID          0xDD
#define BRIGHTNESS_REAL_MIN          0x00

#define BACKLIGHT_PWM_FREQ                  160         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_OD_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(100)
#define DEF_rgbBRIGHTNESS             		(DEF_BRIGHTNESS)
#define BACKLIGHT_PWM_REVERSE          

#elif (defined(Aigao_Project))
//PanelLSF490FJ02
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xE8
#define BRIGHTNESS_REAL_MIN          0x50

#define BACKLIGHT_PWM_FREQ                  160         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_OD_EN_

#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(100)
#define DEF_rgbBRIGHTNESS             		(DEF_BRIGHTNESS)
//#define BACKLIGHT_PWM_REVERSE          

#elif (defined(KeOuShi_Project))
//PanelLSF490FJ02
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xE8
#define BRIGHTNESS_REAL_MIN          0x50

#define BACKLIGHT_PWM_FREQ                  160         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_OD_EN_

#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(100)
#define DEF_rgbBRIGHTNESS             		(DEF_BRIGHTNESS)
#define BACKLIGHT_PWM_REVERSE          



#else

#if (PanelType==PanelLC420EQE_PGM1)
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xD0
#define BRIGHTNESS_REAL_MIN          0x19 

#define BACKLIGHT_PWM_FREQ                  160         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_

#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(80)
#define DEF_rgbBRIGHTNESS             		(80)
//#define BACKLIGHT_PWM_REVERSE          
#elif (PanelType==PanelLTM340YP01)
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0x80
#define BRIGHTNESS_REAL_MIN          0x00 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_

//#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
#define BACKLIGHT_PWM_REVERSE          

#elif (PanelType==PanelP550QVN01)
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xA0
#define BRIGHTNESS_REAL_MIN          0x00 

#define BACKLIGHT_PWM_FREQ                  180         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_

#define PWM_ADJ_Brightness          
#define DEF_BRIGHTNESS             		(50)
#define DEF_rgbBRIGHTNESS             		(50)
//#define BACKLIGHT_PWM_REVERSE          

#elif (PanelType==PanelLSM315HP01)
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xD0
#define BRIGHTNESS_REAL_MIN          0x80 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_

//#define PWM_ADJ_Brightness          
//#define BACKLIGHT_PWM_REVERSE          
#define DEF_BRIGHTNESS             		(80)
#define DEF_rgbBRIGHTNESS             		(80)

#else
#define BRIGHTNESS_REAL_MAX         0xFF
#define BRIGHTNESS_REAL_MID          0xD0
#define BRIGHTNESS_REAL_MIN          0x00 

#define BACKLIGHT_PWM_FREQ                  46000         //unit:Hz
#define BACKLIGHT_PWM_CONFIG                _PWM_DB_EN_

//#define PWM_ADJ_Brightness          
#define BACKLIGHT_PWM_REVERSE          
#define DEF_BRIGHTNESS             		(80)
#define DEF_rgbBRIGHTNESS             		(80)
#endif
#endif





#define DEF_RED_BLACK_LEVEL     DEF_rgbBRIGHTNESS
#define DEF_GREEN_BLACK_LEVEL   DEF_rgbBRIGHTNESS
#define DEF_BLUE_BLACK_LEVEL    DEF_rgbBRIGHTNESS

///Color Setting Menu====================================
//#define DEF_GAMMA_SYSTEM        _PC_

#define DEF_RED_OFFSET          		0x80
#define DEF_GREEN_OFFSET       			0x80
#define DEF_BLUE_OFFSET         		0x80

#if (defined(LeXingEnMa_Project))//
#if (PanelType==PanelLC490EQE_XGF1)
#define DEF_RED_COLOR_SRGB      		       128
#define DEF_GREEN_COLOR_SRGB    		128
#define DEF_BLUE_COLOR_SRGB     		       128
#define DEF_BRIGHTNESS_COLOR_SRGB   	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_SRGB     	DEF_CONTRAST

#define DEF_RED_COLOR_9300K     		       123
#define DEF_GREEN_COLOR_9300K   		128
#define DEF_BLUE_COLOR_9300K    		131
#define DEF_BRIGHTNESS_COLOR_9300K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_9300K     	DEF_CONTRAST

#define DEF_RED_COLOR_6500K     		       135
#define DEF_GREEN_COLOR_6500K   		130
#define DEF_BLUE_COLOR_6500K    		116
#define DEF_BRIGHTNESS_COLOR_6500K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_6500K     	DEF_CONTRAST

#define DEF_RED_COLOR_5700K     		       140
#define DEF_GREEN_COLOR_5700K   		128
#define DEF_BLUE_COLOR_5700K    		110
#define DEF_BRIGHTNESS_COLOR_5700K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_5700K     	DEF_CONTRAST

#define DEF_RED_COLOR_5000K     		       128
#define DEF_GREEN_COLOR_5000K   		116
#define DEF_BLUE_COLOR_5000K    		106
#define DEF_BRIGHTNESS_COLOR_5000K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_5000K     	DEF_CONTRAST

#define DEF_RED_COLOR_7500K     		       128
#define DEF_GREEN_COLOR_7500K   		121
#define DEF_BLUE_COLOR_7500K    		114
#define DEF_BRIGHTNESS_COLOR_7500K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_7500K     	DEF_CONTRAST

#define DEF_RED_COLOR_10000K    		112
#define DEF_GREEN_COLOR_10000K  		103
#define DEF_BLUE_COLOR_10000K   		130
#define DEF_BRIGHTNESS_COLOR_10000K    DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_10000K     	DEF_CONTRAST

#elif (PanelType==PanelV400DK1_KS1)
#if (MS_BOARD_TYPE_SEL == BD_MST9UHL_V3)
//E15028-01
#define DEF_RED_COLOR_SRGB      		       128
#define DEF_GREEN_COLOR_SRGB    		128
#define DEF_BLUE_COLOR_SRGB     		       128
#define DEF_BRIGHTNESS_COLOR_SRGB   	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_SRGB     	DEF_CONTRAST

#define DEF_RED_COLOR_9300K     		       126//115
#define DEF_GREEN_COLOR_9300K   		126//125
#define DEF_BLUE_COLOR_9300K    		130//140
#define DEF_BRIGHTNESS_COLOR_9300K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_9300K     	DEF_CONTRAST

#define DEF_RED_COLOR_6500K     		       126
#define DEF_GREEN_COLOR_6500K   		126
#define DEF_BLUE_COLOR_6500K    		113//128
#define DEF_BRIGHTNESS_COLOR_6500K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_6500K     	DEF_CONTRAST

#define DEF_RED_COLOR_5700K     		       126//132
#define DEF_GREEN_COLOR_5700K   		126//124
#define DEF_BLUE_COLOR_5700K    		94//110
#define DEF_BRIGHTNESS_COLOR_5700K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_5700K     	DEF_CONTRAST

#define DEF_RED_COLOR_5000K     		       132
#define DEF_GREEN_COLOR_5000K   		124
#define DEF_BLUE_COLOR_5000K    		110
#define DEF_BRIGHTNESS_COLOR_5000K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_5000K     	DEF_CONTRAST

#define DEF_RED_COLOR_7500K     		       132
#define DEF_GREEN_COLOR_7500K   		124
#define DEF_BLUE_COLOR_7500K    		110
#define DEF_BRIGHTNESS_COLOR_7500K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_7500K     	DEF_CONTRAST

#define DEF_RED_COLOR_10000K    		112
#define DEF_GREEN_COLOR_10000K  		103
#define DEF_BLUE_COLOR_10000K   		130
#define DEF_BRIGHTNESS_COLOR_10000K    DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_10000K     	DEF_CONTRAST

#else
#if 0
//E15003-01
#define DEF_RED_COLOR_SRGB      		       128
#define DEF_GREEN_COLOR_SRGB    		128
#define DEF_BLUE_COLOR_SRGB     		       128
#define DEF_BRIGHTNESS_COLOR_SRGB   	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_SRGB     	DEF_CONTRAST

#define DEF_RED_COLOR_9300K     		       121
#define DEF_GREEN_COLOR_9300K   		115
#define DEF_BLUE_COLOR_9300K    		140
#define DEF_BRIGHTNESS_COLOR_9300K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_9300K     	DEF_CONTRAST

#define DEF_RED_COLOR_6500K     		       121
#define DEF_GREEN_COLOR_6500K   		115
#define DEF_BLUE_COLOR_6500K    		114
#define DEF_BRIGHTNESS_COLOR_6500K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_6500K     	DEF_CONTRAST

#define DEF_RED_COLOR_5700K     		       134
#define DEF_GREEN_COLOR_5700K   		114
#define DEF_BLUE_COLOR_5700K    		100
#define DEF_BRIGHTNESS_COLOR_5700K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_5700K     	DEF_CONTRAST

#define DEF_RED_COLOR_5000K     		       134
#define DEF_GREEN_COLOR_5000K   		114
#define DEF_BLUE_COLOR_5000K    		100
#define DEF_BRIGHTNESS_COLOR_5000K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_5000K     	DEF_CONTRAST

#define DEF_RED_COLOR_7500K     		       134
#define DEF_GREEN_COLOR_7500K   		114
#define DEF_BLUE_COLOR_7500K    		100
#define DEF_BRIGHTNESS_COLOR_7500K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_7500K     	DEF_CONTRAST

#define DEF_RED_COLOR_10000K    		112
#define DEF_GREEN_COLOR_10000K  		103
#define DEF_BLUE_COLOR_10000K   		130
#define DEF_BRIGHTNESS_COLOR_10000K    DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_10000K     	DEF_CONTRAST

#else
//E15022-01
#define DEF_RED_COLOR_SRGB      		       128
#define DEF_GREEN_COLOR_SRGB    		128
#define DEF_BLUE_COLOR_SRGB     		       128
#define DEF_BRIGHTNESS_COLOR_SRGB   	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_SRGB     	DEF_CONTRAST

#define DEF_RED_COLOR_9300K     		       115
#define DEF_GREEN_COLOR_9300K   		125
#define DEF_BLUE_COLOR_9300K    		140
#define DEF_BRIGHTNESS_COLOR_9300K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_9300K     	DEF_CONTRAST

#define DEF_RED_COLOR_6500K     		       128
#define DEF_GREEN_COLOR_6500K   		128
#define DEF_BLUE_COLOR_6500K    		128
#define DEF_BRIGHTNESS_COLOR_6500K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_6500K     	DEF_CONTRAST

#define DEF_RED_COLOR_5700K     		       132
#define DEF_GREEN_COLOR_5700K   		124
#define DEF_BLUE_COLOR_5700K    		110
#define DEF_BRIGHTNESS_COLOR_5700K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_5700K     	DEF_CONTRAST

#define DEF_RED_COLOR_5000K     		       132
#define DEF_GREEN_COLOR_5000K   		124
#define DEF_BLUE_COLOR_5000K    		110
#define DEF_BRIGHTNESS_COLOR_5000K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_5000K     	DEF_CONTRAST

#define DEF_RED_COLOR_7500K     		       132
#define DEF_GREEN_COLOR_7500K   		124
#define DEF_BLUE_COLOR_7500K    		110
#define DEF_BRIGHTNESS_COLOR_7500K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_7500K     	DEF_CONTRAST

#define DEF_RED_COLOR_10000K    		112
#define DEF_GREEN_COLOR_10000K  		103
#define DEF_BLUE_COLOR_10000K   		130
#define DEF_BRIGHTNESS_COLOR_10000K    DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_10000K     	DEF_CONTRAST
#endif
#endif

#elif (PanelType==PanelLC430EQE_FHM2)

#if BangZi_Special_Color//20150720
#define DEF_RED_COLOR_SRGB      		       128
#define DEF_GREEN_COLOR_SRGB    		128
#define DEF_BLUE_COLOR_SRGB     		       128
#define DEF_BRIGHTNESS_COLOR_SRGB   	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_SRGB     	DEF_CONTRAST

#define DEF_RED_COLOR_9300K     		       111
#define DEF_GREEN_COLOR_9300K   		128
#define DEF_BLUE_COLOR_9300K    		142
#define DEF_BRIGHTNESS_COLOR_9300K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_9300K     	DEF_CONTRAST

#define DEF_RED_COLOR_6500K     		       128
#define DEF_GREEN_COLOR_6500K   		128
#define DEF_BLUE_COLOR_6500K    		128
#define DEF_BRIGHTNESS_COLOR_6500K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_6500K     	DEF_CONTRAST

#define DEF_RED_COLOR_5700K     		       136
#define DEF_GREEN_COLOR_5700K   		125
#define DEF_BLUE_COLOR_5700K    		100
#define DEF_BRIGHTNESS_COLOR_5700K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_5700K     	DEF_CONTRAST

#define DEF_RED_COLOR_5000K     		       136
#define DEF_GREEN_COLOR_5000K   		125
#define DEF_BLUE_COLOR_5000K    		100
#define DEF_BRIGHTNESS_COLOR_5000K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_5000K     	DEF_CONTRAST

#define DEF_RED_COLOR_7500K     		       136
#define DEF_GREEN_COLOR_7500K   		125
#define DEF_BLUE_COLOR_7500K    		100
#define DEF_BRIGHTNESS_COLOR_7500K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_7500K     	DEF_CONTRAST

#define DEF_RED_COLOR_10000K    		112
#define DEF_GREEN_COLOR_10000K  		103
#define DEF_BLUE_COLOR_10000K   		130
#define DEF_BRIGHTNESS_COLOR_10000K    DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_10000K     	DEF_CONTRAST


#else
#define DEF_RED_COLOR_SRGB      		       128
#define DEF_GREEN_COLOR_SRGB    		128
#define DEF_BLUE_COLOR_SRGB     		       128
#define DEF_BRIGHTNESS_COLOR_SRGB   	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_SRGB     	DEF_CONTRAST

#define DEF_RED_COLOR_9300K     		       113
#define DEF_GREEN_COLOR_9300K   		118
#define DEF_BLUE_COLOR_9300K    		140
#define DEF_BRIGHTNESS_COLOR_9300K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_9300K     	DEF_CONTRAST

#define DEF_RED_COLOR_6500K     		       122//120
#define DEF_GREEN_COLOR_6500K   		122//118
#define DEF_BLUE_COLOR_6500K    		105//115
#define DEF_BRIGHTNESS_COLOR_6500K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_6500K     	DEF_CONTRAST

#define DEF_RED_COLOR_5700K     		       122//130
#define DEF_GREEN_COLOR_5700K   		122//118
#define DEF_BLUE_COLOR_5700K    		88//99
#define DEF_BRIGHTNESS_COLOR_5700K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_5700K     	DEF_CONTRAST

#define DEF_RED_COLOR_5000K     		       130
#define DEF_GREEN_COLOR_5000K   		118
#define DEF_BLUE_COLOR_5000K    		99
#define DEF_BRIGHTNESS_COLOR_5000K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_5000K     	DEF_CONTRAST

#define DEF_RED_COLOR_7500K     		       130
#define DEF_GREEN_COLOR_7500K   		118
#define DEF_BLUE_COLOR_7500K    		99
#define DEF_BRIGHTNESS_COLOR_7500K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_7500K     	DEF_CONTRAST

#define DEF_RED_COLOR_10000K    		112
#define DEF_GREEN_COLOR_10000K  		103
#define DEF_BLUE_COLOR_10000K   		130
#define DEF_BRIGHTNESS_COLOR_10000K    DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_10000K     	DEF_CONTRAST
#endif

#elif (PanelType==PanelCMI28UltraHD)
#define DEF_RED_COLOR_SRGB      		       128
#define DEF_GREEN_COLOR_SRGB    		128
#define DEF_BLUE_COLOR_SRGB     		       128
#define DEF_BRIGHTNESS_COLOR_SRGB   	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_SRGB     	DEF_CONTRAST

#define DEF_RED_COLOR_9300K     		       105
#define DEF_GREEN_COLOR_9300K   		130
#define DEF_BLUE_COLOR_9300K    		140
#define DEF_BRIGHTNESS_COLOR_9300K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_9300K     	DEF_CONTRAST

#define DEF_RED_COLOR_6500K     		       128
#define DEF_GREEN_COLOR_6500K   		128
#define DEF_BLUE_COLOR_6500K    		120
#define DEF_BRIGHTNESS_COLOR_6500K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_6500K     	DEF_CONTRAST

#define DEF_RED_COLOR_5700K     		       140
#define DEF_GREEN_COLOR_5700K   		128
#define DEF_BLUE_COLOR_5700K    		110
#define DEF_BRIGHTNESS_COLOR_5700K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_5700K     	DEF_CONTRAST

#define DEF_RED_COLOR_5000K     		       140
#define DEF_GREEN_COLOR_5000K   		128
#define DEF_BLUE_COLOR_5000K    		110
#define DEF_BRIGHTNESS_COLOR_5000K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_5000K     	DEF_CONTRAST

#define DEF_RED_COLOR_7500K     		       140
#define DEF_GREEN_COLOR_7500K   		128
#define DEF_BLUE_COLOR_7500K    		110
#define DEF_BRIGHTNESS_COLOR_7500K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_7500K     	DEF_CONTRAST

#define DEF_RED_COLOR_10000K    		112
#define DEF_GREEN_COLOR_10000K  		103
#define DEF_BLUE_COLOR_10000K   		130
#define DEF_BRIGHTNESS_COLOR_10000K    DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_10000K     	DEF_CONTRAST

#elif (PanelType==PanelM320QAN01)
#define DEF_RED_COLOR_SRGB      		       128
#define DEF_GREEN_COLOR_SRGB    		128
#define DEF_BLUE_COLOR_SRGB     		       128
#define DEF_BRIGHTNESS_COLOR_SRGB   	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_SRGB     	DEF_CONTRAST

#define DEF_RED_COLOR_9300K     		       125//123
#define DEF_GREEN_COLOR_9300K   		128
#define DEF_BLUE_COLOR_9300K    		153//138
#define DEF_BRIGHTNESS_COLOR_9300K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_9300K     	DEF_CONTRAST

#define DEF_RED_COLOR_6500K     		       125//128
#define DEF_GREEN_COLOR_6500K   		128
#define DEF_BLUE_COLOR_6500K    		136//138//128
/*
//20151201 Special Setting
#define DEF_RED_COLOR_6500K     		       128
#define DEF_GREEN_COLOR_6500K   		128
#define DEF_BLUE_COLOR_6500K    		128//138//128
*/
#define DEF_BRIGHTNESS_COLOR_6500K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_6500K     	DEF_CONTRAST

#define DEF_RED_COLOR_5700K     		       128
#define DEF_GREEN_COLOR_5700K   		128
#define DEF_BLUE_COLOR_5700K    		110
#define DEF_BRIGHTNESS_COLOR_5700K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_5700K     	DEF_CONTRAST

#define DEF_RED_COLOR_5000K     		       128
#define DEF_GREEN_COLOR_5000K   		116
#define DEF_BLUE_COLOR_5000K    		106
#define DEF_BRIGHTNESS_COLOR_5000K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_5000K     	DEF_CONTRAST

#define DEF_RED_COLOR_7500K     		       128
#define DEF_GREEN_COLOR_7500K   		121
#define DEF_BLUE_COLOR_7500K    		114
#define DEF_BRIGHTNESS_COLOR_7500K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_7500K     	DEF_CONTRAST

#define DEF_RED_COLOR_10000K    		112
#define DEF_GREEN_COLOR_10000K  		103
#define DEF_BLUE_COLOR_10000K   		130
#define DEF_BRIGHTNESS_COLOR_10000K    DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_10000K     	DEF_CONTRAST

#else
#define DEF_RED_COLOR_SRGB      		       128
#define DEF_GREEN_COLOR_SRGB    		128
#define DEF_BLUE_COLOR_SRGB     		       128
#define DEF_BRIGHTNESS_COLOR_SRGB   	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_SRGB     	DEF_CONTRAST

#define DEF_RED_COLOR_9300K     		       123
#define DEF_GREEN_COLOR_9300K   		128
#define DEF_BLUE_COLOR_9300K    		131
#define DEF_BRIGHTNESS_COLOR_9300K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_9300K     	DEF_CONTRAST

#define DEF_RED_COLOR_6500K     		       135
#define DEF_GREEN_COLOR_6500K   		130
#define DEF_BLUE_COLOR_6500K    		116
#define DEF_BRIGHTNESS_COLOR_6500K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_6500K     	DEF_CONTRAST

#define DEF_RED_COLOR_5700K     		       140
#define DEF_GREEN_COLOR_5700K   		128
#define DEF_BLUE_COLOR_5700K    		110
#define DEF_BRIGHTNESS_COLOR_5700K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_5700K     	DEF_CONTRAST

#define DEF_RED_COLOR_5000K     		       128
#define DEF_GREEN_COLOR_5000K   		116
#define DEF_BLUE_COLOR_5000K    		106
#define DEF_BRIGHTNESS_COLOR_5000K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_5000K     	DEF_CONTRAST

#define DEF_RED_COLOR_7500K     		       128
#define DEF_GREEN_COLOR_7500K   		121
#define DEF_BLUE_COLOR_7500K    		114
#define DEF_BRIGHTNESS_COLOR_7500K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_7500K     	DEF_CONTRAST

#define DEF_RED_COLOR_10000K    		112
#define DEF_GREEN_COLOR_10000K  		103
#define DEF_BLUE_COLOR_10000K   		130
#define DEF_BRIGHTNESS_COLOR_10000K    DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_10000K     	DEF_CONTRAST
#endif

#elif (defined(WanMei_Project))//PanelCMI28UltraHD

#define DEF_RED_COLOR_SRGB      		       128
#define DEF_GREEN_COLOR_SRGB    		128
#define DEF_BLUE_COLOR_SRGB     		       128
#define DEF_BRIGHTNESS_COLOR_SRGB   	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_SRGB     	DEF_CONTRAST

#define DEF_RED_COLOR_9300K     		       125
#define DEF_GREEN_COLOR_9300K   		112
#define DEF_BLUE_COLOR_9300K    		127
#define DEF_BRIGHTNESS_COLOR_9300K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_9300K     	DEF_CONTRAST

#define DEF_RED_COLOR_6500K     		       130
#define DEF_GREEN_COLOR_6500K   		111
#define DEF_BLUE_COLOR_6500K    		116
#define DEF_BRIGHTNESS_COLOR_6500K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_6500K     	DEF_CONTRAST

#define DEF_RED_COLOR_5700K     		       123
#define DEF_GREEN_COLOR_5700K   		117
#define DEF_BLUE_COLOR_5700K    		127
#define DEF_BRIGHTNESS_COLOR_5700K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_5700K     	DEF_CONTRAST

#define DEF_RED_COLOR_5000K     		       128
#define DEF_GREEN_COLOR_5000K   		116
#define DEF_BLUE_COLOR_5000K    		106
#define DEF_BRIGHTNESS_COLOR_5000K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_5000K     	DEF_CONTRAST

#define DEF_RED_COLOR_7500K     		       128
#define DEF_GREEN_COLOR_7500K   		121
#define DEF_BLUE_COLOR_7500K    		114
#define DEF_BRIGHTNESS_COLOR_7500K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_7500K     	DEF_CONTRAST

#define DEF_RED_COLOR_10000K    		112
#define DEF_GREEN_COLOR_10000K  		103
#define DEF_BLUE_COLOR_10000K   		130
#define DEF_BRIGHTNESS_COLOR_10000K    DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_10000K     	DEF_CONTRAST

#elif (defined(ZhouYang_Project))
#if (PanelType==PanelLSC400FN04)
#define DEF_RED_COLOR_SRGB      		       128
#define DEF_GREEN_COLOR_SRGB    		128
#define DEF_BLUE_COLOR_SRGB     		       128
#define DEF_BRIGHTNESS_COLOR_SRGB   	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_SRGB     	DEF_CONTRAST

#define DEF_RED_COLOR_9300K     		       111
#define DEF_GREEN_COLOR_9300K   		104
#define DEF_BLUE_COLOR_9300K    		128
#define DEF_BRIGHTNESS_COLOR_9300K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_9300K     	DEF_CONTRAST

#define DEF_RED_COLOR_6500K     		       128
#define DEF_GREEN_COLOR_6500K   		113
#define DEF_BLUE_COLOR_6500K    		118
#define DEF_BRIGHTNESS_COLOR_6500K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_6500K     	DEF_CONTRAST

#define DEF_RED_COLOR_5700K     		       128
#define DEF_GREEN_COLOR_5700K   		108
#define DEF_BLUE_COLOR_5700K    		110
#define DEF_BRIGHTNESS_COLOR_5700K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_5700K     	DEF_CONTRAST

#define DEF_RED_COLOR_5000K     		       129
#define DEF_GREEN_COLOR_5000K   		116
#define DEF_BLUE_COLOR_5000K    		122
#define DEF_BRIGHTNESS_COLOR_5000K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_5000K     	DEF_CONTRAST

#define DEF_RED_COLOR_7500K     		       129
#define DEF_GREEN_COLOR_7500K   		116
#define DEF_BLUE_COLOR_7500K    		122
#define DEF_BRIGHTNESS_COLOR_7500K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_7500K     	DEF_CONTRAST

#define DEF_RED_COLOR_10000K    		112
#define DEF_GREEN_COLOR_10000K  		103
#define DEF_BLUE_COLOR_10000K   		130
#define DEF_BRIGHTNESS_COLOR_10000K    DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_10000K     	DEF_CONTRAST

#else
//PanelCMI28UltraHD
#define DEF_RED_COLOR_SRGB      		       128
#define DEF_GREEN_COLOR_SRGB    		128
#define DEF_BLUE_COLOR_SRGB     		       128
#define DEF_BRIGHTNESS_COLOR_SRGB   	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_SRGB     	DEF_CONTRAST

#define DEF_RED_COLOR_9300K     		       102
#define DEF_GREEN_COLOR_9300K   		107
#define DEF_BLUE_COLOR_9300K    		128
#define DEF_BRIGHTNESS_COLOR_9300K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_9300K     	DEF_CONTRAST

#define DEF_RED_COLOR_6500K     		       121
#define DEF_GREEN_COLOR_6500K   		117
#define DEF_BLUE_COLOR_6500K    		124
#define DEF_BRIGHTNESS_COLOR_6500K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_6500K     	DEF_CONTRAST

#define DEF_RED_COLOR_5700K     		       129
#define DEF_GREEN_COLOR_5700K   		116
#define DEF_BLUE_COLOR_5700K    		122
#define DEF_BRIGHTNESS_COLOR_5700K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_5700K     	DEF_CONTRAST

#define DEF_RED_COLOR_5000K     		       129
#define DEF_GREEN_COLOR_5000K   		116
#define DEF_BLUE_COLOR_5000K    		122
#define DEF_BRIGHTNESS_COLOR_5000K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_5000K     	DEF_CONTRAST

#define DEF_RED_COLOR_7500K     		       129
#define DEF_GREEN_COLOR_7500K   		116
#define DEF_BLUE_COLOR_7500K    		122
#define DEF_BRIGHTNESS_COLOR_7500K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_7500K     	DEF_CONTRAST

#define DEF_RED_COLOR_10000K    		112
#define DEF_GREEN_COLOR_10000K  		103
#define DEF_BLUE_COLOR_10000K   		130
#define DEF_BRIGHTNESS_COLOR_10000K    DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_10000K     	DEF_CONTRAST
#endif

#elif (defined(SenYo_Project))//PanelCMI28UltraHD
#define DEF_RED_COLOR_SRGB      		       128
#define DEF_GREEN_COLOR_SRGB    		128
#define DEF_BLUE_COLOR_SRGB     		       128
#define DEF_BRIGHTNESS_COLOR_SRGB   	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_SRGB     	DEF_CONTRAST

#define DEF_RED_COLOR_9300K     		       105//110
#define DEF_GREEN_COLOR_9300K   		112
#define DEF_BLUE_COLOR_9300K    		128//127
#define DEF_BRIGHTNESS_COLOR_9300K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_9300K     	DEF_CONTRAST

#define DEF_RED_COLOR_6500K     		       124//114
#define DEF_GREEN_COLOR_6500K   		120//113
#define DEF_BLUE_COLOR_6500K    		128//118
#define DEF_BRIGHTNESS_COLOR_6500K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_6500K     	DEF_CONTRAST

#define DEF_RED_COLOR_5700K     		       117//114
#define DEF_GREEN_COLOR_5700K   		117//112
#define DEF_BLUE_COLOR_5700K    		128//120
#define DEF_BRIGHTNESS_COLOR_5700K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_5700K     	DEF_CONTRAST

#define DEF_RED_COLOR_5000K     		       128
#define DEF_GREEN_COLOR_5000K   		116
#define DEF_BLUE_COLOR_5000K    		106
#define DEF_BRIGHTNESS_COLOR_5000K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_5000K     	DEF_CONTRAST

#define DEF_RED_COLOR_7500K     		       117//114
#define DEF_GREEN_COLOR_7500K   		117//112
#define DEF_BLUE_COLOR_7500K    		128//120
#define DEF_BRIGHTNESS_COLOR_7500K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_7500K     	DEF_CONTRAST

#define DEF_RED_COLOR_10000K    		112
#define DEF_GREEN_COLOR_10000K  		103
#define DEF_BLUE_COLOR_10000K   		130
#define DEF_BRIGHTNESS_COLOR_10000K    DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_10000K     	DEF_CONTRAST

#elif (defined(HuaKe_Project))
#define DEF_RED_COLOR_SRGB      		       128
#define DEF_GREEN_COLOR_SRGB    		128
#define DEF_BLUE_COLOR_SRGB     		       128
#define DEF_BRIGHTNESS_COLOR_SRGB   	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_SRGB     	DEF_CONTRAST

#define DEF_RED_COLOR_9300K     		       112
#define DEF_GREEN_COLOR_9300K   		106//95
#define DEF_BLUE_COLOR_9300K    		128
#define DEF_BRIGHTNESS_COLOR_9300K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_9300K     	DEF_CONTRAST

#define DEF_RED_COLOR_6500K     		       122//128
#define DEF_GREEN_COLOR_6500K   		114//111
#define DEF_BLUE_COLOR_6500K    		128
#define DEF_BRIGHTNESS_COLOR_6500K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_6500K     	DEF_CONTRAST

#define DEF_RED_COLOR_5700K     		       123//128
#define DEF_GREEN_COLOR_5700K   		111//119
#define DEF_BLUE_COLOR_5700K    		119//110
#define DEF_BRIGHTNESS_COLOR_5700K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_5700K     	DEF_CONTRAST

#define DEF_RED_COLOR_5000K     		       128
#define DEF_GREEN_COLOR_5000K   		116
#define DEF_BLUE_COLOR_5000K    		106
#define DEF_BRIGHTNESS_COLOR_5000K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_5000K     	DEF_CONTRAST

#define DEF_RED_COLOR_7500K     		       128
#define DEF_GREEN_COLOR_7500K   		121
#define DEF_BLUE_COLOR_7500K    		114
#define DEF_BRIGHTNESS_COLOR_7500K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_7500K     	DEF_CONTRAST

#define DEF_RED_COLOR_10000K    		112
#define DEF_GREEN_COLOR_10000K  		103
#define DEF_BLUE_COLOR_10000K   		130
#define DEF_BRIGHTNESS_COLOR_10000K    DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_10000K     	DEF_CONTRAST

#elif (defined(FengYuan_Project))//PanelM320QAN01
#if (PanelType==PanelM320QAN01)
#define DEF_RED_COLOR_SRGB      		       128
#define DEF_GREEN_COLOR_SRGB    		128
#define DEF_BLUE_COLOR_SRGB     		       128
#define DEF_BRIGHTNESS_COLOR_SRGB   	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_SRGB     	DEF_CONTRAST

#define DEF_RED_COLOR_9300K     		       115//140
#define DEF_GREEN_COLOR_9300K   		121//139
#define DEF_BLUE_COLOR_9300K    		128//125
#define DEF_BRIGHTNESS_COLOR_9300K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_9300K     	DEF_CONTRAST

#define DEF_RED_COLOR_6500K     		       124//136
#define DEF_GREEN_COLOR_6500K   		126//133
#define DEF_BLUE_COLOR_6500K    		128//108
#define DEF_BRIGHTNESS_COLOR_6500K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_6500K     	DEF_CONTRAST

#define DEF_RED_COLOR_5700K     		       128//140
#define DEF_GREEN_COLOR_5700K   		119//126
#define DEF_BLUE_COLOR_5700K    		105//88
#define DEF_BRIGHTNESS_COLOR_5700K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_5700K     	DEF_CONTRAST

#define DEF_RED_COLOR_5000K     		       128
#define DEF_GREEN_COLOR_5000K   		116
#define DEF_BLUE_COLOR_5000K    		106
#define DEF_BRIGHTNESS_COLOR_5000K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_5000K     	DEF_CONTRAST

#define DEF_RED_COLOR_7500K     		       128
#define DEF_GREEN_COLOR_7500K   		121
#define DEF_BLUE_COLOR_7500K    		114
#define DEF_BRIGHTNESS_COLOR_7500K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_7500K     	DEF_CONTRAST

#define DEF_RED_COLOR_10000K    		112
#define DEF_GREEN_COLOR_10000K  		103
#define DEF_BLUE_COLOR_10000K   		130
#define DEF_BRIGHTNESS_COLOR_10000K    DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_10000K     	DEF_CONTRAST
#elif (PanelType==PanelLSC400FN04)//For ZhouYang_Project

#define DEF_RED_COLOR_SRGB      		       128
#define DEF_GREEN_COLOR_SRGB    		128
#define DEF_BLUE_COLOR_SRGB     		       128
#define DEF_BRIGHTNESS_COLOR_SRGB   	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_SRGB     	DEF_CONTRAST

#define DEF_RED_COLOR_9300K     		       127//111
#define DEF_GREEN_COLOR_9300K   		117//104
#define DEF_BLUE_COLOR_9300K    		128
#define DEF_BRIGHTNESS_COLOR_9300K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_9300K     	DEF_CONTRAST

#define DEF_RED_COLOR_6500K     		       128
#define DEF_GREEN_COLOR_6500K   		117//113
#define DEF_BLUE_COLOR_6500K    		114//118
#define DEF_BRIGHTNESS_COLOR_6500K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_6500K     	DEF_CONTRAST

#define DEF_RED_COLOR_5700K     		       128
#define DEF_GREEN_COLOR_5700K   		112//108
#define DEF_BLUE_COLOR_5700K    		95//97//110
#define DEF_BRIGHTNESS_COLOR_5700K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_5700K     	DEF_CONTRAST

#define DEF_RED_COLOR_5000K     		       129
#define DEF_GREEN_COLOR_5000K   		116
#define DEF_BLUE_COLOR_5000K    		122
#define DEF_BRIGHTNESS_COLOR_5000K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_5000K     	DEF_CONTRAST

#define DEF_RED_COLOR_7500K     		       129
#define DEF_GREEN_COLOR_7500K   		116
#define DEF_BLUE_COLOR_7500K    		122
#define DEF_BRIGHTNESS_COLOR_7500K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_7500K     	DEF_CONTRAST

#define DEF_RED_COLOR_10000K    		112
#define DEF_GREEN_COLOR_10000K  		103
#define DEF_BLUE_COLOR_10000K   		130
#define DEF_BRIGHTNESS_COLOR_10000K    DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_10000K     	DEF_CONTRAST

#elif (defined(ForZhouYang315HP_Project))
#define DEF_RED_COLOR_SRGB      		       128
#define DEF_GREEN_COLOR_SRGB    		128
#define DEF_BLUE_COLOR_SRGB     		       128
#define DEF_BRIGHTNESS_COLOR_SRGB   	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_SRGB     	DEF_CONTRAST

#define DEF_RED_COLOR_9300K     		       111
#define DEF_GREEN_COLOR_9300K   		112
#define DEF_BLUE_COLOR_9300K    		128
#define DEF_BRIGHTNESS_COLOR_9300K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_9300K     	DEF_CONTRAST

#define DEF_RED_COLOR_6500K     		       124//136
#define DEF_GREEN_COLOR_6500K   		126//133
#define DEF_BLUE_COLOR_6500K    		128//108
#define DEF_BRIGHTNESS_COLOR_6500K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_6500K     	DEF_CONTRAST

#define DEF_RED_COLOR_5700K     		       128
#define DEF_GREEN_COLOR_5700K   		120
#define DEF_BLUE_COLOR_5700K    		114
#define DEF_BRIGHTNESS_COLOR_5700K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_5700K     	DEF_CONTRAST

#define DEF_RED_COLOR_5000K     		       128
#define DEF_GREEN_COLOR_5000K   		116
#define DEF_BLUE_COLOR_5000K    		106
#define DEF_BRIGHTNESS_COLOR_5000K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_5000K     	DEF_CONTRAST

#define DEF_RED_COLOR_7500K     		       128
#define DEF_GREEN_COLOR_7500K   		121
#define DEF_BLUE_COLOR_7500K    		114
#define DEF_BRIGHTNESS_COLOR_7500K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_7500K     	DEF_CONTRAST

#define DEF_RED_COLOR_10000K    		112
#define DEF_GREEN_COLOR_10000K  		103
#define DEF_BLUE_COLOR_10000K   		130
#define DEF_BRIGHTNESS_COLOR_10000K    DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_10000K     	DEF_CONTRAST


#else
#define DEF_RED_COLOR_SRGB      		       128
#define DEF_GREEN_COLOR_SRGB    		128
#define DEF_BLUE_COLOR_SRGB     		       128
#define DEF_BRIGHTNESS_COLOR_SRGB   	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_SRGB     	DEF_CONTRAST

#define DEF_RED_COLOR_9300K     		       115//140
#define DEF_GREEN_COLOR_9300K   		121//139
#define DEF_BLUE_COLOR_9300K    		128//125
#define DEF_BRIGHTNESS_COLOR_9300K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_9300K     	DEF_CONTRAST

#define DEF_RED_COLOR_6500K     		       124//136
#define DEF_GREEN_COLOR_6500K   		126//133
#define DEF_BLUE_COLOR_6500K    		128//108
#define DEF_BRIGHTNESS_COLOR_6500K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_6500K     	DEF_CONTRAST

#define DEF_RED_COLOR_5700K     		       128//140
#define DEF_GREEN_COLOR_5700K   		119//126
#define DEF_BLUE_COLOR_5700K    		105//88
#define DEF_BRIGHTNESS_COLOR_5700K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_5700K     	DEF_CONTRAST

#define DEF_RED_COLOR_5000K     		       128
#define DEF_GREEN_COLOR_5000K   		116
#define DEF_BLUE_COLOR_5000K    		106
#define DEF_BRIGHTNESS_COLOR_5000K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_5000K     	DEF_CONTRAST

#define DEF_RED_COLOR_7500K     		       128
#define DEF_GREEN_COLOR_7500K   		121
#define DEF_BLUE_COLOR_7500K    		114
#define DEF_BRIGHTNESS_COLOR_7500K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_7500K     	DEF_CONTRAST

#define DEF_RED_COLOR_10000K    		112
#define DEF_GREEN_COLOR_10000K  		103
#define DEF_BLUE_COLOR_10000K   		130
#define DEF_BRIGHTNESS_COLOR_10000K    DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_10000K     	DEF_CONTRAST
#endif

#elif (defined(RunFeng_Project))
#if (PanelType==PanelT500QVR05)
#define DEF_RED_COLOR_SRGB      		       128
#define DEF_GREEN_COLOR_SRGB    		128
#define DEF_BLUE_COLOR_SRGB     		       128
#define DEF_BRIGHTNESS_COLOR_SRGB   	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_SRGB     	DEF_CONTRAST

#define DEF_RED_COLOR_9300K     		       124
#define DEF_GREEN_COLOR_9300K   		116
#define DEF_BLUE_COLOR_9300K    		110
#define DEF_BRIGHTNESS_COLOR_9300K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_9300K     	DEF_CONTRAST

#define DEF_RED_COLOR_6500K     		       130
#define DEF_GREEN_COLOR_6500K   		115
#define DEF_BLUE_COLOR_6500K    		96
#define DEF_BRIGHTNESS_COLOR_6500K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_6500K     	DEF_CONTRAST

#define DEF_RED_COLOR_5700K     		       128
#define DEF_GREEN_COLOR_5700K   		117
#define DEF_BLUE_COLOR_5700K    		106
#define DEF_BRIGHTNESS_COLOR_5700K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_5700K     	DEF_CONTRAST

#define DEF_RED_COLOR_5000K     		       128
#define DEF_GREEN_COLOR_5000K   		116
#define DEF_BLUE_COLOR_5000K    		106
#define DEF_BRIGHTNESS_COLOR_5000K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_5000K     	DEF_CONTRAST

#define DEF_RED_COLOR_7500K     		       128
#define DEF_GREEN_COLOR_7500K   		121
#define DEF_BLUE_COLOR_7500K    		114
#define DEF_BRIGHTNESS_COLOR_7500K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_7500K     	DEF_CONTRAST

#define DEF_RED_COLOR_10000K    		112
#define DEF_GREEN_COLOR_10000K  		103
#define DEF_BLUE_COLOR_10000K   		130
#define DEF_BRIGHTNESS_COLOR_10000K    DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_10000K     	DEF_CONTRAST

#elif (PanelType==PanelT430QVN01)
#define DEF_RED_COLOR_SRGB      		       128
#define DEF_GREEN_COLOR_SRGB    		128
#define DEF_BLUE_COLOR_SRGB     		       128
#define DEF_BRIGHTNESS_COLOR_SRGB   	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_SRGB     	DEF_CONTRAST

#define DEF_RED_COLOR_9300K     		       103
#define DEF_GREEN_COLOR_9300K   		113
#define DEF_BLUE_COLOR_9300K    		126
#define DEF_BRIGHTNESS_COLOR_9300K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_9300K     	DEF_CONTRAST

#define DEF_RED_COLOR_6500K     		       120
#define DEF_GREEN_COLOR_6500K   		120
#define DEF_BLUE_COLOR_6500K    		116
#define DEF_BRIGHTNESS_COLOR_6500K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_6500K     	DEF_CONTRAST

#define DEF_RED_COLOR_5700K     		       118
#define DEF_GREEN_COLOR_5700K   		117
#define DEF_BLUE_COLOR_5700K    		128
#define DEF_BRIGHTNESS_COLOR_5700K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_5700K     	DEF_CONTRAST

#define DEF_RED_COLOR_5000K     		       128
#define DEF_GREEN_COLOR_5000K   		116
#define DEF_BLUE_COLOR_5000K    		106
#define DEF_BRIGHTNESS_COLOR_5000K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_5000K     	DEF_CONTRAST

#define DEF_RED_COLOR_7500K     		       128
#define DEF_GREEN_COLOR_7500K   		121
#define DEF_BLUE_COLOR_7500K    		114
#define DEF_BRIGHTNESS_COLOR_7500K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_7500K     	DEF_CONTRAST

#define DEF_RED_COLOR_10000K    		112
#define DEF_GREEN_COLOR_10000K  		103
#define DEF_BLUE_COLOR_10000K   		130
#define DEF_BRIGHTNESS_COLOR_10000K    DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_10000K     	DEF_CONTRAST

#else
#define DEF_RED_COLOR_SRGB      		       128
#define DEF_GREEN_COLOR_SRGB    		128
#define DEF_BLUE_COLOR_SRGB     		       128
#define DEF_BRIGHTNESS_COLOR_SRGB   	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_SRGB     	DEF_CONTRAST

#define DEF_RED_COLOR_9300K     		       103//98
#define DEF_GREEN_COLOR_9300K   		114//115
#define DEF_BLUE_COLOR_9300K    		130//129
#define DEF_BRIGHTNESS_COLOR_9300K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_9300K     	DEF_CONTRAST

#define DEF_RED_COLOR_6500K     		       118//117//128
#define DEF_GREEN_COLOR_6500K   		116//116//121
#define DEF_BLUE_COLOR_6500K    		123
#define DEF_BRIGHTNESS_COLOR_6500K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_6500K     	DEF_CONTRAST

#define DEF_RED_COLOR_5700K     		       112//128
#define DEF_GREEN_COLOR_5700K   		117//115//119
#define DEF_BLUE_COLOR_5700K    		128//110
#define DEF_BRIGHTNESS_COLOR_5700K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_5700K     	DEF_CONTRAST

#define DEF_RED_COLOR_5000K     		       128
#define DEF_GREEN_COLOR_5000K   		116
#define DEF_BLUE_COLOR_5000K    		106
#define DEF_BRIGHTNESS_COLOR_5000K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_5000K     	DEF_CONTRAST

#define DEF_RED_COLOR_7500K     		       128
#define DEF_GREEN_COLOR_7500K   		121
#define DEF_BLUE_COLOR_7500K    		114
#define DEF_BRIGHTNESS_COLOR_7500K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_7500K     	DEF_CONTRAST

#define DEF_RED_COLOR_10000K    		112
#define DEF_GREEN_COLOR_10000K  		103
#define DEF_BLUE_COLOR_10000K   		130
#define DEF_BRIGHTNESS_COLOR_10000K    DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_10000K     	DEF_CONTRAST
#endif
#elif (defined(LianHeChuangXin_Project))
  #if (PanelType==PanelLTM340YP01)
  #if (defined(Bangzi_Project))
  #define DEF_RED_COLOR_SRGB      		128
  #define DEF_GREEN_COLOR_SRGB    		128
  #define DEF_BLUE_COLOR_SRGB     		128
  #define DEF_BRIGHTNESS_COLOR_SRGB   	DEF_BRIGHTNESS
  #define DEF_CONTRAST_COLOR_SRGB     	DEF_CONTRAST
  
  #define DEF_RED_COLOR_9300K     		128
  #define DEF_GREEN_COLOR_9300K   		128
  #define DEF_BLUE_COLOR_9300K    		138
  #define DEF_BRIGHTNESS_COLOR_9300K    	DEF_BRIGHTNESS
  #define DEF_CONTRAST_COLOR_9300K     	DEF_CONTRAST
  
  #define DEF_RED_COLOR_6500K     		128
  #define DEF_GREEN_COLOR_6500K   		128
  #define DEF_BLUE_COLOR_6500K    		118
  #define DEF_BRIGHTNESS_COLOR_6500K    	DEF_BRIGHTNESS
  #define DEF_CONTRAST_COLOR_6500K     	DEF_CONTRAST
  
  #define DEF_RED_COLOR_5700K     		128
  #define DEF_GREEN_COLOR_5700K   		128
  #define DEF_BLUE_COLOR_5700K    		133
  #define DEF_BRIGHTNESS_COLOR_5700K    	DEF_BRIGHTNESS
  #define DEF_CONTRAST_COLOR_5700K     	DEF_CONTRAST
  
  #define DEF_RED_COLOR_5000K     		128
  #define DEF_GREEN_COLOR_5000K   		116
  #define DEF_BLUE_COLOR_5000K    		106
  #define DEF_BRIGHTNESS_COLOR_5000K    	DEF_BRIGHTNESS
  #define DEF_CONTRAST_COLOR_5000K     	DEF_CONTRAST
  
  #define DEF_RED_COLOR_7500K     		128
  #define DEF_GREEN_COLOR_7500K   		121
  #define DEF_BLUE_COLOR_7500K    		114
  #define DEF_BRIGHTNESS_COLOR_7500K    	DEF_BRIGHTNESS
  #define DEF_CONTRAST_COLOR_7500K     	DEF_CONTRAST
  
  #define DEF_RED_COLOR_10000K    		112
  #define DEF_GREEN_COLOR_10000K  		103
  #define DEF_BLUE_COLOR_10000K   		130
  #define DEF_BRIGHTNESS_COLOR_10000K    DEF_BRIGHTNESS
  #define DEF_CONTRAST_COLOR_10000K     	DEF_CONTRAST
  #elif defined(W315D_Project)
  #define DEF_RED_COLOR_SRGB      		       128
  #define DEF_GREEN_COLOR_SRGB    		128
  #define DEF_BLUE_COLOR_SRGB     		       128
  #define DEF_BRIGHTNESS_COLOR_SRGB   	DEF_BRIGHTNESS
  #define DEF_CONTRAST_COLOR_SRGB     	DEF_CONTRAST
  
  #define DEF_RED_COLOR_9300K     		       112
  #define DEF_GREEN_COLOR_9300K   		109
  #define DEF_BLUE_COLOR_9300K    		128
  #define DEF_BRIGHTNESS_COLOR_9300K    	DEF_BRIGHTNESS
  #define DEF_CONTRAST_COLOR_9300K     	DEF_CONTRAST
  
  #define DEF_RED_COLOR_6500K     		       128
  #define DEF_GREEN_COLOR_6500K   		126//128//121
  #define DEF_BLUE_COLOR_6500K    		128//124
  #define DEF_BRIGHTNESS_COLOR_6500K    	DEF_BRIGHTNESS
  #define DEF_CONTRAST_COLOR_6500K     	DEF_CONTRAST
  
  #define DEF_RED_COLOR_5700K     		128
  #define DEF_GREEN_COLOR_5700K   		128
  #define DEF_BLUE_COLOR_5700K    		128
  #define DEF_BRIGHTNESS_COLOR_5700K    	DEF_BRIGHTNESS
  #define DEF_CONTRAST_COLOR_5700K     	DEF_CONTRAST
  
  #define DEF_RED_COLOR_5000K     		       128
  #define DEF_GREEN_COLOR_5000K   		116
  #define DEF_BLUE_COLOR_5000K    		106
  #define DEF_BRIGHTNESS_COLOR_5000K    	DEF_BRIGHTNESS
  #define DEF_CONTRAST_COLOR_5000K     	DEF_CONTRAST
  
  #define DEF_RED_COLOR_7500K     		       128
  #define DEF_GREEN_COLOR_7500K   		121
  #define DEF_BLUE_COLOR_7500K    		114
  #define DEF_BRIGHTNESS_COLOR_7500K    	DEF_BRIGHTNESS
  #define DEF_CONTRAST_COLOR_7500K     	DEF_CONTRAST
  
  #define DEF_RED_COLOR_10000K    		112
  #define DEF_GREEN_COLOR_10000K  		103
  #define DEF_BLUE_COLOR_10000K   		130
  #define DEF_BRIGHTNESS_COLOR_10000K    DEF_BRIGHTNESS
  #define DEF_CONTRAST_COLOR_10000K     	DEF_CONTRAST
  
  #else
  #define DEF_RED_COLOR_SRGB      		       128
  #define DEF_GREEN_COLOR_SRGB    		128
  #define DEF_BLUE_COLOR_SRGB     		       128
  #define DEF_BRIGHTNESS_COLOR_SRGB   	DEF_BRIGHTNESS
  #define DEF_CONTRAST_COLOR_SRGB     	DEF_CONTRAST
  
  #define DEF_RED_COLOR_9300K     		       112
  #define DEF_GREEN_COLOR_9300K   		109
  #define DEF_BLUE_COLOR_9300K    		128
  #define DEF_BRIGHTNESS_COLOR_9300K    	DEF_BRIGHTNESS
  #define DEF_CONTRAST_COLOR_9300K     	DEF_CONTRAST
  
  #define DEF_RED_COLOR_6500K     		       128
  #define DEF_GREEN_COLOR_6500K   		126//128//121
  #define DEF_BLUE_COLOR_6500K    		128//124
  #define DEF_BRIGHTNESS_COLOR_6500K    	DEF_BRIGHTNESS
  #define DEF_CONTRAST_COLOR_6500K     	DEF_CONTRAST
  
  #define DEF_RED_COLOR_5700K     		       128
  #define DEF_GREEN_COLOR_5700K   		119
  #define DEF_BLUE_COLOR_5700K    		110
  #define DEF_BRIGHTNESS_COLOR_5700K    	DEF_BRIGHTNESS
  #define DEF_CONTRAST_COLOR_5700K     	DEF_CONTRAST
  
  #define DEF_RED_COLOR_5000K     		       128
  #define DEF_GREEN_COLOR_5000K   		116
  #define DEF_BLUE_COLOR_5000K    		106
  #define DEF_BRIGHTNESS_COLOR_5000K    	DEF_BRIGHTNESS
  #define DEF_CONTRAST_COLOR_5000K     	DEF_CONTRAST
  
  #define DEF_RED_COLOR_7500K     		       128
  #define DEF_GREEN_COLOR_7500K   		121
  #define DEF_BLUE_COLOR_7500K    		114
  #define DEF_BRIGHTNESS_COLOR_7500K    	DEF_BRIGHTNESS
  #define DEF_CONTRAST_COLOR_7500K     	DEF_CONTRAST
  
  #define DEF_RED_COLOR_10000K    		112
  #define DEF_GREEN_COLOR_10000K  		103
  #define DEF_BLUE_COLOR_10000K   		130
  #define DEF_BRIGHTNESS_COLOR_10000K    DEF_BRIGHTNESS
  #define DEF_CONTRAST_COLOR_10000K     	DEF_CONTRAST
  #endif

  #elif (PanelType==PanelLSM315HP01)
  #define DEF_RED_COLOR_SRGB      		       128
  #define DEF_GREEN_COLOR_SRGB    		128
  #define DEF_BLUE_COLOR_SRGB     		       128
  #define DEF_BRIGHTNESS_COLOR_SRGB   	DEF_BRIGHTNESS
  #define DEF_CONTRAST_COLOR_SRGB     	DEF_CONTRAST
  
  #define DEF_RED_COLOR_9300K     		109
  #define DEF_GREEN_COLOR_9300K   		109
  #define DEF_BLUE_COLOR_9300K    		128
  #define DEF_BRIGHTNESS_COLOR_9300K    	DEF_BRIGHTNESS
  #define DEF_CONTRAST_COLOR_9300K     	DEF_CONTRAST
  
  #define DEF_RED_COLOR_6500K     		128
  #define DEF_GREEN_COLOR_6500K   		128
  #define DEF_BLUE_COLOR_6500K    		128
  #define DEF_BRIGHTNESS_COLOR_6500K    	DEF_BRIGHTNESS
  #define DEF_CONTRAST_COLOR_6500K     	DEF_CONTRAST
  
  #define DEF_RED_COLOR_5700K     		128
  #define DEF_GREEN_COLOR_5700K   		118
  #define DEF_BLUE_COLOR_5700K    		106
  #define DEF_BRIGHTNESS_COLOR_5700K    	DEF_BRIGHTNESS
  #define DEF_CONTRAST_COLOR_5700K     	DEF_CONTRAST
  
  #define DEF_RED_COLOR_5000K     		       128
  #define DEF_GREEN_COLOR_5000K   		116
  #define DEF_BLUE_COLOR_5000K    		106
  #define DEF_BRIGHTNESS_COLOR_5000K    	DEF_BRIGHTNESS
  #define DEF_CONTRAST_COLOR_5000K     	DEF_CONTRAST
  
  #define DEF_RED_COLOR_7500K     		       128
  #define DEF_GREEN_COLOR_7500K   		121
  #define DEF_BLUE_COLOR_7500K    		114
  #define DEF_BRIGHTNESS_COLOR_7500K    	DEF_BRIGHTNESS
  #define DEF_CONTRAST_COLOR_7500K     	DEF_CONTRAST
  
  #define DEF_RED_COLOR_10000K    		112
  #define DEF_GREEN_COLOR_10000K  		103
  #define DEF_BLUE_COLOR_10000K   		130
  #define DEF_BRIGHTNESS_COLOR_10000K    DEF_BRIGHTNESS
  #define DEF_CONTRAST_COLOR_10000K     	DEF_CONTRAST
  #elif (PanelType==PanelLSM270HP06)
  #define DEF_RED_COLOR_SRGB      		       128
  #define DEF_GREEN_COLOR_SRGB    		128
  #define DEF_BLUE_COLOR_SRGB     		       128
  #define DEF_BRIGHTNESS_COLOR_SRGB   	DEF_BRIGHTNESS
  #define DEF_CONTRAST_COLOR_SRGB     	DEF_CONTRAST
  
  #define DEF_RED_COLOR_9300K     		116
  #define DEF_GREEN_COLOR_9300K   		111
  #define DEF_BLUE_COLOR_9300K    		128
  #define DEF_BRIGHTNESS_COLOR_9300K    	DEF_BRIGHTNESS
  #define DEF_CONTRAST_COLOR_9300K     	DEF_CONTRAST
  
  #define DEF_RED_COLOR_6500K     		128
  #define DEF_GREEN_COLOR_6500K   		128//121
  #define DEF_BLUE_COLOR_6500K    		128//124
  #define DEF_BRIGHTNESS_COLOR_6500K    	DEF_BRIGHTNESS
  #define DEF_CONTRAST_COLOR_6500K     	DEF_CONTRAST
  
  #define DEF_RED_COLOR_5700K     		129
  #define DEF_GREEN_COLOR_5700K   		112
  #define DEF_BLUE_COLOR_5700K    		103
  #define DEF_BRIGHTNESS_COLOR_5700K    	DEF_BRIGHTNESS
  #define DEF_CONTRAST_COLOR_5700K     	DEF_CONTRAST
  
  #define DEF_RED_COLOR_5000K     		       128
  #define DEF_GREEN_COLOR_5000K   		116
  #define DEF_BLUE_COLOR_5000K    		106
  #define DEF_BRIGHTNESS_COLOR_5000K    	DEF_BRIGHTNESS
  #define DEF_CONTRAST_COLOR_5000K     	DEF_CONTRAST
  
  #define DEF_RED_COLOR_7500K     		       128
  #define DEF_GREEN_COLOR_7500K   		121
  #define DEF_BLUE_COLOR_7500K    		114
  #define DEF_BRIGHTNESS_COLOR_7500K    	DEF_BRIGHTNESS
  #define DEF_CONTRAST_COLOR_7500K     	DEF_CONTRAST
  
  #define DEF_RED_COLOR_10000K    		112
  #define DEF_GREEN_COLOR_10000K  		103
  #define DEF_BLUE_COLOR_10000K   		130
  #define DEF_BRIGHTNESS_COLOR_10000K    DEF_BRIGHTNESS
  #define DEF_CONTRAST_COLOR_10000K     	DEF_CONTRAST
  #else
  #define DEF_RED_COLOR_SRGB      		       128
  #define DEF_GREEN_COLOR_SRGB    		128
  #define DEF_BLUE_COLOR_SRGB     		       128
  #define DEF_BRIGHTNESS_COLOR_SRGB   	DEF_BRIGHTNESS
  #define DEF_CONTRAST_COLOR_SRGB     	DEF_CONTRAST
  
  #define DEF_RED_COLOR_9300K     		       112
  #define DEF_GREEN_COLOR_9300K   		109
  #define DEF_BLUE_COLOR_9300K    		128
  #define DEF_BRIGHTNESS_COLOR_9300K    	DEF_BRIGHTNESS
  #define DEF_CONTRAST_COLOR_9300K     	DEF_CONTRAST
  
  #define DEF_RED_COLOR_6500K     		       128
  #define DEF_GREEN_COLOR_6500K   		128//121
  #define DEF_BLUE_COLOR_6500K    		128//124
  #define DEF_BRIGHTNESS_COLOR_6500K    	DEF_BRIGHTNESS
  #define DEF_CONTRAST_COLOR_6500K     	DEF_CONTRAST
  
  #define DEF_RED_COLOR_5700K     		128
  #define DEF_GREEN_COLOR_5700K   		128
  #define DEF_BLUE_COLOR_5700K    		128
  #define DEF_BRIGHTNESS_COLOR_5700K    	DEF_BRIGHTNESS
  #define DEF_CONTRAST_COLOR_5700K     	DEF_CONTRAST
  
  #define DEF_RED_COLOR_5000K     		       128
  #define DEF_GREEN_COLOR_5000K   		116
  #define DEF_BLUE_COLOR_5000K    		106
  #define DEF_BRIGHTNESS_COLOR_5000K    	DEF_BRIGHTNESS
  #define DEF_CONTRAST_COLOR_5000K     	DEF_CONTRAST
  
  #define DEF_RED_COLOR_7500K     		       128
  #define DEF_GREEN_COLOR_7500K   		121
  #define DEF_BLUE_COLOR_7500K    		114
  #define DEF_BRIGHTNESS_COLOR_7500K    	DEF_BRIGHTNESS
  #define DEF_CONTRAST_COLOR_7500K     	DEF_CONTRAST
  
  #define DEF_RED_COLOR_10000K    		112
  #define DEF_GREEN_COLOR_10000K  		103
  #define DEF_BLUE_COLOR_10000K   		130
  #define DEF_BRIGHTNESS_COLOR_10000K    DEF_BRIGHTNESS
  #define DEF_CONTRAST_COLOR_10000K     	DEF_CONTRAST
  #endif

#elif (defined(ZhongXinKeJi_Project))
  #define DEF_RED_COLOR_SRGB      		       128
  #define DEF_GREEN_COLOR_SRGB    		128
  #define DEF_BLUE_COLOR_SRGB     		       128
  #define DEF_BRIGHTNESS_COLOR_SRGB   	DEF_BRIGHTNESS
  #define DEF_CONTRAST_COLOR_SRGB     	DEF_CONTRAST
  
  #define DEF_RED_COLOR_9300K     		109
  #define DEF_GREEN_COLOR_9300K   		109
  #define DEF_BLUE_COLOR_9300K    		128
  #define DEF_BRIGHTNESS_COLOR_9300K    	DEF_BRIGHTNESS
  #define DEF_CONTRAST_COLOR_9300K     	DEF_CONTRAST
  
  #define DEF_RED_COLOR_6500K     		128
  #define DEF_GREEN_COLOR_6500K   		128
  #define DEF_BLUE_COLOR_6500K    		128
  #define DEF_BRIGHTNESS_COLOR_6500K    	DEF_BRIGHTNESS
  #define DEF_CONTRAST_COLOR_6500K     	DEF_CONTRAST
  
  #define DEF_RED_COLOR_5700K     		128
  #define DEF_GREEN_COLOR_5700K   		118
  #define DEF_BLUE_COLOR_5700K    		106
  #define DEF_BRIGHTNESS_COLOR_5700K    	DEF_BRIGHTNESS
  #define DEF_CONTRAST_COLOR_5700K     	DEF_CONTRAST
  
  #define DEF_RED_COLOR_5000K     		       128
  #define DEF_GREEN_COLOR_5000K   		116
  #define DEF_BLUE_COLOR_5000K    		106
  #define DEF_BRIGHTNESS_COLOR_5000K    	DEF_BRIGHTNESS
  #define DEF_CONTRAST_COLOR_5000K     	DEF_CONTRAST
  
  #define DEF_RED_COLOR_7500K     		       128
  #define DEF_GREEN_COLOR_7500K   		121
  #define DEF_BLUE_COLOR_7500K    		114
  #define DEF_BRIGHTNESS_COLOR_7500K    	DEF_BRIGHTNESS
  #define DEF_CONTRAST_COLOR_7500K     	DEF_CONTRAST
  
  #define DEF_RED_COLOR_10000K    		112
  #define DEF_GREEN_COLOR_10000K  		103
  #define DEF_BLUE_COLOR_10000K   		130
  #define DEF_BRIGHTNESS_COLOR_10000K    DEF_BRIGHTNESS
  #define DEF_CONTRAST_COLOR_10000K     	DEF_CONTRAST

#elif (defined(JieBo_Project))
#if (PanelType==PanelM270QAN01)
#define DEF_RED_COLOR_SRGB      		       128
#define DEF_GREEN_COLOR_SRGB    		128
#define DEF_BLUE_COLOR_SRGB     		       128
#define DEF_BRIGHTNESS_COLOR_SRGB   	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_SRGB     	DEF_CONTRAST

#define DEF_RED_COLOR_9300K     		       112
#define DEF_GREEN_COLOR_9300K   		112
#define DEF_BLUE_COLOR_9300K    		129
#define DEF_BRIGHTNESS_COLOR_9300K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_9300K     	DEF_CONTRAST

#define DEF_RED_COLOR_6500K     		       125
#define DEF_GREEN_COLOR_6500K   		121
#define DEF_BLUE_COLOR_6500K    		125
#define DEF_BRIGHTNESS_COLOR_6500K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_6500K     	DEF_CONTRAST

#define DEF_RED_COLOR_5700K     		       128
#define DEF_GREEN_COLOR_5700K   		127
#define DEF_BLUE_COLOR_5700K    		116
#define DEF_BRIGHTNESS_COLOR_5700K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_5700K     	DEF_CONTRAST

#define DEF_RED_COLOR_5000K     		       128
#define DEF_GREEN_COLOR_5000K   		116
#define DEF_BLUE_COLOR_5000K    		106
#define DEF_BRIGHTNESS_COLOR_5000K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_5000K     	DEF_CONTRAST

#define DEF_RED_COLOR_7500K     		       128
#define DEF_GREEN_COLOR_7500K   		121
#define DEF_BLUE_COLOR_7500K    		114
#define DEF_BRIGHTNESS_COLOR_7500K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_7500K     	DEF_CONTRAST

#define DEF_RED_COLOR_10000K    		112
#define DEF_GREEN_COLOR_10000K  		103
#define DEF_BLUE_COLOR_10000K   		130
#define DEF_BRIGHTNESS_COLOR_10000K    DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_10000K     	DEF_CONTRAST
#else
#define DEF_RED_COLOR_SRGB      		       128
#define DEF_GREEN_COLOR_SRGB    		128
#define DEF_BLUE_COLOR_SRGB     		       128
#define DEF_BRIGHTNESS_COLOR_SRGB   	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_SRGB     	DEF_CONTRAST

#define DEF_RED_COLOR_9300K     		       112
#define DEF_GREEN_COLOR_9300K   		109
#define DEF_BLUE_COLOR_9300K    		128
#define DEF_BRIGHTNESS_COLOR_9300K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_9300K     	DEF_CONTRAST

#define DEF_RED_COLOR_6500K     		       128
#define DEF_GREEN_COLOR_6500K   		121
#define DEF_BLUE_COLOR_6500K    		124
#define DEF_BRIGHTNESS_COLOR_6500K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_6500K     	DEF_CONTRAST

#define DEF_RED_COLOR_5700K     		       128
#define DEF_GREEN_COLOR_5700K   		119
#define DEF_BLUE_COLOR_5700K    		110
#define DEF_BRIGHTNESS_COLOR_5700K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_5700K     	DEF_CONTRAST

#define DEF_RED_COLOR_5000K     		       128
#define DEF_GREEN_COLOR_5000K   		116
#define DEF_BLUE_COLOR_5000K    		106
#define DEF_BRIGHTNESS_COLOR_5000K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_5000K     	DEF_CONTRAST

#define DEF_RED_COLOR_7500K     		       128
#define DEF_GREEN_COLOR_7500K   		121
#define DEF_BLUE_COLOR_7500K    		114
#define DEF_BRIGHTNESS_COLOR_7500K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_7500K     	DEF_CONTRAST

#define DEF_RED_COLOR_10000K    		112
#define DEF_GREEN_COLOR_10000K  		103
#define DEF_BLUE_COLOR_10000K   		130
#define DEF_BRIGHTNESS_COLOR_10000K    DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_10000K     	DEF_CONTRAST
#endif
#elif (defined(XinZuoEr_Project))
#define DEF_RED_COLOR_SRGB      		       128
#define DEF_GREEN_COLOR_SRGB    		128
#define DEF_BLUE_COLOR_SRGB     		       128
#define DEF_BRIGHTNESS_COLOR_SRGB   	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_SRGB     	DEF_CONTRAST

#define DEF_RED_COLOR_9300K     		       112
#define DEF_GREEN_COLOR_9300K   		109
#define DEF_BLUE_COLOR_9300K    		128
#define DEF_BRIGHTNESS_COLOR_9300K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_9300K     	DEF_CONTRAST

#define DEF_RED_COLOR_6500K     		       123
#define DEF_GREEN_COLOR_6500K   		115
#define DEF_BLUE_COLOR_6500K    		125
#define DEF_BRIGHTNESS_COLOR_6500K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_6500K     	DEF_CONTRAST

#define DEF_RED_COLOR_5700K     		       128
#define DEF_GREEN_COLOR_5700K   		119
#define DEF_BLUE_COLOR_5700K    		110
#define DEF_BRIGHTNESS_COLOR_5700K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_5700K     	DEF_CONTRAST

#define DEF_RED_COLOR_5000K     		       128
#define DEF_GREEN_COLOR_5000K   		116
#define DEF_BLUE_COLOR_5000K    		106
#define DEF_BRIGHTNESS_COLOR_5000K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_5000K     	DEF_CONTRAST

#define DEF_RED_COLOR_7500K     		       128
#define DEF_GREEN_COLOR_7500K   		121
#define DEF_BLUE_COLOR_7500K    		114
#define DEF_BRIGHTNESS_COLOR_7500K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_7500K     	DEF_CONTRAST

#define DEF_RED_COLOR_10000K    		112
#define DEF_GREEN_COLOR_10000K  		103
#define DEF_BLUE_COLOR_10000K   		130
#define DEF_BRIGHTNESS_COLOR_10000K    DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_10000K     	DEF_CONTRAST
#elif (defined(KTC_Project))
#define DEF_RED_COLOR_SRGB      		       128
#define DEF_GREEN_COLOR_SRGB    		128
#define DEF_BLUE_COLOR_SRGB     		       128
#define DEF_BRIGHTNESS_COLOR_SRGB   	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_SRGB     	DEF_CONTRAST

#define DEF_RED_COLOR_9300K     		       106
#define DEF_GREEN_COLOR_9300K   		102
#define DEF_BLUE_COLOR_9300K    		128
#define DEF_BRIGHTNESS_COLOR_9300K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_9300K     	DEF_CONTRAST

#define DEF_RED_COLOR_6500K     		       113
#define DEF_GREEN_COLOR_6500K   		108
#define DEF_BLUE_COLOR_6500K    		124
#define DEF_BRIGHTNESS_COLOR_6500K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_6500K     	DEF_CONTRAST

#define DEF_RED_COLOR_5700K     		       115
#define DEF_GREEN_COLOR_5700K   		105
#define DEF_BLUE_COLOR_5700K    		114
#define DEF_BRIGHTNESS_COLOR_5700K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_5700K     	DEF_CONTRAST

#define DEF_RED_COLOR_5000K     		       128
#define DEF_GREEN_COLOR_5000K   		116
#define DEF_BLUE_COLOR_5000K    		106
#define DEF_BRIGHTNESS_COLOR_5000K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_5000K     	DEF_CONTRAST

#define DEF_RED_COLOR_7500K     		       128
#define DEF_GREEN_COLOR_7500K   		121
#define DEF_BLUE_COLOR_7500K    		114
#define DEF_BRIGHTNESS_COLOR_7500K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_7500K     	DEF_CONTRAST

#define DEF_RED_COLOR_10000K    		112
#define DEF_GREEN_COLOR_10000K  		103
#define DEF_BLUE_COLOR_10000K   		130
#define DEF_BRIGHTNESS_COLOR_10000K    DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_10000K     	DEF_CONTRAST
#elif (defined(HongTian_Project))
#define DEF_RED_COLOR_SRGB      		       128
#define DEF_GREEN_COLOR_SRGB    		128
#define DEF_BLUE_COLOR_SRGB     		       128
#define DEF_BRIGHTNESS_COLOR_SRGB   	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_SRGB     	DEF_CONTRAST
// 冷色98 90 95
#define DEF_RED_COLOR_9300K     		       98
#define DEF_GREEN_COLOR_9300K   		90
#define DEF_BLUE_COLOR_9300K    		95
#define DEF_BRIGHTNESS_COLOR_9300K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_9300K     	DEF_CONTRAST

#define DEF_RED_COLOR_6500K     		       128
#define DEF_GREEN_COLOR_6500K   		121
#define DEF_BLUE_COLOR_6500K    		124
#define DEF_BRIGHTNESS_COLOR_6500K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_6500K     	DEF_CONTRAST
//暖色 128 116 100  
#define DEF_RED_COLOR_5700K     		       128
#define DEF_GREEN_COLOR_5700K   		116
#define DEF_BLUE_COLOR_5700K    		100
#define DEF_BRIGHTNESS_COLOR_5700K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_5700K     	DEF_CONTRAST

#define DEF_RED_COLOR_5000K     		       128
#define DEF_GREEN_COLOR_5000K   		116
#define DEF_BLUE_COLOR_5000K    		106
#define DEF_BRIGHTNESS_COLOR_5000K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_5000K     	DEF_CONTRAST

#define DEF_RED_COLOR_7500K     		       128
#define DEF_GREEN_COLOR_7500K   		121
#define DEF_BLUE_COLOR_7500K    		114
#define DEF_BRIGHTNESS_COLOR_7500K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_7500K     	DEF_CONTRAST

#define DEF_RED_COLOR_10000K    		112
#define DEF_GREEN_COLOR_10000K  		103
#define DEF_BLUE_COLOR_10000K   		130
#define DEF_BRIGHTNESS_COLOR_10000K    DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_10000K     	DEF_CONTRAST

#elif (defined(MingCai_Project))

#define DEF_RED_COLOR_SRGB      		       128
#define DEF_GREEN_COLOR_SRGB    		128
#define DEF_BLUE_COLOR_SRGB     		       128
#define DEF_BRIGHTNESS_COLOR_SRGB   	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_SRGB     	DEF_CONTRAST

#define DEF_RED_COLOR_9300K     		123
#define DEF_GREEN_COLOR_9300K   		109
#define DEF_BLUE_COLOR_9300K    		120
#define DEF_BRIGHTNESS_COLOR_9300K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_9300K     	DEF_CONTRAST

#define DEF_RED_COLOR_6500K     		123
#define DEF_GREEN_COLOR_6500K   		119
#define DEF_BLUE_COLOR_6500K    		120
#define DEF_BRIGHTNESS_COLOR_6500K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_6500K     	DEF_CONTRAST

#define DEF_RED_COLOR_5700K     		128
#define DEF_GREEN_COLOR_5700K   		119
#define DEF_BLUE_COLOR_5700K    		110
#define DEF_BRIGHTNESS_COLOR_5700K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_5700K     	DEF_CONTRAST

#define DEF_RED_COLOR_5000K     		       128
#define DEF_GREEN_COLOR_5000K   		116
#define DEF_BLUE_COLOR_5000K    		106
#define DEF_BRIGHTNESS_COLOR_5000K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_5000K     	DEF_CONTRAST

#define DEF_RED_COLOR_7500K     		       128
#define DEF_GREEN_COLOR_7500K   		121
#define DEF_BLUE_COLOR_7500K    		114
#define DEF_BRIGHTNESS_COLOR_7500K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_7500K     	DEF_CONTRAST

#define DEF_RED_COLOR_10000K    		112
#define DEF_GREEN_COLOR_10000K  		103
#define DEF_BLUE_COLOR_10000K   		130
#define DEF_BRIGHTNESS_COLOR_10000K    DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_10000K     	DEF_CONTRAST

#else
#define DEF_RED_COLOR_SRGB      		       128
#define DEF_GREEN_COLOR_SRGB    		128
#define DEF_BLUE_COLOR_SRGB     		       128
#define DEF_BRIGHTNESS_COLOR_SRGB   	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_SRGB     	DEF_CONTRAST

#define DEF_RED_COLOR_9300K     		       112
#define DEF_GREEN_COLOR_9300K   		109
#define DEF_BLUE_COLOR_9300K    		128
#define DEF_BRIGHTNESS_COLOR_9300K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_9300K     	DEF_CONTRAST

#define DEF_RED_COLOR_6500K     		       128
#define DEF_GREEN_COLOR_6500K   		121
#define DEF_BLUE_COLOR_6500K    		124
#define DEF_BRIGHTNESS_COLOR_6500K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_6500K     	DEF_CONTRAST

#define DEF_RED_COLOR_5700K     		       128
#define DEF_GREEN_COLOR_5700K   		119
#define DEF_BLUE_COLOR_5700K    		110
#define DEF_BRIGHTNESS_COLOR_5700K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_5700K     	DEF_CONTRAST

#define DEF_RED_COLOR_5000K     		       128
#define DEF_GREEN_COLOR_5000K   		116
#define DEF_BLUE_COLOR_5000K    		106
#define DEF_BRIGHTNESS_COLOR_5000K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_5000K     	DEF_CONTRAST

#define DEF_RED_COLOR_7500K     		       128
#define DEF_GREEN_COLOR_7500K   		121
#define DEF_BLUE_COLOR_7500K    		114
#define DEF_BRIGHTNESS_COLOR_7500K    	DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_7500K     	DEF_CONTRAST

#define DEF_RED_COLOR_10000K    		112
#define DEF_GREEN_COLOR_10000K  		103
#define DEF_BLUE_COLOR_10000K   		130
#define DEF_BRIGHTNESS_COLOR_10000K    DEF_BRIGHTNESS
#define DEF_CONTRAST_COLOR_10000K     	DEF_CONTRAST
#endif

///Color Setting Menu====================================

//#define DEF_COLOR_PRESET_MODE   CPM_DESKTOP

#define LedColorful                               0x07//

#define DEF_LED_MODE                        LedRed
#define DEF_LED_STRENGTH                LedMax
#define DEF_LED_MODE_TYPE              LedBreath
#define LED_Number             1

#define DEF_LED_MODE_INDEX_TYPE       LedModeIndex_Off


#define DEF_LEDMODE              GreenLedMode

#if ENABLE_DCR
#define DEF_ADAPTIVE_CONTRAST   _OFF_
#endif

#if (defined(LianHeChuangXin_Project))
#if (AIO_Board||HKC_RGB_Range_Func)
#define OSD_INPUT_COLOR_RANGE_DEF   OSD_INPUT_COLOR_RANGE_FULL
#else
#define OSD_INPUT_COLOR_RANGE_DEF   OSD_INPUT_COLOR_RANGE_FULL//OSD_INPUT_COLOR_RANGE_LIMITED
#endif
#else
#define OSD_INPUT_COLOR_RANGE_DEF   OSD_INPUT_COLOR_RANGE_FULL
#endif

///OSD Setting Menu=======================================
#if 0//(defined(FengYuan_Project))
#define DEF_TRANSPARENCY        0
#define MAX_TRANSPARENCY        5
#else
#define DEF_TRANSPARENCY        1
#define MAX_TRANSPARENCY        4
#endif
#if 1//(defined(GK3B_Project))
#define DEF_OSD_H_POSITION      100
#define DEF_OSD_V_POSITION      0
#else
#define DEF_OSD_H_POSITION      50
#define DEF_OSD_V_POSITION      50
#endif

#define DEF_OSD_TIMEOUT         30
#define MAX_OSD_TIMEOUT         60
#define MIN_OSD_TIMEOUT         10//5

#define DEF_OSD_LOCK            FALSE
#define DEF_MENU_ROTATION       FALSE

#if 1//ENABLE_WIDE_MODE
#define DEF_WIDE_MODE           eASPECT_FULL
#endif
#if 1//ENABLE_MULTI_WINDOW_SETTINGS_MENU
#define DEF_MULTIWINW_MODE      eMULTIWIN_OFF
#define DEF_MULTIWIN_SEL        DISPLAY_WIN1
#define DEF_PIP_SIZE            ePIPSIZE_MIDDLE
#define DEF_PIP_H_POSITION      0
#define DEF_PIP_V_POSITION      0//100
#endif


#define DEF_SLEEPMODE       SleepMode_Off

///==================================================

#define DEF_DPTYPE        DP12

///==================================================

#define DEF_HDMITYPE        HDMI20

///Audio Menu===========================================
#if AudioFunc
#define MinVolumeValue          160
#define MaxVolumeValue          255
#endif

#define DEF_POWER_SAVE_AUDIO    _OFF  //default: disable mute
#define MAX_VOLUME              496

#if (MS_BOARD_TYPE_SEL==BD_MST9UHD_DV1)
#define DEF_AUDIOSOURCE_0		(AIDIOSOURCE_DP1)//(AIDIOSOURCE_LINE_IN)
#define DEF_AUDIOSOURCE_1		(AIDIOSOURCE_HDMI1)//(AIDIOSOURCE_HDMI1)
#define DEF_AUDIOSOURCE_2		(AIDIOSOURCE_HDMI2)//(AIDIOSOURCE_HDMI2)
#define DEF_AUDIOSOURCE_3		(AIDIOSOURCE_LINE_IN)//(AIDIOSOURCE_DP1)
#define DEF_AUDIOSOURCE_4		(AIDIOSOURCE_HDMI3)
#define DEF_AUDIOSOURCE_5		(AIDIOSOURCE_LINE_IN)
#elif (MS_BOARD_TYPE_SEL==BD_MST9UHD_BV1)
#define DEF_AUDIOSOURCE_0		(AIDIOSOURCE_HDMI1)
#define DEF_AUDIOSOURCE_1		(AIDIOSOURCE_HDMI2)
#define DEF_AUDIOSOURCE_2		(AIDIOSOURCE_HDMI3)
#define DEF_AUDIOSOURCE_3		(AIDIOSOURCE_DP1)
#define DEF_AUDIOSOURCE_4		(AIDIOSOURCE_HDMI3)
#define DEF_AUDIOSOURCE_5		(AIDIOSOURCE_LINE_IN)
#elif (MS_BOARD_TYPE_SEL==BD_MST9UHD_GV1)
#define DEF_AUDIOSOURCE_0		(AIDIOSOURCE_DP1)
#define DEF_AUDIOSOURCE_1		(AIDIOSOURCE_HDMI1)
#define DEF_AUDIOSOURCE_2		(AIDIOSOURCE_HDMI2)
#define DEF_AUDIOSOURCE_3		(AIDIOSOURCE_HDMI1)
#define DEF_AUDIOSOURCE_4		(AIDIOSOURCE_HDMI1)
#define DEF_AUDIOSOURCE_5		(AIDIOSOURCE_LINE_IN)
#else
#define DEF_AUDIOSOURCE_0		(AIDIOSOURCE_LINE_IN)
#define DEF_AUDIOSOURCE_1		(AIDIOSOURCE_HDMI1)
#define DEF_AUDIOSOURCE_2		(AIDIOSOURCE_DP1)
#define DEF_AUDIOSOURCE_3		(AIDIOSOURCE_HDMI2)
#define DEF_AUDIOSOURCE_4		(AIDIOSOURCE_HDMI3)
#define DEF_AUDIOSOURCE_5		(AIDIOSOURCE_LINE_IN)
#endif

///Factory Reset Menu======================================
#define DEF_DDCCI_SETTING       _ON_


///MISC.SETTING=========================================
#define DEF_DLC_MODE            OFF_DLC
#if Enable_AutoInput_Menu
#if (defined(MicroBoard_Project))
#define DEF_INPUT_PRIORITY      INPUT_PRIORITY_HDMI
#else
#define DEF_INPUT_PRIORITY      INPUT_PRIORITY_AUTO
#endif
#elif (MS_BOARD_TYPE_SEL== BD_MST9UHL_V4||MS_BOARD_TYPE_SEL==BD_MST9UHL_V6||MS_BOARD_TYPE_SEL==BD_MST9UHL_V7||MS_BOARD_TYPE_SEL==BD_MST9UHD_DV1||MS_BOARD_TYPE_SEL==BD_MST9UHD_GV1)
#if AIO_Board
#define DEF_INPUT_PRIORITY      INPUT_PRIORITY_HDMI
#else
#define DEF_INPUT_PRIORITY      INPUT_PRIORITY_HDMI//INPUT_PRIORITY_DVI
#endif
#elif (MS_BOARD_TYPE_SEL== BD_MST9UHL_V4_1H1DP)
#define DEF_INPUT_PRIORITY      INPUT_PRIORITY_HDMI
#else
#define DEF_INPUT_PRIORITY      INPUT_PRIORITY_VGA
#endif

#if (MS_BOARD_TYPE_SEL == BD_MST9UHL_A0)
#define DEF_INPUT_TYPE          INPUT_VGA
#define DEF_SUBWIN_2_INPUT       	(INPUT_DVI)
#define DEF_SUBWIN_3_INPUT       	(INPUT_HDMI)
#define DEF_SUBWIN_4_INPUT       	(INPUT_DISPLAYPORT)
#elif (MS_BOARD_TYPE_SEL == BD_MST9UHL_V3)
#define DEF_INPUT_TYPE          INPUT_VGA
#define DEF_SUBWIN_2_INPUT       	(INPUT_HDMI)
#define DEF_SUBWIN_3_INPUT       	(INPUT_HDMI2ND)
#define DEF_SUBWIN_4_INPUT       	(INPUT_DISPLAYPORT)
#elif (MS_BOARD_TYPE_SEL== BD_MST9UHL_V4||MS_BOARD_TYPE_SEL==BD_MST9UHL_V6||MS_BOARD_TYPE_SEL==BD_MST9UHL_V7||MS_BOARD_TYPE_SEL==BD_MST9UHD_DV1)
#if AIO_Board
#define DEF_INPUT_TYPE          INPUT_HDMI
#define DEF_SUBWIN_2_INPUT       	(INPUT_HDMI)
#define DEF_SUBWIN_3_INPUT       	(INPUT_HDMI)
#define DEF_SUBWIN_4_INPUT       	(INPUT_HDMI)
#else
#define DEF_INPUT_TYPE          INPUT_DVI
#define DEF_SUBWIN_2_INPUT       	(INPUT_HDMI)
#define DEF_SUBWIN_3_INPUT       	(INPUT_DISPLAYPORT)
#define DEF_SUBWIN_4_INPUT       	(INPUT_DISPLAYPORT)
#endif
#elif (MS_BOARD_TYPE_SEL==BD_MST9UHL_V4_1H1DP)
#define DEF_INPUT_TYPE          (INPUT_HDMI)
#define DEF_SUBWIN_2_INPUT       	(INPUT_DISPLAYPORT)
#define DEF_SUBWIN_3_INPUT       	(INPUT_DISPLAYPORT)
#define DEF_SUBWIN_4_INPUT       	(INPUT_DISPLAYPORT)
#elif (MS_BOARD_TYPE_SEL==BD_MST9UHD_GV1)
#define DEF_INPUT_TYPE          INPUT_DISPLAYPORT
#define DEF_SUBWIN_2_INPUT       	(INPUT_HDMI)
#define DEF_SUBWIN_3_INPUT       	(INPUT_HDMI2ND)
#define DEF_SUBWIN_4_INPUT       	(INPUT_DISPLAYPORT)
#else
#define DEF_INPUT_TYPE          INPUT_VGA
#define DEF_SUBWIN_2_INPUT       	(INPUT_DVI)
#define DEF_SUBWIN_3_INPUT       	(INPUT_HDMI)
#define DEF_SUBWIN_4_INPUT       	(INPUT_DISPLAYPORT)
#endif

#define DEF_PIP_POSITION       	       	(PIPPosition_Top_Right)


//===============DEMO && DCR=================================
#define DEF_DEMO_HSTART					((WORD)PANEL_WIDTH/4)
#define DEF_DEMO_HEND						((WORD)PANEL_WIDTH*3/4)
#define DEF_DEMO_HPOS						(DEF_DEMO_HSTART+DEF_DEMO_HEND)
#ifdef SenYo_Project
#define DefBacklightOnCounter2min			(1*60)//(1*20)//(2*2)//(2*60)     //单2だ牧秈backlight off
#else
#define DefBacklightOnCounter2min			(2*60)//(1*20)//(2*2)//(2*60)     //单2だ牧秈backlight off
#endif

#define WhiteRange					80//200//100
#define BlackRange					10//16
#define MaxPixelValue				0x12
#define DCR_PixelCount                         ((g_u16Total_Count[0])?(g_u16Total_Count[0]):(1))
#define DCR_PixelWeight                      g_u16Total_Weight[0]
#define DCR_MaxPixel                          g_u8Ymax[0]

#define BRIGHTNESS_DCR_MAX    0xFF//2//0xFFFF/255
#define BRIGHTNESS_DCR_MIN    0

#if ENABLE_MULTI_WINDOW_SETTINGS_MENU
#define ENABLE_SmartConntrast()    ( (DCR_ENABLE_FLAG)&&(USER_PREF_MULTI_WIN_MODE == eMULTIWIN_OFF)&&(USER_PREF_WIN_SEL== DISPLAY_WIN1))
#else
#define ENABLE_SmartConntrast()    ( (DCR_ENABLE_FLAG)&&(USER_PREF_WIN_SEL== DISPLAY_WIN1))
#endif

#define SET_SMARTCONTRAST_BACKLIGHT_ON()		if(AdjDcrBacklightFlag&&DCR_Status == DCR_WAIT_EXIT_HIGH_CONT && (!BacklightFlag))\
												hw_SetBlacklit();

#define SET_SMARTCONTRAST_BACKLIGHT_OFF()	if(AdjDcrBacklightFlag&&DCR_Status == DCR_WAIT_EXIT_HIGH_CONT && BacklightFlag)\
												hw_ClrBlacklit();
//==========================================================

#if 1
  #if DEF_RED_GAIN<0x400
    #undef DEF_RED_GAIN
    #define DEF_RED_GAIN        0x1000
  #endif
  #if DEF_GREEN_GAIN<0x400
    #undef DEF_GREEN_GAIN
    #define DEF_GREEN_GAIN      0x1000
  #endif
  #if DEF_BLUE_GAIN<0x400
    #undef DEF_BLUE_GAIN
    #define DEF_BLUE_GAIN       0x1000
  #endif
  #if ADC_MAX_LIMIT<0x400
    #undef ADC_MAX_LIMIT
    #define ADC_MAX_LIMIT       0x3000
  #endif
  #if ADC_MIN_LIMIT<0x400
    #undef ADC_MIN_LIMIT
    #define ADC_MIN_LIMIT       0x200
  #endif
#else
    #ifdef _MISC_C
        #warning "Please Check auto color value.";
    #endif
#endif


#endif




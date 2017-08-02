#ifndef _BOARD_H_
#define _BOARD_H_
#include "chiptype.h"

////////////////////////////////////////////////////////////
// INPUT_TYPE DEFINE
////////////////////////////////////////////////////////////
#define INPUT_1A        BIT0    // 1A
#define INPUT_1C        BIT1    // digital portx1
#define INPUT_2C        BIT2    // digital portx2
#define INPUT_3C        BIT3    // digital portx3
#define INPUT_4C        BIT4    // digital portx4
#define INPUT_5C        BIT5    // digital portx5
#define INPUT_6C        BIT6    // digital portx6
#define INPUT_7C        BIT7    // digital portx7

//////////////////////////////////////////////////////////////
// KEY SELECTION
//////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// ODM_NAME DEFINE
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// FACTORYALIGN_TYPE DEFINE
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// CHIP SERIES DEFINE
////////////////////////////////////////////////////////////
#define MST9U                       0
#define MST9U2                      1
#define MST9U3                      2
////////////////////////////////////////////////////////////
// MAINBOARD TYPE DEFINE
////////////////////////////////////////////////////////////
#define BD_MST9U                    100 // TSUMX 1A?H?DP
#define BD_MST203A_A01A_S           101 //BD_MST9U3_BGA
#define BD_MST203B_A01A_S           102 //BD_MST9U3_QFP
#define BD_MST203A_A01B_S           103 //MST9U3 4HDMI
#define BD_MST203C_A01A_S           104 //BD_MST9U3_QFP, 2-layers
 
#define BD_MST9UHL_A0                   105
#define BD_MST9UHL_V3                   106
#define BD_MST9UHL_V4                  107
#define BD_MST9UHL_V6                  108
#define BD_MST9UHL_V4_1H1DP      109
#define BD_MST9UHL_V7                  110
#define BD_MST9UHD_BV1                111
#define BD_MST9UHD_DV1                112
#define BD_MST9UHD_GV1                113

//********************LogoType Define*******************************
//*****************************************************************
//*******************Language Define********************************
//*****************************************************************
//*****************************************************************
//#include "global.h"
#include "Model.h"
#include "DefaultCompilerOption.h"
//*****************************************************************
//*****************************************************************
#define Enable_Cache                    1
//*****************************************************************
//*****************************************************************
#include "reg52.h"
#if (MS_BOARD_TYPE_SEL == BD_MST9U)
#define USEFLASH    0
#include "bd_TSUMX.h"
#elif (MS_BOARD_TYPE_SEL == BD_MST203A_A01A_S)
#define USEFLASH    0
#include "bd_MST203A_A01A_S.h"
#elif (MS_BOARD_TYPE_SEL == BD_MST203A_A01B_S)
#define USEFLASH    0
#include "bd_MST203A_A01B_S.h"
#elif (MS_BOARD_TYPE_SEL == BD_MST203B_A01A_S)
#define USEFLASH    0
#include "bd_MST203B_A01A_S.h"
#elif (MS_BOARD_TYPE_SEL == BD_MST203C_A01A_S)
#define USEFLASH    0
#include "bd_MST203C_A01A_S.h"

#elif (MS_BOARD_TYPE_SEL == BD_MST9UHL_A0)
#define USEFLASH    0
#include "PM_BD_MST9UHL_A0.h"

#elif (MS_BOARD_TYPE_SEL == BD_MST9UHL_V3)
#define USEFLASH    0
#include "PM_BD_MST9UHL_V3.h"

#elif (MS_BOARD_TYPE_SEL == BD_MST9UHL_V4)
#define USEFLASH    0
#include "PM_BD_MST9UHL_V4.h"

#elif (MS_BOARD_TYPE_SEL == BD_MST9UHL_V4_1H1DP)
#define USEFLASH    0
#include "PM_BD_MST9UHL_V4_1H1DP.h"

#elif (MS_BOARD_TYPE_SEL == BD_MST9UHL_V6)
#define USEFLASH    0
#include "PM_BD_MST9UHL_V6.h"

#elif (MS_BOARD_TYPE_SEL==BD_MST9UHL_V7)
#define USEFLASH    0
#include "PM_BD_MST9UHL_V7.h"

#elif (MS_BOARD_TYPE_SEL == BD_MST9UHD_BV1)
#define USEFLASH    0
#include "PM_BD_MST9UHD_BV1.h"

#elif (MS_BOARD_TYPE_SEL == BD_MST9UHD_DV1)
#define USEFLASH    0
#include "PM_BD_MST9UHD_DV1.h"

#elif (MS_BOARD_TYPE_SEL == BD_MST9UHD_GV1)
#define USEFLASH    0
#include "PM_BD_MST9UHD_GV1.h"



#endif

#define ENABLE_SW_DOUBLE_BUFFER   0

#ifndef ENABLE_HK_CODE_ON_DRAM
#define ENABLE_HK_CODE_ON_DRAM  0
#endif

#ifndef ENABLE_HK_CODE_ON_SPI_DRAM_PSRAM
#define ENABLE_HK_CODE_ON_SPI_DRAM_PSRAM        0
#endif

#ifndef MST_ASSERT
#define MST_ASSERT(_test_)
#endif

#ifndef EnableTime1Interrupt
#define EnableTime1Interrupt  1
#endif

#ifndef EnableTime2Interrupt
#define EnableTime2Interrupt  0
#endif

#ifndef ENABLE_TIME_MEASUREMENT
#define ENABLE_TIME_MEASUREMENT    0
#endif

#ifndef FPGA_PM
#define FPGA_PM     0
#endif

#ifndef FPGA_HW_PMMODE
#define FPGA_HW_PMMODE     0
#endif

#ifndef ENABLE_MBX
#define ENABLE_MBX              0 // need sync with R2 setting
#endif

#if ENABLE_MBX
#define ENABLE_MBX_SAR          1 // need sync with R2 setting
#else
#define ENABLE_MBX_SAR          0
#endif

#ifndef ENABLE_BUZZER
#define ENABLE_BUZZER               0
#endif

#ifndef ENABLE_R2_INT_M51_Test
#define ENABLE_R2_INT_M51_Test              0
#endif

#if ENABLE_BUZZER
#define BUZZER_PWM              _PWM1_GP0_  //_PWM0_GP0_~_PWM9_GP1_
#define BUZZER_HIGHFREQ_FREQ    4600
#define BUZZER_LOWFREQ_FREQ     3000
#define BUZZER_HIGH_FREQ        1920
#define BUZZER_LOW_FREQ         800
#define BUZZER_DUTY             0x7FFF
#define BUZZER_TIME             200
#endif

// ---- REMOTE SELECTION  ----------------------------------------------------------
#define DEBUG_IR_SW_MODE            0
#define NO_REMOTE                   0
#define TW_REMOTE                   1   //TV
#define MITSUBISHI_REMOTE           2
#define RC5_MAGNAVOX                3  //RC5
#define MSTAR_REMOTE                4  //NEC
#define RC6_PHILIPS                 5   //dont work
#define JRY_IR_MODE                       6

#define UNKNOW_REMOTE               0xFF

#if(IR_REMOTE_SEL != NO_REMOTE)
// IR mode selection
#define IR_SOFTWARE_MODE             0
#define IR_FULL_INT_NEC_MODE         1
#define IR_FULL_POL_NEC_MODE         2
#define IR_RAW_DATA_POL_MODE         3
#define IR_RAW_DATA_INT_MODE         4
#define IR_SW_RC5_MODE               5
#define IR_FULL_INT_MIT_MODE         6
#define IR_FULL_INT_RC5_EXT_MODE     7
#define IR_FULL_INT_RC5_MODE         8
#define IR_FULL_INT_RC6_HEAD888MS_MODE      9   //Header is double by other bit length
#define IR_FULL_INT_RC6_HEAD444MS_MODE     10  //Header is not double by other bit length

#if(IR_REMOTE_SEL==TW_REMOTE)
  #define IR_MODE_SEL           IR_FULL_POL_NEC_MODE//IR_FULL_INT_NEC_MODE//IR_SW_RC5_MODE//IR_FULL_POL_MODE//IR_FULL_INT_MODE
  #define IR_HEADER_CODE0       0x41
  #define IR_HEADER_CODE1       0x01
#elif(IR_REMOTE_SEL==MSTAR_REMOTE)
  #define IR_MODE_SEL           IR_RAW_DATA_INT_MODE//IR_SOFTWARE_MODE//IR_FULL_POL_NEC_MODE//IR_FULL_INT_NEC_MODE//IR_SW_RC5_MODE//IR_FULL_POL_MODE//IR_FULL_INT_MODE
  #define IR_HEADER_CODE0       0x80
  #define IR_HEADER_CODE1       0x7F
#elif(IR_REMOTE_SEL==RC5_MAGNAVOX)
  #define IR_MODE_SEL           IR_FULL_INT_RC5_EXT_MODE//IR_FULL_INT_NEC_MODE
  #define IR_RC5_ADDR1          0x03
  #define IR_RC5_ADDR2          0x00
#elif(IR_REMOTE_SEL==MITSUBISHI_REMOTE)
  #define IR_MODE_SEL           IR_FULL_INT_MIT_MODE//IR_FULL_INT_NEC_MODE
  #define IR_HEADER_CODE0       0xF4
  #define IR_HEADER_CODE1       0x00
  #define MIT_SEP_FROM_BIT      0x08
#elif(IR_REMOTE_SEL == RC6_PHILIPS)
  #define IR_MODE_SEL           IR_FULL_INT_RC6_HEAD444MS_MODE//IR_FULL_INT_NEC_MODE
  #define IR_RC6_ADDR          0x00
#elif(IR_REMOTE_SEL==JRY_IR_MODE)
#if (defined(LeShiYouXian_Project))
  #define IR_MODE_SEL           IR_RAW_DATA_POL_MODE
  #define IR_HEADER_CODE0       0xFF//0xCB
  #define IR_HEADER_CODE1       0x00//0x04
#elif (defined(Korean_Project))
  #define IR_MODE_SEL           IR_RAW_DATA_POL_MODE
  #define IR_HEADER_CODE0       0xF7//0xCB
  #define IR_HEADER_CODE1       0x08//0x04
#elif (defined(LeXingEnMa_Project)||LHCX_IR_Function_Enable)
  #define IR_MODE_SEL           IR_RAW_DATA_INT_MODE
  #define IR_HEADER_CODE0       0xFF
  #define IR_HEADER_CODE1       0x00
  
  #define IR_MODE_SEL_2
  #ifdef IR_MODE_SEL_2
  #define IR_HEADER_CODE2       0x04
  #define IR_HEADER_CODE3       0xFB
  #endif

  #define IR_MODE_SEL_3
  #ifdef IR_MODE_SEL_3
  #define IR_HEADER_CODE4       0x80
  #define IR_HEADER_CODE5       0xFF
  #endif
   #define IR_MODE_SEL_4
  #ifdef IR_MODE_SEL_4
  #define IR_HEADER_CODE6       0xAB
  #define IR_HEADER_CODE7       0xCD
  
  #define IR_HEADER_CODE6_2       0x86
  #define IR_HEADER_CODE7_2       0xE7
  #endif
  
  #define IR_MODE_SEL_5
  #ifdef IR_MODE_SEL_5
  #define IR_HEADER_CODE8_Low       0x02
  #define IR_HEADER_CODE9_High       0xFD
  #endif
  
#elif (defined(ZhouYang_Project))
  #define IR_MODE_SEL           IR_RAW_DATA_INT_MODE
  #define IR_HEADER_CODE0       0x00
  #define IR_HEADER_CODE1       0xFF
  
#elif (JieBo_IR_Function_Enable)
  #define IR_MODE_SEL           IR_RAW_DATA_INT_MODE
  #define IR_HEADER_CODE0       0x08
  #define IR_HEADER_CODE1       0xF7
  
  #define IR_MODE_SEL_4_Customer
  #ifdef IR_MODE_SEL_4_Customer
  #define IR_HEADER_CODE6_Customer       0x04
  #define IR_HEADER_CODE7_Customer       0x10
  #endif
  
 #elif (defined(TaiBei_Project))
  #define IR_MODE_SEL           IR_RAW_DATA_INT_MODE
  #define IR_HEADER_CODE0       0x08
  #define IR_HEADER_CODE1       0xF7
  
  #define IR_MODE_SEL_2
  #ifdef IR_MODE_SEL_2
  #define IR_HEADER_CODE2       0x01
  #define IR_HEADER_CODE3       0xFE
  #endif
  
   #define IR_MODE_SEL_3
  #ifdef IR_MODE_SEL_3
  #define IR_HEADER_CODE4       0x60 
  #define IR_HEADER_CODE5       0x9F
  #endif
 
 #elif (defined(HuaKe_Project))
  #define IR_MODE_SEL           IR_RAW_DATA_INT_MODE
  #define IR_HEADER_CODE0       0x08
  #define IR_HEADER_CODE1       0xF7
#elif 0//(defined(Japan_Project))
  #define IR_MODE_SEL           IR_RAW_DATA_INT_MODE
  #define IR_HEADER_CODE0       0x02
  #define IR_HEADER_CODE1       0xFD

#elif (defined(FengYuan_Project))
  #define IR_MODE_SEL           IR_RAW_DATA_INT_MODE
  #define IR_HEADER_CODE0       0x86
  #define IR_HEADER_CODE1       0xE7
  
  #define IR_MODE_SEL_2
  #ifdef IR_MODE_SEL_2
#define IR_HEADER_CODE2       0x04
#define IR_HEADER_CODE3       0xFB
  #endif
#elif (defined(KunDe_Project))
  #define IR_MODE_SEL           IR_RAW_DATA_INT_MODE
  #define IR_HEADER_CODE0       0x40
  #define IR_HEADER_CODE1       0xBF
  #define IR_MODE_SEL_2
 #ifdef IR_MODE_SEL_2
   #define IR_HEADER_CODE2       0x86
  #define IR_HEADER_CODE3       0x6B
  #endif
#elif (defined(ZhongChuangLianHe_Project))

  #define IR_MODE_SEL           IR_RAW_DATA_INT_MODE
  #define IR_HEADER_CODE0       0x00
  #define IR_HEADER_CODE1       0xFF
#elif (defined(Changjia_Project))
  #define IR_MODE_SEL           IR_RAW_DATA_INT_MODE
  #define IR_HEADER_CODE0       0x00
  #define IR_HEADER_CODE1       0xBF
#elif (defined(XuLi_Project))
  #define IR_MODE_SEL           IR_RAW_DATA_INT_MODE
  #define IR_HEADER_CODE0       0x02//0x86
  #define IR_HEADER_CODE1       0xFD//0xE7

#elif (defined(MingCai_Project))
  #define IR_MODE_SEL           IR_RAW_DATA_INT_MODE
  #define IR_HEADER_CODE0       0x08
  #define IR_HEADER_CODE1       0xF7
/*
#elif LHCX_IR_Function_Enable
  #define IR_MODE_SEL           IR_RAW_DATA_INT_MODE
  #define IR_HEADER_CODE0       0xFF
  #define IR_HEADER_CODE1       0x00
  
  #define IR_MODE_SEL_2
  #ifdef IR_MODE_SEL_2
  #define IR_HEADER_CODE2       0x04
  #define IR_HEADER_CODE3       0xFB
  #endif
*/
  
 #else
  #define IR_MODE_SEL           IR_RAW_DATA_POL_MODE
  #define IR_HEADER_CODE0       0xF7//0xCB
  #define IR_HEADER_CODE1       0x08//0x04
#endif    
#else
  #define IR_MODE_SEL           IR_FULL_INT_NEC_MODE//IR_FULL_INT_NEC_MODE
  #define IR_HEADER_CODE0       0x41
  #define IR_HEADER_CODE1       0x01
#endif
#endif  //(IR_REMOTE_SEL != NO_REMOTE)

// --------------------------------------------------------------------------------


#define CRYSTAL_CLOCK               12000000ul//14318000ul
#define WATCH_DOG_TIME_RESET        (10)   // need sync with R2 setting, unit: second, 0=off
#define WATCH_DOG_TIME_Interrupt    (3)   // need sync with R2 setting, unit: second
#define EXT_TIMER0_1MS              0       // external timer 0 for 1ms interrupt

// 110905 coding, addition for getting more V to reduce output dclk
#define INT_PERIOD              1// timer interrupt: unit = 1ms
#define SystemTick              1000    // 1 second
#define UseINT                  0//temp, after interrupt table being generated // 1
#define UsesRGB                 0

#define XTAL_CLOCK_KHZ          (12000ul)//(14318ul)
#define ClockPeriod             (0x10000-(((unsigned long)XTAL_CLOCK_KHZ*INT_PERIOD)/12))    // Interrupt every 1ms for Time 0
#define BaudRatePeriod          (0x10000-(((unsigned long)XTAL_CLOCK_KHZ * 1000+(DWORD)SERIAL_BAUD_RATE*16)/((DWORD)SERIAL_BAUD_RATE*32)))
#define T1BaudRatePeriod        (0x100-(((unsigned long)XTAL_CLOCK_KHZ * 1000+(DWORD)SERIAL_BAUD_RATE*192)/((DWORD)SERIAL_BAUD_RATE*384)))
#define HFreq(hPeriod)          ((hPeriod)?(((DWORD)XTAL_CLOCK_KHZ*10+hPeriod/2)/hPeriod):(1)) // 120330 coding modified to avoid divide 0
#define VFreq(hFreq, vTotal)    ((SrcVTotal)?(((DWORD)hFreq*1000+SrcVTotal/2)/SrcVTotal):(1)) // 120330 coding modified to avoid divide 0
#define MST_HPeriod_UINT        (XTAL_CLOCK_KHZ*16)
#define VFreqIn                 (((VFreq( HFreq( SrcHPeriod ), SrcVTotal )+5)/10) * ((SrcFlags&bInterlaceMode)?2:1))

#define PM_USE_EEPROM       0
#endif

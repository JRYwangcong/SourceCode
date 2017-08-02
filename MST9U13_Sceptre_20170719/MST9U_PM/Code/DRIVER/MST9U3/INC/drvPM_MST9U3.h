#ifndef MS_PM_H
#define MS_PM_H

#ifdef _MS_PM_C_
#define INTERFACE
#else
#define INTERFACE extern
#endif

////////////////////////////////
///// Enum MUST Sync with R2 /////
typedef enum
{
    ePM_POWER_ON,
    ePM_POWER_DPMS,
    ePM_POWER_DCOFF,

    ePM_INVAILD
}ePM_Mode;

typedef enum
{
    ePMSTS_NON,
    ePMSTS_VGA_ACT,
    ePMSTS_DVI_ACT,
    ePMSTS_SAR_ACT,
    ePMSTS_GPIO_ACT,
    
    ePMSTS_MCCS04_ACT,
    ePMSTS_MCCS05_ACT,
    ePMSTS_MCCS01_ACT,
    ePMSTS_CEC_ACT,
    ePMSTS_FORCE_ON,
    
    ePMSTS_DP_ACT,
    ePMSTS_MHL_ACT,
    ePMSTS_POWERGPIO_ACT,
    ePMSTS_CABLESAR_ACT,
    ePMSTS_FACTORY_ACT,
    
    ePMSTS_KEYWAKEUP_ACT,
    
    ePMSTS_INVAID
}ePM_WakeupStatus;


typedef enum
{
    ePM_VGA_HVSYNC,
    ePM_VGA_CSYNC,
    ePM_VGA_SOGSYNC
}ePM_SyncStatus;

///// Enum MUST Sync with R2 /////
////////////////////////////////


typedef enum
{
    ePMDVI_DVI0,
    ePMDVI_DVI1,
    ePMDVI_DVI0_DVI1,

    ePMDVI_INVALID
}ePM_DVI;


typedef enum
{
    ePMGPIO04_DectDVI5V,
    ePMGPIO02_DectDVI5V,
    ePMGPIO01_DectDVI5V,
    ePMGPIO00_DectDVI5V,
    ePMDVI5V_INVALID
} ePM_DVI5V;

typedef enum
{
    ePMSAR_ALIVE = 0,
    ePMSAR_SAR0 = BIT0,
    ePMSAR_SAR1 = BIT1,
    ePMSAR_SAR2 = BIT2,
    ePMSAR_SAR3 = BIT3,
    ePMSAR_SAR12 = BIT1|BIT2,
    ePMSAR_SAR123 = BIT1|BIT2|BIT3,
    ePMSAR_SAR01 = BIT0|BIT1,
    ePMSAR_SAR012 = BIT0|BIT1|BIT2,
    ePMSAR_INVALID
}ePM_SAR;

typedef enum
{
    ePM_ENTER_PM=0,
    ePM_EXIT_PM=1,
    ePM_WAIT_EVENT=2,
    ePM_IDLE=3,
} ePM_State;

typedef struct
{
    DWORD V_ID;
    WORD Phy_Addr;
    BYTE opCode[5];

} sPM_CEC;

typedef struct
{
    BYTE IrType;
    BYTE IrAddr;
    BYTE IrComp;

} sPM_IR;


typedef struct
{
    BYTE bHVSync_enable:1;
    BYTE bSOG_enable:1;
    BYTE bGPIO_enable:1;
    BYTE bSAR_enable:1;
    BYTE bMCCS_enable:1;
    BYTE bEDID_enable:1;
    BYTE bIR_enable:1;
    BYTE bCEC_enable:1;
#if( PM_SUPPORT_WAKEUP_DVI )
    BYTE bDVI_enable:1;
#endif  // end of #if( PM_SUPPORT_WAKEUP_DVI )
#if 1//( PM_SUPPORT_WAKEUP_DP )&&(ENABLE_DP_INPUT)
    BYTE bDP_enable:1;
#endif  // end of #if( PM_SUPPORT_WAKEUP_DP )
#if( PM_SUPPORT_AC2DC )
    BYTE bACtoDC_enable:1;
#endif  // end of #if( PM_SUPPORT_AC2DC )
    BYTE bMCUSleep:1;
    ePM_DVI ePMDVImode;
    ePM_SAR ePMSARmode;
    sPM_CEC sPMCECmode;
    sPM_IR sPMIRmode;
}sPM_Config;


typedef struct
{
  BYTE u8SAR_SingleKey;
  BYTE u8SAR_CmpLvl;

}sPM_SARSingleMode;


typedef struct
{
    BYTE ucPMMode;
    ePM_State ePMState;
    sPM_Config  sPMConfig;
    #if CABLE_DETECT_VGA_USE_SAR||CABLE_DETECT_VGA_USE_SAR
     BYTE bCABLE_SAR_VALUE;
    #endif

}sPM_Info;


typedef enum
{
    ePM_CLK_RCOSC,
    ePM_CLK_XTAL,

    ePMCLK_INVALID
}ePM_CLK;


//************ CEC relative ************//
#define CEC_SET_NUM				4

#define CEC_VER_11              0   //CEC1.1
#define CEC_VER_12              1   //CEC1.2
#define CEC_VER_12a             2   //CEC1.2a
#define CEC_VER_13              3   //CEC1.3
#define CEC_VER_13a             4   //CEC1.3a

#define HDMI_CEC_VER            CEC_VER_13a

#define CEC_RETRY_CNT			1
#define CEC_FRAME_INTERVAL      7
#define CEC_BUS_FREE_TIME       5
#define CEC_RESEND_INTERVAL     3

typedef enum _enMsCEC_DEV_LOGIC_ADDR_
{
    CEC_LA_TV               = 0,
    CEC_LA_RECORDER1        = 1,
    CEC_LA_RECORDER2        = 2,
    CEC_LA_TUNER1           = 3,
    CEC_LA_PLAYBACK_DEV1    = 4,
    CEC_LA_AUD_SYSTEM       = 5,
    CEC_LA_TUNER2           = 6,
    CEC_LA_TUNER3           = 7,
    CEC_LA_PLAYBACK_DEV2    = 8,
    CEC_LA_RECORDER3        = 9,
#if (HDMI_CEC_VER == CEC_VER_13a)
    CEC_LA_TUNER4           = 10,
    CEC_LA_PLAYBACK_DEV3    = 11,
#endif
    CEC_LA_FREE_USE         = 14,
    CEC_LA_UNREGISTERED     = 15,
    CEC_LA_BOARDCAST        = 15,
    CEC_LA_MAX              = 15,
} enMsCEC_DEV_LOGIC_ADDR;

//************ CEC relative ************//

#define ConvPercent(x)  ( (WORD)(x)*255/100)

// ACDET low voltage threshold // 0x66
#define Low_Volt_1_20 0x00<<2
#define Low_Volt_1_25 0x01<<2
#define Low_Volt_1_15 0x10<<2
#define Low_Volt_1_10 0x11<<2
// ACDET high voltage threshold // 0x66
#define High_Volt_1_40 0x00
#define High_Volt_1_45 0x01
#define High_Volt_1_50 0x10
#define High_Volt_1_55 0x11

//=============================
#define SAR_Ch          SAR2
//------------------------------------------------
#define SAR0     0
#define SAR1     1
#define SAR2     2
#define SAR3     3
//------------------------------------------------
#define SAR_KEY_TH      0x30

//=============================

#define LEDPWM              LEDPWM1
#define LED_PWM_EN      LED_PWM_Amber

#define PWMDUTY_Val     ConvPercent(2) //%
#define PWMDIV_Val          0xFF // 97.16Hz
//=============================
//#define SAR1   BIT0
//#define SAR2   BIT1
//#define SARALL SAR2|SAR1
//=============================
#define LEDPWM0 0
#define LEDPWM1 1
#define LEDPWM2 2
#define LEDPWM3 3
#define LED_PWM_SEL_P07     LEDPWM<<5
#define LED_PWM_SEL_P06     LEDPWM<<3  // P06 & P15 as same
#define LED_PWM_SEL     LED_PWM_SEL_P07|LED_PWM_SEL_P06

//define IR Type
#define IR_NEC   0
#define IR_RC5   1
#define IR_RC6   2



// PWM0 0xC2,0xC3 ; PWM1 0xC4,0xC5
#if (LEDPWM==LEDPWM1)
#define PWMDIV      0xC4
#define PWMDUTY   0xC5
#elif (LEDPWM==LEDPWM0)
#define PWMDIV      0xC2
#define PWMDUTY   0xC3
#elif (LEDPWM==LEDPWM2)
#define PWMDIV      0xC6
#define PWMDUTY   0xC7
#elif (LEDPWM==LEDPWM3)
#define PWMDIV      0xC6
#define PWMDUTY   0xC7
#endif
//=============================
#define LED_P07_EN            BIT1
#define LED_P06_EN            BIT0
#define LED_P15_EN            BIT0

#define LED_PWM_Green       LED_P06_EN
#define LED_PWM_Amber       LED_P07_EN


//**************************************************************************
//  RCOSC = XTAL * Counter / 512 => Counter = RCOSC *512/XTAL,
//  IF RCOSC=4M, Counter=143 =>8Fh
//  CHIP_FAMILY_TSUMC uses FRO_12M /3 =4M to calibration ==> Counter=143
//  IF RCOSC=12M, Xtal=12M, Counter=512 =>0x200

#define RCOSC_CLK       12 // 4
#define RCOSC_TARGET    0x200 // 143
//**************************************************************************

#define RCOSC       0
#define XTAL        1
#define XTALDIV2    2
#define XTALDIV4    3

#define STANDBY 1
#define DCOFF   2
//--------PM Option----------------
//#define PM_CLOCK                XTAL
#if 0 //TBD
#if PM_CLOCK==RCOSC
#define PM_TIMER_CLOCK_PERIOD (0x10000-265) //more precise
//#define PM_TIMER_CLOCK_PERIOD   (0x10000-((((unsigned long)14318*RCOSC_TARGET/512)*INT_PERIOD)/12))
#define PM_FORCE_DELAY_DIV      8
#define PM_DELAY4US_LOOP        1
#elif PM_CLOCK==XTAL
#define PM_TIMER_CLOCK_PERIOD   (0x10000-(((unsigned long)14318*INT_PERIOD)/12))
#define PM_FORCE_DELAY_DIV      2
#define PM_DELAY4US_LOOP        4
#elif PM_CLOCK==XTALDIV2
#define PM_TIMER_CLOCK_PERIOD   (0x10000-(((unsigned long)14318/2*INT_PERIOD)/12))
#define PM_FORCE_DELAY_DIV      4
#define PM_DELAY4US_LOOP        2
#elif PM_CLOCK==XTALDIV4
#define PM_TIMER_CLOCK_PERIOD   (0x10000-(((unsigned long)14318/4*INT_PERIOD)/12))
#define PM_FORCE_DELAY_DIV      8
#define PM_DELAY4US_LOOP        1
#endif
#endif
#define IsDetDVIClk(x, status)          (g_DVIClk_DetectMask[g_InputPort[x].eIPMux] & status)

//>>-----------------------------------------------------------------------------------------
//power down macro

#define PM_ENABLE_ADC()   do {\
/*PD_REF_RGB*/						msWriteByteMask(REG_102508, 0x00,BIT0); \
/*PD_BG*/							msWriteByteMask(REG_002508, 0x00,BIT1); \
/*PD_REF*/							msWriteByteMask(REG_002508, 0x00,BIT2); \
/*PD_ADCR*/						    msWriteByteMask(REG_102508, 0x00,BIT5); \
/*PD_ADCG*/					    	msWriteByteMask(REG_102508, 0x00,BIT6); \
/*PD_ADCB*/					    	msWriteByteMask(REG_102508, 0x00,BIT7); \
/*PD_PHDIG_D*/						msWriteByteMask(REG_10250B, 0x00,BIT3); \
/*PD_ADCPLA_REG*/					msWriteByteMask(REG_102509, 0x00,BIT1); \
/*PD_ICLP_RGB*/						msWriteByteMask(REG_102509, 0x00,BIT2); \
/*GCR_ADCPLA_PHDAC_R_PD*/			msWriteByteMask(REG_1025BC, 0x00,BIT6); \
/*GCR_ADCPLA_PHDAC_G_PD*/			msWriteByteMask(REG_1025BC, 0x00,BIT7); \
/*GCR_ADCPLA_PHDAC_B_PD*/			msWriteByteMask(REG_1025BD, 0x00,BIT0); \
/*PD_HSYNC0_COMP*/					msWriteByteMask(REG_10250B, 0x00,BIT0); \
/*PD_LDO25_REF*/				    	msWriteByteMask(REG_10250B, 0x00,BIT4); \
/*adc clock gen*/						msWrite2ByteMask(REG_10250C, 0x00,0xFFFF); \
									msWriteByteMask(REG_0003E9, 0x00,BIT7); \
/*PD_LDO11*/						msWriteByteMask(REG_0025B4, 0x00,BIT5); \
/*PD_LDO25_ADCA*/					msWriteByteMask(REG_0025B4, 0x00,BIT6); \
/*PD_SOG_DAC*/						msWriteByteMask(REG_002509, 0x00,BIT1); \
/*PD_SOG_MAIN*/				    	msWriteByteMask(REG_002509, 0x00,BIT2); \
/*PD_SOG_MUX*/						msWriteByteMask(REG_002560, 0x00,BIT0); \
/*ISOG_HIGH_NUM*/					msWriteByteMask(REG_0025F0, 0x02,0x0F); \
/*ISOG_CNT_LIMIT*/					msWrite2ByteMask(REG_0025F2, 0x0C8,0x0FFF); \
/*ISOG_RESET_CNT_LIMIT*/		    	msWrite2ByteMask(REG_0025F0, 0x9280,0xFFF0);\
/*ISOG_RESET_HIGH_NUM*/		    	msWriteByteMask(REG_0025F3, 0x00,0xF0);\
/*ISOG_ITCTRL_CLKIN*/			    	msWriteByteMask(REG_0025F4, 0x00,BIT1);\
/*TEST_SOG_RESET*/					msWriteByteMask(REG_0025F4, 0x00,BIT3);\
                                              }while(0)
#define PM_DISABLE_ADC(status)   do {\
/*PD_REF_RGB*/						msWriteByteMask(REG_102508, BIT0,BIT0); \
/*PD_BG*/							msWriteByteMask(REG_002508, (status==STANDBY)?(0x00):(BIT1|BIT2),BIT1|BIT2); \
/*PD_ADCR*/					    	msWriteByteMask(REG_102508, BIT5,BIT5); \
/*PD_ADCG*/					    	msWriteByteMask(REG_102508, BIT6,BIT6); \
/*PD_ADCB*/					    	msWriteByteMask(REG_102508, BIT7,BIT7); \
/*PD_PHDIG_D*/						msWriteByteMask(REG_10250B, BIT3,BIT3); \
/*PD_ADCPLA_REG*/					msWriteByteMask(REG_102509, BIT1,BIT1); \
/*PD_ICLP_RGB*/						msWriteByteMask(REG_102509, BIT2,BIT2); \
/*GCR_ADCPLA_PHDAC_R_PD*/			msWriteByteMask(REG_1025BC, BIT6,BIT6); \
/*GCR_ADCPLA_PHDAC_G_PD*/			msWriteByteMask(REG_1025BC, BIT7,BIT7); \
/*GCR_ADCPLA_PHDAC_B_PD*/			msWriteByteMask(REG_1025BD, BIT0,BIT0); \
/*PD_HSYNC0_COMP*/					msWriteByteMask(REG_10250B, BIT0,BIT0); \
/*PD_LDO25_REF*/				    	msWriteByteMask(REG_10250B, (status==STANDBY)?(0x00):(BIT4),BIT4); \
/*adc clock gen*/						msWrite2ByteMask(REG_10250C, 0xFFFF,0xFFFF); \
/*PD_LDO11*/						msWriteByteMask(REG_0025B4, (status==STANDBY)?(0x00):(BIT5),BIT5); \
/*PD_LDO25_ADCA*/					msWriteByteMask(REG_0025B4, (status==STANDBY)?(0x00):(BIT6),BIT6); \
/*PD_SOG_DAC*/						msWriteByteMask(REG_002509, (status==STANDBY)?(0x00):(BIT1),BIT1); \
/*PD_SOG_MAIN*/				    	msWriteByteMask(REG_002509, (status==STANDBY)?(0x00):(BIT2),BIT2); \
/*PD_SOG_MUX*/						msWriteByteMask(REG_002560, (status==STANDBY)?(0x00):(BIT0),BIT0); \
/*ISOG_HIGH_NUM*/					msWriteByteMask(REG_0025F0, 0x02,0x0F); \
/*ISOG_CNT_LIMIT*/					msWrite2ByteMask(REG_0025F2, 0x0C8,0x0FFF); \
/*ISOG_RESET_CNT_LIMIT*/		    	msWrite2ByteMask(REG_0025F0, 0x9280,0xFFF0);\
/*ISOG_RESET_HIGH_NUM*/		    	msWriteByteMask(REG_0025F3, 0x10,0xF0);\
/*ISOG_ITCTRL_CLKIN*/			    	msWriteByteMask(REG_0025F4, 0x00,BIT1);\
/*TEST_SOG_RESET*/					msWriteByteMask(REG_0025F4, 0x00,BIT3);\
                                              }while(0)
#define PM_ENABLE_MHL_ATOP_0() do{\
/*P0_PD_DM*/					msWriteByteMask(REG_112106,0,BIT2|BIT1|BIT0); \
/*P0_PD_BG*/						msWriteByteMask(REG_001724,0,BIT3); \
/*P0_PD_DVIPLL*/					msWriteByteMask(REG_112105,0,BIT5); \
/*P0_PD_REG*/					msWriteByteMask(REG_112105,0,BIT4); \
/*PD_P0_DPLPHI*/					msWriteByteMask(REG_112104,0,BIT6|BIT5|BIT4); \
/*PD_P0_DPLPHQ*/				msWriteByteMask(REG_112105,0,BIT2|BIT1|BIT0); \
/*P0_PD_CLKIN_OFFL*/				msWriteByteMask(REG_001724,0,BIT5); \
/*P0_PD_CLKIN*/					msWriteByteMask(REG_001724,0,BIT6); \
/*PD_RCK*/						msWriteByteMask(REG_001724,0,BIT7); \
/*PD_RD0*/						msWriteByteMask(REG_112107,0,BIT2|BIT1|BIT0); \
                                              }while(0)


#define PM_DISABLE_MHL_ATOP_0()   do {\
/*P0_PD_DM*/					msWriteByteMask(REG_112106, BIT2|BIT1|BIT0,BIT2|BIT1|BIT0); \
/*P0_PD_BG*/						msWriteByteMask(REG_001724, sPMInfo.ucPMMode==ePM_POWER_DPMS?0 : BIT3,BIT3); \
/*P0_PD_DVIPLL*/					msWriteByteMask(REG_112105, BIT5,BIT5); \
/*P0_PD_REG*/					msWriteByteMask(REG_112105, BIT4,BIT4); \
/*PD_P0_DPLPHI*/					msWriteByteMask(REG_112104, BIT6|BIT5|BIT4,BIT6|BIT5|BIT4); \
/*PD_P0_DPLPHQ*/				msWriteByteMask(REG_112105, BIT2|BIT1|BIT0,BIT2|BIT1|BIT0); \
/*P0_PD_CLKIN_OFFL*/				msWriteByteMask(REG_001724, sPMInfo.ucPMMode==ePM_POWER_DPMS?0 : BIT5,BIT5); \
/*P0_PD_CLKIN*/					msWriteByteMask(REG_001724, sPMInfo.ucPMMode==ePM_POWER_DPMS?0 : BIT6,BIT6); \
/*PD_RCK*/						msWriteByteMask(REG_001724, sPMInfo.ucPMMode==ePM_POWER_DPMS?0 : BIT7,BIT7); \
/*PD_RD0*/						msWriteByteMask(REG_112107, BIT2|BIT1|BIT0,BIT2|BIT1|BIT0); \
                                              }while(0)
#define PM_ENABLE_MHL_ATOP_1()   do {\
/*P0_PD_DM*/                   msWriteByteMask(REG_112306, 0,BIT2|BIT1|BIT0); \
/*P0_PD_BG*/                   msWriteByteMask(REG_001726, 0,BIT3); \
/*P0_PD_DVIPLL*/                msWriteByteMask(REG_112305, 0,BIT5); \
/*P0_PD_REG*/                   msWriteByteMask(REG_112305, 0,BIT4); \
/*PD_P0_DPLPHI*/                msWriteByteMask(REG_112304, 0,BIT6|BIT5|BIT4); \
/*PD_P0_DPLPHQ*/                msWriteByteMask(REG_112305, 0,BIT2|BIT1|BIT0); \
/*P0_PD_CLKIN_OFFL*/             msWriteByteMask(REG_001726, 0,BIT5); \
/*P0_PD_CLKIN*/                 msWriteByteMask(REG_001726, 0,BIT6); \
/*PD_RCK*/                      msWriteByteMask(REG_001726, 0,BIT7); \
/*PD_RD0*/                      msWriteByteMask(REG_112307, 0,BIT2|BIT1|BIT0); \
                                }while(0)
                                
#define PM_DISABLE_MHL_ATOP_1()   do {\
/*P0_PD_DM*/					msWriteByteMask(REG_112306, BIT2|BIT1|BIT0,BIT2|BIT1|BIT0); \
/*P0_PD_BG*/						msWriteByteMask(REG_001726, sPMInfo.ucPMMode==ePM_POWER_DPMS?0 : BIT3,BIT3); \
/*P0_PD_DVIPLL*/					msWriteByteMask(REG_112305, BIT5,BIT5); \
/*P0_PD_REG*/					msWriteByteMask(REG_112305, BIT4,BIT4); \
/*PD_P0_DPLPHI*/					msWriteByteMask(REG_112304, BIT6|BIT5|BIT4,BIT6|BIT5|BIT4); \
/*PD_P0_DPLPHQ*/				msWriteByteMask(REG_112305, BIT2|BIT1|BIT0,BIT2|BIT1|BIT0); \
/*P0_PD_CLKIN_OFFL*/				msWriteByteMask(REG_001726, sPMInfo.ucPMMode==ePM_POWER_DPMS?0 : BIT5,BIT5); \
/*P0_PD_CLKIN*/					msWriteByteMask(REG_001726, sPMInfo.ucPMMode==ePM_POWER_DPMS?0 : BIT6,BIT6); \
/*PD_RCK*/						msWriteByteMask(REG_001726, sPMInfo.ucPMMode==ePM_POWER_DPMS?0 : BIT7,BIT7); \
/*PD_RD0*/						msWriteByteMask(REG_112307, BIT2|BIT1|BIT0,BIT2|BIT1|BIT0); \
                                              }while(0)
#define PM_ENABLE_MHL3_ATOP_0()   do {\
/*REG_PD_LDO*/                 msWriteByteMask(REG_112500, 0,BIT3); \
/*REG_PD_CLKIN*/               msWriteByteMask(REG_1125B7, 0,BIT4); \
/*REG_PD_BG*/                  msWriteByteMask(REG_1125B7, 0,BIT5); \
/*REG_PD_LANE[2:0]*/             msWriteByteMask(REG_1125B6, 0,0x0F); \
/*REG_PD_PHDAC[2:0]*/           msWriteByteMask(REG_1125B2, 0,BIT1); \
                              msWriteByteMask(REG_1125B7, 0,BIT2|BIT1|BIT0);\
/*NODIE_PD_RTERM[3:0]*/         msWriteByteMask(REG_001720, 0,0x0F); \
/*NODIE_PD_CLKIN*/              msWriteByteMask(REG_001720, 0,BIT4); \
                                              }while(0)           
#define PM_DISABLE_MHL3_ATOP_0()   do {\
/*REG_PD_LDO*/					msWriteByteMask(REG_112500, BIT3,BIT3); \
/*REG_PD_CLKIN*/					msWriteByteMask(REG_1125B7, BIT4,BIT4); \
/*REG_PD_BG*/					msWriteByteMask(REG_1125B7, BIT5,BIT5); \
/*REG_PD_LANE[2:0]*/				msWriteByteMask(REG_1125B6, 0x0F,0x0F); \
/*REG_PD_PHDAC[2:0]*/				msWriteByteMask(REG_1125B2, BIT1,BIT1); \
                                msWriteByteMask(REG_1125B7, BIT2|BIT1|BIT0,BIT2|BIT1|BIT0);\
/*NODIE_PD_RTERM[3:0]*/			msWriteByteMask(REG_001720, sPMInfo.ucPMMode==ePM_POWER_DPMS? (BIT0|BIT1|BIT2): 0x0F,0x0F); \
/*NODIE_PD_CLKIN*/				msWriteByteMask(REG_001720, sPMInfo.ucPMMode==ePM_POWER_DPMS? 0x00 : BIT4,BIT4); \
                                              }while(0)           
#define PM_ENABLE_MHL3_ATOP_1()   do {\
/*REG_PD_LDO*/                 msWriteByteMask(REG_112500, 0,BIT3); \
/*REG_PD_CLKIN*/                msWriteByteMask(REG_1125B7, 0,BIT4); \
/*REG_PD_BG*/                  msWriteByteMask(REG_1125B7, 0,BIT5); \
/*REG_PD_LANE[2:0]*/             msWriteByteMask(REG_1125B6, 0,0x0F); \
/*REG_PD_PHDAC[2:0]*/            msWriteByteMask(REG_1125B2, 0,BIT1); \
                               msWriteByteMask(REG_1125B7, 0,BIT2|BIT1|BIT0);\
/*NODIE_PD_RTERM[3:0]*/          msWriteByteMask(REG_001722, 0,0x0F); \
/*NODIE_PD_CLKIN*/              msWriteByteMask(REG_001722, 0,BIT4); \
                                              }while(0)                                              
#define PM_DISABLE_MHL3_ATOP_1()   do {\
/*REG_PD_LDO*/					msWriteByteMask(REG_112500, BIT3,BIT3); \
/*REG_PD_CLKIN*/					msWriteByteMask(REG_1125B7, BIT4,BIT4); \
/*REG_PD_BG*/					msWriteByteMask(REG_1125B7, BIT5,BIT5); \
/*REG_PD_LANE[2:0]*/				msWriteByteMask(REG_1125B6, 0x0F,0x0F); \
/*REG_PD_PHDAC[2:0]*/				msWriteByteMask(REG_1125B2, BIT1,BIT1); \
                                msWriteByteMask(REG_1125B7, BIT2|BIT1|BIT0,BIT2|BIT1|BIT0);\
/*NODIE_PD_RTERM[3:0]*/			msWriteByteMask(REG_001722, sPMInfo.ucPMMode==ePM_POWER_DPMS? (BIT0|BIT1|BIT2): 0x0F,0x0F); \
/*NODIE_PD_CLKIN*/				msWriteByteMask(REG_001722, sPMInfo.ucPMMode==ePM_POWER_DPMS?0x00 : BIT4,BIT4); \
                                              }while(0)                                              
#define PM_DISABLE_MHL3_ECBUS_ATOP_0()   do {\
/*AFE_TX_REP_DRI_PD*/				msWriteByteMask(REG_003395, BIT6|BIT5,BIT6|BIT5); \
/*AFE_TX_DRI_PD*/					msWriteByteMask(REG_003397, BIT6|BIT5,BIT6|BIT5); \
/*AFE_RX_PD*/					msWriteByteMask(REG_0033AB, BIT6,BIT6); \
/*AFE_TX_LB_DRI_PD*/				msWriteByteMask(REG_0033B0, BIT7,BIT7); \
/*ECBSS_PLL_PD*/					msWriteByteMask(REG_00339A, BIT0,BIT0); \
                                              }while(0)                                              
#define PM_DISABLE_MHL3_ECBUS_ATOP_1()   do {\
/*AFE_TX_REP_DRI_PD*/				msWriteByteMask(REG_003595, BIT6|BIT5,BIT6|BIT5); \
/*AFE_TX_DRI_PD*/					msWriteByteMask(REG_003597, BIT6|BIT5,BIT6|BIT5); \
/*AFE_RX_PD*/					msWriteByteMask(REG_0035AB, BIT6,BIT6); \
/*AFE_TX_LB_DRI_PD*/				msWriteByteMask(REG_0035B0, BIT7,BIT7); \
/*ECBSS_PLL_PD*/					msWriteByteMask(REG_00359A, BIT0,BIT0); \
                                              }while(0)           
#define PM_DISABLE_DP_COMBO_ATOP()   do {\
/*NODIE_PD_RT*/						msWriteByteMask(REG_001900, 0x0F,0x0F); \
/*NODIE_PD_CLKIN*/					msWriteByteMask(REG_001901, BIT4,BIT4); \
/*NODIE_EN_SQH*/					    msWriteByteMask(REG_001901, BIT7,BIT7);\
                                    msWriteByteMask(REG_001900, BIT6,BIT6); \
/*REG_PD_BG*/						msWriteByteMask(REG_112ED7, BIT0,BIT0);\
/*REG_PD_LANE*/						msWriteByteMask(REG_112EB6, 0x0F,0x0F);\
/*REG_PD_PHDAC*/					msWriteByteMask(REG_112EB2, BIT1,BIT1);\
                                    msWriteByteMask(REG_112ED7, 0xF0,0xF0);\
                                              }while(0)           

#define PM_DISABLE_EDP()   do {\
/*EN_AUX_0*/						msWriteByteMask(REG_11263A, BIT2,BIT2); \
/*PD_RTERM_0*/					msWriteByteMask(REG_112640, 0x00,BIT4); \
/*EN_AUX_1*/						msWriteByteMask(REG_11273A, BIT2,BIT2); \
/*PD_RTERM_1*/					msWriteByteMask(REG_112740, 0x00,BIT4); \
/*EN_AUX_2*/						msWriteByteMask(REG_11283A, BIT2,BIT2); \
/*PD_RTERM_2*/					msWriteByteMask(REG_112840, 0x00,BIT4); \
/*EN_AUX_3*/						msWriteByteMask(REG_11293A, BIT2,BIT2); \
/*PD_RTERM_3*/					msWriteByteMask(REG_112940, 0x00,BIT4); \
                                              }while(0)
                                              
#define PM_DISABLE_COMBOAUXSTATUS() do{\
/*DP_AUPLL PD*/						msWriteByteMask(REG_162F07, BIT0,BIT0); \
/*DP_VDPLL PD*/						msWriteByteMask(REG_162F83, BIT0,BIT0); \
/*EN_AUX_0*/						    msWriteByteMask(REG_0011A8, sPMInfo.ucPMMode==ePM_POWER_DPMS? BIT2 : 0x00,BIT2); \
/*PD_RTERM_0*/						msWriteByteMask(REG_0011A9, sPMInfo.ucPMMode==ePM_POWER_DPMS?0x00: BIT5,BIT5); \
                                      }while(0)


#define PM_DISABLE_EDPHBR2() do{\
                     				msWrite2ByteMask(REG_140B16, 0xFFFF,0xFFFF); \
                     				msWrite2ByteMask(REG_140B18, 0xFFFF,0xFFFF); \
                     				msWrite2ByteMask(REG_140B1A, 0xFFFF,0xFFFF); \
                                      }while(0)
      

#define PM_DISABLE_MOD()   do {\
/*power down current gen*/             	msWriteByteMask(REG_140650, BIT0, BIT0);\			
/*power down clock input source*/      	msWriteByteMask(REG_14064E, 0, BIT5);\
/*power down clock input*/             	msWriteByteMask(REG_140780, 0x00, 0xFF);\
/*set TTL mode*/                       msWrite2ByteMask(REG_140620, 0x00, 0xFFFF);\
                                    msWrite2ByteMask(REG_140622, 0x00, 0xFFFF);\
                                    msWrite2ByteMask(REG_140624, 0x00, 0xFFFF);\
                                    msWrite2ByteMask(REG_140782, 0x00, 0xFFFF);\
                                    msWrite2ByteMask(REG_140784, 0x00, 0xFFFF);\
                                    msWrite2ByteMask(REG_140786, 0x00, 0xFFFF);\
                                    msWrite2ByteMask(REG_140628, 0xFFFF, 0xFFFF);\
/*set input mode*/                     msWrite2ByteMask(REG_14062A, 0xFFFF, 0xFFFF);\
                                    msWrite2ByteMask(REG_14062C, 0xFFFF, 0xFFFF);\
                                    msWrite2ByteMask(REG_1407C6, 0xFFFF, 0xFFFF);\
                                    msWrite2ByteMask(REG_1407C8, 0xFFFF, 0xFFFF);\
                                    msWrite2ByteMask(REG_1407CA, 0xFFFF, 0xFFFF);\
                                              }while(0)
                                              
#define PM_DISABLE_LPLL() do{\
/*LPLL_PD*/							msWriteByteMask(REG_140506, BIT5, BIT5);\
/*LPLL_PD*/							msWriteByteMask(REG_140581, BIT7, BIT7);\
/*MPLL_PD*/							msWriteByteMask(REG_1405D8, BIT3, BIT3);\
                                              }while(0)

#define PM_DISABLE_AUDIO() do{\
/*PD_ADC0*/						msWriteByteMask(REG_112CDA, BIT0 , BIT0);\	
/*PD_L0_DAC*/					msWriteByteMask(REG_112CDA, BIT1 , BIT1);\	
/*PD_R0_DAC*/					msWriteByteMask(REG_112CDA, BIT5 , BIT5);\	
/*PD_REF_DAC*/					msWriteByteMask(REG_112CDB,BIT1 , BIT1);\	
/*PD_BIAS_DAC*/					msWriteByteMask(REG_112CDB,BIT2 , BIT2);\	
/*PD_OPLP_EAR*/					msWriteByteMask(REG_000B00, sPMInfo.ucPMMode==ePM_POWER_DPMS?0 : BIT0 ,BIT0);\	
/*PD_IBIAS_EAR*/					msWriteByteMask(REG_112CDB,BIT4 , BIT4);\	
/*PD_RT_EAR*/					msWriteByteMask(REG_112CDB,BIT5 , BIT5);\	
/*PD_LT_EAR*/					msWriteByteMask(REG_112CDB,BIT6 , BIT6);\	
/*PD_INMUX0*/					msWriteByteMask(REG_112CDC,BIT0 , BIT0);\	
/*PD_VI*/						msWriteByteMask(REG_112CDC,BIT3 , BIT3);\	
/*PD_VREF*/						msWriteByteMask(REG_000B00, sPMInfo.ucPMMode==ePM_POWER_DPMS?0 : BIT1 ,BIT1);\	
/*SEL_DEPOP_SB_EAR*/				if(sPMInfo.ucPMMode==ePM_POWER_DPMS)msWriteByteMask(REG_000B00, BIT2, BIT2);\	
/*EN_CK_DAC*/						msWriteByteMask(REG_112CE4, 0x00, BIT0);\	
                                              }while(0)

#define PM_DISABLE_MPLL()  do{\
/*MPLL_PD*/						msWriteByteMask(REG_101ED1, BIT0, BIT0);\
/*MPLL_CLK_ADC216/432M_PD*/		msWriteByteMask(REG_101EDC, BIT5|BIT4, BIT5|BIT4);\
                                             }while(0)

#define PM_ENABLE_MPLL()  do{\
								msWriteByteMask(REG_101ED1, 0, BIT0);\
								msWriteByteMask(REG_101EDC, 0, BIT5|BIT4);\
                                             }while(0)

#define PM_DISABLE_DDRATOP_AB() do{\
/*DDR_ATOP*/                    msWriteByteMask(REG_101154, BIT6|BIT5|BIT4,BIT6|BIT5|BIT4); \
/*DDRDLL*/                      msWriteByteMask(REG_101160, BIT1,BIT1); \
/*DDR_ATOP*/                    msWriteByteMask(REG_101134, BIT7,BIT7); \
/*DDR_ATOP*/                    msWriteByteMask(REG_1011D4, BIT6|BIT5|BIT4,BIT6|BIT5|BIT4); \
/*DDRDLL*/                      msWriteByteMask(REG_1011E0, BIT1,BIT1); \
/*DDR_ATOP*/                    msWriteByteMask(REG_1011B4, BIT7,BIT7); \
                                      }while(0)

#define PM_DISABLE_CHIPTOP()  do{\
/*clk_fclk_gate*/					msWriteByteMask(REG_120F12, BIT0, BIT0);\
                                             }while(0)

INTERFACE XDATA sPM_Info  sPMInfo;

INTERFACE BYTE xdata Check,Check1;
#if ENABLE_DP_INPUT
extern bit g_bDoDPInit;
#endif
extern volatile bit g_bMcuPMClock;

INTERFACE Bool msPM_IsState_IDLE(void);

// setting configuration meta data
INTERFACE void msPM_Init(void);
INTERFACE void msPM_SetFlagDPMS(void);
INTERFACE void msPM_SetFlagDCoff(void);

INTERFACE void msPM_ClearStatus(Bool bResetPM);
INTERFACE void msPM_Handler(void);
INTERFACE void msPM_WaitingEventActivity(void);
INTERFACE void msPM_PMWakeupInit(void);

INTERFACE void msPM_SetPMMode(void);
INTERFACE void msPM_WaitingEvent(void);
INTERFACE Bool msPM_Checkagain(void);
INTERFACE void msPM_InterruptEnable(Bool benable);
INTERFACE void PMDummy(void);

INTERFACE void msPM_Functions(void);
//INTERFACE void msPM_ForceExit(void);
INTERFACE void msPM_ForceEnter(void);

INTERFACE void msPM_EnableSOGDetect(Bool bEnable);
INTERFACE void msPM_EnableSARDetect(Bool bEnable);
INTERFACE void msPM_EnableGPIODetect(Bool bEnable);
INTERFACE void msPM_EnableMCCSDetect(Bool bEnable);
INTERFACE void msPM_EnableHVSyncDetect(Bool bEnable);
#if( PM_SUPPORT_AC2DC )
INTERFACE void msPM_EnableAC2DC(Bool bEnable);
#endif//end of #if( PM_SUPPORT_AC2DC )
#if( PM_SUPPORT_WAKEUP_DVI )
INTERFACE void msPM_EnableDVIDetect(Bool bEnable);
#endif//end of #if( PM_SUPPORT_WAKEUP_DVI )

#if ( PM_SUPPORT_WAKEUP_CEC == 1)
	INTERFACE BYTE msPM_GetCECRegOffset(BYTE ucIdx);
	INTERFACE void msPM_EnableCECDetect(BOOL bEnable);
#endif

INTERFACE void msPM_EnableDPDetect(BOOL bEnable);

//function for time sharing feature
INTERFACE void msPM_EnableDVIClockAmp (Bool bEnable);
INTERFACE void msPM_OutputTriState(void);
INTERFACE void msPM_MCCSReset(void);
INTERFACE Bool DDCCI_CheckDMPSON(void);
INTERFACE void msPM_EnableIRDetect(BOOL bEnable);
INTERFACE void msPM_ResetChip(void);

INTERFACE BOOL msPM_GetDPSquelch(BYTE u8ComboPort, WORD u16Times);
INTERFACE BYTE msPM_GetActiveDPPort(BYTE u8InputPort, Bool bScanAll);
INTERFACE BYTE msPM_GetActiveDVIPort(BYTE u8DVI86_Status, BYTE u8DVI8B_Status);
INTERFACE BYTE msPM_GetActiveMHLPort(Bool bPowerSaving);
#undef INTERFACE

#undef INTERFACE
#endif


#ifndef MS_PM_H
#define MS_PM_H

#ifdef _MS_PM_C_
#define INTERFACE
#else
#define INTERFACE extern
#endif


#if  1//CHIP_FAMILY_TYPE==CHIP_FAMILY_MST9U
#else
#warning "please implement PMSET for new board"
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
  BYTE u8SAR_KeyMask;
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

//--------PM Option----------------
//#define PM_CLOCK                XTAL
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

#define IsDetDVIClk(x, status)          (g_DVIClk_DetectMask[g_InputPort[x].eIPMux] & status)

//power down macro
#define PM_DISABLE_EDP()   do {\
                                                  msWriteByteMask(REG_11261C, BIT2, BIT2);\
                                                  msWriteByteMask(REG_11261F, 0, BIT4);\
                                                  msWriteByteMask(REG_11263C, BIT2, BIT2);\
                                                  msWriteByteMask(REG_11263F, 0, BIT4);\
                                                  msWriteByteMask(REG_11265C, BIT2, BIT2);\
                                                  msWriteByteMask(REG_11265F, 0, BIT4);\
                                                  msWriteByteMask(REG_11267C, BIT2, BIT2);\
                                                  msWriteByteMask(REG_11267F, 0, BIT4);\
                                              }while(0)

#define PM_DISABLE_MOD()   do {\
/*power down current gen*/             msWriteByteMask(REG_140650, BIT0, BIT0);\
/*power down clock input source*/      msWriteByteMask(REG_14064E, 0, BIT5);\
/*power down clock input*/             msWriteByteMask(REG_140780, 0x00, 0xFF);\
/*set TTL mode*/                       msWrite2ByteMask(REG_140620, 0x00, 0xFF);\
                                       msWrite2ByteMask(REG_140622, 0x00, 0xFF);\
                                       msWrite2ByteMask(REG_140624, 0x00, 0xFF);\
                                       msWrite2ByteMask(REG_140782, 0x00, 0xFF);\
                                       msWrite2ByteMask(REG_140784, 0x00, 0xFF);\
                                       msWrite2ByteMask(REG_140786, 0x00, 0xFF);\
                                       msWrite2ByteMask(REG_140628, 0xFF, 0xFF);\
/*set input mode*/                     msWrite2ByteMask(REG_14062A, 0xFF, 0xFF);\
                                       msWrite2ByteMask(REG_14062C, 0xFF, 0xFF);\
                                       msWrite2ByteMask(REG_1407C6, 0xFF, 0xFF);\
                                       msWrite2ByteMask(REG_1407C8, 0xFF, 0xFF);\
                                       msWrite2ByteMask(REG_1407CA, 0xFF, 0xFF);\
                                              }while(0)


#define PM_DISABLE_AUDIO() do{\
/* AUSDM power down */               msWrite2Byte(REG_14020A, 0xFFFF);\
/*Audio Clock power down*/           msWrite2Byte(REG_14020C, 0xFFFF);\
                                     msWrite2Byte(REG_14024C, 0x0000);\
                                     msWrite2Byte(REG_14024E, 0x0000);\
                                              }while(0)

#define PM_DISABLE_LPLL() do{\
                                                  msWriteByteMask(REG_140506, BIT5, BIT5);\
                                                  msWriteByteMask(REG_140586, BIT5, BIT5);\
                                              }while(0)

#define PM_DISABLE_MPLL()  do{\
                                                  msWriteByteMask(REG_101ED1, BIT0, BIT0);\
                                                  msWriteByteMask(REG_101EDC, BIT5|BIT4, BIT5|BIT4);\
                                             }while(0)

#define PM_ENABLE_MPLL()  do{\
                                                  msWriteByteMask(REG_101ED1, 0, BIT0);\
                                                  msWriteByteMask(REG_101EDC, 0, BIT5|BIT4);\
                                             }while(0)


#define PM_DISABLE_CHIPTOP()  do{\
                                                  msWriteByteMask(REG_120F24, BIT0, BIT0);\
                                             }while(0)

#define PM_DISABLE_COMBOAUXSTATUS() do{\
                                             msWriteByte(REG_0011AE+COMBO_OFFSET(MUX_COMBO0), 0x00); \
                                             msWriteByte(REG_0011AE+COMBO_OFFSET(MUX_COMBO1), 0x00); \
                                             msWriteByte(REG_0011AE+COMBO_OFFSET(MUX_COMBO2), 0x00); \
                                             msWriteByte(REG_0011AE+COMBO_OFFSET(MUX_COMBO3), 0x00); \
                                             msWriteByte(REG_0011AE+COMBO_OFFSET(MUX_COMBO4), 0x00); \
                                             msWriteByte(REG_0011AE+COMBO_OFFSET(MUX_COMBO5), 0x00); \
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

#if 0
INTERFACE void msPM_PowerDownMacro(void);
INTERFACE Bool msPM_StartRCOSCCal(void);
INTERFACE Bool msPM_GetPMStatus(void);
INTERFACE void msPM_SetPMClock(void);
INTERFACE void msPM_PassWord(Bool bEnable);
#endif
INTERFACE void msPM_ClearStatus(Bool bResetPM);
INTERFACE void msPM_Handler(void);
INTERFACE void msPM_WaitingEventActivity(void);
//INTERFACE void msPM_InitWaitingEventActivity(void);
INTERFACE void msPM_PMWakeupInit(void);

#if 0
INTERFACE BYTE msPM_Reset(void);
#endif


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
//void msPM_SetupWakeUpFunc(void);
//void msPM_printConfiguration(void);

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

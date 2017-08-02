#if  ENABLE_LED_CONTROLLER

#ifndef     LEDCONTROL_H
#define     LEDCONTROL_H

#ifdef _LEDCONTROL_C_
#define INTERFACE
#else
#define INTERFACE extern
#endif
//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#ifndef LED_CH_0
#define LED_CH_0    1
#endif
#ifndef LED_CH_1
#define LED_CH_1    1
#endif
#ifndef LED_CH_2
#define LED_CH_2    1
#endif
#ifndef LED_CH_3
#define LED_CH_3    1
#endif
#ifndef PanelLightBarDefaultVoltage
#define PanelLightBarDefaultVoltage     20
#endif
#ifndef PanelLightBarMaximumVoltage
#define PanelLightBarMaximumVoltage     25
#endif
#ifndef LEDBL_PanelDefaultCurrent
#define LEDBL_PanelDefaultCurrent    30
#warning "LED controller is enabled but doesnt set LED parameters in panelxx.h"
#endif

#define LED_STRING_NUM                  4
#define LED_SERIES                                  (LED_CH_0+LED_CH_1+LED_CH_2+LED_CH_3)

#define ENABLE_TSUML_NEWOVP             1 // 120327 coding addition for new setting, it needs to modify h/w setting

#if ENABLE_TSUML_NEWOVP
//#define PanelLightBarDefaultVoltage           41
//#define PanelLightBarMaximumVoltage       46
#define OVP1_Ratio                      110    // 1.1
#define OVP1_ProtVoltage                    (OVP1_Ratio*PanelLightBarMaximumVoltage/100)  // 1.1*45=    49.5V
#define OVP2_Ratio                      125    // 1.25
#define OVP2_ProtVoltage                    (OVP2_Ratio*PanelLightBarDefaultVoltage/100)  // 1.1*42=    52.5V
#define ResistorRatio                       46  //(R2+R1)/R1
#define OVP2_INTERRUPT                  1   // enable this define to shunt down Boost if OVP2 is actived.
#define OCP_INTERRUPT                  1   // enable this define to shunt down Boost if OVP2 is actived.
#define PnlVOL_TO_SAR(_X_ )                 ((((DWORD)(_X_)*(0x3FF))/(ResistorRatio*195/100))&0x3FF)
#endif

#define AnalogCurrent(X)  (((WORD)(X)*(120))/133)&0xFF;



#define _OPEN_INTERRUPT_ENABLE_

#define LED_SKIP_DIMMING                1
#define LED_SKIP_LOW_BOOST              1   //un-used
#define LED_CAL_POWER                   0
#define LED_DETECT_STRN                 1   //detect Vstring/Vdet voltage to decide LED status (BKA_C6 and BKA_C7)
#define LED_THERMAL_SENSOR_PROTECT      1
#define ENABLE_LED_INT                  1
#define ENABLE_HW_SHORT_PROTECTION      0

#define LED_SAR_VOLTAGE_MEASURE_ENABLE  1

#define LED_AUTO_RECOVER     1 // 20130107 coding addition




#define LED_limitOCPtoreduceCurrentPeak 	0
	#define LED_Direct5VBoostCircuit		1
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

typedef struct
{
    BYTE  ucPollingCounter;
    BYTE  ucBoostPwmStep;
    Bool  bBoostClkODClk:1;
    Bool  bControlModeHW:1;
    Bool  bBoostOnOff:1;
    Bool  bDimCtlModeMultiPhase:1;
    Bool  bDataReady:1;
    WORD  WCurrentPWM;
    //BYTE    bTime_Out;  //20110302
    BYTE  bMEASURE_Delay;
    BYTE  bPREPARE_Delay;
    BYTE  bPrint_Delay;
    BYTE  bTurnOn_Delay;
    BYTE  bCurrent[16];
} LED_INFO;

typedef union _LED_Status
{
    struct
    {
        BYTE bBoost_Under_Voltage : 1;
        BYTE bBoost_Over_Current : 1;
        BYTE bBoost_Over_Voltage : 1;
        BYTE bBoost_Over_Temperature : 1;
        BYTE bBoost_Over_Skew  : 1;
        BYTE bNonuse2 : 1;
        BYTE bNonuse1 : 1;
        BYTE bNonuse : 1;

        BYTE bHigh_Boost : 1;
        BYTE bLow_Boost : 1;
        BYTE bSkew_Error : 1;
        BYTE bCross_Boundary_Error  : 1;
        BYTE bBoost_High_low_Error  : 1;
        BYTE bBoost_Tuning_Finish : 1;
        BYTE bBoost_Data_Fnish : 1;
        BYTE bBoost_Clock_Fail : 1;

    }   Separate;

    WORD total;
} LED_Status;


typedef struct
{
    Bool bBOOST_Warning;
    WORD wLED_OneFail;
    WORD wLED_TwoFail;
    WORD wLED_Open;
    WORD wSkew_Error;
    WORD wHiBOOST;
    WORD wLowBOOST;
    BYTE bBOOST_PWM_States;
    Bool bSYStemFail;
} LED_Alert_Status;



enum
{
    Boost_ODClk  =0,
#if 1//(CHIP_FAMILY_TYPE==CHIP_FAMILY_TSUMB || CHIP_FAMILY_TYPE==CHIP_FAMILY_TSUMY)
    Boost_216MClk  =1,
#else
    Boost_XtalClk  =1,
#endif
};

enum
{
    HW_Mode  =0,
    SW_Mode  =1,
};

enum
{
    MULTI_Phase=0,
    SINGLE_Phase =1,
};


typedef enum
{
    eLEDSTATE_INITIAL,
    eLEDSTATE_PREPARE,
    eLEDSTATE_MEASURE,
    eLEDSTATE_NORMAL,
    eLEDSTATE_Check,
    //eLEDSTATE_IDLE,  //20110302
    eLEDSTATE_Warn,
    eLEDSTATE_Reset,
#if 1 //20110302
    eLEDSTATE_Off
#endif
} SYM_STATE;


typedef enum
{
    eLED_IDLE =0,
    eLED_ADJUST_BOOST_HIGH = 1,
    eLED_ADJUST_BOOST_LOW = 2,
    eLED_ADJUST_DIMCURRENT =3 ,

} LED_STATE;


typedef enum
{
    eLED_OCPLV_0 =3000,
    eLED_OCPLV_1 =4000,
    eLED_OCPLV_2 =5000,
    eLED_OCPLV_3 =6500,

} LED_OCP_Level;


typedef enum
{
    Min_PWM =0,
    Normal_PWM =1,
    Max_PWM =2

} BOOST_STATE;

typedef enum
{
    UP_PWM=0,
    KEEP_PWM =1,
    DOWM_PWM=2

} BOOST_PWM_ADJUST;

#define GET_BIT0(val)   ((val & BIT0)?1:0)
#define GET_BIT1(val)   ((val & BIT1)?1:0)
#define GET_BIT2(val)   ((val & BIT2)?1:0)
#define GET_BIT3(val)   ((val & BIT3)?1:0)
#define GET_BIT4(val)   ((val & BIT4)?1:0)
#define GET_BIT5(val)   ((val & BIT5)?1:0)
#define GET_BIT6(val)   ((val & BIT6)?1:0)
#define GET_BIT7(val)   ((val & BIT7)?1:0)
#define GET_BIT8(val)   ((val & BIT8)?1:0)
#define GET_BIT9(val)   ((val & BIT9)?1:0)
#define GET_BIT10(val)  ((val & BIT10)?1:0)
#define GET_BIT11(val)  ((val & BIT11)?1:0)
#define GET_BIT12(val)  ((val & BIT12)?1:0)
#define GET_BIT13(val)  ((val & BIT13)?1:0)
#define GET_BIT14(val)  ((val & BIT14)?1:0)
#define GET_BIT15(val)  ((val & BIT15)?1:0)

#define Ch0     0x0001
#define Ch1     0x0002
#define Ch2     0x0004
#define Ch3     0x0008
#define Ch4     0x0010
#define Ch5     0x0020
#define Ch6     0x0040
#define Ch7     0x0080
#define Ch8     0x0100
#define Ch9     0x0200
#define Ch10    0x0400
#define Ch11    0x0800
#define Ch12    0x1000
#define Ch13    0x2000
#define Ch14    0x4000
#define Ch15    0x8000

#if LED_AUTO_RECOVER
INTERFACE  BYTE xdata StringShortOpenStatus; // record short and open string
INTERFACE  BYTE xdata StringErrorCounter; // record count of short or open, recover string when counter small than STRING_ERROR_COUNT
INTERFACE BYTE xdata StringRecoverCounter; // unit: 1second, cout to STRING_ERROR_RECOVER_PERIOD to recover
INTERFACE bit StringShortOpenChecking; // 1 means light bar works normally, and start to check short/open status
INTERFACE bit StringShortOpenFlag; // 1 means short/open occur
INTERFACE bit StringRecoverCounterFlag; // if 1, StringRecoverCounter start to count
INTERFACE bit StringRebootFlag; // 1 means system reboot
INTERFACE BYTE xdata StringRebootNormalCounter; // record that systerm works normal period after reboot
#define STRING_ERROR_COUNT  5// recover times
#define STRING_ERROR_RECOVER_PERIOD  3 // unit: 1 second
#define STRING_REBOOT_NORMAL_PERIOD  10 // unit: 1 second
//#define LED_SERIES_MASK     (LED_CH_0+LED_CH_1+LED_CH_2+LED_CH_3)
#endif

INTERFACE  WORD xdata u16TotalPower;
INTERFACE  LED_INFO xdata gLEDInfo;
INTERFACE  LED_Status xdata LEDSatus;
INTERFACE  BYTE xdata u8SymState;
INTERFACE  BYTE xdata u8LED_Channel;
//INTERFACE  BYTE xdata u8States_5C;
//INTERFACE  BYTE xdata u8States_5D;
//INTERFACE  BYTE idata u8Inter_Count;
//INTERFACE  BYTE idata u8CheckLEDINT;

INTERFACE WORD volatile xdata BoostCounter; // count from enable boost clock to enable dimming
INTERFACE bit volatile BoostCounterFlag; // 120815 coding, set to bit data type for isr usage
#define BOOST_COUNT     500//150    for turn on panel like smooth by different Mainboard
INTERFACE  void msLED_ReloadDimmingPWM(void);
INTERFACE  BYTE msLED_GetPWMValue(void);
INTERFACE  void  msLED_DimmingPWMAdjust(BYTE PwmValue);
#if 0 //20110302
INTERFACE  void  msLED_Off(void);
#endif
INTERFACE  void  Init_LEDIRQEnable(BOOL bEnable);
INTERFACE  void  msLED_Init(void);
INTERFACE  void  msLED_Hander(void);
INTERFACE  void  msLED_SetDimCurrent(WORD, BYTE);
INTERFACE  void  msLED_ReloadBoostPWM(void);
INTERFACE  void  msLED_DimmingEnable( BYTE , WORD);
INTERFACE  void msLED_PowerCal(WORD *Data, Bool value);
INTERFACE  void msLED_Set_BoostPwmStep(BYTE value);
INTERFACE  void msLED_Off(void);
INTERFACE  void msLED_On(void);
INTERFACE  void msLED_BoostEnable( BYTE ucStatus);
INTERFACE  void SetDimmingPWMFreq(WORD freq);

#if LED_AUTO_RECOVER
INTERFACE void msLED_FastReboot(void);
INTERFACE void msLED_InitRecoveryVariables(void);
#endif

#if 0//def _OPEN_INTERRUPT_ENABLE_
INTERFACE  void msLED_CheckStringOpen(void);
#endif
//=========================================================
#define PWM_DIMMING_FREQ         180
#if LED_CTRL_BRIGHTNESS_BY_CURRENT
#define FQC_DIV_M               0
#define FQC_DIV_N               1   //div=2 to fix dimming freq to 14kHz
#else
#define FQC_DIV_M               3
#define FQC_DIV_N               ((28000ul/(PWM_DIMMING_FREQ)) )/(1<<FQC_DIV_M)
#endif
//=========================================================
#define PWM_BOOST_FREQ          225  //khz
#define CLKDIV_OD_N             ((150000ul/PWM_BOOST_FREQ)/16-1)   //PanelDCLK
#define CLKDIV_216M_N           ((216000ul/PWM_BOOST_FREQ)/16 -1)
//=========================================================
//=========================================================
// 1.	Calculate duty (D)
//		D = 1 - Vin/vout
// 2.	Calculate peak current Ipeak (A)
//		Ipeak = Iout/(1-D) + Vin*D/2/L/Fsw
#define Vin_Boost               12// Unit:V
#define L_value                  22 // Unit:uH
#define TotalCurrent          LED_SERIES*LEDBL_PanelDefaultCurrent  // Unit:mA
#define GateDuty                (100-(Vin_Boost*100/PanelLightBarMaximumVoltage))   //Unit:% x 100
#define IPeak			1.5*(((DWORD)TotalCurrent*100)/(100-GateDuty) +((DWORD)Vin_Boost*10000/2/22*GateDuty)/PWM_BOOST_FREQ)   // Unit:mA
//=========================================================
//=========================================================
#define  TIME_TURN_ON_DELAY     5
#define  TIME_SAR_SAMPLE_DELAY  50
//=========================================================

#if LED_CTRL_BRIGHTNESS_BY_CURRENT
	#if LED_DETECT_STRN
	#define NORMAL_UP               1000//1000    //10V
	#define NORMAL_LOW              500   //5.5V
	#else
	#define NORMAL_UP               (1000/10)   //0.6V
	#define NORMAL_LOW              (550/10)  //0.5V
	#endif
#else
	#if LED_DETECT_STRN
	#define NORMAL_UP               700//1000    //10V
	#define NORMAL_LOW              300   //5.5V
	#else
	#define NORMAL_UP               (1000/10)   //0.6V
	#define NORMAL_LOW              (550/10)  //0.5V
	#endif
#endif

#if LED_DETECT_STRN
#define SKEW_LOW           2000//2550//     1550    //20110307 by Scar
#define SKWE_UP                 3000//3500    //20110307 by Scar
#else
#define SKEW_LOW                (1550/10)
#define SKWE_UP                 (3500/10)
#endif

#if ENABLE_TSUML_NEWOVP
	#if LED_CTRL_BRIGHTNESS_BY_CURRENT
	#define BOOST_PWM_TUNING_STEP       (DWORD)((NORMAL_UP-NORMAL_LOW)/((DWORD)ResistorRatio*19/10*1000/0x3FF))/2
	#else
	#define BOOST_PWM_TUNING_STEP       (DWORD)((NORMAL_UP-NORMAL_LOW)/((DWORD)ResistorRatio*19/10*1000/0x3FF))
	#endif
#else
#define BOOST_PWM_TUNING_STEP       5
#endif
#define INIT_BOOST_DUTY           90
#define MIN_BOOST_DUTY            50  //CCJ Modify for HW issue

#define MIN_BOOST_PWM           0x2CC   //70%
#define MAX_BOOST_PWM           0x39A   //90%

#define DUTY_TO_SAR(_X_)        ((((DWORD)(_X_)*(1023))/(100))&0x3FF)
#define VOL_TO_SAR(_X_ )        (((((DWORD)(_X_)*(1023)))/55/100)&0x3FF)

#define SAR_TO_VOL(_X_ )        (((WORD)(_X_)*(55))/1023)

#define SET_BIT(val, X)         ((WORD)val |(1<<X))

#define CLR_SAR_Flag()          (_msWriteByteMask(REG_0A66,BIT4,BIT4))

#define SAR_V_10p00             0xB8    //X=10/5.5*100=182=0xB6
#define SAR_V_8p00              0x96    //X=8/5.5*100=149=0x95
#define SAR_V_5p00              0x5D       // U01  0x22E
#define SAR_V_4p50              0x53       // U01  0x1D1
#define SAR_V_1p55              0x30       //=> 2.5V   Skew Low Bond
#define SAR_V_0p55              0xA
#define SAR_V_0p54              0xA
#define SAR_V_0p10              0x5

#define TOLERANCE_SAR           0x00
#define LIMIT_POWER             2000//  5000           //*10
#define LEAVE_POWER             3000

#if 0
#if (LED_SERIES == 4) || (LED_SERIES == 3)
#define LEDBL_PanelDefaultCurrent        65//50
#elif (LED_SERIES == 6)
#define LEDBL_PanelDefaultCurrent        30
#elif (LED_SERIES == 8)
#define LEDBL_PanelDefaultCurrent        14 //10
#endif
#endif
#define DIM_CURRENT_OVERSKEW    (LEDBL_PanelDefaultCurrent * 2) / 3       // 20A
#define DIM_CURRENT_ONE_FAIL    (LEDBL_PanelDefaultCurrent * 1) / 3       // 10A
#define DIM_CURRENT_LOW         (LEDBL_PanelDefaultCurrent) /5             //  6A

#define DIM_CURRENT_30MA        30      // 30mA
#define DIM_CURRENT_20MA        20      // 30mA
//=========================================================
#if LED_Direct5VBoostCircuit
#define GDRV_THRD           0x1F0 //Set GDRV_THRD =4*(2/5)/3.3*1024=496=0x1F0h
#define UVLO_TH                 0x00 //Set UVLO_Vth=4V
#else
#define GDRV_THRD           0x174 //Set GDRV_THRD =3*(2/5)/3.3*1024=372=0x174h
#define UVLO_TH                 0x00 //Set UVLO_Vth=3V
#endif
// 20110323
typedef enum
{
    STRING_NUM_0,
    STRING_NUM_1,
    STRING_NUM_2,
    STRING_NUM_3,
    STRING_NUM_4,
    STRING_NUM_5,
    STRING_NUM_6,
    STRING_NUM_7,
    STRING_NUM_8,
    STRING_NUM_9,
    STRING_NUM_10,
    STRING_NUM_11,
    STRING_NUM_12,
    STRING_NUM_13,
    STRING_NUM_14,
    STRING_NUM_15,

} StringNumType;

#if LED_CH_0
#define LED_CH_D0   BIT0
#define LED_CH_S0   BIT4
#else
#define LED_CH_D0   0
#define LED_CH_S0   0
#endif

#if LED_CH_1
#define LED_CH_D1   BIT1
#define LED_CH_S1   BIT5
#else
#define LED_CH_D1   0
#define LED_CH_S1   0
#endif

#if LED_CH_2
#define LED_CH_D2   BIT2
#define LED_CH_S2   BIT6
#else
#define LED_CH_S2   0
#define LED_CH_D2   0
#endif

#if LED_CH_3
#define LED_CH_D3   BIT3
#define LED_CH_S3   BIT7
#else
#define LED_CH_S3   0
#define LED_CH_D3   0
#endif

#define STREAM_VALUE_S  (LED_CH_S0|LED_CH_S1|LED_CH_S2|LED_CH_S3)
#define STREAM_VALUE_D  (LED_CH_D0|LED_CH_D1|LED_CH_D2|LED_CH_D3)
//=========================================================
#undef INTERFACE


#endif
#endif


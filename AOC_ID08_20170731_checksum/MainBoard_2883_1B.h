#define CPU_CLOCK_KHZ		28636
#define CPU_CLOCK_MHZ		(CPU_CLOCK_KHZ/1000)
#define MST_CLOCK_MHZ		28636
#define SERIAL_BAUD_RATE	9600
#define UsePWMBrightness	1
#define BrightnessPWM		PWM2C//PWM3
#define USEPWM3

//================================================
// Detect Cable
//#define hwDVICable_Pin		P0_3
//#define hwDSUBCable_Pin 	P0_2//P0_0
//================================================
// Led
//#define hwDVICable_Pin     P0_3;
#define hwGreenLed_Pin		P0_7//P0_6
#define hwAmberLed_Pin		P1_5//P0_7
#define hwBlacklit_Pin		P2_5
#define hwPanel_Pin 	P4
#define hwDDC_WP_Pin	P4//P4_6//
#define hwEEPROM_WP		P4// P4_7////2006-06-01 Andy 
#define hwFLASH_WP       		P2_4//P2_7//P1_5
//================================================
// mStar I2C
#define hwMS_ResetZ_Pin		P3_7  //???

#define ReduceDDC
#ifdef ReduceDDC
#define hwSystemVcc_Pin		P0_0
#endif


#define SetDDRPort	P3_4=ddrPort&BIT0;\
P3_5=ddrPort&BIT1;\
P3_6=ddrPort&BIT2;\
P3_7=ddrPort&BIT3;
//================================================
// I2C
#define hwI2C_SCL_Pin		P1_0
#define hwI2C_SDA_Pin		P1_1
//Audio 
#define hwMute_Pin			P1_3//P1_4
#define Set_ms_ResetZ()		hwMS_ResetZ_Pin=1
#define Clr_ms_ResetZ()		hwMS_ResetZ_Pin=0
#define Set_ms_CSZ()		hwMS_CSZ_Pin=1
#define Clr_ms_CSZ()		hwMS_CSZ_Pin=0
#define Set_ms_SDA()		hwMS_SDA_Pin=1
#define Clr_ms_SDA()		hwMS_SDA_Pin=0
#define ms_SDAHi()			hwMS_SDA_Pin==1
#define Set_ms_SCL()		hwMS_SCL_Pin=1
#define Clr_ms_SCL()		hwMS_SCL_Pin=0
// mStar DDR
#define Set_ms_ALE()		hwMS_ALE_Pin=1
#define Clr_ms_ALE()		hwMS_ALE_Pin=0
#define Set_ms_WRZ()		hwMS_WRZ_Pin=1
#define Clr_ms_WRZ()		hwMS_WRZ_Pin=0
#define Set_ms_RDZ()		hwMS_RDZ_Pin=1
#define Clr_ms_RDZ()		hwMS_RDZ_Pin=0
#define Set_i2c_SDA()		hwI2C_SDA_Pin=1
#define Clr_i2c_SDA()		hwI2C_SDA_Pin=0
#define i2c_SDAHi()		(hwI2C_SDA_Pin)
#define i2c_SDALo()		(!(hwI2C_SDA_Pin))
#define Set_i2c_SCL()		hwI2C_SCL_Pin=1
#define Clr_i2c_SCL()		hwI2C_SCL_Pin=0
#define i2c_SCLHi()		(hwI2C_SCL_Pin)
#define i2c_SCLLo()		(!(hwI2C_SCL_Pin))
#define hw_SetGreenLed()	hwGreenLed_Pin=1
#define hw_ClrGreenLed()	hwGreenLed_Pin=0
#define hw_SetAmberLed()	hwAmberLed_Pin=1
#define hw_ClrAmberLed()	hwAmberLed_Pin=0
#define hw_SetBlacklit()	{hwBlacklit_Pin=0;BackLightActiveFlag=1;}
#define hw_ClrBlacklit()	{hwBlacklit_Pin=1;BackLightActiveFlag=0;}
#define hw_SetPanel()		P4&=~BIT5//hwPanel_Pin =0
#define hw_ClrPanel()		P4|=BIT5//hwPanel_Pin =1
//Audio
#if (ModelName==AOC_930FWz||ModelName==AOC_931Fwz||ModelName==AOC_919Va2)//Use 2824 Q2 Power Board
#define hw_SetMute()			hwMute_Pin=0
#define hw_ClrMute()			hwMute_Pin=1
#define hwStandby_Pin		hwMute_Pin
#define hw_SetStandby()		hwStandby_Pin=0    
#define hw_ClrStandby()		hwStandby_Pin=1 
#else
#define hw_SetMute()			hwMute_Pin=1
#define hw_ClrMute()			hwMute_Pin=0
#define hwStandby_Pin		hwMute_Pin
#define hw_SetStandby()		hwStandby_Pin=1    
#define hw_ClrStandby()		hwStandby_Pin=0 
#endif


#define hw_SetDDC_WP()		P4&=~BIT6//hwDDC_WP_Pin = 0
#define hw_ClrDDC_WP()		P4|=BIT6//hwDDC_WP_Pin = 1
#define Set_EEPROM_WP()     	P4|=BIT7//hwEEPROM_WP=1
#define Clr_EEPROM_WP()     	P4&=~BIT7//hwEEPROM_WP=0
#define hw_SetFlashWP()		hwFLASH_WP=0
#define hw_ClrFlashWP()		hwFLASH_WP=1
#if ENABLE_HDCP
    #define hwHDCP_Hpd_Pin        P1_2//P1_5
    #define hw_Set_HdcpHpd()	hwHDCP_Hpd_Pin=0
    #define hw_Clr_HdcpHpd()		hwHDCP_Hpd_Pin=1
#endif


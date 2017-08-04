//******************************
//Benz 2007.4.13   10:43:02
//for 2701
//*****************************
#define CPU_CLOCK_KHZ		28636//14318//28636
#define CPU_CLOCK_MHZ		(CPU_CLOCK_KHZ/1000)
#define MST_CLOCK_MHZ		28636//14318//28636
#define SERIAL_BAUD_RATE	9600
#define UsePWMBrightness	1
#define BrightnessPWM		PWM1C
//================================================
// Detect Cable
//Benz 2007.4.13   9:44:25	#define hwDVICable_Pin		P1_6
//Benz 2007.4.13   9:44:27	#define hwDSUBCable_Pin		P1_5
//#define hwDVICable_Pin		P1_5 //P35
//#define hwDSUBCable_Pin		P2_2 //P69
//================================================
// Led
#define hwGreenLed_Pin		P0_7
#define hwAmberLed_Pin		P0_6
//#define hwBlueLed_Pin		P0_6
#define hwBlacklit_Pin		P1_2
#define hwPanel_Pin		   P2_7//Benz 2007.4.13   10:40:02 P0_3 
#define hwPanel12V_Pin	//Benz 2007.4.13   10:40:06	P0_3   
#define hwAudio_Pin		 P1_3   
#if USB_Contral
    #define hw_USB_Control		P1_5
#endif 
#define hwMute_Pin		    P1_3
#define hwStandby_Pin		P1_4 
//================================================
// mStar I2C
#define hwMS_ResetZ_Pin		P3_7  //
#define SetDDRPort	P3_4=ddrPort&BIT0;\
P3_5=ddrPort&BIT1;\
P3_6=ddrPort&BIT2;\
P3_7=ddrPort&BIT3;
//================================================
// I2C
#define hwI2C_SCL_Pin		P1_0
#define hwI2C_SDA_Pin		P1_1
//#define hwI2C_WP_Pin		P2_3    
#define hwEEPROM_WP		P2_3    
#define hwDDC_WP_Pin		P2_2    
//#define hwEEPROM_WP               P4 //2006-06-01 Andy 
#define Set_ms_ResetZ()		hwMS_ResetZ_Pin=1
#define Clr_ms_ResetZ()		hwMS_ResetZ_Pin=0
#define Set_ms_CSZ()		hwMS_CSZ_Pin=1
#define Clr_ms_CSZ()		hwMS_CSZ_Pin=0
#define Set_ms_SDA()		hwMS_SDA_Pin=1
#define Clr_ms_SDA()		hwMS_SDA_Pin=0
#define ms_SDAHi()		    hwMS_SDA_Pin==1
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
//#define Set_i2c_WP()		hwI2C_WP_Pin=1
//#define Clr_i2c_WP()		hwI2C_WP_Pin=0
#define Set_EEPROM_WP() 	hwEEPROM_WP=1
#define Clr_EEPROM_WP()		hwEEPROM_WP=0
#define hw_SetGreenLed()	hwGreenLed_Pin=0;//hwBlueLed_Pin=0
#define hw_ClrGreenLed()	hwGreenLed_Pin=1;//hwBlueLed_Pin=1
#define hw_SetAmberLed()	hwAmberLed_Pin=0
#define hw_ClrAmberLed()	hwAmberLed_Pin=1
//#define hw_SetBlueLed()		hwBlueLed_Pin=0
//#define hw_ClrBlueLed()		hwBlueLed_Pin=1
#if USB_Contral
    #define hw_USB_HI()		hw_USB_Control=1
    #define hw_USB_LOW()		hw_USB_Control=0
#endif 
#if Panel_Inv
    #define hw_SetPanel()		hwPanel_Pin=1
    #define hw_ClrPanel()		hwPanel_Pin=0
#else 
    #define hw_SetPanel()		hwPanel_Pin=0
    #define hw_ClrPanel()		hwPanel_Pin=1
#endif 
//#define hw_SetBlacklit()	             {hwBlacklit_Pin&=~BIT7;BackLightActiveFlag=1;}
//#define hw_ClrBlacklit()	              {hwBlacklit_Pin|=BIT7;BackLightActiveFlag=0;}
#define hw_SetBlacklit()	{hwBlacklit_Pin=0;BackLightActiveFlag=1;}
#define hw_ClrBlacklit()	{hwBlacklit_Pin=1;BackLightActiveFlag=0;}
#define hw_SetMute()		hwMute_Pin=1
#define hw_ClrMute()		hwMute_Pin=0
#define hw_SetStandby()		hwStandby_Pin=1    
#define hw_ClrStandby()		hwStandby_Pin=0   
#define hw_SetPanel12V()	             hwPanel12V_Pin= 1 //0
#define hw_ClrPanel12V()	             hwPanel12V_Pin= 0 //1
#define hw_SetDDC_WP()		 hwDDC_WP_Pin=0  //2005-04-14
#define hw_ClrDDC_WP()		 hwDDC_WP_Pin=1  //2005-04-14
//#define Set_EEPROM_WP()     hwEEPROM_WP=1  //2006-03-20 Andy
//#define Clr_EEPROM_WP()     hwEEPROM_WP=0  //2006-03-20 Andy 
//#define Set_EEPROM_WP()     hwEEPROM_WP|=BIT6//2006-06-1 Andy
//#define Clr_EEPROM_WP()     hwEEPROM_WP&=~BIT6//  2006-6-1 Andy 
#if ENABLE_HDCP
    //#define hwHDCP_Hpd_Pin       P4//P1_3   //2006-08-10 Andy Temp Setting
    #define hw_Set_HdcpHpd()     hwBlacklit_Pin&=~BIT4  //2006-08-17 hwHDCP_Hpd_Pin=0  // 1 
    #define hw_Clr_HdcpHpd()     hwBlacklit_Pin|=BIT4     //2006-08-17 hwHDCP_Hpd_Pin=1  // 0 
#endif 
#define hwFlash_WP_Pin   P2_4
#define hw_SetFlashWP()     hwFlash_WP_Pin=0
#define hw_ClrFlashWP()      hwFlash_WP_Pin=1

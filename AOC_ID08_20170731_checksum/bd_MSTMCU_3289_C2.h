#ifndef __BD_MSTMCU__
#define  __BD_MSTMCU__

#define CPU_CLOCK_KHZ		28636
#define CPU_CLOCK_MHZ		(CPU_CLOCK_KHZ/1000)

#define MST_CLOCK_MHZ		28636
#define SERIAL_BAUD_RATE	9600

#define UsePWMBrightness	1
#define BrightnessPWM		PWM0C
#define VolumePWM                 PWM2C

#define UnUsePin                P2_5

#define PowerKey		 P1_6

//================================================
// Detect Cable
#define hwDVICable_Pin		P0_3  //P1_5 
#define hwDSUBCable_Pin		P0_3 //P1_5 

//================================================
// Led
#define hwGreenLed_Pin		P0_6
#define hwAmberLed_Pin		P0_7

#define hwBlacklit_Pin		P1_5
#define hwPanel_Pin			P0_4 // 080530
//#define hwPanel12V_Pin		P0_4  
//#define hwAudio_Pin		 	P2_5    

#define hwMute_Pin		       P4//     P44
#define hwStandby_Pin		P4 //  P44

//================================================
// mStar I2C

#define hwMS_ResetZ_Pin		P3_7  //


#define SetDDRPort	P3_4=ddrPort&BIT0;\
			P3_5=ddrPort&BIT1;\
			P3_6=ddrPort&BIT2;\
			P3_7=ddrPort&BIT3;


//================================================
// I2C
#define hwI2C_SCL_Pin		UnUsePin//P1_0
#define hwI2C_SDA_Pin		UnUsePin//P1_1
#define hwI2C_WP_Pin		UnUsePin//P4

//#define hwHDCP_I2C_SCL_Pin       (P4 & BIT2)   
//#define hw_Set_Hdcp_SCL()   (P4 |= BIT2)  
//#define hw_Clr_Hdcp_SCL()   (P4 &= (~BIT2))  
//#define hwHDCP_I2C_SDA_Pin       (P4 & BIT3) 
//#define hw_Set_Hdcp_SDA()   (P4 |= BIT3)  
//#define hw_Clr_Hdcp_SDA()   (P4 &= (~BIT3))  

#define hwDDC_WP_Pin		P0_0//P1_6
#define hwEEPROM_WP               UnUsePin//P4 //p42

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


#if 0//ENABLE_HDCP
#define Set_i2c_SDA()       (HdcpNVRamFlag? (hw_Set_Hdcp_SDA()): (hwI2C_SDA_Pin=1))
#define Clr_i2c_SDA()       (HdcpNVRamFlag? (hw_Clr_Hdcp_SDA()): (hwI2C_SDA_Pin=0))
#define i2c_SDAHi()         (HdcpNVRamFlag? (hwHDCP_I2C_SDA_Pin): (hwI2C_SDA_Pin))
#define i2c_SDALo()         (HdcpNVRamFlag? (!(hwHDCP_I2C_SDA_Pin)): (!(hwI2C_SDA_Pin)))

#define Set_i2c_SCL()       (HdcpNVRamFlag? (hw_Set_Hdcp_SCL()): (hwI2C_SCL_Pin=1))
#define Clr_i2c_SCL()       (HdcpNVRamFlag? (hw_Clr_Hdcp_SCL()): (hwI2C_SCL_Pin=0))
#define i2c_SCLHi()         (HdcpNVRamFlag? (hwHDCP_I2C_SCL_Pin): (hwI2C_SCL_Pin))
#define i2c_SCLLo()         (HdcpNVRamFlag? (!(hwHDCP_I2C_SCL_Pin)): (!(hwI2C_SCL_Pin)))
#else
#define	Set_i2c_SDA()		hwI2C_SDA_Pin=1
#define	Clr_i2c_SDA()		hwI2C_SDA_Pin=0
#define i2c_SDAHi()		(hwI2C_SDA_Pin)
#define i2c_SDALo()		(!(hwI2C_SDA_Pin))

#define Set_i2c_SCL()		hwI2C_SCL_Pin=1
#define Clr_i2c_SCL()		hwI2C_SCL_Pin=0
#define i2c_SCLHi()		(hwI2C_SCL_Pin)
#define i2c_SCLLo()		(!(hwI2C_SCL_Pin))
#endif

#define Set_i2c_WP()		hwI2C_WP_Pin|=BIT2
#define Clr_i2c_WP()		hwI2C_WP_Pin&=~BIT2

#if ScanKey
extern void SK_CtrlGreenLed(bit ctrl_on);
extern void SK_CtrlAmberLed(bit ctrl_on);
#define hw_SetGreenLed()	    (SK_CtrlGreenLed(1))
#define hw_ClrGreenLed()	    (SK_CtrlGreenLed(0))
#define hw_SetAmberLed()    (SK_CtrlAmberLed(1))
#define hw_ClrAmberLed()    (SK_CtrlAmberLed(0))
#else
#define hw_SetGreenLed()	    hwGreenLed_Pin=1
#define hw_ClrGreenLed()	    hwGreenLed_Pin=0
#define hw_SetAmberLed()	hwAmberLed_Pin=1
#define hw_ClrAmberLed()	hwAmberLed_Pin=0
#endif

#define hw_SetPanel()   hwPanel_Pin=0
#define hw_ClrPanel()   hwPanel_Pin=1


#define hw_SetBlacklit()	             {hwBlacklit_Pin=0;BackLightActiveFlag=1;}
#define hw_ClrBlacklit()	              {hwBlacklit_Pin=1;BackLightActiveFlag=0;}

//#define hw_SetMute()		hwMute_Pin=1
//#define hw_ClrMute()		hwMute_Pin=0

//#define hwMute_Pin        (P4 & BIT4)     // Annie 2006.8.21// Annie 2006.8.14  BIT5
#define hw_SetMute()     (P4 &= (~BIT4))  // Annie 2006.9.1 (P4 |= BIT4) // hwHDCP_Hpd_Pin=0  // 1 
#define hw_ClrMute()      (P4 |= BIT4) // Annie 2006.9.1 (P4 &= (~BIT4)) //  hwHDCP_Hpd_Pin=1  // 0 


#define hw_SetStandby()		(P4 &= (~BIT4))//hwStandby_Pin=0 // 1    //Amy2008.1.9   8:59:32
#define hw_ClrStandby()		 (P4 |= BIT4) //hwStandby_Pin=1 // 0   

#define hw_SetPanel12V()	             hwPanel12V_Pin= 0 //0
#define hw_ClrPanel12V()	             hwPanel12V_Pin= 1

#define hw_SetDDC_WP()		 hwDDC_WP_Pin=0  //2005-04-14
#define hw_ClrDDC_WP()		 hwDDC_WP_Pin=1  //2005-04-14

//#define Set_EEPROM_WP()     hwEEPROM_WP=1  //2006-03-20 Andy
//#define Clr_EEPROM_WP()     hwEEPROM_WP=0  //2006-03-20 Andy 

#define Set_EEPROM_WP()     hwEEPROM_WP|=BIT2//2006-06-1 Andy
#define Clr_EEPROM_WP()     hwEEPROM_WP&=~BIT2//  2006-6-1 Andy 

#if ENABLE_HDCP
#define hwHDCP_Hpd_Pin        (P4 & BIT3)     // Annie 2006.8.21// Annie 2006.8.14  BIT5
#define hw_Set_HdcpHpd()   (P4 &= (~BIT3))  // Annie 2006.9.1 (P4 |= BIT4) // hwHDCP_Hpd_Pin=0  // 1 
#define hw_Clr_HdcpHpd()  (P4 |= BIT3) // Annie 2006.9.1 (P4 &= (~BIT4)) //  hwHDCP_Hpd_Pin=1  // 0 
#endif



#define hwFlash_WP_Pin   P2_4
#define hw_SetFlashWP()     hwFlash_WP_Pin=0
#define hw_ClrFlashWP()      hwFlash_WP_Pin=1


#if ScanKey // 080919 check which IO pin are used
//#define hw_SK_LEDVcc    
#define Set_SK_LEDVcc() 
#define Clr_SK_LEDVcc() 

#define hw_SK_KeyVcc    // p42
#define Set_SK_KeyVcc()  (P4&=~BIT2)//hw_SK_KeyVcc = 0
#define Clr_SK_KeyVcc()  (P4|=BIT2)//hw_SK_KeyVcc = 1

#define hw_SK_SDA      P0_2
#define hw_SK_SCL       P0_1

#define Set_SK_SCL()    hw_SK_SCL = 1  
#define Clr_SK_SCL()     hw_SK_SCL = 0
#define SK_SCLHi()	   (hw_SK_SCL)
#define SK_SCLLo()	   (!(hw_SK_SCL))

#define Set_SK_SDA()    hw_SK_SDA = 1  
#define Clr_SK_SDA()     hw_SK_SDA = 0
#define SK_SDAHi()	    (hw_SK_SDA)
#define SK_SDALo()	    (!(hw_SK_SDA))

#define LEDCtrlByScalerIOPin // 090624 coding for LED control by scaler GPIO pin
#ifdef LEDCtrlByScalerIOPin
#define SetGreenLed()	    hwGreenLed_Pin=1
#define ClrGreenLed()	        hwGreenLed_Pin=0
#define SetAmberLed()	hwAmberLed_Pin=1
#define ClrAmberLed()	hwAmberLed_Pin=0
#endif

#endif


#endif



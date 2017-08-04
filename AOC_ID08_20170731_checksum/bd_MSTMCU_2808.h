#define CPU_CLOCK_KHZ		28636
#define CPU_CLOCK_MHZ		(CPU_CLOCK_KHZ/1000)

#define MST_CLOCK_MHZ		28636

#define SERIAL_BAUD_RATE	9600

#define UsePWMBrightness	1
#define BrightnessPWM		PWM2C
//================================================
// Detect Cable
//#define hwDVICable_Pin		P0_3
//#define hwDSUBCable_Pin 	P0_3
#define hwDSUBCable_Pin		P0_0
#define hwDVICable_Pin	    	P0_0

//================================================
// Led
//#define hwGreenLed_Pin		P0_7
//#define hwAmberLed_Pin		P1_5
#define hwGreenLed_Pin		P0_6  //charles
#define hwAmberLed_Pin		P0_7 //charles

#define hwBlacklit_Pin		P2_5
#define hwPanel_Pin 			P0_4	//P4

#define hwDDC_WP_Pin		P4
#define hwEEPROM_WP		P2_6 //2006-06-01 Andy 
#define hwFLASH_WP       		P2_7//P2_4//P1_5


//================================================
// mStar I2C
#define hwMS_ResetZ_Pin		P3_7  //???

//#define hwMS_CSZ_Pin		P2_3
//#define hwMS_SCL_Pin		P2_4
//#define hwMS_SDA_Pin		P2_5

// DDR
#define hwMS_ALE_Pin		P2_3
#define hwMS_WRZ_Pin		P2_5
#define hwMS_RDZ_Pin		P2_4

#define hwMS_DDRPort		P3
#define hwMS_DDRShiftBits	4
#define hwMS_DDRBits		0xF0//0x3C

#define SetDDRPort	P3_4=ddrPort&BIT0;\
			P3_5=ddrPort&BIT1;\
			P3_6=ddrPort&BIT2;\
			P3_7=ddrPort&BIT3;


//================================================
// I2C
#define hwI2C_SCL_Pin		P1_0
#define hwI2C_SDA_Pin		P1_1


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


#define	Set_i2c_SDA()		hwI2C_SDA_Pin=1
#define	Clr_i2c_SDA()		hwI2C_SDA_Pin=0
#define i2c_SDAHi()		(hwI2C_SDA_Pin)
#define i2c_SDALo()		(!(hwI2C_SDA_Pin))

#define Set_i2c_SCL()		hwI2C_SCL_Pin=1
#define Clr_i2c_SCL()		hwI2C_SCL_Pin=0
#define i2c_SCLHi()		(hwI2C_SCL_Pin)
#define i2c_SCLLo()		(!(hwI2C_SCL_Pin))

#define hw_SetGreenLed()	hwGreenLed_Pin=0
#define hw_ClrGreenLed()	hwGreenLed_Pin=1

#define hw_SetAmberLed()	hwAmberLed_Pin=0
#define hw_ClrAmberLed()	hwAmberLed_Pin=1

#define hw_SetBlacklit()	{hwBlacklit_Pin=0;BackLightActiveFlag=1;}
#define hw_ClrBlacklit()	{hwBlacklit_Pin=1;BackLightActiveFlag=0;}
	
#define hw_SetPanel()		hwPanel_Pin=0
#define hw_ClrPanel()			hwPanel_Pin=1

//Audio
#define hwMute_Pin			P4
#if 1
#define hw_SetMute()			P4&=~BIT2
#define hw_ClrMute()			P4|=BIT2
#else
#define hw_SetMute()			P4|=BIT2
#define hw_ClrMute()			P4&=~BIT2
#endif

#define hw_SetDDC_WP()		 //(P4 &= ~BIT6)
#define hw_ClrDDC_WP()		 //(P4 |= BIT6)

#define Set_EEPROM_WP()     	hwEEPROM_WP=1
#define Clr_EEPROM_WP()     	hwEEPROM_WP=0

#define hw_SetFlashWP()		hwFLASH_WP=0 
#define hw_ClrFlashWP()		hwFLASH_WP=1

//*****************charles
#define hw_SetStandby()		//hwStandby_Pin=0    
#define hw_ClrStandby()		//hwStandby_Pin=1  

#if ENABLE_HDCP
#define hwHDCP_Hpd_Pin        		(P4 & BIT4)     // Annie 2006.8.21// Annie 2006.8.14  BIT5
#define hw_Set_HdcpHpd()   		//(P4 &= (~BIT4))  // Annie 2006.9.1 (P4 |= BIT4) // hwHDCP_Hpd_Pin=0  // 1 
#define hw_Clr_HdcpHpd()  		//(P4 |= BIT4) // Annie 2006.9.1 (P4 &= (~BIT4)) //  hwHDCP_Hpd_Pin=1  // 0 
#endif

#define hwAV_Mute_Pin			   P4


#define hw_Set_AV_Mute()		hwAV_Mute_Pin|=(BIT3) 
#define hw_Clr_AV_Mute()		       hwAV_Mute_Pin&= ~(BIT3)


#if ENABLE_HDMI
#define hw_DVISWITCH_Pin		P2_4
#define hw_SEL_HDMI()  			hw_DVISWITCH_Pin=0
#define hw_SEL_DVI()   			hw_DVISWITCH_Pin=1

#define hw_AUDIOSWITCH_Pin		P4&BIT3
#if 1//AUDIO_SW_SWAP
#define hw_Audio_HDMI()  		(P4 |= BIT3)
#define hw_Audio_PC()    			(P4 &= (~BIT3))
#else
#define hw_Audio_HDMI()  		(P4 &= (~BIT3))
#define hw_Audio_PC()    			(P4 |= BIT3)
#endif
#endif

//**********************


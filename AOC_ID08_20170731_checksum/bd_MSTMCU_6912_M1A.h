#ifndef __MAINBOARD_6912_M1A__
#define __MAINBOARD_6912_M1A__


#define CPU_CLOCK_KHZ		28636
#define CPU_CLOCK_MHZ		(CPU_CLOCK_KHZ/1000)

#ifdef TSUMXXT
#define XTAL_CLOCK_KHZ		14318
#define MST_CLOCK_MHZ		XTAL_CLOCK_KHZ
#else
#define MST_CLOCK_MHZ		28636
#endif

#define SERIAL_BAUD_RATE	 9600
#define UsePWMBrightness	1
#define BrightnessPWM		PWM6
#if  0//ScanKey//petit 20130324
#if PSAmberLEDFlashingType == Type_Breathing
#define PowerSavingLEDPWM   PWM1C
#endif
#endif
#if AudioFunc
#define VolumePWM           PWM0C
#endif
#define PowerKey			P2_6
//================================================
// Detect Cable
#define hwDSUBCable_Pin 	P1_4
//#define hwDVICable_Pin 	P1_3
//================================================
// Led
#define hwGreenLed_Pin		P0_6//P0_7
#define hwAmberLed_Pin		P0_7//P0_6
#define hwBlacklit_Pin		P2_7
#define hwPanel_Pin 	              P4
#define hwEEPROM_WP		// useless
#define hwFLASH_WP       		P2_4
//================================================
// mStar I2C
#define hwMS_ResetZ_Pin		P3_7

//#define ReduceDDC
#ifdef ReduceDDC
#define hwSystemVcc_Pin		P1_7
#endif


#define SetDDRPort	P3_4=ddrPort&BIT0;\
P3_5=ddrPort&BIT1;\
P3_6=ddrPort&BIT2;\
P3_7=ddrPort&BIT3;
//#define hwDDCWP_Pin	P4//P4_6//
//================================================
// I2C
#define hwI2C_SCL_Pin		P1_0
#define hwI2C_SDA_Pin		P1_1

#define Set_ms_ResetZ()		hwMS_ResetZ_Pin=1
#define Clr_ms_ResetZ()		hwMS_ResetZ_Pin=0
#define Set_ms_CSZ()			hwMS_CSZ_Pin=1
#define Clr_ms_CSZ()			hwMS_CSZ_Pin=0
#define Set_ms_SDA()		hwMS_SDA_Pin=1
#define Clr_ms_SDA()			hwMS_SDA_Pin=0
#define ms_SDAHi()			hwMS_SDA_Pin==1
#define Set_ms_SCL()			hwMS_SCL_Pin=1
#define Clr_ms_SCL()			hwMS_SCL_Pin=0
// mStar DDR
#define Set_ms_ALE()			hwMS_ALE_Pin=1
#define Clr_ms_ALE()			hwMS_ALE_Pin=0
#define Set_ms_WRZ()		hwMS_WRZ_Pin=1
#define Clr_ms_WRZ()			hwMS_WRZ_Pin=0
#define Set_ms_RDZ()			hwMS_RDZ_Pin=1
#define Clr_ms_RDZ()			hwMS_RDZ_Pin=0

#if Panel_Inv
     #define hw_SetPanel()		P4&=~BIT2
     #define hw_ClrPanel()		P4|=BIT2
#else 
     #define hw_SetPanel()		P4|=BIT2
     #define hw_ClrPanel()		P4&=~BIT2
#endif 

#if defined(UseVGACableReadWriteAllPortsEDID)&&!defined(ReduceDDC)
#define hw_VGA_EDID_SCL_Pin          P3_0
#define hw_VGA_EDID_SDA_Pin         P3_1

#define hw_DVI_EDID_SCL_Pin          P2_0
#define hw_DVI_EDID_SDA_Pin         P2_1

#define hw_HDMI_EDID_SCL_Pin          P3_0
#define hw_HDMI_EDID_SDA_Pin         P3_1

#define Set_i2c_SDA()		((LoadEDIDSelectPort == VGA1)?(hw_VGA_EDID_SDA_Pin=1):((LoadEDIDSelectPort == DVI1)?(hw_DVI_EDID_SDA_Pin=1):((LoadEDIDSelectPort == HDMI1)?(hw_HDMI_EDID_SDA_Pin=1):(hwI2C_SDA_Pin=1))))
#define Clr_i2c_SDA()		((LoadEDIDSelectPort == VGA1)?(hw_VGA_EDID_SDA_Pin=0):((LoadEDIDSelectPort == DVI1)?(hw_DVI_EDID_SDA_Pin=0):((LoadEDIDSelectPort == HDMI1)?(hw_HDMI_EDID_SDA_Pin=0):(hwI2C_SDA_Pin=0))))
#define i2c_SDAHi()			((LoadEDIDSelectPort == VGA1)?(hw_VGA_EDID_SDA_Pin):((LoadEDIDSelectPort == DVI1)?(hw_DVI_EDID_SDA_Pin):((LoadEDIDSelectPort == HDMI1)?(hw_HDMI_EDID_SDA_Pin):(hwI2C_SDA_Pin))))
#define i2c_SDALo()			((LoadEDIDSelectPort == VGA1)?(!(hw_VGA_EDID_SDA_Pin)):((LoadEDIDSelectPort == DVI1)?(!(hw_DVI_EDID_SDA_Pin)):((LoadEDIDSelectPort == HDMI1)?(!(hw_HDMI_EDID_SDA_Pin)):(!(hwI2C_SDA_Pin)))))

#define Set_i2c_SCL()			((LoadEDIDSelectPort == VGA1)?(hw_VGA_EDID_SCL_Pin=1):((LoadEDIDSelectPort == DVI1)?(hw_DVI_EDID_SCL_Pin=1):((LoadEDIDSelectPort == HDMI1)?(hw_HDMI_EDID_SCL_Pin=1):(hwI2C_SCL_Pin=1))))
#define Clr_i2c_SCL()			((LoadEDIDSelectPort == VGA1)?(hw_VGA_EDID_SCL_Pin=0):((LoadEDIDSelectPort == DVI1)?(hw_DVI_EDID_SCL_Pin=0):((LoadEDIDSelectPort == HDMI1)?(hw_HDMI_EDID_SCL_Pin=0):(hwI2C_SCL_Pin=0))))
#define i2c_SCLHi()			((LoadEDIDSelectPort == VGA1)?(hw_VGA_EDID_SCL_Pin):((LoadEDIDSelectPort == DVI1)?(hw_DVI_EDID_SCL_Pin):((LoadEDIDSelectPort == HDMI1)?(hw_HDMI_EDID_SCL_Pin):(hwI2C_SCL_Pin))))
#define i2c_SCLLo()			((LoadEDIDSelectPort == VGA1)?(!(hw_VGA_EDID_SCL_Pin)):((LoadEDIDSelectPort == DVI1)?(!(hw_DVI_EDID_SCL_Pin)):((LoadEDIDSelectPort == HDMI1)?(!(hw_HDMI_EDID_SCL_Pin)):(!(hwI2C_SCL_Pin)))))
#else
#define Set_i2c_SDA()		hwI2C_SDA_Pin=1
#define Clr_i2c_SDA()			hwI2C_SDA_Pin=0
#define i2c_SDAHi()			(hwI2C_SDA_Pin)
#define i2c_SDALo()			(!(hwI2C_SDA_Pin))
#define Set_i2c_SCL()			hwI2C_SCL_Pin=1
#define Clr_i2c_SCL()			hwI2C_SCL_Pin=0
#define i2c_SCLHi()			(hwI2C_SCL_Pin)
#define i2c_SCLLo()			(!(hwI2C_SCL_Pin))
#endif

#if 0//def ScanKey
extern void SK_CtrlGreenLed(bit ctrl_on);
extern void SK_CtrlAmberLed(bit ctrl_on);
#define hw_SetGreenLed()	    (SK_CtrlGreenLed(1))
#define hw_ClrGreenLed()	    (SK_CtrlGreenLed(0))
#define hw_SetAmberLed()    (SK_CtrlAmberLed(1))
#define hw_ClrAmberLed()    (SK_CtrlAmberLed(0))
#else
#define hw_SetGreenLed()	       hwGreenLed_Pin=0
#define hw_ClrGreenLed()	       hwGreenLed_Pin=1
#define hw_SetAmberLed()	hwAmberLed_Pin=0
#define hw_ClrAmberLed()	hwAmberLed_Pin=1
#endif


#define hw_SetBlacklit()	{hwBlacklit_Pin=1;BackLightActiveFlag=1;}   //20120103 hong
#define hw_ClrBlacklit()	{hwBlacklit_Pin=0;BackLightActiveFlag=0;}  //20120103 hong



#define hwMute_Pin                  P2_3
#define hw_SetMute()       	       hwMute_Pin=0
#define hw_ClrMute()        		hwMute_Pin=1

#define hwStandby_Pin		P2_3
#define hw_SetStandby() 		hwStandby_Pin=0
#define hw_ClrStandby()  		hwStandby_Pin=1


//#define hwDDC_WP_Pin	   	P1_5
#define hw_SetDDC_WP()		{BYTE xdata tempBank = mStar_ReadByte(0x00);\
							mStar_WriteByte(0x00, 0x01);\
                          				mStar_WriteByte(0xA3, mStar_ReadByte(0xA3)|BIT7);\
							mStar_WriteByte(0x00, tempBank);}
#define hw_ClrDDC_WP()		{BYTE xdata tempBank = mStar_ReadByte(0x00);\
							mStar_WriteByte(0x00, 0x01);\
							mStar_WriteByte(0xA4, mStar_ReadByte(0xA4)&~BIT7);\
							mStar_WriteByte(0xA3, mStar_ReadByte(0xA3)&~BIT7);\
							mStar_WriteByte(0x00, tempBank);}

#define Set_EEPROM_WP()     	// useless
#define Clr_EEPROM_WP()     	// useless
#define hw_SetFlashWP()		hwFLASH_WP=0
#define hw_ClrFlashWP()		hwFLASH_WP=1

#ifdef TSUMXXT
#define BD_DOUBLE_LVDS_CLK_SWING        (1) // set (1) to double swing of LVDS clock output
#endif

#if ENABLE_HDCP
    #define hwHDCP_Hpd_Pin      	   // P4
    #define hw_Set_HdcpHpd()    	   // hwHDCP_Hpd_Pin&=~BIT0
    #define hw_Clr_HdcpHpd()    	   //hwHDCP_Hpd_Pin|=BIT0
#endif

#if 0//def ScanKey
#define hw_SK_LEDVcc    P0_6
#define Set_SK_LEDVcc()	 hw_SK_LEDVcc = 0
#define Clr_SK_LEDVcc()	 hw_SK_LEDVcc = 1

#define hw_SK_KeyVcc   	P0_2
#define Set_SK_KeyVcc()  hw_SK_KeyVcc = 0
#define Clr_SK_KeyVcc()  hw_SK_KeyVcc = 1

#define hw_SK_SDA      	P0_1
#define hw_SK_SCL       P0_0

#define Set_SK_SCL()    hw_SK_SCL = 1
#define Clr_SK_SCL()   	hw_SK_SCL = 0
#define SK_SCLHi()	   	(hw_SK_SCL)
#define SK_SCLLo()	   	(!(hw_SK_SCL))

#define Set_SK_SDA()    hw_SK_SDA = 1
#define Clr_SK_SDA()   	hw_SK_SDA = 0
#define SK_SDAHi()	   	(hw_SK_SDA)
#define SK_SDALo()	   	(!(hw_SK_SDA))
#endif
#endif


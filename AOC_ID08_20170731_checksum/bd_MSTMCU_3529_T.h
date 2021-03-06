#define CPU_CLOCK_KHZ		28636
#define CPU_CLOCK_MHZ		(CPU_CLOCK_KHZ/1000)

#if defined (TSUMXXT) 
#define XTAL_CLOCK_KHZ		14318
#define MST_CLOCK_MHZ		XTAL_CLOCK_KHZ
#else
#define MST_CLOCK_MHZ		28636
#endif

#define SERIAL_BAUD_RATE	9600

#define UsePWMBrightness	1
#define BrightnessPWM		PWM1C
#define VolumePWM                 PWM0C

//================================================
// Detect Cable
#define hwDVICable_Pin	       P1_1  //xxx
#define hwDSUBCable_Pin		P1_1

//================================================
// Led
#define hwGreenLed_Pin		P0_6  // ok
#define hwAmberLed_Pin		P0_7  //ok

#define hwBlacklit_Pin		P1_2  //ok
#define hwPanel_Pin		       P1_0  //ok

#define hwMute_Pin		       P1_4  //ok
#define hwStandby_Pin		P1_4

//================================================
// mStar I2C
#define hwMS_ResetZ_Pin		P3_7  //???

//#define ReduceDDC

#ifdef ReduceDDC
#define hwSystemVcc_Pin		P1_7
#endif
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
#define hwSystemVcc_Pin		P1_7
// I2C
#define hwI2C_SCL_Pin		P1_4  //ok
#define hwI2C_SDA_Pin		P1_4  //ok
#define hwI2C_WP_Pin             P1_4

#define hwDDC_WP_Pin		       P2_4//P1_5 //ok
#define NVRAM_Protect_Pin          P2_3 //ok

#define EDIDLock_Pin                   P2_2  //ok


#define Set_ms_ResetZ()		hwMS_ResetZ_Pin=1
#define Clr_ms_ResetZ()		hwMS_ResetZ_Pin=0

#define Set_ms_CSZ()		hwMS_CSZ_Pin=1
#define Clr_ms_CSZ()		hwMS_CSZ_Pin=0

#define Set_ms_SDA()		hwMS_SDA_Pin=1
#define Clr_ms_SDA()		hwMS_SDA_Pin=0
#define ms_SDAHi()		hwMS_SDA_Pin==1

#define Set_ms_SCL()		hwMS_SCL_Pin=1
#define Clr_ms_SCL()		hwMS_SCL_Pin=0

// mStar DDR
#define Set_ms_ALE()		hwMS_ALE_Pin=1
#define Clr_ms_ALE()		hwMS_ALE_Pin=0

#define Set_ms_WRZ()		hwMS_WRZ_Pin=1
#define Clr_ms_WRZ()		hwMS_WRZ_Pin=0

#define Set_ms_RDZ()		hwMS_RDZ_Pin=1
#define Clr_ms_RDZ()		hwMS_RDZ_Pin=0

#define Set_i2c_WP()            hwI2C_WP_Pin=1
#define Clr_i2c_WP()            hwI2C_WP_Pin=0


#ifdef  UseVGACableReadWriteAllPortsEDID

#define hw_VGA_EDID_SCL_Pin          P3_0
#define hw_VGA_EDID_SDA_Pin         P3_1

#define hw_DVI_EDID_SCL_Pin          P2_0
#define hw_DVI_EDID_SDA_Pin         P2_1

#define hw_HDMI_EDID_SCL_Pin          P3_0
#define hw_HDMI_EDID_SDA_Pin         P3_1

#endif

#if defined(UseVGACableReadWriteAllPortsEDID)&&!defined(ReduceDDC)
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
#define Clr_i2c_SDA()		hwI2C_SDA_Pin=0
#define i2c_SDAHi()		(hwI2C_SDA_Pin)
#define i2c_SDALo()		(!(hwI2C_SDA_Pin))

#define Set_i2c_SCL()		hwI2C_SCL_Pin=1
#define Clr_i2c_SCL()		hwI2C_SCL_Pin=0
#define i2c_SCLHi()		(hwI2C_SCL_Pin)
#define i2c_SCLLo()		(!(hwI2C_SCL_Pin))
#endif









#define hw_SetGreenLed() 	hwGreenLed_Pin=0
#define hw_ClrGreenLed()	       hwGreenLed_Pin=1

#define hw_SetAmberLed()	hwAmberLed_Pin=0
#define hw_ClrAmberLed()	hwAmberLed_Pin=1

#define hw_SetBlacklit()	{hwBlacklit_Pin=0;BackLightActiveFlag=1;}
#define hw_ClrBlacklit()	{hwBlacklit_Pin=1;BackLightActiveFlag=0;}

#define hw_SetPanel()		hwPanel_Pin=0
#define hw_ClrPanel()		hwPanel_Pin=1

#define hw_SetMute()		hwMute_Pin=0
#define hw_ClrMute()		hwMute_Pin=1

#define hw_SetStandby()		hwStandby_Pin=0
#define hw_ClrStandby()		hwStandby_Pin=1

#define hw_SetDDC_WP()		 hwDDC_WP_Pin=0
#define hw_ClrDDC_WP()		 hwDDC_WP_Pin=1

#define hw_SetFlashWP()		 P2_7=0
#define hw_ClrFlashWP()		 P2_7=1

#define Set_EEPROM_WP() NVRAM_Protect_Pin=1
#define Clr_EEPROM_WP() NVRAM_Protect_Pin=0

#define Set_EDIDLock()   EDIDLock_Pin=0//   1
#define Clr_EDIDLock()   EDIDLock_Pin=1//  0

#define hw_SetAmberLedVS()	hwAmberLed_Pin=0
#define hw_ClrAmberLedVS()	hwAmberLed_Pin=1

#define hw_SetGreenLedVS()	hwGreenLed_Pin=0
#define hw_ClrGreenLedVS()	hwGreenLed_Pin=1

#if ENABLE_HDCP





    #define hwHDCP_Hpd_Pin        P1_3
    #define hw_Set_HdcpHpd()	hwHDCP_Hpd_Pin=0
    #define hw_Clr_HdcpHpd()		hwHDCP_Hpd_Pin=1


#endif  //EN

#ifdef TSUMXXT
#define BD_DOUBLE_LVDS_CLK_SWING        (0) // set (1) to double swing of LVDS clock output
#endif


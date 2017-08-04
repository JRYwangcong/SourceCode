#define XTAL_CLOCK_KHZ 		14318

#ifdef TSUMXXNX
#define CPU_CLOCK_KHZ		28636
#define CPU_CLOCK_MHZ		(CPU_CLOCK_KHZ/1000)
#define MST_CLOCK_MHZ		XTAL_CLOCK_KHZ

#define SERIAL_BAUD_RATE	9600
#define UsePWMBrightness	1
#define BrightnessPWM		PWM1C
//#define USEPWM3

#if ScanKey
#if PSAmberLEDFlashingType == Type_Breathing
#define PowerSavingLEDPWM   PWM1C
#endif
#endif


#define VolumePWM                 PWM2C
//================================================
// Detect Cable
#define hwDVICable_Pin		P0_3
#define hwDSUBCable_Pin 	P0_2
//================================================
// Led
#define hwGreenLed_Pin		P0_6	//P0_7
#define hwAmberLed_Pin		P0_7	//P1_5

#define hwBlacklit_Pin		P1_2	//P2_5
#define hwPanel_Pin 	        	P2_7	//P4
//================================================
// mStar I2C
#define hwMS_ResetZ_Pin		P3_7  

//#define ReduceDDC
#ifdef ReduceDDC
#define hwSystemVcc_Pin		P1_7	//P0_0
#endif


#define SetDDRPort			P3_4=ddrPort&BIT0;\
							P3_5=ddrPort&BIT1;\
							P3_6=ddrPort&BIT2;\
							P3_7=ddrPort&BIT3;
//================================================
// I2C
#ifdef  UseVGACableReadWriteAllPortsEDID

#define hw_VGA_EDID_SCL_Pin          P3_0
#define hw_VGA_EDID_SDA_Pin         P3_1

#define hw_DVI_EDID_SCL_Pin          P2_0
#define hw_DVI_EDID_SDA_Pin         P2_1

#define hw_HDMI_EDID_SCL_Pin          P3_0
#define hw_HDMI_EDID_SDA_Pin         P3_1

#endif
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
#define Clr_i2c_SDA()			hwI2C_SDA_Pin=0
#define i2c_SDAHi()			(hwI2C_SDA_Pin)
#define i2c_SDALo()			(!(hwI2C_SDA_Pin))
#define Set_i2c_SCL()			hwI2C_SCL_Pin=1
#define Clr_i2c_SCL()			hwI2C_SCL_Pin=0
#define i2c_SCLHi()			(hwI2C_SCL_Pin)
#define i2c_SCLLo()			(!(hwI2C_SCL_Pin))
#endif

#define hw_SetBlacklit()		{hwBlacklit_Pin=0;BackLightActiveFlag=1;}
#define hw_ClrBlacklit()		{hwBlacklit_Pin=1;BackLightActiveFlag=0;}

#define hw_SetPanel()		hwPanel_Pin=1	//P4&=~BIT5//hwPanel_Pin =0
#define hw_ClrPanel()			hwPanel_Pin=0	//P4|=BIT5//hwPanel_Pin =1

//Audio
#define hwMute_Pin			P1_4	//P1_3
#define hw_SetMute()			hwMute_Pin=0
#define hw_ClrMute()			hwMute_Pin=1

#define hwStandby_Pin		hwMute_Pin
#define hw_SetStandby()		(hw_SetMute())   
#define hw_ClrStandby()		(hw_ClrMute())	

#define hwDDC_WP_Pin		P2_2
#define hw_SetDDC_WP()		hwDDC_WP_Pin = 0
#define hw_ClrDDC_WP()		hwDDC_WP_Pin = 1

#define hwEEPROM_WP		P2_3
#define Set_EEPROM_WP()     	hwEEPROM_WP=1
#define Clr_EEPROM_WP()     	hwEEPROM_WP=0

#define hwFLASH_WP       		P2_4
#define hw_SetFlashWP()		hwFLASH_WP=0
#define hw_ClrFlashWP()		hwFLASH_WP=1

#if ENABLE_HDCP
#define hwHDCP_Hpd_Pin        P1_3
    #if ENABLE_HDMI
    #define hw_Set_HdcpHpd()	
    #define hw_Clr_HdcpHpd()
    #else
    #define hw_Set_HdcpHpd()	hwHDCP_Hpd_Pin=0
    #define hw_Clr_HdcpHpd()	hwHDCP_Hpd_Pin=1
    #endif
#endif

#if ScanKey
#define hw_SK_SDA      		P0_1	//P0_2
#define hw_SK_SCL       		P0_0	//P0_1

#define Set_SK_SCL()    		hw_SK_SCL = 1  
#define Clr_SK_SCL()     		hw_SK_SCL = 0
#define SK_SCLHi()	   		(hw_SK_SCL)
#define SK_SCLLo()	   		(!(hw_SK_SCL))

#define Set_SK_SDA()    		hw_SK_SDA = 1  
#define Clr_SK_SDA()     		hw_SK_SDA = 0
#define SK_SDAHi()	    		(hw_SK_SDA)
#define SK_SDALo()	    		(!(hw_SK_SDA))

#if Scan_Source == Cypress
#define hw_SK_KeyVcc   		P4
#define Set_SK_KeyVcc()  		P4&=~BIT0
#define Clr_SK_KeyVcc()  		P4|=BIT0
#define hw_SK_LEDVcc    		P0_6	//P0_7
#define Set_SK_LEDVcc()  		hw_SK_LEDVcc = 0
#define Clr_SK_LEDVcc()  		hw_SK_LEDVcc = 1

extern void SK_CtrlGreenLed(bit ctrl_on);
extern void SK_CtrlAmberLed(bit ctrl_on);

#define hw_SetGreenLed()	    	(SK_CtrlGreenLed(1))
#define hw_ClrGreenLed()	    	(SK_CtrlGreenLed(0))
#define hw_SetAmberLed()    	(SK_CtrlAmberLed(1))
#define hw_ClrAmberLed()    	(SK_CtrlAmberLed(0))
#endif
#else
#define hw_SetGreenLed()		hwGreenLed_Pin=1
#define hw_ClrGreenLed()		hwGreenLed_Pin=0
#define hw_SetAmberLed()	hwAmberLed_Pin=1
#define hw_ClrAmberLed()	hwAmberLed_Pin=0
#endif

#else
#define CPU_CLOCK_KHZ		28636
#define CPU_CLOCK_MHZ		(CPU_CLOCK_KHZ/1000)
#define MST_CLOCK_MHZ		28636

#define SERIAL_BAUD_RATE	9600
#define UsePWMBrightness	1
#define BrightnessPWM		PWM2C//PWM3
#define USEPWM3

#define VolumePWM			PWM1C
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
#define hwPanel_Pin 	        	P4
#define hwDDC_WP_Pin		P4//P4_6//
#define hwEEPROM_WP		P4// P4_7////2006-06-01 Andy 
#define hwFLASH_WP       		P2_4//P2_7//P1_5
//================================================
// mStar I2C
#define hwMS_ResetZ_Pin		P3_7  //???

//#define ReduceDDC
#ifdef ReduceDDC
#define hwSystemVcc_Pin		P0_0
#endif


#define SetDDRPort			P3_4=ddrPort&BIT0;\
							P3_5=ddrPort&BIT1;\
							P3_6=ddrPort&BIT2;\
							P3_7=ddrPort&BIT3;
//================================================
// I2C
#ifdef  UseVGACableReadWriteAllPortsEDID

#define hw_VGA_EDID_SCL_Pin          P3_0
#define hw_VGA_EDID_SDA_Pin         P3_1

#define hw_DVI_EDID_SCL_Pin          P2_0
#define hw_DVI_EDID_SDA_Pin         P2_1

#define hw_HDMI_EDID_SCL_Pin          P3_0
#define hw_HDMI_EDID_SDA_Pin         P3_1

#endif

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
#define hw_SetBlacklit()	{hwBlacklit_Pin=0;BackLightActiveFlag=1;}
#define hw_ClrBlacklit()	{hwBlacklit_Pin=1;BackLightActiveFlag=0;}

#define hw_SetPanel()	P4|=BIT5//	P4&=~BIT5//hwPanel_Pin =0
#define hw_ClrPanel()		P4&=~BIT5//P4|=BIT5//hwPanel_Pin =1

//Audio
#define hw_SetMute()			hwMute_Pin=0
#define hw_ClrMute()			hwMute_Pin=1
#define hwStandby_Pin		hwMute_Pin
#define hw_SetStandby()		(hw_SetMute())   
#define hw_ClrStandby()		(hw_ClrMute())	

#define hw_SetDDC_WP()		P4&=~BIT6//hwDDC_WP_Pin = 0
#define hw_ClrDDC_WP()		P4|=BIT6//hwDDC_WP_Pin = 1
#define Set_EEPROM_WP()     	P4|=BIT7//hwEEPROM_WP=1
#define Clr_EEPROM_WP()     	P4&=~BIT7//hwEEPROM_WP=0
#define hw_SetFlashWP()		hwFLASH_WP=0
#define hw_ClrFlashWP()		hwFLASH_WP=1
#if ENABLE_HDCP
    #define hwHDCP_Hpd_Pin        P1_2//P1_5
    #if ENABLE_HDMI
    #define hw_Set_HdcpHpd()	
    #define hw_Clr_HdcpHpd()
    #else
    #define hw_Set_HdcpHpd()	hwHDCP_Hpd_Pin=0
    #define hw_Clr_HdcpHpd()		hwHDCP_Hpd_Pin=1
    #endif
#endif

#if ScanKey
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
    #if Scan_Source == Cypress
    #define hw_SK_KeyVcc   P2_3
    #define Set_SK_KeyVcc()  hw_SK_KeyVcc = 0
    #define Clr_SK_KeyVcc()  hw_SK_KeyVcc = 1
    #define hw_SK_LEDVcc    P0_7
    #define Set_SK_LEDVcc()  hw_SK_LEDVcc = 0
    #define Clr_SK_LEDVcc()  hw_SK_LEDVcc = 1
    extern void SK_CtrlGreenLed(bit ctrl_on);
    extern void SK_CtrlAmberLed(bit ctrl_on);
    #define hw_SetGreenLed()	    (SK_CtrlGreenLed(1))
    #define hw_ClrGreenLed()	    (SK_CtrlGreenLed(0))
    #define hw_SetAmberLed()    (SK_CtrlAmberLed(1))
    #define hw_ClrAmberLed()    (SK_CtrlAmberLed(0))
    #endif
#else
#define hw_SetGreenLed()	hwGreenLed_Pin=1
#define hw_ClrGreenLed()	hwGreenLed_Pin=0
#define hw_SetAmberLed()	hwAmberLed_Pin=1
#define hw_ClrAmberLed()	hwAmberLed_Pin=0
#endif
#endif

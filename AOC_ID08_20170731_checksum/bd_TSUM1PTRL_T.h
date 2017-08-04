#define CPU_CLOCK_KHZ		28636
#define CPU_CLOCK_MHZ		(CPU_CLOCK_KHZ/1000)

#if defined (TSUMXXT) 
#define XTAL_CLOCK_KHZ		14318
#define MST_CLOCK_MHZ		XTAL_CLOCK_KHZ
#else
#define MST_CLOCK_MHZ		28636
#endif

#define SERIAL_BAUD_RATE		9600

#define UsePWMBrightness	1
#define BrightnessPWM		PWM0C//PWM1C
#define VolumePWM          	PWM1C//PWM0C

#define NoUsePin             0


//#define  ReduceDDC   
#ifdef ReduceDDC
#define hwSystemVcc_Pin		NoUsePin//P0_0
#endif
//================================================
// Detect Cable
#define hwDSUBCable_Pin		P1_5//P1_3//petit 20121031

//================================================
// Led
#define hwGreenLed_Pin		P0_7//P0_6  
#define hwAmberLed_Pin		P0_6//P0_7  
#define hw_SetGreenLed()	hwGreenLed_Pin=1
#define hw_ClrGreenLed()	hwGreenLed_Pin=0

#define hw_SetAmberLed()	hwAmberLed_Pin=1
#define hw_ClrAmberLed()	hwAmberLed_Pin=0
#define hwBlacklit_Pin		P1_3//P1_2
#define hwPanel_Pin		   	P1_2//P2_4  

#define hwMute_Pin		  	P2_7//P1_7//petit 20121031
#define hwStandby_Pin		P2_7//P1_7//petit 20121031

#define hwI2C_SCL_Pin		  P3_0
#define hwI2C_SDA_Pin		  P3_1

//================================================
// mStar I2C
#define hwMS_ResetZ_Pin		P3_7  

// DDR
#define SetDDRPort	P3_4=ddrPort&BIT0;\
					P3_5=ddrPort&BIT1;\
					P3_6=ddrPort&BIT2;\
					P3_7=ddrPort&BIT3;
//================================================
// I2C

#define hwDDC_WP_Pin		NoUsePin
#define NVRAM_Protect_Pin 

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

#ifdef  UseVGACableReadWriteAllPortsEDID

#define hw_VGA_EDID_SCL_Pin  	P3_0
#define hw_VGA_EDID_SDA_Pin  	P3_1

#define hw_DVI_EDID_SCL_Pin    	P2_0
#define hw_DVI_EDID_SDA_Pin   	P2_1

#define hw_HDMI_EDID_SCL_Pin  	P3_0
#define hw_HDMI_EDID_SDA_Pin  	P3_1

#endif

#if defined(UseVGACableReadWriteAllPortsEDID)&&!defined(ReduceDDC)
    #define Set_i2c_SDA()		((LoadEDIDSelectPort == VGA1)?(hw_VGA_EDID_SDA_Pin=1):((LoadEDIDSelectPort == DVI1)?(hw_DVI_EDID_SDA_Pin=1):((LoadEDIDSelectPort == HDMI1)?(hw_HDMI_EDID_SDA_Pin=1):(hwI2C_SDA_Pin=1))))
    #define Clr_i2c_SDA()		((LoadEDIDSelectPort == VGA1)?(hw_VGA_EDID_SDA_Pin=0):((LoadEDIDSelectPort == DVI1)?(hw_DVI_EDID_SDA_Pin=0):((LoadEDIDSelectPort == HDMI1)?(hw_HDMI_EDID_SDA_Pin=0):(hwI2C_SDA_Pin=0))))
    #define i2c_SDAHi()			((LoadEDIDSelectPort == VGA1)?(hw_VGA_EDID_SDA_Pin):((LoadEDIDSelectPort == DVI1)?(hw_DVI_EDID_SDA_Pin):((LoadEDIDSelectPort == HDMI1)?(hw_HDMI_EDID_SDA_Pin):(hwI2C_SDA_Pin))))
    #define i2c_SDALo()			((LoadEDIDSelectPort == VGA1)?(!(hw_VGA_EDID_SDA_Pin)):((LoadEDIDSelectPort == DVI1)?(!(hw_DVI_EDID_SDA_Pin)):((LoadEDIDSelectPort == HDMI1)?(!(hw_HDMI_EDID_SDA_Pin)):(!(hwI2C_SDA_Pin)))))
    		
    #define Set_i2c_SCL()		((LoadEDIDSelectPort == VGA1)?(hw_VGA_EDID_SCL_Pin=1):((LoadEDIDSelectPort == DVI1)?(hw_DVI_EDID_SCL_Pin=1):((LoadEDIDSelectPort == HDMI1)?(hw_HDMI_EDID_SCL_Pin=1):(hwI2C_SCL_Pin=1))))
    #define Clr_i2c_SCL()		((LoadEDIDSelectPort == VGA1)?(hw_VGA_EDID_SCL_Pin=0):((LoadEDIDSelectPort == DVI1)?(hw_DVI_EDID_SCL_Pin=0):((LoadEDIDSelectPort == HDMI1)?(hw_HDMI_EDID_SCL_Pin=0):(hwI2C_SCL_Pin=0))))
    #define i2c_SCLHi()			((LoadEDIDSelectPort == VGA1)?(hw_VGA_EDID_SCL_Pin):((LoadEDIDSelectPort == DVI1)?(hw_DVI_EDID_SCL_Pin):((LoadEDIDSelectPort == HDMI1)?(hw_HDMI_EDID_SCL_Pin):(hwI2C_SCL_Pin))))
    #define i2c_SCLLo()			((LoadEDIDSelectPort == VGA1)?(!(hw_VGA_EDID_SCL_Pin)):((LoadEDIDSelectPort == DVI1)?(!(hw_DVI_EDID_SCL_Pin)):((LoadEDIDSelectPort == HDMI1)?(!(hw_HDMI_EDID_SCL_Pin)):(!(hwI2C_SCL_Pin)))))
#else







#define Set_i2c_WP()			hwI2C_WP_Pin=1
#define Clr_i2c_WP()			hwI2C_WP_Pin=0
//===========================

#define Set_i2c_SDA()			hwI2C_SDA_Pin=1
#define Clr_i2c_SDA()			hwI2C_SDA_Pin=0
#define i2c_SDAHi()			(hwI2C_SDA_Pin)
#define i2c_SDALo()			(!(hwI2C_SDA_Pin))

#define Set_i2c_SCL()			hwI2C_SCL_Pin=1
#define Clr_i2c_SCL()			hwI2C_SCL_Pin=0
#define i2c_SCLHi()			(hwI2C_SCL_Pin)
#define i2c_SCLLo()			(!(hwI2C_SCL_Pin))


#endif



#define HW_VCOM_I2C_SCL_PIN      			P1_0
#define HW_VCOM_I2C_SDA_PIN        			P1_1

#define HW_SET_VCOM_I2C_SDA()          		HW_VCOM_I2C_SDA_PIN=1
#define HW_CLR_VCOM_I2C_SDA()            	HW_VCOM_I2C_SDA_PIN=0
#define HW_GET_VCOM_I2C_SDA_HI()     		(HW_VCOM_I2C_SDA_PIN)
#define HW_GET_VCOM_I2C_SDA_Lo()     		(!(HW_VCOM_I2C_SDA_PIN))
#define HW_SET_VCOM_I2C_SCL()             	HW_VCOM_I2C_SCL_PIN=1
#define HW_CLR_VCOM_I2C_SCL()            	HW_VCOM_I2C_SCL_PIN=0
#define HW_GET_VCOM_I2C_SCL_HI()     		(HW_VCOM_I2C_SCL_PIN)
#define HW_GET_VCOM_I2C_SCL_Lo()       		(!(HW_VCOM_I2C_SCL_PIN))

#define hw_SetBlacklit()		{hwBlacklit_Pin=0;BackLightActiveFlag=1;}
#define hw_ClrBlacklit()		{hwBlacklit_Pin=1;BackLightActiveFlag=0;}

#define hw_SetPanel()			hwPanel_Pin=1
#define hw_ClrPanel()			hwPanel_Pin=0

#define hw_SetMute()			hwMute_Pin=0
#define hw_ClrMute()			hwMute_Pin=1

#define hw_SetStandby()			hwStandby_Pin=0
#define hw_ClrStandby()			hwStandby_Pin=1

#define hw_SetDDC_WP()		 	hwDDC_WP_Pin=0
#define hw_ClrDDC_WP()		 	hwDDC_WP_Pin=1

#define hw_SetFlashWP()		 	P2_4=0//P2_7=0
#define hw_ClrFlashWP()		 	P2_4=1//P2_7=1

#define Set_EEPROM_WP() 		//NVRAM_Protect_Pin=1
#define Clr_EEPROM_WP() 		//NVRAM_Protect_Pin=0

#ifdef TSUMXXT
#define BD_DOUBLE_LVDS_CLK_SWING        (0) // set (1) to double swing of LVDS clock output
#endif


#define P0_Init                BIT0 |  BIT1 |  BIT2 |  BIT3 |  BIT4 |  BIT5 |  !BIT6 |  !BIT7    //
#define P1_Init                BIT0 |  BIT1 |  !BIT2 | BIT3 |  BIT4 |  BIT5 |  BIT6 |  BIT7    //
#define P2_Init                BIT0 |  BIT1 |  BIT2 |  BIT3 |  BIT4 |  BIT5 |  BIT6 |  BIT7   // 
#define P4_Init                BIT0 |  BIT1 |  BIT2 |  BIT3 |  BIT4 |  BIT5 |  BIT6 |  BIT7    //

#define InitMCU_P0_FMD           !BIT0 | !BIT1 | !BIT2 | !BIT3 | !BIT4 | !BIT5 | BIT6 | BIT7    //
#define InitMCU_P0_OUT_EN     !BIT0 | !BIT1 | !BIT2 | !BIT3 | !BIT4 | !BIT5 | BIT6 | BIT7
#define InitMCU_P0_ROD_EN     !BIT0 | !BIT1 | !BIT2 | !BIT3 | !BIT4 | !BIT5 | !BIT6 | !BIT7

#define InitMCU_P1_FMD           !BIT0 | !BIT1 | BIT2 | BIT3 | !BIT4 | !BIT5 | !BIT6 | !BIT7    //
#define InitMCU_P1_OUT_EN     !BIT0 | !BIT1 | BIT2 |!BIT3 | !BIT4 | !BIT5 | !BIT6 | !BIT7
#define InitMCU_P1_ROD_EN     !BIT0 | !BIT1 | !BIT2 | !BIT3 | !BIT4 | !BIT5 | !BIT6 | !BIT7

#define InitMCU_P2_FMD           !BIT0 | !BIT1 | !BIT2 | !BIT3 | BIT4 | !BIT5 | !BIT6 | BIT7     //  
#define InitMCU_P2_OUT_EN     !BIT0 | !BIT1 | !BIT2 | !BIT3 | BIT4 | !BIT5 | !BIT6 | BIT7
#define InitMCU_P2_ROD_EN     !BIT0 | !BIT1 | !BIT2 | !BIT3 | !BIT4 | !BIT5 | !BIT6 | !BIT7

#define InitMCU_P3_FMD           !BIT0 | !BIT1 | !BIT2 | !BIT3 | !BIT4 | !BIT5 | !BIT6 | !BIT7
#define InitMCU_P3_OUT_EN     !BIT0 | !BIT1 | !BIT2 | !BIT3 | !BIT4 | !BIT5 | !BIT6 | !BIT7
#define InitMCU_P3_ROD_EN     !BIT0 | !BIT1 | !BIT2 | !BIT3 | !BIT4 | !BIT5 | !BIT6 | !BIT7

#define InitMCU_P4_FMD           !BIT0 | !BIT1 | !BIT2 | !BIT3 | !BIT4 | !BIT5 | !BIT6 | !BIT7
#define InitMCU_P4_OUT_EN     !BIT0 | !BIT1 | !BIT2 | !BIT3 | !BIT4 | !BIT5 | !BIT6 | !BIT7
#define InitMCU_P4_ROD_EN     !BIT0 | !BIT1 | !BIT2 | !BIT3 | !BIT4 | !BIT5 | !BIT6 | !BIT7



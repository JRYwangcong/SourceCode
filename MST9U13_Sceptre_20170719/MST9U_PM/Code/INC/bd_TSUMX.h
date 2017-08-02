
#ifndef __BD_MSTMCU__
#define __BD_MSTMCU__

#define MCU_SPEED_INDEX         IDX_MCU_CLK_216MHZ // need sync with Sboot setting for AC_ON consistency
#define SPI_SPEED_INDEX         IDX_SPI_CLK_43MHZ // need sync with Sboot setting for AC_ON consistency
#define SPI_MODE                SPI_MODE_FR // need sync with Sboot setting for AC_ON consistency
#define HPD_INVERSE             0 // If it has BJT circuit on board, the HPD_INVERSE should be set to TRUE
#define SPI_SSC_EN              (SPI_SPEED_INDEX==IDX_SPI_CLK_SSC)// use memory clock source, need to enable memclk SSC, and divide it to the one closest & lower to SPI_SPEED_INDEX

#define USE_SW_I2C 1

typedef enum // UART GPIO
{
    UART_GPIO02_03,
    UART_GPIO40_41,  // VGA ?
    UART_GPIO50_51,
    UART_GPIOX03_04, // combo 0
    UART_GPIOX13_14, // combo 1
    UART_GPIOX23_24, // combo 2
    UART_GPIOX33_34, // combo 3
    UART_GPIOX43_44, // combo 4
    UART_GPIOX53_54 // combo 5
}UART_GPIO;

typedef enum // UART Engine
{
    UART_ENGINE_51_UART0,
    UART_ENGINE_51_UART1,
    UART_ENGINE_R2_HK,
    UART_ENGINE_R2_NONHK,
    UART_ENGINE_DW_uart,
    RESERVED
}UART_ENGINE;

#define DFT_51_UART0_GPIO           UART_GPIO40_41 // default mux uart 0 - VGA
#define DFT_51_UART1_GPIO           UART_GPIOX23_24

#define PORT_FLOW_0     MUX_ADC
#define PORT_FLOW_1     MUX_COMBO0
#define PORT_FLOW_2     MUX_COMBO1
#define PORT_FLOW_3     MUX_COMBO2
#define PORT_FLOW_4     MUX_COMBO3
#define PORT_FLOW_5     MUX_COMBO4
//#define PORT_FLOW_6
//#define PORT_FLOW_7

#define PORT_TYPE_0     TYPE_ANALOG
#if(ENABLE_DAISY_CHAIN)
#define PORT_TYPE_1     TYPE_RESERVED  // don't change
#define PORT_TYPE_2     (TYPE_DP|TYPE_HBR2_0|TYPE_DAISY_CHAIN)  // don't change
#else
#define PORT_TYPE_1     (TYPE_DP |TYPE_HBR2_0)
#define PORT_TYPE_2     (TYPE_DP |TYPE_HBR2_1)
#endif
#define PORT_TYPE_3     (TYPE_HDMI |TYPE_MHL)
#define PORT_TYPE_4     (TYPE_HDMI |TYPE_MHL)
#define PORT_TYPE_5     TYPE_DVI_DUAL

// Define TMDS port used
#define TMDS_Port_D0   			0 //DP
#define TMDS_Port_D1      		0 //DP
#define TMDS_Port_D2       		1 //HDMI
#define TMDS_Port_D3       		1 //HDMI
#define TMDS_Port_D4       		1 //DVI dual
#define TMDS_Port_D5       		0 //none

#define  DP_Port_D0				1
#define  DP_Port_D1				1

// Mapping to InputPortType
#define Input_DVI_C1            0//Input_Nothing
#define Input_DVI_C2            0//Input_Nothing
#define Input_DVI_C3            0//Input_Nothing
#define Input_HDMI_C1           0//Input_Digital
#define Input_HDMI_C2           0//Input_Digital2
#define Input_HDMI_C3           0//Input_Digital3
#define Input_Displayport_C2    0//Input_Nothing
#define Input_Displayport_C3    0//Input_Nothing

// For Preprocessing
#define CInput_Nothing          0
#define CInput_Digital          1
#define CInput_Digital2         2
#define CInput_Digital3         3

#define CInput_DVI_C1           CInput_Nothing
#define CInput_DVI_C2           CInput_Nothing
#define CInput_DVI_C3           CInput_Nothing
#define CInput_HDMI_C1          CInput_Digital
#define CInput_HDMI_C2          CInput_Digital2
#define CInput_HDMI_C3          CInput_Digital3
#define CInput_Displayport_C2   CInput_Nothing
#define CInput_Displayport_C3   CInput_Nothing

//=================================================
//for switch input port priority and input source OSD item priority setting
#define DVI_PORT_PRIORITY       5 //Input_Digital5
#define DVI2ND_PORT_PRIORITY    6 //Input_Nums
#define DVI3ND_PORT_PRIORITY    6 //Input_Nums
#define HDMI_PORT_PRIORITY      3 //Input_Digital3
#define HDMI2ND_PORT_PRIORITY   4 //Input_Digital4
#define HDMI3ND_PORT_PRIORITY   6 //Input_Nums
#define DP_PORT_PRIORITY        1 //Input_Digital1
#define DP2ND_PORT_PRIORITY     2 //Input_Digital2
#define DP3ND_PORT_PRIORITY     6 //Input_Nums

#define FIXED_DP_PORT          	Input_Digital1
#define FIXED_MHL_PORT        	Input_Digital3

#define DVI_PORT                ( ((CInput_DVI_C1!=CInput_Nothing)?TMDS_PORT_A:TMDS_PORT_UNUSE) | ((CInput_DVI_C2!=CInput_Nothing)?TMDS_PORT_B:TMDS_PORT_UNUSE) | ((CInput_DVI_C3!=CInput_Nothing)?TMDS_PORT_C:TMDS_PORT_UNUSE) )
#define DVI_HPD_PORT            ( ((CInput_DVI_C1!=CInput_Nothing)?TMDS_PORT_A:TMDS_PORT_UNUSE) | ((CInput_DVI_C2!=CInput_Nothing)?TMDS_PORT_B:TMDS_PORT_UNUSE) | ((CInput_DVI_C3!=CInput_Nothing)?TMDS_PORT_C:TMDS_PORT_UNUSE) )
#define DVI_DDC_PORT            ( ((CInput_DVI_C1!=CInput_Nothing)?TMDS_PORT_A:TMDS_PORT_UNUSE) | ((CInput_DVI_C2!=CInput_Nothing)?TMDS_PORT_B:TMDS_PORT_UNUSE) | ((CInput_DVI_C3!=CInput_Nothing)?TMDS_PORT_C:TMDS_PORT_UNUSE) )
#define DVI_MASTER_PORT         ( ((CInput_DVI_C1!=CInput_Nothing)?TMDS_PORT_A:TMDS_PORT_UNUSE) | ((CInput_DVI_C2!=CInput_Nothing)?TMDS_PORT_B:TMDS_PORT_UNUSE) | ((CInput_DVI_C3!=CInput_Nothing)?TMDS_PORT_C:TMDS_PORT_UNUSE) )

//set to 0 if not DVI dual port enable
//#define DVI_SLAVE_PORT          TMDS_PORT_UNUSE

#define HDMI_PORT               ( ((CInput_HDMI_C1!=CInput_Nothing)?TMDS_PORT_A:TMDS_PORT_UNUSE) | ((CInput_HDMI_C2!=CInput_Nothing)?TMDS_PORT_B:TMDS_PORT_UNUSE) | ((CInput_HDMI_C3!=CInput_Nothing)?TMDS_PORT_C:TMDS_PORT_UNUSE) )
#define HDMI_HPD_PORT           ( ((CInput_HDMI_C1!=CInput_Nothing)?TMDS_PORT_A:TMDS_PORT_UNUSE) | ((CInput_HDMI_C2!=CInput_Nothing)?TMDS_PORT_B:TMDS_PORT_UNUSE) | ((CInput_HDMI_C3!=CInput_Nothing)?TMDS_PORT_C:TMDS_PORT_UNUSE) )
#define HDMI_DDC_PORT           ( ((CInput_HDMI_C1!=CInput_Nothing)?TMDS_PORT_A:TMDS_PORT_UNUSE) | ((CInput_HDMI_C2!=CInput_Nothing)?TMDS_PORT_B:TMDS_PORT_UNUSE) | ((CInput_HDMI_C3!=CInput_Nothing)?TMDS_PORT_C:TMDS_PORT_UNUSE) )

// DDC port used
#define DDC_Port_D0             ( (CInput_DVI_C1!=CInput_Nothing) || (CInput_HDMI_C1!=CInput_Nothing) )
#define DDC_Port_D1             ( (CInput_DVI_C2!=CInput_Nothing) || (CInput_HDMI_C2!=CInput_Nothing) )
#define DDC_Port_D2             ( (CInput_DVI_C3!=CInput_Nothing) || (CInput_HDMI_C3!=CInput_Nothing) )

#define SERIAL_BAUD_RATE        9600

//#define BrightnessPWM           _PWM7_ // Driving Non-LED BL Module
#define Init_BrightnessPWM()    (MEM_MSWRITE_BYTE_MASK(REG_0208,BIT7,BIT7), MEM_MSWRITE_BYTE_MASK(REG_0281,BIT3,BIT3))



//#define PM_POWERKEY_WAKEUP      GPIO02_INT

#define NonUsepin
//================================================================================================================
//================================================================================================================
// 1.Write protection:
#define hwDDC_WP_Pin            //Pull to Ground in Schematic
#define hwEEPROM_WP             //GPIO03
#define hwFlash_WP_Pin          //GPIO10

// ddc eeprom wp be pulled high to 5V, set to input for output high level
#define hw_SetDDC_WP()          //(MEM_MSWRITE_BIT(_REG_PMGPIO2_OUT, 1, BIT5), DDCWPActiveFlag=1)//(MEM_MSWRITE_BIT(_REG_PMGPIO1_OUT, 0, BIT6), MEM_MSWRITE_BIT(_REG_PMGPIO1_OEZ, 0, BIT6), DDCWPActiveFlag=0)
#define hw_ClrDDC_WP()          //(MEM_MSWRITE_BIT(_REG_PMGPIO2_OUT, 0, BIT5), DDCWPActiveFlag=0)
#define Init_hwDDC_WP_Pin()     //MEM_MSWRITE_BIT(_REG_PMGPIO2_OEZ, 0, BIT5)//(MEM_MSWRITE_BIT(_REG_PMGPIO1_OEZ, 1, BIT6)) // output disable

#define Set_EEPROM_WP()         MEM_MSWRITE_BIT(_REG_GPIO2_OUT, 1, BIT3)
#define Clr_EEPROM_WP()         MEM_MSWRITE_BIT(_REG_GPIO2_OUT, 0, BIT3)
#define Init_hwEEPROM_WP()      MEM_MSWRITE_BIT(_REG_GPIO2_OEZ, 0, BIT3)

#define hw_SetFlashWP()         MEM_MSWRITE_BIT(_REG_GPIO1_OUT, 0, BIT0)
#define hw_ClrFlashWP()         MEM_MSWRITE_BIT(_REG_GPIO1_OUT, 1, BIT0)
#define Init_hwFlash_WP_Pin()   MEM_MSWRITE_BIT(_REG_GPIO1_OEZ, 0, BIT0)

//================================================================================================================
//================================================================================================================
// 2.Backlight and panelon:
#define hwBlacklit_Pin          //GPIOL10
#define hwPanel_Pin             //GPIOL8
#define hw_SetBlacklit()        (MEM_MSWRITE_BIT(_REG_GPIO0_OUT, 0, BIT6))
#define hw_ClrBlacklit()        (MEM_MSWRITE_BIT(_REG_GPIO0_OUT, 1, BIT6))
#define Init_hwBlacklit_Pin()   MEM_MSWRITE_BIT(_REG_GPIO0_OEZ, 0, BIT6)

#define hw_SetPanel()           MEM_MSWRITE_BIT(_REG_GPIO0_OUT, 0, BIT5)
#define hw_ClrPanel()           MEM_MSWRITE_BIT(_REG_GPIO0_OUT, 1, BIT5)
#define Init_hwPanel_Pin()      MEM_MSWRITE_BIT(_REG_GPIO0_OEZ, 0, BIT5)

//================================================================================================================
//================================================================================================================
// 4.Detect Cable
#define CABLE_DETECT_VGA_USE_SAR        0
//#define CABLE_DETECT_DVI_USE_SAR      0
//#define CABLE_DETECT_HDMI_USE_SAR     0

#if DP_Port_D0
#define hw_DP0_SDMPin()                  (MEM_MSWRITE_BIT(_REG_GPIO_X0_OEZ, TRUE, BIT0))
#define hwSDMDP0Pin_Pin                 (_bit0_(MEM_MSREAD_BYTE(_REG_GPIO_X0_IN)))  	//GPIOX00
#else
#define hw_DP0_SDMPin()           	   0
#define hwSDMDP0Pin_Pin                 (_bit0_(MEM_MSREAD_BYTE(_REG_GPIO_X0_IN)))  	//GPIOX00
#endif

#if DP_Port_D1
	#if ENABLE_DC_SDM_FORWARD
#define hwSDMDP1Pin_Pin         		(KEYPAD_SAR10<=((255ul*250)/330)?TRUE:FALSE)
	#else
#define hw_DP1_SDMPin()           	   (MEM_MSWRITE_BIT(_REG_GPIO_X1_OEZ, TRUE, BIT0))
#define hwSDMDP1Pin_Pin                 (_bit0_(MEM_MSREAD_BYTE(_REG_GPIO_X1_IN)))  	//GPIOX00
	#endif
#else
#define hw_DP1_SDMPin()          	   0
#define hwSDMDP1Pin_Pin                 (_bit0_(MEM_MSREAD_BYTE(_REG_GPIO_X1_IN)))  	//GPIOX00
#endif
#if CABLE_DETECT_VGA_USE_SAR
#define hwDSUBCable_Pin                 0
#define Init_hwDSUBCable_Pin()
#define CABLE_DET_VGA_SAR               SAR3_EN // 2.8V
#else
#define hwDSUBCable_Pin                 (_bit3_(MEM_MSREAD_BYTE(_REG_GPIO0_IN)))  //GPIO03
#define Init_hwDSUBCable_Pin()          (MEM_MSWRITE_BYTE_MASK(_REG_GPIO0_OEZ, BIT3, BIT3))
#define CABLE_DET_VGA_SAR               0
#endif

#if TMDS_Port_D0
#define hwDVI0Cable_Pin                 (KEYPAD_SAR06)
#define Init_hwDVI0Cable_Pin()          (MEM_MSWRITE_BIT(REG_3A68,1,BIT6)) // Enable SAR ch
#define CABLE_DET_DVI0_SAR              SAR6_EN
#else
#define hwDVI0Cable_Pin                 0
#define Init_hwDVI0Cable_Pin()          (MEM_MSWRITE_BYTE_MASK(_REG_GPIO_X0_OEZ,BIT0,BIT0))
#define CABLE_DET_DVI0_SAR              0
#endif

#if TMDS_Port_D1
#define hwDVI1Cable_Pin                 (KEYPAD_SAR05)
#define Init_hwDVI1Cable_Pin()          (MEM_MSWRITE_BIT(REG_3A68,1,BIT5)) // Enable SAR ch
#define CABLE_DET_DVI1_SAR              SAR5_EN
#else
#define hwDVI1Cable_Pin                 0
#define Init_hwDVI1Cable_Pin()          (MEM_MSWRITE_BYTE_MASK(_REG_GPIO_X1_OEZ,BIT0,BIT0))
#define CABLE_DET_DVI1_SAR              0
#endif

#if TMDS_Port_D2//SAR9
#define hwDVI2Cable_Pin                 (KEYPAD_SAR09)
#define Init_hwDVI2Cable_Pin()          (MEM_MSWRITE_BIT(REG_003A69,1,BIT1))
#define CABLE_DET_DVI2_SAR              SAR9_EN
#else
#define hwDVI2Cable_Pin                 0
#define Init_hwDVI2Cable_Pin()
#define CABLE_DET_DVI2_SAR              0
#endif

#if TMDS_Port_D3//SAR8
#define hwDVI3Cable_Pin                 (KEYPAD_SAR08)
#define Init_hwDVI3Cable_Pin()          (MEM_MSWRITE_BIT(REG_003A69,1,BIT0))
#define CABLE_DET_DVI3_SAR              SAR8_EN
#else
#define hwDVI3Cable_Pin                 0
#define Init_hwDVI3Cable_Pin()
#define CABLE_DET_DVI3_SAR              0
#endif

#if TMDS_Port_D4
#define hwDVI4Cable_Pin                 0
#define Init_hwDVI4Cable_Pin()          (MEM_MSWRITE_BYTE_MASK(_REG_GPIO_X5_OEZ,BIT1,BIT1))
#define CABLE_DET_DVI4_SAR              0
#else
#define hwDVI4Cable_Pin                 0
#define Init_hwDVI4Cable_Pin()
#define CABLE_DET_DVI4_SAR              0
#endif

#if TMDS_Port_D5
#define hwDVI5Cable_Pin                 0
#define Init_hwDVI5Cable_Pin()
#define CABLE_DET_DVI5_SAR              0
#else
#define hwDVI5Cable_Pin                 0
#define Init_hwDVI5Cable_Pin()
#define CABLE_DET_DVI5_SAR              0
#endif


#define CABLE_DET_SAR_EN                (CABLE_DET_VGA_SAR)
#define CABLE_DET_SAR                   0
#define CABLE_DET_SAR2_EN               (CABLE_DET_DVI0_SAR|CABLE_DET_DVI1_SAR|CABLE_DET_DVI2_SAR)

//================================================================================================================
//================================================================================================================
// 5.Led
#define hwGreenLed_Pin                  // GPIO22
#define hwAmberLed_Pin                  // GPIO23

#define hw_SetGreenLed()                {MEM_MSWRITE_BIT(_REG_GPIO2_OUT,1,BIT1);}
#define hw_ClrGreenLed()                {MEM_MSWRITE_BIT(_REG_GPIO2_OUT,0,BIT1);}
#define Init_hwGreenLed_Pin()           MEM_MSWRITE_BIT(_REG_GPIO2_OEZ, 0,BIT1)// output enable
#define hw_GreenLedState()              (_bit1_(MEM_MSREAD_BYTE(_REG_GPIO2_IN)))

#define hw_SetAmberLed()                {MEM_MSWRITE_BIT(_REG_GPIO2_OUT,1,BIT3);MEM_MSWRITE_BIT(_REG_GPIO2_OUT, 0, BIT3);}
#define hw_ClrAmberLed()                {MEM_MSWRITE_BIT(_REG_GPIO2_OUT,0,BIT3);MEM_MSWRITE_BIT(_REG_GPIO2_OUT, 1, BIT3);}
#define Init_hwAmberLed_Pin()           MEM_MSWRITE_BIT(_REG_GPIO2_OEZ, 0,BIT3)// output enable

//================================================================================================================
//================================================================================================================
// 6.Audio control:
#define hwMute_Pin                      //NO_USE_PIN
#define hw_SetMute()                    //(MEM_MSWRITE_BIT(_REG_PMGPIO1_OUT, FALSE, BIT4))
#define hw_ClrMute()                    //(MEM_MSWRITE_BIT(_REG_PMGPIO1_OUT, TRUE, BIT4))
#define Init_hwMute_Pin()               //(MEM_MSWRITE_BIT(_REG_PMGPIO1_OEZ, FALSE, BIT4))

#define hw_SetAudio_SD()                //NO_USE_PIN
#define hw_ClrAudio_SD()
#define Init_Audio_SD_Pin()
#define Init_SPIFGPIO_Pin()   {MEM_MSWRITE_BIT(REG_0206,TRUE,BIT2);}//[0]S/PDIF output control on GPIO01
                                                                    //[2]S/PDIF output control on GPIO_AU5
//Since GPIO01 is the power switch on demo board, output S/PDIF on this pad will cause latch up when AC on.
//The register definition is wrong.

//================================================================================================================
//================================================================================================================
// I2C
#if defined(UseVGACableReadWriteAllPortsEDID) //&&!defined(UseInternalDDCRam)
#define Set_i2c_SDA_VGA()           //(MEM_MSWRITE_BIT(_REG_PMGPIO1_OEZ, TRUE, BIT2))
#define Clr_i2c_SDA_VGA()           //(MEM_MSWRITE_BIT(_REG_PMGPIO1_OUT, FALSE, BIT2), MEM_MSWRITE_BIT(_REG_PMGPIO1_OEZ, FALSE, BIT2))
#define i2c_SDAHi_VGA()             //(_bit2_(MEM_MSREAD_BYTE(_REG_PMGPIO1_IN)))
#define i2c_SDALo_VGA()             //(!i2c_SDAHi())
#define Set_i2c_SCL_VGA()           //(MEM_MSWRITE_BIT(_REG_PMGPIO1_OEZ, TRUE, BIT3))
#define Clr_i2c_SCL_VGA()           //(MEM_MSWRITE_BIT(_REG_PMGPIO1_OUT, FALSE, BIT3), MEM_MSWRITE_BIT(_REG_PMGPIO1_OEZ, FALSE, BIT3))
#define i2c_SCLHi_VGA()             //(_bit3_(MEM_MSREAD_BYTE(_REG_PMGPIO1_IN)))
#define i2c_SCLLo_VGA()             //(!i2c_SCLHi())

#define Set_i2c_SDA_DVI()           //(MEM_MSWRITE_BIT(_REG_PMGPIO2_OEZ, TRUE, BIT1))
#define Clr_i2c_SDA_DVI()           //(MEM_MSWRITE_BIT(_REG_PMGPIO2_OUT, FALSE, BIT1), MEM_MSWRITE_BIT(_REG_PMGPIO2_OEZ, FALSE, BIT1))
#define i2c_SDAHi_DVI()             //(_bit1_(MEM_MSREAD_BYTE(_REG_PMGPIO2_IN)))
#define i2c_SDALo_DVI()             //(!i2c_SDAHi())
#define Set_i2c_SCL_DVI()           //(MEM_MSWRITE_BIT(_REG_PMGPIO2_OEZ, TRUE, BIT2))
#define Clr_i2c_SCL_DVI()           //(MEM_MSWRITE_BIT(_REG_PMGPIO2_OUT, FALSE, BIT2), MEM_MSWRITE_BIT(_REG_PMGPIO2_OEZ, FALSE, BIT2))
#define i2c_SCLHi_DVI()             //(_bit2_(MEM_MSREAD_BYTE(_REG_PMGPIO2_IN)))
#define i2c_SCLLo_DVI()             //(!i2c_SCLHi())

#define Set_i2c_SDA()               //((LoadEDIDSelectPort == DVI1)?(Set_i2c_SDA_DVI()):(Set_i2c_SDA_VGA()))
#define Clr_i2c_SDA()               //((LoadEDIDSelectPort == DVI1)?(Clr_i2c_SDA_DVI()):(Clr_i2c_SDA_VGA()))
#define i2c_SDAHi()                 0//((LoadEDIDSelectPort == DVI1)?(i2c_SDAHi_DVI()):(i2c_SDAHi_VGA()))
#define i2c_SDALo()                 0//((LoadEDIDSelectPort == DVI1)?(i2c_SDALo_DVI()):(i2c_SDALo_VGA()))

#define Set_i2c_SCL()               //((LoadEDIDSelectPort == DVI1)?(Set_i2c_SCL_DVI()):(Set_i2c_SCL_VGA()))
#define Clr_i2c_SCL()               //((LoadEDIDSelectPort == DVI1)?(Clr_i2c_SCL_DVI()):(Clr_i2c_SCL_VGA()))
#define i2c_SCLHi()                 0//((LoadEDIDSelectPort == DVI1)?(i2c_SCLHi_DVI()):(i2c_SCLHi_VGA()))
#define i2c_SCLLo()                 0//((LoadEDIDSelectPort == DVI1)?(i2c_SCLLo_DVI()):(i2c_SCLLo_VGA()))
#endif

#if !USEFLASH
#define hwI2C_SCL_Pin               // GPIO04
#define hwI2C_SDA_Pin               // GPIO05
#define Init_hwI2C_SCL_Pin()        (MEM_MSWRITE_BIT(_REG_GPIO5_OEZ, TRUE, BIT3))
#define Init_hwI2C_SDA_Pin()        (MEM_MSWRITE_BIT(_REG_GPIO5_OEZ, TRUE, BIT2))

#define Set_i2c_SDA()               (MEM_MSWRITE_BIT(_REG_GPIO5_OEZ, TRUE, BIT2))
#define Clr_i2c_SDA()               (MEM_MSWRITE_BIT(_REG_GPIO5_OUT, FALSE, BIT2), MEM_MSWRITE_BIT(_REG_GPIO5_OEZ, FALSE, BIT2))
#define i2c_SDAHi()                 (_bit2_(MEM_MSREAD_BYTE(_REG_GPIO5_IN)))
#define i2c_SDALo()                 (!i2c_SDAHi())
#define Set_i2c_SCL()               (MEM_MSWRITE_BIT(_REG_GPIO5_OEZ, TRUE, BIT3))
#define Clr_i2c_SCL()               (MEM_MSWRITE_BIT(_REG_GPIO5_OUT, FALSE, BIT3), MEM_MSWRITE_BIT(_REG_GPIO5_OEZ, FALSE, BIT3))
#define i2c_SCLHi()                 (_bit3_(MEM_MSREAD_BYTE(_REG_GPIO5_IN)))
#define i2c_SCLLo()                 (!i2c_SCLHi())
#endif


//SARKEY_ENABLE_BIT
#define SAR0_EN     BIT0
#define SAR1_EN     BIT1
#define SAR2_EN     BIT2
#define SAR3_EN     BIT3
#define SARKEY_EN   (SAR1_EN|SAR0_EN)

#define SAR4_EN         BIT0
#define SAR5_EN         BIT1
#define SAR6_EN         BIT2
#define SAR7_EN         BIT3
#define SAR8_EN         BIT4
#define SAR9_EN         BIT5

//SAR SELECT
#define KEYPAD_ADC_A    KEYPAD_SAR00
#define KEYPAD_ADC_B    KEYPAD_SAR01
//SAR_KEY_VALUE
#define ADCKEY_A0_VALUE     0x00
#define ADCKEY_A1_VALUE     0x44
#define ADCKEY_B0_VALUE     0x00
#define ADCKEY_B1_VALUE     0x44
#define KEY_TOL             8

#define ADCKEY_A0       KEY_MENU
#define ADCKEY_A1       KEY_MINUS
#define ADCKEY_B0       KEY_EXIT
#define ADCKEY_B1       KEY_PLUS

//GPIO_KEY
#define HW_KEY_PLUS     NonUsepin
#define HW_KEY_MINUS    NonUsepin
#define HW_KEY_TICK     NonUsepin
#define HW_KEY_MENU     NonUsepin
//POWER_KEY
#define HW_POWER_KEY        (_bit3_(MEM_MSREAD_BYTE(_REG_GPIO7_IN)))
#define INIT_HW_POWER_KEY() (MEM_MSWRITE_BIT(_REG_GPIO7_OEZ, 1, BIT3))
/*#define HW_POWER_KEY        (_bit2_(MEM_MSREAD_BYTE(_REG_GPIO0_IN)))
#define INIT_HW_POWER_KEY() (MEM_MSWRITE_BIT(_REG_GPIO0_OEZ, 1, BIT2))*/
#define PM_POWERKEY_WAKEUP      0//EN_GPIO57_DET


//#define LVDS_CHANNEL    (MOD_PORTB|MOD_PORTC)//(MOD_PORTA|MOD_PORTB|MOD_PORTC|MOD_PORTD)
//#define I_GEN_CH        CH2

#if ENABLE_MHL
// SWI2C for Eland
#define SET_SWI2C_SCL()             //MEM_MSWRITE_BIT(_REG_PMGPIO2_OUT, TRUE, BIT7)         //PMGPIO27
#define CLR_SWI2C_SCL()             //MEM_MSWRITE_BIT(_REG_PMGPIO2_OUT,FALSE, BIT7)
#define IS_SWI2C_SCL_HI()           0//(_bit7_(MEM_MSREAD_BYTE(_REG_PMGPIO2_IN)))
#define IS_SWI2C_SCL_LO()           0//(!IS_SWI2C_SCL_HI())
#define Init_SWI2C_SCL()            //(MEM_MSWRITE_BIT(_REG_PMGPIO2_OEZ, TRUE, BIT7))

#define SET_SWI2C_SDA()             //MEM_MSWRITE_BIT(_REG_PMGPIO3_OUT, TRUE, BIT6)         //PMGPIO36
#define CLR_SWI2C_SDA()             //MEM_MSWRITE_BIT(_REG_PMGPIO3_OUT,FALSE, BIT6)
#define IS_SWI2C_SDA_HI()           0//(_bit6_(MEM_MSREAD_BYTE(_REG_PMGPIO3_IN)))
#define IS_SWI2C_SDA_LO()           0//(!IS_SWI2C_SDA_HI())
#define Init_SWI2C_SDA()            //(MEM_MSWRITE_BIT(_REG_PMGPIO2_OEZ, TRUE, BIT6))
//#define SET_SWI2C_BUS_OUT()   MEM_MSWRITE_BYTE_MASK(_REG_PMGPIO2_OEZ, 0x00, BIT6|BIT7)

// MHL cable detect
#define Init_MHL_CABLE_DETECT_Pin() //(MEM_MSWRITE_BIT(_REG_PMGPIO0_OEZ, TRUE, BIT1) )
#define SET_MHL_CABLE_DETECT()      //MEM_MSWRITE_BIT(_REG_PMGPIO1_OEZ, TRUE, BIT4)    // PMGPIO14
#define IS_MHL_CABLE_HI()           0//(_bit4_(MEM_MSREAD_BYTE(_REG_PMGPIO1_IN)))
#define IS_MHL_CABLE_LO()           0//(!IS_MHL_CABLE_HI())

#endif

// MHL PS_CTRL
#define SET_MHL_PS_CTRL_CHARGE0()
#define CLR_MHL_PS_CTRL_CHARGE0()

#define SET_MHL_PS_CTRL_CHARGE1()
#define CLR_MHL_PS_CTRL_CHARGE1()

#define SET_MHL_PS_CTRL_CHARGE2()   (MEM_MSWRITE_BIT(_REG_GPIO5_OEZ, FALSE, BIT5), MEM_MSWRITE_BIT(_REG_GPIO5_OUT, TRUE, BIT5))
#define CLR_MHL_PS_CTRL_CHARGE2()   (MEM_MSWRITE_BIT(_REG_GPIO5_OEZ, FALSE, BIT5), MEM_MSWRITE_BIT(_REG_GPIO5_OUT, FALSE, BIT5))

#define SET_MHL_PS_CTRL_CHARGE3()   (MEM_MSWRITE_BIT(_REG_GPIO5_OEZ, FALSE, BIT6), MEM_MSWRITE_BIT(_REG_GPIO5_OUT, TRUE, BIT6))
#define CLR_MHL_PS_CTRL_CHARGE3()   (MEM_MSWRITE_BIT(_REG_GPIO5_OEZ, FALSE, BIT6), MEM_MSWRITE_BIT(_REG_GPIO5_OUT, FALSE, BIT6))

#define SET_MHL_PS_CTRL_CHARGE4()
#define CLR_MHL_PS_CTRL_CHARGE4()

#define SET_MHL_PS_CTRL_CHARGE5()
#define CLR_MHL_PS_CTRL_CHARGE5()

#define IR_REMOTE_SEL                   NO_REMOTE//MSTAR_REMOTE//TW_REMOTE//SHINCO_REMOTE//RC5_MAGNAVOX//SZ_REMOTE

#endif



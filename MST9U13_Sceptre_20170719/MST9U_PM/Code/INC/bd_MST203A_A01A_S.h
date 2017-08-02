
#ifndef __BD_MSTMCU__
#define __BD_MSTMCU__

#define MCU_SPEED_INDEX         IDX_MCU_CLK_MPLL_DIV // 172M // need sync with Sboot setting for AC_ON consistency
#define SPI_SPEED_INDEX         IDX_SPI_CLK_43MHZ // need sync with Sboot setting for AC_ON consistency
#define SPI_MODE                SPI_MODE_FR // need sync with Sboot setting for AC_ON consistency
#define HPD_INVERSE             0 // If it has BJT circuit on board, the HPD_INVERSE should be set to TRUE

#define SPI_SSC_EN              (SPI_SPEED_INDEX==IDX_SPI_CLK_SSC)// use memory clock source, need to enable memclk SSC, and divide it to the one closest & lower to SPI_SPEED_INDEX
#define SPI_SSC_SOURCE          0 // 0:mempll, 1:lpll (Current code flow for mempll only)
#define SPI_SSC_DIVIDER         4 // Target Freq => (MEMORY_CLOCK_MHZ*(100+PERCENTAGE)/(SPI_SSC_DIVIDER+1))

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

#define DFT_51_UART0_GPIO           UART_GPIOX13_14//connector J803
#define DFT_51_UART1_GPIO           UART_GPIOX23_24

#define PORT_FLOW_0     MUX_ADC
#define PORT_FLOW_1     MUX_COMBO4
#define PORT_FLOW_2     MUX_COMBO2
#define PORT_FLOW_3     MUX_COMBO3
#define PORT_FLOW_4     MUX_COMBO1//MUX_COMBO0
//#define PORT_FLOW_7

#define PORT_TYPE_0     TYPE_ANALOG
#define PORT_TYPE_1     (TYPE_DP|TYPE_HBR2_0|TYPE_DAISY_CHAIN)
#define PORT_TYPE_2     (TYPE_HDMI |TYPE_MHL |TYPE_HDMI_2_0)
#define PORT_TYPE_3     (TYPE_HDMI |TYPE_MHL |TYPE_HDMI_2_0)
#define PORT_TYPE_4     TYPE_DVI_DUAL
//#define PORT_TYPE_7

// Define TMDS/DP port used
#define TMDS_Port_D0    0 //none
#define TMDS_Port_D1    1 //DVI dual - master
#define TMDS_Port_D2    1 //HDMI
#define TMDS_Port_D3    1 //HDMI
#define TMDS_Port_D4    0 //none
#define TMDS_Port_D5    0 //none
#define TMDS_Port_Num   (TMDS_Port_D0+TMDS_Port_D1+TMDS_Port_D2+TMDS_Port_D3+TMDS_Port_D4+TMDS_Port_D5)

#define DP_Port_D0      0 //none
#define DP_Port_D1      0 //none
#define DP_Port_D2      0 //none
#define DP_Port_D3      0 //none
#define DP_Port_D4      1 //DP
#define DP_Port_D5      0 //none
#define DP_Port_Num     (DP_Port_D0+DP_Port_D1+DP_Port_D2+DP_Port_D3+DP_Port_D4+DP_Port_D5)
//=================================================
//for switch input port priority and input source OSD item priority setting
#define DVI_PORT_PRIORITY       4 //Input_Digital5
#define DVI2ND_PORT_PRIORITY    5 //Input_Nums
#define DVI3ND_PORT_PRIORITY    5 //Input_Nums
#define HDMI_PORT_PRIORITY      2 //Input_Digital3
#define HDMI2ND_PORT_PRIORITY   3 //Input_Digital4
#define HDMI3ND_PORT_PRIORITY   5 //Input_Nums
#define DP_PORT_PRIORITY        1 //Input_Digital1
#define DP2ND_PORT_PRIORITY     5 //Input_Digital2
#define DP3ND_PORT_PRIORITY     5 //Input_Nums
#define VD_PORT_PRIORITY        5 //Input_Digital6

//=================================================

#define SERIAL_BAUD_RATE        9600

#define BACKLIGHT_PWM               _NO_USE_ // Driving Non-LED BL Module
#define BACKLIGHT_PWM_FREQ           _NO_USE_
#define BACKLIGHT_PWM_DUTY           0x7FFF //0~0xFFFF  50%
#define BACKLIGHT_PWM_CONFIG        _PWM_DB_EN_
#define BACKLIGHT_PWM_VS_ALG_NUM    _NO_USE_//vsync align number


#define NonUsepin
//================================================================================================================
//================================================================================================================
// 1.Write protection:
#define hwDDC_WP_Pin            //Pull to Ground in Schematic
#define hwEEPROM_WP             //GPIO03
#define hwFlash_WP_Pin          //GPIO10

// ddc eeprom wp be pulled high to 5V, set to input for output high level
#define hw_SetDDC_WP()          //(_MEM_MSWRITE_BIT(_REG_PMGPIO2_OUT, 1, BIT5), DDCWPActiveFlag=1)//(_MEM_MSWRITE_BIT(_REG_PMGPIO1_OUT, 0, BIT6), _MEM_MSWRITE_BIT(_REG_PMGPIO1_OEZ, 0, BIT6), DDCWPActiveFlag=0)
#define hw_ClrDDC_WP()          //(_MEM_MSWRITE_BIT(_REG_PMGPIO2_OUT, 0, BIT5), DDCWPActiveFlag=0)
#define Init_hwDDC_WP_Pin()     //_MEM_MSWRITE_BIT(_REG_PMGPIO2_OEZ, 0, BIT5)//(_MEM_MSWRITE_BIT(_REG_PMGPIO1_OEZ, 1, BIT6)) // output disable

#define Set_EEPROM_WP()         MEM_MSWRITE_BIT(_REG_GPIO2_OUT, 1, BIT5)
#define Clr_EEPROM_WP()         MEM_MSWRITE_BIT(_REG_GPIO2_OUT, 0, BIT5)
#define Init_hwEEPROM_WP()      MEM_MSWRITE_BIT(_REG_GPIO2_OEZ, 0, BIT5)

#define hw_SetFlashWP()         MEM_MSWRITE_BIT(_REG_GPIO1_OUT, 0, BIT0)
#define hw_ClrFlashWP()         MEM_MSWRITE_BIT(_REG_GPIO1_OUT, 1, BIT0)
#define Init_hwFlash_WP_Pin()   MEM_MSWRITE_BIT(_REG_GPIO1_OEZ, 0, BIT0)

//================================================================================================================
//================================================================================================================
// 2.Backlight and panelon:
#define hw_SetBlacklit()        (MEM_MSWRITE_BIT(_REG_GPIO0_OUT, 0, BIT6), Set_BacklightFlag())
#define hw_ClrBlacklit()        (MEM_MSWRITE_BIT(_REG_GPIO0_OUT, 1, BIT6), Clr_BacklightFlag())
#define Init_hwBlacklit_Pin()   MEM_MSWRITE_BIT(_REG_GPIO0_OEZ, 0, BIT6)

#define hw_SetPanel()           MEM_MSWRITE_BIT(_REG_GPIOSAR_OUT, 0, BIT6)
#define hw_ClrPanel()           MEM_MSWRITE_BIT(_REG_GPIOSAR_OUT, 1, BIT6)
#define Init_hwPanel_Pin()      MEM_MSWRITE_BIT(_REG_GPIOSAR_OEZ, 0, BIT2)

//================================================================================================================
//================================================================================================================
// 4.Detect Cable
//-----------------------------
// Cable Detect : VGA Port
//-----------------------------
#define CABLE_DETECT_VGA_USE_SAR    0
#define VGA_CABLE_DET_SAR_EN        (CABLE_DET_VGA_SAR)
#if CABLE_DETECT_VGA_USE_SAR
#define hwDSUBCable_Pin             0
#define Init_hwDSUBCable_Pin()
#define CABLE_DET_VGA_SAR           SAR3_EN // 2.8V
#else
#define hwDSUBCable_Pin             (_bit7_(MEM_MSREAD_BYTE(_REG_GPIO5_IN)))  //GPIO57
#define Init_hwDSUBCable_Pin()      (MEM_MSWRITE_BYTE_MASK(_REG_GPIO5_OEZ, BIT7, BIT7))
#define CABLE_DET_VGA_SAR           0
#endif
//-----------------------------
// Cable Detect : TMDS Port
//-----------------------------
#define TMDS_CABLE_DET_SAR2_EN      (CABLE_DET_DIGITAL2_SAR|CABLE_DET_DIGITAL3_SAR)
#if TMDS_Port_D0
#define hwDigital0Cable_Pin         0
#define Init_hwDigital0Cable_Pin()
#define CABLE_DET_DIGITAL0_SAR      0
#else
#define hwDigital0Cable_Pin         0
#define Init_hwDigital0Cable_Pin()
#define CABLE_DET_DIGITAL0_SAR      0
#endif
#if TMDS_Port_D1 // GPIO_X41
#define hwDigital1Cable_Pin         (_bit1_(MEM_MSREAD_BYTE(_REG_GPIO_X4_IN)))  // GPIO_X41
#define Init_hwDigital1Cable_Pin()  (MEM_MSWRITE_BYTE_MASK(_REG_GPIO_X4_OEZ,BIT1,BIT1))
#define CABLE_DET_DIGITAL1_SAR      0
#else
#define hwDigital1Cable_Pin         0
#define Init_hwDigital1Cable_Pin()
#define CABLE_DET_DIGITAL1_SAR      0
#endif
#if TMDS_Port_D2 // SAR10
#define hwDigital2Cable_Pin         (KEYPAD_SAR10)
#define Init_hwDigital2Cable_Pin()  (MEM_MSWRITE_BIT(REG_003A69,2,BIT2)) // Enable SAR ch
#define CABLE_DET_DIGITAL2_SAR      SAR10_EN
#else
#define hwDigital2Cable_Pin         0
#define Init_hwDigital2Cable_Pin()
#define CABLE_DET_DIGITAL2_SAR      0
#endif
#if TMDS_Port_D3// SAR9
#define hwDigital3Cable_Pin         (KEYPAD_SAR09)
#define Init_hwDigital3Cable_Pin()  (MEM_MSWRITE_BIT(REG_003A69,1,BIT1)) // Enable SAR ch
#define CABLE_DET_DIGITAL3_SAR      SAR9_EN
#else
#define hwDigital3Cable_Pin         0
#define Init_hwDigital3Cable_Pin()
#define CABLE_DET_DIGITAL3_SAR      0
#endif
#if TMDS_Port_D4
#define hwDigital4Cable_Pin         0
#define Init_hwDigital4Cable_Pin()
#define CABLE_DET_DIGITAL4_SAR      0
#else
#define hwDigital4Cable_Pin         0
#define Init_hwDigital4Cable_Pin()
#define CABLE_DET_DIGITAL4_SAR      0
#endif
#if TMDS_Port_D5
#define hwDigital5Cable_Pin         0
#define Init_hwDigital5Cable_Pin()  0
#define CABLE_DET_DIGITAL5_SAR      0
#else
#define hwDigital5Cable_Pin         0
#define Init_hwDigital5Cable_Pin()
#define CABLE_DET_DIGITAL5_SAR      0
#endif
//-----------------------------
// Cable Detect : DP Port
//-----------------------------
#if DP_Port_D0
#define hwSDMDP0Pin_Pin             0//(_bit0_(MEM_MSREAD_BYTE(_REG_GPIO_X0_IN)))      //GPIOX00
#define Init_hw_DP0_SDMPin()        //(MEM_MSWRITE_BIT(_REG_GPIO_X0_OEZ, TRUE, BIT0))
#else
#define hwSDMDP0Pin_Pin             0
#define Init_hw_DP0_SDMPin()
#endif
#if DP_Port_D1
#define hwSDMDP1Pin_Pin             0
#define Init_hw_DP1_SDMPin()        0
#else
#define hwSDMDP1Pin_Pin             0
#define Init_hw_DP1_SDMPin()
#endif
#if DP_Port_D2
#define hwSDMDP2Pin_Pin             0
#define Init_hw_DP2_SDMPin()
#else
#define hwSDMDP2Pin_Pin             0
#define Init_hw_DP2_SDMPin()
#endif
#if DP_Port_D3
#define hwSDMDP3Pin_Pin             0
#define Init_hw_DP3_SDMPin()
#else
#define hwSDMDP3Pin_Pin             0
#define Init_hw_DP3_SDMPin()
#endif
#if DP_Port_D4
#define hwSDMDP4Pin_Pin             0
#define Init_hw_DP4_SDMPin()
#else
#define hwSDMDP4Pin_Pin             0
#define Init_hw_DP4_SDMPin()
#endif
#if DP_Port_D5
#define hwSDMDP5Pin_Pin             0
#define Init_hw_DP5_SDMPin()
#else
#define hwSDMDP5Pin_Pin             0
#define Init_hw_DP5_SDMPin()
#endif

#define hwDVI0Cable_Pin             0//hwSDMDP0Pin_Pin
#define hwDVI1Cable_Pin             0//hwSDMDP1Pin_Pin
#define hwDVI2Cable_Pin             KEYPAD_SAR10
#define hwDVI3Cable_Pin             KEYPAD_SAR09
#define hwDVI4Cable_Pin             0//hwSDMDP4Pin_Pin
#define hwDVI5Cable_Pin             0//hwSDMDP5Pin_Pin

//================================================================================================================
//================================================================================================================
// 5.Led
#define hwGreenLed_Pin                  // GPIO21
#define hwAmberLed_Pin                  // GPIO20

#define hw_IsGreenLedOn                 (_bit1_(MEM_MSREAD_BYTE(_REG_GPIO2_OUT)))

#define hw_SetGreenLed()                {MEM_MSWRITE_BIT(_REG_GPIO2_OEZ,0,BIT1);MEM_MSWRITE_BIT(_REG_GPIO2_OUT,1,BIT1);}
#define hw_ClrGreenLed()                {MEM_MSWRITE_BIT(_REG_GPIO2_OEZ,0,BIT1);MEM_MSWRITE_BIT(_REG_GPIO2_OUT,0,BIT1);}
#define Init_hwGreenLed_Pin()           MEM_MSWRITE_BIT(_REG_GPIO2_OEZ, 0,BIT1)// output enable
#define hw_GreenLedState()              (_bit1_(MEM_MSREAD_BYTE(_REG_GPIO2_IN)))

#define hw_SetAmberLed()                {MEM_MSWRITE_BIT(_REG_GPIO2_OEZ,0,BIT0);MEM_MSWRITE_BIT(_REG_GPIO2_OUT, 0, BIT0);}
#define hw_ClrAmberLed()                {MEM_MSWRITE_BIT(_REG_GPIO2_OEZ,0,BIT0);MEM_MSWRITE_BIT(_REG_GPIO2_OUT, 1, BIT0);}
#define Init_hwAmberLed_Pin()           MEM_MSWRITE_BIT(_REG_GPIO2_OEZ, 0,BIT0)// output enable

//================================================================================================================
//================================================================================================================
// 6.Audio control:
#define hwMute_Pin                      //NO_USE_PIN
#define hw_SetMute()                    //(_MEM_MSWRITE_BIT(_REG_PMGPIO1_OUT, FALSE, BIT4))
#define hw_ClrMute()                    //(_MEM_MSWRITE_BIT(_REG_PMGPIO1_OUT, TRUE, BIT4))
#define Init_hwMute_Pin()               //(_MEM_MSWRITE_BIT(_REG_PMGPIO1_OEZ, FALSE, BIT4))
#if 1
#define hw_SetAudio_SD()            (MEM_MSWRITE_BIT(REG_00045A, FALSE, BIT0))
#define hw_ClrAudio_SD()                (MEM_MSWRITE_BIT(REG_00045A, TRUE, BIT0))
#define Init_Audio_SD_Pin()         (MEM_MSWRITE_BIT(REG_00045C, FALSE, BIT0))  //[4] Lineout output control on GPIO01
#define Init_SPIFGPIO_Pin()         (MEM_MSWRITE_BIT(REG_000406, TRUE, BIT2))   //[0]S/PDIF output control on GPIO01
#define Init_I2S_Switch_Pin()           {MEM_MSWRITE_BYTE_MASK(REG_000407,BIT6|BIT5,BIT6|BIT5|BIT4);MEM_MSWRITE_BYTE_MASK(REG_000416,0xF0,0xFF);} //[1]:reg_i2s_out_au_en [2]:reg_i2s_out_mck_au_en

#else
#define hw_SetAudio_SD()                //NO_USE_PIN
#define hw_ClrAudio_SD()
#define Init_Audio_SD_Pin()         {_MEM_MSWRITE_BIT(REG_000407,0,BIT4);}    //[4] Lineout output control on GPIO01
#define Init_SPIFGPIO_Pin()         {MEM_MSWRITE_BIT(REG_000406,BIT2,BIT2);}//[0]S/PDIF output control on GPIO01
#endif

//#define hw_SetAudio_SD()                //NO_USE_PIN
//#define hw_ClrAudio_SD()
//#define Init_Audio_SD_Pin()         {_MEM_MSWRITE_BIT(REG_000407,0,BIT4);}    //[4] Lineout output control on GPIO01
//#define Init_SPIFGPIO_Pin()         {MEM_MSWRITE_BIT(REG_000406,BIT2,BIT2);}//[0]S/PDIF output control on GPIO01
                                                                    //[2]S/PDIF output control on GPIO_AU5
//Since GPIO01 is the power switch on demo board, output S/PDIF on this pad will cause latch up when AC on.
//The register definition is wrong.

//================================================================================================================
//================================================================================================================
// I2C
#if defined(UseVGACableReadWriteAllPortsEDID) //&&!defined(UseInternalDDCRam)
#define Set_i2c_SDA_VGA()           //(_MEM_MSWRITE_BIT(_REG_PMGPIO1_OEZ, TRUE, BIT2))
#define Clr_i2c_SDA_VGA()           //(_MEM_MSWRITE_BIT(_REG_PMGPIO1_OUT, FALSE, BIT2), _MEM_MSWRITE_BIT(_REG_PMGPIO1_OEZ, FALSE, BIT2))
#define i2c_SDAHi_VGA()             //(_bit2_(_MEM_MSREAD_BYTE(_REG_PMGPIO1_IN)))
#define i2c_SDALo_VGA()             //(!i2c_SDAHi())
#define Set_i2c_SCL_VGA()           //(_MEM_MSWRITE_BIT(_REG_PMGPIO1_OEZ, TRUE, BIT3))
#define Clr_i2c_SCL_VGA()           //(_MEM_MSWRITE_BIT(_REG_PMGPIO1_OUT, FALSE, BIT3), _MEM_MSWRITE_BIT(_REG_PMGPIO1_OEZ, FALSE, BIT3))
#define i2c_SCLHi_VGA()             //(_bit3_(_MEM_MSREAD_BYTE(_REG_PMGPIO1_IN)))
#define i2c_SCLLo_VGA()             //(!i2c_SCLHi())

#define Set_i2c_SDA_DVI()           //(_MEM_MSWRITE_BIT(_REG_PMGPIO2_OEZ, TRUE, BIT1))
#define Clr_i2c_SDA_DVI()           //(_MEM_MSWRITE_BIT(_REG_PMGPIO2_OUT, FALSE, BIT1), _MEM_MSWRITE_BIT(_REG_PMGPIO2_OEZ, FALSE, BIT1))
#define i2c_SDAHi_DVI()             //(_bit1_(_MEM_MSREAD_BYTE(_REG_PMGPIO2_IN)))
#define i2c_SDALo_DVI()             //(!i2c_SDAHi())
#define Set_i2c_SCL_DVI()           //(_MEM_MSWRITE_BIT(_REG_PMGPIO2_OEZ, TRUE, BIT2))
#define Clr_i2c_SCL_DVI()           //(_MEM_MSWRITE_BIT(_REG_PMGPIO2_OUT, FALSE, BIT2), _MEM_MSWRITE_BIT(_REG_PMGPIO2_OEZ, FALSE, BIT2))
#define i2c_SCLHi_DVI()             //(_bit2_(_MEM_MSREAD_BYTE(_REG_PMGPIO2_IN)))
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
#define hwI2C_SCL_Pin               // GPIO00
#define hwI2C_SDA_Pin               // GPIO01
#define Init_hwI2C_SCL_Pin()        (MEM_MSWRITE_BIT(_REG_GPIO0_OEZ, TRUE, BIT0))
#define Init_hwI2C_SDA_Pin()        (MEM_MSWRITE_BIT(_REG_GPIO0_OEZ, TRUE, BIT1))

#define Set_i2c_SDA()               (MEM_MSWRITE_BIT(_REG_GPIO0_OEZ, TRUE, BIT1))
#define Clr_i2c_SDA()               (MEM_MSWRITE_BIT(_REG_GPIO0_OUT, FALSE, BIT1), MEM_MSWRITE_BIT(_REG_GPIO0_OEZ, FALSE, BIT1))
#define i2c_SDAHi()                 (_bit1_(MEM_MSREAD_BYTE(_REG_GPIO0_IN)))
#define i2c_SDALo()                 (!i2c_SDAHi())
#define Set_i2c_SCL()               (MEM_MSWRITE_BIT(_REG_GPIO0_OEZ, TRUE, BIT0))
#define Clr_i2c_SCL()               (MEM_MSWRITE_BIT(_REG_GPIO0_OUT, FALSE, BIT0), MEM_MSWRITE_BIT(_REG_GPIO0_OEZ, FALSE, BIT0))
#define i2c_SCLHi()                 (_bit0_(MEM_MSREAD_BYTE(_REG_GPIO0_IN)))
#define i2c_SCLLo()                 (!i2c_SCLHi())
#endif


#if ENABLE_DP_INPUT
#define Init_hwDP_Hpd_Pin0()    //(MEM_MSWRITE_BIT(REG_000462, TRUE, BIT1),MEM_MSWRITE_BIT(_REG_GPIO_X0_OUT, FALSE, BIT2), MEM_MSWRITE_BIT(_REG_GPIO_X0_OEZ, FALSE, BIT2))
#define hw_Set_DPHpd0()         //(MEM_MSWRITE_BIT(REG_000462, TRUE, BIT1),MEM_MSWRITE_BIT(_REG_GPIO_X0_OUT, TRUE, BIT2), MEM_MSWRITE_BIT(_REG_GPIO_X0_OEZ, FALSE, BIT2))
#define hw_Clr_DPHpd0()         //(MEM_MSWRITE_BIT(_REG_GPIO_X0_OUT, FALSE, BIT2), MEM_MSWRITE_BIT(_REG_GPIO_X0_OEZ, FALSE, BIT2)) //default high

#define Init_hwDP_Hpd_Pin1()    //(MEM_MSWRITE_BIT(REG_000462, TRUE, BIT1),MEM_MSWRITE_BIT(_REG_GPIO_X0_OUT, FALSE, BIT2), MEM_MSWRITE_BIT(_REG_GPIO_X0_OEZ, FALSE, BIT2))
#define hw_Set_DPHpd1()         //(MEM_MSWRITE_BIT(REG_000462, TRUE, BIT1),MEM_MSWRITE_BIT(_REG_GPIO_X0_OUT, TRUE, BIT2), MEM_MSWRITE_BIT(_REG_GPIO_X0_OEZ, FALSE, BIT2))
#define hw_Clr_DPHpd1()         //(MEM_MSWRITE_BIT(_REG_GPIO_X0_OUT, FALSE, BIT2), MEM_MSWRITE_BIT(_REG_GPIO_X0_OEZ, FALSE, BIT2)) //default high //default high

#define Init_hwDP_Hpd_Pin2()    //(MEM_MSWRITE_BIT(REG_000462, TRUE, BIT5),MEM_MSWRITE_BIT(_REG_GPIO_X2_OUT, FALSE, BIT2), MEM_MSWRITE_BIT(_REG_GPIO_X2_OEZ, FALSE, BIT2))
#define hw_Set_DPHpd2()         //(MEM_MSWRITE_BIT(REG_000462, TRUE, BIT5),MEM_MSWRITE_BIT(_REG_GPIO_X2_OUT, TRUE, BIT2), MEM_MSWRITE_BIT(_REG_GPIO_X2_OEZ, FALSE, BIT2))
#define hw_Clr_DPHpd2()         //(MEM_MSWRITE_BIT(_REG_GPIO_X2_OUT, FALSE, BIT2), MEM_MSWRITE_BIT(_REG_GPIO_X2_OEZ, FALSE, BIT2)) //default high

#define Init_hwDP_Hpd_Pin3()    //(MEM_MSWRITE_BIT(REG_000462, TRUE, BIT7),MEM_MSWRITE_BIT(_REG_GPIO_X3_OUT, FALSE, BIT2), MEM_MSWRITE_BIT(_REG_GPIO_X3_OEZ, FALSE, BIT2))
#define hw_Set_DPHpd3()         //(MEM_MSWRITE_BIT(REG_000462, TRUE, BIT7),MEM_MSWRITE_BIT(_REG_GPIO_X3_OUT, TRUE, BIT2), MEM_MSWRITE_BIT(_REG_GPIO_X3_OEZ, FALSE, BIT2))
#define hw_Clr_DPHpd3()         //(MEM_MSWRITE_BIT(_REG_GPIO_X3_OUT, FALSE, BIT2), MEM_MSWRITE_BIT(_REG_GPIO_X3_OEZ, FALSE, BIT2)) //default high

#define Init_hwDP_Hpd_Pin4()    (MEM_MSWRITE_BIT(REG_000462, TRUE, BIT1),MEM_MSWRITE_BIT(_REG_GPIO_X0_OUT, FALSE, BIT2), MEM_MSWRITE_BIT(_REG_GPIO_X0_OEZ, FALSE, BIT2))
#define hw_Set_DPHpd4()         (MEM_MSWRITE_BIT(REG_000462, TRUE, BIT1),MEM_MSWRITE_BIT(_REG_GPIO_X0_OUT, TRUE, BIT2), MEM_MSWRITE_BIT(_REG_GPIO_X0_OEZ, FALSE, BIT2))
#define hw_Clr_DPHpd4()         (MEM_MSWRITE_BIT(_REG_GPIO_X0_OUT, FALSE, BIT2), MEM_MSWRITE_BIT(_REG_GPIO_X0_OEZ, FALSE, BIT2)) //default high

#define Init_hwDP_Hpd_Pin5()    //(MEM_MSWRITE_BIT(REG_000463, TRUE, BIT3),MEM_MSWRITE_BIT(_REG_GPIO_X5_OUT, FALSE, BIT2), MEM_MSWRITE_BIT(_REG_GPIO_X5_OEZ, FALSE, BIT2))
#define hw_Set_DPHpd5()         //(MEM_MSWRITE_BIT(REG_000463, TRUE, BIT3),MEM_MSWRITE_BIT(_REG_GPIO_X5_OUT, TRUE, BIT2), MEM_MSWRITE_BIT(_REG_GPIO_X5_OEZ, FALSE, BIT2))
#define hw_Clr_DPHpd5()         //(MEM_MSWRITE_BIT(_REG_GPIO_X5_OUT, FALSE, BIT2), MEM_MSWRITE_BIT(_REG_GPIO_X5_OEZ, FALSE, BIT2)) //default high
//================================================================================================================
#define hw_Set_DPCable5V0()     //(MEM_MSWRITE_BIT(_REG_GPIO_X0_OUT, TRUE, BIT1), MEM_MSWRITE_BIT(_REG_GPIO_X0_OEZ, FALSE, BIT1))
#define hw_Clr_DPCable5V0()     //(MEM_MSWRITE_BIT(_REG_GPIO_X0_OUT, TRUE, BIT1), MEM_MSWRITE_BIT(_REG_GPIO_X0_OEZ, TRUE, BIT1))
#define Init_hwDP_Cable5V0()    //(MEM_MSWRITE_BIT(_REG_GPIO_X0_OUT, FALSE, BIT1), MEM_MSWRITE_BIT(_REG_GPIO_X0_OEZ, FALSE, BIT1)) //default high

#define hw_Set_DPCable5V1()     //(MEM_MSWRITE_BIT(_REG_GPIO_X0_OUT, TRUE, BIT1), MEM_MSWRITE_BIT(_REG_GPIO_X0_OEZ, FALSE, BIT1))
#define hw_Clr_DPCable5V1()     //(MEM_MSWRITE_BIT(_REG_GPIO_X0_OUT, TRUE, BIT1), MEM_MSWRITE_BIT(_REG_GPIO_X0_OEZ, TRUE, BIT1))
#define Init_hwDP_Cable5V1()    //(MEM_MSWRITE_BIT(_REG_GPIO_X0_OUT, FALSE, BIT1), MEM_MSWRITE_BIT(_REG_GPIO_X0_OEZ, FALSE, BIT1)) //default high

#define hw_Set_DPCable5V2()     //(MEM_MSWRITE_BIT(_REG_GPIO_X2_OUT, TRUE, BIT1), MEM_MSWRITE_BIT(_REG_GPIO_X2_OEZ, FALSE, BIT1))
#define hw_Clr_DPCable5V2()     //(MEM_MSWRITE_BIT(_REG_GPIO_X2_OUT, TRUE, BIT1), MEM_MSWRITE_BIT(_REG_GPIO_X2_OEZ, TRUE, BIT1))
#define Init_hwDP_Cable5V2()    //(MEM_MSWRITE_BIT(_REG_GPIO_X2_OUT, FALSE, BIT1), MEM_MSWRITE_BIT(_REG_GPIO_X2_OEZ, FALSE, BIT1)) //default high

#define hw_Set_DPCable5V3()     //(MEM_MSWRITE_BIT(_REG_GPIO_X3_OUT, TRUE, BIT1), MEM_MSWRITE_BIT(_REG_GPIO_X3_OEZ, FALSE, BIT1))
#define hw_Clr_DPCable5V3()     //(MEM_MSWRITE_BIT(_REG_GPIO_X3_OUT, TRUE, BIT1), MEM_MSWRITE_BIT(_REG_GPIO_X3_OEZ, TRUE, BIT1))
#define Init_hwDP_Cable5V3()    //(MEM_MSWRITE_BIT(_REG_GPIO_X3_OUT, FALSE, BIT1), MEM_MSWRITE_BIT(_REG_GPIO_X3_OEZ, FALSE, BIT1)) //default high

#define hw_Set_DPCable5V4()     (MEM_MSWRITE_BIT(_REG_GPIO_X0_OUT, TRUE, BIT1), MEM_MSWRITE_BIT(_REG_GPIO_X0_OEZ, FALSE, BIT1))
#define hw_Clr_DPCable5V4()     (MEM_MSWRITE_BIT(_REG_GPIO_X0_OUT, TRUE, BIT1), MEM_MSWRITE_BIT(_REG_GPIO_X0_OEZ, TRUE, BIT1))
#define Init_hwDP_Cable5V4()    (MEM_MSWRITE_BIT(_REG_GPIO_X0_OUT, FALSE, BIT1), MEM_MSWRITE_BIT(_REG_GPIO_X0_OEZ, FALSE, BIT1)) //default high

#define hw_Set_DPCable5V5()     //(MEM_MSWRITE_BIT(_REG_GPIO_X5_OUT, TRUE, BIT1), MEM_MSWRITE_BIT(_REG_GPIO_X5_OEZ, FALSE, BIT1))
#define hw_Clr_DPCable5V5()     //(MEM_MSWRITE_BIT(_REG_GPIO_X5_OUT, TRUE, BIT1), MEM_MSWRITE_BIT(_REG_GPIO_X5_OEZ, TRUE, BIT1))
#define Init_hwDP_Cable5V5()    //(MEM_MSWRITE_BIT(_REG_GPIO_X5_OUT, FALSE, BIT1), MEM_MSWRITE_BIT(_REG_GPIO_X5_OEZ, FALSE, BIT1)) //default high

#endif

//====================================KEYPAD Setting=====================================
//SARKEY_ENABLE
#define SARKEY_EN       (SAR0_EN|SAR1_EN)

//SAR SELECT
#define KEYPAD_ADC_A    KEYPAD_SAR00
#define KEYPAD_ADC_B    KEYPAD_SAR01
//SAR_KEY_VALUE


//GPIO_KEY
#define HW_KEY_PLUS     NonUsepin
#define HW_KEY_MINUS    NonUsepin
#define HW_KEY_TICK     NonUsepin
#define HW_KEY_MENU     NonUsepin
//POWER_KEY
#define HW_POWER_KEY        (_bit6_(MEM_MSREAD_BYTE(_REG_GPIO6_IN)))
#define INIT_HW_POWER_KEY() (MEM_MSWRITE_BIT(_REG_GPIO6_OEZ, 1, BIT6))
#define PM_POWERKEY_WAKEUP      0//GPIO02_INT
//=======================================================================================
//SAR_ENABLE_BIT
#define SAR0_EN         BIT0
#define SAR1_EN         BIT1
#define SAR2_EN         BIT2
#define SAR3_EN         BIT3
#define SAR4_EN         BIT4
#define SAR5_EN         BIT5
#define SAR6_EN         BIT6
#define SAR7_EN         BIT7
#define SAR8_EN         BIT8
#define SAR9_EN         BIT9
#define SAR10_EN        BIT10
#define SAR11_EN        BIT11

#define SAR_ENABLE      (SARKEY_EN|VGA_CABLE_DET_SAR_EN|TMDS_CABLE_DET_SAR2_EN)
//======================================================================================

#define AOVDV_VALUE     6// 4

//#define LVDS_CHANNEL    (MOD_PORTB|MOD_PORTC)//(MOD_PORTA|MOD_PORTB|MOD_PORTC|MOD_PORTD)
//#define I_GEN_CH        CH2

// MHL PS_CTRL
#define SET_MHL_PS_CTRL_CHARGE0()
#define CLR_MHL_PS_CTRL_CHARGE0()

#define SET_MHL_PS_CTRL_CHARGE1()
#define CLR_MHL_PS_CTRL_CHARGE1()

#define SET_MHL_PS_CTRL_CHARGE2()   (MEM_MSWRITE_BIT(_REG_GPIO2_OEZ, FALSE, BIT2), MEM_MSWRITE_BIT(_REG_GPIO2_OUT, TRUE, BIT2))
#define CLR_MHL_PS_CTRL_CHARGE2()   (MEM_MSWRITE_BIT(_REG_GPIO2_OEZ, FALSE, BIT2), MEM_MSWRITE_BIT(_REG_GPIO2_OUT, FALSE, BIT2))

#define SET_MHL_PS_CTRL_CHARGE3()   (MEM_MSWRITE_BIT(_REG_GPIO6_OEZ, FALSE, BIT0), MEM_MSWRITE_BIT(_REG_GPIO6_OUT, TRUE, BIT0))
#define CLR_MHL_PS_CTRL_CHARGE3()   (MEM_MSWRITE_BIT(_REG_GPIO6_OEZ, FALSE, BIT0), MEM_MSWRITE_BIT(_REG_GPIO6_OUT, FALSE, BIT0))

#define SET_MHL_PS_CTRL_CHARGE4()
#define CLR_MHL_PS_CTRL_CHARGE4()

#define SET_MHL_PS_CTRL_CHARGE5()
#define CLR_MHL_PS_CTRL_CHARGE5()

#define Init_CORE_PWR_VID0()        //(MEM_MSWRITE_BIT(_REG_GPIO6_OUT, FALSE, BIT2), MEM_MSWRITE_BIT(_REG_GPIO6_OEZ, FALSE, BIT2)) //default low

#define IR_REMOTE_SEL                   NO_REMOTE//MSTAR_REMOTE//TW_REMOTE//SHINCO_REMOTE//RC5_MAGNAVOX//SZ_REMOTE

#endif // _MSBOARD_H_



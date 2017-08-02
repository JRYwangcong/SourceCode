#define _MCU_C_
#include "Board.h"
#include "datatype.h"
#include "Global.h"
#include "misc.h"
#include "MDebug.h"
#include "Common.h"
#include "Ms_rwreg.h"
//#include "Reg52.h"
#include "mStar.h"
#include "ms_reg.h"
#include "Mcu.h"
#include "GPIO_DEF.h"
#include "Isr.h"
#include "sysinfo2_MST9U.h"
#include "msEread.h"
#include "Ms_PM.h"
#if ENABLE_5707
#include "amp_i2s_tas5707.h"
#endif
//////////////////////////////////
//#include "drvMcu.h"
//#include "halRwreg.h"
/////////////////////////////////

void mStar_InitMCU( void )
{
#if( ENABLE_WATCH_DOG )
    msAPI_InitWDT( _ENABLE );
#else
    msAPI_InitWDT( _DISABLE );
#endif  // end of #if( ENABLE_WATCH_DOG )

#ifndef MST9U_FPGA
    Init_IOPorts();
#endif

    Init_Interrupt();
    msDrvMcuMailBoxRead();
    msDrvMcuXD2MIUAddrSet(MBX_XD2MIU_ADR); // set before 51 active

#if R2_51_DUAL_MODE
    if(!Is51Active() || !IsMailBoxValid())
    {
        msDrvMcuWakeUp51();
    }
#else
    msDrvMcuR2Reset51();
#endif
    msDrvMcuCheckSRBoot();

#if (CHIP_ID==MST9U2)
    msDrvMcuR2Miu1();
#endif
}

static void Init_CORE_PWR_VID(void)
{
    WORD u16Value;

    u16Value = ((WORD)msEread_GetDataFromEfuse(EFUSE_0, 0x141)) << 8;
    u16Value |= msEread_GetDataFromEfuse(EFUSE_0, 0x140);

    if( u16Value & _BIT15 )
    {
        u16Value &= ~(_BIT15);

        if( u16Value > VID_THRESHOLD )
        {
            Init_CORE_PWR_VID_LOW();
        }
        else
        {
            Init_CORE_PWR_VID_HIGH();
        }
    }
    else
    {
        Init_CORE_PWR_VID_HIGH();
    }
}

// Initialize I/O setting
void Init_IOPorts( void )
{
#if (CHIP_ID == MST9U3)
    msWriteByteMask(REG_163022, 0, _BIT0);
    msWriteByteMask(REG_163042, 0, _BIT0);
    msWriteByteMask(REG_163062, 0, _BIT0);
#endif

    hw_SetDDC_WP();
    hw_SetFlashWP();
    #if !ENABLE_LED_CONTROLLER
    hw_ClrBlacklit();
    #endif
    hw_ClrPanel();
    //hw_ClrGreenLed();
    //hw_ClrAmberLed();

    Init_hwDDC_WP_Pin();
    Init_hwFlash_WP_Pin();
    Init_hwBlacklit_Pin();
    Init_hwPanel_Pin();
    Init_hwGreenLed_Pin();
    Init_hwAmberLed_Pin();

    INIT_HW_POWER_KEY();
    Init_hwDSUBCable_Pin();
    Init_hwDigital0Cable_Pin();
    Init_hwDigital1Cable_Pin();
    Init_hwDigital2Cable_Pin();
    Init_hwDigital3Cable_Pin();
    Init_hwDigital4Cable_Pin();
    Init_hwDigital5Cable_Pin();

#if ENABLE_DP_INPUT
#if ((MS_BOARD_TYPE_SEL == BD_MST203A_A01A_S)||(MS_BOARD_TYPE_SEL == BD_MST203A_A01B_S)||(MS_BOARD_TYPE_SEL == BD_MST203B_A01A_S)||(MS_BOARD_TYPE_SEL == BD_MST203C_A01A_S))
    Init_hwDP_RXPWR_Pin4();
    #if !ENABLE_DP_ShortHPD_PM	 	
    Init_hwDP_Hpd_Pin4();
    #endif	
#endif
    Init_hwDP_Hpd_Pin0();
    Init_hwDP_Hpd_Pin1();
    Init_hwDP_Hpd_Pin2();
    Init_hwDP_Hpd_Pin3();
    Init_hwDP_Hpd_Pin4();
#endif
    Init_hw_DP0_SDMPin();
    Init_hw_DP1_SDMPin();
    Init_hw_DP2_SDMPin();
    Init_hw_DP3_SDMPin();
    Init_hw_DP4_SDMPin();
    Init_hw_DP5_SDMPin();
    Init_CORE_PWR_VID();

#if ENABLE_DAISY_CHAIN && (MS_BOARD_TYPE_SEL == BD_MST192A_A01A_S)
    Init_PD_CTL_E_Pin();
    Set_PD_CTL_E_High();
#endif

#if !USEFLASH
    Set_EEPROM_WP();
    Init_hwI2C_SCL_Pin();
    Init_hwI2C_SDA_Pin();
    Init_hwEEPROM_WP();
#endif

#if ENABLE_HDCP
    //Init_hwHDCP_Hpd_Pin();
    #if (ENABLE_DAISY_CHAIN == 0x1)&&((MS_BOARD_TYPE_SEL == BD_MST192A_A01A_S)\
    ||(MS_BOARD_TYPE_SEL == BD_MST192A_A01C_S)||(MS_BOARD_TYPE_SEL == BD_MST203A_A01A_S)||(MS_BOARD_TYPE_SEL == BD_MST203B_A01A_S)||(MS_BOARD_TYPE_SEL == BD_MST203C_A01A_S))

    //For Mstar demo board with Tx out, we should init DP Tx port's IO high/low
    Init_hwDP_TXPWR_Pin();
    Init_hwDP_Hpd_Pin1();
    #else
    //Init_hwHDCP_Hpd_Pin1();
    #endif
        msDrvMcuMailBoxRead();
         ePMWakeupStatus = msAPI_PMGetWakeupStatus();    
        if (ePMWakeupStatus == ePMSTS_NON)
            msAPI_combo_IP_ControlMHLHPD(COMBO_IP_POWER_AC_ON);
        else
            msAPI_combo_IP_ControlMHLHPD(COMBO_IP_POWER_DC_ON|COMBO_IP_POWER_PS_ON);
    
#endif // #if ENABLE_HDCP

#if AudioFunc
#if ENABLE_5707
//AMP_Init();
//AMP_SetSubWooferVol(50);
#else
    hw_SetMute(); 
    hw_Clr_SPIFGPIO_EN_Pin();
    Init_hwMute_Pin();
    //hw_SetAudio_SD(); //20131227, mask it since there is a pop noise if we control this GPIO with demo board.HW issue(dont control it)
    hw_ClrAudio_SD();
    Init_Audio_SD_Pin();
	#endif

    Init_HP_DET_Pin();
	
#endif

#if ENABLE_USB_INPUT
    Init_hwUSB_Pin();
    hw_USB_LOW();
#endif

#if ENABLE_TOUCH_PANEL || ENABLE_TOUCH_PANEL_DATA_FROM_USB||ENABLE_TOUCH_PANEL_CTRL_OSD
#if ENABLE_TOUCH_PANEL_DATA_FROM_USB
    Init_USB_INT();
#endif
    // keep all output pin at low state before turning on touch panel power
    Clr_TOCUCH_PANEL_POWER();
    Init_TOCUCH_PANEL_POWER_Pin();
    Init_SPI_MISO_Pin();
    Clr_SPI_SCK();
    Init_SPI_SCK_Pin();
    Clr_SPI_MOSI();
    Init_SPI_MOSI_Pin();
    Clr_SPI_SEL();
    Init_SPI_SEL_Pin();
#endif

    CLR_MHL_PS_CTRL_CHARGE0();
    CLR_MHL_PS_CTRL_CHARGE1();
    CLR_MHL_PS_CTRL_CHARGE2();
    CLR_MHL_PS_CTRL_CHARGE3();
    CLR_MHL_PS_CTRL_CHARGE4();
    CLR_MHL_PS_CTRL_CHARGE5();

#if (CHIP_ID == MST9U3)
    msWriteByteMask(REG_163022, _BIT0, _BIT0);
    msWriteByteMask(REG_163042, _BIT0, _BIT0);
    msWriteByteMask(REG_163062, _BIT0, _BIT0);
#endif

   Init_hw_Panel_I2C_SDA_Pin();
   Init_hw_Panel_I2C_SCL_Pin();

   INIT_DDC_I2C_SDA_PIN();
   INIT_DDC_I2C_SCL_PIN();

   Init_hwPanel_10Bit_Pin();

#if (PanelType==PanelLSF490FJ02)
   hw_ClrPanel_10Bit();
#else
   hw_SetPanel_10Bit();
#endif

   Init_PWM_ADJ();

#ifdef PWM_ADJ_Brightness
   Set_PWM_ADJ();
#else
   Clr_PWM_ADJ();
#endif

#if LED_HOT_Menu_Func
   Init_LED_Power();
   Clr_LED_Power();
#endif

#if SwitchPortByDP_Detect
   Init_DP_DET_Pin();
#endif

}

void msDrvVsyncINTEnable( void )
{
    msWriteByteMask(BANK_SC_M_L0+SC00_28, 0x00, BIT5); // int mask
}

void msDrvIPVsyncINTEnable( Bool bEnable )
{
    msWriteBit(BANK_SC_M_L0+SC00_29, !bEnable, BIT4);
}

void msDrvATPINTEnable( Bool bEnable )
{
#if 0//CHIP_ID == MST9U4
    msDrvIP1WPIntEnable(bEnable, SCL0_MAIN, SC_INT_ATP);
#else
    msWriteBit(BANK_SC_M_L0+SC00_2B, !bEnable, BIT4);
//if (bEnable)
    //printf(" =============>>bEnable==%x ,SC00_2B==%x \r\n",bEnable,msReadByte(BANK_SC_M_L0+SC00_2B));
#endif
}


/* initialize 8051 CPU timer & interrupt routine */
/* TCON.7(   TF1): Timer 1 overflow flag */
/* TCON.6(   TR1): Timer 1 stop/start bit */
/* TCON.5(   TF0): Timer 0 overflow flag */
/* TCON.4(   TR0): Timer 0 stop/start bit */
/* TCON.3(   IE1): Timer 1 INT1 interrupt flag */
/* TCON.2(   IT1): Timer 1 INT1 interrupt style setup*/
/* TCON.1(   IE0): Timer 0 /INT0 interrupt flag */
/* TCON.0(   IT0): Timer 0 /INT0 interrupt style setup */
void Init_Interrupt( void )
{
    MsOS_AttachInterrupt(E_INT_IRQ_PM_IRQ_OUT, (InterruptCb)ISR_R2IRQ_PM_IRQ);
    MsOS_AttachInterrupt(E_INT_IRQ_PM_FIQ_OUT, (InterruptCb)ISR_R2IRQ_PM_FIQ);
    //MsOS_AttachInterrupt(E_INT_IRQ_COMB,           (InterruptCb)ISR_R2IRQ_COMB_IRQ);
    MsOS_AttachInterrupt(E_INT_IRQ_SC_L0, (InterruptCb)ISR_R2IRQ_SC_L0);
    MsOS_AttachInterrupt(E_INT_IRQ_SC_L1, (InterruptCb)ISR_R2IRQ_SC_L1);
    MsOS_AttachInterrupt(E_INT_IRQ_SC_R0, (InterruptCb)ISR_R2IRQ_SC_R0);
    MsOS_AttachInterrupt(E_INT_IRQ_SC_R1, (InterruptCb)ISR_R2IRQ_SC_R1);
    MsOS_EnableInterrupt(E_INT_IRQ_SC_L0);
    MsOS_EnableInterrupt(E_INT_IRQ_SC_L1);
    MsOS_EnableInterrupt(E_INT_IRQ_SC_R0);
    MsOS_EnableInterrupt(E_INT_IRQ_SC_R1);

    MsOS_EnableInterrupt(E_INT_IRQ_PM_IRQ_OUT);
    MsOS_EnableInterrupt(E_INT_IRQ_PM_FIQ_OUT);
}

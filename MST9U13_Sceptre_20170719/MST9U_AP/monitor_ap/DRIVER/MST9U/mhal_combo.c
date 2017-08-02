///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   Mhal_combo.c
/// @author MStar Semiconductor Inc.
/// @brief  MHL driver Function
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MHAL_COMBO_C_
#define _MHAL_COMBO_C_

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "Global.h"
#include "mapi_combo.h"
#include "mdrv_combo.h"
#include "mhal_combo.h"

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define MHALCOMBO_DEBUG    1
#if ENABLE_MSTV_UART_DEBUG && MHALCOMBO_DEBUG
#define MHALCOMBO_PRINT(format, ...)     printf(format, ##__VA_ARGS__)
#else
#define MHALCOMBO_PRINT(format, ...)
#endif

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

//**************************************************************************
//  [Function Name]:
//                  _mhal_combo_bank_offset()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
WORD _mhal_combo_bank_offset(BYTE ucPortSelect)
{
    WORD wOffset = 0;

    switch(ucPortSelect)
    {
        default:
        case COMBO_INPUT_SELECT_PORT0:
		    wOffset = 0x0000;
			break;
        case COMBO_INPUT_SELECT_PORT1:
		    wOffset = 0x0600;
			break;
        case COMBO_INPUT_SELECT_PORT2:
		    wOffset = 0x0E00;
			break;
        case COMBO_INPUT_SELECT_PORT3:
		    wOffset = 0x1600;
			break;
        case COMBO_INPUT_SELECT_PORT4:
		    wOffset = 0x1E00;
			break;
        case COMBO_INPUT_SELECT_PORT5:
		    wOffset = 0x2600;
			break;
    }

	return wOffset;

}

//**************************************************************************
//  [Function Name]:
//                  _mhal_combo_phy_offset()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
WORD _mhal_combo_phy_offset(BYTE ucPortSelect)
{
    WORD wOffset = 0;

    switch(ucPortSelect)
    {
        default:
        case COMBO_INPUT_SELECT_PORT0:
		    wOffset = 0x0000;
			break;
        case COMBO_INPUT_SELECT_PORT1:
		    wOffset = 0x0100;
			break;
        case COMBO_INPUT_SELECT_PORT2:
		    wOffset = 0x0200;
			break;
        case COMBO_INPUT_SELECT_PORT3:
		    wOffset = 0x0300;
			break;
        case COMBO_INPUT_SELECT_PORT4:
		    wOffset = 0x0400;
			break;
        case COMBO_INPUT_SELECT_PORT5:
		    wOffset = 0x0500;
			break;
    }

	return wOffset;

}

//**************************************************************************
//  [Function Name]:
//                  _mhal_combo_InternalEDIDEnable()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_combo_InternalEDIDEnable(BYTE ucPortSelect, Bool bDDCEnable)
{

	printf(" _mhal_combo_InternalEDIDEnable =====>>> ucPortSelect==%x \r\n",ucPortSelect);

    switch(ucPortSelect)
    {
        case COMBO_INPUT_SELECT_VGA:
            msWriteByteMask(REG_003E49, bDDCEnable? BIT7: 0, BIT7); // REG_DDC_BANK_24[15]: A0 DDC enable
            break;
            
        case COMBO_INPUT_SELECT_PORT0:
            msWriteByteMask(REG_003E45, bDDCEnable? BIT7: 0, BIT7); // REG_DDC_BANK_22[15]: D0 DDC enable
            break;

        case COMBO_INPUT_SELECT_PORT1:
            msWriteByteMask(REG_003E4D, bDDCEnable? BIT7: 0, BIT7); // REG_DDC_BANK_26[15]: D1 DDC enable
            break;

        case COMBO_INPUT_SELECT_PORT2:
            msWriteByteMask(REG_003E58, bDDCEnable? BIT7: 0, BIT7); // REG_DDC_BANK_2C[7]: D2 DDC enable
            break;

        case COMBO_INPUT_SELECT_PORT3:
            msWriteByteMask(REG_003F45, bDDCEnable? BIT7: 0, BIT7); // REG_DDC1_BANK_22[15]: D3 DDC enable
            break;

        case COMBO_INPUT_SELECT_PORT4:
            msWriteByteMask(REG_003F4D, bDDCEnable? BIT7: 0, BIT7); // REG_DDC1_BANK_26[15]: D4 DDC enable
            break;

        case COMBO_INPUT_SELECT_PORT5:
            msWriteByteMask(REG_003F58, bDDCEnable? BIT7: 0, BIT7); // REG_DDC1_BANK_2C[7]: D5 DDC enable
            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  mhal_combo_LoadEDID()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_combo_LoadEDID(BYTE ucPortSelect, Bool bDPInput, BYTE ucEDIDSize, BYTE ucEDIDAddr, BYTE *pComboEDID)
{
    BYTE i = 0;
    BYTE ucValue = 0, ucSizeCount;

	if(ucPortSelect == COMBO_INPUT_SELECT_VGA)
	{
        if(ucEDIDSize != 0)
        {
	        msWriteByteMask( REG_003EEB, 0x00, 0xFF);    // Select sram base address for cpu read/write

    		for( i = 0; i < 128; i++ )
		    {
			    ucValue = pComboEDID[i] ;

			    msWriteByte( REG_003E4B, i );    // DDC address port for CPU read/write

			    msWriteByte( REG_003E4C, ucValue);	// DDC Data Port for cpu write
			    msWriteByteMask( REG_003E43, BIT5, BIT5);    //ADC sram write data pulse gen when cpu write
			    while(msRead2Byte(REG_003E7B) & 0x20);
		    }
        }
	}
    else
    {
        if(!bDPInput)
	    {
	    	printf(" _mhal_combo_InternalEDIDEnable =====>>> ucPortSelect==%x \r\n",ucPortSelect);

	        _mhal_combo_InternalEDIDEnable(ucPortSelect, TRUE);
	    }    

        for( ucSizeCount = 0; ucSizeCount < ucEDIDSize ; ucSizeCount++ )
        {
			msWriteByteMask( REG_003EEB, ((ucEDIDAddr + ucSizeCount) * 0x10), 0xFF);	  // Select sram base address for cpu read/write

		    for( i = 0; i < 128 ; i++ )
		    {
			    ucValue = pComboEDID[i + (ucSizeCount*128)] ;

		        msWriteByte( REG_003E4B, i );    // DDC address port for CPU read/write

		        msWriteByte( REG_003E4C, ucValue);	// DDC Data Port for cpu write
	 	        msWriteByteMask( REG_003E43, BIT5, BIT5);    //ADC sram write data pulse gen when cpu write
                while(msRead2Byte(REG_003E7B) & 0x20);
		    }
        }
    }

	if(ucPortSelect == COMBO_INPUT_SELECT_VGA)
	{
		msWrite2Byte( REG_003EEC, 0x0000);	// EDID Set Base address for A0/P2/P1/P0
		msWrite2Byte( REG_003FEC, 0x0000);	// EDID Set Base address for XX/P5/P4/P3
	}
    else
    {
        if(ucEDIDSize != 0)
        {
            if(ucPortSelect < 3)
            {
		        msWrite2Byte( REG_003EEC, (msRead2Byte(REG_003EEC) | (ucEDIDAddr << ucPortSelect *4)));	// EDID Set Base address for A0/P2/P1/P0
            }
            else
            {
		        msWrite2Byte( REG_003FEC, (msRead2Byte(REG_003FEC) | (ucEDIDAddr << (ucPortSelect -3)*4)));	// EDID Set Base address for XX/P5/P4/P3
            }
        }
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_combo_LoadHDCPKey()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_combo_LoadHDCPKey(BYTE ucPortSelect, Bool bDPInput, BYTE ucBcaps, BYTE *pHDCPKey, BYTE *pBKSV)
{
    static Bool bDoneFlag = 0;

    if(pHDCPKey != NULL)
    {
		BYTE ucValue;
		WORD wCount0;
		WORD wOffset;

        if(bDPInput)
        {

		msWriteByteMask( REG_163080, BIT2|BIT3, BIT2|BIT3);
		msWriteByteMask( REG_163083, 0,BIT1|BIT0);

		for( wCount0 = 0; wCount0 < 5; wCount0++ )
		{

			msWriteByte( REG_163082, wCount0);
			msWriteByte( REG_163084,  pBKSV[wCount0]);
			msWriteByte((REG_0011A0 + COMBO_DP_PHY_OFFSET(ucPortSelect) + wCount0),  pBKSV[wCount0]);
			Delay1ms(5);
			msWriteByteMask( REG_163080, BIT4, BIT4);
			Delay1ms(5);
			msWriteByteMask( REG_163080, BIT5, BIT5);
			Delay1ms(5);
		}
        }
        else
        {
            wOffset = _mhal_combo_phy_offset(ucPortSelect);

            // clock enable
    		msWrite2Byte(REG_112016 + wOffset, 0xFFFF); //Clock enable
            msWrite2Byte(REG_112018 + wOffset, 0x3FFF); //Clock enable

            //Bksv need to write in 74REG for HDMI/DVI
            wOffset = _mhal_combo_bank_offset(ucPortSelect);

    		msWriteByteMask(REG_16040D + wOffset, 0x04, 0x04); //HDCP enable  for DDC
    		msWriteByteMask(REG_16040F + wOffset, 0x80, 0xC0); //enable CPU write

    		msWriteByteMask(REG_16040C + wOffset, 0x00, 0xFF); //bksv address
    		msWriteByteMask(REG_16040D + wOffset, 0x00, 0x03); //bksv address
            msWriteByteMask(REG_16043A + wOffset, 0x10, 0x10); //load address pulse

    		for( wCount0 = 0; wCount0 < 5; wCount0++ )
            {
                ucValue = pBKSV[wCount0];
                msWriteByteMask(REG_16040E + wOffset, ucValue, 0xFF); //bksv
                msWriteByteMask(REG_16043A + wOffset, 0x08, 0x08); //write data pulse
                while(msReadByte(REG_16042E + wOffset) & 0x10);
    	    }

            //Bcaps
            msWriteByteMask(REG_16040C + wOffset, 0x40, 0xFF); //bcaps address
    		msWriteByteMask(REG_16040D + wOffset, 0x00, 0x03); //bcaps address
            msWriteByteMask(REG_16043A + wOffset, 0x10, 0x10); //load address pulse
            msWriteByteMask(REG_16040E + wOffset, ucBcaps, 0xFF); //bcaps
            msWriteByteMask(REG_16043A + wOffset, 0x08, 0x08); //write data pulse
            while(msReadByte(REG_16042E + wOffset) & 0x10);

            msWriteByteMask(REG_16040F + wOffset, 0x00, 0xC0); //disable CPU write
            msWrite2Byte(REG_160400 + wOffset, 0x0721); //enable HDCP
        }

        if(bDoneFlag == 0)
        {
#if 0 //RIU
            msWriteByteMask(REG_163080, 0x0C, 0x0C); //enable CPU write SRAM
            msWriteByteMask(REG_163082, 0x00, 0xFF); //key address
            msWriteByteMask(REG_163083, 0x00, 0x03); //key address
            msWriteByteMask(REG_163080, 0x10, 0x10); //load address

			for( wCount0 = 0; wCount0 < 5; wCount0++ )
            {
                ucValue = pBKSV[wCount0];
                msWriteByteMask(REG_163084, ucValue, 0xFF); // bksv
                msWriteByteMask(REG_163080, 0x10, 0x10); // write data pulse
                while(msReadByte(REG_16042E + wOffset) & 0x10);
            }

			for( wCount0 = 0; wCount0 < 284; wCount0++ )
			{
                ucValue = pHDCPKey[wCount0];
			    msWriteByteMask(REG_163084, ucValue, 0xFF); // bksv
                msWriteByteMask(REG_163080, 0x10, 0x10); // write data pulse
                while(msReadByte(REG_16042E + wOffset) & 0x10);
            }

			msWriteByteMask(REG_163080, 0x00, 0x0C); //disable CPU write
#else //XIU
            msWriteByteMask(REG_163080, 0x0D, 0x0D); //enable CPU write SRAM by XIU
            msWrite2Byte(REG_162E00, 0x0000); //address

			for( wCount0 = 0; wCount0 < 5; wCount0++ )
			{
                ucValue = pBKSV[wCount0];
                msWriteByteMask(REG_162E02, ucValue, 0xFF);
			}
			for( wCount0 = 0; wCount0 < 284; wCount0++ )
			{
				ucValue = pHDCPKey[wCount0];
				msWriteByteMask(REG_162E02, ucValue, 0xFF);
			}
			msWriteByteMask(REG_163080, 0x00, 0x0D); //disable CPU write SRAM by XIU

#endif
            bDoneFlag = 1;
		}
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_combo_ClockEnable()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_combo_ClockEnable(BYTE ucPortSelect, Bool bMainSwitch)
{
    if(bMainSwitch)
    {
        msWrite2Byte(REG_163006, 0xFFFF); //Clock enable
    	msWrite2Byte(REG_163008, 0x3FFF);

        msWrite2Byte(REG_16065C, 0xFFFF);
        msWrite2Byte(REG_160C5C, 0xFFFF);
        msWrite2Byte(REG_16145C, 0xFFFF);
        msWrite2Byte(REG_161C5C, 0xFFFF);
        msWrite2Byte(REG_16245C, 0xFFFF);
        msWrite2Byte(REG_162C5C, 0xFFFF);

        msWrite2Byte(0x1630EE, 0x1FFF); //clk to scaler
    }
    
    switch(ucPortSelect)
    {
        case COMBO_INPUT_SELECT_PORT0:
            msWrite2Byte(REG_163012, 0xFFFF); //Clock enable
            msWrite2Byte(REG_163014, 0x3FFF);
			break;

        case COMBO_INPUT_SELECT_PORT1:
		    msWrite2Byte(REG_163022, 0xFFFF); //Clock enable
            msWrite2Byte(REG_163024, 0x3FFF);
			break;

        case COMBO_INPUT_SELECT_PORT2:
		    msWrite2Byte(REG_163032, 0xFFFF); //Clock enable
            msWrite2Byte(REG_163034, 0x3FFF);
			break;

        case COMBO_INPUT_SELECT_PORT3:
		    msWrite2Byte(REG_163042, 0xFFFF); //Clock enable
            msWrite2Byte(REG_163044, 0x3FFF);
			break;

        case COMBO_INPUT_SELECT_PORT4:
		    msWrite2Byte(REG_163050, 0xFFFF); //Clock enable
            msWrite2Byte(REG_163052, 0x3FFF);
			break;

        case COMBO_INPUT_SELECT_PORT5:
		    msWrite2Byte(REG_16305E, 0xFFFF); //Clock enable
            msWrite2Byte(REG_163060, 0x3FFF);
			break;

        default:

            break;
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_combo_AudioInitial()
//  [Description]
//                  initial audio common settings
//  [Arguments]:
//                  none
//  [Return]:
//                  none
//**************************************************************************
void mhal_combo_AudioInitial(void)
{
    //*** BK: PAD_TOP ***//
#if 1//(ENABLE_HDMI_AUD_BYPASS_I2S_PAD == 1)  //20131227,Set it based on board define
    //msWriteByteMask(REG_000407, 0x03, 0x03); //reg_i2s_out_en [0]: 0:disable, 1:enable i2s output pad control on pad GPIO_AU1, AU2, AU3
    //msWriteByteMask(REG_000416, 0x0F, 0x0F); //[3:0]: reg_i2s_out_data_0~3_en, [7:4]: reg_i2s_out_data_0~3_au_en
#else
    //i2s pad signal from DAC(Hydun)
    msWriteByteMask(REG_000407, 0xF0, 0xF0); //reg_i2s_out_en [0]: 0:disable, 1:enable i2s output pad control on pad GPIO_AU1, AU2, AU3
    msWriteByteMask(REG_000416, 0xF0, 0xF0); //[3:0]: reg_i2s_out_data_0~3_en, [7:4]: reg_i2s_out_data_0~3_au_en
    msWriteByteMask(REG_14026C, 0x10, 0x70); //reg_i2s_tx_ctrl_0[6:4]: Haydn wclk setting, 48fs
#endif

    //*** BK: COMBO_GP_TOP ***//
    msWriteByteMask(REG_1630B2, 0x00, 0x3F); //reg_auoe[3:0]: i2s 0~7 out OEN, [4]:spdif out OEN, [5]: i2s out mute OEN
    //audio clock source selection
    //msWriteByteMask(REG_163004, 0x30, (enInputPort<<4)); //reg_au_clk_sel[6:4]: audio engine source clock sel
    //sample source selection
    //msWriteByteMask(REG_163088, enInputPort, 0x0F); //[3:0]: reg_audio_src_sel
    //gain
    msWriteByte(REG_1630A5, 0xC0); //**MUST SET** reg_gainp[7:0]: 0xC0 is 0 dB, default 0x00 is mute will cause i2s out data fade out

    //*** BK: COMBO_PLL_TOP ***//
    //formula:  ( 12Mhz * 2^km * fbdiv ) / (2^kn * 2^kp * ddiv)
    msWriteByte(REG_162F12, 0x22); //[7:4]: reg_apll_km_reg, [3:0]:reg_apll_divkp_reg
    msWriteByteMask(REG_162F13, 0x00, 0x60); //[6:5]: reg_apll_kn_reg
    msWriteByte(REG_162F15, 0x62); //[3:0]:reg_aupll_ddiv, [7:4]:reg_aupll_fbdiv
    msWriteByteMask(REG_162F07, 0x00, 0x01); //reg_apll_pwdwn[0]: turn off power down
    msWriteByte(REG_162F0E, 0x50); //reg_apll_test_in: makes apll lock
    //CTS
    msWriteByteMask(REG_162F30, 0x80, 0x80); //reg_en_cts_n_synth[7]: enable Audio HDMI CTS-N synthesizer, [0]: Select CTS[19:0].
    msWriteByteMask(REG_162F40, 0x00, 0x03); //reg_apll_src_sel[0]: APLL synthesizer DIN, FBIN input select, [1]: APLL input control and clock source select
    //msWriteByteMask(REG_162F40, (enInputPort<<4), 0xF0); //reg_apll_ls_clk_sel[7:4]: APLL clock source selection
    msWriteByteMask(REG_162F11, 0x80, 0x80); //reg_apll_pdll_testd[7]: bypass DP audio DPLL test mode
    //msWriteByteMask(REG_162F07, 0x10, 0x10); //[4]: reg_apll_freeze

    //BK:combo_top 0~5
    //msWriteByteMask(REG_16064B + wOffset, 0x04, 0x04); //[2]: reg_i2s_mclk_192fs_sel: 192f2(MHL/HDMI only)

    //fading setting
#if ENABLE_AUDIO_AUTO_FADING
    msWriteByteMask(REG_1630A0, 0x08, 0x0C); // [3:2]: reg_fading_mode, 10: automatic fading mode
#endif
    msWriteByteMask(REG_163093, 0x0C, 0x0C); // [3:2]: reg_fad_speed_sel, lfading speed select, 11: 8X
    msWrite2Byte(REG_16308C, 0xFFFF); // reg_zero_cross[15:0]: zero-crossing threshold

}

//**************************************************************************
//  [Function Name]:
//                  mhal_combo_ClockRtermControl()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_combo_ClockRtermControl(BYTE ucPortSelect, Bool bRtermEnable)
{
    switch(ucPortSelect)
    {
        case COMBO_INPUT_SELECT_PORT0:
            msWriteByteMask(REG_001781, bRtermEnable? 0: BIT0, BIT0); // REG_PM_BANK_40[8]: Power control P0 clock r-term
			break;

        case COMBO_INPUT_SELECT_PORT1:
		    msWriteByteMask(REG_001781, bRtermEnable? 0: BIT1, BIT1); // REG_PM_BANK_40[9]: Power control P1 clock r-term
			break;

        case COMBO_INPUT_SELECT_PORT2:
		    msWriteByteMask(REG_001781, bRtermEnable? 0: BIT2, BIT2); // REG_PM_BANK_40[10]: Power control P1 clock r-term
			break;

        case COMBO_INPUT_SELECT_PORT3:
		    msWriteByteMask(REG_001781, bRtermEnable? 0: BIT3, BIT3); // REG_PM_BANK_40[11]: Power control P1 clock r-term
			break;

        case COMBO_INPUT_SELECT_PORT4:
		    msWriteByteMask(REG_001781, bRtermEnable? 0: BIT4, BIT4); // REG_PM_BANK_40[12]: Power control P1 clock r-term
			break;

        case COMBO_INPUT_SELECT_PORT5:
		    msWriteByteMask(REG_001781, bRtermEnable? 0: BIT5, BIT5); // REG_PM_BANK_40[13]: Power control P1 clock r-term
			break;

        default:

            break;
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_combo_DataRtermControl()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_combo_DataRtermControl(BYTE ucPortSelect, Bool bRtermEnable)
{
    switch(ucPortSelect)
    {
        case COMBO_INPUT_SELECT_PORT0:
            msWriteByteMask(REG_0017C4, bRtermEnable? 0: MASKBIT(3:0), MASKBIT(3:0)); // REG_PM_BANK_62[3:0], Power control port0 data rterm.
			break;

        case COMBO_INPUT_SELECT_PORT1:
            msWriteByteMask(REG_0017C4, bRtermEnable? 0: MASKBIT(7:4), MASKBIT(7:4)); // REG_PM_BANK_62[7:4], Power control port1 data rterm.
			break;

        case COMBO_INPUT_SELECT_PORT2:
            msWriteByteMask(REG_0017C5, bRtermEnable? 0: MASKBIT(3:0), MASKBIT(3:0)); // REG_PM_BANK_62[11:8], Power control port2 data rterm.
			break;

        case COMBO_INPUT_SELECT_PORT3:
            msWriteByteMask(REG_0017A2, bRtermEnable? 0: MASKBIT(3:0), MASKBIT(3:0)); // REG_PM_BANK_51[3:0], Power control port3 data rterm.
			break;

        case COMBO_INPUT_SELECT_PORT4:
            msWriteByteMask(REG_0017A2, bRtermEnable? 0: MASKBIT(7:4), MASKBIT(7:4)); // REG_PM_BANK_51[7:4], Power control port4 data rterm.
			break;

        case COMBO_INPUT_SELECT_PORT5:
            msWriteByteMask(REG_0017A3, bRtermEnable? 0: MASKBIT(3:0), MASKBIT(3:0)); // REG_PM_BANK_51[11:8], Power control port5 data rterm.
			break;

        default:

            break;
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_combo_DoSWReset()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_combo_DoSWReset(BYTE ucPortSelect, DWORD ulSWResetValue)
{
    WORD usSWResetValueL = ulSWResetValue;
    WORD usSWResetValueH = (ulSWResetValue >> 16);

    switch(ucPortSelect)
    {
        case COMBO_INPUT_SELECT_PORT0:
            if(usSWResetValueL > 0)
            {
                msWrite2Byte(REG_160652, usSWResetValueL); // REG_COMBO_ATOP0_29[15:0]: SW reset
                msWrite2Byte(REG_160652, 0); // REG_COMBO_ATOP0_29[15:0]: SW reset
            }

            if(usSWResetValueH > 0)
            {
                msWriteByteMask(REG_160654, usSWResetValueH, MASKBIT(1:0)); // REG_COMBO_ATOP0_2A[1:0]: SW reset
                msWriteByteMask(REG_160654, 0, MASKBIT(1:0)); // REG_COMBO_ATOP0_2A[1:0]: SW reset
            }

			break;

        case COMBO_INPUT_SELECT_PORT1:
            if(usSWResetValueL > 0)
            {
                msWrite2Byte(REG_160C52, usSWResetValueL); // REG_COMBO_ATOP1_29[15:0]: SW reset
                msWrite2Byte(REG_160C52, 0); // REG_COMBO_ATOP1_29[15:0]: SW reset
            }

            if(usSWResetValueH > 0)
            {
                msWriteByteMask(REG_160C54, usSWResetValueH, MASKBIT(1:0)); // REG_COMBO_ATOP1_2A[1:0]: SW reset
                msWriteByteMask(REG_160C54, 0, MASKBIT(1:0)); // REG_COMBO_ATOP1_2A[1:0]: SW reset
            }

			break;

        case COMBO_INPUT_SELECT_PORT2:
            if(usSWResetValueL > 0)
            {
                msWrite2Byte(REG_161452, usSWResetValueL); // REG_COMBO_ATOP2_29[15:0]: SW reset
                msWrite2Byte(REG_161452, 0); // REG_COMBO_ATOP2_29[15:0]: SW reset
            }

            if(usSWResetValueH > 0)
            {
                msWriteByteMask(REG_161454, usSWResetValueH, MASKBIT(1:0)); // REG_COMBO_ATOP2_2A[1:0]: SW reset
                msWriteByteMask(REG_161454, 0, MASKBIT(1:0)); // REG_COMBO_ATOP2_2A[1:0]: SW reset
            }

			break;

        case COMBO_INPUT_SELECT_PORT3:
            if(usSWResetValueL > 0)
            {
                msWrite2Byte(REG_161C52, usSWResetValueL); // REG_COMBO_ATOP3_29[15:0]: SW reset
                msWrite2Byte(REG_161C52, 0); // REG_COMBO_ATOP3_29[15:0]: SW reset
            }

            if(usSWResetValueH > 0)
            {
                msWriteByteMask(REG_161C54, usSWResetValueH, MASKBIT(1:0)); // REG_COMBO_ATOP3_2A[1:0]: SW reset
                msWriteByteMask(REG_161C54, 0, MASKBIT(1:0)); // REG_COMBO_ATOP3_2A[1:0]: SW reset
            }

			break;

        case COMBO_INPUT_SELECT_PORT4:
            if(usSWResetValueL > 0)
            {
                msWrite2Byte(REG_162452, usSWResetValueL); // REG_COMBO_ATOP4_29[15:0]: SW reset
                msWrite2Byte(REG_162452, 0); // REG_COMBO_ATOP4_29[15:0]: SW reset
            }

            if(usSWResetValueH > 0)
            {
                msWriteByteMask(REG_162454, usSWResetValueH, MASKBIT(1:0)); // REG_COMBO_ATOP4_2A[1:0]: SW reset
                msWriteByteMask(REG_162454, 0, MASKBIT(1:0)); // REG_COMBO_ATOP4_2A[1:0]: SW reset
            }

			break;

        case COMBO_INPUT_SELECT_PORT5:
            if(usSWResetValueL > 0)
            {
                msWrite2Byte(REG_162C52, usSWResetValueL); // REG_COMBO_ATOP5_29[15:0]: SW reset
                msWrite2Byte(REG_162C52, 0); // REG_COMBO_ATOP5_29[15:0]: SW reset
            }

            if(usSWResetValueH > 0)
            {
                msWriteByteMask(REG_162C54, usSWResetValueH, MASKBIT(1:0)); // REG_COMBO_ATOP5_2A[1:0]: SW reset
                msWriteByteMask(REG_162C54, 0, MASKBIT(1:0)); // REG_COMBO_ATOP5_2A[1:0]: SW reset
            }

			break;

        default:

            break;
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_combo_ConfigAudioSource()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_combo_ConfigAudioSource(Bool bIsTMDS, BYTE ucPortSelect, BYTE ucHBR2Select)
{
    MHALCOMBO_PRINT("mhal_combo_ConfigAudioSource\n");
    MHALCOMBO_PRINT("isTMDS = %d\n", bIsTMDS);
    MHALCOMBO_PRINT("ucComboSelect = 0x%x\n", ucPortSelect);

    //select Audio Data Source
    msWriteByteMask(REG_163004, (ucPortSelect <<4), 0x70);

    //select Audio Clock Source
    if(bIsTMDS == TRUE)
    {
        msWriteByteMask(REG_163088, ucPortSelect, 0x0F);
    }
    else
    {
        if(ucHBR2Select > COMBO_DP_HBR2_ENGINE_1) // Combo path
        {
            msWriteByteMask(REG_163088, (ucPortSelect +6), 0x0F);
        }
        else // HBR2 path
        {
            msWriteByteMask(REG_163088, (ucHBR2Select +12), 0x0F);
        }
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_combo_ConfigAPLLSynthesizer()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_combo_ConfigAPLLSynthesizer(Bool bIsTMDS, BYTE ucPortSelect, BYTE ucHBR2Select)
{
    MHALCOMBO_PRINT("mhal_combo_ConfigAPLLSynthesizer\n");
    MHALCOMBO_PRINT("isTMDS = %d\n", bIsTMDS);
    MHALCOMBO_PRINT("ucComboSelect = 0x%x\n", ucPortSelect);

    if(bIsTMDS == TRUE)
    {
        msWriteByteMask(REG_162F41, 0x00, BIT0); // disable APLL VCO frequency x2
        msWriteByteMask(REG_162F40, 0x00, BIT0|BIT1);
        msWriteByteMask(REG_162F22, 0, BIT5);
    }
    else //DP
    {
        if(ucHBR2Select > COMBO_DP_HBR2_ENGINE_1) // Combo path
        {
            msWriteByteMask(REG_162F41, 0, BIT0); // Enable APLL VCO frequency x2
        }
        else // HBR2 path
        {
            ucPortSelect = ucPortSelect +6;

            msWriteByteMask(REG_162F41, BIT0, BIT0); // Enable APLL VCO frequency x2
        }
                
        msWriteByteMask(REG_162F40, BIT0| BIT1, BIT0| BIT1);
        //DP only
        msWriteByteMask(REG_162F06, 0, BIT0); // reg_apll_sel_dp_pll_div
        msWriteByteMask(REG_162F11, 0, BIT7); //reg_apll_dpll_testd
    }

    msWriteByteMask(REG_162F40, (ucPortSelect <<4), 0xF0); // APLL clock source select;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_combo_ConfigFadingEvent()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_combo_ConfigFadingEvent(Bool bEnable, BYTE ucEvents)
{
    if ( bEnable == TRUE )
        msWriteByte(REG_16308E, ucEvents);
                                //[0]: HDMI CTS/N over range / DP audio sample error
                                //[1]: HDMI TMDS clock big change / DP audio sample parity error
                                //[2]: HDMI AVMUTE / DP non-PCM
                                //[3]: HDMI no input clock / DP audio mute (VB_ID[4])
                                //[5]: HDMI audio channel status change / DP no input recovery clock
                                //[7]: HDMI audio sample error / DP audio FIFO error
    else
        msWriteByte(REG_16308E, 0x00);
}

//**************************************************************************
//  [Function Name]:
//                  mhal_combo_ConfigMuteEvent()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_combo_ConfigMuteEvent(Bool bEnable, BYTE ucEvents)
{
    if ( bEnable == TRUE )
        msWriteByte(REG_16308B, ucEvents);
                                //[0]: HDMI CTS/N over range / DP audio sample error
                                //[1]: HDMI TMDS clock big change / DP audio sample parity error
                                //[2]: HDMI AVMUTE / DP non-PCM
                                //[3]: HDMI no input clock / DP audio mute (VB_ID[4])
                                //[5]: HDMI audio channel status change / DP no input recovery clock
                                //[7]: HDMI audio sample error / DP audio FIFO error
    else
        msWriteByte(REG_16308B, 0x00);
}

//**************************************************************************
//  [Function Name]:
//                  mhal_combo_ConfigGlobalMute()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_combo_ConfigGlobalMute(Bool bEnable)
{
    if ( bEnable == TRUE )
        msWriteByteMask(REG_163088, BIT4, BIT4);
    else
        msWriteByteMask(REG_163088, 0x00, BIT4);
}

//**************************************************************************
//  [Function Name]:
//                  mhal_combo_GetDVIDualMainPort()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE mhal_combo_GetDVIDualMainPort(BYTE ucPortSelect)
{
    if(ucPortSelect %2) // Port 1/3/5
    {
        ucPortSelect = ucPortSelect -1;
    }
    else // Port 0/2/4
    {
        ucPortSelect = ucPortSelect +1;
    }

    return ucPortSelect;
}

#endif



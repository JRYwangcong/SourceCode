///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    mhal_tmds.c
/// @author MStar Semiconductor Inc.
/// @brief  TMDS driver Function
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MHAL_TMDS_C_
#define _MHAL_TMDS_C_

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "Global.h"
#include <string.h>
#include "mhal_tmds.h"
#include "mapi_tmds.h"

#include "Board.h"

#define TMDS_DEBUG    0
#if ENABLE_MSTV_UART_DEBUG && TMDS_DEBUG
#define TMDS_PRINT(format, ...)     printf(format, ##__VA_ARGS__)
#else
#define TMDS_PRINT(format, ...)
#endif

#define LinkLockTH  10

static WORD wPreviousTiming = 0;
static BYTE bPreviousPort = MSCombo_TMDS_PORT_MAX + 1; //make sure even port 0 will setting EQ for first time ISR
Bool LinkLockCheckFlag[TMDS_INPUT_SELECT_MASK][2];
BYTE LinkLockCheckCnts[TMDS_INPUT_SELECT_MASK][2];

//**************************************************************************
//  [Function Name]:
//                  _mhal_tmds_bank_offset()
//  [Description]:
//                  use to get combo bank offset
//  [Arguments]:
//                  enInputPort : combo port
//  [Return]:
//                  comb bank offset
//**************************************************************************
WORD _mhal_tmds_bank_offset(MSCombo_TMDS_PORT_INDEX enInputPort)
{
    WORD wOffset;

    switch(enInputPort)
    {
        default:
        case MSCombo_TMDS_PORT_0:
            wOffset = 0x0000;
            break;
        case MSCombo_TMDS_PORT_1:
            wOffset = 0x0600;
            break;
        case MSCombo_TMDS_PORT_2:
            wOffset = 0x0E00;
            break;
        case MSCombo_TMDS_PORT_3:
            wOffset = 0x1600;
            break;
        case MSCombo_TMDS_PORT_4:
            wOffset = 0x1E00;
            break;
        case MSCombo_TMDS_PORT_5:
            wOffset = 0x2600;
            break;
    }

    return wOffset;

}


//**************************************************************************
//  [Function Name]:
//                  _mhal_tmds_phy_offset()
//  [Description]:
//                  use to get combo_phy_top offset
//  [Arguments]:
//                  enInputPort : combo port
//  [Return]:
//                  combo_phy_top bank offset
//**************************************************************************
WORD _mhal_tmds_phy_offset(MSCombo_TMDS_PORT_INDEX enInputPort)
{
    WORD wOffset;

    switch(enInputPort)
    {
        default:
        case MSCombo_TMDS_PORT_0:
            wOffset = 0x0000;
            break;
        case MSCombo_TMDS_PORT_1:
            wOffset = 0x0100;
            break;
        case MSCombo_TMDS_PORT_2:
            wOffset = 0x0200;
            break;
        case MSCombo_TMDS_PORT_3:
            wOffset = 0x0300;
            break;
        case MSCombo_TMDS_PORT_4:
            wOffset = 0x0400;
            break;
        case MSCombo_TMDS_PORT_5:
            wOffset = 0x0500;
            break;
    }

    return wOffset;

}

//**************************************************************************
//  [Function Name]:
//                  _mhal_tmds_UpdateCTSNValue()
//  [Description]:
//                  update CTS/N value and configure its limit value to setup upper/lower bound
//  [Arguments]:
//                  MSCombo_TMDS_PORT_INDEX enInputPort
//  [Return]:
//                  none
//**************************************************************************
void _mhal_tmds_UpdateCTSNValue(MSCombo_TMDS_PORT_INDEX enInputPort)
{
    WORD wOffset;

    wOffset = _mhal_tmds_bank_offset(enInputPort);

    //load current CTS/N value first
    msWriteByteMask(REG_1603EF + wOffset, 0x40, 0x40);

    //update CTS value
    msWrite2Byte(REG_16030A + wOffset , msRead2Byte(REG_160378 + wOffset)); //CTS0, [15..0]
    msWriteByteMask(REG_160308 + wOffset, (msReadByte(REG_16037A + wOffset) & 0x0F), 0x0F); //CTS1, b[3..0]

    //update N value
    msWrite2Byte(REG_16030C + wOffset , msRead2Byte(REG_16037C + wOffset)); //N0, [15..0]
    msWriteByteMask(REG_160308 + wOffset, (msReadByte(REG_16037A + wOffset) & 0xF0), 0xF0); //N1, b[3..0]

    //disable CTS/N filter
    msWriteByteMask(REG_160305 + wOffset, 0x00, 0x40);

    //setup CTS/N range
    msWrite2Byte(REG_16030E + wOffset, 0x0A0A ); //[15:8]: N range, [7:0]: CTS range

    //upload new CTS/N value
    msWriteByteMask(REG_1603EF + wOffset, 0x80, 0x80);

    //enable CTS/N filter
    msWriteByteMask(REG_160305 + wOffset, 0x40, 0x40);
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_tmds_SetCbusPullDown100K()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_tmds_SetCbusPullDown100K(MSCombo_TMDS_PORT_INDEX enPortSelect, Bool bSetFlag)
{
    switch(enPortSelect)
    {
        case MSCombo_TMDS_PORT_0:
            msWriteByteMask(REG_002F40, bSetFlag? BIT7: 0, BIT7); // REG_PM_MHL_CBUS0_20[7], Cbus pull down 100K port0
            break;

        case MSCombo_TMDS_PORT_1:
            msWriteByteMask(REG_003140, bSetFlag? BIT7: 0, BIT7); // REG_PM_MHL_CBUS1_20[7], Cbus pull down 100K port1
            break;

        case MSCombo_TMDS_PORT_2:
            msWriteByteMask(REG_003340, bSetFlag? BIT7: 0, BIT7); // REG_PM_MHL_CBUS2_20[7], Cbus pull down 100K port2
            break;

        case MSCombo_TMDS_PORT_3:
            msWriteByteMask(REG_003540, bSetFlag? BIT7: 0, BIT7); // REG_PM_MHL_CBUS3_20[7], Cbus pull down 100K port3
            break;

        case MSCombo_TMDS_PORT_4:
            msWriteByteMask(REG_003740, bSetFlag? BIT7: 0, BIT7); // REG_PM_MHL_CBUS4_20[7], Cbus pull down 100K port3
            break;

        case MSCombo_TMDS_PORT_5:
            msWriteByteMask(REG_003940, bSetFlag? BIT7: 0, BIT7); // REG_PM_MHL_CBUS5_20[7], Cbus pull down 100K port3
            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_tmds_SetNoInputInterrupt()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_tmds_SetNoInputInterrupt(MSCombo_TMDS_PORT_INDEX enPortSelect, Bool bEnable)
{
    switch(enPortSelect)
    {
        case MSCombo_TMDS_PORT_0:
            msWriteByteMask(REG_16065C, bEnable? 0: BIT3, BIT3); // REG_COMBO_TOP0_2E[3], No input mask port0
            msWriteByteMask(REG_16065A, BIT3, BIT3); // REG_COMBO_TOP0_2D[3], No input clear port0
            msWriteByteMask(REG_16065A, 0, BIT3); // REG_COMBO_TOP0_2D[3], No input clear port0
            break;

        case MSCombo_TMDS_PORT_1:
            msWriteByteMask(REG_160C5C, bEnable? 0: BIT3, BIT3); // REG_COMBO_TOP1_2E[3], No input mask port1
            msWriteByteMask(REG_160C5A, BIT3, BIT3); // REG_COMBO_TOP1_2D[3], No input clear port1
            msWriteByteMask(REG_160C5A, 0, BIT3); // REG_COMBO_TOP1_2D[3], No input clear port1
            break;

        case MSCombo_TMDS_PORT_2:
            msWriteByteMask(REG_16145C, bEnable? 0: BIT3, BIT3); // REG_COMBO_TOP2_2E[3], No input mask port2
            msWriteByteMask(REG_16145A, BIT3, BIT3); // REG_COMBO_TOP2_2D[3], No input clear port2
            msWriteByteMask(REG_16145A, 0, BIT3); // REG_COMBO_TOP2_2D[3], No input clear port2
            break;

        case MSCombo_TMDS_PORT_3:
            msWriteByteMask(REG_161C5C, bEnable? 0: BIT3, BIT3); // REG_COMBO_TOP3_2E[3], No input mask port3
            msWriteByteMask(REG_161C5A, BIT3, BIT3); // REG_COMBO_TOP3_2D[3], No input clear port3
            msWriteByteMask(REG_161C5A, 0, BIT3); // REG_COMBO_TOP3_2D[3], No input clear port3
            break;

        case MSCombo_TMDS_PORT_4:
            msWriteByteMask(REG_16245C, bEnable? 0: BIT3, BIT3); // REG_COMBO_TOP4_2E[3], No input mask port4
            msWriteByteMask(REG_16245A, BIT3, BIT3); // REG_COMBO_TOP4_2D[3], No input clear port4
            msWriteByteMask(REG_16245A, 0, BIT3); // REG_COMBO_TOP4_2D[3], No input clear port4
            break;

        case MSCombo_TMDS_PORT_5:
            msWriteByteMask(REG_162C5C, bEnable? 0: BIT3, BIT3); // REG_COMBO_TOP5_2E[3], No input mask port5
            msWriteByteMask(REG_162C5A, BIT3, BIT3); // REG_COMBO_TOP5_2D[3], No input clear port5
            msWriteByteMask(REG_162C5A, 0, BIT3); // REG_COMBO_TOP5_2D[3], No input clear port5
            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_tmds_SetClockValidInterrupt()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_tmds_SetClockValidInterrupt(MSCombo_TMDS_PORT_INDEX enPortSelect, Bool bEnable)
{
    switch(enPortSelect)
    {
        case MSCombo_TMDS_PORT_0:
            msWriteByteMask(REG_16065D, bEnable? 0: BIT5, BIT5); // REG_COMBO_TOP0_2E[13], clock stable mask port0

            if(!mhal_tmds_PM_Clock_Detect(enPortSelect))
            {
                msWriteByteMask(REG_16065B, BIT5, BIT5); // REG_COMBO_TOP0_2D[13], clock stable  clear port0
                msWriteByteMask(REG_16065B, 0, BIT5); // REG_COMBO_TOP0_2D[13], clock stable  clear port0
            }

            break;

        case MSCombo_TMDS_PORT_1:
            msWriteByteMask(REG_160C5D, bEnable? 0: BIT5, BIT5); // REG_COMBO_TOP1_2E[13], clock stable  mask port1

            if(!mhal_tmds_PM_Clock_Detect(enPortSelect))
            {
                msWriteByteMask(REG_160C5B, BIT5, BIT5); // REG_COMBO_TOP1_2D[13], clock stable  clear port1
                msWriteByteMask(REG_160C5B, 0, BIT5); // REG_COMBO_TOP1_2D[13], clock stable  clear port1
            }

            break;

        case MSCombo_TMDS_PORT_2:
            msWriteByteMask(REG_16145D, bEnable? 0: BIT5, BIT5); // REG_COMBO_TOP2_2E[13], clock stable  mask port2

            if(!mhal_tmds_PM_Clock_Detect(enPortSelect))
            {
                msWriteByteMask(REG_16145B, BIT5, BIT5); // REG_COMBO_TOP2_2D[13], clock stable  clear port2
                msWriteByteMask(REG_16145B, 0, BIT5); // REG_COMBO_TOP2_2D[13], clock stable  clear port2
            }

            break;

        case MSCombo_TMDS_PORT_3:
            msWriteByteMask(REG_161C5D, bEnable? 0: BIT5, BIT5); // REG_COMBO_TOP3_2E[13], clock stable  mask port3

            if(!mhal_tmds_PM_Clock_Detect(enPortSelect))
            {
                msWriteByteMask(REG_161C5B, BIT5, BIT5); // REG_COMBO_TOP3_2D[13], clock stable  clear port3
                msWriteByteMask(REG_161C5B, 0, BIT5); // REG_COMBO_TOP3_2D[13], clock stable  clear port3
            }

            break;

        case MSCombo_TMDS_PORT_4:
            msWriteByteMask(REG_16245D, bEnable? 0: BIT5, BIT5); // REG_COMBO_TOP4_2E[13], clock stable  mask port4

            if(!mhal_tmds_PM_Clock_Detect(enPortSelect))
            {
                msWriteByteMask(REG_16245B, BIT5, BIT5); // REG_COMBO_TOP4_2D[13], clock stable  clear port4
                msWriteByteMask(REG_16245B, 0, BIT5); // REG_COMBO_TOP4_2D[13], clock stable  clear port4
            }

            break;

        case MSCombo_TMDS_PORT_5:
            msWriteByteMask(REG_162C5D, bEnable? 0: BIT5, BIT5); // REG_COMBO_TOP5_2E[13], clock stable  mask port5

            if(!mhal_tmds_PM_Clock_Detect(enPortSelect))
            {
                msWriteByteMask(REG_162C5B, BIT5, BIT5); // REG_COMBO_TOP5_2D[13], clock stable  clear port5
                msWriteByteMask(REG_162C5B, 0, BIT5); // REG_COMBO_TOP5_2D[13], clock stable  clear port5
            }

            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_tmds_CbusPadEnable()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void _mhal_tmds_CbusPadEnable(BYTE u8ComboPort, Bool bEnableCbus)
{
    switch(u8ComboPort)
    {
        case MSCombo_TMDS_PORT_0:
            msWrite2ByteMask(REG_000460, bEnableCbus? BIT0: 0, BIT0); // REG_PAD_TOP_30[0], Cbus port0 enable
            msWrite2ByteMask(REG_000462, bEnableCbus? 0: BIT1, BIT1); // REG_PAD_TOP_31[1], enable pad pull-high port0
            break;

        case MSCombo_TMDS_PORT_1:
            msWrite2ByteMask(REG_000460, bEnableCbus? BIT1: 0, BIT1); // REG_PAD_TOP_30[1], Cbus port1 enable
            msWrite2ByteMask(REG_000462, bEnableCbus? 0: BIT3, BIT3); // REG_PAD_TOP_31[3], enable pad pull-high port1
            break;

        case MSCombo_TMDS_PORT_2:
            msWrite2ByteMask(REG_000460, bEnableCbus? BIT2: 0, BIT2); // REG_PAD_TOP_30[2], Cbus port2 enable
            msWrite2ByteMask(REG_000462, bEnableCbus? 0: BIT5, BIT5); // REG_PAD_TOP_31[5], enable pad pull-high port2
            break;

        case MSCombo_TMDS_PORT_3:
            msWrite2ByteMask(REG_000460, bEnableCbus? BIT3: 0, BIT3); // REG_PAD_TOP_30[3], Cbus port3 enable
            msWrite2ByteMask(REG_000462, bEnableCbus? 0: BIT7, BIT7); // REG_PAD_TOP_31[7], enable pad pull-high port3
            break;

        case MSCombo_TMDS_PORT_4:
            msWrite2ByteMask(REG_000460, bEnableCbus? BIT4: 0, BIT4); // REG_PAD_TOP_30[4], Cbus port4 enable
            msWrite2ByteMask(REG_000462, bEnableCbus? 0: BIT9, BIT9); // REG_PAD_TOP_31[9], enable pad pull-high port4
            break;

        case MSCombo_TMDS_PORT_5:
            msWrite2ByteMask(REG_000460, bEnableCbus? BIT5: 0, BIT5); // REG_PAD_TOP_30[5], Cbus port5 enable
            msWrite2ByteMask(REG_000462, bEnableCbus? 0: BIT11, BIT11); // REG_PAD_TOP_31[11], enable pad pull-high port5
            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_PM_Clock_Detect_Initial()
//  [Description]:
//                  set PM clock detect initial setting
//  [Arguments]:
//                  enInputPort : combo port
//  [Return]:
//                  none
//**************************************************************************
void mhal_tmds_PM_Clock_Detect_Initial(void)
{
	msWriteByteMask(REG_00038E, 0, BIT4|BIT3|BIT1|BIT0); // 47[4]: reg_dvi_clk_det_clr, [3]: reg_dviclk_gate, [1:0]: reg_dviclk_sel

	// combo 0
	msWriteByteMask(REG_00039F, BIT0, BIT0); // 4F[8]: reg_en_dvi_clk_det
	msWriteByteMask(REG_00038F, 0, BIT3|BIT1|BIT0); // 47[11]: reg_dviraw_clk_gate, [9:8]: reg_dviraw_clk_sel

	// combo 1
	msWriteByteMask(REG_00039F, BIT1, BIT1); // 4F[9]: reg_en_dvi_clk_det
	msWriteByteMask(REG_000394, 0, BIT3|BIT1|BIT0); // 4A[3]: reg_dviraw_clk_gate, [1:0]: reg_dviraw_clk_sel

	// combo 2
	msWriteByteMask(REG_00039F, BIT2, BIT2); //4F[10]:  reg_en_dvi_clk_det
	msWriteByteMask(REG_000394, 0, BIT7|BIT5|BIT4); //  4A[7]: reg_dviraw_clk_gate, [5:4]: reg_dviraw_clk_sel

	// combo 3
	msWriteByteMask(REG_00039F, BIT3, BIT3); // 4F[11]: reg_en_dvi_clk_det
	msWriteByteMask(REG_000395, 0, BIT3|BIT1|BIT0); // 4A[11]: reg_dviraw_clk_gate, [9:8]: reg_dviraw_clk_sel

	// combo 4
	msWriteByteMask(REG_00039E, BIT0, BIT0); //4F[0]:	reg_en_dvi_clk_det
	msWriteByteMask(REG_000395, 0, BIT7|BIT5|BIT4); //	4A[15]: reg_dviraw_clk_gate, [13:12]: reg_dviraw_clk_sel

	// combo 5
	msWriteByteMask(REG_00039E, BIT1, BIT1); // 4F[1]: reg_en_dvi_clk_det
	msWriteByteMask(REG_000396, 0, BIT3|BIT1|BIT0); // 4B[3]: reg_dviraw_clk_gate, [1:0]: reg_dviraw_clk_sel
}

//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_ClockRtermControl()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_tmds_ClockRtermControl(BYTE ucPortSelect, Bool bRtermEnable)
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
//                  mhal_tmds_CheckAudioErrorStatus()
//  [Description]:
//                  Checking current Audio fading/mute event status bit, after store, then clear it
//                  #define ERR_HDMI_CTSN_OVERRANGE     _BIT0
//                  #define ERR_HDMI_CLK_BIG_CHANGE     _BIT1
//                  #define ERR_HDMI_AVMUTE             _BIT2
//                  #define ERR_HDMI_NO_INPUT_CLK       _BIT3
//                  #define ERR_HDMI_AUD_SMP_FLAT       _BIT4
//                  #define ERR_HDMI_AUD_CH_ST_CHANGE   _BIT5
//                  #define ERR_HDMI_AUD_FIFO_ERR       _BIT6
//                  #define ERR_HDMI_AUD_SMP_ERR     _BIT7
//  [Arguments]:
//                  MSCombo_TMDS_PORT_INDEX enInputPort
//  [Return]:
//                  none
//**************************************************************************
BYTE mhal_tmds_CheckAudioErrorStatus(MSCombo_TMDS_PORT_INDEX enInputPort, BOOL bClrAVMute)
{
    WORD wOffset;

    BYTE bAudioErrorStatus = 0;
#if (ENABLE_AUDIO_AUTO_FADING == 1)
    static BYTE bPreAudioErrorStatus = 0;
#endif

    #if (ENABLE_AUDIO_AUTO_FADING == 1)
        bAudioErrorStatus |= msReadByte(REG_16308E);
    #endif
    #if (ENABLE_AUDIO_AUTO_MUTE == 1)
        bAudioErrorStatus |= msReadByte(REG_16308B);
    #endif

    //CTS/N over range
    if (bAudioErrorStatus & ERR_HDMI_CTSN_OVERRANGE)
    {
        wOffset = _mhal_tmds_bank_offset(enInputPort);
        if (msReadByte(REG_1603EC + wOffset) & BIT6) //[6]: CTS N over range
        {
            _mhal_tmds_UpdateCTSNValue(enInputPort);
            msWriteByteMask(REG_1603EC + wOffset, 0x40, 0x40); // clear CTS/N over range
        }
        else
            bAudioErrorStatus &= (~ERR_HDMI_CTSN_OVERRANGE);
    }

    //Clock Big Change
    if (bAudioErrorStatus & ERR_HDMI_CLK_BIG_CHANGE)
    {
        wOffset = _mhal_tmds_phy_offset(enInputPort);
        if (msReadByte(REG_112064 + wOffset) & BIT2) //[2]: Clock  big change
        {
            msWriteByteMask(REG_112064 + wOffset, 0x04, 0x04); //clear clk big change status;
        }
        else
            bAudioErrorStatus &= (~ERR_HDMI_CLK_BIG_CHANGE);
    }

    //AV mute
    if (bAudioErrorStatus & ERR_HDMI_AVMUTE)
    {
        wOffset = _mhal_tmds_bank_offset(enInputPort);
        if (bClrAVMute)
        {
            msWriteByteMask(REG_16031A + wOffset, 0x01, 0x01); // raise clear AV mute status
            msWriteByteMask(REG_160317 + wOffset, 0x01, 0x01); // clear AV mute
            msWriteByteMask(REG_160317 + wOffset, 0x00, 0x01); // clear AV mute
            bAudioErrorStatus &= (~ERR_HDMI_AVMUTE);
        }
    }

    //No Input Clock
    if (bAudioErrorStatus & ERR_HDMI_NO_INPUT_CLK)
    {
        if (!(msReadByte(REG_162F33) & BIT7)) //[7]: CTS N synthesizer input detect
            //NOTE: this status will auto clear
            bAudioErrorStatus &= (~ERR_HDMI_NO_INPUT_CLK);
    }

    //Audio Sample Flat
    if (bAudioErrorStatus & ERR_HDMI_AUD_SMP_FLAT)
    {
        if (msReadByte(REG_1630B0) & BIT7) //[7]: ch1~ ch8 flat bit
        {
            msWriteByteMask(REG_1630B0, BIT7, BIT7);
        }
        else
            bAudioErrorStatus &= (~ERR_HDMI_AUD_SMP_FLAT);
    }

    //Audio Channel Status Change
    if (bAudioErrorStatus & ERR_HDMI_AUD_CH_ST_CHANGE)
    {
        if (msReadByte(REG_1630B0) & BIT2) //[2]: audio ch status change
        {
            msWriteByteMask(REG_1630B0, BIT2, BIT2);
        }
        else
            bAudioErrorStatus &= (~ERR_HDMI_AUD_CH_ST_CHANGE);
    }

    //Audio FIFO Error (underflow / overflow)
    if (bAudioErrorStatus & ERR_HDMI_AUD_FIFO_ERR)
    {
        if (msReadByte(REG_1630B1) & (BIT1 | BIT0)) //[1]: ch1/ch2 over run, [0]: ch1/ch2 under run
        {
            //msWriteByteMask(REG_163069, 0x80, 0x80); // [7] : reset audio module
            //msWriteByteMask(REG_163069, 0x00, 0x80);
            msWriteByteMask(REG_1630B1, (BIT1 | BIT0), (BIT1 | BIT0));
        }
        else
            bAudioErrorStatus &= (~ERR_HDMI_AUD_FIFO_ERR);
    }

    //Audio Sample Error (sample error / polarity error)
    if (bAudioErrorStatus & ERR_HDMI_AUD_SMP_ERR)
    {
        if (msReadByte(REG_1630B0) & (BIT1 | BIT0)) //[1]: audio parity error, [0]:audio sample error
        {
            msWriteByteMask(REG_1630B0, (BIT1 | BIT0), (BIT1 | BIT0));
        }
        else
            bAudioErrorStatus &= (~ERR_HDMI_AUD_SMP_ERR);
    }

    // Audio fading patch due to non zero crossing audio pattern
#if (ENABLE_AUDIO_AUTO_FADING == 1)
    if((bPreAudioErrorStatus !=0) && (bAudioErrorStatus == 0)) // Has been happened audio error then becomes normal now
    {
        if((msReadByte(REG_16308F) & BIT0) == 0) // if fading status is still fading out due to some DC offset audio pattern, force to fading in
        {
            msWriteByteMask(REG_1630A0, BIT0, BIT0); // trigger to fading in
            //ForceDelay1ms(1);
            msWriteByteMask(REG_1630A0, 0, BIT0);
            TMDS_PRINT("\r\n@@ AudioFaing Patch\r\n");
        }
    }
    bPreAudioErrorStatus = bAudioErrorStatus;
#endif

    return bAudioErrorStatus;

}

//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_ConfigAudioPort()
//  [Description]:
//                  Configurate all audio setting which is relative to specific port.
//                  Call flow:
//                      1. mhal_tmds_HDMIInitialAudio()
//                  After decided which port is going to be actived, then call
//                      2. mhal_tmds_ConfigAudioPort()
//                          -> mhal_tmds_ConfigAudioSource()
//                          -> mhal_tmds_ConfigAudioFadingEvent()
//  [Arguments]:
//                  none
//  [Return]:
//                  none
//**************************************************************************
void mhal_tmds_ConfigAudioPort(MSCombo_TMDS_PORT_INDEX enInputPort)
{
    WORD wOffset = 0;

    wOffset = _mhal_tmds_bank_offset(enInputPort);
    //BK:combo_top 0~5
    msWriteByteMask(REG_16064B + wOffset, 0x04, 0x04); //[2]: reg_i2s_mclk_192fs_sel: 192f2(MHL/HDMI only)

    _mhal_tmds_UpdateCTSNValue(enInputPort);
    msWriteByteMask(REG_1603EC + wOffset, 0x40, 0x40); // clear CTS/N over range

    msWrite2ByteMask(REG_1630B0, 0x3FF, 0x3FF); // clear all status;
    msWriteByteMask(REG_160317 + wOffset, 0x01, 0x01); // clear AV mute
    msWriteByteMask(REG_160317 + wOffset, 0x00, 0x01); // clear AV mute
}

//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_EnableAudioMute()
//  [Description]:
//                  Enable/Disable Global Mute function
//  [Arguments]:
//                  EnableMute: if TRUE then trigger global mute, otherwise turn off global mute
//  [Return]:
//                  None
//**************************************************************************
void mhal_tmds_EnableAudioMute(Bool EnableMute)
{
    if (EnableMute) //trigger global mute
        msWriteByteMask(REG_163088, BIT4, BIT4);
    else
        msWriteByteMask(REG_163088, 0, BIT4);
}
//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_HDMI20_MUX()
//  [Description]:
//                  mhl3_atop selection
//  [Arguments]:
//                  enInputPort : combo port
//  [Return]:
//                  None
//**************************************************************************
void mhal_tmds_HDMI20_MUX(MSCombo_TMDS_PORT_INDEX enInputPort)
{
    if(enInputPort)
    {
        // do nothing
    }
}


//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_Initial()
//  [Description]:
//                  initital setting for each combo port,
//                  if enInputPort is dual link port, also config the sub-link
//  [Arguments]:
//                  enInputPort : combo port
//                  bDual : dual link port or not
//  [Return]:
//                  None
//**************************************************************************
void mhal_tmds_Initial(MSCombo_TMDS_PORT_INDEX enInputPort, Bool bDual)
{
    WORD wOffset;

    wPreviousTiming = 0;
    bPreviousPort = MSCombo_TMDS_PORT_MAX + 1; //make sure even port 0 will setting EQ for first time ISR

    wOffset = _mhal_tmds_phy_offset(enInputPort);
    //eq 3tage setting
    msWriteByteMask(REG_1120BE + wOffset, BIT6|BIT5, BIT6|BIT5); //lane0 always turn on 3 stage.
    msWriteByteMask(REG_1120C0 + wOffset, BIT6|BIT5, BIT6|BIT5); //lane1 always turn on 3 stage.
    msWriteByteMask(REG_1120C2 + wOffset, BIT6|BIT5, BIT6|BIT5); //lane2 always turn on 3 stage.
    msWriteByteMask(REG_1120C4 + wOffset, BIT6|BIT5, BIT6|BIT5); //lane3 always turn on 3 stage.

    msWriteByteMask(REG_112000 + wOffset, 0x8F, 0x8F); //[7]: Main link function select: HDMI/MHL, [3:0]: PN Swap
    msWrite2Byte(REG_112016 + wOffset, 0xFFFF); //Clock enable
    msWrite2Byte(REG_112018 + wOffset, 0x3FFF); //Clock enable
    msWriteByteMask(REG_1120AE + wOffset, 0x0C, 0xFF); //Set Xtal freq
    msWrite2Byte(REG_163006, 0xFFFF); //Clock enable
    msWrite2Byte(REG_163008, 0x3FFF);

    LinkLockCheckFlag[enInputPort][0] = FALSE;
    LinkLockCheckFlag[enInputPort][1] = FALSE;
    LinkLockCheckCnts[enInputPort][0] = 0;
    LinkLockCheckCnts[enInputPort][1] = 0;
#if 1 // HDMI default mode
    msWriteByteMask(REG_112020 + wOffset, 0x10, 0x10); //[4]: SWAP_DCDR_UPDN_POLITY
    msWriteByteMask(REG_112029 + wOffset, 0x00, 0x10); //[4]: PHFSM0_FAST_EN
    msWriteByteMask(REG_11202B + wOffset, 0x00, 0x10); //[4]: PHFSM1_FAST_EN
    msWriteByteMask(REG_11202D + wOffset, 0x00, 0x10); //[4]: PHFSM2_FAST_EN

#if 0
    //feedback divider for MHL mode
    msWriteByteMask(REG_1120DA + wOffset, 0, 0x10); //Disable Lane0 overwrite
    msWriteByteMask(REG_1120E1 + wOffset, 0, 0x03); //Divsel_MHL_L0 value
    msWriteByteMask(REG_1120DA + wOffset, 0, 0x20); //Disable Lane1 overwrite
    msWriteByteMask(REG_1120E1 + wOffset, 0, 0x0C); //Divsel_MHL_L1 value
    msWriteByteMask(REG_1120DA + wOffset, 0, 0x40); //Disable Lane2 overwrite
    msWriteByteMask(REG_1120E1 + wOffset, 0, 0x30); //Divsel_MHL_L2 value
    msWriteByteMask(REG_1120DA + wOffset, 0, 0x80); //Disable Lane3 overwrite
    msWriteByteMask(REG_1120E1 + wOffset, 0, 0xC0); //Divsel_MHL_L3 value
    //disable DP full rate mode
    msWriteByteMask(REG_1120DC + wOffset, 0, 0x02); //[1]: disable overwrite
    msWriteByteMask(REG_1120E5 + wOffset, 0, 0x04); //[2]: acdr_fullrate_ov

    //disable LA of each lane
    msWriteByteMask(REG_1120DC + wOffset, 0, 0x3C); //[5:2]: disable overwrite
    msWriteByteMask(REG_1120E5 + wOffset, 0, 0x78);

    //enable PHD feedback mode for HDMI new mode
    msWriteByteMask(REG_1120DD + wOffset, 0, 0x0F); //[3:0]: disable overwrite
#endif

    if(enInputPort == MSCombo_TMDS_PORT_0)
    {
        msWrite2Byte(REG_163012, 0xFFFF); //Clock enable
        msWrite2Byte(REG_163014, 0x3FFF);
        msWriteByteMask(REG_0017A6, 0, 0x01); // REG_EN_SQH = 0
        msWriteByteMask(REG_0017A7, 0, 0x01); // REG_PD_CLKIN_OFFL = 0
        msWriteByteMask(REG_0017C4, 0, 0x0E); //[3:1]: Power on P0 data r-term
        msWriteByteMask(REG_001781, 0, 0x01); //[0]: Power on P0 clock r-term
    }
    else if(enInputPort == MSCombo_TMDS_PORT_1)
    {
        msWrite2Byte(REG_163022, 0xFFFF); //Clock enable
        msWrite2Byte(REG_163024, 0x3FFF);
        msWriteByteMask(REG_0017A6, 0, 0x02); // REG_EN_SQH = 0
        msWriteByteMask(REG_0017A7, 0, 0x02); // REG_PD_CLKIN_OFFL = 0
        msWriteByteMask(REG_0017C4, 0, 0xE0); //[7:4]: Power on P1 data r-term
        msWriteByteMask(REG_001781, 0, 0x02); //[1]: Power on P1 clock r-term
    }
    else if(enInputPort == MSCombo_TMDS_PORT_2)
    {
        msWrite2Byte(REG_163032, 0xFFFF); //Clock enable
        msWrite2Byte(REG_163034, 0x3FFF);
        msWriteByteMask(REG_0017A6, 0, 0x04); // REG_EN_SQH = 0
        msWriteByteMask(REG_0017A7, 0, 0x04); // REG_PD_CLKIN_OFFL = 0
        msWriteByteMask(REG_0017C5, 0, 0x0E); //[3:1]: Power on P2 data r-term
        msWriteByteMask(REG_001781, 0, 0x04); //[2]: Power on P2 clock r-term
    }
    else if(enInputPort == MSCombo_TMDS_PORT_3)
    {
        msWrite2Byte(REG_163042, 0xFFFF); //Clock enable
        msWrite2Byte(REG_163044, 0x3FFF);
        msWriteByteMask(REG_0017A6, 0, 0x08); // REG_EN_SQH = 0
        msWriteByteMask(REG_0017A7, 0, 0x08); // REG_PD_CLKIN_OFFL = 0
        msWriteByteMask(REG_0017A2, 0, 0x0E); //[3:1]: Power on P3 data r-term
        msWriteByteMask(REG_001781, 0, 0x08); //[3]: Power on P3 clock r-term
    }
    else if(enInputPort == MSCombo_TMDS_PORT_4)
    {
        msWrite2Byte(REG_163050, 0xFFFF); //Clock enable
        msWrite2Byte(REG_163052, 0x3FFF);
        msWriteByteMask(REG_0017A6, 0, 0x10); // REG_EN_SQH = 0
        msWriteByteMask(REG_0017A7, 0, 0x10); // REG_PD_CLKIN_OFFL = 0
        msWriteByteMask(REG_0017A2, 0, 0xE0); //[7:4]: Power on P4 data r-term
        msWriteByteMask(REG_001781, 0, 0x10); //[4]: Power on P4 clock r-term
    }
    else if(enInputPort == MSCombo_TMDS_PORT_5)
    {
        msWrite2Byte(REG_16305E, 0xFFFF); //Clock enable
        msWrite2Byte(REG_163060, 0x3FFF);
        msWriteByteMask(REG_0017A6, 0, 0x20); // REG_EN_SQH = 0
        msWriteByteMask(REG_0017A7, 0, 0x20); // REG_PD_CLKIN_OFFL = 0
        msWriteByteMask(REG_0017A3, 0, 0x0E); //[3:1]: Power on P5 data r-term
        msWriteByteMask(REG_001781, 0, 0x20); //[5]: Power on P5 clock r-term
    }

#if 1
    msWriteByteMask(REG_1120D6 + wOffset, 0x02, 0x02); //[1]: REG_ATOP_EN_CLKO_PIX = 1
    msWriteByteMask(REG_1120D9 + wOffset, 0, 0x01); //[0]: REG_ATOP_EN_DP
    msWriteByteMask(REG_1120D7 + wOffset, 0, 0x01); //[0]: REG_ATOP_PD_BG
    msWriteByteMask(REG_1120D6 + wOffset, 0, 0x08); //[3]: REG_ATOP_PD_CLKIN
    msWriteByteMask(REG_1120B6 + wOffset, 0x01, 0x0F); //[3:0]: REG_ATOP_PD_LANE

#endif
    //R-term value over write
    msWriteByteMask(REG_1120B3 + wOffset, 0, 0x01); //[0]: overwtite
    msWriteByteMask(REG_1120B7 + wOffset, 0, 0xF0); //[7:4]: R-term value

    msWriteByteMask(REG_1120B8 + wOffset, 0, 0x0F); //[3:0]: REG_ATOP_SEL_PHDCLK
#endif

    //CR Lock Mode
    msWriteByteMask(REG_11208A + wOffset, 0x83, 0x8F); //[7]: DIG lock new mode, 0: DP mode, 1:MHL/HDMI mode

    //CLK valid boundary
    //msWrite2ByteMask(REG_112074 + wOffset, 0x0886, 0x1FFF);//CLK_VALID_L, keep default value currently
    msWrite2ByteMask(REG_112076 + wOffset, 0x1F00, 0x1FFF);//CLK_VALID_U

    //xtal divider setting, default value is ok, we don't modify here
    //msWriteByte(REG_112062 + wOffset, 0x3F);//clock_xtal_divider_HBR

    //CR lock/unlock threshold, setting default value currently
    //msWrite2Byte(0x112090 + wOffset, 0x0702); //MAINLINK_CRLOCK_THR[7:0], MAINLINK_CRLOSE_THR[8:15];

    //CR lock filter, setting default value currently
    //msWriteByteMask(0x11205D + wOffset, 0x06, 0x1F); //CR_LOCK_TH[4:0]

    //HDMI_MHL divider threshold
    msWrite2Byte(REG_112092 + wOffset, 0x0B43); //HDMI_CLK_THR4, the value of cr_lock_cnt in 270Mhz
    msWrite2Byte(REG_112094 + wOffset, 0x05A0); //HDMI_CLK_THR3, the value of cr_lock_cnt in 135Mhz
    msWrite2Byte(REG_112096 + wOffset, 0x02D0); //HDMI_CLK_THR2, the value of cr_lock_cnt in 67.5Mhz
    msWrite2Byte(REG_112098 + wOffset, 0x0168); //HDMI_CLK_THR1, the value of cr_lock_cnt in 33.75Mhz

    msWriteByteMask(REG_1120DC + wOffset, 0xC0, 0xC0); //atop_gc_eq_l0_ove and atop_gc_eq_l1_ove
    msWriteByteMask(REG_1120DD + wOffset, 0x03, 0x03); //atop_gc_eq_l2_ove and atop_gc_eq_l3_ove
    msWrite2Byte(REG_1120E6 + wOffset, (TMDS_HDMI14_R_CHANNEL_EQ<<10)|(TMDS_HDMI14_G_CHANNEL_EQ<<5)|TMDS_HDMI14_B_CHANNEL_EQ);
    msWriteByteMask(REG_1120E8 + wOffset, TMDS_HDMI14_CLK_CHANNEL_EQ, 0x1F);

    //auto clear enz for hit coundary
    msWriteByteMask(REG_11203B + wOffset, 0x00, 0x01);

    //digital lock part
    //msWriteByteMask(0x11208A + wOffset, 0x03, 0x0F); //DIG_LCK_CNT_PWR[3:0]
    //msWriteByte(0x11208E + wOffset, 0xE1); //DIG_LOSE_RANGE[7:4], DIG_LCK_RANGE[3:0]
    //msWriteByteMask(0x11208B + wOffset, 0x1F, 0x7F); //DIG_LOCK_TIME[6:0]

    wOffset = _mhal_tmds_bank_offset(enInputPort);

    //patch for HDCP issue, reset HDCP FSM when no input clock
    msWriteByteMask(REG_16040D + wOffset, BIT5, BIT5);

    msWriteByteMask(REG_1606A3 + wOffset, 0x80, 0x80); //word boundary detect unlock check
    msWrite2Byte(0x1630EE, 0x1FFF); //clk to scaler

    msWriteByteMask(REG_160303 + wOffset, 0x0C, 0x0C); //[3] : enable video mute, [2] : blanking when AVmute is active.
    msWriteByteMask(REG_160304 + wOffset, 0x03, 0x03); //[1] : auto reset deep color FIFO, [0] : enable deep color mode.
    msWriteByteMask(REG_16032A + wOffset, 0x02, 0x02); //[1] : force 422 to 444.
    msWriteByteMask(REG_16031E + wOffset, 0x80, 0xF0); //[7] : Frame repetition manual mode

    //mask interrupt
    //msWrite2Byte(REG_16065C + wOffset, 0xFFFF); //combo irq mask
    msWrite2Byte(REG_160318 + wOffset, 0xFFFF); //HDMI pkt irq mask
    msWriteByteMask(REG_160409 + wOffset, 0xFF, 0xFF); //HDCP pkt irq mask

    //word boundary by RD suggest
    msWriteByteMask(REG_16051D + wOffset, 0x00, 0xFF);
    msWriteByteMask(REG_160521 + wOffset, 0x00, 0xFF);
    msWriteByteMask(REG_160525 + wOffset, 0x00, 0xFF);
    msWriteByteMask(REG_160529 + wOffset, 0x00, 0xFF);

    if(bDual)
    {
        switch (enInputPort)
        {
            case MSCombo_TMDS_PORT_0:
            case MSCombo_TMDS_PORT_2:
            case MSCombo_TMDS_PORT_4:
                mhal_tmds_Initial( (MSCombo_TMDS_PORT_INDEX)(enInputPort+1), FALSE);
                mhal_tmds_MaskComboIRQ((MSCombo_TMDS_PORT_INDEX)(enInputPort+1), FALSE);
                wOffset = _mhal_tmds_bank_offset((MSCombo_TMDS_PORT_INDEX)(enInputPort+1));
                msWriteByteMask(REG_160452 + wOffset, 0x40, 0x40);
                msWriteByteMask(REG_160303 + wOffset, 0x00, 0x0C); //[3] : enable video mute, [2] : blanking when AVmute is active.
                #if 0   //for HDCP main/sub signal sync setting
                    msWriteByteMask(REG_160633 + wOffset, 0, BIT7); //sub lane setting

                    wOffset = _mhal_tmds_bank_offset(enInputPort);
                    msWriteByteMask(REG_160633 + wOffset, 0, BIT7); //main land setting
                #endif
            break;
            case MSCombo_TMDS_PORT_1:
            case MSCombo_TMDS_PORT_3:
            case MSCombo_TMDS_PORT_5:
                mhal_tmds_Initial( (MSCombo_TMDS_PORT_INDEX)(enInputPort-1), FALSE);
                mhal_tmds_MaskComboIRQ((MSCombo_TMDS_PORT_INDEX)(enInputPort-1), FALSE);
                wOffset = _mhal_tmds_bank_offset((MSCombo_TMDS_PORT_INDEX)(enInputPort-1));
                msWriteByteMask(REG_160452 + wOffset, 0x40, 0x40);
                msWriteByteMask(REG_160303 + wOffset, 0x00, 0x0C); //[3] : enable video mute, [2] : blanking when AVmute is active.
                wOffset = _mhal_tmds_phy_offset(enInputPort);
                msWriteByte(REG_1120D6 + wOffset, 0xF7);
                wOffset = _mhal_tmds_phy_offset((MSCombo_TMDS_PORT_INDEX)(enInputPort - 1));
                msWriteByte(REG_1120B4 + wOffset, 0x06);
                msWriteByteMask(REG_1630F7, (1 << (enInputPort + 2)), (1 << (enInputPort + 2)));
                #if 0   //for HDCP main/sub signal sync setting
                    msWriteByteMask(REG_160633 + wOffset, 0, BIT7); //sub lane setting

                    wOffset = _mhal_tmds_bank_offset(enInputPort);
                    msWriteByteMask(REG_160633 + wOffset, 0, BIT7); //main lane setting
                #endif
            break;
            default:
                //invalid port index, return to prevent the code from accessing illegal register address
                //return;
            break;
        }
    }

    //switch to hdmi old mode
    #if 0
    msWriteByte(0x112020 + wOffset, 0x0000); //SWAP_DCDR_UPDN_POLITY; old mode=0, default = 1;

    msWriteByteMask(0x112029 + wOffset, 0x10, 0x10); //PHFSM0_FST_EN
    msWriteByte(0x112028 + wOffset, 0x80); //PHFSM0_FAST_INTERVAL
    msWriteByteMask(0x11202B + wOffset, 0x10, 0x10); //PHFSM1_FST_EN
    msWriteByte(0x11202A + wOffset, 0x80); //PHFSM1_FAST_INTERVAL
    msWriteByteMask(0x11202D + wOffset, 0x10, 0x10); //PHFSM2_FST_EN
    msWriteByte(0x11202C + wOffset, 0x80); //PHFSM2_FAST_INTERVAL

    msWriteByteMask(0x1120DF + wOffset, 0x0F, 0x0F); //gc_en_phd_fb_ov
    msWriteByteMask(0x1120EB + wOffset, 0x00, 0x78); //gc_phd_fb_ov
    msWriteByteMask(0x1120B8 + wOffset, 0x0F, 0x0F); //REG_SEL_PHDCLK
    #endif

    //enable new DCDR low pass filter
    wOffset = _mhal_tmds_phy_offset(enInputPort);

    //for U02:
    wOffset = _mhal_tmds_phy_offset(enInputPort);
    msWriteByteMask(REG_11205D + wOffset, 0x3E, 0x3E);  //CR lock success threshold time, to avoid CLK_Valid interrupt assert by accident.

    msWriteByte(REG_112020 + wOffset, 0x1D);
    msWriteByte(REG_112021 + wOffset, 0x00);    //[0]               reg_ph_update_timer_dis
                                                //[1]               reg_dp_dcdr_eco_en
                                                //[2]               reg_early_late_no_vote
                                                //[3]               reg_enable_phase_step
                                                //[4]               reg_swap_dcdr_updn_polity

    // upper_boundary ? filter depth, larger => PHDAC code change slow
    msWriteByte(REG_112030 + wOffset, 0xFF);
    msWriteByte(REG_112031 + wOffset, 0x00);

    // lower_boundary ? inverse of upper boundary
    msWriteByte(REG_112032 + wOffset, 0x00);
    msWriteByte(REG_112033 + wOffset, 0xFF);

    // Set Cbus pull down 100K
    _mhal_tmds_SetCbusPullDown100K(enInputPort, TRUE);

   //Set HDP pin control by CBUS pad & disable pull-high voltage level
    _mhal_tmds_CbusPadEnable(enInputPort, TRUE);


}

//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_EnableIRQ()
//  [Description]:
//                  enable certain port's IRQ
//  [Arguments]:
//                  enInputPort : combo port
//  [Return]:
//                  void
//
//**************************************************************************
void mhal_tmds_EnableIRQ(MSCombo_TMDS_PORT_INDEX enInputPort)
{
    msWriteByteMask(REG_163070, (1 << enInputPort), (1 << enInputPort)); //eanble ISR
}

//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_CLK_SRC_Switch()
//  [Description]:
//                  select clk detect src from pm or die domain
//  [Arguments]:
//                  enInputPort : combo port
//                  bFlag : TRUE = from PM, FALSE = from die domain.
//  [Return]:
//                  void
//
//**************************************************************************
void mhal_tmds_CLK_SRC_Switch(MSCombo_TMDS_PORT_INDEX enInputPort, bool bFlag)
{
    WORD wOffset;
    wOffset = _mhal_tmds_phy_offset(enInputPort);
    msWriteByteMask(REG_1120B9 + wOffset, bFlag ?0x04:0x00, 0x04); //switch clock detect from PM
}

//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_MaskComboIRQ()
//  [Description]:
//                  mask combo_TMDS IRQ
//  [Arguments]:
//                  enInputPort : combo port, bFlag : True to enable clock stable irq, False to disable.
//  [Return]:
//                  void
//
//**************************************************************************
void mhal_tmds_MaskComboIRQ(MSCombo_TMDS_PORT_INDEX enInputPort, Bool bFlag)
{
    _mhal_tmds_SetClockValidInterrupt(enInputPort, bFlag);
    //_mhal_tmds_SetNoInputInterrupt(enInputPort, bFlag);
}

//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_HDMIGetPktStatus()
//  [Description]:
//                  use to get HDMI received packet status
//  [Arguments]:
//                  enInputPort : combo port
//  [Return]:
//                  [0] : mpeg infoframe
//                  [1] : audio infoframe
//                  [2] : SPD
//                  [3] : AVI infoframe
//                  [4] : GC packet
//                  [5] : Audio sample packet
//                  [6] : ACR packet
//                  [7] : Vendor specific infoframe
//                  [8] : null packet
//                  [9] : ISRC2
//                  [A] : ISRC
//                  [B] : ACP
//                  [C] : DSD
//                  [E] : Gamut metadata packet
//                  [F] : HBR
//**************************************************************************
WORD mhal_tmds_HDMIGetPktStatus(MSCombo_TMDS_PORT_INDEX enInputPort)
{
    WORD wOffset, temp;

    wOffset = _mhal_tmds_bank_offset(enInputPort);
    temp = msRead2Byte(REG_1603EA + wOffset);
    msWrite2Byte(REG_1603EA + wOffset, 0xFFFF);

    return temp;
}


//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_HDMIGetErrorStatus()
//  [Description]:
//                  use to return HDMI error status
//  [Arguments]:
//                  enInputPort : combo port
//  [Return]:
//                  [0] : BCH error (corrected)
//                  [1] : BCH error
//                  [2] : Checksum error
//                  [3] : Unsupported packet received
//                  [4] : Audio sample error
//                  [5] : Audio sample parity bit error
//                  [6] : Deep color FIFO overflow
//                  [7] : Deep color FIFO underflow
//**************************************************************************
BYTE mhal_tmds_HDMIGetErrorStatus(MSCombo_TMDS_PORT_INDEX enInputPort)
{
    WORD wOffset;
    BYTE temp;

    wOffset = _mhal_tmds_bank_offset(enInputPort);
    temp = msReadByte(REG_1603EE + wOffset);
    msWriteByteMask(REG_1603EE + wOffset, 0xFF, 0xFF);

    return temp;
}


//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_DE_Stable()
//  [Description]:
//                  check input timing is stable or not
//  [Arguments]:
//                  enInputPort : combo port
//  [Return]:
//                  TRUE : Stable
//                  FALSE : Unstable
//**************************************************************************
Bool mhal_tmds_DE_Stable(MSCombo_TMDS_PORT_INDEX enInputPort)
{
    WORD wOffset;

    wOffset = _mhal_tmds_bank_offset(enInputPort);

    if(msReadByte(REG_16045D + wOffset) & 0x40)
        return TRUE;
    else
        return FALSE;
}


//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_Clock_Stable()
//  [Description]:
//                  check input clock is stable or not
//  [Arguments]:
//                  enInputPort : combo port
//  [Return]:
//                  TRUE : Stable
//                  FALSE : Unstable
//**************************************************************************
Bool mhal_tmds_Clock_Stable(MSCombo_TMDS_PORT_INDEX enInputPort)
{
    WORD wOffset;

    wOffset = _mhal_tmds_bank_offset(enInputPort);

    if(msReadByte(REG_16055C + wOffset) & 0x08)
        return TRUE;
    else
        return FALSE;

#if 0

    wOffset = _mhal_tmds_phy_offset(enInputPort);
    msWrite2Byte(REG_112064 + wOffset, 0xFFFF);

    //Delay1ms(70);

    if(msRead2Byte(REG_112064 + wOffset) == 0x9999)
        return TRUE;
    else
        return FALSE;

#endif

}



//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_PM_Clock_Detect()
//  [Description]:
//                  check PM clock is detect or not
//  [Arguments]:
//                  enInputPort : combo port
//  [Return]:
//                  TRUE : clock detct
//                  FALSE : no clock detect
//**************************************************************************
Bool mhal_tmds_PM_Clock_Detect(MSCombo_TMDS_PORT_INDEX enInputPort)
{
    Bool bCLKDET = FALSE;

    switch(enInputPort)
    {
        case MSCombo_TMDS_PORT_0:
			if(msReadByte(REG_0003A2) & BIT6) // 51[6]: reg_dvi_clk_det
			{
			    bCLKDET = TRUE;
			}
            break;

        case MSCombo_TMDS_PORT_1:
			if(msReadByte(REG_0003A1) & BIT0) // 50[8]: reg_dvi_clk_det
			{
			    bCLKDET = TRUE;
			}
            break;

        case MSCombo_TMDS_PORT_2:
			if(msReadByte(REG_0003A1) & BIT1) // 50[9]: reg_dvi_clk_det
			{
			    bCLKDET = TRUE;
			}
			break;

        case MSCombo_TMDS_PORT_3:
			if(msReadByte(REG_0003A1) & BIT2) // 50[10]: reg_dvi_clk_det
			{
			    bCLKDET = TRUE;
			}
            break;

        case MSCombo_TMDS_PORT_4:
			if(msReadByte(REG_0003A1) & BIT3) // 50[11]: reg_dvi_clk_det
			{
			    bCLKDET = TRUE;
			}
            break;

        case MSCombo_TMDS_PORT_5:
			if(msReadByte(REG_0003A1) & BIT4) // 50[12]: reg_dvi_clk_det
			{
			    bCLKDET = TRUE;
			}
            break;

        default:

            break;
    };

    return bCLKDET;

}
//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_GetInputType()
//  [Description]:
//                  check input is DVI or HDMI
//  [Arguments]:
//                  enInputPort : combo port
//  [Return]:
//                  TRUE : HDMI
//                  FALSE : DVI
//**************************************************************************
BYTE mhal_tmds_GetInputType(MSCombo_TMDS_PORT_INDEX enInputPort)
{
    WORD wOffset;

    wOffset = _mhal_tmds_bank_offset(enInputPort);
    if(msRead2Byte(REG_160418 + wOffset) & 0x0001)
        return 1;
    else
        return 0;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_SubDEChangeReset()
//  [Description]:
//                  reset DE change for sub link
//  [Arguments]:
//                  enInputPort : combo port
//  [Return]:
//
//**************************************************************************
void mhal_tmds_SubDEChangeReset(MSCombo_TMDS_PORT_INDEX enInputPort)
{
    WORD wOffset = 0;

    switch (enInputPort)
    {
        case MSCombo_TMDS_PORT_0:
        case MSCombo_TMDS_PORT_2:
        case MSCombo_TMDS_PORT_4:
            wOffset = _mhal_tmds_bank_offset((MSCombo_TMDS_PORT_INDEX)(enInputPort+1));
        break;
        case MSCombo_TMDS_PORT_1:
        case MSCombo_TMDS_PORT_3:
        case MSCombo_TMDS_PORT_5:
            wOffset = _mhal_tmds_bank_offset((MSCombo_TMDS_PORT_INDEX)(enInputPort-1));
        break;
        default:
            //invalid port index, return to prevent the code from accessing illegal register address
            //return;
        break;
    }

    msWriteByteMask(REG_160455 + wOffset, 0x10, 0x10);
    msWriteByteMask(REG_160455 + wOffset, 0x00, 0x10);

}

//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_DualLink_Align()
//  [Description]:
//
//  [Arguments]:
//                  enInputPort : combo port
//  [Return]:
//
//**************************************************************************
void mhal_tmds_DualLink_Align(MSCombo_TMDS_PORT_INDEX enInputPort)
{
    enInputPort = MSCombo_TMDS_PORT_0; //do nothing
}

//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_IsDualLink()
//  [Description]:
//                  check input is "Dual" or "Single" by small DE status of the sub lane.
//  [Arguments]:
//                  enInputPort : combo port
//  [Return]:
//                  TRUE : Dual link
//                  FALSE : Single link
//**************************************************************************
Bool mhal_tmds_IsDualLink(MSCombo_TMDS_PORT_INDEX enInputPort)
{
    WORD wOffset = 0;
    WORD wTmpValue = 0;
    BYTE i, debounceCnt = 10;
    Bool isDual = TRUE;

    switch (enInputPort)
    {
        case MSCombo_TMDS_PORT_0:
        case MSCombo_TMDS_PORT_2:
        case MSCombo_TMDS_PORT_4:
            wOffset = _mhal_tmds_bank_offset((MSCombo_TMDS_PORT_INDEX)(enInputPort+1));
        break;
        case MSCombo_TMDS_PORT_1:
        case MSCombo_TMDS_PORT_3:
        case MSCombo_TMDS_PORT_5:
            wOffset = _mhal_tmds_bank_offset((MSCombo_TMDS_PORT_INDEX)(enInputPort-1));
        break;
        default:
            //invalid port index, return to prevent the code from accessing illegal register address
            //return;
        break;
    }

    //solution for dual link cable coupling issue;
    for ( i=0; i < debounceCnt; i++ )
    {
        wTmpValue = msRead2Byte(REG_16045C + wOffset);
        if (!((wTmpValue & 0x4000) && (!(wTmpValue & 0x8000))))
        {
            isDual = FALSE;
            break;
        }
        else isDual = TRUE;
    }
    return isDual;

}

//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_MainDEChangeReset()
//  [Description]:
//                  reset DE change for main link
//  [Arguments]:
//                  enInputPort : combo port
//  [Return]:
//
//**************************************************************************
void mhal_tmds_MainDEChangeReset(MSCombo_TMDS_PORT_INDEX enInputPort)
{
    WORD wOffset = 0;

    wOffset = _mhal_tmds_bank_offset(enInputPort);

    msWriteByteMask(REG_160455 + wOffset, 0x10, 0x10);
    msWriteByteMask(REG_160455 + wOffset, 0x00, 0x10);

}

//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_IsMainLinkStable()
//  [Description]:
//                  check if DE stable for main link of dual DVI
//  [Arguments]:
//                  enInputPort : combo port
//  [Return]:
//                  TRUE : DE of main link is stable
//                  FALSE : DE of main link is unstable
//**************************************************************************
Bool mhal_tmds_IsMainLinkStable(MSCombo_TMDS_PORT_INDEX enInputPort)
{
    WORD wOffset = 0;
    WORD wTmpValue = 0;
    BYTE i, debounceCnt = 10;
    Bool isStable= TRUE;

    wOffset = _mhal_tmds_bank_offset(enInputPort);

    //solution for dual link cable coupling issue;
    for ( i=0; i < debounceCnt; i++ )
    {
        wTmpValue = msRead2Byte(REG_16045C + wOffset);
        if (!((wTmpValue & 0x4000) && (!(wTmpValue & 0x8000))))
        {
            isStable = FALSE;
            break;
        }
        else isStable = TRUE;
    }

    return isStable;

}


//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_DualLink_ComboMuxClk()
//  [Description]:
//                  TBD:
//  [Arguments]:
//                  enInputPort : combo port
//                  enInputPortSwitch: combo port
//  [Return]:
//                  void
//
//**************************************************************************
void mhal_tmds_DualLink_ComboMuxClk(MSCombo_TMDS_PORT_INDEX enInputPort, MSCombo_TMDS_PORT_INDEX enInputPortSwitch)
{
    BYTE u8InputPort, SCDetWin=0;
    BYTE u8ComboMux = (BYTE)enInputPortSwitch;
    BYTE u8ComboCLK = (BYTE)enInputPortSwitch;
    WORD u16SCMask;

    for( u8InputPort=First_Input_Port; u8InputPort < Input_Nums; u8InputPort++ )
    {
        if((MSCombo_TMDS_PORT_INDEX)(g_InputPort[u8InputPort].eIPMux-1) == enInputPort)
        {
            SCDetWin = 0;
            u16SCMask = MapPort2ScMask(u8InputPort);
            while( u16SCMask )
            {
                if( u16SCMask & BIT0 )
                {
                    //Combo Mux Select
                    switch(SCDetWin)
                    {
                        case SCL0_SUB:
                            msWriteByteMask(REG_1630E8, u8ComboMux << 4, 0xF0);
                            msWriteByteMask(REG_1630F0, u8ComboCLK << 4, 0xF0);
                            msWriteByteMask(REG_120F33, u8ComboCLK, 0x0F);
                            break;
                        case SCL0_MAIN:
                            msWriteByteMask(REG_1630E8, u8ComboMux, 0x0F);
                            msWriteByteMask(REG_1630F0, u8ComboCLK, 0x0F);
                            msWriteByteMask(REG_120F40, u8ComboCLK, 0x0F);
                            break;
                        case SCR0_SUB:
                            msWriteByteMask(REG_1630EB, u8ComboMux, 0x0F);
                            msWriteByteMask(REG_1630F3, u8ComboCLK, 0x0F);
                            msWriteByteMask(REG_120F43, u8ComboCLK, 0x0F);
                            break;
                        case SCR0_MAIN:
                            msWriteByteMask(REG_1630EA, u8ComboMux << 4, 0xF0);
                            msWriteByteMask(REG_1630F2, u8ComboCLK << 4, 0xF0);
                            msWriteByteMask(REG_120F44, u8ComboCLK, 0x0F);
                            break;
                        case SCL1_SUB:
                            msWriteByteMask(REG_1630EA, u8ComboMux, 0x0F);
                            msWriteByteMask(REG_1630F2, u8ComboCLK, 0x0F);
                            msWriteByteMask(REG_120F41, u8ComboCLK, 0x0F);
                            break;
                        case SCL1_MAIN:
                            msWriteByteMask(REG_1630E9, u8ComboMux << 4, 0xF0);
                            msWriteByteMask(REG_1630F1, u8ComboCLK << 4, 0xF0);
                            msWriteByteMask(REG_120F42, u8ComboCLK, 0x0F);
                            break;
                        case SCR1_SUB:
                            msWriteByteMask(REG_1630EC, u8ComboMux, 0x0F);
                            msWriteByteMask(REG_1630F4, u8ComboCLK, 0x0F);
                            msWriteByteMask(REG_120F45, u8ComboCLK, 0x0F);
                            break;
                        case SCR1_MAIN:
                            msWriteByteMask(REG_1630EB, u8ComboMux << 4, 0xF0);
                            msWriteByteMask(REG_1630F3, u8ComboCLK << 4, 0xF0);
                            msWriteByteMask(REG_120F46, u8ComboCLK, 0x0F);
                            break;
                        case SCL0_OFFLINE:
                            msWriteByteMask(REG_1630E9, u8ComboMux, 0x0F);
                            msWriteByteMask(REG_1630F1, u8ComboCLK, 0x0F);
                            msWriteByteMask(REG_120F32, u8ComboCLK, 0x0F);
                            break;
                    }
                }

                u16SCMask >>= 1;
                SCDetWin++;
            }
        }
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_DualLink_Setting()
//  [Description]:
//                  use to config DVI dual link setting
//  [Arguments]:
//                  enInputPort : combo port
//                  bFlag :
//                  1 : enable dual link setting
//                  0 : disable dual link setting
//  [Return]:
//                  None
//
//**************************************************************************
void mhal_tmds_DualLink_Setting(MSCombo_TMDS_PORT_INDEX enInputPort, Bool bFlag)
{
    WORD wOffset;
    Bool bIsSwapped;

    switch (enInputPort)
    {
        case MSCombo_TMDS_PORT_0:
            msWriteByteMask(REG_162F34, bFlag?0x02:0x00, 0x03);
            bIsSwapped = FALSE;
        break;
        case MSCombo_TMDS_PORT_2:
            msWriteByteMask(REG_162F34, bFlag?0x08:0x00, 0x0C);
            bIsSwapped = FALSE;
        break;
        case MSCombo_TMDS_PORT_4:
            msWriteByteMask(REG_162F34, bFlag?0x20:0x00, 0x30);
            bIsSwapped = FALSE;
        break;
        case MSCombo_TMDS_PORT_1:
            msWriteByteMask(REG_162F34, bFlag?0x02:0x00, 0x03);
            bIsSwapped = TRUE;
        break;
        case MSCombo_TMDS_PORT_3:
            msWriteByteMask(REG_162F34, bFlag?0x08:0x00, 0x0C);
            bIsSwapped = TRUE;
        break;
        case MSCombo_TMDS_PORT_5:
            msWriteByteMask(REG_162F34, bFlag?0x20:0x00, 0x30);
            bIsSwapped = TRUE;
        break;
        default:
            //invalid port index, return to prevent the code from accessing illegal register address
            return;
        break;
    }

    //main port is 0, 2, 4
    if ( !bIsSwapped )
    {
        wOffset = _mhal_tmds_phy_offset(enInputPort);
        //[Main Port] PHY Settings
        msWriteByteMask(REG_1120D6 + wOffset, bFlag ? 0x05 : 0, 0x05); //Main:[0]: Enable dual clk; [2]: 2x Pixel clk
        msWriteByteMask(REG_1120B4 + wOffset, bFlag ? 0x04 : 0, 0x04); //main: [2]: Enable 2x TMDS

        wOffset = _mhal_tmds_phy_offset((MSCombo_TMDS_PORT_INDEX)(enInputPort + 1));
        //[Sub Port] PHY Settings
        msWriteByteMask(REG_1120D6 + wOffset, bFlag ? 0x04 : 0, 0x04); //sub: [2]: 2x pixel clk
        msWriteByteMask(REG_1120B4 + wOffset, bFlag ? 0x06 : 0, 0x06); //sub: [1]:Enable dual clock input; [2]: Enable 2x TMDS

        //[Main Port] Settings
        wOffset = _mhal_tmds_bank_offset(enInputPort);

        //if MST9U2
        //msWriteByteMask(REG_160513 + wOffset, bFlag ? 0x00 : 0, (BIT4|BIT5)); //BK:TOP, main enable dual link data in
        //else
        msWriteByteMask(REG_160513 + wOffset, bFlag ? 0x20 : 0, 0x20); //BK:TOP, main enable dual link data in
        msWriteByteMask(REG_160633 + wOffset, bFlag ? 0x40 : 0, 0x40); //main, dual fifo
        msWriteByteMask(REG_160632 + wOffset, bFlag ? 0x10 : 0, 0x10); //enable of dual out FIFO

        //[Sub Port] Settings
        wOffset = _mhal_tmds_bank_offset((MSCombo_TMDS_PORT_INDEX)(enInputPort + 1));

        msWriteByteMask(REG_160633 + wOffset, 0x20, 0x20);
        msWriteByteMask(REG_160513 + wOffset, bFlag ? 0x10 : 0, 0x30); //BK: TOP, sub: enable dual link data out

        //[Index Relative] Settings
        // 3: start from BIT2, and ((Sub Port Index) = (Main Port Index) + 1), so index should be (Main Port Index + 1).
        msWriteByteMask(REG_1630F7, bFlag ? (1 << (enInputPort + 3)): 0, (1 << (enInputPort + 3))); // according to "SUB" port, switch lane clock
        //msWriteByteMask(REG_162F34, (1 << (enInputPort + 1)), 0x1F);

        mhal_tmds_DualLink_ComboMuxClk(enInputPort, enInputPort);
#if 0
        //adding for MST9U 2
        msWriteByteMask(REG_1630E8, bFlag ? enInputPort + 1 : enInputPort, 0x0F); //sc0_src_sel: sub port index.
        msWriteByteMask(REG_1630F0, bFlag ? enInputPort + 1 : enInputPort, 0x0F); //reg_combo2sc_clk_sel_0: sub port index
        //2: start from BIT2;
        //msWriteByteMask(REG_1630F7, bFlag ? (1 << (enInputPort + 2)) : 0, 0xFC); //main port index;

        msWriteByteMask(REG_160632 + wOffset, bFlag ? BIT4 : 0, BIT4);
        msWriteByteMask(REG_160633 + wOffset, bFlag ? (BIT4|BIT5) : BIT5, (BIT4|BIT5|BIT6|BIT7));
        //msWrite2ByteMask(REG_160632 + wOffset, 0x7310, 0x7310);
        //msWrite2ByteMask(REG_160632 + wOffset, bFlag ? 0x7310 : 0x6380, 0xFFFF);
#endif
    }
    //main port is 1, 3, 5
    else
    {
        if(bFlag)
        {
            wOffset = _mhal_tmds_phy_offset(enInputPort);
            msWriteByte(REG_1120D6 + wOffset, 0xF7);
		    msWriteByte(REG_1120B4 + wOffset, 0x04);
            wOffset = _mhal_tmds_phy_offset((MSCombo_TMDS_PORT_INDEX)(enInputPort - 1));
	        msWriteByte(REG_1120D6 + wOffset, 0xF6);
			wOffset = _mhal_tmds_bank_offset(enInputPort);
		    msWriteByte(REG_160513 + wOffset, 0x25);
		    msWriteByte(REG_160633 + wOffset, 0x43);
			msWriteByte(REG_160632 + wOffset, 0x10);
			wOffset = _mhal_tmds_bank_offset((MSCombo_TMDS_PORT_INDEX)(enInputPort - 1));
			msWriteByte(REG_160513 + wOffset, 0x15);
			mhal_tmds_DualLink_ComboMuxClk(enInputPort, (MSCombo_TMDS_PORT_INDEX)(enInputPort - 1));
		    msWriteByte(REG_160633 + wOffset, 0x33);
			msWriteByte(REG_160632 + wOffset, 0x10);
        }
        else
        {
            wOffset = _mhal_tmds_phy_offset(enInputPort);
            msWriteByte(REG_1120D6 + wOffset, 0xF3);
			msWriteByte(REG_1120B4 + wOffset, 0x00);
	        wOffset = _mhal_tmds_phy_offset((MSCombo_TMDS_PORT_INDEX)(enInputPort - 1));
			msWriteByte(REG_1120D6 + wOffset, 0xF2);
		    wOffset = _mhal_tmds_bank_offset(enInputPort);
		    msWriteByte(REG_160513 + wOffset, 0x05);
            msWriteByte(REG_160633 + wOffset, 0x03);
			msWriteByte(REG_160632 + wOffset, 0x00);
			wOffset = _mhal_tmds_bank_offset((MSCombo_TMDS_PORT_INDEX)(enInputPort - 1));
			msWriteByte(REG_160513 + wOffset, 0x05);
			mhal_tmds_DualLink_ComboMuxClk(enInputPort, enInputPort);
		    msWriteByte(REG_160633 + wOffset, 0x23);
			msWriteByte(REG_160632 + wOffset, 0x00);
        }
    }
}


//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_Link_LockClear()
//  [Description]:
//                 using to clear link lock information
//  [Arguments]:
//                 enInputPort : combo port
//  [Return]:
//                  None
//**************************************************************************
void mhal_tmds_Link_LockClear(MSCombo_TMDS_PORT_INDEX enInputPort)
{
    LinkLockCheckFlag[enInputPort][0] = FALSE; // Start Check Flag
    LinkLockCheckFlag[enInputPort][1] = FALSE; // Reset Condition Flag
    LinkLockCheckCnts[enInputPort][0] = 0; // Counts for check times.
    LinkLockCheckCnts[enInputPort][1] = 0; // Counts for stable times.
}


//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_Link_LockCheck()
//  [Description]:
//                 using to check data align
//  [Arguments]:
//                 enInputPort : combo port
//                 bFlag:
//                 TRUE for DVI dual link input
//                 FASLE for DVI single link or HDMI input
//  [Return]:
//                  None
//**************************************************************************
Bool mhal_tmds_Link_LockCheck(MSCombo_TMDS_PORT_INDEX enInputPort, Bool bFlag)
{
    if(LinkLockCheckFlag[enInputPort][0] == FALSE)
    {
        LinkLockCheckFlag[enInputPort][0] = TRUE;
        LinkLockCheckCnts[enInputPort][0] = 0;
        LinkLockCheckCnts[enInputPort][1] = 0;
        return FALSE;
    }
    else if(LinkLockCheckCnts[enInputPort][0] == LinkLockTH)
    {
        if(LinkLockCheckCnts[enInputPort][1] == LinkLockTH)
        {
            return TRUE;
        }
        else
        {
            LinkLockCheckCnts[enInputPort][0] = 0;
            LinkLockCheckCnts[enInputPort][1] = 0;
            LinkLockCheckFlag[enInputPort][1] = !(LinkLockCheckFlag[enInputPort][1]);
            if(bFlag) // dual timing
            {
                switch (enInputPort)
                {
                    case MSCombo_TMDS_PORT_0:
                        msWriteByteMask(REG_162F34, LinkLockCheckFlag[enInputPort][1] ? 0x03:0x00, 0x03);
                        msWriteByteMask(REG_162F34, 0x02, 0x03);
                    break;
                    case MSCombo_TMDS_PORT_2:
                        msWriteByteMask(REG_162F34, LinkLockCheckFlag[enInputPort][1] ? 0x0C:0x00, 0x0C);
                        msWriteByteMask(REG_162F34, 0x08, 0x0C);
                    break;
                    case MSCombo_TMDS_PORT_4:
                        msWriteByteMask(REG_162F34, LinkLockCheckFlag[enInputPort][1] ? 0x30:0x00, 0x30);
                        msWriteByteMask(REG_162F34, 0x20, 0x30);
                    break;
                    case MSCombo_TMDS_PORT_1:
                        msWriteByteMask(REG_162F34, LinkLockCheckFlag[enInputPort][1] ? 0x03:0x00, 0x03);
                        msWriteByteMask(REG_162F34, 0x02, 0x03);
                    break;
                    case MSCombo_TMDS_PORT_3:
                        msWriteByteMask(REG_162F34, LinkLockCheckFlag[enInputPort][1] ? 0x0C:0x00, 0x0C);
                        msWriteByteMask(REG_162F34, 0x08, 0x0C);
                    break;
                    case MSCombo_TMDS_PORT_5:
                        msWriteByteMask(REG_162F34, LinkLockCheckFlag[enInputPort][1] ? 0x30:0x00, 0x30);
                        msWriteByteMask(REG_162F34, 0x20, 0x30);
                    break;
                    default:
                        //invalid port index, return to prevent the code from accessing illegal register address
                    break;
                }
                    return FALSE;
            }
            else // single timing or HDMI input
            {
                msWriteByteMask(REG_162F34, 1<<enInputPort, 1<<enInputPort);
                msWriteByteMask(REG_162F34, 0x00, 1<<enInputPort);
                return FALSE;
            }
        }
    }
    return FALSE;
}


//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_PowerControl()
//  [Description]:
//                  use to control TMDS clock and Data R-term
//  [Arguments]:
//                  enInputPort : combo port
//                  bFlag :
//                  1 : power on
//                  0 : power down R-term
//  [Return]:
//                  None
//**************************************************************************
void mhal_tmds_PowerControl(MSCombo_TMDS_PORT_INDEX enInputPort, Bool bPowerOn, Bool bCLKDET)
{
    if(enInputPort == MSCombo_TMDS_PORT_0)
    {
        if(bPowerOn) // Normal
        {
            msWriteByteMask(REG_0017A7, 0, BIT(0)); // reg_pd_clkin_offl_p0
            msWriteByteMask(REG_1120D6, 0, BIT(3)); // pd_clkin
            msWriteByteMask(REG_0017A6, 0, BIT(0)); // reg_en_sqh_p0
            msWriteByteMask(REG_1120D7, 0, BIT(0)); // reg_pd_bg
            msWriteByteMask(REG_1120B6, 0, MASKBIT(3:0)); // reg_pd_lane[3:0]
            msWriteByteMask(REG_1120ED, 0, MASKBIT(7:4)); // reg_pd_phdac[3:0]
            msWriteByteMask(REG_1120D9, 0, BIT(1)); // reg_atop_gc_ictrl_pd_ov
        }
		else // bCLKDET = 1: Standby; bCLKDET = 0: DC-off
		{
            msWriteByteMask(REG_0017A7, bCLKDET ? 0:BIT(0), BIT(0)); // reg_pd_clkin_offl_p0
            msWriteByteMask(REG_1120D6, BIT(3), BIT(3)); // pd_clkin
            msWriteByteMask(REG_0017A6, 0, BIT(0)); // reg_en_sqh_p0
            msWriteByteMask(REG_1120D7, BIT(0), BIT(0)); // reg_pd_bg
            msWriteByteMask(REG_1120B6, MASKBIT(3:0), MASKBIT(3:0)); // reg_pd_lane[3:0]
            msWriteByteMask(REG_1120ED, MASKBIT(7:4), MASKBIT(7:4)); // reg_pd_phdac[3:0]
            msWriteByteMask(REG_1120D9, BIT(1), BIT(1)); // reg_atop_gc_ictrl_pd_ov
		}
    }
    else if(enInputPort == MSCombo_TMDS_PORT_1)
    {
        if(bPowerOn) // Normal
        {
            msWriteByteMask(REG_0017A7, 0, BIT(1)); // reg_pd_clkin_offl_p0
            msWriteByteMask(REG_1121D6, 0, BIT(3)); // pd_clkin
            msWriteByteMask(REG_0017A6, 0, BIT(1)); // reg_en_sqh_p0
            msWriteByteMask(REG_1121D7, 0, BIT(0)); // reg_pd_bg
            msWriteByteMask(REG_1121B6, 0, MASKBIT(3:0)); // reg_pd_lane[3:0]
            msWriteByteMask(REG_1121ED, 0, MASKBIT(7:4)); // reg_pd_phdac[3:0]
            msWriteByteMask(REG_1121D9, 0, BIT(1)); // reg_atop_gc_ictrl_pd_ov
        }
		else // bCLKDET = 1: Standby; bCLKDET = 0: DC-off
		{
            msWriteByteMask(REG_0017A7, bCLKDET ? 0:BIT(1), BIT(1)); // reg_pd_clkin_offl_p0
            msWriteByteMask(REG_1121D6, BIT(3), BIT(3)); // pd_clkin
            msWriteByteMask(REG_0017A6, 0, BIT(1)); // reg_en_sqh_p0
            msWriteByteMask(REG_1121D7, BIT(0), BIT(0)); // reg_pd_bg
            msWriteByteMask(REG_1121B6, MASKBIT(3:0), MASKBIT(3:0)); // reg_pd_lane[3:0]
            msWriteByteMask(REG_1121ED, MASKBIT(7:4), MASKBIT(7:4)); // reg_pd_phdac[3:0]
            msWriteByteMask(REG_1121D9, BIT(1), BIT(1)); // reg_atop_gc_ictrl_pd_ov
		}
    }
    else if(enInputPort == MSCombo_TMDS_PORT_2)
    {
        if(bPowerOn) // Normal
        {
            msWriteByteMask(REG_0017A7, 0, BIT(2)); // reg_pd_clkin_offl_p0
            msWriteByteMask(REG_1122D6, 0, BIT(3)); // pd_clkin
            msWriteByteMask(REG_0017A6, 0, BIT(2)); // reg_en_sqh_p0
            msWriteByteMask(REG_1122D7, 0, BIT(0)); // reg_pd_bg
            msWriteByteMask(REG_1122B6, 0, MASKBIT(3:0)); // reg_pd_lane[3:0]
            msWriteByteMask(REG_1122ED, 0, MASKBIT(7:4)); // reg_pd_phdac[3:0]
            msWriteByteMask(REG_1122D9, 0, BIT(1)); // reg_atop_gc_ictrl_pd_ov
        }
		else // bCLKDET = 1: Standby; bCLKDET = 0: DC-off
		{
            msWriteByteMask(REG_0017A7, bCLKDET ? 0:BIT(2), BIT(2)); // reg_pd_clkin_offl_p0
            msWriteByteMask(REG_1122D6, BIT(3), BIT(3)); // pd_clkin
            msWriteByteMask(REG_0017A6, 0, BIT(2)); // reg_en_sqh_p0
            msWriteByteMask(REG_1122D7, BIT(0), BIT(0)); // reg_pd_bg
            msWriteByteMask(REG_1122B6, MASKBIT(3:0), MASKBIT(3:0)); // reg_pd_lane[3:0]
            msWriteByteMask(REG_1122ED, MASKBIT(7:4), MASKBIT(7:4)); // reg_pd_phdac[3:0]
            msWriteByteMask(REG_1122D9, BIT(1), BIT(1)); // reg_atop_gc_ictrl_pd_ov
		}
    }
    else if(enInputPort == MSCombo_TMDS_PORT_3)
    {
        if(bPowerOn) // Normal
        {
            msWriteByteMask(REG_0017A7, 0, BIT(3)); // reg_pd_clkin_offl_p0
            msWriteByteMask(REG_1123D6, 0, BIT(3)); // pd_clkin
            msWriteByteMask(REG_0017A6, 0, BIT(3)); // reg_en_sqh_p0
            msWriteByteMask(REG_1123D7, 0, BIT(0)); // reg_pd_bg
            msWriteByteMask(REG_1123B6, 0, MASKBIT(3:0)); // reg_pd_lane[3:0]
            msWriteByteMask(REG_1123ED, 0, MASKBIT(7:4)); // reg_pd_phdac[3:0]
            msWriteByteMask(REG_1123D9, 0, BIT(1)); // reg_atop_gc_ictrl_pd_ov
        }
		else // bCLKDET = 1: Standby; bCLKDET = 0: DC-off
		{
            msWriteByteMask(REG_0017A7, bCLKDET ? 0:BIT(3), BIT(3)); // reg_pd_clkin_offl_p0
            msWriteByteMask(REG_1123D6, BIT(3), BIT(3)); // pd_clkin
            msWriteByteMask(REG_0017A6, 0, BIT(3)); // reg_en_sqh_p0
            msWriteByteMask(REG_1123D7, BIT(0), BIT(0)); // reg_pd_bg
            msWriteByteMask(REG_1123B6, MASKBIT(3:0), MASKBIT(3:0)); // reg_pd_lane[3:0]
            msWriteByteMask(REG_1123ED, MASKBIT(7:4), MASKBIT(7:4)); // reg_pd_phdac[3:0]
            msWriteByteMask(REG_1123D9, BIT(1), BIT(1)); // reg_atop_gc_ictrl_pd_ov
		}
    }
    else if(enInputPort == MSCombo_TMDS_PORT_4)
    {
        if(bPowerOn) // Normal
        {
            msWriteByteMask(REG_0017A7, 0, BIT(4)); // reg_pd_clkin_offl_p0
            msWriteByteMask(REG_1124D6, 0, BIT(3)); // pd_clkin
            msWriteByteMask(REG_0017A6, 0, BIT(4)); // reg_en_sqh_p0
            msWriteByteMask(REG_1124D7, 0, BIT(0)); // reg_pd_bg
            msWriteByteMask(REG_1124B6, 0, MASKBIT(3:0)); // reg_pd_lane[3:0]
            msWriteByteMask(REG_1124ED, 0, MASKBIT(7:4)); // reg_pd_phdac[3:0]
            msWriteByteMask(REG_1124D9, 0, BIT(1)); // reg_atop_gc_ictrl_pd_ov
        }
		else // bCLKDET = 1: Standby; bCLKDET = 0: DC-off
		{
            msWriteByteMask(REG_0017A7, bCLKDET ? 0:BIT(4), BIT(4)); // reg_pd_clkin_offl_p0
            msWriteByteMask(REG_1124D6, BIT(3), BIT(3)); // pd_clkin
            msWriteByteMask(REG_0017A6, 0, BIT(4)); // reg_en_sqh_p0
            msWriteByteMask(REG_1124D7, BIT(0), BIT(0)); // reg_pd_bg
            msWriteByteMask(REG_1124B6, MASKBIT(3:0), MASKBIT(3:0)); // reg_pd_lane[3:0]
            msWriteByteMask(REG_1124ED, MASKBIT(7:4), MASKBIT(7:4)); // reg_pd_phdac[3:0]
            msWriteByteMask(REG_1124D9, BIT(1), BIT(1)); // reg_atop_gc_ictrl_pd_ov
		}
    }
    else if(enInputPort == MSCombo_TMDS_PORT_5)
    {
        if(bPowerOn) // Normal
        {
            msWriteByteMask(REG_0017A7, 0, BIT(5)); // reg_pd_clkin_offl_p0
            msWriteByteMask(REG_1125D6, 0, BIT(3)); // pd_clkin
            msWriteByteMask(REG_0017A6, 0, BIT(5)); // reg_en_sqh_p0
            msWriteByteMask(REG_1125D7, 0, BIT(0)); // reg_pd_bg
            msWriteByteMask(REG_1125B6, 0, MASKBIT(3:0)); // reg_pd_lane[3:0]
            msWriteByteMask(REG_1125ED, 0, MASKBIT(7:4)); // reg_pd_phdac[3:0]
            msWriteByteMask(REG_1125D9, 0, BIT(1)); // reg_atop_gc_ictrl_pd_ov
        }
		else // bCLKDET = 1: Standby; bCLKDET = 0: DC-off
		{
            msWriteByteMask(REG_0017A7, bCLKDET ? 0:BIT(5), BIT(5)); // reg_pd_clkin_offl_p0
            msWriteByteMask(REG_1125D6, BIT(3), BIT(3)); // pd_clkin
            msWriteByteMask(REG_0017A6, 0, BIT(5)); // reg_en_sqh_p0
            msWriteByteMask(REG_1125D7, BIT(0), BIT(0)); // reg_pd_bg
            msWriteByteMask(REG_1125B6, MASKBIT(3:0), MASKBIT(3:0)); // reg_pd_lane[3:0]
            msWriteByteMask(REG_1125ED, MASKBIT(7:4), MASKBIT(7:4)); // reg_pd_phdac[3:0]
            msWriteByteMask(REG_1125D9, BIT(1), BIT(1)); // reg_atop_gc_ictrl_pd_ov
		}
    }
}


//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_TimerInterrupt()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_tmds_TimerInterrupt(void)
{
    MSCombo_TMDS_PORT_INDEX i;
    WORD  wOffset;
    //= _mhal_tmds_bank_offset(enInputPort);

    for(i = MSCombo_TMDS_PORT_0; i <= MSCombo_TMDS_PORT_MAX; i++)
    {
        if(LinkLockCheckFlag[i][0] == TRUE)
        {
            if(LinkLockCheckCnts[i][0]<LinkLockTH)
            {
                LinkLockCheckCnts[i][0]++;
                wOffset =  _mhal_tmds_bank_offset(i);
                if(msRead2Byte(REG_1607C4 + wOffset) & BIT7)
                    LinkLockCheckCnts[i][1]++;
            }
        }
    }


}


//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_Set_HPD()
//  [Description]:
//                  use to control Hot Plug pin
//  [Arguments]:
//                  enInputPort : combo port
//                  bSetHPD :
//                  1 : keep high
//                  0 : pull low
//  [Return]:
//                  None
//**************************************************************************
void mhal_tmds_Set_HPD(MSCombo_TMDS_PORT_INDEX enInputPort, Bool bSetHPD)
{
    if(bSetHPD)
    {
        if(enInputPort == MSCombo_TMDS_PORT_0) // combo_0
        {
            hw_Set_HdcpHpd();
        }
        else if(enInputPort == MSCombo_TMDS_PORT_1) // combo_1
        {
            hw_Set_HdcpHpd1();
        }
        else if(enInputPort == MSCombo_TMDS_PORT_2) // combo_2
        {
            hw_Set_HdcpHpd2();
        }
        else if(enInputPort == MSCombo_TMDS_PORT_3) // combo_3
        {
            hw_Set_HdcpHpd3();
        }
        else if(enInputPort == MSCombo_TMDS_PORT_4) // combo_4
        {
            hw_Set_HdcpHpd4();
        }
        else if(enInputPort == MSCombo_TMDS_PORT_5) // combo_5
        {
            hw_Set_HdcpHpd5();
        }
    }
    else
    {
        if(enInputPort == MSCombo_TMDS_PORT_0) // combo_0
        {
            hw_Clr_HdcpHpd();
        }
        else if(enInputPort == MSCombo_TMDS_PORT_1) // combo_1
        {
            hw_Clr_HdcpHpd1();
        }
        else if(enInputPort == MSCombo_TMDS_PORT_2) // combo_2
        {
            hw_Clr_HdcpHpd2();
        }
        else if(enInputPort == MSCombo_TMDS_PORT_3) // combo_3
        {
            hw_Clr_HdcpHpd3();
        }
        else if(enInputPort == MSCombo_TMDS_PORT_4) // combo_4
        {
            hw_Clr_HdcpHpd4();
        }
        else if(enInputPort == MSCombo_TMDS_PORT_5) // combo_5
        {
            hw_Clr_HdcpHpd5();
        }
    }

}


//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_reset_status()
//  [Description]:
//                  use to reset packet content and packet status
//  [Arguments]:
//                  enInputPort : combo port
//                  bFlag : to reset packet content
//                  ucReset :
//                  [0] : reset AV mute
//                  [1] : reset color format in AVI (need clock)
//                  [2] : reset pixel repetition in AVI
//                  [3] : reset frame repetition ?
//                  [4] : reset pixel packing phase and color depth in GC packet
//                  [5] : reset deep color FIFO
//                  [6] : reserved
//                  [7] : reset packet received status
//  [Return]:
//                  None
//**************************************************************************
void mhal_tmds_reset_status(MSCombo_TMDS_PORT_INDEX enInputPort, Bool bFlag, BYTE ucReset)
{
    WORD wOffset;

    wOffset = _mhal_tmds_bank_offset(enInputPort);

    if(bFlag)
    {
        msWriteByteMask(REG_160310 + wOffset, 0x02, 0x02); //reset packet content
        msWriteByteMask(REG_160310 + wOffset, 0x00, 0x02);
    }

    msWriteByteMask(REG_160317 + wOffset, ucReset, 0xFF); //reset packet status
    msWriteByteMask(REG_160317 + wOffset, 0, 0xFF);

}

//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_NoInputClk()
//  [Description]:
//                  use to check input clock status
//  [Arguments]:
//                  enInputPort : combo port
//  [Return]:
//                  TRUE : no input clock
//                  FALSE : have input clock
//**************************************************************************
Bool mhal_tmds_NoInputClk(MSCombo_TMDS_PORT_INDEX enInputPort)
{
    WORD wOffset;

    wOffset = _mhal_tmds_bank_offset(enInputPort);

    if(msReadByte(REG_160656 + wOffset) & 0x04) // no input clock
    {
        msWriteByteMask(REG_16065A + wOffset, 0x04, 0x04); //clr
        msWriteByteMask(REG_16065A + wOffset, 0x00, 0x04);
        return 1;
    }
    else
        return 0;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_TimingChg()
//  [Description]:
//                  use to check timing chg
//  [Arguments]:
//                  enInputPort : combo port
//  [Return]:
//                  TRUE : chg
//                  FALSE : no chg
//**************************************************************************
Bool mhal_tmds_TimingChg(MSCombo_TMDS_PORT_INDEX enInputPort)
{
    WORD wOffset;

    wOffset = _mhal_tmds_bank_offset(enInputPort);

    if(msReadByte(REG_160657 + wOffset) & 0x02) // lose lock
    {
        msWriteByteMask(REG_16065B + wOffset, 0x02, 0x02); //clr
        msWriteByteMask(REG_16065B + wOffset, 0x00, 0x02);
        msWriteByteMask(REG_160438 + wOffset, 0x02, 0x02);  //clear HDCP encryption status.
        return 1;
    }
    else
        return 0;
}


//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_GetColor()
//  [Description]:
//                  use to check input color format from AVI packet content info
//  [Arguments]:
//                  enInputPort : combo port
//  [Return]:
//
//                  typedef struct
//                  {
//                      EN_COLOR_FORMAT_TYPE    ucColorType;
//                      EN_COLOR_RANGE_TYPE     ucColorRange;
//                      EN_COLORIMETRY_TYPE     ucColorimetry;
//                      EN_YPBPR_COLORIMETRY_TYPE   ucYuvColorimetry;
//                   } ST_COMBO_COLOR_FORMAT;
//
//**************************************************************************
//BYTE mhal_tmds_GetColor(MSCombo_TMDS_PORT_INDEX enInputPort)
ST_COMBO_COLOR_FORMAT mhal_tmds_GetColor(MSCombo_TMDS_PORT_INDEX enInputPort)
{
    WORD wOffset;
    BYTE Y_value = 0;
    BYTE C_value = 0;
    BYTE Q_value = 0;

    //assign initial value
    ST_COMBO_COLOR_FORMAT ColorInfo = {COMBO_COLOR_FORMAT_DEFAULT, COMBO_COLOR_RANGE_DEFAULT, COMBO_COLORIMETRY_NONE, COMBO_YUV_COLORIMETRY_ITU601};

    wOffset = _mhal_tmds_bank_offset(enInputPort);

    //parsing AVI info frame packet
    Y_value = ((msReadByte(REG_1603A2 + wOffset) & (BIT6|BIT5)) >> 5 ); //[6:5]: RGB or YCbCr
    C_value = ((msReadByte(REG_1603A3 + wOffset) & (BIT7|BIT6)) >> 6 ); //[7:6]: Colorimetry

    switch (Y_value)
    {
        case 0:
            ColorInfo.ucColorType = COMBO_COLOR_FORMAT_RGB;
        break;
        case 1:
            ColorInfo.ucColorType = COMBO_COLOR_FORMAT_YUV_422;
        break;
        case 2:
            ColorInfo.ucColorType = COMBO_COLOR_FORMAT_YUV_444;
        break;
        default:
            ColorInfo.ucColorType = COMBO_COLOR_FORMAT_DEFAULT;
        break;
    }

    if((Y_value == 1) || (Y_value == 2))
    {
        Q_value = ((msReadByte(REG_1603A6 + wOffset) & (BIT7|BIT6)) >> 6 ); //[7:6]: Full or limit (for YCC)

        switch (Q_value)
        {
            case 0:
                ColorInfo.ucColorRange = COMBO_COLOR_RANGE_LIMIT;
            break;
            case 1:
                ColorInfo.ucColorRange = COMBO_COLOR_RANGE_FULL;
            break;
            case 2:
                ColorInfo.ucColorRange = COMBO_COLOR_RANGE_RESERVED;
            break;
            default:
                ColorInfo.ucColorRange = COMBO_COLOR_RANGE_RESERVED;
            break;
        }
    }
    else
    {
        Q_value = ((msReadByte(REG_1603A4 + wOffset) & (BIT3|BIT2)) >> 2 ); //[3:2]: Full or limit (for RGB)

        switch (Q_value)
        {
            case 0:
                ColorInfo.ucColorRange = COMBO_COLOR_RANGE_DEFAULT;
            break;
            case 1:
                ColorInfo.ucColorRange = COMBO_COLOR_RANGE_LIMIT;
            break;
            case 2:
                ColorInfo.ucColorRange = COMBO_COLOR_RANGE_FULL;
            break;
            default:
                ColorInfo.ucColorRange = COMBO_COLOR_RANGE_RESERVED;
            break;
        }
    }

    //extended colorimetry
    if (C_value == 0x03)
    {
        BYTE EC_value = 0;

        EC_value = ((msReadByte(REG_1603A4 + wOffset) & (BIT5|BIT4)) >> 4 );

        switch (EC_value)
        {
            case 0:
                ColorInfo.ucColorimetry = COMBO_COLORIMETRY_xvYCC601;
            break;
            case 1:
                ColorInfo.ucColorimetry = COMBO_COLORIMETRY_xvYCC709;
            break;
            case 2:
                ColorInfo.ucColorimetry = COMBO_COLORIMETRY_sYCC601;
            break;
            case 3:
                ColorInfo.ucColorimetry = COMBO_COLORIMETRY_ADOBEYCC601;
            break;
            default:
                ColorInfo.ucColorimetry = COMBO_COLORIMETRY_NONE;
            break;
        }
    }

    switch(C_value)
    {
        case 0:
            ColorInfo.ucYuvColorimetry = COMBO_YUV_COLORIMETRY_NoData;
            break;
        case 1:
            ColorInfo.ucYuvColorimetry = COMBO_YUV_COLORIMETRY_ITU601;
            break;
        case 2:
            ColorInfo.ucYuvColorimetry = COMBO_YUV_COLORIMETRY_ITU709;
            break;
    }

    return ColorInfo;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_GetITContent()
//  [Description]:
//                  use to obtain AVI packet IT content
//  [Arguments]:
//                  enInputPort : combo port
//  [Return]:
//                  none
//
//**************************************************************************
Bool mhal_tmds_GetITContent(MSCombo_TMDS_PORT_INDEX enInputPort)
{
    WORD wOffset;
    wOffset = _mhal_tmds_bank_offset(enInputPort);
    if(msReadByte(REG_1603A4 + wOffset) & BIT7)
        return 1;
    else
        return 0;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_GetScaling_Info()
//  [Description]:
//                  use to obtain AVI packet picture scaling info.
//  [Arguments]:
//                  enInputPort : combo port
//  [Return]:
//                  none
//
//**************************************************************************
BYTE mhal_tmds_GetScaling_Info(MSCombo_TMDS_PORT_INDEX enInputPort)
{
    WORD wOffset;
    wOffset = _mhal_tmds_bank_offset(enInputPort);

    return (msReadByte(REG_1603A2 + wOffset) & (BIT1|BIT0));
}

//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_GetPacketContent()
//  [Description]:
//                  use to obtain HDMI packet contents
//  [Arguments]:
//                  enInputPort : combo port
//                  ucPacketType: indicate the type of the packet which is going to be extracted
//                  ucPacketLength: the total length to be extracted (byte unit)
//                  pPacketData: pointer, address which store return content
//  [Return]:
//                  none
//
//**************************************************************************
void mhal_tmds_GetPacketContent(MSCombo_TMDS_PORT_INDEX enInputPort, EN_PACKET_DEFINE_TYPE ucPacketType, BYTE ucPacketLength, BYTE *pPacketData)
{
    WORD wOffset;
    BYTE i;
    BYTE DataArray[ucPacketLength];
    BYTE MaxCntLength = 0;

    wOffset = _mhal_tmds_bank_offset(enInputPort);

    //initial value 0
    for (i=0; i < ucPacketLength; i++)
    {
        //*(pPacketData + i) = 0;
        DataArray[i] = 0;
    }

    switch (ucPacketType)
    {
        case COMBO_PACKET_NULL:
        break;

        case COMBO_PACKET_AUDIO_CLOCK_REGENERATION:
            //Hardware already extract packet content, we restore it back to packet format here;
            {
                BYTE SB[7] = {0, 0, 0, 0, 0, 0, 0};

                SB[1] = msReadByte(REG_160308 + wOffset) & 0x0F; //CTS[19:16]
                SB[2] = msReadByte(REG_16030A + wOffset); //CTS[15:8]
                SB[3] = msReadByte(REG_16030B + wOffset); //CTS[7:0]
                SB[4] = (msReadByte(REG_160308 + wOffset) & 0xF0) >> 4; //N[19:16]
                SB[5] = msReadByte(REG_16030C + wOffset); //N[15:8]
                SB[6] = msReadByte(REG_16030D + wOffset); //N[7:0]

                MaxCntLength = (ucPacketLength > 7 ? 7 : ucPacketLength);

                for ( i=0; i < MaxCntLength; i++ )
                {
                    DataArray[i] = SB[i];
                }
            }
        break;

        case COMBO_PACKET_AUDIO_SAMPLE:
        break;

        case COMBO_PACKET_GENERAL_CONTROL:
            MaxCntLength = (ucPacketLength > 2 ? 2 : ucPacketLength);

            for ( i=0; i < MaxCntLength; i++ )
            {
                DataArray[i] = msReadByte(REG_160380 + wOffset + i);
            }

        break;

        case COMBO_PACKET_ACP_PACKET:
            MaxCntLength = (ucPacketLength > 16 ? 16 : ucPacketLength);

            for ( i=0; i < MaxCntLength; i++ )
            {
                DataArray[i] = msReadByte(REG_160480 + wOffset + i);
            }

        break;

        //isrc1: byte0~15; isrc2: byte 16~31
        case COMBO_PACKET_ISRC1_PACKET:
            MaxCntLength = (ucPacketLength > 16 ? 16 : ucPacketLength);

            for ( i=0; i < MaxCntLength; i++ )
            {
                    DataArray[i] = msReadByte(REG_160494 + wOffset + i);
            }
        break;

        case COMBO_PACKET_ISRC2_PACKET:
            MaxCntLength = (ucPacketLength > 16 ? 16 : ucPacketLength);

            for ( i=0; i < MaxCntLength; i++ )
            {
                    DataArray[i] = msReadByte(REG_1604A8 + wOffset + i);
            }

        break;

        case COMBO_PACKET_ONE_BIT_AUDIO_SAMPLE:
        break;
        case COMBO_PACKET_DST_AUDIO:
        break;
        case COMBO_PACKET_HBR_AUDIO_STREAM:
        break;

        case COMBO_PACKET_GAMUT_METADATA:
            MaxCntLength = (ucPacketLength > 21 ? 21 : ucPacketLength);

            for ( i=0; i < MaxCntLength; i++ )
            {
                DataArray[i] = msReadByte(REG_1604E4 + wOffset + i);
            }

        break;

        case COMBO_PACKET_VS_INFOFRAME:
            MaxCntLength = (ucPacketLength > 29 ? 29 : ucPacketLength);

            for ( i=0; i < MaxCntLength; i++ )
            {
                DataArray[i] = msReadByte(REG_160382 + wOffset + i);
            }

        break;

        case COMBO_PACKET_AVI_INFOFRAME:
            MaxCntLength = (ucPacketLength > 13 ? 13 : ucPacketLength);

            for ( i=0; i < MaxCntLength; i++ )
            {
                DataArray[i] = msReadByte(REG_1603A2 + wOffset + i);
            }

        break;

        case COMBO_PACKET_SPD_INFOFRAME:
            MaxCntLength = (ucPacketLength > 25 ? 25 : ucPacketLength);

            for ( i=0; i < MaxCntLength; i++ )
            {
                DataArray[i] = msReadByte(REG_1604B8 + wOffset + i);
            }

        break;

        case COMBO_PACKET_AUD_INFOFRAME:
            MaxCntLength = (ucPacketLength > 5 ? 5 : ucPacketLength);

            for ( i=0; i < MaxCntLength; i++ )
            {
                DataArray[i] = msReadByte(REG_1604D4 + wOffset + i);
            }

        break;

        case COMBO_PACKET_MPEG_INFOFRAME:
            MaxCntLength = (ucPacketLength > 5 ? 5 : ucPacketLength);

            for ( i=0; i < MaxCntLength; i++ )
            {
                DataArray[i] = msReadByte(REG_1604DC + wOffset + i);
            }
        break;

        case COMBO_PACKET_DP_MISC:
        break;
        default:
        break;
    }

    memcpy(pPacketData, DataArray, MaxCntLength);

}

//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_DetectCablePlug()
//  [Description]:
//                  detect if HDMI/DVI cable is attached.
//  [Arguments]:
//                  ucInputPort
//                  bFlag : TRUE -> SAR detect, FALSE -> GPIO detect
//  [Return]:
//                  Bool
//
//**************************************************************************
BYTE mhal_tmds_DetectCablePlug(MSCombo_TMDS_PORT_INDEX enInputPort, Bool bFlag)
{
    Bool bCable5VDetect = FALSE;
    BYTE ucCableDetect = TMDS_CABLE_DETECT_NO_INPUT;
    BYTE ucCableSARDetect = 0;

    switch(enInputPort)
    {
        case MSCombo_TMDS_PORT_0:
            if(bFlag)
            {
                ucCableSARDetect = TMDS_CABLE_SAR_DETECT_PORT0;
            }
            else
            {
                bCable5VDetect = TMDS_CABLE_5V_DETECT_PORT0;
            }

            break;

        case MSCombo_TMDS_PORT_1:
            if(bFlag)
            {
                ucCableSARDetect = TMDS_CABLE_SAR_DETECT_PORT1;
            }
            else
            {
                bCable5VDetect = TMDS_CABLE_5V_DETECT_PORT1;
            }

            break;

        case MSCombo_TMDS_PORT_2:
            if(bFlag)
            {
                ucCableSARDetect = TMDS_CABLE_SAR_DETECT_PORT2;
            }
            else
            {
                bCable5VDetect = TMDS_CABLE_5V_DETECT_PORT2;
            }

            break;

        case MSCombo_TMDS_PORT_3:
            if(bFlag)
            {
                ucCableSARDetect = TMDS_CABLE_SAR_DETECT_PORT3;
            }
            else
            {
                bCable5VDetect = TMDS_CABLE_5V_DETECT_PORT3;
            }

            break;

        case MSCombo_TMDS_PORT_4:
            if(bFlag)
            {
                ucCableSARDetect = TMDS_CABLE_SAR_DETECT_PORT4;
            }
            else
            {
                bCable5VDetect = TMDS_CABLE_5V_DETECT_PORT4;
            }

            break;

        case MSCombo_TMDS_PORT_5:
            if(bFlag)
            {
                ucCableSARDetect = TMDS_CABLE_SAR_DETECT_PORT5;
            }
            else
            {
                bCable5VDetect = TMDS_CABLE_5V_DETECT_PORT5;
            }

            break;
    };

    if(bFlag)
    {
        if(ucCableSARDetect < 0x0A)
        {
            ucCableDetect = TMDS_CABLE_DETECT_HDMI;    // 10 * 3.3 / 255 = 0.13v
        }
        else if(ucCableSARDetect > 0xA0)
        {
            ucCableDetect = TMDS_CABLE_DETECT_MHL;
        }
    }
    else
    {
        if(bCable5VDetect)
        {
            ucCableDetect = TMDS_CABLE_DETECT_HDMI;
        }
    }

    return ucCableDetect;
}


//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_fix_audio_clock()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_tmds_fix_audio_clock(MScombo_TMDS_AUD_FREQ_INDEX enAudioFS, Bool bFlag)
{
    Bool btemp = 0;
    if(enAudioFS <= MScombo_TMDS_AUD_FREQ_192K)
    {
        btemp = bFlag;
        TMDS_PRINT("0x%x, not support fix audio clock\r\n", enAudioFS);
    }

}


//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_GetAudioFreq()
//  [Description]:
//                  calculate current HDMI audio frequence by CTS/N value;
//                  formula:
//                      freq = (F_tmds * N) / (128 * CTS);
//  [Arguments]:
//                  enInputPort
//  [Return]:
//                  MScombo_TMDS_AUDFREQ_INDEX
//
//**************************************************************************
MScombo_TMDS_AUD_FREQ_INDEX mhal_tmds_GetAudioFreq(MSCombo_TMDS_PORT_INDEX enInputPort)
{
    WORD wOffset;
    DWORD dwCTSValue, dwNValue, dwTMDSClkValCnt;
    DWORD dwAudFreqValue = MScombo_TMDS_AUD_FREQ_ERROR;

    wOffset = _mhal_tmds_phy_offset(enInputPort);

    //Get current TMDS clock rate
    dwTMDSClkValCnt = mhal_tmds_GetClockRate(enInputPort)*10;//msRead2Byte(REG_112078 + wOffset)* 12000 / 128;

    wOffset = _mhal_tmds_bank_offset(enInputPort);
    //load current CTS/N value first
    msWriteByteMask(REG_1603EF + wOffset, 0x40, 0x40);

    dwNValue = (msRead2Byte(REG_16037C + wOffset)) | (((msReadByte(REG_16037A + wOffset) & 0xF0) >> 4) << 16);
    dwCTSValue = (msRead2Byte(REG_160378 + wOffset)) | ((msReadByte(REG_16037A + wOffset) & 0x0F) << 16);

    if ((dwNValue < 0x10) || (dwCTSValue < 0x10))
        return MScombo_TMDS_AUD_FREQ_ERROR;   //error

    dwAudFreqValue = (dwTMDSClkValCnt * dwNValue) / (128 * dwCTSValue);

    #if 0
    {
        static WORD printcnt = 0;
        if ((printcnt % 10000 == 0) && (printcnt != 0))
        {
            printData("[HDMI] tmds clk = %d\r", dwTMDSClkValCnt);
            printData("[HDMI] N value = %d\r", dwNValue);
            printData("[HDMI] CTS value = %d\r", dwCTSValue);
            printData("[HDMI] audio freq = %d\r", dwAudFreqValue);
            printcnt -= 10000;
        }
        else
            printcnt++;
    }
    #endif

    if (abs(dwAudFreqValue - 32) < 4)
        dwAudFreqValue = MScombo_TMDS_AUD_FREQ_32K;

    else if (abs(dwAudFreqValue - 44) < 4)
        dwAudFreqValue = MScombo_TMDS_AUD_FREQ_44K;

    else if (abs(dwAudFreqValue - 48) < 4)
        dwAudFreqValue = MScombo_TMDS_AUD_FREQ_48K;

    else if (abs(dwAudFreqValue - 88) < 4)
        dwAudFreqValue = MScombo_TMDS_AUD_FREQ_88K;

    else if (abs(dwAudFreqValue - 96) < 4)
        dwAudFreqValue = MScombo_TMDS_AUD_FREQ_96K;

    else if (abs(dwAudFreqValue - 176) < 4)
        dwAudFreqValue = MScombo_TMDS_AUD_FREQ_176K;

    else if (abs(dwAudFreqValue - 192) < 4)
        dwAudFreqValue = MScombo_TMDS_AUD_FREQ_192K;

    else
        dwAudFreqValue = MScombo_TMDS_AUD_FREQ_ERROR; //error

    return (MScombo_TMDS_AUD_FREQ_INDEX)dwAudFreqValue;

}

//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_IsAudioFmtPCM()
//  [Description]:
//                  check audio format information, is it PCM or non-PCM?
//  [Arguments]:
//                  enInputPort
//  [Return]:
//                  MSCombo_TMDS_AUDIO_FORMAT:
//                      MSCombo_AUDIO_FMT_NON_PCM   = 0,
//                      MSCombo_AUDIO_FMT_PCM       = 1,
//                      MSCombo_AUDIO_FMT_UNKNOWN   = 2,
//
//**************************************************************************
MSCombo_TMDS_AUDIO_FORMAT mhal_tmds_IsAudioFmtPCM(MSCombo_TMDS_PORT_INDEX enInputPort)
{
    WORD wOffset;
    MSCombo_TMDS_AUDIO_FORMAT enAudFmt;

    wOffset = _mhal_tmds_bank_offset(enInputPort);

    if ( msReadByte(REG_1603CA + wOffset) & _BIT6 )
        enAudFmt = MSCombo_AUDIO_FMT_NON_PCM;
    else
        enAudFmt = MSCombo_AUDIO_FMT_PCM;

    return enAudFmt;
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_tmds_CheckClockValid()
//  [Description]:
//                  check "clock valid" status
//  [Arguments]:
//                  enInputPort
//  [Return]:
//                  Bool
//**************************************************************************
Bool mhal_tmds_CheckClockValid(MSCombo_TMDS_PORT_INDEX enInputPort)
{
    //WORD wTimeoutCnt = 500;
    WORD wOffset;

    wOffset = _mhal_tmds_bank_offset(enInputPort);

    if (msReadByte(REG_160657 + wOffset) & (0x20))
        return TRUE;
    else
        return FALSE;
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_tmds_CheckClockLock()
//  [Description]:
//                  check "Interskew align lose lock" status
//  [Arguments]:
//                  enInputPort
//  [Return]:
//                  Bool
//**************************************************************************
Bool mhal_tmds_CheckClockLock(MSCombo_TMDS_PORT_INDEX enInputPort)
{
    //WORD wTimeoutCnt = 500;
    WORD wOffset;

    wOffset = _mhal_tmds_bank_offset(enInputPort);

    if (msReadByte(REG_160657 + wOffset) & (0x02))
    {
        msWriteByteMask(REG_16065B + wOffset, 0x02, 0x02);
        msWriteByteMask(REG_16065B + wOffset, 0x00, 0x02);
        return TRUE;
    }
    else
        return FALSE;

}

//**************************************************************************
//  [Function Name]:
//                  _mhal_tmds_SetCbusPullDown100K()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
TMDS_HDCP_STATE mhal_tmds_CheckHDCPState(MSCombo_TMDS_PORT_INDEX enPortSelect)
{
    TMDS_HDCP_STATE temp = TMDS_HDCP_NO_ENCRYPTION;
    WORD wOffset;

    wOffset = _mhal_tmds_bank_offset(enPortSelect);

    if(msReadByte(REG_160438 + wOffset) & 0x02) // encryption enable
        temp = TMDS_HDCP_1_4;

	return temp;
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_tmds_ClearClockStatus()
//  [Description]:
//                  clear "Interskew align lose lock" and "clock valid" status
//  [Arguments]:
//                  enInputPort
//  [Return]:
//                  void
//**************************************************************************
void mhal_tmds_ClearClockStatus(MSCombo_TMDS_PORT_INDEX enInputPort)
{
    //clear clock valid and clock big change status
    WORD wOffset;

    wOffset = _mhal_tmds_bank_offset(enInputPort);

    //clear clock valid
    msWriteByteMask(REG_16065B + wOffset, 0x20, 0x20);
    msWriteByteMask(REG_16065B + wOffset, 0x00, 0x20);

    TMDS_PRINT("clear clock status\r\n");
}

//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_GetColorDepthInfo()
//  [Description]:
//                  return color depth information, ex: 8bit, 10bit, or 12 bit by parsing GCP packet content
//  [Arguments]:
//                  enInputPort
//  [Return]:
//                  MSCombo_TMDS_COLOR_DEPTH_FORMAT
//              8_BIT  = 0,
//              10_BIT = 1,
//              12_BIT = 2,
//              16_BIT = 3,
//              UNKNOWN = 4,
//**************************************************************************
MSCombo_TMDS_COLOR_DEPTH_FORMAT mhal_tmds_GetColorDepthInfo(MSCombo_TMDS_PORT_INDEX enInputPort)
{
    WORD wOffset = 0;
    BYTE ucTmpValue = 0;

    wOffset = _mhal_tmds_bank_offset(enInputPort);

    ucTmpValue = (msReadByte(REG_160381 + wOffset) & 0x0F); //GCP packeting phase bit

    switch (ucTmpValue)
    {
        case 0:
        case 4:
            return MSCombo_TMDS_COLOR_DEPTH_8_BIT;
        break;
        case 5:
            return MSCombo_TMDS_COLOR_DEPTH_10_BIT;
        break;
        case 6:
            return MSCombo_TMDS_COLOR_DEPTH_12_BIT;
        break;
        case 7:
            return MSCombo_TMDS_COLOR_DEPTH_16_BIT;
        break;
        default:
            return MSCombo_TMDS_COLOR_DEPTH_UNKNOWN;
        break;
    }

}


//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_GetPixelRepetitionInfo()
//  [Description]:
//                  return pxiel repetition info
//  [Arguments]:
//                  enInputPort
//  [Return]:
//                  MSCombo_TMDS_PIXEL_REPETITION
//              no = 0,
//              2x = 1,
//              3x = 2,
//              4x = 3,
//              5x = 4,
//              6x = 5,
//              7x = 6,
//              8x = 7,
//              9x = 8,
//             10x = 9,
//**************************************************************************
MSCombo_TMDS_PIXEL_REPETITION mhal_tmds_GetPixelRepetitionInfo(MSCombo_TMDS_PORT_INDEX enInputPort)
{
    WORD wOffset = 0;
    BYTE ucTmpValue = 0;

    wOffset = _mhal_tmds_bank_offset(enInputPort);

    ucTmpValue = (msReadByte(REG_1603A6 + wOffset) & 0x0F); //PR

    switch (ucTmpValue)
    {
        case 0:
            return MSCombo_TMDS_N0_PIX_REP;
        break;
        case 1:
            return MSCombo_TMDS_2x_PIX_REP;
        break;
        case 2:
            return MSCombo_TMDS_3x_PIX_REP;
        break;
        case 3:
            return MSCombo_TMDS_4x_PIX_REP;
        break;
        case 4:
            return MSCombo_TMDS_5x_PIX_REP;
        break;
        case 5:
            return MSCombo_TMDS_6x_PIX_REP;
        break;
        case 6:
            return MSCombo_TMDS_7x_PIX_REP;
        break;
        case 7:
            return MSCombo_TMDS_8x_PIX_REP;
        break;
        case 8:
            return MSCombo_TMDS_9x_PIX_REP;
        break;
        case 9:
            return MSCombo_TMDS_10x_PIX_REP;
        break;
        default:
            return MSCombo_TMDS_RESERVED_PIX_REP;
        break;
    }

}


//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_GetClockRate()
//  [Description]:
//                  return clcok rate
//  [Arguments]:
//                  enInputPort
//  [Return]:
//
//**************************************************************************
WORD mhal_tmds_GetClockRate(MSCombo_TMDS_PORT_INDEX enInputPort)
{
    WORD wOffset, dwTMDSClkValCnt;

    if(mhal_tmds_Clock_Stable(enInputPort))
    {
        wOffset = _mhal_tmds_phy_offset(enInputPort);

        //Get current TMDS clock rate
        dwTMDSClkValCnt = msRead2Byte(REG_112078 + wOffset)* 1200 / 128;

        //MHL PP mode: we have to double TMDS clock value;
        wOffset = _mhal_tmds_bank_offset(enInputPort);

        if (msReadByte(REG_160504 + wOffset) & BIT1) //[1]: MHL PP mode enable
            dwTMDSClkValCnt = dwTMDSClkValCnt << 1;

        return dwTMDSClkValCnt;
    }
    else
        return 0;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_GetIsOver3G()
//  [Description]:
//                  over/under 3.4G determined in mhal_tmds_ISR()
//  [Arguments]:
//                  [MSCombo_TMDS_PORT_INDEX]PortIdx
//  [Return]:
//                  TRUE: Over 3.4G
//                  FALSE: Under 3.4G
//**************************************************************************
Bool mhal_tmds_GetIsOver3G(MSCombo_TMDS_PORT_INDEX enInputPort)
{
    if(enInputPort <= MSCombo_TMDS_PORT_MAX)
    {
        TMDS_PRINT("Port: 0x%x, not support HDMI 2.0\r\n", enInputPort);
    }

    return FALSE;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_SetIsOver3G()
//  [Description]:
//                  over/under 3.4G determined in mhal_tmds_ISR()
//  [Arguments]:
//                  [MSCombo_TMDS_PORT_INDEX]PortIdx
//  [Return]:
//                  TRUE: Over 3.4G
//                  FALSE: Under 3.4G
//**************************************************************************
void mhal_tmds_SetIsOver3G(MSCombo_TMDS_PORT_INDEX enInputPort, Bool Flag)
{
    Bool temp;
    if(enInputPort <= MSCombo_TMDS_PORT_MAX)
    {
        temp = Flag;
        TMDS_PRINT("Port: 0x%x, not support HDMI 2.0\r\n", enInputPort);
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_GetIsSplitterDone()
//  [Description]:
//                  Splitter setting done or not
//  [Arguments]:
//                  [MSCombo_TMDS_PORT_INDEX]PortIdx
//  [Return]:
//                  TRUE: Splitter Setting Done
//                  FALSE: Splitter not setting done
//**************************************************************************
Bool mhal_tmds_GetIsSplitterDone(MSCombo_TMDS_PORT_INDEX enInputPort)
{
    if(enInputPort <= MSCombo_TMDS_PORT_MAX)
    {
        TMDS_PRINT("Port: 0x%x, not support HDMI 2.0\r\n", enInputPort);
    }

    return TRUE;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_SetIsSplitterDone()
//  [Description]:
//                  Splitter setting done or not
//  [Arguments]:
//                  [MSCombo_TMDS_PORT_INDEX]PortIdx
//  [Return]:
//                  TRUE: Splitter Setting Done
//                  FALSE: Splitter not setting done
//**************************************************************************
void mhal_tmds_SetIsSplitterDone(MSCombo_TMDS_PORT_INDEX enInputPort, Bool Flag)
{
    Bool temp;
    if(enInputPort <= MSCombo_TMDS_PORT_MAX)
    {
        temp = Flag;
        TMDS_PRINT("Port: 0x%x, not support HDMI 2.0\r\n", enInputPort);
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_AutoEQCntRst()
//  [Description]:
//                  AutoEQ count reset or not
//  [Arguments]:
//                  [MSCombo_TMDS_PORT_INDEX]PortIdx
//  [Return]:
//                  TRUE: do autoEQ count reset
//                  FALSE: not do autoEQ count reset
//**************************************************************************
Bool mhal_tmds_GetAutoEQCntRst(MSCombo_TMDS_PORT_INDEX enInputPort)
{
    if(enInputPort <= MSCombo_TMDS_PORT_MAX)
    {
        TMDS_PRINT("Port: 0x%x, not support HDMI 2.0\r\n", enInputPort);
    }

    return FALSE;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_SetAutoEQCntRst()
//  [Description]:
//                  AutoEQ count reset or not
//  [Arguments]:
//                  [MSCombo_TMDS_PORT_INDEX]PortIdx
//  [Return]:
//                  TRUE: do autoEQ count reset
//                  FALSE: not do autoEQ count reset
//**************************************************************************
void mhal_tmds_SetAutoEQCntRst(MSCombo_TMDS_PORT_INDEX enInputPort, Bool bFlag)
{
    Bool temp;
    if(enInputPort <= MSCombo_TMDS_PORT_MAX)
    {
        temp = bFlag;
        TMDS_PRINT("Port: 0x%x, not support HDMI 2.0\r\n", enInputPort);
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_AutoEQDone()
//  [Description]:
//                  AutoEQ done or not
//  [Arguments]:
//                  [MSCombo_TMDS_PORT_INDEX]PortIdx
//  [Return]:
//                  TRUE: autoEQ done
//                  FALSE: autoEQ not done
//**************************************************************************
Bool mhal_tmds_AutoEQDone(MSCombo_TMDS_PORT_INDEX enInputPort)
{
    if(enInputPort <= MSCombo_TMDS_PORT_MAX)
    {
        TMDS_PRINT("Port: 0x%x, not support HDMI 2.0\r\n", enInputPort);
    }

    return TRUE;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_TriggerAutoEQ()
//  [Description]:
//                  Do autoEQ or not
//  [Arguments]:
//                  [MSCombo_TMDS_PORT_INDEX]PortIdx
//                  bEnable: TRUE: do autoEQ, FALSE: not do autoEQ
//  [Return]:
//
//**************************************************************************
void mhal_tmds_TriggerAutoEQ(MSCombo_TMDS_PORT_INDEX enInputPort, Bool bEnable, Bool bOverWrite)
{
    Bool temp;
    if(enInputPort <= MSCombo_TMDS_PORT_MAX)
    {
        temp = bEnable;
        TMDS_PRINT("Port: 0x%x, not support HDMI 2.0\r\n", enInputPort);

        bOverWrite = FALSE;
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_CheckAutoEQ()
//  [Description]:
//                  check AutoEQ flow
//  [Arguments]:
//                  [MSCombo_TMDS_PORT_INDEX]PortIdx
//                  bFlag: true -> no timeout, false -> timeout
//  [Return]:
//**************************************************************************
void mhal_tmds_CheckAutoEQ(MSCombo_TMDS_PORT_INDEX enInputPort, Bool bFlag)
{
    Bool temp;
    if(enInputPort <= MSCombo_TMDS_PORT_MAX)
    {
        temp = bFlag;
        TMDS_PRINT("Port: 0x%x, not support HDMI 2.0\r\n", enInputPort);
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_ISR()
//  [Description]:
//                  interrupt handler for tmds CLK_STABLE
//  [Arguments]:
//                  [MSCombo_TMDS_PORT_INDEX]PortIdx
//  [Return]:
//                  void
//
//**************************************************************************
void mhal_tmds_ISR(MSCombo_TMDS_PORT_INDEX enInputPort, Bool bMHLSupport)
{
    WORD wOffset;
    WORD wTMDSClkRate = 0;

    // turn on mask
    wOffset = _mhal_tmds_bank_offset(enInputPort);
    //msWrite2Byte(REG_16065C + wOffset, 0xFFFF); //combo irq mask

    if (msRead2Byte(REG_160656 + wOffset) & BIT13) //[D]: clock valid
    {
        //clear event status;
        msWrite2ByteMask(REG_16065A + wOffset, BIT13, BIT13);
        msWrite2ByteMask(REG_16065A + wOffset, 0x0000, BIT13);

        wOffset = _mhal_tmds_phy_offset(enInputPort);

        wTMDSClkRate = (msRead2Byte(REG_112078 + wOffset)* 12 / 128);

        if ((wPreviousTiming == wTMDSClkRate) && (bPreviousPort == enInputPort))
        {
            //wOffset = _mhal_tmds_bank_offset(enInputPort);
            //msWrite2Byte(REG_16065C + wOffset, 0xDFFF); //combo irq mask
            return; //no timing change or DC on/off
        }
        wPreviousTiming = wTMDSClkRate;
        bPreviousPort = enInputPort;

        LinkLockCheckFlag[enInputPort][0] = FALSE;
        LinkLockCheckFlag[enInputPort][1] = FALSE;
        LinkLockCheckCnts[enInputPort][0] = 0;
        LinkLockCheckCnts[enInputPort][1] = 0;

        if((!bMHLSupport) || (mhal_tmds_DetectCablePlug(enInputPort, bMHLSupport) <= 1)) //HDMI cable
        {
            if (wTMDSClkRate >= 270)
            {
                //printMsg("eq= 12");

				//patch for SEC issue, adjust iCtrl value;
				msWriteByteMask(REG_1120DF + wOffset, BIT7, BIT7); //enable over-write
				msWriteByteMask(REG_1120B6 + wOffset, 0x80, 0xF0); //over write value from 4 to 8

                msWrite2Byte(REG_1120E6 + wOffset, (COMBO_HDMI_270_340MHZ_EQ_VALUE<<10)|(COMBO_HDMI_270_340MHZ_EQ_VALUE<<5)|COMBO_HDMI_270_340MHZ_EQ_VALUE);
                msWriteByteMask(REG_1120E8 + wOffset, COMBO_HDMI_270_340MHZ_EQ_VALUE, 0x1F);
            }
            else
            {
            	msWriteByteMask(REG_1120DF + wOffset, BIT7, BIT7); //enable over-write
				msWriteByteMask(REG_1120B6 + wOffset, 0x40, 0xF0); //over write value from 4 to 8
				msWriteByteMask(REG_1120DF + wOffset, 0x00, BIT7); //disable over-write

                //printMsg("eq= 24");
                msWrite2Byte(REG_1120E6 + wOffset, (COMBO_HDMI_25_270MHZ_EQ_VALUE<<10)|(COMBO_HDMI_25_270MHZ_EQ_VALUE<<5)|COMBO_HDMI_25_270MHZ_EQ_VALUE);
                msWriteByteMask(REG_1120E8 + wOffset, COMBO_HDMI_25_270MHZ_EQ_VALUE, 0x1F);
            }
        }
    }

    // turn off mask
    //wOffset = _mhal_tmds_bank_offset(enInputPort);
    //msWrite2Byte(REG_16065C + wOffset, 0xDFFF); //combo irq mask
}


//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_CheckScrambleStatus()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_tmds_CheckScrambleStatus(MSCombo_TMDS_PORT_INDEX enInputPort, Bool bflag)
{
    Bool btemp = 0;
    if(enInputPort <= MSCombo_TMDS_PORT_MAX)
    {
        btemp = bflag;
        TMDS_PRINT("Port: 0x%x, not support HDMI 2.0\r\n", enInputPort);
    }
}


//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_GetNoInputFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mhal_tmds_GetNoInputFlag(MSCombo_TMDS_PORT_INDEX enInputPort)
{
    Bool bNoInputFlag = FALSE;

    switch(enInputPort)
    {
        case MSCombo_TMDS_PORT_0:
            bNoInputFlag = ((msReadByte(REG_160656) &BIT3)? TRUE: FALSE); // REG_COMBO_TOP0_2B[3], No input status port0

            if(bNoInputFlag)
            {
                msWriteByteMask(REG_16065A, BIT3, BIT3); // REG_COMBO_TOP0_2D[3], No input clear port0
                msWriteByteMask(REG_16065A, 0, BIT3); // REG_COMBO_TOP0_2D[3], No input clear port0
            }

            break;

        case MSCombo_TMDS_PORT_1:
            bNoInputFlag = ((msReadByte(REG_160C56) &BIT3)? TRUE: FALSE); // REG_COMBO_TOP1_2B[3], No input status port1

            if(bNoInputFlag)
            {
                msWriteByteMask(REG_160C5A, BIT3, BIT3); // REG_COMBO_TOP1_2D[3], No input clear port1
                msWriteByteMask(REG_160C5A, 0, BIT3); // REG_COMBO_TOP1_2D[3], No input clear port1
            }

            break;

        case MSCombo_TMDS_PORT_2:
            bNoInputFlag = ((msReadByte(REG_161456) &BIT3)? TRUE: FALSE); // REG_COMBO_TOP2_2B[3], No input status port2

            if(bNoInputFlag)
            {
                msWriteByteMask(REG_16145A, BIT3, BIT3); // REG_COMBO_TOP2_2D[3], No input clear port2
                msWriteByteMask(REG_16145A, 0, BIT3); // REG_COMBO_TOP2_2D[3], No input clear port2
            }

            break;

        case MSCombo_TMDS_PORT_3:
            bNoInputFlag = ((msReadByte(REG_161C56) &BIT3)? TRUE: FALSE); // REG_COMBO_TOP3_2B[3], No input status port3

            if(bNoInputFlag)
            {
                msWriteByteMask(REG_161C5A, BIT3, BIT3); // REG_COMBO_TOP3_2D[3], No input clear port3
                msWriteByteMask(REG_161C5A, 0, BIT3); // REG_COMBO_TOP3_2D[3], No input clear port3
            }

            break;

        case MSCombo_TMDS_PORT_4:
            bNoInputFlag = ((msReadByte(REG_162456) &BIT3)? TRUE: FALSE); // REG_COMBO_TOP4_2B[3], No input status port4

            if(bNoInputFlag)
            {
                msWriteByteMask(REG_16245A, BIT3, BIT3); // REG_COMBO_TOP4_2D[3], No input clear port4
                msWriteByteMask(REG_16245A, 0, BIT3); // REG_COMBO_TOP4_2D[3], No input clear port4
            }

            break;

        case MSCombo_TMDS_PORT_5:
            bNoInputFlag = ((msReadByte(REG_162C56) &BIT3)? TRUE: FALSE); // REG_COMBO_TOP5_2B[3], No input status port5

            if(bNoInputFlag)
            {
                msWriteByteMask(REG_162C5A, BIT3, BIT3); // REG_COMBO_TOP5_2D[3], No input clear port5
                msWriteByteMask(REG_162C5A, 0, BIT3); // REG_COMBO_TOP5_2D[3], No input clear port5
            }

            break;

        default:

            break;
    };

    return bNoInputFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_Over3G_420Setting()
//  [Description]:
//                  4k2k@60Hz 420 deep color setting
//  [Arguments]:
//                  [MSCombo_TMDS_PORT_INDEX]PortIdx
//  [Return]:
//
//**************************************************************************
void mhal_tmds_Over3G_420Setting(MSCombo_TMDS_PORT_INDEX enInputPort, Bool bYUV420Flag)
{
    Bool btemp = 0;
    if(enInputPort <= MSCombo_TMDS_PORT_MAX)
    {
        btemp = bYUV420Flag;
        TMDS_PRINT("Port: 0x%x, not support HDMI 2.0\r\n", enInputPort);
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_HDE_value()
//  [Description]:
//                  Get HDE value
//  [Arguments]:
//                  [MSCombo_TMDS_PORT_INDEX]PortIdx
//  [Return]:
//                  HDE_value
//**************************************************************************
WORD mhal_tmds_HDE_value(MSCombo_TMDS_PORT_INDEX enInputPort)
{
    WORD HDE_value = 0;
    if(enInputPort <= MSCombo_TMDS_PORT_MAX)
    {
        TMDS_PRINT("Port: 0x%x, not support HDMI 2.0\r\n", enInputPort);
    }
    return HDE_value;
}


//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_HDMI20Setting()
//  [Description]:
//                  2p/1p setting
//  [Arguments]:
//                  [MSCombo_TMDS_PORT_INDEX]PortIdx
//                  bFlag : TRUE -> 2p, FALSE -> 1p
//  [Return]:
//
//**************************************************************************
void mhal_tmds_HDMI20Setting(MSCombo_TMDS_PORT_INDEX enInputPort, Bool bFlag)
{
    Bool btemp = 0;
    if(enInputPort <= MSCombo_TMDS_PORT_MAX)
    {
        btemp = bFlag;
        TMDS_PRINT("Port: 0x%x, not support HDMI 2.0\r\n", enInputPort);
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_SetMonochromeMode()
//  [Description]:
//                  used for DVI mono mode setting
//  [Arguments]:
//                  enInputPort
//                  bEnable : True -> enable mono mode, False -> disable mono mode and change to dual link setting for next timing detect
//                  bFlag : True ->  dual link mode. False -> single mode.
//                  ucBits : mono bit
//  [Return]:
//                  None
//**************************************************************************
void mhal_tmds_SetMonochromeMode(MSCombo_TMDS_PORT_INDEX enInputPort, Bool bEnable, Bool bFlag, MSCombo_TMDS_MONO_BIT ucBits)
{
    WORD wOffset;

    wOffset = _mhal_tmds_bank_offset(enInputPort);

    if(bEnable)
    {
        msWriteByteMask(REG_160632 + wOffset, bFlag ? 0x10 : 0x00, 0x1C); //[4]:enable of dual link FIFO, [3:2] : deep color define
        switch(ucBits)
        {
            case MSCombo_TMDS_MONO_8_BIT:
                msWriteByteMask(REG_160633 + wOffset, 0x00, 0x0F); //mono type select
            break;
            case MSCombo_TMDS_MONO_10_BIT:
                msWriteByteMask(REG_160633 + wOffset, 0x01, 0x0F); //mono type select
            break;
            case MSCombo_TMDS_MONO_12_BIT:
                msWriteByteMask(REG_160633 + wOffset, 0x02, 0x0F); //mono type select
            break;
            default:
                msWriteByteMask(REG_160633 + wOffset, 0x03, 0x0F); //default value
            break;
    }
    }
    else
    {
        msWriteByteMask(REG_160632 + wOffset, 0x10, 0x1C); //[4]:enable of dual link FIFO, [3:2] : deep color define
        msWriteByteMask(REG_160633 + wOffset, 0x03, 0x0F); //default value
    }
}


//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_DVIDeepColorSetting()
//  [Description]:
//                  used for DVI deep color mode setting
//  [Arguments]:
//                  enInputPort
//                  bEnable : True : enable deep color mode, False : disable deep color mode and change to dual link setting for next timing detect
//                  ucBits : color depth
//  [Return]:
//                  None
//**************************************************************************
void mhal_tmds_DVIDeepColorSetting(MSCombo_TMDS_PORT_INDEX enInputPort, Bool bEnable, MSCombo_TMDS_DVI_DEEP_COLOR ucBits)
{
    WORD wOffset;

    wOffset = _mhal_tmds_bank_offset(enInputPort);
    if(bEnable)
    {
        switch(ucBits)
        {
            case MSCombo_TMDS_DVI_DEEP_COLOR_8_BIT:
                msWriteByteMask(REG_160632 + wOffset, 0x00, 0x1C);
            break;
            case MSCombo_TMDS_DVI_DEEP_COLOR_10_BIT:
                msWriteByteMask(REG_160632 + wOffset, 0x14, 0x1C);
            break;
            case MSCombo_TMDS_DVI_DEEP_COLOR_12_BIT:
                msWriteByteMask(REG_160632 + wOffset, 0x18, 0x1C);
            break;
            default:
                msWriteByteMask(REG_160632 + wOffset, 0x10, 0x1C);
            break;
        }

        wOffset = _mhal_tmds_phy_offset(enInputPort);
        if(ucBits>MSCombo_TMDS_DVI_DEEP_COLOR_8_BIT)
            msWriteByteMask(REG_1120D6 + wOffset, 0x01, 0x05); //Main:[0]: Enable dual clk; [2]: 2x Pixel clk
        else
            msWriteByteMask(REG_1120D6 + wOffset, 0x05, 0x05); //Main:[0]: Enable dual clk; [2]: 2x Pixel clk
    }
    else
    {
        msWriteByteMask(REG_160632 + wOffset, 0x10, 0x1C);
        wOffset = _mhal_tmds_phy_offset(enInputPort);
        msWriteByteMask(REG_1120D6 + wOffset, 0x05, 0x05);
    }
}


//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_InputIsHDMI2()
//  [Description]:
//                  return input signal is HDMI 2.0 or not
//  [Arguments]:
//                  enInputPort
//  [Return]:
//                  True: 2.0, False: 1.4
//**************************************************************************
BOOL mhal_tmds_InputIsHDMI2(MSCombo_TMDS_PORT_INDEX enInputPort)
{
    if(enInputPort <= MSCombo_TMDS_PORT_MAX)
    {
        TMDS_PRINT("Port: 0x%x, not support HDMI 2.0\r\n", enInputPort);
    }

    return FALSE;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_SetSplitter()
//  [Description]:
//                  Splitter setting
//  [Arguments]:
//                  enInputPort
//  [Return]:
//
//**************************************************************************
void mhal_tmds_SetSplitter(MSCombo_TMDS_PORT_INDEX enInputPort, Bool bFlag)
{
    Bool temp = 0;

    if(enInputPort <= MSCombo_TMDS_PORT_MAX)
    {
        temp = bFlag;
        TMDS_PRINT("Port: 0x%x, no need\r\n", enInputPort);
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_CheckAVMuteINT()
//  [Description]:
//                  check if GCP AV Mute INT status is set
//  [Arguments]:
//                  enInputPort
//                  bClr : True to clr INT status, false to polling status
//  [Return]:
//                  BOOL
//**************************************************************************
BOOL mhal_tmds_CheckAVMuteINT(MSCombo_TMDS_PORT_INDEX enInputPort, Bool bClr)
{
    WORD wOffset;

    wOffset = _mhal_tmds_bank_offset(enInputPort);

    if(!bClr)
    {
        if ((msReadByte(REG_1603D8 + wOffset) & BIT0) || (msReadByte(REG_16031A + wOffset) & BIT0))
            return TRUE;
        else
            return FALSE;
    }
    else
    {
        msWriteByteMask(REG_16031A + wOffset,0x00,0x01);
        msWriteByteMask(REG_16031C + wOffset,0x01,0x01); //[0]: clear GC pkt content chg status
        msWriteByteMask(REG_16031C + wOffset,0x00,0x01);
        return FALSE;
    }
}


//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_CheckAVMute()
//  [Description]:
//                  check if GCP AV Mute bit is set
//  [Arguments]:
//                  enInputPort
//  [Return]:
//                  BOOL
//**************************************************************************
BOOL mhal_tmds_CheckAVMute(MSCombo_TMDS_PORT_INDEX enInputPort)
{
    WORD wOffset;
    wOffset = _mhal_tmds_bank_offset(enInputPort);

    if (msReadByte(REG_160380 + wOffset) & BIT0)
        return TRUE;
    else
        return FALSE;

}

//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_AUPLL_Setting()
//  [Description]:
//                  do aupll setting for different audio sample rate
//  [Arguments]:
//                  bFlag : True > 100 khz, false <= 100khz
//  [Return]:
//
//**************************************************************************
void mhal_tmds_AUPLL_Setting(MScombo_TMDS_AUD_FREQ_INDEX enAudFreq)
{
    msWriteByteMask(REG_162F13, 0x00, 0x60); //KN
    msWriteByteMask(REG_162F15, 0x02, 0x0F); //DDIV
    switch(enAudFreq)
    {
        case MScombo_TMDS_AUD_FREQ_192K:
        case MScombo_TMDS_AUD_FREQ_176K:
            msWriteByteMask(REG_162F12, 0x11, 0xFF); //[7:4]:KM, [3:0]:KP
        break;
        case MScombo_TMDS_AUD_FREQ_96K:
        case MScombo_TMDS_AUD_FREQ_88K:
        case MScombo_TMDS_AUD_FREQ_48K:
        case MScombo_TMDS_AUD_FREQ_44K:
        case MScombo_TMDS_AUD_FREQ_32K:
            msWriteByteMask(REG_162F12, 0x22, 0xFF); //[7:4]:KM, [3:0]:KP
        break;
        default:
            msWriteByteMask(REG_162F12, 0x22, 0xFF); //[7:4]:KM, [3:0]:KP
        break;
    }
    msWriteByteMask(REG_162F15, 0x60,0xF0); //FBDIV
}

#endif // _MHAL_TMDS_C_

//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_HDCPFrameClear()
//  [Description]:
//                  HDCP frame clear
//  [Arguments]:
//                  [MSCombo_TMDS_PORT_INDEX]PortIdx
//                  [Bool]bFlag: TRUE: DVI mode; FALSE: HDMI mode
//  [Return]:
//
//**************************************************************************
void mhal_tmds_HDCPFrameClear(MSCombo_TMDS_PORT_INDEX enInputPort, Bool bFlag)
{
    Bool btemp = 0;
    if(enInputPort <= MSCombo_TMDS_PORT_MAX)
    {
        btemp = bFlag;
        TMDS_PRINT("Port: 0x%x, not support HDMI 2.0\r\n", enInputPort);
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_DEChange()
//  [Description]:
//                  check DE pixel change
//  [Arguments]:
//                  [MSCombo_TMDS_PORT_INDEX]PortIdx
//  [Return]:
//
//**************************************************************************
Bool mhal_tmds_DEChange(MSCombo_TMDS_PORT_INDEX enInputPort)
{
    Bool bDEChangeFlag = FALSE;
    WORD wOffset;

    wOffset = _mhal_tmds_bank_offset(enInputPort);

    if(msReadByte(REG_1603EC + wOffset) & 0x20) // [5]: DE pixel change
    {
        msWriteByte(REG_1603EC + wOffset, 0x20); // clear DE pixel change

        bDEChangeFlag = TRUE;
    }

    return bDEChangeFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_AudioFIFOReset()
//  [Description]:
//                  to reset audio fifo data
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_tmds_AudioFIFOReset(void)
{
    msWriteByteMask(REG_163088, 0x40, 0x40);
    msWriteByteMask(REG_163088, 0x00, 0x40);
}

//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_GetSCDCStatusFlag()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mhal_tmds_GetSCDCStatusFlag(MSCombo_TMDS_PORT_INDEX enInputPort, BYTE ucStatusMask)
{
    Bool bStatusFlag = FALSE;

    if(enInputPort == MSCombo_TMDS_PORT_MAX)
    {
        bStatusFlag = FALSE;
        ucStatusMask = 0;
    }

    return bStatusFlag;
}



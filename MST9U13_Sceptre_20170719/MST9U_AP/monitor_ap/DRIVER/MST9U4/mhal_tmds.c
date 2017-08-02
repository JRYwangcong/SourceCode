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
#include "msEread.h"

#include "Board.h"

#define TMDS_DEBUG    0
#if ENABLE_MSTV_UART_DEBUG && TMDS_DEBUG
#define TMDS_PRINT(format, ...)     printf(format, ##__VA_ARGS__)
#else
#define TMDS_PRINT(format, ...)
#endif

static WORD wPreviousTiming = 0;
static BYTE bPreviousPort = MSCombo_TMDS_PORT_MAX + 1; //make sure even port 0 will setting EQ for first time ISR
static BYTE bCurrentPort = MSCombo_TMDS_PORT_2;
static Bool bOver3GFlag[TMDS_INPUT_SELECT_MASK];
static Bool bSplitterDone[TMDS_INPUT_SELECT_MASK];
static BYTE bAutoEQRetry[TMDS_INPUT_SELECT_MASK];
static Bool bAutoEQCntRst[TMDS_INPUT_SELECT_MASK];
static Bool bAutoEQDone[TMDS_INPUT_SELECT_MASK];
static Bool bMHLCable[TMDS_INPUT_SELECT_MASK];
Bool bSWAutoEQEnable = FALSE;
BYTE ucSWAutoEQState = TMDS_SW_AUTO_EQ_NONE;
BYTE ucEfuseValuePort2Lane0 = 0;
BYTE ucEfuseValuePort2Lane1 = 0;
BYTE ucEfuseValuePort2Lane2 = 0;
BYTE ucEfuseValuePort3Lane0 = 0;
BYTE ucEfuseValuePort3Lane1 = 0;
BYTE ucEfuseValuePort3Lane2 = 0;

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
            wOffset = 0x0200;
            break;
        case MSCombo_TMDS_PORT_2:
        case MSCombo_TMDS_PORT_3:
            wOffset = 0x0400;
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
    if(enInputPort == MSCombo_TMDS_PORT_0)
    {
        //load current CTS/N value first
        msWrite2ByteMask(REG_160524, 0xFFFF, 0xFFFF);
        msWrite2ByteMask(REG_160526, 0xFFFF, 0xFFFF);
        msWriteByteMask(REG_160528, 0xFF, 0xFF);

        //update CTS value
        msWrite2Byte(REG_160514, msRead2Byte(REG_160524)); //CTS0, [15..0]
        msWriteByteMask(REG_160513, (msReadByte(REG_160528) & 0x0F), 0x0F); //CTS1, b[11..8]

        //update N value
        msWrite2Byte(REG_160516 , msRead2Byte(REG_160526)); //N0, [15..0]
        msWriteByteMask(REG_160513, (msReadByte(REG_160528) & 0xF0), 0xF0); //N1, b[11..8]

        //disable CTS/N filter
        msWriteByteMask(REG_16050F , 0x00, 0x40);

        //setup CTS/N range
        msWrite2Byte(REG_160518, 0x1414 ); //[15:8]: N range, [7:0]: CTS range

        //upload new CTS/N value
        msWriteByteMask(REG_16050F, 0x80, 0x80);

        //enable CTS/N filter
        msWriteByteMask(REG_16050F, 0x40, 0x40);
    }
    else if(enInputPort == MSCombo_TMDS_PORT_1)
    {
        //load current CTS/N value first
        msWrite2ByteMask(REG_160A24, 0xFFFF, 0xFFFF);
        msWrite2ByteMask(REG_160A26, 0xFFFF, 0xFFFF);
        msWriteByteMask(REG_160A28, 0xFF, 0xFF);

        //update CTS value
        msWrite2Byte(REG_160A14, msRead2Byte(REG_160A24)); //CTS0, [15..0]
        msWriteByteMask(REG_160A13, (msReadByte(REG_160A28) & 0x0F), 0x0F); //CTS1, b[11..8]

        //update N value
        msWrite2Byte(REG_160A16 , msRead2Byte(REG_160A26)); //N0, [15..0]
        msWriteByteMask(REG_160A13, (msReadByte(REG_160A28) & 0xF0), 0xF0); //N1, b[11..8]

        //disable CTS/N filter
        msWriteByteMask(REG_160A0F , 0x00, 0x40);

        //setup CTS/N range
        msWrite2Byte(REG_160A18, 0x1414 ); //[15:8]: N range, [7:0]: CTS range

        //upload new CTS/N value
        msWriteByteMask(REG_160A0F, 0x80, 0x80);

        //enable CTS/N filter
        msWriteByteMask(REG_160A0F, 0x40, 0x40);
    }
    else if((enInputPort == MSCombo_TMDS_PORT_2) || (enInputPort == MSCombo_TMDS_PORT_3))
    {
        //load current CTS/N value first
        msWrite2ByteMask(REG_161124, 0xFFFF, 0xFFFF);
        msWrite2ByteMask(REG_161126, 0xFFFF, 0xFFFF);
        msWriteByteMask(REG_161128, 0xFF, 0xFF);

        //update CTS value
        msWrite2Byte(REG_161114, msRead2Byte(REG_161124)); //CTS0, [15..0]
        msWriteByteMask(REG_161113, (msReadByte(REG_161128) & 0x0F), 0x0F); //CTS1, b[11..8]

        //update N value
        msWrite2Byte(REG_161116 , msRead2Byte(REG_161126)); //N0, [15..0]
        msWriteByteMask(REG_161113, (msReadByte(REG_161128) & 0xF0), 0xF0); //N1, b[11..8]

        //disable CTS/N filter
        msWriteByteMask(REG_16110F , 0x00, 0x40);

        //setup CTS/N range
        msWrite2Byte(REG_161118, 0x1414 ); //[15:8]: N range, [7:0]: CTS range

        //upload new CTS/N value
        msWriteByteMask(REG_16110F, 0x80, 0x80);

        //enable CTS/N filter
        msWriteByteMask(REG_16110F, 0x40, 0x40);
    }


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
    switch(enPortSelect)
    {
        case MSCombo_TMDS_PORT_0:
            if(msReadByte(REG_160437) & 0x02) // encryption enable
            {
                if(msReadByte(REG_16049E) & 0x01) // SKE successful
                    temp = TMDS_HDCP_2_2;
                else
                    temp = TMDS_HDCP_1_4;
            }
            break;

        case MSCombo_TMDS_PORT_1:
            if(msReadByte(REG_160937) & 0x02) // encryption enable
            {
                if(msReadByte(REG_16099E) & 0x01) // SKE successful
                    temp = TMDS_HDCP_2_2;
                else
                    temp = TMDS_HDCP_1_4;
            }
            break;

        case MSCombo_TMDS_PORT_2:
        case MSCombo_TMDS_PORT_3:
            if(msReadByte(REG_161037) & 0x02) // encryption enable
            {
                if(msReadByte(REG_16109E) & 0x01) // SKE successful
                    temp = TMDS_HDCP_2_2;
                else
                    temp = TMDS_HDCP_1_4;
            }
            break;

        default:

            break;
    };
	return temp;
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

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_tmds_AutoEQDoneInterrupt()
//  [Description]:
//                  use to check autoEQ done or not
//  [Arguments]:
//                  enInputPort : combo port
//  [Return]:
//                  TRUE : done
//                  FALSE : no done
//**************************************************************************
Bool _mhal_tmds_AutoEQDoneInterrupt(MSCombo_TMDS_PORT_INDEX enInputPort)
{
    if((enInputPort == MSCombo_TMDS_PORT_2) || (enInputPort == MSCombo_TMDS_PORT_3))
    {
        if(msReadByte(REG_112580) & 0x80) // autoEQ done
        {
            msWriteByte(REG_112583, 0x80); //clr
            msWriteByte(REG_112583, 0x00);
            msWriteByte(REG_112583, 0x00);
            return TRUE;
        }
        else
            return FALSE;
    }
    else
        return FALSE;
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_tmds_GetEfuseSettingValue()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_tmds_GetEfuseSettingValue(void)
{
    ucEfuseValuePort2Lane0 = msEread_GetDataFromEfuse(EFUSE_0, TMDS_PHY_EFUSE_ADDRESS0);
    ucEfuseValuePort2Lane1 = msEread_GetDataFromEfuse(EFUSE_0, TMDS_PHY_EFUSE_ADDRESS1);
    ucEfuseValuePort2Lane2 = msEread_GetDataFromEfuse(EFUSE_0, TMDS_PHY_EFUSE_ADDRESS2);
    ucEfuseValuePort3Lane0 = msEread_GetDataFromEfuse(EFUSE_0, TMDS_PHY_EFUSE_ADDRESS3);
    ucEfuseValuePort3Lane1 = msEread_GetDataFromEfuse(EFUSE_0, TMDS_PHY_EFUSE_ADDRESS4);
    ucEfuseValuePort3Lane2 = msEread_GetDataFromEfuse(EFUSE_0, TMDS_PHY_EFUSE_ADDRESS5);
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_tmds_SetEfuseSettingValue()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_tmds_SetEfuseSettingValue(MSCombo_TMDS_PORT_INDEX enInputPort, Bool bHDMI20Flag)
{
    switch(enInputPort)
    {
        case MSCombo_TMDS_PORT_2:
            if(ucEfuseValuePort2Lane0 &BIT7)
            {
                msWriteByteMask(REG_1124C0, bHDMI20Flag? 0: BIT7, BIT7); // REG_COMBO_PHY4_60[7]
                msWriteByteMask(REG_1124C2, bHDMI20Flag? 0: BIT7, BIT7); // REG_COMBO_PHY4_61[7]
                msWriteByteMask(REG_1124C3, bHDMI20Flag? 0: BIT7, BIT7); // REG_COMBO_PHY4_62[7]
                msWrite2ByteMask(REG_1124FC, bHDMI20Flag? 0: (WORD)((ucEfuseValuePort2Lane1 << 6)| ucEfuseValuePort2Lane0), BITMASK(11:0)); // REG_COMBO_PHY4_7E[11:6][5:0]
                msWriteByteMask(REG_1124FD, bHDMI20Flag? 0: (ucEfuseValuePort2Lane2 << 4), BITMASK(7:4)); // REG_COMBO_PHY4_7E[15:12]
                msWriteByteMask(REG_1124FE, bHDMI20Flag? 0: ucEfuseValuePort2Lane2, BITMASK(1:0)); // REG_COMBO_PHY4_7F[1:0]
            }

            break;

        case MSCombo_TMDS_PORT_3:
            if(ucEfuseValuePort3Lane0 &BIT7)
            {
                msWriteByteMask(REG_1124C0, bHDMI20Flag? 0: BIT7, BIT7); // REG_COMBO_PHY4_60[7]
                msWriteByteMask(REG_1124C2, bHDMI20Flag? 0: BIT7, BIT7); // REG_COMBO_PHY4_61[7]
                msWriteByteMask(REG_1124C3, bHDMI20Flag? 0: BIT7, BIT7); // REG_COMBO_PHY4_62[7]
                msWrite2ByteMask(REG_1124FC, bHDMI20Flag? 0: (WORD)((ucEfuseValuePort3Lane1 << 6)| ucEfuseValuePort3Lane0), BITMASK(11:0)); // REG_COMBO_PHY4_7E[11:6][5:0]
                msWriteByteMask(REG_1124FD, bHDMI20Flag? 0: (ucEfuseValuePort3Lane2 << 4), BITMASK(7:4)); // REG_COMBO_PHY4_7E[15:12]
                msWriteByteMask(REG_1124FE, bHDMI20Flag? 0: ucEfuseValuePort3Lane2, BITMASK(1:0)); // REG_COMBO_PHY4_7F[1:0]
            }

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
            msWriteByteMask(REG_112181, bEnable? 0: BIT3, BIT3); // REG_COMBO_PHY1_40[11], No input mask port0
            msWriteByte(REG_112183, BIT3); // REG_COMBO_PHY1_41[13], No input clear port0
            msWriteByte(REG_112183, 0); // REG_COMBO_PHY1_41[13], No input clear port0
            break;

        case MSCombo_TMDS_PORT_1:
            msWriteByteMask(REG_112381, bEnable? 0: BIT3, BIT3); // REG_COMBO_PHY3_40[11], No input mask port1
            msWriteByte(REG_112383, BIT3); // REG_COMBO_PHY3_41[11], No input clear port1
            msWriteByte(REG_112383, 0); // REG_COMBO_PHY3_41[11], No input clear port1
            break;

        case MSCombo_TMDS_PORT_2:
        case MSCombo_TMDS_PORT_3:
            msWriteByteMask(REG_112581, bEnable? 0: BIT3, BIT3); // REG_COMBO_PHY5_40[11], No input mask port2&3
            msWriteByte(REG_112583, BIT3); // REG_COMBO_PHY5_41[11], No input clear port2&3
            msWriteByte(REG_112583, 0); // REG_COMBO_PHY5_41[11], No input clear port2&3
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
            msWriteByteMask(REG_112181, bEnable? 0: BIT1, BIT1); // REG_COMBO_PHY1_40[1], clock stable mask port0

            if(!mhal_tmds_PM_Clock_Detect(enPortSelect))
            {
				msWriteByte(REG_112183, BIT1); // REG_COMBO_PHY1_41[9], clock stable clear port0
				msWriteByte(REG_112183, 0); // REG_COMBO_PHY1_41[9], clock stable clear port0
            }

            break;

        case MSCombo_TMDS_PORT_1:
            msWriteByteMask(REG_112381, bEnable? 0: BIT1, BIT1); // REG_COMBO_PHY3_40[1], clock stable mask port1

            if(!mhal_tmds_PM_Clock_Detect(enPortSelect))
            {
                msWriteByte(REG_112383, BIT1); // REG_COMBO_PHY3_41[9],clock stable clear port1
                msWriteByte(REG_112383, 0); // REG_COMBO_PHY3_41[9], clock stable clear port1
            }

            break;

        case MSCombo_TMDS_PORT_2:
        case MSCombo_TMDS_PORT_3:
            msWriteByteMask(REG_112581, bEnable? 0: BIT1, BIT1); // REG_COMBO_PHY5_40[1], clock stable mask port2&3

            if(!mhal_tmds_PM_Clock_Detect(enPortSelect) && (enPortSelect == bCurrentPort))
            {
                msWriteByte(REG_112583, BIT1); // REG_COMBO_PHY5_41[9], clock stable clear port2&3
                msWriteByte(REG_112583, 0); // REG_COMBO_PHY5_41[9], clock stable clear port2&3
            }

            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_tmds_ResetHDMI20VersionFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_tmds_ResetHDMI20VersionFlag(MSCombo_TMDS_PORT_INDEX enPortSelect)
{
    switch(enPortSelect)
    {
        case MSCombo_TMDS_PORT_0:

            break;

        case MSCombo_TMDS_PORT_1:

            break;

        case MSCombo_TMDS_PORT_2:
        case MSCombo_TMDS_PORT_3:
            msWriteByteMask(REG_001818, MASKBIT(3:2), MASKBIT(3:2)); // REG_PM_SCDC_0C[3:2]
            msWriteByteMask(REG_001818, 0, MASKBIT(3:2)); // REG_PM_SCDC_0C[3:2]
            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_tmds_ClearSCDCStatusFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_tmds_ClearSCDCStatusFlag(MSCombo_TMDS_PORT_INDEX enPortSelect, BYTE ucStatusMask)
{
    switch(enPortSelect)
    {
        case MSCombo_TMDS_PORT_0:

            break;

        case MSCombo_TMDS_PORT_1:

            break;

        case MSCombo_TMDS_PORT_2:
        case MSCombo_TMDS_PORT_3:
            msWriteByteMask(REG_00180F, ucStatusMask, ucStatusMask); // REG_PM_SCDC_07[15:8]
            msWriteByteMask(REG_00180F, 0, ucStatusMask); // REG_PM_SCDC_07[15:8]
            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_tmds_HDMI20AutoEQSetting()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_tmds_HDMI20AutoEQSetting(Bool bHDMI20Flag)
{
    msWriteByteMask(REG_11242B, 0x00, 0x80); // Bypass symbol lock detect in coarse tune state
    msWriteByteMask(REG_11258A, bHDMI20Flag? 0x00: 0x7, 0x1F); // The limit of the auto EQ start value (coarse tune min value)
    msWriteByteMask(REG_11258B, 0x1F, 0x1F); // The limit of the auto EQ end value (coarse tune max value)
    msWriteByteMask(REG_112467, bHDMI20Flag? 0x12: 0x7, 0x3F); // Start EQ strength for auto EQ scan (coarse tune start value)
    msWrite2ByteMask(REG_112584, 0x0050, 0xFFFF); // coarse tune symbol detect times
    msWrite2ByteMask(REG_112594, 0x0019, 0xFFFF); // coarse tune symbol aaba number
    msWriteByteMask(REG_112588, 0x01, 0x01); // enable coarse symbol lock
    msWrite2ByteMask(REG_112596, bHDMI20Flag? 0x1FFF: 0, 0xFFFF); // fine tune aaba number
    msWrite2ByteMask(REG_112598, bHDMI20Flag? 0x05FF: 0, 0xFFFF); // fine tune under threshold
    msWriteByteMask(REG_112592, bHDMI20Flag? 0: BIT5, BIT5); // enable fine tune symbol lock mode
    msWriteByteMask(REG_112451, 0x00, 0x04); // disable timer mode in fine tune
    msWriteByteMask(REG_112466, 0x08, 0x08); // enable tmds trigger
    msWrite2ByteMask(REG_112452, bHDMI20Flag? 0x0100: 0x0400, 0xFFFF); // coarse tune each step time
    msWriteByteMask(REG_11244D, bHDMI20Flag? 0x0A: 0x1, 0x3F); // fine tune settel times
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_tmds_SetErrorCountStatus()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_tmds_SetErrorCountStatus(void)
{
    msWriteByteMask(REG_160C2B, BIT2, BIT2); // REG_DVI_DTOP_DUAL_15[10]
    msWriteByteMask(REG_160C16, BIT3, BIT3); // REG_DVI_DTOP_DUAL_0B[3]
    msWriteByteMask(REG_160C16, 0, BIT3); // REG_DVI_DTOP_DUAL_0B[3]
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_tmds_GetErrorCountStatus()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool _mhal_tmds_GetErrorCountStatus(void)
{
    Bool bErrorFlag = FALSE;

    if((msReadByte(REG_160C0E) &MASKBIT(3:0)) > 0) // REG_DVI_DTOP_DUAL_07
    {
        bErrorFlag = TRUE;
    }
    else if((msReadByte(REG_160C10) &MASKBIT(3:0)) > 0) // REG_DVI_DTOP_DUAL_08
    {
        bErrorFlag = TRUE;
    }
    else if((msReadByte(REG_160C12) &MASKBIT(3:0)) > 0) // REG_DVI_DTOP_DUAL_09
    {
        bErrorFlag = TRUE;
    }
    else
    {
        msWrite2ByteMask(REG_160CB0, 0xFE00, MASKBIT(15:0)); // REG_DVI_DTOP_DUAL_58

        if(msRead2Byte(REG_160CB2) > HDMI_DECORD_ERROR_THRESHOLD) // REG_DVI_DTOP_DUAL_59
        {
            bErrorFlag = TRUE;
        }
        else
        {
            msWrite2ByteMask(REG_160CB0, 0xFD00, MASKBIT(15:0)); // REG_DVI_DTOP_DUAL_58

            if(msRead2Byte(REG_160CB2) > HDMI_DECORD_ERROR_THRESHOLD) // REG_DVI_DTOP_DUAL_59
            {
                bErrorFlag = TRUE;
            }
            else
            {
                msWrite2ByteMask(REG_160CB0, 0xFC00, MASKBIT(15:0)); // REG_DVI_DTOP_DUAL_58

                if(msRead2Byte(REG_160CB2) > HDMI_DECORD_ERROR_THRESHOLD) // REG_DVI_DTOP_DUAL_59
                {
                    bErrorFlag = TRUE;
                }
            }
        }
    }

    return bErrorFlag;
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_tmds_AdjustEQValue()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_tmds_AdjustEQValue(BYTE ucEQValueState)
{
    BYTE ucEQSettingValue = 0x0C;

    switch(ucEQValueState)
    {
        case TMDS_SW_AUTO_EQ_VALUE0:
            ucEQSettingValue = 0x03;
            break;

        case TMDS_SW_AUTO_EQ_VALUE1:
            ucEQSettingValue = 0x10;
            break;

        case TMDS_SW_AUTO_EQ_VALUE2:
            ucEQSettingValue = 0x10;
            break;

        case TMDS_SW_AUTO_EQ_VALUE3:
            ucEQSettingValue = 0x10;
            break;

        default:

            break;
    };

    msWrite2ByteMask(REG_1124E6, (ucEQSettingValue<<8)|ucEQSettingValue, 0x1F1F);
    msWrite2ByteMask(REG_1124E8, (ucEQSettingValue<<8)|ucEQSettingValue, 0x1F1F);
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_tmds_SWAutoEQPollingProc()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_tmds_SWAutoEQPollingProc(MSCombo_TMDS_PORT_INDEX enInputPort)
{
    static BYTE ucCheckErrorCounter = 0;
    static BYTE ucEQValeState = TMDS_SW_AUTO_EQ_VALUE0;
    static WORD usSWAutoEQCounter = 0;

    switch(ucSWAutoEQState)
    {
        case TMDS_SW_AUTO_EQ_START: // Change to HDMI 1.4 auto EQ setting, reset error status, and trigger auto EQ
            ucSWAutoEQState = TMDS_SW_AUTO_EQ_WAIT_DONE;

            _mhal_tmds_HDMI20AutoEQSetting(FALSE);
            mhal_tmds_TriggerAutoEQ(enInputPort, TRUE, TRUE);

            TMDS_PRINT("TMDS SW auto EQ start^^\r\n");

            break;

        case TMDS_SW_AUTO_EQ_WAIT_DONE:
            if(_mhal_tmds_AutoEQDoneInterrupt(enInputPort)) // Auto EQ done and heck error status
            {
                ucSWAutoEQState = TMDS_SW_AUTO_EQ_WAIT_SIGNAL_STABLE;

                usSWAutoEQCounter = 0;
                ucEQValeState = TMDS_SW_AUTO_EQ_VALUE0;

                TMDS_PRINT("TMDS SW auto EQ signal done~~\r\n");
            }

            break;

        case TMDS_SW_AUTO_EQ_WAIT_SIGNAL_STABLE:
            if(usSWAutoEQCounter == HDMI_SW_AUTO_WAIT_STABLE_TIME)
            {
                ucSWAutoEQState = TMDS_SW_AUTO_EQ_CLEAR_ERROR_STATUS;
            }

            break;

        case TMDS_SW_AUTO_EQ_CLEAR_ERROR_STATUS:
            ucSWAutoEQState = TMDS_SW_AUTO_EQ_CHECK_ERROR;

            _mhal_tmds_ResetHDMI20VersionFlag(enInputPort);
            _mhal_tmds_SetErrorCountStatus();

            TMDS_PRINT("TMDS SW auto EQ clear error status\r\n");

            ucCheckErrorCounter = 0;
            usSWAutoEQCounter = 0;

            break;

        case TMDS_SW_AUTO_EQ_CHECK_ERROR:
            if(usSWAutoEQCounter == HDMI_SW_AUTO_CHECK_ERROR_TIME)
            {
                if(_mhal_tmds_GetErrorCountStatus()) // Error happen then change EQ setting
                {
                    ucSWAutoEQState = TMDS_SW_AUTO_EQ_CHANGE_SETTING;
                }
                else if(ucCheckErrorCounter < HDMI_SW_AUTO_RETRY_CHECK_ERROR)
                {
                    ucCheckErrorCounter++;
                    usSWAutoEQCounter = 0;
                }
                else
                {
                    ucSWAutoEQState = TMDS_SW_AUTO_EQ_FINISH;

                    TMDS_PRINT("TMDS SW auto EQ finish!!\r\n");
                }
            }

            break;

        case TMDS_SW_AUTO_EQ_CHANGE_SETTING:
            _mhal_tmds_AdjustEQValue(ucEQValeState);
            ucSWAutoEQState = TMDS_SW_AUTO_EQ_CLEAR_ERROR_STATUS;

            ucEQValeState = (ucEQValeState +1) %TMDS_SW_AUTO_EQ_VALUE_MASK;

            break;

        case TMDS_SW_AUTO_EQ_FINISH: // Change to HDMI 2.0 aute EQ setting
            ucSWAutoEQState = TMDS_SW_AUTO_EQ_NONE;
            bSWAutoEQEnable = FALSE;

            _mhal_tmds_HDMI20AutoEQSetting(TRUE);

            break;

        default:

            break;
    };

    if(ucSWAutoEQState < TMDS_SW_AUTO_EQ_FINISH)
    {
        usSWAutoEQCounter++;
    }
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

}

//**************************************************************************
//  [Function Name]:
//                  _mhal_tmds_Cable5V_Detect()
//  [Description]:
//                  cable5V detect
//  [Arguments]:
//                  enInputPort : combo port
//  [Return]:
//                  none
//**************************************************************************
Bool _mhal_tmds_Cable5V_Detect(MSCombo_TMDS_PORT_INDEX enInputPort)
{
    Bool bCable5V = FALSE;

    if(enInputPort == MSCombo_TMDS_PORT_2)
    {
		if(msReadByte(REG_000486) & BIT1)
		{
		   bCable5V = TRUE;
		}
    }
	else if(enInputPort == MSCombo_TMDS_PORT_3)
	{
		if(msReadByte(REG_00048A) & BIT1)
		{
		   bCable5V = TRUE;
		}
	}
	return bCable5V;
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
    if(ucComboVersionIndex > 0)
    {
        switch(ucPortSelect)
        {
            case COMBO_INPUT_SELECT_PORT0:
                msWriteByteMask(REG_001724, bRtermEnable? 0: BIT7, BIT7); // REG_PM_BANK_12[7]: Power control P0 clock r-term
                msWriteByteMask(REG_112068, bRtermEnable? 0: BIT1, BIT1); // REG_COMBO_PHY0_34[1]: Power down standyby clock amplifier
                break;

            case COMBO_INPUT_SELECT_PORT1:
    		    msWriteByteMask(REG_001726, bRtermEnable? 0: BIT7, BIT7); // REG_PM_BANK_13[7]: Power control P1 clock r-term
                msWriteByteMask(REG_112268, bRtermEnable? 0: BIT1, BIT1); // REG_COMBO_PHY2_34[1]: Power down standyby clock amplifier
                break;

            case COMBO_INPUT_SELECT_PORT2:
    		    msWriteByteMask(REG_001720, bRtermEnable? 0: BIT3, BIT3); // REG_PM_BANK_10[3]: Power control P2 clock r-term
                msWriteByteMask(REG_1124B7, bRtermEnable? 0: BIT4, BIT4); // REG_COMBO_PHY4_5B[12]: Power down clock amplifier
                break;

            case COMBO_INPUT_SELECT_PORT3:
    		    msWriteByteMask(REG_001722, bRtermEnable? 0: BIT3, BIT3); // REG_PM_BANK_11[3]: Power control P3 clock r-term
                msWriteByteMask(REG_1124B7, bRtermEnable? 0: BIT4, BIT4); //  REG_COMBO_PHY4_5B[12]: Power down clock amplifier
                break;

            case COMBO_INPUT_SELECT_PORT4:
    		    //msWriteByteMask(REG_001781, bRtermEnable? 0: BIT4, BIT4); // REG_PM_BANK_40[12]: Power control P1 clock r-term
                break;

            default:

                break;
        };
    }
    else
    {
        switch(ucPortSelect)
        {
            case COMBO_INPUT_SELECT_PORT0:
                msWriteByteMask(REG_001724, bRtermEnable? 0: BIT7, BIT7); // REG_PM_BANK_12[7]: Power control P0 clock r-term
                msWriteByteMask(REG_112068, bRtermEnable? 0: BIT1, BIT1); // REG_COMBO_PHY0_34[1]: Power down standyby clock amplifier
                break;

            case COMBO_INPUT_SELECT_PORT1:
    		    msWriteByteMask(REG_001726, bRtermEnable? 0: BIT7, BIT7); // REG_PM_BANK_13[7]: Power control P1 clock r-term
                msWriteByteMask(REG_112268, bRtermEnable? 0: BIT1, BIT1); // REG_COMBO_PHY2_34[1]: Power down standyby clock amplifier
                break;

            case COMBO_INPUT_SELECT_PORT2:
    		    msWriteByteMask(REG_001720, bRtermEnable? 0: BIT0, BIT0); // REG_PM_BANK_10[0]: Power control P2 clock r-term
                msWriteByteMask(REG_1124B7, bRtermEnable? 0: BIT4, BIT4); // REG_COMBO_PHY4_5B[12]: Power down clock amplifier
                break;

            case COMBO_INPUT_SELECT_PORT3:
    		    msWriteByteMask(REG_001722, bRtermEnable? 0: BIT0, BIT0); // REG_PM_BANK_11[0]: Power control P3 clock r-term
                msWriteByteMask(REG_1124B7, bRtermEnable? 0: BIT4, BIT4); //  REG_COMBO_PHY4_5B[12]: Power down clock amplifier
                break;

            case COMBO_INPUT_SELECT_PORT4:
    		    //msWriteByteMask(REG_001781, bRtermEnable? 0: BIT4, BIT4); // REG_PM_BANK_40[12]: Power control P1 clock r-term
                break;

            default:

                break;
        };
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

    // CTS/N over range
    if (bAudioErrorStatus & ERR_HDMI_CTSN_OVERRANGE)
    {
        if(enInputPort == MSCombo_TMDS_PORT_0)
        {
             if (msReadByte(REG_160504) & BIT6) //[6]: CTS N over range
             {
                 _mhal_tmds_UpdateCTSNValue(enInputPort);
                 msWriteByteMask(REG_160504, 0x40, 0x40); // clear CTS/N over range
             }
             else
                 bAudioErrorStatus &= (~ERR_HDMI_CTSN_OVERRANGE);
        }
        else if(enInputPort == MSCombo_TMDS_PORT_1)
        {
             if (msReadByte(REG_160A04) & BIT6) //[6]: CTS N over range
             {
                 _mhal_tmds_UpdateCTSNValue(enInputPort);
                 msWriteByteMask(REG_160A04, 0x40, 0x40); // clear CTS/N over range
             }
             else
                 bAudioErrorStatus &= (~ERR_HDMI_CTSN_OVERRANGE);
        }
        else if((enInputPort == MSCombo_TMDS_PORT_2) || (enInputPort == MSCombo_TMDS_PORT_3))
        {
             if (msReadByte(REG_161104) & BIT6) //[6]: CTS N over range
             {
                 _mhal_tmds_UpdateCTSNValue(enInputPort);
                 msWriteByteMask(REG_161104, 0x40, 0x40); // clear CTS/N over range
             }
             else
                 bAudioErrorStatus &= (~ERR_HDMI_CTSN_OVERRANGE);
        }

    }

    //Clock Big Change
    if (bAudioErrorStatus & ERR_HDMI_CLK_BIG_CHANGE)
    {
        if(enInputPort == MSCombo_TMDS_PORT_0)
        {
             if (msReadByte(REG_160504) & BIT7) // [7]: Clock  big change
             {
                 msWriteByteMask(REG_160504, 0x80, 0x80); // clear clk big change status
             }
             else
                 bAudioErrorStatus &= (~ERR_HDMI_CLK_BIG_CHANGE);
        }
        else if(enInputPort == MSCombo_TMDS_PORT_1)
        {
             if (msReadByte(REG_160A04) & BIT7) // [7]: Clock  big change
             {
                 msWriteByteMask(REG_160A04, 0x80, 0x80); // clear clk big change status
             }
             else
                 bAudioErrorStatus &= (~ERR_HDMI_CLK_BIG_CHANGE);
        }
        else if((enInputPort == MSCombo_TMDS_PORT_2) || (enInputPort == MSCombo_TMDS_PORT_3))
        {
             if (msReadByte(REG_161104) & BIT7) // [7]: Clock  big change
             {
                 msWriteByteMask(REG_161104, 0x80, 0x80); // clear clk big change status
             }
             else
                bAudioErrorStatus &= (~ERR_HDMI_CLK_BIG_CHANGE);
        }
    }

    //AV mute
    if (bAudioErrorStatus & ERR_HDMI_AVMUTE)
    {
        if(enInputPort == MSCombo_TMDS_PORT_0)
        {
            if (bClrAVMute)
            {
                msWriteByteMask(REG_1605C4, 0x01, 0x01); // raise clear AV mute status
                msWriteByteMask(REG_1605BF, 0x01, 0x01); // clear AV mute
                msWriteByteMask(REG_1605BF, 0x00, 0x01); // clear AV mute
                bAudioErrorStatus &= (~ERR_HDMI_AVMUTE);
            }
        }
        else if(enInputPort == MSCombo_TMDS_PORT_1)
        {
            if (bClrAVMute)
            {
                msWriteByteMask(REG_160AC4, 0x01, 0x01); // raise clear AV mute status
                msWriteByteMask(REG_160ABF, 0x01, 0x01); // clear AV mute
                msWriteByteMask(REG_160ABF, 0x00, 0x01); // clear AV mute
                bAudioErrorStatus &= (~ERR_HDMI_AVMUTE);
            }
        }
        else if((enInputPort == MSCombo_TMDS_PORT_2) || (enInputPort == MSCombo_TMDS_PORT_3))
        {
            if (bClrAVMute)
            {
                msWriteByteMask(REG_1611C4, 0x01, 0x01); // raise clear AV mute status
                msWriteByteMask(REG_1611BF, 0x01, 0x01); // clear AV mute
                msWriteByteMask(REG_1611BF, 0x00, 0x01); // clear AV mute
                bAudioErrorStatus &= (~ERR_HDMI_AVMUTE);
            }
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
        if (msReadByte(REG_1630B0) & BIT2) // [2]: audio ch status change
        {
            msWriteByteMask(REG_1630B0, BIT2, BIT2);
        }
        else
            bAudioErrorStatus &= (~ERR_HDMI_AUD_CH_ST_CHANGE);
    }

    //Audio FIFO Error (underflow / overflow)
    if (bAudioErrorStatus & ERR_HDMI_AUD_FIFO_ERR)
    {
        if (msReadByte(REG_1630B1) & (BIT1 | BIT0)) // [1]: ch1/ch2 over run, [0]: ch1/ch2 under run
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

    _mhal_tmds_UpdateCTSNValue(enInputPort);

    if(enInputPort == MSCombo_TMDS_PORT_0)
    {
        msWriteByteMask(REG_160504, 0x40, 0x40); // clear CTS/N over range
        msWriteByteMask(REG_162FFE, 0x00, 0x02); // hdmi synthesizer lane0 clock div2 disable
        msWrite2ByteMask(REG_1630B0, 0x3FF, 0x3FF); // clear all status;
        msWriteByteMask(REG_1605BF, 0x01, 0x01); // clear AV mute
        msWriteByteMask(REG_1605BF, 0x00, 0x01); // clear AV mute
    }
    else if(enInputPort == MSCombo_TMDS_PORT_1)
    {
        msWriteByteMask(REG_160A04, 0x40, 0x40); // clear CTS/N over range
        msWriteByteMask(REG_162FFE, 0x00, 0x02); // hdmi synthesizer lane0 clock div2 disable
        msWrite2ByteMask(REG_1630B0, 0x3FF, 0x3FF); // clear all status;
        msWriteByteMask(REG_160ABF, 0x01, 0x01); // clear AV mute
        msWriteByteMask(REG_160ABF, 0x00, 0x01); // clear AV mute
    }
    else if((enInputPort == MSCombo_TMDS_PORT_2) || (enInputPort == MSCombo_TMDS_PORT_3))
    {
        msWriteByteMask(REG_161104, 0x40, 0x40); // clear CTS/N over range
        msWriteByteMask(REG_162FFE, 0x02, 0x02); // hdmi synthesizer lane0 clock div2 enable
        msWrite2ByteMask(REG_1630B0, 0x3FF, 0x3FF); // clear all status;
        msWriteByteMask(REG_1611BF, 0x01, 0x01); // clear AV mute
        msWriteByteMask(REG_1611BF, 0x00, 0x01); // clear AV mute
    }
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
    if(enInputPort == MSCombo_TMDS_PORT_2)
    {
        bCurrentPort = MSCombo_TMDS_PORT_2;
        msWriteByteMask(REG_11251C, 0x00, 0x01); // [0]:mhl3_atop selection :0 (mhl3_atop_0)
        msWriteByteMask(REG_000461, 0x80, 0x80); // [15]: HDCP & SCDC I2C port selection :1 (PAD_GPIOX13/GPIOX14)
    }
    else if (enInputPort == MSCombo_TMDS_PORT_3)
    {
        bCurrentPort = MSCombo_TMDS_PORT_3;
        msWriteByteMask(REG_11251C, 0x01, 0x01); // [0]: mhl3_atop selection :1 (mhl3_atop_1)
        msWriteByteMask(REG_000461, 0x00, 0x80); // [15]: HDCP & SCDC I2C port selection:0 (PAD_GPIOX23/GPIOX24)
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
    static Bool bReadEfuse = FALSE;
    WORD wOffset;

    wPreviousTiming = 0;
    bPreviousPort = MSCombo_TMDS_PORT_MAX + 1; //make sure even port 0 will setting EQ for first time ISR
    bOver3GFlag[enInputPort] = FALSE;
    bSplitterDone[enInputPort] = FALSE;
    bAutoEQRetry[enInputPort] = 0;
    bAutoEQCntRst[enInputPort] = FALSE;
    bAutoEQDone[enInputPort] = FALSE;
    bMHLCable[enInputPort] = FALSE;

    wOffset = _mhal_tmds_phy_offset(enInputPort);
    //eq 3tage setting
    msWriteByteMask(REG_1120BE + wOffset, BIT6|BIT5, BIT6|BIT5); //lane0 always turn on 3 stage.
    msWriteByteMask(REG_1120C0 + wOffset, BIT6|BIT5, BIT6|BIT5); //lane1 always turn on 3 stage.
    msWriteByteMask(REG_1120C2 + wOffset, BIT6|BIT5, BIT6|BIT5); //lane2 always turn on 3 stage.
    msWriteByteMask(REG_1120C4 + wOffset, BIT6|BIT5, BIT6|BIT5); //lane3 always turn on 3 stage.

    msWrite2ByteMask(REG_163062, 0, 0x0001); //  PD_CLKXTAL
  //  msWrite2Byte(REG_1120DE + wOffset, 0x0F00);// overwrite
    if((enInputPort== MSCombo_TMDS_PORT_0) || (enInputPort== MSCombo_TMDS_PORT_1))
    {
        msWrite2Byte(REG_1120D6 + wOffset, 0x0702); // [1]: reg_atop_en_clko_pix=1
        msWrite2Byte(REG_112014 + wOffset, 0x0040); // [6]: reg_af_ls_20out_sel=1
        msWrite2Byte(REG_1120B4 + wOffset, 0x0000); // [2]: disable reg_atop_en_clko_tmds2x

    }
    else if((enInputPort== MSCombo_TMDS_PORT_2) || (enInputPort== MSCombo_TMDS_PORT_3))
    {
        msWrite2Byte(REG_1120D6 + wOffset, 0x0706); // [1]: reg_atop_en_clko_pix=1
        msWrite2Byte(REG_112014 + wOffset, 0x0040); // [6]: reg_af_ls_20out_sel=1
        msWrite2Byte(REG_1120B4 + wOffset, 0x0004); // [2]: disable reg_atop_en_clko_tmds2x
        msWrite2ByteMask(REG_163062, 0, 0xFFFF); // enable P2 all clock power
        msWriteByteMask(REG_163060, 0x05, 0x0D); // [2]: reg_pix_clk_div2_en_p2; [0]: reg_tmds_clk_div2_en_p2
        msWriteByteMask(REG_1612A8, 0x02, 0x03); // [1:0]: reg_avg_ctrl_case

		if(_mhal_tmds_Cable5V_Detect(enInputPort))
		{
		    mhal_tmds_HDMI20_MUX(enInputPort);
		}
    }

    if(enInputPort == MSCombo_TMDS_PORT_0)
    {
        msWrite2ByteMask(REG_163022, 0, 0xFFFF); // enable P0 all clock power
        msWriteByteMask(REG_001724, 0, 0x40); // REG_PD_CLKIN_OFFL = 0
        msWriteByteMask(REG_112107, 0, 0x07); // [10:8]: Power on P0 data r-term
        msWriteByteMask(REG_001724, 0, 0xFF); // [7]: Power on P0 clock r-term
        msWriteByteMask(REG_001725, 0x01, 0x01); // reg_p0_pd_cmos_sw =1
        msWriteByteMask(REG_112068, 0, BIT1); // power on standby clock amplifier

    }
    else if(enInputPort == MSCombo_TMDS_PORT_1)
    {
       msWrite2ByteMask(REG_163042, 0, 0xFFFF); // enable P1 all clock power
       msWriteByteMask(REG_001726, 0, 0x40); // REG_PD_CLKIN_OFFL = 0
       msWriteByteMask(REG_112307, 0, 0x07); // [10:8]: Power on P1 data r-term
       msWriteByteMask(REG_001726, 0, 0xFF); // [7]: Power on P1 clock r-term
       msWriteByteMask(REG_001727, 0x01, 0x01); // reg_p1_pd_cmos_sw =1
       msWriteByteMask(REG_112268, 0, BIT1); // power on standby clock amplifier
    }
    else if(enInputPort == MSCombo_TMDS_PORT_2)
    {
        msWriteByteMask(REG_001720, 0, 0x0F); // [3:1]: Power on Port2 data r-term
        msWriteByteMask(REG_001720, 0, 0x10); // [4]: Power on Port2 clock r-term
        msWriteByteMask(REG_1124B7, 0, BIT4); // power on clock amplifier
    }
    else if(enInputPort == MSCombo_TMDS_PORT_3)
    {
        msWriteByteMask(REG_001722, 0, 0x0F); // [3:1]: Power on Port3 data r-term
        msWriteByteMask(REG_001722, 0, 0x10); // [4]: Power on Port3 clock r-term
        msWriteByteMask(REG_1124B7, 0, BIT4); // power on clock amplifier
    }

    msWriteByteMask(REG_112500, 0x00, 0x0F); //[2:0]: reg_atop_pd_oscreg_atop_pd_osc; [3]: power down internal LDO
    msWriteByteMask(REG_112000 + wOffset, 0x80, 0x80); //[7]: Main link function select: HDMI/MHL0
    msWrite2Byte(REG_1120AE + wOffset, 0x010C); // [8]: enable mhl fullrate mode; [7:0]: Set Xtal freq
    msWrite2Byte(REG_112016 + wOffset, 0xFFFF); // RX Phy Clock enable
    msWrite2Byte(REG_112018 + wOffset, 0x3FFF); // RX Phy Clock enable
    msWrite2Byte(REG_1120B6 + wOffset, 0x0000); // [13]: reg_atop_pd_bg; [12]: reg_atop_pd_clkin; [11:8]: reg_atop_pd_phdac_ov; [3:0]: reg_atop_pd_lane
    msWriteByteMask(REG_1120F4 + wOffset, 0x00, 0x0F); // [3:0]: reg_atop_pd_pll
    // [0]: REG_EN_SQH ; [1]: reg_pd_clkin; [3:2]: reg_test_sqh; [7:4]: reg_sq_vth;
    // [11:8]: reg_pd_rt; [13:12]: reg_test_clkin; [14]: reg_dato_sqh;[15]: reg_clk_xtali_div2_en
    msWrite2Byte(REG_112068 + wOffset, 0x0040);
    msWrite2Byte(REG_11206A + wOffset, 0x0480); // [10]: reg_hdmi2_acdr_mode = 1
    msWrite2Byte(REG_112090 + wOffset, 0x0702); // [15:8]: reg_mainlink_crlose_thr; [15:8]: reg_mainlink_crlose_thr


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

    //R-term value over write
    msWriteByteMask(REG_1120B3 + wOffset, 0, 0x01); //[0]: overwtite
    msWriteByteMask(REG_1120B7 + wOffset, 0, 0xF0); //[7:4]: R-term value

#endif
    //CLK valid boundary
    msWrite2ByteMask(REG_112076 + wOffset, 0x1F00, 0x1FFF); // [12:0]: reg_clk_valid_u
    //CR Lock Mode
    msWrite2ByteMask(REG_11208A + wOffset, 0x1F83, 0x7FFF); // [14:8]: reg_dig_lock_time; [7]: reg_dig_lock_mode; [3:0]: reg_dig_lck_cnt_power


    //xtal divider setting, default value is ok, we don't modify here
    //msWriteByte(REG_112062 + wOffset, 0x3F);//clock_xtal_divider_HBR

    //CR lock/unlock threshold, setting default value currently
    //msWrite2Byte(0x112090 + wOffset, 0x0702); //MAINLINK_CRLOCK_THR[7:0], MAINLINK_CRLOSE_THR[8:15];

    //CR lock filter, setting default value currently
    //msWriteByteMask(0x11205D + wOffset, 0x06, 0x1F); //CR_LOCK_TH[4:0]


    //HDMI_MHL divider threshold
    if((enInputPort== MSCombo_TMDS_PORT_0) || (enInputPort== MSCombo_TMDS_PORT_1))
    {
        msWrite2Byte(REG_112092 + wOffset, 0x0B43); // reg_hdmi_clk_thr4, the value of cr_lock_cnt in 270Mhz
        msWrite2Byte(REG_112094 + wOffset, 0x0715); // reg_hdmi_clk_thr3, the value of cr_lock_cnt in 170Mhz
        msWrite2Byte(REG_112096 + wOffset, 0x038B); // reg_hdmi_clk_thr2 , the value of cr_lock_cnt in 85Mhz
        msWrite2Byte(REG_112098 + wOffset, 0x01C6); // reg_hdmi_clk_thr2 , the value of cr_lock_cnt in 42.5Mhz
        msWrite2Byte(REG_11209A + wOffset, 0x0716); // reg_hdmi2_clk_thr3, the value of cr_lock_cnt in 170Mhz
        msWrite2Byte(REG_11209C + wOffset, 0x01C6); // reg_hdmi2_clk_thr2, the value of cr_lock_cnt in 42.5Mhz
        msWrite2Byte(REG_11209E + wOffset, 0x012E); //r eg_hdmi2_clk_thr1, the value of cr_lock_cnt in 28.33Mhz
    }
    else if((enInputPort== MSCombo_TMDS_PORT_2) || (enInputPort== MSCombo_TMDS_PORT_3))
    {
        msWrite2Byte(REG_112092 + wOffset, 0x0A6B); // reg_hdmi_clk_thr4, the value of cr_lock_cnt in 250Mhz
        msWrite2Byte(REG_112094 + wOffset, 0x0715); // reg_hdmi_clk_thr3, the value of cr_lock_cnt in 170Mhz
        msWrite2Byte(REG_112096 + wOffset, 0x038B); // reg_hdmi_clk_thr2 , the value of cr_lock_cnt in 85Mhz
        msWrite2Byte(REG_112098 + wOffset, 0x01C5); // reg_hdmi_clk_thr2 , the value of cr_lock_cnt in 42.5Mhz
        msWrite2Byte(REG_11209A + wOffset, 0x0320); // reg_hdmi2_clk_thr3, the value of cr_lock_cnt in 75Mhz
        msWrite2Byte(REG_11209C + wOffset, 0x0190); // reg_hdmi2_clk_thr2, the value of cr_lock_cnt in 37.5Mhz
        msWrite2Byte(REG_11209E + wOffset, 0x0215); //r eg_hdmi2_clk_thr1, the value of cr_lock_cnt in 50Mhz
    }

    // EQ setting initialize
    msWrite2Byte(REG_11204A + wOffset, 0x0AC8);
    msWrite2Byte(REG_112056 + wOffset, 0x0018);
    msWrite2Byte(REG_112058 + wOffset, 0x0004);
    msWrite2Byte(REG_112050 + wOffset, 0x3A83);
    msWrite2Byte(REG_11204C + wOffset, 0x080A); // [5:0]: reg_phase_check_time; [13:8]: reg_eq_fine_tune_time

    // PHASE
    msWrite2ByteMask(REG_112022 + wOffset, 0x0000, 0x1FFF); // [12]: reg_dvi_ovch0; [11:0]: reg_dvi_ch0_ph
    msWrite2ByteMask(REG_112024 + wOffset, 0x0000, 0x1FFF); // [12]: reg_dvi_ovch1; [11:0]: reg_dvi_ch1_ph
    msWrite2ByteMask(REG_112026 + wOffset, 0x0000, 0x1FFF); // [12]: reg_dvi_ovch2; [11:0]: reg_dvi_ch2_ph

    msWrite2Byte(REG_1120B8 + wOffset, 0x0000); // [3:0]: reg_atop_sel_phdclk
    msWriteByteMask(REG_112020 + wOffset, 0x18, 0x18); // [4]: reg_swap_dcdr_updn_polity; [3]: reg_enable_phase_step

    // HDMI Auto Detect
    msWrite2ByteMask(REG_112160 + wOffset, 0x0026, 0x013F); // [5]: reg_hdmi_auto_det_eq_en; [2]: reg_hdmi_auto_det_en; [1]: reg_do_auto_det_hdmi; [0]: reg_tmds_bit_valid
    msWrite2Byte(REG_112170 + wOffset, 0x00FF); // reg_hdmi_symbol_det_times
    msWrite2Byte(REG_112194 + wOffset, 0x003A); // reg_hdmi_pat_det_times1
    msWrite2Byte(REG_112196 + wOffset, 0x0055); // reg_hdmi_pat_det_times2
    msWrite2Byte(REG_112198 + wOffset, 0x001A); // reg_hdmi_under_th
    msWriteByte(REG_11216E + wOffset, 0x1F); // [7:4]: reg_hdmi_auto_det_times; [3:0]: reg_hdmi_eq_sweep_times
    msWrite2Byte(REG_112166 + wOffset, 0x001F); // reg_hdmi_symbol_det_duration

    if((enInputPort== MSCombo_TMDS_PORT_0) || (enInputPort== MSCombo_TMDS_PORT_1))
    {
        // PD initialize
        msWrite2ByteMask(REG_112104 + wOffset, 0x0000, 0x7777);
        msWrite2ByteMask(REG_112106 + wOffset, 0x0000, 0x0777);

        // DPLPH0/1/2 initialize
         msWrite2ByteMask(REG_112022 + wOffset, 0x000A, 0x1FFF); // [12]: reg_dvi_ovch0; [11:0]: reg_dvi_ch0_ph
         msWrite2ByteMask(REG_112024 + wOffset, 0x000A, 0x1FFF); // [12]: reg_dvi_ovch1; [11:0]: reg_dvi_ch1_ph
         msWrite2ByteMask(REG_112026 + wOffset, 0x000A, 0x1FFF); // [12]: reg_dvi_ovch2; [11:0]: reg_dvi_ch2_ph
         msWrite2ByteMask(REG_112160 + wOffset, 0x0000, 0x013F);
         msWrite2ByteMask(REG_11206A + wOffset, 0x0280, 0x17FF);
    }

    //auto clear enz for hit coundary
    msWriteByteMask(REG_11203B + wOffset, 0x00, 0x01);

    //digital lock part
    //msWriteByteMask(0x11208A + wOffset, 0x03, 0x0F); //DIG_LCK_CNT_PWR[3:0]
    //msWriteByte(0x11208E + wOffset, 0xE1); //DIG_LOSE_RANGE[7:4], DIG_LCK_RANGE[3:0]
    //msWriteByteMask(0x11208B + wOffset, 0x1F, 0x7F); //DIG_LOCK_TIME[6:0]


    // msWriteByteMask(REG_1606A3 + wOffset, 0x80, 0x80); //word boundary detect unlock check
    msWrite2Byte(0x1630EE, 0x1FFF); //clk to scaler

    if(enInputPort == MSCombo_TMDS_PORT_0)
    {
        msWriteByteMask(REG_16042F, BIT5, BIT5);// patch for HDCP issue, reset HDCP FSM when no input clock
        msWriteByteMask(REG_16050D, 0x0C, 0x0C); // [3] : enable video mute, [2] : blanking when AVmute is active.
        msWriteByteMask(REG_16050E, 0x03, 0x03); // [1] : auto reset deep color FIFO, [0] : enable deep color mode.
        msWriteByteMask(REG_160640, 0x00, 0x00); // [1] : disable force 422 to 444.
        msWrite2Byte(REG_1605C0, 0xFFFF); // HDMI pkt irq mask
        msWriteByteMask(REG_16042A, 0xFF, 0xFF); // HDCP pkt irq mask
        msWriteByteMask(REG_160247, 0x00, 0xFF); // word boundary unlock threshold
        msWriteByteMask(REG_120FDE, 0x01, 0x01); // [0] force clk_combo_0_rep_en
        msWriteByteMask(REG_1605C8, 0x80, 0xF0); // [7] Frame repetition manual mode
        msWriteByteMask(REG_112120, 0x07, 0x07); // eq overwrite enable
        msWrite2ByteMask(REG_112122, (TMDS_HDMI14_R_CHANNEL_EQ<<8)|(TMDS_HDMI14_G_CHANNEL_EQ<<4)|TMDS_HDMI14_B_CHANNEL_EQ, 0x0FFF);
    }
    else if(enInputPort == MSCombo_TMDS_PORT_1)
    {
        msWriteByteMask(REG_16092F, BIT5, BIT5);// patch for HDCP issue, reset HDCP FSM when no input clock
        msWriteByteMask(REG_160A0D, 0x0C, 0x0C); // [3] : enable video mute, [2] : blanking when AVmute is active.
        msWriteByteMask(REG_160A0E, 0x03, 0x03); // [1] : auto reset deep color FIFO, [0] : enable deep color mode.
        msWriteByteMask(REG_160B40, 0x00, 0x00); // [1] : disable force 422 to 444.
        msWrite2Byte(REG_160AC0, 0xFFFF); // HDMI pkt irq mask
        msWriteByteMask(REG_16092A, 0xFF, 0xFF); // HDCP pkt irq mask
        msWriteByteMask(REG_160747, 0x00, 0xFF); // word boundary unlock threshold
        msWriteByteMask(REG_120FDE, 0x02, 0x02); // [1] force clk_combo_1_rep_en
        msWriteByteMask(REG_160AC8, 0x80, 0xF0); // [7] Frame repetition manual mode
        msWriteByteMask(REG_112320, 0x07, 0x07); // eq overwrite enable
        msWrite2ByteMask(REG_112322, (TMDS_HDMI14_R_CHANNEL_EQ<<8)|(TMDS_HDMI14_G_CHANNEL_EQ<<4)|TMDS_HDMI14_B_CHANNEL_EQ, 0x0FFF);
    }
    else if((enInputPort == MSCombo_TMDS_PORT_2) || (enInputPort == MSCombo_TMDS_PORT_3))
    {
        msWriteByteMask(REG_16102F, BIT5, BIT5); // patch for HDCP issue, reset HDCP FSM when no input clock
        msWriteByteMask(REG_16110D, 0x0C, 0x0C); // [3] : enable video mute, [2] : blanking when AVmute is active.
        msWriteByteMask(REG_16110E, 0x03, 0x03); // [1] : auto reset deep color FIFO, [0] : enable deep color mode.
        msWriteByteMask(REG_161240, 0x00, 0x00); // [1] : disable force 422 to 444.
        msWrite2Byte(REG_1611C0, 0xFFFF); // HDMI pkt irq mask
        msWriteByteMask(REG_16102B, 0xFF, 0xFF); // HDCP pkt irq mask
        msWriteByteMask(REG_120FDE, 0x04, 0x04); // [2] force clk_combo_2_rep_en
        msWriteByteMask(REG_1611C8, 0x80, 0xF0); // [7] Frame repetition manual mode
        msWriteByteMask(REG_1611CF, 0x01, 0x01); // [8]: reg_cts_div2_en
        msWriteByteMask(REG_11257C, 0x04, 0x04); // [2]: SCDC bypass clock tmds ratio

        msWriteByteMask(REG_1124DC, 0xF0, 0xF0); // eq overwrite enable
        msWrite2ByteMask(REG_1124E6, (TMDS_HDMI20_G_CHANNEL_EQ<<8)|TMDS_HDMI20_B_CHANNEL_EQ, 0x1F1F);
        msWrite2ByteMask(REG_1124E8, (TMDS_HDMI20_CLK_CHANNEL_EQ<<8)|TMDS_HDMI20_R_CHANNEL_EQ, 0x1F1F);

        _mhal_tmds_HDMI20AutoEQSetting(TRUE);
    }




    //mask interrupt
   // msWriteByte(REG_112181 + wOffset, 0xFF); //combo irq mask wOffset!!!!!!





    //word boundary by RD suggest
   // msWriteByteMask(REG_16051D + wOffset, 0xFF, 0xFF);
   // msWriteByteMask(REG_160521 + wOffset, 0xFF, 0xFF);
   // msWriteByteMask(REG_160525 + wOffset, 0xFF, 0xFF);
   // msWriteByteMask(REG_160529 + wOffset, 0xFF, 0xFF);

    if(bDual)
    {
        switch (enInputPort)
        {
            case MSCombo_TMDS_PORT_0:
                mhal_tmds_Initial( (MSCombo_TMDS_PORT_INDEX)(enInputPort+1), FALSE);
                mhal_tmds_MaskComboIRQ((MSCombo_TMDS_PORT_INDEX)(enInputPort+1), FALSE);
                msWriteByteMask(REG_160764, 0x04, 0x04); // [2]: small DE
                msWriteByteMask(REG_160A0D, 0x00, 0x0C); // [11] : enable video mute = 0; [10] : blanking when AVmute is active = 0
                #if 0   //for HDCP main/sub signal sync setting
                    msWriteByteMask(REG_160633 + wOffset, 0, BIT7); //sub lane setting

                    wOffset = _mhal_tmds_bank_offset(enInputPort);
                    msWriteByteMask(REG_160633 + wOffset, 0, BIT7); //main land setting
                #endif
            break;
            case MSCombo_TMDS_PORT_1:
                mhal_tmds_Initial( (MSCombo_TMDS_PORT_INDEX)(enInputPort-1), FALSE);
                mhal_tmds_MaskComboIRQ((MSCombo_TMDS_PORT_INDEX)(enInputPort-1), FALSE);
                msWriteByteMask(REG_160264, 0x04, 0x04); // [2]: small DE
                msWriteByteMask(REG_16050D, 0x00, 0x0C); // [3] : enable video mute = 0; [2] : blanking when AVmute is active = 0
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

    // Disable SCDC clear status bit
    _mhal_tmds_ClearSCDCStatusFlag(enInputPort, TMDS_SCDC_ACCESS_ADDERSS_A8);

    if(!bReadEfuse)
    {
        _mhal_tmds_GetEfuseSettingValue();

        bReadEfuse = TRUE;
    }
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
    if((enInputPort == MSCombo_TMDS_PORT_0) || (enInputPort == MSCombo_TMDS_PORT_1))
        msWriteByteMask(REG_163071, (1 << (enInputPort + 1)), (1 << (enInputPort + 1))); //eanble ISR
    else if ((enInputPort == MSCombo_TMDS_PORT_2) || (enInputPort == MSCombo_TMDS_PORT_3))
        msWriteByteMask(REG_163071, (1 << 3), (1 << 3)); //eanble ISR
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
// combo_rx_top (1606)_2e -> combo_phy_1 (1121)_40[15:8]
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
//                  [D] : Gamut metadata packet
//                  [E] : HBR
//**************************************************************************
WORD mhal_tmds_HDMIGetPktStatus(MSCombo_TMDS_PORT_INDEX enInputPort)
{
    WORD temp = 0;

    if(enInputPort == MSCombo_TMDS_PORT_0)
    {
        temp = msRead2Byte(REG_160502);
        msWrite2Byte(REG_160502, 0xFFFF);
    }
    else if(enInputPort == MSCombo_TMDS_PORT_1)
    {
        temp = msRead2Byte(REG_160A02);
        msWrite2Byte(REG_160A02, 0xFFFF);
    }
    else if((enInputPort == MSCombo_TMDS_PORT_2) || (enInputPort == MSCombo_TMDS_PORT_3))
    {
        temp = msRead2Byte(REG_161102);
        msWrite2Byte(REG_161102, 0xFFFF);
    }

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
    BYTE temp = 0;

    if(enInputPort == MSCombo_TMDS_PORT_0)
    {
        temp = msReadByte(REG_160508);
        msWriteByteMask(REG_160508, 0xFF, 0xFF);
    }
    else if(enInputPort == MSCombo_TMDS_PORT_1)
    {
        temp = msRead2Byte(REG_160A08);
        msWriteByteMask(REG_160A08, 0xFF, 0xFF);
    }
    else if((enInputPort == MSCombo_TMDS_PORT_2) || (enInputPort == MSCombo_TMDS_PORT_3))
    {
        temp = msRead2Byte(REG_161108);
        msWriteByteMask(REG_161108, 0xFF, 0xFF);
    }

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
   if(enInputPort == MSCombo_TMDS_PORT_0)
   {
       if(msReadByte(REG_160262) & 0x40)
           return TRUE;
       else
           return FALSE;
   }
   else if(enInputPort == MSCombo_TMDS_PORT_1)
   {
       if(msReadByte(REG_160762) & 0x40)
           return TRUE;
       else
           return FALSE;
   }
   else if((enInputPort == MSCombo_TMDS_PORT_2) || (enInputPort == MSCombo_TMDS_PORT_3))
   {
       if(msReadByte(REG_160C62) & 0x40)
           return TRUE;
       else
           return FALSE;
   }
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

    wOffset = _mhal_tmds_phy_offset(enInputPort);

    if(msRead2Byte(REG_112064 + wOffset) & 0x8888)
        return TRUE;
    else
        return FALSE;
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
   if(enInputPort == MSCombo_TMDS_PORT_0)
   {
       if(msReadByte(REG_160402) & 0x01)
           return 1;
       else
           return 0;
   }
   else if(enInputPort == MSCombo_TMDS_PORT_1)
   {
       if(msReadByte(REG_160902) & 0x01)
           return 1;
       else
           return 0;
   }
   else if((enInputPort == MSCombo_TMDS_PORT_2) || (enInputPort == MSCombo_TMDS_PORT_3))
   {
       if(msReadByte(REG_161002) & 0x01)
           return 1;
       else
           return 0;
   }
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
    if(enInputPort == MSCombo_TMDS_PORT_0)
    {
        msWriteByteMask(REG_160762, 0x02, 0x02); // reset P1
        msWriteByteMask(REG_160762, 0x00, 0x02);
    }
    else if(enInputPort == MSCombo_TMDS_PORT_1)
    {
        msWriteByteMask(REG_160262, 0x02, 0x02); // reset P0
        msWriteByteMask(REG_160262, 0x00, 0x02);
    }
    else
    {
        TMDS_PRINT("Port: 0x%x, no need\r\n", enInputPort);
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_tmds_DualLink_ErrChk()
//  [Description]:
//
//  [Arguments]:
//                  enInputPort : combo port
//  [Return]:
//                  TRUE = unlock
//                  FALSE = lock
//**************************************************************************
Bool _mhal_tmds_DualLink_ErrChk(MSCombo_TMDS_PORT_INDEX enInputPort)
{
    BYTE i = 0;

    if(enInputPort == MSCombo_TMDS_PORT_0)
	   msWriteByteMask(REG_160256, 0x80, 0x80);
    else
       msWriteByteMask(REG_160756, 0x80, 0x80);

    for ( i=0; i < 200; i++ )
    {
        if(enInputPort == MSCombo_TMDS_PORT_0)
        {
            if(msReadByte(REG_160256) & 0x80)
                return TRUE;
		}
        else
        {
            if(msReadByte(REG_160756) & 0x80)
                return TRUE;
		}
    }
    return FALSE;
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
    BYTE i, debounceCnt = 30;
    BYTE status = 0;
	if(enInputPort == MSCombo_TMDS_PORT_0)
	{
		status = msReadByte(REG_160256);
	}
	else if(enInputPort == MSCombo_TMDS_PORT_1)
	{
		status = msReadByte(REG_160756);
	}

    if(status & 0x02)
    {
        for( i=0; i < debounceCnt; i++ )
        {
            if(_mhal_tmds_DualLink_ErrChk(enInputPort))
            {
    			msWriteByteMask(REG_11201C, 0x01, 0x01);
    			msWriteByteMask(REG_11201C, 0x00, 0x01);
    			msWriteByteMask(REG_11221C, 0x01, 0x01);
    			msWriteByteMask(REG_11221C, 0x00, 0x01);
    		}
    	}
    }
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
    WORD wTmpValue = 0;
    BYTE i, debounceCnt = 20;
    Bool isDual = TRUE;

    //solution for dual link cable coupling issue;
    for ( i=0; i < debounceCnt; i++ )
    {
        if(enInputPort == MSCombo_TMDS_PORT_0)
        {
            wTmpValue = msReadByte(REG_160762);
        }
        else if(enInputPort == MSCombo_TMDS_PORT_1)
        {
            wTmpValue = msReadByte(REG_160262);
        }

        if (!(wTmpValue & 0x40))
        {
            isDual = FALSE;
            break;
        }
		else if(abs(msRead2Byte(REG_160266) - msRead2Byte(REG_160766)) > 10 )
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
    if(enInputPort == MSCombo_TMDS_PORT_0)
    {
        msWriteByteMask(REG_160262, 0x02, 0x02); // reset P0
        msWriteByteMask(REG_160262, 0x00, 0x02);
    }
    else if(enInputPort == MSCombo_TMDS_PORT_1)
    {
        msWriteByteMask(REG_160762, 0x02, 0x02); // reset P1
        msWriteByteMask(REG_160762, 0x00, 0x02);
    }
    else
    {
        TMDS_PRINT("Port: 0x%x, no need\r\n", enInputPort);
    }
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
    WORD wTmpValue = 0;
    BYTE i, debounceCnt = 10;
    Bool isStable= TRUE;

    //solution for dual link cable coupling issue;
    for ( i=0; i < debounceCnt; i++ )
    {
        if(enInputPort == MSCombo_TMDS_PORT_0)
        {
            wTmpValue = msRead2Byte(REG_160262);
        }
        else if(enInputPort == MSCombo_TMDS_PORT_1)
        {
            wTmpValue = msRead2Byte(REG_160762);
        }

        if (!(wTmpValue & 0x40))
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
  //  wOffset = _mhal_tmds_phy_offset(enInputPort);

    switch (enInputPort)
    {
        case MSCombo_TMDS_PORT_0:
            msWriteByteMask(REG_162F34, bFlag?0x02:0x00, 0x03);
            bIsSwapped = FALSE;
        break;
        case MSCombo_TMDS_PORT_1:
            msWriteByteMask(REG_162F34, bFlag?0x02:0x00, 0x03);
            bIsSwapped = TRUE;
        break;
        default:
            //invalid port index, return to prevent the code from accessing illegal register address
            return;
        break;
    }

  //  if(msReadByte(REG_112160) & 0x08)
  //  {
      //  msWrite2Byte(REG_1120D6 + wOffset, bFlag ? 0x0707 : 0x0702); //Main:[0]: Enable dual clk; [2]: 2x Pixel clk
      //  msWrite2Byte(REG_112014 + wOffset, 0x0040); // [6]: reg_af_ls_20out_sel=1
      //  msWrite2Byte(REG_1120B4 + wOffset, 0x0000); // [2]: disable reg_atop_en_clko_tmds2x
      //  msWrite2Byte(REG_112108 + wOffset, bFlag ? 0x0001 : 0x0000); // [0]: reg_atop_en_clkpix2x
  //  }
  //  else
  //  {
  //      msWrite2Byte(REG_1120D6, bFlag ? 0x0706 : 0x0702); //Main:[0]: Enable dual clk; [2]: 2x Pixel clk
 //       msWrite2Byte(REG_112014, 0x0040); // [6]: reg_af_ls_20out_sel=1
 //       msWrite2Byte(REG_1120B4, 0x0004); // [2]: disable reg_atop_en_clko_tmds2x
 //   }

    //main port is 0
    if ( !bIsSwapped )
    {
        // main port 0
        wOffset = _mhal_tmds_phy_offset(enInputPort);

        msWrite2Byte(REG_1120D6 + wOffset, bFlag ? 0x0707 : 0x0702); //Main:[0]: Enable dual clk; [2]: 2x Pixel clk
        msWrite2Byte(REG_112014 + wOffset, 0x0040); // [6]: reg_af_ls_20out_sel=1
        msWrite2Byte(REG_1120B4 + wOffset, 0x0000); // [2]: disable reg_atop_en_clko_tmds2x
        msWrite2Byte(REG_112108 + wOffset, bFlag ? 0x0001 : 0x0000); // [0]: reg_atop_en_clkpix2x

        // sub port 1
        wOffset = _mhal_tmds_phy_offset((MSCombo_TMDS_PORT_INDEX)(enInputPort + 1));

        msWrite2Byte(REG_1120D6 + wOffset, bFlag ? 0x0706 : 0x0000); //Main:[0]: Enable dual clk; [2]: 2x Pixel clk
        msWrite2Byte(REG_112014 + wOffset, 0x0040); // [6]: reg_af_ls_20out_sel=1
        msWrite2Byte(REG_1120B4 + wOffset, 0x0002); // [2]: disable reg_atop_en_clko_tmds2x
        msWrite2Byte(REG_112108 + wOffset, bFlag ? 0x0001 : 0x0000); // [0]: reg_atop_en_clkpix2x

        msWriteByteMask(REG_163021, bFlag ? 0x00 : 0, 0x20); // Select main port0
        msWriteByteMask(REG_163041, bFlag ? 0x20 : 0, 0x20);
        msWriteByteMask(REG_160256, bFlag ? 0x02 : 0, 0x0F); // Select cycle align port0
        msWriteByteMask(REG_160756, bFlag ? 0x00 : 0, 0x0F);
        msWriteByteMask(REG_163066, bFlag ? 0x01 : 0, 0x03); // Select ddc main port0 and x76 for port1
        msWriteByteMask(REG_160406, bFlag ? 0x00 : 0, 0x80);
        msWriteByteMask(REG_160906, bFlag ? 0x80 : 0, 0x80);
        msWriteByteMask(REG_1606C0, bFlag ? 0x01 : 0, 0x01); // Select output port0
        msWriteByteMask(REG_160756, bFlag ? 0x01 : 0, 0x01); // Switch dvi decoder data input / hdcp ctrl2_0 from dual-link sub-channel
#if 0
        // Set Port0
        msWriteByteMask(REG_1120D6,0x07, 0x07); // [0]: reg_atop_en_clko_dvidual; [1]: reg_atop_en_clk_pix; [2]: reg_atop_en_clk_pix_2x
        msWriteByteMask(REG_112014,0x40, 0x40); // [6]: reg_af_ls_20out_sel

        // Set Port1
        msWriteByteMask(REG_1122D6,0x06, 0x07); // [0]: reg_atop_en_clko_dvidual; [1]: reg_atop_en_clk_pix; [2]: reg_atop_en_clk_pix_2x
        msWriteByteMask(REG_112214,0x40, 0x40); // [6]: reg_af_ls_20out_sel
        msWriteByteMask(REG_1122B4,0x02, 0x02); // [1]: reg_atop_en_clki_dvidual
#endif
#if 0
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
        msWriteByteMask(REG_1630F7, bFlag ? (1 << (enInputPort + 3)): 0, 0xFC); // according to "SUB" port, switch lane clock
        //msWriteByteMask(REG_162F34, (1 << (enInputPort + 1)), 0x1F);

        mhal_tmds_DualLink_ComboMuxClk(enInputPort, enInputPort);
#endif
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
    //main port is 1
    else
    {
        // main port 1
        wOffset = _mhal_tmds_phy_offset(enInputPort);

        msWrite2Byte(REG_1120D6 + wOffset, bFlag ? 0x0707 : 0x0702); //Main:[0]: Enable dual clk; [2]: 2x Pixel clk
        msWrite2Byte(REG_112014 + wOffset, 0x0040); // [6]: reg_af_ls_20out_sel=1
        msWrite2Byte(REG_1120B4 + wOffset, 0x0000); // [2]: disable reg_atop_en_clko_tmds2x
        msWrite2Byte(REG_112108 + wOffset, bFlag ? 0x0001 : 0x0000); // [0]: reg_atop_en_clkpix2x

        // sub port 0
        wOffset = _mhal_tmds_phy_offset((MSCombo_TMDS_PORT_INDEX)(enInputPort - 1));

        msWrite2Byte(REG_1120D6 + wOffset, bFlag ? 0x0706 : 0x0000); //Main:[0]: Enable dual clk; [2]: 2x Pixel clk
        msWrite2Byte(REG_112014 + wOffset, 0x0040); // [6]: reg_af_ls_20out_sel=1
        msWrite2Byte(REG_1120B4 + wOffset, 0x0002); // [2]: disable reg_atop_en_clko_tmds2x
        msWrite2Byte(REG_112108 + wOffset, bFlag ? 0x0001 : 0x0000); // [0]: reg_atop_en_clkpix2x

        msWriteByteMask(REG_163021, bFlag ? 0x20 : 0, 0x20); // Select main port1
        msWriteByteMask(REG_163041, bFlag ? 0x00 : 0, 0x20);
        msWriteByteMask(REG_160256, bFlag ? 0x00 : 0, 0x0F); // Select cycle align port1
        msWriteByteMask(REG_160756, bFlag ? 0x02 : 0, 0x0F);
        msWriteByteMask(REG_163066, bFlag ? 0x02 : 0, 0x03); // Select ddc main port1 and x76 for port0
        msWriteByteMask(REG_160406, bFlag ? 0x80 : 0, 0x80);
        msWriteByteMask(REG_160906, bFlag ? 0x00 : 0, 0x80);
        msWriteByteMask(REG_160BC0, bFlag ? 0x01 : 0, 0x01); // Select output port1
        msWriteByteMask(REG_160256, bFlag ? 0x01 : 0, 0x01); // Switch dvi decoder data input / hdcp ctrl2_0 from dual-link sub-channel
#if 0
        // Set Port1
        msWriteByteMask(REG_1122D6,0x07, 0x07); // [0]: reg_atop_en_clko_dvidual; [1]: reg_atop_en_clk_pix; [2]: reg_atop_en_clk_pix_2x
        msWriteByteMask(REG_112214,0x40, 0x40); // [6]: reg_af_ls_20out_sel

        // Set Port0
        msWriteByteMask(REG_1120D6,0x06, 0x07); // [0]: reg_atop_en_clko_dvidual; [1]: reg_atop_en_clk_pix; [2]: reg_atop_en_clk_pix_2x
        msWriteByteMask(REG_112014,0x40, 0x40); // [6]: reg_af_ls_20out_sel
        msWriteByteMask(REG_1120B4,0x02, 0x02); // [1]: reg_atop_en_clki_dvidual
#endif
#if 0
        //[Main Port] Settings
        wOffset = _mhal_tmds_bank_offset(enInputPort);

        //if MST9U2
        msWriteByteMask(REG_160513 + wOffset, bFlag ? 0x20 : 0, (BIT4|BIT5)); //BK:TOP, main enable dual link data in
        //else
        //msWriteByteMask(REG_160513 + wOffset, bFlag ? 0x20 : 0, 0x20); //BK:TOP, main enable dual link data in
        msWriteByteMask(REG_160633 + wOffset, bFlag ? 0x40 : 0, 0x40); //main, dual fifo
        msWriteByteMask(REG_160632 + wOffset, bFlag ? 0x10 : 0, 0x10); //enable of dual out FIFO

        //[Sub Port] Settings
        wOffset = _mhal_tmds_bank_offset((MSCombo_TMDS_PORT_INDEX)(enInputPort - 1));

        msWriteByteMask(REG_160633 + wOffset, 0x20, 0x20);
        msWriteByteMask(REG_160513 + wOffset, bFlag ? 0x10 : 0, 0x30); //BK: TOP, sub: enable dual link data out

        //[Index Relative] Settings
        // 3: start from BIT2, and ((Sub Port Index) = (Main Port Index) - 1), so index should be (Main Port Index - 1).
        //msWriteByteMask(REG_1630F7, bFlag ? (1 << (enInputPort + 1)): 0, 0xFC); // according to "SUB" port, switch lane clock
        //msWriteByteMask(REG_162F34, (1 << (enInputPort)), 0x1F);

        //adding for MST9U 2
        if(bFlag)
            mhal_tmds_DualLink_ComboMuxClk(enInputPort, (MSCombo_TMDS_PORT_INDEX)(enInputPort - 1));
        else
            mhal_tmds_DualLink_ComboMuxClk(enInputPort, enInputPort);

        //2: start from BIT2;
        msWriteByteMask(REG_1630F7, bFlag ? (1 << (enInputPort + 2)) : 0, 0xFC); //main port index;

        msWriteByteMask(REG_160632 + wOffset, bFlag ? BIT4 : 0, BIT4);
        msWriteByteMask(REG_160633 + wOffset, bFlag ? (BIT4|BIT5) : BIT5, (BIT4|BIT5|BIT6|BIT7));
        //msWrite2ByteMask(REG_160632 + wOffset, 0x7310, 0x7310);
        //msWrite2ByteMask(REG_160632 + wOffset, bFlag ? 0x7310 : 0x6380, 0xFFFF);
#endif
    }
}


//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_Link_LockClear()
//  [Description]:
//                 do nothing.
//  [Arguments]:
//
//  [Return]:
//                  None
//**************************************************************************
void mhal_tmds_Link_LockClear(MSCombo_TMDS_PORT_INDEX enInputPort)
{
    if(enInputPort!=0){}
    // do nothing
}

Bool mhal_tmds_Link_LockCheck(MSCombo_TMDS_PORT_INDEX enInputPort, Bool bFlag)
{
    if(enInputPort!=0 && bFlag!=0){}
#if 0
    BYTE i;
    WORD wOffset;
    WORD wTmpValue;

    wOffset = _mhal_tmds_bank_offset(enInputPort);
    for(i=0;i<5;i++)
    {
        wTmpValue = msRead2Byte(REG_1607C4 + wOffset);
        if(wTmpValue & BIT7)
            break;
        else
        {
            switch (enInputPort)
            {
                case MSCombo_TMDS_PORT_0:
                    msWriteByteMask(REG_162F34, 0x00, 0x03);
                    msWriteByteMask(REG_162F34, 0x02, 0x03);
                break;
                case MSCombo_TMDS_PORT_2:
                    msWriteByteMask(REG_162F34, 0x00, 0x0C);
                    msWriteByteMask(REG_162F34, 0x08, 0x0C);
                break;
                case MSCombo_TMDS_PORT_1:
                    msWriteByteMask(REG_162F34, 0x00, 0x03);
                    msWriteByteMask(REG_162F34, 0x02, 0x03);
                break;
                case MSCombo_TMDS_PORT_3:
                    msWriteByteMask(REG_162F34, 0x00, 0x0C);
                    msWriteByteMask(REG_162F34, 0x08, 0x0C);
                break;
                default:
                    //invalid port index, return to prevent the code from accessing illegal register address
                    return;
                break;
            }

        }
    }
#endif
    return TRUE;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_PowerControl()
//  [Description]:
//                  use to control power
//  [Arguments]:
//                  enInputPort : combo port
//                  bPowerOn : 1 -> power on; 0 -> power down
//                  bCLKDET: 1 -> do clock detection; 0 -> not do clock detection
//  [Return]:
//                  None
//**************************************************************************
void mhal_tmds_PowerControl(MSCombo_TMDS_PORT_INDEX enInputPort, Bool bPowerOn, Bool bCLKDET)
{
    static Bool bPowerOnPort2 = FALSE;
    static Bool bPowerOnPort3 = FALSE;

    if(enInputPort == MSCombo_TMDS_PORT_0)
    {
        if(bPowerOn) // Normal
        {
            msWriteByteMask(REG_112106, 0, MASKBIT(2:0)); //reg_atop_pd_dm
            msWriteByteMask(REG_001724, 0, BIT(3)); //reg_p0_pd_bg
            msWriteByteMask(REG_112105, 0, BIT(5)); //reg_atop_pd_dvipll
            msWriteByteMask(REG_112105, 0, BIT(4)); //reg_atop_pd_reg
            msWriteByteMask(REG_112104, 0, MASKBIT(6:4)); //reg_atop_pd_dplphi
            msWriteByteMask(REG_112105, 0, MASKBIT(2:0)); //reg_atop_pd_dplphq
            msWriteByteMask(REG_001724, 0, BIT(6)); //reg_p0_clkin_offl
            msWriteByteMask(REG_001724, 0, BIT(5)); //reg_p0_clkin
        }
		else // bCLKDET = 1: Standby; bCLKDET = 0: DC-off
		{
            msWriteByteMask(REG_112106, MASKBIT(2:0), MASKBIT(2:0)); //reg_atop_pd_dm
            msWriteByteMask(REG_001724, bCLKDET ? 0:BIT(3), BIT(3)); //reg_p0_pd_bg
            msWriteByteMask(REG_112105, BIT(5), BIT(5)); //reg_atop_pd_dvipll
            msWriteByteMask(REG_112105, BIT(4), BIT(4)); //reg_atop_pd_reg
            msWriteByteMask(REG_112104, MASKBIT(6:4), MASKBIT(6:4)); //reg_atop_pd_dplphi
            msWriteByteMask(REG_112105, MASKBIT(2:0), MASKBIT(2:0)); //reg_atop_pd_dplphq
            msWriteByteMask(REG_001724, bCLKDET ? 0:BIT(6), BIT(6)); //reg_p0_clkin_offl
            msWriteByteMask(REG_001724, BIT(5), BIT(5)); //reg_p0_clkin
		}
    }
    else if(enInputPort == MSCombo_TMDS_PORT_1)
    {
        if(bPowerOn) // Normal
        {
            msWriteByteMask(REG_112306, 0, MASKBIT(2:0)); //reg_atop_pd_dm
            msWriteByteMask(REG_001726, 0, BIT(3)); //reg_p0_pd_bg
            msWriteByteMask(REG_112305, 0, BIT(5)); //reg_atop_pd_dvipll
            msWriteByteMask(REG_112305, 0, BIT(4)); //reg_atop_pd_reg
            msWriteByteMask(REG_112304, 0, MASKBIT(6:4)); //reg_atop_pd_dplphi
            msWriteByteMask(REG_112305, 0, MASKBIT(2:0)); //reg_atop_pd_dplphq
            msWriteByteMask(REG_001726, 0, BIT(6)); //reg_p0_clkin_offl
            msWriteByteMask(REG_001726, 0, BIT(5)); //reg_p0_clkin
        }
		else // bCLKDET = 1: Standby; bCLKDET = 0: DC-off
		{
            msWriteByteMask(REG_112306, MASKBIT(2:0), MASKBIT(2:0)); //reg_atop_pd_dm
            msWriteByteMask(REG_001726, bCLKDET ? 0:BIT(3), BIT(3)); //reg_p0_pd_bg
            msWriteByteMask(REG_112305, BIT(5), BIT(5)); //reg_atop_pd_dvipll
            msWriteByteMask(REG_112305, BIT(4), BIT(4)); //reg_atop_pd_reg
            msWriteByteMask(REG_112304, MASKBIT(6:4), MASKBIT(6:4)); //reg_atop_pd_dplphi
            msWriteByteMask(REG_112305, MASKBIT(2:0), MASKBIT(2:0)); //reg_atop_pd_dplphq
            msWriteByteMask(REG_001726, bCLKDET ? 0:BIT(6), BIT(6)); //reg_p0_clkin_offl
            msWriteByteMask(REG_001726, BIT(5), BIT(5)); //reg_p0_clkin
		}
    }
    else if(enInputPort == MSCombo_TMDS_PORT_2)
    {
        if(bPowerOn) // Normal
        {
            bPowerOnPort2 = TRUE;
            msWriteByteMask(REG_112500, 0, BIT(3)); //reg_atop_pd_ldo
		    msWriteByteMask(REG_1124B7, 0, BIT(4)); //reg_atop_pd_clkin
		    msWriteByteMask(REG_1124B7, 0, BIT(5)); //reg_atop_pd_bg
		    msWriteByteMask(REG_1124B6, 0, MASKBIT(3:0)); //reg_atop_pd_lane
		    msWriteByteMask(REG_1124B2, 0, BIT(1)); //en_pd_phdac_ove
		    msWriteByteMask(REG_1124B7, 0, MASKBIT(2:0)); // reg_atop_pd_phdac_ov
		    msWriteByteMask(REG_001720, 0, BIT(4)); //reg_p0_nodie_pd_clkin
        }
		else // bCLKDET = 1: Standby; bCLKDET = 0: DC-off
		{
            bPowerOnPort2 = FALSE;

			if((!bPowerOnPort2) && (!bPowerOnPort3))
			{
				msWriteByteMask(REG_112500, BIT(3), BIT(3)); //reg_atop_pd_ldo
				msWriteByteMask(REG_1124B7, BIT(4), BIT(4)); //reg_atop_pd_clkin
				msWriteByteMask(REG_1124B7, BIT(5), BIT(5)); //reg_atop_pd_bg
				msWriteByteMask(REG_1124B6, MASKBIT(3:0), MASKBIT(3:0)); //reg_atop_pd_lane
				msWriteByteMask(REG_1124B2, BIT(1), BIT(1)); //en_pd_phdac_ove
				msWriteByteMask(REG_1124B7, MASKBIT(2:0), MASKBIT(2:0)); // reg_atop_pd_phdac_ov
			}
		    msWriteByteMask(REG_001720, bCLKDET ? 0:BIT(4), BIT(4)); //reg_p0_nodie_pd_clkin
		}
    }
    else if(enInputPort == MSCombo_TMDS_PORT_3)
    {
        if(bPowerOn) // Normal
        {
            bPowerOnPort3 = TRUE;
            msWriteByteMask(REG_112500, 0, BIT(3)); //reg_atop_pd_ldo
		    msWriteByteMask(REG_1124B7, 0, BIT(4)); //reg_atop_pd_clkin
		    msWriteByteMask(REG_1124B7, 0, BIT(5)); //reg_atop_pd_bg
		    msWriteByteMask(REG_1124B6, 0, MASKBIT(3:0)); //reg_atop_pd_lane
		    msWriteByteMask(REG_1124B2, 0, BIT(1)); //en_pd_phdac_ove
		    msWriteByteMask(REG_1124B7, 0, MASKBIT(2:0)); // reg_atop_pd_phdac_ov
		    msWriteByteMask(REG_001722, 0, BIT(4)); //reg_p0_nodie_pd_clkin
        }
		else // bCLKDET = 1: Standby; bCLKDET = 0: DC-off
		{
            bPowerOnPort3 = FALSE;

			if((!bPowerOnPort2) && (!bPowerOnPort3))
			{
				msWriteByteMask(REG_112500, BIT(3), BIT(3)); //reg_atop_pd_ldo
				msWriteByteMask(REG_1124B7, BIT(4), BIT(4)); //reg_atop_pd_clkin
				msWriteByteMask(REG_1124B7, BIT(5), BIT(5)); //reg_atop_pd_bg
				msWriteByteMask(REG_1124B6, MASKBIT(3:0), MASKBIT(3:0)); //reg_atop_pd_lane
				msWriteByteMask(REG_1124B2, BIT(1), BIT(1)); //en_pd_phdac_ove
				msWriteByteMask(REG_1124B7, MASKBIT(2:0), MASKBIT(2:0)); // reg_atop_pd_phdac_ov
			}
		    msWriteByteMask(REG_001722, bCLKDET ? 0:BIT(4), BIT(4)); //reg_p0_nodie_pd_clkin
		}
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_TimerInterrupt()
//  [Description]:
//                 do nothing.
//  [Arguments]:
//
//  [Return]:
//                  None
//**************************************************************************
void mhal_tmds_TimerInterrupt(void)
{
    // do nothing.
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

        _mhal_tmds_ResetHDMI20VersionFlag(enInputPort);
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
    if(enInputPort == MSCombo_TMDS_PORT_0)
    {
        if(bFlag)
        {
            msWriteByteMask(REG_1605BF, 0x80, 0x80); //reset packet content
            msWriteByteMask(REG_1605BF, 0x00, 0x80);
        }
        msWriteByteMask(REG_1605BF, ucReset, 0xFF); //reset packet status
        msWriteByteMask(REG_1605BF, 0, 0xFF);
    }
    else if(enInputPort == MSCombo_TMDS_PORT_1)
    {
        if(bFlag)
        {
            msWriteByteMask(REG_160ABF, 0x80, 0x80); //reset packet content
            msWriteByteMask(REG_160ABF, 0x00, 0x80);
        }
        msWriteByteMask(REG_160ABF, ucReset, 0xFF); //reset packet status
        msWriteByteMask(REG_160ABF, 0, 0xFF);
    }
    else if((enInputPort == MSCombo_TMDS_PORT_2) || (enInputPort == MSCombo_TMDS_PORT_3))
    {
        if(bFlag)
        {
            msWriteByteMask(REG_1611BF, 0x80, 0x80); //reset packet content
            msWriteByteMask(REG_1611BF, 0x00, 0x80);
        }
        msWriteByteMask(REG_1611BF, ucReset, 0xFF); //reset packet status
        msWriteByteMask(REG_1611BF, 0, 0xFF);
    }
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

    wOffset = _mhal_tmds_phy_offset(enInputPort);

    if(msReadByte(REG_112180 + wOffset) & 0x08) // no input clock
    {
       // printf("no input ohohohohohohohohohohoh\n");

       // msWriteByteMask(REG_112183 + wOffset, 0x08, 0x08); //clr
        msWriteByte(REG_112183 + wOffset, 0x08); //clr

       // msWriteByteMask(REG_112183 + wOffset, 0x00, 0x08);
        msWriteByte(REG_112183 + wOffset, 0x00);

       // msWriteByteMask(REG_112183 + wOffset, 0x00, 0x08);
        msWriteByte(REG_112183 + wOffset, 0x00);

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

    wOffset = _mhal_tmds_phy_offset(enInputPort);

    if(msReadByte(REG_112180 + wOffset) & 0x40) // clock big change
    {
      //  msWriteByteMask(REG_112183 + wOffset, 0x40, 0x40); //clr
        msWriteByte(REG_112183 + wOffset, 0x40); //clr

      //  msWriteByteMask(REG_112183 + wOffset, 0x00, 0x40);
        msWriteByte(REG_112183 + wOffset, 0x00);

      //  msWriteByteMask(REG_112183 + wOffset, 0x00, 0x40);
        msWriteByte(REG_112183 + wOffset, 0x00);

        switch(enInputPort)
        {
            case MSCombo_TMDS_PORT_0:
                msWriteByteMask(REG_160437, 0x02, 0x02); //clear HDCP encryption status.
            break;

            case MSCombo_TMDS_PORT_1:
                msWriteByteMask(REG_160937, 0x02, 0x02); //clear HDCP encryption status.
            break;

            case MSCombo_TMDS_PORT_2:
            case MSCombo_TMDS_PORT_3:
                msWriteByteMask(REG_161037, 0x02, 0x02); //clear HDCP encryption status.
            break;

            default:

            break;
        };
        return TRUE;
    }
    else
        return FALSE;
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
    BYTE Y_value = 0;
    BYTE C_value = 0;
    BYTE Q_value = 0;

    //assign initial value
    ST_COMBO_COLOR_FORMAT ColorInfo = {COMBO_COLOR_FORMAT_DEFAULT, COMBO_COLOR_RANGE_DEFAULT, COMBO_COLORIMETRY_NONE, COMBO_YUV_COLORIMETRY_ITU601};

    //parsing AVI info frame packet
    if(enInputPort == MSCombo_TMDS_PORT_0)
    {
        Y_value = ((msReadByte(REG_160580) & (BIT6|BIT5)) >> 5 ); // [6:5]: RGB or YCbCr
        C_value = ((msReadByte(REG_160581) & (BIT7|BIT6)) >> 6 ); // [7:6]: Colorimetry
    }
    else if(enInputPort == MSCombo_TMDS_PORT_1)
    {
        Y_value = ((msReadByte(REG_160A80) & (BIT6|BIT5)) >> 5 ); // [6:5]: RGB or YCbCr
        C_value = ((msReadByte(REG_160A81) & (BIT7|BIT6)) >> 6 ); // [7:6]: Colorimetry
    }
    else if((enInputPort == MSCombo_TMDS_PORT_2) || (enInputPort == MSCombo_TMDS_PORT_3))
    {
        Y_value = ((msReadByte(REG_161180) & (BIT6|BIT5)) >> 5 ); // [6:5]: RGB or YCbCr
        C_value = ((msReadByte(REG_161181) & (BIT7|BIT6)) >> 6 ); // [7:6]: Colorimetry
    }

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
        case 3:
            ColorInfo.ucColorType = COMBO_COLOR_FORMAT_YUV_420;
        break;
        default:
            ColorInfo.ucColorType = COMBO_COLOR_FORMAT_DEFAULT;
        break;
    }

    if((Y_value == 1) || (Y_value == 2)|| (Y_value == 3))
    {
        // [7:6]: Full or limit (for YCC)
        if(enInputPort == MSCombo_TMDS_PORT_0)
            Q_value = ((msReadByte(REG_160584) & (BIT7|BIT6)) >> 6 );

        else if(enInputPort == MSCombo_TMDS_PORT_1)
            Q_value = ((msReadByte(REG_160A84) & (BIT7|BIT6)) >> 6 );

        else if((enInputPort == MSCombo_TMDS_PORT_2) || (enInputPort == MSCombo_TMDS_PORT_3))
            Q_value = ((msReadByte(REG_161184) & (BIT7|BIT6)) >> 6 );

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
        // [3:2]: Full or limit (for RGB)
        if(enInputPort == MSCombo_TMDS_PORT_0)
            Q_value = ((msReadByte(REG_160582) & (BIT3|BIT2)) >> 2 );

        else if(enInputPort == MSCombo_TMDS_PORT_1)
            Q_value = ((msReadByte(REG_160A82) & (BIT3|BIT2)) >> 2 );

        else if((enInputPort == MSCombo_TMDS_PORT_2) || (enInputPort == MSCombo_TMDS_PORT_3))
            Q_value = ((msReadByte(REG_161182) & (BIT3|BIT2)) >> 2 );

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

    // [5:4] extended colorimetry
    if (C_value == 0x03)
    {
        BYTE EC_value = 0;

        if(enInputPort == MSCombo_TMDS_PORT_0)
            Q_value = ((msReadByte(REG_160582) & (BIT5|BIT4)) >> 4  );

        else if(enInputPort == MSCombo_TMDS_PORT_1)
            Q_value = ((msReadByte(REG_160A82) & (BIT5|BIT4)) >> 4  );

        else if((enInputPort == MSCombo_TMDS_PORT_2) || (enInputPort == MSCombo_TMDS_PORT_3))
            Q_value = ((msReadByte(REG_161182) & (BIT5|BIT4)) >> 4  );

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
    if(enInputPort == MSCombo_TMDS_PORT_0)
    {
        if(msReadByte(REG_160582) & BIT7)
            return 1;
        else
            return 0;
    }
    else if(enInputPort == MSCombo_TMDS_PORT_1)
    {
        if(msReadByte(REG_160A82) & BIT7)
            return 1;
        else
            return 0;
    }
    else if((enInputPort == MSCombo_TMDS_PORT_2) || (enInputPort == MSCombo_TMDS_PORT_3))
    {
        if(msReadByte(REG_161182) & BIT7)
            return 1;
        else
            return 0;
    }
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
    if(enInputPort == MSCombo_TMDS_PORT_0)
        return (msReadByte(REG_160580) & (BIT1|BIT0));

    else if(enInputPort == MSCombo_TMDS_PORT_1)
        return (msReadByte(REG_160A80) & (BIT1|BIT0));

    else if((enInputPort == MSCombo_TMDS_PORT_2) || (enInputPort == MSCombo_TMDS_PORT_3))
        return (msReadByte(REG_161180) & (BIT1|BIT0));

    return 0;
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
    BYTE i;
    BYTE DataArray[ucPacketLength];
    BYTE MaxCntLength = 0;

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

                if(enInputPort == MSCombo_TMDS_PORT_0)
                {
                    SB[1] = msReadByte(REG_160513) & 0x0F; // CTS[19:16]
                    SB[2] = msReadByte(REG_160514); // CTS[15:8]
                    SB[3] = msReadByte(REG_160515); // CTS[7:0]
                    SB[4] = (msReadByte(REG_160513) & 0xF0) >> 4; // N[19:16]
                    SB[5] = msReadByte(REG_160516); // N[15:8]
                    SB[6] = msReadByte(REG_160517); // N[7:0]
                }
                else if(enInputPort == MSCombo_TMDS_PORT_1)
                {
                    SB[1] = msReadByte(REG_160A13) & 0x0F; // CTS[19:16]
                    SB[2] = msReadByte(REG_160A14); // CTS[15:8]
                    SB[3] = msReadByte(REG_160A15); // CTS[7:0]
                    SB[4] = (msReadByte(REG_160A13) & 0xF0) >> 4; // N[19:16]
                    SB[5] = msReadByte(REG_160A16); // N[15:8]
                    SB[6] = msReadByte(REG_160A17); // N[7:0]
                }
                else if((enInputPort == MSCombo_TMDS_PORT_2) || (enInputPort == MSCombo_TMDS_PORT_3))
                {
                    SB[1] = msReadByte(REG_161113) & 0x0F; // CTS[19:16]
                    SB[2] = msReadByte(REG_161114); // CTS[15:8]
                    SB[3] = msReadByte(REG_161115); // CTS[7:0]
                    SB[4] = (msReadByte(REG_161113) & 0xF0) >> 4; // N[19:16]
                    SB[5] = msReadByte(REG_161116); // N[15:8]
                    SB[6] = msReadByte(REG_161117); // N[7:0]
                }

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

            if(enInputPort == MSCombo_TMDS_PORT_0)
            {
                for ( i=0; i < MaxCntLength; i++ )
                {
                    DataArray[i] = msReadByte(REG_16052A + i);
                }
            }
            else if(enInputPort == MSCombo_TMDS_PORT_1)
            {
                for ( i=0; i < MaxCntLength; i++ )
                {
                    DataArray[i] = msReadByte(REG_160A2A + i);
                }
            }
            else if((enInputPort == MSCombo_TMDS_PORT_2) || (enInputPort == MSCombo_TMDS_PORT_3))
            {
                for ( i=0; i < MaxCntLength; i++ )
                {
                    DataArray[i] = msReadByte(REG_16112A + i);
                }
            }
        break;

        case COMBO_PACKET_ACP_PACKET:
            MaxCntLength = (ucPacketLength > 16 ? 16 : ucPacketLength);

            if(enInputPort == MSCombo_TMDS_PORT_0)
            {
                for ( i=0; i < MaxCntLength; i++ )
                {
                    DataArray[i] = msReadByte(REG_16052E + i);
                }
            }
            else if(enInputPort == MSCombo_TMDS_PORT_1)
            {
                for ( i=0; i < MaxCntLength; i++ )
                {
                    DataArray[i] = msReadByte(REG_160A2E + i);
                }
            }
            else if((enInputPort == MSCombo_TMDS_PORT_2) || (enInputPort == MSCombo_TMDS_PORT_3))
            {
                for ( i=0; i < MaxCntLength; i++ )
                {
                    DataArray[i] = msReadByte(REG_16112E + i);
                }
            }
         break;

        //isrc1: byte0~15; isrc2: byte 16~31
        case COMBO_PACKET_ISRC1_PACKET:
            MaxCntLength = (ucPacketLength > 16 ? 16 : ucPacketLength);

            if(enInputPort == MSCombo_TMDS_PORT_0)
            {
                msWriteByteMask(REG_16050F, 0x00, 0x20); // [13]: Original ISRC packet = 0
                for ( i=0; i < MaxCntLength; i++ )
                {
                    DataArray[i] = msReadByte(REG_160540 + i);
                }
            }
            else if(enInputPort == MSCombo_TMDS_PORT_1)
            {
                msWriteByteMask(REG_160A0F, 0x00, 0x20);
                for ( i=0; i < MaxCntLength; i++ )
                {
                    DataArray[i] = msReadByte(REG_160A40 + i);
                }
            }
            else if((enInputPort == MSCombo_TMDS_PORT_2) || (enInputPort == MSCombo_TMDS_PORT_3))
            {
                msWriteByteMask(REG_16110F, 0x00, 0x20);
                for ( i=0; i < MaxCntLength; i++ )
                {
                    DataArray[i] = msReadByte(REG_161140 + i);
                }
            }
        break;

        case COMBO_PACKET_ISRC2_PACKET:
            MaxCntLength = (ucPacketLength > 16 ? 16 : ucPacketLength);

            if(enInputPort == MSCombo_TMDS_PORT_0)
            {
                for ( i=0; i < MaxCntLength; i++ )
                {
                    DataArray[i] = msReadByte(REG_160550 + i);
                }
            }
            else if(enInputPort == MSCombo_TMDS_PORT_1)
            {
                for ( i=0; i < MaxCntLength; i++ )
                {
                    DataArray[i] = msReadByte(REG_160A50 + i);
                }
            }
            else if((enInputPort == MSCombo_TMDS_PORT_2) || (enInputPort == MSCombo_TMDS_PORT_3))
            {
                for ( i=0; i < MaxCntLength; i++ )
                {
                    DataArray[i] = msReadByte(REG_161150 + i);
                }
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

            if(enInputPort == MSCombo_TMDS_PORT_0)
            {
                msWriteByteMask(REG_16050F, 0x30, 0x30); // [13]: GM packet = 1; [12]: output current affected GM packet to GMP register
                for ( i=0; i < MaxCntLength; i++ )
                {
                    DataArray[i] = msReadByte(REG_160540 + i);
                }
            }
            else if(enInputPort == MSCombo_TMDS_PORT_1)
            {
                 msWriteByteMask(REG_160A0F, 0x30, 0x30);
                for ( i=0; i < MaxCntLength; i++ )
                {
                    DataArray[i] = msReadByte(REG_160A40 + i);
                }
            }
            else if((enInputPort == MSCombo_TMDS_PORT_2) || (enInputPort == MSCombo_TMDS_PORT_3))
            {
                 msWriteByteMask(REG_16110F, 0x30, 0x30);
                for ( i=0; i < MaxCntLength; i++ )
                {
                    DataArray[i] = msReadByte(REG_161140 + i);
                }
            }

        break;

        case COMBO_PACKET_VS_INFOFRAME:
            MaxCntLength = (ucPacketLength > 29 ? 29 : ucPacketLength);

            if(enInputPort == MSCombo_TMDS_PORT_0)
            {
                for ( i=0; i < MaxCntLength; i++ )
                {
                    DataArray[i] = msReadByte(REG_160560 + i);
                }
            }
            else if(enInputPort == MSCombo_TMDS_PORT_1)
            {
                for ( i=0; i < MaxCntLength; i++ )
                {
                    DataArray[i] = msReadByte(REG_160A60 + i);
                }
            }
            else if((enInputPort == MSCombo_TMDS_PORT_2) || (enInputPort == MSCombo_TMDS_PORT_3))
            {
                for ( i=0; i < MaxCntLength; i++ )
                {
                    DataArray[i] = msReadByte(REG_161160 + i);
                }
            }
        break;

        case COMBO_PACKET_AVI_INFOFRAME:
            MaxCntLength = (ucPacketLength > 13 ? 13 : ucPacketLength);

            if(enInputPort == MSCombo_TMDS_PORT_0)
            {
                for ( i=0; i < MaxCntLength; i++ )
                {
                    DataArray[i] = msReadByte(REG_160580 + i);
                }
            }
            else if(enInputPort == MSCombo_TMDS_PORT_1)
            {
                for ( i=0; i < MaxCntLength; i++ )
                {
                    DataArray[i] = msReadByte(REG_160A80 + i);
                }
            }
            else if((enInputPort == MSCombo_TMDS_PORT_2) || (enInputPort == MSCombo_TMDS_PORT_3))
            {
                for ( i=0; i < MaxCntLength; i++ )
                {
                    DataArray[i] = msReadByte(REG_161180 + i);
                }
            }
        break;

        case COMBO_PACKET_SPD_INFOFRAME:
            MaxCntLength = (ucPacketLength > 25 ? 25 : ucPacketLength);

            if(enInputPort == MSCombo_TMDS_PORT_0)
            {
                for ( i=0; i < MaxCntLength; i++ )
                {
                    DataArray[i] = msReadByte(REG_16058E + i);
                }
            }
            else if(enInputPort == MSCombo_TMDS_PORT_1)
            {
                for ( i=0; i < MaxCntLength; i++ )
                {
                    DataArray[i] = msReadByte(REG_160A8E + i);
                }
            }
            else if((enInputPort == MSCombo_TMDS_PORT_2) || (enInputPort == MSCombo_TMDS_PORT_3))
            {
                for ( i=0; i < MaxCntLength; i++ )
                {
                    DataArray[i] = msReadByte(REG_16118E + i);
                }
            }
        break;

        case COMBO_PACKET_AUD_INFOFRAME:
            MaxCntLength = (ucPacketLength > 5 ? 5 : ucPacketLength);

            if(enInputPort == MSCombo_TMDS_PORT_0)
            {
                for ( i=0; i < MaxCntLength; i++ )
                {
                    DataArray[i] = msReadByte(REG_1605A8 + i);
                }
            }
            else if(enInputPort == MSCombo_TMDS_PORT_1)
            {
                for ( i=0; i < MaxCntLength; i++ )
                {
                    DataArray[i] = msReadByte(REG_160AA8 + i);
                }
            }
            else if((enInputPort == MSCombo_TMDS_PORT_2) || (enInputPort == MSCombo_TMDS_PORT_3))
            {
                for ( i=0; i < MaxCntLength; i++ )
                {
                    DataArray[i] = msReadByte(REG_1611A8 + i);
                }
            }
        break;

        case COMBO_PACKET_MPEG_INFOFRAME:
            MaxCntLength = (ucPacketLength > 5 ? 5 : ucPacketLength);

            if(enInputPort == MSCombo_TMDS_PORT_0)
            {
                for ( i=0; i < MaxCntLength; i++ )
                {
                    DataArray[i] = msReadByte(REG_1605AE + i);
                }
            }
            else if(enInputPort == MSCombo_TMDS_PORT_1)
            {
                for ( i=0; i < MaxCntLength; i++ )
                {
                    DataArray[i] = msReadByte(REG_160AAE + i);
                }
            }
            else if((enInputPort == MSCombo_TMDS_PORT_2) || (enInputPort == MSCombo_TMDS_PORT_3))
            {
                for ( i=0; i < MaxCntLength; i++ )
                {
                    DataArray[i] = msReadByte(REG_1611AE + i);
                }
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
    if(bFlag)
    {
        switch(enAudioFS)
        {
            case MScombo_TMDS_AUD_FREQ_32K:
                msWrite2ByteMask(REG_162F72, 0xBC00, 0xFFFF);
                msWrite2ByteMask(REG_162F74, 0x0034, 0x00FF);
            break;
            case MScombo_TMDS_AUD_FREQ_44K:
                msWrite2ByteMask(REG_162F72, 0x43E8, 0xFFFF);
                msWrite2ByteMask(REG_162F74, 0x0026, 0x00FF);
            break;
            case MScombo_TMDS_AUD_FREQ_48K:
                msWrite2ByteMask(REG_162F72, 0x2800, 0xFFFF);
                msWrite2ByteMask(REG_162F74, 0x0023, 0x00FF);
            break;
            case MScombo_TMDS_AUD_FREQ_88K:
                msWrite2ByteMask(REG_162F72, 0x21F5, 0xFFFF);
                msWrite2ByteMask(REG_162F74, 0x0013, 0x00FF);
            break;
            case MScombo_TMDS_AUD_FREQ_96K:
                msWrite2ByteMask(REG_162F72, 0x9400, 0xFFFF);
                msWrite2ByteMask(REG_162F74, 0x0011, 0x00FF);
            break;
            case MScombo_TMDS_AUD_FREQ_176K:
                msWrite2ByteMask(REG_162F72, 0x906C, 0xFFFF);
                msWrite2ByteMask(REG_162F74, 0x0009, 0x00FF);
            break;
            case MScombo_TMDS_AUD_FREQ_192K:
                msWrite2ByteMask(REG_162F72, 0xCA00, 0xFFFF);
                msWrite2ByteMask(REG_162F74, 0x0008, 0x00FF);
            break;
			default:
                msWrite2ByteMask(REG_162F72, 0x2800, 0xFFFF);
                msWrite2ByteMask(REG_162F74, 0x0023, 0x00FF);
            break;
        }
        msWriteByteMask(REG_163002, 0x08, 0x0C);
        msWriteByteMask(REG_1630A1, 0x08, 0x0C);
        msWriteByteMask(REG_162F70, 0x01, 0x01);
    }
    else
    {
        msWriteByteMask(REG_163002, 0x00, 0x0C);
        msWriteByteMask(REG_1630A1, 0x04, 0x0C);
        msWriteByteMask(REG_162F70, 0x00, 0x01);
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
    DWORD dwCTSValue = 0, dwNValue = 0, dwTMDSClkValCnt;
    DWORD dwAudFreqValue = MScombo_TMDS_AUD_FREQ_ERROR;

    wOffset = _mhal_tmds_phy_offset(enInputPort);

    //Get current TMDS clock rate
    dwTMDSClkValCnt = mhal_tmds_GetClockRate(enInputPort) * 10;

    if(enInputPort == MSCombo_TMDS_PORT_0)
    {
        //load current CTS/N value first
        msWrite2ByteMask(REG_160524, 0xFFFF, 0xFFFF);
        msWrite2ByteMask(REG_160526, 0xFFFF, 0xFFFF);
        msWriteByteMask(REG_160528, 0xFF, 0xFF);

        dwNValue = (msRead2Byte(REG_160526)) | (((msReadByte(REG_160528) & 0xF0) >> 4) << 16);
        dwCTSValue = (msRead2Byte(REG_160524)) | ((msReadByte(REG_160528) & 0x0F) << 16);
    }
    else if(enInputPort == MSCombo_TMDS_PORT_1)
    {
        //load current CTS/N value first
        msWrite2ByteMask(REG_160A24, 0xFFFF, 0xFFFF);
        msWrite2ByteMask(REG_160A26, 0xFFFF, 0xFFFF);
        msWriteByteMask(REG_160A28, 0xFF, 0xFF);

        dwNValue = (msRead2Byte(REG_160A26)) | (((msReadByte(REG_160A28) & 0xF0) >> 4) << 16);
        dwCTSValue = (msRead2Byte(REG_160A24)) | ((msReadByte(REG_160A28) & 0x0F) << 16);
    }
    else if((enInputPort == MSCombo_TMDS_PORT_2) || (enInputPort == MSCombo_TMDS_PORT_3))
    {
        //load current CTS/N value first
        msWrite2ByteMask(REG_161124, 0xFFFF, 0xFFFF);
        msWrite2ByteMask(REG_161126, 0xFFFF, 0xFFFF);
        msWriteByteMask(REG_161128, 0xFF, 0xFF);

        dwNValue = (msRead2Byte(REG_161126)) | (((msReadByte(REG_161128) & 0xF0) >> 4) << 16);

        if(mhal_tmds_InputIsHDMI2(enInputPort))
        {
            dwCTSValue = ((msRead2Byte(REG_161124)) | ((msReadByte(REG_161128) & 0x0F) << 16)) / 2;
        }
        else
        {
            dwCTSValue = ((msRead2Byte(REG_161124)) | ((msReadByte(REG_161128) & 0x0F) << 16)) * 2;
        }

    }

    if ((dwNValue < 0x10) || (dwCTSValue < 0x10))
        return MScombo_TMDS_AUD_FREQ_ERROR;   //error

    dwAudFreqValue = ((dwTMDSClkValCnt / 128) * dwNValue) / dwCTSValue;

    if (abs(dwAudFreqValue - 32) < 3)
        dwAudFreqValue = MScombo_TMDS_AUD_FREQ_32K;

    else if (abs(dwAudFreqValue - 44) < 3)
        dwAudFreqValue = MScombo_TMDS_AUD_FREQ_44K;

    else if (abs(dwAudFreqValue - 48) < 3)
        dwAudFreqValue = MScombo_TMDS_AUD_FREQ_48K;

    else if (abs(dwAudFreqValue - 88) < 4)
        dwAudFreqValue = MScombo_TMDS_AUD_FREQ_88K;

    else if (abs(dwAudFreqValue - 96) < 4)
        dwAudFreqValue = MScombo_TMDS_AUD_FREQ_96K;

    else if (abs(dwAudFreqValue - 176) < 5)
        dwAudFreqValue = MScombo_TMDS_AUD_FREQ_176K;

    else if (abs(dwAudFreqValue - 192) < 5)
        dwAudFreqValue = MScombo_TMDS_AUD_FREQ_192K;

    else
        dwAudFreqValue = MScombo_TMDS_AUD_FREQ_ERROR; //error

    #if 0
    {
        static WORD printcnt = 0;
        if ((printcnt % 10000 == 0) && (printcnt != 0))
        {
            printf("[HDMI] tmds clk = %d\n", dwTMDSClkValCnt);
            printf("[HDMI] N value = %d\n", dwNValue);
            printf("[HDMI] CTS value = %d\n", dwCTSValue);
            printf("[HDMI] audio freq = %d\n", dwAudFreqValue);
            printcnt -= 10000;
        }
        else
            printcnt++;
    }
    #endif

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
    MSCombo_TMDS_AUDIO_FORMAT enAudFmt;

    if(enInputPort == MSCombo_TMDS_PORT_0)
    {
        if ( msReadByte(REG_1605B4) & _BIT6 )
            enAudFmt = MSCombo_AUDIO_FMT_NON_PCM;
        else
            enAudFmt = MSCombo_AUDIO_FMT_PCM;
    }
    else if(enInputPort == MSCombo_TMDS_PORT_1)
    {
        if ( msReadByte(REG_160AB4) & _BIT6 )
            enAudFmt = MSCombo_AUDIO_FMT_NON_PCM;
        else
            enAudFmt = MSCombo_AUDIO_FMT_PCM;
    }
    else if((enInputPort == MSCombo_TMDS_PORT_2) || (enInputPort == MSCombo_TMDS_PORT_3))
    {
        if ( msReadByte(REG_1611B4) & _BIT6 )
            enAudFmt = MSCombo_AUDIO_FMT_NON_PCM;
        else
            enAudFmt = MSCombo_AUDIO_FMT_PCM;
    }
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
    WORD wOffset;

    wOffset = _mhal_tmds_phy_offset(enInputPort);

    if(msReadByte(REG_112180 + wOffset) & 0x02) // clock stable
    {
       // printf("clock stable ohohohohohohohohohohoh\n");
        //msWrite2Byte(REG_100FD6, 0xF000);
        return TRUE;
    }

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
    WORD wOffset;

    wOffset = _mhal_tmds_phy_offset(enInputPort);

    if(msReadByte(REG_112180 + wOffset) & 0x40) // clock big change
    {
       // msWriteByteMask(REG_112183 + wOffset, 0x40, 0x40); //clr
        msWriteByte(REG_112183 + wOffset, 0x40); //clr

      //  msWriteByteMask(REG_112183 + wOffset, 0x00, 0x40);
        msWriteByte(REG_112183 + wOffset, 0x00);

       // msWriteByteMask(REG_112183 + wOffset, 0x00, 0x40);
        msWriteByte(REG_112183 + wOffset, 0x00);
        return TRUE;
    }
    else
        return FALSE;

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

    wOffset = _mhal_tmds_phy_offset(enInputPort);

   // msWriteByteMask(REG_112183 + wOffset, 0x02, 0x02); //clear clock valid
    msWriteByte(REG_112183 + wOffset, 0x02); //clear clock valid

   // msWriteByteMask(REG_112183 + wOffset, 0x00, 0x02);
    msWriteByte(REG_112183 + wOffset, 0x00);

   // msWriteByteMask(REG_112183 + wOffset, 0x00, 0x02);
    msWriteByte(REG_112183 + wOffset, 0x00);

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
    BYTE ucTmpValue = 0;

    //GCP packeting phase bit
    if(enInputPort == MSCombo_TMDS_PORT_0)
        ucTmpValue = (msReadByte(REG_16052B) & 0x0F);

    else if(enInputPort == MSCombo_TMDS_PORT_1)
        ucTmpValue = (msReadByte(REG_160A2B) & 0x0F);

    else if((enInputPort == MSCombo_TMDS_PORT_2) || (enInputPort == MSCombo_TMDS_PORT_3))
        ucTmpValue = (msReadByte(REG_16112B) & 0x0F);

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
    BYTE ucTmpValue = 0;

    // [3:0]: Pixel repetition
    if(enInputPort == MSCombo_TMDS_PORT_0)
        ucTmpValue = (msReadByte(REG_160584) & 0x0F);

    else if(enInputPort == MSCombo_TMDS_PORT_1)
        ucTmpValue = (msReadByte(REG_160A84) & 0x0F);

    else if((enInputPort == MSCombo_TMDS_PORT_2) || (enInputPort == MSCombo_TMDS_PORT_3))
        ucTmpValue = (msReadByte(REG_161184) & 0x0F);

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
        if(enInputPort == MSCombo_TMDS_PORT_0)
        {
            if (msReadByte(REG_16024E) & BIT1) //[1]: MHL PP mode enable
                dwTMDSClkValCnt = dwTMDSClkValCnt << 1;
        }
        else if(enInputPort == MSCombo_TMDS_PORT_1)
        {
            if (msReadByte(REG_16074E) & BIT1) //[1]: MHL PP mode enable
                dwTMDSClkValCnt = dwTMDSClkValCnt << 1;
        }
        else if((enInputPort == MSCombo_TMDS_PORT_2) || (enInputPort == MSCombo_TMDS_PORT_3))
        {
             if (msReadByte(REG_160C4E) & BIT1) //[1]: MHL PP mode enable
                dwTMDSClkValCnt = dwTMDSClkValCnt << 1;
        }
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
    return bOver3GFlag[enInputPort];
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
    bOver3GFlag[enInputPort] = Flag;
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
    return bSplitterDone[enInputPort];
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
    bSplitterDone[enInputPort] = Flag;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_GetAutoEQCntRst()
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
    return bAutoEQCntRst[enInputPort];
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
    bAutoEQCntRst[enInputPort] = bFlag;
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
#if(TMDS_USE_SW_AUTO_EQ_14)
    if(bSWAutoEQEnable)
    {
        if(enInputPort == bCurrentPort)
        {
            _mhal_tmds_SWAutoEQPollingProc(enInputPort);
        }
    }

#endif

    return bAutoEQDone[enInputPort];
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
    WORD wCount = 500;
    if((enInputPort == MSCombo_TMDS_PORT_2) || (enInputPort == MSCombo_TMDS_PORT_3))
    {
        if(bEnable)
        {
            msWriteByteMask(REG_1124DC, bOverWrite? 0xF0: 0x00, 0xF0); // eq overwrite disable

            msWriteByteMask(REG_11244A, 0x04, 0x04); // enable autoEQ
            msWriteByteMask(REG_112466, 0x04, 0x04); // sw trigger autoEQ
            while(wCount > 0)
            {
                wCount--;
                //_nop_();
            }
            msWriteByteMask(REG_112466, 0x00, 0x04); // sw trigger autoEQ
        }
        else
        {
            msWriteByteMask(REG_1124DC, 0xF0, 0xF0); // eq overwrite enable
            msWriteByteMask(REG_11244A, 0x00, 0x04); // disable autoEQ
        }
    }
    else
    {
        TMDS_PRINT("Port: 0x%x, no need\r\n", enInputPort);
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
    BYTE i;
    BYTE ucDEStableCnt = 5;

    if((enInputPort == MSCombo_TMDS_PORT_2) || (enInputPort == MSCombo_TMDS_PORT_3))
    {
        if(bAutoEQRetry[enInputPort] < 5)
        {
            if(bFlag)
            {
                if(_mhal_tmds_AutoEQDoneInterrupt(enInputPort))
                {
                   // printf("_mhal_tmds_AutoEQDoneInterrupt\n");
                    for(i = 0; i < ucDEStableCnt; i++)
                    {
                        if(!mhal_tmds_DE_Stable(enInputPort))
                        {
                            mhal_tmds_TriggerAutoEQ(enInputPort,TRUE, FALSE);
                            bAutoEQCntRst[enInputPort] = TRUE;
                            bAutoEQRetry[enInputPort]++;
                           // printf("port = %d, bAutoEQRetry = %d, i = %d\n", enInputPort, bAutoEQRetry[enInputPort], i);
                            break;
                        }
                    }
                    if(i == 5)
                    {
                        bAutoEQDone[enInputPort] = TRUE;
                       // printf("autoEQ done!!!\n");
                    }

                }
                else
                {
                    mhal_tmds_TriggerAutoEQ(enInputPort,TRUE, FALSE);
                    bAutoEQCntRst[enInputPort] = TRUE;
                    bAutoEQRetry[enInputPort] = 0;
                   // printf("timeout!!!\n");
                }
            }
        }
        else
        {
            bAutoEQDone[enInputPort] = TRUE;
           // printf("retry finish!!!\n");
        }
    }
    else
    {
        TMDS_PRINT("Port: 0x%x, no need\r\n", enInputPort);
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
// (MST9U)combo_rx_top(1606)_2b -> (MST9U3)combo_phy1(1121)_40
//**************************************************************************
void mhal_tmds_ISR(MSCombo_TMDS_PORT_INDEX enInputPort, Bool bMHLSupport)
{
    WORD wOffset = 0;
    WORD wTMDSClkRate = 0;
    Bool bOver3G = FALSE;

    wOffset = _mhal_tmds_phy_offset(enInputPort);

    if((enInputPort == MSCombo_TMDS_PORT_0) || (enInputPort == MSCombo_TMDS_PORT_1))
    {
        // turn on mask
        //msWriteByte(REG_112181 + wOffset, 0xFF); // combo irq mask

        if (msReadByte(REG_112180 + wOffset) & 0x02) // [1]: clock stable
        {
            msWrite2ByteMask(REG_112064 + wOffset, 0x4444, 0x4444); // clear combo_phy0 clock big change

            msWriteByte(REG_112183 + wOffset, 0x02); // clear clock stable
            msWriteByte(REG_112183 + wOffset, 0x00);
            msWriteByte(REG_112183 + wOffset, 0x00);

			if(!mhal_tmds_PM_Clock_Detect(enInputPort))
			{
				msWriteByte(REG_11201C + wOffset, 0x08); // MHL auto EQ sw reset (reset interrupt)
				msWriteByte(REG_11201C + wOffset, 0x00);
			}

            wTMDSClkRate = (msRead2Byte(REG_112078 + wOffset)* 12 / 128);

            if ((wPreviousTiming == wTMDSClkRate) && (bPreviousPort == enInputPort))
            {
                //msWrite2Byte(REG_112181 + wOffset, 0xFD); //combo irq mask
                return; //no timing change or DC on/off
            }
            wPreviousTiming = wTMDSClkRate;
            bPreviousPort = enInputPort;

            if((!bMHLSupport) || (mhal_tmds_DetectCablePlug(enInputPort, bMHLSupport) <= 1)) //HDMI cable
            {
                if(bMHLCable[enInputPort] == TRUE)
                {
                    bMHLCable[enInputPort] = FALSE;
                    msWriteByteMask(REG_11203B + wOffset, BIT0, BIT0); // REG_COMBO_PHY0_1D[8]:Auto clear phase
                    msWriteByteMask(REG_11203B + wOffset, 0, BIT0); // REG_COMBO_PHY0_1D[8]:Auto clear phase
                }
            }
            else
            {
                bMHLCable[enInputPort] = TRUE;
            }
        }

        // turn off mask
        //msWrite2Byte(REG_112181 + wOffset, 0xFD); //combo irq mask
    }

    else if((enInputPort == MSCombo_TMDS_PORT_2) || (enInputPort == MSCombo_TMDS_PORT_3))
    {
        if (enInputPort == bCurrentPort)
        {
            // turn on mask
            //msWriteByte(REG_112181 + wOffset, 0xFF); // combo irq mask

            if (msReadByte(REG_112180 + wOffset) & 0x02) // [1]: clock stable
            {
                bSplitterDone[enInputPort] = FALSE;
                msWrite2ByteMask(REG_112064 + wOffset, 0x4444, 0x4444); // clear combo_phy0 clock big change

                msWriteByte(REG_112183 + wOffset, 0x02); // clear clock stable
                msWriteByte(REG_112183 + wOffset, 0x00);
                msWriteByte(REG_112183 + wOffset, 0x00);

				if(!mhal_tmds_PM_Clock_Detect(enInputPort))
				{
					msWriteByte(REG_11201C + wOffset, 0x08); // MHL auto EQ sw reset (reset interrupt)
					msWriteByte(REG_11201C + wOffset, 0x00);
				}

                msWriteByte(REG_161104, 0x20); // clear DE pixel change

                wTMDSClkRate = (msRead2Byte(REG_112078 + wOffset)* 12 / 128);
                bOver3G = ((msReadByte(REG_112560) &BIT3)? TRUE: FALSE);

                if ((wPreviousTiming == wTMDSClkRate) && (bPreviousPort == enInputPort)
                    && (bOver3GFlag[enInputPort] == bOver3G))
                {
                    //msWrite2Byte(REG_112181 + wOffset, 0xFD); //combo irq mask
                    return; //no timing change or DC on/off
                }

                wPreviousTiming = wTMDSClkRate;
                bPreviousPort = enInputPort;

                if((!bMHLSupport) || (mhal_tmds_DetectCablePlug(enInputPort, bMHLSupport) <= 1)) //HDMI cable
                {
                    if (wTMDSClkRate >= 250)
                    {
                        //printMsg("eq= 12");
                        msWriteByteMask(REG_1124DC, 0xF0, 0xF0); // eq overwrite enable
                        msWrite2ByteMask(REG_1124E6, (COMBO_HDMI_250_340MHZ_EQ_VALUE<<8)|COMBO_HDMI_250_340MHZ_EQ_VALUE, 0x1F1F);
                        msWrite2ByteMask(REG_1124E8, (COMBO_HDMI_250_340MHZ_EQ_VALUE<<8)|COMBO_HDMI_250_340MHZ_EQ_VALUE, 0x1F1F);

                        //enable DIVSEL_IN DIVSEL_POST overwrite
                        msWriteByteMask(REG_1124DA, 0x0F, 0x0F);
                        msWriteByteMask(REG_1124DB, 0xF0, 0xF0);
                        msWriteByteMask(REG_1124E0, 0x00, 0xFF);
                        msWriteByteMask(REG_1124E3, 0x10, 0xF0);
                        msWriteByteMask(REG_1124E4, 0x49, 0xFF);

                        //phase step
                        msWriteByteMask(REG_112420, 0x00, 0x08);

                        // patch for HDMI1.4 jitter issue on 3G
                        msWriteByteMask(REG_1124DF, 0x08, 0x08); // ictrl pfd overwrite enable
                        msWriteByteMask(REG_1124D7, 0x08, 0x0F); // ictrl pfd value

                        // patch for QFP 3G
                        msWriteByteMask(REG_1124C0, 0x70, 0x70);
                        msWriteByteMask(REG_1124C1, 0x0F, 0x0F);
                        msWriteByteMask(REG_1124C2, 0x70, 0x70);
                        msWriteByteMask(REG_1124C3, 0x0F, 0x0F);
                        msWriteByteMask(REG_1124C4, 0x70, 0x70);
                        msWriteByteMask(REG_1124C5, 0x0F, 0x0F);
                        msWriteByteMask(REG_112506, 0x02, 0x02);
                        msWriteByteMask(REG_112509, 0x00, 0xFF);

                        bSWAutoEQEnable = TRUE;
                        ucSWAutoEQState = TMDS_SW_AUTO_EQ_START;
                    }
                    else
                    {
                        //printMsg("eq= 24");
                        msWriteByteMask(REG_1124DC, 0xF0, 0xF0); // eq overwrite enable
                        msWrite2ByteMask(REG_1124E6, (COMBO_HDMI_25_250MHZ_EQ_VALUE<<8)|COMBO_HDMI_25_250MHZ_EQ_VALUE, 0x1F1F);
                        msWrite2ByteMask(REG_1124E8, (COMBO_HDMI_25_250MHZ_EQ_VALUE<<8)|COMBO_HDMI_25_250MHZ_EQ_VALUE, 0x1F1F);

                        //enable DIVSEL_IN DIVSEL_POST overwrite
                        msWriteByteMask(REG_1124DA, 0x00, 0x0F);
                        msWriteByteMask(REG_1124DB, 0x00, 0xF0);

                        //phase step
                        msWriteByteMask(REG_112420, 0x08, 0x08);

                        // patch for HDMI1.4 jitter issue on 3G
                        msWriteByteMask(REG_1124DF, 0x08, 0x08); // ictrl pfd overwrite enable
                        msWriteByteMask(REG_1124D7, 0x04, 0x0F); // ictrl pfd value
                        msWriteByteMask(REG_1124DF, 0x00, 0x08); // ictrl pfd overwrite enable

                        msWriteByteMask(REG_1124C0, 0x60, 0x70);
                        msWriteByteMask(REG_1124C1, 0x00, 0x0F);
                        msWriteByteMask(REG_1124C2, 0x60, 0x70);
                        msWriteByteMask(REG_1124C3, 0x00, 0x0F);
                        msWriteByteMask(REG_1124C4, 0x60, 0x70);
                        msWriteByteMask(REG_1124C5, 0x00, 0x0F);
                        msWriteByteMask(REG_112506, 0x00, 0x02);
                        msWriteByteMask(REG_112509, 0x00, 0xFF);

                        bSWAutoEQEnable = FALSE;
                    }

                    bOver3GFlag[enInputPort] = ((msReadByte(REG_112560) &BIT3)? TRUE: FALSE);
                    if(bOver3GFlag[enInputPort])
                    {
                        mhal_tmds_CheckScrambleStatus(enInputPort, TRUE);
                    }
                    mhal_tmds_HDMI20Setting(enInputPort, bOver3GFlag[enInputPort]);
                }
            }

            // turn off mask
            //msWrite2Byte(REG_112181 + wOffset, 0xFD); //combo irq mask
        }
    }
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
    static WORD waitcounts = 0;
    if(bflag)
    {
        waitcounts = 1;
		msWriteByteMask(REG_00180A, 0x00, 0x30);
        msWriteByteMask(REG_160CC6, 0x01, 0x01);
        msWriteByteMask(REG_160CC6, 0x00, 0x01);
    }
	else
	{
        if((enInputPort == MSCombo_TMDS_PORT_2) || (enInputPort == MSCombo_TMDS_PORT_3))
        {
            if (enInputPort == bCurrentPort)
            {
                if(waitcounts != 0x00)
				   waitcounts++;
				else
                   return;
				if(waitcounts > 25)
				{
				   waitcounts = 0;
                   if(msReadByte(REG_160C30) != 0x00)
                   {
                       msWrite2Byte(REG_001804, 0x0421);
					   ForceDelay1ms(1);
					   if(msReadByte(REG_001808) == 0x00)
					   {
						   msWriteByteMask(REG_00180A, 0x20, 0x30);
                       }
				   }
				}
            }
        }
        else
        {
            TMDS_PRINT("Port: 0x%x, no need\r\n", enInputPort);
        }
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
            bNoInputFlag = ((msReadByte(REG_112180) &BIT3)? TRUE: FALSE); // REG_COMBO_PHY1_40[3], No input status port0

            if(bNoInputFlag)
            {
                msWriteByte(REG_112183, BIT3); // REG_COMBO_PHY1_41[13], No input clear port0
                msWriteByte(REG_112183, 0); // REG_COMBO_PHY1_41[13], No input clear port0
            }

            break;

        case MSCombo_TMDS_PORT_1:
            bNoInputFlag = ((msReadByte(REG_112380) &BIT3)? TRUE: FALSE); // REG_COMBO_PHY3_40[3], No input status port1

            if(bNoInputFlag)
            {
                msWriteByte(REG_112383, BIT3); // REG_COMBO_PHY3_41[11], No input clear port1
                msWriteByte(REG_112383, 0); // REG_COMBO_PHY3_41[11], No input clear port1
            }

            break;

        case MSCombo_TMDS_PORT_2:
        case MSCombo_TMDS_PORT_3:
            if(enInputPort == bCurrentPort)
            {
                bNoInputFlag = ((msReadByte(REG_112580) &BIT3)? TRUE: FALSE); // REG_COMBO_PHY5_40[3], No input status port2&3

                if(bNoInputFlag)
                {
                    msWriteByte(REG_112583, BIT3); // REG_COMBO_PHY5_41[11], No input clear port2&3
                    msWriteByte(REG_112583, 0); // REG_COMBO_PHY5_41[11], No input clear port2&3
                }
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
    if((enInputPort == MSCombo_TMDS_PORT_2) || (enInputPort == MSCombo_TMDS_PORT_3))
    {
        if (enInputPort == bCurrentPort)
        {
           // printf("C. mhal_tmds_Over3G_420Setting =============== 1p\n");
            msWriteByteMask(REG_1124D6, bYUV420Flag? 0x06:0x02, 0x06); // [2]: enable clko_pix 2x; [1]: reg_atop_en_clko_pix=1
            msWriteByteMask(REG_163060, bYUV420Flag? 0x04:0x00, 0x0D); // [2]: reg_pix_clk_div2_en_p2; [0]: reg_tmds_clk_div2_en_p2
            msWriteByteMask(REG_1612A8, bYUV420Flag? 0x02:0x00, 0x03); // [1:0]: reg_avg_ctrl_case
        }
    }
    else
    {
        TMDS_PRINT("Port: 0x%x, no need\r\n", enInputPort);
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
    if(enInputPort == MSCombo_TMDS_PORT_0)
    {
        HDE_value = msRead2Byte(REG_1605CA) & 0x3FFF;
    }
    else if(enInputPort == MSCombo_TMDS_PORT_1)
    {
        HDE_value = msRead2Byte(REG_160ACA) & 0x3FFF;
    }
    else if((enInputPort == MSCombo_TMDS_PORT_2) || (enInputPort == MSCombo_TMDS_PORT_3))
    {
        HDE_value = msRead2Byte(REG_1611CA) & 0x3FFF;
    }
    else
    {
        TMDS_PRINT("Port: 0x%x, no need\r\n", enInputPort);
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
    if((enInputPort == MSCombo_TMDS_PORT_2) || (enInputPort == MSCombo_TMDS_PORT_3))
    {
        if (enInputPort == bCurrentPort)
        {
            if(bFlag) // over 3.4G
            {
               // printf("if(msReadByte(REG_112560 ) & 0x08) =============== over 3.4G\n");
                msWriteByteMask(REG_160C63, 0x05, 0x0F); // [15:8]: De filter length

                msWriteByteMask(REG_1124D6, 0x02, 0x06); // [2]: enable clko_pix 2x; [1]: reg_atop_en_clko_pix=1
                msWriteByteMask(REG_112414, 0x40, 0x40); // [6]: reg_af_ls_20out_sel=1
                msWriteByteMask(REG_1124B4, 0x00, 0x04); // [2]: disable reg_atop_en_clko_tmds2x
                msWrite2ByteMask(REG_163062, 0, 0xFFFF); // enable P2 all clock power
                msWriteByteMask(REG_163060, 0x00, 0x0D); // [0]: reg_tmds_clk_div2_en_p2
                msWriteByteMask(REG_1612A8, 0x00, 0x03); // [1:0]: reg_avg_ctrl_case

                if(bSWAutoEQEnable)
                {
                    _mhal_tmds_HDMI20AutoEQSetting(TRUE);

                    bSWAutoEQEnable = FALSE;
                }

                mhal_tmds_TriggerAutoEQ(enInputPort, TRUE, FALSE);
                bAutoEQRetry[enInputPort] = 0;
                bAutoEQCntRst[enInputPort] = TRUE;
                bAutoEQDone[enInputPort] = FALSE;
            }
            else
            {
               // printf("else(msReadByte(REG_112560 ) & 0x08) =============== under 3.4G\n");
                msWriteByteMask(REG_160C63, 0x0C, 0x0F); // [15:8]: De filter length

                msWriteByteMask(REG_1124D6, 0x06, 0x06); // [2]: enable clko_pix 2x; [1]: reg_atop_en_clko_pix=1
                msWriteByteMask(REG_112414, 0x40, 0x40); // [6]: reg_af_ls_20out_sel=1
                msWriteByteMask(REG_1124B4, 0x04, 0x04); // [2]: disable reg_atop_en_clko_tmds2x
                msWrite2ByteMask(REG_163062, 0, 0xFFFF); // enable P2 all clock power
                msWriteByteMask(REG_163060, 0x05, 0x0D); // [2]: reg_pix_clk_div2_en_p2; [0]: reg_tmds_clk_div2_en_p2
                msWriteByteMask(REG_1612A8, 0x02, 0x03); // [1:0]: reg_avg_ctrl_case

                mhal_tmds_TriggerAutoEQ(enInputPort, FALSE, FALSE);
            }

            _mhal_tmds_SetEfuseSettingValue(enInputPort, bFlag);
        }
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
    if(bEnable)
    {
        if(enInputPort == MSCombo_TMDS_PORT_0)
        {
            msWriteByteMask(REG_1606C0, bFlag ? 0x01 : 0x00, 0x31); //[0]: enable of dual link FIFO, [5:4]: deep color define
            switch(ucBits)
            {
                case MSCombo_TMDS_MONO_8_BIT:
                    msWriteByteMask(REG_1606C1, 0x00, 0x0F); //mono type select
                break;
                case MSCombo_TMDS_MONO_10_BIT:
                    msWriteByteMask(REG_1606C1, 0x04, 0x0F); //mono type select
                break;
                case MSCombo_TMDS_MONO_12_BIT:
                    msWriteByteMask(REG_1606C1, 0x08, 0x0F); //mono type select
                break;
                default:
                    msWriteByteMask(REG_1606C1, 0x0C, 0x0F); //default value
                break;
            }
        }
        else if(enInputPort == MSCombo_TMDS_PORT_1)
        {
            msWriteByteMask(REG_160BC0, bFlag ? 0x01 : 0x00, 0x31); //[0]: enable of dual link FIFO, [5:4]: deep color define
            switch(ucBits)
            {
                case MSCombo_TMDS_MONO_8_BIT:
                    msWriteByteMask(REG_160BC1, 0x00, 0x0F); //mono type select
                break;
                case MSCombo_TMDS_MONO_10_BIT:
                    msWriteByteMask(REG_160BC1, 0x04, 0x0F); //mono type select
                break;
                case MSCombo_TMDS_MONO_12_BIT:
                    msWriteByteMask(REG_160BC1, 0x08, 0x0F); //mono type select
                break;
                default:
                    msWriteByteMask(REG_160BC1, 0x0C, 0x0F); //default value
                break;
            }
        }
        else if((enInputPort == MSCombo_TMDS_PORT_2) || (enInputPort == MSCombo_TMDS_PORT_3))
        {
            msWriteByteMask(REG_1612C0, bFlag ? 0x01 : 0x00, 0x31); //[0]: enable of dual link FIFO, [5:4]: deep color define
            switch(ucBits)
            {
                case MSCombo_TMDS_MONO_8_BIT:
                    msWriteByteMask(REG_1612C1, 0x00, 0x0F); //mono type select
                break;
                case MSCombo_TMDS_MONO_10_BIT:
                    msWriteByteMask(REG_1612C1, 0x04, 0x0F); //mono type select
                break;
                case MSCombo_TMDS_MONO_12_BIT:
                    msWriteByteMask(REG_1612C1, 0x08, 0x0F); //mono type select
                break;
                default:
                    msWriteByteMask(REG_1612C1, 0x0C, 0x0F); //default value
                break;
            }
        }
    }
    else
    {
        if(enInputPort == MSCombo_TMDS_PORT_0)
        {
            msWriteByteMask(REG_1606C0, 0x01, 0x31); //[0]: enable of dual link FIFO, [5:4]: deep color define
            msWriteByteMask(REG_1606C1, 0x0C, 0x0F); //default value
        }
        else if(enInputPort == MSCombo_TMDS_PORT_1)
        {
            msWriteByteMask(REG_160BC0, 0x01, 0x31); //[0]: enable of dual link FIFO, [5:4]: deep color define
            msWriteByteMask(REG_160BC1, 0x0C, 0x0F); //default value
        }
        else if((enInputPort == MSCombo_TMDS_PORT_2) || (enInputPort == MSCombo_TMDS_PORT_3))
        {
            msWriteByteMask(REG_1612C0, 0x01, 0x31); //[0]: enable of dual link FIFO, [5:4]: deep color define
            msWriteByteMask(REG_1612C1, 0x0C, 0x0F); //default value
        }
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

    if(bEnable)
    {
        if(enInputPort == MSCombo_TMDS_PORT_0)
        {
            switch(ucBits)
            {
                case MSCombo_TMDS_DVI_DEEP_COLOR_8_BIT:
                    msWriteByteMask(REG_1606C0, 0x00, 0x31);
                break;
                case MSCombo_TMDS_DVI_DEEP_COLOR_10_BIT:
                    msWriteByteMask(REG_1606C0, 0x11, 0x31);
                break;
                case MSCombo_TMDS_DVI_DEEP_COLOR_12_BIT:
                    msWriteByteMask(REG_1606C0, 0x21, 0x31);
                break;
                default:
                    msWriteByteMask(REG_1606C0, 0x01, 0x31);
                break;
            }
        }
        else if(enInputPort == MSCombo_TMDS_PORT_1)
        {
            switch(ucBits)
            {
                case MSCombo_TMDS_DVI_DEEP_COLOR_8_BIT:
                    msWriteByteMask(REG_160BC0, 0x00, 0x31);
                break;
                case MSCombo_TMDS_DVI_DEEP_COLOR_10_BIT:
                    msWriteByteMask(REG_160BC0, 0x11, 0x31);
                break;
                case MSCombo_TMDS_DVI_DEEP_COLOR_12_BIT:
                    msWriteByteMask(REG_160BC0, 0x21, 0x31);
                break;
                default:
                    msWriteByteMask(REG_160BC0, 0x01, 0x31);
                break;
            }
        }
        else if((enInputPort == MSCombo_TMDS_PORT_2) || (enInputPort == MSCombo_TMDS_PORT_3))
        {
            switch(ucBits)
            {
                case MSCombo_TMDS_DVI_DEEP_COLOR_8_BIT:
                    msWriteByteMask(REG_1612C0, 0x00, 0x31);
                break;
                case MSCombo_TMDS_DVI_DEEP_COLOR_10_BIT:
                    msWriteByteMask(REG_1612C0, 0x11, 0x31);
                break;
                case MSCombo_TMDS_DVI_DEEP_COLOR_12_BIT:
                    msWriteByteMask(REG_1612C0, 0x21, 0x31);
                break;
                default:
                    msWriteByteMask(REG_1612C0, 0x01, 0x31);
                break;
            }
        }

        wOffset = _mhal_tmds_phy_offset(enInputPort);
        if(ucBits>MSCombo_TMDS_DVI_DEEP_COLOR_8_BIT)
            msWriteByteMask(REG_1120D6 + wOffset, 0x01, 0x05); //Main:[0]: Enable dual clk; [2]: 2x Pixel clk
        else
            msWriteByteMask(REG_1120D6 + wOffset, 0x05, 0x05); //Main:[0]: Enable dual clk; [2]: 2x Pixel clk
    }
    else
    {
        if(enInputPort == MSCombo_TMDS_PORT_0)
            msWriteByteMask(REG_1606C0, 0x01, 0x31);

        else if(enInputPort == MSCombo_TMDS_PORT_1)
             msWriteByteMask(REG_160BC0, 0x01, 0x31);

        else if((enInputPort == MSCombo_TMDS_PORT_2) || (enInputPort == MSCombo_TMDS_PORT_3))
            msWriteByteMask(REG_1612C0, 0x01, 0x31);

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
    if((enInputPort == MSCombo_TMDS_PORT_2) || (enInputPort == MSCombo_TMDS_PORT_3))
    {
        if(msReadByte(REG_112560) & BIT3)
            return TRUE;
        else
            return FALSE;
    }
    else
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
    WORD wRealWidth = 0;
    if((enInputPort == MSCombo_TMDS_PORT_2) || (enInputPort == MSCombo_TMDS_PORT_3))
    {
        // Scaler
        if(bFlag == TRUE)
        {
            wRealWidth = msRead2Byte(REG_1611CA) & 0x3FFF;
            msWrite2ByteMask(REG_162DEA, 0x2320, 0xFFF0); // Combo2sc IPMUX  (SPT_0, SPT_1, SPT_2)
            msWrite2ByteMask(REG_162DEC, 0x0003, 0x000F); // Combo2sc IPMUX  (SPT_3)
        }
        else
        {
            wRealWidth = (msRead2Byte(REG_1611CA)  & 0x3FFF )* 2;
            msWrite2ByteMask(REG_162DEA, 0x2220, 0xFFF0); // Combo2sc IPMUX  (SPT_0, SPT_1, SPT_2)
            msWrite2ByteMask(REG_162DEC, 0x0002, 0x000F); // Combo2sc IPMUX  (SPT_3)
        }

        msWrite2ByteMask(REG_162DEE, 0x01E0, 0x01E0); // [8:5]: reg_clk_combo2sc_spt_en

        msWrite2ByteMask(REG_162DF2, 0x2220, 0xFFF0); // Combo2spt CLKMUX (SPT_0, SPT_1, SPT_2)
        msWrite2ByteMask(REG_162DF4, 0x0002, 0x000F); // Combo2sc CLKMUX (SPT_3)

        // Splitter
        msWrite2Byte(REG_162CDE, 0x0001); // [0] reg_spt4k_com_en

        msWrite2ByteMask(REG_162D06, bFlag?0x0000:0x0001, 0x0001); // [0]: reg_spt_420_en
        msWrite2Byte(REG_162D08, 0x0000); // [9] [8] [7] [6] [3:2] [1] [0]
        msWrite2Byte(REG_162D60, 0x0003); // [1] reg_de_only; [0] reg_mode_det_en
        msWrite2Byte(REG_162D10, wRealWidth / 2); // [12:0] reg_spt_in_de_size
        msWrite2Byte(REG_162D16, DFT_HBR2_GUARDBAND); // [12:0] reg_spt_out_gb_size = 0
        msWrite2Byte(REG_162D14, (wRealWidth / 2) + DFT_HBR2_GUARDBAND); // [12:0] reg_spt_out_de_size_with_gb (real_width/2 + reg_spt_out_gb_size)

        msWrite2Byte(REG_162D40, 0x0000); // [12:0] reg_spt_ra_st0 = 0
        msWrite2Byte(REG_162D42, ((wRealWidth / 4) / 2) - 1 + (DFT_HBR2_GUARDBAND/4)); // [12:0] reg_spt_ra_end0
        msWrite2Byte(REG_162D44, (wRealWidth / 4) / 2 - (DFT_HBR2_GUARDBAND/4)); // [12:0] reg_spt_ra_st1 = 1
        msWrite2Byte(REG_162D46, (wRealWidth / 4) - 1); // [12:0] reg_spt_ra_end1
        msWrite2Byte(REG_162D48, 0x0000); // [12:0] reg_spt_wa_st0
        msWrite2Byte(REG_162D4A, ((wRealWidth / 4) - 2)); // [12:0] reg_spt_wa_end0
        msWrite2Byte(REG_162D4C, 0x0001); // [12:0] reg_spt_wa_st1
        msWrite2Byte(REG_162D4E, (wRealWidth / 4) - 1); // [12:0] reg_spt_wa_end0
    }
    else
    {
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
    if(!bClr)
    {
        if(enInputPort == MSCombo_TMDS_PORT_0)
        {
            if ((msReadByte(REG_1605C2) & BIT0) || (msReadByte(REG_1605C4) & BIT0))
                return TRUE;
            else
                return FALSE;
        }
        else if(enInputPort == MSCombo_TMDS_PORT_1)
        {
            if ((msReadByte(REG_160AC2) & BIT0) || (msReadByte(REG_160AC4) & BIT0))
                return TRUE;
            else
                return FALSE;
        }
        else if((enInputPort == MSCombo_TMDS_PORT_2) || (enInputPort == MSCombo_TMDS_PORT_3))
        {
            if ((msReadByte(REG_1611C2) & BIT0) || (msReadByte(REG_1611C4) & BIT0))
                return TRUE;
            else
                return FALSE;
        }
        else
            return FALSE;
    }
    else
    {
        if(enInputPort == MSCombo_TMDS_PORT_0)
        {
            msWriteByteMask(REG_1605C4,0x00,0x01);
            msWriteByteMask(REG_1605C6,0x01,0x01); //[0]: clear GC pkt content chg status
            msWriteByteMask(REG_1605C6,0x00,0x01);
            return FALSE;
        }
        else if(enInputPort == MSCombo_TMDS_PORT_1)
        {
            msWriteByteMask(REG_160AC4,0x00,0x01);
            msWriteByteMask(REG_160AC6,0x01,0x01); //[0]: clear GC pkt content chg status
            msWriteByteMask(REG_160AC6,0x00,0x01);
            return FALSE;
        }
        else if((enInputPort == MSCombo_TMDS_PORT_2) || (enInputPort == MSCombo_TMDS_PORT_3))
        {
            msWriteByteMask(REG_1611C4,0x00,0x01);
            msWriteByteMask(REG_1611C6,0x01,0x01); //[0]: clear GC pkt content chg status
            msWriteByteMask(REG_1611C6,0x00,0x01);
            return FALSE;
        }
        else
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
    if(enInputPort == MSCombo_TMDS_PORT_0)
    {
        if (msReadByte(REG_16052A) & BIT0)
            return TRUE;
        else
            return FALSE;
    }
    else if(enInputPort == MSCombo_TMDS_PORT_1)
    {
        if (msReadByte(REG_160A2A) & BIT0)
            return TRUE;
        else
            return FALSE;
    }
    else if((enInputPort == MSCombo_TMDS_PORT_2) || (enInputPort == MSCombo_TMDS_PORT_3))
    {
        if (msReadByte(REG_16112A) & BIT0)
            return TRUE;
        else
            return FALSE;
    }
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
    if(msReadByte(REG_162F70) & 0x01)
    {
        TMDS_PRINT("Audio Clock has been fixed\r\n");
    }
    else
    {
        msWriteByteMask(REG_162F13, 0x00, 0x60); //KN
        switch(enAudFreq)
        {
            case MScombo_TMDS_AUD_FREQ_192K:
            case MScombo_TMDS_AUD_FREQ_176K:
                msWriteByteMask(REG_162F12, 0x11, 0xFF); //[7:4]:KM, [3:0]:KP
                msWriteByteMask(REG_162F15, 0x02, 0x0F); //DDIV
                msWriteByteMask(REG_162F15, 0x60,0xF0); //FBDIV
                msWriteByteMask(REG_162F0C, 0x00,0x40); //[6]:reg_aupll_enindiv1
            break;
            case MScombo_TMDS_AUD_FREQ_96K:
            case MScombo_TMDS_AUD_FREQ_88K:
                msWriteByteMask(REG_162F12, 0x22, 0xFF); //[7:4]:KM, [3:0]:KP
                msWriteByteMask(REG_162F15, 0x01, 0x0F); //DDIV
                msWriteByteMask(REG_162F15, 0x30,0xF0); //FBDIV
                msWriteByteMask(REG_162F0C, 0x40,0x40); //[6]:reg_aupll_enindiv1
            break;
            case MScombo_TMDS_AUD_FREQ_48K:
            case MScombo_TMDS_AUD_FREQ_44K:
            case MScombo_TMDS_AUD_FREQ_32K:
                msWriteByteMask(REG_162F12, 0x33, 0xFF); //[7:4]:KM, [3:0]:KP
                msWriteByteMask(REG_162F15, 0x01, 0x0F); //DDIV
                msWriteByteMask(REG_162F15, 0x30,0xF0); //FBDIV
                msWriteByteMask(REG_162F0C, 0x40,0x40); //[6]:reg_aupll_enindiv1
            break;
            default:
                msWriteByteMask(REG_162F12, 0x33, 0xFF); //[7:4]:KM, [3:0]:KP
                msWriteByteMask(REG_162F15, 0x01, 0x0F); //DDIV
                msWriteByteMask(REG_162F15, 0x30,0xF0); //FBDIV
                msWriteByteMask(REG_162F0C, 0x40,0x40); //[6]:reg_aupll_enindiv1
            break;
        }
    }
}

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
    if((enInputPort == MSCombo_TMDS_PORT_2) || (enInputPort == MSCombo_TMDS_PORT_3))
    {
        msWriteByteMask(REG_160C32, bFlag?0x08:0x00, 0x08); // [3]: reg_dbg_tst
    }
    else
    {
        TMDS_PRINT("Port: 0x%x, no need\r\n", enInputPort);
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_DEChange()
//  [Description]:
//                  check DE pixel change
//                  deep color FIFO reset when DE pixel change
//  [Arguments]:
//                  [MSCombo_TMDS_PORT_INDEX]PortIdx
//  [Return]:
//
//**************************************************************************
Bool mhal_tmds_DEChange(MSCombo_TMDS_PORT_INDEX enInputPort)
{
    Bool bDEChangeFlag = FALSE;

    if(enInputPort == MSCombo_TMDS_PORT_0)
    {
        if(msReadByte(REG_160504) & 0x20) // [5]: DE pixel change
        {
            msWriteByte(REG_160504, 0x20); // clear DE pixel change

            bDEChangeFlag = TRUE;
        }
    }
    else if(enInputPort == MSCombo_TMDS_PORT_1)
    {
        if(msReadByte(REG_160A04) & 0x20) // [5]: DE pixel change
        {
            msWriteByte(REG_160A04, 0x20); // clear DE pixel change

            bDEChangeFlag = TRUE;
        }
    }
    else if((enInputPort == MSCombo_TMDS_PORT_2) || (enInputPort == MSCombo_TMDS_PORT_3))
    {
        if(msReadByte(REG_161104) & 0x20) // [5]: DE pixel change
        {
            msWriteByte(REG_1611BF, 0x20); // reset deep color FIFO
            msWriteByte(REG_1611BF, 0x00); // reset deep color FIFO
            msWriteByte(REG_161104, 0x20); // clear DE pixel change

            bDEChangeFlag = TRUE;
        }
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

    switch(enInputPort)
    {
        case MSCombo_TMDS_PORT_0:

            break;

        case MSCombo_TMDS_PORT_1:

            break;

        case MSCombo_TMDS_PORT_2:
        case MSCombo_TMDS_PORT_3:
            if(enInputPort == bCurrentPort)
            {
                bStatusFlag = ((msReadByte(REG_00180E) &ucStatusMask) ?TRUE: FALSE); // REG_PM_SCDC_07
            }

            break;

        default:

            break;
    };

    if(bStatusFlag)
    {
        _mhal_tmds_ClearSCDCStatusFlag(enInputPort, ucStatusMask);
    }

    return bStatusFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_HDCP2WriteDone()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BOOL mhal_tmds_HDCP2CheckWriteDone(MSCombo_TMDS_PORT_INDEX enInputPort)
{
    // wirte IRQ done
    if(enInputPort == MSCombo_TMDS_PORT_0)
    {
        if(msReadByte(REG_1604CC) & BIT2)     // IRQ status: 66
        {
            msWriteByteMask(REG_1604CC, 0x04, 0x04); //clear wirte done status
            msWriteByteMask(REG_1604CC, 0x10, 0x10); //clear write start status

            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    else if(enInputPort == MSCombo_TMDS_PORT_1)
    {
        if(msReadByte(REG_1609CC) & BIT2)     // IRQ status: 66
        {
            msWriteByteMask(REG_1609CC, 0x04, 0x04); //clear wirte done status
            msWriteByteMask(REG_1609CC, 0x10, 0x10); //clear write start status

            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    else if((enInputPort == MSCombo_TMDS_PORT_2) || (enInputPort == MSCombo_TMDS_PORT_3))
    {
        if(msReadByte(REG_1610CC) & BIT2)     // IRQ status: 66
        {
            msWriteByteMask(REG_1610CC, 0x04, 0x04); //clear wirte done status
            msWriteByteMask(REG_1610CC, 0x10, 0x10); //clear write start status

            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    else
        return FALSE;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_HDCP2GetDataInfo()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_tmds_HDCP2GetDataInfo(MSCombo_TMDS_PORT_INDEX enInputPort, WORD ucDataSize, BYTE *pHDCPRxData)
{
    WORD i = 0;

    msWriteByteMask(REG_163080, 0x03, 0x07); //enable XIU read
    switch(enInputPort)
    {
        case MSCombo_TMDS_PORT_0:
            RIU[REG_162E08] = 0x0000; // xiu to memory access address: 04
            for ( i = 0; i < ucDataSize; i++ )
            {
                pHDCPRxData[i] = msReadByte(REG_162E1C); // memory to xiu read data (port0): 0e
            }
        break;

        case MSCombo_TMDS_PORT_1:
            RIU[REG_162E0C] = 0x0000; // xiu to memory access address: 06
            for ( i = 0; i < ucDataSize; i++ )
            {
                pHDCPRxData[i] = msReadByte(REG_162E1E); // memory to xiu read data (port1): 0f
            }
        break;

        case MSCombo_TMDS_PORT_2:
        case MSCombo_TMDS_PORT_3:
            RIU[REG_162E10] = 0x0000; // xiu to memory access address: 08
            for ( i = 0; i < ucDataSize; i++ )
            {
                pHDCPRxData[i] = msReadByte(REG_162E20); // memory to xiu read data (port0): 10
            }
        break;
        default:
            break;
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_HDCP2GetRxData()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_tmds_HDCP2GetRxData(MSCombo_TMDS_PORT_INDEX enInputPort, BYTE *pHDCPRxData)
{
    if(enInputPort == MSCombo_TMDS_PORT_0)
        mhal_tmds_HDCP2GetDataInfo(enInputPort, msRead2Byte(REG_1604C8) & 0x03FF, pHDCPRxData);  // message length receive: 64
    else if(enInputPort == MSCombo_TMDS_PORT_1)
        mhal_tmds_HDCP2GetDataInfo(enInputPort, msRead2Byte(REG_1609C8) & 0x03FF, pHDCPRxData);  // message length receive: 64
    else if((enInputPort == MSCombo_TMDS_PORT_2) || (enInputPort == MSCombo_TMDS_PORT_3))
        mhal_tmds_HDCP2GetDataInfo(enInputPort, msRead2Byte(REG_1610C8) & 0x03FF, pHDCPRxData);  // message length receive: 64
    else
    {
        TMDS_PRINT("Port: 0x%x, no need\r\n", enInputPort);
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_HDCP2WriteStart()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BOOL mhal_tmds_HDCP2CheckWriteStart(MSCombo_TMDS_PORT_INDEX enInputPort)
{
    // write IRQ start
    if(enInputPort == MSCombo_TMDS_PORT_0)
    {
        if(msReadByte(REG_1604CC) & BIT4)     // IRQ status: 66
        {
            msWriteByteMask(REG_1604CC, 0x10, 0x10);    // IRQ write start clear: 66
            msWriteByteMask(REG_1604CC, 0x00, 0x10);

            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    else if(enInputPort == MSCombo_TMDS_PORT_1)
    {
        if(msReadByte(REG_1609CC) & BIT4)     // IRQ status: 66
        {
            msWriteByteMask(REG_1609CC, 0x10, 0x10);    // IRQ write start clear: 66
            msWriteByteMask(REG_1609CC, 0x00, 0x10);

            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    else if((enInputPort == MSCombo_TMDS_PORT_2) || (enInputPort == MSCombo_TMDS_PORT_3))
    {
        if(msReadByte(REG_1610CC) & BIT4)     // IRQ status: 66
        {
            msWriteByteMask(REG_1610CC, 0x10, 0x10);    // IRQ write start clear: 66
            msWriteByteMask(REG_1610CC, 0x00, 0x10);

            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    else
        return FALSE;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_HDCP2SetTxData()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_tmds_HDCP2SetDataInfo(MSCombo_TMDS_PORT_INDEX enInputPort, BYTE MessageID, BYTE DataSize, BYTE *pHDCPTxData, BYTE *pHDCPTxData_CertRx)
{
    WORD i = 0;

    msWriteByteMask(REG_163080, 0x05, 0x07); //enable xiu write

    switch(enInputPort)
    {
        case MSCombo_TMDS_PORT_0:

            RIU[REG_162E08] = 0x0000;  // xiu to memory access address: 04

            if(MessageID == TMDS_HDCP2_MSG_ID_AKE_SEND_CERT)
            {
                msWriteByte(REG_162E0A, MessageID);  // xiu to memory wirte data: 05

                for ( i = 0; i < (522 + DataSize); i++ )
                {
                    RIU[REG_162E08] = i + 1; //set address

                    if (i < 522)
                    {
                        msWriteByte(REG_162E0A, msMemReadByte((ULONG)pHDCPTxData_CertRx + i));
                        RIU[REG_162E08] = i + 1; //set address

                        while (msReadByte(REG_162E1C) != msMemReadByte((ULONG)pHDCPTxData_CertRx + i))
                        {
                            RIU[REG_162E08] = i + 1;
                            msWriteByte(REG_162E0A, msMemReadByte((ULONG)pHDCPTxData_CertRx + i));
                            RIU[REG_162E08] = i + 1;
                        }
                    }
                    else
                    {
                        msWriteByte(REG_162E0A, *(BYTE*)(pHDCPTxData + i - 522));
                        RIU[REG_162E08] = i + 1; //set address

                        while (msReadByte(REG_162E1C) != *(BYTE*)(pHDCPTxData + i - 522))
                        {
                            RIU[REG_162E08] = i + 1;
                            msWriteByte(REG_162E0A, *(BYTE*)(pHDCPTxData + i - 522));
                            RIU[REG_162E08] = i + 1;
                        }
                    }
                }

				msWriteByte(REG_1604C6, 0x16);
				msWriteByteMask(REG_1604C7, 0x2, 0x03);

				//polling read status
				while (msReadByte(REG_160433) & BIT2);

                msWriteByteMask(REG_1604C9, 0x80, 0x80); // mdssage data length update pulse (sink)
            }
            else
            {
                msWriteByte(REG_162E0A, MessageID);  // xiu to memory wirte data: 05

                for(i = 0; i< DataSize; i++)
                {
                    RIU[REG_162E08] = i + 1; //set address
                    msWriteByte(REG_162E0A, *(BYTE*)(pHDCPTxData + i));
                    RIU[REG_162E08] = i + 1; //set address

                    while (msReadByte(REG_162E1C) != *(BYTE*)(pHDCPTxData + i))
                    {
                        RIU[REG_162E08] = i + 1;
                        msWriteByte(REG_162E0A, *(BYTE*)(pHDCPTxData + i));
                        RIU[REG_162E08] = i + 1;
                    }
                }

                msWrite2Byte(REG_1604C6, 1 + DataSize);  //  message datalength wrote by sink: 63 (1 + DataSize)
                msWriteByteMask(REG_1604C9, 0x80, 0x80); // mdssage data length update pulse (sink)
            }
        break;

        case MSCombo_TMDS_PORT_1:

            RIU[REG_162E0C] = 0x0000;  // xiu to memory access address: 06

            if(MessageID == TMDS_HDCP2_MSG_ID_AKE_SEND_CERT)
            {
                msWriteByte(REG_162E0E, MessageID);  // xiu to memory wirte data: 07

                for ( i = 0; i < (522 + DataSize); i++ )
                {
                    RIU[REG_162E0C] = i + 1; //set address

                    if (i < 522)
                    {
                        msWriteByte(REG_162E0E, msMemReadByte((ULONG)pHDCPTxData_CertRx + i));
                        RIU[REG_162E0C] = i + 1; //set address

                        while (msReadByte(REG_162E1E) != msMemReadByte((ULONG)pHDCPTxData_CertRx + i))
                        {
                            RIU[REG_162E0C] = i + 1;
                            msWriteByte(REG_162E0E, msMemReadByte((ULONG)pHDCPTxData_CertRx + i));
                            RIU[REG_162E0C] = i + 1;
                        }
                    }
                    else
                    {
                        msWriteByte(REG_162E0E, *(BYTE*)(pHDCPTxData + i - 522));
                        RIU[REG_162E0C] = i + 1; //set address

                        while (msReadByte(REG_162E1E) != *(BYTE*)(pHDCPTxData + i - 522))
                        {
                            RIU[REG_162E0C] = i + 1;
                            msWriteByte(REG_162E0E, *(BYTE*)(pHDCPTxData + i - 522));
                            RIU[REG_162E0C] = i + 1;
                        }
                    }
                }

				msWriteByte(REG_1609C6, 0x16);
				msWriteByteMask(REG_1609C7, 0x2, 0x03);

				//polling read status
				while (msReadByte(REG_160933) & BIT2);

                msWriteByteMask(REG_1609C9, 0x80, 0x80); // mdssage data length update pulse (sink)
            }
            else
            {
                msWriteByte(REG_162E0E, MessageID);  // xiu to memory wirte data: 07

                for(i = 0; i< DataSize; i++)
                {
                    RIU[REG_162E0C] = i + 1; //set address
                    msWriteByte(REG_162E0E, *(BYTE*)(pHDCPTxData + i));
                    RIU[REG_162E0C] = i + 1; //set address

                    while (msReadByte(REG_162E1E) != *(BYTE*)(pHDCPTxData + i))
                    {
                        RIU[REG_162E0C] = i + 1;
                        msWriteByte(REG_162E0E, *(BYTE*)(pHDCPTxData + i));
                        RIU[REG_162E0C] = i + 1;
                    }
                }

                msWrite2Byte(REG_1609C6, 1 + DataSize);  //  message datalength wrote by sink: 63 (1 + DataSize)
                msWriteByteMask(REG_1609C9, 0x80, 0x80); // mdssage data length update pulse (sink)
            }
        break;

        case MSCombo_TMDS_PORT_2:
        case MSCombo_TMDS_PORT_3:

            RIU[REG_162E10] = 0x0000;  // xiu to memory access address: 08

            if(MessageID == TMDS_HDCP2_MSG_ID_AKE_SEND_CERT)
            {
                msWriteByte(REG_162E12, MessageID);  // xiu to memory wirte data: 09

                for ( i = 0; i < (522 + DataSize); i++ )
                {
                    RIU[REG_162E10] = i + 1; //set address

                    if (i < 522)
                    {
                        msWriteByte(REG_162E12, msMemReadByte((ULONG)pHDCPTxData_CertRx + i));
                        RIU[REG_162E10] = i + 1; //set address

                        while (msReadByte(REG_162E20) != msMemReadByte((ULONG)pHDCPTxData_CertRx + i))
                        {
                            RIU[REG_162E10] = i + 1;
                            msWriteByte(REG_162E12, msMemReadByte((ULONG)pHDCPTxData_CertRx + i));
                            RIU[REG_162E10] = i + 1;
                        }
                    }
                    else
                    {
                        msWriteByte(REG_162E12, *(BYTE*)(pHDCPTxData + i - 522));
                        RIU[REG_162E10] = i + 1; //set address

                        while (msReadByte(REG_162E20) != *(BYTE*)(pHDCPTxData + i - 522))
                        {
                            RIU[REG_162E10] = i + 1;
                            msWriteByte(REG_162E12, *(BYTE*)(pHDCPTxData + i - 522));
                            RIU[REG_162E10] = i + 1;
                        }
                    }
                }

				msWriteByte(REG_1610C6, 0x16);
				msWriteByteMask(REG_1610C7, 0x02, 0x03);

				//polling read status
				while (msReadByte(REG_161033) & BIT2);

                msWriteByteMask(REG_1610C9, 0x80, 0x80); // mdssage data length update pulse (sink)
            }
            else
            {
                msWriteByte(REG_162E12, MessageID);  // xiu to memory wirte data: 09

                for(i = 0; i< DataSize; i++)
                {
                    RIU[REG_162E10] = i + 1; //set address
                    msWriteByte(REG_162E12, *(BYTE*)(pHDCPTxData + i));
                    RIU[REG_162E10] = i + 1; //set address

                    while (msReadByte(REG_162E20) != *(BYTE*)(pHDCPTxData + i))
                    {
                        RIU[REG_162E10] = i + 1;
                        msWriteByte(REG_162E12, *(BYTE*)(pHDCPTxData + i));
                        RIU[REG_162E10] = i + 1;
                    }
                }

                msWrite2Byte(REG_1610C6, 1 + DataSize);  //  message datalength wrote by sink: 63 (1 + DataSize)
                msWriteByteMask(REG_1610C9, 0x80, 0x80); // mdssage data length update pulse (sink)

            }

        break;

        default:
        break;
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_HDCP2SetTxData()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_tmds_HDCP2SetTxData(MSCombo_TMDS_PORT_INDEX enInputPort, BYTE MessageID, BYTE DataSize, BYTE *pHDCPTxData, BYTE *pHDCPTxData_CertRx)
{
    mhal_tmds_HDCP2SetDataInfo(enInputPort, MessageID, DataSize, pHDCPTxData, pHDCPTxData_CertRx);
}

#endif // _MHAL_TMDS_C_


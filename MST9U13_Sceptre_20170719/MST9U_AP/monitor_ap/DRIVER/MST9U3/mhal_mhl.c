///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    mhal_mhl.c
/// @author MStar Semiconductor Inc.
/// @brief  MHL driver Function
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MHAL_MHL_C_
#define _MHAL_MHL_C_

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "Global.h"
#include "mapi_mhl.h"
#include "mdrv_mhl.h"
#include "mhal_mhl.h"
#include "msEread.h"

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define MHL_HAL_DEBUG_MESSAGE           0

#if(MHL_DEBUG_MESSAGE && MHL_HAL_DEBUG_MESSAGE)
#define MHL_HAL_DPUTSTR(str)            printMsg(str)
#define MHL_HAL_DPRINTF(str, x)         printData(str, x)
#else
#define MHL_HAL_DPUTSTR(str)
#define MHL_HAL_DPRINTF(str, x)
#endif

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
typedef struct
{
    WORD addr;
    WORD databuf;
    WORD mask;
} msLoadTbl_S;

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
Bool bSignalStable[4] = {TRUE, TRUE, TRUE, TRUE};
Bool bMHLPHYSettingFlag = FALSE;
BYTE ucCableDetectEnable = 0;
BYTE ucCableDetectIndex = 0;
BYTE ucCurrentDisplayPort = 0;
BYTE ucECbusMuxSelect = MHL_ECBUS_MUX_SELECT_NONE;
BYTE ucECbusEnableIndex = 0;
BYTE ucECbusVersionIndex = 0;
WORD usECbusIDIndex = 0;
BYTE ucECbusStatePort[4] = {0, 0, 0, 0};
BYTE ucECbusTrainFailCount[4] = {0, 0, 0, 0};
BYTE ucLinkRatePort[4] = {MHL_AV_LINK_NONE, MHL_AV_LINK_NONE, MHL_AV_LINK_NONE, MHL_AV_LINK_NONE};
BYTE uMHLResetClkCnt = 0;

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_GetECbusMux()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
BYTE _mhal_mhl_GetECbusMux(BYTE ucPortSelect)
{
    BYTE ucECbusMux = MHL_ECBUS_MUX_SELECT_NONE;
    
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            ucECbusMux = MHL_ECBUS_MUX_SELECT_PORT2;
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            ucECbusMux = MHL_ECBUS_MUX_SELECT_PORT3;
            break;
#endif

        default:

            break;
    };

    return ucECbusMux;
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_CheckCurrentPort()
//  [Description]
//                  
//  [Arguments]:
//                  
//  [Return]:
//                  
//**************************************************************************
Bool _mhal_mhl_CheckCurrentPort(BYTE ucPortSelect)
{
    Bool bOnLinePort = FALSE;

    if(_mhal_mhl_GetECbusMux(ucPortSelect) == GET_MHL_ECBUS_MUX_SELECT()) // Display port ECbus mux equal to current ECbus mux
    {
        if(GET_MHL_ECBUS_MUX_SELECT() != MHL_ECBUS_MUX_SELECT_NONE)
        {
            bOnLinePort = TRUE;
        }
    }

    return bOnLinePort;
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_GetNextECbusMux()
//  [Description]
//                  
//  [Arguments]:
//                  
//  [Return]:
//                  
//**************************************************************************
BYTE _mhal_mhl_GetNextECbusMux(BYTE ucPortSelect)
{
    BYTE ucInputPort = MHL_INPUT_SELECT_NONE;
    BYTE ucECbusMux = MHL_ECBUS_MUX_SELECT_NONE;

    for(ucInputPort = ucPortSelect +1; ucInputPort < (ucPortSelect +MHL_INPUT_SELECT_MASK); ucInputPort++)
    {
        if(GET_MHL_CABLE_DETECT_PORT(ucInputPort %MHL_INPUT_SELECT_MASK))
        {
            ucECbusMux = _mhal_mhl_GetECbusMux(ucInputPort %MHL_INPUT_SELECT_MASK);

            if(ucECbusMux != MHL_ECBUS_MUX_SELECT_NONE) // ECbus port
            {
                break;
            }
        }
    }

    return ucECbusMux;
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_CableDetectEnable()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_CableDetectEnable(BYTE ucPortSelect)
{
    // Cbus pad config
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            msWrite2ByteMask(REG_000462, 0, BIT8); // REG_PAD_TOP_31[8], enable pad pull-high port0            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            msWrite2ByteMask(REG_000462, 0, BIT6); // REG_PAD_TOP_31[6], enable pad pull-high port1
            break;
#endif

        case MHL_INPUT_SELECT_PORT2:
            msWrite2ByteMask(REG_000462, 0, BIT4); // REG_PAD_TOP_31[4], enable pad pull-high port2
            break;
            
        case MHL_INPUT_SELECT_PORT3:
            msWrite2ByteMask(REG_000462, 0, BIT2); // REG_PAD_TOP_31[2], enable pad pull-high port3
            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_GetEMSCOneByteCRC()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE _mhal_mhl_GetEMSCOneByteCRC(BYTE ucOpCode)
{
    BYTE ucAValue = 0;
    BYTE ucBValue = 0;
    BYTE ucCommand = BIT7 |BIT6 |((ucOpCode &MASKBIT(2:0)) << 2);

    ucAValue = GET_BIT(ucCommand &BIT7) +GET_BIT(ucCommand &BIT5) +GET_BIT(ucCommand &BIT3) +2;
    ucBValue = GET_BIT(ucCommand &BIT6) +GET_BIT(ucCommand &BIT4) +GET_BIT(ucCommand &BIT2) +2;

    if(ucAValue &BIT0)
    {
        ucCommand |= BIT1;
    }

    if(ucBValue &BIT0)
    {
        ucCommand |= BIT0;
    }

    return ucCommand;
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_GetEMSCTwoByteCRC()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE _mhal_mhl_GetEMSCTwoByteCRC(BYTE ucOpCode, BYTE ucValue)
{
    BYTE ucAValue = 0;
    BYTE ucBValue = 0;
    BYTE ucCValue = 0;
    WORD ucCommand = BIT13 |(((WORD)ucOpCode &MASKBIT(1:0)) << 11) |(((WORD)ucValue &MASKBIT(7:0)) << 3);

    ucAValue = GET_BIT(ucCommand &BIT15) +GET_BIT(ucCommand &BIT13) +GET_BIT(ucCommand &BIT12) +GET_BIT(ucCommand &BIT11) +GET_BIT(ucCommand &BIT8);
    ucAValue = ucAValue +GET_BIT(ucCommand &BIT6) +GET_BIT(ucCommand &BIT5) +GET_BIT(ucCommand &BIT4) +3;
    ucBValue = GET_BIT(ucCommand &BIT15) +GET_BIT(ucCommand &BIT14) +GET_BIT(ucCommand &BIT13) +GET_BIT(ucCommand &BIT10) +GET_BIT(ucCommand &BIT8);
    ucBValue = ucBValue +GET_BIT(ucCommand &BIT7) +GET_BIT(ucCommand &BIT6) +GET_BIT(ucCommand &BIT3) +2;
    ucCValue = GET_BIT(ucCommand &BIT14) +GET_BIT(ucCommand &BIT13) +GET_BIT(ucCommand &BIT12) +GET_BIT(ucCommand &BIT9) +GET_BIT(ucCommand &BIT7);
    ucCValue = ucCValue +GET_BIT(ucCommand &BIT6) +GET_BIT(ucCommand &BIT5) +2;
    
    if(ucAValue &BIT0)
    {
        ucCommand |= BIT2;
    }

    if(ucBValue &BIT0)
    {
        ucCommand |= BIT1;
    }

    if(ucCValue &BIT0)
    {
        ucCommand |= BIT0;
    }

    return (ucCommand &MASKBIT(7:0));
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_GetEMSCPayloadCRC()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
WORD _mhal_mhl_GetEMSCPayloadCRC(BYTE ucOpCode, WORD usCRCValus)
{
    BYTE ucAValue = 0;
    BYTE ucBValue = 0;
    BYTE ucCValue = GET_BIT(ucOpCode &BIT7) +GET_BIT(ucOpCode &BIT6) +GET_BIT(usCRCValus &BIT6) +GET_BIT(usCRCValus &BIT7);
    BYTE ucDValue = GET_BIT(ucOpCode &BIT6) +GET_BIT(ucOpCode &BIT5) +GET_BIT(usCRCValus &BIT5) +GET_BIT(usCRCValus &BIT6);
    BYTE ucEValue = GET_BIT(ucOpCode &BIT5) +GET_BIT(ucOpCode &BIT4) +GET_BIT(usCRCValus &BIT4) +GET_BIT(usCRCValus &BIT5);
    BYTE ucFValue = GET_BIT(ucOpCode &BIT4) +GET_BIT(ucOpCode &BIT3) +GET_BIT(usCRCValus &BIT3) +GET_BIT(usCRCValus &BIT4);
    BYTE ucGValue = GET_BIT(ucOpCode &BIT3) +GET_BIT(ucOpCode &BIT2) +GET_BIT(usCRCValus &BIT2) +GET_BIT(usCRCValus &BIT3);
    BYTE ucHValue = GET_BIT(ucOpCode &BIT2) +GET_BIT(ucOpCode &BIT1) +GET_BIT(usCRCValus &BIT1) +GET_BIT(usCRCValus &BIT2);
    BYTE ucIValue = GET_BIT(ucOpCode &BIT1) +GET_BIT(ucOpCode &BIT0) +GET_BIT(usCRCValus &BIT0) +GET_BIT(usCRCValus &BIT1) +GET_BIT(usCRCValus &BIT15);
    BYTE ucJValue = GET_BIT(ucOpCode &BIT0) +GET_BIT(usCRCValus &BIT0) +GET_BIT(usCRCValus &BIT14);
    BYTE ucKValue = GET_BIT(usCRCValus &BIT13);
    BYTE ucLValue = GET_BIT(usCRCValus &BIT12);
    BYTE ucMValue = GET_BIT(usCRCValus &BIT11);
    BYTE ucNValue = GET_BIT(usCRCValus &BIT10);
    BYTE ucOValue = GET_BIT(usCRCValus &BIT9);
    BYTE ucPValue = 0;
    WORD usCommand = 0;
    
    ucAValue = GET_BIT(ucOpCode &BIT7) +GET_BIT(ucOpCode &BIT6) +GET_BIT(ucOpCode &BIT5) +GET_BIT(ucOpCode &BIT4) +GET_BIT(ucOpCode &BIT3) +GET_BIT(ucOpCode &BIT2) +GET_BIT(ucOpCode &BIT1) +GET_BIT(ucOpCode &BIT0);
    ucAValue = ucAValue +GET_BIT(usCRCValus &BIT0) +GET_BIT(usCRCValus &BIT1) +GET_BIT(usCRCValus &BIT2) +GET_BIT(usCRCValus &BIT3) +GET_BIT(usCRCValus &BIT4) +GET_BIT(usCRCValus &BIT5) +GET_BIT(usCRCValus &BIT6) +GET_BIT(usCRCValus &BIT7);
    ucBValue = GET_BIT(ucOpCode &BIT6) +GET_BIT(ucOpCode &BIT5) +GET_BIT(ucOpCode &BIT4) +GET_BIT(ucOpCode &BIT3) +GET_BIT(ucOpCode &BIT2) +GET_BIT(ucOpCode &BIT1) +GET_BIT(ucOpCode &BIT0);
    ucBValue = ucBValue +GET_BIT(usCRCValus &BIT0) +GET_BIT(usCRCValus &BIT1) +GET_BIT(usCRCValus &BIT2) +GET_BIT(usCRCValus &BIT3) +GET_BIT(usCRCValus &BIT4) +GET_BIT(usCRCValus &BIT5) +GET_BIT(usCRCValus &BIT6);
    ucPValue = ucAValue +GET_BIT(usCRCValus &BIT8);
    
    if(ucAValue &BIT0)
    {
        usCommand |= BIT15;
    }

    if(ucBValue &BIT0)
    {
        usCommand |= BIT14;
    }

    if(ucCValue &BIT0)
    {
        usCommand |= BIT13;
    }

    if(ucDValue &BIT0)
    {
        usCommand |= BIT12;
    }

    if(ucEValue &BIT0)
    {
        usCommand |= BIT11;
    }

    if(ucFValue &BIT0)
    {
        usCommand |= BIT10;
    }

    if(ucGValue &BIT0)
    {
        usCommand |= BIT9;
    }

    if(ucHValue &BIT0)
    {
        usCommand |= BIT8;
    }

    if(ucIValue &BIT0)
    {
        usCommand |= BIT7;
    }

    if(ucJValue &BIT0)
    {
        usCommand |= BIT6;
    }

    if(ucKValue &BIT0)
    {
        usCommand |= BIT5;
    }

    if(ucLValue &BIT0)
    {
        usCommand |= BIT4;
    }

    if(ucMValue &BIT0)
    {
        usCommand |= BIT3;
    }

    if(ucNValue &BIT0)
    {
        usCommand |= BIT2;
    }

    if(ucOValue &BIT0)
    {
        usCommand |= BIT1;
    }

    if(ucPValue &BIT0)
    {
        usCommand |= BIT0;
    }

    return usCommand;
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_CbusPadSetting()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_CbusPadSetting(BYTE ucPortSelect, BYTE ucMHLVersion)
{
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            msWrite2ByteMask(REG_002F62, BIT11, BIT11); // REG_PM_MHL_CBUS0_31[11]
            // Set accepted discover pulse high pulse width to ignore USB pulse
            msWrite2ByteMask(REG_002F18, 0x0046, MASKBIT(15:0)); // REG_PM_MHL_CBUS0_0C[15:0], reject cbus discovery pulse below this limit
            msWrite2ByteMask(REG_002F1A, 0x0082, MASKBIT(15:0)); // REG_PM_MHL_CBUS0_0D[15:0], reject cbus discovery pulse above this limit
            // CTS 4.3.7.1
            msWrite2ByteMask(REG_002EAA, BIT4, MASKBIT(7:4)); // REG_MHL_CBUS0_55[7:4]: cbus requester transmit opportunity after arbitration
            msWrite2ByteMask(REG_002EE0, BIT14, BIT14); // REG_MHL_CBUS0_70[14]: let edid current read initial address add 1
            // For CTS 6.3.10.7
            msWrite2ByteMask(REG_002EB2, 0xADB0, MASKBIT(15:0)); // REG_MHL_CBUS0_59[15:0], timeout for a device receiving a packet within a command
            msWrite2ByteMask(REG_002EB4, 0x0001, MASKBIT(15:0)); // REG_MHL_CBUS0_5A[15:0], modify to 110ms
            // HW auto reply setting
            msWrite2ByteMask(REG_002E46, BIT10 |BIT4, MASKBIT(10:8) |BIT4); // REG_MHL_CBUS0_23[10]: enable HW auto response read_devcap request, [9:8]: old mode RCP/RAP HW auto response, [4]: enable HW responses wrt_grt_int.
            msWrite2ByteMask(REG_002EDA, MASKBIT(11:0), MASKBIT(11:0)); // REG_MHL_CBUS0_6D[11:0], Enable MHL HW mode
            msWrite2ByteMask(REG_002EE2, MASKBIT(15:14), MASKBIT(15:14)); // REG_MHL_CBUS0_71[15:14], [15]: Enable msc hw auto response., [14]:Auto response packet after SW clr fifo or Xdata.
            msWrite2ByteMask(REG_002EA0, BIT13, BIT13); // REG_MHL_CBUS0_50[13]: MSC sending command keep mode once auto response is onging
            // Xdata enable
            msWrite2ByteMask(REG_002ED8, BIT14, BIT14); // REG_MHL_CBUS0_6C[14]: Register xdata enable
            // Extend wake up pulse width
            msWrite2ByteMask(REG_002F22, 0x88B8, MASKBIT(15:0)); // extend the maxi wakeup pulse width to 35msec 
            // Free force to 1K
            msWrite2ByteMask(REG_002F02, 0, MASKBIT(5:2)); // REG_PM_MHL_CBUS0_01[5][3][2]

            msWrite2ByteMask(REG_000462, 0, BIT7); // REG_PAD_TOP_31[7], enable pad pull-high port0 
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            msWrite2ByteMask(REG_003162, BIT11, BIT11); // REG_PM_MHL_CBUS1_31[11]
            // Set accepted discover pulse high pulse width to ignore USB pulse
            msWrite2ByteMask(REG_003118, 0x0046, MASKBIT(15:0)); // REG_PM_MHL_CBUS1_0C[15:0], reject cbus discovery pulse below this limit
            msWrite2ByteMask(REG_00311A, 0x0082, MASKBIT(15:0)); // REG_PM_MHL_CBUS1_0D[15:0], reject cbus discovery pulse above this limit
            // CTS 4.3.7.1
            msWrite2ByteMask(REG_0030AA, BIT4, MASKBIT(7:4)); // REG_MHL_CBUS1_55[7:4]: cbus requester transmit opportunity after arbitration
            msWrite2ByteMask(REG_0030E0, BIT14, BIT14); // REG_MHL_CBUS1_70[14]: let edid current read initial address add 1
            // For CTS 6.3.10.7
            msWrite2ByteMask(REG_0030B2, 0xADB0, MASKBIT(15:0)); // REG_MHL_CBUS1_59[15:0], timeout for a device receiving a packet within a command
            msWrite2ByteMask(REG_0030B4, 0x0001, MASKBIT(15:0)); // REG_MHL_CBUS1_5A[15:0], modify to 110ms
            // HW auto reply setting
            msWrite2ByteMask(REG_003046, BIT10 |BIT4, MASKBIT(10:8) |BIT4); // REG_MHL_CBUS1_23[10]: enable HW auto response read_devcap request, [9:8]: old mode RCP/RAP HW auto response, [4]: enable HW responses wrt_grt_int.
            msWrite2ByteMask(REG_0030DA, MASKBIT(11:0), MASKBIT(11:0)); // REG_MHL_CBUS1_6D[11:0], Enable MHL HW mode
            msWrite2ByteMask(REG_0030E2, MASKBIT(15:14), MASKBIT(15:14)); // REG_MHL_CBUS1_71[15:14], [15]: Enable msc hw auto response., [14]:Auto response packet after SW clr fifo or Xdata.
            msWrite2ByteMask(REG_0030A0, BIT13, BIT13); // REG_MHL_CBUS1_50[13]: MSC sending command keep mode once auto response is onging
            // Xdata enable
            msWrite2ByteMask(REG_0030D8, BIT14, BIT14); // REG_MHL_CBUS1_6C[14]: Register xdata enable
            // Extend wake up pulse width
            msWrite2ByteMask(REG_003122, 0x88B8, MASKBIT(15:0)); // extend the maxi wakeup pulse width to 35msec 
            // Free force to 1K
            msWrite2ByteMask(REG_003102, 0, MASKBIT(5:2)); // REG_PM_MHL_CBUS1_01[5][3][2]

            msWrite2ByteMask(REG_000462, 0, BIT0); // REG_PAD_TOP_31[0], enable pad pull-high port1 
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2 ||MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT2:
        case MHL_INPUT_SELECT_PORT3:
            msWrite2ByteMask(REG_003362, BIT11, BIT11); // REG_PM_MHL_CBUS2_31[11]
            // Set accepted discover pulse high pulse width to ignore USB pulse
            msWrite2ByteMask(REG_003318, 0x0046, MASKBIT(15:0)); // REG_PM_MHL_CBUS2_0C[15:0], reject cbus discovery pulse below this limit
            msWrite2ByteMask(REG_00331A, 0x0082, MASKBIT(15:0)); // REG_PM_MHL_CBUS2_0D[15:0], reject cbus discovery pulse above this limit
            // CTS 4.3.7.1
            msWrite2ByteMask(REG_0032AA, BIT4, MASKBIT(7:4)); // REG_MHL_CBUS2_55[7:4]: cbus requester transmit opportunity after arbitration
            msWrite2ByteMask(REG_0032E0, BIT14, BIT14); // REG_MHL_CBUS2_70[14]: let edid current read initial address add 1
            // For CTS 6.3.10.7
            msWrite2ByteMask(REG_0032B2, 0xADB0, MASKBIT(15:0)); // REG_MHL_CBUS2_59[15:0], timeout for a device receiving a packet within a command
            msWrite2ByteMask(REG_0032B4, 0x0001, MASKBIT(15:0)); // REG_MHL_CBUS2_5A[15:0], modify to 110ms
            // HW auto reply setting
            msWrite2ByteMask(REG_003246, BIT10 |BIT4, MASKBIT(10:8) |BIT4); // REG_MHL_CBUS2_23[10]: enable HW auto response read_devcap request, [9:8]: old mode RCP/RAP HW auto response, [4]: enable HW responses wrt_grt_int.
            msWrite2ByteMask(REG_0032DA, MASKBIT(11:0), MASKBIT(11:0)); // REG_MHL_CBUS2_6D[11:0], Enable MHL HW mode
            msWrite2ByteMask(REG_0032E2, MASKBIT(15:14), MASKBIT(15:14)); // REG_MHL_CBUS2_71[15:14][11:10], [15]: Enable msc hw auto response., [14]:Auto response packet after SW clr fifo or Xdata.
            msWrite2ByteMask(REG_0032A0, BIT13, BIT13); // REG_MHL_CBUS2_50[13]: MSC sending command keep mode once auto response is onging
            // Xdata enable
            msWrite2ByteMask(REG_0032D8, BIT14, BIT14); // REG_MHL_CBUS2_6C[14]: Register xdata enable
            // Extend wake up pulse width
            msWrite2ByteMask(REG_003322, 0x88B8, MASKBIT(15:0)); // extend the maxi wakeup pulse width to 35msec 
            // Free force to 1K
            msWrite2ByteMask(REG_003302, 0, MASKBIT(5:2)); // REG_PM_MHL_CBUS2_01[5][3][2]

            if(ucMHLVersion >= 0x30)
            {
                msWrite2ByteMask(REG_0032E2, MASKBIT(11:10), MASKBIT(11:10)); // REG_MHL_CBUS2_71[11:10], [11]: 
            }
            
            msWriteByteMask(REG_0022EE, _mhal_mhl_GetEMSCOneByteCRC(4), MASKBIT(7:0)); // REG_MHL_ECBUS2_77[7:0]:ACK
            msWriteByteMask(REG_0022EF, _mhal_mhl_GetEMSCOneByteCRC(5), MASKBIT(7:0)); // REG_MHL_ECBUS2_77[15:8]:NACK
            msWriteByteMask(REG_0022F0, _mhal_mhl_GetEMSCOneByteCRC(6), MASKBIT(7:0)); // REG_MHL_ECBUS2_78[7:0]:GRANT
            msWriteByteMask(REG_0022F1, _mhal_mhl_GetEMSCOneByteCRC(7), MASKBIT(7:0)); // REG_MHL_ECBUS2_78[15:8]:DSTART

            msWriteByteMask(REG_0022F2,(_mhal_mhl_GetEMSCTwoByteCRC(1, 1) << 3) , MASKBIT(5:3)); // REG_MHL_ECBUS2_79[5:3]:command error CRC
            msWrite2ByteMask(REG_0022F2, (_mhal_mhl_GetEMSCTwoByteCRC(1, 2) << 6), MASKBIT(8:6)); // REG_MHL_ECBUS2_79[8:6]:payload error CRC

            msWriteByteMask(REG_002233, BIT7, BIT7); // REG_MHL_ECBUS2_19, [15]:EMSC send data CRC ove

            msWrite2ByteMask(REG_000462, 0, BIT3); // REG_PAD_TOP_31[3], enable pad pull-high port2 
            msWrite2ByteMask(REG_000462, 0, BIT5); // REG_PAD_TOP_31[5], enable pad pull-high port3 

            msWrite2ByteMask(REG_003340, 0, BIT10|BIT2);// REG_MHL_CBUS2_20[10][2], [10]:PAD_pdas08pdgz_cbus current drive bit 1, [2]:PAD_pdas08pdgz_cbus current drive
            msWrite2ByteMask(REG_003540, 0, BIT10|BIT2);// REG_MHL_CBUS3_20[10][2], [10]:PAD_pdas08pdgz_cbus current drive bit 1, [2]:PAD_pdas08pdgz_cbus current drive

            break;
#endif
       
        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_LoadDevcapVenderID()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_LoadDevcapVenderID(BYTE ucPortSelect, BYTE *pDevcap, BYTE ucVenderID)
{
    BYTE ucIndex = 0;
    BYTE uctemp = 0; 

    MHL_HAL_DPRINTF("** MHL load DevCap... port %d", ucPortSelect); 

    if(pDevcap != NULL)
    {
        switch(ucPortSelect)
        {
#if(MHL_FUNCTION_SUPPORT_PORT0)
            case MHL_INPUT_SELECT_PORT0:
                // Load vendor ID
                msWriteByte(REG_002E01, ucVenderID); // REG_MHL_CBUS0_00[15:8], MHL_VENDOR_ID

                // Load device capability
                for(uctemp = 0; uctemp <8; uctemp++)
                {
                    if(uctemp == 2)
                    {
                        ucIndex = 1;
                    }
                    
                    msWrite2Byte(REG_002E02 +(uctemp *2), (pDevcap[(uctemp *2) +1 -ucIndex] <<8) | pDevcap[uctemp *2 -ucIndex]);
                }
                
                msWrite2Byte(REG_002E06, (pDevcap[3] <<8) | pDevcap[4]); // REG_MHL_CBUS0_03[15:0], MHL_ADOPTER_ID
                msWrite2Byte(REG_002E0E, (pDevcap[11] <<8) | pDevcap[12]); // REG_MHL_CBUS0_07[15:0], MHL_DEVICE_ID
                msWriteByte(REG_002E12, pDevcap[15]); // REG_MHL_CBUS0_09[7:0], reg_devcap_0f
                
                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
            case MHL_INPUT_SELECT_PORT1:
                // Load vendor ID
                msWriteByte(REG_003001, ucVenderID); // REG_MHL_CBUS1_00[15:8], MHL_VENDOR_ID

                // Load device capability
                for(uctemp = 0; uctemp <8; uctemp++)
                {
                    if(uctemp == 2)
                    {
                        ucIndex = 1;
                    }
                    
                    msWrite2Byte(REG_003002 +(uctemp *2), (pDevcap[(uctemp *2) +1 -ucIndex] <<8) | pDevcap[uctemp *2 -ucIndex]);
                }
                
                msWrite2Byte(REG_003006, (pDevcap[3] <<8) | pDevcap[4]); // REG_MHL_CBUS1_03[15:0], MHL_ADOPTER_ID
                msWrite2Byte(REG_00300E, (pDevcap[11] <<8) | pDevcap[12]); // REG_MHL_CBUS1_07[15:0], MHL_DEVICE_ID
                msWriteByte(REG_003012, pDevcap[15]); // REG_MHL_CBUS1_09[7:0], reg_devcap_0f
                
                break;
#endif

            case MHL_INPUT_SELECT_PORT2:
            case MHL_INPUT_SELECT_PORT3:
                // Load vendor ID
                msWriteByte(REG_003201, ucVenderID); // REG_MHL_CBUS2_00[15:8], MHL_VENDOR_ID

                // Load device capability
                for(uctemp = 0; uctemp <8; uctemp++)
                {
                    if(uctemp == 2)
                    {
                        ucIndex = 1;
                    }
                    
                    msWrite2Byte(REG_003202 +(uctemp *2), (pDevcap[(uctemp *2) +1 -ucIndex] <<8) | pDevcap[uctemp *2 -ucIndex]);
                }
                
                msWrite2Byte(REG_003206, (pDevcap[3] <<8) | pDevcap[4]); // REG_MHL_CBUS2_03[15:0], MHL_ADOPTER_ID
                msWrite2Byte(REG_00320E, (pDevcap[11] <<8) | pDevcap[12]); // REG_MHL_CBUS2_07[15:0], MHL_DEVICE_ID
                msWriteByte(REG_003212, pDevcap[15]); // REG_MHL_CBUS2_09[7:0], reg_devcap_0f

                for(uctemp = 0; uctemp <2; uctemp++)
                {
                    msWrite2Byte(REG_003232 +(uctemp *2), (pDevcap[(uctemp *2) +17] <<8) | pDevcap[(uctemp *2) +16]); // REG_MHL_CBUS2_19[15:0], MHL_ECBUS_SPEEDS
                }
                
                break;

            default:

                break;
        };
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_ComboPhySetting()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_ComboPhySetting(BYTE ucPortSelect)
{
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            msWrite2ByteMask(REG_11209E, MHL_24BIT_VCO_THRESHOLD1, MASKBIT(15:0)); // REG_COMBO_PHY0_4F[15:0], 24bit mode clock threshold1
            msWrite2ByteMask(REG_11209C, MHL_PPMODE_VCO_THRESHOLD1, MASKBIT(15:0)); // REG_COMBO_PHY0_4E[15:0], PP mode clock threshold1
            msWrite2ByteMask(REG_11209A, MHL_PPMODE_VCO_THRESHOLD2, MASKBIT(15:0)); // REG_COMBO_PHY0_4D[15:0], PP mode clock threshold2
            msWrite2ByteMask(REG_1120DE, BIT10, BIT10); // REG_COMBO_PHY0_6F[10], port0 i-control PD overwrite
            msWrite2ByteMask(REG_112120, BIT4, BIT4); // REG_COMBO_PHY1_10[4], port0 atop_dvipll_ictl_ov overwrite enable
            msWrite2ByteMask(REG_112124, MHL_I_CONTROL_2_VALUE, MASKBIT(3:0)); // REG_COMBO_PHY1_12[3:0], port0 reg_atop_dvipll_ictl_ov

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            msWrite2ByteMask(REG_11229E, MHL_24BIT_VCO_THRESHOLD1, MASKBIT(15:0)); // REG_COMBO_PHY2_4F[15:0], 24bit mode clock threshold
            msWrite2ByteMask(REG_11229C, MHL_PPMODE_VCO_THRESHOLD1, MASKBIT(15:0)); // REG_COMBO_PHY2_4E[15:0], PP mode clock threshold1
            msWrite2ByteMask(REG_11229A, MHL_PPMODE_VCO_THRESHOLD2, MASKBIT(15:0)); // REG_COMBO_PHY2_4D[15:0], PP mode clock threshold2
            msWrite2ByteMask(REG_1122DE, BIT10, BIT10); // REG_COMBO_PHY2_6F[10], port1 i-control PD overwrite
            msWrite2ByteMask(REG_112320, BIT4, BIT4); // REG_COMBO_PHY3_10[4], port0 atop_dvipll_ictl_ov overwrite enable
            msWrite2ByteMask(REG_112324, MHL_I_CONTROL_2_VALUE, MASKBIT(3:0)); // REG_COMBO_PHY3_12[3:0], port0 reg_atop_dvipll_ictl_ov
            
            break;
#endif

        case MHL_INPUT_SELECT_PORT2:
        case MHL_INPUT_SELECT_PORT3:
            msWriteByteMask(REG_160CC1, BIT1, MASKBIT(2:1)); // REG_DVI_DTOP_DUAL_60[10:9]: 
            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_SetCableDetect()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_SetCableDetect(BYTE ucPortSelect, Bool bFlag)
{
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            if(bFlag)
            {
                msWriteByteMask(REG_002F01, MASKBIT(1:0), MASKBIT(1:0)); // REG_PM_MHL_CBUS0_00[9:8], SW control cable detect
            }
            else
            {
                msWriteByteMask(REG_002F01, BIT1, MASKBIT(1:0)); // REG_PM_MHL_CBUS0_00[9:8], SW control cable detect
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            if(bFlag)
            {
                msWriteByteMask(REG_003101, MASKBIT(1:0), MASKBIT(1:0)); // REG_PM_MHL_CBUS1_00[9:8], SW control cable detect
            }
            else
            {
                msWriteByteMask(REG_003101, BIT1, MASKBIT(1:0)); // REG_PM_MHL_CBUS1_00[9:8], SW control cable detect
            }
            
            break;
#endif

        case MHL_INPUT_SELECT_PORT2:
        case MHL_INPUT_SELECT_PORT3:
            if(_mhal_mhl_GetECbusMux(ucPortSelect) == GET_MHL_ECBUS_MUX_SELECT())
            {
                if(bFlag)
                {
                    msWriteByteMask(REG_003301, MASKBIT(1:0), MASKBIT(1:0)); // REG_PM_MHL_CBUS2_00[9:8], SW control cable detect
                }
                else
                {
                    msWriteByteMask(REG_003301, BIT1, MASKBIT(1:0)); // REG_PM_MHL_CBUS2_00[9:8], SW control cable detect
                }
            }
            
            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_CbusPadEnable()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_CbusPadEnable(BYTE ucPortSelect, Bool bEnableCbus)
{
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            msWrite2ByteMask(REG_000460, bEnableCbus? BIT3: 0, BIT3); // REG_PAD_TOP_30[3], Cbus port0 enable
            msWriteByteMask(REG_002F43, bEnableCbus? 0: BIT3, BIT3);//REG_PM_MHL_CBUS0_21[11], GPIO Enable port0
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            msWrite2ByteMask(REG_000460, bEnableCbus? BIT4: 0, BIT4); // REG_PAD_TOP_30[4], Cbus port1 enable
            msWriteByteMask(REG_003143, bEnableCbus? 0: BIT3, BIT3);//REG_PM_MHL_CBUS1_21[11], GPIO Enable port1
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            msWrite2ByteMask(REG_000460, bEnableCbus? BIT1: 0, BIT1); // REG_PAD_TOP_30[1], Cbus port2 enable
            msWriteByteMask(REG_003343, bEnableCbus? 0: BIT3, BIT3);//REG_PM_MHL_CBUS2_21[11], GPIO Enable port2
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            msWrite2ByteMask(REG_000460, bEnableCbus? BIT2: 0, BIT2); // REG_PAD_TOP_30[2], Cbus port3 enable
            msWriteByteMask(REG_003543, bEnableCbus? 0: BIT3, BIT3);//REG_PM_MHL_CBUS3_21[11], GPIO Enable port3
            break;
#endif

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_SetVbusCharge()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_SetVbusCharge(BYTE ucPortSelect, Bool bFlag)
{

printData("_mhal_mhl_SetVbusCharge==%d", bFlag);
printData("ucPortSelect==%d", ucPortSelect);

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            if(bFlag)
            {
                SET_MHL_VBUS_CHARGE_PORT0();
            }
            else
            {
                CLR_MHL_VBUS_CHARGE_PORT0();
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            if(bFlag)
            {
                SET_MHL_VBUS_CHARGE_PORT1();
            }
            else
            {
                CLR_MHL_VBUS_CHARGE_PORT1();
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            if(bFlag)
            {
                SET_MHL_VBUS_CHARGE_PORT2();
            }
            else
            {
                CLR_MHL_VBUS_CHARGE_PORT2();
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            if(bFlag)
            {
                SET_MHL_VBUS_CHARGE_PORT3();
            }
            else
            {
                CLR_MHL_VBUS_CHARGE_PORT3();
            }
            
            break;
#endif

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_SetHPDControl()
//  [Description]
//                  
//  [Arguments]:
//                  
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_SetHPDControl(BYTE ucPortSelect, Bool bSetFlag)
{
    switch(ucPortSelect)
    {
        case MHL_INPUT_SELECT_PORT0:
            if(bSetFlag)
            {
                SET_MHL_HDMI_HPD_PORT0();
            }
            else
            {
                CLR_MHL_HDMI_HPD_PORT0();
            }
            
            break;

        case MHL_INPUT_SELECT_PORT1:
            if(bSetFlag)
            {
                SET_MHL_HDMI_HPD_PORT1();
            }
            else
            {
                CLR_MHL_HDMI_HPD_PORT1();
            }
            
            break;

        case MHL_INPUT_SELECT_PORT2:
            if(bSetFlag)
            {
                SET_MHL_HDMI_HPD_PORT2();
            }
            else
            {
                CLR_MHL_HDMI_HPD_PORT2();
            }
            
            break;

        case MHL_INPUT_SELECT_PORT3:
            if(bSetFlag)
            {
                SET_MHL_HDMI_HPD_PORT3();
            }
            else
            {
                CLR_MHL_HDMI_HPD_PORT3();
            }
            
            break;
            
        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_OpenCommonModeResistor()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_OpenCommonModeResistor(BYTE ucPortSelect, Bool bFlag)
{
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            msWrite2ByteMask(REG_11210A, bFlag? 0: BIT3, BIT3); // REG_COMBO_PHY1_05[3]: 
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            msWrite2ByteMask(REG_11230A, bFlag? 0: BIT3, BIT3); // REG_COMBO_PHY3_05[3]: 
            break;
#endif

        case MHL_INPUT_SELECT_PORT2:
        case MHL_INPUT_SELECT_PORT3:
            msWrite2ByteMask(REG_160C4E, bFlag? 0: BIT0, BIT0); // REG_DVI_DTOP_DUAL_27[0], port 2-3 dual mhl enable ; [1]:packed-pixel mode enable 
            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_HDMIByPassSetting()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_HDMIByPassSetting(BYTE ucPortSelect)
{
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            msWrite2ByteMask(REG_112022, 0, MASKBIT(12:0)); // REG_COMBO_PHY0_11[12]:Override B channel PLL phase, [11:0]:Freeze and override DVI red channel PLL phase selection with OVPHR
            msWrite2ByteMask(REG_112110, 0, BIT7); // REG_COMBO_PHY1_08[7]: TEST register control of DVIPLL
            msWrite2ByteMask(REG_16024E, 0, BIT15| BIT6| BIT1| BIT0); //REG_DVI_DTOP0_27[1]: packed-pixel mode enable, [6]dynamic sequential check enable, [15]Enable mhl packed-pixel mode criteria
            msWriteByteMask(REG_160412, 0, BIT0); // REG_DVI_HDCP0_09[0]: port0 packed-pixel mode + HDCP enable
            //msWriteByteMask(REG_112120, BIT0, BIT0); // REG_COMBO_PHY1_10[0], port0 reg_atop_dm_eqb_ove
            msWriteByteMask(REG_112122, MHL_HDMI14_B_CHANNEL_EQ, MASKBIT(3:0)); // REG_COMBO_PHY1_11[3:0], port0 B channel equalizer strength control
            msWriteByteMask(REG_112102, 0, BIT3); // REG_COMBO_PHY1_01[3], port0 enable comparator
            
#if(MHL_RCONTROL_OVERWRITE_ENABLE0)
            msWriteByteMask(REG_112101, 0, MASKBIT(3:0)); // REG_COMBO_PHY1_00[12:8], port0 r-control overwrite value
#endif  
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            msWrite2ByteMask(REG_112222, 0, MASKBIT(12:0)); // REG_COMBO_PHY2_11[12]:Override B channel PLL phase, [11:0]:Freeze and override DVI red channel PLL phase selection with OVPHR
            msWrite2ByteMask(REG_112310, 0, BIT7); // REG_COMBO_PHY3_08[7]: TEST register control of DVIPLL
            msWrite2ByteMask(REG_16074E, 0, BIT15| BIT6| BIT1| BIT0); //REG_DVI_DTOP1_27[1]: packed-pixel mode enable , [6]dynamic sequential check enable, [15]Enable mhl packed-pixel mode criteria
            msWriteByteMask(REG_160912, 0, BIT0); // REG_DVI_HDCP1_09[0]: port1 packed-pixel mode + HDCP enable    
            //msWriteByteMask(REG_112320, BIT0, BIT0); // REG_COMBO_PHY3_10[0], port1 reg_atop_dm_eqb_ove
            msWriteByteMask(REG_112322, MHL_HDMI14_B_CHANNEL_EQ, MASKBIT(3:0)); // REG_COMBO_PHY3_11[3:0], port1 B channel equalizer strength control
            msWriteByteMask(REG_112302, 0, BIT3); // REG_COMBO_PHY3_01[3], port1 enable comparator
            
#if(MHL_RCONTROL_OVERWRITE_ENABLE1)
            msWriteByteMask(REG_112301, 0, MASKBIT(3:0)); // REG_COMBO_PHY3_00[12:8], port1 r-control overwrite value
#endif 
            break;
#endif

        case MHL_INPUT_SELECT_PORT2:
        case MHL_INPUT_SELECT_PORT3:
            msWrite2ByteMask(REG_160C4E, 0, BIT6| BIT1); // REG_DVI_DTOP_DUAL_27[1]:packed-pixel mode enable, [6]: align
            msWrite2ByteMask(REG_161012, BIT0, BIT0); // REG_DVI_HDCP_DUAL_09[0]: port2 packed-pixel mode + HDCP enable
            //msWrite2ByteMask(REG_1124E6, ((MHL_HDMI20_G_CHANNEL_EQ << 8) |MHL_HDMI20_B_CHANNEL_EQ), MASKBIT(12:0)); // REG_COMBO_PHY4_73[12:8][4:0], port2 EQ overwrite value
            //msWrite2ByteMask(REG_1124E8, MHL_HDMI20_R_CHANNEL_EQ, MASKBIT(4:0)); // REG_COMBO_PHY4_74[4:0], port2 EQ overwrite value
            msWriteByteMask(REG_1124B9, 0, BIT1); // REG_COMBO_PHY4_5C[9], port2&3 enable comparator
            msWriteByteMask(REG_1124DF, 0, BIT2); // REG_COMBO_PHY4_6F[10]: icotrol overwrite enable
            msWriteByteMask(REG_1124D9, 0, MASKBIT(5:1)); // REG_COMBO_PHY4_6C[13:9]: icotrol overwrite value
            msWriteByteMask(REG_163060, BIT2| BIT0, MASKBIT(3:2)| MASKBIT(1:0)); // REG_COMBO_GP_TOP_30[3:2][1:0]: 
            
#if(MHL_RCONTROL_OVERWRITE_ENABLE2 || MHL_RCONTROL_OVERWRITE_ENABLE3)
            msWriteByteMask(REG_1124B3, 0, BIT0); // REG_COMBO_PHY0_59[8], port2&3 r-control overwrite enable
            msWriteByteMask(REG_1124D7, 0, MASKBIT(7:4)); // REG_COMBO_PHY0_6B[15:12], port2&3 r-control overwrite value
#endif

            msWriteByteMask(REG_160C3C, BIT3| BIT1, MASKBIT(3:0));//REG_DVI_DTOP_DUAL_1E[3:0],timer v divider for div report
            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_24BitModeSetting()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_24BitModeSetting(BYTE ucPortSelect)
{
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            msWrite2ByteMask(REG_112022, BIT12| MHL_PHASE_CODE_VALUE, MASKBIT(12:0)); // REG_COMBO_PHY0_11[12]:Override B channel PLL phase, [11:0]:Freeze and override DVI red channel PLL phase selection with OVPHR
            msWrite2ByteMask(REG_112110, BIT7, BIT7); // REG_COMBO_PHY1_08[7]: TEST register control of DVIPLL
            msWrite2ByteMask(REG_16024E, BIT15| BIT6| BIT0, BIT15| BIT6| BIT1| BIT0); //REG_DVI_DTOP0_27[1]: packed-pixel mode enable, [6]dynamic sequential check enable, [15]Enable mhl packed-pixel mode criteria  
            msWriteByteMask(REG_160412, 0, BIT0); // REG_DVI_HDCP0_09[0]: port0 packed-pixel mode + HDCP enable
            //msWriteByteMask(REG_112120, BIT0, BIT0); // REG_COMBO_PHY1_10[0], port0 reg_atop_dm_eqb_ove
            msWriteByteMask(REG_112122, MHL_2_B_CHANNEL_EQ_VALUE, MASKBIT(3:0)); // REG_COMBO_PHY1_11[3:0], port0 B channel equalizer strength control
            //msWriteByteMask(REG_112102, BIT3, BIT3); // REG_COMBO_PHY1_01[3], port0 enable comparator
            msWriteByteMask(REG_11210C, BIT0, BIT0); // REG_COMBO_PHY1_06[0], TEST register control of DEMUX

#if(MHL_RCONTROL_OVERWRITE_ENABLE0)
            msWriteByteMask(REG_112101, MHL_RCONTROL_OVERWRITE_PORT0, MASKBIT(3:0)); // REG_COMBO_PHY1_00[12:8], port0 r-control overwrite value
#endif  
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            msWrite2ByteMask(REG_112222, BIT12| MHL_PHASE_CODE_VALUE, MASKBIT(12:0)); // REG_COMBO_PHY2_11[12]:Override B channel PLL phase, [11:0]:Freeze and override DVI red channel PLL phase selection with OVPHR
            msWrite2ByteMask(REG_112310, BIT7, BIT7); // REG_COMBO_PHY3_08[7]: TEST register control of DVIPLL
            msWrite2ByteMask(REG_16074E, BIT15| BIT6| BIT0, BIT15| BIT6| BIT1| BIT0); //REG_DVI_DTOP1_27[1]: packed-pixel mode enable , [6]dynamic sequential check enable, [15]Enable mhl packed-pixel mode criteria
            msWriteByteMask(REG_160912, 0, BIT0); // REG_DVI_HDCP1_09[0]: port1 packed-pixel mode + HDCP enable    
            //msWriteByteMask(REG_112320, BIT0, BIT0); // REG_COMBO_PHY3_10[0], port1 reg_atop_dm_eqb_ove
            msWriteByteMask(REG_112322, MHL_2_B_CHANNEL_EQ_VALUE, MASKBIT(3:0)); // REG_COMBO_PHY3_11[3:0], port1 B channel equalizer strength control
            //msWriteByteMask(REG_112302, BIT3, BIT3); // REG_COMBO_PHY3_01[3], port1 enable comparator
            msWriteByteMask(REG_11230C, BIT0, BIT0); // REG_COMBO_PHY3_06[0], TEST register control of DEMUX

#if(MHL_RCONTROL_OVERWRITE_ENABLE1)
            msWriteByteMask(REG_112301, MHL_RCONTROL_OVERWRITE_PORT1, MASKBIT(3:0)); // REG_COMBO_PHY3_00[12:8], port1 r-control overwrite value
#endif
            break;
#endif

        case MHL_INPUT_SELECT_PORT2:
        case MHL_INPUT_SELECT_PORT3:
            msWrite2ByteMask(REG_160C4E, BIT6, BIT6| BIT1); // REG_DVI_DTOP_DUAL_27[1]: packed-pixel mode enable, [6]: align
            msWrite2ByteMask(REG_161012, 0, BIT0); // REG_DVI_HDCP_DUAL_09[0]: port2 packed-pixel mode + HDCP enable
            msWriteByteMask(REG_1124E6, MHL_3_B_CHANNEL_EQ_VALUE, MASKBIT(4:0)); // REG_COMBO_PHY4_73[4:0], port2 EQ overwrite value            
            //msWriteByteMask(REG_1124B9, BIT1, BIT1); // REG_COMBO_PHY4_5C[9], port2&3 enable comparator
            msWriteByteMask(REG_1124B9, BIT2, BIT2); //REG_COMBO_PHY4_5C[10], ATOP - Test of atop mux
            msWriteByteMask(REG_1124DF, BIT2, BIT2); // REG_COMBO_PHY4_6F[10]: icotrol overwrite enable
            msWriteByteMask(REG_1124D9, (MHL_I_CONTROL_LF_VALUE << 1), MASKBIT(5:1)); // REG_COMBO_PHY4_6C[13:9]: icotrol overwrite value

#if(MHL_RCONTROL_OVERWRITE_ENABLE2 || MHL_RCONTROL_OVERWRITE_ENABLE3)
            msWriteByteMask(REG_1124B3, BIT0, BIT0); // REG_COMBO_PHY0_59[8], port2&3 r-control overwrite enable
            msWriteByteMask(REG_1124D7, (MHL_RCONTROL_OVERWRITE_PORT0 << 4), MASKBIT(7:4)); // REG_COMBO_PHY0_6B[15:12], port2&3 r-control overwrite value
#endif

            msWriteByteMask(REG_160C3C, BIT3|BIT2|BIT1|BIT0, MASKBIT(3:0));//REG_DVI_DTOP_DUAL_1E[3:0],timer v divider for div report
            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_PacketPixelModeSetting()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_PacketPixelModeSetting(BYTE ucPortSelect)
{
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            msWrite2ByteMask(REG_16024E, BIT1, BIT1); //REG_DVI_DTOP0_27[1]: packed-pixel mode enable 
            msWrite2ByteMask(REG_160412, BIT0, BIT0); // REG_DVI_HDCP0_09[0]: port0 packed-pixel mode + HDCP enable
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            msWrite2ByteMask(REG_16074E, BIT1, BIT1); //REG_DVI_DTOP1_27[1]: packed-pixel mode enable 
            msWrite2ByteMask(REG_160912, BIT0, BIT0); // REG_DVI_HDCP1_09[0]: port1 packed-pixel mode + HDCP enable            

            break;
#endif

        case MHL_INPUT_SELECT_PORT2:
        case MHL_INPUT_SELECT_PORT3:
            msWrite2ByteMask(REG_160C4E, BIT1, BIT1); //REG_DVI_DTOP_DUAL_27[1]:packed-pixel mode enable 
            msWrite2ByteMask(REG_161012, BIT0, BIT0); // REG_DVI_HDCP_DUAL_09[0]: port2 packed-pixel mode + HDCP enable
            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_SetCbusReceiveInterrupt()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_SetCbusReceiveInterrupt(BYTE ucPortSelect, Bool bFlag)
{
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            msWriteByteMask(REG_002E74, bFlag? 0: BIT1, BIT1); // REG_MHL_CBUS0_3A[1]: receive packet valid mask
            msWriteByteMask(REG_002E74, BIT0, BIT0); // REG_MHL_CBUS0_3A[0]
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            msWriteByteMask(REG_003074, bFlag? 0: BIT1, BIT1); // REG_MHL_CBUS1_3A[1]: receive packet valid mask
            msWriteByteMask(REG_003074, BIT0, BIT0); // REG_MHL_CBUS1_3A[0]
            
            break;
#endif

        case MHL_INPUT_SELECT_PORT2:
        case MHL_INPUT_SELECT_PORT3:
            msWriteByteMask(REG_003274, bFlag? 0: BIT1, BIT1); // REG_MHL_CBUS2_3A[1]: receive packet valid mask
            msWriteByteMask(REG_003274, BIT0, BIT0); // REG_MHL_CBUS2_3A[0]
            
            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_SetStucktoLowInterrupt()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_SetStucktoLowInterrupt(BYTE ucPortSelect, Bool bFlag)
{
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            msWriteByteMask(REG_002F30, bFlag? 0: BIT1, BIT1); // REG_PM_MHL_CBUS0_18[1]: cbus stuck to low int mask
            msWriteByteMask(REG_002F30, BIT0, BIT0); // REG_PM_MHL_CBUS0_18[0]
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            msWriteByteMask(REG_003130, bFlag? 0: BIT1, BIT1); // REG_PM_MHL_CBUS1_18[1]: cbus stuck to low int mask
            msWriteByteMask(REG_003130, BIT0, BIT0); // REG_PM_MHL_CBUS1_18[0]
            
            break;
#endif

        case MHL_INPUT_SELECT_PORT2:
        case MHL_INPUT_SELECT_PORT3:
            msWriteByteMask(REG_003330, bFlag? 0: BIT1, BIT1); // REG_PM_MHL_CBUS2_18[1]: cbus stuck to low int mask
            msWriteByteMask(REG_003330, BIT0, BIT0); // REG_PM_MHL_CBUS2_18[0]
            
            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_SetECbusStateChangeInterrupt()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_SetECbusStateChangeInterrupt(BYTE ucPortSelect, Bool bEnableFlag)
{
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            
            break;
#endif

        case MHL_INPUT_SELECT_PORT2:
        case MHL_INPUT_SELECT_PORT3:
            msWriteByteMask(REG_00226C, bEnableFlag? 0: BIT0, BIT0); // REG_MHL_ECBUS2_36[0]: Ecbus state change int mask
            msWriteByteMask(REG_002268, BIT0, BIT0); // REG_MHL_CBUS2_34[0]: Ecbus state change int clear
            
            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_SetEMSCReceiveInterrupt()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_SetEMSCReceiveInterrupt(BYTE ucPortSelect, Bool bEnableFlag)
{
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            
            break;
#endif

        case MHL_INPUT_SELECT_PORT2:
        case MHL_INPUT_SELECT_PORT3:
            msWriteByteMask(REG_00226D, bEnableFlag? 0: BIT4, BIT4); // REG_MHL_ECBUS2_36[12]: eMSC receive int mask
            msWriteByteMask(REG_002269, BIT4, BIT4); // REG_MHL_CBUS2_34[12]: eMSC receive int clear
            
            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_ClockBigChangeFlag()
//  [Description]:
//                  
//  [Arguments]:
//                  
//  [Return]:
//                 
//**************************************************************************
Bool _mhal_mhl_ClockBigChangeFlag(BYTE ucPortSelect)
{
    Bool bBigChange = FALSE;

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            //bBigChange = ((msReadByte(REG_160656) & BIT4) ? TRUE : FALSE); // REG_COMBO_ATOP0_2B[4]

            if(bBigChange)
            {
                //msWriteByteMask(REG_16065A, BIT4, BIT4); // REG_COMBO_ATOP0_2D[4]
                //msWriteByteMask(REG_16065A, 0, BIT4); // REG_COMBO_ATOP0_2D[4]
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            //bBigChange = ((msReadByte(REG_160C56) & BIT4) ? TRUE : FALSE); // REG_COMBO_ATOP1_2B[4]

            if(bBigChange)
            {
                //msWriteByteMask(REG_160C5A, BIT4, BIT4); // REG_COMBO_ATOP1_2D[4]
                //msWriteByteMask(REG_160C5A, 0, BIT4); // REG_COMBO_ATOP1_2D[4]
            }
            
            break;
#endif

        case MHL_INPUT_SELECT_PORT2:
        case MHL_INPUT_SELECT_PORT3:
            //bBigChange = ((msReadByte(REG_161456) & BIT4) ? TRUE : FALSE); // REG_COMBO_ATOP2_2B[4]

            if(bBigChange)
            {
                //msWriteByteMask(REG_16145A, BIT4, BIT4); // REG_COMBO_ATOP2_2D[4]
                //msWriteByteMask(REG_16145A, 0, BIT4); // REG_COMBO_ATOP2_2D[4]
            }
            
            break;

        default:

            break;
    };

    return bBigChange;
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_CheckClockStatus()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool _mhal_mhl_CheckClockStatus(BYTE ucPortSelect)
{
    Bool bStable = FALSE;
    Bool bClockChange = _mhal_mhl_ClockBigChangeFlag(ucPortSelect);
    WORD usClkCount = 0;

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            if(msReadByte(REG_112083) & BIT1) // REG_COMBO_PHY0_41[9], Clock stable flag.
            {
                usClkCount = msRead2Byte(REG_112078) &MASKBIT(11:0); // REG_COMBO_PHY0_3C[11:0], Clock counter.

                bStable = TRUE;
                
                if(bClockChange && (usClkCount < CBUS_CLOCK_DETECT_LEVEL))
                {
                    bStable = FALSE;
                }
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            if(msReadByte(REG_112283) & BIT1) // REG_COMBO_PHY2_41[9], Clock stable flag.
            {
                usClkCount = msRead2Byte(REG_112278) &MASKBIT(11:0); // REG_COMBO_PHY2_3C[11:0], Clock counter.

                bStable = TRUE;
                
                if(bClockChange && (usClkCount < CBUS_CLOCK_DETECT_LEVEL))
                {
                    bStable = FALSE;
                }
            }
            
            break;
#endif

        case MHL_INPUT_SELECT_PORT2:
        case MHL_INPUT_SELECT_PORT3:
            if(msReadByte(REG_112483) & BIT1) // REG_COMBO_PHY4_41[9], Clock stable flag.
            {
                usClkCount = msRead2Byte(REG_112478) &MASKBIT(11:0); // REG_COMBO_PHY4_3C[11:0], Clock counter.

                bStable = TRUE;
                
                if(bClockChange && (usClkCount < CBUS_CLOCK_DETECT_LEVEL))
                {
                    bStable = FALSE;
                }
            }
            
            break;

        default:

            break;
    };

    return bStable;
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_CheckDataEnableStatus()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool _mhal_mhl_CheckDataEnableStatus(BYTE ucPortSelect)
{
    Bool bStable = FALSE;

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            if(msReadByte(REG_160262) & BIT6) // REG_DVI_DTOP0_31[6]: DE stable
            {
                bStable = TRUE;
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            if(msReadByte(REG_160762) & BIT6) // REG_DVI_DTOP1_31[6]: DE stable
            {
                bStable = TRUE;
            }
            
            break;
#endif

        case MHL_INPUT_SELECT_PORT2:
        case MHL_INPUT_SELECT_PORT3:
            if(msReadByte(REG_160C62) & BIT6) // REG_DVI_DTOP_DUAL_31[6]: DE stable
            {
                bStable = TRUE;
            }
            
            break;

        default:

            break;
    };

    return bStable;
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_CDRPhySetting()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_mhl_CDRPhySetting(BYTE ucPortSelect, Bool bSetting)
{
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            
            break;
#endif

        case MHL_INPUT_SELECT_PORT2:
        case MHL_INPUT_SELECT_PORT3:
            if(bSetting)
            {
                
            }
            else
            {
                
            }
            
            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_TrimPullDown1K()
//  [Description]
//                  
//  [Arguments]:
//                  
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_TrimPullDown1K(BYTE ucPortSelect, BYTE ucPD1KIndex)
{
    switch(ucPortSelect)
    {
        case MHL_INPUT_SELECT_PORT0:
            msWriteByteMask(REG_002F43, ucPD1KIndex, MASKBIT(5:4)); // REG_PM_MHL_CBUS0_21[13:12], Cbus pull down 1K port0
            break;

        case MHL_INPUT_SELECT_PORT1:
            msWriteByteMask(REG_003143, ucPD1KIndex, MASKBIT(5:4)); // REG_PM_MHL_CBUS1_21[13:12], Cbus pull down 1K port1
            break;

        case MHL_INPUT_SELECT_PORT2:
            msWriteByteMask(REG_003343, ucPD1KIndex, MASKBIT(5:4)); // REG_PM_MHL_CBUS2_21[13:12], Cbus pull down 1K port2
            break;

        case MHL_INPUT_SELECT_PORT3:
            msWriteByteMask(REG_003543, ucPD1KIndex, MASKBIT(5:4)); // REG_PM_MHL_CBUS3_21[13:12], Cbus pull down 1K port3
            break;
            
        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_TrimPullDown100K()
//  [Description]
//                  
//  [Arguments]:
//                  
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_TrimPullDown100K(BYTE ucPortSelect, BYTE ucPD100KIndex)
{
    switch(ucPortSelect)
    {
        case MHL_INPUT_SELECT_PORT0:
            msWriteByteMask(REG_002F43, ucPD100KIndex, MASKBIT(7:6)); // REG_PM_MHL_CBUS0_21[15:14], Cbus pull down 100K port0
            break;

        case MHL_INPUT_SELECT_PORT1:
            msWriteByteMask(REG_003143, ucPD100KIndex, MASKBIT(7:6)); // REG_PM_MHL_CBUS1_21[15:14], Cbus pull down 100K port1
            break;

        case MHL_INPUT_SELECT_PORT2:
            msWriteByteMask(REG_003343, ucPD100KIndex, MASKBIT(7:6)); // REG_PM_MHL_CBUS2_21[15:14], Cbus pull down 100K port2
            break;

        case MHL_INPUT_SELECT_PORT3:
            msWriteByteMask(REG_003543, ucPD100KIndex, MASKBIT(7:6)); // REG_PM_MHL_CBUS3_21[15:14], Cbus pull down 100K port3
            break;
            
        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_SetCbusPullUp1K()
//  [Description]
//                  
//  [Arguments]:
//                  
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_SetCbusPullUp1K(BYTE ucPortSelect, Bool bSetFlag)
{
    switch(ucPortSelect)
    {
        case MHL_INPUT_SELECT_PORT0:
            
            break;

        case MHL_INPUT_SELECT_PORT1:
            
            break;

        case MHL_INPUT_SELECT_PORT2:
            msWriteByteMask(REG_003340, bSetFlag? BIT5: 0, BIT5); // REG_PM_MHL_CBUS2_20[5], Cbus pull up 1K port2
            break;

        case MHL_INPUT_SELECT_PORT3:
            msWriteByteMask(REG_003540, bSetFlag? BIT5: 0, BIT5); // REG_PM_MHL_CBUS3_20[5], Cbus pull up 1K port3
            break;
            
        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_SetPadPullUp1K()
//  [Description]
//                  
//  [Arguments]:
//                  
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_SetPadPullUp1K(BYTE ucPortSelect, Bool bFlag)
{
    switch(ucPortSelect)
    {
        case MHL_INPUT_SELECT_PORT0:
            msWriteByteMask(REG_002FF2, bFlag? BIT1: 0, BIT1); // REG_PM_MHL_CBUS0_79[1],  pull-up 1k
            break;

        case MHL_INPUT_SELECT_PORT1:
            msWriteByteMask(REG_0031F2, bFlag? BIT1: 0, BIT1); // REG_PM_MHL_CBUS1_79[1],  pull-up 1k
            break;

        case MHL_INPUT_SELECT_PORT2:
            msWriteByteMask(REG_0033F2, bFlag? BIT1: 0, BIT1); // REG_PM_MHL_CBUS2_79[1],  pull-up 1k
            break;

        case MHL_INPUT_SELECT_PORT3:
            msWriteByteMask(REG_0035F2, bFlag? BIT1: 0, BIT1); // REG_PM_MHL_CBUS3_79[1],  pull-up 1k
            break;
            
        default:
            bFlag = 0;
            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_SetCbusPullDown100K()
//  [Description]
//                  
//  [Arguments]:
//                  
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_SetCbusPullDown100K(BYTE ucPortSelect, Bool bSetFlag)
{
    switch(ucPortSelect)
    {
        case MHL_INPUT_SELECT_PORT0:
            msWriteByteMask(REG_002F40, bSetFlag? BIT7: 0, BIT7); // REG_PM_MHL_CBUS0_20[7], Cbus pull down 100K port0
            break;

        case MHL_INPUT_SELECT_PORT1:
            msWriteByteMask(REG_003140, bSetFlag? BIT7: 0, BIT7); // REG_PM_MHL_CBUS1_20[7], Cbus pull down 100K port1
            break;

        case MHL_INPUT_SELECT_PORT2:
            msWriteByteMask(REG_003340, bSetFlag? BIT7: 0, BIT7); // REG_PM_MHL_CBUS2_20[7], Cbus pull down 100K port2
            break;

        case MHL_INPUT_SELECT_PORT3:
            msWriteByteMask(REG_003540, bSetFlag? BIT7: 0, BIT7); // REG_PM_MHL_CBUS3_20[7], Cbus pull down 100K port3
            break;
            
        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_ECbusMuxSwitch()
//  [Description]
//                  
//  [Arguments]:
//                  
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_ECbusMuxSwitch(BYTE ucECbusMux)
{
    BYTE ucECbusMuxValue = 0;
    
    SET_MHL_ECBUS_MUX_SELECT(ucECbusMux);

    switch(ucECbusMux)
    {
        case MHL_ECBUS_MUX_SELECT_PORT2:
            msWriteByteMask(REG_000391, 0x10, MASKBIT(5:4)| MASKBIT(1:0)); // REG_PM_BANK_48[13:12][9:8]: ECbus clock select
            
            ucECbusMuxValue = 2;
            
            break;

        case MHL_ECBUS_MUX_SELECT_PORT3:
            msWriteByteMask(REG_000391, 0x01, MASKBIT(5:4)| MASKBIT(1:0)); // REG_PM_BANK_48[13:12][9:8]: ECbus clock select
            
            ucECbusMuxValue = 3;
            
            break;

        default:
            // ECbus mux none is 0
            break;
    };

    // Switch ECbus mux
    msWriteByteMask(REG_00170A, ucECbusMuxValue, MASKBIT(2:0)); // REG_PM_BANK_05[2:0], eCbus mux
    // Cbus reset
    msWriteByteMask(REG_003300, BIT0, BIT0); // REG_PM_MHL_CBUS2_00[0]: Cbus soft reset
    msWriteByteMask(REG_003300, 0, BIT0); // REG_PM_MHL_CBUS2_00[0]: Cbus soft reset
    
    if(ucECbusMuxValue > 0)
    {
        // Set cable detect
        msWriteByteMask(REG_003301, MASKBIT(1:0), MASKBIT(1:0)); // REG_PM_MHL_CBUS2_00[9:8], SW control cable detect
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_CbusConnectCheckEnable()
//  [Description]
//                  
//  [Arguments]:
//                  
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_CbusConnectCheckEnable(BYTE ucPortSelect, Bool bEnable)
{
    switch(ucPortSelect)
    {
        case MHL_INPUT_SELECT_PORT0:
            
            break;

        case MHL_INPUT_SELECT_PORT1:
            
            break;

        case MHL_INPUT_SELECT_PORT2:
        case MHL_INPUT_SELECT_PORT3:
            msWrite2ByteMask(REG_003316, bEnable? MHL_CBUS_CONNECT_CHECK_VALUE: 0, MASKBIT(15:0)); // REG_PM_MHL_CBUS2_0B[15:0]: Disable connection check
            break;
            
        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_ECbusDmuxEnable()
//  [Description]
//                  
//  [Arguments]:
//                  
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_ECbusDmuxEnable(BYTE ucPortSelect, Bool bEnable)
{
    switch(ucPortSelect)
    {
        case MHL_INPUT_SELECT_PORT0:
            
            break;

        case MHL_INPUT_SELECT_PORT1:
            
            break;

        case MHL_INPUT_SELECT_PORT2:
        case MHL_INPUT_SELECT_PORT3:
            msWriteByteMask(REG_00339E, bEnable? BIT3: 0, BIT3); // REG_PM_MHL_CBUS2_4F[3]: Enable ECbus dmux
            break;
            
        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_ECbusAutoTrainProc()
//  [Description]
//                  
//  [Arguments]:
//                  
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_ECbusAutoTrainProc(BYTE ucPortSelect)
{
    Bool bFinish = FALSE;
    Bool bClockLock = FALSE;
    BYTE ucDelayValue = 0;
    BYTE ucLastValue = 0;
    BYTE ucCounter = 0;
    WORD usValue = 0;
    WORD usWaitCounter = 0;
    
    switch(ucPortSelect)
    {
        case MHL_INPUT_SELECT_PORT0:
            
            break;

        case MHL_INPUT_SELECT_PORT1:
            
            break;

        case MHL_INPUT_SELECT_PORT2:
        case MHL_INPUT_SELECT_PORT3:
            while(!bFinish)
            {
                while(!((msReadByte(REG_0033DF) &BIT5)));
                
                msWrite2ByteMask(REG_003394, ((ucDelayValue +4) << 4), MASKBIT(7:4)); // REG_PM_MHL_CBUS2_4A, width
                msWrite2ByteMask(REG_003396, (ucDelayValue << 4), BIT13| MASKBIT(7:4)); // REG_PM_MHL_CBUS2_4B, delay

                usWaitCounter = 1000;

                while(usWaitCounter--);
                
                bClockLock = ((msReadByte(REG_0033DF) &BIT5)? TRUE : FALSE); // REG_PM_MHL_CBUS2_6F[13]: ECbus clock lock flag

                ucCounter = 0;

                usValue = msRead2Byte(REG_0033A4) ^ 0xFFFF;

                while(usValue &BIT0)
                {
                    usValue = usValue >> 1;
                    ucCounter++;
                }

                usValue = msRead2Byte(REG_0033A6) ^ 0xFFFF;

                while(usValue &BIT0)
                {
                    usValue = usValue >> 1;
                    ucCounter++;
                }

                if(ucCounter < 0x17)
                {
                    usValue = msRead2Byte(REG_0033A8) ^ 0xFFFF;

                    while(usValue &BIT0)
                    {
                        usValue = usValue >> 1;
                        ucCounter++;
                    }
                }
                
                if(!bClockLock)
                {
                    msWrite2ByteMask(REG_003394, 4, MASKBIT(7:4)); // REG_PM_MHL_CBUS2_4A, width
                    msWrite2ByteMask(REG_003396, BIT13| 0, BIT13| MASKBIT(7:4)); // REG_PM_MHL_CBUS2_4B, delay

                    if(ucLastValue < 12)
                    {
                        ucLastValue++;
                        ucDelayValue = ucLastValue;
                        
                    }
                    else
                    {
                        bFinish = TRUE;
                        MHL_HAL_DPRINTF("** MHL auto training not find case 0 %d", ucLastValue);
                    }
                }
                else if(ucCounter > 0x17)
                {
                    if(ucDelayValue < 12)
                    {
                        ucDelayValue++;
                    }
                    else
                    {
                        bFinish = TRUE;
                        MHL_HAL_DPRINTF("** MHL auto training not find case 1 %d", ucLastValue);
                    }
                }
                else
                {
                    bFinish = TRUE;
                    MHL_HAL_DPRINTF("** MHL auto training finish x-value = %d", ucLastValue);
                    MHL_HAL_DPRINTF("** MHL auto training finish y-value = %d", ucDelayValue);
                }
            }
            
            break;
            
        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_Version3PhyEnable()
//  [Description]
//                  
//  [Arguments]:
//                  
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_Version3PhyEnable(BYTE ucPortSelect, Bool bEnable)
{
    switch(ucPortSelect)
    {
        case MHL_INPUT_SELECT_PORT0:
            
            break;

        case MHL_INPUT_SELECT_PORT1:
            
            break;

        case MHL_INPUT_SELECT_PORT2:
        case MHL_INPUT_SELECT_PORT3:
            if(_mhal_mhl_GetECbusMux(ucPortSelect) == GET_MHL_ECBUS_MUX_SELECT())
            {
                msWriteByteMask(REG_160CC1, bEnable? BIT0: 0, BIT0); // REG_DVI_DTOP_DUAL_60[8]: MHL3 engine enable
            }
            
            break;
            
        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_SetShortReadAddress()
//  [Description]
//                  
//  [Arguments]:
//                  
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_SetShortReadAddress(BYTE ucPortSelect, Bool bECbusEnable)
{
    switch(ucPortSelect)
    {
        case MHL_INPUT_SELECT_PORT0:
            
            break;

        case MHL_INPUT_SELECT_PORT1:
            
            break;

        case MHL_INPUT_SELECT_PORT2:
            msWriteByte(REG_00322F, bECbusEnable? 0x70: 0x08); //REG_MHL_CBUS2_17[15:8]: reg_ddc_hdcp_short_read_adr
            
            if(bECbusEnable)
            {
                SET_MHL_ECBUS_ENABLE_PORT2();
            }
            else
            {
                CLR_MHL_ECBUS_ENABLE_PORT2();
            }
            
            break;
            
        case MHL_INPUT_SELECT_PORT3:
            msWriteByte(REG_00322F, bECbusEnable? 0x70: 0x08); //REG_MHL_CBUS2_17[15:8]: reg_ddc_hdcp_short_read_adr

            if(bECbusEnable)
            {
                SET_MHL_ECBUS_ENABLE_PORT3();
            }
            else
            {
                CLR_MHL_ECBUS_ENABLE_PORT3();
            }
            
            break;
            
        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_ECbusStateReset()
//  [Description]
//                  
//  [Arguments]:
//                  
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_ECbusStateReset(BYTE ucPortSelect)
{
    switch(ucPortSelect)
    {
        case MHL_INPUT_SELECT_PORT0:
            
            break;

        case MHL_INPUT_SELECT_PORT1:
            
            break;

        case MHL_INPUT_SELECT_PORT2:
        case MHL_INPUT_SELECT_PORT3:
            msWriteByteMask(REG_002200, BIT1, BIT1); // REG_MHL_ECBUS2_00[1]: ECbus state reset
            msWriteByteMask(REG_002200, 0, BIT1); // REG_MHL_ECBUS2_00[1]: ECbus state reset
            break;
            
        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_ECbusInitialSetting()
//  [Description]
//                  
//  [Arguments]:
//                  
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_ECbusInitialSetting(BYTE ucPortSelect)
{
    switch(ucPortSelect)
    {
        case MHL_INPUT_SELECT_PORT0:
            
            break;

        case MHL_INPUT_SELECT_PORT1:
            
            break;

        case MHL_INPUT_SELECT_PORT2:
            msWriteByteMask(REG_0033D2, 0x35, MASKBIT(6:4)| MASKBIT(2:0)); // REG_PM_MHL_CBUS2_69[6:4]: rasing pattern type, [2:0]: deglitch after reorder

            msWrite2ByteMask(REG_003398, 0xFF80, MASKBIT(15:4)); // REG_PM_MHL_CBUS2_4C
            msWrite2ByteMask(REG_00339A, 0x8B18, MASKBIT(15:1)); // REG_PM_MHL_CBUS2_4D
            msWrite2ByteMask(REG_00339C, 0x00C3, MASKBIT(15:0)); // REG_PM_MHL_CBUS2_4E
            msWrite2ByteMask(REG_0033A2, 0x0000, MASKBIT(15:0)); // REG_PM_MHL_CBUS2_51
            msWrite2ByteMask(REG_0033AC, 0x0207, MASKBIT(15:0)); // REG_PM_MHL_CBUS2_56 
            msWrite2ByteMask(REG_003394, BIT14|BIT13, MASKBIT(14:13));// REG_PM_MHL_CBUS2_4A[14:13]
            msWrite2ByteMask(REG_0033AA, 0xF0, MASKBIT(7:0));// REG_PM_MHL_CBUS2_55[7:0]
            msWrite2ByteMask(REG_00339E, 0xF0, MASKBIT(7:4)); // REG_PM_MHL_CBUS2_4F[7:4]:reg_ecbss_afe_tx_dri_step
            
            //auto reset turn on
            msWrite2ByteMask(REG_0033FE, BIT2, BIT2);// REG_PM_MHL_CBUS2_7F, [2]:reg_rst_aft_clklck_en
            msWrite2ByteMask(REG_002200, BIT4|BIT3, MASKBIT(4:3));// REG_MHL_ECBUS2_00,[4]:reg_rst_aft_fail_en, [3]:reg_rst_aft_clklck_en

            // ECbus initial
            msWrite2ByteMask(REG_002204, BIT7, BIT7); // REG_MHL_ECBUS2_02[7]
            msWrite2ByteMask(REG_0033DA, BIT5, MASKBIT(6:5)); // REG_PM_MHL_CBUS2_6D
            msWriteByteMask(REG_0033AF, 0x06, MASKBIT(3:0)); // REG_PM_MHL_CBUS2_57[11:8]: 
            msWriteByteMask(REG_0033D0, 0x30, MASKBIT(6:4)); // REG_PM_MHL_CBUS2_68[6:4]: pre-deglitch 
            msWriteByteMask(REG_0033D2, 0x50, MASKBIT(6:4)| MASKBIT(2:0)); // REG_PM_MHL_CBUS2_69[6:4]: rasing pattern type, [2:0]: deglitch after reorder
            msWriteByteMask(REG_0033CA, 0x08, MASKBIT(5:0)); // REG_PM_MHL_CBUS2_65[5:0]: reg_dat_lsb_mask; after analog rwa data deglicth,re-order,the lsb data 1 will be mask
            
            //eCBUS fsm timing 
            msWrite2ByteMask(REG_002208, 0x000E, MASKBIT(6:0));//REG_MHL_ECBUS2_04[6:0]:reg_t_snk_cal 1; sink sends eCBUS clock after detecting eCBUS clock form source.
            msWrite2ByteMask(REG_002208, 0x0E00, MASKBIT(14:8));//REG_MHL_ECBUS2_04[14:8]:REG_t_sink_cal
            msWrite2ByteMask(REG_00220C, 0x1C00, MASKBIT(14:8));//REG_MHL_ECBUS2_06[14:8]: reg_t_snk_tdm; 
            msWrite2ByteMask(REG_002206, BIT15|BIT14, MASKBIT(15:14));//REG_MHL_ECBUS2_06[15]:reg_wait_tdm_timer_dis [14]:reg_wait_comma2_timer_dis

            //increase bandwidth 
            //msWrite2ByteMask(REG_0033AC, BIT5, BIT5);//REG_PM_MHL_CBUS2_56[5]:AFE test

            //clock detect setting 
            msWrite2ByteMask(REG_0033E0, 0x032B, MASKBIT(15:0));//REG_PM_MHL_CBUS2_70[15:0]:reg_crlock_mid; the CR lock reference value
            msWrite2ByteMask(REG_0033E2, 0x0C04, MASKBIT(15:0));//REG_PM_MHL_CBUS2_71[15:8]:reg_crlock_thr, the CR lock reference failure threshold range; [7:0]:reg_crlock_thr, the CR lock reference correct threshold range;
            msWrite2ByteMask(REG_0033E4, 0x0007, MASKBIT(15:0));//REG_PM_MHL_CBUS2_72[12:8]:reg_cr_unlock_num, clock unlock threshold; [4:0]:reg_cr_lock_num; CR lock success threshold time.
            
#if 0
            // Skip atop training
            // REG_PM_MHL_CBUS2_4A  [14]tx_rep_pd_ove  = 1 [13]tx_rep_pd_ov  [8]:tx_width_ove = 1 [7:4]:tx_width_ov
            msWrite2ByteMask(REG_003394, BIT14| BIT8| 0x30, MASKBIT(14:13)| BIT8| MASKBIT(7:4));
            
            // REG_PM_MHL_CBUS2_4B ,[14]tx_pd_ove  = 1        [13]tx_pd_ov         [8]:tx_delay_ove = 1  [7:4]:tx_delay_ov
            msWrite2ByteMask(REG_003396, BIT14| BIT8| 0x00, MASKBIT(14:13)| BIT8| MASKBIT(7:4)); 
            
            
            msWriteByteMask(REG_002209, 30, MASKBIT(6:0)); // REG_PM_MHL_CBUS2_04[14:8]
            
#else
            //auto training testing
            msWrite2ByteMask(REG_0033B8, 0x010A, MASKBIT(15:0));// REG_PM_MHL_CBUS2_5C, [10:8]:reg_txloc_golden_lowr_tol,    [5:0]:reg_txloc_golden_lower, 
            msWrite2ByteMask(REG_0033CC, 0x2117, MASKBIT(15:0));// REG_PM_MHL_CBUS2_66, [14:12]:reg_txloc_golden2_tol,  [10:8]:reg_txloc_rev_golden_tol,    [5:0]:reg_txloc_rev_golden
            msWrite2ByteMask(REG_0033CE, 0x0403, MASKBIT(15:0));// REG_PM_MHL_CBUS2_67, [14:12]:reg_val_aft_adj_ok, [10:8]:reg_txloc_txdelay_add,   [6:4]:reg_flow_retry_r, [2:0]:reg_flow_retry
            msWrite2ByteMask(REG_0033D2, BIT10, MASKBIT(11:10));// REG_PM_MHL_CBUS2_69, [11:10]:reg_txloc_set_time
            msWrite2ByteMask(REG_0033D4, 0xf117, MASKBIT(15:0));// REG_PM_MHL_CBUS2_6A, [15:12]:reg_txloc_lck_thr, [10:8]:reg_txloc_golden_tol, [5:0]:ret_txloc_golden

#endif

            break;

        case MHL_INPUT_SELECT_PORT3:
            msWriteByteMask(REG_0035D2, 0x35, MASKBIT(6:4)| MASKBIT(2:0)); // REG_PM_MHL_CBUS3_69[6:4]: rasing pattern type, [2:0]: deglitch after reorder

            msWrite2ByteMask(REG_003598, 0xFF80, MASKBIT(15:4)); // REG_PM_MHL_CBUS3_4C
            msWrite2ByteMask(REG_00359A, 0x8B18, MASKBIT(15:1)); // REG_PM_MHL_CBUS3_4D
            msWrite2ByteMask(REG_00359C, 0x00C3, MASKBIT(15:0)); // REG_PM_MHL_CBUS3_4E
            msWrite2ByteMask(REG_0035A2, 0x0000, MASKBIT(15:0)); // REG_PM_MHL_CBUS3_51
            msWrite2ByteMask(REG_0035AC, 0x0207, MASKBIT(15:0)); // REG_PM_MHL_CBUS3_56 
            msWrite2ByteMask(REG_003594, BIT14|BIT13, MASKBIT(14:13));// REG_PM_MHL_CBUS3_4A[14:13]
            msWrite2ByteMask(REG_0035AA, 0xF0, MASKBIT(7:0));// REG_PM_MHL_CBUS3_55[7:0]
            msWrite2ByteMask(REG_00359E, 0xF0, MASKBIT(7:4)); // REG_PM_MHL_CBUS3_4F[7:4]:reg_ecbss_afe_tx_dri_step
            
            //auto reset turn on
            msWrite2ByteMask(REG_0035FE, BIT2, BIT2);// REG_PM_MHL_CBUS3_7F, [2]:reg_rst_aft_clklck_en
            msWrite2ByteMask(REG_002200, BIT4|BIT3, MASKBIT(4:3));// REG_MHL_ECBUS2_00,[4]:reg_rst_aft_fail_en, [3]:reg_rst_aft_clklck_en

            // ECbus initial
            msWrite2ByteMask(REG_002204, BIT7, BIT7); // REG_MHL_ECBUS2_02[7]
            msWrite2ByteMask(REG_0035DA, BIT5, MASKBIT(6:5)); // REG_PM_MHL_CBUS2_6D
            msWriteByteMask(REG_0035AF, 0x06, MASKBIT(3:0)); // REG_PM_MHL_CBUS3_57[11:8]: 
            msWriteByteMask(REG_0035D0, 0x30, MASKBIT(6:4)); // REG_PM_MHL_CBUS3_68[6:4]: pre-deglitch 
            msWriteByteMask(REG_0035D2, 0x50, MASKBIT(6:4)| MASKBIT(2:0)); // REG_PM_MHL_CBUS3_69[6:4]: rasing pattern type, [2:0]: deglitch after reorder
            msWriteByteMask(REG_0035CA, 0x08, MASKBIT(5:0)); // REG_PM_MHL_CBUS3_65[5:0]: reg_dat_lsb_mask; after analog rwa data deglicth,re-order,the lsb data 1 will be mask
            
            //eCBUS fsm timing 
            msWrite2ByteMask(REG_002208, 0x000E, MASKBIT(6:0));//REG_MHL_ECBUS2_04[6:0]:reg_t_snk_cal 1; sink sends eCBUS clock after detecting eCBUS clock form source.
            msWrite2ByteMask(REG_002208, 0x0E00, MASKBIT(14:8));//REG_MHL_ECBUS2_04[14:8]:REG_t_sink_cal
            msWrite2ByteMask(REG_00220C, 0x1C00, MASKBIT(14:8));//REG_MHL_ECBUS2_06[14:8]: reg_t_snk_tdm; 
            msWrite2ByteMask(REG_002206, BIT15|BIT14, MASKBIT(15:14));//REG_MHL_ECBUS2_06[15]:reg_wait_tdm_timer_dis [14]:reg_wait_comma2_timer_dis

            //increase bandwidth 
            //msWrite2ByteMask(REG_0033AC, BIT5, BIT5);//REG_PM_MHL_CBUS2_56[5]:AFE test

            //clock detect setting 
            msWrite2ByteMask(REG_0035E0, 0x032B, MASKBIT(15:0));//REG_PM_MHL_CBUS3_70[15:0]:reg_crlock_mid; the CR lock reference value
            msWrite2ByteMask(REG_0035E2, 0x0C04, MASKBIT(15:0));//REG_PM_MHL_CBUS3_71[15:8]:reg_crlock_thr, the CR lock reference failure threshold range; [7:0]:reg_crlock_thr, the CR lock reference correct threshold range;
            msWrite2ByteMask(REG_0035E4, 0x0007, MASKBIT(15:0));//REG_PM_MHL_CBUS3_72[12:8]:reg_cr_unlock_num, clock unlock threshold; [4:0]:reg_cr_lock_num; CR lock success threshold time.
            
#if 0
            // Skip atop training
            // REG_PM_MHL_CBUS2_4A  [14]tx_rep_pd_ove  = 1 [13]tx_rep_pd_ov  [8]:tx_width_ove = 1 [7:4]:tx_width_ov
            msWrite2ByteMask(REG_003394, BIT14| BIT8| 0x30, MASKBIT(14:13)| BIT8| MASKBIT(7:4));
            
            // REG_PM_MHL_CBUS2_4B ,[14]tx_pd_ove  = 1        [13]tx_pd_ov         [8]:tx_delay_ove = 1  [7:4]:tx_delay_ov
            msWrite2ByteMask(REG_003396, BIT14| BIT8| 0x00, MASKBIT(14:13)| BIT8| MASKBIT(7:4)); 
            
            
            msWriteByteMask(REG_002209, 30, MASKBIT(6:0)); // REG_PM_MHL_CBUS2_04[14:8]
            
#else
            //auto training testing
            msWrite2ByteMask(REG_0035B8, 0x010A, MASKBIT(15:0));// REG_PM_MHL_CBUS3_5C, [10:8]:reg_txloc_golden_lowr_tol,    [5:0]:reg_txloc_golden_lower, 
            msWrite2ByteMask(REG_0035CC, 0x2117, MASKBIT(15:0));// REG_PM_MHL_CBUS3_66, [14:12]:reg_txloc_golden2_tol,  [10:8]:reg_txloc_rev_golden_tol,    [5:0]:reg_txloc_rev_golden
            msWrite2ByteMask(REG_0035CE, 0x0403, MASKBIT(15:0));// REG_PM_MHL_CBUS3_67, [14:12]:reg_val_aft_adj_ok, [10:8]:reg_txloc_txdelay_add,   [6:4]:reg_flow_retry_r, [2:0]:reg_flow_retry
            msWrite2ByteMask(REG_0035D2, BIT10, MASKBIT(11:10));// REG_PM_MHL_CBUS3_69, [11:10]:reg_txloc_set_time
            msWrite2ByteMask(REG_0035D4, 0xf117, MASKBIT(15:0));// REG_PM_MHL_CBUS3_6A, [15:12]:reg_txloc_lck_thr, [10:8]:reg_txloc_golden_tol, [5:0]:ret_txloc_golden

#endif

            break;
            
        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_ECbusEnableSetting()
//  [Description]
//                  
//  [Arguments]:
//                  
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_ECbusEnableSetting(BYTE ucPortSelect, Bool bEnable)
{
    switch(ucPortSelect)
    {
        case MHL_INPUT_SELECT_PORT0:
            
            break;

        case MHL_INPUT_SELECT_PORT1:
            
            break;

        case MHL_INPUT_SELECT_PORT2:
            msWriteByteMask(REG_00339A, bEnable? 0: BIT0, BIT0); // REG_PM_MHL_CBUS2_4D[0]: ECbus PLL pd
            msWriteByteMask(REG_0033AB, bEnable? 0: BIT6, BIT6); // REG_PM_MHL_CBUS2_55[14]: Rx power down
            msWriteByteMask(REG_0033AE, bEnable? BIT3: 0, MASKBIT(3:2)); // REG_PM_MHL_CBUS2_57[3][2]: 
            msWriteByteMask(REG_0033DF, bEnable? BIT0: 0, BIT0); // REG_PM_MHL_CBUS2_6F[8]: ECbus clock detect

            if(bEnable)
            {
                msWriteByteMask(REG_003301, BIT5, BIT5); // REG_PM_MHL_CBUS2_00[13]: ECbus on
            }
            else
            {
                msWriteByteMask(REG_003301, BIT6, BIT6); // REG_PM_MHL_CBUS2_00[14]: ECbus off
            }
            
            break;
            
        case MHL_INPUT_SELECT_PORT3:
            msWriteByteMask(REG_00359A, bEnable? 0: BIT0, BIT0); // REG_PM_MHL_CBUS3_4D[0]: ECbus PLL pd
            msWriteByteMask(REG_0035AB, bEnable? 0: BIT6, BIT6); // REG_PM_MHL_CBUS3_55[14]: Rx power down
            msWriteByteMask(REG_0035AE, bEnable? BIT3: 0, MASKBIT(3:2)); // REG_PM_MHL_CBUS3_57[3][2]: 
            msWriteByteMask(REG_0035DF, bEnable? BIT0: 0, BIT0); // REG_PM_MHL_CBUS3_6F[8]: ECbus clock detect

            if(bEnable)
            {
                msWriteByteMask(REG_003301, BIT5, BIT5); // REG_PM_MHL_CBUS2_00[13]: ECbus on
            }
            else
            {
                msWriteByteMask(REG_003301, BIT6, BIT6); // REG_PM_MHL_CBUS2_00[14]: ECbus off
            }
            
            break;
            
        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_ECbusModeUpProc()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_mhl_ECbusModeUpProc(BYTE ucPortSelect)
{
    switch(ucPortSelect)
    {
        case MHL_INPUT_SELECT_PORT0:

            break;

        case MHL_INPUT_SELECT_PORT1:

            break;

        case MHL_INPUT_SELECT_PORT2:
            msWriteByteMask(REG_002268, BIT4, BIT4); // REG_MHL_ECBUS2_34[4]: clear PLL lock status
            msWriteByteMask(REG_002268, 0, BIT4); // REG_MHL_ECBUS2_34[4]: clear PLL lock status 
 
            msWrite2ByteMask(REG_0033AC, 0x0207, MASKBIT(15:0)); // REG_PM_MHL_CBUS2_56 
            msWrite2ByteMask(REG_003394, BIT13, BIT13);// REG_PM_MHL_CBUS2_4A

            msWrite2ByteMask(REG_003396, MASKBIT(14:12), MASKBIT(14:12)); // REG_PM_MHL_CBUS2_4B, delay

            break;

        case MHL_INPUT_SELECT_PORT3:
            msWriteByteMask(REG_002268, BIT4, BIT4); // REG_MHL_ECBUS2_34[4]: clear PLL lock status
            msWriteByteMask(REG_002268, 0, BIT4); // REG_MHL_ECBUS2_34[4]: clear PLL lock status 
 
            msWrite2ByteMask(REG_0035AC, 0x0207, MASKBIT(15:0)); // REG_PM_MHL_CBUS3_56 
            msWrite2ByteMask(REG_003594, BIT13, BIT13);// REG_PM_MHL_CBUS3_4A

            msWrite2ByteMask(REG_003596, MASKBIT(14:12), MASKBIT(14:12)); // REG_PM_MHL_CBUS3_4B, delay
            
            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_ECbusModeDownProc()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_mhl_ECbusModeDownProc(BYTE ucPortSelect)
{
    switch(ucPortSelect)
    {
        case MHL_INPUT_SELECT_PORT0:
            
            break;

        case MHL_INPUT_SELECT_PORT1:
            
            break;

        case MHL_INPUT_SELECT_PORT2:
            msWrite2ByteMask(REG_0033AC, 0x0207, MASKBIT(15:0)); // REG_PM_MHL_CBUS2_56 
            msWrite2ByteMask(REG_003394, BIT13, BIT13);// REG_PM_MHL_CBUS2_4A
            
            break;

        case MHL_INPUT_SELECT_PORT3:
            msWrite2ByteMask(REG_0035AC, 0x0207, MASKBIT(15:0)); // REG_PM_MHL_CBUS3_56 
            msWrite2ByteMask(REG_003594, BIT13, BIT13);// REG_PM_MHL_CBUS3_4A
            
            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_GetSRAMReceiveEMSCData()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE _mhal_mhl_GetSRAMReceiveEMSCData(void)
{
    msWriteByteMask(REG_002247, BIT7, BIT7); // REG_MHL_ECBUS2_23[15]

    while((msReadByte(REG_002247) & BIT6) == BIT6); // REG_MHL_ECBUS2_23[14]

    return msReadByte(REG_002244); // REG_MHL_ECBUS2_22[7:0]
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_GetSRAMReceiveEMSCData()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_mhl_InsertSRAMSendEMSCData(BYTE ucSendEMSC)
{
    msWriteByteMask(REG_002240, ucSendEMSC, MASKBIT(7:0)); // REG_MHL_ECBUS2_20[7:0]

    msWriteByteMask(REG_002243, BIT7, BIT7); // REG_MHL_ECBUS2_21[15]

    while((msReadByte(REG_002243) & BIT6) == BIT6); // REG_MHL_ECBUS2_21[14]
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_TMDSClkReset()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_mhl_TMDSClkReset(BYTE ucPortSelect)
{
    switch(ucPortSelect)
    {
        case MHL_INPUT_SELECT_PORT0:
            
            break;

        case MHL_INPUT_SELECT_PORT1:
            
            break;

        case MHL_INPUT_SELECT_PORT2:
        case MHL_INPUT_SELECT_PORT3:
            msWrite2ByteMask(REG_112464, 0, BIT12|BIT8|BIT4|BIT0); // REG_combo_phy4_32,  disable clock lock detect.
            msWrite2ByteMask(REG_112464, BIT12|BIT8|BIT4|BIT0, BIT12|BIT8|BIT4|BIT0); // REG_combo_phy4_32, Enable clock lock detect.
            
            break;

        default:

            break;
    };
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_EnableCbusInterrupt()
//  [Description]
//                  
//  [Arguments]:
//                  
//  [Return]:
//
//**************************************************************************
void mhal_mhl_EnableCbusInterrupt(Bool bEnableInterrupt)
{
    BYTE ucECbusMuxValue = msReadByte(REG_00170A) &MASKBIT(2:0); // REG_PM_BANK_05[2:0], eCbus mux

    if(ucECbusMuxValue == 2)
    {
        SET_MHL_ECBUS_MUX_SELECT(MHL_ECBUS_MUX_SELECT_PORT2);
    }
    else if(ucECbusMuxValue == 3)
    {
        SET_MHL_ECBUS_MUX_SELECT(MHL_ECBUS_MUX_SELECT_PORT3);
    }
    
    msWriteByteMask(REG_002B29, bEnableInterrupt? 0: BIT4, BIT4); // REG_IRQ_BANK_14[12]
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_GetCbusInterruptFlag()
//  [Description]
//                  
//  [Arguments]:
//                  
//  [Return]:
//
//**************************************************************************
Bool mhal_mhl_GetCbusInterruptFlag(void)
{
    return ((msReadByte(REG_002B39) & BIT4) ? TRUE : FALSE); // REG_IRQ_BANK_19[12]
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_LoadEDID()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void mhal_mhl_LoadEDID(BYTE ucPortSelect, BYTE *pComboEDID)
{
    WORD uctemp = 0; 

    if(pComboEDID != NULL)
    {
        MHL_HAL_DPRINTF("** MHL load EDID... port %d", ucPortSelect);
        
        switch(ucPortSelect)
        {
#if(MHL_FUNCTION_SUPPORT_PORT0)
            case MHL_INPUT_SELECT_PORT0:
                msWriteByteMask(REG_002EA4, BIT1, BIT1); // REG_MHL_CBUS0_52[1], CPU write enable
                
                for(uctemp = 0; uctemp <256; uctemp++)
                {        
                    msWriteByte(REG_002EA5, uctemp); // REG_MHL_CBUS0_52[15:8], address        
                    msWriteByte(REG_002EA6, pComboEDID[uctemp]); // REG_MHL_CBUS0_53[7:0], data
                    msWriteByteMask(REG_002EA4, BIT0, BIT0); // REG_MHL_CBUS0_52[0], Cpu write EDID SRAM triger.   
                    msWriteByteMask(REG_002EA4, 0, BIT0);
                    while(msReadByte(REG_002EA4) & BIT5); // REG_MHL_CBUS0_52[5], is write data busy       
                }    

                msWriteByteMask(REG_002EA4, 0, BIT1); // REG_MHL_CBUS0_52[1], CPU write disable
                
                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
            case MHL_INPUT_SELECT_PORT1:
                msWriteByteMask(REG_0030A4, BIT1, BIT1); // REG_MHL_CBUS1_52[1], CPU write enable
                
                for(uctemp = 0; uctemp <256; uctemp++)
                {        
                    msWriteByte(REG_0030A5, uctemp); // REG_MHL_CBUS1_52[15:8], address        
                    msWriteByte(REG_0030A6, pComboEDID[uctemp]); // REG_MHL_CBUS1_53[7:0], data
                    msWriteByteMask(REG_0030A4, BIT0, BIT0); // REG_MHL_CBUS1_52[0], Cpu write EDID SRAM triger.   
                    msWriteByteMask(REG_0030A4, 0, BIT0);
                    while(msReadByte(REG_0030A4) & BIT5); // REG_MHL_CBUS1_52[5], is write data busy       
                }    

                msWriteByteMask(REG_0030A4, 0, BIT1); // REG_MHL_CBUS1_52[1], CPU write disable
                
                break;
#endif

            case MHL_INPUT_SELECT_PORT2:
            case MHL_INPUT_SELECT_PORT3:
                msWriteByteMask(REG_0032A4, BIT1, BIT1); // REG_MHL_CBUS2_52[1], CPU write enable
                
                for(uctemp = 0; uctemp <256; uctemp++)
                {        
                    msWriteByte(REG_0032A5, uctemp); // REG_MHL_CBUS2_52[15:8], address        
                    msWriteByte(REG_0032A6, pComboEDID[uctemp]); // REG_MHL_CBUS2_53[7:0], data
                    msWriteByteMask(REG_0032A4, BIT0, BIT0); // REG_MHL_CBUS2_52[0], Cpu write EDID SRAM triger.   
                    msWriteByteMask(REG_0032A4, 0, BIT0);
                    while(msReadByte(REG_0032A4) & BIT5); // REG_MHL_CBUS2_52[5], is write data busy       
                }    

                msWriteByteMask(REG_0032A4, 0, BIT1); // REG_MHL_CBUS2_52[1], CPU write disable
                
                break;

            default:

                break;
        };
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_Initial()
//  [Description]
//                  MHL init
//  [Arguments]:
//                  *edid: MHL EDID data
//                  *devcap: MHL device capability
//  [Return]:
//
//**************************************************************************
void mhal_mhl_Initial(BYTE ucPortSelect, Bool bPowerIgnore, BYTE *pComboEDID, BYTE *pDevcap, BYTE ucVenderID)
{
    static Bool bShareSetting = FALSE;

    msEread_Init(&usECbusIDIndex, &ucECbusVersionIndex);

    // Enable cable detect pad
    _mhal_mhl_CableDetectEnable(ucPortSelect);
    // Set Cbus pull down 100K
    _mhal_mhl_SetCbusPullDown100K(ucPortSelect, TRUE);
    
    if(bPowerIgnore)
    {
        // Set pad PU1K
        _mhal_mhl_SetPadPullUp1K(ucPortSelect, TRUE);
    }
    
    if(!bShareSetting)
    {
        if(!bPowerIgnore)
        {
            // Cbus pad config
            _mhal_mhl_CbusPadSetting(ucPortSelect, pDevcap[MHL_CBUS_VERSION]);
            // Load EDID
            mhal_mhl_LoadEDID(ucPortSelect, pComboEDID);
            // Load device capability and Vendor ID 
            _mhal_mhl_LoadDevcapVenderID(ucPortSelect, pDevcap, ucVenderID);
            // Enable receive MSC interrupt
            _mhal_mhl_SetCbusReceiveInterrupt(ucPortSelect, TRUE);
        }

        // Disable stuck to low interrupt
        _mhal_mhl_SetStucktoLowInterrupt(ucPortSelect, FALSE);
        // Phy setting
        _mhal_mhl_ComboPhySetting(ucPortSelect);
    }

    // ECbus initial
    _mhal_mhl_ECbusInitialSetting(ucPortSelect);

    if((ucPortSelect == MHL_INPUT_SELECT_PORT2) || (ucPortSelect == MHL_INPUT_SELECT_PORT3))
    {
        if(!bShareSetting)
        {
            bShareSetting = TRUE;
        }
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_SetPullDownResistor()
//  [Description]
//                  
//  [Arguments]:
//                  
//  [Return]:
//                  
//**************************************************************************
void mhal_mhl_SetPullDownResistor(BYTE ucEfuseIndex)
{
    BYTE ucPD1KIndex = 0;
    BYTE ucPD100KIndex = 0;
    
    if((ucEfuseIndex & MHL_CBUS_EFUSE_TRIMMING) == MHL_CBUS_EFUSE_TRIMMING)
    {
        MHL_HAL_DPRINTF("** MHL trim index 0 %x", ucEfuseIndex); 
        
        if((ucEfuseIndex & MHL_CBUS_EFUSE_PD_1K_HIGH) == MHL_CBUS_EFUSE_PD_1K_HIGH)
        {
            ucPD1KIndex |= MHL_CBUS_PD_1K_HIGH_FLAG;
        }

        if((ucEfuseIndex & MHL_CBUS_EFUSE_PD_1K_LOW) == MHL_CBUS_EFUSE_PD_1K_LOW)
        {
            ucPD1KIndex |= MHL_CBUS_PD_1K_LOW_FLAG;
        }

        _mhal_mhl_TrimPullDown1K(MHL_INPUT_SELECT_PORT0, ucPD1KIndex);
        _mhal_mhl_TrimPullDown1K(MHL_INPUT_SELECT_PORT1, ucPD1KIndex);

        if((ucEfuseIndex & MHL_CBUS_EFUSE_PD_100K_HIGH) == MHL_CBUS_EFUSE_PD_100K_HIGH)
        {
            ucPD100KIndex |= MHL_CBUS_PD_100K_HIGH_FLAG;
        }

        if((ucEfuseIndex & MHL_CBUS_EFUSE_PD_100K_LOW) == MHL_CBUS_EFUSE_PD_100K_LOW)
        {
            ucPD100KIndex |= MHL_CBUS_PD_100K_LOW_FLAG;
        }

        _mhal_mhl_TrimPullDown100K(MHL_INPUT_SELECT_PORT0, ucPD100KIndex);
        _mhal_mhl_TrimPullDown100K(MHL_INPUT_SELECT_PORT1, ucPD100KIndex);
    }

    ucEfuseIndex = msEread_GetDataFromEfuse(EFUSE_0, MHL_CBUS_EFUSE_ADDRESS1);

    if((ucEfuseIndex & MHL_CBUS_EFUSE_TRIMMING) == MHL_CBUS_EFUSE_TRIMMING)
    {
        MHL_HAL_DPRINTF("** MHL trim index 1 %x", ucEfuseIndex);
        
        if((ucEfuseIndex & MHL_CBUS_EFUSE_PD_1K_HIGH) == MHL_CBUS_EFUSE_PD_1K_HIGH)
        {
            ucPD1KIndex |= MHL_CBUS_PD_1K_HIGH_FLAG;
        }

        if((ucEfuseIndex & MHL_CBUS_EFUSE_PD_1K_LOW) == MHL_CBUS_EFUSE_PD_1K_LOW)
        {
            ucPD1KIndex |= MHL_CBUS_PD_1K_LOW_FLAG;
        }

        _mhal_mhl_TrimPullDown1K(MHL_INPUT_SELECT_PORT2, ucPD1KIndex);
        _mhal_mhl_TrimPullDown1K(MHL_INPUT_SELECT_PORT3, ucPD1KIndex);

        if((ucEfuseIndex & MHL_CBUS_EFUSE_PD_100K_HIGH) == MHL_CBUS_EFUSE_PD_100K_HIGH)
        {
            ucPD100KIndex |= MHL_CBUS_PD_100K_HIGH_FLAG;
        }

        if((ucEfuseIndex & MHL_CBUS_EFUSE_PD_100K_LOW) == MHL_CBUS_EFUSE_PD_100K_LOW)
        {
            ucPD100KIndex |= MHL_CBUS_PD_100K_LOW_FLAG;
        }

        _mhal_mhl_TrimPullDown100K(MHL_INPUT_SELECT_PORT2, ucPD100KIndex);
        _mhal_mhl_TrimPullDown100K(MHL_INPUT_SELECT_PORT3, ucPD100KIndex);
    }
}
//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_SAR_Detect()
//  [Description]
//                  MHL SAR detection
//  [Arguments]:
//                  
//  [Return]:
//                  TRUE: MHL SAR plugged
//                  FALSE: MHL SAR unplugged
//**************************************************************************
Bool mhal_mhl_SAR_Detect(BYTE ucPortSelect)
{
    Bool bCableDetect = FALSE;

    // Use SAR to check MHL cable attach
    switch(ucPortSelect)
    {
    #if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            if (MHL_CABLE_DETECT_LEVEL(MHL_SAR_VALTAGE_MAX_TARGET)>=MHL_CABLE_SAR_DETECT_PORT0 && MHL_CABLE_SAR_DETECT_PORT0 >= MHL_CABLE_DETECT_LEVEL(MHL_SAR_VALTAGE_MIN_TARGET))
                bCableDetect = TRUE;            
            break;
    #endif

    #if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            if (MHL_CABLE_DETECT_LEVEL(MHL_SAR_VALTAGE_MAX_TARGET)>=MHL_CABLE_SAR_DETECT_PORT1 && MHL_CABLE_SAR_DETECT_PORT1 >= MHL_CABLE_DETECT_LEVEL(MHL_SAR_VALTAGE_MIN_TARGET))
                bCableDetect = TRUE;            
            break;
    #endif

    #if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            if (MHL_CABLE_DETECT_LEVEL(MHL_SAR_VALTAGE_MAX_TARGET)>=MHL_CABLE_SAR_DETECT_PORT2 && MHL_CABLE_SAR_DETECT_PORT2 >= MHL_CABLE_DETECT_LEVEL(MHL_SAR_VALTAGE_MIN_TARGET))
                bCableDetect = TRUE;
            break;
    #endif

    #if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            if (MHL_CABLE_DETECT_LEVEL(MHL_SAR_VALTAGE_MAX_TARGET)>=MHL_CABLE_SAR_DETECT_PORT3 && MHL_CABLE_SAR_DETECT_PORT3 >= MHL_CABLE_DETECT_LEVEL(MHL_SAR_VALTAGE_MIN_TARGET))
                bCableDetect = TRUE;
            break;
    #endif

        default:
            break;
    };

    return bCableDetect;
}
//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_CableDetect()
//  [Description]
//                  MHL cable detection
//  [Arguments]:
//                  
//  [Return]:
//                  TRUE: MHL cable plugged
//                  FALSE: MHL cable unplugged
//**************************************************************************
Bool mhal_mhl_CableDetect(BYTE ucPortSelect)
{
    Bool bCableDetect = FALSE;

    // Use SAR to check MHL cable attach
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            if(GET_MHL_CABLE_DETECT_ENABLE0() && mhal_mhl_SAR_Detect(ucPortSelect))
            {
                bCableDetect = TRUE;
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            if(GET_MHL_CABLE_DETECT_ENABLE1() && mhal_mhl_SAR_Detect(ucPortSelect))
            {
                bCableDetect = TRUE;
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            if(GET_MHL_CABLE_DETECT_ENABLE2() && mhal_mhl_SAR_Detect(ucPortSelect))
            {
                bCableDetect = TRUE;
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            if(GET_MHL_CABLE_DETECT_ENABLE3() && mhal_mhl_SAR_Detect(ucPortSelect))
            {
                bCableDetect = TRUE;
            }
            
            break;
#endif

        default:

            break;
    };

    ucCableDetectIndex = MASKSET(ucCableDetectIndex, bCableDetect? _BIT(ucPortSelect): 0, _BIT(ucPortSelect));

    return bCableDetect;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_CbusConnectStatus()
//  [Description]:
//                  MHL Cbus status
//  [Arguments]:
//
//  [Return]:
//                  Cbus status value      
//**************************************************************************
Bool mhal_mhl_CbusConnectStatus(BYTE ucPortSelect)
{
    Bool bCbusConnect = FALSE;
    BYTE ucCbusStatus = 0;

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            ucCbusStatus = msReadByte(REG_002F2E); // REG_PM_MHL_CBUS0_17[1:0], link layer status
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            ucCbusStatus = msReadByte(REG_00312E); // REG_PM_MHL_CBUS1_17[1:0], link layer status
            break;
#endif

        case MHL_INPUT_SELECT_PORT2:
        case MHL_INPUT_SELECT_PORT3:
            if(_mhal_mhl_GetECbusMux(ucPortSelect) == GET_MHL_ECBUS_MUX_SELECT())
            {
                ucCbusStatus = msReadByte(REG_00332E); // REG_PM_MHL_CBUS2_17[1:0], link layer status
            }
            
            break;

        default:

            break;
    };

    if(ucCbusStatus == 0x03)
    {
        bCbusConnect = TRUE;
    }

    return bCbusConnect;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_GetCbusBusyFlag()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
Bool mhal_mhl_GetCbusBusyFlag(BYTE ucPortSelect)
{
    Bool bFlag = FALSE;
    
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            bFlag = ((msReadByte(REG_002EBA) & 0xF0) != 0 ? TRUE : FALSE); // REG_MHL_CBUS0_5D[7:4]
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            bFlag = ((msReadByte(REG_0030BA) & 0xF0) != 0 ? TRUE : FALSE); // REG_MHL_CBUS1_5D[7:4]
            break;
#endif

        case MHL_INPUT_SELECT_PORT2:
            if(!GET_MHL_ECBUS_ENABLE_PORT2())
            {
                bFlag = ((msReadByte(REG_0032BA) & 0xF0) != 0 ? TRUE : FALSE); // REG_MHL_CBUS2_5D[7:4]
            }
            break;

        case MHL_INPUT_SELECT_PORT3:
            if(!GET_MHL_ECBUS_ENABLE_PORT3())
            {
                bFlag = ((msReadByte(REG_0032BA) & 0xF0) != 0 ? TRUE : FALSE); // REG_MHL_CBUS2_5D[7:4]
            }
            break;

        default:

            break;
    };

    return bFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_ComboRtermControl()
//  [Description]
//                  MHL TMDS termination resistor control
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void mhal_mhl_ComboRtermControl(BYTE ucPortSelect, RXRtermControl_T ucControl) 
{
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            if(ucECbusVersionIndex > 0)
            {
                if(ucControl == RX_HDMI_RTERM)
                {
                    msWriteByteMask(REG_112107, 0, MASKBIT(2:0)); // REG_COMBO_PHY1_03[10:8], Power on port0 rterm.
                    msWriteByteMask(REG_001724, 0, BIT4); // REG_PM_BANK_12[4], Power on port0 ch0 of rterm.
                }
                else if(ucControl == RX_MHL_RTERM)
                {
                    msWriteByteMask(REG_112107, 0, MASKBIT(2:0)); // REG_COMBO_PHY1_03[10:8], Power on port0 rterm.
                    msWriteByteMask(REG_001724, 0, BIT4); // REG_PM_BANK_12[4], Power on port0 ch0 of rterm.
                }
                else if(ucControl == RX_RTERM_OFF)
                {
                    msWriteByteMask(REG_112107, MASKBIT(2:0), MASKBIT(2:0)); // REG_COMBO_PHY1_03[10:8], Power on port0 rterm.
                    msWriteByteMask(REG_001724, BIT4, BIT4); // REG_PM_BANK_12[4], Power on port0 ch0 of rterm.
                }
            }
            else
            {
                if(ucControl == RX_HDMI_RTERM)
                {
                    msWriteByteMask(REG_112107, 0, MASKBIT(2:0)); // REG_COMBO_PHY1_03[10:8], Power on port0 rterm.
                }
                else if(ucControl == RX_MHL_RTERM)
                {
                    msWriteByteMask(REG_112107, 0, MASKBIT(2:0)); // REG_COMBO_PHY1_03[10:8], Power on port0's ch0 rterm.
                }
                else if(ucControl == RX_RTERM_OFF)
                {
                    msWriteByteMask(REG_112107, MASKBIT(2:0), MASKBIT(2:0)); // REG_COMBO_PHY1_03[10:8], Power down port0 rterm.
                }
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            if(ucECbusVersionIndex > 0)
            {
                if(ucControl == RX_HDMI_RTERM)
                {
                    msWriteByteMask(REG_112307, 0, MASKBIT(2:0)); // REG_COMBO_PHY3_03[10:8], Power on port1 rterm.
                    msWriteByteMask(REG_001726, 0, BIT4); // REG_PM_BANK_13[4], Power on port1 ch0 of rterm.
                }
                else if(ucControl == RX_MHL_RTERM)
                {
                    msWriteByteMask(REG_112307, 0, MASKBIT(2:0)); // REG_COMBO_PHY3_03[10:8], Power on port1 rterm.
                    msWriteByteMask(REG_001726, 0, BIT4); // REG_PM_BANK_13[4], Power on port1 ch0 of rterm.
                }
                else if(ucControl == RX_RTERM_OFF)
                {
                    msWriteByteMask(REG_112307, MASKBIT(2:0), MASKBIT(2:0)); // REG_COMBO_PHY3_03[10:8], Power on port1 rterm.
                    msWriteByteMask(REG_001726, BIT4, BIT4); // REG_PM_BANK_13[4], Power on port1 ch0 of rterm.
                }
            }
            else
            {
                if(ucControl == RX_HDMI_RTERM)
                {
                    msWriteByteMask(REG_112307, 0, MASKBIT(2:0)); // REG_COMBO_PHY3_03[10:8], Power on port1 rterm.
                }
                else if(ucControl == RX_MHL_RTERM)
                {
                    msWriteByteMask(REG_112307, 0, MASKBIT(2:0)); // REG_COMBO_PHY3_03[10:8], Power on port1's ch0 rterm.
                }
                else if(ucControl == RX_RTERM_OFF)
                {
                    msWriteByteMask(REG_112307, MASKBIT(2:0), MASKBIT(2:0)); // REG_COMBO_PHY3_03[10:8], Power down port1 rterm.
                }
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            if(ucControl == RX_HDMI_RTERM)
            {
                msWriteByteMask(REG_001720, 0, MASKBIT(3:0)); // REG_PM_BANK_10[3:0], Power on port2 rterm.
            }
            else if(ucControl == RX_MHL_RTERM)
            {
                msWriteByteMask(REG_001720, 0, MASKBIT(3:0)); // REG_PM_BANK_10[3:0], Power on port2's ch0 rterm. 
            }
            else if(ucControl == RX_RTERM_OFF)
            {
                msWriteByteMask(REG_001720, MASKBIT(3:0), MASKBIT(3:0)); // REG_PM_BANK_10[3:0], Power down port2 rterm. 
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            if(ucControl == RX_HDMI_RTERM)
            {
                msWriteByteMask(REG_001722, 0, MASKBIT(3:0)); // REG_PM_BANK_11[3:0], Power on port3 rterm.
            }
            else if(ucControl == RX_MHL_RTERM)
            {
                msWriteByteMask(REG_001722, 0, MASKBIT(3:0)); // REG_PM_BANK_11[3:0], Power on port3's ch0 rterm. 
            }
            else if(ucControl == RX_RTERM_OFF)
            {
                msWriteByteMask(REG_001722, MASKBIT(3:0), MASKBIT(3:0)); // REG_PM_BANK_11[3:0], Power on port3's ch0 rterm. 
            }
            
            break;
#endif

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_RtermControlHWMode()
//  [Description]:
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void mhal_mhl_RtermControlHWMode(BYTE ucPortSelect, Bool bRtermHW)
{
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            msWriteByteMask(REG_001707, bRtermHW? BIT4: 0, BIT4); // REG_PM_BANK_03[12], Cbus control port0 rterm.
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            msWriteByteMask(REG_001707, bRtermHW? BIT5: 0, BIT5); // REG_PM_BANK_03[13], Cbus control port1 rterm.
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            msWriteByteMask(REG_001707, bRtermHW? BIT6: 0, BIT6); // REG_PM_BANK_03[14], Cbus control port2 rterm.
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            msWriteByteMask(REG_001707, bRtermHW? BIT7: 0, BIT7); // REG_PM_BANK_03[15], Cbus control port3 rterm.
            break;
#endif

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_CDRModeMonitor()
//  [Description]:
//                  MHL CDR mode monitor
//  [Arguments]:
//                  isCbusConnected: Cbus is at connected state or not
//  [Return]:
//                 
//**************************************************************************
void mhal_mhl_CDRStableMonitor(BYTE ucPortSelect, Bool bDisplayReady)
{
    static BYTE ucStableCount[4] = {0, 0, 0, 0};
    
    if(bDisplayReady && ((_mhal_mhl_CheckClockStatus(ucPortSelect))||uMHLResetClkCnt))
    {
        if(ucStableCount[ucPortSelect] < (MHL_CDR_FORCE_THRESHOLD +1))
        {
            ucStableCount[ucPortSelect]++;
        }

        if(_mhal_mhl_CheckDataEnableStatus(ucPortSelect))
        {
            if(!bSignalStable[ucPortSelect])
            {
                _mhal_mhl_CDRPhySetting(ucPortSelect, TRUE);
                
                bSignalStable[ucPortSelect] = TRUE;
            }

            if(ucLinkRatePort[ucPortSelect] == MHL_AV_LINK_600)
            {

            }

            uMHLResetClkCnt = 0;
        }
        else if((uMHLResetClkCnt == 0) && (ucStableCount[ucPortSelect]==(MHL_CDR_FORCE_THRESHOLD +1)) && GET_MHL_PACKET_PIXEL_MODE_ENABLE())
        {
            _mhal_mhl_TMDSClkReset(ucPortSelect);
            uMHLResetClkCnt = 200;

        }
        else
        {
            if(uMHLResetClkCnt)
                uMHLResetClkCnt--;
         }
    }
    else if(bSignalStable[ucPortSelect])
    {
        _mhal_mhl_CDRPhySetting(ucPortSelect, FALSE);
        
        bSignalStable[ucPortSelect] = FALSE;

        ucStableCount[ucPortSelect] = 0;
        uMHLResetClkCnt = 0;
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_CbusWriteCommand()
//  [Description]
//                  MHL Cbus write trigger
//  [Arguments]:
//                  *pdatabuf: Cbus tx data
//  [Return]:
//
//**************************************************************************
Bool mhal_mhl_CbusWriteCommand(BYTE ucPortSelect, mhalCbusFifo_S *pdatabuf)
{
    Bool bFlag = FALSE;
    BYTE uctemp = 0;

    if(!mhal_mhl_GetCbusBusyFlag(ucPortSelect))
    {
        switch(ucPortSelect)
        {
#if(MHL_FUNCTION_SUPPORT_PORT0)
            case MHL_INPUT_SELECT_PORT0:
                for(uctemp = 0; uctemp <pdatabuf->lens; uctemp++)
                {
                    msWrite2Byte(REG_002E4C +uctemp *2, pdatabuf->databuf[uctemp]); // REG_MHL_CBUS0_26
                }
                
                // clear the unsed parts
                msWrite2Byte(REG_002E4C +((pdatabuf->lens) *2), 0);

                msWriteByteMask(REG_002E4B, BIT4, BIT4); // REG_MHL_CBUS0_25[12], trigger to send
                
                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
            case MHL_INPUT_SELECT_PORT1:
                for(uctemp = 0; uctemp <pdatabuf->lens; uctemp++)
                {
                    msWrite2Byte(REG_00304C +uctemp *2, pdatabuf->databuf[uctemp]); // REG_MHL_CBUS1_26
                }
                
                // clear the unsed parts
                msWrite2Byte(REG_00304C +((pdatabuf->lens) *2), 0);

                msWriteByteMask(REG_00304B, BIT4, BIT4); // REG_MHL_CBUS1_25[12], trigger to send
                
                break;
#endif

            case MHL_INPUT_SELECT_PORT2:
            case MHL_INPUT_SELECT_PORT3:
                for(uctemp = 0; uctemp <pdatabuf->lens; uctemp++)
                {
                    msWrite2Byte(REG_00324C +uctemp *2, pdatabuf->databuf[uctemp]); // REG_MHL_CBUS2_26
                }
                
                // clear the unsed parts
                msWrite2Byte(REG_00324C +((pdatabuf->lens) *2), 0);

                msWriteByteMask(REG_00324B, BIT4, BIT4); // REG_MHL_CBUS2_25[12], trigger to send
                
                break;

            default:

                break;
        };

        bFlag = TRUE;
    }

    return bFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_CbusSetPathEnable()
//  [Description]:
//                  MHL Cbus set path enable
//  [Arguments]:
//                  TRUE: Enable
//                  FALSE: Disable
//  [Return]:
//   
//**************************************************************************
void mhal_mhl_CbusSetPathEnable(BYTE ucPortSelect, Bool bSetPathEn)
{
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            if(bSetPathEn) // set state to PATH_EN
            {
                msWriteByteMask(REG_002F2F, BIT4, BIT4); // REG_PM_MHL_CBUS0_17[12], Sw set conn_state path.
            }
            else // clear state to not PATH_EN
            {
                msWriteByteMask(REG_002F2F, BIT5, BIT5); // REG_PM_MHL_CBUS0_17[13], Sw clr conn_state path.
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            if(bSetPathEn) // set state to PATH_EN
            {
                msWriteByteMask(REG_00312F, BIT4, BIT4); // REG_PM_MHL_CBUS1_17[12], Sw set conn_state path.
            }
            else // clear state to not PATH_EN
            {
                msWriteByteMask(REG_00312F, BIT5, BIT5); // REG_PM_MHL_CBUS1_17[13], Sw clr conn_state path.
            }
            
            break;
#endif

        case MHL_INPUT_SELECT_PORT2:
        case MHL_INPUT_SELECT_PORT3:
            if(bSetPathEn) // set state to PATH_EN
            {
                msWriteByteMask(REG_00332F, BIT4, BIT4); // REG_PM_MHL_CBUS2_17[12], Sw set conn_state path.
            }
            else // clear state to not PATH_EN
            {
                msWriteByteMask(REG_00332F, BIT5, BIT5); // REG_PM_MHL_CBUS2_17[13], Sw clr conn_state path.
            }
            
            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_SetRxXdataAddress()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void mhal_mhl_SetRxXdataAddress(BYTE ucPortSelect, Bool bFirstBlock)
{
    WORD usCommandAddress = 0;
        
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            if(bFirstBlock)
            {
                usCommandAddress = MHL_RX_XRAM_ADDR_FIRST_PORT0;
            }
            else
            {
                usCommandAddress = MHL_RX_XRAM_ADDR_SECOND_PORT0;
            }

            msWrite2ByteMask(REG_002ED6, usCommandAddress, MASKBIT(15:0)); // REG_MHL_CBUS0_6B[15:0], Xdata_write_address.
            msWriteByteMask(REG_002ED9, BIT7, BIT7); // REG_MHL_CBUS0_6C[15], reset xdata write address
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            if(bFirstBlock)
            {
                usCommandAddress = MHL_RX_XRAM_ADDR_FIRST_PORT1;
            }
            else
            {
                usCommandAddress = MHL_RX_XRAM_ADDR_SECOND_PORT1;
            }

            msWrite2ByteMask(REG_0030D6, usCommandAddress, MASKBIT(15:0)); // REG_MHL_CBUS1_6B[15:0], Xdata_write_address.
            msWriteByteMask(REG_0030D9, BIT7, BIT7); // REG_MHL_CBUS1_6C[15], reset xdata write address
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2 || MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT2:
        case MHL_INPUT_SELECT_PORT3:
            if(bFirstBlock)
            {
                #if(MHL_FUNCTION_SUPPORT_PORT2)
                usCommandAddress = MHL_RX_XRAM_ADDR_FIRST_PORT2;
                #elif(MHL_FUNCTION_SUPPORT_PORT3)
                usCommandAddress = MHL_RX_XRAM_ADDR_FIRST_PORT3;
                #endif
            }
            else
            {
                #if(MHL_FUNCTION_SUPPORT_PORT2)
                usCommandAddress = MHL_RX_XRAM_ADDR_SECOND_PORT2;
                #elif(MHL_FUNCTION_SUPPORT_PORT3)
                usCommandAddress = MHL_RX_XRAM_ADDR_SECOND_PORT3;
                #endif
            }

            msWrite2ByteMask(REG_0032D6, usCommandAddress, MASKBIT(15:0)); // REG_MHL_CBUS2_6B[15:0], Xdata_write_address.
            msWriteByteMask(REG_0032D9, BIT7, BIT7); // REG_MHL_CBUS2_6C[15], reset xdata write address
            
            break;
#endif

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_GetRxXdataValue()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
BYTE mhal_mhl_GetRxXdataValue(BYTE ucPortSelect, BYTE ucCommandAddress)//0~128	 alan.lee
{        
	BYTE ucXdataValue = 0;
    WORD usCbusXdataAddress = 0;
	
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
		    usCbusXdataAddress = MHL_CBUS_SRAM_ADDRESS_PORT0;
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
		    usCbusXdataAddress = MHL_CBUS_SRAM_ADDRESS_PORT1;
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2 || MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT2:
        case MHL_INPUT_SELECT_PORT3:
                #if(MHL_FUNCTION_SUPPORT_PORT2)
		    usCbusXdataAddress = MHL_CBUS_SRAM_ADDRESS_PORT2;
                #elif(MHL_FUNCTION_SUPPORT_PORT3)
                  usCbusXdataAddress = MHL_CBUS_SRAM_ADDRESS_PORT3;
                #endif
            break;
#endif

        default:

            break;
    };

    ucXdataValue = R2_XSRAM_READ(usCbusXdataAddress + ucCommandAddress);

    return ucXdataValue;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_GetCbusReceiveFlag()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
Bool mhal_mhl_GetCbusReceiveFlag(BYTE ucPortSelect)
{
    Bool bFlag = FALSE;
    
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            bFlag = ((msReadByte(REG_002E74) & BIT3) ? TRUE : FALSE); // REG_MHL_CBUS0_3A[3], Receive packet valid interrupt.

            if(bFlag)
            {
                msWriteByteMask(REG_002E74, BIT0, BIT0); // REG_MHL_CBUS0_3A[0]
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            bFlag = ((msReadByte(REG_003074) & BIT3) ? TRUE : FALSE); // REG_MHL_CBUS1_3A[3], Receive packet valid interrupt.

            if(bFlag)
            {
                msWriteByteMask(REG_003074, BIT0, BIT0); // REG_MHL_CBUS1_3A[0]
            }
            
            break;
#endif

        case MHL_INPUT_SELECT_PORT2:
        case MHL_INPUT_SELECT_PORT3:
            if(_mhal_mhl_GetECbusMux(ucPortSelect) == GET_MHL_ECBUS_MUX_SELECT())
            {
                bFlag = ((msReadByte(REG_003274) & BIT3) ? TRUE : FALSE); // REG_MHL_CBUS2_3A[3], Receive packet valid interrupt.

                if(bFlag)
                {
                    msWriteByteMask(REG_003274, BIT0, BIT0); // REG_MHL_CBUS2_3A[0]
                }
            }
            
            break;

        default:

            break;
    };

    return bFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_GetStucktoLowFlag()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
Bool mhal_mhl_GetStucktoLowFlag(BYTE ucPortSelect)
{
    Bool bFlag = FALSE;
    
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            bFlag = ((msReadByte(REG_002F30) &BIT3) ?TRUE: FALSE); // REG_PM_MHL_CBUS0_18[3], Cbus stuck to low interrupt.

            if(bFlag)
            {
                msWriteByteMask(REG_002F30, BIT0, BIT0); // REG_PM_MHL_CBUS0_18[0]
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            bFlag = ((msReadByte(REG_003130) &BIT3) ?TRUE: FALSE); // REG_PM_MHL_CBUS1_18[3], Cbus stuck to low interrupt.

            if(bFlag)
            {
                msWriteByteMask(REG_003130, BIT0, BIT0); // REG_PM_MHL_CBUS1_18[0]
            }
            
            break;
#endif

        case MHL_INPUT_SELECT_PORT2:
        case MHL_INPUT_SELECT_PORT3:
            if(_mhal_mhl_GetECbusMux(ucPortSelect) == GET_MHL_ECBUS_MUX_SELECT())
            {
                bFlag = ((msReadByte(REG_003330) &BIT3) ?TRUE: FALSE); // REG_PM_MHL_CBUS2_18[3], Cbus stuck to low interrupt.

                if(bFlag)
                {
                    msWriteByteMask(REG_003330, BIT0, BIT0); // REG_PM_MHL_CBUS2_18[0]
                }
            }
            
            break;

        default:

            break;
    };

    return bFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_GetECbusStateChangeFlag()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
Bool mhal_mhl_GetECbusStateChangeFlag(BYTE ucPortSelect)
{
    Bool bFlag = FALSE;
    
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            
            break;
#endif

        case MHL_INPUT_SELECT_PORT2:
        case MHL_INPUT_SELECT_PORT3:
            if(_mhal_mhl_GetECbusMux(ucPortSelect) == GET_MHL_ECBUS_MUX_SELECT())
            {
                bFlag = ((msReadByte(REG_002274) &BIT0) ?TRUE: FALSE); // REG_MHL_ECBUS_3A[0], ECbus state change interrupt.

                if(bFlag)
                {
                    msWriteByteMask(REG_002268, BIT0, BIT0); // REG_MHL_ECBUS_34[0]: Ecbus state change int clear
                }
            }
            
            break;

        default:

            break;
    };

    return bFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_GetEMSCReceiveFlag()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
Bool mhal_mhl_GetEMSCReceiveFlag(BYTE ucPortSelect)
{
    Bool bFlag = FALSE;
    
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            
            break;
#endif

        case MHL_INPUT_SELECT_PORT2:
        case MHL_INPUT_SELECT_PORT3:
            if(_mhal_mhl_GetECbusMux(ucPortSelect) == GET_MHL_ECBUS_MUX_SELECT())
            {
                bFlag = ((msReadByte(REG_002275) &BIT4) ?TRUE: FALSE); // REG_MHL_ECBUS_3A[12], eMSC receive int.

                if(bFlag)
                {
                    msWriteByteMask(REG_002269, BIT4, BIT4); // REG_MHL_ECBUS_34[12]: eMSC receive int clear
                }
            }
            
            break;

        default:

            break;
    };

    return bFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_GetEMSCSendStatus()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
BYTE mhal_mhl_GetEMSCSendStatus(BYTE ucPortSelect)
{
    BYTE ucSendStatus = MHL_EMSC_SEND_NONE;
    
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            
            break;
#endif

        case MHL_INPUT_SELECT_PORT2:
        case MHL_INPUT_SELECT_PORT3:
            if(_mhal_mhl_GetECbusMux(ucPortSelect) == GET_MHL_ECBUS_MUX_SELECT())
            {
                if(msReadByte(REG_002275) &BIT2) // REG_MHL_ECBUS_3A[10], eMSC send pass interrupt.
                {
                    msWriteByteMask(REG_002269, BIT2, BIT2); // REG_MHL_ECBUS_34[10]: eMSC send pass clear
                    
                    ucSendStatus = MHL_EMSC_SEND_PASS;
                }
                else if(msReadByte(REG_002275) &BIT1) // REG_MHL_ECBUS_3A[9], eMSC send fail interrupt.
                {
                    msWriteByteMask(REG_002269, BIT1, BIT1); // REG_MHL_ECBUS_34[10]: eMSC send fail clear

                    ucSendStatus = MHL_EMSC_SEND_FAIL;
                }
            }
            
            break;

        default:

            break;
    };

    return ucSendStatus;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_CbusForceToStandby()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void mhal_mhl_CbusForceToStandby(BYTE ucPortSelect)
{
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            msWriteByteMask(REG_002F2F, BIT7, BIT7); // REG_PM_MHL_CBUS0_17[15]
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            msWriteByteMask(REG_00312F, BIT7, BIT7); // REG_PM_MHL_CBUS1_17[15]
            break;
#endif

        case MHL_INPUT_SELECT_PORT2:
        case MHL_INPUT_SELECT_PORT3:
            msWriteByteMask(REG_00332F, BIT7, BIT7); // REG_PM_MHL_CBUS2_17[15]
            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_ClockModeSwitchProc()
//  [Description]
//                  
//  [Arguments]:
//                  
//  [Return]:
//
//**************************************************************************
void mhal_mhl_ClockModeSwitchProc(BYTE ucPortSelect, Bool bPacketPixelMode)
{
    if(bPacketPixelMode)
    {
        // Load packet pixel mode table
        _mhal_mhl_PacketPixelModeSetting(ucPortSelect);
    }
    else
    {
        // Load 24 bit table
        _mhal_mhl_24BitModeSetting(ucPortSelect);
    }

    if(_mhal_mhl_GetECbusMux(ucPortSelect) != MHL_ECBUS_MUX_SELECT_NONE) // ECbus port
    {
        if(bPacketPixelMode)
        {
            SET_MHL_PACKET_PIXEL_MODE_ENABLE();
        }
        else
        {
            CLR_MHL_PACKET_PIXEL_MODE_ENABLE();
        }
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_CablePlugProc()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void mhal_mhl_CablePlugProc(BYTE ucPortSelect)
{
    BYTE ucECbusMux = _mhal_mhl_GetECbusMux(ucPortSelect);

 printMsg("mhal_mhl_CablePlugProc");

    // Cbus enable
    _mhal_mhl_CbusPadEnable(ucPortSelect, TRUE);
    // Set HPD
    _mhal_mhl_SetHPDControl(ucPortSelect, TRUE);
    // Set cable detect
    _mhal_mhl_SetCableDetect(ucPortSelect, TRUE);
    // Remove Cbus pull up 1K
    _mhal_mhl_SetCbusPullUp1K(ucPortSelect, FALSE);
    // Vbus charge
    _mhal_mhl_SetVbusCharge(ucPortSelect, TRUE);
    // Rterm off
    mhal_mhl_ComboRtermControl(ucPortSelect, RX_RTERM_OFF);
    // Remove Cbus pull down 100K
    _mhal_mhl_SetCbusPullDown100K(ucPortSelect, FALSE);
    
    if(ucECbusMux == MHL_ECBUS_MUX_SELECT_NONE) // Not ECbus port
    {
        // Open common mode resistor
        _mhal_mhl_OpenCommonModeResistor(ucPortSelect, TRUE);
        // Load 24 bit table
        _mhal_mhl_24BitModeSetting(ucPortSelect);
    }
    else 
    {
        if((GET_MHL_ECBUS_MUX_SELECT() == MHL_ECBUS_MUX_SELECT_NONE) || (ucECbusMux == _mhal_mhl_GetECbusMux(GET_MHL_CURRENT_DISPLAY_PORT()))) // Current ECbus mux is none
        {
            _mhal_mhl_ECbusMuxSwitch(ucECbusMux);

            MHL_HAL_DPRINTF("** MHL switch ECbus mux to %d by cable plug", ucECbusMux); 
        }

        if(_mhal_mhl_CheckCurrentPort(ucPortSelect)) // Check on line port
        {
            // Open common mode resistor
            _mhal_mhl_OpenCommonModeResistor(ucPortSelect, TRUE);
            // Load 24 bit table
            _mhal_mhl_24BitModeSetting(ucPortSelect);

            bMHLPHYSettingFlag = TRUE;
        }
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_CableRemoveProc()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void mhal_mhl_CableRemoveProc(BYTE ucPortSelect)
{
    BYTE ucECbusMux = _mhal_mhl_GetECbusMux(ucPortSelect);

	//printMsg("mhal_mhl_CableRemoveProc");
    // Vbus discharge
    _mhal_mhl_SetVbusCharge(ucPortSelect, FALSE);
    // Set Cbus pull up 1K
    _mhal_mhl_SetCbusPullUp1K(ucPortSelect, TRUE);
    // Cbus disable
    _mhal_mhl_CbusPadEnable(ucPortSelect, FALSE);
    // Clear cable detect
    _mhal_mhl_SetCableDetect(ucPortSelect, FALSE);
    // Rterm HDMI
    mhal_mhl_ComboRtermControl(ucPortSelect, RX_HDMI_RTERM);
    // Disable stuck to low interrupt
    _mhal_mhl_SetStucktoLowInterrupt(ucPortSelect, FALSE);
    // Disable ECbus state change IRQ
    _mhal_mhl_SetECbusStateChangeInterrupt(ucPortSelect, FALSE);
    // Disable eMSC receive IRQ
    _mhal_mhl_SetEMSCReceiveInterrupt(ucPortSelect, FALSE);
    // Set Cbus pull down 100K
    _mhal_mhl_SetCbusPullDown100K(ucPortSelect, TRUE);
    
    if(ucECbusMux == MHL_ECBUS_MUX_SELECT_NONE) // Not ECbus port
    {
        // Short common mode resistor
        _mhal_mhl_OpenCommonModeResistor(ucPortSelect, TRUE);
        // Load HDMI table
        _mhal_mhl_HDMIByPassSetting(ucPortSelect);
    }
    else 
    {
        if(_mhal_mhl_CheckCurrentPort(ucPortSelect)) // Check on line port
        {
            // Short common mode resistor
            _mhal_mhl_OpenCommonModeResistor(ucPortSelect, TRUE);

            if(bMHLPHYSettingFlag)
            {
                // Load HDMI table
                _mhal_mhl_HDMIByPassSetting(ucPortSelect);
            }
            
            // Disable MHL3 phy
            _mhal_mhl_Version3PhyEnable(ucPortSelect, FALSE);

            CLR_MHL_PACKET_PIXEL_MODE_ENABLE();

            bMHLPHYSettingFlag = FALSE;
        }
        
        if(GET_MHL_ECBUS_MUX_SELECT() == ucECbusMux) // Current ECbus mux is this port
        {
            ucECbusMux = _mhal_mhl_GetNextECbusMux(ucPortSelect);
        
            _mhal_mhl_ECbusMuxSwitch(ucECbusMux);

            MHL_HAL_DPRINTF("** MHL switch ECbus mux to %d by cable remove", ucECbusMux);
        }

        ucLinkRatePort[ucPortSelect] = MHL_AV_LINK_NONE;
        // Disable ECbus
        _mhal_mhl_ECbusEnableSetting(ucPortSelect, FALSE);
        // Set short read address to 0x08
        _mhal_mhl_SetShortReadAddress(ucPortSelect, FALSE);
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_CbusConnectProc()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void mhal_mhl_CbusConnectProc(BYTE ucPortSelect)
{
    // Rterm MHL
    mhal_mhl_ComboRtermControl(ucPortSelect, RX_MHL_RTERM);
    // Enable stuck to low interrupt
    _mhal_mhl_SetStucktoLowInterrupt(ucPortSelect, TRUE);

    if((_mhal_mhl_GetECbusMux(ucPortSelect) == MHL_ECBUS_MUX_SELECT_NONE) || _mhal_mhl_CheckCurrentPort(GET_MHL_CURRENT_DISPLAY_PORT())) // Not ECbus port
    {
        // Short common mode resistor
        _mhal_mhl_OpenCommonModeResistor(ucPortSelect, FALSE);
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_CbusStucktoLowProc()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void mhal_mhl_CbusStucktoLowProc(BYTE ucPortSelect)
{
    // Rterm off
    mhal_mhl_ComboRtermControl(ucPortSelect, RX_RTERM_OFF);
    // Disable stuck to low interrupt
    _mhal_mhl_SetStucktoLowInterrupt(ucPortSelect, FALSE);
    
    if(_mhal_mhl_GetECbusMux(ucPortSelect) == MHL_ECBUS_MUX_SELECT_NONE) // Not ECbus port
    {
        // Open common mode resistor
        _mhal_mhl_OpenCommonModeResistor(ucPortSelect, TRUE);
    }
    else if(_mhal_mhl_CheckCurrentPort(ucPortSelect))
    {
        // Open common mode resistor
        _mhal_mhl_OpenCommonModeResistor(ucPortSelect, TRUE);

        CLR_MHL_PACKET_PIXEL_MODE_ENABLE();

        ucLinkRatePort[ucPortSelect] = MHL_AV_LINK_NONE;

        uMHLResetClkCnt = 0;
        // Set short read address to 0x08
        _mhal_mhl_SetShortReadAddress(ucPortSelect, FALSE);
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_SourceChangeProc()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void mhal_mhl_SourceChangeProc(BYTE ucDisplayIndex)
{
    BYTE ucECbusMux = MHL_ECBUS_MUX_SELECT_NONE;
    static BYTE ucPreDisplayIndex = 0;

    ucDisplayIndex = ucDisplayIndex &MASKBIT(3:2);
    
    if(ucPreDisplayIndex != ucDisplayIndex)
    {
        ucDisplayIndex = (ucDisplayIndex ^ucPreDisplayIndex) &ucDisplayIndex;

        if((ucDisplayIndex &BIT2) == BIT2)
        {
            SET_MHL_CURRENT_DISPLAY_PORT(MHL_INPUT_SELECT_PORT2);
        }
        else if((ucDisplayIndex &BIT3) == BIT3)
        {
            SET_MHL_CURRENT_DISPLAY_PORT(MHL_INPUT_SELECT_PORT3);
        }
        else
        {
            SET_MHL_CURRENT_DISPLAY_PORT(MHL_INPUT_SELECT_NONE);
        }

        ucECbusMux = _mhal_mhl_GetECbusMux(GET_MHL_CURRENT_DISPLAY_PORT());

        if(ucECbusMux != MHL_ECBUS_MUX_SELECT_NONE) // ECbus port
        {
            // ECbus mux setting
            if(GET_MHL_CABLE_DETECT_PORT(GET_MHL_CURRENT_DISPLAY_PORT()) && (ucECbusMux != GET_MHL_ECBUS_MUX_SELECT())) // ECbus mux is not current ECbus mux, must switch ECbus mux
            {
                _mhal_mhl_ECbusMuxSwitch(ucECbusMux);

                MHL_HAL_DPRINTF("** MHL switch ECbus mux to %d by source change", ucECbusMux); 
            }

            // PHY setting
            if(_mhal_mhl_CheckCurrentPort(GET_MHL_CURRENT_DISPLAY_PORT()))
            {
                if(mhal_mhl_CbusConnectStatus(GET_MHL_CURRENT_DISPLAY_PORT()))
                {
                    // Short common mode resistor
                    _mhal_mhl_OpenCommonModeResistor(GET_MHL_CURRENT_DISPLAY_PORT(), FALSE);
                }
                
                // Load 24 bit table
                _mhal_mhl_24BitModeSetting(GET_MHL_CURRENT_DISPLAY_PORT());

                if(GET_MHL_PACKET_PIXEL_MODE_ENABLE())
                {
                    // Load packet pixel mode table
                    _mhal_mhl_PacketPixelModeSetting(GET_MHL_CURRENT_DISPLAY_PORT());
                }

                if(ucLinkRatePort[GET_MHL_CURRENT_DISPLAY_PORT()] < MHL_AV_LINK_NONE)
                {
                    // Enable MHL3 phy
                    _mhal_mhl_Version3PhyEnable(GET_MHL_CURRENT_DISPLAY_PORT(), TRUE);
                    
                    mhal_mhl_SetMainLinkRate(GET_MHL_CURRENT_DISPLAY_PORT(), ucLinkRatePort[GET_MHL_CURRENT_DISPLAY_PORT()]);
                }

                bMHLPHYSettingFlag = TRUE;
            }
            else if(bMHLPHYSettingFlag)
            {
                // Short common mode resistor
                _mhal_mhl_OpenCommonModeResistor(GET_MHL_CURRENT_DISPLAY_PORT(), TRUE);
                // Load HDMI table
                _mhal_mhl_HDMIByPassSetting(GET_MHL_CURRENT_DISPLAY_PORT());
                // Disable MHL3 phy
                _mhal_mhl_Version3PhyEnable(GET_MHL_CURRENT_DISPLAY_PORT(), FALSE);

                bMHLPHYSettingFlag = FALSE;
            }
        }

        ucPreDisplayIndex = ucDisplayIndex;
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_PowerDownProc()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void mhal_mhl_PowerDownProc(BYTE ucPortSelect)
{
    // Cbus disable
    _mhal_mhl_CbusPadEnable(ucPortSelect, FALSE);
    // Clear cable detect
    _mhal_mhl_SetCableDetect(ucPortSelect, FALSE);

    if(mhal_mhl_CableDetect(ucPortSelect))
    {
        // Remove pad PU1K
        _mhal_mhl_SetPadPullUp1K(ucPortSelect, FALSE);
        // Enable Rterm HW control
        mhal_mhl_RtermControlHWMode(ucPortSelect, TRUE);
    }
    
    if(_mhal_mhl_GetECbusMux(ucPortSelect) == GET_MHL_ECBUS_MUX_SELECT())
    {
        _mhal_mhl_ECbusMuxSwitch(MHL_ECBUS_MUX_SELECT_NONE);
    }   
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_PMStatePolling()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_mhl_PMStatePolling(BYTE ucPortSelect)
{
    Bool bCableDetect = mhal_mhl_CableDetect(ucPortSelect);

//printData("mhal_mhl_PMStatePolling==%d",bCableDetect);
    // Vbus charge
    _mhal_mhl_SetVbusCharge(ucPortSelect, bCableDetect);
    // Set cable detect
    _mhal_mhl_SetCableDetect(ucPortSelect, bCableDetect);
    // Rterm HW control
    mhal_mhl_RtermControlHWMode(ucPortSelect, bCableDetect);
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_CableDetectEnableControl()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_mhl_CableDetectEnableControl(BYTE ucPortSelect, Bool bCableDetectEnable)
{
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            if(bCableDetectEnable)
            {
                SET_MHL_CABLE_DETECT_ENABLE0();
            }
            else
            {
                CLR_MHL_CABLE_DETECT_ENABLE0();
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            if(bCableDetectEnable)
            {
                SET_MHL_CABLE_DETECT_ENABLE1();
            }
            else
            {
                CLR_MHL_CABLE_DETECT_ENABLE1();
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            if(bCableDetectEnable)
            {
                SET_MHL_CABLE_DETECT_ENABLE2();
            }
            else
            {
                CLR_MHL_CABLE_DETECT_ENABLE2();
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            if(bCableDetectEnable)
            {
                SET_MHL_CABLE_DETECT_ENABLE3();
            }
            else
            {
                CLR_MHL_CABLE_DETECT_ENABLE3();
            }
            
            break;
#endif

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_CheckSignalStable()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mhal_mhl_CheckSignalStable(BYTE ucPortSelect)
{
    return bSignalStable[ucPortSelect];
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_GetECbusStatusFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE mhal_mhl_GetECbusStatusFlag(BYTE ucPortSelect)
{
    BYTE uctemp = 0;
    BYTE ucECbusStatusFlag = 0;
    
    switch(ucPortSelect)
    {
        case MHL_INPUT_SELECT_PORT0:
            
            break;

        case MHL_INPUT_SELECT_PORT1:
            
            break;

        case MHL_INPUT_SELECT_PORT2:
            if(msReadByte(REG_0033DF) &BIT5) // REG_PM_MHL_CBUS2_6F[13]: ECbus clock lock flag
            {
                ucECbusStatusFlag |= MHL_ECBUS_STATUS_CLOCK_LOCK;
            }

            uctemp = (msReadByte(REG_0033D3)&0xF0);
    
            if((uctemp >= 0x50) && (uctemp <= 0x70)) //Whan state of REG_PM_MHL_CBUS2_56 is equla 3, than state of REG_PM_MHL_CBUS2_69[51:12] will from 1 to 7. 
            {
                msWrite2ByteMask(REG_0033AC, 0x0200, MASKBIT(15:0)); // REG_PM_MHL_CBUS2_56 for auto train
                msWrite2ByteMask(REG_003394, 0, BIT13);
            }

            break;
            
        case MHL_INPUT_SELECT_PORT3:
            if(msReadByte(REG_0035DF) &BIT5) // REG_PM_MHL_CBUS2_6F[13]: ECbus clock lock flag
            {
                ucECbusStatusFlag |= MHL_ECBUS_STATUS_CLOCK_LOCK;
            }

            uctemp = (msReadByte(REG_0035D3)&0xF0);
    
            if((uctemp >= 0x50) && (uctemp <= 0x70)) //Whan state of REG_PM_MHL_CBUS2_56 is equla 3, than state of REG_PM_MHL_CBUS2_69[51:12] will from 1 to 7. 
            {
                msWrite2ByteMask(REG_0035AC, 0x0200, MASKBIT(15:0)); // REG_PM_MHL_CBUS2_56 for auto train
                msWrite2ByteMask(REG_003594, 0, BIT13);
            }
            
            break;

        default:

            break;
    };

    if(_mhal_mhl_GetECbusMux(ucPortSelect) == GET_MHL_ECBUS_MUX_SELECT())
    {
        if((ucECbusStatePort[ucPortSelect] < MHL_ECBUS_STATE_FAIL) && (ucECbusStatePort[ucPortSelect] >= MHL_ECBUS_STATE_SOURCE_ACTIVE))
        {
            ucECbusStatusFlag |= MHL_ECBUS_STATUS_TRAINING_PASS;
        }

        if(msReadByte(REG_002274) &BIT7) // REG_MHL_ECBUS2_3A[7]
        {
            ucECbusStatusFlag |= MHL_ECBUS_STATUS_SLOT_SYNC_DONE;
        }

        if(msReadByte(REG_002275) &BIT7) // REG_MHL_ECBUS2_3A[15]
        {
            ucECbusStatusFlag |= MHL_ECBUS_STATUS_EMSC_SYNC_DONE;
        }

        if(ucECbusTrainFailCount[ucPortSelect] > 10)
        {
            ucECbusStatusFlag |= MHL_ECBUS_STATUS_RETRY_TIMEOUT;

            ucECbusTrainFailCount[ucPortSelect] = 0;
        }
    }

    return ucECbusStatusFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_ECbusStateChangeProc()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_mhl_ECbusStateChangeProc(BYTE ucPortSelect)
{
    BYTE ucECbusState = MHL_ECBUS_STATE_TRAINING_NONE;
    
    switch(ucPortSelect)
    {
        case MHL_INPUT_SELECT_PORT0:
            
            break;

        case MHL_INPUT_SELECT_PORT1:
            
            break;

        case MHL_INPUT_SELECT_PORT2:
            if(_mhal_mhl_GetECbusMux(ucPortSelect) == GET_MHL_ECBUS_MUX_SELECT())
            {
                ucECbusState = (msReadByte(REG_002206) &0xF0) >> 4; // REG_MHL_CBUS2_03[7:4]: ECbus state

                if(ucECbusState != MHL_ECBUS_STATE_TRAINING_NONE)
                {
                    if(ucECbusState == MHL_ECBUS_STATE_WAIT_CLOCK_LOCK)
                    {
                        msWriteByteMask(REG_00339A, 0, BIT0); // REG_PM_MHL_CBUS2_4D[0]: ECbus PLL pd
                    }
                    else if(ucECbusState == MHL_ECBUS_STATE_WAIT_SEND_CLOCK)
                    {
                        if(msReadByte(REG_00339B) & BIT7)
                        {
                            _mhal_mhl_ECbusDmuxEnable(ucPortSelect, TRUE);
                        }
                        
                        msWriteByteMask(REG_002268, BIT4, BIT4); // REG_MHL_ECBUS2_34[4]
                    }
                    else if(ucECbusState == MHL_ECBUS_STATE_SOURCE_ACTIVE)
                    {

                    }
                    else if(ucECbusState == MHL_ECBUS_STATE_ACTIVE)
                    {

                    }
                    else if(ucECbusState == MHL_ECBUS_STATE_FAIL)
                    {
                        _mhal_mhl_ECbusDmuxEnable(ucPortSelect, FALSE);

                        msWrite2ByteMask(REG_0033AC, 0x0207, MASKBIT(15:0)); // REG_PM_MHL_CBUS2_56 
                        msWrite2ByteMask(REG_003394, BIT13, BIT13);// REG_PM_MHL_CBUS2_4A
                        msWriteByteMask(REG_00339A,  BIT0, BIT0); // REG_PM_MHL_CBUS2_4D[0]: ECbus PLL pd
                        msWrite2ByteMask(REG_003396, BIT13|BIT12, MASKBIT(14:12)); // REG_PM_MHL_CBUS2_4B, delay

                        ucECbusTrainFailCount[ucPortSelect]++;
                    }

                    ucECbusStatePort[ucPortSelect] = ucECbusState;
                }
            }
            
            break;

        case MHL_INPUT_SELECT_PORT3:
            if(_mhal_mhl_GetECbusMux(ucPortSelect) == GET_MHL_ECBUS_MUX_SELECT())
            {
                ucECbusState = (msReadByte(REG_002206) &0xF0) >> 4; // REG_MHL_CBUS2_03[7:4]: ECbus state

                if(ucECbusState != MHL_ECBUS_STATE_TRAINING_NONE)
                {
                    if(ucECbusState == MHL_ECBUS_STATE_WAIT_CLOCK_LOCK)
                    {
                        msWriteByteMask(REG_00359A, 0, BIT0); // REG_PM_MHL_CBUS3_4D[0]: ECbus PLL pd
                    }
                    else if(ucECbusState == MHL_ECBUS_STATE_WAIT_SEND_CLOCK)
                    {
                        if(msReadByte(REG_00359B) & BIT7)
                        {
                            _mhal_mhl_ECbusDmuxEnable(ucPortSelect, TRUE);
                        }
                        
                        msWriteByteMask(REG_002268, BIT4, BIT4); // REG_MHL_ECBUS2_34[4]
                    }
                    else if(ucECbusState == MHL_ECBUS_STATE_SOURCE_ACTIVE)
                    {

                    }
                    else if(ucECbusState == MHL_ECBUS_STATE_ACTIVE)
                    {

                    }
                    else if(ucECbusState == MHL_ECBUS_STATE_FAIL)
                    {
                        _mhal_mhl_ECbusDmuxEnable(ucPortSelect, FALSE);

                        msWrite2ByteMask(REG_0035AC, 0x0207, MASKBIT(15:0)); // REG_PM_MHL_CBUS3_56 
                        msWrite2ByteMask(REG_003594, BIT13, BIT13);// REG_PM_MHL_CBUS3_4A
                        msWriteByteMask(REG_00335A,  BIT0, BIT0); // REG_PM_MHL_CBUS3_4D[0]: ECbus PLL pd
                        msWrite2ByteMask(REG_003596, BIT13|BIT12, MASKBIT(14:12)); // REG_PM_MHL_CBUS3_4B, delay

                        ucECbusTrainFailCount[ucPortSelect]++;
                    }

                    ucECbusStatePort[ucPortSelect] = ucECbusState;
                }
            }
            
            break;
            
        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_SetMainLinkRate()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_mhl_SetMainLinkRate(BYTE ucPortSelect, BYTE ucLinkRate)
{
    if(ucLinkRate < MHL_AV_LINK_NONE)
    {
        _mhal_mhl_Version3PhyEnable(ucPortSelect, TRUE);
    
        switch(ucPortSelect)
        {
            case MHL_INPUT_SELECT_PORT0:
                
                break;

            case MHL_INPUT_SELECT_PORT1:
                
                break;

            case MHL_INPUT_SELECT_PORT2:
            case MHL_INPUT_SELECT_PORT3:
                if(_mhal_mhl_GetECbusMux(ucPortSelect) == GET_MHL_ECBUS_MUX_SELECT())
                {
                    if(ucLinkRate == MHL_AV_LINK_600)
                    {
                        msWriteByteMask(REG_1124D6, BIT1, MASKBIT(2:1)); // REG_COMBO_PHY4_6B[2]: enable clko_pix 2x; [1]: reg_atop_en_clko_pix=1
                        msWriteByteMask(REG_1124B4, 0, BIT2); // REG_COMBO_PHY4_5A[2]: disable reg_atop_en_clko_tmds2x
                        msWriteByteMask(REG_163060, BIT2, MASKBIT(3:2)| MASKBIT(1:0)); // REG_COMBO_GP_TOP_30[0]: reg_tmds_clk_div2_en_p2
                        msWriteByteMask(REG_1612A8, BIT1, MASKBIT(1:0)); // REG_HDMI20_DUAL1_54[1:0]: reg_avg_ctrl_case
                    }
                    else // MHL_AV_LINK_150 or MHL_AV_LINK_300
                    {
                        msWriteByteMask(REG_1124D6, MASKBIT(2:1), MASKBIT(2:1)); // REG_COMBO_PHY4_6B[2]: enable clko_pix 2x; [1]: reg_atop_en_clko_pix=1
                        msWriteByteMask(REG_1124B4, BIT2, BIT2); // REG_COMBO_PHY4_5A[2]: disable reg_atop_en_clko_tmds2x
                        msWriteByteMask(REG_163060, BIT2| BIT0, MASKBIT(3:2)| MASKBIT(1:0)); // REG_COMBO_GP_TOP_30[2]: reg_pix_clk_div2_en_p2; [0]: reg_tmds_clk_div2_en_p2
                        msWriteByteMask(REG_1612A8, BIT1, MASKBIT(1:0)); // REG_HDMI20_DUAL1_54[1:0]: reg_avg_ctrl_case                        
                    }

                    msWriteByteMask(REG_11251B, (ucLinkRate << 4), MASKBIT(5:4)); // REG_COMBO_PHY5_0D[13:12]: Select MHL3 data rate
                }
                
                break;

            default:

                break;
        };
    }

    ucLinkRatePort[ucPortSelect] = ucLinkRate;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_ECbusEventProc()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_mhl_ECbusEventProc(BYTE ucPortSelect, BYTE ucECbusEvent)
{
    switch(ucECbusEvent)
    {
        case MHL_ECBUS_EVENT_PREPARE_MODE_UP:
            // Disable stuck to low interrupt
            _mhal_mhl_SetStucktoLowInterrupt(ucPortSelect, FALSE);
            // Disable connect detect
            _mhal_mhl_CbusConnectCheckEnable(ucPortSelect, FALSE);
            
            break;

        case MHL_ECBUS_EVENT_MODE_UP_PROCESS:
            // Enable ECbus
            _mhal_mhl_ECbusEnableSetting(ucPortSelect, TRUE);
            // Set short read address to 0x70
            _mhal_mhl_SetShortReadAddress(ucPortSelect, TRUE);
            
            _mhal_mhl_ECbusModeUpProc(ucPortSelect);
            // Enable ECbus state change IRQ
            _mhal_mhl_SetECbusStateChangeInterrupt(ucPortSelect, TRUE);
            // Enable eMSC receive IRQ
            _mhal_mhl_SetEMSCReceiveInterrupt(ucPortSelect, TRUE);
            
            ucECbusTrainFailCount[ucPortSelect] = 0;
            
            break;

        case MHL_ECBUS_EVENT_PREPARE_MODE_DOWN:
            
            break;
            
        case MHL_ECBUS_EVENT_MODE_DOWN_PROCESS:
            // Disable ECbus
            _mhal_mhl_ECbusEnableSetting(ucPortSelect, FALSE);
            // Set short read address to 0x08
            _mhal_mhl_SetShortReadAddress(ucPortSelect, FALSE);
            
            _mhal_mhl_ECbusModeDownProc(ucPortSelect);
            
            break;

        case MHL_ECBUS_EVENT_UNLOCK_RESET:
            _mhal_mhl_ECbusDmuxEnable(ucPortSelect, FALSE);
            _mhal_mhl_ECbusStateReset(ucPortSelect);
            
            ucECbusTrainFailCount[ucPortSelect] = 0;
            
            break;

        case MHL_ECBUS_EVENT_STUCK_TO_LOW:
            // Enable connect detect
            _mhal_mhl_CbusConnectCheckEnable(ucPortSelect, TRUE);
            // Disable ECbus
            _mhal_mhl_ECbusEnableSetting(ucPortSelect, FALSE);
            // Disable ECbus state change IRQ
            _mhal_mhl_SetECbusStateChangeInterrupt(ucPortSelect, FALSE);
            // Disable eMSC receive IRQ
            _mhal_mhl_SetEMSCReceiveInterrupt(ucPortSelect, FALSE);
            // Disable MHL3 phy
            _mhal_mhl_Version3PhyEnable(ucPortSelect, FALSE);
            
            break;
                
        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_GetEMSCReceiveData()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_mhl_GetEMSCReceiveData(BYTE ucPortSelect, BYTE *bReveiceEMSC)
{
    BYTE uctemp = 0;
    WORD usEMSCFreeBuffer = 0;
    
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
		    
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
		    
            break;
#endif

        case MHL_INPUT_SELECT_PORT2:
        case MHL_INPUT_SELECT_PORT3:
		    usEMSCFreeBuffer = (msRead2Byte(REG_002248) &MASKBIT(9:0)); // REG_MHL_ECBUS2_24[9:0]

            usEMSCFreeBuffer = MHL_EMSC_DATA_SIZE - usEMSCFreeBuffer;
            
            MHL_HAL_DPRINTF("** MHL eMSC length %d", usEMSCFreeBuffer); 

            for(uctemp = 0; uctemp < usEMSCFreeBuffer; uctemp++)
            {
                bReveiceEMSC[uctemp +1] = _mhal_mhl_GetSRAMReceiveEMSCData();

                if(uctemp == 1) // Check length remaining
                {
                    usEMSCFreeBuffer = bReveiceEMSC[uctemp +1] +2;
                }

                MHL_HAL_DPRINTF("** MHL eMSC data %x", bReveiceEMSC[uctemp +1]);
            }

            bReveiceEMSC[0] = usEMSCFreeBuffer;

            msWrite2ByteMask(REG_002238, usEMSCFreeBuffer, MASKBIT(9:0)); // REG_MHL_ECBUS2_1C[9:0]
            msWriteByteMask(REG_002239, BIT7, BIT7); // REG_MHL_ECBUS2_1C[15]
            
            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_InsertEMSCSendData()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_mhl_InsertEMSCSendData(BYTE ucPortSelect, BYTE ucLength, BYTE *bSendEMSC)
{
    BYTE uctemp = 0;
    WORD usPayloadCRC = 0xFFFF;

    if((ucLength > 0) && (bSendEMSC != NULL))
    {
        switch(ucPortSelect)
        {
#if(MHL_FUNCTION_SUPPORT_PORT0)
            case MHL_INPUT_SELECT_PORT0:
    		    
                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
            case MHL_INPUT_SELECT_PORT1:
    		    
                break;
#endif

            case MHL_INPUT_SELECT_PORT2:
            case MHL_INPUT_SELECT_PORT3:
                msWrite2ByteMask(REG_002236, (ucLength -1), MASKBIT(9:0)); // REG_MHL_ECBUS2_1B[9:0]: Request command byte count
                msWriteByteMask(REG_0022F2, _mhal_mhl_GetEMSCTwoByteCRC(0, (ucLength -1)) , MASKBIT(2:0)); // REG_MHL_ECBUS2_79[2:0]:byte count CRC

                msWrite2ByteMask(REG_002242, 0, MASKBIT(9:0)); // REG_MHL_ECBUS2_21[9:0]: SRAM address set 0
                msWrite2ByteMask(REG_002243, BIT5, BIT5); // REG_MHL_ECBUS2_21[13]: Trigger address
                
                MHL_HAL_DPRINTF("** MHL request CRC value %x", _mhal_mhl_GetEMSCTwoByteCRC(0, (ucLength -1)));
                
                for(uctemp = 0; uctemp < ucLength; uctemp++)
                {
                    _mhal_mhl_InsertSRAMSendEMSCData(bSendEMSC[uctemp]);
                    
                    usPayloadCRC = _mhal_mhl_GetEMSCPayloadCRC(bSendEMSC[uctemp], usPayloadCRC);

                    MHL_HAL_DPRINTF("** MHL payload value %x", bSendEMSC[uctemp]);
                }

                msWriteByteMask(REG_002233, BIT7, BIT7); // REG_MHL_ECBUS2_19[15]: eMSC payload CRC ove
                msWrite2Byte(REG_002234, (usPayloadCRC ^0xFFFF)); // REG_MHL_ECBUS2_1A[15:0]: eMSC payload CRC value

                MHL_HAL_DPRINTF("** MHL payload CRC value %x", (usPayloadCRC ^0xFFFF));
                
                msWriteByteMask(REG_002237, BIT7, BIT7); // REG_MHL_ECBUS2_1B[15]: Trigger send eMSC command
                
                break;

            default:

                break;
        };
    }
}
//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_CablePlugProcByInit
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_mhl_CablePlugProcByInit(BYTE ucInputPort)
{    
    _mhal_mhl_SetCbusPullDown100K(ucInputPort, FALSE);
}

#endif // _MHAL_MHL_C_


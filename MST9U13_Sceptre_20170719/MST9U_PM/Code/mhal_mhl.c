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
#include "Board.h"

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
BYTE ucMHLPowerStatus = MHL_POWER_STATUS_NONE;
BYTE ucMHLCableDetect = 0;
BYTE ucECbusMuxSelect = 0;
WORD usECbusMuxSwitchTimer = 0;

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_EnableCbusInterrupt()
//  [Description]
//                  
//  [Arguments]:
//                  
//  [Return]:
//
//**************************************************************************
void _mhal_mhl_EnableCbusInterrupt(Bool bEnableInterrupt)
{
    msWriteByteMask(REG_002B29, bEnableInterrupt? 0: BIT4, BIT4); // REG_IRQ_BANK_14[12]
}

#if(CHIP_ID == MST9U3)
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
        case MHL_INPUT_SELECT_PORT0:
            
            break;

        case MHL_INPUT_SELECT_PORT1:
            
            break;

        case MHL_INPUT_SELECT_PORT2:
            ucECbusMux = MHL_ECBUS_MUX_SELECT_PORT2;
            break;

        case MHL_INPUT_SELECT_PORT3:
            ucECbusMux = MHL_ECBUS_MUX_SELECT_PORT3;
            break;

        default:

            break;
    };

    return ucECbusMux;
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
        case MHL_INPUT_SELECT_PORT0:
            msWrite2ByteMask(REG_000460, bEnableCbus? BIT3: 0, BIT3); // REG_PAD_TOP_30[3], Cbus port0 enable
            msWriteByteMask(REG_002F43, bEnableCbus? 0: BIT3, BIT3);//REG_PM_MHL_CBUS0_21[11], GPIO Enable port0
            break;

        case MHL_INPUT_SELECT_PORT1:
            msWrite2ByteMask(REG_000460, bEnableCbus? BIT4: 0, BIT4); // REG_PAD_TOP_30[4], Cbus port1 enable
            msWriteByteMask(REG_003143, bEnableCbus? 0: BIT3, BIT3);//REG_PM_MHL_CBUS1_21[11], GPIO Enable port1
            break;

        case MHL_INPUT_SELECT_PORT2:
            msWrite2ByteMask(REG_000460, bEnableCbus? BIT1: 0, BIT1); // REG_PAD_TOP_30[1], Cbus port2 enable
            msWriteByteMask(REG_003343, bEnableCbus? 0: BIT3, BIT3);//REG_PM_MHL_CBUS2_21[11], GPIO Enable port2
            break;

        case MHL_INPUT_SELECT_PORT3:
            msWrite2ByteMask(REG_000460, bEnableCbus? BIT2: 0, BIT2); // REG_PAD_TOP_30[2], Cbus port3 enable
            msWriteByteMask(REG_003543, bEnableCbus? 0: BIT3, BIT3);//REG_PM_MHL_CBUS3_21[11], GPIO Enable port3
            break;

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
    switch(ucPortSelect)
    {
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
        case MHL_INPUT_SELECT_PORT0:
            //msWrite2ByteMask(REG_11210A, bFlag? 0: BIT3, BIT3); // REG_COMBO_PHY1_05[3]: 
            break;

        case MHL_INPUT_SELECT_PORT1:
            //msWrite2ByteMask(REG_11230A, bFlag? 0: BIT3, BIT3); // REG_COMBO_PHY3_05[3]: 
            break;

        case MHL_INPUT_SELECT_PORT2:
        case MHL_INPUT_SELECT_PORT3:
            //msWrite2ByteMask(REG_160C4E, bFlag? 0: BIT0, BIT0); // REG_DVI_DTOP_DUAL_27[0], port 2-3 dual mhl enable ; [1]:packed-pixel mode enable 
            break;

        default:

            break;
    };

    bFlag = FALSE;
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
            msWriteByteMask(REG_000468, bFlag? BIT0: 0, BIT3); // REG_PAD_TOP_34[3], Enable GPIOX02 pull-up 1k
            break;

        case MHL_INPUT_SELECT_PORT1:
            msWriteByteMask(REG_000468, bFlag? BIT1: 0, BIT4); // REG_PAD_TOP_34[4], Enable GPIOX02 pull-up 1k
            break;

        case MHL_INPUT_SELECT_PORT2:
            msWriteByteMask(REG_000468, bFlag? BIT2: 0, BIT1); // REG_PAD_TOP_34[1], Enable GPIOX02 pull-up 1k
            break;

        case MHL_INPUT_SELECT_PORT3:
            msWriteByteMask(REG_000468, bFlag? BIT3: 0, BIT2); // REG_PAD_TOP_34[2], Enable GPIOX02 pull-up 1k
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
}

#else
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

        case MHL_INPUT_SELECT_PORT2:
            if(bFlag)
            {
                msWriteByteMask(REG_003301, MASKBIT(1:0), MASKBIT(1:0)); // REG_PM_MHL_CBUS2_00[9:8], SW control cable detect
            }
            else
            {
                msWriteByteMask(REG_003301, BIT1, MASKBIT(1:0)); // REG_PM_MHL_CBUS2_00[9:8], SW control cable detect
            }
            
            break;

        case MHL_INPUT_SELECT_PORT3:
            if(bFlag)
            {
                msWriteByteMask(REG_003501, MASKBIT(1:0), MASKBIT(1:0)); // REG_PM_MHL_CBUS3_00[9:8], SW control cable detect
            }
            else
            {
                msWriteByteMask(REG_003501, BIT1, MASKBIT(1:0)); // REG_PM_MHL_CBUS3_00[9:8], SW control cable detect
            }
            
            break;

        case MHL_INPUT_SELECT_PORT4:
            if(bFlag)
            {
                msWriteByteMask(REG_003701, MASKBIT(1:0), MASKBIT(1:0)); // REG_PM_MHL_CBUS4_00[9:8], SW control cable detect
            }
            else
            {
                msWriteByteMask(REG_003701, BIT1, MASKBIT(1:0)); // REG_PM_MHL_CBUS4_00[9:8], SW control cable detect
            }
            
            break;

        case MHL_INPUT_SELECT_PORT5:
            if(bFlag)
            {
                msWriteByteMask(REG_003901, MASKBIT(1:0), MASKBIT(1:0)); // REG_PM_MHL_CBUS5_00[9:8], SW control cable detect
            }
            else
            {
                msWriteByteMask(REG_003901, BIT1, MASKBIT(1:0)); // REG_PM_MHL_CBUS5_00[9:8], SW control cable detect
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
        case MHL_INPUT_SELECT_PORT0:
            msWrite2ByteMask(REG_000460, bEnableCbus? BIT0: 0, BIT0); // REG_PAD_TOP_30[0], Cbus port0 enable
            break;

        case MHL_INPUT_SELECT_PORT1:
            msWrite2ByteMask(REG_000460, bEnableCbus? BIT1: 0, BIT1); // REG_PAD_TOP_30[1], Cbus port1 enable
            break;

        case MHL_INPUT_SELECT_PORT2:
            msWrite2ByteMask(REG_000460, bEnableCbus? BIT2: 0, BIT2); // REG_PAD_TOP_30[2], Cbus port2 enable
            break;

        case MHL_INPUT_SELECT_PORT3:
            msWrite2ByteMask(REG_000460, bEnableCbus? BIT3: 0, BIT3); // REG_PAD_TOP_30[3], Cbus port3 enable
            break;

        case MHL_INPUT_SELECT_PORT4:
            msWrite2ByteMask(REG_000460, bEnableCbus? BIT4: 0, BIT4); // REG_PAD_TOP_30[4], Cbus port4 enable
            break;

        case MHL_INPUT_SELECT_PORT5:
            msWrite2ByteMask(REG_000460, bEnableCbus? BIT5: 0, BIT5); // REG_PAD_TOP_30[5], Cbus port5 enable
            break;

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
    switch(ucPortSelect)
    {
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

        case MHL_INPUT_SELECT_PORT4:
            if(bFlag)
            {
                SET_MHL_VBUS_CHARGE_PORT4();
            }
            else
            {
                CLR_MHL_VBUS_CHARGE_PORT4();
            }
            
            break;

        case MHL_INPUT_SELECT_PORT5:
            if(bFlag)
            {
                SET_MHL_VBUS_CHARGE_PORT5();
            }
            else
            {
                CLR_MHL_VBUS_CHARGE_PORT5();
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
        case MHL_INPUT_SELECT_PORT0:
            msWrite2ByteMask(REG_0017A4, bFlag? 0: BIT8, BIT8); // REG_PM_BANK_52[8], overwrite port0 enable mhl mode in sw mode
            break;

        case MHL_INPUT_SELECT_PORT1:
            msWrite2ByteMask(REG_0017A4, bFlag? 0: BIT9, BIT9); // REG_PM_BANK_52[9], overwrite port1 enable mhl mode in sw mode
            break;

        case MHL_INPUT_SELECT_PORT2:
            msWrite2ByteMask(REG_0017A4, bFlag? 0: BIT10, BIT10); // REG_PM_BANK_52[10], overwrite port2 enable mhl mode in sw mode
            break;

        case MHL_INPUT_SELECT_PORT3:
            msWrite2ByteMask(REG_0017A4, bFlag? 0: BIT11, BIT11); // REG_PM_BANK_52[11], overwrite port3 enable mhl mode in sw mode
            break;

        case MHL_INPUT_SELECT_PORT4:
            msWrite2ByteMask(REG_0017A4, bFlag? 0: BIT12, BIT12); // REG_PM_BANK_52[12], overwrite port4 enable mhl mode in sw mode
            break;

        case MHL_INPUT_SELECT_PORT5:
            msWrite2ByteMask(REG_0017A4, bFlag? 0: BIT13, BIT13); // REG_PM_BANK_52[13], overwrite port5 enable mhl mode in sw mode
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
            msWriteByteMask(REG_000468, bFlag? BIT0: 0, BIT0); // REG_PAD_TOP_34[0], Enable GPIOX02 pull-up 1k
            break;

        case MHL_INPUT_SELECT_PORT1:
            msWriteByteMask(REG_000468, bFlag? BIT1: 0, BIT1); // REG_PAD_TOP_34[1], Enable GPIOX02 pull-up 1k
            break;

        case MHL_INPUT_SELECT_PORT2:
            msWriteByteMask(REG_000468, bFlag? BIT2: 0, BIT2); // REG_PAD_TOP_34[2], Enable GPIOX02 pull-up 1k
            break;

        case MHL_INPUT_SELECT_PORT3:
            msWriteByteMask(REG_000468, bFlag? BIT3: 0, BIT3); // REG_PAD_TOP_34[3], Enable GPIOX02 pull-up 1k
            break;

        case MHL_INPUT_SELECT_PORT4:
            msWriteByteMask(REG_000468, bFlag? BIT4: 0, BIT4); // REG_PAD_TOP_34[4], Enable GPIOX02 pull-up 1k
            break;

        case MHL_INPUT_SELECT_PORT5:
            msWriteByteMask(REG_000468, bFlag? BIT5: 0, BIT5); // REG_PAD_TOP_34[5], Enable GPIOX02 pull-up 1k
            break;
            
        default:

            break;
    };
}

#endif

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_TimerHandler()
//  [Description]
//                  
//  [Arguments]:
//                  
//  [Return]:
//
//**************************************************************************
void mhal_mhl_TimerHandler(void)
{
    if(GET_MHL_MUX_SWITCH_TIMER() > 0)
    {
        DEC_MHL_MUX_SWITCH_TIMER();
    }
}

#if(CHIP_ID == MST9U3)
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
        case MHL_INPUT_SELECT_PORT0:
  #if(CABLE_DET_DVI0_SAR)
            if(MHL_CABLE_SAR_DETECT_PORT0 > MHL_CABLE_DETECT_LEVEL)
            {
                bCableDetect = TRUE;
            }
  #endif
  
            break;

        case MHL_INPUT_SELECT_PORT1:
   #if(CABLE_DET_DVI1_SAR)
            if(MHL_CABLE_SAR_DETECT_PORT1 > MHL_CABLE_DETECT_LEVEL)
            {
                bCableDetect = TRUE;
            }
    #endif
    
            break;

        case MHL_INPUT_SELECT_PORT2:
    #if(CABLE_DET_DVI2_SAR_EN)        
            if(MHL_CABLE_SAR_DETECT_PORT2 > MHL_CABLE_DETECT_LEVEL)
            {
                bCableDetect = TRUE;
            }
    #endif
            
            break;

        case MHL_INPUT_SELECT_PORT3:
    #if(CABLE_DET_DVI3_SAR_EN)
            if(MHL_CABLE_SAR_DETECT_PORT3 > MHL_CABLE_DETECT_LEVEL)
            {
                bCableDetect = TRUE;
            }
    #endif
            
            break;

        default:

            break;
    };

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
        case MHL_INPUT_SELECT_PORT0:
            ucCbusStatus = msReadByte(REG_002F2E); // REG_PM_MHL_CBUS0_17[1:0], link layer status
            break;

        case MHL_INPUT_SELECT_PORT1:
            ucCbusStatus = msReadByte(REG_00312E); // REG_PM_MHL_CBUS1_17[1:0], link layer status
            break;

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
        case MHL_INPUT_SELECT_PORT0:
            if(ucControl == RX_HDMI_RTERM)
            {
                //msWriteByteMask(REG_112107, 0, MASKBIT(2:0)); // REG_COMBO_PHY1_03[10:8], Power on port0 rterm.
            }
            else if(ucControl == RX_MHL_RTERM)
            {
                //msWriteByteMask(REG_112107, 0, BIT0); // REG_COMBO_PHY1_03[10:8], Power on port0's ch0 rterm.
            }
            else if(ucControl == RX_RTERM_OFF)
            {
                //msWriteByteMask(REG_112107, BIT0, BIT0); // REG_COMBO_PHY1_03[10:8], Power down port0 rterm.
            }
            
            break;

        case MHL_INPUT_SELECT_PORT1:
            if(ucControl == RX_HDMI_RTERM)
            {
                //msWriteByteMask(REG_112307, 0, MASKBIT(2:0)); // REG_COMBO_PHY3_03[10:8], Power on port1 rterm.
            }
            else if(ucControl == RX_MHL_RTERM)
            {
                //msWriteByteMask(REG_112307, 0, BIT0); // REG_COMBO_PHY3_03[10:8], Power on port1's ch0 rterm.
            }
            else if(ucControl == RX_RTERM_OFF)
            {
                //msWriteByteMask(REG_112307, BIT0, BIT0); // REG_COMBO_PHY3_03[10:8], Power down port1 rterm.
            }
            
            break;

        case MHL_INPUT_SELECT_PORT2:
            if(ucControl == RX_HDMI_RTERM)
            {
                msWriteByteMask(REG_001720, 0, MASKBIT(3:0)); // REG_PM_BANK_10[3:0], Power on port2 rterm.
            }
            else if(ucControl == RX_MHL_RTERM)
            {
                //msWriteByteMask(REG_001720, 0, MASKBIT(2:1)); // REG_PM_BANK_10[3:0], Power on port2's ch0 rterm.
                msWriteByteMask(REG_001720, 0, BIT0); // REG_PM_BANK_10[3:0], Power on port2's ch0 rterm. 
            }
            else if(ucControl == RX_RTERM_OFF)
            {
                //msWriteByteMask(REG_001720, MASKBIT(2:1), MASKBIT(2:1)); // REG_PM_BANK_10[3:0], Power down port2 rterm.
                msWriteByteMask(REG_001720, BIT0, BIT0); // REG_PM_BANK_10[3:0], Power down port2 rterm. 
            }
            
            break;

        case MHL_INPUT_SELECT_PORT3:
            if(ucControl == RX_HDMI_RTERM)
            {
                msWriteByteMask(REG_001722, 0, MASKBIT(3:0)); // REG_PM_BANK_11[3:0], Power on port3 rterm.
            }
            else if(ucControl == RX_MHL_RTERM)
            {
                //msWriteByteMask(REG_001722, 0, MASKBIT(2:1)); // REG_PM_BANK_11[3:0], Power on port3's ch0 rterm.
                msWriteByteMask(REG_001722, 0, BIT0); // REG_PM_BANK_11[3:0], Power on port3's ch0 rterm. 
            }
            else if(ucControl == RX_RTERM_OFF)
            {
                //msWriteByteMask(REG_001722, MASKBIT(2:1), MASKBIT(2:1)); // REG_PM_BANK_11[3:0], Power down port3 rterm.
                msWriteByteMask(REG_001722, BIT0, BIT0); // REG_PM_BANK_11[3:0], Power on port3's ch0 rterm. 
            }
            
            break;

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
        case MHL_INPUT_SELECT_PORT0:
            msWriteByteMask(REG_001707, bRtermHW? BIT4: 0, BIT4); // REG_PM_BANK_03[12], Cbus control port0 rterm.
            break;

        case MHL_INPUT_SELECT_PORT1:
            msWriteByteMask(REG_001707, bRtermHW? BIT5: 0, BIT5); // REG_PM_BANK_03[13], Cbus control port1 rterm.
            break;

        case MHL_INPUT_SELECT_PORT2:
            msWriteByteMask(REG_001707, bRtermHW? BIT6: 0, BIT6); // REG_PM_BANK_03[14], Cbus control port2 rterm.
            break;

        case MHL_INPUT_SELECT_PORT3:
            msWriteByteMask(REG_001707, bRtermHW? BIT7: 0, BIT7); // REG_PM_BANK_03[15], Cbus control port3 rterm.
            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_SetPowerStatus()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_mhl_SetPowerStatus(BYTE ucPowerStatus)
{
    BYTE ucECbusMuxValue = msReadByte(REG_00170A) &MASKBIT(2:0); // REG_PM_BANK_05[2:0], eCbus mux
    
    ucMHLPowerStatus = ucPowerStatus;

    switch(ucPowerStatus)
    {
        case MHL_POWER_STATUS_SAVING:
            _mhal_mhl_EnableCbusInterrupt(FALSE);
            CLR_MHL_CABLE_DETECT_PORT_ALL();
            break;
            
        case MHL_POWER_STATUS_DOWN:
            _mhal_mhl_EnableCbusInterrupt(FALSE);
            CLR_MHL_CABLE_DETECT_PORT_ALL();
            break;
            
        default:
            
            break;
    };

    if(ucECbusMuxValue == 2)
    {
        SET_MHL_ECBUS_MUX_SELECT(MHL_ECBUS_MUX_SELECT_PORT2);
    }
    else if(ucECbusMuxValue == 3)
    {
        SET_MHL_ECBUS_MUX_SELECT(MHL_ECBUS_MUX_SELECT_PORT3);
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
    BYTE ucECbusMux = _mhal_mhl_GetECbusMux(ucPortSelect);

//printData("mhal_mhl_PMStatePolling==%d",ucPortSelect);

//printData("mhal_mhl_CableDetect==%d",bCableDetect);
//printData("GET_MHL_CABLE_DETECT_PORT(ucPortSelect)==%d",GET_MHL_CABLE_DETECT_PORT(ucPortSelect));

    if(GET_MHL_CABLE_DETECT_PORT(ucPortSelect) != bCableDetect)
    {
        if(ucMHLPowerStatus == MHL_POWER_STATUS_SAVING)
        {
            // Cbus enable
            _mhal_mhl_CbusPadEnable(ucPortSelect, bCableDetect);
        }
        else
        {
            if(ucMHLPowerStatus == MHL_POWER_STATUS_DOWN)
             {
                    // Cbus disable
                    _mhal_mhl_CbusPadEnable(ucPortSelect, FALSE);
              }

            // Remove Pad PU1K
            _mhal_mhl_SetPadPullUp1K(ucPortSelect, !bCableDetect);
        }

        // Remove pull up 1K
        _mhal_mhl_SetCbusPullUp1K(ucPortSelect, !bCableDetect);
        // Vbus charge
        _mhal_mhl_SetVbusCharge(ucPortSelect, bCableDetect);
        // Rterm HW control
        mhal_mhl_RtermControlHWMode(ucPortSelect, bCableDetect);

        if(bCableDetect)
        {
            SET_MHL_CABLE_DETECT_PORT(ucPortSelect);
        }
        else
        {
            CLR_MHL_CABLE_DETECT_PORT(ucPortSelect);
        }

        if(ucECbusMux == MHL_ECBUS_MUX_SELECT_NONE) // Not ECbus port
        {
            if(ucMHLPowerStatus != MHL_POWER_STATUS_DOWN)
            {
                // Set cable detect
                _mhal_mhl_SetCableDetect(ucPortSelect, bCableDetect);
                // Open common mode resistor
                _mhal_mhl_OpenCommonModeResistor(ucPortSelect, bCableDetect);
             }
        }
        else // ECbus port
        {
            if(!bCableDetect && (ucECbusMux == GET_MHL_ECBUS_MUX_SELECT()))
            {
                CLR_MHL_MUX_SWITCH_TIMER();
            }
            
            if(GET_MHL_CABLE_DETECT_PORT(MHL_INPUT_SELECT_PORT2) || GET_MHL_CABLE_DETECT_PORT(MHL_INPUT_SELECT_PORT3))
            {
                bCableDetect = TRUE;
            }
            else
            {
                bCableDetect = FALSE;
            }

            // Set cable detect
            _mhal_mhl_SetCableDetect(ucPortSelect, bCableDetect);
            // Open common mode resistor
            _mhal_mhl_OpenCommonModeResistor(ucPortSelect, bCableDetect);
        }
    }

    if((GET_MHL_MUX_SWITCH_TIMER() == 0) && (ucECbusMux != MHL_ECBUS_MUX_SELECT_NONE)) // Timer out & ECbus port
    {
        if(GET_MHL_CABLE_DETECT_PORT(MHL_INPUT_SELECT_PORT2) || GET_MHL_CABLE_DETECT_PORT(MHL_INPUT_SELECT_PORT3)) // Check port2 &3 cable detect
        {
            if(GET_MHL_ECBUS_MUX_SELECT() == MHL_ECBUS_MUX_SELECT_NONE) // ECbus mux none
            {
                // Do nothing, keep current port ECbus mux
            }
            else if((GET_MHL_ECBUS_MUX_SELECT() == MHL_ECBUS_MUX_SELECT_PORT2) && GET_MHL_CABLE_DETECT_PORT(MHL_INPUT_SELECT_PORT3))
            {
                ucECbusMux = MHL_ECBUS_MUX_SELECT_PORT3;
            }
            else if((GET_MHL_ECBUS_MUX_SELECT() == MHL_ECBUS_MUX_SELECT_PORT3) && GET_MHL_CABLE_DETECT_PORT(MHL_INPUT_SELECT_PORT2))
            {
                ucECbusMux = MHL_ECBUS_MUX_SELECT_PORT2;
            }
        }
        else
        {
            ucECbusMux = MHL_ECBUS_MUX_SELECT_NONE;
        }

        if(ucECbusMux != GET_MHL_ECBUS_MUX_SELECT())
        {
             if (ucMHLPowerStatus == MHL_POWER_STATUS_SAVING)
                _mhal_mhl_ECbusMuxSwitch(ucECbusMux);
             else if (ucMHLPowerStatus == MHL_POWER_STATUS_DOWN)
                _mhal_mhl_ECbusMuxSwitch(MHL_ECBUS_MUX_SELECT_NONE);
             
            // Set cable detect
            _mhal_mhl_SetCableDetect(ucPortSelect, TRUE);
            
            MHL_HAL_DPRINTF("MHL switch MUX to %d ", ucECbusMux);
        }

        if(ucECbusMux != MHL_ECBUS_MUX_SELECT_NONE)
        {
            SET_MHL_MUX_SWITCH_TIMER(MHL_ECBUS_MUX_SWITCH_TIME);
        }
    }
}

#else
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
        case MHL_INPUT_SELECT_PORT0:
#if(CABLE_DET_DVI0_SAR)
            if(MHL_CABLE_SAR_DETECT_PORT0 > MHL_CABLE_DETECT_LEVEL)
            {
                bCableDetect = TRUE;
            }
#endif
            break;

        case MHL_INPUT_SELECT_PORT1:
 #if(CABLE_DET_DVI1_SAR)
            if(MHL_CABLE_SAR_DETECT_PORT1 > MHL_CABLE_DETECT_LEVEL)
            {
                bCableDetect = TRUE;
            }
 #endif           
            break;

        case MHL_INPUT_SELECT_PORT2:
 #if(CABLE_DET_DVI2_SAR)           
            if(MHL_CABLE_SAR_DETECT_PORT2 > MHL_CABLE_DETECT_LEVEL)
            {
                bCableDetect = TRUE;
            }
  #endif          
  
            break;

        case MHL_INPUT_SELECT_PORT3:
  #if(CABLE_DET_DVI3_SAR)          
            if(MHL_CABLE_SAR_DETECT_PORT3 > MHL_CABLE_DETECT_LEVEL)
            {
                bCableDetect = TRUE;
            }
  #endif           
  
            break;

        case MHL_INPUT_SELECT_PORT4:
  #if(CABLE_DET_DVI4_SAR)          
            if(MHL_CABLE_SAR_DETECT_PORT4 > MHL_CABLE_DETECT_LEVEL)
            {
                bCableDetect = TRUE;
            }
  #endif
          
            break;
  
        case MHL_INPUT_SELECT_PORT5:
  #if(CABLE_DET_DVI5_SAR)
            if(MHL_CABLE_SAR_DETECT_PORT5 > MHL_CABLE_DETECT_LEVEL)
            {
                bCableDetect = TRUE;
            }
    #endif
          
            break;
  
        default:

            break;
    };

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
        case MHL_INPUT_SELECT_PORT0:
            ucCbusStatus = msReadByte(REG_002F2E); // REG_PM_MHL_CBUS0_17[1:0], link layer status
            break;

        case MHL_INPUT_SELECT_PORT1:
            ucCbusStatus = msReadByte(REG_00312E); // REG_PM_MHL_CBUS1_17[1:0], link layer status
            break;

        case MHL_INPUT_SELECT_PORT2:
            ucCbusStatus = msReadByte(REG_00332E); // REG_PM_MHL_CBUS2_17[1:0], link layer status
            break;

        case MHL_INPUT_SELECT_PORT3:
            ucCbusStatus = msReadByte(REG_00352E); // REG_PM_MHL_CBUS3_17[1:0], link layer status
            break;

        case MHL_INPUT_SELECT_PORT4:
            ucCbusStatus = msReadByte(REG_00372E); // REG_PM_MHL_CBUS4_17[1:0], link layer status
            break;

        case MHL_INPUT_SELECT_PORT5:
            ucCbusStatus = msReadByte(REG_00392E); // REG_PM_MHL_CBUS5_17[1:0], link layer status
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
        case MHL_INPUT_SELECT_PORT0:
            if(ucControl == RX_HDMI_RTERM)
            {
                msWriteByteMask(REG_0017C4, 0, MASKBIT(3:0)); // REG_PM_BANK_62[3:0], Power on port0 rterm.
            }
            else if(ucControl == RX_MHL_RTERM)
            {
                msWriteByteMask(REG_0017C4, 0, MASKBIT(2:1)); // REG_PM_BANK_62[3:0], Power on port0's ch0 rterm.
            }
            else if(ucControl == RX_RTERM_OFF)
            {
                msWriteByteMask(REG_0017C4, MASKBIT(2:1), MASKBIT(2:1)); // REG_PM_BANK_62[3:0], Power down port0 rterm.
            }
            
            break;

        case MHL_INPUT_SELECT_PORT1:
            if(ucControl == RX_HDMI_RTERM)
            {
                msWriteByteMask(REG_0017C4, 0, MASKBIT(7:4)); // REG_PM_BANK_62[7:4], Power on port1 rterm.
            }
            else if(ucControl == RX_MHL_RTERM)
            {
                msWriteByteMask(REG_0017C4, 0, MASKBIT(6:5)); // REG_PM_BANK_62[7:4], Power on port1's ch0 rterm.
            }
            else if(ucControl == RX_RTERM_OFF)
            {
                msWriteByteMask(REG_0017C4, MASKBIT(6:5), MASKBIT(6:5)); // REG_PM_BANK_62[7:4], Power down port1 rterm.
            }
            
            break;

        case MHL_INPUT_SELECT_PORT2:
            if(ucControl == RX_HDMI_RTERM)
            {
                msWriteByteMask(REG_0017C5, 0, MASKBIT(3:0)); // REG_PM_BANK_62[11:8], Power on port2 rterm.
            }
            else if(ucControl == RX_MHL_RTERM)
            {
                msWriteByteMask(REG_0017C5, 0, MASKBIT(2:1)); // REG_PM_BANK_62[11:8], Power on port2's ch0 rterm.
            }
            else if(ucControl == RX_RTERM_OFF)
            {
                msWriteByteMask(REG_0017C5, MASKBIT(2:1), MASKBIT(2:1)); // REG_PM_BANK_62[11:8], Power down port2 rterm.
            }
            
            break;

        case MHL_INPUT_SELECT_PORT3:
            if(ucControl == RX_HDMI_RTERM)
            {
                msWriteByteMask(REG_0017A2, 0, MASKBIT(3:0)); // REG_PM_BANK_51[3:0], Power on port3 rterm.
            }
            else if(ucControl == RX_MHL_RTERM)
            {
                msWriteByteMask(REG_0017A2, 0, MASKBIT(2:1)); // REG_PM_BANK_51[3:0], Power on port3's ch0 rterm.
            }
            else if(ucControl == RX_RTERM_OFF)
            {
                msWriteByteMask(REG_0017A2, MASKBIT(2:1), MASKBIT(2:1)); // REG_PM_BANK_51[3:0], Power down port3 rterm.
            }
            
            break;

        case MHL_INPUT_SELECT_PORT4:
            if(ucControl == RX_HDMI_RTERM)
            {
                msWriteByteMask(REG_0017A2, 0, MASKBIT(7:4)); // REG_PM_BANK_51[7:4], Power on port4 rterm.
            }
            else if(ucControl == RX_MHL_RTERM)
            {
                msWriteByteMask(REG_0017A2, 0, MASKBIT(6:5)); // REG_PM_BANK_51[7:4], Power on port4's ch0 rterm.
            }
            else if(ucControl == RX_RTERM_OFF)
            {
                msWriteByteMask(REG_0017A2, MASKBIT(6:5), MASKBIT(6:5)); // REG_PM_BANK_51[7:4], Power down port4 rterm.
            }
            
            break;

        case MHL_INPUT_SELECT_PORT5:
            if(ucControl == RX_HDMI_RTERM)
            {
                msWriteByteMask(REG_0017A3, 0, MASKBIT(3:0)); // REG_PM_BANK_51[11:8], Power on port5 rterm.
            }
            else if(ucControl == RX_MHL_RTERM)
            {
                msWriteByteMask(REG_0017A3, 0, MASKBIT(2:1)); // REG_PM_BANK_51[11:8], Power on port5's ch0 rterm.
            }
            else if(ucControl == RX_RTERM_OFF)
            {
                msWriteByteMask(REG_0017A3, MASKBIT(2:1), MASKBIT(2:1)); // REG_PM_BANK_51[11:8], Power down port5 rterm.
            }
            
            break;

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
        case MHL_INPUT_SELECT_PORT0:
            msWriteByteMask(REG_0017C9, bRtermHW? BIT4: 0, BIT4); // REG_PM_BANK_64[12], Cbus control port0 rterm.
            break;

        case MHL_INPUT_SELECT_PORT1:
            msWriteByteMask(REG_0017C9, bRtermHW? BIT5: 0, BIT5); // REG_PM_BANK_64[13], Cbus control port1 rterm.
            break;

        case MHL_INPUT_SELECT_PORT2:
            msWriteByteMask(REG_0017C9, bRtermHW? BIT6: 0, BIT6); // REG_PM_BANK_64[14], Cbus control port2 rterm.
            break;

        case MHL_INPUT_SELECT_PORT3:
            msWriteByteMask(REG_0017A0, bRtermHW? BIT0: 0, BIT0); // REG_PM_BANK_50[0], Cbus control port3 rterm.
            break;

        case MHL_INPUT_SELECT_PORT4:
            msWriteByteMask(REG_0017A0, bRtermHW? BIT1: 0, BIT1); // REG_PM_BANK_50[1], Cbus control port4 rterm.
            break;

        case MHL_INPUT_SELECT_PORT5:
            msWriteByteMask(REG_0017A0, bRtermHW? BIT2: 0, BIT2); // REG_PM_BANK_50[2], Cbus control port5 rterm.
            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_SetPowerStatus()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_mhl_SetPowerStatus(BYTE ucPowerStatus)
{
    ucMHLPowerStatus = ucPowerStatus;

    switch(ucPowerStatus)
    {
        case MHL_POWER_STATUS_SAVING:
            _mhal_mhl_EnableCbusInterrupt(FALSE);
            CLR_MHL_CABLE_DETECT_PORT_ALL();
            break;
            
        case MHL_POWER_STATUS_DOWN:
            _mhal_mhl_EnableCbusInterrupt(FALSE);
            CLR_MHL_CABLE_DETECT_PORT_ALL();
            break;
            
        default:
            
            break;
    };
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

    if(GET_MHL_CABLE_DETECT_PORT(ucPortSelect) != bCableDetect)
    {
        if(ucMHLPowerStatus == MHL_POWER_STATUS_SAVING)
        {
            // Cbus enable
            _mhal_mhl_CbusPadEnable(ucPortSelect, bCableDetect);
        }
        else
        {
            if(ucMHLPowerStatus == MHL_POWER_STATUS_DOWN)
             {
                    // Cbus disable
                    _mhal_mhl_CbusPadEnable(ucPortSelect, FALSE);
              }
        
            // Remove Pad PU1K
            _mhal_mhl_SetPadPullUp1K(ucPortSelect, !bCableDetect);
        }
        
        // Vbus charge
        _mhal_mhl_SetVbusCharge(ucPortSelect, bCableDetect);

        if(ucMHLPowerStatus != MHL_POWER_STATUS_DOWN)
        {        
            // Set cable detect
            _mhal_mhl_SetCableDetect(ucPortSelect, bCableDetect);
            // Open common mode resistor
            _mhal_mhl_OpenCommonModeResistor(ucPortSelect, bCableDetect);
         }
        
        // Rterm HW control
        mhal_mhl_RtermControlHWMode(ucPortSelect, bCableDetect);
        
        if(bCableDetect)
        {
            SET_MHL_CABLE_DETECT_PORT(ucPortSelect);
        }
        else
        {
            CLR_MHL_CABLE_DETECT_PORT(ucPortSelect);
        }
    }
}

#endif

#endif // _MHAL_MHL_C_


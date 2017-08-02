///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    mhal_DPRx.c
/// @author MStar Semiconductor Inc.
/// @brief  DP Rx driver Function
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MHAL_DPRX_C_
#define _MHAL_DPRX_C_

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "Global.h"
#include "mapi_DPRx.h"
#include "mdrv_DPRx.h"
#include "mhal_DPRx.h"
#include "drvDDC2Bi.h"

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define DP_HAL_DEBUG_MESSAGE           	0

#if(DP_DEBUG_MESSAGE && DP_HAL_DEBUG_MESSAGE)
#define DP_HAL_DPUTSTR(str)            	printMsg(str)
#define DP_HAL_DPRINTF(str, x)         	printData(str, x)
#else
#define DP_HAL_DPUTSTR(str)
#define DP_HAL_DPRINTF(str, x)
#endif

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
Bool bDPForceDownSpeed[DP_INPUT_SELECT_MASK];
BYTE ucDDCNullMessage[DP_AUX_COMMAND_DDC_LENGTH] = {0x6e, 0x80, 0xbe};
BYTE ucDDCBuffer[DP_AUX_COMMAND_MAX_LENGTH] = {0};
stDPAudioInfo stAudioInfo = {0, 0, 0, 0, 0};

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

//**************************************************************************
//  [Function Name]:
//                  _mhal_DP_ComboLinkRegisterOffset()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
WORD _mhal_DP_ComboLinkRegisterOffset(BYTE ucPortSelect)
{
    WORD usLinkRegOffset = ucPortSelect *0x600;

    if(ucPortSelect > 0)
    {
        usLinkRegOffset = usLinkRegOffset +(ucPortSelect -1) *0x200;
    }

    return usLinkRegOffset;
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_DP_GetPow2()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
BYTE _mhal_DP_GetPow2(BYTE ucExpValue)
{
    BYTE ucResult = 1;
    
    if(ucExpValue > 0)
    {
        ucResult = 0x02 <<(ucExpValue -1);
    }

    return ucResult;
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_DP_RtermEnable()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void _mhal_DP_RtermEnable(BYTE ucPortSelect)
{
    switch(ucPortSelect)
    {
        case DP_INPUT_SELECT_PORT0:
            msWriteByteMask(REG_0017C4, 0, MASKBIT(3:1)); //[3:1]: Power on P0 data r-term
            msWriteByteMask(REG_001781, 0, BIT0); //[0]: Power on P0 clock r-term
            break;

        case DP_INPUT_SELECT_PORT1:
            msWriteByteMask(REG_0017C4, 0, MASKBIT(7:5)); //[7:5]: Power on P1 data r-term
            msWriteByteMask(REG_001781, 0, BIT1); //[1]: Power on P1 clock r-term
            break;

        case DP_INPUT_SELECT_PORT2:
            msWriteByteMask(REG_0017C5, 0, MASKBIT(3:1)); //[3:1]: Power on P2 data r-term
            msWriteByteMask(REG_001781, 0, BIT2); //[2]: Power on P2 clock r-term
            break;

        case DP_INPUT_SELECT_PORT3:
            msWriteByteMask(REG_0017A2, 0, MASKBIT(3:1)); //[3:1]: Power on P3 data r-term
            msWriteByteMask(REG_001781, 0, BIT3); //[3]: Power on P3 clock r-term
            break;

        case DP_INPUT_SELECT_PORT4:
            msWriteByteMask(REG_0017A2, 0, MASKBIT(7:5)); //[7:5]: Power on P4 data r-term
            msWriteByteMask(REG_001781, 0, BIT4); //[4]: Power on P4 clock r-term
            break;

        case DP_INPUT_SELECT_PORT5:
            msWriteByteMask(REG_0017A3, 0, MASKBIT(3:1)); //[3:1]: Power on P5 data r-term
            msWriteByteMask(REG_001781, 0, BIT5); //[5]: Power on P5 clock r-term
            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_DP_AuxInitialSetting()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void _mhal_DP_AuxInitialSetting(BYTE ucPortSelect)
{
    msWriteByteMask(REG_0017B4, (BIT2 <<ucPortSelect), (BIT2 <<ucPortSelect));	//Common mode Voltage
    msWriteByteMask(REG_0017B9, (BIT0 <<ucPortSelect), (BIT0 <<ucPortSelect));
    
    
    
 #if(DP_SUPPORT_HBR2)
    msWriteByte(REG_001100 +DP_PHY_REG_OFFSET(ucPortSelect), 0x12); // DP12
    msWriteByte(REG_001101 +DP_PHY_REG_OFFSET(ucPortSelect), 0x14); //cyc : 5.4G
    msWriteByte(REG_001103 +DP_PHY_REG_OFFSET(ucPortSelect), 0x8B); // Support TP3// Support 1.62G
 #else
    msWriteByte(REG_001100 +DP_PHY_REG_OFFSET(ucPortSelect), 0x11); // DP11
    msWriteByte(REG_001101 +DP_PHY_REG_OFFSET(ucPortSelect), 0x0A);  //cyc : 2.7G
    msWriteByte(REG_001103 +DP_PHY_REG_OFFSET(ucPortSelect), 0x89); // Support TP3// Support 1.62G
 #endif
 
    msWriteByte(REG_001112 +DP_PHY_REG_OFFSET(ucPortSelect), 0x0E); // Aux UI Count
    msWriteByte(REG_001113 +DP_PHY_REG_OFFSET(ucPortSelect), 0x0B); // Aux Sample Rate
    msWriteByte(REG_001114 +DP_PHY_REG_OFFSET(ucPortSelect), 0x00); // Aux Time Out Low
    msWriteByte(REG_001115 +DP_PHY_REG_OFFSET(ucPortSelect), 0x08); // Aux Time Out  High
    msWriteByteMask(REG_001115 +DP_PHY_REG_OFFSET(ucPortSelect), 0, BIT5|BIT6); // Aux Clk Src
    msWriteByte(REG_00112C +DP_PHY_REG_OFFSET(ucPortSelect), 0x0E); // Aux UI Count
    msWriteByte(REG_00112D +DP_PHY_REG_OFFSET(ucPortSelect), 0x0B); // Aux Sample Rate
    msWriteByte(REG_00112E +DP_PHY_REG_OFFSET(ucPortSelect), 0x00); // Aux Time Out Low
    msWriteByte(REG_00112F +DP_PHY_REG_OFFSET(ucPortSelect), 0x08); // Aux Time Out  High
    msWriteByte(REG_001130 +DP_PHY_REG_OFFSET(ucPortSelect), 0x00); // Aux Reply BY HW
    msWriteByteMask(REG_001131 +DP_PHY_REG_OFFSET(ucPortSelect), 0, BIT0|BIT1); // Aux Time out Reply
    msWriteByte(REG_001132 +DP_PHY_REG_OFFSET(ucPortSelect), 0xD0); // Aux EDID address

    msWriteByteMask(REG_001142 +DP_PHY_REG_OFFSET(ucPortSelect), BIT0,BIT1); // GUID EN
    msWrite2Byte(REG_0011A8 +DP_PHY_REG_OFFSET(ucPortSelect), 0x1044 ); // ENABLE TX
    msWrite2Byte(REG_0011C2, 0x0000);	// Off-line EN = 0
    msWrite2Byte(REG_0011C0, 0x2020);	// SQ Debunce 
    msWriteByteMask(REG_001142 +DP_PHY_REG_OFFSET(ucPortSelect), BIT0, BIT0); // Enable GUID Read/Write
    msWriteByteMask(REG_00119C +DP_PHY_REG_OFFSET(ucPortSelect), BIT4|BIT7, BIT4|BIT5|BIT6|BIT7); // V oltage Swim and PRE-emph

    msWriteByteMask(REG_00119A +DP_PHY_REG_OFFSET(ucPortSelect), BIT1, BIT1); // MCCS reply By MCU

    msWriteByte(REG_001179 +DP_PHY_REG_OFFSET(ucPortSelect), 0xFF);	// Clear Interrupt States
    msWriteByte(REG_001179 +DP_PHY_REG_OFFSET(ucPortSelect), 0x00);

    msWriteByte(REG_001192 +DP_PHY_REG_OFFSET(ucPortSelect), 0x0F);	// Clear dpcd0_irq_mask Interrupt States
    msWriteByte(REG_001192 +DP_PHY_REG_OFFSET(ucPortSelect), 0x00);

    msWriteByteMask(REG_001193 +DP_PHY_REG_OFFSET(ucPortSelect), BIT7, BIT7); // Clear _aksv_irq_mask
    msWriteByteMask(REG_001193 +DP_PHY_REG_OFFSET(ucPortSelect), 0, BIT7);

    msWriteByteMask(REG_001196 +DP_PHY_REG_OFFSET(ucPortSelect), BIT2, BIT2); // Clear_SQ
    msWriteByteMask(REG_001196 +DP_PHY_REG_OFFSET(ucPortSelect), 0, BIT2);
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_DP_SetProgramDPCD()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_DP_SetProgramDPCD(BYTE ucPortSelect, BYTE ucDPCPSelect)
{
    switch(ucDPCPSelect)
    {
        case DP_PROGRAM_DPCD_0:
            msWriteByteMask(REG_00117D +DP_PHY_REG_OFFSET(ucPortSelect), BIT2, BIT2);
            msWrite2Byte(REG_001180 +DP_PHY_REG_OFFSET(ucPortSelect), 0x30);
            msWrite2Byte(REG_001182 +DP_PHY_REG_OFFSET(ucPortSelect), 0x00);
            break;

        case DP_PROGRAM_DPCD_1:
            msWriteByteMask(REG_00117D +DP_PHY_REG_OFFSET(ucPortSelect), BIT3, BIT3);
            msWrite2Byte(REG_001184 +DP_PHY_REG_OFFSET(ucPortSelect), 0x010A);
            msWrite2Byte(REG_001186 +DP_PHY_REG_OFFSET(ucPortSelect), 0x00);
            break;

        case DP_PROGRAM_DPCD_2:
            msWriteByteMask(REG_00117D +DP_PHY_REG_OFFSET(ucPortSelect), BIT4, BIT4);
            msWrite2Byte(REG_001188 +DP_PHY_REG_OFFSET(ucPortSelect), 0x30);
            msWrite2Byte(REG_00118A +DP_PHY_REG_OFFSET(ucPortSelect), 0x00);
            break;

        case DP_PROGRAM_DPCD_3:
            msWriteByteMask(REG_00117D +DP_PHY_REG_OFFSET(ucPortSelect), BIT5, BIT5);
            msWrite2Byte(REG_00118C +DP_PHY_REG_OFFSET(ucPortSelect), 0x30);
            msWrite2Byte(REG_00118E +DP_PHY_REG_OFFSET(ucPortSelect), 0x00);
            break;

        default:
            
           break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_DP_SetMCCSReceiveInterrupt()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_DP_SetMCCSReceiveInterrupt(BYTE ucPortSelect, Bool bEnable)
{
    msWriteByteMask(REG_00117B +DP_PHY_REG_OFFSET(ucPortSelect), bEnable? 0: BIT7, BIT7); // MCCS
    msWriteByteMask(REG_001179 +DP_PHY_REG_OFFSET(ucPortSelect), BIT7, BIT7);
    msWriteByteMask(REG_001179 +DP_PHY_REG_OFFSET(ucPortSelect), 0, BIT7);
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_DP_SetProgramDPCD0Interrupt()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_DP_SetProgramDPCD0Interrupt(BYTE ucPortSelect, Bool bEnable)
{
    msWriteByteMask(REG_001191 +DP_PHY_REG_OFFSET(ucPortSelect), bEnable? 0: BIT4, BIT4); // Program DPCD0
    msWriteByteMask(REG_001192 +DP_PHY_REG_OFFSET(ucPortSelect), BIT0, BIT0);
    msWriteByteMask(REG_001192 +DP_PHY_REG_OFFSET(ucPortSelect), 0, BIT0);
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_DP_SetProgramDPCD1Interrupt()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_DP_SetProgramDPCD1Interrupt(BYTE ucPortSelect, Bool bEnable)
{
    msWriteByteMask(REG_001191 +DP_PHY_REG_OFFSET(ucPortSelect), bEnable? 0: BIT5, BIT5); // Program DPCD1
    msWriteByteMask(REG_001192 +DP_PHY_REG_OFFSET(ucPortSelect), BIT1, BIT1);
    msWriteByteMask(REG_001192 +DP_PHY_REG_OFFSET(ucPortSelect), 0, BIT1);
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_DP_GetLinkRate()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
WORD _mhal_DP_GetLinkRate(BYTE ucPortSelect, BYTE ucHBR2Select)
{
    WORD usLinkRate = 0;
    
    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        ucPortSelect = 0;
    }
    else // HBR2 path
    {
        usLinkRate = 27 *msReadByte(REG_11330E +DP_HBR2_REG_OFFSET(ucHBR2Select));
    }

    return usLinkRate;
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_DP_UpdateAudioFrequence()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_DP_UpdateAudioFrequence(void)
{
    msWriteByteMask(REG_162F07, BIT5, BIT5); //update Freq
    msWriteByteMask(REG_162F07, 0, BIT5); //update Freq
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_DP_SetAUPLLBigChangeInterrupt()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_DP_SetAUPLLBigChangeInterrupt(BYTE ucPortSelect, BYTE ucHBR2Select, Bool bEnable)
{
    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        ucPortSelect = 0;
    }
    else // HBR2 path
    {
        msWriteByteMask(REG_1133E3 +DP_HBR2_REG_OFFSET(ucHBR2Select), bEnable? 0: BIT1, BIT1); //[1] AUPLL Big CHG
        msWriteByteMask(REG_1133EB +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT1, BIT1);
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_DP_SetInterlaneSkewInterrupt()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_DP_SetInterlaneSkewInterrupt(BYTE ucPortSelect, BYTE ucHBR2Select, Bool bEnable)
{
    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        ucPortSelect = 0;
    }
    else // HBR2 path
    {
        msWriteByteMask(REG_1133E2 +DP_HBR2_REG_OFFSET(ucHBR2Select), bEnable? 0: BIT3, BIT3); // [3] Interlaneskew
        msWriteByteMask(REG_1133EA +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT3, BIT3);
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_DP_SetCDRLossLockInterrupt()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_DP_SetCDRLossLockInterrupt(BYTE ucPortSelect, BYTE ucHBR2Select, Bool bEnable)
{
    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        ucPortSelect = 0;
    }
    else // HBR2 path
    {
        msWriteByteMask(REG_1133E1 +DP_HBR2_REG_OFFSET(ucHBR2Select), bEnable? 0: BIT0, BIT0); // [0] CDR loss
        msWriteByteMask(REG_1133E9 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT0, BIT0); 
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_DP_SetVPLLBigChangeInterrupt()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_DP_SetVPLLBigChangeInterrupt(BYTE ucPortSelect, BYTE ucHBR2Select, Bool bEnable)
{
    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        ucPortSelect = 0;
    }
    else // HBR2 path
    {
        msWriteByteMask(REG_1133E3 +DP_HBR2_REG_OFFSET(ucHBR2Select), bEnable? 0: BIT3, BIT3); // [3] VUPLL Big CHG
        msWriteByteMask(REG_1133EB +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT3, BIT3);
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_DP_SetTrainingPattern1Interrupt()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_DP_SetTrainingPattern1Interrupt(BYTE ucPortSelect, BYTE ucHBR2Select, Bool bEnable)
{
    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        ucPortSelect = 0;
    }
    else // HBR2 path
    {
        msWriteByteMask(REG_1133E0 +DP_HBR2_REG_OFFSET(ucHBR2Select), bEnable? 0: BIT0, BIT0); // [0] P1
        msWriteByteMask(REG_1133E8 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT0, BIT0); 
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_DP_SetTrainingPattern2Interrupt()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_DP_SetTrainingPattern2Interrupt(BYTE ucPortSelect, BYTE ucHBR2Select, Bool bEnable)
{
    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        ucPortSelect = 0;
    }
    else // HBR2 path
    {
        msWriteByteMask(REG_1133E0 +DP_HBR2_REG_OFFSET(ucHBR2Select), bEnable? 0: BIT1, BIT1); // [1] P2
        msWriteByteMask(REG_1133E8 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT1, BIT1); 
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_DP_ComboPHYSetting()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_DP_ComboPHYSetting(BYTE ucPortSelect)
{
    // Power control
    msWriteByteMask(REG_1120D7 +DP_PHY_REG_OFFSET(ucPortSelect), 0, BIT0); // Band Gap On
	msWriteByte(REG_112016 +DP_PHY_REG_OFFSET(ucPortSelect), 0xFF); // Clock EN
	msWriteByte(REG_112017 +DP_PHY_REG_OFFSET(ucPortSelect), 0xFF); // Clock EN
	msWrite2Byte(REG_112018 +DP_PHY_REG_OFFSET(ucPortSelect), 0x3FFF); // Clock EN
	msWriteByteMask(REG_1120B6 +DP_PHY_REG_OFFSET(ucPortSelect), 0x00, 0x0F); //Enable DP Lane power

    msWriteByteMask(REG_112000 +DP_PHY_REG_OFFSET(ucPortSelect), 0, BIT7); // Sleect DP port
    msWrite2Byte(REG_112080 +DP_PHY_REG_OFFSET(ucPortSelect), 0x168); // HBR2
    msWrite2Byte(REG_112084 +DP_PHY_REG_OFFSET(ucPortSelect), 0x2d0); // HBR
    msWrite2Byte(REG_112086 +DP_PHY_REG_OFFSET(ucPortSelect), 0x360); // RBR

    msWriteByteMask(REG_1120AF +DP_PHY_REG_OFFSET(ucPortSelect), BIT0| BIT1, BIT0| BIT1| BIT2); //Enable DP mode	In nonFullRate mode.
    msWriteByteMask(REG_1120D9 +DP_PHY_REG_OFFSET(ucPortSelect), BIT0, BIT0); //Enable DP mode
    msWriteByte(REG_1120A6 +DP_PHY_REG_OFFSET(ucPortSelect), 0x0F);

    msWriteByte(REG_112022 +DP_PHY_REG_OFFSET(ucPortSelect), 0x00);
    msWriteByte(REG_112023 +DP_PHY_REG_OFFSET(ucPortSelect), 0x00);
    msWriteByte(REG_112024 +DP_PHY_REG_OFFSET(ucPortSelect), 0x00);
    msWriteByte(REG_112025 +DP_PHY_REG_OFFSET(ucPortSelect), 0x00);
    msWriteByte(REG_112026 +DP_PHY_REG_OFFSET(ucPortSelect), 0x00);
    msWriteByte(REG_112027 +DP_PHY_REG_OFFSET(ucPortSelect), 0x00);

    msWriteByteMask(REG_1120B3 +DP_PHY_REG_OFFSET(ucPortSelect), BIT0, BIT0);
    msWriteByteMask(REG_1120B7 +DP_PHY_REG_OFFSET(ucPortSelect), 0, 0xF0);

    msWrite2Byte(REG_112076 +DP_PHY_REG_OFFSET(ucPortSelect), 0x1F00);

    msWriteByteMask(REG_112014 +DP_PHY_REG_OFFSET(ucPortSelect), BIT6, BIT6); // 20Bit Clk

    msWriteByteMask(REG_112090 +DP_PHY_REG_OFFSET(ucPortSelect), 0x05, 0xFF); //The CR lock reference correct threshold range for HBR/RBR

    // 3 stage
	msWriteByteMask(REG_1120BE +DP_PHY_REG_OFFSET(ucPortSelect), BIT6| BIT5, BIT6| BIT5); //Always turn on 3 stage.
	msWriteByteMask(REG_1120C0 +DP_PHY_REG_OFFSET(ucPortSelect), BIT6| BIT5, BIT6| BIT5);
	msWriteByteMask(REG_1120C2 +DP_PHY_REG_OFFSET(ucPortSelect), BIT6| BIT5, BIT6| BIT5);
	msWriteByteMask(REG_1120C4 +DP_PHY_REG_OFFSET(ucPortSelect), BIT6| BIT5, BIT6| BIT5);
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_DP_HBR2InitialSetting()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_DP_HBR2InitialSetting(BYTE ucHBR2Select)
{
    // ##############################################
    // ##      (0~1)                            BANK 1130                                     ##
    // ##############################################
    msWriteByteMask(REG_11307B +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT7, BIT7); // K28.5

    // ##############################################
    // ##      (0~1)                            BANK 1133                                     ##
    // ##############################################
    msWriteByteMask(REG_113389 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT7, BIT7); //Max SWIM EN
    msWriteByte(REG_113391 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0xAA); // Swim Level

    msWriteByte(REG_1133E0 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0xFF); // mask INT
    msWriteByte(REG_1133E1 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0xFF); // mask INT
    msWriteByte(REG_1133E2 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0xFF); // mask INT
    msWriteByte(REG_1133E3 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0xFF); // mask INT
    msWriteByte(REG_1133E8 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0xFF); // clear INT
    msWriteByte(REG_1133E9 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0xFF); // clear INT
    msWriteByte(REG_1133EA +DP_HBR2_REG_OFFSET(ucHBR2Select), 0xFF); // clear INT
    msWriteByte(REG_1133EB +DP_HBR2_REG_OFFSET(ucHBR2Select), 0xFF); // clear INT

    msWriteByte(REG_1133F0 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x00); // AUX TIME OUT REPLY ACK FOR CTS ISSUE
    //msWriteByte(REG_113300 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x12); // DPCD 1.2 Version
    msWriteByte(REG_113302 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x84); // DP Enhanced Frame capability & Max Lane Count
    //msWriteByte(REG_113301 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x14); // DP main link rate 5.4G
    msWriteByte(REG_113304 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x01); // DP Number of Receiver Ports
    msWriteByte(REG_11331C +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x80); // DP Linkstatus andadjust requestupdated
    msWriteByte(REG_11331E +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x00); // LANE0,1 Pre-emphasis & VoltageSwing
    msWriteByte(REG_11331F +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x00); // LANE2,3 Pre-emphasis & VoltageSwing
    msWriteByte(REG_1133D4 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x01); // RS_SWAP & DP receiever enable
    msWriteByte(REG_1133DC +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x80); // USE Lanes count mask lock status
    msWriteByte(REG_1133F1 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0xD0); // AUX length compare mask
    msWriteByte(REG_1133D2 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0xFF); // Video PLL freeze disable    Audio Enable Frezzen
    msWriteByte(REG_1133D3 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x72); // [3]:=> 1:Dither output 8 bits , 0: 10 bits
    msWriteByte(REG_1133DD +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x80); //  Add by CCJ
    msWriteByte(REG_1133D6 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x10); // Set 0x10
    msWriteByte(REG_1133D8 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x20); // FIFO Delay for Reduce Blanking issue
    msWriteByte(REG_1133A2 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x20); //MVID_Range[7:0]
    msWriteByte(REG_1133A3 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x00); //MVID_Range[15:8]
    msWriteByte(REG_1133A4 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x00); //MVID_Range[23:16]
    msWriteByte(REG_1133D1 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x30); // Bit5: Video M/N Bit4: Audio M/N
    msWriteByteMask(REG_1133C1 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT0, BIT0);
    msWriteByte(REG_1133C2 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x98); // BS-> RE_GEN HPWS delay for Songlu U02
    msWriteByte(REG_1133C6 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x04);
    msWriteByte(REG_1133CF +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x07);
    msWriteByteMask(REG_113399 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT7, BIT7); //vblank config enable
    msWriteByte(REG_113397 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x03); //  Glitch filter use old mode  (Warning: HLBR ISSUE)  No gltch
    msWriteByte(REG_113396 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0xFF); //  Glitch filter use old mode  (Warning: HLBR ISSUE)  No gltch
    msWriteByte(REG_1133CC +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x61); //(bit3=0 use VB_ID rising as frame flag (end of active))

    msWriteByte(REG_1133A2 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x20); //MVID_Range[7:0]
    msWriteByte(REG_1133A3 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x00); //MVID_Range[15:8]
    msWriteByte(REG_1133A4 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x00); //MVID_Range[23:16]

    msWriteByte(REG_1133AA +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x20); //NVID_Range[7:0]
    msWriteByte(REG_1133AB +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x00); //NVID_Range[15:8]
    msWriteByte(REG_1133AC +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x00); //NVID_Range[23:16]

    msWriteByte(REG_1133AE +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x10); //MAUD_CENTER[7:0]
    msWriteByte(REG_1133AF +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x00); //MAUD_CENTER[15:8]
    msWriteByte(REG_1133B0 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x00); //MAUD_CENTER[23:16]
    msWriteByte(REG_1133B2 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x10); //MAUD_Range[7:0]
    msWriteByte(REG_1133B3 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x00); //MAUD_Range[15:8]
    msWriteByte(REG_1133B4 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x00); //MAUD_Range[23:16]
    msWriteByte(REG_1133B6 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x00); //NAUD_CENTER[7:0]
    msWriteByte(REG_1133B7 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x10); //NAUD_CENTER[15:8]
    msWriteByte(REG_1133B8 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x00); //NAUD_CENTER[23:16]
    msWriteByte(REG_1133B9 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x10); //NAUD_Range[7:0]
    msWriteByte(REG_1133BA +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x00); //NAUD_Range[15:8]
    msWriteByte(REG_1133BB +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x00); //NAUD_Range[23:16]

    // ##############################################
    // ##                                        BANK 1134                                     ##
    // ##############################################
    msWriteByteMask(REG_113473 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0, BIT3); // Common Mode Aux
    msWriteByte(REG_1134E0 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0xCE); // clk enable  Bit0=0 EQ issue
    msWriteByte(REG_1134E1 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x6F); // Bit 5: For Squelch clock
    msWriteByte(REG_11346C +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x00); // DP PLL POWER DOWN
    msWriteByte(REG_11346E +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x01); // Training PFD Divide 4

    msWriteByte(REG_11346D +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x00); // DP_SEL_R50_VCM 3.3V & close powerdown
    msWriteByte(REG_11346F +DP_HBR2_REG_OFFSET(ucHBR2Select), 0xF4);
    msWriteByte(REG_113474 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x30); // EQ common mode, TX_OFF
    msWriteByte(REG_113476 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x38); // DP_AUX 6ma 10ns 800mv
    msWriteByte(REG_113480 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x00); // DP PHY Resistor power down.  Enable Rterm
    msWriteByte(REG_113481 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x36); // DP PHY Resistor Control Overwrite enable. DP PHY Resistor Control. Rterm 50ohm
    msWriteByte(REG_11348C +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x10); // DP EQ threshold set 0 for EQ lose lock issue : DP EQ threshold set 10h
    msWriteByte(REG_11348A +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x04); // 12M DP main link clock lock threshold when 162MHz.
    msWriteByte(REG_11348B +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x10); // 12M DP main link clock lose threshold when 162MHz.
    msWriteByte(REG_113492 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x2D); // 12M DP main link clock lock when 270MHz.  0x5A0
    msWriteByte(REG_113493 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x01); // 12M DP main link clock lock when 270MHz.
    msWriteByte(REG_113494 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x04); // 12M DP main link clock lock threshold when 270MHz.
    msWriteByte(REG_113495 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x12); // 12M DP main link clock lose threshold when 270MHz.
    msWriteByte(REG_113496 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x6A); // 12M DP main link clock lock when 162MHz.    0x6C0
    msWriteByte(REG_113497 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x01); // 12M DP main link clock lock when 162MHz.
    msWriteByte(REG_113498 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x10); // 14M DP AUX PHY UI counter threshold.
    msWriteByte(REG_113499 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x0D); // 14M DP AUX PHY over sample rate.
    msWriteByte(REG_11349A +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x63); // 14M DP AUX timeout threshold.
    msWriteByte(REG_11349B +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x08); // 14M DP AUX timeout threshold.  AUX CLK select 50Mhz
    msWriteByte(REG_113468 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x06); // DP EQ0 SET   3Ch
    msWriteByte(REG_113469 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x06); // DP EQ1 SET   3Ch
    msWriteByte(REG_113487 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x06); // DP EQ2 SET   3Ch
    msWriteByte(REG_113489 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x06); // DP EQ3 SET   3Ch
    msWriteByte(REG_11345E +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x04); // BIT4 ignore Audio ID
    msWriteByte(REG_11349E +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x0C); // BIT2 Align Word Boundary Overwrite Enable
    msWriteByte(REG_11349C +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x3f); // DP main link lock Divider 2.7G.
    msWriteByte(REG_11349D +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x7f); // DP main link lock Divider 1.62G
    msWriteByte(REG_1134FA +DP_HBR2_REG_OFFSET(ucHBR2Select), 0xCF); //  Bit1: Disable AUPLL Frezze interrupt , Bit6: CLK HDCP_EN , Bit7: SD clk
    msWriteByte(REG_1134D1 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x00); // VPLL_PWDWN
    msWriteByte(REG_1134B3 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x00); // APLL disable PWD
    msWriteByte(REG_1134B8 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x22); // APLL 512*Fs at 1.62Ghz 162Mhz
    msWriteByte(REG_1134C6 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x09); // BW more speed up
    msWriteByte(REG_1134D5 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x0F); // [1:0] option enable Training P1 P2 IRQ To BankB 0xE8[0:1] [4:2] Pre-syn zero
    msWriteByte(REG_11346E +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x00); //  set default
    msWriteByte(REG_11346F +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x00); //  set default
    msWriteByteMask(REG_113475 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT7, BIT7); // AUX won’t be powerdown

    msWriteByteMask(REG_11349F +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT7| BIT6, BIT7| BIT6);     // MAC
    msWriteByteMask(REG_1134F7 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT5, BIT5);			    // CDR and SQ
    
    // ##############################################
    // ##                                        BANK 1135                                     ##
    // ##############################################
    msWriteByteMask(REG_113581 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT5, BIT5); //de-glitch when frame reset (1680X1050)	
    msWriteByte(REG_113580 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x41);                                                     // Htotal Tolerance (Learn)
    msWriteByte(REG_1135A2 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT7| BIT6| BIT1| BIT0); // Aux Clock Src,// Set AUX Debounce clk
    msWriteByte(REG_11356E +DP_HBR2_REG_OFFSET(ucHBR2Select), 0xFF);                                                     // New 8/10b Fifo
    msWriteByte(REG_1135C8 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x0F);                                                     // select new 10b/8b decoder
    msWriteByte(REG_1135A0 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x40);                                                     // Enable AUX Debounce
    msWriteByte(REG_11350D +DP_HBR2_REG_OFFSET(ucHBR2Select), 0);                                                          // set V paulse  small
    msWriteByte(REG_113518 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x1F);
    msWriteByte(REG_113519 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x3F);
    msWriteByte(REG_113507 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0xE5);                                                    //DP Interlace mode
    msWriteByte(REG_11357F +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x01);                                                    //Reset Word Boundary
    msWriteByteMask(REG_113581 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT3| BIT2, BIT3| BIT2);
    msWriteByteMask(REG_113541 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT3, BIT3);
    msWriteByteMask(REG_1135D4 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT2| BIT1, BIT2| BIT1); // HDCP Rx

    // ##############################################
    // ##                                        BANK 1132                                     ##
    // ##############################################
    msWriteByte(REG_1132A8 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x0C);                                                            //[2]: ignore Audio ID [3] Enable RS code sync reset
    msWriteByte(REG_1132EC +DP_HBR2_REG_OFFSET(ucHBR2Select), 0xFF);                                                           //[5]:Enable Audio time Stamp [6]Audio Stream [7] Audio Protect
    msWriteByte(REG_1132F0 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x7F);                                                             //[0]~[7] Enable Audio Mute Event.
    msWriteByte(REG_1132F1 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x7F);                                                             //[0]~[7] Enable Audio Fading Event.
    msWriteByteMask(REG_1132ED +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT1| BIT3, BIT1| BIT3); //Enable VSC Packet  // receive Packet Update

    // ##############################################
    // ##                                         BANK 1131                                     ##
    // ##############################################
    #if (COMBO_DP_DAISY_CHAIN_SUPPORT == 0x1)
	msWriteByte(REG_113100 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0xB1);
	#else
    msWriteByte(REG_113100 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0xF1);
	#endif
    msWriteByte(REG_113102 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x84);
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_DP_SetHBR2Mux()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void _mhal_DP_SetHBR2Mux(BYTE ucPortSelect, BYTE ucHBR2Select)
{
    if(ucHBR2Select == DP_HBR2_ENGINE_0)
    {
        msWriteByteMask(REG_1630DE, ucHBR2Select, MASKBIT(2:0)); //HBR2 port0  for Port 0
        msWriteByteMask(REG_1630DF, ucHBR2Select, MASKBIT(2:0)); //HBR2 port1  for Port 1
        msWriteByteMask(REG_1630E0, ucHBR2Select, MASKBIT(2:0)); // Clock_pixel

        msWriteByteMask(REG_1630E1, ucHBR2Select, MASKBIT(2:0)); //MAC   // Fixed VPLL 0
        msWriteByteMask(REG_162F96, (DP_HBR2_AUDIO_OFFSET(ucPortSelect) <<4), MASKBIT(7:4)); // HBR0 CLK
        msWrite2ByteMask(REG_1630EC, (DP_HBR2_AUDIO_OFFSET(ucHBR2Select) <<4), MASKBIT(6:4)); // VPLL 0 mux // Fixed VPLL 0
        msWriteByteMask(REG_162F97, BIT0, BIT0); // HBR0 ==> HBR2 1p ou

        msWriteByteMask(REG_1630E0, BIT3| BIT5| BIT6, BIT3| BIT5| BIT6);
        msWriteByteMask(REG_1630E1, BIT3, BIT3);
        
#if DP_AUX_USE_XTAL  
#if MST9U2   		
        msWriteByteMask(REG_1630E2, BIT2| BIT0, MASKBIT(3:0)); // Not inver AUX MAC clock FROM XTAL  
#else
        msWriteByteMask(REG_1630E2, BIT3| BIT2| BIT0, MASKBIT(3:0));  //  Inver AUX
#endif
#else
        msWriteByteMask(REG_1630E2, BIT2, MASKBIT(2:0)); // MAC clock FROM OSC
#endif

        msWriteByteMask(REG_1630EE, BIT0| BIT1, BIT0| BIT1); //reg_clk_combo2sc_en
        msWriteByteMask(REG_162F82, 0, BIT0); //VPLL 0 PD diable
        msWriteByteMask(REG_162F83, 0, BIT0);
    }
    else if(ucHBR2Select == DP_HBR2_ENGINE_1)
    {
        msWriteByteMask(REG_1630DE, (ucHBR2Select <<4), MASKBIT(6:4)); //HBR2 port0  for Port 0
        msWriteByteMask(REG_1630DF, (ucHBR2Select <<4), MASKBIT(6:4)); //HBR2 port1  for Port 1
        msWriteByteMask(REG_1630E4, ucHBR2Select, MASKBIT(2:0)); // Clock_pixel

        msWriteByteMask(REG_1630E5, ucHBR2Select, MASKBIT(2:0)); // MAC  	// Fixed VPLL 1
        msWriteByteMask(REG_162FB6, (DP_HBR2_AUDIO_OFFSET(ucPortSelect) <<4), MASKBIT(7:4));  // HBR1 CLK  SRC
        msWrite2ByteMask(REG_1630EC, (DP_HBR2_AUDIO_OFFSET(ucHBR2Select) <<7), MASKBIT(9:7)); // VPLL 1 mux    // Fixed VPLL 1
        msWriteByteMask(REG_162FB7, BIT0 , BIT0); //  HBR1 ==> HBR2 1p ou
        
        msWriteByteMask(REG_1630E4, BIT3| BIT5| BIT6, BIT3| BIT5| BIT6);
        msWriteByteMask(REG_1630E5, BIT3, BIT3);
        
#if DP_AUX_USE_XTAL
#if MST9U2
        msWriteByteMask(REG_1630E6, BIT2| BIT0, MASKBIT(3:0)); // Not inver AUXMAC clock FROM XTAL
#else
        msWriteByteMask(REG_1630E6, BIT3| BIT2| BIT0, MASKBIT(3:0));	// Inverter AUX	
#endif
#else
        msWriteByteMask(REG_1630E6, BIT2, MASKBIT(2:0)); // MAC clock FROM OSC
#endif

        msWriteByteMask(REG_1630EE, BIT0| BIT1, BIT0| BIT1); //reg_clk_combo2sc_en
        msWriteByteMask(REG_162FA2, 0, BIT0); //VPLL 1 PD diable
        msWriteByteMask(REG_162FA3, 0, BIT0);
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_DP_AutoEQSetting()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_DP_AutoEQSetting(BYTE ucPortSelect, BYTE ucHBR2Select)
{
#if(DP_AUTO_EQ_ENABLE)
    if(ucHBR2Select != DP_HBR2_ENGINE_0)
    {
        msWrite2Byte(REG_1120DC +DP_PHY_REG_OFFSET(ucPortSelect), 0x0000);
        //----SYMBOL
        msWrite2Byte(REG_11204A +DP_PHY_REG_OFFSET(ucPortSelect), 0x0A4C); //wriu -w 0x11204a 0x0a4c //[4]=0, AABA
        msWrite2Byte(REG_112052 +DP_PHY_REG_OFFSET(ucPortSelect), 0x0003);	 	    //wriu -w 0x112052 0x0003        //29[15:0]:Time out for symbol lockdet (us) = 3us
        //TPS2=100bits, (96*4*0.37n*10)=2841ns, correct ratio=0.2841u/3u=0.947
        //if set 0 --> EQ=symbol
        //----EQ quality check (under/over)
        msWrite2Byte(REG_112050 +DP_PHY_REG_OFFSET(ucPortSelect), 0xbb80); //wriu -w 0x112050 0xbb80 //[2:0]=0 eq_accu th=0. [7]=1, speedup. [9:8]=11, set max, [11]=1, retune en. [15]=1, en EQ start
        msWrite2Byte(REG_112056 +DP_PHY_REG_OFFSET(ucPortSelect), 0x0007); //wriu -w 0x112056 0x0007 	//2B[15:0]: time out for EQ under/over check (us) (ACCU=7us)
        msWrite2Byte(REG_112058 +DP_PHY_REG_OFFSET(ucPortSelect), 0x0002); //wriu -w 0x112058 0x0002 	//2C[15:0]: fine tune time value (MASK=2us)
        //----EQ stable check
        msWrite2Byte(REG_11204C +DP_PHY_REG_OFFSET(ucPortSelect), 0x8a0a); //wriu -w 0x11204c 0x8a0a 	//26[13:8]: max fine tune count to check if EQ stable or not,
        //start to check after count=10
        //[15]: enable autoEQ control symbol lock detect timeout to go to EQ fine tune
        msWrite2Byte(REG_11205A +DP_PHY_REG_OFFSET(ucPortSelect), 0x0028); //wriu -w 0x11205a 0x0028 	//2D[15:0], time out for fine tune (unit: find tune count)
        //time out after count=40
        msWrite2Byte(REG_11204E +DP_PHY_REG_OFFSET(ucPortSelect), 0x0503); //wriu -w 0x11204e 0x0503 	//27[13:8]: EQ stable tolerance =5
        //The CR lock reference failure threshold range for HBR/RBR
        msWriteByteMask(REG_112091 +DP_PHY_REG_OFFSET(ucPortSelect), 0x09, 0xFF);
        //Always turn on PHDAC.
        msWriteByteMask(REG_1120B2 +DP_PHY_REG_OFFSET(ucPortSelect), 0x02, 0x02); // en pd_phdac overwirte
        msWriteByteMask(REG_1120D7 +DP_PHY_REG_OFFSET(ucPortSelect), 0x00, 0xF0); // ATOP - Power Down  PHDAC bit[15:12]
        msWriteByte(REG_112067 +DP_PHY_REG_OFFSET(ucPortSelect), 0x07);	//REG_112067	//wriu	  0x112067 0x07 //eq start=7
    
        msWriteByteMask(REG_113000 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x01, 0x01);
        msWriteByte(REG_113012 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x00);
        msWriteByteMask(REG_1133B5 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x20, 0x28);

        msWriteByte(REG_11348C +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x60); //wriu 0x11348c 0x60               //46[7:0]: EQ done threshold, //check symbol correct 0x60=96, 96*8*3.7n=2841n

        //----sync request pre-emphasis
        msWrite2Byte(REG_113000 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x0285); //wriu -w 0x113000 0x0285 	//1130_00: [3:2][5:4][7:6]: 3 pre-emphasis levels, set (1,0,1)
        msWrite2Byte(REG_113076 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x1010); //wriu -w 0x113076 0x1010 	//1130_3b[7:0]: EQ error count th for each good EQ setting judgement
        //1130_3b[15:8]: EQ error count th for each EQ ph check
        msWriteByteMask(REG_113079 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x0C, 0x0C); //wriu -b 0x113079 0x0c 0x0c	//1130_3c[11:10]=11, [10]: align each lane pre-emphasis request value during autoEQ
        //[11]: udate best pre-emphasis setting when previous and current goodphase number are equal
        //----SCAN GOOD PHASE
        msWriteByte(REG_11300C +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x18); //wriu 0x11300c 0x18		//06[7:0]:timer threshold 4x xtal for error counter accumulation period (unit: 0.28uS)
        msWriteByte(REG_11300D +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x20); //wriu 0x11300d 0x20		//06[15:8]: error count mask (unit: LS_CLK)

        // Auto EQ phase step
        msWriteByteMask(REG_113078 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0xC0, 0xC0); // step = 1
    }

#else
    // DP EQ
    msWrite2Byte(REG_1120DC +DP_PHY_REG_OFFSET(ucPortSelect), 0x03C0);
    msWrite2Byte(REG_1120E6 +DP_PHY_REG_OFFSET(ucPortSelect), 0x4210);     //  EQ=16
    msWriteByteMask(REG_1120E8 +DP_PHY_REG_OFFSET(ucPortSelect), 0x10, 0x1F);
    
#endif
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_DP_EnablePHYInterrupt()
//  [Description]
//                  
//  [Arguments]:
//                  
//  [Return]:
//
//**************************************************************************
void _mhal_DP_EnablePHYInterrupt(BYTE ucPortSelect, BYTE ucHBR2Select, Bool bEnable)
{
    BYTE ucInterruptMask = 0;
    
    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        ucInterruptMask = (BIT0 << ucPortSelect);
    }
    else // HBR2 path
    {
        ucInterruptMask = (BIT6 << ucHBR2Select);
    }

    msWriteByteMask(REG_163070, bEnable? ucInterruptMask: 0, ucInterruptMask);
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_DP_SplitSelect()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_DP_SplitSelect(BYTE ucSplitSelect, BYTE ucHBR2Select)
{
    Bool bSplit2P = FALSE;
    
    if(ucSplitSelect == DP_SPLIT_MODE_1P)
    {
        msWriteByteMask(REG_113559 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0, BIT5);
        msWriteByteMask(REG_1630E1 +ucHBR2Select *4, 0, BIT5);
    }
    else if(ucSplitSelect == DP_SPLIT_MODE_2P)
    {
        msWriteByteMask(REG_113559 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT5, BIT5);
        msWriteByteMask(REG_1630E1 +ucHBR2Select *4, BIT5, BIT5);

        bSplit2P = TRUE;
    }

    if(ucHBR2Select == DP_HBR2_ENGINE_0)
    {
        msWriteByteMask(REG_162F97 +DP_HBR2_REG_OFFSET(ucHBR2Select), bSplit2P? 0: BIT0, BIT0);
    }
    else if(ucHBR2Select == DP_HBR2_ENGINE_1)
    {
        msWriteByteMask(REG_162FB7 +DP_HBR2_REG_OFFSET(ucHBR2Select), bSplit2P? 0: BIT0, BIT0);
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_DP_GetAuxDDCData()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
BYTE _mhal_DP_GetAuxDDCData(BYTE ucPortSelect)
{
    BYTE ucDDCData = 0;
    WORD usCount = 100;
    
    msWriteByteMask(REG_0011FC +DP_PHY_REG_OFFSET(ucPortSelect), BIT2, BIT2);

    // Must fine tune
    while(usCount > 0)
    {
        usCount--;

        _nop_();
    }

    ucDDCData = msReadByte(REG_001175 +DP_PHY_REG_OFFSET(ucPortSelect));

    return ucDDCData;
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_DP_MCCSReceiveProc()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_DP_MCCSReceiveProc(BYTE ucPortSelect)
{
    static BYTE ucDDCIndex = 0;
    BYTE uctemp = 0;
    BYTE ucAuxCommand = msReadByte(REG_001138 +DP_PHY_REG_OFFSET(ucPortSelect))&0x0F;
    BYTE ucAuxLength = msReadByte(REG_00113A +DP_PHY_REG_OFFSET(ucPortSelect));

    if(ucAuxLength > DP_AUX_COMMAND_MAX_LENGTH)
    {
        ucAuxLength = DP_AUX_COMMAND_MAX_LENGTH;
    }
    
    if((ucAuxCommand &BIT0) == BIT0)
    {
        if(mdrv_DDC2BI_GetDPSendFlag(ucAuxLength, ucDDCIndex, ucDDCBuffer)) // DDC Tx buffer not empty
        {
            // Update Aux length from DDC Tx buffer
            ucAuxLength = ucDDCBuffer[0];
            
            for(uctemp = 0; uctemp <ucAuxLength; uctemp++)
            {
                if(ucDDCIndex == 0) // First data is 0x6E
                {
                    msWriteByte(REG_0011D3 +DP_PHY_REG_OFFSET(ucPortSelect), DDC2B_DEST_ADDRESS);
                }
                else // Next data from DDC Tx buffer
                {
                    msWriteByte(REG_0011D3 +DP_PHY_REG_OFFSET(ucPortSelect), ucDDCBuffer[uctemp +1]);
                }

                ucDDCIndex++;
            }
        }
        else // DDC Tx buffer not ready or DDC Tx buffer empty
        {
            if(ucAuxLength > DP_AUX_COMMAND_DDC_LENGTH)
            {
                ucAuxLength = DP_AUX_COMMAND_DDC_LENGTH;
            }
            
            for(uctemp = 0; uctemp <ucAuxLength; uctemp++) // Can't write 16 data
            {
                msWriteByte(REG_0011D3 +DP_PHY_REG_OFFSET(ucPortSelect), ucDDCNullMessage[uctemp]);
            }

            // Reset DDC buffer index when Tx buffer empty
            ucDDCIndex = 0;
        }
    }
    else if((ucAuxCommand == 0x00) || (ucAuxCommand == 0x04))
    {
        if(ucAuxLength > 0)
        {
            if(ucDDCIndex == 0)
            {
                // Ignore first data
                uctemp = _mhal_DP_GetAuxDDCData(ucPortSelect);

                ucAuxLength--;
                ucDDCIndex++;
            }

            for(uctemp = 0; uctemp <ucAuxLength; uctemp++)
            {
                ucDDCBuffer[ucDDCIndex -1] = _mhal_DP_GetAuxDDCData(ucPortSelect);

                ucDDCIndex++;
            }
        }

        if(ucAuxCommand == 0x00)
        {
            if(ucDDCIndex > 0)
            {
                mdrv_DDC2BI_SetDPReceiveStatus(ucPortSelect, ucDDCIndex, ucDDCBuffer);

                // Reset DDC buffer index when Rx receive finish
                ucDDCIndex = 0;
            }
        }
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_SquelchLevelSetting()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_DP_SquelchLevelSetting(BYTE ucPortSelect, BYTE ucSquelchLevel)
{
    ucSquelchLevel = ucSquelchLevel &MASKBIT(3:0);
    ucSquelchLevel = ucSquelchLevel << 4 |ucSquelchLevel;
    
    switch(ucPortSelect)
    {
        case DP_INPUT_SELECT_PORT0:
            msWriteByteMask(REG_0017A8, ucSquelchLevel, MASKBIT(3:0));
            break;

        case DP_INPUT_SELECT_PORT1:
            msWriteByteMask(REG_0017A8, ucSquelchLevel, MASKBIT(7:4));
            break;

        case DP_INPUT_SELECT_PORT2:
            msWriteByteMask(REG_0017A9, ucSquelchLevel, MASKBIT(3:0));
            break;

        case DP_INPUT_SELECT_PORT3:
            msWriteByteMask(REG_0017A9, ucSquelchLevel, MASKBIT(7:4));
            break;

        case DP_INPUT_SELECT_PORT4:
            msWriteByteMask(REG_0017AA, ucSquelchLevel, MASKBIT(3:0));
            break;

        case DP_INPUT_SELECT_PORT5:
            msWriteByteMask(REG_0017AA, ucSquelchLevel, MASKBIT(7:4));
            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_DP_SetHDCP2LinkIntegrityFailure()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void _mhal_DP_SetHDCP2LinkIntegrityFailure(BYTE ucPortSelect, Bool bEnable)
{
    switch(ucPortSelect)
    {
        case DP_INPUT_SELECT_PORT0:
            msWriteByteMask(REG_1133E1, bEnable? 0: BIT5, BIT5); // REG_DP_BANKB_70[12]: DP HDCP link integrity failure interrupt mask
            msWriteByteMask(REG_1133EB, BIT5, BIT5); // REG_DP_BANKB_74[12]: DP HDCP link integrity failure interrupt
            break;

        case DP_INPUT_SELECT_PORT1:
            
            break;

        case DP_INPUT_SELECT_PORT2:
            
            break;

        case DP_INPUT_SELECT_PORT3:
            
            break;

        default:

            break;
    };
}

#if(DP_HDCP2_FUNCTION_SUPPORT)
//**************************************************************************
//  [Function Name]:
//                  _mhal_DP_HDCP2Initial()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void _mhal_DP_HDCP2Initial(BYTE ucPortSelect)
{
    switch(ucPortSelect)
    {
        case DP_INPUT_SELECT_PORT0:
            msWriteByteMask(REG_113A9F, MASKBIT(1:0), MASKBIT(1:0)); // REG_DPCD_HDCP22_4_4F[9:8]: reg_rx_status_hw_md and reg_reauth_req_hw_md
            break;

        case DP_INPUT_SELECT_PORT1:
            
            break;

        case DP_INPUT_SELECT_PORT2:
            
            break;

        case DP_INPUT_SELECT_PORT3:
            
            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_DP_SetHDCP2AKEInitInterrupt()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void _mhal_DP_SetHDCP2AKEInitInterrupt(BYTE ucPortSelect, Bool bEnable)
{
    switch(ucPortSelect)
    {
        case DP_INPUT_SELECT_PORT0:
            msWriteByteMask(REG_113AE2, bEnable? 0: BIT1, BIT1); // REG_DPCD_HDCP22_4_71[1]: reg_ake_init_irq_mask
            msWriteByteMask(REG_113AE6, BIT1, BIT1); // REG_DPCD_HDCP22_4_73[1]: reg_ake_init_irq_clr
            msWriteByteMask(REG_113AE6, 0, BIT1); // REG_DPCD_HDCP22_4_73[1]: reg_ake_init_irq_clr
            break;

        case DP_INPUT_SELECT_PORT1:
            
            break;

        case DP_INPUT_SELECT_PORT2:
            
            break;

        case DP_INPUT_SELECT_PORT3:
            
            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_DP_SetHDCP2NoStoredKmInterrupt()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void _mhal_DP_SetHDCP2NoStoredKmInterrupt(BYTE ucPortSelect, Bool bEnable)
{
    switch(ucPortSelect)
    {
        case DP_INPUT_SELECT_PORT0:
            msWriteByteMask(REG_113AE2, bEnable? 0: BIT2, BIT2); // REG_DPCD_HDCP22_4_71[2]: reg_ake_no_stored_km_irq_mask
            msWriteByteMask(REG_113AE6, BIT2, BIT2); // REG_DPCD_HDCP22_4_73[2]: reg_ake_no_stored_km_irq_clr
            msWriteByteMask(REG_113AE6, 0, BIT2); // REG_DPCD_HDCP22_4_73[2]: reg_ake_no_stored_km_irq_clr
            break;

        case DP_INPUT_SELECT_PORT1:
            
            break;

        case DP_INPUT_SELECT_PORT2:
            
            break;

        case DP_INPUT_SELECT_PORT3:
            
            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_DP_SetHDCP2StoredKmInterrupt()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void _mhal_DP_SetHDCP2StoredKmInterrupt(BYTE ucPortSelect, Bool bEnable)
{
    switch(ucPortSelect)
    {
        case DP_INPUT_SELECT_PORT0:
            msWriteByteMask(REG_113AE2, bEnable? 0: BIT3, BIT3); // REG_DPCD_HDCP22_4_71[3]: reg_ake_stored_km_irq_mask
            msWriteByteMask(REG_113AE6, BIT3, BIT3); // REG_DPCD_HDCP22_4_73[3]: reg_ake_stored_km_irq_clr
            msWriteByteMask(REG_113AE6, 0, BIT3); // REG_DPCD_HDCP22_4_73[3]: reg_ake_stored_km_irq_clr
            break;

        case DP_INPUT_SELECT_PORT1:
            
            break;

        case DP_INPUT_SELECT_PORT2:
            
            break;

        case DP_INPUT_SELECT_PORT3:
            
            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_DP_SetHDCP2LCInitInterrupt()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void _mhal_DP_SetHDCP2LCInitInterrupt(BYTE ucPortSelect, Bool bEnable)
{
    switch(ucPortSelect)
    {
        case DP_INPUT_SELECT_PORT0:
            msWriteByteMask(REG_113AE2, bEnable? 0: BIT4, BIT4); // REG_DPCD_HDCP22_4_71[4]: reg_locality_check_irq_mask
            msWriteByteMask(REG_113AE6, BIT4, BIT4); // REG_DPCD_HDCP22_4_73[4]: reg_locality_check_irq_clr
            msWriteByteMask(REG_113AE6, 0, BIT4); // REG_DPCD_HDCP22_4_73[4]: reg_locality_check_irq_clr
            break;

        case DP_INPUT_SELECT_PORT1:
            
            break;

        case DP_INPUT_SELECT_PORT2:
            
            break;

        case DP_INPUT_SELECT_PORT3:
            
            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_DP_SetHDCP2SKESendInterrupt()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void _mhal_DP_SetHDCP2SKESendInterrupt(BYTE ucPortSelect, Bool bEnable)
{
    switch(ucPortSelect)
    {
        case DP_INPUT_SELECT_PORT0:
            msWriteByteMask(REG_113AE2, bEnable? 0: BIT5, BIT5); // REG_DPCD_HDCP22_4_71[5]: reg_ske_irq_mask
            msWriteByteMask(REG_113AE6, BIT5, BIT5); // REG_DPCD_HDCP22_4_73[5]: reg_ske_irq_clr
            msWriteByteMask(REG_113AE6, 0, BIT5); // REG_DPCD_HDCP22_4_73[5]: reg_ske_irq_clr
            break;

        case DP_INPUT_SELECT_PORT1:
            
            break;

        case DP_INPUT_SELECT_PORT2:
            
            break;

        case DP_INPUT_SELECT_PORT3:
            
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
//                  mhal_DP_AuxCommonSetting()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void mhal_DP_AuxCommonSetting(BYTE ucDPEnableIndex)
{
    if(ucDPEnableIndex > 0)
    {
#if DP_AUX_USE_XTAL
#if MST9U2	
        msWriteByteMask(REG_000392, BIT1, BIT1);        
#else
        msWriteByteMask(REG_000392, 0, BIT1);           
#endif	
        msWriteByteMask(REG_000392, BIT2, BIT2 |BIT3);                                                                          // Aux Clock SRC
        msWriteByteMask(REG_000390, 0, BIT5 |BIT4);        // DDC Clock use Xtal.
#else
        msWriteByteMask(REG_000392, 0, BIT2 |BIT3);
        msWriteByteMask(REG_000390, BIT4, BIT5 |BIT4);   // DDC Clock use Xtal.
#endif
    
        msWriteByteMask(REG_0017A6,(ucDPEnableIndex &0x3F), 0x3F); //Enable SQ
        msWriteByteMask(REG_0017C2, BIT2, BIT2); //Enable SQ
        
        msWriteByteMask(REG_000381, BIT3, BIT3); // Fake Training Reply
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_SetInterruptMask()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void mhal_DP_SetInterruptMask(BYTE ucPortSelect)
{
    msWriteByteMask(REG_0011A9 +DP_PHY_REG_OFFSET(ucPortSelect), BIT5, BIT5); //affect SAR11/SAR3 when MHL
    msWriteByte(REG_00117B +DP_PHY_REG_OFFSET(ucPortSelect), 0xFF); // Mask ALL AUX interrupt
    msWriteByte(REG_001191 +DP_PHY_REG_OFFSET(ucPortSelect), 0xF0);	// Mark _dpcd0_irq_mask
    msWriteByteMask(REG_001193 +DP_PHY_REG_OFFSET(ucPortSelect), BIT6, BIT6); // Mask __aksv_irq_mask
    msWriteByteMask(REG_001196 +DP_PHY_REG_OFFSET(ucPortSelect), BIT4 |BIT7, BIT4 |BIT7); // Mask SQ and Time Out  Mask
    msWriteByteMask(REG_0011AE +DP_PHY_REG_OFFSET(ucPortSelect), 0, MASKBIT(2:0)); // turn off PM detect
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_Initial()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_DP_Initial(BYTE ucPortSelect, BYTE ucHBR2Select)
{
    // Open rterm
    _mhal_DP_RtermEnable(ucPortSelect);
    // Aux initial setting
    _mhal_DP_AuxInitialSetting(ucPortSelect);
    _mhal_DP_SetProgramDPCD(ucPortSelect, DP_PROGRAM_DPCD_0);
    // PYH initial setting
    _mhal_DP_ComboPHYSetting(ucPortSelect);
    
    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {

    }
    else // HBR2 path
    {
        _mhal_DP_HBR2InitialSetting(ucHBR2Select);
        _mhal_DP_SetHBR2Mux(ucPortSelect, ucHBR2Select);
    }

    // Auto EQ setting
    _mhal_DP_AutoEQSetting(ucPortSelect, ucHBR2Select);
    // Enable interrupt mask
    _mhal_DP_SetMCCSReceiveInterrupt(ucPortSelect, TRUE);
    _mhal_DP_SetProgramDPCD0Interrupt(ucPortSelect, TRUE);
    _mhal_DP_SetAUPLLBigChangeInterrupt(ucPortSelect, ucHBR2Select, TRUE);
    _mhal_DP_SetInterlaneSkewInterrupt(ucPortSelect, ucHBR2Select, TRUE);
    _mhal_DP_SetCDRLossLockInterrupt(ucPortSelect, ucHBR2Select, TRUE);
    _mhal_DP_SetVPLLBigChangeInterrupt(ucPortSelect, ucHBR2Select, TRUE);
    _mhal_DP_SetTrainingPattern1Interrupt(ucPortSelect, ucHBR2Select, TRUE);
    _mhal_DP_SetTrainingPattern2Interrupt(ucPortSelect, ucHBR2Select, TRUE);

#if(DP_HDCP2_FUNCTION_SUPPORT)
    _mhal_DP_HDCP2Initial(ucPortSelect);
    _mhal_DP_SetHDCP2AKEInitInterrupt(ucPortSelect, TRUE);
    _mhal_DP_SetHDCP2NoStoredKmInterrupt(ucPortSelect, TRUE);
    _mhal_DP_SetHDCP2StoredKmInterrupt(ucPortSelect, TRUE);
    _mhal_DP_SetHDCP2LCInitInterrupt(ucPortSelect, TRUE);
    _mhal_DP_SetHDCP2SKESendInterrupt(ucPortSelect, TRUE);
#endif

    // Enable interrupt
    _mhal_DP_EnablePHYInterrupt(ucPortSelect, ucHBR2Select, TRUE);

    bDPForceDownSpeed[ucPortSelect] = FALSE;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_ClockEnable()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_DP_ClockEnable(void)
{
    msWrite2Byte(REG_162FF8, 0xFFFF);
    msWriteByte(REG_162FFA, 0xFF);
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_EnableHPD()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_DP_EnableHPD(BYTE ucPortSelect)
{
    switch(ucPortSelect)
    {
        case DP_INPUT_SELECT_PORT0:
            SET_DP_HPD_PORT0();
            SET_DP_CABLE_5V_PORT0();
            break;

        case DP_INPUT_SELECT_PORT1:
            SET_DP_HPD_PORT1();
            SET_DP_CABLE_5V_PORT1();
            break;

        case DP_INPUT_SELECT_PORT2:
            SET_DP_HPD_PORT2();
            SET_DP_CABLE_5V_PORT2();
            break;

        case DP_INPUT_SELECT_PORT3:
            SET_DP_HPD_PORT3();
            SET_DP_CABLE_5V_PORT3();
            break;

        case DP_INPUT_SELECT_PORT4:
            SET_DP_HPD_PORT4();
            SET_DP_CABLE_5V_PORT4();
            break;

        case DP_INPUT_SELECT_PORT5:
            SET_DP_HPD_PORT5();
            SET_DP_CABLE_5V_PORT5();
            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_HPDControl()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_DP_HPDControl(BYTE ucPortSelect, Bool bSetHPD)
{
    switch(ucPortSelect)
    {
        case DP_INPUT_SELECT_PORT0:
            if(bSetHPD)
            {
                SET_DP_HPD_PORT0();
            }
            else
            {
                CLR_DP_HPD_PORT0();
            }
            
            break;

        case DP_INPUT_SELECT_PORT1:
            if(bSetHPD)
            {
                SET_DP_HPD_PORT1();
            }
            else
            {
                CLR_DP_HPD_PORT1();
            }
            
            break;

        case DP_INPUT_SELECT_PORT2:
            if(bSetHPD)
            {
                SET_DP_HPD_PORT2();
            }
            else
            {
                CLR_DP_HPD_PORT2();
            }
            
            break;

        case DP_INPUT_SELECT_PORT3:
            if(bSetHPD)
            {
                SET_DP_HPD_PORT3();
            }
            else
            {
                CLR_DP_HPD_PORT3();
            }
            
            break;

        case DP_INPUT_SELECT_PORT4:
            if(bSetHPD)
            {
                SET_DP_HPD_PORT4();
            }
            else
            {
                CLR_DP_HPD_PORT4();
            }
            
            break;

        case DP_INPUT_SELECT_PORT5:
            if(bSetHPD)
            {
                SET_DP_HPD_PORT5();
            }
            else
            {
                CLR_DP_HPD_PORT5();
            }
            
            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_CableDetect()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mhal_DP_CableDetect(BYTE ucPortSelect)
{
    Bool bCableDetect = FALSE;
    
    switch(ucPortSelect)
    {
        case DP_INPUT_SELECT_PORT0:
            bCableDetect = (DP_CABLE_DETECT_PORT0 ?TRUE: FALSE);
            break;

        case DP_INPUT_SELECT_PORT1:
            bCableDetect = (DP_CABLE_DETECT_PORT1 ?TRUE: FALSE);
            break;

        case DP_INPUT_SELECT_PORT2:
            bCableDetect = (DP_CABLE_DETECT_PORT2 ?TRUE: FALSE);
            break;

        case DP_INPUT_SELECT_PORT3:
            bCableDetect = (DP_CABLE_DETECT_PORT3 ?TRUE: FALSE);
            break;

        case DP_INPUT_SELECT_PORT4:
            bCableDetect = (DP_CABLE_DETECT_PORT4 ?TRUE: FALSE);
            break;

        case DP_INPUT_SELECT_PORT5:
            bCableDetect = (DP_CABLE_DETECT_PORT5 ?TRUE: FALSE);
            break;

        default:

            break;
    };

    return bCableDetect;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_EnableAuxInterrupt()
//  [Description]
//                  
//  [Arguments]:
//                  
//  [Return]:
//
//**************************************************************************
void mhal_DP_EnableAuxInterrupt(Bool bEnableInterrupt)
{
    msWriteByteMask(REG_002B29, bEnableInterrupt? 0: BIT5, BIT5); // REG_IRQ_BANK_14[13]
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_GetAuxInterruptFlag()
//  [Description]
//                  
//  [Arguments]:
//                  
//  [Return]:
//
//**************************************************************************
Bool mhal_DP_GetAuxInterruptFlag(void)
{
    return ((msReadByte(REG_002B39) & BIT5) ?TRUE :FALSE); // REG_IRQ_BANK_19[13]
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_CheckSquelch()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mhal_DP_CheckSquelch(BYTE ucPortSelect)
{
    Bool bSquelchFlag = TRUE;
    BYTE uctemp = 0;
    BYTE ucSquelchMask = (BIT0 << ucPortSelect);

    for(uctemp = 0; uctemp <DP_CHECK_SQUELCH_COUNTER; uctemp--)
    {
        if(!(msReadByte(REG_0017B0) &ucSquelchMask))
        {
            bSquelchFlag = FALSE;
        }
    }

    return bSquelchFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_InsertGUIDValue()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_DP_InsertGUIDValue(BYTE ucPortSelect, BYTE *ucGUIDQueue)
{
    BYTE uctemp = 0;

    for(uctemp = 0; uctemp <DP_AUX_COMMAND_MAX_LENGTH; uctemp--)
    {
        msWriteByte(REG_001162 +DP_PHY_REG_OFFSET(ucPortSelect) +uctemp, ucGUIDQueue[uctemp]);
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_CheckCDRLock()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mhal_DP_CheckCDRLock(BYTE ucPortSelect, BYTE ucHBR2Select)
{
    Bool bCDRLockFlag = FALSE;
    BYTE ucLansCount = 0;
    WORD usLinkRegOffset = _mhal_DP_ComboLinkRegisterOffset(ucPortSelect);
    WORD usLockStatus = 0;
    
    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        ucLansCount = msReadByte(REG_160781 +usLinkRegOffset) &0x07;
        usLockStatus = msRead2Byte(REG_1607C3 +usLinkRegOffset);
        usLockStatus = ((usLockStatus &0x3838) << 1) | (usLockStatus &0x0707);
    }
    else // HBR2 path
    {
        ucLansCount = msReadByte(REG_11330F +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x07;
        usLockStatus = msRead2Byte(REG_11331A +DP_HBR2_REG_OFFSET(ucHBR2Select));
    }

    if((ucLansCount == DP_ONE_LANE) && (usLockStatus == 0x7))
    {
        bCDRLockFlag = TRUE;
    }
    else if((ucLansCount == DP_TWO_LANE) && (usLockStatus == 0x77))
    {
        bCDRLockFlag = TRUE;
    }
    else if((ucLansCount == DP_FOUR_LANE) && (usLockStatus == 0x7777))
    {
        bCDRLockFlag = TRUE;
    }

    return bCDRLockFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_SetFTSyntheizer()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_DP_SetFTSyntheizer(BYTE ucPortSelect, BYTE ucHBR2Select, BYTE ucRateType)
{    
    switch(ucRateType)
    {
        case DP_FAST_TRAINING_RATE_LBR:
            msWrite2Byte(REG_112004 +DP_PHY_REG_OFFSET(ucPortSelect), 0xAAAB);
            msWriteByte(REG_112006 +DP_PHY_REG_OFFSET(ucPortSelect), 0x2A);
            msWriteByte(REG_11330E +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x06);
            break;

        case DP_FAST_TRAINING_RATE_HBR:
            msWrite2Byte(REG_112004 +DP_PHY_REG_OFFSET(ucPortSelect), 0x3333);
            msWriteByte(REG_112006 +DP_PHY_REG_OFFSET(ucPortSelect), 0x33);
            msWriteByte(REG_11330E +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x0A);
            break;

        case DP_FAST_TRAINING_RATE_HBR2:
            msWrite2Byte(REG_112004 +DP_PHY_REG_OFFSET(ucPortSelect), 0xAA06);
            msWriteByte(REG_112006 +DP_PHY_REG_OFFSET(ucPortSelect), 0x19);
            msWriteByte(REG_11330E +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x14);
            break;

        default:
            
            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_SetFTLaneCount()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_DP_SetFTLaneCount(BYTE ucPortSelect, BYTE ucHBR2Select, BYTE ucLaneType)
{
    BYTE ucLaneCount = 0;
    
    switch(ucLaneType)
    {
        case DP_FAST_TRAINING_1LANE:
            ucLaneCount = 1;
            break;

        case DP_FAST_TRAINING_2LANE:
            ucLaneCount = 2;
            break;

        case DP_FAST_TRAINING_4LANE:
            ucLaneCount = 4;
            break;

        default:
            
            break;
    };
    
    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        ucPortSelect = 0;
    }
    else // HBR2 path
    {
        msWriteByte(REG_11330F +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x80 +ucLaneCount);
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_FTScrambleEnable()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_DP_FTScrambleEnable(BYTE ucPortSelect, BYTE ucHBR2Select, Bool bEnable)
{
    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        ucPortSelect = 0;
    }
    else // HBR2 path
    {
        msWriteByteMask(REG_113310 +DP_HBR2_REG_OFFSET(ucHBR2Select), bEnable? BIT5: 0, BIT5);
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_FastTrainingEnable()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_DP_FastTrainingEnable(BYTE ucPortSelect, BYTE ucHBR2Select, Bool bEnable)
{
    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        ucPortSelect = 0;
    }
    else // HBR2 path
    {
        if(bEnable)
        {
            msWriteByteMask(REG_1120D7 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x0E, 0x0E);
            msWriteByteMask(REG_1134F7 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT1| BIT3, BIT1| BIT3);
            msWriteByte(REG_11358E +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x5B);
        }
        else
        {
            msWriteByteMask(REG_1120D7 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x00, 0x0E);
            msWriteByteMask(REG_1134F7 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0, BIT1| BIT3);
            msWriteByte(REG_11358E +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x58);
        }
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_FTCheckTimingPrepare()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_DP_FTCheckTimingPrepare(BYTE ucPortSelect, BYTE ucHBR2Select)
{
    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        ucPortSelect = 0;
    }
    else // HBR2 path
    {
        msWriteByte(REG_113396 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0xFF);
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_AutoEQEnable()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_DP_AutoEQEnable(BYTE ucPortSelect, BYTE ucHBR2Select, Bool bEnable)
{
    if(ucHBR2Select != DP_HBR2_ENGINE_NONE) // HBR2 path
    {
        msWriteByteMask(REG_11204A +DP_PHY_REG_OFFSET(ucPortSelect), bEnable? BIT2: 0, BIT2);
        msWriteByteMask(REG_113000 +DP_HBR2_REG_OFFSET(ucHBR2Select), bEnable? BIT0: 0, BIT0);
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_FTAutoEQEnable()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mhal_DP_FTAutoEQEnable(BYTE ucPortSelect, BYTE ucHBR2Select, Bool bEnable)
{
    Bool bFTAutoEQEnable = FALSE;
#if(DP_FT_EQ_OFFSET_FUNCTION)	
	WORD EQ0=0, EQ1=0, EQ2=0, EQ3=0, EQ_CAL1=0, EQ_CAL2=0, EQ_CAL3=0;
#endif

    if(ucHBR2Select != DP_HBR2_ENGINE_NONE) // HBR2 path
    {
        if(bEnable)
        {
            msWrite2Byte(REG_1120DC +DP_PHY_REG_OFFSET(ucPortSelect), 0x0000);
    
            msWriteByteMask(REG_113000 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0, MASKBIT(5:2));
            msWriteByteMask(REG_113012 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT4 , BIT4);
    
            msWriteByteMask(REG_11204A +DP_PHY_REG_OFFSET(ucPortSelect), 0, BIT2);
            msWriteByteMask(REG_112051 +DP_PHY_REG_OFFSET(ucPortSelect), BIT1, MASKBIT(1:0));
    
            msWriteByteMask(REG_113000 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0 , BIT0);
            msWriteByteMask(REG_113000 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT1 , BIT1);
            msWriteByteMask(REG_113000 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT0 , BIT0);
    
            Delay1ms(1);
    
            msWriteByteMask(REG_11204A +DP_PHY_REG_OFFSET(ucPortSelect), BIT2 , BIT2);

            bFTAutoEQEnable = TRUE;
        }
        else
        {
            msWriteByteMask(REG_113000 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0 , BIT1);  
    
            msWriteByteMask(REG_113000 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT2, MASKBIT(5:2));
            msWriteByteMask(REG_113012 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0 , BIT4);
    
            msWriteByteMask(REG_112051 +DP_PHY_REG_OFFSET(ucPortSelect), MASKBIT(1:0), MASKBIT(1:0));
    
#if(DP_FT_EQ_OFFSET_FUNCTION)
            if((msReadByte(REG_1120E6 +DP_PHY_REG_OFFSET(ucPortSelect)) &0x1F) <= (0x1F -DP_FT_EQ_OFFSET_VALUE))
            {      
                EQ0 = (msReadByte(REG_1120E6 +DP_PHY_REG_OFFSET(ucPortSelect)) &0x1F) +DP_FT_EQ_OFFSET_VALUE;
            }
            else
            {
                EQ0 = 0x1F;
            }
                
            if((((msReadByte(REG_1120E6 +DP_PHY_REG_OFFSET(ucPortSelect)) &0xE0) >> 5) | ((msReadByte(REG_1120E7 +DP_PHY_REG_OFFSET(ucPortSelect)) &0x03) << 3)) <= (0x1F -DP_FT_EQ_OFFSET_VALUE))
            {
                EQ1 = (((msReadByte(REG_1120E6 +DP_PHY_REG_OFFSET(ucPortSelect)) &0xE0) >> 5) | ((msReadByte(REG_1120E7 +DP_PHY_REG_OFFSET(ucPortSelect)) &0x03) << 3)) +DP_FT_EQ_OFFSET_VALUE;
            }
            else
            {
                EQ1 =0x1F;
            }
                
            if(((msReadByte(REG_1120E7 +DP_PHY_REG_OFFSET(ucPortSelect)) &0x7C) >> 2) <= (0x1F -DP_FT_EQ_OFFSET_VALUE))
            {
                EQ2 = ((msReadByte(REG_1120E7 +DP_PHY_REG_OFFSET(ucPortSelect)) &0x7C) >> 2) +DP_FT_EQ_OFFSET_VALUE;
            }
            else
            {
                EQ2 =0x1F;
            }
                
            if((msReadByte(REG_1120E8 +DP_PHY_REG_OFFSET(ucPortSelect)) &0x1F) <= (0x1F -DP_FT_EQ_OFFSET_VALUE))
            {
                EQ3 = (msReadByte(REG_1120E8 +DP_PHY_REG_OFFSET(ucPortSelect)) &0x1F) +DP_FT_EQ_OFFSET_VALUE;
            }
            else
            {
                EQ3 =0x1F;
            }
    
            EQ_CAL1 = (((EQ1 & 0x07) << 5) | (EQ0 & 0x1F));
            msWriteByte(REG_1120E6 +DP_PHY_REG_OFFSET(ucPortSelect), EQ_CAL1);
    
            EQ_CAL2 = (((EQ2 & 0x1F) << 2) | ((EQ1 & 0x18) >> 3));
            msWriteByte(REG_1120E7 +DP_PHY_REG_OFFSET(ucPortSelect), EQ_CAL2);
    
            EQ_CAL3 = (EQ3 & 0x1F);
            msWriteByte(REG_1120E8 +DP_PHY_REG_OFFSET(ucPortSelect), EQ_CAL3);
    
            msWrite2Byte(REG_1120DC +DP_PHY_REG_OFFSET(ucPortSelect), 0x03C0);
        
#endif
        }
    }

    return bFTAutoEQEnable;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_FTAutoEQCheckDown()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_DP_FTAutoEQCheckDown(BYTE ucPortSelect, BYTE ucHBR2Select)
{
    if((msReadByte(REG_112083 +DP_PHY_REG_OFFSET(ucPortSelect)) &0x01) == 0x01)
	{
		Delay1ms(5);

		if((msReadByte(REG_112083 +DP_PHY_REG_OFFSET(ucPortSelect)) &0x01) == 0x01)
		{
			mhal_DP_FTAutoEQEnable(ucPortSelect, ucHBR2Select, FALSE);
		}
	}
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_SplitSwitch()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mhal_DP_SplitSwitch(BYTE ucHBR2Select, Bool bEnable)
{
    Bool bSplitEnable = FALSE;
    BYTE ucSplitSelect = DP_SPLIT_MODE_1P;
    static BYTE ucPreSplitSelect = DP_SPLIT_MODE_MASK;

    if(bEnable)
    {
        bSplitEnable = TRUE;
        ucSplitSelect = DP_SPLIT_MODE_2P;
    }

    if(ucSplitSelect != ucPreSplitSelect)
    {
        _mhal_DP_SplitSelect(ucSplitSelect, ucHBR2Select);
    }
    
    return bSplitEnable;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_UpdateMVIDValue()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_DP_UpdateMVIDValue(BYTE ucPortSelect, BYTE ucHBR2Select)
{
    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        ucPortSelect = 0;
    }
    else // HBR2 path
    {
        msWriteByte(REG_1133A8 +DP_HBR2_REG_OFFSET(ucHBR2Select), msReadByte(REG_11338C +DP_HBR2_REG_OFFSET(ucHBR2Select)));
        msWriteByte(REG_1133A7 +DP_HBR2_REG_OFFSET(ucHBR2Select), msReadByte(REG_11338B +DP_HBR2_REG_OFFSET(ucHBR2Select)));
        msWriteByte(REG_1133A6 +DP_HBR2_REG_OFFSET(ucHBR2Select), msReadByte(REG_11338A +DP_HBR2_REG_OFFSET(ucHBR2Select)));
        msWriteByte(REG_11339E +DP_HBR2_REG_OFFSET(ucHBR2Select), msReadByte(REG_113386 +DP_HBR2_REG_OFFSET(ucHBR2Select)));
        msWriteByte(REG_11339F +DP_HBR2_REG_OFFSET(ucHBR2Select), msReadByte(REG_113387 +DP_HBR2_REG_OFFSET(ucHBR2Select)));
        msWriteByte(REG_1133A0 +DP_HBR2_REG_OFFSET(ucHBR2Select), msReadByte(REG_113388 +DP_HBR2_REG_OFFSET(ucHBR2Select)));
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_GetTimingPixelClock()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  Pixel clock = xxx MHz
//**************************************************************************
WORD mhal_DP_GetTimingPixelClock(BYTE ucPortSelect, BYTE ucHBR2Select)
{
    WORD usPixelClock = 0;
    WORD usLSClock = _mhal_DP_GetLinkRate(ucPortSelect, ucHBR2Select);
    DWORD ulBaseMValue = 0;
    DWORD ulBaseNValue = 0;
    
    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        ucPortSelect = 0;
    }
    else // HBR2 path
    {
        ulBaseMValue = ((DWORD)msRead2Byte(REG_113387 +DP_HBR2_REG_OFFSET(ucHBR2Select)) <<8) + msReadByte(REG_113386 +DP_HBR2_REG_OFFSET(ucHBR2Select));
        ulBaseNValue = ((DWORD)msRead2Byte(REG_11338B +DP_HBR2_REG_OFFSET(ucHBR2Select)) <<8) + msReadByte(REG_11338A +DP_HBR2_REG_OFFSET(ucHBR2Select));
    }

    if(ulBaseNValue > 0)
    {
        usPixelClock = ((DWORD)ulBaseMValue *usLSClock) /ulBaseNValue;
    }

    return usPixelClock;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_GetTimingPixelClock_10KHz()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  Pixel clock = XXX x 10KHz, not MHz
//**************************************************************************
WORD mhal_DP_GetTimingPixelClock_10KHz(BYTE ucPortSelect, BYTE ucHBR2Select)
{
    WORD usPixelClock = 0;
    WORD usLSClock = _mhal_DP_GetLinkRate(ucPortSelect, ucHBR2Select);
    DWORD ulBaseMValue = 0;
    DWORD ulBaseNValue = 0;
    
    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        ucPortSelect = 0;
    }
    else // HBR2 path
    {
        ulBaseMValue = ((DWORD)msRead2Byte(REG_113387 +DP_HBR2_REG_OFFSET(ucHBR2Select)) <<8) + msReadByte(REG_113386 +DP_HBR2_REG_OFFSET(ucHBR2Select));
        ulBaseNValue = ((DWORD)msRead2Byte(REG_11338B +DP_HBR2_REG_OFFSET(ucHBR2Select)) <<8) + msReadByte(REG_11338A +DP_HBR2_REG_OFFSET(ucHBR2Select));
    }

    if(ulBaseNValue > 0)
    {
        usPixelClock = ((DWORD)ulBaseMValue *usLSClock *100) /ulBaseNValue;
    }

    return usPixelClock;
}


//**************************************************************************
//  [Function Name]:
//                  mhal_DP_GetTimingInformation()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mhal_DP_GetTimingInformation(BYTE ucPortSelect, BYTE ucHBR2Select, WORD *usTimingInfo)
{
    Bool bInterlace = FALSE;
    
    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        ucPortSelect = 0;
    }
    else // HBR2 path
    {
        bInterlace = ((msReadByte(REG_1133A5 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &BIT2) ?TRUE: FALSE);
        
        usTimingInfo[0] = msRead2Byte(REG_113376 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x7FFF; // H total
        usTimingInfo[1] = msRead2Byte(REG_113378 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x7FFF; // V total
        usTimingInfo[2] = msRead2Byte(REG_11337A +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x7FFF; // H width
        usTimingInfo[3] = msRead2Byte(REG_11337C +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x7FFF; // V width
        usTimingInfo[4] = msRead2Byte(REG_11337E +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x7FFF; // H start
        usTimingInfo[5] = msRead2Byte(REG_113380 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x7FFF; // V start
        usTimingInfo[6] = msRead2Byte(REG_113382 +DP_HBR2_REG_OFFSET(ucHBR2Select)); // H PWS
        usTimingInfo[7] = msRead2Byte(REG_113384 +DP_HBR2_REG_OFFSET(ucHBR2Select)); // V PWS
    }

    return bInterlace;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_RegenTimingInformation()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_DP_RegenTimingInformation(BYTE ucPortSelect, BYTE ucHBR2Select, WORD *usTimingInfo)
{
    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        ucPortSelect = 0;
    }
    else // HBR2 path
    {
        msWrite2Byte(REG_11350A +DP_HBR2_REG_OFFSET(ucHBR2Select), msRead2Byte(REG_11350A +DP_HBR2_REG_OFFSET(ucHBR2Select))&0x7FFF);
        msWriteByteMask(REG_113508 +DP_HBR2_REG_OFFSET(ucHBR2Select), MASKBIT(1:0), MASKBIT(1:0));
        // V parameter
        msWrite2Byte(REG_11350C +DP_HBR2_REG_OFFSET(ucHBR2Select), (usTimingInfo[0] &0x7FFF)); // V width
        msWriteByte(REG_113540 +DP_HBR2_REG_OFFSET(ucHBR2Select), usTimingInfo[1]); // V front porch
        msWriteByte(REG_1135A7 +DP_HBR2_REG_OFFSET(ucHBR2Select), (usTimingInfo[2]) << 1); // V back porch
        msWriteByteMask(REG_113501 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT4, BIT4);
        // H parameter
        msWrite2Byte(REG_11350A +DP_HBR2_REG_OFFSET(ucHBR2Select), usTimingInfo[3]); // H width
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_GetPacketStatus()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
WORD mhal_DP_GetPacketStatus(BYTE ucPortSelect, BYTE ucHBR2Select)
{
    WORD usPacketStatus = 0;
    
    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        ucPortSelect = 0;
    }
    else // HBR2 path
    {
        usPacketStatus = msRead2Byte(REG_1132AA +DP_HBR2_REG_OFFSET(ucHBR2Select));
        
        msWrite2Byte(REG_1132AC +DP_HBR2_REG_OFFSET(ucHBR2Select), 0xFFFF); 
		msWrite2Byte(REG_1132AC +DP_HBR2_REG_OFFSET(ucHBR2Select), 0); 
    }

    return usPacketStatus;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_GetColorSpace()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE mhal_DP_GetColorSpace(BYTE ucPortSelect, BYTE ucHBR2Select, WORD *usMISCInfo)
{
    BYTE usColorSpace = DP_COLOR_FORMAT_UNKNOWN;
    
    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        ucPortSelect = 0;
    }
    else // HBR2 path
    {
        msWriteByteMask(REG_1133BC +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT6, BIT6);
        usMISCInfo[1] = msRead2Byte(REG_113359 +DP_HBR2_REG_OFFSET(ucHBR2Select)); // MISC1
        msWriteByteMask(REG_1133BC +DP_HBR2_REG_OFFSET(ucHBR2Select), 0, BIT6);
        usMISCInfo[0] = msRead2Byte(REG_113359 +DP_HBR2_REG_OFFSET(ucHBR2Select)); // MISC0
    }

    usColorSpace = (usMISCInfo[0] &MASKBIT(2:1)) >> 1;

    switch(usColorSpace)
    {
        case 0:
            usColorSpace = DP_COLOR_FORMAT_RGB;
            break;

        case 1:
            usColorSpace = DP_COLOR_FORMAT_YUV_422;
            break;

        case 2:
            usColorSpace = DP_COLOR_FORMAT_YUV_444;
            break;

        default:
            
            break;
    };

    return usColorSpace;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_GetTrainingPatternFlag()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mhal_DP_GetTrainingPatternFlag(BYTE ucPortSelect, BYTE ucHBR2Select)
{
    Bool bTrainingFlag = FALSE;
    BYTE ucTrainingState = 0;
    
    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        ucPortSelect = 0;
    }
    else // HBR2 path
    {
        ucTrainingState = msReadByte(REG_113310 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &MASKBIT(1:0);
    }

    if(ucTrainingState != 0)
    {
        bTrainingFlag = TRUE;
    }

    return bTrainingFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_CheckReceiveIdlePattern()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mhal_DP_CheckReceiveIdlePattern(BYTE ucPortSelect, BYTE ucHBR2Select)
{
    Bool bIdlePatternFlag = FALSE;
    
    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        ucPortSelect = 0;
    }
    else // HBR2 path
    {
        bIdlePatternFlag = ((msReadByte(REG_1133A5 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &BIT3) ?TRUE: FALSE);
    }

    return bIdlePatternFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_CheckDPCDPowerState()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mhal_DP_CheckDPCDPowerState(BYTE ucPortSelect)
{
    Bool bPowerStateFlag = ((msReadByte(REG_001172 +DP_PHY_REG_OFFSET(ucPortSelect)) &BIT0) ?TRUE: FALSE);

    return bPowerStateFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_GetHDCPFailFlag()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mhal_DP_GetHDCPFailFlag(BYTE ucPortSelect, BYTE ucHBR2Select)
{
    Bool bEventFlag = FALSE;
    
    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        ucPortSelect = 0;
    }
    else // HBR2 path
    {
        bEventFlag = ((msReadByte(REG_113374 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &BIT2) ?TRUE: FALSE);
    }

    return bEventFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_CheckLossCDRStatus()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mhal_DP_CheckLossCDRStatus(BYTE ucPortSelect, BYTE ucHBR2Select)
{
    Bool bEventFlag = FALSE;
    
    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        ucPortSelect = 0;
    }
    else // HBR2 path
    {
        if((msReadByte(REG_11335A +DP_HBR2_REG_OFFSET(ucHBR2Select)) &MASKBIT(1:0)) != 0x2)
        {
            bEventFlag = TRUE;

            msWriteByteMask(REG_113000 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT1, BIT1);
            msWriteByteMask(REG_113000 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0, BIT1);
            msWriteByte(REG_11331C +DP_HBR2_REG_OFFSET(ucHBR2Select), 0);
            msWriteByte(REG_11331C +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT7);
        }
    }

    return bEventFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_CheckInterlaneSkewStatus()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mhal_DP_CheckInterlaneSkewStatus(BYTE ucPortSelect, BYTE ucHBR2Select)
{
    Bool bEventFlag = FALSE;
    
    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        ucPortSelect = 0;
    }
    else // HBR2 path
    {
        if((msReadByte(REG_11331C +DP_HBR2_REG_OFFSET(ucHBR2Select)) &BIT0) != BIT0)
        {
            if((msReadByte(REG_11335A +DP_HBR2_REG_OFFSET(ucHBR2Select)) &MASKBIT(1:0)) != 0x2)
            {
                bEventFlag = TRUE;

                msWriteByte(REG_11331C +DP_HBR2_REG_OFFSET(ucHBR2Select), 0);
                msWriteByte(REG_11331C +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT7);
            }
        }
    }

    return bEventFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_GetMCCSReceiveFlag()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mhal_DP_GetMCCSReceiveFlag(BYTE ucPortSelect)
{
    Bool bEventFlag = ((msReadByte(REG_001178 +DP_PHY_REG_OFFSET(ucPortSelect)) &BIT7) ?TRUE: FALSE);
    
    if(bEventFlag)
    {
        _mhal_DP_MCCSReceiveProc(ucPortSelect);

        // Clear int, then trigger HW send ACK and clear DDC buffer
        msWriteByteMask(REG_001179 +DP_PHY_REG_OFFSET(ucPortSelect), BIT7, BIT7);
        msWriteByteMask(REG_001179 +DP_PHY_REG_OFFSET(ucPortSelect), 0, BIT7);
    }

    return bEventFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_GetProgramDPCD0Flag()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mhal_DP_GetProgramDPCD0Flag(BYTE ucPortSelect)
{
    Bool bEventFlag = ((msReadByte(REG_001190 +DP_PHY_REG_OFFSET(ucPortSelect)) &BIT0) ?TRUE: FALSE);
    
    if(bEventFlag)
    {
        // Clear int, then trigger HW send ACK and clear DDC buffer
        //msWriteByteMask(REG_001192 +DP_PHY_REG_OFFSET(ucPortSelect), BIT0, BIT0);
        //msWriteByteMask(REG_001192 +DP_PHY_REG_OFFSET(ucPortSelect), 0, BIT0);
    }

    return bEventFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_GetProgramDPCD1Flag()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mhal_DP_GetProgramDPCD1Flag(BYTE ucPortSelect)
{
    Bool bEventFlag = ((msReadByte(REG_001190 +DP_PHY_REG_OFFSET(ucPortSelect)) &BIT1) ?TRUE: FALSE);
    
    if(bEventFlag)
    {
        // Clear int, then trigger HW send ACK and clear DDC buffer
        //msWriteByteMask(REG_001192 +DP_PHY_REG_OFFSET(ucPortSelect), BIT1, BIT1);
        //msWriteByteMask(REG_001192 +DP_PHY_REG_OFFSET(ucPortSelect), 0, BIT1);
    }

    return bEventFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_GetAuxReceiveFlag()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mhal_DP_GetAuxReceiveFlag(BYTE ucPortSelect)
{
    Bool bEventFlag = ((msReadByte(REG_001178 +DP_PHY_REG_OFFSET(ucPortSelect)) &BIT4) ?TRUE: FALSE);
    
    if(bEventFlag)
    {
        msWriteByteMask(REG_001179 +DP_PHY_REG_OFFSET(ucPortSelect), BIT4, BIT4);
        msWriteByteMask(REG_001179 +DP_PHY_REG_OFFSET(ucPortSelect), 0, BIT4);
    }

    return bEventFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_ProgramDPCD0Proc()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mhal_DP_ProgramDPCD0Proc(BYTE ucPortSelect, BYTE *ucDataQueue)
{
    Bool bDPCDWrite = FALSE;
    BYTE uctemp = 0;
    BYTE ucAuxCommand = msReadByte(REG_001138 +DP_PHY_REG_OFFSET(ucPortSelect))&0x0F;
    BYTE ucAuxLength = msReadByte(REG_00113A +DP_PHY_REG_OFFSET(ucPortSelect)) +1;

    msWriteByteMask(REG_001130 +DP_PHY_REG_OFFSET(ucPortSelect), BIT1, BIT1);
    
    if(ucAuxCommand == 0x09)
    {
        if(ucAuxLength <= DP_AUX_COMMAND_MAX_LENGTH)
        {
            for(uctemp = 0; uctemp <(ucAuxLength -1); uctemp++) // Can't write 16 data
            {
                msWriteByte(REG_0011D3 +DP_PHY_REG_OFFSET(ucPortSelect), ucDataQueue[uctemp]);
            }
        }
    }
    else if((ucAuxCommand == 0x08) || (ucAuxCommand == 0x04))
    {
        uctemp = _mhal_DP_GetAuxDDCData(ucPortSelect);

        if(ucAuxLength <= DP_AUX_COMMAND_MAX_LENGTH)
        {
            for(uctemp = 0; uctemp <ucAuxLength; uctemp++)
            {
                ucDataQueue[uctemp] = _mhal_DP_GetAuxDDCData(ucPortSelect);
            }
        }

        bDPCDWrite = TRUE;
    }

    msWriteByteMask(REG_001130 +DP_PHY_REG_OFFSET(ucPortSelect), 0, BIT1);

    // Clear int, then trigger HW send ACK and clear DDC buffer
    msWriteByteMask(REG_001192 +DP_PHY_REG_OFFSET(ucPortSelect), BIT0, BIT0);
    msWriteByteMask(REG_001192 +DP_PHY_REG_OFFSET(ucPortSelect), 0, BIT0);

    return bDPCDWrite;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_ProgramDPCD1Proc()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mhal_DP_ProgramDPCD1Proc(BYTE ucPortSelect, BYTE *ucDataQueue)
{
    Bool bDPCDWrite = FALSE;
    BYTE uctemp = 0;
    BYTE ucAuxCommand = msReadByte(REG_001138 +DP_PHY_REG_OFFSET(ucPortSelect))&0x0F;
    BYTE ucAuxLength = msReadByte(REG_00113A +DP_PHY_REG_OFFSET(ucPortSelect)) +1;

    msWriteByteMask(REG_001130 +DP_PHY_REG_OFFSET(ucPortSelect), BIT1, BIT1);
    
    if(ucAuxCommand == 0x09)
    {
        if(ucAuxLength <= DP_AUX_COMMAND_MAX_LENGTH)
        {
            for(uctemp = 0; uctemp <(ucAuxLength -1); uctemp++) // Can't write 16 data
            {
                //msWriteByte(REG_0011D3 +DP_PHY_REG_OFFSET(ucPortSelect), ucDataQueue[uctemp]);
            }
        }
    }
    else if((ucAuxCommand == 0x08) || (ucAuxCommand == 0x04))
    {
        ucDataQueue[0] = _mhal_DP_GetAuxDDCData(ucPortSelect);

        if(ucAuxLength <= DP_AUX_COMMAND_MAX_LENGTH)
        {
            for(uctemp = 0; uctemp <ucAuxLength; uctemp++)
            {
                //msWriteByteMask(REG_0011FC +DP_PHY_REG_OFFSET(ucPortSelect), BIT2, BIT2);
                //_mhal_DP_AuxReadDebounce(255);
                //ucDataQueue[uctemp] = msReadByte(REG_001175 +DP_PHY_REG_OFFSET(ucPortSelect));
            }
        }

        bDPCDWrite = TRUE;
    }

    msWriteByteMask(REG_001130 +DP_PHY_REG_OFFSET(ucPortSelect), 0, BIT1);

    // Clear int, then trigger HW send ACK and clear DDC buffer
    msWriteByteMask(REG_001192 +DP_PHY_REG_OFFSET(ucPortSelect), BIT1, BIT1);
    msWriteByteMask(REG_001192 +DP_PHY_REG_OFFSET(ucPortSelect), 0, BIT1);

    return bDPCDWrite;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_GetAUPLLBigChangeFlag()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mhal_DP_GetAUPLLBigChangeFlag(BYTE ucPortSelect, BYTE ucHBR2Select)
{
    Bool bEventFlag = FALSE;
    
    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        ucPortSelect = 0;
    }
    else // HBR2 path
    {
        bEventFlag = ((msReadByte(REG_1133EB +DP_HBR2_REG_OFFSET(ucHBR2Select)) &BIT1) ?TRUE: FALSE); 

        if(bEventFlag)
        {
            msWriteByteMask(REG_1133EB +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT1, BIT1); 
        }
    }

    return bEventFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_GetInterlaneSkewFlag()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mhal_DP_GetInterlaneSkewFlag(BYTE ucPortSelect, BYTE ucHBR2Select)
{
    Bool bEventFlag = FALSE;
    
    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        ucPortSelect = 0;
    }
    else // HBR2 path
    {
        bEventFlag = ((msReadByte(REG_1133EA +DP_HBR2_REG_OFFSET(ucHBR2Select)) &BIT3) ?TRUE: FALSE); 

        if(bEventFlag)
        {
            msWriteByteMask(REG_1133EA +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT3, BIT3); 
        }
    }

    return bEventFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_GetCDRLossLockFlag()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mhal_DP_GetCDRLossLockFlag(BYTE ucPortSelect, BYTE ucHBR2Select)
{
    Bool bEventFlag = FALSE;
    
    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        ucPortSelect = 0;
    }
    else // HBR2 path
    {
        bEventFlag = ((msReadByte(REG_1133E9 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &BIT0) ?TRUE: FALSE); 

        if(bEventFlag)
        {
            msWriteByteMask(REG_1133E9 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT0, BIT0); 
        }
    }

    return bEventFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_GetVPLLBigChangeFlag()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mhal_DP_GetVPLLBigChangeFlag(BYTE ucPortSelect, BYTE ucHBR2Select)
{
    Bool bEventFlag = FALSE;
    
    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        ucPortSelect = 0;
    }
    else // HBR2 path
    {
        bEventFlag = ((msReadByte(REG_1133EB +DP_HBR2_REG_OFFSET(ucHBR2Select)) &BIT3) ?TRUE: FALSE); 

        if(bEventFlag)
        {
            msWriteByteMask(REG_1133EB +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT3, BIT3); 
        }
    }

    return bEventFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_GetTrainingPattern1Flag()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mhal_DP_GetTrainingPattern1Flag(BYTE ucPortSelect, BYTE ucHBR2Select)
{
    Bool bEventFlag = FALSE;
    BYTE uctemp = 0;
    BYTE ucSwingLevel = 0;
    WORD usTP1Count = 0;
    
    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        ucPortSelect = 0;
    }
    else // HBR2 path
    {
        bEventFlag = ((msReadByte(REG_1133E8 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &BIT0) ?TRUE: FALSE); 

        if(bEventFlag || (msReadByte(REG_1133E4 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &BIT0))
        {
            if(msRegs(REG_11330E +DP_HBR2_REG_OFFSET(ucHBR2Select)) == 0x14) 
            {
                mhal_DP_SquelchLevelSetting(ucPortSelect, 0xF);
            }
            else
            {
                mhal_DP_SquelchLevelSetting(ucPortSelect, 0x4);
            }

            msWriteByte(REG_11331E +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x00);
            msWriteByte(REG_11331F +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x00);

            msWriteByteMask(REG_11357F +DP_HBR2_REG_OFFSET(ucHBR2Select), 0, BIT0); //disable Reset Word Boundary
            
#if(DP_AUTO_EQ_ENABLE)
            mhal_DP_FTAutoEQEnable(ucPortSelect, ucHBR2Select, FALSE);

            msWriteByteMask(REG_113000 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0, BIT2);

            mhal_DP_AutoEQEnable(ucPortSelect, ucHBR2Select, FALSE);
            mhal_DP_AutoEQEnable(ucPortSelect, ucHBR2Select, TRUE);

            msWriteByteMask(REG_113310 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0, BIT0);

            for(uctemp = 0; uctemp < 5; uctemp++)
            {
                _nop_();
            }
            
            msWriteByteMask(REG_113310 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT0, BIT0);

            msWriteByteMask(REG_1133E0 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0, BIT1); //enable TPS2 interrupt
#endif

            while(((msReadByte(REG_113310 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x03) == 0x01) && (usTP1Count < 50000)) // while pattern 1 is On-Going
            {
                usTP1Count++;
                
                switch(msReadByte(REG_11330F +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x07)
                {
                    case 0x01:  // 1 lane
                        if((msReadByte(REG_113311 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x03) == (msReadByte(REG_11331E +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x03)) // request reach
                        {
                            if((msReadByte(REG_11331A +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x01) != 0x01) // 1 lane LOCK Fail
                            {
                                if(ucSwingLevel < 3)
                                {
                                    ucSwingLevel++; // update swing level
                                }
                                    
                                msWriteByteMask(REG_11331E +DP_HBR2_REG_OFFSET(ucHBR2Select), ucSwingLevel, 0x03);
                                msWriteByteMask(REG_11331C +DP_HBR2_REG_OFFSET(ucHBR2Select), 0, BIT7);
                                msWriteByteMask(REG_11331C +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT7, BIT7);
                                
#if(DP_AUTO_EQ_ENABLE &&(MS_BOARD_TYPE_SEL == BD_MST182A_A01A_S))
                                msWriteByteMask(REG_11331E +DP_HBR2_REG_OFFSET(ucHBR2Select), ucSwingLevel, 0x03);
#endif
                            }
                        }
                        
                        break;

                    case 0x02:  // 2 lanes
                        if((msReadByte(REG_113311 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x03) == (msReadByte(REG_11331E +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x03)) // request reach
                        {
                            if((msReadByte(REG_113312 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x03) == ((msReadByte(REG_11331E +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x30) >> 4)) // request reach
                            {
                                if((msReadByte(REG_11331A +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x11) != 0x11) // 2 lanes CDR LOCK Fail
                                {
                                    if(ucSwingLevel < 3)
                                    {
                                        ucSwingLevel++; // update swing level
                                    }
                                    
                                    msWriteByteMask(REG_11331E +DP_HBR2_REG_OFFSET(ucHBR2Select), (ucSwingLevel << 4) |ucSwingLevel, 0x33);
                                    msWriteByteMask(REG_11331C +DP_HBR2_REG_OFFSET(ucHBR2Select), 0, BIT7);
                                    msWriteByteMask(REG_11331C +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT7, BIT7);
                                    
#if(DP_AUTO_EQ_ENABLE &&(MS_BOARD_TYPE_SEL == BD_MST182A_A01A_S))
                                    msWriteByteMask(REG_11331E +DP_HBR2_REG_OFFSET(ucHBR2Select), (2 << 4) |2, 0x33);
#endif
                                }
                            }
                        }
                        
                        break;

                    case 0x04:  // 4 lanes
                        if((msReadByte(REG_113311 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x03) == (msReadByte(REG_11331E +DP_HBR2_REG_OFFSET(ucHBR2Select)) & 0x03)) // request reach
                        {
                            if((msReadByte(REG_113312 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x03) == ((msReadByte(REG_11331E +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x30) >> 4)) // request reach
                            {
                                if((msReadByte(REG_113313 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x03) == (msReadByte(REG_11331F +DP_HBR2_REG_OFFSET(ucHBR2Select)) & 0x03)) // request reach
                                {
                                    if((msReadByte(REG_113314 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x03) == ((msReadByte(REG_11331F +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x30) >> 4)) // request reach
                                    {
                                        if(((msReadByte(REG_11331A +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x11) != 0x11) | ((msReadByte(REG_11331B +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x11) != 0x11)) // 4 lanes CDR LOCK Fail
                                        {
                                            if(ucSwingLevel < 3)
                                            {
                                                ucSwingLevel++; // update swing level
                                            }
                                            
                                            msWriteByteMask(REG_11331E +DP_HBR2_REG_OFFSET(ucHBR2Select), (ucSwingLevel << 4) |ucSwingLevel, 0x33);
                                            msWriteByteMask(REG_11331F +DP_HBR2_REG_OFFSET(ucHBR2Select), (ucSwingLevel << 4) |ucSwingLevel, 0x33);
                                            msWriteByteMask(REG_11331C +DP_HBR2_REG_OFFSET(ucHBR2Select), 0, BIT7);
                                            msWriteByteMask(REG_11331C +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT7, BIT7);
                                    
#if (DP_AUTO_EQ_ENABLE &&(MS_BOARD_TYPE_SEL == BD_MST182A_A01A_S))
                                            msWriteByteMask(REG_11331E +DP_HBR2_REG_OFFSET(ucHBR2Select), (2 << 4) |2, 0x33);
                                            msWriteByteMask(REG_11331F +DP_HBR2_REG_OFFSET(ucHBR2Select), (2 << 4) |2, 0x33);
#endif
                                        }
                                    }
                                }
                            }
                        }
                        
                        break;

                    default:
                        
                        break;
                }
            }
     
            msWriteByteMask(REG_1133E8 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT0, BIT0); 
        }
    }

    return bEventFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_GetTrainingPattern2Flag()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mhal_DP_GetTrainingPattern2Flag(BYTE ucPortSelect, BYTE ucHBR2Select)
{
    Bool bTP2Flag = FALSE;
    Bool bTP3Flag = FALSE;
    WORD usTP2Count = 0;
    
    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        ucPortSelect = 0;
    }
    else // HBR2 path
    {
        bTP2Flag = ((msReadByte(REG_1133E8 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &BIT1) ?TRUE: FALSE); 
        bTP3Flag = ((msReadByte(REG_1133EA +DP_HBR2_REG_OFFSET(ucHBR2Select)) &BIT1) ?TRUE: FALSE);

        if(bTP2Flag || bTP3Flag || (msRegs(REG_1133E4 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &BIT1) || (msRegs(REG_1133E6 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &BIT1))
        {
            
#if(DP_AUTO_EQ_ENABLE)
            BYTE AEQ_done = 0;

#if(DP_EQ_OFFSET_FUNCTION)
            BYTE lane_sel = 0, AEQ_flag = 0;
#endif

#if(DP_ERROR_COUNT_FUNCTION || DP_ENABLE_STABLE_COUNT)
            msWriteByteMask(REG_1135C1 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT3, MASKBIT(3:2));
            
#endif
            
#if(DP_EQ_OFFSET_FUNCTION)
            msWriteByte(REG_1120DC +DP_PHY_REG_OFFSET(ucPortSelect), 0x00);
            msWriteByte(REG_1120DD +DP_PHY_REG_OFFSET(ucPortSelect), 0x00);
            
#endif
            
            while((((msReadByte(REG_113310 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x03) == 0x02)||((msReadByte(REG_113310 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x03) == 0x03)) && (usTP2Count < 50000) && (AEQ_done == 0))
            {
                usTP2Count++;
    
#if(DP_DYNAMIC_PREEMPHASIS)
    
                WORD LANE0_EQ = 0, LANE1_EQ = 0, LANE2_EQ = 0, LANE3_EQ = 0;
    
                if(((msReadByte(REG_113311 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x08) == 0x08) && ((msReadByte(REG_112083 +DP_PHY_REG_OFFSET(ucPortSelect)) &0x01) == 0x01))
                {
                    LANE0_EQ = (msReadByte(REG_1120E6 +DP_PHY_REG_OFFSET(ucPortSelect)) &0x1F);
                    LANE1_EQ = (((msReadByte(REG_1120E6 +DP_PHY_REG_OFFSET(ucPortSelect)) &0xE0) >> 5) | ((msReadByte(REG_1120E7 +DP_PHY_REG_OFFSET(ucPortSelect)) &0x03) << 3));
                    LANE2_EQ = ((msReadByte(REG_1120E7 +DP_PHY_REG_OFFSET(ucPortSelect)) &0x7C) >> 2);
                    LANE3_EQ = (msReadByte(REG_1120E8 +DP_PHY_REG_OFFSET(ucPortSelect)) &0x1F);
    
                    if((LANE0_EQ >= DP_EQ_THRESHOLD) || (LANE1_EQ >= DP_EQ_THRESHOLD) || (LANE2_EQ >= DP_EQ_THRESHOLD) || (LANE3_EQ >= DP_EQ_THRESHOLD))
                    {
                        msWriteByteMask(REG_113012 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT4, BIT4);
                    }
                }
            
#endif
            
#if (DP_EQ_OFFSET_FUNCTION || DP_ERROR_COUNT_FUNCTION || DP_ENABLE_STABLE_COUNT)
                switch(msReadByte(REG_11330F +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x07)
                {
                    case 0x01:  // 1 lane
                        msWriteByteMask(REG_113001 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0, MASKBIT(1:0));
    
                        if((msReadByte(REG_113014 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x80) == 0x80)
                        {
                            AEQ_done = 0x01;
                        }
    
                        break;
    
                    case 0x02:  // 2 lanes
                        for(lane_sel = 0; lane_sel <2; lane_sel++)
                        {
                            msWriteByteMask(REG_113001 +DP_HBR2_REG_OFFSET(ucHBR2Select), lane_sel, MASKBIT(1:0));
    
                            if(lane_sel == 0)
                            {
                                if((msReadByte(REG_113014 +DP_HBR2_REG_OFFSET(ucHBR2Select)) & 0x80) == 0x80)
                                {
                                    AEQ_flag |= 0x01;
                                }
                            }
                            else if(lane_sel == 1)
                            {
                                if((msReadByte(REG_113014 +DP_HBR2_REG_OFFSET(ucHBR2Select)) & 0x80) == 0x80)
                                {
                                    AEQ_flag |= 0x02;
                                }
                            }
                        }
    
                        if(AEQ_flag == 0x03)
                        {
                            AEQ_done = 0x01;
                        }
    
                        break;
    
                    case 0x04:  // 4 lanes
                        for(lane_sel = 0; lane_sel <4; lane_sel++)
                        {
                            msWriteByteMask(REG_113001 +DP_HBR2_REG_OFFSET(ucHBR2Select), lane_sel, MASKBIT(1:0));
    
                            if(lane_sel == 0)
                            {
                                if((msReadByte(REG_113014 +DP_HBR2_REG_OFFSET(ucHBR2Select)) & 0x80) == 0x80)
                                {
                                    AEQ_flag |= 0x01;
                                }
                            }
                            else if(lane_sel == 1)
                            {
                                if((msReadByte(REG_113014 +DP_HBR2_REG_OFFSET(ucHBR2Select)) & 0x80) == 0x80)
                                {
                                    AEQ_flag |= 0x02;
                                }
                            }
                            else if(lane_sel == 2)
                            {
                                if((msReadByte(REG_113014 +DP_HBR2_REG_OFFSET(ucHBR2Select)) & 0x80) == 0x80)
                                {
                                    AEQ_flag |= 0x04;
                                }
                            }
                            else if(lane_sel == 3)
                            {
                                if((msReadByte(REG_113014 +DP_HBR2_REG_OFFSET(ucHBR2Select)) & 0x80) == 0x80)
                                {
                                    AEQ_flag |= 0x08;
                                }
                            }
                        }
    
                        if(AEQ_flag == 0x0F)
                        {
                            AEQ_done = 0x01;
                        }
    
                        break;
    
                    default:
                        AEQ_done = 0x00;
    
                        break;
                };
            
#endif
            }             
            
#if(DP_EQ_OFFSET_FUNCTION)
            WORD EQ0=0, EQ1=0, EQ2=0, EQ3=0, EQ_CAL1=0, EQ_CAL2=0, EQ_CAL3=0;
    
            if(((msReadByte(REG_113310 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x03) == 0x02)&&(AEQ_done == 1))
            {
                if((msReadByte(REG_1120E6 +DP_PHY_REG_OFFSET(ucPortSelect)) & 0x1F) <= (0x1F -DP_EQ_OFFSET_VALUE))
                {
                    EQ0 = (msReadByte(REG_1120E6 +DP_PHY_REG_OFFSET(ucPortSelect)) & 0x1F) +DP_EQ_OFFSET_VALUE;
                }
                else
                {
                    EQ0 = 0x1F;
                }
                    
                if((((msReadByte(REG_1120E6 +DP_PHY_REG_OFFSET(ucPortSelect)) &0xE0) >> 5) | ((msReadByte(REG_1120E7 +DP_PHY_REG_OFFSET(ucPortSelect)) &0x03) << 3)) <= (0x1F -DP_EQ_OFFSET_VALUE))
                {
                    EQ1 = (((msReadByte(REG_1120E6 +DP_PHY_REG_OFFSET(ucPortSelect)) &0xE0) >> 5) | ((msReadByte(REG_1120E7 +DP_PHY_REG_OFFSET(ucPortSelect)) &0x03) << 3)) +DP_EQ_OFFSET_VALUE;
                }
                else
                {
                    EQ1 =0x1F;
                }
                    
                if(((msReadByte(REG_1120E7 +DP_PHY_REG_OFFSET(ucPortSelect)) &0x7C) >> 2) <= (0x1F -DP_EQ_OFFSET_VALUE))
                {
                    EQ2 = ((msReadByte(REG_1120E7 +DP_PHY_REG_OFFSET(ucPortSelect)) & 0x7C) >> 2) +DP_EQ_OFFSET_VALUE;
                }
                else
                {
                    EQ2 =0x1F;
                }
                    
                if((msReadByte(REG_1120E8 +DP_PHY_REG_OFFSET(ucPortSelect)) &0x1F) <= (0x1F -DP_EQ_OFFSET_VALUE))
                {
                    EQ3 = (msReadByte(REG_1120E8 +DP_PHY_REG_OFFSET(ucPortSelect)) & 0x1F) +DP_EQ_OFFSET_VALUE;
                }
                else
                {
                    EQ3 =0x1F;
                }
    
                EQ_CAL1 = (((EQ1 &0x07) << 5) | (EQ0 &0x1F));
                msWriteByte(REG_1120E6 +DP_PHY_REG_OFFSET(ucPortSelect), EQ_CAL1);
    
                EQ_CAL2 = (((EQ2 &0x1F) << 2) | ((EQ1 &0x18) >> 3));
                msWriteByte(REG_1120E7 +DP_PHY_REG_OFFSET(ucPortSelect), EQ_CAL2);
    
                EQ_CAL3 = (EQ3 & 0x1F);
                msWriteByte(REG_1120E8 +DP_PHY_REG_OFFSET(ucPortSelect), EQ_CAL3);
    
                msWriteByte(REG_1120DC +DP_PHY_REG_OFFSET(ucPortSelect), 0xC0);
                msWriteByte(REG_1120DD +DP_PHY_REG_OFFSET(ucPortSelect), 0x03);
            }
            
#endif
            
#if(DP_ERROR_COUNT_FUNCTION || DP_ENABLE_STABLE_COUNT)
            volatile WORD delay_count = 0;
            BYTE error_flag = 0;
            WORD ErrorCnt = 0, error_L0 = 0,error_L1 = 0,error_L2 = 0,error_L3 = 0;
            
            /*
            3. 設定error cnt
                寫0x113006 [15:8] = 8'h00       //error_cnt_mask_thd
                寫0x113000 [11] = 1'b1             //error_cnt_sel
                寫 0x113000 [10] = 1'b1             //error_cnt_en
                寫 0x113000 [14] = 1'b1             //eq_ctrl_reserved  這個reg在Odin U02才有用
            
            4. 間隔一段時間(需實驗決定), 讀取error cnt以判斷是否stable
                讀 0x113033 [15:0]       // lane0 error cnt
                讀 0x113034 [15:0]       // lane1 error cnt
                讀 0x113035 [15:0]       // lane2 error cnt
                讀 0x113036 [15:0]       // lane3 error cnt
            */
            
            if(AEQ_done == 0x01)
            {
                AEQ_done = 0;
    
                while(delay_count < 2200)
                {
                   delay_count++;
                }
    
                msWriteByteMask(REG_113001 +DP_HBR2_REG_OFFSET(ucHBR2Select), MASKBIT(3:2), MASKBIT(3:2));
                msWriteByteMask(REG_113001 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT6, BIT6);
    
                while((error_flag == 0) && (ErrorCnt < 1000))
                {
                    usTP2Count++;
                    ErrorCnt++;
    
                    switch(msReadByte(REG_11330F +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x07)
                    {
                        case 0x01:  // 1 lane
                            error_L0 = msRead2Byte(REG_113066 +DP_HBR2_REG_OFFSET(ucHBR2Select));
    
                            if(error_L0 >= 1)
                            {
                                error_flag = 1;
                            }
                                
                            break;
    
                        case 0x02:  // 2 lanes
                            error_L0 = msRead2Byte(REG_113066 +DP_HBR2_REG_OFFSET(ucHBR2Select));
                            error_L1 = msRead2Byte(REG_113068 +DP_HBR2_REG_OFFSET(ucHBR2Select));
    
                            if((error_L0 >= 1) || (error_L1 >= 1))
                            {
                                error_flag = 1;
                            }
                                
                            break;
    
                        case 0x04:  // 4 lanes
                            error_L0 = msRead2Byte(REG_113066 +DP_HBR2_REG_OFFSET(ucHBR2Select));
                            error_L1 = msRead2Byte(REG_113068 +DP_HBR2_REG_OFFSET(ucHBR2Select));
                            error_L2 = msRead2Byte(REG_11306A +DP_HBR2_REG_OFFSET(ucHBR2Select));
                            error_L3 = msRead2Byte(REG_11306C +DP_HBR2_REG_OFFSET(ucHBR2Select));
    
                            if((error_L0 >= 1) || (error_L1 >= 1) || (error_L2 >= 1) || (error_L3 >= 1))
                            {
                                error_flag = 1;
                            }
    
                            break;
    
                        default:
    
                            break;
                    };
                }
    
                if(error_flag == 0x00 && (!bDPForceDownSpeed[ucPortSelect]))
                {
                    msWriteByteMask(REG_1135C1 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0, MASKBIT(3:2));
                }
            }

            bDPForceDownSpeed[ucPortSelect] = FALSE;
                
            msWriteByteMask(REG_113001 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0, MASKBIT(3:2));
            msWriteByteMask(REG_113001 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0, BIT6);
            
#endif
            
            
#if(DP_DYNAMIC_PREEMPHASIS)
            msWriteByteMask(REG_113012 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0, BIT4);
            
#endif
            
            msWriteByteMask(REG_11357F +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT0, BIT0); // enable Reset Word Boundary
            msWriteByteMask(REG_1133E0 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT1, BIT1); // disable TPS2 interrupt
            
#endif
                    //#if 1//DPCTS
#if(!DP_AUTO_EQ_ENABLE)//DPCTS
            BYTE ucPreemphasis = 0;
    
            msWriteByteMask(REG_11331E +DP_HBR2_REG_OFFSET(ucHBR2Select), 0, MASKBIT(7:6) |MASKBIT(3:2));
            msWriteByteMask(REG_11331F +DP_HBR2_REG_OFFSET(ucHBR2Select), 0, MASKBIT(7:6) |MASKBIT(3:2));
            
            while((((msReadByte(REG_113310 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x03) == 0x02)||((msReadByte(REG_113310 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x03) == 0x03)) && (usTP2Count < 50000)) // while pattern 2 is On-Going
            {
                usTP2Count++;
                
                switch(msReadByte(REG_11330F +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x07)
                {
                    case 0x01:  // 1 lane
                        if((msReadByte(REG_113311 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x03) == (msReadByte(REG_11331E +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x0C)) // request reach
                        {
                            if((msReadByte(REG_11331A +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x06) != 0x06) // 1 lane LOCK Fail
                            {
                                if(ucPreemphasis < 3)
                                {
                                    ucPreemphasis++;
                                }
                                
                                msWriteByteMask(REG_11331E +DP_HBR2_REG_OFFSET(ucHBR2Select), (ucPreemphasis << 2), 0x0C);
                                msWriteByteMask(REG_11331C +DP_HBR2_REG_OFFSET(ucHBR2Select), 0, BIT7);
                                msWriteByteMask(REG_11331C +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT7, BIT7);
                            }
                        }
                        
                        break;
    
                    case 0x02:  // 2 lanes
                        if((msReadByte(REG_113311 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x18) == (msReadByte(REG_11331E +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x0C)) // request reach
                        {
                            if((msReadByte(REG_113312 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x18) == ((msReadByte(REG_11331E +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0xC0) >> 6)) // request reach
                            {
                                if((msReadByte(REG_11331A +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x66) != 0x66) // 2 lanes CDR LOCK Fail
                                {
                                    if(ucPreemphasis < 3)
                                    {
                                        ucPreemphasis++;
                                    }
                                    
                                    msWriteByteMask(REG_11331E +DP_HBR2_REG_OFFSET(ucHBR2Select), (ucPreemphasis << 6) |(ucPreemphasis << 2), 0xCC);
                                    msWriteByteMask(REG_11331C +DP_HBR2_REG_OFFSET(ucHBR2Select), 0, BIT7);
                                    msWriteByteMask(REG_11331C +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT7, BIT7);
                                }
                            }
                        }
                        
                        break;
    
                    case 0x04:  // 4 lanes
                        if((msReadByte(REG_113311 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x18) == (msReadByte(REG_11331E +DP_HBR2_REG_OFFSET(ucHBR2Select)) & 0x0C)) // request reach
                        {
                            if((msReadByte(REG_113312 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x18) == ((msReadByte(REG_11331E +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0xC0) >> 4)) // request reach
                            {
                                if((msReadByte(REG_113313 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x18) == (msReadByte(REG_11331F +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x0C)) // request reach
                                {
                                    if((msReadByte(REG_113314 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x18) == ((msReadByte(REG_11331F +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0xC0) >> 4)) // request reach
                                    {
                                        if(((msReadByte(REG_11331A +DP_HBR2_REG_OFFSET(ucHBR2Select)) & 0x66) != 0x66) | ((msReadByte(REG_11331B +DP_HBR2_REG_OFFSET(ucHBR2Select)) & 0x66) != 0x66)) // 4 lanes CDR LOCK Fail
                                        {
                                            if(ucPreemphasis < 3)
                                            {
                                                ucPreemphasis++;
                                            }
                                            
                                            msWriteByteMask(REG_11331E +DP_HBR2_REG_OFFSET(ucHBR2Select), (ucPreemphasis << 6) |(ucPreemphasis << 2), 0xCC);
                                            msWriteByteMask(REG_11331F +DP_HBR2_REG_OFFSET(ucHBR2Select), (ucPreemphasis << 6) |(ucPreemphasis << 2), 0xCC);
                                            msWriteByteMask(REG_11331C +DP_HBR2_REG_OFFSET(ucHBR2Select), 0, BIT7);
                                            msWriteByteMask(REG_11331C +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT7, BIT7);
                                        }
                                    }
                                }
                            }
                        }
                        
                        break;
                        
                    default:
    
                        break;
                };
            }
            
#endif

            mhal_DP_SquelchLevelSetting(ucPortSelect, 0x4);
                 
            msWriteByteMask(REG_1133E8 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT1, BIT1); 
            msWriteByteMask(REG_1133EA +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT1, BIT1); 
        }
    }

    return (bTP2Flag || bTP3Flag);
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_HDCPReset()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_DP_HDCPReset(BYTE ucPortSelect, BYTE ucHBR2Select)
{
    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        ucPortSelect = 0;
    }
    else // HBR2 path
    {
        msWriteByteMask(REG_1133A1 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT4, BIT4);	//HDCP reset
        msWriteByteMask(REG_1133A1 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0, BIT4);  //HDCP reset.
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_GetHDCP2LinkIntegrityFailure()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
Bool mhal_DP_GetHDCP2LinkIntegrityFailure(BYTE ucPortSelect)
{
    Bool bEventFlag = FALSE;
    
    switch(ucPortSelect)
    {
        case DP_INPUT_SELECT_PORT0:
            bEventFlag = ((msReadByte(REG_1133EB) &BIT5) ?TRUE: FALSE); // REG_DP_BANKB_74[12]: DP HDCP link integrity failure interrupt

            if(bEventFlag)
            {
                msWriteByteMask(REG_1133EB, BIT5, BIT5); // REG_DP_BANKB_74[12]: DP HDCP link integrity failure interrupt
            }
            
            break;

        case DP_INPUT_SELECT_PORT1:
            
            break;

        case DP_INPUT_SELECT_PORT2:
            
            break;

        case DP_INPUT_SELECT_PORT3:
            
            break;

        default:

            break;
    };

    return bEventFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_SetGuardBand()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_DP_SetGuardBand(BYTE ucPortSelect, BYTE ucHBR2Select, WORD usGuardBandValue)
{
    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        ucPortSelect = 0;
    }
    else // HBR2 path
    {
        msWrite2ByteMask(REG_113554 +DP_HBR2_REG_OFFSET(ucHBR2Select), usGuardBandValue, MASK_11BIT);
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_AudioInitial()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_DP_AudioInitial(void)
{
    msWriteByte(REG_162F0B, 0x2A); // Audio Low Pass Filter

    msWrite2Byte(REG_162FF8, 0xFFFF);
    msWriteByte(REG_162FFA, 0xFF);
    msWriteByteMask(REG_162F07, 0, BIT0);
    msWriteByteMask(REG_162F41, BIT0, BIT0);
        
#if(DP_SSC_ENABLE)
    msWriteByteMask(REG_162F22, 0x09, 0x0F);
    msWriteByteMask(REG_162F2A, BIT4 ,BIT4);
#endif
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_AUPLLSetting()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_DP_AUPLLSetting(void)
{
    msWriteByteMask(REG_162F11, 0, BIT7); // Turn off Bypasse mode
    
    _mhal_DP_UpdateAudioFrequence();
    
#if(DP_SSC_ENABLE)
    msWriteByteMask(REG_162F22, 0, BIT5);
#endif

    msWriteByteMask(REG_162F2A, BIT4, BIT4);
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_CheckAudioAbsent()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mhal_DP_CheckAudioAbsent(BYTE ucPortSelect, BYTE ucHBR2Select)
{
    Bool bNoAudio = FALSE;
    
    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        ucPortSelect = 0;
    }
    else // HBR2 path
    {
        if(msReadByte(REG_1133A5 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &BIT4) // Read BK36 A5 Bit4:1 NoAudioStrm
        {
            bNoAudio = TRUE;
        }
        else if(((msReadByte(REG_1132AA +DP_HBR2_REG_OFFSET(ucHBR2Select))) &0x60) == 0) // Check audio status
        {
            bNoAudio = TRUE;
        }
    }

    return bNoAudio;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_CheckAudioMuteEvent()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_DP_CheckAudioMuteEvent(BYTE ucPortSelect, BYTE ucHBR2Select)
{
    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        ucPortSelect = 0;
    }
    else // HBR2 path
    {
        if(msReadByte(REG_1630B0) &(BIT0 |BIT1))
        {
            msWriteByteMask(REG_1630B0, BIT0 |BIT1, BIT0 |BIT1);
        }
        
#if(!DP_SSC_ENABLE)
        if(msReadByte(REG_1630B1) &(BIT0 |BIT1))
        {
            msWriteByteMask(REG_1630B1, BIT0 |BIT1, BIT0 |BIT1);   
        }    
#endif    

        if(msReadByte(REG_1133E7 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &BIT0)
        {
            msWriteByte(REG_1133EB +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT0);
        }
        
        if(msReadByte(REG_1133E7 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &BIT2)
        {
            msWriteByte(REG_1133EB +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT2);
        }
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_CheckLossAlignment()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mhal_DP_CheckLossAlignment(BYTE ucPortSelect, BYTE ucHBR2Select)
{
    Bool bAlignment = FALSE;
    
    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        ucPortSelect = 0;
    }
    else // HBR2 path
    {
        if(msReadByte(REG_11331C +DP_HBR2_REG_OFFSET(ucHBR2Select)) &BIT0) // lost alignment
        {
            bAlignment = TRUE;
        }
    }

    return (!bAlignment);
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_CheckAUPLLBigChange()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mhal_DP_CheckAUPLLBigChange(BYTE ucPortSelect, BYTE ucHBR2Select)
{
    Bool bBigChange = FALSE;
    stDPAudioInfo stAudioTemp = {0, 0, 0, 0, 0};
    
    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        ucPortSelect = 0;
    }
    else // HBR2 path
    {
        stAudioTemp.ulMCodeValue = msReadByte(REG_113390 +DP_HBR2_REG_OFFSET(ucHBR2Select)); //MSA MAUD_CENTER[23:16]
        stAudioTemp.ulMCodeValue = (stAudioTemp.ulMCodeValue << 16) +msRead2Byte(REG_11338E +DP_HBR2_REG_OFFSET(ucHBR2Select)); //MSA MAUD_CENTER[15:0]
        stAudioTemp.ulNCodeValue = msReadByte(REG_113394 +DP_HBR2_REG_OFFSET(ucHBR2Select)); //MSA NAUD_CENTER[23:16]
        stAudioTemp.ulNCodeValue = (stAudioTemp.ulNCodeValue << 16) +msRead2Byte(REG_113392 +DP_HBR2_REG_OFFSET(ucHBR2Select)); //MSA NAUD_CENTER[15:0]
        stAudioTemp.usDivValue = msRead2Byte(REG_162F04); //FBDIV[15:0]
        stAudioTemp.ucKpValue = _mhal_DP_GetPow2((msReadByte( REG_162F00) >> 4) &0x0F); //Kp
        stAudioTemp.ucKmValue = _mhal_DP_GetPow2(msReadByte(REG_162F01) &0x0F); //KM

        stAudioInfo.ulMCodeValue = msReadByte(REG_1133B0 +DP_HBR2_REG_OFFSET(ucHBR2Select));
        stAudioInfo.ulMCodeValue = (stAudioInfo.ulMCodeValue << 16) +msRead2Byte(REG_1133AE +DP_HBR2_REG_OFFSET(ucHBR2Select));
        stAudioInfo.ulNCodeValue = msReadByte(REG_1133B8 +DP_HBR2_REG_OFFSET(ucHBR2Select));
        stAudioInfo.ulNCodeValue = (stAudioInfo.ulNCodeValue << 16) +msRead2Byte(REG_1133B6 +DP_HBR2_REG_OFFSET(ucHBR2Select));
    }

    if(ABS_MINUS(stAudioInfo.ulMCodeValue, stAudioTemp.ulMCodeValue) > 0x80) // Compare Mcode
    {
        bBigChange = TRUE;
    }
    else if(stAudioInfo.ucKmValue != stAudioTemp.ucKmValue) // Compare Km
    {
        bBigChange = TRUE;
    }
    else if(stAudioInfo.ucKpValue != stAudioTemp.ucKpValue) // Compare Kp
    {
        bBigChange = TRUE;
    }
    else if(stAudioInfo.usDivValue != stAudioTemp.usDivValue) // Compare Div
    {
        bBigChange = TRUE;
    }

    if(bBigChange) // Update audio info
    {
        stAudioInfo.ucKmValue = stAudioTemp.ucKmValue;
        stAudioInfo.ucKpValue = stAudioTemp.ucKpValue;
        stAudioInfo.usDivValue = stAudioTemp.usDivValue;
        stAudioInfo.ulMCodeValue = stAudioTemp.ulMCodeValue;
        stAudioInfo.ulNCodeValue = stAudioTemp.ulNCodeValue;
    }

    return bBigChange;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_UpdateAudioMNCode()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_DP_UpdateAudioMNCode(BYTE ucPortSelect, BYTE ucHBR2Select, DWORD ulAudioMCode, DWORD ulAudioNCode)
{
    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        ucPortSelect = 0;
    }
    else // HBR2 path
    {
        msWrite2Byte(REG_1133AE +DP_HBR2_REG_OFFSET(ucHBR2Select), ulAudioMCode &0xFFFF); //UPDATE MAUD_CENTER[15:0]
        msWriteByte(REG_1133B0 +DP_HBR2_REG_OFFSET(ucHBR2Select), (ulAudioMCode >> 16)); //UPDATE MAUD_CENTER[23:16]
        msWrite2Byte(REG_1133B6 +DP_HBR2_REG_OFFSET(ucHBR2Select), ulAudioNCode &0xFFFF); //UPDATE NAUD_CENTER[15:0]
        msWriteByte(REG_1133B8 +DP_HBR2_REG_OFFSET(ucHBR2Select), (ulAudioNCode >> 16)); //UPDATE NAUD_CENTER[23:16]
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_AUPLLBigChangeProc()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_DP_AUPLLBigChangeProc(BYTE ucPortSelect, BYTE ucHBR2Select)
{
    WORD usLSClock = _mhal_DP_GetLinkRate(ucPortSelect, ucHBR2Select);
    DWORD ulAudioFreq = 0;

    //################  Audio M/N example ###################
    //M=512*fs*N/LinkRate
    //N=0x8000=32768
    //32K,2.7G   => M=512*32K*32768/270000K=1988.4=0x07_C4
    //48K,2.7G   => M=512*48K*32768/270000K=2982.6=0x0B_A6
    //32K,1.62G => M=512*32K*32768/162000K=3314.0=0x0C_F2
    //48K,1.62G => M=512*48K*32768/162000K=4971.0=0x13_6B
    // ################################################
    
    mhal_DP_UpdateAudioMNCode(ucPortSelect, ucHBR2Select, stAudioInfo.ulMCodeValue, stAudioInfo.ulNCodeValue);
        
    ulAudioFreq = 64 *(214.77 /usLSClock) *(stAudioInfo.ucKmValue *stAudioInfo.ulMCodeValue) *stAudioInfo.usDivValue;
    ulAudioFreq = ulAudioFreq /(stAudioInfo.ucKpValue *stAudioInfo.ulNCodeValue);

    msWriteByte(REG_162F08, ulAudioFreq &0xFFFF); //Freq
    msWriteByte(REG_162F0A, (ulAudioFreq >> 16)); //Freq

    _mhal_DP_UpdateAudioFrequence();
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_ErrorCountEnable()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_DP_ErrorCountEnable(BYTE ucPortSelect, BYTE ucHBR2Select, Bool bEnable)
{
    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        ucPortSelect = 0;
    }
    else // HBR2 path
    {
        msWriteByteMask(REG_113001 +DP_HBR2_REG_OFFSET(ucHBR2Select), bEnable? (BIT6| BIT3| BIT2): 0, BIT6| BIT3| BIT2);
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_GetErrorCountFlag()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mhal_DP_GetErrorCountFlag(BYTE ucPortSelect, BYTE ucHBR2Select)
{
    Bool bErrorCountFlag = FALSE;
    BYTE uctemp = 0;
    BYTE ucLansCount = 0;
    WORD usErrorCountLane[4] = {0, 0, 0, 0};
    
    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        ucPortSelect = 0;
    }
    else // HBR2 path
    {
        ucLansCount = msReadByte(REG_11330F +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x07;
        usErrorCountLane[0] = msRead2Byte(REG_113066 +DP_HBR2_REG_OFFSET(ucHBR2Select));
        usErrorCountLane[1] = msRead2Byte(REG_113068 +DP_HBR2_REG_OFFSET(ucHBR2Select));
        usErrorCountLane[2] = msRead2Byte(REG_11306A +DP_HBR2_REG_OFFSET(ucHBR2Select));
        usErrorCountLane[3] = msRead2Byte(REG_11306C +DP_HBR2_REG_OFFSET(ucHBR2Select));
    }

#if(DP_ENABLE_STABLE_COUNT)
    for(uctemp = 0; uctemp < ucLansCount; uctemp++)
    {
        if((usErrorCountLane[uctemp] >= DP_ERROR_COUNT_THRESHOLD) && (usErrorCountLane[uctemp] != 0xFFFF))
        {
            bErrorCountFlag = TRUE;

            break;
        }
    }

#endif

    if(bErrorCountFlag)
    {
        if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
        {
            ucPortSelect = 0;
        }
        else // HBR2 path
        {
            msWriteByteMask(REG_1135C1 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT3, MASKBIT(3:2));
        }

        bDPForceDownSpeed[ucPortSelect] = TRUE;

        uctemp = 0;
    }
    
    return bErrorCountFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_OverwriteInterlaceMode()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_DP_OverwriteInterlaceMode(BYTE ucPortSelect, BYTE ucHBR2Select, Bool bEnable)
{
    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        ucPortSelect = 0;
    }
    else // HBR2 path
    {
        msWriteByteMask(REG_113541 +DP_HBR2_REG_OFFSET(ucHBR2Select), bEnable? MASKBIT(5:4): 0, MASKBIT(5:4));
    }
}

#if(DP_HDCP2_FUNCTION_SUPPORT)
//**************************************************************************
//  [Function Name]:
//                  mhal_DP_GetHDCP2AKEInitFlag()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
Bool mhal_DP_GetHDCP2AKEInitFlag(BYTE ucPortSelect)
{
    Bool bReceived = FALSE;
    
    switch(ucPortSelect)
    {
        case DP_INPUT_SELECT_PORT0:
            bReceived = ((MEM_MSREAD_BYTE(REG_113AE0) &BIT1) ?TRUE: FALSE); // REG_DPCD_HDCP22_4_70[1], reg_ake_init_irq

            if(bReceived)
            {
                msWriteByteMask(REG_113AE6, BIT1, BIT1); // REG_DPCD_HDCP22_4_73[1]: reg_ake_init_irq_clr
                msWriteByteMask(REG_113AE6, 0, BIT1); // REG_DPCD_HDCP22_4_73[1]: reg_ake_init_irq_clr
            }
            
            break;

        case DP_INPUT_SELECT_PORT1:
            
            break;

        case DP_INPUT_SELECT_PORT2:
            
            break;

        case DP_INPUT_SELECT_PORT3:
            
            break;

        default:

            break;
    };

    return bReceived;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_GetHDCP2NoStoredKmFlag()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
Bool mhal_DP_GetHDCP2NoStoredKmFlag(BYTE ucPortSelect)
{
    Bool bReceived = FALSE;
    
    switch(ucPortSelect)
    {
        case DP_INPUT_SELECT_PORT0:
            bReceived = ((MEM_MSREAD_BYTE(REG_113AE0) &BIT2) ?TRUE: FALSE); // REG_DPCD_HDCP22_4_70[2], reg_ake_no_stored_km_irq

            if(bReceived)
            {
                msWriteByteMask(REG_113AE6, BIT2, BIT2); // REG_DPCD_HDCP22_4_73[2]: reg_ake_no_stored_km_irq_clr
                msWriteByteMask(REG_113AE6, 0, BIT2); // REG_DPCD_HDCP22_4_73[2]: reg_ake_no_stored_km_irq_clr
            }
            
            break;

        case DP_INPUT_SELECT_PORT1:
            
            break;

        case DP_INPUT_SELECT_PORT2:
            
            break;

        case DP_INPUT_SELECT_PORT3:
            
            break;

        default:

            break;
    };

    return bReceived;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_GetHDCP2StoredKmFlag()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
Bool mhal_DP_GetHDCP2StoredKmFlag(BYTE ucPortSelect)
{
    Bool bReceived = FALSE;
    
    switch(ucPortSelect)
    {
        case DP_INPUT_SELECT_PORT0:
            bReceived = ((MEM_MSREAD_BYTE(REG_113AE0) &BIT3) ?TRUE: FALSE); // REG_DPCD_HDCP22_4_70[3], reg_ake_stored_km_irq

            if(bReceived)
            {
                msWriteByteMask(REG_113AE6, BIT3, BIT3); // REG_DPCD_HDCP22_4_73[3]: reg_ake_stored_km_irq_clr
                msWriteByteMask(REG_113AE6, 0, BIT3); // REG_DPCD_HDCP22_4_73[3]: reg_ake_stored_km_irq_clr
            }
            
            break;

        case DP_INPUT_SELECT_PORT1:
            
            break;

        case DP_INPUT_SELECT_PORT2:
            
            break;

        case DP_INPUT_SELECT_PORT3:
            
            break;

        default:

            break;
    };

    return bReceived;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_GetHDCP2LCInitFlag()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
Bool mhal_DP_GetHDCP2LCInitFlag(BYTE ucPortSelect)
{
    Bool bReceived = FALSE;
    
    switch(ucPortSelect)
    {
        case DP_INPUT_SELECT_PORT0:
            bReceived = ((MEM_MSREAD_BYTE(REG_113AE0) &BIT4) ?TRUE: FALSE); // REG_DPCD_HDCP22_4_70[4], reg_locality_check_irq

            if(bReceived)
            {
                msWriteByteMask(REG_113AE6, BIT4, BIT4); // REG_DPCD_HDCP22_4_73[4]: reg_locality_check_irq_clr
                msWriteByteMask(REG_113AE6, 0, BIT4); // REG_DPCD_HDCP22_4_73[4]: reg_locality_check_irq_clr
            }
            
            break;

        case DP_INPUT_SELECT_PORT1:
            
            break;

        case DP_INPUT_SELECT_PORT2:
            
            break;

        case DP_INPUT_SELECT_PORT3:
            
            break;

        default:

            break;
    };

    return bReceived;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_GetHDCP2SKESendFlag()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
Bool mhal_DP_GetHDCP2SKESendFlag(BYTE ucPortSelect)
{
    Bool bReceived = FALSE;
    
    switch(ucPortSelect)
    {
        case DP_INPUT_SELECT_PORT0:
            bReceived = ((MEM_MSREAD_BYTE(REG_113AE0) &BIT5) ?TRUE: FALSE); // REG_DPCD_HDCP22_4_70[5], reg_ske_irq

            if(bReceived)
            {
                msWriteByteMask(REG_113AE6, BIT5, BIT5); // REG_DPCD_HDCP22_4_73[5]: reg_ske_irq_clr
                msWriteByteMask(REG_113AE6, 0, BIT5); // REG_DPCD_HDCP22_4_73[5]: reg_ske_irq_clr
            }
            
            break;

        case DP_INPUT_SELECT_PORT1:
            
            break;

        case DP_INPUT_SELECT_PORT2:
            
            break;

        case DP_INPUT_SELECT_PORT3:
            
            break;

        default:

            break;
    };

    return bReceived;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_HDCP2GetRxData()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
Bool mhal_DP_HDCP2GetRxData(BYTE ucPortSelect, BYTE ucDataInfo, BYTE *pHDCPData)
{
    Bool bReceived = FALSE;
    WORD ustemp = 0;
    
    switch(ucPortSelect)
    {
        case DP_INPUT_SELECT_PORT0:
            switch(ucDataInfo)
            {
                case DP_HDCP2_INFO_AKE_RTX:
                    for(ustemp = 0; ustemp < DP_HDCP2_AKE_RTX_SIZE; ustemp++)
                    {
                        pHDCPData[ustemp] = msReadByte(REG_11380A +ustemp); // REG_DPCD_HDCP22_2_05, reg_r_tx
                    }

                    bReceived = TRUE;
                    
                    break;

                case DP_HDCP2_INFO_AKE_TXCAPS:
                    for(ustemp = 0; ustemp < DP_HDCP2_AKE_TXCAPS_SIZE; ustemp++)
                    {
                        pHDCPData[ustemp] = msReadByte(REG_113812 +ustemp); // REG_DPCD_HDCP22_2_09, reg_tx_caps
                    }

                    bReceived = TRUE;

                    break;

                case DP_HDCP2_INFO_AKE_EKPUB_KM:
                    for(ustemp = 0; ustemp < DP_HDCP2_AKE_EKPUB_KM_SIZE; ustemp++)
                    {
                        pHDCPData[ustemp] = msReadByte(REG_113822 +ustemp); // REG_DPCD_HDCP22_2_11, reg_ekpub_km
                    }

                    bReceived = TRUE;
                    
                    break;

                case DP_HDCP2_INFO_AKE_EKH_KM_WR:
                    for(ustemp = 0; ustemp < DP_HDCP2_AKE_EKH_KM_WR_SIZE; ustemp++)
                    {
                        pHDCPData[ustemp] = msReadByte(REG_1138A2 +ustemp); // REG_DPCD_HDCP22_2_51, reg_ekh_km_wr
                    }

                    bReceived = TRUE;
                    
                    break;

                case DP_HDCP2_INFO_AKE_M:
                    for(ustemp = 0; ustemp < DP_HDCP2_AKE_M_SIZE; ustemp++)
                    {
                        pHDCPData[ustemp] = msReadByte(REG_1138B2 +ustemp); // REG_DPCD_HDCP22_2_59, reg_m
                    }

                    bReceived = TRUE;
                    
                    break;

                case DP_HDCP2_INFO_LC_RN:
                    for(ustemp = 0; ustemp < DP_HDCP2_LC_RN_SIZE; ustemp++)
                    {
                        pHDCPData[ustemp] = msReadByte(REG_1138F2 +ustemp); // REG_DPCD_HDCP22_2_79, reg_r_n
                    }
                    
                    break;

                case DP_HDCP2_INFO_SKE_EDKEY_KS:
                    for(ustemp = 0; ustemp < DP_HDCP2_SKE_EDKEY_KS_SIZE; ustemp++)
                    {
                        pHDCPData[ustemp] = msReadByte(REG_113920 +ustemp); // REG_DPCD_HDCP22_3_10, reg_edkey_ks
                    }

                    bReceived = TRUE;
                    
                    break;

                case DP_HDCP2_INFO_SKE_RIV:
                    for(ustemp = 0; ustemp < DP_HDCP2_SKE_RIV_SIZE; ustemp++)
                    {
                        pHDCPData[ustemp] = msReadByte(REG_113930 +ustemp); // REG_DPCD_HDCP22_3_18, reg_r_iv
                    }

                    bReceived = TRUE;
                    
                    break;

                default:
                    
                    break;
            };
            
            break;

        case DP_INPUT_SELECT_PORT1:
            
            break;

        case DP_INPUT_SELECT_PORT2:
            
            break;

        case DP_INPUT_SELECT_PORT3:
            
            break;

        default:

            break;
    };

    return bReceived;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_HDCP2SetTxData()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
Bool mhal_DP_HDCP2SetTxData(BYTE ucPortSelect, BYTE ucDataInfo, BYTE *pHDCPData)
{
    Bool bReceived = FALSE;
    WORD ustemp = 0;
    
    switch(ucPortSelect)
    {
        case DP_INPUT_SELECT_PORT0:
            switch(ucDataInfo)
            {
                case DP_HDCP2_INFO_AKE_CERTRX:
                    for(ustemp = 0; ustemp < DP_HDCP2_AKE_CERTRX_SIZE; ustemp++)
                    {
                        if(ustemp > 512)
                        {
                            msWriteByte(REG_113800 +(ustemp -512), pHDCPData[ustemp]); // REG_DPCD_HDCP22_2_00, reg_cert_rx_512_521
                        }
                        else if(ustemp > 256)
                        {
                            msWriteByte(REG_113700 +(ustemp -256), pHDCPData[ustemp]); // REG_DPCD_HDCP22_1_00, reg_cert_rx_256_511
                        }
                        else
                        {
                            msWriteByte(REG_113600 +ustemp, pHDCPData[ustemp]); // REG_DPCD_HDCP22_0_00, reg_cert_rx_0_255
                        }
                    }
                    
                    break;

                case DP_HDCP2_INFO_AKE_RRX:
                    for(ustemp = 0; ustemp < DP_HDCP2_AKE_RRX_SIZE; ustemp++)
                    {
                        msWriteByte(REG_113816 +ustemp, pHDCPData[ustemp]); // REG_DPCD_HDCP22_2_0B, reg_r_rx
                    }

                    break;

                case DP_HDCP2_INFO_AKE_RXCAPS:
                    for(ustemp = 0; ustemp < DP_HDCP2_AKE_RXCAPS_SIZE; ustemp++)
                    {
                        msWriteByte(REG_11381E +ustemp, pHDCPData[ustemp]); // REG_DPCD_HDCP22_2_0F, reg_rx_caps
                    }
                    
                    break;

                case DP_HDCP2_INFO_AKE_H_PRIME:
                    for(ustemp = 0; ustemp < DP_HDCP2_AKE_H_PRIME_SIZE; ustemp++)
                    {
                        msWriteByte(REG_1138C2 +ustemp, pHDCPData[ustemp]); // REG_DPCD_HDCP22_2_61, reg_h_pulum
                    }
                    
                    break;

                case DP_HDCP2_INFO_AKE_EKH_KM_RD:
                    for(ustemp = 0; ustemp < DP_HDCP2_AKE_EKH_KM_RD_SIZE; ustemp++)
                    {
                        msWriteByte(REG_1138E2 +ustemp, pHDCPData[ustemp]); // REG_DPCD_HDCP22_2_71, reg_ekh_km_rd
                    }
                    
                    break;

                case DP_HDCP2_INFO_LC_L_PRIME:
                    for(ustemp = 0; ustemp < DP_HDCP2_LC_L_PRIME_SIZE; ustemp++)
                    {
                        msWriteByte(REG_113900 +ustemp, pHDCPData[ustemp]); // REG_DPCD_HDCP22_3_00, reg_l_pulum
                    }
                    
                    break;

                default:
                    
                    break;
            };
            
            break;

        case DP_INPUT_SELECT_PORT1:
            
            break;

        case DP_INPUT_SELECT_PORT2:
            
            break;

        case DP_INPUT_SELECT_PORT3:
            
            break;

        default:

            break;
    };

    return bReceived;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_HDCP2SetRxStatus()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void mhal_DP_HDCP2SetRxStatus(BYTE ucPortSelect, BYTE ucMaskIndex, Bool bEnable)
{
    switch(ucPortSelect)
    {
        case DP_INPUT_SELECT_PORT0:
            msWriteByteMask(REG_113A9E, bEnable? ucMaskIndex: 0, ucMaskIndex); // REG_DPCD_HDCP22_4_4F[7:0]: reg_rx_status
            break;

        case DP_INPUT_SELECT_PORT1:
            
            break;

        case DP_INPUT_SELECT_PORT2:
            
            break;

        case DP_INPUT_SELECT_PORT3:
            
            break;

        default:

            break;
    };
}

#endif

#endif // _MHAL_DPRX_C_


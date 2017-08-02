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
#if 1//ENABLE_DP_INPUT
#include "mapi_DPRx.h"
#include "mdrv_DPRx.h"
#include "mhal_DPRx.h"
#include "drvDDC2Bi.h"
#if ENABLE_DAC
#include "drvAudio.h"
#endif

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
BYTE ucDPLinkRate[DP_INPUT_SELECT_MASK];
BYTE ucDDCNullMessage[DP_AUX_COMMAND_DDC_LENGTH] = {0x6e, 0x80, 0xbe};
BYTE ucDDCBuffer[DP_AUX_COMMAND_MAX_LENGTH] = {0};
stDPAudioInfo stAudioInfo = {0, 0, 0, 0, 0};

//#if DP_SSC_ENABLE

#if  1
WORD DPRxAudioCnt=0;
WORD DPRxAudioSSCCnt=0;
WORD DPRxAudioSSCEn=0;
BYTE   DPAudio_State=1;
WORD   DPRxUnMuteCnt=0;
BYTE   DPRxAudioSSCEnCnt=0;
WORD DPRxPolling3Cnt=0;
BYTE   DPRxPolling3FIFOCnt=0;
WORD DPRxFreCtrl_Old=0;
WORD   DPRxCearCnt=0;
BYTE   DPRxCearCnt_On=0;
BYTE   DPFBDIV1_Old = 0;
BYTE   DPFBDIV1_Cnt = 0;
BYTE   DPAudioSampleCnt = 0;
Bool    DPAudioTimeOut=0;
Bool    DPAudioUnmute_Case1=0;
BYTE   DPRxAudioMuteEn=0;
Bool    DPAudioUnmute_UdfChk=0;
#endif

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
//                  _mhal_DP_ResetAsyncFIFO()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void _mhal_DP_ResetAsyncFIFO(void)
{
	msWriteBit(0x00111E, 0, BIT1);
   	msWriteBit(0x00111E, 1, BIT1);
   	msWriteBit(0x00111E, 0, BIT1);
   	msWriteBit(0x00113134, 0, BIT5);
   	msWriteBit(0x00113134, 1, BIT5);
   	msWriteBit(0x00113134, 0, BIT5);
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
            //break;

        case DP_INPUT_SELECT_PORT1:
            //break;

        case DP_INPUT_SELECT_PORT2:
            //break;

        case DP_INPUT_SELECT_PORT3:
            //break;

        case DP_INPUT_SELECT_PORT4:
            msWriteByteMask(REG_001900, 0, MASKBIT(3:0)); // Power on R-term of each Lane			
		    msWriteByte(REG_001901, 0x10);
			//msWriteByte(REG_001901, 0x00); // For MCP case NODIE_EN_LVRT=0

            break;

        case DP_INPUT_SELECT_PORT5:
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
    msWriteByteMask(REG_0019B9, 0, (BIT0 <<ucPortSelect)); // Odin set 1, but Leona set 0
    
#if(DP_SUPPORT_HBR2)
if(USER_PREF_DPConfigType==DP12)  //  160901_04
{
    msWriteByte(REG_001100 +DP_PHY_REG_OFFSET(ucPortSelect), 0x12); // DPCD minor revision number
    msWriteByte(REG_001101 +DP_PHY_REG_OFFSET(ucPortSelect), 0x14); // Maximum link rate of main link lanes
    msWriteByteMask(REG_001103 +DP_PHY_REG_OFFSET(ucPortSelect), 0x8B, BIT7|BIT5|BIT4|BIT3|BIT2|BIT1|BIT0); // EQ training Pattern 3 supported
}
else
{
    msWriteByte(REG_001100 +DP_PHY_REG_OFFSET(ucPortSelect), 0x11); // DPCD minor revision number
    msWriteByte(REG_001101 +DP_PHY_REG_OFFSET(ucPortSelect), 0x0A); // Maximum link rate of main link lanes
    msWriteByte(REG_001103 +DP_PHY_REG_OFFSET(ucPortSelect), 0x89); // EQ training Pattern 3 not supported

}
/*
    msWriteByte(REG_001100 +DP_PHY_REG_OFFSET(ucPortSelect), 0x12); // DPCD minor revision number
    msWriteByte(REG_001101 +DP_PHY_REG_OFFSET(ucPortSelect), 0x14); // Maximum link rate of main link lanes
    msWriteByteMask(REG_001103 +DP_PHY_REG_OFFSET(ucPortSelect), 0x8B, BIT7|BIT5|BIT4|BIT3|BIT2|BIT1|BIT0); // EQ training Pattern 3 supported
*/
#else
    msWriteByte(REG_001100 +DP_PHY_REG_OFFSET(ucPortSelect), 0x11); // DPCD minor revision number
    msWriteByte(REG_001101 +DP_PHY_REG_OFFSET(ucPortSelect), 0x0A); // Maximum link rate of main link lanes
    msWriteByteMask(REG_001103 +DP_PHY_REG_OFFSET(ucPortSelect), 0x89, BIT7|BIT5|BIT4|BIT3|BIT2|BIT1|BIT0); // EQ training Pattern 3 not supported 
#endif
 
    msWriteByte(REG_001123 +DP_PHY_REG_OFFSET(ucPortSelect), 0x84); // DP Enhanced Frame capability & Max Lane Count
    msWriteByte(REG_001112 +DP_PHY_REG_OFFSET(ucPortSelect), 0x0E); // DP AUX PHY UI counter threshold
    msWriteByte(REG_001113 +DP_PHY_REG_OFFSET(ucPortSelect), 0x0B); // DP AUX PHY over sample rate
    msWriteByte(REG_001114 +DP_PHY_REG_OFFSET(ucPortSelect), 0x00); // DP AUX timeout threshold Low byte
    msWriteByte(REG_001115 +DP_PHY_REG_OFFSET(ucPortSelect), 0x08); // DP AUX timeout threshold High byte
    msWriteByteMask(REG_001115 +DP_PHY_REG_OFFSET(ucPortSelect), 0, BIT6|BIT5); // DP AUX clock select
    msWriteByte(REG_00112C +DP_PHY_REG_OFFSET(ucPortSelect), 0x0E); // DP AUX PHY UI counter threshold
    msWriteByte(REG_00112D +DP_PHY_REG_OFFSET(ucPortSelect), 0x0B); // DP AUX PHY over sample rate
    msWriteByte(REG_00112E +DP_PHY_REG_OFFSET(ucPortSelect), 0x00); // DP AUX timeout threshold Low byte
    msWriteByte(REG_00112F +DP_PHY_REG_OFFSET(ucPortSelect), 0x08); // DP AUX timeout threshold High byte
    msWriteByte(REG_001130 +DP_PHY_REG_OFFSET(ucPortSelect), 0x00); // Aux Reply BY HW
//    msWriteByteMask(REG_001131 +DP_PHY_REG_OFFSET(ucPortSelect), 0, BIT1|BIT0); // Aux Time out Reply  // Time out reply defer
    msWriteByteMask(REG_001131 +DP_PHY_REG_OFFSET(ucPortSelect), BIT1, BIT1|BIT0); // Aux Time out Reply  // Time out reply defer
    msWriteByteMask(REG_001132 +DP_PHY_REG_OFFSET(ucPortSelect), 0x50, MASKBIT(6:0)); // EDID mapped I2C addressess
    msWriteByteMask(REG_001132 +DP_PHY_REG_OFFSET(ucPortSelect), BIT7, BIT7); // AUX length compare mask.

    msWrite2Byte(REG_0011A8 +DP_PHY_REG_OFFSET(ucPortSelect), 0x1044 ); // ENABLE TX
    msWrite2Byte(REG_0011C2, 0x0000);	// Off-line EN = 0
    msWrite2Byte(REG_0011C0, 0x2020);	// SQ Debunce 
    msWriteByteMask(REG_001142 +DP_PHY_REG_OFFSET(ucPortSelect), BIT0, BIT0); // Enable GUID Read/Write
    msWriteByteMask(REG_00119C +DP_PHY_REG_OFFSET(ucPortSelect), BIT7|BIT4, BIT7|BIT6|BIT5|BIT4); // Voltage Swim and PRE-emph when cheat link training

    msWriteByteMask(REG_00119A +DP_PHY_REG_OFFSET(ucPortSelect), BIT1, BIT1); // enable MCCS reply by mcu

    msWriteByte(REG_001179 +DP_PHY_REG_OFFSET(ucPortSelect), 0xFF);	// Clear Interrupt States
    msWriteByte(REG_001179 +DP_PHY_REG_OFFSET(ucPortSelect), 0x00);

    msWriteByte(REG_001192 +DP_PHY_REG_OFFSET(ucPortSelect), 0x0F);	// Clear dpcd0_irq_mask Interrupt States
    msWriteByte(REG_001192 +DP_PHY_REG_OFFSET(ucPortSelect), 0x00);

    msWriteByteMask(REG_001193 +DP_PHY_REG_OFFSET(ucPortSelect), BIT7, BIT7); // Clear _aksv_irq_mask
    msWriteByteMask(REG_001193 +DP_PHY_REG_OFFSET(ucPortSelect), 0, BIT7);

    msWriteByteMask(REG_001196 +DP_PHY_REG_OFFSET(ucPortSelect), BIT2, BIT2); // Clear_SQ
    msWriteByteMask(REG_001196 +DP_PHY_REG_OFFSET(ucPortSelect), 0, BIT2);

    DP_HAL_DPRINTF("** _mhal_DP_AuxInitialSetting -> done! port %d", ucPortSelect);
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
void _mhal_DP_SetProgramDPCD(BYTE ucPortSelect, BYTE ucDPCPSelect, Bool bAddrGroup)
{
    switch(ucDPCPSelect)
    {
        case DP_PROGRAM_DPCD_0:
            msWriteByteMask(REG_00117D +DP_PHY_REG_OFFSET(ucPortSelect), BIT2, BIT2); // REG_PM_AUX_3E[10]: enable DPCD0 custom address
            msWrite2Byte(REG_001180 +DP_PHY_REG_OFFSET(ucPortSelect), (DP_PROGRAM_DPCD0_ADDERSS & MASKBIT(15:0))); // REG_PM_AUX_40[15:0]: DPCD0 custom address
            msWriteByteMask(REG_001182 +DP_PHY_REG_OFFSET(ucPortSelect), (DP_PROGRAM_DPCD0_ADDERSS >> 16), MASKBIT(3:0)); // REG_PM_AUX_41[3:0]: DPCD0 custom address
            msWriteByteMask(REG_001183 +DP_PHY_REG_OFFSET(ucPortSelect), bAddrGroup? BIT7: 0, BIT7); // REG_PM_AUX_41[15]: DPCD0 enable address block
            break;

        case DP_PROGRAM_DPCD_1:
            msWriteByteMask(REG_00117D +DP_PHY_REG_OFFSET(ucPortSelect), BIT3, BIT3); // REG_PM_AUX_3E[11]: enable DPCD1 custom address
            msWrite2Byte(REG_001184 +DP_PHY_REG_OFFSET(ucPortSelect), (DP_PROGRAM_DPCD1_ADDERSS & MASKBIT(15:0))); // REG_PM_AUX_42[15:0]: DPCD1 custom address
            msWriteByteMask(REG_001186 +DP_PHY_REG_OFFSET(ucPortSelect), (DP_PROGRAM_DPCD1_ADDERSS >> 16), MASKBIT(3:0)); // REG_PM_AUX_43[3:0]: DPCD1 custom address
            msWriteByteMask(REG_001187 +DP_PHY_REG_OFFSET(ucPortSelect), bAddrGroup? BIT7: 0, BIT7); // REG_PM_AUX_43[15]: DPCD1 enable address block
            break;

        case DP_PROGRAM_DPCD_2:
            msWriteByteMask(REG_00117D +DP_PHY_REG_OFFSET(ucPortSelect), BIT4, BIT4); // REG_PM_AUX_3E[12]: enable DPCD2 custom address
            msWrite2Byte(REG_001188 +DP_PHY_REG_OFFSET(ucPortSelect), (DP_PROGRAM_DPCD2_ADDERSS & MASKBIT(15:0))); // REG_PM_AUX_44[15:0]: DPCD2 custom address
            msWriteByteMask(REG_00118A +DP_PHY_REG_OFFSET(ucPortSelect), (DP_PROGRAM_DPCD2_ADDERSS >> 16), MASKBIT(3:0)); // REG_PM_AUX_45[3:0]: DPCD2 custom address
            msWriteByteMask(REG_00118B +DP_PHY_REG_OFFSET(ucPortSelect), bAddrGroup? BIT7: 0, BIT7); // REG_PM_AUX_45[15]: DPCD2 enable address block
            break;

        case DP_PROGRAM_DPCD_3:
            msWriteByteMask(REG_00117D +DP_PHY_REG_OFFSET(ucPortSelect), BIT5, BIT5); // REG_PM_AUX_3E[13]: enable DPCD3 custom address
            msWrite2Byte(REG_00118C +DP_PHY_REG_OFFSET(ucPortSelect), (DP_PROGRAM_DPCD3_ADDERSS & MASKBIT(15:0))); // REG_PM_AUX_46[15:0]: DPCD3 custom address
            msWriteByteMask(REG_00118E +DP_PHY_REG_OFFSET(ucPortSelect), (DP_PROGRAM_DPCD3_ADDERSS >> 16), MASKBIT(3:0)); // REG_PM_AUX_47[3:0]: DPCD3 custom address
            msWriteByteMask(REG_00118F +DP_PHY_REG_OFFSET(ucPortSelect), bAddrGroup? BIT7: 0, BIT7); // REG_PM_AUX_47[15]: DPCD3 enable address block
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
    msWriteByteMask(REG_00117B +DP_PHY_REG_OFFSET(ucPortSelect), bEnable? 0: BIT7, BIT7); // DP AUX recv MCCS cmd mask
    msWriteByte(REG_001179 +DP_PHY_REG_OFFSET(ucPortSelect), BIT7); // Write 1 to clear DP AUX recv MCCS cmd.
    msWriteByte(REG_001179 +DP_PHY_REG_OFFSET(ucPortSelect), 0);
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
    msWriteByteMask(REG_001191 +DP_PHY_REG_OFFSET(ucPortSelect), bEnable? 0: BIT4, BIT4); // REG_PM_AUX_48[12]: programmable DPCD0 mask
    msWriteByte(REG_001192 +DP_PHY_REG_OFFSET(ucPortSelect), BIT0); // REG_PM_AUX_49[0]: Write 1 to clear touch programmable DPCD0 address IRQ.
    msWriteByte(REG_001192 +DP_PHY_REG_OFFSET(ucPortSelect), 0); // REG_PM_AUX_49[0]: Write 1 to clear touch programmable DPCD0 address IRQ.
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
    msWriteByteMask(REG_001191 +DP_PHY_REG_OFFSET(ucPortSelect), bEnable? 0: BIT5, BIT5); // REG_PM_AUX_48[13]: programmable DPCD1 mask
    msWriteByte(REG_001192 +DP_PHY_REG_OFFSET(ucPortSelect), BIT1); // REG_PM_AUX_49[1]: Write 1 to clear touch programmable DPCD1 address IRQ.
    msWriteByte(REG_001192 +DP_PHY_REG_OFFSET(ucPortSelect), 0); // REG_PM_AUX_49[1]: Write 1 to clear touch programmable DPCD1 address IRQ.
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_DP_SetProgramDPCD2Interrupt()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_DP_SetProgramDPCD2Interrupt(BYTE ucPortSelect, Bool bEnable)
{
    msWriteByteMask(REG_001191 +DP_PHY_REG_OFFSET(ucPortSelect), bEnable? 0: BIT6, BIT6); // REG_PM_AUX_48[14]: programmable DPCD2 mask
    msWriteByte(REG_001192 +DP_PHY_REG_OFFSET(ucPortSelect), BIT2); // REG_PM_AUX_49[2]: Write 1 to clear touch programmable DPCD2 address IRQ.
    msWriteByte(REG_001192 +DP_PHY_REG_OFFSET(ucPortSelect), 0); // REG_PM_AUX_49[2]: Write 1 to clear touch programmable DPCD2 address IRQ.
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_DP_SetProgramDPCD3Interrupt()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_DP_SetProgramDPCD3Interrupt(BYTE ucPortSelect, Bool bEnable)
{
    msWriteByteMask(REG_001191 +DP_PHY_REG_OFFSET(ucPortSelect), bEnable? 0: BIT7, BIT7); // REG_PM_AUX_48[15]: programmable DPCD3 mask
    msWriteByte(REG_001192 +DP_PHY_REG_OFFSET(ucPortSelect), BIT3); // REG_PM_AUX_49[3]: Write 1 to clear touch programmable DPCD3 address IRQ.
    msWriteByte(REG_001192 +DP_PHY_REG_OFFSET(ucPortSelect), 0); // REG_PM_AUX_49[3]: Write 1 to clear touch programmable DPCD3 address IRQ.
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
    msWriteByteMask(REG_162F07, BIT5, BIT5); // REG_COMBO_PLL_TOP_03[13]: load new calc KN/KP/KM/DQUANT/FB_DQUANT
    msWriteByteMask(REG_162F07, 0, BIT5); // REG_COMBO_PLL_TOP_03[13]: load new calc KN/KP/KM/DQUANT/FB_DQUANT
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
        msWriteByteMask(REG_1135E4 +DP_HBR2_REG_OFFSET(ucHBR2Select), bEnable? 0: BIT0, BIT0); //[0] AUPLL Big CHG
//        msWriteByteMask(REG_1135E8 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT0, BIT0);
//        msWriteByteMask(REG_1135E8 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0, BIT0);		
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
        msWriteByteMask(REG_1135E4 +DP_HBR2_REG_OFFSET(ucHBR2Select), bEnable? 0: BIT1, BIT1); // [1] VUPLL Big CHG
//        msWriteByteMask(REG_1135E8 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT1, BIT1);
//        msWriteByteMask(REG_1135E8 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0, BIT1);		
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
        msWriteByteMask(REG_1133E0 +DP_HBR2_REG_OFFSET(ucHBR2Select), bEnable? 0: BIT0, BIT0); // [0] TPS1
        msWriteByte(REG_1133E8 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT0);
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
        msWriteByteMask(REG_1133E0 +DP_HBR2_REG_OFFSET(ucHBR2Select), bEnable? 0: BIT1, BIT1); // [1] TP2
        msWriteByte(REG_1133E8 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT1);

	    msWriteByteMask(REG_1133E2 +DP_HBR2_REG_OFFSET(ucHBR2Select), bEnable? 0: BIT1, BIT1); // [1] TP3
    	msWriteByte(REG_1133EA +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT1);
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
#if(COMBO_DP_DAISY_CHAIN_SUPPORT)
	msWriteByteMask(REG_001910, 0, BIT0); // NODIE_EN_LVRT ==> EPONA MCP LEONA
#else
	msWriteByteMask(REG_001910, BIT0, BIT0); // NODIE_EN_LVRT ==> LEONA alone
#endif

	msWriteByteMask(REG_001921, BIT7|BIT6, MASKBIT(7:6));	   // Enable SQ

    Delay1ms(1);

	msWriteByteMask(REG_001921, 0, BIT6);
	msWriteByteMask(REG_001921, BIT6, BIT6);

	msWriteByteMask(REG_001921, 0, BIT7);	   // disable override enable	

    msWriteByteMask(REG_112E00 +DP_PHY_REG_OFFSET(ucPortSelect), 0, BIT7); // Sleect DP port
    msWriteByte(REG_112E01 +DP_PHY_REG_OFFSET(ucPortSelect), 0x00);
    msWriteByte(REG_112E02 +DP_PHY_REG_OFFSET(ucPortSelect), 0x04); //[0]: br mode ov en
    msWriteByte(REG_112E03 +DP_PHY_REG_OFFSET(ucPortSelect), 0x00);	

    msWriteByte(REG_112E14 +DP_PHY_REG_OFFSET(ucPortSelect), 0x40); // 20Bit Clk
    msWriteByte(REG_112E15 +DP_PHY_REG_OFFSET(ucPortSelect), 0x00);	
	msWrite2Byte(REG_112E16 +DP_PHY_REG_OFFSET(ucPortSelect), 0xFFFF); // Clock EN
	msWrite2Byte(REG_112E18 +DP_PHY_REG_OFFSET(ucPortSelect), 0x3FFF); // Clock EN

    msWriteByte(REG_112E20 +DP_PHY_REG_OFFSET(ucPortSelect), 0x10);
    msWriteByte(REG_112E21 +DP_PHY_REG_OFFSET(ucPortSelect), 0x00);
    msWriteByte(REG_112E22 +DP_PHY_REG_OFFSET(ucPortSelect), 0x00);
    msWriteByte(REG_112E23 +DP_PHY_REG_OFFSET(ucPortSelect), 0x10);//0x00
    msWriteByte(REG_112E24 +DP_PHY_REG_OFFSET(ucPortSelect), 0x00);
    msWriteByte(REG_112E25 +DP_PHY_REG_OFFSET(ucPortSelect), 0x10);//0x00
    msWriteByte(REG_112E26 +DP_PHY_REG_OFFSET(ucPortSelect), 0x00);
    msWriteByte(REG_112E27 +DP_PHY_REG_OFFSET(ucPortSelect), 0x10);//0x00

    msWriteByte(REG_112E4A +DP_PHY_REG_OFFSET(ucPortSelect), 0xC8);
    msWriteByte(REG_112E4B +DP_PHY_REG_OFFSET(ucPortSelect), 0x30);
    msWriteByte(REG_112E4C +DP_PHY_REG_OFFSET(ucPortSelect), 0x00);
    msWriteByte(REG_112E4D +DP_PHY_REG_OFFSET(ucPortSelect), 0x08);

    msWriteByte(REG_112E50 +DP_PHY_REG_OFFSET(ucPortSelect), 0x83);
    msWriteByte(REG_112E51 +DP_PHY_REG_OFFSET(ucPortSelect), 0x3E);
    msWriteByte(REG_112E54 +DP_PHY_REG_OFFSET(ucPortSelect), 0x00);
    msWriteByte(REG_112E55 +DP_PHY_REG_OFFSET(ucPortSelect), 0x00);
    msWriteByte(REG_112E56 +DP_PHY_REG_OFFSET(ucPortSelect), 0x18);
    msWriteByte(REG_112E57 +DP_PHY_REG_OFFSET(ucPortSelect), 0x00);
    msWriteByte(REG_112E58 +DP_PHY_REG_OFFSET(ucPortSelect), 0x04);
    msWriteByte(REG_112E59 +DP_PHY_REG_OFFSET(ucPortSelect), 0x00);

    msWriteByte(REG_112E76 +DP_PHY_REG_OFFSET(ucPortSelect), 0x00); // upper boundary of valid clk freq
    msWriteByte(REG_112E77 +DP_PHY_REG_OFFSET(ucPortSelect), 0x1F);

    msWrite2Byte(REG_112E80 +DP_PHY_REG_OFFSET(ucPortSelect), 0x168); // The CR lock reference value for HBR2
    msWrite2Byte(REG_112E84 +DP_PHY_REG_OFFSET(ucPortSelect), 0x2CF); // The CR lock reference value for HBR
    msWrite2Byte(REG_112E86 +DP_PHY_REG_OFFSET(ucPortSelect), 0x360); // The CR lock reference value for RBR
    msWriteByte(REG_112E8A +DP_PHY_REG_OFFSET(ucPortSelect), 0x83);
    msWriteByte(REG_112E8B +DP_PHY_REG_OFFSET(ucPortSelect), 0x00);	

    msWriteByte(REG_112E90 +DP_PHY_REG_OFFSET(ucPortSelect), 0x05); //The CR lock reference correct threshold range for HBR/RBR
    msWriteByte(REG_112E91 +DP_PHY_REG_OFFSET(ucPortSelect), 0x09);

//	msWriteByte(REG_112EA6 +DP_PHY_REG_OFFSET(ucPortSelect), 0x0F);
    msWriteByte(REG_112EAE +DP_PHY_REG_OFFSET(ucPortSelect), 0x0C);
//    msWriteByte(REG_112EAF +DP_PHY_REG_OFFSET(ucPortSelect), 0x01);
    msWriteByteMask(REG_112EAF +DP_PHY_REG_OFFSET(ucPortSelect), BIT1| BIT0, BIT2| BIT1| BIT0); // Enable DP mode	In nonFullRate mode.

    msWriteByte(REG_112EB2 +DP_PHY_REG_OFFSET(ucPortSelect), 0x00);
    msWriteByte(REG_112EB3 +DP_PHY_REG_OFFSET(ucPortSelect), 0x01); // en rt_ctrl_overwrite
    msWriteByte(REG_112EB6 +DP_PHY_REG_OFFSET(ucPortSelect), 0x00); // Enable DP Lane power
    msWriteByte(REG_112EB7 +DP_PHY_REG_OFFSET(ucPortSelect), 0x00); // r-term value controlled	
    msWriteByte(REG_112EB8 +DP_PHY_REG_OFFSET(ucPortSelect), 0x00);
    msWriteByte(REG_112EB9 +DP_PHY_REG_OFFSET(ucPortSelect), 0x00);

    msWriteByte(REG_112EC0 +DP_PHY_REG_OFFSET(ucPortSelect), 0x00);
    msWriteByte(REG_112EC1 +DP_PHY_REG_OFFSET(ucPortSelect), 0x00);
    msWriteByte(REG_112EC2 +DP_PHY_REG_OFFSET(ucPortSelect), 0x00);
    msWriteByte(REG_112EC3 +DP_PHY_REG_OFFSET(ucPortSelect), 0x00);
	
    msWriteByte(REG_112ED6 +DP_PHY_REG_OFFSET(ucPortSelect), 0xF0);
    msWriteByte(REG_112ED7 +DP_PHY_REG_OFFSET(ucPortSelect), 0xF0);
    msWriteByte(REG_112ED8 +DP_PHY_REG_OFFSET(ucPortSelect), 0x00);
    msWriteByte(REG_112ED9 +DP_PHY_REG_OFFSET(ucPortSelect), 0x0F);
//    msWriteByte(REG_112EDC +DP_PHY_REG_OFFSET(ucPortSelect), 0xF0);
    msWriteByte(REG_112EDD +DP_PHY_REG_OFFSET(ucPortSelect), 0x00);

#if(COMBO_DP_DAISY_CHAIN_SUPPORT)
    msWriteByte(REG_112FC4 +DP_PHY_REG_OFFSET(ucPortSelect), 0x04);
    msWriteByte(REG_112FC5 +DP_PHY_REG_OFFSET(ucPortSelect), 0x00);
    msWriteByte(REG_112FC6 +DP_PHY_REG_OFFSET(ucPortSelect), 0x04);
    msWriteByte(REG_112FC7 +DP_PHY_REG_OFFSET(ucPortSelect), 0x00);
    msWriteByte(REG_112FC8 +DP_PHY_REG_OFFSET(ucPortSelect), 0x04);
    msWriteByte(REG_112FC9 +DP_PHY_REG_OFFSET(ucPortSelect), 0x00);
    msWriteByte(REG_112FCA +DP_PHY_REG_OFFSET(ucPortSelect), 0x04);
    msWriteByte(REG_112FCB +DP_PHY_REG_OFFSET(ucPortSelect), 0x00);
#else
    msWriteByte(REG_112FC4 +DP_PHY_REG_OFFSET(ucPortSelect), 0x60);  // Always turn on 3 stage.
    msWriteByte(REG_112FC5 +DP_PHY_REG_OFFSET(ucPortSelect), 0x00);
    msWriteByte(REG_112FC6 +DP_PHY_REG_OFFSET(ucPortSelect), 0x60);
    msWriteByte(REG_112FC7 +DP_PHY_REG_OFFSET(ucPortSelect), 0x00);
    msWriteByte(REG_112FC8 +DP_PHY_REG_OFFSET(ucPortSelect), 0x60);
    msWriteByte(REG_112FC9 +DP_PHY_REG_OFFSET(ucPortSelect), 0x00);
    msWriteByte(REG_112FCA +DP_PHY_REG_OFFSET(ucPortSelect), 0x60);
    msWriteByte(REG_112FCB +DP_PHY_REG_OFFSET(ucPortSelect), 0x00);
#endif

      msWriteByteMask(REG_112F46 +DP_PHY_REG_OFFSET(ucPortSelect), 0x0F, 0x0F); //  _ov enable L0~L3
      msWriteByteMask(REG_112F34 +DP_PHY_REG_OFFSET(ucPortSelect), 0x01, 0x01); //  overwrite  lane 0v alue
      



    DP_HAL_DPRINTF("** _mhal_DP_ComboPHYSetting -> done! port %d", ucPortSelect);	
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_DP_RegenTimingInitialSetting()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_DP_RegenTimingInitialSetting(BYTE ucHBR2Select)
{
	msWriteByteMask(REG_113508 + DP_HBR2_REG_OFFSET(ucHBR2Select), MASKBIT(1:0), MASKBIT(1:0));
	// V parameter
	msWrite2Byte(REG_11350C + DP_HBR2_REG_OFFSET(ucHBR2Select), 0x0F); // V width
	msWriteByte(REG_113540 + DP_HBR2_REG_OFFSET(ucHBR2Select), 0x8); // V front porch
	msWriteByte(REG_113505 + DP_HBR2_REG_OFFSET(ucHBR2Select), 0x10); // V back porch
	msWriteByteMask(REG_113501 + DP_HBR2_REG_OFFSET(ucHBR2Select), BIT4, BIT4);
	// H parameter
	msWrite2Byte(REG_11350A + DP_HBR2_REG_OFFSET(ucHBR2Select), 0x34); // H width

	msWriteByteMask(REG_113502 + DP_HBR2_REG_OFFSET(ucHBR2Select), BIT6, BIT6);
	ForceDelay1ms(1);
	msWriteByteMask(REG_113502 + DP_HBR2_REG_OFFSET(ucHBR2Select), 0x0, BIT6);
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
    // ##                                         BANK 1130                                     ##
    // ##############################################
    msWriteByte(REG_113003 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x1E);
	msWrite2Byte(REG_113086 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x0001); // stb chk err th for new auto eq
    msWriteByte(REG_11308F +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x32); // [5]: k28.5p=k28.5n
#if(COMBO_DP_DAISY_CHAIN_SUPPORT)
    msWriteByteMask(REG_11308F +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT3|BIT2, BIT3|BIT2);
#else
    msWriteByteMask(REG_11308F +DP_HBR2_REG_OFFSET(ucHBR2Select), 0, BIT3|BIT2);
#endif

    // ##############################################
    // ##                                         BANK 1131                                     ##
    // ##############################################
    msWriteByte(REG_113100 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0xF1);
    msWriteByte(REG_11317E +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x81);

    // ##############################################
    // ##                                        BANK 1132                                     ##
    // ##############################################
    msWriteByte(REG_1132A8 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x0C);
    msWrite2Byte(REG_1132F2 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0xFFFF);

    // ##############################################
    // ##                                        BANK 1133                                     ##
    // ##############################################
    msWriteByteMask(REG_113389 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT7, BIT7); // [7]: Enable minimum swing level to bypass squelch check for CR lock.
#if(DP_AUTO_CR_ENABLE)
    msWriteByte(REG_113391 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0xAA); // [15:8]:max swing for all lanes
#else
    //msWriteByte(REG_113391 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0xFF); // minimum swing level to bypass squelch check for CR lock. // when AQE support SW3
    msWriteByte(REG_113391 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0xAA); // [15:8]:max swing for all lanes
#endif
	msWriteByte(REG_1133E8 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0xFF); // clear INT
	msWriteByte(REG_1133E9 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0xFF); // clear INT
	msWriteByte(REG_1133EA +DP_HBR2_REG_OFFSET(ucHBR2Select), 0xFF); // clear INT
	msWriteByte(REG_1133EB +DP_HBR2_REG_OFFSET(ucHBR2Select), 0xFF); // clear INT

	msWriteByte(REG_1133C4 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x03); // [1:0]: clear (training, HDCP22) INT
	msWriteByte(REG_1133C4 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x00); // [1:0]: enable (training, HDCP22) INT

#if(DP_SUPPORT_HBR2)
	//msWriteByte(REG_113300 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x12); // DPCD 1.2 Version
	//msWriteByte(REG_113301 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x14); // DP main link rate 5.4G
#else
	//msWriteByte(REG_113300 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x11); // DPCD 1.1 Version
	//msWriteByte(REG_113301 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x0A); // DP main link rate 2.7G
#endif

	msWriteByte(REG_113302 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x84); // DP Enhanced Frame capability & Max Lane Count
	msWriteByte(REG_113304 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x01); // [0]: DP Number of Receiver Ports
	msWriteByte(REG_11331C +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x80); // [7]: DP Linkstatus andadjust requestupdated
	msWriteByte(REG_11331E +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x00); // LANE0,1 Pre-emphasis & VoltageSwing
	msWriteByte(REG_11331F +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x00); // LANE2,3 Pre-emphasis & VoltageSwing
	msWriteByte(REG_1133DD +DP_HBR2_REG_OFFSET(ucHBR2Select), 0xA0); // [7]: Clear interskew align lock when CR lose lock  //For CTS 20151116
	msWriteByte(REG_113311 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x00);

    // ##############################################
    // ##                                        BANK 1134                                     ##
    // ##############################################
    msWriteByte(REG_113406 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x01); // [0]: reply actual interlanskew for CTS   // [1]: old mode
    msWriteByte(REG_11349E +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x0C); // BIT2 Align Word Boundary Overwrite Enable
    msWriteByte(REG_1134D5 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x03); // [1:0] option enable Training P1 P2 IRQ To BankB 0xE8[0:1] [4:2] Pre-syn zero
#if(COMBO_DP_DAISY_CHAIN_SUPPORT)
    msWriteByteMask(REG_1134F7 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT7, BIT7|BIT5); // [5]: cr done not gate by squelch
#else
    msWriteByteMask(REG_1134F7 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT5|BIT7, BIT7|BIT5); // [5]: cr done not gate by squelch
#endif
    msWriteByte(REG_1134FA +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x0F); // [3]: reply DPCD 202/203 0000 when no ls clk 
    					                                             // [2:0]: AUPLL Frezze interrupt Enable
    // ##############################################
    // ##                                        BANK 1135                                     ##
    // ##############################################
    msWriteByte(REG_11351F +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x00); 
    msWriteByte(REG_113501 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x30); 
    msWriteByte(REG_113507 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x65); 
    msWriteByte(REG_11350D +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x00);
    msWrite2Byte(REG_113518 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x3F20);
    msWriteByte(REG_113521 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x10);
    msWriteByte(REG_113541 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x0C); // [2]:field inv, due to epona bug
    msWrite2Byte(REG_113570 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x3C41); // [13]: de-glitch when frame reset (1680X1050)
						                                                // [7:4]: htotal tolerence

	msWriteByte(REG_1135B2 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0xFF); // Glitch filter use old mode	(Warning: HLBR ISSUE)  No gltch
	msWriteByte(REG_1135B3 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x2F); // [3]: fix high pix rate enable
	msWriteByte(REG_1135B4 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0xFF); // Video PLL freeze disable Audio Enable Frezzen
	//msWriteByte(REG_1135B5 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x30); // [5]: Video M/N, [4]: Audio M/N
	msWriteByte(REG_1135B5 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x20);  //For CTS 20151116
	msWriteByte(REG_1135B8 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x07); // [3]: (bit3=0 use VB_ID rising as frame flag (end of active))
						                                             // [1]: use hactive value to generate DE
						                                             // [0]: use vheight value to generate DE

    msWriteByte(REG_1135BA +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x61); // for CRC test

	msWriteByteMask(REG_1135BC +DP_HBR2_REG_OFFSET(ucHBR2Select), 0, BIT4); // report received vb_id to reg_tu_size

	msWriteByte(REG_1135C8 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x20); // MVID_CENTER[7:0]
	msWriteByte(REG_1135C9 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x00); // MVID_CENTER[15:8]
	msWriteByte(REG_1135CA +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x00); // MVID_CENTER[23:16]
	msWriteByte(REG_1135CC +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x20); // MVID_Range[7:0]
	msWriteByte(REG_1135CD +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x00); // MVID_Range[15:8]
	msWriteByte(REG_1135CE +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x00); // MVID_Range[23:16]
	msWriteByte(REG_1135D8 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x20); // NVID_CENTER[7:0]
	msWriteByte(REG_1135D9 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x00); // NVID_CENTER[15:8]
	msWriteByte(REG_1135DA +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x00); // NVID_CENTER[23:16]
	msWriteByte(REG_1135DC +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x20); // NVID_Range[7:0]
	msWriteByte(REG_1135DD +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x00); // NVID_Range[15:8]
	msWriteByte(REG_1135DE +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x00); // NVID_Range[23:16]

	msWriteByte(REG_1135C0 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x10); // MAUD_CENTER[7:0]
	msWriteByte(REG_1135C1 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x00); // MAUD_CENTER[15:8]
	msWriteByte(REG_1135C2 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x00); // MAUD_CENTER[23:16]
	msWriteByte(REG_1135C4 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x10); // MAUD_Range[7:0]
	msWriteByte(REG_1135C5 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x00); // MAUD_Range[15:8]
	msWriteByte(REG_1135C6 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x00); // MAUD_Range[23:16]
	msWriteByte(REG_1135D0 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x10); // MAUD_CENTER[7:0]
	msWriteByte(REG_1135D1 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x00); // MAUD_CENTER[15:8]
	msWriteByte(REG_1135D2 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x00); // MAUD_CENTER[23:16]
	msWriteByte(REG_1135D4 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x10); // NVID_Range[7:0]
	msWriteByte(REG_1135D5 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x00); // NVID_Range[15:8]
	msWriteByte(REG_1135D6 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x00); // NVID_Range[23:16]

    msWriteByte(REG_1135E4 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0xFF); // mask INT
    msWriteByte(REG_1135E5 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0xFF); // mask INT
    msWriteByte(REG_1135E8 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0xFF); // force clear INT
    msWriteByte(REG_1135E9 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0xFF); // force clear INT
    msWriteByte(REG_1135E8 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x00); // freeze clear INT
    msWriteByte(REG_1135E9 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x00); // freeze clear INT

    DP_HAL_DPRINTF("** _mhal_DP_HBR2InitialSetting -> done! port %d", ucHBR2Select);
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
        //msWriteByteMask(REG_1630DE, ucHBR2Select, MASKBIT(2:0)); //HBR2 port0  for Port 0
        //msWriteByteMask(REG_1630DF, ucHBR2Select, MASKBIT(2:0)); //HBR2 port1  for Port 1
        msWriteByteMask(REG_1630E0, ucHBR2Select, MASKBIT(2:0)); // Clock_pixel
        msWriteByteMask(REG_1630E1, ucHBR2Select, MASKBIT(2:0)); //MAC   // Fixed VPLL 0
        msWriteByteMask(REG_1630E0, BIT3| BIT5| BIT6, BIT3| BIT5| BIT6);
        msWriteByteMask(REG_1630E1, BIT3, BIT3);

        msWriteByteMask(REG_162F96, 0xE0, MASKBIT(7:4)); //[7:4]: vpll0_ls_clk_sel
        msWriteByteMask(REG_162F97, 0, BIT0); // [8]: vpll0_vco_freq_sel

#if DP_AUX_USE_XTAL  
//#if MST9U2   		
        msWriteByteMask(REG_1630E2, BIT2| BIT0, MASKBIT(3:0)); // Not inver AUX MAC clock FROM XTAL  
//#else
//        msWriteByteMask(REG_1630E2, BIT3| BIT2| BIT0, MASKBIT(3:0));  //  Inver AUX
//#endif
#else
        msWriteByteMask(REG_1630E2, BIT2, MASKBIT(2:0)); // MAC clock FROM OSC
#endif

        msWriteByteMask(REG_162DEE, BIT0| BIT1, BIT0| BIT1); //reg_clk_combo2sc_en
        msWriteByteMask(REG_162F82, 0, BIT0); // [0]: vpll_sel_dp_pll_div 
        msWriteByteMask(REG_162F83, 0, BIT0); // [0]: VPLL PD, 
    }
/*
    else if(ucHBR2Select == DP_HBR2_ENGINE_1)
    {
        //msWriteByteMask(REG_1630DE, (ucHBR2Select <<4), MASKBIT(6:4)); //HBR2 port0  for Port 0
        //msWriteByteMask(REG_1630DF, (ucHBR2Select <<4), MASKBIT(6:4)); //HBR2 port1  for Port 1
        msWriteByteMask(REG_1630E4, ucHBR2Select, MASKBIT(2:0)); // Clock_pixel
        msWriteByteMask(REG_1630E5, ucHBR2Select, MASKBIT(2:0)); // MAC  	// Fixed VPLL 1
        msWriteByteMask(REG_1630E4, BIT3| BIT5| BIT6, BIT3| BIT5| BIT6);
        msWriteByteMask(REG_1630E5, BIT3, BIT3);

        msWriteByteMask(REG_162FB6, 0xE0, MASKBIT(7:4)); //[7:4]: vpll0_ls_clk_sel
        msWriteByteMask(REG_162FB7, 0, BIT0); // [8]: vpll0_vco_freq_sel

        msWrite2ByteMask(REG_1630EC, (DP_HBR2_AUDIO_OFFSET(ucHBR2Select) <<7), MASKBIT(9:7)); // VPLL 1 mux    // Fixed VPLL 1

        
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
*/
    ucPortSelect = ucHBR2Select;
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_DP_SpecificFiledSetting()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_DP_SpecificFiledSetting(BYTE ucPortSelect, BYTE ucHBR2Select)
{

	msWriteByte( REG_113352+DP_HBR2_REG_OFFSET(ucHBR2Select), IEEE_OUI_FIRST );             
	msWriteByte( REG_113353+DP_HBR2_REG_OFFSET(ucHBR2Select), IEEE_OUI_SECOND );  
	msWriteByte( REG_113354+DP_HBR2_REG_OFFSET(ucHBR2Select), IEEE_OUI_THIRD );  

	msWriteByte( REG_1131AC+DP_HBR2_REG_OFFSET(ucHBR2Select), DEVICE_STRING_0 );     
	msWriteByte( REG_1131AD+DP_HBR2_REG_OFFSET(ucHBR2Select), DEVICE_STRING_1 );     
	msWriteByte( REG_1131AE+DP_HBR2_REG_OFFSET(ucHBR2Select), DEVICE_STRING_2 );     
	msWriteByte( REG_1131AF+DP_HBR2_REG_OFFSET(ucHBR2Select), DEVICE_STRING_3 );     
	msWriteByte( REG_1131B0+DP_HBR2_REG_OFFSET(ucHBR2Select), DEVICE_STRING_4 );     
	msWriteByte( REG_1131B1+DP_HBR2_REG_OFFSET(ucHBR2Select), DEVICE_STRING_5 );     

	msWriteByte( REG_1131B2+DP_HBR2_REG_OFFSET(ucHBR2Select), HARDWARE_VERSION );  
	
	msWriteByte( REG_1131B3+DP_HBR2_REG_OFFSET(ucHBR2Select), FIRMWARE_MAJOR_VERSION );  
	msWriteByte( REG_1131B4+DP_HBR2_REG_OFFSET(ucHBR2Select), FIRMWARE_MINOR_VERSION );  

       ucPortSelect = ucHBR2Select;
}
//**************************************************************************
//  [Function Name]:
//                  _mhal_DP_AutoCRSetting()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_DP_AutoCRSetting(BYTE ucPortSelect, BYTE ucHBR2Select)
{

#if(DP_AUTO_CR_ENABLE)
	msWriteByteMask(REG_113400 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT0, BIT0);  //[0]: enable
	msWriteByteMask(REG_113400 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0, MASKBIT(7:4));  //[7:4]: LSB
    msWriteByte(REG_113401 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x4B); //[7:0]: MSB
	msWriteByteMask(REG_113400 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0, MASKBIT(7:4));  //[7:4]: LSB
#endif //#if(DP_AUTO_CR_ENABLE)

	ucPortSelect = ucHBR2Select;
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

	//--------------------------PHY setting------------------------------

	// Coarse Tune
	//msWrite2Byte(REG_112E52 +DP_PHY_REG_OFFSET(ucPortSelect), 0x0003);  //[15:0]: one coarse tune step  //20160303
	msWrite2Byte(REG_112E52 +DP_PHY_REG_OFFSET(ucPortSelect), 0x0008);  //[15:0]: one coarse tune step
	msWriteByte(REG_11348C +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x60);	//[7:0]: EQ done threshold
	msWrite2Byte(REG_112F20 +DP_PHY_REG_OFFSET(ucPortSelect), 0x1F07);  //[4:0]: EQ lower bound	//[12:8]: EQ high bound
	msWriteByteMask(REG_112E51 +DP_PHY_REG_OFFSET(ucPortSelect), 0x30, 0x70);  //[14:12]: EQ step
	msWriteByteMask(REG_112F22 +DP_PHY_REG_OFFSET(ucPortSelect), 0x00, 0x01);  //[0]: trianular/wrap sweep
	msWriteByteMask(REG_112E67 +DP_PHY_REG_OFFSET(ucPortSelect), 0x07, 0x3F);  //[13:8]: EQ initial value
	msWriteByteMask(REG_112E2B +DP_PHY_REG_OFFSET(ucPortSelect), 0x00, 0xC0);  //[14]: bypass frequency lock check  //[15]: bypass symbol lock check
	msWriteByteMask(REG_112E4D +DP_PHY_REG_OFFSET(ucPortSelect), 0x80, 0x80);  //[15]: enable time-out for coarse-tune
	//msWriteByteMask(REG_112E5E +DP_PHY_REG_OFFSET(ucPortSelect), 0x20, 0x3F);  //[5:0]: max. EQ steps for coarse-tune   //20160303
	msWriteByteMask(REG_112E5E +DP_PHY_REG_OFFSET(ucPortSelect), 0x3F, 0x3F);  //[5:0]: max. EQ steps for coarse-tune
	// Fine Tune
	msWriteByte(REG_112F23 +DP_PHY_REG_OFFSET(ucPortSelect), 0x00);	//[13:8]: search pattern
	msWriteByteMask(REG_112E50 +DP_PHY_REG_OFFSET(ucPortSelect), 0x80, 0x80);  //[7]: turn-on speed-up
	msWrite2Byte(REG_112E56 +DP_PHY_REG_OFFSET(ucPortSelect), 0x0007);  //[15:0]: accumulation time
	msWrite2Byte(REG_112E58 +DP_PHY_REG_OFFSET(ucPortSelect), 0x0002);  //[15:0]: accumulation mask
	msWriteByte(REG_112E4F +DP_PHY_REG_OFFSET(ucPortSelect), 0x05);	//[13:8]: accumulation threshold
	msWriteByteMask(REG_112E51 +DP_PHY_REG_OFFSET(ucPortSelect), 0x03, 0x03);  //[9:8]: choose min/present/avg/max EQ state after settling
	msWriteByteMask(REG_112F22 +DP_PHY_REG_OFFSET(ucPortSelect), 0x00, 0x02);  //[1]: no clamping
	msWrite2Byte(REG_112FA2 +DP_PHY_REG_OFFSET(ucPortSelect), 0x0400);  //[10]: no clamping	//[9]: enable initial value shift  //[8]: initial value shift-up/down  //[5:0]: initial value shift value
	msWriteByteMask(REG_112E4D +DP_PHY_REG_OFFSET(ucPortSelect), 0x0A, 0x3F);  //[13:8]: fine tune count for settling
	msWriteByteMask(REG_112E51 +DP_PHY_REG_OFFSET(ucPortSelect), 0x04, 0x04);  //[10]: enable fine tune time-out
	msWrite2Byte(REG_112E5A +DP_PHY_REG_OFFSET(ucPortSelect), 0x0028);  //[15:0]: fine tune times to time-out
	msWriteByteMask(REG_112E50 +DP_PHY_REG_OFFSET(ucPortSelect), 0x00, 0x7F);  //[6:0]: under/over count threshold to modify EQ
	msWriteByteMask(REG_112F22 +DP_PHY_REG_OFFSET(ucPortSelect), 0x00, 0x10);  //[4]: new/old algorithm for auto EQ, 0:new, YYAABAY, 1:old
	msWriteByte(REG_112FA0 +DP_PHY_REG_OFFSET(ucPortSelect), 0x00);	//[4:0]: final EQ state offset	//[7]: final EQ state add/sub offset

    // CR lose/lock threshold for AEQ
	msWriteByteMask(REG_112F66 +DP_PHY_REG_OFFSET(ucPortSelect), BIT0, BIT0);  //[0]: enable
	msWriteByte(REG_112F64 +DP_PHY_REG_OFFSET(ucPortSelect), 0x05);            //[7:0]: The CR lock reference correct threshold range for HBR in auto-eq mode // ??? Remove after ECO
	msWriteByte(REG_112F65 +DP_PHY_REG_OFFSET(ucPortSelect), 0x09);            //[15:8]: The CR lock reference failure threshold range for HBR in auto-eq mode

    // choice eq result 0 : min 1: original 2: mean 3:max
	msWriteByteMask( REG_112E51 +DP_PHY_REG_OFFSET(ucPortSelect), BIT1 , BIT1|BIT0);

       msWriteByteMask( REG_112E68 +DP_PHY_REG_OFFSET(ucPortSelect), BIT1 , BIT1);   //AEQ time out setting
	mhal_DP_AutoEQEnable(ucPortSelect, ucHBR2Select, TRUE);

	//--------------------------MAC setting------------------------------
#if(!DP_AUTO_EQ_NEW_MODE)

	msWrite2Byte(REG_113004 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x2018);
																		// [7:0]: reg_timer_thd 
																		// One phase scan time 
																		// = 4*reg_timer_thd*xtal_period
																		// [15:8]: reg_error_cnt_mask_thd 
																		// Mask time for AC couple
																		// = reg_error_cnt_mask_thd*(1/link rate)

    msWrite2Byte(REG_113006 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x1010);	
																		// [15:8]: reg_eq_ph_err_cnt_th
																		// Good Phase Threshold

    msWrite2Byte(REG_113008 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x0C80);		
												                        // [7:6]: # of scan phase
																		// 0: 10 phases, 1: 20 phases, 2: 40 phases

    msWrite2Byte(REG_113000 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x8085);	
												                        // [0]: AEQ enable
																		// [3:2]: 1st pre-emphasis
																		// [5:4]: 2nd pre-emphasis
																		// [7:6]: 3rd pre-emphasis
	
//    msWrite2Byte(REG_113002 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x1E11);	// 1 pre-emphasis AEQ
	msWrite2Byte(REG_113002 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x1E01); // 2 pre-emphasis AEQ
//    msWrite2Byte(REG_113002 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x1E81);	// 3 pre-emphasis AEQ

	msWrite2Byte(REG_113080 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x0000); // [0]: AEQ new mode
	
#else

	// Scan GoodPhase
	// msWrite2Byte(REG_11300C +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x200F); // EPONA
    msWrite2Byte(REG_113004 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x2018); 
                                                                        // [7:0]: reg_timer_thd 
                                                                        // One phase scan time 
                                                                        // = 4*reg_timer_thd*xtal_period
                                                                        // [15:8]: reg_error_cnt_mask_thd 
                                                                        // Mask time for AC couple
                                                                        // = reg_error_cnt_mask_thd*(1/link rate)
    msWrite2Byte(REG_113006 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x1010); 
                                                                        // [15:8]: reg_eq_ph_err_cnt_th
                                                                        // Good Phase Threshold
    msWrite2Byte(REG_113008 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x0C80); 
                                                                        // [7:6]: # of scan phase
                                                                        // 0: 10 phases, 1: 20 phases, 2: 40 phases
    msWrite2Byte(REG_113000 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0xC091); 
                                                                        // [0]: AEQ enable
                                                                        // [3:2]: 1st pre-emphasis
                                                                        // [5:4]: 2nd pre-emphasis
                                                                        // [7:6]: 3rd pre-emphasis
                                                                        // [14]: reg_mask_symbol_err_clr
    msWrite2Byte(REG_113088 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x3210); 
                                                                        // [1:0]: reg_sw0_set0_swing
                                                                        // [3:2]: reg_sw0_set0_pre_emph
                                                                        // [5:4]: reg_sw1_set0_swing
                                                                        // [7:6]: reg_sw1_set0_pre_emph
                                                                        // [9:8]: reg_sw2_set0_swing
                                                                        // [11:10]: reg_sw2_set0_pre_emph
                                                                        // [13:12]: reg_sw3_set0_swing
                                                                        // [15:14]: reg_sw3_set0_pre_emph
    msWrite2Byte(REG_11308A +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x6654); 
                                                                        // [1:0]: reg_sw0_set1_swing
                                                                        // [3:2]: reg_sw0_set1_pre_emph
                                                                        // [5:4]: reg_sw1_set1_swing
                                                                        // [7:6]: reg_sw1_set1_pre_emph
                                                                        // [9:8]: reg_sw2_set1_swing
                                                                        // [11:10]: reg_sw2_set1_pre_emph
                                                                        // [13:12]: reg_sw3_set1_swing
                                                                        // [15:14]: reg_sw3_set1_pre_emph
    msWrite2Byte(REG_11308C +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x9998); 
                                                                        // [1:0]: reg_sw0_set2_swing
                                                                        // [3:2]: reg_sw0_set2_pre_emph
                                                                        // [5:4]: reg_sw1_set2_swing
                                                                        // [7:6]: reg_sw1_set2_pre_emph
                                                                        // [9:8]: reg_sw2_set2_swing
                                                                        // [11:10]: reg_sw2_set2_pre_emph
                                                                        // [13:12]: reg_sw3_set2_swing
                                                                        // [15:14]: reg_sw3_set2_pre_emph
#if(DP_HW_ERROR_COUNT_FUNCTION)
    msWriteByte(REG_113084 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x4B);    // 400us
#else
    msWriteByte(REG_113084 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x00); 
#endif
                                                                        // reg_stb_chk_time stable check time
                                                        			 // = (64*reg_stb_chk_time+1)*xtal_period
    msWriteByte(REG_113085 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x0A); 
                                                                        // reg_stb_chk_mask_time Mask time for stable check
                                                           			 // = (16*reg_stb_chk_time+1)*xtal_period
    msWrite2Byte(REG_113086 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x0001);	// reg_stb_chk_err_th stable check error threshold


    // 0x113082 useful when 0x113080[9:8]==11
    // [3:0] set0_plus
    // [7:4] set1_plus
    // [11:8] set2_plus
	msWrite2Byte(REG_113082 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x0000); 

    // 0x113080
    // [0]: AEQ new mode
    // [1]: auto eq swing adjust enable
    // [8]: 0: each lane can has different set request
    //       1: each lane has the same set request
    // [10:9]: useful when[8]==1
    //      00: choose highest set among each lane
    //      10: choose lowest set among each lane
    //      01: choose set with largest good phase, prefer higher set
    //      11: choose set with largest good phase, prefer lower set
    // [11]: cont. good phase calculate
    // [12]: don't care 5.4G mode
    // [13]: fast 2 auto eq
    // [14]: stable check select, 0: TPS2/3 pattern ; 1: 8b/10b

    // New 1 pre-emphasis AEQ with SW adjust
    //	msWrite2Byte(REG_113002 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x1E11); // [4]: fast auto EQ
    //	msWrite2Byte(REG_113080 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x4703);

    // New 2 pre-emphasis AEQ with SW adjust
    msWrite2Byte(REG_113002 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x1E01);
    msWrite2Byte(REG_113080 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x6703); // [13]: fast2 auto EQ

    // New 3 pre-emphasis AEQ with SW adjust
    //	msWrite2Byte(REG_113002 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x1E01);
    //	msWrite2Byte(REG_113080 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x4703); 

    // New 1 pre-emphasis AEQ without SW adjust
    //	msWrite2Byte(REG_113002 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x1E11); // [4]: fast auto EQ
    //	msWrite2Byte(REG_113080 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x4701);

    // New 2 pre-emphasis AEQ without SW adjust
    //	msWrite2Byte(REG_113002 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x1E01);
    //	msWrite2Byte(REG_113080 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x6701); // [13]: fast2 auto EQ

    // New 3 pre-emphasis AEQ without SW adjust
//    msWrite2Byte(REG_113002 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x1E01);
//    msWrite2Byte(REG_113080 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x4701);

#endif // #if(!DP_AUTO_EQ_NEW_MODE)

#else

    // DP force EQ
#if(COMBO_DP_DAISY_CHAIN_SUPPORT)
    msWrite2Byte(REG_112EC0 +DP_PHY_REG_OFFSET(ucPortSelect), 0x0303);     //  EQ0=3, EQ1=3
    msWrite2Byte(REG_112EC2 +DP_PHY_REG_OFFSET(ucPortSelect), 0x0303);     //  EQ2=3, EQ3=3
    msWriteByte(REG_112EDC +DP_PHY_REG_OFFSET(ucPortSelect), 0xF0);
#else
    msWrite2Byte(REG_112EC0 +DP_PHY_REG_OFFSET(ucPortSelect), 0x1010);	   //  EQ0=16, EQ1=16
    msWrite2Byte(REG_112EC2 +DP_PHY_REG_OFFSET(ucPortSelect), 0x1010);	   //  EQ2=16, EQ3=16
    msWriteByte(REG_112EDC +DP_PHY_REG_OFFSET(ucPortSelect), 0xF0);
#endif

    ucPortSelect = ucHBR2Select;
    
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
    BYTE ucInterruptMask_0 = 0;
    BYTE ucInterruptMask_1 = 0;	
    
    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
//        ucInterruptMask = (BIT0 << ucPortSelect);
    }
    else // HBR2 path
    {
        ucInterruptMask_0 = (BIT7 << ucHBR2Select);
        ucInterruptMask_1 = (BIT0 << ucHBR2Select);
    }

    msWriteByteMask(REG_163070, bEnable? ucInterruptMask_0: 0, ucInterruptMask_0);
    msWriteByteMask(REG_163071, bEnable? ucInterruptMask_1: 0, ucInterruptMask_1);

    ucPortSelect = ucHBR2Select;
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
        msWriteByteMask(REG_113559 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0, BIT5); // [5]: eo2p mode
        msWriteByteMask(REG_1630E1 +ucHBR2Select *4, 0, BIT5); // [5]: stream_clk_eo2p_hbr2_0

        if(msReadByte(REG_11330E +DP_HBR2_REG_OFFSET(ucHBR2Select)) == 0x14)
        {
			msWriteByteMask(REG_120FDE +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT4, BIT4);	// [4]: vpll_clk_eo2p, [3]: vpll_clk_en
			msWriteByte(REG_1134D1 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x00); // [7]: vpll_fin_mode
			msWrite2Byte(REG_162F96 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x01D0); // [8]: vpll0_vco_freq_sel, [7:4]: vpll0_ls_clk_sel
        }
		else
		{
			msWriteByteMask(REG_120FDE +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT4, BIT4);	// [4]: vpll_clk_eo2p, [3]: vpll_clk_en
			msWriteByte(REG_1134D1 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x00); // [7]: vpll_fin_mode
			msWrite2Byte(REG_162F96 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x00E0); // [8]: vpll0_vco_freq_sel, [7:4]: vpll0_ls_clk_sel
		}
    }
    else if(ucSplitSelect == DP_SPLIT_MODE_2P)
    {
        msWriteByteMask(REG_113559 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT5, BIT5); // [5]: eo2p mode
        msWriteByteMask(REG_1630E1 +ucHBR2Select *4, BIT5, BIT5); // [5]: stream_clk_eo2p_hbr2_0

        if(msReadByte(REG_11330E +DP_HBR2_REG_OFFSET(ucHBR2Select)) == 0x14)
        {
			msWriteByteMask(REG_120FDE +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT4, BIT4);	// [4]: vpll_clk_eo2p, [3]: vpll_clk_en
			msWriteByte(REG_1134D1 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x80); // [7]: vpll_fin_mode
			msWrite2Byte(REG_162F96 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x00D0); // [8]: vpll0_vco_freq_sel, [7:4]: vpll0_ls_clk_sel
        }
		else
		{
			msWriteByteMask(REG_120FDE +DP_HBR2_REG_OFFSET(ucHBR2Select), 0, BIT4);	// [4]: vpll_clk_eo2p, [3]: vpll_clk_en
			msWriteByte(REG_1134D1 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x00); // [7]: vpll_fin_mode
			msWrite2Byte(REG_162F96 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x00E0); // [8]: vpll0_vco_freq_sel, [7:4]: vpll0_ls_clk_sel
        }

        bSplit2P = TRUE;
    }
/*
    if(ucHBR2Select == DP_HBR2_ENGINE_0)
    {
        msWriteByteMask(REG_162F97 +DP_HBR2_REG_OFFSET(ucHBR2Select), bSplit2P? 0: BIT0, BIT0);
    }

    else if(ucHBR2Select == DP_HBR2_ENGINE_1)
    {
        msWriteByteMask(REG_162FB7 +DP_HBR2_REG_OFFSET(ucHBR2Select), bSplit2P? 0: BIT0, BIT0);
    }
*/
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
    WORD usCount = 50;
    	
    msWriteByteMask(REG_0011FC +DP_PHY_REG_OFFSET(ucPortSelect), BIT2, BIT2); // REG_PM_AUX_7E[2]:
      
    // Must fine tune
    while(usCount > 0)
    {
        usCount--;

        _nop_();
    }

    ucDDCData = msReadByte(REG_001175 +DP_PHY_REG_OFFSET(ucPortSelect)); // REG_PM_AUX_3A[15:8]: mcu read rx fifo data

    return ucDDCData;
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_DP_PutAuxDDCData()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void _mhal_DP_PutAuxDDCData(BYTE ucPortSelect, BYTE ucDDCData)
{
    WORD usCount = 50;
      
    // Must fine tune
    while(usCount > 0)
    {
        usCount--;

        _nop_();
    }

    msWriteByte(REG_0011D3 +DP_PHY_REG_OFFSET(ucPortSelect), ucDDCData); // REG_PM_AUX_69[15:8]: 
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
#if 1
BYTE g_ucDDC_TxRecord=0;
BYTE dpNullMsg[3]={0x6e, 0x80, 0xbe};
BYTE u8DDC_RxRecord = 0;
BYTE AuxIndex=0;
//extern BYTE D2B_FIFOMODE_DDCPORT2INPUT(BYTE ComboPort);
void _mhal_DP_MCCSReceiveProc(BYTE ucPortSelect)
{
    BYTE AuxData;
    BYTE u8AUXCH_RxCmd;
    BYTE u8DDC_Length;

    u8AUXCH_RxCmd = msRegs(REG_001138 + DP_PHY_REG_OFFSET(ucPortSelect))&0x0F;
    u8DDC_Length = msRegs(REG_00113A+ DP_PHY_REG_OFFSET(ucPortSelect));
    //===============//<write>PC Read Command============================
    if(u8AUXCH_RxCmd & _BIT0)    //DP Tx
    {
        if (u8DDC_Length)
        {
            if(rxStatus == DDC2B_CLEAR && txLength)
            {
                 while(u8DDC_Length-- && txLength)
                 {
                    WORD  temp3=50;
                    while(temp3--)
                    {
                        _nop_();
                    };
                    if(g_ucDDC_TxRecord)
                    {
                    	   //msRegs(REG_0011D3 + DP_PHY_REG_OFFSET(ucPortSelect))=CCJ[g_ucDDC_TxRecord-1];
                        msRegs(REG_0011D3 + DP_PHY_REG_OFFSET(ucPortSelect))=DDCBuffer[g_ucDDC_TxRecord-1];
                        //msRegs(REG_0011D3 + Aux)=GUIDData[g_ucDDC_TxRecord-1];
                        txLength--;
                    }
                    else
                        msRegs(REG_0011D3 + DP_PHY_REG_OFFSET(ucPortSelect))=DDC2B_DEST_ADDRESS;
                     g_ucDDC_TxRecord++;
                }
            }
            else
            {
                while(u8DDC_Length--)
                {
                	WORD  temp3=50;
                    while(temp3--)
                    {
                        _nop_();
                    };    
                    
                    msRegs(REG_0011D3 + DP_PHY_REG_OFFSET(ucPortSelect))= dpNullMsg[g_ucDDC_TxRecord++];
                }
            }
        }
    }
    //===============//<write>PC Write Command============================

    else if(u8AUXCH_RxCmd==0x00 || u8AUXCH_RxCmd==0x04)    //DP Rx
    {
        if (u8DDC_Length)
        {
            if(u8DDC_RxRecord==0)
            {
                u8DDC_RxRecord=TRUE;
                msRegs(REG_0011FC + DP_PHY_REG_OFFSET(ucPortSelect))= msRegs(REG_0011FC + DP_PHY_REG_OFFSET(ucPortSelect))|_BIT2;
                //_nop_();
                AuxData= msRegs(REG_001175 + DP_PHY_REG_OFFSET(ucPortSelect));
                              
                u8DDC_Length--;
            }
            while(u8DDC_Length--)
            {
                WORD  temp3=50;
                 msRegs(REG_0011FC + DP_PHY_REG_OFFSET(ucPortSelect))= msRegs(REG_0011FC + DP_PHY_REG_OFFSET(ucPortSelect))|_BIT2;
                 
                while(temp3--)
                {
                    _nop_();
                };
                DDCBuffer[rxIndex++] = msRegs(REG_001175 + DP_PHY_REG_OFFSET(ucPortSelect));
               // RX[rxCont++] = DDCBuffer[rxIndex-1];
            }
        }

        if (u8AUXCH_RxCmd==0x00)
        {  
			msWriteByteMask(REG_113333,BIT0,BIT0);	
			u8DDC_RxRecord=FALSE;
			g_ucDDC_TxRecord=0;
			rxStatus = DDC2B_COMPLETED;
			txLength=0;
			rxInputPort = D2B_FIFOMODE_DDCPORT2INPUT(4);
			
         }

    }

}

  #else
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
    static BYTE uc_TxDDCIndex = 0;
    static BYTE uc_RxDDCIndex = 0;
    BYTE uctemp = 0;
    BYTE ucAuxCommand = 0;
    BYTE ucAuxLength = 0;

    ucAuxCommand = msReadByte(REG_001138 +DP_PHY_REG_OFFSET(ucPortSelect))&0x0F; // REG_PM_AUX_1C[3:0]: DP AUX CH Rx command
    ucAuxLength = msReadByte(REG_00113A +DP_PHY_REG_OFFSET(ucPortSelect)); // REG_PM_AUX_1D[7:0]: DP AUX CH Rx length

    if(ucAuxLength > DP_AUX_COMMAND_MAX_LENGTH)
    {
        ucAuxLength = DP_AUX_COMMAND_MAX_LENGTH;
    }
    
    if((ucAuxCommand &BIT0) == BIT0)
    {
        if(mdrv_DDC2BI_GetDPSendFlag(ucAuxLength, uc_TxDDCIndex, ucDDCBuffer)) // DDC Tx buffer not empty
        {
            for(uctemp = 0; uctemp <ucAuxLength; uctemp++)
            {
                _mhal_DP_PutAuxDDCData(ucPortSelect, ucDDCBuffer[uctemp] );                  	 

                uc_TxDDCIndex++;
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
                _mhal_DP_PutAuxDDCData(ucPortSelect, ucDDCNullMessage[uctemp]);
            }

            // Reset DDC buffer index when Tx buffer empty
        }
    }
    else if((ucAuxCommand == 0x00) || (ucAuxCommand == 0x04))
    {
        if(ucAuxLength > 0)
        {
            if(uc_RxDDCIndex == 0)
            {
                // Ignore first data
                uctemp = _mhal_DP_GetAuxDDCData(ucPortSelect);

                ucAuxLength--;
                uc_RxDDCIndex++;
            }

            for(uctemp = 0; uctemp <ucAuxLength; uctemp++)
            {
                ucDDCBuffer[uc_RxDDCIndex -1] = _mhal_DP_GetAuxDDCData(ucPortSelect);		
                uc_RxDDCIndex++;
            }
        }

        if(ucAuxCommand == 0x00)
        {
            if(uc_RxDDCIndex > 0)
            {
                mdrv_DDC2BI_SetDPReceiveStatus(ucPortSelect, uc_RxDDCIndex, ucDDCBuffer);

                // Reset DDC buffer index when Rx receive finish
                uc_RxDDCIndex = 0;
                uc_TxDDCIndex = 0;
            }
        }
    }
}
#endif
//**************************************************************************
//  [Function Name]:
//                  _mhal_DP_SquelchLevelSetting()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_DP_SquelchLevelSetting(BYTE ucPortSelect, BYTE ucSquelchLevel)
{
    ucSquelchLevel = ucSquelchLevel & MASKBIT(3:0);
 
    switch(ucPortSelect)
    {
        case DP_INPUT_SELECT_PORT0:
            //break;

        case DP_INPUT_SELECT_PORT1:
            //break;

        case DP_INPUT_SELECT_PORT2:
            //break;

        case DP_INPUT_SELECT_PORT3:
            //break;

        case DP_INPUT_SELECT_PORT4:
			if(ucSquelchLevel == 0x0F)
			{
                msWriteByteMask(REG_001921, BIT7, MASKBIT(7:6)); 	 // disable SQ
			}
			else
			{
                msWriteByteMask(REG_001921, BIT6, MASKBIT(7:6)); 	 // Enable SQ
			}
			
//            msWriteByteMask(REG_001921, ucSquelchLevel, MASKBIT(3:0)); // DP squelch threshold setting for nodie domain			
//            msWriteByteMask(REG_001921, BIT7|BIT6, MASKBIT(7:6));      // Enable SQ
			//msWriteByteMask(REG_0017C2, BIT2, BIT2);                                  // Enable SQH monitor input signal   
            break;

        case DP_INPUT_SELECT_PORT5:
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
            
            //break;

        case DP_INPUT_SELECT_PORT1:
            
            //break;

        case DP_INPUT_SELECT_PORT2:
            
            //break;

        case DP_INPUT_SELECT_PORT3:
            
            //break;
            
        case DP_INPUT_SELECT_PORT4:
            msWriteByteMask(REG_1133E1, bEnable? 0: BIT5, BIT5); // REG_DP_BANKB_70[12]: DP HDCP link integrity failure interrupt mask
            msWriteByteMask(REG_1133EB, BIT5, BIT5); // REG_DP_BANKB_74[12]: DP HDCP link integrity failure interrupt
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
            
            //break;

        case DP_INPUT_SELECT_PORT1:
            
            //break;

        case DP_INPUT_SELECT_PORT2:
            
            //break;

        case DP_INPUT_SELECT_PORT3:
            
            //break;

        case DP_INPUT_SELECT_PORT4:
            msWriteByteMask(REG_113A9F, MASKBIT(1:0), MASKBIT(1:0)); // REG_DPCD_HDCP22_4_4F[9:8]: reg_rx_status_hw_md and reg_reauth_req_hw_md
            break;
            
        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_DP_SetHDCP2InitialInterrupt()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void _mhal_DP_SetHDCP2InitialInterrupt(BYTE ucPortSelect, Bool bEnable)
{
    switch(ucPortSelect)
    {
        case DP_INPUT_SELECT_PORT0:
            
            //break;

        case DP_INPUT_SELECT_PORT1:
            
            //break;

        case DP_INPUT_SELECT_PORT2:
            
            //break;

        case DP_INPUT_SELECT_PORT3:
            
            //break;

        case DP_INPUT_SELECT_PORT4:
			msWriteByteMask(REG_1133C2, bEnable? 0: BIT0, BIT0);	 // [0]: enable (HDCP22) INT
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
            
            //break;

        case DP_INPUT_SELECT_PORT1:
            
            //break;

        case DP_INPUT_SELECT_PORT2:
            
            //break;

        case DP_INPUT_SELECT_PORT3:
            
            //break;

        case DP_INPUT_SELECT_PORT4:
            msWriteByteMask(REG_113AE2, bEnable? 0: BIT1, BIT1); // REG_DPCD_HDCP22_4_71[1]: reg_ake_init_irq_mask
            msWriteByteMask(REG_113AEA, bEnable? 0: MASKBIT(1:0), MASKBIT(1:0)); // REG_DPCD_HDCP22_4_75[1:0]: reg_r_tx_irq_mask and reg_tx_caps_irq_mask
            
            msWriteByteMask(REG_113AE6, BIT1, BIT1); // REG_DPCD_HDCP22_4_73[1]: reg_ake_init_irq_clr
            msWriteByteMask(REG_113AE6, 0, BIT1); // REG_DPCD_HDCP22_4_73[1]: reg_ake_init_irq_clr
            msWriteByteMask(REG_113AEE, MASKBIT(1:0), MASKBIT(1:0)); // REG_DPCD_HDCP22_4_77[1:0]:  reg_r_tx_irq_clr and reg_tx_caps_irq_clr
            msWriteByteMask(REG_113AEE, 0, MASKBIT(1:0)); // REG_DPCD_HDCP22_4_77[1:0]:  reg_r_tx_irq_clr and reg_tx_caps_irq_clr            
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
            
            //break;

        case DP_INPUT_SELECT_PORT1:
            
            //break;

        case DP_INPUT_SELECT_PORT2:
            
            //break;

        case DP_INPUT_SELECT_PORT3:
            
            //break;

        case DP_INPUT_SELECT_PORT4:
            msWriteByteMask(REG_113AE2, bEnable? 0: BIT2, BIT2); // REG_DPCD_HDCP22_4_71[2]: reg_ake_no_stored_km_irq_mask
            msWriteByteMask(REG_113AE6, BIT2, BIT2); // REG_DPCD_HDCP22_4_73[2]: reg_ake_no_stored_km_irq_clr
            msWriteByteMask(REG_113AE6, 0, BIT2); // REG_DPCD_HDCP22_4_73[2]: reg_ake_no_stored_km_irq_clr
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
            
            //break;

        case DP_INPUT_SELECT_PORT1:
            
            //break;

        case DP_INPUT_SELECT_PORT2:
            
            //break;

        case DP_INPUT_SELECT_PORT3:
            
            //break;

        case DP_INPUT_SELECT_PORT4:
            msWriteByteMask(REG_113AE2, bEnable? 0: BIT3, BIT3); // REG_DPCD_HDCP22_4_71[3]: reg_ake_stored_km_irq_mask
            msWriteByteMask(REG_113AEA, bEnable? 0: MASKBIT(3:2), MASKBIT(3:2)); // REG_DPCD_HDCP22_4_75[3:2]: reg_ekh_km_irq_mask and reg_m_irq_mask
            msWriteByteMask(REG_113AE6, BIT3, BIT3); // REG_DPCD_HDCP22_4_73[3]: reg_ake_stored_km_irq_clr
            msWriteByteMask(REG_113AE6, 0, BIT3); // REG_DPCD_HDCP22_4_73[3]: reg_ake_stored_km_irq_clr
            msWriteByteMask(REG_113AEE, MASKBIT(3:2), MASKBIT(3:2)); // REG_DPCD_HDCP22_4_77[3:2]:  reg_ekh_km_irq_clr and reg_m_irq_clr
            msWriteByteMask(REG_113AEE, 0, MASKBIT(3:2)); // REG_DPCD_HDCP22_4_77[3:2]:  reg_ekh_km_irq_clr and reg_m_irq_clr
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
            
            //break;

        case DP_INPUT_SELECT_PORT1:
            
            //break;

        case DP_INPUT_SELECT_PORT2:
            
            //break;

        case DP_INPUT_SELECT_PORT3:
            
            //break;

        case DP_INPUT_SELECT_PORT4:
            msWriteByteMask(REG_113AE2, bEnable? 0: BIT4, BIT4); // REG_DPCD_HDCP22_4_71[4]: reg_locality_check_irq_mask
            msWriteByteMask(REG_113AE6, BIT4, BIT4); // REG_DPCD_HDCP22_4_73[4]: reg_locality_check_irq_clr
            msWriteByteMask(REG_113AE6, 0, BIT4); // REG_DPCD_HDCP22_4_73[4]: reg_locality_check_irq_clr
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
            
            //break;

        case DP_INPUT_SELECT_PORT1:
            
            //break;

        case DP_INPUT_SELECT_PORT2:
            
            //break;

        case DP_INPUT_SELECT_PORT3:
            
            //break;

        case DP_INPUT_SELECT_PORT4:
            msWriteByteMask(REG_113AE2, bEnable? 0: BIT5, BIT5); // REG_DPCD_HDCP22_4_71[5]: reg_ske_irq_mask
            msWriteByteMask(REG_113AEA, bEnable? 0: MASKBIT(5:4), MASKBIT(5:4)); // REG_DPCD_HDCP22_4_75[5:4]: reg_edkey_ks_irq_mask and reg_r_iv_irq_mask
            msWriteByteMask(REG_113AE6, BIT5, BIT5); // REG_DPCD_HDCP22_4_73[5]: reg_ske_irq_clr
            msWriteByteMask(REG_113AE6, 0, BIT5); // REG_DPCD_HDCP22_4_73[5]: reg_ske_irq_clr
            msWriteByteMask(REG_113AEE, MASKBIT(5:4), MASKBIT(5:4)); // REG_DPCD_HDCP22_4_77[5:4]:  reg_edkey_ks_irq_clr and reg_r_iv_irq_clr
            msWriteByteMask(REG_113AEE, 0, MASKBIT(5:4)); // REG_DPCD_HDCP22_4_77[5:4]:  reg_edkey_ks_irq_clr and reg_r_iv_irq_clr
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
        msWriteByteMask(REG_000392, 0, BIT1);           

        msWriteByteMask(REG_000392, BIT2, BIT3|BIT2);                                                                          // Aux Clock SRC
        msWriteByteMask(REG_000390, 0, BIT5|BIT4);        // DDC Clock use Xtal.
#else
        msWriteByteMask(REG_000392, 0, BIT3|BIT2);
        msWriteByteMask(REG_000390, BIT4, BIT5|BIT4);     // DDC Clock use FRO 12M.
#endif     
        msWriteByteMask(REG_000381, BIT3, BIT3);          // Fake Training Reply
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_AudioSSCInit()
//  [Description]
//                  mhal_DP_AudioSSCInit
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_DP_AudioSSCInit()
{	
    DP_HAL_DPUTSTR("\r\n==DP  Audio Init == ");
    EnableDPMute();
    DPRxAudioMuteEn = 1;	       
    DPRxUnMuteCnt =0;
    DPAudioUnmute_Case1 = 0;
    DPRxAudioSSCEnCnt=0;
    DPAudioTimeOut =0;	   
    DPRxCearCnt_On=0;
    DPRxPolling3FIFOCnt = 0;
    DisableDPSSC();
    DPRxAudioSSCEn= 0;
    DPAudio_State = 1;		
    msWriteByteMask(REG_1630B1,BIT0|BIT1,BIT0|BIT1);
    
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_AudioSSCAdjust()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
#if DP_SSC_ENABLE
void mhal_DP_AudioSSCAdjust(BYTE ucPortSelect, BYTE ucHBR2Select)
{
    UNUSED(ucPortSelect);
    UNUSED(ucHBR2Select);

#if DP_HAL_DEBUG_MESSAGE
#define  Const        1
#define  Const1      1
#else
#define  Const         1
#define  Const1       5
#endif

    switch(DPAudio_State)
    {
        case 0:

            break;
            
        case 1:

            if(msReadByte(REG_1630B1)&(BIT0|BIT1))
            {	                                       				               
                DP_HAL_DPRINTF( "FIFO== %x", msReadByte(REG_1630B1) );
                DP_HAL_DPRINTF( "DPRxAudioSSCEnCnt== %x", DPRxAudioSSCEnCnt );
                DP_HAL_DPRINTF("\r\n FrH=%x ",msReadByte(REG_162F20));
                DP_HAL_DPRINTF("\r\n FrL=%x ",msReadByte(REG_162F1F));
                DP_HAL_DPRINTF( "DPRxAudioMuteEn== %x", DPRxAudioMuteEn );

                EnableDPMute();
                DPRxAudioMuteEn = 1;		
                DPRxUnMuteCnt=0;
                DPRxCearCnt = 0;
                DPRxUnMuteCnt =0;
                DPRxAudioSSCEnCnt++;
                
                if(DPRxAudioSSCEnCnt > 7)
                {								

                    DPRxAudioSSCEnCnt=0;
                    msWriteByteMask(REG_1630B1,BIT0|BIT1,BIT0|BIT1);				
                    DPRxUnMuteCnt=0;				
                    EnableDPSSC();
                    DP_HAL_DPUTSTR("\r\n==1== Err enable Audio SSC ");
                    DPRxAudioSSCEn= 1;						
                    DPAudio_State =2;		
                    DP_HAL_DPUTSTR("\r\n DPAudio_State =2 ");
                }

                DP_HAL_DPUTSTR("\r\n==1==Audio FIFO Unde/OverRUN ");
                msWriteByteMask(REG_1630B1,BIT0|BIT1,BIT0|BIT1);
            }
            else
            {
                if(DPRxAudioMuteEn == 1)
                {	
                    DPRxUnMuteCnt++;
                }

                if(DPRxUnMuteCnt >=(125*Const))
                {
                    DPRxUnMuteCnt=0;
                    DPRxAudioSSCEnCnt=0;
                    DPRxCearCnt_On=0;
                    DPRxCearCnt=0;

                    if(DPRxAudioMuteEn)
                    { 	
                        DP_HAL_DPUTSTR("\r\n==================1=>Audio unMute ");						
                        EnableDPSSC();
                        DP_HAL_DPUTSTR("\r\n==1==Unmute enable Audio SSC ");
                        DPRxAudioSSCEn= 1;						
                        DPAudio_State =2;	
                    }					  
                }
            }
            //   DP_HAL_DPRINTF("\r\n =1= 0x26=%x ",msReadByte(REG_162F26));
            //   DP_HAL_DPRINTF("\r\n =1= 0x27=%x ",msReadByte(REG_162F27));
            //    DP_HAL_DPRINTF("\r\n =1= 0x29=%x ",msReadByte(REG_162F29));
            
            break;
            
        case 2:         
#if 0
            DP_HAL_DPRINTF("\r\n FrH=%x ",msReadByte(REG_162F20));
            DP_HAL_DPRINTF("\r\n FrL=%x ",msReadByte(REG_162F1F));
            DP_HAL_DPRINTF("\r\n 0x26=%x ",msReadByte(REG_162F26));
            DP_HAL_DPRINTF("\r\n 0x27=%x ",msReadByte(REG_162F27));
            DP_HAL_DPRINTF("\r\n 0x29=%x ",msReadByte(REG_162F29));
            DP_HAL_DPRINTF("\r\n lock=%x, ",msReadByte(REG_162F2B));
#endif

            if((msReadByte(REG_162F26)==0)&&((msReadByte((REG_162F27))&0x07) ==0))
            {
                if(!(msReadByte(REG_1630B1)&(BIT0|BIT1)))   // Unmute
                {	
                    if(DPRxAudioMuteEn ==1)
                    {
                        if(++DPRxPolling3FIFOCnt > (10*Const))
                        { 
                            DPRxPolling3FIFOCnt = 0;
                            
                            if(DPRxAudioMuteEn)
                            {   
                                #if  DP_NoInput_Audio	
                                msDrvAudioSineGen(FALSE,0x3<<8|0xe8, 0); //enable R_FR L_FR GAIN
 				#endif
                                DP_HAL_DPRINTF("\r\n=======================2=>Audio unMute ",0);
                                DisableDPMute();
                                DPRxAudioMuteEn = 0;			      	
                            }
                        }
                    }     
                }
                else
                {
                    DPRxPolling3FIFOCnt=0;                   
                    DP_HAL_DPRINTF("\r\n=2=>Audio FIFO Error ",0);
                    msWriteByteMask(REG_1630B1,BIT0|BIT1,BIT0|BIT1);
                }
            }
            else
            {
                msWriteByteMask(REG_1630B1,BIT0|BIT1,BIT0|BIT1);
            }
            
            if((DPRxAudioMuteEn ==0)&&(!((msReadByte(REG_162F26)==0)&&((msReadByte((REG_162F27))&0x07) ==0))))
            {
                DP_HAL_DPRINTF("\r\n=====>Audio unMute ---->Mute ",0);	
                EnableDPMute();
                DPRxAudioMuteEn = 1;
                DisableDPSSC();
                DPRxAudioSSCEn= 0;	
                DPAudio_State =1;
                msWriteByteMask(REG_1630B1,BIT0|BIT1,BIT0|BIT1);
            }
            
            break;

        default:

            break;
    };
}

#endif

#if DP_SSC_ENABLE
//**************************************************************************
//  [Function Name]:
//                  mhal_DP_UpdateAudioSSC()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void mhal_DP_UpdateAudioSSC(void)
{
    mhal_DP_AudioSSCInit();
}

#endif

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
    ucPortSelect = 0;

//	msWriteByteMask(REG_0011A9+DP_PHY_REG_OFFSET(ucPortSelect), BIT5, BIT5);			  // power down AUX 50-Ohm termination
	msWriteByte(REG_00117B+DP_PHY_REG_OFFSET(ucPortSelect), 0xFF);						  // Mask ALL AUX interrupt
	msWriteByte(REG_001191+DP_PHY_REG_OFFSET(ucPortSelect), 0xF0);						  // Mark _dpcd0_irq_mask
	msWriteByteMask(REG_001193+DP_PHY_REG_OFFSET(ucPortSelect), BIT6, BIT6);			  // Mask __aksv_irq_mask
	msWriteByteMask(REG_001196+DP_PHY_REG_OFFSET(ucPortSelect), BIT7|BIT4, BIT7|BIT4);	  // Mask SQ and Time Out Mask
	msWriteByteMask(REG_0011AE+DP_PHY_REG_OFFSET(ucPortSelect), 0, MASKBIT(2:0));		  // turn off PM detect
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_GetDebugRegisterValue()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
BYTE mhal_DP_GetDebugRegisterValue(BYTE ucPortSelect, BYTE ucRegSelect)
{
    BYTE ucRegisterValue = 0;

    ucPortSelect = 0;
    
    if(ucRegSelect == 1)
    {
        ucRegisterValue = msReadByte(REG_11332E);
        msWriteByte(REG_11332E, 0);
    }

    return ucRegisterValue;
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
    ucPortSelect = 0;

	//For DP wake-up, AUX fail issue, Peter.Pan suggest reset async. FIFO
	_mhal_DP_ResetAsyncFIFO();	
    // Open rterm
    _mhal_DP_RtermEnable(ucPortSelect);
    // Aux initial setting
    _mhal_DP_AuxInitialSetting(ucPortSelect);
    // Program DPCD
#if(DP_PROGRAM_DPCD0_ENABLE)
    _mhal_DP_SetProgramDPCD(ucPortSelect, DP_PROGRAM_DPCD_0, FALSE);
#endif
#if(DP_PROGRAM_DPCD1_ENABLE)
    _mhal_DP_SetProgramDPCD(ucPortSelect, DP_PROGRAM_DPCD_1, TRUE);
#endif
#if(DP_PROGRAM_DPCD2_ENABLE)
    _mhal_DP_SetProgramDPCD(ucPortSelect, DP_PROGRAM_DPCD_2, FALSE);
#endif
#if(DP_PROGRAM_DPCD3_ENABLE)
    _mhal_DP_SetProgramDPCD(ucPortSelect, DP_PROGRAM_DPCD_3, FALSE);
#endif
    // PYH initial setting
    _mhal_DP_ComboPHYSetting(ucPortSelect);

    // Regen timing initial setting
    _mhal_DP_RegenTimingInitialSetting(ucHBR2Select);

    
    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {

    }
    else // HBR2 path
    {
        _mhal_DP_HBR2InitialSetting(ucHBR2Select);
        _mhal_DP_SetHBR2Mux(ucPortSelect, ucHBR2Select);
    }

    //SpecificFiledSetting	 
	_mhal_DP_SpecificFiledSetting(ucPortSelect, ucHBR2Select);
    // Auto CR setting
	_mhal_DP_AutoCRSetting(ucPortSelect, ucHBR2Select);
    // Auto EQ setting
    _mhal_DP_AutoEQSetting(ucPortSelect, ucHBR2Select);
    // Enable interrupt mask
    _mhal_DP_SetMCCSReceiveInterrupt(ucPortSelect, TRUE);
#if(DP_PROGRAM_DPCD0_ENABLE)
    _mhal_DP_SetProgramDPCD0Interrupt(ucPortSelect, TRUE);
#endif
#if(DP_PROGRAM_DPCD1_ENABLE)
    _mhal_DP_SetProgramDPCD1Interrupt(ucPortSelect, TRUE);
#endif
#if(DP_PROGRAM_DPCD2_ENABLE)
    _mhal_DP_SetProgramDPCD2Interrupt(ucPortSelect, TRUE);
#endif
#if(DP_PROGRAM_DPCD3_ENABLE)
    _mhal_DP_SetProgramDPCD3Interrupt(ucPortSelect, TRUE);
#endif
    _mhal_DP_SetAUPLLBigChangeInterrupt(ucPortSelect, ucHBR2Select, TRUE);
    _mhal_DP_SetInterlaneSkewInterrupt(ucPortSelect, ucHBR2Select, TRUE);
    _mhal_DP_SetCDRLossLockInterrupt(ucPortSelect, ucHBR2Select, TRUE);
    _mhal_DP_SetVPLLBigChangeInterrupt(ucPortSelect, ucHBR2Select, TRUE);
    _mhal_DP_SetTrainingPattern1Interrupt(ucPortSelect, ucHBR2Select, TRUE);
    _mhal_DP_SetTrainingPattern2Interrupt(ucPortSelect, ucHBR2Select, TRUE);

#if(DP_HDCP2_FUNCTION_SUPPORT)
    _mhal_DP_HDCP2Initial(ucPortSelect);
    _mhal_DP_SetHDCP2InitialInterrupt(ucPortSelect, TRUE);
    _mhal_DP_SetHDCP2AKEInitInterrupt(ucPortSelect, TRUE);
    _mhal_DP_SetHDCP2NoStoredKmInterrupt(ucPortSelect, TRUE);
    _mhal_DP_SetHDCP2StoredKmInterrupt(ucPortSelect, TRUE);
    _mhal_DP_SetHDCP2LCInitInterrupt(ucPortSelect, TRUE);
    _mhal_DP_SetHDCP2SKESendInterrupt(ucPortSelect, TRUE);
#endif

    // Enable interrupt
    _mhal_DP_EnablePHYInterrupt(ucPortSelect, ucHBR2Select, TRUE);

    bDPForceDownSpeed[ucPortSelect] = FALSE;
	ucDPLinkRate[ucPortSelect] = 0x06;
	
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
    // Enable PLL clock
    msWrite2Byte(REG_162FF8, 0xFFFF); // REG_COMBO_PLL_TOP_7C[15:0]: 
    msWriteByte(REG_162FFA, 0xFF); // REG_COMBO_PLL_TOP_7D[7:0]: 
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

printData("mhal_DP_HPDControl==%d",bSetHPD);
printData("ucPortSelect==%d",ucPortSelect);

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
            bCableDetect = (DP_CABLE_DETECT_PORT0 ?FALSE: TRUE);
            break;

        case DP_INPUT_SELECT_PORT1:
            bCableDetect = (DP_CABLE_DETECT_PORT1 ?FALSE: TRUE);
            break;

        case DP_INPUT_SELECT_PORT2:
            bCableDetect = (DP_CABLE_DETECT_PORT2 ?FALSE: TRUE);
            break;

        case DP_INPUT_SELECT_PORT3:
            bCableDetect = (DP_CABLE_DETECT_PORT3 ?FALSE: TRUE);
            break;

        case DP_INPUT_SELECT_PORT4:
            bCableDetect = (DP_CABLE_DETECT_PORT4 ?FALSE: TRUE);
            break;

        case DP_INPUT_SELECT_PORT5:
            bCableDetect = (DP_CABLE_DETECT_PORT5 ?FALSE: TRUE);
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
//    BYTE ucSquelchMask = (BIT0 << ucPortSelect);
    BYTE ucSquelchMask = BIT6;

    ucPortSelect = 0;
	
    for(uctemp = 0; uctemp <DP_CHECK_SQUELCH_COUNTER; uctemp--)
    {
        if(!(msReadByte(REG_001923) &ucSquelchMask))
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
    BYTE ucDelay = 0;

	ucPortSelect = 0;

    for(uctemp = 0; uctemp <DP_AUX_COMMAND_MAX_LENGTH; uctemp++)
    {
		for(ucDelay = 0; ucDelay < 10; ucDelay++)
		{
			_nop_();
		}

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
    WORD usLinkRegOffset = _mhal_DP_ComboLinkRegisterOffset(ucPortSelect); // This function only use in combo case, not work
    WORD usLockStatus = 0;

	ucPortSelect = 0;
    
    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        usLinkRegOffset = 0;
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
	ucPortSelect = 0;

    switch(ucRateType)
    {
        case DP_FAST_TRAINING_RATE_LBR:
            msWrite2Byte(REG_112E04 +DP_PHY_REG_OFFSET(ucPortSelect), 0xAAAB);
            msWriteByte(REG_112E06 +DP_PHY_REG_OFFSET(ucPortSelect), 0x2A);
            msWriteByte(REG_11330E +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x06);
            break;

        case DP_FAST_TRAINING_RATE_HBR:
            msWrite2Byte(REG_112E04 +DP_PHY_REG_OFFSET(ucPortSelect), 0x3333);
            msWriteByte(REG_112E06 +DP_PHY_REG_OFFSET(ucPortSelect), 0x33);
            msWriteByte(REG_11330E +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x0A);
            break;

        case DP_FAST_TRAINING_RATE_HBR2:
            msWrite2Byte(REG_112E04 +DP_PHY_REG_OFFSET(ucPortSelect), 0xAA06);
            msWriteByte(REG_112E06 +DP_PHY_REG_OFFSET(ucPortSelect), 0x19);
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
            msWriteByteMask(REG_112ED7 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x0E, 0x0E);
            msWriteByteMask(REG_1134F7 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT1| BIT3, BIT1| BIT3);
            msWriteByteMask(REG_113409 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT3| BIT1| BIT0, BIT3| BIT1| BIT0);
        }
        else
        {
            msWriteByteMask(REG_112ED7 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x00, 0x0E);
            msWriteByteMask(REG_1134F7 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0, BIT1| BIT3);
            msWriteByteMask(REG_113409 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT0, BIT3| BIT1| BIT0);
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
        msWriteByte(REG_1135B2 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0xFF);
        msWriteByteMask(REG_1135B3 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT2|BIT1|BIT0, BIT2|BIT1|BIT0);		
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
	ucPortSelect = 0;

    if(ucHBR2Select != DP_HBR2_ENGINE_NONE) // HBR2 path
    {
        msWriteByteMask(REG_112E4A +DP_PHY_REG_OFFSET(ucPortSelect), bEnable? BIT2: 0, BIT2);
        msWriteByteMask(REG_113000 +DP_HBR2_REG_OFFSET(ucHBR2Select), bEnable? BIT0: 0, BIT0);

        if(bEnable == TRUE)
        {
#if(!DP_AUTO_EQ_NEW_MODE)
 	    msWriteByteMask(REG_113080 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0, BIT0);
#else
	    msWriteByteMask(REG_113080 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT0, BIT0);
#endif
        }
	else
	    msWriteByteMask(REG_113080 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0, BIT0);
			
    }
}

#if(DP_AUTO_EQ_ENABLE)
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
//    BYTE uctemp = 0;

	ucPortSelect = 0;

    if(ucHBR2Select != DP_HBR2_ENGINE_NONE) // HBR2 path
    {
        if(bEnable)
        {
            msWriteByte(REG_112EDC +DP_PHY_REG_OFFSET(ucPortSelect), 0x00);
/*    
#if(!DP_AUTO_EQ_NEW_MODE)
            msWriteByteMask(REG_113000 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0, MASKBIT(5:2));
#else
            msWriteByteMask(REG_113000 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0, MASKBIT(7:2));
#endif    
*/
//            msWriteByteMask(REG_113002 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT4 , BIT4);
//			msWriteByteMask(REG_113003 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0, BIT4);    
    
            msWriteByteMask(REG_112E4A +DP_PHY_REG_OFFSET(ucPortSelect), 0, BIT2);
            msWriteByteMask(REG_112E51 +DP_PHY_REG_OFFSET(ucPortSelect), BIT1, MASKBIT(1:0));
    
            msWriteByteMask(REG_113000 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0 , BIT0);
//            msWriteByteMask(REG_113000 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT1 , BIT1);
//            msWriteByteMask(REG_113000 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT0 , BIT0);
            msWriteByteMask(REG_113080 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0 , BIT0);    
    
            Delay1ms(1);
    
            msWriteByteMask(REG_112E4A +DP_PHY_REG_OFFSET(ucPortSelect), BIT2 , BIT2);

            bFTAutoEQEnable = TRUE;
        }
        else
        {
/*
            msWriteByteMask(REG_113000 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0 , BIT1);  
    
	        for(uctemp=0; uctemp<=3; uctemp++)
			{
				msWriteByteMask(REG_113000 +DP_HBR2_REG_OFFSET(ucHBR2Select), (uctemp << 2), MASKBIT(3:2));
			}
    
#if(!DP_AUTO_EQ_NEW_MODE)
            msWriteByteMask(REG_113000 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT2, MASKBIT(5:2));
#else
			msWriteByteMask(REG_113000 +DP_HBR2_REG_OFFSET(ucHBR2Select), (BIT7|BIT4), MASKBIT(7:2));
#endif
            msWriteByteMask(REG_113002 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0 , BIT4);
    
			msWriteByteMask(REG_113003 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT4, BIT4);			
*/    
            msWriteByteMask(REG_112E51 +DP_PHY_REG_OFFSET(ucPortSelect), MASKBIT(1:0), MASKBIT(1:0));
    
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
	ucPortSelect = 0;
	ucHBR2Select = 0;

    if((msReadByte(REG_112E83 +DP_PHY_REG_OFFSET(ucPortSelect)) &0x01) == 0x01)
	{
		Delay1ms(5);

		if((msReadByte(REG_112E83 +DP_PHY_REG_OFFSET(ucPortSelect)) &0x01) == 0x01)
		{
			mhal_DP_FTAutoEQEnable(ucPortSelect, ucHBR2Select, FALSE);
		}
	}
}
#endif

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
        msWriteByte(REG_1135C8 +DP_HBR2_REG_OFFSET(ucHBR2Select), msReadByte(REG_113584 +DP_HBR2_REG_OFFSET(ucHBR2Select)));// MVID_CENTER[7:0]
        msWriteByte(REG_1135C9 +DP_HBR2_REG_OFFSET(ucHBR2Select), msReadByte(REG_113585 +DP_HBR2_REG_OFFSET(ucHBR2Select)));// MVID_CENTER[15:8]
        msWriteByte(REG_1135CA +DP_HBR2_REG_OFFSET(ucHBR2Select), msReadByte(REG_113586 +DP_HBR2_REG_OFFSET(ucHBR2Select)));// MVID_CENTER[23:16]

        msWriteByte(REG_1135D8 +DP_HBR2_REG_OFFSET(ucHBR2Select), msReadByte(REG_11358C +DP_HBR2_REG_OFFSET(ucHBR2Select)));// NVID_CENTER[7:0]
        msWriteByte(REG_1135D9 +DP_HBR2_REG_OFFSET(ucHBR2Select), msReadByte(REG_11358D +DP_HBR2_REG_OFFSET(ucHBR2Select)));// NVID_CENTER[15:8]
        msWriteByte(REG_1135DA +DP_HBR2_REG_OFFSET(ucHBR2Select), msReadByte(REG_11358E +DP_HBR2_REG_OFFSET(ucHBR2Select)));// NVID_CENTER[23:16]		
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
        ulBaseMValue = ((DWORD)msRead2Byte(REG_113585 +DP_HBR2_REG_OFFSET(ucHBR2Select)) <<8) + msReadByte(REG_113584 +DP_HBR2_REG_OFFSET(ucHBR2Select));
        ulBaseNValue = ((DWORD)msRead2Byte(REG_11358D +DP_HBR2_REG_OFFSET(ucHBR2Select)) <<8) + msReadByte(REG_11358C +DP_HBR2_REG_OFFSET(ucHBR2Select));
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
        ulBaseMValue = ((DWORD)msRead2Byte(REG_113585 +DP_HBR2_REG_OFFSET(ucHBR2Select)) <<8) + msReadByte(REG_113584 +DP_HBR2_REG_OFFSET(ucHBR2Select));
        ulBaseNValue = ((DWORD)msRead2Byte(REG_11358D +DP_HBR2_REG_OFFSET(ucHBR2Select)) <<8) + msReadByte(REG_11358C +DP_HBR2_REG_OFFSET(ucHBR2Select));
    }

      if(ulBaseNValue > 0)//CCJ151102 update for get correct PixelClock by new calculate rule
    {
        usPixelClock = DPRxLongDIV(ulBaseMValue, usLSClock, ulBaseNValue);
        //usPixelClock = ((DWORD)ulBaseMValue *usLSClock *100) /ulBaseNValue;
    }

    return usPixelClock;
}
//**************************************************************************
//  [Function Name]:
//                  DPRxLongDIV()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
DWORD DPRxLongDIV(DWORD M_Value, WORD LingRate, DWORD N_Value) //CCJ151102 update for get correct PixelClock by new calculate rule
{
    struct{
        DWORD l0;                   
        DWORD h0;                
    }Div;
    
    DWORD N=0;
    WORD uwLinkRate=0,uwCount=0;
    DWORD d=0,cc=0;//,High=0,Low=0; 
    Div.l0  =0;
    Div.h0 =0;
    uwLinkRate = LingRate * 100;
    d = M_Value;
    N = N_Value;

    if(N ==0)
        return 0;       

    uwCount = (0xFFFFFFFF/(LingRate * M_Value));

    if(uwCount > 100 )
    {
        return (((DWORD) M_Value * LingRate * 100) / N_Value);
    }

    for(uwCount=0 ; uwCount < uwLinkRate ; uwCount++)
    {
        Div.l0 = Div.l0 + d;
        //Low =  st.l0;
        if(((Div.l0) < d)||( Div.l0 == 0))
        { 
            Div.h0 = Div.h0 + 1;
            //High = st.h0;
        }        
    }                  
    //High = st.h0;
    //Low =  st.l0;                                           
    while(1)
    {        
        if(Div.l0 >= N )
        {
            Div.l0 = Div.l0 - N;
            cc = cc + 1;
        }     
        else
        {
            if(Div.h0)
            {
                Div.h0 = Div.h0 -1;                  
                d = 0xFFFFFFFF-N;    
                Div.l0 = d + 1 + Div.l0;  
                cc = cc + 1 ;
            }
            else
            {
                return cc;                                                       
            }
        }       
    } 
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
        bInterlace = ((msReadByte(REG_1135F4 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &BIT2) ?TRUE: FALSE);
        
        usTimingInfo[0] = msRead2Byte(REG_113590 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x7FFF; // H total
        usTimingInfo[1] = msRead2Byte(REG_113592 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x7FFF; // V total
        usTimingInfo[2] = msRead2Byte(REG_113594 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x7FFF; // H width
        usTimingInfo[3] = msRead2Byte(REG_113596 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x7FFF; // V width
        usTimingInfo[4] = msRead2Byte(REG_113598 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x7FFF; // H start
        usTimingInfo[5] = msRead2Byte(REG_11359A +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x7FFF; // V start
        usTimingInfo[6] = msRead2Byte(REG_11359C +DP_HBR2_REG_OFFSET(ucHBR2Select)); // H PWS
        usTimingInfo[7] = msRead2Byte(REG_11359E +DP_HBR2_REG_OFFSET(ucHBR2Select)); // V PWS
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
	 if((!(msReadByte(REG_113574+DP_HBR2_REG_OFFSET(ucHBR2Select))&BIT2))&&(msRead2Byte(REG_113576+DP_HBR2_REG_OFFSET(ucHBR2Select))<400))		
	 mhal_DP_SetMSAMode(ucPortSelect,  ucHBR2Select);
	    
	
        msWrite2Byte(REG_11350A +DP_HBR2_REG_OFFSET(ucHBR2Select), msRead2Byte(REG_11350A +DP_HBR2_REG_OFFSET(ucHBR2Select))&0x7FFF);
        msWriteByteMask(REG_113508 +DP_HBR2_REG_OFFSET(ucHBR2Select), MASKBIT(1:0), MASKBIT(1:0));
        // V parameter
        msWrite2Byte(REG_11350C +DP_HBR2_REG_OFFSET(ucHBR2Select), (usTimingInfo[0] &0x7FFF)); // V sync width
        msWriteByte(REG_113540 +DP_HBR2_REG_OFFSET(ucHBR2Select), usTimingInfo[1]); // V front porch
        msWriteByte(REG_113505 +DP_HBR2_REG_OFFSET(ucHBR2Select), (usTimingInfo[2])); // V back porch
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
        usPacketStatus = msRead2Byte(REG_1132AA +DP_HBR2_REG_OFFSET(ucHBR2Select)); // REG_DP_SP_55[15:0]: Packet status
        
        msWrite2Byte(REG_1132AC +DP_HBR2_REG_OFFSET(ucHBR2Select), 0xFFFF); // REG_DP_SP_56[15:0]: Clear packet status
		msWrite2Byte(REG_1132AC +DP_HBR2_REG_OFFSET(ucHBR2Select), 0); // REG_DP_SP_56[15:0]: Clear packet status
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
        msWriteByteMask(REG_1135BC +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT6, BIT6);
        usMISCInfo[1] = msRead2Byte(REG_1135F0 +DP_HBR2_REG_OFFSET(ucHBR2Select)); // MISC1
        msWriteByteMask(REG_1135BC +DP_HBR2_REG_OFFSET(ucHBR2Select), 0, BIT6);
        usMISCInfo[0] = msRead2Byte(REG_1135F0 +DP_HBR2_REG_OFFSET(ucHBR2Select)); // MISC0
    }

    usColorSpace = (usMISCInfo[0] &MASKBIT(2:1)) >> 1;

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
        bIdlePatternFlag = ((msReadByte(REG_1135F4 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &BIT3) ?TRUE: FALSE);
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
    Bool bPowerStateFlag = 0;

	ucPortSelect = 0;

	bPowerStateFlag = ((msReadByte(REG_001172 +DP_PHY_REG_OFFSET(ucPortSelect)) &BIT0) ?TRUE: FALSE);

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
    Bool bEventFlag = 0;

	ucPortSelect = 0;

    bEventFlag = ((msReadByte(REG_001178 +DP_PHY_REG_OFFSET(ucPortSelect)) &BIT7) ?TRUE: FALSE);    

    if(bEventFlag)
    {
        _mhal_DP_MCCSReceiveProc(ucPortSelect);

        // Clear int, then trigger HW send ACK and clear DDC buffer
        msWriteByte(REG_001179 +DP_PHY_REG_OFFSET(ucPortSelect), BIT7);
        msWriteByte(REG_001179 +DP_PHY_REG_OFFSET(ucPortSelect), 0);
        
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
    Bool bEventFlag = 0;

	ucPortSelect = 0;

    bEventFlag = ((msReadByte(REG_001190 +DP_PHY_REG_OFFSET(ucPortSelect)) &BIT0) ?TRUE: FALSE); // REG_PM_AUX_48[0]: programmable DPCD0 IRQ

    if(bEventFlag)
    {
        //msWriteByteMask(REG_001192 +DP_PHY_REG_OFFSET(ucPortSelect), BIT0, BIT0); // REG_PM_AUX_49[0]: Write 1 to clear touch programmable DPCD0 address IRQ.
        //msWriteByteMask(REG_001192 +DP_PHY_REG_OFFSET(ucPortSelect), 0, BIT0); // REG_PM_AUX_49[0]: Write 1 to clear touch programmable DPCD0 address IRQ.
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
    Bool bEventFlag = 0;

	ucPortSelect = 0;

    bEventFlag = ((msReadByte(REG_001190 +DP_PHY_REG_OFFSET(ucPortSelect)) &BIT1) ?TRUE: FALSE); // REG_PM_AUX_48[1]: programmable DPCD1 IRQ
    
    if(bEventFlag)
    {
        //msWriteByteMask(REG_001192 +DP_PHY_REG_OFFSET(ucPortSelect), BIT1, BIT1); // REG_PM_AUX_49[1]: Write 1 to clear touch programmable DPCD1 address IRQ.
        //msWriteByteMask(REG_001192 +DP_PHY_REG_OFFSET(ucPortSelect), 0, BIT1); // REG_PM_AUX_49[1]: Write 1 to clear touch programmable DPCD1 address IRQ.
    }

    return bEventFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_GetProgramDPCD2Flag()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mhal_DP_GetProgramDPCD2Flag(BYTE ucPortSelect)
{
    Bool bEventFlag = 0;

	ucPortSelect = 0;

    bEventFlag = ((msReadByte(REG_001190 +DP_PHY_REG_OFFSET(ucPortSelect)) &BIT2) ?TRUE: FALSE); // REG_PM_AUX_48[2]: programmable DPCD2 IRQ
    
    if(bEventFlag)
    {
//        msWriteByteMask(REG_001192 +DP_PHY_REG_OFFSET(ucPortSelect), BIT2, BIT2); // REG_PM_AUX_49[2]: Write 1 to clear touch programmable DPCD2 address IRQ.
//        msWriteByteMask(REG_001192 +DP_PHY_REG_OFFSET(ucPortSelect), 0, BIT2); // REG_PM_AUX_49[2]: Write 1 to clear touch programmable DPCD2 address IRQ.
    }

    return bEventFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_GetProgramDPCD3Flag()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mhal_DP_GetProgramDPCD3Flag(BYTE ucPortSelect)
{
    Bool bEventFlag = 0;

	ucPortSelect = 0;

    bEventFlag = ((msReadByte(REG_001190 +DP_PHY_REG_OFFSET(ucPortSelect)) &BIT3) ?TRUE: FALSE); // REG_PM_AUX_48[3]: programmable DPCD3 IRQ
    
    if(bEventFlag)
    {
        msWriteByteMask(REG_001192 +DP_PHY_REG_OFFSET(ucPortSelect), BIT3, BIT3); // REG_PM_AUX_49[3]: Write 1 to clear touch programmable DPCD3 address IRQ.
        msWriteByteMask(REG_001192 +DP_PHY_REG_OFFSET(ucPortSelect), 0, BIT3); // REG_PM_AUX_49[3]: Write 1 to clear touch programmable DPCD3 address IRQ.
    }

    return bEventFlag;
}
#if(0)
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
    Bool bEventFlag = 0;

	ucPortSelect = 0;

    bEventFlag = ((msReadByte(REG_001178 +DP_PHY_REG_OFFSET(ucPortSelect)) &BIT4) ?TRUE: FALSE);
    
    if(bEventFlag)
    {
        msWriteByte(REG_001179 +DP_PHY_REG_OFFSET(ucPortSelect), BIT4);
        msWriteByte(REG_001179 +DP_PHY_REG_OFFSET(ucPortSelect), 0);
    }

    return bEventFlag;
}
#endif
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
    BYTE ucAuxCommand = 0;
    BYTE ucAuxLength = 0;

	ucPortSelect = 0;

    ucAuxCommand = msReadByte(REG_001138 +DP_PHY_REG_OFFSET(ucPortSelect))&0x0F; // REG_PM_AUX_1C[3:0]: DP AUX CH Rx command
    ucAuxLength = msReadByte(REG_00113A +DP_PHY_REG_OFFSET(ucPortSelect)); // REG_PM_AUX_1D[7:0]: DP AUX CH Rx length

    msWriteByteMask(REG_001130 +DP_PHY_REG_OFFSET(ucPortSelect), BIT1, BIT1);
    
    if(ucAuxCommand == 0x09)
    {
        if(ucAuxLength <= DP_AUX_COMMAND_MAX_LENGTH)
        {
            for(uctemp = 0; uctemp <ucAuxLength; uctemp++) // Can't write 16 data
            {
                _mhal_DP_PutAuxDDCData(ucPortSelect, ucDataQueue[uctemp]);
            }
        }
    }
    else if((ucAuxCommand == 0x08) || (ucAuxCommand == 0x04))
    {
        if(ucAuxLength <= DP_AUX_COMMAND_MAX_LENGTH)
        {
            for(uctemp = 0; uctemp <ucAuxLength; uctemp++)
            {
                ucDataQueue[uctemp] = _mhal_DP_GetAuxDDCData(ucPortSelect);
            }
        }

        bDPCDWrite = TRUE;
    }
    
    msWriteByte(REG_001179 +DP_PHY_REG_OFFSET(ucPortSelect), BIT4);
    msWriteByteMask(REG_001130 +DP_PHY_REG_OFFSET(ucPortSelect), 0, BIT1);

    // Clear int, then trigger HW send ACK and clear DDC buffer
    msWriteByte(REG_001192 +DP_PHY_REG_OFFSET(ucPortSelect), BIT0); // REG_PM_AUX_49[0]: Write 1 to clear touch programmable DPCD0 address IRQ.
    msWriteByte(REG_001192 +DP_PHY_REG_OFFSET(ucPortSelect), 0); // REG_PM_AUX_49[0]: Write 1 to clear touch programmable DPCD0 address IRQ.

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
    BYTE ucAuxCommand = 0;
    BYTE ucAuxLength = 0;
    BYTE ucAddress=0;
    BYTE ucNum=0;
	

    ucPortSelect = 0;

    ucAuxCommand = msReadByte(REG_001138 +DP_PHY_REG_OFFSET(ucPortSelect))&0x0F; // REG_PM_AUX_1C[3:0]: DP AUX CH Rx command
    ucAuxLength = msReadByte(REG_00113A +DP_PHY_REG_OFFSET(ucPortSelect)); // REG_PM_AUX_1D[7:0]: DP AUX CH Rx length
    ucAddress = msReadByte(REG_001134 +DP_PHY_REG_OFFSET(ucPortSelect)); 
    msWriteByteMask(REG_001130 +DP_PHY_REG_OFFSET(ucPortSelect), BIT1, BIT1);

    if(ucAuxCommand == 0x09)   //Read
    {
        if(ucAuxLength <= DP_AUX_COMMAND_MAX_LENGTH)
        {
            for(uctemp = 0; uctemp <(ucAuxLength); uctemp++) // Can't write 16 data
            {
            	  	
            	    ucNum = ucAddress + uctemp;
            	    
            	    if(  ucNum > 0x0b)
            	    {
		      _mhal_DP_PutAuxDDCData(ucPortSelect, 0);			
            	    }
            	    else
            	    {   
   			msWriteByteMask(REG_1131ED , BIT4, BIT4);    				   //MCU ACE enable
			msWriteByte(REG_1131EC, 0x00);							   //DPCD address
			msWriteByte(REG_1131EB, 0x03);							   //DPCD address
			msWriteByte(REG_1131EA, ucNum);         //DPCD address
			msWriteByteMask(REG_1131ED , BIT6, BIT6);		                 //Read Trigger
			while(msReadByte(REG_1131ED)&BIT5)                                        // Busy Check    
			{};
			msWriteByteMask(REG_1131ED , 0, BIT4);                                    // release MCU ace			
	             _mhal_DP_PutAuxDDCData(ucPortSelect, msReadByte(REG_1131EE));
            	    }
            }
        }
    }
    else if((ucAuxCommand == 0x08) || (ucAuxCommand == 0x04))   // Write
    {
        if(ucAuxLength <= DP_AUX_COMMAND_MAX_LENGTH)
        {
            for(uctemp = 0; uctemp <ucAuxLength; uctemp++)
            {
                ucDataQueue[uctemp] = _mhal_DP_GetAuxDDCData(ucPortSelect);
            }
        }

        if(ucAuxLength <= DP_AUX_COMMAND_MAX_LENGTH)
        {
            for(uctemp = 0; uctemp <ucAuxLength; uctemp++)
            {
            	   ucNum = ucAddress +uctemp;

            	    if(  ucNum <= 0x0b)
            	    {	     			  
			msWriteByteMask(REG_1131ED , BIT4, BIT4);    				   //MCU ACE enable
			msWriteByte(REG_1131EC, 0x00);							   //DPCD address
			msWriteByte(REG_1131EB, 0x03);							   //DPCD address
			msWriteByte(REG_1131EA, ucNum);         //DPCD address
			msWriteByte(REG_1131EF, ucDataQueue[uctemp]);
			msWriteByteMask(REG_1131ED , BIT7, BIT7);		                 //Write Trigger
			while(msReadByte(REG_1131ED)&BIT5)                                        // Busy Check    
			{};
			msWriteByteMask(REG_1131ED , 0, BIT4);                                    // release MCU ace			
            	    }
            }
        }

        bDPCDWrite = TRUE;
    }
    msWriteByte(REG_001179 +DP_PHY_REG_OFFSET(ucPortSelect), BIT4);
    msWriteByteMask(REG_001130 +DP_PHY_REG_OFFSET(ucPortSelect), 0, BIT1);

    // Clear int, then trigger HW send ACK and clear DDC buffer
    msWriteByte(REG_001192 +DP_PHY_REG_OFFSET(ucPortSelect), BIT1); // REG_PM_AUX_49[0]: Write 1 to clear touch programmable DPCD0 address IRQ.
    msWriteByte(REG_001192 +DP_PHY_REG_OFFSET(ucPortSelect), 0); // REG_PM_AUX_49[0]: Write 1 to clear touch programmable DPCD0 address IRQ.
    return bDPCDWrite;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_ProgramDPCD2Proc()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mhal_DP_ProgramDPCD2Proc(BYTE ucPortSelect, BYTE *ucDataQueue)
{
    Bool bDPCDWrite = FALSE;
    BYTE uctemp = 0;
    BYTE ucAuxCommand = 0;
    BYTE ucAuxLength = 0;

	ucPortSelect = 0;

    ucAuxCommand = msReadByte(REG_001138 +DP_PHY_REG_OFFSET(ucPortSelect))&0x0F; // REG_PM_AUX_1C[3:0]: DP AUX CH Rx command
    ucAuxLength = msReadByte(REG_00113A +DP_PHY_REG_OFFSET(ucPortSelect)); // REG_PM_AUX_1D[7:0]: DP AUX CH Rx length

    msWriteByteMask(REG_001130 +DP_PHY_REG_OFFSET(ucPortSelect), BIT1, BIT1);

    if(ucAuxCommand == 0x09)
    {
        if(ucAuxLength <= DP_AUX_COMMAND_MAX_LENGTH)
        {
            for(uctemp = 0; uctemp <ucAuxLength; uctemp++) // Can't write 16 data
            {
                _mhal_DP_PutAuxDDCData(ucPortSelect, ucDataQueue[uctemp]);
            }
        }
    }
    else if((ucAuxCommand == 0x08) || (ucAuxCommand == 0x04))
    {
        if(ucAuxLength <= DP_AUX_COMMAND_MAX_LENGTH)
        {
            for(uctemp = 0; uctemp <ucAuxLength; uctemp++)
            {
                ucDataQueue[uctemp] = _mhal_DP_GetAuxDDCData(ucPortSelect);
            }
        }

        bDPCDWrite = TRUE;
    }
    msWriteByte(REG_001179 +DP_PHY_REG_OFFSET(ucPortSelect), BIT4);
    msWriteByteMask(REG_001130 +DP_PHY_REG_OFFSET(ucPortSelect), 0, BIT1);

    // Clear int, then trigger HW send ACK and clear DDC buffer
    msWriteByte(REG_001192 +DP_PHY_REG_OFFSET(ucPortSelect), BIT2); // REG_PM_AUX_49[0]: Write 1 to clear touch programmable DPCD0 address IRQ.
    msWriteByte(REG_001192 +DP_PHY_REG_OFFSET(ucPortSelect), 0); // REG_PM_AUX_49[0]: Write 1 to clear touch programmable DPCD0 address IRQ.

    return bDPCDWrite;
}
//**************************************************************************
//  [Function Name]:
//                  mhal_DP_ProgramDPCD3Proc()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mhal_DP_ProgramDPCD3Proc(BYTE ucPortSelect, BYTE *ucDataQueue)
{
    Bool bDPCDWrite = FALSE;
    BYTE ucAuxCommand = 0;
    BYTE ucAuxLength = 0;
	

	ucPortSelect = 0;

    ucAuxCommand = msReadByte(REG_001138 +DP_PHY_REG_OFFSET(ucPortSelect))&0x0F; // REG_PM_AUX_1C[3:0]: DP AUX CH Rx command
    ucAuxLength = msReadByte(REG_00113A +DP_PHY_REG_OFFSET(ucPortSelect)); // REG_PM_AUX_1D[7:0]: DP AUX CH Rx length

    msWriteByteMask(REG_001130 +DP_PHY_REG_OFFSET(ucPortSelect), BIT1, BIT1);

    if(ucAuxCommand == 0x09)
    {
    	_mhal_DP_PutAuxDDCData(ucPortSelect, *ucDataQueue);
    }
    else if(ucAuxCommand == 0x08)
    {
    	*ucDataQueue = _mhal_DP_GetAuxDDCData(ucPortSelect);
        bDPCDWrite = TRUE;
    }

	msWriteByte(REG_1130E5 +DP_PHY_REG_OFFSET(ucPortSelect), (*ucDataQueue));  //reg_hdcp22_type
    msWriteByte(REG_001179 +DP_PHY_REG_OFFSET(ucPortSelect), BIT4);	
    msWriteByteMask(REG_001130 +DP_PHY_REG_OFFSET(ucPortSelect), 0, BIT1);

    // Clear int, then trigger HW send ACK and clear DDC buffer
    msWriteByte(REG_001192 +DP_PHY_REG_OFFSET(ucPortSelect), BIT3); // REG_PM_AUX_49[0]: Write 1 to clear touch programmable DPCD0 address IRQ.
    msWriteByte(REG_001192 +DP_PHY_REG_OFFSET(ucPortSelect), 0); // REG_PM_AUX_49[0]: Write 1 to clear touch programmable DPCD0 address IRQ.

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
        bEventFlag = ((msReadByte(REG_1135E0 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &BIT0) ?TRUE: FALSE); 

        if(bEventFlag)
        {
			msWriteByte(REG_1135E8 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT0);
			msWriteByte(REG_1135E8 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0);
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
            msWriteByte(REG_1133EA +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT3);
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
            msWriteByte(REG_1133E9 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT0);
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
        bEventFlag = ((msReadByte(REG_1135E0 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &BIT1) ?TRUE: FALSE); 

        if(bEventFlag)
        {
			msWriteByte(REG_1135E8 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT1);
			msWriteByte(REG_1135E8 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0);
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
/*	
#if(DP_AUTO_EQ_ENABLE) 
    BYTE uctemp = 0;
#endif
*/
#if(!DP_AUTO_CR_ENABLE)
    BYTE ucSwingLevel = 0;
#endif

    DWORD usTP1Count = 0;

    ucPortSelect = 0;

    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        ucPortSelect = 0;
    }
    else // HBR2 path
    {
        bEventFlag = ((msReadByte(REG_1133E8 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &BIT0) ?TRUE: FALSE); 

        if(bEventFlag || (msReadByte(REG_1133E4 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &BIT0))
        {
             mhal_DP_FastTrainingEnable(ucPortSelect, ucHBR2Select, FALSE);
            if(msRegs(REG_11330E +DP_HBR2_REG_OFFSET(ucHBR2Select)) == 0x14) 
            {
                _mhal_DP_SquelchLevelSetting(ucPortSelect, 0xF);
            }
            else
            {
                _mhal_DP_SquelchLevelSetting(ucPortSelect, 0x4);
            }

            msWriteByte(REG_11331E +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x00);
            msWriteByte(REG_11331F +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x00);

    	 //	msWriteByteMask(REG_1133A1 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT4, BIT4);  //HDCP reset
	 //  	msWriteByteMask(REG_1133A1 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0, BIT4);  //HDCP reset
        
#if(DP_AUTO_EQ_ENABLE)   

#if(DP_AUTO_EQ_NEW_MODE)
            msWriteByteMask(REG_113080 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT0, BIT0);
#endif
/*
#if(!DP_AUTO_EQ_NEW_MODE)
            msWriteByteMask(REG_113000 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT2, MASKBIT(5:2));
#else
            msWriteByteMask(REG_113000 +DP_HBR2_REG_OFFSET(ucHBR2Select), (BIT7|BIT4), MASKBIT(7:2));
#endif
            msWriteByteMask(REG_113000 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0, BIT1);
            msWriteByteMask(REG_113002 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0 , BIT4); 
			msWriteByteMask(REG_113003 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT4, BIT4);
*/
            msWriteByteMask(REG_112051 +DP_PHY_REG_OFFSET(ucPortSelect), MASKBIT(1:0), MASKBIT(1:0));


            mhal_DP_AutoEQEnable(ucPortSelect, ucHBR2Select, FALSE);
            mhal_DP_AutoEQEnable(ucPortSelect, ucHBR2Select, TRUE);
/*
            msWriteByteMask(REG_113310 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0, BIT0);

            for(uctemp = 0; uctemp < 10; uctemp++)
            {
                _nop_();
            }
            
            msWriteByteMask(REG_113310 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT0, BIT0);
*/
#endif

            msWriteByteMask(REG_1133E0 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0, BIT1); //enable TPS2 interrupt

            while(((msReadByte(REG_113310 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x03) == 0x01) && (usTP1Count < 200000)) // while pattern 1 is On-Going
            {
                usTP1Count++;
             
#if(!DP_AUTO_CR_ENABLE)             

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
                                        }
                                    }
                                }
                            }
                        }
                        
                        break;

                    default:
                        
                        break;
                }
     
#endif //#if(!DP_AUTO_CR_ENABLE)     

            }

              msWriteByte(REG_1133E8 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT0);
              msWriteByte(REG_1133E8 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0);
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
    DWORD usTP2Count = 0;

    ucPortSelect = 0;
    
    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        ucPortSelect = 0;
    }
    else // HBR2 path
    {
        bTP2Flag = ((msReadByte(REG_1133E8 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &BIT1) ?TRUE: FALSE) | ((msReadByte(REG_1133E4 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &BIT1) ?TRUE: FALSE); 
        bTP3Flag = ((msReadByte(REG_1133EA +DP_HBR2_REG_OFFSET(ucHBR2Select)) &BIT1) ?TRUE: FALSE) | ((msReadByte(REG_1133E6 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &BIT1) ?TRUE: FALSE);

       if(bTP2Flag || bTP3Flag)
        {

	     mhal_DP_FastTrainingEnable(ucPortSelect, ucHBR2Select, FALSE);
#if(DP_AUTO_EQ_ENABLE)

#if 1//(DP_EQ_OFFSET_FUNCTION || DP_ERROR_COUNT_FUNCTION || DP_ENABLE_STABLE_COUNT)
            BYTE AEQ_done = 0;
#endif

#if(DP_ERROR_COUNT_FUNCTION || DP_ENABLE_STABLE_COUNT)
            msWriteByteMask(REG_113484 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT0, MASKBIT(1:0));
#endif
	    msWriteByteMask(REG_113484 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT0, MASKBIT(1:0));
	    msWriteByteMask(REG_113484 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT2, MASKBIT(3:2));

            
#if(DP_EQ_OFFSET_FUNCTION)
            msWriteByte(REG_112EDC +DP_PHY_REG_OFFSET(ucPortSelect), 0x00);
#endif
            
#if (DP_EQ_OFFSET_FUNCTION || DP_ERROR_COUNT_FUNCTION || DP_ENABLE_STABLE_COUNT)            
            while((((msReadByte(REG_113310 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x03) == 0x02)||((msReadByte(REG_113310 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x03) == 0x03)) && (usTP2Count < 200000) && (AEQ_done == 0))
#else
            while((((msReadByte(REG_113310 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x03) == 0x02)||((msReadByte(REG_113310 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x03) == 0x03)) && (usTP2Count < 200000))
#endif
            {
                usTP2Count++;
    
#if(DP_DYNAMIC_PREEMPHASIS)
    
                WORD LANE0_EQ = 0, LANE1_EQ = 0, LANE2_EQ = 0, LANE3_EQ = 0;
    
                if(((msReadByte(REG_113311 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x08) == 0x08) && ((msReadByte(REG_112E83 +DP_PHY_REG_OFFSET(ucPortSelect)) &0x01) == 0x01))
                {
                    LANE0_EQ = (msReadByte(REG_112EE6 +DP_PHY_REG_OFFSET(ucPortSelect)) &0x1F);
                    LANE1_EQ = (((msReadByte(REG_112EE6 +DP_PHY_REG_OFFSET(ucPortSelect)) &0xE0) >> 5) | ((msReadByte(REG_1120E7 +DP_PHY_REG_OFFSET(ucPortSelect)) &0x03) << 3));
                    LANE2_EQ = ((msReadByte(REG_112EE7 +DP_PHY_REG_OFFSET(ucPortSelect)) &0x7C) >> 2);
                    LANE3_EQ = (msReadByte(REG_112EE8 +DP_PHY_REG_OFFSET(ucPortSelect)) &0x1F);
    
                    if((LANE0_EQ >= DP_EQ_THRESHOLD) || (LANE1_EQ >= DP_EQ_THRESHOLD) || (LANE2_EQ >= DP_EQ_THRESHOLD) || (LANE3_EQ >= DP_EQ_THRESHOLD))
                    {
                        msWriteByteMask(REG_113002 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT4, BIT4);
                    }
                }
            
#endif
            
	#if 1//(DP_EQ_OFFSET_FUNCTION || DP_ERROR_COUNT_FUNCTION || DP_ENABLE_STABLE_COUNT)
				if((msReadByte(REG_11300E +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x80) == 0x80)
                        {
                            AEQ_done = 0x01;
				//==========================================================================
				//		For CTS patch
				//==========================================================================
			// if((msReadByte(REG_11300E +DP_HBR2_REG_OFFSET(ucHBR2Select))&0x80) == 0x80)
		 	 {
		 	//	  msWriteByte(REG_11333D , msReadByte(REG_11333D)+1 );
				switch( msReadByte(REG_11330F +DP_HBR2_REG_OFFSET(ucHBR2Select))&0x07 )
			     {
				     	BYTE ucTemp=0;
			              //DWORD ucCount=0; 	

						case 0x01:
			              	{
					    	      if(msReadByte(REG_1134E3 +DP_HBR2_REG_OFFSET(ucHBR2Select))==msReadByte(REG_1133D5 +DP_HBR2_REG_OFFSET(ucHBR2Select)))			    		 		 
						      {
				                    		
				                       	msWriteByteMask(REG_113484 +DP_HBR2_REG_OFFSET(ucHBR2Select),0,BIT0|BIT1);
				                       	msWriteByteMask(REG_113484 +DP_HBR2_REG_OFFSET(ucHBR2Select),0,BIT3|BIT2);
				                            msWriteByteMask(REG_113406 +DP_HBR2_REG_OFFSET(ucHBR2Select),BIT0,BIT0);	
				                    }
				                    else
				                    {	  
				                    		 
				                    		   mhal_DP_AutoEQEnable(ucPortSelect, ucHBR2Select, FALSE);
				                    		   msWriteByteMask(REG_113000 +DP_HBR2_REG_OFFSET(ucHBR2Select),  0, BIT0);
								   ucTemp = 0x03 - (msReadByte(REG_113311+DP_HBR2_REG_OFFSET(ucHBR2Select))&0x03);
								   msWriteByte(REG_11331E +DP_HBR2_REG_OFFSET(ucHBR2Select) , msReadByte(REG_11331E +DP_HBR2_REG_OFFSET(ucHBR2Select)) | (ucTemp << 2));
								   //msWriteByte(REG_11331E +DP_HBR2_REG_OFFSET(ucHBR2Select) , msReadByte(REG_11331E +DP_HBR2_REG_OFFSET(ucHBR2Select)) | (ucTemp << 6));
								   msWriteByteMask(REG_113484 +DP_HBR2_REG_OFFSET(ucHBR2Select),0,BIT0|BIT1);
								   msWriteByteMask(REG_113484 +DP_HBR2_REG_OFFSET(ucHBR2Select),0,BIT3|BIT2);
								   msWriteByteMask(REG_113406 +DP_HBR2_REG_OFFSET(ucHBR2Select),0,BIT0);	

				                    }					
				                 break;   
						}
						case 0x02:  //2 lanes
					      {
					    		  if(msReadByte(REG_1134E3 +DP_HBR2_REG_OFFSET(ucHBR2Select))==msReadByte(REG_1133D5 +DP_HBR2_REG_OFFSET(ucHBR2Select))&&
							 	(msReadByte(REG_1134E5 +DP_HBR2_REG_OFFSET(ucHBR2Select))==msReadByte(REG_1133D5 +DP_HBR2_REG_OFFSET(ucHBR2Select))))			    		 		 

						      {
				                    		
				                       	msWriteByteMask(REG_113484 +DP_HBR2_REG_OFFSET(ucHBR2Select),0,BIT0|BIT1);
				                       	msWriteByteMask(REG_113484 +DP_HBR2_REG_OFFSET(ucHBR2Select),0,BIT3|BIT2);
				                            msWriteByteMask(REG_113406 +DP_HBR2_REG_OFFSET(ucHBR2Select),BIT0,BIT0);	
				                    }
				                    else
				                    {	  
				                    		 
				                    		   mhal_DP_AutoEQEnable(ucPortSelect, ucHBR2Select, FALSE);
				                    		   msWriteByteMask(REG_113000 +DP_HBR2_REG_OFFSET(ucHBR2Select),  0, BIT0);
								   ucTemp = 0x03 - (msReadByte(REG_113311+DP_HBR2_REG_OFFSET(ucHBR2Select))&0x03);
								   msWriteByte(REG_11331E +DP_HBR2_REG_OFFSET(ucHBR2Select) , msReadByte(REG_11331E +DP_HBR2_REG_OFFSET(ucHBR2Select)) | (ucTemp << 2));
								   msWriteByte(REG_11331E +DP_HBR2_REG_OFFSET(ucHBR2Select) , msReadByte(REG_11331E +DP_HBR2_REG_OFFSET(ucHBR2Select)) | (ucTemp << 6));
								   msWriteByteMask(REG_113484 +DP_HBR2_REG_OFFSET(ucHBR2Select),0,BIT0|BIT1);
								   msWriteByteMask(REG_113484 +DP_HBR2_REG_OFFSET(ucHBR2Select),0,BIT3|BIT2);
								   msWriteByteMask(REG_113406 +DP_HBR2_REG_OFFSET(ucHBR2Select),0,BIT0);	

				                    }					
				                 break;   
						}
			              
					case 0x04:  //4 lanes
					    	{
					    		  if(msReadByte(REG_1134E3 +DP_HBR2_REG_OFFSET(ucHBR2Select))==msReadByte(REG_1133D5 +DP_HBR2_REG_OFFSET(ucHBR2Select))&&
							 	(msReadByte(REG_1134E5 +DP_HBR2_REG_OFFSET(ucHBR2Select))==msReadByte(REG_1133D5 +DP_HBR2_REG_OFFSET(ucHBR2Select)))&&
					    		  	(msReadByte(REG_1134E7 +DP_HBR2_REG_OFFSET(ucHBR2Select))==msReadByte(REG_1133D5 +DP_HBR2_REG_OFFSET(ucHBR2Select)))&&
					    		  	(msReadByte(REG_1134E9 +DP_HBR2_REG_OFFSET(ucHBR2Select))==msReadByte(REG_1133D5 +DP_HBR2_REG_OFFSET(ucHBR2Select))))			    		 		 

						      {
				                    		msWriteByte(REG_11333E , msReadByte(REG_11333E)+1 );
				                       	msWriteByteMask(REG_113484 +DP_HBR2_REG_OFFSET(ucHBR2Select),0,BIT0|BIT1);
				                       	msWriteByteMask(REG_113484 +DP_HBR2_REG_OFFSET(ucHBR2Select),0,BIT3|BIT2);
				                            msWriteByteMask(REG_113406 +DP_HBR2_REG_OFFSET(ucHBR2Select),BIT0,BIT0);	
				                    }
				                    else
				                    {	  
				                    		   msWriteByte(REG_113339 , msReadByte(REG_113339)+1 );
				                    		   mhal_DP_AutoEQEnable(ucPortSelect, ucHBR2Select, FALSE);
				                    		   msWriteByteMask(REG_113000 +DP_HBR2_REG_OFFSET(ucHBR2Select),  0, BIT0);
								   ucTemp = 0x03 - (msReadByte(REG_113311+DP_HBR2_REG_OFFSET(ucHBR2Select))&0x03);
								   msWriteByte(REG_11331E +DP_HBR2_REG_OFFSET(ucHBR2Select) , msReadByte(REG_11331E +DP_HBR2_REG_OFFSET(ucHBR2Select)) | (ucTemp << 2));
								   msWriteByte(REG_11331E +DP_HBR2_REG_OFFSET(ucHBR2Select) , msReadByte(REG_11331E +DP_HBR2_REG_OFFSET(ucHBR2Select)) | (ucTemp << 6));
								   msWriteByte(REG_11331F +DP_HBR2_REG_OFFSET(ucHBR2Select) , msReadByte(REG_11331F +DP_HBR2_REG_OFFSET(ucHBR2Select)) | (ucTemp << 2));
								   msWriteByte(REG_11331F +DP_HBR2_REG_OFFSET(ucHBR2Select) , msReadByte(REG_11331F +DP_HBR2_REG_OFFSET(ucHBR2Select)) | (ucTemp << 6));
								   msWriteByteMask(REG_113484 +DP_HBR2_REG_OFFSET(ucHBR2Select),0,BIT0|BIT1);
								   msWriteByteMask(REG_113484 +DP_HBR2_REG_OFFSET(ucHBR2Select),0,BIT3|BIT2);
								   msWriteByteMask(REG_113406 +DP_HBR2_REG_OFFSET(ucHBR2Select),0,BIT0);	

				                    }

							// for(ucCount=0;ucCount<100000;ucCount++)
							// _nop_();
				                    
							// msWriteByteMask(REG_113484 +DP_HBR2_REG_OFFSET(ucHBR2Select),0,BIT3|BIT2);
				                    
						}
					break;
				     }	

			 }




                            
                        }
#endif
                        }
    



    
#if (DP_EQ_OFFSET_FUNCTION || DP_ERROR_COUNT_FUNCTION || DP_ENABLE_STABLE_COUNT)
    
            if(AEQ_done == 0x01)
                        {
                AEQ_done = 0;
            
#if(DP_EQ_OFFSET_FUNCTION)
    
	            if((msReadByte(REG_113310 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x03) == 0x02)
            {
	                if((msReadByte(REG_112EC0 +DP_PHY_REG_OFFSET(ucPortSelect)) & 0x1F) <= (0x1F -DP_EQ_OFFSET_VALUE))
                {
	                    msWriteByte(REG_112EC0 +DP_PHY_REG_OFFSET(ucPortSelect), (msReadByte(REG_112EC0 +DP_PHY_REG_OFFSET(ucPortSelect)) & 0x1F) +DP_EQ_OFFSET_VALUE); 
                }
                else
                {
	                    msWriteByte(REG_112EC0 +DP_PHY_REG_OFFSET(ucPortSelect), 0x1F);
                }
                    
					if((msReadByte(REG_112EC1 +DP_PHY_REG_OFFSET(ucPortSelect)) & 0x1F) <= (0x1F -DP_EQ_OFFSET_VALUE))
                {
						msWriteByte(REG_112EC1 +DP_PHY_REG_OFFSET(ucPortSelect), (msReadByte(REG_112EC1 +DP_PHY_REG_OFFSET(ucPortSelect)) & 0x1F) +DP_EQ_OFFSET_VALUE); 
                }
                else
                {
						msWriteByte(REG_112EC1 +DP_PHY_REG_OFFSET(ucPortSelect), 0x1F);
                }
                    
					if((msReadByte(REG_112EC2 +DP_PHY_REG_OFFSET(ucPortSelect)) & 0x1F) <= (0x1F -DP_EQ_OFFSET_VALUE))
                {
						msWriteByte(REG_112EC2 +DP_PHY_REG_OFFSET(ucPortSelect), (msReadByte(REG_112EC2 +DP_PHY_REG_OFFSET(ucPortSelect)) & 0x1F) +DP_EQ_OFFSET_VALUE); 
                }
                else
                {
						msWriteByte(REG_112EC2 +DP_PHY_REG_OFFSET(ucPortSelect), 0x1F);
                }
                    
					if((msReadByte(REG_112EC3 +DP_PHY_REG_OFFSET(ucPortSelect)) & 0x1F) <= (0x1F -DP_EQ_OFFSET_VALUE))
                {
						msWriteByte(REG_112EC3 +DP_PHY_REG_OFFSET(ucPortSelect), (msReadByte(REG_112EC3 +DP_PHY_REG_OFFSET(ucPortSelect)) & 0x1F) +DP_EQ_OFFSET_VALUE); 
                }
                else
                {
						msWriteByte(REG_112EC3 +DP_PHY_REG_OFFSET(ucPortSelect), 0x1F);
                }
    
    
	                msWriteByte(REG_112EDC +DP_PHY_REG_OFFSET(ucPortSelect), 0xF0);
            }
            
#endif
            
#if(DP_ERROR_COUNT_FUNCTION)
            volatile WORD delay_count = 0;
            BYTE error_flag = 0;
            WORD ErrorCnt = 0, error_L0 = 0,error_L1 = 0,error_L2 = 0,error_L3 = 0;
            
                msWriteByteMask(REG_113001 +DP_HBR2_REG_OFFSET(ucHBR2Select), MASKBIT(3:2), MASKBIT(3:2));
				msWriteByteMask(REG_113001 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0, BIT6);    
    
                while(delay_count < 2500)
                {
                   delay_count++;
                }
    
                msWriteByteMask(REG_113001 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT6, BIT6);
    
                while((error_flag == 0) && (ErrorCnt < 2000))
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
    
				msWriteByteMask(REG_113001 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0, MASKBIT(3:2));
				msWriteByteMask(REG_113001 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0, BIT6);
#endif

#if(DP_ENABLE_STABLE_COUNT)
				BYTE link_rate = 0;

				if((ucDPLinkRate[ucPortSelect] == msReadByte(REG_11330E +DP_HBR2_REG_OFFSET(ucHBR2Select))) && bDPForceDownSpeed[ucPortSelect])
                {
					link_rate = 0;
            }
				else
				{
					link_rate = 1;
            bDPForceDownSpeed[ucPortSelect] = FALSE;
				}
#endif
                
#if(DP_ERROR_COUNT_FUNCTION && DP_ENABLE_STABLE_COUNT)
				if((error_flag == 0x00) && link_rate)
#elif(DP_ENABLE_STABLE_COUNT)
				if(link_rate)
#else
				if(error_flag == 0x00)
#endif
				{
                    msWriteByteMask(REG_113484 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0, MASKBIT(1:0)); 				
				}
            
            }                            
#endif
            
            
#if(DP_DYNAMIC_PREEMPHASIS)
            msWriteByteMask(REG_113002 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0, BIT4);
            
#endif
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
                        if((msReadByte(REG_113311 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x03) == ((msReadByte(REG_11331E +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x0C)>>2)) // request reach
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
                        if((msReadByte(REG_113311 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x18) == ((msReadByte(REG_11331E +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x0C)>>2)) // request reach
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
                        if((msReadByte(REG_113311 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x18) == ((msReadByte(REG_11331E +DP_HBR2_REG_OFFSET(ucHBR2Select)) & 0x0C)>>2)) // request reach
                        {
                            if((msReadByte(REG_113312 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x18) == ((msReadByte(REG_11331E +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0xC0) >> 4)) // request reach
                            {
                                if((msReadByte(REG_113313 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x18) == ((msReadByte(REG_11331F +DP_HBR2_REG_OFFSET(ucHBR2Select)) &0x0C)>>2)) // request reach
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

            _mhal_DP_SquelchLevelSetting(ucPortSelect, 0x4);

		msWriteByte(REG_1133E8 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT1);
		msWriteByte(REG_1133E8 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0);
		msWriteByte(REG_1133EA +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT1);
		msWriteByte(REG_1133EA +DP_HBR2_REG_OFFSET(ucHBR2Select), 0);
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
        msWriteByte(REG_1133A1 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT4);	//HDCP reset
        msWriteByte(REG_1133A1 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0);  //HDCP reset.
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
            
            //break;

        case DP_INPUT_SELECT_PORT1:
            
            //break;

        case DP_INPUT_SELECT_PORT2:
            
            //break;

        case DP_INPUT_SELECT_PORT3:
            
            //break;

        case DP_INPUT_SELECT_PORT4:
        /*
                    bEventFlag = ((msReadByte(REG_1133EB) &BIT5) ?TRUE: FALSE); // REG_DP_BANKB_74[12]: DP HDCP link integrity failure interrupt
        
                    if(bEventFlag)
                    {
                        msWriteByteMask(REG_1133EB, BIT5, BIT5); // REG_DP_BANKB_74[12]: DP HDCP link integrity failure interrupt
                    }
        */            
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
#if(DP_SSC_ENABLE)
    msWriteByteMask(REG_162F11, 0, BIT7); //REG_COMBO_PLL_TOP_08[15]: DP audio DPLL test mode configuration.
#else
    msWriteByteMask(REG_162F11, BIT7, BIT7); //REG_COMBO_PLL_TOP_08[15]: DP audio DPLL test mode configuration.
#endif
    msWriteByteMask(REG_162F70, BIT7|BIT6, BIT7|BIT6);  	  //DP SSC audio recovery clock takes long time
    msWriteByteMask(REG_162F71, BIT0, BIT0);  			  //DP SSC audio recovery clock takes long time
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
void mhal_DP_AUPLLSetting(BYTE ucInputPort)
{

//#if(DP_SSC_ENABLE)
#if(1)
    msWriteByteMask( REG_162F2A, BIT4 ,BIT4 );    // For DP port 
    msWriteByteMask(REG_162F0C,0,BIT6);                            //[6]:reg_aupll_enindiv1
#endif	
	msWriteByte(REG_1134C8+DP_HBR2_REG_OFFSET(ucInputPort), msReadByte(REG_11338E+DP_HBR2_REG_OFFSET(ucInputPort) )); //MSA MAUD_CENTER[7:0]
	msWriteByte(REG_1134C9+DP_HBR2_REG_OFFSET(ucInputPort), msReadByte(REG_11338F+DP_HBR2_REG_OFFSET(ucInputPort))); //MSA MAUD_CENTER[7:0]
	msWriteByte(REG_1134CA+DP_HBR2_REG_OFFSET(ucInputPort), msReadByte(REG_113390+DP_HBR2_REG_OFFSET(ucInputPort) )); //MSA MAUD_CENTER[7:0]

	msWriteByte(REG_1134CB+DP_HBR2_REG_OFFSET(ucInputPort), msReadByte(REG_113392+DP_HBR2_REG_OFFSET(ucInputPort))); //MSA MAUD_CENTER[7:0]
	msWriteByte(REG_1134CC+DP_HBR2_REG_OFFSET(ucInputPort), msReadByte(REG_113393+DP_HBR2_REG_OFFSET(ucInputPort))); //MSA MAUD_CENTER[7:0]
	msWriteByte(REG_1134CD+DP_HBR2_REG_OFFSET(ucInputPort), msReadByte(REG_113394+DP_HBR2_REG_OFFSET(ucInputPort))); //MSA MAUD_CENTER[7:0]
    
    
	msWriteByteMask(REG_1134D5+DP_HBR2_REG_OFFSET(ucInputPort), BIT5,BIT5);    //Oven 
	msWriteByteMask(REG_1134B3+DP_HBR2_REG_OFFSET(ucInputPort), BIT2,BIT2);    //Oven 
	msWriteByteMask(REG_1134B3+DP_HBR2_REG_OFFSET(ucInputPort), 0,BIT2);	      //Oven
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
Bool mhal_DP_CheckAudioAbsent(BYTE ucPortSelect, BYTE ucHBR2Select, WORD usPacketStatus)
{
    Bool bNoAudio = FALSE;
    
    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        ucPortSelect = 0;
    }
    else // HBR2 path
    {
        if(msReadByte(REG_1135F4 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &BIT4) // REG_DP_OP_7A[4]: Read Bit4:1 NoAudioStrm
        {
            bNoAudio = TRUE;
        }
        else if((usPacketStatus &MASKBIT(6:5)) == 0) // Check audio status
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
        if(msReadByte(REG_1630B0) &(BIT0 |BIT1)) // REG_COMBO_GP_TOP_58[1]: audio sample parity error, [0]: audio sample error
        {
            _mhal_DP_UpdateAudioFrequence();
            
            msWriteByteMask(REG_1630B0, BIT0 |BIT1, BIT0 |BIT1); // REG_COMBO_GP_TOP_58[1]: audio sample parity error, [0]: audio sample error
        }
        
#if(!DP_SSC_ENABLE)
        if(msReadByte(REG_1630B1) &(BIT0 |BIT1)) // REG_COMBO_GP_TOP_58[9]: audio fifo overflow, [8]: audio fifo underflow
        {
            msWriteByteMask(REG_1630B1, BIT0 |BIT1, BIT0 |BIT1); // REG_COMBO_GP_TOP_58[9]: audio fifo overflow, [8]: audio fifo underflow
        }    
#endif    

        if(msReadByte(REG_1133E7 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &BIT0) // REG_DP_BANKB_73[8]: DP no ls clock interrupt sense
        {
            msWriteByte(REG_1133EB +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT0); // REG_DP_BANKB_75[8]: DP no ls clock irq
        }
        
        if(msReadByte(REG_1133E7 +DP_HBR2_REG_OFFSET(ucHBR2Select)) &BIT2) // REG_DP_BANKB_73[10]: DP link rate change interrupt sense
        {
            msWriteByte(REG_1133EB +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT2); // REG_DP_BANKB_75[10]: DP link rate change irq
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
        if(msReadByte(REG_11331C +DP_HBR2_REG_OFFSET(ucHBR2Select)) &BIT0) // REG_DP_BANKB_0E[0]: Interlane alignment done
        {
            bAlignment = TRUE;
        }
    }

    return bAlignment;
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
        stAudioTemp.ulMCodeValue = msReadByte(REG_113582 +DP_HBR2_REG_OFFSET(ucHBR2Select)); // REG_DP_OP_40[23:16]: recived audio M value
        stAudioTemp.ulMCodeValue = (stAudioTemp.ulMCodeValue << 16) +msRead2Byte(REG_113580 +DP_HBR2_REG_OFFSET(ucHBR2Select)); // REG_DP_OP_40[15:0]: recived audio M value
        stAudioTemp.ulNCodeValue = msReadByte(REG_11358A +DP_HBR2_REG_OFFSET(ucHBR2Select)); // REG_DP_OP_44[23:16]: recived audio N value
        stAudioTemp.ulNCodeValue = (stAudioTemp.ulNCodeValue << 16) +msRead2Byte(REG_113588 +DP_HBR2_REG_OFFSET(ucHBR2Select)); // REG_DP_OP_44[15:0]: recived audio N value
        stAudioTemp.usDivValue = msRead2Byte(REG_162F04); // REG_COMBO_PLL_TOP_02[15:0]: FeedBack path output from feedback counter second stage
        stAudioTemp.ucKpValue = _mhal_DP_GetPow2((msReadByte( REG_162F00) >> 4) &0x0F); // REG_COMBO_PLL_TOP_00[7:4]: Kp
        stAudioTemp.ucKmValue = _mhal_DP_GetPow2(msReadByte(REG_162F01) &0x0F); // REG_COMBO_PLL_TOP_01[11:8]: Km

        stAudioInfo.ulMCodeValue = msReadByte(REG_1135C2 +DP_HBR2_REG_OFFSET(ucHBR2Select)); // REG_DP_OP_60[23:16]:
        stAudioInfo.ulMCodeValue = (stAudioInfo.ulMCodeValue << 16) +msRead2Byte(REG_1135C0+DP_HBR2_REG_OFFSET(ucHBR2Select)); // REG_DP_OP_60[15:0]:
        stAudioInfo.ulNCodeValue = msReadByte(REG_1135D2 +DP_HBR2_REG_OFFSET(ucHBR2Select)); // REG_DP_OP_68[23:16]:
        stAudioInfo.ulNCodeValue = (stAudioInfo.ulNCodeValue << 16) +msRead2Byte(REG_1135D0 +DP_HBR2_REG_OFFSET(ucHBR2Select)); // REG_DP_OP_68[15:0]:
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
    else if(ABS_MINUS(stAudioInfo.usDivValue, stAudioTemp.usDivValue) > 0x80) // Compare Div
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
//                  DPRxCheckAudioFreq()
//  [Description]
//                  DPRxCheckAudioFreq
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE mhal_DP_CalAudioFreq(BYTE ucPortSelect, BYTE ucHBR2Select)  
{
     DPAudioFreqType ucFreqVaild;
     WORD wDPAudioFreq;
     WORD usLSClock = _mhal_DP_GetLinkRate(ucPortSelect, ucHBR2Select);           
     stDPAudioInfo stAudioTemp = {0, 0, 0, 0, 0};
    
    stAudioTemp.ulMCodeValue = msReadByte(REG_113582 +DP_HBR2_REG_OFFSET(ucHBR2Select)); // REG_DP_OP_40[23:16]: recived audio M value
    stAudioTemp.ulMCodeValue = (stAudioTemp.ulMCodeValue << 16) +msRead2Byte(REG_113580 +DP_HBR2_REG_OFFSET(ucHBR2Select)); // REG_DP_OP_40[15:0]: recived audio M value
    stAudioTemp.ulNCodeValue = msReadByte(REG_11358A +DP_HBR2_REG_OFFSET(ucHBR2Select)); // REG_DP_OP_44[23:16]: recived audio N value
    stAudioTemp.ulNCodeValue = (stAudioTemp.ulNCodeValue << 16) +msRead2Byte(REG_113588 +DP_HBR2_REG_OFFSET(ucHBR2Select)); // REG_DP_OP_44[15:0]: recived audio N value

    if(stAudioTemp.ulNCodeValue)    	
   	   wDPAudioFreq = ((((usLSClock*1000ul)+256ul)/512ul)*stAudioTemp.ulMCodeValue)/  stAudioTemp.ulNCodeValue;    
    else
    {   
    	   	   ucFreqVaild = DPAUD_FREQ_ERROR;
    	   	   return ucFreqVaild;
    }
    	
    //DP_printData("\r\n DP wDPAudioFreq %d", wDPAudioFreq);
    
    if( abs( wDPAudioFreq - 32 ) < 4 )
        ucFreqVaild = DPAUD_FREQ_32K;
    else if( abs( wDPAudioFreq - 44 ) < 4 )
        ucFreqVaild = DPAUD_FREQ_44K;
    else if( abs( wDPAudioFreq - 48 ) < 4 )
        ucFreqVaild = DPAUD_FREQ_48K;
    else if( abs( wDPAudioFreq - 88 ) < 4 )
        ucFreqVaild = DPAUD_FREQ_88K;
    else if( abs( wDPAudioFreq - 96 ) < 4 )
        ucFreqVaild = DPAUD_FREQ_96K;
    else if( abs( wDPAudioFreq - 176 ) < 4 )
        ucFreqVaild = DPAUD_FREQ_176K;
    else if( abs( wDPAudioFreq - 192 ) < 4 )
        ucFreqVaild = DPAUD_FREQ_192K;
    else
        ucFreqVaild = DPAUD_FREQ_ERROR;

    //DP_HAL_DPRINTF("\r\n DP ucFreqVaild %d", ucFreqVaild);

    return ucFreqVaild;
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
        msWrite2Byte(REG_1135C0 +DP_HBR2_REG_OFFSET(ucHBR2Select), ulAudioMCode &0xFFFF); // REG_DP_OP_60[15:0]:
        msWriteByte(REG_1135C2 +DP_HBR2_REG_OFFSET(ucHBR2Select), (ulAudioMCode >> 16)); // REG_DP_OP_60[23:16]:
        msWrite2Byte(REG_1135D0 +DP_HBR2_REG_OFFSET(ucHBR2Select), ulAudioNCode &0xFFFF); // REG_DP_OP_68[15:0]:
        msWriteByte(REG_1133D2 +DP_HBR2_REG_OFFSET(ucHBR2Select), (ulAudioNCode >> 16)); // REG_DP_OP_68[23:16]:
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
    BYTE ucSourceSelect = 14;
    WORD usLSClock= _mhal_DP_GetLinkRate(ucPortSelect, ucHBR2Select);
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
        
    ulAudioFreq = 64 *((float) 216.04/usLSClock) *(stAudioInfo.ucKmValue *stAudioInfo.ulNCodeValue) /(stAudioInfo.ucKpValue *stAudioInfo.ulMCodeValue) *stAudioInfo.usDivValue;

    msWrite2Byte(REG_162F08, ulAudioFreq &0xFFFF); // REG_COMBO_PLL_TOP_04[15:0]: new freqency ctrl value from MCU calculator
    msWriteByte(REG_162F0A, (ulAudioFreq >> 16)); // REG_COMBO_PLL_TOP_05[7:0]: new freqency ctrl value from MCU calculator

    if(msReadByte(REG_11330E +DP_HBR2_REG_OFFSET(ucHBR2Select))==0x14) // HBR2
    {
        ucSourceSelect = 13;
        
        msWriteByteMask(REG_162F41, BIT0, BIT0); // REG_COMBO_PLL_TOP_20[8]: Enable APLL VCO frequency x2
    }
    else // HBR or RBR
    {
        msWriteByteMask(REG_162F41, 0, BIT0); // REG_COMBO_PLL_TOP_20[8]: Enable APLL VCO frequency x2
    }

    msWriteByteMask(REG_162F40, (ucSourceSelect <<4), 0xF0); // REG_COMBO_PLL_TOP_20[7:4]: APLL clock source select

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
    
    ucPortSelect = 0;	
    
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
            msWriteByteMask(REG_113484 +DP_HBR2_REG_OFFSET(ucHBR2Select), BIT0, MASKBIT(1:0));
        }

        bDPForceDownSpeed[ucPortSelect] = TRUE;
        ucDPLinkRate[ucPortSelect] = msReadByte(REG_11330E +DP_HBR2_REG_OFFSET(ucHBR2Select));

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

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_VersionSetting()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_DP_VersionSetting(BYTE ucPortSelect, BYTE ucHBR2Select, BYTE ucVersion )
{

    ucPortSelect = 0;

    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        ucPortSelect = 0;
    }
    else // HBR2 path
    {
		if(ucVersion == E_DP_V11)
		{
			msWriteByte(REG_113300 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x11 );	 // DPCD 1.1 Version
			msWriteByte(REG_113301 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x0A );    // DP main link rate 2.7G
		}
		else
		{
			msWriteByte(REG_113300 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x12 );    // DPCD 1.2 Version
			msWriteByte(REG_113301 +DP_HBR2_REG_OFFSET(ucHBR2Select), 0x14 );	 // DP main link rate 5.4G
		}
		
		if(ucVersion == E_DP_V11)
		{
			msWriteByte(REG_001100 +DP_PHY_REG_OFFSET(ucPortSelect), 0x11 );    // DPCD 1.1 Version
			msWriteByte(REG_001101 +DP_PHY_REG_OFFSET(ucPortSelect), 0x0A );	// DP main link rate 2.7G												   // Support 1.62G 													 // Support 1.62G
			msWriteByteMask(REG_001103 +DP_PHY_REG_OFFSET(ucPortSelect), 0x89, BIT7|BIT5|BIT4|BIT3|BIT2|BIT1|BIT0);	    // non-Support TP3
		}
		else
		{
			msWriteByte(REG_001100 +DP_PHY_REG_OFFSET(ucPortSelect), 0x12 );    // DPCD 1.2 Version
			msWriteByte(REG_001101 +DP_PHY_REG_OFFSET(ucPortSelect), 0x14 );	// DP main link rate 5.4G													 // Support 1.62G
			msWriteByteMask(REG_001103 +DP_PHY_REG_OFFSET(ucPortSelect), 0x8B, BIT7|BIT5|BIT4|BIT3|BIT2|BIT1|BIT0);	    // Support TP3
		}

    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_GetDRRFlag()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mhal_DP_GetDRRFlag(BYTE ucPortSelect, BYTE ucHBR2Select)
{

    Bool bDRRFlag = FALSE;
    ucPortSelect = 0;

    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        ucPortSelect = 0;
    }
    else // HBR2 path
    {
        if((msReadByte(REG_113147 +DP_HBR2_REG_OFFSET(ucHBR2Select)) & BIT7) == BIT7)
        {
	        bDRRFlag = TRUE;
        }
    }

    return bDRRFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_EnableDRRFunction()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_DP_EnableDRRFunction(BYTE ucPortSelect, BYTE ucHBR2Select, Bool bEnable)
{
    ucPortSelect = 0;

    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        ucPortSelect = 0;
    }
    else // HBR2 path
    {
        if(bEnable == TRUE)
            msWriteByteMask(REG_001103 +DP_PHY_REG_OFFSET(ucPortSelect), BIT6, BIT6);
		else
            msWriteByteMask(REG_001103 +DP_PHY_REG_OFFSET(ucPortSelect), 0, BIT6);
    }
}
//**************************************************************************
//  [Function Name]:
//                  mhal_DP_GetMSAChgFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mhal_DP_GetMSAChgFlag(BYTE ucPortSelect,BYTE ucHBR2Select)
{
	 ucPortSelect = 0;
	 
       return ((msReadByte(REG_1135E2 +DP_PHY_REG_OFFSET(ucHBR2Select))&BIT4) ? TRUE : FALSE);
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_ClrMSAChgFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_DP_ClrMSAChgFlag(BYTE ucPortSelect,BYTE ucHBR2Select)
{

     ucPortSelect = 0;
   
     msWriteByteMask(REG_1135E8 +DP_PHY_REG_OFFSET(ucHBR2Select), BIT4, BIT4);
     msWriteByteMask(REG_1135E8 +DP_PHY_REG_OFFSET(ucHBR2Select), 0, BIT4);
}


//**************************************************************************
//  [Function Name]:
//                  mhal_DP_SetLearnMode()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_DP_SetLearnMode(BYTE ucPortSelect, BYTE ucHBR2Select) 
{

     ucPortSelect = 0;   
     msWriteByteMask(REG_113570 +DP_PHY_REG_OFFSET(ucHBR2Select), BIT0, BIT0|BIT1);
 
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_HVFSMCheck()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_DP_HVFSMCheck(BYTE ucPortSelect, BYTE ucHBR2Select) 
{
	 BYTE ubHs_gen_fsm_status;
	 BYTE ubCheck_fsm_Times;

     ucPortSelect = 0;   
	 ubHs_gen_fsm_status = 0x0;
	 ubCheck_fsm_Times = 0x2;  //RD suggest we should check FSM 2 times
	
	 //make sure hs_gen_fsm_status[3:0] only has 1 bit as 1, otherwise reset reg_hs_gen_fsm_rst
	 while(ubCheck_fsm_Times > 0)
	 {	 	
		 ubHs_gen_fsm_status = (msReadByte(REG_113524 +DP_PHY_REG_OFFSET(ucHBR2Select)) & 0x0F);

		 if(((ubHs_gen_fsm_status & (ubHs_gen_fsm_status - 1)) != 0x0) || (ubHs_gen_fsm_status == 0x0))
		 {
			 msWriteByteMask(REG_113521 +DP_PHY_REG_OFFSET(ucHBR2Select), BIT0, BIT0); //reg_hs_gen_fsm_rst
			 Delay2us();
			 msWriteByteMask(REG_113521 +DP_PHY_REG_OFFSET(ucHBR2Select), 0, BIT0);			 
		 }
		 
		 ubCheck_fsm_Times --;
	 }
 
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_SetMSAMode()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_DP_SetMSAMode(BYTE ucPortSelect, BYTE ucHBR2Select) 
{
     ucPortSelect = 0;   
      msWriteByteMask(REG_113570 +DP_PHY_REG_OFFSET(ucHBR2Select), 0, BIT0|BIT1);
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_GetInterlaceFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mhal_DP_GetInterlaceFlag(BYTE ucPortSelect, BYTE ucHBR2Select)
{
    Bool bInterlaceFlag = 0;

    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        ucPortSelect = 0;
    }
    else // HBR2 path
    {   
		bInterlaceFlag = ((msReadByte(REG_1135F4 +DP_PHY_REG_OFFSET(ucHBR2Select)) & BIT2) ? TRUE : FALSE);
    }

	return bInterlaceFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_SetInterlaceInverse()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_DP_SetInterlaceInverse(BYTE ucPortSelect, BYTE ucHBR2Select, Bool bEnable)
{
    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        ucPortSelect = 0;
    }
    else // HBR2 path
    {   
        msWriteByteMask(REG_113541 +DP_HBR2_REG_OFFSET(ucHBR2Select), bEnable? BIT2 : 0, BIT2);
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_GetFieldFlag()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mhal_DP_GetFieldFlag(BYTE ucPortSelect, BYTE ucHBR2Select)
{
    Bool bFieldFlag = 0;

    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        ucPortSelect = 0;
    }
    else // HBR2 path
    {   
		bFieldFlag = ((msReadByte(REG_1135F4 +DP_PHY_REG_OFFSET(ucHBR2Select)) & BIT1) ? TRUE : FALSE);
    }

	return bFieldFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_GetFieldCount()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
DWORD mhal_DP_GetFieldCount(BYTE ucPortSelect, BYTE ucHBR2Select, Bool bField)
{
    DWORD ulFieldCount = 0 ;

    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        ucPortSelect = 0;
    }
    else // HBR2 path
    {
        if(bField == 0)
        {
		    ulFieldCount = (msRead2Byte(REG_113534 +DP_HBR2_REG_OFFSET(ucHBR2Select)) + ((msReadByte(REG_113536 +DP_HBR2_REG_OFFSET(ucHBR2Select))&0x0F) << 16));
        }

        if(bField == 1)
        {
		    ulFieldCount = (msRead2Byte(REG_113538 +DP_HBR2_REG_OFFSET(ucHBR2Select)) + ((msReadByte(REG_11353A +DP_HBR2_REG_OFFSET(ucHBR2Select))&0x0F) << 16));    
        }
    }

	return ulFieldCount;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_RegenTimingProtect()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_DP_RegenTimingProtect(BYTE ucPortSelect, BYTE ucHBR2Select)
{
    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        ucPortSelect = 0;
    }
    else // HBR2 path
    {
		if((msRead2Byte(REG_113542 + DP_HBR2_REG_OFFSET(ucHBR2Select)) & 0x1FFF) > 0x200) 
		{
			msWriteByteMask(REG_113508 + DP_HBR2_REG_OFFSET(ucHBR2Select), MASKBIT(1:0), MASKBIT(1:0));
			// V parameter
			msWrite2Byte(REG_11350C + DP_HBR2_REG_OFFSET(ucHBR2Select), 0x0F); // V width
			msWriteByte(REG_113540 + DP_HBR2_REG_OFFSET(ucHBR2Select), 0x8); // V front porch
			msWriteByte(REG_113505 + DP_HBR2_REG_OFFSET(ucHBR2Select), 0x10); // V back porch
			msWriteByteMask(REG_113501 + DP_HBR2_REG_OFFSET(ucHBR2Select), BIT4, BIT4);
			// H parameter
			msWrite2Byte(REG_11350A + DP_HBR2_REG_OFFSET(ucHBR2Select), 0x34); // H width

			msWriteByteMask(REG_113502 + DP_HBR2_REG_OFFSET(ucHBR2Select), BIT6, BIT6);
			ForceDelay1ms(1);
			msWriteByteMask(REG_113502 + DP_HBR2_REG_OFFSET(ucHBR2Select), 0x0, BIT6);
		}
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_PowerOnOff()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_DP_PowerOnOff(BYTE ucPortSelect, BYTE ucHBR2Select, Bool Enable)
{
    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        ucPortSelect = 0;
    }
    else // HBR2 path
    {
		
	  if(Enable)
         {		
         		//printf(" DP Power On");
			msWriteByteMask(REG_001900, 0, BIT0|BIT1|BIT2|BIT3);							//Nodie PD_RT
			msWriteByteMask(REG_001901, 0, BIT4); 										// NODIE_PD_CLKIN
		   	msWriteByteMask(REG_001921, 0, BIT7|BIT6); 									// NODIE_EN_SQH
		   	msWriteByteMask(REG_112ED7, 0, BIT0); 										// REG_PD_BG
		   	msWriteByteMask(REG_112EB6 , 0, BIT0|BIT1|BIT2|BIT3); 						 // REG_PD_LANE
		   	msWriteByteMask(REG_162F07, 0, BIT0); 										 //AUPLL_PD
		   	msWriteByteMask(REG_162F83, 0, BIT0); 										 // VDPLL_PD
		    	msWriteByteMask(REG_112EB2 , 0, BIT1);  										 //reg_en_pd_phdac_ove
		   	msWriteByteMask(REG_112ED7, 0, BIT7|BIT6|BIT5|BIT4);   						 //reg_atop_pd_phdac_ov
			//msWriteByte(REG_0011C2,0x00); // Disable offline
	  }
	  else
	  {		
	  		//printf(" DP Power Off");
	  		//msWriteByte(REG_0011C2,0x04); // enable offline
			msWriteByteMask(REG_001900, BIT0|BIT1|BIT2|BIT3, BIT0|BIT1|BIT2|BIT3);
			msWriteByteMask(REG_001901, BIT4, BIT4); 		
			msWriteByteMask(REG_001921, BIT7|BIT6, BIT7|BIT6); 
			msWriteByteMask(REG_112ED7 , BIT0, BIT0); 
			msWriteByteMask(REG_112EB6 , BIT0|BIT1|BIT2|BIT3, BIT0|BIT1|BIT2|BIT3);  
			msWriteByteMask(REG_162F07, BIT0, BIT0); 									
			msWriteByteMask(REG_162F83, BIT0, BIT0); 	
			msWriteByteMask(REG_112EB2 , BIT1, BIT1);  									//reg_en_pd_phdac_ove
		   	msWriteByteMask(REG_112ED7 , BIT7|BIT6|BIT5|BIT4, BIT7|BIT6|BIT5|BIT4);   		//reg_atop_pd_phdac_ov
	
	  }

		
    }
}
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MSCombo_DP_AUDIO_FORMAT mhal_DP_IsAudioFmtPCM(BYTE ucInputPort,BYTE ucHBR2Select)
{
	BYTE temp;
	MSCombo_DP_AUDIO_FORMAT states = DP_AUDIO_FMT_PCM;
	
    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        ucInputPort = 0;
        
    }
    else // HBR2 path
    {		
		temp = msReadByte(REG_163088)&0x0F;        
		msWriteByteMask(REG_163088, 0x0C, 0x0F); // Switch to DP
		if(msReadByte(REG_163094)&BIT1)
		states =  DP_AUDIO_FMT_NON_PCM;
		msWriteByteMask(REG_163088, temp, 0x0F); 
    }
    return states;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_AudioInfo()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
 void mhal_DP_AudioInfo(BYTE ucInputPort, BYTE ucHBR2Select, BYTE *pPacketData)
 {
		   
	if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
	{
	 	   ucInputPort = 0;
	    
	}
	else // HBR2 path
	{		
		if(msReadByte(REG_1132F1 + DP_HBR2_REG_OFFSET(ucHBR2Select))>= 2 )
		pPacketData[0] = 8;
		else
		pPacketData[0] = 2;	
	}	
 }

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_PowerOnOff()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BOOL mhal_DPRXCheckAUXValid(BYTE ucPortSelect, BYTE ucHBR2Select)
{
	BYTE xdata i=0;
	BOOL xdata btemp = FALSE;
	
    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        ucPortSelect = 0;
    }
    else // HBR2 path
    {			
		for(i=0;i<=10;i++)
		{
			if(!((msRegs(REG_0011AC))&BIT4))
			{
				btemp = TRUE;
				break;
			}	
			Delay1ms(5);
		}	
    }
    
	return btemp;
}





#if  DP_NoInput_Audio
//**************************************************************************
//  [Function Name]:
//                  mhal_DP_GetAudioSrc(void)
//  [Description]
//                  mhal_DP_GetAudioSrc
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE mhal_DP_GetAudioSrc(BYTE ucPortSelect, BYTE ucHBR2Select)
{
	
    BYTE AudioSrc=0;
    
    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        ucPortSelect = 0;
    }
    else // HBR2 path
    {        
       AudioSrc =msReadByte(REG_163088 +DP_HBR2_REG_OFFSET(ucHBR2Select))&0x0F;       	
    }

    return AudioSrc;


}
//**************************************************************************
//  [Function Name]:
//                  mhal_DP_FreeRunSetting(void)
//  [Description]
//                  mhal_DP_FreeRunSetting
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void  mhal_DP_FreeRunSetting(BYTE ucInputPort, BYTE ucHBR2Select)
{
	 BYTE ucSourceSelect=14;	 
	 DWORD  DPAUM0 = 0;
	 DWORD  Freq_Ctrl= 0;

	if(msReadByte(REG_11330E +DP_HBR2_REG_OFFSET(ucHBR2Select))==0x14) // HBR2
	{
		ucSourceSelect = 13;
 	       DPAUM0 = 0x2983>>1;
		msWriteByteMask(REG_162F41,BIT0, BIT0); // REG_COMBO_PLL_TOP_20[8]: Enable APLL VCO frequency x2
	}
	else  // HBR or RBR
{
		if(msReadByte(REG_11330E +DP_HBR2_REG_OFFSET(ucHBR2Select))==0x0A)
		{
			DPAUM0 = 0x2983;
			Freq_Ctrl = 0x199999;
		}
		else
		{
			DPAUM0 = 0x2983<<1;
			Freq_Ctrl = 0x155555;		
		}
		msWriteByteMask(REG_162F41, 0, BIT0); // REG_COMBO_PLL_TOP_20[8]: Enable APLL VCO frequency x2
	}

        msWriteByteMask(REG_162F40, (ucSourceSelect <<4), 0xF0); // REG_COMBO_PLL_TOP_20[7:4]: APLL clock source select

	msWriteByte(REG_162F08,  ( Freq_Ctrl & 0x000000FF )); //MSA MAUD_CENTER[7:0]
	msWriteByte(REG_162F09,  ( Freq_Ctrl & 0x0000FF00 ) >> 8); //MSA MAUD_CENTER[7:0]
	msWriteByte(REG_162F0A,  ( Freq_Ctrl & 0x00FF0000 ) >> 16); //MSA MAUD_CENTER[7:0]
	
	msWriteByte(REG_1134C8+DP_HBR2_REG_OFFSET(ucInputPort), ( DPAUM0 & 0x000000FF )); //MSA MAUD_CENTER[7:0]
	msWriteByte(REG_1134C9+DP_HBR2_REG_OFFSET(ucInputPort), ( DPAUM0 & 0x0000FF00 ) >> 8); //MSA MAUD_CENTER[7:0]
	msWriteByte(REG_1134CA+DP_HBR2_REG_OFFSET(ucInputPort), ( DPAUM0 & 0x00FF0000 ) >> 16); //MSA MAUD_CENTER[7:0]

	msWriteByte(REG_1134CB+DP_HBR2_REG_OFFSET(ucInputPort), 0x00); //MSA MAUD_CENTER[7:0]
	msWriteByte(REG_1134CC+DP_HBR2_REG_OFFSET(ucInputPort), 0x80); //MSA MAUD_CENTER[7:0]
	msWriteByte(REG_1134CD+DP_HBR2_REG_OFFSET(ucInputPort), 0x00); //MSA MAUD_CENTER[7:0]
    
    
	msWriteByteMask(REG_1134D5+DP_HBR2_REG_OFFSET(ucInputPort), BIT5,BIT5);    //Oven 
	msWriteByteMask(REG_1134B3+DP_HBR2_REG_OFFSET(ucInputPort), BIT2,BIT2);    //Oven 
	msWriteByteMask(REG_1134B3+DP_HBR2_REG_OFFSET(ucInputPort), 0,BIT2);	      //Oven

	msWriteByteMask( REG_162F11, 0,BIT7 );             // Turn off Bypasse mode
	msWriteByteMask( REG_162F07, BIT5,BIT5 );           //update Freq
	msWriteByteMask( REG_162F07, 0,BIT5);                 //update Freq


}
#endif
//**************************************************************************
//  [Function Name]:
//                  DPRx_Dpcd1ByteOverwrite(void)
//  [Description]
//                  DPRx_Dpcd1ByteOverwrite
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
/*
void DPRx_Dpcd1ByteOverwrite(DWORD  usDPCDADDR, BYTE ubData)
{
	msWriteByteMask(REG_0035ED, 0x10, BIT4);  //reg_neo_mcu_acs_en
	msWrite2Byte(REG_0035EA, (usDPCDADDR&0xFFFF));
	msWriteByteMask(REG_0035EC, ((usDPCDADDR&0xF0000) >> 16), 0x0F);
	msWriteByte(REG_0035EF, ubData);
	msWriteByteMask(REG_0035ED, BIT7, BIT7); //reg_neo_mcu_acs_wrt_trig
	while((msReadByte(REG_0035ED)&BIT5) != 0x0);
	msWriteByteMask(REG_0035ED, 0x0, BIT4);  //reg_neo_mcu_acs_en
}
*/

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
            
            //break;

        case DP_INPUT_SELECT_PORT1:
            
            //break;

        case DP_INPUT_SELECT_PORT2:
            
            //break;

        case DP_INPUT_SELECT_PORT3:
            
            //break;

        case DP_INPUT_SELECT_PORT4:
            bReceived = (((MEM_MSREAD_BYTE(REG_113AE0) &BIT1) ?TRUE: FALSE) & ((MEM_MSREAD_BYTE(REG_1133C0) &BIT0) ?TRUE: FALSE)); // REG_DPCD_HDCP22_4_70[1], reg_ake_init_irq

            if(bReceived)
            {
				msWriteByteMask(REG_1133C4, BIT0, BIT0); // [0]: clear (HDCP22) INT
				msWriteByteMask(REG_1133C4, 0, BIT0);    // [0]: enable (HDCP22) INT
                msWriteByteMask(REG_113AE6, BIT1, BIT1); // REG_DPCD_HDCP22_4_73[1]: reg_ake_init_irq_clr
                msWriteByteMask(REG_113AE6, 0, BIT1); // REG_DPCD_HDCP22_4_73[1]: reg_ake_init_irq_clr
            }
            
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
            
            //break;

        case DP_INPUT_SELECT_PORT1:
            
            //break;

        case DP_INPUT_SELECT_PORT2:
            
            //break;

        case DP_INPUT_SELECT_PORT3:
            
            //break;

        case DP_INPUT_SELECT_PORT4:
            bReceived = (((MEM_MSREAD_BYTE(REG_113AE0) &BIT2) ?TRUE: FALSE) & ((MEM_MSREAD_BYTE(REG_1133C0) &BIT0) ?TRUE: FALSE)); // REG_DPCD_HDCP22_4_70[2], reg_ake_no_stored_km_irq

            if(bReceived)
            {
				msWriteByteMask(REG_1133C4, BIT0, BIT0); // [0]: clear (HDCP22) INT
				msWriteByteMask(REG_1133C4, 0, BIT0);    // [0]: enable (HDCP22) INT
                msWriteByteMask(REG_113AE6, BIT2, BIT2); // REG_DPCD_HDCP22_4_73[2]: reg_ake_no_stored_km_irq_clr
                msWriteByteMask(REG_113AE6, 0, BIT2); // REG_DPCD_HDCP22_4_73[2]: reg_ake_no_stored_km_irq_clr
            }
            
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
            
            //break;

        case DP_INPUT_SELECT_PORT1:
            
            //break;

        case DP_INPUT_SELECT_PORT2:
            
            //break;

        case DP_INPUT_SELECT_PORT3:
            
            //break;

        case DP_INPUT_SELECT_PORT4:
            bReceived = (((MEM_MSREAD_BYTE(REG_113AE0) &BIT3) ?TRUE: FALSE) & ((MEM_MSREAD_BYTE(REG_1133C0) &BIT0) ?TRUE: FALSE)); // REG_DPCD_HDCP22_4_70[3], reg_ake_stored_km_irq

            if(bReceived)
            {
				msWriteByteMask(REG_1133C4, BIT0, BIT0); // [0]: clear (HDCP22) INT
				msWriteByteMask(REG_1133C4, 0, BIT0);    // [0]: enable (HDCP22) INT
                msWriteByteMask(REG_113AE6, BIT3, BIT3); // REG_DPCD_HDCP22_4_73[3]: reg_ake_stored_km_irq_clr
                msWriteByteMask(REG_113AE6, 0, BIT3); // REG_DPCD_HDCP22_4_73[3]: reg_ake_stored_km_irq_clr
            }
            
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
            
            //break;

        case DP_INPUT_SELECT_PORT1:
            
            //break;

        case DP_INPUT_SELECT_PORT2:
            
            //break;

        case DP_INPUT_SELECT_PORT3:
            
            //break;

        case DP_INPUT_SELECT_PORT4:
            bReceived = (((MEM_MSREAD_BYTE(REG_113AE0) &BIT4) ?TRUE: FALSE) & ((MEM_MSREAD_BYTE(REG_1133C0) &BIT0) ?TRUE: FALSE)); // REG_DPCD_HDCP22_4_70[4], reg_locality_check_irq

            if(bReceived)
            {
				msWriteByteMask(REG_1133C4, BIT0, BIT0); // [0]: clear (HDCP22) INT
				msWriteByteMask(REG_1133C4, 0, BIT0);    // [0]: enable (HDCP22) INT
                msWriteByteMask(REG_113AE6, BIT4, BIT4); // REG_DPCD_HDCP22_4_73[4]: reg_locality_check_irq_clr
                msWriteByteMask(REG_113AE6, 0, BIT4); // REG_DPCD_HDCP22_4_73[4]: reg_locality_check_irq_clr
            }
            
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
            
            //break;

        case DP_INPUT_SELECT_PORT1:
            
            //break;

        case DP_INPUT_SELECT_PORT2:
            
            //break;

        case DP_INPUT_SELECT_PORT3:
            
            //break;

        case DP_INPUT_SELECT_PORT4:
            bReceived = (((MEM_MSREAD_BYTE(REG_113AE0) &BIT5) ?TRUE: FALSE) & ((MEM_MSREAD_BYTE(REG_1133C0) &BIT0) ?TRUE: FALSE)); // REG_DPCD_HDCP22_4_70[5], reg_ske_irq

            if(bReceived)
            {
				msWriteByteMask(REG_1133C4, BIT0, BIT0); // [0]: clear (HDCP22) INT
				msWriteByteMask(REG_1133C4, 0, BIT0);    // [0]: enable (HDCP22) INT
                msWriteByteMask(REG_113AE6, BIT5, BIT5); // REG_DPCD_HDCP22_4_73[5]: reg_ske_irq_clr
                msWriteByteMask(REG_113AE6, 0, BIT5); // REG_DPCD_HDCP22_4_73[5]: reg_ske_irq_clr
            }
            
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
            
            //break;

        case DP_INPUT_SELECT_PORT1:
            
            //break;

        case DP_INPUT_SELECT_PORT2:
            
            //break;

        case DP_INPUT_SELECT_PORT3:
            
            //break;

        case DP_INPUT_SELECT_PORT4:
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
                    
                    bReceived = TRUE;
                    
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
            
            //break;

        case DP_INPUT_SELECT_PORT1:
            
            //break;

        case DP_INPUT_SELECT_PORT2:
            
            //break;

        case DP_INPUT_SELECT_PORT3:
            
            //break;

        case DP_INPUT_SELECT_PORT4:
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

            //break;

        case DP_INPUT_SELECT_PORT1:

            //break;

        case DP_INPUT_SELECT_PORT2:

            //break;

        case DP_INPUT_SELECT_PORT3:

            //break;

        case DP_INPUT_SELECT_PORT4:
            msWriteByteMask(REG_113A9E, bEnable? ucMaskIndex: 0, ucMaskIndex); // REG_DPCD_HDCP22_4_4F[7:0]: reg_rx_status
            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_HDCP2SetCpIrq()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void mhal_DP_HDCP2SetCpIrq(BYTE ucPortSelect)
{
    switch(ucPortSelect)
    {
        case DP_INPUT_SELECT_PORT0:
            
            //break;

        case DP_INPUT_SELECT_PORT1:
            
            //break;

        case DP_INPUT_SELECT_PORT2:
            
            //break;

        case DP_INPUT_SELECT_PORT3:
            
            //break;

        case DP_INPUT_SELECT_PORT4:
			msWriteByteMask(REG_113319, BIT2, BIT2);
			msWriteByteMask(REG_1131B6, BIT4, BIT4);
            break;
            
        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_HDCP2GetHPulum()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_DP_HDCP2GetHPulum(BYTE ucPortSelect, BYTE *ucDataQueue)
{
    BYTE uctemp = 0;
    BYTE ucAuxLength = 16;

	ucPortSelect = 0;

    if(ucAuxLength <= DP_AUX_COMMAND_MAX_LENGTH)
    {
        for(uctemp = 0; uctemp < ucAuxLength; uctemp++)
        {
            ucDataQueue[uctemp] = msReadByte(REG_1138D2 +uctemp);            
        }
    }

	msWriteByteMask(REG_113ADF, BIT7, BIT7); // H' flag
}

#endif
#endif

#endif // _MHAL_DPRX_C_


//******************************************************************************
//  Copyright (c) 2003-2008 MStar Semiconductor, Inc.
//  All rights reserved.
//
//  [Module Name]:
//      DPRxApp.c
//  [Abstract]:
//      This module contains code for DisplayPort receiver's application level
//      procedure and subroutin
//  [Author(s)]:
//      Vincent Kuo
//  [Reversion History]:
//      Initial release:    06 May, 2008
//*******************************************************************************

#define _drvDPRXAPP_7_C_
#include "Board.h"
#if ENABLE_DP_INPUT


#include <string.h>
#include <math.h>
#include "datatype.h"
#include "Board.h"
#include "ms_reg.h"
#include "Mode.h"
#include "UserPrefDef.h"
#include "UserPref.h"
#include "Global.h"
#include "Panel.h"
#include "Ms_rwreg.h"
#include "MDebug.h"
#include "Detect.h"
#include "mStar.h"
#include "misc.h"
#include "drvDPRxApp.h"
#include "drvDPRxComboApp.h"
#include <msScaler.h>
#include "DDC2Bi.H"
#include "msACE.h"
#include "NVRam.h"
#include "drvMcu.h"
#include "Common.h"
#include "mapi_combo.h"
////////////////////////////////////
//#include "drvMcu.h"
///////////////////////////////////
#include "msflash.h"
#include "GPIO_DEF.h"
#include "Adjust.h"
#include "Isr.h"
#if ENABLE_DAC
#include "drvAudio.h"
#endif

#if ENABLE_DAISY_CHAIN
#include "msDaisyChain.h"
#endif

//-------------------------------------------------------------------------------------------------
//  Local Structures & define
//-------------------------------------------------------------------------------------------------
#include "msComboRx.h"

#define DP_DEBUG      0

#define  DPCTS_Patch           1
#define HBRPort(a)      ((((HBR2_IP_SELECT)a) == (DP_HBR2_1)) ? (1) : (0))

DP_INFO _DPDATATYPE_ gDPInfo[DP_NUM];
DP_ComboInfo  gDPCombo[DP_COMBO];

#if (DP_DEBUG&&ENABLE_MSTV_UART_DEBUG)
#define DP_printData(str, value)   printData(str, value)
#define DP_printMsg(str)           printMsg(str)
#else
#define DP_printData(str, value)
#define DP_printMsg(str)
#endif
                                                            // Scaler Port.
BYTE PortToCombo[2] = {0, 1};                               // Which DP Port in Combo 0 ~ 5
BYTE ComboToPort[7] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};   // HBR2 0 ~ 1  in scan port.
                      // 0     1     2     3     4     5     6
BYTE HBR2ToScanPort[2] = {0};

BYTE xdata DP_Connected[6] ={0};
BYTE xdata ucDPEnableIndex_Temp =0;
BYTE xdata ucDisplayEnableIndex_Temp=0;
HBR2_IP_SELECT xdata DP_HBR2_0=HBR2_NONUSE;
HBR2_IP_SELECT xdata DP_HBR2_1=HBR2_NONUSE;
//HBR_IP_SELECT  xdata DP_HBR_0=HBR_NONUSE;
//HBR_IP_SELECT  xdata DP_HBR_1=HBR_NONUSE;
BYTE port=0;
BYTE DPConfigFinish=0;
BYTE DPCombo_Update=0;
WORD Aux =0;
BYTE DDCData[16] ={0};
#if (COMBO_DP_DAISY_CHAIN_SUPPORT == 0x1)
BYTE  GUIDData[16] = {0x0};
#else
BYTE  GUIDData[16]={0x10,0xDE,0x90,0x70,0x00,0x00,0x00,0x02,0xC0,0x7C,0x4E,0x44,0x00,0x00,0x00,0x56};
#endif
//BYTE  GUIDData[16]={0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff,0xcf};
WORD u16DPAudio_BWCnt;
bool    DPRxAudioMuteEn=0;
BYTE xdata ucDPVersion=E_DP_V12;
BYTE ucDPDaisyChainFlag = 0;
#if ENABLE_DP_ShortHPD_PM	
BOOL bHPDShortPulse[DP_INPUT_SELECT_MASK] = {FALSE};
#endif

#if DPSSC
WORD DPRxAudioCnt=0;
WORD DPRxAudioSSCCnt=0;
WORD DPRxAudioSSCEn=0;
BYTE   DPAudio_State=0;
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
Bool    DPAudioUpDown=0;

BYTE DPAudioTestFlag = 0;
BYTE DPAudioTestCount = 0;

#endif

#define  AUTO_EQ      1

#define  DP_AUX_XTAL                  1 // (1 : XTAL  , 0 : OSC)
//#define  Support_HBR2                     1

#define DP_V_BLANKING_VALID_RANGE	0x400

/////// temp //////
#if (MS_BOARD_TYPE_SEL == BD_MST182A_A01A_S)
    #define  AUX_TESTBUS    1
#else
    #define  AUX_TESTBUS    0
#endif
/////// temp //////



//BYTE  XDATA   DP_Data[32] _at_ 0x4A00;
#if ENABLE_DP_INPUT
code RegTbl2Type tComboDPMuxTbl[]=
{

    {REG_1542, 0x1189, 0xFFFF},     // Override Ch0 DAC
    {REG_17A0, 0x0246, 0xFFFF},

};
#endif

#if 0
BYTE code  EDID[256] =       //Port B  EDID
{

0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x3E, 0x93, 0x08, 0xC0, 0x00, 0x00, 0x00, 0x00,
0x20, 0x15, 0x01, 0x04, 0xA5, 0x33, 0x1D, 0x78, 0x22, 0x1E, 0x55, 0xA0, 0x59, 0x56, 0x9F, 0x27,
0x0D, 0x50, 0x54, 0xBD, 0x4B, 0x00, 0xD1, 0xC0, 0x81, 0x80, 0x95, 0x0F, 0x95, 0x00, 0xB3, 0x00,
0x81, 0xC0, 0x01, 0x01, 0x01, 0x01, 0x02, 0x3A, 0x80, 0x18, 0x71, 0x38, 0x2D, 0x40, 0x58, 0x2C,
0x45, 0x00, 0xFD, 0x1E, 0x11, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x0A, 0x20, 0x20,
0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x56,
0x53, 0x32, 0x33, 0x30, 0x50, 0x47, 0x4C, 0x57, 0x0A, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xFD,
0x00, 0x38, 0x4C, 0x1E, 0x53, 0x11, 0x00, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x01, 0x6E,
0x02, 0x03, 0x0C, 0xC1, 0x23, 0x09, 0x17, 0x07, 0x83, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60

};

//BYTE code DPCPBKSV[5] =
//{
//    0x89, 0x38, 0xae, 0x0d, 0xed
//};
#endif
BYTE ucDPHBR20PortSelect = 0;
BYTE ucDPHBR21PortSelect = 0;

#if(DP_MCCS_NEW_MODE)
BYTE ucDDCNullMessage[DP_AUX_COMMAND_DDC_LENGTH] = {0x6e, 0x80, 0xbe};
BYTE ucDDCBuffer[DP_AUX_COMMAND_MAX_LENGTH] = {0};

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

#endif

//**************************************************************************
//  [Function Name]:
//                  mdrv_DP_GetHBR2PortSelect()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE mdrv_DP_GetHBR2PortSelect(BYTE ucPortSelect)
{
    BYTE ucInputSelect = 0;
    BYTE ucHBR2Select = DP_HBR2_ENGINE_NONE;
    BYTE ucHBR20PortSelect = ucDPHBR20PortSelect;
    BYTE ucHBR21PortSelect = ucDPHBR21PortSelect;

    if(!DPConfigFinish)
    {
        ucDPHBR20PortSelect = DP_INPUT_SELECT_MASK;
        ucDPHBR21PortSelect = DP_INPUT_SELECT_MASK;

        for(ucInputSelect = DP_INPUT_SELECT_PORT0; ucInputSelect < DP_INPUT_SELECT_MASK; ucInputSelect++)
        {
            if(ucHBR20PortSelect %2)
            {
                if(ucHBR20PortSelect > 1) // More than one port HBR2_0 config
                {
                    DP_printData("** DP more than one HBR2_0 config %d", ucDPHBR20PortSelect);
                }

                ucHBR20PortSelect = 0;
                ucDPHBR20PortSelect = ucInputSelect;
            }
            else
            {
                ucHBR20PortSelect = ucHBR20PortSelect >> 1;
            }

            if(ucHBR21PortSelect %2)
            {
                if(ucHBR21PortSelect > 1) // More than one port HBR2_1 config
                {
                    DP_printData("** DP more than one HBR2_1 config %d", ucDPHBR21PortSelect);
                }

                ucHBR21PortSelect = 0;
                ucDPHBR21PortSelect = ucInputSelect;
            }
            else
            {
                ucHBR21PortSelect = ucHBR21PortSelect >> 1;
            }
        }

        DPConfigFinish = TRUE;
    }

    if(ucPortSelect == ucDPHBR20PortSelect)
    {
        ucHBR2Select = DP_HBR2_ENGINE_0;
    }
    else if(ucPortSelect == ucDPHBR21PortSelect)
    {
        ucHBR2Select = DP_HBR2_ENGINE_1;
    }

    return ucHBR2Select;
}

void DPVersionSetting(BYTE enInputPort, BYTE bVer )
{
    WORD u16Regoffset=0;

    u16Regoffset = enInputPort * HBR2_OFFSET;
    if(bVer == E_DP_V11)
    {
        msWriteByte( REG_113300+u16Regoffset, 0x11 );      // DPCD 1.1 Version
        msWriteByte( REG_113301+u16Regoffset, 0x0A );           // DP main link rate 2.7G
    }
    else
    {
        msWriteByte( REG_113300+u16Regoffset, 0x12 );           // DPCD 1.2 Version
        msWriteByte( REG_113301+u16Regoffset, 0x14 );           // DP main link rate 5.4G
    }

    u16Regoffset = PortToCombo[enInputPort]*COMBO_PHY_OFFSET;

    if(bVer == E_DP_V11)
    {
        msWriteByte( REG_001100+u16Regoffset, 0x11 );     // DP12
        msWriteByte( REG_001101+u16Regoffset, 0x0A );           //cyc : 5.4G                                                           // Support 1.62G														 // Support 1.62G
        msWriteByteMask( REG_001103+u16Regoffset, 0x89, 0xBF);      // Support TP3
    }
    else
    {
        msWriteByte( REG_001100+u16Regoffset, 0x12 );           // DPCD 1.2 Version
        msWriteByte( REG_001101+u16Regoffset, 0x14 );           // DP main link rate 5.4G													 // Support 1.62G
        msWriteByteMask( REG_001103+u16Regoffset, 0x8B, 0xBF);      // Support TP3
    }
}

void DPRxInit( void )
{
    WORD i=0,j=0;
    BYTE K=0;
    memset(&gDPInfo, 0, sizeof(gDPInfo));
    memset(&gDPCombo, 0, sizeof(gDPCombo));

    for(i=0;i<DP_NUM;i++)
     {
    	gDPInfo[i].ucState = eSTATE_STARTUP;
	gDPInfo[i].bDPSplitEn_Cnt_Up =0xFF;
      	gDPInfo[i].bDPSplitEn_Cnt_Down =0xFF;
    }


    for(i=0;i<6;i+=1)
    {
         msWriteByteMask( REG_0011A9+i*COMBO_PHY_OFFSET, BIT5, BIT5);                           //affect SAR11/SAR3 when MHL
        msWriteByte( REG_00117B+i*COMBO_PHY_OFFSET, 0xFF );                                 // Mask ALL AUX interrupt
        msWriteByte( REG_001191+i*COMBO_PHY_OFFSET, 0xF0 );                                 // Mark _dpcd0_irq_mask
        msWriteByteMask( REG_001193+i*COMBO_PHY_OFFSET, BIT6, BIT6);                            // Mask __aksv_irq_mask
        msWriteByteMask( REG_001196+i*COMBO_PHY_OFFSET, BIT4|BIT7, BIT4|BIT7);                  // Mask SQ and Time Out  Mask
        msWriteByteMask( REG_0011AE+i*COMBO_PHY_OFFSET,0, BIT0|BIT1|BIT2);                     // turn off PM detect
        if(DP_Connected[i] ==0)
            continue;
        #if !ENABLE_DAISY_CHAIN
        msWriteByteMask(REG_0017B4 , BIT2<<i ,BIT2<<i);                                 //Common mode Voltage
        #endif
        DP_printData( " DP_Connected =>ComboPort  %x ",i);
        DPRxRterm((Combo_IP_SELECT)i);
        DPRxAUXInit(i);
        DPRxFastTrainingInitial(i);
      //  DPRxComboInit(i);
     DPRxGUIDInit(i, GUIDData);
     DPRxAutoEQ((Combo_IP_SELECT)i);
     DPRxPHYPowerOnOff((Combo_IP_SELECT)i,TRUE);
 	 DPRxSpecificFiled((Combo_IP_SELECT)i);
        // ##############################################
        // ##   (0~5)                              BANK 1120  Phy Setting                    ##
        // ##############################################
        msWriteByteMask(REG_112000 + i*COMBO_PHY_OFFSET, 0, BIT7);           // Sleect DP port
        msWrite2Byte(REG_112080 + i*COMBO_PHY_OFFSET, 0x168);                   // HBR2
        msWrite2Byte(REG_112084 + i*COMBO_PHY_OFFSET, 0x2d0);                   // HBR
        msWrite2Byte(REG_112086 + i*COMBO_PHY_OFFSET, 0x360);                   // RBR

        msWriteByteMask(REG_1120AF + i*COMBO_PHY_OFFSET, BIT0|BIT1, BIT0|BIT1|BIT2);   //Enable DP mode In nonFullRate mode.

        msWriteByteMask(REG_1120D9 + i*COMBO_PHY_OFFSET, BIT0,BIT0);        //Enable DP mode

        msWriteByte(REG_1120A6 + i*COMBO_PHY_OFFSET, 0x0F);

        msWriteByte(REG_112022 + i*COMBO_PHY_OFFSET, 0x00);
        msWriteByte(REG_112023 + i*COMBO_PHY_OFFSET, 0x00);
        msWriteByte(REG_112024 + i*COMBO_PHY_OFFSET, 0x00);
        msWriteByte(REG_112025 + i*COMBO_PHY_OFFSET, 0x00);
        msWriteByte(REG_112026 + i*COMBO_PHY_OFFSET, 0x00);
        msWriteByte(REG_112027 + i*COMBO_PHY_OFFSET, 0x00);

        msWriteByteMask(REG_1120B3 + i*COMBO_PHY_OFFSET, BIT0,BIT0);
        msWriteByteMask(REG_1120B7 + i*COMBO_PHY_OFFSET, 0,0xF0);

        msWrite2Byte(REG_112076 + i*COMBO_PHY_OFFSET, 0x1F00);

        msWriteByteMask(REG_112014 + i*COMBO_PHY_OFFSET, BIT6,BIT6);         // 20Bit Clk

        msWriteByteMask(REG_112090 + i*COMBO_PHY_OFFSET, 0x05, 0xFF);         //The CR lock reference correct threshold range for HBR/RBR

#if(COMBO_DP_DAISY_CHAIN_SUPPORT)
        if(i == Combo_PORT1)
        {
		msWriteByte(REG_1120BE + i*COMBO_PHY_OFFSET, 0x04);
		msWriteByte(REG_1120BF + i*COMBO_PHY_OFFSET, 0x00);
		msWriteByte(REG_1120C0 + i*COMBO_PHY_OFFSET, 0x04);
		msWriteByte(REG_1120C1 + i*COMBO_PHY_OFFSET, 0x00);
		msWriteByte(REG_1120C2 + i*COMBO_PHY_OFFSET, 0x04);
		msWriteByte(REG_1120C3 + i*COMBO_PHY_OFFSET, 0x00);
		msWriteByte(REG_1120C4 + i*COMBO_PHY_OFFSET, 0x04);
		msWriteByte(REG_1120C5 + i*COMBO_PHY_OFFSET, 0x00);
        }
		else
		{
			msWriteByte(REG_1120BE + i*COMBO_PHY_OFFSET, 0x60);  //Always turn on 3 stage.
			msWriteByte(REG_1120BF + i*COMBO_PHY_OFFSET, 0x00);
			msWriteByte(REG_1120C0 + i*COMBO_PHY_OFFSET, 0x60);
			msWriteByte(REG_1120C1 + i*COMBO_PHY_OFFSET, 0x00);
			msWriteByte(REG_1120C2 + i*COMBO_PHY_OFFSET, 0x60);
			msWriteByte(REG_1120C3 + i*COMBO_PHY_OFFSET, 0x00);
			msWriteByte(REG_1120C4 + i*COMBO_PHY_OFFSET, 0x60);
			msWriteByte(REG_1120C5 + i*COMBO_PHY_OFFSET, 0x00);
		}
#else
		msWriteByte(REG_1120BE + i*COMBO_PHY_OFFSET, 0x60);  //Always turn on 3 stage.
		msWriteByte(REG_1120BF + i*COMBO_PHY_OFFSET, 0x00);
		msWriteByte(REG_1120C0 + i*COMBO_PHY_OFFSET, 0x60);
		msWriteByte(REG_1120C1 + i*COMBO_PHY_OFFSET, 0x00);
		msWriteByte(REG_1120C2 + i*COMBO_PHY_OFFSET, 0x60);
		msWriteByte(REG_1120C3 + i*COMBO_PHY_OFFSET, 0x00);
		msWriteByte(REG_1120C4 + i*COMBO_PHY_OFFSET, 0x60);
		msWriteByte(REG_1120C5 + i*COMBO_PHY_OFFSET, 0x00);
#endif


    }

    for(j=0;j<DP_NUM;j++)
    {
        i = j * HBR2_OFFSET;


        // ##############################################
        // ##      (0~1)                            BANK 1130                                     ##
        // ##############################################
        msWriteByteMask( REG_11307B+i, BIT7,BIT7 );                             // K28.5
	 msWriteByteMask( REG_113001+i, BIT5,BIT5 );                             // High Bandwidth
        // ##############################################
        // ##      (0~1)                            BANK 1133                                     ##
        // ##############################################

        msWriteByteMask(REG_113389+i, _BIT7, _BIT7);    //Max SWIM EN
        msWriteByte(REG_113391+i, 0xAA);                // Swim Level

        msWriteByte( REG_1133E0+i, 0xFF );          // mask INT
        msWriteByte( REG_1133E1+i, 0xFF );          // mask INT
        msWriteByte( REG_1133E2+i, 0xFF );          // mask INT
        msWriteByte( REG_1133E3+i, 0xFF );          // mask INT
        msWriteByte( REG_1133E8+i, 0xFF );          // clear INT
        msWriteByte( REG_1133E9+i, 0xFF );          // clear INT
        msWriteByte( REG_1133EA+i, 0xFF );          // clear INT
        msWriteByte( REG_1133EB+i, 0xFF );          // clear INT

        msWriteByte( REG_1133F0+i, 0x00 );           // AUX TIME OUT REPLY ACK FOR CTS ISSUE
        #if 0
	        msWriteByte( REG_113300+i, 0x12 );           // DPCD 1.2 Version
	        msWriteByte( REG_113301+i, 0x14 );           // DP main link rate 5.4G
        #else
		DPVersionSetting(j,USER_PREF_DPConfigType);
        #endif

        msWriteByte( REG_113302+i, 0x84 );           // DP Enhanced Frame capability & Max Lane Count
        msWriteByte( REG_113304+i, 0x01 );            // DP Number of Receiver Ports
        msWriteByte( REG_11331C+i, 0x80 );           // DP Linkstatus andadjust requestupdated
        msWriteByte( REG_11331E+i, 0x00 );            // LANE0,1 Pre-emphasis & VoltageSwing
        msWriteByte( REG_11331F+i, 0x00 );            // LANE2,3 Pre-emphasis & VoltageSwing
        msWriteByte( REG_1133D4+i, 0x01 );           // RS_SWAP & DP receiever enable
        msWriteByte( REG_1133DC+i, 0x80 );           // USE Lanes count mask lock status
        msWriteByte( REG_1133F1+i, 0xD0 );            // AUX length compare mask
        msWriteByte( REG_1133D2+i, 0xFF );           // Video PLL freeze disable    Audio Enable Frezzen
        msWriteByte( REG_1133D3+i, 0x72 );           // [3]:=> 1:Dither output 8 bits , 0: 10 bits
        msWriteByte( REG_1133DD+i, 0x80 );           //  Add by CCJ
        msWriteByte( REG_1133D6+i, 0x10 );            // Set 0x10
        msWriteByte( REG_1133D8+i, 0x20 );            // FIFO Delay for Reduce Blanking issue
  //  msWriteByte( REG_1133B5+i, 0x08 );            // Auto Reply Defer when Auto EQ.
        msWriteByte( REG_1133A2+i, 0x20 );           //MVID_Range[7:0]
        msWriteByte( REG_1133A3+i, 0x00 );           //MVID_Range[15:8]
        msWriteByte( REG_1133A4+i, 0x00 );           //MVID_Range[23:16]
        msWriteByte( REG_1133D1+i, 0x30 );           // Bit5: Video M/N Bit4: Audio M/N
        msWriteByte( REG_1133C1+i, msReadByte( REG_1133C1+i ) | _BIT0 );
        msWriteByte( REG_1133C2+i, 0x98 );           // BS-> RE_GEN HPWS delay for Songlu U02
        msWriteByte( REG_1133C6+i, 0x04 );
        msWriteByte( REG_1133CF+i, 0x07 );
        msWriteByte( REG_113399+i, msReadByte( REG_113399+i ) | _BIT7 ); //vblank config enable
        msWriteByte( REG_113397+i, 0x03 );           //  Glitch filter use old mode  (Warning: HLBR ISSUE)  No gltch
        msWriteByte( REG_113396+i, 0xFF );           //  Glitch filter use old mode  (Warning: HLBR ISSUE)  No gltch
        msWriteByte( REG_1133CC+i, 0x61 );          //(bit3=0 use VB_ID rising as frame flag (end of active))

        msWriteByte( REG_1133A2+i, 0x20 );           //MVID_Range[7:0]
        msWriteByte( REG_1133A3+i, 0x00 );           //MVID_Range[15:8]
        msWriteByte( REG_1133A4+i, 0x00 );           //MVID_Range[23:16]

        msWriteByte( REG_1133AA+i, 0x20 );           //NVID_Range[7:0]
        msWriteByte( REG_1133AB+i, 0x00 );           //NVID_Range[15:8]
        msWriteByte( REG_1133AC+i, 0x00 );           //NVID_Range[23:16]

        msWriteByte( REG_1133AE+i, 0x10 );           //MAUD_CENTER[7:0]
        msWriteByte( REG_1133AF+i, 0x00 );           //MAUD_CENTER[15:8]
        msWriteByte( REG_1133B0+i, 0x00 );           //MAUD_CENTER[23:16]
        msWriteByte( REG_1133B2+i, 0x10 );           //MAUD_Range[7:0]
        msWriteByte( REG_1133B3+i, 0x00 );           //MAUD_Range[15:8]
        msWriteByte( REG_1133B4+i, 0x00 );           //MAUD_Range[23:16]
        msWriteByte( REG_1133B6+i, 0x00 );           //NAUD_CENTER[7:0]
        msWriteByte( REG_1133B7+i, 0x10 );           //NAUD_CENTER[15:8]
        msWriteByte( REG_1133B8+i, 0x00 );           //NAUD_CENTER[23:16]
        msWriteByte( REG_1133B9+i, 0x10 );           //NAUD_Range[7:0]
        msWriteByte( REG_1133BA+i, 0x00 );           //NAUD_Range[15:8]
        msWriteByte( REG_1133BB+i, 0x00 );           //NAUD_Range[23:16]

        // ##############################################
        // ##                                        BANK 1134                                     ##
        // ##############################################

        msWriteByteMask(REG_113473+i, 0, BIT3);  // Common Mode Aux
        msWriteByte( REG_1134E0+i, 0xCE );           // clk enable  Bit0=0 EQ issue
        msWriteByte( REG_1134E1+i, 0x6F );           // Bit 5: For Squelch clock
        msWriteByte( REG_11346C+i, 0x00 );           // DP PLL POWER DOWN
        msWriteByte( REG_11346E+i, 0x01 );           // Training PFD Divide 4

        msWriteByte( REG_11346D+i, 0x00 );           // DP_SEL_R50_VCM 3.3V & close powerdown
        msWriteByte( REG_11346F+i, 0xF4 );
        msWriteByte( REG_113474+i, 0x30 );           // EQ common mode, TX_OFF
        msWriteByte( REG_113476+i, 0x38 );           // DP_AUX 6ma 10ns 800mv
        msWriteByte( REG_113480+i, 0x00 );           // DP PHY Resistor power down.  Enable Rterm
        msWriteByte( REG_113481+i, 0x36 );           // DP PHY Resistor Control Overwrite enable. DP PHY Resistor Control. Rterm 50ohm
        msWriteByte( REG_11348C+i, 0x10 );           // DP EQ threshold set 0 for EQ lose lock issue : DP EQ threshold set 10h
        msWriteByte( REG_11348A+i, 0x04 );           // 12M DP main link clock lock threshold when 162MHz.
        msWriteByte( REG_11348B+i, 0x10 );           // 12M DP main link clock lose threshold when 162MHz.
        msWriteByte( REG_113492+i, 0x2D );           // 12M DP main link clock lock when 270MHz.  0x5A0
        msWriteByte( REG_113493+i, 0x01 );           // 12M DP main link clock lock when 270MHz.
        msWriteByte( REG_113494+i, 0x04 );           // 12M DP main link clock lock threshold when 270MHz.
        msWriteByte( REG_113495+i, 0x12 );           // 12M DP main link clock lose threshold when 270MHz.
        msWriteByte( REG_113496+i, 0x6A );           // 12M DP main link clock lock when 162MHz.    0x6C0
        msWriteByte( REG_113497+i, 0x01 );           // 12M DP main link clock lock when 162MHz.
        msWriteByte( REG_113498+i, 0x10 );           // 14M DP AUX PHY UI counter threshold.
        msWriteByte( REG_113499+i, 0x0D );           // 14M DP AUX PHY over sample rate.
        msWriteByte( REG_11349A+i, 0x63 );           // 14M DP AUX timeout threshold.
        msWriteByte( REG_11349B+i, 0x08 );           // 14M DP AUX timeout threshold.  AUX CLK select 50Mhz
        msWriteByte( REG_113468+i, 0x6  );           // DP EQ0 SET   3Ch
        msWriteByte( REG_113469+i, 0x6  );           // DP EQ1 SET   3Ch
        msWriteByte( REG_113487+i, 0x6  );           // DP EQ2 SET   3Ch
        msWriteByte( REG_113489+i, 0x6  );           // DP EQ3 SET   3Ch
        msWriteByte( REG_11345E+i, 0x04 );            // BIT4 ignore Audio ID
        msWriteByte( REG_11349E+i, 0x0C );            // BIT2 Align Word Boundary Overwrite Enable
        msWriteByte( REG_11349C+i, 0x3f );           // DP main link lock Divider 2.7G.
        msWriteByte( REG_11349D+i, 0x7f );           // DP main link lock Divider 1.62G
        msWriteByte( REG_1134FA+i, 0xCF );         //  Bit1: Disable AUPLL Frezze interrupt , Bit6: CLK HDCP_EN , Bit7: SD clk
        msWriteByte( REG_1134D1+i, 0x00 );           // VPLL_PWDWN
        msWriteByte( REG_1134B3+i, 0x00 );           // APLL disable PWD
        msWriteByte( REG_1134B8+i, 0x22 );           // APLL 512*Fs at 1.62Ghz 162Mhz
        msWriteByte( REG_1134C6+i, 0x09 );           // BW more speed up
        msWriteByte( REG_1134D5+i, 0x0F );           // [1:0] option enable Training P1 P2 IRQ To BankB 0xE8[0:1] [4:2] Pre-syn zero
        msWriteByte( REG_11346E+i, 0x00 );           //  set default
        msWriteByte( REG_11346F+i, 0x00 );           //  set default
        msWriteByteMask( REG_113475+i, BIT7,BIT7 );     // AUX won¡¦t be powerdown

        msWriteByteMask( REG_11349F+i, BIT7|BIT6,BIT7|BIT6);     // MAC
        msWriteByteMask( REG_1134F7+i, BIT7|BIT5,BIT7|BIT5 );             // [5]CDR and SQ [7] Bypass bypass check interskew align
        // ##############################################
        // ##                                        BANK 1135                                     ##
        // ##############################################
        msWriteByteMask( REG_113581+i, BIT5,BIT5);                            //de-glitch when frame reset (1680X1050)
        msWriteByte( REG_113580+i, 0x41 );                                                     // Htotal Tolerance (Learn)
        msWriteByte( REG_1135A2+i, _BIT7 | _BIT6 | _BIT1 | _BIT0 );               // Aux Clock Src,// Set AUX Debounce clk
        msWriteByte( REG_11356E+i, 0xFF );                                                     // New 8/10b Fifo
        msWriteByte( REG_1135C8+i, 0x0F );                                                     // select new 10b/8b decoder
        msWriteByte( REG_1135A0+i, 0x40 );                                                     // Enable AUX Debounce
        msWriteByte( REG_11350D+i, 0 );                                                          // set V paulse  small
        msWriteByte( REG_113518+i, 0x1F );
        msWriteByte( REG_113519+i, 0x3F );
        msWriteByte( REG_113507+i, 0x65);//0xE5 );                                                    //DP Interlace mode
        msWriteByte( REG_11357F+i, 0x01 );                                                    //Reset Word Boundary
        msWriteByteMask( REG_113581+i, BIT3 | BIT2, BIT3 | BIT2 );
        msWriteByteMask( REG_113541+i, BIT3, BIT3 );
        msWriteByteMask( REG_1135D4+i, BIT2|BIT1, BIT2|BIT1 );                   // HDCP Rx
	 msWriteByteMask( REG_113501+i, BIT5, BIT5 );					  // Vblk_en_sel   2014/6/17
	
        // ##############################################
        // ##                                        BANK 1132                                     ##
        // ##############################################
        msWriteByte( REG_1132C7+i, 0x00);       							           //20141125MSACHG
        msWriteByte( REG_1132A8+i, 0x0C );                                                            //[2]: ignore Audio ID [3] Enable RS code sync reset
        msWriteByte( REG_1132EC+i, 0xFF );                                                           //[5]:Enable Audio time Stamp [6]Audio Stream [7] Audio Protect
        msWriteByte(REG_1132F0+i, 0x7F);                                                             //[0]~[7] Enable Audio Mute Event.
        msWriteByte(REG_1132F1+i, 0x7F);                                                             //[0]~[7] Enable Audio Fading Event.
        msWriteByteMask( REG_1132ED+i, _BIT1 | _BIT3, _BIT1 | _BIT3 );             //Enable VSC Packet  // receive Packet Update

        // ##############################################
        // ##                                         BANK 1131                                     ##
        // ##############################################
		#if (COMBO_DP_DAISY_CHAIN_SUPPORT == 0x1)
		msWriteByte( REG_113100+i, 0xB1 );
		#else
		msWriteByte( REG_113100+i, 0x81 );
		#endif
        msWriteByte( REG_113102+i, 0x84 );

		// ##############################################
        // ##                        DPMS/WakeUp patch code                                     ##
        // ##############################################

		//4K2K@30Hz, reduced blanking as default re-gen timing settings.
		msWriteByteMask(REG_113508+i, BIT1 | BIT0, BIT1 | BIT0 );
		msWrite2Byte(REG_11350C+i, 0xF );
	    msWriteByte(REG_113540+i, 0x8);
	    msWriteByte(REG_1135A7+i, 0x10);
	    msWriteByteMask(REG_113501+i, BIT4, BIT4 );    
	   	msWrite2Byte(REG_11350A+i, 0x34 );

		msWriteByteMask(REG_113502+i, BIT6, BIT6);
		ForceDelay1ms(1);
	    msWriteByteMask(REG_113502+i, 0x0, BIT6);    

    }
    // ##############################################
    // ##                                        AUPLL                               ##
    // ##############################################
    DPRxSetAUPLL(0);
    // ##############################################
    // ##                                        BANK 1630  HBR2      port0               ##
    // ##############################################
    DPRxSetHBR2_0();
    // ##############################################
    // ##                                        BANK 1630  HBR2      port1               ##
    // ##############################################
    DPRxSetHBR2_1();
    // ##############################################
    // ##                                        BANK 1630                                       ##
    // ##############################################
    DPRxEnSplit(DP_1P,HBR2_0);
    DPRxEnSplit(DP_1P,HBR2_1);
    msWrite2Byte(0x1630EE, 0x1FFF); //clk to scaler
    msWriteByte(REG_16308B, 0xFF);                  // Audio  Mute Event
    // ##############################################
    // ##                                        BANK 162F                                    ##
    // ##############################################
    msWriteByte( REG_162F0B, 0x2A );                                        // Audio Low Pass Filter
    msWriteByteMask( REG_162F11, 0,BIT7 );                                      // Bypass Mode =0
    msWrite2Byte( REG_162FF8, 0xFFFF );
    msWriteByte( REG_162FFA, 0xFF );
    msWriteByteMask( REG_162F07, 0, BIT0 );
    msWriteByteMask( REG_162F41, BIT0, BIT0 );
    #if DPSSC
    msWriteByteMask( REG_162F70, BIT5, BIT5 );   							// Audio ECO Enable
    msWriteByteMask( REG_162F22, 0x09, 0x0F );
    msWriteByteMask( REG_162F2A, BIT4 ,BIT4 );
    #endif
    // ##############################################
    // ##                                        BANK 17 COMBO PM                         ##
    // ##############################################
    msWriteByteMask(REG_0017A6 , (ucDPEnableIndex_Temp&0x3F),0x3F);                 //Enable SQ
    msWriteByteMask(REG_0017C2 , BIT2 ,BIT2);                                               //Enable SQ
   // msWriteByteMask(REG_0017C4 , 0 ,BIT3|BIT2|BIT1);
   // msWriteByteMask(REG_001781 , 0 ,BIT0);

   // msWriteByte(REG_0017B9, 0xFF );                                                    // Enable AUX0 ~ AUX5 ISR
    // ##############################################
    // ##                                        BANK 03 PM                                      ##
    // ##############################################
    msWriteByteMask(REG_000381 , BIT3 ,BIT3);                                               // Fake Training Reply Correctly.

    //======================================================
    //          aux Scipt
    //========================================================
#if AUX_TESTBUS
    msWrite2Byte( REG_0011A8, 0x1044 );
    msWrite2Byte( REG_001176, 0x0014 );    //  0x1276
    msWrite2Byte( REG_0017B2, 0x0008 );    //  0x10
    msWrite2Byte( REG_0003BC, 0xC001 );
    msWrite2Byte( REG_101EEA, 0x4680 );//bit7 for DP switch control
    msWrite2Byte( REG_101E82, 0xFFFF );
    msWrite2Byte( REG_000400, 0x0040 );    // Test Bus DP tx . GPIO_66
#endif




    //========================================================
    //msDrvSrcSetHBR2FlagMux(1, TRUE,TYPE_HBR2_0|TYPE_DP);
    //msDrvSrcSetHBR2FlagMux(2, TRUE,TYPE_HBR2_1|TYPE_DP);

    DPRxHDCPLoadKey();

    mdrv_DP_GetHBR2PortSelect(0);

    for(j=0;j<(DP_NUM);j++)
    {
        DPRxIRQComboEnable( ENABLE, j);                                         // HBR 0/1 Interrupt
    }
    for(K=0;K<6;K+=1)
    {
        if(DP_Connected[K] ==0)
            continue;

        DP_printData( " Port ==> %x",K);
        DPRxIRQAuxEnable( ENABLE, (Combo_IP_SELECT)K);
        //DPRxHPDEnable(TRUE, (Combo_IP_SELECT)K);
    }
    #if 1
    for(i=0;i<=6;i++)
    DP_printData(" ComboToPort<%x>", ComboToPort[i]);    // HBR2_0   HBR2_1
    for(i=0;i<=1;i++)
    DP_printData(" PortToCombo<%x>", PortToCombo[i]);
    DP_printData( "DP_HBR2_0=  %x", DP_HBR2_0);
    DP_printData( "DP_HBR2_1=  %x", DP_HBR2_1);
    #endif
    //msWriteByteMask(REG_000455, 0x0, BIT2);
}

//**************************************************************************
//  [Function Name]:
//                  DPRxRterm()
//  [Description]
//                  DPRxRterm()
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void  DPRxPHYPowerOnOff(Combo_IP_SELECT i,  bool  bEnable)
{

       if(bEnable)
       {
        msWriteByteMask(REG_1120D7 + i*COMBO_PHY_OFFSET, 0,BIT0);          // Band Gap On
        msWriteByte(REG_112016 + i*COMBO_PHY_OFFSET, 0xFF);                          // Clock EN
        msWriteByte(REG_112017 + i*COMBO_PHY_OFFSET, 0xFF);                          // Clock EN
        msWrite2Byte(REG_112018 + i*COMBO_PHY_OFFSET, 0x3FFF);                     // Clock EN
        msWriteByteMask(REG_1120B6 + i*COMBO_PHY_OFFSET, 0x00,0x0F);          //Enable DP Lane power
       }
    else
    {
        msWriteByteMask(REG_1120D7 + i*COMBO_PHY_OFFSET, BIT0,BIT0);       // Band Gap On
        msWriteByte(REG_112016 + i*COMBO_PHY_OFFSET, 0x00);                          // Clock EN
        msWriteByte(REG_112017 + i*COMBO_PHY_OFFSET, 0x00);                          // Clock EN
        msWrite2Byte(REG_112018 + i*COMBO_PHY_OFFSET, 0x0000);                     // Clock EN
        msWriteByteMask(REG_1120B6 + i*COMBO_PHY_OFFSET, 0x0F,0x0F);          //Enable DP Lane power
    }

}

//**************************************************************************
//  [Function Name]:
//                  DPRxRterm()
//  [Description]
//                  DPRxRterm()
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void  DPRxRterm(Combo_IP_SELECT Input)
{

    if(Input == Combo_PORT0)
    {

        msWriteByteMask(REG_0017C4, 0, 0x0E); //[3:1]: Power on P0 data r-term
        msWriteByteMask(REG_001781, 0, 0x01); //[0]: Power on P0 clock r-term
    }
    else if(Input == Combo_PORT1)
    {

        msWriteByteMask(REG_0017C4, 0, 0xE0); //[7:4]: Power on P1 data r-term
        msWriteByteMask(REG_001781, 0, 0x02); //[1]: Power on P1 clock r-term
    }
    else if(Input == Combo_PORT2)
    {

        msWriteByteMask(REG_0017C5, 0, 0x0E); //[3:1]: Power on P2 data r-term
        msWriteByteMask(REG_001781, 0, 0x04); //[2]: Power on P2 clock r-term
    }
    else if(Input == Combo_PORT3)
    {

        msWriteByteMask(REG_0017A2, 0, 0x0E); //[3:1]: Power on P3 data r-term
        msWriteByteMask(REG_001781, 0, 0x08); //[3]: Power on P3 clock r-term
    }
    else if(Input == Combo_PORT4)
    {

        msWriteByteMask(REG_0017A2, 0, 0xE0); //[7:4]: Power on P4 data r-term
        msWriteByteMask(REG_001781, 0, 0x10); //[4]: Power on P4 clock r-term
    }
    else if(Input == Combo_PORT5)
    {

        msWriteByteMask(REG_0017A3, 0, 0x0E); //[3:1]: Power on P5 data r-term
        msWriteByteMask(REG_001781, 0, 0x20); //[5]: Power on P5 clock r-term
    }

}
//**************************************************************************
//  [Function Name]:
//                  DPRxAutoEQ()
//  [Description]
//                  DPRxAutoEQ()
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void  DPRxAutoEQ(Combo_IP_SELECT i)
{
    WORD j=0;

       j = HBRPort(i) * HBR2_OFFSET;

    #if !AUTO_EQ
    // DP EQ
    msWrite2Byte( REG_1120DC+i*COMBO_PHY_OFFSET, 0x03C0 );
   // msWrite2Byte( REG_1120E6+i*COMBO_PHY_OFFSET, 0x5AD6 ); // EQ=22
   // msWriteByteMask(REG_1120E8+i*COMBO_PHY_OFFSET, 0x16, 0x1F);
    msWrite2Byte( REG_1120E6+i*COMBO_PHY_OFFSET, 0x4210);     //  EQ=16
    msWriteByteMask(REG_1120E8+i*COMBO_PHY_OFFSET, 0x10, 0x1F);
      #else

#if(COMBO_DP_DAISY_CHAIN_SUPPORT)
    if(i == Combo_PORT1)
    {
		 // DP EQ
		 msWrite2Byte( REG_1120DC+i*COMBO_PHY_OFFSET, 0x03C0 );
		 msWrite2Byte( REG_1120E6+i*COMBO_PHY_OFFSET, 0x4210);	   //  EQ=16
		 msWriteByteMask(REG_1120E8+i*COMBO_PHY_OFFSET, 0x10, 0x1F);
    }
#else
    if(0)
    {}
#endif
    else
    {
    msWriteByteMask(REG_113000+j, 0x01, 0x01);
    ////msWriteByte( REG_11300D+j, 0x58 );
    msWriteByte( REG_113012+j, 0x00 );
//    msWriteByte( REG_112056+i*COMBO_PHY_OFFSET, 0x05);
    msWrite2Byte( REG_1120DC+i*COMBO_PHY_OFFSET, 0x0000 );
//    msWriteByteMask(REG_11204A+i*COMBO_PHY_OFFSET, 0x04, 0x04);
    msWriteByteMask(REG_1133B5+j, 0x20, 0x28);
    //Kujo
    //----SYMBOL
//    msWriteByteMask(REG_11204A+i*COMBO_PHY_OFFSET, 0x40, 0x40);   //wriu -b 0x11204a 0x40 0x40    //25[6]=1, 1:real timer count
    msWrite2Byte( REG_11204A+i*COMBO_PHY_OFFSET, 0x0A4C ); //wriu -w 0x11204a 0x0a4c //[4]=0, AABA

    msWrite2Byte( REG_112052+i*COMBO_PHY_OFFSET, 0x0003 );          //wriu -w 0x112052 0x0003        //29[15:0]:Time out for symbol lockdet (us) = 3us
    msWriteByte( REG_11348C+j, 0x60);               //wriu 0x11348c 0x60               //46[7:0]: EQ done threshold, //check symbol correct 0x60=96, 96*8*3.7n=2841n
    //TPS2=100bits, (96*4*0.37n*10)=2841ns, correct ratio=0.2841u/3u=0.947
    //if set 0 --> EQ=symbol
    //----EQ quality check (under/over)
    msWrite2Byte( REG_112050+i*COMBO_PHY_OFFSET, 0xbb80 ); //wriu -w 0x112050 0xbb80 //[2:0]=0 eq_accu th=0. [7]=1, speedup. [9:8]=11, set max, [11]=1, retune en. [15]=1, en EQ start
    msWrite2Byte( REG_112056+i*COMBO_PHY_OFFSET, 0x0007 ); //wriu -w 0x112056 0x0007    //2B[15:0]: time out for EQ under/over check (us) (ACCU=7us)
    msWrite2Byte( REG_112058+i*COMBO_PHY_OFFSET, 0x0002 ); //wriu -w 0x112058 0x0002    //2C[15:0]: fine tune time value (MASK=2us)
    //----EQ stable check
    msWrite2Byte( REG_11204C+i*COMBO_PHY_OFFSET, 0x8a0a ); //wriu -w 0x11204c 0x8a0a    //26[13:8]: max fine tune count to check if EQ stable or not,
    //start to check after count=10
    //[15]: enable autoEQ control symbol lock detect timeout to go to EQ fine tune
    msWrite2Byte( REG_11205A+i*COMBO_PHY_OFFSET, 0x0028 ); //wriu -w 0x11205a 0x0028    //2D[15:0], time out for fine tune (unit: find tune count)
    //time out after count=40
    msWrite2Byte( REG_11204E+i*COMBO_PHY_OFFSET, 0x0503 ); //wriu -w 0x11204e 0x0503    //27[13:8]: EQ stable tolerance =5
    //----sync request pre-emphasis
    msWrite2Byte( REG_113000+j, 0x0285 ); //wriu -w 0x113000 0x0285     //1130_00: [3:2][5:4][7:6]: 3 pre-emphasis levels, set (1,0,1)
    msWrite2Byte( REG_113076+j, 0x1010 ); //wriu -w 0x113076 0x1010     //1130_3b[7:0]: EQ error count th for each good EQ setting judgement
    //1130_3b[15:8]: EQ error count th for each EQ ph check
    msWriteByteMask(REG_113079+j, 0x0C, 0x0C); //wriu -b 0x113079 0x0c 0x0c //1130_3c[11:10]=11, [10]: align each lane pre-emphasis request value during autoEQ
    //[11]: udate best pre-emphasis setting when previous and current goodphase number are equal
    //----SCAN GOOD PHASE
    msWriteByte( REG_11300C+j, 0x18); //wriu 0x11300c 0x18      //06[7:0]:timer threshold 4x xtal for error counter accumulation period (unit: 0.28uS)
    msWriteByte( REG_11300D+j, 0x20); //wriu 0x11300d 0x20      //06[15:8]: error count mask (unit: LS_CLK)
    //The CR lock reference failure threshold range for HBR/RBR
    msWriteByteMask(REG_112091+i*COMBO_PHY_OFFSET, 0x09, 0xFF);
    // Auto EQ phase step
    msWriteByteMask(REG_113078+j, 0xC0, 0xC0); // step = 1
    //Always turn on PHDAC.
    msWriteByteMask(REG_1120B2+i*COMBO_PHY_OFFSET, 0x02, 0x02); // en pd_phdac overwirte
    msWriteByteMask(REG_1120D7+i*COMBO_PHY_OFFSET, 0x00, 0xF0); // ATOP - Power Down  PHDAC bit[15:12]
    msWriteByte( REG_112067+i*COMBO_PHY_OFFSET, 0x07);  //REG_112067    //wriu    0x112067 0x07 //eq start=7

//Auto EQ new setting 2013/09/05---------------------------------------------------------------
//REG_112052    //wriu -w 0x112052 0x03 //check symbol 3us
//REG_11348C    //wriu -w 0x11348c 0x60 //check symbol correct 0x60=96, 96*8*3.7n=2841n

    //////wriu 0x11300c 0x18    //06[7:0]:timer threshold 4x xtal for error counter accumulation period (unit: 0.333uS)
    //////          //0x18=24, 24*0.333u*20(ph)=160us
    //////wriu 0x11300d 0x20    //06[15:8]: error count mask (unit: LS_CLK), 0x20=32, 32*3.7n=118ns.

//REG_11204A    //wriu -w 0x11204a 0x0a4c //[4]=0, AABA
//REG_112050    //wriu -w 0x112050 0xbb80 //[2:0]=0 eq_accu th=0. [7]=1, speedup. [9:8]=11, set max, [11]=1, retune en. [15]=1, en EQ start
//REG_112056    //wriu -w 0x112056 0x0007 //eq accu=7u
//REG_112067    //wriu    0x112067 0x07 //eq start=7


//for AMD HD7870(option)
//wriu -b 0x113000 0x3c 0x14  //[3:2]:1st pre, [5:4]:2nd pre, set 2step pre=pre1, pre1
//wriu 0x112067 0x0d //eq start=13
//Auto EQ new setting 2013/09/05---------------------------------------------------------------
    }

#endif

    }
//**************************************************************************
//  [Function Name]:
//                  DPRxGUIDInit()
//  [Description]
//                  DPRxGUIDInit()
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void DPRxGUIDInit(BYTE temp, BYTE *GUIDPtr)
{
    BYTE  uctemp;
    for(uctemp =0;uctemp<=0x0F;uctemp++)
    {
        ForceDelay1ms(1);
        msWriteByte( REG_001162+(uctemp)+temp*COMBO_PHY_OFFSET, *(GUIDPtr+uctemp) );
    }
}
//**************************************************************************
//  [Function Name]:
//                  DPRxHPDEnable()
//  [Description]
//                  DPRxHPDEnable()
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void DPRxIRQAuxEnable( Bool bEnable, Combo_IP_SELECT Aux_Port )
{
    UNUSED(Aux_Port);

    if( bEnable )
    {
        DPRxSetAUXMask(Aux_Port);
        // msWriteByteMask(REG_00117B+COMBO_PHY_OFFSET* Aux_Port, 0 ,  BIT4);        //AUX
        // msWriteByteMask(REG_001196+COMBO_PHY_OFFSET* Aux_Port, 0 ,  BIT4);        //SQ
        msWriteByteMask(REG_00117B+COMBO_PHY_OFFSET* Aux_Port, 0 ,  BIT7);   //MCCS
        msWriteByteMask(REG_001191+COMBO_PHY_OFFSET* Aux_Port, 0 ,  BIT4);   //Program DPCD0  0030h
        //msWriteByteMask(REG_001191+COMBO_PHY_OFFSET* Aux_Port, 0 ,  BIT5);       //Program DPCD1   10Ah
        #if DP_HDCP_Reset
        msWriteByteMask(REG_001191+COMBO_PHY_OFFSET* Aux_Port, 0 ,  BIT6);       //Program DPCD2  0x68007
	 #endif
	 msWriteByteMask(REG_001193+COMBO_PHY_OFFSET* Aux_Port, 0 ,  BIT6);       //AKSV


        DP_printMsg( " DP AUX IRQ Enable!");
        msWriteByteMask(REG_002B29 ,0 , BIT5);                             // Enable AUX interrupt
    }
    else
    {
        DP_printMsg( " DP AUX IRQ Disable!");
        msWriteByteMask(REG_002B29 ,BIT5 , BIT5);                   // Disable AUX interrupt
    }

}

//**************************************************************************
//  [Function Name]:
//                  DPRxHDCPLoadKey()
//  [Description]
//                  DPRxHDCPLoadKey()
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void DPRxHDCPLoadKey( void )
{
    //WORD xdata  i;
    return;

    #if 0
    msWriteByteMask( REG_163080, BIT2|BIT3, BIT2|BIT3);
    msWriteByteMask( REG_163083, 0,BIT1|BIT0);
    for( i = 0; i < 5; i++ )
    {
        msWriteByte( REG_163082, i);
        msWriteByte( REG_163084, DPCPBKSV[i] );
        Delay1ms(5);
        msWriteByteMask( REG_163080, BIT4, BIT4);
        Delay1ms(5);
        msWriteByteMask( REG_163080, BIT5, BIT5);
        Delay1ms(5);
    }
    DP_printMsg( "------DPRxHDCPLoadKey OK!" );
   #endif	
}
//**************************************************************************
//  [Function Name]:
//                  DPRxHPDEnable()
//  [Description]
//                  DPRxHPDEnable()
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void DPRxHPDEnable(Bool bEnable,  Combo_IP_SELECT HPD_Port )
{
    if(bEnable)
    {
        if(HPD_Port == Combo_PORT0)
        {
            hw_Set_DPHpd0();
            hw_Set_DPCable5V0();
         }
        else if(HPD_Port == Combo_PORT1)
        {
            hw_Set_DPHpd1();
            hw_Set_DPCable5V1();
        }
        else if(HPD_Port == Combo_PORT2)
        {
            hw_Set_DPHpd2();
            hw_Set_DPCable5V2();
        }
        else if(HPD_Port == Combo_PORT3)
        {
            hw_Set_DPHpd3();
            hw_Set_DPCable5V3();
        }
        else if(HPD_Port == Combo_PORT4)
        {
            hw_Set_DPHpd4();
            hw_Set_DPCable5V4();
        }
        else if(HPD_Port == Combo_PORT5)
        {
            hw_Set_DPHpd5();
            hw_Set_DPCable5V5();
        }
    }
    else
    {
        if(HPD_Port == Combo_PORT0)
        {
            hw_Clr_DPHpd0();
            hw_Clr_DPCable5V0();
        }
        else if(HPD_Port == Combo_PORT1)
        {
            hw_Clr_DPHpd1();
            hw_Clr_DPCable5V1();
        }
        else if(HPD_Port == Combo_PORT2)
        {
            hw_Clr_DPHpd2();
            hw_Clr_DPCable5V2();
        }
        else if(HPD_Port == Combo_PORT3)
        {
            hw_Clr_DPHpd3();
            hw_Clr_DPCable5V3();
        }
        else if(HPD_Port == Combo_PORT4)
        {
            hw_Clr_DPHpd4();
            hw_Clr_DPCable5V4();
        }
        else if(HPD_Port == Combo_PORT5)
        {
            hw_Clr_DPHpd5();
            hw_Clr_DPCable5V5();
    }
}

}

//**************************************************************************
//  [Function Name]:
//                  DPRxHandle()
//  [Description]
//                  DPRxHandle
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void DPRxHandle( void )
{
    BYTE ComboPort=0;
    BYTE i =0;

    if(msRegs(REG_11332E) == 1)
    {
        for(i =0;i<=15;i++)
            DP_printData(" GUID=>%x ",GUIDData[i]);
        msRegs(REG_11332E) =0;
    }
    if(msRegs(REG_11332E) == 2)
    {
        for(i =0;i<=15;i++)
            DP_printData(" DDC =>%x ",DDCData[i]);
        msRegs(REG_11332E) =0;
    }



    for(ComboPort = 0; ComboPort < 7; ComboPort++ )  // Scaler Scan Port
    {

     	if( ComboToPort[ComboPort]==0xFF) 
    	{   								     // Map to DP_Connected
		continue;
	}    	     	
        port =  ComboToPort[ComboPort];

	 DPRxComboPort();
	 
 	 if( !(ucDisplayEnableIndex_Temp & (1<<PortToCombo[port])) )
	{
		continue;
	}

      if( gDPInfo[port].ucState ==eSTATE_NORMAL)
        {
        DPRXGetPktStatus();
     }
	 #if (DP_NOVIDEO == 1)
        else if(gDPInfo[port].ucState ==eSTATE_VIDEOMUTE)
        {
           	 DPRXGetAudioPktStatus();
        }
        #endif

	 
        if( gDPInfo[port].ucState !=eSTATE_POWERSAVE)
        {
#if LOOPOUT_DPNOSIGNAL_SOLUTION
            DPRxReChkTiming();					// CCJ 20140528 (invaild Packet)	// 140528 DMD Loop Out¿¡¼­ °£ÇæÀû È­¸é ¾È³ª¿È °³¼±À» À§ÇÑ Spot ¼Ö·ç¼Ç Àû¿ë
#endif
            DPRxCDRLossHander();                         //CCJ 20110321
            if( gDPInfo[port].ucState ==eSTATE_NORMAL)
            DPRxCheckInputFormat();
            DPRxHdcpReAuthen();
        }
#if DP_NOVIDEO
	     DPRxNoVideoCheck();
#endif

#if(ENABLE_STABLE_COUNT)

        if((gDPInfo[port].ucState == eSTATE_NORMAL) && DPRxCheckLock())
        {
#if(COMBO_DP_DAISY_CHAIN_SUPPORT)
			if(port == HBR2_1)
#endif
            DPRxErrorCountHander();
        }
        else
        {
            gDPInfo[port].bDPCheckFlag = 0;
            gDPInfo[port].uwDPStableCount = 0;
        }
#endif

        if(  gDPInfo[port].DPState != gDPInfo[port].ucState )
        {
            DPPrintState( gDPInfo[port].ucState);
        }

       // msWriteByte( REG_11332A+port * HBR2_OFFSET, gDPInfo[port].ucState);

        gDPInfo[port].DPState = gDPInfo[port].ucState;


		//Periodically check VS counter under reasnable value or not.
		if((msRead2Byte(REG_113542 + port*HBR2_OFFSET)& 0x1FFF) > (DP_V_BLANKING_VALID_RANGE/2)) {
			//4K2K@30Hz, reduced blanking as default re-gen timing settings.
			msWriteByteMask(REG_113508 + port*HBR2_OFFSET, BIT1 | BIT0, BIT1 | BIT0 );
			msWrite2Byte(REG_11350C + port*HBR2_OFFSET, 0xF );
		    msWriteByte(REG_113540 + port*HBR2_OFFSET, 0x8);
		    msWriteByte(REG_1135A7 + port*HBR2_OFFSET, 0x10);
		    msWriteByteMask(REG_113501 + port*HBR2_OFFSET, BIT4, BIT4 );    
		   	msWrite2Byte(REG_11350A + port*HBR2_OFFSET, 0x34 );

			msWriteByteMask(REG_113502 + port*HBR2_OFFSET, BIT6, BIT6);
			ForceDelay1ms(1);
		    msWriteByteMask(REG_113502 + port*HBR2_OFFSET, 0x0, BIT6);    
		}

        switch( gDPInfo[port].ucState )
        {

            case eSTATE_STARTUP:
                gDPInfo[port].LockFailCnt = 0;
                gDPInfo[port].PowerSavingCnt = 0;
                gDPInfo[port].ucState = eSTATE_CKECKLOCK;
                break;
            case eSTATE_CKECKLOCK:
                DPRxResetAudioMN();
                DPRxCheckSQTime(1500);
                if( gDPInfo[port].bDPTrainingP0T || gDPInfo[port].bDPTrainingP1T ||( DPRxCheckLock() ) )
                {
                    DP_printMsg( "<Normal> " );
                    if( DPRxCheckLock() )
                    {
                        DP_printMsg( "<Normal> - DP LOCK!" );
                        gDPInfo[port].bDPTrainingP0T = FALSE;    // clear flag
                        gDPInfo[port].bDPTrainingP1T = FALSE;    // clear flag
                        gDPInfo[port].TrainingP1Cnt = 0;               // clear TrainingP1Cnt
                        gDPInfo[port].TrainingP0Cnt = 0;                // clear TrainingP2Cnt
                        gDPInfo[port].LockFailCnt = 0;                    // clear ChkLockCnt
                        gDPInfo[port].PowerSavingCnt = 0;
                        gDPInfo[port].DPCheckModeCnt=0;
                        gDPInfo[port].ucDPNormalModeDelayCnt = 0;
	  		   gDPInfo[port].bDPNormalModeFlag = 0;
                        gDPInfo[port].ucState = eSTATE_MEASURE;
                    }
                    else
                    {
                        DP_printMsg( "<Normal> - DP Un_LOCK!" );
                        Delay1ms( 1 );
                        gDPInfo[port].LockFailCnt++;
                        if( gDPInfo[port].LockFailCnt > 10 ) // ChkLock Fail too much time => HotPlug  worstcase:1msX250= 500ms
                        {
                            gDPInfo[port].LockFailCnt = 0;
                            gDPInfo[port].PowerSavingCnt = 0;
                            gDPInfo[port].bDPTrainingP0T=FALSE;  // clear flag
                            gDPInfo[port].bDPTrainingP1T=FALSE;  // clear flag
                            //if((msReadByte(REG_1F5A)&0x03)!=0x02)
                            //DPRxHPDIRQ();
                        }
                    }
                }
            #if 1
                else if( gDPInfo[port].bSquelchStable && ( !( gDPInfo[port].bDPTrainingP0T || gDPInfo[port].bDPTrainingP1T ) )&&( !DPRxCheckLock() ) )
                {
					#if (COMBO_DP_DAISY_CHAIN_SUPPORT == 0x1)
					if(port == HBR2_0) 
					{
						msDCForceTx1Retrain();
						DPRxClrSQTime();
						continue;
					}
					#endif              
                    DPRxClrSQTime();
			#if ENABLE_DP_ShortHPD_PM		
			if(bHPDShortPulse[port])
			{
				if(DPRXCheckAUXValid())
				{
					DPRxToggleShortHPD(PortToCombo[port]);
					DPRxSet_ShortHPDFlag(PortToCombo[port], FALSE);
				}
			}
			else
			#endif	
                    if(DPFastLink())
                    { 
                    		gDPInfo[port].ucState = eSTATE_MEASURE;
				gDPInfo[port].ucDPNormalModeDelayCnt = 0;
				gDPInfo[port].bDPNormalModeFlag = 0;
                    }	
                }
            #endif
                else
                {
                    gDPInfo[port].PowerSavingCnt++;
                    if( gDPInfo[port].PowerSavingCnt > 5000 ) // ChkLock Fail too much time => HotPlug  worstcase:5000m
                    {
                        gDPInfo[port].PowerSavingCnt = 0;
                        gDPInfo[port].bDPTrainingP0T = FALSE; // clear flag
                        gDPInfo[port].bDPTrainingP1T = FALSE; // clear flag
                    #if 0
                        gDPInfo[port].ucState = eSTATE_POWERSAVE;
                    #endif
                    }
                }
                break;
            case eSTATE_MEASURE:
		  //if(!((DPRxGetMSAFlagCheck())&&(DPRxMSACheckDelay())))
		  if(!DPRxMSACheckDelay())
		  {
		  		break;
		  }	
                if(DPRxCheckTiming(MSACnt))
                {    
                    DPRxSetLearnMode(port);	
                    gDPInfo[port].ucState = eSTATE_NORMAL;
                    gDPInfo[port].NoVscPktCnt = 0;

				    DPRxCheckInterlaceInverse(port); 
					DPRxHVFSMCheck(port);
                }
                #if (DP_NOVIDEO == 1)
                else if(DPRxVideoMuteStateCheck())
                {                   
                    gDPInfo[port].ucState = eSTATE_NORMAL;//eSTATE_VIDEOMUTE;
                    gDPInfo[port].NoVscPktCnt = 0;               
                }
                #endif
                else if(++gDPInfo[port].DPCheckModeCnt > 5)
                {
                    gDPInfo[port].DPCheckModeCnt =0;
                    gDPInfo[port].ucState = eSTATE_CKECKLOCK;
                }
                #if ENABLE_DP_ShortHPD_PM		
                DPRxSet_ShortHPDFlag(PortToCombo[port], FALSE);
                #endif
                break;

            case eSTATE_NORMAL:
                if( !DPRxCheckLock() )
                {
	            #if DPSSC
			DPAudioSSCInit(PortToCombo[port]);
	            #endif

                    HW_SET_AUDIO_MUTE_ENABLE();
                    DPRxClrSQTime();
                    gDPInfo[port].bDPTrainingP0T = FALSE; // clear flag
                    gDPInfo[port].bDPTrainingP1T = FALSE; // clear flag
                    gDPInfo[port].ucState = eSTATE_CKECKLOCK;
                }
                else
                {
                    gDPInfo[port].bAutoEQReset = 1;
                    gDPInfo[port].ucState = eSTATE_NORMAL;
                    DPRxAudioHandle();
                    DPRxTiming();
                    DPRx3DInform();
                    DPRxVPLLHandle();
                    DPRxCheckInputFormat();
                   #if (DP_NOVIDEO == 1)
                    if(DPRxVideoMuteStateCheck())
                    {
                        #if DPSSC
                        DPAudioSSCInit(PortToCombo[port]);
                        #endif
                        gDPInfo[port].ucState = eSTATE_VIDEOMUTE;                      
                    }
                    #endif
                }
                break;
            case eSTATE_POWERSAVE:
                if( !gDPInfo[port].bDPPowerDown )
                {
                    DP_printMsg( " DP ---En for Power Saving" );
                    gDPInfo[port].bDPPowerDown = TRUE;
                    DPRxClearAUXVliadStatus();
                    //msWriteByte( REG_1133E0, msReadByte( REG_1133E0 ) & 0xEF );               //[4]:Enable AUX IRQ
                };
                gDPInfo[port].TrainingP1Cnt = 0;               // clear TrainingP1Cnt
                gDPInfo[port].TrainingP0Cnt = 0;               // clear TrainingP2Cnt
                gDPInfo[port].bDPTrainingP0T = FALSE; // clear flag
                gDPInfo[port].bDPTrainingP1T = FALSE; // clear flag
                gDPInfo[port].ucState = eSTATE_DPIDLE;
                DPRxClrSQTime();
                break;
            case eSTATE_DPIDLE:
                if( gDPInfo[port].bDPAUXVALID )
                {
                    DPRxClearAUXVliadStatus();
                    gDPInfo[port].ucState = eSTATE_STARTUP;                   
                }
                break;
            #if (DP_NOVIDEO)
            case eSTATE_VIDEOMUTE:
                if( !DPRxCheckLock() )
                {
                    #if DPSSC
                    DPAudioSSCInit(PortToCombo[port]);
                    #endif
                    HW_SET_AUDIO_MUTE_ENABLE();
                    DPRxClrSQTime();
                    gDPInfo[port].bDPTrainingP0T = FALSE; // clear flag
                    gDPInfo[port].bDPTrainingP1T = FALSE; // clear flag
                    gDPInfo[port].ucState = eSTATE_CKECKLOCK;                
                    EnableDPMute();
                    DPRxAudioMuteEn =1; 
                }       
                else if(gDPInfo[port].bDPNoVideoStrm == FALSE)
                {
                    gDPInfo[port].ucState = eSTATE_MEASURE;               
                    EnableDPMute();
                    DPRxAudioMuteEn =1;
                    gDPInfo[port].bDPAUPLLBIGChange = TRUE;
                }
                else
                {
                    DPRxVideoMuteStateCheck();
                    gDPInfo[port].bAutoEQReset = 1;
                    gDPInfo[port].ucState = eSTATE_VIDEOMUTE;
                    DPRxAudioHandle();
                }
                break;
            #endif
                
        }

        }

        }
//**************************************************************************
//  [Function Name]:
//                  DPRXCheckAUXValid()
//  [Description]
//                  DPRXCheckAUXValid
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
#if ENABLE_DP_ShortHPD_PM		
BOOL DPRXCheckAUXValid(void)
{
	BYTE xdata i=0;
	BOOL xdata btemp = FALSE;
	
	for(i=0;i<=10;i++)
	{
		if(!((msRegs(REG_0011AC + port *COMBO_PHY_OFFSET))&BIT4))
		{
			btemp = TRUE;
			break;
		}	
		Delay1ms(5);
	}
	
	return btemp;
}
#endif
//**************************************************************************
//  [Function Name]:
//                  DPRxGetMSAFalgCheck()
//  [Description]
//                  DPRxGetMSAFalgCheck
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BOOL  DPRxGetMSAFlagCheck(void)
{
		  	
	DWORD xdata Times= 500;
	
	msWriteByteMask( REG_1132AD+port*HBR2_OFFSET,BIT3,BIT3); 
		
	while( Times-- )
	{
		if(( msReadByte( REG_1132AB + port*HBR2_OFFSET )&BIT3 ) )
		{
			return TRUE;
		}

	} 

	return FALSE;	


}

//**************************************************************************
//  [Function Name]:
//                  DPRxGetMSAFalgCheck()
//  [Description]
//                  DPRxGetMSAFalgCheck
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BOOL  DPRxMSACheckDelay(void)
{
	Bool bflag = FALSE;
		  	
	if(gDPInfo[port].bDPNormalModeFlag == 0 )
	{
		gDPInfo[port].ucDPNormalModeDelayCnt = 100;

		gDPInfo[port].bDPNormalModeFlag = 1;
	}
	if(gDPInfo[port].ucDPNormalModeDelayCnt == 0)
	{
		gDPInfo[port].bDPNormalModeFlag = 0;
		
		bflag = TRUE;
	}
			 	
return bflag;

}




//**************************************************************************
//  [Function Name]:
//                  DPRxVideoMuteStateCheck()
//  [Description]
//                  DPRxVideoMuteStateCheck
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************

#if (DP_NOVIDEO==1)
BOOL DPRxVideoMuteStateCheck( void )
{
    Bool bRtn = FALSE;
    #define bCurNoVideo (gDPInfo[port].bDPNoVideoStrm)
    #define bCurNoAudio (gDPInfo[port].bDPNoAudioStrm)

    if(gDPInfo[port].bDPVBIDChk)
    {
        gDPInfo[port].bDPVBIDChk =0;
        bCurNoVideo = ( msReadByte(REG_1133A5+port*HBR2_OFFSET ) & 0x08 ) ? 1 : 0; // Read BK36 A5 Bit3:1 NoVideoStrm
        bCurNoAudio = ( msReadByte(REG_1133A5+port*HBR2_OFFSET ) & 0x10 ) ? 1 : 0; // Read BK36 A5 Bit4:1 NoAudioStrm

        if( (gDPInfo[port].bDPNoVideoStrm == 1) && (gDPInfo[port].bDPNoAudioStrm == 0) )
        {
            bRtn = TRUE;
        }
        else
        {
            bRtn = FALSE;
        }
    }
    else if(gDPInfo[port].bDPVBIDmsCnt == 0)
    {
      		 gDPInfo[port].bDPVBIDmsCnt = 10;
    }

    #undef bCurNoVideo
    #undef bCurNoAudio

    return bRtn;
 }
#endif



//**************************************************************************
//  [Function Name]:
//                  DPRxReChkTiming()
//  [Description]
//                  DPRxReChkTiming
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
// 140528 DMD Loop Out¿¡¼­ °£ÇæÀû È­¸é ¾È³ª¿È °³¼±À» À§ÇÑ Spot ¼Ö·ç¼Ç Àû¿ë
#if LOOPOUT_DPNOSIGNAL_SOLUTION
void DPRxReChkTiming(void)
{
	if(gDPInfo[port].ucState != eSTATE_NORMAL
    #if (DP_NOVIDEO==1)
        && gDPInfo[port].ucState != eSTATE_VIDEOMUTE
    #endif
    )
	{
		if(DPRxCheckLock()&&!DPRxCheckTiming(5)&& !gDPInfo[port].bReChkEnable)
		{
			 gDPInfo[port].bReChkEnable = 1;
			 gDPInfo[port].uwReChkTiming = 3000;

		}
		if( gDPInfo[port].bReChkEnable && gDPInfo[port].uwReChkTiming ==0)
		{
			gDPInfo[port].bReChkEnable =0;
			if(DPRxCheckLock()&&!DPRxCheckTiming(5))
			{
				DP_printMsg("---------------------------------------==========================Re------check \n");
				DPRxHPDEnable(FALSE, (Combo_IP_SELECT)port);
				ForceDelay1ms(100); // delay 1 msec
				DPRxHPDEnable(TRUE, (Combo_IP_SELECT)port);

			}
		}
	}
}
#endif

//**************************************************************************
//  [Function Name]:
//                  DPRxHdcpReAuthen()
//  [Description]
//                  DPRxHdcpReAuthen
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void DPRXGetPktStatus()
{
    if(gDPInfo[port].bPacketTimer==0)
    {
        gDPInfo[port].InputPacketStatus = msRead2Byte(REG_1132AA+port*HBR2_OFFSET);
        msWrite2Byte(  REG_1132AC+port*HBR2_OFFSET, 0xFFFF );
        msWrite2Byte(  REG_1132AC+port*HBR2_OFFSET, 0x00 );
        gDPInfo[port].bPacketTimer = 100;
    }
}

#if (DP_NOVIDEO == 1)
void DPRXGetAudioPktStatus(void)
{
	if(gDPInfo[port].bPacketTimer==0)
	{
		gDPInfo[port].InputPacketStatus = msRead2Byte(REG_1132AA+port*HBR2_OFFSET);
		msWrite2Byte(  REG_1132AC+port*HBR2_OFFSET, 0x60 );
		msWrite2Byte(  REG_1132AC+port*HBR2_OFFSET, 0x00 );
		gDPInfo[port].bPacketTimer = 100;
	}
}
#endif

//**************************************************************************
//  [Function Name]:
//                  DPRxHdcpReAuthen()
//  [Description]
//                  DPRxHdcpReAuthen
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void DPRxHdcpReAuthen(void)
{

      if(((msReadByte(REG_113310+port*HBR2_OFFSET )&0x03)==0) && DPRxCheckLock()&&(msReadByte(REG_113374+port*HBR2_OFFSET)&BIT2))
      {
                if(gDPInfo[port].bDPHDCPIRQ ==0)
               {
                    gDPInfo[port].uwHDCPCnt = 1000;
                    gDPInfo[port].bDPHDCPIRQ = 1;
               }

               if(gDPInfo[port].bDPHDCPIRQ && ( gDPInfo[port].uwHDCPCnt==0))
            {
                    DPRxHPDIRQ();
                    gDPInfo[port].bDPHDCPIRQ=0;
                }
       }
      else
      {
               gDPInfo[port].bDPHDCPIRQ = 0;

      }
}

//**************************************************************************
//  [Function Name]:
//                  DPRxCDRLossHander()
//  [Description]
//                  DPRxCDRLossHander
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void DPRxCDRLossHander()
{
    if( gDPInfo[port].bDPLoseCDRLockIRQ ) // Lock fail have occured
    {
      if( !DPRxCheckLock() )
        {
            if(( gDPInfo[port].uwCDRHPDCnt == 0 ) && ((msReadByte(REG_113310+port*HBR2_OFFSET)&0x03) == 0 ) )
            {
                if( (msReadByte( REG_11335A+port*HBR2_OFFSET )&0x03)!=0x02)   //CCJ 20110727
                {
                    msRegs(REG_113000+port*HBR2_OFFSET) = msRegs(REG_113000+port*HBR2_OFFSET)|BIT1;
               	 msRegs(REG_113000+port*HBR2_OFFSET) = msRegs(REG_113000+port*HBR2_OFFSET)&(~BIT1);
                    msWriteByte( REG_11331C+port*HBR2_OFFSET, 0x00 );
                    msWriteByte( REG_11331C+port*HBR2_OFFSET, 0x80 );
                    DP_printMsg("CDR_Loss");
                    DPRxHPDIRQ();                             //CCJ mark 20090925
                }
                gDPInfo[port].bDPLoseCDRLockIRQ = FALSE;    //clear flag

            }
        }
        else
        {
            gDPInfo[port].bDPLoseCDRLockIRQ = FALSE;         //clear flag
        }

    }
    if(gDPInfo[port].bDPInterlaneSkewIRQ ) // Lock fail have occured
    {
        if( DPRxCheckLock() )
        {
            if(( gDPInfo[port].uwSkewHPDCnt == 0 ) && ((msReadByte(REG_113310+port*HBR2_OFFSET)&0x03)== 0 ) )
            {
                if( !( msReadByte( REG_11331C+port*HBR2_OFFSET )&_BIT0 ) ) // now is not in traning stage
                {
                    if( (msReadByte( REG_11335A+port*HBR2_OFFSET )&0x03)!=0x02)    //CCJ 20110727
                    {
                        msWriteByte( REG_11331C+port*HBR2_OFFSET, 0x00 );
                        msWriteByte( REG_11331C+port*HBR2_OFFSET, 0x80 );
                        DP_printMsg("Interlane_Loss");
                        DPRxHPDIRQ();                                   //CCJ mark 20090925
                    }

                    gDPInfo[port].bDPInterlaneSkewIRQ = FALSE; //clear flag
                }
                else
                {
                    gDPInfo[port].bDPInterlaneSkewIRQ = FALSE; //clear flag

                }
            }
        }
        else
        {
            gDPInfo[port].bDPInterlaneSkewIRQ = FALSE;      //clear flag

        }
    }
}

#if(ENABLE_STABLE_COUNT)
//**************************************************************************
//  [Function Name]:
//                  DPRxErrorCountHander()
//  [Description]
//                  DPRxErrorCountHander
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void DPRxErrorCountHander()
{
    BYTE error_flag = 0;
    WORD error_L0 = 0,error_L1 = 0,error_L2 = 0,error_L3 = 0;

    /*
    3. ³]©werror cnt
        ¼g0x113006 [15:8] = 8'h00        //error_cnt_mask_thd
        ¼g0x113000 [11] = 1'b1              //error_cnt_sel
        ¼g 0x113000 [10] = 1'b1          //error_cnt_en
        ¼g 0x113000 [14] = 1'b1          //eq_ctrl_reserved  ³o­Óreg¦b MST9U U02¤~¦³¥Î

    4. ¶¡¹j¤@¬q®É¶¡(»Ý¹êÅç¨M©w), Åª¨úerror cnt¥H§PÂ_¬O§_stable
        Åª 0x113033 [15:0]        // lane0 error cnt
        Åª 0x113034 [15:0]        // lane1 error cnt
        Åª 0x113035 [15:0]        // lane2 error cnt
        Åª 0x113036 [15:0]        // lane3 error cnt
    */

    if(gDPInfo[port].uwDPStableCount == 0)
    {
        if(gDPInfo[port].bDPCheckFlag == 0x01)
        {
            gDPInfo[port].bDPCheckFlag = 0x00;

            switch( msReadByte(REG_11330F + port*HBR2_OFFSET) & 0x07 )
            {
                case 0x01:  // 1 lane

                    error_L0 = (msReadByte(REG_113067 + port*HBR2_OFFSET) << 8) | msReadByte(REG_113066 + port*HBR2_OFFSET);

                    if((error_L0 >= ERROR_THRESHOLD) && (error_L0 != 0xFFFF))
                    {
                        error_flag = 1;
                    }

                break;

                case 0x02:  // 2 lanes

                    error_L0 = (msReadByte(REG_113067 + port*HBR2_OFFSET) << 8) | msReadByte(REG_113066 + port*HBR2_OFFSET);
                    error_L1 = (msReadByte(REG_113069 + port*HBR2_OFFSET) << 8) | msReadByte(REG_113068 + port*HBR2_OFFSET);

                    if(((error_L0 >= ERROR_THRESHOLD) && (error_L0 != 0xFFFF)) || ((error_L1 >= ERROR_THRESHOLD) && (error_L1 != 0xFFFF)))
                    {
                        error_flag = 1;
                    }

                break;

                case 0x04:  // 4 lanes

                    error_L0 = (msReadByte(REG_113067 + port*HBR2_OFFSET) << 8) | msReadByte(REG_113066 + port*HBR2_OFFSET);
                    error_L1 = (msReadByte(REG_113069 + port*HBR2_OFFSET) << 8) | msReadByte(REG_113068 + port*HBR2_OFFSET);
                    error_L2 = (msReadByte(REG_11306B + port*HBR2_OFFSET) << 8) | msReadByte(REG_11306A + port*HBR2_OFFSET);
                    error_L3 = (msReadByte(REG_11306D + port*HBR2_OFFSET) << 8) | msReadByte(REG_11306C + port*HBR2_OFFSET);

                    if(((error_L0 >= ERROR_THRESHOLD) && (error_L0 != 0xFFFF)) || ((error_L1 >= ERROR_THRESHOLD) && (error_L1 != 0xFFFF)) || ((error_L2 >= ERROR_THRESHOLD) && (error_L2 != 0xFFFF)) || ((error_L3 >= ERROR_THRESHOLD) && (error_L3 != 0xFFFF)))
                    {
                       error_flag = 1;
                    }

                break;

                default:

                break;
            }

            if(error_flag == 0x01)
            {
                msWriteByteMask(REG_1135C1 + port*HBR2_OFFSET, BIT3, BIT3|BIT2);
#if(AUTO_EQ)
				DPRXAutoEQOnOff(DISABLE,(HBR2_SELECT) port);
#endif
                DPRxHPDEnable(FALSE, (Combo_IP_SELECT)port);

                ForceDelay1ms(1); // delay 1 msec

#if(AUTO_EQ)
				DPRXAutoEQOnOff(ENABLE,(HBR2_SELECT) port);
#endif
                DPRxHPDEnable(TRUE, (Combo_IP_SELECT)port);

                gDPInfo[port].bDPDownSpeed = 1;
                gDPInfo[port].bDPLinkRate = msReadByte(REG_11330E + port*HBR2_OFFSET);

                error_flag = 0;
            }

            msWriteByteMask(REG_113001 + port*HBR2_OFFSET, 0, BIT6|BIT3|BIT2);

            gDPInfo[port].uwDPStableCount = 20; // 20ms
        }
        else
        {
            gDPInfo[port].bDPCheckFlag = 1;

			msWriteByteMask(REG_113001 + port*HBR2_OFFSET, 0, BIT6);
			msWriteByteMask(REG_113001 + port*HBR2_OFFSET, BIT3|BIT2, BIT3|BIT2);
			msWriteByteMask(REG_113001 + port*HBR2_OFFSET, BIT6, BIT6);

            gDPInfo[port].uwDPStableCount = 500; // 500ms
        }
    }

}
#endif

//**************************************************************************
//  [Function Name]:
//                  DPComboPort()
//  [Description]
//                  DPComboPort
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void DPRxComboPort()
{
    BYTE K;

        ucDisplayEnableIndex_Temp = ucDisplayEnableIndex_Temp & ucDPEnableIndex_Temp;

    if(DPCombo_Update)  // do One time
    {
            if(g_bDoDPInit )
            {
               for(K=0;K<6;K+=1)
                 {
                      if(DP_Connected[K] ==0)
                      continue;
                  	DPRxHPDEnable(TRUE, (Combo_IP_SELECT)K);
				
			#if ENABLE_DAISY_CHAIN
                    {         
	                     if( (ucDPDaisyChainFlag & (0x01 << K)) == (0x01 << K))
	                     {
							if(g_eBootStatus != eBOOT_STATUS_DCOFF) {
		        				msDCInit_DPRxHPD();
							}
						
	        				if( g_eBootStatus == eBOOT_STATUS_DCON
	                                    || g_eBootStatus == eBOOT_STATUS_DPMSON)
	        		       {
	        		    	    msDCForceTx1Retrain();
	        				}
	                     }
                     }
			#endif
				  
                 }
             g_bDoDPInit = 0;
            }

        if(ucDisplayEnableIndex_Temp & BIT0)    // Combo 0
        {


                //DP_printMsg("===COMBO 0");
        }
        if(ucDisplayEnableIndex_Temp & BIT1)    // Combo 1
        {

                //DP_printMsg("===COMBO 1");

        }
        if(ucDisplayEnableIndex_Temp & BIT2)    // Combo 2
        {

            //DP_printMsg("===COMBO 2");

        }
        if(ucDisplayEnableIndex_Temp & BIT3)    // Combo 3
        {

            //DP_printMsg("===COMBO 3");

        }
        if(ucDisplayEnableIndex_Temp & BIT4)    // Combo 4
        {

            //DP_printMsg("===COMBO 4");

        }
        if(ucDisplayEnableIndex_Temp & BIT5)    // Combo 5
        {

            //DP_printMsg("===COMBO 5");

        }

        DPCombo_Update = 0;
    }
    else   // do many times
    {
            #if 1
            if(ucDisplayEnableIndex_Temp & BIT0)    // Combo 0
            {
                #if (DP_Port_D0)
                if(mapi_DP_GetCableDetectPort(Combo_PORT0) == COMBO_IP_CABLE_NONE)
                {
                    //msWriteByteMask( REG_11349B+HBRPort(Combo_PORT0) * HBR2_OFFSET , BIT7, BIT7);
                    //msWriteByteMask( REG_11314A+HBRPort(Combo_PORT0) * HBR2_OFFSET , 0, BIT0);
                    //msWriteByteMask( REG_11349B+HBRPort(Combo_PORT0) * HBR2_OFFSET , 0, BIT7);
                    //DP_printMsg("===Combo 0 Plug out");
                }
                #endif
            }
            if(ucDisplayEnableIndex_Temp & BIT1)    // Combo 1
            {
                #if (DP_Port_D1)
                if(mapi_DP_GetCableDetectPort(Combo_PORT1) == COMBO_IP_CABLE_NONE)
                {

                    //DP_printMsg("===Combo 1 Plug out");
                }
                #endif
            }
            if(ucDisplayEnableIndex_Temp & BIT2)    // Combo 2
            {

                #if (DP_Port_D2)
                if(mapi_DP_GetCableDetectPort(Combo_PORT2) == COMBO_IP_CABLE_NONE)
                {

                    //DP_printMsg("===Combo 2 Plug out");
                }
                #endif

            }
            if(ucDisplayEnableIndex_Temp & BIT3)    // Combo 3
            {

                #if (DP_Port_D3)
                if(mapi_DP_GetCableDetectPort(Combo_PORT3) == COMBO_IP_CABLE_NONE)
                {

                    //DP_printMsg("===Combo 3 Plug out");
                }
                #endif

            }
            if(ucDisplayEnableIndex_Temp & BIT4)    // Combo 4
            {

                #if (DP_Port_D4)
                if(mapi_DP_GetCableDetectPort(Combo_PORT4) == COMBO_IP_CABLE_NONE)
                {

                    //DP_printMsg("===Combo 4 Plug out");
                }
                #endif

            }
            if(ucDisplayEnableIndex_Temp & BIT5)    // Combo 5
            {

                #if (DP_Port_D5)
                if(mapi_DP_GetCableDetectPort(Combo_PORT5) == COMBO_IP_CABLE_NONE)
                {

                    //DP_printMsg("===Combo 5 Plug out");
                }
                #endif

            }
        #endif



    }


    if(gDPInfo[port].bDPGUID)
    {
            DPRxGUIDInit(PortToCombo[port], GUIDData);
            gDPInfo[port].bDPGUID =0;
    }


}
//**************************************************************************
//  [Function Name]:
//                  DPRxCheckLock()
//  [Description]
//                  DPRxCheckLock
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BOOL DPRxCheckLock( )
{
    BYTE xdata ucLans = 0;
    BOOL xdata bStstus = FALSE;
    BYTE xdata ucStatus0 = 0;
    BYTE xdata ucStatus1 = 0;

#if 0
    DP_printData( " 10=%x", msReadByte( REG_1F10 ) );
    DP_printData( " A8=%x", msReadByte( REG_36A8 ) );
    DP_printData( " 8E=%x", msReadByte( REG_368E ) );
#endif

    gDPInfo[port].bDPLANELOCK = FALSE;
    gDPInfo[port].bDPCDRLOCK = FALSE;
    ucLans = msReadByte( REG_11330F+port*HBR2_OFFSET ) & 0x07;
    ucStatus0 = msReadByte( REG_11331A+port*HBR2_OFFSET );
    ucStatus1 = msReadByte( REG_11331B+port*HBR2_OFFSET );

    switch( ucLans )
    {
        case 0x01:
            if( (ucStatus0 & 0x07) == 0x07 )
            {
                bStstus = TRUE;
                gDPInfo[port].bDPLANELOCK = TRUE;
                gDPInfo[port].bDPCDRLOCK = TRUE;

            }
            break;
        case 0x02:
            if( ucStatus0 == 0x77 )
            {
                bStstus = TRUE;
                gDPInfo[port].bDPLANELOCK = TRUE;
                gDPInfo[port].bDPCDRLOCK = TRUE;
            }
            break;
        case 0x04:
            if(( ucStatus0 == 0x77 ) && ( ucStatus1 == 0x77 ) )
            {
                bStstus = TRUE;
                gDPInfo[port].bDPLANELOCK = TRUE;
                gDPInfo[port].bDPCDRLOCK = TRUE;
            }
            break;
        default:
            gDPInfo[port].bDPLANELOCK = FALSE;
            gDPInfo[port].bDPCDRLOCK = FALSE;
            break;
    }

    if( !( msReadByte( REG_11331C+port*HBR2_OFFSET )&_BIT0 ) ) // lost alignment
    {
        gDPInfo[port].bDPLANELOCK = FALSE;
        //   bStstus = FALSE;                          //CCJ
    }
#if 0
    DP_printData( " 1C=%x", msReadByte( REG_1F1C ) );
    DP_printData( " 1A=%x", msReadByte( REG_1F1A ) );
#endif

    gDPInfo[port].bDPLock = gDPInfo[port].bDPCDRLOCK;

#if 0//DP_CheckSquelch
    if(bStstus == TRUE)
    u8EnableCheckSquelchCnt = 0;
#endif

    return bStstus;
}

//**************************************************************************
//  [Function Name]:
//                  DPPrintState(BYTE state)
//  [Description]
//                  DPPrintState
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void DPPrintState( BYTE state )
{
    switch( state )
    {
        case eSTATE_STARTUP:
            DP_printMsg( "##  eSTATE_STARTUP! ##" );
            break;
        case eSTATE_CKECKLOCK:
            DP_printMsg( "## eSTATE_CKECKLOCK! ##" );
            break;
        case eSTATE_MEASURE:
            DP_printMsg( "## eSTATE_MEASURE!##" );
            break;
        case eSTATE_NORMAL:
            DP_printMsg( "## eSTATE_NORMAL! ##" );
            break;
        case  eSTATE_POWERSAVE:
            DP_printMsg( "## eSTATE_POWERSAVE! ##" );
            break;
        case eSTATE_DPIDLE:
            DP_printMsg( "## eSTATE_DPIDLE! ##" );
            break;
         #if (DP_NOVIDEO == 1)
         case eSTATE_VIDEOMUTE:
            DP_printMsg( "## eSTATE_VIDEOMUTE! ##" );
            break;   
         #endif   
    }
}

//**************************************************************************
//  [Function Name]:
//                  DPRxCheckTiming()
//  [Description]
//                  DPRxCheckTiming
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BOOL DPRxCheckTiming( BYTE Count)
{
    Bool found = FALSE;
    WORD xdata LS_Clock=0;
    WORD xdata Pixel_Clock=0;
    DWORD xdata Base_N=0;
    DWORD xdata Base_M=0;

    while(Count--)
    {

        LS_Clock = 27 * msReadByte(REG_11330E+port*HBR2_OFFSET);
        DP_printData( "DP Input : LS_Clock %d ", LS_Clock );
        Base_M = ((DWORD) msRead2Byte( REG_113387+port*HBR2_OFFSET ) <<8) + msReadByte(REG_113386+port*HBR2_OFFSET);
        DP_printData( "DP Input : Base_M %x ", Base_M );
        Base_N = ((DWORD)msRead2Byte( REG_11338B+port*HBR2_OFFSET ) <<8) + msReadByte(REG_11338A+port*HBR2_OFFSET);
        DP_printData( "DP Input : Base_N %x ", Base_N );
        if(Base_N == 0)
        {
            Base_N = 1;
            DP_printData( "DP Input : Base_N Overwrite from 0 to 1, %x ", Base_N ); // prevent /0 exception, due to temporal unstable Base_N
        }
        Pixel_Clock = ((DWORD) Base_M * LS_Clock ) / Base_N ;
        DP_printData( "DP Input : Pixel_Clock %d Mhz", Pixel_Clock );

        gDPInfo[port].bDPInterlace = ( msReadByte( REG_1133A5+port*HBR2_OFFSET ) & 0x04 ) ? TRUE : FALSE; // Read BK36 A5 Bit2 Interlance mode

        gDPInfo[port].uwDPHtotal = msRead2Byte( REG_113376+port*HBR2_OFFSET) & 0x7FFF;

        if( gDPInfo[port].bDPInterlace )
            gDPInfo[port].uwDPVtotal = ( msRead2Byte( REG_113378+port*HBR2_OFFSET ) & 0x7FFF ) << 1;
        else
            gDPInfo[port].uwDPVtotal = msRead2Byte( REG_113378+port*HBR2_OFFSET ) & 0x7FFF;

        gDPInfo[port].uwDPHWidth = msRead2Byte(REG_11337A+port*HBR2_OFFSET ) & 0x7FFF;

        if( gDPInfo[port].bDPInterlace )
        {
            gDPInfo[port].uwDPVWidth = ( msRead2Byte( REG_11337C+port*HBR2_OFFSET ) & 0x7FFF ) << 1;
        }
        else
        {
            gDPInfo[port].uwDPVWidth = msRead2Byte( REG_11337C+port*HBR2_OFFSET ) & 0x7FFF;
            gDPInfo[port].uwDPHStart = msRead2Byte( REG_11337E+port*HBR2_OFFSET ) & 0x7FFF;
            gDPInfo[port].uwDPVStart = msRead2Byte( REG_113380+port*HBR2_OFFSET ) & 0x7FFF;
            gDPInfo[port].uwDPHPWS = msRead2Byte( REG_113382+port*HBR2_OFFSET ) & 0x7FFF;
            gDPInfo[port].bDPHPol = ( msReadByte( REG_113383+port*HBR2_OFFSET ) & 0x80 ) ? 1 : 0;
            gDPInfo[port].uwDPVPWS = msRead2Byte( REG_113384+port*HBR2_OFFSET ) & 0x7FFF;
            gDPInfo[port].bDPVPol = ( msReadByte( REG_113385+port*HBR2_OFFSET ) & 0x80 ) ? 1 : 0;
        }
        DP_printData( "DP Input : HDE  %d ", gDPInfo[port].uwDPHWidth );
        DP_printData( "DP Input : VDE  %d ", gDPInfo[port].uwDPVWidth );
        //Pixel_Clock = 100;
        if(( gDPInfo[port].uwDPHWidth < 4500 ) && ( gDPInfo[port].uwDPVWidth < 4500 ) && ( Pixel_Clock > 10) && ( Pixel_Clock < 700)
        &&( gDPInfo[port].uwDPHtotal > gDPInfo[port].uwDPHWidth) && ( gDPInfo[port].uwDPVtotal > gDPInfo[port].uwDPVWidth))
        {
            found = TRUE;
            DP_printMsg( "===> DP timing Pass:" );
        }
        else
        {
            DP_printMsg( "===> DP timing Fail:" );
            found = FALSE;
            break;
        }

    }
    return found;
}

//**************************************************************************
//  [Function Name]:
//                  DPRxTiming()
//  [Description]
//                  DPRxTiming
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void DPRxTiming( )
{
    WORD xdata Blanking = 0, H_Blanking = 0;
    WORD xdata Width, Front_Porch = 0, Back_Porch = 0;
    WORD xdata H_Width = 0;

    WORD xdata LS_Clock=0;
    DWORD xdata Base_N=0;
    DWORD xdata Base_M=0;
    DWORD temp=0, temp1=0;
    BYTE xdata Cnt=5;
    BYTE xdata CheckCnt=0;	
    
    if(!(ucDisplayEnableIndex_Temp & (1<< PortToCombo[port])))
    {
        //DP_printMsg("===Enable 1P mode ");
        //DPRxEnSplit(DP_1P);
        //msDrvSrcSetHBR2FlagMux(port+1, FALSE,TYPE_HBR2_0|TYPE_DP);
        //gDPInfo[port].bDPSplitOnOff =0;
        //gDPInfo[port].bDPSplitEn = 0;
        //gDPInfo[port].bDPSplitEn_temp=0;
        return;
    }
   if((!(msReadByte(REG_113584+port*HBR2_OFFSET)&BIT2))&&(msRead2Byte(REG_113586+port*HBR2_OFFSET)<400))		
   DPRxSeMSAMode(port);	

    LS_Clock = 27 * msReadByte(REG_11330E+port*HBR2_OFFSET);
    //DP_printData( "DP Input : LS_Clock %d ", LS_Clock );
    Base_M = ((DWORD) msRead2Byte( REG_113387+port*HBR2_OFFSET ) <<8) + msReadByte(REG_113386+port*HBR2_OFFSET);
    //DP_printData( "DP Input : Base_M %x ", Base_M );
    Base_N = ((DWORD)msRead2Byte( REG_11338B+port*HBR2_OFFSET ) <<8) + msReadByte(REG_11338A+port*HBR2_OFFSET);
    //DP_printData( "DP Input : Base_N %x ", Base_N );
    if(Base_N == 0)
    {
        Base_N = 1;
        //DP_printData( "DP Input : Base_N Overwrite from 0 to 1, %x ", Base_N ); // prevent /0 exception, due to temporal unstable Base_N
    }
    gDPInfo[port].uwDPPixel = ((DWORD) Base_M * LS_Clock ) / Base_N;

 
    if((gDPInfo[port].uwDPPixel  > 330 )&&(gDPInfo[port].bDPSplitOnOff ==0))
    {
    	   if(	gDPInfo[port].bDPSplitEn_Cnt_Up  ==0xFF)
    	   {
          	gDPInfo[port].bDPSplitEn_Cnt_Up =50;

          	gDPInfo[port].bDPSplitEn_Cnt_Down =0xFF;
    	   }
    	   
        if(gDPInfo[port].bDPSplitEn_Cnt_Up ==0)
        {
           gDPInfo[port].bDPSplitOnOff =1;  	
        }   
    }
    else if((gDPInfo[port].uwDPPixel  <= 330 )&&(gDPInfo[port].bDPSplitOnOff ==1))
    {

    	if(gDPInfo[port].bDPSplitEn_Cnt_Down ==0xFF)
    	  {	
        	gDPInfo[port].bDPSplitEn_Cnt_Up =0xFF;

        	gDPInfo[port].bDPSplitEn_Cnt_Down=50;
    	  }
    	
        if(gDPInfo[port].bDPSplitEn_Cnt_Down ==0)
        {
            gDPInfo[port].bDPSplitOnOff =0;	
        }
    }


    //DP_printData( "Down %x ", gDPInfo[port].bDPSplitEn_Cnt_Down  );
    //DP_printData( "Up     %x ", gDPInfo[port].bDPSplitEn_Cnt_Up  );
    //DP_printData( "OnOff  %x ", gDPInfo[port].bDPSplitOnOff  );

   

    if(gDPInfo[port].bDPSplitOnOff)
    {
        DPRxEnSplit(DP_2P, (HBR2_SELECT)port);
        gDPInfo[port].bDPSplitEn = 1;
    }
    else
    {
        DPRxEnSplit(DP_1P,(HBR2_SELECT)port);
        gDPInfo[port].bDPSplitEn = 0;
    }
#if 0
    if((gDPInfo[port].bDPVPLLBIGChange)||(gDPInfo[port].bDPSplitEn_temp != gDPInfo[port].bDPSplitEn))
    {
        if(gDPInfo[port].bDPSplitEn)
        {
            DP_printMsg("===Enable 2P mode ");
            if(port ==0)
                msDrvSrcSetHBR2FlagMux( HBR2ToScanPort[port], TRUE,TYPE_HBR2_0|TYPE_DP);
            else
                msDrvSrcSetHBR2FlagMux( HBR2ToScanPort[port], TRUE,TYPE_HBR2_1|TYPE_DP);
        }
        else
        {
            DP_printMsg("===Enable 1P mode ");
            if(port ==0)
                msDrvSrcSetHBR2FlagMux(HBR2ToScanPort[port], FALSE,TYPE_HBR2_0|TYPE_DP);
            else
                msDrvSrcSetHBR2FlagMux( HBR2ToScanPort[port], FALSE,TYPE_HBR2_1|TYPE_DP);
        }
    }
#endif
    gDPInfo[port].bDPSplitEn_temp = gDPInfo[port].bDPSplitEn;

#if 0  //COMBO
    gDPInfo[port].uwDPVtotal  = msRead2Byte( REG_1603B6 ) & 0x7FFF;
    gDPInfo[port].uwDPVWidth = msRead2Byte( REG_1603BE) & 0x7FFF;
    msWriteByteMask( REG_160210, BIT1 | BIT0, BIT1 | BIT0 );
    Blanking = gDPInfo[port].uwDPVtotal - gDPInfo[port].uwDPVWidth;
    Width =  Blanking / 2;
    Front_Porch = ( Blanking - Width ) / 2;
    Back_Porch = Blanking - Width - Front_Porch ;
    msWrite2Byte( REG_160214, Width );
    msWriteByte( REG_16020B, Front_Porch );
    msWriteByte( REG_16020E, ( Back_Porch ));
    msWrite2Byte( REG_160212, msRead2Byte( REG_160212 ) );
#else

    //if(gDPInfo[port].uwDPVWidth < 2000)
    {
        gDPInfo[port].bDPInterlace = ( msReadByte( REG_1133A5 +port*HBR2_OFFSET) & 0x04 ) ? TRUE : FALSE;

        if( gDPInfo[port].bDPInterlace )
        {
        	gDPInfo[port].uwDPVtotal = ( msRead2Byte( REG_113378+port*HBR2_OFFSET ) & 0x7FFF ) << 1;
        	gDPInfo[port].uwDPVWidth = ( msRead2Byte( REG_11337C+port*HBR2_OFFSET ) & 0x7FFF ) << 1;
        }
        else
       {
       	gDPInfo[port].uwDPVtotal = msRead2Byte( REG_113378+port*HBR2_OFFSET ) & 0x7FFF;
       	gDPInfo[port].uwDPVWidth = msRead2Byte( REG_11337C+port*HBR2_OFFSET ) & 0x7FFF;
        }

        gDPInfo[port].uwDPHtotal = msRead2Byte( REG_113376+port*HBR2_OFFSET ) & 0x7FFF;
        gDPInfo[port].uwDPHWidth = msRead2Byte( REG_11337A+port*HBR2_OFFSET ) & 0x7FFF;

       
        Blanking = gDPInfo[port].uwDPVtotal - gDPInfo[port].uwDPVWidth;

	  	 if( gDPInfo[port].bDPInterlace )
		 Blanking = Blanking >> 1 ;

		 if(Blanking < DP_V_BLANKING_VALID_RANGE) {
			
			msWrite2Byte( REG_11350A+port*HBR2_OFFSET, msRead2Byte( REG_11350A+port*HBR2_OFFSET )&0x7FFF );
			msWriteByteMask( REG_113508+port*HBR2_OFFSET, BIT1 | BIT0, BIT1 | BIT0 );
			
	        Width =  Blanking / 2;
		 	Front_Porch = ( Blanking - Width ) / 2;

		 	if (Front_Porch > 255)   // Fix V front too big
	        {
	            Front_Porch = 255;
	        }

	        Back_Porch = Blanking - Width - Front_Porch ;

	        if(Back_Porch > 127 )   // Fix V blacking too big
	        {
			Back_Porch = 127;

			Width = Blanking - Back_Porch -Front_Porch;
	        }

	        msWrite2Byte( REG_11350C+port*HBR2_OFFSET, (Width&0x7FFF) );
	        msWriteByte( REG_113540+port*HBR2_OFFSET, Front_Porch );
	        msWriteByte( REG_1135A7+port*HBR2_OFFSET, ( Back_Porch ) << 1 );
	        msWriteByteMask( REG_113501+port*HBR2_OFFSET, BIT4, BIT4 );

	        H_Blanking = gDPInfo[port].uwDPHtotal - gDPInfo[port].uwDPHWidth;
	        H_Width =  H_Blanking / 3;
	         if((gDPInfo[port].uwDPHWidth > 3000)||(gDPInfo[port].uwDPVWidth > 3000)||(gDPInfo[port].uwDPPixel >300))
	        {
	        //H_Width =  (gDPInfo[port].uwDPPixel /10) + 1;
	        //H_Width = (H_Width >= 0x34) ? H_Width : 0x34 ;
	        msWrite2Byte( REG_11350A+port*HBR2_OFFSET, 0x34 );
	        }
	        else
	        msWrite2Byte( REG_11350A+port*HBR2_OFFSET, 0x2C );
	 	}

    }
#endif

 ///=================================
// 	Interlace patch
//===================================


	
	 if(gDPInfo[port].bDPInterlaceFiledToggleFlag)
	 {
		  temp = ((msReadByte( REG_113536+port*HBR2_OFFSET )&0x0F) <<16)  + msRead2Byte( REG_113534+port*HBR2_OFFSET );
		  temp1 = ((msReadByte( REG_11353A+port*HBR2_OFFSET )&0x0F) <<16)  + msRead2Byte( REG_113538+port*HBR2_OFFSET );
		//  DP_printData( "=1=>abs(temp-temp1)= %x", abs(temp-temp1) );

		  if(abs(temp-temp1) >20 )
		  {
			  if(!(msReadByte( REG_113541+port*HBR2_OFFSET)&BIT2))
			  {
				  if(temp1 < temp )
					  msWriteByteMask( REG_113541+port*HBR2_OFFSET, BIT2, BIT2 );
			  }
			  else
			  {
				  if(temp1 < temp )
				  msWriteByteMask( REG_113541+port*HBR2_OFFSET, 0, BIT2 );
			  }
		  }
		  else
			  msWriteByteMask( REG_113541+port*HBR2_OFFSET, 0, BIT2 );
	   }
	   else
	   {
		   msWriteByteMask( REG_113541+port*HBR2_OFFSET, 0, BIT2 );
	   }

	//==============================================================////20160412
	
	if((gDPInfo[port].uwDPVtotal_Temp !=0)&&(gDPInfo[port].uwDPHtotal_Temp !=0))
	{
		while(Cnt--)
		{
			if((abs(gDPInfo[port].uwDPHtotal - gDPInfo[port].uwDPHtotal_Temp) >0x100)
			    ||((abs(gDPInfo[port].uwDPVtotal - gDPInfo[port].uwDPVtotal_Temp) >0x100)))	
			{
				CheckCnt++;				
			}			
		}
	}
	gDPInfo[port].uwDPVtotal_Temp =gDPInfo[port].uwDPVtotal;
	gDPInfo[port].uwDPHtotal_Temp =gDPInfo[port].uwDPHtotal;

	if(CheckCnt > 2)
	{	
		//printMsg("--------HTOTAL AND VTT Chang     ");
		IsrModeChangeCombo((port+1));    ////20160412
	}
  //================================================================////20160412

/*

     if(gDPInfo[port].bDPInterlace)
     {
	  	 DWORD temp=0, temp1=0;
		temp = ((msReadByte( REG_113536+port*HBR2_OFFSET )&0x0F) <<16)  + msRead2Byte( REG_113534+port*HBR2_OFFSET );
		temp1 = ((msReadByte( REG_11353A+port*HBR2_OFFSET )&0x0F) <<16)  + msRead2Byte( REG_113538+port*HBR2_OFFSET );

		if(abs(temp-temp1) >20 )
		{

			if(!(msReadByte( REG_113541+port*HBR2_OFFSET)&BIT2))
			{
				if(temp1 < temp )
				msWriteByteMask( REG_113541+port*HBR2_OFFSET, BIT2, BIT2 );
			}
			else
			{
				if(temp1 < temp )
				msWriteByteMask( REG_113541+port*HBR2_OFFSET, 0, BIT2 );
			}
		}
		else
			msWriteByteMask( REG_113541+port*HBR2_OFFSET, 0, BIT2 );
     }
     else
     {
     		msWriteByteMask( REG_113541+port*HBR2_OFFSET, 0, BIT2 );
     }
*/     
}

//**************************************************************************
//  [Function Name]:
//                  DPRx3DInform()
//  [Description]
//                  DPRx3DInform
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
//    SC_FMT_IN_NORMAL,
//    SC_FMT_IN_3D_PF,        /* page flipping */
//    SC_FMT_IN_3D_FP,        /* frame packing */
//    SC_FMT_IN_3D_FPI,       /* frame packing interlace */
//    SC_FMT_IN_3D_TB,        /* top bottom */
//    SC_FMT_IN_3D_SBSH,      /* side by side half */
//    SC_FMT_IN_3D_LA,        /* line interleave */
//    SC_FMT_IN_3D_SBSF,      /* side by side full */
//    SC_FMT_IN_3D_CB,        /* chess board */

void DPRx3DInform( )
{
    //BYTE ucSCFmtIn = SC_FMT_IN_NORMAL;

    gDPInfo[port].DP_3DMode = E_DP_3D_INPUT_NO_STEREO;

#if 0//Displayport12

    NoVscPktCnt++;

    msWriteByteMask( REG_1FBC, BIT6, BIT6 );

    if( msReadByte( REG_1F59) & BIT1 )
    {
        gDPInfo[port].DP_3DMode = E_DP_3D_INPUT_FRAME_SEQUENTIAL;
        ucSCFmtIn = SC_FMT_IN_3D_PF;
        NoVscPktCnt=0;
    }
    else if( Get_VSC_FLAG() )
    {
        if(( msReadByte( REG_21A4 ) & 0x0F ) == 0 )               //E_DP_3D_INPUT_NO_STEREO
        {
            gDPInfo[port].DP_3DMode = E_DP_3D_INPUT_NO_STEREO;
            ucSCFmtIn = SC_FMT_IN_NORMAL;
        }
        else if(( msReadByte( REG_21A4 ) & 0x0F ) == 0x01 )       //E_DP_3D_INPUT_FRAME_SEQUENTIAL
        {
            gDPInfo[port].DP_3DMode = E_DP_3D_INPUT_FRAME_SEQUENTIAL;
            ucSCFmtIn = SC_FMT_IN_3D_PF;
        }
        else if(( msReadByte( REG_21A4 ) & 0x0F ) == 0x02 )      //E_DP_3D_INPUT_STACK_FRAME
        {
            gDPInfo[port].DP_3DMode = E_DP_3D_INPUT_STACK_FRAME;
            ucSCFmtIn = SC_FMT_IN_3D_FP;
        }
        else if(( msReadByte( REG_21A4 ) & 0x0F ) == 0x03 )      //E_DP_3D_INPUT_PIXEL_INTERLEAVED
        {
            gDPInfo[port][port].DP_3DMode = E_DP_3D_INPUT_PIXEL_INTERLEAVED;
            ucSCFmtIn = SC_FMT_IN_3D_LA;

            if((( msReadByte( REG_21A4 ) >> 4 ) & 0x0F ) < 5 )
                gDPInfo[port].PAYLOAD = ( msReadByte( REG_21A4 ) >> 4 ) & 0x0F;
            else
                gDPInfo[port].PAYLOAD = E_DP_3D_DATA_RESERVED;

            if( gDPInfo[port].PAYLOAD == 0x02 )
            {
                ucSCFmtIn = SC_FMT_IN_3D_CB;
            }
        }
        else if(( msReadByte( REG_21A4 ) & 0x0F ) == 0x04 )          //E_DP_3D_INPUT_SIDE_BY_SIDE_FULL
        {
            gDPInfo[port].DP_3DMode = E_DP_3D_INPUT_SIDE_BY_SIDE_FULL;
            ucSCFmtIn = SC_FMT_IN_3D_SBSF;

            if((( msReadByte( REG_21A4 ) >> 4 ) & 0x0F ) < 2 )
                gDPInfo[port].PAYLOAD = ( msReadByte( REG_21A4 ) >> 4 ) & 0x03;
            else
                gDPInfo[port].PAYLOAD = E_DP_3D_DATA_RESERVED;
        }
        else
        {
            gDPInfo[port].DP_3DMode = E_DP_3D_RESERVED;
            ucSCFmtIn = SC_FMT_IN_NORMAL;
        }
        NoVscPktCnt = 0;
    }
 //DP_printData( " (ucSCFmtIn=%x)", ucSCFmtIn);
 //DP_printData( " ( g_SetupPathInfo.ucSCFmtIn=%x)", g_SetupPathInfo.ucSCFmtIn );
 //DP_printData( " (NoVscPktCnt=%x)", NoVscPktCnt);

    if((( ucSCFmtIn != g_SetupPathInfo.ucSCFmtIn ) && (( NoVscPktCnt == 0 ) || ( NoVscPktCnt >= 10 ) ) ) )
    {

        DP_printData( "DP 3D FMT CHG : %d", ucSCFmtIn );
        NoVscPktCnt = 0;
        g_SetupPathInfo.ucSCFmtIn = ucSCFmtIn;
        Set_InputTimingChangeFlag();
    }
//#else
 //if(!g_SetupPathInfo.bOverrideSCFmtIn)
 //g_SetupPathInfo.ucSCFmtIn = SC_FMT_IN_NORMAL;

#endif
}

//**************************************************************************
//  [Function Name]:
//                  DPRxTiming()
//  [Description]
//                  DPRxTiming
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void DPRxVPLLHandle( )
{
    if( gDPInfo[port].bDPVPLLBIGChange )
    {
        gDPInfo[port].ucState = eSTATE_MEASURE;
        gDPInfo[port].bDPVPLLBIGChange = FALSE;                               // clear IRQ Flag
        msWriteByte(  REG_1133A8+port*HBR2_OFFSET, msReadByte(  REG_11338C+port*HBR2_OFFSET ) ); // update NVID
        msWriteByte(  REG_1133A7+port*HBR2_OFFSET, msReadByte(  REG_11338B+port*HBR2_OFFSET ) ); // update NVID
        msWriteByte(  REG_1133A6+port*HBR2_OFFSET, msReadByte(  REG_11338A+port*HBR2_OFFSET ) ); // update NVID
        msWriteByte(  REG_11339E+port*HBR2_OFFSET, msReadByte(  REG_113386+port*HBR2_OFFSET ) ); // update MVID
        msWriteByte(  REG_11339F+port*HBR2_OFFSET, msReadByte(  REG_113387+port*HBR2_OFFSET ) ); // update MVID
        msWriteByte(  REG_1133A0+port*HBR2_OFFSET, msReadByte(  REG_113388+port*HBR2_OFFSET ) ); // update MVID
        DP_printMsg( "\r\nVIDEO MN BIG CHANGE" );
	#if DPSSC
	DPAudioSSCInit(PortToCombo[port]);
	#endif
    }
}
//**************************************************************************
//  [Function Name]:
//                  DPRxCheckInputFormat()
//  [Description]
//                  DPRxCheckInputFormat
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void DPRxCheckInputFormat( )
{
    BYTE u8Temp=DPRxPacketColor();
    BYTE u8Temp1 = DPRxPacketType();
    BYTE u8Temp2 = DPRxPacketRange();
    BYTE u8Temp3 = DPRxColorDepth();
    // DP MSA MISC[7:0]
    // [2:1]        00:RGB              ,             01:YCbCr 422        ,   10:YCbCr 444
    // [3]          0:VESA Range     ,               1:CEA Range
    // [4]          0:BT601              ,               1:BT709
    // [7:5]     000:6bits
    //             001:8bits
    //             010:10bits
    //             011:12bits
    //             100:16bits

    if( u8Temp != gDPInfo[port].ucDPColorFormat )
    {
        gDPInfo[port].ucDPColorFormat = u8Temp;
    }
    if( u8Temp1 != gDPInfo[port].bColorType )
    {
        gDPInfo[port].bColorType = u8Temp1;
    }
     if( u8Temp2 != gDPInfo[port].ucDPRangeFormat )
    {
        gDPInfo[port].ucDPRangeFormat = u8Temp2;
    }
    if( u8Temp3 != gDPInfo[port].ucDPColorDepth )
    {
         gDPInfo[port].ucDPColorDepth = u8Temp3;
    }

}

//**************************************************************************
//  [Function Name]:
//                  DPRxPacketType()
//  [Description]
//                  DPRxPacketType
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE DPRxPacketType(void)
{

    BYTE  temp= BT601;

    msWriteByteMask( REG_1133BC+port*HBR2_OFFSET, 0, BIT6 );

    if( msReadByte( REG_113359+port*HBR2_OFFSET ) & BIT4 )
    {
        temp = BT709;
    }

    msWriteByteMask( REG_1133BC+port*HBR2_OFFSET, BIT6, BIT6 );

    return temp;

}
//**************************************************************************
//  [Function Name]:
//                  DPRxColorDepth()
//  [Description]
//                  DPRxColorDepth
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE DPRxColorDepth(void)
{
     BYTE  temp;
      msWriteByteMask( REG_1133BC+port*HBR2_OFFSET, 0, BIT6 );
      switch(msReadByte( REG_113359+port*HBR2_OFFSET )>>5 )
    {
        case 0:
            temp = DP_COLOR_DEPTH_6_BIT;
            break;
        case 1:
            temp = DP_COLOR_DEPTH_8_BIT;
            break;
        case 2:
            temp = DP_COLOR_DEPTH_10_BIT;
            break;
      case 3:
            temp = DP_COLOR_DEPTH_12_BIT;
            break;
          case 4:
            temp = DP_COLOR_DEPTH_16_BIT;
            break;
          default:
           temp = DP_COLOR_DEPTH_UNKNOWN;
            break;

    }
    msWriteByteMask( REG_1133BC+port*HBR2_OFFSET, BIT6, BIT6 );
    return temp;
}

//**************************************************************************
//  [Function Name]:
//                  DPRxPacketRange()
//  [Description]
//                  DPRxPacketRange
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE DPRxPacketRange( )
{
    BYTE  temp;

    gDPInfo[port].DPMISC1 = msReadByte( REG_113359+port*HBR2_OFFSET );
    msWriteByteMask( REG_1133BC+port*HBR2_OFFSET, 0, BIT6 );
    gDPInfo[port].DPMISC0 = msReadByte( REG_113359+port*HBR2_OFFSET );

    temp = InputRange_Full;

    if( msReadByte( REG_113359+port*HBR2_OFFSET ) & BIT3 )
    {
        temp = InputRange_Limited;
    }
    else
    {
        temp = InputRange_Full;
    }

    msWriteByteMask( REG_1133BC+port*HBR2_OFFSET, BIT6, BIT6 );

    return temp;
}
//**************************************************************************
//  [Function Name]:
//                  DPRxPacketColor()
//  [Description]
//                  DPRxPacketColor
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE DPRxPacketColor( )
{
    BYTE  temp;

    gDPInfo[port].DPMISC1 = msReadByte( REG_113359+port*HBR2_OFFSET );
    msWriteByteMask( REG_1133BC+port*HBR2_OFFSET, 0, BIT6 );
    gDPInfo[port].DPMISC0 = msReadByte( REG_113359+port*HBR2_OFFSET );

    temp = InputColor_RGB;
    switch(( msReadByte( REG_113359+port*HBR2_OFFSET ) & 0x06 ) >> 1 )
    {
        case 0:
            temp = COMBO_COLOR_FORMAT_RGB;
            break;
        case 1:
            temp = COMBO_COLOR_FORMAT_YUV_422;
            break;
        case 2:
            temp = COMBO_COLOR_FORMAT_YUV_444;
            break;
        default:
            temp =COMBO_COLOR_FORMAT_UNKNOWN;
    }

    msWriteByteMask( REG_1133BC+port*HBR2_OFFSET, BIT6, BIT6 );

    return temp;
}

//**************************************************************************
//  [Function Name]:
//                  DPRxToggleShortHPD()
//  [Description]
//                  DPRxToggleShortHPD
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void DPRxToggleShortHPD( BYTE InputPort)
{

	if( InputPort == Combo_PORT0)
	{
		hw_Clr_DPHpd0();
	}
	else if(InputPort == Combo_PORT1)
	{
		hw_Clr_DPHpd1();
	}
	else if(InputPort == Combo_PORT2)
	{
		hw_Clr_DPHpd2();
	}
	else if(InputPort == Combo_PORT3)
	{
		hw_Clr_DPHpd3();
	}
	else if(InputPort == Combo_PORT4)
	{
		hw_Clr_DPHpd4();
	}
	else if(InputPort == Combo_PORT5)
	{
		hw_Clr_DPHpd5();
	}
	DelayUs( 250 );
	DelayUs( 250 );
	DelayUs( 250 );
	DelayUs( 250 );

	if( InputPort == Combo_PORT0)
	{
		hw_Set_DPHpd0();
	}
	else if(InputPort == Combo_PORT1)
	{
		hw_Set_DPHpd1();
	}
	else if(InputPort == Combo_PORT2)
	{
		hw_Set_DPHpd2();
	}
	else if(InputPort == Combo_PORT3)
	{
		hw_Set_DPHpd3();
	}
	else if(InputPort == Combo_PORT4)
	{
		hw_Set_DPHpd4();
	}
	else if(InputPort == Combo_PORT5)
	{
		hw_Set_DPHpd5();
	}
}


//**************************************************************************
//  [Function Name]:
//                  DPRxHPDIRQ()
//  [Description]
//                  DPRxHPDIRQ
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void DPRxHPDIRQ( )
{
    BYTE ucTemp = PortToCombo[port];

    if( ucTemp == Combo_PORT0)
        hw_Clr_DPHpd0();
    else if(ucTemp == Combo_PORT1)
        hw_Clr_DPHpd1();
    else if(ucTemp == Combo_PORT2)
        hw_Clr_DPHpd2();
    else if(ucTemp == Combo_PORT3)
        hw_Clr_DPHpd3();
    else if(ucTemp == Combo_PORT4)
        hw_Clr_DPHpd4();
    else if(ucTemp == Combo_PORT5)
        hw_Clr_DPHpd5();

    //Delay1ms( 2 );
       DelayUs(250);
	DelayUs(250);   
	DelayUs(250);   
	DelayUs(250);   

    if( ucTemp == Combo_PORT0)
        hw_Set_DPHpd0();
    else if(ucTemp == Combo_PORT1)
        hw_Set_DPHpd1();
    else if(ucTemp == Combo_PORT2)
        hw_Set_DPHpd2();
    else if(ucTemp == Combo_PORT3)
        hw_Set_DPHpd3();
    else if(ucTemp == Combo_PORT4)
        hw_Set_DPHpd4();
    else if(ucTemp == Combo_PORT5)
        hw_Set_DPHpd5();

    DP_printMsg( "DPRxHPDIRQ!" );
}

//**************************************************************************
//  [Function Name]:
//                  DPRxClearAUXVliadStatus()
//  [Description]
//                 DPRxClearAUXVliadStatus
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void DPRxClearAUXVliadStatus( )
{
    gDPInfo[port].bDPAUXVALID = FALSE;
}

//**************************************************************************
//  [Function Name]:
//                  DPRxCheckSQTime()
//  [Description]
//                  DPRxCheckSQTime
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BOOL DPRxCheckSQTime(WORD Times)
{
    if (DPRxGetSquelch(1) )
    {
        if (gDPInfo[port].uwDPRxStableTimeout==0 && !gDPInfo[port].bSquelchStable)
        {
            if (!gDPInfo[port].bSquelchWaitStable)
            {
                gDPInfo[port].uwDPRxStableTimeout=Times;
                gDPInfo[port].bSquelchWaitStable=1;
                gDPInfo[port].bSquelchStable=0;
            }
            else
            {
                gDPInfo[port].bSquelchWaitStable=0;
                gDPInfo[port].bSquelchStable=1;
                gDPInfo[port].uwFastTrainingCntDown=0;
            }
        }
        return TRUE;
    }
    else
    {
        gDPInfo[port].bSquelchWaitStable=0;
        gDPInfo[port].bSquelchStable=0;
        gDPInfo[port].uwDPRxStableTimeout=0;
        gDPInfo[port].uwFastTrainingCntDown=0;
        return FALSE;
    }

}

//**************************************************************************
//  [Function Name]:
//                  DPRxGetSquelch()
//  [Description]
//                  DPRxGetSquelch
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool DPRxGetSquelch( BYTE Times)
{
    BYTE xdata Temp=1;

     Temp = (Temp << PortToCombo[port]);

    while( Times-- )
    {
        if( !( msReadByte( REG_0017B0 )&Temp ) )
        {
            return FALSE;
        }

    }  //DP_printMsg( "SQ Active" );

    return TRUE;
}

//**************************************************************************
//  [Function Name]:
//                  DPRxClrSQTime()
//  [Description]
//                  DPRxClrSQTime
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void DPRxClrSQTime()
{
    gDPInfo[port].bSquelchWaitStable=0;
    gDPInfo[port].bSquelchStable=0;
    gDPInfo[port].uwDPRxStableTimeout=0;
    gDPInfo[port].uwFastTrainingCntDown=0;
}
//**************************************************************************
//  [Function Name]:
//                  DPFastLink()
//  [Description]
//                  DPFastLink
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BOOL DPFastLink()
{
    BYTE xdata i=0,j = 0;
#if(AUTO_EQ)
	BYTE xdata delay_count = 0;
#endif
    BYTE xdata FastLane[3] ={0x04,0x02,0x01};
    BYTE xdata FastRate[4] = {0x14,0x0A,0x06,0x1};

    DP_printMsg( "------>>  <Fast> " );

    for(j=0;j<4;j++)
    {

            if((j==3)|| gDPInfo[port].bDPTrainingP0T || gDPInfo[port].bDPTrainingP1T )
        {
            goto FastTraining_Exit;
        }

        DP_printData( "---Rate--->>%x ", FastRate[j]);

        if( FastRate[j] == 0x06 )
            DPRxSetSyntheizer( DP_LBR, port );
            else if( FastRate[j] == 0x0A )
            DPRxSetSyntheizer( DP_HBR, port );
         else if( FastRate[j] == 0x14 )
                DPRxSetSyntheizer( DP_HBR2, port );

        for(i=0;i<3;i++)
        {
            msWriteByte( REG_11330F+port*HBR2_OFFSET, 0x80 + FastLane[i] );
            if( (msReadByte(REG_113310+port*HBR2_OFFSET )&0x03) == 0x00 )
            {
                if(msReadByte(REG_113310+port*HBR2_OFFSET)&BIT5)
                    msWriteByteMask( REG_113310+port*HBR2_OFFSET, 0 , BIT5  );
                DPRxFastTraining2Enable( TRUE );
            }

	 #if(AUTO_EQ)

    #if(COMBO_DP_DAISY_CHAIN_SUPPORT)
            if(port == HBR2_0)
            {
				ForceDelay1ms( 150 );
            }
    #else			
			if(0)
			{}
    #endif
	        else
	        {
            DPRxSetFastLinkAEQ(port, TRUE);

            for(delay_count=0 ; delay_count<50 ; delay_count++)
            {
                Delay1ms( 1 );
                if(gDPInfo[port].bDPTrainingP0T || gDPInfo[port].bDPTrainingP1T )
                {
                    DPRxSetFastLinkAEQ(port, FALSE);

                    goto FastTraining_Exit;
                }

            }

            if((msRegs(REG_112083+port*COMBO_PHY_OFFSET)& 0x01)==0x01)
            {
                Delay1ms( 5 );

                if((msRegs(REG_112083+port*COMBO_PHY_OFFSET)& 0x01)==0x01)
                {
                    DPRxSetFastLinkAEQ(port, FALSE);
                }
            }
	        }
	  #else
            ForceDelay1ms( 150 );
	  #endif

            if( !DPRxCheckLock() )
            {
                DPRxFastTraining2Enable(FALSE);
            }
            else
            {
                BYTE xdata Counter=0;

                while(DPRxCheckLock())
                {
                    if(++Counter>5)
                    {
                        goto FastTraining_OK;
                    }
                }
            }
        }
    }
    goto FastTraining_Exit;

    FastTraining_OK:
        Delay1ms( 50 );
        msWriteByte( REG_113396+port*HBR2_OFFSET,0xFF  );
        if(!DPRxCheckTiming(MSACnt))
        {
            msWriteByteMask( REG_113310+port*HBR2_OFFSET, BIT5 , BIT5  );
            if(!DPRxCheckTiming(MSACnt))
            {
                msWriteByteMask( REG_113310+port*HBR2_OFFSET, 0 , BIT5  );
                if(!DPRxCheckTiming(MSACnt))
                {
                	 DPRxFastTraining2Enable(FALSE);
                    //msWriteByte( REG_113396+port*HBR2_OFFSET,0x00  );
                    return FALSE;
                }
            }
        }
        // msWriteByte( REG_113396+port*HBR2_OFFSET,0x00  );
        DPRxFastTraining2Enable(FALSE);
        return TRUE;

    FastTraining_Exit:
        DP_printMsg( " <Fast> - DP Lose Lock!" );
        DPRxFastTraining2Enable( FALSE );
        gDPInfo[port].DPCheckModeCnt =0;
        return TRUE;


}


//**************************************************************************
//  [Function Name]:
//                  DPRxHPDIRQ()
//  [Description]
//                  DPRxHPDIRQ
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
//
//  FullRate_Mode:  HBR/HBR2 = 0x19999a,  ==> 0x19AA06
//  Nonfullrate mode    HBR  = 0x333333
//  Nonfullrate mode    RBR  = 0x2aaaab
//==========================================================
void DPRxSetSyntheizer( E_DP_Rate bRate, BYTE temp )
{
    if( bRate == DP_LBR )
    {
        msWrite2Byte( REG_112004 + temp*COMBO_PHY_OFFSET, 0xaaab );
        msWriteByte( REG_112006 + temp*COMBO_PHY_OFFSET, 0x2a );
        msWriteByte( REG_11330E + HBRPort(temp)*HBR2_OFFSET,0x06  );
    }
    else if( bRate == DP_HBR )
    {
        msWrite2Byte( REG_112004 + temp*COMBO_PHY_OFFSET, 0x3333 );
        msWriteByte( REG_112006 + temp*COMBO_PHY_OFFSET, 0x33 );
        msWriteByte( REG_11330E+HBRPort(temp)*HBR2_OFFSET,0x0A  );
    }
    else if( bRate == DP_HBR2 )
    {
        msWrite2Byte( REG_112004 + temp*COMBO_PHY_OFFSET, 0xAA06 );
        msWriteByte( REG_112006 + temp*COMBO_PHY_OFFSET, 0x19 );
        msWriteByte( REG_11330E+HBRPort(temp)*HBR2_OFFSET,0x14  );
    }
}

//**************************************************************************
//  [Function Name]:
//                  DPRxSetFastLinkAEQ(BYTE Inputport, Bool bEnable)
//  [Description]
//                  DPRxSetFastLinkAEQ
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void DPRxSetFastLinkAEQ(BYTE Inputport, Bool bEnable)
{

#if (FT_EQ_OFFSET_FUNCTION)
    WORD EQ0=0, EQ1=0, EQ2=0, EQ3=0, EQ_CAL1=0, EQ_CAL2=0, EQ_CAL3=0;
#endif

    if(bEnable == TRUE)
    {
        msWrite2Byte( REG_1120DC + Inputport*COMBO_PHY_OFFSET, 0x0000 );

        msWriteByteMask( REG_113000 + HBRPort(Inputport)*HBR2_OFFSET, 0 , BIT5|BIT4|BIT3|BIT2);
        msWriteByteMask( REG_113012 + HBRPort(Inputport)*HBR2_OFFSET, BIT4 , BIT4);

        msWriteByteMask( REG_11204A + Inputport*COMBO_PHY_OFFSET, 0 , BIT2);
        msWriteByteMask( REG_112051 + Inputport*COMBO_PHY_OFFSET, BIT1 , BIT1|BIT0);

        msWriteByteMask( REG_113000 + HBRPort(Inputport)*HBR2_OFFSET, 0 , BIT0);
        msWriteByteMask( REG_113000 + HBRPort(Inputport)*HBR2_OFFSET, BIT1 , BIT1);
        msWriteByteMask( REG_113000 + HBRPort(Inputport)*HBR2_OFFSET, BIT0 , BIT0);

        Delay1ms( 1 );

        msWriteByteMask( REG_11204A + Inputport*COMBO_PHY_OFFSET, BIT2 , BIT2);
    }
    else
    {
        msWriteByteMask( REG_113000 + HBRPort(Inputport)*HBR2_OFFSET, 0 , BIT1);

        msWriteByteMask( REG_113000 + HBRPort(Inputport)*HBR2_OFFSET, BIT2 , BIT5|BIT4|BIT3|BIT2);
        msWriteByteMask( REG_113012 + HBRPort(Inputport)*HBR2_OFFSET, 0 , BIT4);

        msWriteByteMask( REG_112051 + Inputport*COMBO_PHY_OFFSET, BIT1|BIT0 , BIT1|BIT0);

#if (FT_EQ_OFFSET_FUNCTION)

        if((msReadByte(REG_1120E6+Inputport*COMBO_PHY_OFFSET) & 0x1F) <= (0x1F - FT_EQ_OFFSET_VALUE))
            EQ0 = (msReadByte(REG_1120E6+Inputport*COMBO_PHY_OFFSET) & 0x1F) + FT_EQ_OFFSET_VALUE;
        else
            EQ0 = 0x1F;
        if((((msReadByte(REG_1120E6+Inputport*COMBO_PHY_OFFSET) & 0xE0) >> 5) | ((msReadByte(REG_1120E7+Inputport*COMBO_PHY_OFFSET) & 0x03) << 3)) <= (0x1F - FT_EQ_OFFSET_VALUE))
            EQ1 = (((msReadByte(REG_1120E6+Inputport*COMBO_PHY_OFFSET) & 0xE0) >> 5) | ((msReadByte(REG_1120E7+Inputport*COMBO_PHY_OFFSET) & 0x03) << 3)) + FT_EQ_OFFSET_VALUE;
        else
            EQ1 =0x1F;
        if(((msReadByte(REG_1120E7+Inputport*COMBO_PHY_OFFSET) & 0x7C) >> 2) <= (0x1F - FT_EQ_OFFSET_VALUE))
            EQ2 = ((msReadByte(REG_1120E7+Inputport*COMBO_PHY_OFFSET) & 0x7C) >> 2) + FT_EQ_OFFSET_VALUE;
        else
            EQ2 =0x1F;
        if((msReadByte(REG_1120E8+Inputport*COMBO_PHY_OFFSET) & 0x1F) <= (0x1F - FT_EQ_OFFSET_VALUE))
            EQ3 = (msReadByte(REG_1120E8+Inputport*COMBO_PHY_OFFSET) & 0x1F) + FT_EQ_OFFSET_VALUE;
        else
            EQ3 =0x1F;

        EQ_CAL1 = (((EQ1 & 0x07) << 5) | (EQ0 & 0x1F));
        msWriteByte( REG_1120E6+Inputport*COMBO_PHY_OFFSET, EQ_CAL1 );

        EQ_CAL2 = (((EQ2 & 0x1F) << 2) | ((EQ1 & 0x18) >> 3));
        msWriteByte( REG_1120E7+Inputport*COMBO_PHY_OFFSET, EQ_CAL2 );

        EQ_CAL3 = (EQ3 & 0x1F);
        msWriteByte( REG_1120E8+Inputport*COMBO_PHY_OFFSET, EQ_CAL3 );

        msWrite2Byte( REG_1120DC + Inputport*COMBO_PHY_OFFSET, 0x03C0 );

#endif

    }
}


//**************************************************************************
//  [Function Name]:
//                  DPRxFastTraining2Enable()
//  [Description]
//                  DPRxFastTraining2Enable
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void DPRxFastTraining2Enable( Bool bEnable )
{
    if( bEnable )
    {
        DP_printMsg( " <Fast> - En " );
        msWriteByteMask( REG_1120D7+port*COMBO_PHY_OFFSET, 0x0E,0x0E );
        msWriteByteMask( REG_1134F7+port*HBR2_OFFSET, BIT1|BIT3, BIT1|BIT3 );
        msWriteByte( REG_11358E+port*HBR2_OFFSET,0x5B );
    }
    else
    {
        DP_printMsg( " <Fast> - Diable " );
        msWriteByteMask( REG_1120D7+port*COMBO_PHY_OFFSET, 0x00,0x0E );
        msWriteByteMask( REG_1134F7+port*HBR2_OFFSET, 0, BIT1|BIT3 );
        msWriteByte( REG_11358E+port*HBR2_OFFSET,0x58);
    }
}

//**************************************************************************
//  [Function Name]:
//                  DPRxAudioHandle()
//  [Description]
//                  DPRxAudioHandle
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void DPRxAudioHandle( )
{
    WORD xdata LinkRate=0;
    DWORD xdata DPAUM0 = 0;
    DWORD xdata DPAUM1 = 0;
    DWORD xdata DPAUM2 = 0;
    DWORD xdata DPAUN0 = 0;
    DWORD xdata DPAUN1 = 0;
    DWORD xdata DPAUN2 = 0;
    BYTE xdata DPFBDIV0 = 0;
    BYTE xdata DPFBDIV1 = 0;
    BYTE xdata DPKP = 0;
    BYTE xdata DPKM = 0;
    BYTE xdata DPFREQ0 = 0;
    BYTE xdata DPFREQ1 = 0;
    BYTE xdata DPFREQ2 = 0;
    DWORD xdata DPAUM = 0;
    DWORD xdata DPAUN = 0;
    DWORD xdata DPFREQ = 0;
    DWORD xdata DPFBDIV = 0;
    BYTE xdata utemp=0;
    WORD wDPAudioFreq;
    
    if(!gDPCombo[PortToCombo[port]].AudioEn)
    return;

    gDPInfo[port].bDPNoAudioStrm = ( msReadByte(REG_1133A5+port*HBR2_OFFSET ) & 0x10 ) ? 1 : 0; // Read BK36 A5 Bit4:1 NoAudioStrm

     #if (DP_NOVIDEO)
    if((gDPInfo[port].ucState==eSTATE_VIDEOMUTE)
        #if DPSSC
        && gDPInfo[port].bDPAudioSSCInit
        #endif
        )
    {
        if(
        #if DPSSC
            (DPRxAudioCnt ==0)&&
        #endif
            (( gDPInfo[port].InputPacketStatus & 0x60) == 0x60)
            )
        {
            #if DPSSC
            DPRxAudioCnt =5;
            DPAudioSSCAdjust();
            #else
            DisableDPMute();
            DPRxAudioMuteEn =0;
            #endif
        }
    }
    else
    #endif



    
    if( gDPInfo[port].bDPNoAudioStrm )
    {
        HW_SET_AUDIO_MUTE_ENABLE();
        return;
    }
    else
    {
    	#if 1
        if(( gDPInfo[port].InputPacketStatus & 0x60) == 0x60)
        {
           	#if !DPSSC
		DisableDPMute();
              DPRxAudioMuteEn =0;
		#endif
		//DP_printMsg("\r\n===HBR2==>   Unmute ");
        }
        else
        {
		EnableDPMute();
	       DPRxAudioMuteEn = 1;
		#if DPSSC
		DPAudioSSCInit(PortToCombo[port]);
		#endif
		//DP_printMsg("\r\n===HBR2==>   mute ");
        }
        #endif
    }

#if DPSSC
    if((DPRxAudioCnt ==0)&&(( gDPInfo[port].InputPacketStatus & 0x60) == 0x60))
    {

        DPRxAudioCnt =5;
    DPAudioSSCAdjust();
    }
 #endif
    DPAudioMuteCheck();

    if( gDPInfo[port].bDPLANELOCK == TRUE )
    {
        utemp = msReadByte(  REG_11330E+port*HBR2_OFFSET );

        if(( utemp & 0x0F ) == 0x06 )
        {
            //msWriteByte(REG_37B8,0x22);              // APLL 512*Fs at 1.62Ghz 162Mhz MCKSEL=0
            LinkRate = 162;
        }
        else  if(( utemp & 0x0F ) == 0x0A)
        {
            //msWriteByte(REG_37B8,0x23);              // APLL 512*Fs at 2.7Ghz 270Mhz  MCKSEL=0
            LinkRate = 270;
        }
        else  if(( utemp & 0x1F ) == 0x14)
        {
          LinkRate = 540;
        }
        else

             LinkRate = 270;
        //################  Audio M/N example ###################
        //M=512*fs*N/LinkRate
        //N=0x8000=32768
        //32K,2.7G   => M=512*32K*32768/270000K=1988.4=0x07_C4
        //48K,2.7G   => M=512*48K*32768/270000K=2982.6=0x0B_A6
        //32K,1.62G => M=512*32K*32768/162000K=3314.0=0x0C_F2
        //48K,1.62G => M=512*48K*32768/162000K=4971.0=0x13_6B
        // ################################################

        DPAUM0 = msReadByte(  REG_11338E+port*HBR2_OFFSET ); //MSA MAUD_CENTER[7:0]
        DPAUM1 = msReadByte(  REG_11338F+port*HBR2_OFFSET ); //MSA MAUD_CENTER[15:8]
        DPAUM2 = msReadByte(  REG_113390+port*HBR2_OFFSET ); //MSA MAUD_CENTER[23:16]
        DPAUM = ( DPAUM2 << 16 ) + ( DPAUM1 << 8 ) + ( DPAUM0 );
        DPAUN0 = msReadByte(  REG_113392+port*HBR2_OFFSET ); //MSA NAUD_CENTER[7:0]
        DPAUN1 = msReadByte(  REG_113393+port*HBR2_OFFSET ); //MSA NAUD_CENTER[15:8]
        DPAUN2 = msReadByte(  REG_113394+port*HBR2_OFFSET ); //MSA NAUD_CENTER[23:16]
        DPAUN = ( DPAUN2 << 16 ) + ( DPAUN1 << 8 ) + ( DPAUN0 );
       DPFBDIV0 = msReadByte(  REG_162F04); //FBDIV[7:0]
       DPFBDIV1 = msReadByte(  REG_162F05 ); //FBDIV[15:8]
       DPFBDIV = ( DPFBDIV1 << 8 ) + DPFBDIV0;
       DPKM = GetPow2( msReadByte( REG_162F01) & 0x0F ); //KM
       DPKP = GetPow2((msReadByte( REG_162F00 ) >> 4 ) & 0x0F ); //Kp

	if(DPAUN)
		 wDPAudioFreq= ((((LinkRate*1000ul)+256ul)/512ul)*DPAUM)/ DPAUN;    
	else
	  {
	  	  gDPInfo[port].bDPAudioFreq  = DPAUD_FREQ_ERROR;
	  	  return;
	  }	
	if( abs( wDPAudioFreq - 32 ) < 4 )
	  gDPInfo[port].bDPAudioFreq  = DPAUD_FREQ_32K;
	else if( abs( wDPAudioFreq - 44 ) < 4 )
	  gDPInfo[port].bDPAudioFreq  = DPAUD_FREQ_44K;
	else if( abs( wDPAudioFreq - 48 ) < 4 )
	  gDPInfo[port].bDPAudioFreq  = DPAUD_FREQ_48K;
	else if( abs( wDPAudioFreq - 88 ) < 4 )
	  gDPInfo[port].bDPAudioFreq  = DPAUD_FREQ_88K;
	else if( abs( wDPAudioFreq - 96 ) < 4 )
	  gDPInfo[port].bDPAudioFreq  = DPAUD_FREQ_96K;
	else if( abs( wDPAudioFreq - 176 ) < 4 )
	  gDPInfo[port].bDPAudioFreq  = DPAUD_FREQ_176K;
	else if( abs( wDPAudioFreq - 192 ) < 4 )
	  gDPInfo[port].bDPAudioFreq  = DPAUD_FREQ_192K;
	else
	  gDPInfo[port].bDPAudioFreq  = DPAUD_FREQ_ERROR;
	  
        gDPInfo[port].DPAUM_OLD =  (msReadByte( REG_1133B0+port*HBR2_OFFSET  ) <<16)  + (msReadByte( REG_1133AF+port*HBR2_OFFSET  )<<8) + msReadByte( REG_1133AE+port*HBR2_OFFSET );
        gDPInfo[port].DPAUN_OLD =  (msReadByte( REG_1133B8+port*HBR2_OFFSET  ) <<16)  + (msReadByte( REG_1133B7+port*HBR2_OFFSET  )<<8) + msReadByte( REG_1133B6+port*HBR2_OFFSET );

        if((abs( DPAUM - gDPInfo[port].DPAUM_OLD ) > 0x80)||(DPKM !=  gDPInfo[port].DPAudioKM_OLD)||(DPKP != gDPInfo[port].DPAudioKP_OLD)||(DPFBDIV1 !=gDPInfo[port].DPAudioFBDIV_OLD))
        {
            gDPInfo[port].bDPAUPLLBIGChange = TRUE;
        }

      gDPInfo[port].DPAudioKM_OLD = DPKM;
      gDPInfo[port].DPAudioKP_OLD = DPKP;
      gDPInfo[port].DPAudioFBDIV_OLD = DPFBDIV1;

        if( gDPInfo[port].bDPAUPLLBIGChange && ( !gDPInfo[port].bDPNoAudioStrm ) )
        {

            msWriteByte(  REG_1133AE+port*HBR2_OFFSET, DPAUM0 ); //UPDATE MAUD_CENTER[7:0]
            msWriteByte(  REG_1133AF+port*HBR2_OFFSET, DPAUM1 ); //UPDATE MAUD_CENTER[15:8]
            msWriteByte(  REG_1133B0+port*HBR2_OFFSET, DPAUM2 ); //UPDATE MAUD_CENTER[23:16]
            msWriteByte(  REG_1133B6+port*HBR2_OFFSET, DPAUN0 ); //UPDATE NAUD_CENTER[7:0]
            msWriteByte(  REG_1133B7+port*HBR2_OFFSET, DPAUN1 ); //UPDATE NAUD_CENTER[15:8]
            msWriteByte(  REG_1133B8+port*HBR2_OFFSET, DPAUN2 ); //UPDATE NAUD_CENTER[23:16]
            DP_printData( "\r\nDP_Audio_bigchange%x ", 0 );
            gDPInfo[port].bDPAUPLLBIGChange = FALSE;        // claer IRQ flag

        #if DPSSC
         if(DPFBDIV1_Old == DPFBDIV1)
         {
                if(++DPFBDIV1_Cnt >20)
                {
                        DPFBDIV1_Cnt=0;
                    gDPInfo[port].bDPAUPLLBIGChange = FALSE;        // claer IRQ flag
                }
         }
         else
         {
            DPFBDIV1_Cnt=0;
         }
               DPFBDIV1_Old = DPFBDIV1;
         #else
            gDPInfo[port].bDPAUPLLBIGChange = FALSE;
           #endif

            DPFREQ = 64 * (216.04 / LinkRate ) * ( DPKM * DPAUN ) / ( DPKP * DPAUM ) * DPFBDIV;

            DPFREQ0 = ( DPFREQ & 0x000000FF );
            DPFREQ1 = ( DPFREQ & 0x0000FF00 ) >> 8;
            DPFREQ2 = ( DPFREQ & 0x00FF0000 ) >> 16;

            msWriteByte( REG_162F08, DPFREQ0 );           //Freq
            msWriteByte( REG_162F09, DPFREQ1 );           //Freq
            msWriteByte( REG_162F0A, DPFREQ2 );           //Freq

        #if DPSSC
		DPAudioSSCInit(PortToCombo[port]);
        #endif

            msWriteByteMask( REG_162F07, BIT5,BIT5 );           //update Freq
            msWriteByteMask( REG_162F07, 0,BIT5);                 //update Freq


        #if 1
            DP_printData( "AUD M0 %x", DPAUM0 );
            DP_printData( "AUD M1 %x", DPAUM1 );
            DP_printData( "AUD M2 %x", DPAUM2 );
            DP_printData( "AUD N0 %x", DPAUN0 );
            DP_printData( "AUD N1 %x", DPAUN1 );
            DP_printData( "AUD N2 %x", DPAUN2 );
            DP_printData( "AUD Freq0 %x", DPFREQ0 );
            DP_printData( "AUD Freq1 %x", DPFREQ1 );
            DP_printData( "AUD Freq2 %x", DPFREQ2 );
        #endif
        }
    }
}
#if DPSSC
//**************************************************************************
//  [Function Name]:
//                  DPAudioSSCInit()
//  [Description]
//                  DPAudioSSCInit
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void DPAudioSSCInit(BYTE InputPort)
{
	  if( gDPCombo[InputPort].AudioEn )
{
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
		msWriteByteMask(REG_1134B3,0,BIT2);     //OV MN EN
		msWriteByteMask(REG_1630B1,BIT0|BIT1,BIT0|BIT1);
		//msWriteByteMask( REG_162F44, BIT6,BIT6 );              // FIFO reset
		DPAudioTestFlag = 0;
		DPAudioTestCount = 0;

		#if (DP_NOVIDEO == 1)
		gDPInfo[port].bDPAudioSSCInit = 1;
		#endif
		
	  }
}
#endif
//**************************************************************************
//  [Function Name]:
//                  DPAudioSSCAdjust()
//  [Description]
//                  DPAudioSSCAdjust
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void DPAudioSSCAdjust()
{
#if DPSSC
    BYTE i=0;
    BYTE AudioError=0;
#endif

#if DPSSC
#if DP_DEBUG
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

                                  DP_printData( "FIFO== %x", msReadByte(REG_1630B1) );
                                  DP_printData( "DPRxAudioSSCEnCnt== %x", DPRxAudioSSCEnCnt );
                                 if(!DPAudioUnmute_Case1)
                                  {
                             for(i=0;i<DP_COMBO;i++)
                             {
                                if( gDPCombo[i].AudioEn )
                                {   DP_printMsg("\r\n==1==Audio Mute ");
                                    EnableDPMute();
                                    DPRxAudioMuteEn = 1;
                                }
                             }
								}

							DPRxCearCnt = 0;
                                DPRxUnMuteCnt =0;
                                DPRxAudioSSCEnCnt++;

                                if(DPRxAudioSSCEnCnt > 7)
                                {

						DPRxCearCnt_On=0;
                                    {
								WORD Fre_Ctrl;
								WORD temp;
								BYTE   temp1;

								temp = msReadByte(  REG_162F20 )<<8;
								Fre_Ctrl =  temp + msReadByte( REG_162F1F);
								temp1 = (msReadByte(REG_162F22)&0x0F) + 1;
								DP_printData( "Fre_Cal=L= %x", (msReadByte(REG_162F09)));
								DP_printData( "Fre_Cal=H= %x", (msReadByte(REG_162F0A)));
								DP_printData( "Fre_old=L= %x", Fre_Ctrl);
								DP_printData( "Fre_old=H= %x", Fre_Ctrl>>8 );
								msWriteByte( REG_162F09,  Fre_Ctrl );
								msWriteByte( REG_162F0A,  (Fre_Ctrl >> 8));

						}
						msWriteByteMask( REG_162F07, BIT5,BIT5 );           	   //update Freq
						DPRxAudioSSCEnCnt=0;
									EnableDPSSC();
									DPRxUnMuteCnt=0;
	                                    {
							msWriteByteMask(REG_1630B1,BIT0|BIT1,BIT0|BIT1);
							DPRxAudioSSCEn= 1;
							DP_printMsg("\r\n==1==Enable Audio SSC ");
							DPAudio_State =2;
							DP_printMsg("\r\n DPAudio_State =2 ");
									}

		   			     			}

                                DP_printMsg("\r\n==1==Audio FIFO Unde/OverRUN ");
                                msWriteByteMask(REG_1630B1,BIT0|BIT1,BIT0|BIT1);
        }
        else
        {
				if(DPRxAudioMuteEn == 1)
			{
				if(gDPCombo[DP_HBR_0].AudioEn )
				{
					if(gDPComboInfo[DP_HBR_0].bPacket &BIT2)
				DPRxUnMuteCnt++;

				}
				else
				{
					if(msReadByte(REG_1132AA+port*HBR2_OFFSET)&BIT5)
					{
						DPRxUnMuteCnt++;
						msWriteByteMask(REG_1132AC+port*HBR2_OFFSET ,BIT5,BIT5);
						msWriteByteMask(REG_1132AC+port*HBR2_OFFSET ,0,BIT5);
					}
				}

                    }
                    if(DPRxUnMuteCnt >=(125*Const))
                    {
                        DPRxUnMuteCnt=0;
                        DPRxAudioSSCEnCnt=0;

            if(DPRxAudioMuteEn)
            {
                   for(i=0;i<DP_COMBO;i++)
                   {
	                    if( gDPCombo[i].AudioEn )
	                     {
					DP_printMsg("\r\n=========================1=>Audio unMute ");
					DPAudioUnmute_Case1 =1;
	                    }
                   }

								{
										WORD Fre_Ctrl;
										WORD temp=0;
										WORD temp1=0;
										DPRxAudioSSCEnCnt=0;
									       DPRxUnMuteCnt =0;
										DPAudioUnmute_Case1=0;
										temp = msReadByte(  REG_162F20 )<<8;
										Fre_Ctrl =  temp + msReadByte( REG_162F1F);
										temp1 = (msReadByte(REG_162F22)&0x0F) + 1;
										DP_printData( "Fre_old=L= %x", Fre_Ctrl);
										DP_printData( "Fre_old=H= %x", Fre_Ctrl>>8 );
										{
										Fre_Ctrl = Fre_Ctrl - (Fre_Ctrl >> temp1);
										msWriteByte( REG_162F08,  0x00 );
										DP_printMsg("\r\n   ");
										DPAudioUpDown = 0;
										}
										msWriteByte( REG_162F09,  Fre_Ctrl );
										msWriteByte( REG_162F0A,  (Fre_Ctrl >> 8));
										DP_printData( "Fre_Ctrl=L= %x", Fre_Ctrl);
										DP_printData( "Fre_Ctrl=H= %x", Fre_Ctrl>>8 );

				   			     			msWriteByteMask( REG_162F07, BIT5,BIT5 );           	                      //update Freq

										DPAudio_State =2;
										DP_printMsg("\r\n AudioUnmute ==Force state 2 ");
										DPRxAudioSSCEnCnt=0;
										EnableDPSSC();
									}

            }

    }
    }
               break;
        case 2:
    {

//if((DPAudioTestCount < 5)&&(DPAudioTestFlag == 1))
{
                    AudioError = msReadByte(REG_162F26)&0x7F;
		//    DP_printData("=2=>AudioError=%x ",AudioError);
		//    DP_printData(" FrH=%x ",msReadByte(REG_162F20));
		//    DP_printData(" FrL=%x ",msReadByte(REG_162F1F));
		//    DP_printData(" REG_162F07=%x ",msReadByte(REG_162F07));

//if((AudioError==0)&&(msRegs(REG_113336) == 1))
{
    //DPAudioTestCount++;
}

}
		      if(!(AudioError == 1))
				msWriteByteMask( REG_162F07, 0,BIT5 );

			  if(DPRxAudioMuteEn==0 && AudioError==1)
			  {
					DP_printMsg("\r\n ==2== RTN state1 ");
					DPAudioSSCInit(PortToCombo[port]);

			  }
                    if(AudioError == 0)
                    {
			   DPRxPolling3Cnt=0;
			//===========================================================
			//          Odin2  no need
			//============================================================
		  	  #if (CHIP_ID!=MST9U2)
                        if((msReadByte(REG_1630B1)&(BIT0|BIT1))&&(msReadByte(REG_162F27)&(BIT4|BIT5)))
                        {
                            #if 1
                                 for(i=0;i<DP_COMBO;i++)
                                {
                                    if( gDPCombo[i].AudioEn )
                                   {
                                        DP_printMsg("\r\n==2==Audio Mute ");
                                        EnableDPMute();
                                        DPRxAudioMuteEn = 1;
                                    }
                                }
                                				 {
									WORD Fre_Ctrl;
									WORD temp;
									BYTE   temp1;
									temp = msReadByte(  REG_162F09 )<<8;
									Fre_Ctrl =  temp + msReadByte( REG_162F0A);
									temp1 = (msReadByte(REG_162F22)&0x0F) + 1;
									 if(msReadByte(REG_162F27)&BIT4)  // speed up error
									{
										Fre_Ctrl = Fre_Ctrl - (Fre_Ctrl >> temp1);
										msWriteByte( REG_162F08,  0x00 );
										DP_printMsg("\r\n  Under run ");

									}
									else
									{
										Fre_Ctrl = Fre_Ctrl + (Fre_Ctrl >> temp1);
										msWriteByte( REG_162F08,  0x00 );
										DP_printMsg("\r\n  Over run ");
									}
									msWriteByte( REG_162F09,  Fre_Ctrl );
									msWriteByte( REG_162F0A,  (Fre_Ctrl >> 8));
								//	DP_printData( "Fre_Ctrl=L= %x", Fre_Ctrl);
								//	DP_printData( "Fre_Ctrl=H= %x", Fre_Ctrl>>8 );
							             }
					DisableDPSSC();
					DPRxAudioSSCEn= 0;
					msWriteByteMask( REG_162F07, BIT5,BIT5 );
					msWriteByteMask( REG_162F2A, 0 ,BIT4 );
					msWriteByteMask( REG_162F2A, BIT4 ,BIT4 );
					EnableDPSSC();
					DPRxAudioSSCEn= 1;
					DP_printMsg("\r\n=2=>Toggle SSC ");
                                 #endif
                        }
                        #endif

                        if(!(msReadByte(REG_1630B1)&(BIT0|BIT1)))   // Unmute
			   {
			   			//DP_printData("=2=>unmute_cnt=%x ",DPRxPolling3FIFOCnt);
						//DP_printData("=2=>Error=%x ",AudioError);
						//DP_printData("REG_162F07=%x",msReadByte(REG_162F07));
                            if(++DPRxPolling3FIFOCnt > (10*Const))
				{
                                DPRxPolling3FIFOCnt=0;
                                if(DPRxAudioMuteEn)
				      {   DP_printMsg("\r\n=2=>3 ");
                                    for(i=0;i<DP_COMBO;i++)
                                   {
                                    if( gDPCombo[i].AudioEn )
                                    {   DP_printMsg("\r\n=======================2=>Audio unMute ");
                                         DisableDPMute();
                                         DPRxAudioMuteEn = 0;

						DPAudioTestFlag = 1;
						DPAudioTestCount = 0;

                                    }
                                   }
                                }
                            }
                        }
                        else
                        {
                             DPRxPolling3FIFOCnt=0;
                             DP_printMsg("\r\n=2=>Audio FIFO Error ");
                             msWriteByteMask(REG_1630B1,BIT0|BIT1,BIT0|BIT1);
                        }
                    }
                    else
                    {
                            DP_printMsg("\r\n==2===> DPRxFreCtrl Equel ");
                            ++DPRxPolling3Cnt;
	                            if(DPRxPolling3Cnt >= (250*Const1))
					{
							DPRxPolling3Cnt=0;
					DP_printMsg("\r\n==2===> Audio Polling 3 Time out ");
					DisableDPSSC();
					DPRxAudioSSCEn= 0;
					msWriteByteMask( REG_162F2A, 0 ,BIT4 );
					msWriteByteMask( REG_162F2A, BIT4 ,BIT4 );
					msWriteByteMask(REG_1630B1,BIT0|BIT1,BIT0|BIT1);
					EnableDPSSC();
					DPRxAudioSSCEn= 1;
							            {
									WORD Fre_Ctrl;
									WORD temp;
									BYTE   temp1;

									temp = msReadByte(  REG_162F20 )<<8;
									Fre_Ctrl =  temp + msReadByte( REG_162F1F);

									temp1 = (msReadByte(REG_162F22)&0x0F) + 1;
									if(DPAudioUpDown)	// speed down
									{
										Fre_Ctrl = Fre_Ctrl + (Fre_Ctrl >> temp1);
										msWriteByte( REG_162F08,  0xff );
										DP_printMsg("\r\n  SD for TO ");
							}
									else
									{
									Fre_Ctrl = Fre_Ctrl - (Fre_Ctrl >> temp1);
									msWriteByte( REG_162F08,  0x00 );
									DP_printMsg("\r\n  SU for TO ");
									}
									msWriteByte( REG_162F09,  Fre_Ctrl );
									msWriteByte( REG_162F0A,  (Fre_Ctrl >> 8));
									DP_printData( "Fre_Ctrl=L= %x", Fre_Ctrl);
									DP_printData( "Fre_Ctrl=H= %x", Fre_Ctrl>>8 );
						}
									msWriteByteMask( REG_162F07, BIT5,BIT5 );           	   //update Freq

				}
		     }

		}
			   break;

		case 3:
				{
					DP_printData("\r\n 162F07=%x ",msReadByte(REG_162F07));
					AudioError = msReadByte(REG_162F26)&0x7F;
					DP_printData("\r\n=3=>AudioError=%x ",AudioError);
					DP_printData("\r\n FrH=%x ",msReadByte(REG_162F20));
					DP_printData("\r\n FrL=%x ",msReadByte(REG_162F1F));
					DP_printData("\r\n DPRxAudioSSCEn=%x ",DPRxAudioSSCEn);

						if((msReadByte(REG_162F2B)&BIT7)&&(DPRxAudioSSCEn==0))
		   			     	{

							EnableDPSSC();
							DPRxAudioSSCEn= 1;
						}
						else if(DPRxAudioSSCEn== 0)
						{
							break;
						}
						if(AudioError & BIT0)
						{
						 	break;
						}
						else
						{
							//DPAudioTimeOut =0;
							msWriteByteMask( REG_162F07, 0,BIT5 );
						}

					if(AudioError == 0)
					{
						//===========================================================
						//          Odin2  no need
						//============================================================
						#if (CHIP_ID!=MST9U2)
						if((msReadByte(REG_1630B1)&(BIT0|BIT1))&&(msReadByte(REG_162F27)&(BIT4|BIT5)))
						{
							#if 1
							 for(i=0;i<DP_COMBO;i++)
							 {
								if( gDPCombo[i].AudioEn )
								{	DP_printMsg("\r\n==3==Audio Mute ");
									EnableDPMute();
									DPRxAudioMuteEn = 1;
								}
							 }
							DisableDPSSC();
							DPRxAudioSSCEn= 0;
							msWriteByteMask( REG_162F07, BIT5,BIT5 );
							msWriteByteMask( REG_162F2A, 0 ,BIT4 );
							msWriteByteMask( REG_162F2A, BIT4 ,BIT4 );

							DP_printMsg("\r\n=3=>Toggle SSC ");
						   #endif
						}
						#endif
						if(!(msReadByte(REG_1630B1)&(BIT0|BIT1)))   // Unmute
						{
							if(++DPRxPolling3FIFOCnt > (10*Const))
							{
								DPRxPolling3FIFOCnt=0;
							    if(DPRxAudioMuteEn)
							    {
								    for(i=0;i<DP_COMBO;i++)
								   {
									if( gDPCombo[i].AudioEn )
									{	DP_printMsg("\r\n=======================3=>Audio unMute ");
										DisableDPMute();
										DPRxAudioMuteEn = 0;

									}
								   }
							    }
							}
						}
						else
						{
							 DPRxPolling3FIFOCnt=0;
							 DP_printMsg("\r\n=3=>Audio FIFO Error ");
							 msWriteByteMask(REG_1630B1,BIT0|BIT1,BIT0|BIT1);
						}
					}
					else
					{
						WORD DPRxFreCtrl=(msReadByte(REG_162F20) <<8) + msReadByte(REG_162F1F);
						if(DPRxFreCtrl == DPRxFreCtrl_Old)
						{
							DP_printMsg("\r\n==3===> DPRxFreCtrl Equal ");
							++DPRxPolling3Cnt;
							DP_printData("\r\n=3=>DPRxPolling3Cnt=%x ",DPRxPolling3Cnt);
							if(DPRxPolling3Cnt >= (100*Const1))
							{


								WORD Fre_Ctrl;
								WORD temp;
								BYTE   temp1;


								temp = msReadByte(  REG_162F20 )<<8;
								Fre_Ctrl =  temp + msReadByte( REG_162F1F);
								temp1 = (msReadByte(REG_162F22)&0x0F) + 1;

								if(AudioError & BIT6)
								{
									Fre_Ctrl = Fre_Ctrl + (Fre_Ctrl >> temp1);
									DP_printMsg("\r\n  Under run ");
									msWriteByte( REG_162F08, 0xff );
								}
								else
								{
									Fre_Ctrl = Fre_Ctrl - (Fre_Ctrl >> temp1);
									DP_printMsg("\r\n  Over run ");
									msWriteByte( REG_162F08, 0x00 );
								}
								msWriteByte( REG_162F09,  Fre_Ctrl );
								msWriteByte( REG_162F0A,  (Fre_Ctrl >> 8));
								DP_printData( "Fre_Ctrl=L= %x", Fre_Ctrl);
								DP_printData( "Fre_Ctrl=H= %x", Fre_Ctrl>>8 );

                                				msWriteByteMask( REG_162F07, BIT5,BIT5 );                  //update Freq
								DPRxPolling3Cnt=0;
								DP_printMsg("\r\n==3===> Audio Polling 3 Time out ");
								DisableDPSSC();
								DPRxAudioSSCEn= 0;
								msWriteByteMask( REG_162F2A, 0 ,BIT4 );
								msWriteByteMask( REG_162F2A, BIT4 ,BIT4 );
                                    			msWriteByteMask(REG_1630B1,BIT0|BIT1,BIT0|BIT1);
                            }
                        }
                        else
                        {
                            DPRxPolling3Cnt =0;
                        }
                        DPRxFreCtrl_Old = DPRxFreCtrl;
                    }
                }

               break;
       }
#endif
    }
//**************************************************************************
//  [Function Name]:
//                  DPAudioMuteCheck()
//  [Description]
//                  DPAudioMuteCheck
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
#define Audio_Sample_Time     0    //ms
void  DPAudioMuteCheck(void)
{


    if(msReadByte(REG_1630B0)&(BIT0|BIT1))
    {
	// msWriteByteMask( REG_162F07, BIT5,BIT5 );           //update Freq
	 //msWriteByteMask( REG_162F07, 0,BIT5);                 //update Freq
        //DP_printMsg("\r\nAudio Sample / Parity Error ");
        msWriteByteMask(REG_1630B0,BIT0|BIT1,BIT0|BIT1);
    }
    #if !DPSSC
    if(msReadByte(REG_1630B1)&(BIT0|BIT1))
    {
     msWriteByteMask(REG_1630B1,BIT0|BIT1,BIT0|BIT1);
    }
    #endif
    if(msReadByte(REG_1133E7+port*HBR2_OFFSET )&(BIT0))
    {
        //DP_printMsg("\r\nNo Ls Clock");
        msWriteByte(REG_1133EB+port*HBR2_OFFSET ,BIT0);
    }
    if(msReadByte(REG_1133E7+port*HBR2_OFFSET )&(BIT2))
    {
        // DP_printMsg("\r\n Linke Rate Change");
        msWriteByte(REG_1133EB+port*HBR2_OFFSET ,BIT2);
    }




}

//**************************************************************************
//  [Function Name]:
//                  DPRxFastTrainingInitial()
//  [Description]
//                  DPRxFastTrainingInitial
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void DPRxFastTrainingInitial( BYTE temp )
{
    DPRxSetSyntheizer( DP_HBR, temp );
}

//**************************************************************************
//  [Function Name]:
//                  DPRxAUXInit()
//  [Description]
//                  DPRxAUXInit
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void DPRxAUXInit(BYTE temp )
{
#if DP_AUX_XTAL
    #if (CHIP_ID==MST9U2)
    msWriteByteMask( REG_000392, BIT1, BIT1);
    #else
    msWriteByteMask( REG_000392, 0, BIT1);
    #endif
    msWriteByteMask( REG_000392, BIT2,BIT2|BIT3);                                                                           // Aux Clock SRC
    msWriteByteMask( REG_000390, 0,BIT5|BIT4 );        // DDC Clock use Xtal.
#else
    msWriteByteMask( REG_000392, 0, BIT2|BIT3);
    msWriteByteMask( REG_000390, BIT4,BIT5|BIT4 );   // DDC Clock use Xtal.
#endif

#if 0  //move to DPVersionSetting()
  #if Support_HBR2
   msWriteByte( REG_001100+temp*COMBO_PHY_OFFSET, 0x12);      // DP12
   msWriteByte( REG_001101+temp*COMBO_PHY_OFFSET, 0x14 );     //cyc : 5.4G                                                           // Support 1.62G
   msWriteByte( REG_001103+temp*COMBO_PHY_OFFSET, 0x8B);      // Support TP3
 #else
   msWriteByte( REG_001100+temp*COMBO_PHY_OFFSET, 0x11);     // DP11
   msWriteByte( REG_001101+temp*COMBO_PHY_OFFSET, 0x0A );    //cyc : 2.7G
   msWriteByte( REG_001103+temp*COMBO_PHY_OFFSET, 0x89);     // Not supprot TP3
 #endif
#endif

#if DP_OUI
    msWriteByteMask( REG_001104+temp*COMBO_PHY_OFFSET,  BIT0, BIT0);                                              // Aux UI Count	
#endif    
    msWriteByte( REG_001112+temp*COMBO_PHY_OFFSET, 0x0E );                                              // Aux UI Count
    msWriteByte( REG_001113+temp*COMBO_PHY_OFFSET, 0x0B );                                              // Aux Sample Rate
    msWriteByte( REG_001114+temp*COMBO_PHY_OFFSET, 0x00 );                                              // Aux Time Out Low
    msWriteByte( REG_001115+temp*COMBO_PHY_OFFSET, 0x08 );                                              // Aux Time Out  High
    msWriteByteMask( REG_001115+temp*COMBO_PHY_OFFSET, 0, BIT5|BIT6);                           // Aux Clk Src
    msWriteByte( REG_00112C+temp*COMBO_PHY_OFFSET, 0x0E );                                              // Aux UI Count
    msWriteByte( REG_00112D+temp*COMBO_PHY_OFFSET, 0x0B );                                              // Aux Sample Rate
    msWriteByte( REG_00112E+temp*COMBO_PHY_OFFSET, 0x00 );                                              // Aux Time Out Low
    msWriteByte( REG_00112F+temp*COMBO_PHY_OFFSET, 0x08 );                                              // Aux Time Out  High
    msWriteByte( REG_001130+temp*COMBO_PHY_OFFSET, 0x00);                                               // Aux Reply BY HW
    msWriteByteMask( REG_001131+temp*COMBO_PHY_OFFSET, BIT1, BIT0|BIT1);                           // Aux Time out Reply // Time out reply defer
    msWriteByte( REG_001132+temp*COMBO_PHY_OFFSET, 0xD0);                                               // Aux EDID address

   // msWriteByte( REG_001103+temp*COMBO_PHY_OFFSET, 0x8B);                                                // TP3 Support // Not Support EDP /Enhance frame mode
    msWriteByteMask( REG_001142+temp*COMBO_PHY_OFFSET, BIT0,BIT1);                                // GUID EN
    msWrite2Byte( REG_0011A8+temp*COMBO_PHY_OFFSET, 0x1044 );                                         // ENABLE TX
    msWrite2Byte( REG_0011C2, 0x0000 );                                                      // Off-line EN = 0
    msWrite2Byte( REG_0011C0, 0x2020 );                                                 // SQ Debunce
    msWriteByteMask( REG_001142+temp*COMBO_PHY_OFFSET, BIT0, BIT0 );                     // Enable GUID Read/Write
    msWriteByteMask( REG_00119C+temp*COMBO_PHY_OFFSET, BIT4|BIT7, BIT4|BIT5|BIT6|BIT7 );  // V oltage Swim = 2 and PRE-emph =1

    msWriteByteMask( REG_00119A+temp*COMBO_PHY_OFFSET, BIT1, BIT1 );                                // MCCS reply By MCU
    //msWrite2Byte(REG_0011C2 , 0x0000 );                                                  // Off - line =0
    //==============================================================================
    msWriteByteMask( REG_0017B9, (BIT0<<temp), (BIT0<<temp));                       //ENable AUX0 ~ AUX5 Interrupt
    DPRxSetAUXMask(temp);
    DPRxProgramDPCD(temp);
}

//**************************************************************************
//  [Function Name]:
//                  DPRxProgramDPCD()
//  [Description]
//                  DPRxProgramDPCD
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void  DPRxProgramDPCD(BYTE temp)
{

    msWriteByteMask( REG_00117D+temp*COMBO_PHY_OFFSET, BIT2, BIT2 );
    msWrite2Byte( REG_001180+temp*COMBO_PHY_OFFSET,0x30);
    msWrite2Byte( REG_001182+temp*COMBO_PHY_OFFSET,0x00);

  // msWriteByteMask( REG_00117D+temp*COMBO_PHY_OFFSET, BIT3, BIT3 );
  // msWrite2Byte( REG_001184+temp*COMBO_PHY_OFFSET,0x010A);
  // msWrite2Byte( REG_001186+temp*COMBO_PHY_OFFSET,0x0000);
#if DP_HDCP_Reset
    msWriteByteMask( REG_00117D+temp*COMBO_PHY_OFFSET, BIT4, BIT4 );   // 0x6800B
    msWriteByte( REG_001188+temp*COMBO_PHY_OFFSET,0x0B);
    msWriteByte( REG_001189+temp*COMBO_PHY_OFFSET,0x80);
    msWriteByteMask( REG_00118A+temp*COMBO_PHY_OFFSET,0x06,0x0F);
#endif



}

//**************************************************************************
//  [Function Name]:
//                  DPRxAUXInit()
//  [Description]
//                  DPRxAUXInit
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void DPRxEnSplit(E_DP_OP  output,  HBR2_SELECT temp)
{
    if(output == DP_1P)
    {
        msWriteByteMask( REG_113559+temp*HBR2_OFFSET, 0,BIT5);
        msWriteByteMask( REG_1630E1+temp*4, 0,BIT5);

        if( temp ==0 )
            msWriteByteMask(REG_162F97,  BIT0, BIT0);                               // HBR0 ==> HBR2 1p ou
        else if (temp == 1)
            msWriteByteMask(REG_162FB7,  BIT0 ,BIT0);                                   // HBR1 ==> HBR2 1p ou
    }
    else if(output == DP_2P)
    {
        msWriteByteMask( REG_113559+temp*HBR2_OFFSET, BIT5,BIT5);
        msWriteByteMask( REG_1630E1+temp*4, BIT5,BIT5);

        if( temp ==0 )
            msWriteByteMask(REG_162F97,  0, BIT0);                                         // HBR0 ==> HBR2 1p ou
        else if (temp == 1)
            msWriteByteMask(REG_162FB7,  0 , BIT0);                                 // HBR1 ==> HBR2 1p ou
    }
}

//**************************************************************************
//  [Function Name]:
//                  DPSetHBR2_0()
//  [Description]
//                  DPSetHBR2_0
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void DPRxSetHBR2_0(void)
{
    msWriteByteMask(REG_1630DE ,  DP_HBR2_0, BIT0|BIT1|BIT2);                       //HBR2 port0  for Port 0
    msWriteByteMask(REG_1630DF ,  DP_HBR2_0, BIT0|BIT1|BIT2);                       //HBR2 port1  for Port 1
    msWriteByteMask(REG_1630E0 ,  DP_HBR2_0, BIT0|BIT1|BIT2);                       // Clock_pixel

    //msWriteByteMask(REG_1630E1 ,  DP_HBR2_0, BIT0|BIT1|BIT2);                             //MAC
    msWriteByteMask(REG_1630E1 ,  0, BIT0|BIT1|BIT2);                                   //MAC   // Fixed VPLL 0

    msWriteByteMask(REG_162F96,  (DP_HBR2_0+6)<<4, 0xF0);                            // HBR0 CLK
    //msWriteByteMask(REG_1630EC, (DP_HBR2_0+6)<<4,0xF0);                         // VPLL 0 mux
    msWriteByteMask(REG_1630EC, (0+6)<<4,0xF0);                                     // VPLL 0 mux // Fixed VPLL 0
    msWriteByteMask(REG_162F97,  BIT0, BIT0);                                                     // HBR0 ==> HBR2 1p ou

    msWrite2Byte( REG_163006, 0xFFFF );
    msWrite2Byte( REG_163008, 0x3FFF );
    msWriteByteMask( REG_1630E0, BIT3|BIT5|BIT6, BIT3|BIT5|BIT6 );
    msWriteByteMask( REG_1630E1, BIT3, BIT3 );
#if DP_AUX_XTAL
    #if (CHIP_ID==MST9U2)
    msWriteByteMask( REG_1630E2, BIT2|BIT0, BIT3|BIT2|BIT1|BIT0 );                       // Not inver AUX MAC clock FROM XTAL
    #else
    msWriteByteMask( REG_1630E2, BIT3|BIT2|BIT0, BIT3|BIT2|BIT1|BIT0 );              //  Inver AUX
    #endif
#else
    msWriteByteMask( REG_1630E2, BIT2, BIT2|BIT1|BIT0 );                                          // MAC clock FROM OSC
#endif

    msWriteByteMask( REG_1630EE, BIT0|BIT1,  BIT0|BIT1 );                       //reg_clk_combo2sc_en
    //msWriteByte( REG_1630F0, 0x66);                                                          //reg_combo2sc_clk_sel_0 / 1
    //msWriteByte( REG_1630E8, 0x76);
    msWrite2Byte( REG_163012, 0xFFFF);
    msWrite2Byte( REG_163014, 0x3FFF);

    msWriteByteMask( REG_162F82, 0,BIT0 );                                   //VPLL 0 PD diable
    msWriteByteMask( REG_162F83, 0,BIT0  );

}

//**************************************************************************
//  [Function Name]:
//                  DPSetHBR2_1()
//  [Description]
//                  DPSetHBR2_1
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void DPRxSetHBR2_1(void)
{
    msWriteByteMask(REG_1630DE ,  DP_HBR2_1<<4, BIT6|BIT5|BIT4);                 //HBR2 port0  for Port 0
    msWriteByteMask(REG_1630DF ,  DP_HBR2_1<<4, BIT6|BIT5|BIT4);                 //HBR2 port1  for Port 1
    msWriteByteMask(REG_1630E4 ,  DP_HBR2_1, BIT0|BIT1|BIT2);                       // Clock_pixel

    //msWriteByteMask(REG_1630E5 ,  DP_HBR2_1, BIT0|BIT1|BIT2);                // MAC
    msWriteByteMask(REG_1630E5 ,   1, BIT0|BIT1|BIT2);                         // MAC        // Fixed VPLL 1

    msWriteByteMask(REG_162FB6 ,  (DP_HBR2_1+6)<<4, 0xF0);                            // HBR1 CLK  SRC
    msWrite2ByteMask(REG_1630EC , (1+6)<<7, 0x0380);                            // VPLL 1 mux    // Fixed VPLL 1

    msWriteByteMask(REG_162FB7 ,  BIT0 ,BIT0);                                          //  HBR1 ==> HBR2 1p ou

    msWriteByteMask( REG_1630E4, BIT3|BIT5|BIT6, BIT3|BIT5|BIT6 );
    msWriteByteMask( REG_1630E5, BIT3, BIT3 );
#if DP_AUX_XTAL
    #if (CHIP_ID==MST9U2)
    msWriteByteMask( REG_1630E6, BIT2|BIT0, BIT3|BIT2|BIT1|BIT0 );                       // Not inver AUXMAC clock FROM XTAL
    #else
    msWriteByteMask( REG_1630E6, BIT3|BIT2|BIT0, BIT3|BIT2|BIT1|BIT0 );          // Inverter AUX
    #endif
#else
    msWriteByteMask( REG_1630E6, BIT2, BIT2|BIT1|BIT0 );                             // MAC clock FROM OSC
#endif
    msWriteByteMask( REG_1630EE, BIT0|BIT1,  BIT0|BIT1 );                       //reg_clk_combo2sc_en

    //msWriteByte( REG_1630F0, 0x66);                                                          //reg_combo2sc_clk_sel_0 / 1
    //msWriteByte( REG_1630E8, 0x76);

    msWriteByteMask( REG_162FA2, 0,BIT0 );                                   //VPLL 1 PD diable
    msWriteByteMask( REG_162FA3, 0,BIT0  );
}

//**************************************************************************
//  [Function Name]:
//                  DPRxGetVB_ID(Bool bEnable)
//  [Description]
//                  DPRxGetVB_ID
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE DPRxGetVB_ID(BYTE InputPort)
{
    BYTE bVBIDFlag = FALSE;
    BYTE ucHBR2Select = mdrv_DP_GetHBR2PortSelect(InputPort);   //scaler Port to Combo

    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
	 bVBIDFlag = gDPComboInfo[InputPort].bDPInterlace;
    }
    else // HBR2 path
    {
        bVBIDFlag = gDPInfo[ucHBR2Select].bDPInterlace;
    }

    return bVBIDFlag;
}

//**************************************************************************
//  [Function Name]:
//                  DPRxIRQComboEnable(Bool bEnable)
//  [Description]
//                  DPRxIRQComboEnable
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void DPRxIRQComboEnable( Bool bEnable, BYTE HBR2_Port )
{
     WORD i=0;

     i = HBR2_Port * HBR2_OFFSET;

    if( bEnable )
    {
        msWriteByteMask(REG_1133E0+i , 0 , BIT0|BIT1);       //[0] P1 [2] P2
        msWriteByteMask(REG_1133E1+i , 0 , BIT0);        //[0] CDR loss
        msWriteByteMask(REG_1133E3+i , 0 , BIT3);        //[3] VUPLL Big CHG
        msWriteByteMask(REG_1133E3+i , 0 , BIT1);        //[1] AUPLL Big CHG
     msWriteByteMask(REG_1133E2+i , 0 , BIT3);       //[3] Interlaneskew
        DP_printMsg( " DP Combo IRQ Enable!");
        msWriteByteMask(REG_163070 ,(BIT6<<HBR2_Port) , (BIT6<<HBR2_Port));      // Enable Port 0 interrupt
    }
    else
    {
        DP_printMsg( " DP Combo IRQ Disable!");
        msWriteByteMask(REG_163070 ,0 ,(BIT6<<HBR2_Port));                             // Disable Port 0 interrupt
    }
}

//**************************************************************************
//  [Function Name]:
//                  DPRxClearAUXStatus()
//  [Description]
//                  DPRxClearAUXStatus
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void DPRxSetAUXMask(BYTE temp)
{
    msWriteByte( REG_001179+temp*COMBO_PHY_OFFSET, 0xFF );                           // Clear Interrupt States
    msWriteByte( REG_001179+temp*COMBO_PHY_OFFSET, 0x00 );

    msWriteByte( REG_001192+temp*COMBO_PHY_OFFSET, 0x0F );                           // Clear dpcd0_irq_mask Interrupt States
    msWriteByte( REG_001192+temp*COMBO_PHY_OFFSET, 0x00 );

    msWriteByteMask( REG_001193+temp*COMBO_PHY_OFFSET, BIT7, BIT7);                      // Clear __aksv_irq_mask
    msWriteByteMask( REG_001193+temp*COMBO_PHY_OFFSET, 0, BIT7);

    msWriteByteMask( REG_001196+temp*COMBO_PHY_OFFSET, BIT2, BIT2);                           // Clear_SQ
    msWriteByteMask( REG_001196+temp*COMBO_PHY_OFFSET, 0, BIT2);

}


//**************************************************************************
//  [Function Name]:
//                  DPRxComboPortConvert(BYTE InputPort)
//  [Description]
//                  DPRxComboPortConvert
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE DPRxComboPortConvert(BYTE InputPort)
{
    BYTE temp = ComboToPort[InputPort];
    if(temp == 0xFF)
        return  0;
    else
        return temp;
}

//**************************************************************************
//  [Function Name]:
//                  DPRxSetAUPLL(Byte DPRxSetAUPLL)
//  [Description]
//                  DPRxSetAUPLL
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void DPRxSetAUPLL(BYTE ucComboPort)
{
    #if DPSSC
    BYTE ucHBR2Select = mdrv_DP_GetHBR2PortSelect(ucComboPort);
    #endif
    gDPInfo[HBRPort(ucComboPort)].bDPAUPLLBIGChange = 1;
    //HBR_IP_SELECT BYTE temp = ComboToPort[ucComboPort];
    DP_printMsg("=====DPRxAuPll");
    msWriteByteMask( REG_162F11, 0,BIT7 );             // Turn off Bypasse mode
    msWriteByteMask( REG_162F07, BIT5,BIT5 );           //update Freq
    msWriteByteMask( REG_162F07, 0,BIT5);                 //update Freq
    #if DPSSC
    msWriteByteMask( REG_162F22, 0,BIT5 );
    DPAudioSSCInit(ucComboPort);
    #endif
    msWriteByteMask( REG_162F2A, BIT4 ,BIT4 );
    #if DPSSC
    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
    	    #if DP_COMBO_PORT
    	    WORD DP_Combo_Offset=0;
    	    DP_Combo_Offset = DPRxComboOffset(ucComboPort);

		gDPComboInfo[ucComboPort].bDPAUPLLBIGChange = TRUE;
   		DP_printMsg("==Combo2===Oven M/N");
   		msWriteByteMask(REG_160310+DP_Combo_Offset,0,BIT0);
		msWriteByte(REG_160616+DP_Combo_Offset, msReadByte(REG_1603C0+DP_Combo_Offset)); //MSA MAUD_CENTER[7:0]
		msWriteByte(REG_160617+DP_Combo_Offset, msReadByte(REG_1603C1+DP_Combo_Offset)); //MSA MAUD_CENTER[7:0]
		msWriteByte(REG_160618+DP_Combo_Offset, msReadByte(REG_1603C2+DP_Combo_Offset)); //MSA MAUD_CENTER[7:0]

		msWriteByte(REG_16061A+DP_Combo_Offset, msReadByte(REG_1603C3+DP_Combo_Offset)); //MSA MAUD_CENTER[7:0]
		msWriteByte(REG_16061B+DP_Combo_Offset, msReadByte(REG_1603C4+DP_Combo_Offset)); //MSA MAUD_CENTER[7:0]
		msWriteByte(REG_16061C+DP_Combo_Offset, msReadByte(REG_1603C5+DP_Combo_Offset)); //MSA MAUD_CENTER[7:0]
   		msWriteByteMask(REG_161110+DP_Combo_Offset,BIT0,BIT0);
		msWriteByteMask(REG_16060D+DP_Combo_Offset, BIT7,BIT7);	        //Oven
   		DP_printData( "==>Path %x", msReadByte(REG_163088+DP_Combo_Offset) );
   		msWriteByteMask(REG_160607+DP_Combo_Offset, BIT2,BIT2);    //Oven
		msWriteByteMask(REG_160607+DP_Combo_Offset, 0,BIT2);	        //Oven
		//DP_printData( "==>FBDIV %x", msReadByte(REG_162F04) );
		//DP_printData( "==>FBDIV %x", msReadByte(REG_162F05) );
	 #endif
    }
    else // HBR2 path
    {
        BYTE utemp=0;

   	utemp = msReadByte(  REG_11330E+HBRPort(ucComboPort)*HBR2_OFFSET );

        if(( utemp & 0x0F ) == 0x06 )
        {
            msWriteByteMask(REG_162F40, ((ucComboPort)<<4), 0xF0);
            msWriteByteMask(REG_162F41, 0, BIT0);
        }
        else  if(( utemp & 0x0F ) == 0x0A)
        {
            msWriteByteMask(REG_162F40, ((ucComboPort)<<4), 0xF0);
            msWriteByteMask(REG_162F41, 0, BIT0);
        }
        else  if(( utemp & 0x1F ) == 0x14)
        {
            msWriteByteMask( REG_162F40, ((ucComboPort)+6) <<4 , 0xF0);
            msWriteByteMask(REG_162F41, BIT0, BIT0);
        }

		gDPInfo[HBRPort(ucComboPort)].bDPAUPLLBIGChange  = TRUE;
		DP_printMsg("===HBR2==Oven M/N");
		msWriteByte(REG_1134C8+HBRPort(ucComboPort)*HBR2_OFFSET, msReadByte(REG_11338E+HBRPort(ucComboPort)*HBR2_OFFSET )); //MSA MAUD_CENTER[7:0]
		msWriteByte(REG_1134C9+HBRPort(ucComboPort)*HBR2_OFFSET, msReadByte(REG_11338F+HBRPort(ucComboPort)*HBR2_OFFSET )); //MSA MAUD_CENTER[7:0]
		msWriteByte(REG_1134CA+HBRPort(ucComboPort)*HBR2_OFFSET, msReadByte(REG_113390+HBRPort(ucComboPort)*HBR2_OFFSET )); //MSA MAUD_CENTER[7:0]

		msWriteByte(REG_1134CB+HBRPort(ucComboPort)*HBR2_OFFSET, msReadByte(REG_113392+HBRPort(ucComboPort)*HBR2_OFFSET )); //MSA MAUD_CENTER[7:0]
		msWriteByte(REG_1134CC+HBRPort(ucComboPort)*HBR2_OFFSET, msReadByte(REG_113393+HBRPort(ucComboPort)*HBR2_OFFSET )); //MSA MAUD_CENTER[7:0]
		msWriteByte(REG_1134CD+HBRPort(ucComboPort)*HBR2_OFFSET, msReadByte(REG_113394+HBRPort(ucComboPort)*HBR2_OFFSET )); //MSA MAUD_CENTER[7:0]


		msWriteByteMask(REG_1134D5+HBRPort(ucComboPort)*HBR2_OFFSET , BIT5,BIT5);    //Oven
		msWriteByteMask(REG_1134B3+HBRPort(ucComboPort)*HBR2_OFFSET , BIT2,BIT2);    //Oven
		msWriteByteMask(REG_1134B3+HBRPort(ucComboPort)*HBR2_OFFSET , 0,BIT2);	      //Oven

		//DP_printData( "==>FBDIV %x", msReadByte(REG_162F04) );
		//DP_printData( "==>FBDIV %x", msReadByte(REG_162F05) );
    }
    #endif

}
//**************************************************************************
//  [Function Name]:
//                  DPRxSResetAudioMN(void)
//  [Description]
//                  DPRxSResetAudioMN
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void DPRxResetAudioMN(void)
{
    msWriteByte(  REG_1133AE+port*HBR2_OFFSET, 0 ); //UPDATE MAUD_CENTER[7:0]
    msWriteByte(  REG_1133AF+port*HBR2_OFFSET, 0 ); //UPDATE MAUD_CENTER[15:8]
    msWriteByte(  REG_1133B0+port*HBR2_OFFSET, 0 ); //UPDATE MAUD_CENTER[23:16]
    msWriteByte(  REG_1133B6+port*HBR2_OFFSET, 0 ); //UPDATE NAUD_CENTER[7:0]
    msWriteByte(  REG_1133B7+port*HBR2_OFFSET, 0 ); //UPDATE NAUD_CENTER[15:8]
    msWriteByte(  REG_1133B8+port*HBR2_OFFSET, 0 ); //UPDATE NAUD_CENTER[23:16]
}


//**************************************************************************
//  [Function Name]:
//                  DPRXProgramDPCD_ISR1(void)
//  [Description]
//                  DPRXProgramDPCD_ISR1
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
#if DP_HDCP_Reset
void DPRxProgramDPCD_ISR2(WORD uctemp)
{

    BYTE AuxData=0;
    BYTE RxCmd=0;
    BYTE RxLength=0;
    BYTE AuxIndex=0;

	msRegs(REG_1133A1+HBR2_OFFSET* HBRPort(uctemp) ) = msRegs(REG_1133A1+HBR2_OFFSET* HBRPort(uctemp)) | BIT4;   //HDCP reset
	msRegs(REG_1133A1+HBR2_OFFSET* HBRPort(uctemp) ) = msRegs(REG_1133A1+HBR2_OFFSET* HBRPort(uctemp)) & (~BIT4);  //HDCP reset.

    RxCmd = msRegs(REG_001138 + uctemp)&0x0F;

    msRegs(REG_001130 + uctemp)= msRegs(REG_001130 + uctemp)|_BIT1;
    if(RxCmd ==0x09 )    //DP Read
    {
            msRegs(REG_0011D3 + uctemp)= msRegs(REG_113368+ HBR2_OFFSET* HBRPort(uctemp) );
    }
    else if(RxCmd==0x08 || RxCmd==0x04)    //DP Write
    {
        msRegs(REG_0011FC + uctemp)= msRegs(REG_0011FC + uctemp)|_BIT2;
        AuxData= msRegs(REG_001175 + uctemp);
        {
            BYTE  temp3=255;
            msRegs(REG_0011FC + uctemp)= msRegs(REG_0011FC + uctemp)|_BIT2;
            while(temp3--){};
             msRegs(REG_113368)   = msRegs(REG_001175 + uctemp);
        }
    }
    msRegs(REG_001179 + uctemp)= msRegs(REG_001179 + uctemp)|(_BIT4);
    msRegs(REG_001130 + uctemp)= msRegs(REG_001130 + uctemp)&(~_BIT1);

}

#endif
//**************************************************************************
//  [Function Name]:
//                  DPRXProgramDPCD_ISR1(void)
//  [Description]
//                  DPRXProgramDPCD_ISR1
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void DPRxProgramDPCD_ISR1(WORD uctemp)
{
    BYTE AuxData=0;
    BYTE RxCmd=0;
    BYTE RxLength=0;
    BYTE AuxIndex=0;

    msRegs(REG_001130 + uctemp)= msRegs(REG_001130 + uctemp)|_BIT1;
    if(RxCmd ==0x09 )    //DP Read
    {
        AuxIndex=0 ;
        while(RxLength--)
        {
           // msRegs(REG_11332A)= msRegs(REG_11332A)+1;
            msRegs(REG_0011D3 + uctemp)=GUIDData[AuxIndex];
            AuxIndex=AuxIndex+1;
        }
    }
    else if(RxCmd==0x08 || RxCmd==0x04)    //DP Write
    {       // BYTE  temp2=255;
        msRegs(REG_0011FC + uctemp)= msRegs(REG_0011FC + uctemp)|_BIT2;
        AuxData= msRegs(REG_001175 + uctemp);
        AuxIndex=0;
        RxLength = RxLength+1;
        while(RxLength--)
        {
            BYTE  temp3=255;
            msRegs(REG_0011FC + uctemp)= msRegs(REG_0011FC + uctemp)|_BIT2;
            while(temp3--){};
                GUIDData[AuxIndex++] = msRegs(REG_001175 + uctemp);
        }
              gDPInfo[HBRPort(uctemp/COMBO_PHY_OFFSET)].bDPGUID = TRUE;
    }
    msRegs(REG_001179 + uctemp)= msRegs(REG_001179 + uctemp)|(_BIT4);
    msRegs(REG_001130 + uctemp)= msRegs(REG_001130 + uctemp)&(~_BIT1);


}


//**************************************************************************
//  [Function Name]:
//                  DPRXProgramDPCD_ISR(void)
//  [Description]
//                  DPRXProgramDPCD_ISR
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void DPRxProgramDPCD_ISR0(WORD uctemp)
{
   // BYTE AuxData=0;
    BYTE RxCmd=0;
    BYTE RxLength=0;
    BYTE AuxIndex=0;
    RxCmd = msRegs(REG_001138 + uctemp)&0x0F;
    RxLength = msRegs(REG_00113A+ uctemp);
  
       
 
	    msRegs(REG_001130 + uctemp)= msRegs(REG_001130 + uctemp)|_BIT1;
	    if(RxCmd ==0x09 )    //DP Read
	    {    //msRegs(REG_11333B) =   msRegs(REG_11333B) +1;
	        AuxIndex=0 ;
	        while(RxLength--)
	        {
		     BYTE  temp3=100;
                    while(temp3--)
                    {
                        _nop_();
                    };

	           // msRegs(REG_0011D3 + uctemp)=GUIDData[AuxIndex];
	            msRegs(REG_0011D3 + uctemp)=msRegs(REG_001171 + uctemp - AuxIndex );
	            AuxIndex=AuxIndex+1;
	        }
	    }
	    else if(RxCmd==0x08 || RxCmd==0x04)    //DP Write
	    {       // BYTE  temp2=255;
	    	 //msRegs(REG_11333A) =   msRegs(REG_11333A) +1;
	       //msRegs(REG_0011FC + uctemp)= msRegs(REG_0011FC + uctemp)|_BIT2;
	       // msRegs(REG_11333C) = msRegs(REG_001175 + uctemp);
	        AuxIndex=0;
	        ///RxLength = RxLength+1;
	        while(RxLength--)
	        {
	            BYTE  temp3=100;
	            //msRegs(REG_11333C) =   msRegs(REG_11333C) +1;
	            msRegs(REG_0011FC + uctemp)= msRegs(REG_0011FC + uctemp)|_BIT2;
	            while(temp3--){ _nop_();};
	             { //  GUIDData[AuxIndex++] = msRegs(REG_001175 + uctemp);
				msRegs(REG_001162 + uctemp + AuxIndex) = msRegs(REG_001175 + uctemp);
				AuxIndex=AuxIndex+1;
	             }
	        }
	             // gDPInfo[HBRPort(uctemp/COMBO_PHY_OFFSET)].bDPGUID = TRUE;
	    }

	    msRegs(REG_001179 + uctemp)= msRegs(REG_001179 + uctemp)|(_BIT4);
	    msRegs(REG_001130 + uctemp)= msRegs(REG_001130 + uctemp)&(~_BIT1);
      
}

//**************************************************************************
//  [Function Name]:
//                  DPRXCheckDPMS(void)
//  [Description]
//                  DPRXCheckDPMS
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BOOL DPRXCheckDPMS(BYTE InputPort)
{
    BOOL bIgnoreDPMS = FALSE;
    BYTE ucHBR2Select = mdrv_DP_GetHBR2PortSelect(InputPort);   //scaler Port to Combo

    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {

    }
    else // HBR2 path
    {
        port = ucHBR2Select;

        if(((msReadByte(REG_1133A5+ucHBR2Select*HBR2_OFFSET ) & 0x08 ) ? 1 : 0)&&DPRxCheckLock()&&(msReadByte(REG_001172+ucHBR2Select*COMBO_PHY_OFFSET )&0x01))
        {
            bIgnoreDPMS = TRUE;
        }
    }

    return bIgnoreDPMS;
}

//**************************************************************************
//  [Function Name]:
//                  DPRXAutoEQOnOff(void)
//  [Description]
//                  DPRXAutoEQOnOff
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void DPRXAutoEQOnOff(Bool Enable, HBR2_SELECT HBR2PORT)
{
    if(Enable)
    {
     msRegs(REG_11204A+PortToCombo[HBR2PORT]*COMBO_PHY_OFFSET) = msRegs(REG_11204A+PortToCombo[HBR2PORT]*COMBO_PHY_OFFSET)|(BIT2);
     msRegs(REG_113000+HBR2PORT*HBR2_OFFSET) = msRegs(REG_113000+HBR2PORT*HBR2_OFFSET)|(BIT0);
    }
    else
    {
     msRegs(REG_11204A+PortToCombo[HBR2PORT]*COMBO_PHY_OFFSET) = msRegs(REG_11204A+PortToCombo[HBR2PORT]*COMBO_PHY_OFFSET)&(~BIT2);
     msRegs(REG_113000+HBR2PORT*HBR2_OFFSET) = msRegs(REG_113000+HBR2PORT*HBR2_OFFSET)&(~BIT0);
    }
}


//**************************************************************************
//  [Function Name]:
//                  DPRxAstroPTGInterlacePatch(BYTE InputPort)
//  [Description]
//                  DPRxAstroPTGInterlacePatch
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool DPRxAstroPTGInterlacePatch(BYTE InputPort)
{
    BYTE ucHBR2Select = mdrv_DP_GetHBR2PortSelect(InputPort);   //scaler Port to Combo

    if ((( msReadByte( REG_1133A5 + ucHBR2Select*HBR2_OFFSET) & BIT2 ) == 0)
        && ((gDPInfo[ucHBR2Select].uwDPHWidth%720==0 && gDPInfo[ucHBR2Select].uwDPVWidth==480/2 ) || (gDPInfo[ucHBR2Select].uwDPHWidth%720==0 && gDPInfo[ucHBR2Select].uwDPVWidth==576/2) || (gDPInfo[ucHBR2Select].uwDPHWidth==1920 && gDPInfo[ucHBR2Select].uwDPVWidth==1080/2))) //480i/576i/108oi
    {//override
        msWriteByteMask(REG_113541+ucHBR2Select*HBR2_OFFSET, BIT5|BIT4, BIT5|BIT4);

        return TRUE;
    }
    else
    {//restore
        msWriteByteMask(REG_113541+ucHBR2Select*HBR2_OFFSET, 0, BIT5|BIT4);

        return FALSE;
    }
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_DP_Set_ShortHPDFlag()
//  [Description]
//                  mdrv_DP_Set_ShortHPDFlag
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
#if ENABLE_DP_ShortHPD_PM
void DPRxSet_ShortHPDFlag(BYTE ucInputPort, Bool bEnable)
{
        bHPDShortPulse[ucInputPort] = bEnable;

}
#endif
//                  DPRxAstroPTGInterlaceFieldPatch(BYTE InputPort)
//  [Description]
//                  DPRxAstroPTGInterlaceFieldPatch
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool DPRxAstroPTGInterlaceFieldPatch(BYTE InputPort)
{
    BYTE ucHBR2Select = mdrv_DP_GetHBR2PortSelect(InputPort);   //scaler Port to Combo
    Bool bFieldFlag = 0, bFieldToggle = 0;
    BYTE ucFlagCount = 30;

    if(( msReadByte( REG_1133A5 + ucHBR2Select*HBR2_OFFSET) & BIT2 ) == BIT2)
    {
		bFieldFlag = ( msReadByte( REG_1133A5 + ucHBR2Select*HBR2_OFFSET) & BIT1 );
		
		while(ucFlagCount > 0)
		{
			Delay1ms(1);
		
			if(bFieldFlag != ( msReadByte( REG_1133A5 + ucHBR2Select*HBR2_OFFSET) & BIT1 ))
			{
				bFieldToggle = 1;
				break;				
			}
			
			ucFlagCount--;
		}

		if(bFieldToggle == 0)
		{//override
			msWriteByteMask(REG_113541+ucHBR2Select*HBR2_OFFSET, BIT5|BIT4, BIT5|BIT4);
		
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
//                  DPRxCheckInterlaceInverse(BYTE InputPort)
//  [Description]
//                  DPRxCheckInterlaceInverse
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void DPRxCheckInterlaceInverse(BYTE InputPort)
{
    Bool bFieldFlag = 0, bFieldToggle = 0;
    BYTE ucFlagCount = 30;
	DWORD temp=0, temp1=0;
	
    if(gDPInfo[InputPort].bDPInterlace)
    {
		bFieldFlag = ( msReadByte( REG_1133A5 + InputPort*HBR2_OFFSET) & BIT1 );
		
		while(ucFlagCount > 0)
		{
			Delay1ms(1);
		
			if(bFieldFlag != ( msReadByte( REG_1133A5 + InputPort*HBR2_OFFSET) & BIT1 ))
			{
				bFieldToggle = 1;
				break;				
			}
			
			ucFlagCount--;
		}
    }
    
 gDPInfo[InputPort].bDPInterlaceFiledToggleFlag = bFieldToggle;
//=================================
//	Interlace patch
//=================================
	 if(bFieldToggle)
	 {
		  temp = ((msReadByte( REG_113536+InputPort*HBR2_OFFSET )&0x0F) <<16)  + msRead2Byte( REG_113534+InputPort*HBR2_OFFSET );
		  temp1 = ((msReadByte( REG_11353A+InputPort*HBR2_OFFSET )&0x0F) <<16)  + msRead2Byte( REG_113538+InputPort*HBR2_OFFSET );

		  if(abs(temp-temp1) >20 )
		  { 			
			  if(!(msReadByte( REG_113541+InputPort*HBR2_OFFSET)&BIT2))
			  {
				  if(temp1 < temp )
					  msWriteByteMask( REG_113541+InputPort*HBR2_OFFSET, BIT2, BIT2 );
			  }
			  else
			  {
				  if(temp1 < temp )
				  msWriteByteMask( REG_113541+InputPort*HBR2_OFFSET, 0, BIT2 );
			  }
		  }
		  else
			  msWriteByteMask( REG_113541+InputPort*HBR2_OFFSET, 0, BIT2 );
	   }
	   else
	   {
		   msWriteByteMask( REG_113541+InputPort*HBR2_OFFSET, 0, BIT2 );
	   }

}

//**************************************************************************
//  [Function Name]:
//                  DPRxHVFSMCheck(BYTE InputPort)
//  [Description]
//                  DPRxHVFSMCheck
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void DPRxHVFSMCheck(BYTE InputPort)
{
	BYTE ubHs_gen_fsm_status;
	BYTE ubCheck_fsm_Times;
		
	ubHs_gen_fsm_status = 0x0;
	ubCheck_fsm_Times = 0x2;  //RD suggest we should check FSM 2 times
	
	//make sure hs_gen_fsm_status[3:0] only has 1 bit as 1, otherwise reset reg_hs_gen_fsm_rst
	while(ubCheck_fsm_Times > 0)
	{	   
		ubHs_gen_fsm_status = (msReadByte(REG_113524+InputPort*HBR2_OFFSET) & 0x0F);
	
		if(((ubHs_gen_fsm_status & (ubHs_gen_fsm_status - 1)) != 0x0) || (ubHs_gen_fsm_status == 0x0))
		{
			msWriteByteMask((REG_113521+InputPort*HBR2_OFFSET), BIT0, BIT0); //reg_hs_gen_fsm_rst
			Delay2us();
			msWriteByteMask((REG_113521+InputPort*HBR2_OFFSET), 0, BIT0);			
		}
		
		ubCheck_fsm_Times --;
	}

}

//**************************************************************************
//  [Function Name]:
//                  DPRxGetDRRFlag(BYTE InputPort)
//  [Description]
//                  DPRxGetDRRFlag
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool DPRxGetDRRFlag(BYTE InputPort)
{
    BYTE ucHBR2Select = mdrv_DP_GetHBR2PortSelect(InputPort);   //scaler Port to Combo

    if (( msReadByte( REG_113147 + ucHBR2Select*HBR2_OFFSET) & BIT7 ) == BIT7)
    {//DRR
        return TRUE;
    }
    else
    {//non-DRR
        return FALSE;
    }
}

//**************************************************************************
//  [Function Name]:
//                  DPRxEnableDRRFlag()
//  [Description]
//                  DPRxEnableDRRFlag
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void DPRxEnableDRRFlag(BYTE InputPort, Bool bEnable)
{

    WORD u16Regoffset=0;
		
    u16Regoffset = PortToCombo[InputPort]*COMBO_PHY_OFFSET;

    if(bEnable == TRUE)
    {//Enable DRR
		msWriteByteMask(REG_001103 +u16Regoffset, BIT6, BIT6);
    }
    else
    {//Disable DRR
		msWriteByteMask(REG_001103 +u16Regoffset, 0, BIT6);
    }
}

//**************************************************************************
//  [Function Name]:
//                  DPRxSpecificFiled(void)
//  [DPRxSpecificFiled
//  [Arguments]:
//
//**************************************************************************
void DPRxSpecificFiled( Combo_IP_SELECT i)
{
	  WORD j=0;
         j = HBRPort(i) * HBR2_OFFSET;

	msWriteByte( REG_113352+j, IEEE_OUI_FIRST );             
	msWriteByte( REG_113353+j, IEEE_OUI_SECOND );  
	msWriteByte( REG_113354+j, IEEE_OUI_THIRD );  

	msWriteByte( REG_1131AC+j, DEVICE_STRING_0 );     
	msWriteByte( REG_1131AD+j, DEVICE_STRING_1 );     
	msWriteByte( REG_1131AE+j, DEVICE_STRING_2 );     
	msWriteByte( REG_1131AF+j, DEVICE_STRING_3 );     
	msWriteByte( REG_1131B0+j, DEVICE_STRING_4 );     
	msWriteByte( REG_1131B1+j, DEVICE_STRING_5 );     

	msWriteByte( REG_1131B2+j, HARDWARE_VERSION );  
	
	msWriteByte( REG_1131B3+j, FIRMWARE_MAJOR_VERSION );  
	msWriteByte( REG_1131B4+j, FIRMWARE_MINOR_VERSION );  
	
}

//**************************************************************************
//  [Function Name]:
//                  DWORD DPRxLongDIV(DWORD M_Value, WORD LingRate, DWORD N_Value)
//  [Description]
//                  DPRxLongDIV
//
//**************************************************************************
DWORD DPRxLongDIV(DWORD M_Value, WORD LingRate, DWORD N_Value)
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
//                 DPRxNoVideoCheck
//  [Description]
//                  DPRxNoVideoCheck
//
//**************************************************************************
#if DP_NOVIDEO
void  DPRxNoVideoCheck(void)
{

	if(DPRxCheckLock())
	{
		if(msReadByte( REG_1133A5+port*HBR2_OFFSET )&BIT3)
		{
			msWriteByteMask(REG_1132C0 +port*HBR2_OFFSET, 0, BIT0);	
			//DP_printData( "  Disable  RS ", 0);
		}
		else
		{	
			msWriteByteMask(REG_1132C0 +port*HBR2_OFFSET, BIT0, BIT0);	
			//DP_printData( "  Enable  RS ", 0);
		}
	}
	
}	
#endif



//**************************************************************************
//  [Function Name]:
//                  mapi_DP_GetDPNoVideoBit(void)
//  [mapi_DP_GetDPNoVideoBit
//  [Arguments]:
//
//**************************************************************************
bool mapi_DP_GetDPNoVideoBit(BYTE ucInputPort)
{
	 return  gDPInfo[HBRPort(ucInputPort)].bDPNoVideoStrm;
}
//**************************************************************************
//  [Function Name]:
//                  mapi_DP_GetDPNoVideoBit(void)
//  [mapi_DP_GetDPNoVideoBit
//  [Arguments]:
//
//**************************************************************************
bool mapi_DP_GetDPNoAudioBit(BYTE ucInputPort)
{
	 return  gDPInfo[HBRPort(ucInputPort)].bDPNoAudioStrm;
}
//**************************************************************************
//  [Function Name]:
//                  mapi_DP_CheckDPLock(void)
//  [Descriptionmapi_DP_CheckDPLock
//  [Arguments]:
//
//**************************************************************************
bool mapi_DP_CheckDPLock(BYTE ucInputPort)
{
	 return  gDPInfo[HBRPort(ucInputPort)].bDPLock;
}
//**************************************************************************
//  [Function Name]:
//                  mapi_DP_GetColorType(void)
//  [Description]
//                  mapi_DP_GetColorType
//  [Arguments]:
//
//  [Return]:   1 : Audio Exit
//                  0 :  no Audio
//**************************************************************************
BYTE mapi_DP_GetColorType(BYTE InputPort)
{
    return gDPInfo[HBRPort(InputPort)].bColorType;
}
//**************************************************************************
//  [Function Name]:
//                  mapi_DP_GetColorDepth(void)
//  [Description]
//                  mapi_DP_GetColorDepth
//  [Arguments]:
//
//  [Return]:   1 : Audio Exit
//                  0 :  no Audio
//**************************************************************************
BYTE mapi_DP_GetColorDepth(BYTE InputPort)
{
    return gDPInfo[HBRPort(InputPort)].ucDPColorDepth;
}

//**************************************************************************
//  [Function Name]:
//                  mapi_DP_CheckAudioStatus(void)
//  [Description]
//                  mapi_DP_RXChkAudio
//  [Arguments]:
//
//  [Return]:   1 : Audio Exit
//                  0 :  no Audio
//**************************************************************************
Bool mapi_DP_CheckAudioStatus(BYTE InputPort)
{
    if((gDPInfo[HBRPort(InputPort)].InputPacketStatus&(BIT5|BIT6))==(BIT5|BIT6))
    return TRUE;
    else
    return FALSE;
}

//**************************************************************************
//  [Function Name]:
//                  mapi_DP_GetDPMisc(BYTE InputPort)
//  [Description]
//                  mapi_DP_GetDPMisc
//  [Arguments]:
//
//  [Return]:
//  msAPI_combo_IPGetPacketContent(COMBO_IP_SELECT_PORT0,COMBO_PACKET_DP_MISC,2, Data);
//**************************************************************************
void mapi_DP_Get_Misc(BYTE ucInputPort, BYTE *pPacketData)
{
    pPacketData[0] = gDPInfo[HBRPort(ucInputPort)].DPMISC0;
    pPacketData[1] = gDPInfo[HBRPort(ucInputPort)].DPMISC1;
}


//**************************************************************************
//  [Function Name]:
//                  mapi_DP_GetColorSpace(BYTE InputPort)
//  [Description]
//                  mapi_DP_GetColorSpace
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE mapi_DP_GetColorSpace(BYTE InputPort)
{
    return gDPInfo[HBRPort(InputPort)].ucDPColorFormat;
}

//**************************************************************************
//  [Function Name]:
//                  mapi_DP_GetRangeFormat(BYTE InputPort)
//  [Description]
//                  mapi_DP_GetRangeFormat
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE mapi_DP_GetRangeFormat(BYTE InputPort)
{
    return gDPInfo[HBRPort(InputPort)].ucDPRangeFormat;
}

//**************************************************************************
//  [Function Name]:
//                  mapi_DP_SetPowerStatus()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
 
void mapi_DP_SetPowerStatus(BYTE ucPowerStatus)
{
 #if ENABLE_DP_ShortHPD_PM	
    BYTE i;
 #endif 
    if(ucPowerStatus == COMBO_IP_POWER_PS_ON)
    {
      #if ENABLE_DP_ShortHPD_PM	 
        for(i=0; i<DP_NUM; i++)
        {
        
            DPRxSet_ShortHPDFlag(i, TRUE);
        
        }
       #endif 
    }
    if((ucPowerStatus == COMBO_IP_POWER_DC_ON) )
    {
        
    }
}
   	
//**************************************************************************
//  [Function Name]:
//                  mapi_DP_SetConfiguration()
//  [Description]
//                  mapi_DP_SetConfiguration
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_DP_SetConfiguration(BYTE ucHBR20EnableIndex, BYTE ucHBR21EnableIndex, BYTE ucDPEnableIndex, BYTE ucDisplayEnableIndex )
{
    BYTE  i=0, uctemp=1;
    BYTE ucHBREnableIndex=0;

    ucDPEnableIndex_Temp = ucDPEnableIndex;
    ucDisplayEnableIndex_Temp = ucDisplayEnableIndex;

    if(DPConfigFinish==0)
    {
        ucDPHBR20PortSelect = ucHBR20EnableIndex;
        ucDPHBR21PortSelect = ucHBR21EnableIndex;

        for(i=0;i<=5;i++)
        {
            if(ucDPEnableIndex & uctemp)
            {
                DP_Connected[i] = 1;
            }
            uctemp = uctemp <<1;
        }
        ucHBREnableIndex = ucDPEnableIndex ^ ucHBR20EnableIndex ^ ucHBR21EnableIndex;
        uctemp=1;
        for(i=0;i<=5;i++)
        {
            if(ucHBR20EnableIndex & uctemp)
                DP_HBR2_0 = (HBR2_IP_SELECT)i;
            uctemp = uctemp <<1;
            PortToCombo[0] = DP_HBR2_0;
        }
        //DP_printData( "DP_HBR2_0=  %x", DP_HBR2_0);
        uctemp=1;
        for(i=0;i<=5;i++)
        {
            if(ucHBR21EnableIndex & uctemp)
                DP_HBR2_1 = (HBR2_IP_SELECT)i;
            uctemp = uctemp <<1;
            PortToCombo[1] = DP_HBR2_1;
        }
        //DP_printData( "DP_HBR2_1=  %x", DP_HBR2_1);
        uctemp=1;
        for(i=0;i<=5;i++)
        {
            if(ucHBREnableIndex & uctemp)
            {
                DP_HBR_0 = (HBR_IP_SELECT)i;
                    DP_Combo_Connected[i] =1;

            }
                  DP_printData( " i  =>  %x ", i);
                  DP_printData( " DP_Combo_Connected[] =>  %x ", DP_Combo_Connected[i]);
            uctemp = uctemp <<1;
        }

        for(i=0;i<Input_Nums;i++)
        {
        	if((g_InputPort[i].eIPType & TYPE_DP) && (g_InputPort[i].eIPType & TYPE_HBR2_0))
        	{
               // printData(" HBR2_0 <%x>", i);
                ComboToPort[i]=0;
            }
            if((g_InputPort[i].eIPType & TYPE_DP) && (g_InputPort[i].eIPType & TYPE_HBR2_1))
            {
                //printData(" HBR2_1<%x>", i);
                ComboToPort[i]=1;
            }
        }

        //for(i=0;i<=6;i++)
        //printData(" ComboToPort<%x>", ComboToPort[i]);

        //for(i=0;i<=1;i++)
        //printData(" PortToCombo<%x>", PortToCombo[i]);

        for(i=0;i<7;i+=1)
        {
            if(ComboToPort[i] !=0xFF)
                HBR2ToScanPort[ComboToPort[i]]  = i;
            //DP_printData( " HBR2ToScanPort[0] =>  %x ",HBR2ToScanPort[0]);
            //DP_printData( " HBR2ToScanPort[1] =>  %x ",HBR2ToScanPort[1]);
        }
    }

         DP_printData( " ucHBR20EnableIndex =>  %x ",ucHBR20EnableIndex);
             DP_printData( " ucHBR21EnableIndex =>  %x ",ucHBR21EnableIndex);
         DP_printData( " ucDPEnableIndex =>  %x ",ucDPEnableIndex);
         DP_printData( " DP_HBR_0 =>  %x ", DP_HBR_0);
         DP_printData( " ucHBREnableIndex =>  %x ", ucHBREnableIndex);

    for(uctemp=0;uctemp<(DP_NUM);uctemp++)
    {
        if(!(ucDisplayEnableIndex_Temp & (1<< PortToCombo[uctemp])))
        {
            //DP_printMsg("===Enable 1P mode ");
            //DPRxEnSplit(DP_1P,(HBR2_SELECT)uctemp);
            //if(uctemp ==0)
                //msDrvSrcSetHBR2FlagMux(HBR2ToScanPort[uctemp], FALSE,TYPE_HBR2_0|TYPE_DP);
            //else
                //msDrvSrcSetHBR2FlagMux(HBR2ToScanPort[uctemp], FALSE,TYPE_HBR2_1|TYPE_DP);
            gDPInfo[uctemp].bDPSplitOnOff =0;
            gDPInfo[uctemp].bDPSplitEn = 0;
            gDPInfo[uctemp].bDPSplitEn_temp=0;
        }
    }

    DPCombo_Update = 1;
}

//**************************************************************************
//  [Function Name]:
//                  mapi_DP_GetCableDetectPort()
//  [Description]
//                  mapi_DP_GetCableDetectPort
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE mapi_DP_GetCableDetectPort(BYTE ucInputPort)
{
    EN_COMBO_IP_CABLE ucComboIPCableDetect = COMBO_IP_CABLE_NONE;

    switch(ucInputPort)
    {
        case 0:
            ucComboIPCableDetect = (hwSDMDP0Pin_Pin ? COMBO_IP_CABLE_NONE : COMBO_IP_CABLE_DP);
            break;
        case 1:
            ucComboIPCableDetect = (hwSDMDP1Pin_Pin ? COMBO_IP_CABLE_NONE : COMBO_IP_CABLE_DP);
            break;
        case 2:
            ucComboIPCableDetect = (hwSDMDP2Pin_Pin ? COMBO_IP_CABLE_NONE : COMBO_IP_CABLE_DP);
            break;
        case 3:
            ucComboIPCableDetect = (hwSDMDP3Pin_Pin ? COMBO_IP_CABLE_NONE : COMBO_IP_CABLE_DP);
            break;
        case 4:
            ucComboIPCableDetect = (hwSDMDP4Pin_Pin ? COMBO_IP_CABLE_NONE : COMBO_IP_CABLE_DP);
            break;
        case 5:
            ucComboIPCableDetect = (hwSDMDP5Pin_Pin ? COMBO_IP_CABLE_NONE : COMBO_IP_CABLE_DP);
            break;
        default:
            break;
    }
    return ucComboIPCableDetect;
}

//**************************************************************************
//  [Function Name]:
//                  mapi_DP_SetGuardBand()
//  [Description]
//                  mapi_DP_SetGuardBand
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_DP_SetGuardBand(BYTE ucInputPort, WORD usGuardBandValue)
{
    BYTE ucHBR2Select = mdrv_DP_GetHBR2PortSelect(ucInputPort);

    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {

    }
    else // HBR2 path
    {
        msWrite2ByteMask(REG_113554 +ucHBR2Select*HBR2_OFFSET, usGuardBandValue, MASK_11BIT); // SPRHST = 0x10
    }
}

//**************************************************************************
//  [Function Name]:
//                  mapi_DP_GetHVInformation()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_DP_GetHVInformation(BYTE ucInputPort, WORD *usHTotalValue, WORD *usVTotalValue)
{
    BYTE ucHBR2Select = mdrv_DP_GetHBR2PortSelect(ucInputPort);

    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {

    }
    else // HBR2 path
    {
        *usHTotalValue = gDPInfo[ucHBR2Select].uwDPHtotal;
        *usVTotalValue = gDPInfo[ucHBR2Select].uwDPVtotal;
    }
}

//**************************************************************************
//  [Function Name]:
//                  mapi_DP_GetInterlaceFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mapi_DP_GetInterlaceFlag(BYTE ucInputPort)
{
    Bool bFlag = FALSE;

    if(DPRxGetVB_ID(ucInputPort))
    {
        bFlag = TRUE;
    }

    return bFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mapi_DP_GetAstorPTGInterlacePatch(BYTE ucInputPort)
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mapi_DP_GetAstorPTGInterlacePatch(BYTE ucInputPort)
{
    Bool bFlag = FALSE;

    if(DPRxAstroPTGInterlacePatch(ucInputPort))
    {
        bFlag = TRUE;
    }

    return bFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mapi_DP_GetAstorPTGInterlaceFieldPatch(BYTE ucInputPort)
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mapi_DP_GetAstorPTGInterlaceFieldPatch(BYTE ucInputPort)
{
    Bool bFlag = FALSE;

    if(DPRxAstroPTGInterlaceFieldPatch(ucInputPort))
    {
        bFlag = TRUE;
    }

    return bFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mapi_DP_CheckIgnoreDPMS()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mapi_DP_CheckIgnoreDPMS(BYTE ucInputPort)
{
     BYTE ucHBR2Select = mdrv_DP_GetHBR2PortSelect(ucInputPort);

    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
	return 0;
    }
    else // HBR2 path
    {
    return DPRXCheckDPMS(ucInputPort);

    }

}

//**************************************************************************
//  [Function Name]:
//                  mapi_DP_GetSplitFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mapi_DP_GetSplitFlag(BYTE ucInputPort)
{
    Bool bSplitFlag = FALSE;
    BYTE ucHBR2Select = mdrv_DP_GetHBR2PortSelect(ucInputPort);

    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {

    }
    else // HBR2 path
    {
        bSplitFlag = gDPInfo[ucHBR2Select].bDPSplitEn;
    }

    return bSplitFlag;
}
//**************************************************************************
//  [Function Name]:
//                  mapi_DP_GetSplitFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mapi_DP_GetMSAChgFlag(BYTE ucInputPort)
{
    Bool bChgFlag = FALSE;
    BYTE ucHBR2Select = mdrv_DP_GetHBR2PortSelect(ucInputPort);

    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {

    }
    else // HBR2 path
    {
        bChgFlag =((msReadByte(REG_1133E4 +ucHBR2Select*HBR2_OFFSET)&BIT5) ? TRUE : FALSE);       
    }

    return bChgFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mapi_DP_GetSplitFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mapi_DP_ClrMSAChgFlag(BYTE ucInputPort)
{
    Bool bChgFlag = FALSE;
    BYTE ucHBR2Select = mdrv_DP_GetHBR2PortSelect(ucInputPort);

    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {

    }
    else // HBR2 path
    {        
        msWriteByteMask(REG_1133E8+ucHBR2Select*HBR2_OFFSET, BIT5,BIT5);
        msWriteByteMask(REG_1133E8+ucHBR2Select*HBR2_OFFSET, 0,BIT5);
        DP_printMsg("=====MSA  Chg    M/N");
    }

    return bChgFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mapi_DP_TimerInterrupt()
//  [Description]
//                  mapi_DP_TimerInterrupt
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_DP_TimerInterrupt(void)
{
    BYTE i=0;

    for (i = 0; i < DP_NUM; i++)
    {
	#if (DP_NOVIDEO==1)

	     if(gDPInfo[i].bDPVBIDmsCnt)
               gDPInfo[i].bDPVBIDmsCnt--;
            if(gDPInfo[i].bDPVBIDmsCnt ==0)
            {
			gDPInfo[i].bDPVBIDChk =1;
            }  
       #endif
	 if(gDPInfo[i].ucDPNormalModeDelayCnt)
  	  gDPInfo[i].ucDPNormalModeDelayCnt--;

        if(gDPInfo[i].uwDPRxStableTimeout)
            gDPInfo[i].uwDPRxStableTimeout--;

        if(gDPInfo[i].uwCDRHPDCnt)
            gDPInfo[i].uwCDRHPDCnt--;

        if(gDPInfo[i].uwSkewHPDCnt)
           gDPInfo[i].uwSkewHPDCnt--;

        if(gDPInfo[i].uwHDCPCnt)
        gDPInfo[i].uwHDCPCnt--;

      if(gDPInfo[i].bPacketTimer)
        gDPInfo[i].bPacketTimer--;

	if((gDPInfo[i].bDPSplitEn_Cnt_Up < 0x80)&&(gDPInfo[i].bDPSplitEn_Cnt_Up >0))
	 gDPInfo[i].bDPSplitEn_Cnt_Up--;

	if((gDPInfo[i].bDPSplitEn_Cnt_Down < 0x80)&&(gDPInfo[i].bDPSplitEn_Cnt_Down >0))
	 gDPInfo[i].bDPSplitEn_Cnt_Down--;


#if LOOPOUT_DPNOSIGNAL_SOLUTION
	  if(gDPInfo[i].uwReChkTiming)	// 140528 DMD Loop Out¿¡¼­ °£ÇæÀû È­¸é ¾È³ª¿È °³¼±À» À§ÇÑ Spot ¼Ö·ç¼Ç Àû¿ë
	   gDPInfo[i].uwReChkTiming--;
#endif


#if(ENABLE_STABLE_COUNT)
        if(gDPInfo[i].uwDPStableCount)
            gDPInfo[i].uwDPStableCount--;
#endif

    }

    #if DPSSC
      if(DPRxAudioCnt)
        DPRxAudioCnt--;
    #endif

    if(u16DPAudio_BWCnt)
        u16DPAudio_BWCnt--;

#if DP_COMBO_PORT
    mapi_DPCom_TimerInterrupt();
#endif
}

//**************************************************************************
//  [Function Name]:
//                  mapi_DP_Set_HPD()
//  [Description]
//                  mapi_DP_Set_HPD
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_DP_Set_HPD(BYTE ucTemp, Bool bSetHPD)
{
    if( bSetHPD == TRUE )
    {
        if( ucTemp == Combo_PORT0)
            hw_Set_DPHpd0();
        else if(ucTemp == Combo_PORT1)
            hw_Set_DPHpd1();
        else if(ucTemp == Combo_PORT2)
            hw_Set_DPHpd2();
        else if(ucTemp == Combo_PORT3)
            hw_Set_DPHpd3();
        else if(ucTemp == Combo_PORT4)
            hw_Set_DPHpd4();
        else if(ucTemp == Combo_PORT5)
            hw_Set_DPHpd5();
        DP_printMsg( "\nDPRxHPD_High" );
		DP_printData("** combo input port %d", ucTemp);
		
    }
    else
    {
        if( ucTemp == Combo_PORT0)
            hw_Clr_DPHpd0();
        else if(ucTemp == Combo_PORT1)
            hw_Clr_DPHpd1();
        else if(ucTemp == Combo_PORT2)
            hw_Clr_DPHpd2();
        else if(ucTemp == Combo_PORT3)
            hw_Clr_DPHpd3();
        else if(ucTemp == Combo_PORT4)
            hw_Clr_DPHpd4();
        else if(ucTemp == Combo_PORT5)
            hw_Clr_DPHpd5();
        DP_printMsg( "\nDPRxHPD_Low" );
		DP_printData("** combo input port %d", ucTemp);

    }
}

//**************************************************************************
//  [Function Name]:
//                  mapi_DP_GetColorDepthInfo()
//  [Description]
//                  mapi_DP_GetColorDepthInfo
//  [Arguments]:
//
//  [Return]:
//
//  000 : 6 bit
//  001 : 8 bit
//    010 : 10bit
//  011 : 12bit
//    100 : 16 bit
        //pPacketData[0]    ªº [7:5] bit
        //000 : 6 bit
        //001 : 8 bit
        //010 : 10bit
        //011 : 12bit
        //100 : 16 bit

//**************************************************************************
BYTE mapi_DP_GetColorDepthInfo(BYTE InputPort)
{
      BYTE ucHBR2Select = mdrv_DP_GetHBR2PortSelect(InputPort);

    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
	  return  gDPComboInfo[InputPort].ucDPColorDepth;
    }
    else // HBR2 path
    {
    return( gDPInfo[HBRPort(InputPort)].DPMISC0 >> 5)&0x07;
}


}

//**************************************************************************
//  [Function Name]:
//                  mapi_DP_ConvertHBR2()
//  [Description]
//                  mapi_DP_ConvertHBR2
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE mapi_DP_ComboConvertHBR2(BYTE InputPort)
{
    return mdrv_DP_GetHBR2PortSelect(InputPort);
}

//**************************************************************************
//  [Function Name]:
//                  mapi_DP_ConfigAudioPort()
//  [Description]
//                  mapi_DP_ConfigAudioPort
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_DP_ConfigAudioPort(BYTE InputPort , Bool  Enable)
{
    BYTE i=0;

    if(Enable)
    {
        DPRxSetAUPLL(InputPort);
        gDPCombo[InputPort].AudioEn = TRUE;

        for(i=0;i<DP_COMBO;i++)
        {
            if(i != InputPort)
            gDPCombo[i].AudioEn = 0;
        }
    }
    else
    {
        for(i=0;i<DP_COMBO;i++)
        gDPCombo[i].AudioEn = 0;
    }

}


//**************************************************************************
//  [Function Name]:
//                  DPRxSetAUPLL(Byte DPRxSetAUPLL)
//  [Description]
//                  DPRxSetAUPLL
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_DPRxAudioFreeRun(BYTE ucComboPort)
{
	 BYTE utemp=0;	 
	 DWORD  DPAUM0 = 0;
	 DWORD  Freq_Ctrl= 0;
	 utemp = msReadByte(  REG_11330E+HBRPort(ucComboPort)*HBR2_OFFSET );
	 DP_printMsg( "\n==========>DP AudioFreeRun" );
        if(( utemp & 0x0F ) == 0x06 )
        {
            msWriteByteMask(REG_162F40, ((ucComboPort)<<4), 0xF0);
            msWriteByteMask(REG_162F41, 0, BIT0);
             DPAUM0 = 0x2983<<1;
             Freq_Ctrl = 0x155555;
        }
        else  if(( utemp & 0x0F ) == 0x0A)
        {
            msWriteByteMask(REG_162F40, ((ucComboPort)<<4), 0xF0);
            msWriteByteMask(REG_162F41, 0, BIT0);
            DPAUM0 = 0x2983;
            Freq_Ctrl = 0x199999;
        }
        else  if(( utemp & 0x1F ) == 0x14)
        {
		msWriteByteMask( REG_162F40, ((ucComboPort)+6) <<4 , 0xF0);
		msWriteByteMask(REG_162F41, BIT0, BIT0);
		 DPAUM0 = 0x2983>>1;
		 Freq_Ctrl = 0x33333;
        }

	msWriteByte(REG_162F08,  ( Freq_Ctrl & 0x000000FF )); //MSA MAUD_CENTER[7:0]
	msWriteByte(REG_162F09,  ( Freq_Ctrl & 0x0000FF00 ) >> 8); //MSA MAUD_CENTER[7:0]
	msWriteByte(REG_162F0A,  ( Freq_Ctrl & 0x00FF0000 ) >> 16); //MSA MAUD_CENTER[7:0]

        
	msWriteByte(REG_1134C8+HBRPort(ucComboPort)*HBR2_OFFSET,  ( DPAUM0 & 0x000000FF )); //MSA MAUD_CENTER[7:0]
	msWriteByte(REG_1134C9+HBRPort(ucComboPort)*HBR2_OFFSET,  ( DPAUM0 & 0x0000FF00 ) >> 8); //MSA MAUD_CENTER[7:0]
	msWriteByte(REG_1134CA+HBRPort(ucComboPort)*HBR2_OFFSET,  ( DPAUM0 & 0x00FF0000 ) >> 16); //MSA MAUD_CENTER[7:0]

	msWriteByte(REG_1134CB+HBRPort(ucComboPort)*HBR2_OFFSET, 0x00); //MSA MAUD_CENTER[7:0]
	msWriteByte(REG_1134CC+HBRPort(ucComboPort)*HBR2_OFFSET, 0x80); //MSA MAUD_CENTER[7:0]
	msWriteByte(REG_1134CD+HBRPort(ucComboPort)*HBR2_OFFSET, 0x00); //MSA MAUD_CENTER[7:0]


	msWriteByteMask(REG_1134D5+HBRPort(ucComboPort)*HBR2_OFFSET, BIT5,BIT5);    //Oven
	msWriteByteMask(REG_1134B3+HBRPort(ucComboPort)*HBR2_OFFSET, BIT2,BIT2);    //Oven
	msWriteByteMask(REG_1134B3+HBRPort(ucComboPort)*HBR2_OFFSET, 0,BIT2);	      //Oven


	msWriteByteMask( REG_162F11, 0,BIT7 );             // Turn off Bypasse mode
	msWriteByteMask( REG_162F07, BIT5,BIT5 );           //update Freq
	msWriteByteMask( REG_162F07, 0,BIT5);                 //update Freq

}
//**************************************************************************
//
//              ISR()
//
//**************************************************************************
void mapi_DP_PMIsrHandler(InterruptNum eIntNum)
{
    UNUSED(eIntNum);

    if(mdrv_DP_GetInterruptFlag())
    {
        ISR_DPRxAuxISR();
    }

}
//**************************************************************************
//
//              DP_AUX()
//
//**************************************************************************
void ISR_DPRxAuxISR(void)
{
    BYTE ucPort=0;

    for(ucPort =0; ucPort <=5; ucPort++)
    {
        if(DP_Connected[ucPort] ==0)
            continue;

        Aux = ucPort * COMBO_PHY_OFFSET;

     msRegs(REG_00113E + Aux) =msRegs(REG_00113E + Aux)&(~(BIT1|BIT0));

    #if DPENABLEMCCS
        if(msRegs(REG_001178+ Aux)&_BIT7)   //MCCS Interrupt
        {
#if(DP_MCCS_NEW_MODE)
            _mhal_DP_MCCSReceiveProc(ucPort);
#else
            msDP_DDC2BI() ;
#endif
        }
    #endif

        if( msRegs(REG_001190+ Aux) & BIT0 ) // Program DPCD 0
        {
            DPRxProgramDPCD_ISR0(Aux);

            msRegs(REG_001192+ Aux)  = BIT0;
            msRegs(REG_001192+ Aux)  = 0;
        }

      if( msRegs(REG_001190+ Aux) & BIT1 ) // Program DPCD 1
        {
            DPRxProgramDPCD_ISR1(Aux);

            msRegs(REG_001192+ Aux)  = BIT1;
            msRegs(REG_001192+ Aux)  = 0;
        }

#if DP_HDCP_Reset
        if( msRegs(REG_001190+ Aux) & BIT2 ) // Program DPCD 2
        {
            DPRxProgramDPCD_ISR2(ucPort);

            msRegs(REG_001192+ Aux)  = BIT2;
            msRegs(REG_001192+ Aux)  = 0;
        }
#endif
 #if 0
        if( msRegs(REG_001178+ Aux) & BIT4 ) // Aux Comd
        {
            msRegs(REG_001179+ Aux)  = BIT4;
            msRegs(REG_001179+ Aux)  = 0;
        }
#endif
	 
	    if( msRegs(REG_001193+ Aux) & BIT4 ) // AKSV INt
        {
            // msRegs(REG_1133A1+HBR2_OFFSET* HBRPort(ucPort) ) = msRegs(REG_1133A1+HBR2_OFFSET* HBRPort(ucPort)) | BIT4;   //HDCP reset
            // msRegs(REG_1133A1+HBR2_OFFSET* HBRPort(ucPort) ) = msRegs(REG_1133A1+HBR2_OFFSET* HBRPort(ucPort)) & (~BIT4);  //HDCP reset.
             msRegs(REG_001193+ Aux)  = BIT7;
             msRegs(REG_001193+ Aux)  = 0;
        }






    #if 0
        if( msRegs(REG_001190+ Aux) & BIT5 ) // SQ
        {
            //msRegs(REG_11332B) +=1;
            msRegs(REG_001196+ Aux)  = BIT2;
            msRegs(REG_001196+ Aux)  = 0;
        }
    #endif
    }
}
//**************************************************************************
//
//              ISR()
//
//**************************************************************************
void ISR_DPHBR2(HBR2_SELECT HBR2PORT)
{
    WORD Offset;
    WORD Offset_PHY;

    Offset = HBR2PORT * HBR2_OFFSET;
    Offset_PHY = PortToCombo[HBR2PORT]* COMBO_PHY_OFFSET;

    // ***************************************************
    // ****************   AUPLLBIGChange  *********************
    // ***************************************************
    if( msRegs(REG_1133EB+Offset)& 0x02 ) //  AUPLL MN BIG CHANGE
    {
        gDPInfo[HBR2PORT].bDPAUPLLBIGChange  = TRUE;
        msRegs(REG_1133EB+Offset) = 0x02;            // clear IRQ
    }
    // ***************************************************
    // ****************   InterlaneSkew  ********************
    // ***************************************************
#if 1
    if( msRegs(REG_1133EA+Offset) & 0x08 ) // InterlaneSkew
    {
        gDPInfo[HBR2PORT].bDPInterlaneSkewIRQ= TRUE;
        gDPInfo[HBR2PORT].uwSkewHPDCnt = 1000;
        msRegs(REG_1133EA+Offset) = 0x08;            // clear IRQ
    }
#endif

    // ***************************************************
    // ****************   CDRLoseLock  *********************
    // ***************************************************
#if 1
    if( msRegs(REG_1133E9+Offset) & 0x01 ) // CDRLoseLock
    {
        if(gDPInfo[HBR2PORT].bAutoEQReset)
    {
             msRegs(REG_113000+Offset) = msRegs(REG_113000+Offset)|BIT1;
             msRegs(REG_113000+Offset) = msRegs(REG_113000+Offset)&(~BIT1);
             gDPInfo[HBR2PORT].bAutoEQReset =0;
        }
        gDPInfo[HBR2PORT].bDPLoseCDRLockIRQ= TRUE;
        gDPInfo[HBR2PORT].uwCDRHPDCnt = 1000;
        msRegs(REG_1133E9+Offset) = 0x01;            // clear IRQ
    }
#endif

    // ***************************************************
    // ****************   VPLLBIGChange  *********************
    // ***************************************************
    if( msRegs(REG_1133EB+Offset)& 0x08 ) // VPLL MN BIG CHANGE
    {
        //msRegs(REG_11332B+Offset) +=1;
        gDPInfo[HBR2PORT].bDPVPLLBIGChange  = TRUE;
        msRegs(REG_1133EB+Offset) = 0x08;            // clear IRQ
    }

    // ****************************************************
    // ****************   Training P1  ************************
    // ****************************************************

    if(( msRegs(REG_1133E4+Offset) & 0x01 ) || ( msRegs(REG_1133E8+Offset) & 0x01 ) )    // Training P1
    {
        DWORD DPTP1Cnt = 0;
#if(AUTO_EQ)
        BYTE delay_count = 0;
#endif

#if(ENABLE_STABLE_COUNT)
        gDPInfo[HBR2PORT].bDPCheckFlag = 0;
		gDPInfo[HBR2PORT].uwDPStableCount = 0;
#endif

        #if(AUTO_EQ)

#if(COMBO_DP_DAISY_CHAIN_SUPPORT)
		if(HBR2PORT == HBR2_1)
#endif
		{
	 #if DPCTS_Patch
	  #if (CHIP_ID==MST9U2)	 
		 msRegs(REG_11307B+Offset) = msRegs(REG_11307B+Offset)|BIT6;		
		 #else
		 msRegs(REG_11307B+Offset) = msRegs(REG_11307B+Offset)|BIT5;		
		 #endif
	 #endif
        msRegs(REG_113000+Offset) = msRegs(REG_113000+Offset) & 0xC3;
        msRegs(REG_113000+Offset) = msRegs(REG_113000+Offset) | BIT2;
        msRegs(REG_113012+Offset) = msRegs(REG_113012+Offset) & 0xEF;
        msRegs(REG_112051+Offset_PHY) = msRegs(REG_112051+Offset_PHY) | 0x03;
        msRegs(REG_113000+Offset) = msRegs(REG_113000+Offset) & 0xFD;
		
        DPRXAutoEQOnOff(DISABLE,(HBR2_SELECT) HBR2PORT);

		msRegs(REG_0011FA+Offset_PHY) = msRegs(REG_0011FA+Offset_PHY) | 0x40;		
		msRegs(REG_0011EA+Offset_PHY) = msRegs(REG_0011EA+Offset_PHY) | 0x01;
		msRegs(REG_0011C2) = msRegs(REG_0011C2) | 0x04;

        while((( msRegs(REG_113310+Offset)& 0x03 ) == 0x01 ) && ( delay_count < 40 ) )
        {
            delay_count++;
            msRegs(REG_113310+Offset) = msRegs(REG_113310+Offset) & 0xFE;
            Delay4us();
        }

		delay_count = 0;

        while((( msRegs(REG_113310+Offset)& 0x03 ) == 0x00 ) && ( delay_count < 40 ) )
        {
            delay_count++;
            msRegs(REG_113310+Offset) = msRegs(REG_113310+Offset) | 0x01;
            Delay4us();
        }

		msRegs(REG_0011FA+Offset_PHY) = msRegs(REG_0011FA+Offset_PHY) & 0xBF;		
		msRegs(REG_0011EA+Offset_PHY) = msRegs(REG_0011EA+Offset_PHY) & 0xFE;
		msRegs(REG_0011C2) = msRegs(REG_0011C2) & 0xFB;
		
        DPRXAutoEQOnOff(ENABLE,(HBR2_SELECT) HBR2PORT);
		
		}
        #endif

        gDPInfo[HBR2PORT].bDPLoseCDRLockIRQ= FALSE;
        gDPInfo[HBR2PORT].bDPInterlaneSkewIRQ= FALSE;
        gDPInfo[HBR2PORT].bDPTrainingP0T = TRUE;
        gDPInfo[HBR2PORT].TrainingP0Cnt++;
        gDPInfo[HBR2PORT].CTSLevel = 0;
        DPTP1Cnt = 0;

    #if 1
     if(msRegs(REG_11330E+Offset)==0x14)
     {

         if (PortToCombo[HBR2PORT]&BIT0)
         msRegs(REG_0017A8 + (PortToCombo[HBR2PORT] >> 1)) = msRegs(REG_0017A8 + (PortToCombo[HBR2PORT] >> 1) )|0xF0;
        else
         msRegs(REG_0017A8 + (PortToCombo[HBR2PORT] >> 1)) = msRegs(REG_0017A8 + (PortToCombo[HBR2PORT] >> 1))|0x0F;
     }
     else
     {
        if(PortToCombo[HBR2PORT]&BIT0)
         msRegs(REG_0017A8 +(PortToCombo[HBR2PORT] >> 1) ) = (msRegs(REG_0017A8 + (PortToCombo[HBR2PORT] >> 1))&0x0F)|0x40;
        else
        msRegs(REG_0017A8 + (PortToCombo[HBR2PORT] >> 1)) =(msRegs(REG_0017A8 +(PortToCombo[HBR2PORT] >> 1) )&0xF0)|0x04;

     }
     #endif


        msRegs(REG_11331E+Offset) =0x00;
        msRegs(REG_11331F+Offset) =0x00;

        //msRegs(REG_1133A1+Offset) = msRegs(REG_1133A1+Offset) | BIT4;   //HDCP reset      // Fix Astro (HDCP on) and then press LT.  //20140826
        //msRegs(REG_1133A1+Offset) = msRegs(REG_1133A1+Offset) & (~BIT4);  //HDCP reset.

#if(AUTO_EQ)
#if(COMBO_DP_DAISY_CHAIN_SUPPORT)
		if(HBR2PORT == HBR2_1)
#endif
        {
        msRegs(REG_11357F+Offset) = msRegs(REG_11357F+Offset) & 0xFE;    //disable Reset Word Boundary
        msRegs(REG_1133E0+Offset) = msRegs(REG_1133E0+Offset) & 0xFD;       //enable TPS2 interrupt
        }
#endif

        while((( msRegs(REG_113310+Offset)& 0x03 ) == 0x01 ) && ( DPTP1Cnt < 200000 ) ) // while pattern 1 is On-Going
        {
            // if(DPTP1Cnt > 49990)
            // {
            //      msRegs(REG_113310) = 0x00;
            // }
            DPTP1Cnt++;
            switch( msRegs(REG_11330F+Offset) & 0x07 )
            {
                case 0x01:  // 1 lane
                    if(( msRegs(REG_113311+Offset) & 0x03 ) == ( msRegs(REG_11331E+Offset) & 0x03 ) ) // request reach
                    {
                        if(( msRegs(REG_11331A+Offset) & 0x01 ) != 0x01 ) // 1 lane LOCK Fail
                        {
                            if(  gDPInfo[HBR2PORT].CTSLevel < 3 )
                                gDPInfo[HBR2PORT].CTSLevel++;  // update swing level
                            msRegs(REG_11331E+Offset) =( msRegs(REG_11331E+Offset) & 0x0C) | (  gDPInfo[HBR2PORT].CTSLevel & 0x03 );
                            msRegs(REG_11331C+Offset)  = msRegs(REG_11331C+Offset) & 0x7F;
                            msRegs(REG_11331C+Offset) = msRegs(REG_11331C+Offset) | 0x80;
                        #if  (AUTO_EQ&&(MS_BOARD_TYPE_SEL == BD_MST182A_A01A_S))
                            msRegs(REG_11331E+Offset) =(msRegs(REG_11331E+Offset) & 0x0C) | (2 & 0x03 );
                        #endif
                        }
                    }
                    break;

                case 0x02:  // 2 lanes
                    if(( msRegs(REG_113311+Offset) & 0x03 ) == ( msRegs(REG_11331E+Offset) & 0x03 ) ) // request reach
                    {
                        if(( msRegs(REG_113312+Offset) & 0x03 ) == (( msRegs(REG_11331E+Offset)& 0x30 ) >> 4 ) ) // request reach
                        {
                            if(( msRegs(REG_11331A+Offset) & 0x11 ) != 0x11 ) // 2 lanes CDR LOCK Fail
                            {
                                if(  gDPInfo[HBR2PORT].CTSLevel < 3 )
                                    gDPInfo[HBR2PORT].CTSLevel++;  // update swing level
                                msRegs(REG_11331E+Offset)=( msRegs(REG_11331E+Offset) & 0xCC) | (((  gDPInfo[HBR2PORT].CTSLevel & 0x03 ) << 4 ) | (  gDPInfo[HBR2PORT].CTSLevel & 0x03 ) );
                                msRegs(REG_11331C+Offset)  = msRegs(REG_11331C+Offset) & 0x7F;
                                msRegs(REG_11331C+Offset) = msRegs(REG_11331C+Offset) | 0x80;
                            #if  (AUTO_EQ&&(MS_BOARD_TYPE_SEL == BD_MST182A_A01A_S))
                                msRegs(REG_11331E+Offset) =(msRegs(REG_11331E+Offset) & 0xCC) | (((  2 & 0x03 ) << 4 ) | (  2 & 0x03 ) );
                            #endif
                            }
                        }
                    }
                    break;

                case 0x04:  // 4 lanes
                    if(( msRegs(REG_113311+Offset) & 0x03 ) == ( msRegs(REG_11331E+Offset) & 0x03 ) ) // request reach
                    {
                        if(( msRegs(REG_113312+Offset) & 0x03 ) == (( msRegs(REG_11331E+Offset) & 0x30 ) >> 4 ) ) // request reach
                        {
                            if(( msRegs(REG_113313+Offset) & 0x03 ) == ( msRegs(REG_11331F+Offset) & 0x03 ) ) // request reach
                            {
                                if(( msRegs(REG_113314+Offset) & 0x03 ) == (( msRegs(REG_11331F+Offset) & 0x30 ) >> 4 ) ) // request reach
                                {
                                    if((( msRegs(REG_11331A+Offset) & 0x11 ) != 0x11 ) | (( msRegs(REG_11331B+Offset) & 0x11 ) != 0x11 ) ) // 4 lanes CDR LOCK Fail
                                    {
                                        if(  gDPInfo[HBR2PORT].CTSLevel < 3 )
                                            gDPInfo[HBR2PORT].CTSLevel++;  // update swing level
                                        msRegs(REG_11331E+Offset) =  (msRegs(REG_11331E+Offset) & 0xCC) | (((  gDPInfo[HBR2PORT].CTSLevel & 0x03 ) << 4 ) | (  gDPInfo[HBR2PORT].CTSLevel & 0x03 ) );
                                        msRegs(REG_11331F+Offset)  = (msRegs(REG_11331F+Offset) & 0xCC) | (((  gDPInfo[HBR2PORT].CTSLevel & 0x03 ) << 4 ) | (  gDPInfo[HBR2PORT].CTSLevel & 0x03 ) );
                                        msRegs(REG_11331C+Offset)  = msRegs(REG_11331C+Offset) & 0x7F;
                                        msRegs(REG_11331C+Offset) = msRegs(REG_11331C+Offset) | 0x80;
                                    #if (AUTO_EQ&&(MS_BOARD_TYPE_SEL == BD_MST182A_A01A_S))
                                        msRegs(REG_11331E+Offset) =(msRegs(REG_11331E+Offset) & 0xCC) | (((  2 & 0x03 ) << 4 ) | (  2 & 0x03 ) );
                                        msRegs(REG_11331F+Offset)= (msRegs(REG_11331F+Offset) & 0xCC) | (((  2 & 0x03 ) << 4 ) | (  2 & 0x03 ) );
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

	 DPRxClrSQTime();
        msRegs(REG_1133E8+Offset) = 0x01; // clear IRQ

    }


    // ****************************************************
    // ****************   Training P2 or P3 ******************
    // ****************************************************
    if((( msRegs(REG_1133E4+Offset) & 0x02 ) || ( msRegs(REG_1133E8+Offset) & 0x02 ) )||(( msRegs(REG_1133E6+Offset) & 0x02 ) || ( msRegs(REG_1133EA+Offset) & 0x02 ) ))           // Training P2 or Training P3
    {
        DWORD DPTP2Cnt=0;

        gDPInfo[HBR2PORT].bDPTrainingP1T = TRUE;
        gDPInfo[HBR2PORT].TrainingP1Cnt++;

#if (AUTO_EQ)
#if(COMBO_DP_DAISY_CHAIN_SUPPORT)
	if(HBR2PORT == HBR2_1)
#endif
    {
        BYTE AEQ_done = 0;
#if (EQ_OFFSET_FUNCTION)
        BYTE lane_sel = 0, AEQ_flag = 0;
#endif

#if (LT_ERROR_COUNT_FUNCTION || ENABLE_STABLE_COUNT)

        msRegs(REG_1135C1+Offset) = msRegs(REG_1135C1+Offset) & 0xFB;
        msRegs(REG_1135C1+Offset) = msRegs(REG_1135C1+Offset) | 0x08;

#endif

#if (EQ_OFFSET_FUNCTION)

        msRegs(REG_1120DC+Offset_PHY) =0x00;
        msRegs(REG_1120DD+Offset_PHY) =0x00;

#endif

        while(((( msRegs(REG_113310+Offset)& 0x03 ) == 0x02 )||(( msRegs(REG_113310+Offset)& 0x03 ) == 0x03 )) && ( DPTP2Cnt < 200000 ) && (AEQ_done == 0))
        {
            DPTP2Cnt++;

#if (DYNAMIC_PREEMPHASIS)

            WORD LANE0_EQ = 0, LANE1_EQ = 0, LANE2_EQ = 0, LANE3_EQ = 0;

            if(((msRegs(REG_113311+Offset)& 0x08)==0x08) && ((msRegs(REG_112083+Offset_PHY)& 0x01)==0x01))
            {
                LANE0_EQ = (msRegs(REG_1120E6+Offset_PHY) & 0x1F);

                LANE1_EQ = (((msRegs(REG_1120E6+Offset_PHY) & 0xE0) >> 5) | ((msRegs(REG_1120E7+Offset_PHY) & 0x03) << 3));

                LANE2_EQ = ((msRegs(REG_1120E7+Offset_PHY) & 0x7C) >> 2);

                LANE3_EQ = (msRegs(REG_1120E8+Offset_PHY) & 0x1F);

                if((LANE0_EQ >= EQ_THRESHOLD) || (LANE1_EQ >= EQ_THRESHOLD) || (LANE2_EQ >= EQ_THRESHOLD) || (LANE3_EQ >= EQ_THRESHOLD))
                    msRegs(REG_113012+Offset) = msRegs(REG_113012+Offset) | 0x10;
            }

#endif

#if (EQ_OFFSET_FUNCTION || LT_ERROR_COUNT_FUNCTION || ENABLE_STABLE_COUNT)

            switch( msRegs(REG_11330F+Offset) & 0x07 )
            {
                case 0x01:  // 1 lane

                    msRegs(REG_113001+Offset) = msRegs(REG_113001+Offset) & 0xFC;

                    if((msRegs(REG_113014+Offset) & 0x80) == 0x80)
                    {
                        AEQ_done = 0x01;
                    }

                break;

                case 0x02:  // 2 lanes

                    for(lane_sel=0;lane_sel<2;lane_sel++)
                    {
                        msRegs(REG_113001+Offset) = msRegs(REG_113001+Offset) & 0xFC;
                        msRegs(REG_113001+Offset) = ( msRegs(REG_113001+Offset) | lane_sel);

                        if(lane_sel == 0)
                        {
                            if((msRegs(REG_113014+Offset) & 0x80) == 0x80)
                            {
                                AEQ_flag |= 0x01;
                            }
                        }
                        else if(lane_sel == 1)
                        {
                            if((msRegs(REG_113014+Offset) & 0x80) == 0x80)
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

                    for(lane_sel=0;lane_sel<4;lane_sel++)
                    {
                        msRegs(REG_113001+Offset) = msRegs(REG_113001+Offset) & 0xFC;
                        msRegs(REG_113001+Offset) = ( msRegs(REG_113001+Offset) | lane_sel);

                        if(lane_sel == 0)
                        {
                            if((msRegs(REG_113014+Offset) & 0x80) == 0x80)
                            {
                                AEQ_flag |= 0x01;
                            }
                        }
                        else if(lane_sel == 1)
                        {
                            if((msRegs(REG_113014+Offset) & 0x80) == 0x80)
                            {
                                AEQ_flag |= 0x02;
                            }
                        }
                        else if(lane_sel == 2)
                        {
                            if((msRegs(REG_113014+Offset) & 0x80) == 0x80)
                            {
                                AEQ_flag |= 0x04;
                            }
                        }
                        else if(lane_sel == 3)
                        {
                            if((msRegs(REG_113014+Offset) & 0x80) == 0x80)
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
            }
#endif
        }


#if (EQ_OFFSET_FUNCTION)

        WORD EQ0=0, EQ1=0, EQ2=0, EQ3=0, EQ_CAL1=0, EQ_CAL2=0, EQ_CAL3=0;

        if((( msRegs(REG_113310+Offset)& 0x03 ) == 0x02 )&&(AEQ_done == 1))
        {
            if((msRegs(REG_1120E6+Offset_PHY) & 0x1F) <= (0x1F -EQ_OFFSET_VALUE))
                EQ0 = (msRegs(REG_1120E6+Offset_PHY) & 0x1F) + EQ_OFFSET_VALUE;
            else
                EQ0 = 0x1F;
            if((((msRegs(REG_1120E6+Offset_PHY) & 0xE0) >> 5) | ((msRegs(REG_1120E7+Offset_PHY) & 0x03) << 3)) <= (0x1F -EQ_OFFSET_VALUE))
                EQ1 = (((msRegs(REG_1120E6+Offset_PHY) & 0xE0) >> 5) | ((msRegs(REG_1120E7+Offset_PHY) & 0x03) << 3)) + EQ_OFFSET_VALUE;
            else
                EQ1 =0x1F;
            if(((msRegs(REG_1120E7+Offset_PHY) & 0x7C) >> 2) <= (0x1F -EQ_OFFSET_VALUE))
                EQ2 = ((msRegs(REG_1120E7+Offset_PHY) & 0x7C) >> 2) + EQ_OFFSET_VALUE;
            else
                EQ2 =0x1F;
            if((msRegs(REG_1120E8+Offset_PHY) & 0x1F) <= (0x1F -EQ_OFFSET_VALUE))
                EQ3 = (msRegs(REG_1120E8+Offset_PHY) & 0x1F) + EQ_OFFSET_VALUE;
            else
                EQ3 =0x1F;

                 EQ_CAL1 = (((EQ1 & 0x07) << 5) | (EQ0 & 0x1F));
            msRegs(REG_1120E6+Offset_PHY) = EQ_CAL1;

            EQ_CAL2 = (((EQ2 & 0x1F) << 2) | ((EQ1 & 0x18) >> 3));
            msRegs(REG_1120E7+Offset_PHY) = EQ_CAL2;

            EQ_CAL3 = (EQ3 & 0x1F);
            msRegs(REG_1120E8+Offset_PHY) = EQ_CAL3;


            msRegs(REG_1120DC+Offset_PHY) =0xC0;
            msRegs(REG_1120DD+Offset_PHY) =0x03;

        }

#endif



#if(LT_ERROR_COUNT_FUNCTION || ENABLE_STABLE_COUNT)

        if(AEQ_done == 0x01)
        {
	        AEQ_done = 0;

#if(LT_ERROR_COUNT_FUNCTION)
        volatile WORD delay_count = 0;
        BYTE error_flag = 0;
        WORD ErrorCnt = 0, error_L0 = 0,error_L1 = 0,error_L2 = 0,error_L3 = 0;


            while(delay_count < 2200)
            {
               delay_count++;
            }

            msRegs(REG_113001+Offset) = msRegs(REG_113001+Offset) | 0x08;
            msRegs(REG_113001+Offset) = msRegs(REG_113001+Offset) | 0x04;
            msRegs(REG_113001+Offset) = msRegs(REG_113001+Offset) & 0xBF;
            msRegs(REG_113001+Offset) = msRegs(REG_113001+Offset) | 0x40;

            while((error_flag == 0) && (ErrorCnt < 1000))
            {
                DPTP2Cnt++;
                ErrorCnt++;

                switch( msRegs(REG_11330F+Offset) & 0x07 )
                {
                    case 0x01:  // 1 lane

                        error_L0 = (msRegs(REG_113067+Offset) << 8) | msRegs(REG_113066+Offset);

                        if(error_L0 >= 1)
                            error_flag = 1;
                    break;

                    case 0x02:  // 2 lanes

                        error_L0 = (msRegs(REG_113067+Offset) << 8) | msRegs(REG_113066+Offset);
                        error_L1 = (msRegs(REG_113069+Offset) << 8) | msRegs(REG_113068+Offset);

                        if((error_L0 >= 1) || (error_L1 >= 1))
                            error_flag = 1;
                    break;

                    case 0x04:  // 4 lanes

                        error_L0 = (msRegs(REG_113067+Offset) << 8) | msRegs(REG_113066+Offset);
                        error_L1 = (msRegs(REG_113069+Offset) << 8) | msRegs(REG_113068+Offset);
                        error_L2 = (msRegs(REG_11306B+Offset) << 8) | msRegs(REG_11306A+Offset);
                        error_L3 = (msRegs(REG_11306D+Offset) << 8) | msRegs(REG_11306C+Offset);

                        if((error_L0 >= 1) || (error_L1 >= 1) || (error_L2 >= 1) || (error_L3 >= 1))
                        {
                            error_flag = 1;
                        }

                    break;

                    default:

                    break;
                }

            }

			msRegs(REG_113001+Offset) = msRegs(REG_113001+Offset) & 0xF7;
			msRegs(REG_113001+Offset) = msRegs(REG_113001+Offset) & 0xFB;
			msRegs(REG_113001+Offset) = msRegs(REG_113001+Offset) & 0xBF;
#endif

#if(ENABLE_STABLE_COUNT)
			BYTE link_rate = 0;

			if((gDPInfo[HBR2PORT].bDPLinkRate == msRegs(REG_11330E+Offset)) && (gDPInfo[HBR2PORT].bDPDownSpeed == 0x01))
			{
                link_rate = 0;
			}
            else
            {
                link_rate = 1;
			    gDPInfo[HBR2PORT].bDPDownSpeed = 0;
            }
#endif

#if(LT_ERROR_COUNT_FUNCTION && ENABLE_STABLE_COUNT)
            if((error_flag == 0x00) && link_rate)
#elif(ENABLE_STABLE_COUNT)
            if(link_rate)
#else
            if(error_flag == 0x00)
#endif
            {
                msRegs(REG_1135C1+Offset) = msRegs(REG_1135C1+Offset) & 0xF3;
            }
        }

#endif


#if (DYNAMIC_PREEMPHASIS)
        msRegs(REG_113012+Offset) = msRegs(REG_113012+Offset) & 0xEF;
#endif

        msRegs(REG_11357F+Offset) = msRegs(REG_11357F+Offset) | 0x01;    //enable Reset Word Boundary
        msRegs(REG_1133E0+Offset) = msRegs(REG_1133E0+Offset) | 0x02;       //disable TPS2 interrupt

	 #if DPCTS_Patch
		 #if (CHIP_ID==MST9U2)	 
			 msRegs(REG_11307B+Offset) = msRegs(REG_11307B+Offset)&(~BIT6);	
		   #else
			 msRegs(REG_11307B+Offset) = msRegs(REG_11307B+Offset)&(~BIT5);
		 #endif	 
	 #endif
    }
#endif

        //#if 1//DPCTS
#if ((!AUTO_EQ)||COMBO_DP_DAISY_CHAIN_SUPPORT)//DPCTS

#if(COMBO_DP_DAISY_CHAIN_SUPPORT)
		if(HBR2PORT == HBR2_0)
#endif
        {
        gDPInfo[HBR2PORT].CTSLevel = 0;

        msRegs(REG_11331E+Offset) = msRegs(REG_11331E+Offset) & 0x33;
        msRegs(REG_11331F+Offset) = msRegs(REG_11331F+Offset) & 0x33;
        while(((( msRegs(REG_113310+Offset)& 0x03 ) == 0x02 )||(( msRegs(REG_113310+Offset)& 0x03 ) == 0x03 )) && ( DPTP2Cnt < 50000 ) ) // while pattern 2 is On-Going
        {
            // if(DPTP2Cnt > 49990)
            // {
            //      msRegs(REG_113310) = 0x00;
            // }

            DPTP2Cnt++;
            switch( msRegs(REG_11330F+Offset) & 0x07 )
            {
                case 0x01:  // 1 lane
                    if((( msRegs(REG_113311+Offset) & 0x018 )>>3) == (( msRegs(REG_11331E+Offset) & 0x0C ) >>2)) // request reach
                    {
                        if(( msRegs(REG_11331A+Offset) & 0x06 ) != 0x06 ) // 1 lane LOCK Fail
                        {
                            if(  gDPInfo[HBR2PORT].CTSLevel < 3 )
                                gDPInfo[HBR2PORT].CTSLevel++;  // update swing level
                            msRegs(REG_11331E+Offset) =( msRegs(REG_11331E+Offset) & 0x03) | ((  gDPInfo[HBR2PORT].CTSLevel & 0x03 )<<2);
                            msRegs(REG_11331C+Offset)  = msRegs(REG_11331C+Offset) & 0x7F;
                            msRegs(REG_11331C+Offset) = msRegs(REG_11331C+Offset) | 0x80;
                        }
                    }
                    break;

                case 0x02:  // 2 lanes
                    if((( msRegs(REG_113311+Offset) & 0x18 )>>3) == (( msRegs(REG_11331E+Offset) & 0x0C )>>2) ) // request reach
                    {
                        if((( msRegs(REG_113312+Offset) & 0x18 )>>3) == (( msRegs(REG_11331E+Offset)& 0xC0 ) >> 6 ) ) // request reach
                        {
                            if(( msRegs(REG_11331A+Offset) & 0x66 ) != 0x66 ) // 2 lanes CDR LOCK Fail
                            {
                                if(  gDPInfo[HBR2PORT].CTSLevel < 3 )
                                    gDPInfo[HBR2PORT].CTSLevel++;  // update swing level
                                msRegs(REG_11331E+Offset)=( msRegs(REG_11331E+Offset) & 0x33) | (((  gDPInfo[HBR2PORT].CTSLevel & 0x03 ) << 6 ) | ((  gDPInfo[HBR2PORT].CTSLevel & 0x03 )<<2));
                                msRegs(REG_11331C+Offset)  = msRegs(REG_11331C+Offset) & 0x7F;
                                msRegs(REG_11331C+Offset) = msRegs(REG_11331C+Offset) | 0x80;
                            }
                        }
                    }
                    break;

                case 0x04:  // 4 lanes
                    if((( msRegs(REG_113311+Offset) & 0x18 )>>3) == (( msRegs(REG_11331E+Offset) & 0x0C )>>2) ) // request reach
                    {
                        if((( msRegs(REG_113312+Offset) & 0x18  )>>3) == (( msRegs(REG_11331E+Offset) & 0xC0 ) >> 6 ) ) // request reach
                        {
                            if((( msRegs(REG_113313+Offset) & 0x18  )>>3) == (( msRegs(REG_11331F+Offset) & 0x0C )>>2) ) // request reach
                            {
                                if((( msRegs(REG_113314+Offset) & 0x18  )>>3) == (( msRegs(REG_11331F+Offset) & 0xC0 ) >> 6 ) ) // request reach
                                {
                                    if((( msRegs(REG_11331A+Offset) & 0x66 ) != 0x66 ) | (( msRegs(REG_11331B+Offset) & 0x66 ) != 0x66 ) ) // 4 lanes CDR LOCK Fail
                                    {

                                        if(gDPInfo[HBR2PORT].CTSLevel < 3 )
                                            gDPInfo[HBR2PORT].CTSLevel++;  // update swing level
                                        msRegs(REG_11331E+Offset) =(msRegs(REG_11331E) & 0x33) | (((  gDPInfo[HBR2PORT].CTSLevel & 0x03 ) << 6 ) | ((  gDPInfo[HBR2PORT].CTSLevel & 0x03 )<<2));
                                        msRegs(REG_11331F+Offset)= (msRegs(REG_11331F) & 0x33) | (((  gDPInfo[HBR2PORT].CTSLevel & 0x03 ) << 6 ) | ((  gDPInfo[HBR2PORT].CTSLevel & 0x03 )<<2));
                                        msRegs(REG_11331C+Offset)  = msRegs(REG_11331C+Offset) & 0x7F;
                                        msRegs(REG_11331C+Offset) = msRegs(REG_11331C+Offset) | 0x80;
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

#if (ENABLE_STABLE_COUNT)
	msRegs(REG_113001+Offset) = msRegs(REG_113001+Offset) & 0xF7;
	msRegs(REG_113001+Offset) = msRegs(REG_113001+Offset) & 0xFB;
	msRegs(REG_113001+Offset) = msRegs(REG_113001+Offset) & 0xBF;
#endif
    	}
    #endif

        if(PortToCombo[HBR2PORT]&BIT0)
             msRegs(REG_0017A8 +(PortToCombo[HBR2PORT] >> 1) ) = (msRegs(REG_0017A8 + (PortToCombo[HBR2PORT] >> 1))&0x0F)|0x40;
        else
               msRegs(REG_0017A8 + (PortToCombo[HBR2PORT] >> 1)) =(msRegs(REG_0017A8 +(PortToCombo[HBR2PORT] >> 1) )&0xF0)|0x04;

	 DPRxClrSQTime();
        msRegs(REG_1133E8+Offset) = 0x02;                                                          // clear TPS2 IRQ
     msRegs(REG_1133EA+Offset) = 0x02;                                           // clear TPS3 IRQ


    }
}

//**************************************************************************
//  [Function Name]:
//                  mapi_DP_Initial()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_DP_Initial(void)
{
    DPRxInit();
    DPRxComboInit();
}

//**************************************************************************
//  [Function Name]:
//                  mapi_DP_Handler()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_DP_Handler(void)
{
    DPRxHandle();
    DPRxComboHandle();
}

//**************************************************************************
//  [Function Name]:
//                  mapi_DP_IsrHandler()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_DP_IsrHandler(void)
{
    ISR_DPHBR2(HBR2_0);
    ISR_DPHBR2(HBR2_1);

#if DP_COMBO_PORT
    mapi_DPCombo_ISR();
#endif
}


//**************************************************************************
//  [Function Name]:
//                  mapi_DP_GetColorFormate
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
ST_COMBO_COLOR_FORMAT mapi_DP_GetColorFormate(BYTE ucInputPort)
{
    BYTE ucHBR2Select = mdrv_DP_GetHBR2PortSelect(ucInputPort);
    ST_COMBO_COLOR_FORMAT stColorInfo = {COMBO_COLOR_FORMAT_DEFAULT, COMBO_COLOR_RANGE_DEFAULT, COMBO_COLORIMETRY_NONE, COMBO_YUV_COLORIMETRY_ITU601};

    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
	 stColorInfo.ucColorType =    (EN_COLOR_FORMAT_TYPE)gDPComboInfo[ucInputPort].ucDPColorFormat;
        stColorInfo.ucColorRange =  (EN_COLOR_RANGE_TYPE)gDPComboInfo[ucInputPort].ucDPRangeFormat;
        stColorInfo.ucColorimetry =  (EN_COLORIMETRY_TYPE)gDPComboInfo[ucInputPort].bColorType;
    }
    else // HBR2 path
    {
        stColorInfo.ucColorType = (EN_COLOR_FORMAT_TYPE)gDPInfo[HBRPort(ucInputPort)].ucDPColorFormat;
        stColorInfo.ucColorRange = (EN_COLOR_RANGE_TYPE)gDPInfo[HBRPort(ucInputPort)].ucDPRangeFormat;
        stColorInfo.ucColorimetry = (EN_COLORIMETRY_TYPE)gDPInfo[HBRPort(ucInputPort)].bColorType;
    }

    return stColorInfo;
}

//**************************************************************************
//  [Function Name]:
//                  mapi_DP_CheckSourceLock()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mapi_DP_CheckSourceLock(BYTE ucInputPort)
{
    Bool bSourceLock = FALSE;
    BYTE ucHBR2Select = mdrv_DP_GetHBR2PortSelect(ucInputPort);

    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        if((gDPComboInfo[ucInputPort].ucState > eComSTATE_CKECKLOCK) && (gDPComboInfo[ucInputPort].ucState <= eComSTATE_NORMAL))
        {
            bSourceLock = TRUE;
        }
    }
    else // HBR2 path
    {
        if((gDPInfo[HBRPort(ucInputPort)].ucState > eSTATE_CKECKLOCK) && (gDPInfo[HBRPort(ucInputPort)].ucState <= eSTATE_NORMAL))
        {
            bSourceLock = TRUE;
        }
    }

    return bSourceLock;
}

//**************************************************************************
//  [Function Name]:
//                  mapi_DP_Set_Version()
//  [Description]
//                  mapi_DP_Set_Version
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_DP_Set_Version(BYTE ucTemp, BYTE ucVersion)
{
    BYTE ucHBR2Select = mdrv_DP_GetHBR2PortSelect(ucTemp);   //scaler Port to Combo

	DPVersionSetting(ucHBR2Select, ucVersion);
}

//**************************************************************************
//  [Function Name]:
//                  mapi_DP_GetDRRFlag()
//  [Description]
//                  mapi_DP_GetDRRFlag
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mapi_DP_GetDRRFlag(BYTE ucInputPort)
{
	return DPRxGetDRRFlag(ucInputPort);
}

//**************************************************************************
//  [Function Name]:
//                  mapi_DP_EnableDRRFunction()
//  [Description]
//                  mapi_DP_EnableDRRFunction
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_DP_EnableDRRFunction(BYTE ucInputPort, Bool Enable)
{
    BYTE ucHBR2Select = mdrv_DP_GetHBR2PortSelect(ucInputPort);   //scaler Port to Combo

	DPRxEnableDRRFlag(ucHBR2Select, Enable);
}

//**************************************************************************
//  [Function Name]:
//                  mapi_DP_GetPixelClock()
//  [Description]
//                  mapi_DP_GetPixelClock
//  [Arguments]:
//
//  [Return]:
//                  Pixel clock = xxx MHz
//**************************************************************************
WORD mapi_DP_GetPixelClock(BYTE ucInputPort)
{
    BYTE ucHBR2Select = mdrv_DP_GetHBR2PortSelect(ucInputPort);
    WORD xdata LS_Clock=0;
    WORD xdata Pixel_Clock=0;
    DWORD xdata Base_N=0;
    DWORD xdata Base_M=0;
    WORD DP_Combo_Offset=0;

	if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
	{
        if((gDPComboInfo[ucInputPort].ucState > eComSTATE_CKECKLOCK) && (gDPComboInfo[ucInputPort].ucState <= eComSTATE_NORMAL))
        {
			DP_Combo_Offset = DPRxComboOffset(ucInputPort);
			
			LS_Clock = 27 * msReadByte(REG_160780+DP_Combo_Offset);
			DP_printData( "Combo DP Input : LS_Clock %d ", LS_Clock );
			DP_printData( "Combo DP Input : Lane %d ", msReadByte(REG_160781+DP_Combo_Offset)&0x0F);
			Base_M = ((DWORD) msRead2Byte( REG_1603C1+DP_Combo_Offset ) <<8) + msReadByte(REG_1603C0+DP_Combo_Offset);
			DP_printData( "Combo DP Input : Base_M %x ", Base_M );
			Base_N = ((DWORD)msRead2Byte(REG_1603C4+DP_Combo_Offset ) <<8) + msReadByte(REG_1603C3+DP_Combo_Offset);
			if(Base_N == 0)
			{
				Base_N = 1;
				DP_printData( "Combo DP Input : Base_N Overwrite from 0 to 1, %x ", Base_N ); // prevent /0 exception, due to temporal unstable Base_N
			}
			DP_printData( "Combo DP Input : Base_N %x ", Base_N );
			Pixel_Clock = ((DWORD) Base_M * LS_Clock ) / Base_N ;
			DP_printData( "Combo DP Input : Pixel_Clock %d ", Pixel_Clock );
        }
	}
	else // HBR2 path
	{
        if((gDPInfo[HBRPort(ucInputPort)].ucState > eSTATE_CKECKLOCK) && (gDPInfo[HBRPort(ucInputPort)].ucState <= eSTATE_NORMAL))
        {
			LS_Clock = 27 * msReadByte(REG_11330E+ucHBR2Select*HBR2_OFFSET);
			DP_printData( "DP Input : LS_Clock %d ", LS_Clock );
			Base_M = ((DWORD) msRead2Byte( REG_113387+ucHBR2Select*HBR2_OFFSET ) <<8) + msReadByte(REG_113386+ucHBR2Select*HBR2_OFFSET);
			DP_printData( "DP Input : Base_M %x ", Base_M );
			Base_N = ((DWORD)msRead2Byte( REG_11338B+ucHBR2Select*HBR2_OFFSET ) <<8) + msReadByte(REG_11338A+ucHBR2Select*HBR2_OFFSET);
			DP_printData( "DP Input : Base_N %x ", Base_N );
			if(Base_N == 0)
			{
				Base_N = 1;
				DP_printData( "DP Input : Base_N Overwrite from 0 to 1, %x ", Base_N ); // prevent /0 exception, due to temporal unstable Base_N
			}
			Pixel_Clock = ((DWORD) Base_M * LS_Clock ) / Base_N ;
			DP_printData( "DP Input : Pixel_Clock %d Mhz", Pixel_Clock );
        }
    }

	return Pixel_Clock ;
	
}

//**************************************************************************
//  [Function Name]:
//                  mapi_DP_GetPixelClock_10KHz()
//  [Description]
//                  mapi_DP_GetPixelClock
//  [Arguments]:
//
//  [Return]:
//                  Pixel clock = XXX x 10KHz, not MHz
//**************************************************************************
WORD mapi_DP_GetPixelClock_10KHz(BYTE ucInputPort)
{
    BYTE ucHBR2Select = mdrv_DP_GetHBR2PortSelect(ucInputPort);
    WORD xdata LS_Clock=0;
    WORD xdata Pixel_Clock=0;
    DWORD xdata Base_N=0;
    DWORD xdata Base_M=0;
    WORD DP_Combo_Offset=0;

	if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
	{
        if((gDPComboInfo[ucInputPort].ucState > eComSTATE_CKECKLOCK) && (gDPComboInfo[ucInputPort].ucState <= eComSTATE_NORMAL))
        {
			DP_Combo_Offset = DPRxComboOffset(ucInputPort);
			
			LS_Clock = 27 * msReadByte(REG_160780+DP_Combo_Offset);
			DP_printData( "Combo DP Input : LS_Clock %d ", LS_Clock );
			DP_printData( "Combo DP Input : Lane %d ", msReadByte(REG_160781+DP_Combo_Offset)&0x0F);
			Base_M = ((DWORD) msRead2Byte( REG_1603C1+DP_Combo_Offset ) <<8) + msReadByte(REG_1603C0+DP_Combo_Offset);
			DP_printData( "Combo DP Input : Base_M %x ", Base_M );
			Base_N = ((DWORD)msRead2Byte(REG_1603C4+DP_Combo_Offset ) <<8) + msReadByte(REG_1603C3+DP_Combo_Offset);
			if(Base_N == 0)
			{
				Base_N = 1;
				DP_printData( "Combo DP Input : Base_N Overwrite from 0 to 1, %x ", Base_N ); // prevent /0 exception, due to temporal unstable Base_N
			}
			DP_printData( "Combo DP Input : Base_N %x ", Base_N );
			Pixel_Clock = ((DWORD) Base_M * LS_Clock * 100) / Base_N ;
			DP_printData( "Combo DP Input : Pixel_Clock %d ", Pixel_Clock );
        }
	}
	else // HBR2 path
	{
        if((gDPInfo[HBRPort(ucInputPort)].ucState > eSTATE_CKECKLOCK) && (gDPInfo[HBRPort(ucInputPort)].ucState <= eSTATE_NORMAL))
        {
			LS_Clock = 27 * msReadByte(REG_11330E+ucHBR2Select*HBR2_OFFSET);
			DP_printData( "DP Input : LS_Clock %d ", LS_Clock );
			Base_M = ((DWORD) msRead2Byte( REG_113387+ucHBR2Select*HBR2_OFFSET ) <<8) + msReadByte(REG_113386+ucHBR2Select*HBR2_OFFSET);
			DP_printData( "DP Input : Base_M %x ", Base_M );
			Base_N = ((DWORD)msRead2Byte( REG_11338B+ucHBR2Select*HBR2_OFFSET ) <<8) + msReadByte(REG_11338A+ucHBR2Select*HBR2_OFFSET);
			DP_printData( "DP Input : Base_N %x ", Base_N );
			if(Base_N == 0)
			{
				Base_N = 1;
				DP_printData( "DP Input : Base_N Overwrite from 0 to 1, %x ", Base_N ); // prevent /0 exception, due to temporal unstable Base_N
			}
			//Pixel_Clock = ((DWORD) Base_M * LS_Clock * 100) / Base_N ;
			Pixel_Clock = DPRxLongDIV(Base_M, LS_Clock, Base_N);
			DP_printData( "DP Input : Pixel_Clock %d Mhz", Pixel_Clock );
        }
    }

	return Pixel_Clock ;
	
}

//**************************************************************************
//  [Function Name]:
//                  mapi_DP_DaisychainFlag()
//  [Description]
//                  mapi_DP_DaisychainFlag
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_DP_DaisychainFlag(BYTE ucDaisyChain)
{
	ucDPDaisyChainFlag = ucDaisyChain;
}


//**************************************************************************
//  [Function Name]:
//                  mapi_DP_DaisychainFlag()
//  [Description]
//                  mapi_DP_DaisychainFlag
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
 BYTE mapi_DP_GetAudioFreq(BYTE ucInputPort)
{       
      	 return gDPInfo[HBRPort(ucInputPort)].bDPAudioFreq;
}

//**************************************************************************
//  [Function Name]:
//                  mapi_DP_DaisychainFlag()
//  [Description]
//                  mapi_DP_DaisychainFlag
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void  mapi_DP_PowerOnOff(BYTE ucInputPort,  Bool Enable)
{
	UNUSED(ucInputPort);
	UNUSED(Enable);
}
//                  mapi_DP_DaisychainFlag
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MSCombo_DP_AUDIO_FORMAT mapi_DP_IsAudioFmtPCM(BYTE ucInputPort)
{
	BYTE temp;
	MSCombo_DP_AUDIO_FORMAT states = DP_AUDIO_FMT_PCM;
	 	UNUSED(ucInputPort);
		temp = msReadByte(REG_163088)&0x0F;        
		msWriteByteMask(REG_163088, 0x0C, 0x0F); // Switch to DP
		if(msReadByte(REG_163094)&BIT1)
		states =  DP_AUDIO_FMT_NON_PCM;
		msWriteByteMask(REG_163088, temp, 0x0F); 
  
    return states;
}

//**************************************************************************
//  [Function Name]:
//                  mapi_DP_DaisychainFlag()
//  [Description]
//                  mapi_DP_DaisychainFlag
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void  mapi_DP_AudioInfo(BYTE ucInputPort, BYTE *pPacketData)
{
	if(msReadByte(REG_1132F1 +HBRPort(ucInputPort)*HBR2_OFFSET) >= 2 )
	pPacketData[0] = 8;
	else
	pPacketData[0] = 2;
}
#endif



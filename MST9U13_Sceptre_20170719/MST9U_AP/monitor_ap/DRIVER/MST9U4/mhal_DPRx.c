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

#define  DisableDPSSC()                               (msWriteByteMask( REG_162F22, 0,BIT6))
#define  EnableDPSSC()                                (msWriteByteMask( REG_162F22, BIT6,BIT6 ))

#define  DisableDPMute()                             (msWriteByteMask( REG_163088, 0,BIT4 ))
#define  EnableDPMute()                              (msWriteByteMask( REG_163088, BIT4,BIT4))

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
 WORD _mhal_DP_ComboLinkRegisterOffset(DPRx_ID dprx_id)
 {
    WORD usLinkRegOffset = dprx_id *0x600;

    if(dprx_id > 0)
    {
        usLinkRegOffset = usLinkRegOffset +(dprx_id -1) *0x200;
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
//                  _mhal_DP_GetLinkRate()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
WORD _mhal_DP_GetLinkRate(DPRx_ID dprx_id, BYTE ucHBR2Select)
{
    WORD usLinkRate = 0;
    
    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        dprx_id = DPRx_ID_0;
    }
    else // HBR2 path
    {
        usLinkRate = 27 *msReadByte(REG_DPRX_AUX_00_H +DP_PHY_REG_OFFSET(dprx_id));  
    }

    return usLinkRate;
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
BYTE g_ucDDC_TxRecord=0;
BYTE dpNullMsg[3]={0x6e, 0x80, 0xbe};
BYTE u8DDC_RxRecord = 0;
BYTE AuxIndex=0;
void _mhal_DP_MCCSReceiveProc(DPRx_ID dprx_id)
{
    BYTE AuxData;
    BYTE u8AUXCH_RxCmd;
    BYTE u8DDC_Length;

    u8AUXCH_RxCmd = msRegs(REG_001138 + DP_PHY_REG_OFFSET(dprx_id))&0x0F;
    u8DDC_Length = msRegs(REG_00113A+ DP_PHY_REG_OFFSET(dprx_id));
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
                        msRegs(REG_0011D3 + DP_PHY_REG_OFFSET(dprx_id))=DDCBuffer[g_ucDDC_TxRecord-1];
                        //msRegs(REG_0011D3 + Aux)=GUIDData[g_ucDDC_TxRecord-1];
                        txLength--;
                    }
                    else
                        msRegs(REG_0011D3 + DP_PHY_REG_OFFSET(dprx_id))=DDC2B_DEST_ADDRESS;
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
                    
                    msRegs(REG_0011D3 + DP_PHY_REG_OFFSET(dprx_id))= dpNullMsg[g_ucDDC_TxRecord++];
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
                msRegs(REG_0011FC + DP_PHY_REG_OFFSET(dprx_id))= msRegs(REG_0011FC + DP_PHY_REG_OFFSET(dprx_id))|_BIT2;
                //_nop_();
                AuxData= msRegs(REG_001175 + DP_PHY_REG_OFFSET(dprx_id));
                              
                u8DDC_Length--;
            }
            while(u8DDC_Length--)
            {
                WORD  temp3=50;
                 msRegs(REG_0011FC + DP_PHY_REG_OFFSET(dprx_id))= msRegs(REG_0011FC + DP_PHY_REG_OFFSET(dprx_id))|_BIT2;
                 
                while(temp3--)
                {
                    _nop_();
                };
                DDCBuffer[rxIndex++] = msRegs(REG_001175 + DP_PHY_REG_OFFSET(dprx_id));
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

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_HPDControl() // cyc : need mailbox
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_DP_HPDControl(DPRx_ID dprx_id, Bool bSetHPD)
{
    switch(dprx_id)
    {
        case DPRx_ID_0:
            if(bSetHPD)
            {
                SET_DP_HPD_PORT0();
            }
            else
            {
                CLR_DP_HPD_PORT0();
            }
            
            break;

        case DPRx_ID_1:
            if(bSetHPD)
            {
                SET_DP_HPD_PORT1();
            }
            else
            {
                CLR_DP_HPD_PORT1();
            }
            
            break;

        case DPRx_ID_2:
            if(bSetHPD)
            {
                SET_DP_HPD_PORT2();
            }
            else
            {
                CLR_DP_HPD_PORT2();
            }
            
            break;

     

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_CableDetect() // cyc : need to modify
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mhal_DP_CableDetect(DPRx_ID dprx_id)
{
    Bool bCableDetect = FALSE;
    
    switch(dprx_id)
    {
        case DPRx_ID_0:
            bCableDetect = (DP_CABLE_DETECT_PORT0 ?FALSE: TRUE);
            break;

        case DPRx_ID_1:
            bCableDetect = (DP_CABLE_DETECT_PORT1 ?FALSE: TRUE);
            break;

        case DPRx_ID_2:
            bCableDetect = (DP_CABLE_DETECT_PORT2 ?FALSE: TRUE);
            break;    

        default:

            break;
    };

    return bCableDetect;
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
//                  mhal_DP_CheckCDRLock()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mhal_DP_CheckCDRLock(DPRx_ID dprx_id, BYTE ucHBR2Select)
{
    Bool bCDRLockFlag = FALSE;
    BYTE ucLansCount = 0;
    WORD usLinkRegOffset = _mhal_DP_ComboLinkRegisterOffset(dprx_id); // This function only use in combo case, not work
    WORD usLockStatus = 0;

   
    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        usLinkRegOffset = 0;
    }
    else // HBR2 path
    {
        //ucLansCount = msReadByte(REG_11330F +DP_PHY_REG_OFFSET(dprx_id)) &0x07;
        //usLockStatus = msRead2Byte(REG_11331A +DP_PHY_REG_OFFSET(dprx_id));
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
//                  mhal_DP_GetTimingPixelClock()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  Pixel clock = xxx MHz
//**************************************************************************
WORD mhal_DP_GetTimingPixelClock(DPRx_ID dprx_id, BYTE ucHBR2Select)
{
    WORD usPixelClock = 0;
    WORD usLSClock = _mhal_DP_GetLinkRate(dprx_id, ucHBR2Select);
    DWORD ulBaseMValue = 0;
    DWORD ulBaseNValue = 0;
    
    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        dprx_id = DPRx_ID_0;
    }
    else // HBR2 path
    {
       // ulBaseMValue = ((DWORD)msRead2Byte(REG_113585 +DP_PHY_REG_OFFSET(dprx_id)) <<8) + msReadByte(REG_113584 +DP_PHY_REG_OFFSET(ucHBR2Select));
       // ulBaseNValue = ((DWORD)msRead2Byte(REG_11358D +DP_PHY_REG_OFFSET(dprx_id)) <<8) + msReadByte(REG_11358C +DP_PHY_REG_OFFSET(ucHBR2Select));
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
WORD mhal_DP_GetTimingPixelClock_10KHz(DPRx_ID dprx_id, BYTE ucHBR2Select)
{
    WORD usPixelClock = 0;
    WORD usLSClock = _mhal_DP_GetLinkRate(dprx_id, ucHBR2Select);
    DWORD ulBaseMValue = 0;
    DWORD ulBaseNValue = 0;
    
    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        dprx_id = DPRx_ID_0;
    }
    else // HBR2 path
    {
      //  ulBaseMValue = ((DWORD)msRead2Byte(REG_113585 +DP_PHY_REG_OFFSET(dprx_id)) <<8) + msReadByte(REG_113584 +DP_PHY_REG_OFFSET(ucHBR2Select));
      //  ulBaseNValue = ((DWORD)msRead2Byte(REG_11358D +DP_PHY_REG_OFFSET(dprx_id)) <<8) + msReadByte(REG_11358C +DP_PHY_REG_OFFSET(ucHBR2Select));
    }

    if(ulBaseNValue > 0)
    {
        usPixelClock = ((DWORD)ulBaseMValue *usLSClock *100) /ulBaseNValue;
    }

    return usPixelClock;
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
Bool mhal_DP_CheckReceiveIdlePattern(DPRx_ID dprx_id, BYTE ucHBR2Select)
{
    Bool bIdlePatternFlag = FALSE;
    
    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        dprx_id = DPRx_ID_0;
    }
    else // HBR2 path
    {
       // bIdlePatternFlag = ((msReadByte(REG_1135F4 +DP_PHY_REG_OFFSET(dprx_id)) &BIT3) ?TRUE: FALSE);
    }

    return bIdlePatternFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_CheckDPCDPowerState() // cyc : no need
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mhal_DP_CheckDPCDPowerState(DPRx_ID dprx_id)
{
    Bool bPowerStateFlag = 0;

	bPowerStateFlag = ((msReadByte(REG_001172 +DP_PHY_REG_OFFSET(dprx_id)) &BIT0) ?TRUE: FALSE);

    return bPowerStateFlag;
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
Bool mhal_DP_GetMCCSReceiveFlag(DPRx_ID dprx_id)
{
    Bool bEventFlag = 0;

    bEventFlag = ((msReadByte(REG_001178 +DP_PHY_REG_OFFSET(dprx_id)) &BIT7) ?TRUE: FALSE);    

    if(bEventFlag)
    {
        _mhal_DP_MCCSReceiveProc(dprx_id);

        // Clear int, then trigger HW send ACK and clear DDC buffer
        msWriteByteMask(REG_001179 +DP_PHY_REG_OFFSET(dprx_id), BIT7, BIT7);
        msWriteByteMask(REG_001179 +DP_PHY_REG_OFFSET(dprx_id), 0, BIT7);
    }

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
void mhal_DP_SetGuardBand(DPRx_ID dprx_id, BYTE ucHBR2Select, WORD usGuardBandValue)
{
    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        dprx_id = DPRx_ID_0;
    }
    else // HBR2 path
    {
    	  usGuardBandValue =0;
       //msWrite2ByteMask(REG_113554 +DP_PHY_REG_OFFSET(dprx_id), usGuardBandValue, MASK_11BIT);
    }
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
void mhal_DP_AUPLLSetting(DPRx_ID dprx_id)
{
	
	dprx_id =DPRx_ID_0;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_VersionSetting() // cyc : need mailbox
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_DP_VersionSetting(DPRx_ID dprx_id, BYTE ucHBR2Select, BYTE ucVersion )
{

    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        dprx_id = DPRx_ID_0;
    }
    else // HBR2 path
    {
		if(ucVersion == E_DP_V11)
		{
			//msWriteByte(REG_113300 +DP_PHY_REG_OFFSET(dprx_id), 0x11 );	 // DPCD 1.1 Version
			//msWriteByte(REG_113301 +DP_PHY_REG_OFFSET(dprx_id), 0x0A );    // DP main link rate 2.7G
		}
		else
		{
			//msWriteByte(REG_113300 +DP_PHY_REG_OFFSET(dprx_id), 0x12 );    // DPCD 1.2 Version
			//msWriteByte(REG_113301 +DP_PHY_REG_OFFSET(dprx_id), 0x14 );	 // DP main link rate 5.4G
		}
		
		if(ucVersion == E_DP_V11)
		{
			//msWriteByte(REG_001100 +DP_PHY_REG_OFFSET(dprx_id), 0x11 );    // DPCD 1.1 Version
			//msWriteByte(REG_001101 +DP_PHY_REG_OFFSET(dprx_id), 0x0A );	// DP main link rate 2.7G												   // Support 1.62G 													 // Support 1.62G
			//msWriteByte(REG_001103 +DP_PHY_REG_OFFSET(dprx_id), 0x89);	    // non-Support TP3
		}
		else
		{
			//msWriteByte(REG_001100 +DP_PHY_REG_OFFSET(dprx_id), 0x12 );    // DPCD 1.2 Version
			//msWriteByte(REG_001101 +DP_PHY_REG_OFFSET(dprx_id), 0x14 );	// DP main link rate 5.4G													 // Support 1.62G
			//(REG_001103 +DP_PHY_REG_OFFSET(dprx_id), 0x8B);	    // Support TP3
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
Bool mhal_DP_GetDRRFlag(DPRx_ID dprx_id, BYTE ucHBR2Select)
{

    Bool bDRRFlag = FALSE;

    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        dprx_id = DPRx_ID_0;
    }
    else // HBR2 path
    {
       // if((msReadByte(REG_113147 +DP_PHY_REG_OFFSET(dprx_id)) & BIT7) == BIT7)
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
void mhal_DP_EnableDRRFunction(DPRx_ID dprx_id, BYTE ucHBR2Select, Bool bEnable)
{

    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        dprx_id = DPRx_ID_0;
    }
    else // HBR2 path
    {
        if(bEnable == TRUE)
            msWriteByteMask(REG_001103 +DP_PHY_REG_OFFSET(dprx_id), BIT6, BIT6);
		else
            msWriteByteMask(REG_001103 +DP_PHY_REG_OFFSET(dprx_id), 0, BIT6);
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
Bool mhal_DP_GetMSAChgFlag(DPRx_ID dprx_id,BYTE ucHBR2Select)
{	 
	ucHBR2Select =0;
       return ((msReadByte(REG_1135E2 +DP_PHY_REG_OFFSET(dprx_id))&BIT4) ? TRUE : FALSE);
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
void mhal_DP_ClrMSAChgFlag(DPRx_ID dprx_id,BYTE ucHBR2Select)
{   

     ucHBR2Select = ucHBR2Select;
     msWriteByteMask(REG_1135E8 +DP_PHY_REG_OFFSET(dprx_id), BIT4, BIT4);
     msWriteByteMask(REG_1135E8 +DP_PHY_REG_OFFSET(dprx_id), 0, BIT4);
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
Bool mhal_DP_GetInterlaceFlag(DPRx_ID dprx_id, BYTE ucHBR2Select)
{
    Bool bInterlaceFlag = 0;

    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        dprx_id = DPRx_ID_0;
    }
    else // HBR2 path
    {   
		bInterlaceFlag = ((msReadByte(REG_1135F4 +DP_PHY_REG_OFFSET(dprx_id)) & BIT2) ? TRUE : FALSE);
    }

	return bInterlaceFlag;
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
BYTE mhal_DP_CalAudioFreq(DPRx_ID dprx_id, BYTE ucHBR2Select)
{

    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        dprx_id = DPRx_ID_0;
    }
    else // HBR2 path
    {

    }
	
	return 1;
}
//**************************************************************************
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MSCombo_DP_AUDIO_FORMAT mhal_DP_IsAudioFmtPCM(DPRx_ID dprx_id,BYTE ucHBR2Select)
{
	BYTE temp;
	MSCombo_DP_AUDIO_FORMAT states = DP_AUDIO_FMT_PCM;
	dprx_id =DPRx_ID_0;
	ucHBR2Select=0;

	
	temp = msReadByte(REG_163088)&0x0F;        
	msWriteByteMask(REG_163088, 0x0C, 0x0F); // Switch to DP
	if(msReadByte(REG_163094)&BIT1)
	states =  DP_AUDIO_FMT_NON_PCM;
	msWriteByteMask(REG_163088, temp, 0x0F); 

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
 void mhal_DP_AudioInfo(DPRx_ID dprx_id, BYTE ucHBR2Select, BYTE *pPacketData)
 {
		   
	if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
	{
	 	dprx_id =DPRx_ID_0;	 
	    
	}
	else // HBR2 path
	{		
		if(msReadByte(REG_1132F1 + DP_PHY_REG_OFFSET(dprx_id))>= 2 )
		pPacketData[0] = 8;
		else
		pPacketData[0] = 2;	
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
Bool mhal_DP_GetTimingInformation(DPRx_ID dprx_id, BYTE ucHBR2Select, WORD *usTimingInfo)
{
    Bool bInterlace = FALSE;

    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        dprx_id =DPRx_ID_0;
    }
    else // HBR2 path
    {
        bInterlace = ((msReadByte(REG_1133A5 +DP_PHY_REG_OFFSET(dprx_id)) &BIT2) ?TRUE: FALSE);

        usTimingInfo[0] = msRead2Byte(REG_113376 +DP_PHY_REG_OFFSET(dprx_id)) &0x7FFF; // H total
        usTimingInfo[1] = msRead2Byte(REG_113378 +DP_PHY_REG_OFFSET(dprx_id)) &0x7FFF; // V total
        usTimingInfo[2] = msRead2Byte(REG_11337A +DP_PHY_REG_OFFSET(dprx_id)) &0x7FFF; // H width
        usTimingInfo[3] = msRead2Byte(REG_11337C +DP_PHY_REG_OFFSET(dprx_id)) &0x7FFF; // V width
        usTimingInfo[4] = msRead2Byte(REG_11337E +DP_PHY_REG_OFFSET(dprx_id)) &0x7FFF; // H start
        usTimingInfo[5] = msRead2Byte(REG_113380 +DP_PHY_REG_OFFSET(dprx_id)) &0x7FFF; // V start
        usTimingInfo[6] = msRead2Byte(REG_113382 +DP_PHY_REG_OFFSET(dprx_id)); // H PWS
        usTimingInfo[7] = msRead2Byte(REG_113384 +DP_PHY_REG_OFFSET(dprx_id)); // V PWS
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
void mhal_DP_RegenTimingInformation(DPRx_ID dprx_id, BYTE ucHBR2Select, WORD *usTimingInfo)
{
    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        dprx_id = DPRx_ID_0;
    }
    else // HBR2 path
    {
        msWrite2Byte(REG_11350A +DP_PHY_REG_OFFSET(dprx_id), msRead2Byte(REG_11350A +DP_PHY_REG_OFFSET(ucHBR2Select))&0x7FFF);
        msWriteByteMask(REG_113508 +DP_PHY_REG_OFFSET(dprx_id), MASKBIT(1:0), MASKBIT(1:0));
        // V parameter
        msWrite2Byte(REG_11350C +DP_PHY_REG_OFFSET(dprx_id), (usTimingInfo[0] &0x7FFF)); // V width
        msWriteByte(REG_113540 +DP_PHY_REG_OFFSET(dprx_id), usTimingInfo[1]); // V front porch
        msWriteByte(REG_1135A7 +DP_PHY_REG_OFFSET(dprx_id), (usTimingInfo[2]) << 1); // V back porch
        msWriteByteMask(REG_113501 +DP_PHY_REG_OFFSET(dprx_id), BIT4, BIT4);
        // H parameter
        msWrite2Byte(REG_11350A +DP_PHY_REG_OFFSET(dprx_id), usTimingInfo[3]); // H width
    }
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
void mhal_DP_GetMISC01(DPRx_ID dprx_id, BYTE ucHBR2Select, WORD *usMISCInfo)
{

    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        dprx_id = DPRx_ID_0;
    }
    else // HBR2 path
    {
        msWriteByteMask(REG_1133BC +DP_PHY_REG_OFFSET(dprx_id), BIT6, BIT6);
        
        usMISCInfo[1] = msRead2Byte(REG_113359 +DP_PHY_REG_OFFSET(dprx_id)); // MISC1

        msWriteByteMask(REG_1133BC +DP_PHY_REG_OFFSET(dprx_id), 0, BIT6);

        usMISCInfo[0] = msRead2Byte(REG_113359 +DP_PHY_REG_OFFSET(dprx_id)); // MISC0
    }

 
}

//**************************************************************************
//  [Function Name]:
//                  mhal_DP_GetMISC01()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_DP_GetVscSdp(DPRx_ID dprx_id, BYTE ucHBR2Select, WORD *usMISCInfo)
{
    
    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        dprx_id = DPRx_ID_0;
    }
    else // HBR2 path
    {
    	usMISCInfo[0] = msRead2Byte(REG_113359 +DP_PHY_REG_OFFSET(dprx_id)); // SDP DB16
 	usMISCInfo[1] = msRead2Byte(REG_113359 +DP_PHY_REG_OFFSET(dprx_id)); // SDP DB17
	usMISCInfo[2] = msRead2Byte(REG_113359 +DP_PHY_REG_OFFSET(dprx_id)); // SDP DB18
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
WORD mhal_DP_GetPacketStatus(DPRx_ID dprx_id, BYTE ucHBR2Select)
{
    WORD usPacketStatus = 0;

    if(ucHBR2Select == DP_HBR2_ENGINE_NONE) // Combo path
    {
        dprx_id = DPRx_ID_0;
    }
    else // HBR2 path
    {
	usPacketStatus = msRead2Byte(REG_1132AA +DP_PHY_REG_OFFSET(dprx_id));

	msWrite2Byte(REG_1132AC +DP_PHY_REG_OFFSET(dprx_id), 0xFFFF);
	msWrite2Byte(REG_1132AC +DP_PHY_REG_OFFSET(dprx_id), 0);
    }

    return usPacketStatus;
}


//**************************************************************************
//  [Function Name]:
//                  DPMST_ISR_McuIndexWriteDPCD()
//  [Description]
//                  DPMST_ISR_McuIndexWriteDPCD
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void  mhal_DPRx_MCUWritePMDPCD(DPRx_ID dprx_id,DWORD ulDPCDAddress , BYTE  ucDATA)
{


	BYTE XDATA ubTimeOutLimt = 0x5;

	
	//Address overwrite enable
	msWriteByteMask(REG_DPRX_AUX_67_H+ DPRX_PHY_REG_OFFSET(dprx_id), BIT4, BIT4);   //Enable
	//20 bit DPCD address
	//msWrite2Byte(REG_003010, ((ulDPCDAddress + i)&0xFFFF));
	msWriteByte(REG_DPRX_AUX_66_L+ DPRX_PHY_REG_OFFSET(dprx_id), ((ulDPCDAddress)&0xFF));
	msWriteByte(REG_DPRX_AUX_66_H+ DPRX_PHY_REG_OFFSET(dprx_id), (((ulDPCDAddress)>> 0x8)&0xFF));
	msWriteByte(REG_DPRX_AUX_67_L+ DPRX_PHY_REG_OFFSET(dprx_id), ((ulDPCDAddress)>>0x10)&0xF);

	msWriteByte(REG_DPRX_AUX_68_H+ DPRX_PHY_REG_OFFSET(dprx_id), ucDATA);
		
	msWriteByteMask(REG_DPRX_AUX_67_H+ DPRX_PHY_REG_OFFSET(dprx_id), BIT7, BIT7);    //trigger MUC write
	
	do {
			DELAY_NOP(0x4);	
			ubTimeOutLimt --;
	} while((msReadByte(REG_DPRX_AUX_67_H+ DPRX_PHY_REG_OFFSET(dprx_id)) & BIT5)  && (ubTimeOutLimt > 0));

	msWriteByteMask(REG_DPRX_AUX_67_H+ DPRX_PHY_REG_OFFSET(dprx_id), 0, BIT4);	    //Disable
	
	
}

//**************************************************************************
//  [Function Name]:
//                  DPMST_ISR_McuIndexWriteDPCD()
//  [Description]
//                  DPMST_ISR_McuIndexWriteDPCD
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE  mhal_DPRx_MCUReadPMDPCD(DPRx_ID dprx_id, DWORD ulDPCDAddress)
{


	BYTE XDATA ubTimeOutLimt = 0x5;
	BYTE XDATA Data=0;
		
	//Address overwrite enable
	msWriteByteMask(REG_DPRX_AUX_67_H+ DPRX_PHY_REG_OFFSET(dprx_id), BIT4, BIT4);   //Enable
	//20 bit DPCD address
	//msWrite2Byte(REG_003010, ((ulDPCDAddress + i)&0xFFFF));
	msWriteByte(REG_DPRX_AUX_66_L+ DPRX_PHY_REG_OFFSET(dprx_id), ((ulDPCDAddress)&0xFF));
	msWriteByte(REG_DPRX_AUX_66_H+ DPRX_PHY_REG_OFFSET(dprx_id), (((ulDPCDAddress)>> 0x8)&0xFF));
	msWriteByte(REG_DPRX_AUX_67_L+ DPRX_PHY_REG_OFFSET(dprx_id), ((ulDPCDAddress)>>0x10)&0xF);

	msWriteByteMask(REG_DPRX_AUX_67_H+ DPRX_PHY_REG_OFFSET(dprx_id), BIT6, BIT6);    //trigger MUC Read
	
	do {
			DELAY_NOP(0x4);	
			ubTimeOutLimt --;
	} while((msReadByte(REG_DPRX_AUX_67_H+ DPRX_PHY_REG_OFFSET(dprx_id)) & BIT5)  && (ubTimeOutLimt > 0));

	Data = msReadByte(REG_DPRX_AUX_68_H+ DPRX_PHY_REG_OFFSET(dprx_id));
		
	msWriteByteMask(REG_DPRX_AUX_67_H+ DPRX_PHY_REG_OFFSET(dprx_id), 0, BIT6);	    //Disable
	
	return Data;
	
}



//**************************************************************************
//  [Function Name]:
//                  DPMST_ISR_McuIndexWriteDPCD()
//  [Description]
//                  DPMST_ISR_McuIndexWriteDPCD
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void  mhal_DPRx_MCUWriteNonPMDPCD(DPRx_ID dprx_id, DWORD ulDPCDAddress , BYTE  ucDATA)
{


	BYTE XDATA ubTimeOutLimt = 0x5;

	
	//Address overwrite enable
	msWriteByteMask(REG_DPRX_AUX_76_H+ DPRX_PHY_REG_OFFSET(dprx_id), BIT4, BIT4);   //Enable
	//20 bit DPCD address
	//msWrite2Byte(REG_003010, ((ulDPCDAddress + i)&0xFFFF));
	msWriteByte(REG_DPRX_AUX_75_L+ DPRX_PHY_REG_OFFSET(dprx_id), ((ulDPCDAddress)&0xFF));
	msWriteByte(REG_DPRX_AUX_75_H+ DPRX_PHY_REG_OFFSET(dprx_id), (((ulDPCDAddress)>> 0x8)&0xFF));
	msWriteByte(REG_DPRX_AUX_76_L+ DPRX_PHY_REG_OFFSET(dprx_id), ((ulDPCDAddress)>>0x10)&0xF);

	msWriteByte(REG_DPRX_AUX_77_H+ DPRX_PHY_REG_OFFSET(dprx_id), ucDATA);
		
	msWriteByteMask(REG_DPRX_AUX_76_H+ DPRX_PHY_REG_OFFSET(dprx_id), BIT7, BIT7);    //trigger MUC write
	
	do {
			DELAY_NOP(0x4);	
			ubTimeOutLimt --;
	} while((msReadByte(REG_DPRX_AUX_76_H+ DPRX_PHY_REG_OFFSET(dprx_id)) & BIT5)  && (ubTimeOutLimt > 0));

	msWriteByteMask(REG_DPRX_AUX_76_H+ DPRX_PHY_REG_OFFSET(dprx_id), 0, BIT4);	    //Disable
	
	
}

//**************************************************************************
//  [Function Name]:
//                  DPMST_ISR_McuIndexWriteDPCD()
//  [Description]
//                  DPMST_ISR_McuIndexWriteDPCD
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE  mhal_DPRx_MCUReadNonPMDPCD(DPRx_ID dprx_id, DWORD ulDPCDAddress)
{

	
	BYTE XDATA ubTimeOutLimt = 0x5;
	BYTE XDATA Data=0;
		
	//Address overwrite enable
	msWriteByteMask(REG_DPRX_AUX_76_H+ DPRX_PHY_REG_OFFSET(dprx_id), BIT4, BIT4);   //Enable
	//20 bit DPCD address
	//msWrite2Byte(REG_003010, ((ulDPCDAddress + i)&0xFFFF));
	msWriteByte(REG_DPRX_AUX_75_L+ DPRX_PHY_REG_OFFSET(dprx_id), ((ulDPCDAddress)&0xFF));
	msWriteByte(REG_DPRX_AUX_75_H+ DPRX_PHY_REG_OFFSET(dprx_id), (((ulDPCDAddress)>> 0x8)&0xFF));
	msWriteByte(REG_DPRX_AUX_75_L+ DPRX_PHY_REG_OFFSET(dprx_id), ((ulDPCDAddress)>>0x10)&0xF);

	msWriteByteMask(REG_DPRX_AUX_76_H+ DPRX_PHY_REG_OFFSET(dprx_id), BIT6, BIT6);    //trigger MUC Read
	
	do {
			DELAY_NOP(0x4);	
			ubTimeOutLimt --;
	} while((msReadByte(REG_DPRX_AUX_76_H+ DPRX_PHY_REG_OFFSET(dprx_id)) & BIT5)  && (ubTimeOutLimt > 0));

	Data = msReadByte(REG_DPRX_AUX_77_L+ DPRX_PHY_REG_OFFSET(dprx_id));
		
	msWriteByteMask(REG_DPRX_AUX_76_H+ DPRX_PHY_REG_OFFSET(dprx_id), 0, BIT4);	    //Disable
	
	return Data;
	
}



//**************************************************************************
//  [Function Name]:
//                  mhal_DPRx_GetOuputMode()
//  [Description]
//                  mhal_DPRx_GetOuputMode
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
DPRx_MODE  mhal_DPRx_GetOuputMode(DPRx_ID dprx_id)
{
	return ((msReadByte(REG_DPRX_DECODER_E0_59_L+ DPRX_PHY_REG_OFFSET(dprx_id))&(BIT6|BIT5)) >>5);
}

#endif // _MHAL_DPRX_C_


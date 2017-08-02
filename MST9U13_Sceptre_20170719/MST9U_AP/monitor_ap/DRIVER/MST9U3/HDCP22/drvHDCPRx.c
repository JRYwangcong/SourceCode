///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    drvHDCPRx.c
/// @author MStar Semiconductor Inc.
/// @brief  HDCP22 driver Function
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef DRV_HDCPRX_C
#define DRV_HDCPRX_C

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include <string.h>
#include "datatype.h"
#include "Global.h"
#include "Common.h"
#include "drvHDCPRx.h"
#include "drvHDCPMbx.h"
#include "mhal_combo.h"

#if (ENABLE_SECU_R2 == 1) && (ENABLE_HDCP22 == 1)
//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define HDCP_DRV_DEBUG_MESSAGE          0
#define HDCP_DRV_PARSER_MESSAGE         0

#if(ENABLE_MSTV_UART_DEBUG & HDCP_DRV_DEBUG_MESSAGE)
#define HDCP_DRV_DPUTSTR(str)           printMsg(str)
#define HDCP_DRV_DPRINTF(str, x)        printData(str, x)
#else
#define HDCP_DRV_DPUTSTR(str)
#define HDCP_DRV_DPRINTF(str, x)
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
//length = 1 byte(msgID) + argument length
const WORD gMsgArgLen[AUTH_CMD_NUM] = 
{
    0,      //na
    0,      //NULL_MESSAGE = 1,
    12,     //AKE_INIT = 2,
    534,    //AKE_SEND_CERT = 3,
    129,    //AKE_NO_STORED_KM = 4,
    33,     //AKE_STORED_KM = 5,
    0,      //AKE_SEND_RRX = 6,
    33,     //AKE_SEND_H_PRIME = 7,
    17,     //AKE_SEND_PAIRING_INFO = 8,
    9,      //LC_INIT = 9,
    33,     //LC_SEND_L_PRIME =10,
    25,     //SKE_SEND_EKS = 11,
    0,      //5 to (5+DeviceCount*5); REPEATERAUTH_SEND_RECEIVERID_LIST = 12,
    0,      //RTT_READY = 13,
    0,     //RTT_CHALLENGE = 14,
    0,     //REPEATERAUTH_SEND_ACK = 15,
    0,      //REPEATERAUTH_STREAM_MANAGE = 16,
    0,     //REPEATERAUTH_STREAM_READY = 17,
    0,      //RECEIVER_AUTHSTATUS = 18,
    0,      //AKE_TRANSMITTER_INFO = 19,
    0,      //AKE_RECEIVER_INFO = 20,
};

static stHDCPContent volatile HdcpHandler[DEV_NUM];

BYTE volatile gHdcpMbx_TxBuf[DEV_NUM][32 + 16]; //*2 for Sec world and HK world //__attribute__((aligned(16))); //H'(32) + Ekh(km) (16)
BYTE volatile gHdcpMbx_RxBuf[DEV_NUM][130]; //__attribute__((aligned(16))); //idx 1+1+128 = portIdx + msgID + msgArg

BYTE ucSecR2EventFlag = 0;

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
void __HDCP_MsgPool_Relative_Func__(void){}

//**************************************************************************
//  [Function Name]:
//                  _drv_HDCPRx_MsgPoolGetMax()
//  [Description]:
//                  
//  [Arguments]:
//                  
//  [Return]:
//                  
//
//**************************************************************************
BYTE _drv_HDCPRx_MsgPoolGetMax(void)
{
    BYTE uctemp = 0;
    BYTE MaxPriority = 0;

    for(uctemp = 0; uctemp < DEV_NUM; uctemp++ )
    {
        if(gHdcpMbx_RxBuf[uctemp][0] > MaxPriority)
        {
            MaxPriority = gHdcpMbx_RxBuf[uctemp][0];
        }
    }

    return MaxPriority;
}

//**************************************************************************
//  [Function Name]:
//                  _drv_HDCPRx_MsgPoolGetMin()
//  [Description]:
//                  
//  [Arguments]:
//                  
//  [Return]:
//                  
//
//**************************************************************************
BYTE _drv_HDCPRx_MsgPoolGetMin(void)
{
    BYTE uctemp = 0;
    BYTE MinPriority = DEV_NUM;

    for(uctemp = 0; uctemp < DEV_NUM; uctemp++)
    {
        if((gHdcpMbx_RxBuf[uctemp][0] < MinPriority) && (gHdcpMbx_RxBuf[uctemp][0] != 0))
        {
            MinPriority = gHdcpMbx_RxBuf[uctemp][0];
        }
    }

    return MinPriority;
}

//**************************************************************************
//  [Function Name]:
//                  _drv_HDCPRx_MsgPoolFetchMsgIdx()
//  [Description]:
//                  
//  [Arguments]:
//                  
//  [Return]:
//                  
//
//**************************************************************************
BYTE _drv_HDCPRx_MsgPoolFetchMsgIdx(void)
{
    BYTE uctemp = 0;
    BYTE MaxPriority = 0;
    BYTE MsgIdx = 0;

    for(uctemp = 0; uctemp < DEV_NUM; uctemp++)
    {
        if(gHdcpMbx_RxBuf[uctemp][0] > MaxPriority)
        {
            MaxPriority = gHdcpMbx_RxBuf[uctemp][0];
            MsgIdx = uctemp;
        }
    }

    if (MaxPriority != 0)
    {
        return MsgIdx;
    }
    else
    {
        return (DEV_NUM + 1); //no msg received
    }
}

//**************************************************************************
//  [Function Name]:
//                  _drv_HDCPRx_MsgPoolFillPriority()
//  [Description]:
//                  
//  [Arguments]:
//                  
//  [Return]:
//                  
//
//**************************************************************************
void _drv_HDCPRx_MsgPoolFillPriority(BYTE ucPortIdx)
{
    BYTE uctemp = 0;
    BYTE ucCurMax = 0;
    BYTE ucCurMin = 0;

    ucCurMax = _drv_HDCPRx_MsgPoolGetMax();

    if(ucCurMax == 0)
    {
        gHdcpMbx_RxBuf[ucPortIdx][0] = 1; //first msg
    }
    else
    {
        ucCurMin = _drv_HDCPRx_MsgPoolGetMin();

        if(ucCurMin != (DEV_NUM + 1)) 
        {
            for(uctemp = 0; uctemp < DEV_NUM; uctemp++) // Increase priority 1 in all active process
            {
                if((gHdcpMbx_RxBuf[uctemp][0] != 0) && (gHdcpMbx_RxBuf[uctemp][0] <= DEV_NUM))
                {
                    gHdcpMbx_RxBuf[uctemp][0]++;
                }
            }
            
            gHdcpMbx_RxBuf[ucPortIdx][0] = ucCurMin;
        }
        else
        {
            gHdcpMbx_RxBuf[ucPortIdx][0] = 1; //should not entering this condition
        }
    }
}

//pesudo function
void __HDCP_Relative_Func__(void){}

//**************************************************************************
//  [Function Name]:
//                  _drv_HDCPRx_FlushCache()
//  [Description]:
//                  
//  [Arguments]:
//                  
//  [Return]:
//                  
//
//**************************************************************************
void _drv_HDCPRx_FlushCache(ULONG dwBufAddr, ULONG dwsize)
{
	ULONG ultemp = 0;

	for(ultemp = 0; ultemp < dwsize; ultemp++)
	{
		*(BYTE*)((dwBufAddr | 0x80000000) + ultemp) = *(BYTE*)(dwBufAddr + ultemp);
	}
}

//**************************************************************************
//  [Function Name]:
//                  _drv_HDCPRx_UpdateFromDRAM()
//  [Description]:
//                  
//  [Arguments]:
//                  
//  [Return]:
//                  
//
//**************************************************************************
void _drv_HDCPRx_UpdateFromDRAM(ULONG dwBufAddr, ULONG dwsize)
{
	ULONG ultemp = 0;

	for(ultemp = 0; ultemp < dwsize; ultemp++)
	{
		//*(BYTE*)(dwBufAddr + i) = (BYTE*)((dwBufAddr | 0x80000000) + i);
		*(BYTE*)(dwBufAddr + ultemp) = msMemReadByte(dwBufAddr + ultemp);
	}
}

//**************************************************************************
//  [Function Name]:
//                  _drv_HDCPRx_MsgPoolAssignPriority()
//  [Description]:
//                  
//  [Arguments]:
//                  
//  [Return]:
//                  
//
//**************************************************************************
void _drv_HDCPRx_MsgPoolAssignPriority(BYTE ucPortIdx, BYTE ucPriority)
{
    gHdcpMbx_RxBuf[ucPortIdx][0] = ucPriority;
    
	_drv_HDCPRx_FlushCache((ULONG)&(gHdcpMbx_RxBuf[ucPortIdx][0]), 1);
}


//**************************************************************************
//  [Function Name]:
//                  _drv_HDCPRx_CheckWaitingProcess()
//  [Description]:
//                  check if there is any port waiting security module process
//  [Arguments]:
//                  void
//  [Return]:
//                  [enHDCP_PORT_IDX]
//
//**************************************************************************
BYTE _drv_HDCPRx_CheckWaitingProcess(void)
{
    BYTE uctemp = 0;
    
    for(uctemp = HDCP_PORT_0; uctemp < HDCP_PORT_MAX; uctemp++)
    {
        if(HdcpHandler[uctemp].Polling == TRUE)
        {
            return uctemp;
        }
    }
    
    return HDCP_PORT_ALL;
}

//**************************************************************************
//  [Function Name]:
//                  _drv_HDCPRx_ClrMbxBuf()
//  [Description]:
//                  
//  [Arguments]:
//                  
//  [Return]:
//                  void
//
//**************************************************************************
void _drv_HDCPRx_ClrMbxRxBuf(BYTE ucPortIdx)
{
    BYTE uctemp = 0;
    
    for(uctemp = 0; uctemp < (gMsgArgLen[AKE_No_Stored_km] + 1); uctemp++)
    {
        gHdcpMbx_RxBuf[ucPortIdx][uctemp] = 0;
    }
}

//**************************************************************************
//  [Function Name]:
//                  _drv_HDCPRx_ParserMessage()
//  [Description]:
//                  
//  [Arguments]:
//                  
//  [Return]:
//                  void
//
//**************************************************************************
void _drv_HDCPRx_ParserMessage(BYTE ucPortIndex, WORD usLength)
{
#if(HDCP_DRV_PARSER_MESSAGE)
    WORD ustemp = 0;

    for(ustemp = 0; ustemp < usLength; ustemp++)
    {
        HDCP_DRV_DPRINTF("0x%02X, ", gHdcpMbx_RxBuf[ucPortIndex][MSGID_IDX + ustemp]);
        
        if((ustemp +1) %16 == 0)
        {
            HDCP_DRV_DPUTSTR("\r\n");
        }
    }
    
    HDCP_DRV_DPUTSTR("\r\n");
                
#endif

    ucPortIndex = usLength;
}

//**************************************************************************
//  [Function Name]:
//                  _drv_HDCPRx_AKEInit()
//  [Description]:
//                  Pass rtx, TxCaps, rrx, and RxCaps to Security R2
//  [Arguments]:
//                  [enHDCP_PORT_IDX] enPortIdx, 
//                  [stHDCPContent]* pstHandler
//  [Return]:
//                  [enHDCP_Err_Status]
//
//**************************************************************************
enHDCP_Err_Status _drv_HDCPRx_AKEInit(BYTE enPortIdx, stHDCPContent *pstHandler)
{
    enHDCPMBX_ERROR_CODE MbxErrCode = MBX_ERR_NONE;

    MbxErrCode = drv_HDCPMBX_SetMsg(enPortIdx, MSG_HDCP22_AKE_INIT, MBX_HKR2, MBXMSG_WITH_ARG, MBXMSG_STATE_WAIT, \
                                    MBX_ERR_NONE, (ULONG)&gHdcpMbx_RxBuf[enPortIdx][MSGARG_IDX], (RTX_SIZE + TXCAPS_SIZE));
	
    if(MbxErrCode == MBX_ERR_NONE)
    {
    	//polling random number
    	pstHandler->Polling = TRUE;

		//update priority
		_drv_HDCPRx_MsgPoolAssignPriority(enPortIdx, (gHdcpMbx_RxBuf[enPortIdx][0] + DEV_NUM));
		
        return HDCP_ERR_NONE;
    }
    else
    {
        return HDCP_PROCESS_FAIL;
    }
}

//**************************************************************************
//  [Function Name]:
//                  _drv_HDCPRx_AKESendCert()
//  [Description]:
//                  
//  [Arguments]:
//                  [enHDCP_PORT_IDX] enPortIdx, 
//                  [stHDCPContent]* pstHandler
//  [Return]:
//                  [enHDCP_Err_Status]
//
//**************************************************************************
enHDCP_Err_Status _drv_HDCPRx_AKESendCert(BYTE enPortIdx, stHDCPContent* pstHandler)
{
    pstHandler->SubState = AKE_Send_Cert;

    //command IP to send message; update from DRAM    
    _drv_HDCPRx_UpdateFromDRAM((ULONG)(&gHdcpMbx_TxBuf[enPortIdx][0]), (RRX_SIZE + RXCAPS_SIZE));

	pstHandler->SendFunc(enPortIdx, AKE_Send_Cert, (RRX_SIZE + RXCAPS_SIZE), (BYTE*)&gHdcpMbx_TxBuf[enPortIdx][0]);

    return HDCP_ERR_NONE;
}

//**************************************************************************
//  [Function Name]:
//                  _drv_HDCPRx_AKENoStoredKm()
//  [Description]:
//                  
//  [Arguments]:
//                  [BYTE] ucIdx: port index
//  [Return]:
//                  [bool]: TRUE if the message is done, and FALSE for halt
//
//**************************************************************************
enHDCP_Err_Status _drv_HDCPRx_AKENoStoredKm(BYTE enPortIdx, stHDCPContent* pstHandler)
{
    enHDCPMBX_ERROR_CODE MbxErrCode = MBX_ERR_NONE;

    MbxErrCode = drv_HDCPMBX_SetMsg(enPortIdx, MSG_HDCP22_AKE_NO_STORED_KM, MBX_HKR2, MBXMSG_WITH_ARG, MBXMSG_STATE_WAIT,\
                                    MBX_ERR_NONE, (ULONG)&gHdcpMbx_RxBuf[enPortIdx][MSGARG_IDX], EKPUBKM_SIZE);

    if (MbxErrCode == MBX_ERR_NONE)
    {
        pstHandler->Polling = TRUE;

        //update priority
		_drv_HDCPRx_MsgPoolAssignPriority(enPortIdx, (gHdcpMbx_RxBuf[enPortIdx][0] + DEV_NUM));
        
        return HDCP_ERR_NONE;
    }
    else
    {
        return HDCP_PROCESS_FAIL;
    }
}

//**************************************************************************
//  [Function Name]:
//                  _drv_HDCPRx_AKEStoredKm()
//  [Description]:
//                  
//  [Arguments]:
//                  [BYTE] ucIdx: port index
//  [Return]:
//                  [bool]: TRUE if the message is done, and FALSE for halt
//
//**************************************************************************
enHDCP_Err_Status _drv_HDCPRx_AKEStoredKm(BYTE enPortIdx, stHDCPContent* pstHandler)
{
    enHDCPMBX_ERROR_CODE MbxErrCode = MBX_ERR_NONE;

	MbxErrCode = drv_HDCPMBX_SetMsg(enPortIdx, MSG_HDCP22_AKE_STORED_KM, MBX_HKR2, MBXMSG_WITH_ARG, MBXMSG_STATE_WAIT,\
                                    MBX_ERR_NONE, (ULONG)&gHdcpMbx_RxBuf[enPortIdx][MSGARG_IDX], (EKHKM_SIZE + M_SIZE));

    if (MbxErrCode == MBX_ERR_NONE)
    {
        pstHandler->Polling = TRUE;

        //update priority
		_drv_HDCPRx_MsgPoolAssignPriority(enPortIdx, (gHdcpMbx_RxBuf[enPortIdx][0] + DEV_NUM));
        
        return HDCP_ERR_NONE;
    }
    else
    {
        return HDCP_PROCESS_FAIL;
    }
}

//**************************************************************************
//  [Function Name]:
//                  _drv_HDCPRx_AKESendHPrime()
//  [Description]:
//                  
//  [Arguments]:
//                  [BYTE] ucIdx: port index
//  [Return]:
//                  [bool]: TRUE if the message is done, and FALSE for halt
//
//**************************************************************************
enHDCP_Err_Status _drv_HDCPRx_AKESendHPrime(BYTE enPortIdx, stHDCPContent* pstHandler)
{
    //[NOTE] The Security R2 Will Calculate H' and Pairing Info at Same Time,
    //So the Return Data Will be the Combination of 2 Variables ( H' + Ekh(km))
    
	_drv_HDCPRx_UpdateFromDRAM((ULONG)(&gHdcpMbx_TxBuf[enPortIdx][0]), H_SIZE + EKHKM_SIZE);

	pstHandler->SendFunc(enPortIdx, AKE_Send_H_Prime, H_SIZE, (BYTE*)&gHdcpMbx_TxBuf[enPortIdx][0]);

    return HDCP_ERR_NONE;

}

//**************************************************************************
//  [Function Name]:
//                  _drv_HDCPRx_AKESendPairingInfo()
//  [Description]:
//                  
//  [Arguments]:
//                  [BYTE] ucIdx: port index
//  [Return]:
//                  [bool]: TRUE if the message is done, and FALSE for halt
//
//**************************************************************************
enHDCP_Err_Status _drv_HDCPRx_AKESendPairingInfo(BYTE enPortIdx, stHDCPContent* pstHandler)
{
    pstHandler->SubState = AKE_Send_Pairing_Info;

    //[NOTE] The Security R2 Will Calculate H' and Pairing Info at Same Time,
    //So the Return Data Will be the Combination of 2 Variables ( H' + Ekh(km))
	
	pstHandler->SendFunc(enPortIdx, AKE_Send_Pairing_Info, EKHKM_SIZE, (BYTE*)&gHdcpMbx_TxBuf[enPortIdx][H_SIZE]);
	//pstHandler->SendFunc(enPortIdx, AKE_Send_Pairing_Info, EKHKM_SIZE, (BYTE*)&gHdcpMbx_TxBuf[enPortIdx][0]);

    return HDCP_ERR_NONE;
}

//**************************************************************************
//  [Function Name]:
//                  _drv_HDCPRx_LCInit()
//  [Description]:
//                  
//  [Arguments]:
//                  [BYTE] ucIdx: port index
//  [Return]:
//                  [bool]: TRUE if the message is done, and FALSE for halt
//
//**************************************************************************
enHDCP_Err_Status _drv_HDCPRx_LCInit(BYTE enPortIdx, stHDCPContent* pstHandler)
{
    enHDCPMBX_ERROR_CODE MbxErrCode = MBX_ERR_NONE;

	MbxErrCode = drv_HDCPMBX_SetMsg(enPortIdx, MSG_HDCP22_LC_INIT, MBX_HKR2, MBXMSG_WITH_ARG, MBXMSG_STATE_WAIT,\
                                    MBX_ERR_NONE, (ULONG)&gHdcpMbx_RxBuf[enPortIdx][MSGARG_IDX], RN_SIZE);

    if (MbxErrCode == MBX_ERR_NONE)
    {
        pstHandler->Polling = TRUE;

        //update priority
		_drv_HDCPRx_MsgPoolAssignPriority(enPortIdx, (gHdcpMbx_RxBuf[enPortIdx][0] + DEV_NUM));
        
        return HDCP_ERR_NONE;   
    }
    else
    {
        return HDCP_PROCESS_FAIL;
    }
}

//**************************************************************************
//  [Function Name]:
//                  _drv_HDCPRx_LCSendLPrime()
//  [Description]:
//                  
//  [Arguments]:
//                  [BYTE] ucIdx: port index
//  [Return]:
//                  [bool]: TRUE if the message is done, and FALSE for halt
//
//**************************************************************************
enHDCP_Err_Status _drv_HDCPRx_LCSendLPrime(BYTE enPortIdx, stHDCPContent* pstHandler)
{
    //update substate
    pstHandler->SubState = LC_Send_L_Prime;

	_drv_HDCPRx_UpdateFromDRAM((ULONG)(&gHdcpMbx_TxBuf[enPortIdx][0]), LPRIME_SIZE);
	
	pstHandler->SendFunc(enPortIdx, LC_Send_L_Prime, LPRIME_SIZE, (BYTE*)&gHdcpMbx_TxBuf[enPortIdx][0]);

    return HDCP_ERR_NONE;
}

//**************************************************************************
//  [Function Name]:
//                  _drv_HDCPRx_SKESendEks()
//  [Description]:
//                  
//  [Arguments]:
//                  [BYTE] ucIdx: port index
//  [Return]:
//                  [bool]: TRUE if the message is done, and FALSE for halt
//
//**************************************************************************
enHDCP_Err_Status _drv_HDCPRx_SKESendEks(BYTE enPortIdx, stHDCPContent* pstHandler)
{
    enHDCPMBX_ERROR_CODE MbxErrCode = MBX_ERR_NONE;

	MbxErrCode = drv_HDCPMBX_SetMsg(enPortIdx, MSG_HDCP22_SKE_SEND_EKS, MBX_HKR2, MBXMSG_WITH_ARG, MBXMSG_STATE_WAIT,\
                                MBX_ERR_NONE, (ULONG)&gHdcpMbx_RxBuf[enPortIdx][MSGARG_IDX], (EDKEYKS_SIZE + RIV_SIZE));

    //TBD: start cipher
    if(MbxErrCode == MBX_ERR_NONE)
    {
        pstHandler->Polling = TRUE; //FALSE;

        //update priority
        _drv_HDCPRx_MsgPoolAssignPriority(enPortIdx, (gHdcpMbx_RxBuf[enPortIdx][0] + DEV_NUM));
        //_drv_HDCPRx_MsgPoolAssignPriority(enPortIdx, 0x00);
        
        return HDCP_ERR_NONE;   
    }
    else
    {
        return HDCP_PROCESS_FAIL;
    }
}

//**************************************************************************
//  [Function Name]:
//                  _drv_HDCPRx_FSM()
//  [Description]:
//                  
//  [Arguments]:
//                  [BYTE] ucIdx: port index
//  [Return]:
//                  [bool]: TRUE if the message is done, and FALSE for halt
//
//**************************************************************************
//extern BYTE Sec2HKFlag;
enHDCP_Err_Status _drv_HDCPRx_FSM(BYTE enPortIdx)
{
    enHDCP_Err_Status enErrCode = HDCP_ERR_NONE;
    enHDCPMBX_MSG_STATE enMessageState = MBXMSG_STATE_IDLE;

    switch(HdcpHandler[enPortIdx].MainState)
    {
        case RxState_B0:
            switch(HdcpHandler[enPortIdx].SubState)
            {
                case StateZero:
                    //waiting msg
                    break;

                case AKE_Init:
                    enMessageState = drv_HDCPMBX_CheckMsgState(enPortIdx);
                    
					//check if security r2 process is halt or done;
					if((enMessageState != MBXMSG_STATE_IDLE) && (enMessageState != MBXMSG_STATE_DONE) && (enMessageState != MBXMSG_STATE_HALT))
					{
						HDCP_DRV_DPUTSTR("ake init return\r\n");
                        
						return HDCP_ERR_NONE;
					}
					
                    _drv_HDCPRx_AKEInit(enPortIdx, (stHDCPContent*)&HdcpHandler[enPortIdx]);

                    //update state
                    HdcpHandler[enPortIdx].MainState = RxState_B1;
                    HdcpHandler[enPortIdx].SubState = StateZero;

                    //goto RecvAKEInit;
                    break;

                default:
                    enErrCode = HDCP_UNKNOWN_STATE;
                    break;
            };
            
            break;

        case RxState_B1:
            switch(HdcpHandler[enPortIdx].SubState)
            {
                case StateZero:
                    //AKE_Send_Cert
                    if(drv_HDCPRx_GetR2EventFlag())
                    {
	                    gHdcpMbx_RxBuf[enPortIdx][0] = 0; //msg done //update priority
	                    drv_HDCPMBX_ClearActiveBit(enPortIdx); //clear active bit

						HdcpHandler[enPortIdx].Polling = FALSE; //clear polling status
						
						_drv_HDCPRx_AKESendCert(enPortIdx, (stHDCPContent*)&HdcpHandler[enPortIdx]);
					}
                    
                    break;

                case AKE_No_Stored_km:
					_drv_HDCPRx_AKENoStoredKm(enPortIdx, (stHDCPContent*)&HdcpHandler[enPortIdx]);
                    HdcpHandler[enPortIdx].bStoredkm = FALSE;
					HdcpHandler[enPortIdx].SubState = AKE_Send_H_Prime;
                    break;

                case AKE_Stored_km:
					_drv_HDCPRx_AKEStoredKm(enPortIdx, (stHDCPContent*)&HdcpHandler[enPortIdx]);
                    HdcpHandler[enPortIdx].bStoredkm = TRUE;
					HdcpHandler[enPortIdx].SubState = AKE_Send_H_Prime;
                    break;

                case AKE_Send_H_Prime:
                    if(drv_HDCPRx_GetR2EventFlag()) // got ISR from SEC R2, means calculation is done
                    {
						//clear polling status
						HdcpHandler[enPortIdx].Polling = FALSE;

						if(HdcpHandler[enPortIdx].bStoredkm == FALSE)
						{
                            HdcpHandler[enPortIdx].SubState = AKE_Send_Pairing_Info;
							//drv_HDCPRx_AKEComputePairingInfo(enPortIdx, (stHDCPContent*)&HdcpHandler[enPortIdx]);
						}
						else
                        {
                            gHdcpMbx_RxBuf[enPortIdx][0] = 0; //clear priority if stored km, cuz AKE is finished
                            drv_HDCPMBX_ClearActiveBit(enPortIdx); //clear active bit
                        }
						
						_drv_HDCPRx_AKESendHPrime(enPortIdx, (stHDCPContent*)&HdcpHandler[enPortIdx]);
                    }
                    
                    break;

                case AKE_Send_Pairing_Info:
                    //make sure TX read data done
                    if(mhal_combo_HDCP2GetReadDownFlag(enPortIdx))
                    {
						gHdcpMbx_RxBuf[enPortIdx][0] = 0; //clear priority, cuz AKE is finished
                        drv_HDCPMBX_ClearActiveBit(enPortIdx); //clear active bit
                        
                        _drv_HDCPRx_AKESendPairingInfo(enPortIdx, (stHDCPContent*)&HdcpHandler[enPortIdx]);

						//clear polling status
						//HdcpHandler[enPortIdx].Polling = FALSE;
                    }
                    
                    break;

                case LC_Init:
					_drv_HDCPRx_LCInit(enPortIdx, (stHDCPContent*)&HdcpHandler[enPortIdx]);
					HdcpHandler[enPortIdx].MainState = RxState_B2;
                    HdcpHandler[enPortIdx].SubState = StateZero;
                    break;
                
                default:
                    enErrCode = HDCP_UNKNOWN_STATE;
                    break;
            };
            
            break;
        
        case RxState_B2:
            switch(HdcpHandler[enPortIdx].SubState)
            {
                case StateZero:
                    if(drv_HDCPRx_GetR2EventFlag())
                    {
                        gHdcpMbx_RxBuf[enPortIdx][0] = 0; //clear priority, cuz LC is finished
                        drv_HDCPMBX_ClearActiveBit(enPortIdx); //clear active bit

						HdcpHandler[enPortIdx].Polling = FALSE;  //clear polling status

						_drv_HDCPRx_LCSendLPrime(enPortIdx, (stHDCPContent*)&HdcpHandler[enPortIdx]);
                    }
                    
                    break;

                case LC_Init:
                    _drv_HDCPRx_LCInit(enPortIdx, (stHDCPContent*)&HdcpHandler[enPortIdx]);
                    
                    HdcpHandler[enPortIdx].SubState = StateZero; //retry
                    break;
                
                case SKE_Send_Eks:
                    _drv_HDCPRx_SKESendEks(enPortIdx, (stHDCPContent*)&HdcpHandler[enPortIdx]);
                    
                    HdcpHandler[enPortIdx].MainState = RxState_B3;
                    HdcpHandler[enPortIdx].SubState = StateZero;
                    //goto RecvSKESendEks;
                    break;
                
                default:
                    enErrCode = HDCP_UNKNOWN_STATE;
                    break;
            };
            
            break;

        case RxState_B3:
            //compute ks
            if(drv_HDCPRx_GetR2EventFlag())
            {
				gHdcpMbx_RxBuf[enPortIdx][0] = 0; //clear priority
				drv_HDCPMBX_ClearActiveBit(enPortIdx); //clear active bit

				//clear polling status
				HdcpHandler[enPortIdx].Polling = FALSE;
				
	            HdcpHandler[enPortIdx].MainState = RxState_B4;
            }
            
            break;
        
        case RxState_B4:
            //authenticated
            HdcpHandler[enPortIdx].AuthDone = TRUE;
			drv_HDCPMBX_UpdateState(enPortIdx, MBXMSG_STATE_IDLE);
            
            break;

        default:
            enErrCode = HDCP_UNKNOWN_STATE;
            break;
    }; //end of state switch case

    return enErrCode;
}

//**************************************************************************
//  [Function Name]:
//                  _drv_HDCPRx_AKEComputePairingInfo()
//  [Description]:
//                  
//  [Arguments]:
//                  [BYTE] ucIdx: port index
//  [Return]:
//                  
//
//**************************************************************************
enHDCP_Err_Status _drv_HDCPRx_AKEComputePairingInfo(BYTE enPortIdx, stHDCPContent* pstHandler)
{
	enHDCPMBX_ERROR_CODE MbxErrCode = MBX_ERR_NONE;

	MbxErrCode = drv_HDCPMBX_SetMsg(enPortIdx, MSG_HDCP22_AKE_SEND_PARING_INFO, MBX_HKR2, MBXMSG_WITHOUT_ARG, MBXMSG_STATE_WAIT,\
									MBX_ERR_NONE, 0x00, 0x00);

	if (MbxErrCode == MBX_ERR_NONE)
	{
		pstHandler->Polling = TRUE;

		//update priority
		_drv_HDCPRx_MsgPoolAssignPriority(enPortIdx, (gHdcpMbx_RxBuf[enPortIdx][0] + DEV_NUM));
		
		return HDCP_ERR_NONE;
	}
	else
    {
        return HDCP_PROCESS_FAIL;
    }
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

//**************************************************************************
//  [Function Name]:
//                  drv_HDCPRx_GetTxBufAddr()
//  [Description]:
//                  
//  [Arguments]:
//                  
//  [Return]:
//                  
//
//**************************************************************************
ULONG drv_HDCPRx_GetTxBufAddr(void)
{
    return (ULONG)(&gHdcpMbx_TxBuf[0][0]);
}

//**************************************************************************
//  [Function Name]:
//                  drv_HDCPRx_SetR2EventFlag()
//  [Description]:
//                  
//  [Arguments]:
//                  
//  [Return]:
//
//**************************************************************************
void drv_HDCPRx_SetR2EventFlag(void)
{
    ucSecR2EventFlag = 1;
}

//**************************************************************************
//  [Function Name]:
//                  drv_HDCPRx_GetR2EventFlag()
//  [Description]:
//                  
//  [Arguments]:
//                  
//  [Return]:
//
//**************************************************************************
Bool drv_HDCPRx_GetR2EventFlag(void)
{
    Bool bR2EventFlag = FALSE;

    if(ucSecR2EventFlag > 0)
    {
        bR2EventFlag = TRUE;
        ucSecR2EventFlag = 0;
    }

    return bR2EventFlag;
}

//**************************************************************************
//  [Function Name]:
//                  drv_HDCPRx_InitCBFunction()
//  [Description]:
//                  initialize variables
//  [Arguments]:
//                  [BYTE] ucPortIdx
//                  [HDCP_SEND_FUNC] pCBFunc: function pointer
//  [Return]:
//                  void
//
//**************************************************************************
void drv_HDCPRx_InitCBFunction(BYTE ucPortIdx, HDCP_SEND_FUNC pCBFunc)
{
    HdcpHandler[ucPortIdx].SendFunc = pCBFunc;
}

//**************************************************************************
//  [Function Name]:
//                  drv_HDCPRx_Init()
//  [Description]:
//                  initialize variables
//  [Arguments]:
//                  [BYTE] ucPortIdx
//  [Return]:
//                  void
//
//**************************************************************************
void drv_HDCPRx_Init(BYTE enPortIdx)
{
    BYTE uctemp = 0;

    if(enPortIdx == HDCP_PORT_ALL)
    {
        for(uctemp = 0; uctemp < DEV_NUM; uctemp++)
        {
            memset((void*)&HdcpHandler[uctemp], 0x00, sizeof(stHDCPContent));
            
            HdcpHandler[uctemp].bStoredkm = FALSE;
            HdcpHandler[uctemp].MainState = RxState_B0;
            HdcpHandler[uctemp].SubState = StateZero;
            HdcpHandler[uctemp].AuthDone = FALSE;

#if(SUPPORT_REPEATER == 1)
            HdcpHandler[uctemp].rxCaps[0] = 0x02;
            HdcpHandler[uctemp].rxCaps[1] = 0x00;
            HdcpHandler[uctemp].rxCaps[2] = 0x01;
            
#else
            HdcpHandler[uctemp].rxCaps[0] = 0x02;
            HdcpHandler[uctemp].rxCaps[1] = 0x00;
            HdcpHandler[uctemp].rxCaps[2] = 0x00;
            
#endif

            HdcpHandler[uctemp].rrx[7] = 0xe1;
            HdcpHandler[uctemp].rrx[6] = 0x7a;
            HdcpHandler[uctemp].rrx[5] = 0xb0;
            HdcpHandler[uctemp].rrx[4] = 0xfd;
            HdcpHandler[uctemp].rrx[3] = 0x0f;
            HdcpHandler[uctemp].rrx[2] = 0x54;
            HdcpHandler[uctemp].rrx[1] = 0x40;
            HdcpHandler[uctemp].rrx[0] = 0x52;
        }
    }
    else
    {
        memset((void*)&HdcpHandler[enPortIdx], 0x00, sizeof(stHDCPContent));
        
        HdcpHandler[enPortIdx].bStoredkm = FALSE;
        HdcpHandler[enPortIdx].MainState = RxState_B0;
        HdcpHandler[enPortIdx].SubState = StateZero;
        HdcpHandler[enPortIdx].AuthDone = FALSE;

#if(SUPPORT_REPEATER == 1)
        HdcpHandler[enPortIdx].rxCaps[0] = 0x02;
        HdcpHandler[enPortIdx].rxCaps[1] = 0x00;
        HdcpHandler[enPortIdx].rxCaps[2] = 0x01;
        
#else
        HdcpHandler[enPortIdx].rxCaps[0] = 0x02;
        HdcpHandler[enPortIdx].rxCaps[1] = 0x00;
        HdcpHandler[enPortIdx].rxCaps[2] = 0x00;
        
#endif

        HdcpHandler[enPortIdx].rrx[7] = 0xe1;
        HdcpHandler[enPortIdx].rrx[6] = 0x7a;
        HdcpHandler[enPortIdx].rrx[5] = 0xb0;
        HdcpHandler[enPortIdx].rrx[4] = 0xfd;
        HdcpHandler[enPortIdx].rrx[3] = 0x0f;
        HdcpHandler[enPortIdx].rrx[2] = 0x54;
        HdcpHandler[enPortIdx].rrx[1] = 0x40;
        HdcpHandler[enPortIdx].rrx[0] = 0x52;
    }
}

//**************************************************************************
//  [Function Name]:
//                  drv_HDCPRx_DecodeKey()
//  [Description]:
//                  
//  [Arguments]:
//                  
//  [Return]:
//                  
//**************************************************************************
void drv_HDCPRx_DecodeKey(BYTE *pCertRxTable)
{
    enHDCPMBX_ERROR_CODE MbxErrCode = MBX_ERR_NONE;

    MbxErrCode = drv_HDCPMBX_SetKeyMessage(MSG_HDCP22_DECODE_KEY, MBX_HKR2, MBXMSG_WITH_ARG, MBXMSG_STATE_WAIT, \
                                    MBX_ERR_NONE, (ULONG)pCertRxTable, (1 + ENCODE_KEY_SIZE));
}

//**************************************************************************
//  [Function Name]:
//                  drv_HDCPRx_MsgParser()
//  [Description]:
//                  Fetch Message Arguments into HDCP Message Pool
//  [Arguments]:
//                  [BYTE] ucPortIdx
//                  [BYTE] *pucMsg
//  [Return]:
//                  [enHDCP_Err_Status]
//
//**************************************************************************
enHDCP_Err_Status drv_HDCPRx_MsgParser(BYTE ucPortIdx, BYTE *pucMsg)
{
	ULONG dwArgAddr = 0;

	dwArgAddr = (ULONG)(&gHdcpMbx_RxBuf[ucPortIdx][MSGID_IDX]);
	
    switch(*(BYTE*)pucMsg) // first byte, msgID
    {
        case AKE_Init:
			//check if there is other operation on going
			if((HdcpHandler[ucPortIdx].Polling == TRUE))// || (drv_HDCPMBX_CheckMsgState(ucPortIdx)!= MBXMSG_STATE_IDLE))
			{
				HDCP_DRV_DPRINTF("ake init int!! port %d\r\n", ucPortIdx);
				HdcpHandler[ucPortIdx].Polling = FALSE;
				drv_HDCPMBX_UpdateState(ucPortIdx, MBXMSG_STATE_INIT);
			}
			else
			{
				_drv_HDCPMBX_ClrMsgSlot(ucPortIdx);
				drv_HDCPMBX_UpdateState(ucPortIdx, MBXMSG_STATE_IDLE);
			}

            //init
            _drv_HDCPRx_ClrMbxRxBuf(ucPortIdx);

            //update FSM
            HdcpHandler[ucPortIdx].MainState = RxState_B0;
            HdcpHandler[ucPortIdx].SubState = AKE_Init;
            
            //fetch msg
            memcpy((void*)dwArgAddr, pucMsg, (RTX_SIZE + TXCAPS_SIZE + 1));

			//flush cache
			_drv_HDCPRx_FlushCache(dwArgAddr, gMsgArgLen[AKE_Init]);

			//assign first priority
            _drv_HDCPRx_MsgPoolAssignPriority(ucPortIdx, (DEV_NUM*2 + 1));

            _drv_HDCPRx_ParserMessage(ucPortIdx, gMsgArgLen[AKE_Init]);
            
            HDCP_DRV_DPRINTF("** HDCP22 receive AKE init port %d \r\n", ucPortIdx);

            break;
        
        case AKE_No_Stored_km:
            //update FSM
            HdcpHandler[ucPortIdx].SubState = AKE_No_Stored_km;
            
            //if (gHdcpMbx_RxBuf[ucPortIdx][0] != 0) //already got message
            if(gHdcpMbx_RxBuf[ucPortIdx][0] != 0) //already got message
            {
                return HDCP_PROCESS_FAIL;
            }

            //init
            _drv_HDCPRx_ClrMbxRxBuf(ucPortIdx);

            //fetch msg
            memcpy((void*)dwArgAddr, pucMsg, EKPUBKM_SIZE + 1);

			//flush cache
			_drv_HDCPRx_FlushCache(dwArgAddr, gMsgArgLen[AKE_No_Stored_km]);
			
            _drv_HDCPRx_MsgPoolFillPriority(ucPortIdx);

            _drv_HDCPRx_ParserMessage(ucPortIdx, gMsgArgLen[AKE_No_Stored_km]);

            HDCP_DRV_DPRINTF("** HDCP22 receive No stored km port %d \r\n", ucPortIdx);
                        
            break;
        
        case AKE_Stored_km:
            //update FSM
            HdcpHandler[ucPortIdx].SubState = AKE_Stored_km;
            
            if(gHdcpMbx_RxBuf[ucPortIdx][0] != 0) //already got message
            {
                return HDCP_PROCESS_FAIL;
            }

            //init
            _drv_HDCPRx_ClrMbxRxBuf(ucPortIdx);

            //fetch msg
            memcpy((void*)dwArgAddr, pucMsg, (EKHKM_SIZE + M_SIZE + 1));

			//flush cache
			_drv_HDCPRx_FlushCache(dwArgAddr, gMsgArgLen[AKE_Stored_km]);
            
            _drv_HDCPRx_MsgPoolFillPriority(ucPortIdx);

            _drv_HDCPRx_ParserMessage(ucPortIdx, gMsgArgLen[AKE_Stored_km]);

            HDCP_DRV_DPRINTF("** HDCP22 receive Stored km port %d \r\n", ucPortIdx);
			
            break;
        
        case LC_Init:
            //update FSM
            HdcpHandler[ucPortIdx].SubState = LC_Init;
            
            if(gHdcpMbx_RxBuf[ucPortIdx][0] != 0) //already got message
            {
                return HDCP_PROCESS_FAIL;
            }

            //init
            _drv_HDCPRx_ClrMbxRxBuf(ucPortIdx);

            //fetch msg
            memcpy((void*)dwArgAddr, pucMsg, RN_SIZE + 1);

			//flush cache
			_drv_HDCPRx_FlushCache(dwArgAddr, gMsgArgLen[LC_Init]);
            
            _drv_HDCPRx_MsgPoolFillPriority(ucPortIdx);

            _drv_HDCPRx_ParserMessage(ucPortIdx, gMsgArgLen[LC_Init]);

            HDCP_DRV_DPRINTF("** HDCP22 receive LC init port %d \r\n", ucPortIdx);
            			
            break;
        
        case SKE_Send_Eks:
            //update FSM
            HdcpHandler[ucPortIdx].SubState = SKE_Send_Eks;
            
            if(gHdcpMbx_RxBuf[ucPortIdx][0] != 0) //already got message
            {
                return HDCP_PROCESS_FAIL;
            }

            //init
            _drv_HDCPRx_ClrMbxRxBuf(ucPortIdx);

            //fetch msg
            memcpy((void*)dwArgAddr, pucMsg, (EDKEYKS_SIZE + RIV_SIZE + 1));

			//flush cache
			_drv_HDCPRx_FlushCache(dwArgAddr, gMsgArgLen[SKE_Send_Eks]);

            _drv_HDCPRx_MsgPoolFillPriority(ucPortIdx);

            _drv_HDCPRx_ParserMessage(ucPortIdx, gMsgArgLen[SKE_Send_Eks]);

            HDCP_DRV_DPRINTF("** HDCP22 receive Send EKS port %d \r\n", ucPortIdx);
            			
            break;
        
        default:
            return HDCP_UNKNOWN_STATE;
            break;
    }
    
    return HDCP_ERR_NONE;
}

//**************************************************************************
//  [Function Name]:
//                  drv_HDCP_Handler()
//  [Description]:
//                  HDCP task handler
//  [Arguments]:
//                  void
//  [Return]:
//                  void
//**************************************************************************
void drv_HDCPRx_Handler(void)
{
    BYTE ucIndex = 0;
    
    //fetch first priority
    ucIndex = _drv_HDCPRx_MsgPoolFetchMsgIdx();

    //printf("priority = %d\r\n", gHdcpMbx_RxBuf[0][0]);

    if(ucIndex != (DEV_NUM + 1))
    {
        _drv_HDCPRx_FSM(ucIndex);
    }
}

#endif // #if (ENABLE_SECU_R2 == 1) && (ENABLE_HDCP22 == 1)

#endif


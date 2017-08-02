///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    drvHDCPMbx.c
/// @author MStar Semiconductor Inc.
/// @brief  HDCP22 driver Function
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef HAL_MBX_C
#define HAL_MBX_C

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "Global.h"

#if (ENABLE_SECU_R2 == 1) && (ENABLE_HDCP22 == 1)
#include <string.h>
#include "drvHDCPMbx.h"
#include "drvISR.h"

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
//static LONG g_dwMBXBaseAddr = 0;
#define HDCP22_MBX_BASE_ADDR    0x103320
#define HDCP22_KEY_MESSAGE_ADDR 0x103300

//#define MBX_REG(offset)         (g_dwMBXBaseAddr + offset)
#define MBX_REG(offset)                         (HDCP22_MBX_BASE_ADDR + offset)
#define MBXReadByte(offset)                     msReadByte(MBX_REG(offset))
#define MBXRead4Byte(offset)                    msRead4Byte(MBX_REG(offset))
#define MBXWriteByte(offset, value)             msWriteByte(MBX_REG(offset), value)
#define MBXWriteByteMask(offset, value, mask)   msWriteByteMask(MBX_REG(offset), value, mask)
#define MBXWrite4Byte(offset, value)            msWrite4Byte(MBX_REG(offset), value)

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
//                  _drv_HDCPMBX_GetMsgSlotOffset()
//  [Description]:
//                  There will be various number of message slots in Mailbox, 
//                  according to the port number in board define.
//                  This function will return the address offset of each slot.
//  [Arguments]:
//                  [BYTE] ucSlotIdx: index of slots
//  [Return]:
//                  [WORD] address offset of specific slot
//
//**************************************************************************
WORD _drv_HDCPMBX_GetMsgSlotOffset(BYTE ucSlotIdx)
{
    if(ucSlotIdx > MBX_MAX_SLOT)
    {
        //print dbg msg here;
        return MBX_INVALID_SLOT_IDX; //TBD; error handler
    }

    return (ucSlotIdx* HDCPMBX_SLOT_SIZE);
}

//**************************************************************************
//  [Function Name]:
//                  _drv_HDCPMBX_ClrMsgSlot()
//  [Description]:
//                  clear specific slot's content, set to default value 0
//  [Arguments]:
//                  [BYTE] ucSlotIdx: index of slots
//  [Return]:
//                  [enHDCPMBX_ERROR_CODE] return error status
//
//**************************************************************************
enHDCPMBX_ERROR_CODE _drv_HDCPMBX_ClrMsgSlot(BYTE ucSlotIdx)
{
    BYTE uctemp = 0;
    WORD wSlotOffset = 0;

    wSlotOffset = _drv_HDCPMBX_GetMsgSlotOffset(ucSlotIdx);

    if(wSlotOffset == MBX_INVALID_SLOT_IDX)
    {
        return MBX_ERR_INVALID_PORTIDX;
    }
    
    for(uctemp = 0; uctemp < sizeof(stHDCPMBX_HANDLER); uctemp++ )
    {
        MBXWriteByte(wSlotOffset + uctemp, 0);
    }

    return MBX_ERR_NONE;
}

//**************************************************************************
//  [Function Name]:
//                  _drv_HDCPMBX_CheckMutexFree()
//  [Description]:
//                  Treat bit 7 of first byte of message as Mutex key.
//                  If there is any other slot is currently processing, then Mutex is unavailable.
//  [Arguments]:
//                  [BYTE] ucSlotIdx: index of slots
//  [Return]:
//                  [BOOL] return if the Mutex key is available
//
//**************************************************************************
BOOL _drv_HDCPMBX_CheckMutexFree(BYTE ucSlotIdx)
{
    BYTE uctmep = 0;
    WORD wSlotOffset = 0;

    for(uctmep = 0; uctmep < MBX_MAX_SLOT; uctmep++ )
    {
        if(uctmep != ucSlotIdx)
        {
            wSlotOffset = _drv_HDCPMBX_GetMsgSlotOffset(uctmep);
            
            if((wSlotOffset == MBX_INVALID_SLOT_IDX) || (MBXReadByte(wSlotOffset) & BIT7))
            {
                return FALSE;
            }
        }
    }
    
    return TRUE;
}

//**************************************************************************
//  [Function Name]:
//                  _drv_HDCPMBX_FillErrorCode()
//  [Description]:
//                  Fill content of error code field, it includes:
//                  BIT 7:      host CPU, it could be HK R2, SEC R2
//                  BIT 6:      indicate if this message has parameter attached.
//                  BIT 2..0:  message status
//  [Arguments]:
//                  [enHDCPMBX_CPU_LIST] enHost: host CPU
//                  [enHDCPMBX_ERROR_CODE] enErrCode: Error code
//                  [BYTE] ucSlotIdx: index of message slot
//  [Return]:
//                  [enHDCPMBX_ERROR_CODE] Error status
//
//**************************************************************************
enHDCPMBX_ERROR_CODE _drv_HDCPMBX_FillErrorCode(enHDCPMBX_CPU_LIST enHost, enHDCPMBX_ERROR_CODE enErrCode, BYTE ucSlotIdx)
{
    WORD wSlotOffset;

    wSlotOffset = _drv_HDCPMBX_GetMsgSlotOffset(ucSlotIdx);

    if(wSlotOffset == MBX_INVALID_SLOT_IDX)
    {
        return MBX_ERR_INVALID_PORTIDX;
    }

    MBXWriteByte(wSlotOffset + MBX_STATUS_IDX, ((enHost << 7) | (MBXMSG_WITHOUT_ARG << 6)) | MBXMSG_STATE_HALT);
    MBXWriteByte(wSlotOffset + MBX_ERRCODE_IDX, enErrCode);

    return MBX_ERR_NONE;
}

//**************************************************************************
//  [Function Name]:
//                  _drv_HDCPMBX_CheckMsgValid()
//  [Description]:
//                  Verify the message index and content.
//  [Arguments]:
//                  [enHDCPMBX_CPU_LIST] enClient: receiver 
//                  [BYTE] ucSlotIdx: index of slots
//  [Return]:
//                  [BOOL] return if the message content is valid
//
//**************************************************************************
BOOL _drv_HDCPMBX_CheckMsgValid(enHDCPMBX_CPU_LIST enClient, BYTE ucSlotIdx)
{
    WORD wSlotOffset;
    //enMBX_CPU_LIST enClient;

#if 0
    if (enHost == MBX_HKR2)
        enClient = MBX_SECR2;
    else
        enClient = MBX_HKR2;
#endif

    wSlotOffset = _drv_HDCPMBX_GetMsgSlotOffset(ucSlotIdx);

    //check if index is valid
    if (wSlotOffset == MBX_INVALID_SLOT_IDX)
    {
        return FALSE;
    }
    
    //check if slot is actived
    if (!(MBXReadByte(wSlotOffset) & BIT7))
    {
        return FALSE;
    }

    //check host
    if((enClient == MBX_HKR2) && ((MBXReadByte(wSlotOffset + MBX_STATUS_IDX) >> 7) == MBX_SECR2))
    {
        return TRUE;
    }
    else if((enClient == MBX_SECR2) && ((MBXReadByte(wSlotOffset + MBX_STATUS_IDX) >> 7) == MBX_HKR2))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

//**************************************************************************
//  [Function Name]:
//                  drv_HDCPMBX_ClearActiveBit()
//  [Description]:
//                  Clear Active Bit, indicate process is Done
//  [Arguments]:
//                  [BYTE] ucSlotIdx
//  [Return]:
//                  void
//
//**************************************************************************
void drv_HDCPMBX_ClearActiveBit(BYTE ucIdx)
{
    WORD wOffset = 0;
    
    wOffset = _drv_HDCPMBX_GetMsgSlotOffset(ucIdx);
    
    MBXWriteByte(wOffset + MBX_MSGID_IDX, (MBXReadByte(wOffset + MBX_MSGID_IDX) & (~BIT7)));
}

//**************************************************************************
//  [Function Name]:
//                  drv_HDCPMBX_Init()
//  [Description]:
//                  Initialized mailbox content, clear all content to zero.
//  [Arguments]:
//                  none
//  [Return]:
//                  [enHDCPMBX_ERROR_CODE] Return error status
//
//**************************************************************************
enHDCPMBX_ERROR_CODE drv_HDCPMBX_Init(void)//(LONG dwMBXBaseAddr)
{
    WORD ustemp = 0;

    //set mailbox base address
    //g_dwMBXBaseAddr = dwMBXBaseAddr;
    
    //init structure, init slots
#if 0
    for(ustemp = 0; ustemp < (MBX_MAX_SLOT * sizeof(stMBX_HANDLER)); ustemp++ )
    {
        MBXWriteByte(ustemp, 0x00);
    }
    
#else
    for(ustemp = 0; ustemp < MBX_MAX_SLOT; ustemp++)
    {
        _drv_HDCPMBX_ClrMsgSlot(ustemp);
		drv_HDCPMBX_UpdateState(ustemp, MBXMSG_STATE_IDLE);
    }
    
#endif

    //printf("[MBX] init mailbox\r\n");

    
    //init interrupt
    //TBD
    return MBX_ERR_NONE;
}

//**************************************************************************
//  [Function Name]:
//                  drv_HDCPMBX_UpdateState()
//  [Description]:
//                  Set message state field
//  [Arguments]:
//                  [BYTE] ucSlotIdx: index of current message slot,
//                  [enHDCPMBX_MSG_STATE] enState: state to be set
//  [Return]:
//                  [enHDCPMBX_ERROR_CODE] Return error status
//
//**************************************************************************
enHDCPMBX_ERROR_CODE drv_HDCPMBX_UpdateState(BYTE ucSlotIdx, enHDCPMBX_MSG_STATE enState)
{
    WORD wSlotOffset = 0;
    
    wSlotOffset = _drv_HDCPMBX_GetMsgSlotOffset(ucSlotIdx);
    
    if(wSlotOffset == MBX_INVALID_SLOT_IDX)
    {
        return MBX_ERR_INVALID_PORTIDX;
    }

    MBXWriteByteMask(wSlotOffset + MBX_STATUS_IDX, enState, ((BIT2 | BIT1)|BIT0));
    
    return MBX_ERR_NONE;
}

//**************************************************************************
//  [Function Name]:
//                  drv_HDCPMBX_CheckMsgState()
//  [Description]:
//                   check if the operation of specific index of message is done
//  [Arguments]:
//                  [BYTE] ucSlotIdx: index of current message slot,
//  [Return]:
//                  [enHDCPMBX_MSG_STATE] Return message status
//
//**************************************************************************
enHDCPMBX_MSG_STATE drv_HDCPMBX_CheckMsgState(BYTE ucSlotIdx)
{
    WORD wSlotOffset = 0;
    
    wSlotOffset = _drv_HDCPMBX_GetMsgSlotOffset(ucSlotIdx);
    
    if(wSlotOffset == MBX_INVALID_SLOT_IDX)
    {
        return MBXMSG_STATE_HALT;
    }

    return (enHDCPMBX_MSG_STATE)(MBXReadByte(wSlotOffset + MBX_STATUS_IDX) & 0x07);
}

//**************************************************************************
//  [Function Name]:
//                  drv_HDCPMBX_SetMsg()
//  [Description]:
//                  Fill up all message field with input arguments
//  [Arguments]:
//                  [BYTE] ucSlotIdx: message index slot
//                  [BYTE] ucMsgID: message ID
//                  [enHDCPMBX_CPU_LIST] enHost: host CPU
//                  [enHDCPMBX_MSG_ARG_ATTR] enArgAttribute: indicate if there is any parameter within message
//                  [enHDCPMBX_MSG_STATE] enStatus
//                  [enHDCPMBX_ERROR_CODE] enErrCode: Error code
//                  [LONG] dwArgAddr: Memory address of the arguments
//                  [LONG] dwArgSize: Total Size of arguments
//  [Return]:
//                  [enMBX_ERROR_CODE] Return error status
//
//**************************************************************************
enHDCPMBX_ERROR_CODE drv_HDCPMBX_SetMsg(BYTE ucSlotIdx, BYTE ucMsgID, enHDCPMBX_CPU_LIST enHost, enHDCPMBX_MSG_ARG_ATTR enArgAttribute, enHDCPMBX_MSG_STATE enStatus, enHDCPMBX_ERROR_CODE enErrCode, ULONG dwArgAddr, ULONG dwArgSize)
{
    //check if any other active port
    if (_drv_HDCPMBX_CheckMutexFree(ucSlotIdx))
    {
        WORD wSlotOffset = 0;
        
        wSlotOffset = _drv_HDCPMBX_GetMsgSlotOffset(ucSlotIdx);

		//printf("wSlotOffset = %d\r\n", wSlotOffset);

        if(wSlotOffset == MBX_INVALID_SLOT_IDX)
        {
            return MBX_ERR_INVALID_PORTIDX;
        }

        _drv_HDCPMBX_ClrMsgSlot(ucSlotIdx);
        
        //fill MsgID field
        MBXWriteByte(wSlotOffset, (MBX_Port_Active | ucMsgID));

        //fill status field
        MBXWriteByte(wSlotOffset + 1, (((enHost << 7) | (enArgAttribute << 6)) | enStatus));

        //fill error code field
        MBXWriteByte(wSlotOffset + 2, enErrCode);

        //fill argument address/size
        if (enArgAttribute != 0)
        {
            MBXWrite4Byte(wSlotOffset + 3, dwArgAddr | 0x80000000);
            MBXWrite4Byte(wSlotOffset + 7, dwArgSize);
        }

        // trigger interrupt to receiver
        if (enHost == MBX_HKR2)
        {
            msWriteByte(0x100544, 0x00);
            msWriteByte(0x100544, 0x04);
        }
        else
        {
            msWriteByte(0x10054C, 0x00);
            msWriteByte(0x10054C, 0x02);
        }

        return MBX_ERR_NONE;
    }
    
    return MBX_ERR_MSG_SEND_FAIL;
}

//**************************************************************************
//  [Function Name]:
//                  drv_HDCPMBX_GetMsg()
//  [Description]:
//                  Fetch message from mailbox
//  [Arguments]:
//                  [enHDCPMBX_CPU_LIST] enClient: client CPU who is going to fetch this message
//                  [stHDCPMBX_HANDLER] *pMBXHnadler: structure to store message content
//  [Return]:
//                  [enHDCPMBX_ERROR_CODE] Return error status
//
//**************************************************************************
enHDCPMBX_ERROR_CODE drv_HDCPMBX_GetMsg(enHDCPMBX_CPU_LIST enClient, stHDCPMBX_HANDLER* pMBXHnadler)
{
    BOOL bIsMsgValid = FALSE;
    BYTE uctemp = 0;
    BYTE ucSlotIdx = MBX_ERR_INVALID_PORTIDX;
    WORD wSlotOffset = MBX_INVALID_SLOT_IDX;

    //if (enClient == MBX_HKR2)
        //bIsMsgValid = _hal_MBX_CheckMsgValid(enClient, i)
    //else
    {
        for(uctemp = 0; uctemp < MBX_MAX_SLOT; uctemp++)
        {
            if(_drv_HDCPMBX_CheckMsgValid(enClient, uctemp))
            {
                bIsMsgValid = TRUE;
                ucSlotIdx = uctemp;
                
                break;
            }
        }
    }

    if(bIsMsgValid)
    {
        wSlotOffset = _drv_HDCPMBX_GetMsgSlotOffset(ucSlotIdx);

        //check if index is valid
        //if (wSlotOffset == MBX_INVALID_SLOT_IDX)
            //return MBX_ERR_INVALID_PORTIDX;
        
        pMBXHnadler->MsgID = MBXReadByte(MBX_MSGID_IDX);
        pMBXHnadler->Status = MBXReadByte(MBX_STATUS_IDX);
        pMBXHnadler->ErrCode = MBXReadByte(MBX_ERRCODE_IDX);
        
        if(pMBXHnadler->Status & BIT6)
        {
            pMBXHnadler->ArgAddr = MBXRead4Byte(MBX_ARGADDR_IDX);
            pMBXHnadler->ArgSize = MBXRead4Byte(MBX_ARGSIZE_IDX);
        }
        else
        {
            pMBXHnadler->ArgAddr = 0;
            pMBXHnadler->ArgSize= 0;
        }
        
        if(enClient == MBX_SECR2)
        {
            drv_HDCPMBX_UpdateState(ucSlotIdx, MBXMSG_STATE_PROCESSING); //update state first
        }
            
        return MBX_ERR_NONE;
    }
    else 
    {
        if(ucSlotIdx != MBX_ERR_INVALID_PORTIDX)
        {
            drv_HDCPMBX_UpdateState(ucSlotIdx, MBXMSG_STATE_HALT);
        }
        
        return MBX_ERR_MSG_RECEIVE_FAIL; // can't get proper msg
    }
}

//**************************************************************************
//  [Function Name]:
//                  drv_HDCPMBX_SetKeyMessage()
//  [Description]:
//                  Fill up all message field with input arguments
//  [Arguments]:
//                  [BYTE] ucMsgID: message ID
//                  [enHDCPMBX_CPU_LIST] enHost: host CPU
//                  [enHDCPMBX_MSG_ARG_ATTR] enArgAttribute: indicate if there is any parameter within message
//                  [enHDCPMBX_MSG_STATE] enStatus
//                  [enHDCPMBX_ERROR_CODE] enErrCode: Error code
//                  [LONG] dwArgAddr: Memory address of the arguments
//                  [LONG] dwArgSize: Total Size of arguments
//  [Return]:
//                  [enMBX_ERROR_CODE] Return error status
//
//**************************************************************************
enHDCPMBX_ERROR_CODE drv_HDCPMBX_SetKeyMessage(BYTE ucMsgID, enHDCPMBX_CPU_LIST enHost, enHDCPMBX_MSG_ARG_ATTR enArgAttribute, enHDCPMBX_MSG_STATE enStatus, enHDCPMBX_ERROR_CODE enErrCode, ULONG dwArgAddr, ULONG dwArgSize)
{
    //fill MsgID field
    msWriteByte(HDCP22_KEY_MESSAGE_ADDR, (MBX_Port_Active | ucMsgID));

    //fill status field
    msWriteByte(HDCP22_KEY_MESSAGE_ADDR + 1, (((enHost << 7) | (enArgAttribute << 6)) | enStatus));

    //fill error code field
    msWriteByte(HDCP22_KEY_MESSAGE_ADDR + 2, enErrCode);

    //fill argument address/size
    if (enArgAttribute != 0)
    {
        msWrite4Byte(HDCP22_KEY_MESSAGE_ADDR + 3, dwArgAddr | 0x80000000);
        msWrite4Byte(HDCP22_KEY_MESSAGE_ADDR + 7, dwArgSize);
    }

    // trigger interrupt to receiver
    if (enHost == MBX_HKR2)
    {
        msWriteByte(0x100544, 0x00);
        msWriteByte(0x100544, 0x04);
    }
    else
    {
        msWriteByte(0x10054C, 0x00);
        msWriteByte(0x10054C, 0x02);
    }

    return MBX_ERR_NONE;
}

#endif //#if (ENABLE_SECU_R2 == 1) && (ENABLE_HDCP22 == 1)
#endif

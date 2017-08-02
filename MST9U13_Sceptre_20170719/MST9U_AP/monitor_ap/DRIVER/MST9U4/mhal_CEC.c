
//#include "board.h"

#if ENABLE_CEC

#define _HAL_CEC_C_
#include "board.h"
#include "Datatype.h"
#include "ms_reg_MST9U.h"
#include "debug.h"
#include "Misc.h"
#include "Global.h"
#include "mhal_CEC.h"

#define ENABLE_HAL_CEC_DEBUG 0

#if (ENABLE_HAL_CEC_DEBUG == 1)
#define HALCEC_PRINT_MSG(str)          printMsg(str)//putstr(str)
#define HALCEC_PRINT_DATA(str, x)      printData(str, x)//printf(str, x)
#else
#define HALCEC_PRINT_MSG(str)
#define HALCEC_PRINT_DATA(str, x)
#endif

//**************************************************************************
//  [Function Name]:
//                  _hal_CEC_InitVariable()
//  [Description]:
//                  initialize CEC relative structure and variables
//  [Arguments]:
//                  none
//  [Return]:
//                  none
//
//**************************************************************************
void _hal_CEC_InitVariable(void)
{
    BYTE i, j;
    enMsCEC_SET_INDEX k = CEC_SET_IDX_0;

    HALCEC_PRINT_MSG("[CEC]func: _hal_CEC_InitVariable()");

    for ( k = CEC_SET_IDX_0; k < CEC_SET_IDX_MAX; k++)
    {
        if (gStCECInfo[k].bIsSetEnabled == TRUE)
        {
            gStCECInfo[k].bInitDone = FALSE;
            
            for ( i=0; i < 15; i++ )
                gStCECInfo[k].bIsDevExisted[i] = FALSE;

            gStCECInfo[k].bMSGQStartIdx = 0;
            gStCECInfo[k].bMSGQEndIdx = 0;
            gStCECInfo[k].bMSGCnt = 0;

            for ( i=0; i < CEC_MSGQ_DEPTH; i++ )
            {
                gStCECInfo[k].stRxBufList[i].bLength = 0;
                for ( j=0; j < CEC_FIFO_DEPTH; j++ )
                    gStCECInfo[k].stRxBufList[i].bData[j] = 0;
            }

            for ( i=0; i < CEC_FIFO_DEPTH; i++ )
                gStCECInfo[k].bTxBuf[i] = 0;

            gStCECInfo[k].enMyDevType = CEC_DEV_TV;
            gStCECInfo[k].enMyLogicAddr = CEC_LA_TV;
            gStCECInfo[k].bMyPhysicalAddr[0] = 0x10;
            gStCECInfo[k].bMyPhysicalAddr[1] = 0x00;
            gStCECInfo[k].enMyPwrStatus = CEC_PWRSTS_ON;

            gStCECInfo[k].bActDevCECVersion = CEC_VER_11;
            gStCECInfo[k].enActDevType = CEC_DEV_RESERVED;
            gStCECInfo[k].enActLogicAddr = CEC_LA_TV;
            gStCECInfo[k].bActPhysicalAddr[0] = 0x00;
            gStCECInfo[k].bActPhysicalAddr[1] = 0x00;
            gStCECInfo[k].enActPwrStatus = CEC_PWRSTS_STANDBY;
        }
    }

}

//**************************************************************************
//  [Function Name]:
//                  _hal_CEC_GetRegOffset()
//  [Description]:
//                  Get register address offset
//  [Arguments]:
//                  [enMsCEC_SET_INDEX] u8SetIndex: current CEC set index
//  [Return]:
//                  [WORD]
//
//**************************************************************************
WORD _hal_CEC_GetRegOffset(enMsCEC_SET_INDEX enSetIndex)
{
    WORD wOffset = 0;
    
    switch(enSetIndex)
    {
        case 0:
            wOffset = 0;
        break;
        case 1:
            wOffset = 0x80;
        break;
        case 2:
            wOffset = 0x100;
        break;
        case 3:
        default:
            wOffset = 0x180;
        break;
    }

    return wOffset;
}


//**************************************************************************
//  [Function Name]:
//                  _hal_CEC_SendFrame()
//  [Description]:
//                  initialize CEC relative structure and variables
//  [Arguments]:
//                  [enMsCEC_SET_INDEX] enSetIndex: current active set index
//                  [BYTE] header: header byte of frame to be sent;
//                  [enMsCEC_OPCODE_LIST] opCode: opcode of frame to be sent;
//                  [BYTE*] operand: operand of frame to be sent;
//                  [BYTE] length: total lenght of frame to be sent;
//  [Return]:
//                  [_enMsCEC_INT_EVENT_]
//
//**************************************************************************
enMsCEC_INT_EVENT _hal_CEC_SendFrame(enMsCEC_SET_INDEX enSetIndex, BYTE header, enMsCEC_OPCODE_LIST opCode, BYTE* operand, BYTE length )
{
    BYTE waitCnt = 0;
    BYTE cnt = 0;
    enMsCEC_INT_EVENT res;
    WORD wOffset = 0;

    if (gStCECInfo[enSetIndex].bIsSetEnabled == FALSE)
        return CEC_EVENT_FEATURE_ABORT;
    
    HALCEC_PRINT_MSG("[CEC]func: _hal_CEC_SendFrame()");
    HALCEC_PRINT_DATA("SetIdx [%x]\r\n", enSetIndex);
    HALCEC_PRINT_DATA("header [%x]\r\n", header);
    HALCEC_PRINT_DATA("opCode [%d]\r\n", opCode);
    HALCEC_PRINT_DATA("length [%d]\r\n", length);

    wOffset = _hal_CEC_GetRegOffset(enSetIndex);

    //Clear TX interrupt status;
    msWriteByteMask(REG_000625 + wOffset, 0x0E, 0x0E);
    msWriteByteMask(REG_000625 + wOffset, 0x00, 0x0E);

    //Fill TX data
    msWriteByte(REG_000630 + wOffset, header);
    msWriteByte(REG_000631 + wOffset, opCode);

    if ( length > 0 )
    {
        BYTE i;

        for ( i=0; i < length; i++ )
            msWriteByte(REG_000632 + wOffset + i, *(operand + i));
    }

    if ((opCode = CEC_OPCODE_POLLING_MESSAGE) && (length == 0))
    {
        waitCnt = 5;
        msWriteByteMask(REG_000600 + wOffset, length, 0x0F); //set TX length
    }
    else
    {
        waitCnt = 30;
        msWriteByteMask(REG_000600 + wOffset, (length+1), 0x0F);
    }

    //The total time,
    //(1). successful, 4.5 ms + 10 * 2.4 ms * N = 4.5 ms + 24 * N
    //              = 28.5 ms (1), or 52.5 ms (2), ....
    //(2). NAK,        (4.5 ms + 10 * 2.4 ms) * 1 + (4.5 ms + 10 * 2.4 ms +7.2 ms(3 bit time)) * retry (3)
    //              = 28.5 + 35.2 * 3 = 133.6 ms
    
    ForceDelay1ms(20);

    do
    {
        ForceDelay1ms(10);
        if (cnt++ >= waitCnt)
            break;
    } while ((msReadByte(REG_000623 + wOffset) & 0x0E) == 0);

    res = (enMsCEC_INT_EVENT)msReadByte(REG_000623 + wOffset);

    if (cnt >= waitCnt)
        res |= CEC_EVENT_SYSTEM_BUSY;

    //Clear TX interrupt status;
    msWriteByteMask(REG_000625 + wOffset, 0x0E, 0x0E);
    msWriteByteMask(REG_000625 + wOffset, 0x00, 0x0E);

    return res;
    
}

//**************************************************************************
//  [Function Name]:
//                  _hal_CEC_PingDevice()
//  [Description]:
//                  Get device logical address
//  [Arguments]:
//                  [enMsCEC_SET_INDEX] enSetIndex: current CEC set index
//                  [enMsCEC_DEV_LOGIC_ADDR] addr: logical address of device
//  [Return]:
//                  [enMsCEC_INT_EVENT]: ping result of the logical address
//
//**************************************************************************
enMsCEC_INT_EVENT _hal_CEC_PingDevice(enMsCEC_SET_INDEX enSetIndex, enMsCEC_DEV_LOGIC_ADDR addr)
{
    enMsCEC_INT_EVENT error_code = CEC_EVENT_FEATURE_ABORT;
    BYTE tmpAddr;

    if (gStCECInfo[enSetIndex].bIsSetEnabled == FALSE)
        return CEC_EVENT_FEATURE_ABORT;

    HALCEC_PRINT_MSG("[CEC]func: _hal_CEC_PingDevice()");

    if( addr >= CEC_LA_MAX )
    {
        error_code = CEC_EVENT_FEATURE_ABORT;
        return error_code;
    }

    tmpAddr = (BYTE)addr;

    error_code = _hal_CEC_SendFrame(enSetIndex, (( addr << 4 ) & 0xF0 ) | ( addr & 0x0F ), CEC_OPCODE_POLLING_MESSAGE, &tmpAddr, 0 );

    if (error_code & CEC_EVENT_TX_SUCCESS)
        error_code = CEC_EVENT_TX_SUCCESS;
    
    else if (error_code & CEC_EVENT_RETRY_FAIL)
        error_code = CEC_EVENT_RETRY_FAIL;
    
    else if (error_code & CEC_EVENT_LOST_ARBITRATION)
        error_code = CEC_EVENT_LOST_ARBITRATION;
    
    else if (error_code & CEC_EVENT_SYSTEM_BUSY)
        error_code = CEC_EVENT_SYSTEM_BUSY;

    return error_code;
}


//**************************************************************************
//  [Function Name]:
//                  _hal_CEC_GetSelfAddress()
//  [Description]:
//                  Get device logical address
//  [Arguments]:
//                  none
//  [Return]:
//                  none
//
//**************************************************************************
void _hal_CEC_GetSelfAddress(enMsCEC_SET_INDEX enSetIndex)
{
    WORD wOffset = 0;
    
    HALCEC_PRINT_MSG("[CEC]func: _hal_CEC_GetSelfAddress()");

    wOffset = _hal_CEC_GetRegOffset(enSetIndex);

    if ( _hal_CEC_PingDevice(enSetIndex, CEC_LA_TV) == CEC_EVENT_RETRY_FAIL )
        gStCECInfo[enSetIndex].enMyLogicAddr = CEC_LA_TV;
    
    else if ( _hal_CEC_PingDevice(enSetIndex, CEC_LA_FREE_USE) == CEC_EVENT_RETRY_FAIL )
        gStCECInfo[enSetIndex].enMyLogicAddr = CEC_LA_FREE_USE;
    
    else 
        gStCECInfo[enSetIndex].enMyLogicAddr = CEC_LA_UNREGISTERED;

    msWriteByteMask(REG_000604 + wOffset, gStCECInfo[enSetIndex].enMyLogicAddr << 4, 0xF0); //[7:4]: logical address

}

//**************************************************************************
//  [Function Name]:
//                  hal_CEC_ConfigPinMapping()
//  [Description]:
//                  initialize CEC variables, register settings.
//  [Arguments]:
//                  [enMsCEC_PIN_MAPPING] *enCECPinMap: array which to store GPIO mapping for each CEC set
//  [Return]:
//                  none
//
//**************************************************************************
void hal_CEC_EnableSet(WORD bCECSetConfig)
{
    enMsCEC_SET_INDEX i;

    HALCEC_PRINT_MSG("[CEC] hal_CEC_EnableSet\r");
    
    //init array first
    for ( i = CEC_SET_IDX_0; i < CEC_SET_IDX_MAX; i++)
        gStCECInfo[i].bIsSetEnabled = FALSE;
    
    for ( i = CEC_SET_IDX_0; i < CEC_SET_IDX_MAX; i++)
		gStCECInfo[i].bIsSetEnabled = ((bCECSetConfig >> i) & BIT0) ? TRUE : FALSE;
    
}

//**************************************************************************
//  [Function Name]:
//                  hal_CEC_Init()
//  [Description]:
//                  initialize CEC variables, register settings.
//  [Arguments]:
//                  none
//  [Return]:
//                  none
//
//**************************************************************************
void hal_CEC_Init(void)
{
    enMsCEC_SET_INDEX k = CEC_SET_IDX_0;
    WORD clkCnt = 0;

    //NOTE: initialize 4 sets CEC at once
    HALCEC_PRINT_MSG("[CEC]func: hal_CEC_Init\r");
    
    _hal_CEC_InitVariable();

    for ( k = CEC_SET_IDX_0; k < CEC_SET_IDX_MAX; k++ )
    {
        if (gStCECInfo[k].bIsSetEnabled == TRUE)
        {
            WORD wOffset = 0;
            
            wOffset = _hal_CEC_GetRegOffset(k);
            
            msWriteByteMask(REG_000601 + wOffset, (0x10 | CEC_RETRY_CNT), 0x1F); //[4:3]: 1.5x low bit period [2:0]=CEC_RETRY_CNT
            msWriteByte(REG_000603 + wOffset, ((CEC_BUS_FREE_TIME << 4) | CEC_RESEND_INTERVAL)); //[3:0]: cnt1, [7:4]: cnt2
            msWriteByte(REG_000604 + wOffset,  ((CEC_LA_TV << 4) | CEC_FRAME_INTERVAL)); //[3:0]: cnt3, [7:4]: logical address: TV

            //Number of counts to achieve 10us
            clkCnt = (CRYSTAL_CLOCK % 100000L)*0.00016 + 0.5;
            msWriteByte(REG_000605 + wOffset, CRYSTAL_CLOCK/100000L); // integer part
            msWriteByteMask(REG_000606 + wOffset, clkCnt, 0x0F); // CEC time unit by Xtal(fractional)

            //select sw CEC ontroller
            msWriteByteMask(REG_000628 + wOffset, 0x03, _BIT1|_BIT0); //[1]: 0: clock source from xtal
            
            //[1:0]: tx rising shift 0us, [3:2]: tx falling shift 0us, [4]: cancel current tx = false, [5]: CEC main clock no gating, 
            //[6]: rx test mode:normal, [7]: enable PCM CEC controller
            msWriteByte(REG_000602 + wOffset, 0x80);

            msWriteByteMask(REG_000607 + wOffset, 0x00, _BIT4); //[4]: select CEC controller, 0:software / 1: PM HW 

            //clear CEC interrupt status
            msWriteByteMask(REG_000625 + wOffset, 0x1F, 0x1F);
            msWriteByteMask(REG_000625 + wOffset, 0x00, 0x1F);

            //undercheck
            //enable CEC function
            switch (k)
            {
                case CEC_SET_IDX_0:
                    msWriteByteMask(REG_000406, _BIT4, _BIT4); //[4]: 1: Enable GPIO26 as CEC function
                break;
                case CEC_SET_IDX_1:
                    msWriteByteMask(REG_000407, _BIT3, _BIT3); //[3]: 1: Enable GPIO25 as CEC function
                break;
                case CEC_SET_IDX_2:
                    msWriteByteMask(REG_000417, _BIT0, _BIT0); //[0]: 1: Enable GPIO24 as CEC function
                break;
                case CEC_SET_IDX_3:
                default:
					printf("[CEC] Invalid CEC Pad Setting!!\r\n"); //MST9U3 only has 3 GPIO as CEC pin
                    //msWriteByteMask(REG_000417, _BIT1, _BIT1); //[1]: 1: Enable GPIO23 as CEC function                    
                break;
            }
            
            #if (ENABLE_CEC_INT == 1)
            //TBD
            #endif

            _hal_CEC_GetSelfAddress(k);

            gStCECInfo[k].bInitDone = TRUE;
        }
    }
    HALCEC_PRINT_MSG("[CEC]func: drv_CEC_Init done\r");

}

//**************************************************************************
//  [Function Name]:
//                  hal_CEC_CheckExistedDevice()
//  [Description]:
//                  check current existed device
//  [Arguments]:
//                  [enMsCEC_SET_INDEX] enSetIndex: current active CEC set index
//  [Return]:
//                  none
//
//**************************************************************************
void hal_CEC_CheckExistedDevice(enMsCEC_SET_INDEX enSetIndex)
{
    BYTE i;
    enMsCEC_INT_EVENT ret;

    HALCEC_PRINT_MSG("[CEC]func: hal_CEC_CheckExistedDevice()");

    if (gStCECInfo[enSetIndex].bIsSetEnabled == FALSE)
        return;

    for ( i = CEC_LA_TV; i < CEC_LA_MAX; i++ )
    {
        ret = _hal_CEC_SendFrame(enSetIndex, ((gStCECInfo[enSetIndex].enMyLogicAddr << 4) & 0xF0)|( i & 0x0F), CEC_OPCODE_POLLING_MESSAGE, &i, 0);

        if ( ret & CEC_EVENT_TX_SUCCESS )
        {
            gStCECInfo[enSetIndex].bIsDevExisted[i] = TRUE;
            gStCECInfo[enSetIndex].enActLogicAddr = (enMsCEC_DEV_LOGIC_ADDR)i;
        }
        else
            gStCECInfo[enSetIndex].bIsDevExisted[i] = FALSE;
    }

}

//**************************************************************************
//  [Function Name]:
//                  hal_CEC_CheckRxBuffer()
//  [Description]:
//                  check RX buffer
//  [Arguments]:
//                  [enMsCEC_SET_INDEX] enSetIndex: current active CEC set index
//  [Return]:
//                  none
//
//**************************************************************************
void hal_CEC_CheckRxBuffer(enMsCEC_SET_INDEX enSetIndex) //don't place print message in this function
{
    BYTE i;
    WORD wOffset;

    //HALCEC_PRINT_MSG("[CEC]func: hal_CEC_CheckRxBuffer()");
    
    if (gStCECInfo[enSetIndex].bIsSetEnabled == FALSE)
        return;

    wOffset = _hal_CEC_GetRegOffset(enSetIndex);
    
    if ( msReadByte( REG_000623 + wOffset ) & _BIT0 ) //[0]: receive a new message successfully
    {
        gStCECInfo[enSetIndex].stRxBufList[gStCECInfo[enSetIndex].bMSGQEndIdx].bLength = (msReadByte(REG_000608 + wOffset) & 0x1F) + 1; //Get received message length

        if (gStCECInfo[enSetIndex].stRxBufList[gStCECInfo[enSetIndex].bMSGQEndIdx].bLength > 1)
        {
            for ( i = 0; i < gStCECInfo[enSetIndex].stRxBufList[gStCECInfo[enSetIndex].bMSGQEndIdx].bLength; i++ )
                gStCECInfo[enSetIndex].stRxBufList[gStCECInfo[enSetIndex].bMSGQEndIdx].bData[i] = msReadByte(REG_000640 + wOffset + i); //extract message data

            gStCECInfo[enSetIndex].bMSGCnt++;
            gStCECInfo[enSetIndex].bMSGQEndIdx = ((++gStCECInfo[enSetIndex].bMSGQEndIdx) >= CEC_MSGQ_DEPTH ? 0 : gStCECInfo[enSetIndex].bMSGQEndIdx);
        }

        //clear RX interrupt status
        msWriteByteMask(REG_000625 + wOffset, _BIT0, _BIT0);
        msWriteByteMask(REG_000625 + wOffset, 0, _BIT0);
    }
}

//**************************************************************************
//  [Function Name]:
//                  hal_CEC_ProcessTX()
//  [Description]:
//                  trigger TX operation
//  [Arguments]:
//                  [enMsCEC_SET_INDEX] enSetIndex: current active CEC set index
//                  [enMsCEC_DEV_LOGIC_ADDR] dst_addr:
//                  [enMsCEC_OPCODE_LIST] msg:
//                  [BYTE]* operand_ptr:
//                  [BYTE] len:
//  [Return]:
//                  [enMsCEC_INT_EVENT]
//
//**************************************************************************
enMsCEC_INT_EVENT hal_CEC_ProcessTX(enMsCEC_SET_INDEX enSetIndex, enMsCEC_DEV_LOGIC_ADDR dst_addr, enMsCEC_OPCODE_LIST msg, BYTE* operand_ptr, BYTE len)
{
    enMsCEC_INT_EVENT retVal = CEC_EVENT_FEATURE_ABORT;
    BYTE header;

    if (gStCECInfo[enSetIndex].bIsSetEnabled == FALSE)
        return CEC_EVENT_FEATURE_ABORT;

    HALCEC_PRINT_MSG("[CEC]func: hal_CEC_ProcessTX()");
    HALCEC_PRINT_DATA("SetIdx [%x]\r\n", enSetIndex);
    HALCEC_PRINT_DATA("dst_addr = %d\r", dst_addr);
    HALCEC_PRINT_DATA("opcode = %d\r", msg);    
    
    header = (( gStCECInfo[enSetIndex].enMyLogicAddr << 4 ) & 0xF0) | (dst_addr & 0x0F);

    retVal = _hal_CEC_SendFrame(enSetIndex, header, msg, operand_ptr, len);

    if (retVal & CEC_EVENT_TX_SUCCESS)
        retVal = CEC_EVENT_TX_SUCCESS;
    else if (retVal & CEC_EVENT_RETRY_FAIL)
        retVal = CEC_EVENT_RETRY_FAIL;
    else if (retVal & CEC_EVENT_LOST_ARBITRATION)
        retVal = CEC_EVENT_LOST_ARBITRATION;
    else if (retVal & CEC_EVENT_SYSTEM_BUSY)
        retVal = CEC_EVENT_SYSTEM_BUSY;

    if ((msg == CEC_OPCODE_UI_PRESS) && (retVal == CEC_EVENT_TX_SUCCESS))
    {
        retVal = _hal_CEC_SendFrame(enSetIndex, header, CEC_OPCODE_UI_RELEASE, operand_ptr, 0);

        if (retVal & CEC_EVENT_TX_SUCCESS)
            retVal = CEC_EVENT_TX_SUCCESS;
        else if (retVal & CEC_EVENT_RETRY_FAIL)
            retVal = CEC_EVENT_RETRY_FAIL;
        else if (retVal & CEC_EVENT_LOST_ARBITRATION)
            retVal = CEC_EVENT_LOST_ARBITRATION;
        else if (retVal & CEC_EVENT_SYSTEM_BUSY)
            retVal = CEC_EVENT_SYSTEM_BUSY;
    }

    return retVal;

}

//**************************************************************************
//  [Function Name]:
//                  hal_CEC_CheckFrame()
//  [Description]:
//                  check if frame is valid or not
//  [Arguments]:
//                  [enMsCEC_SET_INDEX] enSetIndex: current active CEC set index
//                  [enMsCEC_MSGTRANS_TYPE] MsgType: CEC message types;
//                  [BYTE] length: CEC command length, including header and opcode;
//  [Return]:
//                  [BOOL]
//
//**************************************************************************
BOOL hal_CEC_CheckFrame(enMsCEC_SET_INDEX enSetIndex, enMsCEC_MSGTRANS_TYPE MsgType, BYTE length)
{
    BOOL isMsgBoardcast, isLenLegal, retVal;

    retVal = FALSE;

    if (gStCECInfo[enSetIndex].bIsSetEnabled == FALSE)
        return FALSE;

    HALCEC_PRINT_MSG("[CEC]func: hal_CEC_CheckFrame()");
    HALCEC_PRINT_DATA("SetIdx [%x]\r\n", enSetIndex);

    isMsgBoardcast = ((gStCECInfo[enSetIndex].stRxBufList[gStCECInfo[enSetIndex].bMSGQStartIdx].bData[0] & 0x0F) == 0x0F);
    isLenLegal = ((gStCECInfo[enSetIndex].stRxBufList[gStCECInfo[enSetIndex].bMSGQStartIdx].bLength) >= length);

    if ((gStCECInfo[enSetIndex].stRxBufList[gStCECInfo[enSetIndex].bMSGQStartIdx].bData[0] & 0xF0) == 0xF0)
    {
        switch (gStCECInfo[enSetIndex].stRxBufList[gStCECInfo[enSetIndex].bMSGQStartIdx].bData[1]) //check opcode
        {
            case CEC_OPCODE_INACTIVE_SOURCE:
            case CEC_OPCODE_REQ_ACTIVE_SOURCE:
            case CEC_OPCODE_ROUTING_CHANGE:
            case CEC_OPCODE_ROUTING_INFO:
            case CEC_OPCODE_SET_STREAM_PATH:

            case CEC_OPCODE_STANDBY:

            //following command invoke broadcast response
            case CEC_OPCODE_GIVE_PHY_ADDR:
            case CEC_OPCODE_GET_MENU_LANGUAGE:
            case CEC_OPCODE_GIVE_VENDOR_ID:
            break;
            
            default:
                return (FALSE);
            break;
         }
    }

    switch (MsgType)
    {
        case CEC_MSGTRANS_BROADCAST:
            retVal = isMsgBoardcast & isLenLegal;
        break;
        case CEC_MSGTRANS_DIRECT:
            retVal = (! isMsgBoardcast) & isLenLegal;
        break;
        case CEC_MSGTRANS_BOTH:
            retVal = isLenLegal;
        break;
    }

    return retVal;

}

//**************************************************************************
//  [Function Name]:
//                  hal_CEC_ConfigWakeUp()
//  [Description]:
//                  check RX buffer
//  [Arguments]:
//                  [enMsCEC_SET_INDEX] enSetIndex: current active CEC set index
//  [Return]:
//                  none
//
//**************************************************************************
void hal_CEC_ConfigWakeUp(enMsCEC_SET_INDEX enSetIndex)
{
    WORD clkCnt;
    WORD wOffset = 0;

    if (gStCECInfo[enSetIndex].bIsSetEnabled == FALSE)
        return;
    
    HALCEC_PRINT_MSG("\r\n Here do the PM config cec wakeup \r\n");

    //XBYTE[0x250C] &= ~BIT0;
    //msWriteByte( REG_290C, msReadByte(REG_290C) &(~ BIT0));

    //(1) enable chiptop clk_mcu & clk_pram
    //XBYTE[0x0E00] = 0x03;
    //enable PM_Sleep's clk_mcu and _pram
    //??    MDrv_WriteByte(L_BK_PMSLP(0x00), 0x03);
    //XBYTE[0x0E01] = 0x0F;
    //0x0C; Ken 20080916 for calibration to 1Mz
    //??    MDrv_WriteByte(H_BK_PMSLP(0x00), 0x0F);

    wOffset = _hal_CEC_GetRegOffset(enSetIndex);

    //(2) HDMI CEC settings
    msWriteByteMask(REG_000601 + wOffset, (0x10 | CEC_RETRY_CNT), 0x1F); //[4:3]: 1.5x low bit period [2:0]=CEC_RETRY_CNT
    //[1:0]: tx rising shift 0us, [3:2]: tx falling shift 0us, [4]: cancel current tx = false, [5]: CEC main clock no gating, 
    //[6]: rx test mode:normal, [7]: enable PCM CEC controller
    msWriteByte(REG_000602 + wOffset, 0x80);
    msWriteByte(REG_000603 + wOffset, ((CEC_BUS_FREE_TIME << 4) | CEC_RESEND_INTERVAL)); //[3:0]: cnt1, [7:4]: cnt2
    msWriteByte(REG_000604 + wOffset,  ((CEC_LA_TV << 4) | CEC_FRAME_INTERVAL)); //[3:0]: cnt3, [7:4]: logical address: TV
    
    //Number of counts to achieve 10us
    clkCnt = (CRYSTAL_CLOCK % 100000l)*0.00016 + 0.5;
    msWriteByte(REG_000605 + wOffset, CRYSTAL_CLOCK/1000001); // integer part
    msWriteByteMask(REG_000606 + wOffset, clkCnt, 0x0F); // CEC time unit by Xtal(fractional)

    //(3) PM Sleep: wakeup enable sources
    //??    PM_REG_WRITE(L_BK_PMMCU(0x00),0x01); // reg_cec_enw

    //(4) PM CEC power down controller settings
    msWriteByteMask(REG_000626 + wOffset, 0x1F, 0x1F); // Mask CEC interrupt in standby mode

    msWriteByteMask(REG_000602 + wOffset, 0x00, _BIT7); // [7]: Disable CEC controller
    msWriteByteMask(REG_000602 + wOffset, _BIT7, _BIT7); // [7]: Enable CEC controller

    msWriteByteMask(REG_000607 + wOffset, _BIT4, _BIT4); //[4]: 0: software CEC controller; 1: power down hardware CEC controller;

    //(5) PM CEC wakeup opcode settings
    // OPCODE0: 0x04(Image view on)
    // OPCODE1: 0x0D(Text view on)
    // OPCODE2: 0x44 0x40(Power)
    //          0x44 0x6D(Power ON Function)
    // OPCODE3: N/A
    // OPCODE4: 0x82(Active source) length = 2
    msWriteByte(REG_00060E + wOffset, 0x37); // Enable OP0~2 and OP4
    msWriteByte(REG_00060F + wOffset, 0x24); // Eanble OPCODE2's operand
    msWriteByte(REG_000610 + wOffset, 0x04); // OPCODE0: Image View On
    msWriteByte(REG_000611 + wOffset, 0x0D); // OPCODE1: Text View ON
    msWriteByte(REG_000612 + wOffset, 0x44); // OPCODE2: User control
    msWriteByte(REG_000614 + wOffset, 0x82); // OPCODE4: Active source
    msWriteByte(REG_000617 + wOffset, 0x40); // User control - Power
    msWriteByte(REG_000618 + wOffset, 0x6D); // User control - Power ON Function
    msWriteByte(REG_00061B + wOffset, 0x84); // [2:0]: CEC version 1.3a; [7:3]: OP4 is broadcast message

    //(6) Device(TV) Vendor ID for customer (Big Endian)
    // It depends end-customer's vendor ID
    //MS_DEBUG_MSG(printf("!!!!!!!!!!!!!!!!!!!Change this Vendor ID according to customer!!!!!!!!!!!!!!!!\n"));
    msWriteByte(REG_00061E + wOffset,0x00); // Device Vendor ID
    msWriteByte(REG_00061F + wOffset,0x00); // Device Vendor ID
    msWriteByte(REG_000620 + wOffset,0x00); // Device Vendor ID
    msWriteByte(REG_000621 + wOffset,0x01); // [2:0]: Feature abort reason - "Not in correct mode to respond"

    //(7) Device Physical address: default is 0x00 0x00 0x00
    msWriteByte(REG_00061C + wOffset, 0x00); // Physical address 0.0
    msWriteByte(REG_00061D + wOffset, 0x00); // Physical address 0.0
    msWriteByte(REG_000629 + wOffset, 0x00); // Device type: TV

    //(8) Clear CEC status
    msWriteByte(REG_000622 + wOffset, 0x7F); // Clear CEC wakeup status
    msWriteByte(REG_000625 + wOffset, 0x1F); // Clear RX/TX/RF/LA/NACK status status
    msWriteByte(REG_000625 + wOffset, 0x00);

}


#endif //#if ENABLE_CEC

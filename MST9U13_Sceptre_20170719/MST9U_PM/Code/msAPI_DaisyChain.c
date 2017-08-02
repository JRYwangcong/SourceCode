#define _MSAPI_DAISYCHAIN_C_

#include "board.h"

#if ENABLE_DAISY_CHAIN

#include "types.h"
#include "ms_reg.h"
#include "Ms_rwreg.h"
#include "misc.h"
#include "GPIO_DEF.h"
#include "swi2c_debug.h"
#include "msAPI_DaisyChain.h"

#define ENABLE_DC_CMD_DEBUG	0

#if ENABLE_DEBUG && ENABLE_DC_CMD_DEBUG
#define DC_PRINT_DATA(str, value)	printData(str, value)
#define DC_PRINT_MSG(str) 			printMsg(str)
#else
#define DC_PRINT_DATA(str, value)  	
#define DC_PRINT_MSG(str)	 			
#endif

#define DC_SLAVE_ID 0xB2

/* For Enter/Exit DB MODE */
//use config (0x7F) to keep channel, data bytes and address bytes
#define DC_DEBUG_ENTER()    (sw_i2c_db_enter(DC_SLAVE_ID), sw_i2c_db_other_cmd(DC_SLAVE_ID, 0x7F), sw_i2c_db_acquire_bus(DC_SLAVE_ID, TRUE))
#define DC_DEBUG_EXIT()     (sw_i2c_db_acquire_bus(DC_SLAVE_ID, FALSE), sw_i2c_db_exit(DC_SLAVE_ID))

typedef enum
{
    eDC_CMD_DCOFF            = 0x01, // PM DC off
    eDC_CMD_DPMS             = 0x02, // PM DPMS
    eDC_CMD_WAKEUP           = 0x03, // PM Wakeup
    eDC_CMD_OPENSHORT        = 0x04, // Open Short Test
    eDC_CMD_PATTERNGEN_ON    = 0x05, // Pattern Gen
    eDC_CMD_PATTERNGEN_OFF   = 0x06, // Pattern Gen Off, Normal Rx in
    eDC_CMD_LONG_HPD         = 0x07, // Rx Trigger Long HPD
    eDC_CMD_FORCE_RE_TRAIN   = 0x08, // Force Video Card Re-training
    eDC_CMD_RX_HPD_INIT      = 0x09, // After Host chip read, it should init Epona Rx HPD
    eDC_CMD_SET_SYS_MODE     = 0x0A, // Host chip set system mode
    eDC_CMD_SET_RX_HPD       = 0x0B, // Host control Epona Rx HPD output signal
    eDC_CMD_SET_HOST_FEATURE = 0x0C, // Host pass the chip info. (Host chip info)
    eDC_CMD_MCCS_ACK_READY   = 0x0D, // Host inform the MCCS Read Data Ready
    eDC_CMD_SET_SDM_WORK_PIN = 0x0E, // Host Chip switch DP SDM pin for different DP port
    eDC_CMD_AUTO_SDM_CTL     = 0x0F, // Host Chip enable/disable Epona set SDM_pin as input mode automatically.
    eDC_CMD_SWITCH_DP_INOUT  = 0x11, // Host chip switch in or out DP port
    eDC_CMD_DRR_DPCD_CTL     = 0x12, // Host adjust DP Rx DPCD 0x7 for AMD DRR support.
	eDC_CMD_CHECK_MST_BUSY 	 = 0x13, // Host check Epona is busy or not in MST mode.
	eDC_CMD_GET_SLEEP_SEL	 = 0x14, // Host chip use this command to decide enter fake-sleep or real-sleep	
	eDC_CMD_FORCE_TX1_RETRAIN= 0x15, // Host chip use this command to force Epona do Tx1 re-train
	eDC_CMD_DPRX_GND_SDM	 = 0x16, //Host deliver DP Rx cable GND and SDM(Aux_N) information to Epona
	eDC_CMD_EXTCMD           = 0x7F, // Extension Cmd
    eDC_CMD_INVAILD,
}eDC_CMD;

/* For Communication command retry delay */
#define DC_CMD_RETRY_DELAY  1

/* For DC Communication Address */
#define DCADR_STATUS        0x000460
#define DCADR_REPLYDATA     0x000461
#define DCADR_SENDCMD       0x0003CB
#define DCADR_SENDDATA      0x0003CA

/* For DC Command BIT Info */
#define DCCMD_TRIGGER   BIT7      // Trigger for Command Starting to Process

/* For DC Status BIT Info */
#define DCSTS_ALIVE     BIT7      // EP Alive
#define DCSTS_PM        BIT6      // EP in PM mode
#define DCSTS_TXCABLE   BIT5      // EP TX Cable Plug In
#define DCSTS_RXSYNC    BIT4      // EP RX Signal Ready
#define DCSTS_CMDDONE   BIT3      // EP Process Command Done
#define DCSTS_BUSY      BIT2      // EP Busy (ex: Executing Command)

/* For DC Status Condition Check */
#define IsDC_ALIVE(u8Sts)       (u8Sts & DCSTS_ALIVE)
#define IsDC_PM(u8Sts)          (u8Sts & DCSTS_PM)
#define IsDC_TXCABLE(u8Sts)     (u8Sts & DCSTS_TXCABLE)
#define IsDC_RXSYNC(u8Sts)      (u8Sts & DCSTS_RXSYNC)
#define IsDC_CMDDONE(u8Sts)     (u8Sts & DCSTS_CMDDONE)
#define IsDC_BUSY(u8Sts)        (u8Sts & DCSTS_BUSY)

#if ENABLE_DC_SDM_FORWARD
bit g_SDMCommandFlagForce = 0;
#endif

/* Semaphore for Communication command 
(prevent 51 and R2 send commands at the samt itme) */
#define DCCOMMAND_USE_HW_SEM	0

#if DCCOMMAND_USE_HW_SEM
#define DCCOMMAND_SEM_WAIT_TIME	5 
#define DCCOMMAND_SEMID			0x0A	// use 11th semaphore
#define SEM_HKR2ID        		0x01
#define SEM_PM51ID         		0x02
#define REG_SEM_MAX_NUM        	(13) 	// 14th reserved for RegisterAccess, 15th reserved for WDT, 16th reserved for MailBox
#define REG_SEM_BASE			0x101800

static BOOL SEM_Get_Resource(BYTE u8SemID, WORD u16ResId)
{
	
    if(u8SemID> REG_SEM_MAX_NUM)
        return FALSE;
	
	DC_PRINT_DATA("[DC] g_bR2Status = %d", g_bR2Status);			
	if (g_bR2Status)
	{
		msWriteByte(REG_SEM_BASE + u8SemID*2, u16ResId);
    	return (u16ResId == msReadByte(REG_SEM_BASE + u8SemID*2))? TRUE: FALSE;
	}
	else
	{
		return TRUE;
	}
}

static BOOL SEM_Free_Resource(BYTE u8SemID, WORD u16ResId)
{
    if(u8SemID> REG_SEM_MAX_NUM)
        return FALSE;

	if (g_bR2Status)
	{	
	    if (u16ResId != msReadByte(REG_SEM_BASE + u8SemID*2))
	    {
	        return FALSE;
	    }
	    msWriteByte(REG_SEM_BASE + u8SemID*2, 0x00);
	    return (0x00 == msReadByte(REG_SEM_BASE + u8SemID*2))? TRUE: FALSE;
	}
	else
	{
		return TRUE;
	}
}
#endif

////////////////////////////////////////////
// bret = TRUE ==> Command Process ok
// bret = FALSE ==> Command Process fail
// For msPM_DCCmdHandler()
////////////////////////////////////////////
static BOOL msPM_DCSetCmd( eDC_CMD eCmd, BYTE u8SendData, BYTE* pu8ReplyData, BYTE u8RetryMs )
{
    BOOL bret;
    BYTE u8Status;

    bret  = sw_i2c_db_write_byte(DC_SLAVE_ID, DCADR_SENDDATA, u8SendData);
    bret &= sw_i2c_db_write_byte(DC_SLAVE_ID, DCADR_SENDCMD, eCmd|DCCMD_TRIGGER);

   	if ((eCmd == eDC_CMD_DCOFF) || (eCmd == eDC_CMD_DPMS))
	{		
		do
		{
			ForceDelay1ms(3);
			if (CHIP_ID == MST9U2)
				u8Status = _bit2_(MEM_MSREAD_BYTE(_REG_GPIO_X0_IN));
			else
				u8Status = _bit7_(MEM_MSREAD_BYTE(_REG_GPIO4_IN));
		}while ((u8Status) && (--u8RetryMs));

		if (u8RetryMs!=0)
			*pu8ReplyData = BIT0;
	}
	else if (eCmd == eDC_CMD_WAKEUP)
	{		
		do
		{
			ForceDelay1ms(3);
			if (CHIP_ID == MST9U2)
				u8Status = _bit2_(MEM_MSREAD_BYTE(_REG_GPIO_X0_IN));
			else
				u8Status = _bit7_(MEM_MSREAD_BYTE(_REG_GPIO4_IN));
		}while ((!u8Status) && (--u8RetryMs));

		if (u8RetryMs==0)
			return FALSE;

        u8RetryMs = 50;
        do
        {
              ForceDelay1ms(3);
              bret = sw_i2c_db_read_byte(DC_SLAVE_ID, DCADR_STATUS, &u8Status);
              if(IsDC_CMDDONE(u8Status) && (bret==TRUE))
              {
                  bret = sw_i2c_db_read_byte(DC_SLAVE_ID, DCADR_REPLYDATA, pu8ReplyData);
                  break;
              }
  
       	}while(--u8RetryMs);
	}
    else
    {
        do
        {
            ForceDelay1ms(3);
            bret = sw_i2c_db_read_byte(DC_SLAVE_ID, DCADR_STATUS, &u8Status);
            if(IsDC_CMDDONE(u8Status) && (bret==TRUE))
            {
                bret = sw_i2c_db_read_byte(DC_SLAVE_ID, DCADR_REPLYDATA, pu8ReplyData);
                break;
            }

        }while(--u8RetryMs);
    }

    if(u8RetryMs==0)
    {
        bret = FALSE;
    }
    return bret;
}

////////////////////////////////////////////
// bret = TRUE ==> Command Process ok
// bret = FALSE ==> Command Process fail
// Command retries supposed to be done by Caller
////////////////////////////////////////////
static BOOL msPM_DCCmdHandler( eDC_CMD eCmd, BYTE u8SendData, BYTE* pu8ReplyData )
{
    BOOL bret;
    BYTE u8Status, u8Tmp1, u8Tmp2;

	#if DCCOMMAND_USE_HW_SEM
	WORD u16Cnt = 0;
  	while(FALSE == SEM_Get_Resource(DCCOMMAND_SEMID, SEM_PM51ID))
  	{
  		if (DCCOMMAND_SEM_WAIT_TIME == ++u16Cnt)
		{
			DC_PRINT_MSG("[DC] Can't get HW SEM!\n");
			return FALSE;
		}
        ForceDelay1ms(1);
  	}
	#endif
	
    ////////////////////////////////////////////////////////////////////
    // Common Setting for Daisy Chain I2C Communicate///////////////////////
	/*Don't Change*/DC_DEBUG_ENTER();
    ////////////////////////////////////////////////////////////////////

    // Read DC Status
	bret = sw_i2c_db_read_byte(DC_SLAVE_ID, DCADR_STATUS, &u8Status);
    if( (u8Status & (DCSTS_ALIVE|DCSTS_CMDDONE|DCSTS_BUSY)) != DCSTS_ALIVE)
    {
        DC_PRINT_DATA("[DC] Status: 0x%x\n",u8Status);
		ForceDelay1ms(6);
        bret = FALSE;
    }

    if(bret)
    {
        DC_PRINT_DATA("[DC] Process Cmd: 0x%x\n",eCmd);
        switch(eCmd)
        {
            case eDC_CMD_DCOFF:
            case eDC_CMD_DPRX_GND_SDM:
                bret = msPM_DCSetCmd(eCmd, u8SendData, pu8ReplyData, 50);
                break;

            case eDC_CMD_OPENSHORT:
            case eDC_CMD_EXTCMD:
            default:
                bret = FALSE;
                DC_PRINT_DATA("[DC] Command Not Defined: 0x%x\n",eCmd);
                break;
        }

        // Clear Command
        if(  (sw_i2c_db_write_byte(DC_SLAVE_ID, DCADR_SENDCMD, 0x00)==FALSE) 		// Write 0
           ||(sw_i2c_db_write_byte(DC_SLAVE_ID, DCADR_SENDDATA, 0x00)==FALSE) )
        {
            bret = FALSE;
            DC_PRINT_MSG("[DC] Command Clear Fail.\n");
        }
        else if( (sw_i2c_db_read_byte(DC_SLAVE_ID, DCADR_SENDCMD, &u8Tmp1)==FALSE)	// Read back
               ||(sw_i2c_db_read_byte(DC_SLAVE_ID, DCADR_SENDDATA, &u8Tmp2)==FALSE) )
        {
            bret = FALSE;
            DC_PRINT_MSG("[DC] Command Clear Fail...\n");
        }
        else if( (u8Tmp1!=0) || (u8Tmp2!=0) ) // Read back check 0
        {
            bret = FALSE;
            DC_PRINT_MSG("[DC] Command Clear Fail......\n");
        }
    }

    ///////////////////////////////////////////////////////////////////////
    // Common Setting for Daisy Chain I2C Communicate /////////////////////////
	/*Don't Change*/DC_DEBUG_EXIT();
    //////////////////////////////////////////////////////////////////////

	#if DCCOMMAND_USE_HW_SEM
	bret = SEM_Free_Resource(DCCOMMAND_SEMID, SEM_PM51ID);
	#endif
	
    if(bret)
    {
        DC_PRINT_MSG("[DC] Command Process OK.\n");
    }
    else
    {
        DC_PRINT_MSG("[DC] Command Process NG.\n");
    }

    return bret;
}

void msPM_DCCmdInit()
{
	sw_i2c_db_init();
}

////////////////////////////////////////////
// bret = TRUE ==> Command Process ok
// bret = FALSE ==> Command Process fail
// Command retries supposed to be done by Caller
////////////////////////////////////////////
BOOL msPM_DCSetCmdDcOff( BYTE u8SendData, BYTE* pu8ReplyData )
{
    BOOL bret;

	bret = msPM_DCCmdHandler(eDC_CMD_DCOFF, u8SendData, pu8ReplyData);
    return bret;
}

#if ENABLE_DC_SDM_FORWARD

void msPM_DCSdmForward( void )
{	
    static BOOL g_PreSdmState = FALSE;
    BOOL bCurrentState = FALSE;
    BOOL bResult = FALSE;
    static BYTE u8DisconnectCnt = 0;
    BYTE u8SendData = 0, u8ReplyData = 0;
    
    bCurrentState = IsDC_SDMConnected();
    
    if (g_PreSdmState != bCurrentState ||g_SDMCommandFlagForce)
    {
        g_PreSdmState = bCurrentState;
        if (g_PreSdmState)
        {
            DC_PRINT_MSG("[DC]Cable Connect\n");
            u8SendData = 0x0F;
            u8ReplyData = 0;
            bResult = msPM_DCCmdHandler(eDC_CMD_DPRX_GND_SDM, u8SendData, &u8ReplyData);
            if(!bResult)
                g_PreSdmState = FALSE;
            u8DisconnectCnt = 0x0;
            g_SDMCommandFlagForce = 0;
        }
        else if(++u8DisconnectCnt>=DAISY_CHAIN_SDM_DECOUNCE_TIMES )
        {	
            DC_PRINT_MSG("[DC]Cable Disconnect\n");
            u8SendData = 0x05; 
            u8ReplyData = 0;
            bResult = msPM_DCCmdHandler(eDC_CMD_DPRX_GND_SDM, u8SendData, &u8ReplyData);
            if(!bResult)
                g_PreSdmState = TRUE;
            u8DisconnectCnt = 0x0;
            g_SDMCommandFlagForce = 0;
        }
        else // Counnect-> Disconnect lower than 3 times
        {
            g_PreSdmState = TRUE;      
        }
    }
    return;
}
#endif	// #if ENABLE_DC_SDM_FORWARD

#endif

#undef _MSAPI_DAISYCHAIN_C_


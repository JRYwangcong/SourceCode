#ifndef _ISR_C_
#define _ISR_C_

#include "datatype.h"
#include "Board.h"
#include "Global.h"
#include "Power.h"
#include "MDebug.h"
//#include "debugDef.h"
#include "Common.h"
#include "DDC2Bi.H"
#include "ms_reg.h"
#include "Ms_rwreg.h"
//#include "Userpref.h"
#include "GPIO_DEF.h"
#include "drvADC.h"
#include "Mcu.h"
#include "Isr.h"
#include "MsIR.h"
//#include "halRwreg.h"
#if ENABLE_DP_INPUT
#include "drvDPRxApp.h"
#endif
#include "mapi_combo.h"
#if ENABLE_DAC
#include "drvAudio.h"
#endif
#if ENABLE_DPS
#include "msDPS_Setting.h"
#endif
#include "Keypad.h"
#if UseVsyncINT
#include "msPQ.h"
#endif
extern void msWriteBit_Isr( DWORD u32Reg, Bool bBit, BYTE u8BitPos );
extern void msWrite4ByteMask_Isr( DWORD u32RegLoAddr, DWORD u32Value , DWORD u32Mask );

#if UseINT
void IsrModeChange(BYTE ucSCIndex, DWORD dwStatus)
{
    BYTE u8IdxWin;
    BYTE u8IdxSC;
    WORD u16SCMask;
    BYTE u8PortIndex = MapScaler2Port(ucSCIndex);
    DWORD u32SCIntMute = ( ucSCIndex & _BIT0 )?SC_INT_MUTE_MAIN:SC_INT_MUTE_SUB;

#if 0   // simplay
    if( (dwStatus==0x80 || dwStatus==0x40) && INPUT_IS_TMDS(u8PortIndex)
      && msAPI_combo_IPCheckAVMuteINT((EN_COMBO_IP_SELECT)g_InputPort[u8PortIndex].eIPMux, FALSE) )
    {
        msAPI_combo_IPCheckAVMuteINT((EN_COMBO_IP_SELECT)g_InputPort[u8PortIndex].eIPMux, TRUE);
        return;
    }
#else
    dwStatus = dwStatus;
#endif

#if ENABLE_SW_FLOCK
    if (!SyncLossState(u8PortIndex)) // move before msDrvIP1SetOutputFreerun() for MST9U4
    {
        Set_InputTimingChangeFlag(u8PortIndex);
        SrcFlags(u8PortIndex) |= SyncLoss;
     	 if(g_bAPPWindow_Init&&CB_FuncList[eCB_CustomerSetIsrTimingChangeStatus]!=NULL)
	   ((fpCustomerSetIsrTimingChangeStatus)CB_FuncList[eCB_CustomerSetIsrTimingChangeStatus])(u8PortIndex);		
    }
    for(u8IdxWin=0 ; u8IdxWin<DISPLAY_MAX_NUMS ; u8IdxWin++)
    {
        if ( (g_DisplayWindow[u8IdxWin].eDwStatus == WIN_STATUS_OUTPUT_DISPLAYOK) && (u8PortIndex == MapWin2Port(u8IdxWin)) )
        {
            u16SCMask = g_DisplayWindow[u8IdxWin].u16DwScMaskOut|g_DisplayWindow[u8IdxWin].u16DwScMaskIn;
            u8IdxSC = 0;

            msDrvScMuteScreen(u16SCMask, TRUE);

            while(u16SCMask!=0)
            {
                if(u16SCMask & BIT0) //
                {
                    //printf(" @@@@@@@@@@@@ IsrModeChange==%x \r\n",0);
                    msDrvIP1SetOutputFreerun(TRUE, u8IdxSC);
                }
                u16SCMask >>=1;
                u8IdxSC +=1;
            }
        }
    }

    msWrite4ByteMask_Isr(SC00_28+SCRegBase(ucSCIndex), u32SCIntMute, u32SCIntMute); // int mask
#else

    for(u8IdxWin=0 ; u8IdxWin<DISPLAY_MAX_NUMS ; u8IdxWin++)
    {
        if ( (g_DisplayWindow[u8IdxWin].eDwStatus == WIN_STATUS_OUTPUT_DISPLAYOK) && (u8PortIndex == MapWin2Port(u8IdxWin)) )
        {
            u16SCMask = g_DisplayWindow[u8IdxWin].u16DwScMaskOut|g_DisplayWindow[u8IdxWin].u16DwScMaskIn;
            u8IdxSC = 0;

            while(u16SCMask!=0)
            {
                if(u16SCMask & BIT0) // sub
                {
                    msWriteBit(SC03_04+SCIP1RegBase(u8IdxSC), _ENABLE, _BIT7);
                    msWriteBit(SC10_32+SCRegBase(u8IdxSC), _ENABLE, _BIT5); // output mute
                }
                if(u16SCMask & BIT1) // main
                {
                    msWriteBit(SC01_04+SCIP1RegBase(u8IdxSC+1), _ENABLE, _BIT7);
                    msWriteBit(SC10_32+SCRegBase(u8IdxSC+1), _ENABLE, _BIT1); // output mute
                }
                u16SCMask >>=2;
                u8IdxSC +=2;
            }
        }
    }

    if (!SyncLossState(u8PortIndex))
    {
        Set_InputTimingChangeFlag(u8PortIndex);
        SrcFlags(u8PortIndex) |= SyncLoss;
     	 if(g_bAPPWindow_Init&&CB_FuncList[eCB_CustomerSetIsrTimingChangeStatus]!=NULL)
	   ((fpCustomerSetIsrTimingChangeStatus)CB_FuncList[eCB_CustomerSetIsrTimingChangeStatus])(u8PortIndex);
   }

    msWrite4ByteMask(SC00_28+SCRegBase(ucSCIndex), u32SCIntMute, u32SCIntMute); // int mask



#endif
	
}
#endif

void IsrModeChangeCombo(BYTE eIPMux)   ////20150721
//void IsrModeChangeCombo(BYTE eIPMux)
{
    BYTE    u8SCBankBackupL0,u8SCBankBackupL1,u8SCBankBackupR0,u8SCBankBackupR1;
#if ENABLE_SW_SCALER_COMMON_WRITE
    WORD    u16BackupSWCMWMask;
#else
    BYTE    u8SCBankBackupCWL0,u8SCBankBackupCWL1,u8SCBankBackupCWR0,u8SCBankBackupCWR1;
#endif

    u8SCBankBackupL0 = scReadByte(BANK_SC_M_L0+SC00_00); // Scaler sub bank backup
    u8SCBankBackupL1 = scReadByte(BANK_SC_S_L1+SC00_00);
    u8SCBankBackupR0 = scReadByte(BANK_SC_S_R0+SC00_00);
    u8SCBankBackupR1 = scReadByte(BANK_SC_S_R1+SC00_00);

#if ENABLE_SW_SCALER_COMMON_WRITE
    u16BackupSWCMWMask = g_u16CommonWriteSCMask;
    g_u16CommonWriteSCMask = 0;
#else
    u8SCBankBackupCWL0 = msReadByte(BANK_SC_M_L0+SC00_84); // common write setting backup
    u8SCBankBackupCWL1 = msReadByte(BANK_SC_S_L1+SC00_84);
    u8SCBankBackupCWR0 = msReadByte(BANK_SC_S_R0+SC00_84);
    u8SCBankBackupCWR1 = msReadByte(BANK_SC_S_R1+SC00_84);

    msWriteByteMask(BANK_SC_M_L0+SC00_84, 0x00, 0x0F);
    msWriteByteMask(BANK_SC_S_L1+SC00_84, 0x00, 0x0F);
    msWriteByteMask(BANK_SC_S_R0+SC00_84, 0x00, 0x0F);
    msWriteByteMask(BANK_SC_S_R1+SC00_84, 0x00, 0x0F);
#endif


    IsrModeChange(MapPort2DetScIndex(msDrvSrcMapIPMuxToPort(eIPMux)), 0);

#if ENABLE_SW_SCALER_COMMON_WRITE
    g_u16CommonWriteSCMask = u16BackupSWCMWMask;
#else
    msWriteByteMask(BANK_SC_S_R1+SC00_84, u8SCBankBackupCWR1, 0x0F); // restore SC common write bank
    msWriteByteMask(BANK_SC_S_L1+SC00_84, u8SCBankBackupCWL1, 0x0F); // restore SC common write bank
    msWriteByteMask(BANK_SC_S_R0+SC00_84, u8SCBankBackupCWR0, 0x0F); // restore SC common write bank
    msWriteByteMask(BANK_SC_M_L0+SC00_84, u8SCBankBackupCWL0, 0x0F); // restore SC common write bank
#endif
    scWriteByte(BANK_SC_M_L0+SC00_00, u8SCBankBackupL0); // restore SC sub bank
    scWriteByte(BANK_SC_S_L1+SC00_00, u8SCBankBackupL1); // restore SC sub bank
    scWriteByte(BANK_SC_S_R0+SC00_00, u8SCBankBackupR0); // restore SC sub bank
    scWriteByte(BANK_SC_S_R1+SC00_00, u8SCBankBackupR1); // restore SC sub bank
}

void ms1MsHandler(void)
{
    ms_Counter--;


 if(g_bAPPWindow_Init&&CB_FuncList[eCB_Customer1ms]!=NULL)
    ((fpCustomer1ms)CB_FuncList[eCB_Customer1ms])();


    if( bDelayFlag )
        if( --DelayCounter == 0 )
            bDelayFlag = 0;

    if(TimeOutCounter)
        TimeOutCounter--;

#if PANEL_VBY1()
    if( bVby1CheckLockFlag )
        if( --u8Vby1CheckLockCounter == 0 )
            bVby1CheckLockFlag = 0;
#endif

    if(g_u8KeyDebounceCounter)
        g_u8KeyDebounceCounter--;

    if(g_u8KeyDebounceCounter)
        g_u8KeyDebounceCounter--;


#if ENABLE_TIME_MEASUREMENT
    if(bTimeMeasurementFlag)
    {
        if(--g_u16TimeMeasurementCounter == 0)
            bTimeMeasurementFlag = 0;
    }
#endif

#if ENABLE_DAC
#if EnableDACDePop
    if( bDAC_DePopCountFlag )
    {
        if( --u16AudioDePopCounter == 0 )
        {
            bDAC_DePopCountFlag = 0;
            bAudioDePopFlag = 1;
#if CHIP_ID == MST9U3
            bAudioWakeUpDePopFlag = 1;
#endif
        }
    }
#endif
#endif

    if(!g_bMcuPMClock) // system runs at normal speed
    {
        if( bPanelOffOnFlag ) //091013
            if( --PanelOffOnCounter == 0 )
                bPanelOffOnFlag = 0;

    }


    if(( ms_Counter % 10 ) == 0 )
    {
        Set_ms10Flag();
	
#if (IR_REMOTE_SEL!=NO_REMOTE)
        if( IRkeyDelayCounter)
            IRkeyDelayCounter--;
#endif

        if(( ms_Counter % 20 ) == 0 )
        {
            Set_ms20Flag();
        }
        if(( ms_Counter % 50 ) == 0 )
        {
            Set_ms50Flag();

            if(( ms_Counter % 500 ) == 0 )
            {
                Set_ms500Flag();
            }
        }

        if(( ms_Counter % 200 ) == 0 )
        {
#if ENABLE_VGA_INPUT && _NEW_SOG_DET_
            if(SyncLossState(INPUT_VGA))
                NEW_SOG_DETECT_FLUSH();
#endif
        }

		
    }
	
#if (ZUI)
    if(g_u16ZUIDebounceTime > 0 && g_u16ZUIDebounceTime < ZUI_DEBOUNCE_TIME)
        g_u16ZUIDebounceTime--;
#endif

#if(IR_REMOTE_SEL != NO_REMOTE)
#if(IR_MODE_SEL == IR_SOFTWARE_MODE || IR_MODE_SEL==IR_FULL_POL_NEC_MODE || IR_MODE_SEL==IR_FULL_INT_NEC_MODE)
    if(g_wIRRepeatTimer)
        if((--g_wIRRepeatTimer)==0)
            g_bIRRepeatTimeOut = 1;
#endif
#endif


    if( ms_Counter == 0 )        //
    {
        Set_SecondFlag();
        SET_SYSTEM_AUTO_SAVE_FLAG();
        ms_Counter = SystemTick;
    }



    msAPI_combo_IPTimerIsrHandler();
}

void ISR_R2IRQ_SC(InterruptNum eIntNum, DWORD dwSCRegBase, BYTE ucSCOffset)
{
    BYTE    u8SCBankBackupL0,u8SCBankBackupL1,u8SCBankBackupR0,u8SCBankBackupR1;
#if ENABLE_SW_SCALER_COMMON_WRITE
    WORD    u16BackupSWCMWMask;
#else
    BYTE    u8SCBankBackupCWL0,u8SCBankBackupCWL1,u8SCBankBackupCWR0,u8SCBankBackupCWR1;
#endif
    DWORD   u32FinalStatus;

    ucSCOffset = ucSCOffset;
    u8SCBankBackupL0 = scReadByte(BANK_SC_M_L0+SC00_00); // Scaler sub bank backup
    u8SCBankBackupL1 = scReadByte(BANK_SC_S_L1+SC00_00);
    u8SCBankBackupR0 = scReadByte(BANK_SC_S_R0+SC00_00);
    u8SCBankBackupR1 = scReadByte(BANK_SC_S_R1+SC00_00);
    
#if ENABLE_SW_SCALER_COMMON_WRITE
    u16BackupSWCMWMask = g_u16CommonWriteSCMask;
    g_u16CommonWriteSCMask = 0;
#else
    u8SCBankBackupCWL0 = msReadByte(BANK_SC_M_L0+SC00_84); // common write setting backup
    u8SCBankBackupCWL1 = msReadByte(BANK_SC_S_L1+SC00_84);
    u8SCBankBackupCWR0 = msReadByte(BANK_SC_S_R0+SC00_84);
    u8SCBankBackupCWR1 = msReadByte(BANK_SC_S_R1+SC00_84);

    msWriteByteMask(BANK_SC_M_L0+SC00_84, 0x00, 0x0F);
    msWriteByteMask(BANK_SC_S_L1+SC00_84, 0x00, 0x0F);
    msWriteByteMask(BANK_SC_S_R0+SC00_84, 0x00, 0x0F);
    msWriteByteMask(BANK_SC_S_R1+SC00_84, 0x00, 0x0F);
#endif
    u32FinalStatus = msRead4Byte(dwSCRegBase+SC00_20);

#if ENABLE_SW_FLOCK
    if(u32FinalStatus & SC_INT_ATP)
    {
        msDrvSCCommonWrite ( bSCL0_MAIN | bSCR0_MAIN, ENABLE );
        msWriteBit ( SCL0_0F_AC, TRUE, _BIT3 );
        msWriteBit ( SCL0_0F_AC, FALSE, _BIT3 );
        msWriteBit ( SCL0_0F_AC, TRUE, _BIT0 );
        msWriteBit ( SCL0_0F_AC, FALSE, _BIT0 );
        msDrvSCCommonWrite ( bSCL0_MAIN | bSCR0_MAIN, DISABLE );
    }
#endif	

#if UseINT
    if(u32FinalStatus & SC_INT_MUTE_MAIN)
    {
        IsrModeChange(SCL0_MAIN + ucSCOffset*2, u32FinalStatus & SC_INT_MUTE_MAIN);
    }
    if(u32FinalStatus & SC_INT_MUTE_SUB)
    {
        IsrModeChange(SCL0_SUB + ucSCOffset*2, u32FinalStatus & SC_INT_MUTE_SUB);
    }
#endif

#if UseVsyncINT
    {
        BYTE u8WinIdx;
        if(u32FinalStatus & SC_INT_VSYNC)
        {
            for( u8WinIdx = 0; u8WinIdx<DISPLAY_MAX_NUMS; u8WinIdx++ )
            {
                msPQ_AdaptiveTuning(u8WinIdx);
            }
        }
    }
#endif

#if ENABLE_HDMI_DRR
    if(u32FinalStatus & SC_INT_IP_VSYNC)
    {
        msAPISrcHDMIDRRChk();
    }
#endif

    msWrite4Byte(dwSCRegBase+SC00_24, u32FinalStatus); // int clear

#if ENABLE_SW_SCALER_COMMON_WRITE
    g_u16CommonWriteSCMask = u16BackupSWCMWMask;
#else
    msWriteByteMask(BANK_SC_S_R1+SC00_84, u8SCBankBackupCWR1, 0x0F); // restore SC common write bank
    msWriteByteMask(BANK_SC_S_L1+SC00_84, u8SCBankBackupCWL1, 0x0F); // restore SC common write bank
    msWriteByteMask(BANK_SC_S_R0+SC00_84, u8SCBankBackupCWR0, 0x0F); // restore SC common write bank
    msWriteByteMask(BANK_SC_M_L0+SC00_84, u8SCBankBackupCWL0, 0x0F); // restore SC common write bank
#endif
    scWriteByte(BANK_SC_M_L0+SC00_00, u8SCBankBackupL0); // restore SC sub bank
    scWriteByte(BANK_SC_S_L1+SC00_00, u8SCBankBackupL1); // restore SC sub bank
    scWriteByte(BANK_SC_S_R0+SC00_00, u8SCBankBackupR0); // restore SC sub bank
    scWriteByte(BANK_SC_S_R1+SC00_00, u8SCBankBackupR1); // restore SC sub bank

    MsOS_EnableInterrupt(eIntNum);
}
void ISR_R2IRQ_SC_L0(void) // R2 IRQ #56, source is Scaler L0
{
    ISR_R2IRQ_SC(E_INT_IRQ_SC_L0, BANK_SC_M_L0, 0);
}
void ISR_R2IRQ_SC_L1(void) // R2 IRQ #55, source is Scaler L1
{
    ISR_R2IRQ_SC(E_INT_IRQ_SC_L1, BANK_SC_S_L1, 2);
}
void ISR_R2IRQ_SC_R0(void) // R2 IRQ #54, source is Scaler R0
{
    ISR_R2IRQ_SC(E_INT_IRQ_SC_R0, BANK_SC_S_R0, 1);
}
void ISR_R2IRQ_SC_R1(void) // R2 IRQ #53, source is Scaler R1
{
    ISR_R2IRQ_SC(E_INT_IRQ_SC_R1, BANK_SC_S_R1, 3);
}

void ISR_R2IRQ_PM_IRQ(void) // R2 IRQ #46, source is PM_IRQ
{
    // need backup sub bank if accessed.

    if( INT_STATUS_D2B_ISR() )
    {
        ISR_DDC2Bi();
    }

    msAPI_combo_IPPMIsrHandler();

    MsOS_EnableInterrupt(E_INT_IRQ_PM_IRQ_OUT);
}
void ISR_R2IRQ_PM_FIQ(void)
{
    WORD u16Status = msRead2Byte(REG_002B18);
    // need backup sub bank if accessed.

#if(IR_REMOTE_SEL != NO_REMOTE)
    if(_bit3_(u16Status))
    {
        irInterruptProcess();
    }
#endif

    msWriteByte(REG_002B18, u16Status); // clear int status
    MsOS_EnableInterrupt(E_INT_IRQ_PM_FIQ_OUT);
}
#endif

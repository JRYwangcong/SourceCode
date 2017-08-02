#if 0//nop?
#include <intrins.h>
#endif
#include "datatype.h"
#include "Board.h"
#include "Global.h"
#include "Ms_PM.h"
//#define DelayPeriod ((WORD)CPU_CLOCK_MHZ*78/12)
#include "drvMcu.h"

void _nop_(void)
{
asm __volatile__ ( "l.nop;" );
}
void Delay2us( void )
{
     DelayUs(2);
}
void Delay4us( void )
{
#if 1   //not test yet
    DelayUs(4);
	#if LED_HOT_Menu_Func
	if(I2C_TYPE==S_EDID_VGA)
	{
		DelayUs(11);
	}
	#endif
#else
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();

    if( g_bMcuPMClock )
    {
        BYTE i;
        for( i = 0; i < PM_DELAY4US_LOOP; i++ )
        {
            _nop_();
            _nop_();
            _nop_();
            _nop_();
            _nop_();
            _nop_();
            _nop_();
        }

    }
#endif
}
void ForceDelay1ms( WORD msNums )
{
    if( !msNums )
        return;

    DelayCounter = msNums;
    bDelayFlag = 1;
    while( bDelayFlag )
        {
//printf("\r\n DelayCount: %d",DelayCounter);
        }
}

void Delay1ms( WORD msNums )
{
    if( !msNums )
        return;

    if(g_bAPPWindow_Init&&CB_FuncList[eCB_CustomerDrawOSDAutoStatus]!=NULL)
	 ((fpCustomerDrawOSDAutoStatus)CB_FuncList[eCB_CustomerDrawOSDAutoStatus])();

    DelayCounter = msNums;
    bDelayFlag = 1;
    while( bDelayFlag )
    {
        if( InputTimingChangeFlag(SrcInputPortM/*//tmp*/) )
        {
            DelayCounter = 0;
            bDelayFlag = 0;
            break;
        }
    }
}

void ResetMsCounter(void)
{
    Clr_SecondFlag();
    ms_Counter=SystemTick;
}

void WaitVOutputBlankling(void)
{
    if(!DDCCI_MCCS_CMD_FLAG && bPanelOnFlag) {TimeOutCounter=50;while((msReadByte(SCL0_01_3C)&BIT0)!=0 && (TimeOutCounter));}
}
void WaitVOutputBlanklingStart(void)
{
    if(!DDCCI_MCCS_CMD_FLAG && bPanelOnFlag) {TimeOutCounter=50;while((msReadByte(SCL0_01_3C)&BIT0)==0 && (TimeOutCounter));
                                               TimeOutCounter=50;while((msReadByte(SCL0_01_3C)&BIT0)!=0 && (TimeOutCounter));}
}

void WaitVInputBlankling(BYTE SCDetWin)
{
    BYTE  u8InputVsyncPol = 0; // polarity
    DWORD u32SCIP1RegBase = SCIP1RegBase(SCDetWin);

    if( SCDetWin & _BIT0 )
        u32SCIP1RegBase |= SC01_3C;
    else
        u32SCIP1RegBase |= SC03_3C;

    if(msReadByte(u32SCIP1RegBase+1)& BIT0) // SC01_3D, SC03_3D
    {
        u8InputVsyncPol = BIT2;
    }

    if(!DDCCI_MCCS_CMD_FLAG && bPanelOnFlag) {TimeOutCounter=50;while((msReadByte(u32SCIP1RegBase)&BIT2)!=u8InputVsyncPol && (TimeOutCounter));}
}
void WaitVInputBlanklingStart(BYTE SCDetWin)
{
    BYTE  u8InputVsyncPol = 0; // polarity
    DWORD u32SCIP1RegBase = SCIP1RegBase(SCDetWin);

    if( SCDetWin & _BIT0 )
        u32SCIP1RegBase |= SC01_3C;
    else
        u32SCIP1RegBase |= SC03_3C;

    if(msReadByte(u32SCIP1RegBase+1)& BIT0) // SC01_3D, SC03_3D
    {
        u8InputVsyncPol = BIT2;
    }

    if(!DDCCI_MCCS_CMD_FLAG && bPanelOnFlag) {TimeOutCounter=50;while((msReadByte(u32SCIP1RegBase)&BIT2)==u8InputVsyncPol && (TimeOutCounter));
                                               TimeOutCounter=50;while((msReadByte(u32SCIP1RegBase)&BIT2)!=u8InputVsyncPol && (TimeOutCounter));}
}


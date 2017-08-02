#include <intrins.h>
#include "types.h"
#include "board.h"
#include "global.h"
#include "drvPM.h"
//#define DelayPeriod ((WORD)CPU_CLOCK_MHZ*78/12)
BYTE xdata u8Loop4us = 20;

void Delay4us(void)
{
    WORD i; // Add by Jonson 20100917
#if 1 //asm code better
    i=u8Loop4us;
    while(i--)
    {
        _nop_();
    }
#else
    for(i=0;i<u8Loop4us;i++)
    {
        _nop_();
    }
#endif
}

/*
void Delay4us( void )
{
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();

    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();

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
}
*/
void ForceDelay1ms( WORD msNums )
{
    if( !msNums )
        return;

    DelayCounter = msNums;
    bDelayFlag = 1;
    while( bDelayFlag );
}

void Delay1ms( WORD msNums )
{
    if( !msNums )
        return;

    DelayCounter = msNums;
    bDelayFlag = 1;
    while( bDelayFlag )
    {
    };
}


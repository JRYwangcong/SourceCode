#ifndef _MISC_H
#define _MISC_H
extern void Delay4us( void );
extern void Delay1ms( WORD msNums );
extern void ForceDelay1ms( WORD msNums );
extern BYTE GetVSyncTime( BYTE InputPort );
extern BYTE xdata u8Loop4us;

#endif /// _MISC_H


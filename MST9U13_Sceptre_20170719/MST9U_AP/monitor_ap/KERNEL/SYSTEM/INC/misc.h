#ifndef _MISC_H
#define _MISC_H

extern void _nop_(void);
extern void Delay4us( void );
extern void Delay2us( void );
extern void Delay1ms( WORD msNums );
extern void ForceDelay1ms( WORD msNums );
extern void ResetMsCounter(void);
extern void WaitVOutputBlankling(void);
extern void WaitVOutputBlanklingStart(void);
extern void WaitVInputBlankling(BYTE SCDetWin);
extern void WaitVInputBlanklingStart(BYTE SCDetWin);

#endif

#include "stdio.h"
#include "datatype.h"
#include "risc32.h"
//#include "mhal_arch.h"

static struct
{
    mhal_isr_t  isr;
    U32         data;
} expt_table[MHAL_EXCEPTION_MAX + 1];


void mhal_exception_attach(MHAL_EXCEPTION_TYPE expt_num, mhal_isr_t isr, U32 data)
{
    int old;
    if ((U32)expt_num <= MHAL_EXCEPTION_MAX)
    {
        __mhal_interrupt_disable(old);
        expt_table[expt_num].isr = isr;
        expt_table[expt_num].data = data;
        __mhal_interrupt_restore(old);
    }
}

void mhal_exception_detach(MHAL_EXCEPTION_TYPE expt_num)
{
    if ((U32)expt_num <= MHAL_EXCEPTION_MAX)
    {
        expt_table[expt_num].isr = 0;
    }
}

void mhal_exception_handler(MHAL_SavedRegisters *regs, int vector)
{
    mhal_isr_t isr = 0;

    if ((U32)vector <= MHAL_EXCEPTION_MAX)
    {
        isr = expt_table[vector].isr;
        if (isr)
        {
            isr(regs, expt_table[vector].data);
        }
    }

    if (!isr)
    {
        printf("Exception: %d\n", vector);

        printf("r0 : %08lX  r1 : %08lX  r2 : %08lX  r3 : %08lX\n", regs->r[0],  regs->r[1],  regs->r[2],  regs->r[3]);
        printf("r4 : %08lX  r5 : %08lX  r6 : %08lX  r7 : %08lX\n", regs->r[4],  regs->r[5],  regs->r[6],  regs->r[7]);
        printf("r8 : %08lX  r9 : %08lX  r10: %08lX  r11: %08lX\n", regs->r[8],  regs->r[9],  regs->r[10], regs->r[11]);
        printf("r12: %08lX  r13: %08lX  r14: %08lX  r15: %08lX\n", regs->r[12], regs->r[13], regs->r[14], regs->r[15]);
        printf("r16: %08lX  r17: %08lX  r18: %08lX  r19: %08lX\n", regs->r[16], regs->r[17], regs->r[18], regs->r[19]);
        printf("r20: %08lX  r21: %08lX  r22: %08lX  r23: %08lX\n", regs->r[20], regs->r[21], regs->r[22], regs->r[23]);
        printf("r24: %08lX  r25: %08lX  r26: %08lX  r27: %08lX\n", regs->r[24], regs->r[25], regs->r[26], regs->r[27]);
        printf("r28: %08lX  r29: %08lX  r30: %08lX  r31: %08lX\n", regs->r[28], regs->r[29], regs->r[30], regs->r[31]);

        printf("sr : %08X  pc : %08lX  eear: %08lX\n", regs->sr, regs->pc, regs->eear);

        dump_backtrace();
        while (1);
    }
}

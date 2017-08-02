#include <stdio.h>
#include <assert.h>
#include "datatype.h"
#include "risc32.h"
//#include "mhal_arch.h"
#include "risc32_spr.h"
#include "risc32_util.h"
#include "hwreg_MST9U.h"

#ifndef ASSERT
#define ASSERT(expr)    ((expr) ? (void)0 : __assert(__FILE__, __LINE__, ""))
#endif

#ifdef __AEONR2__
#define AEON_CLOCK_FREQ 12000000
#else
#define AEON_CLOCK_FREQ 12000000
#endif

#if 1   // To fix compile warning and need to check after, 2009/11/23
int mhal_mmu_disable(void);
void mhal_interrupt_restore(int old);
void mhal_interrupt_handler(MHAL_SavedRegisters *regs, int vector);
void mhal_interrupt_dumpvector(void);
extern void dump_backtrace(void);
#endif

struct interrupt
{
    mhal_isr_t  isr;
    U32         data;
};

static void __mhal_default_isr(MHAL_SavedRegisters *regs, U32 vector)
{
    printf("\nmhal_default_isr vector: %ld regs: %p\n", vector, regs);
    dump_backtrace();
    while(1);
}

void mhal_default_isr(MHAL_SavedRegisters *regs, U32 vector) __attribute__ ((weak, alias("__mhal_default_isr")));

static struct interrupt intr_table[MHAL_INTERRUPT_MAX + 1];

static volatile int nr_intr_locks;
static volatile int saved_intr_state;

int  mhal_interrupt_disable(void)
{
    int spr_sr;
    __mhal_interrupt_disable(spr_sr);
    return spr_sr;
}

void mhal_interrupt_enable(void)
{
    __mhal_interrupt_enable();
}

void mhal_interrupt_restore(int old)
{
    __mhal_interrupt_restore(old);
}

void mhal_interrupt_mask_all(void)
{
    mtspr(SPR_PICMR, 0x00000000);
}

void mhal_interrupt_mask(MHAL_INTERRUPT_TYPE intr_num)
{
    __mhal_interrupt_mask(intr_num);
}

void mhal_interrupt_unmask(MHAL_INTERRUPT_TYPE intr_num)
{
    __mhal_interrupt_unmask(intr_num);
}

void mhal_interrupt_lock(void)
{
    int spr_sr;

    __mhal_interrupt_disable(spr_sr);
    if (++nr_intr_locks == 1)
    {
        saved_intr_state = spr_sr;
    }

    ASSERT(nr_intr_locks > 0);
}

void mhal_interrupt_unlock(void)
{
    ASSERT(nr_intr_locks > 0);

    if (--nr_intr_locks == 0)
    {
        __mhal_interrupt_restore(saved_intr_state);
    }
}

void mhal_interrupt_attach(MHAL_INTERRUPT_TYPE intr_num, mhal_isr_t isr, U32 dat)
{
    if (intr_num <= MHAL_INTERRUPT_MAX)
    {
        intr_table[intr_num].isr = isr;
        intr_table[intr_num].data = dat;
    }
}

void mhal_interrupt_detach(MHAL_INTERRUPT_TYPE intr_num)
{
    if (intr_num <= MHAL_INTERRUPT_MAX)
    {
        intr_table[intr_num].isr = 0;
    }
}

#define ISR_TIMER0_PERIOD_US        1000UL  // : 1000us, unit:us, Timer 0 Interrupt period

void mhal_interrupt_handler(MHAL_SavedRegisters *regs, int vector)
{
    unsigned long spr;
    unsigned long picvec;
    mhal_isr_t    isr;

    UNUSED(vector);

    spr = mfspr(SPR_PICSR);
    picvec = __mhal_lsbit_index(spr);
    isr = intr_table[picvec].isr;
    if (isr)
    {
       isr(regs, intr_table[picvec].data);   //call isr here
    }
    else
    {
       mhal_default_isr(regs, picvec);
    }
    if (picvec)
    {
        picvec--;
        spr = mfspr(SPR_PICMR);
        mtspr(SPR_PICMR, spr & ~(1 << picvec));
        mtspr(SPR_PICSR, mfspr(SPR_PICSR) & ~(1 << picvec));
        mtspr(SPR_PICMR, spr);
    }

    if(picvec==0)
    {
        mtspr(SPR_TTMR, 0x60000000 | (R2_CLK/ ISR_TIMER0_PERIOD_US));
    }
}

void mhal_interrupt_dumpvector(void)
{
    unsigned int i;
    for(i=0;i< MHAL_INTERRUPT_MAX;i++)
    {
        printf("vector %d addr:%lX data:%lX\n",i,(U32)intr_table[i].isr,(U32)intr_table[i].data);
    }
}


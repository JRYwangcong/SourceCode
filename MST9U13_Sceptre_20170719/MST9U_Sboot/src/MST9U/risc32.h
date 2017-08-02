#ifndef __RISC32_H__
#define __RISC32_H__

//#ifdef LIBPRANA
#include "risc32_util.h"
//#endif

#include "datatype.h"

typedef enum {
    MHAL_EXCEPTION_BUS_ERROR = 2,
    MHAL_EXCEPTION_DATA_PAGE_FAULT,
    MHAL_EXCEPTION_INSTRUCTION_PAGE_FAULT,
    MHAL_EXCEPTION_TICK_TIMER,
    MHAL_EXCEPTION_UNALIGNED_ACCESS,
    MHAL_EXCEPTION_ILLEGAL_INSTRUCTION,
    MHAL_EXCEPTION_EXTERNAL_INTERRUPT,
    MHAL_EXCEPTION_DTLB_MISS,
    MHAL_EXCEPTION_ITLB_MISS,
    MHAL_EXCEPTION_RANGE,
    MHAL_EXCEPTION_SYSCALL,
    MHAL_EXCEPTION_RESERVED,
    MHAL_EXCEPTION_TRAP,
    MHAL_EXCEPTION_MAX = MHAL_EXCEPTION_TRAP,
} MHAL_EXCEPTION_TYPE;

typedef enum {
    MHAL_INTERRUPT_TICK_TIMER,
    MHAL_INTERRUPT_00 = 1,  // NOTE, PIC interrupt from 1 for handler performance
    MHAL_INTERRUPT_01,
    MHAL_INTERRUPT_02,
    MHAL_INTERRUPT_03,
    MHAL_INTERRUPT_04,
    MHAL_INTERRUPT_05,
    MHAL_INTERRUPT_06,
    MHAL_INTERRUPT_07,
    MHAL_INTERRUPT_08,
    MHAL_INTERRUPT_09,
    MHAL_INTERRUPT_10,
    MHAL_INTERRUPT_11,
    MHAL_INTERRUPT_12,
    MHAL_INTERRUPT_13,
    MHAL_INTERRUPT_14,
    MHAL_INTERRUPT_15,
    MHAL_INTERRUPT_16,
    MHAL_INTERRUPT_17,
    MHAL_INTERRUPT_18,
    MHAL_INTERRUPT_19,
    MHAL_INTERRUPT_20,
    MHAL_INTERRUPT_21,
    MHAL_INTERRUPT_22,
    MHAL_INTERRUPT_23,
    MHAL_INTERRUPT_24,
    MHAL_INTERRUPT_25,
    MHAL_INTERRUPT_26,
    MHAL_INTERRUPT_27,
    MHAL_INTERRUPT_28,
    MHAL_INTERRUPT_29,
    MHAL_INTERRUPT_30,
    MHAL_INTERRUPT_31,
} MHAL_INTERRUPT_TYPE;

// Aliases for interrupt number
#define MHAL_INTERRUPT_FIQ          MHAL_INTERRUPT_02
#define MHAL_INTERRUPT_IRQ          MHAL_INTERRUPT_03
#define MHAL_INTERRUPT_UART         MHAL_INTERRUPT_19
#define MHAL_INTERRUPT_MAX          MHAL_INTERRUPT_31

typedef struct
{
    unsigned long   r1;
    unsigned long   r2;
    unsigned long   r9;
    unsigned long   r10;
    unsigned long   r12;
    unsigned long   r14;
    unsigned long   r16;
    unsigned long   r18;
    unsigned long   r20;
    unsigned long   r22;
    unsigned long   r24;
    unsigned long   r26;
    unsigned long   r28;
    unsigned long   r30;
    unsigned long   sr;
} MHAL_Context;

typedef struct
{
    unsigned long   r[32];          // GPR regs
    unsigned long   machi2;         // AMAC extension
    unsigned long   machi;          // High and low words of
    unsigned long   maclo;          //   multiply/accumulate reg

    // These are only saved for exceptions and interrupts
    int             vector;         // Vector number
    int             sr;             // Status Reg
    unsigned long   pc;             // Program Counter

    // Saved only for exceptions, and not restored when continued:
    // Effective address of instruction/data access that caused exception
    unsigned long   eear;           // Exception effective address reg
} MHAL_SavedRegisters;

typedef void (*mhal_isr_t)(MHAL_SavedRegisters *regs, U32 vector);

MST_BEGIN_DECL

void mhal_exception_attach(MHAL_EXCEPTION_TYPE expt_num, mhal_isr_t isr, U32 dat);
void mhal_exception_detach(MHAL_EXCEPTION_TYPE expt_num);
void mhal_exception_handler(MHAL_SavedRegisters *regs, int vector);

int  mhal_interrupt_disable(void);
void mhal_interrupt_enable(void);
void mhal_interrupt_resotre(int old);

void mhal_interrupt_mask(MHAL_INTERRUPT_TYPE intr_num);
void mhal_interrupt_unmask(MHAL_INTERRUPT_TYPE intr_num);
void mhal_interrupt_mask_all(void);

void mhal_interrupt_lock(void);
void mhal_interrupt_unlock(void);

void mhal_interrupt_attach(MHAL_INTERRUPT_TYPE intr_num, mhal_isr_t isr, U32 dat);
void mhal_interrupt_detach(MHAL_INTERRUPT_TYPE intr_num);

void mhal_stack_init(void *stack);

void mhal_dcache_flush(U32 u32Base, U32 u32Size);

void mhal_switch_context(MHAL_Context *pNextCtx, MHAL_Context **pSaveCtxPtr);
void mhal_load_context(MHAL_Context *pNextCtx);

MST_END_DECL

#endif /* __RISC32_H__ */

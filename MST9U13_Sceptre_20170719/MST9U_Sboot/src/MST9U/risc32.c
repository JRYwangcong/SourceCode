////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2008 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
///////////////////////////////////////////////////////////////////////////////


#include "datatype.h"
#include "risc32_spr.h"
#include "risc32.h"

extern U32 RISC32_DCACHE_SIZE;      // Size of data cache in bytes
extern U32 RISC32_DCACHE_LINE_SIZE; //Bytes in a data cache line
extern U32 RISC32_DCACHE_WAYS;      // Associativity of the cache

#define RISC32_DISABLE_INTERRUPTS(_old_)                    \
    MST_MACRO_START                                         \
    _old_ = mfspr(SPR_SR);                                  \
    mtspr(SPR_SR, _old_ & ~(SPR_SR_IEE|SPR_SR_TEE));        \
    MST_MACRO_END

// Copy interrupt flags from argument into Supervisor Register
#define RISC32_RESTORE_INTERRUPTS(_old_)                    \
    MST_MACRO_START                                         \
    U32 t1,t2;                                              \
    t1 = mfspr(SPR_SR) & ~(SPR_SR_IEE|SPR_SR_TEE);          \
    t2 = (_old_) & (SPR_SR_IEE|SPR_SR_TEE);                 \
    mtspr(SPR_SR, t1 | t2);                                 \
    MST_MACRO_END

// Write dirty cache lines to memory and invalidate the cache entries
// for the given address range.
// Aeon2 does not have an explicit invalidate memory instruction, so use
// flush are necessary.
#if 1   // To fix compile warning and need to check after, 2009/11/23
void RISC32_DCACHE_FLUSH(U32 u32Base, U32 u32Size);
#endif

void RISC32_DCACHE_FLUSH(U32 u32Base, U32 u32Size)
{
    U32 u32SizeOnce = u32Size, u32BaseOnce = u32Base;
    U32 u32Addr, u32End, u32Inten;
    u32End = u32BaseOnce + ((u32SizeOnce < ((U32)(&RISC32_DCACHE_SIZE))) ?
                            u32SizeOnce : ((U32)(&RISC32_DCACHE_SIZE)));
    for(u32Addr = u32End; u32Addr >= u32BaseOnce; u32Addr -= (U32)(&RISC32_DCACHE_LINE_SIZE))
    {
        RISC32_DISABLE_INTERRUPTS(u32Inten);
        __asm__ __volatile__(
            "l.flush_line       0(%0), 0\n" /* way 0 */
            "l.flush_line       0(%0), 1\n" /* way 1 */
            "l.invalidate_line  0(%0), 0\n" /* way 0 */
            "l.invalidate_line  0(%0), 1\n" /* way 1 */
            : : "r"(u32Addr)
            );
        RISC32_RESTORE_INTERRUPTS(u32Inten);
    }
    __asm__ __volatile__( "l.syncwritebuffer\n" );
}

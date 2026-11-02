#ifndef RR_INTERRUPTS_H
#define RR_INTERRUPTS_H

#include "rr/layout.h"

/* Interrupt status and mask register pair at the configured hardware base. */
typedef struct {
    volatile u32 pending;
    volatile u32 mask;
} RrInterruptRegisters;

extern RrInterruptRegisters *D_800ABC58, *D_800ABDAC, *D_800ABD30, *D_800ABCC8;
/* Exchange the interrupt mask; return the previous zero-extended 16-bit value. */
s32 func_8008B4E0(s32 value);
s32 func_80097680(void);
s32 func_8009BAD8(void);
s32 func_8009BB18(void);
s32 func_8009B380(void);

RR_ASSERT_SIZE(RrInterruptRegisters, 8);
RR_ASSERT_OFFSET(RrInterruptRegisters, mask, 4);

#endif

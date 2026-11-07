#ifndef RR_COUNTERS_H
#define RR_COUNTERS_H

#include "rr/layout.h"

/* Three root counters, each occupying a 0x10-byte hardware register window. */
typedef struct {
    volatile u16 count;
    u8 pad02[2];
    volatile u16 mode;
    u8 pad06[2];
    volatile u16 target;
    u8 pad0A[6];
} RrRootCounterRegisters;

extern RrRootCounterRegisters *RCNT_REGS;
/* The selector is its unsigned low halfword; selectors outside 0..2 return 0. */
s32 GetRCnt(u32 selector);

RR_ASSERT_SIZE(RrRootCounterRegisters, 0x10);
RR_ASSERT_OFFSET(RrRootCounterRegisters, mode, 4);
RR_ASSERT_OFFSET(RrRootCounterRegisters, target, 8);

#endif

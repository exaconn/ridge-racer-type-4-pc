#ifndef RR_INTERRUPTS_H
#define RR_INTERRUPTS_H

#include "rr/layout.h"

/* Interrupt status and mask register pair at the configured hardware base. */
typedef struct {
    volatile u32 pending;
    volatile u32 mask;
} RrInterruptRegisters;

extern RrInterruptRegisters *I_STAT_PadVsync, *I_STAT_Card, *I_STAT_Sio, *I_STAT_Pad;
/* Exchange the interrupt mask; return the previous zero-extended 16-bit value. */
s32 SetIntrMask(s32 value);
s32 _padVsyncHookCheck(void);
s32 _cardChkVsyncIrq(void);
s32 _cardChkSioIrq(void);
s32 _sio_irq_check(void);

RR_ASSERT_SIZE(RrInterruptRegisters, 8);
RR_ASSERT_OFFSET(RrInterruptRegisters, mask, 4);

#endif

#ifndef RR_SPU_TICK_H
#define RR_SPU_TICK_H

#include "rr/layout.h"

typedef void (*RrSpuTickCallback)(void);
typedef struct {
    s8 direct, divided, interrupt, reserved;
} RrSpuTickFlags4;
/* Timer state through the three-word D_800A4D4C owner, ending at0x800A4D58. The first two words also have the
 * assembler names D_800A4D38 and D_800A4D3C; callback pointers retain native width. */
typedef struct {
    s32 selector, active;
    RrSpuTickCallback callback, previous;
    RrSpuTickFlags4 flags;
    s32 state[3];
} RrSpuTickState20;

extern RrSpuTickState20 D_800A4D38;
extern s32 D_8010F064;
s32 func_8008BDF8(void);
void func_80084004(s32 mode);

RR_ASSERT_SIZE(RrSpuTickFlags4,4);
RR_ASSERT_SIZE(RrSpuTickState20,0x20);
RR_ASSERT_OFFSET(RrSpuTickState20,active,4);
RR_ASSERT_OFFSET(RrSpuTickState20,callback,8);
RR_ASSERT_OFFSET(RrSpuTickState20,previous,0xC);
RR_ASSERT_OFFSET(RrSpuTickState20,flags,0x10);
RR_ASSERT_OFFSET(RrSpuTickState20,state,0x14);

#endif

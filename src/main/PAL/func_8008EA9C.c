#include "rr/library.h"
#include "rr/cd.h"

extern RrLibTransferState38 D_800A67B0;
extern u32 D_800A63F4;
extern void func_8008C12C(RrLibCompletionCallback);
extern void func_8008C55C(RrLibCompletionCallback);
extern void func_8008C118(RrLibCompletionCallback);
extern void func_8008E7F4(void);

void func_8008EA9C(void) {
    register volatile RrLibTransferState38 *state = &D_800A67B0;
    /* ASM_BARRIER: Keep the complete state address in its saved register. */
    __asm__("" : "=r"(state) : "0"(state));
    state->position08 += state->step10 << 2;
    state->remaining14 -= 1;
    state->completed20 += 1;
    if (state->remaining14 == 0) {
        u8 *page;
        RrLibNotificationCallback callback;
        func_8008C12C(state->restore2C);
        if (D_800A63F4 & 1) func_8008C55C(state->restore30);
        func_8008C118(func_8008E7F4);
        func_8008C27C(9, 0);
        /* PAL mapping: page + 0x63F0 is &D_800A63F0. */
        page = (u8 *)0x800A0000U;
        callback = *(RrLibNotificationCallback *)(page + 0x63F0);
        /* Ordinary state publication permits the original branch-delay store. */
        ((RrLibTransferState38 *)state)->finished24 = 1;
        if (callback != 0) callback(2, state->notify_arg34);
    }
}

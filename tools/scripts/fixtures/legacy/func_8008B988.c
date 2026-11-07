/* CC1_FLAGS: -mno-split-addresses */
#include "rr/library.h"

extern RrLibSavedState D_800A52C8;
extern u16 *D_800A6358;
extern s32 *D_800A635C;
extern void HookEntryInt(void *data);
extern void ExitCriticalSection(void);

RrLibSavedState *restartIntr(void) {
    RrLibSavedState *state = &D_800A52C8;
    if (state->active != 0) return 0;
    HookEntryInt(state->callback_data);
    state->active = 1;
    *D_800A6358 = state->register16;
    {
        s32 *destination = D_800A635C;
        s32 value = state->register32;
        /* ASM_BARRIER: retain the original load-delay nop before the call/store pair. */
        __asm__ volatile("nop");
        *destination = value;
    }
    ExitCriticalSection();
    return state;
}

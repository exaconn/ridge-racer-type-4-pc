#include "rr/library.h"
#include "rr/cd.h"

extern RrLibTransferState38 CdReadState;
extern u32 CD_read_dma_mode;
extern void CdReadyCallback(RrLibCompletionCallback);
extern void CdDataCallback(RrLibCompletionCallback);
extern void CdSyncCallback(RrLibCompletionCallback);
extern void cd_read_restore_sync(void);

void cb_data(void) {
    register volatile RrLibTransferState38 *state = &CdReadState;
    /* ASM_BARRIER: Keep the complete state address in its saved register. */
    __asm__("" : "=r"(state) : "0"(state));
    state->position08 += state->step10 << 2;
    state->remaining14 -= 1;
    state->completed20 += 1;
    if (state->remaining14 == 0) {
        u8 *page;
        RrLibNotificationCallback callback;
        CdReadyCallback(state->restore2C);
        if (CD_read_dma_mode & 1) CdDataCallback(state->restore30);
        CdSyncCallback(cd_read_restore_sync);
        CdControlF(9, 0);
        /* PAL mapping: page + 0x63F0 is &CD_cbread. */
        page = (u8 *)0x800A0000U;
        callback = *(RrLibNotificationCallback *)(page + 0x63F0);
        /* Ordinary state publication permits the original branch-delay store. */
        ((RrLibTransferState38 *)state)->finished24 = 1;
        if (callback != 0) callback(2, state->notify_arg34);
    }
}

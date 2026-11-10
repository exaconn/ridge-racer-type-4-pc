#include "rr/library.h"
#include "rr/cd.h"

extern LibTransferState38 CdReadState;
extern u32 CD_read_dma_mode;
extern void CdReadyCallback(LibCompletionCallback);
extern void CdDataCallback(LibCompletionCallback);
extern void CdSyncCallback(LibCompletionCallback);
extern void cd_read_restore_sync(void);

void cb_data(void) {
    register volatile LibTransferState38 *state = &CdReadState;
    /* ASM_BARRIER: Keep the complete state address in its saved register. */
    __asm__("" : "=r"(state) : "0"(state));
    state->position08 += state->step10 << 2;
    state->remaining14 -= 1;
    state->completed20 += 1;
    if (state->remaining14 == 0) {
        u8 *page;
        LibNotificationCallback callback;
        CdReadyCallback(state->restore2C);
        if (CD_read_dma_mode & 1) CdDataCallback(state->restore30);
        CdSyncCallback(cd_read_restore_sync);
        CdControlF(9, 0);
        /* PAL mapping: page + 0x63F0 is &CD_cbread. */
        page = (u8 *)0x800A0000U;
        callback = *(LibNotificationCallback *)(page + 0x63F0);
        /* Ordinary state publication permits the original branch-delay store. */
        ((LibTransferState38 *)state)->finished24 = 1;
        if (callback != 0) callback(2, state->notify_arg34);
    }
}

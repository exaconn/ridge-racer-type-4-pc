/* CC1_FLAGS: -mno-split-addresses */
#include "psyq/libpad/padintr.h"
#include "rr/channels.h"
#include "psyq/libpad/padenab.h"

int PadEnableCom(int a0);
void _padSetVsyncParam();

int PadEnableCom(int a0) {
    int s1;
    /* ASM_BARRIER: pins p to s2 to match original register allocation */
    register int *p __asm__("$18");
    int *q;
    int t1, t2;
    s1 = (PadLastChannel * 2) | (PadFirstChannel == 0);
    if (s1 != a0) {
        PadComActive = 0;
        if (a0 & 1) {
            p = PadPortTimeoutCounts;
            t1 = *p;
            PadFirstChannel = 0;
            if (t1 >= 0x96) {
                PadResetChannelFn(PadChannelRecords);
            }
            *p = 0;
        } else {
            PadFirstChannel = 1;
        }
        if (a0 & 2) {
            q = PadPortTimeoutCounts+1;
            PadLastChannel = 1;
            if (*q >= 0x96) {
                PadResetChannelFn(PadChannelRecords + 1);
            }
            *q = 0;
        } else {
            PadLastChannel = 0;
        }
        PadComActive = 1;
    }
    return s1;
}

void _padSetVsyncParam(void) {
    PadInterruptHandler *callbacks=PadIntQueueEntry.callbacks04;
    callbacks[PAD_INTERRUPT_HANDLER]=_padIntPad;
    callbacks[PAD_INTERRUPT_TEST]=_padIsVsync;
    PadIntQueueEntry.next00=0;
    PadIntQueueEntry.reserved0C=0;
}

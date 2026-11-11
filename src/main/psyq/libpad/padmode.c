#include "psyq/libpad/padintr.h"
#include "rr/channels.h"
#include "psyq/libpad/padenab.h"

int PadEnableCom(int a0);
void _padSetVsyncParam();

int PadEnableCom(int a0) {
    int s1;
    s1 = (PadLastChannel * 2) | (PadFirstChannel == 0);
    if (s1 == a0) {
        return s1;
    }
    PadComActive = 0;
    if (a0 & 1) {
        PadFirstChannel = 0;
        if (PadPortTimeoutCounts[0] >= 0x96) {
            PadResetChannelFn(PadChannelRecords);
        }
        PadPortTimeoutCounts[0] = 0;
    } else {
        PadFirstChannel = 1;
    }
    if (a0 & 2) {
        PadLastChannel = 1;
        if (PadPortTimeoutCounts[1] >= 0x96) {
            PadResetChannelFn(PadChannelRecords + 1);
        }
        PadPortTimeoutCounts[1] = 0;
    } else {
        PadLastChannel = 0;
    }
    PadComActive = 1;
    return s1;
}

void _padSetVsyncParam(void) {
    PadInterruptHandler *callbacks=PadIntQueueEntry.callbacks04;
    callbacks[PAD_INTERRUPT_HANDLER]=_padIntPad;
    callbacks[PAD_INTERRUPT_TEST]=_padIsVsync;
    PadIntQueueEntry.next00=0;
    PadIntQueueEntry.reserved0C=0;
}

#include "game/data/resinit.h"
#include "game/data/cdfile2.h"

void ResetCarSlotBuffers(void) {
    int i;
    for (i = 0; i < 16; i++) {
        CarSlotBuffers[i].mainCount0C = 0;
        CarSlotBuffers[i].secondCount34 = 0;
        CarSlotBuffers[i].phase5C = 0;
        CarSlotBuffers[i].historyCount68 = 0;
    }
}

/* Initial values of the globals this module owns. */
CarSlotBufferC8 CarSlotBuffers[16];

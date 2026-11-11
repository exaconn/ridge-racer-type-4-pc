#include "rr/library.h"
#include "psyq/libcd/dsmedia2.h"
void StSetRing(LibQueueRecord32 *first, s32 second) {
    StRingHeaders = first;
    StRingSize = second;
    StClearRing();
}

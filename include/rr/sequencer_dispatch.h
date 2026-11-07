#ifndef RR_SEQUENCER_DISPATCH_H
#define RR_SEQUENCER_DISPATCH_H
#include "rr/sequencer_init.h"

extern s32 _snd_ev_flag;

/* Lock1 skips dispatch. Otherwise publish lock1, run the initial SPU callback,
 * then process enabled banks and each record's current flags in observed order.
 * Counts, mask, table entries and flags can change in callbacks. Every bank
 * reached must be below32 and every reached record accessible; nonpositive
 * dimensions skip their loops. Flag0x4's handler is followed by a fresh-record
 * flag clear. Unlock is unconditional after entered dispatch. */
void SsSeqCalledTbyT(void);
#endif

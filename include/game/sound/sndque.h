#ifndef RR_GAME_SOUND_SNDQUE_H
#define RR_GAME_SOUND_SNDQUE_H

#include "types.h"

/* The table ID is in [0,24]. Lists end with a nonpositive halfword or use the
 * pointer sentinel -1. Initialization/restart accepts at most seven programs;
 * append starts with a count in [0,7] and rejects a total of eight or more.
 * Failure returns pointer sentinel -1. Success preserves the existing ABI
 * result (SoundState byte base + 2*new_count); all known callers ignore it. */
void *SoundQueueEnqueue(s32 id, s32 restart);

#endif

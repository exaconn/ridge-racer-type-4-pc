#ifndef RR_SOUND_SEQUENCE_TYPES_H
#define RR_SOUND_SEQUENCE_TYPES_H

#include "rr/layout.h"

enum {RR_SOUND_SEQUENCE_CAPACITY = 8};

/* Shared by enqueue, stereo voice playback and clear. Programs use bank24[4]
 * and voices 4/5. The first nonpositive input ends an enqueue list; the stored
 * queue ends with -1. unknown0C is cleared with the read index; its purpose
 * has not been established. */
typedef struct {
    s32 active;
    s32 count;
    s32 readIndex;
    s32 unknown0C;
    s32 startImmediately;
    s16 leftVolume, rightVolume;
    s16 programs[RR_SOUND_SEQUENCE_CAPACITY];
} SoundSequenceState;

RR_ASSERT_SIZE(SoundSequenceState, 0x28);
RR_ASSERT_OFFSET(SoundSequenceState, count, 4);
RR_ASSERT_OFFSET(SoundSequenceState, readIndex, 8);
RR_ASSERT_OFFSET(SoundSequenceState, startImmediately, 0x10);
RR_ASSERT_OFFSET(SoundSequenceState, programs, 0x18);

#endif

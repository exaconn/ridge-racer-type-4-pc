#ifndef RR_SEQUENCE_QUEUE_H
#define RR_SEQUENCE_QUEUE_H

#include "rr/layout.h"
#include "game/sound/sndque.h"
#include "rr/audio_state_types.h"

extern AudioState SoundState;
/* Existing SoundSequenceTable..PanGainTable storage:25 two-pointer rows. */
enum {RR_SEQUENCE_TABLE_COUNT=25};
extern s16 *SoundSequenceTable[RR_SEQUENCE_TABLE_COUNT][2];
extern s32 SoundSeqVariantIndex;

typedef char rr_sequence_count_offset[
    (RR_FIELD_OFFSET(AudioState, sequence19D0) +
     RR_FIELD_OFFSET(SoundSequenceState, count) == 0x19D4) ? 1 : -1];
typedef char rr_sequence_programs_offset[
    (RR_FIELD_OFFSET(AudioState, sequence19D0) +
     RR_FIELD_OFFSET(SoundSequenceState, programs) == 0x19E8) ? 1 : -1];

#endif

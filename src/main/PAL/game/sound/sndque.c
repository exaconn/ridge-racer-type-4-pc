#include "game/sound/sndque.h"
#include "rr/sequence_queue.h"

static __inline__ void ResetQueuedSoundPlayback(AudioState *state, s32 restart) {
    state->sequence19D0.readIndex = 0;
    state->sequence19D0.unknown0C = 0;
    state->sequence19D0.startImmediately = restart;
    state->sequence19D0.leftVolume = 80;
    state->sequence19D0.rightVolume = 80;
}

void *SoundQueueEnqueue(s32 id, s32 restart) {
    s32 incomingCount = 0, programCount = 0, copiedCount;
    const s16 *input, *scan;
    /* ASM_BARRIER: keep the final address in the ABI return register. */
    register u8 *result __asm__("$2");
    AudioState *state;
    AudioState *updatedState;
    if (SoundSeqVariantIndex == 0) {
        input = SoundSequenceTable[id][0];
    } else {
        input = SoundSequenceTable[id][1];
    }
    if (input == (s16 *)-1) {
        return (void *)-1;
    }
    state = &SoundState;
    if (state->sequence19D0.active == 0) {
        state->sequence19D0.active = 1;
        ResetQueuedSoundPlayback(state, restart);
    } else if (restart) {
        ResetQueuedSoundPlayback(state, restart);
    } else {
        programCount = state->sequence19D0.count;
        if (*input > 0) {
            scan = input;
            do {
                scan++;
                incomingCount++;
            } while (*scan > 0);
        }
        if (programCount + incomingCount >= RR_SOUND_SEQUENCE_CAPACITY) {
            return (void *)-1;
        }
    }
    copiedCount = 0;
    if (*input > 0) {
        do {
            u16 program = *input++;
            copiedCount++;
            SoundState.sequence19D0.programs[programCount] = program;
            programCount++;
        } while (*input > 0);
    }
    updatedState = &SoundState;
    /* Preserve the existing base-plus-count result with a native byte pointer. */
    result = (u8 *)updatedState + programCount * sizeof(updatedState->sequence19D0.programs[0]);
    updatedState->sequence19D0.programs[programCount] = -1;
    updatedState->sequence19D0.count = programCount;
    if (copiedCount == 0) {
        updatedState->sequence19D0.active = 0;
    }
    return result;
}

#include "game/sound/sndque.h"
#include "rr/sequence_queue.h"
#include <stdint.h>

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
    AudioState *state;
    AudioState *updatedState;
    if (SoundState.sequenceVariant19CC == 0) {
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
    updatedState->sequence19D0.programs[programCount] = -1;
    updatedState->sequence19D0.count = programCount;
    if (copiedCount == 0) {
        updatedState->sequence19D0.active = 0;
    }
#ifndef __mips__
    return (u8 *)updatedState + programCount * sizeof(updatedState->sequence19D0.programs[0]);
#endif
}

/* Initial values of the globals this module owns. */
short D_800A02C4[2] = {1, -1};
short D_800A02C8[2] = {2, -1};
short D_800A02CC[2] = {3, -1};
short D_800A02D0[2] = {4, -1};
short D_800A02D4[4] = {5, 6, -1};
short D_800A02DC[4] = {7, 11, -1};
short D_800A02E4[4] = {8, 11, -1};
short D_800A02EC[4] = {9, 11, -1};
short D_800A02F4[4] = {10, 11, -1};
short D_800A02FC[2] = {12, -1};
short D_800A0300[2] = {13, -1};
short D_800A0304[2] = {14, -1};
short D_800A0308[2] = {15, -1};
short D_800A030C[2] = {16, -1};
short D_800A0310[2] = {17, -1};
short D_800A0314[4] = {18, 19, -1};
short D_800A031C[2] = {18, -1};
short D_800A0320[2] = {21, -1};
short D_800A0324[2] = {22, -1};
short D_800A0328[2] = {23, -1};
short D_800A032C[2] = {24, -1};
short D_800A0330[2] = {5, -1};
short D_800A0334[2] = {6, -1};
short D_800A0338[2] = {5, -1};
short D_800A033C[4] = {6, 10, -1};
short D_800A0344[4] = {7, 10, -1};
short D_800A034C[4] = {8, 10, -1};
short D_800A0354[4] = {9, 10, -1};
short D_800A035C[2] = {11, -1};
short D_800A0360[2] = {12, -1};
short D_800A0364[2] = {13, -1};
short D_800A0368[2] = {14, -1};
short D_800A036C[2] = {15, -1};
short D_800A0370[2] = {16, -1};
s16 *SoundSequenceTable[RR_SEQUENCE_TABLE_COUNT][2] = {{(short *)D_800A02C4, (short *)D_800A02C4}, {(short *)D_800A02C8, (short *)D_800A02C8}, {(short *)D_800A02CC, (short *)D_800A02CC}, {(short *)D_800A02D0, (short *)D_800A02D0}, {(short *)D_800A02D4, (short *)D_800A0338}, {(short *)D_800A02F4, (short *)D_800A0354}, {(short *)D_800A02EC, (short *)D_800A034C}, {(short *)D_800A02E4, (short *)D_800A0344}, {(short *)D_800A02DC, (short *)D_800A033C}, {(short *)D_800A02FC, (short *)D_800A035C}, {(short *)D_800A0300, (short *)(intptr_t)-1}, {(short *)D_800A0304, (short *)(intptr_t)-1}, {(short *)D_800A0308, (short *)(intptr_t)-1}, {(short *)D_800A030C, (short *)(intptr_t)-1}, {(short *)D_800A0310, (short *)(intptr_t)-1}, {(short *)D_800A0314, (short *)D_800A0360}, {(short *)D_800A031C, (short *)(intptr_t)-1}, {(short *)D_800A0320, (short *)(intptr_t)-1}, {(short *)D_800A0324, (short *)D_800A0364}, {(short *)D_800A0328, (short *)D_800A0368}, {(short *)D_800A032C, (short *)(intptr_t)-1}, {(short *)(intptr_t)-1, (short *)D_800A036C}, {(short *)(intptr_t)-1, (short *)D_800A0370}, {(short *)D_800A0330, (short *)D_800A0330}, {(short *)D_800A0334, (short *)(intptr_t)-1}};

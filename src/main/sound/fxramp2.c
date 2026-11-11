#include "types.h"
#include "game/sound/fxramp2.h"
#include "rr/audio_bank_setup.h"

void SeqMarkCallback(s16 a0, s16 a1, s16 a2) {
    AudioState *p = &SoundState;
    int v = a2;
    p->seqMark2880 = v;
    if (v < 100U) {
        p->seqBeat2874++;
        if (p->seqBeat2874 > 0x61A7F) p->seqBeat2874 = 0;
    }
}

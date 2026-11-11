#include "game/sound/fxplay.h"
#include "rr/audio_state_types.h"
#include "game/sound/fxramp1.h"

#include "game/sound/cdmain.h"

void SeqFadeStep(void)
{
    int d = SoundState.seqFadeLen288A;
    int t;
    int v;
    int from;
    unsigned c;
    int to;
    if (d > 0) {
        c = *(u16 *)&SoundState.seqFadeTick288C;
        if (SoundState.seqFadeTick288C < d) {
            t = c + 1;
            to = SoundState.seqFadeTarget288E;
            from = SoundState.seqFadeStart2890;
            v = (to - from) << 12;
            v = (v / d) * ((short) t);
            SoundState.seqFadeTick288C = t;
            if (v < 0) {
                v += 0xFFF;
            }
            v = (v >> 12) + from;
            SeqSetVolume((short)v);
            return;
        }
        SeqMuteVolume();
    }
}

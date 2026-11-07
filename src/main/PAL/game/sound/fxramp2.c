#include "types.h"
#include "game/sound/fxramp2.h"
#include "rr/audio_bank_setup.h"
struct S { char pad[0x2874]; u32 a; char p[8]; s32 b; };
void SeqMarkCallback(int a0, int a1, s16 a2) {
    struct S *p = &SoundState;
    int v = a2;
    p->b = v;
    if ((u32)v < 100) {
        p->a++;
        if (p->a > 0x61A7F) p->a = 0;
    }
}

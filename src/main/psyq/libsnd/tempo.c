#include "rr/sequencer_tempo.h"
void _SsGetMetaEvent(s32 raw_bank, s32 raw_row) {
    s16 a0 = raw_bank;
    s16 a1 = raw_row;
    LibStateB0 *c = &_ss_score[a0][a1];
    u32 lo, m, v0;
    u8 *p = c->c0;
    s32 b0, b1, b2;
    b0 = *p++;
    c->c0 = p;
    b1 = *p;
    c->c0 = p + 1;
    b2 = p[1];
    lo = 0x03938700 / (b2 | ((b0 << 16) | (b1 << 8)));
    m = c->h48[4] * lo;
    c->c0 = p + 2;
    c->w94 = lo;
    if (m * 10 < VBLANK_MINUS * 60) {
        v0 = (VBLANK_MINUS * 600) / m;
        c->h48[5] = v0;
        c->h48[6] = v0;
    } else {
        c->h48[5] = -1;
        c->h48[6] = ((u32)(s32)c->h48[4] * c->w94 * 10) / (VBLANK_MINUS * 60);
        if ((VBLANK_MINUS * 30) < ((u32)(s32)c->h48[4] * c->w94 * 10) % (VBLANK_MINUS * 60)) {
            c->h48[6]++;
        }
    }
    c->w90 = _SsReadDeltaValue(a0, a1);
}

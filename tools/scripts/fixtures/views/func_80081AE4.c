/* MASPSX_FLAGS: --assembler-profile=gnu */

/* CC1_FLAGS: -mno-split-addresses */

#include "rr/sequencer_tempo.h"

void _SsGetMetaEvent(s32 raw_bank, s32 raw_row) {
    s16 a0 = raw_bank;
    s16 a1 = raw_row;
    register LibStateB0 **b = _ss_score;
    LibStateB0 **q = b+a0;
    LibStateB0 *c = *q+a1;
    unsigned int a2, v0, lo, m;
    /* ASM_BARRIER: Preserve overlapping arithmetic lifetimes. */
    register u32 a3 asm("$7");
    u32 t0;
    /* ASM_BARRIER: Preserve the frequency intermediate register. */
    register u32 tt asm("$3");
    unsigned char *p;
    int b0, b1, b2;
    p = c->c0;
    b0 = *p++;
    c->c0 = p;
    b1 = *p;
    c->c0 = p + 1;
    b2 = p[1];
    lo = 0x03938700 / (b2 | ((b0 << 16) | (b1 << 8)));
    m = c->h48[4] * lo;
    a2 = VBLANK_MINUS;
    c->c0 = p + 2;
    c->w94 = lo;
    /* ASM_BARRIER: Keep the tempo store before subsequent arithmetic. */
    __asm__("" : "+m"(((u8*)&c->w94)[0]));
    tt = a2 << 4;
    t0 = tt - a2;
    a3 = a2 * 0x3C;
    if (m * 0xA < a3) {
        v0 = (a2 * 0x258) / m;
        c->h48[5] = v0;
        c->h48[6] = v0;
    } else {
        a2 = (unsigned)((u32)(s32)c->h48[4] * (u32)c->w94 * 0xA) / a3;
        c->h48[5] = -1;
        c->h48[6] = a2;
        if ((unsigned)(t0 * 2) < (unsigned)((u32)(s32)c->h48[4] * (u32)c->w94 * 0xA) % a3)
            c->h48[6] = a2 + 1;
    }
    c->w90 = _SsReadDeltaValue(a0, a1);
}

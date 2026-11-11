#include "psyq/libsnd/ssatr2.h"
#include "psyq/libsnd/sspause2.h"
#include "rr/sequencer_init.h"

void _SsSndSetReplayMode(int a0, short a1);
void SsSepReplay(short a, short b);
void Snd_SetPlayMode(s32 a0, s32 a1_in, s32 a2, s32 a3_in);

void _SsSndSetReplayMode(int a0, short a1) {
    u32 bankBits = (u32)a0 << 16;
    LibStateB0 **table = _ss_score;
    LibStateB0 **q = &table[(s32)bankBits >> 16];
    LibStateB0 *p = &(*q)[(s16)a1];
    int f = p->w98;
    if (!(f & 0x204)) {
        if (!(f & 0x100)) {
            p->w98 = f & ~2;
            (*q)[(s16)a1].w98 |= 8;
            (*q)[(s16)a1].w98 |= 1;
        }
    }
}

void SsSeqReplay(short a) { _SsSndSetReplayMode(a, 0); }

void SsSepReplay(short a, short b) { _SsSndSetReplayMode(a, b); }

void Snd_SetPlayMode(s32 a0, s32 a1_in, s32 a2, s32 a3_in) {
    short a1 = a1_in;
    signed char a3 = a3_in;
    u32 bankBits = (u32)a0 << 16;
    LibStateB0 **table = _ss_score;
    LibStateB0 **q = &table[(s32)bankBits >> 16];
    LibStateB0 *p = &(*q)[(s16)a1];
    a2 = (signed char)a2;
    p->c0 = p->c4;
    p->c8 = p->c4;
    p->cC = p->c4;
    (*q)[(s16)a1].w98 &= ~0x200;
    (*q)[(s16)a1].w98 &= ~4;
    p->b20 = a3;
    if (a2 == 1) {
        (*q)[(s16)a1].w98 |= 1;
        p->b14 = a2;
        p->b21 = 0;
        _SsVmSetSeqVol((short)(a0 | ((u32)a1 << 8)), p->h58[0], p->h58[1], 1);
    } else if (a2 == 0) {
        (*q)[(s16)a1].w98 |= 2;
    }
}


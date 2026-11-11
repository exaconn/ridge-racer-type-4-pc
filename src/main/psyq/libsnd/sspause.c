#include "psyq/libsnd/sspause2.h"
#include "psyq/libsnd/vmchval.h"
#include "psyq/libsnd/sspause.h"
#include "rr/sequencer_init.h"

void _SsSndSetPauseMode(int a0, short a1);
void SsSepPause(short a, short b);
void SsSeqPlay(short a, signed char b, short c);
void SsSepPlay(short a, short b, signed char c, short d);

void _SsSndSetPauseMode(int a0, short a1) {
    u32 bankBits = (u32)a0 << 16;
    LibStateB0 **table = _ss_score;
    LibStateB0 **q = &table[(s32)bankBits >> 16];
    LibStateB0 *p = &(*q)[(s16)a1];
    _SsVmGetSeqVol((short)(a0 | ((u32)a1 << 8)), &p->h58[2], &p->h58[3]);
    (*q)[(s16)a1].w98 &= ~1;
    (*q)[(s16)a1].w98 &= ~8;
    (*q)[(s16)a1].w98 |= 2;
}

void SsSeqPause(short a) { _SsSndSetPauseMode(a, 0); }

void SsSepPause(short a, short b) { _SsSndSetPauseMode(a, b); }

void SsSeqPlay(short a, signed char b, short c) { Snd_SetPlayMode(a, 0, b, c); }

void SsSepPlay(short a, short b, signed char c, short d) { Snd_SetPlayMode(a, b, c, d); }

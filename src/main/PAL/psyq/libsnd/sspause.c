/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "psyq/libsnd/sspause2.h"
#include "psyq/libsnd/vmchval.h"
#include "psyq/libsnd/sspause.h"
#include "rr/sequencer_init.h"

void _SsSndSetPauseMode(int a0, short a1);
void SsSepPause(short a, short b);
void SsSeqPlay(short a, signed char b, short c);
void SsSepPlay(short a, short b, signed char c, short d);

void _SsSndSetPauseMode(int a0, short a1) {
    int t = a0 << 16;
    LibStateB0 **b = _ss_score;
    LibStateB0 **q = (LibStateB0 **)((int)b + (t >> 14));
    int o = a1 * 0xB0;
    LibStateB0 *p = (LibStateB0 *)((char *)*q + o);
    _SsVmGetSeqVol((short)(a0 | (a1 << 8)), (char *)p + 0x5C, (char *)p + 0x5E);
    ((LibStateB0 *)(o + (int)*q))->w98 &= ~1;
    ((LibStateB0 *)(o + (int)*q))->w98 &= ~8;
    ((LibStateB0 *)(o + (int)*q))->w98 |= 2;
}

void SsSeqPause(short a) { _SsSndSetPauseMode(a, 0); }

void SsSepPause(short a, short b) { _SsSndSetPauseMode(a, b); }

void SsSeqPlay(short a, signed char b, short c) { Snd_SetPlayMode(a, 0, b, c); }

void SsSepPlay(short a, short b, signed char c, short d) { Snd_SetPlayMode(a, b, c, d); }

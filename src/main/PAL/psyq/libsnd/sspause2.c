/* GCC_VERSION: 2.7.2 */
#include "psyq/libsnd/ssatr2.h"
#include "psyq/libsnd/sspause2.h"
#include "rr/sequencer_init.h"

void _SsSndSetReplayMode(int a0, short a1);
void SsSepReplay(short a, short b);
void Snd_SetPlayMode(s32 a0, s32 a1_in, s32 a2, s32 a3_in);

void _SsSndSetReplayMode(int a0, short a1) {
    int t = a0 << 16;
    LibStateB0 **b = _ss_score;
    LibStateB0 **q = (LibStateB0 **)((int)b + (t >> 14));
    int o = a1 * 0xB0;
    LibStateB0 *p = (LibStateB0 *)((char *)*q + o);
    int f = p->w98;
    if (!(f & 0x204)) {
        if (!(f & 0x100)) {
            p->w98 = f & ~2;
            ((LibStateB0 *)(o + (int)*q))->w98 |= 8;
            ((LibStateB0 *)(o + (int)*q))->w98 |= 1;
        }
    }
}

void SsSeqReplay(short a) { _SsSndSetReplayMode(a, 0); }

void SsSepReplay(short a, short b) { _SsSndSetReplayMode(a, b); }

void Snd_SetPlayMode(s32 a0, s32 a1_in, s32 a2, s32 a3_in) {
    short a1 = a1_in;
    signed char a3 = a3_in;
    int t = a0 << 16;
    LibStateB0 **b = _ss_score;
    LibStateB0 **q = (LibStateB0 **)((int)b + (t >> 14));
    int o = a1 * 0xB0;
    LibStateB0 *p = (LibStateB0 *)((char *)*q + o);
    a2 = (signed char)a2;
    p->c0 = (unsigned char *)p->c4;
    p->c8 = (unsigned char *)p->c4;
    p->cC = p->c4;
    ((LibStateB0 *)(a1 * 0xB0 + (int)*q))->w98 &= ~0x200;
    ((LibStateB0 *)(a1 * 0xB0 + (int)*q))->w98 &= ~4;
    p->b20 = a3;
    if (a2 == 1) {
        ((LibStateB0 *)(a1 * 0xB0 + (int)*q))->w98 |= 1;
        p->b14 = a2;
        p->b21 = 0;
        _SsVmSetSeqVol((short)(a0 | (a1 << 8)), p->h58[0], p->h58[1], 1);
    } else if (a2 == 0) {
        ((LibStateB0 *)(a1 * 0xB0 + (int)*q))->w98 |= 2;
    }
}


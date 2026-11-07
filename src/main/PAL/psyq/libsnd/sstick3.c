#include "rr/library.h"
#include "psyq/libsnd/vmtick.h"

extern RrLibStateB0 *_ss_score[];
extern void _SsVmSeqKeyOff(s16, s32, s32, s32);

void _SsSndReplay(s32 a0, s16 a1);
void _SsSndStop(s32 a0, s32 a1);

void _SsSndReplay(s32 a0, s16 a1) {
    s32 t = a0 << 16;
    RrLibStateB0 **b = _ss_score;
    RrLibStateB0 **q = (RrLibStateB0 **)((s32)b + (t >> 14));
    s32 o = a1 * 0xB0;
    ((RrLibStateB0 *)((char *)*q + o))->b14 = 1;
    ((RrLibStateB0 *)(o + (s32)*q))->w98 &= ~8;
}

void _SsSndStop(s32 a0, s32 a1) {
    s32 t = a0 << 16;
    RrLibStateB0 **b = _ss_score;
    RrLibStateB0 **q = (RrLibStateB0 **)((s32)b + (t >> 14));
    s32 o = (s16)a1 * 0xB0;
    s32 a;
    RrLibStateB0 *c = (RrLibStateB0 *)((char *)*q + o);
    s32 i;
    RrLibStateB0 *p4;
    c->w98 &= ~1;
    ((RrLibStateB0 *)(o + (s32)*q))->w98 &= ~2;
    a = a1 << 8;
    ((RrLibStateB0 *)(o + (s32)*q))->w98 &= ~8;
    p4 = (RrLibStateB0 *)(o + (s32)*q);
    p4->w98 &= ~0x400;
    ((RrLibStateB0 *)(o + (s32)*q))->w98 |= 4;
    _SsVmSeqKeyOff(a0 | a, a, (s32)p4, -0x401);
    _SsVmDamperOff();
    c->b14 = 0; c->w88 = 0; c->b1c = 0; c->b18 = 0; c->b19 = 0; c->b1e = 0; c->b1a = 0; c->b1b = 0; c->b1f = 0; c->b17 = 0; c->b21 = 0; c->b1c = 0; c->b1d = 0; c->b15 = 0; c->b16 = 0;
    c->w90 = *(s32 *)((char *)c + 0x84);
    *(s32 *)((char *)c + 0x94) = *(s32 *)((char *)c + 0x8c);
    c->h48[6] = c->h48[7];
    *(s32 *)c = c->c4;
    c->c8 = (u8 *)(unsigned long)(u32)*(volatile s32 *)&c->c4;
    for (i = 0; i < 0x10; i++) {
        c->b37[i] = i;
        c->b27[i] = 0x40;
        c->h60[i] = 0x7F;
    }
    c->h58[2] = 0x7F; c->h58[3] = 0x7F;
}

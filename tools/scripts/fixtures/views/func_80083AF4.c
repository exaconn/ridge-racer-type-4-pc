#include "rr/library.h"

extern LibStateB0 *_ss_score[];

extern void _SsVmSeqKeyOff(s16, s32, s32, s32);

extern void _SsVmDamperOff(void);

void _SsSndStop(s32 a0, s32 a1) {
    s32 t = a0 << 16;
    LibStateB0 **b = _ss_score;
    LibStateB0 **q = (LibStateB0 **)((s32)b + (t >> 14));
    s32 o = (s16)a1 * 0xB0;
    s32 a;
    LibStateB0 *c = (LibStateB0 *)((char *)*q + o);
    s32 i;
    LibStateB0 *p4;
    c->w98 &= ~1;
    ((LibStateB0 *)(o + (s32)*q))->w98 &= ~2;
    a = a1 << 8;
    ((LibStateB0 *)(o + (s32)*q))->w98 &= ~8;
    p4 = (LibStateB0 *)(o + (s32)*q);
    p4->w98 &= ~0x400;
    ((LibStateB0 *)(o + (s32)*q))->w98 |= 4;
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

#include "types.h"
#include "rr/library.h"
#include "m2c_macros.h"
#include "psyq/libsnd/cc_nrpn3.h"

typedef struct { unsigned short s0, s2, s4, s6, s8; short sA, sC, sE, s10; } S;

void func_80080E54();
void func_80080F04(s32 arg0, u32 arg1, void *arg2);
void func_80081004();

void func_80080E54(a0, a1, a2, s, c) s16 a0; s16 a1; s16 a2; RrLibBlock24Fields s; u8 c; {
    register s32 cc = c;
    s16 loc[12];
    func_800850C4(a0, a1, a2, &s);
    func_80080F04(s.h10, s.h12, loc);
    loc[5] = 0;
    loc[0] = cc;
    func_80080F60(loc, &s.h10, &s.h12);
    func_800867E4(a0, a1, a2, &s);
}

void func_80080F04(s32 arg0, u32 arg1, void *arg2) {
    u32 temp_v1;

    M2C_FIELD(arg2, s16 *, 0xA) = (s16) (arg0 & 0x8000);
    M2C_FIELD(arg2, s16 *, 0xC) = (s16) (arg1 & 0x8000);
    M2C_FIELD(arg2, s16 *, 0x10) = (s16) (arg1 & 0x4000);
    temp_v1 = arg0 & 0xFFFF;
    M2C_FIELD(arg2, s16 *, 0xE) = (s16) (arg1 & 0x20);
    M2C_FIELD(arg2, s16 *, 0) = (s16) ((temp_v1 >> 8) & 0x7F);
    M2C_FIELD(arg2, s16 *, 2) = (s16) ((temp_v1 >> 4) & 0xF);
    M2C_FIELD(arg2, s16 *, 4) = (s16) (arg0 & 0xF);
    M2C_FIELD(arg2, s16 *, 6) = (s16) ((arg1 >> 6) & 0x7F);
    M2C_FIELD(arg2, s16 *, 8) = (s16) (arg1 & 0x1F);
}

void func_80080F60(a0, a1, a2) S *a0; short *a1; short *a2; {
    S *p;
    short *d; 
    register int c __asm__("$2"); /* ASM_BARRIER: pin to the original register allocation */
    register int t __asm__("$5"); /* ASM_BARRIER: pin to the original register allocation */
    int v; 
    int e; 
    register int x __asm__("$3"); /* ASM_BARRIER: pin to the original register allocation */
    register int y; 
    register int w __asm__("$5"); /* ASM_BARRIER: pin to the original register allocation */
    int k, g;
    p = a0;
    d = a1;
    k = -0x8000;
    c = p->sC;
    g = p->s10;
    c = c != 0;
    c = -c;
    t = c & k;
    v = t;
    c = p->sA;
    c = c != 0;
    c = -c;
    e = c & k;
    if (g != 0) v = t | 0x4000;
    if (p->sE != 0) v |= 0x20;
    x = (p->s0 << 8) & 0x7F00;
    x = e | x;
    y = (p->s2 * 0x10) & 0xF0;
    x |= y;
    w = p->s8 & 0x1F;
    x |= p->s4 & 0xF;
    y = v | ((p->s6 << 6) & 0x1FC0) | w;
    *d = x;
    *a2 = y;
}

void func_80081004(a0, a1, a2, s, c) s16 a0; s16 a1; s16 a2; RrLibBlock24Fields s; u8 c; {
    register s32 cc = c;
    s16 loc[12];
    func_800850C4(a0, a1, a2, &s);
    func_80080F04(s.h10, s.h12, loc);
    loc[5] = 1;
    loc[0] = cc;
    func_80080F60(loc, &s.h10, &s.h12);
    func_800867E4(a0, a1, a2, &s);
}

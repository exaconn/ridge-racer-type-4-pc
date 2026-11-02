#include "rr/geometry.h"
typedef struct { u8 c[8]; } B8;

extern s8 D_8009EFFC[];
extern char D_80010460[];
extern s32 D_800ACC14;
extern s32 func_8002071C();

void func_8002AFA4(a0, a1, a2, a3, a4, a5) s32 a0; s32 a1; s32 a2; s32 a3; s32 a4; s32 a5; {
    s32 i, j, k, t0, sel, sg, off;
    register s32 s1 __asm__("$17"); /* ASM_BARRIER: pin i+1 to s1 so loop var i gets s2 (saved reg order) */
    s8 *tb;
    RrVec2s *p, *bp;
    char *ub;
    struct { RrVec2s v[3]; s32 pad; B8 u; } w;
    i = 0;
    bp = w.v;
    tb = D_8009EFFC;
    t0 = *(s32 *)0x1F800000;
    w.u = *(B8 *)D_80010460;
L0:
    j = 0;
L1:
    k = 0;
    sg = 1 - (i & 1) * 2;
    p = bp;
    off = j << 1;
    do {
        p->x = (*(s8 *)(off + (s32)tb) - a2) * sg + a0;
        p->y = *(s8 *)(off + (s32)tb + 1) + a1;
        off += 4;
        k++;
        p++;
    } while (k < 3);
    s1 = i + 1;
    sel = 0;
    if (a5 == s1) sel = 0 < j;
    j++;
    sg = D_800ACC14 + 0x70;
    ub = (char *)&w.u;
    t0 = func_8002071C(sg, t0, ((s32 *)w.v)[0], ((s32 *)w.v)[1], ((s32 *)w.v)[2], ub + sel * 4, 0);
    if (j < 2) goto L1;
    i = s1;
    if (i < 2) goto L0;
    *(s32 *)0x1F800000 = t0;
}

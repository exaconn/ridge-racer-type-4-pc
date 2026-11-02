#include "rr/geometry.h"
typedef struct { u8 c[8]; } B8;

extern s8 D_8009EFFC[];
extern char D_80010460[];
extern s32 D_800ACC14;
extern s32 D_8009FDA4;
extern s32 func_8002071C();
extern s32 func_8001FD20();

void func_8002B12C(p0, p1, p2, p3, p4, a5, a6) s32 p0; s32 p1; s32 p2; s32 p3; s32 p4; s32 a5; s32 a6; {
    s32 i, k, t0, g, sel, sg;
    register s32 one __asm__("$18"); /* ASM_BARRIER: pin s-reg order (saved reg priority) */
    s32 a3; 
    register s32 a4 __asm__("$20"); /* ASM_BARRIER: pin s-reg order (saved reg priority) */
    s32 a0, a1;
    s32 a2; 
    RrVec2s *p, *bp;
    char *ub;
    s8 *tb, *nv;
    struct { RrVec2s v[3]; s32 pad; B8 u; } w;
    a0 = p0; a1 = p1; a2 = p2; a3 = p3;
    i = 0;
    bp = w.v;
    one = 1;
    a4 = p4;
    t0 = *(s32 *)0x1F800000;
    g = D_8009FDA4;
    nv = D_8009EFFC;
    w.u = *(B8 *)D_80010460;
L0:
    k = 0;
    sg = one - (i & 1) * 2;
    p = bp;
    tb = nv;
    do {
        p->x = (tb[0] - a2) * sg + a0;
        p->y = tb[1] + a1;
        tb += 4;
        k++;
        p++;
    } while (k < 3);
    if (a3 == one) g = 0;
    if (g == one) {
        sel = 1;
        if (i != 0) goto B8;
    } else {
B8:
        sel = 0;
        if (g == 2) sel = i == 1;
    }
    sel ^= a3;
    if (a4 == a5) {
        if (i != 0) goto B12;
    } else {
B12:
        if (a4 == a6 - 1) {
            if (i != one) goto B14;
        } else {
B14:
            ub = (char *)&w.u;
            t0 = func_8002071C(D_800ACC14 + 0x70, t0, ((s32 *)w.v)[0], ((s32 *)w.v)[1], ((s32 *)w.v)[2], ub + (sel << 2), 0);
        }
    }
    i++;
    if (i < 2) goto L0;
    *(s32 *)0x1F800000 = t0;
    if (a3 == 1) {
        *(s32 *)0x1F800000 = func_8001FD20(D_800ACC14 + 0x70, t0, a0, a1 - 0xC, a3, 0x18, 0xFF, 0xFF, 0xFF);
    }
}

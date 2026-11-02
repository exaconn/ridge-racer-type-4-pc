/* CC1_FLAGS: -mno-split-addresses */
struct S2 { char p0[2]; unsigned char b2, b3, b4, b5, b6, b7; };
struct S {
    char p0[0x3C];
    struct S2 *q;
    char p1[6];
    unsigned char st, cnt;
    char p2[1];
    unsigned char f49;
    char p3[0x99];
    unsigned char c3, c4;
    unsigned short s6;
    char p4[1];
    unsigned char c9, ca, cb;
    unsigned short sc, se;
};
extern void (*D_800ABC6C)(struct S *);
extern int func_80098DD4();
extern void func_80098E0C();
#define VQ (*(struct S2 * volatile *)&a0->q)
int func_80098B64(a0) struct S *a0; {
    struct S2 *q;
    int t, n;
    /* ASM_BARRIER: pin b4 to $v1 so the load/add chain takes the original register allocation */
    register int b __asm__("$3");
    switch (a0->st) {
    case 2:
        q = a0->q;
        if (q->b7 != 0) goto r0;
        if (a0->c3 == q->b3 && a0->c4 == q->b4 && a0->c9 == q->b5 && a0->ca == q->b6) {
            a0->se = 0;
        } else {
            a0->se = 0xFFFF;
        }
        a0->c3 = VQ->b3;
        a0->c4 = VQ->b4;
        a0->s6 = 0;
        a0->c9 = VQ->b5;
        a0->ca = VQ->b6;
        a0->sc = 0;
        if (a0->se != 0) goto r0;
        a0->cb = 0;
        goto r1;
    case 3:
        if (a0->q->b2 != 0) goto r0;
        if (a0->q->b3 != 0) goto r0;
        t = a0->q->b5 + (a0->q->b4 << 8);
        a0->s6 = t;
        if (a0->se == (t & 0xFFFF)) goto eq;
        a0->se = t;
    r0:
        return 0;
    eq:
        a0->se = 0xFFFF;
        a0->cb = 0;
        a0->cnt = 0;
        goto r1;
    case 4:
        if (a0->q->b2 != 0) goto r0;
        if (a0->q->b3 != 0) goto r0;
        t = a0->sc;
        b = a0->q->b4;
        t += 8;
        t += (b + 3) & 0x1FC;
        n = a0->cnt + 1;
        a0->cnt = n;
        a0->sc = t;
        if ((unsigned char)n < a0->ca) goto r0;
        if (func_80098DD4(a0) >= 0x81) {
            D_800ABC6C(a0);
            a0->st = 0xFE;
            a0->f49 = 2;
            goto r0;
        }
        if (a0->se != a0->sc) {
            a0->se = a0->sc;
            a0->cnt = 0;
            a0->sc = 0;
            return 0;
        }
        a0->se = 0;
        a0->cb = 0;
        a0->st = 0xFF;
        func_80098E0C(a0, (char *)a0 + 0x63);
        a0->st = 2;
        goto r0;
    default:
        return 1;
    }
r1:
    return 1;
}

/* CC1_FLAGS: -mno-split-addresses */
struct HW { unsigned char b0; char p1[3]; unsigned short s4; short p6; short s8; unsigned short sA; char pC[2]; short sE; };
struct A { char p0[0xC]; char *f0C; char p1[0x27]; unsigned char b37; char p2[0x18]; unsigned char b50; char p3[0x97]; unsigned char bE8; };
extern volatile struct HW *D_800ABCCC;
extern volatile int *D_800ABCC8;
extern int D_800ABCA8;
extern int D_800ABCC0[];
extern void (*D_800ABC88)();
extern void (*D_800ABC8C)();
extern int func_8009A814(), func_80098A00();
extern void func_8009A7F4(), func_80098A90();
int func_80098154(a0) struct A *a0; {
    int *p;
    volatile struct HW *hw;
    int v;
    register int t __asm__("$3");
    register int *base __asm__("$17");
    D_800ABCCC->sA = 0x40;
    D_800ABCCC->sA = 0;
    D_800ABCCC->s8 = 0xD;
    D_800ABCCC->sE = 0x88;
    func_8009A7F4(a0->bE8 == 8 ? 0x50 : 0x91);
    hw = D_800ABCCC;
    t = 0x1003;
    if (D_800ABCA8 != 0) t = 0x3003;
    hw->sA = t;
    if (D_800ABCC0[D_800ABCA8] >= 0) {
        if (D_800ABCC0[D_800ABCA8] > 0) {
            base = D_800ABCC0;
            do {
                v = base[D_800ABCA8] - 1;
                base[D_800ABCA8] = v;
                D_800ABC88(a0->f0C + v * 0xF0);
            } while (base[D_800ABCA8] > 0);
        }
        p = &D_800ABCC0[D_800ABCA8];
        if (*p == 0) {
            *p = -1;
            D_800ABC88(a0);
            D_800ABC8C(a0);
        }
    }
    if (D_800ABCCC->s4 & 0x200) {
        D_800ABCCC->sA |= 0x10;
        if (D_800ABCCC->s4 & 0x200) {
            while (func_8009A814() == 0);
            D_800ABCCC->b0 = 1;
            func_8009A7F4(0x64);
            if (func_80098A00() == 0) return 0;
            func_80098A90();
            D_800ABCCC->b0;
            func_8009A7F4(0x1AE);
            goto l1;
        l1b:
            if (func_8009A814(0x3C) != 0) return 0;
        l1:
            if (!(*D_800ABCC8 & 0x80)) goto l1b;
            D_800ABCCC->b0 = 0x42;
            func_8009A7F4(0x3C);
            if (func_80098A00() == 0) return 0;
            func_80098A90();
            D_800ABCCC->b0;
            func_8009A7F4(0x1AE);
            goto l2;
        l2b:
            if (func_8009A814(0x3C) != 0) return 0;
        l2:
            if (!(*D_800ABCC8 & 0x80)) goto l2b;
            D_800ABCCC->b0 = 1;
            func_8009A7F4(0x3C);
            if (func_80098A00() == 0) return 0;
            func_80098A90();
            D_800ABCCC->b0;
            return 0;
        }
        *D_800ABCC8 = -0x81;
    }
    v = 1;
    if (a0->b50 != 0) {
        v = 0;
        if (a0->b37 == 0) v = 1;
    }
    return v;
}

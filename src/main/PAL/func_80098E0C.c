/* CC1_FLAGS: -mno-split-addresses */
typedef struct { int f0; int f4; int f8; char p0[8]; int f14; int f18; char p1[0x2A]; char f46; char f47; char p2[1]; char f49; char p3[0x99]; unsigned char fE3; char p4[5]; unsigned char fE9; } O;
extern int (*D_800ABC84)();
extern int func_80098EE0(), func_80098F88();
int func_80098E0C(o, a1) O *o; int a1; {
    int r;
    if (a1 != 0) {
        if (o->f4 != 0) return 0;
        if (D_800ABC84() != 0) return 0;
    } else return 0;
    /* ASM_BARRIER: return value 1 must be a separate li (not CSE'd with the constant 1 used by the stores/shift) */
    asm volatile("" : "=r"(r) : "0"(1));
    o->f49 = 4;
    o->f0 = (a1 + 3) >> 2 << 2;
    o->f4 = o->f0 + ((o->fE3 + 1) >> 1 << 2); o->f8 = o->f4 + ((o->fE9 * 5 + 3) & 0xFFC);
    o->f46 = 1;
    o->f14 = (int)func_80098EE0;
    o->f47 = 0;
    o->f18 = (int)func_80098F88;
    return r;
}

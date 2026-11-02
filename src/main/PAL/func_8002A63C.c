/* RODATA 0x80010448 0x18 */

typedef struct {
    unsigned char u, v, w, h;
} __attribute__((packed)) R4;

typedef struct {
    unsigned char a, b, c, d;
} S_ACC68;

typedef struct {
    short s0;
    short s2;
    unsigned char b4;
    unsigned char b5;
    unsigned char b6;
    unsigned char b7;
    int w8;
    unsigned char bC;
    unsigned char bD;
    short sE;
    short s10;
    short s12;
    short s14;
    short s16;
    short s18;
    short s1A;
    short s1C;
    short s1E;
} S_F4CB0;

typedef struct {
    char pad[0x10];
    int w10;
    char pad14[0x48];
} S_F3A38;

extern unsigned short D_80010440[];
extern R4 D_8009EE70[];
extern R4 D_8009EEC8;
extern signed char D_8009EF3C[][4];
extern short D_8009EFD8[];
extern short D_8009EFE8[];
extern R4 D_8009EFF8;
extern int D_800AC5D4;
extern char *D_800ACC14;
extern S_ACC68 D_800ACC68;
extern S_F3A38 D_800F3A38[];
extern S_F4CB0 D_800F4CB0;

extern void *func_8001FC28();
extern void *func_8001FCC8();
extern void *func_8001FD20();
extern void *func_8001FF84();
extern void *func_80020074();
extern void *func_80020508();
extern int func_80090424();

void func_8002A63C(int arg0) {
    R4 sp30;
    int s7;
    int y;
    int sp38;
    int s0;
    int s6;
    int v;
    int s1, j, m, k;
    int s4;
    int s5;
    unsigned short sp40;
    void *a1;
    int c;
    R4 *r;
    int s2;
    unsigned short *pc;
    int col1, col2, col3;

    s7 = 0x80;
    if (D_800F4CB0.b6 != arg0) {
        D_800F4CB0.b7 = D_800F4CB0.b6;
    }
    y = 0x1C0;
    D_800F4CB0.b6 = arg0;
    s0 = D_800F4CB0.s2;
    s6 = D_800F4CB0.b6;
    sp38 = D_800ACC68.c;

    switch (D_800F4CB0.s0) {
    case 0:
        D_800F4CB0.s12 = 0x38;
        D_800F4CB0.s1A = 0x1B8;
        D_800F4CB0.s16 = D_800F4CB0.s1E = 2;
        if (D_800ACC68.a == 2) {
            D_800F4CB0.s0 = 1;
            D_800F4CB0.b4 = D_800ACC68.c;
        }
        break;
    case 1:
        D_800F4CB0.s14 = D_800F4CB0.s1C = (func_80090424((s0 << 10) / 30) * 0x140) / 0x1000;
        if (s0 >= 30) {
            D_800F4CB0.s0 = 2;
            D_800F4CB0.s2 = 0;
        } else {
            D_800F4CB0.s2++;
        }
        break;
    case 2:
        v = (func_80090424((s0 << 10) / 20) * 0x20) / 0x1000;
        D_800F4CB0.s16 = D_800F4CB0.s1E = v * 2 + 2;
        D_800F4CB0.s12 = 0x38 - v;
        D_800F4CB0.s1A = 0x1B8 - v;
        if (s0 >= 20) {
            D_800F4CB0.s0 = 3;
            D_800F4CB0.s2 = 0;
        } else {
            D_800F4CB0.s2++;
        }
    case 3:
        if (D_800ACC68.a == 1) {
            D_800F4CB0.s0 = 4;
            D_800F4CB0.s2 = 0;
        }
        if (D_800ACC68.a != 2) {
            s6 = D_800F4CB0.b7;
            sp38 = D_800ACC68.c ^ 1;
        }
        break;
    case 4:
        if (s0 >= 20) {
            if (D_800ACC68.a == 2) {
                D_800F4CB0.s0 = 5;
                D_800F4CB0.s2 = 0;
                D_800F4CB0.b4 = D_800ACC68.c;
            }
        } else {
            D_800F4CB0.s2++;
        }
        s7 = ((20 - s0) * 0x80) / 20;
        sp38 = D_800ACC68.c ^ 1;
        s6 = D_800F4CB0.b7;
        break;
    case 5:
        if (s0 >= 20) {
            D_800F4CB0.s0 = 3;
            D_800F4CB0.s2 = 0;
        } else {
            D_800F4CB0.s2++;
        }
        s7 = (s0 * 0x80) / 20;
        break;
    }

    pc = D_80010440;
    sp40 = (((sp38 + 1) & 3) << 5) | 0x1E;
    a1 = func_8001FC28(D_800ACC14 + 0x70, *(void **)0x1F800000, 0, 0, 0x140, 0x200);
    a1 = func_80020074(D_800ACC14 + 0x70, a1, 0x10, 0x24, 0x20, 0x10, 0, 0x20, sp40, 0x7F38, 1, (unsigned short)s7);
    s1 = 0;
    s4 = 0x30;
    s5 = 0x24;
    s7++;

    c = D_8009EF3C[s6][0];
    while (c != -1) {
        r = &D_8009EE70[c];
        a1 = func_8001FF84(D_800ACC14 + 0x70, a1, s4, s5, r, 0x7F38, ((unsigned short)s7 << 1) | 1);
        c = D_8009EF3C[s6][++s1];
        { int t = s4 + 2; s4 = t + r->w; }
    }

    if (D_800F4CB0.w8 > 0) {
        a1 = func_8001FF84(D_800ACC14 + 0x70, a1, s4, s5, &D_8009EEC8, 0x7F38, ((unsigned short)s7 << 1) | 1);
        D_8009EFF8.u = D_800F4CB0.w8 * 8;
        { int t = s4 + 2; s4 = t + D_8009EEC8.w; }
        a1 = func_8001FF84(D_800ACC14 + 0x70, a1, s4, s5, &D_8009EFF8, 0x7F38, 1, ((unsigned short)s7 << 1) | 1);
    }

    a1 = func_80020508(
        D_800ACC14 + 0x70, a1, 8, 0x38, 0x130, 2,
        col1 = *pc, col1, col1
    );
    a1 = func_8001FCC8(D_800ACC14 + 0x70, a1, sp40);
    a1 = func_8001FC28(D_800ACC14 + 0x70, a1, D_800F4CB0.s10, D_800F4CB0.s12, D_800F4CB0.s14, D_800F4CB0.s16);

    for (j = 2; j < 7; j++) {
        s4 = D_8009EFD8[j];
        s5 = (j >= 2) ? 0x1BE : 0x1AA;
        sp30 = D_8009EE70[j];
        if ((unsigned int)(j - 3) < 2) {
            s2 = (D_800F3A38[D_800AC5D4 & 1].w10 == 1);
            if (s2 == 1) {
                sp30.u += 0x14;
                if (j == 4) {
                    s4 -= 4;
                } else {
                    s4 -= 2;
                }
            }
        } else if (j == 5) {
            if (D_800F4CB0.bC != 1) {
                sp30.w -= 0x14;
            }
        } else if (j == 6) {
            if (D_800F4CB0.bD != 1) {
                sp30.w -= 0x1E;
            }
        }
        a1 = func_8001FF84(D_800ACC14 + 0x70, a1, s4, s5, &sp30, 0x7F38, ((unsigned short)s7 << 1) | 1);
    }

    if (D_800F4CB0.sE == 0) {
        s2 = 0x32;
    } else {
        s2 = 0x31;
    }
    if (D_800ACC68.a == 2) {
        j = sp38 + 0x7DBA;
        a1 = func_8001FD20(D_800ACC14 + 0x70, a1, D_8009EFE8[7], y, 1, 0xC, 0xF0, 0x7A, 0x14);
        a1 = func_8001FF84(D_800ACC14 + 0x70, a1, D_8009EFE8[7] + 8, s5, &D_8009EE70[s2], j, 0);
    }

    col2 = *pc;
    a1 = func_8001FCC8(D_800ACC14 + 0x70, a1, sp40);
    {
        char *p;
        int w, w2, w3, d;
        for (j = 3; j < 7; j++) {
            a1 = func_80020508(D_800ACC14 + 0x70, a1, D_8009EFE8[j], (j >= 3) ? y : 0x1A8, 1, 0xC, col2, col2, col2);
            /* ASM_BARRIER: define p after last loop call so p is live-in to next block without crossing calls */
            __asm__("" : "=r"(p));
        }
        w = 0x130;
        /* ASM_BARRIER: prevent constant folding of 0x130 into stack store */
        __asm__("" : "+r"(w));
        /* ASM_BARRIER: create anti-dependency from p load to w2 birthing insn in sched1 */
        __asm__("" : "=r"(w2) : "0"(w), "r"(p));
        /* ASM_BARRIER: chain birthing insn w3 so 16(sp) store stall cycle is filled before a3=0x1B8 */
        __asm__("" : "=r"(w3) : "0"(w2));
        a1 = func_80020508(
            (p = D_800ACC14) + 0x70, a1, 8, 0x1B8,
            (col3 = *pc, w3), 2,
            col3, col3, ({ /* ASM_BARRIER: keep w3 live past 16(sp) store so combine preserves w3 birthing insn */ __asm__("" : "=r"(d) : "r"(w3), "m"(*(char *)col3)); col3; })
        );
        if (D_800F4CB0.bD != 1) {
            a1 = func_80020508(D_800ACC14 + 0x70, a1, 0xEE, 0x1C6, 0x16, 2, col3, col3, col3);
        }
        if (D_800F4CB0.bC != 1) {
            a1 = func_80020508(D_800ACC14 + 0x70, a1, 0xA8, 0x1C6, 0x12, 2, col3, col3, col3);
            /* ASM_BARRIER: make REG_N_SETS > 1 to prevent birthing_insn_p boost in sched1 */
            __asm__ volatile("" : "+r"(col3), "=r"(p) : "m"(D_800ACC14), "m"(D_800ACC14), "m"(D_800ACC14), "m"(D_800ACC14), "m"(D_800F4CB0.bC), "m"(D_800F4CB0.bC));
        }
        /* ASM_BARRIER: extend live range of d so global-alloc assigns %hi(D_800ACC14) to $s5 */
        __asm__ volatile("" : : "r"(d));
        a1 = func_8001FC28(D_800ACC14 + 0x70, a1, D_800F4CB0.s18, D_800F4CB0.s1A, D_800F4CB0.s1C, D_800F4CB0.s1E);
        *(void **)0x1F800000 = func_8001FCC8(D_800ACC14 + 0x70, a1, sp40);
    }
}

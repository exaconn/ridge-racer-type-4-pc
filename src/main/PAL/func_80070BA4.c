extern int func_80096454();
extern int func_80096464();
extern int func_8006E09C();
extern int func_8006E108();
extern int func_8006E030();
extern int func_80090DB4();
extern int func_80090F14();
extern int func_80095AC4();
extern int func_80070934();
extern int func_80091308();
extern int func_80070ACC();
extern int func_8006DB0C();
extern int func_80095A84();
extern int func_80090884();
extern int func_8006E368();

typedef struct { unsigned char c[4]; } U4;
typedef struct { int x, y; } __attribute__((packed)) U8;
typedef struct { short h0, h2, h4, h6, h8, hA, hC, hE; unsigned char c10[8]; unsigned char c18[8]; int f20; } P;
typedef struct { U4 u; int n; } K;
typedef struct {
    P *p0; int f4, f8, fC, f10, f14, f18, f1C, p20, p24, f28, f2C, f30, f34, f38, p3C;
    K *k40; K *k44;
    U4 c48[16]; U4 c88[16];
    int fC8, fCC, fD0, pD4[5]; int e8[2]; int f0[2];
} O;

extern O *D_8010E78C;
extern int D_800F4BF0;
extern short D_80107180[];
extern short D_80107182;
extern short D_80107184;
extern struct { short m[3][3]; int t[3]; } D_800AD570;
extern struct { short a, b, c, d; } D_800F38F8[];

void func_80070BA4(char *a0, int a1, int a2) {
    int sp10[3];
    int sp20[8];
    int sp40[8];
    short sp60[4];
    int sp68, sp6c, sp70;
    O *s1; /*D*/
    P *s0; /*D*/
    K *k; /*D*/
    int s2; /*D*/
    int s3; /*D*/
    int s7; /*D*/
    int *g = (int *)0x1F800008; /*D*/
    int a; /*D*/
    int b; /*D*/
    int c; /*D*/
    int d; /*D*/
    unsigned char *q; /*D*/

    func_80096454(sp60, 0, 8);
    sp60[2] = 0x100;
    s1 = D_8010E78C + a1;
    s7 = *(int *)(a0 + 4) + 0x900;
    s1->f30 = a2;
    s2 = (s1->f14 - s1->f28) & 0xF;
    if (s1->f10 < s1->f8) {
        a1 = 0;
        while (a1 < 10) {
            if (!(s1->f10 < s1->f8)) break;
            s0 = s1->p0 + s1->f10;
            s0->h0 = 0;
            s0->h2 = 0;
            s0->h4 = 0;
            s0->h8 = 0;
            s0->hA = 0;
            s0->hC = 0;
            s0->f20 = func_80096464() & 0xF;
            func_8006E09C(sp20, func_80096464() & 0xFFF);
            a1++;
            func_8006E108(sp40, func_80096464() & 0xFFF);
            func_80090DB4(sp40, sp20);
            func_8006E030(sp40, func_80096464() & 0xFFF);
            func_80090DB4(sp40, sp20);
            func_80090F14(sp20, sp60, s0->c10);
            *(U8 *)s0->c18 = *(U8 *)s0->c10;
            s1->f10 = s1->f10 + 1;
        }
    } else {
        a = s1->f0[D_800F4BF0];
        s3 = s1->e8[D_800F4BF0];
        if (a != 0) func_80095AC4(a, a + 0x14);
        __asm__ volatile ("cfc2 $8, $21\n\tcfc2 $9, $22\n\tcfc2 $10, $23\n\tsw $8, 0(%0)\n\tsw $9, 0(%1)\n\tsw $10, 0(%2)"
            : : "r"(&sp68), "r"(&sp6c), "r"(&sp70) : "$8", "$9", "$10", "memory");
        if (s1->f14 > s1->f2C) {
            func_80070934(a1, 4, 0, -1);
        } else {
            a = s1->f28;
            if (s1->f14 < a) {
                if (s1->f30) s1->f14 = s1->f14 + 1;
                s1->f4 = 0;
                s1->f0[D_800F4BF0] = 0;
            } else {
                b = s1->f14 - a + 1;
                s1->f4 = b;
                if (b >= 0x10) s1->f4 = s1->f8;
                else s1->f4 = b * (s1->f8 / 16);
                if (s1->f30) s1->f14 = s1->f14 + 1;
                k = s1->k40 + s1->f34;
                if (k->n == -1) {
                    s1->c48[s2] = k->u;
                } else {
                    __asm__ volatile ("ctc2 %0, $21\n\tctc2 %1, $22\n\tctc2 %2, $23" : : "r"(k[1].u.c[0] << 4), "r"(k[1].u.c[1] << 4), "r"(k[1].u.c[2] << 4));
                    d = (s1->f18 << 12) / k->n;
                    func_80091308(k, d, &s1->c48[s2]);
                    if (s1->f30) {
                        s1->f18 = s1->f18 + 1;
                        if (s1->f18 >= k->n) {
                            s1->f18 = 0;
                            s1->f34 = s1->f34 + 1;
                        }
                    }
                }
                k = s1->k44 + s1->f38;
                if (k->n == -1) {
                    s1->c88[s2] = k->u;
                } else {
                    __asm__ volatile ("ctc2 %0, $21\n\tctc2 %1, $22\n\tctc2 %2, $23" : : "r"(k[1].u.c[0] << 4), "r"(k[1].u.c[1] << 4), "r"(k[1].u.c[2] << 4));
                    d = (s1->f1C << 12) / k->n;
                    func_80091308(k, d, &s1->c88[s2]);
                    if (s1->f30) {
                        s1->f1C = s1->f1C + 1;
                        if (s1->f1C >= k->n) {
                            s1->f1C = 0;
                            s1->f38 = s1->f38 + 1;
                        }
                    }
                }
                func_80070ACC(s1);
                s3 = func_8006DB0C(a0, s3, s1, s2 << 2);
                if (s3 != s1->e8[D_800F4BF0]) s1->f0[D_800F4BF0] = s3 - 0x14;
                else s1->f0[D_800F4BF0] = 0;
                if (a1 < 2) {
                    if (s1->c48[s2].c[0] * 16 > D_80107180[0]) D_800AD570.m[0][a1] = (short)s1->c48[s2].c[0] << 4;
                    else D_800AD570.m[0][a1] = D_80107180[0];
                    if (s1->c48[s2].c[1] * 16 > D_80107180[1]) D_800AD570.m[1][a1] = (short)s1->c48[s2].c[1] << 4;
                    else D_800AD570.m[1][a1] = D_80107180[1];
                    if (s1->c48[s2].c[2] * 16 > D_80107180[2]) D_800AD570.m[2][a1] = (short)s1->c48[s2].c[2] << 4;
                    else D_800AD570.m[2][a1] = D_80107180[2];
                }
            }
        }
        c = s1->f0[D_800F4BF0];
        if (c != 0) {
            func_80095A84(s7, s1->e8[D_800F4BF0], c);
        } else if (a1 < 2) {
            D_800AD570.m[0][a1] = D_80107180[0];
            D_800AD570.m[1][a1] = D_80107180[1];
            D_800AD570.m[2][a1] = D_80107180[2];
        }
        if (a1 < 2) {
            sp10[0] = (g[0] >> 2) - 0x8000;
            sp10[1] = g[1] >> 2;
            sp10[2] = (g[2] >> 2) - 0x8000;
            sp10[0] = (sp10[0] >> 1) - s1->fC8;
            sp10[1] = (sp10[1] >> 1) - s1->fCC;
            sp10[2] = (sp10[2] >> 1) - s1->fD0;
            D_800F38F8[a1].a = -func_8006E368(func_80090884((sp10[0] >> 4) * (sp10[0] >> 4) + (sp10[2] >> 4) * (sp10[2] >> 4)), sp10[1] >> 4) & 0xFFF;
            D_800F38F8[a1].b = (0x400 - func_8006E368(sp10[0], sp10[2])) & 0xFFF;
            D_800F38F8[a1].c = 0;
        }
        __asm__ volatile ("ctc2 %0, $21\n\tctc2 %1, $22\n\tctc2 %2, $23" : : "r"(sp68), "r"(sp6c), "r"(sp70));
    }
}

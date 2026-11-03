#define B (*(int *)&D_800ACC18)

struct A { char p[0x14]; unsigned f14; char q[0x20]; unsigned f38; };

struct Q { int a; int v; };

struct E {
    struct A *src; int pad0; int z8, zC, z10;
    struct Q q1[4];
    int z34, z38;
    struct Q q2[4];
    int z5c, z60, z64, z68, z6c, z70;
    int pad74[4];
    struct Q q3[8];
    int fC4;
};

extern int func_80096434();
extern int D_800ACC18;
extern int *D_800F3A08[];
extern struct E D_8010B2E0[];
extern int D_8009DF98;
extern int D_8010943C;
extern int D_8010BF68;
extern int D_8010BF60;
extern int func_80012208();
extern int func_8001292C();
extern int func_80012A58();
extern void func_80012A68();
extern void func_800292A8();
extern void func_800293C4();
extern void func_800436E8();
extern void func_800700AC();
extern void func_80070108();
extern void func_8007026C();
extern void func_8007080C();
extern void func_800713C4();
extern void func_80071434();
extern int func_80077D08();
extern int D_8009DF98, D_80109440, D_80109444, D_80109448, D_8010944C, D_80109450, D_80109454;
extern int func_80012210();

void func_8001301C(struct A *a0, int a1);
int func_800131D0(int a0);
int func_80013200(void);
int func_80013514(int a0, int a1, int a2, int a3, int a4);

void func_8001301C(struct A *a0, int a1)
{
    int s1;
    int a3, v1, t, u;
    struct E *s0;
    struct Q *p;

    s0 = (struct E *)((char *)D_8010B2E0 + a1 * 0x320);
    a3 = D_800ACC18;
    u = ((a0->f14 + 2) / 3) * 0x18;
    t = ((a0->f38 + 2) / 3) * 0x18;
    s1 = 4;
    do {
        p = s0->q1;
        v1 = 3;
        s0->src = a0;
        s0->z8 = 0;
        s0->zC = 0;
        s0->z10 = 0;
        s0->z34 = 0;
        s0->z38 = 0;
        s0->z5c = 0;
        s0->z60 = 0;
        s0->z64 = 0;
        s0->z68 = 0;
        s0->z6c = 0;
        s0->z70 = 0;
        do {
            p->v = a3;
            p++;
            v1 -= 1;
            a3 += u;
        } while (v1 >= 0);
        p = s0->q2;
        v1 = 3;
        do {
            p->v = a3;
            p++;
            v1 -= 1;
            a3 += t;
        } while (v1 >= 0);
        p = s0->q3;
        v1 = 7;
        do {
            p->v = a3;
            p++;
            v1 -= 1;
            a3 += t;
        } while (v1 >= 0);
        s0++;
        s1 -= 1;
    } while (s1 > 0);
    D_800ACC18 = a3;
    if (*(unsigned char *)((char *)D_800F3A08[a1] + 0xA) == 2) {
        s0 = (struct E *)((char *)D_8010B2E0 + a1 * 0x320);
        s1 = 4;
        do {
            s1 -= 1;
            s0->fC4 = D_800ACC18;
            func_80096434(D_800ACC18, 0x100);
            s0++;
            D_800ACC18 += 0x100;
        } while (s1 > 0);
    }
}

int func_800131D0(int a0) {
    if (D_8009DF98 != 0) {
        return 0;
    }
    D_8010943C = a0;
    return D_8009DF98 = 1;
}

int func_80013200(void)
{
    int *a1;
    /* ASM_BARRIER: pin saved regs to original assignment */
    register int s1 __asm__("$17");
    register int s4 __asm__("$20");
    int s2,s3,s5,s6,s7,fp;
    int sp10, sp14, sp18;
    /* ASM_BARRIER: pin temps to the original v0/v1/a0 choices */
    register int a0 __asm__("$4");
    register int u __asm__("$2");
    register int t __asm__("$3");
    register int *pw __asm__("$2"); int s0;

    switch (D_8009DF98) {
    case 1:
        if (func_8001292C(func_80012208(D_8010943C), D_800ACC18) == 0)
            return 0;
        D_8009DF98 = 2;
    case 2:
        if (func_80012A58() == 0)
            return 0;
        a0 = *(int *)(D_800ACC18 + 4) + D_800ACC18;
        s0 = *(int *)(D_800ACC18 + 8) + D_800ACC18;
        *(int *)(D_800ACC18 + 4) = a0;
        *(int *)(D_800ACC18 + 8) = s0;
        func_800436E8(a0);
        func_800436E8(s0);
        D_8009DF98 = 3;
    case 3:
        if (func_8001292C(func_80012208(D_8010943C) + 1, D_800ACC18) == 0)
            return 0;
        D_8009DF98 = 4;
    case 4:
        s1 = func_80012A58();
        if (s1 == 0)
            return 0;
        a1 = (int *)B;
        a1++; t = *a1; sp10 = t + B; *a1 = sp10;
        a1++; t = *a1; s2 = t + B; *a1 = s2;
        a1++; t = *a1; sp14 = t + B; *a1 = sp14;
        a1++; t = *a1; sp18 = t + B; *a1 = sp18;
        a1++; t = *a1; fp = t + B; *a1 = fp;
        a1++; t = *a1; s6 = t + B; *a1 = s6;
        a1++; t = *a1; s7 = t + B; *a1 = s7;
        a1++; t = *a1; s3 = t + B; *a1 = s3;
        a1++; t = *a1; s5 = t + B; *a1 = s5;
        /* ASM_BARRIER: hoist lui of D_8010943C above the store (hi part only) */
        a1++; t = *a1; u = B; s4 = t + u; __asm__("" : "=r"(pw) : "0"(0x80110000)); *a1 = s4;
        a1++;
        if (*(int *)((char *)pw - 0x6BC4) == 7) {
            a0 = *a1; a0 += B;
            *a1 = a0;
            func_8007080C(a0);
        }
        D_8010BF68 = s2;
        D_800ACC18 = D_800ACC18 + s1;
        func_8007026C(sp10);
        func_80012A68(s2);
        func_80070108(sp14, 4);
        func_800700AC(sp18);
        func_800292A8(fp);
        func_800293C4(s6);
        D_8010BF60 = s7;
        D_800ACC18 = D_800ACC18 + func_80077D08(s3, D_800ACC18);
        func_800713C4(s5);
        func_80071434(s4);
        D_8009DF98 = 0;
        return 1;
    }
}

int func_80013514(int a0, int a1, int a2, int a3, int a4)
{
    int r;
    register int p0 asm("$17") = a0;
    register int p1 asm("$16") = a1;
    register int p2 asm("$18") = a2;
    register int p3 asm("$19") = a3;
    if (D_8009DF98 != 0) {
        return 0;
    }
    D_80109440 = func_80012210();
    r = 1;
    D_80109444 = a4;
    D_80109448 = p1;
    D_8010944C = p0;
    D_80109450 = p2;
    D_80109454 = p3;
    D_8009DF98 = r;
    return r;
}

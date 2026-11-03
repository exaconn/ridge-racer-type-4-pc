/* RODATA 0x800103e0 0x1c */
struct W3G0A { char *p0; char *p4; short s8, sA, sC, sE, s10, s12; char *p14; char *p18; short s1C, s1E, s20, s22, s24, s26; };

extern struct W3G0A D_8010BF88;
extern char D_800ADB60[], D_800D02D8[], D_800AE660[], D_800D0DD8[];
extern short D_80107178;
extern int func_80020920(), func_8001FCC8();
extern char D_8009EA90[], D_8009EACC[];
extern char *D_800ACC14;

void func_80021084(int a0);
void func_800211A4();

void func_80021084(int a0) {
    switch (a0) {
    case 0: case 1: case 5: break;
    case 2: case 3: case 4: case 6:
        if (a0 == 4 && D_80107178 == 2) {
        } else if (a0 != 6) {
            goto w3g0b;
        }
    }
    D_8010BF88.p0 = D_800ADB60;
    D_8010BF88.p4 = D_800D02D8;
    D_8010BF88.s8 = 0x106;
    D_8010BF88.sA = 0xE6;
    D_8010BF88.sC = 0x122;
    D_8010BF88.sE = 0xDC;
    D_8010BF88.s10 = 0x112;
    D_8010BF88.s12 = 0xCE;
    return;
w3g0b:
    D_8010BF88.p0 = D_800AE660;
    D_8010BF88.p4 = D_800D0DD8;
    D_8010BF88.s8 = 0xFC;
    D_8010BF88.sA = 0x58;
    D_8010BF88.sC = 0x128;
    D_8010BF88.sE = 0x4A;
    D_8010BF88.s10 = 0xFC;
    D_8010BF88.s12 = 0x68;
    D_8010BF88.p14 = D_800AE660;
    D_8010BF88.p18 = D_800D0DD8;
    D_8010BF88.s1C = 0xFC;
    D_8010BF88.s1E = 0xD8;
    D_8010BF88.s20 = 0x122;
    D_8010BF88.s22 = 0xCA;
    D_8010BF88.s24 = 0xFC;
    D_8010BF88.s26 = 0xE8;
}

void func_800211A4(a0)
int a0;
{
    unsigned *s0;
    unsigned *s2;
    char *s1;
    char *s4;
    int s3, s5, c3;
    unsigned m1;
    unsigned h1;
    unsigned m2;
    unsigned h2;

    s0 = *(unsigned **)0x1F800000;
    if (a0 >= 2) {
        s2 = (unsigned *)(D_800ACC14 + 0xB70);
        s5 = 4;
        if (a0 == 3) s5 = 2;
        s3 = 0;
        if (s5 != 0) {
            m1 = 0xFFFFFF;
            h1 = 0xFF000000;
            s1 = D_8009EACC;
            do {
                func_80020920(s0, s1);
                s1 += 0x14;
                s3++;
                *s0 = (*s0 & h1) | (*s2 & m1);
                *s2 = (*s2 & h1) | ((unsigned)s0 & m1);
                s0 += 5;
            } while (s3 < s5);
        }
    } else {
        s3 = 0;
        m2 = 0xFFFFFF;
        h2 = 0xFF000000;
        s4 = D_8009EA90;
        s2 = (unsigned *)(D_800ACC14 + 0x70);
        s1 = (char *)s0 + 0xA;
        do {
            func_80020920(s0, s4);
            __asm__ volatile("" : "=r"(c3) : "0"(3)); /* ASM_BARRIER: keep li 3 inside the loop (not hoisted) */
            if (a0 == c3) *(unsigned short *)s1 -= 0x80;
            s1 += 0x14;
            s4 += 0x14;
            s3++;
            *s0 = (*s0 & h2) | (*s2 & m2);
            *s2 = (*s2 & h2) | ((unsigned)s0 & m2);
            s0 += 5;
        } while (s3 < 3);
    }
    *(int *)0x1F800000 = func_8001FCC8(s2, s0, 5);
}

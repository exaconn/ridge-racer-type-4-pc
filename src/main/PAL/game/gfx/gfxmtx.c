#include "psyq/libgte/gtemat7.h"
typedef struct { unsigned short a, b, c, d; } W8;

typedef struct { unsigned short a, b, c; } W6;

typedef struct { unsigned char a, b, c, d; } B4;

extern unsigned char D_8009F924[];
extern B4 D_8009F928[];
extern W8 D_8009F934[];
extern W6 D_800ABED0[];
extern char *D_800F4D90;
extern int D_800ACC24;
extern char *D_800F2AB0;
extern int D_800F2ECC;
extern char *D_800F3AFC;
extern int func_80096434();
extern int func_8001E84C();
extern int D_80010874[];

void func_8003CF34(int a0);
void func_8003D01C();
int func_8003D108();
int func_8003D1D4(int a0);

void func_8003CF34(int a0)
{
    short sp[3][3];
    int pad[2];
    int i;
    int x;

    for (i = 0; i < 3; i++) {
        D_800ABED0[i].a = D_8009F934[i].a;
        D_800ABED0[i].b = D_8009F934[i].b;
        D_800ABED0[i].c = D_8009F934[i].c;
        sp[0][i] = D_8009F928[i].a << 4;
        sp[1][i] = D_8009F928[i].b << 4;
        sp[2][i] = D_8009F928[i].c << 4;
    }
    x = D_8009F924[0] * a0;
    {
        int y = D_8009F924[1] * a0;
        x >>= 8;
        y >>= 8;
        func_80091274(x, y, (D_8009F924[2] * a0) >> 8);
    }
    func_80091114(sp);
}

/* legacy typing of shared globals in this part of the unit */
#define D_800ABED0 (*(unsigned short (*)[])&D_800ABED0)

void func_8003D01C(a0) int a0;
{
    short sp[14];
    short *s0;
    int i = 0;
    char *base;
    unsigned short *a3 = D_800ABED0;
    unsigned char *a1;
    unsigned char *a2;
    base = D_800F4D90;
    a1 = (unsigned char *)(a0 * 0x2C + (int)base);
    a2 = a1;
L:
    {
        short *v;
        s0 = sp;
        v = (short *)((int)s0 + i * 2);
        i++;
        a3[0] = *(unsigned short *)(a2 + 0x10);
        a3[1] = *(unsigned short *)(a2 + 0x12);
        a3[2] = *(unsigned short *)(a2 + 0x14);
        a2 += 8;
        v[0] = a1[4] << 4;
        v[3] = a1[5] << 4;
        v[6] = a1[6] << 4;
        a3 += 3;
        a1 += 4;
    }
    if (i < 3) goto L;
    {
        char *b2 = D_800F4D90;
        unsigned char *q = (unsigned char *)(a0 * 0x2C + (int)b2);
        func_80091274(q[0x28], q[0x29], q[0x2A], a3);
    }
    func_80091114(s0);
}

#undef D_800ABED0

int func_8003D108(a0) int a0;
{
    int r = 3;
    int i;
    int n;
    char *p;
    if (a0 & 1) {
        i = 0;
        n = D_800F2ECC;
        if (n > 0) {
            int c = 0xB;
            int m = n;
            p = D_800F3AFC;
            do {
                short t = *(short *)(p + 0x10);
                if (t != 0 && t != c) {
                    r &= 0xFFFE;
                    break;
                }
                i++;
                p += 0x1C;
            } while (i < m);
        }
    }
    if (a0 & 2) {
        i = 0;
        n = D_800ACC24;
        if (n > 0) {
            int c = 5;
            int m = n;
            a0 = (int)D_800F2AB0;
            do {
                signed char t = *(char *)(a0 + 1);
                if (t != 0) {
                    i++;
                    if (t != c) {
                        r &= 0xFFFD;
                        break;
                    }
                } else i++;
                a0 += 0x14;
            } while (i < m);
        }
    }
    return r;
}

/* legacy typing of shared globals in this part of the unit */
#define D_800F3AFC (*(int (*))&D_800F3AFC)

int func_8003D1D4(int a0) {
    int n = D_800F2ECC * 28;
    D_800F3AFC = a0;
    func_80096434(a0, n);
    func_8001E84C(D_80010874, n);
    return n;
}

#undef D_800F3AFC

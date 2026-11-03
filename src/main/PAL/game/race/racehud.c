#include "types.h"
#include "m2c_macros.h"

typedef struct { char pad[0x50]; short f; char pad2[0xA]; } S1;

typedef struct { char pad[0x1E4]; short f; char pad2[0x13A]; } S2;

typedef struct {
    short x, y;
} V;

extern int func_8004B864();
extern int func_8004BAD4();
extern S2 D_800ABEF8[];
extern S1 D_800F3A38[];
extern int func_8004F24C();
extern unsigned char D_8009F6DC[];
extern signed char D_800F2FDC;
extern unsigned short D_800F3A40;
extern int func_8003660C();
extern int D_8009F6E4[];
extern short D_800AC5D0;
extern char D_800AD4D8[];
extern int D_800F2A68;
extern int D_800F2EB8;
extern short D_800F3A26;
extern int D_800F4C64;
extern short D_800F4C6A;
extern short D_800F4D8A;
extern int D_800FF6B0;
extern int func_8001FEC0();
extern int func_8002071C();
extern int func_8001FCC8();
extern void func_80022694();
extern short D_800F4D44;
extern unsigned char D_8009F71C[][4];
extern int func_8009AEAC();
extern int func_80096F24();
extern int D_800105C8[], D_800F2AAC, D_800F2AA8;
extern int func_80096F64();
extern s32 D_800F2AAC;
extern s16 D_800FF688;

void func_800347BC(void);
void func_80034844(int a0);
void func_800348E8(int arg0);
void func_800349F4(int a0, int a1);
void func_80034E14(void);
int func_80034E3C(void);
int func_80034E70(int a0, unsigned int a1);
void func_80034E84(void);
void func_80034F18(void);
void func_80034F80(void);
int func_80034FC0(void);

void func_800347BC(void) {
    int i;
    func_8004B864(0);
    func_8004B864(1);
    for (i = 0; i < 2; i++) {
        if (D_800ABEF8[i].f != 0) {
            D_800F3A38[i].f = 1;
            func_8004BAD4(i, 0, 0xF);
        }
    }
}

void func_80034844(int a0) {
    if (D_800F3A40 & 0x1000) {
        if (D_800F2FDC > 0) {
            D_800F2FDC = D_800F2FDC - 1;
            func_8004F24C(0);
        }
    }
    if (D_800F3A40 & 0x4000) {
        if (D_800F2FDC < D_8009F6DC[a0]) {
            D_800F2FDC = D_800F2FDC + 1;
            func_8004F24C(0);
        }
    }
    func_800347BC();
}

void func_800348E8(int arg0) {
    int *t;
    int b;
    if (D_800F2FDC == 0) {
        D_800AC5D0 = 0x1E;
        return;
    }
    D_800AD4D8[0x19] = 0;
    D_800F4C64 = -1;
    if (arg0 == 1) {
        D_800AD4D8[0x19] = 9;
        D_800FF6B0 = 6;
        D_800F2A68 = -D_800F2EB8;
        if (D_800F2FDC == 2) {
            D_800F4D8A = 0;
            D_800F4C6A = D_8009F6E4[3] >> 8;
            D_800F3A26 = *(unsigned char *)&D_8009F6E4[3];
            return;
        }
        func_8003660C(D_800AD4D8, D_800F2FDC);
        return;
    }
    b = (int)D_8009F6E4;
    D_800FF6B0 = 6;
    t = (int *)((D_800F2FDC - 1) * 4 + arg0 * 8 + b);
    D_800F4C6A = *t >> 8;
    {
        int v = *(unsigned char *)t;
        D_800F4D8A = 0;
        D_800F3A26 = v;
    }
}

void func_800349F4(int a0, int a1) {
    V v[4];
    unsigned char rgb[4];
    int p;
    int i, j;
    unsigned char (*base)[4];
    unsigned char *t;

    p = func_8001FEC0(a0, *(int *)0x1F800000, 0x90, 0x5C, 0x1C, 8, 0x60, 0xA0, 0x7FF8);
    p = func_8001FEC0(a0, p, 0x90, 0x6A, 0x20, 8, 0xC4, 0xA0, 0x7FF8);
    if (D_800F4D44 == 1) {
        p = func_8001FEC0(a0, p, 0x90, 0x74, 0x1C, 8, 0x7C, 0xA0, 0x7FF8);
    } else {
        p = func_8001FEC0(a0, p, 0x90, 0x74, 0x24, 8, 0x9C, 0xA0, 0x7FF8);
        p = func_8001FEC0(a0, p, 0x90, 0x7E, 0x1C, 8, 0x7C, 0xA0, 0x7FF8);
    }
    p = func_8001FEC0(a0, p, 0x88, a1 * 10 + 0x6A, 8, 8, 0xE4, 0xA0, 0x7FF8);

    for (i = 0; i < 9; i++) {
        for (j = 0; j < 4; j++) {
            int kx = ((j & 1) << 5) + 0x70;
            v[j].x = ((i % 3) << 5) + kx;
            {
                int ky = ((j >> 1) << 4) + 0x56;
                v[j].y = ((i / 3) << 4) + ky;
            }
        }
        rgb[0] = 0xFF;
        rgb[2] = 0x20;
        rgb[1] = 0x20;
        j = i / 3;
        j = j == 1;
        base = D_8009F71C;
        t = base[j];
        p = func_8002071C(a0, p, v[t[0]], v[t[1]], v[t[2]], rgb, 1);
        rgb[0] = 0xFF;
        rgb[2] = 0xFF;
        rgb[1] = 0xFF;
        p = func_8002071C(a0, p, v[t[1]], v[t[2]], v[t[3]], rgb, 1);
    }
    *(int *)0x1F800000 = func_8001FCC8(a0, p, 5);

    for (j = 0; j < 4; j++) {
        if (j == 1 || j == 2) {
            v[j].x = 0xD0;
        } else {
            v[j].x = 0x70;
        }
        if (j >= 2) {
            v[j].y = 0x86;
        } else {
            v[j].y = 0x56;
        }
    }

    for (i = 0; i < 3; i++) {
        j = 0x40;
        func_80022694(a0, v[i].x, v[i].y, v[i + 1].x, v[i + 1].y, 0x100, 0, j, j, j, -1);
    }
}

void func_80034E14(void) {
    func_8009AEAC(1, 1, 1);
}

int func_80034E3C(void) {
    return (unsigned)(((unsigned)func_8009AEAC(0, 0, 0) >> 7) & 3) - 1 < 2;
}

int func_80034E70(int a0, unsigned int a1) {
    return a1 <= 0x1869E;
}

void func_80034E84(void) {
    func_8009AEAC(1, 3, 0x7E900);
    func_8009AEAC(1, 4, 1);
    D_800F2AAC = func_80096F24(D_800105C8, 2);
    D_800F2AA8 = func_80096F24(D_800105C8, 1);
    func_8009AEAC(4, 0, (int)func_80034E70);
    func_8009AEAC(2, 0, 0);
}

void func_80034F18(void) {
    func_8009AEAC(2, 3, 0);
    func_8009AEAC(2, 0, 0);
    func_80096F64(D_800F2AAC);
    func_80096F64(D_800F2AA8);
    func_8009AEAC(4, 0, 0);
}

void func_80034F80(void) {
    if (D_800FF688 == 2) {
        D_800FF688 = 3;
        func_8009AEAC(1, 1, 2);
    }
}

int func_80034FC0(void) {
    if (D_800FF688 == -1) {
        D_800F4C6A = 1;
        D_800F3A26 = 0x2D;
        D_800F4D8A = 0;
        func_8009AEAC(1, 1, 0);
        return 1;
    }
    return 0;
}

#include "types.h"
#include "m2c_macros.h"
#include "rr/bios_devices.h"

extern int func_80095D04();
extern int func_80095D70();
extern int func_80095C74();
extern int func_8009BA04();
extern int func_80096F04();
extern int func_80096E74();
extern int func_80096F14();
extern int func_80096EB4();
extern int D_8010EF58, D_8010EF5C, D_8010EF60, D_8010EF64, D_8010EF68, D_8010EF6C, D_8010EF70, D_8010EF74;
extern unsigned char D_800F3A24[];
extern int D_800ACC0C;
extern char D_800FFC48[];
extern int func_80095DA8();
extern int func_80096E84();
extern int func_8009BA94();
extern s32 D_8010EF58;
extern s32 D_8010EF5C;
extern s32 D_8010EF60;
extern s32 D_8010EF64;
extern s32 D_8010EF68;
extern s32 D_8010EF6C;
extern s32 D_8010EF70;
extern int func_80096EA4();
extern int D_8010ECC0[];
extern char D_800113D8[];
extern char D_8010ECD0[];
extern int func_80096484();
extern char *func_80096F74();

void func_80077E10();
void func_80078000(void);
int func_800780B8(int arg);
s32 func_80078178(void);
void func_80078204(void);
int func_8007825C(void);
void func_800782F4(void);
int func_8007834C(int a, int b);
int func_800783C8(int a0);
int func_80078414(int a0, unsigned char *a1);

void func_80077E10()
{
    func_80095D04();
    func_80095D70();
    func_80095C74();
    func_8009BA04();
    func_80096F04();
    D_8010EF58 = func_80096E74(0xF4000001, 4, 0x2000, 0);
    D_8010EF5C = func_80096E74(0xF4000001, 0x8000, 0x2000, 0);
    D_8010EF60 = func_80096E74(0xF4000001, 0x100, 0x2000, 0);
    D_8010EF64 = func_80096E74(0xF4000001, 0x2000, 0x2000, 0);
    D_8010EF68 = func_80096E74(0xF0000011, 4, 0x2000, 0);
    D_8010EF6C = func_80096E74(0xF0000011, 0x8000, 0x2000, 0);
    D_8010EF70 = func_80096E74(0xF0000011, 0x100, 0x2000, 0);
    D_8010EF74 = func_80096E74(0xF0000011, 0x2000, 0x2000, 0);
    func_80096F14();
    func_80096EB4(D_8010EF58);
    func_80096EB4(D_8010EF5C);
    func_80096EB4(D_8010EF60);
    func_80096EB4(D_8010EF64);
    func_80096EB4(D_8010EF68);
    func_80096EB4(D_8010EF6C);
    func_80096EB4(D_8010EF70);
    func_80096EB4(D_8010EF74);
    D_800F3A24[1] = 0x5A;
    D_800F3A24[0] = 0x5A;
    D_800ACC0C = (int)D_800FFC48;
}

void func_80078000(void) {
    func_80096F04();
    func_80096E84(D_8010EF58);
    func_80096E84(D_8010EF5C);
    func_80096E84(D_8010EF60);
    func_80096E84(D_8010EF64);
    func_80096E84(D_8010EF68);
    func_80096E84(D_8010EF6C);
    func_80096E84(D_8010EF70);
    func_80096E84(D_8010EF74);
    func_80096F14();
    func_8009BA94();
    func_80095DA8();
}

int func_800780B8(int arg) {
    int r;
    int t;
    r = func_80096EA4(D_8010EF58) == 1;
    if (func_80096EA4(D_8010EF5C) == 1) r = 2;
    if (func_80096EA4(D_8010EF60) == 1) r = 3;
    if (func_80096EA4(D_8010EF64) == 1) r = 4;
    t = D_8010ECC0[arg]++;
    if (t >= 0x78) r = 2;
    return r;
}

s32 func_80078178(void) {
    if (func_80096EA4(D_8010EF58) == 1) {
        return 1;
    }
    if (func_80096EA4(D_8010EF5C) == 1) {
        return 2;
    }
    if (func_80096EA4(D_8010EF60) != 1) {
        return (func_80096EA4(D_8010EF64) == 1) * 4;
    }
    return 3;
}

void func_80078204(void) {
    func_80096EA4(D_8010EF58);
    func_80096EA4(D_8010EF5C);
    func_80096EA4(D_8010EF60);
    func_80096EA4(D_8010EF64);
}

int func_8007825C(void) {
    for (;;) {
        if (func_80096EA4(D_8010EF68) == 1) return 1;
        if (func_80096EA4(D_8010EF6C) == 1) return 2;
        if (func_80096EA4(D_8010EF70) == 1) return 3;
        if (func_80096EA4(D_8010EF74) == 1) return 4;
    }
}

void func_800782F4(void) {
    func_80096EA4(D_8010EF68);
    func_80096EA4(D_8010EF6C);
    func_80096EA4(D_8010EF70);
    func_80096EA4(D_8010EF74);
}

int func_8007834C(int a, int b) {
    char sp[32];
    char *s0;
    int n = 0;
    func_80096484(sp, D_800113D8, a, b);
    s0 = D_8010ECD0;
    if (func_80097114(sp, s0) == s0) {
        do { n++; s0 += 0x28; } while (func_80096F74(s0) == s0);
        n = n;
    }
    return n;
}

int func_800783C8(int a0) {
    int a2 = 0, a1 = 0;
    char *v1;
    char *p = D_8010ECD0;
    int v;
    if (a0 > 0) { v1 = p; do { a1 += *(int *)(v1 + 0x18); v1 += 0x28; } while (++a2 < a0); }
    v = a1;
    if (v < 0) v += 0x1FFF;
    a1 = v >> 13;
    return 15 - a1;
}

int func_80078414(int a0, unsigned char *a1) {
    int t1 = 0, t2 = 0;
    char *t0;
    int a3, v1;
    unsigned char a2;
    char *b = D_8010ECD0;
    if (a0 <= 0) goto end;
    t0 = b;
outer:
    a3 = 0;
    v1 = a3;
top:
    a2 = a1[v1];
    if (a2 == 0) goto l64;
    if (a2 != (unsigned char)t0[v1]) goto l74;
    v1++;
    if (v1 < 0x15) goto top;
l64:
    t1++;
    if (a3 != 0) goto l7c;
    t2 = 1;
    goto end;
l74:
    a3++;
    goto l64;
l7c:
    t0 += 0x28;
    if (t1 < a0) goto outer;
end:
    return t2;
}

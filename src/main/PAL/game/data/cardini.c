/* RODATA 0x800113f0 0x1c */
#include "types.h"
#include "m2c_macros.h"
#include "rr/bios_devices.h"
#include "psyq/libcard/cardirq.h"
#include "psyq/libcard/cardsts.h"
#include "psyq/libcard/init.h"
#include "psyq/libc/printf.h"
#include "psyq/libcard/cardc162.h"
#include "psyq/libcard/init.h"
#include "psyq/libgpu/gpuctl.h"
#include "game/data/cardini.h"

extern int func_80095C74();
extern int func_80096F04();
extern int func_80096E74();
extern int func_80096F14();
extern int func_80096EB4();
extern int D_8010EF58;
extern int D_8010EF5C;
extern int D_8010EF60;
extern int D_8010EF64;
extern int D_8010EF68;
extern int D_8010EF6C;
extern int D_8010EF70;
extern int D_8010EF74;
extern unsigned char D_800F3A24[];
extern int D_800ACC0C;
extern char D_800FFC48[];
extern int func_80096E84();
extern int func_80096EA4();
extern int D_8010ECC0[];
extern char D_800113D8[];
extern char D_8010ECD0[];
extern char *func_80096F74();
extern int func_80096F24();
extern int func_80096F44();
extern int func_80096F64();
extern char D_800113E4[];
extern int D_8010EF78;
extern int func_80096F34();
extern int func_80096F54();
extern int func_80095C84();
extern int func_80095C94();
extern int D_8010ECB8[];
extern int D_8010ECC8[];
extern int D_8010EF80[];
extern int D_800A4094[];
extern char D_8010EF28[];
extern int func_80096198();

void func_80078000(void);
int func_800780B8(int arg);
void func_80078204(void);
int func_8007825C(void);
void func_800782F4(void);
int func_8007834C(int a, int b);
int func_80078414(int a0, unsigned char *a1);
int func_80078544(int a0, int a1, int a2, int a3, int a4, int a5);
int func_800786F8(int a0, int a1, int a2, int a3, int a4, int a5);
int func_800787C4(int p, char *a1);
void func_80078E1C(s32 arg0, s32 arg1);

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

int func_80078490(a0, a1, a2, a3, a4)
int a0, a1, a2, a3, a4;
{
    int sp[8];
    int r;
    int v;
    int ret;
    func_80096484(sp, D_800113E4, a0, a1, a2);
    v = a4 << 13;
    r = func_80096F24(sp, 0x8001);
    if (r != -1) {
        D_8010EF78 = r;
        func_80078204();
        while (func_80096F44(r, a3, v) == 0) {
        }
        ret = 0;
        func_80096F64(r);
    } else {
        ret = -1;
    }
    return ret;
}

int func_80078544(int a0, int a1, int a2, int a3, int a4, int a5) {
    char sp18[32];
    int h; register int r asm("$17"); int t;
    func_80096484(sp18, D_800113E4, a0, a1, a2);
    t = a4;
    h = func_80096F24(sp18, 0x8001);
    /* ASM_BARRIER: loads -1 into r after the call, not hoisted before it */
    __asm__ volatile("" : "=r"(r) : "0"(-1));
    if (h != r) {
        if (func_80096F34(h, a5, 0) != r) {
            D_8010EF78 = h;
            func_80078204();
            while (func_80096F44(h, a3, t) == 0) ;
            r = 0;
        }
        func_80096F64(h);
        return r;
    }
    /* ASM_BARRIER: legacy, reason not recorded (forces reg copy); retire & retest with fd.py */
    __asm__ volatile("" : "=r"(r) : "0"(r));
    return r;
}

int func_80078610(a0, a1, a2, a3, a4, a5)
int a0, a1, a2, a3, a4, a5;
{
    int sp[8];
    int r;
    int v;
    int ret;
    func_80096484(sp, D_800113E4, a0, a1, a2);
    if (a5 == 1) {
        r = func_80096F24(sp, (a4 << 16) | 0x200);
        if (r == -1) {
            return -1;
        }
        func_80096F64(r);
    }
    v = a4 << 13;
    r = func_80096F24(sp, 0x8002);
    if (r != -1) {
        D_8010EF78 = r;
        func_80078204();
        while (func_80096F54(r, a3, v) == 0) {
        }
        ret = 0;
        func_80096F64(r);
    } else {
        ret = -1;
    }
    return ret;
}

int func_800786F8(int a0, int a1, int a2, int a3, int a4, int a5) {
    char sp18[32];
    int h; register int r asm("$17"); int t;
    func_80096484(sp18, D_800113E4, a0, a1, a2);
    t = a4;
    h = func_80096F24(sp18, 0x8002);
    /* ASM_BARRIER: loads -1 into r after the call, not hoisted before it */
    __asm__ volatile("" : "=r"(r) : "0"(-1));
    if (h != r) {
        if (func_80096F34(h, a5, 0) != r) {
            D_8010EF78 = h;
            func_80078204();
            while (func_80096F54(h, a3, t) == 0) ;
            r = 0;
        }
        func_80096F64(h);
        return r;
    }
    /* ASM_BARRIER: re-launders r so the -1 return uses s1 (not the equal h in s0) */
    __asm__ volatile("" : "=r"(r) : "0"(r));
    return r;
}

int func_800787C4(int p, char *a1) {
    char *s2;
    int a0;
    int sp[2];
    int x;
    s2 = (char *)((p << 4) + (int)a1);
    switch (D_8010ECB8[p]) {
    case 0:
        func_80095C84(s2);
        D_8010ECB8[p] = 1;
        D_8010ECC0[p] = 0;
        D_8010ECC8[p] = 0;
        break;
    case 1:
        a0 = func_800780B8(p);
        if (a0 == 0) break;
        switch (a0) {
        case 1:
            D_8010EF80[p] = a0;
            D_8010ECB8[p] = 2;
            D_8010ECC0[p] = 0;
            break;
        case 3:
            D_8010EF80[p] = -1;
            D_8010ECB8[p] = 6;
            D_800A4094[p] = 0;
            break;
        case 4:
            D_8010EF80[p] = 2;
            D_8010ECB8[p] = 2;
            D_8010ECC0[p] = 0;
            break;
        case 2:
        default:
            D_8010EF80[p] = -4;
            D_8010ECB8[p] = 6;
            D_800A4094[p] = 0;
            break;
        }
        break;
    case 2:
        if (func_8009D700(s2, p * 17 + (int)D_8010EF28)) {
            D_8010ECB8[p] = 3;
            D_8010ECC0[p] = 0;
        } else {
            if (D_8010ECC0[p]++ >= 0x78) {
                D_8010EF80[p] = -4;
                D_8010ECB8[p] = 6;
                D_800A4094[p] = 0;
            }
        }
        break;
    case 3:
        a0 = func_8009BF94(1, &sp[0], &sp[1]);
        if (a0 != 0) {
            if (a0 > 0) {
                a0 = sp[1];
                switch (a0) {
                case 0:
                    D_8010EF80[p] = 3;
                    if (D_800A4094[p] == 1) {
                        D_8010ECB8[p] = 6;
                    } else {
                        D_8010ECB8[p] = 4;
                    }
                    break;
                case 2:
                    if (D_8010EF80[p] == 1) {
                        if (D_800A4094[p] == 1) {
                            D_8010ECB8[p] = 6;
                        } else {
                            D_8010ECB8[p] = 4;
                        }
                    } else {
                        D_8010EF80[p] = 2;
                        func_800782F4();
                        func_80095CA4(s2);
                        func_8007825C();
                        D_8010ECB8[p] = 4;
                        D_800A4094[p] = 0;
                    }
                    break;
                case 1:
                    D_8010EF80[p] = -1;
                    D_8010ECB8[p] = 6;
                    D_800A4094[p] = 0;
                    break;
                case 3:
                    D_8010EF80[p] = 4;
                    func_800782F4();
                    func_80095CA4(s2);
                    func_8007825C();
                    D_8010ECB8[p] = 4;
                    D_800A4094[p] = 0;
                    break;
                }
            } else {
                D_8010EF80[p] = -4;
                D_8010ECB8[p] = 6;
                D_800A4094[p] = 0;
            }
        } else {
            if (D_8010ECC0[p]++ >= 0x78) {
                D_8010EF80[p] = -4;
                D_8010ECB8[p] = 6;
                D_800A4094[p] = 0;
            }
        }
        break;
    case 4:
        func_80078204();
        func_80095C94(s2);
        D_8010ECB8[p] = 5;
        D_8010ECC0[p] = 0;
        break;
    case 5:
        a0 = func_800780B8(p);
        if (a0 == 0) break;
        D_8010ECB8[p] = 6;
        switch (a0) {
        case 1:
            D_800A4094[p] = a0;
            break;
        case 3:
            D_8010EF80[p] = -1;
            D_800A4094[p] = 0;
            break;
        case 4:
            x = D_8010EF80[p];
            if ((unsigned)(x - 1) < 2) {
                D_8010EF80[p] = -2;
            } else if ((unsigned)(x - 3) < 2) {
                D_8010EF80[p] = -3;
            }
            D_800A4094[p] = 0;
            break;
        case 2:
        default:
            D_8010EF80[p] = -4;
            D_800A4094[p] = 0;
            break;
        }
        break;
    case 6:
        D_8010ECC8[p] = D_8010EF80[p];
        D_8010ECB8[p] = 0;
        break;
    }
    return D_8010ECC8[p];
}

void func_80078E1C(s32 arg0, s32 arg1) {
    func_80096198((arg0 * 0x10) + arg1);
}

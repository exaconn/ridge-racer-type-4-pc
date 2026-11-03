/* RODATA 0x800113f0 0x1c */
#include "types.h"
#include "m2c_macros.h"

extern int func_80078204();
extern int func_80096484();
extern int func_80096F24();
extern int func_80096F44();
extern int func_80096F64();
extern char D_800113E4[];
extern int D_8010EF78;
extern int func_80096F34();
extern int func_80096F54();
extern int func_80095C84();
extern int func_800780B8();
extern int func_8009D700();
extern int func_8009BF94();
extern int func_800782F4();
extern int func_80095CA4();
extern int func_8007825C();
extern int func_80095C94();
extern int D_8010ECB8[], D_8010ECC0[], D_8010ECC8[], D_8010EF80[], D_800A4094[];
extern char D_8010EF28[];
extern int func_80096198();

int func_80078490();
int func_80078544(int a0, int a1, int a2, int a3, int a4, int a5);
int func_80078610();
int func_800786F8(int a0, int a1, int a2, int a3, int a4, int a5);
int func_800787C4(int p, char *a1);
void func_80078E1C(s32 arg0, s32 arg1);

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

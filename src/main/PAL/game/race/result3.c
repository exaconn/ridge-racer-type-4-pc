#include "types.h"
#include "m2c_macros.h"

typedef struct { int k; short f; } E;

struct w3g2s { char pad[0x17]; signed char a; char b; signed char c; short d; };

extern int func_8002A344();
extern int func_8003A198();
extern u32 D_800F2DE4;
extern s16 D_800F3A26;
extern s32 D_800F3A60;
extern s16 D_800F4C6A;
extern s16 D_800F4D8A;
extern s32 D_8010C0E0;
extern int func_8002B574();
extern signed char D_800AD4D8[];
extern signed char D_800AD4EF;
extern E *D_800F2DE8[];
extern int D_800F39C8, D_800F4C64;
extern short D_800F4D44;
extern unsigned char D_8009F72C[];

void func_80036270(void);
void func_80036330(void);
void func_8003645C(void);
void func_80036538(void);
void func_800365BC(void);
void func_800365E4(void);
void func_8003660C(void);

void func_80036270(void) {
    s32 temp_s0;

    D_800F2DE4 += 1;
    func_8002A344(1);
    if ((u32) D_800F2DE4 < 0x1C2U) {
        if (D_800F3A60 != 0) {
            goto block_2;
        }
        goto block_5;
    }
block_2:
    if (D_8010C0E0 <= 0) {
        D_8010C0E0 += 1;
        D_800F2DE4 = 0;
        return;
    }
block_5:
    temp_s0 = D_8010C0E0;
    if (temp_s0 == 1) {
        if (D_800F3A60 != 0) {
            D_800F2DE4 = 0;
            func_8003A198();
            D_800F4C6A = (s16) temp_s0;
            D_800F3A26 = 0xB;
            D_800F4D8A = 0;
        }
    }
}

void func_80036330(void) {
    int a2;
    int a0;
    int a1;
    a2 = D_800F39C8 + 0x1E8;
    if (D_800AD4EF >= 0) {
        a1 = 0;
        if (D_800AD4EF >= 2) {
            a1 = 2;
            if (D_800AD4EF >= 4) {
                goto b3;
            }
        }
    } else {
b3:
        a1 = 4;
    }
    a0 = 0;
    do {
        if (D_800F2DE8[a0]->f == 0) {
            D_800AD4D8[D_800AD4D8[0x17]] = a0 + 1;
            if (a0 == 0) {
                if (a1 < 4) {
                    if (func_8002B574(*(int *)(a2 + ((D_800AD4D8[0x17] - a1) << 2)), 0, a2) > D_800F4C64) {
                        D_800AD4D8[D_800AD4D8[0x17]] = 0;
                    }
                }
            }
            break;
        }
        a0 += 1;
    } while (a0 < 8);
    if (D_800AD4D8[0x19] < 9) {
        D_800AD4D8[0x19] = D_800AD4D8[D_800AD4D8[0x17]];
    }
}

void func_8003645C(void) {
    int i, j;
    int t[2];
    for (i = 0; i < 7; i++) {
        for (j = 7; i < j; j--) {
            t[0] = D_800F2DE8[j - 1]->k;
            t[1] = D_800F2DE8[j]->k;
            if (t[0] < t[1] || (t[0] == t[1] && D_800F2DE8[j]->f == 0)) {
                E *x = D_800F2DE8[j - 1];
                E *y = D_800F2DE8[j];
                D_800F2DE8[j] = x;
                D_800F2DE8[j - 1] = y;
            }
        }
    }
    if (D_800F4D44 == 1) {
        func_80036330();
    }
}

void func_80036538(void) {
    struct w3g2s *p;
    if (D_800F4D44 == 1) {
        p = &D_800AD4D8;
        if (p->c > D_8009F72C[p->a / 2]) p->d = 5;
        else p->d = 4;
    }
    D_800F4C6A = 1;
    D_800F3A26 = 0x17;
    D_800F4D8A = 0;
}

void func_800365BC(void) {
    func_8003645C();
    func_80036538();
}

void func_800365E4(void) {
    func_8003645C();
    func_80036538();
}

void func_8003660C(void) {
    func_800365E4();
}

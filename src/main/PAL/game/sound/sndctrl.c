/* RODATA 0x80011118 0xc */
#include "types.h"
#include "m2c_macros.h"

extern int func_8004EE08();
extern int func_8004F2A0();
extern int func_8004FBF0();
extern int func_80050E58();
extern int func_80051248();
extern int func_800514FC();
extern int func_800517D8();
extern int func_80051B28();
extern int func_80051B44();
extern void func_8007F910(int);
extern int func_80083C74();
extern int func_80084534();
extern int func_8008A674();
extern struct {
    char pad00[0x24];
    short h24[0x1A];
    char pad0[0x1A08-0x24-0x34];
    int f1a08;
    int a1a0c[7];
    char pad2[0x1A2C - 0x1A28];
    int f1a2c;
    int f1a30;
    char pad3[0x2864 - 0x1A34];
    int f2864;
    int f2868;
} D_800F4DB8;
extern int func_80086554();
extern int func_800865E4();
extern int func_800867A4();
extern int func_80050028();
extern int func_80052C70();
extern s32 D_800F4DBC;
extern int func_8008A634();
extern int func_8008A624();
extern int func_8008C4FC();

void func_8004F82C(int a0, int a1);
void func_8004F9C8(void);
void func_8004FA04(void);
void func_8004FA50(void);
void func_8004FA98(int a0);

void func_8004F82C(int a0, int a1) {
    int a[3] = { 2, 3, 7 };
    int i;
    int *p;
    char *g;
    func_80084534(0);
    if (D_800F4DB8.f2864 >= 0) {
        func_80083C74((short)D_800F4DB8.f2864);
        func_8007F910((short)D_800F4DB8.f2864);
        D_800F4DB8.f2864 = -1;
    }
    for (i = 0; i < 3; i++) {
        if (D_800F4DB8.h24[a[i]] >= 0) {
            func_8008A674(D_800F4DB8.h24[a[i]]);
            D_800F4DB8.h24[a[i]] = -1;
        }
    }
    func_8004FBF0();
    func_80050E58();
    func_80051248(a0);
    func_800514FC();
    func_8004F2A0();
    func_800517D8();
    func_8004EE08();
    if (a0 == 0) {
        func_80051B44(a1 & 0xFFFF, 0);
    } else {
        func_80051B44(a1 & 0xFFFF, 1);
    }
    func_80051B28();
    func_80084534(0);
    D_800F4DB8.f1a08 = 0;
    i = 6;
    do {
        D_800F4DB8.a1a0c[i] = 1;
        i--;
    } while (i >= 0);
    D_800F4DB8.f1a30 = 0;
    D_800F4DB8.f1a2c = 0;
    D_800F4DB8.f2868 = -1;
}

void func_8004F9C8(void) {
    func_80084534(0);
    func_800867A4();
    func_800865E4(0);
    func_80086554(0, 0);
}

void func_8004FA04(void) {
    s32 var_a0;
    s32 var_s0;

    D_800F4DBC = 1;
    func_80052C70();
    var_s0 = 0;
    var_a0 = 0;
    do {
        func_80050028(var_a0, 1);
        var_s0 += 1;
        var_a0 = var_s0;
    } while (var_s0 < 4);
}

void func_8004FA50(void) {
    s32 var_a0;
    s32 var_s0;

    D_800F4DBC = 0;
    func_80052C70();
    var_s0 = 0;
    var_a0 = 0;
    do {
        func_80050028(var_a0, 0);
        var_s0 += 1;
        var_a0 = var_s0;
    } while (var_s0 < 4);
}

void func_8004FA98(int a0) {
    u8 b[4];
    u8 *p;
    if (a0 == 0) {
        func_8008A634();
        p = b;
        b[0] = 0x80; b[1] = 0; b[2] = 0x80; b[3] = 0;
    } else {
        func_8008A624();
        p = b;
        b[0] = 0x64; b[1] = 0x64; b[2] = 0x64; b[3] = 0x64;
    }
    func_8008C4FC(p);
}

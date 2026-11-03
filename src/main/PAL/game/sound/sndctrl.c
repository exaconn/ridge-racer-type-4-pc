/* RODATA 0x80011118 0xc */
#include "types.h"
#include "m2c_macros.h"
#include "game/sound/sndbgm3.h"
#include "game/sound/sndevt.h"
#include "game/sound/sndloop2.h"
#include "game/sound/sndown.h"
#include "game/sound/sndrst2.h"
#include "game/sound/sndslot.h"
#include "game/sound/sndvab.h"
#include "psyq/libcd/s_2.h"
#include "psyq/libsnd/ssmvol.h"
#include "psyq/libsnd/utrev3.h"
#include "psyq/libsnd/vmsel.h"
#include "psyq/libsnd/vmvinit.h"

struct E { char pad[0x38]; u32 flags; char r[1568 - 0x3C]; };

extern void func_8007F910(int);
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
extern int func_80052C70();
extern s32 D_800F4DBC;
extern int func_80052AB8();
extern void func_80052B8C(int, short);

void func_8004F82C(int a0, int a1);
void func_8004F9C8(void);
void func_8004FA04(void);
void func_8004FA50(void);
void func_8004FA98(int a0);
void func_8004FB00(void);
void func_8004FBF0(void);
void func_8004FC18(short arg0, short arg1);

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

/* legacy typing of shared globals in this part of the unit */
#define D_800F4DB8 (*(struct E (*)[])&D_800F4DB8)

void func_8004FB00(void) {
    int i;
    struct E *p;
    for (i = 0, p = D_800F4DB8; i < 4; i++, p++) {
        if (p->flags & 1) func_8004FFA4(i);
    }
    for (i = 0; i < 2; i++) func_80050E94(0, 0, 0, i);
    for (i = 0; i < 2; i++) func_800512C4(i, 0, 0);
    func_80051538(0, 0, 0);
    func_800517F8(5, 0x1E00, 0, 0x40);
    func_80052AB8();
    func_8004F9C8();
    func_80051B38();
    D_800F4DBC = 1;
}

#undef D_800F4DB8

/* legacy typing of shared globals in this part of the unit */
#define D_800F4DB8 (*(struct { char pad[0x18]; s16 a, b, c, d, e, f; } (*))&D_800F4DB8)

void func_8004FBF0(void) {
    D_800F4DB8.a = 0x80;
    D_800F4DB8.b = 0;
    D_800F4DB8.c = 0;
    D_800F4DB8.d = 0;
    D_800F4DB8.e = 0;
    D_800F4DB8.f = 0;
}

#undef D_800F4DB8

/* legacy typing of shared globals in this part of the unit */
#define D_800F4DB8 (*(struct { char pad[0x18]; unsigned short a; short b; short c; unsigned short d; unsigned short e; } (*))&D_800F4DB8)

void func_8004FC18(short arg0, short arg1) {
    if (arg1 >= 2) {
        D_800F4DB8.b = arg1;
        D_800F4DB8.c = 0;
        D_800F4DB8.d = arg0;
        D_800F4DB8.e = D_800F4DB8.a;
    } else {
        D_800F4DB8.a = arg0;
        D_800F4DB8.b = 0;
        D_800F4DB8.c = 0;
        D_800F4DB8.d = 0;
        D_800F4DB8.e = 0;
        if (arg0 == 0) {
            func_8004FB00();
        }
    }
    func_80052B8C(arg0 & 0xFFFF, arg1);
}

#undef D_800F4DB8

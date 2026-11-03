#include "types.h"
#include "m2c_macros.h"
#include "game/ui/window2.h"
#include "game/race/objsim.h"
#include "psyq/libgpu/primattr.h"
#include "rr/hud_number.h"
#include "rr/fade_sprite.h"
#include "rr/audio_loop.h"

struct S { char pad[0x2AA]; short f; char pad2[0x320 - 0x2AC]; };

extern s32 func_8001FCC8();
extern s32 D_800F4BF0;
extern int D_800ABEB0;
extern short D_80107178;
extern short D_800AC59C;
extern signed char D_800AD4EF;
extern unsigned char D_8009EE40[];
extern unsigned char D_8009EE44[];
extern int func_80090424();
extern struct S D_800ABEF8[];
extern int D_800F3A30;

void func_800219BC(s32 arg0, s32 arg1);
void func_80021A7C();
void func_80021C00(int a0);

void func_800219BC(s32 arg0, s32 arg1) {
    s32 temp_s0;
    s32 temp_v1;
    char *temp_a3;
    char *b;
    s32 cur, m, r;
    s32 a = arg0;
    b = D_8010BF88;
    temp_v1 = arg1 * 0x14;
    temp_a3 = b + temp_v1;
    cur = *(s32 *)0x1F800000;
    temp_s0 = *(s32 *)(D_800F4BF0 * 4 + temp_v1 + b);
    m = a % 10;
    r = func_800215F4(temp_s0, cur, M2C_FIELD(temp_a3, s16 *, 0xC), M2C_FIELD(temp_a3, s16 *, 0xE), m, 0x7EFB);
    *(s32 *)0x1F800000 = func_8001FCC8(temp_s0, r, 5);
}

void func_80021A7C(a0, a1) int a0; int a1; {
    int base, a, s0, s1, v;
    char *s2, *p2;
    base = D_800ACC14;
    s2 = (char *)(base + 0x226B0);
    *(char *)(a1 * 0x78 + (int)s2 + 0xC) = (a0 + 1) * 0x18;
    p2 = (char *)(base + 0x226D8);
    if (D_800F4D44 == 4) {
        a = D_80107178;
    } else {
        if (D_800F4D44 == 3) {
            a = 2;
        } else {
            a = D_800AC59C;
        }
    }
    *(char *)(a1 * 0x78 + (int)p2 + 0xC) = D_8009EE44[a - 1];
    if (D_800F4D44 == 1) {
        s0 = 0x40;
        s1 = 0;
        if (D_800AC0E6 <= D_8009EE40[D_800AD4EF / 2]) {
            s1 = 1;
            v = func_80090424((D_800ABEB0 & 0x3F) << 6);
            if (v < 0) v += 0x3F;
            s0 = v >> 6;
            if (s0 < 0) {
                s1 = 0;
                s0 = -s0;
            }
        }
        s0 += 0x40;
        *(short *)(s2 + 0xE) = s1 + 0x7EF8;
        func_80095B24(s2, 0);
        s2[4] = s0;
        s2[5] = s0;
        s2[6] = s0;
    }
}

/* legacy typing of shared globals in this part of the unit */
#define D_800ACC14 ((char *)D_800ACC14)

void func_80021C00(int a0) {
    int a1, a2;
    char *p;
    a2 = a0 - 1;
    if (a0 == 0) {
        a1 = 7;
        a2 = 0;
    } else {
        a1 = a2 * 6 + 1;
    }
    p = D_800ACC14 + (a1 * 0x14 + 0x22688);
    if (D_800ABEF8[a2].f < D_800F3A30) {
        *(short *)(p + 0x10) = 0;
        return;
    }
    *(short *)(p + 0x10) = 0x14;
}

#undef D_800ACC14

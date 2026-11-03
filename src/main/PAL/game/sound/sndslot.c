#include "types.h"
#include "rr/audio_loop.h"
#include "m2c_macros.h"

typedef struct { short pad[0xC]; short o; short n; short c; short to; short from; } T;

struct E { char pad[0x38]; u32 flags; char p[0xC]; s32 a, b, c; char r[1568 - 0x54]; };

struct S { int a; int b; int c; int d; char pad[0x610]; };

extern void func_8004FB00();
extern void func_80052B8C(int, short);
extern int func_800534D8();
extern int func_80053D24();
extern int func_80052548();
extern int func_800501BC();
extern int func_80050EC4();
extern int func_80051378();
extern int func_80051554();
extern int func_8004F418();
extern s32 D_800ACBC8;
extern s32 D_800F4DC8;
extern int func_800500A8();
extern int func_80050028();
extern struct S D_800F4DF0[];

void func_8004FBF0(void);
void func_8004FC18(short arg0, short arg1);
void func_8004FCB4(void);
void func_8004FD80(void);
void func_8004FE7C(s32 a0, int a1, int a2);
void func_8004FED4(void);
void func_8004FF34(int a0);
void func_8004FFA4(int arg0);

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

void func_8004FCB4(void) {
    T *p = &D_800F4DB8;
    int n = p->n;
    int v = 0x80;
    int c;
    short c1;
    if (n > 0) {
        c = (unsigned short)p->c;
        if (p->c < n) {
            n = ((p->to - p->from) << 12) / n;
            c1 = c + 1;
            p->c = c1;
            v = (unsigned short)p->from + n * c1 / 4096;
        } else {
            p->n = 0;
            func_8004FB00(n, p, c);
            return;
        }
    }
    p->o = v;
}

#undef D_800F4DB8

/* legacy typing of shared globals in this part of the unit */
#define D_800F4DB8 (*(struct { char p0[0x286C]; s16 x286C; s16 x286E; s16 x2870; char p1[0x2]; s32 x2874; s32 x2878; } (*))&D_800F4DB8)

void func_8004FD80(void) {
    int i;
    u32 *p;
    int a;
    if (D_800F4DB8.x2878 != D_800F4DB8.x2874) {
        a = D_800F4DB8.x2874 - 1;
        D_800F4DB8.x286C = 1;
        if ((a & 1) == 0) D_800F4DB8.x286E = 1;
        if ((a & 3) == 0) D_800F4DB8.x2870 = 1;
    } else {
        D_800F4DB8.x286C = 0;
        D_800F4DB8.x286E = 0;
        D_800F4DB8.x2870 = 0;
    }
    i = 0;
    D_800F4DB8.x2878 = D_800F4DB8.x2874;
    func_800534D8();
    func_80053D24();
    func_8004FCB4();
    func_80052548();
    for (p = (u32 *)((char *)&D_800F4DB8 + 0x38); i < 4; i++, p += 0x188) {
        if (*p & 1) func_800501BC(i);
    }
    func_80050EC4();
    func_80051378();
    func_80051554();
    func_8004F418();
    func_800518E0();
}

#undef D_800F4DB8

void func_8004FE7C(s32 a0, int a1, int a2) {
    int v = D_800F4DC0;
    func_80086B00((s16)a0, (s16)((s32)((u32)a1 * (u32)v) >> 7), (s16)((s32)((u32)a2 * (u32)v) >> 7));
}

void func_8004FED4(void) {
    M2C_UNK var_a0;

    func_8004FF34(0);
    if (D_800F4DC8 == 0) {
        if (D_800ACBC8 >= 0) {
            func_8004FF34(2);
            var_a0 = 3;
            goto block_4;
        }
    } else {
        var_a0 = 1;
block_4:
        func_8004FF34(var_a0);
    }
}

/* legacy typing of shared globals in this part of the unit */
#define D_800F4DB8 (*(struct E (*)[])&D_800F4DB8)
#define func_80085AEC ((void (*)(short))func_80085AEC)

void func_8004FF34(int a0) {
    struct E *base = D_800F4DB8;
    struct E *e = base + a0;
    e->flags |= 1;
    func_800500A8(a0);
    e->c = e->b;
    func_80050028(a0, 0);
}

void func_8004FFA4(int arg0) {
 int i; int s0; struct S *t = &D_800F4DF0[arg0]; s0 = t->c; i = 0; while (i < t->d) { func_80085AEC(s0); i++; s0++; }
 t->a = 0;
}

#undef D_800F4DB8
#undef func_80085AEC

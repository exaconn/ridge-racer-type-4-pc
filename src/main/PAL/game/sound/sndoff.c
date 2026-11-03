#include "types.h"

struct E { char pad[0x38]; u32 flags; char p[0xC]; s32 a, b, c; char r[1568 - 0x54]; };

struct S { int a; int b; int c; int d; char pad[0x610]; };

extern int func_800500A8();
extern int func_80050028();
extern struct E D_800F4DB8[];
extern void func_80085AEC(short);
extern struct S D_800F4DF0[];

void func_8004FF34(int a0);
void func_8004FFA4(int arg0);

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

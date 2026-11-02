#include "types.h"
extern int func_800500A8();
extern int func_80050028();
struct E { char pad[0x38]; u32 flags; char p[0xC]; s32 a, b, c; char r[1568 - 0x54]; };
extern struct E D_800F4DB8[];
void func_8004FF34(int a0) {
    struct E *base = D_800F4DB8;
    struct E *e = base + a0;
    e->flags |= 1;
    func_800500A8(a0);
    e->c = e->b;
    func_80050028(a0, 0);
}

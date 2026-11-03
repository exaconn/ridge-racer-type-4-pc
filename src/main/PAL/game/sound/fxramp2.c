#include "types.h"
struct S { char pad[0x2874]; u32 a; char p[8]; s32 b; };
extern struct S D_800F4DB8;
void func_80053DE8(int a0, int a1, s16 a2) {
    struct S *p = &D_800F4DB8;
    int v = a2;
    p->b = v;
    if ((u32)v < 100) {
        p->a++;
        if (p->a > 0x61A7F) p->a = 0;
    }
}

#include "types.h"
extern int func_800841E4();
extern u8 D_800AD558;
struct S { char pad[0x2864]; s16 a; char p[0x2888 - 0x2866]; s16 b; };
extern struct S D_800F4DB8;
void func_80053A9C(s16 a0) {
    struct S *p = &D_800F4DB8;
    s16 v;
    p->b = a0;
    v = (a0 * D_800AD558) / 16;
    func_800841E4(p->a, v, v);
}

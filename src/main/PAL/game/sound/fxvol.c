#include "types.h"

struct S { char pad[0x2864]; s16 a; char p[0x2888 - 0x2866]; s16 b; };

extern struct { char pad[0x288A]; s16 a, b, c, d; } D_800F4DB8;
extern int func_800841E4();
extern u8 D_800AD558;

void func_80053A80(void);
void func_80053A9C(s16 a0);

void func_80053A80(void) {
    D_800F4DB8.a = 0;
    D_800F4DB8.b = 0;
    D_800F4DB8.c = 0;
    D_800F4DB8.d = 0;
}

void func_80053A9C(s16 a0) {
    struct S *p = &D_800F4DB8;
    s16 v;
    p->b = a0;
    v = (a0 * D_800AD558) / 16;
    func_800841E4(p->a, v, v);
}

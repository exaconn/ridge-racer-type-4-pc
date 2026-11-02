#include "types.h"
extern int func_800867A4();
extern int func_800865E4();
extern int func_800867C4();
extern struct { char pad[0x2894]; s32 a, b, c; } D_800F4DB8;
void func_8004EE90(int a0, int a1) {
    func_800867A4();
    if (a0 > 0) {
        D_800F4DB8.a = a0;
        D_800F4DB8.b = a1;
        D_800F4DB8.c = a1;
        func_800865E4((s16)a0);
        func_800867C4();
    } else {
        D_800F4DB8.a = 0;
        D_800F4DB8.b = 0;
        D_800F4DB8.c = 0;
    }
}

#include "types.h"
extern int func_800867A4();
extern int func_800865E4();
extern int func_800867C4();
extern int func_80086554();
extern struct { char pad[0x2894]; s32 a, b, c, d; } D_800F4DB8;
void func_8004EE08(void) {
    D_800F4DB8.a = 0;
    D_800F4DB8.b = 0;
    D_800F4DB8.c = 0;
    D_800F4DB8.d = 0xFFFFFF;
    func_800867A4();
    func_800865E4(2);
    func_800867C4();
    func_80086554(0, 0);
}

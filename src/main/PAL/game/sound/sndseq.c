#include "types.h"
#include "m2c_macros.h"

extern int func_800867A4();
extern int func_800865E4();
extern int func_800867C4();
extern int func_80086554();
extern struct { char pad[0x2894]; s32 a, b, c, d; } D_800F4DB8;
extern s32 D_800F7650;

void func_8004EE08(void);
void func_8004EE60(s32 arg0);
void func_8004EE90(int a0, int a1);

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

void func_8004EE60(s32 arg0) {
    D_800F7650 = arg0;
    func_80086554((s16) arg0, (s16) arg0);
}

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

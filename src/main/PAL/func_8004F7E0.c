#include "types.h"
extern int func_80084534();
extern int func_8008A644();
extern int func_8004F82C();
extern struct { char pad[4]; s32 a; char p[4]; s32 b; s32 c; } D_800F4DB8;
extern s32 D_800AC66C;
void func_8004F7E0(void) {
    func_80084534(0);
    func_8008A644(4);
    D_800F4DB8.b = 1;
    D_800F4DB8.a = 0;
    func_8004F82C(D_800F4DB8.c, 0);
    D_800AC66C = 0;
}

#include "types.h"
extern struct { char pad[0x48]; struct { s32 a, b; char pad[1568 - 8]; } e[1]; } D_800F4DB8;
void func_8005007C(int a0, int a1, int a2) {
    D_800F4DB8.e[a2].a = a0;
    D_800F4DB8.e[a2].b = a1;
}

#include "types.h"
extern void func_80036068(void), func_8003CAB4(void);
extern void func_800207D4(void *, s32, s32);
extern s16 D_800F4C6A, D_800F3A26, D_800F4D8A;
extern u8 *D_800ACC14;
void ovl666_func_801166D0(void)
{
    func_80036068();
    func_8003CAB4();
    D_800F4C6A = 1;
    D_800F3A26 = 82;
    D_800F4D8A = 0;
    func_800207D4(D_800ACC14 + 112, 255, 69);
}

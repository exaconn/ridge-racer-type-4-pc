#include "types.h"
extern void func_8002A604(void);
extern void func_8003D228(s32, s32, s32, s32, s32, s32, s32);
extern void func_8002AF8C(s32, s32, s32);
extern u8 D_800ACC68;
extern s16 *D_800F3AFC;
void ovl665_func_80115C8C(void)
{
    func_8002A604();
    D_800ACC68 = 2;
    func_8003D228(0, 12, 304, 296, 88, 0, 0);
    D_800F3AFC[1] = 1;
    func_8002AF8C(1, 1, 0);
}

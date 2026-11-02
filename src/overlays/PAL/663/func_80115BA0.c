#include "types.h"
extern void func_80013890(s32);
extern void func_8002E4F4(void *);
extern void func_80037110(void);
extern s32 D_800F2DE4;
extern s32 D_800ABEF8[];
extern s16 *D_800F3AFC;
void ovl663_func_80115BA0(void)
{
    s32 *state;
    s32 *scratch;
    D_800F2DE4 = 0;
    func_80013890(5);
    state = D_800ABEF8;
    scratch = (s32 *)0x1F800000;
    scratch[6] = 120;
    scratch[2] = -16;
    scratch[3] = -248;
    scratch[7] = 0;
    scratch[8] = 0;
    scratch[4] = -96;
    func_8002E4F4(state);
    state[10] = 308;
    state[9] = 10;
    state[21] = 2560;
    func_80037110();
    D_800F3AFC[1] = 1;
}

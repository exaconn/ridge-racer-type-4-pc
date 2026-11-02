#include "rr/overlay666_checkpoint.h"

extern void func_800295B8(s32);
extern void func_8004FC18(s16, s16);
extern void *func_8004F2D8(s32, s32);

extern s32 D_800F2EB8;
extern s32 D_800F3A30;
extern s16 D_8010C134;
extern u16 D_800F3914;
extern s32 D_800FF6B0;
extern s16 D_800AC5D0;
extern s32 D_800F4C64;

void ovl666_func_8011656C(RrRaceCheckpointVehicle *arg0)
{
    RrRaceVehicleControl *sub;
    /* Preserve the target register through the mode-dependent adjustment. */
    register s32 target __asm__("$3");
    s32 id;

    sub = &arg0->control;
    if ((s32)((u32)arg0->second + (u32)arg0->first) >= (s32)((u32)(s32)sub->steering94.count22 * (u32)D_800F2EB8)) {
        if (sub->steering94.count22 <= D_800F3A30) {
            sub->steering94.count22=(s16)((u16)sub->steering94.count22+1U);
            D_8010C134 = 0;
            target = D_800F3A30;
            if ((u16)(D_800F3914 - 6) < 2) {
                target=(s32)((u32)target-1U);
            }
            if (sub->steering94.count22 == target) {
                func_800295B8(0);
            } else if (sub->steering94.count22 > D_800F3A30) {
                if (D_800FF6B0 < 4) {
                    D_8010C134 = -1;
                    D_800FF6B0 = 4;
                    D_800AC5D0 = 75;
                    func_8004FC18(0, 33);
                    D_800F4C64 = 0;
                    func_8004F2D8(19, 1);
                }
            }
        }
    }
    if (D_8010C134 == 0) {
        id = (s32)(9U - ((u32)D_800F3A30 - (u32)(s32)sub->steering94.count22));
        if (id == 9) {
            id = 15;
        }
        if (id >= 6) {
            func_8004F2D8(id, 0);
        }
        D_8010C134 = -1;
    }
}

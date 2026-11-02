#include "rr/overlay667_checkpoint.h"

extern s32 D_800F2EB8;
extern s32 D_800F3A30;
extern s32 D_800FF6B0;
extern s32 D_800F4C64;
extern s16 D_800AC5D0;
extern void func_800295B8(s32);
extern void func_8004FC18(s16, s16);
extern void *func_8004F2D8(s32, s32);

void ovl667_func_80115508(RrRaceCheckpointVehicle *arg0) {
    RrRaceVehicleControl *sub = &arg0->control;
    /* MIPS sums and products retain their low 32 bits. */
    if ((s32)((u32)arg0->second + (u32)arg0->first) >= (s32)((u32)(s32)sub->steering94.count22 * (u32)D_800F2EB8)) {
        if (sub->steering94.count22 <= D_800F3A30) {
            sub->steering94.count22=(s16)((u16)sub->steering94.count22+1U);
            if (sub->steering94.count22 == D_800F3A30) {
                func_800295B8(0);
            } else if (sub->steering94.count22 > D_800F3A30) {
                if (D_800FF6B0 < 4) {
                    D_800F4C64 = 1;
                    D_800FF6B0 = 4;
                    D_800AC5D0 = 75;
                    func_8004FC18(0, 71);
                    func_8004F2D8(19, 1);
                }
            }
        }
    }
}

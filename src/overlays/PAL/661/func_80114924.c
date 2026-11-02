#include "rr/overlay661_checkpoint.h"

extern s32 D_800F2EB8;
extern s32 D_800F3A30;
extern s16 D_8010C134;
extern s32 D_800FF6B0;
extern s16 D_800AC5D0;
extern s32 D_800F4C64;
extern s32 D_800ABEC0;

extern void func_800295B8(s32);
extern void func_8004FC18(s16, s16);

void ovl661_func_80114924(RrRaceCheckpointVehicle *vehicle) {
    RrRaceVehicleControl *control = &vehicle->control;
    s16 counter;
    s32 target;
    s32 message;

    counter = control->steering94.count22;
    if (((s32)((u32)vehicle->second + (u32)vehicle->first)) >= ((s32)((u32)(s32)counter * (u32)D_800F2EB8))) {
        target = D_800F3A30;
        if (target >= counter) {
            control->steering94.count22=(s16)((u16)control->steering94.count22+1U);
            D_8010C134 = 0;
            if (control->steering94.count22 == target) {
                func_800295B8(0);
            } else if (target < control->steering94.count22) {
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
        message = (s32)(9U - ((u32)D_800F3A30 - (u32)(s32)control->steering94.count22));
        if (message >= 6) {
            if (message == 9) {
                message = 15;
            }
            if (!(D_800ABEC0 & (0x200U << ((u32)message - 6U)))) {
                func_8004F2D8(message, 0);
                D_800ABEC0 |= 0x200U << ((u32)message - 6U);
            }
        }
        D_8010C134 = -1;
    }
}

#include "rr/overlay668_camera.h"

void ovl668_func_80115554(void) {
    s32 unused[4];
    s32 var_a3;
    s32 idx1;
    s32 idx2;
    s32 old_unk462;
    s16 new_unk462;
    s32 diff;
    s32 unk4A0;

    var_a3 = D_800AC650.fraction49C;
    idx1 = D_800AC650.node498 - ((3072 - var_a3) / 1024);
    idx2 = idx1 - 1;
    if (idx1 < 0) {
        idx1 += D_800ABECC;
        idx2 = idx1 - 1;
    }
    if (idx2 < 0) {
        idx2 += D_800ABECC;
    }
    func_80047610(&D_800ABEA4[idx1], &D_800ABEA4[idx2], 0, (1024 - var_a3) & 0x3FF, D_800AC650.target468);
    D_800AC650.delta478[0] = (s32)((u32)D_800AC650.target468[0] - (u32)(D_800AC650.position450[0] / 4));
    D_800AC650.delta478[1] = (s32)((u32)D_800AC650.target468[1] - (u32)(D_800AC650.position450[1] / 4));
    D_800AC650.delta478[2] = (s32)((u32)D_800AC650.target468[2] - (u32)(D_800AC650.position450[2] / 4));
    old_unk462 = D_800AC650.yaw462;
    new_unk462 = 1024 - func_8006E368(D_800AC650.delta478[0], D_800AC650.delta478[2]);
    old_unk462 = (new_unk462 - old_unk462) * 2;
    diff = old_unk462;
    old_unk462 &= 0xFFF;
    D_800AC650.yaw462 = new_unk462;
    if (diff & 0x800) {
        old_unk462 |= ~0xFFF;
    }
    unk4A0 = D_800AC650.velocity4A0;
    D_800AC650.velocity4A0 = (s32)((u32)unk4A0 + (u32)((s32)(((u32)old_unk462 - (u32)unk4A0) * 15U) / 16));
    D_800AC650.roll464 = (s32)((u32)D_800AC650.velocity4A0 + (u32)D_800AC650.roll464) / 2;
}

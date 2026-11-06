#include "rr/overlay668_camera.h"

void ovl668_func_80115850(void) {
    RrPreviewMatrixWorkspace sp10;
    RrPreviewVector8 sp30;
    s32 *scratch = RR_REPLAY_SCRATCH_WORDS;

    scratch[6] = D_800AC650.pitch460;
    scratch[7] = D_800AC650.yaw462;
    scratch[8] = D_800AC650.roll464;
    scratch[2] = D_800AC650.position450[0];
    scratch[3] = D_800AC650.position450[1];
    scratch[4] = D_800AC650.position450[2];
    sp30.x = scratch[6];
    sp30.y = scratch[7];
    sp30.z = scratch[8];
    RotMatrix(&sp30.x, sp10.halves);
    ApplyMatrixSV(&sp10.matrix, D_800AC650.offset488, &sp30);
    D_800ABEF8[0].pos20[0] = (D_800AC650.position450[0] / 4) + sp30.x;
    D_800ABEF8[0].pos20[1] = (D_800AC650.position450[1] / 4) + sp30.y;
    D_800ABEF8[0].pos20[2] = (D_800AC650.position450[2] / 4) + sp30.z;
    D_800ABEF8[0].rot50 = D_800AC650.pitch460 + D_800AC650.angle490;
    D_800ABEF8[0].rot54 = D_800AC650.yaw462;
    D_800ABEF8[0].rot58 = D_800AC650.roll464;
    func_8006E1AC();
}

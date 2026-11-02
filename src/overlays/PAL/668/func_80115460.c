#include "rr/overlay668_track_project.h"
/* The initializer supplies its position pointer; this body uses globals. */
void ovl668_func_80115460(s32 *unused_position) {
    RrTrackOffsetArgument offsets[2];
    s32 position[3];
    s32 height;
    s32 idx;

    (void)unused_position;

    position[0] = D_800AC650.position450[0] >> 2;
    position[1] = D_800AC650.position450[1] >> 2;
    position[2] = D_800AC650.position450[2] >> 2;
    idx = func_80026A74(position, D_800AC650.node498, (s32)((u32)D_800ABECC + 1U));
    D_800AC650.node498 = idx;
    offsets[0].value = (D_800AC650.position450[0] / 4) - (u16)D_800ABEA4[idx].x08;
    offsets[1].value = (D_800AC650.position450[2] / 4) - (u16)D_800ABEA4[idx].z0C;
    func_800464E8(offsets[0], offsets[1], D_800AC650.node498, &D_800AC650.fraction49C, &D_800AC650.lateral49E, &height);
}

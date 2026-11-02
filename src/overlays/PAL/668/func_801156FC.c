#include "rr/overlay668_camera.h"

void ovl668_func_801156FC(void) {
    s32 sp18[4];
    s32 sp28[4];
    s32 sp38[4];
    s32 idx;
    /* Keep the neighbour index in v1 across the two sample calculations. */
    register s32 next __asm__("$3");
    s32 dx;
    s32 dz;

    idx = D_800AC650.node498;
    next = idx + 1;
    if (next >= D_800ABECC) {
        next = 0;
    }
    func_80047610(&D_800ABEA4[idx], &D_800ABEA4[next], D_800AC650.lateral49E, D_800AC650.fraction49C, sp18);
    next = D_800AC650.node498;
    idx = next - 1;
    D_800AC650.position450[1] = (s32)((u32)sp18[1] - 120U);
    if (idx < 0) {
        idx += D_800ABECC;
    }
    func_80047610(&D_800ABEA4[idx], &D_800ABEA4[next], D_800AC650.lateral49E, D_800AC650.fraction49C, sp28);
    dx = (s32)(((u32)sp28[0] - (u32)sp18[0]) << 2);
    sp38[0] = dx;
    dz = (s32)(((u32)sp28[2] - (u32)sp18[2]) << 2);
    sp38[2] = dz;
    sp38[1] = (s32)((u32)sp28[1] - (u32)sp18[1]);
    D_800AC650.pitch460 = 1024 - func_8006E368(sp38[1], func_80090758((s32)((u32)dx * (u32)dx + (u32)dz * (u32)dz)) >> 6);
}

#include "rr/overlay668_camera.h"

void ovl668_func_80115DFC(void) {
    s32 i;
    RrTrackSample3C *p;

    func_80092CB8(0);
    func_8001ECE8(0, 0, 0, 1);
    D_800AC5DC = 384;
    func_8006E794();
    func_80013890(5);
    func_8006E1AC();
    p = D_800ABEA4;
    D_800AC650.counter444 = 0;
    D_800AC650.state44C = 0;
    D_800AC650.counter4A4 = 0;
    D_800AC650.mode4A8 = 1;
    D_800AC650.phase4AC = 0;
    D_800AC650.position450[0] = (s32)((u32)p[320].x08 << 2);
    D_800AC650.position450[1] = p[320].y10 - 120;
    D_800AC650.position450[2] = (s32)((u32)p[320].z0C << 2);
    D_800AC650.pitch460 = 0;
    D_800AC650.yaw462 = (s16)(0U-(u32)(u16)p[320].heading12);
    D_800AC650.roll464 = 0;
    D_800AC650.offset488[0] = 0;
    D_800AC650.offset488[1] = 0;
    D_800AC650.offset488[2] = 0;
    D_800AC650.angle490 = -96;
    D_8010718C = 0;
    D_800AC650.timer492 = 0;
    D_800AC650.timer494 = 0;
    D_800AC650.node498 = 320;
    ovl668_func_80115460(D_800AC650.position450);
    for (i = 0; i < 16; i++) {
        ovl668_func_80115554();
    }
    ovl668_func_801156FC();
    ovl668_func_80115850();
    func_80055300();
    func_80055330(0, 144, 144, 144);
    func_8005534C(1);
    D_800F2EC8 = 1;
    D_80127E90 = 16000;
    D_80127E94 = ovl668_D_80116CC0[(s32)((u32)D_800AD564 * 4U + (u32)(s32)D_800AD520)];
}

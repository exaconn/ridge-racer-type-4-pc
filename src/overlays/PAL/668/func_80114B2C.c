#include "rr/overlay668_stream.h"
#define STR_STATE ((RrStrPlaybackState *)D_800AC650.activeB8)

void ovl668_func_80114B2C(u8 *arg0) {
    s32 vol;
    func_80092CB8(0);
    STR_STATE->width00 = 320;
    STR_STATE->height04 = 240;
    func_800825B4(0, 0, 1);
    vol = (D_800AD558 * 127) / 16;
    func_80083CD4(0, vol, vol);
    D_800AC5DC = 128;
    ovl668_func_80114E48(arg0);
    ovl668_func_80114E90(&STR_STATE->display24, 0, 24, 0, 264);
    ovl668_func_80114EE8(ovl668_func_80114F48);
    while (ovl668_func_801150D8(&STR_STATE->display24) == -1) {
        ovl668_func_801153F0(D_800AC650.currentC0.c);
    }
    D_800AC650.phaseB4 = 1;
    D_800AC650.counterC4 = 0;
    D_800F2DE4 = 0;
    func_8008B154(0);
}

#include "rr/overlay662_color_pick.h"


void ovl662_func_80125370(void) {
    s32 x, y;
    u8 *img0, *img1;
    u8 c;

    if (ovl662_func_8011E6C8(82, 71, 128, 256) != 0) {
        return;
    }
    if (!(ovl662_D_80127668.words[1] & 2)) {
        return;
    }
    img0 = (u8 *)(RrGarageImage512 *)ovl662_D_801276A8.image38[ovl662_D_801276A8.active21];
    img1 = (u8 *)(RrGarageImage512 *)ovl662_D_801276A8.image40[ovl662_D_801276A8.active21];
    x = ovl662_D_801276A8.reset08[0] + (ovl662_D_80127668.halves[10] - 82) / ovl662_D_801276A8.config1C;
    y = ovl662_D_801276A8.reset08[1] + ((ovl662_D_80127668.halves[11] - 71) / ovl662_D_801276A8.config1C) / 2;
    c = (img0 + y * 32)[x / 2];
    if (x & 1) {
        c >>= 4;
    } else {
        c &= 0xF;
    }
    if (c != 0) {
        ovl662_D_801276A8.config22 = c;
        func_8004F24C(1);
    } else {
        c = (img1 + y * 32)[x / 2];
        if (x & 1) {
            c >>= 4;
        } else {
            c &= 0xF;
        }
        if (c != 0) {
            ovl662_D_801276A8.config22 = c;
            func_8004F24C(1);
        } else {
            func_8004F24C(4);
        }
    }
}

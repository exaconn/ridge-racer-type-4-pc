#include "rr/overlay662_image_drag.h"

void ovl662_func_801251AC(void) {
    u32 *src;
    u32 *dst;

    if (ovl662_D_80127668.words[0] & 8) {
        if (ovl662_D_80127668.words[1] & 4) {
            ovl662_D_801276A8.config20 = 0;
            ovl662_D_80127668.words[0] &= ~8;
            ovl662_D_80127668.words[14] = 0;
            ovl662_func_8011E7C0(13, 63, 294, 346);
            return;
        }
    } else {
        if (ovl662_func_8011E6C8(82, 71, 128, 256) != 0) {
            return;
        }
        if (!(ovl662_D_80127668.words[1] & 1)) {
            return;
        }
        ovl662_D_801276A8.config20 = 1;
        ovl662_D_80127668.words[4] = 52;
        ovl662_D_80127668.words[14] = 1;
        ovl662_D_80127668.words[0] |= 8;
        ovl662_func_8011E7C0(82, 71, 128, 256);
        ovl662_func_8011D120();
        ovl662_D_801277AC.x = ovl662_D_801276A8.reset08[0] + ovl662_D_801276A8.reset08[6];
        ovl662_D_801277AC.y = ovl662_D_801276A8.reset08[1] + ovl662_D_801276A8.reset08[7];
    }
    if (ovl662_D_801276A8.layer23 == 0) {
        src = ovl662_D_801276A8.image38[!ovl662_D_801276A8.active21];
        dst = ovl662_D_801276A8.image38[ovl662_D_801276A8.active21];
    } else {
        src = ovl662_D_801276A8.image40[!ovl662_D_801276A8.active21];
        dst = ovl662_D_801276A8.image40[ovl662_D_801276A8.active21];
    }
    ovl662_func_80125DE0(
        src,
        dst,
        ovl662_D_801276A8.reset08[0] + ovl662_D_801276A8.reset08[6] - ovl662_D_801277AC.x,
        ovl662_D_801276A8.reset08[1] + ovl662_D_801276A8.reset08[7] - ovl662_D_801277AC.y);
}

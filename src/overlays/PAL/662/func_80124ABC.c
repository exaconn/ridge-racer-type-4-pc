#include "rr/overlay662_image_stroke.h"

void ovl662_func_80124ABC(void) {
    u32 *img;

    if (ovl662_D_80127668.words[0] & 8) {
        if (ovl662_D_80127668.words[1] & 4) {
            ovl662_D_801276A8.config20 = 0;
            ovl662_D_80127668.words[0] &= ~8;
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
        ovl662_D_80127668.words[0] |= 8;
        ovl662_func_8011E7C0(82, 71, 128, 256);
        ovl662_func_8011D120();
        ovl662_D_80127798.x = ovl662_D_801276A8.reset08[0] + ovl662_D_801276A8.reset08[2];
        ovl662_D_80127798.y = ovl662_D_801276A8.reset08[1] + ovl662_D_801276A8.reset08[3];
    }
    if (ovl662_D_801276A8.layer23 == 0) {
        img = ovl662_D_801276A8.image38[ovl662_D_801276A8.active21];
    } else {
        img = ovl662_D_801276A8.image40[ovl662_D_801276A8.active21];
    }
    ovl662_D_8012779C.x = ovl662_D_801276A8.reset08[0] + ovl662_D_801276A8.reset08[2];
    ovl662_D_8012779C.y = ovl662_D_801276A8.reset08[1] + ovl662_D_801276A8.reset08[3];
    ovl662_func_801259FC(img, ovl662_D_80127798.x, ovl662_D_80127798.y, ovl662_D_8012779C.x, ovl662_D_8012779C.y, ovl662_D_801276A8.config22, ovl662_D_801276A8.config1D);
    ovl662_D_80127798.x = ovl662_D_8012779C.x;
    ovl662_D_80127798.y = ovl662_D_8012779C.y;
}

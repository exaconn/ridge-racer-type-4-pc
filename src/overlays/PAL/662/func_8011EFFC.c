#include "rr/overlay662_mirror.h"
void ovl662_func_8011EFFC(s32 arg0) {
    s32 last;
    s32 temp_a1;
    s32 var_a2;
    u32 *var_t0;
    s32 var_t2;
    u32 *var_a3;
    u32 *var_t1;
    u32 temp_a0;

    if(arg0<0) {
        ovl662_D_801276A8.optionIndices98.flat[0] = 1;
        ovl662_D_801276A8.optionIndices98.flat[3] = 1;
        return;
    }
    if (arg0 > 0) {
        switch (arg0) {
        case 1:
            ovl662_D_801276A8.optionIndices98.flat[0] = 0;
            ovl662_D_801276A8.selectorsB8[3] = 0;
            ovl662_func_8011E5C0(0);
            return;
        case 2:
            ovl662_D_801276A8.optionIndices98.flat[0] = 2;
            ovl662_D_801276A8.selectorsB8[3] = 2;
            ovl662_func_8011E5C0(0);
            return;
        case 4:
            ovl662_D_801276A8.optionIndices98.flat[3] = 0;
            ovl662_D_801276A8.selectorsB8[2] = 0;
            ovl662_func_8011E5C0(1);
            return;
        case 8:
            ovl662_D_801276A8.selectorsB8[1] = 2;
            ovl662_D_801276A8.selectorsB8[2] = 2;
            ovl662_func_8011E5C0(1);
            return;
        default:
            return;
        }
    }
        if ((ovl662_func_8011E734(2) == 0) && (ovl662_D_80127668.words[1] & 1)) {
            if (ovl662_D_80127668.words[0] & 4) {
                ovl662_func_8011E694();
            } else {
                ovl662_D_801276A8.optionIndices98.flat[0] = 0;
                ovl662_D_801276A8.optionIndices98.flat[3] = 1;
                ovl662_D_801276A8.saved64[0] = 2;
                ovl662_D_801276A8.saved64[1] = 0;
                ovl662_D_801276A8.saved64[2] = 0;
                ovl662_D_801276A8.saved64[3] = 1;
                ovl662_D_801276A8.saved64[4] = 1;
            }
            ovl662_func_8011D120();
            ((GarageButton *)ovl662_D_801273B8[2])->timer0E = 7;
            PlaySfx(1);
            var_t2 = 0;
            if (ovl662_D_801276A8.layer23 == 0) {
                var_t1 = ovl662_D_801276A8.image38[ovl662_D_801276A8.active21 == 0];
                var_t0 = ovl662_D_801276A8.image38[ovl662_D_801276A8.active21];
            } else {
                var_t1 = ovl662_D_801276A8.image40[ovl662_D_801276A8.active21 == 0];
                var_t0 = ovl662_D_801276A8.image40[ovl662_D_801276A8.active21];
            }
            last=7;
            do {
                var_a2 = 0;
                var_a3 = var_t1;
loop_23:
                temp_a0 = *var_a3;
                var_a3 += 1;
                temp_a1 = last - var_a2;
                var_a2 += 1;
                var_t0[temp_a1] = (temp_a0 << 0x1C) | (((temp_a0 >> 4) & 0xF) << 0x18) | (((temp_a0 >> 8) & 0xF) << 0x14) | (((temp_a0 >> 0xC) & 0xF) << 0x10) | (((temp_a0 << 12) >> 16) & 0xF000) | (((temp_a0 << 8) >> 20) & 0xF00) | ((temp_a0 >> 0x14) & 0xF0) | (temp_a0 >> 0x1C);
                if (var_a2 < 8) {
                    goto loop_23;
                }
                var_t2 += 1;
                var_t1 += 8;
                var_t0 += 8;
            } while (var_t2 < 0x40);
        }
}

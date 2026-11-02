#include "rr/overlay662_tool_action.h"

void ovl662_func_8011F678(s32 mode) {
    u32 *dst;
    s32 i;

    if (mode < 0) {
        ovl662_D_801276A8.optionIndices98.flat[0] = 2;
        ovl662_D_801276A8.optionIndices98.flat[3] = 1;
    } else if (mode > 0) {
        switch (mode) {
        case 1:
            ovl662_D_801276A8.optionIndices98.flat[0] = 1;
            ovl662_D_801276A8.selectorsB8[3] = 1;
            ovl662_func_8011E5C0(0);
            break;
        case 2:
            ovl662_D_801276A8.optionIndices98.flat[0] = 3;
            ovl662_D_801276A8.selectorsB8[3] = 3;
            ovl662_func_8011E5C0(0);
            break;
        case 4:
            ovl662_D_801276A8.optionIndices98.flat[3] = 0;
            ovl662_D_801276A8.selectorsB8[2] = 0;
            ovl662_func_8011E5C0(1);
            break;
        case 8:
            ovl662_D_801276A8.selectorsB8[1] = 2;
            ovl662_D_801276A8.selectorsB8[2] = 2;
            ovl662_func_8011E5C0(1);
            break;
        }
    } else if (ovl662_func_8011E734(5) == 0 && (ovl662_D_80127668.words[1] & 1)) {
        if (ovl662_func_8011D0C0() != 0) {
            ((RrGarageButton *)ovl662_D_801273B8[5])->timer0E = 7;
            func_8004F24C(4);
        } else {
            if (ovl662_D_80127668.words[0] & 4) {
                ovl662_func_8011E694();
            } else {
                ovl662_D_801276A8.optionIndices98.flat[0] = 2;
                ovl662_D_801276A8.optionIndices98.flat[3] = 1;
                ovl662_D_801276A8.saved64[0] = 5;
                ovl662_D_801276A8.saved64[1] = 0;
                ovl662_D_801276A8.saved64[2] = 0;
                ovl662_D_801276A8.saved64[3] = 1;
                ovl662_D_801276A8.saved64[4] = 2;
            }
            ovl662_func_8011D120();
            ((RrGarageButton *)ovl662_D_801273B8[5])->timer0E = 7;
            func_8004F24C(1);
            if (ovl662_D_801276A8.layer23 == 0) {
                dst = ovl662_D_801276A8.image38[ovl662_D_801276A8.active21];
            } else {
                dst = ovl662_D_801276A8.image40[ovl662_D_801276A8.active21];
            }
            for (i = 0; i < 512; i++) {
                *dst++ = 0;
            }
        }
    }
}

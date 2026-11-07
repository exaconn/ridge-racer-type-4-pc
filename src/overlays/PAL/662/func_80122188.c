#include "rr/overlay662_tool_action.h"

void ovl662_func_80122188(s32 mode) {
    if (mode < 0) {
        ovl662_D_801276A8.optionIndices98.flat[2] = 0;
        ovl662_D_801276A8.optionIndices98.flat[5] = 3;
    } else if (mode > 0) {
        switch (mode) {
        case 1:
        ovl662_D_801276A8.optionIndices98.flat[1] = 1;
        ovl662_D_801276A8.selectorsB8[0] = 1;
        ovl662_D_801276A8.selectorsB8[3] = 6;
        ovl662_func_8011E5C0(0);
            break;
        case 2:
        ovl662_D_801276A8.optionIndices98.flat[2] = 1;
        ovl662_D_801276A8.selectorsB8[3] = 8;
        ovl662_func_8011E5C0(0);
            break;
        case 4:

        ovl662_D_801276A8.optionIndices98.flat[5] = 2;
        ovl662_D_801276A8.selectorsB8[2] = 5;
        ovl662_func_8011E5C0(1);
            break;
        case 8:

        ovl662_D_801276A8.optionIndices98.flat[5] = 4;
        ovl662_D_801276A8.selectorsB8[2] = 7;
        ovl662_func_8011E5C0(1);
            break;
        }
    } else {
        if (ovl662_func_8011E734(0x1B) == 0) {
            if (ovl662_D_80127668.words[1] & 1) {
                if (ovl662_D_80127668.words[0] & 4) {
                    ovl662_func_8011E694();
                } else {
                    ovl662_D_801276A8.optionIndices98.flat[2] = 0;
                    ovl662_D_801276A8.optionIndices98.flat[5] = 3;
                    ovl662_D_801276A8.saved64[0] = 0x1B;
                    ovl662_D_801276A8.saved64[1] = 2;
                    ovl662_D_801276A8.saved64[2] = 2;
                    ovl662_D_801276A8.saved64[3] = 6;
                    ovl662_D_801276A8.saved64[4] = 7;
                }
                ovl662_D_801276A8.config22 = 7;
                PlaySfx(1);
            }
        }
    }
}

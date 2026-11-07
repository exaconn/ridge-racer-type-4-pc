#include "rr/overlay662_tool_action.h"

void ovl662_func_80120648(s32 mode) {
    if (mode < 0) {
        ovl662_D_801276A8.optionIndices98.flat[1] = 1;
    } else if (mode > 0) {
        switch (mode) {
        case 1:

ovl662_D_801276A8.optionIndices98.flat[1] = 0;
        ovl662_D_801276A8.selectorsB8[3] = 5;
        ovl662_func_8011E5C0(0);
            break;
        case 2:

ovl662_D_801276A8.optionIndices98.flat[2] = 0;
        ovl662_D_801276A8.selectorsB8[0] = 2;
        ovl662_D_801276A8.selectorsB8[3] = 7;
        ovl662_func_8011E5C0(0);
            break;
        case 4:
        ovl662_D_801276A8.selectorsB8[1] = 1;
        ovl662_D_801276A8.selectorsB8[2] = 0;
        ovl662_func_8011E5C0(1);
            break;
        case 8:
        ovl662_D_801276A8.selectorsB8[1] = 3;
        ovl662_D_801276A8.selectorsB8[2] = 2;
        ovl662_func_8011E5C0(1);
            break;
        }
    } else {
        if (ovl662_func_8011E734(0xE) == 0) {
            if (ovl662_D_80127668.words[1] & 1) {
                if (ovl662_D_80127668.words[0] & 4) {
                    ovl662_func_8011E694();
                } else {
                    ovl662_D_801276A8.optionIndices98.flat[1] = 1;
                    ovl662_D_801276A8.optionIndices98.flat[5] = 0;
                    ovl662_D_801276A8.saved64[0] = 0xE;
                    ovl662_D_801276A8.saved64[1] = 1;
                    ovl662_D_801276A8.saved64[2] = 2;
                    ovl662_D_801276A8.saved64[3] = 1;
                    ovl662_D_801276A8.saved64[4] = 6;
                }
                ovl662_D_801276A8.config1E = 3;
                PlaySfx(1);
            }
        }
    }
}

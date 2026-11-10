#include "rr/overlay662_tool_action.h"

void ovl662_func_80121564(s32 mode) {
    if (mode < 0) {

    } else if (mode > 0) {
        switch (mode) {
        case 1:
        ovl662_D_801276A8.selectorsB8[0] = 2;
        ovl662_D_801276A8.selectorsB8[3] = 2;
        ovl662_func_8011E5C0(0);
            break;
        case 2:
        ovl662_D_801276A8.selectorsB8[0] = 1;
        ovl662_D_801276A8.selectorsB8[3] = 1;
        ovl662_func_8011E5C0(0);
            break;
        case 4:
        ovl662_D_801276A8.selectorsB8[1] = 2;
        ovl662_D_801276A8.selectorsB8[2] = 2;
        ovl662_func_8011E5C0(1);
            break;
        case 8:
        ovl662_D_801276A8.selectorsB8[1] = 4;
        ovl662_D_801276A8.selectorsB8[2] = 4;
        ovl662_func_8011E5C0(1);
            break;
        }
    } else {
        if (ovl662_func_8011E734(0x13) == 0) {
            if (ovl662_D_80127668.words[1] & 1) {
                if (ovl662_D_80127668.words[0] & 4) {
                    ovl662_func_8011E694();
                } else {
                    ovl662_D_801276A8.optionIndices98.flat[0] = 4;
                    ovl662_D_801276A8.optionIndices98.flat[6] = 0;
                    ovl662_D_801276A8.saved64[0] = 0x13;
                    ovl662_D_801276A8.saved64[1] = 0;
                    ovl662_D_801276A8.saved64[2] = 3;
                    ovl662_D_801276A8.saved64[3] = 3;
                    ovl662_D_801276A8.saved64[4] = 0;
                }
                ((GarageButton *)ovl662_D_801273B8[19])->timer0E = 7;
                ovl662_D_801276A8.layer23 = 0;
                PlaySfx(1);
            }
        }
    }
}

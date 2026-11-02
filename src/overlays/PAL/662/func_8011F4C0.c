#include "rr/overlay662_tool_action.h"

void ovl662_func_8011F4C0(s32 mode) {
    if (mode < 0) {
        ovl662_D_801276A8.optionIndices98.flat[0] = 2;
        ovl662_D_801276A8.optionIndices98.flat[3] = 0;
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
            ovl662_D_801276A8.selectorsB8[1] = 4;
            ovl662_D_801276A8.selectorsB8[2] = 4;
            ovl662_func_8011E5C0(1);
            break;
        case 8:
            ovl662_D_801276A8.optionIndices98.flat[3] = 1;
            ovl662_D_801276A8.selectorsB8[2] = 1;
            ovl662_func_8011E5C0(1);
            break;
        }
    } else if (ovl662_func_8011E734(4) == 0 && (ovl662_D_80127668.words[1] & 1)) {
        if (ovl662_D_80127668.words[0] & 4) {
            ovl662_func_8011E694();
        } else {
            ovl662_D_801276A8.optionIndices98.flat[0] = 2;
            ovl662_D_801276A8.optionIndices98.flat[3] = 0;
            ovl662_D_801276A8.saved64[0] = 4;
            ovl662_D_801276A8.saved64[1] = 0;
            ovl662_D_801276A8.saved64[2] = 0;
            ovl662_D_801276A8.saved64[3] = 0;
            ovl662_D_801276A8.saved64[4] = 2;
        }
        ovl662_D_801276A8.config1E = 4;
        func_8004F24C(3);
        ovl662_D_80127668.words[0] |= 2;
        ovl662_D_80127668.points.first20 = ovl662_D_80127668.points.cursor14;
        ovl662_D_80127668.points.second24.x = 146;
        ovl662_D_80127668.points.second24.y = 199;
        ovl662_D_80127668.points.word28 = 0;
        ovl662_D_80127668.points.distance2C = ovl662_func_8011DBD8(&ovl662_D_80127668.points.first20, &ovl662_D_80127668.points.second24);
    }
}

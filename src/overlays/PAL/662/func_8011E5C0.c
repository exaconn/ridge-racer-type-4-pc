/* CC1_FLAGS: -fno-schedule-insns */
#include "rr/overlay662_options.h"
#include "rr/overlay662_distance.h"

void ovl662_func_8011E5C0(s32 mode) {
    RrGarageOptionRecord *options;
    s32 row;
    s32 col;
    s32 target;

    if (mode != 0) {
        s32 sel = ovl662_D_801276A8.selectorsB8[0];
        options = ovl662_D_801276A8.options78.groups.first[sel];
        col = ovl662_D_801276A8.selectorsB8[2];
        row = ovl662_D_801276A8.optionIndices98.groups.first[sel];
    } else {
        s32 sel = ovl662_D_801276A8.selectorsB8[1];
        options = ovl662_D_801276A8.options78.groups.second[sel];
        col = ovl662_D_801276A8.selectorsB8[3];
        row = ovl662_D_801276A8.optionIndices98.groups.second[sel];
    }
    target = *(col + options[row].destinations);
    ovl662_D_80127668.points.second24 = ovl662_func_8011DC74(target);
    ovl662_D_80127668.points.distance2C = ovl662_func_8011DBD8(&ovl662_D_80127668.points.first20, &ovl662_D_80127668.points.second24);
    ovl662_D_801276A8.current60 = target;
}

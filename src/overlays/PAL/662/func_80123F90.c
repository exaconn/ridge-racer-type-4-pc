#include "rr/overlay662_scroll_drag.h"

void ovl662_func_80123F90(void) {
    s32 imageTravel;
    s32 trackTravel;
    s32 limit;
    s32 imageThumb;
    s32 trackThumb;
    s32 projected;
    s32 relative;
    u16 narrowed;
    RrGarageSelectionPrefix *selection;
    s32 coordinate;

    if (ovl662_D_80127668.words[0] & 8) {
        if (ovl662_D_80127668.words[1] & 4) {
            ovl662_D_80127668.words[0] = (s32) (ovl662_D_80127668.words[0] & ~8);
            ovl662_func_8011E7C0(0xD, 0x3F, 0x126, 0x15A);
            return;
        }
        goto update_position;
    }
    if (((u8) ovl662_D_801276A8.config1C >= 4U) && (ovl662_func_8011E6C8(0xD8, 0x57, 4, 0xE0) == 0) && (ovl662_D_80127668.words[1] & 1)) {
        ovl662_D_80127668.words[4] = 0x2F;
        ovl662_D_80127668.words[0] = (s32) (ovl662_D_80127668.words[0] | 8);
        ovl662_func_8011E7C0(0xD8, 0x57, 4, 0xE0);
        imageThumb = 0x40 / (s32) ((u8) ovl662_D_801276A8.config1C >> 1);
        __asm__("" : : : "memory");
        trackThumb = 0xE0 / (s32) ((u8) ovl662_D_801276A8.config1C >> 1);
        imageTravel = 0x40 - imageThumb;
        trackTravel = 0xE0 - trackThumb;
        projected = (s32) ((ovl662_D_80127668.points.cursor14.y - 0x57) * imageTravel) / trackTravel;
        ovl662_D_80127280 = imageThumb;
        ovl662_D_80127284 = imageTravel;
        ovl662_D_80127288 = trackThumb;
        ovl662_D_8012728C = trackTravel;
        relative = projected - (s16) ovl662_D_801276A8.reset08[1];
        if ((relative < 0) || (relative >= imageThumb)) {
            ovl662_D_8012727E = (u16) ((s32) (((u32) trackThumb >> 1) * imageTravel) / trackTravel);
        } else {
            ovl662_D_8012727E = projected - ovl662_D_801276A8.reset08[1];
        }
update_position:
        coordinate=ovl662_D_8012767E-0x57;
        limit = ovl662_D_80127284;
        selection=&ovl662_D_801276A8;
        narrowed = ((s32) (coordinate * limit) / (s32) ovl662_D_8012728C) - ovl662_D_8012727E;
        selection->reset08[1] = narrowed;
        if ((s16) narrowed < 0) {
            selection->reset08[1] = 0U;
            return;
        }
        if (limit < (s16) narrowed) {
            selection->reset08[1] = (u16) ovl662_D_80127284;
        }
    }
}

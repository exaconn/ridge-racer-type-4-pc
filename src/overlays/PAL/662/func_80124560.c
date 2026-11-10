#include "rr/overlay662_palette_edit.h"

void ovl662_func_80124560(void) {
    u16 val;

    if (ovl662_D_80127668.words[0] & 8) {
        if (ovl662_D_80127668.words[1] & 4) {
            ovl662_D_80127668.words[0] &= ~8;
            ovl662_func_8011E7C0(13, 63, 294, 346);
            return;
        }
    } else {
        if (ovl662_func_8011E6C8(149, 391, 65, 13) != 0) {
            return;
        }
        if (!(ovl662_D_80127668.words[1] & 1)) {
            return;
        }
        ovl662_D_80127668.words[4] = 50;
        ovl662_D_80127668.words[0] |= 8;
        ovl662_func_8011E7C0(149, 391, 64, 12);
        ovl662_D_80127790 = 0;
        ovl662_D_80127794 = (*(u16 *)((ovl662_D_801276A8.config22*2)+(PaletteAddress)(Palette16 *)ovl662_D_801276A8.palette48[ovl662_D_801276A8.active21]) >> 10) & 0x1F;
    }
    val = (ovl662_D_80127668.halves[10] - 149) / 2;
    if (ovl662_D_80127794 != val) {
        if (ovl662_D_80127790 == 0) {
            ovl662_D_80127790 = 1;
            ovl662_func_8011D120();
        }
    }
    *(u16 *)((ovl662_D_801276A8.config22*2)+(PaletteAddress)(Palette16 *)ovl662_D_801276A8.palette48[ovl662_D_801276A8.active21]) =
        (*(u16 *)((ovl662_D_801276A8.config22*2)+(PaletteAddress)(Palette16 *)ovl662_D_801276A8.palette48[ovl662_D_801276A8.active21]) & 0x83FF) | (val << 10);
}

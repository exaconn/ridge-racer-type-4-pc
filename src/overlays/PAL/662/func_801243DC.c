#include "rr/overlay662_palette_edit.h"

void ovl662_func_801243DC(void) {
    u16 val;

    if (ovl662_D_80127668.words[0] & 8) {
        if (ovl662_D_80127668.words[1] & 4) {
            ovl662_D_80127668.words[0] &= ~8;
            ovl662_func_8011E7C0(13, 63, 294, 346);
            return;
        }
    } else {
        if (ovl662_func_8011E6C8(149, 373, 65, 13) != 0) {
            return;
        }
        if (!(ovl662_D_80127668.words[1] & 1)) {
            return;
        }
        ovl662_D_80127668.words[4] = 49;
        ovl662_D_80127668.words[0] |= 8;
        ovl662_func_8011E7C0(149, 373, 64, 12);
        ovl662_D_80127788 = 0;
        ovl662_D_8012778C = (*(u16 *)((ovl662_D_801276A8.config22*2)+(PaletteAddress)(Palette16 *)ovl662_D_801276A8.palette48[ovl662_D_801276A8.active21]) >> 5) & 0x1F;
    }
    val = (ovl662_D_80127668.halves[10] - 149) / 2;
    if (ovl662_D_8012778C != val) {
        if (ovl662_D_80127788 == 0) {
            ovl662_D_80127788 = 1;
            ovl662_func_8011D120();
        }
    }
    *(u16 *)((ovl662_D_801276A8.config22*2)+(PaletteAddress)(Palette16 *)ovl662_D_801276A8.palette48[ovl662_D_801276A8.active21]) =
        (*(u16 *)((ovl662_D_801276A8.config22*2)+(PaletteAddress)(Palette16 *)ovl662_D_801276A8.palette48[ovl662_D_801276A8.active21]) & 0xFC1F) | (val << 5);
}

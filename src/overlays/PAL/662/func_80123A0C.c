#include "rr/overlay662_palette_edit.h"
#include "rr/overlay662_tool_action.h"

void ovl662_func_80123A0C(void) {
    u16 *pal = (u16 *)((RrPaletteAddress)(RrPalette16 *)ovl662_D_801276A8.palette48[ovl662_D_801276A8.active21]+(ovl662_D_801276A8.config22*2));
    s32 val = (*pal >> 10) & 0x1F;

    if (ovl662_D_80127668.words[0] & 8) {
        if (ovl662_D_80127668.words[1] & 4) {
            ovl662_D_80127668.words[0] &= ~8;
            ovl662_func_8011E7C0(13, 63, 294, 346);
            return;
        }
    } else {
        if (ovl662_func_8011E6C8(141, 391, 8, 12) != 0) {
            return;
        }
        if (!(ovl662_D_80127668.words[1] & 1)) {
            return;
        }
        ovl662_D_80127668.words[4] = 44;
        ovl662_D_80127668.words[0] |= 8;
        ovl662_func_8011E7DC(44);
        ovl662_func_8011D120();
    }
    if (ovl662_D_80127668.words[1] & 0x40) {
        val--;
        ((RrGarageButton *)ovl662_D_801273B8[44])->timer0E = 7;
        if (val < 0) {
            val = 0;
            PlaySfx(4);
        } else {
            PlaySfx(0);
        }
        *pal = (*pal & 0x83FF) | (val << 10);
    }
}

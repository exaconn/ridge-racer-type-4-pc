#include "rr/overlay662_scroll_actions.h"

void ovl662_func_80123228(void) {
    if (ovl662_D_80127668.words[0] & 8) {
        if (ovl662_D_80127668.words[1] & 4) {
            ovl662_D_80127668.words[0] &= ~8;
            ovl662_func_8011E7C0(13, 63, 294, 346);
            return;
        }
    } else {
        if (ovl662_func_8011E6C8(81, 335, 9, 12) != 0) {
            return;
        }
        if (!(ovl662_D_80127668.words[1] & 1)) {
            return;
        }
        ovl662_D_80127668.words[4] = 38;
        ovl662_D_80127668.words[0] |= 8;
        ovl662_func_8011E7DC(38);
    }
    if (ovl662_D_80127668.words[1] & 0x40) {
        ((RrGarageButton *)ovl662_D_801273B8[38])->timer0E = 7;
        if (--ovl662_D_801276A8.reset08[0] < 0) {
            ovl662_D_801276A8.reset08[0] = 0;
            PlaySfx(4);
        } else {
            PlaySfx(0);
        }
    }
}

#include "rr/overlay662_scroll_actions.h"

void ovl662_func_80122FB4(void) {
    if (ovl662_D_80127668.words[0] & 8) {
        if (ovl662_D_80127668.words[1] & 4) {
            ovl662_D_80127668.words[0] &= ~8;
            ovl662_func_8011E7C0(13, 63, 294, 346);
            return;
        }
    } else {
        if (ovl662_func_8011E6C8(215, 69, 6, 18) != 0) {
            return;
        }
        if (!(ovl662_D_80127668.words[1] & 1)) {
            return;
        }
        ovl662_D_80127668.words[4] = 36;
        ovl662_D_80127668.words[0] |= 8;
        ovl662_func_8011E7DC(36);
    }
    if (ovl662_D_80127668.words[1] & 0x40) {
        ((RrGarageButton *)ovl662_D_801273B8[36])->timer0E = 7;
        if (--ovl662_D_801276A8.reset08[1] < 0) {
            ovl662_D_801276A8.reset08[1] = 0;
            PlaySfx(4);
        } else {
            PlaySfx(0);
        }
    }
}

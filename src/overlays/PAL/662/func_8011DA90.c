#include "game/system/pad.h"
#include "rr/overlay662_garage_fade.h"

void ovl662_func_8011DA90(void)
{
    ControllerState5C *pad;

    if ((ovl662_D_80127668.words[0] & 2) ||
        ((pad = &PlayerControllers[0])->input00.fields.type != 0x12 && (pad->input00.fields.buttons & 4))) {
        /* ASM_BARRIER: GCC local-alloc assigns state base to $a0 so controller base gets $a1 */
        register u32 *s __asm__("$4") = ovl662_D_80127668.words;
        s[10] += s[11];
        if ((s32)s[10] > 4096) {
            s[10] = 4096;
        }
    } else {
        if (ovl662_D_80127668.words[10] == 4096) {
            PlaySfx(2);
        }
        ovl662_D_80127668.words[10] -= ovl662_D_80127668.words[11];
        if ((s32)ovl662_D_80127668.words[10] < 0) {
            ovl662_D_80127668.words[10] = 0;
            ovl662_D_80127668.words[1] = 48;
            ovl662_D_80127668.words[0] &= ~4;
        }
    }
    RR_GARAGE_GTE_LERP(ovl662_D_80127668.words[10],
                       &ovl662_D_80127668.words[8],
                       &ovl662_D_80127668.words[5]);
}

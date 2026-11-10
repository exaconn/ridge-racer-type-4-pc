#include "game/system/pad.h"
#include "rr/overlay662_garage_fade.h"

void ovl662_step_panel_fade_timers(void)
{
    ControllerState5C *pad;

    if ((ovl662_editor_cursor_state.words[0] & 2) ||
        ((pad = &PlayerControllers[0])->input00.fields.type != 0x12 && (pad->input00.fields.buttons & 4))) {
        /* ASM_BARRIER: GCC local-alloc assigns state base to $a0 so controller base gets $a1 */
        register u32 *s __asm__("$4") = ovl662_editor_cursor_state.words;
        s[10] += s[11];
        if ((s32)s[10] > 4096) {
            s[10] = 4096;
        }
    } else {
        if (ovl662_editor_cursor_state.words[10] == 4096) {
            PlaySfx(2);
        }
        ovl662_editor_cursor_state.words[10] -= ovl662_editor_cursor_state.words[11];
        if ((s32)ovl662_editor_cursor_state.words[10] < 0) {
            ovl662_editor_cursor_state.words[10] = 0;
            ovl662_editor_cursor_state.words[1] = 48;
            ovl662_editor_cursor_state.words[0] &= ~4;
        }
    }
    RR_GARAGE_GTE_LERP(ovl662_editor_cursor_state.words[10],
                       &ovl662_editor_cursor_state.words[8],
                       &ovl662_editor_cursor_state.words[5]);
}

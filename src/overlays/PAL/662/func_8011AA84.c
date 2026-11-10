#include "rr/overlay662_transition.h"

void ovl662_draw_editor_transition(void) {
    ovl662_draw_editor_panel_fades();
    ovl662_init_editor_draw_env();
    ovl662_update_editor_overlay();
    {
    union {u32 bits;s32 value;} count;
    count.bits=ovl662_editor_workspace.word04+1U;
    ovl662_editor_workspace.word04=count.value;
    if (count.value >= 28) {
        ModeGroup = 3;
        ModeIndex = 5;
        SubmodeState = 0;
    }
    }
}

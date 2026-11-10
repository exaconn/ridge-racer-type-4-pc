#include "rr/overlay662_frame_step.h"

void ovl662_step_editor_frame(void) {
    ScreenFrame=(ScreenFrame+1U);
    if (++EmblemEditFrames > 0x014996FFU) {
        EmblemEditFrames = 0x014996FFU;
    }
    ovl662_update_cursor_pulse();
    ovl662_update_editor_cursor();
    ovl662_dispatch_editor_actions(0);
    ovl662_draw_editor_cursor();
    ovl662_draw_editor_panel_fades();
    ovl662_init_editor_draw_env();
    ovl662_update_editor_overlay();
    ovl662_upload_editor_textures();
    GarageActiveDecalTimA = ovl662_editor_workspace.buffers24[ovl662_editor_workspace.active21];
    GarageActiveDecalTimB = ovl662_editor_workspace.tim2C[ovl662_editor_workspace.active21];
}

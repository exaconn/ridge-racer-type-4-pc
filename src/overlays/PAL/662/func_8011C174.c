/* CC1_FLAGS: -fno-cse-follow-jumps -fno-rerun-cse-after-loop */
#include "rr/overlay662_scrollbar.h"

void ovl662_compute_scrollbar_thumb(s32 arg0, GpuRectangleFields *arg1)
{
    s32 range, span;
    if (arg0 == 0) {
        arg1->w = 112 / (ovl662_editor_workspace.config1C >> 1);
        arg1->h = 8;
        arg1->y = 337;
        if (ovl662_editor_workspace.config1C == 2) {
            arg1->x = 90;
            return;
        }
        range = 64 - 64 / (ovl662_editor_workspace.config1C >> 1);
        span = 112 - arg1->w;
        arg1->x = ovl662_editor_workspace.reset08[0] * span / range + 90;
    } else {
        arg1->w = 4;
        arg1->h = 224 / (ovl662_editor_workspace.config1C >> 1);
        arg1->x = 216;
        if (ovl662_editor_workspace.config1C == 2) {
            arg1->y = 87;
            return;
        }
        range = 64 - 64 / (ovl662_editor_workspace.config1C >> 1);
        span = 224 - arg1->h;
        arg1->y = ovl662_editor_workspace.reset08[1] * span / range + 87;
    }
}

#include "rr/overlay662_primitives.h"
#include "rr/animated_sprite_grid.h"
#ifndef RR_GPU_SCRATCH
#define RR_GPU_SCRATCH ((GpuScratchPointers *)0x1F800000)
#endif
void ovl662_draw_editor_cursor(void)
{
    GpuScratchPointers *scratch=RR_GPU_SCRATCH;
    u32 *ordering=scratch->order+132;
    if (!(ovl662_editor_cursor_state.words[0]&1)) {
        scratch->cursor=ovl662_emit_indexed_sprite(ovl662_editor_cursor_state.words[14],scratch->cursor,
            ovl662_editor_cursor_state.halves[10],ovl662_editor_cursor_state.halves[11]);
        scratch->cursor=ovl662_emit_draw_area(ordering,scratch->cursor,0,0,320,480);
    }
}

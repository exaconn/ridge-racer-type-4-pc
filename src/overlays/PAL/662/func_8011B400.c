#include "rr/overlay662_primitives.h"
#include "rr/animated_sprite_grid.h"
#ifndef RR_GPU_SCRATCH
#define RR_GPU_SCRATCH ((GpuScratchPointers *)0x1F800000)
#endif
void ovl662_init_editor_draw_env(void)
{
    GpuScratchPointers *scratch=RR_GPU_SCRATCH;
    u32 *order=scratch->order;
    scratch->cursor=ovl662_emit_draw_area(order+146,scratch->cursor,12,61,296,352);
    scratch->cursor=(u8 *)ovl662_emit_draw_mode(order+133,(GpuDrawModePacket *)scratch->cursor,704,0);
    scratch->cursor=(u8 *)ovl662_emit_draw_mode(order+147,(GpuDrawModePacket *)scratch->cursor,704,0);
}

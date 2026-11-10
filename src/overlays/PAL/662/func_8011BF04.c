#include "rr/overlay662_palette.h"
#ifndef RR_GPU_SCRATCH
#define RR_GPU_SCRATCH ((GpuScratchPointers *)0x1F800000)
#endif
void ovl662_draw_rgb_sliders(void)
{
    GpuScratchPointers *scratch=RR_GPU_SCRATCH;
    /* unsigned long retains the complete PAL/native address before selecting a color. */
    GaragePaletteEntry *color=(GaragePaletteEntry *)(ovl662_editor_workspace.config22*2 + (unsigned long)ovl662_editor_workspace.palette48[ovl662_editor_workspace.active21]);
    /* ASM_BARRIER: retain the captured red component across renderer calls. */
    register u32 red asm("$17");
    u16 blue,green;
    red=color->bytes[0];
    blue=color->value;
    red=(red&31)<<3;
    green=(blue&0x3E0)>>2;
    blue=(blue&0x7C00)>>7;
    scratch->cursor=(u8 *)ovl662_emit_filled_tile(scratch->order+139,(GpuTilePacket *)scratch->cursor,115,363,9,32,red,green,blue);
    scratch->cursor=ovl662_emit_indexed_sprite(70,scratch->cursor,114,361);
    scratch->cursor=ovl662_emit_indexed_sprite(46,scratch->cursor,125,356);
    ovl662_draw_rgb_digits(0,red>>3);
    ovl662_draw_rgb_digits(1,green>>3);
    ovl662_draw_rgb_digits(2,blue>>3);
}

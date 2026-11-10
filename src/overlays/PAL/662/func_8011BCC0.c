#include "rr/overlay662_palette_tile.h"
#ifndef RR_GPU_SCRATCH
#define RR_GPU_SCRATCH ((GpuScratchPointers *)0x1F800000)
#endif
void ovl662_draw_palette_swatch(s32 index)
{
    GpuScratchPointers *scratch=RR_GPU_SCRATCH;
    u32 *pal=ovl662_editor_workspace.palette48[ovl662_editor_workspace.active21];
    s32 slot=index-20;
    PaletteEntry16 *color=(PaletteEntry16 *)(slot*2+(PaletteAddress)pal);
    u32 r=color->bytes[0];
    GarageRectangle *rect=ovl662_button_rect_lookup[index];
    u16 raw=color->value;
    scratch->cursor=(u8*)ovl662_emit_filled_tile(scratch->order+139,(GpuTilePacket*)scratch->cursor,(s16)(*rect)[0],(s16)(*rect)[1],(s16)(*rect)[2],(s16)(*rect)[3],(r&31)<<3,(raw&0x3E0)>>2,(raw&0x7C00)>>7);
    if(slot==ovl662_editor_workspace.config22)
        scratch->cursor=ovl662_emit_indexed_sprite(69,scratch->cursor,(s16)((*rect)[0]-1),(s16)((*rect)[1]-2));
}

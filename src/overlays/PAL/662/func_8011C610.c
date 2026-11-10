#include "rr/overlay662_scale_panel.h"
void ovl662_draw_emblem_preview_box(s32 renderArgument) {
 s32 width=64/(((volatile GarageSelectionPrefix*)&ovl662_editor_workspace)->config1C>>1);
 s32 height;
 u32 *order;GpuScratchPointers *scratch;
 height=128/(((GarageSelectionPrefix*)&ovl662_editor_workspace)->config1C>>1);
 order=RR_GARAGE_SCRATCH->order+136;
 scratch=RR_GARAGE_SCRATCH;
 scratch->cursor=(u8*)ovl662_emit_wire_box(order,(GpuLineF4Packet*)scratch->cursor,(s16)(ovl662_editor_workspace.reset08[0]+237),(s16)(ovl662_editor_workspace.reset08[1]*2+71),width,height,0,ovl662_editor_workspace.shadeD0,0);
 scratch->cursor=ovl662_emit_draw_area(order,scratch->cursor,12,61,296,352);
 scratch->cursor=(u8*)ovl662_emit_textured_quad(order,(GpuPolyFT4Packet*)scratch->cursor,236,69,66,132,71,1,66,66,4272,12);
 scratch->cursor=(u8*)ovl662_emit_textured_quad(order,(GpuPolyFT4Packet*)scratch->cursor,236,69,66,132,3,1,66,66,4273,12);
 scratch->cursor=ovl662_emit_draw_area(order,scratch->cursor,237,71,64,128);
 scratch->cursor=(u8*)ovl662_emit_wire_box(order,(GpuLineF4Packet*)scratch->cursor,236,69,66,132,255,255,255);
}

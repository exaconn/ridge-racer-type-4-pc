/* CC1_FLAGS: -fno-cse-follow-jumps -fno-rerun-cse-after-loop */
#include "rr/overlay662_scrollbar.h"
#include "rr/overlay662_scroll_panel.h"
#include "rr/overlay662_scale_panel.h"

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

void ovl662_draw_scrollbars(s32 mode) {
#ifndef RR_GPU_SCRATCH
#define RR_GPU_SCRATCH ((GpuScratchPointers *)0x1F800000U)
#endif
 GpuScratchPointers *scratch=RR_GPU_SCRATCH;
 u32 *ordering=scratch->order+140;
 GpuRectangleFields rectangle;
 switch(mode) {
 case 46:
  ovl662_compute_scrollbar_thumb(0,&rectangle);
  scratch->cursor=(u8 *)ovl662_emit_filled_tile(ordering,(GpuTilePacket *)scratch->cursor,rectangle.x,rectangle.y,rectangle.w,rectangle.h,204,204,204);
  scratch->cursor=(u8 *)ovl662_emit_filled_tile(ordering,(GpuTilePacket *)scratch->cursor,90,337,112,8,77,77,77);
  scratch->cursor=(u8 *)ovl662_emit_wire_box(ordering,(GpuLineF4Packet *)scratch->cursor,89,335,114,12,136,136,136);
  return;
 case 47:
  ovl662_compute_scrollbar_thumb(1,&rectangle);
  scratch->cursor=(u8 *)ovl662_emit_filled_tile(ordering,(GpuTilePacket *)scratch->cursor,rectangle.x,rectangle.y,rectangle.w,rectangle.h,204,204,204);
  scratch->cursor=(u8 *)ovl662_emit_filled_tile(ordering,(GpuTilePacket *)scratch->cursor,216,87,4,224,77,77,77);
  scratch->cursor=(u8 *)ovl662_emit_wire_box(ordering,(GpuLineF4Packet *)scratch->cursor,215,85,6,228,136,136,136);
  return;
 case 48:
  scratch->cursor=(u8 *)ovl662_emit_gradient_quad(ordering,(PolyG4Storage *)scratch->cursor,149,355,65,13,0,0,0,255,0,0);return;
 case 49:
  scratch->cursor=(u8 *)ovl662_emit_gradient_quad(ordering,(PolyG4Storage *)scratch->cursor,149,373,65,13,0,0,0,0,255,0);return;
 case 50:
  scratch->cursor=(u8 *)ovl662_emit_gradient_quad(ordering,(PolyG4Storage *)scratch->cursor,149,391,65,13,0,0,0,0,0,255);return;
 }
}

void ovl662_draw_emblem_preview_box(s32 renderArgument) {
#ifndef RR_GARAGE_SCRATCH
#define RR_GARAGE_SCRATCH ((GpuScratchPointers *)0x1F800000U)
#endif
 s32 width=64/(((volatile GarageSelectionPrefix*)&ovl662_editor_workspace)->config1C>>1);
 s32 height;
 u32 *order;GpuScratchPointers *scratch;
 height=128/(((GarageSelectionPrefix*)&ovl662_editor_workspace)->config1C>>1);
 order=RR_GARAGE_SCRATCH->order+136;
 scratch=RR_GARAGE_SCRATCH;
 scratch->cursor=(u8*)ovl662_emit_wire_box(order,(GpuLineF4Packet*)scratch->cursor,(s16)(ovl662_editor_workspace.reset08[0]+237),(s16)(ovl662_editor_workspace.reset08[1]*2+71),width,height,0,ovl662_editor_workspace.shadeD0,0);
 scratch->cursor=ovl662_emit_draw_area(order, (u32 *)scratch->cursor,12,61,296,352);
 scratch->cursor=(u8*)ovl662_emit_textured_quad(order,(GpuPolyFT4Packet*)scratch->cursor,236,69,66,132,71,1,66,66,4272,12);
 scratch->cursor=(u8*)ovl662_emit_textured_quad(order,(GpuPolyFT4Packet*)scratch->cursor,236,69,66,132,3,1,66,66,4273,12);
 scratch->cursor=ovl662_emit_draw_area(order, (u32 *)scratch->cursor,237,71,64,128);
 scratch->cursor=(u8*)ovl662_emit_wire_box(order,(GpuLineF4Packet*)scratch->cursor,236,69,66,132,255,255,255);
}

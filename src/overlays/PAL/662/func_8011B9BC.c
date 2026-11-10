#include "rr/overlay662_primitives.h"
#include "rr/overlay662_line.h"
#ifndef RR_GPU_SCRATCH
#define RR_GPU_SCRATCH ((GpuScratchPointers *)0x1F800000)
#endif
void ovl662_draw_canvas_grid(s32 index)
{
    GpuScratchPointers *scratch = RR_GPU_SCRATCH;
    u32 *ordering = scratch->order + 137;
    GarageRectangle *rect = ovl662_button_rect_lookup[index];
    s16 u;
    s32 v;
    u16 clut;
    s32 tpage;

    if (index == 19) {
        u = 72;
        v = 2;
        clut = 0x10B1;
        tpage = 12;
    } else {
        u = 4;
        v = 2;
        clut = 0x10B1;
        tpage = 12;
        scratch->cursor = (u8 *)ovl662_emit_wire_box(ordering, (GpuLineF4Packet *)scratch->cursor, 273, 245, 7, 4, 255, 255, 255);
        scratch->cursor = (u8 *)ovl662_emit_wire_box(ordering, (GpuLineF4Packet *)scratch->cursor, 273, 325, 7, 4, 255, 255, 255);
        scratch->cursor = (u8 *)ovl662_emit_vline_prim(ordering, (GpuLineF2Packet *)scratch->cursor, 278, 245, 328, 255, 255, 255);
        scratch->cursor = (u8 *)ovl662_emit_vline_prim(ordering, (GpuLineF2Packet *)scratch->cursor, 279, 245, 328, 255, 255, 255);
    }
    scratch->cursor = ovl662_emit_draw_area(ordering, scratch->cursor, 12, 61, 296, 352);
    scratch->cursor = (u8 *)ovl662_emit_textured_quad(ordering, (GpuPolyFT4Packet *)scratch->cursor,
        (s16)((*rect)[0] - 1), (s16)((*rect)[1] - 2), (s16)((*rect)[2] + 2), (s16)((*rect)[3] + 4),
        u - 2, v - 2, 68, 68, clut, tpage);
    scratch->cursor = ovl662_emit_draw_area(ordering, scratch->cursor,
        (s16)(*rect)[0], (s16)(*rect)[1], (s16)(*rect)[2], (s16)(*rect)[3]);
    scratch->cursor = (u8 *)ovl662_emit_wire_box(ordering, (GpuLineF4Packet *)scratch->cursor,
        (s16)((*rect)[0] - 2), (s16)((*rect)[1] - 4), (s16)((*rect)[2] + 4), (s16)((*rect)[3] + 8), 255, 255, 255);
    scratch->cursor = (u8 *)ovl662_emit_wire_box(ordering, (GpuLineF4Packet *)scratch->cursor,
        (s16)((*rect)[0] - 1), (s16)((*rect)[1] - 2), (s16)((*rect)[2] + 2), (s16)((*rect)[3] + 4), 255, 255, 255);
}

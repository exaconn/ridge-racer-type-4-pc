#include "rr/overlay662_primitives.h"
#include "rr/overlay662_garage_fade.h"
#include "rr/animated_sprite_grid.h"
#ifndef RR_GPU_SCRATCH
#define RR_GPU_SCRATCH ((GpuScratchPointers *)0x1F800000)
#endif

/* Animated panel element: ordering slot, position/size and colour. */
typedef struct {
    s32 order;
    s16 x, y, w, h;
    u8 r, g, b, pad;
} GaragePanelElement;

/* Animation track: time window [start,end] and packed endpoints for GTE lerp. */
typedef struct {
    s16 start, end;
    u32 from, to;
} GaragePanelTrack;

extern GaragePanelElement ovl662_panel_elem_hline_top, ovl662_panel_elem_hline_bot, ovl662_panel_elem_tile_0,
    ovl662_panel_elem_tile_1, ovl662_panel_elem_tile_2;
extern GaragePanelTrack *ovl662_panel_fade_tracks[5];

void ovl662_draw_editor_panel_fades(void)
{
    GpuScratchPointers *scratch = RR_GPU_SCRATCH;
    u32 *ordering = scratch->order + 128;
    GaragePoint point;
    s32 i;
    s32 t;
    s32 range;

    if (ovl662_editor_workspace.flagsD1 != 0) {
        return;
    }
    for (i = 0; i < 5; i++) {
        t = ovl662_editor_workspace.word04 - ovl662_panel_fade_tracks[i]->start;
        range = ovl662_panel_fade_tracks[i]->end - ovl662_panel_fade_tracks[i]->start;
        if (t < 0) {
            continue;
        }
        if (range < t) {
            t = 4096;
        } else {
            t = (t << 12) / range;
        }
        RR_GARAGE_GTE_LERP(t, &ovl662_panel_fade_tracks[i]->from, &point);
        switch (i) {
        case 0:
            scratch->cursor = (u8 *)ovl662_emit_hline_tile(ordering + ovl662_panel_elem_hline_top.order, (GpuTilePacket *)scratch->cursor,
                ovl662_panel_elem_hline_top.x, ovl662_panel_elem_hline_top.y, point.x, ovl662_panel_elem_hline_top.r, ovl662_panel_elem_hline_top.g, ovl662_panel_elem_hline_top.b);
            break;
        case 1:
            scratch->cursor = (u8 *)ovl662_emit_hline_tile(ordering + ovl662_panel_elem_hline_bot.order, (GpuTilePacket *)scratch->cursor,
                ovl662_panel_elem_hline_bot.x, ovl662_panel_elem_hline_bot.y, point.x, ovl662_panel_elem_hline_bot.r, ovl662_panel_elem_hline_bot.g, ovl662_panel_elem_hline_bot.b);
            break;
        case 2:
            scratch->cursor = (u8 *)ovl662_emit_filled_tile(ordering + ovl662_panel_elem_tile_0.order, (GpuTilePacket *)scratch->cursor,
                point.x, point.y, ovl662_panel_elem_tile_0.w, ovl662_panel_elem_tile_0.h, ovl662_panel_elem_tile_0.r, ovl662_panel_elem_tile_0.g, ovl662_panel_elem_tile_0.b);
            scratch->cursor = (u8 *)ovl662_emit_wire_box(ordering + ovl662_panel_elem_tile_0.order, (GpuLineF4Packet *)scratch->cursor,
                (s16)(point.x - 1), (s16)(point.y - 2), (s16)(ovl662_panel_elem_tile_0.w + 2), (s16)(ovl662_panel_elem_tile_0.h + 4), 255, 255, 255);
            break;
        case 3:
            if (ovl662_editor_workspace.layer23) {
                point.y += 80;
            }
            scratch->cursor = (u8 *)ovl662_emit_filled_tile(ordering + ovl662_panel_elem_tile_1.order, (GpuTilePacket *)scratch->cursor,
                point.x, point.y, ovl662_panel_elem_tile_1.w, ovl662_panel_elem_tile_1.h, ovl662_panel_elem_tile_1.r, ovl662_panel_elem_tile_1.g, ovl662_panel_elem_tile_1.b);
            scratch->cursor = (u8 *)ovl662_emit_wire_box(ordering + ovl662_panel_elem_tile_1.order, (GpuLineF4Packet *)scratch->cursor,
                point.x, (s16)(point.y - 2), (s16)(ovl662_panel_elem_tile_1.w + 1), (s16)(ovl662_panel_elem_tile_1.h + 4), 255, 255, 255);
            break;
        case 4:
            scratch->cursor = (u8 *)ovl662_emit_filled_tile(ordering + ovl662_panel_elem_tile_2.order, (GpuTilePacket *)scratch->cursor,
                point.x, point.y, ovl662_panel_elem_tile_2.w, ovl662_panel_elem_tile_2.h, ovl662_panel_elem_tile_2.r, ovl662_panel_elem_tile_2.g, ovl662_panel_elem_tile_2.b);
            scratch->cursor = (u8 *)ovl662_emit_wire_box(ordering + ovl662_panel_elem_tile_2.order, (GpuLineF4Packet *)scratch->cursor,
                (s16)(point.x - 1), (s16)(point.y - 2), (s16)(ovl662_panel_elem_tile_2.w + 2), (s16)(ovl662_panel_elem_tile_2.h + 4), 255, 255, 255);
            break;
        }
    }
}

#include "rr/overlay662_primitives.h"
#include "rr/overlay662_garage_fade.h"
#include "rr/animated_sprite_grid.h"
#include "rr/overlay662_buttons.h"
#include "rr/overlay662_line.h"
#include "rr/overlay662_palette_tile.h"
#include "rr/overlay662_palette.h"

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
#ifndef RR_GPU_SCRATCH
#define RR_GPU_SCRATCH ((GpuScratchPointers *)0x1F800000U)
#endif
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

void ovl662_draw_editor_cursor(void)
{
#ifndef RR_GPU_SCRATCH
#define RR_GPU_SCRATCH ((GpuScratchPointers *)0x1F800000U)
#endif
    GpuScratchPointers *scratch=RR_GPU_SCRATCH;
    u32 *ordering=scratch->order+132;
    if (!(ovl662_editor_cursor_state.words[0]&1)) {
        scratch->cursor=ovl662_emit_indexed_sprite(ovl662_editor_cursor_state.words[14],scratch->cursor,
            ovl662_editor_cursor_state.halves[10],ovl662_editor_cursor_state.halves[11]);
        scratch->cursor=ovl662_emit_draw_area(ordering, (u32 *)scratch->cursor,0,0,320,480);
    }
}

void ovl662_draw_button_highlight(s32 index)
{
#ifndef RR_GPU_SCRATCH
#define RR_GPU_SCRATCH ((GpuScratchPointers *)0x1F800000U)
#endif
    GpuScratchPointers *scratch=RR_GPU_SCRATCH;
    GarageButton *button=(GarageButton *)ovl662_button_rect_lookup[index];
    s16 timer=button->timer0E;
    s32 sprite;
    if(timer==0)sprite=button->sprite08;
    else {
        sprite=button->sprite08+1;
        if(timer>0)button->timer0E=timer-1;
    }
    scratch->cursor=ovl662_emit_indexed_sprite(sprite,scratch->cursor,(s16)button->rectangle[0],(s16)button->rectangle[1]);
}

void ovl662_draw_canvas_grid(s32 index)
{
#ifndef RR_GPU_SCRATCH
#define RR_GPU_SCRATCH ((GpuScratchPointers *)0x1F800000U)
#endif
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
    scratch->cursor = ovl662_emit_draw_area(ordering, (u32 *)scratch->cursor, 12, 61, 296, 352);
    scratch->cursor = (u8 *)ovl662_emit_textured_quad(ordering, (GpuPolyFT4Packet *)scratch->cursor,
        (s16)((*rect)[0] - 1), (s16)((*rect)[1] - 2), (s16)((*rect)[2] + 2), (s16)((*rect)[3] + 4),
        u - 2, v - 2, 68, 68, clut, tpage);
    scratch->cursor = ovl662_emit_draw_area(ordering, (u32 *)scratch->cursor,
        (s16)(*rect)[0], (s16)(*rect)[1], (s16)(*rect)[2], (s16)(*rect)[3]);
    scratch->cursor = (u8 *)ovl662_emit_wire_box(ordering, (GpuLineF4Packet *)scratch->cursor,
        (s16)((*rect)[0] - 2), (s16)((*rect)[1] - 4), (s16)((*rect)[2] + 4), (s16)((*rect)[3] + 8), 255, 255, 255);
    scratch->cursor = (u8 *)ovl662_emit_wire_box(ordering, (GpuLineF4Packet *)scratch->cursor,
        (s16)((*rect)[0] - 1), (s16)((*rect)[1] - 2), (s16)((*rect)[2] + 2), (s16)((*rect)[3] + 4), 255, 255, 255);
}

void ovl662_draw_palette_swatch(s32 index)
{
#ifndef RR_GPU_SCRATCH
#define RR_GPU_SCRATCH ((GpuScratchPointers *)0x1F800000U)
#endif
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

/* The mode cycle established by1A788/1A414 and1FB2C is0..2. */
void ovl662_draw_tool_button(s32 index)
{
#ifndef RR_GPU_SCRATCH
#define RR_GPU_SCRATCH ((GpuScratchPointers *)0x1F800000U)
#endif
    GpuScratchPointers *scratch = RR_GPU_SCRATCH;
    GarageButton *button = (GarageButton *)ovl662_button_rect_lookup[index];
    s32 sprite0;
    s32 sprite1;

    if (button->timer0E == 0) {
        sprite0 = 16;
        switch (ovl662_editor_workspace.config1F) {
        case 0:
            sprite1 = 18;
            break;
        case 1:
            sprite1 = 20;
            break;
        case 2:
            sprite1 = 22;
            break;
        }
    } else {
        sprite0 = 17;
        switch (ovl662_editor_workspace.config1F) {
        case 0:
            sprite1 = 19;
            break;
        case 1:
            sprite1 = 21;
            break;
        case 2:
            sprite1 = 23;
            break;
        }
        if (button->timer0E > 0) {
            button->timer0E--;
        }
    }
    scratch->cursor = ovl662_emit_indexed_sprite(sprite0, scratch->cursor, 19, 173);
    scratch->cursor = ovl662_emit_indexed_sprite(sprite1, scratch->cursor, 39, 173);
}

void ovl662_draw_rgb_sliders(void)
{
#ifndef RR_GPU_SCRATCH
#define RR_GPU_SCRATCH ((GpuScratchPointers *)0x1F800000U)
#endif
    GpuScratchPointers *scratch=RR_GPU_SCRATCH;
    /* unsigned long retains the complete PAL/native address before selecting a color. */
    GaragePaletteEntry *color=(GaragePaletteEntry *)(ovl662_editor_workspace.config22*2 + (unsigned long)ovl662_editor_workspace.palette48[ovl662_editor_workspace.active21]);
    u32 r,red;
    u16 blue,green;
    r=color->bytes[0];
    blue=color->value;
    red=(r&31)<<3;
    green=(blue&0x3E0)>>2;
    blue=(blue&0x7C00)>>7;
    scratch->cursor=(u8 *)ovl662_emit_filled_tile(scratch->order+139,(GpuTilePacket *)scratch->cursor,115,363,9,32,red,green,blue);
    scratch->cursor=ovl662_emit_indexed_sprite(70,scratch->cursor,114,361);
    scratch->cursor=ovl662_emit_indexed_sprite(46,scratch->cursor,125,356);
    ovl662_draw_rgb_digits(0,red>>3);
    ovl662_draw_rgb_digits(1,green>>3);
    ovl662_draw_rgb_digits(2,blue>>3);
}

/* BF04 passes channel0..2 and a masked five-bit component. The tens
 * descriptor is read again after emitting that digit, for the ones position. */
void ovl662_draw_rgb_digits(s32 channel, s32 value)
{
#ifndef RR_GPU_SCRATCH
#define RR_GPU_SCRATCH ((GpuScratchPointers *)0x1F800000U)
#endif
    GpuScratchPointers *scratch = RR_GPU_SCRATCH;
    s16 sliderX;
    s16 sliderY;
    s16 digitX;
    s16 digitY;
    s32 tensSprite, idx;

    switch (channel) {
    case 0:
        sliderX = (value * 2) + 148;
        sliderY = 353;
        digitX = 133;
        digitY = 356;
        break;
    case 1:
        sliderX = (value * 2) + 148;
        sliderY = 371;
        digitX = 133;
        digitY = 374;
        break;
    case 2:
        sliderX = (value * 2) + 148;
        sliderY = 389;
        digitX = 133;
        digitY = 392;
        break;
    }
    scratch->cursor = ovl662_emit_indexed_sprite(71, scratch->cursor, sliderX, sliderY);
    tensSprite = (value / 10) + 59;
    scratch->cursor = ovl662_emit_indexed_sprite(tensSprite, scratch->cursor, digitX, digitY);
    idx = (tensSprite | 1);
    idx &= tensSprite;
    idx |= 1;
    idx &= tensSprite;
    scratch->cursor = ovl662_emit_indexed_sprite((value % 10) + 59, scratch->cursor, (s16)(digitX + ovl662_sprite_lookup[idx]->width04), digitY);
}

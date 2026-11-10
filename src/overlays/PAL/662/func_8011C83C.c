#include "rr/overlay662_garage.h"
#include "rr/animated_sprite_grid.h"

#define RR_GARAGE_SCRATCH ((GpuScratchPointers *)0x1F800000U)

/* Local views of the overlay primitive helpers: this unit passes the
 * rectangle coordinates as signed 16-bit values. */
u8 *ovl662_emit_draw_area(u32 *ordering, u8 *cursor, u32 x, u32 y, u32 w, u32 h);
u8 *ovl662_emit_wire_box(u32 *ordering, u8 *cursor, s16 x, s16 y, s16 w, s16 h,
    u32 r, u32 g, u32 b);
u8 *ovl662_emit_textured_quad(u32 *ordering, u8 *cursor, s16 x, s16 y, s16 w, s16 h,
    s16 u, s16 v, u32 uw, u32 vh, u32 clut, u32 tpage);

/* Zoomed preview panel: clip, optional selection outlines, then the preview
 * image at the current zoom (config1C) and its frame. */
void ovl662_draw_zoomed_canvas(s32 renderArgument)
{
    GpuScratchPointers *scratch;
    GarageSelectionPrefix *garage;
    u32 *order0, *order1, *order;
    u16 x, y, w, h;
    s16 size, size2;
    s32 clut, tpage;
    u16 u, v;

    scratch = RR_GARAGE_SCRATCH;
    order0 = scratch->order + 134;
    order1 = scratch->order + 135;
    scratch->cursor = ovl662_emit_draw_area(order0, scratch->cursor, 12, 61, 296, 352);
    /* ASM_BARRIER: keeps the clip cursor store ahead of the selection-size load. */
    __asm__("" : : : "memory");
    if (ovl662_editor_workspace.reset08[4] > 0 && !(ovl662_editor_cursor_state.words[0] & 6)) {
        if (ovl662_editor_workspace.config1E < 2 || ovl662_editor_workspace.config1E == 3
            || ovl662_editor_workspace.config1F == 1) {
            x = ovl662_editor_workspace.reset08[2];
            y = ovl662_editor_workspace.reset08[3];
            w = ovl662_editor_workspace.reset08[4];
            h = ovl662_editor_workspace.reset08[5];
            scratch->cursor = ovl662_emit_wire_box(order0, scratch->cursor,
                (s16)x * ovl662_editor_workspace.config1C + 82,
                (s16)y * ovl662_editor_workspace.config1C * 2 + 71,
                (s16)w * ovl662_editor_workspace.config1C,
                (s16)h * ovl662_editor_workspace.config1C * 2,
                0, ovl662_editor_workspace.shadeD0, 0);
        }
        if (ovl662_editor_workspace.config1E == 2 || ovl662_editor_workspace.config1F == 0) {
            x = ovl662_editor_workspace.reset08[6];
            y = ovl662_editor_workspace.reset08[7];
            w = ovl662_editor_workspace.reset08[8];
            h = ovl662_editor_workspace.reset08[9];
            scratch->cursor = ovl662_emit_wire_box(order0, scratch->cursor,
                (s16)x * ovl662_editor_workspace.config1C + 82,
                (s16)y * ovl662_editor_workspace.config1C * 2 + 71,
                (s16)w * ovl662_editor_workspace.config1C,
                (s16)h * ovl662_editor_workspace.config1C * 2,
                0, ovl662_editor_workspace.shadeD0, 0);
        }
    }
    switch (ovl662_editor_workspace.config1C) {
    case 2:
        x = 81; y = 69; w = 133; h = 265; size = 67; size2 = 67;
        break;
    case 4:
        x = 80; y = 67; w = 137; h = 273; size = 35; size2 = 35;
        break;
    case 8:
        x = 78; y = 63; w = 145; h = 289; size = 19; size2 = 19;
        break;
    default:
        x = 74; y = 55; w = 161; h = 321; size = 11; size2 = 11;
        break;
    }
    garage = &ovl662_editor_workspace;
    order = order0;
    u = garage->reset08[0] + 139;
    v = garage->reset08[1] + 1;
    if (garage->layer23) order = order1;
    clut = 4272;
    tpage = 12;
    scratch->cursor = ovl662_emit_textured_quad(order, scratch->cursor,
        x, y, w, h, u, v, size2, size, clut, tpage);
    scratch->cursor = ovl662_emit_textured_quad(order0, scratch->cursor,
        x, y, w, h, garage->reset08[0] + 71, garage->reset08[1] + 1, size2, size, clut, tpage);
    scratch->cursor = ovl662_emit_textured_quad(order1, scratch->cursor,
        x, y, w, h, garage->reset08[0] + 3, garage->reset08[1] + 1, size2, size, 4273, tpage);
    scratch->cursor = ovl662_emit_draw_area(order1, scratch->cursor, 82, 71, 128, 256);
    scratch->cursor = ovl662_emit_wire_box(order1, scratch->cursor, 81, 69, 130, 260, 255, 255, 255);
}

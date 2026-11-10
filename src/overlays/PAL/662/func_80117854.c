#include "rr/overlay662_history_labels.h"

void ovl662_draw_team_car_counts_label(u32 *order, void *cursor, s32 row)
{
    s32 i, x, y;
    s32 flag = 0;

    x = BadgeRows[row].x04 + 8;
    y = BadgeRows[row].y06 + 32;
    for (i = 0; i < 4; i++) {
        if (SelectionRows.bytes[0] == SaveProgress.history6C[i].selection) {
            flag = rsin(
                (((ScreenFrame * 128U) - 682U)) & 0xFFE) < 0;
            break;
        }
    }
    for (i = 0; i < 7; i++) {
        if (ovl662_team_count_glyphs[i] == -1) {
            cursor = AddTextSprites(
                order, cursor, x, y,
                FormatCountOfTotal((s16)ovl662_team_unlocked_counts[SelectionRows.bytes[0]], 80, 1),
                flag + 0x7FF8, 0, 0);
            x += 34;
            cursor = AddTilePrim(order, cursor, x, y - 2, 1, 20, 255, 255, 255);
            x += 8;
        } else if (ovl662_team_count_glyphs[i] == -2) {
            cursor = AddTextSprites(
                order, cursor, x, y,
                FormatCountOfTotal((s16)ovl662_total_unlocked_cars, 320, 2),
                flag + 0x7FF8, 0, 0);
        } else {
            cursor = AddSpritePrim(order, cursor, x, y,
                &UiFontGlyphs[ovl662_team_count_glyphs[i]], 0x7FF8, 0);
            x += UiFontGlyphs[ovl662_team_count_glyphs[i]].w + 2;
        }
    }
    AddDrawModePrim(order, cursor, 56);
}

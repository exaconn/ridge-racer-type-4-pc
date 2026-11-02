#include "rr/overlay663.h"

u8 *ovl663_format_time(s32 milliseconds) {
    s32 seconds = milliseconds / 1000;
    s32 minutes = seconds / 60;
    s32 remainder = seconds % 60;
    seconds = milliseconds - seconds * 1000;
    ovl663_time_text[0] = minutes / 10 + '0';
    ovl663_time_text[1] = minutes % 10 + '0';
    ovl663_time_text[3] = remainder / 10 + '0';
    ovl663_time_text[4] = remainder % 10 + '0';
    ovl663_time_text[6] = seconds / 100 + '0';
    ovl663_time_text[7] = seconds / 10 % 10 + '0';
    ovl663_time_text[8] = seconds % 10 + '0';
    return ovl663_time_text;
}

void *ovl663_draw_race_records(u32 *ordering, void *cursor, s32 row) {
    s32 i = 0, x, y, clut, rowY;
    s32 group = D_800AC648 >= 8;
    y = D_800F3AFC[row].y06 + 40;
    do {
        if (i == ovl663_selected_record) {
            if (func_800904F4((s32)(((u32)D_800ABEB0 << 7) - 682U)) < 0)
                clut = 32761;
            else
                clut = 32760;
        } else
            clut = 32760;
        x = D_800F3AFC[row].x04 + 8;
        cursor = func_8001FF84(ordering, cursor, x, y, &D_8009FAA0[i], clut, 0);
        x += ovl663_race_column_widths[0];
        cursor = func_8001FD20(ordering, cursor, x, y - 2, 1, 20, 255, 255, 255);
        x += ovl663_race_column_widths[1];
        cursor = func_8001F5B4(ordering, cursor, x, y,
                               (char *)ovl663_format_time(D_800AD5C8[group][D_800F3914][i].time),
                               clut, 0, 0);
        x += ovl663_race_column_widths[2];
        cursor = func_8001FD20(ordering, cursor, x, y - 2, 1, 20, 255, 255, 255);
        x += ovl663_race_column_widths[3];
        cursor = func_8001F5B4(ordering, cursor, x, y, D_800AD5C8[group][D_800F3914][i].name, clut,
                               0, 0);
        y += 24;
        i++;
    } while (i < 5);
    x += 52;
    rowY = D_800F3AFC[row].y06;
    y = rowY + 40;
    cursor = func_8001FD20(ordering, cursor, x, y - 2, 1, 116, 255, 255, 255);
    cursor = func_8001FCC8(ordering, cursor, 56);
    x += 16;
    cursor = func_80032EC8(ordering, cursor, x, y, D_800AC648 & 7);
    cursor = func_8001FF84(ordering, cursor, x + 8, rowY + 112, &ovl663_record_badges[group],
                           group + 30180, 0);
    return func_8001FCC8(ordering, cursor, 89);
}

void *ovl663_draw_lap_records(u32 *ordering, void *cursor, s32 row) {
    s32 i = 0, x, y, clut, rowY;
    s32 group = D_800AC648 >= 8;
    y = D_800F3AFC[row].y06 + 40;
    do {
        if (i == ovl663_selected_record) {
            if (func_800904F4((s32)(((u32)D_800ABEB0 << 7) - 682U)) < 0)
                clut = 32761;
            else
                clut = 32760;
        } else
            clut = 32760;
        x = D_800F3AFC[row].x04 + 8;
        cursor = func_8001FF84(ordering, cursor, x, y, &D_8009FAA0[i], clut, 0);
        x += ovl663_lap_column_widths[0];
        cursor = func_8001FD20(ordering, cursor, x, y - 2, 1, 20, 255, 255, 255);
        x += ovl663_lap_column_widths[1];
        cursor = func_8001F5B4(ordering, cursor, x, y,
                               (char *)ovl663_format_time(D_800FF700[group][D_800F3914][i].time),
                               clut, 0, 0);
        x += ovl663_lap_column_widths[2];
        cursor = func_8001FD20(ordering, cursor, x, y - 2, 1, 20, 255, 255, 255);
        x += ovl663_lap_column_widths[3];
        cursor = func_8001F5B4(ordering, cursor, x, y, D_800FF700[group][D_800F3914][i].name, clut,
                               0, 0);
        y += 24;
        i++;
    } while (i < 5);
    x += 52;
    rowY = D_800F3AFC[row].y06;
    y = rowY + 40;
    cursor = func_8001FD20(ordering, cursor, x, y - 2, 1, 116, 255, 255, 255);
    cursor = func_8001FCC8(ordering, cursor, 56);
    x += 16;
    cursor = func_80032EC8(ordering, cursor, x, y, D_800AC648 & 7);
    cursor = func_8001FF84(ordering, cursor, x + 8, rowY + 112, &ovl663_record_badges[group],
                           group + 30180, 0);
    return func_8001FCC8(ordering, cursor, 89);
}

void *ovl663_draw_record_labels(u32 *arg0, void *arg1, s32 arg2) {
    char buf[8];
    s32 i;
    s32 x;
    s32 y;

    x = D_800F3AFC[arg2].x04 + 8;
    y = D_800F3AFC[arg2].y06 + 32;
    for (i = 0; i < 8; i++) {
        buf[i] = 0;
    }
    arg1 = func_8001FF84(arg0, arg1, x, y, &ovl663_record_label, 0x7FF8, 0);
    x += ovl663_record_label.w;
    func_8002B814(buf, ovl663_course_unlock_levels[ovl663_selected_record], 1);
    arg1 = func_8001F5B4(arg0, arg1, x, y, buf, 0x7FF8, 0, 0);
    return func_8001FCC8(arg0, arg1, 56);
}

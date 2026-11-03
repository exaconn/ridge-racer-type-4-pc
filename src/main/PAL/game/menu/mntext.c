/* RODATA 0x80010e20 0x7c */
#include "rr/menu_text_rows.h"
#include "rr/menu_state_types.h"
#include "rr/time_record_types.h"
#include "rr/string_tail_copy.h"

void func_8002B814(char *output, s32 value, s32 digits);
void func_8002B934(char *output, s32 time);
extern char D_80010BF0[];
extern char D_80010E0C[];
extern u8 *D_8009FD04[];
extern char *D_8009FD8C[];
extern char *D_8009FD9C;
extern RrMenuSettings2C D_800AD4D8;
extern s8 D_800AD4F1;
extern s16 D_800AD522;
extern s16 D_800F3914;
extern RrTimeRecord16 D_800AD5C8[2][8][5];
extern RrTimeRecord16 D_800FF700[2][8][5];
extern u8 *D_800F39C8[];
extern s32 D_800F4C64;


s32 func_800402F4(RrMenuTextRow60 *rows) {
    u32 size = (u32)D_800F4D30 * 60U;
    D_800FFC14 = rows;
    func_80096434(rows, size);
    return (s32)size;
}

void func_80040338(s32 row, s32 position, s32 phase, s32 style, s32 label) {
    s32 templateIndex;
    s32 formatKind;
    s32 column;
    s32 value;
    s32 suffixIndex;

    if (row < D_800F4D30) {
        templateIndex = label & 0xFF;
        formatKind = label >> 8;
        D_800FFC14[row].rendering.phase = phase;
        column = 0;
        D_800FFC14[row].rendering.style = style;
        D_800FFC14[row].rendering.x = 0x11;
        D_800FFC14[row].rendering.y = (s16)position * 28 + 0x60;
        D_800FFC14[row].rendering.clut = 0x2638;
        if (label >= 0) {
            while (D_8009FD04[templateIndex][column] != 0) {
                D_800FFC14[row].rendering.text[column] = D_8009FD04[templateIndex][column];
                column++;
            }
        } else {
            for (; column < 0x16; column++) {
                D_800FFC14[row].rendering.text[column] = 0;
            }
            return;
        }
        switch (formatKind) {
        case 1:
        case 2:
        case 3:
            func_8002B814((char *)&D_800FFC14[row].rendering.text[4], formatKind, 0);
        case 4:
            func_8002B934((char *)&D_800FFC14[row].rendering.text[7], D_800F4C64);
            break;
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
            value = (formatKind - 5) % 5;
            func_8002B814((char *)&D_800FFC14[row].rendering.text[0], value + 1, 0);
            suffixIndex = value;
            if (value >= 3) {
                suffixIndex = 3;
            }
            func_8002B75C((char *)&D_800FFC14[row].rendering.text[1], D_8009FD8C[suffixIndex]);
            func_8002B75C((char *)&D_800FFC14[row].rendering.text[4], D_80010E0C);
            if (formatKind < 10) {
                func_8002B75C((char *)&D_800FFC14[row].rendering.text[14], D_800FF700[D_800AD522][D_800F3914][value].name);
                func_8002B934((char *)&D_800FFC14[row].rendering.text[4], D_800FF700[D_800AD522][D_800F3914][value].time);
            } else {
                func_8002B75C((char *)&D_800FFC14[row].rendering.text[14], D_800AD5C8[D_800AD522][D_800F3914][value].name);
                func_8002B934((char *)&D_800FFC14[row].rendering.text[4], D_800AD5C8[D_800AD522][D_800F3914][value].time);
            }
            break;
        case 15:
            value = D_800F39C8[0][0x1F3];
            func_8002B814((char *)&D_800FFC14[row].rendering.text[12], value + 1, 0);
            break;
        case 16:
            func_8002B814((char *)&D_800FFC14[row].rendering.text[8], D_800AD4D8.retries08[D_800AD4D8.phase17], 0);
            break;
        case 17:
            value = D_800AD4F1;
            func_8002B75C((char *)&D_800FFC14[row].rendering.text[0], D_80010BF0);
            if (value == 0) {
                value = 1;
            }
            if (value < 9) {
                func_8002B814((char *)&D_800FFC14[row].rendering.text[1], value, 0);
                value--;
                if (value >= 3) {
                    value = 3;
                }
                func_8002B75C((char *)&D_800FFC14[row].rendering.text[2], D_8009FD8C[value]);
            } else {
                func_8002B75C((char *)&D_800FFC14[row].rendering.text[1], D_8009FD9C);
            }
            break;
        case 18:
        case 19:
        case 20:
        case 21:
            value = formatKind - 18;
            func_8002B814((char *)&D_800FFC14[row].rendering.text[1], value + 1, 0);
            if (value >= 3) {
                value = 3;
            }
            func_8002B75C((char *)&D_800FFC14[row].rendering.text[2], D_8009FD8C[value]);
            break;
        case 30:
            D_800FFC14[row].rendering.clut = 0x2639;
            D_800FFC14[row].rendering.flags = 3;
            break;
        case 31:
            D_800FFC14[row].rendering.clut = 0x2639;
            D_800FFC14[row].rendering.flags = 2;
            D_800FFC14[row].rendering.text[0] = D_8009FD04[templateIndex][0];
            D_800FFC14[row].rendering.text[1] = 0;
            break;
        }
    }
}

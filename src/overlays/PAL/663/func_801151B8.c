#include "rr/course_choice_types.h"
#include "rr/icon_sprites.h"
#include "rr/menu_glyph.h"
#include "rr/menu_selection.h"
#include "rr/menu_transition.h"
#include "rr/overlay_menu.h"
#include "rr/tables.h"
#include "rr/overlay663_records.h"

extern RrMenuItemState5C D_800F3A38[2];
extern RrCourseChoice4 D_800ABE08[2][9];
extern s32 ovl663_D_80116898;
extern s32 ovl663_D_8011689C;
extern s32 ovl663_D_801168C8;
extern s16 D_800AC5D0;
extern s32 D_800F2DE4;
extern s32 D_800ABEB0;

void func_800207D4(u32 *, s32, u32);
void func_8002A344(s32);
void func_8003EC88(s32);
void func_8003D6F0(s32);
s32 func_8003AE4C(s32, s32);
void ovl663_func_801148D8(void);
void ovl663_func_80115BA0(void);

void ovl663_func_801151B8(void) {
    s32 car;
    s32 var_a0;
    s32 temp_v0;
    s16 temp_v1;
    s32 mirrorSet;
    s32 i;
    s32 glyphIndex;

    D_800F2DE4 = (u32)D_800F2DE4 + 1;
    D_800ABEB0 = (u32)D_800ABEB0 + 1;
    switch ((u16)D_800F4D8A) {
    case 0:
        i = 0;
        if (D_800F3AFC[1].state10 == 3) {
            D_800F4D8A = 1;
            func_8002AF8C(1, 1, 1);
            i = 0;
        }
        break;
    case 1:
        func_8002B12C(0xA0, 0x108, 0x76, 0, (s32)D_800AC648, 0, ovl663_D_8011689C);
        func_8002B330(1, 2, (char *)&D_800AC648, ovl663_D_8011689C, 0);
        i = 0;
        D_800F3914 = func_80032E9C(D_800AC648);
        while (i < (s32)D_800F4D80[2].byte02) {
            if (i == ((u16)D_800AC648 & 7)) {
                D_800F4D80[2].cells[i] = (s8)((u8)D_800AD522 + 2);
                D_800F4D80[2].byte16 = i;
            } else if (i >= ovl663_D_8011689C) {
                D_800F4D80[2].cells[i] = 1;
            } else {
                D_800F4D80[2].cells[i] = (u8)D_800AD522;
            }
            i += 1;
        }
        D_800F4D80[D_800AC64A].cells[0] = 0;
        func_8002B330(0, 2, (char *)&D_800AC64A, 2, 0);
        i = 0;
        D_800F4D80[D_800AC64A].cells[0] = 2;
        ovl663_D_80116898 = -1;
        if (D_800F3A38[0].a != 0) {
            func_8004F24C(1);
            D_800F4D80[D_800AC64A].cells[0] = 1;
            if (D_800AC64A == 0) {
                ovl663_D_80116898 = 0;
                D_800F4D8A = 2;
            } else if (D_800AC64A == 1) {
                i = 1;
            }
        } else if (D_800F3A38[0].b != 0) {
            func_8004F24C(2);
            i = 1;
        }
        if (i == 1) {
            func_8002AF8C(0, 1, 1);
            i = 0;
            D_800AC5D0 = 0x40;
            D_800F4D8A = 8;
            do {
                D_800F4D80[i].byte14 = 3;
                i += 1;
            } while (i < 3);
            D_800F3AFC[1].state10 = 4;
        }
        break;
    case 2:
        func_8002B330(0, 4, (char *)&ovl663_D_80116898, 5, 0);
        if (D_800F3A38[0].b != 0) {
            func_8004F24C(2);
            D_800F4D8A = 1;
        } else if (D_800F3A38[0].a != 0) {
            func_8002AF8C(0, 1, 1);
            D_800F4D8A = 3;
            func_8003A198();
            D_800ACC18 = D_800F4D40;
            mirrorSet = D_800AC648 >= 8;
            if (ovl663_D_801168C8 == 1) {
                car = D_800FF700[mirrorSet][D_800F3914][ovl663_D_80116898].car;
            } else {
                car = D_800AD5C8[mirrorSet][D_800F3914][ovl663_D_80116898].car;
            }
            func_80037040((s8 *)&D_800F4D98, car);
            if (D_800F4D98.bytes[2] == 0x27) {
                if (D_800F4D98.bytes[0] != 2) {
                    glyphIndex = D_800F4D98.bytes[1] + (D_800F4D98.bytes[0] * 4);
                } else {
                    glyphIndex = 0;
                }
                D_800ACBD0[0].first = (s16)(s8)D_800ABE08[0][glyphIndex].first;
                D_800ACBD0[0].second = 0;
            } else {
                D_800ACBD0[0].first = 0;
                D_800ACBD0[0].second = 0;
            }
            i = 0;
            do {
                D_800F4D80[i].byte14 = 3;
                i += 1;
            } while (i < 3);
            D_800F3AFC[1].state10 = 4;
        }
        break;
    case 3:
        temp_v0 = func_8003AE4C(0, 1);
        if ((temp_v0 == 1) && (D_800F4D80->byte14 == 5)) {
            ovl663_func_80115BA0();
            D_800F4C6A = temp_v0;
            D_800F3A26 = 0x4D;
            D_800F4D8A = 0;
        }
        break;
    case 8:
        temp_v1 = (u16)D_800AC5D0 - 1;
        D_800AC5D0 = (u16)temp_v1;
        func_800207D4(&D_800ACC14->head70, (0x40 - temp_v1) * 4, 0x45);
        i = 0;
        if ((s16)D_800AC5D0 <= 0) {
            ovl663_func_801148D8();
            D_800F4C6A = 1;
            D_800F3A26 = 0x4A;
            D_800F4D8A = 0;
            return;
        }
        break;
    default:
        break;
    }
    i = 0;
    do {
        func_8003EC88(i);
        i += 1;
    } while (i < 3);
    func_8003D6F0(1);
    var_a0 = 0x10063;
    if (ovl663_D_801168C8 == 1) {
        var_a0 = 0x1005F;
    }
    func_8002A344(var_a0);
}

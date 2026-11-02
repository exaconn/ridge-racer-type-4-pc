#include "rr/overlay666_preview.h"

#include "rr/icon_sprites.h"
#include "rr/preview_rgb.h"
#include "rr/tables.h"
#include "rr/selection_types.h"

extern u8 *D_800F2AB0;
extern s16 ovl666_D_80116F88[4];
extern u16 ovl666_D_80116F68[4][4];
extern s32 ovl666_D_80116F94;
extern RrMenuItemState5C D_800F3A38[2];
extern s16 D_800AC5D0;
extern s16 D_800F4D44;
extern u16 D_800F4BE8, D_800F4C6A, D_800F4D8A;
extern s16 D_800F3A26;

void func_80033A74(s32);
void func_8006EF8C(s32);
s32 func_8003AE4C(s32, s32);
s32 func_8003708C(s8 *);
void func_8002E394(RrRaceVehicle *);
void func_80014A8C(s32);
void func_800207D4(u32 *, s32, u32);
void func_8003EC88(s32);
void func_80037198(s32);
void func_8002E5A0(RrRaceVehicle *);
void func_8002CC88(RrRaceVehicle *, RrPreviewRgb3, s32, s32);
void func_8003D6F0(s32);
s32 func_8003FBA8(s32);
void func_8002A344(s32);
void func_80053CB0(void);
void func_80053AF4(s32, s32);
void ovl666_func_801148D4(void);

void ovl666_func_80115C5C(void) {
    volatile RrPreviewRgb3 color;
    s32 var_a3;
    s32 var_a2;
    s32 var_s5;
    s32 var_s0;
    s32 temp_v0;
    s32 temp_s0;
    u8 temp_s0_2;
    u8 temp_s2;
    u8 temp_v1_6;
    u16 temp_v1_5;
    s8 temp_v0_3;
    RrUiRow24 *temp_v1_3;
    RrUiRow24 *temp_v1_4;
    register s32 var_s0_2 __asm__("$16");
    register s32 var_s0_3 __asm__("$16");
    s32 idx_2;
    s32 idx_3;

    D_800F2DE4 = (u32)D_800F2DE4 + 1;
    var_s5 = 0;
    __asm__("" : "+r"(var_s5));
    D_800ABEB0 = (u32)D_800ABEB0 + 1;
    D_800ABEF8[0].rot54 = ((u32)D_800ABEF8[0].rot54 + 8) & 0xFFF;
    func_80033A74(1);
    func_8006EF8C(6);
    if (D_800F4D8A != 3) {
        temp_v0 = func_8003AE4C(0, 1);
        if (temp_v0 != 1) {
            if (((s8 *)D_800F2AB0)[1] == 1) {
                ((s8 *)D_800F2AB0)[1] = 0;
            }
            var_s0 = 0;
            goto loop_45;
        }
        func_8002E394(D_800ABEF8);
        if (((s8 *)D_800F2AB0)[1] == 7) {
            ((s8 *)D_800F2AB0)[1] = temp_v0;
        }
    }
    var_s5 = 1;
    switch (D_800F4D8A) {
    case 0:
        func_8002AF8C(1, 1, 1);
        temp_s0 = func_8003708C((s8 *)&D_800F4D98);
        func_8002B330(1, 2, (char *)&ovl666_D_80116F98.halves[0], ovl666_D_80116F88[D_800F3826], 0);
        func_80037040((s8 *)&D_800F4D98, ovl666_D_80116FA0[D_800F3826][ovl666_D_80116F98.word].glyph);
        if (((u32)temp_s0 << 16) != ((u32)func_8003708C((s8 *)&D_800F4D98) << 16)) {
            func_8003A198();
            D_800ACC18 = D_800F4D40;
            func_80037040((s8 *)&D_800F4D98, ovl666_D_80116FA0[D_800F3826][ovl666_D_80116F98.word].glyph);
            D_800ACBD0[0].first = ovl666_D_80116FA0[D_800F3826][ovl666_D_80116F98.word].first;
            D_800ACBD0[0].second = ovl666_D_80116FA0[D_800F3826][ovl666_D_80116F98.word].second;
            ((s8 *)D_800F2AB0)[1] = 6;
        }
        D_800F4D80[D_800AC64A + 3].cells[0] = 0;
        func_8002B330(0, 2, (char *)&D_800AC64A, 2, 0);
        D_800F4D80[D_800AC64A + 3].cells[0] = 2;
        temp_s0_2 = D_800F4D80[D_800AC64A + 3].byte14;
        if (temp_s0_2 == 2) {
            if (D_800F3A38[0].a != 0) {
                D_800F4D80[D_800AC64A + 3].cells[0] = 1;
                if (D_800AC64A == 0) {
                    func_8004F24C(1);
                    ovl666_D_80116F90 = temp_s0_2;
                    D_800F4D8A = 4;
                    goto block_44;
                } else if (D_800AC64A == 1) {
                    func_8004F24C(1);
                    var_s0_2 = 0;
                    idx_2 = 0;
                    do {
                        temp_v1_3 = (RrUiRow24 *)((unsigned long)(idx_2 * sizeof(RrUiRow24)) + (unsigned long)D_800F4D80 + 0x6C);
                        if (temp_v1_3->byte14 == 2) {
                            temp_v1_3->byte14 = 3;
                        }
                        var_s0_2 += 1;
                        idx_2++;
                    } while (var_s0_2 < 2);
                    goto block_27;
                }
            } else if (D_800F3A38[0].b != 0) {
                func_8004F24C(2);
                var_s0_3 = 0;
                idx_3 = 0;
                do {
                    temp_v1_4 = (RrUiRow24 *)((unsigned long)(idx_3 * sizeof(RrUiRow24)) + (unsigned long)D_800F4D80 + 0x6C);
                    if (temp_v1_4->byte14 == 2) {
                        temp_v1_4->byte14 = 3;
                    }
                    var_s0_3 += 1;
                    idx_3++;
                } while (var_s0_3 < 2);
block_27:
                ((s8 *)D_800F2AB0)[1] = 3;
                D_800F4D8A = 5;
                D_800F3AFC->state10 = 4;
            }
        }
        goto block_44;
    case 3:
        func_8002AF8C(0, 1, 1);
        if (D_800AC5D0 > 0) {
            D_800AC5D0 = (u16)D_800AC5D0 - 1;
        }
        func_80014A8C(-1);
        func_800207D4(&D_800ACC14->head70, (0x40 - D_800AC5D0) * 4, 0x45);
        temp_v0 = func_8003AE4C(1, 1);
        if ((temp_v0 == 1) && (D_800AC5D0 <= 0)) {
            D_800F4D44 = 6;
            temp_v1_5 = ovl666_D_80116F68[D_800F4D98.primary][D_800F4D98.variant];
            ovl666_D_80116F94 = temp_v0;
            D_800AC648 = temp_v1_5;
            D_800F3914 = func_80032E9C((s16)temp_v1_5);
            func_80053CB0();
            D_800F4BE8 = 0;
            D_800F4C6A = temp_v0;
            D_800F3A26 = 0x23;
            D_800F4D8A = 0;
            goto block_36;
        }
        goto block_44;
    case 4:
        temp_s2 = D_800F4D80[5].byte14;
        if (temp_s2 == 2) {
            D_800F4D80[D_800F4D98.pad02[1] + 5].cells[0] = 0;
            func_8002B330(0, 1, (char *)&D_800F4D98.pad02[1], 2, 0);
            D_800F4D80[D_800F4D98.pad02[1] + 5].cells[0] = temp_s2;
            if (D_800F3A38[0].a != 0) {
                func_8004F24C(1);
                func_8002AF8C(0, 1, 1);
                D_800F4D80[D_800F4D98.pad02[1] + 5].cells[0] = 1;
                D_800F4D80[5].byte14 = 7;
                D_800F4D80[6].byte14 = 7;
                __builtin_memcpy(&D_800F4D98.rows[0], &D_800F4D98, 4);
                D_800F4D98.rows[0].parameter = 0x13;
                D_800F3914 = 0;
                D_800F4D8A = 3;
                func_8003A198();
                D_800AC5D0 = 0x40;
                func_80053AF4(0, 0x3C);
block_36:
                D_800F4D70.first = D_800ACC18;
                goto block_44;
            }
            if (D_800F3A38[0].b != 0) {
                func_8004F24C(2);
                D_800F4D80[5].byte14 = 3;
                D_800F4D80[6].byte14 = 3;
                D_800F4D8A = 6;
            }
        }
        goto block_44;
    case 6:
        if (D_800F4D80[5].byte14 == 5) {
            D_800F4D8A = 0;
            ovl666_D_80116F90 = 1;
            D_800F4D80[5].byte14 = 0;
            D_800F4D80[6].byte14 = 0;
        }
        goto block_44;
    case 5:
        func_8002AF8C(0, 1, 1);
        temp_v1_6 = D_800F4D80[3].byte14;
        if ((temp_v1_6 == 5) && (((s8 *)D_800F2AB0)[1] == temp_v1_6)) {
            func_8003A198();
            ovl666_func_801148D4();
            D_800F4C6A = 1;
            D_800F3A26 = 0x4F;
            D_800F4D8A = 0;
            goto end;
        }
        goto block_44;
    }
block_44:
    var_s0 = 0;
loop_45:
    do {
        func_8003EC88(var_s0 + 3);
        var_s0 += 1;
    } while (var_s0 < 2);
    if (ovl666_D_80116F90 == 2) {
        func_8003EC88(5);
        func_8003EC88(6);
    }
    var_s0 = var_s5;
    if (((u32)((u8)((s8 *)D_800F2AB0)[1] - 1) < 2U) && (var_s0 == 1)) {
        func_80037198(0);
        func_8002E5A0(D_800ABEF8);
        var_a2 = 0x100;
        if (((s8 *)D_800F2AB0)[1] == var_s0) {
            temp_v0_3 = ~((D_800F2AB0[2] * 0xFF) / 30);
            color.r = temp_v0_3;
            color.g = temp_v0_3;
            color.b = temp_v0_3;
            var_a3 = 0x102;
            var_a2 = (D_800F2AB0[2] << 8) / 30;
        } else {
            var_a3 = 1;
            color.r = 0x80;
            color.g = 0x80;
            color.b = 0x80;
        }
        func_8002CC88(D_800ABEF8, color, var_a2, var_a3);
    }
    func_8003D6F0(0);
    if (func_8003FBA8(0) > 0) {
        if (ovl666_D_80116F88[D_800F3826] >= 2) {
            func_8002B12C(0xA0, 0x90, 8, 1, ovl666_D_80116F98.word, 0, ovl666_D_80116F88[D_800F3826]);
        }
    }
end:
    func_8002A344(0x10036);
}

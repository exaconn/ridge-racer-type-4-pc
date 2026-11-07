#include "rr/menu_overlay_runtime.h"

typedef struct {s16 value;u16 reserved;} RrGarageCarSlot;
extern s16 ovl662_D_801272BA;
extern s16 ovl662_D_801272D4,ovl662_D_801272E0,ovl662_D_801272E4;
extern s16 ovl662_D_80127300[20];
extern RrGarageCarSlot ovl662_D_80127348[20];
void ovl662_func_8011512C(void);
void ovl662_func_80116C34(s32);

void ovl662_func_80116E10(void)
{
    union {RrPreviewRgb3 rgb; u8 channel[3];} color;
    s8 shown;
    s32 ready;
    s32 before;
    s32 status;
    s32 state;
    s32 cell;
    s32 i;
    s32 fadeMode;
    s16 steps;
    s32 fadeLevel;
    s8 shade;
    RrRaceVehicle *vehicle;

    shown = 0;
    D_800F2DE4++;
    vehicle = D_800ABEF8;
    ScreenFrame++;
    vehicle->rot54 = (vehicle->rot54 + 8) & 0xFFF;
    func_80033A74(1);
    func_8006EF8C(6);
    ready = func_8003AE4C(0, 1);
    if (ready != 1) {
        if ((s8)D_800F2AB0[1] == 1) {
            D_800F2AB0[1] = 0;
        }
        goto common;
    }
    func_8002E394(vehicle);
    if ((s8)D_800F2AB0[1] == 7) {
        D_800F2AB0[1] = ready;
    }
    shown = 1;
    switch (D_800F4D8A) {
    case 0:
        func_8002AF8C(1, 1, 1);
        before = func_8003708C((s8 *)&D_800F4D98);
        func_8002B330(3, 2, (char *)&ovl662_D_801272E4, ovl662_D_801272E0, 0);
        func_80037040((s8 *)&D_800F4D98, ovl662_D_80127348[ovl662_D_801272E4].value);
        status = ovl662_D_80127300[(D_800F4D98.entry.parameter % 20) & 0xFF];
        if ((s16)before != (s16)func_8003708C((s8 *)&D_800F4D98)) {
            ResetScreenCounters();
            D_800ACC18 = D_800F4D70.first;
            func_80037040((s8 *)&D_800F4D98, ovl662_D_80127348[ovl662_D_801272E4].value);
            D_800F2AB0[1] = 6;
            D_800ACBD0[0].first = 0;
            D_800ACBD0[0].second = 0;
        }
        D_800F4D80[D_800AC64A].cells[0] = 0;
        func_8002B330(0, 2, (char *)&D_800AC64A, 2, 0);
        cell = 2;
        if (status != 1) {
            D_800F4D80[0].half20 = 1;
            if (D_800AC64A == 0) {
                cell = 4;
            }
        } else {
            D_800F4D80[0].half20 = 0;
        }
        D_800F4D80[D_800AC64A].cells[0] = cell;
        if (D_800F4D80[0].byte14 != 2) {
            break;
        }
        state = D_800F3AFC[0].state10;
        if (state != 3) {
            break;
        }
        if (D_800F3A38[0].a) {
            func_8002AF8C(0, 1, 1);
            D_800F4D80[D_800AC64A].cells[0] = 1;
            if (D_800AC64A == 0) {
                if (status == 1) {
                    PlaySfx(1);
                    func_80014A8C(-1);
                    D_800F4D8A = 3;
                    D_800F3AFC[0].state10 = 4;
                    D_800F3AFC[1].state10 = 4;
                    D_800F2AB0[1] = 3;
                    for (i = 0; i < 4; i++) {
                        if (D_800F4D80[i].byte14 == 2) {
                            D_800F4D80[i].byte14 = 3;
                        }
                    }
                    D_800AC5D0 = 64;
                } else {
                    PlaySfx(4);
                }
            } else if (D_800AC64A == 1) {
                func_80014A8C(-1);
                PlaySfx(1);
                D_800F3AFC[0].state10 = 4;
                D_800F3AFC[1].state10 = 4;
                D_800F2AB0[1] = state;
                for (i = 0; i < D_800F4BEC; i++) {
                    if (D_800F4D80[i].byte14 == 2) {
                        D_800F4D80[i].byte14 = 3;
                    }
                }
                D_800AC5D0 = 64;
                D_800F4D8A = 8;
            }
        } else if (D_800F3A38[0].b) {
            PlaySfx(2);
            func_80014A8C(-1);
            func_8002AF8C(0, 1, 1);
            D_800F3AFC[0].state10 = 4;
            D_800F3AFC[1].state10 = 4;
            D_800F2AB0[1] = state;
            for (i = 0; i < D_800F4BEC; i++) {
                if (D_800F4D80[i].byte14 == 2) {
                    D_800F4D80[i].byte14 = 3;
                }
            }
            D_800AC5D0 = 64;
            D_800F4D8A = 8;
        }
        break;
    case 3:
        D_800AC5D0--;
        steps = 64;
        func_800207D4(D_800ACC14->ordering.ordering70, (steps - D_800AC5D0) * 4, 69);
        if (D_800AC5D0 <= 0) {
            D_800ABE08[1][ovl662_D_801272BA].glyph = ovl662_D_80127348[ovl662_D_801272E4].value;
            D_800ABE08[1][ovl662_D_801272BA].first = 0;
            D_800ABE08[1][ovl662_D_801272BA].second = 0;
            ovl662_D_801272E4 = ovl662_D_801272BA;
            ovl662_func_8011512C();
            D_800F4C6A = ready;
            D_800F3A26 = steps;
            D_800F4D8A = 0;
            return;
        }
        break;
    case 8:
        D_800AC5D0--;
        steps = 64;
        func_800207D4(D_800ACC14->ordering.ordering70, (steps - D_800AC5D0) * 4, 69);
        if (D_800AC5D0 <= 0) {
            ovl662_D_801272E4 = ovl662_D_801272BA;
            ovl662_func_8011512C();
            D_800F4C6A = ready;
            D_800F3A26 = steps;
            D_800F4D8A = 0;
            return;
        }
        break;
    }
common:
    if (ovl662_D_801272D4 == 0 && (s8)D_800F2AB0[1] == 2) {
        ovl662_D_801272D4 = 1;
        D_800F3AFC[0].half02 = 1;
        D_800F3AFC[1].half02 = 1;
    }
    for (i = 0; i < 2; i++) {
        func_8003EC88(i);
    }
    if (D_800F3AFC[1].state10 == 3) {
        ovl662_func_80116C34(D_800F4D98.entry.parameter);
    }
    i = shown;
    if ((u8)(D_800F2AB0[1] - 1) < 2 && i == 1) {
        func_80037198(0);
        func_8002E5A0(D_800ABEF8);
        fadeLevel = 0x100;
        if ((s8)D_800F2AB0[1] == i) {
            shade = ~((D_800F2AB0[2] * 255) / 30);
            color.channel[0] = shade;
            color.channel[1] = shade;
            color.channel[2] = shade;
            fadeMode = 0x102;
            fadeLevel = (D_800F2AB0[2] << 8) / 30;
        } else {
            fadeMode = 1;
            color.channel[0] = 0x80;
            color.channel[1] = 0x80;
            color.channel[2] = 0x80;
        }
        func_8002CC88(D_800ABEF8, color.rgb, fadeLevel, fadeMode);
    }
    func_8003D6F0(0);
    func_8003D6F0(1);
    if (func_8003FBA8(0) > 0) {
        func_8002B12C(160, 144, 8, 1, ovl662_D_801272E4, -1, ovl662_D_801272E0 + 1);
    }
    func_8002A344(0x1008E);
}

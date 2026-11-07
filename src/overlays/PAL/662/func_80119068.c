#include "rr/overlay662_course_controller.h"
extern s16 ovl662_D_801272BC;
extern s32 ovl662_D_801272E8;
void ovl662_func_8011512C(void);

void ovl662_func_80119068(void) {
    union {RrPreviewRgb3 rgb; u8 channel[3];} color;
    s8 active;
    s32 ready;
    s32 i;
    s32 cell;
    s32 phase;
    s16 prev;
    s16 fade;
    s32 tint;
    s32 mode;
    s8 shade;
    u8 row;
    RrRaceVehicle *vehicle;
    RrUiRow24 *p;
    s32 *tick;

    active = 0;
    tick = &D_800F2DE4;
    vehicle = D_800ABEF8;
    *tick += 1;
    vehicle->rot54 = (vehicle->rot54 + 8) & 0xFFF;
    func_80033A74(1);
    func_8006EF8C(6);
    ready = func_8003AE4C(0, 1);
    if (ready != 1) {
        if ((*(s8*)(D_800F2AB0+1)) == 1) {
            (*(s8*)(D_800F2AB0+1)) = 0;
        }
        goto tail;
    }
    func_8002E394(vehicle);
    if ((*(s8*)(D_800F2AB0+1)) == 7) {
        (*(s8*)(D_800F2AB0+1)) = ready;
    }
    active = 1;
    switch ((u16)D_800F4D8A) {
    case 0:
        func_8002AF8C(1, 1, 1);
        prev = ovl662_D_801272BC;
        func_8002B330(3, 2, (char*)&ovl662_D_801272BC, 17, 0);
        if (prev != ovl662_D_801272BC) {
            D_800F4D98.pad02[0] %= 20;
            if (ovl662_D_801272BC > 0) {
                D_800F4D98.pad02[0] += 20;
            }
            D_800ACBD0[0].first = ovl662_D_801272BC;
            ResetScreenCounters();
            D_800ACC18 = D_800F4D70.first;
            (*(s8*)(D_800F2AB0+1)) = 6;
        }
        D_800F4D80[D_800AC64A].cells[0] = 0;
        func_8002B330(0, 2, (char*)&D_800AC64A, 3, 0);
        cell = 2;
        if (ovl662_D_801272BC == 0) {
            D_800F4D80[1].half20 = 1;
            if (D_800AC64A == 1) {
                cell = 4;
            }
        } else {
            D_800F4D80[1].half20 = 0;
        }
        D_800F4D80[D_800AC64A].cells[0] = cell;
        row = D_800F4D80[0].byte14;
        if (row != 2) break;
        phase = D_800F3AFC->state10;
        if (phase != 3) break;
        if (D_800F3A38[0].a != 0) {
            D_800F4D80[D_800AC64A].cells[0] = 1;
            if (D_800AC64A == 0) {
                func_80014A8C(-1);
                PlaySfx(1);
                D_800F4D8A = phase;
                i = 0;
                do {
                    p = (RrUiRow24 *)((unsigned long)i * sizeof(RrUiRow24) + (unsigned long)D_800F4D80);
                    if (p->byte14 == 2) {
                        p->byte14 = 3;
                    }
                    i++;
                } while (i < 4);
                D_800F3AFC[0].state10 = 4;
                D_800F3AFC[1].state10 = 4;
                (*(s8*)(D_800F2AB0+1)) = 3;
                D_800AC5D0 = 0x40;
            } else if (D_800AC64A == 1) {
                if (ovl662_D_801272BC > 0) {
                    func_80014A8C(-1);
                    PlaySfx(1);
                    D_800F4D8A = 4;
                    i = 0;
                    do {
                        p = (RrUiRow24 *)((unsigned long)i * sizeof(RrUiRow24) + (unsigned long)D_800F4D80);
                        if (p->byte14 == 2) {
                            p->byte14 = 3;
                        }
                        i++;
                    } while (i < 4);
                    D_800F3AFC[0].state10 = 4;
                    D_800F3AFC[1].state10 = 4;
                    (*(s8*)(D_800F2AB0+1)) = 3;
                } else {
                    PlaySfx(4);
                }
            } else if (D_800AC64A == row) {
                func_80014A8C(-1);
                PlaySfx(1);
                i = 0;
                D_800F3AFC[0].state10 = 4;
                D_800F3AFC[1].state10 = 4;
                (*(s8*)(D_800F2AB0+1)) = phase;
                do {
                    p = (RrUiRow24 *)((unsigned long)i * sizeof(RrUiRow24) + (unsigned long)D_800F4D80);
                    if (p->byte14 == 2) {
                        p->byte14 = 3;
                    }
                    i++;
                } while (i < 4);
                goto close;
            }
        } else if (D_800F3A38[0].b != 0) {
            PlaySfx(2);
            func_80014A8C(-1);
            i = 0;
            D_800F3AFC[0].state10 = 4;
            D_800F3AFC[1].state10 = 4;
            (*(s8*)(D_800F2AB0+1)) = phase;
            do {
                p = (RrUiRow24 *)((unsigned long)i * sizeof(RrUiRow24) + (unsigned long)D_800F4D80);
                if (p->byte14 == 2) {
                    p->byte14 = 3;
                }
                i++;
            } while (i < 4);
close:
            D_800AC5D0 = 0x40;
            D_800F4D8A = 8;
        }
        break;
    case 3:
        fade = D_800AC5D0 - 1;
        D_800AC5D0 = fade;
        func_800207D4(((RrFrameStorage*)(void*)D_800ACC14)->ordering.ordering70, (0x40 - fade) * 4, 0x45);
        if ((s16)D_800AC5D0 > 0) break;
        D_800ABE08[1][ovl662_D_801272E4].glyph = func_8003708C(&D_800F4D98);
        D_800ABE08[1][ovl662_D_801272E4].first = D_800ACBD0[0].first;
        D_800ABE08[1][ovl662_D_801272E4].second = D_800ACBD0[0].second;
        ovl662_func_8011512C();
        D_800F4C6A = 1;
        D_800F3A26 = 0x40;
        D_800F4D8A = 0;
        return;
    case 4:
        if (D_800F4D80[0].byte14 == 5 && (*(s8*)(D_800F2AB0+1)) == 5) {
            ovl662_D_801272E8 = 0;
            D_800F4C6A = 1;
            D_800F3A26 = 0x45;
            D_800F4D8A = 0;
        }
        break;
    case 8:
        fade = D_800AC5D0 - 1;
        D_800AC5D0 = fade;
        func_800207D4(((RrFrameStorage*)(void*)D_800ACC14)->ordering.ordering70, (0x40 - fade) * 4, 0x45);
        if ((s16)D_800AC5D0 > 0) break;
        ovl662_func_8011512C();
        D_800F4C6A = 1;
        D_800F3A26 = 0x40;
        D_800F4D8A = 0;
        return;
    }
tail:
    if (ovl662_D_801272D4 == 0 && (*(s8*)(D_800F2AB0+1)) == 2) {
        ovl662_D_801272D4 = 1;
        D_800F3AFC[0].half02 = 1;
        D_800F3AFC[1].half02 = 1;
        i = 0;
    }
    i = 0;
    do {
        func_8003EC88(i);
        i++;
    } while (i < 3);
    i = active;
    if (((u32)((u8)(*(s8*)(D_800F2AB0+1)) - 1) < 2U) && (i == 1)) {
        func_80037198(0);
        func_8002E5A0((char *)(void *)D_800ABEF8);
        tint = 0x100;
        if ((*(s8*)(D_800F2AB0+1)) == i) {
            shade = ~((D_800F2AB0[2] * 0xFF) / 30);
            color.channel[0] = shade;
            color.channel[1] = shade;
            color.channel[2] = shade;
            mode = 0x102;
            tint = (D_800F2AB0[2] << 8) / 30;
        } else {
            mode = 1;
            color.channel[0] = 0x80;
            color.channel[1] = 0x80;
            color.channel[2] = 0x80;
        }
        func_8002CC88(D_800ABEF8, color.rgb, tint, mode);
    }
    func_8003D6F0(0);
    func_8003D6F0(1);
    if (func_8003FBA8(0) > 0) {
        func_8002B12C(0xA0, 0x90, 8, 1, ovl662_D_801272BC, -1, 18);
    }
    func_8002A344(0x10046);
}

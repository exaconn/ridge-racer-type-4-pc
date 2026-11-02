#include "rr/menu_overlay_runtime.h"

/* Versus course selection: each player picks a course group and course. */
extern u8 ovl661_D_80118358[];            /* option count per course group */
extern s8 ovl661_D_8011835C[2][4];        /* option meaning per group slot */
extern s8 ovl661_D_80118364[2][2][4];     /* UI row per player, group, slot */
extern s16 ovl661_D_80118438;
extern s16 ovl661_D_80118440[2][2];       /* course count per player/group */
extern s16 ovl661_D_8011844C[2];          /* selected course per player */
extern s16 ovl661_D_80118450[2];          /* selected group per player */
extern RrCourseChoice4 ovl661_D_80118458[2][2][9];
extern s32 D_800F4BEC, D_800F4CB8;
extern s16 D_800FFC18;
void func_80033A74(s32);
void func_8006EF8C(s32);
s32 func_8003AE4C(s32, s32);
void func_8002E394(RrRaceVehicle *);
void func_8002E5A0(RrRaceVehicle *);
void func_8002CC88(RrRaceVehicle *, RrPreviewRgb3, s32, s32);
void func_8002AF8C(s32, s32, s32);
void func_8002B330(s32, s32, char *, s32, s32);
void func_80037040(s8 *, s32);
s32 func_8003708C(s8 *);
void func_8003A198(void);
void func_80014A8C(s32);
void func_8004F24C(s32);
void func_8003EC88(s32);
void func_80037198(s32);
void func_8003D6F0(s32);
s32 func_8003FBA8(s32);
void func_8002B12C(s32, s32, s32, s32, s32, s32, s32);
void func_8002A344(s32);
void func_800207D4(u32 *, s32, u32);
void func_80053AF4(s32, s32);
void func_80053CB0(void);
void ovl661_func_80115598(s32);
void ovl661_func_80115F8C(void);
void ovl661_func_80117350(void);

#define MODE (((s8 *)D_800F2AB0)[1])
#define SELECTION ((s8 *)&D_800F4D98)
#define ENTRIES ((RrSelectionRow *)&D_800F4D98)
#define PLAYER (D_800AC5D4 & 1)
#define GROUP_ROW(p, g, i) ovl661_D_80118364[p][g][i]

void ovl661_func_801160F0(void)
{
    union { RrPreviewRgb3 rgb; u8 channel[3]; } color;
    s16 active;
    s32 result;
    s32 row;
    u8 status;
    s32 before;
    s32 i;
    s32 close;
    s16 player;
    s16 group;
    s32 brightness;
    s32 flags;
    s32 sound;
    s32 groups;
    s8 last;
    RrRaceVehicle *vehicle;
    RrRaceVehicle *vehicles;
    s32 *tick;
    RrMenuItemState5C *input;

    active = 0;
    tick = &D_800F2DE4;
    vehicles = D_800ABEF8;
    vehicle = &vehicles[PLAYER];
    (*tick)++;
    vehicle->rot54 = (vehicle->rot54 + 8) & 0xFFF;
    func_80033A74(1);
    func_8006EF8C(6);
    result = func_8003AE4C(D_800AC5D4, 1);
    if (result != 1) {
        if (MODE == 1) {
            MODE = 0;
        }
        goto tail;
    }
    func_8002E394(&vehicles[PLAYER]);
    if (MODE == 7) {
        MODE = result;
    }
    active = 1;
    switch (D_800F4D8A) {
    case 7:
        row = GROUP_ROW(PLAYER, (s16)(ovl661_D_80118450[PLAYER] ^ 1), 0);
        if (D_800F4D80[row].byte14 == 5) {
            D_800F4D8A = 0;
        }
    case 0:
        func_8002AF8C(1, 1, 1);
        before = func_8003708C(SELECTION + D_800AC5D4 * 4);
        func_8002B330(PLAYER * 4 | 1, 2, (char *)&ovl661_D_8011844C[PLAYER],
                      ovl661_D_80118440[PLAYER][ovl661_D_80118450[PLAYER]], 0);
        func_80037040(SELECTION + D_800AC5D4 * 4,
                      ovl661_D_80118458[PLAYER][ovl661_D_80118450[PLAYER]][ovl661_D_8011844C[PLAYER]].glyph);
        if ((s16)before != (s16)func_8003708C(SELECTION + D_800AC5D4 * 4)) {
            func_8003A198();
            D_800ACC18 = D_800F4D70.first;
            func_80037040(SELECTION + D_800AC5D4 * 4,
                          ovl661_D_80118458[PLAYER][ovl661_D_80118450[PLAYER]][ovl661_D_8011844C[PLAYER]].glyph);
            D_800ACBD0[D_800AC5D4].first = ovl661_D_80118458[PLAYER][ovl661_D_80118450[PLAYER]][ovl661_D_8011844C[PLAYER]].first;
            D_800ACBD0[D_800AC5D4].second = ovl661_D_80118458[PLAYER][ovl661_D_80118450[PLAYER]][ovl661_D_8011844C[PLAYER]].second;
            MODE = 6;
        }
        row = GROUP_ROW(PLAYER, ovl661_D_80118450[PLAYER], D_800AC64A);
        D_800F4D80[row].cells[0] = 0;
        func_8002B330(PLAYER * 4, 2, (char *)&D_800AC64A, ovl661_D_80118358[ovl661_D_80118450[PLAYER]], 0);
        row = GROUP_ROW(PLAYER, ovl661_D_80118450[PLAYER], D_800AC64A);
        D_800F4D80[row].cells[0] = 2;
        i = D_800F4D80[row].byte14;
        close = 0;
        if (i != 2) {
            break;
        }
        if (D_800F3AFC->state10 != 3) {
            break;
        }
        {
            RrMenuItemState5C *inputs = D_800F3A38;
            input = inputs + PLAYER;
        }
        if (input->a != 0) {
            func_8004F24C(1);
            D_800F4D80[row].cells[0] = 1;
            if (D_800AC64A == ovl661_D_8011835C[ovl661_D_80118450[PLAYER]][0]) {
                ovl661_D_80118438 = i;
                D_800F4D8A = 4;
            } else if (D_800AC64A == ovl661_D_8011835C[ovl661_D_80118450[PLAYER]][1]
                       && ovl661_D_80118458[D_800AC5D4][1][0].glyph != -1) {
                ovl661_D_8011844C[PLAYER] = 0;
                D_800AC64A = 0;
                for (i = 0; i < 4; i++) {
                    row = GROUP_ROW(PLAYER, ovl661_D_80118450[PLAYER], i);
                    if (row == -1) {
                        break;
                    }
                    if (D_800F4D80[row].byte14 == 2) {
                        D_800F4D80[row].byte14 = 3;
                    }
                }
                for (i = 0; i < 4;) {
                    row = GROUP_ROW(PLAYER, (s16)(ovl661_D_80118450[PLAYER] ^ 1), i);
                    i++;
                    if (row == -1) {
                        break;
                    }
                    D_800F4D80[row].byte14 = 0;
                }
                D_800F4D8A = 7;
                ovl661_D_80118450[PLAYER] ^= 1;
            } else if (D_800AC64A == ovl661_D_8011835C[ovl661_D_80118450[PLAYER]][2]) {
                close = 1;
            } else if (D_800AC64A == ovl661_D_8011835C[ovl661_D_80118450[PLAYER]][3]) {
                for (i = 0; i < D_800F4BEC; i++) {
                    if (D_800F4D80[i].byte14 == 2) {
                        D_800F4D80[i].byte14 = 3;
                    }
                }
                MODE = 3;
                D_800AC5D0 = 0x40;
                D_800F4D8A = 10;
                D_800F3AFC->state10 = 4;
            }
        } else if (input->b != 0) {
            func_8004F24C(2);
            close = 1;
        }
        if (close == 1) {
            for (i = 0; i < D_800F4BEC; i++) {
                if (D_800F4D80[i].byte14 == 2) {
                    D_800F4D80[i].byte14 = 3;
                }
            }
            MODE = 3;
            D_800AC5D0 = 0x40;
            D_800F3AFC->state10 = 4;
            func_80053AF4(0, 60);
            D_800F4D8A = 8;
        }
        break;
    case 3:
        row = GROUP_ROW((D_800AC5D4 - 1) & 1, 1, 3) + 1;
        if (D_800F4D80[row].byte14 == 5) {
            ovl661_D_80118438 = 1;
            D_800F4D8A = 0;
        }
        break;
    case 4:
        row = GROUP_ROW(PLAYER, 1, 3) + 1;
        i = D_800F4D80[row].byte14;
        if (i != 2) {
            break;
        }
        D_800F4D80[row + ENTRIES[D_800AC5D4].reserved].cells[0] = 0;
        func_8002B330(PLAYER * 4, 1, (char *)&ENTRIES[D_800AC5D4].reserved, 2, 0);
        D_800F4D80[row + ENTRIES[D_800AC5D4].reserved].cells[0] = i;
        {
            RrMenuItemState5C *inputs = D_800F3A38;
            input = inputs + PLAYER;
        }
        if (input->a != 0) {
            func_8004F24C(1);
            D_800F4D80[row + ENTRIES[0].reserved].cells[0] = 1;
            D_800F4D80[row].byte14 = 3;
            D_800F4D80[row + 1].byte14 = 3;
            for (i = 0; i < 4; i++) {
                row = GROUP_ROW(PLAYER, ovl661_D_80118450[PLAYER], i);
                if (row == -1) {
                    break;
                }
                if (D_800F4D80[row].byte14 == 2) {
                    D_800F4D80[row].byte14 = 3;
                }
            }
            active = 0;
            D_800F4D70.first = D_800ACC18;
            if ((D_800AC5D4 & 1) > 0) {
                D_800F4D8A = 5;
                for (i = 0; i < D_800F4BEC; i++) {
                    if (D_800F4D80[i].byte14 == 2) {
                        D_800F4D80[i].byte14 = 3;
                    }
                }
                MODE = 3;
                D_800F3AFC->state10 = 4;
            } else {
                D_800F4D8A = 3;
                D_800AC5D4++;
                ovl661_func_80115F8C();
            }
        } else if (input->b != 0) {
            func_8004F24C(2);
            D_800F4D80[row].byte14 = 3;
            D_800F4D80[row + 1].byte14 = 3;
            D_800F4D8A = 6;
        }
        break;
    case 5:
        func_8002AF8C(0, 1, 1);
        row = GROUP_ROW(PLAYER, 1, 3) + 1;
        status = D_800F4D80[row].byte14;
        if (status == 5 && MODE == status) {
            func_8003A198();
            ovl661_func_80117350();
            D_800F4C6A = 1;
            D_800F3A26 = 0x39;
            D_800F4D8A = 0;
        }
        break;
    case 6:
        row = GROUP_ROW(PLAYER, 1, 3) + 1;
        if (D_800F4D80[row].byte14 == 5) {
            D_800F4D8A = 0;
            ovl661_D_80118438 = 1;
            D_800F4D80[row].byte14 = 0;
            D_800F4D80[row + 1].byte14 = 0;
        }
        break;
    case 9:
        row = GROUP_ROW(PLAYER, 1, 3) + 1;
        status = D_800F4D80[row].byte14;
        if (status == 5 && MODE == status) {
            ovl661_func_80115598(0);
            D_800F4D8A = 0;
        }
        break;
    case 8:
        func_8002AF8C(0, 1, 1);
        D_800AC5D0--;
        func_80014A8C(-1);
        func_800207D4(D_800ACC14->ordering.ordering70, (0x40 - D_800AC5D0) * 4, 0x45);
        if (D_800AC5D0 <= 0) {
            func_80053CB0();
            D_800F4C6A = 1;
            D_800F3A26 = 3;
            D_800F4D8A = 0;
        }
        break;
    case 10:
        func_8002AF8C(0, 1, 1);
        func_80014A8C(-1);
        D_800AC5D0--;
        func_800207D4(D_800ACC14->ordering.ordering70, (0x40 - D_800AC5D0) * 4, 0x45);
        if (D_800AC5D0 <= 0) {
            D_800FFC18 = 2;
            D_800F4C6A = 1;
            D_800F3A26 = 0x47;
            D_800F4D8A = 0;
        }
        break;
    }
tail:
    if (ovl661_D_80118438 == 0 && MODE == 2) {
        ovl661_D_80118438 = 1;
        D_800F3AFC->half02 = 1;
    }
    if (D_800F4D8A == 3) {
        player = D_800AC5D4 - 1;
    } else {
        player = D_800AC5D4;
    }
    if (D_800F4D8A == 7) {
        group = ovl661_D_80118450[player & 1] ^ 1;
        row = GROUP_ROW(player & 1, group, 0);
        if (D_800F4D80[row].byte14 == 5) {
            group = ovl661_D_80118450[player & 1];
        }
    } else {
        group = ovl661_D_80118450[player & 1];
    }
    for (i = 0; i < 4; i++) {
        row = GROUP_ROW(player & 1, group, i);
        if (row == -1) {
            break;
        }
        func_8003EC88(row);
    }
    if (ovl661_D_80118438 == 2) {
        last = GROUP_ROW(player & 1, 1, 3);
        func_8003EC88(last + 1);
        func_8003EC88(last + 2);
    }
    if ((u8)MODE - 1 < 2U) {
        i = active;
        if (i == 1) {
            func_80037198(D_800AC5D4);
            func_8002E5A0(&D_800ABEF8[PLAYER]);
            brightness = 0x100;
            if (MODE == i) {
                last = ~((D_800F2AB0[2] * 0xFF) / 30);
                color.channel[0] = last;
                color.channel[1] = last;
                color.channel[2] = last;
                flags = 0x102;
                brightness = (D_800F2AB0[2] << 8) / 30;
            } else {
                flags = 1;
                color.channel[0] = 0x80;
                color.channel[1] = 0x80;
                color.channel[2] = 0x80;
            }
            func_8002CC88(&D_800ABEF8[PLAYER], color.rgb, brightness, flags);
        }
    }
    func_8003D6F0(0);
    if (func_8003FBA8(0) > 0) {
        func_8002B12C(0xA0, 0x90, 8, 1, ovl661_D_8011844C[PLAYER], 0,
                      ovl661_D_80118440[PLAYER][ovl661_D_80118450[PLAYER]]);
    }
    D_800F4CB8 = D_800AC5D4 + 1;
    sound = 0x10032;
    /* Integer base address: the original's table read is not disambiguated
     * from the D_800F4CB8 store above. */
    groups = (s32)ovl661_D_80118450;
    if (*(s16 *)(groups + (D_800AC5D4 & 1) * 2) != 0) {
        sound = 0x10036;
    }
    func_8002A344(sound);
}

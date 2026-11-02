#include "rr/menu_text_rows.h"
#include "rr/overlay_menu.h"
#include "rr/standings.h"
#include "rr/menu_selection.h"

extern void ovl660_func_801162D8(void);
extern s32 ovl660_func_80115FA8(s32, s32);
extern void func_8002AF8C(s32, s32, s32);

extern s32 D_800F2DE4;
extern s32 D_800F4C64;
extern s32 ovl660_D_80117910;
extern s32 ovl660_D_80117938;
extern s16 D_800AC64A;
extern s16 D_800AC5E0;
extern s16 D_800F3914;
extern s32 ovl660_D_80117820[55];
extern s32 D_800F3A48;
extern s16 ovl660_D_80117918;
extern s32 ovl660_D_80117930;

void ovl660_func_801154D8(void)
{
    s32 i;
    s32 off;
    s32 mode;

    D_800F2DE4 = 0;
    if (D_800F4C64 != -1) {
        ovl660_func_801162D8();
        ovl660_D_80117910 = ovl660_func_80115FA8(0, D_800F4C80[(s16)D_800AC1A0]);
        ovl660_D_80117938 = ovl660_func_80115FA8(1, D_800F4C64);
    } else {
        ovl660_D_80117910 = 6;
        ovl660_D_80117938 = 6;
    }
    func_800402F4(D_800FFC48);
    D_800AC64A = 0;
    D_800AC5E0 = 3;
    off = 0;
    if (D_800F3914 == 7) {
        off = 11;
    }
    for (i = 0; i < 44; i++) {
        func_80040338(i, i % 11, (i >= 11) * 2, 0, ovl660_D_80117820[off + i]);
        if (i == 10) {
            off = 11;
        }
    }
    for (i = 44; i < 55; i++) {
        func_80040338(i, i - 44, 0, 0, -1);
    }
    ovl660_D_80117918 = 0;
    mode = D_800F3A48;
    if (mode == 2) {
        ovl660_D_80117918 = mode;
    } else if (mode == 1) {
        ovl660_D_80117918 = mode;
    }
    ovl660_D_80117930 = 0;
    func_8002AF8C(0, 1, 0);
}

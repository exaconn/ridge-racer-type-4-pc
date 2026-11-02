#include "rr/menu_text_rows.h"
#include "rr/overlay_menu.h"

extern s16 D_800AC64A;
extern s16 D_800AC5E0;
extern s32 ovl661_D_80118378[22];
extern s32 D_800F4C64;
extern s16 ovl661_D_80118448[2];
extern s16 ovl661_D_80118410[2];
extern s16 ovl661_D_80118418[2];
extern s32 D_800F2DE4;

extern void func_8002AF8C(s32, s32, s32);

void ovl661_func_80117C78(void) {
    s32 i;

    func_800402F4(D_800FFC48);
    D_800AC64A = 0;
    D_800AC5E0 = 3;
    for (i = 0; i < 11; i++) {
        func_80040338(i, i, 0, 0, ovl661_D_80118378[i]);
    }
    for (i = 11; i < 22; i++) {
        func_80040338(i, i - 11, 0, 0, -1);
    }
    for (i = 33; i < 44; i++) {
        func_80040338(i, i - 33, 2, 0, ovl661_D_80118378[i - 22]);
    }
    if (D_800F4C64 >= 0) {
        if (ovl661_D_80118448[0] > ovl661_D_80118448[1]) {
            ovl661_D_80118410[0] = 1;
            ovl661_D_80118410[1] = 0;
        } else {
            ovl661_D_80118410[0] = 0;
            ovl661_D_80118410[1] = 1;
        }
        if (ovl661_D_80118418[ovl661_D_80118410[0]] < 99) {
            ovl661_D_80118418[ovl661_D_80118410[0]]++;
        }
    }
    D_800F2DE4 = 0;
    func_8002AF8C(0, 1, 0);
}

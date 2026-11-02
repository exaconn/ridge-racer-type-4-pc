#include "rr/menu_text_rows.h"
#include "rr/menu_choice.h"
extern void ovl659_func_801158B4(void);
extern s16 D_800AC64A,D_800AC5E0;
extern s32 ovl659_D_80116534[34],ovl659_D_801165BC,D_800F2DE4;
void ovl659_func_801150CC(void)
{
    s32 i;
    s32 offset;

    ovl659_func_801158B4();
    func_800402F4(D_800FFC48);
    D_800AC64A = 0;
    D_800AC5E0 = 2;
    offset = 0;
    if (D_800AD4D8.mode1A != 4) {
        offset = 11;
    }
    for (i = 0; i < 11; i++) {
        if (D_800AD4D8.phase17 >= 4 && i == 4 && offset == 0) {
            func_80040338(i, i, 0, 0, ovl659_D_80116534[22]);
        } else {
            func_80040338(i, i, 0, 0, ovl659_D_80116534[i + offset]);
        }
    }
    for (i = 0; i < 11; i++) {
        func_80040338(i + 33, i, 2, 0, ovl659_D_80116534[i + 23]);
    }
    func_8002AF8C(0, 1, 0);
    ovl659_D_801165BC = -2;
    D_800F2DE4 = 0;
}

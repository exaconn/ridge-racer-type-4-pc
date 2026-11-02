#include "rr/menu_text_rows.h"
#include "rr/overlay_menu.h"

#include "rr/save_progress.h"
extern s16 D_800AC64A;
extern s16 D_800AC5E0;
extern s32 ovl666_D_80116F94;
extern s32 ovl666_D_80116EE4[33];
extern s32 D_800F2DE4;
extern s32 D_800ABEB0;

extern void func_8002AF8C(s32, s32, s32);

void ovl666_func_80115388(void) {
    s32 i;
    s32 base;

    /* The selected label bank is0..2; the two progress selectors are0..3. */
    func_800402F4(D_800FFC48);
    D_800AC64A = 0;
    D_800AC5E0 = 3;
    if (ovl666_D_80116F94 == 0) {
        D_800F3920.prefix.flags5C[D_800F4D98.bytes[1]] = 1;
        D_800F3920.prefix.progress.progress10[D_800F4D98.bytes[0]][D_800F4D98.bytes[1]] |= 0x80000;
    }
    base = ovl666_D_80116F94 * 11;
    for (i = 0; i < 11; i++) {
        func_80040338(i, i, 0, 0, ovl666_D_80116EE4[i + base]);
    }
    for (i = 11; i < 22; i++) {
        func_80040338(i, i - 11, 0, 0, -1);
    }
    for (i = 33; i < 44; i++) {
        func_80040338(i, i - 33, 2, 0, ovl666_D_80116EE4[i - 11]);
    }
    func_8002AF8C(0, 1, 0);
    D_800F2DE4 = 0;
    D_800ABEB0 = 0;
}

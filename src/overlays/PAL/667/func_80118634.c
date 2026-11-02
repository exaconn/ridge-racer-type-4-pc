#include "rr/menu_text_rows.h"
#include "rr/overlay_menu.h"
extern s32 D_800F2DE4;
extern s16 D_800AC64A;
extern s16 D_800AC5E0;
extern s32 ovl667_D_80118E2C[22];
extern s32 D_800F4C64;
extern s16 D_800AC59C;
extern s16 D_8010C070[8];
extern s16 D_8010C080[8];
extern s16 D_8010C0C8[8];
extern s16 D_800FF688;
extern void func_8002AF8C(s32, s32, s32);

void ovl667_func_80118634(void) {
    s32 i;

    /* The active participant count is0..8; order entries and the preexisting
     * inverse position for participant0 are valid slots0..7. */
    D_800F2DE4 = 0;
    func_800402F4(D_800FFC48);
    D_800AC64A = 0;
    D_800AC5E0 = 3;
    for (i = 0; i < 11; i++) {
        func_80040338(i, i, 0, 0, ovl667_D_80118E2C[i]);
    }
    for (i = 11; i < 22; i++) {
        func_80040338(i, i - 11, 0, 0, -1);
    }
    for (i = 33; i < 44; i++) {
        func_80040338(i, i - 33, 2, 0, ovl667_D_80118E2C[i - 22]);
    }
    if (D_800F4C64 >= 0) {
        for (i = 0; i < D_800AC59C; i++) {
            D_8010C070[D_8010C0C8[i]] = i;
        }
        if (D_8010C080[D_8010C070[0]] < 99) {
            D_8010C080[D_8010C070[0]]++;
        }
    }
    D_800FF688 = 0;
    func_8002AF8C(0, 1, 0);
}

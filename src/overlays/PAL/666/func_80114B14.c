#include "rr/diagnostics.h"
#include "rr/overlay666_preview.h"
#include "rr/frame_environment.h"
extern RrCourseChoice4 D_800ABE08[2][9];
extern s16 ovl666_D_80116F88[4];
extern const char ovl666_D_8011487C[];
extern u16 D_800F4C6A;
extern s16 D_800F3A26,D_800F4D8A;
void func_80092CB8(s32 mode);
void func_80053BB4(s32 mode);
void ovl666_func_801148D4(void);
void ovl666_func_80114B14(void)
{
    s32 i;
    s32 j;

    func_80092CB8(0);
    func_8002A604();
    func_8001ECE8(0, 0, 0, 2);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 9; j++) {
            ovl666_D_80116FA0[i][j].glyph = -1;
        }
    }
    for (i = 0; i < 4; i++) {
        ovl666_D_80116F88[i] = 0;
        for (j = 0; j < 9; j++) {
            func_80037040((s8 *)(u8 *)&D_800F4D98, D_800ABE08[1][j].glyph);
            if (D_800F4D98.variant == i && D_800F4D98.pad02[0] < 39) {
                ovl666_D_80116FA0[i][ovl666_D_80116F88[i]] = D_800ABE08[1][j];
                ovl666_D_80116F88[i]++;
            }
        }
    }
    D_800AC648 = 0;
    D_800F3826 = 0;
    func_8001E84C(ovl666_D_8011487C);
    ovl666_func_801148D4();
    D_800F4C6A = 1;
    D_800F3A26 = 79;
    D_800F4D8A = 0;
    func_80053BB4(0);
}

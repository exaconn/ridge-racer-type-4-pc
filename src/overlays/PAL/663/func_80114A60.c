#include "rr/diagnostics.h"
#include "rr/overlay_menu_init.h"
#include "rr/menu_transition.h"
#include "rr/frame_environment.h"
#include "rr/save_progress.h"
#include "rr/course_choice_types.h"
#include "rr/string_tail_copy.h"
extern RrCourseChoice4 D_800ABE08[2][9];
void func_80053BB4(s32 mode);
extern const char ovl663_D_8011487C[];
extern u8 ovl663_D_80116868[9];
extern s32 ovl663_D_801168A0[9],ovl663_D_801168D0[9],ovl663_D_8011689C;
void ovl663_func_80114A60(void)
{
    s32 i;
    s32 val;

    func_80092CB8(0);
    func_8001ECE8(0, 0, 0, 2);
    D_800F4C6A = 1;
    D_800F3A26 = 74;
    D_800F4D8A = 0;
    func_8001E84C(ovl663_D_8011487C);
    ovl663_D_801168CC = 0;
    for (i = 0; i < 9; i++) {
        if (D_800F3920.unlocked60[ovl663_D_80116868[i]] != 0) {
            ovl663_D_801168A0[ovl663_D_801168CC] = ovl663_D_80116868[i];
            ovl663_D_801168D0[ovl663_D_801168CC++] = D_800F3920.unlocked60[ovl663_D_80116868[i]];
        }
    }
    ovl663_func_801148D8();
    func_80053BB4(0);
    ovl663_D_8011689C = 4;
    val = D_800F3920.prefix.progress.prefix00.fields.configuration0C;
    if (val >= 16) {
        ovl663_D_8011689C = 8;
    }
    if (val & 0x100) {
        ovl663_D_8011689C = 16;
    }
}

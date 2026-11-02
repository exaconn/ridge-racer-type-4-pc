#include "rr/diagnostics.h"
#include "rr/overlay_menu_init.h"
#include "rr/menu_transition.h"
#include "rr/frame_environment.h"
#include "rr/save_progress.h"
#include "rr/course_choice_types.h"
#include "rr/string_tail_copy.h"
#include "rr/menu_glyph.h"
extern RrCourseChoice4 D_800ABE08[2][9];
void func_80053BB4(s32 mode);
extern s16 ovl660_D_8011793C,ovl660_D_80117934,ovl660_D_8011792C;
extern char ovl660_D_80117920[12];
extern s32 ovl660_D_8011790C;
extern const char ovl660_D_8011487C[];
void ovl660_func_80114920(void);
void ovl660_func_80114C40(void)
{
    s32 val;

    func_80092CB8(0);
    func_8002A604();
    ovl660_D_8011793C = 0;
    ovl660_D_80117934 = 0;
    ovl660_D_8011792C = 0;
    func_8002B75C(ovl660_D_80117920, D_800F3920.prefix.progress.prefix00.fields.name);
    func_8001ECE8(0, 0, 0, 2);
    val = D_800F3920.prefix.progress.prefix00.fields.configuration0C;
    D_800AC648 = 0;
    ovl660_D_8011790C = 4;
    if (val >= 16) {
        ovl660_D_8011790C = 8;
    }
    if (val & 0x100) {
        ovl660_D_8011790C = 16;
    }
    ovl660_func_80114920();
    func_80037040((s8*)D_800F4D98.bytes,D_800ABE08[0][0].glyph);
    D_800F4C6A = 1;
    D_800F3A26 = 50;
    D_800F4D8A = 0;
    func_8001E84C(ovl660_D_8011487C);
    func_80053BB4(0);
}

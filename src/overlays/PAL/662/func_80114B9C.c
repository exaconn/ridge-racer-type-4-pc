#include "rr/diagnostics.h"
#include "rr/overlay_menu_init.h"
#include "rr/menu_transition.h"
#include "rr/frame_environment.h"
#include "rr/save_progress.h"
#include "rr/course_choice_types.h"
#include "rr/string_tail_copy.h"
extern RrCourseChoice4 D_800ABE08[2][9];
void func_80053BB4(s32 mode);
#include "rr/overlay662_progress_init.h"
extern const char ovl662_D_8011487C[];
void SetModelTexturePage(s32 mode);
void ovl662_func_80114B9C(void) {
    SetDispMask(0);
    ClearScreenOverlayState();
    ovl662_func_80114998();
    SetupDrawDispEnvs(0, 0, 0, 2);
    SetModelTexturePage(25);
    ModeGroup = 1;
    D_800F3A26 = 40;
    D_800F4D8A = 0;
    func_8001E84C(ovl662_D_8011487C);
    if (SaveProgress.prefix.reserved50.words[0] == 1) {
        if (D_800ABE08[1][7].glyph >= 0) {
            D_800ABE08[1][8].glyph = 359;
            D_800ABE08[1][8].first = 1;
        }
    }
    func_80053BB4(0);
}

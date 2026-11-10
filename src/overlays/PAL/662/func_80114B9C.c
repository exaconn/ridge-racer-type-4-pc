#include "rr/diagnostics.h"
#include "rr/overlay_menu_init.h"
#include "rr/menu_transition.h"
#include "rr/frame_environment.h"
#include "rr/save_progress.h"
#include "rr/course_choice_types.h"
#include "rr/string_tail_copy.h"
extern CourseChoice4 CourseChoiceTable[2][9];
void SeqStartTrack(s32 mode);
#include "rr/overlay662_progress_init.h"
extern const char ovl662_str_init_garage_scene[];
void SetModelTexturePage(s32 mode);
void ovl662_init_garage_scene(void) {
    SetDispMask(0);
    ClearScreenOverlayState();
    ovl662_init_progress_menu();
    SetupDrawDispEnvs(0, 0, 0, 2);
    SetModelTexturePage(25);
    ModeGroup = 1;
    ModeIndex = 40;
    SubmodeState = 0;
    DebugPrintStub(ovl662_str_init_garage_scene);
    if (SaveProgress.prefix.reserved50.words[0] == 1) {
        if (CourseChoiceTable[1][7].glyph >= 0) {
            CourseChoiceTable[1][8].glyph = 359;
            CourseChoiceTable[1][8].first = 1;
        }
    }
    SeqStartTrack(0);
}

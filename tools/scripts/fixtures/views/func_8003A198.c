/* CC1_FLAGS: -fno-strength-reduce */

#include "rr/menu_transition.h"

/* legacy typing of shared globals in this part of the unit */
#define SetDispMask ((int (*)())SetDispMask)

#define IntroScriptStep ((int (*)())IntroScriptStep)

#define D_800F4D70 (*(int (*))&D_800F4D70)

#define ModeGroup (*(short (*))&ModeGroup)

#undef SetDispMask

#undef IntroScriptStep

#undef D_800F4D70

#undef ModeGroup

extern s32 ScreenFrame;

void ResetScreenCounters(void) {
    LoadState = 0;
    MenuScriptStep = 0;
    ScreenFrame = 0;
}

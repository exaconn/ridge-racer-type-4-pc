/* CC1_FLAGS: -fno-strength-reduce */

#include "rr/menu_transition.h"

/* legacy typing of shared globals in this part of the unit */
#define SetDispMask ((int (*)())SetDispMask)

#define IntroScriptStep ((int (*)())IntroScriptStep)

#define MenuArenas (*(int (*))&MenuArenas)

#define ModeGroup (*(short (*))&ModeGroup)

#undef SetDispMask

#undef IntroScriptStep

#undef MenuArenas

#undef ModeGroup

extern s32 ScreenFrame;

void ResetScreenCounters(void) {
    LoadState = 0;
    MenuScriptStep = 0;
    ScreenFrame = 0;
}

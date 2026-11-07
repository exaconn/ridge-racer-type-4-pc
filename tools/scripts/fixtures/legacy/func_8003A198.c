#include "types.h"
#include "m2c_macros.h"

extern s32 ScreenFrame;
extern s8 MenuScriptStep;
extern s8 LoadState;

void ResetScreenCounters(void) {
    LoadState = 0;
    MenuScriptStep = 0;
    ScreenFrame = 0;
}

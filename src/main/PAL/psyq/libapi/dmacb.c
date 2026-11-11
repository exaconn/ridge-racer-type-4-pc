/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "types.h"
#include "rr/spu_tick.h"

extern s32 CurrentVideoMode;

s32 SetVideoMode(s32 value);

s32 SetVideoMode(s32 value) {
    s32 previous = CurrentVideoMode;
    CurrentVideoMode = value;
    return previous;
}

s32 GetVideoMode(void) {
    return CurrentVideoMode;
}

/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/channels.h"
s32 _padSetMainMode(PadChannel *raw, s32 first_raw, s32 second_raw) {
    u8 copy = first_raw;
    if (PadChannelBusyFn(raw)) {
        return 0;
    }
    raw->phase = 1;
    raw->update14 = _padMainModeUpdate;
    raw->complete18 = _padMainModeComplete;
    raw->modeArgs51[0] = first_raw;
    raw->modeArgs51[1] = second_raw;
    raw->modeEqual53 = (copy == raw->argumentE4);
    return 1;
}

/* CC1_FLAGS: -mno-split-addresses */
#include "rr/channels.h"
s32 _padSetMainMode(PadChannel *raw, s32 first_raw, s32 second_raw) {
    /* ASM_BARRIER: Keep the comparison copy in s3 after saving raw/first/second in s0..s2. */
    register s32 copy __asm__("$19");
    ChannelStatusCallback ready = PadChannelBusyFn;
    s32 result;

    copy = first_raw;
    if (ready(raw)) {
        return 0;
    }
    result = 1;
    /* ASM_BARRIER: Separate success from byte constants and capture argument only after readiness. */
    __asm__("" : "+r"(result) : : "memory");
    {
        u32 wanted = raw->argumentE4;
        raw->phase = 1;
        raw->update14 = _padMainModeUpdate;
        raw->complete18 = _padMainModeComplete;
        raw->modeArgs51[0] = first_raw;
        raw->modeArgs51[1] = second_raw;
        raw->modeEqual53 = ((u8)copy == wanted);
        /* ASM_BARRIER: Preserve the unmasked saved word until the comparison has been written. */
        __asm__("" : : "r"(copy));
    }
    return result;
}

/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "psyq/libspu/s_snc.h"
#include "rr/spu.h"

s32 SpuSetNoiseClock(s32 a0) {
    s32 v;
    s32 r;

    v = a0;
    /* ASM_BARRIER: keep the copy of a0 as a separate pseudo so the clamp tests the copy as the original does */
    __asm__ volatile("" : "+r"(v));
    if (v < 0) {
        r = 0;
    } else {
        r = v;
        if (r >= 0x40) {
            r = 0x3F;
        }
    }
    _spu_RXX->spu_control = (_spu_RXX->spu_control & 0xC0FF) | ((r & 0x3F) << 8);
    return r;
}

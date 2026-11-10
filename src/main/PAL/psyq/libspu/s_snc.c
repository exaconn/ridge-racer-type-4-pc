/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "psyq/libspu/s_snc.h"
#include "rr/spu.h"

s32 SpuSetNoiseClock(s32 a0) {
    u32 u = a0;
    s32 v = u;
    s32 r;

    if (v < 0) {
        r = 0;
    } else if (v >= 0x40) {
        r = 0x3F;
    } else {
        r = v;
    }
    _spu_RXX->spu_control = (_spu_RXX->spu_control & 0xC0FF) | ((r & 0x3F) << 8);
    return r;
}

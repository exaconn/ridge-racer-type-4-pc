/* CC1_FLAGS: -mno-split-addresses */
#include "rr/spu.h"
u32 _spu_Fw(const u16 *a0, u32 a1) {
    if (_spu_transMode == 0) {
        _spu_t(2, (u32)_spu_tsa << _spu_mem_mode_plus);
        _spu_t(1);
        _spu_t(3, a0, a1);
    } else {
        _spu_FwriteByIO(a0, a1);
    }
    return a1;
}

/* CC1_FLAGS: -mno-split-addresses */
#include "rr/spu.h"
u32 func_8007F2E8(const u16 *a0, u32 a1) {
    if (D_800A47F4 == 0) {
        func_8007F068(2, (u32)D_800A47F0 << D_800A4800);
        func_8007F068(1);
        func_8007F068(3, a0, a1);
    } else {
        func_8007ED44(a0, a1);
    }
    return a1;
}

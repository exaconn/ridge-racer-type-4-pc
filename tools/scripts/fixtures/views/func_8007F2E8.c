/* GCC_VERSION: 970318 */

/* MASPSX_FLAGS: --assembler-profile=gnu */

#include "rr/spu.h"

/* legacy typing of shared globals in this part of the unit */
#define _spu_RXX ((struct H *)_spu_RXX)

#define _spu_tsa (*(unsigned short (*))&_spu_tsa)

#undef _spu_RXX

#undef _spu_tsa

/* legacy typing of shared globals in this part of the unit */
#define _spu_RXX ((unsigned short *)_spu_RXX)

#undef _spu_RXX

/* legacy typing of shared globals in this part of the unit */
#define _spu_RXX ((unsigned short *)_spu_RXX)

#undef _spu_RXX

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

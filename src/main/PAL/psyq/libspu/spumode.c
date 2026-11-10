/* GCC_VERSION: 2.7.2 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "types.h"
#include "psyq/libspu/spuwait.h"
#include "psyq/libspu/spuregs2.h"
#include "rr/spu.h"

#include "psyq/libspu/spumode.h"

void SpuSetTransferMode(s32 mode) {
    s32 selected;
    switch (mode) {
    case 0: selected = 0; break;
    case 1: selected = 1; break;
    default: selected = 0; break;
    }
    _spu_trans_mode = mode;
    _spu_transMode = selected;
}

s32 SsVabTransCompleted(s32 wait) {
    return (s16)SpuIsTransferCompleted((s16)wait);
}

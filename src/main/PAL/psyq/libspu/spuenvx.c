#include "types.h"
#include "m2c_macros.h"
#include "psyq/libspu/spuenvx.h"

extern s32 _spu_RXX;

void SpuGetVoiceEnvelope(s32 arg0, u16 *arg1) {
    *arg1 = M2C_FIELD(((arg0 * 0x10) + _spu_RXX), u16 *, 0xC);
}

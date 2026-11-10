#include "types.h"
#include "rr/spu.h"
#include "psyq/libspu/spuenvx.h"

void SpuGetVoiceEnvelope(s32 arg0, u16 *arg1) {
    *arg1 = (arg0 + _spu_RXX->voices)->envx;
}

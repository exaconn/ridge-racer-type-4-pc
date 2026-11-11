#include "types.h"
#include "rr/gpu.h"
#include "psyq/libgpu/settrans.h"

void SetSemiTrans(void *packet, s32 enabled) {
    GpuPrimitiveHeader *header = packet;
    u8 code;

    if (enabled != 0) {
        code = header->color.bytes.code | 2;
    } else {
        code = header->color.bytes.code & 0xFD;
    }
    header->color.bytes.code = code;
}

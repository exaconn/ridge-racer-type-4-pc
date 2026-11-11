#include "types.h"
#include "rr/gpu.h"
#include "psyq/libgpu/primattr.h"

void SetShadeTex(void *packet, s32 enabled) {
    GpuPrimitiveHeader *header = packet;
    u8 code;

    if (enabled != 0) {
        code = header->color.bytes.code | 1;
    } else {
        code = header->color.bytes.code & 0xFE;
    }
    header->color.bytes.code = code;
}

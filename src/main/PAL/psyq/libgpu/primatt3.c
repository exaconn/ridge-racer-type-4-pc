#include "types.h"
#include "rr/gpu.h"
#include "psyq/libgpu/primatt3.h"

void SetPolyFT3(void *packet) {
    GpuPrimitiveHeader *header = packet;
    header->tag.bytes.length = 7;
    header->color.bytes.code = 0x24;
}

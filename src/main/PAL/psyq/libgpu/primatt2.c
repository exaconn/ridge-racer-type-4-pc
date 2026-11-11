#include "types.h"
#include "rr/gpu.h"
#include "psyq/libgpu/primatt2.h"

void SetPolyF3(void *packet) {
    GpuPrimitiveHeader *header = packet;
    header->tag.bytes.length = 4;
    header->color.bytes.code = 0x20;
}

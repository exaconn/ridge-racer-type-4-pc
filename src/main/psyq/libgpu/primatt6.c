#include "types.h"
#include "rr/gpu.h"
#include "psyq/libgpu/primatt6.h"

void SetLineF2(void *packet) {
    GpuPrimitiveHeader *header = packet;
    header->tag.bytes.length = 3;
    header->color.bytes.code = 0x40;
}

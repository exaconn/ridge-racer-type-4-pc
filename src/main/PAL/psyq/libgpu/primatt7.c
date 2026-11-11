#include "types.h"
#include "rr/gpu.h"
#include "psyq/libgpu/primatt7.h"

void SetLineG2(void *packet) {
    GpuPrimitiveHeader *header = packet;
    header->tag.bytes.length = 4;
    header->color.bytes.code = 0x50;
}

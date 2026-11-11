#include "types.h"
#include "rr/gpu.h"
#include "psyq/libgpu/primatt5.h"

void SetTile(void *packet) {
    GpuPrimitiveHeader *header = packet;
    header->tag.bytes.length = 3;
    header->color.bytes.code = 0x60;
}

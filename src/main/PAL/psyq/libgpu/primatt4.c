#include "types.h"
#include "rr/gpu.h"

void SetSprt(void *packet) {
    GpuPrimitiveHeader *header = packet;
    header->tag.bytes.length = 4;
    header->color.bytes.code = 0x64;
}

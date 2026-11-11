#include "rr/gpu.h"
#include "rr/gpu_address.h"
#include "rr/animated_glyph.h"

void AddPrim(u32 *ordering, u32 *packet) {
    GpuDmaTag *head = (GpuDmaTag *)ordering;
    GpuDmaTag *tag = (GpuDmaTag *)packet;
    tag->bits.address = head->bits.address;
    head->bits.address = GpuDmaEncodeAddress(packet);
}

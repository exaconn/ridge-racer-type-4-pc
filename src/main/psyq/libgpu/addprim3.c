#include "psyq/libgpu/addprim3.h"
#include "rr/gpu_address.h"

void AddPrims(u32 *ordering, const void *first, u32 *last) {
    u32 lo = 0xFFFFFF;
    u32 hi = 0xFF000000;
    *last = (*last & hi) | (*ordering & lo);
    *ordering = (*ordering & hi) | (GpuDmaEncodeAddress(first) & lo);
}

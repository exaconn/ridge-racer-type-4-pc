#include "psyq/libgpu/addprim4.h"
#include "rr/gpu_address.h"

void CatPrim(u32 *tag, const void *next) {
    *tag = (*tag & 0xFF000000U) | (GpuDmaEncodeAddress(next) & 0xFFFFFFU);
}

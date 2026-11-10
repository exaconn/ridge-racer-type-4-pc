#include "rr/animated_glyph.h"
#include "rr/gpu_address.h"
void AddPrim(u32 *ot, u32 *p) {
    /* ASM_BARRIER: retained low-address-mask register binding. */
    register u32 lo asm("$6") = 0xFFFFFF;
    u32 hi = 0xFF000000;
    *p = (*p & hi) | (*ot & lo);
    *ot = (*ot & hi) | (GpuDmaEncodeAddress(p) & lo);
}

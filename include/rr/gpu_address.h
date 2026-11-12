#ifndef RR_GPU_ADDRESS_H
#define RR_GPU_ADDRESS_H

#include "types.h"

/* DMA links contain PSX CPU addresses, with their high byte replaced by the
 * packet length. Ordering heads clear that byte. These are wire addresses,
 * not native pointers. The first payload byte follows the four-byte tag. */
#if defined(mips)
static __inline__ u32 GpuDmaEncodeAddress(const void *pointer) {
    return (uintptr_t)pointer;
}
static __inline__ u8 GpuDmaReadPayloadByte(u32 address) {
    return ((const u8 *)(uintptr_t)address)[4];
}
#else
/* Native backends install the actual buffers and their PSX CPU addresses.
 * Descriptors and buffers remain alive until the next successful install.
 * Physical ranges must fit the 24-bit DMA address space. KSEG0/KSEG1 mirrors
 * resolve to the same physical range; the registered CPU alias is encoded. */
typedef struct {
    const void *native;
    u32 address, size;
} GpuDmaRegion;

/* Returns zero for an invalid/overlapping map and preserves the previous map.
 * An empty map resets it. Installation happens outside command processing. */
s32 GpuDmaSetRegions(const GpuDmaRegion *regions, u32 count);
/* A nonempty span must fit one region. Unmapped spans return NULL. */
const u8 *GpuDmaResolveAddress(u32 address, u32 size);
/* Encodes an accessible byte; on failure leaves the output word untouched. */
s32 GpuDmaAddressOf(const void *pointer, u32 *address);
/* Command-processing boundaries require a mapped pointer/header and abort
 * on an invalid address, instead of manufacturing a native pointer. */
u32 GpuDmaEncodeAddress(const void *pointer);
u8 GpuDmaReadPayloadByte(u32 address);
#endif

#endif

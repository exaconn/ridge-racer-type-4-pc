#include "rr/gpu_address.h"

#include <stdint.h>
#include <stdlib.h>

static const GpuDmaRegion *activeRegions;
static u32 activeRegionCount;

static u32 physicalAddress(u32 address) {
    u32 segment = address & 0xE0000000U;
    if (segment != 0 && segment != 0x80000000U && segment != 0xA0000000U) {
        return UINT32_MAX;
    }
    return address & 0x1FFFFFFFU;
}

s32 GpuDmaSetRegions(const GpuDmaRegion *regions, u32 count) {
    if (count && !regions) {
        return 0;
    }
    for (u32 i = 0; i < count; i++) {
        const GpuDmaRegion *region = &regions[i];
        uintptr_t native = (uintptr_t)region->native;
        u32 address = physicalAddress(region->address);
        if (!native || !region->size || address >= 0x01000000U ||
            region->size > 0x01000000U - address ||
            region->size > UINTPTR_MAX - native) {
            return 0;
        }
        for (u32 j = 0; j < i; j++) {
            const GpuDmaRegion *other = &regions[j];
            uintptr_t otherNative = (uintptr_t)other->native;
            u32 otherAddress = physicalAddress(other->address);
            if ((address < otherAddress + other->size &&
                 otherAddress < address + region->size) ||
                (native < otherNative + other->size &&
                 otherNative < native + region->size)) {
                return 0;
            }
        }
    }
    activeRegions = regions;
    activeRegionCount = count;
    return 1;
}

const u8 *GpuDmaResolveAddress(u32 address, u32 size) {
    if (!size) {
        return NULL;
    }
    address = physicalAddress(address);
    for (u32 i = 0; i < activeRegionCount; i++) {
        const GpuDmaRegion *region = &activeRegions[i];
        u32 base = physicalAddress(region->address);
        if (address >= base && address - base < region->size &&
            size <= region->size - (address - base)) {
            return (const u8 *)region->native + (address - base);
        }
    }
    return NULL;
}

s32 GpuDmaAddressOf(const void *pointer, u32 *address) {
    uintptr_t native = (uintptr_t)pointer;
    if (!address) {
        return 0;
    }
    for (u32 i = 0; i < activeRegionCount; i++) {
        const GpuDmaRegion *region = &activeRegions[i];
        uintptr_t base = (uintptr_t)region->native;
        if (native >= base && native - base < region->size) {
            *address = region->address + (u32)(native - base);
            return 1;
        }
    }
    return 0;
}

u32 GpuDmaEncodeAddress(const void *pointer) {
    u32 address;
    if (!GpuDmaAddressOf(pointer, &address)) {
        abort();
    }
    return address;
}

u8 GpuDmaReadPayloadByte(u32 address) {
    const u8 *packet = GpuDmaResolveAddress(address, 5);
    if (!packet) {
        abort();
    }
    return packet[4];
}

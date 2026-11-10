/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr_asm.h"
#include "rr/channels.h"
#include "rr/spu.h"
#include "psyq/libspu/sminit.h"
#include "rr/spu_allocation.h"
#include "psyq/libsnd/vmcfg.h"



s32 SsSetReservedVoice(u32 count) {
    s32 result;
    if (((count - 1) & 0xFFU) < 24) {
        spuVmMaxVoice = count;
        result = (s8)count;
    } else {
        result = -1;
    }
    return result;
}

void SsVabClose(s16 channel) {
    if ((u16)channel < 16) {
        s32 index = channel;
        if (_svm_vab_used[index] == 1) {
            SpuFree(_svm_vab_start[index]);
            _svm_vab_used[index] = 0;
            _svm_vab_count--;
        }
    }
}

void SpuFree(u32 address) {
    s32 visited = 0;
    s32 count = _spu_AllocBlockNum;
    if (count > 0) {
        u32 end_marker = 0x40000000U;
        u32 freed = address | 0x80000000U;
        s32 limit = count;
        SpuAllocation *entry;

        /* ASM_BARRIER: materialize loop invariants before loading the cursor. */
        __asm__ volatile("" : : "r"(end_marker), "r"(freed), "r"(limit));
        entry = _spu_memList;
        RR_NOP_DEP();
        do {
            /* ASM_BARRIER: reuse v1 for the current entry's address and flags. */
            register u32 flags __asm__("$3") = entry->address_flags;
            if (flags & end_marker) break;
            visited++;
            if (flags == address) {
                entry->address_flags = freed;
                break;
            }
            entry++;
        } while (visited < limit);
    }
    _spu_gcSPU();
}

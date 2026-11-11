/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
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
    if (_spu_AllocBlockNum > 0) {
        do {
            u32 flags = _spu_memList[visited].address_flags;
            if (flags & 0x40000000U) break;
            if (flags == address) {
                _spu_memList[visited].address_flags = address | 0x80000000U;
                break;
            }
            visited++;
        } while (visited < _spu_AllocBlockNum);
    }
    _spu_gcSPU();
}

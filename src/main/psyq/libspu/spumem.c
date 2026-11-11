#include "rr/spu_allocation.h"
#include "psyq/libsnd/ssvab.h"
#include "psyq/libspu/spumem.h"

/* -g retains lexical block labels used by stock MASPSX to place load delays.
 * Keep the pointer snapshot scope before the zero-size loop. */
void _spu_gcSPU(void) {
    s32 i, j;
    u32 t;

    for (i = 0; i <= _spu_AllocLastNum; ) {
        u32 free_bit = 0x80000000U;
        u32 deleted = 0x2FFFFFFFU;
        if (_spu_memList[i].address_flags & free_bit) {
            j = i + 1;
            {
                SpuAllocation *scan = (SpuAllocation *)((j * 8) + (unsigned long)_spu_memList);
                for (;;) {
                    u32 flags = scan->address_flags;
                    scan++;
                    if (flags != deleted) break;
                    j++;
                }
            }
            if (_spu_memList[j].address_flags & free_bit) {
                if ((_spu_memList[j].address_flags & 0x0FFFFFFFU) ==
                    (_spu_memList[i].address_flags & 0x0FFFFFFFU) + _spu_memList[i].size) {
                    _spu_memList[j].address_flags = deleted;
                    _spu_memList[i].size += _spu_memList[j].size;
                    continue;
                }
            }
        }
        i++;
    }
    for (i = 0; i <= _spu_AllocLastNum; i++) {
        if (_spu_memList[i].size == 0) {
            _spu_memList[i].address_flags = 0x2FFFFFFFU;
        }
    }
    for (i = 0; i <= _spu_AllocLastNum; i++) {
        if (_spu_memList[i].address_flags & 0x40000000U) {
            break;
        }
        for (j = i + 1; j <= _spu_AllocLastNum; j++) {
            if (_spu_memList[j].address_flags & 0x40000000U) {
                break;
            }
            if ((_spu_memList[j].address_flags & 0x0FFFFFFFU) <
                (_spu_memList[i].address_flags & 0x0FFFFFFFU)) {
                u32 addr = _spu_memList[i].address_flags;
                u32 size = _spu_memList[i].size;
                _spu_memList[i].address_flags = _spu_memList[j].address_flags;
                _spu_memList[i].size = _spu_memList[j].size;
                _spu_memList[j].address_flags = addr;
                _spu_memList[j].size = size;
            }
        }
    }
    for (i = 0; i <= _spu_AllocLastNum; i++) {
        if (_spu_memList[i].address_flags & 0x40000000U) {
            break;
        }
        if (_spu_memList[i].address_flags == 0x2FFFFFFFU) {
            _spu_memList[i].address_flags = _spu_memList[_spu_AllocLastNum].address_flags;
            _spu_memList[i].size = _spu_memList[_spu_AllocLastNum].size;
            _spu_AllocLastNum = i;
            break;
        }
    }
    for (i = _spu_AllocLastNum - 1; i >= 0; i--) {
        if (!(_spu_memList[i].address_flags & 0x80000000U)) {
            break;
        }
        _spu_memList[i].address_flags = (_spu_memList[i].address_flags & 0x0FFFFFFFU) | 0x40000000U;
        t = _spu_AllocLastNum;
        _spu_AllocLastNum = i;
        _spu_memList[i].size += _spu_memList[t].size;
    }
}
s32 SsVabOpenHeadSticky(u8 *data, s32 bank, s32 address) {
    return (s16)_SsVabOpenHeadWithMode(data, (s16)bank, VsGetAddr, address);
}

s32 SsVabFakeHead(u8 *data, s32 bank, s32 address) {
    return (s16)_SsVabOpenHeadWithMode(data, (s16)bank, VsGetAddr, address);
}

s32 VsGetAddr(u32 bytes, s32 address, s16 bank) {
    (void)bytes;
    (void)bank;
    return address;
}

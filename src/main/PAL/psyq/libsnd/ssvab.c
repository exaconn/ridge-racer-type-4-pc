/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "types.h"
#include "rr/spu.h"
#include "psyq/libsnd/vmmain.h"
#include "psyq/libsnd/ssvab.h"

s32 _SsVabOpenHeadWithMode(void *addr, s16 vabid, VabTransFunc func, s32 mode) {
    s16 vab_id = 0x10;
    u8 *p;
    VabHdr *vab_h;
    ProgAtr *prog_atr;
    s32 i;
    u32 tot_size;
    u16 *vag_off;
    u32 vag_size[256];
    u32 spu_alloc;
    u8 num_vags;

    if (_spu_getInTransfer() == 1) {
        return -1;
    }
    _spu_setInTransfer(1);
    if (vabid < 0x10) {
        if (vabid == -1) {
            i = 0;
        loop:
            if (_svm_vab_used[i] != 0) {
                i++;
                if (i < 0x10) {
                    goto loop;
                }
            } else {
                _svm_vab_used[i] = 1;
                vab_id = i;
                _svm_vab_count++;
            }
        } else {
            if (_svm_vab_used[vabid] == 0) {
                _svm_vab_used[vabid] = 1;
                vab_id = vabid;
                _svm_vab_count++;
            }
        }
    } else {
        _spu_setInTransfer(0);
        return -1;
    }
    if (vab_id >= 0x10) {
        _spu_setInTransfer(0);
        return -1;
    }
    p = addr;
    _svm_vab_vh[vab_id] = p;
    p += 0x20;
    vab_h = (VabHdr *)addr;
    if ((vab_h->form >> 8) != 0x564142) {
        _svm_vab_used[vab_id] = 0;
        _spu_setInTransfer(0);
        { s32 ret = -1; _svm_vab_count--; return ret; }
    }
    if ((vab_h->form & 0xFF) == 0x70) {
        if (vab_h->ver >= 5) {
            kMaxPrograms = 0x80;
        } else {
            kMaxPrograms = 0x40;
        }
    } else {
        kMaxPrograms = 0x40;
    }
    if (vab_h->ps > kMaxPrograms) {
        _svm_vab_used[vab_id] = 0;
        _spu_setInTransfer(0);
        { s32 ret = -1; _svm_vab_count--; return ret; }
    }
    _svm_vab_pg[vab_id] = (ProgAtrBuffer *)p;
    prog_atr = (ProgAtr *)p;
    p += kMaxPrograms * sizeof(ProgAtr);
    i = 0;
tot_size = 0;
    if ((s32)tot_size < kMaxPrograms) {
        s32 limit = kMaxPrograms;
        ProgAtr *cur = prog_atr;
        do {
            cur->toneBlockIndex = tot_size;
            if (cur->attributes.tones != 0) {
                tot_size++;
            }
            i++;
            cur++;
        } while (i < limit);
    }
    tot_size = 0;
_svm_vab_tn[vab_id] = (VagAtrBuffer *)p;
    p += vab_h->ps << 9;
    vag_off = (u16 *)p;
    num_vags = vab_h->vs;
    for (i = 0; i < 256; i++) {
        if (num_vags >= i) {
            u32 v = *vag_off;
            if (vab_h->ver >= 5) {
                vag_size[i] = v << 3;
            } else {
                vag_size[i] = v << 2;
            }
            tot_size += vag_size[i];
        }
        vag_off++;
    }
    spu_alloc = (*func)(tot_size, mode, vab_id);
    if (spu_alloc == 0xFFFFFFFFU) {
        return -1;
    }
    if ((spu_alloc + tot_size) > 0x80000U) {
        _svm_vab_used[vab_id] = 0;
        _spu_setInTransfer(0);
        { s32 ret = -1; _svm_vab_count--; return ret; }
    }
    _svm_vab_start[vab_id] = spu_alloc;
    tot_size = 0;
{
        s32 nv = num_vags;
        s32 zero = 0;
        s32 cond = (nv < zero);
i = 0;
        if (!cond) {
            s32 limit = nv;
            do {
                tot_size += vag_size[i];
                if (!(i & 1)) {
                    prog_atr[i / 2].sampleAddresses[0] = (spu_alloc + tot_size) >> 3;
                } else {
                    prog_atr[i / 2].sampleAddresses[1] = (spu_alloc + tot_size) >> 3;
                }
                i++;
            } while (i <= limit);
        }
    }
    _svm_vab_total[vab_id] = tot_size;
    _svm_vab_used[vab_id] = 2;
    return vab_id;
}

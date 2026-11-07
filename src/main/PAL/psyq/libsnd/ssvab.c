/* MASPSX_FLAGS: --assembler-profile=gnu */
/* CC1_FLAGS: -mno-split-addresses */
#include "types.h"
#include "psyq/libsnd/vmmain.h"

typedef struct {
    u32 form;
    s32 ver;
    s32 id;
    u32 fsize;
    u16 reserved0;
    u16 ps;
    u16 ts;
    u16 vs;
    u8 mvol;
    u8 pan;
    u8 attr1;
    u8 attr2;
    u32 reserved1;
} VabHdr;

typedef struct {
    u8 tones;
    u8 mvol;
    u8 prior;
    u8 mode;
    u8 mpan;
    s8 reserved0;
    s16 attr;
    u32 reserved1;
    u16 fake_prog_idx;
    u16 reserved2;
} ProgAtr;

extern void _spu_setInTransfer(s32);

extern ProgAtr *D_80110020[];
extern u8 *D_80110060[];
extern u8 *D_801100A8[];
extern s16 kMaxPrograms;
extern u8 _svm_vab_used[];
extern u32 D_80110138[];
extern u16 _svm_vab_count;
extern u32 D_80110180[];

static inline void vab_count_dec(void) {
    /* ASM_BARRIER: keep the decremented count in v1 as in the target */
    register u16 c __asm__("$3");
    c = _svm_vab_count;
    _svm_vab_count = c - 1;
}

typedef s32 (*VabTransFunc)(u32, s32, s16);

s16 _SsVabOpenHeadWithMode(u8 *addr, s16 vabid, VabTransFunc func, s32 mode) {
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
    D_80110060[vab_id] = p;
    p += 0x20;
    vab_h = (VabHdr *)addr;
    if ((vab_h->form >> 8) != 0x564142) {
        _svm_vab_used[vab_id] = 0;
        _spu_setInTransfer(0);
        vab_count_dec();
        goto err;
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
        vab_count_dec();
        goto err;
    }
    D_80110020[vab_id] = (ProgAtr *)p;
    prog_atr = (ProgAtr *)p;
    p += kMaxPrograms * sizeof(ProgAtr);
    i = 0;
    /* ASM_BARRIER: hide zero in i from reload_cse so tot_size is initialized from $zero */
    __asm__("" : "+r"(i));
    tot_size = 0;
    if ((s32)tot_size < kMaxPrograms) {
        s32 limit = kMaxPrograms;
        /* ASM_BARRIER: keep the loop pointer in v1 as in the target */
        register ProgAtr *cur __asm__("$3") = prog_atr;
        do {
            cur->reserved1 = tot_size;
            if (cur->tones != 0) {
                tot_size++;
            }
            i++;
            cur++;
        } while (i < limit);
    }
    tot_size = 0;
    /* ASM_BARRIER: hide the zero from reload_cse so later zero inits stay separate (target has no reg copy of 0) */
    __asm__("" : "+r"(tot_size));
    D_801100A8[vab_id] = p;
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
    if (spu_alloc == -1) {
    err:
        return -1;
    }
    if ((spu_alloc + tot_size) > 0x80000U) {
        _svm_vab_used[vab_id] = 0;
        _spu_setInTransfer(0);
        vab_count_dec();
        goto err;
    }
    D_80110180[vab_id] = spu_alloc;
    tot_size = 0;
    /* ASM_BARRIER: hide zero in tot_size from reload_cse */
    __asm__("" : "+r"(tot_size));
    {
        /* ASM_BARRIER: keep zero-extended num_vags in v1 before copying to loop limit */
        register s32 nv __asm__("$3") = num_vags;
        s32 zero = 0;
        s32 cond = (nv < zero);
        /* ASM_BARRIER: schedule slti before i = 0 so reload_cse keeps slti and reorg fills delay slot with i = 0 */
        __asm__ volatile("" : : "r"(cond));
        i = 0;
        if (!cond) {
            s32 limit = nv;
            do {
                tot_size += vag_size[i];
                if (!(i & 1)) {
                    prog_atr[i / 2].fake_prog_idx = (spu_alloc + tot_size) >> 3;
                } else {
                    prog_atr[i / 2].reserved2 = (spu_alloc + tot_size) >> 3;
                }
                i++;
            } while (i <= limit);
        }
    }
    D_80110138[vab_id] = tot_size;
    _svm_vab_used[vab_id] = 2;
    return vab_id;
}

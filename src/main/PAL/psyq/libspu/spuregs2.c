/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/spu.h"
#include "rr/spu_key.h"
#include "psyq/libspu/spuregs2.h"
#include "psyq/libspu/sminit.h"
#include "rr/library.h"
#include "rr/spu_allocation.h"
#include "rr/spu_init.h"
extern int _spu_FsetRXX();
extern unsigned short D_800A48A2;
void _SpuInit(a0) int a0; {
    unsigned short *p;
    int i; unsigned c; RevAttr *q;
    ResetCallback();
    _spu_init(a0);
    c = 0xC000;
    if (a0 == 0) {
        i = 0x17;
        p = &D_800A48A2;
        do {
            *p = c;
            i--;
            p--;
        } while (i >= 0);
    }
    SpuStart();
    _spu_rev_flag = 0;
    _spu_rev_reserve_wa = 0;
    q = &_spu_rev_attr;
    q->mode = 0;
    q->depth_left = 0;
    q->depth_right = 0;
    q->delay = 0;
    q->feedback = 0;
    _spu_rev_offsetaddr = _spu_rev_startaddr[0];
    _spu_FsetRXX(0xD1, _spu_rev_startaddr[0], 0);
    _spu_AllocBlockNum = 0;
    _spu_AllocLastNum = 0;
    _spu_memList = 0;
    _spu_trans_mode = 0;
    _spu_transMode = 0;
    _spu_keystat = 0;
    _spu_RQmask = 0;
    _spu_RQvoice = 0;
    _spu_env = 0;
}

#include "rr/library.h"
#include "rr/channel_stream.h"
void _SsSndNextSep(s32 a0, s16 a1) {
    u32 bankBits = (u32)a0 << 16;
    LibStateB0 **table = _ss_score;
    LibStateB0 **q = &table[(s32)bankBits >> 16];
    LibStateB0 *p = &(*q)[(s16)a1];
    p->b20 = 1;
    p->b21 = 0;
    (*q)[(s16)a1].w98 &= ~0x100;
    (*q)[(s16)a1].w98 &= ~8;
    (*q)[(s16)a1].w98 &= ~2;
    (*q)[(s16)a1].w98 &= ~4;
    (*q)[(s16)a1].w98 &= ~0x200;
    p->b14 = 1;
    p->c0 = p->c4;
    (*q)[(s16)a1].w98 |= 1;
}

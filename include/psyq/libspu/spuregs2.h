#ifndef RR_PSYQ_LIBSPU_SPUREGS2_H
#define RR_PSYQ_LIBSPU_SPUREGS2_H

#include "types.h"

typedef struct {
    s32 mode;
    s16 depth_left;
    s16 depth_right;
    s32 delay;
    s32 feedback;
} RevAttr;

extern s32 _spu_trans_mode;
extern int _spu_rev_flag;
extern int _spu_rev_reserve_wa;
extern RevAttr _spu_rev_attr;
extern s32 _spu_rev_offsetaddr;
extern u32 _spu_rev_startaddr[];
void _SpuInit();

#endif /* RR_PSYQ_LIBSPU_SPUREGS2_H */

#ifndef RR_SPU_ALLOCATION_H
#define RR_SPU_ALLOCATION_H

#include "rr/spu.h"

/* Inclusive last record index. A nonempty table ends with a nonzero-sized
 * bit-30 record without bit 31; interior records are allocated, free (bit 31),
 * or deleted (0x2FFFFFFF). Indices and scaled offsets must fit signed32.
 * A negative index other than INT_MIN is an empty table. Addresses stored in
 * records are numeric SPU offsets; the table pointer retains native width. */
extern s32 D_800A48BC;
void func_8008A774(void);

#endif

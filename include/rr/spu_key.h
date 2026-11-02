#ifndef RR_SPU_KEY_H
#define RR_SPU_KEY_H
#include "rr/spu_init.h"
extern u32 D_800A48A4,D_800A4870,D_800A486C,D_800A4844;
/* Mode1 keys voices on,mode0 keys them off; other full32 modes do nothing.
 * Only the low24 mask bits participate. Queued commands use the first four
 * halfwords of the existing ten-halfword cache, preserving its other cells. */
void func_80087814(s32 mode,u32 mask);
#endif

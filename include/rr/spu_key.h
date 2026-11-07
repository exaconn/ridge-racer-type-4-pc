#ifndef RR_SPU_KEY_H
#define RR_SPU_KEY_H
#include "rr/spu_init.h"
extern u32 _spu_env,_spu_RQmask,_spu_RQvoice,_spu_keystat;
/* Mode1 keys voices on,mode0 keys them off; other full32 modes do nothing.
 * Only the low24 mask bits participate. Queued commands use the first four
 * halfwords of the existing ten-halfword cache, preserving its other cells. */
void SpuSetKey(s32 mode,u32 mask);
#endif

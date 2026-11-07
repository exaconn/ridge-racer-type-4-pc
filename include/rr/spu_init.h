#ifndef RR_SPU_INIT_H
#define RR_SPU_INIT_H

#include "rr/spu.h"

/* DMA priority register and the software state reset by SPU initialization. */
extern volatile u32 *D_800A47E8;
extern s32 D_800A47F8;
extern volatile u32 D_800A4814;
extern const u16 D_800A4818[8];
extern u16 D_8010EFB0[10];
extern const char D_80011698[], D_800116A8[];
s32 func_8001E84C(const char *format, ...);
s32 _spu_init(s32 warm);

#endif

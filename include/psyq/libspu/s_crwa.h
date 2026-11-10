#ifndef RR_PSYQ_LIBSPU_S_CRWA_H
#define RR_PSYQ_LIBSPU_S_CRWA_H

#include "types.h"

extern s32 _spu_EVdma;
void SpuInit(void);
s32 SpuClearReverbWorkArea(u32 preset);

#endif /* RR_PSYQ_LIBSPU_S_CRWA_H */

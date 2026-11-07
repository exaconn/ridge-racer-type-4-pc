/* GCC_VERSION: 970318 */

/* MASPSX_FLAGS: --assembler-profile=gnu */

#include "types.h"

#define COPY8(dst_arr, src_arr) \
    { \
        u8 *d = (dst_arr); \
        u8 *s = (src_arr); \
        s32 n; \
        if (d != 0) { \
            for (n = 7; n != -1; n--) { \
                *d++ = *s++; \
            } \
        } \
    }

extern int DMACallback();

void CdDataCallback(a0) { DMACallback(3, a0); }

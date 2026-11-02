/* GCC_VERSION: 2.7.2 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "types.h"

s32 func_80090604(s32 arg0) {
    s32 x[8];
    s32 y[8];
    s32 i;
    s32 k;
    s32 *q;
    s32 *p;
    s32 a;

    x[1] = arg0 + 0x5D50AD;
    y[1] = arg0 - 0x5D50AD;
    for (i = 1, k = 4, q = &y[1], p = &y[2]; i < 7; p++, i++) {
        if (i != k) {
            if (y[i] >= 0) {
                x[i + 1] = x[i] - (y[i] >> i);
                *p = y[i] - (x[i] >> i);
            } else {
                x[i + 1] = x[i] + (y[i] >> i);
                *(s32 *)((u8 *)q + (i << 2)) = y[i] + (x[i] >> i);
            }
        } else {
            if (y[4] >= 0) {
                a = x[4];
                x[4] = a - (y[4] >> 4);
                y[4] = y[4] - (a >> 4);
                if (y[4] >= 0) {
                    x[5] = x[4] - (y[4] >> 4);
                    y[5] = y[4] - (x[4] >> 4);
                } else {
                    x[5] = x[4] + (y[4] >> 4);
                    y[5] = y[4] + (x[4] >> 4);
                }
            } else {
                a = x[4];
                x[4] = a + (y[4] >> 4);
                y[4] = y[4] + (a >> 4);
                if (y[4] >= 0) {
                    x[5] = x[4] - (y[4] >> 4);
                    y[5] = y[4] - (x[4] >> 4);
                } else {
                    x[5] = x[4] + (y[4] >> 4);
                    y[5] = y[4] + (x[4] >> 4);
                }
            }
        }
    }
    return x[7];
}






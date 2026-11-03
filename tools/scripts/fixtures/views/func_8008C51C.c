/* CC1_FLAGS: -mno-split-addresses */

#include "rr/cd.h"

/* legacy typing of shared globals in this part of the unit */
#define func_8008DCB4 ((int (*)())func_8008DCB4)

#undef func_8008DCB4

s32 func_8008C51C(u8 *buffer, u32 words) {
    return func_8008DBB4(buffer, words) == 0;
}

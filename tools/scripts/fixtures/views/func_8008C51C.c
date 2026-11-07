/* CC1_FLAGS: -mno-split-addresses */

#include "rr/cd.h"

/* legacy typing of shared globals in this part of the unit */
#define CD_getsector2 ((int (*)())CD_getsector2)

#undef CD_getsector2

s32 CdGetSector(u8 *buffer, u32 words) {
    return CD_getsector(buffer, words) == 0;
}

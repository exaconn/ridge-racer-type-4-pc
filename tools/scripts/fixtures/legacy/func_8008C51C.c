#include "rr/cd.h"
s32 CdGetSector(u8 *buffer, u32 words) {
    return CD_getsector(buffer, words) == 0;
}

#include "rr/cd.h"
s32 func_8008C51C(u8 *buffer, u32 words) {
    return CD_getsector(buffer, words) == 0;
}

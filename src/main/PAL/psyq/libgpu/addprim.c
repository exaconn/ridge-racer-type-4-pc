#include "types.h"
#include "m2c_macros.h"

s32 GetClut(s32 arg0, s32 arg1) {
    return ((arg1 << 6) | ((arg0 >> 4) & 0x3F)) & 0xFFFF;
}

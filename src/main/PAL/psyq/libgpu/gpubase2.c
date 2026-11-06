#include "types.h"
#include "m2c_macros.h"

s32 GetTPage(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    return ((arg0 & 3) << 7) | ((arg1 & 3) << 5) | ((s32) (arg3 & 0x100) >> 4) | ((s32) (arg2 & 0x3FF) >> 6) | ((arg3 & 0x200) * 4);
}

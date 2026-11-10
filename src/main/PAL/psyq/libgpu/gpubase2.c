#include "types.h"
#include "psyq/libgpu/gpubase2.h"

u16 GetTPage(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    return ((arg0 & 3) << 7) | ((arg1 & 3) << 5) | ((arg3 & 0x100) >> 4) | ((arg2 & 0x3FF) >> 6) | ((arg3 & 0x200) * 4);
}

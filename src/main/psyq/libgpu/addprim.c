#include "types.h"
#include "psyq/libgpu/addprim.h"

u16 GetClut(s32 arg0, s32 arg1) {
    return (arg1 << 6) | ((arg0 >> 4) & 0x3F);
}

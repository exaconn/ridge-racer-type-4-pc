#include "types.h"
#include "m2c_macros.h"

s32 func_800430BC(s32 arg0, s32 arg1) {
    return (((arg1 - 0x4B) * 0x10) | ((arg0 - 0x41) & 0xF)) & 0xFF;
}

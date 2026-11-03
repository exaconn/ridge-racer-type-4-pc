/* GCC_VERSION: 2.7.2 */
#include "rr/spu.h"

void func_80086B84(u32 voice, u16 *left, u16 *right) {
    RrSpuVoiceRegisters *registers = (RrSpuVoiceRegisters *)(voice * 16 + (unsigned long)D_800A47D8);
    u16 raw_left = registers->left;
    u16 raw_right = registers->right;
    u32 value = raw_left & 0xFFFF;
    u32 half_range;
    u32 right_value;
    if (value >= 0x4000U) {
        half_range = 0x8000;
        *left = value - half_range;
    } else {
        *left = raw_left;
    }
    right_value = raw_right & 0xFFFF;
    if (right_value >= 0x4000U) {
        half_range = 0x8000;
        *right = right_value - half_range;
        return;
    }
    *right = raw_right;
}

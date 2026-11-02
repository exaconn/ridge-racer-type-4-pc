#include "types.h"
#include "m2c_macros.h"

void func_80038550(s32 arg0) {
    void *temp_v0;

    temp_v0 = (arg0 * 0x24) + 0x1F800080;
    M2C_FIELD(temp_v0, s16 *, 0x54) = (s16) ((s32) (M2C_FIELD(temp_v0, s16 *, 0x44) + M2C_FIELD(temp_v0, s16 *, 0x48)) >> 1);
    M2C_FIELD(temp_v0, s16 *, 0x56) = (s16) ((s32) (M2C_FIELD(temp_v0, s16 *, 0x46) + M2C_FIELD(temp_v0, s16 *, 0x4A)) >> 1);
    M2C_FIELD(temp_v0, s16 *, 0x58) = (s16) ((s32) (M2C_FIELD(temp_v0, s16 *, 0x44) + M2C_FIELD(temp_v0, s16 *, 0x4C)) >> 1);
    M2C_FIELD(temp_v0, s16 *, 0x5A) = (s16) ((s32) (M2C_FIELD(temp_v0, s16 *, 0x46) + M2C_FIELD(temp_v0, s16 *, 0x4E)) >> 1);
    M2C_FIELD(temp_v0, s16 *, 0x5C) = (s16) ((s32) (M2C_FIELD(temp_v0, s16 *, 0x48) + M2C_FIELD(temp_v0, s16 *, 0x50)) >> 1);
    M2C_FIELD(temp_v0, s16 *, 0x5E) = (s16) ((s32) (M2C_FIELD(temp_v0, s16 *, 0x4A) + M2C_FIELD(temp_v0, s16 *, 0x52)) >> 1);
    M2C_FIELD(temp_v0, s16 *, 0x60) = (s16) ((s32) (M2C_FIELD(temp_v0, s16 *, 0x4C) + M2C_FIELD(temp_v0, s16 *, 0x50)) >> 1);
    M2C_FIELD(temp_v0, s16 *, 0x62) = (s16) ((s32) (M2C_FIELD(temp_v0, s16 *, 0x4E) + M2C_FIELD(temp_v0, s16 *, 0x52)) >> 1);
    M2C_FIELD(temp_v0, s16 *, 0x40) = (s16) ((s32) (M2C_FIELD(temp_v0, s16 *, 0x44) + M2C_FIELD(temp_v0, s16 *, 0x48) + M2C_FIELD(temp_v0, s16 *, 0x4C) + M2C_FIELD(temp_v0, s16 *, 0x50)) >> 2);
    M2C_FIELD(temp_v0, s16 *, 0x42) = (s16) ((s32) (M2C_FIELD(temp_v0, s16 *, 0x46) + M2C_FIELD(temp_v0, s16 *, 0x4A) + M2C_FIELD(temp_v0, s16 *, 0x4E) + M2C_FIELD(temp_v0, s16 *, 0x52)) >> 2);
}

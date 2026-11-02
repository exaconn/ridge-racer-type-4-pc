#include "types.h"
#include "m2c_macros.h"

extern s32 D_800ACC14;

void func_8004CFB0(void *arg0, void *arg1, void *arg2, s32 arg3) {
    void *temp_a3;
    void *temp_t0;

    temp_t0 = *(void **)0x1F800000;
    M2C_FIELD(temp_t0, s8 *, 3) = 8;
    M2C_FIELD(temp_t0, s8 *, 7) = 0x3A;
    M2C_FIELD(temp_t0, u16 *, 8) = (u16) M2C_FIELD(arg0, u16 *, 0);
    M2C_FIELD(temp_t0, u16 *, 0xA) = (u16) M2C_FIELD(arg0, u16 *, 2);
    M2C_FIELD(temp_t0, s16 *, 0x10) = (s16) (M2C_FIELD(arg0, u16 *, 0) + M2C_FIELD(arg0, u16 *, 4));
    M2C_FIELD(temp_t0, u16 *, 0x12) = (u16) M2C_FIELD(arg0, u16 *, 2);
    M2C_FIELD(temp_t0, u16 *, 0x18) = (u16) M2C_FIELD(arg0, u16 *, 0);
    M2C_FIELD(temp_t0, s16 *, 0x1A) = (s16) (M2C_FIELD(arg0, u16 *, 2) + M2C_FIELD(arg0, u16 *, 6));
    M2C_FIELD(temp_t0, s16 *, 0x20) = (s16) (M2C_FIELD(arg0, u16 *, 0) + M2C_FIELD(arg0, u16 *, 4));
    M2C_FIELD(temp_t0, s16 *, 0x22) = (s16) (M2C_FIELD(arg0, u16 *, 2) + M2C_FIELD(arg0, u16 *, 6));
    M2C_FIELD(temp_t0, u8 *, 4) = (u8) M2C_FIELD(arg1, u8 *, 0);
    M2C_FIELD(temp_t0, u8 *, 5) = (u8) M2C_FIELD(arg1, u8 *, 1);
    M2C_FIELD(temp_t0, u8 *, 6) = (u8) M2C_FIELD(arg1, u8 *, 2);
    M2C_FIELD(temp_t0, u8 *, 0xC) = (u8) M2C_FIELD(arg1, u8 *, 0);
    M2C_FIELD(temp_t0, u8 *, 0xD) = (u8) M2C_FIELD(arg1, u8 *, 1);
    M2C_FIELD(temp_t0, u8 *, 0xE) = (u8) M2C_FIELD(arg1, u8 *, 2);
    M2C_FIELD(temp_t0, u8 *, 0x14) = (u8) M2C_FIELD(arg2, u8 *, 0);
    M2C_FIELD(temp_t0, u8 *, 0x15) = (u8) M2C_FIELD(arg2, u8 *, 1);
    M2C_FIELD(temp_t0, u8 *, 0x16) = (u8) M2C_FIELD(arg2, u8 *, 2);
    M2C_FIELD(temp_t0, u8 *, 0x1C) = (u8) M2C_FIELD(arg2, u8 *, 0);
    M2C_FIELD(temp_t0, u8 *, 0x1D) = (u8) M2C_FIELD(arg2, u8 *, 1);
    M2C_FIELD(temp_t0, u8 *, 0x1E) = (u8) M2C_FIELD(arg2, u8 *, 2);
    temp_a3 = (arg3 * 4) + D_800ACC14;
    M2C_FIELD(temp_t0, s32 *, 0) = (s32) ((M2C_FIELD(temp_t0, s32 *, 0) & 0xFF000000) | (M2C_FIELD(temp_a3, s32 *, 0x70) & 0xFFFFFF));
    M2C_FIELD(temp_a3, s32 *, 0x70) = (s32) ((M2C_FIELD(temp_a3, s32 *, 0x70) & 0xFF000000) | ((s32) temp_t0 & 0xFFFFFF));
    *(void **)0x1F800000 = temp_t0 + 0x24;
}

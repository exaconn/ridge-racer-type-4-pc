#include "types.h"
#include "rr/hud_number.h"
#include "m2c_macros.h"

void *func_80021568(u32 *arg0, void *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
void *func_800215F4(s32 *arg0, void *arg1, s16 arg2, s16 arg3, s32 arg4, s32 arg5);
void func_80021684(int a0, int a1);

void *func_80021568(u32 *arg0, void *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    M2C_FIELD(arg1, s8 *, 3) = 4;
    M2C_FIELD(arg1, s8 *, 7) = 0x65;
    M2C_FIELD(arg1, s16 *, 0x10) = 0xC;
    M2C_FIELD(arg1, s16 *, 0x12) = 0x10;
    M2C_FIELD(arg1, s16 *, 8) = arg2;
    M2C_FIELD(arg1, s16 *, 0xA) = arg3;
    M2C_FIELD(arg1, u8 *, 0xD) = 0xF0;
    M2C_FIELD(arg1, s8 *, 0xC) = (s8) (arg4 * 0xC);
    M2C_FIELD(arg1, s16 *, 0xE) = (s16) arg5;
    M2C_FIELD(arg1, s32 *, 0) = (s32) ((M2C_FIELD(arg1, s32 *, 0) & 0xFF000000) | (*arg0 & 0xFFFFFF));
    *arg0 = (*arg0 & 0xFF000000) | ((s32) arg1 & 0xFFFFFF);
    return arg1 + 0x14;
}

void *func_800215F4(s32 *arg0, void *arg1, s16 arg2, s16 arg3, s32 arg4, s32 arg5) {
    M2C_FIELD(arg1, s8 *, 3) = 4;
    M2C_FIELD(arg1, s8 *, 7) = 0x65;
    M2C_FIELD(arg1, s16 *, 0x10) = 0xC;
    M2C_FIELD(arg1, s16 *, 0x12) = 0x10;
    M2C_FIELD(arg1, s16 *, 8) = arg2;
    M2C_FIELD(arg1, s16 *, 0xA) = arg3;
    M2C_FIELD(arg1, u8 *, 0xD) = 0xF0;
    M2C_FIELD(arg1, s8 *, 0xC) = (s8) ((arg4 * 0xC) + 0x78);
    M2C_FIELD(arg1, s16 *, 0xE) = (s16) arg5;
    M2C_FIELD(arg1, s32 *, 0) = (s32) ((M2C_FIELD(arg1, s32 *, 0) & 0xFF000000) | (*arg0 & 0xFFFFFF));
    *arg0 = (*arg0 & 0xFF000000) | ((s32) arg1 & 0xFFFFFF);
    return arg1 + 0x14;
}

/* legacy typing of shared globals in this part of the unit */
#define D_8010BF88 (*(char (*)[])&D_8010BF88)

void func_80021684(int a0, int a1)
{
    char *b = D_8010BF88;
    int off = a1 * 0x14;
    char *t = b + off;
    short x = *(short *)(t + 8);
    short y = *(short *)(t + 10);
    int s1 = *(int *)(D_800F4BF0 * 4 + off + (int)b);
    int h, tn, on;
    h = a0 / 100;
    tn = (a0 / 10) - h * 10;
    on = a0 % 10;
    *(int *)0x1F800000 = func_8001FCC8(s1, func_80021568(s1, func_80021568(s1, func_80021568(s1, *(int *)0x1F800000, x, y, h, 0x7EFA), x + 8, y, tn, 0x7EFA), x + 0x10, y, on, 0x7EFA), 5);
}

#undef D_8010BF88

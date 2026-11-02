#include "types.h"
#include "m2c_macros.h"
extern s32 func_8001FCC8();
extern s32 func_800215F4();
extern s32 D_800F4BF0;
extern char D_8010BF88[];

void func_800219BC(s32 arg0, s32 arg1) {
    s32 temp_s0;
    s32 temp_v1;
    char *temp_a3;
    char *b;
    s32 cur, m, r;
    s32 a = arg0;
    b = D_8010BF88;
    temp_v1 = arg1 * 0x14;
    temp_a3 = b + temp_v1;
    cur = *(s32 *)0x1F800000;
    temp_s0 = *(s32 *)(D_800F4BF0 * 4 + temp_v1 + b);
    m = a % 10;
    r = func_800215F4(temp_s0, cur, M2C_FIELD(temp_a3, s16 *, 0xC), M2C_FIELD(temp_a3, s16 *, 0xE), m, 0x7EFB);
    *(s32 *)0x1F800000 = func_8001FCC8(temp_s0, r, 5);
}

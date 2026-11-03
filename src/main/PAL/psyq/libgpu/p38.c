/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "types.h"
#include "m2c_macros.h"

extern void *D_800ABAB8;
extern int func_8009424C();
extern int func_80094100();
extern int func_80094198();
extern int func_80094230();

u32 func_80093AAC(void);
void func_80093ADC(void *arg0, M2C_UNK arg1);
void func_80093B14(void *arg0, void *arg1);
void func_80093B94(void *arg0, void *arg1);
void func_80093BD4(void *arg0, s32 arg1, s32 arg2);
void func_80093BFC(char *p, int a);

u32 func_80093AAC(void) {
    return M2C_FIELD(D_800ABAB8, u32 (**)(), 0x38)() >> 0x1F;
}

void func_80093ADC(void *arg0, M2C_UNK arg1) {
    M2C_FIELD(arg0, s8 *, 3) = 2;
    M2C_FIELD(arg0, s32 *, 4) = func_8009424C(arg1);
    M2C_FIELD(arg0, s32 *, 8) = 0;
}

void func_80093B14(void *arg0, void *arg1) {
    M2C_FIELD(arg0, s8 *, 3) = 2;
    M2C_FIELD(arg0, s32 *, 4) = func_80094100(M2C_FIELD(arg1, s16 *, 0), M2C_FIELD(arg1, s16 *, 2));
    M2C_FIELD(arg0, s32 *, 8) = func_80094198((s16) (((u16) M2C_FIELD(arg1, s16 *, 0) + M2C_FIELD(arg1, u16 *, 4)) - 1), (s16) (((u16) M2C_FIELD(arg1, s16 *, 2) + M2C_FIELD(arg1, u16 *, 6)) - 1));
}

void func_80093B94(void *arg0, void *arg1) {
    M2C_FIELD(arg0, s8 *, 3) = 2;
    M2C_FIELD(arg0, s32 *, 4) = func_80094230(M2C_FIELD(arg1, s16 *, 0), M2C_FIELD(arg1, s16 *, 2));
    M2C_FIELD(arg0, s32 *, 8) = 0;
}

void func_80093BD4(void *arg0, s32 arg1, s32 arg2) {
    s32 var_v1;

    M2C_FIELD(arg0, s8 *, 3) = 2;
    var_v1 = 0xE6000000;
    if (arg1 != 0) {
        var_v1 = 0xE6000002;
    }
    M2C_FIELD(arg0, s32 *, 4) = (s32) (var_v1 | (arg2 != 0));
    M2C_FIELD(arg0, s32 *, 8) = 0;
}

void func_80093BFC(char *p, int a) { p[3] = 2; ((int *)p)[1] = a ? 0xE6000001 : 0xE6000000; ((int *)p)[2] = 0; }

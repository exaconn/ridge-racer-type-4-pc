#include "types.h"
#include "m2c_macros.h"

extern int func_8003A198();
extern int func_8005C13C();
extern int func_8005C988();
extern int func_80092CB8();
extern s32 D_800ACBF8;
extern s16 D_800F3A26;
extern s16 D_800F4BE8;
extern s16 D_800F4C6A;
extern s16 D_800F4D8A;

void func_8005DD8C(void) {
    func_80092CB8(0);
    func_8003A198();
    D_800F4BE8 = 8;
    D_800F4C6A = 1;
    D_800F3A26 = 0x23;
    D_800F4D8A = 0;
    func_8005C13C(0);
    func_8005C988(1);
    D_800ACBF8 = 0;
}

#include "types.h"
#include "m2c_macros.h"

extern int func_8004E008();
extern int func_8005C13C();
extern int func_8005C988();
extern int func_80092CB8();
extern int func_80096434();
extern s32 D_800102F0;
extern M2C_UNK D_800ACA94;
extern s32 D_800ACC18;
extern s16 D_800F3A26;
extern s16 D_800F4C6A;
extern s16 D_800F4D8A;

void func_8005D2A0(void) {
    func_80092CB8(0);
    func_80096434(&D_800ACA94, 0x78);
    func_8005C13C(0);
    func_8005C988(0);
    D_800ACC18 = D_800102F0;
    func_8004E008();
    D_800F4C6A = 2;
    D_800F3A26 = 6;
    D_800F4D8A = 0;
}

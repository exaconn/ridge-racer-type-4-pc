#include "types.h"
#include "m2c_macros.h"

extern int func_8001E378();
extern int func_8002A344();
extern s32 D_800F2DE4;
extern s16 D_800F3A26;
extern s16 D_800F4C6A;
extern s16 D_800F4D8A;

void func_8003984C(void) {
    D_800F2DE4 = 0;
    func_8001E378();
    func_8002A344(0x1000C);
    D_800F4C6A = 0;
    D_800F3A26 = 6;
    D_800F4D8A = 0;
}

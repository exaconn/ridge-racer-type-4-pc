#include "types.h"
#include "m2c_macros.h"

extern int func_80096434();
extern M2C_UNK D_800ACC68;
extern M2C_UNK D_800F4CB0;

void func_8002A604(void) {
    func_80096434(&D_800F4CB0, 0x20);
    func_80096434(&D_800ACC68, 0xC);
}

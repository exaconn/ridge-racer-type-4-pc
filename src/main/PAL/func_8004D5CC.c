#include "types.h"
#include "m2c_macros.h"

extern int func_8004D4DC();
extern int func_8004D58C();
extern int func_8004D598();
extern int func_8004D5A4();

void func_8004D5CC(void *arg0) {
    func_8004D4DC(arg0 + M2C_FIELD(arg0, s32 *, 4));
    func_8004D58C(arg0 + M2C_FIELD(arg0, s32 *, 8));
    func_8004D598(arg0 + M2C_FIELD(arg0, s32 *, 0xC));
    func_8004D5A4(arg0 + M2C_FIELD(arg0, s32 *, 0x10));
}

#include "types.h"
#include "m2c_macros.h"

extern int func_80022888();
extern int func_800244B0();
extern int func_8002648C();
extern int func_80026F5C();
extern int func_8002C638();

void func_80022C90(s32 arg0) {
    func_8002C638();
    func_800244B0(arg0);
    func_8002648C(arg0);
    func_80022888(arg0);
    func_80026F5C(arg0);
}

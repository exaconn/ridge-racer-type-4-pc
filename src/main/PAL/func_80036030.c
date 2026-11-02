#include "types.h"
#include "m2c_macros.h"

extern int func_8004B864();
extern int func_8004BB54();

void func_80036030(void) {
    func_8004BB54(0);
    func_8004BB54(1);
    func_8004B864(0);
    func_8004B864(1);
}

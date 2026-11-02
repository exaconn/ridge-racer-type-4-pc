#include "types.h"
#include "m2c_macros.h"

extern int func_8002A344();
extern int func_8002FECC();
extern int func_8002FEFC();

void func_8002FE98(void) {
    func_8002FEFC();
    func_8002FECC();
    func_8002A344(0x10086);
}

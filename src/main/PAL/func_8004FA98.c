#include "types.h"
extern int func_8008A634();
extern int func_8008A624();
extern int func_8008C4FC();
void func_8004FA98(int a0) {
    u8 b[4];
    u8 *p;
    if (a0 == 0) {
        func_8008A634();
        p = b;
        b[0] = 0x80; b[1] = 0; b[2] = 0x80; b[3] = 0;
    } else {
        func_8008A624();
        p = b;
        b[0] = 0x64; b[1] = 0x64; b[2] = 0x64; b[3] = 0x64;
    }
    func_8008C4FC(p);
}

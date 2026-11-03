/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "types.h"
#include "m2c_macros.h"
#include "psyq/libapi/pad1.h"
#include "rr/memory.h"
#include "rr/spu_tick_control.h"

extern int func_80095DD4();
extern int func_80095E9C();
extern int func_80095F30();
extern int func_80096F84();
int func_80095DE4();
int func_80095DF4();
/* extern */
int func_80095FD4();

void func_80095D04(s32 arg0);
void func_80095D70(void);
void func_80095DA8(void);

void func_80095D04(s32 arg0) {
    s32 var_s0;

    var_s0 = arg0;
    func_80096F84(0);
    func_80096F04();
    if (func_800973D0() == 0) {
        var_s0 = 0;
    }
    func_80095DD4(var_s0);
    func_80095FA0();
    func_80095E9C();
    func_80095F30();
    func_80096F14();
}

/* legacy typing of shared globals in this part of the unit */
#define func_80096F84 ((int (*)(int))func_80096F84)

/* extern */
void func_80095D70(void) {
    func_80096F04();
    func_80095DE4();
    func_80096F84(0);
    func_80096F14();
}

#undef func_80096F84

/* extern */
void func_80095DA8(void) {
    func_80095DF4();
    func_80095FD4();
}

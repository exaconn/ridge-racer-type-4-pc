/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "types.h"
#include "m2c_macros.h"
#include "rr/interrupts.h"

extern int (**D_800A6350)();
extern u16 D_800A52CA;
extern u16 *D_800A6358;

void func_8008B428();
void func_8008B458();
void func_8008B488();
u16 func_8008B4B8(void);
u16 func_8008B4C8(void);
s32 func_8008B4E0(s32 value);

void func_8008B428() { D_800A6350[5](); }

void func_8008B458() { D_800A6350[4](); }

void func_8008B488() { D_800A6350[6](); }

u16 func_8008B4B8(void) {
    return D_800A52CA;
}

u16 func_8008B4C8(void) {
    return *D_800A6358;
}

s32 func_8008B4E0(s32 value) {
    s32 previous=*D_800A6358;
    *D_800A6358=value;
    return previous;
}

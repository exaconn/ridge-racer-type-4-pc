/* CC1_FLAGS: -mno-split-addresses */
#include "types.h"
#include "m2c_macros.h"
#include "rr/serial.h"

extern int func_80096F04();
extern int func_80096F14();
extern int func_8009B9E4();
extern M2C_UNK D_800ABD50;
M2C_UNK func_80096E14();
/* extern */
M2C_UNK func_8009B9F4(M2C_UNK *);
/* extern */
extern M2C_UNK D_80012104;

void func_8009A8B4(void);
void func_8009A8F8(void);
void func_8009A944(void);
s32 func_8009A94C(s32 first, s32 second);

void func_8009A8B4(void) {
    s32 temp_s0;

    temp_s0 = func_80096F04();
    func_8009B9E4(&D_800ABD50);
    if (temp_s0 == 1) {
        func_80096F14();
    }
}

/* legacy typing of shared globals in this part of the unit */
#define func_80096F14 ((M2C_UNK (*)())func_80096F14)

void func_8009A8F8(void) {
    s32 temp_s0;

    temp_s0 = func_80096F04();
    func_8009B9F4(&D_80012104);
    func_80096E14();
    if (temp_s0 == 1) {
        func_80096F14();
    }
}

#undef func_80096F14

void func_8009A944(void) {
}

s32 func_8009A94C(s32 first, s32 second) {
    volatile RrSerialRequest *request = &D_801146E8;
    RrSerialPortRegisters *registers;

    if (request->active == 0) {
        request->second = second;
        request->first = first;
        registers = D_800ABD18;
        request->active = 1;
        /* Keep the separate read/write operations on the control register. */
        registers->control |= 0x800;
        registers->control |= 0x20;
        return 0;
    }
    return -1;
}

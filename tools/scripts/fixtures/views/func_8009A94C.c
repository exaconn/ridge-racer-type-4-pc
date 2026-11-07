/* CC1_FLAGS: -mno-split-addresses */

#include "rr/serial.h"

/* legacy typing of shared globals in this part of the unit */
#define ExitCriticalSection ((M2C_UNK (*)())ExitCriticalSection)

#undef ExitCriticalSection

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

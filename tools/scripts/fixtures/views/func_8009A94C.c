/* CC1_FLAGS: -mno-split-addresses */

#include "rr/serial.h"

/* legacy typing of shared globals in this part of the unit */
#define ExitCriticalSection ((M2C_UNK (*)())ExitCriticalSection)

#undef ExitCriticalSection

s32 _sio_write_async(s32 first, s32 second) {
    volatile SerialRequest *request = &SioRequest;
    SerialPortRegisters *registers;

    if (request->active == 0) {
        request->second = second;
        request->first = first;
        registers = SIO1_REGS;
        request->active = 1;
        /* Keep the separate read/write operations on the control register. */
        registers->control |= 0x800;
        registers->control |= 0x20;
        return 0;
    }
    return -1;
}

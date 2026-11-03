/* GCC_VERSION: 2.7.2 */

#include "rr/serial.h"

s32 func_8009ABF8(s32 first, s32 second) {
    RrSerialReceiveRequest *state = &D_801146D8;
    RrSerialPortRegisters *port;
    /* ASM_BARRIER: retain the request base for its adjacent fields. */
    __asm__("" : "=r"(state) : "0"(state));
    if (state->request.active != 0) return -1;
    state->request.second = second;
    /* ASM_BARRIER: publish the second parameter before the first parameter and activation. */
    __asm__ volatile("" : : : "memory");
    state->request.first = first;
    port = D_800ABD18;
    state->request.active = 1;
    state->receive_status = port->status & 0x80;
    port->control |= 0x400;
    return 0;
}

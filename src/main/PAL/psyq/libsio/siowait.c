/* GCC_VERSION: 2.7.2 */
#include "rr/serial.h"
#include "psyq/libsio/siowait.h"

s32 _sio_read_async(s32 first, s32 second) {
    RrSerialReceiveRequest *state = &SioReceiveRequest;
    RrSerialPortRegisters *port;
    /* ASM_BARRIER: retain the request base for its adjacent fields. */
    __asm__("" : "=r"(state) : "0"(state));
    if (state->request.active != 0) return -1;
    state->request.second = second;
    /* ASM_BARRIER: publish the second parameter before the first parameter and activation. */
    __asm__ volatile("" : : : "memory");
    state->request.first = first;
    port = SIO1_REGS;
    state->request.active = 1;
    state->receive_status = port->status & 0x80;
    port->control |= 0x400;
    return 0;
}

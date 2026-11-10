/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/serial.h"
#include "psyq/libsio/siowait.h"

s32 _sio_read_async(u8 *buffer, s32 length) {
    volatile SerialReceiveRequest *state = &SioReceiveRequest;
    SerialPortRegisters *port;
    if (state->request.active != 0) return -1;
    state->request.remaining = length;
    state->request.cursor = buffer;
    port = SIO1_REGS;
    state->request.active = 1;
    state->receive_status = port->status & 0x80;
    port->control |= 0x400;
    return 0;
}

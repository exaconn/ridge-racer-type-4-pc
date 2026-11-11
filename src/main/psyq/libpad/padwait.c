#include "rr_asm.h"
#include "rr/serial.h"
#include "psyq/libpad/padwait.h"

void _padWaitRXready(void) {
    SerialPortRegisters *port = PadSio0Base;

    do {
    } while (!(port->status & 2));
}

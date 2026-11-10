/* GCC_VERSION: 2.7.2 */
#include "rr_asm.h"
#include "rr/serial.h"
#include "psyq/libpad/padwait.h"

void _padWaitRXready(void) {
    SerialPortRegisters *port = PadSio0Base;

    /* ASM_BARRIER: keep the pointer's load delay before the polling loop label. */
    RR_NOP_DEP();
    do {
    } while (!(port->status & 2));
}

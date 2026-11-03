/* GCC_VERSION: 2.7.2 */
#include "rr/serial.h"
#include "psyq/libpad/padwait.h"

void func_80098A90(void) {
    RrSerialPortRegisters *port = D_800ABCCC;

    /* ASM_BARRIER: keep the pointer's load delay before the polling loop label. */
    __asm__ volatile("nop");
    do {
    } while (!(port->status & 2));
}

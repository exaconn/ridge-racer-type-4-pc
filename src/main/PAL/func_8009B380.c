/* CC1_FLAGS: -mno-split-addresses */
#include "rr/interrupts.h"
#include "rr/serial.h"
s32 func_8009B380(void) {
    RrInterruptRegisters *interrupts=D_800ABD30;
    /* ASM_BARRIER: Keep the predicate result in the common return register. */
    register s32 result;
    if(interrupts->pending & interrupts->mask & 0x100) {
        RrSerialReceiveRequest *request=&D_801146D8;
        /* ASM_BARRIER: Keep the original address materialization before the active load. */
        __asm__("" : "+r"(request));
        if(request->request.active) {
            RrSerialPortRegisters *port=D_800ABD18;
            if(!(port->status & 1)) {
                result=1;
                goto done;
            }
            port->control &= 0xFBFF;
        }
        result=1;
    } else result=0;
done:
    /* ASM_BARRIER: Keep the selected result before the shared return instruction. */
    __asm__ volatile("" : : "r"(result));
    return result;
}

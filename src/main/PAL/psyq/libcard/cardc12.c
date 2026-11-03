/* GCC_VERSION: 970318 */
#include "rr_asm.h"
#include "rr/commands.h"
#include "rr/serial.h"
s32 func_8009CF94(s32 unused) {
    s32 received;
    RrSerialPortRegisters *port;
    s32 result;
    if(D_800ABDF8==3) {
        port=D_800ABDE0;
        RR_NOP_DEP_ON(port);
        while(!(port->status & 2)) { }
        D_80114728=D_800ABDE0->data;
    } else if(D_800ABDF8>=4) {
        if((u32)D_800ABDF8==(u32)D_80114728+4) {
            port=D_800ABDE0;
            RR_NOP_DEP_ON(port);
            while(!(port->status & 2)) { }
            received=D_800ABDE0->data;
            D_80114718=D_8011471C;
            D_8011472C.value=1;
            D_80114730=received+1;
        }
    }
    result=0;
    return result;
}

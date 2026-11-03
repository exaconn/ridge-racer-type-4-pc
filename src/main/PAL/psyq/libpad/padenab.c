/* CC1_FLAGS: -mno-split-addresses */
#include "rr/channels.h"
#include "rr/serial.h"
s32 func_80097EA0(void) {
    s32 first,condition,enabled;
    u32 shifted;
    RrSerialPortRegisters *port=D_800ABCCC;
    if(port->control&2) {*(u16 *)&port->control=0;goto out;}
    condition=D_800ABCB8;
    /* ASM_BARRIER: Form the enable page after the serial-control gate. */
    __asm__("" : : : "$2");
    enabled=1;
    /* ASM_BARRIER: Materialize the enable value before the PAL page address. */
    __asm__("" : "+r"(enabled) : "r"(condition) : "$1");
    {
        /* ASM_BARRIER: Retain the actual PAL state page for the enable-store branch delay. */
        register s32 *page __asm__("$1")=(s32 *)0x800B0000U;
        /* ASM_BARRIER: Expose the enable store independently of its global-address macro. */
        __asm__("" : "+r"(page));
        page[-0x4330/4]=enabled;
    }
    if(condition) {
        s32 *counter=D_80114428;
        s32 value=*counter;
        if(value<150)*counter=value+1;
    }
    if(!D_800ABCBC) {
        s32 *counter=D_80114428+1;
        s32 value=*counter;
        if(value<150)*counter=value+1;
    }
    if(!D_800ABCA0)goto out;
    first=D_800ABCB8;
    /* Stage 15 times the index; quotient/remainder preserve native record sizes. */
    shifted=(u32)first<<4;
    if(D_800ABCBC<first)goto out;
    D_800ABCAC=0;
    D_800ABCA8=first;
    if(!func_80098154((RrChannelRecordF0 *)((unsigned long)D_800ABC9C+(long)(s32)((shifted-(u32)first)*(sizeof(RrChannelRecordF0)/15)+(u32)first*(sizeof(RrChannelRecordF0)%15)))))D_800ABC68(65535);
    D_800ABCB0=0;
    while(D_800ABCA8<=D_800ABCBC)func_80098488(D_800ABC9C+D_800ABCA8);
    D_800ABCCC->baud=0x88;
out:
    return 0;
}

/* CC1_FLAGS: -mno-split-addresses */
#include "rr/commands.h"
#include "rr/serial.h"
#include "rr_asm.h"

s32 func_8009C0F4(void) {
    register s32 initial=D_800ABDE8;
    s32 one;
    /* ASM_BARRIER: Keep each common-return result in the ABI result register. */
    register s32 answer __asm__("$2");
    /* ASM_BARRIER: Preserve the original masked exchange argument and delay slot. */
    register s32 byte __asm__("$4");
    register s32 value;
    one=1;
    if(initial==one) {
        s32 slot=D_800ABDEC;
        register u32 header;
        register u32 count;
        D_800ABDE4=4;
        /* ASM_BARRIER: Capture the request before preparing the state-four temporary. */
        __asm__("" : : : "$2");
        func_8009BD44(slot>>4);
        func_8009BE18(~((D_800ABDEC&15)+0x81));
        /* ASM_BARRIER: Retain an immediate increment instead of reusing saved constant one. */
        __asm__ __volatile__("" : "+r"(one));
        header=*(u8 *)&D_800ABDEC;
        count=D_800ABDF4;
        D_801147D8[0]=(header&15)-0x7F;
        D_800ABDF4=count+1;
        answer=0;goto done;
    }
    {
        register u32 remaining=D_80114730;
        register RrCommandReadCallback reader=D_80114734;
        /* ASM_BARRIER: Publish the real remaining-count store in the reader call delay slot. */
        register s32 *page __asm__("$1");
        remaining--;
        /* ASM_BARRIER: Finish the decrement before forming its mapped store address. */
        __asm__("" : : : "$1");
        page=(s32 *)0x80110000U;
        /* ASM_BARRIER: Expose the PAL RAM page to the stock delay-slot scheduler. */
        __asm__("" : "+r"(page));
        page[0x4730/4]=remaining;
        byte=reader()&255;
    }
    {
        register s32 *count=&D_800ABDF4;
        u32 index;
        index=*count;
        D_801147D8[index]=byte;
        *count=index+1;
        value=func_8009BE18(byte);
    }
    byte=value&255;
    if(value<0) {answer=(u32)value-1000;goto done;}
    {
        register s32 *phase=&D_800ABDF8;
        u32 index;
        register RrCommandWriteCallback writer;
        index=*phase;
        D_80114740[index]=value;
        writer=D_80114738;
        *phase=index+1;
        value=writer(byte);
    }
    if(value) {answer=(u32)value-2000;goto done;}
    if(D_80114730!=one) {answer=0;goto done;}
    if(D_80114870==14) goto special;
    /* ASM_BARRIER: Retain the separate fresh subtype read for the second comparison. */
    __asm__("" : : : "$2", "$3", "memory");
    if(D_80114870!=15) goto wait;
special:
    if((u32)D_800ABDF8==(u32)D_80114728+4) goto final_byte;
wait:
    {
        RrSerialPortRegisters *port=D_800ABDE0;
        /* ASM_BARRIER: Keep the captured-port load gap before the shared polling label. */
        RR_NOP_DEP_ON(port);
        while(!(port->status&2)) {}
    }
final_byte:
    {
        register s32 *phase=&D_800ABDF8;
        u32 tick;
        u32 remaining,index;
        register u32 raw;
        RrSerialPortRegisters *port;
        register RrCommandWriteCallback writer;
        tick=D_800ABDE8;
        remaining=D_80114730;
        D_800ABDE8=tick+1;
        port=D_800ABDE0;
        D_80114730=remaining-1;
        raw=port->data;
        /* ASM_BARRIER: Read the device byte before loading the fresh receive index. */
        __asm__("" : "+r"(raw) : : "$3", "memory");
        index=*phase;
        value=raw&255;
        D_80114740[index]=value;
        index++;
        /* ASM_BARRIER: Increment the trace index before preparing the fresh writer and argument. */
        __asm__("" : "+r"(index) : : "$2", "$4");
        writer=D_80114738;
        *phase=index;
        value=writer(value);
    }
    if(value) answer=(u32)value-3000;
    else answer=1;
done:
    return answer;
}

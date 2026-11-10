/* MASPSX_FLAGS: --assembler-profile=gnu */
/* CC1_FLAGS: -mno-split-addresses */
#include "rr/commands.h"
#include "rr/serial.h"
#include "rr_asm.h"

s32 _cardCmdStep(void) {
    s32 initial=CardTxCount;
    s32 one;
    /* ASM_BARRIER: Preserve the original masked exchange argument and delay slot. */
    register s32 byte __asm__("$4");
    s32 value;
    one=1;
    if(initial==one) {
        s32 slot=CardCommandRequest;
        u32 header;
        u32 count;
        CardCommandStatus=4;
        /* ASM_BARRIER: Capture the request before preparing the state-four temporary. */
        __asm__("" : : : "$2");
        _cardSelectSlot(slot>>4);
        _cardXferByte(~((CardCommandRequest&15)+0x81));
        __asm__ __volatile__("" : "+r"(one));
        header=*(u8 *)&CardCommandRequest;
        count=CardReceiveIndex;
        D_801147D8[0]=(header&15)-0x7F;
        CardReceiveIndex=count+1;
        return 0;
    }
    {
        u32 remaining=CardBytesRemaining;
        CommandReadCallback reader=CardCommandReader;
        s32 *page;
        remaining--;
        page=(s32 *)0x80110000U;
        page[0x4730/4]=remaining;
        byte=reader()&255;
    }
    {
        s32 *count=&CardReceiveIndex;
        u32 index;
        index=*count;
        D_801147D8[index]=byte;
        *count=index+1;
        value=_cardXferByte(byte);
    }
    byte=value&255;
    if(value<0) {return value-1000U;}
    {
        s32 *phase=&CardRxCount;
        u32 index;
        CommandWriteCallback writer;
        index=*phase;
        CardRxTrace[index]=value;
        writer=CardWriteHandler;
        *phase=index+1;
        value=writer(byte);
    }
    if(value) {return value-2000;}
    if(CardBytesRemaining!=one) {return 0;}
    if(CardCurrentCommand==14) goto special;
    /* ASM_BARRIER: Retain the separate fresh subtype read for the second comparison. */
    __asm__("" : : : "$2", "$3", "memory");
    if(CardCurrentCommand!=15) goto wait;
special:
    if(CardRxCount==CardCmdCount+4U) goto final_byte;
wait:
    {
        SerialPortRegisters *port=CardCmdSioRegs;
        /* ASM_BARRIER: Keep the captured-port load gap before the shared polling label. */
        RR_NOP_DEP_ON(port);
        while(!(port->status&2)) {}
    }
final_byte:
    {
        s32 *phase=&CardRxCount;
        u32 tick;
        u32 remaining,index;
        u32 raw;
        SerialPortRegisters *port;
        CommandWriteCallback writer;
        tick=CardTxCount;
        remaining=CardBytesRemaining;
        CardTxCount=tick+1;
        port=CardCmdSioRegs;
        CardBytesRemaining=remaining-1;
        raw=port->data;
        /* ASM_BARRIER: Read the device byte before loading the fresh receive index. */
        __asm__("" : "+r"(raw) : : "$3", "memory");
        index=*phase;
        value=raw&255;
        CardRxTrace[index]=value;
        index++;
        /* ASM_BARRIER: Increment the trace index before preparing the fresh writer and argument. */
        __asm__("" : "+r"(index) : : "$2", "$4");
        writer=CardWriteHandler;
        *phase=index;
        value=writer(value);
    }
    if(value) return value-3000;
    return 1;
}

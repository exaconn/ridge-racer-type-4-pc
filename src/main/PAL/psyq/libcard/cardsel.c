/* CC1_FLAGS: -mno-split-addresses */
#include "rr_asm.h"
#include "rr/serial.h"

typedef struct { unsigned char b0; char p0[3]; unsigned short h4; char p1[4]; unsigned short hA; } S;

int func_8009BD44();
s32 func_8009BE18(s32 value);

/* legacy typing of shared globals in this part of the unit */
#define D_800ABDD0 ((S *)D_800ABDD0)
#define D_800ABDD4 ((int *)D_800ABDD4)
#define func_8009A7F4 ((int (*)())func_8009A7F4)

int func_8009BD44(a0) int a0; {
    volatile S *p, *q = D_800ABDD0;
    q->hA = 0;
    func_8009A7F4(0x91);
    p = D_800ABDD0;
    p->hA = a0 ? 0x3003 : 0x1003;
    if (p->h4 & 0x200) {
        p->hA |= 0x10;
        if (p->h4 & 0x200) {
            p->b0 = 1;
            p = D_800ABDD0;
            /* ASM_BARRIER: orig has the load-delay nop BEFORE the loop label; maspsx emits it after the label (inside the loop) */
            RR_NOP_DEP();
            while ((p->h4 & 2) == 0) ;
            p = D_800ABDD0;
            p->b0;
            return 0;
        }
        *D_800ABDD4 = -0x81;
    }
    return 1;
}

#undef D_800ABDD0
#undef D_800ABDD4
#undef func_8009A7F4

s32 func_8009BE18(s32 value) {
    s32 ready, completed, input;
    RrSerialPortRegisters *initial=D_800ABDD0;
    RrSerialPortRegisters *port;
    u32 *interrupt;
    {
        /* ASM_BARRIER: Keep the captured byte in the original zero-extended temporary. */
        register s32 byte __asm__("$2")=initial->data;
        /* ASM_BARRIER: Publish the captured byte in the original branch delay slot. */
        register s32 *page __asm__("$1");
        /* ASM_BARRIER: Capture the incoming byte before saving the outgoing value. */
        __asm__("" : : "r"(byte) : "$16");
        input=value;
        /* ASM_BARRIER: Finish the input capture before forming the saved-byte page. */
        __asm__("" : : : "$1");
        page=(s32 *)0x80110000U;
        /* ASM_BARRIER: Expose the real mapped store to the branch-delay scheduler. */
        __asm__("" : "+r"(page));
        page[0x4700/4]=byte;
    }
    if(input<0) {
        while(!(initial->status&1)) {}
        while(!func_8009A814()) {}
        {
            /* ASM_BARRIER: Form the complemented byte in the original polling delay slot. */
            register u32 complement __asm__("$2")=~input;
            RrSerialPortRegisters *output=D_800ABDD0;
            /* ASM_BARRIER: Preserve the original load gap before the shared success jump. */
            RR_NOP_DEP_ON(output);
            /* Ordinary write view retains the original jump-delay store. */
            *(u8 *)&output->data=complement;
        }
        goto success;
    }
    goto irq_test;
irq_wait:
    if(func_8009A814()) { ready=0;goto irq_done; }
irq_test:
    if(!(*D_800ABDD4&0x80)) goto irq_wait;
    ready=1;
irq_done:
    if(!ready) return (u32)-0x200-(u32)D_80114700;
    func_8009A7F4(5);
    while(!func_8009A814()) {}
    {
        s32 limit=60;
        RrSerialPortRegisters *output;
        /* ASM_BARRIER: Prepare the next timer limit before loading the fresh output port. */
        __asm__("" : "+r"(limit));
        output=D_800ABDD0;
        /* ASM_BARRIER: Retain the original output-port load gap before launching the timer. */
        RR_NOP_DEP_ON(output);
        /* Ordinary write view retains the original call-delay store. */
        *(u8 *)&output->data=input;
        func_8009A7F4(limit);
    }
    interrupt=D_800ABDD4;
    port=D_800ABDD0;
    *interrupt=~0x80U;
    if(port->status&0x80) {
        do {
            if(func_8009A814()) { completed=0;goto transmit_done; }
        } while(D_800ABDD0->status&0x80);
    }
    completed=1;
    D_800ABDD0->control|=0x10;
transmit_done:
    if(!completed) return (u32)-0x300-(u32)D_80114700;
success:
    return D_80114700;
}

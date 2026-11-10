/* MASPSX_FLAGS: --assembler-profile=gnu */
/* CC1_FLAGS: -mno-split-addresses */
#include "rr_asm.h"
#include "rr/serial.h"

typedef struct { unsigned char b0; char p0[3]; unsigned short h4; char p1[4]; unsigned short hA; } S;

int _cardSelectSlot(a0) int a0; {
    volatile S *p, *q = CardSio0Base;
    q->hA = 0;
    setRC2wait(0x91);
    p = CardSio0Base;
    p->hA = a0 ? 0x3003 : 0x1003;
    if (p->h4 & 0x200) {
        p->hA |= 0x10;
        if (p->h4 & 0x200) {
            p->b0 = 1;
            p = CardSio0Base;
            /* ASM_BARRIER: orig has the load-delay nop BEFORE the loop label; maspsx emits it after the label (inside the loop) */
            RR_NOP_DEP();
            while ((p->h4 & 2) == 0) ;
            p = CardSio0Base;
            p->b0;
            return 0;
        }
        *I_STAT_CardSel = -0x81;
    }
    return 1;
}


s32 _cardXferByte(s32 value) {
    s32 ready, completed, input;
    SerialPortRegisters *initial=CardSio0Base;
    SerialPortRegisters *port;
    u32 *interrupt;
    {
        u8 byte=initial->data;
        s32 *page;
        input=value;
        page=(s32 *)0x80110000U;
        page[0x4700/4]=byte;
    }
    if(input<0) {
        while(!(initial->status&1)) {}
        while(!chkRC2wait()) {}
        {
            /* ASM_BARRIER: Form the complemented byte in the original polling delay slot. */
            register u32 complement __asm__("$2")=~input;
            SerialPortRegisters *output=CardSio0Base;
            /* ASM_BARRIER: Preserve the original load gap before the shared success jump. */
            RR_NOP_DEP_ON(output);
            /* Ordinary write view retains the original jump-delay store. */
            *(u8 *)&output->data=complement;
        }
        goto success;
    }
    goto irq_test;
irq_wait:
    if(chkRC2wait()) { ready=0;goto irq_done; }
irq_test:
    if(!(*I_STAT_CardSel&0x80)) goto irq_wait;
    ready=1;
irq_done:
    if(!ready) return (u32)-0x200-D_80114700;
    setRC2wait(5);
    while(!chkRC2wait()) {}
    {
        s32 limit=60;
        SerialPortRegisters *output;
        /* ASM_BARRIER: Prepare the next timer limit before loading the fresh output port. */
        __asm__("" : "+r"(limit));
        output=CardSio0Base;
        /* ASM_BARRIER: Retain the original output-port load gap before launching the timer. */
        RR_NOP_DEP_ON(output);
        /* Ordinary write view retains the original call-delay store. */
        *(u8 *)&output->data=input;
        setRC2wait(limit);
    }
    interrupt=I_STAT_CardSel;
    port=CardSio0Base;
    *interrupt=~0x80U;
    if(port->status&0x80) {
        do {
            if(chkRC2wait()) { completed=0;goto transmit_done; }
        } while(CardSio0Base->status&0x80);
    }
    completed=1;
    CardSio0Base->control|=0x10;
transmit_done:
    if(!completed) return (u32)-0x300-D_80114700;
success:
    return D_80114700;
}

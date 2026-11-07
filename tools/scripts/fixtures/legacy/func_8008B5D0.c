/* MASPSX_FLAGS: --assembler-profile=gnu */
/* CC1_FLAGS: -mno-split-addresses */
#include "rr/interrupt_dispatch.h"
void trapIntr(void) {
    RrLibSavedState *state=&D_800A52C8;
    RrLibInterruptCallback *base;
    register u32 one __asm__("$19");
    RrLibInterruptCallback *callbacks;
    u32 pending;
    u32 next;
    register s32 index __asm__("$17");
    if(state->active==0) {
        DebugPrintStub(UnexpectedInterruptFmt,*D_800A6354);
        ReturnFromException();
    }
    {
        register volatile u16 *status __asm__("$4")=D_800A6354;
        u32 enabled=state->enabled_mask;
        volatile u16 *mask;
        register u32 statusValue __asm__("$4");
        register u32 maskValue __asm__("$2");
        state->dispatching02=1;
        mask=I_MASK;
        statusValue=*status;
        maskValue=*mask;
        /* Preserve the initialized register values through mask composition. */
        __asm__("" : : "r"(enabled),"r"(statusValue));
        enabled&=statusValue;
        next=maskValue&enabled;
        pending=next;
    }
    if(next) {
        one=1;
        base=state->callbacks;
        do {
            index=0;
            if(pending) {
                callbacks=base;
again:
                if(index>=11)goto reread;
                if(pending&1) {
                    RrLibInterruptCallback callback;
                    RR_IRQ_ACK16(D_800A6354,~(one<<index));
                    callback=*callbacks;
                    if(callback) callback();
                }
                callbacks++;
                pending>>=1;
                index++;
                if(pending&0xFFFF)goto again;
            }
reread:
            {
                register volatile u16 *status __asm__("$4")=D_800A6354;
                u32 enabled=D_800A52C8.enabled_mask;
                volatile u16 *mask=I_MASK;
                register u32 statusValue __asm__("$4")=*status;
                register u32 maskValue __asm__("$2")=*mask;
                __asm__("" : : "r"(enabled),"r"(statusValue));
                enabled&=statusValue;
                next=maskValue&enabled;
            }
            pending=next;
        } while(next);
    }
    {
        volatile u16 *status=D_800A6354;
        volatile u16 *mask=I_MASK;
        if(*status & *mask) {
            s32 *counter=&IntrStuckCount;
            u32 count=(u32)*counter;
            s32 previous=(s32)count;
            count++;
            *counter=(s32)count;
            if(previous>=0x801) {
                DebugPrintStub(({const char *format=D_80011858; __asm__("" : "+r"(format));format;}),*status,*mask);
                IntrStuckCount=0;
                RR_IRQ_ACK16(D_800A6354,0);
            }
        } else IntrStuckCount=0;
    }
    D_800A52C8.dispatching02=0;
    ReturnFromException();
}

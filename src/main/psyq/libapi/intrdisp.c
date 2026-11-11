#include "rr_asm.h"
#include "rr/interrupt_dispatch.h"
#include "rr/library.h"
#include "psyq/libapi/intrdisp.h"
#include "rr/spu_tick_control.h"

extern s32 ChangeClearPAD(s32);
extern s32 ChangeClearRCnt(s32,s32);
extern void ResetEntryInt();
extern void HookEntryInt(void *data);

LibInterruptCallback setIntr(s32 id,LibInterruptCallback callback);
LibSavedState *stopIntr(void);
LibSavedState *restartIntr(void);

void trapIntr(void) {
    LibSavedState *state=&IntrDispatchState;
    LibInterruptCallback *base;
    u32 one;
    LibInterruptCallback *callbacks;
    u32 pending;
    u32 next;
    s32 index;
    volatile u16 *status;
    if(state->active==0) {
        DebugPrintStub(UnexpectedInterruptFmt,*I_STAT);
        ReturnFromException();
    }
    {
        u16 enabled;
        volatile u16 *mask;
        u16 statusValue;
        u16 maskValue;
        status=I_STAT;
        enabled=state->enabled_mask;
        state->dispatching02=1;
        mask=I_MASK;
        statusValue=*status;
        maskValue=*mask;
        enabled&=statusValue;
        maskValue&=enabled;
        next=maskValue;
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
                    LibInterruptCallback callback;
                    RR_IRQ_ACK16(I_STAT,~(one<<index));
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
                u16 enabled;
                volatile u16 *mask;
                u16 statusValue;
                u16 maskValue;
                status=I_STAT;
                enabled=IntrDispatchState.enabled_mask;
                mask=I_MASK;
                statusValue=*status;
                maskValue=*mask;
                enabled&=statusValue;
                maskValue&=enabled;
                next=maskValue;
            }
            pending=next;
        } while(next);
    }
    {
        volatile u16 *st=I_STAT, *mk=I_MASK;
        if(*st & *mk) {
            s32 *counter=&IntrStuckCount;
            u32 count=(u32)*counter;
            s32 previous=count;
            count++;
            *counter=count;
            if(previous>=0x801) {
                DebugPrintStub(IntrTimeoutFmt,*st,*mk);
                IntrStuckCount=0;
                RR_IRQ_ACK16(I_STAT,0);
            }
        } else IntrStuckCount=0;
    }
    IntrDispatchState.dispatching02=0;
    ReturnFromException();
}

LibInterruptCallback setIntr(s32 id,LibInterruptCallback callback) {
    s32 selector=id;
    LibInterruptCallback requested=callback;
    LibInterruptCallback *base;
    LibInterruptCallback *entry;
    LibInterruptCallback previous;
    LibSavedState *state;
    u32 saved;
    s32 disabled;
    base=IntrDispatchState.callbacks;
    entry=(LibInterruptCallback *)((selector*sizeof(*base))+(unsigned long)base);
    previous=*entry;
    if(requested==previous) goto out;
    state=(LibSavedState *)((unsigned long)base-RR_FIELD_OFFSET(LibSavedState,callbacks));
    if(!state->active) goto out;
    {
        u16 raw=*I_MASK;
        *I_MASK=0;
        saved=raw;
    }
    if(requested) {
        u32 bit=1U<<selector;
        *entry=requested;
        saved |= bit;
        state->enabled_mask |= bit;
    } else {
        u32 bit;
        bit=~(1U<<selector);
        *entry=0;
        saved &= bit;
        *(u16 *)((unsigned long)base+RR_FIELD_OFFSET(LibSavedState,enabled_mask)-RR_FIELD_OFFSET(LibSavedState,callbacks)) &= bit;
    }
    if(selector==0) {
        disabled=requested==0;
        ChangeClearPAD(disabled);
        ChangeClearRCnt(3,disabled);
    }
    if(selector==4)ChangeClearRCnt(0,requested==0);
    if(selector==5)ChangeClearRCnt(1,requested==0);
    if(selector==6)ChangeClearRCnt(2,requested==0);
    *I_MASK=saved;
out:
    return previous;
}

LibSavedState *stopIntr(void) {
  LibSavedState *p = &IntrDispatchState;
  unsigned short *a, *c; int *b; int t;
  if (p->active == 0) return 0;
  EnterCriticalSection();
  a = I_MASK;
  b = DPCR_Intr;
  p->register16 = *a;
  t = *b;
  c = I_STAT;
  p->register32 = t;
  *a = 0;
  *c = *(volatile unsigned short *)a;
  *DPCR_Intr = *DPCR_Intr & 0x77777777;
  ResetEntryInt();
  p->active = 0;
  return p; }

LibSavedState *restartIntr(void) {
    LibSavedState *state = &IntrDispatchState;
    if (state->active != 0) return 0;
    HookEntryInt(state->callback_data);
    state->active = 1;
    *I_MASK = state->register16;
    {
        s32 *destination = DPCR_Intr;
        s32 value = state->register32;
        *destination = value;
    }
    ExitCriticalSection();
    return state;
}

void memclr(p, n) int *p; int n; {
    while (n--) *p++ = 0;
}

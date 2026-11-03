/* MASPSX_FLAGS: --assembler-profile=gnu */
/* CC1_FLAGS: -mno-split-addresses */
#include "rr/interrupt_dispatch.h"
#include "rr/library.h"

extern s32 func_80096F84(s32),func_80096F94(s32,s32);
extern int *D_800A635C;
extern void func_80096F04(), func_80096EE4();
extern void func_80096EF4(void *data);
extern void func_80096F14(void);

void func_8008B5D0(void);
RrLibInterruptCallback func_8008B7A0(s32 id,RrLibInterruptCallback callback);
RrLibSavedState *func_8008B8E8(void);
RrLibSavedState *func_8008B988(void);
void func_8008BA00();

void func_8008B5D0(void) {
    RrLibSavedState *state=&D_800A52C8;
    RrLibInterruptCallback *base;
    register u32 one __asm__("$19");
    RrLibInterruptCallback *callbacks;
    u32 pending;
    u32 next;
    register s32 index __asm__("$17");
    if(state->active==0) {
        func_8001E84C(D_8001183C,*D_800A6354);
        func_80096ED4();
    }
    {
        register volatile u16 *status __asm__("$4")=D_800A6354;
        u32 enabled=state->enabled_mask;
        volatile u16 *mask;
        register u32 statusValue __asm__("$4");
        register u32 maskValue __asm__("$2");
        state->dispatching02=1;
        mask=D_800A6358;
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
                volatile u16 *mask=D_800A6358;
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
        volatile u16 *mask=D_800A6358;
        if(*status & *mask) {
            s32 *counter=&D_800A6360;
            u32 count=(u32)*counter;
            s32 previous=(s32)count;
            count++;
            *counter=(s32)count;
            if(previous>=0x801) {
                func_8001E84C(({const char *format=D_80011858; __asm__("" : "+r"(format));format;}),*status,*mask);
                D_800A6360=0;
                RR_IRQ_ACK16(D_800A6354,0);
            }
        } else D_800A6360=0;
    }
    D_800A52C8.dispatching02=0;
    func_80096ED4();
}

RrLibInterruptCallback func_8008B7A0(s32 id,RrLibInterruptCallback callback) {
    s32 selector=id;
    /* ASM_BARRIER: Preserve the requested callback across kernel calls. */
    register RrLibInterruptCallback requested __asm__("$18")=callback;
    RrLibInterruptCallback *base;
    RrLibInterruptCallback *entry;
    RrLibInterruptCallback previous;
    RrLibSavedState *state;
    u32 saved;
    s32 disabled;
    /* ASM_BARRIER: Capture both full arguments before reusing argument registers for addresses. */
    __asm__("" : "+r"(selector), "+r"(requested));
    base=D_800A52C8.callbacks;
    entry=(RrLibInterruptCallback *)((unsigned long)((u32)selector*sizeof(*base))+(unsigned long)base);
    previous=*entry;
    if(requested==previous) goto out;
    state=(RrLibSavedState *)((unsigned long)base-RR_FIELD_OFFSET(RrLibSavedState,callbacks));
    if(!state->active) goto out;
    {
        u16 raw=*D_800A6358;
        *D_800A6358=0;
        saved=raw;
    }
    if(requested) {
        u32 bit=1U<<selector;
        *entry=requested;
        saved |= bit;
        state->enabled_mask |= bit;
    } else {
        u32 bit;
        /* ASM_BARRIER: Keep the null callback's table store independent of its saved register. */
        __asm__("" : "+r"(requested));
        bit=~(1U<<selector);
        *entry=0;
        saved &= bit;
        *(u16 *)((unsigned long)base+RR_FIELD_OFFSET(RrLibSavedState,enabled_mask)-RR_FIELD_OFFSET(RrLibSavedState,callbacks)) &= bit;
    }
    if(selector==0) {
        disabled=requested==0;
        func_80096F84(disabled);
        func_80096F94(3,disabled);
    }
    if(selector==4)func_80096F94(0,requested==0);
    if(selector==5)func_80096F94(1,requested==0);
    if(selector==6)func_80096F94(2,requested==0);
    *D_800A6358=saved;
out:
    return previous;
}

/* legacy typing of shared globals in this part of the unit */
#define D_800A6358 ((unsigned short *)D_800A6358)
#define D_800A6354 ((unsigned short *)D_800A6354)

RrLibSavedState *func_8008B8E8(void) {
  RrLibSavedState *p = &D_800A52C8;
  unsigned short *a, *c; int *b; int t;
  if (p->active == 0) return 0;
  func_80096F04();
  a = D_800A6358;
  b = D_800A635C;
  p->register16 = *a;
  t = *b;
  c = D_800A6354;
  p->register32 = t;
  *a = 0;
  *c = *(volatile unsigned short *)a;
  *D_800A635C = *D_800A635C & 0x77777777;
  func_80096EE4();
  p->active = 0;
  return p; }

#undef D_800A6358
#undef D_800A6354

/* legacy typing of shared globals in this part of the unit */
#define D_800A6358 ((u16 *)D_800A6358)

RrLibSavedState *func_8008B988(void) {
    RrLibSavedState *state = &D_800A52C8;
    if (state->active != 0) return 0;
    func_80096EF4(state->callback_data);
    state->active = 1;
    *D_800A6358 = state->register16;
    {
        s32 *destination = D_800A635C;
        s32 value = state->register32;
        /* ASM_BARRIER: retain the original load-delay nop before the call/store pair. */
        __asm__ volatile("nop");
        *destination = value;
    }
    func_80096F14();
    return state;
}

#undef D_800A6358

void func_8008BA00(p, n) int *p; int n; {
    while (n--) *p++ = 0;
}

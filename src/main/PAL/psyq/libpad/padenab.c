/* CC1_FLAGS: -mno-split-addresses */
#include "types.h"
#include "rr/channels.h"
#include "rr/serial.h"
#include "rr/interrupts.h"

extern u32 D_80114418[4];
extern s32 func_80096F04(void),func_80096F94(s32,s32);
extern void func_80096F14(void);
extern s32 func_80097714(s32,void *),func_80097704(s32,void *);

s32 func_80097EA0(void);
s32 func_80098030(void);
void func_80098044(void);

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

s32 func_80098030(void) {
    s32 previous = D_800ABCD0;
    /* ASM_BARRIER: use the original assembler temporary for the store address. */
    register s32 *base __asm__("$1");
    /* ASM_BARRIER: load the prior value before materializing the store address. */
    __asm__ volatile("" : "=r"(previous) : "0"(previous) : "$1");
    /* PAL RAM mapping: D_800ABCD0 is base plus the signed offset below. */
    base = (s32 *)0x800B0000U;
    /* ASM_BARRIER: expose a register-relative store so it can occupy the return slot. */
    __asm__("" : "=r"(base) : "0"(base));
    base[(-0x4330) / 4] = 0;
    return previous;
}

void func_80098044(void) {
    RrInterruptRegisters *p;
    u32 mask;
    s32 *counters;
    {
        /* ASM_BARRIER: Keep the initial enable-state address in the assembler temporary. */
        register s32 *page __asm__("$1");
        /* ASM_BARRIER: Expose the initial state store in the critical-section call's delay slot. */
        __asm__ volatile("" : : : "$1", "memory");
        page=(s32 *)0x800B0000U;
        /* ASM_BARRIER: Preserve the PAL page-relative enable-state store. */
        __asm__("" : "+r"(page));
        page[-0x4360/4]=0;
    }
    func_80096F04();
    func_80097714(2,D_80114418);
    func_80097704(2,D_80114418);
    p=D_800ABCC8;
    p->pending=0xFFFFFFFEU;
    mask=p->mask;
    /* Ordinary final write view permits the original call-delay store. */
    *(u32 *)((u8 *)p+RR_FIELD_OFFSET(RrInterruptRegisters,mask))=mask|1;
    func_80096F94(3,0);
    func_80096F14();
    D_800ABC6C(D_800ABC9C);
    D_800ABC6C(D_800ABC9C+1);
    counters=D_80114428;
    counters[1]=0;counters[0]=0;
    D_800ABCA0=1;
    /* ASM_BARRIER: Restore saved registers only after publishing the enabled state. */
    __asm__ volatile("" : : : "$16", "$31", "memory");
}

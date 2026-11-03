/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "types.h"
#include "rr/channels.h"
#include "rr/interrupts.h"

extern s32 D_800ABCD0;
extern s32 D_80114428[2];
extern u32 D_80114418[4];
extern s32 func_80096F04(void),func_80096F94(s32,s32);
extern void func_80096F14(void);
extern s32 func_80097714(s32,void *),func_80097704(s32,void *);

s32 func_80098030(void);
void func_80098044(void);
void func_80098110();

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

void func_80098110() { func_80096F04(); func_80096F94(3, 1); func_80097714(2, &D_80114418); func_80096F14(); }

/* GCC_VERSION: 2.7.2 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/interrupts.h"

extern char *D_800ABDA8;
extern char D_800ABDB0[];
extern int func_80096F04(), func_80096F14(), func_80096F94(), func_80097704(), func_80097714();
extern int func_80096F04(), func_80096F94(), func_80097714(), func_80096F14();

void func_8009BA04(void);
void func_8009BA94();
s32 func_8009BAD8(void);
s32 func_8009BB18(void);

void func_8009BA04(void)
{
    RrInterruptRegisters *q;
    *(short *)(D_800ABDA8 + 0xA) = 0x40;
    *(short *)(D_800ABDA8 + 0xE) = 0x88;
    *(short *)(D_800ABDA8 + 8) = 0xD;
    func_80096F04();
    func_80097714(2, D_800ABDB0);
    func_80097704(2, D_800ABDB0);
    q = D_800ABDAC;
    q->pending = ~1U;
    *(u32 *)&q->mask = q->mask | 1;
    func_80096F94(3, 0);
    func_80096F14();
}

void func_8009BA94() { func_80096F04(); func_80096F94(3, 1); func_80097714(2, &D_800ABDB0); func_80096F14(); }

s32 func_8009BAD8(void) {
    RrInterruptRegisters *registers=D_800ABDAC;
    register s32 result;
    if(registers->mask&1) {
        if(registers->pending&1) result=1;
        else {
            result=0;
            /* ASM_BARRIER: Preserve the conditional false-result block instead of folding the predicate. */
            __asm__ volatile("" : : "r"(result));
        }
    } else result=0;
    return result;
}

s32 func_8009BB18(void) {
    RrInterruptRegisters *registers=D_800ABDAC;
    register s32 result;
    if(registers->mask&0x80) {
        if(registers->pending&0x80) result=1;
        else {
            result=0;
            /* ASM_BARRIER: Preserve the conditional false-result block instead of folding the predicate. */
            __asm__ volatile("" : : "r"(result));
        }
    } else result=0;
    return result;
}

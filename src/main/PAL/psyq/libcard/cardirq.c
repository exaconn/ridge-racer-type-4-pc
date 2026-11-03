/* GCC_VERSION: 2.7.2 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/interrupts.h"

extern char *D_800ABDA8;
extern int func_80096F04(), func_80096F14(), func_80096F94(), func_80097704(), func_80097714();
extern int func_80096F04(), func_80096F94(), func_80097714(), func_80096F14();
extern int D_800ABDB0;
extern int D_800ABDE8;
extern int D_800ABDE4;
extern char D_800ABDC0[];
extern int func_80097704();
extern int func_8009BC3C();

void func_8009BA04(void);
void func_8009BA94();
s32 func_8009BAD8(void);
s32 func_8009BB18(void);
int func_8009BB58();

/* legacy typing of shared globals in this part of the unit */
#define D_800ABDB0 (*(char (*)[])&D_800ABDB0)

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

#undef D_800ABDB0

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

/* legacy typing of shared globals in this part of the unit */
#define D_800ABDAC ((unsigned int *)D_800ABDAC)
#define D_800ABDA8 ((short *)D_800ABDA8)
#define func_80097714 ((int (*)())func_80097714)

int func_8009BB58() {
    volatile unsigned int *g;
    short *h;
    if (D_800ABDE8) {
        g = D_800ABDAC;
        D_800ABDE4 = 0x11;
        D_800ABDE8 = 0;
        g[0] = ~0x80;
        g[1] &= ~0x80;
        h = D_800ABDA8;
        h[5] = 0;
        func_80097714(1, D_800ABDC0);
        D_800ABDA8[5] = 0x40;
        D_800ABDA8[7] = 0x88;
        D_800ABDA8[4] = 0xD;
        D_800ABDA8[5] = 0;
        return 0;
    }
    if (D_800ABDE4 & 1) return 0;
    func_80097714(1, D_800ABDC0);
    func_80097704(1, D_800ABDC0);
    func_8009BC3C();
    return 0;
}

#undef D_800ABDAC
#undef D_800ABDA8
#undef func_80097714

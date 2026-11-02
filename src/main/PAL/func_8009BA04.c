/* CC1_FLAGS: -mno-split-addresses */
extern char *D_800ABDA8;
#include "rr/interrupts.h"
extern char D_800ABDB0[];
extern int func_80096F04(), func_80096F14(), func_80096F94(), func_80097704(), func_80097714();
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

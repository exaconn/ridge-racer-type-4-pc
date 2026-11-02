#include "rr_asm.h"
extern int func_800701C4();
extern int func_80091844();
extern int func_80090A24();
extern int func_8006EE3C();
extern int func_8005F0D0();
extern char *D_8010BF78;
extern int D_800F2FD8;
void func_8001BFA8(int a0, int a1, int a2)
{
    int t[8];
    func_800701C4(4);
    func_80091844(a1, 0x1F800084);
    RR_GTE_SETROT(D_8010BF78 + 0x20);
    func_80090A24(0x1F800084, 0x1F8000A4);
    RR_GTE_SETLLM(0x1F8000A4);
    RR_GTE_SETROT(0x1F800028);
    func_80090A24(0x1F800084, t);
    func_8006EE3C(0x1F80007C, a2, t);
    *(int *)0x1F800078 = 0;
    func_8005F0D0(0x1F800000, a0 < D_800F2FD8 ? a0 : 1, 0, 0);
}

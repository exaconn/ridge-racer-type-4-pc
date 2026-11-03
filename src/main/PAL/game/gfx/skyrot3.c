#include "types.h"
#include "rr_asm.h"
#include "m2c_macros.h"

extern int func_800701C4();
extern int func_80091844();
extern int func_80090A24();
extern int func_8006EE3C();
extern int func_8005F0D0();
extern char *D_8010BF78;
extern int D_800F2FD8;
extern int func_80092CB8();
extern s16 D_800AC5D0;
extern s32 D_800AC5DC;
extern s16 D_800F3A26;
extern s16 D_800F4C6A;
extern s16 D_800F4D8A;
extern s32 D_8010BF7C;

void func_8001BFA8(int a0, int a1, int a2);
void func_8001C0F8(void);

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

void func_8001C0F8(void) {
    D_800AC5DC = 0x80;
    D_800AC5D0 = 0;
    D_8010BF7C = 0;
    func_80092CB8(1);
    D_800F4C6A = 0;
    D_800F3A26 = 2;
    D_800F4D8A = 0;
}

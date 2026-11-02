#include "rr_asm.h"
extern char D_8009E248[];
extern int D_800AC64C;
extern int func_800194BC();

void func_800195BC(char *a0, volatile unsigned short *a1)
{
    volatile unsigned short *p = (unsigned short *)0x1F80007C;
    RR_GTE_SETROT(D_8009E248);
    RR_GTE_MV3_COL(a1);
    RR_GTE_ST3_COL(p);
    RR_GTE_MV3_COL(a1 + 1);
    RR_GTE_ST3_COL((unsigned short *)0x1F80007E);
    RR_GTE_MV3_COL(a1 + 2);
    RR_GTE_ST3_COL((unsigned short *)0x1F800080);
    RR_GTE_SETLLM(p);
    p[0] = a1[0];
    p[1] = a1[1];
    p[2] = a1[2];
    p[3] = a1[3] << 1;
    p[4] = a1[4] << 1;
    p[5] = a1[5] << 1;
    p[6] = a1[6];
    p[7] = a1[7];
    p[8] = a1[8];
    RR_GTE_SETROT(p);
    RR_GTE_SETTR(a0);
    func_800194BC(D_800AC64C, 0);
}

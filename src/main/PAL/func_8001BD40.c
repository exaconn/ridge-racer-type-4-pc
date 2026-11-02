#include "rr_asm.h"
extern int func_800701C4();
extern int func_80091844();
extern int func_80090A24();
extern int func_8006EE3C();
extern int func_8005F0D0();
extern char *D_8010BF78;
extern char D_8001013C[];
extern int D_800F2FD8;
int func_8001BD40(int a0, int a1)
{
    int t[8];
    short *s3 = (short *)0x1F80007C;
    int s2;
    func_800701C4(4);
    s2 = *(short *)(D_8010BF78 + (a0 << 1) + 0xAA);
    if (a1 || (s2 && (s2 & 0x3FF) > 0x20)) s2 += 0x20;
    else if (s2) s2 = 0;
    s2 &= 0xFFF;
    *(short *)(D_8010BF78 + (a0 << 1) + 0xAA) = s2;
    s3[0] = 0;
    s3[2] = 0;
    s3[1] = -s2;
    func_80091844(s3, (char *)s3 + 8);
    RR_GTE_SETROT(D_8010BF78);
    func_80090A24((char *)s3 + 8, (char *)s3 + 0x28);
    RR_GTE_SETLLM((char *)s3 + 0x28);
    RR_GTE_SETROT(0x1F800028);
    func_80090A24((char *)s3 + 8, t);
    func_8006EE3C(0x1F80007C, D_8001013C + a0 * 16, t);
    *(int *)0x1F800078 = 0;
    func_8005F0D0(0x1F800000, D_800F2FD8 < 1, 0, 0);
    return s2;
}

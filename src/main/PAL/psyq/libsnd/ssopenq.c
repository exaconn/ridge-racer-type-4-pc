/* MASPSX_FLAGS: --assembler-profile=gnu */
/* CC1_FLAGS: -mno-split-addresses */
#include "psyq/libsnd/ssopenq.h"
extern int func_8001E84C(), func_80081EB4();
extern char D_800116E8[];
extern int D_8010EFC8[];
extern int D_8010F060;
extern int func_80080034(), func_800800B4(), func_80080434(), func_80080504(), func_800805D4(), func_800806C4(), func_80080774(), func_80080804(), func_80080904(), func_80080A44(), func_80080AB4(), func_80080B24(), func_80080BE4(), func_80080C74(), func_80080D34(), func_80080DC4(), func_80080E54(), func_80081004(), func_800810C4(), func_80081174(), func_80081224(), func_800812D4(), func_80081394(), func_80081444(), func_800814F4(), func_800815D4(), func_80081674(), func_80081714(), func_80081744(), func_80081774(), func_800817A4(), func_800817D4(), func_80081804(), func_800818B4(), func_80081AE4(), func_80081CB4(), func_80081D94();
short func_8007FD94(a0, a1) int a0; int a1; {
    /* ASM_BARRIER: pin t0 to match original register allocation */
    register int t0 asm("t0");
    /* ASM_BARRIER: pin v1 to match original register allocation */
    register int v1 asm("v1");
    /* ASM_BARRIER: pin a3 to match original register allocation */
    register int a3 asm("a3");
    int a;
    /* ASM_BARRIER: pin x to a1 to match original register allocation */
    register int x asm("a1");
    int bit;
    /* ASM_BARRIER: pin tab to v1 to match original register allocation */
    register int *tab asm("v1");
    /* ASM_BARRIER: pin s0 to match original register allocation */
    register int s0 asm("s0");
    /* ASM_BARRIER: pin v0 to match original register allocation */
    register int v0 asm("v0");
    /* ASM_BARRIER: pin t1 to match original register allocation */
    register int t1 asm("t1");
    /* ASM_BARRIER: pin a2 to match original register allocation */
    register int a2 asm("a2");
    t0 = 0;
    /* ASM_BARRIER: force t0 = 0 first */
    __asm__("" : "+r"(t0));
    a2 = a0;
    /* ASM_BARRIER: force a2 = a0 before D_8010F060 load */
    __asm__("" : "+r"(a2));
    x = a1;
    a3 = D_8010F060;
    if (a3 == -1) {
    err:
        func_8001E84C(D_800116E8, x, a2, a3);
        return -1;
    } else {
        v1 = 0;
        /* ASM_BARRIER: prevent CSE from copying v1=0 into a */
        __asm__ volatile("" : "+r"(v1));
        a = 0;
        /* ASM_BARRIER: force addu a0, zero, zero */
        __asm__ volatile("" : "+r"(a));
        t1 = 1;
        /* ASM_BARRIER: prevent CSE from seeing t1 == 1 */
        __asm__ volatile("" : "+r"(t1));
        for (;;) {
            /* ASM_BARRIER: prevent stealing first loop insn into back-edge delay slot */
            __asm__ volatile("");
            bit = t1 << v1;
            if (!(a3 & bit)) {
                t0 = v1;
                a = 1;
                /* ASM_BARRIER: force addiu a0, zero, 1 */
                __asm__ volatile("" : "+r"(a));
            }
            v1++;
            if (v1 >= 0x20) goto err;
            /* ASM_BARRIER: force addu v0, a0, zero in delay slot */
            __asm__ volatile("" : "=r"(v0) : "0"(a));
            if (v0 != 0) {
                v0 = t0 << 16;
                /* ASM_BARRIER: force sll v0, t0, 16 */
                __asm__ volatile("" : "+r"(v0));
                s0 = v0 >> 16;
                /* ASM_BARRIER: force addu a0, s0, zero before 1 << s0 */
                __asm__ volatile("" : "=r"(a) : "0"(s0));
                /* ASM_BARRIER: force addiu v0, zero, 1 instead of reusing t1 */
                __asm__ volatile("" : "=r"(v0) : "0"(1));
                v0 <<= s0;
                /* ASM_BARRIER: force sllv v0, v0, s0 before sll a1, a1, 16 */
                __asm__ volatile("" : "+r"(v0));
                x <<= 16;
                /* ASM_BARRIER: force sll a1, a1, 16 before D_8010F060 load */
                __asm__ volatile("" : "+r"(x));
                v1 = D_8010F060;
                /* ASM_BARRIER: load D_8010F060 into v1 */
                __asm__ volatile("" : "+r"(v1));
                v1 |= v0;
                /* ASM_BARRIER: force or v1, v1, v0 */
                __asm__ volatile("" : "+r"(v1));
                D_8010F060 = v1;
                x = func_80081EB4(a, x >> 16, a2, a3) << 16;
                /* ASM_BARRIER: keep x in a1 and reserve v0 across tab setup */
                __asm__ volatile("" : "+r"(x), "=r"(v0));
                tab = D_8010EFC8;
                /* ASM_BARRIER: force lui v1, addiu v1 */
                __asm__ volatile("" : "+r"(tab));
                tab[0] = (int)func_80081CB4;
                tab[1] = (int)func_80081D94;
                tab[3] = (int)func_80081AE4;
                tab[2] = (int)func_80081804;
                tab[4] = (int)func_800818B4;
                tab[5] = (int)func_80080034;
                tab[7] = (int)func_80080434;
                tab[8] = (int)func_80080504;
                tab[9] = (int)func_800805D4;
                tab[10] = (int)func_800806C4;
                tab[11] = (int)func_80080804;
                tab[12] = (int)func_80080904;
                tab[13] = (int)func_80080A44;
                tab[14] = (int)func_80080AB4;
                tab[15] = (int)func_80080774;
                tab[16] = (int)func_80080B24;
                tab[6] = (int)func_800800B4;
                tab[17] = (int)func_80080BE4;
                tab[18] = (int)func_80080C74;
                tab[19] = (int)func_80080D34;
                tab[20] = (int)func_80080DC4;
                tab[21] = (int)func_80080E54;
                tab[22] = (int)func_80081004;
                tab[23] = (int)func_800810C4;
                tab[24] = (int)func_80081174;
                tab[25] = (int)func_80081224;
                tab[26] = (int)func_800812D4;
                tab[27] = (int)func_80081394;
                tab[28] = (int)func_80081444;
                tab[29] = (int)func_800814F4;
                tab[30] = (int)func_800815D4;
                tab[31] = (int)func_80081674;
                tab[32] = (int)func_80081714;
                tab[33] = (int)func_80081744;
                tab[34] = (int)func_80081774;
                tab[35] = (int)func_800817A4;
                a = (int)func_800817D4;
                /* ASM_BARRIER: materialize func_800817D4 in a0 before shifting x */
                __asm__ volatile("" : "+r"(a));
                x >>= 16;
                /* ASM_BARRIER: shift x before final table store */
                __asm__ volatile("" : "+r"(x));
                tab[36] = a;
                /* ASM_BARRIER: end reservation of v0 */
                __asm__ volatile("" : : "r"(v0));
                v1 = -1;
                /* ASM_BARRIER: keep -1 in v1 for comparison */
                __asm__("" : "+r"(v1));
                if (x != v1) return s0;
                return -1;
            }
        }
    }
}

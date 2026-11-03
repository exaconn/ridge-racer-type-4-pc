#include "rr_asm.h"

extern int func_8001AC24(), func_80092334();
extern int D_800FF690[], D_800FF6A4[];

void func_8001ADA0(a0, a1) int a0; int a1;
{
    int *m = (int *)0x1F80007C;
    int *g;
    /* ASM_BARRIER: keep v pointer in $v0 */
    register int *v __asm__("$2");
    /* ASM_BARRIER: keep a1*12 offset and v[3]/v[1] temp in $v1 */
    register int v1 __asm__("$3");
    /* ASM_BARRIER: keep 0x1F800000 / 0x1F80007C arg in $a1 */
    register int r_a1 __asm__("$5");
    char *a3;
    int s2, s4, s5, a2, cnt;
    char *s0;
    /* ASM_BARRIER: keep GTE table pointer in $t0 */
    register int *p__r8 __asm__("$8");
    int g20, g1, pt1, pt2;

    v1 = a1 * 3;
    r_a1 = 0x1F800000;
    /* ASM_BARRIER: load 0x1F800000 into $a1 after a1*3 before shifting by 2 */
    __asm__("" : "+r"(r_a1), "+r"(v1));
    g = (int *)r_a1;
    /* ASM_BARRIER: copy $a1 to $s3 before shifting v1 by 2 */
    __asm__("" : "+r"(g), "+r"(v1));
    v1 <<= 2;
    v = (int *)g[0x48 / 4];
    g20 = g[0x20 / 4];
    /* ASM_BARRIER: load g[0x20/4] in delay slot of g[0x48/4] before subtracting 4 */
    __asm__("" : "+r"(v), "+r"(g20));
    v = (int *)((char *)v - 4);
    /* ASM_BARRIER: subtract 4 before adding v1 */
    __asm__("" : "+r"(v));
    v = (int *)((char *)v + v1);
    /* ASM_BARRIER: keep r_a1 |= 0x7C after v addition so it fills v[3] load delay slot */
    __asm__ volatile("" : "+r"(r_a1), "+r"(v));
    v1 = v[3];
    r_a1 |= 0x7C;
    m[0x30 / 4] = v1;
    v1 = v[1];
    m[0] = 0x1000;
    m[0x10 / 4] = 0x1000;
    m[0x8 / 4] = 0x1000;
    m[0xC / 4] = 0;
    m[1] = 0;
    m[0x2C / 4] = v1;
    func_80092334(g20, r_a1);
    RR_GTE_SETROT(m);
    p__r8 = D_800FF690;
    RR_GTE_SETLLM(p__r8);
    p__r8 = D_800FF6A4;
    RR_GTE_SETBK_P(p__r8);
    {
        /* ASM_BARRIER: keep a0*12 in $v0 */
        register int off0 __asm__("$2") = a0 * 12;
        s4 = 0x12A;
        a3 = *(char **)(g[0x48 / 4] + off0 + 8);
        /* ASM_BARRIER: keep off0 ($v0) live so g[0x48/4] + off0 writes into $v1 */
        __asm__("" : : "r"(off0));
    }
    s2 = *(unsigned short *)(a3 + 2);
    a3 += 4;
    if (g[0x64 / 4]) s4 = 0x95;
    cnt = s2;
    s2--;
    /* ASM_BARRIER: keep cnt = s2 and s2-- at top of .L8001AEC8 before preheader loads */
    __asm__ volatile("");
    a2 = g[0];
    g1 = g[1];
    /* ASM_BARRIER: order g[0]/g[1] loads before a3 loads */
    __asm__ volatile("" : : "r"(a2), "r"(g1));
    pt1 = *(int *)a3;
    pt2 = *(unsigned short *)(a3 + 4);
    m[0x24 / 4] = pt1;
    *(short *)((char *)m + 0x28) = pt2;
    /* ASM_BARRIER: order m+0x24/0x28 stores before g[0x60/4] load and s5 addiu in branch delay slot */
    __asm__ volatile("");
    g = (int *)g[0x60 / 4];
    s5 = g1 + 0x200;
    if (cnt > 0) {
        s0 = a3 + 7;
        do {
            int t1, t2, t, sx;
            /* ASM_BARRIER: prevent loop strength reduction from creating a second pointer GIV for s0+1/s0+5 */
            __asm__("" : "+r"(s0));
            /* ASM_BARRIER: load V0 from m+0x24 and run GTE MVMVA (0x4A4A2012) */
            __asm__ volatile ("lwc2 $0, 0(%0); lwc2 $1, 4(%0)" : : "r"((char *)m + 0x24));
            /* ASM_BARRIER: two delay slot nops before GTE MVMVA */
            __asm__ volatile ("nop; nop");
            /* ASM_BARRIER: GTE MVMVA 1, 1, 0, 1, 0 (0x4A4A2012) */
            __asm__ volatile (".word 0x4A4A2012");
            t1 = *(int *)(s0 + 1);
            t2 = *(unsigned short *)(s0 + 5);
            m[0x24 / 4] = t1;
            *(short *)((char *)m + 0x28) = t2;
            RR_GTE_SWC2_IR3((char *)m + 0x54);
            t = m[0x54 / 4];
            sx = t >> (int)g;
            if (t > 0) {
                if (sx < s4) {
                    RR_GTE_TR_FROM_IR();
                    a2 = func_8001AC24(m, s5 + sx * 4 + *(signed char *)s0 * 4, a2);
                }
            }
            s0 += 8;
        } while (s2-- > 0);
    }
    *(int *)0x1F800000 = a2;
}

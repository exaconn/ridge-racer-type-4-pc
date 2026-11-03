#include "types.h"
#include "rr_asm.h"

extern char D_8009E124[];

char *func_80015D00(char *a0_in, s32 a1_in, char *a2_in) {
    /* ASM_BARRIER: PsyQ-style register allocation */
    register s32 v0 __asm__("$2");
    /* ASM_BARRIER: PsyQ-style register allocation */
    register s32 v1 __asm__("$3");
    /* ASM_BARRIER: PsyQ-style register allocation */
    register s32 a0 __asm__("$4");
    /* ASM_BARRIER: PsyQ-style register allocation */
    register s32 a1 __asm__("$5") = a1_in;
    /* ASM_BARRIER: PsyQ-style register allocation */
    register char *a2 __asm__("$6") = a2_in;
    /* ASM_BARRIER: PsyQ-style register allocation */
    register char *a3 __asm__("$7");
    /* ASM_BARRIER: PsyQ-style register allocation */
    register char *t0 __asm__("$8");
    /* ASM_BARRIER: PsyQ-style register allocation */
    register s32 t1 __asm__("$9");
    /* ASM_BARRIER: PsyQ-style register allocation */
    register char *t2 __asm__("$10");
    /* ASM_BARRIER: PsyQ-style register allocation */
    register s32 t3 __asm__("$11");
    /* ASM_BARRIER: PsyQ-style register allocation */
    register s32 t4 __asm__("$12");
    /* ASM_BARRIER: PsyQ-style register allocation */
    register s32 t5 __asm__("$13");
    /* ASM_BARRIER: PsyQ-style register allocation */
    register s32 t6 __asm__("$14");
    /* ASM_BARRIER: PsyQ-style register allocation */
    register s32 t7 __asm__("$15");
    /* ASM_BARRIER: PsyQ-style register allocation */
    register char *t8 __asm__("$24");
    /* ASM_BARRIER: PsyQ-style register allocation */
    register s32 t9 __asm__("$25");
    s32 s0;

    v0 = *(s32 *)(a2 + 0x98);
    /* ASM_BARRIER: load-use delay on *(s32 *)(a2 + 0x98) */
    __asm__ volatile("" : "+r"(v0));
    v1 = -v0;
    t8 = a0_in;
    if (v1 < 0) {
        /* ASM_BARRIER: place t8 = a0 in bgez delay slot */
        __asm__ volatile("" : "+r"(v1), "+r"(t8));
        v1 += 0x3F;
    }

    v0 = *(s32 *)(a2 + 0x9C);
    /* ASM_BARRIER: load-use delay on *(s32 *)(a2 + 0x9C) */
    __asm__ volatile("" : "+r"(v0));
    a0 = -v0;
    t5 = v1 >> 6;
    if (a0 < 0) {
        /* ASM_BARRIER: place t5 = v1 >> 6 in bgez delay slot */
        __asm__ volatile("" : "+r"(a0), "+r"(t5));
        a0 += 0x3F;
    }

    t2 = a2 + 0x1A0;
    t1 = a1;
    v0 = 0x1F800000;
    /* ASM_BARRIER: order pre-loop pointer/count setup */
    __asm__ volatile("" : "+r"(t2), "+r"(t1), "+r"(v0));
    v0 = *(s32 *)(v0 + 0x60);
    t4 = a0 >> 6;
    /* ASM_BARRIER: order scratchpad load and t4 shift */
    __asm__ volatile("" : "+r"(v0), "+r"(t4));
    t3 = v0 + 2;
    if (a1 > 0) {
        /* ASM_BARRIER: place t3 = v0 + 2 in blez delay slot */
        __asm__ volatile("" : "+r"(t3));
        t0 = a2 + 0x70;
        a3 = a2 + 0x1B0;
        /* ASM_BARRIER: order t0 and a3 loop pointers */
        __asm__ volatile("" : "+r"(t0), "+r"(a3));
        do {
            /* ASM_BARRIER: GTE RTPS on vertex at t8 */
            RR_GTE_RTPS_LDV(t8);
            /* ASM_BARRIER: store SZ3 to 0(t0) */
            __asm__ volatile("swc2 $19, 0(%0)" : : "r"(t0) : "memory");
            v0 = *(s32 *)(a2 + 0x70);
            /* ASM_BARRIER: load-use delay on *(s32 *)(a2 + 0x70) */
            __asm__ volatile("" : "+r"(v0));
            a0 = v0 >> t3;
            if (v0 <= 0)
                goto L_zero;
            /* ASM_BARRIER: keep srav a0 in blez delay slot */
            __asm__ volatile("" : "+r"(a0));
            v0 = a0 < 0x1C0;
            /* ASM_BARRIER: materialize slti into v0 */
            __asm__ volatile("" : "+r"(v0));
            if (v0 != 0)
                goto L_valid;
L_zero:
            /* ASM_BARRIER: keep srav a0 live on L_zero path for blez delay slot */
            __asm__ volatile("" : "+r"(a0));
            *(s32 *)a3 = 0;
            goto L_next;
L_valid:
            /* ASM_BARRIER: store GTE IR1..IR3 to t2 */
            RR_GTE_STORE_IR(t2);
            v0 = (s32)(t2 + 0xC);
            /* ASM_BARRIER: store SXY2 to t2+0xC */
            RR_GTE_SWC2_SXY2(v0);
            v0 = *(s32 *)(t2 + 0x0);
            /* ASM_BARRIER: load-use delay on *(s32 *)(t2 + 0x0) */
            __asm__ volatile("" : "+r"(v0));
            v0 += t5;
            *(s32 *)(t2 + 0x0) = v0;
            /* ASM_BARRIER: order t2[0] update before a3[-3] update */
            __asm__ volatile("" : "+r"(a0) : : "memory");
            v0 = *(s32 *)(a3 - 0xC);
            a0 <<= 2;
            v0 += t4;
            *(s32 *)(a3 - 0xC) = v0;
            /* ASM_BARRIER: order a3[-3] store before OT pointer calculation */
            __asm__ volatile("" : "+r"(a0) : : "memory");
            v1 = *(s32 *)(a2 + 0x6C);
            v0 = *(s8 *)(t8 + 7);
            /* ASM_BARRIER: order v1 and v0 loads */
            __asm__ volatile("" : "+r"(v1), "+r"(v0));
            v1 += a0;
            v0 <<= 2;
            /* ASM_BARRIER: order v1 add and v0 shift */
            __asm__ volatile("" : "+r"(v1), "+r"(v0));
            v1 += v0;
            *(s32 *)a3 = v1;
L_next:
            t1 -= 1;
            a3 += 0x14;
            t2 += 0x14;
            /* ASM_BARRIER: order loop increments before t8 += 8 in bgtz delay slot */
            __asm__ volatile("" : "+r"(t1), "+r"(a3), "+r"(t2));
            t8 += 8;
        } while (t1 > 0);
    }

    v0 = 0x1000;
    /* ASM_BARRIER: materialize 0x1000 in v0 */
    __asm__ volatile("" : "+r"(v0));
    *(s32 *)(a2 + 0x58) = v0;
    *(s32 *)(a2 + 0x50) = v0;
    *(s32 *)(a2 + 0x48) = v0;
    /* ASM_BARRIER: order diagonal stores before v0 = a2 + 0x48 */
    __asm__ volatile("" : : : "memory");
    v0 = (s32)(a2 + 0x48);
    /* ASM_BARRIER: compute v0 = a2 + 0x48 before zeroing off-diagonals */
    __asm__ volatile("" : "+r"(v0));
    *(s32 *)(a2 + 0x54) = 0;
    *(s32 *)(a2 + 0x4C) = 0;
    /* ASM_BARRIER: load identity rotation matrix into GTE */
    RR_GTE_SETROT(v0);

    s0 = 0xE1000220;
    a0 = 0x1F800000;
    /* ASM_BARRIER: order s0 and a0 setup */
    __asm__ volatile("" : "+r"(s0), "+r"(a0));
    a0 = *(s32 *)a0;
    /* ASM_BARRIER: order scratchpad load before blez a1 */
    __asm__ volatile("" : "+r"(a0));
    t2 = a2 + 0x1A0;
    if (a1 > 0) {
        /* ASM_BARRIER: place t2 = a2 + 0x1A0 in blez delay slot */
        __asm__ volatile("" : "+r"(t2));
        do {
            t9 = 6;
            t3 = (s32)(a2 + 0x1AC);
            /* ASM_BARRIER: keep t9 and t3 setup before loop label */
            __asm__ volatile("" : "+r"(t9), "+r"(t3));
            do {
                a3 = *(char **)(t3 + 4);
                /* ASM_BARRIER: load-use delay on *(char **)(t3 + 4) */
                __asm__ volatile("" : "+r"(a3));
                if (a3 != 0) {
                    /* ASM_BARRIER: prevent delay-slot filling into beqz */
                    __asm__ volatile("" : : : "memory");
                    t5 = (s32)D_8009E124;
                    /* ASM_BARRIER: load translation vector from t2 into GTE */
                    RR_GTE_SETTR(t2);
                    t0 = D_8009E124;
                    /* ASM_BARRIER: compute t0 = D_8009E124 before lw t5, 0(t3) */
                    __asm__ volatile("" : "+r"(t0));
                    t5 = *(s32 *)(t3 + 0);
                    /* ASM_BARRIER: order lw t5 before GTE RTPS */
                    __asm__ volatile("" : "+r"(t5));
                    /* ASM_BARRIER: GTE RTPS on first offset at t0 */
                    RR_GTE_RTPS_LDV(t0);
                    /* ASM_BARRIER: read SXY2 into t7 */
                    __asm__ volatile("mfc2 %0, $14" : "=r"(t7));
                    t1 = 8;
                    v1 = a0 + 0x18;
                    t4 = *(s32 *)(a2 + 0x160);
                    /* ASM_BARRIER: order inner loop setup */
                    __asm__ volatile("" : "+r"(t1), "+r"(v1), "+r"(t4));
                    do {
                        t0 += 8;
                        /* ASM_BARRIER: advance t0 before GTE RTPS */
                        __asm__ volatile("" : "+r"(t0));
                        /* ASM_BARRIER: GTE RTPS on next offset at t0 */
                        RR_GTE_RTPS_LDV(t0);
                        *(s32 *)(v1 - 0x10) = t5;
                        *(s32 *)(v1 - 0x8) = t7;
                        *(s32 *)(v1 - 0x14) = t4;
                        *(s32 *)(v1 - 0x4) = 0;
                        *(s32 *)(v1 - 0xC) = 0;
                        /* ASM_BARRIER: order packet stores before OT link load */
                        __asm__ volatile("" : : : "memory");
                        v0 = *(s32 *)a3;
                        /* ASM_BARRIER: load-use delay on *(s32 *)a3 */
                        __asm__ volatile("" : "+r"(v0));
                        *(s32 *)a0 = v0;
                        *(s8 *)(v1 - 0x15) = t9;
                        *(s32 *)a3 = a0;
                        *(s8 *)(a3 + 3) = 0;
                        /* ASM_BARRIER: read SXY2 into t7 with nop delay */
                        __asm__ volatile("mfc2 %0, $14; nop" : "=r"(t7) : : "memory");
                        *(s32 *)v1 = t7;
                        v1 += 0x1C;
                        t1 -= 1;
                        /* ASM_BARRIER: order v1 and t1 updates before bgtz */
                        __asm__ volatile("" : "+r"(v1), "+r"(t1) : : "memory");
                        a0 += 0x1C;
                    } while (t1 > 0);
                    /* ASM_BARRIER: keep a0 increment in bgtz delay slot */
                    __asm__ volatile("" : "+r"(a0));
                    *(s32 *)(a0 + 4) = s0;
                    /* ASM_BARRIER: order s0 store before OT link load */
                    __asm__ volatile("" : : : "memory");
                    v0 = *(s32 *)a3;
                    /* ASM_BARRIER: load-use delay on *(s32 *)a3 */
                    __asm__ volatile("" : "+r"(v0));
                    *(s32 *)a0 = v0;
                    v0 = 1;
                    /* ASM_BARRIER: materialize 1 in v0 */
                    __asm__ volatile("" : "+r"(v0) : : "memory");
                    *(s8 *)(a0 + 3) = v0;
                    *(s32 *)a3 = a0;
                    a0 += 8;
                    /* ASM_BARRIER: advance a0 before final sb zero, 3(a3) */
                    __asm__ volatile("" : "+r"(a0) : : "memory");
                    *(s8 *)(a3 + 3) = 0;
                    /* ASM_BARRIER: keep sb before loop tail */
                    __asm__ volatile("" : : : "memory");
                }
                a1 -= 1;
                t3 += 0x14;
                /* ASM_BARRIER: order a1 and t3 updates before bgtz */
                __asm__ volatile("" : "+r"(a1), "+r"(t3));
                t2 += 0x14;
            } while (a1 > 0);
        } while (a1 > 0);
        /* ASM_BARRIER: place t2 += 0x14 in bgtz delay slot */
        __asm__ volatile("" : "+r"(t2));
    }

    *(s32 *)0x1F800000 = a0;
    return t8;
}

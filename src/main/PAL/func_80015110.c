#include "types.h"
#include "rr_asm.h"

void func_80015110(s32 *a0_in, char *a1_in) {
    /* ASM_BARRIER: PsyQ-style register allocation */
    register s32 v0 __asm__("$2");
    /* ASM_BARRIER: PsyQ-style register allocation */
    register s32 v1 __asm__("$3");
    /* ASM_BARRIER: PsyQ-style register allocation */
    register s32 *a0 __asm__("$4") = a0_in;
    /* ASM_BARRIER: PsyQ-style register allocation */
    register char *a1 __asm__("$5") = a1_in;
    /* ASM_BARRIER: PsyQ-style register allocation */
    register char *a2 __asm__("$6");
    /* ASM_BARRIER: PsyQ-style register allocation */
    register s32 a3 __asm__("$7");
    /* ASM_BARRIER: PsyQ-style register allocation */
    register char *t0 __asm__("$8");
    /* ASM_BARRIER: PsyQ-style register allocation */
    register char *t1 __asm__("$9");
    /* ASM_BARRIER: PsyQ-style register allocation */
    register s32 t2 __asm__("$10");
    /* ASM_BARRIER: PsyQ-style register allocation */
    register char *t3 __asm__("$11");
    /* ASM_BARRIER: PsyQ-style register allocation */
    register char *t4 __asm__("$12");
    /* ASM_BARRIER: PsyQ-style register allocation */
    register s32 t5 __asm__("$13");
    /* ASM_BARRIER: PsyQ-style register allocation */
    register s32 t6 __asm__("$14");
    /* ASM_BARRIER: PsyQ-style register allocation */
    register s32 t7 __asm__("$15");
    /* ASM_BARRIER: PsyQ-style register allocation */
    register s32 t8 __asm__("$24");
    /* ASM_BARRIER: PsyQ-style register allocation */
    register s32 t9 __asm__("$25");

    a2 = (char *)a0[0];
    /* ASM_BARRIER: schedule lw a2, 0(a0) before addu t0, a1, zero */
    __asm__ volatile("" : "+r"(a2), "+r"(a1));
    t0 = a1;
    /* ASM_BARRIER: order a2 and t0 initialization */
    __asm__ volatile("" : "+r"(a2), "+r"(t0));

    v0 = *(s32 *)(a2 + 0x0);
    v1 = *(s32 *)(a2 + 0x4);
    /* ASM_BARRIER: order first pair load before t1 setup */
    __asm__ volatile("" : "+r"(v0), "+r"(v1), "+r"(t0));
    t1 = t0 + 0x188;
    /* ASM_BARRIER: order t1 setup before stores */
    __asm__ volatile("" : "+r"(v0), "+r"(v1), "+r"(t1));
    *(s32 *)(t0 + 0xE0) = v0;
    *(s32 *)(t0 + 0xE4) = v1;
    /* ASM_BARRIER: order first pair stores before second pair load */
    __asm__ volatile("" : "+r"(a2), "+r"(t0) : "r"(v0), "r"(v1) : "memory");

    v0 = *(s32 *)(a2 + 0x8);
    v1 = *(s32 *)(a2 + 0xC);
    /* ASM_BARRIER: order second pair load before t2 setup */
    __asm__ volatile("" : "+r"(v0), "+r"(v1), "+r"(t0));
    t2 = (s32)(t0 + 0xE0);
    /* ASM_BARRIER: order t2 setup before stores */
    __asm__ volatile("" : "+r"(v0), "+r"(v1), "+r"(t2));
    *(s32 *)(t0 + 0xE8) = v0;
    *(s32 *)(t0 + 0xEC) = v1;
    /* ASM_BARRIER: order second pair stores before third pair load */
    __asm__ volatile("" : "+r"(a2), "+r"(t0) : "r"(v0), "r"(v1) : "memory");

    v0 = *(s32 *)(a2 + 0x10);
    v1 = *(s32 *)(a2 + 0x14);
    /* ASM_BARRIER: order third pair load before a1 setup */
    __asm__ volatile("" : "+r"(v0), "+r"(v1), "+r"(a2));
    a1 = a2 + 0x14;
    /* ASM_BARRIER: order a1 setup before stores */
    __asm__ volatile("" : "+r"(v0), "+r"(v1), "+r"(a1));
    *(s32 *)(t0 + 0xF0) = v0;
    *(s32 *)(t0 + 0xF4) = v1;

    a3 = a0[1];
    /* ASM_BARRIER: order a3 load before loop entry */
    __asm__ volatile("" : "+r"(a3));

    do {
        RR_GTE14_LWC2_V012((char *)t2);
        a1 += 0x18;
        a2 += 0x18;
        a3 -= 3;
        t1 += 0x18;
        /* ASM_BARRIER: order pointer increments before RTPT */
        __asm__ volatile("" : "+r"(a1), "+r"(a2), "+r"(a3), "+r"(t1));
        RR_GTE14_NOP2();
        RR_GTE14_RTPT();

        v0 = *(s32 *)(a2 + 0x0);
        v1 = *(s32 *)(a1 - 0x10);
        /* ASM_BARRIER: order pair 1 load before store */
        __asm__ volatile("" : "+r"(v0), "+r"(v1));
        *(s32 *)(t0 + 0xE0) = v0;
        *(s32 *)(t0 + 0xE4) = v1;

        v0 = *(s32 *)(a1 - 0xC);
        v1 = *(s32 *)(a1 - 0x8);
        /* ASM_BARRIER: order pair 2 load before store */
        __asm__ volatile("" : "+r"(v0), "+r"(v1));
        *(s32 *)(t0 + 0xE8) = v0;
        *(s32 *)(t0 + 0xEC) = v1;

        v0 = *(s32 *)(a1 - 0x4);
        v1 = *(s32 *)(a1 + 0x0);
        /* ASM_BARRIER: order pair 3 load before store */
        __asm__ volatile("" : "+r"(v0), "+r"(v1));
        *(s32 *)(t0 + 0xF0) = v0;
        *(s32 *)(t0 + 0xF4) = v1;

        /* ASM_BARRIER: store SXY0..SXY2 and SZ1..SZ3 from GTE */
        __asm__ volatile("swc2 $12, 0(%0); swc2 $17, 4(%0); swc2 $13, 8(%0); swc2 $18, 12(%0); swc2 $14, 16(%0); swc2 $19, 20(%0)" : : "r"(t1) : "memory");
    } while (a3 > 0);

    t6 = 0xE1000220;
    /* ASM_BARRIER: keep t6 initialization immediately after loop so lui fills bgtz delay slot */
    __asm__ volatile("" : "+r"(t6));

    v0 = a0[2];
    a0 = (s32 *)0x1F800000;
    /* ASM_BARRIER: order v0 load and a0 lui before t3/t2 setup */
    __asm__ volatile("" : "+r"(v0), "+r"(a0));
    t3 = (char *)v0 + 4;
    t2 = *(u16 *)((char *)v0 + 2);
    /* ASM_BARRIER: order t3 and t2 before 0x1F800060 load */
    __asm__ volatile("" : "+r"(t3), "+r"(t2), "+r"(a0));
    v0 = a0[0x60 / 4];
    v1 = *(s32 *)(t0 + 0xA0);
    /* ASM_BARRIER: order v0 and v1 loads before blez */
    __asm__ volatile("" : "+r"(v0), "+r"(v1));
    t5 = v0 + 4;
    if (v1 > 0) {
        /* ASM_BARRIER: keep t5 in blez delay slot */
        __asm__ volatile("" : "+r"(t5), "+r"(a0));
        v0 = a0[1];
        t4 = (char *)v0 + 0x1C4;
    } else {
        /* ASM_BARRIER: keep t5 in blez delay slot */
        __asm__ volatile("" : "+r"(t5), "+r"(a0));
        v0 = a0[1];
        /* ASM_BARRIER: load-use delay on a0[1] in else branch */
        __asm__ volatile("" : "+r"(v0));
        t4 = (char *)v0 + 0x1B0;
    }
    /* ASM_BARRIER: join after t4 selection */
    __asm__ volatile("" : "+r"(t4), "+r"(t5));

    t1 = (char *)0x1F800000;
    /* ASM_BARRIER: force lui $t1, 0x1F80 */
    __asm__ volatile("" : "+r"(t1));
    t1 = *(char **)t1;
    /* ASM_BARRIER: order t1 load before loop count check */
    __asm__ volatile("" : "+r"(t1), "+r"(t2));

    v0 = t2;
    t2--;
    if (v0 > 0) {
        /* ASM_BARRIER: keep t2-- in blez delay slot */
        __asm__ volatile("" : "+r"(t2), "+r"(t3), "+r"(t1));
        a3 = (s32)(t3 + 6);
        a2 = t1 + 4;
        /* ASM_BARRIER: order loop pointer setup before loop body */
        __asm__ volatile("" : "+r"(a3), "+r"(a2));
        do {
            v1 = *(u16 *)(t3 + 0);
            v0 = *(u16 *)((char *)a3 - 4);
            a1 = (char *)(s32)*(u16 *)((char *)a3 - 2);
            /* ASM_BARRIER: order 3 vertex index loads before shifts */
            __asm__ volatile("" : "+r"(v1), "+r"(v0), "+r"(a1));
            v1 = (s32)(t0 + (v1 << 3));
            v0 = (s32)(t0 + (v0 << 3));
            a1 = t0 + ((s32)a1 << 3);
            /* ASM_BARRIER: order 3 vertex pointers before SXY loads */
            __asm__ volatile("" : "+r"(v1), "+r"(v0), "+r"(a1));
            t7 = *(s32 *)((char *)v1 + 0x1A0);
            t8 = *(s32 *)((char *)v0 + 0x1A0);
            t9 = *(s32 *)(a1 + 0x1A0);
            /* ASM_BARRIER: load SXY0..SXY2 and run NCLIP */
            __asm__ volatile("mtc2 %0, $12\n\tmtc2 %2, $14\n\tmtc2 %1, $13\n\tnop\n\tnop\n\t.word 0x4B400006" : : "r"(t7), "r"(t8), "r"(t9));

            v1 = *(s32 *)((char *)v1 + 0x1A4);
            a0 = (s32 *)*(s32 *)((char *)v0 + 0x1A4);
            v0 = *(u16 *)((char *)a3 + 0);
            /* ASM_BARRIER: order SZ1, SZ2, and index 3 loads */
            __asm__ volatile("" : "+r"(v1), "+r"(a0), "+r"(v0));
            v1 += (s32)a0;
            v0 <<= 3;
            v0 = (s32)(t0 + v0);
            /* ASM_BARRIER: order v0 pointer computation before SZ3, SZ4 loads */
            __asm__ volatile("" : "+r"(v1), "+r"(v0), "+r"(a1));
            a0 = (s32 *)*(s32 *)(a1 + 0x1A4);
            v0 = *(s32 *)((char *)v0 + 0x1A4);
            /* ASM_BARRIER: order SZ3 and SZ4 loads before additions */
            __asm__ volatile("" : "+r"(a0), "+r"(v0), "+r"(v1));
            v1 += (s32)a0;
            v1 += v0;
            if (v1 > 0) {
                v1 >>= t5;
                if (v1 < 0x1C0) {
                    RR_GTE14_SWC2_MAC0(t0 + 0x74);
                    v0 = *(s32 *)(t0 + 0x74);
                    if (v0 != 0) {
                        v1 <<= 2;
                        v1 += (s32)t4;
                        v0 = *(s32 *)v1;
                        /* ASM_BARRIER: load-use delay on OT entry */
                        __asm__ volatile("" : "+r"(v0), "+r"(v1));
                        *(s32 *)t1 = v0;
                        *(u8 *)(a2 - 1) = 9;
                        *(char **)v1 = t1;
                        *(u8 *)((char *)v1 + 3) = 0;

                        v0 = *(u16 *)((char *)a3 + 2);
                        /* ASM_BARRIER: load-use delay on color index 0 */
                        __asm__ volatile("" : "+r"(v0));
                        v0 = (s32)(t0 + (v0 << 2));
                        v0 = *(s32 *)((char *)v0 + 0x160);
                        /* ASM_BARRIER: load-use delay on color 0 */
                        __asm__ volatile("" : "+r"(v0));
                        *(s32 *)(a2 + 0x4) = v0;

                        v0 = *(u16 *)((char *)a3 + 4);
                        /* ASM_BARRIER: load-use delay on color index 1 */
                        __asm__ volatile("" : "+r"(v0));
                        v0 = (s32)(t0 + (v0 << 2));
                        v0 = *(s32 *)((char *)v0 + 0x160);
                        /* ASM_BARRIER: load-use delay on color 1 */
                        __asm__ volatile("" : "+r"(v0));
                        *(s32 *)(a2 + 0xC) = v0;

                        v0 = *(u16 *)((char *)a3 + 6);
                        /* ASM_BARRIER: load-use delay on color index 2 */
                        __asm__ volatile("" : "+r"(v0));
                        v0 = (s32)(t0 + (v0 << 2));
                        v0 = *(s32 *)((char *)v0 + 0x160);
                        /* ASM_BARRIER: load-use delay on color 2 */
                        __asm__ volatile("" : "+r"(v0));
                        *(s32 *)(a2 + 0x14) = v0;

                        v0 = *(u16 *)((char *)a3 + 8);
                        /* ASM_BARRIER: load-use delay on color index 3 */
                        __asm__ volatile("" : "+r"(v0));
                        v0 = (s32)(t0 + (v0 << 2));
                        v0 = *(s32 *)((char *)v0 + 0x160);
                        /* ASM_BARRIER: load-use delay on color 3 */
                        __asm__ volatile("" : "+r"(v0));
                        *(s32 *)(a2 + 0x1C) = v0;

                        v0 = *(u16 *)(t3 + 0);
                        /* ASM_BARRIER: load-use delay on vertex index 0 */
                        __asm__ volatile("" : "+r"(v0));
                        v0 = (s32)(t0 + (v0 << 3));
                        v0 = *(s32 *)((char *)v0 + 0x1A0);
                        /* ASM_BARRIER: load-use delay on SXY0 */
                        __asm__ volatile("" : "+r"(v0));
                        *(s32 *)(a2 + 0x8) = v0;

                        v0 = *(u16 *)((char *)a3 - 4);
                        /* ASM_BARRIER: load-use delay on vertex index 1 */
                        __asm__ volatile("" : "+r"(v0));
                        v0 = (s32)(t0 + (v0 << 3));
                        v0 = *(s32 *)((char *)v0 + 0x1A0);
                        /* ASM_BARRIER: load-use delay on SXY1 */
                        __asm__ volatile("" : "+r"(v0));
                        *(s32 *)(a2 + 0x10) = v0;

                        v0 = *(u16 *)((char *)a3 - 2);
                        /* ASM_BARRIER: load-use delay on vertex index 2 */
                        __asm__ volatile("" : "+r"(v0));
                        v0 = (s32)(t0 + (v0 << 3));
                        v0 = *(s32 *)((char *)v0 + 0x1A0);
                        /* ASM_BARRIER: load-use delay on SXY2 */
                        __asm__ volatile("" : "+r"(v0));
                        *(s32 *)(a2 + 0x18) = v0;

                        v0 = *(u16 *)((char *)a3 + 0);
                        /* ASM_BARRIER: load-use delay on vertex index 3 */
                        __asm__ volatile("" : "+r"(v0));
                        v0 = (s32)(t0 + (v0 << 3));
                        v0 = *(s32 *)((char *)v0 + 0x1A0);
                        /* ASM_BARRIER: order SXY3 load before t1 increment */
                        __asm__ volatile("" : "+r"(v0), "+r"(t1));
                        t1 += 0x28;
                        /* ASM_BARRIER: schedule t1 increment in load delay slot of SXY3 */
                        __asm__ volatile("" : "+r"(v0), "+r"(t1));
                        *(s32 *)(a2 + 0x0) = t6;
                        *(s32 *)(a2 + 0x20) = v0;
                        a2 += 0x28;
                        /* ASM_BARRIER: keep a2 increment inside conditional block */
                        __asm__ volatile("" : "+r"(a2));
                    }
                }
            }
            a3 += 0x14;
            t3 += 0x14;
            /* ASM_BARRIER: order pointer advances before loop condition */
            __asm__ volatile("" : "+r"(a3), "+r"(t3), "+r"(t2));
            v0 = t2;
            t2--;
        } while (v0 > 0);
    }
    *(char **)0x1F800000 = t1;
}

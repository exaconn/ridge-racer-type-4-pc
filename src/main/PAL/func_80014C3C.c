#include "types.h"
#include "rr_asm.h"

extern s32 func_80096464(void);

void func_80014C3C(char *a0_in, char *a1_in, s32 a2_in) {
    s32 sp[6];
    /* ASM_BARRIER: PsyQ-style register allocation */
    register s32 v0 __asm__("$2");
    /* ASM_BARRIER: PsyQ-style register allocation */
    register s32 v1 __asm__("$3");
    /* ASM_BARRIER: PsyQ-style register allocation */
    register s32 a0 __asm__("$4");
    /* ASM_BARRIER: PsyQ-style register allocation */
    register s32 a1 __asm__("$5");
    /* ASM_BARRIER: PsyQ-style register allocation */
    register s32 a2 __asm__("$6");
    /* ASM_BARRIER: PsyQ-style register allocation */
    register s32 a3 __asm__("$7");
    /* ASM_BARRIER: PsyQ-style register allocation */
    register s32 s0 __asm__("$16");
    /* ASM_BARRIER: PsyQ-style register allocation */
    register s32 s1 __asm__("$17");
    /* ASM_BARRIER: PsyQ-style register allocation */
    register char *s2 __asm__("$18");
    /* ASM_BARRIER: PsyQ-style register allocation */
    register s32 s3 __asm__("$19");
    /* ASM_BARRIER: PsyQ-style register allocation */
    register char *s4 __asm__("$20");
    /* ASM_BARRIER: PsyQ-style register allocation */
    register char *s5 __asm__("$21");
    /* ASM_BARRIER: PsyQ-style register allocation */
    register char *s6 __asm__("$22");
    /* ASM_BARRIER: PsyQ-style register allocation */
    register char *s7 __asm__("$23");
    /* ASM_BARRIER: PsyQ-style register allocation */
    s32 fp;

    s7 = a0_in;
    s5 = a1_in;
    s0 = a2_in;
    v0 = (s32)(s5 + 0x174);
    /* ASM_BARRIER: load far color from s5+0x174 into GTE FC registers */
    __asm__ volatile(
        "lbu $12, 0(%0); lbu $13, 1(%0); lbu $14, 2(%0); sll $12, $12, 4; sll $13, $13, 4; sll $14, $14, 4; ctc2 $12, $21; ctc2 $13, $22; ctc2 $14, $23"
        : : "r"(v0), "r"(s0) : "$12", "$13", "$14");

    a3 = 0x42000000;
    /* ASM_BARRIER: load RGB code into GTE $6 */
    __asm__ volatile("mtc2 %0, $6" : : "r"(a3));

    a3 = 0xE1000220;
    fp = 0x10;
    v0 = 0x1F800000;
    /* ASM_BARRIER: order setup before sp[2] store and scratchpad loads */
    __asm__ volatile("" : "+r"(a3), "+r"(fp), "+r"(v0));
    sp[2] = a3;
    s4 = *(char **)(v0 + 0x0);
    v1 = *(s32 *)(v0 + 0x4);
    v0 = *(s32 *)(v0 + 0x60);
    /* ASM_BARRIER: order 3 scratchpad loads before additions */
    __asm__ volatile("" : "+r"(s4), "+r"(v1), "+r"(v0));
    v1 += 0x1B0;
    v0 += 2;
    /* ASM_BARRIER: order additions before sp[0]/sp[1] stores and jal */
    __asm__ volatile("" : "+r"(v1), "+r"(v0));
    sp[0] = v1;
    sp[1] = v0;
    v0 = func_80096464() & 3;
    s0 = -(s0 + 0xA0);
    a3 = (s32)(s5 + 0xE0);
    s2 = s7 + 8;
    s6 = s4 + 4;
    /* ASM_BARRIER: order post-call setup before sp[3..5] stores */
    __asm__ volatile("" : "+r"(v0), "+r"(s0), "+r"(a3), "+r"(s2), "+r"(s6));
    sp[3] = v0;
    sp[4] = s0;
    sp[5] = a3;
    /* ASM_BARRIER: keep sp[3..5] stores before loop entry */
    __asm__ volatile("" : "+r"(s5) : : "memory");

    do {
        v0 = *(s32 *)s7;
        /* ASM_BARRIER: load-use delay on *(s32 *)s7 */
        __asm__ volatile("" : "+r"(v0));
        s3 = v0 - 0xAA;
        if (s3 <= 0) {
            /* ASM_BARRIER: prevent delay-slot filling into bgtz */
            __asm__ volatile("" : "+r"(s3));
            a3 = sp[3];
            /* ASM_BARRIER: load-use delay on sp[3] */
            __asm__ volatile("" : "+r"(a3));
            if (a3 != 0) {
                goto L_next;
            }
            /* ASM_BARRIER: prevent a3 = 1 from filling bnez delay slot */
            __asm__ volatile("" : "+r"(a3));
            a3 = 1;
            sp[3] = a3;
            s3 = 0x1000;
            v0 = func_80096464() & 0x1FF;
            s0 = v0 - 0x100;
            v0 = func_80096464() & 0x1FF;
            s1 = v0 - 0x100;
            if (s0 == 0) {
                /* ASM_BARRIER: keep s1 assignment in bnez delay slot */
                __asm__ volatile("" : "+r"(s1));
                s0 = 0x100;
            }
            /* ASM_BARRIER: join after s0 check */
            __asm__ volatile("" : "+r"(s0), "+r"(s1));
            if (s1 == 0) {
                s1 = 0x100;
            }
            v0 = s1 + 0x140;
            /* ASM_BARRIER: order v0 before stores and jal */
            __asm__ volatile("" : "+r"(v0), "+r"(s0), "+r"(s1));
            *(s16 *)(s2 - 4) = s0;
            *(s16 *)(s2 - 2) = v0;
            v0 = (func_80096464() & 0x7F) - 0xC30;
            *(s16 *)(s2 + 0) = v0;
            v0 = s0 / 2;
            *(s16 *)(s2 + 4) = v0;
            v0 = s1 / 2;
            *(s16 *)(s2 + 6) = v0;
            a3 = *(u16 *)&sp[4];
            /* ASM_BARRIER: load-use delay on sp[4] */
            __asm__ volatile("" : "+r"(a3));
            *(s16 *)(s2 + 2) = a3;
        }
        /* ASM_BARRIER: join at L80014DA0 */
        __asm__ volatile("" : "+r"(s3));
        *(s32 *)s7 = s3;
        RR_GTE_MTC2_IR0(s3);
        RR_GTE14_NOP2();
        RR_GTE_OP(0x4A780010);
        RR_GTE14_SWC2_RGB2(s4 + 8);

        s0 = *(s32 *)(s2 - 4);
        a2 = *(s32 *)(s2 + 0);
        a0 = *(s32 *)(s2 + 4);
        /* ASM_BARRIER: order 3 word loads from s2 */
        __asm__ volatile("" : "+r"(s0), "+r"(a2), "+r"(a0));
        s1 = s0 >> 16;
        v0 = s0 << 16;
        /* ASM_BARRIER: order s1 and v0 before s0 sign-extend */
        __asm__ volatile("" : "+r"(s1), "+r"(v0));
        s0 = v0 >> 16;
        a1 = a2 >> 16;
        v1 = a2 << 16;
        /* ASM_BARRIER: order s0, a1, v1 before a2 sign-extend */
        __asm__ volatile("" : "+r"(s0), "+r"(a1), "+r"(v1));
        a2 = v1 >> 16;
        v0 >>= 20;
        /* ASM_BARRIER: order a2 and v0 before sh E0 */
        __asm__ volatile("" : "+r"(a2), "+r"(v0));
        *(s16 *)(s5 + 0xE0) = v0;
        v0 = s1 >> 4;
        v1 >>= 20;
        /* ASM_BARRIER: order v0 and v1 before sh E4 */
        __asm__ volatile("" : "+r"(v0), "+r"(v1));
        *(s16 *)(s5 + 0xE4) = v1;
        v1 = a0 >> 16;
        a0 <<= 16;
        /* ASM_BARRIER: order v1 and a0 before sh E2 */
        __asm__ volatile("" : "+r"(v1), "+r"(a0), "+r"(v0));
        *(s16 *)(s5 + 0xE2) = v0;

        a3 = sp[5];
        /* ASM_BARRIER: load-use delay on sp[5] */
        __asm__ volatile("" : "+r"(a3));
        /* ASM_BARRIER: load V1 from a3 */
        __asm__ volatile("lwc2 $2, 0(%0); lwc2 $3, 4(%0)" : : "r"(a3) : "memory");

        v0 = (s0 + (a0 >> 19)) >> 4;
        *(s16 *)(s5 + 0xE0) = v0;
        v0 = (s1 + (v1 >> 3)) >> 4;
        *(s16 *)(s5 + 0xE2) = v0;
        v0 = (a2 + (a1 >> 3)) >> 4;
        *(s16 *)(s5 + 0xE4) = v0;

        /* ASM_BARRIER: load V2 from a3 and run RTPT */
        __asm__ volatile("lwc2 $4, 0(%0); lwc2 $5, 4(%0); nop; nop; .word 0x4A280030" : : "r"(a3) : "memory");

        a0 = s0 + (a0 >> 20);
        v1 = s1 + (v1 >> 4);
        a1 = a2 + (a1 >> 4);
        v0 = (s32)(s5 + 0x70);
        /* ASM_BARRIER: order additions and v0 address before stores */
        __asm__ volatile("" : "+r"(a0), "+r"(v1), "+r"(a1), "+r"(v0));
        *(s16 *)(s2 - 4) = a0;
        *(s16 *)(s2 - 2) = v1;
        *(s16 *)(s2 + 0) = a1;
        /* ASM_BARRIER: store SZ3 to s5+0x70 */
        __asm__ volatile("swc2 $19, 0(%0)" : : "r"(v0) : "memory");

        v0 = *(s32 *)(s5 + 0x70);
        /* ASM_BARRIER: load-use delay on s5+0x70 */
        __asm__ volatile("" : "+r"(v0));
        if (v0 > 0) {
            /* ASM_BARRIER: prevent delay-slot filling into blez */
            __asm__ volatile("" : "+r"(v0));
            a3 = sp[1];
            /* ASM_BARRIER: load-use delay on sp[1] */
            __asm__ volatile("" : "+r"(a3));
            v1 = v0 >> a3;
            if (v1 < 0x1C0) {
                v0 = (s32)(s4 + 0xC);
                /* ASM_BARRIER: store SXY1 to s4+0xC */
                __asm__ volatile("swc2 $13, 0(%0)" : : "r"(v0) : "memory");
                v0 = (s32)(s4 + 0x10);
                /* ASM_BARRIER: store SXY2 to s4+0x10 */
                __asm__ volatile("swc2 $14, 0(%0)" : : "r"(v0) : "memory");

                a3 = sp[0];
                /* ASM_BARRIER: order sp[0] load before v1 shift to fill load delay slot */
                __asm__ volatile("" : "+r"(a3), "+r"(v1));
                v1 <<= 2;
                /* ASM_BARRIER: order v1 shift before addition */
                __asm__ volatile("" : "+r"(a3), "+r"(v1));
                v1 += a3;
                v0 = *(s32 *)v1;
                /* ASM_BARRIER: load-use delay on OT entry */
                __asm__ volatile("" : "+r"(v0), "+r"(v1));
                *(s32 *)s4 = v0;
                *(u8 *)(s6 - 1) = 4;
                *(char **)v1 = s4;
                *(u8 *)((char *)v1 + 3) = 0;
                /* ASM_BARRIER: order byte store before sp[2] load */
                __asm__ volatile("" : : : "memory");

                a3 = sp[2];
                /* ASM_BARRIER: order sp[2] load before s4 increment in its load delay slot */
                __asm__ volatile("" : "+r"(a3), "+r"(s4));
                s4 += 0x14;
                /* ASM_BARRIER: order s4 increment before s6 store */
                __asm__ volatile("" : "+r"(a3), "+r"(s4));
                *(s32 *)s6 = a3;
                s6 += 0x14;
                /* ASM_BARRIER: keep s6 increment inside conditional block */
                __asm__ volatile("" : "+r"(s6));
            }
        }
L_next:
        /* ASM_BARRIER: prevent delay-slot filling from L_next into bnez/blez/beqz */
        __asm__ volatile("" : "+r"(s2));
        s2 += 0x10;
        fp--;
        /* ASM_BARRIER: order s2 and fp updates before bgtz */
        __asm__ volatile("" : "+r"(s2), "+r"(fp));
        s7 += 0x10;
    } while (fp > 0);

    *(char **)0x1F800000 = s4;
}

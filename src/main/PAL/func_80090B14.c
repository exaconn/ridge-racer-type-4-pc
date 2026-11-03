#include "types.h"

int *func_80090B14(int *a0, int *a1, int *a2_in) {
    /* ASM_BARRIER: pin $zero ($0) to emit PC-relative beq $0, $0 (b) instead of j */
    register int zero __asm__("$0");
    /* ASM_BARRIER: keep third argument pointer in $a2 ($6) */
    register int *a2 __asm__("$6") = a2_in;
    /* ASM_BARRIER: PsyQ hand-coded register allocation */
    register int t0 __asm__("$8");
    /* ASM_BARRIER: PsyQ hand-coded register allocation */
    register int t1 __asm__("$9");
    /* ASM_BARRIER: PsyQ hand-coded register allocation */
    register int t2 __asm__("$10");
    /* ASM_BARRIER: PsyQ hand-coded register allocation */
    register int t3 __asm__("$11");
    /* ASM_BARRIER: PsyQ hand-coded register allocation */
    register int t4 __asm__("$12");
    /* ASM_BARRIER: PsyQ hand-coded register allocation */
    register int t5 __asm__("$13");

    t0 = a0[0];
    t1 = a0[1];
    t2 = a0[2];
    t3 = a0[3];
    t4 = a0[4];
    /* ASM_BARRIER: GTE rotation matrix control register load */
    __asm__ volatile ("ctc2 %0, $0" : : "r"(t0));
    /* ASM_BARRIER: GTE rotation matrix control register load */
    __asm__ volatile ("ctc2 %0, $1" : : "r"(t1));
    /* ASM_BARRIER: GTE rotation matrix control register load */
    __asm__ volatile ("ctc2 %0, $2" : : "r"(t2));
    /* ASM_BARRIER: GTE rotation matrix control register load */
    __asm__ volatile ("ctc2 %0, $3" : : "r"(t3));
    /* ASM_BARRIER: GTE rotation matrix control register load */
    __asm__ volatile ("ctc2 %0, $4" : : "r"(t4));

    t0 = a1[0];
    t1 = a1[1];
    t2 = a1[2];
    /* ASM_BARRIER: keep a1 loads ordered before sign-split branches */
    __asm__ volatile ("" : "+r"(t0), "+r"(t1), "+r"(t2), "=r"(zero));

    if (t0 < 0) {
        /* ASM_BARRIER: prevent delay-slot filling into bgez */
        __asm__ volatile ("" : "+r"(t0));
        t0 = -t0;
        /* ASM_BARRIER: order negu before sra 15 */
        __asm__ volatile ("" : "+r"(t0));
        t3 = t0 >> 15;
        /* ASM_BARRIER: order sra 15 before negu t3 */
        __asm__ volatile ("" : "+r"(t3), "+r"(t0));
        t3 = -t3;
        /* ASM_BARRIER: order negu t3 before andi t0 */
        __asm__ volatile ("" : "+r"(t3), "+r"(t0));
        t0 &= 0x7FFF;
        /* ASM_BARRIER: place final negu t0 in branch delay slot */
        __asm__ volatile ("" : "+r"(t0));
        t0 = -t0;
        if (zero == 0) {
            goto L80090B70;
        }
    }
    /* ASM_BARRIER: prevent delay-slot filling into bgez from non-negative path */
    __asm__ volatile ("" : "+r"(t0));
    t3 = t0 >> 15;
    /* ASM_BARRIER: order sra 15 before andi */
    __asm__ volatile ("" : "+r"(t3), "+r"(t0));
    t0 &= 0x7FFF;
L80090B70:
    /* ASM_BARRIER: join point after first component split */
    __asm__ volatile ("" : "+r"(t3), "+r"(t0), "+r"(zero));

    if (t1 < 0) {
        /* ASM_BARRIER: prevent delay-slot filling into bgez */
        __asm__ volatile ("" : "+r"(t1));
        t1 = -t1;
        /* ASM_BARRIER: order negu before sra 15 */
        __asm__ volatile ("" : "+r"(t1));
        t4 = t1 >> 15;
        /* ASM_BARRIER: order sra 15 before negu t4 */
        __asm__ volatile ("" : "+r"(t4), "+r"(t1));
        t4 = -t4;
        /* ASM_BARRIER: order negu t4 before andi t1 */
        __asm__ volatile ("" : "+r"(t4), "+r"(t1));
        t1 &= 0x7FFF;
        /* ASM_BARRIER: place final negu t1 in branch delay slot */
        __asm__ volatile ("" : "+r"(t1));
        t1 = -t1;
        if (zero == 0) {
            goto L80090B98;
        }
    }
    /* ASM_BARRIER: prevent delay-slot filling into bgez from non-negative path */
    __asm__ volatile ("" : "+r"(t1));
    t4 = t1 >> 15;
    /* ASM_BARRIER: order sra 15 before andi */
    __asm__ volatile ("" : "+r"(t4), "+r"(t1));
    t1 &= 0x7FFF;
L80090B98:
    /* ASM_BARRIER: join point after second component split */
    __asm__ volatile ("" : "+r"(t4), "+r"(t1), "+r"(zero));

    if (t2 < 0) {
        /* ASM_BARRIER: prevent delay-slot filling into bgez */
        __asm__ volatile ("" : "+r"(t2));
        t2 = -t2;
        /* ASM_BARRIER: order negu before sra 15 */
        __asm__ volatile ("" : "+r"(t2));
        t5 = t2 >> 15;
        /* ASM_BARRIER: order sra 15 before negu t5 */
        __asm__ volatile ("" : "+r"(t5), "+r"(t2));
        t5 = -t5;
        /* ASM_BARRIER: order negu t5 before andi t2 */
        __asm__ volatile ("" : "+r"(t5), "+r"(t2));
        t2 &= 0x7FFF;
        /* ASM_BARRIER: place final negu t2 in branch delay slot */
        __asm__ volatile ("" : "+r"(t2));
        t2 = -t2;
        if (zero == 0) {
            goto L80090BC0;
        }
    }
    /* ASM_BARRIER: prevent delay-slot filling into bgez from non-negative path */
    __asm__ volatile ("" : "+r"(t2));
    t5 = t2 >> 15;
    /* ASM_BARRIER: order sra 15 before andi */
    __asm__ volatile ("" : "+r"(t5), "+r"(t2));
    t2 &= 0x7FFF;
L80090BC0:
    /* ASM_BARRIER: join point after third component split */
    __asm__ volatile ("" : "+r"(t5), "+r"(t2), "+r"(zero));

    /* ASM_BARRIER: GTE IR1..IR3 load for high-half vector */
    __asm__ volatile ("mtc2 %0, $9" : : "r"(t3));
    /* ASM_BARRIER: GTE IR1..IR3 load for high-half vector */
    __asm__ volatile ("mtc2 %0, $10" : : "r"(t4));
    /* ASM_BARRIER: GTE IR1..IR3 load for high-half vector */
    __asm__ volatile ("mtc2 %0, $11" : : "r"(t5));
    /* ASM_BARRIER: GTE MVMVA sf=0 mx=rot v=IR cv=none lm=0 */
    __asm__ volatile ("nop\n\t.word 0x4A41E012");
    /* ASM_BARRIER: GTE MAC1..MAC3 read for high-half result */
    __asm__ volatile ("mfc2 %0, $25" : "=r"(t3));
    /* ASM_BARRIER: GTE MAC1..MAC3 read for high-half result */
    __asm__ volatile ("mfc2 %0, $26" : "=r"(t4));
    /* ASM_BARRIER: GTE MAC1..MAC3 read for high-half result */
    __asm__ volatile ("mfc2 %0, $27" : "=r"(t5));
    /* ASM_BARRIER: GTE IR1..IR3 load for low-half vector */
    __asm__ volatile ("mtc2 %0, $9" : : "r"(t0));
    /* ASM_BARRIER: GTE IR1..IR3 load for low-half vector */
    __asm__ volatile ("mtc2 %0, $10" : : "r"(t1));
    /* ASM_BARRIER: GTE IR1..IR3 load for low-half vector */
    __asm__ volatile ("mtc2 %0, $11" : : "r"(t2));
    /* ASM_BARRIER: GTE MVMVA sf=1 mx=rot v=IR cv=none lm=0 */
    __asm__ volatile ("nop\n\t.word 0x4A49E012");

    if (t3 < 0) {
        /* ASM_BARRIER: prevent delay-slot filling into bgez */
        __asm__ volatile ("" : "+r"(t3));
        t3 = -t3;
        /* ASM_BARRIER: order negu before sll 3 */
        __asm__ volatile ("" : "+r"(t3));
        t3 <<= 3;
        /* ASM_BARRIER: place final negu t3 in branch delay slot */
        __asm__ volatile ("" : "+r"(t3));
        t3 = -t3;
        if (zero == 0) {
            goto L80090C10;
        }
    }
    /* ASM_BARRIER: prevent delay-slot filling into bgez from non-negative path */
    __asm__ volatile ("" : "+r"(t3));
    t3 <<= 3;
L80090C10:
    /* ASM_BARRIER: join point after first high-half shift */
    __asm__ volatile ("" : "+r"(t3), "+r"(zero));

    if (t4 < 0) {
        /* ASM_BARRIER: prevent delay-slot filling into bgez */
        __asm__ volatile ("" : "+r"(t4));
        t4 = -t4;
        /* ASM_BARRIER: order negu before sll 3 */
        __asm__ volatile ("" : "+r"(t4));
        t4 <<= 3;
        /* ASM_BARRIER: place final negu t4 in branch delay slot */
        __asm__ volatile ("" : "+r"(t4));
        t4 = -t4;
        if (zero == 0) {
            goto L80090C2C;
        }
    }
    /* ASM_BARRIER: prevent delay-slot filling into bgez from non-negative path */
    __asm__ volatile ("" : "+r"(t4));
    t4 <<= 3;
L80090C2C:
    /* ASM_BARRIER: join point after second high-half shift */
    __asm__ volatile ("" : "+r"(t4), "+r"(zero));

    if (t5 < 0) {
        /* ASM_BARRIER: prevent delay-slot filling into bgez */
        __asm__ volatile ("" : "+r"(t5));
        t5 = -t5;
        /* ASM_BARRIER: order negu before sll 3 */
        __asm__ volatile ("" : "+r"(t5));
        t5 <<= 3;
        /* ASM_BARRIER: place final negu t5 in branch delay slot */
        __asm__ volatile ("" : "+r"(t5));
        t5 = -t5;
        if (zero == 0) {
            goto L80090C48;
        }
    }
    /* ASM_BARRIER: prevent delay-slot filling into bgez from non-negative path */
    __asm__ volatile ("" : "+r"(t5));
    t5 <<= 3;
L80090C48:
    /* ASM_BARRIER: join point after third high-half shift */
    __asm__ volatile ("" : "+r"(t5));

    /* ASM_BARRIER: GTE MAC1..MAC3 read for low-half result */
    __asm__ volatile ("mfc2 %0, $25" : "=r"(t0));
    /* ASM_BARRIER: GTE MAC1..MAC3 read for low-half result */
    __asm__ volatile ("mfc2 %0, $26" : "=r"(t1));
    /* ASM_BARRIER: GTE MAC1..MAC3 read for low-half result */
    __asm__ volatile ("mfc2 %0, $27" : "=r"(t2));

    t0 = t0 + t3;
    t1 = t1 + t4;
    t2 = t2 + t5;
    /* ASM_BARRIER: order additions before output stores */
    __asm__ volatile ("" : "+r"(t0), "+r"(t1), "+r"(t2));

    a2[0] = t0;
    a2[1] = t1;
    a2[2] = t2;
    /* ASM_BARRIER: order final store before return value copy in delay slot */
    __asm__ volatile ("" : : "r"(t0), "r"(t1), "r"(t2) : "memory");
    return a2;
}

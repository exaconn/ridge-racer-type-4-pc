#include "types.h"
#include "rr_asm.h"
#include "psyq/libgte/gtemat3.h"


Vec3i *ApplyMatrixLV(const MatrixWords32 *matrix, const Vec3i *input, Vec3i *output_in) {
    /* ASM_BARRIER: pin $zero ($0) to emit PC-relative beq $0, $0 (b) instead of j */
    register int zero __asm__("$0");
    /* ASM_BARRIER: keep third argument pointer in $a2 ($6) */
    register Vec3i *output __asm__("$6") = output_in;
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

    t0 = matrix->w[0];
    t1 = matrix->w[1];
    t2 = matrix->w[2];
    t3 = matrix->w[3];
    t4 = matrix->w[4];
    RR_GTE_CTC2(t0, 0);
    RR_GTE_CTC2(t1, 1);
    RR_GTE_CTC2(t2, 2);
    RR_GTE_CTC2(t3, 3);
    RR_GTE_CTC2(t4, 4);

    t0 = input->x;
    t1 = input->y;
    t2 = input->z;

    if (t0 < 0) {
        t0 = 0U - t0;
        t3 = t0 >> 15;
        t3 = 0U - t3;
        t0 &= 0x7FFF;
        t0 = 0U - t0;
        if (zero == 0) {
            goto L80090B70;
        }
    }
    /* ASM_BARRIER: prevent delay-slot filling into bgez from non-negative path */
    __asm__ volatile ("" : "+r"(t0));
    t3 = t0 >> 15;
    t0 &= 0x7FFF;
L80090B70:

    if (t1 < 0) {
        t1 = 0U - t1;
        t4 = t1 >> 15;
        t4 = 0U - t4;
        t1 &= 0x7FFF;
        t1 = 0U - t1;
        if (zero == 0) {
            goto L80090B98;
        }
    }
    /* ASM_BARRIER: prevent delay-slot filling into bgez from non-negative path */
    __asm__ volatile ("" : "+r"(t1));
    t4 = t1 >> 15;
    t1 &= 0x7FFF;
L80090B98:

    if (t2 < 0) {
        t2 = 0U - t2;
        t5 = t2 >> 15;
        t5 = 0U - t5;
        t2 &= 0x7FFF;
        t2 = 0U - t2;
        if (zero == 0) {
            goto L80090BC0;
        }
    }
    /* ASM_BARRIER: prevent delay-slot filling into bgez from non-negative path */
    __asm__ volatile ("" : "+r"(t2));
    t5 = t2 >> 15;
    t2 &= 0x7FFF;
L80090BC0:

    RR_GTE_MTC2(t3, 9);
    RR_GTE_MTC2(t4, 10);
    RR_GTE_MTC2(t5, 11);
    RR_GTE_CMD_1NOP(0x4A41E012);
    RR_GTE_MFC2(t3, 25);
    RR_GTE_MFC2(t4, 26);
    RR_GTE_MFC2(t5, 27);
    RR_GTE_MTC2(t0, 9);
    RR_GTE_MTC2(t1, 10);
    RR_GTE_MTC2(t2, 11);
    RR_GTE_CMD_1NOP(0x4A49E012);

    if (t3 < 0) {
        t3 = 0U - t3;
        t3 = (0U + t3) << 3;
        /* ASM_BARRIER: place final negu t3 in branch delay slot */
        __asm__ volatile ("" : "+r"(t3));
        t3 = 0U - t3;
        if (zero == 0) {
            goto L80090C10;
        }
    }
    t3 = (0U + t3) << 3;
L80090C10:

    if (t4 < 0) {
        t4 = 0U - t4;
        t4 = (0U + t4) << 3;
        /* ASM_BARRIER: place final negu t4 in branch delay slot */
        __asm__ volatile ("" : "+r"(t4));
        t4 = 0U - t4;
        if (zero == 0) {
            goto L80090C2C;
        }
    }
    t4 = (0U + t4) << 3;
L80090C2C:

    if (t5 < 0) {
        t5 = 0U - t5;
        t5 = (0U + t5) << 3;
        /* ASM_BARRIER: place final negu t5 in branch delay slot */
        __asm__ volatile ("" : "+r"(t5));
        t5 = 0U - t5;
        if (zero == 0) {
            goto L80090C48;
        }
    }
    t5 = (0U + t5) << 3;
L80090C48:

    RR_GTE_MFC2(t0, 25);
    RR_GTE_MFC2(t1, 26);
    RR_GTE_MFC2(t2, 27);

    t0 = 0U + t0 + t3;
    t1 = 0U + t1 + t4;
    t2 = 0U + t2 + t5;

    output->x = t0;
    output->y = t1;
    output->z = t2;
    /* ASM_BARRIER: order final store before return value copy in delay slot */
    __asm__ volatile ("" : : "r"(t0), "r"(t1), "r"(t2) : "memory");
    return output;
}

Vec3i *ApplyRotMatrix(const Vec3s8 *input, Vec3i *output) {
    Vec3i *r;
    RR_GTE_LDV0_2W(input);
    RR_GTE_MVMVA_ROT_V0_1NOP();
    RR_GTE_SWC2_TRIPLE_AT(9, 0, 10, 4, 11, 8, output);
    /* ASM_BARRIER: legacy, reason not recorded (forces reg copy/CSE/scheduling); retire & retest with fd.py */
    __asm__ volatile ("" : "=r"(r) : "0"(output));
    return r;
}

MatrixWords32 *MulMatrix(MatrixWords32 *basis, const MatrixWords32 *right) {
    register u32 t0 __asm__("$8");
    register u32 t1 __asm__("$9");
    register u32 t2 __asm__("$10");
    register u32 t3 __asm__("$11");
    register u32 t4 __asm__("$12");
    register u32 t5 __asm__("$13");
    register u32 t6 __asm__("$14");
    register u32 t7 __asm__("$15");
    register u32 t8 __asm__("$24");
    MatrixWords32 *result;
    t0 = basis->w[0];
    t1 = basis->w[1];
    t2 = basis->w[2];
    t3 = basis->w[3];
    t4 = basis->w[4];
    RR_GTE_CTC2(t0, 0);
    RR_GTE_CTC2(t1, 1);
    RR_GTE_CTC2(t2, 2);
    RR_GTE_CTC2(t3, 3);
    RR_GTE_CTC2(t4, 4);
    t0 = right->halves[0];
    t1 = right->w[1];
    t2 = right->w[3];
    RR_GTE_MASK_HI16(t1);
    t0 |= t1;
    RR_GTE_MTC2(t0, 0);
    RR_GTE_MTC2(t2, 1);
    RR_GTE_MVMVA_ROT_V0_1NOP();
    t0 = right->halves[1];
    t1 = right->w[2];
    t2 = (s16)right->halves[7];
    t1 <<= 16;
    t0 |= t1;
    RR_GTE_MFC2(t3, 9);
    RR_GTE_MFC2(t4, 10);
    RR_GTE_MFC2(t5, 11);
    RR_GTE_MTC2(t0, 0);
    RR_GTE_MTC2(t2, 1);
    RR_GTE_MVMVA_ROT_V0_1NOP();
    t0 = right->halves[2];
    t1 = right->w[2];
    t2 = right->w[4];
    RR_GTE_MASK_HI16(t1);
    t0 |= t1;
    RR_GTE_MFC2(t6, 9);
    RR_GTE_MFC2(t7, 10);
    RR_GTE_MFC2(t8, 11);
    RR_GTE_MTC2(t0, 0);
    RR_GTE_MTC2(t2, 1);
    RR_GTE_MVMVA_ROT_V0_1NOP();
    t3 &= 0xFFFF;
    t6 <<= 16;
    t6 |= t3;
    RR_GTE_SW_AT(t6, basis, 0);
    t5 &= 0xFFFF;
    t8 <<= 16;
    t8 |= t5;
    RR_GTE_SW_AT(t8, basis, 3);
    RR_GTE_MFC2(t0, 9);
    RR_GTE_MFC2(t1, 10);
    t0 &= 0xFFFF;
    t4 <<= 16;
    t0 |= t4;
    RR_GTE_SW_AT(t0, basis, 1);
    t7 &= 0xFFFF;
    t1 <<= 16;
    t1 |= t7;
    RR_GTE_SW_AT(t1, basis, 2);
    RR_GTE_SWC2_AT(11, 16, basis);
    /* ASM_BARRIER: keeps return pointer in $v0 across GTE register sequence */
    __asm__ volatile ("" : "=r"(result) : "0"(basis));
    return result;
}

/* GCC_VERSION: 970318 */
#include "rr_asm.h"

int *func_80090EC4(int *a0, int *a1, int *a2);
int *func_80090F14(int *a0, int *a1, short *a2);

int *func_80090EC4(int *a0, int *a1, int *a2) {
    register int t0 __asm__("$8");
    register int t1 __asm__("$9");
    register int t2 __asm__("$10");
    register int t3 __asm__("$11");
    register int t4 __asm__("$12");
    int *r;
    t0 = a0[0];
    t1 = a0[1];
    t2 = a0[2];
    t3 = a0[3];
    t4 = a0[4];
    __asm__ volatile ("ctc2 %0, $0" : : "r"(t0));
    __asm__ volatile ("ctc2 %0, $1" : : "r"(t1));
    __asm__ volatile ("ctc2 %0, $2" : : "r"(t2));
    __asm__ volatile ("ctc2 %0, $3" : : "r"(t3));
    __asm__ volatile ("ctc2 %0, $4" : : "r"(t4));
    __asm__ volatile ("lwc2 $0, 0(%0)\n\tlwc2 $1, 4(%0)" : : "r"(a1));
    __asm__ volatile ("nop\n\t.word 0x4A486012");
    __asm__ volatile ("swc2 $25, 0(%0)\n\tswc2 $26, 4(%0)\n\tswc2 $27, 8(%0)" : : "r"(a2));
    /* ASM_BARRIER: legacy, reason not recorded (forces reg copy/CSE/scheduling); retire & retest with fd.py */
    __asm__ volatile ("" : "=r"(r) : "0"(a2));
    return r;
}

int *func_80090F14(int *a0, int *a1, short *a2) {
    register int t0 __asm__("$8");
    register int t1 __asm__("$9");
    register int t2 __asm__("$10");
    register int t3 __asm__("$11");
    register int t4 __asm__("$12");
    int *r;
    t0 = a0[0];
    t1 = a0[1];
    t2 = a0[2];
    t3 = a0[3];
    t4 = a0[4];
    __asm__ volatile ("ctc2 %0, $0" : : "r"(t0));
    __asm__ volatile ("ctc2 %0, $1" : : "r"(t1));
    __asm__ volatile ("ctc2 %0, $2" : : "r"(t2));
    __asm__ volatile ("ctc2 %0, $3" : : "r"(t3));
    __asm__ volatile ("ctc2 %0, $4" : : "r"(t4));
    __asm__ volatile ("lwc2 $0, 0(%0)\n\tlwc2 $1, 4(%0)" : : "r"(a1));
    RR_GTE_MVMVA_ROT_V0_1NOP();
    __asm__ volatile ("mfc2 %0, $9" : "=r"(t0));
    __asm__ volatile ("mfc2 %0, $10" : "=r"(t1));
    __asm__ volatile ("mfc2 %0, $11" : "=r"(t2));
    RR_GTE_SH_AT(t0, a2, 0);
    RR_GTE_SH_AT(t1, a2, 1);
    RR_GTE_SH_AT(t2, a2, 2);
    /* ASM_BARRIER: keeps return pointer in $v0 across GTE register sequence */
    __asm__ volatile ("" : "=r"(r) : "0"((int *)a2));
    return r;
}

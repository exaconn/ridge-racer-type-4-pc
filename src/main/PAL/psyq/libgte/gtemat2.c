#include "rr_asm.h"
#include "psyq/libgte/gtemat2.h"

int *func_80090A24(unsigned short *a0, int *a1) {
    register int t0 __asm__("$8");
    register int t1 __asm__("$9");
    register int t2 __asm__("$10");
    register int t3 __asm__("$11");
    register int t4 __asm__("$12");
    register int t5 __asm__("$13");
    register int t6 __asm__("$14");
    register int t7 __asm__("$15");
    register int t8 __asm__("$24");
    int *r;
    t0 = a0[0];
    t1 = *(int *)(a0 + 2);
    t2 = *(int *)(a0 + 6);
    RR_GTE_MASK_HI16(t1);
    t0 |= t1;
    __asm__ volatile ("mtc2 %0, $0" : : "r"(t0));
    __asm__ volatile ("mtc2 %0, $1" : : "r"(t2));
    RR_GTE_MVMVA_ROT_V0_1NOP();
    t0 = a0[1];
    t1 = *(int *)(a0 + 4);
    t2 = *(short *)(a0 + 7);
    t1 <<= 16;
    t0 |= t1;
    __asm__ volatile ("mfc2 %0, $9" : "=r"(t3));
    __asm__ volatile ("mfc2 %0, $10" : "=r"(t4));
    __asm__ volatile ("mfc2 %0, $11" : "=r"(t5));
    __asm__ volatile ("mtc2 %0, $0" : : "r"(t0));
    __asm__ volatile ("mtc2 %0, $1" : : "r"(t2));
    RR_GTE_MVMVA_ROT_V0_1NOP();
    t0 = a0[2];
    t1 = *(int *)(a0 + 4);
    t2 = *(int *)(a0 + 8);
    RR_GTE_MASK_HI16(t1);
    t0 |= t1;
    __asm__ volatile ("mfc2 %0, $9" : "=r"(t6));
    __asm__ volatile ("mfc2 %0, $10" : "=r"(t7));
    __asm__ volatile ("mfc2 %0, $11" : "=r"(t8));
    __asm__ volatile ("mtc2 %0, $0" : : "r"(t0));
    __asm__ volatile ("mtc2 %0, $1" : : "r"(t2));
    RR_GTE_MVMVA_ROT_V0_1NOP();
    t3 &= 0xFFFF;
    t6 <<= 16;
    t6 |= t3;
    RR_GTE_SW_AT(t6, a1, 0);
    t5 &= 0xFFFF;
    t8 <<= 16;
    t8 |= t5;
    RR_GTE_SW_AT(t8, a1, 3);
    __asm__ volatile ("mfc2 %0, $9" : "=r"(t0));
    __asm__ volatile ("mfc2 %0, $10" : "=r"(t1));
    t0 &= 0xFFFF;
    t4 <<= 16;
    t0 |= t4;
    RR_GTE_SW_AT(t0, a1, 1);
    t7 &= 0xFFFF;
    t1 <<= 16;
    t1 |= t7;
    RR_GTE_SW_AT(t1, a1, 2);
    __asm__ volatile ("swc2 $11, 16(%0)" : : "r"(a1));
    /* ASM_BARRIER: keeps return pointer in $v0 across GTE register sequence */
    __asm__ volatile ("" : "=r"(r) : "0"(a1));
    return r;
}

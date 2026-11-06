#include "rr_asm.h"
#include "psyq/libgte/gtemath4.h"

int RotAverage3(int *a0, int *a1, int *a2, int *a3, int * volatile a4, int * volatile a5, int * volatile a6, int * volatile a7) {
    register int *t0 __asm__("$8");
    register int *t1 __asm__("$9");
    register int *t2 __asm__("$10");
    register int *t3 __asm__("$11");
    register int f __asm__("$8");
    int v;
    __asm__ volatile ("lwc2 $0, 0(%0)\n\tlwc2 $1, 4(%0)" : : "r"(a0));
    __asm__ volatile ("lwc2 $2, 0(%0)\n\tlwc2 $3, 4(%0)" : : "r"(a1));
    __asm__ volatile ("lwc2 $4, 0(%0)\n\tlwc2 $5, 4(%0)" : : "r"(a2));
    __asm__ volatile ("nop\n\t.word 0x4A280030");
    t0 = a4;
    t1 = a5;
    t2 = a6;
    t3 = a7;
    __asm__ volatile ("swc2 $12, 0(%0)" : : "r"(a3));
    __asm__ volatile ("swc2 $13, 0(%0)" : : "r"(t0));
    __asm__ volatile ("swc2 $14, 0(%0)" : : "r"(t1));
    __asm__ volatile ("cfc2 %0, $31" : "=r"(f));
    __asm__ volatile ("swc2 $8, 0(%0)" : : "r"(t2));
    *t3 = f;
    RR_GTE_OP(0x4B58002D);
    __asm__ volatile ("mfc2 %0, $7" : "=r"(v));
    return v;
}

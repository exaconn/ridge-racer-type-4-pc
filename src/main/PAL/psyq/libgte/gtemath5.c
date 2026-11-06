#include "rr_asm.h"
#include "psyq/libgte/gtemath5.h"

int RotAverage4(int *a0, int *a1, int *a2, int *a3, int * volatile a4, int * volatile a5, int * volatile a6, int * volatile a7, int * volatile a8, int * volatile a9) {
    register int *t0 __asm__("$8");
    register int *t1 __asm__("$9");
    register int *t2 __asm__("$10");
    register int f __asm__("$3");
    register int g __asm__("$8");
    int v;
    __asm__ volatile ("lwc2 $0, 0(%0)\n\tlwc2 $1, 4(%0)" : : "r"(a0));
    __asm__ volatile ("lwc2 $2, 0(%0)\n\tlwc2 $3, 4(%0)" : : "r"(a1));
    __asm__ volatile ("lwc2 $4, 0(%0)\n\tlwc2 $5, 4(%0)" : : "r"(a2));
    __asm__ volatile ("nop\n\t.word 0x4A280030");
    t0 = a4;
    t1 = a5;
    t2 = a6;
    __asm__ volatile ("swc2 $12, 0(%0)" : : "r"(t0));
    __asm__ volatile ("swc2 $13, 0(%0)" : : "r"(t1));
    __asm__ volatile ("swc2 $14, 0(%0)" : : "r"(t2));
    __asm__ volatile ("cfc2 %0, $31" : "=r"(f));
    __asm__ volatile ("lwc2 $0, 0(%0)\n\tlwc2 $1, 4(%0)" : : "r"(a3));
    __asm__ volatile ("nop\n\t.word 0x4A180001");
    t0 = a7;
    t1 = a8;
    t2 = a9;
    __asm__ volatile ("swc2 $14, 0(%0)" : : "r"(t0));
    __asm__ volatile ("cfc2 %0, $31" : "=r"(g));
    __asm__ volatile ("swc2 $8, 0(%0)" : : "r"(t1));
    g |= f;
    *t2 = g;
    RR_GTE_OP(0x4B68002E);
    __asm__ volatile ("mfc2 %0, $7" : "=r"(v));
    return v;
}

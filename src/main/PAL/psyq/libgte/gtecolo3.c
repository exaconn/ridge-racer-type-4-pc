/* Candidate module: GTE AverageZ3 / AverageZ4 wrappers, 80091498..800914B8. */
#include "rr_asm.h"
#include "psyq/libgte/gtecolo3.h"

void LocalLight(int *a0, int *a1);
void DpqColor(int *a0, int a1, int *a2);
void NormalColor(int *a0, int *a1);
void NormalColor3(int *a0, int *a1, int *a2, int *a3, int * volatile a4, int * volatile a5);
void NormalColorDpq(int *a0, int *a1, int a2, int *a3);
void NormalColorDpq3(int *a0, int *a1, int *a2, int *a3, volatile int a4, int * volatile a5, int * volatile a6, int * volatile a7);
void NormalColorCol(int *a0, int *a1, int *a2);
void NormalColorCol3(int *a0, int *a1, int *a2, int *a3, int * volatile a4, int * volatile a5, int * volatile a6);
void ColorDpq(int *a0, int *a1, int a2, int *a3);
void ColorCol(int *a0, int *a1, int *a2);
int func_80091498(void);
int func_800914A8(void);

void LocalLight(int *a0, int *a1) {
    __asm__ volatile ("lwc2 $0, 0(%0)\n\tlwc2 $1, 4(%0)" : : "r"(a0));
    __asm__ volatile ("nop\n\t.word 0x4A4A6412");
    __asm__ volatile ("swc2 $9, 0(%0)\n\tswc2 $10, 4(%0)\n\tswc2 $11, 8(%0)" : : "r"(a1));
}

void DpqColor(int *a0, int a1, int *a2) {
    __asm__ volatile ("lwc2 $6, 0(%0)" : : "r"(a0));
    __asm__ volatile ("mtc2 %0, $8" : : "r"(a1));
    __asm__ volatile ("nop\n\t.word 0x4A780010");
    __asm__ volatile ("swc2 $22, 0(%0)" : : "r"(a2));
}

void NormalColor(int *a0, int *a1) {
    __asm__ volatile ("lwc2 $0, 0(%0)\n\tlwc2 $1, 4(%0)" : : "r"(a0));
    __asm__ volatile ("nop\n\t.word 0x4AC8041E");
    __asm__ volatile ("swc2 $22, 0(%0)" : : "r"(a1));
}

void NormalColor3(int *a0, int *a1, int *a2, int *a3, int * volatile a4, int * volatile a5) {
    register int *t0 __asm__("$8");
    register int *t1 __asm__("$9");
    __asm__ volatile ("lwc2 $0, 0(%0)\n\tlwc2 $1, 4(%0)" : : "r"(a0));
    __asm__ volatile ("lwc2 $2, 0(%0)\n\tlwc2 $3, 4(%0)" : : "r"(a1));
    __asm__ volatile ("lwc2 $4, 0(%0)\n\tlwc2 $5, 4(%0)" : : "r"(a2));
    __asm__ volatile ("nop\n\t.word 0x4AD80420");
    t0 = a4;
    t1 = a5;
    __asm__ volatile ("swc2 $20, 0(%0)" : : "r"(a3));
    __asm__ volatile ("swc2 $21, 0(%0)" : : "r"(t0));
    __asm__ volatile ("swc2 $22, 0(%0)" : : "r"(t1));
}

void NormalColorDpq(int *a0, int *a1, int a2, int *a3) {
    __asm__ volatile ("lwc2 $0, 0(%0)\n\tlwc2 $1, 4(%0)" : : "r"(a0));
    __asm__ volatile ("lwc2 $6, 0(%0)" : : "r"(a1));
    __asm__ volatile ("mtc2 %0, $8" : : "r"(a2));
    __asm__ volatile ("nop\n\t.word 0x4AE80413");
    __asm__ volatile ("swc2 $22, 0(%0)" : : "r"(a3));
}

void NormalColorDpq3(int *a0, int *a1, int *a2, int *a3, volatile int a4, int * volatile a5, int * volatile a6, int * volatile a7) {
    register int *t0 __asm__("$8");
    register int *t1 __asm__("$9");
    register int *t2 __asm__("$10");
    __asm__ volatile ("lwc2 $0, 0(%0)\n\tlwc2 $1, 4(%0)" : : "r"(a0));
    __asm__ volatile ("lwc2 $2, 0(%0)\n\tlwc2 $3, 4(%0)" : : "r"(a1));
    __asm__ volatile ("lwc2 $4, 0(%0)\n\tlwc2 $5, 4(%0)" : : "r"(a2));
    __asm__ volatile ("lwc2 $6, 0(%0)" : : "r"(a3));
    __asm__ volatile ("lwc2 $8, %0" : : "m"(a4));
    __asm__ volatile ("nop\n\t.word 0x4AF80416");
    t0 = a5;
    t1 = a6;
    t2 = a7;
    __asm__ volatile ("swc2 $20, 0(%0)" : : "r"(t0));
    __asm__ volatile ("swc2 $21, 0(%0)" : : "r"(t1));
    __asm__ volatile ("swc2 $22, 0(%0)" : : "r"(t2));
}

void NormalColorCol(int *a0, int *a1, int *a2) {
    __asm__ volatile ("lwc2 $0, 0(%0)\n\tlwc2 $1, 4(%0)" : : "r"(a0));
    __asm__ volatile ("lwc2 $6, 0(%0)" : : "r"(a1));
    __asm__ volatile ("nop\n\t.word 0x4B08041B");
    __asm__ volatile ("swc2 $22, 0(%0)" : : "r"(a2));
}

void NormalColorCol3(int *a0, int *a1, int *a2, int *a3, int * volatile a4, int * volatile a5, int * volatile a6) {
    register int *t0 __asm__("$8");
    register int *t1 __asm__("$9");
    register int *t2 __asm__("$10");
    __asm__ volatile ("lwc2 $0, 0(%0)\n\tlwc2 $1, 4(%0)" : : "r"(a0));
    __asm__ volatile ("lwc2 $2, 0(%0)\n\tlwc2 $3, 4(%0)" : : "r"(a1));
    __asm__ volatile ("lwc2 $4, 0(%0)\n\tlwc2 $5, 4(%0)" : : "r"(a2));
    __asm__ volatile ("lwc2 $6, 0(%0)" : : "r"(a3));
    __asm__ volatile ("nop\n\t.word 0x4B18043F");
    t0 = a4;
    t1 = a5;
    t2 = a6;
    __asm__ volatile ("swc2 $20, 0(%0)" : : "r"(t0));
    __asm__ volatile ("swc2 $21, 0(%0)" : : "r"(t1));
    __asm__ volatile ("swc2 $22, 0(%0)" : : "r"(t2));
}

void ColorDpq(int *a0, int *a1, int a2, int *a3) {
    __asm__ volatile ("lwc2 $9, 0(%0)\n\tlwc2 $10, 4(%0)\n\tlwc2 $11, 8(%0)" : : "r"(a0));
    __asm__ volatile ("lwc2 $6, 0(%0)" : : "r"(a1));
    __asm__ volatile ("mtc2 %0, $8" : : "r"(a2));
    __asm__ volatile ("nop\n\t.word 0x4B280414");
    __asm__ volatile ("swc2 $22, 0(%0)" : : "r"(a3));
}

void ColorCol(int *a0, int *a1, int *a2) {
    __asm__ volatile ("lwc2 $9, 0(%0)\n\tlwc2 $10, 4(%0)\n\tlwc2 $11, 8(%0)" : : "r"(a0));
    __asm__ volatile ("lwc2 $6, 0(%0)" : : "r"(a1));
    __asm__ volatile ("nop\n\t.word 0x4B38041C");
    __asm__ volatile ("swc2 $22, 0(%0)" : : "r"(a2));
}

int func_80091498(void) {
    int v;
    RR_GTE_OP(0x4B58002D);
    __asm__ volatile ("mfc2 %0, $7" : "=r"(v));
    return v;
}

int func_800914A8(void) {
    int v;
    RR_GTE_OP(0x4B68002E);
    __asm__ volatile ("mfc2 %0, $7" : "=r"(v));
    return v;
}

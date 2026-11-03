#include "rr/matrix_workspace.h"
void func_800910B4(int *a0);
void func_80091114(int *a0);
void func_80091144(int *a0);
void func_80091164(int *a0);
void func_80091174(int *a0);
void func_80091184(int *a0);
void func_80091194(int *a0, int *a1, int *a2);
void func_800911B4(int *a0, int *a1, int *a2);
void func_800911C8(int a0, int a1, int a2);
void func_800911DC(int a0);
void func_800911E8(int a0, int a1, int a2);
void func_800911FC(int a0, int a1, int a2, int a3);
void func_80091214(int a0, int a1, int a2);
void func_80091228(int a0, int a1, int a2);
void func_8009123C(int a0, int a1, int a2);
void func_80091250(int a0);
void func_8009125C(int a0);
void func_80091268(int a0);
void func_80091274(int a0, int a1, int a2);
void func_80091294(int a0, int a1, int a2);
void func_800912B4(int a0, int a1);

void func_800910B4(int *a0) {
    register int t0 __asm__("$8");
    register int t1 __asm__("$9");
    register int t2 __asm__("$10");
    register int t3 __asm__("$11");
    register int t4 __asm__("$12");
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
}

void func_800910E4(int *a0) {
    register int t0 __asm__("$8");
    register int t1 __asm__("$9");
    register int t2 __asm__("$10");
    register int t3 __asm__("$11");
    register int t4 __asm__("$12");
    t0 = a0[0];
    t1 = a0[1];
    t2 = a0[2];
    t3 = a0[3];
    t4 = a0[4];
    __asm__ volatile ("ctc2 %0, $8" : : "r"(t0));
    __asm__ volatile ("ctc2 %0, $9" : : "r"(t1));
    __asm__ volatile ("ctc2 %0, $10" : : "r"(t2));
    __asm__ volatile ("ctc2 %0, $11" : : "r"(t3));
    __asm__ volatile ("ctc2 %0, $12" : : "r"(t4));
}

void func_80091114(int *a0) {
    register int t0 __asm__("$8");
    register int t1 __asm__("$9");
    register int t2 __asm__("$10");
    register int t3 __asm__("$11");
    register int t4 __asm__("$12");
    t0 = a0[0];
    t1 = a0[1];
    t2 = a0[2];
    t3 = a0[3];
    t4 = a0[4];
    __asm__ volatile ("ctc2 %0, $16" : : "r"(t0));
    __asm__ volatile ("ctc2 %0, $17" : : "r"(t1));
    __asm__ volatile ("ctc2 %0, $18" : : "r"(t2));
    __asm__ volatile ("ctc2 %0, $19" : : "r"(t3));
    __asm__ volatile ("ctc2 %0, $20" : : "r"(t4));
}

void func_80091144(int *a0) {
    register int t0 __asm__("$8");
    register int t1 __asm__("$9");
    register int t2 __asm__("$10");
    t0 = a0[5];
    t1 = a0[6];
    t2 = a0[7];
    __asm__ volatile ("ctc2 %0, $5" : : "r"(t0));
    __asm__ volatile ("ctc2 %0, $6" : : "r"(t1));
    __asm__ volatile ("ctc2 %0, $7" : : "r"(t2));
}

void func_80091164(int *a0) {
    __asm__ volatile ("lwc2 $0, 0(%0)\n\tlwc2 $1, 4(%0)" : : "r"(a0));
}

void func_80091174(int *a0) {
    __asm__ volatile ("lwc2 $2, 0(%0)\n\tlwc2 $3, 4(%0)" : : "r"(a0));
}

void func_80091184(int *a0) {
    __asm__ volatile ("lwc2 $4, 0(%0)\n\tlwc2 $5, 4(%0)" : : "r"(a0));
}

void func_80091194(int *a0, int *a1, int *a2) {
    __asm__ volatile ("lwc2 $0, 0(%0)\n\tlwc2 $1, 4(%0)" : : "r"(a0));
    __asm__ volatile ("lwc2 $2, 0(%0)\n\tlwc2 $3, 4(%0)" : : "r"(a1));
    __asm__ volatile ("lwc2 $4, 0(%0)\n\tlwc2 $5, 4(%0)" : : "r"(a2));
}

void func_800911B4(int *a0, int *a1, int *a2) {
    __asm__ volatile ("lwc2 $20, 0(%0)" : : "r"(a0));
    __asm__ volatile ("lwc2 $21, 0(%0)" : : "r"(a1));
    __asm__ volatile ("lwc2 $22, 0(%0)" : : "r"(a2));
}

void func_800911C8(int a0, int a1, int a2) {
    __asm__ volatile ("mtc2 %0, $9" : : "r"(a0));
    __asm__ volatile ("mtc2 %0, $10" : : "r"(a1));
    __asm__ volatile ("mtc2 %0, $11" : : "r"(a2));
}

void func_800911DC(int a0) {
    __asm__ volatile ("mtc2 %0, $8" : : "r"(a0));
}

void func_800911E8(int a0, int a1, int a2) {
    __asm__ volatile ("mtc2 %0, $17" : : "r"(a0));
    __asm__ volatile ("mtc2 %0, $18" : : "r"(a1));
    __asm__ volatile ("mtc2 %0, $19" : : "r"(a2));
}

void func_800911FC(int a0, int a1, int a2, int a3) {
    __asm__ volatile ("mtc2 %0, $16" : : "r"(a0));
    __asm__ volatile ("mtc2 %0, $17" : : "r"(a1));
    __asm__ volatile ("mtc2 %0, $18" : : "r"(a2));
    __asm__ volatile ("mtc2 %0, $19" : : "r"(a3));
}

void func_80091214(int a0, int a1, int a2) {
    __asm__ volatile ("mtc2 %0, $12" : : "r"(a0));
    __asm__ volatile ("mtc2 %0, $13" : : "r"(a1));
    __asm__ volatile ("mtc2 %0, $14" : : "r"(a2));
}

void func_80091228(int a0, int a1, int a2) {
    __asm__ volatile ("ctc2 %0, $0" : : "r"(a0));
    __asm__ volatile ("ctc2 %0, $2" : : "r"(a1));
    __asm__ volatile ("ctc2 %0, $4" : : "r"(a2));
}

void func_8009123C(int a0, int a1, int a2) {
    __asm__ volatile ("mtc2 %0, $25" : : "r"(a0));
    __asm__ volatile ("mtc2 %0, $26" : : "r"(a1));
    __asm__ volatile ("mtc2 %0, $27" : : "r"(a2));
}

void func_80091250(int a0) {
    __asm__ volatile ("mtc2 %0, $30" : : "r"(a0));
}

void func_8009125C(int a0) {
    __asm__ volatile ("ctc2 %0, $27" : : "r"(a0));
}

void func_80091268(int a0) {
    __asm__ volatile ("ctc2 %0, $28" : : "r"(a0));
}

void func_80091274(int a0, int a1, int a2) {
    a0 <<= 4; a1 <<= 4; a2 <<= 4;
    __asm__ volatile ("ctc2 %0, $13" : : "r"(a0));
    __asm__ volatile ("ctc2 %0, $14" : : "r"(a1));
    __asm__ volatile ("ctc2 %0, $15" : : "r"(a2));
}

void func_80091294(int a0, int a1, int a2) {
    a0 <<= 4; a1 <<= 4; a2 <<= 4;
    __asm__ volatile ("ctc2 %0, $21" : : "r"(a0));
    __asm__ volatile ("ctc2 %0, $22" : : "r"(a1));
    __asm__ volatile ("ctc2 %0, $23" : : "r"(a2));
}

void func_800912B4(int a0, int a1) {
    a0 <<= 16; a1 <<= 16;
    __asm__ volatile ("ctc2 %0, $24" : : "r"(a0));
    __asm__ volatile ("ctc2 %0, $25" : : "r"(a1));
}

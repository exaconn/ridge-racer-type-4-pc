/* GCC_VERSION: 970318 */
void func_800910B4(int *a0);
void func_800910E4(int *a0);
void func_80091114(int *a0);
void func_80091144(int *a0);

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

typedef struct { short s0, s2, s4, s6; int s8, sC; char p10[8]; short s18, s1A; char p1C[16]; short s2C, s2E; } Q;

void func_80047230(a0, a1, a2, a3, a4) Q *a0; Q *a1; int a2; int a3; int *a4; {
    int A[4];
    int B[4];
    /* ASM_BARRIER: pin t1 to $t1 */
    register int t1 __asm__("$9");
    /* ASM_BARRIER: pin temp to $v1 (register choice) */
    register int t __asm__("$3");
    Q *t0;
    int *p4;

    p4 = a4;
    t1 = a2;
    if (a2 < 0) t1 = -t1;
    t0 = a0;
    if (a2 < 0) {
        t = t0->s4 * (t0->s18 + t0->s2E) / t0->s18;
        A[0] = t;
        A[0] = (t * t1) / 1024 + t0->s8;
        t = t0->s6 * (t0->s18 + t0->s2E) / t0->s18;
        A[2] = t;
        A[2] = (t * t1) / 1024 + t0->sC;
        t = a1->s4 * (a1->s18 + a1->s2E) / a1->s18;
        B[0] = t;
        B[0] = (t * t1) / 1024 + a1->s8;
        t = a1->s6 * (a1->s18 + a1->s2E) / a1->s18;
    } else {
        /* ASM_BARRIER: stop reload_cse turning lh a0,0(t0) into lh a0,0(a0) */
        __asm__ volatile ("" : "=r"(t0) : "0"(t0));
        t = t0->s0 * (t0->s1A + t0->s2C) / t0->s1A;
        A[0] = t;
        A[0] = (t * t1) / 1024 + t0->s8;
        t = t0->s2 * (t0->s1A + t0->s2C) / t0->s1A;
        A[2] = t;
        A[2] = (t * t1) / 1024 + t0->sC;
        t = a1->s0 * (a1->s1A + a1->s2C) / a1->s1A;
        B[0] = t;
        B[0] = (t * t1) / 1024 + a1->s8;
        t = a1->s2 * (a1->s1A + a1->s2C) / a1->s1A;
    }
    B[2] = t;
    B[2] = (t * t1) / 1024 + a1->sC;
    /* ASM_BARRIER: keep final A/B reloads after the B[2] store */
    __asm__ volatile ("");
    p4[0] = (B[0] - A[0]) * a3 / 1024 + A[0];
    p4[2] = (B[2] - A[2]) * a3 / 1024 + A[2];
}

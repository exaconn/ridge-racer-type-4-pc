#include "types.h"

int *ScaleMatrix(int *a0_in, int *a1) {
    /* ASM_BARRIER: keep first argument pointer in $a0 ($4) */
    register int *a0 __asm__("$4") = a0_in;
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
    /* ASM_BARRIER: bind 64-bit multiply output directly to HI/LO pair ($64/$65) */
    union {
        u64 d;
        struct {
            s32 hi;
            s32 lo;
        } w;
    } hilo __asm__("hi");

    t3 = a1[0];
    t4 = a1[1];
    t5 = a1[2];
    /* ASM_BARRIER: keep a1 loads ordered before a0[0] load */
    __asm__ volatile("" : "+r"(t3), "+r"(t4), "+r"(t5), "+r"(a0));

    t0 = a0[0];
    t1 = t0 & 0xFFFF;
    /* ASM_BARRIER: preserve explicit andi 0xFFFF before sign-extend shifts */
    __asm__ volatile("" : "+r"(t1));
    t1 = (t1 << 16) >> 16;
    hilo.d = (u64)(u32)t1 * (u32)t3;
    t1 = hilo.w.lo;
    /* ASM_BARRIER: order mflo before sra 12 */
    __asm__ volatile("" : "+r"(t1));
    t1 >>= 12;
    t1 &= 0xFFFF;
    t2 = t0 >> 16;
    /* ASM_BARRIER: order sra 16 before multu */
    __asm__ volatile("" : "+r"(t2));
    hilo.d = (u64)(u32)t2 * (u32)t4;
    t2 = hilo.w.lo;
    /* ASM_BARRIER: order mflo before sra 12 */
    __asm__ volatile("" : "+r"(t2));
    t2 >>= 12;
    t2 <<= 16;
    t1 |= t2;
    a0[0] = t1;
    /* ASM_BARRIER: keep a0[0] store ordered before a0[1] load */
    __asm__ volatile("" : "+r"(a0) : "r"(t1));

    t0 = a0[1];
    t1 = t0 & 0xFFFF;
    /* ASM_BARRIER: preserve explicit andi 0xFFFF before sign-extend shifts */
    __asm__ volatile("" : "+r"(t1));
    t1 = (t1 << 16) >> 16;
    hilo.d = (u64)(u32)t1 * (u32)t5;
    t1 = hilo.w.lo;
    /* ASM_BARRIER: order mflo before sra 12 */
    __asm__ volatile("" : "+r"(t1));
    t1 >>= 12;
    t1 &= 0xFFFF;
    t2 = t0 >> 16;
    /* ASM_BARRIER: order sra 16 before multu */
    __asm__ volatile("" : "+r"(t2));
    hilo.d = (u64)(u32)t2 * (u32)t3;
    t2 = hilo.w.lo;
    /* ASM_BARRIER: order mflo before sra 12 */
    __asm__ volatile("" : "+r"(t2));
    t2 >>= 12;
    t2 <<= 16;
    t1 |= t2;
    a0[1] = t1;
    /* ASM_BARRIER: keep a0[1] store ordered before a0[2] load */
    __asm__ volatile("" : "+r"(a0) : "r"(t1));

    t0 = a0[2];
    t1 = t0 & 0xFFFF;
    /* ASM_BARRIER: preserve explicit andi 0xFFFF before sign-extend shifts */
    __asm__ volatile("" : "+r"(t1));
    t1 = (t1 << 16) >> 16;
    hilo.d = (u64)(u32)t1 * (u32)t4;
    t1 = hilo.w.lo;
    /* ASM_BARRIER: order mflo before sra 12 */
    __asm__ volatile("" : "+r"(t1));
    t1 >>= 12;
    t1 &= 0xFFFF;
    t2 = t0 >> 16;
    /* ASM_BARRIER: order sra 16 before multu */
    __asm__ volatile("" : "+r"(t2));
    hilo.d = (u64)(u32)t2 * (u32)t5;
    t2 = hilo.w.lo;
    /* ASM_BARRIER: order mflo before sra 12 */
    __asm__ volatile("" : "+r"(t2));
    t2 >>= 12;
    t2 <<= 16;
    t1 |= t2;
    a0[2] = t1;
    /* ASM_BARRIER: keep a0[2] store ordered before a0[3] load */
    __asm__ volatile("" : "+r"(a0) : "r"(t1));

    t0 = a0[3];
    t1 = t0 & 0xFFFF;
    /* ASM_BARRIER: preserve explicit andi 0xFFFF before sign-extend shifts */
    __asm__ volatile("" : "+r"(t1));
    t1 = (t1 << 16) >> 16;
    hilo.d = (u64)(u32)t1 * (u32)t3;
    t1 = hilo.w.lo;
    /* ASM_BARRIER: order mflo before sra 12 */
    __asm__ volatile("" : "+r"(t1));
    t1 >>= 12;
    t1 &= 0xFFFF;
    t2 = t0 >> 16;
    /* ASM_BARRIER: order sra 16 before multu */
    __asm__ volatile("" : "+r"(t2));
    hilo.d = (u64)(u32)t2 * (u32)t4;
    t2 = hilo.w.lo;
    /* ASM_BARRIER: order mflo before sra 12 */
    __asm__ volatile("" : "+r"(t2));
    t2 >>= 12;
    t2 <<= 16;
    t1 |= t2;
    a0[3] = t1;
    /* ASM_BARRIER: keep a0[3] store ordered before a0[4] load */
    __asm__ volatile("" : "+r"(a0) : "r"(t1));

    t0 = a0[4];
    /* ASM_BARRIER: keep a0[4] load in $t0 */
    __asm__ volatile("" : "+r"(t0));
    t1 = t0 & 0xFFFF;
    /* ASM_BARRIER: preserve explicit andi 0xFFFF before sign-extend shifts */
    __asm__ volatile("" : "+r"(t1));
    t1 = (t1 << 16) >> 16;
    hilo.d = (u64)(u32)t1 * (u32)t5;
    t1 = hilo.w.lo;
    /* ASM_BARRIER: order mflo before sra 12 */
    __asm__ volatile("" : "+r"(t1));
    t1 >>= 12;
    a0[4] = t1;
    /* ASM_BARRIER: order final store before return value copy in delay slot */
    __asm__ volatile("" : : "r"(t1) : "memory");
    return a0;
}

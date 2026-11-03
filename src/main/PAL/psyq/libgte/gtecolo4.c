/* GCC_VERSION: 970318 */
#include "rr/geometry.h"
#include "rr_asm.h"

void func_800914C4(int *a0, int *a1);
void func_800914EC(int *a0, int *a1, int a2, int *a3);
void func_80091514(int *a0, int *a1, int *a2, int a3, int * volatile a4, int * volatile a5, int * volatile a6);
void func_80091550(int *a0, int a1, int *a2);
RrVec3i *func_80091574(const RrVec3i *input,RrVec3i *output);
RrVec3i *func_8009159C(const RrVec3i *input,RrVec3i *output);
int func_800915C4(int a0, int a1, int a2);
int func_800915E4(int a0, int a1, int a2, int a3);
void func_80091608(int *a0, int *a1, int *a2);
void func_80091660(int *a0, int *a1, int *a2);
int func_800916B8(int a0);

void func_800914C4(int *a0, int *a1) {
    __asm__ volatile ("lwc2 $9, 0(%0)\n\tlwc2 $10, 4(%0)\n\tlwc2 $11, 8(%0)" : : "r"(a0));
    __asm__ volatile ("nop\n\t.word 0x4A4DA412");
    __asm__ volatile ("swc2 $9, 0(%0)\n\tswc2 $10, 4(%0)\n\tswc2 $11, 8(%0)" : : "r"(a1));
}

void func_800914EC(int *a0, int *a1, int a2, int *a3) {
    __asm__ volatile ("lwc2 $9, 0(%0)\n\tlwc2 $10, 4(%0)\n\tlwc2 $11, 8(%0)" : : "r"(a0));
    __asm__ volatile ("lwc2 $6, 0(%0)" : : "r"(a1));
    __asm__ volatile ("mtc2 %0, $8" : : "r"(a2));
    __asm__ volatile ("nop\n\t.word 0x4A680029");
    __asm__ volatile ("swc2 $22, 0(%0)" : : "r"(a3));
}

void func_80091514(int *a0, int *a1, int *a2, int a3, int * volatile a4, int * volatile a5, int * volatile a6) {
    register int *t0 __asm__("$8");
    register int *t1 __asm__("$9");
    register int *t2 __asm__("$10");
    __asm__ volatile ("lwc2 $20, 0(%0)" : : "r"(a0));
    __asm__ volatile ("lwc2 $21, 0(%0)" : : "r"(a1));
    __asm__ volatile ("lwc2 $22, 0(%0)" : : "r"(a2));
    __asm__ volatile ("lwc2 $6, 0(%0)" : : "r"(a2));
    __asm__ volatile ("mtc2 %0, $8" : : "r"(a3));
    __asm__ volatile ("nop\n\t.word 0x4AF8002A");
    t0 = a4;
    t1 = a5;
    t2 = a6;
    __asm__ volatile ("swc2 $20, 0(%0)" : : "r"(t0));
    __asm__ volatile ("swc2 $21, 0(%0)" : : "r"(t1));
    __asm__ volatile ("swc2 $22, 0(%0)" : : "r"(t2));
}

void func_80091550(int *a0, int a1, int *a2) {
    __asm__ volatile ("lwc2 $9, 0(%0)\n\tlwc2 $10, 4(%0)\n\tlwc2 $11, 8(%0)" : : "r"(a0));
    __asm__ volatile ("mtc2 %0, $8" : : "r"(a1));
    __asm__ volatile ("nop\n\t.word 0x4A980011");
    __asm__ volatile ("swc2 $22, 0(%0)" : : "r"(a2));
}

RrVec3i *func_80091574(const RrVec3i *input,RrVec3i *output) {
    RR_GTE_LWC2_IR(input);
    /* ASM_BARRIER: Leave the original GTE input-load pipeline gap before SQR. */
    RR_NOP_DEP();
    RR_GTE_SQR12();
    RR_GTE_SWC2_MAC123(output);
    /* ASM_BARRIER: Finish hardware stores before forming the returned pointer. */
    __asm__("" : : "r"(output) : "$2");
    return output;
}

RrVec3i *func_8009159C(const RrVec3i *input,RrVec3i *output) {
    RR_GTE_LWC2_IR(input);
    /* ASM_BARRIER: Leave the original GTE input-load pipeline gap before SQR. */
    RR_NOP_DEP();
    RR_GTE_SQR0();
    RR_GTE_SWC2_MAC123(output);
    /* ASM_BARRIER: Finish hardware stores before forming the returned pointer. */
    __asm__("" : : "r"(output) : "$2");
    return output;
}

int func_800915C4(int a0, int a1, int a2) {
    int v;
    __asm__ volatile ("mtc2 %0, $17" : : "r"(a0));
    __asm__ volatile ("mtc2 %0, $18" : : "r"(a1));
    __asm__ volatile ("mtc2 %0, $19" : : "r"(a2));
    __asm__ volatile ("nop\n\t.word 0x4B58002D");
    __asm__ volatile ("mfc2 %0, $7" : "=r"(v));
    return v;
}

int func_800915E4(int a0, int a1, int a2, int a3) {
    int v;
    __asm__ volatile ("mtc2 %0, $16" : : "r"(a0));
    __asm__ volatile ("mtc2 %0, $17" : : "r"(a1));
    __asm__ volatile ("mtc2 %0, $18" : : "r"(a2));
    __asm__ volatile ("mtc2 %0, $19" : : "r"(a3));
    __asm__ volatile ("nop\n\t.word 0x4B68002E");
    __asm__ volatile ("mfc2 %0, $7" : "=r"(v));
    return v;
}

void func_80091608(int *a0, int *a1, int *a2) {
    register int t0 __asm__("$8");
    register int t1 __asm__("$9");
    register int t2 __asm__("$10");
    register int t5 __asm__("$13");
    register int t6 __asm__("$14");
    register int t7 __asm__("$15");
    __asm__ volatile ("cfc2 %0, $0" : "=r"(t5));
    __asm__ volatile ("cfc2 %0, $2" : "=r"(t6));
    __asm__ volatile ("cfc2 %0, $4" : "=r"(t7));
    t0 = a0[0];
    t1 = a0[1];
    t2 = a0[2];
    __asm__ volatile ("ctc2 %0, $0" : : "r"(t0));
    __asm__ volatile ("ctc2 %0, $2" : : "r"(t1));
    __asm__ volatile ("ctc2 %0, $4" : : "r"(t2));
    __asm__ volatile ("lwc2 $11, 8(%0)\n\tlwc2 $9, 0(%0)\n\tlwc2 $10, 4(%0)" : : "r"(a1));
    __asm__ volatile ("nop\n\t.word 0x4B78000C");
    __asm__ volatile ("swc2 $25, 0(%0)\n\tswc2 $26, 4(%0)\n\tswc2 $27, 8(%0)" : : "r"(a2));
    __asm__ volatile ("ctc2 %0, $0" : : "r"(t5));
    __asm__ volatile ("ctc2 %0, $2" : : "r"(t6));
    __asm__ volatile ("ctc2 %0, $4" : : "r"(t7));
}

void func_80091660(int *a0, int *a1, int *a2) {
    register int t0 __asm__("$8");
    register int t1 __asm__("$9");
    register int t2 __asm__("$10");
    register int t5 __asm__("$13");
    register int t6 __asm__("$14");
    register int t7 __asm__("$15");
    __asm__ volatile ("cfc2 %0, $0" : "=r"(t5));
    __asm__ volatile ("cfc2 %0, $2" : "=r"(t6));
    __asm__ volatile ("cfc2 %0, $4" : "=r"(t7));
    t0 = a0[0];
    t1 = a0[1];
    t2 = a0[2];
    __asm__ volatile ("ctc2 %0, $0" : : "r"(t0));
    __asm__ volatile ("ctc2 %0, $2" : : "r"(t1));
    __asm__ volatile ("ctc2 %0, $4" : : "r"(t2));
    __asm__ volatile ("lwc2 $11, 8(%0)\n\tlwc2 $9, 0(%0)\n\tlwc2 $10, 4(%0)" : : "r"(a1));
    __asm__ volatile ("nop\n\t.word 0x4B70000C");
    __asm__ volatile ("swc2 $25, 0(%0)\n\tswc2 $26, 4(%0)\n\tswc2 $27, 8(%0)" : : "r"(a2));
    __asm__ volatile ("ctc2 %0, $0" : : "r"(t5));
    __asm__ volatile ("ctc2 %0, $2" : : "r"(t6));
    __asm__ volatile ("ctc2 %0, $4" : : "r"(t7));
}

int func_800916B8(int a0) {
    int v;
    __asm__ volatile ("mtc2 %0, $30" : : "r"(a0));
    __asm__ volatile ("nop\n\tnop");
    __asm__ volatile ("mfc2 %0, $31" : "=r"(v));
    return v;
}

#include "psyq/libgte/gtemath2.h"
void func_80091704(int *a0, int *a1, int *a2) {
    int f;
    __asm__ volatile ("lwc2 $0, 0(%0)\n\tlwc2 $1, 4(%0)" : : "r"(a0));
    __asm__ volatile ("nop\n\t.word 0x4A480012");
    __asm__ volatile ("swc2 $25, 0(%0)\n\tswc2 $26, 4(%0)\n\tswc2 $27, 8(%0)" : : "r"(a1));
    __asm__ volatile ("cfc2 %0, $31" : "=r"(f));
    *a2 = f;
}

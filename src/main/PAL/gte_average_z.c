/* Candidate module: GTE AverageZ3 / AverageZ4 wrappers, 80091498..800914B8. */
#include "rr_asm.h"

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

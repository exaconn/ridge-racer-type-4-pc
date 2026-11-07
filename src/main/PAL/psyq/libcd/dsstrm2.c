/* CC1_FLAGS: -mno-split-addresses */
#include "psyq/libcd/dsstrm23.h"
#include "psyq/libcd/dsmedia2.h"
#include "psyq/libcd/dsstrm2.h"
#include "psyq/libcd/dsstrm32.h"
extern int D_801102D0;
extern int D_80112760;
extern int D_80112764;
void StSetStream(int a, int b, int c, int d, int e) {
    StSetMask(1);
    D_80110300 = 0;
    D_80112760 = d;
    D_801102D8 = a & 1;
    D_801102E8 = 0;
    D_801102E0 = 0;
    D_801102D4 = 0;
    D_801102D0 = 0;
    D_80112764 = e;
    /* ASM_BARRIER: keeps epilogue stack loads (lw ra/s*) after the final global stores; sched2 otherwise hoists/sinks them */
    __asm__ volatile ("" : : : "memory");
}

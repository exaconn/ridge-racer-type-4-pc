#include "psyq/libcd/dsfinish.h"
#include "psyq/libcd/dsread.h"
int cd_read(a, b, c) int a; int b; int c; {
    int buf[2];
    int z, r;
    CdIntToPos(b, buf);
    CdControl(2, buf, 0);
    CdRead(a, c, 0x80);
    /* ASM_BARRIER: stops cse turning the second zero argument into a copy of a0 */
    __asm__("" : "=r"(z) : "0"(0));
    r = CdReadSync(z, 0) == 0;
    /* ASM_BARRIER: keeps sltiu directly after the call (before the epilogue loads) */
    __asm__ volatile("" : "=r"(r) : "0"(r));
    return r;
}

#include "psyq/libgpu/addprim3.h"
void AddPrims(unsigned *ot, unsigned p, unsigned *q) {
    register unsigned lo asm("$7") = 0xFFFFFF;
    unsigned hi = 0xFF000000;
    *q = (*q & hi) | (*ot & lo);
    *ot = (*ot & hi) | (p & lo);
}

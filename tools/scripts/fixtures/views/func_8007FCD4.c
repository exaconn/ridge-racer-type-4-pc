/* GCC_VERSION: 2.7.2 */

#include "rr/spu.h"

/* legacy typing of shared globals in this part of the unit */
#define _spu_memList ((unsigned int *)_spu_memList)

#undef _spu_memList

int _SpuIsInAllocateArea_(a0)
unsigned int a0;
{
    unsigned int *p, *q;
    unsigned int v, m1, m2, m3;

    a0 <<= _spu_mem_mode_plus;
    q = _spu_memList;
    if (q == 0) return 0;
    {
        m1 = 0x80000000;
        m2 = 0x40000000;
        m3 = 0xFFFFFFF;
        p = q;
        for (;; p += 2) {
            v = p[0];
            if (v & m1) continue;
            if (v & m2) goto zero;
            v &= m3;
            if (v >= a0) return 1;
            if (a0 < v + p[1]) return 1;
        }
    }
zero:
    return 0;
}

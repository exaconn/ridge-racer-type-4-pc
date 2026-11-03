/* GCC_VERSION: 2.7.2 */
#include "rr/spu.h"
#include "psyq/libspu/smutil.h"

s32 func_8007FC54(u32 address) {
    RrSpuAllocation *initial=D_800A48C0;
    RrSpuAllocation *p;
    u32 value;
    u32 skip=0x80000000U;
    u32 end;
    u32 low;
    s32 result;
    if(!initial) {
        result=0;goto out;
    }
    end=0x40000000U;
    low=0x0FFFFFFFU;
    p=initial;
    for(;;p++) {
        value=p->address_flags;
        if(value&skip) continue;
        if(value&end) break;
        value &= low;
        if(value>=address) {result=1;goto out;}
        if(address<value+p->size) {result=1;goto out;}
    }
    result=0;
out:
    return result;
}

/* legacy typing of shared globals in this part of the unit */
#define D_800A48C0 ((unsigned int *)D_800A48C0)

int func_8007FCD4(a0)
unsigned int a0;
{
    unsigned int *p, *q;
    unsigned int v, m1, m2, m3;

    a0 <<= D_800A4800;
    q = D_800A48C0;
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

#undef D_800A48C0

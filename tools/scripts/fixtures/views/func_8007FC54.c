/* GCC_VERSION: 2.7.2 */

#include "rr/spu.h"

/* legacy typing of shared globals in this part of the unit */
#define _spu_memList ((unsigned int *)_spu_memList)

#undef _spu_memList

s32 _SpuIsInAllocateArea(u32 address) {
    SpuAllocation *initial=_spu_memList;
    SpuAllocation *p;
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

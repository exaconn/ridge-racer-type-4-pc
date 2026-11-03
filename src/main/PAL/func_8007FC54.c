/* GCC_VERSION: 2.7.2 */
#include "rr/spu.h"
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

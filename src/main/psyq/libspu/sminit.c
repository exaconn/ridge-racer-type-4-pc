#include "psyq/libspu/sminit.h"
#include "rr/spu.h"
#include "rr/spu_allocation.h"
extern int _spu_mem_mode_plus;
int SpuInitMalloc(a0, a1) int a0; int *a1; {
    if (a0 > 0) {
        int sh = _spu_mem_mode_plus;
        a1[0] = 0x40001010;
        _spu_memList = a1;
        _spu_AllocLastNum = 0;
        _spu_AllocBlockNum = a0;
        a1[1] = (0x10000 << sh) - 0x1010;
        return a0;
    }
    return 0;
}

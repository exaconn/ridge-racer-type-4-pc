/* GCC_VERSION: 2.7.2 */
#include "psyq/libspu/sminit.h"
#include "rr/spu.h"
#include "rr/spu_allocation.h"
extern int D_800A4800;
int func_80087D44(a0, a1) int a0; int *a1; {
    if (a0 > 0) {
        int sh = D_800A4800;
        a1[0] = 0x40001010;
        D_800A48C0 = a1;
        D_800A48BC = 0;
        D_800A48B8 = a0;
        a1[1] = (0x10000 << sh) - 0x1010;
        return a0;
    }
    return 0;
}

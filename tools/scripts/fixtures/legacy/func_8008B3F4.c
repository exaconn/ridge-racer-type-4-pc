/* GCC_VERSION: 2.7.2 */
#include "rr/spu_tick_control.h"
void VSyncCallback(RrSpuTickCallback callback)
{
    ((RrSpuTickInterruptDriver20 *)D_800A6350)->install14(4,callback);
}

/* GCC_VERSION: 2.7.2 */
#include "rr/spu_tick_control.h"
void VSyncCallback(RrSpuTickCallback callback)
{
    ((RrSpuTickInterruptDriver20 *)IntrDriverTablePtr)->install14(4,callback);
}

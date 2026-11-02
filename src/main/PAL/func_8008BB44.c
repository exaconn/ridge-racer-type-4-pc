/* GCC_VERSION: 2.7.2 */
#include "rr/library.h"
RrLibInterruptExchange func_8008BB44(void) {
    func_8008BDBC(D_800A63A4, 8);
    /* The ordinary write view permits the original call-delay store. */
    *(u32 *)D_800A63A0 = 0;
    func_8008B394(3, func_8008BB90);
    return func_8008BD10;
}

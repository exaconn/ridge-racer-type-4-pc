#include "rr/overlay667_status.h"
s32 ovl667_check_sio_timeout(s32 arg0, u32 arg1) {
    /* ASM_BARRIER: Hold constant 2 and return value in $2 so dbr_schedule fills bne delay slot */
    register s32 ret __asm__("$2") = 2;
    if (arg0 == ret) {
        ret = 0;
        if (arg1 < 25000U) {
            ret = 1;
            /* ASM_BARRIER: Prevent dbr_schedule from duplicating jr ra for the inner branch */
            __asm__("" : "=r"(ret) : "0"(ret));
        }
    } else {
        ret = 1;
    }
    return ret;
}

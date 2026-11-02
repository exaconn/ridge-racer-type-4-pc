/* GCC_VERSION: 2.7.2 */
#include "rr/interrupts.h"
extern u16 *D_800A6358;
s32 func_8008B4E0(s32 value) {
    s32 previous=*D_800A6358;
    *D_800A6358=value;
    return previous;
}

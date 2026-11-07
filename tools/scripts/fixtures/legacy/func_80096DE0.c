/* GCC_VERSION: 2.7.2 */
#include "types.h"
#include "m2c_macros.h"

extern int DMACallback();

void func_80096DE0(s32 arg0) {
    DMACallback(3, arg0);
}

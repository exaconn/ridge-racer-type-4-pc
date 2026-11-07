/* GCC_VERSION: 2.7.2 */
#include "types.h"
#include "m2c_macros.h"

extern int DMACallback();

void CdAltDataCallback(s32 arg0) {
    DMACallback(3, arg0);
}
